/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#import <msxml6.dll> //read the GitHub project readme regarding what you need to make this work

#include "keyboard.h"

/**A class to hold the current key bindings.*/
class KeyInputConfig
{
public:
	//these are the defaults which may be overridden by the XML config

	int key_toggle_main_menu = VK_F4;
	int key_toggle_airbrake = VK_F6;

	int key_menu_up = VK_NUMPAD8;
	int key_menu_down = VK_NUMPAD2;
	int key_menu_left = VK_NUMPAD4;
	int key_menu_right = VK_NUMPAD6;
	int key_menu_confirm = VK_NUMPAD5;
	int key_menu_back = VK_NUMPAD0;

	int key_veh_boost = VK_NUMPAD9;
	int key_veh_stop = VK_NUMPAD3;
	int key_veh_rockets = VK_ADD;

	int key_airbrake_up = VK_KEY_Q;
	int key_airbrake_down = VK_KEY_Z;
	int key_airbrake_forward = VK_KEY_W;
	int key_airbrake_back = VK_KEY_S;
	int key_airbrake_rotate_left = VK_KEY_A;
	int key_airbrake_rotate_right = VK_KEY_D;
	int key_airbrake_speed = VK_SHIFT;

	//int key_hotkey_wanted = VK_SHIFT;
	int key_hotkey_throughdoor = VK_DIVIDE;

	/**Change the key binding using a function string and key string.*/
	void set_key(char* function, char* keyName);
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

