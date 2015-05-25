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

Weapon wepcheck;


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

//DLC Weapons
const std::vector<std::string> CAPTIONS_ATTACH_BULLPUPRIFLE{ "Extended Magazine", "Flashlight", "Scope", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_BULLPUPRIFLE{ "COMPONENT_BULLPUPRIFLE_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_SCOPE_SMALL", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> CAPTIONS_ATTACH_GUSENBERG{ "Extended Magazine" };
const std::vector<std::string> VALUES_ATTACH_GUSENBERG{ "COMPONENT_GUSENBERG_CLIP_02" };

const std::vector<std::string> CAPTIONS_ATTACH_HEAVYPISTOL{ "Extended Magazine", "Flashlight", "Suppressor" };
const std::vector<std::string> VALUES_ATTACH_HEAVYPISTOL{ "COMPONENT_HEAVYPISTOL_CLIP_02", "COMPONENT_AT_PI_FLSH", "COMPONENT_AT_PI_SUPP" };

const std::vector<std::string> CAPTIONS_ATTACH_HEAVYSHOTGUN{ "Extended Magazine", "Flashlight", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_HEAVYSHOTGUN{ "COMPONENT_HEAVYSHOTGUN_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> CAPTIONS_ATTACH_MARKSMANRIFLE{ "Extended Magazine", "Flashlight", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_MARKSMANRIFLE{ "COMPONENT_MARKSMANRIFLE_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> CAPTIONS_ATTACH_SNSPISTOL{ "Extended Magazine" };
const std::vector<std::string> VALUES_ATTACH_SNSPISTOL{ "COMPONENT_SNSPISTOL_CLIP_02" };

const std::vector<std::string> CAPTIONS_ATTACH_SPECIALCARBINE{ "Extended Magazine", "Flashlight", "Scope", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_SPECIALCARBINE{ "COMPONENT_SPECIALCARBINE_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_SCOPE_MEDIUM", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> CAPTIONS_ATTACH_VINTAGEPISTOL{ "Extended Magazine", "Suppressor" };
const std::vector<std::string> VALUES_ATTACH_VINTAGEPISTOL{ "COMPONENT_VINTAGEPISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP" };



const std::vector<std::string> VOV_WEAPON_CAPTIONS[] = { CAPTIONS_ATTACH_PISTOL, CAPTIONS_ATTACH_COMBATPISTOL, CAPTIONS_ATTACH_APPISTOL, CAPTIONS_ATTACH_PISTOL50, CAPTIONS_ATTACH_MICROSMG, CAPTIONS_ATTACH_SMG, CAPTIONS_ATTACH_ASSAULTSMG, CAPTIONS_ATTACH_ASSAULTRIFLE, CAPTIONS_ATTACH_CARBINERIFLE, CAPTIONS_ATTACH_ADVANCEDRIFLE, CAPTIONS_ATTACH_MG, CAPTIONS_ATTACH_COMBATMG, CAPTIONS_ATTACH_PUMPSHOTGUN, CAPTIONS_ATTACH_ASSAULTSHOTGUN, CAPTIONS_ATTACH_BULLPUPSHOTGUN, CAPTIONS_ATTACH_SNIPERRIFLE, CAPTIONS_ATTACH_HEAVYSNIPER, CAPTIONS_ATTACH_GRENADELAUNCHER, CAPTIONS_ATTACH_BULLPUPRIFLE, CAPTIONS_ATTACH_GUSENBERG, CAPTIONS_ATTACH_HEAVYPISTOL, CAPTIONS_ATTACH_HEAVYSHOTGUN, CAPTIONS_ATTACH_MARKSMANRIFLE, CAPTIONS_ATTACH_SNSPISTOL, CAPTIONS_ATTACH_SPECIALCARBINE, CAPTIONS_ATTACH_VINTAGEPISTOL };
const std::vector<std::string> VOV_WEAPON_VALUES[] = { VALUES_ATTACH_PISTOL, VALUES_ATTACH_COMBATPISTOL, VALUES_ATTACH_APPISTOL, VALUES_ATTACH_PISTOL50, VALUES_ATTACH_MICROSMG, VALUES_ATTACH_SMG, VALUES_ATTACH_ASSAULTSMG, VALUES_ATTACH_ASSAULTRIFLE, VALUES_ATTACH_CARBINERIFLE, VALUES_ATTACH_ADVANCEDRIFLE, VALUES_ATTACH_MG, VALUES_ATTACH_COMBATMG, VALUES_ATTACH_PUMPSHOTGUN, VALUES_ATTACH_ASSAULTSHOTGUN, VALUES_ATTACH_BULLPUPSHOTGUN, VALUES_ATTACH_SNIPERRIFLE, VALUES_ATTACH_HEAVYSNIPER, VALUES_ATTACH_GRENADELAUNCHER, VALUES_ATTACH_BULLPUPRIFLE, VALUES_ATTACH_GUSENBERG, VALUES_ATTACH_HEAVYPISTOL, VALUES_ATTACH_HEAVYSHOTGUN, VALUES_ATTACH_MARKSMANRIFLE, VALUES_ATTACH_SNSPISTOL, VALUES_ATTACH_SPECIALCARBINE, VALUES_ATTACH_VINTAGEPISTOL };


//Tintable weapons list
const std::vector<std::string> WEAPONTYPES_TINT{ "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_SNSPISTOL", "WEAPON_HEAVYPISTOL", "WEAPON_VINTAGEPISTOL", "WEAPON_STUNGUN", "WEAPON_FLAREGUN", "WEAPON_MICROSMG", "WEAPON_SMG", "WEAPON_ASSAULTSMG", "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_GUSENBERG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_SPECIALCARBINE", "WEAPON_BULLPUPRIFLE", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_BULLPUPSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_MUSKET", "WEAPON_HEAVYSHOTGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_MARKSMANRIFLE", "WEAPON_GRENADELAUNCHER", "WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_FIREWORK", "WEAPON_RAILGUN", "WEAPON_HOMINGLAUNCHER" };

//moddable weapons list
const std::vector<std::string> WEAPONTYPES_MOD{ "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG", "WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_BULLPUPRIFLE", "WEAPON_GUSENBERG", "WEAPON_HEAVYPISTOL", "WEAPON_HEAVYSHOTGUN", "WEAPON_MARKSMANRIFLE", "WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_VINTAGEPISTOL" };


bool onconfirm_weapon_mod_menu(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Weapon wep = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);
	int wepindex = -1;

	switch (choice.currentMenuIndex)
	{
	case 0:
		wepindex = checkweapon_tintlist(wep);
		if (wepindex > -1)
		{
			process_weapon_mod_menu_tint();
		}
		else
		{
			set_status_text("You do not have a tintable weapon selected");
		}
		break;

	case 1:
		wepindex = checkweapon_modlist(wep);
		if (wepindex > -1)
		{
			process_weapon_mod_menu_attachments(wepindex);
		}
		else
		{
			set_status_text("You do not have a moddable weapon selected");
		}
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
	int wepindex = checkweapon_tintlist(wep); //Check again if weapon is tintable

	if (wepindex > -1)
	{
		WEAPON::SET_PED_WEAPON_TINT_INDEX(playerPed, wep, whichtint);
	}
	else
	{
		set_status_text("Weapon was changed. You do not have a tintable weapon selected.");
	}

	return true;
}

bool process_weapon_mod_menu_attachments(int wepindex)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Weapon wep = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);
	::wepcheck = wep;

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

	if (wep == ::wepcheck) //check if player swapped weapons since menu was generated
	{
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
	}
	else
	{
		int wepindex = checkweapon_modlist(wep); //Check new weapon and reload menu or exit
		if (wepindex > -1)
		{
			set_status_text("Weapon changed. Refreshing Menu.");
			process_weapon_mod_menu_attachments(wepindex);
		}
		else
		{
			set_status_text("Weapon changed. You do not have a moddable weapon selected.");
		}
		return true;
	}
	return false;
}

int checkweapon_modlist(Weapon wep)
{
	int wepindex = -1;
	for (int i = 0; i < WEAPONTYPES_MOD.size(); i++)
	{
		DWORD weplist = GAMEPLAY::GET_HASH_KEY((char *)WEAPONTYPES_MOD[i].c_str());

		if (wep == weplist)
		{
			wepindex = i;
			break;
		}
	}
	return wepindex;
}

int checkweapon_tintlist(Weapon wep)
{
	int wepindex = -1;
	for (int i = 0; i < WEAPONTYPES_TINT.size(); i++)
	{
		DWORD weplist = GAMEPLAY::GET_HASH_KEY((char *)WEAPONTYPES_TINT[i].c_str());

		if (wep == weplist)
		{
			wepindex = i;
			break;
		}
	}
	return wepindex;
}