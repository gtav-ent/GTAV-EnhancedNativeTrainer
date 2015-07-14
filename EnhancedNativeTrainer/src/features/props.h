/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <string>
#include <vector>

#include "../ui_support/menu_functions.h"

#pragma once

struct PropInfo
{
	char* model;
	std::string label;
	std::string category;
};

bool onconfirm_prop_selection(MenuItem<std::string> choice);

void process_props_menu();