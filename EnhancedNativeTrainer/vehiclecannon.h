/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

#include "menu_functions.h"

#include <string>

void process_can_menu();

float vectRads(float degs);

bool process_fodderchanger_menu();

bool onconfirm_fodderchanger_menu(MenuItem<int> choice);

bool onconfirm_fodderchanger_choices_animals(MenuItem<std::string> choice);

bool process_fodderchanger_choices_animals();

bool onconfirm_fodderchanger_choices_misc(MenuItem<std::string> choice);

bool process_fodderchanger_choices_misc();

bool process_carpick_menu();

void vehicle_cannon(std::string lastSpawn);

void update_cannon();

void reset_cannon_globals();

bool process_pick_menu_cars();

bool process_pick_menu_indus();

bool process_pick_menu_generic(int topMenuSelection);

bool onconfirm_pick_menu_vehicle_selection(MenuItem<std::string> choice);

bool updateCannonChoice(std::string modelName, std::string modelTitle);

std::vector<FeatureEnabledLocalDefinition> get_feature_state_cannon();
