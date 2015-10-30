#ifndef SOUND_H
#define SOUND_H

#include <FlexiTimer\FlexiTimer2.h>

#include <ToneAC\toneAC.h>

//gonna use namespace instead of class b/c of library
//and really, sounds are global anyway
//maybe I should make arduino a namespace...
//f that
namespace Sounds {  //updating lights semi-consistintly works, but it doesn't for sound.  This namespace will handle it all in an ISR which will be called every 100us or so
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
	extern volatile unsigned int currentFreq;
	extern volatile bool playingSound;
	extern volatile bool paused;
	extern volatile soundProp currentSound;
}



#endif
