/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "area_effect.h"
#include "script.h"

int areaeffect_top_level_menu_index = 0;
int areaeffect_ped_level_menu_index = 0;
int areaeffect_veh_level_menu_index = 0;

std::set<Ped> lastSeenPeds;
std::set<Vehicle> lastSeenVehicles;

//features
bool featurePlayerIgnoredByAll = false;
bool featurePlayerIgnoredByAllUpdated = false;

bool featureAreaPedsInvincible = false;
bool featureAreaPedsInvincibleUpdated = false;

bool featureAreaVehiclesInvincible = false;
bool featureAreaVehiclesInvincibleUpdated = false;

bool featureAreaVehiclesBroken = false;
bool featureAreaVehiclesBrokenUpdated = false;

bool featureAreaPedsHeadExplode = false;
//bool featureAreaPedsHeadExplodeUpdated = false;

void add_areaeffect_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerIgnoredByAll", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaPedsInvincible", &featureAreaPedsInvincible, &featureAreaPedsInvincibleUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaPedsHeadExplode", &featureAreaPedsHeadExplode });

	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaVehiclesInvincible", &featureAreaVehiclesInvincible, &featureAreaVehiclesInvincibleUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaVehiclesBroken", &featureAreaVehiclesBroken, &featureAreaVehiclesBrokenUpdated });
}

void reset_areaeffect_globals()
{
	featurePlayerIgnoredByAll = false;
	featurePlayerIgnoredByAllUpdated = true;

	featureAreaVehiclesBroken = false;
	featureAreaVehiclesBrokenUpdated = true;

	featureAreaPedsInvincible = false;
	featureAreaPedsInvincibleUpdated = true;
}

void process_areaeffect_peds_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "Everyone Permanently Calm";
	togItem->value = 1;
	togItem->toggleValue = &featurePlayerIgnoredByAll;
	togItem->toggleValueUpdated = &featurePlayerIgnoredByAllUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Everyone Invincible";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaPedsInvincible;
	togItem->toggleValueUpdated = &featureAreaPedsInvincibleUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Everyone Dies";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaPedsHeadExplode;
	//togItem->toggleValueUpdated = &featureAreaPedsInvincibleUpdated;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &areaeffect_ped_level_menu_index, "Ped Effects", NULL, NULL, NULL);
}

void process_areaeffect_vehicle_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "All Vehicles Invincible";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaVehiclesInvincible;
	togItem->toggleValueUpdated = &featureAreaVehiclesInvincibleUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "All Vehicles Immobile";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaVehiclesBroken;
	togItem->toggleValueUpdated = &featureAreaVehiclesBrokenUpdated;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &areaeffect_veh_level_menu_index, "Vehicle Effects", NULL, NULL, NULL);
}

bool onconfirm_areaeffect_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case -1:
		process_areaeffect_peds_menu();
		break;
	case -2:
		process_areaeffect_vehicle_menu();
		break;
	}
	return false;
}

void process_areaeffect_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	
	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "Pedestrians";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Vehicles";
	item->value = -2;
	item->isLeaf = true;
	menuItems.push_back(item);

	draw_generic_menu<int>(menuItems, &areaeffect_top_level_menu_index, "Area Effects", onconfirm_areaeffect_menu, NULL, NULL);

	//		{ "Everyone Ignores You", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated, true },
}

void update_area_effects(Ped playerPed)
{
	Player player = PLAYER::PLAYER_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	update_nearby_peds(playerPed, 50);
	update_nearby_vehicles(playerPed, 50);

	// everyone ignores player
	if (featurePlayerIgnoredByAll)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, true);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, false);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, true);
			if (get_frame_number() % 5 == 0)
			{
				set_all_nearby_peds_to_calm();
			}
		}
	}
	else if (featurePlayerIgnoredByAllUpdated)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, is_player_ignored_by_police());
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, false);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, true);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, false);
		}
		featurePlayerIgnoredByAllUpdated = false;
	}

	if (featureAreaPedsInvincible)
	{
		if (get_frame_number() % 5 == 0)
		{
			set_all_nearby_peds_to_invincible();
		}
	}
	else if (featureAreaPedsInvincibleUpdated)
	{
		featureAreaPedsInvincibleUpdated = false;
		//undo?
	}

	if (featureAreaPedsHeadExplode && get_frame_number() % 5 == 0)
	{
		kill_all_nearby_peds();
	}

	if (featureAreaVehiclesInvincible && get_frame_number() % 5 == 2)
	{
		set_all_nearby_vehs_to_invincible(featureAreaVehiclesInvincible);
	}
	else if (featureAreaVehiclesInvincibleUpdated)
	{
		set_all_nearby_vehs_to_invincible(false);
		featureAreaVehiclesInvincibleUpdated = false;
	}

	if (featureAreaVehiclesBroken && get_frame_number() % 5 == 2)
	{
		set_all_nearby_vehs_to_broken(featureAreaVehiclesBroken);
	}
	else if (featureAreaVehiclesBrokenUpdated)
	{
		set_all_nearby_vehs_to_broken(false);
		featureAreaVehiclesBrokenUpdated = false;
	}
}

void set_all_nearby_peds_to_calm()
{
	for each (Ped xped in lastSeenPeds)
	{
		// Only calm down peds if they're NOT in our group (keeps our bodyguards from chilling out and being lazy)
		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(xped, true);
			PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);
			PED::SET_PED_COMBAT_ATTRIBUTES(xped, 17, 1);
		}
	}
}

void set_all_nearby_peds_to_invincible()
{
	for each (Ped xped in lastSeenPeds)
	{
		// Only apply to non-bodyguards
		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			PED::SET_PED_DIES_WHEN_INJURED(xped, false);
			PED::SET_PED_MAX_HEALTH(xped, 10000);
			ENTITY::SET_ENTITY_HEALTH(xped, 10000);
			PED::SET_PED_SUFFERS_CRITICAL_HITS(xped, false);

			PED::SET_PED_COMBAT_ABILITY(xped, 1);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(xped, false);
		}
	}
}

void set_all_nearby_vehs_to_invincible(bool enabled)
{
	for each (Vehicle veh in lastSeenVehicles)
	{
		if (PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) == veh)
		{
			continue;
		}

		ENTITY::SET_ENTITY_HEALTH(veh, 10000.0f);
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 10000.0);
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 10000.0);

		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, enabled);
		VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, enabled);

		VEHICLE::SET_VEHICLE_CAN_BREAK(veh, !enabled);
		ENTITY::SET_ENTITY_INVINCIBLE(veh, enabled);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(veh, !enabled);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, !enabled);

		for (int i = 0; i < 6; i++)
		{
			VEHICLE::_SET_VEHICLE_DOOR_BREAKABLE(veh, i, !enabled); //(Vehicle, doorIndex, isBreakable)
		}

		if (enabled)
		{
			ENTITY::SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(veh, 1);

			VEHICLE::SET_VEHICLE_BODY_HEALTH(veh, 10000.0f);
		}
	}
}

void set_all_nearby_vehs_to_broken(bool enabled)
{
	for each (Vehicle veh in lastSeenVehicles)
	{
		if (PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) == veh)
		{
			continue;
		}

		if (enabled)
		{
			VEHICLE::SET_VEHICLE_ENGINE_ON(veh, false, true);
			VEHICLE::SET_VEHICLE_UNDRIVEABLE(veh, true);

			Object taskHdl;

			for (int i = -1; i < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh); i++)
			{
				Ped passenger = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, i);
				if (ENTITY::DOES_ENTITY_EXIST(passenger))
				{
					if (passenger != PLAYER::PLAYER_PED_ID())
					{
						AI::CLEAR_PED_TASKS(passenger);

						AI::OPEN_SEQUENCE_TASK(&taskHdl);
						AI::TASK_LEAVE_VEHICLE(passenger, veh, 1);
						AI::TASK_WANDER_STANDARD(passenger, 100.0, 1);
						AI::CLOSE_SEQUENCE_TASK(taskHdl);

						AI::TASK_PERFORM_SEQUENCE(passenger, taskHdl);
						AI::CLEAR_SEQUENCE_TASK(&taskHdl);
					}
				}
			}
		}
		else
		{
			//VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 1000.0f);
			VEHICLE::SET_VEHICLE_UNDRIVEABLE(veh, false);
		}
	}
}

void update_nearby_peds(Ped playerPed, int count)
{
	const int numElements = count;
	const int arrSize = numElements * 2 + 2;

	Ped *peds = new Ped[arrSize];
	peds[0] = numElements;
	int found = PED::GET_PED_NEARBY_PEDS(playerPed, peds, -1);

	for (int i = 0; i < found; i++)
	{
		int offsettedID = i * 2 + 2;

		if (!ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
		{
			continue;
		}

		Ped xped = peds[offsettedID];

		bool inSet = lastSeenPeds.find(xped) != lastSeenPeds.end();
		if (!inSet)
		{
			lastSeenPeds.insert(xped);
		}
	}

	std::set<Ped>::iterator it;
	for (it = lastSeenPeds.begin(); it != lastSeenPeds.end();)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(*it))
		{
			lastSeenPeds.erase(it++);
		}
		else
		{
			++it;
		}
	}

	delete peds;
}

void update_nearby_vehicles(Ped playerPed, int count)
{
	const int numElements = count;
	/*(const int arrSize = numElements * 2 + 2;

	Vehicle *vehs = new Vehicle[arrSize];
	vehs[0] = numElements;
	int found = PED::GET_PED_NEARBY_VEHICLES(playerPed, vehs);*/

	const int ARR_SIZE = 1024;
	Vehicle vehs[ARR_SIZE];
	int found = worldGetAllVehicles(vehs, ARR_SIZE);

	for (int i = 0; i < found; i++)
	{
		int offsettedID = i * 2 + 2;

		if (!ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
		{
			continue;
		}

		Ped xveh = vehs[offsettedID];

		bool inSet = lastSeenVehicles.find(xveh) != lastSeenVehicles.end();
		if (!inSet)
		{
			lastSeenVehicles.insert(xveh);
		}
	}

	std::set<Ped>::iterator it;
	for (it = lastSeenVehicles.begin(); it != lastSeenVehicles.end();)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(*it))
		{
			lastSeenVehicles.erase(it++);
		}
		else
		{
			++it;
		}
	}

	//delete vehs;
}

void cleanup_area_effects()
{
	lastSeenPeds.clear();
	lastSeenVehicles.clear();
}

void kill_all_nearby_peds()
{
	for each (Ped xped in lastSeenPeds)
	{
		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			//remove invincibility
			PED::SET_PED_DIES_WHEN_INJURED(xped, true);
			PED::SET_PED_MAX_HEALTH(xped, 1);
			ENTITY::SET_ENTITY_HEALTH(xped, 1);
			PED::SET_PED_SUFFERS_CRITICAL_HITS(xped, true);
			PED::SET_PED_COMBAT_ABILITY(xped, 1);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(xped, true);

			PED::EXPLODE_PED_HEAD(xped, GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE"));
		}
	}
}

void kill_all_nearby_vehicles()
{
	for each (Vehicle vehicle in lastSeenVehicles)
	{
		if (PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) != vehicle)
		{
			VEHICLE::EXPLODE_VEHICLE(vehicle, true, false);
		}
	}
}