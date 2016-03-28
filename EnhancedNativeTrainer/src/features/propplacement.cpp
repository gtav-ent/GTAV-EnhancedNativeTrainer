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

int pp_travel_speed = 2;

bool in_placement_mode = false;

bool pp_frozen_time = false;
bool pp_help_showing = true;

const int MOVE_SPEED_COUNTS = 5;

Vector3 pp_cur_location;
Vector3 pp_cur_rotation;
float pp_cur_heading = 0.0f;
float pp_cur_roll = 0.0f;
float pp_cur_pitch = 0.0f;

std::vector<std::string> propPlacerStatusLines;

DWORD propPlacerStatusTextDrawTicksMax;
bool propPlacerStatusTextGxtEntry;

SpawnedPropInstance currentProp;

Camera propCamera;

void exit_prop_placer_if_showing()
{
	pp_exit_flag = true;
}

void begin_prop_placement(SpawnedPropInstance prop)
{
	setGameInputToEnabled(false);
	bool wasHudHidden = is_hud_hidden();
	set_hud_hidden(true);

	pp_exit_flag = false;
	currentProp = prop;

	const float lineWidth = 250.0;
	const int lineCount = 1;
	
	std::string caption = "Object Placement";

	//draw_menu_header_line(caption,350.0f,50.0f,15.0f,0.0f,15.0f,false);

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (&currentProp == NULL)
	{
		pp_exit_flag = true;
		return;
	}

	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
	BOOL propExists = ENTITY::DOES_ENTITY_EXIST(currentProp.instance);

	//Disable on object death
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || !bPlayerExists || !propExists)
	{
		return;
	}
	
	pp_frozen_time = false;

	pp_cur_location = ENTITY::GET_ENTITY_COORDS(prop.instance, 0);
	pp_cur_rotation = ENTITY::GET_ENTITY_ROTATION(prop.instance, 0);
	pp_cur_heading = ENTITY::GET_ENTITY_HEADING(prop.instance);

	//normalise the gameplay camera
	float tempFloat = CAM::GET_GAMEPLAY_CAM_RELATIVE_PITCH();
	CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
	CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(tempFloat, 0.0f);

	//work out a view distance
	ENTITY::SET_ENTITY_ROTATION(currentProp.instance, 0, 0, 0, 0, false);
	Hash modelHash = ENTITY::GET_ENTITY_MODEL(currentProp.instance);
	Vector3 minDimens;
	Vector3 maxDimens;
	GAMEPLAY::GET_MODEL_DIMENSIONS(modelHash, &minDimens, &maxDimens);
	float cameraDistance = max(4.0f, 1.3f * max(maxDimens.x - minDimens.x, maxDimens.y - minDimens.y));

	//create and configure our object camera
	propCamera = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
	//CAM::ATTACH_CAM_TO_ENTITY(propCamera, currentProp.instance, 0.0f, -cameraDistance, 2.0f, 1);
	Vector3 worldCamCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(currentProp.instance, 0.0f, -cameraDistance, 2.0f);
	CAM::SET_CAM_COORD(propCamera, worldCamCoords.x, worldCamCoords.y, worldCamCoords.z);
	CAM::POINT_CAM_AT_ENTITY(propCamera, currentProp.instance, 0.0f, 0.0f, 0.0f, 1);
	
	Vector3 camRot = CAM::GET_CAM_ROT(propCamera, 2);
	Vector3 camCoords = CAM::GET_CAM_COORD(propCamera);
	CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(20.0f, 0x3f800000);
	CAM::SET_CAM_ACTIVE(propCamera, 1);
	CAM::RENDER_SCRIPT_CAMS(1, 0, 3000, 1, 0);

	//check it worked
	if (!CAM::DOES_CAM_EXIST(propCamera))
	{
		set_status_text("Camera failure");
		return;
	}

	ENTITY::SET_ENTITY_QUATERNION(currentProp.instance, 0.0f, 0.0f, 0.0f, 0.0f);

	//begin the loop
	while (true && !pp_exit_flag)
	{
		in_placement_mode = true;

		// draw menu
		draw_menu_header_line(caption, 350.0f, 50.0f, 15.0f, 0.0f, 15.0f, false);
		//draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);

		make_periodic_feature_call();

		if (&currentProp == NULL)
		{
			pp_exit_flag = true;
			break;
		}

		bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
		propExists = ENTITY::DOES_ENTITY_EXIST(currentProp.instance);
		BOOL camExists = CAM::DOES_CAM_EXIST(propCamera);

		//Disable on object death
		if (ENTITY::IS_ENTITY_DEAD(playerPed) || !bPlayerExists || !propExists || !camExists)
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

		if (NETWORK::NETWORK_IS_GAME_IN_PROGRESS())
		{
			break;
		}

		/*
		std::ostringstream ss;
		ss << "Pitch: " << CAM::GET_GAMEPLAY_CAM_RELATIVE_PITCH() << ", Heading:" << CAM::GET_GAMEPLAY_CAM_RELATIVE_HEADING();
		set_status_text_centre_screen(ss.str());
		*/

		prop_placement();

		Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(currentProp.instance, 1);

		float camPitch = CAM::GET_GAMEPLAY_CAM_RELATIVE_PITCH();
		float camHeading = CAM::GET_GAMEPLAY_CAM_RELATIVE_HEADING();

		CAM::_CLAMP_GAMEPLAY_CAM_PITCH(-90.0f, 90.0f);

		SimpleVector3 newCamCoords = get_camera_coords(entityCoords, cameraDistance, camPitch, camHeading);

		float groundZ;
		float aboveGroundMargin = 0.15f;
		bool groundFound = GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(newCamCoords.x, newCamCoords.y, entityCoords.z + 3.0f, &groundZ) == 1;
		if (groundFound && groundZ > newCamCoords.z - aboveGroundMargin)
		{
			float limitedCamAngle = radToDeg(asin((newCamCoords.z - entityCoords.z) / cameraDistance));
			newCamCoords = get_camera_coords(entityCoords, cameraDistance, limitedCamAngle, camHeading);
			CAM::_CLAMP_GAMEPLAY_CAM_PITCH(limitedCamAngle, 90.0f);
		}

		CAM::SET_CAM_COORD(propCamera, newCamCoords.x, newCamCoords.y, newCamCoords.z);
		CAM::POINT_CAM_AT_ENTITY(propCamera, currentProp.instance, 0.0f, 0.0f, 0.0f, 1);

		WAIT(0);
	}

	setGameInputToEnabled(true);

	set_hud_hidden(wasHudHidden);

	CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, 1, 0);
	if (CAM::DOES_CAM_EXIST(propCamera))
	{
		CAM::DESTROY_CAM(propCamera, 1);
	}

	if (!currentProp.isImmovable)
	{
		OBJECT::SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(currentProp.instance, true);
		ENTITY::FREEZE_ENTITY_POSITION(currentProp.instance, false);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(currentProp.instance, false);
		ENTITY::APPLY_FORCE_TO_ENTITY(currentProp.instance, 3, 0, 0, 0.1, 0, 0, 0, 0, 1, 1, 0, 0, 1);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(currentProp.instance, !currentProp.isInvincible);
	}

	pp_exit_flag = false;
	in_placement_mode = false;
}

SimpleVector3 get_camera_coords(Vector3 entityCoords, float cameraDistance, float camPitch, float camHeading)
{
	float xVect = cameraDistance * sin(degToRad(camHeading)) * cos(degToRad(camPitch));
	float yVect = cameraDistance * cos(degToRad(camHeading)) * cos(degToRad(camPitch));
	float zVect = cameraDistance * sin(degToRad(camPitch));

	float newCamX = entityCoords.x + xVect;
	float newCamY = entityCoords.y + yVect;
	float newCamZ = entityCoords.z + zVect;

	return SimpleVector3{ newCamX, newCamY, newCamZ };
}

void update_prop_placement_text()
{
	if (GetTickCount() < propPlacerStatusTextDrawTicksMax)
	{
		int numLines = propPlacerStatusLines.size();

		float textY = 0.1f;

		int numActualLines = 0;
		for (int i = 0; i < numLines; i++)
		{
			if (!pp_help_showing && i < 18)
			{
				continue;
			}

			numActualLines++;

			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.3, 0.3);
			if (i == 0 || i == 10 || i >= 17)
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
				UI::_SET_TEXT_ENTRY((char *)propPlacerStatusLines.at(i).c_str());
			}
			else
			{
				UI::_SET_TEXT_ENTRY("STRING");
				UI::_ADD_TEXT_COMPONENT_STRING((char *)propPlacerStatusLines.at(i).c_str());
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
		pp_travel_speedStr = "Very Slow";
		break;
	case 1:
		pp_travel_speedStr = "Slow";
		break;
	case 2:
		pp_travel_speedStr = "Normal";
		break;
	case 3:
		pp_travel_speedStr = "Fast";
		break;
	case 4:
		pp_travel_speedStr = "Very Fast";
		break;
	}

	propPlacerStatusLines.clear();

	propPlacerStatusLines.push_back("Default Object Placement Keys (change in XML):");
	propPlacerStatusLines.push_back("Q/Z - Move Up/Down");
	propPlacerStatusLines.push_back("A/D - Rotate Left/Right (Alt: Roll)");
	propPlacerStatusLines.push_back("W/S - Move Forward/Back (Alt: Pitch)");
	propPlacerStatusLines.push_back("Alt - Alternate Movement");
	propPlacerStatusLines.push_back("[ and ] - Decrease/Increase Move Speed");
	propPlacerStatusLines.push_back("T - Toggle Frozen Time");
	propPlacerStatusLines.push_back("G - Toggle Object Frozen On Exit");
	propPlacerStatusLines.push_back("H - Toggle This Help");

	propPlacerStatusLines.push_back(" ");
	propPlacerStatusLines.push_back("Default Controller Input (change in XML):");
	propPlacerStatusLines.push_back("Triggers - Move Up/Down");
	propPlacerStatusLines.push_back("Left Stick - Rotate, Move Forward/Back");
	propPlacerStatusLines.push_back("A - Cycle Move Speeds");
	propPlacerStatusLines.push_back("B - Toggle Frozen Time");
	propPlacerStatusLines.push_back("Y - Toggle Object Frozen On Exit");
	propPlacerStatusLines.push_back(" ");

	propPlacerStatusLines.push_back("Press 'Menu Back' to save and exit this mode");
	propPlacerStatusLines.push_back(" ");

	ss << "Current Travel Speed: ~HUD_COLOUR_WHITE~" << pp_travel_speedStr;
	propPlacerStatusLines.push_back(ss.str());
	ss.str(""); ss.clear();

	ss << "Object Frozen On Exit: ~HUD_COLOUR_WHITE~" << (currentProp.isImmovable ? "Yes": "No");
	propPlacerStatusLines.push_back(ss.str());
	ss.str(""); ss.clear();

	propPlacerStatusTextDrawTicksMax = GetTickCount() + 2500;
	propPlacerStatusTextGxtEntry = false;
}

void prop_placement()
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	//float tmpHeading = pp_cur_heading += ;

	float rotationSpeed = 2.5f;
	float forwardPush;

	switch (pp_travel_speed)
	{
	case 0:
		rotationSpeed = 0.5f;
		forwardPush = 0.04f;
		break;
	case 1:
		rotationSpeed = 1.0f;
		forwardPush = 0.1f;
		break;
	case 2:
		forwardPush = 0.2f;
		break;
	case 3:
		forwardPush = 0.8f;
		break;
	case 4:
		forwardPush = 1.6f;
		break;
	}

	float xVect = forwardPush * sin(degToRad(pp_cur_heading)) * -1.0f;
	float yVect = forwardPush * cos(degToRad(pp_cur_heading));

	KeyInputConfig* keyConfig = get_config()->get_key_config();

	bool moveUpKey = IsKeyDown(KeyConfig::KEY_OBJECTPLACER_UP) || IsControllerButtonDown(KeyConfig::KEY_OBJECTPLACER_UP);
	bool moveDownKey = IsKeyDown(KeyConfig::KEY_OBJECTPLACER_DOWN) || IsControllerButtonDown(KeyConfig::KEY_OBJECTPLACER_DOWN);
	bool moveForwardKey = IsKeyDown(KeyConfig::KEY_OBJECTPLACER_FORWARD) || IsControllerButtonDown(KeyConfig::KEY_OBJECTPLACER_FORWARD);
	bool moveBackKey = IsKeyDown(KeyConfig::KEY_OBJECTPLACER_BACK) || IsControllerButtonDown(KeyConfig::KEY_OBJECTPLACER_BACK);
	bool rotateLeftKey = IsKeyDown(KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT) || IsControllerButtonDown(KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT);
	bool rotateRightKey = IsKeyDown(KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT) || IsControllerButtonDown(KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT);

	bool secondaryMove = IsKeyDown(KeyConfig::KEY_OBJECTPLACER_ALT_MOVE) || IsControllerButtonDown(KeyConfig::KEY_OBJECTPLACER_ALT_MOVE);

	BOOL xBoolParam = 1;
	BOOL yBoolParam = 1;
	BOOL zBoolParam = 1;

	ENTITY::SET_ENTITY_VELOCITY(currentProp.instance, 0.0f, 0.0f, 0.0f);

	if (IsControllerButtonJustUp(KeyConfig::KEY_OBJECTPLACER_SPEED_CYCLE))
	{
		pp_travel_speed++;
		if (pp_travel_speed >= MOVE_SPEED_COUNTS)
		{
			pp_travel_speed = 0;
		}
	}

	if (IsKeyJustUp(KeyConfig::KEY_OBJECTPLACER_SPEED_UP))
	{
		if (pp_travel_speed < MOVE_SPEED_COUNTS - 1)
		{
			pp_travel_speed++;
		}
	}
	else if (IsKeyJustUp(KeyConfig::KEY_OBJECTPLACER_SPEED_DOWN))
	{
		if (pp_travel_speed > 0)
		{
			pp_travel_speed--;
		}
	}

	if (IsKeyJustUp(KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME) || IsControllerButtonJustUp(KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME))
	{
		pp_frozen_time = !pp_frozen_time;
	}

	if (IsKeyJustUp(KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION) || IsControllerButtonJustUp(KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION))
	{
		currentProp.isImmovable = !currentProp.isImmovable;
	}

	if (IsKeyJustUp(KeyConfig::KEY_OBJECTPLACER_HELP) || IsControllerButtonJustUp(KeyConfig::KEY_OBJECTPLACER_HELP))
	{
		pp_help_showing = !pp_help_showing;
	}

	create_prop_placement_help_text();
	update_prop_placement_text();

	if (secondaryMove)
	{
		/*float roll = ENTITY::GET_ENTITY_ROLL(currentProp.instance);
		float pitch = ENTITY::GET_ENTITY_PITCH(currentProp.instance);

		float qx, qy, qz, qw;
		ENTITY::GET_ENTITY_QUATERNION(currentProp.instance, &qx, &qy, &qz, &qw);

		float mouseX = CONTROLS::GET_CONTROL_NORMAL(2, 239);
		float mouseY = CONTROLS::GET_CONTROL_NORMAL(2, 240);

		std::ostringstream ss;
		ss << "QX: " << qx << ", QY:" << qy << ", QZ: " << qz << ", QW: " << qw;
		//ss << "Roll: " << roll << ", Pitch: " << pitch;
		set_status_text_centre_screen(ss.str());
		*/

		if (moveForwardKey)
		{
			pp_cur_pitch -= rotationSpeed;
		}
		else if (moveBackKey)
		{
			pp_cur_pitch += rotationSpeed;
		}

		if (rotateLeftKey)
		{
			pp_cur_roll -= rotationSpeed;
		}
		else if (rotateRightKey)
		{
			pp_cur_roll += rotationSpeed;
		}

		//ENTITY::SET_ENTITY_QUATERNION(currentProp.instance, qx, qy, qz, qw);
	}
	else
	{
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
	}

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(currentProp.instance, pp_cur_location.x, pp_cur_location.y, pp_cur_location.z, xBoolParam, yBoolParam, zBoolParam);
	ENTITY::SET_ENTITY_ROTATION(currentProp.instance, pp_cur_pitch, pp_cur_roll, pp_cur_heading, 2, 1);
}

bool is_in_prop_placement_mode()
{
	return in_placement_mode;
}

bool is_prop_placement_frozen_time()
{
	return pp_frozen_time;
}