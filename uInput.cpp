/*
    This file is part of libuInputPlus.
    Copyright (C) 2018 YukiWorkshop

    This program is free software: you can redistribute it and/or modify
    it under the terms of the MIT License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/


#include "CommonIncludes.hpp"
#include "uInputSetup.hpp"
#include "uInput.hpp"

using namespace uInputPlus;


uInput::uInput(const uInputSetup &setup) {
	Init(setup);
}

uInput::~uInput() {
	Destroy();
}

void uInput::Init(const uInputSetup &setup) {
	FD = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if (FD < 0)
		throw std::runtime_error("failed to open uinput device");

	for (auto it : setup.Events) {
		if (ioctl(FD, UI_SET_EVBIT, it))
			throw std::runtime_error("UI_SET_EVBIT ioctl failed");
	}

	if (setup.Events.find(EV_KEY) != setup.Events.end()) {
		for (auto it : setup.Keys) {
			if (ioctl(FD, UI_SET_KEYBIT, it))
				throw std::runtime_error("UI_SET_EVBIT ioctl failed");
		}
	}

	if (setup.Events.find(EV_REL) != setup.Events.end()) {
		for (auto it : setup.Rels) {
			if (ioctl(FD, UI_SET_RELBIT, it))
				throw std::runtime_error("UI_SET_RELBIT ioctl failed");
		}
	}


//	uinput_abs_setup s1, s2, s3, s4;
//
//	memset(&s1, 0, sizeof(uinput_abs_setup));
//	memset(&s2, 0, sizeof(uinput_abs_setup));
//	memset(&s3, 0, sizeof(uinput_abs_setup));
//	memset(&s4, 0, sizeof(uinput_abs_setup));
//
//	s1.code = ABS_MT_POSITION_X;
//	s2.code = ABS_MT_POSITION_Y;
//	s3.code = ABS_MT_SLOT;
//	s4.code = ABS_MT_TRACKING_ID;
//
//	s1.absinfo.maximum = s2.absinfo.maximum = 4000;
//	s3.absinfo.maximum = 5;
//	s4.absinfo.maximum = 65535;
//
//	ioctl(FD, UI_ABS_SETUP, &s1);
//	ioctl(FD, UI_ABS_SETUP, &s2);
//	ioctl(FD, UI_ABS_SETUP, &s3);
//	ioctl(FD, UI_ABS_SETUP, &s4);


	if (setup.Events.find(EV_ABS) != setup.Events.end()) {

		for (auto &it : setup.AbsSetup) {
			if (ioctl(FD, UI_SET_ABSBIT, it.Code))
				throw std::runtime_error("UI_SET_ABSBIT ioctl failed");

			if (ioctl(FD, UI_ABS_SETUP, &(it.setup)))
				throw std::runtime_error("UI_ABS_SETUP ioctl failed");
		}
	}

	for (auto &it : setup.Props) {
		if (ioctl(FD, UI_SET_PROPBIT, it))
			throw std::runtime_error("UI_SET_PROPBIT ioctl failed");
	}

	if (ioctl(FD, UI_DEV_SETUP, &(setup.DeviceInfo.usetup)))
		throw std::runtime_error("UI_DEV_SETUP ioctl failed");

	if (ioctl(FD, UI_DEV_CREATE))
		throw std::runtime_error("UI_DEV_CREATE ioctl failed");

}

void uInput::Destroy() {
	if (FD > 0) {
		ioctl(FD, UI_DEV_DESTROY);
		close(FD);
		FD = -1;
	}
}

void uInput::Emit(uint16_t type, uint16_t code, int32_t val) const {
	if (custom_callback) {
		int rc_ccb = custom_callback(type, code, val, custom_data);
	} else {
		input_event ie{};

		ie.type = type;
		ie.code = code;
		ie.value = val;

		write(FD, &ie, sizeof(ie));
	}
}

void uInput::SendKey(uint16_t key_code, uint32_t value, bool report) const {
	Emit(EV_KEY, key_code, value);

	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::SendKeyPress(const std::initializer_list<uint16_t> &keycodes, bool report) const {
	for (auto it : keycodes) {
		SendKey(it, 1, report);
		SendKey(it, 0, report);
	}
}

void uInput::SendKeyPress(std::vector<std::pair<int, int>> keys, bool report) const {

}

void uInput::RelativeMove(const uInputCoordinate &movement, bool report) const {
	if (movement.X)
		Emit(EV_REL, REL_X, movement.X);

	if (movement.Y)
		Emit(EV_REL, REL_Y, movement.Y);

	if (movement.Z)
		Emit(EV_REL, REL_Z, movement.Z);


	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);

}

void uInput::RelativeWheel(int32_t movement, bool h, bool report) const {
	Emit(EV_REL, h ? REL_HWHEEL : REL_WHEEL, movement);

	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::AbsoluteWheel(int32_t movement, bool report) const {
	Emit(EV_ABS, ABS_WHEEL, movement);

	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::AbsolutePosition(const uInputCoordinate &pos, int32_t mt_slot, bool report) const {
	if (mt_slot == -1) {
		if (pos.X)
			Emit(EV_ABS, ABS_X, pos.X);

		if (pos.Y)
			Emit(EV_ABS, ABS_Y, pos.Y);

		if (pos.Z)
			Emit(EV_ABS, ABS_Z, pos.Z);
	} else {
		Emit(EV_ABS, ABS_MT_SLOT, mt_slot);

		if (pos.X)
			Emit(EV_ABS, ABS_MT_POSITION_X, pos.X);

		if (pos.Y)
			Emit(EV_ABS, ABS_MT_POSITION_Y, pos.Y);

	}

	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::EmulateSmoothScroll(int offset, bool report) const {

	int polar = (offset / abs(offset)) * 4;
	int32_t ypos_expected = 1000 + offset;

	Emit(EV_ABS, ABS_MT_SLOT, 0);
	Emit(EV_ABS, ABS_MT_TRACKING_ID, 2000);
	Emit(EV_ABS, ABS_MT_POSITION_X, 1000);
	Emit(EV_ABS, ABS_MT_POSITION_Y, 1000);
	Emit(EV_ABS, ABS_MT_PRESSURE, 70);
	Emit(EV_KEY, BTN_TOUCH, 1);
	Emit(EV_ABS, ABS_X, 1000);
	Emit(EV_ABS, ABS_Y, 1000);
	Emit(EV_ABS, ABS_PRESSURE, 70);
	Emit(EV_KEY, BTN_TOOL_FINGER, 1);
	Emit(EV_SYN, SYN_REPORT, 0);

	//
	Emit(EV_ABS, ABS_MT_POSITION_X, 1000);
	Emit(EV_ABS, ABS_MT_POSITION_Y, 1000);
	Emit(EV_ABS, ABS_MT_PRESSURE, 70);
	Emit(EV_ABS, ABS_MT_SLOT, 1);
	Emit(EV_ABS, ABS_MT_TRACKING_ID, 2001);
	Emit(EV_ABS, ABS_MT_POSITION_X, 1100);
	Emit(EV_ABS, ABS_MT_POSITION_Y, 1000);
	Emit(EV_ABS, ABS_MT_PRESSURE, 70);
	Emit(EV_ABS, ABS_X, 1000);
	Emit(EV_ABS, ABS_Y, 1000);
	Emit(EV_ABS, ABS_PRESSURE, 70);
	Emit(EV_KEY, BTN_TOOL_FINGER, 0);
	Emit(EV_KEY, BTN_TOOL_DOUBLETAP, 1);
	Emit(EV_SYN, SYN_REPORT, 0);

//	Emit(EV_KEY, BTN_TOUCH, 1);
//
//	Emit(EV_ABS, ABS_X, 1000);
//	Emit(EV_ABS, ABS_Y, 1000);
//
//	Emit(EV_KEY, BTN_TOOL_DOUBLETAP, 1);
//
//	Emit(EV_SYN, SYN_REPORT, 0);

	int32_t ypos;

	for (ypos = 1000; ypos != (ypos_expected / 4 * 4); ypos += polar) {
		std::cout << ypos << "\n";
		AbsolutePosition({1000, ypos}, 0, false);
		AbsolutePosition({1000, ypos}, 1, false);
		AbsolutePosition({1000, ypos}, -1, false);
		Emit(EV_SYN, SYN_REPORT, 0);
		usleep(1000*5);
	}

	Emit(EV_ABS, ABS_MT_POSITION_X, 1000);
	Emit(EV_ABS, ABS_MT_POSITION_Y, ypos);
	Emit(EV_ABS, ABS_MT_SLOT, 0);
	Emit(EV_ABS, ABS_MT_TRACKING_ID, -1);
	Emit(EV_ABS, ABS_X, 1000);
	Emit(EV_ABS, ABS_Y, ypos);
	Emit(EV_KEY, BTN_TOOL_FINGER, 1);
	Emit(EV_KEY, BTN_TOOL_DOUBLETAP, 0);
	Emit(EV_SYN, SYN_REPORT, 0);

	Emit(EV_ABS, ABS_MT_SLOT, 1);
	Emit(EV_ABS, ABS_MT_TRACKING_ID, -1);
	Emit(EV_KEY, BTN_TOUCH, 0);
	Emit(EV_KEY, BTN_TOOL_FINGER, 0);
	Emit(EV_SYN, SYN_REPORT, 0);

//	Emit(EV_KEY, BTN_TOOL_DOUBLETAP, 1);

}

void uInput::Init(int (*__custom_callback)(uint16_t, uint16_t, int32_t, void *), void *__userp) {
	custom_callback = __custom_callback;
	custom_data = __userp;
}
