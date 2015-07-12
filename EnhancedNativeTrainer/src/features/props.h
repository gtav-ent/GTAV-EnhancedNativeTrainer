/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include <string>
#include <vector>

#include "../ui_support/menu_functions.h"

struct PropInfo
{
	char* model;
	std::string label;
};

bool onconfirm_prop_selection(MenuItem<std::string> choice);

void process_props_menu();