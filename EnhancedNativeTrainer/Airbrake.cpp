/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "menu_functions.h"
#include "Airbrake.h"

bool travelSpeed = false;

//Converts Radians to Degrees
float radToDeg(float rads)
{
	return rads*3.141592653589793 / 180;
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
		UI::_DRAW_TEXT(0.01, 0.12);
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

	float xVect = ((forwardPush / sin(radToDeg(90)))*sin(radToDeg(angle)));
	float yVect = ((forwardPush / sin(radToDeg(90)))*sin(radToDeg(180 - (angle + 90))));

	switch (sector)
	{
	case 0: //0-90‹
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y + yVect, curLocation.z, 0, 0, 1);
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

void airbrake()
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	Vector3 curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 curRotation = ENTITY::GET_ENTITY_ROTATION(playerPed, 0);
	float curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	float forwardPush;
	if (travelSpeed){ forwardPush = 5; }
	else{ forwardPush = 0.3; }
	float rotationSpeed = 2.5;

	int sector = (int) curHeading / 90;
	float angle = fmod(curHeading, 90);

	float xVect = ((forwardPush / sin(radToDeg(90)))*sin(radToDeg(angle)));
	float yVect = ((forwardPush / sin(radToDeg(90)))*sin(radToDeg(180 - (angle + 90))));

	bool moveUpKey		=	get_key_pressed(0x57);
	bool moveDownKey	=	get_key_pressed(0x53);
	bool moveFowardKey	=	get_key_pressed(VK_NUMPAD8);
	bool moveBackKey	=	get_key_pressed(VK_NUMPAD5);
	bool rotateLeftKey	=	get_key_pressed(0x41);
	bool rotateRightKey = get_key_pressed(0x44);

	//Airbrake controls vehicle if occupied
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		playerPed = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	}

	ENTITY::SET_ENTITY_VELOCITY(playerPed, 0, 0, 0);

	if (IsKeyJustUp(VK_SHIFT)){ travelSpeed = !travelSpeed; }

	//Debug
	std::stringstream ss;
	/*ss << "Heading: " << curHeading << " Rotation: " << curRotation.z
	<< "\n xVect: " << xVect << "yVect: " << yVect;*/
	ss << "W/S - Up/Down\nA/D - Rotate\nNUM8/NUM5 - Forwards/Backwards"
		<< "\nShift - Movement Speed" << "\nTravel Speed On: " << travelSpeed;
	std::string s = ss.str();
	set_airbrake_text(s);
	update_airbrake_text();

	if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x, curLocation.y, curLocation.z + forwardPush/2, 0, 0, 1); }
	if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x, curLocation.y, curLocation.z - forwardPush/2, 0, 0, 1); }
	if (rotateLeftKey){ ENTITY::SET_ENTITY_HEADING(playerPed, curHeading + rotationSpeed); }
	if (rotateRightKey){ ENTITY::SET_ENTITY_HEADING(playerPed, curHeading - rotationSpeed); }

	if (curHeading >= 0 && curHeading <= 90)
	{
		if (moveFowardKey)
		{
			if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y + yVect, curLocation.z + forwardPush/2, 0, 0, 1); }
			else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y + yVect, curLocation.z - forwardPush / 2, 0, 0, 1); }
			else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y + yVect, curLocation.z, 0, 0, 1); }
		}
		if (moveBackKey)
		{
			if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y - yVect, curLocation.z + forwardPush / 2, 0, 0, 1); }
			else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y - yVect, curLocation.z - forwardPush / 2, 0, 0, 1); }
			else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y - yVect, curLocation.z, 0, 0, 1); }
		}
	}
	else if (curHeading >= 91 && curHeading <= 180)
	{
		if (moveFowardKey)
		{
			if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - yVect, curLocation.y - xVect, curLocation.z + forwardPush / 2, 0, 0, 1); }
			else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - yVect, curLocation.y - xVect, curLocation.z - forwardPush / 2, 0, 0, 1); }
			else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - yVect, curLocation.y - xVect, curLocation.z, 0, 0, 1); }
		}
		if (moveBackKey)
		{
			if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + yVect, curLocation.y + xVect, curLocation.z + forwardPush / 2, 0, 0, 1); }
			else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + yVect, curLocation.y + xVect, curLocation.z - forwardPush / 2, 0, 0, 1); }
			else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + yVect, curLocation.y + xVect, curLocation.z, 0, 0, 1); }
		}
	}
	else if (curHeading >= 181 && curHeading <= 270)
	{
		if (moveFowardKey)
		{
			if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y - yVect, curLocation.z + forwardPush / 2, 0, 0, 1); }
			else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y - yVect, curLocation.z - forwardPush / 2, 0, 0, 1); }
			else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y - yVect, curLocation.z, 0, 0, 1); }
		}
		if (moveBackKey)
		{
			if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y + yVect, curLocation.z + forwardPush / 2, 0, 0, 1); }
			else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y + yVect, curLocation.z - forwardPush / 2, 0, 0, 1); }
			else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - xVect, curLocation.y + yVect, curLocation.z, 0, 0, 1); }
		}
	}
	else if (curHeading >= 271 && curHeading <= 360)
	{
		if (moveFowardKey)
		{
			if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + yVect, curLocation.y + xVect, curLocation.z + forwardPush / 2, 0, 0, 1); }
			else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + yVect, curLocation.y + xVect, curLocation.z - forwardPush / 2, 0, 0, 1); }
			else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + yVect, curLocation.y + xVect, curLocation.z, 0, 0, 1); }
		}
		if (moveBackKey)
		{
			if (moveUpKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - yVect, curLocation.y - xVect, curLocation.z + forwardPush / 2, 0, 0, 1); }
			else if (moveDownKey){ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - yVect, curLocation.y - xVect, curLocation.z - forwardPush / 2, 0, 0, 1); }
			else{ ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x - yVect, curLocation.y - xVect, curLocation.z, 0, 0, 1); }
		}
	}
}