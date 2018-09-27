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

void uInput::Emit(uint16_t type, uint16_t code, int32_t val) {
	input_event ie{};

	ie.type = type;
	ie.code = code;
	ie.value = val;

	write(FD, &ie, sizeof(ie));
}

void uInput::SendKey(uint16_t key_code, uint32_t value, bool report) {
	Emit(EV_KEY, key_code, value);

	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::SendKeyPress(const std::initializer_list<uint16_t> &keycodes, bool report) {
	for (auto it : keycodes) {
		SendKey(it, 1, report);
		SendKey(it, 0, report);
	}
}

void uInput::SendKeyPress(std::vector<std::pair<int, int>> keys, bool report) {

}

void uInput::RelativeMove(const uInputRelativeMovement &movement, bool report) {
	if (movement.X)
		Emit(EV_REL, REL_X, movement.X);

	if (movement.Y)
		Emit(EV_REL, REL_Y, movement.Y);

	if (movement.Z)
		Emit(EV_REL, REL_Z, movement.Z);


	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);

}

void uInput::RelativeWheel(int32_t movement, bool h, bool report) {
	Emit(EV_REL, h ? REL_HWHEEL : REL_WHEEL, movement);

	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);
}

void uInput::AbsoluteWheel(int32_t movement, bool report) {
	Emit(EV_ABS, ABS_WHEEL, movement);

	if (report)
		Emit(EV_SYN, SYN_REPORT, 0);
}
