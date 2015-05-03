/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "skins.h"
#include "menu_functions.h"

/***
* VARS
*/

std::string chosenSkinName = "";

bool DEBUG_MODE_SKINS = false;

int skinDetailMenuIndex = 0;
int skinDetailMenuValue = 0;

int skinDrawableMenuIndex = 0;
int skinDrawableMenuValue = -1;

int skinTextureMenuIndex = 0;
int skinTextureMenuValue = -1;

bool skinchanger_used = false;

const std::vector<std::string> MENU_SKINS_TYPES_CAPTIONS{ "Players", "Animals", "Everything Else", "Customise Current", "Reset Current" };

int skinMainMenuPosition = 0;

int skinTypesMenuPositionMemory[3] = { 0, 0, 0 }; //player, animals, general

const std::vector<std::string> SKINS_PLAYER_CAPTIONS{ "Michael", "Franklin", "Trevor" };
const std::vector<std::string> SKINS_PLAYER_VALUES{ "player_zero", "player_one", "player_two" };

const std::vector<std::string> SKINS_ANIMAL_CAPTIONS{ "Boar", "Chimp", "Cow", "Coyote", "Deer", "Fish", "Hen", "Cat", "Hawk", "Cormorant", "Crow", "Dolphin", "Humpback", "Whale", "Pigeon", "Seagull", "Hammerhead Shark", "Pig", "Rat", "Rhesus", "Chop", "Husky", "Mountain Lion", "Retriever", "Tiger Shark", "German Shepherd" };
const std::vector<std::string> SKINS_ANIMAL_VALUES{ "a_c_boar", "a_c_chimp", "a_c_cow", "a_c_coyote", "a_c_deer", "a_c_fish", "a_c_hen", "a_c_cat_01", "a_c_chickenhawk", "a_c_cormorant", "a_c_crow", "a_c_dolphin", "a_c_humpback", "a_c_killerwhale", "a_c_pigeon", "a_c_seagull", "a_c_sharkhammer", "a_c_pig", "a_c_rat", "a_c_rhesus", "a_c_chop", "a_c_husky", "a_c_mtlion", "a_c_retriever", "a_c_sharktiger", "a_c_shepherd" };

const std::vector<std::string> SKINS_GENERAL_CAPTIONS{ "Abigail Mathers", "Abigail Mathers (IG)", "Abner", "African American Male", "Air Hostess", "Air Worker Male", "Al Di Napoli Male", "Alien", "Altruist Cult Mid-Age Male", "Altruist Cult Old Male", "Altruist Cult Old Male 2", "Altruist Cult Young Male", "Altruist Cult Young Male 2", "Amanda De Santa (CS)", "Amanda De Santa (IG)", "Ammu-Nation City Clerk", "Ammu-Nation Rural Clerk", "Andreas Sanchez (CS)", "Andreas Sanchez (IG)", "Anita Mendoza", "Anton Beaudelaire", "Anton Beaudelaire", "Armenian Boss", "Armenian Goon", "Armenian Goon 2", "Armenian Lieutenant", "Armoured Van Security", "Armoured Van Security", "Armoured Van Security 2", "Army Mechanic", "Ashley Butler (CS)", "Ashley Butler (IG)", "Autopsy Tech", "Autoshop Worker", "Autoshop Worker 2", "Azteca", "Baby D", "Ballas East Male", "Ballas Female", "Ballas OG", "Ballas OG (IG)", "Ballas Original Male (IG)", "Ballas South Male", "Bank Manager (CS)", "Bank Manager (IG)", "Bank Manager Male", "Barber Female", "Barman", "Barry (CS)", "Barry (IG)", "Bartender", "Bartender (Rural)", "Baywatch Female", "Baywatch Male", "Beach Female", "Beach Male", "Beach Male 2", "Beach Muscle Male", "Beach Muscle Male 2", "Beach Old Male", "Beach Tramp Female", "Beach Tramp Male", "Beach Young Female", "Beach Young Male", "Beach Young Male 2", "Beach Young Male 3", "Best Man (IG)", "Beverly Felton (CS)", "Beverly Felton (IG)", "Beverly Hills Female", "Beverly Hills Female 2", "Beverly Hills Male", "Beverly Hills Male 2", "Beverly Hills Young Female", "Beverly Hills Young Female 2", "Beverly Hills Young Female 3", "Beverly Hills Young Female 4", "Beverly Hills Young Male", "Beverly Hills Young Male 2", "Bigfoot (CS)", "Bigfoot (IG)", "Bike Hire Guy", "Biker Chic Female", "Black Ops Soldier", "Black Ops Soldier 2", "Black Street Male", "Black Street Male 2", "Bodybuilder Female", "Bouncer", "Brad (CS)", "Brad (IG)", "Brad's Cadaver (CS)", "Breakdancer Male", "Bride", "Bride (IG)", "Burger Drug Worker", "Burger Drug Worker", "Busboy", "Business Casual", "Business Female 2", "Business Male", "Business Young Female", "Business Young Female 2", "Business Young Female 3", "Business Young Female 4", "Business Young Male", "Business Young Male 2", "Business Young Male 3", "Busker", "Car 3 Guy 1", "Car 3 Guy 1 (IG)", "Car 3 Guy 2", "Car 3 Guy 2 (IG)", "Car Buyer (CS)", "Casey (CS)", "Casey (IG)", "Chef", "Chef", "Chef (IG)", "Chemical Plant Security", "Chemical Plant Worker", "Chinese Boss", "Chinese Goon", "Chinese Goon", "Chinese Goon 2", "Chinese Goon Older", "Chip", "Claude Speed", "Clay Jackson (The Pain Giver) (IG)", "Clay Simons (The Lost) (CS)", "Clay Simons (The Lost) (IG)", "Cletus", "Cletus (IG)", "Clown", "Construction Worker", "Construction Worker 2", "Cop", "Cop Female", "Cop Male", "Corpse Female", "Corpse Young Female", "Corpse Young Female 2", "Cris Formage (CS)", "Cris Formage (IG)", "Customer", "Cyclist Male", "Cyclist Male", "Dale (CS)", "Dale (IG)", "Dave Norton (CS)", "Dave Norton (IG)", "Dead Hooker", "Dealer", "Debra (CS)", "Denise (CS)", "Denise (IG)", "Denise's Friend", "Devin (CS)", "Devin (IG)", "Devin's Security", "Dock Worker", "Dock Worker", "Doctor", "Dom Beasley (CS)", "Dom Beasley (IG)", "Doorman", "Downhill Cyclist", "Downtown Female", "Downtown Male", "Dr. Friedlander (CS)", "Dr. Friedlander (IG)", "Dressy Female", "DW Airport Worker", "DW Airport Worker 2", "East SA Female", "East SA Female 2", "East SA Male", "East SA Male 2", "East SA Young Female", "East SA Young Female 2", "East SA Young Female 3", "East SA Young Male", "East SA Young Male 2", "Epsilon Female", "Epsilon Male", "Epsilon Male 2", "Epsilon Tom (CS)", "Epsilon Tom (IG)", "Ex-Army Male", "Ex-Mil Bum", "Fabien (CS)", "Fabien (IG)", "Factory Worker Female", "Factory Worker Male", "Families CA Male", "Families DD Male", "Families DNF Male", "Families Female", "Families FOR Male", "Families Gang Member?", "Families Gang Member? (IG)", "Farmer", "Fat Black Female", "Fat Cult Female", "Fat Latino Male", "Fat White Female", "Ferdinand Kerimov (Mr. K) (CS)", "Ferdinand Kerimov (Mr. K) (IG)", "FIB Architect", "FIB Mugger", "FIB Office Worker", "FIB Office Worker 2", "FIB Security", "FIB Suit (CS)", "FIB Suit (IG)", "Financial Guru", "Fireman Male", "Fitness Female", "Fitness Female 2", "Floyd Hebert (CS)", "Floyd Hebert (IG)", "FOS Rep?", "Free Mode Female", "Free Mode Male", "G?", "Gaffer", "Garbage Worker", "Gardener", "Gay Male", "Gay Male 2", "General Fat Male", "General Fat Male 2", "General Hot Young Female", "General Street Old Female", "General Street Old Male", "General Street Young Male", "General Street Young Male 2", "GLENSTANK? Male", "Golfer Male", "Golfer Young Female", "Golfer Young Male", "Griff", "Grip", "Groom", "Groom (IG)", "Grove Street Dealer", "Guadalope (CS)", "Guido", "Gun Vendor", "GURK? (CS)", "Hairdresser Male", "Hao", "Hao (IG)", "Hasidic Jew Male", "Hasidic Jew Young Male", "Hick", "Hick (IG)", "High Security", "High Security 2", "Highway Cop", "Hiker Female", "Hiker Male", "Hillbilly Male", "Hillbilly Male 2", "Hippie Female", "Hippie Male", "Hippie Male", "Hipster", "Hipster (IG)", "Hipster Female", "Hipster Female 2", "Hipster Female 3", "Hipster Female 4", "Hipster Male", "Hipster Male 2", "Hipster Male 3", "Hooker", "Hooker 2", "Hooker 3", "Hospital Scrubs Female", "Hot Posh Female", "Hugh Welsh", "Hunter (CS)", "Hunter (IG)", "IAA Security", "Impotent Rage", "Imran Shinowa", "Indian Male", "Indian Old Female", "Indian Young Female", "Indian Young Male", "Jane", "Janet (CS)", "Janet (IG)", "Janitor", "Janitor", "Jay Norris (IG)", "Jesco White (Tapdancing Hillbilly)", "Jesus", "Jetskier", "Jewel Heist Driver", "Jewel Heist Gunman", "Jewel Heist Hacker", "Jewel Thief", "Jeweller Assistant", "Jeweller Assistant (CS)", "Jeweller Assistant (IG)", "Jeweller Security", "Jimmy Boston (CS)", "Jimmy Boston (IG)", "Jimmy De Santo (CS)", "Jimmy De Santo (IG)", "Jogger Female", "Jogger Male", "Jogger Male 2", "John Marston", "Johnny Klebitz (CS)", "Johnny Klebitz (IG)", "Josef (CS)", "Josef (IG)", "Josh (CS)", "Josh (IG)", "Juggalo Female", "Juggalo Male", "Justin", "Kerry McIntosh (IG)", "Kifflom Guy", "Korean Boss", "Korean Female", "Korean Female 2", "Korean Lieutenant", "Korean Male", "Korean Old Female", "Korean Old Male", "Korean Young Male", "Korean Young Male", "Korean Young Male 2", "Korean Young Male 2", "Lamar Davis (CS)", "Lamar Davis (IG)", "Latino Handyman Male", "Latino Street Male 2", "Latino Street Young Male", "Latino Young Male", "Lazlow (CS)", "Lazlow (IG)", "Lester Crest (CS)", "Lester Crest (IG)", "Life Invader (CS)", "Life Invader (IG)", "Life Invader 2 (IG)", "Life Invader Male", "Line Cook", "Love Fist Willy", "LS Metro Worker Male", "Magenta (CS)", "Magenta (IG)", "Maid", "Malibu Male", "Mani", "Manuel (CS)", "Manuel (IG)", "Mariachi", "Marine", "Marine", "Marine 2", "Marine Young", "Marine Young 2", "Marine Young 3", "Mark Fostenburg", "Marnie Allen (CS)", "Marnie Allen (IG)", "Martin Madrazo (CS)", "Mary-Ann Quinn (CS)", "Mary-Ann Quinn (IG)", "Maude", "Maude (IG)", "Mechanic", "Mechanic 2", "Merryweather Merc", "Meth Addict", "Mexican", "Mexican (IG)", "Mexican Boss", "Mexican Boss 2", "Mexican Gang Member", "Mexican Goon", "Mexican Goon 2", "Mexican Goon 3", "Mexican Labourer", "Mexican Rural", "Mexican Thug", "Michelle (CS)", "Michelle (IG)", "Migrant Female", "Migrant Male", "Milton McIlroy (CS)", "Milton McIlroy (IG)", "Mime Artist", "Minuteman Joe (CS)", "Minuteman Joe (IG)", "Miranda", "Mistress", "Misty", "Molly (CS)", "Molly (IG)", "Motocross Biker", "Motocross Biker 2", "Movie Astronaut", "Movie Director", "Movie Premiere Female", "Movie Premiere Female (CS)", "Movie Premiere Male", "Movie Premiere Male (CS)", "Movie Star Female", "Mrs. Phillips (CS)", "Mrs. Phillips (IG)", "Mrs. Thornhill (CS)", "Mrs. Thornhill (IG)", "Natalia (CS)", "Natalia (IG)", "Nervous Ron (CS)", "Nervous Ron (IG)", "Nigel (CS)", "Nigel (IG)", "Niko Bellic", "OG Boss", "Old Man 1 (CS)", "Old Man 1 (IG)", "Old Man 2 (CS)", "Old Man 2 (IG)", "Omega (CS)", "Omega (IG)", "O'Neil Brothers (IG)", "Ortega", "Ortega (IG)", "Oscar", "Paparazzi Male", "Paparazzi Young Male", "Paramedic", "Party Target", "Partygoer", "Patricia (CS)", "Patricia (IG)", "Pest Control", "Peter Dreyfuss (CS)", "Peter Dreyfuss (IG)", "Pilot", "Pilot", "Pilot 2", "Pogo the Monkey", "Polynesian", "Polynesian Goon", "Polynesian Goon 2", "Polynesian Young", "Poppy Mitchell", "Porn Dude", "Postal Worker Male", "Postal Worker Male 2", "Priest (CS)", "Priest (IG)", "Princess", "Prison Guard", "Prisoner", "Prisoner", "Prisoner (Muscular)", "Prologue Driver", "Prologue Driver", "Prologue Host Female", "Prologue Host Male", "Prologue Host Old Female", "Prologue Mourner Female", "Prologue Mourner Male", "Prologue Security", "Prologue Security", "Prologue Security 2 (CS)", "Prologue Security 2 (IG)", "PROS?", "Ranger Female", "Ranger Male", "Reporter", "Republican Space Ranger", "Rival Paparazzo", "Road Cyclist", "Robber", "Rocco Pelosi", "Rocco Pelosi (IG)", "Rural Meth Addict Female", "Rural Meth Addict Male", "Russian Drunk (CS)", "Russian Drunk (IG)", "Sales Assistant (High-End)", "Sales Assistant (Low-End)", "Sales Assistant (Mask Stall)", "Sales Assistant (Mid-Price)", "Salton Female", "Salton Male", "Salton Male 2", "Salton Male 3", "Salton Male 4", "Salton Old Female", "Salton Old Male", "Salton Young Male", "Salvadoran Boss", "Salvadoran Goon", "Salvadoran Goon 2", "Salvadoran Goon 3", "Scientist", "Screenwriter", "Screenwriter (IG)", "Security Guard", "Sheriff Female", "Sheriff Male", "Shopkeeper", "Simeon Yetarian (CS)", "Simeon Yetarian (IG)", "Skater Female", "Skater Male", "Skater Young Male", "Skater Young Male 2", "Skid Row Female", "Skid Row Male", "Snow Cop Male", "Solomon Richards (CS)", "Solomon Richards (IG)", "South Central Female", "South Central Female 2", "South Central Latino Male", "South Central Male", "South Central Male 2", "South Central Male 3", "South Central Male 4", "South Central MC Female", "South Central Old Female", "South Central Old Female 2", "South Central Old Male", "South Central Old Male 2", "South Central Old Male 3", "South Central Young Female", "South Central Young Female 2", "South Central Young Female 3", "South Central Young Male", "South Central Young Male 2", "South Central Young Male 3", "South Central Young Male 4", "Sports Biker", "Spy Actor", "Spy Actress", "Stag Party Groom", "Steve Haines (CS)", "Steve Haines (IG)", "Street Performer", "Street Preacher", "Street Punk", "Street Punk 2", "Street Vendor", "Street Vendor Young", "Stretch (CS)", "Stretch (IG)", "Stripper", "Stripper", "Stripper 2", "Stripper 2", "Stripper Lite", "Stripper Lite", "Sunbather Male", "Surfer", "SWAT", "Sweatshop Worker", "Sweatshop Worker Young", "Talina (IG)", "Tanisha (CS)", "Tanisha (IG)", "Tao Cheng (CS)", "Tao Cheng (IG)", "Tao's Translator (CS)", "Tao's Translator (IG)", "Tattoo Artist", "Tennis Coach (CS)", "Tennis Coach (IG)", "Tennis Player Female", "Tennis Player Male", "Terry (CS)", "Terry (IG)", "The Lost MC Female", "The Lost MC Male", "The Lost MC Male 2", "The Lost MC Male 3", "Tom (CS)", "Tonya", "Tonya (IG)", "Topless", "Tourist Female", "Tourist Male", "Tourist Young Female", "Tourist Young Female 2", "Tracey De Santa (CS)", "Tracey De Santa (IG)", "Traffic Warden", "Traffic Warden (IG)", "Tramp Female", "Tramp Male", "Tramp Old Male", "Tramp Old Male", "Transport Worker Male", "Transvestite Male", "Transvestite Male 2", "Trucker Male", "Tyler Dixon (IG)", "United Paper Man (CS)", "United Paper Man (IG)", "UPS Driver", "UPS Driver 2", "US Coastguard", "Vagos Female", "Valet", "Vespucci Beach Male", "Vespucci Beach Male 2", "Vinewood Douche", "Vinewood Female", "Vinewood Female 2", "Vinewood Female 3", "Vinewood Female 4", "Vinewood Male", "Vinewood Male 2", "Vinewood Male 3", "Vinewood Male 4", "Wade (CS)", "Wade (IG)", "Waiter", "Wei Cheng (CS)", "Wei Cheng (IG)", "White Street Male", "White Street Male 2", "Window Cleaner", "Yoga Female", "Yoga Male", "Zimbor (CS)", "Zimbor (IG)", "Zombie" };
const std::vector<std::string> SKINS_GENERAL_VALUES{ "csb_abigail", "ig_abigail", "u_m_y_abner", "a_m_m_afriamer_01", "s_f_y_airhostess_01", "s_m_y_airworker", "u_m_m_aldinapoli", "s_m_m_movalien_01", "a_m_m_acult_01", "a_m_o_acult_01", "a_m_o_acult_02", "a_m_y_acult_01", "a_m_y_acult_02", "cs_amandatownley", "ig_amandatownley", "s_m_y_ammucity_01", "s_m_m_ammucountry", "cs_andreas", "ig_andreas", "csb_anita", "csb_anton", "u_m_y_antonb", "g_m_m_armboss_01", "g_m_m_armgoon_01", "g_m_y_armgoon_02", "g_m_m_armlieut_01", "mp_s_m_armoured_01", "s_m_m_armoured_01", "s_m_m_armoured_02", "s_m_y_armymech_01", "cs_ashley", "ig_ashley", "s_m_y_autopsy_01", "s_m_m_autoshop_01", "s_m_m_autoshop_02", "g_m_y_azteca_01", "u_m_y_babyd", "g_m_y_ballaeast_01", "g_f_y_ballas_01", "csb_ballasog", "ig_ballasog", "g_m_y_ballaorig_01", "g_m_y_ballasout_01", "cs_bankman", "ig_bankman", "u_m_m_bankman", "s_f_m_fembarber", "s_m_y_barman_01", "cs_barry", "ig_barry", "s_f_y_bartender_01", "s_m_m_cntrybar_01", "s_f_y_baywatch_01", "s_m_y_baywatch_01", "a_f_m_beach_01", "a_m_m_beach_01", "a_m_m_beach_02", "a_m_y_musclbeac_01", "a_m_y_musclbeac_02", "a_m_o_beach_01", "a_f_m_trampbeac_01", "a_m_m_trampbeac_01", "a_f_y_beach_01", "a_m_y_beach_01", "a_m_y_beach_02", "a_m_y_beach_03", "ig_bestmen", "cs_beverly", "ig_beverly", "a_f_m_bevhills_01", "a_f_m_bevhills_02", "a_m_m_bevhills_01", "a_m_m_bevhills_02", "a_f_y_bevhills_01", "a_f_y_bevhills_02", "a_f_y_bevhills_03", "a_f_y_bevhills_04", "a_m_y_bevhills_01", "a_m_y_bevhills_02", "cs_orleans", "ig_orleans", "u_m_m_bikehire_01", "u_f_y_bikerchic", "s_m_y_blackops_01", "s_m_y_blackops_02", "a_m_y_stbla_01", "a_m_y_stbla_02", "a_f_m_bodybuild_01", "s_m_m_bouncer_01", "cs_brad", "ig_brad", "cs_bradcadaver", "a_m_y_breakdance_01", "csb_bride", "ig_bride", "csb_burgerdrug", "u_m_y_burgerdrug_01", "s_m_y_busboy_01", "a_m_y_busicas_01", "a_f_m_business_02", "a_m_m_business_01", "a_f_y_business_01", "a_f_y_business_02", "a_f_y_business_03", "a_f_y_business_04", "a_m_y_business_01", "a_m_y_business_02", "a_m_y_business_03", "s_m_o_busker_01", "csb_car3guy1", "ig_car3guy1", "csb_car3guy2", "ig_car3guy2", "cs_carbuyer", "cs_casey", "ig_casey", "s_m_y_chef_01", "csb_chef", "ig_chef", "s_m_m_chemsec_01", "g_m_m_chemwork_01", "g_m_m_chiboss_01", "g_m_m_chigoon_01", "csb_chin_goon", "g_m_m_chigoon_02", "g_m_m_chicold_01", "u_m_y_chip", "mp_m_claude_01", "ig_claypain", "cs_clay", "ig_clay", "csb_cletus", "ig_cletus", "s_m_y_clown_01", "s_m_y_construct_01", "s_m_y_construct_02", "csb_cop", "s_f_y_cop_01", "s_m_y_cop_01", "u_f_m_corpse_01", "u_f_y_corpse_01", "u_f_y_corpse_02", "cs_chrisformage", "ig_chrisformage", "csb_customer", "a_m_y_cyclist_01", "u_m_y_cyclist_01", "cs_dale", "ig_dale", "cs_davenorton", "ig_davenorton", "mp_f_deadhooker", "s_m_y_dealer_01", "cs_debra", "cs_denise", "ig_denise", "csb_denise_friend", "cs_devin", "ig_devin", "s_m_y_devinsec_01", "s_m_m_dockwork_01", "s_m_y_dockwork_01", "s_m_m_doctor_01", "cs_dom", "ig_dom", "s_m_y_doorman_01", "a_m_y_dhill_01", "a_f_m_downtown_01", "a_m_y_downtown_01", "cs_drfriedlander", "ig_drfriedlander", "a_f_y_scdressy_01", "s_m_y_dwservice_01", "s_m_y_dwservice_02", "a_f_m_eastsa_01", "a_f_m_eastsa_02", "a_m_m_eastsa_01", "a_m_m_eastsa_02", "a_f_y_eastsa_01", "a_f_y_eastsa_02", "a_f_y_eastsa_03", "a_m_y_eastsa_01", "a_m_y_eastsa_02", "a_f_y_epsilon_01", "a_m_y_epsilon_01", "a_m_y_epsilon_02", "cs_tomepsilon", "ig_tomepsilon", "mp_m_exarmy_01", "u_m_y_militarybum", "cs_fabien", "ig_fabien", "s_f_y_factory_01", "s_m_y_factory_01", "g_m_y_famca_01", "mp_m_famdd_01", "g_m_y_famdnf_01", "g_f_y_families_01", "g_m_y_famfor_01", "csb_ramp_gang", "ig_ramp_gang", "a_m_m_farmer_01", "a_f_m_fatbla_01", "a_f_m_fatcult_01", "a_m_m_fatlatin_01", "a_f_m_fatwhite_01", "cs_mrk", "ig_mrk", "u_m_m_fibarchitect", "u_m_y_fibmugger_01", "s_m_m_fiboffice_01", "s_m_m_fiboffice_02", "mp_m_fibsec_01", "cs_fbisuit_01", "ig_fbisuit_01", "u_m_o_finguru_01", "s_m_y_fireman_01", "a_f_y_fitness_01", "a_f_y_fitness_02", "cs_floyd", "ig_floyd", "csb_fos_rep", "mp_f_freemode_01", "mp_m_freemode_01", "csb_g", "s_m_m_gaffer_01", "s_m_y_garbage", "s_m_m_gardener_01", "a_m_y_gay_01", "a_m_y_gay_02", "a_m_m_genfat_01", "a_m_m_genfat_02", "a_f_y_genhot_01", "a_f_o_genstreet_01", "a_m_o_genstreet_01", "a_m_y_genstreet_01", "a_m_y_genstreet_02", "u_m_m_glenstank_01", "a_m_m_golfer_01", "a_f_y_golfer_01", "a_m_y_golfer_01", "u_m_m_griff_01", "s_m_y_grip_01", "csb_groom", "ig_groom", "csb_grove_str_dlr", "cs_guadalope", "u_m_y_guido_01", "u_m_y_gunvend_01", "cs_gurk", "s_m_m_hairdress_01", "csb_hao", "ig_hao", "a_m_m_hasjew_01", "a_m_y_hasjew_01", "csb_ramp_hic", "ig_ramp_hic", "s_m_m_highsec_01", "s_m_m_highsec_02", "s_m_y_hwaycop_01", "a_f_y_hiker_01", "a_m_y_hiker_01", "a_m_m_hillbilly_01", "a_m_m_hillbilly_02", "a_f_y_hippie_01", "u_m_y_hippie_01", "a_m_y_hippy_01", "csb_ramp_hipster", "ig_ramp_hipster", "a_f_y_hipster_01", "a_f_y_hipster_02", "a_f_y_hipster_03", "a_f_y_hipster_04", "a_m_y_hipster_01", "a_m_y_hipster_02", "a_m_y_hipster_03", "s_f_y_hooker_01", "s_f_y_hooker_02", "s_f_y_hooker_03", "s_f_y_scrubs_01", "u_f_y_hotposh_01", "csb_hugh", "cs_hunter", "ig_hunter", "s_m_m_ciasec_01", "u_m_y_imporage", "csb_imran", "a_m_m_indian_01", "a_f_o_indian_01", "a_f_y_indian_01", "a_m_y_indian_01", "u_f_y_comjane", "cs_janet", "ig_janet", "csb_janitor", "s_m_m_janitor", "ig_jay_norris", "u_m_o_taphillbilly", "u_m_m_jesus_01", "a_m_y_jetski_01", "hc_driver", "hc_gunman", "hc_hacker", "u_m_m_jewelthief", "u_f_y_jewelass_01", "cs_jewelass", "ig_jewelass", "u_m_m_jewelsec_01", "cs_jimmyboston", "ig_jimmyboston", "cs_jimmydisanto", "ig_jimmydisanto", "a_f_y_runner_01", "a_m_y_runner_01", "a_m_y_runner_02", "mp_m_marston_01", "cs_johnnyklebitz", "ig_johnnyklebitz", "cs_josef", "ig_josef", "cs_josh", "ig_josh", "a_f_y_juggalo_01", "a_m_y_juggalo_01", "u_m_y_justin", "ig_kerrymcintosh", "u_m_y_baygor", "g_m_m_korboss_01", "a_f_m_ktown_01", "a_f_m_ktown_02", "g_m_y_korlieut_01", "a_m_m_ktown_01", "a_f_o_ktown_01", "a_m_o_ktown_01", "g_m_y_korean_01", "a_m_y_ktown_01", "g_m_y_korean_02", "a_m_y_ktown_02", "cs_lamardavis", "ig_lamardavis", "s_m_m_lathandy_01", "a_m_m_stlat_02", "a_m_y_stlat_01", "a_m_y_latino_01", "cs_lazlow", "ig_lazlow", "cs_lestercrest", "ig_lestercrest", "cs_lifeinvad_01", "ig_lifeinvad_01", "ig_lifeinvad_02", "s_m_m_lifeinvad_01", "s_m_m_linecook", "u_m_m_willyfist", "s_m_m_lsmetro_01", "cs_magenta", "ig_magenta", "s_f_m_maid_01", "a_m_m_malibu_01", "u_m_y_mani", "cs_manuel", "ig_manuel", "s_m_m_mariachi_01", "csb_ramp_marine", "s_m_m_marine_01", "s_m_m_marine_02", "s_m_y_marine_01", "s_m_y_marine_02", "s_m_y_marine_03", "u_m_m_markfost", "cs_marnie", "ig_marnie", "cs_martinmadrazo", "cs_maryann", "ig_maryann", "csb_maude", "ig_maude", "s_m_y_xmech_01", "s_m_y_xmech_02", "csb_mweather", "a_m_y_methhead_01", "csb_ramp_mex", "ig_ramp_mex", "g_m_m_mexboss_01", "g_m_m_mexboss_02", "g_m_y_mexgang_01", "g_m_y_mexgoon_01", "g_m_y_mexgoon_02", "g_m_y_mexgoon_03", "a_m_m_mexlabor_01", "a_m_m_mexcntry_01", "a_m_y_mexthug_01", "cs_michelle", "ig_michelle", "s_f_y_migrant_01", "s_m_m_migrant_01", "cs_milton", "ig_milton", "s_m_y_mime", "cs_joeminuteman", "ig_joeminuteman", "u_f_m_miranda", "u_f_y_mistress", "mp_f_misty_01", "cs_molly", "ig_molly", "a_m_y_motox_01", "a_m_y_motox_02", "s_m_m_movspace_01", "u_m_m_filmdirector", "s_f_y_movprem_01", "cs_movpremf_01", "s_m_m_movprem_01", "cs_movpremmale", "u_f_o_moviestar", "cs_mrsphillips", "ig_mrsphillips", "cs_mrs_thornhill", "ig_mrs_thornhill", "cs_natalia", "ig_natalia", "cs_nervousron", "ig_nervousron", "cs_nigel", "ig_nigel", "mp_m_niko_01", "a_m_m_og_boss_01", "cs_old_man1a", "ig_old_man1a", "cs_old_man2", "ig_old_man2", "cs_omega", "ig_omega", "ig_oneil", "csb_ortega", "ig_ortega", "csb_oscar", "a_m_m_paparazzi_01", "u_m_y_paparazzi", "s_m_m_paramedic_01", "u_m_m_partytarget", "u_m_y_party_01", "cs_patricia", "ig_patricia", "s_m_y_pestcont_01", "cs_dreyfuss", "ig_dreyfuss", "s_m_m_pilot_01", "s_m_y_pilot_01", "s_m_m_pilot_02", "u_m_y_pogo_01", "a_m_m_polynesian_01", "g_m_y_pologoon_01", "g_m_y_pologoon_02", "a_m_y_polynesian_01", "u_f_y_poppymich", "csb_porndudes", "s_m_m_postal_01", "s_m_m_postal_02", "cs_priest", "ig_priest", "u_f_y_princess", "s_m_m_prisguard_01", "s_m_y_prisoner_01", "u_m_y_prisoner_01", "s_m_y_prismuscl_01", "u_m_y_proldriver_01", "csb_prologuedriver", "a_f_m_prolhost_01", "a_m_m_prolhost_01", "u_f_o_prolhost_01", "u_f_m_promourn_01", "u_m_m_promourn_01", "csb_prolsec", "u_m_m_prolsec_01", "cs_prolsec_02", "ig_prolsec_02", "mp_g_m_pros_01", "s_f_y_ranger_01", "s_m_y_ranger_01", "csb_reporter", "u_m_y_rsranger_01", "u_m_m_rivalpap", "a_m_y_roadcyc_01", "s_m_y_robber_01", "csb_roccopelosi", "ig_roccopelosi", "a_f_y_rurmeth_01", "a_m_m_rurmeth_01", "cs_russiandrunk", "ig_russiandrunk", "s_f_m_shop_high", "s_f_y_shop_low", "s_m_y_shop_mask", "s_f_y_shop_mid", "a_f_m_salton_01", "a_m_m_salton_01", "a_m_m_salton_02", "a_m_m_salton_03", "a_m_m_salton_04", "a_f_o_salton_01", "a_m_o_salton_01", "a_m_y_salton_01", "g_m_y_salvaboss_01", "g_m_y_salvagoon_01", "g_m_y_salvagoon_02", "g_m_y_salvagoon_03", "s_m_m_scientist_01", "csb_screen_writer", "ig_screen_writer", "s_m_m_security_01", "s_f_y_sheriff_01", "s_m_y_sheriff_01", "mp_m_shopkeep_01", "cs_siemonyetarian", "ig_siemonyetarian", "a_f_y_skater_01", "a_m_m_skater_01", "a_m_y_skater_01", "a_m_y_skater_02", "a_f_m_skidrow_01", "a_m_m_skidrow_01", "s_m_m_snowcop_01", "cs_solomon", "ig_solomon", "a_f_m_soucent_01", "a_f_m_soucent_02", "a_m_m_socenlat_01", "a_m_m_soucent_01", "a_m_m_soucent_02", "a_m_m_soucent_03", "a_m_m_soucent_04", "a_f_m_soucentmc_01", "a_f_o_soucent_01", "a_f_o_soucent_02", "a_m_o_soucent_01", "a_m_o_soucent_02", "a_m_o_soucent_03", "a_f_y_soucent_01", "a_f_y_soucent_02", "a_f_y_soucent_03", "a_m_y_soucent_01", "a_m_y_soucent_02", "a_m_y_soucent_03", "a_m_y_soucent_04", "u_m_y_sbike", "u_m_m_spyactor", "u_f_y_spyactress", "u_m_y_staggrm_01", "cs_stevehains", "ig_stevehains", "s_m_m_strperf_01", "s_m_m_strpreach_01", "g_m_y_strpunk_01", "g_m_y_strpunk_02", "s_m_m_strvend_01", "s_m_y_strvend_01", "cs_stretch", "ig_stretch", "csb_stripper_01", "s_f_y_stripper_01", "csb_stripper_02", "s_f_y_stripper_02", "s_f_y_stripperlite", "mp_f_stripperlite", "a_m_y_sunbathe_01", "a_m_y_surfer_01", "s_m_y_swat_01", "s_f_m_sweatshop_01", "s_f_y_sweatshop_01", "ig_talina", "cs_tanisha", "ig_tanisha", "cs_taocheng", "ig_taocheng", "cs_taostranslator", "ig_taostranslator", "u_m_y_tattoo_01", "cs_tenniscoach", "ig_tenniscoach", "a_f_y_tennis_01", "a_m_m_tennis_01", "cs_terry", "ig_terry", "g_f_y_lost_01", "g_m_y_lost_01", "g_m_y_lost_02", "g_m_y_lost_03", "cs_tom", "csb_tonya", "ig_tonya", "a_f_y_topless_01", "a_f_m_tourist_01", "a_m_m_tourist_01", "a_f_y_tourist_01", "a_f_y_tourist_02", "cs_tracydisanto", "ig_tracydisanto", "csb_trafficwarden", "ig_trafficwarden", "a_f_m_tramp_01", "a_m_m_tramp_01", "u_m_o_tramp_01", "a_m_o_tramp_01", "s_m_m_gentransport", "a_m_m_tranvest_01", "a_m_m_tranvest_02", "s_m_m_trucker_01", "ig_tylerdix", "cs_paper", "ig_paper", "s_m_m_ups_01", "s_m_m_ups_02", "s_m_y_uscg_01", "g_f_y_vagos_01", "s_m_y_valet_01", "a_m_y_beachvesp_01", "a_m_y_beachvesp_02", "a_m_y_vindouche_01", "a_f_y_vinewood_01", "a_f_y_vinewood_02", "a_f_y_vinewood_03", "a_f_y_vinewood_04", "a_m_y_vinewood_01", "a_m_y_vinewood_02", "a_m_y_vinewood_03", "a_m_y_vinewood_04", "cs_wade", "ig_wade", "s_m_y_waiter_01", "cs_chengsr", "ig_chengsr", "a_m_y_stwhi_01", "a_m_y_stwhi_02", "s_m_y_winclean_01", "a_f_y_yoga_01", "a_m_y_yoga_01", "cs_zimbor", "ig_zimbor", "u_m_y_zombie_01" };

/***
* METHODS
*/

void reset_skin_globals()
{
	chosenSkinName = "";
}

bool is_custom_skin_applied()
{
	return skinchanger_used;
}

void set_custom_skin_applied(bool applied)
{
	skinchanger_used = applied;
}

/*
* ===============
* WORKER METHODS
* =================
*/

int findFirstValidPedTexture(int drawable)
{
	return 0;/*
	for (int j = 0; j < 100; j++)
	{
		if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, drawable, j))
		{
			return j;
		}
	}
	return -1;*/
}

int findFirstValidPedDrawable(int component)
{
	return 0;/*
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, i, j))
			{
				return i;
			}
		}
	}
	return -1;*/
}

bool applyChosenSkin(std::string skinName)
{
	DWORD model = GAMEPLAY::GET_HASH_KEY((char *)skinName.c_str());
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0);
		//STREAMING::LOAD_ALL_OBJECTS_NOW();
		PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
		//PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), FALSE);
		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
		WAIT(0);

		skinchanger_used = true;

		//reset the skin detail choice
		skinDetailMenuIndex = 0;
		skinDetailMenuValue = 0;

		skinDrawableMenuIndex = 0;
		skinDrawableMenuValue = -1;

		skinTextureMenuIndex = 0;
		skinTextureMenuValue = -1;

		WAIT(100);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

		chosenSkinName = skinName;

		return true;
	}
	return false;
}

std::string getSkinDetailAttribDescription(int i)
{
	switch (i)
	{
	case 0:
		return "Head/Face";
	case 1:
		return "Beard/Mask";
	case 2:
		return "Hair/Hat";
	case 3:
		return "Top";
	case 4:
		return "Legs";
	case 5:
		return "Accessory/Gloves";
	case 6:
		return "Accessory/Shoes";
	case 7:
	case 8:
	case 9:
		return "Accessory";
	case 10:
		return "Badges";
	case 11:
		return "Shirt/Jacket";
	default:
		return std::to_string(i);
	}
}

/*
* ===============
* TEXTURE MENU
* =================
*/

void onhighlight_skinchanger_texture_menu(int selection, std::string caption, int value)
{
	//TODO: remove these two
	skinTextureMenuIndex = selection;
	skinTextureMenuValue = value;

	if (true)//PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, value))
	{
		PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, value, 0);
	}
	skinchanger_used = true;
	WAIT(100);
}

bool onconfirm_skinchanger_texture_menu(int selection, std::string caption, int value)
{
	skinTextureMenuIndex = selection;
	skinTextureMenuValue = value;

	onhighlight_skinchanger_texture_menu(selection, caption, value);

	return true;
}

void onexit_skinchanger_texture_menu(bool returnValue)
{
	/*
	//restore the applied selection
	int texture;
	if (skinTextureMenuValue == -1)
	{
	texture = findFirstValidPedTexture(skinDrawableMenuValue);
	}
	else
	{
	texture = skinTextureMenuValue;
	}

	if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, texture))
	{
	PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, texture, 0);
	}
	*/
}

bool process_skinchanger_texture_menu(std::string caption)
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<std::string> menuCaptions;
	std::vector<int> menuValues;

	DWORD model = GAMEPLAY::GET_HASH_KEY((char *)chosenSkinName.c_str());
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0);

		int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue);
		for (int i = 0; i < textures; i++)
		{
			std::ostringstream ss;
			ss << "Texture #" << i;
			menuCaptions.push_back(ss.str());
			menuValues.push_back(i);
		}

		/*
		for (int j = 0; j < 100; j++)
		{
			int texture = j;
			if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, texture))
			{
				std::ostringstream ss;
				ss << "Texture #" << std::to_string(texture);

				menuCaptions.push_back(ss.str());
				menuValues.push_back(texture);
			}
		}
		*/

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::ostringstream ss;
	ss << "Available Textures";

	return draw_generic_menu<int>(menuCaptions, menuValues, skinTextureMenuIndex, ss.str(), onconfirm_skinchanger_texture_menu, onhighlight_skinchanger_texture_menu, onexit_skinchanger_texture_menu);
}

/*
* ===============
* SKIN DRAWABLES MENU
* =================
*/

bool onconfirm_skinchanger_drawable_menu(int selection, std::string caption, int value)
{
	skinDrawableMenuIndex = selection;
	skinDrawableMenuValue = value;

	skinTextureMenuIndex = 0;
	skinTextureMenuValue = -1;

	return process_skinchanger_texture_menu(caption);
}

void onhighlight_skinchanger_drawable_menu(int selection, std::string caption, int value)
{
	//TODO: remove these two
	skinDrawableMenuIndex = selection;
	skinDrawableMenuValue = value;

	int texture;
	if (value == skinDrawableMenuValue && skinTextureMenuValue != -1)
	{
		texture = skinTextureMenuValue;
	}
	else
	{
		texture = findFirstValidPedTexture(value);
	}
	if (true)//PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, value, texture))
	{
		PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, value, texture, 0);
	}
	skinchanger_used = true;
	WAIT(100);
}

void onexit_skinchanger_drawable_menu(bool returnValue)
{
	/*
	//restore the applied selection
	int drawable;
	if (skinDrawableMenuValue == -1)
	{
	drawable = findFirstValidPedDrawable(skinDetailMenuValue);
	}
	else
	{
	drawable = skinDrawableMenuValue;
	}

	//restore the applied selection
	int texture;
	if (skinTextureMenuValue == -1)
	{
	texture = findFirstValidPedTexture(skinDrawableMenuValue);
	}
	else
	{
	texture = skinTextureMenuValue;
	}

	if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, drawable, texture))
	{
	PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, drawable, texture, 0);
	}
	*/
}

bool process_skinchanger_drawable_menu(std::string caption, int component)
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<std::string> menuCaptions;
	std::vector<int> menuValues;

	DWORD model = GAMEPLAY::GET_HASH_KEY((char *)chosenSkinName.c_str());
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0);

		int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), component);
		for (int i = 0; i < drawables; i++)
		{
			int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), component, i);
			std::ostringstream ss;
			ss << "Drawable #" << i << " (" << textures << ")";
			menuCaptions.push_back(ss.str());
			menuValues.push_back(i);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::ostringstream ss;
	ss << "Available Drawables";

	return draw_generic_menu<int>(menuCaptions, menuValues, skinDrawableMenuIndex, ss.str(), onconfirm_skinchanger_drawable_menu, onhighlight_skinchanger_drawable_menu, onexit_skinchanger_drawable_menu);
}

/*
* ===============
* SKIN DETAIL MENU
* =================
*/

void onhighlight_skinchanger_detail_menu(int selection, std::string caption, int value)
{
	//do nothing
}

int lastTriedComponentIndex = 0;
int lastTriedPalette = 0;

bool onconfirm_skinchanger_detail_menu(int selection, std::string caption, int value)
{
	skinDetailMenuIndex = selection;
	skinDetailMenuValue = value;

	skinDrawableMenuIndex = 0;
	skinDrawableMenuValue = -1;

	skinTextureMenuIndex = 0;
	skinTextureMenuValue = -1;

	if (DEBUG_MODE_SKINS && value == -1)
	{
		DWORD playerModel = ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID());

		std::ostringstream ss1;
		for (int i = 0; i < 7; i++)
		{
			int d = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), i);
			int t = PED::GET_PED_TEXTURE_VARIATION(PLAYER::PLAYER_PED_ID(), i);
			ss1 << "D.T[" << i << "]=" << d << "." << t << "; ";
		}
		
		set_status_text(ss1.str(), 3000);

		DWORD time = GetTickCount() + 4000;
		while (GetTickCount() < time)
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		std::ostringstream ss2;
		for (int i = 7; i <= 12; i++)
		{
			int d = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), i);
			int t = PED::GET_PED_TEXTURE_VARIATION(PLAYER::PLAYER_PED_ID(), i);
			ss2 << "D.T[" << i << "]=" << d << "." << t << "; ";
		}

		set_status_text(ss2.str(), 3000);
		
		return false;
	}
	else if (DEBUG_MODE_SKINS && value == -2)
	{
		DWORD playerModel = ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID());

		std::ostringstream ss1;
		for (int i = 0; i < 7; i++)
		{
			int skins = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), i);
			ss1 << "D" << i << "=" << skins << "; ";
		}

		set_status_text(ss1.str(), 3000);

		DWORD time = GetTickCount() + 4000;
		while (GetTickCount() < time)
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		std::ostringstream ss2;
		for (int i = 7; i <= 12; i++)
		{
			int skins = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), i);
			ss2 << "D" << i << "=" << skins << "; ";
		}

		set_status_text(ss2.str(), 3000);

		return false;
	}
	else
	{
		return process_skinchanger_drawable_menu(caption, value);
	}
}

bool process_skinchanger_detail_menu()
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<std::string> menuCaptions;
	std::vector<int> menuValues;

	int fixedChoices = 0;
	const int partVariations = 12;

	int i = 0;

	if (DEBUG_MODE_SKINS)
	{
		menuCaptions.push_back("Output current D/T");
		menuValues.push_back(-1);
		menuCaptions.push_back("Output drawable slots");
		menuValues.push_back(-2);
		fixedChoices += 2;
	}

	if (skinchanger_used)
	{
		DWORD model = GAMEPLAY::GET_HASH_KEY((char *)chosenSkinName.c_str());
		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
		{
			STREAMING::REQUEST_MODEL(model);
			while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0);

			for (; i < partVariations + fixedChoices; i++)
			{
				bool iFound = false;
				int compIndex = i - fixedChoices;

				int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), compIndex);
				int textures = 0;
				if (drawables == 1)
				{
					textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), compIndex, 0);
				}
				if ( drawables > 1 || textures != 0 )
				{
					std::ostringstream ss;
					std::string itemText = getSkinDetailAttribDescription(compIndex);
					ss << "Slot " << compIndex << ": " << itemText << " (" << drawables << ")";
					menuCaptions.push_back(ss.str());
					menuValues.push_back(compIndex);
				}
			}

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		}
	}
	else
	{
		set_status_text("Please apply a custom skin in order to access this");
		return false;
	}

	return draw_generic_menu<int>(menuCaptions, menuValues, skinDetailMenuIndex, "Skin Details", onconfirm_skinchanger_detail_menu, onhighlight_skinchanger_detail_menu, NULL);
}

/*
* ===============
* SKIN PLAYER CHOICES
* =================
*/

bool onconfirm_skinchanger_choices_players(int selection, std::string caption, std::string value)
{
	skinTypesMenuPositionMemory[0] = selection;
	applyChosenSkin(value);
	return false;
}

bool process_skinchanger_choices_players()
{
	return draw_generic_menu<std::string>(SKINS_PLAYER_CAPTIONS, SKINS_PLAYER_VALUES, skinTypesMenuPositionMemory[0], "Player Skins", onconfirm_skinchanger_choices_players, NULL, NULL);
}

/*
* ===============
* SKIN ANIMAL CHOICES
* =================
*/

bool onconfirm_skinchanger_choices_animals(int selection, std::string caption, std::string value)
{
	skinTypesMenuPositionMemory[1] = selection;
	applyChosenSkin(value);
	return false;
}

bool process_skinchanger_choices_animals()
{
	return draw_generic_menu<std::string>(SKINS_ANIMAL_CAPTIONS, SKINS_ANIMAL_VALUES, skinTypesMenuPositionMemory[1], "Animal Skins", onconfirm_skinchanger_choices_animals, NULL, NULL);
}

/*
* ===============
* SKIN GENERAL CHOICES
* =================
*/

bool onconfirm_skinchanger_choices_misc(int selection, std::string caption, std::string value)
{
	skinTypesMenuPositionMemory[2] = selection;
	applyChosenSkin(value);
	return false;
}

bool process_skinchanger_choices_misc()
{
	return draw_generic_menu<std::string>(SKINS_GENERAL_CAPTIONS, SKINS_GENERAL_VALUES, skinTypesMenuPositionMemory[2], "General Skins", onconfirm_skinchanger_choices_misc, NULL, NULL);
}

/*
* ===============
* SKIN MAIN MENU
* =================
*/

bool onconfirm_skinchanger_menu(int selection, std::string caption, int value)
{
	skinMainMenuPosition = selection;

	switch (value)
	{
	case 0: //Players
		process_skinchanger_choices_players();
		break;
	case 1: //Animals
		process_skinchanger_choices_animals();
		break;
	case 2: //Misc
		process_skinchanger_choices_misc();
		break;
	case 3: //Detail
		process_skinchanger_detail_menu();
		break;
	case 4: //Reset
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
		set_status_text("Using default model skin");
		break;
	}
	return false;
}

bool process_skinchanger_menu()
{
	std::vector<int> menuIDs;
	for (int i = 0; i < MENU_SKINS_TYPES_CAPTIONS.size(); i++)
	{
		menuIDs.push_back(i);
	}

	return draw_generic_menu<int>(MENU_SKINS_TYPES_CAPTIONS, menuIDs, skinMainMenuPosition, "Player Skin Options", onconfirm_skinchanger_menu, NULL, NULL);
}