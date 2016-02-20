#ifndef MILES_DECODE_H
#define MILES_DECODE_H

#include <GameMode.h>

enum SuitCommands {  //also includes message packet commands
	cShot = 139, //take damage
		   //message packet commands
	cAddHealth = 128,
	cAddAmmo = 129,
	cSpecialCommand = 131,
	cKill = 0,//0x00
	cPause = 1,//0x01
			   //cStartGame = 2,//0x02
	cDefaults = 3,//etc.
	cRespawn = 4,
	cINewGame = 5,//Immediate new game
	cFullAmmo = 6,
	cEndGame = 7,
	cResetClock = 8, //not implemented
	cExplodePlayer = 11,
	cReadyUp = 12, //ready up for new game, not implemented
	cFullHealth = 13,
	cFullArmor = 15,
	cClearScores = 20,
	cTestSensors = 21,
	cStun = 22,
	cDisarm = 23,
	cAmmoPick = 136,
	cHealthPick = 137,
	cObj = 138,
	cNull = 0
};

extern double MHitDelay(myByte in);
extern unsigned int milesHealth(myByte health);
extern unsigned int milesRPM(myByte rpm);
extern unsigned int milesDamage(myByte damageIn);

#endif //guard
