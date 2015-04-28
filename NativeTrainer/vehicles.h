#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

#include <string>

/***
* METHODS
*/

void process_veh_menu();

bool process_carspawn_menu();

void update_vehicle_features(BOOL playerExists, Ped playerPed);

void reset_vehicle_globals();

bool process_spawn_menu_cars();

bool process_spawn_menu_indus();

bool process_spawn_menu_generic(int topMenuSelection);

bool onconfirm_spawn_menu_vehicle_selection(int selection, std::string caption, std::string value);

bool do_spawn_vehicle(std::string modelName, std::string modelTitle);