#ifndef SUITLIGHTS_CPP
#define SUITLIGHTS_CPP

#include <SuitLights.h>
/*
I'm thinking of having a set of instructions for each command, and when update() is called follow the instructions
for example, the instructions are:
delay 1000
red LED on
delay 1000
green LED on
end
so action() would be called and push the stuff into the command buffer, and then wait.
in the update() function, It would check the micros,and wait until it is called and it has been more than 1000ms
then it would turn the red LED on, and repeat

gonna be a bit complicated, but will work

I think
*/

void SuitLights::setup(myByte iTeam) {
	pinMode(muzzlePin, OUTPUT);
	pinMode(leftPin, OUTPUT);
	pinMode(rightPin, OUTPUT);
	pinMode(hitPin, OUTPUT);
	pinMode(buzzerPin, OUTPUT);
	//TODO: Implement RGB Leds
	team = iTeam;
}
void SuitLights::playIdle() {
	idle = true;
	paused = true;
	currentStep = 0;
	lastTime = millis();
}

bool SuitLights::playLights(const lightControl * command) {
	for (unsigned int i = 0; i<5; i++) {
		if (commandBuffer[i] == NULL) {
			commandBuffer[i] = command;
			return true;
		}
	}
	return false;
}
bool SuitLights::lightCommand(const lightControl steps[]) {
	switch (steps[currentStep]) {
	case allOff:
		digitalWrite(muzzlePin, LOW);
		digitalWrite(hitPin, LOW);
		digitalWrite(leftPin, LOW);
		digitalWrite(rightPin, LOW);
		break;
	case allOn:
		digitalWrite(muzzlePin, HIGH);
		digitalWrite(hitPin, HIGH);
		digitalWrite(leftPin, HIGH);
		digitalWrite(rightPin, HIGH);
		break;
	case muzzleOn:
		digitalWrite(muzzlePin, HIGH);
		break;
	case muzzleOff:
		digitalWrite(muzzlePin, LOW);
		break;
	case hitOn:
		digitalWrite(hitPin, HIGH);
		break;
	case hitOff:
		digitalWrite(hitPin, LOW);
		break;
	case rightOn:
		digitalWrite(rightPin, HIGH);
		break;
	case rightOff:
		digitalWrite(rightPin, LOW);
		break;
	case leftOn:
		digitalWrite(leftPin, HIGH);
		break;
	case leftOff:
		digitalWrite(leftPin, LOW);
		break;
	case Tdelay:
		if (lastTime == 0) {
#ifdef DEBUG
			Serial.println("delay");
#endif
			lastTime = millis();
			return false;
		}
		else if(millis() - lastTime>constDelay){
			lastTime = 0;
		}
		else return false;
		break;
	case over:
#ifdef DEBUG
		Serial.println("Light command over");
#endif
		return true;
		break;
	default:
		return true;
		break;
	}
	currentStep++;
	return false;
}

void SuitLights::pause() {
	paused = !paused;
}

void SuitLights::reset() {
	currentStep = 0;
	lastTime = 0;
	idle = false;
	for (unsigned int i = 0; i<5; i++) {
		commandBuffer[i] = pNull;
	}
	paused = false;
}

bool SuitLights::update() {
	if (paused) {
		if (idle && millis() - lastTime>500) {
			//TODO: Idle animation
			currentStep++;
			if (currentStep >= 8) {
				currentStep = 0;
			}
			lastTime = millis();
		}
		return false;
	}
	else {
		if (commandBuffer[0] != pNull) {
			if (lightCommand(commandBuffer[0])) {
				for (unsigned int i = 1; i<5; i++) {
					commandBuffer[i - 1] = commandBuffer[i];
				}
				commandBuffer[4] = pNull;
				currentStep = 0;
			}
		}
	}
	if (commandBuffer[0] == pNull) return false;
	return true;
}

SuitLights::SuitLights(void) {
	reset();
}

#endif