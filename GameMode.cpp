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

#if ON_OBJECTIVE_START== true
	void Suit::onObjStart(int progress) {

	}
#endif

#if ON_OBJECTIVE_FINISH == true
	void Suit::onObjFinish(int progress) {

	}
#endif

#if ON_GAME_START == true
	void Suit::onGameStart() {

	}
#endif
#if ON_GAME_END == true
	void Suit::onGameEnd() {

	}
#endif

#if ON_CUSTOM_EVENT == true
	void Suit::onCustomEvent() {

	}
#endif




#endif
