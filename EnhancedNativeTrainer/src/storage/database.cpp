/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "database.h"
#include "..\debug\debuglog.h"
#include "..\features\script.h"

#include <string>
#include <sstream>

#pragma warning(disable : 4267) // size_t conversion

/**This value should be increased whenever you change the schema and a release is made.
However you must also put in code to upgrade from older versions, in ENTDatabase::handle_version,
as they will be deployed in the wild already.*/
const int DATABASE_VERSION = 9;

static int singleIntResultCallback(void *data, int count, char **rows, char **azColName)
{
	if (count == 1 && rows)
	{
		*static_cast<int*>(data) = atoi(rows[0]);
		return 0;
	}
	return 1;
}

static int emptyCallback(void *data, int count, char **rows, char **azColName)
{
	return 0;
}

static int featureEnablementFetchCallback(void *data, int count, char **rows, char **azColName)
{
	std::vector<FeatureEnabledLocalDefinition> defs = *static_cast<std::vector<FeatureEnabledLocalDefinition>*>(data);
	int enabledInt = atoi(rows[1]);
	char *name = rows[0];

	for (int i = 0; i < defs.size(); i++)
	{
		FeatureEnabledLocalDefinition def = defs.at(i);

		if (strcmp(def.name, name) == 0)
		{
			*def.enabled = enabledInt ? 1 : 0;
			if (def.updateFlag != NULL)
			{
				*def.updateFlag = true;
			}
			break;
		}
	}

	return 0;
}

static int genericSettingPairsFetchCallback(void *data, int count, char **rows, char **azColName)
{
	std::vector<StringPairSettingDBRow> *results = static_cast<std::vector<StringPairSettingDBRow>*>(data);
	std::string k = std::string(rows[0]);
	std::string v = std::string(rows[1]);
	StringPairSettingDBRow setting{k,v};
	results->push_back(setting);
	return 0;
}

void ENTDatabase::handle_version(int oldVersion)
{
	if (oldVersion == -1)
	{
		write_text_to_log_file("Feature enablement table not found, so creating it");
		char* CREATE_VERSION_TABLE_QUERY = "create table ENT_FEATURE_ENABLEMENT (FEATURE_NAME TEXT PRIMARY KEY NOT NULL, FEATURE_ENABLED INT NOT NULL)";
		int rcFeat = sqlite3_exec(db, CREATE_VERSION_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcFeat != SQLITE_OK)
		{
			write_text_to_log_file("Enablement table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Enablement table created");
		}
	}

	if (oldVersion < 2)
	{
		write_text_to_log_file("Generic setting pairs table not found, so creating it");
		char* CREATE_VERSION_TABLE_QUERY = "create table ENT_SETTING_PAIRS (SETTING_NAME TEXT PRIMARY KEY NOT NULL, SETTING_VALUE TEXT NOT NULL)";
		int rcGen = sqlite3_exec(db, CREATE_VERSION_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcGen != SQLITE_OK)
		{
			write_text_to_log_file("Generic setting pairs table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Generic setting pairs table created");
		}
	}

	if (oldVersion < 3)
	{
		write_text_to_log_file("Main vehicle table not found, so creating it");
		char* CREATE_VEHICLE_TABLE_QUERY = "CREATE TABLE ENT_SAVED_VEHICLES ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			saveName TEXT NOT NULL, \
			model INTEGER NOT NULL, \
			colourPrimary INTEGER, \
			colourSecondary INTEGER, \
			colourExtraPearl INTEGER, \
			colourExtraWheel INTEGER, \
			colourMod1Type INTEGER, \
			colourMod1Colour INTEGER, \
			colourMod1P3 INTEGER, \
			colourMod2Type INTEGER, \
			colourMod2Colour INTEGER, \
			colourCustom1R INTEGER, \
			colourCustom1G INTEGER, \
			colourCustom1B INTEGER, \
			colourCustom2R INTEGER, \
			colourCustom2G INTEGER, \
			colourCustom2B INTEGER, \
			livery INTEGER, \
			plateText TEXT, \
			plateType INTEGER, \
			wheelType INTEGER, \
			windowTint INTEGER, \
			burstableTyres INTEGER, \
			customTyres INTEGER, \
			dirtLevel REAL DEFAULT 0, \
			fadeLevel REAL DEFAULT 0, \
			neonR INTEGER DEFAULT -1, \
			neonG INTEGER DEFAULT -1, \
			neonB INTEGER DEFAULT -1, \
			neon0Enabled INTEGER DEFAULT 0, \
			neon1Enabled INTEGER DEFAULT 0, \
			neon2Enabled INTEGER DEFAULT 0, \
			neon3Enabled INTEGER DEFAULT 0, \
			tyreSmokeR INTEGER DEFAULT -1, \
			tyreSmokeG INTEGER DEFAULT -1, \
			tyreSmokeB INTEGER DEFAULT -1, \
			convertibleRoofUp INTEGER DEFAULT 0, \
			dashColour INTEGER DEFAULT -1, \
			interiorColour INTEGER DEFAULT -1 \
			)";
		int rcVeh1 = sqlite3_exec(db, CREATE_VEHICLE_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh1 != SQLITE_OK)
		{
			write_text_to_log_file("Main vehicle table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Main vehicle table created");
		}

		write_text_to_log_file("Vehicle extras table not found, so creating it");
		char* CREATE_VEHEXTRAS_TABLE_QUERY = "CREATE TABLE ENT_VEHICLE_EXTRAS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			extraId INTEGER NOT NULL, \
			extraState INTEGER NOT NULL, \
			UNIQUE(parentId, extraId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_VEHICLES(id) ON DELETE CASCADE \
			)";
		int rcVeh2 = sqlite3_exec(db, CREATE_VEHEXTRAS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh2 != SQLITE_OK)
		{
			write_text_to_log_file("Vehicle extras table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Vehicle extras table created");
		}

		write_text_to_log_file("Vehicle mods table not found, so creating it");
		char* CREATE_VEHMODS_TABLE_QUERY = "CREATE TABLE ENT_VEHICLE_MODS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			modId INTEGER NOT NULL, \
			modState INTEGER NOT NULL, \
			isToggle INTEGER NOT NULL, \
			UNIQUE(parentId, modId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_VEHICLES(id) ON DELETE CASCADE \
			)";

		int rcVeh3 = sqlite3_exec(db, CREATE_VEHMODS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh3 != SQLITE_OK)
		{
			write_text_to_log_file("Vehicle mods table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Vehicle mods table created");
		}
	}

	if (oldVersion < 4)
	{
		write_text_to_log_file("Main skin table not found, so creating it");

		char* CREATE_SKIN_TABLE_QUERY = "CREATE TABLE ENT_SAVED_SKINS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			saveName TEXT NOT NULL, \
			model INTEGER NOT NULL \
			)";

		int rcSkin1 = sqlite3_exec(db, CREATE_SKIN_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin1 != SQLITE_OK)
		{
			write_text_to_log_file("Main skin table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Main skin table created");
		}

		write_text_to_log_file("Skin components table not found, so creating it");
		char* CREATE_SKIN_COMPS_TABLE_QUERY = "CREATE TABLE ENT_SKIN_COMPONENTS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			slotId INTEGER NOT NULL, \
			drawable INTEGER NOT NULL, \
			texture INTEGER NOT NULL, \
			UNIQUE(parentId, slotId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_SKINS(id) ON DELETE CASCADE \
			)";

		int rcSkin2 = sqlite3_exec(db, CREATE_SKIN_COMPS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin2 != SQLITE_OK)
		{
			write_text_to_log_file("Skin components table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Skin components table created");
		}

		write_text_to_log_file("Skin props table not found, so creating it");
		char* CREATE_SKIN_PROPS_TABLE_QUERY = "CREATE TABLE ENT_SKIN_PROPS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			propId INTEGER NOT NULL, \
			drawable INTEGER NOT NULL, \
			texture INTEGER NOT NULL, \
			UNIQUE(parentId, propId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_SKINS(id) ON DELETE CASCADE \
			)";

		int rcSkin3 = sqlite3_exec(db, CREATE_SKIN_PROPS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin3 != SQLITE_OK)
		{
			write_text_to_log_file("Skin props table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Skin props table created");
		}
	}

	if (oldVersion >= 3 && oldVersion < 5)
	{
		char* ADD_TYRES_COL = "ALTER TABLE ENT_SAVED_VEHICLES ADD customTyres INTEGER DEFAULT 0";
		
		int custTyresAddition = sqlite3_exec(db, ADD_TYRES_COL, NULL, 0, &zErrMsg);
		if (custTyresAddition != SQLITE_OK)
		{
			write_text_to_log_file("Couldn't add custom tyres column");
			sqlite3_free(zErrMsg);
		}
	}

	if (oldVersion >= 3 && oldVersion < 6)
	{
		char* queries[]
		{
			"ALTER TABLE ENT_SAVED_VEHICLES ADD dirtLevel REAL DEFAULT 0",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD fadeLevel REAL DEFAULT 0",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD neonR INTEGER DEFAULT -1",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD neonG INTEGER DEFAULT -1",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD neonB INTEGER DEFAULT -1",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD neon0Enabled INTEGER DEFAULT 0",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD neon1Enabled INTEGER DEFAULT 0",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD neon2Enabled INTEGER DEFAULT 0",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD neon3Enabled INTEGER DEFAULT 0",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD tyreSmokeR INTEGER DEFAULT -1",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD tyreSmokeG INTEGER DEFAULT -1",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD tyreSmokeB INTEGER DEFAULT -1",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD convertibleRoofUp INTEGER DEFAULT 0"
		};

		for each (char* q in queries)
		{
			int extraColsAddition = sqlite3_exec(db, q, NULL, 0, &zErrMsg);
			if (extraColsAddition != SQLITE_OK)
			{
				write_text_to_log_file("Couldn't add v6 vehicle column");
				sqlite3_free(zErrMsg);
			}
		}
	}


	if (oldVersion == 7)
	{
		{
			int dropTable = sqlite3_exec(db, "DROP TABLE ENT_PROP_INSTANCES;", NULL, 0, &zErrMsg);
			if (dropTable != SQLITE_OK)
			{
				write_text_to_log_file("Couldn't alter props table");
				sqlite3_free(zErrMsg);
			}
		}
		{
			int dropTable = sqlite3_exec(db, "DROP TABLE ENT_PROP_SETS;", NULL, 0, &zErrMsg);
			if (dropTable != SQLITE_OK)
			{
				write_text_to_log_file("Couldn't alter props table");
				sqlite3_free(zErrMsg);
			}
		}
	}

	if (oldVersion < 8)
	{
		write_text_to_log_file("Props sets table not found, so creating it");
		char* CREATE_PROP_SETS_TABLE_QUERY = "CREATE TABLE ENT_PROP_SETS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			name TEXT NOT NULL)";

		int propSetRC = sqlite3_exec(db, CREATE_PROP_SETS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (propSetRC != SQLITE_OK)
		{
			write_text_to_log_file("Prop set table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Prop set table creation problem");
		}

		write_text_to_log_file("Props instances table not found, so creating it");
		char* CREATE_PROP_INSTANCES_TABLE_QUERY = "CREATE TABLE ENT_PROP_INSTANCES ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			modelHash INT NOT NULL, \
			title TEXT NOT NULL, \
			counter INTEGER NOT NULL, \
			posX REAL NOT NULL, \
			posY REAL NOT NULL, \
			posZ REAL NOT NULL, \
			pitch REAL NOT NULL, \
			roll REAL NOT NULL, \
			yaw REAL NOT NULL, \
			isImmovable INT NOT NULL, \
			isInvincible INT NOT NULL, \
			hasGravity INT NOT NULL, \
			alpha INT NOT NULL, \
			FOREIGN KEY (parentId) REFERENCES ENT_PROP_SETS(id) ON DELETE CASCADE)";

		int propInstanceRC = sqlite3_exec(db, CREATE_PROP_INSTANCES_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (propInstanceRC != SQLITE_OK)
		{
			write_text_to_log_file("Prop instance table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Prop instance table creation problem");
		}
	}

	if (oldVersion >= 3 && oldVersion < 9)
	{
		char* queries[]
		{
			"ALTER TABLE ENT_SAVED_VEHICLES ADD dashColour INTEGER DEFAULT -1",
				"ALTER TABLE ENT_SAVED_VEHICLES ADD interiorColour INTEGER DEFAULT -1"
		};

		for each (char* q in queries)
		{
			int extraColsAddition = sqlite3_exec(db, q, NULL, 0, &zErrMsg);
			if (extraColsAddition != SQLITE_OK)
			{
				write_text_to_log_file("Couldn't add v9 vehicle column");
				sqlite3_free(zErrMsg);
			}
		}
	}
}

bool ENTDatabase::open()
{
	std::stringstream ss;

	write_text_to_log_file("Initialising DB engine");

	sqlite3_initialize();

	db_mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_RECURSIVE);

	write_text_to_log_file("Opening DB file");

	WCHAR* db_path = get_storage_dir_path("ent.db");

	std::wstring ws(db_path);
	std::string fileSS(ws.begin(), ws.end());
	write_text_to_log_file(fileSS);

	mutex_lock();

	int rc = sqlite3_open16(db_path, &db);
	delete db_path;
	if (rc == SQLITE_OK)
	{
		write_text_to_log_file("DB opened");
	}
	else
	{
		write_text_to_log_file("DB couldn't be opened or created");
		ss.str(""); ss.clear();
		ss << "DB error code was: " << rc;
		write_text_to_log_file(ss.str());
		close();
		mutex_unlock();
		return false;
	}

	char* PRAGMA_FKEY_QUERY = "PRAGMA foreign_keys = ON";
	int rcPragma = sqlite3_exec(db, PRAGMA_FKEY_QUERY, NULL, 0, &zErrMsg);
	if (rcPragma != SQLITE_OK)
	{
		write_text_to_log_file("Couldn't exec pragma statement");
		sqlite3_free(zErrMsg);
	}

	int count = 0;
	char* VERSION_TABLE_EXISTS_QUERY = "select count(type) from sqlite_master where type = 'table' and name = 'ENT_DB_MANIFEST' ";
	rc = sqlite3_exec(db, VERSION_TABLE_EXISTS_QUERY, singleIntResultCallback, &count, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("Couldn't query sqlite_master");
		sqlite3_free(zErrMsg);
	}

	if (count == 0 )
	{
		write_text_to_log_file("Manifest table not found, so creating it");
		char* CREATE_VERSION_TABLE_QUERY = "create table ENT_DB_MANIFEST (MANIFEST_KEY TEXT PRIMARY KEY NOT NULL, MANIFEST_VALUE TEXT NOT NULL)";
		rc = sqlite3_exec(db, CREATE_VERSION_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Manifest table creation problem");
			sqlite3_free(zErrMsg);
			mutex_unlock();
			return false;
		}
		else
		{
			write_text_to_log_file("Manifest table created");
		}
	}

	int dbversion = -1;
	char* FIND_VERSION_TABLE_QUERY = "select CAST(MANIFEST_VALUE as integer) from ENT_DB_MANIFEST where MANIFEST_KEY='VERSION'";
	rc = sqlite3_exec(db, FIND_VERSION_TABLE_QUERY, singleIntResultCallback, &dbversion, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("Version not found");
		sqlite3_free(zErrMsg);
	}
	else
	{
		ss.str(""); ss.clear();
		ss << "Version found: " << dbversion;
		write_text_to_log_file(ss.str());
		handle_version(dbversion);
	}

	if (dbversion != DATABASE_VERSION)
	{
		ss.str(""); ss.clear();
		ss << "INSERT OR REPLACE INTO ENT_DB_MANIFEST (MANIFEST_KEY, MANIFEST_VALUE) VALUES('VERSION', '" << DATABASE_VERSION << "')";
		auto ssStr = ss.str();
		rc = sqlite3_exec(db, ssStr.c_str(), emptyCallback, NULL, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Couldn't update version");
			write_text_to_log_file(ss.str());
			write_text_to_log_file(zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Updated version");
		}
	}

	mutex_unlock();
	return true;
}

void ENTDatabase::close()
{
	write_text_to_log_file("DB closing");
	if (db != NULL)
	{
		int rc = sqlite3_close_v2(db);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("DB not closed properly!");
		}
		db = NULL;
	}

	sqlite3_mutex_free(db_mutex);

	write_text_to_log_file("DB closed");
	sqlite3_shutdown();
	write_text_to_log_file("DB shutdown");
}

void ENTDatabase::store_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values)
{
	bool cacheIsSame = true;
	for each (FeatureEnabledLocalDefinition def in values)
	{
		if (featureEnablementCache.find(def.name) == featureEnablementCache.end())
		{
			cacheIsSame = false;
			break;
		}
		bool enabledInCache = featureEnablementCache[def.name];
		if ( *def.enabled != enabledInCache)
		{
			cacheIsSame = false;
			break;
		}
	}

	if (cacheIsSame)
	{
		return;
	}

	mutex_lock();
	begin_transaction();

	write_text_to_log_file("Asked to store feature pairs");
	for (int i = 0; i < values.size(); i++)
	{
		FeatureEnabledLocalDefinition def = values.at(i);
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_FEATURE_ENABLEMENT VALUES ('" << def.name << "', " << (*def.enabled ? 1 : 0) << ")";
		auto ssStr = ss.str();
		int rc = sqlite3_exec(db, ssStr.c_str(), emptyCallback, NULL, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Failed to insert feature enablement row");
			write_text_to_log_file(ss.str());
			write_text_to_log_file(zErrMsg);
			sqlite3_free(zErrMsg);
			break;
		}
	}

	for each (FeatureEnabledLocalDefinition def in values)
	{
		if (featureEnablementCache.find(def.name) == featureEnablementCache.end())
		{
			std::pair<std::string, bool> pair(std::string(def.name), *def.enabled);
			featureEnablementCache.insert(pair);
		}
		else
		{
			featureEnablementCache[def.name] = *def.enabled;
		}
	}

	end_transaction();
	mutex_unlock();

	write_text_to_log_file("Done storing feature pairs");
}

void ENTDatabase::load_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values)
{
	mutex_lock();

	{
		std::stringstream ss;
		ss << "Asked to load " << values.size() << " feature pairs";
		write_text_to_log_file(ss.str());
	}
	std::vector<FeatureEnabledDBRow> dbPairs;
	char* QUERY = "select FEATURE_NAME, FEATURE_ENABLED from ENT_FEATURE_ENABLEMENT";
	int rc = sqlite3_exec(db, QUERY, featureEnablementFetchCallback, &values, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("Pairs not loaded");
		write_text_to_log_file(zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		write_text_to_log_file("Done loading feature pairs");
	}

	for each (FeatureEnabledLocalDefinition def in values)
	{
		if (featureEnablementCache.find(def.name) == featureEnablementCache.end())
		{
			std::pair<std::string, bool> pair(std::string(def.name), *def.enabled);
			featureEnablementCache.insert(pair);
		}
		else
		{
			featureEnablementCache[def.name] = *def.enabled;
		}
	}

	mutex_unlock();
}

void ENTDatabase::store_setting_pairs(std::vector<StringPairSettingDBRow> values)
{
	bool cacheIsSame = true;
	for each (StringPairSettingDBRow row in values)
	{
		if (genericSettingsCache.find(row.name) == genericSettingsCache.end())
		{
			cacheIsSame = false;
			break;
		}
		std::string valueInCache = genericSettingsCache[row.name];
		if (valueInCache.compare(row.value) != 0)
		{
			cacheIsSame = false;
			break;
		}
	}

	if (cacheIsSame)
	{
		return;
	}

	mutex_lock();
	begin_transaction();

	{
		std::stringstream ss;
		ss << "Asked to store " << values.size() << " generic pairs";
		write_text_to_log_file(ss.str());
	}

	write_text_to_log_file("Asked to store generic pairs");
	for (int i = 0; i < values.size(); i++)
	{
		StringPairSettingDBRow setting = values.at(i);
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_SETTING_PAIRS VALUES (?, ?);";

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc == SQLITE_OK)
		{
			{
				std::stringstream ss;
				ss << "Storing generic pair " << setting.name << " with value " << setting.value;
				write_text_to_log_file(ss.str());
			}

			// bind the value
			sqlite3_bind_text(stmt, 1, setting.name.c_str(), setting.name.length(), 0);
			sqlite3_bind_text(stmt, 2, setting.value.c_str(), setting.value.length(), 0);

			// commit
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
		else
		{
			write_text_to_log_file("Failed to prepare setting row");
			write_text_to_log_file(ss.str());
			write_text_to_log_file(sqlite3_errmsg(db));
			break;
		}
	}

	for each (StringPairSettingDBRow row in values)
	{
		if (genericSettingsCache.find(row.name) == genericSettingsCache.end())
		{
			std::pair<std::string, std::string> pair(row.name, row.value);
			genericSettingsCache.insert(pair);
		}
		else
		{
			genericSettingsCache[row.name] = row.value;
		}
	}

	end_transaction();
	mutex_unlock();

	write_text_to_log_file("Done storing generic pairs");
}

std::vector<StringPairSettingDBRow> ENTDatabase::load_setting_pairs()
{
	mutex_lock();

	write_text_to_log_file("Asked to load generic pairs");
	std::vector<StringPairSettingDBRow> dbPairs;
	char* QUERY = "select SETTING_NAME, SETTING_VALUE from ENT_SETTING_PAIRS";
	int rc = sqlite3_exec(db, QUERY, genericSettingPairsFetchCallback, &dbPairs, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("Pairs not loaded");
		write_text_to_log_file(zErrMsg);
		sqlite3_free(zErrMsg);
	}

	for each (StringPairSettingDBRow row in dbPairs)
	{
		if (genericSettingsCache.find(row.name) == genericSettingsCache.end())
		{
			std::pair<std::string, std::string> pair(row.name, row.value);
			genericSettingsCache.insert(pair);
		}
		else
		{
			genericSettingsCache[row.name] = row.value;
		}
	}

	mutex_unlock();

	write_text_to_log_file("Done loading generic pairs");
	return dbPairs;
}

void ENTDatabase::save_vehicle_extras(Vehicle veh, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 1; i < 10; i++)
	{
		if (!VEHICLE::DOES_EXTRA_EXIST(veh, i))
		{
			continue;
		}

		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_VEHICLE_EXTRAS VALUES (?, ?, ?, ?)";

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Vehicle extras save failed");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i);
			sqlite3_bind_int(stmt, index++, VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(veh, i) ? 1 : 0);

			// commit
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}
	end_transaction();

	mutex_unlock();
}

void ENTDatabase::save_vehicle_mods(Vehicle veh, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 49; i++) //49 mods total (that is including Bennys mods)
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_VEHICLE_MODS VALUES (?, ?, ?, ?, ?)";

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Vehicle mods save failed");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			bool isToggleable = (i >= 17 && i <= 22);

			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i);

			if (isToggleable)
			{
				sqlite3_bind_int(stmt, index++, VEHICLE::IS_TOGGLE_MOD_ON(veh, i) ? 1 : 0);
			}
			else
			{
				sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_MOD(veh, i));
			}
			sqlite3_bind_int(stmt, index++, isToggleable ? 1 : 0);
			// commit
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

void ENTDatabase::save_skin_components(Ped ped, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 12; i++)
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_SKIN_COMPONENTS VALUES (?, ?, ?, ?, ?)";

		int drawable = PED::GET_PED_DRAWABLE_VARIATION(ped, i);
		int texture = PED::GET_PED_TEXTURE_VARIATION(ped, i);

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Skin components save failed");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i); //slot id
			sqlite3_bind_int(stmt, index++, drawable); //drawable id
			sqlite3_bind_int(stmt, index++, texture); //texture id

			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

void ENTDatabase::save_skin_props(Ped ped, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 10; i++)
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_SKIN_PROPS VALUES (?, ?, ?, ?, ?)";

		int drawable = PED::GET_PED_PROP_INDEX(ped, i);
		int texture = PED::GET_PED_PROP_TEXTURE_INDEX(ped, i);

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Skin props save failed");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i); //slot id
			sqlite3_bind_int(stmt, index++, drawable); //drawable id
			sqlite3_bind_int(stmt, index++, texture); //texture id

			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

bool ENTDatabase::save_skin(Ped ped, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();

	std::stringstream ss;
	ss << "INSERT OR REPLACE INTO ENT_SAVED_SKINS VALUES (?, ?, ?);";

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto ssStr = ss.str();
	int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);
	bool result = true;

	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("Vehicle save failed");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}

	int index = 1;
	if (slot == -1)
	{
		sqlite3_bind_null(stmt, index++);
	}
	else
	{
		sqlite3_bind_int64(stmt, index++, slot);
	}
	sqlite3_bind_text(stmt, index++, saveName.c_str(), saveName.length(), 0); //save name
	sqlite3_bind_int(stmt, index++, ENTITY::GET_ENTITY_MODEL(ped)); //model

	// commit
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	sqlite3_int64 newRowID = sqlite3_last_insert_rowid(db);

	//if we're updating, delete any pre-existing children
	if (slot != -1)
	{
		delete_saved_skin_children(slot);
	}

	save_skin_components(ped, newRowID);
	save_skin_props(ped, newRowID);

	mutex_unlock();

	return result;
}

bool ENTDatabase::save_vehicle(Vehicle veh, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();

	std::stringstream ss;
	ss << "INSERT OR REPLACE INTO ENT_SAVED_VEHICLES VALUES (";
	for (int i = 0; i < 40; i++)
	{
		if (i > 0)
		{
			ss << ", ";
		}
		ss << "?";
	}
	ss << ");"; 

	/*
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \ 1
			saveName TEXT NOT NULL, \ 2
			model INTEGER NOT NULL, \ 3
			colourPrimary INTEGER, \ 4
			colourSecondary INTEGER, \ 5
			colourExtraPearl INTEGER, \ 6
			colourExtraWheel INTEGER, \ 7
			colourMod1Type INTEGER, \ 8
			colourMod1Colour INTEGER, \ 9
			colourMod1P3 INTEGER, \ 10
			colourMod2Type INTEGER, \ 11
			colourMod2Colour INTEGER, \ 12
			colourCustom1R INTEGER, \ 13
			colourCustom1G INTEGER, \ 14
			colourCustom1B INTEGER, \ 15
			colourCustom2R INTEGER, \ 16
			colourCustom2G INTEGER, \ 17
			colourCustom2B INTEGER, \ 18

	*/

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto ssStr = ss.str();
	int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);
	bool result = true;

	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("Vehicle save failed");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}
	else
	{
		int index = 1;
		if (slot == -1)
		{
			sqlite3_bind_null(stmt, index++);
		}
		else
		{
			sqlite3_bind_int64(stmt, index++, slot);
		}
		sqlite3_bind_text(stmt, index++, saveName.c_str(), saveName.length(), 0); //save name
		sqlite3_bind_int(stmt, index++, ENTITY::GET_ENTITY_MODEL(veh)); //model

		int primaryCol, secondaryCol;
		VEHICLE::GET_VEHICLE_COLOURS(veh, &primaryCol, &secondaryCol);
		sqlite3_bind_int(stmt, index++, primaryCol);
		sqlite3_bind_int(stmt, index++, secondaryCol);

		int pearlCol, wheelCol;
		VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearlCol, &wheelCol);
		sqlite3_bind_int(stmt, index++, pearlCol);
		sqlite3_bind_int(stmt, index++, wheelCol);

		int mod1a, mod1b, mod1c;
		VEHICLE::GET_VEHICLE_MOD_COLOR_1(veh, &mod1a, &mod1b, &mod1c);
		sqlite3_bind_int(stmt, index++, mod1a);
		sqlite3_bind_int(stmt, index++, mod1b);
		sqlite3_bind_int(stmt, index++, mod1c);

		int mod2a, mod2b;
		VEHICLE::GET_VEHICLE_MOD_COLOR_2(veh, &mod2a, &mod2b);
		sqlite3_bind_int(stmt, index++, mod2a);
		sqlite3_bind_int(stmt, index++, mod2b);

		if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh))
		{
			int custR1, custG1, custB1;
			VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &custR1, &custG1, &custB1);
			sqlite3_bind_int(stmt, index++, custR1);
			sqlite3_bind_int(stmt, index++, custG1);
			sqlite3_bind_int(stmt, index++, custB1);
		}
		else
		{
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
		}

		if (VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh))
		{
			int custR2, custG2, custB2;
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &custR2, &custG2, &custB2);
			sqlite3_bind_int(stmt, index++, custR2);
			sqlite3_bind_int(stmt, index++, custG2);
			sqlite3_bind_int(stmt, index++, custB2);
		}
		else
		{
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
		}

		/*
		livery INTEGER, \ 19
		plateText TEXT, \ 20
		plateType INTEGER, \ 21
		wheelType INTEGER, \ 22
		windowTint INTEGER, \ 23
		burstableTyres INTEGER \ 24
		*/

		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_LIVERY(veh));

		char* plateText = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
		sqlite3_bind_text(stmt, index++, plateText, strlen(plateText), 0);

		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_WINDOW_TINT(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(veh) ? 1 : 0);
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_MOD_VARIATION(veh,23) ? 1 : 0);

		/*
		dirtLevel REAL DEFAULT 0, \ 25
			fadeLevel REAL DEFAULT 0, \ 26
			neonR INTEGER DEFAULT - 1, \ 27
			neonG INTEGER DEFAULT - 1, \ 28
			neonB INTEGER DEFAULT - 1, \ 29
			neon0Enabled INTEGER DEFAULT 0, \ 30
			neon1Enabled INTEGER DEFAULT 0, \ 21
			neon2Enabled INTEGER DEFAULT 0, \ 32
			neon3Enabled INTEGER DEFAULT 0 \*/

		sqlite3_bind_double(stmt, index++, VEHICLE::GET_VEHICLE_DIRT_LEVEL(veh));
		sqlite3_bind_double(stmt, index++, VEHICLE::_GET_VEHICLE_PAINT_FADE(veh));

		int neonR, neonG, neonB;
		VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &neonR, &neonG, &neonB);
		sqlite3_bind_int(stmt, index++, neonR);
		sqlite3_bind_int(stmt, index++, neonG);
		sqlite3_bind_int(stmt, index++, neonB);

		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 0));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 1));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 2));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 3));

		int tyreSmokeR, tyreSmokeG, tyreSmokeB;
		VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &tyreSmokeR, &tyreSmokeG, &tyreSmokeB);
		sqlite3_bind_int(stmt, index++, tyreSmokeR);
		sqlite3_bind_int(stmt, index++, tyreSmokeG);
		sqlite3_bind_int(stmt, index++, tyreSmokeB);

		sqlite3_bind_int(stmt, index++, VEHICLE::IS_VEHICLE_A_CONVERTIBLE(veh, 0) && VEHICLE::GET_CONVERTIBLE_ROOF_STATE(veh));

		/*dashColour INTEGER,
			interiorColour INTEGER*/
		int dashCol, interiorCol;
		VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &dashCol);
		VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &interiorCol);
		sqlite3_bind_int(stmt, index++, dashCol);
		sqlite3_bind_int(stmt, index++, interiorCol);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		sqlite3_int64 newRowID = sqlite3_last_insert_rowid(db);

		//if we're updating, delete any pre-existing children
		if (slot != -1)
		{
			delete_saved_vehicle_children(slot);
		}

		save_vehicle_extras(veh, newRowID);
		save_vehicle_mods(veh, newRowID);
	}

	mutex_unlock();

	return result;
}

std::vector<SavedSkinDBRow*> ENTDatabase::get_saved_skins(int index)
{
	write_text_to_log_file("Asked to load saved skins");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "select * from ENT_SAVED_SKINS";
	if (index != -1)
	{
		ss << " WHERE id = ?";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedSkinDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// bind the value
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("Skin row found");

			SavedSkinDBRow *skin = new SavedSkinDBRow();

			int index = 0;
			skin->rowID = sqlite3_column_int(stmt, index++);
			skin->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			skin->model = sqlite3_column_int(stmt, index++);
			
			results.push_back(skin);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to fetch saved skins");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

std::vector<SavedVehicleDBRow*> ENTDatabase::get_saved_vehicles(int index)
{
	write_text_to_log_file("Asked to load saved vehicles");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "select * from ENT_SAVED_VEHICLES";
	if (index != -1)
	{
		ss << " WHERE id = ?";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedVehicleDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// bind the value
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("Vehicle row found");

			SavedVehicleDBRow *veh = new SavedVehicleDBRow();

			int index = 0;
			veh->rowID = sqlite3_column_int(stmt, index++);
			veh->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			veh->model = sqlite3_column_int(stmt, index++);
			veh->colourPrimary = sqlite3_column_int(stmt, index++);
			veh->colourSecondary = sqlite3_column_int(stmt, index++);
			veh->colourExtraPearl = sqlite3_column_int(stmt, index++);
			veh->colourExtraWheel = sqlite3_column_int(stmt, index++);
			veh->colourMod1Type = sqlite3_column_int(stmt, index++);
			veh->colourMod1Colour = sqlite3_column_int(stmt, index++);
			veh->colourMod1P3 = sqlite3_column_int(stmt, index++);
			veh->colourMod2Type = sqlite3_column_int(stmt, index++);
			veh->colourMod2Colour = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[0] = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[1] = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[2] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[0] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[1] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[2] = sqlite3_column_int(stmt, index++);

			veh->livery = sqlite3_column_int(stmt, index++);
			veh->plateText = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			veh->plateType = sqlite3_column_int(stmt, index++);
			veh->wheelType = sqlite3_column_int(stmt, index++);
			veh->windowTint = sqlite3_column_int(stmt, index++);
			veh->burstableTyres = sqlite3_column_int(stmt, index++) == 1 ? true : false;
			veh->customTyres = sqlite3_column_int(stmt, index++) == 1 ? true : false;

			veh->dirtLevel = sqlite3_column_double(stmt, index++);
			veh->fadeLevel = sqlite3_column_double(stmt, index++);

			veh->neonRGB[0] = sqlite3_column_int(stmt, index++);
			veh->neonRGB[1] = sqlite3_column_int(stmt, index++);
			veh->neonRGB[2] = sqlite3_column_int(stmt, index++);

			veh->neonEnablement[0] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[1] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[2] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[3] = sqlite3_column_int(stmt, index++);

			veh->tyreSmokeRGB[0] = sqlite3_column_int(stmt, index++);
			veh->tyreSmokeRGB[1] = sqlite3_column_int(stmt, index++);
			veh->tyreSmokeRGB[2] = sqlite3_column_int(stmt, index++);

			veh->convertibleRoofUp = sqlite3_column_int(stmt, index++) == 1;

			veh->dashboardColour = sqlite3_column_int(stmt, index++);
			veh->interiorColour = sqlite3_column_int(stmt, index++);

			results.push_back(veh);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to fetch saved vehicles");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

void ENTDatabase::populate_saved_skin(SavedSkinDBRow *entry)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "select * from ENT_SKIN_COMPONENTS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int(stmt, 1, entry->rowID);

		// commit
		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			SavedSkinComponentDBRow *comp = new SavedSkinComponentDBRow();
			//0 and 1 are IDs
			comp->slotID = sqlite3_column_int(stmt, 2);
			comp->drawable = sqlite3_column_int(stmt, 3);
			comp->texture = sqlite3_column_int(stmt, 4);
			entry->components.push_back(comp);
			r = sqlite3_step(stmt);
		}

		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to fetch saved skin components");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "select * from ENT_SKIN_PROPS WHERE parentId=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int(stmt2, 1, entry->rowID);

		// commit
		int r = sqlite3_step(stmt2);
		while (r == SQLITE_ROW)
		{
			SavedSkinPropDBRow *prop = new SavedSkinPropDBRow();
			//0 and 1 are IDs
			prop->propID = sqlite3_column_int(stmt2, 2);
			prop->drawable = sqlite3_column_int(stmt2, 3);
			prop->texture = sqlite3_column_int(stmt2, 4);
			entry->props.push_back(prop);
			r = sqlite3_step(stmt2);
		}

		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("Failed to fetch saved skin props");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	write_text_to_log_file("Done loading saved skins");
	return;
}

void ENTDatabase::populate_saved_vehicle(SavedVehicleDBRow *entry)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "select * from ENT_VEHICLE_EXTRAS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int(stmt, 1, entry->rowID);

		// commit
		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			SavedVehicleExtraDBRow *extra = new SavedVehicleExtraDBRow();
			//0 and 1 are IDs
			extra->extraID = sqlite3_column_int(stmt, 2);
			extra->extraState = sqlite3_column_int(stmt, 3);
			entry->extras.push_back(extra);
			r = sqlite3_step(stmt);
		}

		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to fetch saved vehicle extras");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "select * from ENT_VEHICLE_MODS WHERE parentId=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int(stmt2, 1, entry->rowID);

		// commit
		int r = sqlite3_step(stmt2);
		while (r == SQLITE_ROW)
		{
			SavedVehicleModDBRow *mod = new SavedVehicleModDBRow();
			//0 and 1 are IDs
			mod->modID = sqlite3_column_int(stmt2, 2);
			mod->modState = sqlite3_column_int(stmt2, 3);
			mod->isToggle = (sqlite3_column_int(stmt2, 4) == 1) ? true : false;
			entry->mods.push_back(mod);
			r = sqlite3_step(stmt2);
		}

		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("Failed to fetch saved vehicle mods");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	write_text_to_log_file("Done loading saved vehicles");
	return;
}

void ENTDatabase::delete_saved_vehicle(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_SAVED_VEHICLES WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int64(stmt, 1, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to delete saved vehicle");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_skin(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_SAVED_SKINS WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int64(stmt, 1, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to delete saved skin");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::rename_saved_vehicle(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "UPDATE ENT_SAVED_VEHICLES SET saveName=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to rename saved vehicle");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::rename_saved_propset(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "UPDATE ENT_PROP_SETS SET name=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to rename saved vehicle");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::rename_saved_skin(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "UPDATE ENT_SAVED_SKINS SET saveName=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to rename saved skin");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_vehicle_children(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_VEHICLE_EXTRAS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int64(stmt, 1, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to delete saved vehicle extras");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "DELETE FROM ENT_VEHICLE_MODS WHERE id=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int64(stmt2, 1, slot);

		// commit
		sqlite3_step(stmt2);
		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("Failed to delete saved vehicle mods");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_skin_children(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_SKIN_COMPONENTS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int64(stmt, 1, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to delete saved skin components");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "DELETE FROM ENT_SKIN_PROPS WHERE id=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int64(stmt2, 1, slot);

		// commit
		sqlite3_step(stmt2);
		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("Failed to delete saved skin props");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::begin_transaction()
{
	if (has_transaction_begun == false)
	{
		has_transaction_begun = true;
		sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
	}
}
void ENTDatabase::end_transaction()
{
	if (has_transaction_begun == true)
	{
		has_transaction_begun = false;
		sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
	}
}

void ENTDatabase::mutex_lock()
{
	sqlite3_mutex_enter(db_mutex);
}

void ENTDatabase::mutex_unlock()
{
	sqlite3_mutex_leave(db_mutex);
}

void ENTDatabase::delete_saved_propset(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_PROP_SETS WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int64(stmt, 1, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to delete saved propset");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_propset_children(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_PROP_INSTANCES WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// bind the value
		sqlite3_bind_int64(stmt, 1, slot);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to delete saved prop instances");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

std::vector<SavedPropDBRow*> ENTDatabase::get_saved_prop_instances(int parentId)
{
	write_text_to_log_file("Asked to load saved prop instances");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "select * from ENT_PROP_INSTANCES";
	if (parentId != -1)
	{
		ss << " WHERE parentId = ? ";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedPropDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// bind the value
		if (parentId != -1)
		{
			sqlite3_bind_int(stmt, 1, parentId);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("Prop row found");

			SavedPropDBRow *prop = new SavedPropDBRow();

			int index = 0;
			prop->rowID = sqlite3_column_int(stmt, index++);
			prop->parentID = sqlite3_column_int(stmt, index++);
			prop->model = sqlite3_column_int(stmt, index++);
			prop->title = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			prop->counter = sqlite3_column_int(stmt, index++);

			prop->posX = sqlite3_column_double(stmt, index++);
			prop->posY = sqlite3_column_double(stmt, index++);
			prop->posZ = sqlite3_column_double(stmt, index++);
			prop->pitch = sqlite3_column_double(stmt, index++);
			prop->roll = sqlite3_column_double(stmt, index++);
			prop->yaw = sqlite3_column_double(stmt, index++);
			
			prop->isImmovable = sqlite3_column_int(stmt, index++);
			prop->isInvincible = sqlite3_column_int(stmt, index++);
			prop->hasGravity = sqlite3_column_int(stmt, index++);

			prop->alpha = sqlite3_column_double(stmt, index++);

			results.push_back(prop);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to fetch saved props");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

std::vector<SavedPropSet*> ENTDatabase::get_saved_prop_sets(int index)
{
	write_text_to_log_file("Asked to load saved prop sets");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "SELECT s.id, s.name, COUNT(i.id) AS size FROM ENT_PROP_SETS s LEFT JOIN ENT_PROP_INSTANCES i ON s.id = i.parentId ";
	if (index != -1)
	{
		ss << " WHERE s.id = ? ";
	}
	ss << " GROUP BY i.parentId ORDER BY s.id";

	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedPropSet*> results;

	if (rc == SQLITE_OK)
	{
		// bind the value
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("Prop set found");

			SavedPropSet *set = new SavedPropSet();

			int index = 0;
			set->rowID = sqlite3_column_int(stmt, index++);
			set->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			set->dbSize = sqlite3_column_int(stmt, index++);;

			results.push_back(set);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("Failed to fetch saved prop sets");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

void ENTDatabase::populate_saved_prop_set(SavedPropSet *entry)
{
	std::vector<SavedPropDBRow*> instances = get_saved_prop_instances(entry->rowID);
	entry->items = instances;
}

bool ENTDatabase::save_props(std::vector<SavedPropDBRow*> props, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();
	begin_transaction();

	std::stringstream ss_set;
	ss_set << "INSERT OR REPLACE INTO ENT_PROP_SETS VALUES (?, ?);";
	sqlite3_stmt *set_stmt;
	const char *pzTest;
	auto ss_set_str = ss_set.str();
	int set_rc = sqlite3_prepare_v2(db, ss_set_str.c_str(), ss_set_str.length(), &set_stmt, &pzTest);

	bool result = true;
	sqlite3_int64 newRowID;

	if (set_rc != SQLITE_OK)
	{
		write_text_to_log_file("Prop set save failed");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}
	else
	{
		int index = 1;
		if (slot == -1)
		{
			sqlite3_bind_null(set_stmt, index++);
		}
		else
		{
			sqlite3_bind_int64(set_stmt, index++, slot);
		}

		sqlite3_bind_text(set_stmt, index++, saveName.c_str(), saveName.length(), 0); //save name
		
		// commit
		sqlite3_step(set_stmt);
		sqlite3_finalize(set_stmt);

		newRowID = sqlite3_last_insert_rowid(db);

		//if we're updating, delete any pre-existing children
		if (slot != -1)
		{
			delete_saved_propset_children(slot);;
		}
		else
		{
			slot = newRowID; //get new ID to use for parent of children
		}
	}

	if (result)
	{
		for each (SavedPropDBRow* prop in props)
		{
			std::stringstream ss_inst;
			ss_inst << "INSERT OR REPLACE INTO ENT_PROP_INSTANCES VALUES (";
			for (int i = 0; i < 15; i++)
			{
				if (i > 0)
				{
					ss_inst << ", ";
				}
				ss_inst << "?";
			}
			ss_inst << ");";

			sqlite3_stmt *inst_stmt;
			const char *pzTest;
			auto ss_instance_str = ss_inst.str();
			int inst_rc = sqlite3_prepare_v2(db, ss_instance_str.c_str(), ss_instance_str.length(), &inst_stmt, &pzTest);;

			if (inst_rc != SQLITE_OK)
			{
				write_text_to_log_file("Prop instance save failed");
				write_text_to_log_file(sqlite3_errmsg(db));
				result = false;
			}
			else
			{
				int index = 1;

				sqlite3_bind_null(inst_stmt, index++);
				sqlite3_bind_int64(inst_stmt, index++, slot); //parent id
				sqlite3_bind_int(inst_stmt, index++, prop->model);
				auto propTitle = prop->title;
				sqlite3_bind_text(inst_stmt, index++, (char*)propTitle.c_str(), propTitle.length(), 0);
				sqlite3_bind_int(inst_stmt, index++, prop->counter);
				sqlite3_bind_double(inst_stmt, index++, prop->posX);
				sqlite3_bind_double(inst_stmt, index++, prop->posY);
				sqlite3_bind_double(inst_stmt, index++, prop->posZ);
				sqlite3_bind_double(inst_stmt, index++, prop->pitch);
				sqlite3_bind_double(inst_stmt, index++, prop->roll);
				sqlite3_bind_double(inst_stmt, index++, prop->yaw);
				sqlite3_bind_int(inst_stmt, index++, prop->isImmovable);
				sqlite3_bind_int(inst_stmt, index++, prop->isInvincible);
				sqlite3_bind_int(inst_stmt, index++, prop->hasGravity);
				sqlite3_bind_int(inst_stmt, index++, prop->alpha);

				sqlite3_step(inst_stmt);
				
				int finalOK = sqlite3_finalize(inst_stmt);
				if (finalOK != SQLITE_OK)
				{
					write_text_to_log_file("Prop instance save failed (finalise)");
					write_text_to_log_file(sqlite3_errmsg(db));
					result = false;
				}

				write_text_to_log_file("Prop instance save OK");
			}
		}	
	}

	end_transaction();

	mutex_unlock();

	return result;

	return false;
}