/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
					http://dev-c.com
				(C) Alexander Blade 2015
*/

#pragma once

#include <windows.h>
#include "..\io\config_io.h"
#include "..\..\inc\xinput\CXBOXController.h"

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include "..\..\inc\main.h"

static const WORD XINPUT_L_STICK_UP = 1;
static const WORD XINPUT_L_STICK_DOWN = 2;
static const WORD XINPUT_L_STICK_LEFT = 3;
static const WORD XINPUT_L_STICK_RIGHT = 4;
static const WORD XINPUT_R_STICK_UP = 5;
static const WORD XINPUT_R_STICK_DOWN = 6;
static const WORD XINPUT_R_STICK_LEFT = 7;
static const WORD XINPUT_R_STICK_RIGHT = 8;
static const WORD XINPUT_L_TRIGGER = 9;
static const WORD XINPUT_R_TRIGGER = 10;

#define ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  11000 
#define ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define ENT_XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

static const ButtonsWithNames ALL_BUTTONS[] =
{
	{ ControllerButtonConfig::CONTROLLER_BTN_DPAD_U, XINPUT_GAMEPAD_DPAD_UP, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_DPAD_D, XINPUT_GAMEPAD_DPAD_DOWN, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_DPAD_L, XINPUT_GAMEPAD_DPAD_LEFT, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_DPAD_R, XINPUT_GAMEPAD_DPAD_RIGHT, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_X, XINPUT_GAMEPAD_X, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_Y, XINPUT_GAMEPAD_Y, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_A, XINPUT_GAMEPAD_A, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_B, XINPUT_GAMEPAD_B, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_SHOULDER_L, XINPUT_GAMEPAD_LEFT_SHOULDER, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_SHOULDER_R, XINPUT_GAMEPAD_RIGHT_SHOULDER, false },

	{ ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_L, XINPUT_L_TRIGGER, true },
	{ ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_R, XINPUT_R_TRIGGER, true },

	{ ControllerButtonConfig::CONTROLLER_LSTICK_CLICK, XINPUT_GAMEPAD_LEFT_THUMB, false },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_CLICK, XINPUT_GAMEPAD_RIGHT_THUMB, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_START, XINPUT_GAMEPAD_START, false },
	{ ControllerButtonConfig::CONTROLLER_BTN_BACK, XINPUT_GAMEPAD_BACK, false },

	{ ControllerButtonConfig::CONTROLLER_LSTICK_L, XINPUT_L_STICK_LEFT, true },
	{ ControllerButtonConfig::CONTROLLER_LSTICK_R, XINPUT_L_STICK_RIGHT, true },
	{ ControllerButtonConfig::CONTROLLER_LSTICK_U, XINPUT_L_STICK_UP, true },
	{ ControllerButtonConfig::CONTROLLER_LSTICK_D, XINPUT_L_STICK_DOWN, true },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_L, XINPUT_R_STICK_LEFT, true },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_R, XINPUT_R_STICK_RIGHT, true },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_U, XINPUT_R_STICK_UP, true },
	{ ControllerButtonConfig::CONTROLLER_RSTICK_D, XINPUT_R_STICK_DOWN, true },

	{ ControllerButtonConfig::CONTROLLER_NOT_BOUND, 0, false }
};

void init_xinput();

void end_xinput();

bool IsControllerButtonDown(std::string function);

bool IsControllerButtonJustUp(std::string btnName);

bool IsAnalogControlPressed(int ourID, XINPUT_STATE state);

bool UpdateXInputControlState();

ButtonsWithNames buttonNameToVal(char * input);