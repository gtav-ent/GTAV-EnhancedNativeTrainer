/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "..\io\io.h"
#include "..\io\config_io.h"
#include "..\io\xinput.h"
#include "..\ui_support\menu_functions.h"
#include "..\storage\database.h"
#include "..\debug\debuglog.h"

#include "skins.h"
#include "script.h"
#include "vehicles.h"
#include "teleportation.h"
#include "airbrake.h"
#include "weapons.h"
#include "anims.h";
#include "world.h";
#include "time.h";
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

#include <fstream>

//Ensures numbers are formatted with commas, not the locale option
class comma_numpunct : public std::numpunct<char>
{
protected:
	virtual char do_thousands_sep() const
	{
		return ',';
	}

	virtual std::string do_grouping() const
	{
		return "\03";
	}
};

void ScriptMain();

void ScriptTidyUp();

bool process_skinchanger_menu();

bool process_skinchanger_detail_menu();

void update_features();

void reset_globals();

void set_all_nearby_peds_to_calm(Ped playerPed, int count);

//DB persistent stuff

std::vector<FeatureEnabledLocalDefinition> get_feature_enablements();

std::vector<StringPairSettingDBRow> get_generic_settings();

void handle_generic_settings(std::vector<StringPairSettingDBRow> settings);


int getFrozenWantedLvl();
void setFrozenWantedLvl(int level);
bool getFrozenWantedFeature();
void setFrozenWantedFeature(bool b);


void turn_off_never_wanted();

void load_settings();

void save_settings();

DWORD WINAPI save_settings_thread(LPVOID lpParameter);

void init_storage();

char* get_storage_dir_path();

char* get_storage_dir_path(char* file);

ENTDatabase* get_database();

bool should_block_input_in_menu();

bool does_veh_invuln_include_cosmetic();
