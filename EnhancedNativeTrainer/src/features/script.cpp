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
#include "hotkeys.h"
#include "propplacement.h"
#include "../version.h"
#include "../utils.h"
#include "../ui_support/file_dialog.h"

#include <set>
#include <iostream>
#include <vector>

#pragma warning(disable : 4244 4305) // double <-> float conversions

int last_player_slot_seen = 0;

int game_frame_num = 0;

bool everInitialised = false;

ENTDatabase* database = NULL;

bool onlineWarningShown = false;

//std::mutex db_mutex;

// features
bool featurePlayerInvincible = false;
bool featurePlayerInvincibleUpdated = false;
bool featurePlayerIgnoredByPolice = false;
bool featurePlayerIgnoredByPoliceUpdated = false;
bool featurePlayerIgnoredByAll = false;
bool featurePlayerIgnoredByAllUpdated = false;
bool featurePlayerUnlimitedAbility = false;
bool featurePlayerNoNoise = false;
bool featurePlayerNoNoiseUpdated = false;
bool featurePlayerFastSwim = false;
bool featurePlayerFastSwimUpdated = false;
bool featurePlayerFastRun = false;
bool featurePlayerFastRunUpdated = false;
bool featurePlayerSuperJump = false;
bool featurePlayerInvisible = false;
bool featurePlayerInvisibleUpdated = false;
bool featurePlayerDrunk = false;
bool featurePlayerDrunkUpdated = false;
bool featureNightVision = false;
bool featureNightVisionUpdated = false;
bool featureThermalVision = false;
bool featureThermalVisionUpdated = false;
bool featureWantedLevelFrozen = false;
bool featureWantedLevelFrozenUpdated = false;

bool featureNoRagdoll = false; 
bool featureNoRagdollUpdated = false;

int  frozenWantedLevel = 0;

// player model control, switching on normal ped model when needed	

char* player_models[] = { "player_zero", "player_one", "player_two" };

char* mplayer_models[] = { "mp_f_freemode_01", "mp_m_freemode_01" };

const char* CLIPSET_DRUNK = "move_m@drunk@verydrunk";

const std::vector<std::string> GRAVITY_CAPTIONS{ "Minimum", "0.1x", "0.5x", "0.75x", "1x (Normal)" };
const std::vector<float> GRAVITY_VALUES{ 0.0f, 0.1f, 0.5f, 0.75f, 1.0f };

std::set<Ped> lastSeenPeds;

void check_player_model()
{
	/*
	std::stringstream ss;
	ss << "PID: " << *gp;
	set_status_text_centre_screen(ss.str());
	*/

	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed))
	{
		return;
	}

	//find out whether we're a default player model

		bool found = false;
		Hash playerModel = ENTITY::GET_ENTITY_MODEL(playerPed);
	int playerSlot = 0;

		for each (char* model  in player_models)
		{
			if (GAMEPLAY::GET_HASH_KEY(model) == playerModel)
			{
			last_player_slot_seen = playerSlot;
				found = true;
				break;
			}
		playerSlot++;
		}

		if (!found && NETWORK::NETWORK_IS_GAME_IN_PROGRESS())
		{
			for each (char* model  in mplayer_models)
			{
				if (GAMEPLAY::GET_HASH_KEY(model) == playerModel)
				{
				last_player_slot_seen = playerSlot;
					found = true;
					break;
				}
			playerSlot++;
			}
		}

	if (ENTITY::IS_ENTITY_DEAD(playerPed) && is_player_reset_on_death())
	{
		if (!found)
		{
			set_status_text("Resetting death state because a custom skin was used");
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();

			int spPlayerCount = sizeof(player_models) / sizeof(player_models[0]);
			if (last_player_slot_seen < spPlayerCount)
			{
				applyChosenSkin(player_models[last_player_slot_seen]);
			}
			else
			{
				applyChosenSkin(mplayer_models[last_player_slot_seen - spPlayerCount]);
			}
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
	if (!is_fivem() && NETWORK::NETWORK_IS_GAME_IN_PROGRESS())
	{
		if (!onlineWarningShown)
		{
			set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT ~HUD_COLOUR_WHITE~is not for use online");
			onlineWarningShown = true;
		}
		WAIT(0);
		return;
	}
	else
	{
		onlineWarningShown = false;
	}

	/*
	GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("shop_controller");
	
	std::ostringstream ss;
	ss << "Scripts disabled";
	set_status_text_centre_screen(ss.str());*/

	/*
	std::ostringstream perfSS;
	perfSS << "Calls this frame: " << get_calls_per_frame() << " in " << get_ticks_since_last_frame() << "ms";
	write_text_to_log_file(perfSS.str());
	*/

	/*int* gp = reinterpret_cast<int *>(getGlobalPtr(0x1801D9));
	*gp = 1;

	gp = reinterpret_cast<int *>(getGlobalPtr(0x187385 + 0x10F18));
	*gp = 1;

	gp = reinterpret_cast<int *>(getGlobalPtr(0x250FDB + 0xf158D));
	*gp = 0;

	int* gp = reinterpret_cast<int *>(getGlobalPtr(0x42CA + 0x9));
	*gp = 0xFF;
	*/

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

	check_for_hotkey_presses();

	if (is_menu_showing() || is_in_airbrake_mode() || is_in_prop_placement_mode())
	{
		if (is_input_blocked_in_menu() || is_in_airbrake_mode() || is_in_prop_placement_mode())
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

	if (is_in_airbrake_mode() || is_in_prop_placement_mode())
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

	update_teleport_features();

	check_player_model();

	maintain_bodyguards();

	update_props_pending_dialogs();

	debug_native_investigation();

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

	if (featurePlayerInvincible && bPlayerExists)
	{
		PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
	}

	if (featureWantedLevelFrozen)
	{
		if (featureWantedLevelFrozenUpdated)
		{
			frozenWantedLevel = PLAYER::GET_PLAYER_WANTED_LEVEL(player);
			featureWantedLevelFrozenUpdated = false;
		}
		PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
	}

	if (featureWantedLevelFrozenUpdated && !featureWantedLevelFrozen)
	{
		PLAYER::SET_MAX_WANTED_LEVEL(5);
		featureWantedLevelFrozenUpdated = false;
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
		update_nearby_peds(playerPed, 50);

		if (bPlayerExists)
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, true);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, false);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, true);
			if (game_frame_num % 5 == 0)
			{
				set_all_nearby_peds_to_calm();
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
			PLAYER::SPECIAL_ABILITY_FILL_METER(player, 1);
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
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.0);
		featurePlayerFastSwimUpdated = false;
	}
	if (featurePlayerFastSwim)
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.49);

	// player fast run
	if (featurePlayerFastRunUpdated)
	{
		if (bPlayerExists && !featurePlayerFastRun)
			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.0);
		featurePlayerFastRunUpdated = false;
	}
	if (featurePlayerFastRun)
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49);

	// player super jump
	if (featurePlayerSuperJump)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
	}

	//No Radgoll

	if (featureNoRagdoll)
	{
		if (bPlayerExists)
		{
			PED::SET_PED_CAN_RAGDOLL(playerPed, 0);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, 0);
		}
	}
	else if (featureNoRagdollUpdated)
	{
		if (bPlayerExists)
		{
			PED::SET_PED_CAN_RAGDOLL(playerPed, 1);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, 1);
		}
		featureNoRagdollUpdated = false;
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
				make_periodic_feature_call();
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

	if (featureNightVisionUpdated)
	{
		GRAPHICS::SET_NIGHTVISION(featureNightVision);
		featureNightVisionUpdated = false;
	}

	if (featureThermalVisionUpdated)
	{
		GRAPHICS::SET_SEETHROUGH(featureThermalVision);
		featureThermalVisionUpdated = false;
	}

	update_weapon_features(bPlayerExists, player);

	update_aimbot_esp_features();

	update_vehicle_features(bPlayerExists, playerPed);

	update_misc_features(bPlayerExists, playerPed);

	update_time_features(player);

	update_bodyguard_features();

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

bool getFrozenWantedFeature(){ return featureWantedLevelFrozen; }

void updateFrozenWantedFeature(int level)
{
	frozenWantedLevel = level; 
	featureWantedLevelFrozenUpdated = true;
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
		heal_player();
		break;
	case 18:
		process_anims_menu_top();
		break;
	default:
		break;
	}
	return false;
}

void process_player_menu()
{
	const int lineCount = 19;
	
	std::string caption = "Player Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Player Appearance", NULL, NULL, false },
		{ "Heal Player", NULL, NULL, true },
		{ "Add Cash", NULL, NULL, true, CASH },
		{ "Wanted Level", NULL, NULL, true, WANTED },
		{ "Freeze Wanted Level", &featureWantedLevelFrozen, &featureWantedLevelFrozenUpdated, true },
		{ "Invincible", &featurePlayerInvincible, &featurePlayerInvincibleUpdated, true },
		{ "Police Ignore You", &featurePlayerIgnoredByPolice, &featurePlayerIgnoredByPoliceUpdated, true },
		{ "Everyone Ignores You", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated, true },
		{ "Unlimited Ability", &featurePlayerUnlimitedAbility, NULL, true },
		{ "Noiseless", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated, true },
		{ "Fast Swim", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated, true },
		{ "Fast Run", &featurePlayerFastRun, &featurePlayerFastRunUpdated, true },
		{ "Super Jump", &featurePlayerSuperJump, NULL, true },
		{ "No Ragdoll", &featureNoRagdoll, &featureNoRagdollUpdated, true }, //could this be the toggle value?
		{ "Invisibility", &featurePlayerInvisible, &featurePlayerInvisibleUpdated, true },
		{ "Drunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated, true },
		{ "Night Vision", &featureNightVision, &featureNightVisionUpdated, true },
		{ "Thermal Vision", &featureThermalVision, &featureThermalVisionUpdated, true },
		{ "Animations", NULL, NULL, false },
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexPlayer, caption, onconfirm_player_menu);
}


//==================
// MAIN MENU
//==================

int activeLineIndexMain = 0;

bool onconfirm_main_menu(MenuItem<int> choice)
{
	switch (choice.value)
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
		process_aimbot_esp_menu();
		break;
	case 4:
		process_bodyguard_menu();
		break;
	case 5:
		process_veh_menu();
		break;
	case 6:
		process_world_menu();
		break;
	case 7:
		process_time_menu();
		break;
	case 8:
		process_props_menu();
		break;
	case 9:
		process_misc_menu();
		break;
	case 10:
		reset_globals();
		break;
	}
	return false;
}

void process_main_menu()
{
	std::ostringstream captionSS;
	captionSS << "~HUD_COLOUR_MENU_YELLOW~Enhanced ~HUD_COLOUR_WHITE~Native Trainer ~HUD_COLOUR_GREY~Update ";
	captionSS << VERSION_STRING;

	std::vector<MenuItem<int>*> menuItems;

	int i = 0;
	MenuItem<int> *item;

	item = new MenuItem<int>();
	item->caption = "Player";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Locations";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Weapons";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Aimbot ESP";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Bodyguards";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Vehicles";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "World";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Time";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Objects";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Miscellaneous";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Reset All Settings";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	MenuParameters<int> params(menuItems, captionSS.str());
	params.menuSelectionPtr = &activeLineIndexMain;
	params.onConfirmation = onconfirm_main_menu;
	params.sanitiseHeaderText = false;

	draw_generic_menu<int>(params);
}

void reset_globals()
{
	reset_skin_globals();

	reset_vehicle_globals();

	reset_teleporter_globals();

	reset_weapon_globals();

	reset_aimbot_globals();

	reset_world_globals();

	reset_misc_globals();

	reset_prop_globals();

	activeLineIndexMain =
		activeLineIndexPlayer =
		activeLineIndexWantedFreeze =
		frozenWantedLevel = 0;

	featurePlayerDrunk =
		featurePlayerInvincible =
		featurePlayerIgnoredByPolice =
	featurePlayerIgnoredByAll =
		featurePlayerUnlimitedAbility =
		featurePlayerNoNoise =
		featurePlayerFastSwim =
		featurePlayerFastRun =
		featurePlayerSuperJump =
		featurePlayerInvisible =
	featureNightVision =
	featureThermalVision =

	featureNoRagdoll = 

		featureWantedLevelFrozen = false;

	featurePlayerInvincibleUpdated =
	featurePlayerIgnoredByPoliceUpdated =
	featurePlayerIgnoredByAllUpdated =
	featurePlayerNoNoiseUpdated =
	featurePlayerFastSwimUpdated =
	featurePlayerFastRunUpdated =
	featurePlayerDrunkUpdated =
	featureNightVisionUpdated =
	featureThermalVisionUpdated =
		featurePlayerInvisibleUpdated =

		featureNoRagdollUpdated = 
		featureWantedLevelFrozenUpdated = true;

	set_status_text("All settings reset to defaults");

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

	init_vehicle_feature();

	write_text_to_log_file("Loaded settings OK");

	// this creates a new locale based on the current application default
	// (which is either the one given on startup, but can be overriden with
	// std::locale::global) - then extends it with an extra facet that 
	// controls numeric output.
	std::locale comma_locale(std::locale(), new comma_numpunct());

	// tell cout to use our new locale.
	std::cout.imbue(comma_locale);
	
	set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT~HUD_COLOUR_WHITE~ is ready!");

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

	auto hFile = CreateFileW(get_storage_dir_path("ENT-minidump.dmp"), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
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
	set_status_text("Whoops, ENT crashed!");

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

		set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT~HUD_COLOUR_WHITE~ is initializing...");

		clear_log_file();

		write_text_to_log_file("Trying to init storage");
		init_storage();
		write_text_to_log_file("Init storage complete");

		write_text_to_log_file("Trying to init XINPUT");
		init_xinput();
		write_text_to_log_file("Init XINPUT complete");

		database = new ENTDatabase();
		if (!database->open())
		{
			write_text_to_log_file("Failed to open database");
			set_status_text("ENT couldn't open the database - exiting");
			database = NULL;
			return;
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
#ifdef _DEBUG
	__try
	{
#endif

	write_text_to_log_file("ScriptTidyUp called");

	setGameInputToEnabled(true, true);
	setAirbrakeRelatedInputToBlocked(false, true);

	cleanup_script();
	write_text_to_log_file("Cleaned up script");
	WAIT(0);
	cleanup_props();
	write_text_to_log_file("Cleaned up props");
	WAIT(0);
	cleanup_anims();
	write_text_to_log_file("Cleaned up anims");

	save_settings();
	write_text_to_log_file("Saved settings");

	end_xinput();
	write_text_to_log_file("XInput terminated");

	if (database != NULL)
	{
		database->close();
		delete database;
		write_text_to_log_file("Database killed");
	}

	write_text_to_log_file("ScriptTidyUp done");
#ifdef _DEBUG
	}
	__except (filterException(GetExceptionCode(), GetExceptionInformation()))
	{

	}
#endif
}

void update_nearby_peds(Ped playerPed, int count)
{
	const int numElements = count;
	const int arrSize = numElements * 2 + 2;

	Ped *peds = new Ped[arrSize];
	peds[0] = numElements;
	int found = PED::GET_PED_NEARBY_PEDS(playerPed, peds, -1);

	for (int i = 0; i < found; i++)
	{
		int offsettedID = i * 2 + 2;

		if (!ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
		{
			continue;
		}

		Ped xped = peds[offsettedID];

		bool inSet = lastSeenPeds.find(xped) != lastSeenPeds.end();
		if (!inSet)
		{
			lastSeenPeds.insert(xped);
		}
	}

	std::set<Ped>::iterator it;
	for (it = lastSeenPeds.begin(); it != lastSeenPeds.end();)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(*it))
		{
			lastSeenPeds.erase(it++);
		}
		else
		{
			++it;
		}
	}

	delete peds;
}

void set_all_nearby_peds_to_calm()
{
	for each (Ped xped in lastSeenPeds)
	{
		// Only calm down peds if they're NOT in our group (keeps our bodyguards from chilling out and being lazy)
		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(xped, true);
			PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);
			PED::SET_PED_COMBAT_ATTRIBUTES(xped, 17, 1);

			//this commented-out code lives here because it will be used for something else in future

			/*
			PED::SET_PED_DIES_WHEN_INJURED(xped, false);
			PED::SET_PED_MAX_HEALTH(xped, 10000);
			ENTITY::SET_ENTITY_HEALTH(xped, 10000);
			PED::SET_PED_SUFFERS_CRITICAL_HITS(xped, false);
			WEAPON::GIVE_WEAPON_TO_PED(xped, GAMEPLAY::GET_HASH_KEY("WEAPON_MG"), 1000, true, true);

			PED::SET_PED_COMBAT_ABILITY(xped, 1);
			//PED::SET_PED_
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(xped, false);

			PED::SET_PED_AS_ENEMY(xped, true);
			*/
		}
	}
}

void add_player_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerInvincible", &featurePlayerInvincible, &featurePlayerInvincibleUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWantedLevelFrozen", &featureWantedLevelFrozen, &featureWantedLevelFrozenUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerIgnoredByPolice", &featurePlayerIgnoredByPolice, &featurePlayerIgnoredByPoliceUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerIgnoredByAll", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerUnlimitedAbility", &featurePlayerUnlimitedAbility });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerNoNoise", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerFastSwim", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerFastRun", &featurePlayerFastRun, &featurePlayerFastRunUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerSuperJump", &featurePlayerSuperJump });
	results->push_back(FeatureEnabledLocalDefinition{ "featureNoRagdoll", &featureNoRagdoll, &featureNoRagdollUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerInvisible", &featurePlayerInvisible, &featurePlayerInvisibleUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerDrunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureNightVision", &featureNightVision, &featureNightVisionUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureThermalVision", &featureThermalVision, &featureThermalVisionUpdated });
}

std::vector<FeatureEnabledLocalDefinition> get_feature_enablements()
{
	std::vector<FeatureEnabledLocalDefinition> results;

	add_player_feature_enablements(&results);

	add_world_feature_enablements(&results);

	add_time_feature_enablements(&results);

	add_misc_feature_enablements(&results);

	add_props_feature_enablements(&results);

	add_vehicle_feature_enablements(&results);

	add_weapon_feature_enablements(&results);

	add_bodyguards_feature_enablements(&results);

	return results;
}

std::vector<StringPairSettingDBRow> get_generic_settings()
{
	std::vector<StringPairSettingDBRow> settings;
	add_time_generic_settings(&settings);
	add_world_generic_settings(&settings);
	add_vehicle_generic_settings(&settings);
	add_misc_generic_settings(&settings);
	add_hotkey_generic_settings(&settings);
	add_props_generic_settings(&settings);
	add_weapons_generic_settings(&settings);
	add_aimbot_esp_generic_settings(&settings);
	add_bodyguards_generic_settings(&settings);
	add_skin_generic_settings(&settings);

	settings.push_back(StringPairSettingDBRow{ "frozenWantedLevel", std::to_string(frozenWantedLevel) });

	return settings;
}

void handle_generic_settings(std::vector<StringPairSettingDBRow> settings)
{
	for (int i = 0; i < settings.size(); i++)
	{
		StringPairSettingDBRow setting = settings.at(i);
		if (setting.name.compare("frozenWantedLevel") == 0)
		{
			frozenWantedLevel = stoi(setting.value);
		}
	}

	//pass to anyone else, vehicles, weapons etc

	handle_generic_settings_time(&settings);

	handle_generic_settings_misc(&settings);

	handle_generic_settings_vehicle(&settings);

	handle_generic_settings_world(&settings);

	handle_generic_settings_hotkey(&settings);

	handle_generic_settings_props(&settings);

	handle_generic_settings_weapons(&settings);

	handle_generic_settings_aimbot_esp(&settings);

	handle_generic_settings_bodyguards(&settings);

	handle_generic_settings_skin(&settings);
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

	if (database != NULL)
	{
		write_text_to_log_file("Actually saving");
		database->store_setting_pairs(get_generic_settings());
		database->store_feature_enabled_pairs(get_feature_enablements());
		write_text_to_log_file("Save flag released");
	}
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
	WCHAR* folder = get_storage_dir_path();
	write_text_to_log_file("Trying to create storage folder");

	std::wstring ws1(folder);
	std::string folderSS1(ws1.begin(), ws1.end());

	write_text_to_log_file(folderSS1);
	if (CreateDirectoryW(folder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		write_text_to_log_file("Storage dir created or exists");
	}
	else
	{
		write_text_to_log_file("Couldn't create storage dir");
	}
	delete folder;

	WCHAR* folder2 = get_temp_dir_path();
	std::wstring ws2(folder2);
	std::string folderSS2(ws2.begin(), ws2.end());

	write_text_to_log_file("Trying to create temp folder");
	write_text_to_log_file(folderSS2);
	if (CreateDirectoryW(folder2, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		write_text_to_log_file("Temp dir created or exists");
	}
	else
	{
		write_text_to_log_file("Couldn't create temp dir");
	}
	delete folder2;
}

WCHAR* get_temp_dir_path()
{
	WCHAR s[MAX_PATH];
	GetTempPathW(MAX_PATH, s);
	
	WCHAR combined[MAX_PATH];
	PathCombineW(combined, s, L"GTAV Enhanced Native Trainer");

	WCHAR *result = new WCHAR[MAX_PATH];

	wcsncpy(result, combined, MAX_PATH);

	std::wstring ws(result);
	std::string folderSS(ws.begin(), ws.end());

	return result;
}

WCHAR* get_temp_dir_path(char* file)
{
	WCHAR *output = new WCHAR[MAX_PATH];

	WCHAR* folder = get_temp_dir_path();

	WCHAR* wfile = new WCHAR[MAX_PATH];
	mbstowcs(wfile, file, MAX_PATH);

	PathCombineW(output, folder, wfile);

	delete folder;
	delete wfile;

	return output;
}

WCHAR* get_storage_dir_path()
{
	PWSTR localAppData;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &localAppData);

	WCHAR combined[MAX_PATH];
	PathCombineW(combined, localAppData, L"GTAV Enhanced Native Trainer");

	WCHAR *result = new WCHAR[MAX_PATH];

	wcsncpy(result, combined, MAX_PATH);

	CoTaskMemFree(localAppData);

	return result;
}

WCHAR* get_storage_dir_path(char* file)
{
	WCHAR *output = new WCHAR[MAX_PATH];
	WCHAR* folder = get_storage_dir_path();

	WCHAR* wfile = new WCHAR[MAX_PATH];
	mbstowcs(wfile, file, MAX_PATH);

	PathCombineW(output, folder, wfile);

	delete folder;
	delete wfile;

	return output;
}

ENTDatabase* get_database()
{
	return database;
}

struct GraphicsTest
{
	void(*function)(BOOL);
	bool state;
};

bool allGraphicsOn = false;

std::vector<GraphicsTest> graphicsTests = {
	{ GRAPHICS::_0x1BBC135A4D25EDDE, false },
	{ GRAPHICS::_0x23BA6B0C2AD7B0D3, false },
	{ GRAPHICS::_0x22A249A53034450A, false },
	{ GRAPHICS::_0xDC459CFA0CCE245B, false },
	{ GRAPHICS::_0xC6372ECD45D73BCD, false },
	//{ GRAPHICS::_0x61BB1D9B3A95D802, false },
	{ GRAPHICS::_0xEF398BEEE4EF45F9, false },
	//{ GRAPHICS::_0x80ECBC0C856D3B0B, false }, //off = far shadows

	{ GRAPHICS::_0x25FC3E33A31AD0C9, false },
	{ GRAPHICS::_0x6DDBF9DFFC4AC080, false },
	//{ GRAPHICS::_0xD39D13C9FEBF0511, false }, //off = detailed shadows

	{ GRAPHICS::_0x0AE73D8DF3A762B2, false },
	{ GRAPHICS::_0xA51C4B86B71652AE, false },
	{ GRAPHICS::_0xC0416B061F2B7E5E, false },

	{ GRAPHICS::_0x06F761EA47C1D3ED, false },
	{ GRAPHICS::_0xE63D7C6EECECB66B, false },
	//{ GRAPHICS::_0x7AC24EAB6D74118D, false },
	{ GRAPHICS::_0x8CDE909A0370BB3A, false },
	{ GRAPHICS::_0x8CDE909A0370BB3A, false },

	{ GRAPHICS::ENABLE_ALIEN_BLOOD_VFX, false },
	{ GRAPHICS::_0xCA4AE345A153D573, false },
	{ GRAPHICS::_0x9B079E5221D984D3, false },
	{ GRAPHICS::_0xA46B73FAA3460AE1, false },

	{ GRAPHICS::_0x0E4299C549F0D1F1, false },
	{ GRAPHICS::_0x02369D5C8A51FDCF, false },
	{ GRAPHICS::_0x03300B57FCAC6DDB, false },
	{ GRAPHICS::_SET_FORCE_PED_FOOTSTEPS_TRACKS, false },
	{ GRAPHICS::_SET_FORCE_VEHICLE_TRAILS, false },

	{ GRAPHICS::_0x74C180030FDE4B69, false },
	//{ GRAPHICS::_0xD1C55B110E4DF534, false },
	{ GRAPHICS::_0x108BE26959A9D9BB, false },
	{ GRAPHICS::_0xA356990E161C9E65, false },
	//{ GRAPHICS::_SET_BLACKOUT, false }
};

bool get_graphics_test(std::vector<int> extras)
{
	int choice = extras.at(0);
	GraphicsTest* gt = &graphicsTests.at(choice);
	return gt->state;
}

void set_graphics_test(bool applied, std::vector<int> extras)
{
	int choice = extras.at(0);

	GraphicsTest* gt = &graphicsTests.at(choice);
	gt->state = !gt->state;
	gt->function(gt->state);

	//std::ostringstream ss;
	//ss << "Item " << choice << " set to " << gt->state << " using " << gt->function;
	//set_status_text_centre_screen(ss.str());
}

void set_all_graphics_test(bool applied, std::vector<int> extras)
{
	allGraphicsOn = applied;
	for (int i = 0; i < graphicsTests.size(); i++)
	{
		GraphicsTest* gt = &graphicsTests.at(i);
		gt->state = applied;
		gt->function(gt->state);
	}
}

bool get_all_graphics_test(std::vector<int> extras)
{
	return allGraphicsOn;
}

bool onconfirm_testmenu(MenuItem<int> choice)
{
	Ped ped = PLAYER::PLAYER_PED_ID();
	Hash hash = GAMEPLAY::GET_HASH_KEY("WEAPON_NIGHTSTICK");
	DWORD start = GetTickCount();
	for (int i = 0; i < 100000; i++)
	{
		Hash hash;
		WEAPON::GET_CURRENT_PED_WEAPON(ped, &hash, true);
	}
	DWORD end = GetTickCount();
	std::ostringstream ss;
	ss << "1000 weapon calls took " << (end - start) << "msec";
	set_status_text_centre_screen(ss.str());
	return false;
}

void process_test_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *mitem = new MenuItem<int>();
	mitem->caption = "SCPW Performance Test";
	mitem->isLeaf = false;
	mitem->value = -2;
	menuItems.push_back(mitem);

	FunctionDrivenToggleMenuItem<int> *item = new FunctionDrivenToggleMenuItem<int>();
	item->getter_call = get_all_graphics_test;
	item->setter_call = set_all_graphics_test;
	item->caption = "All Options";
	item->isLeaf = false;
	item->value = -1;
	menuItems.push_back(item);

	for (int i = 0; i < graphicsTests.size(); i++)
	{
		FunctionDrivenToggleMenuItem<int> *item = new FunctionDrivenToggleMenuItem<int>();
		item->getter_call = get_graphics_test;
		item->setter_call = set_graphics_test;
		item->extra_arguments.push_back(i);
		item->isLeaf = false;
		item->value = i;

		std::ostringstream ss;
		ss << "Test #" << i;

		item->caption = ss.str();
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, 0, "Test Funcs", onconfirm_testmenu, NULL, NULL, skin_save_menu_interrupt);

}

void debug_native_investigation()
{
	/*
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			int intCol, dashCol;
			VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &intCol);
			VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &dashCol);
			std::ostringstream ss;
			ss << "Interior: " << intCol << "; Dash: "<< dashCol;
			set_status_text_centre_screen(ss.str());
		}
	}*/

	/*
	bool online = NETWORK::NETWORK_IS_GAME_IN_PROGRESS() == 1;

	std::ostringstream ss;
	ss << "Online: " << (online ? "Yes" : "No");
	set_status_text_centre_screen(ss.str());*/
	/*
	for (int i = 0; i < graphicsTests.size(); i++)
	{
		GraphicsTest* gt = &graphicsTests.at(i);
		//gt->state = applied;
		gt->function(gt->state);
	}
	*/

	/*
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
	if (PED::_0xE0D36E5D9E99CC21(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0xE0D... true");
	}
	if (PED::_0x604E810189EE3A59(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x604... true");
	}

	if (PED::_0x3E802F11FBE27674(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x3e80... true");
	}
	if (PED::_0xF41B5D290C99A3D6(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x3e80... true");
	}

	if (PED::_0x604E810189EE3A59(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x604... true");
	}
	*/

	/*
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

		int primary, secondary;
		VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);

		std::ostringstream ss;
		ss << "Primary: " << primary << ", sec: " << secondary;
		set_status_text_centre_screen(ss.str());*/

		/*if (!VEHICLE::_0x8D474C8FAEFF6CDE(veh))
		{
			set_status_text_centre_screen("0x8D474... false");
		}
		else if (VEHICLE::_0xAE3FEE8709B39DCB(veh))
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
		else if (VEHICLE::_0x4198AB0022B15F87(veh))
		{
			set_status_text_centre_screen("0x419... true");
		}
		else if (VEHICLE::_0x755D6D5267CBBD7E(veh))
		{
			set_status_text_centre_screen("0x755... true");
		}
		else if (VEHICLE::_0x5991A01434CE9677(veh))
		{
			set_status_text_centre_screen("0x599... true");
		}
		else if (VEHICLE::_0x1821D91AD4B56108(veh))
		{
			set_status_text_centre_screen("0x182... true");
		}
		else if (VEHICLE::_0x6E08BF5B3722BAC9(veh))
		{
			set_status_text_centre_screen("0x6E0... true");
		}
		else if (VEHICLE::_0xD4C4642CB7F50B5D(veh))
		{
			set_status_text_centre_screen("0xD4C... true");
		}
	}*/
}

void heal_player()
{
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

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
	set_status_text("Player healed");
}

void toggle_invisibility()
{
	featurePlayerInvisible = !featurePlayerInvisible;
	if (featurePlayerInvisible)
	{
		set_status_text("Player invisible");
	}
	else
	{
		set_status_text("Player no longer invisible");
	}
	featurePlayerInvisibleUpdated = true;
}

void reset_wanted_level()
{
	PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
	set_status_text("Wanted level cleared");
}

int get_frame_number()
{
	return game_frame_num;
}

void toggle_thermal_vision()
{
	featureThermalVision = !featureThermalVision;
	featureThermalVisionUpdated = true;
}

void toggle_night_vision()
{
	featureNightVision = !featureNightVision;
	featureNightVisionUpdated = true;
}

void cleanup_script()
{
	lastSeenPeds.clear();
}
