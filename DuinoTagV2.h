//---------------------------------------------------------------------------
#ifndef DUINOTAGV2_H
#define DUINOTAGV2_H
//---------------------------------------------------------------------------

#include <Custom IR\IRremote.h>

#include <Neopixel\Adafruit_NeoPixel.h>

#include <GameMode.h>

#include <Sound.h>

#include <MilesDecode.h>

#include <Bitshift.h>

#include <LaserWiFi.h>

#define myByte unsigned int

#define sPacketLength 29 
const unsigned int frequency = 56;

/*--MARIMOLE-DEF_BEGIN--*/
void loop();
void setup();
class Arduino;
class Suit;
class Stats;
class IRrecv;
class IRsend;
/*--MARIMOLE-DEF_END--*/

struct packet{
	Bitshift data1;  //1 bit for packet type, 7 bits for playerID 
	Bitshift data2;  //6 or 8 bits for team and damage or message data
};

enum lightControl{
	muzzleOn,
	muzzleOff,
	leftOn,
	leftOff,
	rightOn,
	rightOff,
	hitOn,
	hitOff,
	allOn,
	allOff,
	playHit,
	playGameOn,
	playDead,
	Tdelay,
	over
};

//below are command lists, and can be edited freely, use the commands above
//const lightControl shoot[] = {playPew}; not used after playPew function implemented
const lightControl pLightsHit[] = {hitOn,playHit,hitOff,over}; //set delay to 250 or something
const lightControl pLightsGameOn[] = {allOn,playGameOn,muzzleOff,Tdelay,hitOff,Tdelay,over};
const lightControl pLightsDead[] = {hitOn,playDead,hitOff,over};
const lightControl pLightsGameOver[] = {allOn,Tdelay,allOff,Tdelay,allOn,Tdelay,allOff,over};
const lightControl pNull[] = {over};
	//is this cool or what?!

	enum SuitCommmands {  //also includes message packet commands
		cShot, //take damage
		//message packet commands
		cAddHealth = 128,
		cAddAmmo = 129,
		cSpecialCommand = 131,
		cKill = 0,//0x00
		cPause = 1,//0x01
		cStartGame = 2,//0x02
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
	
	struct parsedPacket{
		unsigned int whatToDo;
		unsigned int amount; //for add ammo or health
	};
	
	struct gunProfile{
		myByte damage;
		myByte rpm;
		myByte reload;
		myByte clipSize;
		myByte clipNum;
	};
	
	class Arduino{ //for playing sounds and lights
		public:
		//functions
		bool playLights(const lightControl * command);
		void pause();
		void reset();
		void setup(unsigned int maxHealth,unsigned int maxAmmo,unsigned int maxArmor, myByte iTeam); //initilizes pins, thats all
		bool update(); //used to not delay entire program if playing leds, just update when ready and change action
		void changeValues(double aHealth, double aAmmo, double aArmor);
		void playPew(); //pew needs to behave differently than other sounds, so yeah
		void playIdle();
		Arduino();
		private:
		const lightControl * commandBuffer[5];
		bool lightCommand(const lightControl steps[15]);
		unsigned int currentStep;
		bool idle;
		bool paused;
		bool delaying;
		bool pewOverride;
		bool newHealth;
		bool newAmmo;
		unsigned int soundDelay;
		unsigned long long int lastTime;
		unsigned long int lastPewTime;
		double aMaxHealth;
		double healthDisp;
		double aMaxAmmo;
		double ammoDisp;
		double aMaxArmor;
		double armorDisp;
		uint8_t team;

		Adafruit_NeoPixel neopix;  //lights handled by neopixel library, these are indicator
		Adafruit_NeoPixel left;  //these are left side team
		Adafruit_NeoPixel right;

		const uint32_t teamColors[4] = { neopix.Color(55,128,255), neopix.Color(255,0,0), neopix.Color(255,0,95), neopix.Color(255,128,0) }; //team colors, in a nice array to make things easier
	};

#if USE_STATS == true
	class Stats{
		public:
		unsigned int	calculate(statCommand command);
		void returnHits(unsigned int * ray);
		void addValue(statCommand command,unsigned  int input);
		void reset();
		Stats();
		private:
		uint_least8_t hitCount[127]; //I got hit x times by y!
		unsigned int deathCount;
		unsigned int shotCount;
		unsigned int reloadCount;
	};
#endif

	class Suit{
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
		Suit();
		void setup(myByte iTeamID, myByte iPlayerID, IRrecv * showMe); //sets everything to defaults for a quick game, except for shown varibles
		//will add score and cloning packets later
		bool action(packet packetYay); //put in packet, out goes lasers!
		bool sCommand(unsigned int command, unsigned int amount);
#if IR_SETUP==1
		void waitForSetup(IRrecv * showMe); //damn you FTC
#endif
#if USE_STATS == true
		Stats stat;
#endif
		
		//test merging of classes
		//gun stuff
		//functions or variables not included in table
		unsigned int currentReload;
		unsigned int shotPacket[30]; //for IRsend library, initilized in setuppacket
		bool gunCommand(GunCommands command, unsigned int amount);
#if CUSTOM_WEAPONS == true
		void switchGun(gunProfile newGun);
#endif
		Arduino display;
		IRrecv * recv;
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
		IRsend irsend;

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
	
	#endif