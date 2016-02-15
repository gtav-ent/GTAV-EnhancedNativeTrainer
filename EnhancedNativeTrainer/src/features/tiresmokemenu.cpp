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

const std::vector<RGBColor> SMOKE_COLORS = {
	{ "White", TIRESMOKE_COLOR_WHITE },
	{ "Black", TIRESMOKE_COLOR_BLACK },
	{ "Blue", TIRESMOKE_COLOR_BLUE },
	{ "Yellow", TIRESMOKE_COLOR_YELLOW },
	{ "Purple", TIRESMOKE_COLOR_PURPLE },
	{ "Orange", TIRESMOKE_COLOR_ORANGE },
	{ "Green", TIRESMOKE_COLOR_GREEN },
	{ "Red", TIRESMOKE_COLOR_RED },
	{ "Pink", TIRESMOKE_COLOR_PINK },
	{ "Brown", TIRESMOKE_COLOR_BROWN },
	{ "Patriot", TIRESMOKE_COLOR_PATRIOT }
};

void apply_smoke_colors(int colorIndex)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	RGBColor whichcolor = SMOKE_COLORS[colorIndex];
	VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, whichcolor.rVal, whichcolor.gVal, whichcolor.bVal);
}

void onhighlight_tire_smoke_selection(MenuItem<int> choice)
{
	onconfirm_tire_smoke_selection(choice);
}

bool onconfirm_tire_smoke_selection(MenuItem<int> choice)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
	{
		return true;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("Player isn't in a vehicle");
		return true;
	}

	apply_smoke_colors(choice.value);

	return true;
}

bool is_tireSmoke(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	return VEHICLE::IS_TOGGLE_MOD_ON(veh, MOD_TIRESMOKE);
}

void set_tireSmoke(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (!is_this_a_tire_vehicle(veh))
	{
		set_status_text("Can't add tire smoke to this vehicle");
		return;
	}

	if (applied)
	{
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_TIRESMOKE, TRUE);
	}
	else
	{
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_TIRESMOKE, FALSE);
	}
}

bool process_smoke_colour_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	int colIndex = -1;

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int r = 0, g = 0, b = 0;
	VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);

	for (int i = 0; i < SMOKE_COLORS.size(); i++)
	{
		RGBColor thisCol = SMOKE_COLORS[i];

		//try and match the current col to a value, in order to set the menu index
		if (colIndex == -1 && r == thisCol.rVal && g == thisCol.gVal && b == thisCol.bVal)
		{
			colIndex = i;
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = SMOKE_COLORS[i].colorName;
		item->isLeaf = true;
		item->value = i;
		menuItems.push_back(item);
	}

	if (colIndex == -1)
	{
		colIndex = 0;
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "Choose Tire Smoke Color", onconfirm_tire_smoke_selection, onhighlight_tire_smoke_selection, NULL, vehicle_menu_interrupt);
}

bool onconfirm_smoke_menu(MenuItem<int> choice)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (choice.value == -1)
	{
		process_smoke_colour_menu();
	}
	return false;
}

bool process_tire_smoke_menu() 
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
	{
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); // Get current vehicle

	if (!is_this_a_tire_vehicle(veh)) {
		set_status_text("Can't add tire smoke to this vehicle");
		return false;
	}

	std::vector<MenuItem<int>*> menuItems;

	FunctionDrivenToggleMenuItem<int> *toggleItem = new FunctionDrivenToggleMenuItem<int>();
	toggleItem->caption = "Toggle Tire Smoke";
	toggleItem->getter_call = is_tireSmoke;
	toggleItem->setter_call = set_tireSmoke;
	menuItems.push_back(toggleItem);

	MenuItem<int> *chooseSmokeColor = new MenuItem<int>();
	chooseSmokeColor->caption = "Choose Color";
	chooseSmokeColor->value = -1;
	chooseSmokeColor->isLeaf = false;
	menuItems.push_back(chooseSmokeColor);

	return draw_generic_menu<int>(menuItems, 0, "Tire Smoke", onconfirm_smoke_menu, NULL, NULL, vehicle_menu_interrupt);
}