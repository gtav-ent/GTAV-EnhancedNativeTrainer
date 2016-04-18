/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "hotkeys.h"
#include "..\utils.h"
#include "..\io\config_io.h"

#include "anims.h"
#include "vehicles.h"
#include "script.h"
#include "area_effect.h"
#include "teleportation.h"

#include <string>
#include <sstream> 

const int MAX_HOTKEYS = 9;

int functionIDs[MAX_HOTKEYS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

bool hotkey_held_slow_mo = false;

bool hotkey_held_normal_speed = false;

bool hotkey_held_veh_burnout = false;

bool hotkey_held_veh_extrapower = false;

bool is_hotkey_held_slow_mo()
{
	return hotkey_held_slow_mo;
}

bool is_hotkey_held_normal_speed()
{
	return hotkey_held_normal_speed;
}

bool is_hotkey_held_veh_burnout()
{
	return hotkey_held_veh_burnout;
}

bool is_hotkey_held_veh_extrapower()
{
	return hotkey_held_veh_extrapower;
}

void check_for_hotkey_presses()
{
	KeyInputConfig* config = get_config()->get_key_config();
	for (int i = 0; i < MAX_HOTKEYS; i++)
	{
		if (!config->is_hotkey_assigned(i + 1))
		{
			continue;
		}

		std::string target;
		switch (i+1)
		{
		case 1:
			target = KeyConfig::KEY_HOT_1;
			break;
		case 2:
			target = KeyConfig::KEY_HOT_2;
			break;
		case 3:
			target = KeyConfig::KEY_HOT_3;
			break;
		case 4:
			target = KeyConfig::KEY_HOT_4;
			break;
		case 5:
			target = KeyConfig::KEY_HOT_5;
			break;
		case 6:
			target = KeyConfig::KEY_HOT_6;
			break;
		case 7:
			target = KeyConfig::KEY_HOT_7;
			break;
		case 8:
			target = KeyConfig::KEY_HOT_8;
			break;
		case 9:
			target = KeyConfig::KEY_HOT_9;
			break;
		default:
			break;
		}

		if (!target.empty() && IsKeyJustUp(target))
		{
			trigger_function_for_hotkey_onkeyup(i);
		}
		else if (!target.empty() && IsKeyDown(target))
		{
			trigger_function_for_hotkey_onkeydown(i);
		}
	}
}

void trigger_function_for_hotkey_onkeyup(int hotkey)
{
	int function = functionIDs[hotkey];
	if (function == 0)
	{
		return;
	}

	switch (function)
	{
	case HKEY_CLEAN_CAR:
		clean_vehicle();
		break;
	case HKEY_FIX_CAR:
		fix_vehicle();
		break;
	case HKEY_HEAL_PLAYER:
		heal_player();
		break;
	case HKEY_REPLAY_ANIM:
		replay_last_anim();
		break;
	case HKEY_RESET_WANTED:
		reset_wanted_level();
		break;
	case HKEY_TELEPORT_TO_MARKER:
		teleport_to_marker();
		break;
	case HKEY_TELEPORT_TO_VEHICLE:
		teleport_to_last_vehicle();
		break;
	case HKEY_TOGGLE_INVISIBILITY:
		toggle_invisibility();
		break;
	case HKEY_MENU_VEHICLE_SPAWN:
		set_status_text("Not implemented yet");
		break;
	case HKEY_MENU_VEHICLE_PAINT:
		set_status_text("Not implemented yet");
		break;
	case HKEY_MENU_SKINS:
		set_status_text("Not implemented yet");
		break;
	case HKEY_MENU_SKIN_CUST:
		set_status_text("Not implemented yet");
		break;
	case HKEY_MENU_NEXT_RADIO_TRACK:
		AUDIO::SKIP_RADIO_FORWARD();
		break;
	case HKEY_TOGGLE_NIGHT_VIS:
		toggle_night_vision();
		break;
	case HKEY_TOGGLE_THERMAL_VIS:
		toggle_thermal_vision();
		break;
	case HKEY_PASSENGER:
		drive_passenger();
		break;
	case HKEY_VEHICLE_BURNOUT:
		hotkey_held_veh_burnout = false;
		break;
	case HKEY_VEHICLE_POWER:
		hotkey_held_veh_extrapower = false;
		break;
	case HKEY_SLOW_MOTION:
		hotkey_held_slow_mo = false;
		break;
	case HKEY_NORMAL_SPEED:
		hotkey_held_normal_speed = false;
		break;
	case HKEY_KILL_NEARBY_PEDS:
		kill_all_nearby_peds_now();
		break;
	case HKEY_EXPLODE_NEARBY_VEHS:
		kill_all_nearby_vehicles_now();
		break;
	default:
	{
		std::ostringstream ss;
		ss << "Hotkey " << (hotkey + 1) << " has unrecognised function";
		set_status_text(ss.str());
		break;
	}
	}
}

void trigger_function_for_hotkey_onkeydown(int hotkey)
{
	int function = functionIDs[hotkey];
	if (function == 0)
	{
		return;
	}

	switch (function)
	{
		case HKEY_VEHICLE_BURNOUT:
			hotkey_held_veh_burnout = true;
			break;
		case HKEY_SLOW_MOTION:
			hotkey_held_slow_mo = true;
			break;
		case HKEY_NORMAL_SPEED:
			hotkey_held_normal_speed = true;
			break;
		case HKEY_VEHICLE_POWER:
			hotkey_held_veh_extrapower = true;
			break;
		default:
			break;
	}
}

void add_hotkey_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	for (int i = 0; i < MAX_HOTKEYS; i++)
	{
		std::ostringstream ss;
		ss << "hotkey_" << i;
		results->push_back(StringPairSettingDBRow{ ss.str(), std::to_string(functionIDs[i]) });
	}
}

void handle_generic_settings_hotkey(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (StringStartsWith(setting.name, "hotkey_"))
		{
			int id = stoi(setting.name.substr(7));
			int value = stoi(setting.value);
			functionIDs[id] = value;
		}
	}
}


void change_hotkey_function(int hotkey, int funcIndex)
{
	functionIDs[hotkey - 1] = HOTKEY_AVAILABLE_FUNCS.at(funcIndex).id;
}

int get_hotkey_function_index(int hotkey)
{
	int func = functionIDs[hotkey - 1];
	int i = 0;
	for each (HOTKEY_DEF def in HOTKEY_AVAILABLE_FUNCS)
	{
		if (def.id == func)
		{
			return i;
		}
		i++;
	}
	return 0;
}