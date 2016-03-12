/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "..\..\resource.h"
#include "..\utils.h"
#include "vehicles.h"
#include "script.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"
#include "..\debug\debuglog.h"

bool featureVehInvincible = false;
bool featureVehInvincibleUpdated = false;

bool featureVehNoDamage = false;

bool featureVehInvulnIncludesCosmetic = false;

bool featureNoVehFallOff = false;
bool featureNoVehFallOffUpdated = false;
bool featureVehSpeedBoost = false;
bool featureVehSpawnInto = false;
bool featureVehSpawnTuned = false;
bool featureVehSpawnOptic = false;
bool featureVehicleDoorInstant = false;
bool featureWearHelmetOff = false;
bool featureWearHelmetOffUpdated = false;

int activeLineIndexVeh = 0;
int activeSavedVehicleIndex = -1;
std::string activeSavedVehicleSlotName;
int lastKnownSavedVehicleCount = 0;
bool vehSaveMenuInterrupt = false;
bool vehSaveSlotMenuInterrupt = false;
bool requireRefreshOfVehSaveSlots = false;
bool requireRefreshOfVehSlotMenu = false;

const int PED_FLAG_THROUGH_WINDSCREEN = 32;

const std::vector<std::string> VEH_INVINC_MODE_CAPTIONS{ "OFF", "Mech. Only", "Mech. + Visual", "Mech. + Vis. + Cosmetic" };
const std::vector<int> VEH_INVINC_MODE_VALUES{ 0, 1, 2, 3 };

// engine power stuff
const std::vector<std::string> VEH_ENG_POW_CAPTIONS{ "1x", "5x", "10x", "25x", "50x", "75x", "100x", "125x", "150x", "175x", "200x", "225x", "250x", "275x", "300x", "325x", "350x", "375x", "400x" };
const std::vector<int> VEH_ENG_POW_VALUES{ 0, 5, 10, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400 };
int engPowMultIndex = 0;
bool powChanged = true;

//vehicle mass stuff
const std::vector<std::string> VEH_MASS_CAPTIONS{ "1x", "5x", "10x", "25x", "50x", "75x", "100x", "125x", "150x", "175x", "200x", "225x", "250x", "275x", "300x", "325x", "350x", "375x", "400x" };
const std::vector<int> VEH_MASS_VALUES{ 0, 5, 10, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400 };
int VehMassMultIndex = 0;
bool massChanged = true;


// player in vehicle state... assume true initially since our quicksave might have us in a vehicle already, in which case we can't check if we just got into one
bool oldVehicleState = true;

//Door Options list + struct
struct struct_door_options {
	std::string text;
	bool *pState;
};

int doorOptionsMenuIndex = 0;

//Top Level

const std::vector<std::string> MENU_VEHICLE_CATEGORIES{ "Cars", "Industrial", "Emergency and Military", "Motorcycles", "Planes", "Helicopters", "Boats", "Bicycles", "Enter Name Manually" };

//Cars

const std::vector<std::string> MENU_CAR_CATEGORIES{ "Supercars", "Sports", "Sport Classics", "Coupes", "Muscle", "Offroad", "SUVs", "Sedans", "Compacts", "Lowriders" };

const std::vector<std::string> CAPTIONS_SUPERCARS{ "Coil Voltic", "Grotti Cheetah", "Grotti Turismo R", "Overflod Entity XF", "Pegassi Infernus", "Pegassi Osiris", "Pegassi Vacca", "Pegassi Zentorno", "Progen T20", "Truffade Adder", "Vapid Bullet" };

const std::vector<std::string> CAPTIONS_SPORTS{ "Albany Alpha", "Annis Elegy RH8", "Benefactor Feltzer", "Benefactor Schwartzer", "Benefactor Surano", "Bravado Banshee", "Bravado Banshee 900R", "Bravado Buffalo", "Bravado Buffalo S", "Bravado Buffalo S (Race)", "Bravado Verlier", "Dewbauchee Massacro", "Dewbauchee Massacro (Race)", "Dewbauchee Rapid GT", "Dewbauchee Rapid GT Cabrio", "Dinka Blista Compact", "Dinka Blista Compact (Go Go Monkey Race)", "Dinka Jester", "Dinka Jester (Race)", "Grotti Carbonizzare", "Hijak Khamelion", "Invetero Coquette", "Karin Futo", "Karin Kuruma", "Karin Kuruma (Armoured)", "Karin Sultan", "Karin Sultan RS", "Lampadati Furore GT", "Maibatsu Penumbra", "Obey 9F", "Obey 9F Cabrio", "Phister Comet", "Schyster Fusilade" };

const std::vector<std::string> CAPTIONS_SPORTCLASSICS{ "Albany Franken Strange", "Albany Manana", "Albany Roosevelt", "Albany Roosevelt Valor", "Benefactor Stirling GT", "Declasse Mamba", "Declasse Tornado", "Declasse Tornado (Rusty)", "Declasse Tornado Cabrio", "Declasse Tornado Cabrio (Rusty)", "Dewbauchee JB 700", "Grotti Stinger", "Grotti Stinger GT", "Invetero Coquette Classic", "Lampadati Casco", "Lampadati Pigalle", "Pegassi Monroe", "Truffade Z-Type", "Vapid Peyote" };

const std::vector<std::string> CAPTIONS_COUPES{ "Dewbauchee Exemplar", "Enus Cognoscenti Cabrio", "Enus Cognoscenti Coupe", "Enus Cognoscenti Sport", "Enus Windsor", "Lampadati Felon", "Lampadati Felon GT", "Ocelot F620", "Ocelot Jackal", "Ubermacht Sentinel", "Ubermacht Sentinel XS", "Ubermacht Zion", "Ubermacht Zion Cabrio" };

const std::vector<std::string> CAPTIONS_MUSCLE{ "Albany Buccaneer", "Albany Lurcher", "Albany Virgo", "Bravado Gauntlet", "Bravado Gauntlet (Race)", "Cheval Picador", "Declasse Moonbeam", "Declasse Sabre Turbo", "Declasse Stallion", "Declasse Stallion (Race)", "Declasse Tampa", "Declasse Vigero", "Declasse Voodoo", "Imponte Duke O' Death", "Imponte Dukes", "Imponte Nightshade", "Imponte Phoenix", "Imponte Ruiner", "Invetero Coquette BlackFin", "Vapid Blade", "Vapid Chino", "Vapid Dominator", "Vapid Dominator (Race)", "Vapid Hotknife", "Vapid Slamvan", "Vapid Slamvan (Lost MC)", "Willard Faction" };

const std::vector<std::string> CAPTIONS_OFFROAD{ "Benefactor Dubsta 6x6", "BF Bifta", "BF Injection", "Bravado Dune", "Bravado Duneloader", "Bravado Space Docker", "Canis Bodhi", "Canis Kalahari", "Canis Mesa (Off-Road)", "Cheval Marshall", "Coil Brawler", "Declasse Rancher XL", "Declasse Rancher XL (Snow)", "Insurgent", "Insurgent (Gun Mount)", "Karin Rebel", "Karin Rebel (Rusty)", "Karin Technical", "Nagasaki Blazer", "Nagasaki Blazer (Hot Rod)", "Nagasaki Blazer (Lifeguard)", "Vapid Guardian", "Vapid Sandking", "Vapid Sandking XL", "Vapid The Liberator" };

const std::vector<std::string> CAPTIONS_SUVS{ "Albany Cavalcade", "Albany Cavalcade Mk2", "Benefactor Dubsta", "Benefactor Dubsta (Flat Black)", "Benefactor Serrano", "Bravado Gresley", "Canis Mesa", "Canis Mesa (Snow)", "Canis Seminole", "Declasse Granger", "Dundreary Landstalker", "Emperor Habanero", "Enus Huntley S", "Fathom FQ 2", "Gallivanter Baller (Large)", "Gallivanter Baller (Small)", "Gallivanter Baller (Large) LWB", "Gallivanter Baller (Small) LWB", "Gallivanter Baller LE (Armoured)", "Gallivanter Baller LE LWB (Armoured)", "Karin BeeJay XL", "Mammoth Patriot", "Obey Rocoto", "Vapid Radius" };

const std::vector<std::string> CAPTIONS_SEDANS{ "Albany Emperor", "Albany Emperor (Rusty)", "Albany Emperor (Snow)", "Albany Primo", "Albany Washington", "Benefactor Glendale", "Benefactor Schafter", "Benefactor Schafter LWB", "Benefactor Schafter Sport", "Benefactor Schafter V12", "Benefactor Schafter V12 Armoured", "Chariot Romero Hearse", "Cheval Fugitive", "Cheval Surge", "Declasse Asea", "Declasse Asea (Snow)", "Declasse Premier", "Dundreary Regina", "Dundreary Stretch", "Enus Super Diamond", "Karin Asterope", "Karin Intruder", "Obey Tailgater", "Ubermacht Oracle", "Ubermacht Oracle Mk2", "Vapid Stanier", "Vapid Stanier (Taxi)", "Vulcan Ingot", "Vulcar Warrener", "Zirconium Stratum" };

const std::vector<std::string> CAPTIONS_COMPACTS{ "Benefactor Panto", "Bollokan Prairie", "Declasse Rhapsody", "Dinka Blista", "Karin Dilettante", "Karin Dilettante (Liveried)", "Weeny Issi" };

const std::vector<std::string> CAPTIONS_LOWRIDERS{ "Albany Buccaneer (Custom)", "Albany Primo (Custom)", "Declasse Moonbeam (Custom)", "Declasse Voodoo (Custom)", "Vapid Chino (Custom)", "Willard Faction (Custom)" };

const std::vector<std::string> VALUES_SUPERCARS{ "VOLTIC", "CHEETAH", "TURISMOR", "ENTITYXF", "INFERNUS", "OSIRIS", "VACCA", "ZENTORNO", "T20", "ADDER", "BULLET" };

const std::vector<std::string> VALUES_SPORTS{ "ALPHA", "ELEGY2", "FELTZER2", "SCHWARZER", "SURANO", "BANSHEE", "BANSHEE2", "BUFFALO", "BUFFALO2", "BUFFALO3","VERLIER", "MASSACRO", "MASSACRO2", "RAPIDGT", "RAPIDGT2", "BLISTA2", "BLISTA3", "JESTER", "JESTER2", "CARBONIZZARE", "KHAMELION", "COQUETTE", "FUTO", "KURUMA", "KURUMA2", "SULTAN", "SULTANRS", "FUROREGT", "PENUMBRA", "NINEF", "NINEF2", "COMET2", "FUSILADE" };

const std::vector<std::string> VALUES_SPORTCLASSICS{ "BTYPE2", "MANANA", "BTYPE", "BTYPE3", "FELTZER3", "MAMBA", "TORNADO", "TORNADO3", "TORNADO2", "TORNADO4", "JB700", "STINGER", "STINGERGT", "COQUETTE2", "CASCO", "PIGALLE", "MONROE", "ZTYPE", "PEYOTE" };

const std::vector<std::string> VALUES_COUPES{ "EXEMPLAR", "COGCABRIO", "COG55", "COGNOSC", "WINDSOR", "FELON", "FELON2", "F620", "JACKAL", "SENTINEL2", "SENTINEL", "ZION", "ZION2" };

const std::vector<std::string> VALUES_MUSCLE{ "BUCCANEER", "LURCHER", "VIRGO", "GAUNTLET", "GAUNTLET2", "PICADOR", "MOONBEAM", "SABREGT", "STALION", "STALION2", "TAMPA", "VIGERO", "VOODOO2", "DUKES2", "DUKES", "NITESHAD", "PHOENIX", "RUINER", "COQUETTE3", "BLADE", "CHINO", "DOMINATOR", "DOMINATOR2", "HOTKNIFE", "SLAMVAN", "SLAMVAN2", "FACTION" };

const std::vector<std::string> VALUES_OFFROAD{ "DUBSTA3", "BIFTA", "BFINJECTION", "DUNE", "DLOADER", "DUNE2", "BODHI2", "KALAHARI", "MESA3", "MARSHALL", "BRAWLER", "RANCHERXL", "RANCHERXL2", "INSURGENT2", "INSURGENT", "REBEL2", "REBEL", "TECHNICAL", "BLAZER", "BLAZER3", "BLAZER2", "GUARDIAN", "SANDKING2", "SANDKING", "MONSTER" };

const std::vector<std::string> VALUES_SUVS{ "CAVALCADE", "CAVALCADE2", "DUBSTA", "DUBSTA2", "SERRANO", "GRESLEY", "MESA", "MESA2", "SEMINOLE", "GRANGER", "LANDSTALKER", "HABANERO", "HUNTLEY", "FQ2", "BALLER", "BALLER2", "BALLER3", "BALLER4", "BALLER5", "BALLER6", "BJXL", "PATRIOT", "ROCOTO", "RADI" };

const std::vector<std::string> VALUES_SEDANS{ "EMPEROR", "EMPEROR2", "EMPEROR3", "PRIMO", "WASHINGTON", "GLENDALE", "SCHAFTER2", "SCHAFTER5", "SCHAFTER3", "SCHAFTER4", "SCHAFTER6", "ROMERO", "FUGITIVE", "SURGE", "ASEA", "ASEA2", "PREMIER", "REGINA", "STRETCH", "SUPERD", "ASTEROPE", "INTRUDER", "TAILGATER", "ORACLE", "ORACLE2", "STANIER", "TAXI", "INGOT", "WARRENER", "STRATUM" };

const std::vector<std::string> VALUES_COMPACTS{ "PANTO", "PRAIRIE", "RHAPSODY", "BLISTA", "DILETTANTE", "DILETTANTE2", "ISSI2" };

const std::vector<std::string> VALUES_LOWRIDERS{ "BUCCANEER2", "PRIMO2", "MOONBEAM2", "VOODOO2", "CHINO2", "FACTION2" };

const std::vector<std::string> VOV_CAR_CAPTIONS[] = { CAPTIONS_SUPERCARS, CAPTIONS_SPORTS, CAPTIONS_SPORTCLASSICS, CAPTIONS_COUPES, CAPTIONS_MUSCLE, CAPTIONS_OFFROAD, CAPTIONS_SUVS, CAPTIONS_SEDANS, CAPTIONS_COMPACTS, CAPTIONS_LOWRIDERS };

const std::vector<std::string> VOV_CAR_VALUES[] = { VALUES_SUPERCARS, VALUES_SPORTS, VALUES_SPORTCLASSICS, VALUES_COUPES, VALUES_MUSCLE, VALUES_OFFROAD, VALUES_SUVS, VALUES_SEDANS, VALUES_COMPACTS, VALUES_LOWRIDERS };

//Industrial

const std::vector<std::string> MENU_INDUS_CATEGORIES{ "Pickups", "Vans", "Trucks", "Service/Utility", "Trailers", "Trains (Undriveable)" };

const std::vector<std::string> CAPTIONS_PICKUPS{ "Bravado Bison", "Bravado Bison (Backrack)", "Bravado Bison (Construction)", "Bravado Ratloader", "Bravado Ratloader (Rusty)", "Vapid Bobcat", "Vapid Sadler", "Vapid Sadler (Snow)" };

const std::vector<std::string> CAPTIONS_VANS{ "BF Surfer", "BF Surfer (Rusty)", "Bravado Paradise", "Bravado Rumpo (Deludamol)", "Bravado Rumpo (Plain/Weazel)", "Bravado Youga", "Brute Camper", "Brute Pony (Business)", "Brute Pony (Cannibus Shop)", "Brute Taco Van", "Declasse Burrito", "Declasse Burrito (Bug Stars)", "Declasse Burrito (Construction)", "Declasse Burrito (Gang)", "Declasse Burrito (Graphics)", "Declasse Burrito (Snow)", "Declasse Burrito (The Lost)", "Vapid Minivan", "Vapid Speedo", "Vapid Speedo (Clown)", "Zirconium Journey" };

const std::vector<std::string> CAPTIONS_TRUCKS{ "Brute Boxville (Go Postal)", "Brute Boxville (Humane Labs)", "Brute Boxville (Post OP)", "Brute Boxville (Water & Power)", "Brute Stockade", "Brute Stockade (Snow)", "Brute Tipper (2 Axle)", "Brute Tipper (3 Axle)", "Cutter", "Dock Handler", "Dock Tug", "Dump Truck", "HVY Biff", "Jobuilt Hauler", "Jobuilt Phantom", "Jobuilt Rubble", "Maibatsu Mule (Graphics 1)", "Maibatsu Mule (Graphics 2)", "Maibatsu Mule (Plain)", "Mixer", "Mixer (Support Wheel)", "MTL Flatbed Truck", "MTL Packer", "MTL Pounder", "Vapid Benson", "Vapid Scrap Truck", "Vapid Tow Truck", "Vapid Tow Truck (Old)" };

const std::vector<std::string> CAPTIONS_SERVICES{ "Airtug", "Brute Airport Bus", "Brute Bus (City Bus)", "Brute Rental Shuttle Bus", "Brute Tourbus", "Cable Car (Mt. Chilliad)", "Dashound Coach", "Dozer", "Forklift", "Jobuilt Trashmaster", "Jobuilt Trashmaster (Rusty)", "Nagasaki Caddy (New)", "Nagasaki Caddy (Old)", "Ripley (Airport Tug)", "Stanley Fieldmaster Tractor", "Stanley Fieldmaster Tractor (Snow)", "Stanley Lawn Mower", "Stanley Tractor (Rusty)", "Vapid Pickup Utility", "Vapid Plumbing Utility", "Vapid Telephone Utility" };

const std::vector<std::string> CAPTIONS_TRAILERS{ "Army Flatbed Trailer (Empty)", "Army Flatbed Trailer (With Drill)", "Army Fuel Tanker", "Boat Trailer", "Boat Trailer (With Boat)", "Car Transport Trailer", "Car Transport Trailer (Empty)", "Commercial Trailer (Graphics 1)", "Commercial Trailer (Graphics 2)", "Container Trailer", "Fame or Shame Trailer", "Flatbed Trailer 1", "Flatbed Trailer 2", "Grain Trailer", "Hay Bale Trailer", "Logging Trailer", "Meth Lab Trailer", "Petrol Tanker Trailer (Plain)", "Petrol Tanker Trailer (RON)", "Plain Trailer", "Rake Trailer", "Small Trailer" };

const std::vector<std::string> CAPTIONS_TRAINS{ "Container Car 1", "Container Car 2", "Flatbed Car", "Freight Train Cab", "Grain Car", "Metro Train (Half)", "Oil Tanker Car" };

const std::vector<std::string> VALUES_PICKUPS{ "BISON", "BISON3", "BISON2", "RATLOADER2", "RATLOADER", "BOBCATXL", "SADLER", "SADLER2" };

const std::vector<std::string> VALUES_VANS{ "SURFER", "SURFER2", "PARADISE", "RUMPO2", "RUMPO", "YOUGA", "CAMPER", "PONY", "PONY2", "TACO", "BURRITO3", "BURRITO2", "BURRITO4", "GBURRITO2", "BURRITO", "BURRITO5", "GBURRITO", "MINIVAN", "SPEEDO", "SPEEDO2", "JOURNEY" };

const std::vector<std::string> VALUES_TRUCKS{ "BOXVILLE2", "BOXVILLE3", "BOXVILLE4", "BOXVILLE", "STOCKADE", "STOCKADE3", "TIPTRUCK", "TIPTRUCK2", "CUTTER", "HANDLER", "DOCKTUG", "DUMP", "BIFF", "HAULER", "PHANTOM", "RUBBLE", "MULE", "MULE2", "MULE3", "MIXER", "MIXER2", "FLATBED", "PACKER", "POUNDER", "BENSON", "SCRAP", "TOWTRUCK", "TOWTRUCK2" };

const std::vector<std::string> VALUES_SERVICES{ "AIRTUG", "AIRBUS", "BUS", "RENTALBUS", "TOURBUS", "CABLECAR", "COACH", "BULLDOZER", "FORKLIFT", "TRASH", "TRASH2", "CADDY", "CADDY2", "RIPLEY", "TRACTOR2", "TRACTOR3", "MOWER", "TRACTOR", "UTILLITRUCK3", "UTILLITRUCK2", "UTILLITRUCK" };

const std::vector<std::string> VALUES_TRAILERS{ "ARMYTRAILER", "ARMYTRAILER2", "ARMYTANKER", "BOATTRAILER", "TR3", "TR4", "TR2", "TRAILERS2", "TRAILERS3", "DOCKTRAILER", "TVTRAILER", "FREIGHTTRAILER", "TRFLAT", "GRAINTRAILER", "BALETRAILER", "TRAILERLOGS", "PROPTRAILER", "TANKER2", "TANKER", "TRAILERS", "RAKETRAILER", "TRAILERSMALL" };

const std::vector<std::string> VALUES_TRAINS{ "FREIGHTCONT1", "FREIGHTCONT2", "FREIGHTCAR", "FREIGHT", "FREIGHTGRAIN", "METROTRAIN", "TANKERCAR" };

const std::vector<std::string> VOV_INDUS_CAPTIONS[] = { CAPTIONS_PICKUPS, CAPTIONS_VANS, CAPTIONS_TRUCKS, CAPTIONS_SERVICES, CAPTIONS_TRAILERS, CAPTIONS_TRAINS };

const std::vector<std::string> VOV_INDUS_VALUES[] = { VALUES_PICKUPS, VALUES_VANS, VALUES_TRUCKS, VALUES_SERVICES, VALUES_TRAILERS, VALUES_TRAINS };

//Everything else

const std::vector<std::string> CAPTIONS_EMERGENCY{ "Albany Police Roadcruiser (Snow)", "Ambulance", "Army Barracks Truck", "Army Truck Cab", "Bravado Buffalo (FIB)", "Brute Police Riot Van", "Canis Crusader (Army Mesa)", "Declasse Granger (FIB)", "Declasse Lifeguard", "Declasse Park Ranger", "Declasse Police Rancher (Snow)", "Declasse Police Transporter", "Declasse Sheriff SUV", "Firetruck", "Prison Bus", "Rhino Tank", "Vapid Police Buffalo", "Vapid Police Cruiser", "Vapid Police Interceptor", "Vapid Sheriff Cruiser", "Vapid Unmarked Police Cruiser", "Western Police Bike" };

const std::vector<std::string> CAPTIONS_MOTORCYCLES{ "Dinka Akuma", "Dinka Double-T", "Dinka Enduro", "Dinka Thrust", "Dinka Vindicator", "LCC Hexer", "LCC Innovation", "Maibatsu Sanchez", "Maibatsu Sanchez (Graphics)", "Nagasaki Carbon RS", "Pegassi Bati", "Pegassi Bati (Race)", "Pegassi Ruffian", "Principe Faggio", "Principe Lectro", "Principe Nemesis", "Shitzu Hakuchou", "Shitzu PCJ 600", "Shitzu Vader", "Western Bagger", "Western Daemon", "Western Sovereign" };

const std::vector<std::string> CAPTIONS_PLANES{ "Buckingham Cargo Plane (An-225)", "Buckingham Jet (B747)", "Buckingham Luxor", "Buckingham Luxor Deluxe", "Buckingham Miljet", "Buckingham Shamal", "Buckingham Vestra", "Jobuilt Mammatus", "Jobuilt P-996 Lazer", "Jobuilt Velum (4 Seater)", "Jobuilt Velum (5 Seater)", "Mammoth Dodo", "Mammoth Hydra", "Mammoth Titan", "Western Besra", "Western Cuban 800", "Western Duster", "Western Mallard Stunt Plane" };

const std::vector<std::string> CAPTIONS_HELOS{ "Blimp (Atomic)", "Blimp (Xero Gas)", "Buckingham Savage", "Buckingham Swift", "Buckingham Swift Deluxe", "Buckingham Valkyrie", "HVY Skylift", "Maibatsu Frogger", "Maibatsu Frogger (TPE/FIB)", "Nagasaki Buzzard (Unarmed)", "Nagasaki Buzzard Attack Chopper", "Western Annihilator", "Western Cargobob (Desert Camo)", "Western Cargobob (Jetsam)", "Western Cargobob (TPE)", "Western Maverick", "Western Maverick (Emergency)" };

const std::vector<std::string> CAPTIONS_BOATS{ "Dinka Marquis", "Kraken Sub", "Lampadati Toro", "Nagasaki Dinghy (2 Seater)", "Nagasaki Dinghy (4 Seater, Black)", "Nagasaki Dinghy (4 Seater, Red)", "Pegassi Speeder", "Shitzu Jetmax", "Shitzu Predator (Police)", "Shitzu Squalo", "Shitzu Suntrap", "Shitzu Tropic", "Speedophile Seashark", "Speedophile Seashark (Lifeguard)", "Submersible" };

const std::vector<std::string> CAPTIONS_BICYCLES{ "BMX", "Cruiser", "Endurex Race", "Fixter", "Scorcher", "Tri-Cycles Race", "Whippet Race" };

const std::vector<std::string> VALUES_EMERGENCY{ "POLICEOLD2", "AMBULANCE", "BARRACKS", "BARRACKS2", "FBI", "RIOT", "CRUSADER", "FBI2", "LGUARD", "PRANGER", "POLICEOLD1", "POLICET", "SHERIFF2", "FIRETRUK", "PBUS", "RHINO", "POLICE2", "POLICE", "POLICE3", "SHERIFF", "POLICE4", "POLICEB" };

const std::vector<std::string> VALUES_MOTORCYCLES{ "AKUMA", "DOUBLE", "ENDURO", "THRUST", "VINDICATOR", "HEXER", "INNOVATION", "SANCHEZ2", "SANCHEZ", "CARBONRS", "BATI", "BATI2", "RUFFIAN", "FAGGIO2", "LECTRO", "NEMESIS", "HAKUCHOU", "PCJ", "VADER", "BAGGER", "DAEMON", "SOVEREIGN" };

const std::vector<std::string> VALUES_PLANES{ "CARGOPLANE", "JET", "LUXOR", "LUXOR2", "MILJET", "SHAMAL", "VESTRA", "MAMMATUS", "LAZER", "VELUM", "VELUM2", "DODO", "HYDRA", "TITAN", "BESRA", "CUBAN800", "DUSTER", "STUNT" };

const std::vector<std::string> VALUES_HELOS{ "BLIMP", "BLIMP2", "SAVAGE", "SWIFT", "SWIFT2", "VALKYRIE", "SKYLIFT", "FROGGER", "FROGGER2", "BUZZARD2", "BUZZARD", "ANNIHILATOR", "CARGOBOB", "CARGOBOB2", "CARGOBOB3", "MAVERICK", "POLMAV" };

const std::vector<std::string> VALUES_BOATS{ "MARQUIS", "SUBMERSIBLE2", "TORO", "DINGHY2", "DINGHY3", "DINGHY", "SPEEDER", "JETMAX", "PREDATOR", "SQUALO", "SUNTRAP", "TROPIC", "SEASHARK", "SEASHARK2", "SUBMERSIBLE" };

const std::vector<std::string> VALUES_BICYCLES{ "BMX", "CRUISER", "TRIBIKE2", "FIXTER", "SCORCHER", "TRIBIKE3", "TRIBIKE" };

const std::vector<std::string> VOV_SHALLOW_CAPTIONS[] = { CAPTIONS_EMERGENCY, CAPTIONS_MOTORCYCLES, CAPTIONS_PLANES, CAPTIONS_HELOS, CAPTIONS_BOATS, CAPTIONS_BICYCLES };

const std::vector<std::string> VOV_SHALLOW_VALUES[] = { VALUES_EMERGENCY, VALUES_MOTORCYCLES, VALUES_PLANES, VALUES_HELOS, VALUES_BOATS, VALUES_BICYCLES };

std::string lastCustomVehicleSpawn;

bool onconfirm_vehdoor_menu(MenuItem<int> choice) {

	if (choice.value == -1)
	{
		return false;
	}

	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (choice.value >= 0)
	{
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			int value = choice.value;

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

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	std::vector<MenuItem<int>*> menuItems;
	std::vector<int>menuIndexes;

	std::vector<std::string> DOOR_NAMES = {
		"Front Left",
		"Front Right",
		"Rear Left",
		"Rear Right",
		"Hood",
		"Trunk ",
		"Trunk 2"
	};

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Toggle Open Instantly";
	toggleItem->toggleValue = &featureVehicleDoorInstant;
	menuItems.push_back(toggleItem);

	for (int i = 0; i < DOOR_NAMES.size(); i++)
	{

		MenuItem<int> *item = new MenuItem<int>();
		item->value = i;
		item->caption = DOOR_NAMES[i];
		menuItems.push_back(item);
	}

	if (VEHICLE::IS_VEHICLE_A_CONVERTIBLE(veh, false))
	{
		FunctionDrivenToggleMenuItem<int>* toggleItem = new FunctionDrivenToggleMenuItem<int>();
		toggleItem->caption = "Convertible Roof Down?";
		toggleItem->getter_call = is_convertible_roofdown;
		toggleItem->setter_call = set_convertible_roofdown;
		toggleItem->value = -1;
		menuItems.push_back(toggleItem);
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
		if (process_savedveh_menu()) return false;
		break;
	case 2: // fix
		fix_vehicle();
		break;
	case 3: // clean
		clean_vehicle();
		break;
	case 4: // paint
		if (process_paint_menu()) return false;
		break;
	case 5: // mods
		if (process_vehmod_menu()) return false;
		break;
	case 14: // door menu
		if (process_veh_door_menu()) return false;
		break;
	default:
		break;
	}
	return false;
}

void process_veh_menu()
{
	std::string caption = "Vehicle Options";

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *item;
	SelectFromListMenuItem *listItem;

	int i = 0;

	item = new MenuItem<int>();
	item->caption = "Vehicle Spawner";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Saved Vehicles";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Fix";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Clean";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Paint Menu";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Modifications";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(VEH_INVINC_MODE_CAPTIONS, onchange_veh_invincibility_mode);
	listItem->wrap = false;
	listItem->caption = "Vehicle Invincibility";
	listItem->value = get_current_veh_invincibility_mode();
	menuItems.push_back(listItem);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Falling Off/Out";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureNoVehFallOff;
	toggleItem->toggleValueUpdated = &featureNoVehFallOffUpdated;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Don't Wear Helmet";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWearHelmetOff;
	toggleItem->toggleValueUpdated = &featureWearHelmetOffUpdated;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Into Vehicle";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSpawnInto;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Vehicles Fully Tuned";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSpawnTuned;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Vehicles Fully Pimped";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSpawnOptic;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Speed Boost";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSpeedBoost;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_ENG_POW_CAPTIONS, onchange_veh_eng_pow_index);
	listItem->wrap = false;
	listItem->caption = "Engine Power Multiplier";
	listItem->value = engPowMultIndex;
	menuItems.push_back(listItem);

	/*listItem = new SelectFromListMenuItem(VEH_MASS_CAPTIONS, onchange_veh_mass_index);
	listItem->wrap = false;
	listItem->caption = "Vehicle Mass Multiplier";
	listItem->value = VehMassMultIndex;
	menuItems.push_back(listItem);*/

	item = new MenuItem<int>();
	item->caption = "Door Control";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	draw_generic_menu<int>(menuItems, &activeLineIndexVeh, caption, onconfirm_veh_menu, NULL, NULL);
}

void update_vehicle_features(BOOL bPlayerExists, Ped playerPed)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	// player's vehicle invincible
	if (featureVehInvincibleUpdated)
	{
		if (bPlayerExists && !featureVehInvincible && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
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
			if (FIRE::IS_ENTITY_ON_FIRE(veh))
			{
				FIRE::STOP_ENTITY_FIRE(veh);
			}

			ENTITY::SET_ENTITY_HEALTH(veh, 10000.0f);
			VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 10000.0);
			VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 10000.0);

			ENTITY::SET_ENTITY_PROOFS(veh, 1, 1, 1, featureVehNoDamage, 1, 1, 1, 1);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 0);

			VEHICLE::SET_VEHICLE_CAN_BREAK(veh, !featureVehNoDamage);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, featureVehNoDamage);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(veh, !featureVehNoDamage);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, !featureVehNoDamage);

			for (int i = 0; i < 6; i++)
			{
				VEHICLE::_SET_VEHICLE_DOOR_BREAKABLE(veh, i, !featureVehNoDamage); //(Vehicle, doorIndex, isBreakable)
			}

			if (featureVehNoDamage)
			{
				ENTITY::SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(veh, 1);

				VEHICLE::SET_VEHICLE_BODY_HEALTH(veh, 10000.0f);
				
				/*
				* This API seems to be a damage check - don't just continually repair the
				* vehicle as it causes glitches.
				*/
			
				if (VEHICLE::_IS_VEHICLE_DAMAGED(veh) && featureVehNoDamage && featureVehInvulnIncludesCosmetic)
				{
					VEHICLE::SET_VEHICLE_FIXED(veh);
				}
			}
		}
	}

	// fall off
	if (bPlayerExists && featureNoVehFallOffUpdated && !featureNoVehFallOff)
	{
		PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_THROUGH_WINDSCREEN, TRUE);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPed, 0);
		featureNoVehFallOffUpdated = false;
	}
	else if (bPlayerExists && featureNoVehFallOff)
	{
		PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_THROUGH_WINDSCREEN, FALSE);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPed, 1);
	}

	// player's vehicle boost
	if (featureVehSpeedBoost && bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		bool bUp = IsKeyDown(KeyConfig::KEY_VEH_BOOST) || IsControllerButtonDown(KeyConfig::KEY_VEH_BOOST);
		bool bDown = IsKeyDown(KeyConfig::KEY_VEH_STOP) || IsControllerButtonDown(KeyConfig::KEY_VEH_STOP);

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
				{
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0);
				}
		}
	}

	//Prevents player from wearing a helmet
	if (bPlayerExists && featureWearHelmetOffUpdated)
	{
		PED::SET_PED_HELMET(playerPed, !featureWearHelmetOff);
		featureWearHelmetOffUpdated = false;
	}

	if (bPlayerExists && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
		oldVehicleState = false; // player is NOT in a vehicle, set state to false
	}

	if (bPlayerExists && (did_player_just_enter_vehicle(playerPed) || powChanged)) { // check if player entered vehicle, only need to set mults once
		VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(veh, 1.8f);
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, VEH_ENG_POW_VALUES[engPowMultIndex]);
		powChanged = false;
	}

	if (bPlayerExists && (did_player_just_enter_vehicle(playerPed) || massChanged)) { // check if player entered vehicle, only need to set mults once
		VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(VEH_MASS_VALUES[VehMassMultIndex]);
		massChanged = false;
	}
}

bool did_player_just_enter_vehicle(Ped playerPed) {
	if (oldVehicleState == false && PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) { // if we weren't in a car before, but we are now...
		oldVehicleState = true;
		return true;
	}
	return false;
}

void set_old_vehicle_state(bool updatedState)
{ // used by other functions, like teleporting into cars
	oldVehicleState = updatedState;
}

void reset_vehicle_globals()
{
	//veh_spawn_menu_index = 0;

	featureVehInvincible =
	featureVehSpeedBoost =
	featureVehicleDoorInstant =
	featureVehSpawnInto =
	featureNoVehFallOff =
	featureWearHelmetOff = false;

	featureNoVehFallOffUpdated =
	featureWearHelmetOffUpdated =
	featureVehInvincibleUpdated =
	featureWearHelmetOffUpdated = true;

	featureVehNoDamage = false;
	featureVehInvulnIncludesCosmetic = false;
}

bool onconfirm_carspawn_menu(MenuItem<int> choice)
{
	if (choice.value == MENU_VEHICLE_CATEGORIES.size() - 1) //custom spawn
	{
		std::string result = show_keyboard(NULL, (char*) lastCustomVehicleSpawn.c_str());
		if (!result.empty())
		{
			result = trim(result);
			lastCustomVehicleSpawn = result;
			Hash hash = GAMEPLAY::GET_HASH_KEY((char*)result.c_str());
			if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash) || !STREAMING::IS_MODEL_A_VEHICLE(hash))
			{
				std::ostringstream ss;
				ss << "~r~Error:~r~ Couldn't find model '" << result << "'";
				set_status_text(ss.str());
				return false;
			}
			else
			{
				do_spawn_vehicle(result, result);
			}
		}
		return false;
	}

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
		break;
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
		item->isLeaf = (i == MENU_VEHICLE_CATEGORIES.size() - 1);
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

	MenuParameters<std::string> params(menuItems, category);
	params.menuSelectionPtr = 0;
	params.onConfirmation = onconfirm_spawn_menu_vehicle_selection;
	params.lineImageProvider = vehicle_image_preview_finder;

	return draw_generic_menu<std::string>(params);
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

	std::string category;
	if (choice.value == 3)
	{
		category = "Service And Utility";
	}
	else if (choice.value == 5)
	{
		category = "Trains";
	}
	else
	{
		category = choice.caption;
	}

	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_INDUS_CAPTIONS[selection].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_INDUS_CAPTIONS[selection][i];
		item->value = VOV_INDUS_VALUES[selection][i];
		menuItems.push_back(item);
	}

	MenuParameters<std::string> params(menuItems, category);
	params.menuSelectionPtr = 0;
	params.onConfirmation = onconfirm_spawn_menu_vehicle_selection;
	params.lineImageProvider = vehicle_image_preview_finder;

	return draw_generic_menu<std::string>(params);
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

	MenuParameters<std::string> params(menuItems, category);
	params.menuSelectionPtr = 0;
	params.onConfirmation = onconfirm_spawn_menu_vehicle_selection;
	params.lineImageProvider = vehicle_image_preview_finder;

	return draw_generic_menu<std::string>(params);
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
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		Vector3 minDimens;
		Vector3 maxDimens;
		GAMEPLAY::GET_MODEL_DIMENSIONS(model, &minDimens, &maxDimens);
		float spawnOffY = max(5.0f, 2.0f + 0.5f * (maxDimens.y - minDimens.y));

		FLOAT lookDir = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, spawnOffY, 0.0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, lookDir, 1, 0);

		//if we're mid-air, don't put it on the ground
		if (!ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()))
		{
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
		}

		if (featureVehSpawnTuned)
		{
			fully_tune_vehicle(veh, false, featureVehSpawnOptic);
		}

		if (featureVehSpawnInto)
		{
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
			oldVehicleState = false; // set old vehicle state to false since we changed cars but didn't actually exit the last one

			if (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh)))
			{
				VEHICLE::SET_HELI_BLADES_FULL_SPEED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
			}
		}

		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.0f);

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


void add_vehicle_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featureNoVehFallOff", &featureNoVehFallOff, &featureNoVehFallOffUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureVehicleDoorInstant", &featureVehicleDoorInstant });
	results->push_back(FeatureEnabledLocalDefinition{ "featureVehInvincible", &featureVehInvincible, &featureVehInvincibleUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureVehNoDamage", &featureVehNoDamage, &featureVehInvincibleUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureVehSpawnInto", &featureVehSpawnInto });
	results->push_back(FeatureEnabledLocalDefinition{ "featureVehSpeedBoost", &featureVehSpeedBoost });
	results->push_back(FeatureEnabledLocalDefinition{ "featureVehSpawnTuned", &featureVehSpawnTuned });
	results->push_back(FeatureEnabledLocalDefinition{ "featureVehSpawnOptic", &featureVehSpawnOptic });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWearHelmetOff", &featureWearHelmetOff, &featureWearHelmetOffUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureVehInvulnIncludesCosmetic", &featureVehInvulnIncludesCosmetic, &featureVehInvincibleUpdated });
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

		int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 23);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, currmod, savedVeh->customTyres);
		VEHICLE::SET_VEHICLE_MOD(veh, 24, currmod, savedVeh->customTyres);

		if (savedVeh->livery != -1)
		{
			VEHICLE::SET_VEHICLE_LIVERY(veh, savedVeh->livery);
		}

		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, savedVeh->colourExtraPearl, savedVeh->colourExtraWheel);
		VEHICLE::SET_VEHICLE_MOD_COLOR_1(veh, savedVeh->colourMod1Type, savedVeh->colourMod1Colour, savedVeh->colourMod1P3);
		VEHICLE::SET_VEHICLE_MOD_COLOR_2(veh, savedVeh->colourMod2Type, savedVeh->colourMod2Colour);

		VEHICLE::SET_VEHICLE_COLOURS(veh, savedVeh->colourPrimary, savedVeh->colourSecondary);

		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, savedVeh->colourExtraPearl, savedVeh->colourExtraWheel);

		if (savedVeh->colourCustom1RGB[0] != -1 &&
			savedVeh->colourCustom1RGB[1] != -1 &&
			savedVeh->colourCustom1RGB[2] != -1)
		{
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, savedVeh->colourCustom1RGB[0], savedVeh->colourCustom1RGB[1], savedVeh->colourCustom1RGB[2]);
		}

		if (savedVeh->colourCustom2RGB[0] != -1 &&
			savedVeh->colourCustom2RGB[1] != -1 &&
			savedVeh->colourCustom2RGB[2] != -1)
		{
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, savedVeh->colourCustom2RGB[0], savedVeh->colourCustom2RGB[1], savedVeh->colourCustom2RGB[2]);
		}

		VEHICLE::_SET_VEHICLE_PAINT_FADE(veh, savedVeh->fadeLevel);

		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, savedVeh->dirtLevel);

		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 0, savedVeh->neonEnablement[0] == 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 1, savedVeh->neonEnablement[1] == 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 2, savedVeh->neonEnablement[2] == 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 3, savedVeh->neonEnablement[3] == 1);

		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, savedVeh->neonRGB[0], savedVeh->neonRGB[1], savedVeh->neonRGB[2]);

		if (savedVeh->tyreSmokeRGB[0] != -1 &&
			savedVeh->tyreSmokeRGB[1] != -1 &&
			savedVeh->tyreSmokeRGB[2] != -1)
		{
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, savedVeh->tyreSmokeRGB[0], savedVeh->tyreSmokeRGB[1], savedVeh->tyreSmokeRGB[2]);
		}

		if (VEHICLE::IS_VEHICLE_A_CONVERTIBLE(veh, 0))
		{
			if (savedVeh->convertibleRoofUp)
			{
				VEHICLE::SET_CONVERTIBLE_ROOF(veh, 1);
			}
			else
			{
				VEHICLE::SET_CONVERTIBLE_ROOF(veh, 0);
			}
		}

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
			requireRefreshOfVehSaveSlots = true;
			requireRefreshOfVehSlotMenu = true;
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
			requireRefreshOfVehSaveSlots = true;
			requireRefreshOfVehSlotMenu = true;
			vehSaveSlotMenuInterrupt = true;
			vehSaveMenuInterrupt = true;
		}
		break;
	case 4: //delete
		{
			ENTDatabase* database = get_database();
			database->delete_saved_vehicle(activeSavedVehicleIndex);

			requireRefreshOfVehSlotMenu = false;
			requireRefreshOfVehSaveSlots = true;
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
		requireRefreshOfVehSlotMenu = false;

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
	} while (requireRefreshOfVehSlotMenu);
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
					set_status_text("Vehicle saved");
					activeSavedVehicleSlotName = result;
				}
				else
				{
					set_status_text("Save error");
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
		requireRefreshOfVehSaveSlots = true;
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
		requireRefreshOfVehSlotMenu = false;
		requireRefreshOfVehSaveSlots = false;

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
	while (requireRefreshOfVehSaveSlots);

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

void add_vehicle_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "lastCustomVehicleSpawn", lastCustomVehicleSpawn });
	results->push_back(StringPairSettingDBRow{ "engPowMultIndex", std::to_string(engPowMultIndex) });
}

void handle_generic_settings_vehicle(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("lastCustomVehicleSpawn") == 0)
		{
			lastCustomVehicleSpawn = setting.value;
		} else if (setting.name.compare("engPowMultIndex") == 0)
		{
			engPowMultIndex = stoi(setting.value);
		}
	}
}

int get_current_veh_invincibility_mode()
{
	if (!featureVehInvincible)
	{
		return 0;
	}
	else if (!featureVehNoDamage)
	{
		return 1;
	}
	else if (!featureVehInvulnIncludesCosmetic)
	{
		return 2;
	}
	return 3;
}

void onchange_veh_invincibility_mode(int value, SelectFromListMenuItem* source)
{
	featureVehInvincible = (value > 0);
	featureVehNoDamage = (value > 1);
	featureVehInvulnIncludesCosmetic = (value > 2);

	featureVehInvincibleUpdated = true;
}

int get_current_veh_eng_pow_index(){
	return engPowMultIndex;
}

void onchange_veh_eng_pow_index(int value, SelectFromListMenuItem* source) {
	engPowMultIndex = value;
	powChanged = true;
}

void onchange_veh_mass_index(int value, SelectFromListMenuItem* source) 
{
	VehMassMultIndex = value;
	massChanged = true;
}
struct VehicleImage
{
	char* modelName;
	char* dict;
	char* imgName;
	int localID;
};

//car name, DLC name for texture (scaleform_web.rpf > the name of the ytd file), name of car in file - no need for extracting files!

const std::vector<VehicleImage> INGAME_VEH_IMAGES = 
{
	{ "AIRBUS", "candc_default", "airbus" },
	{ "BARRACKS", "candc_default", "barracks" },
	{ "BOXVILLE4", "candc_default", "boxville4" },
	{ "BUS", "candc_default", "bus" },
	{ "BUZZARD", "candc_default", "buzzard" },
	{ "CARGOBOB", "candc_default", "cargobob" },
	{ "COACH", "candc_default", "coach" },
	{ "CRUSADER", "candc_default", "crusader" },
	{ "DUBSTA3", "candc_default", "dubsta3" },
	{ "DUKES2", "candc_default", "dukes" },
	{ "DUMP", "candc_default", "dump" },
	{ "HYDRA", "candc_default", "hydra" },
	{ "INSURGENT", "candc_default", "insurgent" },
	{ "INSURGENT2", "candc_default", "insurgent2" },
	{ "JOURNEY", "candc_default", "journey" },
	{ "MARSHALL", "candc_default", "marshall" },
	{ "MESA", "candc_default", "mesa" },
	{ "MESA3", "candc_default", "mesa3" },
	{ "MONSTER", "candc_default", "monster" },
	{ "MULE", "candc_default", "mule" },
	{ "MULE3", "candc_default", "mule3" },
	{ "PBUS", "candc_default", "pbus" },
	{ "RENTALBUS", "candc_default", "rentbus" },
	{ "RHINO", "candc_default", "rhino" },
	{ "SAVAGE", "candc_default", "savage" },
	{ "TECHNICAL", "candc_default", "technical" },
	{ "VALKYRIE", "candc_default", "valkyrie" },
	{ "DINGHY3", "dock_default", "dinghy3" },
	{ "JETMAX", "dock_default", "jetmax" },
	{ "MARQUIS", "dock_default", "marquis" },
	{ "SEASHARK", "dock_default", "seashark" },
	{ "SPEEDER", "dock_default", "speeder" },
	{ "SQUALO", "dock_default", "squalo" },
	{ "SUBMERSIBLE2", "dock_default", "sub2" },
	{ "SUNTRAP", "dock_default", "suntrap" },
	{ "TROPIC", "dock_default", "tropic" },
	{ "ANNIHILATOR", "elt_default", "annihl" },
	{ "CUBAN800", "elt_default", "cuban800" },
	{ "DODO", "elt_default", "dodo" },
	{ "DUSTER", "elt_default", "duster" },
	{ "FROGGER", "elt_default", "frogger" },
	{ "LUXOR", "elt_default", "luxor" },
	{ "MAMMATUS", "elt_default", "mammatus" },
	{ "MAVERICK", "elt_default", "maverick" },
	{ "SHAMAL", "elt_default", "shamal" },
	{ "STUNT", "elt_default", "stunt" },
	{ "TITAN", "elt_default", "titan" },
	{ "VELUM", "elt_default", "velum" },
	{ "VELUM2", "elt_default", "velum2" },
	{ "VESTRA", "elt_dlc_business", "vestra" },
	{ "LUXOR2", "elt_dlc_luxe", "luxor2" },
	{ "SWIFT2", "elt_dlc_luxe", "swift2" },
	{ "BESRA", "elt_dlc_pilot", "besra" },
	{ "MILJET", "elt_dlc_pilot", "miljet" },
	{ "SWIFT", "elt_dlc_pilot", "swift" },
	{ "ADDER", "lgm_default", "adder" },
	{ "BANSHEE", "lgm_default", "banshee" },
	{ "BULLET", "lgm_default", "bullet" },
	{ "CARBONIZZARE", "lgm_default", "carboniz" },
	{ "CARBONRS", "lgm_default", "carbon" },
	{ "CHEETAH", "lgm_default", "cheetah" },
	{ "COGCABRIO", "lgm_default", "cogcabri" },
	{ "COMET2", "lgm_default", "comet2" },
	{ "COQUETTE", "lgm_default", "coquette" },
	{ "ELEGY2", "lgm_default", "elegy2" },
	{ "ENTITYXF", "lgm_default", "entityxf" },
	{ "EXEMPLAR", "lgm_default", "exemplar" },
	{ "FELTZER2", "lgm_default", "feltzer" },
	{ "HOTKNIFE", "lgm_default", "hotknife" },
	{ "JB700", "lgm_default", "jb700" },
	{ "KHAMELION", "lgm_default", "khamel" },
	{ "MONROE", "lgm_default", "monroe" },
	{ "NEMESIS", "sssa_dlc_heist", "nemesis" },
	{ "NINEF", "lgm_default", "ninef" },
	{ "NINEF2", "lgm_default", "ninef2" },
	{ "RAPIDGT", "lgm_default", "rapidgt" },
	{ "RAPIDGT2", "lgm_default", "rapidgt2" },
	{ "STINGER", "lgm_default", "stinger" },
	{ "STINGERGT", "lgm_default", "stingerg" },
	{ "VOLTIC", "lgm_default", "voltic_tless" },
	{ "ZTYPE", "lgm_default", "ztype" },
	{ "ALPHA", "lgm_dlc_business", "alpha" },
	{ "JESTER", "lgm_dlc_business", "jester" },
	{ "TURISMOR", "lgm_dlc_business", "turismor" },
	{ "COQUETTE2", "lgm_dlc_pilot", "coquette2" },
	{ "HUNTLEY", "lgm_dlc_business2", "huntley" },
	{ "MASSACRO", "lgm_dlc_business2", "massacro" },
	{ "THRUST", "lgm_dlc_business2", "thrust" },
	{ "ZENTORNO", "lgm_dlc_business2", "zentorno" },
	{ "FUROREGT", "lgm_dlc_lts_creator", "furore" },
	{ "FELTZER3", "lgm_dlc_luxe", "feltzer3" },
	{ "OSIRIS", "lgm_dlc_luxe", "osiris" },
	{ "VIRGO", "lgm_dlc_luxe", "virgo" },
	{ "WINDSOR", "lgm_dlc_luxe", "windsor" },
	{ "BMX", "pandm_default", "bmx" },
	{ "CRUISER", "pandm_default", "cruiser" },
	{ "SCORCHER", "pandm_default", "scorcher" },
	{ "TRIBIKE", "pandm_default", "tribike" },
	{ "TRIBIKE2", "pandm_default", "tribike2" },
	{ "TRIBIKE3", "pandm_default", "tribike3" },
	{ "AKUMA", "sssa_default", "akuma" },
	{ "BALLER", "sssa_default", "baller2" },
	{ "BATI", "sssa_default", "bati" },
	{ "BATI2", "sssa_default", "bati2" },
	{ "BFINJECTION", "sssa_default", "bfinject" },
	{ "BIFTA", "sssa_default", "bifta" },
	{ "BISON", "sssa_default", "bison" },
	{ "BLAZER", "sssa_default", "blazer" },
	{ "BODHI2", "sssa_default", "bodhi2" },
	{ "CAVALCADE", "sssa_default", "cavcade" },
	{ "DILETTANTE", "sssa_default", "dilettan" },
	{ "DOUBLE", "sssa_default", "double" },
	{ "DUNE", "sssa_default", "dune" },
	{ "FAGGIO2", "sssa_default", "faggio" },
	{ "FELON", "sssa_default", "felon" },
	{ "FELON2", "sssa_default", "felon2" },
	{ "FUGITIVE", "sssa_default", "fugitive" },
	{ "GAUNTLET", "sssa_default", "gauntlet" },
	{ "HEXER", "sssa_default", "hexer" },
	{ "INFERNUS", "sssa_default", "infernus" },
	{ "ISSI2", "sssa_default", "issi2" },
	{ "KALAHARI", "sssa_default", "kalahari" },
	{ "ORACLE", "sssa_default", "oracle" },
	{ "PARADISE", "sssa_default", "paradise" },
	{ "PCJ", "sssa_default", "pcj" },
	{ "REBEL", "sssa_default", "rebel" },
	{ "ROCOTO", "sssa_default", "rocoto" },
	{ "RUFFIAN", "sssa_default", "ruffian" },
	{ "SADLER", "sssa_default", "sadler" },
	{ "SANCHEZ", "sssa_default", "sanchez" },
	{ "SANCHEZ2", "sssa_default", "sanchez2" },
	{ "SANDKING", "sssa_default", "sandking" },
	{ "SANDKING2", "sssa_default", "sandkin2" },
	{ "SCHWARZER", "sssa_default", "schwarze" },
	{ "STRETCH", "sssa_default", "stretch" },
	{ "SUPERD", "lgm_default", "superd" },
	{ "SURANO", "lgm_default", "surano_convertable" },
	{ "VACCA", "lgm_default", "vacca" },
	{ "VADER", "sssa_default", "vader" },
	{ "VIGERO", "sssa_default", "vigero" },
	{ "ZION", "sssa_default", "zion" },
	{ "ZION2", "sssa_default", "zion2" },
	{ "ASEA", "sssa_dlc_business", "asea" },
	{ "ASTEROPE", "sssa_dlc_business", "astrope" },
	{ "BOBCATXL", "sssa_dlc_business", "bobcatxl" },
	{ "CAVALCADE2", "sssa_dlc_business", "cavcade2" },
	{ "INGOT", "sssa_dlc_business", "ingot" },
	{ "INTRUDER", "sssa_dlc_business", "intruder" },
	{ "MINIVAN", "sssa_dlc_business", "minivan" },
	{ "PREMIER", "sssa_dlc_business", "premier" },
	{ "RADI", "sssa_dlc_business", "radi" },
	{ "RANCHERXL", "sssa_dlc_business", "rancherx" },
	{ "STANIER", "sssa_dlc_business", "stanier" },
	{ "STRATUM", "sssa_dlc_business", "stratum" },
	{ "WASHINGTON", "sssa_dlc_business", "washingt" },
	{ "DOMINATOR", "sssa_dlc_business2", "dominato" },
	{ "F620", "sssa_dlc_business2", "f620" },
	{ "FUSILADE", "sssa_dlc_business2", "fusilade" },
	{ "PENUMBRA", "sssa_dlc_business2", "penumbra" },
	{ "SENTINEL", "sssa_dlc_business2", "sentinel" },
	{ "JESTER2", "sssa_dlc_christmas_2", "jester2" },
	{ "MASSACRO2", "sssa_dlc_christmas_2", "massacro2" },
	{ "RATLOADER2", "sssa_dlc_christmas_2", "rloader2" },
	{ "SLAMVAN", "sssa_dlc_christmas_2", "slamvan" },
	{ "ENDURO", "sssa_dlc_heist", "enduro" },
	{ "GBURRITO2", "sssa_dlc_heist", "gburrito2" },
	{ "GRESLEY", "sssa_dlc_heist", "gresley" },
	{ "JACKAL", "sssa_dlc_heist", "jackal" },
	{ "KURUMA", "sssa_dlc_heist", "kuruma" },
	{ "KURUMA2", "sssa_dlc_heist", "kuruma2" },
	{ "LANDSTALKER", "sssa_dlc_heist", "landstalker" },
	{ "RUMPO", "sssa_dlc_heist", "rumpo" },
	{ "SCHAFTER2", "sssa_dlc_heist", "schafter2" },
	{ "SEMINOLE", "sssa_dlc_heist", "seminole" },
	{ "SURGE", "sssa_dlc_heist", "surge" },
	{ "BLADE", "sssa_dlc_hipster", "blade" },
	{ "BLAZER3", "sssa_dlc_hipster", "blazer3" },
	{ "BUFFALO", "sssa_dlc_hipster", "buffalo" },
	{ "BUFFALO2", "sssa_dlc_hipster", "buffalo2" },
	{ "GLENDALE", "sssa_dlc_hipster", "glendale" },
	{ "PANTO", "sssa_dlc_hipster", "panto" },
	{ "PICADOR", "sssa_dlc_hipster", "picador" },
	{ "PIGALLE", "sssa_dlc_hipster", "pigalle" },
	{ "PRIMO", "sssa_dlc_hipster", "primo" },
	{ "REBEL2", "sssa_dlc_hipster", "rebel2" },
	{ "REGINA", "sssa_dlc_hipster", "regina" },
	{ "RHAPSODY", "sssa_dlc_hipster", "rhapsody" },
	{ "SURFER", "sssa_dlc_hipster", "surfer" },
	{ "TAILGATER", "sssa_dlc_hipster", "tailgater" },
	{ "WARRENER", "sssa_dlc_hipster", "warrener" },
	{ "YOUGA", "sssa_dlc_hipster", "youga" },
	{ "SOVEREIGN", "sssa_dlc_independence", "sovereign" },
	{ "HAKUCHOU", "sssa_dlc_lts_creator", "hakuchou" },
	{ "INNOVATION", "sssa_dlc_lts_creator", "innovation" },
	{ "BLISTA2", "sssa_dlc_mp_to_sp", "blista2" },
	{ "BUFFALO3", "sssa_dlc_mp_to_sp", "buffalo3" },
	{ "DOMINATOR2", "sssa_dlc_mp_to_sp", "dominator2" },
	{ "GAUNTLET2", "sssa_dlc_mp_to_sp", "gauntlet2" },
	{ "STALION", "sssa_dlc_mp_to_sp", "stallion" },
	{ "STALION2", "sssa_dlc_mp_to_sp", "stalion2" },
	{ "RATLOADER", "sssa_dlc_valentines", "rloader" },
	{ "CASCO", "lgm_dlc_heist", "casco" },
	{ "CHINO", "lgm_dlc_luxe", "chino" },
	{ "COQUETTE3", "lgm_dlc_luxe", "coquette3" },
	{ "T20", "lgm_dlc_luxe", "t20" },
	{ "VINDICATOR", "lgm_dlc_luxe", "vindicator" },
	{ "BRAWLER", "lgm_dlc_luxe", "brawler" },
	{ "TORO", "dock_default", "toro" }, //started adding new cars after this line
	{ "BUCCANEER2", "lsc_default", "buccaneer2_a" },
	{ "CHINO2", "lsc_default", "chino2_a" },
	{ "FACTION2", "lsc_default", "faction2_a" },
	{ "MOONBEAM2", "lsc_default", "moonbeam2_a" },
	{ "PRIMO2", "lsc_default", "primo2_a" },
	{ "VOODOO", "lsc_default", "voodoo_a" },
	{ "VOODOO2", "lsc_default", "voodoo_a" },
	{ "MOONBEAM", "lsc_default", "moonbeam2_a" },
	{ "FACTION", "lsc_default", "faction2_a" },
	{ "BALLER3", "lgm_dlc_apartments", "baller3" },
	//{ "BALLER4", "lgm_dlc_apartments", "baller4" },
	{ "BALLER5", "lgm_dlc_apartments", "baller3" },
	{ "BALLER4", "sssa_default", "baller2" },
	{ "BALLER6", "sssa_default", "baller2" },
	{ "COG55", "lgm_dlc_apartments", "cog55" },
	{ "COGNOSC", "lgm_dlc_apartments", "cognosc" },
	{ "MAMBA", "lgm_dlc_apartments", "mamba" },
	{ "NITESHAD", "lgm_dlc_apartments", "niteshad" },
	{ "SCHAFTER3", "lgm_dlc_apartments", "schafter3" },
	{ "SCHAFTER4", "lgm_dlc_apartments", "schafter4" },
	{ "SCHAFTER5", "sssa_dlc_heist", "schafter2" },
	{ "SCHAFTER6", "lgm_dlc_apartments", "schafter4" },
	{ "VERLIER", "lgm_dlc_apartments", "verlier" },
	{ "BTYPE2", "sssa_dlc_halloween", "btype2" },
	{ "LURCHER", "sssa_dlc_halloween", "lurcher" },
	{ "BTYPE3", "lgm_dlc_valentines2", "roosevelt2" },
	{ "BANSHEE2", "lsc_jan2016", "banshee2_a" },
	{ "SULTANRS", "lsc_jan2016", "sultan2_a" },
	{ "TAMPA", "sssa_dlc_christmas_3", "tampa" }
};

static std::vector<VehicleImage> ALL_VEH_IMAGES;

MenuItemImage* vehicle_image_preview_finder(MenuItem<std::string> choice)
{
	if (!is_vehicle_preview_enabled())
	{
		return NULL;
	}

	for each (VehicleImage vimg in ALL_VEH_IMAGES)
	{
		if (strcmp(vimg.modelName, choice.value.c_str()) == 0)
		{
			MenuItemImage* image = new MenuItemImage();
			image->dict = vimg.dict;
			if (image->is_local())
			{
				image->localID = vimg.localID;
			}
			else
			{
				image->name = vimg.imgName;
			}
			return image;
		}
	}

	std::ostringstream ss;
	ss << "Couldn't find preview for " << choice.value;
	write_text_to_log_file(ss.str());
	return NULL;
}



void unpack_veh_preview(char* model, int resRef, std::string bitmapName)
{
	WAIT(0);
	make_periodic_feature_call();

	HMODULE module = GetENTModuleHandle();
	if (module == NULL)
	{
		write_text_to_log_file("Can't load images because module is null");
		return;
	}

	std::ostringstream filenameSS;
	filenameSS << bitmapName << ".png";
	auto filename = filenameSS.str();
	WCHAR* filePath = get_temp_dir_path((char*) filename.c_str());

	char* filePathNonW = new char[MAX_PATH];
	wcstombs(filePathNonW, filePath, MAX_PATH);

	if (!does_file_exist(filePathNonW))
	{
		// Locate the resource in the application's executable.
		HRSRC imageResHandle = FindResource(
			module,             // This component.
			MAKEINTRESOURCE(resRef),   // Resource name.
			_T("PNG"));        // Resource type.

		if (imageResHandle == NULL)
		{
			std::ostringstream ss;
			int e = GetLastError();
			ss << "Couldn't find resource " << bitmapName << " and code " << resRef << ", error " << e;
			write_text_to_log_file(ss.str());
			return;
		}

		HGLOBAL hRes = LoadResource(module, imageResHandle);
		LPVOID memRes = LockResource(hRes);
		DWORD sizeRes = SizeofResource(module, imageResHandle);

		std::transform(bitmapName.begin(), bitmapName.end(), bitmapName.begin(), ::tolower);

		HANDLE newFile = CreateFileW(filePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (newFile == INVALID_HANDLE_VALUE)
		{
			write_text_to_log_file("Couldn't create img file in temp");
			return;
		}

		DWORD written = 0;
		if (!WriteFile(newFile, hRes, sizeRes, &written, NULL))
		{
			write_text_to_log_file("Couldn't write to img file in temp");
			CloseHandle(newFile);
			return;
		}
		else
		{
			CloseHandle(newFile);
		}
	}

	if (does_file_exist(filePathNonW))
	{
		int textureID = createTexture(filePathNonW);
		ALL_VEH_IMAGES.push_back({ model, (char*)LOCAL_TEXTURE_DICT, NULL, textureID });
	}

	delete filePathNonW;
}

void init_vehicle_feature()
{
	//copy all the ingame images
	ALL_VEH_IMAGES.insert(ALL_VEH_IMAGES.end(), INGAME_VEH_IMAGES.begin(), INGAME_VEH_IMAGES.end());

	// spawn name, defined name, image file name
	unpack_veh_preview("AIRTUG", VP_AIRTUG, "VP_AIRTUG");
	unpack_veh_preview("AMBULANCE", VP_AMBULANCE, "VP_AMBULANCE");
	unpack_veh_preview("ARMYTANKER", VP_ARMYTANKER, "VP_ARMYTANKER");
	unpack_veh_preview("ARMYTRAILER", VP_ARMYTRAILER, "VP_ARMYTRAILER");
	unpack_veh_preview("ARMYTRAILER2", VP_ARMYTRAILER2, "VP_ARMYTRAILER2");
	unpack_veh_preview("ASEA2", VP_ASEA_SNOW, "VP_ASEA_SNOW");

	unpack_veh_preview("BAGGER", VP_BAGGER, "VP_BAGGER");
	unpack_veh_preview("BALETRAILER", VP_BALETRAILER, "VP_BALETRAILER");
	unpack_veh_preview("BALLER2", VP_BALLER_SMALL, "VP_BALLER_SMALL"); //added baller 3 + 4
	unpack_veh_preview("BALLER3", VP_BALLER_3, "VP_BALLER3");
	unpack_veh_preview("BALLER4", VP_BALLER_4, "VP_BALLER4");
	unpack_veh_preview("BARRACKS2", VP_BARRACKS2, "VP_BARRACKS2");
	unpack_veh_preview("BENSON", VP_BENSON, "VP_BENSON");
	unpack_veh_preview("BIFF", VP_BIFF, "VP_BIFF");
	unpack_veh_preview("BISON2", VP_BISON2, "VP_BISON2");
	unpack_veh_preview("BISON3", VP_BISON3, "VP_BISON3");
	unpack_veh_preview("BJXL", VP_BJXL, "VP_BJXL");
	unpack_veh_preview("BLAZER2", VP_LIFEGUARD, "VP_LIFEGUARD");
	unpack_veh_preview("BLIMP", VP_BLIMP, "VP_BLIMP");
	unpack_veh_preview("BLIMP2", VP_BLIMP2, "VP_BLIMP2");
	unpack_veh_preview("BLISTA", VP_BLISTA, "VP_BLISTA");
	unpack_veh_preview("BOATTRAILER", VP_BOATTRAILER, "VP_BOATTRAILER");
	unpack_veh_preview("BOXVILLE", VP_BOXVILLE, "VP_BOXVILLE");
	unpack_veh_preview("BOXVILLE2", VP_BOXVILLE2, "VP_BOXVILLE2");
	unpack_veh_preview("BOXVILLE3", VP_BOXVILLE3, "VP_BOXVILLE3");
	unpack_veh_preview("BLISTA3", VP_MONKEY_BLISTA, "VP_MONKEY_BLISTA");
	unpack_veh_preview("BTYPE", VP_ROOSEVELT, "VP_ROOSEVELT");
	unpack_veh_preview("BTYPE2", VP_BTYPE_2, "VP_BTYPE2"); //added new
	unpack_veh_preview("BUCCANEER", VP_BUCCANEER, "VP_BUCCANEER");
	unpack_veh_preview("BURRITO", VP_BURRITO, "VP_BURRITO");
	unpack_veh_preview("BURRITO2", VP_BURRITO2, "VP_BURRITO2");
	unpack_veh_preview("BURRITO3", VP_BURRITO3, "VP_BURRITO3");
	unpack_veh_preview("BURRITO4", VP_BURRITO4, "VP_BURRITO4");
	unpack_veh_preview("BURRITO5", VP_BURRITO5, "VP_BURRITO5");
	unpack_veh_preview("BUZZARD2", VP_BUZZARD2, "VP_BUZZARD2");
	unpack_veh_preview("CABLECAR", VP_CABLECAR, "VP_CABLECAR");
	unpack_veh_preview("CADDY", VP_CADDY, "VP_CADDY");
	unpack_veh_preview("CADDY2", VP_CADDY2, "VP_CADDY2");
	unpack_veh_preview("CAMPER", VP_CAMPER, "VP_CAMPER");
	unpack_veh_preview("CARGOBOB2", VP_CARGOBOB2, "VP_CARGOBOB2");
	unpack_veh_preview("CARGOBOB3", VP_CARGOBOB3, "VP_CARGOBOB3");
	unpack_veh_preview("CUTTER", VP_CUTTER, "VP_CUTTER");
	unpack_veh_preview("CARGOPLANE", VP_CARGO, "VP_CARGO");
	unpack_veh_preview("DAEMON", VP_DAEMON, "VP_DAEMON");
	unpack_veh_preview("DILETTANTE2", VP_DILET_LIVERY, "VP_DILET_LIVERY");
	unpack_veh_preview("DINGHY", VP_DINGHY, "VP_DINGHY");
	unpack_veh_preview("DINGHY2", VP_DINGHY2, "VP_DINGHY2");
	unpack_veh_preview("HANDLER", VP_DOCKHANDLER, "VP_DOCKHANDLER");
	unpack_veh_preview("DOCKTRAILER", VP_DOCKTRAILER, "VP_DOCKTRAILER");
	unpack_veh_preview("DOCKTUG", VP_DOCKTUG, "VP_DOCKTUG");
	unpack_veh_preview("BULLDOZER", VP_DOZER, "VP_DOZER");
	unpack_veh_preview("DLOADER", VP_DUNELOADER, "VP_DUNELOADER");
	unpack_veh_preview("DUBSTA", VP_DUBSTA, "VP_DUBSTA");
	unpack_veh_preview("DUBSTA2", VP_DUBSTA_BLACK, "VP_DUBSTA_BLACK");
	unpack_veh_preview("DUKES", VP_DUKES, "VP_DUKES");
	unpack_veh_preview("DUNE2", VP_SPACEDOCKER, "VP_SPACEDOCKER");
	unpack_veh_preview("EMPEROR", VP_EMPEROR, "VP_EMPEROR");
	unpack_veh_preview("EMPEROR2", VP_EMPEROR_RUSTY, "VP_EMPEROR_RUSTY");
	unpack_veh_preview("EMPEROR3", VP_EMPEROR_SNOW, "VP_EMPEROR_SNOW");
	unpack_veh_preview("FBI", VP_FBI, "VP_FBI");
	unpack_veh_preview("FBI2", VP_FBI2, "VP_FBI2");
	unpack_veh_preview("FIRETRUK", VP_FIRETRUK, "VP_FIRETRUK");
	unpack_veh_preview("FIXTER", VP_FIXTER, "VP_FIXTER");
	unpack_veh_preview("FLATBED", VP_FLATBED, "VP_FLATBED");
	unpack_veh_preview("FORKLIFT", VP_FORKLIFT, "VP_FORKLIFT");
	unpack_veh_preview("FQ2", VP_FQ2, "VP_FQ2");
	unpack_veh_preview("FREIGHT", VP_FREIGHT, "VP_FREIGHT");
	unpack_veh_preview("FREIGHTCAR", VP_FREIGHTCAR, "VP_FREIGHTCAR");
	unpack_veh_preview("FREIGHTCONT1", VP_FREIGHTCONT1, "VP_FREIGHTCONT1");
	unpack_veh_preview("FREIGHTCONT2", VP_FREIGHTCONT2, "VP_FREIGHTCONT2");
	unpack_veh_preview("FREIGHTGRAIN", VP_FREIGHTGRAIN, "VP_FREIGHTGRAIN");
	unpack_veh_preview("FREIGHTTRAILER", VP_FREIGHTTRAILER, "VP_FREIGHTTRAILER");
	unpack_veh_preview("FROGGER2", VP_FROGGER2, "VP_FROGGER2");
	unpack_veh_preview("FUTO", VP_FUTO, "VP_FUTO");
	unpack_veh_preview("GBURRITO", VP_GBURRITO, "VP_GBURRITO");
	unpack_veh_preview("GRAINTRAILER", VP_GRAINTRAILER, "VP_GRAINTRAILER");
	unpack_veh_preview("GRANGER", VP_GRANGER, "VP_GRANGER");
	unpack_veh_preview("GUARDIAN", VP_GUARDIAN, "VP_GUARDIAN");
	unpack_veh_preview("HABANERO", VP_HABANERO, "VP_HABANERO");
	unpack_veh_preview("HAULER", VP_HAULER, "VP_HAULER");
	unpack_veh_preview("JET", VP_JET, "VP_JET");
	unpack_veh_preview("LAZER", VP_LAZER, "VP_LAZER");
	unpack_veh_preview("LECTRO", VP_LECTRO, "VP_LECTRO");
	unpack_veh_preview("LGUARD", VP_LGUARD, "VP_LGUARD");
	unpack_veh_preview("MANANA", VP_MANANA, "VP_MANANA");
	unpack_veh_preview("MESA2", VP_MESA_SNOW, "VP_MESA_SNOW");
	unpack_veh_preview("METROTRAIN", VP_METROTRAIN, "VP_METROTRAIN");
	unpack_veh_preview("MIXER", VP_MIXER, "VP_MIXER");
	unpack_veh_preview("MIXER2", VP_MIXER2, "VP_MIXER2");
	unpack_veh_preview("MOWER", VP_MOWER, "VP_MOWER");
	unpack_veh_preview("MULE2", VP_MULE2, "VP_MULE2");
	unpack_veh_preview("ORACLE2", VP_ORACLE2, "VP_ORACLE2");
	unpack_veh_preview("PACKER", VP_PACKER, "VP_PACKER");
	unpack_veh_preview("PATRIOT", VP_PATRIOT, "VP_PATRIOT");
	unpack_veh_preview("PEYOTE", VP_PEYOTE, "VP_PEYOTE");
	unpack_veh_preview("PHANTOM", VP_PHANTOM, "VP_PHANTOM");
	unpack_veh_preview("PHOENIX", VP_PHOENIX, "VP_PHOENIX");
	unpack_veh_preview("POLICE", VP_POLICE, "VP_POLICE");
	unpack_veh_preview("POLICE2", VP_POLICE2, "VP_POLICE2");
	unpack_veh_preview("POLICE3", VP_POLICE3, "VP_POLICE3");
	unpack_veh_preview("POLICE4", VP_POLICE4, "VP_POLICE4");
	unpack_veh_preview("POLICEB", VP_POLICEB, "VP_POLICEB");
	unpack_veh_preview("POLICEOLD1", VP_POLICEOLD1, "VP_POLICEOLD1");
	unpack_veh_preview("POLICEOLD2", VP_POLICEOLD2, "VP_POLICEOLD2");
	unpack_veh_preview("POLICET", VP_POLICET, "VP_POLICET");
	unpack_veh_preview("POLMAV", VP_POLMAV, "VP_POLMAV");
	unpack_veh_preview("PONY", VP_PONY, "VP_PONY");
	unpack_veh_preview("PONY2", VP_PONY2, "VP_PONY2");
	unpack_veh_preview("POUNDER", VP_POUNDER, "VP_POUNDER");
	unpack_veh_preview("PRAIRIE", VP_PRAIRIE, "VP_PRAIRIE");
	unpack_veh_preview("PRANGER", VP_PRANGER, "VP_PRANGER");
	unpack_veh_preview("PREDATOR", VP_PREDATOR, "VP_PREDATOR");
	unpack_veh_preview("PROPTRAILER", VP_PROPTRAILER, "VP_PROPTRAILER");
	unpack_veh_preview("RAKETRAILER", VP_RAKETRAILER, "VP_RAKETRAILER");
	unpack_veh_preview("RANCHERXL2", VP_RANCHERXL_SNOW, "VP_RANCHERXL_SNOW");
	unpack_veh_preview("RIOT", VP_RIOT, "VP_RIOT");
	unpack_veh_preview("RIPLEY", VP_RIPLEY, "VP_RIPLEY");
	unpack_veh_preview("ROMERO", VP_HEARSE, "VP_HEARSE");
	unpack_veh_preview("RUINER", VP_RUINER, "VP_RUINER");
	unpack_veh_preview("RUBBLE", VP_RUBBLE, "VP_RUBBLE");
	unpack_veh_preview("RUMPO2", VP_RUMPO2, "VP_RUMPO2");
	unpack_veh_preview("SABREGT", VP_SABRE, "VP_SABRE");
	unpack_veh_preview("SADLER2", VP_SADLER_SNOW, "VP_SADLER_SNOW");
	unpack_veh_preview("SCRAP", VP_SCRAP, "VP_SCRAP");
	unpack_veh_preview("SEASHARK2", VP_SEASHARK2, "VP_SEASHARK2");
	unpack_veh_preview("SENTINEL2", VP_SENTINEL, "VP_SENTINEL");
	unpack_veh_preview("SERRANO", VP_SERRANO, "VP_SERRANO");
	unpack_veh_preview("SHERIFF", VP_SHERIFF, "VP_SHERIFF");
	unpack_veh_preview("SHERIFF2", VP_SHERIFF2, "VP_SHERIFF2");
	unpack_veh_preview("SKYLIFT", VP_SKYLIFT, "VP_SKYLIFT");
	unpack_veh_preview("SLAMVAN2", VP_SLAMVAN_LOSTMC, "VP_SLAMVAN_LOSTMC");
	unpack_veh_preview("SPEEDO", VP_SPEEDO, "VP_SPEEDO");
	unpack_veh_preview("SPEEDO2", VP_SPEEDO2, "VP_SPEEDO2");
	unpack_veh_preview("STOCKADE", VP_STOCKADE, "VP_STOCKADE");
	unpack_veh_preview("STOCKADE3", VP_STOCKADE3, "VP_STOCKADE3");
	unpack_veh_preview("SUBMERSIBLE", VP_SUB, "VP_SUB");
	unpack_veh_preview("SULTAN", VP_SULTAN, "VP_SULTAN");
	unpack_veh_preview("SURFER2", VP_SURFER2, "VP_SURFER2");
	unpack_veh_preview("TACO", VP_TACO, "VP_TACO");
	unpack_veh_preview("TANKER", VP_TANKER, "VP_TANKER");
	unpack_veh_preview("TANKER2", VP_TANKER2, "VP_TANKER2");
	unpack_veh_preview("TANKERCAR", VP_TANKERCAR, "VP_TANKERCAR");
	unpack_veh_preview("TAXI", VP_TAXI, "VP_TAXI");
	unpack_veh_preview("TIPTRUCK", VP_TIPTRUCK, "VP_TIPTRUCK");
	unpack_veh_preview("TIPTRUCK2", VP_TIPTRUCK2, "VP_TIPTRUCK2");
	unpack_veh_preview("TORNADO", VP_TORNADO, "VP_TORNADO");
	unpack_veh_preview("TORNADO2", VP_TORNADO_CAB, "VP_TORNADO_CAB");
	unpack_veh_preview("TORNADO3", VP_TORNADO_RUSTY, "VP_TORNADO_RUSTY");
	unpack_veh_preview("TORNADO4", VP_TORNADO_CAB_RUSTY, "VP_TORNADO_CAB_RUSTY");
	unpack_veh_preview("TOURBUS", VP_TOURBUS, "VP_TOURBUS");
	unpack_veh_preview("TOWTRUCK", VP_TOWTRUCK, "VP_TOWTRUCK");
	unpack_veh_preview("TOWTRUCK2", VP_TOWTRUCK2, "VP_TOWTRUCK2");
	unpack_veh_preview("TR2", VP_TR2, "VP_TR2");
	unpack_veh_preview("TR3", VP_TR3, "VP_TR3");
	unpack_veh_preview("TR4", VP_TR4, "VP_TR4");
	unpack_veh_preview("TRACTOR", VP_TRACTOR, "VP_TRACTOR");
	unpack_veh_preview("TRACTOR2", VP_TRACTOR2, "VP_TRACTOR2");
	unpack_veh_preview("TRACTOR3", VP_TRACTOR3, "VP_TRACTOR3");
	unpack_veh_preview("TRAILERLOGS", VP_TRAILERLOGS, "VP_TRAILERLOGS");
	unpack_veh_preview("TRAILERS", VP_TRAILERS, "VP_TRAILERS");
	unpack_veh_preview("TRAILERS2", VP_TRAILERS2, "VP_TRAILERS2");
	unpack_veh_preview("TRAILERS3", VP_TRAILERS3, "VP_TRAILERS3");
	unpack_veh_preview("TRAILERSMALL", VP_TRAILERSMALL, "VP_TRAILERSMALL");
	unpack_veh_preview("TRASH", VP_TRASH, "VP_TRASH");
	unpack_veh_preview("TRASH2", VP_TRASH2, "VP_TRASH2");
	unpack_veh_preview("TRFLAT", VP_TRFLAT, "VP_TRFLAT");
	unpack_veh_preview("TVTRAILER", VP_TVTRAILER, "VP_TVTRAILER");
	unpack_veh_preview("UTILLITRUCK", VP_UTILLITRUCK, "VP_UTILLITRUCK");
	unpack_veh_preview("UTILLITRUCK2", VP_UTILLITRUCK2, "VP_UTILLITRUCK2");
	unpack_veh_preview("UTILLITRUCK3", VP_UTILLITRUCK3, "VP_UTILLITRUCK3");
	//unpack_veh_preview("VOODOO2", VP_VOODOO, "VP_VOODOO");
}

void fix_vehicle()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

			VEHICLE::SET_VEHICLE_FIXED(veh);
			VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(veh);
			VEHICLE::SET_VEHICLE_NEEDS_TO_BE_HOTWIRED(veh, false);

			if (FIRE::IS_ENTITY_ON_FIRE(veh))
			{
				FIRE::STOP_ENTITY_FIRE(veh);
			}

			VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 1000.0f);
			VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 1000.0f);
			VEHICLE::SET_VEHICLE_BODY_HEALTH(veh, 1000.f);

			VEHICLE::SET_VEHICLE_UNDRIVEABLE(veh, false);
			VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(veh, false);

			VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true);

			set_status_text("Vehicle repaired");
		}
		else
		{
			set_status_text("Player isn't in a vehicle");
		}
	}
}

void clean_vehicle()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(playerPed), 0);

			set_status_text("Vehicle cleaned");

		}
		else
		{
			set_status_text("Player isn't in a vehicle");
		}
	}
}


bool is_convertible_roofdown(std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int roofState = VEHICLE::GET_CONVERTIBLE_ROOF_STATE(veh);
	return ( roofState == 2 || roofState == 1);
}

void set_convertible_roofdown(bool applied, std::vector<int> extras)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	if (applied)
	{
		VEHICLE::LOWER_CONVERTIBLE_ROOF(veh, featureVehicleDoorInstant);
	}
	else
	{
		VEHICLE::RAISE_CONVERTIBLE_ROOF(veh, featureVehicleDoorInstant);
	}
}

void drive_passenger()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 coords = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
	Vehicle veh = VEHICLE::GET_CLOSEST_VEHICLE(coords.x, coords.y, coords.z, 20.f, 0, 70);

	if (veh) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
			Object mytask;
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::OPEN_SEQUENCE_TASK(&mytask);
			AI::TASK_LEAVE_ANY_VEHICLE(playerPed, 0, 0);
			AI::CLOSE_SEQUENCE_TASK(mytask);
			AI::TASK_PERFORM_SEQUENCE(playerPed, mytask);
			AI::CLEAR_SEQUENCE_TASK(&mytask);

		} else {
			set_status_text("Vehicle found");
			//		AI::TASK_ENTER_VEHICLE(playerPed, veh, 0, 0, 2, 1, 0);
			//		PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
			Object mytask;
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::OPEN_SEQUENCE_TASK(&mytask);
			AI::TASK_ENTER_VEHICLE(0, veh, -1, 0, 2, 1, 0);
			AI::TASK_VEHICLE_DRIVE_WANDER(0, veh, 30, 786599);
			AI::CLOSE_SEQUENCE_TASK(mytask);
			AI::TASK_PERFORM_SEQUENCE(playerPed, mytask);
			AI::CLEAR_SEQUENCE_TASK(&mytask);

			PED::SET_PED_CAN_BE_DRAGGED_OUT(playerPed, 0);
		}
		
	}
}

