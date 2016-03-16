/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai SMith and fellow contributors 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include "..\..\inc\main.h"

#include "..\ui_support\menu_functions.h"

#include "..\storage\database.h"

//I added the luxury tints to the weapons that got them, but they don't work?

const std::vector<std::string> MENU_WEAPON_CATEGORIES{ "Melee", "Handguns", "Submachine Guns", "Assault Rifles", "Shotguns", "Sniper Rifles", "Heavy Weapons", "Thrown Weapons" };

const std::vector<std::string> CAPTIONS_MELEE{ "Knife", "Nightstick", "Hammer", "Baseball Bat", "Golf Club", "Crowbar", "Bottle", "Antique Dagger", "Hatchet", "Knuckle Duster", "Machete", "Flashlight", "Switchblade" };
const std::vector<std::string> VALUES_MELEE{ "WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR", "WEAPON_BOTTLE", "WEAPON_DAGGER", "WEAPON_HATCHET", "WEAPON_KNUCKLE", "WEAPON_MACHETE", "WEAPON_FLASHLIGHT", "WEAPON_SWITCHBLADE" }; //11
const std::vector<std::string> CAPTIONS_HANDGUN{ "Pistol", "Combat Pistol", "AP Pistol", "Pistol .50", "SNS Pistol", "Heavy Pistol", "Vintage Pistol", "Stun Gun", "Flare Gun", "Marksman Pistol", "Revolver" };
const std::vector<std::string> VALUES_HANDGUN{ "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_SNSPISTOL", "WEAPON_HEAVYPISTOL", "WEAPON_VINTAGEPISTOL", "WEAPON_STUNGUN", "WEAPON_FLAREGUN", "WEAPON_MARKSMANPISTOL", "WEAPON_REVOLVER" }; //11
const std::vector<std::string> CAPTIONS_SUBMACHINE{ "Micro SMG", "SMG", "Assault SMG", "MG", "Combat MG", "Gusenberg Sweeper", "Combat PDW", "Machine Pistol" };
const std::vector<std::string> VALUES_SUBMACHINE{ "WEAPON_MICROSMG", "WEAPON_SMG", "WEAPON_ASSAULTSMG", "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_GUSENBERG", "WEAPON_COMBATPDW", "WEAPON_MACHINEPISTOL" }; //6
const std::vector<std::string> CAPTIONS_ASSAULT{ "Assault Rifle", "Carbine Rifle", "Advanced Rifle", "Special Carbine", "Bullpup Rifle", "Compact Rifle" };
const std::vector<std::string> VALUES_ASSAULT{ "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_SPECIALCARBINE", "WEAPON_BULLPUPRIFLE", "WEAPON_COMPACTRIFLE" }; //6
const std::vector<std::string> CAPTIONS_SHOTGUN{ "Pump Shotgun", "Sawed-Off Shotgun", "Bullpup Shotgun", "Assault Shotgun", "Musket", "Heavy Shotgun", "Double Barrel Shotgun" };
const std::vector<std::string> VALUES_SHOTGUN{ "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_BULLPUPSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_MUSKET", "WEAPON_HEAVYSHOTGUN", "WEAPON_DBSHOTGUN" }; //7
const std::vector<std::string> CAPTIONS_SNIPER{ "Sniper Rifle", "Heavy Sniper", "Marksman Rifle" };
const std::vector<std::string> VALUES_SNIPER{ "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_MARKSMANRIFLE" }; //3
const std::vector<std::string> CAPTIONS_HEAVY{ "Grenade Launcher", "RPG", "Minigun", "Fireworks Launcher", "Railgun", "Homing Launcher" };
const std::vector<std::string> VALUES_HEAVY{ "WEAPON_GRENADELAUNCHER", "WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_FIREWORK", "WEAPON_RAILGUN", "WEAPON_HOMINGLAUNCHER" }; //6
const std::vector<std::string> CAPTIONS_THROWN{ "Grenade", "Sticky Bomb", "Proximity Mine", "Teargas", "Molotov", "Fire Extinguisher", "Jerry Can", "Snowball", "Flare" };
const std::vector<std::string> VALUES_THROWN{ "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_PROXMINE", "WEAPON_SMOKEGRENADE", "WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN", "WEAPON_SNOWBALL", "WEAPON_FLARE" }; //9

const std::vector<std::string> VOV_WEAPON_CAPTIONS[] = { CAPTIONS_MELEE, CAPTIONS_HANDGUN, CAPTIONS_SUBMACHINE, CAPTIONS_ASSAULT, CAPTIONS_SHOTGUN, CAPTIONS_SNIPER, CAPTIONS_HEAVY, CAPTIONS_THROWN };
const std::vector<std::string> VOV_WEAPON_VALUES[] = { VALUES_MELEE, VALUES_HANDGUN, VALUES_SUBMACHINE, VALUES_ASSAULT, VALUES_SHOTGUN, VALUES_SNIPER, VALUES_HEAVY, VALUES_THROWN };


//Weapon tints
const std::vector<std::string> CAPTIONS_TINT{ "Normal", "Green", "Gold", "Pink", "Army", "LSPD", "Orange", "Platinum" };
const std::vector<int> VALUES_TINT{ 0, 1, 2, 3, 4, 5, 6, 7 };

//Pistols
const std::vector<std::string> CAPTIONS_ATTACH_PISTOL{ "Extended Magazine", "Suppressor", "Flashlight", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_PISTOL{ "COMPONENT_PISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP_02", "COMPONENT_AT_PI_FLSH", "COMPONENT_PISTOL_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_SNSPISTOL{ "Extended Magazine", "Etched Wood Grip Finish" };
const std::vector<std::string> VALUES_ATTACH_SNSPISTOL{ "COMPONENT_SNSPISTOL_CLIP_02", "COMPONENT_SNSPISTOL_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_HEAVYPISTOL{ "Extended Magazine", "Suppressor", "Flashlight", "Etched Wood Grip Finish" };
const std::vector<std::string> VALUES_ATTACH_HEAVYPISTOL{ "COMPONENT_HEAVYPISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP", "COMPONENT_AT_PI_FLSH", "COMPONENT_HEAVYPISTOL_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_COMBATPISTOL{ "Extended Magazine", "Suppressor", "Flashlight", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_COMBATPISTOL{ "COMPONENT_COMBATPISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP", "COMPONENT_AT_PI_FLSH", "COMPONENT_COMBATPISTOL_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_APPISTOL{ "Extended Magazine", "Suppressor", "Flashlight", "Gilded Gun Metal Finish" };
const std::vector<std::string> VALUES_ATTACH_APPISTOL{ "COMPONENT_APPISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP", "COMPONENT_AT_PI_FLSH", "COMPONENT_APPISTOL_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_PISTOL50{ "Extended Magazine", "Suppressor", "Flashlight", "Platinum Pearl Deluxe Finish" };
const std::vector<std::string> VALUES_ATTACH_PISTOL50{ "COMPONENT_PISTOL50_CLIP_02", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_PI_FLSH", "COMPONENT_PISTOL50_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_VINTAGEPISTOL{ "Extended Magazine", "Suppressor" };
const std::vector<std::string> VALUES_ATTACH_VINTAGEPISTOL{ "COMPONENT_VINTAGEPISTOL_CLIP_02", "COMPONENT_AT_PI_SUPP" };


//SMGs
const std::vector<std::string> CAPTIONS_ATTACH_MICROSMG{ "Extended Magazine", "Scope", "Suppressor", "Flashlight", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_MICROSMG{ "COMPONENT_MICROSMG_CLIP_02", "COMPONENT_AT_SCOPE_MACRO", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_PI_FLSH", "COMPONENT_MICROSMG_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_SMG{ "Extended Magazine", "Scope", "Suppressor", "Flashlight", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_SMG{ "COMPONENT_SMG_CLIP_02", "COMPONENT_AT_SCOPE_MACRO_02", "COMPONENT_AT_PI_SUPP", "COMPONENT_AT_AR_FLSH", "COMPONENT_SMG_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_ASSAULTSMG{ "Extended Magazine", "Scope", "Suppressor", "Flashlight", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_ASSAULTSMG{ "COMPONENT_ASSAULTSMG_CLIP_02", "COMPONENT_AT_SCOPE_MACRO", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_ASSULTSMG_VARMOD_LUXE" };

//Rifles
const std::vector<std::string> CAPTIONS_ATTACH_ASSAULTRIFLE{ "Extended Magazine", "Scope", "Suppressor", "Grip", "Flashlight", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_ASSAULTRIFLE{ "COMPONENT_ASSAULTRIFLE_CLIP_02", "COMPONENT_AT_SCOPE_MACRO", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_AFGRIP", "COMPONENT_AT_AR_FLSH", "COMPONENT_ASSAULTRIFLE_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_CARBINERIFLE{ "Extended Magazine", "Scope", "Suppressor", "Grip", "Flashlight", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_CARBINERIFLE{ "COMPONENT_CARBINERIFLE_CLIP_02", "COMPONENT_AT_SCOPE_MEDIUM", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_AFGRIP", "COMPONENT_AT_AR_FLSH", "COMPONENT_CARBINERIFLE_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_ADVANCEDRIFLE{ "Extended Magazine", "Scope", "Suppressor", "Flashlight", "Gilded Gun Metal Finish" };
const std::vector<std::string> VALUES_ATTACH_ADVANCEDRIFLE{ "COMPONENT_ADVANCEDRIFLE_CLIP_02", "COMPONENT_AT_SCOPE_SMALL", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_FLSH", "COMPONENT_ADVANCEDRIFLE_VARMOD_LUXE" };

//Machine Guns
const std::vector<std::string> CAPTIONS_ATTACH_MG{ "Extended Magazine", "Scope", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_MG{ "COMPONENT_MG_CLIP_02", "COMPONENT_AT_SCOPE_SMALL_02", "COMPONENT_MG_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_COMBATMG{ "Extended Magazine", "Scope", "Grip", "Etched Gun Metal Finish" };
const std::vector<std::string> VALUES_ATTACH_COMBATMG{ "COMPONENT_COMBATMG_CLIP_02", "COMPONENT_AT_SCOPE_MEDIUM", "COMPONENT_AT_AR_AFGRIP", "COMPONENT_COMBATMG_VARMOD_LUXE" };

//Shotguns

const std::vector<std::string> CAPTIONS_ATTACH_SAWNOFF{ "Gilded Gun Metal Finish" };
const std::vector<std::string> VALUES_ATTACH_SAWNOFF{ "COMPONENT_SAWNOFFSHOTGUN_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_PUMPSHOTGUN{ "Suppressor", "Flashlight", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_PUMPSHOTGUN{ "COMPONENT_AT_SR_SUPP", "COMPONENT_AT_AR_FLSH", "COMPONENT_PUMPSHOTGUN_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_ASSAULTSHOTGUN{ "Extended Magazine", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_ASSAULTSHOTGUN{ "COMPONENT_ASSAULTSHOTGUN_CLIP_02", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> CAPTIONS_ATTACH_BULLPUPSHOTGUN{ "Flashlight", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_BULLPUPSHOTGUN{ "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_AFGRIP" };

//Sniper Rifles
const std::vector<std::string> CAPTIONS_ATTACH_SNIPERRIFLE{ "Advanced Scope", "Suppressor", "Etched Wood Grip Finish" };
const std::vector<std::string> VALUES_ATTACH_SNIPERRIFLE{ "COMPONENT_AT_SCOPE_MAX", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_SNIPERRIFLE_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_HEAVYSNIPER{ "Normal Scope" };
const std::vector<std::string> VALUES_ATTACH_HEAVYSNIPER{ "COMPONENT_AT_SCOPE_LARGE" };

//Heavy Weapons
const std::vector<std::string> CAPTIONS_ATTACH_GRENADELAUNCHER{ "Scope", "Flashlight", "Grip" };
const std::vector<std::string> VALUES_ATTACH_GRENADELAUNCHER{ "COMPONENT_AT_SCOPE_SMALL", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_AR_AFGRIP" };

//DLC Weapons
const std::vector<std::string> CAPTIONS_ATTACH_BULLPUPRIFLE{ "Extended Magazine", "Flashlight", "Scope", "Suppressor", "Grip", "Gilded Gun Metal Finish" };
const std::vector<std::string> VALUES_ATTACH_BULLPUPRIFLE{ "COMPONENT_BULLPUPRIFLE_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_SCOPE_SMALL", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_AFGRIP", "COMPONENT_BULLPUPRIFLE_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_GUSENBERG{ "Extended Magazine" };
const std::vector<std::string> VALUES_ATTACH_GUSENBERG{ "COMPONENT_GUSENBERG_CLIP_02" };

const std::vector<std::string> CAPTIONS_ATTACH_HEAVYSHOTGUN{ "Extended Magazine", "Flashlight", "Suppressor", "Grip" };
const std::vector<std::string> VALUES_ATTACH_HEAVYSHOTGUN{ "COMPONENT_HEAVYSHOTGUN_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> CAPTIONS_ATTACH_MARKSMANRIFLE{ "Extended Magazine", "Flashlight", "Suppressor", "Grip", "Yusuf Amir Luxury Finish" };
const std::vector<std::string> VALUES_ATTACH_MARKSMANRIFLE{ "COMPONENT_MARKSMANRIFLE_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_AR_SUPP", "COMPONENT_AT_AR_AFGRIP", "COMPONENT_MARKSMANRIFLE_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_SPECIALCARBINE{ "Extended Magazine", "Flashlight", "Scope", "Suppressor", "Grip", "Etched Gun Metal Finish" };
const std::vector<std::string> VALUES_ATTACH_SPECIALCARBINE{ "COMPONENT_SPECIALCARBINE_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_SCOPE_MEDIUM", "COMPONENT_AT_AR_SUPP_02", "COMPONENT_AT_AR_AFGRIP", "COMPONENT_SPECIALCARBINE_VARMOD_LUXE" };

const std::vector<std::string> CAPTIONS_ATTACH_COMBATPDW{ "Extended Magazine", "Flashlight", "Scope", "Grip" };
const std::vector<std::string> VALUES_ATTACH_COMBATPDW{ "COMPONENT_COMBATPDW_CLIP_02", "COMPONENT_AT_AR_FLSH", "COMPONENT_AT_SCOPE_SMALL", "COMPONENT_AT_AR_AFGRIP" };

const std::vector<std::string> CAPTIONS_ATTACH_KNUCKLES{ "Default", "Pimp", "Ballas", "Dollars", "Diamond", "Hate", "Love", "Player", "King", "Vagos" };
const std::vector<std::string> VALUES_ATTACH_KNUCKLES{ "COMPONENT_KNUCKLE_VARMOD_BASE", "COMPONENT_KNUCKLE_VARMOD_PIMP", "COMPONENT_KNUCKLE_VARMOD_BALLAS", "COMPONENT_KNUCKLE_VARMOD_DOLLAR", "COMPONENT_KNUCKLE_VARMOD_DIAMOND", "COMPONENT_KNUCKLE_VARMOD_HATE", "COMPONENT_KNUCKLE_VARMOD_LOVE", "COMPONENT_KNUCKLE_VARMOD_PLAYER", "COMPONENT_KNUCKLE_VARMOD_KING", "COMPONENT_KNUCKLE_VARMOD_VAGOS" };

const std::vector<std::string> CAPTIONS_ATTACH_MACHINEPISTOL{ "Extended Magazine", "Suppressor" };
const std::vector<std::string> VALUES_ATTACH_MACHINEPISTOL{ "COMPONENT_MACHINEPISTOL_CLIP_02" "COMPONENT_AT_PI_SUPP_02" };

const std::vector<std::string> CAPTIONS_ATTACH_REVOLVER{ "VIP Finish", "Bodyguard Finish" };
const std::vector<std::string> VALUES_ATTACH_REVOLVER{ "COMPONENT_REVOLVER_VARMOD_BOSS", "COMPONENT_REVOLVER_VARMOD_GOON" };

const std::vector<std::string> CAPTIONS_ATTACH_SWITCHBLADE{ "Default", "VIP Finish", "Bodyguard Finish" };
const std::vector<std::string> VALUES_ATTACH_SWITCHBLADE{ "COMPONENT_SWITCHBLADE_VARMOD_BASE", "COMPONENT_SWITCHBLADE_VARMOD_VAR1", "COMPONENT_SWITCHBLADE_VARMOD_VAR2" };

const std::vector<std::string> CAPTIONS_ATTACH_COMPACTRIFLE{ "Default Clip", "Extended Clip" };
const std::vector<std::string> VALUES_ATTACH_COMPACTRIFLE{ "COMPONENT_COMPACTRIFLE_CLIP_01", "COMPONENT_COMPACTRIFLE_CLIP_02" };

//Tintable weapons list
const std::vector<std::string> WEAPONTYPES_TINT{ "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_SNSPISTOL", "WEAPON_HEAVYPISTOL", "WEAPON_VINTAGEPISTOL", "WEAPON_STUNGUN", "WEAPON_FLAREGUN", "WEAPON_MARKSMANPISTOL", "WEAPON_MICROSMG", "WEAPON_SMG", "WEAPON_ASSAULTSMG", "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_GUSENBERG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_SPECIALCARBINE", "WEAPON_BULLPUPRIFLE", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_BULLPUPSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_MUSKET", "WEAPON_HEAVYSHOTGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_MARKSMANRIFLE", "WEAPON_GRENADELAUNCHER", "WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_FIREWORK", "WEAPON_RAILGUN", "WEAPON_HOMINGLAUNCHER", "WEAPON_COMBATPDW", "WEAPON_MACHINEPISTOL", "WEAPON_DBSHOTGUN", "WEAPON_COMPACTRIFLE" };

//moddable weapons list
const std::vector<std::string> WEAPONTYPES_MOD{ "WEAPON_PISTOL", "WEAPON_HEAVYPISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG", "WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_PUMPSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_BULLPUPRIFLE", "WEAPON_GUSENBERG", "WEAPON_HEAVYSHOTGUN", "WEAPON_MARKSMANRIFLE", "WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_VINTAGEPISTOL", "WEAPON_COMBATPDW", "WEAPON_COMPACTRIFLE" };

const std::vector<std::string> VOV_WEAPONMOD_CAPTIONS[] = { CAPTIONS_ATTACH_PISTOL, CAPTIONS_ATTACH_HEAVYPISTOL, CAPTIONS_ATTACH_COMBATPISTOL, CAPTIONS_ATTACH_APPISTOL, CAPTIONS_ATTACH_PISTOL50, CAPTIONS_ATTACH_MICROSMG, CAPTIONS_ATTACH_SMG, CAPTIONS_ATTACH_ASSAULTSMG, CAPTIONS_ATTACH_ASSAULTRIFLE, CAPTIONS_ATTACH_CARBINERIFLE, CAPTIONS_ATTACH_ADVANCEDRIFLE, CAPTIONS_ATTACH_MG, CAPTIONS_ATTACH_COMBATMG, CAPTIONS_ATTACH_SAWNOFF, CAPTIONS_ATTACH_PUMPSHOTGUN, CAPTIONS_ATTACH_ASSAULTSHOTGUN, CAPTIONS_ATTACH_BULLPUPSHOTGUN, CAPTIONS_ATTACH_SNIPERRIFLE, CAPTIONS_ATTACH_HEAVYSNIPER, CAPTIONS_ATTACH_GRENADELAUNCHER, CAPTIONS_ATTACH_BULLPUPRIFLE, CAPTIONS_ATTACH_GUSENBERG, CAPTIONS_ATTACH_HEAVYSHOTGUN, CAPTIONS_ATTACH_MARKSMANRIFLE, CAPTIONS_ATTACH_SNSPISTOL, CAPTIONS_ATTACH_SPECIALCARBINE, CAPTIONS_ATTACH_VINTAGEPISTOL, CAPTIONS_ATTACH_COMBATPDW, CAPTIONS_ATTACH_MACHINEPISTOL, CAPTIONS_ATTACH_SWITCHBLADE, CAPTIONS_ATTACH_COMPACTRIFLE };
const std::vector<std::string> VOV_WEAPONMOD_VALUES[] = { VALUES_ATTACH_PISTOL, VALUES_ATTACH_HEAVYPISTOL, VALUES_ATTACH_COMBATPISTOL, VALUES_ATTACH_APPISTOL, VALUES_ATTACH_PISTOL50, VALUES_ATTACH_MICROSMG, VALUES_ATTACH_SMG, VALUES_ATTACH_ASSAULTSMG, VALUES_ATTACH_ASSAULTRIFLE, VALUES_ATTACH_CARBINERIFLE, VALUES_ATTACH_ADVANCEDRIFLE, VALUES_ATTACH_MG, VALUES_ATTACH_COMBATMG, VALUES_ATTACH_SAWNOFF, VALUES_ATTACH_PUMPSHOTGUN, VALUES_ATTACH_ASSAULTSHOTGUN, VALUES_ATTACH_BULLPUPSHOTGUN, VALUES_ATTACH_SNIPERRIFLE, VALUES_ATTACH_HEAVYSNIPER, VALUES_ATTACH_GRENADELAUNCHER, VALUES_ATTACH_BULLPUPRIFLE, VALUES_ATTACH_GUSENBERG, VALUES_ATTACH_HEAVYSHOTGUN, VALUES_ATTACH_MARKSMANRIFLE, VALUES_ATTACH_SNSPISTOL, VALUES_ATTACH_SPECIALCARBINE, VALUES_ATTACH_VINTAGEPISTOL, VALUES_ATTACH_COMBATPDW, VALUES_ATTACH_MACHINEPISTOL, VALUES_ATTACH_SWITCHBLADE, VALUES_ATTACH_REVOLVER, VALUES_ATTACH_COMPACTRIFLE };

//Weapon damage modifier list
const std::vector<std::string> WEAP_DMG_CAPTIONS{ "1.0x", "1.5x", "2.0x", "3.0x", "5.0x", "10.0x", "50.0x", "100.0x", "1000.0x" };
const std::vector<float> WEAP_DMG_FLOAT{ 1.0, 1.5, 2.0, 3.0, 5.0, 10.0, 50.0, 100.0, 1000.0 };

const int PARACHUTE_ID = 0xFBAB5776;

const int TOTAL_WEAPONS_COUNT = 58;
const int TOTAL_WEAPONS_SLOTS = 57;
const int MAX_MOD_SLOTS = 15;

bool process_weapon_menu();

bool process_give_melee();
bool do_give_weapon(std::string modelName);
bool process_weaponlist_menu();

void reset_weapon_globals();

void update_weapon_features(BOOL bPlayerExists, Player player);

void update_vehicle_guns();

void save_player_weapons();

void restore_player_weapons();

bool is_weapon_equipped(std::vector<int> extras);

void set_weapon_equipped(bool equipped, std::vector<int> extras);

bool is_weaponmod_equipped(std::vector<int> extras);

void set_weaponmod_equipped(bool equipped, std::vector<int> extras);

void give_weapon_clip(MenuItem<int> choice);

void fill_weapon_ammo(MenuItem<int> choice);

void onconfirm_open_tint_menu(MenuItem<int> choice);

bool process_weapon_mod_menu_tint();

//Weapon mod menu

bool process_weapon_mod_menu_tint();

bool onconfirm_weapon_mod_menu_tint(MenuItem<int> choice);

void add_weapon_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

bool weapon_reequip_interrupt();

// weapon damage modifier
void onchange_weap_dmg_modifier(int value, SelectFromListMenuItem* source);

void add_weapons_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_weapons(std::vector<StringPairSettingDBRow>* settings);
