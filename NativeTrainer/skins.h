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