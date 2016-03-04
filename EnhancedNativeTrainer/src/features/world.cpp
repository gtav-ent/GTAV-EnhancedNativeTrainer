/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/


#include "world.h"
#include "script.h"
#include "..\ui_support\menu_functions.h"
#include <Psapi.h>
#include <Windows.h>

int activeLineIndexWorld = 0;
int activeLineIndexWeather = 0;

bool featureRestrictedZones = true;

bool featureWorldMoonGravity = false;
bool featureWorldMoonGravityUpdated = false;

bool featureWorldNoPeds = false;
bool featureWorldNoPedsUpdated = false;
bool featureWorldNoTraffic = false;
bool featureWorldNoTrafficUpdated = false;

bool featureWorldRandomCops = true;
bool featureWorldRandomTrains = true;
bool featureWorldRandomBoats = true;
bool featureWorldGarbageTrucks = true;

bool featureBlackout = false;
bool featureBlackoutUpdated = false;

bool featureWeatherWind = false;
bool featureWeatherFreeze = false;

//GTA MP's Snow Mod
bool featureSnow = false;
bool featureSnowUpdated = false;

std::string lastWeather;
std::string lastWeatherName;


/*******************Snow Related code********************/

static bool configEnableDefault = true;
static bool configVehicleTracks = true;
static bool configPedTracks = true;
static bool configVehicleTrackDepth = true; //false
static bool configPedTrackDepth = true; //false


// Find Pattern
bool CompareMemory(const uint8_t* pData, const uint8_t* bMask, const char* sMask)
{
	for (; *sMask; ++sMask, ++pData, ++bMask)
		if (*sMask == 'x' && *pData != *bMask)
			return false;

	return *sMask == NULL;
}

intptr_t FindPattern(const char* bMask, const char* sMask) //finds snow pattern in memory
{
	// Game Base & Size
	static intptr_t pGameBase = (intptr_t)GetModuleHandle(nullptr);
	static uint32_t pGameSize = 0;
	if (!pGameSize)
	{
		MODULEINFO info;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)pGameBase, &info, sizeof(MODULEINFO));
		pGameSize = info.SizeOfImage;
	}

	// Scan
	for (uint32_t i = 0; i < pGameSize; i++)
		if (CompareMemory((uint8_t*)(pGameBase + i), (uint8_t*)bMask, sMask))
			return pGameBase + i;

	return 0;
}

// Tracks
void EnableTracks(bool tracksVehicle = true, bool tracksPeds = true, bool deepTracksVehicle = true, bool deepTracksPed = true)
{
	static auto VAR_FeetSnowTracks_call = FindPattern("\x80\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x37", "xx?????xxxxx");
	if (!VAR_FeetSnowTracks_call)
	{
		set_status_text("Snow Mod is ~r~not compatible ~s~with this GTA Version #1!");
		return;
	}
	static auto VAR_FeetSnowTracks = VAR_FeetSnowTracks_call + (*(int32_t *)(VAR_FeetSnowTracks_call + 2)) + 7;
	//
	static auto VAR_VehicleSnowTracks_call = FindPattern("\x40\x38\x3D\x00\x00\x00\x00\x48\x8B\x42\x20", "xxx????xxxx");
	if (!VAR_VehicleSnowTracks_call)
	{
		set_status_text("Snow Mod is ~r~not compatible ~s~with this GTA Version #2!");
		return;
	}
	static auto VAR_VehicleSnowTracks = VAR_VehicleSnowTracks_call + (*(int32_t *)(VAR_VehicleSnowTracks_call + 3)) + 7;
	//

	VirtualProtect((void*)VAR_FeetSnowTracks, 1, PAGE_EXECUTE_READWRITE, nullptr);
	VirtualProtect((void*)VAR_VehicleSnowTracks, 1, PAGE_EXECUTE_READWRITE, nullptr);

	// Enable/Disable Vehicle/Foot Snow tracks
	*(uint8_t *)VAR_FeetSnowTracks = tracksVehicle;
	*(uint8_t *)VAR_VehicleSnowTracks = tracksPeds;

	// Switch for big/small tracks
	static auto vehicleTrackTypes = FindPattern("\xB9\x00\x00\x00\x00\x84\xC0\x44\x0F\x44\xF1", "x????xxxxxx");
	if (!vehicleTrackTypes)
	{
		set_status_text("Snow Mod is ~r~not compatible ~s~with this GTA Version #3!");
		return;
	}

	VirtualProtect((void*)vehicleTrackTypes, 1, PAGE_EXECUTE_READWRITE, nullptr);
	*(uint8_t *)(vehicleTrackTypes + 1) = deepTracksVehicle ? 0x13 : 0x14;

	static auto pedTrackTypes = FindPattern("\xB9\x00\x00\x00\x00\x84\xC0\x0F\x44\xD9\x48\x8B\x4F\x30", "x????xxxxxxxxx");
	if (!pedTrackTypes)
	{
		set_status_text("Snow Mod is ~r~not compatible ~s~with this GTA Version #4!");
		return;
	}
	VirtualProtect((void*)pedTrackTypes, 1, PAGE_EXECUTE_READWRITE, nullptr);
	*(uint8_t *)(pedTrackTypes + 1) = deepTracksPed ? 0x13 : 0x14;
}

// Snow
void EnableSnow(bool featureSnow)
{
	if (featureSnow) //bEnable
		EnableTracks(configVehicleTracks, configPedTracks, configVehicleTrackDepth, configPedTrackDepth);
	else
		EnableTracks();

	// Adresses
	static auto addr1 = FindPattern("\x80\x3D\x00\x00\x00\x00\x00\x74\x27\x84\xC0", "xx?????xxxx");
	static auto addr2 = FindPattern("\x44\x38\x3D\x00\x00\x00\x00\x74\x0F", "xxx????xx");

	// Outdated
	// In future the patterns might change
	if (!addr1 || !addr2)
	{
		static auto addr3 = FindPattern("\x40\x38\x35\x00\x00\x00\x00\x74\x18\x84\xdb\x74\x14", "xxx????xxxxxx");

		if (!addr3)
		{
			set_status_text("Snow Mod is ~r~not compatible ~s~with this GTA Version #5!");
			return;
		}
		else
		{
			addr1 = addr3;
		}

	}

	// Original Memory
	static uint8_t original1[14] = { 0 };
	static uint8_t original2[15] = { 0 };

	// Initialize
	static bool bInitialized = false;
	if (!bInitialized)
	{
		bInitialized = true;

		// Unprotect Memory
		VirtualProtect((void*)addr1, 13, PAGE_EXECUTE_READWRITE, nullptr);
		VirtualProtect((void*)addr2, 14, PAGE_EXECUTE_READWRITE, nullptr);

		// Copy original Memory
		memcpy(&original1, (void*)addr1, 13);
		memcpy(&original2, (void*)addr2, 14);
	}

	// Toggle
	if (featureSnow) //bEnable
	{
		// Weather
		GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST("XMAS");

		// NOP checks
		memset((void*)addr1, 0x90, 13);
		memset((void*)addr2, 0x90, 14);

		// Notification
		set_status_text("Snow ~g~ON");
	}
	else
	{
		// Copy original memory
		memcpy((void*)addr1, &original1, 13);
		memcpy((void*)addr2, &original2, 14);

		// Weather
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		GAMEPLAY::SET_WEATHER_TYPE_NOW("CLEAR");

		// Notification
		set_status_text("Snow ~r~OFF");
	}
}


/*************** End of Snow Related code****************/



bool onconfirm_weather_menu(MenuItem<std::string> choice)
{
	std::stringstream ss; ss << "Weather Frozen at: " << lastWeatherName;
	switch (choice.currentMenuIndex)
	{
		// wind
	case 0:
		if (featureWeatherWind)
		{
			GAMEPLAY::SET_WIND(1.0);
			GAMEPLAY::SET_WIND_SPEED(11.99);
			GAMEPLAY::SET_WIND_DIRECTION(ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
		}
		else
		{
			GAMEPLAY::SET_WIND(0.0);
			GAMEPLAY::SET_WIND_SPEED(0.0);
		}
		break;
		// set weather
	case 1:

		GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();

		if (featureWeatherFreeze && !lastWeather.empty())
		{
			std::stringstream ss; ss << "Weather frozen at: " << lastWeatherName;
			set_status_text(ss.str());
		}
		else if (!featureWeatherFreeze)
		{
			set_status_text("Weather unfrozen");
		}
		else
		{
			set_status_text("Set a weather value first");
			featureWeatherFreeze = false;
		}
		break;
	default:
		lastWeather = choice.value.c_str();
		lastWeatherName = choice.caption;

		GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();

		GAMEPLAY::SET_WEATHER_TYPE_NOW((char *)lastWeather.c_str());

		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();

		std::ostringstream ss2;
		ss2 << "Weather: " << choice.caption;
		set_status_text(ss2.str());
	}

	return false;
}

void process_weather_menu()
{
	const int lineCount = 16;

	std::string caption = "Weather Options";

	StringStandardOrToggleMenuDef lines[lineCount] = {
		{ "Wind", "WIND", &featureWeatherWind, NULL },
		{ "Freeze Weather", "FREEZEWEATHER", &featureWeatherFreeze, NULL },
		{ "Extra Sunny", "EXTRASUNNY", NULL, NULL },
		{ "Clear", "CLEAR", NULL, NULL },
		{ "Cloudy", "CLOUDS", NULL, NULL },
		{ "Smog", "SMOG", NULL, NULL },
		{ "Foggy", "FOGGY", NULL, NULL },
		{ "Overcast", "OVERCAST", NULL, NULL },
		{ "Rain", "RAIN", NULL, NULL },
		{ "Stormy", "THUNDER", NULL, NULL },
		{ "Clearing", "CLEARING", NULL, NULL },
		{ "Neutral", "NEUTRAL", NULL, NULL },
		{ "Snow", "SNOW", NULL, NULL },
		{ "Blizzard", "BLIZZARD", NULL, NULL },
		{ "Light Snow", "SNOWLIGHT", NULL, NULL },
		{ "Christmas", "XMAS", NULL, NULL }
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexWeather, caption, onconfirm_weather_menu);
}

bool onconfirm_world_menu(MenuItem<int> choice)
{
	switch (activeLineIndexWorld)
	{
	case 0:
		process_weather_menu();
		break;
	case 2:
		// featureWorldRandomCops being set in update_features
		break;
	case 3:
		VEHICLE::SET_RANDOM_TRAINS(featureWorldRandomTrains);
		break;
	case 4:
		VEHICLE::SET_RANDOM_BOATS(featureWorldRandomBoats);
		break;
	case 5:
		VEHICLE::SET_GARBAGE_TRUCKS(featureWorldGarbageTrucks);
		break;
	case 6:
		EnableSnow(featureSnow);
	}
	return false;
}

void process_world_menu()
{
	const int lineCount = 8; // Amount of cases +1

	std::string caption = "World Options";

	// read default feature values from the game
	featureWorldRandomCops = (PED::CAN_CREATE_RANDOM_COPS() == TRUE);

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *item = new MenuItem<int>();
	item->isLeaf = false;
	item->caption = "Weather";
	item->value = 1;
	menuItems.push_back(item);

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "Moon Gravity";
	togItem->value = 1;
	togItem->toggleValue = &featureWorldMoonGravity;
	togItem->toggleValueUpdated = &featureWorldMoonGravityUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "No Pedestrians";
	togItem->value = 1;
	togItem->toggleValue = &featureWorldNoPeds;
	togItem->toggleValueUpdated = &featureWorldNoPedsUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "No Traffic";
	togItem->value = 1;
	togItem->toggleValue = &featureWorldNoTraffic;
	togItem->toggleValueUpdated = &featureWorldNoTrafficUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Random Cops";
	togItem->value = 2;
	togItem->toggleValue = &featureWorldRandomCops;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Random Trains";
	togItem->value = 3;
	togItem->toggleValue = &featureWorldRandomTrains;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Random Boats";
	togItem->value = 4;
	togItem->toggleValue = &featureWorldRandomBoats;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Garbage Trucks";
	togItem->value = 5;
	togItem->toggleValue = &featureWorldGarbageTrucks;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Restricted Zones";
	togItem->value = 6;
	togItem->toggleValue = &featureRestrictedZones;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Blackout";
	togItem->value = 6;
	togItem->toggleValue = &featureBlackout;
	togItem->toggleValueUpdated = &featureBlackoutUpdated;
	menuItems.push_back(togItem);

	/*togItem = new ToggleMenuItem<int>(); //Needs an updated mem address!
	togItem->caption = "Heavy Snow";
	togItem->value = 7;
	togItem->toggleValue = &featureSnow;
	togItem->toggleValueUpdated = &featureSnowUpdated;
	menuItems.push_back(togItem);*/

	draw_generic_menu<int>(menuItems, &activeLineIndexWorld, caption, onconfirm_world_menu, NULL, NULL);
}

void reset_world_globals()
{
	activeLineIndexWorld = 0;
	activeLineIndexWeather = 0;
	lastWeather.clear();
	lastWeatherName.clear();

	featureWeatherWind =
	featureWeatherFreeze =
	featureWorldMoonGravity = false;

	featureWorldNoPeds = false;
	featureWorldNoTraffic = false;
	featureBlackout = false;

	featureSnow = false;
	
	featureWorldRandomCops =
		featureWorldRandomTrains =
		featureWorldRandomBoats =
		featureWorldGarbageTrucks = true;

	featureSnowUpdated = true;
	featureWorldNoPedsUpdated = true;
	featureWorldMoonGravityUpdated = true;
	featureWorldNoTrafficUpdated = true;
	featureBlackoutUpdated = true;
}

void update_world_features()
{
	PED::SET_CREATE_RANDOM_COPS(featureWorldRandomCops);

	if (featureWorldMoonGravity)
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(1);
	}
	else if (featureWorldMoonGravityUpdated)
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(0);
	}

	if (featureBlackoutUpdated)
	{
		GRAPHICS::_SET_BLACKOUT(featureBlackout);
		featureBlackoutUpdated = false;
	}

	/*
	if (featureBlackout)
	{
		if (STREAMING::IS_IPL_ACTIVE("DT1_22_bldg2"))
		{
			set_status_text("DT1_22_bldg2");
			STREAMING::REMOVE_IPL("DT1_22_bldg2");
		}

		GRAPHICS::_0x1600FD8CF72EBC12(0.001);
		GRAPHICS::_0x1A8E2C8B9CF4549C("lab_none_dark", "lab_none_dark_OVR");
		GRAPHICS::_0x1A8E2C8B9CF4549C("lab_none_exit", "lab_none_exit_OVR");

		for (int i = 0; i <= 35; i++)
		{
			std::ostringstream ss;
			ss << "distlodlights_medium";
			ss << std::setfill('0') << std::setw(3) << i;
			auto scenery = ss.str().c_str();
			if (STREAMING::IS_IPL_ACTIVE(scenery))
			{
				set_status_text(ss.str());
				STREAMING::REMOVE_IPL(scenery);
			}
		}
		for (int i = 0; i <= 61; i++)
		{
			std::ostringstream ss;
			ss << "distlodlights_small";
			ss << std::setfill('0') << std::setw(3) << i;
			auto scenery = ss.str().c_str();
			if (STREAMING::IS_IPL_ACTIVE(scenery))
			{
				set_status_text(ss.str());
				STREAMING::REMOVE_IPL(scenery);
			}
		}
		for (int i = 0; i <= 35; i++)
		{
			std::ostringstream ss;
			ss << "lodlights_medium";
			ss << std::setfill('0') << std::setw(3) << i;
			auto scenery = ss.str().c_str();
			if (STREAMING::IS_IPL_ACTIVE(scenery))
			{
				set_status_text(ss.str());
				STREAMING::REMOVE_IPL(scenery);
			}
		}
		for (int i = 0; i <= 61; i++)
		{
			std::ostringstream ss;
			ss << "lodlights_small";
			ss << std::setfill('0') << std::setw(3) << i;
			auto scenery = ss.str().c_str();
			if (STREAMING::IS_IPL_ACTIVE(scenery))
			{
				set_status_text(ss.str());
				STREAMING::REMOVE_IPL(scenery);
			}
		}

		for (int i = 0; i <= 0; i++)
		{
			std::ostringstream ss;
			ss << "lodlights_large";
			ss << std::setfill('0') << std::setw(3) << i;
			auto scenery = ss.str().c_str();
			if (STREAMING::IS_IPL_ACTIVE(scenery))
			{
				set_status_text(ss.str());
				STREAMING::REMOVE_IPL(scenery);
			}
		}

		for (int i = 0; i <= 0; i++)
		{
			std::ostringstream ss;
			ss << "distlodlights_large";
			ss << std::setfill('0') << std::setw(3) << i;
			auto scenery = ss.str().c_str();
			if (STREAMING::IS_IPL_ACTIVE(scenery))
			{
				set_status_text(ss.str());
				STREAMING::REMOVE_IPL(scenery);
			}
		}
	}*/

	if (featureWorldNoPedsUpdated)
	{
		if (featureWorldNoPeds)
		{
			Vector3 v3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			GAMEPLAY::CLEAR_AREA_OF_PEDS(v3.x, v3.y, v3.z, 1000.0, 0);
			STREAMING::SET_PED_POPULATION_BUDGET(0);
		}
		else
		{
			STREAMING::SET_PED_POPULATION_BUDGET(3);
		}

		featureWorldNoPedsUpdated = false;
	}
	else if (featureWorldNoPeds && get_frame_number() % 100 == 50)
	{
		Vector3 v3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		GAMEPLAY::CLEAR_AREA_OF_PEDS(v3.x, v3.y, v3.z, 1000.0, 0);
		STREAMING::SET_PED_POPULATION_BUDGET(0);
	}

	if (featureWorldNoTrafficUpdated)
	{
		VEHICLE::_0xF796359A959DF65D(!featureWorldNoTraffic);
		GRAPHICS::DISABLE_VEHICLE_DISTANTLIGHTS(featureWorldNoTraffic);

		if (featureWorldNoTraffic)
		{
			Vector3 v3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			GAMEPLAY::CLEAR_AREA_OF_VEHICLES(v3.x, v3.y, v3.z, 1000.0, 0, 0, 0, 0, 0);

			STREAMING::SET_VEHICLE_POPULATION_BUDGET(0);
			VEHICLE::SET_ALL_VEHICLE_GENERATORS_ACTIVE_IN_AREA(-10000.0, -10000.0, -200.0, 10000.0, 10000.0, 1000.0, 0, 1);
			PATHFIND::SET_ROADS_IN_AREA(-10000.0, -10000.0, -200.0, 10000.0, 10000.0, 1000.0, 0, 1);
		}
		else
		{
			STREAMING::SET_VEHICLE_POPULATION_BUDGET(3);
			VEHICLE::SET_ALL_VEHICLE_GENERATORS_ACTIVE();
			PATHFIND::SET_ROADS_BACK_TO_ORIGINAL(-10000.0, -10000.0, -200.0, 10000.0, 10000.0, 1000.0);
		}

		featureWorldNoTrafficUpdated = false;
	}
	else if (featureWorldNoTraffic)// && get_frame_number() % 100 == 0)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			Vector3 v3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			GAMEPLAY::CLEAR_AREA_OF_VEHICLES(v3.x, v3.y, v3.z, 1000.0, 0, 0, 0, 0, 0);
		}
		STREAMING::SET_VEHICLE_POPULATION_BUDGET(0);
		VEHICLE::SET_ALL_VEHICLE_GENERATORS_ACTIVE_IN_AREA(-10000.0, -10000.0, -200.0, 10000.0, 10000.0, 1000.0, 0, 1);
		PATHFIND::SET_ROADS_IN_AREA(-10000.0, -10000.0, -200.0, 10000.0, 10000.0, 1000.0, 0, 1);
	}

	if (!featureWorldRandomTrains)
	{
		VEHICLE::DELETE_ALL_TRAINS();
	}

	if (featureWeatherFreeze && !lastWeather.empty())
	{
		GAMEPLAY::SET_WEATHER_TYPE_NOW((char *)lastWeather.c_str());
	}

	if (!featureRestrictedZones)
	{
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("restrictedareas");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_lossantosintl");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prison");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prisonvanbreak");
	}

	if (featureSnowUpdated)
	{
		if (featureSnow)
		{
		//	EnableSnow(!featureBlackout);
			EnableSnow(true);
			EnableTracks(true, true, true, true);
		}
		else
		{
			EnableSnow(false);
			EnableTracks(false, false, false, false);
		}
		featureSnowUpdated = false;
	}
}

void add_world_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldMoonGravity", &featureWorldMoonGravity, &featureWorldMoonGravityUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomCops", &featureWorldRandomCops });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomTrains", &featureWorldRandomTrains });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomBoats", &featureWorldRandomBoats });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldGarbageTrucks", &featureWorldGarbageTrucks });

	results->push_back(FeatureEnabledLocalDefinition{ "featureWeatherWind", &featureWeatherWind });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeatherFreeze", &featureWeatherFreeze });
	results->push_back(FeatureEnabledLocalDefinition{ "featureBlackout", &featureBlackout, &featureBlackoutUpdated });

	results->push_back(FeatureEnabledLocalDefinition{ "featureRestrictedZones", &featureRestrictedZones });

	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldNoPeds", &featureWorldNoPeds, &featureWorldNoPedsUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldNoTraffic", &featureWorldNoTraffic, &featureWorldNoTrafficUpdated });

	//results->push_back(FeatureEnabledLocalDefinition{ "featureSnow", &featureSnow }); NEEDS NEW MEM ADDRESS
}

void add_world_generic_settings(std::vector<StringPairSettingDBRow>* settings)
{
	settings->push_back(StringPairSettingDBRow{ "lastWeather", lastWeather });
	settings->push_back(StringPairSettingDBRow{ "lastWeatherName", lastWeatherName });
}

void handle_generic_settings_world(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("lastWeather") == 0)
		{
			lastWeather = setting.value;
		}
		else if (setting.name.compare("lastWeatherName") == 0)
		{
			lastWeatherName = setting.value;
		}
	}

}