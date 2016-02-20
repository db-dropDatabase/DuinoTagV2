#ifndef LIGHTS_CPP
#define LIGHTS_CPP

#include <Lights.h>
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
/*

void Arduino::setup(unsigned int smaxHealth, unsigned int smaxAmmo, unsigned int smaxArmor, myByte iTeam) {
	pinMode(muzzlePin, OUTPUT);
	pinMode(leftPin, OUTPUT);
	pinMode(rightPin, OUTPUT);
	pinMode(hitPin, OUTPUT);
	pinMode(buzzerPin, OUTPUT);
	neopix = Adafruit_NeoPixel(16, neoPin, NEO_GRB + NEO_KHZ800);
	left = Adafruit_NeoPixel(8, leftPin, NEO_GRB + NEO_KHZ800);
	right = Adafruit_NeoPixel(8, rightPin, NEO_GRB + NEO_KHZ800);

	neopix.begin();
	left.begin();
	right.begin();

	neopix.setBrightness(32);
	left.setBrightness(32);
	right.setBrightness(32);

	for (unsigned int i = 0; i<8; i++) {
		left.setPixelColor(i, teamColors[team]);
		right.setPixelColor(i, teamColors[team]);
	}
	left.show();
	right.show();
	neopix.show();
	reset();
	aMaxHealth = smaxHealth;
	aMaxAmmo = smaxAmmo;
	aMaxArmor = smaxArmor;
	team = iTeam;

}
void Arduino::playIdle() {
	idle = true;
	paused = true;
	currentStep = 0;
	lastTime = millis();
}

bool Arduino::playLights(const lightControl * command) {
	for (unsigned int i = 0; i<5; i++) {
		if (commandBuffer[i] == NULL) {
			commandBuffer[i] = command;
			return true;
		}
	}
	return false;
}
bool Arduino::lightCommand(const lightControl steps[]) {
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
		for (unsigned int i = 0; i < 8; i++) {
			right.setPixelColor(i, teamColors[team]);
		}
		right.show();
		break;
	case rightOff:
		right.clear();
		right.show();
		break;
	case leftOn:
		for (unsigned int i = 0; i < 8; i++) {
			left.setPixelColor(i, teamColors[team]);
		}
		left.show();
		break;
	case leftOff:
		left.clear();
		left.show();
		break;
	case Tdelay:
		if (!delaying) {
#ifdef DEBUG
			Serial.println("delay");
#endif
			lastTime = micros();
			delaying = true;
			return false;
		}
		else {
			if (micros() - lastTime>constDelay * 1000L) {
				delaying = false;
			}
			else {
				return false;
			}
		}
		break;
	case playHit:
		if (!pewOverride) {
			pewOverride = true;
			Sounds::playSound(Sounds::pHit);
			return false;
		}
		else if (Sounds::playingSound) {
			return false;
		}
		else {
			pewOverride = false;
		}
		break;
	case playDead:
		if (!pewOverride) {
			pewOverride = true;
			Sounds::playSound(Sounds::pDead);
			return false;
		}
		else if (Sounds::playingSound) {
			return false;
		}
		else {
			pewOverride = false;
		}
		break;
	case playGameOn:
		if (!pewOverride) {
			pewOverride = true;
			Sounds::playSound(Sounds::pStart);
			return false;
		}
		else if (Sounds::playingSound) {
			return false;
		}
		else {
			pewOverride = false;
		}
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

void Arduino::pause() {
	paused = !paused;
}

void Arduino::reset() {
	currentStep = 0;
	lastTime = 0;
	lastPewTime = 0;
	healthDisp = 0;
	ammoDisp = 0;
	armorDisp = 0;
	soundDelay = 0;
	delaying = false;
	newHealth = false;
	newAmmo = false;
	pewOverride = false;
	idle = false;
	for (unsigned int i = 0; i<5; i++) {
		commandBuffer[i] = pNull;
	}
	Sounds::reset();
	noToneAC();
	paused = false;
}

void Arduino::changeValues(double aHealth, double aAmmo, double	aArmor) {
	if (healthDisp != aHealth) {
#ifdef DEBUG
		Serial.print("Changing health value to: ");
		Serial.println(aHealth);
#endif
		healthDisp = aHealth;
		newHealth = true;
	}
	if (ammoDisp != aAmmo) {
#ifdef DEBUG
		Serial.print("Changing ammo value to: ");
		Serial.println(aAmmo);
#endif
		ammoDisp = aAmmo;
		newAmmo = true;
	}
	if (armorDisp != aArmor) {
#ifdef DEBUG
		Serial.print("Changing armor value to: ");
		Serial.println(aArmor);
#endif
		armorDisp = aArmor;
		newHealth = true;
	}
}
void Arduino::playPew() {
	if (!pewOverride) {
		Sounds::playSound(Sounds::pPew);
	}
	digitalWrite(muzzlePin, HIGH);
	lastPewTime = millis();
}

bool Arduino::update() {
	//update display
	if (newHealth) {
#ifdef VERBOSE_DEBUG
		Serial.println("Updating health!");
		Serial.println(healthDisp);
		Serial.println(aMaxHealth);
#endif
		for (double i = 0; i<8.0; i++) {
			if (i*(aMaxHealth / 8.0) < healthDisp) {
				neopix.setPixelColor(i, 0, 255, 0);
			}
			else {
				neopix.setPixelColor(i, 0, 0, 0);
			}
		}

#ifdef VERBOSE_DEBUG
		Serial.println("Updating armor!");
		Serial.println(armorDisp);
		Serial.println(aMaxArmor);
#endif
		for (double i = 0; i<8.0; i++) {
			if (i*(aMaxArmor / 8.0) < armorDisp) {
				neopix.setPixelColor(i, 200, 100, 0);
			}
			else {
				i = 9;
			}
		}
		newHealth = false;
		neopix.show();
	}
	if (newAmmo) {
#ifdef VERBOSE_DEBUG
		Serial.println("Updating ammo!");
#endif
		for (double i = 0; i<8.0; i++) {
			if (i*(aMaxAmmo / 8.0) < ammoDisp) {
				neopix.setPixelColor(i + 8, 255, 255, 0);
			}
			else {
				neopix.setPixelColor(i + 8, 0, 0, 0);
			}
		}
		newAmmo = false;
		neopix.show();
	}
	if (paused) {
		if (idle && millis() - lastTime>500) {
			left.clear();
			right.clear();
			left.setPixelColor(currentStep, 255, 0, 0);
			right.setPixelColor(currentStep, 255, 0, 0);
			left.show();
			right.show();
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
		if (lastPewTime > 0) {
			if (millis() - lastPewTime > constDelay / 4) {
				lastPewTime = 0;
				digitalWrite(muzzlePin, LOW);
			}
		}
	}
	if (commandBuffer[0] == pNull) {
		return false;
	}
	else {
		return true;
	}
}

Arduino::Arduino(void) {
	reset();
}
*/

#endif //guard