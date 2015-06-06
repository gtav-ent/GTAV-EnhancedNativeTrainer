/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "database.h"
#include "debuglog.h"

#include <string>
#include <sstream>

/**This value should be increased whenever you change the schema and a release is made.
However you must also put in code to upgrade from older versions, in ENTDatabase::handle_version,
as they will be deployed in the wild already.*/
const int DATABASE_VERSION = 2;

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
		int rc = sqlite3_exec(db, CREATE_VERSION_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rc != SQLITE_OK)
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
		int rc = sqlite3_exec(db, CREATE_VERSION_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Generic setting pairs table creation problem");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("Generic setting pairs table created");
		}
	}
}

bool ENTDatabase::open()
{
	std::stringstream ss;

	write_text_to_log_file("Initialising DB engine");

	sqlite3_initialize();

	write_text_to_log_file("Opening DB file");

	int rc = sqlite3_open_v2("ent.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
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
		return false;
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

	return true;
}

void ENTDatabase::close()
{
	write_text_to_log_file("DB closing");
	if (db != NULL)
	{
		sqlite3_close_v2(db);
		db = NULL;
	}
	write_text_to_log_file("DB closed");
	sqlite3_shutdown();
	write_text_to_log_file("DB shutdown");
}

void ENTDatabase::store_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values)
{
	write_text_to_log_file("Asked to store feature pairs");
	for (int i = 0; i < values.size(); i++)
	{
		FeatureEnabledLocalDefinition def = values.at(i);
		std::stringstream ss;
		ss << "REPLACE INTO ENT_FEATURE_ENABLEMENT VALUES ('" << def.name << "', " << (*def.enabled ? 1 : 0) << ")";
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
		else
		{
			write_text_to_log_file("Done storing feature pairs");
		}
	}
}

void ENTDatabase::load_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values)
{
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
		return;
	}
}

void ENTDatabase::store_setting_pairs(std::vector<StringPairSettingDBRow> values)
{
	write_text_to_log_file("Asked to store generic pairs");
	for (int i = 0; i < values.size(); i++)
	{
		StringPairSettingDBRow setting = values.at(i);
		std::stringstream ss;
		ss << "REPLACE INTO ENT_SETTING_PAIRS VALUES (?, ?);";

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc == SQLITE_OK)
		{
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
			write_text_to_log_file(zErrMsg);
			sqlite3_free(zErrMsg);
			break;
		}
	}
	write_text_to_log_file("Done storing generic pairs");
}

std::vector<StringPairSettingDBRow> ENTDatabase::load_setting_pairs()
{
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
	write_text_to_log_file("Done loading generic pairs");
	return dbPairs;
}