#ifndef LASERCOM_H
#define LASERCOM_H

#include <Wire\Wire.h>
#include <GameMode.h>
#include <Bitshift.h>

#define MASTER false
#define PING_VAL 1
#define CONFIRM_VAL 69
#define CONFIRM_TIMEOUT 25

namespace LaserCom {
	enum headTypes {
		I2CSuitCommand = 'S',
		irCommand = 'I',
		briefSetupCommand = 'Y',
		longSetupCommand = 'T',
		statCommand = 'O',
		systemCommand = 7
	};

	enum confirmStates {
		notRec,
		dropped,
		recieved,
		recOther
	};

	struct I2CPacket {
		uint8_t head;
		uint8_t data1;
		uint8_t data2;
	};

	extern I2CPacket lastRead;
	extern unsigned int lastTime;

	extern void begin();
	extern void send(uint8_t header, uint8_t command, uint8_t command2, bool confirm);
	extern I2CPacket read();
	extern uint8_t peek();
	extern void sendConfirmation();
	extern confirmStates checkConfirmation();
	extern void clearBuffer();
	extern bool update();
}



#endif