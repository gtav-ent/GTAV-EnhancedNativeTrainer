/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "teleportation.h"
#include "misc.h"
#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\debug\debuglog.h"
#include "..\ent-enums.h"

struct tele_location {
	std::string text;
	float x;
	float y;
	float z;
	std::vector<const char*> scenery_required;
	std::vector<const char*> scenery_toremove;
	bool isLoaded;
};

int mainMenuIndex = 0;

int lastChosenCategory = -1;

int lastMenuChoiceInCategories[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

bool beingChauffeured = false;

Vehicle waitingToRetakeSeat = -1;

float chauffTolerance = 25.0;

Vector3 blipCoords = { 0, 0, 0 };

std::vector<tele_location> LOCATIONS_SAFE = {
	{ "Michael's Safehouse", -827.138f, 176.368f, 70.4999f },
	{ "Franklin's Safehouse", -18.0355f, -1456.94f, 30.4548f },
	{ "Franklin's Safehouse 2", 10.8766f, 545.654f, 175.419f },
	{ "Trevor's Safehouse", 1982.13f, 3829.44f, 32.3662f },
	{ "Trevor's Safehouse 2", -1157.05f, -1512.73f, 4.2127f },
	{ "Trevor's Safehouse 3", 91.1407f, -1280.65f, 29.1353f },
	{ "Michael's Safehouse Inside", -813.603f, 179.474f, 72.1548f },
	{ "Franklin's Safehouse Inside", -14.3803f, -1438.51f, 31.1073f },
	{ "Franklin's Safehouse 2 Inside", 7.11903f, 536.615f, 176.028f },
	{ "Trevor's Safehouse Inside", 1972.61f, 3817.04f, 33.4278f },
	{ "Trevor's Safehouse 2 Inside", -1151.77f, -1518.14f, 10.6327f },
	{ "Trevor's Safehouse 3 Inside", 96.1536f, -1290.73f, 29.2664f },
};

std::vector<tele_location> LOCATIONS_LANDMARKS = {
	{ "Airport Entrance", -1034.6f, -2733.6f, 13.8f },
	{ "Airport Field", -1336.0f, -3044.0f, 13.9f },
	{ "Airport Entrance Tower", -912.523f, -2529.81f, 41.96f },
	{ "Airport Tower Roof", -982.67f, -2638.2f, 89.522f },
	{ "Altruist Cult Camp", -1170.841f, 4926.646f, 224.295f },
	{ "Calafia Train Bridge", -517.869f, 4425.284f, 89.795f },
	{ "Cargo Ship", 899.678f, -2882.191f, 19.013f },
	{ "Chumash", -3192.6f, 1100.0f, 20.2f },
	{ "Chumash Historic Family Pier", -3426.683f, 967.738f, 8.347f },
	{ "Del Perro Pier", -1850.127f, -1231.751f, 13.017f },
	{ "Devin Weston's House", -2639.872f, 1866.812f, 160.135f },
	{ "El Burro Heights", 1384.0f, -2057.1f, 52.0f },
	{ "Elysian Island", 338.2f, -2715.9f, 38.5f },
	{ "Far North San Andreas", 24.775f, 7644.102f, 19.055f },
	{ "Ferris Wheel", -1670.7f, -1125.0f, 13.0f },
	{ "Fort Zancudo", -2047.4f, 3132.1f, 32.8f },
	{ "Fort Zancudo ATC Entrance", -2344.373f, 3267.498f, 32.811f },
	{ "Fort Zancudo ATC Top Floor", -2358.132f, 3249.754f, 101.451f },
	{ "God's Thumb", -1006.402f, 6272.383f, 1.503f },
	{ "Hippy Camp", 2476.712f, 3789.645f, 41.226f },
	{ "Jetsam", 760.4f, -2943.2f, 5.8f },
	{ "Jolene Cranley-Evans Ghost", 3059.620f, 5564.246f, 197.091f },
	{ "Kortz Center", -2243.810f, 264.048f, 174.615f },
	{ "Main LS Customs", -365.425f, -131.809f, 37.873f },
	{ "Marlowe Vineyards", -1868.971f, 2095.674f, 139.115f },
	{ "McKenzie Airfield", 2121.7f, 4796.3f, 41.1f },
	{ "Merryweather Dock", 486.417f, -3339.692f, 6.070f },
	{ "Mineshaft", -595.342f, 2086.008f, 131.412f },
	{ "Mt. Chiliad", 425.4f, 5614.3f, 766.5f },
	{ "Mt. Chiliad Summit", 450.718f, 5566.614f, 806.183f },
	{ "NOOSE Headquarters", 2535.243f, -383.799f, 92.993f },
	{ "Pacific Standard Bank", 235.046f, 216.434f, 106.287f },
	{ "Paleto Bay Pier", -275.522f, 6635.835f, 7.425f },
	{ "Playboy Mansion", -1475.234f, 167.088f, 55.841f },
	{ "Police Station", 436.491f, -982.172f, 30.699f },
	{ "Quarry", 2954.196f, 2783.410f, 41.004f },
	{ "Sandy Shores Airfield", 1747.0f, 3273.7f, 41.1f },
	{ "Satellite Dishes", 2062.123f, 2942.055f, 47.431f },
	{ "Sisyphus Theater Stage", 686.245f, 577.950f, 130.461f },
	{ "Trevor's Meth Lab", 1391.773f, 3608.716f, 38.942f },
	{ "Weed Farm", 2208.777f, 5578.235f, 53.735f },
	{ "Wind Farm", 2354.0f, 1830.3f, 101.1f }
};

// Extra locations coordinates source: "PulseR_HD" @ http://gtaforums.com/topic/789786-vrelwip-simple-trainer-enhancements-skin-detail-chooser-menu-architecture/?p=1067398379

std::vector<tele_location> LOCATIONS_HIGH = {
	{ "Airplane Graveyard Airplane Tail ", 2395.096f, 3049.616f, 60.053f },
	{ "Bridge Pole", -279.242f, -2438.71f, 124.004f },
	{ "Building Crane Top", -119.879f, -977.357f, 304.249f },
	{ "FIB Building Roof", 150.126f, -754.591f, 262.865f },
	{ "Galileo Observatory Roof", -438.804f, 1076.097f, 352.411f },
	{ "IAA Building Roof", 134.085f, -637.859f, 262.851f },
	{ "Maze Bank Arena Roof", -324.300f, -1968.545f, 67.002f },
	{ "Maze Bank Roof", -75.015f, -818.215f, 326.176f },
	{ "Palmer-Taylor Power Station Chimney", 2732.931f, 1577.540f, 83.671f },
	{ "Rebel Radio", 736.153f, 2583.143f, 79.634f },
	{ "Sandy Shores Building Site Crane", 1051.209f, 2280.452f, 89.727f },
	{ "Satellite Dish Antenna", 2034.988f, 2953.105f, 74.602f },
	{ "Stab City", 126.975f, 3714.419f, 46.827f },
	{ "Very High Up", -129.964f, 8130.873f, 6705.307f },
	{ "Very Very High Up", -119.879f, -977.357f, 10000.0f },
	{ "Windmill Top", 2026.677f, 1842.684f, 133.313f }
};

std::vector<tele_location> LOCATIONS_UNDERWATER = {
	{ "Dead Sea Monster", -3373.726f, 504.714f, -24.656f },
	{ "Humane Labs Tunnel", 3838.47f, 3673.06f, -19.7281f, {}, {"chemgrill_grp1"}, false },
	{ "Sunken Body", -3161.078f, 3001.998f, -37.974f },
	{ "Sunken Cargo Ship", 3199.748f, -379.018f, -22.500f },
	{ "Sunken Plane", -942.350f, 6608.752f, -20.912f },
	{ "Underwater Hatch", 4273.950f, 2975.714f, -170.746f },
	{ "Underwater UFO", 762.426f, 7380.371f, -111.377f },
	{ "Underwater WW2 Tank", 4201.633f, 3643.821f, -39.016f },
};

std::vector<tele_location> LOCATIONS_INTERIORS = {
	{ "10 Car Garage Back Room", 223.193f, -967.322f, -99.000f },
	{ "10 Car Garage Bay", 228.135f, -995.350f, -99.000f },
	{ "Ammunation Gun Range", 22.153f, -1072.854f, 29.797f },
	{ "Ammunation Office", 12.494f, -1110.130f, 29.797f },
	//{ "Bahama Mamas West", -1387.08f, -588.4f, 30.3195f },
	{ "Blaine County Savings Bank", -109.299f, 6464.035f, 31.627f },
	{ "Clucking Bell Farms Warehouse", -70.0624f, 6263.53f, 31.0909f, { "CS1_02_cf_onmission1", "CS1_02_cf_onmission2", "CS1_02_cf_onmission3", "CS1_02_cf_onmission4" }, { "CS1_02_cf_offmission" }, false },
	//{ "Control Office", 1080.97f, -1976.0f, 31.4721f },
	//{ "Devin's Garage", 482.027f, -1317.96f, 29.2021f },
	{ "Eclipse Apartment 5", -762.762f, 322.634f, 175.401f },
	{ "Eclipse Apartment 9", -790.673f, 334.468f, 158.599f },
	{ "Eclipse Apartment 31", -762.762f, 322.634f, 221.855f },
	{ "Eclipse Apartment 40", -790.673f, 334.468f, 206.218f },
	{ "FIB Building Burnt", 159.553f, -738.851f, 246.152f },
	{ "FIB Building Floor 47", 134.573f, -766.486f, 234.152f },
	{ "FIB Building Floor 49", 134.635f, -765.831f, 242.152f },
	{ "FIB Building Lobby", 110.4f, -744.2f, 45.7f, { "FIBlobby" }, { "FIBlobbyfake" } },
	{ "FIB Building Top Floor", 135.733f, -749.216f, 258.152f },
	//{ "Garment Factory", 717.397f, -965.572f, 30.3955f },
	{ "Hospital (Destroyed)", 302.651f, -586.293f, 43.3129f, { "RC12B_Destroyed", "RC12B_HospitalInterior" }, { "RC12B_Default", "RC12B_Fixed" }, false },
	{ "Humane Labs Lower Level", 3525.495f, 3705.301f, 20.992f },
	{ "Humane Labs Upper Level", 3618.52f, 3755.76f, 28.6901f },
	{ "IAA Office", 117.220f, -620.938f, 206.047f },
	//{ "Ice Planet Jewelery", 243.516f, 364.099f, 105.738f },
	{ "Janitor's Apartment", -110.721f, -8.22095f, 70.5197f },
	{ "Jewel Store", -630.07f, -236.332f, 38.0571f, { "post_hiest_unload" }, { "jewel2fake", "bh1_16_refurb" }, false },
	{ "Lester's House", 1273.898f, -1719.304f, 54.771f },
	{ "Life Invader Office", -1049.13f, -231.779f, 39.0144f, { "facelobby" }, { "facelobbyfake" }, false },
	//{ "Maze Bank Arena", -254.918f, -2019.75f, 30.1456f },
	{ "Morgue", 275.446f, -1361.11f, 24.5378f, { "Coroner_Int_on" }, {"Coroner_Int_off"}, false },
	{ "O'Neil Farm", 2454.78f, 4971.92f, 46.8103f, { "farm", "farm_props", "farmint" }, { "farm_burnt", "farm_burnt_props", "farmint_cap" }, false },
	{ "Pacific Standard Bank Vault", 255.851f, 217.030f, 101.683f },
	//{ "Paleto Bay Sheriff", -446.135f, 6012.91f, 31.7164f },
	//{ "Raven Slaughterhouse", 967.357f, -2184.71f, 30.0613f },
	//{ "Rogers Salvage & Scrap", -609.962f, -1612.49f, 27.0105f },
	//{ "Sandy Shores Sheriff", 1853.18f, 3686.63f, 34.2671f },
	//{ "Simeon's Dealership", -56.4951f, -1095.8f, 26.4224f },
	//{ "Split Sides West Comedy Club", -564.261f, 278.232f, 83.1364f },
	{ "Stadium", -248.4916f, -2010.509f, 34.5743f, {"SP1_10_real_interior"}, {"SP1_10_fake_interior"}, false },
	{ "Strip Club DJ Booth", 126.135f, -1278.583f, 29.270f },
	{ "Torture Warehouse", 136.514f, -2203.15f, 7.30914f },

};

std::vector<const char*> IPLS_CARRIER = {
	"hei_carrier",
	"hei_carrier_DistantLights",
	"hei_Carrier_int1",
	"hei_Carrier_int2",
	"hei_Carrier_int3",
	"hei_Carrier_int4",
	"hei_Carrier_int5",
	"hei_Carrier_int6",
	"hei_carrier_LODLights"
};

std::vector<const char*> IPLS_HEISTYACHT = {
	"hei_yacht_heist",
	"hei_yacht_heist_Bar",
	"hei_yacht_heist_Bedrm",
	"hei_yacht_heist_Bridge",
	"hei_yacht_heist_DistantLights",
	"hei_yacht_heist_enginrm",
	"hei_yacht_heist_LODLights",
	"hei_yacht_heist_Lounge"
};

std::vector<const char*> IPLS_HEIST_APT_1 = {
	"hw1_blimp_interior_v_apartment_high_milo_",
	"hw1_blimp_interior_v_apartment_high_milo__1",
	"hw1_blimp_interior_v_apartment_high_milo__2",
	"hw1_blimp_interior_v_apartment_high_milo__3",
	"hw1_blimp_interior_v_apartment_high_milo__4",
	"hw1_blimp_interior_v_apartment_high_milo__5",
	"hw1_blimp_interior_v_apartment_high_milo__6",
	"hw1_blimp_interior_v_apartment_high_milo__7",
	"hw1_blimp_interior_v_apartment_high_milo__8",
	"hw1_blimp_interior_v_apartment_high_milo__9",
	"hw1_blimp_interior_v_apartment_high_milo__10",
	"hw1_blimp_interior_v_apartment_high_milo__11",
	"hw1_blimp_interior_v_apartment_high_milo__12",
	"hw1_blimp_interior_v_apartment_high_milo__13",
	"hw1_blimp_interior_v_apartment_high_milo__14",
	"hw1_blimp_interior_v_apartment_high_milo__15",
	"hw1_blimp_interior_v_apartment_high_milo__16",
	"hw1_blimp_interior_v_apartment_high_milo__17",
	"hw1_blimp_interior_v_apartment_high_milo__18",
	"hw1_blimp_interior_v_apartment_high_milo__19"
};

std::vector<const char*> IPLS_HEIST_APT_2 = {
	"hei_hw1_blimp_interior_8_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_9_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_10_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_11_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_12_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_13_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_14_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_15_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_16_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_17_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_18_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_19_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_20_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_21_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_22_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_23_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_24_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_25_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_26_dlc_apart_high_new_milo_",
	"hei_hw1_blimp_interior_27_dlc_apart_high_new_milo_"
};

std::vector<const char*> IPLS_HEIST_APT_3 = {
	"mpbusiness_int_placement",
	"mpbusiness_int_placement_interior_v_mp_apt_h_01_milo_",
	"mpbusiness_int_placement_interior_v_mp_apt_h_01_milo__1",
	"mpbusiness_int_placement_interior_v_mp_apt_h_01_milo__2",
	"mpbusiness_int_placement_interior_v_mp_apt_h_01_milo__3",
	"mpbusiness_int_placement_interior_v_mp_apt_h_01_milo__4",
	"mpbusiness_int_placement_interior_v_mp_apt_h_01_milo__5"
};

std::vector<const char*> IPLS_HEIST_APT_4 = {
	"hei_hw1_blimp_interior_28_dlc_apart_high2_new_milo",
	"hei_hw1_blimp_interior_29_dlc_apart_high2_new_milo",
	"hei_hw1_blimp_interior_30_dlc_apart_high2_new_milo",
	"hei_hw1_blimp_interior_31_dlc_apart_high2_new_milo",
	"hei_hw1_blimp_interior_32_dlc_apart_high2_new_milo",
	"hei_hw1_blimp_interior_33_dlc_apart_high2_new_milo",
};

std::vector<const char*> IPLS_HEIST_APT_5 = {
	"hei_mpheist_int_placement_interior_20_dlc_apart_high2_new_milo_",
	"hei_mpheist_int_placement_interior_21_dlc_apart_high2_new_milo_",
	"hei_mpheist_int_placement_interior_22_dlc_apart_high2_new_milo_",
	"hei_mpheist_int_placement_interior_23_dlc_apart_high2_new_milo_",
	"hei_mpheist_int_placement_interior_24_dlc_apart_high2_new_milo_",
	"hei_mpheist_int_placement_interior_25_dlc_apart_high2_new_milo_",
};

std::vector<const char*> IPLS_NORTH_YANKTON = {
	"plg_01",
	"prologue01",
	"prologue01_lod",
	"prologue01c",
	"prologue01c_lod",
	"prologue01d",
	"prologue01d_lod",
	"prologue01e",
	"prologue01e_lod",
	"prologue01f",
	"prologue01f_lod",
	"prologue01g",
	"prologue01h",
	"prologue01h_lod",
	"prologue01i",
	"prologue01i_lod",
	"prologue01j",
	"prologue01j_lod",
	"prologue01k",
	"prologue01k_lod",
	"prologue01z",
	"prologue01z_lod",
	"plg_02",
	"prologue02",
	"prologue02_lod",
	"plg_03",
	"prologue03",
	"prologue03_lod",
	"prologue03b",
	"prologue03b_lod",
	//the commented code disables the 'Prologue' grave and
	//enables the 'Bury the Hatchet' grave
	//"prologue03_grv_cov",
	//"prologue03_grv_cov_lod",
	"prologue03_grv_dug",
	"prologue03_grv_dug_lod",
	//"prologue03_grv_fun",
	"prologue_grv_torch",
	"plg_04",
	"prologue04",
	"prologue04_lod",
	"prologue04b",
	"prologue04b_lod",
	"prologue04_cover",
	"des_protree_end",
	"des_protree_start",
	"des_protree_start_lod",
	"plg_05",
	"prologue05",
	"prologue05_lod",
	"prologue05b",
	"prologue05b_lod",
	"plg_06",
	"prologue06",
	"prologue06_lod",
	"prologue06b",
	"prologue06b_lod",
	"prologue06_int",
	"prologue06_int_lod",
	"prologue06_pannel",
	"prologue06_pannel_lod",
	//"prologue_m2_door",
	//"prologue_m2_door_lod",
	"plg_occl_00",
	"prologue_occl",
	"plg_rd",
	"prologuerd",
	"prologuerdb",
	"prologuerd_lod"
};


std::vector<tele_location> LOCATIONS_REQSCEN = {
	//{ "Carrier", 3069.330f, -4632.4f, 15.043f, IPLS_CARRIER, {}, false },
	{ "Fort Zancudo UFO", -2052.000f, 3237.000f, 1456.973f, { "ufo", "ufo_lod", "ufo_eye" }, {}, false },
	//{ "Heist Yacht", -2043.974f, -1031.582f, 11.981f, IPLS_HEISTYACHT, {}, false },
	{ "Max Renda Refit", -583.1606f, -282.3967f, 35.394f, {"refit_unload"}, {"bh1_16_doors_shut"}, false },
	{ "North Yankton", 3360.19f, -4849.67f, 111.8f, IPLS_NORTH_YANKTON, {}, false },
	{ "North Yankton Bank", 5309.519f, -5212.375f, 83.522f, IPLS_NORTH_YANKTON, {}, false },
	{ "Simeon's Showroom", -59.7936f, -1098.784f, 27.2612f, { "shr_int" }, { "fakeint" }, false },
	{ "SS Bulker (Intact)", -163.749f, -2377.94f, 9.3192f, { "cargoship" }, { "sunkcargoship" }, false },
	{ "SS Bulker (Sunk)", -162.8918f, -2365.769f, 0.0f, { "sunkcargoship" }, { "cargoship" }, false },
	{ "Train Crash Bridge", -532.1309f, 4526.187f, 88.7955f, { "canyonriver01_traincrash", "railing_end" }, { "railing_start", "canyonriver01" }, false },
	{ "Yacht", -2023.661f, -1038.038f, 5.577f, { "smboat", "smboat_lod" }, {}, false },
};

std::vector<tele_location> LOCATIONS_BROKEN = {
	//{ "Carrier", 3069.330f, -4704.220f, 15.043f, IPLS_CARRIER, {}, false },
	{ "des_farmhouse", 2447.9f, 4973.4f, 47.7f, {}, {}, false },
	{ "canyon", -1600.6194f, 4443.4565f, 0.725f, {}, {}, false },

	{ "vb_30_crimetape", -1150.0391f, -1521.761f, 9.6331f, {}, {}, false },
	{ "sheriff_cap", 1856.0288f, 3682.9983f, 33.2675f, {}, {}, false },
	{ "CS1_16_sheriff_Cap", -440.5073f, 6018.766f, 30.49f, {}, {}, false },
	{ "chemgrill_grp1", 3832.9f, 3665.5f, -23.4f, {}, {}, false },
	{ "Hospitaldoorsfixed", {}, {}, false },
	{ "SP1_10_fake/real_interior", -248.4916f, -2010.509f, 34.5743f, {}, {}, false },
	{ "id2_14_pre_no_int etc", 716.84f, -962.05f, 31.59f, {}, {}, false },
	{ "burnt_switch_off", 716.84f, -962.05f, 31.59f, {}, {}, false },
	{ "des_farmhouse", 2447.9f, 4973.4f, 47.7f, {}, {}, false },
	{ "FINBANK (1)", 2.69689322f, -667.0166f, 16.1306286f, {}, {}, false },
	{ "FINBANK (2)", 6.194215f, -660.759338f, 33.4501877f, {}, {}, false },
	{ "DT1_03_Shutter", 23.9346f, -669.7552f, 30.8853f, {}, {}, false },
	{ "CS3_07_MPGates", -1601.424072265625, 2808.212646484375, 16.2598, {}, {}, false },
	{ "PaperRCM", -1459.1273193359375f, 486.12811279296875f, 115.20159912109375f, {}, {}, false },
	{"KorizTempWalls", -2199.1376953125, 223.4647979736328, 181.11180114746094, {}, {}, false },
	{"mic3_chopper_debris", -2242.78466796875, 263.4779052734375, 173.6154022216797 },
	{ "showroom", -59.79359817504883, -1098.7840576171875, 27.2612 },
	{"FBI_colPLUG, repair, rubble", 74.29, -736.0499877929688, 46.76 },
	{ "FBI heist", 136.00399780273438, -749.2869873046875, 153.302 }
	//{ "Director Mod Trailer", -20.004f, -10.889f, 500.602f },
};

std::vector<tele_location> LOCATIONS_JELLMAN = {
	{ "Aircraft Carrier", 3069.330f, -4704.220f, 15.043f, IPLS_CARRIER, {}, false },
	{ "Heist Police Station", 445.488f, -983.779f, 30.6896f, { "" }, {}, false },
	{ "Heist Yacht", -2043.974f, -1031.582f, 11.981f, IPLS_HEISTYACHT, {}, false },
	{ "Integrity Way Apt 28", -14.7964, -581.709, 79.4307, {}, {}, false },
	{ "Del Perro Heights Apt 28", -1468.14f, -541.815f, 73.4442f, {}, {}, false },
	{ "Richard Majestic Apt 2", -915.811f, -379.432f, 113.675f, {}, {}, false },
	{ "Tinsel Towers Apt 42", -614.86f, 40.6783f, 97.6f, {}, {}, false },
	{ "Eclipse Towers Apt 3", -773.407, 341.766, 211.397, {}, {}, false }
	//{ "High Life Garage", 228.135f, -995.350f, -99.000f, { "hei_hw1_blimp_interior_2_dlc_garage_high_new_milo_" }, { "hw1_blimp_interior_v_garagel_milo_" }, false },
};

std::string JELLMAN_CAPTION = "Heist Map Updates In SP";

static std::vector<std::string> MENU_LOCATION_CATEGORIES{ "Safehouses", "Landmarks", "Roof/High Up", "Underwater", "Interiors", "Extra Exterior Scenery", JELLMAN_CAPTION };// "Test", "Toggles" };

static std::vector<tele_location> VOV_LOCATIONS[] = { LOCATIONS_SAFE, LOCATIONS_LANDMARKS, LOCATIONS_HIGH, LOCATIONS_UNDERWATER, LOCATIONS_INTERIORS, LOCATIONS_REQSCEN, LOCATIONS_JELLMAN };// , LOCATIONS_BROKEN };

void teleport_to_coords(Entity e, Vector3 coords)
{
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
	WAIT(0);
	set_status_text("Teleported");
}

void output_current_location(Entity e)
{
	Vector3 coords = ENTITY::GET_ENTITY_COORDS(e, 0);
	std::ostringstream ss;
	ss << "X: " << coords.x << "\nY: " << coords.y << "\nZ: " << coords.z;
	set_status_text_centre_screen(ss.str(), 4000UL);
}

Vector3 get_blip_marker()
{
	static Vector3 zero;
	Vector3 coords;

	bool blipFound = false;
	// search for marker blip
	int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
	for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
	{
		if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
		{
			coords = UI::GET_BLIP_INFO_ID_COORD(i);
			blipFound = true;
			break;
		}
	}
	if (blipFound)
	{
		return coords;
	}

	set_status_text("Map marker isn't set");
	return zero;
}

void teleport_to_marker()
{
	Vector3 coords = get_blip_marker();
	
	if (coords.x + coords.y == 0) return;
	
	// get entity to teleport
	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
	{
		e = PED::GET_VEHICLE_PED_IS_USING(e);
	}

	// load needed map region and check height levels for ground existence
	bool groundFound = false;
	static float groundCheckHeight[] = 
	{ 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
	for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
		WAIT(100);
		if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z))
		{
			groundFound = true;
			coords.z += 3.0;
			break;
		}
	}
	// if ground not found then set Z in air and give player a parachute
	if (!groundFound)
	{
		coords.z = 1000.0;
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
	}
	//do it
	teleport_to_coords(e, coords);
}

void teleport_to_last_vehicle()
{
	Vehicle veh = PLAYER::GET_PLAYERS_LAST_VEHICLE();
	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
		if (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh)))
		{
			VEHICLE::SET_HELI_BLADES_FULL_SPEED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
		}
	}
	else
	{
		set_status_text("No vehicle found");
	}
}

bool is_player_at_blip(Vector3 currentCords, Vector3 destCords, float tolerance)
{
	float eucDistance;

	float xDiff = destCords.x - currentCords.x;
	float yDiff = destCords.y - currentCords.y;

	eucDistance = sqrt(pow(xDiff, 2) + pow(yDiff, 2));

	return (eucDistance <= tolerance);
}

float get_euc_distance(Vector3 currentCords, Vector3 destCords)
{
	float xDiff = destCords.x - currentCords.x;
	float yDiff = destCords.y - currentCords.y;
	float zDiff = destCords.y - currentCords.y;

	float eucDistance = sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));

	return eucDistance;
}

void get_chauffeur_to_marker()
{
	beingChauffeured = true;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	blipCoords = get_blip_marker();

	if (blipCoords.x == 0 && blipCoords.y == 0)
	{
		// no blip marker set
		return;
	}

	if (is_player_at_blip(playerCoords, blipCoords, chauffTolerance))
	{
		set_status_text("You're already at your destination");
		return;
	}

	Vector3 spawn_coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);

	Vehicle veh;
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, 0);

		if (is_this_a_heli_or_plane(veh))
		{
			set_status_text("Aircraft chauffeuring not supported yet");
			return;
		}

		if (!VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -1))
		{
			Ped oldDriver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1);
			if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, -2))
			{
				PED::SET_PED_INTO_VEHICLE(oldDriver, veh, -2);
			}
			else
			{
				if (oldDriver == playerPed)
				{
					set_status_text("Couldn't make room for your chauffeur");
					return;
				}
				else
				{
					PED::DELETE_PED(&oldDriver);
				}
			}
		}
	}
	else
	{
		//random supercar
		int carIndex = rand() % VALUES_SUPERCARS.size();
		std::string carName = VALUES_SUPERCARS.at(carIndex);
		Hash vehHash = GAMEPLAY::GET_HASH_KEY((char*)carName.c_str());

		STREAMING::REQUEST_MODEL(vehHash);
		while (!STREAMING::HAS_MODEL_LOADED(vehHash))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		FLOAT lookDir = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		veh = VEHICLE::CREATE_VEHICLE(vehHash, spawn_coords.x, spawn_coords.y, spawn_coords.z, lookDir, 1, 0);
	}

	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(blipCoords.x, blipCoords.y, blipCoords.z, &blipCoords.z);
	blipCoords.z += 3.0;

	Hash driverPedHash;
	if (is_this_a_heli_or_plane(veh))
	{
		driverPedHash = GAMEPLAY::GET_HASH_KEY("s_m_y_pilot_01");
	}
	else
	{
		driverPedHash = GAMEPLAY::GET_HASH_KEY("A_C_CHIMP");
	}
	STREAMING::REQUEST_MODEL(driverPedHash);
	while (!STREAMING::HAS_MODEL_LOADED(driverPedHash))
	{
		make_periodic_feature_call();
		WAIT(0);
	}

	Ped driver = PED::CREATE_PED(25, driverPedHash, spawn_coords.x, spawn_coords.y, spawn_coords.z, 0, false, false);

	while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
	{
		make_periodic_feature_call();
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
		WAIT(0);
	}

	char* playerName = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
	if (playerName != NULL && strlen(playerName) != 0)
	{
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, playerName);
	}

	//random metallic paint
	int useless, wheelCol;//pearl topcoat, wheel color
	int paintIndex = rand() % PAINTS_METALLIC.size();
	PaintColour paint = PAINTS_METALLIC.at(paintIndex);
	VEHICLE::SET_VEHICLE_COLOURS(veh, paint.mainValue, paint.mainValue);
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &useless, &wheelCol);
	VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, paint.pearlAddition, wheelCol);

	PED::SET_PED_INTO_VEHICLE(driver, veh, -1);
	set_old_vehicle_state(false); // set old vehicle state to false since we changed cars but didn't actually exit the last one

	/* DRIVING MODES :
	0 = Normal behaviour but doesn't recognize other cars on the road, should only be used without ped cars in world.
	1 = Drives legit and does no overtakes.Drives carefully
	2 = Normal behaviour but doesn't recognize other cars on the road, should only be used without ped cars in world.
	3 = Drives legit and does normal overtakes.Ignores traffic lights, and avoids other cars
	4 = Drives legit and does normal overtakes.Ignores traffic lights, and avoids other cars(fast accelerate, chase ? )
	5 = Drives legit and does normal overtakes.Ignores traffic lights, and avoids other cars
	6 = Drives legit and does normal overtakes.Ignores traffic lights, and avoids other cars
	7 = Drives legit and does overtakes depending on speed ? Drives carefully
	8 = Normal behaviour but doesn't recognize other cars on the road, should only be used without ped cars in world.
	9 = Drives legit and does no overtakes.Drives carefully
	10 = Normal behaviour but doesn't recognize other cars on the road, should only be used without ped cars in world.
	*/
	//AI::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(ped, veh, blipCoords.x, blipCoords.y, blipCoords.z, 100, 5, chauffTolerance);

	if (is_this_a_heli_or_plane(veh))
	{
		//TODO
	}
	else
	{
		if (get_euc_distance(playerCoords, blipCoords) >= 1000.0)
		{
			AI::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(driver, veh, blipCoords.x, blipCoords.y, blipCoords.z, 40.0, 4, chauffTolerance);
		}
		else
		{
			AI::TASK_VEHICLE_DRIVE_TO_COORD(driver, veh, blipCoords.x, blipCoords.y, blipCoords.z, 40.0, 1, ENTITY::GET_ENTITY_MODEL(veh), 4, -1.0, -1.0);
		}
	}
}

void cancel_chauffeur(std::string message)
{
	Object taskHdl;

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1);

		VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, FALSE, true);
		if (ENTITY::DOES_ENTITY_EXIST(driver))
		{
			if (driver != PLAYER::PLAYER_PED_ID())
			{
				AI::CLEAR_PED_TASKS(driver);

				AI::OPEN_SEQUENCE_TASK(&taskHdl);
				AI::TASK_LEAVE_VEHICLE(driver, veh, 1);
				AI::TASK_WANDER_STANDARD(driver, 100.0, 1);
				AI::CLOSE_SEQUENCE_TASK(taskHdl);

				AI::TASK_PERFORM_SEQUENCE(driver, taskHdl);
				AI::CLEAR_SEQUENCE_TASK(&taskHdl);

				waitingToRetakeSeat = veh;
			}
		}
	}



	std::ostringstream ss;
	ss << message;
	set_status_text(ss.str());
	beingChauffeured = false;
}

bool onconfirm_teleport_category(MenuItem<int> choice)
{
	Entity e = PLAYER::PLAYER_PED_ID();
	if (choice.value == -2)
	{
		teleport_to_marker();
		return false;
	}
	else if (choice.value == -3)
	{
		if (beingChauffeured)
		{
			cancel_chauffeur("Chauffeur cancelled");
		}
		else
		{
			get_chauffeur_to_marker();
		}
		return false;
	}
	else if (choice.value == -4)
	{
		teleport_to_last_vehicle();
		return false;
	}
	else if (choice.value == -1)
	{
		output_current_location(e);
		return false;
	}
	else if (choice.value == -5)
	{
		process_toggles_menu();
		return false;
	}

	lastChosenCategory = choice.value;

	if (process_teleport_menu(lastChosenCategory))
	{
		return true;
	}
	return false;
}

bool onconfirm_teleport_location(MenuItem<int> choice)
{
	lastMenuChoiceInCategories[lastChosenCategory] = choice.value;

	tele_location* value = &VOV_LOCATIONS[lastChosenCategory][choice.value];

	// get entity to teleport
	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
	{
		e = PED::GET_VEHICLE_PED_IS_USING(e);
	}

	Vector3 coords;

	if ((value->scenery_required.size() > 0 || value->scenery_toremove.size() > 0) && !value->isLoaded)
	{
		set_status_text("Loading new scenery...");
		
		if ( ENTITY::DOES_ENTITY_EXIST ( PLAYER::PLAYER_PED_ID () ) )// && STREAMING::IS_IPL_ACTIVE("plg_01") == 0)
		{
			for each (const char* scenery in value->scenery_toremove)
			{
				if (STREAMING::IS_IPL_ACTIVE(scenery))
				{
					STREAMING::REMOVE_IPL(scenery);
				}
			}
			for each ( const char* scenery in value->scenery_required )
			{
				if (!STREAMING::IS_IPL_ACTIVE(scenery))
				{
					STREAMING::REQUEST_IPL(scenery);
				}
			}
		}

		value->isLoaded = true;

		DWORD time = GetTickCount() + 1000;
		while (GetTickCount() < time)
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		set_status_text("New scenery loaded");

		time = GetTickCount() + 1000;
		while (GetTickCount() < time)
		{
			make_periodic_feature_call();
			WAIT(0);
		}
	}

	coords.x = value->x;
	coords.y = value->y;
	coords.z = value->z;
	teleport_to_coords(e, coords);

	bool unloadedAnything = false;
	DWORD time = GetTickCount() + 1000;

	for (int x = 0; x < MENU_LOCATION_CATEGORIES.size(); x++)
	{
		/*
		//Added to avoid showing debug toggle menu.
		if (x == MENU_LOCATION_CATEGORIES.size() - 1)
		{
			continue;
		}
		*/

		for (int y = 0; y < VOV_LOCATIONS[x].size(); y++)
		{
			//don't unload our newly loaded scenery
			if (x == lastChosenCategory && y == choice.value)
			{
				continue;
			}

			tele_location* loc = &VOV_LOCATIONS[x][y];

			//don't unload something using same loader
			if (loc->scenery_required == value->scenery_required && loc->scenery_toremove == value->scenery_toremove)
			{
				continue;
			}

			if (loc->isLoaded && loc->scenery_required.size() > 0)
			{
				if (!unloadedAnything)
				{
					set_status_text("Unloading old scenery...");
					time = GetTickCount() + 1000;
					while (GetTickCount() < time)
					{
						make_periodic_feature_call();
						WAIT(0);
					}
				}

				if ( ENTITY::DOES_ENTITY_EXIST ( PLAYER::PLAYER_PED_ID () ) )// && STREAMING::IS_IPL_ACTIVE("plg_01") == 1)
				{
					for each ( const char* scenery in loc->scenery_required )
					{
						if (STREAMING::IS_IPL_ACTIVE(scenery))
						{
							STREAMING::REMOVE_IPL(scenery);
						}
					}
					for each (const char* scenery in loc->scenery_toremove)
					{
						if (!STREAMING::IS_IPL_ACTIVE(scenery))
						{
							STREAMING::REQUEST_IPL(scenery);
						}
					}
				}

				unloadedAnything = true;
				loc->isLoaded = false;
			}
		}
	}

	if (unloadedAnything)
	{
		set_status_text("Old scenery unloaded");

		time = GetTickCount() + 1000;
		while (GetTickCount() < time)
		{
			make_periodic_feature_call();
			WAIT(0);
		}
	}

	return false;
}

bool process_teleport_menu(int categoryIndex)
{
	if (categoryIndex == -1)
	{
		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *markerItem = new MenuItem<int>();
		markerItem->caption = "Go To Marker";
		markerItem->value = -2;
		markerItem->isLeaf = true;
		menuItems.push_back(markerItem);

		markerItem = new MenuItem<int>();
		markerItem->caption = "Chauffeur To Marker";
		markerItem->value = -3;
		markerItem->isLeaf = true;
		menuItems.push_back(markerItem);

		markerItem = new MenuItem<int>();
		markerItem->caption = "Go To Last Vehicle";
		markerItem->value = -4;
		markerItem->isLeaf = true;
		menuItems.push_back(markerItem);

		MenuItem<int> *dialogItem = new MenuItem<int>();
		dialogItem->caption = "Show Coordinates";
		dialogItem->value = -1;
		dialogItem->isLeaf = true;
		menuItems.push_back(dialogItem);

		for (int i = 0; i < MENU_LOCATION_CATEGORIES.size(); i++)
		{
			if (MENU_LOCATION_CATEGORIES[i].compare(JELLMAN_CAPTION) == 0 && !is_jellman_scenery_enabled())
			{
				continue;
			}

			MenuItem<int> *item = new MenuItem<int>();
			item->caption = MENU_LOCATION_CATEGORIES[i];
			item->value = i;
			item->isLeaf = false;
			menuItems.push_back(item);
		}

		bool result = draw_generic_menu<int>(menuItems, &mainMenuIndex, "Teleport Locations", onconfirm_teleport_category, NULL, NULL);
		return result;
	}
	else
	{
		std::vector<MenuItem<int>*> menuItems;

		for (int i = 0; i < VOV_LOCATIONS[categoryIndex].size(); i++)
		{
			MenuItem<int> *item = new MenuItem<int>();
			item->caption = VOV_LOCATIONS[categoryIndex][i].text;
			item->value = i;
			menuItems.push_back(item);
		}

		return draw_generic_menu<int>(menuItems, &lastMenuChoiceInCategories[lastChosenCategory], "Teleport Locations", onconfirm_teleport_location, NULL, NULL);
	}
}

void reset_teleporter_globals()
{
	for (int i = 0; i < MENU_LOCATION_CATEGORIES.size(); i++)
	{
		lastMenuChoiceInCategories[i] = 0;
	}
	lastChosenCategory = 0;
}

const std::vector<std::string> TOGGLE_IPLS
{
	"vb_30_crimetape",
	"sheriff_cap",
	"CS1_16_sheriff_Cap",
	"Hospitaldoorsfixed",
	"SP1_10_fake_interior",
	"SP1_10_real_interior",
	"id2_14_pre_no_int",
	"id2_14_post_no_int",
	"id2_14_during1",
	"id2_14_during2",
	"id2_14_during_door",
	"id2_14_on_fire",
	"burnt_switch_off",
	"des_farmhouse",
	"FINBANK",
	"DT1_03_Shutter",
	"dt1_03_interior_dt1_03_carpark",
	"DT1_03_Gr_Closed",
	"PAPER1_RCM_ALT", //some house's yoga mats
	"PAPER1_RCM", //some house's yoga mats, moved slightly
	"CS3_07_MPGates", //military base gates
	"KorizTempWalls",
	"mic3_chopper_debris", //debris at mall
	"carshowroom_broken",
	"carshowroom_boarded",
	"FBI_colPLUG",
	"FBI_repair",
	"FIB_heist_dmg",
	"FIB_heist_lights",
	"DT1_05_rubble" //rubble outside FBI HQ
};

bool is_ipl_active(std::vector<std::string> extras)
{
	return STREAMING::IS_IPL_ACTIVE(extras.at(0).c_str()) == 1;
}

void set_ipl_active(bool applied, std::vector<std::string> extras)
{
	char* scenery = (char*) extras.at(0).c_str();
	if (applied)
	{
		if (!STREAMING::IS_IPL_ACTIVE(scenery))
		{
			STREAMING::REQUEST_IPL(scenery);
		}
	}
	else
	{
		if (STREAMING::IS_IPL_ACTIVE(scenery))
		{
			STREAMING::REMOVE_IPL(scenery);
		}
	}
}

int toggleIndex = 0;

void process_toggles_menu()
{
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < TOGGLE_IPLS.size(); i++)
	{
		std::string item = TOGGLE_IPLS.at(i);
		FunctionDrivenToggleMenuItem<std::string>* toggleItem = new FunctionDrivenToggleMenuItem<std::string>();
		toggleItem->caption = item;
		toggleItem->getter_call = is_ipl_active;
		toggleItem->setter_call = set_ipl_active;
		toggleItem->value = item;
		toggleItem->extra_arguments.push_back(item);
		menuItems.push_back(toggleItem);
	}

	draw_generic_menu<std::string>(menuItems, &toggleIndex, "Test Toggles", NULL, NULL, NULL);
}

void update_teleport_features()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (beingChauffeured)
	{
		Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		// Moved blipCoords to global scope... we don't want to call for new blip coords each time (we've already told mr. monkey where to go)
		
		if (is_player_at_blip(playerCoords, blipCoords, chauffTolerance))
		{
			cancel_chauffeur("Arrived at destination");
		}
	}
	else
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, 0);
		if (waitingToRetakeSeat != -1 && veh == waitingToRetakeSeat)
		{
			Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1);
			if (driver == NULL || !ENTITY::DOES_ENTITY_EXIST(driver))
			{
				AI::TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT(playerPed, veh);
				waitingToRetakeSeat = -1;
			}
		}
		else
		{
			waitingToRetakeSeat = -1;
		}
	}
}

