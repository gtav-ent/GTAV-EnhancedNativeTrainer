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

void process_world_menu();

void process_weather_menu();

void reset_world_globals();

void update_world_features();

void add_world_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_world_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_world(std::vector<StringPairSettingDBRow>* settings);