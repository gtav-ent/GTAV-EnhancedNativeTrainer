#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#import <msxml6.dll>

class KeyInputConfig
{
public:
	int key_up = VK_NUMPAD8;
	int key_down = VK_NUMPAD2;
	int key_left = VK_NUMPAD4;
	int key_right = VK_NUMPAD6;
	int key_confirm = VK_NUMPAD5;
	int key_activate = VK_F4;
	int key_back = VK_NUMPAD0;

	void set_key(char* function, char* keyName);
};

class TrainerConfig
{
public:
	TrainerConfig();
	KeyInputConfig* get_key_config() { return keyConfig;  }
private:
	KeyInputConfig* keyConfig;
};

extern TrainerConfig* config;

void read_config_file();

inline TrainerConfig* get_config() { return config;  }

