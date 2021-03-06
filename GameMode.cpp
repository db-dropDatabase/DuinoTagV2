#ifndef GAMEMODE_CPP
#define GAMEMODE_CPP

#include <GameMode.h>

#include <DuinoTagV2.h>
//below are the functions
//they run as events just before lights are played on the gun, with the exception of death
//which runs after the lights are finished

#if ON_SHOOT == true
	void Suit::onShoot() {

	}
#endif

#if ON_HIT == true
	void Suit::onHit() {

	}
#endif

#if ON_RELOAD_START == true
	void Suit::onReloadStart() {

	}
#endif

#if ON_RELOAD_END == true
	void Suit::onReloadEnd() {

	}
#endif

#if ON_DEATH == true
	void Suit::onDeath() {

	}
#endif

#if ON_RESPAWN == true
	void Suit::onRespawn() {
		
	}
#endif

#if ON_OBJECTIVE == true
	void Suit::onObj(unsigned int progress) {
		//DO NOT USE NEGATIVE NUMBERS
		//sorry, it's just better this way
	}
#endif

#if ON_GAME_START == true
	void Suit::onGameStart() {

	}
#endif
#if ON_GAME_END == true
	void Suit::onGameEnd() {
		//Spray and pray mass game end test
		unsigned int gameEnd[52] = { 2400, 600, 1200, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 1200, 600, 1200, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 1200, 600, 1200, 600, 1200, 600, 1200, 600, 1200, 600, 1200, 600, 600, 600, 1200, 600, 600, 600, 600, 600, 600, 600 };
		for (int i = 0; i < 25; i++) {
			delayMicroseconds(100);
			irsend.sendRaw(gameEnd, 52, frequency);
			delayMicroseconds(100);
			delay(250);
		}
	}
#endif

#if ON_CUSTOM_EVENT == true
	void Suit::onCustomEvent() {
		Serial.println("Custom Event!");
	}
#endif




#endif
