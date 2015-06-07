/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <stdio.h>
#include "inc/sqlite3/sqlite3.h"

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include <vector>

#pragma once

struct FeatureEnabledDBRow
{
	char* name;
	bool enabled;
};

struct StringPairSettingDBRow
{
	std::string name;
	std::string value;
};

class SavedVehicleModDBRow
{
public:
	int rowID;
	int parentID;
	int modID;
	int modState;
	bool isToggle;
};

class SavedVehicleExtraDBRow
{
public:
	int rowID;
	int parentID;
	int extraID;
	int extraState;
};

class SavedVehicleDBRow
{
public:

	int rowID;
	std::string saveName;
	DWORD model;
	int colourPrimary;
	int colourSecondary;
	int colourExtraPearl;
	int colourExtraWheel;
	int colourMod1Type;
	int colourMod1Colour;
	int colourMod1P3;
	int colourMod2Type;
	int colourMod2Colour;
	int colourCustom1RGB[3];
	int colourCustom2RGB[3];
	int livery;
	std::string plateText;
	int plateType;
	int wheelType;
	int windowTint;
	bool burstableTyres;

	std::vector<SavedVehicleExtraDBRow*> extras;
	std::vector<SavedVehicleModDBRow*> mods;

	inline ~SavedVehicleDBRow()
	{
		for (std::vector<SavedVehicleExtraDBRow*>::iterator it = extras.begin(); it != extras.end(); ++it)
		{
			delete (*it);
		}

		for (std::vector<SavedVehicleModDBRow*>::iterator it = mods.begin(); it != mods.end(); ++it)
		{
			delete (*it);
		}
	}

	inline SavedVehicleDBRow()
	{

	}
};

class FeatureEnabledLocalDefinition
{
public:
	char* name;
	bool* enabled;
	bool* updateFlag = NULL;

	inline FeatureEnabledLocalDefinition(char* name, bool* enabled, bool* updateFlag = NULL)
	{
		this->name = name;
		this->enabled = enabled;
		this->updateFlag = updateFlag;
	};
};

class ENTDatabase
{
public:

	virtual ~ENTDatabase() {};

	bool open();

	void close();

	void store_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values);

	void load_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values);

	void store_setting_pairs(std::vector<StringPairSettingDBRow> values);

	std::vector<StringPairSettingDBRow> load_setting_pairs();

	bool save_vehicle(Vehicle veh, std::string saveName, int slot);

	std::vector<SavedVehicleDBRow*> get_saved_vehicles(int index=-1);

	void populate_saved_vehicle(SavedVehicleDBRow *entry);

	void delete_saved_vehicle(int slot);

private:

	void save_vehicle_extras(Vehicle veh, int rowID);

	void save_vehicle_mods(Vehicle veh, int rowID);

	void ENTDatabase::handle_version(int oldVersion);

	sqlite3 *db;

	char *zErrMsg = 0;

	int manifest_version = 0;
};