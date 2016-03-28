#include "aimbot_esp.h"

int aimbotESPLineIndex = 0;

bool pedESP = false;

Entity aimedAt = NULL;
bool isTargetLocked = false;

bool aimAtVehicles = false;
bool aimThroughWalls = false;

int aimbotIndex = 0;
int targetMethod = 0;
int aimbotBoneIndex = 0;
int aimbotToleranceIndex = 0;
int aimbotVehOffsetIndex = 12;

static Hash noAimWeapons[] = { 0x93E220BD, 0x2C3731D9, 0xAB564B93, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0x7F7497E5, 0xA284510B, 0xB1CA77B1, 0x63AB0442 }; // Hashes of weapons for which we don't want AA, I should have labeled these or used ent-enums

void draw_ESP_box(Ped ped, int red, int green, int blue, int alpha) {
	Vector3 pedPosition = ENTITY::GET_ENTITY_COORDS(ped, FALSE);

	int screenResX, screenResY;
	float screenX, screenY;

	GRAPHICS::_GET_SCREEN_ACTIVE_RESOLUTION(&screenResX, &screenResY); // use this to correct for screen ratio

	if (GRAPHICS::_WORLD3D_TO_SCREEN2D(pedPosition.x, pedPosition.y, pedPosition.z, &screenX, &screenY) == TRUE) {
		GRAPHICS::DRAW_RECT(screenX, screenY, 5.0f / (float)screenResX, 5.0f / (float)screenResY, red, green, blue, alpha);
	}

	UI::SET_TEXT_OUTLINE();
	GRAPHICS::DRAW_LINE(pedPosition.x + 0.5, pedPosition.y + 0.5, pedPosition.z + 0.75, pedPosition.x + 0.5, pedPosition.y - 0.5, pedPosition.z + 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x + 0.5, pedPosition.y - 0.5, pedPosition.z + 0.75, pedPosition.x - 0.5, pedPosition.y - 0.5, pedPosition.z + 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x - 0.5, pedPosition.y - 0.5, pedPosition.z + 0.75, pedPosition.x - 0.5, pedPosition.y + 0.5, pedPosition.z + 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x - 0.5, pedPosition.y + 0.5, pedPosition.z + 0.75, pedPosition.x + 0.5, pedPosition.y + 0.5, pedPosition.z + 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x + 0.5, pedPosition.y + 0.5, pedPosition.z - 0.75, pedPosition.x + 0.5, pedPosition.y - 0.5, pedPosition.z - 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x + 0.5, pedPosition.y - 0.5, pedPosition.z - 0.75, pedPosition.x - 0.5, pedPosition.y - 0.5, pedPosition.z - 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x - 0.5, pedPosition.y - 0.5, pedPosition.z - 0.75, pedPosition.x - 0.5, pedPosition.y + 0.5, pedPosition.z - 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x - 0.5, pedPosition.y + 0.5, pedPosition.z - 0.75, pedPosition.x + 0.5, pedPosition.y + 0.5, pedPosition.z - 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x + 0.5, pedPosition.y + 0.5, pedPosition.z - 0.75, pedPosition.x + 0.5, pedPosition.y + 0.5, pedPosition.z + 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x + 0.5, pedPosition.y - 0.5, pedPosition.z - 0.75, pedPosition.x + 0.5, pedPosition.y - 0.5, pedPosition.z + 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x - 0.5, pedPosition.y - 0.5, pedPosition.z - 0.75, pedPosition.x - 0.5, pedPosition.y - 0.5, pedPosition.z + 0.75, red, green, blue, alpha);
	GRAPHICS::DRAW_LINE(pedPosition.x - 0.5, pedPosition.y + 0.5, pedPosition.z - 0.75, pedPosition.x - 0.5, pedPosition.y + 0.5, pedPosition.z + 0.75, red, green, blue, alpha);
}

void doESP() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<Ped> peds;
	const int ARR_SIZE = 1024;
	Ped worldPeds[ARR_SIZE];

	int numPedsInWorld = worldGetAllPeds(worldPeds, ARR_SIZE);

	for (int i = 0; i < numPedsInWorld; i++) {
		if (ENTITY::DOES_ENTITY_EXIST(worldPeds[i]) && !ENTITY::IS_ENTITY_DEAD(worldPeds[i]) && worldPeds[i] != playerPed)
		peds.push_back(worldPeds[i]);
	}

	for (std::vector<Ped>::iterator pedIterator = peds.begin(); pedIterator != peds.end(); ++pedIterator)
	{
		//Remove this if you want to see everyone or people behind walls.
		if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, *pedIterator, 17)) { // 17 is a recommended value
			draw_ESP_box(*pedIterator, 255, 0, 0, 175);
		}
		else {
			draw_ESP_box(*pedIterator, 255, 145, 0, 175);
		}
	}
}

void doAimbot(Entity targetPed) {
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Hash currentWeapon;
	WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &currentWeapon, true);

	for each (Hash weapon in noAimWeapons) {
		if (currentWeapon == weapon) {
			return; // don't auto aim if we're wielding a weapon in noAimWeapons
		}
	}

	if (PED::GET_PED_PARACHUTE_STATE(playerPed) == 2) { // if our parachute is open, don't auto-aim (it's really buggy)
		return;
	}

	int boneId = 0;

	if (ENTITY::IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(targetPed)) {
		boneId = 0x796E; // if Ped is in a vehicle, just aim for the head because we won't get any hits otherwise
	}
	else {
		boneId = AIMBOT_BONE[aimbotBoneIndex];
	}

	Vector3D targetPosWorld3D(PED::GET_PED_BONE_COORDS(targetPed, boneId, 0.0f, 0.0f, 0.0f));
	Vector3D playerPosWorld3D(ENTITY::GET_ENTITY_COORDS(playerPed, true));
	Vector3D playerCam3D(CAM::GET_GAMEPLAY_CAM_COORD());

	// draw a red box over what we're aiming
	int screenResX, screenResY;
	GRAPHICS::_GET_SCREEN_ACTIVE_RESOLUTION(&screenResX, &screenResY); // use this to correct for screen ratio

	float screenX, screenY;
	if (GRAPHICS::_WORLD3D_TO_SCREEN2D(targetPosWorld3D.x, targetPosWorld3D.y, targetPosWorld3D.z, &screenX, &screenY) == TRUE) {
		GRAPHICS::DRAW_RECT(screenX, screenY, 10.0f / (float)screenResX, 10.0f / (float)screenResY, 255, 0, 0, 150);
	}

	// get x-angle between player heading and player to target

	// x-axis is west-east
	// y-axis is north-south
	// z-axis is above/below player (looking up and down)

	// get vector from camera to target
	Vector3D camToTarget = targetPosWorld3D - playerCam3D;
	// get vector from camera to player
	// Vector3D camToPlayer = playerPosWorld3D - playerCam3D;
	// get vector from player to target
	// Vector3D playerToTarget = targetPosWorld3D - playerPosWorld3D;

	// angle between camera and target along the x-y plane
	float camHeadingAngle = atan2f(camToTarget.x, camToTarget.y) * 180.0 / 3.14159265359; // GTA V likes things in degrees, not radians
	// float camToPlayerHeadingAngle = atan2f(camToPlayer.x, camToPlayer.y) * 180.0 / 3.14159265359;
	// float playerToTargetHeadingAngle = atan2f(playerToTarget.x, playerToTarget.y) * 180.0 / 3.14159265359;

	// angle between camera and target along the z axis
	float camPitchAngle = asinf(camToTarget.z / camToTarget.GetMagnitude()) * 180.0 / 3.14159265359;
	// float camToPlayerPitchAngle = asinf(camToPlayer.z / camToPlayer.GetMagnitude()) * 180.0 / 3.14159265359;
	// float playerToTargetPitchAngle = asinf(playerToTarget.z / playerToTarget.GetMagnitude()) * 180.0 / 3.14159265359;
	
	float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);
	float playerPitch = ENTITY::GET_ENTITY_PITCH(playerPed);

	// float vehHeading = ENTITY::GET_ENTITY_HEADING(ENTITY::GET_ENTITY_ATTACHED_TO(playerPed));
	// float vehPitch = ENTITY::GET_ENTITY_PITCH(ENTITY::GET_ENTITY_ATTACHED_TO(playerPed));

	if (ENTITY::IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(playerPed)) { // This function compensates us for the weird vertical offsets introduced by vehicles. Would love to figure out how to remove the need for this.
		camPitchAngle += AIMBOT_VEH_OFFSET[aimbotVehOffsetIndex];
	}

	// Using SET_GAMEPLAY_CAM_RELATIVE_HEADING/PITCH sets the camera pitch relative to the entity's heading/pitch
	// Another way to think about it is that the x/y plane is equivalent to a piece of cardboard glued flat on the top of the entity's head
	// This is especially noticable when sitting in a car, where if we don't take into account the entity's pitch, we'll be aiming either way above or way below our target

	if (camHeadingAngle >= 0.0 && camHeadingAngle <= 180.0) {
		camHeadingAngle = 360.0 - camHeadingAngle;
	}
	else if (camHeadingAngle <= -0.0 && camHeadingAngle >= -180.0) {
		camHeadingAngle = -camHeadingAngle;
	}

	float deltaHeading = camHeadingAngle - playerHeading;
	float deltaPitch = camPitchAngle - playerPitch;

	CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(deltaHeading); // adjust heading
	CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(deltaPitch, 0x3F800000); // adjust pitch
	CAM::_SET_GAMEPLAY_CAM_RAW_PITCH(deltaHeading);
	CAM::_SET_GAMEPLAY_CAM_RAW_YAW(deltaPitch);
}

void onchange_aimbot(int value, SelectFromListMenuItem* source)
{
	aimbotIndex = value;
}

void onchange_aimbot_bone(int value, SelectFromListMenuItem* source)
{
	aimbotBoneIndex = value;
}

void onchange_target_method(int value, SelectFromListMenuItem* source)
{
	targetMethod = value;
}

void onchange_aimbot_tolerance(int value, SelectFromListMenuItem* source)
{
	aimbotToleranceIndex = value;
}

void onchange_aimbot_veh_offset(int value, SelectFromListMenuItem* source)
{
	aimbotVehOffsetIndex = value;
}

void add_aimbot_esp_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "pedESP", std::to_string(pedESP) });
	results->push_back(StringPairSettingDBRow{ "aimbotIndex", std::to_string(aimbotIndex) });
	results->push_back(StringPairSettingDBRow{ "aimbotBoneIndex", std::to_string(aimbotBoneIndex) });
	results->push_back(StringPairSettingDBRow{ "targetMethod", std::to_string(targetMethod) });
	results->push_back(StringPairSettingDBRow{ "aimbotToleranceIndex", std::to_string(aimbotToleranceIndex) });
	results->push_back(StringPairSettingDBRow{ "aimbotVehOffsetIndex", std::to_string(aimbotVehOffsetIndex) });
	results->push_back(StringPairSettingDBRow{ "aimAtVehicles", std::to_string(aimAtVehicles) });
	results->push_back(StringPairSettingDBRow{ "aimThroughWalls", std::to_string(aimThroughWalls) });
}

void handle_generic_settings_aimbot_esp(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("pedESP") == 0)
		{
			pedESP = stoi(setting.value);
		}
		if (setting.name.compare("aimbotIndex") == 0)
		{
			aimbotIndex = stoi(setting.value);
		}
		if (setting.name.compare("aimbotBoneIndex") == 0)
		{
			aimbotBoneIndex = stoi(setting.value);
		}
		if (setting.name.compare("targetMethod") == 0)
		{
			targetMethod = stoi(setting.value);
		}
		if (setting.name.compare("aimbotToleranceIndex") == 0)
		{
			aimbotToleranceIndex = stoi(setting.value);
		}
		if (setting.name.compare("aimbotVehOffsetIndex") == 0)
		{
			aimbotVehOffsetIndex = stoi(setting.value);
		}
		if (setting.name.compare("aimAtVehicles") == 0)
		{
			aimAtVehicles = stoi(setting.value);
		}
		if (setting.name.compare("aimThroughWalls") == 0)
		{
			aimThroughWalls = stoi(setting.value);
		}
	}
}

void reset_aimbot_globals() {
	aimbotESPLineIndex = 0;

	pedESP = 0;

	aimedAt = NULL;
	isTargetLocked = false;

	aimAtVehicles = false;
	aimThroughWalls = false;

	aimbotIndex = 0;
	aimbotBoneIndex = 0;
	aimbotToleranceIndex = 0;
	aimbotVehOffsetIndex = 0;
}


bool process_aimbot_esp_menu()
{
	std::string caption = "Aimbot ESP Options";
	std::vector<MenuItem<int>*> menuItems;
	ToggleMenuItem<int>* toggleItem;
	SelectFromListMenuItem *listItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Ped ESP";
	toggleItem->value = i++;
	toggleItem->toggleValue = &pedESP;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(AIMBOT_TARGETS, onchange_aimbot);
	listItem->wrap = false;
	listItem->caption = "Aimbot";
	listItem->value = aimbotIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(AIMBOT_TARGET_METHOD, onchange_target_method);
	listItem->wrap = false;
	listItem->caption = "Target Method";
	listItem->value = targetMethod;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(AIMBOT_TOLERANCE_CAPTIONS, onchange_aimbot_tolerance);
	listItem->wrap = false;
	listItem->caption = "Target Lock Radius";
	listItem->value = aimbotToleranceIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(AIMBOT_VEH_OFFSET_CAPTIONS, onchange_aimbot_veh_offset);
	listItem->wrap = false;
	listItem->caption = "Vehicle Aimbot Offset";
	listItem->value = aimbotVehOffsetIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Aim Through Walls";
	toggleItem->value = i++;
	toggleItem->toggleValue = &aimThroughWalls;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Target Peds in Vehicles";
	toggleItem->value = i++;
	toggleItem->toggleValue = &aimAtVehicles;
	toggleItem->toggleValueUpdated = NULL;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(AIMBOT_BONE_CAPTION, onchange_aimbot_bone);
	listItem->wrap = false;
	listItem->caption = "Aim Bone";
	listItem->value = aimbotBoneIndex;
	menuItems.push_back(listItem);

	return draw_generic_menu<int>(menuItems, &aimbotESPLineIndex, caption, onconfirm_aimbot_esp_menu, NULL, NULL);
}

bool onconfirm_aimbot_esp_menu(MenuItem<int> choice) {
	return false;
	// nothing here yet
}

Entity get_ped_in_freeaim() {
	Entity aimedAt = 0;
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &aimedAt);
	Ped targetPed = ENTITY::GET_PED_INDEX_FROM_ENTITY_INDEX(aimedAt);

	bool inSameCar = ENTITY::IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(aimedAt) && (ENTITY::GET_ENTITY_ATTACHED_TO(playerPed) == ENTITY::GET_ENTITY_ATTACHED_TO(aimedAt));

	// Make sure we're aiming at a ped that's NOT a vehicle, that's ALIVE, and is NOT friendly to the player
	if (!inSameCar && !PED::IS_PED_DEAD_OR_DYING(aimedAt, true) && PED::IS_PED_HUMAN(aimedAt) && (PED::GET_RELATIONSHIP_BETWEEN_PEDS(playerPed, aimedAt) >= 3)) {
	// if (!inSameCar && ENTITY::IS_ENTITY_A_PED(aimedAt) && !ENTITY::IS_ENTITY_A_VEHICLE(aimedAt) &&  !ENTITY::IS_ENTITY_DEAD(aimedAt) && ENTITY::GET_ENTITY_ALPHA(aimedAt) == 255 && (PED::GET_RELATIONSHIP_BETWEEN_PEDS(playerPed, targetPed) >= 3)) {
		return aimedAt;
	}
	return 0;
}

Entity get_ped_nearest_to_crosshair() {
	Entity targetThis = 0;

	float minDist = 5.0f;
	float pedDist = 1.0f;

	float xScreen = 1.0f;
	float yScreen = 1.0f;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<Ped> peds;
	const int ARR_SIZE = 1024;
	Ped worldPeds[ARR_SIZE];

	int numPedsInWorld = worldGetAllPeds(worldPeds, ARR_SIZE);

	for (int i = 0; i < numPedsInWorld; i++) {
		peds.push_back(worldPeds[i]);
	}

	for (std::vector<Ped>::iterator pedIterator = peds.begin(); pedIterator != peds.end(); ++pedIterator)
	{
		if (ENTITY::DOES_ENTITY_EXIST(*pedIterator) && *pedIterator != playerPed)
		{
			bool inSameCar = ENTITY::IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(*pedIterator) && (ENTITY::GET_ENTITY_ATTACHED_TO(playerPed) == ENTITY::GET_ENTITY_ATTACHED_TO(*pedIterator));

			// Don't lock onto friendlies
			// PED::GET_RELATIONSHIP_BETWEEN_PEDS(playerPed, *pedIterator)
			// 0 = Companion
			// 1 = Respect
			// 2 = Like
			// 3 = Neutral
			// 4 = Dislike
			// 5 = Hate
			// 255 = Pedestrians

			if (!inSameCar && !PED::IS_PED_DEAD_OR_DYING(*pedIterator, true) && PED::IS_PED_HUMAN(*pedIterator) && (PED::GET_RELATIONSHIP_BETWEEN_PEDS(playerPed, *pedIterator) >= 3) && (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, *pedIterator, 17) || (!ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, *pedIterator, 17) && aimThroughWalls))) {
				// bunch of conditions for auto aim, code is self explanatory
				float xScreen, yScreen;

				Vector3D pedPosWorld3D(PED::GET_PED_BONE_COORDS(*pedIterator, AIMBOT_BONE[aimbotBoneIndex], 0.0, 0.0, 0.0));

				GRAPHICS::_WORLD3D_TO_SCREEN2D(pedPosWorld3D.x, pedPosWorld3D.y, pedPosWorld3D.z, &xScreen, &yScreen);

				float xDelta = xScreen - 0.5f;
				float yDelta = yScreen - 0.5f;

				float crosshairMag = sqrtf(xDelta * xDelta + yDelta * yDelta);

				if ((crosshairMag < AIMBOT_TOLERANCE[aimbotToleranceIndex]) && (crosshairMag < minDist)) {
					targetThis = *pedIterator;
					minDist = crosshairMag;
				}
			}
		}
	}

	return targetThis;
}

void update_aimbot_esp_features() {
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3D playerCam3D(CAM::GET_GAMEPLAY_CAM_COORD());

	Vector3D playerCoords(ENTITY::GET_ENTITY_COORDS(playerPed, true));
	Vector3D entityCoords(ENTITY::GET_ENTITY_COORDS(ENTITY::GET_ENTITY_ATTACHED_TO(playerPed), true));
	float pHeight = ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(playerPed);
	float eHeight = ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(ENTITY::GET_ENTITY_ATTACHED_TO(playerPed));

	if (bPlayerExists && pedESP) {
		doESP();
	}

	if (bPlayerExists && aimbotIndex) {
		if (PLAYER::IS_PLAYER_FREE_AIMING(player)) {

			if (isTargetLocked == false) {
				// acquire a new target

				if (aimbotIndex == 1) { // Target NPCs
					if (targetMethod == 0) {
						// target whatever GTA thinks the player is free aiming at
						aimedAt = get_ped_in_freeaim();
					}
					else if (targetMethod == 1) {
						// get the ped nearest to the crosshair
						aimedAt = get_ped_nearest_to_crosshair();
					}

					if (!ENTITY::IS_ENTITY_DEAD(aimedAt) && !PED::IS_PED_A_PLAYER(aimedAt) && (!ENTITY::IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(aimedAt) || (ENTITY::IS_ENTITY_ATTACHED_TO_ANY_VEHICLE(aimedAt) && aimAtVehicles))) {
						isTargetLocked = true;
					}
					else {
						isTargetLocked = false;
					}
				}
				// Removed a bunch of code for aiming only at vehicles, objects, etc...
			}
			else { // Target is locked
				if (!ENTITY::DOES_ENTITY_EXIST(aimedAt) || ENTITY::IS_ENTITY_DEAD(aimedAt)) { // Do a sanity check to make sure we're not constantly locking onto something that we shouldn't be 
					isTargetLocked = false;
				}

				if (!ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, aimedAt, 17) && !aimThroughWalls) { // Drop target if wallhack is off and we lose vision of him
					isTargetLocked = false;
				}
				else {
					doAimbot(aimedAt);
				}
			}
		}
		else {
			isTargetLocked = false; // no longer free aiming, so unlock the target
		}
	}
}