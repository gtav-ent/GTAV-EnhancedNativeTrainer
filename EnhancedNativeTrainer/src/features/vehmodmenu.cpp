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

const static int WHEEL_CATEGORY_COUNT = 8; //was 7

const static std::string WHEEL_CATEGORY_NAMES[] = { "Sports", "Muscle", "Lowrider", "SUV", "Offroad", "Tuner", "High End", "Benny's Originals" };

const static int WHEEL_CATEGORY_COUNTS[] = { 25, 18, 15, 19, 10, 24, 20, 31 }; /* SORTED */

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

const static int SPECIAL_ID_FOR_NEON_LIGHTS = 97;

const static int SPECIAL_ID_FOR_ORNAMENTS = 98; // we may or may not need this, can't figure out if we will or not

const static int SPECIAL_ID_FOR_TIRE_SMOKE = 99;

const static int SPECIAL_ID_FOR_INTERIOR_COLOUR = 100;

/*
static Hash SUPPORTS_DIALSANDTRIM[] = { VEHICLE_BANSHEE2, VEHICLE_BUCCANEER2, VEHICLE_CHINO2, VEHICLE_FACTION2, VEHICLE_MOONBEAM2, VEHICLE_SULTANRS, VEHICLE_PRIMO2, VEHICLE_VOODOO };

static Hash LOWRIDER_VEHS[] = { VEHICLE_BUCCANEER2, VEHICLE_CHINO2, VEHICLE_FACTION2, VEHICLE_MOONBEAM2, VEHICLE_PRIMO2, VEHICLE_VOODOO };

static Hash SUPPORTS_TRIM[] = { VEHICLE_SCHAFTER3, VEHICLE_SCHAFTER4, VEHICLE_SCHAFTER5, VEHICLE_SCHAFTER6, VEHICLE_COG55, VEHICLE_COG552, VEHICLE_COGNOSCENTI, VEHICLE_COGNOSCENTI2, VEHICLE_BALLER3, VEHICLE_BALLER4, VEHICLE_BALLER5, VEHICLE_BALLER6 };

*/

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
	case 9:
		return "Skirts";
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
	case 25: 
		return "Plate Holder";
	case 26:
		return "Vanity Plates";
	case 27:
		return "Trim Design";
	case 28:
		return "Ornaments";
	case 29:
		return "Dashboard"; //The Banshee has a different interior Index than the lowriders
	case 30:
		return "Dials";
	case 31:
		return "Door Cards";
	case 32:
		return "Seats";
	//case 19:
		//return "Headlights";
	case 33:
		return "Steering Wheels";
	case 34:
		return "Gear Sticks";
	case 35:
		return "Plaques";
	case 36:
		return "Shelf Speakers";
	case 37:
		return "Trunk Speakers";
	case 38:
		return "Hydraulics";
	case 39:
		return "Engine Block";
	case 40:
		return "Misc Engine";
	case 41:
		return "Strut Braces";
	case 42:
		return "Arch Covers";
	case 43:
		return "Foglamps";
	case 44:
		return "Exterior Extra";
	case 45:
		return "Fuel Tank";
	case 46:
		return "Doors Extra";
	case 48:
		return "Liveries";
	case SPECIAL_ID_FOR_WHEEL_CATEGORY:
		return "Wheel Category";
	case SPECIAL_ID_FOR_WHEEL_SELECTION:
		return "Wheel Choice";
	case SPECIAL_ID_FOR_WINDOW_TINT:
		return "Window Tint";
	case SPECIAL_ID_FOR_LICENSE_PLATES:
		return "License Plates";
	//case 50:
		//return "case 50";
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
	case 32: v_3 = "HORN_LUXE2"; break; //Classical Horn Loop 1
	case 33: v_3 = "HORN_LUXE1"; break; //Classical Horn 8
	case 34: v_3 = "HORN_LUXE3"; break; //Classical Horn Loop 2
	case 35: return "Classical Horn Loop 1 Preview"; break;
	case 36: return "Classical Horn 8 Preview"; break;
	case 37: return "Classical Horn Loop 2 Preview"; break;
	case 38: v_3 = "HORN_HWEEN1"; break; //Halloween Loop 1
	case 39: return "Halloween Loop 1 Preview"; break;
	case 40: v_3 = "HORN_HWEEN2"; break; //Halloween Loop 2
	case 41: return "Halloween Loop 2 Preview"; break;
	case 42: v_3 = "HORN_LOWRDER1"; break; //San Andreas Loop
	case 43: return "San Andreas Loop Preview"; break;
	case 44: v_3 = "HORN_LOWRDER2"; break; //Liberty City Loop
	case 45: return "Liberty City Loop Preview"; break;
	case 46: v_3 = "HORN_XM15_1"; break; //Festive Loop 1
	case 47: return "Festive Bells 1"; break;
	case 48: v_3 = "HORN_XM15_2"; break; //Festive Loop 2
	case 49: return "Festive Bells 2"; break;
	case 50: v_3 = "HORN_XM15_3"; break; //Festive Loop 3
	case 51: return "Festive Bells 3"; break;
	}

	if (v_3 == NULL)
	{
		return "Unknown Horn";
	}
	else
	{
		char* label = UI::_GET_LABEL_TEXT(v_3);
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
	case 32: v_D = 1000; break;
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
	{ //could this be the reason ornaments are 28? 
		char* modItemNameChr = VEHICLE::GET_MOD_TEXT_LABEL(veh, category, index);
		bool foundName = false;
		if (modItemNameChr != NULL && strlen(modItemNameChr) > 0)
		{
			char* modItemNameTxt = UI::_GET_LABEL_TEXT(modItemNameChr);
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

void addClanLogoToVehicle(Vehicle vehicle, Ped ped)
{
	Vector3 x, y, z;
	float scale;
	Hash modelHash = ENTITY::GET_ENTITY_MODEL(vehicle);
	if (GetVehicleInfoForClanLogo(modelHash, x, y, z, scale))
	{
		int alpha = 200;
		if (modelHash == VEHICLE_WINDSOR)
			alpha = 255;
		GRAPHICS::_ADD_CLAN_DECAL_TO_VEHICLE(vehicle, ped, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(vehicle, "chassis_dummy"), x.x, x.y, x.z, y.x, y.y, y.z, z.x, z.y, z.z, scale, 0, alpha);
	}
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
		set_status_text("~r~Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	switch (lastSelectedModValue) //is this why Benny's mods aren't applying? It should go up to ~ 48
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
	case 9:
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
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 48:
		
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
		set_status_text("Changed window tint");
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
		set_status_text("Changed license plate");
	}
	break;

	case SPECIAL_ID_FOR_WHEEL_CATEGORY: //Change Wheel Category
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, choice.value); //Increment ModValue
		VEHICLE::SET_VEHICLE_MOD(veh, 23, 0, 0);
		set_status_text("Changed wheel category");
	}
	break;

	case SPECIAL_ID_FOR_WHEEL_SELECTION: //Change Wheels 
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, choice.value, 0); //Remove mod and start from beginning
		VEHICLE::SET_VEHICLE_MOD(veh, 24, choice.value, 0); //Remove mod and start from beginning (For bike rear wheels if they exist)
		set_status_text("Changed wheels");
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
		set_status_text("~r~Player isn't in a vehicle");
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
		else if (wheelType == 8) //Benny's Originals
		{
			count = WHEEL_CATEGORY_COUNTS[7];
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
		if (!(category == 14 && i > 52)) //34
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
		set_status_text("~r~Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	switch (choice.value)
	{
	case -1: //Upgrade Performance
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_ENGINE, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ENGINE) - 1, 1); //Engine
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_BRAKES, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_BRAKES) - 1, 1); //Brakes
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRANSMISSION, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRANSMISSION) - 1, 1); //Transmission
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_TURBO, 1); //Turbo Tuning
		set_status_text("Added all performance upgrades");
		break;

	case -2: //Upgrade Armor and Tires

		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_ARMOR, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ARMOR) - 1, 1); //Armor
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0); //Bulletproof Tires
		set_status_text("Added all armor upgrades and bulletproof tires");
		break;

	case -3: //Add All Mods Pimp My Ride

		fully_tune_vehicle(veh, false, false);

		set_status_text("Added all available upgrades");
		break;

	case -4: //Remove All Mods

		for (int i = 0; i < 25; i++)
		{
			VEHICLE::REMOVE_VEHICLE_MOD(veh, i);
		}
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 0);
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 1);
		set_status_text("Removed all upgrades");
		break;

	case -5: // Vehicle interior colors
		//process_interior_colour_menu();  -- gives linker error on compilation if this is not commented out
		process_dash_colour_menu();
		return false;

	case  SPECIAL_ID_FOR_TOGGLE_VARIATIONS:
		//these are toggles, do nothing
		return false;

	case SPECIAL_ID_FOR_NEON_LIGHTS:
		process_neon_lights_menu();
		return false;

	case SPECIAL_ID_FOR_TIRE_SMOKE:
		process_smoke_colour_menu();
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
		set_status_text("~r~Player isn't in a vehicle");
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
		item3->caption = "Add All Available Upgrades";
		item3->value = -3;
		item3->isLeaf = true;
		menuItems.push_back(item3);

		MenuItem<int> *item4 = new MenuItem<int>();
		item4->caption = "Remove All Upgrades";
		item4->value = -4;
		item4->isLeaf = true;
		menuItems.push_back(item4);

		Hash currVeh = ENTITY::GET_ENTITY_MODEL(veh);
		bool supports_trim_cols = false;

		for each (char* vehModel in TRIM_OR_DIAL_VEHS)
		{
			if (GAMEPLAY::GET_HASH_KEY(vehModel) == currVeh)
			{
				supports_trim_cols = true;
			}
		}

		if (supports_trim_cols)
		{
			MenuItem<int> *item5 = new MenuItem<int>();
			item5->caption = "Interior Colors";
			item5->value = -5;
			item5->isLeaf = false;
			menuItems.push_back(item5);
		}
	}

	if (!isWeird && !isAircraft)
	{
		for (int i = 0; i < 49; i++) //was 30
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
		else if (wheelType == 8) //Benny's Originals
		{
			wheelCount = WHEEL_CATEGORY_COUNTS[7];
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

	if (is_this_a_car(veh))
	{
		MenuItem<int>* item = new MenuItem<int>();
		item->caption = "Neon Lights Menu";
		item->value = SPECIAL_ID_FOR_NEON_LIGHTS;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	if (is_this_a_car(veh) || is_this_a_motorcycle(veh))
	{
		MenuItem<int>* item = new MenuItem<int>();
		item->caption = "Tire Smoke Menu";
		item->value = SPECIAL_ID_FOR_TIRE_SMOKE;
		item->isLeaf = false;
		menuItems.push_back(item);
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

	for (int i = 1; i < 12; i++)
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
	set_status_text("Changed tires");
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


void fully_tune_vehicle(Vehicle veh, bool repaint, bool optics)
{
	VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
	VEHICLE::SET_VEHICLE_MOD(veh, MOD_ENGINE, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ENGINE) - 1, 1); //Engine
	VEHICLE::SET_VEHICLE_MOD(veh, MOD_BRAKES, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_BRAKES) - 1, 1); //Brakes
	VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRANSMISSION, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRANSMISSION) - 1, 1); //Transmission
	
	/*Below test works!
	VEHICLE::SET_VEHICLE_MOD(veh, MOD_BOBBLEHEAD, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_BOBBLEHEAD) - 1, 1); //--Bobblehead test
	VEHICLE::SET_VEHICLE_MOD(veh, MOD_LIVERY, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_LIVERY) - 1, 1); //--Livery Test 
	end of test*/

	VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_TURBO, 1); //Turbo Tuning
	VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_XENONLIGHTS, 1); //Headlights

	if (optics) {
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_SPOILER, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SPOILER) - 1, 1); //--Spoilers
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_FRONTBUMPER, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_FRONTBUMPER) - 1, 1); //--Front Bumper
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_REARBUMPER, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_REARBUMPER) - 1, 1); //--Rear Bumper
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_SIDESKIRT, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SIDESKIRT) - 1, 1); //--Side Skirt
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_EXHAUST, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_EXHAUST) - 1, 1); //--Exhaust
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_CHASSIS, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_CHASSIS) - 1, 1); //--Chassis or roll cage
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_GRILLE, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_GRILLE) - 1, 1); //--Grille
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_HOOD, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_HOOD) - 1, 1); //--Hood
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_FENDER, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_FENDER) - 1, 1); //--Fender
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_RIGHTFENDER, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_RIGHTFENDER) - 1, 1); //--Right Fender
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_ROOF, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ROOF) - 1, MOD_ROOF); //--Roof
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_HORNS, HORN_MUSICAL5, 0);										  //--Horns
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_SUSPENSION, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SUSPENSION) - 1, 1); //--Suspension
		VEHICLE::SET_VEHICLE_MOD(veh, MOD_ARMOR, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ARMOR) - 1, 1); //--Armor
		

	
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, PLATE_YANKTON);
//		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "ENHANCED");
	}

	VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.0f);

	if (repaint)
	{
		//random metallic paint
		int useless, wheelCol;//pearl topcoat, wheel color
		int paintIndex = rand() % PAINTS_METALLIC.size();
		PaintColour paint = PAINTS_METALLIC.at(paintIndex);
		VEHICLE::SET_VEHICLE_COLOURS(veh, paint.mainValue, paint.mainValue);
		VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &useless, &wheelCol);
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, paint.pearlAddition, wheelCol);
	}
}
