/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
		http://dev-c.com
		(C) Alexander Blade 2015
		*/

#include "..\io\xinput.h"
#include "..\features\script.h"
#include "..\debug\debuglog.h"

#include <sstream>

CXBOXController* controller;

XINPUT_STATE prevState;

void init_xinput()
{
	controller = new CXBOXController(1);
}

void end_xinput()
{
	delete controller;
}

bool IsControllerButtonDown(std::string btnName)
{
	ControllerButtonConfig* buttonConf = get_config()->get_key_config()->get_controller_button(btnName);
	if (buttonConf == NULL || buttonConf->buttonCodes.size() == 0)
	{
		return false;
	}

	if (controller == NULL || !controller->IsConnected())
	{
		return false;
	}

	XINPUT_STATE state = controller->GetState();
	bool result = true;
	for each (ButtonsWithNames btn in buttonConf->buttonCodes)
	{
		WORD code = btn.buttonCode;

		if (code == 0 )
		{
			result = false;
			break;
		}

		bool pressedNow;
		if (btn.isAnalog)
		{
			pressedNow = IsAnalogControlPressed(code, state);
		}
		else
		{
			pressedNow = (state.Gamepad.wButtons & code);
		}

		if (!pressedNow)
		{
			result = false;
			break;
		}
	}

	if (state.dwPacketNumber > prevState.dwPacketNumber)
	{
		prevState = state;
	}
	return result;
}

bool IsControllerButtonJustUp(std::string btnName)
{
	ControllerButtonConfig* buttonConf = get_config()->get_key_config()->get_controller_button(btnName);
	if (buttonConf == NULL || buttonConf->buttonCodes.size() == 0)
	{
		std::ostringstream ss;
		ss << "Warning: no controller button for function " << btnName;
		set_status_text_centre_screen(ss.str());
		return false;
	}

	if (controller == NULL || !controller->IsConnected())
	{
		return false;
	}

	XINPUT_STATE state = controller->GetState();
	bool result = true;
	bool anyJustReleased = false;
	bool anyPressed = false;

	for each (ButtonsWithNames btn in buttonConf->buttonCodes)
	{
		WORD code = btn.buttonCode;
		if (code == 0 )
		{
			std::ostringstream ss;
			ss << "Warning: no key for function " << btnName;
			set_status_text_centre_screen(ss.str());

			result = false;
			break;
		}

		bool pressedNow;
		bool pressedBefore;
		
		if (btn.isAnalog)
		{
			pressedNow = IsAnalogControlPressed(code, state);
			pressedBefore = IsAnalogControlPressed(code, prevState);
		}
		else
		{
			pressedNow = (state.Gamepad.wButtons & code);
			pressedBefore = (prevState.Gamepad.wButtons & code);
		}

		if (!anyPressed && pressedNow)
		{
			anyPressed = true;
		}

		if (!pressedNow && pressedBefore)
		{
			anyJustReleased = true;
		}
	}

	return result && anyJustReleased && (buttonConf->buttonCodes.size() == 1 || anyPressed);
}

ButtonsWithNames buttonNameToVal(char * input)
{
	std::string inputS = std::string(input);
	for each (ButtonsWithNames button in ALL_BUTTONS)
	{
		if (button.name.compare(inputS) == 0)
		{
			return button;
		}
	}
	return ButtonsWithNames{ 0, 0, 0 };
}

void setAllXInputButtonsToEnabled(bool enabled)
{
	if (enabled)
	{
		PLAYER::SET_PLAYER_CONTROL(0, 1, 0);
	}
	else
	{
		PLAYER::SET_PLAYER_CONTROL(0, 0, 256);
	}
}

bool IsAnalogControlPressed(int ourID, XINPUT_STATE state)
{
	if (ourID == XINPUT_L_TRIGGER)
	{
		return (state.Gamepad.bLeftTrigger && state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
	else if (ourID == XINPUT_R_TRIGGER)
	{
		return (state.Gamepad.bRightTrigger && state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
	else if (ourID == XINPUT_L_STICK_DOWN)
	{
		return (state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
	}
	else if (ourID == XINPUT_L_STICK_UP)
	{
		return (state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else if (ourID == XINPUT_L_STICK_LEFT)
	{
		return (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else if (ourID == XINPUT_L_STICK_RIGHT)
	{
		return (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else if (ourID == XINPUT_R_STICK_DOWN)
	{
		return (state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else if (ourID == XINPUT_R_STICK_UP)
	{
		return (state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else if (ourID == XINPUT_R_STICK_LEFT)
	{
		return (state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else if (ourID == XINPUT_R_STICK_RIGHT)
	{
		return (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
}