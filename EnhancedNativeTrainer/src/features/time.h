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

#include <string>
#include <vector>

#include "..\storage\database.h"

void process_time_menu();

void reset_time_globals();

void update_time_features(Player player);

void add_time_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_time_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_time(std::vector<StringPairSettingDBRow>* settings);

void movetime_day_forward();

void movetime_day_backward();

void movetime_hour_forward();

void movetime_hour_backward();

void movetime_fivemin_forward();

void movetime_fivemin_backward();

std::string get_day_of_game_week();