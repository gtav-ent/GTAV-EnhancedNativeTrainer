/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#import <msxml6.dll> //read the GitHub project readme regarding what you need to make this work

/**A class to hold the current key bindings.*/
class KeyInputConfig
{
public:
	//these are the defaults which may be overridden by the XML config
	int key_up = VK_NUMPAD8;
	int key_down = VK_NUMPAD2;
	int key_left = VK_NUMPAD4;
	int key_right = VK_NUMPAD6;
	int key_confirm = VK_NUMPAD5;
	int key_activate = VK_F4;
	int key_back = VK_NUMPAD0;
	int key_boost = VK_NUMPAD9;
	int key_airbrake = VK_NUMPAD3;
	int key_rockets = VK_ADD;

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

