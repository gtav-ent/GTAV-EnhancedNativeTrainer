/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
					http://dev-c.com
				(C) Alexander Blade 2015
*/

#include "..\io\keyboard.h"
#include "..\features\script.h"
#include "..\debug\debuglog.h"

#include <sstream>

const int KEYS_SIZE = 255;

struct {
	DWORD time;
	BOOL isWithAlt;
	BOOL wasDownBefore;
	BOOL isUpNow;
} keyStates[KEYS_SIZE];

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (key < KEYS_SIZE)
	{
		keyStates[key].time = GetTickCount();
		keyStates[key].isWithAlt = isWithAlt;
		keyStates[key].wasDownBefore = wasDownBefore;
		keyStates[key].isUpNow = isUpNow;
	}
}

const int NOW_PERIOD = 100, MAX_DOWN = 5000; // ms

bool IsKeyDown(std::string keyName)
{
	KeyConfig* key = get_config()->get_key_config()->get_key(keyName);
	if (key == NULL)
	{
		return false;
	}
	bool result = IsKeyDown(key->keyCode);
	if (result && key->modAlt)
	{
		result = (GetKeyState(VK_MENU) & 0x8000;
	}
	if (result && key->modShift)
	{
		result = (GetKeyState(VK_SHIFT) & 0x8000);
	}
	if (result && key->modCtrl)
	{
		result = (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000);
	}
	return result;
}

bool IsKeyDown(DWORD key)
{
	return (key < KEYS_SIZE) ? ((GetTickCount() < keyStates[key].time + MAX_DOWN) && !keyStates[key].isUpNow) : false;
}

bool IsKeyJustUp(std::string keyName, bool exclusive)
{
	KeyConfig* key = get_config()->get_key_config()->get_key(keyName);

	if (key == NULL)
	{
		return false;
	}
	bool result = IsKeyJustUp(key->keyCode, exclusive);
	if (result && key->modAlt)
	{
		result = (GetKeyState(VK_MENU) & 0x8000;
	}
	if (result && key->modShift)
	{
		result = (GetKeyState(VK_SHIFT) & 0x8000);
	}
	if (result && key->modCtrl)
	{
		result = (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000);
	}
	return result;
}

bool IsKeyJustUp(DWORD key, bool exclusive)
{
	bool b = (key < KEYS_SIZE) ? (GetTickCount() < keyStates[key].time + NOW_PERIOD && keyStates[key].isUpNow) : false;
	if (b && exclusive)
		ResetKeyState(key);
	return b;
}

void ResetKeyState(DWORD key)
{
	if (key < KEYS_SIZE)
		memset(&keyStates[key], 0, sizeof(keyStates[0]));
}

int keyNameToVal(char * input)
{
	std::ostringstream ss;
	ss << "Searching for " << input;
	write_text_to_log_file(ss.str());

	for (int i = 0; i < (sizeof ALL_KEYS / sizeof ALL_KEYS[0]); i++)
	{
		if (strcmp(input, ALL_KEYS[i].name) == 0)
		{
			ss.str(""); ss.clear();
			ss << "Found match of " << ALL_KEYS[i].name << " with code " << ALL_KEYS[i].keyCode;
			write_text_to_log_file(ss.str());

			return ALL_KEYS[i].keyCode;
		}
	}
	return -1;
}