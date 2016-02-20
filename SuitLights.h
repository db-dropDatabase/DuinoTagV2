#ifndef SUITLIGHTS_H
#define SUITLIGHTS_H

#include <GameMode.h>

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
const lightControl pLightsHit[] = { hitOn,Tdelay,hitOff,over }; //set delay to 250 or something
const lightControl pLightsGameOn[] = { allOn,Tdelay,muzzleOff,Tdelay,hitOff,Tdelay,over };
const lightControl pLightsDead[] = { hitOn,Tdelay,hitOff,over };
const lightControl pLightsGameOver[] = { allOn,Tdelay,allOff,Tdelay,allOn,Tdelay,allOff,over };
const lightControl pNull[] = { over };
//is this cool or what?!

class SuitLights { //for playing sounds and lights
public:
	//functions
	bool playLights(const lightControl * command);
	void pause();
	void reset();
	void setup(myByte iTeam); //initilizes pins, thats all
	bool update(); //used to not delay entire program if playing leds, just update when ready and change action
	void playIdle();
	SuitLights();
private:
	const lightControl * commandBuffer[5];
	bool lightCommand(const lightControl steps[15]);
	unsigned int currentStep;
	bool idle;
	bool paused;
	unsigned long int lastTime;
	unsigned char team;
};



#endif