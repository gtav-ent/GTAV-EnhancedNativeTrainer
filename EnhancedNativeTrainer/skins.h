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

#include <string>

/***
* METHODS
*/

int findFirstValidPedTexture(int drawable);

int findFirstValidPedDrawable(int component);

bool applyChosenSkin(std::string skinName);

void reset_skin_globals();

bool is_custom_skin_applied();

void set_custom_skin_applied(bool applied);

bool process_skinchanger_menu();