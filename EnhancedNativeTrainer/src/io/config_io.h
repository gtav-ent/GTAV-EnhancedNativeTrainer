/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#import <msxml6.dll> //read the GitHub project readme regarding what you need to make this work

#include <map>
#include <vector>
#include "keyboard.h"

class KeyConfig
{
public:

	static const std::string KEY_TOGGLE_MAIN_MENU;
	static const std::string KEY_TOGGLE_AIRBRAKE;

	static const std::string KEY_MENU_UP;
	static const std::string KEY_MENU_DOWN;
	static const std::string KEY_MENU_LEFT;
	static const std::string KEY_MENU_RIGHT;
	static const std::string KEY_MENU_SELECT;
	static const std::string KEY_MENU_BACK;

	static const std::string KEY_VEH_BOOST;
	static const std::string KEY_VEH_STOP;
	static const std::string KEY_VEH_ROCKETS;

	static const std::string KEY_AIRBRAKE_UP;
	static const std::string KEY_AIRBRAKE_DOWN;
	static const std::string KEY_AIRBRAKE_ROTATE_LEFT;
	static const std::string KEY_AIRBRAKE_ROTATE_RIGHT;
	static const std::string KEY_AIRBRAKE_FORWARD;
	static const std::string KEY_AIRBRAKE_BACK;
	static const std::string KEY_AIRBRAKE_SPEED;
	static const std::string KEY_AIRBRAKE_FREEZE_TIME;
	static const std::string KEY_AIRBRAKE_HELP;

	static const std::string KEY_HOT_AIRBRAKE_THROUGH_DOOR;

	inline KeyConfig(int code)
	{
		this->keyCode = code;
	};

	int keyCode;
	bool modCtrl = false;
	bool modAlt = false;
	bool modShift = false;
};

struct ButtonsWithNames
{
	std::string name;
	WORD buttonCode;
	bool isAnalog;
};

class ControllerButtonConfig
{
public:
	static const std::string CONTROLLER_BTN_X;
	static const std::string CONTROLLER_BTN_Y;
	static const std::string CONTROLLER_BTN_A;
	static const std::string CONTROLLER_BTN_B;
	static const std::string CONTROLLER_BTN_DPAD_L;
	static const std::string CONTROLLER_BTN_DPAD_R;
	static const std::string CONTROLLER_BTN_DPAD_U;
	static const std::string CONTROLLER_BTN_DPAD_D;
	static const std::string CONTROLLER_BTN_SHOULDER_L;
	static const std::string CONTROLLER_BTN_SHOULDER_R;
	static const std::string CONTROLLER_BTN_TRIGGER_L;
	static const std::string CONTROLLER_BTN_TRIGGER_R;
	static const std::string CONTROLLER_BTN_BACK;
	static const std::string CONTROLLER_BTN_START;
	static const std::string CONTROLLER_LSTICK_L;
	static const std::string CONTROLLER_LSTICK_R;
	static const std::string CONTROLLER_LSTICK_U;
	static const std::string CONTROLLER_LSTICK_D;
	static const std::string CONTROLLER_RSTICK_L;
	static const std::string CONTROLLER_RSTICK_R;
	static const std::string CONTROLLER_RSTICK_U;
	static const std::string CONTROLLER_RSTICK_D;
	static const std::string CONTROLLER_LSTICK_CLICK;
	static const std::string CONTROLLER_RSTICK_CLICK;
	static const std::string CONTROLLER_NOT_BOUND;

	inline ControllerButtonConfig()
	{
	};

	void add_button(char* name);

	void add_button(std::string name);

	std::vector<ButtonsWithNames> buttonCodes;
};

/**A class to hold the current key bindings.*/
class KeyInputConfig
{
public:
	KeyInputConfig();

	virtual ~KeyInputConfig();

	std::map<std::string, KeyConfig*> keyConfigs;

	std::map<std::string, ControllerButtonConfig*> controllerConfigs;

	KeyConfig* get_key(std::string function);

	ControllerButtonConfig* get_controller_button(std::string function);

	/**Change the key binding using a function string and key string.*/
	void set_key(char* function, char* keyName, bool modCtrl = false, bool modAlt = false, bool modShift = false);

	/**Change the key binding using a function string and key string.*/
	void set_control(char* function, ControllerButtonConfig* config);
};

/**A class to hold all the user settings.*/
class TrainerConfig
{
public:
	TrainerConfig();
	KeyInputConfig* get_key_config() { return keyConfig;  }

private:
	KeyInputConfig* keyConfig;
};

/**The current user config.*/
extern TrainerConfig* config;

/**Read the user config in from an XML file.*/
void read_config_file();

/**Get the current config object.*/
inline TrainerConfig* get_config() { return config;  }

