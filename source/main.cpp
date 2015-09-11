/* File automatically created by MariaMole */
#define DEBUG
#include "header.h"

using namespace std;

Suit laser;
IRrecv recver(4);  // I KNOW I SPELLED IT WRONG
decode_results results;
//long int lastTime=0;

void setup(){
	#ifdef DEBUG
	Serial.begin(9600);
	delay(200);
	Serial.println("Booting up...");
	#endif
	//pinMode(triggerPin, INPUT_PULLUP);
	pinMode(13,OUTPUT); //used as game indicator during setup
	laser.setup(0x02,0x10,&recver);
	laser.clipSize=100;
	laser.clipNum=25;
	laser.startHealth=0x24;
	#ifdef DEBUG
	Serial.println("IR initilization done");
	#endif
	#ifdef DEBUG
	Serial.println("Suit setup done");
	#endif
	#ifdef DEBUG
	Serial.println("Starting game in 5 seconds...");
	#endif
	digitalWrite(13, HIGH);
	delay(5000);
	digitalWrite(13, LOW);
}


void loop(){
	//if(millis()-lastTime > 5000){
	//	laser.gunCommand(gShoot,0);
	//	lastTime=millis();
	//}
	if(laser.checkStatus()){
		laser.sCommand(cRespawn,0);
	}
}
