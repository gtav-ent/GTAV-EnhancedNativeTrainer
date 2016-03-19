/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "..\ui_support\menu_functions.h"
#include "airbrake.h"
#include "..\io\keyboard.h"
#include "..\io\config_io.h"
#include "..\utils.h"
#include "script.h"

bool exitFlag = false;

char* AIRBRAKE_ANIM_A = "amb@world_human_stand_impatient@male@no_sign@base";
char* AIRBRAKE_ANIM_B = "base";

int travelSpeed = 0;

bool in_airbrake_mode = false;

bool frozen_time = false;
bool help_showing = true;

Vector3 curLocation;
Vector3 curRotation;
float curHeading;

std::string airbrakeStatusLines[15];

DWORD airbrakeStatusTextDrawTicksMax;
bool airbrakeStatusTextGxtEntry;

void exit_airbrake_menu_if_showing()
{
	exitFlag = true;
}

//Test for airbrake command.
void process_airbrake_menu()
{
	exitFlag = false;

	const float lineWidth = 250.0;
	const int lineCount = 1;
	bool loadedAnims = false;

	std::string caption = "Airbrake Mode";

	//draw_menu_header_line(caption,350.0f,50.0f,15.0f,0.0f,15.0f,false);

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	bool inVehicle = PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) ? true : false;

	frozen_time = false;
	if (!inVehicle)
	{
		STREAMING::REQUEST_ANIM_DICT(AIRBRAKE_ANIM_A);
		while (!STREAMING::HAS_ANIM_DICT_LOADED(AIRBRAKE_ANIM_A))
		{
			make_periodic_feature_call();
			WAIT(0);
		}
		loadedAnims = true;
	}

	curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	curRotation = ENTITY::GET_ENTITY_ROTATION(playerPed, 0);
	curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);

	while (true && !exitFlag)
	{
		in_airbrake_mode = true;

		// draw menu
		draw_menu_header_line(caption, 350.0f, 50.0f, 15.0f, 0.0f, 15.0f, false);
		//draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);

		make_periodic_feature_call();

		//Disable airbrake on death
		if (ENTITY::IS_ENTITY_DEAD(playerPed))
		{
			exitFlag = true;
		}
		else if (airbrake_switch_pressed())
		{
			menu_beep();
			break;
		}

		airbrake(inVehicle);

		WAIT(0);
	}

	if (!inVehicle)
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
	}

	exitFlag = false;
	in_airbrake_mode = false;
}

void update_airbrake_text()
{
	if (GetTickCount() < airbrakeStatusTextDrawTicksMax)
	{
		int numLines = sizeof(airbrakeStatusLines) / sizeof(airbrakeStatusLines[0]);

		float textY = 0.1f;

		int numActualLines = 0;
		for (int i = 0; i < numLines; i++)
		{
			if (!help_showing && i != 14)
			{
				continue;
			}

			numActualLines++;

			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.3, 0.3);
			if (i == 0 || i == 8 || i==14)
			{
				UI::SET_TEXT_OUTLINE();
				UI::SET_TEXT_COLOUR(255, 180, 0, 255);
			}
			else
			{
				UI::SET_TEXT_COLOUR(255, 255, 255, 255);
			}
			UI::SET_TEXT_WRAP(0.0, 1.0);
			UI::SET_TEXT_DROPSHADOW(1, 1, 1, 1, 1);
			UI::SET_TEXT_EDGE(1, 0, 0, 0, 305);
			if (airbrakeStatusTextGxtEntry)
			{
				UI::_SET_TEXT_ENTRY((char *)airbrakeStatusLines[i].c_str());
			}
			else
			{
				UI::_SET_TEXT_ENTRY("STRING");
				UI::_ADD_TEXT_COMPONENT_STRING((char *)airbrakeStatusLines[i].c_str());
			}
			UI::_DRAW_TEXT(0.01, textY);

			textY += 0.025f;
		}

		int screen_w, screen_h;
		GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);
		float rectWidthScaled = 350 / (float)screen_w;
		float rectHeightScaled = (20 + (numActualLines * 18)) / (float)screen_h;
		float rectXScaled = 0 / (float)screen_h;
		float rectYScaled = 65 / (float)screen_h;

		int rect_col[4] = { 128, 128, 128, 75.0f };

		// rect
		draw_rect(rectXScaled, rectYScaled,
			rectWidthScaled, rectHeightScaled,
			rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
	}
}

void create_airbrake_help_text()
{
	//Debug
	std::stringstream ss;

	/*ss << "Heading: " << curHeading << " Rotation: " << curRotation.z
	<< "\n xVect: " << xVect << "yVect: " << yVect;*/

	std::string travelSpeedStr;
	switch (travelSpeed)
	{
	case 0:
		travelSpeedStr = "Slow";
		break;
	case 1:
		travelSpeedStr = "Fast";
		break;
	case 2:
		travelSpeedStr = "Very Fast";
		break;
	}

	ss << "Current Travel Speed: ~HUD_COLOUR_WHITE~" << travelSpeedStr;

	int index = 0;
	airbrakeStatusLines[index++] = "Default Airbrake Keys (change in XML):";
	airbrakeStatusLines[index++] = "Q/Z - Move Up/Down";
	airbrakeStatusLines[index++] = "A/D - Rotate Left/Right";
	airbrakeStatusLines[index++] = "W/S - Move Forward/Back";
	airbrakeStatusLines[index++] = "Shift - Toggle Move Speed";
	airbrakeStatusLines[index++] = "T - Toggle Frozen Time";
	airbrakeStatusLines[index++] = "H - Toggle This Help";
	airbrakeStatusLines[index++] = " ";
	airbrakeStatusLines[index++] = "Default Controller Input (change in XML):";
	airbrakeStatusLines[index++] = "Triggers - Move Up/Down";
	airbrakeStatusLines[index++] = "Left Stick - Rotate, Move Forward/Back";
	airbrakeStatusLines[index++] = "A - Toggle Move Speed";
	airbrakeStatusLines[index++] = "B - Toggle Frozen Time";
	airbrakeStatusLines[index++] = " ";
	airbrakeStatusLines[index++] = ss.str();

	airbrakeStatusTextDrawTicksMax = GetTickCount() + 2500;
	airbrakeStatusTextGxtEntry = false;
}

void moveThroughDoor()
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		return;
	}

	curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);

	float forwardPush = 0.6;

	float xVect = forwardPush * sin(degToRad(curHeading)) * -1.0f;
	float yVect = forwardPush * cos(degToRad(curHeading));

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y + yVect, curLocation.z, 1, 1, 1);
}

bool lshiftWasDown = false;

void airbrake(bool inVehicle)
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	//float tmpHeading = curHeading += ;

	float rotationSpeed = 2.5;
	float forwardPush;

	switch (travelSpeed)
	{
	case 0:
		rotationSpeed = 0.8f;
		forwardPush = 0.2f;
		break;
	case 1:
		forwardPush = 1.8f;
		break;
	case 2:
		forwardPush = 3.6f;
		break;
	}

	float xVect = forwardPush * sin(degToRad(curHeading)) * -1.0f;
	float yVect = forwardPush * cos(degToRad(curHeading));

	KeyInputConfig* keyConfig = get_config()->get_key_config();

	bool moveUpKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_UP) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_UP);
	bool moveDownKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_DOWN) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_DOWN);
	bool moveForwardKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_FORWARD) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_FORWARD);
	bool moveBackKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_BACK) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_BACK);
	bool rotateLeftKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT);
	bool rotateRightKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT);

	//Airbrake controls vehicle if occupied
	Entity target = playerPed;
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		target = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	}

	BOOL xBoolParam = 1;
	BOOL yBoolParam = 1;
	BOOL zBoolParam = 1;

	ENTITY::SET_ENTITY_VELOCITY(target, 0.0f, 0.0f, 0.0f);
	ENTITY::SET_ENTITY_ROTATION(target, 0, 0, 0, 0, false);

	if (!inVehicle)
	{
		AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), AIRBRAKE_ANIM_A, AIRBRAKE_ANIM_B, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_SPEED) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_SPEED))
	{
		travelSpeed++;
		if (travelSpeed > 2)
		{
			travelSpeed = 0;
		}
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_FREEZE_TIME) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_FREEZE_TIME))
	{
		frozen_time = !frozen_time;
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_HELP) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_HELP))
	{
		help_showing = !help_showing;
	}

	create_airbrake_help_text();
	update_airbrake_text();

	if (moveUpKey)
	{
		curLocation.z += forwardPush / 2;
	}
	else if (moveDownKey)
	{
		curLocation.z -= forwardPush / 2;
	}

	if (moveForwardKey)
	{
		curLocation.x += xVect;
		curLocation.y += yVect;
	}
	else if (moveBackKey)
	{
		curLocation.x -= xVect;
		curLocation.y -= yVect;
	}

	if (rotateLeftKey)
	{
		curHeading += rotationSpeed;
	}
	else if (rotateRightKey)
	{
		curHeading -= rotationSpeed;
	}

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(target, curLocation.x, curLocation.y, curLocation.z, xBoolParam, yBoolParam, zBoolParam);
	ENTITY::SET_ENTITY_HEADING(target, curHeading - rotationSpeed);
}

bool is_in_airbrake_mode()
{
	return in_airbrake_mode;
}

bool is_airbrake_frozen_time()
{
	return frozen_time;
}