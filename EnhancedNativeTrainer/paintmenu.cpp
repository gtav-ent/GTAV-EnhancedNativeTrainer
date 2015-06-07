/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "vehicles.h"
#include "menu_functions.h"
#include "config_io.h"

extern int whichpart = 0;
extern int whichtype = 0;

//Parts
const std::vector<std::string> MENU_PAINT_WHAT{ "Primary Color", "Secondary Color", "Pearl Topcoat", "Wheels" };


//Paint Names
const std::vector<std::string> MENU_PAINT_TYPE{ "Metallic", "Normal", "Matte", "Metal", "Chrome" };

const std::vector<std::string> CAPTIONS_NORMAL{ "Black", "Carbon Black", "Graphite Black", "Anthracite Black", "Black Steel", "Dark Steel", "Silver", "Bluish Silver", "Rolled Steel", "Shadow Silver", "Stone Silver", "Midnight Silver", "Cast Iron Silver", "Red", "Torino Red", "Formula Red", "Lava Red", "Blaze Red", "Grace Red", "Garnet Red", "Sunset Red", "Cabernet Red", "Wine Red", "Candy Red", "Hot Pink", "Pfister Pink", "Salmon Pink", "Sunrise Orange", "Orange", "Bright Orange", "Gold", "Bronze", "Yellow", "Race Yellow", "Dew Yellow", "Dark Green", "Racing Green", "Sea Green", "Olive Green", "Bright Green", "Gasoline Green", "Lime Green", "Midnight Blue", "Galaxy Blue", "Dark Blue", "Saxon Blue", "Blue", "Mariner Blue", "Harbor Blue", "Diamond Blue", "Surf Blue", "Nautical Blue", "Racing Blue", "Ultra Blue", "Light Blue", "Chocolate Brown", "Bison Brown", "Creek Brown", "Feltzer Brown", "Maple Brown", "Beechwood Brown", "Sienna Brown", "Saddle Brown", "Moss Brown", "Woodbeech Brown", "Straw Brown", "Sandy Brown", "Bleached Brown", "Schafter Purple", "Spinnaker Purple", "Midnight Purple", "Bright Purple", "Cream", "Ice White", "Frost White" };

const std::vector<std::string> CAPTIONS_METALLIC{ "Black", "Carbon Black", "Graphite Black", "Anthracite Black", "Black Steel", "Dark Steel", "Silver", "Bluish Silver", "Rolled Steel", "Shadow Silver", "Stone Silver", "Midnight Silver", "Cast Iron Silver", "Red", "Torino Red", "Formula Red", "Lava Red", "Blaze Red", "Grace Red", "Garnet Red", "Sunset Red", "Cabernet Red", "Wine Red", "Candy Red", "Hot Pink", "Pfister Pink", "Salmon Pink", "Sunrise Orange", "Orange", "Bright Orange", "Gold", "Bronze", "Yellow", "Race Yellow", "Dew Yellow", "Dark Green", "Racing Green", "Sea Green", "Olive Green", "Bright Green", "Gasoline Green", "Lime Green", "Midnight Blue", "Galaxy Blue", "Dark Blue", "Saxon Blue", "Blue", "Mariner Blue", "Harbor Blue", "Diamond Blue", "Surf Blue", "Nautical Blue", "Racing Blue", "Ultra Blue", "Light Blue", "Chocolate Brown", "Bison Brown", "Creek Brown", "Feltzer Brown", "Maple Brown", "Beechwood Brown", "Sienna Brown", "Saddle Brown", "Moss Brown", "Woodbeech Brown", "Straw Brown", "Sandy Brown", "Bleached Brown", "Schafter Purple", "Spinnaker Purple", "Midnight Purple", "Bright Purple", "Cream", "Ice White", "Frost White" };

const std::vector<std::string> CAPTIONS_MATTE{ "Black", "Gray", "Light Gray", "Ice White", "Blue", "Dark Blue", "Midnight Blue", "Midnight Purple", "Schafter Purple", "Red", "Dark Red", "Orange", "Yellow", "Lime Green", "Green", "Forest Green", "Foliage Green", "Olive Drab", "Dark Earth", "Desert Tan" };

const std::vector<std::string> CAPTIONS_METAL{ "Brushed Steel", "Brushed Black Steel", "Brushed Aluminum", "Pure Gold", "Brushed Gold" };

const std::vector<std::string> CAPTIONS_CHROME{ "Chrome" };

const std::vector<std::string> CAPTIONS_WHEELS{ "Bright Silver", "Aluminum", "Alloy", "Black", "Carbon Black", "Anthracite Black", "Black Steel", "Stone Silver", "Frost White", "Red", "Blaze Red", "Garnet Red", "Candy Red", "Sunset Red", "Salmon Pink", "Hot Pink", "Sunrise Orange", "Orange", "Bright Orange", "Gold", "Straw Brown", "Dark Copper", "Dark Ivory", "Dark Brown", "Bronze", "Dark Earth", "Desert Tan", "Yellow", "Race Yellow", "Yellow Bird", "Lime Green", "Pea Green", "Green", "Dark Green", "Olive Green", "Midnight Blue", "Royal Blue", "Baby Blue", "Bright Blue", "Flourescent Blue", "Slate Blue", "Schafter Purple", "Midnight Purple" };


//Paint Values
const std::vector<std::string> VALUES_NORMAL{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74" };

const std::vector<std::string> VALUES_METALLIC{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74" };

const std::vector<std::string> VALUES_MATTE{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" };

const std::vector<std::string> VALUES_METAL{ "0", "1", "2", "3", "4" };

const std::vector<std::string> VALUES_CHROME{ "0" };

const std::vector<std::string> VALUES_WHEELS{ "5", "119", "156", "0", "1", "11", "2", "8", "122", "27", "30", "45", "35", "33", "136", "135", "36", "41", "138", "37", "99", "90", "95", "115", "109", "153", "154", "88", "89", "91", "55", "125", "53", "56", "151", "82", "64", "87", "70", "140", "81", "145", "142" };


const std::vector<std::string> VOV_PAINT_CAPTIONS[] = { CAPTIONS_METALLIC, CAPTIONS_NORMAL, CAPTIONS_MATTE, CAPTIONS_METAL, CAPTIONS_CHROME };
const std::vector<std::string> VOV_PAINT_VALUES[] = { VALUES_METALLIC, VALUES_NORMAL, VALUES_MATTE, VALUES_METAL, VALUES_CHROME };

const std::vector<std::string> VOV_SPECIAL_CAPTIONS[] = { CAPTIONS_METALLIC, CAPTIONS_WHEELS };
const std::vector<std::string> VOV_SPECIAL_VALUES[] = { VALUES_METALLIC, VALUES_WHEELS };


void onhighlight_livery(MenuItem<int> choice)
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
	{
		return;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("Player isn't in a vehicle");
		return;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	VEHICLE::SET_VEHICLE_LIVERY(veh, choice.value);
}

bool onconfirm_livery(MenuItem<int> choice)
{
	return true;
}

bool process_paint_menu_liveries()
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
	{
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	int count = VEHICLE::GET_VEHICLE_LIVERY_COUNT(veh);
	if (count <= 1)
	{
		set_status_text("No liveries for this vehicle");
	}

	std::vector<MenuItem<int>*> menuItems;

	for (int i = 0; i < count; i++)
	{
		std::string modItemNameStr;

		char* modItemNameChr = VEHICLE::GET_LIVERY_NAME(veh, i);
		if (modItemNameChr == NULL)
		{
			std::ostringstream ss;
			ss << "Livery #" << (i+1);
			modItemNameStr = ss.str();
		}
		else
		{
			modItemNameStr = std::string(modItemNameChr);
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = modItemNameStr;
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	int currentSelection = VEHICLE::GET_VEHICLE_LIVERY(veh);
	return draw_generic_menu<int>(menuItems, &currentSelection, "Liveries", onconfirm_livery, onhighlight_livery, NULL, vehicle_menu_interrupt);
}

bool onconfirm_paint_menu(MenuItem<int> choice)
{
	::whichpart = choice.value;
	if (whichpart == 3) //Wheels
	{
		process_paint_menu_special(1);
	}
	else if (whichpart == 2) //Pearl topcoat
	{
		process_paint_menu_special(0);
	}
	else if (whichpart == -1)
	{
		process_paint_menu_liveries();
	}
	else
	{
		process_paint_menu_type(); //Primary and Secondary Colors
	}
	return false;
}
bool process_paint_menu_special(int category)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	std::vector<MenuItem<std::string>*> menuItems;
	int paint1, paint2, paint3;
	VEHICLE::GET_VEHICLE_MOD_COLOR_1(veh, &paint1, &paint2, &paint3);
	int index = 0;

	if (paint1 == 3 || paint1 == 4 || paint1 == 5)
	{
		set_status_text("Pearl cannot be applied over this paint type");
		return false;
	}

	else
	{
		for (int i = 0; i < VOV_SPECIAL_VALUES[category].size(); i++)
		{
			MenuItem<std::string> *item = new MenuItem<std::string>();
			item->caption = VOV_SPECIAL_CAPTIONS[category][i];
			item->value = VOV_SPECIAL_VALUES[category][i];
			menuItems.push_back(item);
		}

		if (::whichpart == 2) //index as pearlescent top coat color
		{
			index = paint3;
			if (index > -1){ index = paint3 + 1; }
		}
	}
	return draw_generic_menu<std::string>(menuItems, &index, "Select Color", onconfirm_color_menu_selection, onhighlight_color_menu_selection, NULL);
}
bool process_paint_menu()
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists)
	{
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	int liveryCount = VEHICLE::GET_VEHICLE_LIVERY_COUNT(veh);

	std::vector<MenuItem<int>*> menuItems;

	for (int i = 0; i < MENU_PAINT_WHAT.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_PAINT_WHAT[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	if (liveryCount > 1)
	{
		std::ostringstream ss;
		ss << "Liveries (" << liveryCount << ")";
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = ss.str();
		item->value = -1;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Choose which part to paint", onconfirm_paint_menu, NULL, NULL, vehicle_menu_interrupt);
}

bool onconfirm_paint_menu_type(MenuItem<int> choice)
{
	std::string category = choice.caption;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	int index = 0;

	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_PAINT_VALUES[choice.value].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_PAINT_CAPTIONS[choice.value][i];
		item->value = VOV_PAINT_VALUES[choice.value][i];
		menuItems.push_back(item);
	}

	::whichtype = choice.value; //save paint type for later
	if (whichtype > 1) { ::whichtype = whichtype + 1; }

	if (::whichpart == 0) //index as primary color
	{
		int paint1, paint2, paint3;
		VEHICLE::GET_VEHICLE_MOD_COLOR_1(veh, &paint1, &paint2, &paint3);
		index = paint2;
	}
	else if (::whichpart == 1) //index as secondary color
	{
		int paint1, paint2;
		VEHICLE::GET_VEHICLE_MOD_COLOR_2(veh, &paint1, &paint2);
		index = paint2;
	}

	return draw_generic_menu<std::string>(menuItems, &index, category, onconfirm_color_menu_selection, onhighlight_color_menu_selection, NULL, vehicle_menu_interrupt);
}

bool process_paint_menu_type()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	int index = 0;

	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < MENU_PAINT_TYPE.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_PAINT_TYPE[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	if (::whichpart == 0) //index as primary color type
	{
		int paint1, paint2, paint3;
		VEHICLE::GET_VEHICLE_MOD_COLOR_1(veh, &paint1, &paint2, &paint3);
		index = paint1;
		if (index == 2) { index = 0; } //if paint type is pearlescent index to paint type metallic
		else if (index > 2) { index = index - 1; }
	}
	else if (::whichpart == 1) //index as secondary color type
	{
		int paint1, paint2;
		VEHICLE::GET_VEHICLE_MOD_COLOR_2(veh, &paint1, &paint2);
		index = paint1;
		if (index > 1) { index = index - 1; }
	}

	return draw_generic_menu<int>(menuItems, &index, "Choose Paint Type", onconfirm_paint_menu_type, NULL, NULL, vehicle_menu_interrupt);
}

void onhighlight_color_menu_selection(MenuItem<std::string> choice)
{
	onconfirm_color_menu_selection(choice);
}

bool onconfirm_color_menu_selection(MenuItem<std::string> choice)
{
	int whichpaint = std::stoi(choice.value);
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

			if (::whichpart == 0) //Apply primary Color
			{
				VEHICLE::SET_VEHICLE_MOD_COLOR_1(veh, ::whichtype, whichpaint, -1);
			}
			else if (::whichpart == 1) //apply secondary Color
			{
				VEHICLE::SET_VEHICLE_MOD_COLOR_2(veh, ::whichtype, whichpaint);
			}
			else if (::whichpart == 2) //Apply pearl Topcoat
			{
				int paint1, paint2, paint3;
				VEHICLE::GET_VEHICLE_MOD_COLOR_1(veh, &paint1, &paint2, &paint3);
				VEHICLE::SET_VEHICLE_MOD_COLOR_1(veh, 2, paint2, whichpaint - 1); //change paint type to pearl and apply topcoat
			}
			else if (::whichpart == 3) //Apply wheel color
			{
				int paint1, paint2;//pearl topcoat, wheel color
				VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &paint1, &paint2);
				VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, paint1, whichpaint); //apply wheel color without changing pearl topcoat
			}

		}
		else
		{
			set_status_text("Player isn't in a vehicle");
		}
	}
	return true;
}