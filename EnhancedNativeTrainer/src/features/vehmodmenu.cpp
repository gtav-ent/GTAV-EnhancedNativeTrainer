/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "vehicles.h"

int activeLineIndexVehMod = 0;

int lastSelectedModValue = 0;

int modChoiceMenuIndex = 0;

const static int CUSTOM_TYRE_COUNT = 1;

const static int WHEEL_CATEGORY_COUNT = 7;

const static std::string WHEEL_CATEGORY_NAMES[] = { "Sports", "Muscle", "Lowrider", "SUV", "Offroad", "Tuner", "High End" };

const static int WHEEL_CATEGORY_COUNTS[] = { 25, 18, 15, 19, 10, 24, 20 };

const static int WHEEL_CATEGORY_COUNT_BIKE = 13;

const static std::string TINT_NAMES[] = { "No Tint", "Dark", "Medium", "Light", "Very Light", "Safety Value" };

const static std::string PLATE_NAMES[] = { "Blue on White", "Yellow/Black", "Gold/Blue", "Blue/White SA Caps", "Blue/White SA Exempt", "Blue/White Yankton"};

const static int SPECIAL_ID_START = 90;

const static int SPECIAL_ID_FOR_WHEEL_CATEGORY = 91;

const static int SPECIAL_ID_FOR_WHEEL_SELECTION = 92;

const static int SPECIAL_ID_FOR_WINDOW_TINT = 93;

const static int SPECIAL_ID_FOR_LICENSE_PLATES = 94;

const static int SPECIAL_ID_FOR_TOGGLE_VARIATIONS = 95;

const static int SPECIAL_ID_FOR_PLATE_TEXT = 96;

std::string getModCategoryName(int i)
{
	switch (i)
	{
	case 0:
		return "Spoiler";
	case 1:
		return "Front Bumper";
	case 2:
		return "Rear Bumper";
	case 3:
		return "Side Skirts";
	case 4:
		return "Exhaust";
	case 5:
		return "Rollcage";
	case 6:
		return "Grille";
	case 7:
		return "Bonnet";
	case 8:
		return "Fenders/Arches";
	case 10:
		return "Roof";
	case 11:
		return "Engine";
	case 12:
		return "Brakes";
	case 13:
		return "Transmission";
	case 14:
		return "Horn";
	case 15:
		return "Suspension";
	case 16:
		return "Armor";
	case 22:
		return "Headlights";
	case SPECIAL_ID_FOR_WHEEL_CATEGORY:
		return "Wheel Category";
	case SPECIAL_ID_FOR_WHEEL_SELECTION:
		return "Wheel Choice";
	case SPECIAL_ID_FOR_WINDOW_TINT:
		return "Window Tint";
	case SPECIAL_ID_FOR_LICENSE_PLATES:
		return "License Plates";
	default:
		return std::to_string(i);
	}
}

std::string geSpecialItemTitle(int category, int index)
{
	switch (category)
	{
	case SPECIAL_ID_FOR_LICENSE_PLATES:
		return PLATE_NAMES[index];

	case SPECIAL_ID_FOR_WHEEL_CATEGORY:
		return WHEEL_CATEGORY_NAMES[index];

	case SPECIAL_ID_FOR_WINDOW_TINT:
		return TINT_NAMES[index];
	}
	return std::string();
}

const static std::string BRAKES_AND_TRANS_PREFIXES[] = { "Street", "Sports", "Race" };

const static std::string SUSP_PREFIXES[] = { "Lowered", "Street", "Sports", "Competition", "Race", "Extra Low" };

std::string getHornTitle(int index)
{
	char* v_3 = NULL;
	switch (index)
	{
	case -1: v_3 = "CMOD_HRN_0"; break;
	case 0: v_3 = "CMOD_HRN_TRK"; break;
	case 1: v_3 = "CMOD_HRN_COP"; break;
	case 2:  v_3 = "CMOD_HRN_CLO";  break;
	case 3: v_3 = "CMOD_HRN_MUS1"; break;
	case 4: v_3 = "CMOD_HRN_MUS2"; break;
	case 5: v_3 = "CMOD_HRN_MUS3"; break;
	case 6: v_3 = "CMOD_HRN_MUS4"; break;
	case 7: v_3 = "CMOD_HRN_MUS5"; break;
	case 8: v_3 = "CMOD_HRN_SAD"; break;
	case 9: v_3 = "HORN_CLAS1"; break;
	case 10: v_3 = "HORN_CLAS2"; break;
	case 11: v_3 = "HORN_CLAS3"; break;
	case 12: v_3 = "HORN_CLAS4"; break;
	case 13: v_3 = "HORN_CLAS5"; break;
	case 14: v_3 = "HORN_CLAS6"; break;
	case 15: v_3 = "HORN_CLAS7"; break;
	case 16: v_3 = "HORN_CNOTE_C0"; break;
	case 17: v_3 = "HORN_CNOTE_D0"; break;
	case 18: v_3 = "HORN_CNOTE_E0"; break;
	case 19: v_3 = "HORN_CNOTE_F0"; break;
	case 20: v_3 = "HORN_CNOTE_G0"; break;
	case 21: v_3 = "HORN_CNOTE_A0"; break;
	case 22: v_3 = "HORN_CNOTE_B0"; break;
	case 23: v_3 = "HORN_CNOTE_C1"; break;
	case 24: v_3 = "HORN_HIPS1"; break;
	case 25: v_3 = "HORN_HIPS2"; break;
	case 26: v_3 = "HORN_HIPS3"; break;
	case 27: v_3 = "HORN_HIPS4"; break;
	case 28: v_3 = "HORN_INDI_1"; break;
	case 29: v_3 = "HORN_INDI_2"; break;
	case 30: v_3 = "HORN_INDI_3"; break;
	case 31: v_3 = "HORN_INDI_4"; break;
	case 32: v_3 = "HORN_LUXE1"; break;
	case 33: v_3 = "HORN_LUXE2"; break;
	case 34: v_3 = "HORN_LUXE3"; break;
	}

	if (v_3 == NULL)
	{
		return "Unknown Horn";
	}
	else
	{
		char* label = UI::GET_LABEL_TEXT(v_3);
		if (label == NULL)
		{
			return "Unknown Horn";
		}
		return std::string(label);
	}
}

int getHornDuration(int index)
{
	int v_D = 0;

	switch (index)
	{
	case -1: v_D = 1000; break;
	case 28: v_D = 1960; break;
	case 29: v_D = 1790; break;
	case 30: v_D = 1990; break;
	case 31: v_D = 2400; break;
	case 24: v_D = 2000; break;
	case 25: v_D = 2000; break;
	case 26: v_D = 1500; break;
	case 27: v_D = 2500; break;
	case 16: v_D = 1000; break;
	case 17: v_D = 1000; break;
	case 18: v_D = 1000; break;
	case 19: v_D = 1000; break;
	case 20: v_D = 1000; break;
	case 21: v_D = 1000; break;
	case 22: v_D = 1000; break;
	case 23: v_D = 1000; break;
	case 9: v_D = 5500; break;
	case 10: v_D = 5500; break;
	case 11: v_D = 5500; break;
	case 12: v_D = 4500; break;
	case 13: v_D = 4500; break;
	case 14: v_D = 4500; break;
	case 15: v_D = 4500; break;
	case 2: v_D = 1000; break;
	case 1: v_D = 1000; break;
	case 0: v_D = 1000; break;
	case 3: v_D = 3500; break;
	case 4: v_D = 5500; break;
	case 5: v_D = 4500; break;
	case 6: v_D = 4500; break;
	case 7: v_D = 4500; break;
	case 8: v_D = 4500; break;
	default: v_D = 1000; break;
	}

	return v_D;
}

std::string getNormalItemTitle(Vehicle veh, int category, int index)
{
	//Engine stuff is EMS Upgrade, Level 1-4
	//Brakes/trans are stock, street, sports, race
	//Susp is stock,lowered,street,sport,competition
	//Armor is none, 20, 40, 60, 80, 100%

	std::string modItemNameStr;
	
	if (index == -1)
	{
		if (category == 16)
		{
			modItemNameStr = "No Armor";
		}
		else
		{
			std::ostringstream ss;
			ss << "Stock " << getModCategoryName(lastSelectedModValue);
			modItemNameStr = ss.str();
		}
	}
	else if (category == 11) //Engine
	{
		std::ostringstream ss;
		ss << "EMS Upgrade, Level " << (index + 1);
		modItemNameStr = ss.str();
	}
	else if (category == 12 || category == 13) //brakes, trans
	{
		std::ostringstream ss;
		ss << BRAKES_AND_TRANS_PREFIXES[index];
		if (category == 12)
		{
			ss << " Brakes";
		}
		else
		{
			ss << " Transmission";
		}
		modItemNameStr = ss.str();
	}
	else if (category == 14) //horns
	{
		modItemNameStr = getHornTitle(index);
	}
	else if (category == 15) //suspension
	{
		std::ostringstream ss;
		ss << SUSP_PREFIXES[index] << " Suspension";
		modItemNameStr = ss.str();
	}
	else if (category == 16) //Armor
	{
		std::ostringstream ss;
		ss << ((index + 1) * 20) << "% Armor";
		modItemNameStr = ss.str();
	}
	else
	{
		char* modItemNameChr = VEHICLE::GET_MOD_TEXT_LABEL(veh, category, index);
		bool foundName = false;
		if (modItemNameChr != NULL && strlen(modItemNameChr) > 0)
		{
			char* modItemNameTxt = UI::GET_LABEL_TEXT(modItemNameChr);
			if (modItemNameTxt != NULL)
			{
				modItemNameStr = std::string(modItemNameTxt);
				foundName = true;
			}
		}

		if (!foundName)
		{
			std::ostringstream ss;
			ss << getModCategoryName(lastSelectedModValue) << " Item " << (index+1);
			modItemNameStr = ss.str();
		}
	}

	return modItemNameStr;
}

//std::string HORN_NAMES[] = { "Stock Horn", "Star Spangled Banner 1", "Star Spangled Banner 1", "Star Spangled Banner 1", "Star Spangled Banner 1", "Jazz Horn 1", };

bool onconfirm_vehmod_category_menu(MenuItem<int> choice)
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

	switch (lastSelectedModValue)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 21:
	case 22:
	case 23:
	case 24:

	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

		std::string modItemNameStr = getNormalItemTitle(veh, lastSelectedModValue, choice.value);

		VEHICLE::SET_VEHICLE_MOD(veh, lastSelectedModValue, choice.value, 1);
		std::ostringstream ss;
		ss << modItemNameStr << " Applied";
		set_status_text(ss.str());

		/*
		if (lastSelectedModValue == 14)
		{
			STREAMING::STOP_PLAYER_REMAIN_ARCADE();
			PED::SET_PED_RESET_FLAG(PLAYER::PLAYER_PED_ID(), 102, 1);

			Hash hashOfHorn = VEHICLE::_0x4593CF82AA179706(veh, 14, VEHICLE::GET_VEHICLE_MOD(veh, 14));
			//AI::TASK_VEHICLE_TEMP_ACTION(PLAYER::PLAYER_PED_ID(), veh, actionID++, 4000);

			AI::_0xCC665AAC360D31E7(PLAYER::PLAYER_PED_ID(), veh, true);
			//AI::_0xCC665AAC360D31E7(PLAYER::PLAYER_PED_ID(), veh, true);

			//AUDIO::OVERRIDE_VEH_HORN(veh, 1, AUDIO::GET_VEHICLE_DEFAULT_HORN(veh));
			//VEHICLE::START_VEHICLE_HORN(veh, getHornDuration(choice.value), GAMEPLAY::GET_HASH_KEY("HELDDOWN"), false);
		}
		*/
	}
	break;

	case SPECIAL_ID_FOR_WINDOW_TINT: //Change Window Tint
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, choice.value); //Start from beginning
		set_status_text("Changed Window Tint");
		/*
		std::ostringstream ss;
		ss << "Set tint value " << choice.value;
		set_status_text(ss.str());
		*/
	}
	break;

	case SPECIAL_ID_FOR_LICENSE_PLATES: //Change license plate style
	{
		int plateCount = VEHICLE::GET_NUMBER_OF_VEHICLE_NUMBER_PLATES();
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, choice.value); //Start from beginning
		set_status_text("Changed License Plate");
	}
	break;

	case SPECIAL_ID_FOR_WHEEL_CATEGORY: //Change Wheel Category
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, choice.value); //Increment ModValue
		VEHICLE::SET_VEHICLE_MOD(veh, 23, 0, 0);
		set_status_text("Changed Wheel Category");
	}
	break;

	case SPECIAL_ID_FOR_WHEEL_SELECTION: //Change Wheels 
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, choice.value, 0); //Remove mod and start from beginning
		VEHICLE::SET_VEHICLE_MOD(veh, 24, choice.value, 0); //Remove mod and start from beginning (For bike rear wheels if they exist)
		set_status_text("Changed Wheels");
	}
	break;
	}
	return false;
}

bool process_vehmod_category_special_menu(int category)
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	std::vector<int> values;

	switch (category)
	{
	case SPECIAL_ID_FOR_LICENSE_PLATES:
	{
		int plateCount = VEHICLE::GET_NUMBER_OF_VEHICLE_NUMBER_PLATES();
		for (int i = 0; i < plateCount; i++)
		{
			values.push_back(i);
		}
	}
		break;
	case SPECIAL_ID_FOR_WHEEL_CATEGORY:
	{
		for (int i = 0; i < WHEEL_CATEGORY_COUNT; i++)
		{
			int j = i;
			if (j >= 6)
			{
				j = j + 1; //skip 6
			}
			values.push_back(j);
		}
	}
		break;
	case SPECIAL_ID_FOR_WINDOW_TINT:
	{
		int tintCount = VEHICLE::GET_NUM_VEHICLE_WINDOW_TINTS();
		for (int i = 0; i < tintCount; i++)
		{
			values.push_back(i);
		}
	}
		break;
	default:
		return false;
	}

	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < values.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		std::string specialName = geSpecialItemTitle(category, i);
		if (!specialName.empty())
		{
			item->caption = specialName;
		}
		else if (i == 0 && values.at(i) == -1)
		{
			item->caption = "Default";
		}
		else
		{
			std::ostringstream ss;
			ss << getModCategoryName(category) << " Item " << i;
			item->caption = ss.str();
		}
		item->value = values.at(i);
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	//Find menu index to return to
	int modChoiceMenuIndex = find_menu_index_to_restore(category, category, veh);

	std::ostringstream ss;
	ss << getModCategoryName(category);

	draw_generic_menu<int>(menuItems, &modChoiceMenuIndex, ss.str(), onconfirm_vehmod_category_menu, NULL, NULL, vehicle_menu_interrupt);

	return false;
}

bool process_vehmod_category_menu(int category)
{
	int actualCategory = category;

	if (category == SPECIAL_ID_FOR_WHEEL_SELECTION)
	{
		actualCategory = 23;
	}
	else if (category > SPECIAL_ID_START)
	{
		return process_vehmod_category_special_menu(category);
	}

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
	std::vector<MenuItem<int>*> menuItems;

	int count = 0;
	if (category == SPECIAL_ID_FOR_WHEEL_SELECTION)
	{
		int wheelType = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
		if (wheelType == 6)
		{
			count = WHEEL_CATEGORY_COUNT_BIKE;
		}
		else if ( wheelType == 7)
		{
			count = WHEEL_CATEGORY_COUNTS[6];
		}
		else
		{
			count = WHEEL_CATEGORY_COUNTS[wheelType];
		}
	}
	else
	{
		count = VEHICLE::GET_NUM_VEHICLE_MODS(veh, actualCategory);
	}

	if (category == SPECIAL_ID_FOR_WHEEL_SELECTION)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = "Default Wheel For Vehicle";
		item->value = -1;
		item->isLeaf = true;
		menuItems.push_back(item);
	}
	else
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = getNormalItemTitle(veh, category, -1);
		item->value = -1;
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	for (int i = 0; i < count; i++)
	{
		if (!(category == 14 && i > 34))
		{
			std::string modItemNameStr = getNormalItemTitle(veh, actualCategory, i);
			MenuItem<int> *item = new MenuItem<int>();
			item->caption = modItemNameStr;
			item->value = i;
			item->isLeaf = true;
			menuItems.push_back(item);
		}
	}

	//Find menu index to return to
	int modChoiceMenuIndex = find_menu_index_to_restore(category, actualCategory, veh);

	std::ostringstream ss;
	ss << getModCategoryName(lastSelectedModValue);

	draw_generic_menu<int>(menuItems, &modChoiceMenuIndex, ss.str(), onconfirm_vehmod_category_menu, NULL, NULL, vehicle_menu_interrupt);
	return false;
}

int find_menu_index_to_restore(int category, int actualCategory, Vehicle veh)
{
	int modChoiceMenuIndex = 0;

	if (category == SPECIAL_ID_FOR_WHEEL_CATEGORY)
	{
		modChoiceMenuIndex = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
		if (modChoiceMenuIndex > 6)
		{
			modChoiceMenuIndex--;
		}
	}
	else if (category == SPECIAL_ID_FOR_LICENSE_PLATES)
	{
		modChoiceMenuIndex = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh);
	}
	else if (category == SPECIAL_ID_FOR_WINDOW_TINT)
	{
		modChoiceMenuIndex = VEHICLE::GET_VEHICLE_WINDOW_TINT(veh);
		/*
		std::ostringstream ss;
		ss << "Retrieved tint value " << modChoiceMenuIndex;
		set_status_text(ss.str());
		*/
	}
	else
	{
		modChoiceMenuIndex = VEHICLE::GET_VEHICLE_MOD(veh, actualCategory);
		modChoiceMenuIndex++; //to compensate for extra 'default' item
	}
	return modChoiceMenuIndex;
}

bool onconfirm_vehmod_menu(MenuItem<int> choice)
{
	lastSelectedModValue = choice.value;

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

	switch (choice.value)
	{
	case -1: //Upgrade Performance
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 11, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 11) - 1, 1); //Engine
		VEHICLE::SET_VEHICLE_MOD(veh, 12, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 12) - 1, 1); //Brakes
		VEHICLE::SET_VEHICLE_MOD(veh, 13, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 13) - 1, 1); //Transmission
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, 1); //Turbo Tuning
		set_status_text("Added All Performance Upgrades");
		break;

	case -2: //Upgrade Armor and Tires

		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 16, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 16) - 1, 1); //Armor
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0); //Bulletproof Tires
		set_status_text("Added All Armor Upgrades and Bulletproof Tires");
		break;

	case -3: //Remove All Mods

		for (int i = 0; i < 25; i++)
		{
			VEHICLE::REMOVE_VEHICLE_MOD(veh, i);
		}
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 0);
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 1);
		set_status_text("Removed All Upgrades");
		break;

	case  SPECIAL_ID_FOR_TOGGLE_VARIATIONS:
		//these are toggles, do nothing
		return false;

	default:
		process_vehmod_category_menu(choice.value);
		break;
	}
	return false;
}


bool process_vehmod_menu()
{
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
	{
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

	Entity et = ENTITY::GET_ENTITY_MODEL(veh);

	BOOL isABike = VEHICLE::IS_THIS_MODEL_A_BIKE(et);
	BOOL isAircraft = VEHICLE::IS_THIS_MODEL_A_HELI(et) || VEHICLE::IS_THIS_MODEL_A_PLANE(et);
	BOOL isWeird = VEHICLE::IS_THIS_MODEL_A_TRAIN(et) || VEHICLE::IS_THIS_MODEL_A_BICYCLE(et) || VEHICLE::IS_THIS_MODEL_A_BOAT(et);

	std::string caption = "Vehicle Mod Options";

	std::vector<MenuItem<int>*> menuItems;

	if (!isWeird && !isAircraft)
	{
		MenuItem<int> *item1 = new MenuItem<int>();
		item1->caption = "Add All Performance Upgrades";
		item1->value = -1;
		item1->isLeaf = true;
		menuItems.push_back(item1);

		MenuItem<int> *item2 = new MenuItem<int>();
		item2->caption = "Add All Armor Upgrades";
		item2->value = -2;
		item2->isLeaf = true;
		menuItems.push_back(item2);

		MenuItem<int> *item3 = new MenuItem<int>();
		item3->caption = "Remove All Upgrades";
		item3->value = -3;
		item3->isLeaf = true;
		menuItems.push_back(item3);
	}

	if (!isWeird && !isAircraft)
	{
		for (int i = 0; i < 30; i++)
		{
			if (i == 23 || i == 24 || i == 21)
			{
				continue;
			}

			bool iFound = false;
			int compIndex = i;

			int mods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, i);
			if (mods > 0)
			{
				std::ostringstream ss;
				//ss << "Slot " << (compIndex + 1) << ": ";
				ss << getModCategoryName(i) << " ~HUD_COLOUR_GREYLIGHT~(" << (mods + 1) << ")";

				MenuItem<int> *item = new MenuItem<int>();
				item->caption = ss.str();
				item->value = compIndex;
				item->isLeaf = false;
				menuItems.push_back(item);
			}
		}
	}

	std::ostringstream ss;

	if (!isWeird && !isAircraft)
	{
		int tintCount = VEHICLE::GET_NUM_VEHICLE_WINDOW_TINTS();
		MenuItem<int> *item = new MenuItem<int>();
		ss << getModCategoryName(SPECIAL_ID_FOR_WINDOW_TINT) << " ~HUD_COLOUR_GREYLIGHT~(" << tintCount << ")";
		item->caption = ss.str();
		item->value = SPECIAL_ID_FOR_WINDOW_TINT;
		item->isLeaf = false;
		menuItems.push_back(item);

		ss.str(""); ss.clear();

		int plateCount = VEHICLE::GET_NUMBER_OF_VEHICLE_NUMBER_PLATES();
		item = new MenuItem<int>();
		ss << getModCategoryName(SPECIAL_ID_FOR_LICENSE_PLATES) << " ~HUD_COLOUR_GREYLIGHT~(" << plateCount << ")";
		item->caption = ss.str();
		item->value = SPECIAL_ID_FOR_LICENSE_PLATES;
		item->isLeaf = false;
		menuItems.push_back(item);

		ss.str(""); ss.clear();

		if (!VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(veh)))
		{
			item = new MenuItem<int>();
			ss << getModCategoryName(SPECIAL_ID_FOR_WHEEL_CATEGORY) << " ~HUD_COLOUR_GREYLIGHT~(" << WHEEL_CATEGORY_COUNT << ")";
			item->caption = ss.str();
			item->value = SPECIAL_ID_FOR_WHEEL_CATEGORY;
			item->isLeaf = false;
			menuItems.push_back(item);

			ss.str(""); ss.clear();
		}

		int wheelCount = 0;
		int wheelType = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
		if (wheelType == 6)
		{
			wheelCount = WHEEL_CATEGORY_COUNT_BIKE;
		}
		else if (wheelType == 7)
		{
			wheelCount = WHEEL_CATEGORY_COUNTS[6];
		}
		else
		{
			wheelCount = WHEEL_CATEGORY_COUNTS[wheelType];
		}

		item = new MenuItem<int>();
		ss << getModCategoryName(SPECIAL_ID_FOR_WHEEL_SELECTION) << " ~HUD_COLOUR_GREYLIGHT~(" << wheelCount << ")";
		item->caption = ss.str();
		item->value = SPECIAL_ID_FOR_WHEEL_SELECTION;
		item->isLeaf = false;
		menuItems.push_back(item);

		ss.str(""); ss.clear();
	}

	FunctionDrivenToggleMenuItem<int> *toggleItem;

	if (!isWeird && !isAircraft)
	{
		toggleItem = new FunctionDrivenToggleMenuItem<int>();
		toggleItem->caption = "Toggle Turbo Tuning";
		toggleItem->getter_call = is_turbocharged;
		toggleItem->setter_call = set_turbocharged;
		toggleItem->value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
		menuItems.push_back(toggleItem);

		toggleItem = new FunctionDrivenToggleMenuItem<int>();
		toggleItem->caption = "Toggle Xenon Lights";
		toggleItem->getter_call = is_xenon_headlights;
		toggleItem->setter_call = set_xenon_headlights;
		toggleItem->value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
		menuItems.push_back(toggleItem);

		toggleItem = new FunctionDrivenToggleMenuItem<int>();
		toggleItem->caption = "Toggle Neon Lights";
		toggleItem->getter_call = is_neon_lights_enabled;
		toggleItem->setter_call = set_neon_lights_enabled;
		toggleItem->value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
		menuItems.push_back(toggleItem);

		toggleItem = new FunctionDrivenToggleMenuItem<int>();
		toggleItem->caption = "Toggle Bulletproof Tires";
		toggleItem->getter_call = is_bulletproof_tyres;
		toggleItem->setter_call = set_bulletproof_tyres;
		toggleItem->value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
		menuItems.push_back(toggleItem);

		if (!isWeird && !isAircraft && !isABike)
		{
			toggleItem = new FunctionDrivenToggleMenuItem<int>();
			toggleItem->caption = "Toggle Custom Tires";
			toggleItem->getter_call = is_custom_tyres;
			toggleItem->setter_call = set_custom_tyres;
			toggleItem->value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
			menuItems.push_back(toggleItem);
		}
	}

	for (int i = 1; i < 10; i++)
	{
		if (!VEHICLE::DOES_EXTRA_EXIST(veh, i))
		{
			continue;
		}

		ss << "Toggle Extra #" << i;
		toggleItem = new FunctionDrivenToggleMenuItem<int>();
		toggleItem->caption = ss.str();
		toggleItem->getter_call = is_extra_enabled;
		toggleItem->setter_call = set_extra_enabled;
		toggleItem->value = SPECIAL_ID_FOR_TOGGLE_VARIATIONS;
		toggleItem->extra_arguments.push_back(i);
		menuItems.push_back(toggleItem);
		ss.str(""); ss.clear();
	}

	if (!isWeird && !isAircraft)
	{
		MenuItem<int>* item = new MenuItem<int>();
		item->caption = "Change Plate Text";
		item->isLeaf = false;
		item->onConfirmFunction = set_plate_text;
		item->value = SPECIAL_ID_FOR_PLATE_TEXT;
		menuItems.push_back(item);
	}

	if (menuItems.size() == 0)
	{
		set_status_text("No relevant mods for this vehicle");
		return false;
	}

	draw_generic_menu<int>(menuItems, 0, "Vehicle Mods", onconfirm_vehmod_menu, NULL, NULL, vehicle_menu_interrupt);
	return false;
}

void set_plate_text(MenuItem<int> choice)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	char* existingText = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
	std::string result = show_keyboard("CMOD_MOD_18_D", existingText);
	if (!result.empty())
	{
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, (char*) result.c_str());
	}
}

bool is_custom_tyres(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int tyreCount = VEHICLE::GET_VEHICLE_MOD_VARIATION(veh, 23);
	return (tyreCount != 0);
}

void set_custom_tyres(bool applied, std::vector<int> extras)
{
	if (applied)
	{
		set_status_text("CTs applied");
	}
	else
	{
		set_status_text("CTs unapplied");
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
	int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 23);
	VEHICLE::SET_VEHICLE_MOD(veh, 23, currmod, applied); //Add Custom Tires
	VEHICLE::SET_VEHICLE_MOD(veh, 24, currmod, applied); //Add Custom Tires (For bike rear wheels if they exist)
	set_status_text("Changed Tires");
}

bool is_turbocharged(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	return VEHICLE::IS_TOGGLE_MOD_ON(veh, 18) ? true : false;
}

void set_turbocharged(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, applied); //Turbo Tuning
}

bool is_bulletproof_tyres(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	return VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(veh) ? false : true;
}

void set_bulletproof_tyres(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, !applied); //Bulletproof Tires
}

bool is_extra_enabled(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int extraIndex = extras.at(0);
	return VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(veh, extraIndex) ? true : false;
}

void set_extra_enabled(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int extraIndex = extras.at(0);

	/*std::ostringstream ss;
	ss << "Asked for extra at " << extraIndex << " and state " << (applied? "true" : "false");
	set_status_text(ss.str());*/

	VEHICLE::SET_VEHICLE_EXTRA(veh, extraIndex, applied ? 0 : -1);
}

bool is_xenon_headlights(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	return VEHICLE::IS_TOGGLE_MOD_ON(veh, 22) ? true : false;
}

void set_xenon_headlights(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, applied); //Headlights
}

bool is_neon_lights_enabled(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	for (int i = 0; i < 3; i++)
	{
		return VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, i) ? true : false;
	}
}

int RandomRGB()
{
	return rand() % 255;
}

void set_neon_lights_enabled(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	for (int i = 0; i < 3; i++)
	{
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, i, applied);
	}

	if (applied) VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, RandomRGB(), RandomRGB(), RandomRGB());
}

bool vehicle_menu_interrupt()
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
		return true;
	}

	return false;
}

void set_chrome_wheels_enabled(Vehicle veh, bool enabled)
{
	int count;
	int wheelType = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
	if (wheelType == 6)
	{
		count = WHEEL_CATEGORY_COUNT_BIKE;
	}
	else if (wheelType == 7)
	{
		count = WHEEL_CATEGORY_COUNTS[6];
	}
	else
	{
		count = WHEEL_CATEGORY_COUNTS[wheelType];
	}

	int curWheel = VEHICLE::GET_VEHICLE_MOD(veh, 23);
	if (curWheel == -1)
	{
		return;
	}

	int newWheel;
	if (curWheel > count && !enabled)
	{
		newWheel = curWheel - count;
	}
	else if (curWheel < count && enabled)
	{
		newWheel = curWheel + count;
	}
	else
	{
		newWheel = curWheel;
	}

	if (newWheel != curWheel)
	{
		VEHICLE::SET_VEHICLE_MOD(veh, 23, newWheel, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 24, newWheel, 0);
	}
}