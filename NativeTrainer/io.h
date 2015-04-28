#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

bool get_key_pressed(int nVirtKey);

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r);

bool trainer_switch_pressed();

void reset_trainer_switch();