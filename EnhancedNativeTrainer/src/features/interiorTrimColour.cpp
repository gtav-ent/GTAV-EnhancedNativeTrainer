#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"


const std::vector<std::string> TRIM_COLOUR_CAPTIONS{ "Black", "Graphite Black", "Black Steel", "Dark Silver", "Silver", "Blue Silver", "Steel Gray", "Shadow Silver", "Stone Silver", "Midnight Silver", "Gun Metal", "Anthracite Grey", "Black Red", "Red", "Torino Red", "Formula Red", "Blaze Red", "Graceful Red", "Garnet Red", "Desert Red", "Cabernet Red", "Candy Red", "Lava Red", "Vermillion Pink", "Choco Orange", "Sunrise Orange", "Classic Gold", "Orange", "Dark Green", "Racing Green", "Sea Green", "Olive Green", "Green", "Gasoline Blue Green", "Securicor Green", "Mettalic Black Blue", "Metaillic Dark Blue", "Midnight Blue", "Dark Blue", "Saxony Blue", "Blue", "Mariner Blue", "Harbor Blue", "Diamond Blue", "Surf Blue", "Nautical Blue", "Bright Blue", "Purple Blue", "Spinnaker Blue", "Ultra Blue", "Bright Blue", "Black Purple", "Purple", "Taxi Yellow", "Race Yellow", "Yellow Bird", "Bronze", "Lime", "Champagne", "Pueblo Beige", "Straw Beige", "Dark Ivory", "Choco Brown", "Golden Brown", "Light Brown", "Moss Brown", "Biston Brown", "Beechwood", "Dark Beechwood", "Beach Sand", "Sun Bleeched Sand", "Cream", "White", "Frost White" };

const std::vector<int> TRIM_COLOUR_VALUES{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 143, 27, 28, 29, 30, 31, 32, 33, 34, 35, 150, 137, 104, 36, 37, 38, 49, 50, 51, 52, 53, 54, 125, 141, 146, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 142, 145, 88, 89, 91, 90, 92, 93, 94, 99, 95, 96, 97, 98, 99, 100, 101, 102, 103, 105, 106, 107, 111, 112 };


/* Dashboard Colors */
int activeLineIndexLightColors = 0;

bool onconfirm_light_colors(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	VEHICLE::_SET_VEHICLE_DASHBOARD_COLOUR(veh, choice.value);
	return false;
}

bool process_light_colors()
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < TRIM_COLOUR_CAPTIONS.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = TRIM_COLOUR_CAPTIONS[i];
		item->value = TRIM_COLOUR_VALUES[i];
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &activeLineIndexLightColors, "Dash Colors", onconfirm_light_colors, NULL, NULL);
}


/*  Trim Colors */
int activeLineIndexTrimColors = 0;

bool onconfirm_trim_colors(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(veh, choice.value);
	return false;
}

bool process_trim_colors()
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < TRIM_COLOUR_CAPTIONS.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = TRIM_COLOUR_CAPTIONS[i];
		item->value = TRIM_COLOUR_VALUES[i];
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &activeLineIndexTrimColors, "Interior Colors", onconfirm_trim_colors, NULL, NULL);
}