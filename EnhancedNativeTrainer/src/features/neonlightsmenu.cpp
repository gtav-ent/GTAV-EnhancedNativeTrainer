/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"

const std::vector<NeonLightsColor> NEON_COLORS{
	{ "White", 255, 255, 255},
	{ "Red", 255, 0, 0 },
	{ "Green", 0, 255, 0 },
	{ "Blue", 0, 0, 255 },
	{ "Yellow", 255, 255, 0 },
	{ "Magenta", 255, 0, 255 },
	{ "Cyan", 0, 255, 255 }
};

int menuIndex = 0;

bool inline isThisACar(Vehicle veh) {
	// Return true if the current vehicle is a car, as certain vehicles don't support neon lights
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);

	return !(VEHICLE::IS_THIS_MODEL_A_BIKE(et) || VEHICLE::IS_THIS_MODEL_A_HELI(et) || VEHICLE::IS_THIS_MODEL_A_PLANE(et) || VEHICLE::IS_THIS_MODEL_A_TRAIN(et) || VEHICLE::IS_THIS_MODEL_A_BICYCLE(et) || VEHICLE::IS_THIS_MODEL_A_BOAT(et));
}

void apply_neon_colors(int colorIndex) {
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); // Get current vehicle

	NeonLightsColor whichcolor = NEON_COLORS[colorIndex];
	VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, whichcolor.rVal, whichcolor.gVal, whichcolor.bVal);
}

void onhighlight_neon_lights_selection(MenuItem<int> colorIndex) {
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
		return;

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		set_status_text("Player isn't in a vehicle");
		return;
	}

	if (colorIndex.value != 0)
		apply_neon_colors(colorIndex.value - 1);
}

bool onconfirm_neon_lights_selection(MenuItem<int> choice) {
	return true;
}

bool is_neonLights(std::vector<int> extras) {
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	for (int loc = 0; loc <= 3; loc++) {
		if (VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, loc))
			return true; // return as soon as we detect at least a single neon light enabled
	}

	return false;
}

void set_neonLights(bool applied, std::vector<int> extras) {
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int rCol, bCol, gCol = 0;
	bool lightFound = false;

	if (!isThisACar(veh)) {
		set_status_text("Can't add neon lights to this vehicle");
		return;
	}

	if (applied) { // Turn on the neon lights
		for (int loc = 0; loc <= 3; loc++)
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, loc, true);

		VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &rCol, &bCol, &gCol); // Let's check if we have existing colors defined... if not, slap on something random
		if (!rCol && !bCol && !gCol) {
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
		}
	} else {// Turn off the lights
		for (int loc = 0; loc <= 3; loc++)
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, loc, false);
		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, 0, 0, 0); // Zero out any colors we had defined
	}
}

bool process_neon_lights_menu() {
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists) {
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); // Get current vehicle

	if (!isThisACar(veh)) {
		set_status_text("Can't add neon lights to this vehicle");
		return false;
	}

	std::vector<MenuItem<int>*> menuItems;

	FunctionDrivenToggleMenuItem<int> *neonLightsToggle = new FunctionDrivenToggleMenuItem<int>();
	neonLightsToggle->caption = "Toggle Neon Lights";
	neonLightsToggle->getter_call = is_neonLights;
	neonLightsToggle->setter_call = set_neonLights;
	neonLightsToggle->value = 0;
	menuItems.push_back(neonLightsToggle);


	for (int i = 1; i < NEON_COLORS.size(); i++) {
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = NEON_COLORS[i-1].colorString;
		item->isLeaf = true;
		item->value = i;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &menuIndex, "Choose Neon Lights Color", onconfirm_neon_lights_selection, onhighlight_neon_lights_selection, NULL, vehicle_menu_interrupt);
}