/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

/*
	F4					activate
	NUM2/8/4/6			navigate thru the menus and lists (numlock must be on)
	NUM5 				select
	NUM0/BACKSPACE/F4 	back
	NUM9/3 				use vehicle boost when active
	NUM+ 				use vehicle rockets when active
*/

#pragma comment(lib, "Shlwapi.lib")

#include "script.h"
#include "../version.h"

#pragma warning(disable : 4244 4305) // double <-> float conversions

int game_frame_num = 0;

bool everInitialised = false;

ENTDatabase* database = NULL;

//std::mutex db_mutex;

// features
bool featurePlayerInvincible			=	false;
bool featurePlayerInvincibleUpdated		=	false;
bool featurePlayerNeverWanted			=	false;
bool featurePlayerNeverWantedUpdated = false;
bool featurePlayerIgnoredByPolice				=	false;
bool featurePlayerIgnoredByPoliceUpdated		=	false;
bool featurePlayerIgnoredByAll = false;
bool featurePlayerIgnoredByAllUpdated = false;
bool featurePlayerUnlimitedAbility		=	false;
bool featurePlayerNoNoise				=	false;
bool featurePlayerNoNoiseUpdated		=	false;
bool featurePlayerFastSwim				=	false;
bool featurePlayerFastSwimUpdated		=	false;
bool featurePlayerFastRun				=	false;
bool featurePlayerFastRunUpdated		=	false;
bool featurePlayerSuperJump				=	false;
bool featurePlayerInvisible				=	false;
bool featurePlayerInvisibleUpdated		=	false;
bool featurePlayerDrunk = false;
bool featurePlayerDrunkUpdated = false;
bool featurePlayerRadio					=	false;
bool featurePlayerRadioUpdated			=	false;
bool featureRadioAlwaysOff = false;
bool featureRadioAlwaysOffUpdated = false;

bool featureMiscLockRadio				=	false;
bool featureMiscHideHud					=	false;

bool featureWantedLevelFrozen			=	false;
bool featureWantedLevelFrozenUpdated	=	false;
int  frozenWantedLevel					=	0;

bool featureBlockInputInMenu = true;
bool featureVehInvulnIncludesCosmetic = true;
bool featurePlayerResetOnDeath = true;

// player model control, switching on normal ped model when needed	

LPCSTR player_models[] = { "player_zero", "player_one", "player_two" };

const char* CLIPSET_DRUNK = "move_m@drunk@verydrunk";

const std::vector<std::string> GRAVITY_CAPTIONS{ "Minimum", "0.1x", "0.5x", "0.75x", "1x (Normal)" };
const std::vector<float> GRAVITY_VALUES{ 0.0f, 0.1f, 0.5f, 0.75f, 1.0f };

void check_player_model()
{
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;

	if (ENTITY::IS_ENTITY_DEAD(playerPed) && featurePlayerResetOnDeath)
	{
		bool found = false;
		Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);

		for (int i = 0; i < (sizeof(player_models) / sizeof(player_models[0])); i++)
		{
			if (GAMEPLAY::GET_HASH_KEY((char *)player_models[i]) == model)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			set_status_text("Resetting player model");
			applyChosenSkin("player_zero");
		}

		// wait until player is ressurected
		while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()))
		{
			WAIT(0);
		}
	}
}

// Updates all features that can be turned off by the game, being called each game frame
void update_features()
{
	everInitialised = true;
	game_frame_num++;
	if (game_frame_num >= 100000)
	{
		game_frame_num = 0;
	}

	if (game_frame_num % 1000 == 0)
	{
		DWORD myThreadID;
		HANDLE myHandle = CreateThread(0, 0, save_settings_thread, 0, 0, &myThreadID);
		CloseHandle(myHandle);
	}

	UpdateXInputControlState();

	if (is_menu_showing() || is_in_airbrake_mode())
	{
		if (should_block_input_in_menu() || is_in_airbrake_mode())
		{
			setGameInputToEnabled(false);
		}
		else
		{
			setGameInputToEnabled(true);
		}
	}
	else
	{
		setGameInputToEnabled(true);
	}

	if (is_in_airbrake_mode())
	{
		setAirbrakeRelatedInputToBlocked(true);
	}
	else
	{
		setAirbrakeRelatedInputToBlocked(false);
	}

	

	update_centre_screen_status_text();

	update_vehicle_guns();

	update_world_features();

	check_player_model();

	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();	
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	//PLAYER::DISABLE_PLAYER_FIRING(playerPed, TRUE);

	// player invincible
	if (featurePlayerInvincibleUpdated)
	{
		if (bPlayerExists && !featurePlayerInvincible)
			PLAYER::SET_PLAYER_INVINCIBLE(player, FALSE);
		featurePlayerInvincibleUpdated = false;
	}
	if (featurePlayerInvincible)
	{
		if (bPlayerExists)
			PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
	}

	
	//Wanted Level Frozen - prevents stars increasing/decreasing
	if (featureWantedLevelFrozen)
	{
		if (featureWantedLevelFrozenUpdated)
		{
			frozenWantedLevel = PLAYER::GET_PLAYER_WANTED_LEVEL(player);
			PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
			featureWantedLevelFrozenUpdated = false;

			if (frozenWantedLevel > 0)
			{
				std::stringstream ss;
				ss << "Wanted Level frozen at: " << frozenWantedLevel << " Star";
				if (frozenWantedLevel > 1){ ss << "s"; }
				set_status_text(ss.str());
			}
		}
		if (frozenWantedLevel > 0)
		{
			if (bPlayerExists)
				PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		}
		else
		{
			featureWantedLevelFrozen = false;
			set_status_text("You must have a Wanted Level first.");
		}
	}
	if (featureWantedLevelFrozenUpdated)
	{
		if (!featureWantedLevelFrozen)
		{
			set_status_text("Wanted Level Unfrozen");
			PLAYER::SET_MAX_WANTED_LEVEL(5);
		}
		featureWantedLevelFrozenUpdated = false;
	}
	
	
	// player never wanted
	if (featurePlayerNeverWanted)
	{
		if (bPlayerExists)
		{
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
			PLAYER::SET_MAX_WANTED_LEVEL(0);
		}
	}
	else if (featurePlayerNeverWantedUpdated)
	{
		PLAYER::SET_MAX_WANTED_LEVEL(5);
	}

	// police ignore player
	if (featurePlayerIgnoredByPolice)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
		}
	}
	else if (featurePlayerIgnoredByPoliceUpdated)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, false);
		}
		featurePlayerIgnoredByPoliceUpdated = false;
	}

	// everyone ignores player
	if (featurePlayerIgnoredByAll)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, true);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, false);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, true);
			if (game_frame_num % 5 == 0)
			{
				set_all_nearby_peds_to_calm(playerPed, 50);
			}
		}
	}
	else if (featurePlayerIgnoredByAllUpdated)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, featurePlayerIgnoredByPolice);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, false);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, true);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, false);
		}
		featurePlayerIgnoredByAllUpdated = false;
	}

	// player special ability
	if (featurePlayerUnlimitedAbility)
	{
		if (bPlayerExists)
			PLAYER::_RECHARGE_SPECIAL_ABILITY(player, 1);
	}

	// player no noise
	if (featurePlayerNoNoiseUpdated)
	{
		if (bPlayerExists && !featurePlayerNoNoise)
			PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 1.0);
		featurePlayerNoNoiseUpdated = false;
	}
	if (featurePlayerNoNoise)
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 0.0);

	// player fast swim
	if (featurePlayerFastSwimUpdated)
	{
		if (bPlayerExists && !featurePlayerFastSwim)
			PLAYER::_SET_SWIM_SPEED_MULTIPLIER(player, 1.0);
		featurePlayerFastSwimUpdated = false;
	}
	if (featurePlayerFastSwim)
		PLAYER::_SET_SWIM_SPEED_MULTIPLIER(player, 1.49);

	// player fast run
	if (featurePlayerFastRunUpdated)
	{
		if (bPlayerExists && !featurePlayerFastRun)
			PLAYER::_SET_MOVE_SPEED_MULTIPLIER(player, 1.0);
		featurePlayerFastRunUpdated = false;
	}
	if (featurePlayerFastRun)
		PLAYER::_SET_MOVE_SPEED_MULTIPLIER(player, 1.49);

	// player super jump
	if (featurePlayerSuperJump)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
	}

	//Player Invisible
	if (featurePlayerInvisibleUpdated)
	{
		featurePlayerInvisibleUpdated = false;
		if (bPlayerExists && featurePlayerInvisible)
			ENTITY::SET_ENTITY_VISIBLE(playerPed, false);
		else if (bPlayerExists){ ENTITY::SET_ENTITY_VISIBLE(playerPed, true); }
	}

	if (featurePlayerDrunkUpdated)
	{
		featurePlayerDrunkUpdated = false;
		if (featurePlayerDrunk)
		{
			STREAMING::REQUEST_ANIM_SET((char*)CLIPSET_DRUNK);
			while (!STREAMING::HAS_ANIM_SET_LOADED((char*)CLIPSET_DRUNK))
			{
				WAIT(0);
			}
			PED::SET_PED_MOVEMENT_CLIPSET(playerPed, (char*)CLIPSET_DRUNK, 1.0f);
			CAM::SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 1.0f);
		}
		else
		{
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			CAM::STOP_GAMEPLAY_CAM_SHAKING(true);
		}
		AUDIO::SET_PED_IS_DRUNK(playerPed, featurePlayerDrunk);
	}

	if (featureRadioAlwaysOff || featurePlayerRadioUpdated)
	{
		if (featureRadioAlwaysOff)
		{
			if (featurePlayerRadio)
			{
				featurePlayerRadio = false;
				featurePlayerRadioUpdated = true;
			}
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, !featureRadioAlwaysOff);
		}

		AUDIO::SET_USER_RADIO_CONTROL_ENABLED(!featureRadioAlwaysOff);
	}

	// Portable radio
	if (featurePlayerRadio || featurePlayerRadioUpdated)
	{
		if (featurePlayerRadio)
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
		else
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		}
	}

	update_weapon_features(bPlayerExists, player);

	update_vehicle_features(bPlayerExists, playerPed);

	update_time_features(player);

	// hide hud
	if (featureMiscHideHud)
	{
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}

	//Disable airbrake on death
	if (ENTITY::IS_ENTITY_DEAD(playerPed))
	{
		exit_airbrake_menu_if_showing();
	}

	//----Hotkeys----
	
	//Move through door (use '-key)
	//Pushes player through solid door objects.
	if (bPlayerExists)
	{
		bool throughDoorPressed = IsKeyJustUp(KeyConfig::KEY_HOT_AIRBRAKE_THROUGH_DOOR) || IsControllerButtonJustUp(KeyConfig::KEY_HOT_AIRBRAKE_THROUGH_DOOR);
		//bool disablePolicePressed = IsKeyJustUp(VK_OEM_6);
		if (throughDoorPressed)
		{
			moveThroughDoor();
		}
	}
}

int activeLineIndexWantedFreeze = 0;

const std::vector<std::string> MENU_WANTED_LEVELS{ "1 Star", "2 Stars", "3 Stars", "4 Stars", "5 Stars", "OFF/Clear" };


int getFrozenWantedLvl(){ return frozenWantedLevel; }
void setFrozenWantedLvl(int level){ frozenWantedLevel = level; }
void setFrozenWantedFeature(bool b){ featureWantedLevelFrozen = b; }
bool getFrozenWantedFeature(){ return featureWantedLevelFrozen; }

bool onConfirm_wantedlevel_menu(int selection, std::string caption, int value)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	activeLineIndexWantedFreeze = selection;

	switch (selection)
	{
	case 0:
		if (bPlayerExists)
		{
			featureWantedLevelFrozen = true;
			frozenWantedLevel = 1;
			PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
			set_status_text("Wanted Level Frozen at 1 Star");
		}
		break;
	case 1:
		if (bPlayerExists)
		{
			featureWantedLevelFrozen = true;
			frozenWantedLevel = 2;
			PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
			set_status_text("Wanted Level Frozen at 2 Stars");
		}
		break;
	case 2:
		if (bPlayerExists)
		{
			featureWantedLevelFrozen = true;
			frozenWantedLevel = 3;
			PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
			set_status_text("Wanted Level Frozen at 3 Stars");
		}
		break;
	case 3:
		if (bPlayerExists)
		{
			featureWantedLevelFrozen = true;
			frozenWantedLevel = 4;
			PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
			set_status_text("Wanted Level Frozen at 4 Stars");
		}
		break;
	case 4:
		if (bPlayerExists)
		{
			featureWantedLevelFrozen = true;
			frozenWantedLevel = 5;
			PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
			set_status_text("Wanted Level Frozen at 5 Stars");
		}
		break;
	default:
		if (bPlayerExists)
		{
			featureWantedLevelFrozen = false;
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
			PLAYER::SET_MAX_WANTED_LEVEL(5);
			set_status_text("Wanted Level settings returned to default.");
		}
		break;
	}
	return false;
}

int activeLineIndexPlayer = 0;

bool onconfirm_player_menu(MenuItem<int> choice)
{

	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	switch (activeLineIndexPlayer)
	{
		// skin changer
	case 0:
		if (process_skinchanger_menu())	return true;
		break;
	case 1:
	{
		ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed));
		PED::ADD_ARMOUR_TO_PED(playerPed, PLAYER::GET_PLAYER_MAX_ARMOUR(player) - PED::GET_PED_ARMOUR(playerPed));
		PED::SET_PED_WETNESS_HEIGHT(playerPed, -2.0);
		PED::CLEAR_PED_BLOOD_DAMAGE(playerPed);
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (ENTITY::DOES_ENTITY_EXIST(playerVeh) && !ENTITY::IS_ENTITY_DEAD(playerVeh))
				VEHICLE::SET_VEHICLE_FIXED(playerVeh);
		}
		set_status_text("Player Healed");
	}
	break;
	case 16:
		process_anims_menu_top();
		break;
	default:
		break;
	}
	return false;
}

void process_player_menu()
{
	const int lineCount = 17;
	
	std::string caption = "Player Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{"Player Skin", NULL, NULL, false},
		{"Heal Player", NULL, NULL, true},
		{"Add Cash", NULL, NULL, true, CASH},
		{"Wanted Level", NULL, NULL, true, WANTED},
		{ "Freeze Wanted Level", &featureWantedLevelFrozen, &featureWantedLevelFrozenUpdated, true },
		{ "Never Wanted", &featurePlayerNeverWanted, &featurePlayerNeverWantedUpdated, true },
		{"Invincible", &featurePlayerInvincible, &featurePlayerInvincibleUpdated, true},
		{"Police Ignore You", &featurePlayerIgnoredByPolice, &featurePlayerIgnoredByPoliceUpdated, true },
		{ "Everyone Ignores You", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated, true },
		{"Unlimited Ability", &featurePlayerUnlimitedAbility, NULL, true},
		{"Noiseless", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated, true},
		{"Fast Swim", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated, true},
		{"Fast Run", &featurePlayerFastRun, &featurePlayerFastRunUpdated, true},
		{"Super Jump", &featurePlayerSuperJump, NULL, true},
		{"Invisibility", &featurePlayerInvisible, &featurePlayerInvisibleUpdated, true},
		{ "Drunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated, true },
		{ "Anims", NULL, NULL, false }
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexPlayer, caption, onconfirm_player_menu);
}

//==================
// MISC MENU
//==================

int activeLineIndexTrainerConfig = 0;

void process_misc_trainerconfig_menu()
{
	const int lineCount = 3;

	std::string caption = "Trainer Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Lock Controls While In Menu", &featureBlockInputInMenu, NULL },
		{ "Reset Skin On Death", &featurePlayerResetOnDeath, NULL },
		{ "Vehicle Invinc. Includes Visual Damage", &featureVehInvulnIncludesCosmetic, NULL },
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexTrainerConfig, caption, NULL);
}

int activeLineIndexMisc = 0;

bool onconfirm_misc_menu(MenuItem<int> choice)
{
	switch (activeLineIndexMisc)
	{
	case 0:
		process_misc_trainerconfig_menu();
		break;
		// next radio track
	case 2:
		AUDIO::SKIP_RADIO_FORWARD();
		break;
		// switchable features
	default:
		break;
	}
	return false;
}

void process_misc_menu()
{
	const int lineCount = 5;

	std::string caption = "Miscellaneous Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Trainer Options", NULL, NULL, false },
		{ "Portable Radio", &featurePlayerRadio, &featurePlayerRadioUpdated, true },
		{"Next Radio Track",	NULL,					NULL, true},
		{ "Radio Always Off", &featureRadioAlwaysOff, &featureRadioAlwaysOffUpdated, true },
		{"Hide HUD",			&featureMiscHideHud,	NULL},
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexMisc, caption, onconfirm_misc_menu);
}

//==================
// MAIN MENU
//==================

int activeLineIndexMain = 0;

bool onconfirm_main_menu(MenuItem<int> choice)
{
	switch (activeLineIndexMain)
	{
	case 0:
		process_player_menu();
		break;
	case 1:
		process_teleport_menu(-1);
		break;
	case 2:
		process_weapon_menu();
		break;
	case 3:
		process_veh_menu();
		break;
	case 4:
		process_world_menu();
		break;
	case 5:
		process_time_menu();
		break;
	case 6:
		process_misc_menu();
		break;
	case 7:
		reset_globals();
		break;
	case 8:
		process_test_menu();
		break;
	}
	return false;
}

void process_main_menu()
{
	std::ostringstream captionSS;
	captionSS << "~HUD_COLOUR_MENU_YELLOW~Enhanced ~HUD_COLOUR_WHITE~Native Trainer ~HUD_COLOUR_GREY~Update ";
	captionSS << VERSION_STRING;

	std::vector<std::string> TOP_OPTIONS = {
		"Player",
		"Teleport",
		"Weapons",
		"Vehicles",
		"World",
		"Time",
		"Miscellaneous",
		"Reset All Settings"
	};

	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < TOP_OPTIONS.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = TOP_OPTIONS[i];
		item->value = i;
		item->isLeaf = (i==7);
		item->currentMenuIndex = i;
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, &activeLineIndexMain, captionSS.str(), onconfirm_main_menu, NULL, NULL);
}

void reset_globals()
{
	reset_skin_globals();

	reset_vehicle_globals();

	reset_teleporter_globals();

	reset_weapon_globals();

	reset_world_globals();

	activeLineIndexMain			=
	activeLineIndexPlayer		=
	activeLineIndexWantedFreeze	=
	frozenWantedLevel			=	0;

	featurePlayerInvincible			=
	featurePlayerNeverWanted		=
	featurePlayerIgnoredByPolice			=
	featurePlayerIgnoredByAll =
	featurePlayerUnlimitedAbility	=
	featurePlayerNoNoise			=
	featurePlayerFastSwim			=
	featurePlayerFastRun			=
	featurePlayerSuperJump			=
	featurePlayerInvisible			=
	featurePlayerDrunk =
	featurePlayerRadio				=
	featureRadioAlwaysOff =
	featureMiscLockRadio			=
	featureMiscHideHud				=	
	featureWantedLevelFrozen		=	false;

	featurePlayerResetOnDeath = true;
	featureBlockInputInMenu = true;
	featureVehInvulnIncludesCosmetic = true;

	featurePlayerInvincibleUpdated =
	featurePlayerNeverWantedUpdated =
	featurePlayerIgnoredByPoliceUpdated =
	featurePlayerIgnoredByAllUpdated =
	featurePlayerNoNoiseUpdated =
	featurePlayerFastSwimUpdated =
	featurePlayerFastRunUpdated =
	featureRadioAlwaysOffUpdated =
	featurePlayerRadioUpdated =
	featurePlayerDrunkUpdated =
	featurePlayerInvisibleUpdated = true;

	set_status_text("Reset All Settings");

	DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, save_settings_thread, 0, 0, &myThreadID);
	CloseHandle(myHandle);

}

void main()
{	
	//reset_globals();

	setGameInputToEnabled(true, true);
	setAirbrakeRelatedInputToBlocked(false, true);

	write_text_to_log_file("Setting up calls");

	set_periodic_feature_call(update_features);

	write_text_to_log_file("Loading settings");

	load_settings();

	write_text_to_log_file("Loaded settings OK");

	// this creates a new locale based on the current application default
	// (which is either the one given on startup, but can be overriden with
	// std::locale::global) - then extends it with an extra facet that 
	// controls numeric output.
	std::locale comma_locale(std::locale(), new comma_numpunct());

	// tell cout to use our new locale.
	std::cout.imbue(comma_locale);

	while (true)
	{
		if (trainer_switch_pressed())
		{
			menu_beep();
			set_menu_showing(true);
			process_main_menu();
			set_menu_showing(false);
		}
		else if (airbrake_switch_pressed())
		{
			menu_beep();
			process_airbrake_menu();
		}

		update_features();

		WAIT(0);
	}
}

void make_minidump(EXCEPTION_POINTERS* e)
{
	write_text_to_log_file("Dump requested");

	auto hDbgHelp = LoadLibraryA("dbghelp");
	if (hDbgHelp == nullptr)
		return;
	auto pMiniDumpWriteDump = (decltype(&MiniDumpWriteDump))GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (pMiniDumpWriteDump == nullptr)
		return;

	auto hFile = CreateFileA(get_storage_dir_path("ENT-minidump.dmp"), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = e;
	exceptionInfo.ClientPointers = FALSE;

	auto dumped = pMiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory),
		e ? &exceptionInfo : nullptr,
		nullptr,
		nullptr);

	CloseHandle(hFile);

	write_text_to_log_file("Dump complete");

	return;
}

LONG CALLBACK unhandled_handler(EXCEPTION_POINTERS* e)
{
	write_text_to_log_file("Exception occured");
	make_minidump(e);
	return EXCEPTION_EXECUTE_HANDLER;
}

int filterException(int code, PEXCEPTION_POINTERS ex)
{
	write_text_to_log_file("ScriptMain exception");
	make_minidump(ex);
	return EXCEPTION_EXECUTE_HANDLER;
}

void ScriptMain()
{
	#ifdef _DEBUG
	__try
	{
#endif

		clear_log_file();

		write_text_to_log_file("Trying to init storage");
		init_storage();
		write_text_to_log_file("Init storage complete");

		write_text_to_log_file("Trying to init XINPUT");
		init_xinput();
		write_text_to_log_file("Init XINPUT complete");

		database = new ENTDatabase();
		if (!database->open() )
		{
			write_text_to_log_file("Failed to open database");
		}

		build_anim_tree();

		write_text_to_log_file("ScriptMain called - handler set");

		srand(GetTickCount());
		write_text_to_log_file("Reading config...");
		read_config_file();
		write_text_to_log_file("Config read complete");
		main();

		write_text_to_log_file("ScriptMain ended");

#ifdef _DEBUG
	}
	__except (filterException(GetExceptionCode(), GetExceptionInformation()))
	{

	}
#endif
}

void ScriptTidyUp()
{
	setGameInputToEnabled(true, true);
	setAirbrakeRelatedInputToBlocked(false, true);

	write_text_to_log_file("ScriptTidyUp called");

	save_settings();

	end_xinput();

	if (database != NULL)
	{
		database->close();
		delete database;
	}

	write_text_to_log_file("ScriptTidyUp done");
}

void turn_off_never_wanted()
{
	featurePlayerNeverWanted = false;
	featurePlayerNeverWantedUpdated = false;
	PLAYER::SET_MAX_WANTED_LEVEL(5);
}

void set_all_nearby_peds_to_calm(Ped playerPed, int count)
{
	const int numElements = count;
	const int arrSize = numElements * 2 + 2;

	Ped *peds = new Ped[arrSize];
	peds[0] = numElements;
	int found = PED::GET_PED_NEARBY_PEDS(playerPed, peds, -1);
	int y = 0;
	for (int i = 0; i < found; i++)
	{
		int offsettedID = i * 2 + 2;

		if (!ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
		{
			continue;
		}

		Ped xped = peds[offsettedID];

		y++;
		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(xped, true);
		PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);
		PED::SET_PED_COMBAT_ATTRIBUTES(xped, 17, 1);
	}
	delete peds;
}

std::vector<FeatureEnabledLocalDefinition> get_feature_enablements()
{
	std::vector<FeatureEnabledLocalDefinition> results;

	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerInvincible", &featurePlayerInvincible, &featurePlayerInvincibleUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerNeverWanted", &featurePlayerNeverWanted, &featurePlayerNeverWantedUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerIgnoredByPolice", &featurePlayerIgnoredByPolice, &featurePlayerIgnoredByPoliceUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerIgnoredByAll", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerUnlimitedAbility", &featurePlayerUnlimitedAbility });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerNoNoise", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerFastSwim", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerFastRun", &featurePlayerFastRun, &featurePlayerFastRunUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerSuperJump", &featurePlayerSuperJump });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerInvisible", &featurePlayerInvisible, &featurePlayerInvisibleUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerDrunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerRadio", &featurePlayerRadio, &featurePlayerRadioUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featureRadioAlwaysOff", &featureRadioAlwaysOff, &featureRadioAlwaysOffUpdated });

	results.push_back(FeatureEnabledLocalDefinition{ "featureMiscLockRadio", &featureMiscLockRadio });
	results.push_back(FeatureEnabledLocalDefinition{ "featureMiscHideHud", &featureMiscHideHud });

	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerResetOnDeath", &featurePlayerResetOnDeath });
	results.push_back(FeatureEnabledLocalDefinition{ "featureVehInvulnIncludesCosmetic", &featureVehInvulnIncludesCosmetic });
	results.push_back(FeatureEnabledLocalDefinition{ "featureBlockInputInMenu", &featureBlockInputInMenu });

	add_world_feature_enablements(&results);

	add_time_feature_enablements(&results);

	std::vector<FeatureEnabledLocalDefinition> vehResults = get_feature_enablements_vehicles();
	results.insert(results.end(), vehResults.begin(), vehResults.end());

	std::vector<FeatureEnabledLocalDefinition> weapResults = get_feature_enablements_weapons();
	results.insert(results.end(), weapResults.begin(), weapResults.end());

	return results;
}

std::vector<StringPairSettingDBRow> get_generic_settings()
{
	std::vector<StringPairSettingDBRow> settings;
	add_time_generic_settings(&settings);
	add_world_generic_settings(&settings);
	add_vehicle_generic_settings(&settings);
	return settings;
}

void handle_generic_settings(std::vector<StringPairSettingDBRow> settings)
{
	/*
	for (int i = 0; i < settings.size(); i++)
	{
		StringPairSettingDBRow setting = settings.at(i);
		if (setting.name.compare("timeSpeedIndexWhileAiming") == 0)
		{
			timeSpeedIndexWhileAiming = stoi(setting.value);
		}
	}
	*/

	//pass to anyone else, vehicles, weapons etc

	handle_generic_settings_time(&settings);

	handle_generic_settings_vehicle(&settings);

	handle_generic_settings_world(&settings);
}

DWORD WINAPI save_settings_thread(LPVOID lpParameter)
{
	save_settings();
	return 0;
}

void save_settings()
{
	if (!everInitialised)
	{
		return;
	}

	write_text_to_log_file("Saving settings, start");

	/*
	if (!db_mutex.try_lock())
	{
		write_text_to_log_file("Couldn't get lock, aborting");
		return;
	}
	*/

	write_text_to_log_file("Locked");

	write_text_to_log_file("Actually saving");
	database->store_setting_pairs(get_generic_settings());
	database->store_feature_enabled_pairs(get_feature_enablements());
	write_text_to_log_file("Save flag released");

	write_text_to_log_file("Unlocking");
	//db_mutex.unlock();
	write_text_to_log_file("Unlocked");
}

void load_settings()
{
	handle_generic_settings(database->load_setting_pairs());

	write_text_to_log_file("Got generic pairs");

	database->load_feature_enabled_pairs(get_feature_enablements());

	write_text_to_log_file("Got feature pairs");

	//db_mutex.unlock();
}

void init_storage()
{
	char* folder = get_storage_dir_path();
	write_text_to_log_file("Trying to create storage folder");
	write_text_to_log_file(std::string(folder));
	if (CreateDirectory(folder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		write_text_to_log_file("Storage dir created or exists");
	}
	else
	{
		write_text_to_log_file("Couldn't create storage dir");
	}
	delete folder;
}

char* get_storage_dir_path()
{
	PWSTR localAppData;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &localAppData);

	WCHAR combined[MAX_PATH];
	PathCombineW(combined, localAppData, L"GTAV Enhanced Native Trainer");

	char *result = new char[MAX_PATH];

	wcstombs(result, combined, MAX_PATH);

	CoTaskMemFree(localAppData);

	return result;
}

char* get_storage_dir_path(char* file)
{
	char *output = new char[MAX_PATH];

	char* folder = get_storage_dir_path();
	PathCombine(output, folder, file);

	delete folder;

	return output;
}

ENTDatabase* get_database()
{
	return database;
}

bool should_block_input_in_menu()
{
	return featureBlockInputInMenu;
}

bool does_veh_invuln_include_cosmetic()
{
	return featureVehInvulnIncludesCosmetic;
}

bool onconfirm_test_menu(MenuItem<int> choice)
{
	Vehicle veh;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true))
	{
		veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	}

	switch (choice.value)
	{
	case 0:
		PED::_0xF79F9DEF0AADE61A(PLAYER::PLAYER_PED_ID());
		break;
	case 1:
		VEHICLE::_0x7D6F9A3EF26136A0(veh, true);
		break;
	case 2:
		VEHICLE::_0x7D6F9A3EF26136A0(veh, false);
		break;
	case 3:
	{
		int i = VEHICLE::_0xE6B0E8CFC3633BF0(veh);
		std::ostringstream ss;
		ss << "Value is " << i;
		set_status_text(ss.str());
		break;
	}
	case 4:
		VEHICLE::_0x0A436B8643716D14();
		break;
	case 5:
	{
		bool b = VEHICLE::_0xF7F203E31F96F6A1(veh, 0);
		std::ostringstream ss;
		ss << "Value is " << (b?1:0);
		set_status_text(ss.str());
		break;
	}
	case 6:
	{
		int i = VEHICLE::_0xE33FFA906CE74880(veh, 0);
		std::ostringstream ss;
		ss << "Value is " << i;
		set_status_text(ss.str());
		break;
	}
	case 7:
	{
		VEHICLE::_0x99AD4CCCB128CBC9(veh);
		break;
	}
	case 8:
		VEHICLE::SET_VEHICLE_REDUCE_GRIP(veh, true);
		break;
	case 9:
		VEHICLE::SET_VEHICLE_REDUCE_GRIP(veh, false);
		break;
	case 10:
		VEHICLE::SET_VEHICLE_FRICTION_OVERRIDE(veh, 0.1f);
		break;
	case 11:
		VEHICLE::SET_VEHICLE_FRICTION_OVERRIDE(veh, 1.0f);
		break;
	case 12:
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 5.0f);
		break;
	case 13:
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 1.0f);
		break;
	case 14:
		VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(veh, 5.0f);
		break;
	case 15:
		VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(veh, 1.0f);
		break;
	}
	return false;
}

void process_test_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	for (int i = 0; i <=15; i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = i;

		std::ostringstream ss;
		ss << "Test #" << i;

		item->caption = ss.str();
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, 0, "Test Funcs", onconfirm_test_menu, NULL, NULL, skin_save_menu_interrupt);

}

void debug_native_investigation()
{
	if (!PED::_0x784002A632822099(PLAYER::PLAYER_PED_ID())) //putting on helmet?
	{
		set_status_text_centre_screen("0x784... false");
	}

	if (!PED::_0x66680A92700F43DF(PLAYER::PLAYER_PED_ID())) //getting in plane?
	{
		set_status_text_centre_screen("0x666... false");
	}

	if (PED::_0x0525A2C2562F3CD4(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x0525... true");
	}
	if (PED::_0x3E802F11FBE27674(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x3e80... true");
	}
	if (PED::_0xF41B5D290C99A3D6(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x3e80... true");
	}
	if (PED::_0xE0D36E5D9E99CC21(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0xE0D... true");
	}

	if (PED::_0x604E810189EE3A59(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x604... true");
	}

	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		if (VEHICLE::_0xAE3FEE8709B39DCB(veh))
		{
			set_status_text_centre_screen("0xAE3F... true");
		}
		else if (VEHICLE::_0x26C10ECBDA5D043B(veh))
		{
			set_status_text_centre_screen("0x26C10... true");
		}
		else if (VEHICLE::_0xB5CC40FBCB586380(veh)) //possibly is_vehicle_strong
		{
			set_status_text_centre_screen("0xB5CCC... true");
		}
		else if (VEHICLE::_0x62CA17B74C435651(veh))
		{
			set_status_text_centre_screen("0x62CA1... true");
		}
		else if (VEHICLE::_0x634148744F385576(veh))
		{
			set_status_text_centre_screen("0x634... true");
		}
		else if (VEHICLE::_0x1033371FC8E842A7(veh))
		{
			set_status_text_centre_screen("0x10333... true");
		}
		else if (!VEHICLE::_0x9A83F5F9963775EF(veh))
		{
			set_status_text_centre_screen("0x9a8... false");
		}
		else if (VEHICLE::_0xAE3FEE8709B39DCB(veh))
		{
			set_status_text_centre_screen("0xAE3Fee... true");
		}
		else if (VEHICLE::_0x291E373D483E7EE7(veh))
		{
			set_status_text_centre_screen("0x291... true");
		}
	}
}