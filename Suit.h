#ifndef SUIT_H
#define SUIT_H

#define MASTER false

#ifdef DEBUG
# define SER_PRINT(...) Serial.print(__VA_ARGS__)
# define SER_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
# define SER_PRINT(...)
# define SER_PRINTLN(...)
#endif

#include <SuitLights.h>
#include <LaserCom.h>
#include <MilesIR.h>
#include <MilesDecode.h>
#include <GameMode.h>

enum I2CSuitCommands {
	cIrOn = 1,
	cIrOff,
	cLightsOn,
	cLightsOff,
	cPlayLights,
	cResetStats,
	cResetValues,
	cReportStats,
	cStartGame,
	cNull1
};

enum I2CLightCommands {
	lightsHit = 1,
	lightsGameOn = 2,
	lightsDead = 3,
	lightsGameOver = 4,
	null = 0
};

enum GunCommands {
	gStop = 0,
	gShoot,
	gReload,
	gOverheat,
	gAddAmmo,
	gFullAmmo,
	gTest
};

struct parsedPacket {
	unsigned int whatToDo;
	unsigned int amount; //for add ammo or health
};

#if USE_STATS == true
class SuitStats {
public:
	void returnHits(uint8_t & ray);
	void addHit(uint8_t input);
	void reset();
	SuitStats();
private:
	uint8_t hitCount[127]; //I got hit x times by y!
};
#endif

class Suit {
public:
	Suit();
	void setup();
	void setup(uint8_t iteamID, uint8_t iPlayerId); 
	parsedPacket readPacket();
	bool I2CSuitCommand(I2CSuitCommands command);
	bool I2CSuitCommand(I2CSuitCommands command, I2CSuitCommands command2);
	bool I2CLightCommand(I2CLightCommands command);
	void waitForStart();
	void startGame();
	void updateIR();
	void updateLights();
	void updateI2C();
	inline void irOff();
	inline void irOn();
	inline void lightsOff();
	inline void lightsOn();
private:
	bool irEnabled;
	bool lightsEnabled;
	parsedPacket readPacket(packet packetYay);
	uint8_t teamID;
	uint8_t playerID;
	MilesIR ir;
	SuitLights lights;
#if USE_STATS == true
	SuitStats stat;
#endif
};

#endif //guard
