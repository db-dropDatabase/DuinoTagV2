#ifndef MILES_DECODE_CPP
#define MILES_DECODE_CPP

#include <MilesDecode.h>

unsigned int milesDamage(myByte damageIn) {
	//damage table shown below:
	/*Value Damage dealt
	0x00 1
	0x01 2
	0x02 4
	0x03 5
	0x04 7
	0x05 10
	0x06 15
	0x07 17
	0x08 20
	0x09 25
	0x0A 30
	0x0B 35
	0x0C 40
	0x0D 50
	0x0E 75
	0x0F 100*/
	switch (damageIn) {
	case 0:
		return 1;
		break;
	case 1:
		return 2;
		break;
	case 2:
		return 4;
		break;
	case 3:
		return 5;
		break;
	case 4:
		return 7;
		break;
	case 5:
		return 10;
		break;
	case 6:
		return 15;
		break;
	case 7:
		return 17;
		break;
	case 8:
		return 20;
		break;
	case 9:
		return 25;
		break;
	case 10:
		return 30;
		break;
	case 11:
		return 35;
		break;
	case 12:
		return 40;
		break;
	case 13:
		return 50;
		break;
	case 14:
		return 75;
		break;
	case 15:
		return 100;
		break;
	default:
		return 0;
		break;
	}
}
unsigned int milesRPM(myByte rpm) {
	return (rpm * 50) + 250;
}
unsigned int milesHealth(myByte health) {
	if (health <= 20) {
		return health;
	}
	else {
		return ((health - 20) * 5) + 20;
	}
}
double MHitDelay(myByte in) {
	double out;
	switch (in) {
	case 0:
		out = 0;
		break;
	case 1:
		out = 0.25;
		break;
	case 2:
		out = 0.5;
		break;
	case 3:
		out = 0.75;
		break;
	default:
		out = (in - 3.0);
		break;
	}
	return out;
}

unsigned int decodePulse(int pulseLength) {
	if (pulseLength > IR_BURST_UPPER) {
#ifdef DEBUG
		Serial.println("Recieved too long pulse!");
#endif //debug
		return -1;
	}
	else if (pulseLength > IR_BURST_HEADER) {
		return -2;
	}
	else if (pulseLength > IR_BURST_ONE) {
		return 1;
	}
	else if (pulseLength > IR_BURST_ZERO) {
		return 0;
	}
	else {
		return -1;
	}
}
#endif //guard