/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "..\ui_support\menu_functions.h"
#include "weapons.h"
#include "..\io\config_io.h"

int activeLineIndexWeapon = 0;
int lastSelectedWeaponCategory = 0;
int lastSelectedWeapon = 0;

int weapDmgModIndex = 0;

bool featureWeaponInfiniteAmmo = false;
bool featureWeaponInfiniteParachutes = false;
bool featureWeaponNoReload = false;
bool featureWeaponFireAmmo = false;
bool featureWeaponExplosiveAmmo = false;
bool featureWeaponExplosiveMelee = false;
bool featureWeaponVehRockets = false;

DWORD featureWeaponVehShootLastTime = 0;

int saved_weapon_model[TOTAL_WEAPONS_SLOTS];
int saved_ammo[TOTAL_WEAPONS_SLOTS];
int saved_clip_ammo[TOTAL_WEAPONS_SLOTS];
int saved_weapon_tints[TOTAL_WEAPONS_SLOTS];
bool saved_weapon_mods[TOTAL_WEAPONS_SLOTS][MAX_MOD_SLOTS];
bool saved_parachute = false;
int saved_armour = 0;

bool redrawWeaponMenuAfterEquipChange = false;

void onchange_knuckle_appearance(int value, SelectFromListMenuItem* source)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int weapHash = GAMEPLAY::GET_HASH_KEY("WEAPON_KNUCKLE");

	int i = 0;

	Hash hashToApply = 0;
	for each (std::string componentName in VALUES_ATTACH_KNUCKLES)
	{
		DWORD componentHash = GAMEPLAY::GET_HASH_KEY((char *)componentName.c_str());

		WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, weapHash, componentHash);
		
		if ( i==value )
		{
			hashToApply = componentHash;
		}

		i++;
	}

	if ( hashToApply != 0)
	{
		WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, weapHash, hashToApply);
	}
}

int get_current_knuckle_appearance()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int weapHash = GAMEPLAY::GET_HASH_KEY("WEAPON_KNUCKLE");

	int i = 0;
	for each (std::string componentName in VALUES_ATTACH_KNUCKLES)
	{
		if (i == 0)
		{
			continue;
		}

		DWORD componentHash = GAMEPLAY::GET_HASH_KEY((char *)componentName.c_str());

		if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, weapHash, componentHash))
		{
			return i;
		}

		i++;
	}
	
	return 0;
}

void onchange_switchblade_appearance(int value, SelectFromListMenuItem* source)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int weapHash = GAMEPLAY::GET_HASH_KEY("WEAPON_SWITCHBLADE");

	int i = 0;

	Hash hashToApply = 0;
	for each (std::string componentName in VALUES_ATTACH_SWITCHBLADE)
	{
		DWORD componentHash = GAMEPLAY::GET_HASH_KEY((char *)componentName.c_str());

		WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, weapHash, componentHash);

		if (i == value)
		{
			hashToApply = componentHash;
		}

		i++;
	}

	if (hashToApply != 0)
	{
		WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, weapHash, hashToApply);
	}
}

int get_current_switchblade_appearance()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int weapHash = GAMEPLAY::GET_HASH_KEY("WEAPON_SWITCHBLADE");

	int i = 0;
	for each (std::string componentName in VALUES_ATTACH_SWITCHBLADE)
	{
		if (i == 0)
		{
			continue;
		}

		DWORD componentHash = GAMEPLAY::GET_HASH_KEY((char *)componentName.c_str());

		if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, weapHash, componentHash))
		{
			return i;
		}

		i++;
	}

	return 0;
}

bool process_individual_weapon_menu(int weaponIndex)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	int originalWeapon = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	lastSelectedWeapon = weaponIndex;

	std::string caption = VOV_WEAPON_CAPTIONS[lastSelectedWeaponCategory].at(weaponIndex);
	if (caption.compare("Pistol .50") == 0)
	{
		caption = "Pistol"; //menu title can't handle symbols
	}

	std::string value = VOV_WEAPON_VALUES[lastSelectedWeaponCategory].at(weaponIndex);
	std::vector<MenuItem<int>*> menuItems;

	std::string weaponValue = VOV_WEAPON_VALUES[lastSelectedWeaponCategory].at(weaponIndex);
	char *weaponChar = (char*)weaponValue.c_str();
	int thisWeaponHash = GAMEPLAY::GET_HASH_KEY(weaponChar);
	bool isEquipped = (WEAPON::HAS_PED_GOT_WEAPON(playerPed, GAMEPLAY::GET_HASH_KEY(weaponChar), 0) ? true : false);

	WEAPON::SET_CURRENT_PED_WEAPON(playerPed, thisWeaponHash, true);

	FunctionDrivenToggleMenuItem<int> *equipItem = new FunctionDrivenToggleMenuItem<int>();
	std::stringstream ss;
	ss << "Equip "<<caption<<"?";
	equipItem->caption = ss.str();
	equipItem->value = 1;
	equipItem->getter_call = is_weapon_equipped;
	equipItem->setter_call = set_weapon_equipped;
	equipItem->extra_arguments.push_back(lastSelectedWeaponCategory);
	equipItem->extra_arguments.push_back(weaponIndex);
	menuItems.push_back(equipItem);

	if (isEquipped)
	{

		Ped playerPed = PLAYER::PLAYER_PED_ID();
		std::string weaponValue = VOV_WEAPON_VALUES[lastSelectedWeaponCategory].at(lastSelectedWeapon);
		char *weaponChar = (char*)weaponValue.c_str();
		int weapHash = GAMEPLAY::GET_HASH_KEY(weaponChar);
		int maxClipAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, weapHash, false);

		if (maxClipAmmo > 0)
		{
			MenuItem<int> *giveClipItem = new MenuItem<int>();
			giveClipItem->caption = "Give Clip";
			giveClipItem->value = 2;
			giveClipItem->isLeaf = true;
			giveClipItem->onConfirmFunction = give_weapon_clip;
			menuItems.push_back(giveClipItem);

			MenuItem<int> *fillAmmoItem = new MenuItem<int>();
			fillAmmoItem->caption = "Fill Ammo";
			fillAmmoItem->value = 3;
			fillAmmoItem->isLeaf = true;
			fillAmmoItem->onConfirmFunction = fill_weapon_ammo;
			menuItems.push_back(fillAmmoItem);
		}

		int moddableIndex = -1;
		for (int i = 0; i < WEAPONTYPES_MOD.size(); i++)
		{
			if (weaponValue.compare(WEAPONTYPES_MOD.at(i)) == 0)
			{
				moddableIndex = i;
				break;
			}
		}

		if (moddableIndex != -1)
		{
			std::vector<std::string> modCaptions = VOV_WEAPONMOD_CAPTIONS[moddableIndex];
			for (int i = 0; i < modCaptions.size(); i++)
			{
				FunctionDrivenToggleMenuItem<int> *item = new FunctionDrivenToggleMenuItem<int>();
				item->caption = modCaptions.at(i);
				item->getter_call = is_weaponmod_equipped;
				item->setter_call = set_weaponmod_equipped;
				item->extra_arguments.push_back(lastSelectedWeaponCategory);
				item->extra_arguments.push_back(weaponIndex);
				item->extra_arguments.push_back(moddableIndex);
				item->extra_arguments.push_back(i);
				menuItems.push_back(item);
			}
		}

		if (strcmp(weaponChar, "WEAPON_KNUCKLE") == 0)
		{
			SelectFromListMenuItem *listItem = new SelectFromListMenuItem(CAPTIONS_ATTACH_KNUCKLES, onchange_knuckle_appearance);
			listItem->wrap = false;
			listItem->caption = "Skin Choice";
			listItem->value = get_current_knuckle_appearance();
			menuItems.push_back(listItem);
		}

		if (strcmp(weaponChar, "WEAPON_SWITCHBLADE") == 0)
		{
			SelectFromListMenuItem *listItem = new SelectFromListMenuItem(CAPTIONS_ATTACH_SWITCHBLADE, onchange_switchblade_appearance);
			listItem->wrap = false;
			listItem->caption = "Skin Choice";
			listItem->value = get_current_switchblade_appearance();
			menuItems.push_back(listItem);
		}

		int tintableIndex = -1;
		for (int i = 0; i < WEAPONTYPES_TINT.size(); i++)
		{
			if (weaponValue.compare(WEAPONTYPES_TINT.at(i)) == 0)
			{
				tintableIndex = i;
				break;
			}
		}

		if (tintableIndex != -1)
		{
			MenuItem<int> *tintItem = new MenuItem<int>();
			tintItem->caption = "Weapon Tints";
			tintItem->value = 4;
			tintItem->isLeaf = false;
			tintItem->onConfirmFunction = onconfirm_open_tint_menu;
			menuItems.push_back(tintItem);
		}
	}

	draw_generic_menu<int>(menuItems, 0, caption, NULL, NULL, NULL, weapon_reequip_interrupt);

	int unarmed = GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED");
	if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, originalWeapon, 0))
	{
		if (originalWeapon != unarmed)
		{
			WEAPON::SET_CURRENT_PED_WEAPON(playerPed, originalWeapon, true);
		}
	}
	else
	{
		WEAPON::SET_CURRENT_PED_WEAPON(playerPed, unarmed, true);
	}

	return false;
}

bool weapon_reequip_interrupt()
{
	return redrawWeaponMenuAfterEquipChange;
}

bool onconfirm_weapon_in_category(MenuItem<int> choice)
{
	do
	{
		redrawWeaponMenuAfterEquipChange = false;
		process_individual_weapon_menu(choice.value);
	} while (redrawWeaponMenuAfterEquipChange);

	return false;
}


bool process_weapons_in_category_menu(int category)
{
	lastSelectedWeaponCategory = category;
	std::vector<MenuItem<int>*> menuItems;

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int weaponSelectionIndex = 0;
	int current = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	for (int i = 0; i < VOV_WEAPON_CAPTIONS[category].size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = VOV_WEAPON_CAPTIONS[category].at(i);

		const char* value = VOV_WEAPON_VALUES[category].at(i).c_str();
		if (weaponSelectionIndex == 0 && GAMEPLAY::GET_HASH_KEY((char*)value) == current)
		{
			weaponSelectionIndex = i;
		}

		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &weaponSelectionIndex, MENU_WEAPON_CATEGORIES[category], onconfirm_weapon_in_category, NULL, NULL);
}

bool onconfirm_weaponlist_menu(MenuItem<int> choice)
{
	process_weapons_in_category_menu(choice.value);
	return false;
}

bool process_weaponlist_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int weaponSelectionIndex = 0;
	int current = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	for (int i = 0; i < MENU_WEAPON_CATEGORIES.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_WEAPON_CATEGORIES[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);

		if (weaponSelectionIndex == 0)
		{
			for (int j = 0; j < VOV_WEAPON_VALUES[i].size(); j++)
			{
				const char* value = VOV_WEAPON_VALUES[i].at(j).c_str();
				if (GAMEPLAY::GET_HASH_KEY((char*)value) == current)
				{
					weaponSelectionIndex = i;
					break;
				}
			}
		}
	}

	return draw_generic_menu<int>(menuItems, &weaponSelectionIndex, "Weapon Categories", onconfirm_weaponlist_menu, NULL, NULL);
}

bool do_give_weapon(std::string modelName)
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists){ WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY((char *) modelName.c_str()), 1000, 0); return true; }
	else{ return false; }
}

bool onconfirm_weapon_menu(MenuItem<int> choice)
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	switch (activeLineIndexWeapon)
	{
	case 0:
		for (int i = 0; i < sizeof(VOV_WEAPON_VALUES) / sizeof(VOV_WEAPON_VALUES[0]); i++)
		{
			for (int j = 0; j < VOV_WEAPON_VALUES[i].size(); j++)
			{
				char *weaponName = (char*) VOV_WEAPON_VALUES[i].at(j).c_str();
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY(weaponName), 1000, 0);
			}
		}

		//parachute
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), PARACHUTE_ID, 1, 0);

		set_status_text("All weapons added");
		break;
	// switchable features
	case 1:
		process_weaponlist_menu();
		break;
	default:
		break;
	}
	return false;
}

bool process_weapon_menu()
{
	int i = 0;

	std::string caption = "Weapon Options";
	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "Give All Weapons";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);
	
	item = new MenuItem<int>();
	item->caption = "Individual Weapons";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);
	
	SelectFromListMenuItem *listItem = new SelectFromListMenuItem(WEAP_DMG_CAPTIONS, onchange_weap_dmg_modifier);
	listItem->wrap = false;
	listItem->caption = "Weapon Damage Modifier";
	listItem->value = weapDmgModIndex;
	menuItems.push_back(listItem);
	
	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Infinite Ammo";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWeaponInfiniteAmmo;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Infinite Parachutes";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWeaponInfiniteParachutes;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Reload";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWeaponNoReload;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Fire Ammo";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWeaponFireAmmo;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Explosive Ammo";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWeaponExplosiveAmmo;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Explosive Melee";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWeaponExplosiveMelee;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Vehicle Rockets";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWeaponVehRockets;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);
/*
	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Give All Weapons", NULL, NULL, true },
		{ "Individual Weapons", NULL, NULL, false },
		{ "Infinite Ammo", &featureWeaponInfiniteAmmo, NULL },
		{ "Infinite Parachutes", &featureWeaponInfiniteParachutes, NULL },
		{ "No Reload", &featureWeaponNoReload, NULL },
		{ "Fire Ammo", &featureWeaponFireAmmo, NULL },
		{ "Explosive Ammo", &featureWeaponExplosiveAmmo, NULL },
		{ "Explosive Melee", &featureWeaponExplosiveMelee, NULL },
		{ "Vehicle Rockets", &featureWeaponVehRockets, NULL }
	};
*/
	return draw_generic_menu<int>(menuItems, &activeLineIndexWeapon, caption, onconfirm_weapon_menu, NULL, NULL);
}

void reset_weapon_globals()
{
	activeLineIndexWeapon = 0;

	weapDmgModIndex = 0;

	featureWeaponInfiniteAmmo =
		featureWeaponInfiniteParachutes =
		featureWeaponNoReload =
		featureWeaponFireAmmo =
		featureWeaponExplosiveAmmo =
		featureWeaponExplosiveMelee =
		featureWeaponVehRockets = false;
}

void update_weapon_features(BOOL bPlayerExists, Player player)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	// weapon damage modifier
	if (bPlayerExists) {
		// Don't need to set this per-frame if it's at the default
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, WEAP_DMG_FLOAT[weapDmgModIndex]);
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(player, WEAP_DMG_FLOAT[weapDmgModIndex]);
		PLAYER::SET_PLAYER_VEHICLE_DAMAGE_MODIFIER(player, WEAP_DMG_FLOAT[weapDmgModIndex]);
	}
	
	// weapon
	if (featureWeaponFireAmmo)
	{
		if (bPlayerExists)
		{
			GAMEPLAY::SET_FIRE_AMMO_THIS_FRAME(player);
		}
	}
	if (featureWeaponExplosiveAmmo)
	{
		if (bPlayerExists)
		{
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(player);
		}
	}
	if (featureWeaponExplosiveMelee)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(player);
	}

	// infinite ammo
	if (bPlayerExists && featureWeaponInfiniteAmmo)
	{
		for (int i = 0; i < sizeof(VOV_WEAPON_VALUES) / sizeof(VOV_WEAPON_VALUES[0]); i++)
		{
			for (int j = 0; j < VOV_WEAPON_VALUES[i].size(); j++)
			{
				char *weaponName = (char*)VOV_WEAPON_VALUES[i].at(j).c_str();
				Hash weapon = GAMEPLAY::GET_HASH_KEY(weaponName);

				if (WEAPON::IS_WEAPON_VALID(weapon) && WEAPON::HAS_PED_GOT_WEAPON(playerPed, weapon, 0))
				{
					int ammo;

					if (WEAPON::GET_MAX_AMMO(playerPed, weapon, &ammo))
					{
						WEAPON::SET_PED_AMMO(playerPed, weapon, ammo);
					}
				}
			}
		}
	}

	// infinite parachutes
	if (bPlayerExists && featureWeaponInfiniteParachutes)
	{
		int pState = PED::GET_PED_PARACHUTE_STATE(playerPed);
		//unarmed or falling - don't try and give p/chute to player already using one, crashes game
		if (pState == -1 || pState == 3)
		{
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, 0xFBAB5776, 1, 0);
		}
	}

	// weapon no reload
	if (bPlayerExists)
	{
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(playerPed, featureWeaponNoReload);
	}
}

void update_vehicle_guns()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehRockets) return;

	bool bSelect = IsKeyDown(KeyConfig::KEY_VEH_ROCKETS) || IsControllerButtonDown(KeyConfig::KEY_VEH_ROCKETS);

	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
			{
				WAIT(0);
			}
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();
	}
}

void save_player_weapons()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	int index = 0;
	for (int i = 0; i < sizeof(VOV_WEAPON_VALUES) / sizeof(VOV_WEAPON_VALUES[0]); i++)
	{
		for (int j = 0; j < VOV_WEAPON_VALUES[i].size(); j++)
		{
			std::string weaponNameStr = VOV_WEAPON_VALUES[i].at(j);
			char *weaponName = (char*)weaponNameStr.c_str();
			if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, GAMEPLAY::GET_HASH_KEY(weaponName), 0))
			{
				Weapon w = GAMEPLAY::GET_HASH_KEY(weaponName);
				saved_weapon_model[index] = w;
				saved_ammo[index] = WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, w);
				WEAPON::GET_AMMO_IN_CLIP(playerPed, w, &saved_clip_ammo[index]);

				for (int k = 0; k < WEAPONTYPES_TINT.size(); k++)
				{
					if (weaponNameStr.compare(WEAPONTYPES_TINT.at(k)) == 0)
					{
						saved_weapon_tints[index] = WEAPON::GET_PED_WEAPON_TINT_INDEX(playerPed, w);
						break;
					}
				}

				for (int k = 0; k < WEAPONTYPES_MOD.size(); k++)
				{
					if (weaponNameStr.compare(WEAPONTYPES_MOD.at(k)) == 0)
					{
						for (int m = 0; m < VOV_WEAPONMOD_VALUES[k].size(); m++)
						{
							std::string componentName = VOV_WEAPONMOD_VALUES[k].at(m);
							DWORD componentHash = GAMEPLAY::GET_HASH_KEY((char *)componentName.c_str());

							bool modEquipped = WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, w, componentHash) ? true : false;
							saved_weapon_mods[index][m] = modEquipped;
						}
						break;
					}
				}
			}

			index++;
		}
	}

	saved_parachute = (WEAPON::HAS_PED_GOT_WEAPON(playerPed, PARACHUTE_ID, 0)) ? true : false;
	saved_armour = PED::GET_PED_ARMOUR(playerPed);
}

void restore_player_weapons()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int index = 0;
	for (int i = 0; i < sizeof(VOV_WEAPON_VALUES) / sizeof(VOV_WEAPON_VALUES[0]); i++)
	{
		for (int j = 0; j < VOV_WEAPON_VALUES[i].size(); j++)
		{
			char *weaponName = (char*)VOV_WEAPON_VALUES[i].at(j).c_str();
			Weapon w = GAMEPLAY::GET_HASH_KEY(weaponName);
			WEAPON::GIVE_WEAPON_TO_PED(playerPed, saved_weapon_model[index], 1000, 0, 0);
			WEAPON::SET_PED_AMMO(playerPed, saved_weapon_model[i], saved_ammo[index]);
			WEAPON::SET_AMMO_IN_CLIP(playerPed, saved_weapon_model[i], saved_clip_ammo[index]);

			for (int k = 0; k < WEAPONTYPES_TINT.size(); k++)
			{
				if (VOV_WEAPON_VALUES[i].at(j).compare(WEAPONTYPES_TINT.at(k)) == 0)
				{
					WEAPON::SET_PED_WEAPON_TINT_INDEX(playerPed, w, saved_weapon_tints[index]);
					break;
				}
			}

			for (int k = 0; k < WEAPONTYPES_MOD.size(); k++)
			{
				if (VOV_WEAPON_VALUES[i].at(j).compare(WEAPONTYPES_MOD.at(k)) == 0)
				{
					for (int m = 0; m < VOV_WEAPONMOD_VALUES[k].size(); m++)
					{
						std::string componentName = VOV_WEAPONMOD_VALUES[k].at(m);
						DWORD componentHash = GAMEPLAY::GET_HASH_KEY((char *)componentName.c_str());

						if (saved_weapon_mods[index][m])
						{
							WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, w, componentHash);
						}
						else
						{
							WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, w, componentHash);
						}
					}
					break;
				}
			}

			index++;
		}
	}

	if (saved_parachute)
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, PARACHUTE_ID, 1, 0);
	}

	PED::SET_PED_ARMOUR(playerPed, saved_armour);
}

bool is_weapon_equipped(std::vector<int> extras)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::string weaponValue = VOV_WEAPON_VALUES[extras.at(0)].at(extras.at(1));
	char *weaponChar = (char*) weaponValue.c_str();
	return (WEAPON::HAS_PED_GOT_WEAPON(playerPed, GAMEPLAY::GET_HASH_KEY(weaponChar), 0) ? true : false);
}

void set_weapon_equipped(bool equipped, std::vector<int> extras)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::string weaponValue = VOV_WEAPON_VALUES[extras.at(0)].at(extras.at(1));
	char *weaponChar = (char*) weaponValue.c_str();
	int weapHash = GAMEPLAY::GET_HASH_KEY(weaponChar);
	if (equipped)
	{
		WEAPON::GIVE_WEAPON_TO_PED(playerPed, weapHash, 1000, 0, 0);

		//fill the clip and one spare
		int maxClipAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, weapHash, false);
		WEAPON::SET_PED_AMMO(playerPed, weapHash, maxClipAmmo);
		WEAPON::SET_AMMO_IN_CLIP(playerPed, weapHash, maxClipAmmo);
	}
	else
	{
		WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, GAMEPLAY::GET_HASH_KEY(weaponChar));
	}

	redrawWeaponMenuAfterEquipChange = true;
}

bool is_weaponmod_equipped(std::vector<int> extras)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::string weaponValue = VOV_WEAPON_VALUES[extras.at(0)].at(extras.at(1));

	char *weaponChar = (char*)weaponValue.c_str();
	int weapHash = GAMEPLAY::GET_HASH_KEY(weaponChar);

	std::string componentName = VOV_WEAPONMOD_VALUES[extras.at(2)].at(extras.at(3));
	DWORD componentHash = GAMEPLAY::GET_HASH_KEY((char *)componentName.c_str());

	return WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, weapHash, componentHash) ? true : false;
}

void set_weaponmod_equipped(bool equipped, std::vector<int> extras)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::string weaponValue = VOV_WEAPON_VALUES[extras.at(0)].at(extras.at(1));
	char *weaponChar = (char*)weaponValue.c_str();
	int weapHash = GAMEPLAY::GET_HASH_KEY(weaponChar);

	std::string componentName = VOV_WEAPONMOD_VALUES[extras.at(2)].at(extras.at(3));
	DWORD componentHash = GAMEPLAY::GET_HASH_KEY((char *)componentName.c_str());

	if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, weapHash, componentHash))
	{
		WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, weapHash, componentHash);
	}
	else
	{
		WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, weapHash, componentHash);
	}
}

void give_weapon_clip(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::string weaponValue = VOV_WEAPON_VALUES[lastSelectedWeaponCategory].at(lastSelectedWeapon);
	char *weaponChar = (char*)weaponValue.c_str();
	int weapHash = GAMEPLAY::GET_HASH_KEY(weaponChar);

	int curAmmo = WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weapHash);
	int curClipAmmo = 0;
	WEAPON::GET_AMMO_IN_CLIP(playerPed, weapHash, &curClipAmmo);
	int maxClipAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, weapHash, false);

	if (curClipAmmo < maxClipAmmo)
	{
		set_status_text("Clip filled");
		WEAPON::SET_AMMO_IN_CLIP(playerPed, weapHash, maxClipAmmo);
	}
	else
	{
		set_status_text("Extra clip added");
		WEAPON::SET_PED_AMMO(playerPed, weapHash, curAmmo+maxClipAmmo);
	}
}

void fill_weapon_ammo(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::string weaponValue = VOV_WEAPON_VALUES[lastSelectedWeaponCategory].at(lastSelectedWeapon);
	char *weaponChar = (char*)weaponValue.c_str();
	int weapHash = GAMEPLAY::GET_HASH_KEY(weaponChar);

	int maxAmmo = 0;
	WEAPON::GET_MAX_AMMO(playerPed, weapHash, &maxAmmo);
	int maxClipAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, weapHash, false);

	WEAPON::SET_AMMO_IN_CLIP(playerPed, weapHash, maxClipAmmo);
	WEAPON::SET_PED_AMMO(playerPed, weapHash, maxAmmo);

	set_status_text("Ammo filled");
}

void onhighlight_weapon_mod_menu_tint(MenuItem<int> choice)
{
	onconfirm_weapon_mod_menu_tint(choice);
}

bool onconfirm_weapon_mod_menu_tint(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::string weaponName = VOV_WEAPON_VALUES[lastSelectedWeaponCategory].at(lastSelectedWeapon);
	int weapHash = GAMEPLAY::GET_HASH_KEY((char*)weaponName.c_str());
	
	WEAPON::SET_PED_WEAPON_TINT_INDEX(playerPed, weapHash, choice.value);
	
	return true;
}

void onconfirm_open_tint_menu(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < VALUES_TINT.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = CAPTIONS_TINT[i];
		item->value = VALUES_TINT[i];
		menuItems.push_back(item);
	}

	std::string weaponValue = VOV_WEAPON_VALUES[lastSelectedWeaponCategory].at(lastSelectedWeapon);
	char *weaponChar = (char*)weaponValue.c_str();
	int weapHash = GAMEPLAY::GET_HASH_KEY(weaponChar);

	int tintSelection = 0;
	for (int i = 0; i < WEAPONTYPES_TINT.size(); i++)
	{
		if (WEAPON::GET_PED_WEAPON_TINT_INDEX(playerPed, weapHash) == VALUES_TINT[i])
		{
			tintSelection = i;
			break;
		}
	}

	draw_generic_menu<int>(menuItems, &tintSelection, "Select Tint Color", onconfirm_weapon_mod_menu_tint, onhighlight_weapon_mod_menu_tint, NULL);
}

void add_weapon_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeaponExplosiveAmmo", &featureWeaponExplosiveAmmo });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeaponExplosiveMelee", &featureWeaponExplosiveMelee });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeaponFireAmmo", &featureWeaponFireAmmo });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeaponInfiniteAmmo", &featureWeaponInfiniteAmmo });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeaponInfiniteParachutes", &featureWeaponInfiniteParachutes });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeaponNoReload", &featureWeaponNoReload });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeaponVehRockets", &featureWeaponVehRockets });
}

void onchange_weap_dmg_modifier(int value, SelectFromListMenuItem* source)
{
	weapDmgModIndex = value;
}

void add_weapons_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "weapDmgModIndex", std::to_string(weapDmgModIndex) });
}

void handle_generic_settings_weapons(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("weapDmgModIndex") == 0)
		{
			weapDmgModIndex = stoi(setting.value);
		}
	}
}
