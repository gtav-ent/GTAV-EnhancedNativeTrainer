/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "..\io\config_io.h"
#include <string.h>

#include "bodyguards.h"

int activeLineIndexBodyguards = 0;

bool featureBodyguardInvincible = false;
bool featureBodyguardInvincibleUpdated = false;
bool featureBodyguardHelmet = false;
bool featureBodyguardInfAmmo = false;

bool requireRefreshOfBodyguardMainMenu = false;

//first index is which category, second is position in that category
int skinTypesBodyguardMenuPositionMemory[2] = { 0, 0 };

Hash bodyGuardWeapon = NULL;

std::vector<Ped> spawnedBodyguards;

bool process_bodyguard_skins_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	MenuItem<int> *item;

	item = new MenuItem<int>();
	item->caption = "Players";
	item->value = 0;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "NPCs";
	item->value = 1;
	item->isLeaf = false;
	menuItems.push_back(item);

	return draw_generic_menu<int>(menuItems, &skinTypesBodyguardMenuPositionMemory[0], "Bodyguard Skins", onconfirm_bodyguard_skins_menu, NULL, NULL);
}

bool onconfirm_bodyguard_skins_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case 0:
		return process_player_skins_menu();
	case 1:
		return process_npc_skins_menu();
	default:
		break;
	}
	return false;
}

std::string get_current_model_name()
{
	std::string value;
	switch (skinTypesBodyguardMenuPositionMemory[0])
	{
	case 0:
		value = SKINS_PLAYER_CAPTIONS[skinTypesBodyguardMenuPositionMemory[1]];
		break;
	case 1:
		value = SKINS_GENERAL_CAPTIONS[skinTypesBodyguardMenuPositionMemory[1]];
		break;
	default:
		value = SKINS_GENERAL_CAPTIONS[0];
		break;
	}
	return value;
}

Hash get_current_model_hash()
{
	std::string value;
	switch (skinTypesBodyguardMenuPositionMemory[0])
	{
	case 0:
		value = SKINS_PLAYER_VALUES[skinTypesBodyguardMenuPositionMemory[1]];
		break;
	case 1:
		value = SKINS_GENERAL_VALUES[skinTypesBodyguardMenuPositionMemory[1]];
		break;
	default:
		value = SKINS_GENERAL_VALUES[0];
		break;
	}
	return GAMEPLAY::GET_HASH_KEY((char*)value.c_str());
}

bool onconfirm_bodyguards_skins_players(MenuItem<std::string> choice)
{
	skinTypesBodyguardMenuPositionMemory[0] = 0;
	skinTypesBodyguardMenuPositionMemory[1] = choice.currentMenuIndex;
	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool onconfirm_bodyguards_skins_npcs(MenuItem<std::string> choice)
{
	skinTypesBodyguardMenuPositionMemory[0] = 1;
	skinTypesBodyguardMenuPositionMemory[1] = choice.currentMenuIndex;

	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool process_player_skins_menu()
{
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < SKINS_PLAYER_CAPTIONS.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_PLAYER_CAPTIONS[i];
		item->value = SKINS_PLAYER_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "Player Skins", onconfirm_bodyguards_skins_players, NULL, NULL);
}

bool process_npc_skins_menu()
{
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < SKINS_GENERAL_CAPTIONS.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_GENERAL_CAPTIONS[i];
		item->value = SKINS_GENERAL_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "NPC Skins", onconfirm_bodyguards_skins_npcs, NULL, NULL);
}

bool process_bodyguard_weapons_menu()
{
	// For now we'll just give every weapon to the bodyguards
	// Maybe down the road we can allow a player to choose one or more weapons to give to a bodyguard
	return false;
}

void dismiss_bodyguards() {

	if (spawnedBodyguards.size() == 0) {
		set_status_text("You don't have any bodyguards");
		return;
	}

	for (int i = 0; i < spawnedBodyguards.size(); i++)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(spawnedBodyguards[i], false);
		PED::REMOVE_PED_FROM_GROUP(spawnedBodyguards[i]);
		AI::CLEAR_PED_TASKS(spawnedBodyguards[i]);
	}

	spawnedBodyguards.clear();
	requireRefreshOfBodyguardMainMenu = true;

	set_status_text("Bodyguards dismissed");
}

void do_spawn_bodyguard() {

	requireRefreshOfBodyguardMainMenu = true;

	if (spawnedBodyguards.size() >= 7)
	{
		set_status_text("Cannot spawn any more bodyguards");
		return;
	}

	DWORD bodyGuardModel = get_current_model_hash();

	if (STREAMING::IS_MODEL_IN_CDIMAGE(bodyGuardModel) && STREAMING::IS_MODEL_VALID(bodyGuardModel))
	{
		STREAMING::REQUEST_MODEL(bodyGuardModel);
		while (!STREAMING::HAS_MODEL_LOADED(bodyGuardModel))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		int myGroup = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID());

		Vector3 spawnCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 2.5, 2.5, 0.0);
		Ped bodyGuard = PED::CREATE_PED(25, bodyGuardModel, spawnCoords.x, spawnCoords.y, spawnCoords.z, 0, 0, 0);

		spawnedBodyguards.push_back(bodyGuard); // save the current bodyguard

		PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), myGroup);
		PED::SET_PED_AS_GROUP_MEMBER(bodyGuard, myGroup);
		PED::SET_PED_NEVER_LEAVES_GROUP(bodyGuard, true);

		if (featureBodyguardInvincible)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(bodyGuard, true);
		}

		if (featureBodyguardHelmet)
		{
			PED::GIVE_PED_HELMET(bodyGuard, 1, 4096, -1);
		}

		PED::SET_PED_COMBAT_ABILITY(bodyGuard, 2);
		PED::SET_PED_COMBAT_RANGE(bodyGuard, 2);


		// throw every weapon at the bodyguards
		static Hash weaponList[] = { WEAPON_ADVANCEDRIFLE, WEAPON_APPISTOL, WEAPON_ASSAULTRIFLE, WEAPON_ASSAULTSHOTGUN, WEAPON_ASSAULTSMG, WEAPON_BALL, WEAPON_BAT, WEAPON_BOTTLE, WEAPON_BULLPUPSHOTGUN, WEAPON_CARBINERIFLE, WEAPON_COMBATMG, WEAPON_COMBATPDW, WEAPON_COMBATPISTOL, WEAPON_CROWBAR, WEAPON_DAGGER, WEAPON_FIREEXTINGUISHER, WEAPON_FIREWORK, WEAPON_FLAREGUN, WEAPON_GOLFCLUB, WEAPON_GRENADE, WEAPON_GRENADELAUNCHER, WEAPON_GUSENBERG, WEAPON_HAMMER, WEAPON_HEAVYPISTOL, WEAPON_HEAVYSHOTGUN, WEAPON_HEAVYSNIPER, WEAPON_HOMINGLAUNCHER, WEAPON_KNIFE, WEAPON_KNUCKLE, WEAPON_MARKSMANPISTOL, WEAPON_MARKSMANRIFLE, WEAPON_MG, WEAPON_MICROSMG, WEAPON_MOLOTOV, WEAPON_MUSKET, WEAPON_NIGHTSTICK, WEAPON_PETROLCAN, WEAPON_PISTOL, WEAPON_PISTOL50, WEAPON_PROXMINE, WEAPON_PUMPSHOTGUN, WEAPON_RPG, WEAPON_SAWNOFFSHOTGUN, WEAPON_SMG, WEAPON_SMOKEGRENADE, WEAPON_SNIPERRIFLE, WEAPON_SNOWBALL, WEAPON_SNSPISTOL, WEAPON_SPECIALCARBINE, WEAPON_STICKYBOMB, WEAPON_STUNGUN, WEAPON_VINTAGEPISTOL, WEAPON_MINIGUN, WEAPON_RAILGUN };
		
		for (int i = 0; i < sizeof(weaponList) / sizeof(Hash); i++)
		{
			if (WEAPON::HAS_PED_GOT_WEAPON(bodyGuard, weaponList[i], FALSE) == FALSE)
			{
				WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, weaponList[i], 9999, FALSE, TRUE);
			}
			if (WEAPON::GET_WEAPONTYPE_GROUP(weaponList[i]) == WEAPON_TYPE_GROUP_THROWABLE)
			{
				WEAPON::REMOVE_WEAPON_FROM_PED(bodyGuard, weaponList[i]);
				WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, weaponList[i], 9999, FALSE, TRUE);
			}
		}

		if (featureBodyguardInfAmmo)
		{
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(bodyGuard, true);
		}

		PED::SET_PED_CAN_SWITCH_WEAPON(bodyGuard, true);
		PED::SET_GROUP_FORMATION(myGroup, 1);

		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(bodyGuard);
		WAIT(0);

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

			for (int i = 0; i <= VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh) - 1; i++) {
				if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i)) {
					AI::TASK_WARP_PED_INTO_VEHICLE(bodyGuard, veh, i); break;
					PED::SET_PED_INTO_VEHICLE(bodyGuard, veh, i);
				}
			}
		}

		WAIT(50);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(bodyGuardModel);
	}
	return;
}

void maintain_bodyguards()
{
	std::vector<Ped>::iterator iter = spawnedBodyguards.begin();

	while (iter != spawnedBodyguards.end())
	{
		if (PED::IS_PED_DEAD_OR_DYING(*iter, true))
		{
			// clean up PED stuff, for now let's assume the game handles everything and we just worry about our vector
			iter = spawnedBodyguards.erase(iter);
			requireRefreshOfBodyguardMainMenu = true;
		}
		else
		{
			++iter;
		}
	}
}

bool process_bodyguard_menu()
{
	do
	{
		requireRefreshOfBodyguardMainMenu = false;
		int i = 0;

		std::string caption = "Bodyguard Options";

		std::vector<MenuItem<int>*> menuItems;
		MenuItem<int> *item;
		ToggleMenuItem<int>* toggleItem;

		item = new MenuItem<int>();
		std::ostringstream ss;
		ss << "Spawn Bodyguard: " << get_current_model_name();
		item->caption = ss.str();
		item->value = i++;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Dismiss All Bodyguards";
		item->value = i++;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Choose Model";
		item->value = i++;
		item->isLeaf = false;
		menuItems.push_back(item);
		/*
		item = new MenuItem<int>();
		item->caption = "Choose Weapon";
		item->value = i++;
		item->isLeaf = false;
		menuItems.push_back(item);
		*/
		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Invincible";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardInvincible;
		toggleItem->toggleValueUpdated = &featureBodyguardInvincibleUpdated;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Infinite Ammo";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardInfAmmo;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Spawn With Helmet";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardHelmet;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		draw_generic_menu<int>(menuItems, &activeLineIndexBodyguards, caption, onconfirm_bodyguard_menu, NULL, NULL, bodyguards_main_menu_interrupt);
	}
	while (requireRefreshOfBodyguardMainMenu);
	return false;
}

bool onconfirm_bodyguard_menu(MenuItem<int> choice)
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	switch (choice.value)
	{
	case 0:
		do_spawn_bodyguard();
		break;
	case 1:
		dismiss_bodyguards();
		break;
	case 2:
		process_bodyguard_skins_menu();
		break;
	case 3:
		//process_bodyguard_weapons_menu();
		break;
	default:
		break;
	}
	return false;
}

void update_bodyguard_features()
{
	if (featureBodyguardInvincibleUpdated)
	{
		for (int i = 0; i < spawnedBodyguards.size(); i++)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(spawnedBodyguards[i], featureBodyguardInvincible);
		}
	}
}

void add_bodyguards_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featureBodyguardInvincible", &featureBodyguardInvincible });
	results->push_back(FeatureEnabledLocalDefinition{ "featureBodyguardHelmet", &featureBodyguardHelmet });
	results->push_back(FeatureEnabledLocalDefinition{ "featureBodyguardInfAmmo", &featureBodyguardInfAmmo });
}

void add_bodyguards_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "skinTypesBodyguardMenuPositionMemory0", std::to_string(skinTypesBodyguardMenuPositionMemory[0]) });
	results->push_back(StringPairSettingDBRow{ "skinTypesBodyguardMenuPositionMemory1", std::to_string(skinTypesBodyguardMenuPositionMemory[1]) });
}

void handle_generic_settings_bodyguards(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("skinTypesBodyguardMenuPositionMemory0") == 0)
		{
			skinTypesBodyguardMenuPositionMemory[0] = stoi(setting.value);
		}
		else if (setting.name.compare("skinTypesBodyguardMenuPositionMemory1") == 0)
		{
			skinTypesBodyguardMenuPositionMemory[1] = stoi(setting.value);
		}
	}
}

bool bodyguards_main_menu_interrupt()
{
	if (requireRefreshOfBodyguardMainMenu)
	{
		return true;
	}
	return false;
}