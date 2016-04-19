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
#include "../../inc/nativeCaller.h"

int areaeffect_top_level_menu_index = 0;
int areaeffect_ped_level_menu_index = 0;
int areaeffect_veh_level_menu_index = 0;
int areaeffect_ped_advconfig_menu_index = 0;

int callsA = 0;
int callsB = 0;
int callsC = 0;
int callsD = 0;
int callsE = 0;

std::deque<ENTTrackedPedestrian*> trackedPeds;
std::deque<ENTTrackedVehicle*> trackedVehicles;

//features
bool featurePlayerIgnoredByAll = false;
bool featurePlayerIgnoredByAllUpdated = false;

bool featureAreaPedsInvincible = false;
bool featureAreaPedsInvincibleUpdated = false;

bool featureAreaVehiclesInvincible = false;
bool featureAreaVehiclesInvincibleUpdated = false;

bool featureAreaVehiclesBroken = false;
bool featureAreaVehiclesBrokenUpdated = false;

bool featureAreaPedsRioting = false;

bool featureAreaPedsRiotingUpdated = false;

bool featureAreaVehiclesExploded = false;

bool featureAreaPedsHeadExplode = false;
//bool featureAreaPedsHeadExplodeUpdated = false;

bool featureAngryPedsUseCover = false;
bool featureAngryPedsTargetYou = false;
bool featurePedsIncludeDrivers = false;
bool featurePedsIncludePilots = false;

int pedWeaponSetIndex = 0;
bool pedWeaponSetUpdated = false;

std::set<Ped> allWorldPedsThisFrame;
bool allWorldPedsThisFrameFilled = false;

std::set<Vehicle> allWorldVehiclesThisFrame;
bool allWorldVehiclesThisFrameFilled = false;

std::set<Ped> releasedPeds;
std::set<Vehicle> releasedVehicles;

void add_areaeffect_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerIgnoredByAll", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaPedsInvincible", &featureAreaPedsInvincible, &featureAreaPedsInvincibleUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaPedsHeadExplode", &featureAreaPedsHeadExplode });

	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaVehiclesInvincible", &featureAreaVehiclesInvincible, &featureAreaVehiclesInvincibleUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaVehiclesBroken", &featureAreaVehiclesBroken, &featureAreaVehiclesBrokenUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaVehiclesExploded", &featureAreaVehiclesExploded });

	results->push_back(FeatureEnabledLocalDefinition{ "featureAreaPedsRioting", &featureAreaPedsRioting, &featureAreaPedsRiotingUpdated });

	results->push_back(FeatureEnabledLocalDefinition{ "featureAngryPedsUseCover", &featureAngryPedsUseCover });
	results->push_back(FeatureEnabledLocalDefinition{ "featureAngryPedsTargetYou", &featureAngryPedsTargetYou });

	results->push_back(FeatureEnabledLocalDefinition{ "featurePedsIncludeDrivers", &featurePedsIncludeDrivers });
	results->push_back(FeatureEnabledLocalDefinition{ "featurePedsIncludePilots", &featurePedsIncludePilots });
}

void reset_areaeffect_globals()
{
	featurePlayerIgnoredByAll = false;
	featurePlayerIgnoredByAllUpdated = true;

	featureAreaVehiclesBroken = false;
	featureAreaVehiclesBrokenUpdated = true;

	featureAreaPedsInvincible = false;
	featureAreaPedsInvincibleUpdated = true;

	featureAreaPedsHeadExplode = false;

	featureAreaVehiclesExploded = false;

	featureAreaPedsRioting = false;
	featureAreaPedsRiotingUpdated = true;

	featureAngryPedsTargetYou = false;
	featureAngryPedsUseCover = false;

	featurePedsIncludeDrivers = false;
	featurePedsIncludePilots = false;

	pedWeaponSetIndex = 0;
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

	SelectFromListMenuItem* listItem = new SelectFromListMenuItem(PED_WEAPON_TITLES, onchange_areaeffect_ped_weapons);
	listItem->wrap = false;
	listItem->caption = "Peds Armed With...";
	listItem->value = pedWeaponSetIndex;
	menuItems.push_back(listItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Peds Angry";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaPedsRioting;
	togItem->toggleValueUpdated = &featureAreaPedsRiotingUpdated;
	menuItems.push_back(togItem);

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "Advanced Config";
	item->value = 1;
	item->isLeaf = false;
	menuItems.push_back(item);

	draw_generic_menu<int>(menuItems, &areaeffect_ped_level_menu_index, "Ped Effects", onconfirm_areaeffect_ped_menu, NULL, NULL);
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
	togItem->caption = "All Vehicles Abandoned";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaVehiclesBroken;
	togItem->toggleValueUpdated = &featureAreaVehiclesBrokenUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "All Vehicles Exploded";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaVehiclesExploded;
	//togItem->toggleValueUpdated = &featureAreaVehiclesExplodedUpdated;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &areaeffect_veh_level_menu_index, "Vehicle Effects", NULL, NULL, NULL);
}

void process_areaeffect_advanced_ped_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "Angry Peds Use Cover";
	togItem->value = 1;
	togItem->toggleValue = &featureAngryPedsUseCover;
	//togItem->toggleValueUpdated = &featureAreaVehiclesExplodedUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Angry Peds All Target You";
	togItem->value = 1;
	togItem->toggleValue = &featureAngryPedsTargetYou;
	//togItem->toggleValueUpdated = &featureAreaVehiclesExplodedUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Effects Include Drivers";
	togItem->value = 1;
	togItem->toggleValue = &featurePedsIncludeDrivers;
	//togItem->toggleValueUpdated = &featureAreaVehiclesExplodedUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Effects Include Pilots";
	togItem->value = 1;
	togItem->toggleValue = &featurePedsIncludePilots;
	//togItem->toggleValueUpdated = &featureAreaVehiclesExplodedUpdated;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &areaeffect_ped_advconfig_menu_index, "Advanced Ped Config", NULL, NULL, NULL);
}

bool onconfirm_areaeffect_ped_menu(MenuItem<int> choice)
{
	if (choice.value == 1) //advance options
	{
		process_areaeffect_advanced_ped_menu();
	}
	return false;
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
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Vehicles";
	item->value = -2;
	item->isLeaf = false;
	menuItems.push_back(item);

	draw_generic_menu<int>(menuItems, &areaeffect_top_level_menu_index, "Area Effects", onconfirm_areaeffect_menu, NULL, NULL);

	//		{ "Everyone Ignores You", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated, true },
}

void do_maintenance_on_tracked_entities()
{
	for each (ENTTrackedPedestrian* tped in trackedPeds)
	{
		//only apply this on average every 20 frames to save effort
		int randNum = rand() % 20;
		if (tped->angryApplied && randNum == 1)
		{
			findRandomTargetForPed(tped);
		}
	}
}

void findRandomTargetForPed(ENTTrackedPedestrian* tped)
{
	Ped otherPed = 0;
	if (tped->lastTarget == 0 || !ENTITY::DOES_ENTITY_EXIST(tped->lastTarget) || ENTITY::IS_ENTITY_DEAD(tped->lastTarget))
	{
		tped->lastTarget = 0;
		while (tped->lastTarget == 0)
		{
			int randIndex = rand() % (trackedPeds.size() + 1); //add one to the random range
			randIndex--;
			if (randIndex < 0 || featureAngryPedsTargetYou) //chance of fighting the player
			{
				otherPed = PLAYER::PLAYER_PED_ID();
				PED::SET_PED_AS_ENEMY(otherPed, true);
			}
			else
			{
				otherPed = trackedPeds.at(randIndex)->ped;
			}

			//if we've found ourselves
			if (otherPed == tped->ped)
			{
				continue;
			}

			PED::REGISTER_TARGET(tped->ped, otherPed);
			AI::TASK_COMBAT_PED(tped->ped, otherPed, 0, 16);
			tped->lastTarget = otherPed;
		}
	}
}

void update_area_effects(Ped playerPed)
{
	//callsPerFrame = 0;

	allWorldVehiclesThisFrameFilled = false;
	allWorldPedsThisFrameFilled = false;
	allWorldVehiclesThisFrame.clear();
	allWorldPedsThisFrame.clear();

	Player player = PLAYER::PLAYER_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	clear_up_missionised_entitities();

	do_maintenance_on_tracked_entities();
	
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

	if (featureAreaPedsInvincible || featureAreaPedsInvincibleUpdated)
	{
		if (get_frame_number() % 5 == 0)
		{
			set_all_nearby_peds_to_invincible(featureAreaPedsInvincible);
			featureAreaPedsInvincibleUpdated = false;
		}
	}

	if (featureAreaPedsHeadExplode)
	{
		kill_all_nearby_peds_continuous();
	}

	if (featureAreaVehiclesInvincible || featureAreaVehiclesInvincibleUpdated)
	{
		set_all_nearby_vehs_to_invincible(featureAreaVehiclesInvincible, false);
		featureAreaVehiclesInvincibleUpdated = false;
	}

	if (featureAreaVehiclesBroken || featureAreaVehiclesBrokenUpdated)
	{
		set_all_nearby_vehs_to_broken(featureAreaVehiclesBroken);
		featureAreaVehiclesBrokenUpdated = false;
	}

	if (featureAreaVehiclesExploded)
	{
		kill_all_nearby_vehicles_continuous();
	}

	if (featureAreaPedsRioting || featureAreaPedsRiotingUpdated)
	{
		set_all_nearby_peds_to_angry(featureAreaPedsRioting);
		featureAreaPedsRiotingUpdated = false;
	}

	if (pedWeaponSetIndex != 0 || pedWeaponSetUpdated )
	{
		give_all_nearby_peds_a_weapon(pedWeaponSetIndex != 0);
		pedWeaponSetUpdated = false;
	}

	std::ostringstream ss;
	ss << "Peds: " << trackedPeds.size() << "; Vehs: " << trackedVehicles.size() << "\nCalls Total: " << callsPerFrame << ", A: " << callsA << ", B: " << callsB << "\nWP: " << allWorldPedsThisFrame.size() << ", WV: " << allWorldVehiclesThisFrame.size();
	callsPerFrame = 0;
	set_status_text_centre_screen(ss.str());
}

void set_all_nearby_peds_to_calm()
{
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());
	for each (Ped xped in peds)
	{
		// Only calm down peds if they're NOT in our group (keeps our bodyguards from chilling out and being lazy)
		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(xped, true);
			PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);
			//PED::SET_PED_COMBAT_ATTRIBUTES(xped, 17, 1);
		}
	}
}

void set_all_nearby_peds_to_invincible(bool enabled)
{
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());
	for each (Ped xped in peds)
	{
		// Only apply to non-bodyguards
		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);
			if ((enabled && !trackedPed->madeInvincible) || (!enabled && trackedPed->madeInvincible))
			{
				PED::SET_PED_DIES_WHEN_INJURED(xped, !enabled);

				PED::SET_PED_MAX_HEALTH(xped, enabled ? 10000 : 100 );
				ENTITY::SET_ENTITY_HEALTH(xped, enabled ? 10000 : 100);
				PED::SET_PED_SUFFERS_CRITICAL_HITS(xped, !enabled);

				PED::SET_PED_COMBAT_ABILITY(xped, enabled ? 1 : 0);
				ENTITY::SET_ENTITY_CAN_BE_DAMAGED(xped, !enabled);

				trackedPed->madeInvincible = enabled;
			}
		}
	}
}

void set_all_nearby_peds_to_angry(bool enabled)
{
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());

	for each (Ped xped in peds)
	{
		// Only apply to non-bodyguards
		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);

			if (enabled && !trackedPed->angryApplied)
			{
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 0, featureAngryPedsUseCover ? 1 : 0); //use cover

				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 46, 1); //always fight
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 5, 1); //fight armed peds when unarmed
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 3, featurePedsIncludeDrivers ? 1 : 0); //can leave vehicle
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 2, featurePedsIncludeDrivers ? 1 : 0); //can do driveby
				PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);

				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(xped, true);

				findRandomTargetForPed(trackedPed);

				trackedPed->angryApplied = true;
				trackedPed->missionise();
			}
			else if(!enabled && trackedPed->angryApplied)
			{
				//stop them being angry
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 0, 0); //use cover
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 46, 0); //always fight
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 5, 0); //fight armed peds when unarmed
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 3, 0); //can leave vehicle
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 2, 0); //can do driveby

				PED::SET_PED_AS_ENEMY(xped, enabled);
				PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);

				trackedPed->angryApplied = false;
				trackedPed->demissionise();
				releasedPeds.insert(xped);
			}
		}
	}
}

void set_all_nearby_vehs_to_invincible(bool enabled, bool force)
{
	std::set<Vehicle> vehicles = get_nearby_vehicles(PLAYER::PLAYER_PED_ID());

	for each (Vehicle veh in vehicles)
	{
		int chanceOfSelection = rand() % 5;
		if (chanceOfSelection != 1 || force)
		{
			continue;
		}

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
	std::set<Vehicle> vehicles = get_nearby_vehicles(PLAYER::PLAYER_PED_ID());

	for each (Vehicle veh in vehicles)
	{
		int chanceOfSelection = rand() % 5;
		if (chanceOfSelection != 1)
		{
			continue;
		}

		if (PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) == veh)
		{
			continue;
		}

		Hash vehModel = ENTITY::GET_ENTITY_MODEL(veh);

		BOOL isAircraft = VEHICLE::IS_THIS_MODEL_A_HELI(vehModel) || VEHICLE::IS_THIS_MODEL_A_PLANE(vehModel);
		BOOL isWeird = VEHICLE::IS_THIS_MODEL_A_TRAIN(vehModel) || VEHICLE::IS_THIS_MODEL_A_BOAT(vehModel) || VEHICLE::_IS_THIS_MODEL_A_SUBMERSIBLE(vehModel);
		if (isAircraft || isWeird)
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

std::set<Ped> get_nearby_peds(Ped playerPed)
{
	if (allWorldPedsThisFrameFilled)
	{
		return allWorldPedsThisFrame;
	}

	const int ARR_SIZE = 1024;
	Ped peds[ARR_SIZE];
	int found = worldGetAllPeds(peds, ARR_SIZE);

	callsA = 0;
	callsB = found;

	for (int i = 0; i < found; i++)
	{
		Ped item = peds[i];

		if (releasedPeds.find(item) != releasedPeds.end())
		{
			continue;
		}

		if (ENTITY::IS_ENTITY_DEAD(item))
		{
			continue;
		}
		else if (playerPed == item)
		{
			continue;
		}
		else if (!PED::IS_PED_HUMAN(item))
		{
			continue;
		}
		else if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(item) && !ENTITY::DOES_ENTITY_BELONG_TO_THIS_SCRIPT(item, true))
		{
			continue;
		}

		//filter out drivers/pilots if necessary
		if ((!featurePedsIncludePilots || !featurePedsIncludeDrivers) && PED::IS_PED_IN_ANY_VEHICLE(item, false))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(item, false);

			Hash vehModel = ENTITY::GET_ENTITY_MODEL(veh);

			bool isAircraft = VEHICLE::IS_THIS_MODEL_A_HELI(vehModel) || VEHICLE::IS_THIS_MODEL_A_PLANE(vehModel);
			if (!featurePedsIncludePilots && isAircraft)
			{
				continue;
			}

			bool isWeird = VEHICLE::IS_THIS_MODEL_A_TRAIN(vehModel) || VEHICLE::IS_THIS_MODEL_A_BOAT(vehModel) || VEHICLE::_IS_THIS_MODEL_A_SUBMERSIBLE(vehModel);
			if (!featurePedsIncludeDrivers && !isAircraft && !isWeird)
			{
				continue;
			}
		}

		allWorldPedsThisFrame.insert(item);
	}

	allWorldPedsThisFrameFilled = true;
	return allWorldPedsThisFrame;
}

std::set<Vehicle> get_nearby_vehicles(Ped playerPed)
{
	if (allWorldVehiclesThisFrameFilled)
	{
		return allWorldVehiclesThisFrame;
	}

	BOOL playerInVehicle = PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0);
	Vehicle playerVehicle = playerInVehicle ? PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : 0;

	const int ARR_SIZE = 1024;
	Vehicle vehs[ARR_SIZE];
	int found = worldGetAllVehicles(vehs, ARR_SIZE);

	for (int i = 0; i < found; i++)
	{
		Vehicle item = vehs[i];

		if (releasedVehicles.find(item) != releasedVehicles.end())
		{
			continue;
		}

		//don't return dead items
		if (ENTITY::IS_ENTITY_DEAD(item))
		{
			continue;
		}
		//don't return the vehicle we're using
		else if (playerInVehicle && playerVehicle == item)
		{
			continue;
		}
		//don't do stuff to mission entities
		else if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(item) && !ENTITY::DOES_ENTITY_BELONG_TO_THIS_SCRIPT(item, true))
		{
			continue;
		}

		allWorldVehiclesThisFrame.insert(item);
	}

	allWorldVehiclesThisFrameFilled = true;
	return allWorldVehiclesThisFrame;
}

void cleanup_area_effects()
{
	for each (ENTTrackedVehicle* veh in trackedVehicles)
	{
		delete veh;
	}
	trackedVehicles.clear();

	for each (ENTTrackedPedestrian* ped in trackedPeds)
	{
		delete ped;
	}
	trackedPeds.clear();
}

void kill_all_nearby_peds_now()
{
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());

	for each (Ped xped in peds)
	{
		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(xped, true, true);
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);
			trackedPed->missionise();

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

void kill_all_nearby_peds_continuous()
{
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());

	for each (Ped xped in peds)
	{
		int chanceOfSelection = rand() % 5;
		if (chanceOfSelection != 1)
		{
			continue;
		}

		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(xped, true, true);
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);
			trackedPed->missionise();

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

void kill_all_nearby_vehicles_now()
{
	std::set<Vehicle> vehicles = get_nearby_vehicles(PLAYER::PLAYER_PED_ID());

	set_all_nearby_vehs_to_invincible(false, true);

	for each (Vehicle vehicle in vehicles)
	{
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehicle, true, true);
		ENTTrackedVehicle* trackedVeh = findOrCreateTrackedVehicle(vehicle);
		trackedVeh->missionise();

		VEHICLE::EXPLODE_VEHICLE(vehicle, true, false);
	}
}

void kill_all_nearby_vehicles_continuous()
{
	std::set<Vehicle> vehicles = get_nearby_vehicles(PLAYER::PLAYER_PED_ID());

	for each (Vehicle vehicle in vehicles)
	{
		int chanceOfBeingExploded = rand() % 5;
		if (chanceOfBeingExploded != 1)
		{
			continue;
		}

		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehicle, true, true);
		ENTTrackedVehicle* trackedVeh = findOrCreateTrackedVehicle(vehicle);
		trackedVeh->missionise();

		VEHICLE::EXPLODE_VEHICLE(vehicle, true, false);
	}
}

void clear_up_missionised_entitities()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	std::deque<ENTTrackedVehicle*>::iterator vit;
	for (vit = trackedVehicles.begin(); vit != trackedVehicles.end();)
	{
		Vehicle actualVeh = (*vit)->vehicle;
		if (!ENTITY::DOES_ENTITY_EXIST(actualVeh) || is_entity_to_be_disposed(playerPed, actualVeh))
		{
			delete *vit;
			releasedVehicles.insert(actualVeh);
			vit = trackedVehicles.erase(vit);
		}
		else
		{
			++vit;
		}
	}

	std::deque<ENTTrackedPedestrian*>::iterator pit;
	for (pit = trackedPeds.begin(); pit != trackedPeds.end();)
	{
		Ped actualPed = (*pit)->ped;
		if (!ENTITY::DOES_ENTITY_EXIST(actualPed)|| is_entity_to_be_disposed(playerPed, actualPed))
		{
			delete *pit;
			releasedPeds.insert(actualPed);
			pit = trackedPeds.erase(pit);
		}
		else
		{
			++pit;
		}
	}

	std::set<Ped>::iterator rpIt;
	for (rpIt = releasedPeds.begin(); rpIt != releasedPeds.end();)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(*rpIt))
		{
			rpIt = releasedPeds.erase(rpIt);
		}
		else if (!is_entity_to_be_disposed(playerPed, *rpIt)) //no longer for deletion after all
		{
			rpIt = releasedPeds.erase(rpIt);
		}
		else
		{
			rpIt++;
		}
	}

	std::set<Vehicle>::iterator rvIt;
	for (rvIt = releasedVehicles.begin(); rvIt != releasedVehicles.end();)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(*rvIt))
		{
			rvIt = releasedVehicles.erase(rvIt);
		}
		else if (!is_entity_to_be_disposed(playerPed, *rvIt)) //no longer for deletion after all
		{
			rvIt = releasedVehicles.erase(rvIt);
		}
		else
		{
			rvIt++;
		}
	}
}

void onchange_areaeffect_ped_weapons(int value, SelectFromListMenuItem* source)
{
	pedWeaponSetIndex = value;
	pedWeaponSetUpdated = true;
}

void give_all_nearby_peds_a_weapon(bool enabled)
{
	//callsPerFrame = 0;

	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());

	//callsA = callsPerFrame;
	//callsPerFrame = 0;

	for each (Ped xped in peds)
	{
		int chanceOfGettingWeapon = rand() % 5;
		if (chanceOfGettingWeapon != 1)
		{
			continue;
		}

		if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())))
		{
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);

			if (enabled && !trackedPed->weaponSetApplied != pedWeaponSetIndex)
			{
				std::vector<std::string> weaponSet = VOV_PED_WEAPONS[pedWeaponSetIndex];

				int index = rand() % weaponSet.size();
				std::string weapon = weaponSet.at(index);
				Hash weapHash = GAMEPLAY::GET_HASH_KEY((char *)weapon.c_str());

				bool foundWeapon = false;
				for (std::string searchStr : weaponSet)
				{
					Hash searchHash = GAMEPLAY::GET_HASH_KEY((char *)searchStr.c_str());
					if (trackedPed->lastWeaponApplied == searchHash)
					{
						foundWeapon = true;
						break;
					}
				}

				if (!foundWeapon)
				{
					WEAPON::GIVE_WEAPON_TO_PED(xped, weapHash, 9999, FALSE, TRUE);
					WEAPON::SET_PED_INFINITE_AMMO_CLIP(xped, true);
					PED::SET_PED_CAN_SWITCH_WEAPON(xped, true);
					if (WEAPON::HAS_PED_GOT_WEAPON(xped, weapHash, 0) && !PED::IS_PED_IN_ANY_VEHICLE(xped, false) && !PED::IS_PED_INJURED(xped))
					{
						WEAPON::SET_CURRENT_PED_WEAPON(xped, weapHash, 0);
					}
					trackedPed->lastWeaponApplied = weapHash;
				}

				trackedPed->weaponSetApplied = pedWeaponSetIndex;
			}
			else if (!enabled && trackedPed->weaponSetApplied != 0)
			{
				//TODO: take all their weapons from that set away?

				trackedPed->weaponSetApplied = 0;
			}
		}
	}
}

void add_areaeffect_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "pedWeaponSetIndex", std::to_string(pedWeaponSetIndex) });
}

void handle_generic_settings_areaeffect(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("pedWeaponSetIndex") == 0)
		{
			pedWeaponSetIndex = stoi(setting.value);
			pedWeaponSetUpdated = true;
		}
	}
}

ENTTrackedPedestrian* findOrCreateTrackedPed(Ped searchPed)
{
	std::deque<ENTTrackedPedestrian*>::iterator pit;
	for (pit = trackedPeds.begin(); pit != trackedPeds.end();pit++)
	{
		Ped queuePed = (*pit)->ped;

		if (searchPed == queuePed)
		{
			return *pit;
		}
	}
	//if nowt found
	ENTTrackedPedestrian* result = new ENTTrackedPedestrian(searchPed);
	trackedPeds.push_back(result);

	return result;
}

ENTTrackedVehicle* findOrCreateTrackedVehicle(Vehicle searchVeh)
{
	std::deque<ENTTrackedVehicle*>::iterator vit;
	for (vit = trackedVehicles.begin(); vit != trackedVehicles.end(); vit++)
	{
		Vehicle queueVeh = (*vit)->vehicle;
		if (searchVeh == queueVeh)
		{
			return *vit;
		}
	}
	//if nowt found
	ENTTrackedVehicle* result = new ENTTrackedVehicle(searchVeh);
	trackedVehicles.push_back(result);
	return result;
}


bool is_entity_to_be_disposed(Ped playerPed, Entity entity)
{
	return (!ENTITY::IS_ENTITY_ON_SCREEN(entity) && !ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, entity, 17));
}