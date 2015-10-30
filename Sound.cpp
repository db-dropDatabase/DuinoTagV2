#ifndef SOUND_CPP
#define SOUND_CPP

#include <Sound.h>

//Sounds varibles that have to be declared
volatile bool Sounds::playingSound;
volatile int unsigned Sounds::currentFreq;
volatile bool Sounds::paused;
volatile Sounds::soundProp Sounds::currentSound;

volatile void Sounds::reset() {
	currentFreq = 0;
	currentSound.escalating = false;
	currentSound.start = 0;
	currentSound.end = 0;
	currentSound.interval = 0;
	currentSound.jump = 0;
	playingSound = false;
	paused = false;
	FlexiTimer2::stop();
	noToneAC();
}

//simple enough

void Sounds::pause() {
	if (!paused) {
		currentFreq = 0;
		currentSound.escalating = false;
		currentSound.start = 0;
		currentSound.end = 0;
		currentSound.interval = 0;
		currentSound.jump = 0;
		playingSound = false;
		FlexiTimer2::stop();
		noToneAC();
	}
	paused = !paused;
}

void Sounds::playSound(const soundProp sound) {
	if (!paused) {
		currentSound.escalating = sound.escalating;
		currentSound.start = sound.start;
		currentSound.end = sound.end;
		currentSound.interval = sound.interval;
		currentSound.jump = sound.jump;
#ifdef DEBUG
		Serial.println("Playing Sound!");
		Serial.println(currentSound.start);
#endif
		currentFreq = currentSound.start;
		FlexiTimer2::set(1, (float)(currentSound.interval / 1000000), updateSound);
		FlexiTimer2::start();
	}
	playingSound = true;
}

void Sounds::updateSound(void) { //technicly an ISR
	toneAC(currentFreq);
	if (!currentSound.escalating) {
		currentFreq -= currentSound.jump;
	}
	else {
		currentFreq += currentSound.jump;
	}
	if (currentFreq <= currentSound.end) {
		reset();
	}
}


#endif