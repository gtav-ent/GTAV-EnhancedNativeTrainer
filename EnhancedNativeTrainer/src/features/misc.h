/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "..\io\io.h"
#include "..\io\config_io.h"
#include "..\ui_support\menu_functions.h"
#include "..\debug\debuglog.h"
#include "..\storage\database.h"

void process_misc_menu();

void reset_misc_globals();

void update_misc_features(BOOL playerExists, Ped playerPed);

void add_misc_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_misc_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_misc(std::vector<StringPairSettingDBRow>* settings);

bool is_player_reset_on_death();

bool is_input_blocked_in_menu();
