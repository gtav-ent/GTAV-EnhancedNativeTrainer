/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "vehicles.h"
#include "menu_functions.h"
#include "config_io.h"
#include "debuglog.h"

bool lastSeenInVehicle = false;

bool featureVehInvincible = false;
bool featureVehInvincibleUpdated = false;
bool featureNoVehFallOff = false;
bool featureNoVehFallOffUpdated = false;
bool featureVehSpeedBoost = false;
bool featureVehSpawnInto = false;
bool featureVehicleDoorInstant = false;

int activeLineIndexVeh = 0;
int activeSavedVehicleIndex = -1;
std::string activeSavedVehicleSlotName;
int lastKnownSavedVehicleCount = 0;
bool vehSaveMenuInterrupt = false;
bool vehSaveSlotMenuInterrupt = false;
bool requireRefreshOfSaveSlots = false;
bool requireRefreshOfSlotMenu = false;

//Door Options list + struct
struct struct_door_options {
	std::string text;
	bool *pState;
};

std::vector<struct_door_options> DOOR_OPTIONS = {
	{ "Toggle Open Instantly", &featureVehicleDoorInstant },
	{ "Front Right", NULL }, //INDEX 0
	{ "Front Left", NULL }, //INDEX 1
	{ "Rear Right", NULL }, //INDEX 2
	{ "Rear Left", NULL }, //INDEX 3 (This opens the ramp on the An-225|CARGOPLANE)
	{ "Hood", NULL }, //INDEX 4
	{ "Trunk ", NULL }, //INDEX 5 (Opens ramp on C-130|TITAN)
	{ "Trunk 2", NULL } //INDEX 6 (What uses this?)
};

int doorOptionsMenuIndex = 0;

//Top Level

const std::vector<std::string> MENU_VEHICLE_CATEGORIES{ "Cars", "Industrial", "Emergency and Military", "Motorcycles", "Planes", "Helicopters", "Boats", "Bicycles" };

//Cars

const std::vector<std::string> MENU_CAR_CATEGORIES{ "Supercars", "Sports", "Sport Classics", "Coupes", "Muscle", "Offroad", "SUVs", "Sedans", "Compacts" };

const std::vector<std::string> CAPTIONS_SUPERCARS{ "Coil Voltic", "Grotti Cheetah", "Grotti Turismo R", "Overflod Entity XF", "Pegassi Infernus", "Pegassi Vacca", "Pegassi Zentorno", "Truffade Adder", "Vapid Bullet" };

const std::vector<std::string> CAPTIONS_SPORTS{ "Albany Alpha", "Annis Elegy RH8", "Benefactor Feltzer", "Benefactor Schwartzer", "Benefactor Surano", "Bravado Banshee", "Bravado Buffalo", "Bravado Buffalo S", "Bravado Buffalo S (Race)", "Dewbauchee Massacro", "Dewbauchee Massacro (Race)", "Dewbauchee Rapid GT", "Dewbauchee Rapid GT Cabrio", "Dinka Blista Compact", "Dinka Blista Compact (Go Go Monkey Race)", "Dinka Jester", "Dinka Jester (Race)", "Grotti Carbonizzare", "Hijak Khamelion", "Invetero Coquette", "Karin Futo", "Karin Kuruma", "Karin Kuruma (Armoured)", "Karin Sultan", "Lampadati Furore GT", "Maibatsu Penumbra", "Obey 9F", "Obey 9F Cabrio", "Phister Comet", "Schyster Fusilade" };

const std::vector<std::string> CAPTIONS_SPORTCLASSICS{ "Albany Manana", "Albany Roosevelt", "Declasse Tornado", "Declasse Tornado (Rusty)", "Declasse Tornado Cabrio", "Declasse Tornado Cabrio (Rusty)", "Dewbauchee JB 700", "Grotti Stinger", "Grotti Stinger GT", "Invetero Coquette Classic", "Lampadati Casco", "Lampadati Pigalle", "Pegassi Monroe", "Truffade Z-Type", "Vapid Peyote" };

const std::vector<std::string> CAPTIONS_COUPES{ "Dewbauchee Exemplar", "Enus Cognoscenti Cabrio", "Lampadati Felon", "Lampadati Felon GT", "Ocelot F620", "Ocelot Jackal", "Ubermacht Sentinel", "Ubermacht Sentinel XS", "Ubermacht Zion", "Ubermacht Zion Cabrio" };

const std::vector<std::string> CAPTIONS_MUSCLE{ "Albany Buccaneer", "Bravado Gauntlet", "Bravado Gauntlet (Race)", "Cheval Picador", "Declasse Sabre Turbo", "Declasse Stallion", "Declasse Stallion (Race)", "Declasse Vigero", "Declasse Voodoo", "Imponte Duke O' Death", "Imponte Dukes", "Imponte Phoenix", "Imponte Ruiner", "Vapid Blade", "Vapid Dominator", "Vapid Dominator (Race)", "Vapid Hotknife", "Vapid Slamvan", "Vapid Slamvan (Lost MC)" };

const std::vector<std::string> CAPTIONS_OFFROAD{ "Benefactor Dubsta 6x6", "BF Bifta", "BF Injection", "Bravado Dune", "Bravado Duneloader", "Bravado Space Docker", "Canis Bodhi", "Canis Kalahari", "Canis Mesa (Off-Road)", "Cheval Marshall", "Declasse Rancher XL", "Declasse Rancher XL (Snow)", "Insurgent", "Insurgent (Gun Mount)", "Karin Rebel", "Karin Rebel (Rusty)", "Karin Technical", "Nagasaki Blazer", "Nagasaki Blazer (Hot Rod)", "Nagasaki Blazer (Lifeguard)", "Vapid Guardian", "Vapid Sandking", "Vapid Sandking XL", "Vapid The Liberator" };

const std::vector<std::string> CAPTIONS_SUVS{ "Albany Cavalcade", "Albany Cavalcade Mk2", "Benefactor Dubsta", "Benefactor Dubsta (Flat Black)", "Benefactor Serrano", "Bravado Gresley", "Canis Mesa", "Canis Mesa (Snow)", "Canis Seminole", "Declasse Granger", "Dundreary Landstalker", "Emperor Habanero", "Enus Huntley S", "Fathom FQ 2", "Gallivanter Baller (Large)", "Gallivanter Baller (Small)", "Karin BeeJay XL", "Mammoth Patriot", "Obey Rocoto", "Vapid Radius" };

const std::vector<std::string> CAPTIONS_SEDANS{ "Albany Emperor", "Albany Emperor (Rusty)", "Albany Emperor (Snow)", "Albany Primo", "Albany Washington", "Benefactor Glendale", "Benefactor Schafter", "Chariot Romero Hearse", "Cheval Fugitive", "Cheval Surge", "Declasse Asea", "Declasse Asea (Snow)", "Declasse Premier", "Dundreary Regina", "Dundreary Stretch", "Enus Super Diamond", "Karin Asterope", "Karin Intruder", "Obey Tailgater", "Ubermacht Oracle", "Ubermacht Oracle Mk2", "Vapid Stanier", "Vapid Stanier (Taxi)", "Vulcan Ingot", "Vulcar Warrener", "Zirconium Stratum" };

const std::vector<std::string> CAPTIONS_COMPACTS{ "Benefactor Panto", "Bollokan Prairie", "Declasse Rhapsody", "Dinka Blista", "Karin Dilettante", "Karin Dilettante (FlyUS)", "Weeny Issi" };

const std::vector<std::string> VALUES_SUPERCARS{ "VOLTIC", "CHEETAH", "TURISMOR", "ENTITYXF", "INFERNUS", "VACCA", "ZENTORNO", "ADDER", "BULLET" };

const std::vector<std::string> VALUES_SPORTS{ "ALPHA", "ELEGY2", "FELTZER2", "SCHWARZER", "SURANO", "BANSHEE", "BUFFALO", "BUFFALO2", "BUFFALO3", "MASSACRO", "MASSACRO2", "RAPIDGT", "RAPIDGT2", "BLISTA2", "BLISTA3", "JESTER", "JESTER2", "CARBONIZZARE", "KHAMELION", "COQUETTE", "FUTO", "KURUMA", "KURUMA2", "SULTAN", "FUROREGT", "PENUMBRA", "NINEF", "NINEF2", "COMET2", "FUSILADE" };

const std::vector<std::string> VALUES_SPORTCLASSICS{ "MANANA", "BTYPE", "TORNADO", "TORNADO3", "TORNADO2", "TORNADO4", "JB700", "STINGER", "STINGERGT", "COQUETTE2", "CASCO", "PIGALLE", "MONROE", "ZTYPE", "PEYOTE" };

const std::vector<std::string> VALUES_COUPES{ "EXEMPLAR", "COGCABRIO", "FELON", "FELON2", "F620", "JACKAL", "SENTINEL", "SENTINEL2", "ZION", "ZION2" };

const std::vector<std::string> VALUES_MUSCLE{ "BUCCANEER", "GAUNTLET", "GAUNTLET2", "PICADOR", "SABREGT", "STALION", "STALION2", "VIGERO", "VOODOO2", "DUKES2", "DUKES", "PHOENIX", "RUINER", "BLADE", "DOMINATOR", "DOMINATOR2", "HOTKNIFE", "SLAMVAN", "SLAMVAN2" };

const std::vector<std::string> VALUES_OFFROAD{ "DUBSTA3", "BIFTA", "BFINJECTION", "DUNE", "DLOADER", "DUNE2", "BODHI2", "KALAHARI", "MESA3", "MARSHALL", "RANCHERXL", "RANCHERXL2", "INSURGENT2", "INSURGENT", "REBEL2", "REBEL", "TECHNICAL", "BLAZER", "BLAZER3", "BLAZER2", "GUARDIAN", "SANDKING2", "SANDKING", "MONSTER" };

const std::vector<std::string> VALUES_SUVS{ "CAVALCADE", "CAVALCADE2", "DUBSTA", "DUBSTA2", "SERRANO", "GRESLEY", "MESA", "MESA2", "SEMINOLE", "GRANGER", "LANDSTALKER", "HABANERO", "HUNTLEY", "FQ2", "BALLER", "BALLER2", "BJXL", "PATRIOT", "ROCOTO", "RADI" };

const std::vector<std::string> VALUES_SEDANS{ "EMPEROR", "EMPEROR2", "EMPEROR3", "PRIMO", "WASHINGTON", "GLENDALE", "SCHAFTER2", "ROMERO", "FUGITIVE", "SURGE", "ASEA", "ASEA2", "PREMIER", "REGINA", "STRETCH", "SUPERD", "ASTEROPE", "INTRUDER", "TAILGATER", "ORACLE", "ORACLE2", "STANIER", "TAXI", "INGOT", "WARRENER", "STRATUM" };

const std::vector<std::string> VALUES_COMPACTS{ "PANTO", "PRAIRIE", "RHAPSODY", "BLISTA", "DILETTANTE", "DILETTANTE2", "ISSI2" };

const std::vector<std::string> VOV_CAR_CAPTIONS[] = { CAPTIONS_SUPERCARS, CAPTIONS_SPORTS, CAPTIONS_SPORTCLASSICS, CAPTIONS_COUPES, CAPTIONS_MUSCLE, CAPTIONS_OFFROAD, CAPTIONS_SUVS, CAPTIONS_SEDANS, CAPTIONS_COMPACTS };

const std::vector<std::string> VOV_CAR_VALUES[] = { VALUES_SUPERCARS, VALUES_SPORTS, VALUES_SPORTCLASSICS, VALUES_COUPES, VALUES_MUSCLE, VALUES_OFFROAD, VALUES_SUVS, VALUES_SEDANS, VALUES_COMPACTS };

//Industrial

const std::vector<std::string> MENU_INDUS_CATEGORIES{ "Pickups", "Vans", "Trucks", "Service/Utility", "Trailers", "Trains (Undriveable)" };

const std::vector<std::string> CAPTIONS_PICKUPS{ "Bravado Bison", "Bravado Bison (Backrack)", "Bravado Bison (Construction)", "Bravado Ratloader", "Bravado Ratloader (Rusty)", "Vapid Bobcat", "Vapid Sadler", "Vapid Sadler (Snow)" };

const std::vector<std::string> CAPTIONS_VANS{ "BF Surfer", "BF Surfer (Rusty)", "Bravado Paradise", "Bravado Rumpo (Deludamol)", "Bravado Rumpo (Weazel News)", "Bravado Youga", "Brute Camper", "Brute Pony (Business)", "Brute Pony (Cannibus Shop)", "Brute Taco Van", "Declasse Burrito", "Declasse Burrito (Bug Stars)", "Declasse Burrito (Construction)", "Declasse Burrito (Gang)", "Declasse Burrito (Graphics)", "Declasse Burrito (Snow)", "Declasse Burrito (The Lost)", "Vapid Minivan", "Vapid Speedo", "Vapid Speedo (Clown)", "Zirconium Journey" };

const std::vector<std::string> CAPTIONS_TRUCKS{ "Brute Boxville (Go Postal)", "Brute Boxville (Humane Labs)", "Brute Boxville (Post OP)", "Brute Boxville (Water & Power)", "Brute Stockade", "Brute Stockade (Snow)", "Brute Tipper (2 Axle)", "Brute Tipper (3 Axle)", "Cutter", "Dock Handler", "Dock Tug", "Dump Truck", "HVY Biff", "Jobuilt Hauler", "Jobuilt Phantom", "Jobuilt Rubble", "Maibatsu Mule (Graphics 1)", "Maibatsu Mule (Graphics 2)", "Maibatsu Mule (Plain)", "Mixer", "Mixer (Support Wheel)", "MTL Flatbed Truck", "MTL Packer", "MTL Pounder", "Vapid Benson", "Vapid Scrap Truck", "Vapid Tow Truck", "Vapid Tow Truck (Old)" };

const std::vector<std::string> CAPTIONS_SERVICES{ "Airtug", "Brute Airport Bus", "Brute Bus (City Bus)", "Brute Rental Shuttle Bus", "Brute Tourbus", "Cable Car (Mt. Chilliad)", "Dashound Dashound Coach", "Dozer", "Forklift", "Jobuilt Trashmaster", "Jobuilt Trashmaster (Rusty)", "Nagasaki Caddy", "Nagasaki Caddy (Golf)", "Ripley (Airport Tug)", "Stanley Fieldmaster Tractor", "Stanley Fieldmaster Tractor (Snow)", "Stanley Lawn Mower", "Stanley Tractor (Rusty)", "Vapid Pickup Utility", "Vapid Plumbing Utility", "Vapid Telephone Utility" };

const std::vector<std::string> CAPTIONS_TRAILERS{ "Army Flatbed Trailer (Empty)", "Army Flatbed Trailer (With Drill)", "Army Fuel Tanker", "Boat Trailer", "Boat Trailer", "Car Transport Trailer", "Car Transport Trailer (Empty)", "Commercial Trailer (Graphics 1)", "Commercial Trailer (Graphics 2)", "Container Trailer", "Fame or Shame Trailer", "Flatbed Trailer", "Flatbed Trailer", "Grain Trailer", "Hay Bale Trailer", "Logging Trailer", "Meth Lab Trailer", "Petrol Tanker Trailer (Plain)", "Petrol Tanker Trailer (RON)", "Plain Trailer", "Rake Trailer", "Small Trailer" };

const std::vector<std::string> CAPTIONS_TRAINS{ "Container Car 1", "Container Car 2", "Flatbed Car", "Freight Train Cab", "Grain Car", "Metro Train (Half)", "Oil Tanker Car" };

const std::vector<std::string> VALUES_PICKUPS{ "BISON", "BISON3", "BISON2", "RATLOADER2", "RATLOADER", "BOBCATXL", "SADLER", "SADLER2" };

const std::vector<std::string> VALUES_VANS{ "SURFER", "SURFER2", "PARADISE", "RUMPO2", "RUMPO", "YOUGA", "CAMPER", "PONY", "PONY2", "TACO", "BURRITO3", "BURRITO2", "BURRITO4", "GBURRITO2", "BURRITO", "BURRITO5", "GBURRITO", "MINIVAN", "SPEEDO", "SPEEDO2", "JOURNEY" };

const std::vector<std::string> VALUES_TRUCKS{ "BOXVILLE2", "BOXVILLE3", "BOXVILLE4", "BOXVILLE", "STOCKADE", "STOCKADE3", "TIPTRUCK", "TIPTRUCK2", "CUTTER", "HANDLER", "DOCKTUG", "DUMP", "BIFF", "HAULER", "PHANTOM", "RUBBLE", "MULE", "MULE2", "MULE3", "MIXER", "MIXER2", "FLATBED", "PACKER", "POUNDER", "BENSON", "SCRAP", "TOWTRUCK", "TOWTRUCK2" };

const std::vector<std::string> VALUES_SERVICES{ "AIRTUG", "AIRBUS", "BUS", "RENTALBUS", "TOURBUS", "CABLECAR", "COACH", "BULLDOZER", "FORKLIFT", "TRASH2", "TRASH", "CADDY2", "CADDY", "RIPLEY", "TRACTOR2", "TRACTOR3", "MOWER", "TRACTOR", "UTILLITRUCK3", "UTILLITRUCK2", "UTILLITRUCK" };

const std::vector<std::string> VALUES_TRAILERS{ "ARMYTRAILER", "ARMYTRAILER2", "ARMYTANKER", "BOATTRAILER", "TR3", "TR4", "TR2", "TRAILERS2", "TRAILERS3", "DOCKTRAILER", "TVTRAILER", "FREIGHTTRAILER", "TRFLAT", "GRAINTRAILER", "BALETRAILER", "TRAILERLOGS", "PROPTRAILER", "TANKER2", "TANKER", "TRAILERS", "RAKETRAILER", "TRAILERSMALL" };

const std::vector<std::string> VALUES_TRAINS{ "FREIGHTCONT1", "FREIGHTCONT2", "FREIGHTCAR", "FREIGHT", "FREIGHTGRAIN", "METROTRAIN", "TANKERCAR" };

const std::vector<std::string> VOV_INDUS_CAPTIONS[] = { CAPTIONS_PICKUPS, CAPTIONS_VANS, CAPTIONS_TRUCKS, CAPTIONS_SERVICES, CAPTIONS_TRAILERS, CAPTIONS_TRAINS };

const std::vector<std::string> VOV_INDUS_VALUES[] = { VALUES_PICKUPS, VALUES_VANS, VALUES_TRUCKS, VALUES_SERVICES, VALUES_TRAILERS, VALUES_TRAINS };

//Everything else

const std::vector<std::string> CAPTIONS_EMERGENCY{ "Albany Police Roadcruiser (Snow)", "Ambulance", "Army Barracks Truck", "Army Truck Cab", "Bravado Buffalo (FIB)", "Brute Police Riot Van", "Canis Crusader (Army Mesa)", "Declasse Granger (FIB)", "Declasse Lifeguard", "Declasse Park Ranger", "Declasse Police Rancher (Snow)", "Declasse Police Transporter", "Declasse Sheriff SUV", "Firetruck", "Prison Bus", "Rhino Tank", "Vapid Police Buffalo", "Vapid Police Cruiser", "Vapid Police Interceptor", "Vapid Sheriff Cruiser", "Vapid Unmarked Police Cruiser", "Western Police Bike" };

const std::vector<std::string> CAPTIONS_MOTORCYCLES{ "Dinka Akuma", "Dinka Double-T", "Dinka Enduro", "Dinka Thrust", "LCC Hexer", "LCC Innovation", "Maibatsu Sanchez", "Maibatsu Sanchez (Graphics)", "Nagasaki Carbon RS", "Pegassi Bati", "Pegassi Bati (Race)", "Pegassi Ruffian", "Principe Lectro", "Principe Nemesis", "Principe Phaggio", "Shitzu Hakuchou", "Shitzu PCJ 600", "Shitzu Vader", "Western Bagger", "Western Daemon", "Western Sovereign" };

const std::vector<std::string> CAPTIONS_PLANES{ "Buckingham Cargo Plane (An-225)", "Buckingham Jet (B747)", "Buckingham Luxor", "Buckingham Miljet", "Buckingham Shamal", "Buckingham Vestra", "Jobuilt Mammatus", "Jobuilt P-996 Lazer", "Jobuilt Velum (4 Seater)", "Jobuilt Velum (5 Seater)", "Mammoth Dodo", "Mammoth Hydra", "Mammoth Titan", "Western Besra", "Western Cuban 800", "Western Duster", "Western Mallard Stunt Plane" };

const std::vector<std::string> CAPTIONS_HELOS{ "Blimp (Atomic)", "Blimp (Xero Gas)", "Buckingham Savage", "Buckingham Swift", "Buckingham Valkyrie", "HVY Skylift", "Maibatsu Frogger", "Maibatsu Frogger (TPE/FIB)", "Nagasaki Buzzard (Unarmed)", "Nagasaki Buzzard Attack Chopper", "Western Annihilator", "Western Cargobob (Desert Camo)", "Western Cargobob (Jetsam)", "Western Cargobob (TPE)", "Western Maverick", "Western Maverick (Police)" };

const std::vector<std::string> CAPTIONS_BOATS{ "Dinka Marquis", "Kraken Sub", "Nagasaki Dinghy (2 Seater)", "Nagasaki Dinghy (4 Seater, Black)", "Nagasaki Dinghy (4 Seater, Red)", "Pegassi Speeder", "Shitzu Jetmax", "Shitzu Predator (Police)", "Shitzu Squalo", "Shitzu Suntrap", "Shitzu Tropic", "Speedophile Seashark", "Speedophile Seashark (Lifeguard)", "Submersible" };

const std::vector<std::string> CAPTIONS_BICYCLES{ "BMX", "Cruiser", "Endurex Race", "Fixter", "Scorcher", "Tri-Cycles Race", "Whippet Race" };

const std::vector<std::string> VALUES_EMERGENCY{ "POLICEOLD2", "AMBULANCE", "BARRACKS", "BARRACKS2", "FBI", "RIOT", "CRUSADER", "FBI2", "LGUARD", "PRANGER", "POLICEOLD1", "POLICET", "SHERIFF2", "FIRETRUK", "PBUS", "RHINO", "POLICE2", "POLICE", "POLICE3", "SHERIFF", "POLICE4", "POLICEB" };

const std::vector<std::string> VALUES_MOTORCYCLES{ "AKUMA", "DOUBLE", "ENDURO", "THRUST", "HEXER", "INNOVATION", "SANCHEZ2", "SANCHEZ", "CARBONRS", "BATI", "BATI2", "RUFFIAN", "LECTRO", "NEMESIS", "FAGGIO2", "HAKUCHOU", "PCJ", "VADER", "BAGGER", "DAEMON", "SOVEREIGN" };

const std::vector<std::string> VALUES_PLANES{ "CARGOPLANE", "JET", "LUXOR", "MILJET", "SHAMAL", "VESTRA", "MAMMATUS", "LAZER", "VELUM", "VELUM2", "DODO", "HYDRA", "TITAN", "BESRA", "CUBAN800", "DUSTER", "STUNT" };

const std::vector<std::string> VALUES_HELOS{ "BLIMP", "BLIMP2", "SAVAGE", "SWIFT", "VALKYRIE", "SKYLIFT", "FROGGER", "FROGGER2", "BUZZARD2", "BUZZARD", "ANNIHILATOR", "CARGOBOB", "CARGOBOB2", "CARGOBOB3", "MAVERICK", "POLMAV" };

const std::vector<std::string> VALUES_BOATS{ "MARQUIS", "SUBMERSIBLE2", "DINGHY2", "DINGHY3", "DINGHY", "SPEEDER", "JETMAX", "PREDATOR", "SQUALO", "SUNTRAP", "TROPIC", "SEASHARK", "SEASHARK2", "SUBMERSIBLE" };

const std::vector<std::string> VALUES_BICYCLES{ "BMX", "CRUISER", "TRIBIKE2", "FIXTER", "SCORCHER", "TRIBIKE3", "TRIBIKE" };

const std::vector<std::string> VOV_SHALLOW_CAPTIONS[] = { CAPTIONS_EMERGENCY, CAPTIONS_MOTORCYCLES, CAPTIONS_PLANES, CAPTIONS_HELOS, CAPTIONS_BOATS, CAPTIONS_BICYCLES };

const std::vector<std::string> VOV_SHALLOW_VALUES[] = { VALUES_EMERGENCY, VALUES_MOTORCYCLES, VALUES_PLANES, VALUES_HELOS, VALUES_BOATS, VALUES_BICYCLES };

bool onconfirm_vehdoor_menu(MenuItem<int> choice) {
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (choice.currentMenuIndex > 0) {
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			int value = choice.currentMenuIndex - 1;

			float doorAngle = VEHICLE::GET_VEHICLE_DOOR_ANGLE_RATIO(veh, value); //Best way I could figure out to detect if the part is animated.
			if (doorAngle < 0.01) {
				VEHICLE::SET_VEHICLE_DOOR_OPEN(veh, value, false, featureVehicleDoorInstant);
			}
			else {
				VEHICLE::SET_VEHICLE_DOOR_SHUT(veh, value, featureVehicleDoorInstant);
			}
		}
		else
		{
			set_status_text("Player isn't in a vehicle");
		}
	}
	return false;
}

bool process_veh_door_menu() {
	std::string caption = "Door Options";

	std::vector<MenuItem<int>*> menuItems;
	std::vector<int>menuIndexes;

	ToggleMenuItem<int> *immediateToggle = new ToggleMenuItem<int>();
	immediateToggle->value = 0;
	immediateToggle->caption = DOOR_OPTIONS[0].text;
	immediateToggle->toggleValue = DOOR_OPTIONS[0].pState;
	menuItems.push_back(immediateToggle);

	for (int i = 1; i < DOOR_OPTIONS.size(); i++) {

		MenuItem<int> *item = new MenuItem<int>();
		item->value = i;
		item->caption = DOOR_OPTIONS[i].text;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &doorOptionsMenuIndex, caption, onconfirm_vehdoor_menu, NULL, NULL);
}

void on_toggle_invincibility(MenuItem<int> choice)
{
	featureVehInvincibleUpdated = true;
}

bool onconfirm_veh_menu(MenuItem<int> choice)
{
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	switch (activeLineIndexVeh)
	{
	case 0:
		if (process_carspawn_menu()) return false;
		break;
	case 1:
	{
		if (process_savedveh_menu()) return false;
		break;
	}
		break;
	case 2: //fix
		if (bPlayerExists)
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

				VEHICLE::SET_VEHICLE_FIXED(veh);

				VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 1000.0f);
				VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 1000.0f);

				VEHICLE::SET_VEHICLE_UNDRIVEABLE(veh, false);
				VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(veh, false);

				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true);

				set_status_text("Vehicle Repaired");
			}
			else
				set_status_text("Player isn't in a vehicle");
		break;
	case 3: //clean
		if (bPlayerExists)
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(playerPed), 0);
			else
				set_status_text("Player isn't in a vehicle");
		break;
	case 4: //Replaced random paint with paint menu, not sure if random would still be a desired feature
		if (process_paint_menu()) return false;
		//if (bPlayerExists)
		//{
		//	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		//	{
		//		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		//		VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
		//		if (VEHICLE::_DOES_VEHICLE_HAVE_SECONDARY_COLOUR(veh))
		//			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
		//	}
		//	else
		//	{
		//		set_status_text("Player isn't in a vehicle");
		//	}
		//}
		break;

	case 9:
		if (process_vehmod_menu()) return false;
		break;
	case 10:
		if (process_veh_door_menu()) return false;
		break;
		// switchable features
	default:
		break;
	}
	return false;
}

void process_veh_menu()
{
	const int lineCount = 11;

	std::string caption = "Vehicle Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Vehicle Spawner", NULL, NULL, false },
		{ "Saved Vehicles", NULL, NULL, false },
		{ "Fix", NULL, NULL, true },
		{ "Clean", NULL, NULL, true },
		{ "Paint Menu", NULL, NULL, false },
		{ "Invincible", &featureVehInvincible, &featureVehInvincibleUpdated, true },
		{ "No Falling Off/Out", &featureNoVehFallOff, &featureNoVehFallOffUpdated, true },
		{ "Spawn Into Vehicle", &featureVehSpawnInto, NULL, true },
		{ "Speed Boost", &featureVehSpeedBoost, NULL, true },
		{ "Modifications", NULL, NULL, false },
		{ "Door Control", NULL, NULL, false }
	};
	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexVeh, caption, onconfirm_veh_menu);
}

void update_vehicle_features(BOOL bPlayerExists, Ped playerPed)
{
	// player's vehicle invincible
	if (featureVehInvincibleUpdated)
	{
		if (bPlayerExists && !featureVehInvincible && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, FALSE);
			ENTITY::SET_ENTITY_PROOFS(veh, 0, 0, 0, 0, 0, 0, 0, 0);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 1);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 1);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 1);
			for (int i = 0; i < 6; i++){
				VEHICLE::_SET_VEHICLE_DOOR_BREAKABLE(veh, i, TRUE); //(Vehicle, doorIndex, isBreakable)
			}
			featureVehInvincibleUpdated = false;
		}
	}

	if (featureVehInvincible)
	{
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, TRUE);
			ENTITY::SET_ENTITY_PROOFS(veh, 1, 1, 1, 1, 1, 1, 1, 1);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 0);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 0);
			for (int i = 0; i < 6; i++){
				VEHICLE::_SET_VEHICLE_DOOR_BREAKABLE(veh, i, FALSE); //(Vehicle, doorIndex, isBreakable)
			}
		}
	}

	// fall off
	if (bPlayerExists && featureNoVehFallOffUpdated && !featureNoVehFallOff)
	{
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPed, 0);
		featureNoVehFallOffUpdated = false;
	}
	else if (bPlayerExists && featureNoVehFallOff)
	{
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPed, 1);
	}

	if (bPlayerExists && featureNoVehFallOff)
	{
		if (PED::IS_PED_RAGDOLL(playerPed) && !PED::IS_PED_BEING_JACKED(playerPed) && lastSeenInVehicle)
		{
			PED::SET_PED_INTO_VEHICLE(playerPed, PED::GET_VEHICLE_PED_IS_IN(playerPed, true), -1);
			//set_status_text("That's Why We Wear Seatbelts!");
		}
		else if (!PED::IS_PED_RAGDOLL(playerPed))
		{
			bool curInVeh = PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) ? true : false;
			if (!curInVeh)
			{
				lastSeenInVehicle = false;
			}
			else if (PED::IS_PED_GETTING_INTO_A_VEHICLE(playerPed))
			{
				lastSeenInVehicle = true;
			}
		}
	}

	// player's vehicle boost
	if (featureVehSpeedBoost && bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		bool bUp = IsKeyDown(get_config()->get_key_config()->key_veh_boost);
		bool bDown = IsKeyDown(get_config()->get_key_config()->key_veh_stop);

		if (bUp || bDown)
		{
			float speed = ENTITY::GET_ENTITY_SPEED(veh);
			if (bUp)
			{
				speed += speed * 0.05f;
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
			}
			else
				if (ENTITY::IS_ENTITY_IN_AIR(veh) || speed > 5.0)
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0);
		}
	}
}

void reset_vehicle_globals()
{
	//veh_spawn_menu_index = 0;

	featureVehInvincible =
		featureVehSpeedBoost =
		featureVehicleDoorInstant =
		featureVehSpawnInto = false;

	featureVehInvincibleUpdated = true;
}

bool onconfirm_carspawn_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case 0:
		process_spawn_menu_cars();
		break;
	case 1:
		process_spawn_menu_indus();
		break;
	default:
		process_spawn_menu_generic(choice.value);
	}
	return false;
}

bool process_carspawn_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < MENU_VEHICLE_CATEGORIES.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_VEHICLE_CATEGORIES[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	/*
	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "Saved Vehicles";
	item->value = -1;
	item->isLeaf = false;
	menuItems.push_back(item);
	*/

	return draw_generic_menu<int>(menuItems, 0, "Vehicle Categories", onconfirm_carspawn_menu, NULL, NULL);
}

bool onconfirm_spawn_menu_cars(MenuItem<int> choice)
{
	std::string category = choice.caption;

	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_CAR_VALUES[choice.value].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_CAR_CAPTIONS[choice.value][i];
		item->value = VOV_CAR_VALUES[choice.value][i];
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, 0, category, onconfirm_spawn_menu_vehicle_selection, NULL, NULL);
}

bool process_spawn_menu_cars()
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < MENU_CAR_CATEGORIES.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_CAR_CATEGORIES[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Car Categories", onconfirm_spawn_menu_cars, NULL, NULL);
}

bool onconfirm_spawn_menu_indus(MenuItem<int> choice)
{
	int selection = choice.value;
	std::string category = choice.caption;

	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_INDUS_CAPTIONS[selection].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_INDUS_CAPTIONS[selection][i];
		item->value = VOV_INDUS_VALUES[selection][i];
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, 0, category, onconfirm_spawn_menu_vehicle_selection, NULL, NULL);
}

bool process_spawn_menu_indus()
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < MENU_INDUS_CATEGORIES.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_INDUS_CATEGORIES[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Industrial Categories", onconfirm_spawn_menu_indus, NULL, NULL);
}

bool onconfirm_spawn_menu_vehicle_selection(MenuItem<std::string> choice)
{
	do_spawn_vehicle(choice.value, choice.caption);
	return false;
}

bool process_spawn_menu_generic(int topMenuSelection)
{
	int selection = topMenuSelection - 2;

	std::string category = MENU_VEHICLE_CATEGORIES[topMenuSelection];

	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_SHALLOW_CAPTIONS[selection].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_SHALLOW_CAPTIONS[selection][i];
		item->value = VOV_SHALLOW_VALUES[selection][i];
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, 0, category, onconfirm_spawn_menu_vehicle_selection, NULL, NULL);
}

bool do_spawn_vehicle(std::string modelName, std::string modelTitle)
{
	DWORD model = GAMEPLAY::GET_HASH_KEY((char *)modelName.c_str());
	Vehicle veh = do_spawn_vehicle(model, modelTitle, true);
	if (veh != -1)
	{
		return true;
	}
	return false;
}

Vehicle do_spawn_vehicle(DWORD model, std::string modelTitle, bool cleanup)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
		FLOAT lookDir = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, lookDir, 1, 0);

		//		if (!VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh)) || !ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()))
		//		{
		//			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh); //returns BOOL of Vehicle on ground status.
		//		}

		if (featureVehSpawnInto)
		{
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
			if (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh)))
			{
				VEHICLE::SET_HELI_BLADES_FULL_SPEED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
			}
			lastSeenInVehicle = true;
		}

		WAIT(0);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		if (cleanup)
		{
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
		}

		std::ostringstream ss;
		ss << modelTitle << " spawned";
		set_status_text(ss.str());

		return veh;
	}
	return -1;
}


std::vector<FeatureEnabledLocalDefinition> get_feature_enablements_vehicles()
{
	std::vector<FeatureEnabledLocalDefinition> results;
	results.push_back(FeatureEnabledLocalDefinition{ "featureNoVehFallOff", &featureNoVehFallOff, &featureNoVehFallOffUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featureVehicleDoorInstant", &featureVehicleDoorInstant });
	results.push_back(FeatureEnabledLocalDefinition{ "featureVehInvincible", &featureVehInvincible, &featureVehInvincibleUpdated });
	results.push_back(FeatureEnabledLocalDefinition{ "featureVehSpawnInto", &featureVehSpawnInto });
	results.push_back(FeatureEnabledLocalDefinition{ "featureVehSpeedBoost", &featureVehSpeedBoost });
	return results;
}

bool spawn_saved_car(int slot, std::string caption)
{
	ENTDatabase* database = get_database();

	std::vector<SavedVehicleDBRow*> savedVehs = database->get_saved_vehicles(slot);

	lastKnownSavedVehicleCount = savedVehs.size();

	SavedVehicleDBRow* savedVeh = savedVehs.at(0);
	database->populate_saved_vehicle(savedVeh);

	Vehicle veh = do_spawn_vehicle(savedVeh->model, caption, false);
	if (veh == -1)
	{
		set_status_text("Spawn failed");
	}
	else
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, (savedVeh->burstableTyres == 1) ? TRUE : FALSE);
		
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, savedVeh->plateType);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, (char*) savedVeh->plateText.c_str());

		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, savedVeh->windowTint);

		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, savedVeh->wheelType);

		for each (SavedVehicleExtraDBRow *extra in savedVeh->extras)
		{
			VEHICLE::SET_VEHICLE_EXTRA(veh, extra->extraID, (extra->extraState == 1) ? 0 : -1);
		}

		for each (SavedVehicleModDBRow *mod in savedVeh->mods)
		{
			if (mod->isToggle)
			{
				VEHICLE::TOGGLE_VEHICLE_MOD(veh, mod->modID, mod->modState);
			}
			else
			{
				VEHICLE::SET_VEHICLE_MOD(veh, mod->modID, mod->modState, 0);
			}
		}

		if (savedVeh->livery != -1)
		{
			VEHICLE::SET_VEHICLE_LIVERY(veh, savedVeh->livery);
		}

		VEHICLE::SET_VEHICLE_COLOURS(veh, savedVeh->colourPrimary, savedVeh->colourSecondary);
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, savedVeh->colourExtraPearl, savedVeh->colourExtraWheel);
		VEHICLE::SET_VEHICLE_MOD_COLOR_1(veh, savedVeh->colourMod1Type, savedVeh->colourMod1Colour, savedVeh->colourMod1P3);
		VEHICLE::SET_VEHICLE_MOD_COLOR_2(veh, savedVeh->colourMod2Type, savedVeh->colourMod2Colour);

		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
	}

	for (std::vector<SavedVehicleDBRow*>::iterator it = savedVehs.begin(); it != savedVehs.end(); ++it)
	{
		delete (*it);
	}
	savedVehs.clear();

	return false;
}

bool onconfirm_savedveh_slot_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case 1: //spawn
		spawn_saved_car(activeSavedVehicleIndex, activeSavedVehicleSlotName);
		break;
	case 2: //overwrite
		{
			save_current_vehicle(activeSavedVehicleIndex);
			requireRefreshOfSaveSlots = true;
			requireRefreshOfSlotMenu = true;
			vehSaveSlotMenuInterrupt = true;
			vehSaveMenuInterrupt = true;
		}
		break;
	case 3: //rename
		{
			std::string result = show_keyboard(NULL, (char*)activeSavedVehicleSlotName.c_str());
			if (!result.empty())
			{
				ENTDatabase* database = get_database();
				database->rename_saved_vehicle(result, activeSavedVehicleIndex);

				activeSavedVehicleSlotName = result;
			}
			requireRefreshOfSaveSlots = true;
			requireRefreshOfSlotMenu = true;
			vehSaveSlotMenuInterrupt = true;
			vehSaveMenuInterrupt = true;
		}
		break;
	case 4: //delete
		{
			ENTDatabase* database = get_database();
			database->delete_saved_vehicle(activeSavedVehicleIndex);

			requireRefreshOfSlotMenu = false;
			requireRefreshOfSaveSlots = true;
			vehSaveSlotMenuInterrupt = true;
			vehSaveMenuInterrupt = true;
		}
		break;
	}
	return false;
}

bool process_savedveh_slot_menu(int slot)
{
	do
	{
		vehSaveSlotMenuInterrupt = false;
		requireRefreshOfSlotMenu = false;

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = 1;
		item->caption = "Spawn";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = 2;
		item->caption = "Overwrite With Current";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = 3;
		item->caption = "Rename";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = 4;
		item->caption = "Delete";
		menuItems.push_back(item);

		draw_generic_menu<int>(menuItems, 0, activeSavedVehicleSlotName, onconfirm_savedveh_slot_menu, NULL, NULL, vehicle_save_slot_menu_interrupt);
	} while (requireRefreshOfSlotMenu);
	return false;
}

void save_current_vehicle(int slot)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

			std::ostringstream ss;
			if (slot != -1)
			{
				ss << activeSavedVehicleSlotName;
			}
			else
			{
				ss << "Saved Vehicle "<< (lastKnownSavedVehicleCount+1);
			}

			auto existingText = ss.str();
			std::string result = show_keyboard(NULL, (char*)existingText.c_str());
			if (!result.empty())
			{
				ENTDatabase* database = get_database();
				if (database->save_vehicle(veh, result, slot))
				{
					set_status_text("Saved Vehicle");
					activeSavedVehicleSlotName = result;
				}
				else
				{
					set_status_text("Save Error");
				}
			}
		}
		else
		{
			set_status_text("Player isn't in a vehicle");
		}
	}
}

bool onconfirm_savedveh_menu(MenuItem<int> choice)
{
	if ( choice.value == -1 )
	{
		save_current_vehicle(-1);
		requireRefreshOfSaveSlots = true;
		vehSaveMenuInterrupt = true;
		return false;
	}

	activeSavedVehicleIndex = choice.value;
	activeSavedVehicleSlotName = choice.caption;
	return process_savedveh_slot_menu(choice.value);
}

bool process_savedveh_menu()
{
	do
	{
		vehSaveMenuInterrupt = false;
		requireRefreshOfSlotMenu = false;
		requireRefreshOfSaveSlots = false;

		ENTDatabase* database = get_database();
		std::vector<SavedVehicleDBRow*> savedVehs = database->get_saved_vehicles();

		lastKnownSavedVehicleCount = savedVehs.size();

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = -1;
		item->caption = "Create New Vehicle Save";
		menuItems.push_back(item);

		for each (SavedVehicleDBRow *sv in savedVehs)
		{
			MenuItem<int> *item = new MenuItem<int>();
			item->isLeaf = false;
			item->value = sv->rowID;
			item->caption = sv->saveName;
			menuItems.push_back(item);
		}

		draw_generic_menu<int>(menuItems, 0, "Saved Vehicles", onconfirm_savedveh_menu, NULL, NULL, vehicle_save_menu_interrupt);

		for (std::vector<SavedVehicleDBRow*>::iterator it = savedVehs.begin(); it != savedVehs.end(); ++it)
		{
			delete (*it);
		}
		savedVehs.clear();
	}
	while (requireRefreshOfSaveSlots);

	return false;
}

bool vehicle_save_menu_interrupt()
{
	if (vehSaveMenuInterrupt)
	{
		vehSaveMenuInterrupt = false;
		return true;
	}
	return false;
}

bool vehicle_save_slot_menu_interrupt()
{
	if (vehSaveSlotMenuInterrupt)
	{
		vehSaveSlotMenuInterrupt = false;
		return true;
	}
	return false;
}