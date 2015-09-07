/* File automatically created by MariaMole */
#define DEBUG
#include "header.h"

using namespace std;

Suit laser;
IRrecv recver(4);  // I KNOW I SPELLED IT WRONG
decode_results results;
int counter=0;
long int lastTime=0;
long int otherTime=0;

void setup(){
	#ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Booting up...");
	#endif
	pinMode(triggerPin, INPUT_PULLUP);
	pinMode(13,OUTPUT); //used as game indicator during setup
	recver.enableIRIn();
	#ifdef DEBUG
	Serial.println("IR initilization done");
	#endif
	laser.setup(0x00,0x10,&recver);
	laser.clipSize=100;
	laser.clipNum=25;
	laser.startHealth=0x24;
	#ifdef DEBUG
	Serial.println("Suit setup done");
	#endif
	#ifdef DEBUG
	Serial.println("Starting game in 5 seconds...");
	#endif
	digitalWrite(13, HIGH);
	delay(5000);
	digitalWrite(13, LOW);
	laser.currentHealth=100;
	laser.currentArmor=5;
	lastTime=millis();
}


void loop(){
	counter+=millis()-lastTime;
	lastTime=millis();
	if(counter > 300){
		if(!laser.gunCommand(gShoot,0)){
			laser.gunCommand(gReload,0);
		}
		counter=0;
	}
	if(laser.checkStatus()){
		otherTime=millis();
		while(millis()-otherTime<5000){
			laser.checkStatus();
		}
		laser.sCommand(cRespawn,0);
	}
}
