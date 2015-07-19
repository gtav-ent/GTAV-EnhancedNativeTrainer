/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "..\ui_support\menu_functions.h"
#include "propplacement.h"
#include "..\io\keyboard.h"
#include "..\io\config_io.h"
#include "..\utils.h"
#include "script.h"

bool pp_exit_flag = false;

int pp_travel_speed = 0;

bool in_placement_mode = false;

bool pp_frozen_time = false;
bool pp_help_showing = true;

Vector3 pp_cur_location;
Vector3 pp_cur_rotation;
float pp_cur_heading;

std::string propPlacerStatusLines[15];

DWORD propPlacerStatusTextDrawTicksMax;
bool propPlacerStatusTextGxtEntry;

SpawnedPropInstance* currentProp;

Camera propCamera;

void exit_prop_placer_if_showing()
{
	pp_exit_flag = true;
}

void begin_prop_placement(SpawnedPropInstance* prop)
{
	setGameInputToEnabled(false);

	pp_exit_flag = false;
	currentProp = prop;

	const float lineWidth = 250.0;
	const int lineCount = 1;
	
	std::string caption = "Object Placement";

	//draw_menu_header_line(caption,350.0f,50.0f,15.0f,0.0f,15.0f,false);

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	pp_frozen_time = false;

	pp_cur_location = ENTITY::GET_ENTITY_COORDS(prop->instance, 0);
	pp_cur_rotation = ENTITY::GET_ENTITY_ROTATION(prop->instance, 0);
	pp_cur_heading = ENTITY::GET_ENTITY_HEADING(prop->instance);

	propCamera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);

	//CAM::SET_CAM_FOV(propCamera, 50.0f);
	CAM::ATTACH_CAM_TO_ENTITY(propCamera, currentProp->instance, 0.0f, 5.0f, 1.0f, 1);
	CAM::POINT_CAM_AT_ENTITY(propCamera, currentProp->instance, 0.0f, 0.0f, 0.0f, 1);

	CAM::SET_CAM_ACTIVE(propCamera, 1);
	CAM::RENDER_SCRIPT_CAMS(1, 0, 3000, 1, 0);

	if (!CAM::DOES_CAM_EXIST(propCamera))
	{
		set_status_text("Camera failure");
	}
	else
	{
		set_status_text("Created camera");
	}

	while (true && !pp_exit_flag)
	{
		in_placement_mode = true;

		// draw menu
		draw_menu_header_line(caption, 350.0f, 50.0f, 15.0f, 0.0f, 15.0f, false);
		//draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);

		make_periodic_feature_call();

		BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
		BOOL propExists = ENTITY::DOES_ENTITY_EXIST(currentProp->instance);

		//Disable on object death
		if (ENTITY::IS_ENTITY_DEAD(playerPed) || !bPlayerExists || !propExists)
		{
			pp_exit_flag = true;
			break;
		}

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);
		if (bBack)
		{
			menu_beep();
			break;
		}

		if (!is_fivem() && NETWORK::NETWORK_IS_GAME_IN_PROGRESS())
		{
			break;
		}

		prop_placement();

		WAIT(0);
	}

	setGameInputToEnabled(true);
	CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, 1, 0);
	if (CAM::DOES_CAM_EXIST(propCamera))
	{
		set_status_text_centre_screen("Destroy Cam");
		CAM::DESTROY_CAM(propCamera, 1);
	}

	pp_exit_flag = false;
	in_placement_mode = false;
}

void update_prop_placement_text()
{
	if (GetTickCount() < propPlacerStatusTextDrawTicksMax)
	{
		int numLines = sizeof(propPlacerStatusLines) / sizeof(propPlacerStatusLines[0]);

		float textY = 0.1f;

		int numActualLines = 0;
		for (int i = 0; i < numLines; i++)
		{
			if (!pp_help_showing && i != 14)
			{
				continue;
			}

			numActualLines++;

			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.3, 0.3);
			if (i == 0 || i == 8 || i == 14)
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
			if (propPlacerStatusTextGxtEntry)
			{
				UI::_SET_TEXT_ENTRY((char *)propPlacerStatusLines[i].c_str());
			}
			else
			{
				UI::_SET_TEXT_ENTRY("STRING");
				UI::_ADD_TEXT_COMPONENT_STRING((char *)propPlacerStatusLines[i].c_str());
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

void create_prop_placement_help_text()
{
	//Debug
	std::stringstream ss;

	/*ss << "Heading: " << pp_cur_heading << " Rotation: " << pp_cur_rotation.z
	<< "\n xVect: " << xVect << "yVect: " << yVect;*/

	std::string pp_travel_speedStr;
	switch (pp_travel_speed)
	{
	case 0:
		pp_travel_speedStr = "Slow";
		break;
	case 1:
		pp_travel_speedStr = "Fast";
		break;
	case 2:
		pp_travel_speedStr = "Very Fast";
		break;
	}

	ss << "Current Travel Speed: ~HUD_COLOUR_WHITE~" << pp_travel_speedStr;

	int index = 0;
	propPlacerStatusLines[index++] = "Default Object Placement Keys (change in XML):";
	propPlacerStatusLines[index++] = "Q/Z - Move Up/Down";
	propPlacerStatusLines[index++] = "A/D - Rotate Left/Right";
	propPlacerStatusLines[index++] = "W/S - Move Forward/Back";
	propPlacerStatusLines[index++] = "Shift - Toggle Move Speed";
	propPlacerStatusLines[index++] = "T - Toggle Frozen Time";
	propPlacerStatusLines[index++] = "H - Toggle This Help";
	propPlacerStatusLines[index++] = " ";
	propPlacerStatusLines[index++] = "Default Controller Input (change in XML):";
	propPlacerStatusLines[index++] = "Triggers - Move Up/Down";
	propPlacerStatusLines[index++] = "Left Stick - Rotate, Move Forward/Back";
	propPlacerStatusLines[index++] = "A - Toggle Move Speed";
	propPlacerStatusLines[index++] = "B - Toggle Frozen Time";
	propPlacerStatusLines[index++] = " ";
	propPlacerStatusLines[index++] = ss.str();

	propPlacerStatusTextDrawTicksMax = GetTickCount() + 2500;
	propPlacerStatusTextGxtEntry = false;
}

void prop_placement()
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	//float tmpHeading = pp_cur_heading += ;

	float rotationSpeed = 2.5;
	float forwardPush;

	switch (pp_travel_speed)
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

	float xVect = forwardPush * sin(degToRad(pp_cur_heading)) * -1.0f;
	float yVect = forwardPush * cos(degToRad(pp_cur_heading));

	KeyInputConfig* keyConfig = get_config()->get_key_config();

	bool moveUpKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_UP) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_UP);
	bool moveDownKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_DOWN) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_DOWN);
	bool moveForwardKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_FORWARD) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_FORWARD);
	bool moveBackKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_BACK) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_BACK);
	bool rotateLeftKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT);
	bool rotateRightKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT);

	BOOL xBoolParam = 1;
	BOOL yBoolParam = 1;
	BOOL zBoolParam = 1;

	ENTITY::SET_ENTITY_VELOCITY(currentProp->instance, 0.0f, 0.0f, 0.0f);
	ENTITY::SET_ENTITY_ROTATION(currentProp->instance, 0, 0, 0, 0, false);

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_SPEED) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_SPEED))
	{
		pp_travel_speed++;
		if (pp_travel_speed > 2)
		{
			pp_travel_speed = 0;
		}
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_FREEZE_TIME) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_FREEZE_TIME))
	{
		pp_frozen_time = !pp_frozen_time;
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_HELP) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_HELP))
	{
		pp_help_showing = !pp_help_showing;
	}

	create_prop_placement_help_text();
	update_prop_placement_text();

	if (moveUpKey)
	{
		pp_cur_location.z += forwardPush / 2;
	}
	else if (moveDownKey)
	{
		pp_cur_location.z -= forwardPush / 2;
	}

	if (moveForwardKey)
	{
		pp_cur_location.x += xVect;
		pp_cur_location.y += yVect;
	}
	else if (moveBackKey)
	{
		pp_cur_location.x -= xVect;
		pp_cur_location.y -= yVect;
	}

	if (rotateLeftKey)
	{
		pp_cur_heading += rotationSpeed;
	}
	else if (rotateRightKey)
	{
		pp_cur_heading -= rotationSpeed;
	}

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(currentProp->instance, pp_cur_location.x, pp_cur_location.y, pp_cur_location.z, xBoolParam, yBoolParam, zBoolParam);
	ENTITY::SET_ENTITY_HEADING(currentProp->instance, pp_cur_heading - rotationSpeed);
}

bool is_in_prop_placement_mode()
{
	return in_placement_mode;
}

bool is_prop_placement_frozen_time()
{
	return pp_frozen_time;
}