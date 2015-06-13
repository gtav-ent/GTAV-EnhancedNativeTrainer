/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
					http://dev-c.com
				(C) Alexander Blade 2015
*/

#pragma once

#include <windows.h>
#include "..\io\config_io.h"

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include "..\..\inc\main.h"

struct ButtonsWithNames
{
	std::string name;
	int buttonCode;
};

static const ButtonsWithNames ALL_BUTTONS[] =
{
	{ ControllerButtonConfig::CONTROLLER_BTN_DPAD_U, 27 },
	{ ControllerButtonConfig::CONTROLLER_BTN_DPAD_D, 19 },
	{ ControllerButtonConfig::CONTROLLER_BTN_DPAD_L, 15 },
	{ ControllerButtonConfig::CONTROLLER_BTN_DPAD_R, 74 },
	{ ControllerButtonConfig::CONTROLLER_BTN_X, 22 },
	{ ControllerButtonConfig::CONTROLLER_BTN_Y, 23 },
	{ ControllerButtonConfig::CONTROLLER_BTN_A, 18 },
	{ ControllerButtonConfig::CONTROLLER_BTN_B, 45 },
	{ ControllerButtonConfig::CONTROLLER_BTN_SHOULDER_L, 37 },
	{ ControllerButtonConfig::CONTROLLER_BTN_SHOULDER_R, 44 },
	{ ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_L, 10 },
	{ ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_R, 11 },
	{ ControllerButtonConfig::CONTROLLER_LSTICK_CLICK, 28 },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_CLICK, 29 },
	{ ControllerButtonConfig::CONTROLLER_BTN_MENU, 0 },
	{ ControllerButtonConfig::CONTROLLER_BTN_GUIDE, 99 },
	{ ControllerButtonConfig::CONTROLLER_LSTICK_L, 34 },
	{ ControllerButtonConfig::CONTROLLER_LSTICK_R, 9 },
	{ ControllerButtonConfig::CONTROLLER_LSTICK_U, 32 },
	{ ControllerButtonConfig::CONTROLLER_LSTICK_D, 8 },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_L, 5 },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_R, 1 },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_U, 3 },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_D, 2 }
};

bool IsControllerButtonDown(std::string function);

bool IsControllerButtonJustUp(std::string btnName);

int buttonNameToVal(char * input);