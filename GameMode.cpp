#ifndef GAMEMODE_CPP
#define GAMEMODE_CPP

#include <GameMode.h>

//now to define what actually happens in the functions
#if ON_SHOOT == true
	void onShoot() {

	}
#endif

#if ON_HIT == true
	void onHit() {

	}
#endif

#if ON_RELOAD_START == true
	void onReloadStart() {

	}
#endif

#if ON_RELOAD_END == true
	void onReloadEnd() {

	}
#endif

#if ON_DEATH == true
	void onDeath() {

	}
#endif

#if ON_RESPAWN == true
	void onRespawn() {

	}
#endif

#if ON_OBJECTIVE_START== true
	void onObjStart() {

	}
#endif

#if ON_OBJECTIVE_DNF == true
	void onObjDnf() {

	}
#endif

#if ON_OBJECTIVE_FINISH == true
	void onObjFinish() {

	}
#endif

#if ON_GAME_START == true
	void onGameStart() {

	}
#endif
#if ON_GAME_END == true
	void onGameEnd() {

	}
#endif

#if ON_CUSTOM_EVENT == true
	void onCustomEvent() {

	}
#endif




#endif
