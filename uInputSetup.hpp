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
    extern const std::set<int> DefaultAbss;

    class uInputAbsSetup {
    public:
	uinput_abs_setup setup{};

	uint16_t &Code = setup.code;

	int32_t &Value = setup.absinfo.value;
	int32_t &Min = setup.absinfo.minimum;
	int32_t &Max = setup.absinfo.maximum;
	int32_t &Resoltion = setup.absinfo.resolution;
	int32_t &Fuzz = setup.absinfo.fuzz;
	int32_t &Flat = setup.absinfo.flat;

	uInputAbsSetup() = default;
	uInputAbsSetup(uint16_t code, int32_t max) {
		Code = code;
		Max = max;
	}
	uInputAbsSetup(uint16_t code, int32_t max, int32_t resolution) {
		Code = code;
		Max = max;
		Resoltion = resolution;
	}
	uInputAbsSetup(uint16_t code, int32_t min, int32_t max, int32_t value, int32_t resolution, int32_t fuzz, int32_t flat) {
		Code = code;
		Min = min;
		Max = max;
		Value = value;
		Resoltion = resolution;
		Fuzz = fuzz;
		Flat = flat;
	}

	friend void swap(uInputAbsSetup &first, uInputAbsSetup &second) {
		using std::swap;

		swap(first.setup, second.setup);
	}

	uInputAbsSetup(uInputAbsSetup &&other) noexcept : uInputAbsSetup() {
		swap(*this, other);
	}

	uInputAbsSetup& operator= (uInputAbsSetup other) {
		swap(*this, other);
		return *this;
	}

	uInputAbsSetup(const uInputAbsSetup &other) {
		memcpy(&setup, &(other.setup), sizeof(uinput_abs_setup));
	}

    };

    class uInputDeviceInfo {
    public:
	uinput_setup usetup{};

	uint16_t &BusType = usetup.id.bustype;
	uint16_t &Vendor = usetup.id.vendor;
	uint16_t &Product = usetup.id.product;
	uint16_t &Version = usetup.id.version;


	uInputDeviceInfo() = default;
	uInputDeviceInfo(const std::string &name, uint16_t bus_type = BUS_USB, uint16_t vid = 0x2333, uint16_t pid = 0x6666, uint16_t version = 23333);

	void Name(const std::string &__name);
	std::string Name();


	friend void swap(uInputDeviceInfo &first, uInputDeviceInfo &second) {
		using std::swap;

		swap(first.usetup, second.usetup);
	}

	uInputDeviceInfo(uInputDeviceInfo &&other) noexcept : uInputDeviceInfo() {
		swap(*this, other);
	}

	uInputDeviceInfo& operator= (uInputDeviceInfo other) noexcept {
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
	std::vector<uInputAbsSetup> AbsSetup;
	std::set<int> Props;


	uInputSetup() = default;
	uInputSetup(const uInputDeviceInfo &device_info,
		    const std::set<int> &events = DefaultEventTypes,
		    const std::set<int> &keys = DefaultKeys,
		    const std::set<int> &rels = DefaultRels,
		    const std::vector<uInputAbsSetup> &abs_setup = {},
		    const std::set<int> &props = {});
    };


}

#endif //LIBUINPUTPLUS_UINPUTSETUP_HPP
