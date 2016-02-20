#ifndef MILESIR_CPP
#define MILESIR_CPP

#include <MilesIR.h>

MilesIR::MilesIR():recv(recievePin) {
	recv.enableIRIn();
	recv.resume();
}

void MilesIR::shoot(myByte shotPacket[]) {
	delayMicroseconds(100);
	send.sendRaw(shotPacket, sizeof(shotPacket), frequency);
	delayMicroseconds(100);
	recv.enableIRIn();
	recv.resume();
}

void MilesIR::clearBuffer() {
	recv.resume();
}

bool MilesIR::checkRecieve(packet & outPacket) {
	decode_results results;
	if (recv.decode(&results)) {
		unsigned int counter = 15;
#ifdef VERBOSE_DEBUG
		Serial.println("Raw Packet:");
		for (unsigned int o = 1; o < results.rawlen; o++) {
			Serial.print(results.rawbuf[o]);
			Serial.print(", ");
		}
		Serial.println("");
#endif
		for (unsigned int i = 1; i < results.rawlen && counter >= 0; i += 2) {
			if (decodePulse(results.rawbuf[i]) == 0) {
				if (counter < 8 && counter >= 0) {
					outPacket.data2.flip(counter, 0);
					counter--;
				}
				else if (counter < 16) {
					outPacket.data1.flip(counter - 8, 0);
					counter--;
				}
			}
			else if (decodePulse(results.rawbuf[i]) == 1) {
				if (counter < 8 && counter >= 0) {
					outPacket.data2.flip(counter, 1);
					counter--;
				}
				else if (counter < 16) {
					outPacket.data1.flip(counter - 8, 1);
					counter--;
				}
			}
		}
		/*
		if(counter < 0){
		unsigned int otherInt=7;
		//verify message terminator
		bool terminator[8];
		for(; i<results.rawlen && counter>=0; i+=2){
		switch(decodePulse(results.rawbuf[i])){
		case 0:
		terminator[otherInt]=false;
		otherInt--;
		break;
		case 1:
		terminator[otherInt]=true;
		otherInt--;
		break;
		default:
		break;
		}
		}
		if(boolToInt(terminator) != 0xe8){
		#ifdef DEBUG
		Serial.println("Terminators do not match!");
		for(unsigned int i=0; i<8; i++){
		Serial.print(terminator[i]);
		}
		#endif
		if(currentHealth<1 && !isDead){
		isDead=true;
		#if USE_STATS == true
		stat.addValue(sDeath,1);
		#endif
		display.playLights(pLightsDead);
		}
		recv->enableIRIn();
		recv->resume();
		return isDead;
		}
		#ifdef DEBUG
		Serial.println("Terminators Match!");
		#endif
		}
		*/
		return outPacket.data1.store;
	}
}

unsigned int MilesIR::decodePulse(int pulseLength) {
	if (pulseLength > IR_BURST_UPPER) {
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