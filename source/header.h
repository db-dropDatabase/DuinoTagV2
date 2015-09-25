//---------------------------------------------------------------------------
#ifndef _MARIAMOLE_AUTOMATIC_CODE_H
#define _MARIAMOLE_AUTOMATIC_CODE_H
//---------------------------------------------------------------------------
#include <Arduino.h>

#include "IRremote.h"

#include "toneAC.h"

#include "Adafruit_NeoPixel.h"

#define myByte unsigned int

#define constDelay 1000
#define sPacketLength 29
const int frequency = 56;
#define sendPin 3 //never used in code, but still important
#define recievePin A4
#define muzzlePin 2 //light pins below
#define leftPin 5
#define rightPin 6
#define hitPin A0
#define buzzerPin 10 //other pins, with buzzer both 9 and 10 are taken
#define triggerPin 12
#define neoPin 11

using namespace std;

/*--MARIMOLE-DEF_BEGIN--*/
void loop();
void setup();
//bool Suit::command(SuitCommmands command, int amount);
//bool Lasergun::setupGun(myByte iPlayerID, myByte currentTeam, myByte iClipNum, myByte iClipSize);
//bool Suit::setup(myByte iTeamID, myByte iPlayerID, myByte iClipNum, myByte iClipSize, Lasergun *gun);
//void Arduino::pause(bool reset);
//bool Arduino::lightCommand(const lightControl steps[15]);
//bool Arduino::checkTrigger();
//bool Arduino::playLights(arduinoLights command);
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
bool initPin(int pinNum);
int decodePulse(int pulseLength);
int boolToInt(bool input[8]);
void intToBool(unsigned int input, unsigned int start, unsigned int len, char * ray);
/*--MARIMOLE-DEF_END--*/
//sorry, but it has to be done...
class Bitshift{
		public:
		unsigned int store;
		bool& operator[] (unsigned int x);
		Bitshift& operator= (const unsigned int &x);
		void flip(unsigned int place, bool value);
};
// :(

typedef struct {
	Bitshift data1;//1 bit for packet type, 7 bits for playerID 
	Bitshift data2;  //6 or 8 bits for team and damage or message data
}packet;

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
	
	typedef struct {
		SuitCommmands whatToDo;
		unsigned int amount; //for add ammo or health
	}parsedPacket;
	
	struct gunProfile{
		myByte damage;
		myByte rpm;
		myByte reload;
		myByte clipSize;
	};
	
	const gunProfile machineGun = {3, 7, 4, 50}; //conforms to the f ing milestag protocol
	const gunProfile pistol = {5, 0, 2, 8}; //technicly should be semi-auto, but can't add that here
	const gunProfile sniper = {12, 0, 5, 4};
	const gunProfile shotgun = {6, 0, 4, 8};
	
	class Arduino{ //for playing sounds and lights
		public:
		//functions
		bool playLights(arduinoLights command);
		void pause();
		void reset();
		bool setup(int maxHealth, int maxAmmo, int maxArmor, myByte team); //initilizes pins, thats all
		bool update(); //used to not delay entire program if playing leds, just update when ready and change action
		void changeValues(double aHealth, double aAmmo, double aArmor);
		void playPew(); //pew needs to behave differently than other sounds, so yeah
		void playIdle();
		private:
		arduinoLights commandBuffer[5];
		bool lightCommand(const lightControl steps[15]);
		int currentStep;
		bool idle;
		bool paused;
		bool delaying;
		bool pewCommand;
		int currentPew;
		bool pewOverride;
		bool newHealth;
		bool newAmmo;
		int soundDelay;
		long long int lastTime;
		double aMaxHealth;
		double healthDisp;
		double aMaxAmmo;
		double ammoDisp;
		double aMaxArmor;
		double armorDisp;
		
		Adafruit_NeoPixel neopix;
		Adafruit_NeoPixel left;
		Adafruit_NeoPixel right;
	};
	
	class Stats{
		public:
		int	calculate(statCommand command);
		void returnHits(int * ray);
		void addValue(statCommand command, int input);
		void reset();
		private:
		uint_least8_t hitCount[127]; //I got hit x times by x!
		unsigned int deathCount;
		unsigned int shotCount;
		unsigned int reloadCount;
	};
	
	class Suit{
		public:
		//in order as shown in table, but only if pertains to suits
		myByte teamID;
		myByte hitLEDTimeout; //seconds
		myByte startHealth; //value reference in code.cpp
		myByte respawnTime; //in tens of seconds, not used
		int armor; 
		//bytes below are all part of a gamsettings thing, so I split them up, 0x00 is false and 0x01 is true
		bool friendlyFire;
		bool zombieMode;
		bool medic;
		//part 2
		bool respawns;  //enable respawns
		bool gamemode; //enable gamemode in general
		//normal bytes
		myByte hitDelay;//value reference in code.cpp
		myByte startDelay; //seconds
		myByte deathDelay; //seconds
		myByte timeLimit; //minutes
		myByte maxRespawn;
		//values and functions not included in table
		bool respawnLimit; //enables or disables respawn limit
		myByte playerID;
		int currentHealth;
		int currentArmor;
		unsigned int numRespawns; //if respawn limit is set
		unsigned int respawnTimeLeft;
		bool isDead;
		void setup(myByte iTeamID, myByte iPlayerID, myByte iDamage, IRrecv * showMe); //sets everything to defaults for a quick game, except for shown varibles
		//will add score and cloning packets later
		bool action(packet packetYay); //put in packet, out goes lasers!
		void sCommand(SuitCommmands command, int amount);
		void waitForSetup(IRrecv * showMe); //damn you FTC
		
		Stats stat;
		
		//test merging of classes
		//gun stuff
		bool overheat;  //if infinite clips, after awhile the gun will stop shooting
		myByte damage; //function above converts int to damage
		myByte clipNum; //0xCA for unlimited
		myByte fireType;  //0x00 semi auto, 0x01 burst, 0x02 full auto
		myByte burstRounds; //num of rounds in burst mode
		bool IRPower; //0x00 low, 0x01 high :/
		//myByte IRRange; //not entirely sure how to regulate this, so unused for now
		myByte gunSettings; //0x01 muzzle flash on, 0x02 Overheat on, 0x03 for both, not implemented
		//bytes below are all part of a gamsettings thing, so I split them up, 0x00 is false and 0x01 is true
		//custom, the way they did this in the protocol is weird
		//bytes below are in the part 2 of the gamesettings thing
		bool ammoReset;  //restock ammo on spawn
		//functions or variables not included in table
		unsigned int currentAmmo;
		unsigned int currentClip;
		unsigned int currentReload;
		unsigned int shotPacket[30]; //for IRsend library, initilized in setuppacket
		bool gunCommand(GunCommands, int amount);
		void switchGun(gunProfile newGun);
		Arduino display;
		IRrecv * recv;
		bool checkStatus();
		unsigned int rpmDelay; //initialized in setup
		gunProfile currentProfile;
		private:
		void setUpPacket(); //called in setup
		long int currentDelay;
		unsigned long int tmpTime;
		int lastDamage;
		gunProfile gunValues;
		
		parsedPacket readPacket(packet packetYay);
		IRsend irsend;
	};
	
	#endif