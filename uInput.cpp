/*
    This file is part of libuInputPlus.
    Copyright (C) 2018-2021 Reimu NotMoe <reimu@sudomaker.com>

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
	__init(setup);
}

uInput::uInput(const uInputSetup &setup, std::function<int(uint16_t, uint16_t, int32_t)> custom_callback) {
	__init(setup);
	custom_callback_ = std::move(custom_callback);
}

uInput::~uInput() {
	__deinit();
}

void uInput::__init(const uInputSetup &setup) {
	fd_ = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if (fd_ < 0)
		throw std::system_error(errno, std::system_category(), "failed to open uinput device");

	for (auto it : setup.Events) {
		if (ioctl(fd_, UI_SET_EVBIT, it))
			throw std::system_error(errno, std::system_category(), "UI_SET_EVBIT ioctl failed");
	}

	if (setup.Events.find(EV_KEY) != setup.Events.end()) {
		for (auto it : setup.Keys) {
			if (ioctl(fd_, UI_SET_KEYBIT, it))
				throw std::system_error(errno, std::system_category(), "UI_SET_EVBIT ioctl failed");
		}
	}

	if (setup.Events.find(EV_REL) != setup.Events.end()) {
		for (auto it : setup.Rels) {
			if (ioctl(fd_, UI_SET_RELBIT, it))
				throw std::system_error(errno, std::system_category(), "UI_SET_RELBIT ioctl failed");
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
			if (ioctl(fd_, UI_SET_ABSBIT, it.Code))
				throw std::system_error(errno, std::system_category(), "UI_SET_ABSBIT ioctl failed");

			if (ioctl(fd_, UI_ABS_SETUP, &(it.setup)))
				throw std::system_error(errno, std::system_category(), "UI_ABS_SETUP ioctl failed");
		}
	}

	for (auto &it : setup.Props) {
		if (ioctl(fd_, UI_SET_PROPBIT, it))
			throw std::system_error(errno, std::system_category(), "UI_SET_PROPBIT ioctl failed");
	}

	if (ioctl(fd_, UI_DEV_SETUP, &(setup.DeviceInfo.usetup)))
		throw std::system_error(errno, std::system_category(), "UI_DEV_SETUP ioctl failed");

	if (ioctl(fd_, UI_DEV_CREATE))
		throw std::system_error(errno, std::system_category(), "UI_DEV_CREATE ioctl failed");

}

void uInput::__deinit() {
	if (fd_ > 0) {
		ioctl(fd_, UI_DEV_DESTROY);
		close(fd_);
		fd_ = -1;
	}
}

void uInput::emit(uint16_t type, uint16_t code, int32_t val) const {
	if (custom_callback_) {
		custom_callback_(type, code, val);
	} else {
		input_event ie{};

		ie.type = type;
		ie.code = code;
		ie.value = val;

		write(fd_, &ie, sizeof(ie));
	}
}

void uInput::send_key(uint16_t key_code, uint32_t value, bool report) const {
	emit(EV_KEY, key_code, value);

	if (report)
		emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::send_keypress(const std::initializer_list<uint16_t> &keycodes, bool report) const {
	for (auto it : keycodes) {
		send_key(it, 1, report);
		send_key(it, 0, report);
	}
}

void uInput::send_keypress(std::vector<std::pair<int, int>> keys, bool report) const {

}

void uInput::send_pos_relative(const uInputCoordinate &movement, bool report) const {
	if (movement.X)
		emit(EV_REL, REL_X, movement.X);

	if (movement.Y)
		emit(EV_REL, REL_Y, movement.Y);

	if (movement.Z)
		emit(EV_REL, REL_Z, movement.Z);


	if (report)
		emit(EV_SYN, SYN_REPORT, 0);

}

void uInput::send_wheel_relative(int32_t movement, bool h, bool report) const {
	emit(EV_REL, h ? REL_HWHEEL : REL_WHEEL, movement);

	if (report)
		emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::send_wheel_absolute(int32_t movement, bool report) const {
	emit(EV_ABS, ABS_WHEEL, movement);

	if (report)
		emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::send_pos_absolute(const uInputCoordinate &pos, int32_t mt_slot, bool report) const {
	if (mt_slot == -1) {
		if (pos.X)
			emit(EV_ABS, ABS_X, pos.X);

		if (pos.Y)
			emit(EV_ABS, ABS_Y, pos.Y);

		if (pos.Z)
			emit(EV_ABS, ABS_Z, pos.Z);
	} else {
		emit(EV_ABS, ABS_MT_SLOT, mt_slot);

		if (pos.X)
			emit(EV_ABS, ABS_MT_POSITION_X, pos.X);

		if (pos.Y)
			emit(EV_ABS, ABS_MT_POSITION_Y, pos.Y);

	}

	if (report)
		emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::emulate_touchpad_scroll(int offset, bool report) const {

	int polar = (offset / abs(offset)) * 4;
	int32_t ypos_expected = 1000 + offset;

	emit(EV_ABS, ABS_MT_SLOT, 0);
	emit(EV_ABS, ABS_MT_TRACKING_ID, 2000);
	emit(EV_ABS, ABS_MT_POSITION_X, 1000);
	emit(EV_ABS, ABS_MT_POSITION_Y, 1000);
	emit(EV_ABS, ABS_MT_PRESSURE, 70);
	emit(EV_KEY, BTN_TOUCH, 1);
	emit(EV_ABS, ABS_X, 1000);
	emit(EV_ABS, ABS_Y, 1000);
	emit(EV_ABS, ABS_PRESSURE, 70);
	emit(EV_KEY, BTN_TOOL_FINGER, 1);
	emit(EV_SYN, SYN_REPORT, 0);

	//
	emit(EV_ABS, ABS_MT_POSITION_X, 1000);
	emit(EV_ABS, ABS_MT_POSITION_Y, 1000);
	emit(EV_ABS, ABS_MT_PRESSURE, 70);
	emit(EV_ABS, ABS_MT_SLOT, 1);
	emit(EV_ABS, ABS_MT_TRACKING_ID, 2001);
	emit(EV_ABS, ABS_MT_POSITION_X, 1100);
	emit(EV_ABS, ABS_MT_POSITION_Y, 1000);
	emit(EV_ABS, ABS_MT_PRESSURE, 70);
	emit(EV_ABS, ABS_X, 1000);
	emit(EV_ABS, ABS_Y, 1000);
	emit(EV_ABS, ABS_PRESSURE, 70);
	emit(EV_KEY, BTN_TOOL_FINGER, 0);
	emit(EV_KEY, BTN_TOOL_DOUBLETAP, 1);
	emit(EV_SYN, SYN_REPORT, 0);

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
		send_pos_absolute({1000, ypos}, 0, false);
		send_pos_absolute({1000, ypos}, 1, false);
		send_pos_absolute({1000, ypos}, -1, false);
		emit(EV_SYN, SYN_REPORT, 0);
		usleep(1000*5);
	}

	emit(EV_ABS, ABS_MT_POSITION_X, 1000);
	emit(EV_ABS, ABS_MT_POSITION_Y, ypos);
	emit(EV_ABS, ABS_MT_SLOT, 0);
	emit(EV_ABS, ABS_MT_TRACKING_ID, -1);
	emit(EV_ABS, ABS_X, 1000);
	emit(EV_ABS, ABS_Y, ypos);
	emit(EV_KEY, BTN_TOOL_FINGER, 1);
	emit(EV_KEY, BTN_TOOL_DOUBLETAP, 0);
	emit(EV_SYN, SYN_REPORT, 0);

	emit(EV_ABS, ABS_MT_SLOT, 1);
	emit(EV_ABS, ABS_MT_TRACKING_ID, -1);
	emit(EV_KEY, BTN_TOUCH, 0);
	emit(EV_KEY, BTN_TOOL_FINGER, 0);
	emit(EV_SYN, SYN_REPORT, 0);

//	Emit(EV_KEY, BTN_TOOL_DOUBLETAP, 1);

}


