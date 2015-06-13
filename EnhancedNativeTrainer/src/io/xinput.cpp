/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
					http://dev-c.com
				(C) Alexander Blade 2015
*/

#include "..\io\xinput.h"
//#include "..\features\script.h"
#include "..\debug\debuglog.h"

#include <sstream>

bool IsControllerButtonDown(std::string btnName)
{
	ControllerButtonConfig* buttonConf = get_config()->get_key_config()->get_controller_button(btnName);
	if (buttonConf == NULL || buttonConf->buttonCodes.size() == 0)
	{
		return false;
	}
	for each (int code in buttonConf->buttonCodes)
	{
		if (!CONTROLS::IS_CONTROL_PRESSED(2, code) && !CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, code))
		{
			return false;
		}
	}
	return true;
}

bool IsControllerButtonJustUp(std::string btnName)
{
	ControllerButtonConfig* buttonConf = get_config()->get_key_config()->get_controller_button(btnName);
	if (buttonConf == NULL || buttonConf->buttonCodes.size() == 0)
	{
		return false;
	}
	bool anyJustPressed = false;
	for each (int code in buttonConf->buttonCodes)
	{
		if (!CONTROLS::IS_CONTROL_PRESSED(2, code) && !CONTROLS::IS_CONTROL_JUST_RELEASED(2, code) 
			&& !CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, code)
			&& CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, code))
		{
			return false;
		}
		else if (!anyJustPressed)
		{
			anyJustPressed = CONTROLS::IS_CONTROL_JUST_RELEASED(2, code) || CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, code);
		}
	}
	return anyJustPressed;
}

int buttonNameToVal(char * input)
{
	std::string inputS = std::string(input);
	for each (ButtonsWithNames button in ALL_BUTTONS)
	{
		if (button.name.compare(inputS) == 0)
		{
			return button.buttonCode;
		}
	}
	return -1;
}