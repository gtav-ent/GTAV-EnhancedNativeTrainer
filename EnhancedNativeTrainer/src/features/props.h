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
	bool isInvincible;
	bool isImmovable;
	bool hasGravity;
};

extern SpawnedPropInstance* currentProp;

bool onconfirm_prop_selection(MenuItem<std::string> choice);

void process_props_menu();

void add_props_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_props_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_props(std::vector<StringPairSettingDBRow>* settings);

void cleanup_props();

void reset_prop_globals();

int find_highest_instance_num_of_prop(Hash model);

bool prop_spawned_instances_menu();

void onhighlight_prop_instance_menu(MenuItem<int> choice);

bool onconfirm_prop_instance_menu(MenuItem<int> choice);

bool prop_spawned_single_instance_menu(int index);

bool prop_instance_menu_interrupt();

SpawnedPropInstance* get_prop_at_index(int i);

void flash_prop_callback();

void process_prop_explosion_choices();