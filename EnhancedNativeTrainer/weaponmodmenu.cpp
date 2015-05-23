/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "weapons.h"
#include "menu_functions.h"


//Main
const std::vector<std::string> MENU_WEAPON_MAIN{ "Change Weapon Tint", "Weapon Mods" };


//Weapon tints
const std::vector<std::string> CAPTIONS_TINT{ "Normal", "Green", "Gold", "Pink", "Army", "LSPD", "Orange", "Platinum" };
const std::vector<std::string> VALUES_TINT{ "0", "1", "2", "3", "4", "5", "6", "7" };

//Pistols
const std::vector<std::string> CAPTIONS_ATTACH_PISTOL{ "Extended Magazine", "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_PISTOL{ "COMPONENT_PISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP_02", "COMPONENT_AT_PI_FLSH" };

const std::vector<std::string> CAPTIONS_ATTACH_COMBATPISTOL{ "Extended Magazine", "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_COMBATPISTOL{ "COMPONENT_COMBATPISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP", "COMPONENT_AT_PI_FLSH" };

const std::vector<std::string> CAPTIONS_ATTACH_APPISTOL{ "Extended Magazine", "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_APPISTOL{ "COMPONENT_APPISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP", "COMPONENT_AT_PI_FLSH" };

const std::vector<std::string> CAPTIONS_ATTACH_PISTOL50{ "Extended Magazine", "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_PISTOL50{ "COMPONENT_PISTOL50_CLIP_02", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_PI_FLSH" };

//SMGs
const std::vector<std::string> CAPTIONS_ATTACH_MICROSMG{ "Extended Magazine", "Scope", "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_MICROSMG{ "COMPONENT_MICROSMG_CLIP_02", "COMPONENT_AT_SCOPE_MACRO", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_PI_FLSH" };

const std::vector<std::string> CAPTIONS_ATTACH_SMG{ "Extended Magazine", "Scope", "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_SMG{ "COMPONENT_SMG_CLIP_02", "COMPONENT_AT_SCOPE_MACRO_02", "COMPONENT_AT_PI_SUPP", "COMPONENT_AT_AR_FLSH" };

const std::vector<std::string> CAPTIONS_ATTACH_ASSAULTSMG{ "Extended Magazine", "Scope", "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_ASSAULTSMG{ "COMPONENT_ASSAULTSMG_CLIP_02", "COMPONENT_AT_SCOPE_MACRO", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_FLSH" };

//Rifles
const std::vector<std::string> CAPTIONS_ATTACH_ASSAULTRIFLE{ "Extended Magazine", "Scope", "Suppressor", "Grip", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_ASSAULTRIFLE{ "COMPONENT_ASSAULTRIFLE_CLIP_02", "COMPONENT_AT_SCOPE_MACRO", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_AFGRIP", "COMPONENT_AT_AR_FLSH" };

const std::vector<std::string> CAPTIONS_ATTACH_CARBINERIFLE{ "Extended Magazine", "Scope", "Suppressor", "Grip", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_CARBINERIFLE{ "COMPONENT_CARBINERIFLE_CLIP_02", "COMPONENT_AT_SCOPE_MEDIUM", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_AFGRIP", "COMPONENT_AT_AR_FLSH" };

const std::vector<std::string> CAPTIONS_ATTACH_ADVANCEDRIFLE{ "Extended Magazine", "Scope", "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_ADVANCEDRIFLE{ "COMPONENT_ADVANCEDRIFLE_CLIP_02", "COMPONENT_AT_SCOPE_SMALL", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_FLSH" };

//Machine Guns
const std::vector<std::string> CAPTIONS_ATTACH_MG{ "Extended Magazine", "Scope" };
const std::vector<std::string> VALUES_ATTACH_MG{ "COMPONENT_MG_CLIP_02", "COMPONENT_AT_SCOPE_SMALL_02" };

const std::vector<std::string> CAPTIONS_ATTACH_COMBATMG{ "Extended Magazine", "Scope", "Grip" };
const std::vector<std::string> VALUES_ATTACH_COMBATMG{ "COMPONENT_COMBATMG_CLIP_02", "COMPONENT_AT_SCOPE_MEDIUM", "COMPONENT_AT_AR_AFGRIP" };

//Shotguns
const std::vector<std::string> CAPTIONS_ATTACH_PUMPSHOTGUN{ "Suppressor", "Flashlight" };
const std::vector<std::string> VALUES_ATTACH_PUMPSHOTGUN{ "COMPONENT_AT_SR_SUPP", "COMPONENT_AT_AR_FLSH" };

const std::vector<std::string> CAPTIONS_ATTACH_ASSAULTSHOTGUN{ "Extended Magazine", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_ASSAULTSHOTGUN{ "COMPONENT_ASSAULTSHOTGUN_CLIP_02", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> CAPTIONS_ATTACH_BULLPUPSHOTGUN{ "Flashlight", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_BULLPUPSHOTGUN{ "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_AFGRIP" };

//Sniper Rifles
const std::vector<std::string> CAPTIONS_ATTACH_SNIPERRIFLE{ "Advanced Scope", "Suppressor" };
const std::vector<std::string> VALUES_ATTACH_SNIPERRIFLE{ "COMPONENT_AT_SCOPE_MAX", "COMPONENT_AT_AR_SUPP_02" };

const std::vector<std::string> CAPTIONS_ATTACH_HEAVYSNIPER{ "Normal Scope" };
const std::vector<std::string> VALUES_ATTACH_HEAVYSNIPER{ "COMPONENT_AT_SCOPE_LARGE" };

//Heavy Weapons
const std::vector<std::string> CAPTIONS_ATTACH_GRENADELAUNCHER{ "Scope", "Flashlight", "Grip" };
const std::vector<std::string> VALUES_ATTACH_GRENADELAUNCHER{ "COMPONENT_AT_SCOPE_SMALL", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> WEAPONTYPES{ "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG", "WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER" };

const std::vector<std::string> VOV_WEAPON_CAPTIONS[] = { CAPTIONS_ATTACH_PISTOL, CAPTIONS_ATTACH_COMBATPISTOL, CAPTIONS_ATTACH_APPISTOL, CAPTIONS_ATTACH_PISTOL50, CAPTIONS_ATTACH_MICROSMG, CAPTIONS_ATTACH_SMG, CAPTIONS_ATTACH_ASSAULTSMG, CAPTIONS_ATTACH_ASSAULTRIFLE, CAPTIONS_ATTACH_CARBINERIFLE, CAPTIONS_ATTACH_ADVANCEDRIFLE, CAPTIONS_ATTACH_MG, CAPTIONS_ATTACH_COMBATMG, CAPTIONS_ATTACH_PUMPSHOTGUN, CAPTIONS_ATTACH_ASSAULTSHOTGUN, CAPTIONS_ATTACH_BULLPUPSHOTGUN, CAPTIONS_ATTACH_SNIPERRIFLE, CAPTIONS_ATTACH_HEAVYSNIPER, CAPTIONS_ATTACH_GRENADELAUNCHER };
const std::vector<std::string> VOV_WEAPON_VALUES[] = { VALUES_ATTACH_PISTOL, VALUES_ATTACH_COMBATPISTOL, VALUES_ATTACH_APPISTOL, VALUES_ATTACH_PISTOL50, VALUES_ATTACH_MICROSMG, VALUES_ATTACH_SMG, VALUES_ATTACH_ASSAULTSMG, VALUES_ATTACH_ASSAULTRIFLE, VALUES_ATTACH_CARBINERIFLE, VALUES_ATTACH_ADVANCEDRIFLE, VALUES_ATTACH_MG, VALUES_ATTACH_COMBATMG, VALUES_ATTACH_PUMPSHOTGUN, VALUES_ATTACH_ASSAULTSHOTGUN, VALUES_ATTACH_BULLPUPSHOTGUN, VALUES_ATTACH_SNIPERRIFLE, VALUES_ATTACH_HEAVYSNIPER, VALUES_ATTACH_GRENADELAUNCHER };


bool onconfirm_weapon_mod_menu(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Weapon wep = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);
	int wepindex = 0;

	switch (choice.currentMenuIndex)
	{
	case 0:
		if (WEAPON::IS_PED_ARMED(playerPed, 6) && WEAPON::GET_WEAPON_TINT_COUNT(wep) > 1) //This always returns all possible tints even if weapon is not tintable. Need to find a better way to check.
		{
			process_weapon_mod_menu_tint();
		}
		else
		{
			set_status_text("You do not have a tintable weapon selected");
		}
		break;

	case 1:
		if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL")) { wepindex = 0; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_COMBATPISTOL")){ wepindex = 1; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_APPISTOL")){ wepindex = 2; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL50")){ wepindex = 3; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_MICROSMG")){ wepindex = 4; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_SMG")){ wepindex = 5; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTSMG")){ wepindex = 6; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE")){ wepindex = 7; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_CARBINERIFLE")){ wepindex = 8; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_ADVANCEDRIFLE")){ wepindex = 9; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_MG")){ wepindex = 10; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_COMBATMG")){ wepindex = 11; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN")){ wepindex = 12; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTSHOTGUN")){ wepindex = 13; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_BULLPUPSHOTGUN")){ wepindex = 14; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE")){ wepindex = 15; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_HEAVYSNIPER")){ wepindex = 16; }
		else if (wep == GAMEPLAY::GET_HASH_KEY("WEAPON_GRENADELAUNCHER")){ wepindex = 17; }

		else
		{
			set_status_text("You do not have a moddable weapon selected");
			break;
		}

		process_weapon_mod_menu_attachments(wepindex);

		break;
	}
	return false;
}

bool process_weapon_mod_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	for (int i = 0; i < MENU_WEAPON_MAIN.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_WEAPON_MAIN[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Weapon Mod Menu", onconfirm_weapon_mod_menu, NULL, NULL);
}

bool process_weapon_mod_menu_tint()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Weapon wep = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VALUES_TINT.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = CAPTIONS_TINT[i];
		item->value = VALUES_TINT[i];
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, 0, "Select Color", onconfirm_weapon_mod_menu_tint, onhighlight_weapon_mod_menu_tint, NULL);
}

void onhighlight_weapon_mod_menu_tint(MenuItem<std::string> choice)
{
	onconfirm_weapon_mod_menu_tint(choice);
}

bool onconfirm_weapon_mod_menu_tint(MenuItem<std::string> choice)
{
	int whichtint = std::stoi(choice.value);
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Weapon wep = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);
	WEAPON::SET_PED_WEAPON_TINT_INDEX(playerPed, wep, whichtint);

	return true;
}

bool process_weapon_mod_menu_attachments(int wepindex)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Weapon wep = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_WEAPON_VALUES[wepindex].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_WEAPON_CAPTIONS[wepindex][i];
		item->value = VOV_WEAPON_VALUES[wepindex][i];
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, 0, "Select Attachments", onconfirm_weapon_mod_menu_attachments, NULL, NULL);
}

bool onconfirm_weapon_mod_menu_attachments(MenuItem<std::string> choice)
{
	DWORD component = GAMEPLAY::GET_HASH_KEY((char *)choice.value.c_str());

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Weapon wep = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, wep, component))
	{
		WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, wep, component);
		std::ostringstream ss;
		ss << choice.caption << " Removed";
		set_status_text(ss.str());
	}
	else
	{
		WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, wep, component);
		std::ostringstream ss;
		ss << choice.caption << " Added";
		set_status_text(ss.str());
	}

	return false;
}