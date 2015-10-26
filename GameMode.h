#ifndef GAMEMODE_H
#define GAMEMODE_H

//Welcome to the game mode settings file!
//This file should serve as a dynamic properties file for custom gamemodes
//Eventually, I would like to add a gui for doing all of this, but text is fine for now



//pin configuration
#define constDelay 250 //make longer to make lights flash for longer,and vice versa
#define sendPin 5 //never used in code, but still important
#define recievePin 15 //IR reciever pin
#define muzzlePin A1 //muzzle flash
#define leftPin 6 //left team neopixels
#define rightPin 7 //right team neopixels
#define hitPin A3 //hit LED
#define buzzerPin 9 //other pins, with buzzer both 9 and 10 are taken
#define triggerPin A2 //duh
#define neoPin 4 //Indicator neopixel pin

//code config
#define USE_STATS false //stats machine, but no way to report stats currently
#define DEBUG
//#define VERBOSE_DEBUG

//game config
#define IR_SETUP 0  //setting up over IR or with default values in gun

//default values, will be overriden if IR setup is in place
#define TEAM 0 //0-3
#define PLAYER_ID 0 //7 bit player ID, max 127
#define HEALTH 0x24 //there is a weird table of values this has to conform to, so this would technicly be 100
#define ARMOR 50

//gun values
#define DAMAGE 3 //another not normal number, check out milesDamage()
#define RPM 7 //oh look another one! milesRPM()
#define RELOAD 4 //seconds, thank god
#define CLIP_SIZE 50 //normal also, 0xFF for unlimited
#define CLIP_NUM 0xCA //0xCA for unlimited


#define RESPAWN_TIME 3 //seconds
#define FRIENDLY_FIRE false //I suggest that this is never turned on, simply because it's really not fun with it on
#define MEDIC_ALLOWED true 
#define HIT_DELAY 0 //also strange values, refer to mhitdelay() or just leave zero 
#define START_DELAY 0 //seconds
#define DEATH_DELAY 0 //seconds
#define RESPAWN_ALLOWED true
#define MAX_RESPAWN 0 //zero for no limit
#define AMMO_RESTOCK true //restock ammo on spawn


//#define OVERHEAT false //would be used if infinite clips, not implemented
//#define FIRE_TYPE 2 //0 semi auto, 1 burst (3), 2 full auto, not implemented
//#define GUN_SETTING 1 //this property is so weird, I don't think I even understand it.  not implemented
//#define TIME_LIMIT 0 //minuets, not implemented
//#define GAMEMODE_ALLOWED true //this is directly from the protocol, but I don't nkow what they mean by gamemode, so not implemented
//#define ZOMBIE_MODE false //not implemented yet
//#define BURST_ROUNDS 3 //number of rounds in burst fire mode, not implemented

//below is the main game loop for the gun.
void gunSetup();
void gunLoop();

//gun configuration below, only used if CUSTOM_WEAPONS is true
//otherwise the gun values above are used
#define CUSTOM_WEAPONS false

#if CUSTOM_WEAPONS == true
//damage, rpm, reload, clipsize, clipnum
//damage and rpm are special numbers, and can be reverse engeneered from milesDamage() and milesRpm()
const gunProfile machineGun = { 3, 7, 4, 50, 0xCA }; //conforms to the f ing milestag protocol
const gunProfile pistol = { 5, 0, 2, 8, 0xCA }; //technicly should be semi-auto, but can't add that here
const gunProfile sniper = { 12, 0, 5, 4, 0xCA };
const gunProfile shotgun = { 6, 0, 4, 8, 0xCA };

#define DEFUALT_GUN machineGun
#endif

//below are functions that control what the gun does when game events happen
//These can be toggled by the defines below
#define ON_SHOOT false
#define ON_HIT false
#define ON_RELOAD_START false
#define ON_RELOAD_END false
#define ON_DEATH false
#define ON_RESPAWN false
#define ON_OBJECTIVE_START false //flag, bomb, whatever.  all the same trigger b/c it's easier that way
#define ON_OBJECTIVE_FINISH false
#define ON_GAME_START false
#define ON_GAME_END true
#define ON_CUSTOM_EVENT false //very complicated, but will try to implement
#define CUSTOM_EVENT cAddHealth //use a suit command, which are enumerated in duinotagv2.h

//do not change below values
#if MAX_RESPAWN != 0 && RESPAWN_ALLOWED
	#define RESPAWN_LIMITED true
#else
	#define RESPAWN_LIMITED false
#endif

#endif