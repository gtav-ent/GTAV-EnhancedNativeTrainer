/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <string>
#include <vector>

#include "../ui_support/menu_functions.h"
#include "../storage/database.h"

#pragma once

struct PropInfo
{
	char* model;
	std::string label;
	std::string category;
};

class SpawnedPropInstance
{
public:
	Object instance;
	std::string title;
	int counter;
};

bool onconfirm_prop_selection(MenuItem<std::string> choice);

void process_props_menu();

void add_props_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_props_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_props(std::vector<StringPairSettingDBRow>* settings);

void cleanup_props();

void reset_prop_globals();