/*
    This file is part of libuInputPlus.
    Copyright (C) 2018 YukiWorkshop

    This program is free software: you can redistribute it and/or modify
    it under the terms of the MIT License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "uInput.hpp"

using namespace uInputPlus;

int main() {

	uInputSetup mysetup(
		{BUS_USB, 0x1234, 0x1234, "呀"},
		{EV_KEY, EV_ABS},
		{BTN_LEFT, BTN_RIGHT, BTN_TOOL_FINGER, BTN_TOOL_QUINTTAP, BTN_TOUCH, BTN_TOOL_DOUBLETAP, BTN_TOOL_TRIPLETAP, BTN_TOOL_QUADTAP},
		{},
		{
			{ABS_X, 6000, 40},
			{ABS_Y, 6000, 40},
			{ABS_PRESSURE, 255},
			{ABS_TOOL_WIDTH, 15},
			{ABS_MT_SLOT, 1},
			{ABS_MT_POSITION_X, 6000, 40},
			{ABS_MT_POSITION_Y, 6000, 40},
			{ABS_MT_TRACKING_ID, 65535},
			{ABS_MT_PRESSURE, 255}
		},
		{INPUT_PROP_POINTER}
	);

	uInput myinput(mysetup);

	sleep(10);

//	myinput.SendKeyPress({KEY_2, KEY_3, KEY_3, KEY_6, KEY_6, KEY_6});
//	myinput.SendKeyPress({{KEY_1, 1000}, {KEY_2, 600}});


	myinput.EmulateSmoothScroll(-4000);
//	int i = 600;
//
//	while (i--) {
////		myinput.RelativeWheel(1);
//		myinput.AbsoluteWheel(1);
//		//myinput.RelativeMove({10, 10});
//		usleep(1000*16);
//	}


}