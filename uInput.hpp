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

    };
}

#endif //LIBUINPUTPLUS_UINPUT_HPP