/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "..\storage\database.h"
#include "..\ui_support\menu_functions.h"

#include <string>
#include <sstream> 
#include <fstream>
#include <vector>
#include <set>
#include <queue>

class ENTTrackedPedestrian
{
public:

	Ped ped;
	bool angryApplied = false;
	int weaponSetApplied = 0;
	bool missionised = true;
	bool madeInvincible = false;

	virtual ~ENTTrackedPedestrian();

	inline ENTTrackedPedestrian(Ped ped)
	{
		this->ped = ped;
	}

protected:

};

class ENTTrackedVehicle
{
public:

	Vehicle vehicle;
	bool missionised = true;

	virtual ~ENTTrackedVehicle();

	inline ENTTrackedVehicle(Vehicle vehicle)
	{
		this->vehicle = vehicle;
	}

protected:

};

/*const std::vector<std::string> PED_SKINS_ARMY{ "s_m_y_blackops_01", "s_m_y_blackops_02", "s_m_y_blackops_03", "s_m_y_armymech_01", "csb_ramp_marine", "s_m_m_marine_01", "s_m_m_marine_02", "s_m_y_marine_01", "s_m_y_marine_02", "s_m_y_marine_03" };

const std::vector<std::string> PED_SKINS_STRIPPERS{ "csb_stripper_01", "s_f_y_stripper_01", "csb_stripper_02", "s_f_y_stripper_02", "s_f_y_stripperlite", "mp_f_stripperlite" };

const std::vector<std::string> PED_SKINS_ZOMBIES{ "u_m_y_zombie_01" };

const std::vector<std::vector<std::string>> VOV_PED_SKINS{ {}, PED_SKINS_ARMY, PED_SKINS_STRIPPERS, PED_SKINS_ZOMBIES };

const std::vector<std::string> PED_SKIN_TITLES{ "Unchanged", "Army", "Strippers", "Zombies" };*/

const std::vector<std::string> PED_WEAPONS_MELEE{ "WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR", "WEAPON_BOTTLE", "WEAPON_DAGGER", "WEAPON_HATCHET", "WEAPON_KNUCKLE", "WEAPON_MACHETE", "WEAPON_FLASHLIGHT", "WEAPON_SWITCHBLADE" };

const std::vector<std::string> PED_WEAPONS_SMALL{ "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_SNSPISTOL", "WEAPON_HEAVYPISTOL", "WEAPON_VINTAGEPISTOL", "WEAPON_STUNGUN", "WEAPON_FLAREGUN", "WEAPON_MARKSMANPISTOL", "WEAPON_REVOLVER" };

const std::vector<std::string> PED_WEAPONS_RIFLES{ "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_SPECIALCARBINE", "WEAPON_BULLPUPRIFLE", "WEAPON_COMPACTRIFLE" };

const std::vector<std::string> PED_WEAPONS_HEAVY{ "WEAPON_MINIGUN", "WEAPON_HEAVYSNIPER", "WEAPON_MG", "WEAPON_COMBATMG" };

const std::vector<std::string> PED_WEAPONS_EXPLOSIVES{ "WEAPON_GRENADELAUNCHER", "WEAPON_RPG", "WEAPON_RAILGUN", "WEAPON_HOMINGLAUNCHER", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_PROXMINE", "WEAPON_SMOKEGRENADE", "WEAPON_MOLOTOV" };

const std::vector<std::string> PED_WEAPONS_RANDOM{ "WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR", "WEAPON_BOTTLE", "WEAPON_DAGGER", "WEAPON_HATCHET", "WEAPON_KNUCKLE", "WEAPON_MACHETE", "WEAPON_FLASHLIGHT", "WEAPON_SWITCHBLADE", "WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_SNSPISTOL", "WEAPON_HEAVYPISTOL", "WEAPON_VINTAGEPISTOL", "WEAPON_STUNGUN", "WEAPON_FLAREGUN", "WEAPON_MARKSMANPISTOL", "WEAPON_REVOLVER", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_SPECIALCARBINE", "WEAPON_BULLPUPRIFLE", "WEAPON_COMPACTRIFLE", "WEAPON_MINIGUN", "WEAPON_HEAVYSNIPER", "WEAPON_MG", "WEAPON_COMBATMG", "WEAPON_GRENADELAUNCHER", "WEAPON_RPG", "WEAPON_RAILGUN", "WEAPON_HOMINGLAUNCHER", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_PROXMINE", "WEAPON_SMOKEGRENADE", "WEAPON_MOLOTOV" };

const std::vector<std::vector<std::string>> VOV_PED_WEAPONS{ {}, PED_WEAPONS_MELEE, PED_WEAPONS_SMALL, PED_WEAPONS_RIFLES, PED_WEAPONS_HEAVY, PED_WEAPONS_EXPLOSIVES, PED_WEAPONS_RANDOM };

const std::vector<std::string> PED_WEAPON_TITLES{ "None (Normal)", "Melee", "Small Arms", "Rifles", "Heavy", "Explosives", "Random" };

bool onconfirm_areaeffect_ped_menu(MenuItem<int> choice);

void process_areaeffect_menu();

void process_areaeffect_advanced_ped_menu();

void update_area_effects(Ped playerPed);

std::set<Ped> get_nearby_peds(Ped playerPed);

std::set<Vehicle> get_nearby_vehicles(Ped playerPed);

void cleanup_area_effects();

void reset_areaeffect_globals();

void add_areaeffect_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_areaeffect_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_areaeffect(std::vector<StringPairSettingDBRow>* settings);

void set_all_nearby_peds_to_invincible(bool enabled);

void set_all_nearby_peds_to_calm();

void set_all_nearby_peds_to_angry(bool enabled);

void give_all_nearby_peds_a_weapon(bool enabled);

void kill_all_nearby_peds();

void kill_all_nearby_vehicles();

void set_all_nearby_vehs_to_invincible(bool enabled);

void set_all_nearby_vehs_to_broken(bool enabled);

void clear_up_missionised_entitities();

void onchange_areaeffect_ped_weapons(int value, SelectFromListMenuItem* source);

ENTTrackedPedestrian* findOrCreateTrackedPed(Ped ped);

ENTTrackedVehicle* findOrCreateTrackedVehicle(Vehicle vehicle);