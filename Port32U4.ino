/* File automatically created by MariaMole */
//#define DEBUG
#define VERBOSE_DEBUG

#include <Suit.h>

//This code will be split into two sections, one for each of the two boards in lasertag
//which one you are compiling to can be switched here

#define SUIT_OR_GUN false //false is suit, true is gun

#if SUIT_OR_GUN == false
//suit code

Suit suit;
bool firstRun = false;

void setup() {
	Serial.begin(115200);
	
}

void loop() {
	if (!firstRun) {
		Serial.println("Waiting for setup");
		suit.setup();
		Serial.println("Starting game!");
		suit.startGame();
		firstRun = true;
	}
	suit.updateI2C();
	suit.updateIR();
	suit.updateLights();
}

#else
//gun code






#endif



