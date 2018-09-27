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
	uInput myinput({{BUS_USB, 0x1234, 0x1234, "呀"}});

	sleep(5);

//	myinput.SendKeyPress({KEY_2, KEY_3, KEY_3, KEY_6, KEY_6, KEY_6});
//	myinput.SendKeyPress({{KEY_1, 1000}, {KEY_2, 600}});

	int i = 600;

	while (i--) {
//		myinput.RelativeWheel(1);
		myinput.AbsoluteWheel(1);
		//myinput.RelativeMove({10, 10});
		usleep(1000*16);
	}


}