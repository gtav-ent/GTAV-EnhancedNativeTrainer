/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "inc\main.h"
#include "script.h"
#include "keyboard.h"
#include "config_io.h"
#include "debuglog.h"

#include <thread>

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		write_text_to_log_file("Attach");
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
		write_text_to_log_file("Done attaching");
		break;
	case DLL_PROCESS_DETACH:
		write_text_to_log_file("Detach");
		scriptUnregister(ScriptMain);
		write_text_to_log_file("Unregistered");
		keyboardHandlerUnregister(OnKeyboardMessage);
		write_text_to_log_file("Unregistered KB");
		ScriptTidyUp();
		write_text_to_log_file("Tidied up");
		break;
	}
	return TRUE;
}
