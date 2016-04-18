/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/
#include "time.h"
#include "hotkeys.h"
#include "propplacement.h"

#include <iomanip>

#include "..\ui_support\menu_functions.h"

const std::vector<std::string> TIME_SPEED_CAPTIONS{ "Minimum", "0.1x", "0.5x", "0.75x", "1x (Normal)" };
const std::vector<float> TIME_SPEED_VALUES{ 0.0f, 0.1f, 0.5f, 0.75f, 1.0f };
const int DEFAULT_TIME_SPEED = 4;

const int TIME_TO_SLOW_AIM = 2000;

int timeSpeedIndexWhileAiming = DEFAULT_TIME_SPEED;
int timeSpeedIndex = DEFAULT_TIME_SPEED;

bool featureTimePaused = false;
bool featureTimePausedUpdated = false;
bool featureTimeSynced = false;

int activeLineIndexTime = 0;

int timeSinceAimingBegan = 0;

bool weHaveChangedTimeScale;

float quadratic_time_transition(float start, float end, float progress)
{
	//The quadratic stuff
	float t = 1 - progress;
	t = 1 - (t * t);

	float difference = end - start;

	return (start + (difference * t));
}

bool onconfirm_time_menu(MenuItem<int> choice)
{
	switch (activeLineIndexTime)
	{
		// hour forward/backward
	case 0:
		movetime_hour_forward();
		break;
	case 1:
		movetime_hour_backward();
		break;
	case 2:
		movetime_fivemin_forward();
		break;
	case 3:
		movetime_fivemin_backward();
		break;
	case 4:
		movetime_day_forward();
		break;
	case 5:
		movetime_day_backward();
		break;
	case -1:
		if (featureTimePaused)
		{
			set_status_text("Time now paused");
		}
	case -2:
		if (featureTimeSynced)
		{
			set_status_text("Time synced with system");
		}
	}
	return false;
}

void onchange_game_speed_callback(int value, SelectFromListMenuItem* source)
{
	timeSpeedIndex = value;
	std::stringstream ss;
	ss << "Game speed: " << TIME_SPEED_CAPTIONS.at(value);
	set_status_text(ss.str());
}

void onchange_aiming_speed_callback(int value, SelectFromListMenuItem* source)
{
	timeSpeedIndexWhileAiming = value;
	std::stringstream ss;
	ss << "Aiming speed: " << TIME_SPEED_CAPTIONS.at(value);
	set_status_text(ss.str());
}

void process_time_menu()
{
	const int lineCount = 5;

	std::string caption = "Time Options";

	std::vector<MenuItem<int>*> menuItems;

	int index = 0;

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "1 Hour Forward";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "1 Hour Backward";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "5 Mins Forward";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "5 Mins Backward";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Day Forward";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Day Backward";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "Clock Paused";
	togItem->value = -1;
	togItem->toggleValue = &featureTimePaused;
	togItem->toggleValueUpdated = &featureTimePausedUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Sync With System";
	togItem->value = -2;
	togItem->toggleValue = &featureTimeSynced;
	togItem->toggleValueUpdated = NULL;
	menuItems.push_back(togItem);

	SelectFromListMenuItem *listItem = new SelectFromListMenuItem(TIME_SPEED_CAPTIONS, onchange_game_speed_callback);
	listItem->wrap = false;
	listItem->caption = "Game Speed";
	if (timeSpeedIndex == -1)
	{
		listItem->value = TIME_SPEED_VALUES.size() - 1;
	}
	else
	{
		listItem->value = timeSpeedIndex;
	}
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(TIME_SPEED_CAPTIONS, onchange_aiming_speed_callback);
	listItem->wrap = false;
	listItem->caption = "Game Speed While Aiming";
	listItem->value = timeSpeedIndexWhileAiming;
	menuItems.push_back(listItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexTime, caption, onconfirm_time_menu, NULL, NULL);
}

void reset_time_globals()
{
	featureTimePaused =
		featureTimeSynced = false;

	featureTimePausedUpdated = true;

	timeSpeedIndexWhileAiming = DEFAULT_TIME_SPEED;
	timeSpeedIndex = DEFAULT_TIME_SPEED;
}

void add_time_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featureTimePaused", &featureTimePaused, &featureTimePausedUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureTimeSynced", &featureTimeSynced });
}



void movetime_day_forward()
{
	/*
	bool timeWasPaused = featureTimePaused;
	TIME::PAUSE_CLOCK(true);
	*/

	int calDay = TIME::GET_CLOCK_DAY_OF_MONTH();
	int calMon = TIME::GET_CLOCK_MONTH();
	int calYear = TIME::GET_CLOCK_YEAR();

	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();

	bool leapYear = false;
	if (calYear % 4 == 0)
	{
		leapYear = true;
	}

	/*
	std::ostringstream ss2;
	ss2 << "Date is: ";
	ss2 << std::setfill('0') << std::setw(2) << calDay;
	ss2 << ".";
	ss2 << std::setfill('0') << std::setw(2) << calMon;
	ss2 << ".";
	ss2 << calYear;
	set_status_text(ss2.str());
	*/

	if ((calDay == 27 && calMon == 2 && !leapYear) ||
		(calDay == 28 && calMon == 2 && leapYear) ||
		(calDay == 30 && (calMon == 4 || calMon == 6 || calMon == 9 || calMon == 11)) ||
		(calDay == 31))
	{
		calDay = 1;
		if (calMon == 12)
		{
			calMon = 1;
			calYear++;
		}
		else
		{
			calMon++;
		}
	}
	else
	{
		calDay++;
	}

	TIME::SET_CLOCK_DATE(calDay, calMon, calYear);
	TIME::SET_CLOCK_TIME(gameHour, gameMins, 0);

	std::ostringstream ss;
	ss << "Date is now: " << get_day_of_game_week() << " ";
	ss << std::setfill('0') << std::setw(2) << TIME::GET_CLOCK_DAY_OF_MONTH();
	ss << ".";
	ss << std::setfill('0') << std::setw(2) << TIME::GET_CLOCK_MONTH();
	ss << ".";
	ss << TIME::GET_CLOCK_YEAR();
	set_status_text(ss.str());

	//TIME::PAUSE_CLOCK(timeWasPaused);
}

void movetime_day_backward()
{
	int calDay = TIME::GET_CLOCK_DAY_OF_MONTH();
	int calMon = TIME::GET_CLOCK_MONTH();
	int calYear = TIME::GET_CLOCK_YEAR();

	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();

	bool leapYear = false;
	if (calYear % 4 == 0)
	{
		leapYear = true;
	}

	if (calDay != 1)
	{
		calDay--;
	}
	else if (calMon == 1)
	{
		calDay = 31;
		calMon = 12;
		calYear--;
	}
	else
	{
		if (calMon == 5 || calMon == 7 || calMon == 10 || calMon == 12)
		{
			calDay = 30;
		}
		if (calMon == 3)
		{
			if (leapYear)
			{
				calDay = 29;
			}
			else
			{
				calDay = 28;
			}
		}
		else
		{
			calDay = 31;
		}
		calMon--;
	}

	TIME::SET_CLOCK_DATE(calDay, calMon, calYear);
	TIME::SET_CLOCK_TIME(gameHour, gameMins, 0);

	std::ostringstream ss;
	ss << "Date is now " << get_day_of_game_week() << " ";
	ss << std::setfill('0') << std::setw(2) << calDay;
	ss << ".";
	ss << std::setfill('0') << std::setw(2) << calMon;
	ss << ".";
	ss << calYear;
	set_status_text(ss.str());
}

void movetime_hour_forward()
{
	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();
	gameHour++;
	if (gameHour == 24)
	{
		movetime_day_forward();
		gameHour = 00;
	}
	TIME::SET_CLOCK_TIME(gameHour, gameMins, 00);
	char text[32];
	sprintf_s(text, "Time is now %02d:%02d", gameHour, gameMins);
	set_status_text(text);
}

void movetime_hour_backward()
{
	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();
	gameHour--;
	if (gameHour == -1)
	{
		movetime_day_backward();
		gameHour = 23;
	}
	TIME::SET_CLOCK_TIME(gameHour, gameMins, 00);
	char text[32];
	sprintf_s(text, "Time is now %02d:%02d", gameHour, gameMins);
	set_status_text(text);
}

void movetime_fivemin_forward()
{
	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();

	if (gameHour == 23 && gameMins > 54)
	{
		movetime_day_forward();
		gameHour = 0;
		gameMins = (gameMins + (-55));
	}
	else if (gameMins > 54)
	{
		gameHour++;
		gameMins = gameMins + (-55);
	}
	else
	{
		gameMins = gameMins + 5;
	}

	TIME::SET_CLOCK_TIME(gameHour, gameMins, 00);
	char text[32];
	sprintf_s(text, "Time is now %02d:%02d", gameHour, gameMins);
	set_status_text(text);
}

void movetime_fivemin_backward()
{
	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();

	if (gameHour == 0 && gameMins < 5)
	{
		movetime_day_backward();
		gameHour = 23;
		gameMins = gameMins + 55;
	}
	else if (gameMins < 5)
	{
		gameHour--;
		gameMins = gameMins + 55;
	}
	else
	{
		gameMins = gameMins - 5;
	}

	TIME::SET_CLOCK_TIME(gameHour, gameMins, 00);
	char text[32];
	sprintf_s(text, "Time is now %02d:%02d", gameHour, gameMins);
	set_status_text(text);
}

std::string get_day_of_game_week()
{
	int day = TIME::GET_CLOCK_DAY_OF_WEEK();
	switch (day)
	{
	case 0:
		return "Sun";
	case 1:
		return "Mon";
	case 2:
		return "Tue";
	case 3:
		return "Wed";
	case 4:
		return "Thu";
	case 5:
		return "Fri";
	case 6:
		return "Sat";
	}
	return std::string();
}

void handle_generic_settings_time(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("timeSpeedIndexWhileAiming") == 0)
		{
			timeSpeedIndexWhileAiming = stoi(setting.value);
		}
	}
}

void add_time_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "timeSpeedIndexWhileAiming", std::to_string(timeSpeedIndexWhileAiming) });
}

void update_time_features(Player player)
{
	// time pause
	if (featureTimePaused || featureTimePausedUpdated)
	{
		TIME::PAUSE_CLOCK(featureTimePaused);
		featureTimePausedUpdated = false;
	}

	// time sync
	if (featureTimeSynced)
	{
		if (featureTimePaused)
		{
			featureTimePaused = false;
			featureTimePausedUpdated = true;
		}

		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		TIME::SET_CLOCK_TIME(t.tm_hour, t.tm_min, t.tm_sec);
	}

	if ((is_in_airbrake_mode() && is_airbrake_frozen_time()) || is_in_prop_placement_mode() && is_prop_placement_frozen_time())
	{
		GAMEPLAY::SET_TIME_SCALE(0.0f);
		weHaveChangedTimeScale = true;
	}
	else if (CONTROLS::IS_CONTROL_PRESSED(0, 19) || PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()))
	{
		//do nothing so the game chooses the speed for us
	}
	else if (is_hotkey_held_normal_speed())
	{
		GAMEPLAY::SET_TIME_SCALE(1.0f);
		weHaveChangedTimeScale = true;
	}
	else if (is_hotkey_held_slow_mo())
	{
		GAMEPLAY::SET_TIME_SCALE(0.0f);
		weHaveChangedTimeScale = true;
	}
	else if (PLAYER::IS_PLAYER_FREE_AIMING(player) && PLAYER::IS_PLAYER_CONTROL_ON(player))
	{
		if (timeSinceAimingBegan == 0)
		{
			timeSinceAimingBegan = GetTickCount();
		}

		if ((GetTickCount() - timeSinceAimingBegan) < TIME_TO_SLOW_AIM)
		{
			float fullSpeedTime = weHaveChangedTimeScale ? TIME_SPEED_VALUES.at(timeSpeedIndex) : 1.0f;
			float targetTime = TIME_SPEED_VALUES.at(timeSpeedIndexWhileAiming);
			
			float progress = ((float)(GetTickCount() - timeSinceAimingBegan) / TIME_TO_SLOW_AIM);

			float rate = quadratic_time_transition(fullSpeedTime, targetTime, progress);
			
			GAMEPLAY::SET_TIME_SCALE(rate);
		}
		else
		{
			GAMEPLAY::SET_TIME_SCALE(TIME_SPEED_VALUES.at(timeSpeedIndexWhileAiming));
			weHaveChangedTimeScale = true;
		}
	}
	else if (PLAYER::IS_PLAYER_CONTROL_ON(player))
	{
		GAMEPLAY::SET_TIME_SCALE(TIME_SPEED_VALUES.at(timeSpeedIndex));
		weHaveChangedTimeScale = true;
	}
	else if (weHaveChangedTimeScale)
	{
		GAMEPLAY::SET_TIME_SCALE(1.0f);
		weHaveChangedTimeScale = false;
	}

	if (timeSinceAimingBegan > 0 && !(PLAYER::IS_PLAYER_FREE_AIMING(player) && PLAYER::IS_PLAYER_CONTROL_ON(player)))
	{
		timeSinceAimingBegan = 0;
	}
}