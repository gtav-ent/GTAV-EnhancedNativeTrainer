/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

#include <string>

void moveThroughDoor();

void airbrake();

void set_airbrake_text(std::string str, DWORD time = 2500, bool isGxtEntry = false);

void update_airbrake_text();

void airbrake_flip_angle();
