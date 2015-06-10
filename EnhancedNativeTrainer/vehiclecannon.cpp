/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "vehiclecannon.h"
#include "menu_functions.h"
#include "config_io.h"
#include "skins.h"
#include "debuglog.h"


bool featureVehicleCannon = false;
bool featureVehicleCannonUpdated = false;

int activeLineIndexPick = 0;
int currType = 1;
std::string currSpawn = "VOLTIC";
std::string currSpawnTitle = "Coil Voltic";
float currSpawnClass = 00.00;
std::string currSpawnSize = "SM";
float spawnOffY = 5.0;
float spawnOffX = 0.0;
float spawnOffZ = 0.0;
float looking;
int waitTime = 25;
float entSpeed = 300.00;

const std::vector<std::string> MENU_SKINS_TYPES_CAPTIONS{"Animals", "NPCs"};

int fodderMainMenuPosition = 0;

int fodderTypesMenuPositionMemory[2] = { 0, 0}; //player, animals, general

const std::vector<std::string> SKINS_ANIMAL_CAPTIONS{ 
	"Boar", "Cat", "Chimp", "Chop", "Cormorant", "Cow", "Coyote", "Crow", "Deer", "Dolphin", "Fish", "German Shepherd", "Grey Whale", "Hammerhead Shark", "Hawk", "Hen", "Humpback", "Husky", "Killer Whale", "Mountain Lion", "Pig", "Pigeon", "Poodle", "Pug", "Rabbit", "Rat", "Retriever", "Rhesus", "Rottweiler", "Seagull", "Stingray", "Tiger Shark", "Westy" };
const std::vector<std::string> SKINS_ANIMAL_VALUES{ 
	"a_c_boar", "a_c_cat_01", "a_c_chimp", "a_c_chop", "a_c_cormorant", 
	"a_c_cow", "a_c_coyote", "a_c_crow", "a_c_deer", "a_c_dolphin", 
	"a_c_fish", "a_c_shepherd", "a_c_whalegrey", "a_c_sharkhammer", "a_c_chickenhawk", 
	"a_c_hen", "a_c_humpback", "a_c_husky", "a_c_killerwhale", "a_c_mtlion", 
	"a_c_pig", "a_c_pigeon", "a_c_poodle", "a_c_pug", "a_c_rabbit_01", 
	"a_c_rat", "a_c_retriever", "a_c_rhesus", "a_c_rottweiler", "a_c_seagull", 
	"a_c_stingray", "a_c_sharktiger", "a_c_westy" };


const std::vector<std::string> SKINS_GENERAL_CAPTIONS{ 
	"Abigail Mathers (CS)", "Abigail Mathers (IG)", "Abner", "African American Male", "Agent (CS)", 
	"Agent (IG)", "Agent 14 (CS)", "Agent 14 (IG)", "Air Hostess", "Air Worker Male", 
	"Al Di Napoli Male", "Alien", "Altruist Cult Mid-Age Male", "Altruist Cult Old Male", "Altruist Cult Old Male 2", 
	"Altruist Cult Young Male", "Altruist Cult Young Male 2", "Amanda De Santa (CS)", "Amanda De Santa (IG)", "Ammu-Nation City Clerk", 
	"Ammu-Nation Rural Clerk", "Andreas Sanchez (CS)", "Andreas Sanchez (IG)", "Anita Mendoza", "Anton Beaudelaire", 
	"Anton Beaudelaire", "Armenian Boss", "Armenian Goon", "Armenian Goon 2", "Armenian Lieutenant", 
	"Armoured Van Security", "Armoured Van Security", "Armoured Van Security 2", "Army Mechanic", "Ashley Butler (CS)", 
	"Ashley Butler (IG)", "Autopsy Tech", "Autoshop Worker", "Autoshop Worker 2", "Azteca", 
	"Baby D", "Ballas East Male", "Ballas Female", "Ballas OG", "Ballas OG (IG)", 
	"Ballas Original Male (IG)", "Ballas South Male", "Bank Manager (CS)", "Bank Manager (IG)", "Bank Manager Male", 
	"Barber Female", "Barman", "Barry (CS)", "Barry (IG)", "Bartender", "Bartender (Rural)", 
	"Baywatch Female", "Baywatch Male", "Beach Female", "Beach Male", "Beach Male 2", 
	"Beach Muscle Male", "Beach Muscle Male 2", "Beach Old Male", "Beach Tramp Female", "Beach Tramp Male", 
	"Beach Young Female", "Beach Young Male", "Beach Young Male 2", "Beach Young Male 3", "Best Man (IG)", 
	"Beverly Felton (CS)", "Beverly Felton (IG)", "Beverly Hills Female", "Beverly Hills Female 2", "Beverly Hills Male", 
	"Beverly Hills Male 2", "Beverly Hills Young Female", "Beverly Hills Young Female 2", "Beverly Hills Young Female 3", "Beverly Hills Young Female 4", 
	"Beverly Hills Young Male", "Beverly Hills Young Male 2", "Bigfoot (CS)", "Bigfoot (IG)", "Bike Hire Guy", 
	"Biker Chic Female", "Black Ops Soldier", "Black Ops Soldier 2", "Black Ops Soldier 3", "Black Street Male", 
	"Black Street Male 2", "Bodybuilder Female", "Bouncer", "Brad (CS)", "Brad (IG)", 
	"Brad's Cadaver (CS)", "Breakdancer Male", "Bride", "Bride (IG)", "Burger Drug Worker", "Burger Drug Worker", 
	"Busboy", "Business Casual", "Business Female 2", "Business Male", "Business Young Female", "Business Young Female 2", 
	"Business Young Female 3", "Business Young Female 4", "Business Young Male", "Business Young Male 2", "Business Young Male 3", 
	"Busker", "Car 3 Guy 1", "Car 3 Guy 1 (IG)", "Car 3 Guy 2", "Car 3 Guy 2 (IG)", 
	"Car Buyer (CS)", "Casey (CS)", "Casey (IG)", "Chef", "Chef", 
	"Chef (CS)", "Chef (IG)", "Chef (IG)", "Chemical Plant Security", "Chemical Plant Worker", 
	"Chinese Boss", "Chinese Goon", "Chinese Goon", "Chinese Goon 2", "Chinese Goon Older", 
	"Chip", "Claude Speed", "Clay Jackson (The Pain Giver) (IG)", "Clay Simons (The Lost) (CS)", "Clay Simons (The Lost) (IG)", 
	"Cletus", "Cletus (IG)", "Clown", "Construction Worker", "Construction Worker 2", 
	"Cop", "Cop Female", "Cop Male", "Corpse Female", "Corpse Young Female", 
	"Corpse Young Female 2", "Crew Member", "Cris Formage (CS)", "Cris Formage (IG)", "Customer", 
	"Cyclist Male", "Cyclist Male", "Dale (CS)", "Dale (IG)", "Dave Norton (CS)", 
	"Dave Norton (IG)", "Dead Hooker", "Dealer", "Debra (CS)", "Denise (CS)", 
	"Denise (IG)", "Denise's Friend", "Devin (CS)", "Devin (IG)", "Devin's Security", 
	"Dima Popov (CS)", "Dima Popov (IG)", "DOA Man", "Dock Worker", "Dock Worker", 
	"Doctor", "Dom Beasley (CS)", "Dom Beasley (IG)", "Doorman", "Downhill Cyclist", 
	"Downtown Female", "Downtown Male", "Dr. Friedlander (CS)", "Dr. Friedlander (IG)", "Dressy Female", 
	"DW Airport Worker", "DW Airport Worker 2", "East SA Female", "East SA Female 2", "East SA Male", 
	"East SA Male 2", "East SA Young Female", "East SA Young Female 2", "East SA Young Female 3", "East SA Young Male", 
	"East SA Young Male 2", "Ed Toh", "Epsilon Female", "Epsilon Male", "Epsilon Male 2", 
	"Epsilon Tom (CS)", "Epsilon Tom (IG)", "Ex-Army Male", "Ex-Mil Bum", "Fabien (CS)", 
	"Fabien (IG)", "Factory Worker Female", "Factory Worker Male", "Families CA Male", "Families DD Male", 
	"Families DNF Male", "Families Female", "Families FOR Male", "Families Gang Member?", "Families Gang Member? (IG)", 
	"Farmer", "Fat Black Female", "Fat Cult Female", "Fat Latino Male", "Fat White Female", 
	"Female Agent", "Ferdinand Kerimov (Mr. K) (CS)", "Ferdinand Kerimov (Mr. K) (IG)", "FIB Architect", "FIB Mugger", 
	"FIB Office Worker", "FIB Office Worker 2", "FIB Security", "FIB Security", "FIB Suit (CS)", 
	"FIB Suit (IG)", "Financial Guru", "Fireman Male", "Fitness Female", "Fitness Female 2", 
	"Floyd Hebert (CS)", "Floyd Hebert (IG)", "FOS Rep?", "Free Mode Female", "Free Mode Male", 
	"Gaffer", "Garbage Worker", "Gardener", "Gay Male", "Gay Male 2", 
	"General Fat Male", "General Fat Male 2", "General Hot Young Female", "General Street Old Female", "General Street Old Male", 
	"General Street Young Male", "General Street Young Male 2", "Gerald", "GLENSTANK? Male", "Golfer Male", 
	"Golfer Young Female", "Golfer Young Male", "Griff", "Grip", "Groom", 
	"Groom (IG)", "Grove Street Dealer", "Guadalope (CS)", "Guido", "Gun Vendor", 
	"GURK? (CS)", "Hairdresser Male", "Hao", "Hao (IG)", "Hasidic Jew Male", 
	"Hasidic Jew Young Male", "Hick", "Hick (IG)", "High Security", "High Security 2", 
	"Highway Cop", "Hiker Female", "Hiker Male", "Hillbilly Male", "Hillbilly Male 2", 
	"Hippie Female", "Hippie Male", "Hippie Male", "Hipster", "Hipster (IG)", 
	"Hipster Female", "Hipster Female 2", "Hipster Female 3", "Hipster Female 4", "Hipster Male", 
	"Hipster Male 2", "Hipster Male 3", "Hooker", "Hooker 2", "Hooker 3", 
	"Hospital Scrubs Female", "Hot Posh Female", "Hugh Welsh", "Hunter (CS)", "Hunter (IG)", 
	"IAA Security", "Impotent Rage", "Imran Shinowa", "Indian Male", "Indian Old Female", 
	"Indian Young Female", "Indian Young Male", "Jane", "Janet (CS)", "Janet (IG)", 
	"Janitor", "Janitor", "Jay Norris (IG)", "Jesco White (Tapdancing Hillbilly)", "Jesus", 
	"Jetskier", "Jewel Heist Driver", "Jewel Heist Gunman", "Jewel Heist Hacker", "Jewel Thief", 
	"Jeweller Assistant", "Jeweller Assistant (CS)", "Jeweller Assistant (IG)", "Jeweller Security", "Jimmy Boston (CS)", 
	"Jimmy Boston (IG)", "Jimmy De Santa (CS)", "Jimmy De Santa (IG)", "Jogger Female", "Jogger Male", 
	"Jogger Male 2", "John Marston", "Johnny Klebitz (CS)", "Johnny Klebitz (IG)", "Josef (CS)", 
	"Josef (IG)", "Josh (CS)", "Josh (IG)", "Juggalo Female", "Juggalo Male", 
	"Justin", "Karen Daniels (CS)", "Karen Daniels (IG)", "Kerry McIntosh (IG)", "Kifflom Guy", 
	"Korean Boss", "Korean Female", "Korean Female 2", "Korean Lieutenant", "Korean Male", 
	"Korean Old Female", "Korean Old Male", "Korean Young Male", "Korean Young Male", "Korean Young Male 2", 
	"Korean Young Male 2", "Lamar Davis (CS)", "Lamar Davis (IG)", "Latino Handyman Male", "Latino Street Male 2", 
	"Latino Street Young Male", "Latino Young Male", "Lazlow (CS)", "Lazlow (IG)", "Lester Crest (CS)", 
	"Lester Crest (IG)", "Life Invader (CS)", "Life Invader (IG)", "Life Invader 2 (IG)", "Life Invader Male", 
	"Line Cook", "Love Fist Willy", "LS Metro Worker Male", "Magenta (CS)", "Magenta (IG)", 
	"Maid", "Malibu Male", "Mani", "Manuel (CS)", "Manuel (IG)", 
	"Mariachi", "Marine", "Marine", "Marine 2", "Marine Young", 
	"Marine Young 2", "Marine Young 3", "Mark Fostenburg", "Marnie Allen (CS)", "Marnie Allen (IG)", 
	"Martin Madrazo (CS)", "Mary-Ann Quinn (CS)", "Mary-Ann Quinn (IG)", "Maude", "Maude (IG)", 
	"Maxim Rashkovsky (CS)", "Maxim Rashkovsky (IG)", "Mechanic", "Mechanic 2", "Merryweather Merc", 
	"Meth Addict", "Mexican", "Mexican (IG)", "Mexican Boss", "Mexican Boss 2", 
	"Mexican Gang Member", "Mexican Goon", "Mexican Goon 2", "Mexican Goon 3", "Mexican Labourer", 
	"Mexican Rural", "Mexican Thug", "Michelle (CS)", "Michelle (IG)", "Migrant Female", 
	"Migrant Male", "Milton McIlroy (CS)", "Milton McIlroy (IG)", "Mime Artist", "Minuteman Joe (CS)", 
	"Minuteman Joe (IG)", "Miranda", "Mistress", "Misty", "Molly (CS)", 
	"Molly (IG)", "Money Man (CS)", "Money Man (IG)", "Motocross Biker", "Motocross Biker 2", 
	"Movie Astronaut", "Movie Director", "Movie Premiere Female", "Movie Premiere Female (CS)", "Movie Premiere Male", 
	"Movie Premiere Male (CS)", "Movie Star Female", "Mrs. Phillips (CS)", "Mrs. Phillips (IG)", "Mrs. Thornhill (CS)", 
	"Mrs. Thornhill (IG)", "Natalia (CS)", "Natalia (IG)", "Nervous Ron (CS)", "Nervous Ron (IG)", 
	"Nigel (CS)", "Nigel (IG)", "Niko Bellic", "OG Boss", "Old Man 1 (CS)", 
	"Old Man 1 (IG)", "Old Man 2 (CS)", "Old Man 2 (IG)", "Omega (CS)", "Omega (IG)", 
	"O'Neil Brothers (IG)", "Ortega", "Ortega (IG)", "Oscar", "Paige Harris (CS)", 
	"Paige Harris (IG)", "Paparazzi Male", "Paparazzi Young Male", "Paramedic", "Party Target", 
	"Partygoer", "Patricia (CS)", "Patricia (IG)", "Pest Control", "Peter Dreyfuss (CS)", 
	"Peter Dreyfuss (IG)", "Pilot", "Pilot", "Pilot 2", "Pogo the Monkey", 
	"Polynesian", "Polynesian Goon", "Polynesian Goon 2", "Polynesian Young", "Poppy Mitchell", 
	"Porn Dude", "Postal Worker Male", "Postal Worker Male 2", "Priest (CS)", "Priest (IG)", 
	"Princess", "Prison Guard", "Prisoner", "Prisoner", "Prisoner (Muscular)", 
	"Prologue Driver", "Prologue Driver", "Prologue Host Female", "Prologue Host Male", "Prologue Host Old Female", 
	"Prologue Mourner Female", "Prologue Mourner Male", "Prologue Security", "Prologue Security", "Prologue Security 2 (CS)", 
	"Prologue Security 2 (IG)", "PROS?", "Ranger Female", "Ranger Male", "Reporter", 
	"Republican Space Ranger", "Rival Paparazzo", "Road Cyclist", "Robber", "Rocco Pelosi", 
	"Rocco Pelosi (IG)", "Rural Meth Addict Female", "Rural Meth Addict Male", "Russian Drunk (CS)", "Russian Drunk (IG)", 
	"Sales Assistant (High-End)", "Sales Assistant (Low-End)", "Sales Assistant (Mask Stall)", "Sales Assistant (Mid-Price)", "Salton Female", 
	"Salton Male", "Salton Male 2", "Salton Male 3", "Salton Male 4", "Salton Old Female", 
	"Salton Old Male", "Salton Young Male", "Salvadoran Boss", "Salvadoran Goon", "Salvadoran Goon 2", 
	"Salvadoran Goon 3", "Scientist", "Screenwriter", "Screenwriter (IG)", "Security Guard", 
	"Sheriff Female", "Sheriff Male", "Shopkeeper", "Simeon Yetarian (CS)", "Simeon Yetarian (IG)", 
	"Skater Female", "Skater Male", "Skater Young Male", "Skater Young Male 2", "Skid Row Female", 
	"Skid Row Male", "Snow Cop Male", "Solomon Richards (CS)", "Solomon Richards (IG)", "South Central Female", 
	"South Central Female 2", "South Central Latino Male", "South Central Male", "South Central Male 2", "South Central Male 3", 
	"South Central Male 4", "South Central MC Female", "South Central Old Female", "South Central Old Female 2", "South Central Old Male", 
	"South Central Old Male 2", "South Central Old Male 3", "South Central Young Female", "South Central Young Female 2", "South Central Young Female 3", 
	"South Central Young Male", "South Central Young Male 2", "South Central Young Male 3", "South Central Young Male 4", "Sports Biker", 
	"Spy Actor", "Spy Actress", "Stag Party Groom", "Steve Haines (CS)", "Steve Haines (IG)", 
	"Street Performer", "Street Preacher", "Street Punk", "Street Punk 2", "Street Vendor", 
	"Street Vendor Young", "Stretch (CS)", "Stretch (IG)", "Stripper", "Stripper", 
	"Stripper 2", "Stripper 2", "Stripper Lite", "Stripper Lite", "Sunbather Male", 
	"Surfer", "SWAT", "Sweatshop Worker", "Sweatshop Worker Young", "Talina (IG)", 
	"Tanisha (CS)", "Tanisha (IG)", "Tao Cheng (CS)", "Tao Cheng (IG)", "Tao's Translator (CS)", 
	"Tao's Translator (IG)", "Tattoo Artist", "Tennis Coach (CS)", "Tennis Coach (IG)", "Tennis Player Female", 
	"Tennis Player Male", "Terry (CS)", "Terry (IG)", "The Lost MC Female", "The Lost MC Male", 
	"The Lost MC Male 2", "The Lost MC Male 3", "Tom (CS)", "Tonya", "Tonya (IG)", 
	"Topless", "Tourist Female", "Tourist Male", "Tourist Young Female", "Tourist Young Female 2", 
	"Tracey De Santa (CS)", "Tracey De Santa (IG)", "Traffic Warden", "Traffic Warden (IG)", "Tramp Female", 
	"Tramp Male", "Tramp Old Male", "Tramp Old Male", "Transport Worker Male", "Transvestite Male", 
	"Transvestite Male 2", "Trucker Male", "Tyler Dixon (IG)", "Undercover Cop", "United Paper Man (CS)", 
	"United Paper Man (IG)", "UPS Driver", "UPS Driver 2", "US Coastguard", "Vagos Female", 
	"Valet", "Vespucci Beach Male", "Vespucci Beach Male 2", "Vinewood Douche", "Vinewood Female", 
	"Vinewood Female 2", "Vinewood Female 3", "Vinewood Female 4", "Vinewood Male", "Vinewood Male 2", 
	"Vinewood Male 3", "Vinewood Male 4", "Wade (CS)", "Wade (IG)", "Waiter", 
	"Wei Cheng (CS)", "Wei Cheng (IG)", "White Street Male", "White Street Male 2", "Window Cleaner", 
	"Yoga Female", "Yoga Male", "Zimbor (CS)", "Zimbor (IG)", "Zombie" };
const std::vector<std::string> SKINS_GENERAL_VALUES{ 
	"csb_abigail", "ig_abigail", "u_m_y_abner", "a_m_m_afriamer_01", "csb_agent", 
	"ig_agent", "csb_mp_agent14", "ig_mp_agent14", "s_f_y_airhostess_01", "s_m_y_airworker", 
	"u_m_m_aldinapoli", "s_m_m_movalien_01", "a_m_m_acult_01", "a_m_o_acult_01", "a_m_o_acult_02", 
	"a_m_y_acult_01", "a_m_y_acult_02", "cs_amandatownley", "ig_amandatownley", "s_m_y_ammucity_01", 
	"s_m_m_ammucountry", "cs_andreas", "ig_andreas", "csb_anita", "csb_anton", 
	"u_m_y_antonb", "g_m_m_armboss_01", "g_m_m_armgoon_01", "g_m_y_armgoon_02", "g_m_m_armlieut_01", 
	"mp_s_m_armoured_01", "s_m_m_armoured_01", "s_m_m_armoured_02", "s_m_y_armymech_01", "cs_ashley", 
	"ig_ashley", "s_m_y_autopsy_01", "s_m_m_autoshop_01", "s_m_m_autoshop_02", "g_m_y_azteca_01", 
	"u_m_y_babyd", "g_m_y_ballaeast_01", "g_f_y_ballas_01", "csb_ballasog", "ig_ballasog", 
	"g_m_y_ballaorig_01", "g_m_y_ballasout_01", "cs_bankman", "ig_bankman", "u_m_m_bankman", 
	"s_f_m_fembarber", "s_m_y_barman_01", "cs_barry", "ig_barry", "s_f_y_bartender_01", 
	"s_m_m_cntrybar_01", "s_f_y_baywatch_01", "s_m_y_baywatch_01", "a_f_m_beach_01", "a_m_m_beach_01", 
	"a_m_m_beach_02", "a_m_y_musclbeac_01", "a_m_y_musclbeac_02", "a_m_o_beach_01", "a_f_m_trampbeac_01", 
	"a_m_m_trampbeac_01", "a_f_y_beach_01", "a_m_y_beach_01", "a_m_y_beach_02", "a_m_y_beach_03", 
	"ig_bestmen", "cs_beverly", "ig_beverly", "a_f_m_bevhills_01", "a_f_m_bevhills_02", 
	"a_m_m_bevhills_01", "a_m_m_bevhills_02", "a_f_y_bevhills_01", "a_f_y_bevhills_02", "a_f_y_bevhills_03", 
	"a_f_y_bevhills_04", "a_m_y_bevhills_01", "a_m_y_bevhills_02", "cs_orleans", "ig_orleans", 
	"u_m_m_bikehire_01", "u_f_y_bikerchic", "s_m_y_blackops_01", "s_m_y_blackops_02", "s_m_y_blackops_03", 
	"a_m_y_stbla_01", "a_m_y_stbla_02", "a_f_m_bodybuild_01", "s_m_m_bouncer_01", "cs_brad", 
	"ig_brad", "cs_bradcadaver", "a_m_y_breakdance_01", "csb_bride", "ig_bride", 
	"csb_burgerdrug", "u_m_y_burgerdrug_01", "s_m_y_busboy_01", "a_m_y_busicas_01", "a_f_m_business_02", 
	"a_m_m_business_01", "a_f_y_business_01", "a_f_y_business_02", "a_f_y_business_03", "a_f_y_business_04", 
	"a_m_y_business_01", "a_m_y_business_02", "a_m_y_business_03", "s_m_o_busker_01", "csb_car3guy1", 
	"ig_car3guy1", "csb_car3guy2", "ig_car3guy2", "cs_carbuyer", "cs_casey", 
	"ig_casey", "s_m_y_chef_01", "csb_chef", "csb_chef2", "ig_chef", 
	"ig_chef2", "s_m_m_chemsec_01", "g_m_m_chemwork_01", "g_m_m_chiboss_01", "g_m_m_chigoon_01", 
	"csb_chin_goon", "g_m_m_chigoon_02", "g_m_m_chicold_01", "u_m_y_chip", "mp_m_claude_01", 
	"ig_claypain", "cs_clay", "ig_clay", "csb_cletus", "ig_cletus", 
	"s_m_y_clown_01", "s_m_y_construct_01", "s_m_y_construct_02", "csb_cop", "s_f_y_cop_01", 
	"s_m_y_cop_01", "u_f_m_corpse_01", "u_f_y_corpse_01", "u_f_y_corpse_02", "s_m_m_ccrew_01", 
	"cs_chrisformage", "ig_chrisformage", "csb_customer", "a_m_y_cyclist_01", "u_m_y_cyclist_01", 
	"cs_dale", "ig_dale", "cs_davenorton", "ig_davenorton", "mp_f_deadhooker", 
	"s_m_y_dealer_01", "cs_debra", "cs_denise", "ig_denise", "csb_denise_friend", 
	"cs_devin", "ig_devin", "s_m_y_devinsec_01", "csb_popov", "ig_popov", 
	"u_m_m_doa_01", "s_m_m_dockwork_01", "s_m_y_dockwork_01", "s_m_m_doctor_01", "cs_dom", 
	"ig_dom", "s_m_y_doorman_01", "a_m_y_dhill_01", "a_f_m_downtown_01", "a_m_y_downtown_01", 
	"cs_drfriedlander", "ig_drfriedlander", "a_f_y_scdressy_01", "s_m_y_dwservice_01", "s_m_y_dwservice_02", 
	"a_f_m_eastsa_01", "a_f_m_eastsa_02", "a_m_m_eastsa_01", "a_m_m_eastsa_02", "a_f_y_eastsa_01", 
	"a_f_y_eastsa_02", "a_f_y_eastsa_03", "a_m_y_eastsa_01", "a_m_y_eastsa_02", "u_m_m_edtoh", 
	"a_f_y_epsilon_01", "a_m_y_epsilon_01", "a_m_y_epsilon_02", "cs_tomepsilon", "ig_tomepsilon", 
	"mp_m_exarmy_01", "u_m_y_militarybum", "cs_fabien", "ig_fabien", "s_f_y_factory_01", 
	"s_m_y_factory_01", "g_m_y_famca_01", "mp_m_famdd_01", "g_m_y_famdnf_01", "g_f_y_families_01", 
	"g_m_y_famfor_01", "csb_ramp_gang", "ig_ramp_gang", "a_m_m_farmer_01", "a_f_m_fatbla_01", 
	"a_f_m_fatcult_01", "a_m_m_fatlatin_01", "a_f_m_fatwhite_01", "a_f_y_femaleagent", "cs_mrk", 
	"ig_mrk", "u_m_m_fibarchitect", "u_m_y_fibmugger_01", "s_m_m_fiboffice_01", "s_m_m_fiboffice_02", 
	"mp_m_fibsec_01", "s_m_m_fibsec_01", "cs_fbisuit_01", "ig_fbisuit_01", "u_m_o_finguru_01", 
	"s_m_y_fireman_01", "a_f_y_fitness_01", "a_f_y_fitness_02", "cs_floyd", 
	"ig_floyd", "csb_fos_rep", "mp_f_freemode_01", "mp_m_freemode_01", "s_m_m_gaffer_01", 
	"s_m_y_garbage", "s_m_m_gardener_01", "a_m_y_gay_01", "a_m_y_gay_02", "a_m_m_genfat_01", 
	"a_m_m_genfat_02", "a_f_y_genhot_01", "a_f_o_genstreet_01", "a_m_o_genstreet_01", "a_m_y_genstreet_01", 
	"a_m_y_genstreet_02", "csb_g", "u_m_m_glenstank_01", "a_m_m_golfer_01", "a_f_y_golfer_01", 
	"a_m_y_golfer_01", "u_m_m_griff_01", "s_m_y_grip_01", "csb_groom", "ig_groom", 
	"csb_grove_str_dlr", "cs_guadalope", "u_m_y_guido_01", "u_m_y_gunvend_01", "cs_gurk", 
	"s_m_m_hairdress_01", "csb_hao", "ig_hao", "a_m_m_hasjew_01", "a_m_y_hasjew_01", "csb_ramp_hic", 
	"ig_ramp_hic", "s_m_m_highsec_01", "s_m_m_highsec_02", "s_m_y_hwaycop_01", "a_f_y_hiker_01", 
	"a_m_y_hiker_01", "a_m_m_hillbilly_01", "a_m_m_hillbilly_02", "a_f_y_hippie_01", "u_m_y_hippie_01", 
	"a_m_y_hippy_01", "csb_ramp_hipster", "ig_ramp_hipster", "a_f_y_hipster_01", "a_f_y_hipster_02", 
	"a_f_y_hipster_03", "a_f_y_hipster_04", "a_m_y_hipster_01", "a_m_y_hipster_02", "a_m_y_hipster_03", 
	"s_f_y_hooker_01", "s_f_y_hooker_02", "s_f_y_hooker_03", "s_f_y_scrubs_01", "u_f_y_hotposh_01", 
	"csb_hugh", "cs_hunter", "ig_hunter", "s_m_m_ciasec_01", "u_m_y_imporage", 
	"csb_imran", "a_m_m_indian_01", "a_f_o_indian_01", "a_f_y_indian_01", "a_m_y_indian_01", 
	"u_f_y_comjane", "cs_janet", "ig_janet", "csb_janitor", "s_m_m_janitor", 
	"ig_jay_norris", "u_m_o_taphillbilly", "u_m_m_jesus_01", "a_m_y_jetski_01", "hc_driver", 
	"hc_gunman", "hc_hacker", "u_m_m_jewelthief", "u_f_y_jewelass_01", "cs_jewelass", 
	"ig_jewelass", "u_m_m_jewelsec_01", "cs_jimmyboston", "ig_jimmyboston", "cs_jimmydisanto", 
	"ig_jimmydisanto", "a_f_y_runner_01", "a_m_y_runner_01", "a_m_y_runner_02", "mp_m_marston_01", 
	"cs_johnnyklebitz", "ig_johnnyklebitz", "cs_josef", "ig_josef", "cs_josh", 
	"ig_josh", "a_f_y_juggalo_01", "a_m_y_juggalo_01", "u_m_y_justin", "cs_karen_daniels", 
	"ig_karen_daniels", "ig_kerrymcintosh", "u_m_y_baygor", "g_m_m_korboss_01", "a_f_m_ktown_01", 
	"a_f_m_ktown_02", "g_m_y_korlieut_01", "a_m_m_ktown_01", "a_f_o_ktown_01", "a_m_o_ktown_01", 
	"g_m_y_korean_01", "a_m_y_ktown_01", "g_m_y_korean_02", "a_m_y_ktown_02", "cs_lamardavis", 
	"ig_lamardavis", "s_m_m_lathandy_01", "a_m_m_stlat_02", "a_m_y_stlat_01", "a_m_y_latino_01", 
	"cs_lazlow", "ig_lazlow", "cs_lestercrest", "ig_lestercrest", "cs_lifeinvad_01", 
	"ig_lifeinvad_01", "ig_lifeinvad_02", "s_m_m_lifeinvad_01", "s_m_m_linecook", "u_m_m_willyfist", 
	"s_m_m_lsmetro_01", "cs_magenta", "ig_magenta", "s_f_m_maid_01", "a_m_m_malibu_01", 
	"u_m_y_mani", "cs_manuel", "ig_manuel", "s_m_m_mariachi_01", "csb_ramp_marine", 
	"s_m_m_marine_01", "s_m_m_marine_02", "s_m_y_marine_01", "s_m_y_marine_02", "s_m_y_marine_03", 
	"u_m_m_markfost", "cs_marnie", "ig_marnie", "cs_martinmadrazo", "cs_maryann", 
	"ig_maryann", "csb_maude", "ig_maude", "csb_rashcosvki", "ig_rashcosvki", 
	"s_m_y_xmech_01", "s_m_y_xmech_02", "csb_mweather", "a_m_y_methhead_01", "csb_ramp_mex", 
	"ig_ramp_mex", "g_m_m_mexboss_01", "g_m_m_mexboss_02", "g_m_y_mexgang_01", "g_m_y_mexgoon_01", 
	"g_m_y_mexgoon_02", "g_m_y_mexgoon_03", "a_m_m_mexlabor_01", "a_m_m_mexcntry_01", "a_m_y_mexthug_01", 
	"cs_michelle", "ig_michelle", "s_f_y_migrant_01", "s_m_m_migrant_01", "cs_milton", 
	"ig_milton", "s_m_y_mime", "cs_joeminuteman", "ig_joeminuteman", "u_f_m_miranda", 
	"u_f_y_mistress", "mp_f_misty_01", "cs_molly", "ig_molly", "csb_money", 
	"ig_money", "a_m_y_motox_01", "a_m_y_motox_02", "s_m_m_movspace_01", "u_m_m_filmdirector", 
	"s_f_y_movprem_01", "cs_movpremf_01", "s_m_m_movprem_01", "cs_movpremmale", "u_f_o_moviestar", 
	"cs_mrsphillips", "ig_mrsphillips", "cs_mrs_thornhill", "ig_mrs_thornhill", "cs_natalia", 
	"ig_natalia", "cs_nervousron", "ig_nervousron", "cs_nigel", "ig_nigel", 
	"mp_m_niko_01", "a_m_m_og_boss_01", "cs_old_man1a", "ig_old_man1a", "cs_old_man2", 
	"ig_old_man2", "cs_omega", "ig_omega", "ig_oneil", "csb_ortega", 
	"ig_ortega", "csb_oscar", "csb_paige", "ig_paige", "a_m_m_paparazzi_01", 
	"u_m_y_paparazzi", "s_m_m_paramedic_01", "u_m_m_partytarget", "u_m_y_party_01", "cs_patricia", 
	"ig_patricia", "s_m_y_pestcont_01", "cs_dreyfuss", "ig_dreyfuss", "s_m_m_pilot_01", 
	"s_m_y_pilot_01", "s_m_m_pilot_02", "u_m_y_pogo_01", "a_m_m_polynesian_01", "g_m_y_pologoon_01", 
	"g_m_y_pologoon_02", "a_m_y_polynesian_01", "u_f_y_poppymich", "csb_porndudes", 
	"s_m_m_postal_01", "s_m_m_postal_02", "cs_priest", "ig_priest", "u_f_y_princess", 
	"s_m_m_prisguard_01", "s_m_y_prisoner_01", "u_m_y_prisoner_01", "s_m_y_prismuscl_01", "u_m_y_proldriver_01", 
	"csb_prologuedriver", "a_f_m_prolhost_01", "a_m_m_prolhost_01", "u_f_o_prolhost_01", "u_f_m_promourn_01", 
	"u_m_m_promourn_01", "csb_prolsec", "u_m_m_prolsec_01", "cs_prolsec_02", "ig_prolsec_02", 
	"mp_g_m_pros_01", "s_f_y_ranger_01", "s_m_y_ranger_01", "csb_reporter", "u_m_y_rsranger_01", 
	"u_m_m_rivalpap", "a_m_y_roadcyc_01", "s_m_y_robber_01", "csb_roccopelosi", "ig_roccopelosi", 
	"a_f_y_rurmeth_01", "a_m_m_rurmeth_01", "cs_russiandrunk", "ig_russiandrunk", "s_f_m_shop_high", 
	"s_f_y_shop_low", "s_m_y_shop_mask", "s_f_y_shop_mid", "a_f_m_salton_01", "a_m_m_salton_01", 
	"a_m_m_salton_02", "a_m_m_salton_03", "a_m_m_salton_04", "a_f_o_salton_01", "a_m_o_salton_01", 
	"a_m_y_salton_01", "g_m_y_salvaboss_01", "g_m_y_salvagoon_01", "g_m_y_salvagoon_02", "g_m_y_salvagoon_03", 
	"s_m_m_scientist_01", "csb_screen_writer", "ig_screen_writer", "s_m_m_security_01", "s_f_y_sheriff_01", 
	"s_m_y_sheriff_01", "mp_m_shopkeep_01", "cs_siemonyetarian", "ig_siemonyetarian", "a_f_y_skater_01", 
	"a_m_m_skater_01", "a_m_y_skater_01", "a_m_y_skater_02", "a_f_m_skidrow_01", "a_m_m_skidrow_01", 
	"s_m_m_snowcop_01", "cs_solomon", "ig_solomon", "a_f_m_soucent_01", "a_f_m_soucent_02",
	"a_m_m_socenlat_01", "a_m_m_soucent_01", "a_m_m_soucent_02", "a_m_m_soucent_03", "a_m_m_soucent_04", 
	"a_f_m_soucentmc_01", "a_f_o_soucent_01", "a_f_o_soucent_02", "a_m_o_soucent_01", "a_m_o_soucent_02", 
	"a_m_o_soucent_03", "a_f_y_soucent_01", "a_f_y_soucent_02", "a_f_y_soucent_03", "a_m_y_soucent_01", 
	"a_m_y_soucent_02", "a_m_y_soucent_03", "a_m_y_soucent_04", "u_m_y_sbike", "u_m_m_spyactor", 
	"u_f_y_spyactress", "u_m_y_staggrm_01", "cs_stevehains", "ig_stevehains", "s_m_m_strperf_01", 
	"s_m_m_strpreach_01", "g_m_y_strpunk_01", "g_m_y_strpunk_02", "s_m_m_strvend_01", "s_m_y_strvend_01", 
	"cs_stretch", "ig_stretch", "csb_stripper_01", "s_f_y_stripper_01", "csb_stripper_02", 
	"s_f_y_stripper_02", "s_f_y_stripperlite", "mp_f_stripperlite", "a_m_y_sunbathe_01", "a_m_y_surfer_01", 
	"s_m_y_swat_01", "s_f_m_sweatshop_01", "s_f_y_sweatshop_01", "ig_talina", "cs_tanisha", 
	"ig_tanisha", "cs_taocheng", "ig_taocheng", "cs_taostranslator", "ig_taostranslator", 
	"u_m_y_tattoo_01", "cs_tenniscoach", "ig_tenniscoach", "a_f_y_tennis_01", "a_m_m_tennis_01", 
	"cs_terry", "ig_terry", "g_f_y_lost_01", "g_m_y_lost_01", "g_m_y_lost_02", 
	"g_m_y_lost_03", "cs_tom", "csb_tonya", "ig_tonya", "a_f_y_topless_01", 
	"a_f_m_tourist_01", "a_m_m_tourist_01", "a_f_y_tourist_01", "a_f_y_tourist_02", "cs_tracydisanto", 
	"ig_tracydisanto", "csb_trafficwarden", "ig_trafficwarden", "a_f_m_tramp_01", "a_m_m_tramp_01", 
	"u_m_o_tramp_01", "a_m_o_tramp_01", "s_m_m_gentransport", "a_m_m_tranvest_01", "a_m_m_tranvest_02", 
	"s_m_m_trucker_01", "ig_tylerdix", "csb_undercover", "cs_paper", "ig_paper", 
	"s_m_m_ups_01", "s_m_m_ups_02", "s_m_y_uscg_01", "g_f_y_vagos_01", "s_m_y_valet_01", 
	"a_m_y_beachvesp_01", "a_m_y_beachvesp_02", "a_m_y_vindouche_01", "a_f_y_vinewood_01", "a_f_y_vinewood_02", 
	"a_f_y_vinewood_03", "a_f_y_vinewood_04", "a_m_y_vinewood_01", "a_m_y_vinewood_02", "a_m_y_vinewood_03", 
	"a_m_y_vinewood_04", "cs_wade", "ig_wade", "s_m_y_waiter_01", "cs_chengsr", 
	"ig_chengsr", "a_m_y_stwhi_01", "a_m_y_stwhi_02", "s_m_y_winclean_01", "a_f_y_yoga_01", 
	"a_m_y_yoga_01", "cs_zimbor", "ig_zimbor", "u_m_y_zombie_01" };



const std::vector<std::string> MENU_VEHICLE_CATEGORIES{ "Cars", "Industrial", "Emergency and Military", "Motorcycles", "Planes", "Helicopters", "Boats", "Bicycles" };

//Cars

const std::vector<std::string> MENU_CAR_CATEGORIES{ 
	"Supercars", "Sports", "Sport Classics", 
	"Coupes", "Muscle", "Offroad", 
	"SUVs", "Sedans", "Compacts" };
//10.00 - 18.00
const std::vector<std::string> CAPTIONS_SUPERCARS{ 
	"Coil Voltic", "Grotti Cheetah", "Grotti Turismo R", 
	"Overflod Entity XF", "Pegassi Infernus", "Pegassi Vacca", 
	"Pegassi Zentorno", "Truffade Adder", "Vapid Bullet" };
//10.00 - 10.08
const std::vector<std::string> CAPTIONS_SPORTS{ 
	"Albany Alpha", "Annis Elegy RH8", "Benefactor Feltzer", 
	"Benefactor Schwartzer", "Benefactor Surano", "Bravado Banshee", 
	"Bravado Buffalo", "Bravado Buffalo S", "Bravado Buffalo S (Race)", 
	"Dewbauchee Massacro", "Dewbauchee Massacro (Race)", "Dewbauchee Rapid GT", 
	"Dewbauchee Rapid GT Cabrio", "Dinka Blista Compact", "Dinka Blista Compact (Go Go Monkey Race)", 
	"Dinka Jester", "Dinka Jester (Race)", "Grotti Carbonizzare", 
	"Hijak Khamelion", "Invetero Coquette", "Karin Futo", 
	"Karin Kuruma", "Karin Kuruma (Armoured)", "Karin Sultan", 
	"Lampadati Furore GT", "Maibatsu Penumbra", "Obey 9F", 
	"Obey 9F Cabrio", "Phister Comet", "Schyster Fusilade" };
//11.00 - 11.29
const std::vector<std::string> CAPTIONS_SPORTCLASSICS{ 
	"Albany Manana", "Albany Roosevelt", "Declasse Tornado", 
	"Declasse Tornado (Rusty)", "Declasse Tornado Cabrio", "Declasse Tornado Cabrio (Rusty)", 
	"Dewbauchee JB 700", "Grotti Stinger", "Grotti Stinger GT", 
	"Invetero Coquette Classic", "Lampadati Casco", "Lampadati Pigalle", 
	"Pegassi Monroe", "Truffade Z-Type", "Vapid Peyote" };
//12.00
const std::vector<std::string> CAPTIONS_COUPES{ 
	"Dewbauchee Exemplar", "Enus Cognoscenti Cabrio", "Lampadati Felon", 
	"Lampadati Felon GT", "Ocelot F620", "Ocelot Jackal", 
	"Ubermacht Sentinel", "Ubermacht Sentinel XS", "Ubermacht Zion", 
	"Ubermacht Zion Cabrio" };
//13.00
const std::vector<std::string> CAPTIONS_MUSCLE{ 
	"Albany Buccaneer", "Bravado Gauntlet", "Bravado Gauntlet (Race)", 
	"Cheval Picador", "Declasse Sabre Turbo", "Declasse Stallion", 
	"Declasse Stallion (Race)", "Declasse Vigero", "Declasse Voodoo", 
	"Imponte Duke O' Death", "Imponte Dukes", "Imponte Phoenix", 
	"Imponte Ruiner", "Vapid Blade", "Vapid Dominator", 
	"Vapid Dominator (Race)", "Vapid Hotknife", "Vapid Slamvan", 
	"Vapid Slamvan (Lost MC)" };
//14.00
const std::vector<std::string> CAPTIONS_OFFROAD{ 
	"Benefactor Dubsta 6x6", "BF Bifta", "BF Injection", 
	"Bravado Dune", "Bravado Duneloader", "Bravado Space Docker", 
	"Canis Bodhi", "Canis Kalahari", "Canis Mesa (Off-Road)", 
	"Cheval Marshall", "Declasse Rancher XL", "Declasse Rancher XL (Snow)", 
	"Insurgent", "Insurgent (Gun Mount)", "Karin Rebel", 
	"Karin Rebel (Rusty)", "Karin Technical", "Nagasaki Blazer", 
	"Nagasaki Blazer (Hot Rod)", "Nagasaki Blazer (Lifeguard)", "Vapid Guardian", 
	"Vapid Sandking", "Vapid Sandking XL", "Vapid The Liberator" };
//15.00
const std::vector<std::string> CAPTIONS_SUVS{ 
	"Albany Cavalcade", "Albany Cavalcade Mk2", "Benefactor Dubsta", 
	"Benefactor Dubsta (Flat Black)", "Benefactor Serrano", "Bravado Gresley", 
	"Canis Mesa", "Canis Mesa (Snow)", "Canis Seminole", "Declasse Granger", 
	"Dundreary Landstalker", "Emperor Habanero", "Enus Huntley S", 
	"Fathom FQ 2", "Gallivanter Baller (Large)", "Gallivanter Baller (Small)", 
	"Karin BeeJay XL", "Mammoth Patriot", "Obey Rocoto", 
	"Vapid Radius" };
//16.00
const std::vector<std::string> CAPTIONS_SEDANS{ 
	"Albany Emperor", "Albany Emperor (Rusty)", "Albany Emperor (Snow)", 
	"Albany Primo", "Albany Washington", "Benefactor Glendale", 
	"Benefactor Schafter", "Chariot Romero Hearse", "Cheval Fugitive", 
	"Cheval Surge", "Declasse Asea", "Declasse Asea (Snow)", 
	"Declasse Premier", "Dundreary Regina", "Dundreary Stretch", 
	"Enus Super Diamond", "Karin Asterope", "Karin Intruder", 
	"Obey Tailgater", "Ubermacht Oracle", "Ubermacht Oracle Mk2", 
	"Vapid Stanier", "Vapid Stanier (Taxi)", "Vulcan Ingot", 
	"Vulcar Warrener", "Zirconium Stratum" };
//17.00
const std::vector<std::string> CAPTIONS_COMPACTS{ 
	"Benefactor Panto", "Bollokan Prairie", "Declasse Rhapsody", 
	"Dinka Blista", "Karin Dilettante", "Karin Dilettante (FlyUS)", 
	"Weeny Issi" };
//18.00
const std::vector<std::string> VALUES_SUPERCARS{ 
	"VOLTIC", "CHEETAH", "TURISMOR", "ENTITYXF", "INFERNUS", "VACCA", "ZENTORNO", "ADDER", "BULLET" };

const std::vector<std::string> VALUES_SPORTS{ 
	"ALPHA", "ELEGY2", "FELTZER2", "SCHWARZER", "SURANO", "BANSHEE", "BUFFALO", "BUFFALO2", "BUFFALO3", "MASSACRO", "MASSACRO2", "RAPIDGT", "RAPIDGT2", "BLISTA2", "BLISTA3", "JESTER", "JESTER2", "CARBONIZZARE", "KHAMELION", "COQUETTE", "FUTO", "KURUMA", "KURUMA2", "SULTAN", "FUROREGT", "PENUMBRA", "NINEF", "NINEF2", "COMET2", "FUSILADE" };

const std::vector<std::string> VALUES_SPORTCLASSICS{ 
	"MANANA", "BTYPE", "TORNADO", "TORNADO3", "TORNADO2", "TORNADO4", "JB700", "STINGER", "STINGERGT", "COQUETTE2", "CASCO", "PIGALLE", "MONROE", "ZTYPE", "PEYOTE" };

const std::vector<std::string> VALUES_COUPES{ 
	"EXEMPLAR", "COGCABRIO", "FELON", "FELON2", "F620", "JACKAL", "SENTINEL", "SENTINEL2", "ZION", "ZION2" };

const std::vector<std::string> VALUES_MUSCLE{ 
	"BUCCANEER", "GAUNTLET", "GAUNTLET2", "PICADOR", "SABREGT", "STALION", "STALION2", "VIGERO", "VOODOO2", "DUKES2", "DUKES", "PHOENIX", "RUINER", "BLADE", "DOMINATOR", "DOMINATOR2", "HOTKNIFE", "SLAMVAN", "SLAMVAN2" };

const std::vector<std::string> VALUES_OFFROAD{ 
	"DUBSTA3", "BIFTA", "BFINJECTION", "DUNE", "DLOADER", "DUNE2", "BODHI2", "KALAHARI", "MESA3", "MARSHALL", "RANCHERXL", "RANCHERXL2", "INSURGENT2", "INSURGENT", "REBEL2", "REBEL", "TECHNICAL", "BLAZER", "BLAZER3", "BLAZER2", "GUARDIAN", "SANDKING2", "SANDKING", "MONSTER" };

const std::vector<std::string> VALUES_SUVS{ 
	"CAVALCADE", "CAVALCADE2", "DUBSTA", "DUBSTA2", "SERRANO", "GRESLEY", "MESA", "MESA2", "SEMINOLE", "GRANGER", "LANDSTALKER", "HABANERO", "HUNTLEY", "FQ2", "BALLER", "BALLER2", "BJXL", "PATRIOT", "ROCOTO", "RADI" };

const std::vector<std::string> VALUES_SEDANS{ 
	"EMPEROR", "EMPEROR2", "EMPEROR3", "PRIMO", "WASHINGTON", "GLENDALE", "SCHAFTER2", "ROMERO", "FUGITIVE", "SURGE", "ASEA", "ASEA2", "PREMIER", "REGINA", "STRETCH", "SUPERD", "ASTEROPE", "INTRUDER", "TAILGATER", "ORACLE", "ORACLE2", "STANIER", "TAXI", "INGOT", "WARRENER", "STRATUM" };

const std::vector<std::string> VALUES_COMPACTS{ 
	"PANTO", "PRAIRIE", "RHAPSODY", "BLISTA", "DILETTANTE", "DILETTANTE2", "ISSI2" };

const std::vector<std::string> VOV_CAR_CAPTIONS[] = { 
	CAPTIONS_SUPERCARS, CAPTIONS_SPORTS, CAPTIONS_SPORTCLASSICS, CAPTIONS_COUPES, CAPTIONS_MUSCLE, CAPTIONS_OFFROAD, CAPTIONS_SUVS, CAPTIONS_SEDANS, CAPTIONS_COMPACTS };

const std::vector<std::string> VOV_CAR_VALUES[] = { VALUES_SUPERCARS, VALUES_SPORTS, VALUES_SPORTCLASSICS, VALUES_COUPES, VALUES_MUSCLE, VALUES_OFFROAD, VALUES_SUVS, VALUES_SEDANS, VALUES_COMPACTS };

//Industrial

const std::vector<std::string> MENU_INDUS_CATEGORIES{ "Pickups", "Vans", "Trucks", "Service Utility", "Trailers", "Trains Undriveable" };
//20.00 - 25.00
const std::vector<std::string> CAPTIONS_PICKUPS{ 
	"Bravado Bison", "Bravado Bison (Backrack)", "Bravado Bison (Construction)", 
	"Bravado Ratloader", "Bravado Ratloader (Rusty)", "Vapid Bobcat", "Vapid Sadler", 
	"Vapid Sadler (Snow)" };
//20.00 - 20.07
const std::vector<std::string> CAPTIONS_VANS{ 
	"BF Surfer", "BF Surfer (Rusty)", "Bravado Paradise", 
	"Bravado Rumpo (Deludamol)", "Bravado Rumpo (Weazel News)", "Bravado Youga", 
	"Brute Camper", "Brute Pony (Business)", 
	"Brute Pony (Cannibus Shop)", "Brute Taco Van", "Declasse Burrito", 
	"Declasse Burrito (Bug Stars)", "Declasse Burrito (Construction)", "Declasse Burrito (Gang)", 
	"Declasse Burrito (Graphics)", "Declasse Burrito (Snow)", "Declasse Burrito (The Lost)", 
	"Vapid Minivan", "Vapid Speedo", "Vapid Speedo (Clown)", 
	"Zirconium Journey" };
//21.00
const std::vector<std::string> CAPTIONS_TRUCKS{ 
	"Brute Boxville (Go Postal)", "Brute Boxville (Humane Labs)", "Brute Boxville (Post OP)", 
	"Brute Boxville (Water & Power)", "Brute Stockade", "Brute Stockade (Snow)", 
	"Brute Tipper (2 Axle)", "Brute Tipper (3 Axle)", "Cutter", "Dock Handler", 
	"Dock Tug", "Dump Truck", "HVY Biff", 
	"Jobuilt Hauler", "Jobuilt Phantom", 
	"Jobuilt Rubble", "Maibatsu Mule (Graphics 1)", "Maibatsu Mule (Graphics 2)", 
	"Maibatsu Mule (Plain)", "Mixer", "Mixer (Support Wheel)", 
	"MTL Flatbed Truck", "MTL Packer", "MTL Pounder", 
	"Vapid Benson", "Vapid Scrap Truck", "Vapid Tow Truck", 
	"Vapid Tow Truck (Old)" };
//22.00
const std::vector<std::string> CAPTIONS_SERVICES{ 
	"Airtug", "Brute Airport Bus", "Brute Bus (City Bus)", 
	"Brute Rental Shuttle Bus", "Brute Tourbus", "Cable Car (Mt. Chilliad)", 
	"Dashound Dashound Coach", "Dozer", "Forklift", 
	"Jobuilt Trashmaster", "Jobuilt Trashmaster (Rusty)", "Nagasaki Caddy", 
	"Nagasaki Caddy (Golf)", "Ripley (Airport Tug)", "Stanley Fieldmaster Tractor", 
	"Stanley Fieldmaster Tractor (Snow)", "Stanley Lawn Mower", "Stanley Tractor (Rusty)", 
	"Vapid Pickup Utility", "Vapid Plumbing Utility", "Vapid Telephone Utility" };
//23.00
const std::vector<std::string> CAPTIONS_TRAILERS{ 
	"Army Flatbed Trailer (Empty)", "Army Flatbed Trailer (With Drill)", "Army Fuel Tanker", 
	"Boat Trailer", "Boat Trailer", "Car Transport Trailer", 
	"Car Transport Trailer (Empty)", "Commercial Trailer (Graphics 1)", "Commercial Trailer (Graphics 2)", 
	"Container Trailer", "Fame or Shame Trailer", "Flatbed Trailer", 
	"Flatbed Trailer", "Grain Trailer", "Hay Bale Trailer", 
	"Logging Trailer", "Meth Lab Trailer", "Petrol Tanker Trailer (Plain)", 
	"Petrol Tanker Trailer (RON)", "Plain Trailer", "Rake Trailer", 
	"Small Trailer" };
//24.00
const std::vector<std::string> CAPTIONS_TRAINS{ 
	"Container Car 1", "Container Car 2", "Flatbed Car", 
	"Freight Train Cab", "Grain Car", "Metro Train (Half)", 
	"Oil Tanker Car" };
//25.00
const std::vector<std::string> VALUES_PICKUPS{ "BISON", "BISON3", "BISON2", "RATLOADER2", "RATLOADER", "BOBCATXL", "SADLER", "SADLER2" };

const std::vector<std::string> VALUES_VANS{ "SURFER", "SURFER2", "PARADISE", "RUMPO2", "RUMPO", "YOUGA", "CAMPER", "PONY", "PONY2", "TACO", "BURRITO3", "BURRITO2", "BURRITO4", "GBURRITO2", "BURRITO", "BURRITO5", "GBURRITO", "MINIVAN", "SPEEDO", "SPEEDO2", "JOURNEY" };

const std::vector<std::string> VALUES_TRUCKS{ "BOXVILLE2", "BOXVILLE3", "BOXVILLE4", "BOXVILLE", "STOCKADE", "STOCKADE3", "TIPTRUCK", "TIPTRUCK2", "CUTTER", "HANDLER", "DOCKTUG", "DUMP", "BIFF", "HAULER", "PHANTOM", "RUBBLE", "MULE", "MULE2", "MULE3", "MIXER", "MIXER2", "FLATBED", "PACKER", "POUNDER", "BENSON", "SCRAP", "TOWTRUCK", "TOWTRUCK2" };

const std::vector<std::string> VALUES_SERVICES{ "AIRTUG", "AIRBUS", "BUS", "RENTALBUS", "TOURBUS", "CABLECAR", "COACH", "BULLDOZER", "FORKLIFT", "TRASH2", "TRASH", "CADDY2", "CADDY", "RIPLEY", "TRACTOR2", "TRACTOR3", "MOWER", "TRACTOR", "UTILLITRUCK3", "UTILLITRUCK2", "UTILLITRUCK" };

const std::vector<std::string> VALUES_TRAILERS{ "ARMYTRAILER", "ARMYTRAILER2", "ARMYTANKER", "BOATTRAILER", "TR3", "TR4", "TR2", "TRAILERS2", "TRAILERS3", "DOCKTRAILER", "TVTRAILER", "FREIGHTTRAILER", "TRFLAT", "GRAINTRAILER", "BALETRAILER", "TRAILERLOGS", "PROPTRAILER", "TANKER2", "TANKER", "TRAILERS", "RAKETRAILER", "TRAILERSMALL" };

const std::vector<std::string> VALUES_TRAINS{ "FREIGHTCONT1", "FREIGHTCONT2", "FREIGHTCAR", "FREIGHT", "FREIGHTGRAIN", "METROTRAIN", "TANKERCAR" };

const std::vector<std::string> VOV_INDUS_CAPTIONS[] = { CAPTIONS_PICKUPS, CAPTIONS_VANS, CAPTIONS_TRUCKS, CAPTIONS_SERVICES, CAPTIONS_TRAILERS, CAPTIONS_TRAINS };

const std::vector<std::string> VOV_INDUS_VALUES[] = { VALUES_PICKUPS, VALUES_VANS, VALUES_TRUCKS, VALUES_SERVICES, VALUES_TRAILERS, VALUES_TRAINS };

//Everything else

const std::vector<std::string> CAPTIONS_EMERGENCY{ 
	"Albany Police Roadcruiser (Snow)", "Ambulance", "Army Barracks Truck", 
	"Army Truck Cab", "Bravado Buffalo (FIB)", "Brute Police Riot Van", 
	"Canis Crusader (Army Mesa)", "Declasse Granger (FIB)", "Declasse Lifeguard", 
	"Declasse Park Ranger", "Declasse Police Rancher (Snow)", "Declasse Police Transporter", 
	"Declasse Sheriff SUV", "Firetruck", "Prison Bus", "Rhino Tank", 
	"Vapid Police Buffalo", "Vapid Police Cruiser", "Vapid Police Interceptor", 
	"Vapid Sheriff Cruiser", "Vapid Unmarked Police Cruiser", "Western Police Bike" };
//00.00 - 00.21
const std::vector<std::string> CAPTIONS_MOTORCYCLES{ 
	"Dinka Akuma", "Dinka Double-T", "Dinka Enduro", 
	"Dinka Thrust", "LCC Hexer", "LCC Innovation", 
	"Maibatsu Sanchez", "Maibatsu Sanchez (Graphics)", "Nagasaki Carbon RS", 
	"Pegassi Bati", "Pegassi Bati (Race)", "Pegassi Ruffian", 
	"Principe Lectro", "Principe Nemesis", "Principe Phaggio", 
	"Shitzu Hakuchou", "Shitzu PCJ 600", "Shitzu Vader", 
	"Western Bagger", "Western Daemon", "Western Sovereign" };
//01.00
const std::vector<std::string> CAPTIONS_PLANES{ 
	"Buckingham Cargo Plane (An-225)", "Buckingham Jet (B747)", "Buckingham Luxor", //.00 -.02
	"Buckingham Miljet", "Buckingham Shamal", "Buckingham Vestra", //.03 - .05
	"Jobuilt Mammatus", "Jobuilt P-996 Lazer", "Jobuilt Velum (4 Seater)", //.06
	"Jobuilt Velum (5 Seater)", "Mammoth Dodo", "Mammoth Hydra", //.09
	"Mammoth Titan", "Western Besra", "Western Cuban 800", //.12
	"Western Duster", "Western Mallard Stunt Plane" };
//02.00
const std::vector<std::string> CAPTIONS_HELOS{ 
	"Blimp (Atomic)", "Blimp (Xero Gas)", "Buckingham Savage", 
	"Buckingham Swift", "Buckingham Valkyrie", "HVY Skylift", 
	"Maibatsu Frogger", "Maibatsu Frogger (TPE/FIB)", "Nagasaki Buzzard (Unarmed)", 
	"Nagasaki Buzzard Attack Chopper", "Western Annihilator", "Western Cargobob (Desert Camo)", 
	"Western Cargobob (Jetsam)", "Western Cargobob (TPE)", "Western Maverick", 
	"Western Maverick (Police)" };
//03.00
const std::vector<std::string> CAPTIONS_BOATS{ 
	"Dinka Marquis", "Kraken Sub", "Nagasaki Dinghy (2 Seater)", 
	"Nagasaki Dinghy (4 Seater, Black)", "Nagasaki Dinghy (4 Seater, Red)", "Pegassi Speeder", 
	"Shitzu Jetmax", "Shitzu Predator (Police)", "Shitzu Squalo", 
	"Shitzu Suntrap", "Shitzu Tropic", "Speedophile Seashark", 
	"Speedophile Seashark (Lifeguard)", "Submersible" };
//04.00 - 04.13
const std::vector<std::string> CAPTIONS_BICYCLES{ 
	"BMX", "Cruiser", "Endurex Race", 
	"Fixter", "Scorcher", "Tri-Cycles Race", 
	"Whippet Race" };
//05.00
const std::vector<std::string> VALUES_EMERGENCY{ "POLICEOLD2", "AMBULANCE", "BARRACKS", "BARRACKS2", "FBI", "RIOT", "CRUSADER", "FBI2", "LGUARD", "PRANGER", "POLICEOLD1", "POLICET", "SHERIFF2", "FIRETRUK", "PBUS", "RHINO", "POLICE2", "POLICE", "POLICE3", "SHERIFF", "POLICE4", "POLICEB" };

const std::vector<std::string> VALUES_MOTORCYCLES{ "AKUMA", "DOUBLE", "ENDURO", "THRUST", "HEXER", "INNOVATION", "SANCHEZ2", "SANCHEZ", "CARBONRS", "BATI", "BATI2", "RUFFIAN", "LECTRO", "NEMESIS", "FAGGIO2", "HAKUCHOU", "PCJ", "VADER", "BAGGER", "DAEMON", "SOVEREIGN" };

const std::vector<std::string> VALUES_PLANES{ "CARGOPLANE", "JET", "LUXOR", "MILJET", "SHAMAL", "VESTRA", "MAMMATUS", "LAZER", "VELUM", "VELUM2", "DODO", "HYDRA", "TITAN", "BESRA", "CUBAN800", "DUSTER", "STUNT" };

const std::vector<std::string> VALUES_HELOS{ "BLIMP", "BLIMP2", "SAVAGE", "SWIFT", "VALKYRIE", "SKYLIFT", "FROGGER", "FROGGER2", "BUZZARD2", "BUZZARD", "ANNIHILATOR", "CARGOBOB", "CARGOBOB2", "CARGOBOB3", "MAVERICK", "POLMAV" };

const std::vector<std::string> VALUES_BOATS{ "MARQUIS", "SUBMERSIBLE2", "DINGHY2", "DINGHY3", "DINGHY", "SPEEDER", "JETMAX", "PREDATOR", "SQUALO", "SUNTRAP", "TROPIC", "SEASHARK", "SEASHARK2", "SUBMERSIBLE" };

const std::vector<std::string> VALUES_BICYCLES{ "BMX", "CRUISER", "TRIBIKE2", "FIXTER", "SCORCHER", "TRIBIKE3", "TRIBIKE" };

const std::vector<std::string> VOV_SHALLOW_CAPTIONS[] = { CAPTIONS_EMERGENCY, CAPTIONS_MOTORCYCLES, CAPTIONS_PLANES, CAPTIONS_HELOS, CAPTIONS_BOATS, CAPTIONS_BICYCLES };

const std::vector<std::string> VOV_SHALLOW_VALUES[] = { VALUES_EMERGENCY, VALUES_MOTORCYCLES, VALUES_PLANES, VALUES_HELOS, VALUES_BOATS, VALUES_BICYCLES };

float vectRads(float degs){
	float radialConv = degs*3.1415926536 / 180;
	return radialConv;
}

void vehicle_cannon(std::string defaultSpawn){
	Hash model = GAMEPLAY::GET_HASH_KEY((char *)defaultSpawn.c_str());
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL memoryKiller = IsKeyDown(get_config()->get_key_config()->key_veh_cannon);
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model)){
		if (IsKeyDown(get_config()->get_key_config()->key_veh_cannon)){
			Hash lastSpawn = GAMEPLAY::GET_HASH_KEY((char *)defaultSpawn.c_str());
			STREAMING::REQUEST_MODEL(model);
			while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
			FLOAT look = ENTITY::GET_ENTITY_HEADING(playerPed);
			FLOAT lookAni = look + 180.00;
			FLOAT lookOff = look + 90.00;
			FLOAT vecX = (cos(vectRads(lookOff))*(entSpeed));
			FLOAT vecY = (sin(vectRads(lookOff))*(entSpeed));
			if (featureVehicleCannonUpdated){
				if (currType == 2){
					if (lookAni > 360.00){
						lookAni = lookAni - 360.00;
					}
					spawnOffZ = -0.35;
					spawnOffY = 2.0f;
					waitTime = 75;
				}
				else if (currSpawnSize == "MD"){
					spawnOffY = 4.0;
					spawnOffZ = 0.0;
					waitTime = 40;
				}
				else if (currSpawnSize == "SM"){
					spawnOffY = 3.5;
					spawnOffZ = 0.0;
					waitTime = 35;
				}
				else if (currSpawnSize == "XS"){
					spawnOffY = 2.0;
					spawnOffZ = 0.0;
					waitTime = 25;
				}
				else if (currSpawnSize == "LG"){
					spawnOffY = 4.5;
					spawnOffZ = 0.0;
					waitTime = 45;
				}
				else if (currSpawnSize == "XXL"){
					spawnOffY = 6.75;
					spawnOffZ = 2.5;
					waitTime = 125;
				}
				else if (currSpawnSize == "XL"){
					spawnOffY = 10.5;
					spawnOffZ = 0.0;
					waitTime = 150;
					if (VEHICLE::IS_THIS_MODEL_A_TRAIN(model)){
						spawnOffZ = -1.0;
					}
				}
				else if (currSpawnSize == "XXXL"){
					spawnOffY = 15.0;
					spawnOffZ = 3.0;
					waitTime = 200;
				}
				else {
					spawnOffY = 4.0;
					spawnOffZ = 0.0;
					waitTime = 40;
					std::ostringstream ss;
					ss << "It\'s Broken";
					set_status_text(ss.str(), 3000UL);
				}
				if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
					spawnOffY = spawnOffY + 2.5;
				}
			}
			Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, spawnOffX, spawnOffY, spawnOffZ);
			if (currType == 1){
				Vehicle veh = VEHICLE::CREATE_VEHICLE(lastSpawn, coords.x, coords.y, coords.z, look, 1, 1);
				if ( currSpawnSize == "XXXL" || currSpawnSize == "XXL" ){
					ENTITY::SET_ENTITY_MAX_SPEED(veh, 700.00);
				}
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 225.00);

				WAIT(waitTime);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
			}
			else if (currType == 2){
				Ped peds = PED::CREATE_PED(28, model, coords.x, coords.y, coords.z, lookAni, 0, 0);
				PED::SET_PED_ACCURACY(peds, 100.00);
				PED::SET_PED_CAN_RAGDOLL(peds, 1);
				PED::SET_PED_DIES_WHEN_INJURED(peds, true);
				PED::APPLY_DAMAGE_TO_PED(peds, 15, true);
				PED::SET_PED_GRAVITY(peds, true);
				PED::SET_PED_TO_RAGDOLL(peds, 5, 5, 0, 0, 0, 0);
				PED::SET_PED_KEEP_TASK(peds, true);
				ENTITY::SET_ENTITY_MAX_SPEED(peds, 500.00);
				ENTITY::SET_ENTITY_VELOCITY(peds, vecX, vecY, 0.0);
				WAIT(waitTime);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&peds);
			}
		}
	}
}

bool onconfirm_can_menu(MenuItem<int> choice)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	switch (activeLineIndexPick)
	{
	case 0:
		if (process_carpick_menu()) 
		{ 
			return false;
		}
		break;
	case 1:
		process_fodderchanger_menu();
		return false;
		break;
	case 2:
		if (featureVehicleCannon)
		{
			return false;
		}
		break;
	case 3:
		if (featureVehicleCannonUpdated)
		{
			return false;
		}
		break;
	default:
		break;
	}
	return false;
}

void process_can_menu()
{
	const int lineCount = 3;

	std::string caption = "Vehicle Cannon";
	StandardOrToggleMenuDef lines[lineCount] = {
		{ "Vehicle Selector", NULL, NULL, false },
		{ "Ped Selector", NULL, NULL, false },
		{ "Vehicle Cannon", &featureVehicleCannon, NULL, false },
	};
	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexPick, caption, onconfirm_can_menu);
}

void update_cannon()
{
	if (featureVehicleCannon){
		vehicle_cannon(currSpawn);
	}
}

void reset_cannon_globals()
{
		featureVehicleCannon =
		featureVehicleCannonUpdated = false;
}

bool onconfirm_carpick_menu(MenuItem<int> choice)
{
	switch (choice.currentMenuIndex)
	{
	case 0:
		process_pick_menu_cars();
		break;
	case 1:
		process_pick_menu_indus();
		break;
	default:
		process_pick_menu_generic(choice.value);
	}
	return false;
}

bool process_carpick_menu()
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

	return draw_generic_menu<int>(menuItems, 0, "Vehicle Categories", onconfirm_carpick_menu, NULL, NULL);
}

bool onconfirm_pick_menu_cars(MenuItem<int> choice)
{
	std::string category = choice.caption;
	currSpawnClass = choice.value + 10.00;
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_CAR_VALUES[choice.value].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_CAR_CAPTIONS[choice.value][i];
		item->value = VOV_CAR_VALUES[choice.value][i];
		currSpawnClass = currSpawnClass + (i / 100);
		menuItems.push_back(item);
	}
	return draw_generic_menu<std::string>(menuItems, 0, category, onconfirm_pick_menu_vehicle_selection, NULL, NULL);
}

bool process_pick_menu_cars()
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

	return draw_generic_menu<int>(menuItems, 0, "Car Categories", onconfirm_pick_menu_cars, NULL, NULL);
}

bool onconfirm_pick_menu_indus(MenuItem<int> choice)
{
	int selection = choice.value;
	std::string category = choice.caption;
	currSpawnClass = choice.value + 20.00;
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_INDUS_CAPTIONS[selection].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_INDUS_CAPTIONS[selection][i];
		item->value = VOV_INDUS_VALUES[selection][i];
		currSpawnClass = currSpawnClass + (i / 100);
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, 0, category, onconfirm_pick_menu_vehicle_selection, NULL, NULL);
}

bool process_pick_menu_indus()
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

	return draw_generic_menu<int>(menuItems, 0, "Industrial Categories", onconfirm_pick_menu_indus, NULL, NULL);
}

bool onconfirm_pick_menu_vehicle_selection(MenuItem<std::string> choice)
{
	currType = 1;
	int currSpawnClassI = currSpawnClass;

	if ( currSpawnClassI == 23 || currSpawnClassI == 24 || currSpawnClassI == 25 ){
		currSpawnSize = "XL";
	}
	else if ( currSpawnClassI ==  0 || currSpawnClassI >= 20 || currSpawnClass == 4.13 ){
		currSpawnSize = "LG";
	}
	else if ( currSpawnClassI == 2 || currSpawnClassI == 3 ){
		currSpawnSize = "XXL";
	}
	else if ( currSpawnClassI == 5 || currSpawnClassI == 1 ){
		currSpawnSize = "XS";
	}
	else if ( currSpawnClassI == 10 || currSpawnClassI == 11 || currSpawnClassI == 12||currSpawnClassI == 13 || currSpawnClassI == 18 ){
		currSpawnSize = "SM";
	}
	else {
		currSpawnSize = "MD";
	}
	if (currSpawnClass == 2.00 || currSpawnClass == 2.12 ){
		currSpawnSize == "XXXL";
	}

	updateCannonChoice(choice.value, choice.caption);
	return false;
}

bool process_pick_menu_generic(int topMenuSelection)
{
	int selection = topMenuSelection - 2;
	std::string category = MENU_VEHICLE_CATEGORIES[topMenuSelection];
	currSpawnClass = selection *1.00;
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < VOV_SHALLOW_CAPTIONS[selection].size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_SHALLOW_CAPTIONS[selection][i];
		item->value = VOV_SHALLOW_VALUES[selection][i];
		currSpawnClass = currSpawnClass + (i / 100);
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, 0, category, onconfirm_pick_menu_vehicle_selection, NULL, NULL);
}

bool updateCannonChoice(std::string modelName, std::string modelTitle)
{
	DWORD model = GAMEPLAY::GET_HASH_KEY((char *)modelName.c_str());
	currSpawn = modelName;
	currSpawnTitle = modelTitle;
	featureVehicleCannonUpdated = true;
	std::ostringstream ss;
	ss << modelTitle << " loaded";
	set_status_text(ss.str());
	return true;
}

bool onconfirm_fodderchanger_choices_animals(MenuItem<std::string> choice)
{
	currType = 2;
	fodderTypesMenuPositionMemory[0] = choice.currentMenuIndex;
	updateCannonChoice(choice.value, choice.caption);
	return false;
}

bool process_fodderchanger_choices_animals()
{
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < SKINS_ANIMAL_CAPTIONS.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_ANIMAL_CAPTIONS[i];
		item->value = SKINS_ANIMAL_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &fodderTypesMenuPositionMemory[0], "Animal Skins", onconfirm_fodderchanger_choices_animals, NULL, NULL);
}

bool onconfirm_fodderchanger_choices_misc(MenuItem<std::string> choice)
{
	currType = 2;
	fodderTypesMenuPositionMemory[1] = choice.currentMenuIndex;
	updateCannonChoice(choice.value, choice.caption);
	return false;
}

bool process_fodderchanger_choices_misc()
{
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < SKINS_GENERAL_CAPTIONS.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_GENERAL_CAPTIONS[i];
		item->value = SKINS_GENERAL_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &fodderTypesMenuPositionMemory[1], "General Skins", onconfirm_fodderchanger_choices_misc, NULL, NULL);
}

bool onconfirm_fodderchanger_menu(MenuItem<int> choice)
{
	fodderMainMenuPosition = choice.currentMenuIndex;

	switch (choice.value)
	{
	case 0:
		process_fodderchanger_choices_animals();
		break;
	case 1:
		process_fodderchanger_choices_misc();
		break;
	default:
		break;
	}
	return false;
}

bool process_fodderchanger_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	for (int i = 0; i < MENU_SKINS_TYPES_CAPTIONS.size(); i++)
	{
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_SKINS_TYPES_CAPTIONS[i];
		item->value = i;
		item->isLeaf = false; //only Reset is a leaf
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &fodderMainMenuPosition, "Player Skin Options", onconfirm_fodderchanger_menu, NULL, NULL);
}

std::vector<FeatureEnabledLocalDefinition> get_feature_state_cannon()
{
	std::vector<FeatureEnabledLocalDefinition> results;
	results.push_back(FeatureEnabledLocalDefinition{ "featureVehicleCannon", &featureVehicleCannon, &featureVehicleCannonUpdated });
	return results;
}
