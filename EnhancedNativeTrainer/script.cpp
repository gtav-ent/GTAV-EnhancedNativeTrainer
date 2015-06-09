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

#include "io.h"
#include "config_io.h"
#include "menu_functions.h"
#include "skins.h"
#include "script.h"
#include "database.h"
#include "debuglog.h"
#include "vehicles.h"
#include "teleportation.h"
#include "airbrake.h"
#include "weapons.h"
#include "anims.h";
//#include "crash_handler.h"

#include <DbgHelp.h>
#include <ShlObj.h>
#include <windows.h>
#include <Shlwapi.h>

#include <string>
#include <sstream> 
#include <fstream>
#include <mutex>
#include <thread>

#include <ctime>
#include <cctype>
#include <vector>

#include <locale>
#include <iostream>
#include <iomanip>

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
bool featurePlayerRadio					=	false;
bool featurePlayerRadioUpdated			=	false;

bool featureRestrictedZones = true;

bool featureWorldMoonGravity			=	false;
bool featureWorldRandomCops				=	true;
bool featureWorldRandomTrains			=	true;
bool featureWorldRandomBoats			=	true;
bool featureWorldGarbageTrucks			=	true;

bool featureTimePaused					=	false;
bool featureTimePausedUpdated			=	false;
bool featureTimeSynced					=	false;
bool featureTimeSlow = false;
bool featureTimeSlowUpdated = false;

bool featureWeatherWind					=	false;
bool featureWeatherFreeze				=	false;
std::string lastWeather;
std::string lastWeatherName;

bool featureMiscLockRadio				=	false;
bool featureMiscHideHud					=	false;

bool featureWantedLevelFrozen			=	false;
int  frozenWantedLevel					=	0;

bool featurePlayerResetOnDeath = true;

// player model control, switching on normal ped model when needed	

LPCSTR player_models[] = { "player_zero", "player_one", "player_two" };

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

	PED::SET_CREATE_RANDOM_COPS(featureWorldRandomCops);

	update_centre_screen_status_text();

	update_vehicle_guns();

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

	/*
	//Wanted Level Frozen - prevents stars from disappearing
	if (featureWantedLevelFrozen)
	{
		if (bPlayerExists)
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		if (getFrozenWantedLvl() == 0){ featureWantedLevelFrozen = false; }
	}
	*/
	
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

	// Portable radio
	if (featurePlayerRadio || featurePlayerRadioUpdated)
	{
		if (featurePlayerRadio)
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		else
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
	}

	update_weapon_features(bPlayerExists, player);

	update_vehicle_features(bPlayerExists, playerPed);

	// time pause
	if (featureTimePausedUpdated)
	{
		TIME::PAUSE_CLOCK(featureTimePaused);
		featureTimePausedUpdated = false;
	}

	// time sync
	if (featureTimeSynced)
	{
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		TIME::SET_CLOCK_TIME(t.tm_hour, t.tm_min, t.tm_sec);
	}

	if (featureTimeSlow)
	{
		GAMEPLAY::SET_TIME_SCALE(0.5f);
	}
	else if (featureTimeSlowUpdated)
	{
		featureTimeSlowUpdated = false;
		GAMEPLAY::SET_TIME_SCALE(1.0f);
	}

	if (!featureRestrictedZones)
	{
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("restrictedareas");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_lossantosintl");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prison");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prisonvanbreak");
	}

	// hide hud
	if (featureMiscHideHud)
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();

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
		bool throughDoorPressed = IsKeyJustUp(get_config()->get_key_config()->key_hotkey_throughdoor);
		//bool disablePolicePressed = IsKeyJustUp(VK_OEM_6);
		if (throughDoorPressed)
		{
			moveThroughDoor();
			//set_status_text("Moved through door");
		}
		/*
		// Commented out until we deal with hotkeys properly, i.e at least have modifiers
		if (disablePolicePressed)
		{
			featurePlayerNeverWanted = !featurePlayerNeverWanted;
			std::string s = ((featurePlayerNeverWanted) ? "Enabled" : "Disabled");
			set_status_text("Never Wanted : " + s);
		}
		*/
	}
}

/*
LPCSTR pedModels[69][10] = {
	{"player_zero", "player_one", "player_two", "a_c_boar", "a_c_chimp", "a_c_cow", "a_c_coyote", "a_c_deer", "a_c_fish", "a_c_hen"},
	{ "a_c_cat_01", "a_c_chickenhawk", "a_c_cormorant", "a_c_crow", "a_c_dolphin", "a_c_humpback", "a_c_killerwhale", "a_c_pigeon", "a_c_seagull", "a_c_sharkhammer"},
	{"a_c_pig", "a_c_rat", "a_c_rhesus", "a_c_chop", "a_c_husky", "a_c_mtlion", "a_c_retriever", "a_c_sharktiger", "a_c_shepherd", "s_m_m_movalien_01"},
	{"a_f_m_beach_01", "a_f_m_bevhills_01", "a_f_m_bevhills_02", "a_f_m_bodybuild_01", "a_f_m_business_02", "a_f_m_downtown_01", "a_f_m_eastsa_01", "a_f_m_eastsa_02", "a_f_m_fatbla_01", "a_f_m_fatcult_01"},
	{"a_f_m_fatwhite_01", "a_f_m_ktown_01", "a_f_m_ktown_02", "a_f_m_prolhost_01", "a_f_m_salton_01", "a_f_m_skidrow_01", "a_f_m_soucentmc_01", "a_f_m_soucent_01", "a_f_m_soucent_02", "a_f_m_tourist_01"},
	{"a_f_m_trampbeac_01", "a_f_m_tramp_01", "a_f_o_genstreet_01", "a_f_o_indian_01", "a_f_o_ktown_01", "a_f_o_salton_01", "a_f_o_soucent_01", "a_f_o_soucent_02", "a_f_y_beach_01", "a_f_y_bevhills_01"},
	{"a_f_y_bevhills_02", "a_f_y_bevhills_03", "a_f_y_bevhills_04", "a_f_y_business_01", "a_f_y_business_02", "a_f_y_business_03", "a_f_y_business_04", "a_f_y_eastsa_01", "a_f_y_eastsa_02", "a_f_y_eastsa_03"},
	{"a_f_y_epsilon_01", "a_f_y_fitness_01", "a_f_y_fitness_02", "a_f_y_genhot_01", "a_f_y_golfer_01", "a_f_y_hiker_01", "a_f_y_hippie_01", "a_f_y_hipster_01", "a_f_y_hipster_02", "a_f_y_hipster_03"},
	{"a_f_y_hipster_04", "a_f_y_indian_01", "a_f_y_juggalo_01", "a_f_y_runner_01", "a_f_y_rurmeth_01", "a_f_y_scdressy_01", "a_f_y_skater_01", "a_f_y_soucent_01", "a_f_y_soucent_02", "a_f_y_soucent_03"},
	{"a_f_y_tennis_01", "a_f_y_topless_01", "a_f_y_tourist_01", "a_f_y_tourist_02", "a_f_y_vinewood_01", "a_f_y_vinewood_02", "a_f_y_vinewood_03", "a_f_y_vinewood_04", "a_f_y_yoga_01", "a_m_m_acult_01"},
	{"a_m_m_afriamer_01", "a_m_m_beach_01", "a_m_m_beach_02", "a_m_m_bevhills_01", "a_m_m_bevhills_02", "a_m_m_business_01", "a_m_m_eastsa_01", "a_m_m_eastsa_02", "a_m_m_farmer_01", "a_m_m_fatlatin_01"},
	{"a_m_m_genfat_01", "a_m_m_genfat_02", "a_m_m_golfer_01", "a_m_m_hasjew_01", "a_m_m_hillbilly_01", "a_m_m_hillbilly_02", "a_m_m_indian_01", "a_m_m_ktown_01", "a_m_m_malibu_01", "a_m_m_mexcntry_01"},
	{"a_m_m_mexlabor_01", "a_m_m_og_boss_01", "a_m_m_paparazzi_01", "a_m_m_polynesian_01", "a_m_m_prolhost_01", "a_m_m_rurmeth_01", "a_m_m_salton_01", "a_m_m_salton_02", "a_m_m_salton_03", "a_m_m_salton_04"},
	{"a_m_m_skater_01", "a_m_m_skidrow_01", "a_m_m_socenlat_01", "a_m_m_soucent_01", "a_m_m_soucent_02", "a_m_m_soucent_03", "a_m_m_soucent_04", "a_m_m_stlat_02", "a_m_m_tennis_01", "a_m_m_tourist_01"},
	{"a_m_m_trampbeac_01", "a_m_m_tramp_01", "a_m_m_tranvest_01", "a_m_m_tranvest_02", "a_m_o_acult_01", "a_m_o_acult_02", "a_m_o_beach_01", "a_m_o_genstreet_01", "a_m_o_ktown_01", "a_m_o_salton_01"},
	{"a_m_o_soucent_01", "a_m_o_soucent_02", "a_m_o_soucent_03", "a_m_o_tramp_01", "a_m_y_acult_01", "a_m_y_acult_02", "a_m_y_beachvesp_01", "a_m_y_beachvesp_02", "a_m_y_beach_01", "a_m_y_beach_02"},
	{"a_m_y_beach_03", "a_m_y_bevhills_01", "a_m_y_bevhills_02", "a_m_y_breakdance_01", "a_m_y_busicas_01", "a_m_y_business_01", "a_m_y_business_02", "a_m_y_business_03", "a_m_y_cyclist_01", "a_m_y_dhill_01"},
	{"a_m_y_downtown_01", "a_m_y_eastsa_01", "a_m_y_eastsa_02", "a_m_y_epsilon_01", "a_m_y_epsilon_02", "a_m_y_gay_01", "a_m_y_gay_02", "a_m_y_genstreet_01", "a_m_y_genstreet_02", "a_m_y_golfer_01"},
	{"a_m_y_hasjew_01", "a_m_y_hiker_01", "a_m_y_hippy_01", "a_m_y_hipster_01", "a_m_y_hipster_02", "a_m_y_hipster_03", "a_m_y_indian_01", "a_m_y_jetski_01", "a_m_y_juggalo_01", "a_m_y_ktown_01"},
	{"a_m_y_ktown_02", "a_m_y_latino_01", "a_m_y_methhead_01", "a_m_y_mexthug_01", "a_m_y_motox_01", "a_m_y_motox_02", "a_m_y_musclbeac_01", "a_m_y_musclbeac_02", "a_m_y_polynesian_01", "a_m_y_roadcyc_01"},
	{"a_m_y_runner_01", "a_m_y_runner_02", "a_m_y_salton_01", "a_m_y_skater_01", "a_m_y_skater_02", "a_m_y_soucent_01", "a_m_y_soucent_02", "a_m_y_soucent_03", "a_m_y_soucent_04", "a_m_y_stbla_01"},
	{"a_m_y_stbla_02", "a_m_y_stlat_01", "a_m_y_stwhi_01", "a_m_y_stwhi_02", "a_m_y_sunbathe_01", "a_m_y_surfer_01", "a_m_y_vindouche_01", "a_m_y_vinewood_01", "a_m_y_vinewood_02", "a_m_y_vinewood_03"},
	{"a_m_y_vinewood_04", "a_m_y_yoga_01", "u_m_y_proldriver_01", "u_m_y_rsranger_01", "u_m_y_sbike", "u_m_y_staggrm_01", "u_m_y_tattoo_01", "csb_abigail", "csb_anita", "csb_anton"},
	{"csb_ballasog", "csb_bride", "csb_burgerdrug", "csb_car3guy1", "csb_car3guy2", "csb_chef", "csb_chin_goon", "csb_cletus", "csb_cop", "csb_customer"},
	{"csb_denise_friend", "csb_fos_rep", "csb_g", "csb_groom", "csb_grove_str_dlr", "csb_hao", "csb_hugh", "csb_imran", "csb_janitor", "csb_maude"},
	{"csb_mweather", "csb_ortega", "csb_oscar", "csb_porndudes", "csb_porndudes_p", "csb_prologuedriver", "csb_prolsec", "csb_ramp_gang", "csb_ramp_hic", "csb_ramp_hipster"},
	{"csb_ramp_marine", "csb_ramp_mex", "csb_reporter", "csb_roccopelosi", "csb_screen_writer", "csb_stripper_01", "csb_stripper_02", "csb_tonya", "csb_trafficwarden", "cs_amandatownley"},
	{"cs_andreas", "cs_ashley", "cs_bankman", "cs_barry", "cs_barry_p", "cs_beverly", "cs_beverly_p", "cs_brad", "cs_bradcadaver", "cs_carbuyer"},
	{"cs_casey", "cs_chengsr", "cs_chrisformage", "cs_clay", "cs_dale", "cs_davenorton", "cs_debra", "cs_denise", "cs_devin", "cs_dom"},
	{"cs_dreyfuss", "cs_drfriedlander", "cs_fabien", "cs_fbisuit_01", "cs_floyd", "cs_guadalope", "cs_gurk", "cs_hunter", "cs_janet", "cs_jewelass"},
	{"cs_jimmyboston", "cs_jimmydisanto", "cs_joeminuteman", "cs_johnnyklebitz", "cs_josef", "cs_josh", "cs_lamardavis", "cs_lazlow", "cs_lestercrest", "cs_lifeinvad_01"},
	{"cs_magenta", "cs_manuel", "cs_marnie", "cs_martinmadrazo", "cs_maryann", "cs_michelle", "cs_milton", "cs_molly", "cs_movpremf_01", "cs_movpremmale"},
	{"cs_mrk", "cs_mrsphillips", "cs_mrs_thornhill", "cs_natalia", "cs_nervousron", "cs_nigel", "cs_old_man1a", "cs_old_man2", "cs_omega", "cs_orleans"},
	{"cs_paper", "cs_paper_p", "cs_patricia", "cs_priest", "cs_prolsec_02", "cs_russiandrunk", "cs_siemonyetarian", "cs_solomon", "cs_stevehains", "cs_stretch"},
	{"cs_tanisha", "cs_taocheng", "cs_taostranslator", "cs_tenniscoach", "cs_terry", "cs_tom", "cs_tomepsilon", "cs_tracydisanto", "cs_wade", "cs_zimbor"},
	{"g_f_y_ballas_01", "g_f_y_families_01", "g_f_y_lost_01", "g_f_y_vagos_01", "g_m_m_armboss_01", "g_m_m_armgoon_01", "g_m_m_armlieut_01", "g_m_m_chemwork_01", "g_m_m_chemwork_01_p", "g_m_m_chiboss_01"},
	{"g_m_m_chiboss_01_p", "g_m_m_chicold_01", "g_m_m_chicold_01_p", "g_m_m_chigoon_01", "g_m_m_chigoon_01_p", "g_m_m_chigoon_02", "g_m_m_korboss_01", "g_m_m_mexboss_01", "g_m_m_mexboss_02", "g_m_y_armgoon_02"},
	{"g_m_y_azteca_01", "g_m_y_ballaeast_01", "g_m_y_ballaorig_01", "g_m_y_ballasout_01", "g_m_y_famca_01", "g_m_y_famdnf_01", "g_m_y_famfor_01", "g_m_y_korean_01", "g_m_y_korean_02", "g_m_y_korlieut_01"},
	{"g_m_y_lost_01", "g_m_y_lost_02", "g_m_y_lost_03", "g_m_y_mexgang_01", "g_m_y_mexgoon_01", "g_m_y_mexgoon_02", "g_m_y_mexgoon_03", "g_m_y_mexgoon_03_p", "g_m_y_pologoon_01", "g_m_y_pologoon_01_p"},
	{"g_m_y_pologoon_02", "g_m_y_pologoon_02_p", "g_m_y_salvaboss_01", "g_m_y_salvagoon_01", "g_m_y_salvagoon_02", "g_m_y_salvagoon_03", "g_m_y_salvagoon_03_p", "g_m_y_strpunk_01", "g_m_y_strpunk_02", "hc_driver"},
	{"hc_gunman", "hc_hacker", "ig_abigail", "ig_amandatownley", "ig_andreas", "ig_ashley", "ig_ballasog", "ig_bankman", "ig_barry", "ig_barry_p"},
	{"ig_bestmen", "ig_beverly", "ig_beverly_p", "ig_brad", "ig_bride", "ig_car3guy1", "ig_car3guy2", "ig_casey", "ig_chef", "ig_chengsr"},
	{"ig_chrisformage", "ig_clay", "ig_claypain", "ig_cletus", "ig_dale", "ig_davenorton", "ig_denise", "ig_devin", "ig_dom", "ig_dreyfuss"},
	{"ig_drfriedlander", "ig_fabien", "ig_fbisuit_01", "ig_floyd", "ig_groom", "ig_hao", "ig_hunter", "ig_janet", "ig_jay_norris", "ig_jewelass"},
	{"ig_jimmyboston", "ig_jimmydisanto", "ig_joeminuteman", "ig_johnnyklebitz", "ig_josef", "ig_josh", "ig_kerrymcintosh", "ig_lamardavis", "ig_lazlow", "ig_lestercrest"},
	{"ig_lifeinvad_01", "ig_lifeinvad_02", "ig_magenta", "ig_manuel", "ig_marnie", "ig_maryann", "ig_maude", "ig_michelle", "ig_milton", "ig_molly"},
	{"ig_mrk", "ig_mrsphillips", "ig_mrs_thornhill", "ig_natalia", "ig_nervousron", "ig_nigel", "ig_old_man1a", "ig_old_man2", "ig_omega", "ig_oneil"},
	{"ig_orleans", "ig_ortega", "ig_paper", "ig_patricia", "ig_priest", "ig_prolsec_02", "ig_ramp_gang", "ig_ramp_hic", "ig_ramp_hipster", "ig_ramp_mex"},
	{"ig_roccopelosi", "ig_russiandrunk", "ig_screen_writer", "ig_siemonyetarian", "ig_solomon", "ig_stevehains", "ig_stretch", "ig_talina", "ig_tanisha", "ig_taocheng"},
	{"ig_taostranslator", "ig_taostranslator_p", "ig_tenniscoach", "ig_terry", "ig_tomepsilon", "ig_tonya", "ig_tracydisanto", "ig_trafficwarden", "ig_tylerdix", "ig_wade"},
	{"ig_zimbor", "mp_f_deadhooker", "mp_f_freemode_01", "mp_f_misty_01", "mp_f_stripperlite", "mp_g_m_pros_01", "mp_headtargets", "mp_m_claude_01", "mp_m_exarmy_01", "mp_m_famdd_01"},
	{"mp_m_fibsec_01", "mp_m_freemode_01", "mp_m_marston_01", "mp_m_niko_01", "mp_m_shopkeep_01", "mp_s_m_armoured_01", "", "", "", ""},
	{"", "s_f_m_fembarber", "s_f_m_maid_01", "s_f_m_shop_high", "s_f_m_sweatshop_01", "s_f_y_airhostess_01", "s_f_y_bartender_01", "s_f_y_baywatch_01", "s_f_y_cop_01", "s_f_y_factory_01"},
	{"s_f_y_hooker_01", "s_f_y_hooker_02", "s_f_y_hooker_03", "s_f_y_migrant_01", "s_f_y_movprem_01", "s_f_y_ranger_01", "s_f_y_scrubs_01", "s_f_y_sheriff_01", "s_f_y_shop_low", "s_f_y_shop_mid"},
	{"s_f_y_stripperlite", "s_f_y_stripper_01", "s_f_y_stripper_02", "s_f_y_sweatshop_01", "s_m_m_ammucountry", "s_m_m_armoured_01", "s_m_m_armoured_02", "s_m_m_autoshop_01", "s_m_m_autoshop_02", "s_m_m_bouncer_01"},
	{"s_m_m_chemsec_01", "s_m_m_ciasec_01", "s_m_m_cntrybar_01", "s_m_m_dockwork_01", "s_m_m_doctor_01", "s_m_m_fiboffice_01", "s_m_m_fiboffice_02", "s_m_m_gaffer_01", "s_m_m_gardener_01", "s_m_m_gentransport"},
	{"s_m_m_hairdress_01", "s_m_m_highsec_01", "s_m_m_highsec_02", "s_m_m_janitor", "s_m_m_lathandy_01", "s_m_m_lifeinvad_01", "s_m_m_linecook", "s_m_m_lsmetro_01", "s_m_m_mariachi_01", "s_m_m_marine_01"},
	{"s_m_m_marine_02", "s_m_m_migrant_01", "u_m_y_zombie_01", "s_m_m_movprem_01", "s_m_m_movspace_01", "s_m_m_paramedic_01", "s_m_m_pilot_01", "s_m_m_pilot_02", "s_m_m_postal_01", "s_m_m_postal_02"},
	{"s_m_m_prisguard_01", "s_m_m_scientist_01", "s_m_m_security_01", "s_m_m_snowcop_01", "s_m_m_strperf_01", "s_m_m_strpreach_01", "s_m_m_strvend_01", "s_m_m_trucker_01", "s_m_m_ups_01", "s_m_m_ups_02"},
	{"s_m_o_busker_01", "s_m_y_airworker", "s_m_y_ammucity_01", "s_m_y_armymech_01", "s_m_y_autopsy_01", "s_m_y_barman_01", "s_m_y_baywatch_01", "s_m_y_blackops_01", "s_m_y_blackops_02", "s_m_y_busboy_01"},
	{"s_m_y_chef_01", "s_m_y_clown_01", "s_m_y_construct_01", "s_m_y_construct_02", "s_m_y_cop_01", "s_m_y_dealer_01", "s_m_y_devinsec_01", "s_m_y_dockwork_01", "s_m_y_doorman_01", "s_m_y_dwservice_01"},
	{"s_m_y_dwservice_02", "s_m_y_factory_01", "s_m_y_fireman_01", "s_m_y_garbage", "s_m_y_grip_01", "s_m_y_hwaycop_01", "s_m_y_marine_01", "s_m_y_marine_02", "s_m_y_marine_03", "s_m_y_mime"},
	{"s_m_y_pestcont_01", "s_m_y_pilot_01", "s_m_y_prismuscl_01", "s_m_y_prisoner_01", "s_m_y_ranger_01", "s_m_y_robber_01", "s_m_y_sheriff_01", "s_m_y_shop_mask", "s_m_y_strvend_01", "s_m_y_swat_01"},
	{"s_m_y_uscg_01", "s_m_y_valet_01", "s_m_y_waiter_01", "s_m_y_winclean_01", "s_m_y_xmech_01", "s_m_y_xmech_02", "u_f_m_corpse_01", "u_f_m_miranda", "u_f_m_promourn_01", "u_f_o_moviestar"},
	{"u_f_o_prolhost_01", "u_f_y_bikerchic", "u_f_y_comjane", "u_f_y_corpse_01", "u_f_y_corpse_02", "u_f_y_hotposh_01", "u_f_y_jewelass_01", "u_f_y_mistress", "u_f_y_poppymich", "u_f_y_princess"},
	{"u_f_y_spyactress", "u_m_m_aldinapoli", "u_m_m_bankman", "u_m_m_bikehire_01", "u_m_m_fibarchitect", "u_m_m_filmdirector", "u_m_m_glenstank_01", "u_m_m_griff_01", "u_m_m_jesus_01", "u_m_m_jewelsec_01"},
	{"u_m_m_jewelthief", "u_m_m_markfost", "u_m_m_partytarget", "u_m_m_prolsec_01", "u_m_m_promourn_01", "u_m_m_rivalpap", "u_m_m_spyactor", "u_m_m_willyfist", "u_m_o_finguru_01", "u_m_o_taphillbilly"},
	{"u_m_o_tramp_01", "u_m_y_abner", "u_m_y_antonb", "u_m_y_babyd", "u_m_y_baygor", "u_m_y_burgerdrug_01", "u_m_y_chip", "u_m_y_cyclist_01", "u_m_y_fibmugger_01", "u_m_y_guido_01"},
	{"u_m_y_gunvend_01", "u_m_y_hippie_01", "u_m_y_imporage", "u_m_y_justin", "u_m_y_mani", "u_m_y_militarybum", "u_m_y_paparazzi", "u_m_y_party_01", "u_m_y_pogo_01", "u_m_y_prisoner_01"}
};*/

/*
LPCSTR pedModelNames[69][10] = {
	{"MICHAEL", "FRANKLIN", "TREVOR", "BOAR", "CHIMP", "COW", "COYOTE", "DEER", "FISH", "HEN"},
	{ "CAT", "HAWK", "CORMORANT", "CROW", "DOLPHIN", "HUMPBACK", "WHALE", "PIGEON", "SEAGULL", "SHARKHAMMER" },
	{"PIG", "RAT", "RHESUS", "CHOP", "HUSKY", "MTLION", "RETRIEVER", "SHARKTIGER", "SHEPHERD", "ALIEN"},
	{"BEACH", "BEVHILLS", "BEVHILLS", "BODYBUILD", "BUSINESS", "DOWNTOWN", "EASTSA", "EASTSA", "FATBLA", "FATCULT"},
	{"FATWHITE", "KTOWN", "KTOWN", "PROLHOST", "SALTON", "SKIDROW", "SOUCENTMC", "SOUCENT", "SOUCENT", "TOURIST"},
	{"TRAMPBEAC", "TRAMP", "GENSTREET", "INDIAN", "KTOWN", "SALTON", "SOUCENT", "SOUCENT", "BEACH", "BEVHILLS"},
	{"BEVHILLS", "BEVHILLS", "BEVHILLS", "BUSINESS", "BUSINESS", "BUSINESS", "BUSINESS", "EASTSA", "EASTSA", "EASTSA"},
	{"EPSILON", "FITNESS", "FITNESS", "GENHOT", "GOLFER", "HIKER", "HIPPIE", "HIPSTER", "HIPSTER", "HIPSTER"},
	{"HIPSTER", "INDIAN", "JUGGALO", "RUNNER", "RURMETH", "SCDRESSY", "SKATER", "SOUCENT", "SOUCENT", "SOUCENT"},
	{"TENNIS", "TOPLESS", "TOURIST", "TOURIST", "VINEWOOD", "VINEWOOD", "VINEWOOD", "VINEWOOD", "YOGA", "ACULT"},
	{"AFRIAMER", "BEACH", "BEACH", "BEVHILLS", "BEVHILLS", "BUSINESS", "EASTSA", "EASTSA", "FARMER", "FATLATIN"},
	{"GENFAT", "GENFAT", "GOLFER", "HASJEW", "HILLBILLY", "HILLBILLY", "INDIAN", "KTOWN", "MALIBU", "MEXCNTRY"},
	{"MEXLABOR", "OG_BOSS", "PAPARAZZI", "POLYNESIAN", "PROLHOST", "RURMETH", "SALTON", "SALTON", "SALTON", "SALTON"},
	{"SKATER", "SKIDROW", "SOCENLAT", "SOUCENT", "SOUCENT", "SOUCENT", "SOUCENT", "STLAT", "TENNIS", "TOURIST"},
	{"TRAMPBEAC", "TRAMP", "TRANVEST", "TRANVEST", "ACULT", "ACULT", "BEACH", "GENSTREET", "KTOWN", "SALTON"},
	{"SOUCENT", "SOUCENT", "SOUCENT", "TRAMP", "ACULT", "ACULT", "BEACHVESP", "BEACHVESP", "BEACH", "BEACH"},
	{"BEACH", "BEVHILLS", "BEVHILLS", "BREAKDANCE", "BUSICAS", "BUSINESS", "BUSINESS", "BUSINESS", "CYCLIST", "DHILL"},
	{"DOWNTOWN", "EASTSA", "EASTSA", "EPSILON", "EPSILON", "GAY", "GAY", "GENSTREET", "GENSTREET", "GOLFER"},
	{"HASJEW", "HIKER", "HIPPY", "HIPSTER", "HIPSTER", "HIPSTER", "INDIAN", "JETSKI", "JUGGALO", "KTOWN"},
	{"KTOWN", "LATINO", "METHHEAD", "MEXTHUG", "MOTOX", "MOTOX", "MUSCLBEAC", "MUSCLBEAC", "POLYNESIAN", "ROADCYC"},
	{"RUNNER", "RUNNER", "SALTON", "SKATER", "SKATER", "SOUCENT", "SOUCENT", "SOUCENT", "SOUCENT", "STBLA"},
	{"STBLA", "STLAT", "STWHI", "STWHI", "SUNBATHE", "SURFER", "VINDOUCHE", "VINEWOOD", "VINEWOOD", "VINEWOOD"},
	{"VINEWOOD", "YOGA", "PROLDRIVER", "RSRANGER", "SBIKE", "STAGGRM", "TATTOO", "ABIGAIL", "ANITA", "ANTON"},
	{"BALLASOG", "BRIDE", "BURGERDRUG", "CAR3GUY1", "CAR3GUY2", "CHEF", "CHIN_GOON", "CLETUS", "COP", "CUSTOMER"},
	{"DENISE_FRIEND", "FOS_REP", "G", "GROOM", "DLR", "HAO", "HUGH", "IMRAN", "JANITOR", "MAUDE"},
	{"MWEATHER", "ORTEGA", "OSCAR", "PORNDUDES", "PORNDUDES_P", "PROLOGUEDRIVER", "PROLSEC", "GANG", "HIC", "HIPSTER"},
	{"MARINE", "MEX", "REPORTER", "ROCCOPELOSI", "SCREEN_WRITER", "STRIPPER", "STRIPPER", "TONYA", "TRAFFICWARDEN", "AMANDATOWNLEY"},
	{"ANDREAS", "ASHLEY", "BANKMAN", "BARRY", "BARRY_P", "BEVERLY", "BEVERLY_P", "BRAD", "BRADCADAVER", "CARBUYER"},
	{"CASEY", "CHENGSR", "CHRISFORMAGE", "CLAY", "DALE", "DAVENORTON", "DEBRA", "DENISE", "DEVIN", "DOM"},
	{"DREYFUSS", "DRFRIEDLANDER", "FABIEN", "FBISUIT", "FLOYD", "GUADALOPE", "GURK", "HUNTER", "JANET", "JEWELASS"},
	{"JIMMYBOSTON", "JIMMYDISANTO", "JOEMINUTEMAN", "JOHNNYKLEBITZ", "JOSEF", "JOSH", "LAMARDAVIS", "LAZLOW", "LESTERCREST", "LIFEINVAD"},
	{"MAGENTA", "MANUEL", "MARNIE", "MARTINMADRAZO", "MARYANN", "MICHELLE", "MILTON", "MOLLY", "MOVPREMF", "MOVPREMMALE"},
	{"MRK", "MRSPHILLIPS", "MRS_THORNHILL", "NATALIA", "NERVOUSRON", "NIGEL", "OLD_MAN1A", "OLD_MAN2", "OMEGA", "ORLEANS"},
	{"PAPER", "PAPER_P", "PATRICIA", "PRIEST", "PROLSEC", "RUSSIANDRUNK", "SIEMONYETARIAN", "SOLOMON", "STEVEHAINS", "STRETCH"},
	{"TANISHA", "TAOCHENG", "TAOSTRANSLATOR", "TENNISCOACH", "TERRY", "TOM", "TOMEPSILON", "TRACYDISANTO", "WADE", "ZIMBOR"},
	{"BALLAS", "FAMILIES", "LOST", "VAGOS", "ARMBOSS", "ARMGOON", "ARMLIEUT", "CHEMWORK", "CHEMWORK_P", "CHIBOSS"},
	{"CHIBOSS_P", "CHICOLD", "CHICOLD_P", "CHIGOON", "CHIGOON_P", "CHIGOON", "KORBOSS", "MEXBOSS", "MEXBOSS", "ARMGOON"},
	{"AZTECA", "BALLAEAST", "BALLAORIG", "BALLASOUT", "FAMCA", "FAMDNF", "FAMFOR", "KOREAN", "KOREAN", "KORLIEUT"},
	{"LOST", "LOST", "LOST", "MEXGANG", "MEXGOON", "MEXGOON", "MEXGOON", "MEXGOON_P", "POLOGOON", "POLOGOON_P"},
	{"POLOGOON", "POLOGOON_P", "SALVABOSS", "SALVAGOON", "SALVAGOON", "SALVAGOON", "SALVAGOON_P", "STRPUNK", "STRPUNK", "HC_DRIVER"},
	{"HC_GUNMAN", "HC_HACKER", "ABIGAIL", "AMANDATOWNLEY", "ANDREAS", "ASHLEY", "BALLASOG", "BANKMAN", "BARRY", "BARRY_P"},
	{"BESTMEN", "BEVERLY", "BEVERLY_P", "BRAD", "BRIDE", "CAR3GUY1", "CAR3GUY2", "CASEY", "CHEF", "CHENGSR"},
	{"CHRISFORMAGE", "CLAY", "CLAYPAIN", "CLETUS", "DALE", "DAVENORTON", "DENISE", "DEVIN", "DOM", "DREYFUSS"},
	{"DRFRIEDLANDER", "FABIEN", "FBISUIT", "FLOYD", "GROOM", "HAO", "HUNTER", "JANET", "JAY_NORRIS", "JEWELASS"},
	{"JIMMYBOSTON", "JIMMYDISANTO", "JOEMINUTEMAN", "JOHNNYKLEBITZ", "JOSEF", "JOSH", "KERRYMCINTOSH", "LAMARDAVIS", "LAZLOW", "LESTERCREST"},
	{"LIFEINVAD", "LIFEINVAD", "MAGENTA", "MANUEL", "MARNIE", "MARYANN", "MAUDE", "MICHELLE", "MILTON", "MOLLY"},
	{"MRK", "MRSPHILLIPS", "MRS_THORNHILL", "NATALIA", "NERVOUSRON", "NIGEL", "OLD_MAN1A", "OLD_MAN2", "OMEGA", "ONEIL"},
	{"ORLEANS", "ORTEGA", "PAPER", "PATRICIA", "PRIEST", "PROLSEC", "GANG", "HIC", "HIPSTER", "MEX"},
	{"ROCCOPELOSI", "RUSSIANDRUNK", "SCREEN_WRITER", "SIEMONYETARIAN", "SOLOMON", "STEVEHAINS", "STRETCH", "TALINA", "TANISHA", "TAOCHENG"},
	{"TAOSTRANSLATOR", "TAOSTRANSLATOR_P", "TENNISCOACH", "TERRY", "TOMEPSILON", "TONYA", "TRACYDISANTO", "TRAFFICWARDEN", "TYLERDIX", "WADE"},
	{"ZIMBOR", "DEADHOOKER", "FREEMODE", "MISTY", "STRIPPERLITE", "PROS", "MP_HEADTARGETS", "CLAUDE", "EXARMY", "FAMDD"},
	{"FIBSEC", "FREEMODE", "MARSTON", "NIKO", "SHOPKEEP", "ARMOURED", "NONE", "NONE", "NONE", "NONE"},
	{"NONE", "FEMBARBER", "MAID", "SHOP_HIGH", "SWEATSHOP", "AIRHOSTESS", "BARTENDER", "BAYWATCH", "COP", "FACTORY"},
	{"HOOKER", "HOOKER", "HOOKER", "MIGRANT", "MOVPREM", "RANGER", "SCRUBS", "SHERIFF", "SHOP_LOW", "SHOP_MID"},
	{"STRIPPERLITE", "STRIPPER", "STRIPPER", "SWEATSHOP", "AMMUCOUNTRY", "ARMOURED", "ARMOURED", "AUTOSHOP", "AUTOSHOP", "BOUNCER"},
	{"CHEMSEC", "CIASEC", "CNTRYBAR", "DOCKWORK", "DOCTOR", "FIBOFFICE", "FIBOFFICE", "GAFFER", "GARDENER", "GENTRANSPORT"},
	{"HAIRDRESS", "HIGHSEC", "HIGHSEC", "JANITOR", "LATHANDY", "LIFEINVAD", "LINECOOK", "LSMETRO", "MARIACHI", "MARINE"},
	{"MARINE", "MIGRANT", "ZOMBIE", "MOVPREM", "MOVSPACE", "PARAMEDIC", "PILOT", "PILOT", "POSTAL", "POSTAL"},
	{"PRISGUARD", "SCIENTIST", "SECURITY", "SNOWCOP", "STRPERF", "STRPREACH", "STRVEND", "TRUCKER", "UPS", "UPS"},
	{"BUSKER", "AIRWORKER", "AMMUCITY", "ARMYMECH", "AUTOPSY", "BARMAN", "BAYWATCH", "BLACKOPS", "BLACKOPS", "BUSBOY"},
	{"CHEF", "CLOWN", "CONSTRUCT", "CONSTRUCT", "COP", "DEALER", "DEVINSEC", "DOCKWORK", "DOORMAN", "DWSERVICE"},
	{"DWSERVICE", "FACTORY", "FIREMAN", "GARBAGE", "GRIP", "HWAYCOP", "MARINE", "MARINE", "MARINE", "MIME"},
	{"PESTCONT", "PILOT", "PRISMUSCL", "PRISONER", "RANGER", "ROBBER", "SHERIFF", "SHOP_MASK", "STRVEND", "SWAT"},
	{"USCG", "VALET", "WAITER", "WINCLEAN", "XMECH", "XMECH", "CORPSE", "MIRANDA", "PROMOURN", "MOVIESTAR"},
	{"PROLHOST", "BIKERCHIC", "COMJANE", "CORPSE", "CORPSE", "HOTPOSH", "JEWELASS", "MISTRESS", "POPPYMICH", "PRINCESS"},
	{"SPYACTRESS", "ALDINAPOLI", "BANKMAN", "BIKEHIRE", "FIBARCHITECT", "FILMDIRECTOR", "GLENSTANK", "GRIFF", "JESUS", "JEWELSEC"},
	{"JEWELTHIEF", "MARKFOST", "PARTYTARGET", "PROLSEC", "PROMOURN", "RIVALPAP", "SPYACTOR", "WILLYFIST", "FINGURU", "TAPHILLBILLY"},
	{"TRAMP", "ABNER", "ANTONB", "BABYD", "BAYGOR", "BURGERDRUG", "CHIP", "CYCLIST", "FIBMUGGER", "GUIDO"},
	{"GUNVEND", "HIPPIE", "IMPORAGE", "JUSTIN", "MANI", "MILITARYBUM", "PAPARAZZI", "PARTY", "POGO", "PRISONER"}
};*/

int activeLineIndexWantedFreeze = 0;

const std::vector<std::string> MENU_WANTED_LEVELS{ "1 Star", "2 Stars", "3 Stars", "4 Stars", "5 Stars", "OFF/Clear" };

/*
int getFrozenWantedLvl(){ return frozenWantedLevel; }
void setFrozenWantedLvl(int level){ frozenWantedLevel = level; }
void setFrozenWantedFeature(bool b){ featureWantedLevelFrozen = b; }
*/

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
		/*
		// skin changer
		case 1:
		if (process_skinchanger_detail_menu())	return;
		break;
		*/
		// fix player
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
	case 14:
		process_anims_menu_top();
		break;
	default:
		break;
	}
	return false;
}

void process_player_menu()
{
	const int lineCount = 15;
	
	std::string caption = "Player Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{"Player Skin", NULL, NULL, false},
		{"Heal Player", NULL, NULL, true},
		{"Add Cash", NULL, NULL, true, CASH},
		{"Wanted Level", NULL, NULL, true, WANTED},
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
		{ "Anims", NULL, NULL, false }
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexPlayer, caption, onconfirm_player_menu);
}


//==================
// WORLD MENU
//==================

int activeLineIndexWorld = 0;
int activeLineIndexTime = 0;

bool onconfirm_time_menu ( MenuItem<int> choice )
{
	switch ( activeLineIndexTime )
	{
		// hour forward/backward
	case 0:
	case 1:
	{
		int h = TIME::GET_CLOCK_HOURS ();
		if ( activeLineIndexTime == 0 ) h = ( h == 23 ) ? 0 : h + 1; else h = ( h == 0 ) ? 23 : h - 1;
		int m = TIME::GET_CLOCK_MINUTES ();
		TIME::SET_CLOCK_TIME ( h, m, 0 );
		char text[32];
		sprintf_s ( text, "Time is now %02d:%02d", h, m );
		set_status_text ( text );
	}
	break;
	case 3:
		if ( featureTimeSynced )
		{
			set_status_text ( "Time synced with system" );
		}
	}
	return false;
}

void process_time_menu ()
{
	const int lineCount = 5;

	std::string caption = "Time Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Hour Forward", NULL, NULL, true },
		{ "Hour Backward", NULL, NULL, true },
		{ "Clock Paused", &featureTimePaused, &featureTimePausedUpdated },
		{ "Sync With System", &featureTimeSynced, NULL },
		{ "Slow Motion", &featureTimeSlow, &featureTimeSlowUpdated }
	};

	draw_menu_from_struct_def ( lines, lineCount, &activeLineIndexTime, caption, onconfirm_time_menu );
}


bool onconfirm_world_menu ( MenuItem<int> choice )
{
	switch ( activeLineIndexWorld )
	{
	case 0:
		process_time_menu ();
		break;
	case 1:
		GAMEPLAY::SET_GRAVITY_LEVEL ( featureWorldMoonGravity );
		break;
	case 2:
		// featureWorldRandomCops being set in update_features
		break;
	case 3:
		VEHICLE::SET_RANDOM_TRAINS ( featureWorldRandomTrains );
		break;
	case 4:
		VEHICLE::SET_RANDOM_BOATS ( featureWorldRandomBoats );
		break;
	case 5:
		VEHICLE::SET_GARBAGE_TRUCKS ( featureWorldGarbageTrucks );
		break;
	}
	return false;
}

void process_world_menu ()
{
	const int lineCount = 7;

	std::string caption = "World Options";

	// read default feature values from the game
	featureWorldRandomCops = (PED::CAN_CREATE_RANDOM_COPS() == TRUE);

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Time", NULL, NULL },
		{ "Moon Gravity", &featureWorldMoonGravity, NULL },
		{ "Random Cops", &featureWorldRandomCops, NULL },
		{ "Random Trains", &featureWorldRandomTrains, NULL },
		{ "Random Boats", &featureWorldRandomBoats, NULL },
		{ "Garbage Trucks", &featureWorldGarbageTrucks, NULL },
		{ "Restricted Zones", &featureRestrictedZones, NULL }
	};

	draw_menu_from_struct_def ( lines, lineCount, &activeLineIndexWorld, caption, onconfirm_world_menu );
}


//==================
// WEATHER MENU
//==================

int activeLineIndexWeather = 0;

bool onconfirm_weather_menu(MenuItem<std::string> choice)
{
	std::stringstream ss; ss << "Weather Frozen at: " << lastWeatherName;
	switch (choice.currentMenuIndex)
	{
		// wind
	case 0:
		if (featureWeatherWind)
		{
			GAMEPLAY::SET_WIND(1.0);
			GAMEPLAY::SET_WIND_SPEED(11.99);
			GAMEPLAY::SET_WIND_DIRECTION(ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
		}
		else
		{
			GAMEPLAY::SET_WIND(0.0);
			GAMEPLAY::SET_WIND_SPEED(0.0);
		}
		break;
		// set weather
	case 1:
		if (featureWeatherFreeze && !lastWeather.empty()){ GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST((char *) lastWeather.c_str()); set_status_text(ss.str()); }
		else if (!featureWeatherFreeze){ GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST(); set_status_text("Weather Freeze Disabled"); }
		else{ set_status_text("Weather Frozen"); }
		break;
	default:
		lastWeather = choice.value.c_str();
		lastWeatherName = choice.caption;
		GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST((char *)lastWeather.c_str());
		if (!featureWeatherFreeze){ GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST(); }
		set_status_text(choice.caption);
	}

	return false;
}

void process_weather_menu()
{
	const int lineCount = 16;

	std::string caption = "Weather Options";

	StringStandardOrToggleMenuDef lines[lineCount] = {
		{"Wind", "WIND",		&featureWeatherWind,	NULL},
		{"Freeze Weather", "FREEZEWEATHER", &featureWeatherFreeze ,NULL },
		{"Extra Sunny", "EXTRASUNNY",	NULL,					NULL},
		{"Clear", "CLEAR",		NULL,					NULL},
		{"Cloudy", "CLOUDS",		NULL,					NULL},
		{"Smog", "SMOG",		NULL,					NULL},
		{"Foggy", "FOGGY",		NULL,					NULL},
		{"Overcast", "OVERCAST",	NULL,					NULL},
		{"Rain", "RAIN",		NULL,					NULL},
		{"Stormy", "THUNDER",		NULL,					NULL},
		{"Clearing", "CLEARING",	NULL,					NULL},
		{"Neutral", "NEUTRAL",		NULL,					NULL},
		{"Snow", "SNOW",		NULL,					NULL},
		{"Blizzard", "BLIZZARD",	NULL,					NULL},
		{"Light Snow", "SNOWLIGHT",	NULL,					NULL},
		{"Christmas", "XMAS",		NULL,					NULL}
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexTime, caption, onconfirm_weather_menu);
}

//==================
// MISC MENU
//==================

int activeLineIndexMisc = 0;

bool onconfirm_misc_menu(MenuItem<int> choice)
{
	switch (activeLineIndexMisc)
	{
		// next radio track
	case 1:
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
	const int lineCount = 4;

	std::string caption = "Miscellaneous Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Portable Radio", &featurePlayerRadio, &featurePlayerRadioUpdated, true },
		{"Next Radio Track",	NULL,					NULL, true},
		{"Hide HUD",			&featureMiscHideHud,	NULL},
		{"Reset Skin On Death", &featurePlayerResetOnDeath, NULL}
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
		process_weather_menu();
		break;
	case 6:
		process_misc_menu();
		break;
	case 7:
		reset_globals();
		break;
	}
	return false;
}

void process_main_menu()
{
	std::string caption = "Enhanced Native Trainer";

	std::vector<std::string> TOP_OPTIONS = {
		"Player",
		"Teleport",
		"Weapons",
		"Vehicles",
		"World/Time",
		"Weather",
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

	draw_generic_menu<int>(menuItems, &activeLineIndexMain, caption, onconfirm_main_menu, NULL, NULL);
}

void reset_globals()
{
	reset_skin_globals();

	reset_vehicle_globals();

	reset_teleporter_globals();

	reset_weapon_globals();

	activeLineIndexMain			=
	activeLineIndexPlayer		=
	activeLineIndexWorld		=
	activeLineIndexWeather		=
	activeLineIndexWantedFreeze	=
	frozenWantedLevel			=	0;

	lastWeather.clear();
	lastWeatherName.clear();

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

	featurePlayerRadio				=

	featureWorldMoonGravity			=
	featureTimePaused				=
	featureTimeSynced				=
	featureWeatherWind				=
	featureWeatherFreeze			=
	featureMiscLockRadio			=
	featureMiscHideHud				=	
	featureWantedLevelFrozen		=	false;

	featureWorldRandomCops		=
	featureWorldRandomTrains	=
	featureWorldRandomBoats		=
	featureWorldGarbageTrucks	=	true;

	featureTimeSlow = false;

	featurePlayerResetOnDeath = true;

	featurePlayerInvincibleUpdated =
	featurePlayerNeverWantedUpdated =
	featurePlayerIgnoredByPoliceUpdated =
	featurePlayerIgnoredByAllUpdated =
	featurePlayerNoNoiseUpdated =
	featurePlayerFastSwimUpdated =
	featurePlayerFastRunUpdated =
	featurePlayerRadioUpdated =
	featurePlayerInvisibleUpdated =
	featureTimeSlowUpdated =
	featureTimePausedUpdated = true;

	set_status_text("Reset All Settings");

	DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, save_settings_thread, 0, 0, &myThreadID);
	CloseHandle(myHandle);

}

void main()
{	
	//reset_globals();

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

		init_storage();

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
	write_text_to_log_file("ScriptTidyUp called");

	save_settings();

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

void update_feature_enablements(std::vector<FeatureEnabledLocalDefinition> pairs)
{
	for (int i = 0; i < pairs.size(); i++)
	{
		FeatureEnabledLocalDefinition pair = pairs.at(i);
	}
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
	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerRadio", &featurePlayerRadio, &featurePlayerRadioUpdated });

	results.push_back(FeatureEnabledLocalDefinition{ "featureWorldMoonGravity", &featureWorldMoonGravity });
	results.push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomCops", &featureWorldRandomCops });
	results.push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomTrains", &featureWorldRandomTrains });
	results.push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomBoats", &featureWorldRandomBoats });
	results.push_back(FeatureEnabledLocalDefinition{ "featureWorldGarbageTrucks", &featureWorldGarbageTrucks });

	results.push_back(FeatureEnabledLocalDefinition{ "featureTimePaused", &featureTimePaused, &featureTimePausedUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featureTimeSynced", &featureTimeSynced });
	results.push_back(FeatureEnabledLocalDefinition{ "featureTimeSlow", &featureTimeSlow, &featureTimeSlowUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featureRestrictedZones", &featureRestrictedZones });

	results.push_back(FeatureEnabledLocalDefinition{ "featureWeatherWind", &featureWeatherWind });
	results.push_back(FeatureEnabledLocalDefinition{ "featureWeatherFreeze", &featureWeatherFreeze });

	results.push_back(FeatureEnabledLocalDefinition{ "featureMiscLockRadio", &featureMiscLockRadio });
	results.push_back(FeatureEnabledLocalDefinition{ "featureMiscHideHud", &featureMiscHideHud });

	results.push_back(FeatureEnabledLocalDefinition{ "featurePlayerResetOnDeath", &featurePlayerResetOnDeath });

	std::vector<FeatureEnabledLocalDefinition> vehResults = get_feature_enablements_vehicles();
	results.insert(results.end(), vehResults.begin(), vehResults.end());

	std::vector<FeatureEnabledLocalDefinition> weapResults = get_feature_enablements_weapons();
	results.insert(results.end(), weapResults.begin(), weapResults.end());

	return results;
}

std::vector<StringPairSettingDBRow> get_generic_settings()
{
	std::vector<StringPairSettingDBRow> settings;
	settings.push_back(StringPairSettingDBRow{ "lastWeather", lastWeather });
	settings.push_back(StringPairSettingDBRow{ "lastWeatherName", lastWeatherName });
	return settings;
}

void handle_generic_settings(std::vector<StringPairSettingDBRow> settings)
{
	for (int i = 0; i < settings.size(); i++)
	{
		StringPairSettingDBRow setting = settings.at(i);
		if (setting.name.compare("lastWeather") == 0)
		{
			lastWeather = setting.value;
		}
		else if (setting.name.compare("lastWeatherName") == 0)
		{
			lastWeatherName = setting.value;
		}
	}

	//pass to anyone else, vehicles, weapons etc
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