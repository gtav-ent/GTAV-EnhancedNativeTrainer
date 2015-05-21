/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "menu_functions.h"
#include "weapons.h"
#include "config_io.h"

const std::vector<std::string> MENU_WEAPON_CATEGORIES{ "Melee", "Handguns", "Sub-machine Guns", "Shotguns", "Assault Rifles", "Light Machine Guns", "Sniper Rifles", "Heavy Weapons", "Thrown Weapons" };
const std::vector<std::string> CAPTIONS_MELEE{ "Knife", "Nightstick", "Hammer", "Baseball Bat", "Golf Club", "Crowbar", "Bottle", "Antique Dagger", "Hatchet" };
const std::vector<std::string> VALUES_MELEE{ "WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR", "WEAPON_BOTTLE", "WEAPON_DAGGER", "WEAPON_HATCHET" };
const std::vector<std::string> CAPTIONS_HANDGUN{ "Pistol", "Combat Pistol", "AP Pistol", "Pistol .50", "SNS Pistol", "Heavy Pistol", "Vintage Pistol", "Stun Gun", "Flare Gun" };
const std::vector<std::string> VALUES_HANDGUN{ "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_SNSPISTOL", "WEAPON_HEAVYPISTOL", "WEAPON_VINTAGEPISTOL", "WEAPON_STUNGUN", "WEAPON_FLAREGUN" };
const std::vector<std::string> CAPTIONS_SUBMACHINE{ "Micro SMG", "SMG", "Assault SMG", "MG", "Combat MG", "Gusenberg Sweeper" };
const std::vector<std::string> VALUES_SUBMACHINE{ "WEAPON_MICROSMG", "WEAPON_SMG", "WEAPON_ASSAULTSMG", "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_GUSENBERG" };
const std::vector<std::string> CAPTIONS_ASSAULT{ "Assault Rifle", "Carbine Rifle", "Advanced Rifle", "Special Carbine", "Bullpup Rifle" };
const std::vector<std::string> VALUES_ASSAULT{ "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_SPECIALCARBINE", "WEAPON_BULLPUPRIFLE" };
const std::vector<std::string> CAPTIONS_SHOTGUN{ "Pump Shotgun", "Sawed-Off Shotgun", "Bullpup Shotgun", "Assault Shotgun", "Musket", "Heavy Shotgun" };
const std::vector<std::string> VALUES_SHOTGUN{ "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_BULLPUPSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_MUSKET", "WEAPON_HEAVYSHOTGUN" };
const std::vector<std::string> CAPTIONS_SNIPER{ "Sniper Rifle", "Heavy Sniper", "Marksman Rifle" };
const std::vector<std::string> VALUES_SNIPER{ "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_MARKSMANRIFLE" };
const std::vector<std::string> CAPTIONS_HEAVY{ "Grenade Launcher", "RPG", "Minigun", "Fireworks Launcher", "Railgun", "Homing Launcher" };
const std::vector<std::string> VALUES_HEAVY{ "WEAPON_GRENADELAUNCHER", "WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_FIREWORK", "WEAPON_RAILGUN", "WEAPON_HOMINGLAUNCHER" };
const std::vector<std::string> CAPTIONS_THROWN{ "Grenade", "Sticky Bomb", "Proximity Mine", "Teargas", "Molotov", "Fire Extinguisher", "Jerry Can", "Snowball", "Flare" };
const std::vector<std::string> VALUES_THROWN{ "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_PROXMINE", "WEAPON_SMOKEGRENADE", "WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN", "WEAPON_SNOWBALL", "WEAPON_FLARE" };

const std::vector<std::string> VOV_WEAPON_VALUES[] = { VALUES_MELEE, VALUES_HANDGUN, VALUES_SUBMACHINE, VALUES_ASSAULT, VALUES_SHOTGUN, VALUES_SNIPER, VALUES_HEAVY, VALUES_THROWN };

int activeLineIndexWeapon = 0;

bool featureWeaponInfiniteAmmo = false;
bool featureWeaponInfiniteParachutes = false;
bool featureWeaponNoReload = false;
bool featureWeaponFireAmmo = false;
bool featureWeaponExplosiveAmmo = false;
bool featureWeaponExplosiveMelee = false;
bool featureWeaponVehRockets = false;

DWORD featureWeaponVehShootLastTime = 0;

bool onconfirm_give_weapon_selection(std::string choice, std::string name)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	if (bPlayerExists)
	{
		do_give_weapon(choice);
		std::stringstream ss;
		ss << name << " recieved.";
		set_status_text(ss.str());
		return true;
	}
	return false;
}

bool onconfirm_give_melee(MenuItem<int> choice)
{
	/*std::vector<MenuItem<std::string>*> menuItems;
	MenuItem<std::string> *item = new MenuItem<std::string>();
	item->caption = CAPTIONS_MELEE[choice.value];
	item->value = VALUES_MELEE[choice.value];
	menuItems.push_back(item);*/
	std::string w = VALUES_MELEE[choice.value];
	std::string n = CAPTIONS_MELEE[choice.value];

	return onconfirm_give_weapon_selection(w,n);
}

bool process_give_melee()
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < CAPTIONS_MELEE.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = CAPTIONS_MELEE[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Melee Weapons", onconfirm_give_melee, NULL, NULL);
}

bool onconfirm_weaponlist_menu(MenuItem<int> choice)
{
	switch (choice.currentMenuIndex)
	{
	case 0:
		process_give_melee();
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	}
	return false;
}

bool process_weaponlist_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < MENU_WEAPON_CATEGORIES.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_WEAPON_CATEGORIES[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Weapon Categories", onconfirm_weaponlist_menu, NULL, NULL);
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
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);

		set_status_text("All weapons added");
		break;
		// switchable features
		/*case 1:
		process_weaponlist_menu();
		break;*/
	default:
		break;
	}
	return false;
}

void process_weapon_menu()
{
	const int lineCount = 8;

	std::string caption = "Weapon Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Give All Weapons", NULL, NULL, true },
		//{"Add Weapon", NULL, NULL, true },
		{ "Infinite Ammo", &featureWeaponInfiniteAmmo, NULL },
		{ "Infinite Parachutes", &featureWeaponInfiniteParachutes, NULL },
		{ "No Reload", &featureWeaponNoReload, NULL },
		{ "Fire Ammo", &featureWeaponFireAmmo, NULL },
		{ "Explosive Ammo", &featureWeaponExplosiveAmmo, NULL },
		{ "Explosive Melee", &featureWeaponExplosiveMelee, NULL },
		{ "Vehicle Rockets", &featureWeaponVehRockets, NULL }
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexWeapon, caption, onconfirm_weapon_menu);
}

void reset_weapon_globals()
{
	activeLineIndexWeapon = 0;

	featureWeaponInfiniteAmmo =
		featureWeaponInfiniteParachutes =
		featureWeaponNoReload =
		featureWeaponFireAmmo =
		featureWeaponExplosiveAmmo =
		featureWeaponExplosiveMelee =
		featureWeaponVehRockets = false;
}

void update_weapon_features(BOOL bPlayerExists, Ped player)
{
	// weapon
	if (featureWeaponFireAmmo)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_FIRE_AMMO_THIS_FRAME(player);
	}
	if (featureWeaponExplosiveAmmo)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(player);
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

				if (WEAPON::IS_WEAPON_VALID(weapon) && WEAPON::HAS_PED_GOT_WEAPON(player, weapon, 0))
				{
					int ammo;

					if (WEAPON::GET_MAX_AMMO(player, weapon, &ammo))
					{
						WEAPON::SET_PED_AMMO(player, weapon, ammo);
					}
				}
			}
		}
	}

	// infinite parachutes
	if (bPlayerExists && featureWeaponInfiniteParachutes)
	{
		//parachute
		//WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);

		int pState = PED::GET_PED_PARACHUTE_STATE(PLAYER::PLAYER_PED_ID());
		if (pState == -1 || pState == 3)
		{
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
		}
	}

	// weapon no reload
	if (bPlayerExists && featureWeaponNoReload)
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(player, &cur, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(cur))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(player, cur, &maxAmmo))
				{
					WEAPON::SET_PED_AMMO(player, cur, maxAmmo);

					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(player, cur, 1);
					if (maxAmmo > 0)
						WEAPON::SET_AMMO_IN_CLIP(player, cur, maxAmmo);
				}
			}
		}
	}
}

void update_vehicle_guns()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehRockets) return;

	bool bSelect = IsKeyDown(get_config()->get_key_config()->key_veh_rockets);
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
				WAIT(0);
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