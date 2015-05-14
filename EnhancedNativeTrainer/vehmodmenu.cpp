/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "vehicles.h"
//#include "menu_functions.h"

int activeLineIndexVehMod = 0;


bool onconfirm_vehmod_menu(MenuItem<int> choice)
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	switch (activeLineIndexVehMod)
	{
	case 0: //Upgrade Performance
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				VEHICLE::SET_VEHICLE_MOD(veh, 11, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 11) - 1, 1); //Engine
				VEHICLE::SET_VEHICLE_MOD(veh, 12, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 12) - 1, 1); //Brakes
				VEHICLE::SET_VEHICLE_MOD(veh, 13, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 13) - 1, 1); //Transmission
				VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, 1); //Turbo Tuning
				set_status_text("Added all Performance Upgrades");
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 1: //Upgrade Armor and Tires
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				VEHICLE::SET_VEHICLE_MOD(veh, 16, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 16) - 1, 1); //Armor
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0); //Bulletproof Tires
				set_status_text("Added all Armor Upgrades and Bulletproof Tires");
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 2: //Upgrade Suspension
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 15);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 15) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 15);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 15, currmod + 1, 1); //Increment ModValue
					set_status_text("Suspension Upgraded");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 15, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Suspension");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 3: //Change Spoiler
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 0) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 0);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 0, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Spoiler");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 0, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Spoiler");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 4: //Change Exhaust
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 4) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 4);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 4, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Exhaust");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 4, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Exhaust");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 5: //Change Hood
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 7) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 7);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 7, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Hood");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 7, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Hood");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 6: //Change Front Bumper
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 1) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 1);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 1, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Front Bumper");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 1, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Front Bumper");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 7: //Change Rear Bumper
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 2) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 2);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 2, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Rear Bumper");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 2, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Rear Bumper");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 8: //Change Grille
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 6) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 6);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 6, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Grille");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 6, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Grille");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 9: //Change Side Skirts
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 3) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 3);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 3, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Side Skirts");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 3, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Side Skirts");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 10: //Change Roof
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 10) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 10);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 10, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Roof");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 10, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Roof");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 11: //Change Window Tint
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int currmod = VEHICLE::GET_VEHICLE_WINDOW_TINT(veh);
				if (currmod < 3)
				{
					VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, currmod + 1); //Increment ModValue
					set_status_text("Changed Window Tint");
				}
				else
				{
					VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 0); //Start from beginning
					set_status_text("Default Window Tint");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 12: //Change Fenders
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 8) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 8);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 8, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Fenders");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 8, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Fenders");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 13: //Change Rollcage
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 5) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 5);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 5, currmod + 1, 1); //Increment ModValue
					set_status_text("Changed Rollcage");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 5, -1, 1); //Remove mod and start from beginning
					set_status_text("Default Rollcage");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 14: //Xenon Headlights
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, 1); //Headlights
				set_status_text("Added Xenon Headlights");
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 15: //Change Wheel Category
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int currmod = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
				if (currmod < 5)
				{
					VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, currmod + 1); //Increment ModValue
					VEHICLE::SET_VEHICLE_MOD(veh, 23, 1, 0); //Change to non-default wheel in category
				}
				else if (currmod == 5)
				{
					VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, currmod + 2); //Increment ModValue to 7 to skip bike wheels that glitch with cars
					VEHICLE::SET_VEHICLE_MOD(veh, 23, 1, 0);
				}
				else
				{
					VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 0); //Start over from 0 = Sports Wheels
					VEHICLE::SET_VEHICLE_MOD(veh, 23, 1, 0);
				}
				set_status_text("Changed Wheel Category");
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 16: //Change Wheels 
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 23) - 1;
				int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 23);
				if (currmod < nummods)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 23, currmod + 1, 0); //Increment ModValue
					set_status_text("Changed Wheels");
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(veh, 23, -1, 0); //Remove mod and start from beginning
					set_status_text("Default Wheels");
				}
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;

	case 17: //Remove All Mods
		if (bPlayerExists)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				for (int i = 0; i < 25; i++) {
					VEHICLE::REMOVE_VEHICLE_MOD(veh, i);
				}
				VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 1);
				set_status_text("Removed All Upgrades");
			}
			else
			{
				set_status_text("Player isn't in a vehicle");
			}
		}
		break;
	}
	return false;
}


void process_vehmod_menu()
{
	const int lineCount = 18;

	std::string caption = "Vehicle Mod Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "All Performance Upgrades", NULL, NULL, true },
		{ "All Armor Upgrades", NULL, NULL, true },
		{ "Change Suspension", NULL, NULL, true },
		{ "Change Spoiler", NULL, NULL, true },
		{ "Change Exhaust", NULL, NULL, true },
		{ "Change Hood", NULL, NULL, true },
		{ "Change Front Bumper", NULL, NULL, true },
		{ "Change Rear Bumper", NULL, NULL, true },
		{ "Change Grille", NULL, NULL, true },
		{ "Change Side Skirts", NULL, NULL, true },
		{ "Change Roof", NULL, NULL, true },
		{ "Change Window Tint", NULL, NULL, true },
		{ "Change Fenders", NULL, NULL, true },
		{ "Change Rollcage", NULL, NULL, true },
		{ "Add Xenon Headlights", NULL, NULL, true },
		{ "Change Wheel Category", NULL, NULL, true },
		{ "Change Wheels", NULL, NULL, true },
		{ "Remove All Upgrades", NULL, NULL, true }
	};
	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexVehMod, caption, onconfirm_vehmod_menu);
}
