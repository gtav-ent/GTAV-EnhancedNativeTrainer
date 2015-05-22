/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "menu_functions.h"
#include "airbrake.h"
#include "keyboard.h"
#include "config_io.h"

bool exitFlag = false;

char* AIRBRAKE_ANIM_A = "amb@world_human_stand_impatient@male@no_sign@base";
char* AIRBRAKE_ANIM_B = "base";

int travelSpeed = 0;

//Converts Radians to Degrees
float degToRad(float degs)
{
	return degs*3.141592653589793 / 180;
}

std::string airbrakeStatusLines[6];

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

	std::string caption = "Airbrake";

	//draw_menu_header_line(caption,350.0f,50.0f,15.0f,0.0f,15.0f,false);

	DWORD waitTime = 150;

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	bool inVehicle = PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) ? true : false;

	if (!inVehicle)
	{
		STREAMING::REQUEST_ANIM_DICT(AIRBRAKE_ANIM_A);
		while (!STREAMING::HAS_ANIM_DICT_LOADED(AIRBRAKE_ANIM_A))
		{
			WAIT(0);
		}
		loadedAnims = true;
	}

	while (true && !exitFlag)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_header_line(caption, 350.0f, 50.0f, 15.0f, 0.0f, 15.0f, false);
			//draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);

			make_periodic_feature_call();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		airbrake(inVehicle);

		//// process buttons
		//bool bSelect, bBack, bUp, bDown;
		//get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (airbrake_switch_pressed())
		{
			menu_beep();
			break;
		}
	}

	if (!inVehicle)
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
	}

	exitFlag = false;
}

void update_airbrake_text()
{
	if (GetTickCount() < airbrakeStatusTextDrawTicksMax)
	{
		int numLines = sizeof(airbrakeStatusLines) / sizeof(airbrakeStatusLines[0]);

		float textY = 0.1f;

		for (int i = 0; i < numLines; i++)
		{
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.3, 0.3);
			UI::SET_TEXT_COLOUR(255, 255, 255, 255);
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

	ss << "Current Travel Speed: " << travelSpeedStr;

	airbrakeStatusLines[0] = "Default Airbrake Keys (may be changed in config):";
	airbrakeStatusLines[1] = "Q/Z - Move Up/Down";
	airbrakeStatusLines[2] = "A/D - Rotate Left/Right";
	airbrakeStatusLines[3] = "W/S - Move Forward/Back";
	airbrakeStatusLines[4] = "Shift - Toggle Move Speed";
	airbrakeStatusLines[5] = ss.str();

	airbrakeStatusTextDrawTicksMax = GetTickCount() + 2500;
	airbrakeStatusTextGxtEntry = false;
}

void moveThroughDoor()
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	float curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	float forwardPush = 0.6;

	int sector = (int) curHeading / 90;
	float angle = fmod(curHeading, 90);

	float xVect = ((forwardPush / sin(degToRad(90)))*sin(degToRad(angle)));
	float yVect = ((forwardPush / sin(degToRad(90)))*sin(degToRad(180 - (angle + 90))));

	switch (sector)
	{
	case 0: //0-90‹
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y + yVect, curLocation.z, 0, 0, 1);
		break;
	case 1: //90 - 180‹
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y - yVect, curLocation.z, 0, 0, 1);
		break;
	case 2: //180 - 270‹
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y - yVect, curLocation.z, 0, 0, 1);
		break;
	case 3: //270 - 360‹
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y + yVect, curLocation.z, 0, 0, 1);
		break;
	default:
		break;
	}
}

bool lshiftWasDown = false;

void airbrake(bool inVehicle)
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	Vector3 curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 curRotation = ENTITY::GET_ENTITY_ROTATION(playerPed, 0);
	float curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	//float tmpHeading = curHeading += ;

	float rotationSpeed = 2.5;
	float forwardPush;

	switch (travelSpeed)
	{
	case 0:
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

	bool moveUpKey =	get_key_pressed(keyConfig->key_airbrake_up);
	bool moveDownKey = get_key_pressed(keyConfig->key_airbrake_down);
	bool moveForwardKey = get_key_pressed(keyConfig->key_airbrake_forward);
	bool moveBackKey = get_key_pressed(keyConfig->key_airbrake_back);
	bool rotateLeftKey = get_key_pressed(keyConfig->key_airbrake_rotate_left);
	bool rotateRightKey = get_key_pressed(keyConfig->key_airbrake_rotate_right);

	//Airbrake controls vehicle if occupied
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		playerPed = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	}

	BOOL xBoolParam = 1;
	BOOL yBoolParam = 1;
	BOOL zBoolParam = 1;

	ENTITY::SET_ENTITY_VELOCITY(playerPed, 0, 0, 0);
	ENTITY::SET_ENTITY_ROTATION(playerPed, 0, 0, 0, 0, false);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x, curLocation.y, curLocation.z, xBoolParam, yBoolParam, zBoolParam);
	ENTITY::SET_ENTITY_HEADING(playerPed, curHeading);

	if (!inVehicle)
	{
		AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), AIRBRAKE_ANIM_A, AIRBRAKE_ANIM_B, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
	}

	if (IsKeyJustUp(keyConfig->key_airbrake_speed))
	{
		travelSpeed++;
		if (travelSpeed > 2)
		{
			travelSpeed = 0;
		}
	}

	create_airbrake_help_text();
	update_airbrake_text();

	if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x, curLocation.y, curLocation.z + forwardPush / 2, xBoolParam, yBoolParam, zBoolParam); }
	if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x, curLocation.y, curLocation.z - forwardPush / 2, xBoolParam, yBoolParam, zBoolParam); }
	if (rotateLeftKey)
	{
		ENTITY::SET_ENTITY_HEADING(playerPed, curHeading + rotationSpeed);
	}
	else if (rotateRightKey)
	{
		ENTITY::SET_ENTITY_HEADING(playerPed, curHeading - rotationSpeed);
	}

	/*
	std::stringstream ss2;
	ss2 << "Angle: " << curHeading << "\nXV: " << xVect << "\nYV: " << yVect << "\nCRZ: " << curRotation.z;
	set_status_text(ss2.str());
	*/

	if (moveForwardKey)
	{
		if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y + yVect, curLocation.z + forwardPush / 2, xBoolParam, yBoolParam, zBoolParam); }
		else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y + yVect, curLocation.z - forwardPush / 2, xBoolParam, yBoolParam, zBoolParam); }
		else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y + yVect, curLocation.z, xBoolParam, yBoolParam, zBoolParam); }
	}
	else if (moveBackKey)
	{
		if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y - yVect, curLocation.z + forwardPush / 2, xBoolParam, yBoolParam, zBoolParam); }
		else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y - yVect, curLocation.z - forwardPush / 2, xBoolParam, yBoolParam, zBoolParam); }
		else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y - yVect, curLocation.z, xBoolParam, yBoolParam, zBoolParam); }
	}
}