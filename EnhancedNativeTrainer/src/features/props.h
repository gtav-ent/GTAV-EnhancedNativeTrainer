/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <string>
#include <vector>

#include "../ui_support/menu_functions.h"
#include "../storage/database.h"
#include "../ent-enums.h"

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
	const bool isEmpty() { return instance == -1; };
	virtual inline SpawnedPropInstance::~SpawnedPropInstance() {};
	Object instance = -1;
	std::string title;
	int counter;
	bool isInvincible;
	bool isImmovable;
	bool hasGravity;
	bool inline operator==(SpawnedPropInstance rhs)const
	{
		return rhs.instance == instance;
	}
};

extern SpawnedPropInstance currentProp;

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

SpawnedPropInstance get_prop_at_index(int i);

void flash_prop_callback();

void process_prop_explosion_choices();

SimpleVector3 get_camera_coords(Vector3 entityCoords, float cameraDistance, float camPitch, float camHeading);

void do_spawn_model_by_player(Hash propHash, char* model, std::string title, bool silent);

void do_spawn_model(Hash propHash, char* model, std::string title, SimpleVector3* coords, float pitch, float roll, float heading,
	bool invincible, bool immovable, bool gravity, float alpha, bool silent);

bool props_individual_slot_menu_interrupt();

bool props_save_slots_menu_interrupt();

bool process_savedprops_menu();