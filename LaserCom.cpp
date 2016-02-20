#ifndef LASERCOM_CPP
#define LASERCOM_CPP

#include <LaserCom.h>

LaserCom::I2CPacket LaserCom::lastRead;
unsigned int LaserCom::lastTime;

void LaserCom::begin() {
#if MASTER 
	Wire.begin();

#else
	Wire.begin(1);
#endif
	while (!Wire.available()) {
#if MASTER
		Wire.beginTransmission(1);
#else
		Wire.beginTransmission(0);
#endif
		Wire.write(systemCommand);
		Wire.write(PING_VAL);
		Wire.endTransmission();
		delay(100);
		SER_PRINTLN("Wire Ping!");
	}
	clearBuffer();
}

void LaserCom::send(uint8_t header, uint8_t command, uint8_t command2, bool confirm) {
#if MASTER
	Wire.beginTransmission(1);
#else
	Wire.beginTransmission(0);
#endif
	Wire.write(header);
	Wire.write(command);
	if (command2 != 0) Wire.write(command2);
	Wire.endTransmission();
	if (confirm) lastTime = millis();
}

LaserCom::I2CPacket LaserCom::read() {
	I2CPacket out = lastRead;
	lastRead.head = 0;
	lastRead.data1 = 0;
	lastRead.data2= 0;
	return out;
}

uint8_t LaserCom::peek() {
	return lastRead.head;
}

void LaserCom::sendConfirmation() {
#if MASTER
	Wire.beginTransmission(1);
#else
	Wire.beginTransmission(0);
#endif
	Wire.write(systemCommand);
	Wire.write(CONFIRM_VAL);
	Wire.endTransmission();
}

LaserCom::confirmStates LaserCom::checkConfirmation() {
	if (lastRead.head == 0) {
		if (millis() - lastTime > CONFIRM_TIMEOUT && lastTime != 0) {
			lastTime = 0;
			return dropped;
		}
		return notRec;
	}
	if (lastRead.head == systemCommand && lastRead.data1 == CONFIRM_VAL) {
		return recieved;
	}
	return recOther;
}

void LaserCom::clearBuffer() {
	while (Wire.available()) Wire.read();
	lastRead.head = 0;
	lastRead.data1 = 0;
	lastRead.data2 = 0;
}

bool LaserCom::update() {
	if (Wire.available()) {
		lastRead.head = Wire.read();
		lastRead.data1 = Wire.read();
		lastRead.data2 = Wire.read();
		while (Wire.available()) Wire.read();
		return true;
	}
	return false;
}
#endif