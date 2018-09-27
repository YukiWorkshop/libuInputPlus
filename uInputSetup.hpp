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
    extern const std::set<int> DefaultEventTypes;
    extern const std::set<int> DefaultKeys;
    extern const std::set<int> DefaultRels;

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
	std::set<int> Events;
	std::set<int> Keys;
	std::set<int> Rels;

	uInputSetup() = default;
	uInputSetup(const uInputDeviceInfo &device_info, const std::set<int> &events = DefaultEventTypes,
		    const std::set<int> &keys = DefaultKeys, const std::set<int> &rels = DefaultRels);
    };


}

#endif //LIBUINPUTPLUS_UINPUTSETUP_HPP
