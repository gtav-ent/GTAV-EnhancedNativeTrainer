/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai SMith and fellow contributors 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include "..\..\inc\main.h"

#include "weapons.h"
#include "skins.h"
#include "..\ent-enums.h"

#include "..\ui_support\menu_functions.h"

#include "..\storage\database.h"

bool process_bodyguard_menu();

bool onconfirm_bodyguard_menu(MenuItem<int> choice);

bool process_bodyguard_skins_menu();

bool onconfirm_bodyguard_skins_menu(MenuItem<int> choice);

bool process_player_skins_menu();

bool process_npc_skins_menu();

bool process_bodyguard_weapons_menu();

void dismiss_bodyguards();

void do_spawn_bodyguard();

void update_bodyguard_features();

void add_bodyguards_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_bodyguards_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_bodyguards(std::vector<StringPairSettingDBRow>* settings);

bool bodyguards_main_menu_interrupt();