/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

#pragma once

#include <windows.h>

enum eGameVersion : int
{
	VER_1_0_335_2_STEAM,
	VER_1_0_335_2_NOSTEAM,

	VER_1_0_350_1_STEAM,
	VER_1_0_350_2_NOSTEAM,

	VER_UNK = -1
};

#define IMPORT __declspec(dllimport)

IMPORT eGameVersion getGameVersion();

IMPORT void scriptWait(DWORD time);
IMPORT void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)());
IMPORT void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)());
IMPORT void scriptUnregister(HMODULE module);
IMPORT void scriptUnregister(void(*LP_SCRIPT_MAIN)()); // deprecated

typedef void(*KeyboardHandler)(DWORD, WORD, BYTE, BOOL, BOOL, BOOL, BOOL);

IMPORT void keyboardHandlerRegister(KeyboardHandler handler);
IMPORT void keyboardHandlerUnregister(KeyboardHandler handler);

IMPORT void nativeInit(UINT64 hash);
IMPORT void nativePush64(UINT64 val);
IMPORT PUINT64 nativeCall();

static void WAIT(DWORD time) { scriptWait(time); }
static void TERMINATE() { WAIT(MAXDWORD); }

