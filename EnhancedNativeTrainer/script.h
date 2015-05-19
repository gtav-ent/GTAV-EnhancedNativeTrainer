/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

#include <fstream>

void ScriptMain();

bool process_skinchanger_menu();

bool process_skinchanger_detail_menu();

void update_features();

/*
int getFrozenWantedLvl();
void setFrozenWantedLvl(int level);
void setFrozenWantedFeature(bool b);
*/

/**Append a line of text to the log file. Does nothing unless the debug
constant is set.*/
void write_text_to_log_file(const std::string &text);

/**Wipe the debug log file. Called every time the trainer starts.*/
void clear_log_file();

void turn_off_never_wanted();