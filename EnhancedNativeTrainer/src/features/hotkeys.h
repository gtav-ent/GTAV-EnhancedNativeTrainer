/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include <vector>
#include "..\storage\\database.h"

struct HOTKEY_DEF
{
	std::string caption;
	int id;
};

static const int HKEY_FIX_CAR = 101;
static const int HKEY_CLEAN_CAR = 102;
static const int HKEY_HEAL_PLAYER = 103;
static const int HKEY_RESET_WANTED = 104;
static const int HKEY_REPLAY_ANIM = 105;
static const int HKEY_TOGGLE_INVISIBILITY = 106;
static const int HKEY_TELEPORT_TO_MARKER = 107;
static const int HKEY_MENU_VEHICLE_SPAWN = 108;
static const int HKEY_MENU_VEHICLE_PAINT = 109;
static const int HKEY_MENU_SKINS = 110;
static const int HKEY_MENU_SKIN_CUST = 111;
static const int HKEY_TELEPORT_TO_VEHICLE = 112;
static const int HKEY_MENU_NEXT_RADIO_TRACK = 113;
static const int HKEY_TOGGLE_THERMAL_VIS = 114;
static const int HKEY_TOGGLE_NIGHT_VIS = 115;
static const int HKEY_PASSENGER = 116;
static const int HKEY_TOGGLE_TURN_SIGNAL_LEFT = 117;
static const int HKEY_TOGGLE_TURN_SIGNAL_RIGHT = 118;

const std::vector<HOTKEY_DEF> HOTKEY_AVAILABLE_FUNCS{
	{ "No Function", 0 },
	{ "Fix Car", HKEY_FIX_CAR },
	{ "Clean Car", HKEY_CLEAN_CAR },
	{ "Heal Player", HKEY_HEAL_PLAYER },
	{ "Reset Wanted", HKEY_RESET_WANTED },
	{ "Replay Last Anim", HKEY_REPLAY_ANIM },
	{ "Teleport To Marker", HKEY_TELEPORT_TO_MARKER },
	{ "Teleport To Last Veh", HKEY_TELEPORT_TO_VEHICLE },
	{ "Toggle Invisibility", HKEY_TOGGLE_INVISIBILITY },
	{ "Veh. Spawn Menu", HKEY_MENU_VEHICLE_SPAWN },
	{ "Veh. Paint Menu", HKEY_MENU_VEHICLE_PAINT },
	{ "Skin Select Menu", HKEY_MENU_SKINS },
	{ "Skin Customise Menu", HKEY_MENU_SKIN_CUST },
	{ "Next Radio Track", HKEY_MENU_NEXT_RADIO_TRACK },
	{ "Toggle Night Vision", HKEY_TOGGLE_NIGHT_VIS },
	{ "Toggle Thermal Vision", HKEY_TOGGLE_THERMAL_VIS },
	//{ "Drive as Passenger", HKEY_PASSENGER }
	{ "Left Turn Signal", HKEY_TOGGLE_TURN_SIGNAL_LEFT },
	{ "Right Turn Signal", HKEY_TOGGLE_TURN_SIGNAL_RIGHT }
};

void change_hotkey_function(int hotkey, int funcIndex);

int get_hotkey_function_index(int hotkey);

void check_for_hotkey_presses();

void add_hotkey_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_hotkey(std::vector<StringPairSettingDBRow>* settings);

void trigger_function_for_hotkey(int i);