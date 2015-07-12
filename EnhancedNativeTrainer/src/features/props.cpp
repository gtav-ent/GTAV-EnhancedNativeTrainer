/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "props.h"

std::vector<PropInfo> ALL_PROPS =
{
	//DO NOT EDIT THESE DIRECTLY, USE THE SPREADSHEET IN DOCUMENTS & EXPORT FROM THERE
	{ "prop_a_base_bars_01", "A Base Bars #1" },
};

float vectRads(float degs)
{
	float radialConv = degs*3.1415926536 / 180;
	return radialConv;
}

float entSpeed = 300.00;

void do_spawn_prop(PropInfo prop, bool silent)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash propHash = GAMEPLAY::GET_HASH_KEY((char *)prop.model);

	if (!STREAMING::IS_MODEL_IN_CDIMAGE(propHash) || !STREAMING::IS_MODEL_VALID(propHash))
	{
		if (!silent)
		{
			std::ostringstream ss;
			ss << "Model " << prop.model << " is not valid";
			set_status_text(ss.str());
		}

		std::ostringstream ss2;
		ss2 << "INVALID-MODEL: " << prop.model;
		write_text_to_log_file(ss2.str());
		return;
	}

	STREAMING::REQUEST_MODEL(propHash);
	while (!STREAMING::HAS_MODEL_LOADED(propHash))
	{
		make_periodic_feature_call();
		WAIT(0);
	}
	FLOAT look = ENTITY::GET_ENTITY_HEADING(playerPed);
	FLOAT lookAni = look + 180.00;
	FLOAT lookOff = look + 90.00;
	FLOAT vecX = 0;
	FLOAT vecY = 0;

	FLOAT spawnOffX = 0.0f;
	FLOAT spawnOffY = 3.5f;
	FLOAT spawnOffZ = 0.0f;

	Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, spawnOffX, spawnOffY, spawnOffZ);

	Object obj = OBJECT::CREATE_OBJECT_NO_OFFSET(propHash, coords.x, coords.y, coords.z, 1, 1, 1);
	if (ENTITY::DOES_ENTITY_EXIST(obj))
	{
		ENTITY::SET_ENTITY_COLLISION(obj, 1, 0);
		OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(obj);
		ENTITY::SET_ENTITY_HAS_GRAVITY(obj, true);
		ENTITY::FREEZE_ENTITY_POSITION(obj, false);
		OBJECT::SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(obj, true);
		ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(obj, true);
	}
	else
	{
		if (!silent)
		{
			std::ostringstream ss;
			ss << "Failed to create " << prop.label;
			set_status_text(ss.str());
		}

		std::ostringstream ss2;
		ss2 << "INVALID-PROP: " << prop.model;
		write_text_to_log_file(ss2.str());
		return;
	}

	if (!silent)
	{
		std::ostringstream ss;
		ss << "Spawned " << prop.label;
		set_status_text(ss.str());
	}

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(propHash);
	ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&obj);
}

bool onconfirm_prop_selection(MenuItem<int> choice)
{
	if (choice.value == -1)
	{
		int i = 1;
		for each (PropInfo prop  in ALL_PROPS)
		{
			do_spawn_prop(prop, true);
			WAIT(0);
			make_periodic_feature_call();

			std::ostringstream ss;
			ss << "Done " << i++ << " of " << ALL_PROPS.size();
			set_status_text_centre_screen(ss.str());
		}
		return false;
	}

	PropInfo prop = ALL_PROPS.at(choice.value);
	do_spawn_prop(prop, false);

	return false;
}

int propSelection = 0;

void process_props_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int>* item = new MenuItem<int>();
	item->value = -1;
	item->caption = "Spawn Everything";
	item->isLeaf = true;
	menuItems.push_back(item);

	int i = 0;
	for each (PropInfo prop in ALL_PROPS )
	{
		MenuItem<int>* item = new MenuItem<int>();
		item->value = i;
		item->caption = prop.label;
		item->isLeaf = true;
		menuItems.push_back(item);
		i++;
	}

	draw_generic_menu<int>(menuItems, &propSelection, "All Props", onconfirm_prop_selection, NULL, NULL, NULL);

}