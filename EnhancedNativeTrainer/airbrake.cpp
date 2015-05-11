/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "menu_functions.h"
#include "airbrake.h"
#include "keyboard.h"
#include "config_io.h"

bool travelSpeed = false;

//Converts Radians to Degrees
float degToRad(float degs)
{
	return degs*3.141592653589793 / 180;
}

std::string airbrakeStatusText;
DWORD airbrakeStatusTextDrawTicksMax;
bool airbrakeStatusTextGxtEntry;

void update_airbrake_text()
{
	if (GetTickCount() < airbrakeStatusTextDrawTicksMax)
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.3, 0.3);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_DROPSHADOW(1, 1, 1, 1, 1);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 305);
		if (airbrakeStatusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *) airbrakeStatusText.c_str());
		}
		else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *) airbrakeStatusText.c_str());
		}
		UI::_DRAW_TEXT(0.01, 0.1);
	}
}

void set_airbrake_text(std::string str, DWORD time, bool isGxtEntry)
{
	airbrakeStatusText = str;
	airbrakeStatusTextDrawTicksMax = GetTickCount() + time;
	airbrakeStatusTextGxtEntry = isGxtEntry;
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

void airbrake_flip_angle()
{
	/*Ped playerPed = PLAYER::PLAYER_PED_ID();
	float curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	curHeading += 180.0f;
	while (curHeading >= 360.0f)
	{
		curHeading -= 360.0f;
	}
	ENTITY::SET_ENTITY_HEADING(playerPed, curHeading);*/
}

void airbrake()
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	Vector3 curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 curRotation = ENTITY::GET_ENTITY_ROTATION(playerPed, 0);
	float curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	//float tmpHeading = curHeading += ;

	float forwardPush;
	if (travelSpeed)
	{
		forwardPush = 1.0f;
	}
	else
	{
		forwardPush = 0.1f;
	}
	float rotationSpeed = 2.5;

	float xVect = forwardPush * sin(degToRad(curHeading)) * -1.0f;
	float yVect = forwardPush * cos(degToRad(curHeading));

	KeyInputConfig* keyConfig = get_config()->get_key_config();

	bool moveUpKey		=	get_key_pressed(keyConfig->key_airbrake_up);
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

	BOOL xBoolParam = 0;
	BOOL yBoolParam = 0;
	BOOL zBoolParam = 1;

	ENTITY::SET_ENTITY_VELOCITY(playerPed, 0, 0, 0);
	ENTITY::SET_ENTITY_ROTATION(playerPed, 0, 0, 0, 0, false);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x, curLocation.y, curLocation.z, xBoolParam, yBoolParam, zBoolParam);
	ENTITY::SET_ENTITY_HEADING(playerPed, curHeading);

	if (IsKeyJustUp(keyConfig->key_airbrake_speed)){ travelSpeed = !travelSpeed; }

	//Debug
	std::stringstream ss;
	/*ss << "Heading: " << curHeading << " Rotation: " << curRotation.z
	<< "\n xVect: " << xVect << "yVect: " << yVect;*/
	std::string aa = ((travelSpeed) ? "On" : "Off");
	ss << "Default Airbrake Keys:\nQ/Z - Up/Down\nA/D - Rotate\nW/S - Forwards/Backwards"
		<< "\nShift - Move Speed" << "\nTravel Speed: " << aa;
	std::string s = ss.str();
	set_airbrake_text(s);
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

	std::stringstream ss2;
	ss2 << "Angle: " << curHeading << "\nXV: " << xVect << "\nYV: " << yVect << "\nCRZ: " << curRotation.z;
	set_status_text(ss2.str());

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