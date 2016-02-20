//---------------------------------------------------------------------------
#ifndef DUINOTAGV2_H
#define DUINOTAGV2_H
//---------------------------------------------------------------------------
/*
#include <Custom IR\IRremote.h>

#include <GameMode.h>

#include <MilesDecode.h>

#include <MilesIR.h>

#include <Bitshift.h>

#include <LaserWiFi.h>

#include <Lights.h>

#include <LaserCom.h>

void loop();
void setup();
class Suit;
class Stats;
class IRrecv;
class IRsend;

	enum SuitCommandsOld {  //also includes message packet commands
		cShot, //take damage
		//message packet commands
		cAddHealth = 128,
		cAddAmmo = 129,
		cSpecialCommand = 131,
		cKill = 0,//0x00
		cPause = 1,//0x01
		//cStartGame = 2,//0x02
		cDefaults = 3,//etc.
		cRespawn = 4,
		cINewGame = 5,//Immediate new game
		cFullAmmo = 6,
		cEndGame = 7,
		cResetClock = 8, //not implemented
		cExplodePlayer = 11,
		cReadyUp = 12, //ready up for new game, not implemented
		cFullHealth = 13,
		cFullArmor = 15,
		cClearScores = 20,
		cTestSensors = 21,
		cStun = 22,
		cDisarm = 23,
		cAmmoPick = 136,
		cHealthPick = 137,
		cObj = 138,
		cNull = 0
	};
	enum GunCommands{
		gStop,
		gShoot,
		gReload,
		gOverheat,
		gAddAmmo,
		gFullAmmo,
		gTest
	};
	enum statCommand{
		sHit,
		sShot,
		sReload,
		sDeath
	};
	
	
	struct gunProfile{
		myByte damage;
		myByte rpm;
		myByte reload;
		myByte clipSize;
		myByte clipNum;
	};

	class OldSuit{
		public:
		//in order as shown in table, but only if pertains to suits
		myByte playerID;
		myByte teamID;
		myByte startHealth; //value reference in code.cpp
		myByte respawnTime; //in seconds
		unsigned int armor; 
		//bytes below are all part of a gamsettings thing, so I split them up, 0x00 is false and 0x01 is true
		//part 2
		bool respawns;  //enable respawns
		//normal bytes
		//values and functions not included in table
		int currentHealth;
		int currentArmor;
		unsigned int numRespawns; //if respawn limit is set
		bool isDead;
		OldSuit();
		void setup(myByte iTeamID, myByte iPlayerID); //sets everything to defaults for a quick game, except for shown varibles
		//will add score and cloning packets later
		bool action(packet packetYay); //put in packet, out goes lasers!
		bool sCommand(unsigned int command, unsigned int amount);
#if IR_SETUP==1
		void waitForSetup(IRrecv * showMe); //damn you FTC
#endif
#if USE_STATS == true
		//Stats stat;
#endif
		
		//test merging of classes
		//gun stuff
		//functions or variables not included in table
		unsigned int currentReload;
		unsigned int shotPacket[sPacketLength]; //for IRsend library, initilized in setuppacket
		bool gunCommand(GunCommands command, unsigned int amount);
#if CUSTOM_WEAPONS == true
		void switchGun(gunProfile newGun);
#endif
		Arduino display;
		bool checkStatus();
		private:
		unsigned int rpmDelay; //initialized in setup
		gunProfile currentProfile;
		gunProfile gunValues;
		unsigned int currentClip;
		unsigned int currentAmmo;
		void setUpPacket(); //called in setup
		signed long int currentDelay;
		unsigned long int tmpTime;
		
		parsedPacket readPacket(packet packetYay);
		MilesIR ir;

		//custom gamemode funcs.
#if ON_SHOOT == true
		void onShoot();
#endif
#if ON_HIT == true
		void onHit();
#endif
#if ON_RELOAD_START == true
		void onReloadStart();
#endif
#if ON_RELOAD_END == true
		void onReloadEnd();
#endif
#if ON_DEATH == true
		void onDeath();
#endif
#if ON_RESPAWN == true
		void onRespawn();
#endif
#if ON_OBJECTIVE == true
		void onObj(unsigned int progress);
#endif
#if ON_GAME_START == true
		void onGameStart();
#endif
#if ON_GAME_END == true
		void onGameEnd();
#endif
#if ON_CUSTOM_EVENT == true
		void onCustomEvent();
#endif
	};
#ifdef NOTHING //just saving this for later
	class Stats {
	public:
		unsigned int	calculate(statCommand command);
		void returnHits(unsigned int * ray);
		void addValue(statCommand command, unsigned  int input);
		void reset();
		Stats();
	private:
		uint_least8_t hitCount[127]; //I got hit x times by y!
		unsigned int deathCount;
		unsigned int shotCount;
		unsigned int reloadCount;
#endif
*/
#endif