
/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <fstream>
#include <sstream>

#include "debuglog.h"

void clear_log_file()
{
	remove("ent-log.txt");
}

void write_text_to_log_file(const std::string &text)
{
	if (!DEBUG_LOG_ENABLED)
	{
		return;
	}
	std::ofstream log_file("ent-log.txt", std::ios_base::out | std::ios_base::app);
	log_file << text << std::endl;
	log_file.close();
}