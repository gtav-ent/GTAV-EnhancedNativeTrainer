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

const std::vector<NeonLightsColor> NEON_COLORS = {
	{ "Bright White", NEON_COLOR_WHITE },
	{ "Dim White", NEON_COLOR_BLACK },
	{ "Electric Blue", NEON_COLOR_ELECTRICBLUE },
	{ "Mint Green", NEON_COLOR_MINTGREEN },
	{ "Lime Green", NEON_COLOR_LIMEGREEN },
	{ "Yellow", NEON_COLOR_YELLOW },
	{ "Gold", NEON_COLOR_GOLDENSHOWER },
	{ "Orange", NEON_COLOR_ORANGE },
	{ "Red", NEON_COLOR_RED },
	{ "Pink", NEON_COLOR_PONYPINK },
	{ "Hot Pink", NEON_COLOR_HOTPINK },
	{ "Purple", NEON_COLOR_PURPLE },
	{ "Black Light", NEON_COLOR_BLACKLIGHT }
};

int menuIndex = 0;

void apply_neon_colors(int colorIndex)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); // Get current vehicle
	NeonLightsColor whichcolor = NEON_COLORS[colorIndex];
	VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, whichcolor.rVal, whichcolor.gVal, whichcolor.bVal);
}

void onhighlight_neon_lights_selection(MenuItem<int> choice)
{
	onconfirm_neon_lights_selection(choice);
}

bool onconfirm_neon_lights_selection(MenuItem<int> choice)
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
		set_status_text("~r~Error:~r~ Player isn't in a vehicle");
		return true;
	}

	apply_neon_colors(choice.value);

	return true;
}

bool is_neonLights(std::vector<int> extras)
{
	int loc = extras.at(0);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, loc))
	{
		return true; // return as soon as we detect at least a single neon light enabled
	}

	return false;
}

void set_neonLights(bool applied, std::vector<int> extras)
{
	int loc = extras.at(0);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int rCol = 0, gCol = 0, bCol = 0;
	bool lightFound = false;

	if (!is_this_a_car(veh))
	{
		set_status_text("~r~Error:~r~ Can't add neon lights to this vehicle");
		return;
	}

	if (applied) // Turn on the neon lights
	{
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, loc, true);
		VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &rCol, &gCol, &bCol);
		if (!rCol && !gCol && !bCol)
		{
			NeonLightsColor col = NEON_COLORS.at(0);
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, col.rVal, col.gVal, col.bVal);
		}
	}
	else
	{
		// Turn off the lights
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, loc, false);
	}
}

std::string getNeonPositionLabel(int i)
{
	switch (i)
	{
	case NEON_LEFT:
		return "Left";
	case NEON_RIGHT:
		return "Right";
	case NEON_FRONT:
		return "Front";
	case NEON_BACK:
		return "Back";
	default:
		return "Unknown";
	}
}

bool process_neon_colour_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	int colIndex = -1;

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int r = 0, g = 0, b = 0;
	VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &r, &g, &b);

	for (int i = 0; i < NEON_COLORS.size(); i++)
	{
		NeonLightsColor thisCol = NEON_COLORS[i];

		//try and match the current col to a value, in order to set the menu index
		if (colIndex == -1 && r == thisCol.rVal && g == thisCol.gVal && b == thisCol.bVal)
		{
			colIndex = i;
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = NEON_COLORS[i].colorString;
		item->isLeaf = true;
		item->value = i;
		menuItems.push_back(item);
	}

	if (colIndex == -1)
	{
		colIndex = 0;
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "Choose Neon Lights Color", onconfirm_neon_lights_selection, onhighlight_neon_lights_selection, NULL, vehicle_menu_interrupt);
}

bool onconfirm_neon_menu(MenuItem<int> choice)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (choice.value == -1) //toggle all
	{
		bool anyEnabled = false;
		int r = 0, g = 0, b = 0;
		
		for (int loc = 0; loc <= 3; loc++)
		{
			if (VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, loc))
			{
				anyEnabled = true;
				break;
			}
		}
		for (int loc = 0; loc <= 3; loc++)
		{
			if (r == 0 && g == 0 && b == 0)
			{
				VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &r, &g, &b);
			}
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, loc, !anyEnabled);
		}

		//if they have no colour, set the default
		if (!anyEnabled && r == 0 && g == 0 && b == 0)
		{
			NeonLightsColor col = NEON_COLORS.at(0);
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, col.rVal, col.gVal, col.bVal);
		}
	}
	else if (choice.value == -2)
	{
		process_neon_colour_menu();
	}
	return false;
}


bool process_neon_lights_menu() {
	// common variables
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

	if (!is_this_a_car(veh)) {
		set_status_text("Can't add neon lights to this vehicle");
		return false;
	}

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *allLightsToggle = new MenuItem<int>();
	allLightsToggle->caption = "Toggle All Neon Lights";
	allLightsToggle->value = -1;
	allLightsToggle->isLeaf = true;
	menuItems.push_back(allLightsToggle);

	for (int loc = 0; loc <= 3; loc++)
	{
		FunctionDrivenToggleMenuItem<int> *neonLightsToggle = new FunctionDrivenToggleMenuItem<int>();
		std::ostringstream ss;
		ss << "Enable Neons: " << getNeonPositionLabel(loc);
		neonLightsToggle->caption = ss.str();
		neonLightsToggle->getter_call = is_neonLights;
		neonLightsToggle->setter_call = set_neonLights;
		neonLightsToggle->extra_arguments.push_back(loc);
		menuItems.push_back(neonLightsToggle);
	}

	MenuItem<int> *chooseColourAll = new MenuItem<int>();
	chooseColourAll->caption = "Choose Colour";
	chooseColourAll->value = -2;
	chooseColourAll->isLeaf = false;
	menuItems.push_back(chooseColourAll);

	return draw_generic_menu<int>(menuItems, &menuIndex, "Neon Lights", onconfirm_neon_menu, NULL, NULL, vehicle_menu_interrupt);
}
