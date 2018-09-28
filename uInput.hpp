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

#ifndef LIBUINPUTPLUS_UINPUT_HPP
#define LIBUINPUTPLUS_UINPUT_HPP

namespace uInputPlus {
    class uInputCoordinate {
    public:
	int32_t X = 0, Y = 0, Z = 0;

	uInputCoordinate() = default;
	uInputCoordinate(int32_t x, int32_t y) {
		X = x; Y = y;
	}
	uInputCoordinate(int32_t x, int32_t y, int32_t z) {
		X = x; Y = y; Z = z;
	}
    };

    class uInput {
    private:

    public:
	uInput() = default;
	uInput(const uInputSetup &setup);
	~uInput();

	int FD = -1;

	void Init(const uInputSetup &setup);
	void Destroy();

	void Emit(uint16_t type, uint16_t code, int32_t val);

	void SendKey(uint16_t key_code, uint32_t value, bool report=true);
	void SendKeyPress(const std::initializer_list<uint16_t> &keycodes, bool report=true);
	void SendKeyPress(std::vector<std::pair<int, int>> keys, bool report=true);

	void RelativeMove(const uInputCoordinate &movement, bool report=true);
	void RelativeWheel(int32_t movement, bool h=false, bool report=true);

	void AbsolutePosition(const uInputCoordinate &pos, int32_t mt_slot=-1, bool report=true);
	void AbsoluteWheel(int32_t movement, bool report=true);

	void EmulateSmoothScroll(int offset, bool report=true);

    };
}

#endif //LIBUINPUTPLUS_UINPUT_HPP