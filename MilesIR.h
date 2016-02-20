#ifndef MILESIR_H
#define MILESIR_H

#include <GameMode.h>
#include <Bitshift.h>
#include <Custom IR\IRremote.h>

class MilesIR {
	IRsend send;
	IRrecv recv;
	unsigned int decodePulse(int pulseLength);
public:
	MilesIR();
	void shoot(myByte shotPacket[]);
	bool checkRecieve(packet & out);
	void clearBuffer();
};



#endif //guard
