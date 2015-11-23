//---------------------------------------------------------------------------
#ifndef DUINOTAGV2_H
#define DUINOTAGV2_H
//---------------------------------------------------------------------------

#include <Custom IR\IRremote.h>

#include <ToneAC\toneAC.h>

#include <Neopixel\Adafruit_NeoPixel.h>

#include <FlexiTimer\FlexiTimer2.h>

#include <GameMode.h>

#define myByte unsigned int

#define sPacketLength 29 
const int frequency = 56;
#define IR_BURST_UPPER 60 //numbers to turn raw lengths of IR into signal, probably don't mess with them
#define IR_BURST_HEADER 40
#define IR_BURST_ONE 20
#define IR_BURST_ZERO 8


/*--MARIMOLE-DEF_BEGIN--*/
void loop();
void setup();
class Arduino;
class Bitshift;
class Suit;
class Stats;
class Lasergun;
class IRrecv;
class IRsend;
double MHitDelay(myByte in);
int milesHealth(myByte health);
int milesRPM(myByte rpm);
int milesDamage(myByte damageIn);
int decodePulse(int pulseLength);
int boolToInt(bool input[8]);
void intToBool(unsigned int input, unsigned int start, unsigned int len, char * ray);
/*--MARIMOLE-DEF_END--*/
//sorry, but it has to be done...
class Bitshift{
		public:
		unsigned int store;
		bool grab(unsigned int place);
		Bitshift& operator= (const unsigned int &x);
		void flip(unsigned int place, bool value);
		Bitshift();
};
// :(

struct packet{
	Bitshift data1;  //1 bit for packet type, 7 bits for playerID 
	Bitshift data2;  //6 or 8 bits for team and damage or message data
};

enum arduinoLights {
	pLightsHit,
	pLightsDead,
	pLightsGameOn,
	pLightsGameOver,
	null
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
//const lightControl shoot[15] = {playPew}; not used after playPew function implemented
const lightControl hit[15] = {hitOn,playHit,hitOff,over}; //set delay to 250 or something
const lightControl gameOn[15] = {allOn,playGameOn,muzzleOff,Tdelay,hitOff,Tdelay,rightOff,leftOff,over};
const lightControl dead[15] = {hitOn,playDead,hitOff,over};
const lightControl gameOver[15] ={allOn,Tdelay,allOff,Tdelay,allOn,Tdelay,allOff,over};
	//is this cool or what?!

enum SuitCommmands{  //also includes message packet commands
		cShot, //take damage
		//message packet commands
		cAddHealth,
		cAddAmmo,
		cKill,//0x00
		cPause,//0x01
		cStartGame,//0x02
		cDefaults,//etc.//Value Team name 0x00 Red,0x01 Blue,0x02 Yellow,0x03 Green
		cRespawn,
		cINewGame,//Immediate new game
		cFullAmmo,
		cEndGame,
		cResetClock, //not implemented
		cExplodePlayer,
		cReadyUp, //ready up for new game, not implemented
		cFullHealth,
		cFullArmor,
		cClearScores,
		cTestSensors,
		cStun,
		cDisarm,
		cNull
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
		SuitCommmands whatToDo;
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
		bool playLights(arduinoLights command);
		void pause();
		void reset();
		void setup(int maxHealth, int maxAmmo, int maxArmor, myByte team); //initilizes pins, thats all
		bool update(); //used to not delay entire program if playing leds, just update when ready and change action
		void changeValues(double aHealth, double aAmmo, double aArmor);
		void playPew(); //pew needs to behave differently than other sounds, so yeah
		void playIdle();
		Arduino();
		private:
		arduinoLights commandBuffer[5];
		bool lightCommand(const lightControl steps[15]);
		int currentStep;
		bool idle;
		bool paused;
		bool delaying;
		bool pewOverride;
		bool newHealth;
		bool newAmmo;
		int soundDelay;
		long long int lastTime;
		long int lastPewTime;
		double aMaxHealth;
		double healthDisp;
		double aMaxAmmo;
		double ammoDisp;
		double aMaxArmor;
		double armorDisp;
		
		Adafruit_NeoPixel neopix;  //lights handled by neopixel library, these are indicator
		Adafruit_NeoPixel left;  //these are left side team
		Adafruit_NeoPixel right;
	};
	//gonna use namespace instead of class b/c of library
	//and really, sounds are global anyway
	//maybe I should make arduino a namespace...
	//f that
	namespace Sounds{  //updating lights semi-consistintly works, but it doesn't for sound.  This namespace will handle it all in an ISR which will be called every 100us or so
		//is will also be pretty much handled by Arduino
		struct soundProp {
			bool escalating; //playing sound up or down
			unsigned int start;
			unsigned int end;
			unsigned int interval; //in microseconds
			unsigned int jump; //in hz
		};

		//below are sound props. can be edited freely.
		//the sound is played by escalating or deescalating from start freq. to end freq. with a buzzer
		const soundProp pPew = { false, 1600, 400, 1000, 50 }; //ex. starts a 1600hz, goes down 50hz every 1000us until 400hz
		const soundProp pHit = { false, 750, 200, 100, 10 };
		const soundProp pStart = { true, 50, 2000, 10, 10 };
		const soundProp pDead = { false, 1000, 10, 10, 1 };
		//yay!

		extern void playSound(const soundProp sound);
		extern void updateSound(void); //called by timer 3
		extern volatile void reset();
		extern void pause();
		extern volatile int currentFreq;
		extern volatile bool playingSound;
		extern volatile bool paused;
		extern volatile soundProp currentSound;
	}
#if USE_STATS == true
	class Stats{
		public:
		int	calculate(statCommand command);
		void returnHits(int * ray);
		void addValue(statCommand command, int input);
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
		int armor; 
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
		void sCommand(SuitCommmands command, int amount);
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
		bool gunCommand(GunCommands, int amount);
#if CUSTOM_WEAPONS == true
		void switchGun(gunProfile newGun);
#endif
		Arduino display;
		IRrecv * recv;
		bool checkStatus();
		unsigned int rpmDelay; //initialized in setup
		gunProfile currentProfile;
		gunProfile gunValues;
		int currentClip;
		int currentAmmo;
		private:
		void setUpPacket(); //called in setup
		long int currentDelay;
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
#if ON_OBJECTIVE_START== true
		void onObjStart(int progress);
#endif
#if ON_OBJECTIVE_FINISH == true
		void onObjFinish(int progress);
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