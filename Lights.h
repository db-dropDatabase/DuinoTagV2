#ifndef LIGHTS_H
#define LIGHTS_H

#include <Neopixel\Adafruit_NeoPixel.h>

#include <Sound.h>

#include <GameMode.h>

/*
enum lightControl {
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
const lightControl pLightsHit[] = { hitOn,playHit,hitOff,over }; //set delay to 250 or something
const lightControl pLightsGameOn[] = { allOn,playGameOn,muzzleOff,Tdelay,hitOff,Tdelay,over };
const lightControl pLightsDead[] = { hitOn,playDead,hitOff,over };
const lightControl pLightsGameOver[] = { allOn,Tdelay,allOff,Tdelay,allOn,Tdelay,allOff,over };
const lightControl pNull[] = { over };
//is this cool or what?!
*/
/*
class Arduino { //for playing sounds and lights
public:
	//functions
	bool playLights(const lightControl * command);
	void pause();
	void reset();
	void setup(unsigned int maxHealth, unsigned int maxAmmo, unsigned int maxArmor, myByte iTeam); //initilizes pins, thats all
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
	unsigned char team;

	Adafruit_NeoPixel neopix;  //lights handled by neopixel library, these are indicator
	Adafruit_NeoPixel left;  //these are left side team
	Adafruit_NeoPixel right;

	const uint32_t teamColors[4] = { neopix.Color(55,128,255), neopix.Color(255,0,0), neopix.Color(255,0,95), neopix.Color(255,128,0) }; //team colors, in a nice array to make things easier
};
*/

#endif //guard
