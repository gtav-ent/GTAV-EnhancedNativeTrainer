/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "..\storage\database.h"
#include "..\ui_support\menu_functions.h"

#include <string>
#include <sstream> 
#include <fstream>
#include <vector>
#include <set>

void process_areaeffect_menu();

void update_area_effects(Ped playerPed);

void update_nearby_peds(Ped playerPed, int pedCount);

void update_nearby_vehicles(Ped playerPed, int vehCount);

void cleanup_area_effects();

void reset_areaeffect_globals();

void add_areaeffect_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_areaeffect_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_areaeffect(std::vector<StringPairSettingDBRow>* settings);

void set_all_nearby_peds_to_invincible();

void set_all_nearby_peds_to_calm();

void kill_all_nearby_peds();

void kill_all_nearby_vehicles();

void set_all_nearby_vehs_to_invincible(bool enabled);

void set_all_nearby_vehs_to_broken(bool enabled);