/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "teleportation.h"
#include "menu_functions.h"

struct tele_location {
	std::string text;
	float x;
	float y;
	float z;
	void (*scenery_loader)(void);
	void (*scenery_unloader)(void);
	bool isLoaded;
};

bool operator==(const tele_location& lhs, const tele_location& rhs)
{
	return (lhs.text == rhs.text && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
}

std::vector<tele_location> LOCATIONS = {
	{ "MARKER" },
	{ "MICHAEL'S HOUSE", -852.4f, 160.0f, 65.6f },
	{ "FRANKLIN'S HOUSE", 7.9f, 548.1f, 175.5f },
	{ "TREVOR'S TRAILER", 1985.7f, 3812.2f, 32.2f },
	{ "AIRPORT ENTRANCE", -1034.6f, -2733.6f, 13.8f },
	{ "AIRPORT FIELD", -1336.0f, -3044.0f, 13.9f },
	{ "ELYSIAN ISLAND", 338.2f, -2715.9f, 38.5f },
	{ "JETSAM", 760.4f, -2943.2f, 5.8f },
	{ "STRIPCLUB", 127.4f, -1307.7f, 29.2f },
	{ "ELBURRO HEIGHTS", 1384.0f, -2057.1f, 52.0f },
	{ "FERRIS WHEEL", -1670.7f, -1125.0f, 13.0f },
	{ "CHUMASH", -3192.6f, 1100.0f, 20.2f },
	{ "WINDFARM", 2354.0f, 1830.3f, 101.1f },
	{ "MILITARY BASE", -2047.4f, 3132.1f, 32.8f },
	{ "MCKENZIE AIRFIELD", 2121.7f, 4796.3f, 41.1f },
	{ "DESERT AIRFIELD", 1747.0f, 3273.7f, 41.1f },
	{ "CHILLIAD", 425.4f, 5614.3f, 766.5f },
	{ "NORTH YANKTON", 3360.19f, -4849.67f, 111.8f, load_north_yankton, unload_north_yankton, false } };

int teleportMenuIndex = 0;

void teleport_to_coords(Entity e, Vector3 coords)
{
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
	WAIT(0);
	set_status_text("Teleported");
}

void teleport_to_marker(Entity e)
{
	Vector3 coords;
	bool success = false;

	bool blipFound = false;
	// search for marker blip
	int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
	for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
	{
		if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
		{
			coords = UI::GET_BLIP_INFO_ID_COORD(i);
			blipFound = true;
			break;
		}
	}
	if (blipFound)
	{
		// load needed map region and check height levels for ground existence
		bool groundFound = false;
		static float groundCheckHeight[] = {
			100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
			450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
		};
		for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
			WAIT(100);
			if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z))
			{
				groundFound = true;
				coords.z += 3.0;
				break;
			}
		}
		// if ground not found then set Z in air and give player a parachute
		if (!groundFound)
		{
			coords.z = 1000.0;
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
		}

		//do it
		teleport_to_coords(e, coords);
	}
	else
	{
		set_status_text("Map marker isn't set");
	}
}

bool onconfirm_teleport_location(int selection, std::string caption, tele_location value)
{
	teleportMenuIndex = selection;

	// get entity to teleport
	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
	{
		e = PED::GET_VEHICLE_PED_IS_USING(e);
	}

	if (selection == 0)
	{
		teleport_to_marker(e);
	}
	else
	{
		Vector3 coords;

		if (value.scenery_loader != NULL && !value.isLoaded)
		{
			set_status_text("Loading new scenery...");
			value.scenery_loader();
			value.isLoaded = true;

			DWORD time = GetTickCount() + 1000;
			while (GetTickCount() < time)
			{
				make_periodic_feature_call();
				WAIT(0);
			}

			set_status_text("New scenery loaded");

			time = GetTickCount() + 1000;
			while (GetTickCount() < time)
			{
				make_periodic_feature_call();
				WAIT(0);
			}
		}

		coords.x = value.x;
		coords.y = value.y;
		coords.z = value.z;
		teleport_to_coords(e, coords);

		bool unloadedAnything = false;
		DWORD time = GetTickCount() + 1000;

		for (int i = 0; i < LOCATIONS.size(); i++)
		{
			tele_location loc = LOCATIONS[i];
			if (loc == value) //don't unload what we just loaded!
			{
				continue;
			}
			if (loc.isLoaded && loc.scenery_unloader != NULL)
			{
				if (!unloadedAnything)
				{
					set_status_text("Unloading old scenery...");
					time = GetTickCount() + 1000;
					while (GetTickCount() < time)
					{
						make_periodic_feature_call();
						WAIT(0);
					}
				}

				loc.scenery_unloader();
				unloadedAnything = true;
				loc.isLoaded = false;
			}
		}

		if (unloadedAnything)
		{
			set_status_text("Old scenery unloaded");

			time = GetTickCount() + 1000;
			while (GetTickCount() < time)
			{
				make_periodic_feature_call();
				WAIT(0);
			}
		}
	}
	return true;
}

bool process_teleport_menu()
{
	std::string caption = "TELEPORT";

	std::vector<std::string> menuCaptions;
	for (int i = 0; i < LOCATIONS.size(); i++)
	{
		menuCaptions.push_back(LOCATIONS[i].text);
	}

	return draw_generic_menu<tele_location>(menuCaptions, LOCATIONS, teleportMenuIndex, "Teleport Locations", onconfirm_teleport_location, NULL, NULL);
}

void reset_teleporter_globals()
{
	teleportMenuIndex = 0;
}

void load_north_yankton()
{
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) )// && STREAMING::IS_IPL_ACTIVE("plg_01") == 0)
	{
		STREAMING::REQUEST_IPL("plg_01");
		STREAMING::REQUEST_IPL("prologue01");
		STREAMING::REQUEST_IPL("prologue01_lod");
		STREAMING::REQUEST_IPL("prologue01c");
		STREAMING::REQUEST_IPL("prologue01c_lod");
		STREAMING::REQUEST_IPL("prologue01d");
		STREAMING::REQUEST_IPL("prologue01d_lod");
		STREAMING::REQUEST_IPL("prologue01e");
		STREAMING::REQUEST_IPL("prologue01e_lod");
		STREAMING::REQUEST_IPL("prologue01f");
		STREAMING::REQUEST_IPL("prologue01f_lod");
		STREAMING::REQUEST_IPL("prologue01g");
		STREAMING::REQUEST_IPL("prologue01h");
		STREAMING::REQUEST_IPL("prologue01h_lod");
		STREAMING::REQUEST_IPL("prologue01i");
		STREAMING::REQUEST_IPL("prologue01i_lod");
		STREAMING::REQUEST_IPL("prologue01j");
		STREAMING::REQUEST_IPL("prologue01j_lod");
		STREAMING::REQUEST_IPL("prologue01k");
		STREAMING::REQUEST_IPL("prologue01k_lod");
		STREAMING::REQUEST_IPL("prologue01z");
		STREAMING::REQUEST_IPL("prologue01z_lod");
		STREAMING::REQUEST_IPL("plg_02");
		STREAMING::REQUEST_IPL("prologue02");
		STREAMING::REQUEST_IPL("prologue02_lod");
		STREAMING::REQUEST_IPL("plg_03");
		STREAMING::REQUEST_IPL("prologue03");
		STREAMING::REQUEST_IPL("prologue03_lod");
		STREAMING::REQUEST_IPL("prologue03b");
		STREAMING::REQUEST_IPL("prologue03b_lod");
		//the commented code disables the 'Prologue' grave and
		//enables the 'Bury the Hatchet' grave
		//STREAMING::REQUEST_IPL("prologue03_grv_cov");
		//STREAMING::REQUEST_IPL("prologue03_grv_cov_lod");
		STREAMING::REQUEST_IPL("prologue03_grv_dug");
		STREAMING::REQUEST_IPL("prologue03_grv_dug_lod");
		//STREAMING::REQUEST_IPL("prologue03_grv_fun");
		STREAMING::REQUEST_IPL("prologue_grv_torch");
		STREAMING::REQUEST_IPL("plg_04");
		STREAMING::REQUEST_IPL("prologue04");
		STREAMING::REQUEST_IPL("prologue04_lod");
		STREAMING::REQUEST_IPL("prologue04b");
		STREAMING::REQUEST_IPL("prologue04b_lod");
		STREAMING::REQUEST_IPL("prologue04_cover");
		STREAMING::REQUEST_IPL("des_protree_end");
		STREAMING::REQUEST_IPL("des_protree_start");
		STREAMING::REQUEST_IPL("des_protree_start_lod");
		STREAMING::REQUEST_IPL("plg_05");
		STREAMING::REQUEST_IPL("prologue05");
		STREAMING::REQUEST_IPL("prologue05_lod");
		STREAMING::REQUEST_IPL("prologue05b");
		STREAMING::REQUEST_IPL("prologue05b_lod");
		STREAMING::REQUEST_IPL("plg_06");
		STREAMING::REQUEST_IPL("prologue06");
		STREAMING::REQUEST_IPL("prologue06_lod");
		STREAMING::REQUEST_IPL("prologue06b");
		STREAMING::REQUEST_IPL("prologue06b_lod");
		STREAMING::REQUEST_IPL("prologue06_int");
		STREAMING::REQUEST_IPL("prologue06_int_lod");
		STREAMING::REQUEST_IPL("prologue06_pannel");
		STREAMING::REQUEST_IPL("prologue06_pannel_lod");
		//STREAMING::REQUEST_IPL("prologue_m2_door");
		//STREAMING::REQUEST_IPL("prologue_m2_door_lod");
		STREAMING::REQUEST_IPL("plg_occl_00");
		STREAMING::REQUEST_IPL("prologue_occl");
		STREAMING::REQUEST_IPL("plg_rd");
		STREAMING::REQUEST_IPL("prologuerd");
		STREAMING::REQUEST_IPL("prologuerdb");
		STREAMING::REQUEST_IPL("prologuerd_lod");
	}
}

void unload_north_yankton()
{
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) )// && STREAMING::IS_IPL_ACTIVE("plg_01") == 1)
	{
		STREAMING::REMOVE_IPL("plg_01");
		STREAMING::REMOVE_IPL("prologue01");
		STREAMING::REMOVE_IPL("prologue01_lod");
		STREAMING::REMOVE_IPL("prologue01c");
		STREAMING::REMOVE_IPL("prologue01c_lod");
		STREAMING::REMOVE_IPL("prologue01d");
		STREAMING::REMOVE_IPL("prologue01d_lod");
		STREAMING::REMOVE_IPL("prologue01e");
		STREAMING::REMOVE_IPL("prologue01e_lod");
		STREAMING::REMOVE_IPL("prologue01f");
		STREAMING::REMOVE_IPL("prologue01f_lod");
		STREAMING::REMOVE_IPL("prologue01g");
		STREAMING::REMOVE_IPL("prologue01h");
		STREAMING::REMOVE_IPL("prologue01h_lod");
		STREAMING::REMOVE_IPL("prologue01i");
		STREAMING::REMOVE_IPL("prologue01i_lod");
		STREAMING::REMOVE_IPL("prologue01j");
		STREAMING::REMOVE_IPL("prologue01j_lod");
		STREAMING::REMOVE_IPL("prologue01k");
		STREAMING::REMOVE_IPL("prologue01k_lod");
		STREAMING::REMOVE_IPL("prologue01z");
		STREAMING::REMOVE_IPL("prologue01z_lod");
		STREAMING::REMOVE_IPL("plg_02");
		STREAMING::REMOVE_IPL("prologue02");
		STREAMING::REMOVE_IPL("prologue02_lod");
		STREAMING::REMOVE_IPL("plg_03");
		STREAMING::REMOVE_IPL("prologue03");
		STREAMING::REMOVE_IPL("prologue03_lod");
		STREAMING::REMOVE_IPL("prologue03b");
		STREAMING::REMOVE_IPL("prologue03b_lod");
		//the commented code disables the 'Prologue' grave and
		//enables the 'Bury the Hatchet' grave
		//STREAMING::REMOVE_IPL("prologue03_grv_cov");
		//STREAMING::REMOVE_IPL("prologue03_grv_cov_lod");
		STREAMING::REMOVE_IPL("prologue03_grv_dug");
		STREAMING::REMOVE_IPL("prologue03_grv_dug_lod");
		//STREAMING::REMOVE_IPL("prologue03_grv_fun");
		STREAMING::REMOVE_IPL("prologue_grv_torch");
		STREAMING::REMOVE_IPL("plg_04");
		STREAMING::REMOVE_IPL("prologue04");
		STREAMING::REMOVE_IPL("prologue04_lod");
		STREAMING::REMOVE_IPL("prologue04b");
		STREAMING::REMOVE_IPL("prologue04b_lod");
		STREAMING::REMOVE_IPL("prologue04_cover");
		STREAMING::REMOVE_IPL("des_protree_end");
		STREAMING::REMOVE_IPL("des_protree_start");
		STREAMING::REMOVE_IPL("des_protree_start_lod");
		STREAMING::REMOVE_IPL("plg_05");
		STREAMING::REMOVE_IPL("prologue05");
		STREAMING::REMOVE_IPL("prologue05_lod");
		STREAMING::REMOVE_IPL("prologue05b");
		STREAMING::REMOVE_IPL("prologue05b_lod");
		STREAMING::REMOVE_IPL("plg_06");
		STREAMING::REMOVE_IPL("prologue06");
		STREAMING::REMOVE_IPL("prologue06_lod");
		STREAMING::REMOVE_IPL("prologue06b");
		STREAMING::REMOVE_IPL("prologue06b_lod");
		STREAMING::REMOVE_IPL("prologue06_int");
		STREAMING::REMOVE_IPL("prologue06_int_lod");
		STREAMING::REMOVE_IPL("prologue06_pannel");
		STREAMING::REMOVE_IPL("prologue06_pannel_lod");
		//STREAMING::REMOVE_IPL("prologue_m2_door");
		//STREAMING::REMOVE_IPL("prologue_m2_door_lod");
		STREAMING::REMOVE_IPL("plg_occl_00");
		STREAMING::REMOVE_IPL("prologue_occl");
		STREAMING::REMOVE_IPL("plg_rd");
		STREAMING::REMOVE_IPL("prologuerd");
		STREAMING::REMOVE_IPL("prologuerdb");
		STREAMING::REMOVE_IPL("prologuerd_lod");
	}
}