#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"

char* SUPPORTS_DIAL_AND_TRIM_COLOUR[] = { "BANSHEE2", "BUCCANEER2", "CHINO2", "FACTION2", "MOONBEAM2", "SULTANRS", "PRIMO2", "VOODOO" };
char* SUPPORTS_TRIM_COLOUR[] = { "SCHAFTER3", "SCHAFTER4", "SCHAFTER5", "SCHAFTER6", "COG55", "COG552", "COGNOSCENTI", "COGNOSCENTI2", "BALLER3", "BALLER4", "BALLER5", "BALLER6" };

char* TRIM_OR_DIAL_VEHS[] = { "SCHAFTER3", "SCHAFTER4", "SCHAFTER5", "SCHAFTER6", "COG55", "COG552", "COGNOSCENTI", "COGNOSCENTI2", "BALLER3", "BALLER4", "BALLER5", "BALLER6", "BANSHEE2", "BUCCANEER2", "CHINO2", "FACTION2", "MOONBEAM2", "SULTANRS", "PRIMO2", "VOODOO" };


const std::vector<TrimColour> TRIM_COLORS = {
	{ "Black", 0 },
	{ "Graphite Black", 1 },
	{ "Black Steel", 2 },
	{ "Dark Silver", 3 },
	{ "Silver", 4 },
	{ "Blue Silver", 5 },
	{ "Steel Gray", 6 },
	{ "Shadow Silver", 7 },
	{ "Stone Silver", 8 },
	{ "Midnight Silver", 9 },
	{ "Gun Metal", 10 },
	{ "Anthracite Grey", 11 },
	{ "Black Red", 143 },
	{ "Red", 27 },
	{ "Torino Red", 28 },
	{ "Formula Red", 29 },
	{ "Blaze Red", 30 },
	{ "Graceful Red", 31 },
	{ "Garnet Red", 32},
	{ "Desert Red", 33 },
	{ "Cabernet Red", 34 },
	{ "Candy Red", 35 },
	{ "Lava Red", 150 },
	{ "Vermillion Pink", 137 },
	{ "Choco Orange", 104 },
	{ "Sunrise Orange", 36 },
	{ "Classic Gold", 37 },
	{ "Orange", 38 },
	{ "Dark Green", 49 },
	{ "Racing Green", 50 },
	{ "Sea Green", 51 },
	{ "Olive Green", 52 },
	{ "Green", 53 },
	{ "Gasoline Blue Green", 54 },
	{ "Securicor Green", 125 },
	{ "Mettalic Black Blue", 141 },
	{ "Mettalic Dark Blue", 146 },
	{ "Midnight Blue", 61 },
	{ "Dark Blue", 62 },
	{ "Saxony Blue", 63 },
	{ "Blue", 64 },
	{ "Mariner Blue", 65 },
	{ "Harbor Blue", 66 },
	{ "Diamond Blue", 67 },
	{ "Surf Blue", 68 },
	{ "Nautical Blue", 69 },
	{ "Bright Blue", 70 },
	{ "Purple Blue", 71 },
	{ "Spinnaker Blue", 72 },
	{ "Ultra Blue", 73 },
	{ "Bright Blue", 74 },
	{ "Black Purple", 142 },
	{ "Purple", 145 },
	{ "Taxi Yellow", 88 },
	{ "Race Yellow", 89 },
	{ "Yellow Bird", 91 },
	{ "Bronze", 90 },
	{ "Lime", 92 },
	{ "Champagne", 93 },
	{ "Pueblo Beige", 94 },
	{ "Straw Beige", 95 },
	{ "Dark Ivory", 96 },
	{ "Choco Brown", 97 },
	{ "Golden Brown", 98 },
	{ "Light Brown", 99},
	{ "Moss Brown", 100 },
	{ "Biston Brown", 101 },
	{ "Beechwood", 102 },
	{ "Dark Beechwood", 103 },
	{ "Beach Sand", 105 },
	{ "Sun Bleeched Sand", 106 },
	{ "Cream", 107 },
	{ "White", 111 },
	{ "Frost White", 112 }
};

void apply_dash_colors(int colorIndex)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	TrimColour whichcolor = TRIM_COLORS[colorIndex];
	VEHICLE::_SET_VEHICLE_DASHBOARD_COLOUR(veh, whichcolor.trimColVal);
}

void onhighlight_dash_colour_selection(MenuItem<int> choice)
{
	onconfirm_dash_colour_selection(choice);
}

bool onconfirm_dash_colour_selection(MenuItem<int> choice)
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

	apply_dash_colors(choice.value);

	return true;
}

bool process_dash_colour_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	int colIndex = -1;

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int currDashCol;
	VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &currDashCol);

	for (int i = 0; i < TRIM_COLORS.size(); i++)
	{
		TrimColour thisCol = TRIM_COLORS[i];

		//try and match the current col to a value, in order to set the menu index
		if (colIndex == -1 && currDashCol == thisCol.trimColVal)
		{
			colIndex = i;
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = TRIM_COLORS[i].trimColName;
		item->isLeaf = true;
		item->value = i;
		menuItems.push_back(item);
	}

	if (colIndex == -1)
	{
		colIndex = 0;
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "Choose Dash Colour", onconfirm_dash_colour_selection, onhighlight_dash_colour_selection, NULL, vehicle_menu_interrupt);
}

void apply_trim_colors(int colorIndex)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	TrimColour whichcolor = TRIM_COLORS[colorIndex];
	VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(veh, whichcolor.trimColVal);
}

void onhighlight_trim_colour_selection(MenuItem<int> choice)
{
	onconfirm_trim_colour_selection(choice);
}

bool onconfirm_trim_colour_selection(MenuItem<int> choice)
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

	apply_trim_colors(choice.value);

	return true;
}

bool process_trim_colour_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	int colIndex = -1;

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int currTrimCol;
	VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &currTrimCol);

	for (int i = 0; i < TRIM_COLORS.size(); i++)
	{
		TrimColour thisCol = TRIM_COLORS[i];

		//try and match the current col to a value, in order to set the menu index
		if (colIndex == -1 && currTrimCol == thisCol.trimColVal)
		{
			colIndex = i;
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = TRIM_COLORS[i].trimColName;
		item->isLeaf = true;
		item->value = i;
		menuItems.push_back(item);
	}

	if (colIndex == -1)
	{
		colIndex = 0;
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "Choose Trim Colour", onconfirm_trim_colour_selection, onhighlight_trim_colour_selection, NULL, vehicle_menu_interrupt);
}

bool onconfirm_trim_colour_menu(MenuItem<int> choice)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (choice.value == -1)
	{
		process_dash_colour_menu();
	}
	else if (choice.value == -2)
	{
		process_trim_colour_menu();
	}
	return false;
}

bool process_interior_colour_menu()
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
	
	Hash currVehModel = ENTITY::GET_ENTITY_MODEL(veh);

	bool foundVeh = false;
	for each (char* vehModel in TRIM_OR_DIAL_VEHS)
	{
		if (GAMEPLAY::GET_HASH_KEY(vehModel) == currVehModel)
		{
			foundVeh = true;
		}
	}

	if (!foundVeh)
	{
		set_status_text("This vehicle is not supported");
		return false;
	}

	std::vector<MenuItem<int>*> menuItems;

	bool dial_and_trim_col = false;
	bool trim_col = false;

	for each (char* vehModel in SUPPORTS_DIAL_AND_TRIM_COLOUR)
	{
		if (GAMEPLAY::GET_HASH_KEY(vehModel) == currVehModel)
		{
			dial_and_trim_col = true;
		}
	}

	for each (char* vehModel in SUPPORTS_TRIM_COLOUR)
	{
		if (GAMEPLAY::GET_HASH_KEY(vehModel) == currVehModel)
		{
			trim_col = true;
		}
	}

	if (dial_and_trim_col)
	{
		MenuItem<int> *chooseDashColor = new MenuItem<int>();
		chooseDashColor->caption = "Dash Colour ~HUD_COLOUR_GREYLIGHT~(74)";
		chooseDashColor->value = -1;
		chooseDashColor->isLeaf = false;
		menuItems.push_back(chooseDashColor);

		MenuItem<int> *chooseTrimColor = new MenuItem<int>();
		chooseTrimColor->caption = "Trim Colour ~HUD_COLOUR_GREYLIGHT~(74)";
		chooseTrimColor->value = -2;
		chooseTrimColor->isLeaf = false;
		menuItems.push_back(chooseTrimColor);
	}

	if (trim_col)
	{
		MenuItem<int> *chooseTrimColor = new MenuItem<int>();
		chooseTrimColor->caption = "Trim Colour ~HUD_COLOUR_GREYLIGHT~(74)";
		chooseTrimColor->value = -2;
		chooseTrimColor->isLeaf = false;
		menuItems.push_back(chooseTrimColor);
	}

	return draw_generic_menu<int>(menuItems, 0, "Interior Colors", onconfirm_trim_colour_menu, NULL, NULL, vehicle_menu_interrupt);
}
