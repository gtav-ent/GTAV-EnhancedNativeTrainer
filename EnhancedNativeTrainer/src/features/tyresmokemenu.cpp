
#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"

const std::vector<TireSmokeColor> SMOKE_COLORS = {
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
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); // Get current vehicle
	TireSmokeColor whichcolor = SMOKE_COLORS[colorIndex];
	VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, whichcolor.rVal, whichcolor.gVal, whichcolor.bVal);
	VEHICLE::TOGGLE_VEHICLE_MOD(veh, 20, 1);
}

void onhighlight_smoke_selection(MenuItem<int> choice)
{
	onconfirm_smoke_selection(choice);
}

bool onconfirm_smoke_selection(MenuItem<int> choice)
{
	// common variables
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

void set_smoke(bool applied, std::vector<int> extras)
{
	int loc = extras.at(0);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int rCol, bCol, gCol = 0;
	bool lightFound = false;

	if (!is_this_a_car(veh) || !is_this_a_motorcycle)
	{
		set_status_text("Can't add smoke to this vehicle");
		return;
	}

	if (applied) // Turn on the smoke
	{
		if (!rCol && !bCol && !gCol)
		{
			TireSmokeColor col = SMOKE_COLORS.at(0);
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, col.rVal, col.gVal, col.bVal);
		}
	}
	else
	{
		// Default smoke
		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, 255, 255, 255);
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
		TireSmokeColor thisCol = SMOKE_COLORS[i];

		//try and match the current col to a value, in order to set the menu index
		if (colIndex == -1 && r == thisCol.rVal && g == thisCol.gVal && b == thisCol.bVal)
		{
			colIndex = i;
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = SMOKE_COLORS[i].colorString;
		item->isLeaf = true;
		item->value = i;
		menuItems.push_back(item);
	}

	if (colIndex == -1)
	{
		colIndex = 0;
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "Choose Tire Smoke Color", onconfirm_smoke_selection, onhighlight_smoke_selection, NULL, vehicle_menu_interrupt);
}