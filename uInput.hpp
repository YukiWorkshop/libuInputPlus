/*
    This file is part of libuInputPlus.
    Copyright (C) 2018-2021 Reimu NotMoe <reimu@sudomaker.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the MIT License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "CommonIncludes.hpp"
#include "uInputSetup.hpp"

namespace uInputPlus {

	struct uInputRawData {
		uint16_t type;
		uint16_t code;
		int32_t value;
	} __attribute__((packed));

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
		int fd_ = -1;

		std::function<void(uint16_t type, uint16_t code, int32_t val)> custom_callback_;

		void __init(const uInputSetup &setup);
		void __deinit();
	public:
		uInput() = default;
		uInput(const uInputSetup &setup);
		uInput(const uInputSetup &setup, std::function<int(uint16_t type, uint16_t code, int32_t val)> custom_callback);
		~uInput();

		int fd() const noexcept {
			return fd_;
		}

		void emit(uint16_t type, uint16_t code, int32_t val) const;

		void send_key(uint16_t key_code, uint32_t value, bool report = true) const;
		void send_keypress(const std::initializer_list<uint16_t> &keycodes, bool report = true) const;
		void send_keypress(std::vector<std::pair<int, int>> keys, bool report = true) const;

		void send_pos_relative(const uInputCoordinate &movement, bool report = true) const;
		void send_pos_absolute(const uInputCoordinate &pos, int32_t mt_slot = -1, bool report = true) const;

		void send_wheel_absolute(int32_t movement, bool report = true) const;
		void send_wheel_relative(int32_t movement, bool h = false, bool report = true) const;

		void emulate_touchpad_scroll(int offset, bool report = true) const;

	};
}