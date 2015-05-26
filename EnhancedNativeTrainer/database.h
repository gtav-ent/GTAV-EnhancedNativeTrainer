/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <stdio.h>
#include "inc/sqlite3/sqlite3.h"

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

	void open();

	void close();

	void store_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values);

	void load_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values);

	void store_setting_pairs(std::vector<StringPairSettingDBRow> values);

	std::vector<StringPairSettingDBRow> load_setting_pairs();

private:

	void ENTDatabase::handle_version(int oldVersion);

	sqlite3 *db;

	char *zErrMsg = 0;

	int manifest_version = 0;
};