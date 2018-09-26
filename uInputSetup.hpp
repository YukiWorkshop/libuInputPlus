/*
    This file is part of libuInputPlus.
    Copyright (C) 2018 YukiWorkshop

    This program is free software: you can redistribute it and/or modify
    it under the terms of the MIT License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef LIBUINPUTPLUS_UINPUTSETUP_HPP
#define LIBUINPUTPLUS_UINPUTSETUP_HPP

#include "CommonIncludes.hpp"

namespace uInputPlus {
    extern std::initializer_list<int> DefaultEventTypes;
    extern std::initializer_list<int> DefaultKeys;

    class uInputDeviceInfo {
    public:
	uinput_setup usetup{};

	uint16_t &BusType = usetup.id.bustype;
	uint16_t &Vendor = usetup.id.vendor;
	uint16_t &Product = usetup.id.product;
	uint16_t &Version = usetup.id.version;


	uInputDeviceInfo() = default;
	uInputDeviceInfo(uint16_t bus_type, uint16_t vid, uint16_t pid, const std::string &name);

	void Name(const std::string &__name);
	std::string Name();


	friend void swap(uInputDeviceInfo &first, uInputDeviceInfo &second) {
		using std::swap;

		swap(first.usetup, second.usetup);
	}

	uInputDeviceInfo(uInputDeviceInfo &&other) noexcept : uInputDeviceInfo() {
		swap(*this, other);
	}

	uInputDeviceInfo& operator= (uInputDeviceInfo other) {
		swap(*this, other);
		return *this;
	}

	uInputDeviceInfo(const uInputDeviceInfo &other) {
		memcpy(&usetup, &(other.usetup), sizeof(uinput_setup));
	}
    };

    class uInputSetup {
    public:
	uInputDeviceInfo DeviceInfo;
	std::initializer_list<int> Events;
	std::initializer_list<int> Keys;

	uInputSetup() = default;
	uInputSetup(const uInputDeviceInfo &device_info, const std::initializer_list<int> &events = DefaultEventTypes,
		    const std::initializer_list<int> &keys = DefaultKeys);
    };


}

#endif //LIBUINPUTPLUS_UINPUTSETUP_HPP
