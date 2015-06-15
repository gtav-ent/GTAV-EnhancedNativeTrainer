/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "io.h"
#include "config_io.h"
#include <string>
#include <sstream>

DWORD trainerResetTime = 0;

bool gameInputDisabledByUs = false;

bool trainer_switch_pressed()
{
	bool result = IsKeyJustUp(KeyConfig::KEY_TOGGLE_MAIN_MENU) || IsControllerButtonJustUp(KeyConfig::KEY_TOGGLE_MAIN_MENU);
	if (result)
	{
		//avoid repeat of key press
		DWORD maxTickCount = GetTickCount() + 200;
		do
		{
			UpdateXInputControlState();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
	}
	return result;
}

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r)
{
	KeyInputConfig *keyConf = config->get_key_config();

	if (a) *a = IsKeyJustUp(KeyConfig::KEY_MENU_SELECT) || IsControllerButtonJustUp(KeyConfig::KEY_MENU_SELECT);
	if (b) *b = IsKeyJustUp(KeyConfig::KEY_MENU_BACK) || IsControllerButtonJustUp(KeyConfig::KEY_MENU_BACK);

	if (up) *up = IsKeyDown(KeyConfig::KEY_MENU_UP) || IsControllerButtonDown(KeyConfig::KEY_MENU_UP);
	if (down) *down = IsKeyDown(KeyConfig::KEY_MENU_DOWN) || IsControllerButtonDown(KeyConfig::KEY_MENU_DOWN);
	if (r) *r = IsKeyDown(KeyConfig::KEY_MENU_RIGHT) || IsControllerButtonDown(KeyConfig::KEY_MENU_RIGHT);
	if (l) *l = IsKeyDown(KeyConfig::KEY_MENU_LEFT) || IsControllerButtonDown(KeyConfig::KEY_MENU_LEFT);
}

bool get_key_pressed(int nVirtKey)
{
	//return (GetKeyState(nVirtKey) & 0x8000) != 0;
	return (GetAsyncKeyState(nVirtKey) & 0x8000) != 0;
}

bool airbrake_switch_pressed()
{
	bool result = IsKeyJustUp(KeyConfig::KEY_TOGGLE_AIRBRAKE) || IsControllerButtonJustUp(KeyConfig::KEY_TOGGLE_AIRBRAKE);
	if (result)
	{
		//avoid repeat of key press
		DWORD maxTickCount = GetTickCount() + 200;
		do
		{
			UpdateXInputControlState();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
	}
	return result;
}

void reset_trainer_switch()
{
	trainerResetTime = GetTickCount();
}


void setGameInputToEnabled(bool enabled, bool force)
{
	if (enabled && (gameInputDisabledByUs || force))
	{
		PLAYER::SET_PLAYER_CONTROL(0, 1, 0);
		gameInputDisabledByUs = false;
	}
	else if (!enabled)
	{
		PLAYER::SET_PLAYER_CONTROL(0, 0, 256);
		gameInputDisabledByUs = true;
	}
}