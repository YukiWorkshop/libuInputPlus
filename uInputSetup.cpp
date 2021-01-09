/*
    This file is part of libuInputPlus.
    Copyright (C) 2018-2021 Reimu NotMoe <reimu@sudomaker.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the MIT License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "uInputSetup.hpp"

using namespace uInputPlus;


void uInputDeviceInfo::set_name(const std::string &__name) {
	auto len = __name.size();
	if (len > (UINPUT_MAX_NAME_SIZE-1))
		len = (UINPUT_MAX_NAME_SIZE-1);

	memcpy(usetup.name, __name.c_str(), len);
	usetup.name[len] = 0;
}

std::string uInputDeviceInfo::name() {
	return std::string(usetup.name);
}

uInputDeviceInfo::uInputDeviceInfo(const std::string &name, uint16_t bus_type, uint16_t vid, uint16_t pid, uint16_t version) {
	BusType = bus_type;
	Vendor = vid;
	Product = pid;
	Version = version;
	set_name(name);
}

uInputSetup::uInputSetup(const uInputDeviceInfo &device_info, const std::set<int> &events,
			 const std::set<int> &keys, const std::set<int> &rels,
			 const std::vector<uInputAbsSetup> &abs_setup, const std::set<int> &props) {
	DeviceInfo = device_info;
	Events = events;
	Keys = keys;
	Rels = rels;
	AbsSetup = abs_setup;
	Props = props;

}
