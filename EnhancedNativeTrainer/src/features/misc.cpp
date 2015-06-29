/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "misc.h"
#include "hotkeys.h"

//==================
// MISC MENU
//==================

int activeLineIndexTrainerConfig = 0;
int activeLineHotkeyConfig = 0;

bool featureBlockInputInMenu = false;
bool featurePlayerResetOnDeath = true;

bool featurePlayerRadio = false;
bool featurePlayerRadioUpdated = false;
bool featureRadioAlwaysOff = false;
bool featureRadioAlwaysOffUpdated = false;

bool featureMiscLockRadio = false;
bool featureMiscHideHud = false;

const int TRAINERCONFIG_HOTKEY_MENU = 99;

void onchange_hotkey_function(int value, SelectFromListMenuItem* source)
{
	change_hotkey_function(source->extras.at(0), value);
}

bool process_misc_hotkey_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	for (int i = 1; i < 10; i++)
	{
		std::ostringstream itemCaption;
		std::vector<std::string> captions;
		void(*callback)(int, SelectFromListMenuItem*);

		itemCaption << "Hotkey " << i;

		bool keyAssigned = get_config()->get_key_config()->is_hotkey_assigned(i);
		if (!keyAssigned)
		{
			captions.push_back("Key Not Bound");
			callback = NULL;
		}
		else
		{
			for each (HOTKEY_DEF var in HOTKEY_AVAILABLE_FUNCS)
			{
				captions.push_back(var.caption);
			}
			callback = onchange_hotkey_function;
		}

		SelectFromListMenuItem* item = new SelectFromListMenuItem(captions, callback);
		item->caption = itemCaption.str();
		item->wrap = keyAssigned;
		item->extras.push_back(i);
		item->value = get_hotkey_function_index(i);
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, &activeLineHotkeyConfig, "Hotkey Config", NULL, NULL, NULL);

	return false;
}

bool onconfirm_trainerconfig_menu(MenuItem<int> choice)
{
	if (choice.value == TRAINERCONFIG_HOTKEY_MENU)
	{
		process_misc_hotkey_menu();
	}
	return false;
}

void process_misc_trainerconfig_menu()
{
	std::string caption = "Trainer Options";

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int>* stdItem = new MenuItem<int>();
	stdItem->caption = "Hotkey Setup";
	stdItem->value = TRAINERCONFIG_HOTKEY_MENU;
	stdItem->isLeaf = false;
	menuItems.push_back(stdItem);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Lock Controls While In Menu";
	toggleItem->toggleValue = &featureBlockInputInMenu;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Reset Skin On Death";
	toggleItem->toggleValue = &featurePlayerResetOnDeath;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexTrainerConfig, caption, onconfirm_trainerconfig_menu, NULL, NULL);
}

int activeLineIndexMisc = 0;

bool onconfirm_misc_menu(MenuItem<int> choice)
{
	switch (activeLineIndexMisc)
	{
	case 0:
		process_misc_trainerconfig_menu();
		break;
		// next radio track
	case 2:
		AUDIO::SKIP_RADIO_FORWARD();
		break;
		// switchable features
	default:
		break;
	}
	return false;
}

void process_misc_menu()
{
	const int lineCount = 5;

	std::string caption = "Miscellaneous Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Trainer Options", NULL, NULL, false },
		{ "Portable Radio", &featurePlayerRadio, &featurePlayerRadioUpdated, true },
		{ "Next Radio Track", NULL, NULL, true },
		{ "Radio Always Off", &featureRadioAlwaysOff, &featureRadioAlwaysOffUpdated, true },
		{ "Hide HUD", &featureMiscHideHud, NULL },
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexMisc, caption, onconfirm_misc_menu);
}

void reset_misc_globals()
{
	featureMiscHideHud =
		featurePlayerRadio =
		featureMiscLockRadio =
		featureRadioAlwaysOff = false;

	featurePlayerResetOnDeath = true;
	featureBlockInputInMenu = false;

	featureRadioAlwaysOffUpdated =
		featurePlayerRadioUpdated = true;
}

void update_misc_features(BOOL playerExists, Ped playerPed)
{
	if (featureRadioAlwaysOff || featurePlayerRadioUpdated)
	{
		if (featureRadioAlwaysOff)
		{
			if (featurePlayerRadio)
			{
				featurePlayerRadio = false;
				featurePlayerRadioUpdated = true;
			}
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, !featureRadioAlwaysOff);
		}

		AUDIO::SET_USER_RADIO_CONTROL_ENABLED(!featureRadioAlwaysOff);
	}

	// Portable radio
	if (featurePlayerRadio || featurePlayerRadioUpdated)
	{
		if (featurePlayerRadio)
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
		else
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		}
	}

	// hide hud
	if (featureMiscHideHud)
	{
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}
}

void add_misc_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerRadio", &featurePlayerRadio, &featurePlayerRadioUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureRadioAlwaysOff", &featureRadioAlwaysOff, &featureRadioAlwaysOffUpdated });

	results->push_back(FeatureEnabledLocalDefinition{ "featureMiscLockRadio", &featureMiscLockRadio });
	results->push_back(FeatureEnabledLocalDefinition{ "featureMiscHideHud", &featureMiscHideHud });

	results->push_back(FeatureEnabledLocalDefinition{ "featurePlayerResetOnDeath", &featurePlayerResetOnDeath });
	results->push_back(FeatureEnabledLocalDefinition{ "featureBlockInputInMenu", &featureBlockInputInMenu });
}

void add_misc_generic_settings(std::vector<StringPairSettingDBRow>* results)
{

}

void handle_generic_settings_misc(std::vector<StringPairSettingDBRow>* settings)
{

}

bool is_player_reset_on_death()
{
	return featurePlayerResetOnDeath;
}

bool is_input_blocked_in_menu()
{
	return featureBlockInputInMenu;
}