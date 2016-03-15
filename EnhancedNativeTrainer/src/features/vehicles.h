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

#include "..\ui_support\menu_functions.h"
#include "..\storage\database.h"

#include "..\ent-enums.h"

#include <string>

struct PaintColour
{
	std::string name;
	int mainValue;
	int pearlAddition;
};

extern const std::vector<PaintColour> PAINTS_METALLIC;

extern const std::vector<std::string> VALUES_SUPERCARS;

/***
* METHODS
*/

void process_veh_menu();

bool process_savedveh_menu();

bool process_savedveh_slot_menu(int slot);

bool process_vehmod_menu();

bool process_carspawn_menu();

void update_vehicle_features(BOOL playerExists, Ped playerPed);

void reset_vehicle_globals();

bool process_spawn_menu_cars();

bool process_spawn_menu_indus();

bool process_spawn_menu_generic(int topMenuSelection);

bool spawn_saved_car(int slot, std::string caption);

void save_current_vehicle(int slot);

bool onconfirm_spawn_menu_vehicle_selection(MenuItem<std::string> choice);

bool do_spawn_vehicle(std::string modelName, std::string modelTitle);

Vehicle do_spawn_vehicle(DWORD modelHash, std::string modelTitle, bool cleanup);

//Paint menus

bool process_paint_menu();

bool process_paint_menu_type();

bool process_paint_menu_liveries();

bool process_paint_menu_special();

bool onconfirm_color_menu_selection(MenuItem<int> choice);

void onhighlight_color_menu_selection(MenuItem<int> choice);

//Interior Trim 

bool process_light_colors();
bool process_trim_colors();

//Vehicle mod getters and setters

bool is_custom_tyres(std::vector<int> extras);

void set_custom_tyres(bool applied, std::vector<int> extras);

bool is_turbocharged(std::vector<int> extras);

void set_turbocharged(bool applied, std::vector<int> extras);

bool is_bulletproof_tyres(std::vector<int> extras);

void set_bulletproof_tyres(bool applied, std::vector<int> extras);

bool is_xenon_headlights(std::vector<int> extras);

void set_xenon_headlights(bool applied, std::vector<int> extras);

bool is_extra_enabled(std::vector<int> extras);

void set_extra_enabled(bool applied, std::vector<int> extras);

void set_plate_text(MenuItem<int> choice);

bool is_convertible_roofdown(std::vector<int> extras);

void set_convertible_roofdown(bool applied, std::vector<int> extras);

int find_menu_index_to_restore(int category, int actualCategory, Vehicle veh);

bool vehicle_menu_interrupt();

bool vehicle_save_menu_interrupt();

bool vehicle_save_slot_menu_interrupt();

void add_vehicle_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_vehicle_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_vehicle(std::vector<StringPairSettingDBRow>* settings);

void set_chrome_wheels_enabled(Vehicle veh, bool enabled);

bool onconfirm_paintfade(MenuItem<float> choice);

void onhighlight_paintfade(MenuItem<float> choice);

int get_current_veh_invincibility_mode();

void onchange_veh_invincibility_mode(int value, SelectFromListMenuItem* source);

int get_current_veh_eng_pow_index();

void onchange_veh_eng_pow_index(int value, SelectFromListMenuItem* source);

void onchange_veh_mass_index(int value, SelectFromListMenuItem* source);

void set_old_vehicle_state(bool updatedState);

MenuItemImage* vehicle_image_preview_finder(MenuItem<std::string> choice);

void init_vehicle_feature();

void fix_vehicle();

void clean_vehicle();

void apply_paint(PaintColour whichpaint);

int get_current_veh_friction_mode();

void onchange_veh_friction_mode(int value, SelectFromListMenuItem* source);

struct NeonLightsColor
{
	std::string colorString;
	int rVal, gVal, bVal;
};

struct TireSmokeColor
{
	std::string colorString;
	int rVal, gVal, bVal;
};

void apply_neon_colors(int colorIndex);

void onhighlight_neon_lights_selection(MenuItem<int> colorIndex);

bool onconfirm_neon_lights_selection(MenuItem<int> choice);

bool is_neonLights(std::vector<int> extras);

void set_neonLights(bool applied, std::vector<int> extras);

bool process_neon_lights_menu();

//Smoke related code

void apply_smoke_colors(int colorIndex);

void onhighlight_smoke_selection(MenuItem<int> choice);

bool onconfirm_smoke_selection(MenuItem<int> choice);

void set_smoke(bool applied, std::vector<int> extras);

bool process_smoke_colour_menu();

//End of smoke related code

void drive_passenger();

// Vehicle Interior Colours

void apply_trim_colors(int colorIndex);

void onhighlight_trim_colour_selection(MenuItem<int> choice);

bool onconfirm_trim_colour_selection(MenuItem<int> choice);

bool process_trim_design_menu();

void apply_dash_colors(int colorIndex);

void onhighlight_dash_colour_selection(MenuItem<int> choice);

bool onconfirm_dash_colour_selection(MenuItem<int> choice);

bool process_dash_colour_menu();

bool inline is_this_a_car(Vehicle veh)
{
	// Return true if the current vehicle is a car, e.g. as certain vehicles don't support neon lights
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return !(VEHICLE::IS_THIS_MODEL_A_BIKE(et) || VEHICLE::IS_THIS_MODEL_A_HELI(et) || VEHICLE::IS_THIS_MODEL_A_PLANE(et) || VEHICLE::IS_THIS_MODEL_A_TRAIN(et) || VEHICLE::IS_THIS_MODEL_A_BICYCLE(et) || VEHICLE::IS_THIS_MODEL_A_BOAT(et));
}

bool inline is_this_a_motorcycle(Vehicle veh)
{
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_BIKE(et);
}

bool inline is_this_a_heli_or_plane(Vehicle veh)
{
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_HELI(et) || VEHICLE::IS_THIS_MODEL_A_PLANE(et);
}

bool did_player_just_enter_vehicle(Ped playerPed);

void fully_tune_vehicle(Vehicle veh, bool repaint = true, bool optics = true);
