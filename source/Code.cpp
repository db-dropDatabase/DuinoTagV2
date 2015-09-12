#include "IRremote.cpp"
#include "toneAC.cpp"
#include "Adafruit_NeoPixel.cpp"
#include "header.h"

#ifndef DEBUG
#define DEBUG
#endif

using namespace std;

//used to remove some compilier optimization
//hopefully

//miles library

int milesDamage(int damageIn){
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
	switch(damageIn) {
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
int milesRPM(myByte rpm){
	return (rpm*50)+250;
}
int milesHealth(myByte health){
	int in = health;
	int out;
	if(in <= 20){
		out = in;
	}
	else{
		out = ((in-20)*5)+20;
	}
	return out;
}
double MHitDelay(myByte in){
	double out;
	switch(in){
		case 0:
		out=0;
		break;
		case 1:
		out=0.25;
		break;
		case 2:
		out=0.5;
		break;
		case 3:
		out=0.75;
		break;
		default:
		out=(in-3.0);
		break;
	}
	return out;
}
//unrealted
int decodePulse(int pulseLength){
	if(pulseLength > 60){
		#ifdef DEBUG
		Serial.println("Recieved too long pulse!");
		#endif //debug
		return -1;
	}
	else if(pulseLength > 40){
		return -2;
	}
	else if(pulseLength > 20){
		return 1;
	}
	else if(pulseLength > 8){
		return 0;
	}
	else{
		return -1;
	}
}
//duinotag
//header

/*
I'm thinking of having a set of instructions for each command, and when update() is called follow the instructions
for example, the instructions are:
delay 1000
red LED on
delay 1000
green LED on
end
so action() would be called and push the stuff into the command buffer, and then wait.
in the update() function, It would check the micros,and wait until it is called and it has been more than 1000ms
then it would turn the red LED on, and repeat

gonna be a bit complicated, but will work

I think
*/

//int micros(); //used as a temporary substitution for the arduino function
bool initPin(int pinNum,uint8_t mode){
	if(pinNum>13){
		return false;
	}
	else{
		pinMode(pinNum, mode);
		return true;
	}
}

void writePin(int pinNum, uint8_t mode){
	if(pinNum>0 && pinNum<14){
		digitalWrite(pinNum, mode);
	}
}
//cpp
bool Arduino::setup(int smaxHealth, int smaxAmmo, int smaxArmor, myByte team){
	bool check=true;
	if(!initPin(muzzlePin, OUTPUT)){
		check=false;
	}
	if(!initPin(leftPin, OUTPUT)){
		check=false;
	}
	if(!initPin(rightPin, OUTPUT)){
		check=false;
	}
	if(!initPin(hitPin, OUTPUT)){
		check=false;
	}
	if(!initPin(buzzerPin, OUTPUT)){
		check=false;
	}
	neopix = Adafruit_NeoPixel(8, neoPin, NEO_GRB + NEO_KHZ800); //change to 16 later
	left = Adafruit_NeoPixel(8/*?*/, leftPin, NEO_GRB + NEO_KHZ800);
	right = Adafruit_NeoPixel(8, rightPin, NEO_GRB + NEO_KHZ800);
	
	neopix.begin();
	left.begin();
	right.begin();
	
	neopix.setBrightness(32);
	left.setBrightness(32);
	right.setBrightness(32);
	
	for(int i=0; i<8; i++){
		switch(team){
		case 0:
			left.setPixelColor(i, 55, 128, 255);
			right.setPixelColor(i, 55, 128, 255);
			break;
		case 1:
			left.setPixelColor(i, 255, 0, 0);
			right.setPixelColor(i, 255, 0, 0);
			break;
		case 2:
			left.setPixelColor(i,255,0,95);
			right.setPixelColor(i,255,0,95);
			break;
		case 3:
			left.setPixelColor(i,255,128,0);
			right.setPixelColor(i,255,128,0);
			break;
		default:
		#ifdef DEBUG
		Serial.println("bad teamID parsed");
		#endif
		break;
		}
	}
	left.show();
	right.show();
	neopix.show();
	reset();
	aMaxHealth=smaxHealth;
	aMaxAmmo=smaxAmmo;
	aMaxArmor=smaxArmor;
	return check;
	
}
void Arduino::playIdle(){
	idle=true;
	paused=true;
	currentStep=0;
	lastTime=millis();
}

bool Arduino::playLights(arduinoLights command){
	bool check=false;
	for(int i=0; i<5&&!check; i++){
		if(commandBuffer[i]==null){
			commandBuffer[i]=command;
			check=true;
		}
	}
	if(check){
		return true;
	}
	else{
		return false;
	}
}
bool Arduino::lightCommand(const lightControl steps[15]){
	switch(steps[currentStep]){
		case allOff:
		writePin(muzzlePin, LOW);
		writePin(hitPin, LOW);
		writePin(leftPin, LOW);
		writePin(rightPin,LOW);
		break;
		case allOn:
		writePin(muzzlePin, HIGH);
		writePin(hitPin, HIGH);
		writePin(leftPin, HIGH);
		writePin(rightPin,HIGH);
		break;
		case muzzleOn:
		//#ifdef DEBUG
		//Serial.println("Muzzle on");
		//#endif
		writePin(muzzlePin, HIGH);
		break;
		case muzzleOff:
		//#ifdef DEBUG
		//Serial.println("Muzzle off");
		//#endif
		writePin(muzzlePin, LOW);
		break;
		case hitOn:
		//#ifdef DEBUG
		//Serial.println("Hit LED on");
		//#endif
		writePin(hitPin, HIGH);
		break;
		case hitOff:
		//#ifdef DEBUG
		//Serial.println("Hit LED off");
		//#endif
		writePin(hitPin, LOW);
		break;
		case rightOn:
		//#ifdef DEBUG
		//Serial.println("Right on");
		//#endif
		writePin(rightPin, HIGH);
		break;
		case rightOff:
		//#ifdef DEBUG
		//Serial.println("Right off");
		//#endif
		writePin(rightPin, LOW);
		break;
		case leftOn:
		//#ifdef DEBUG
		//Serial.println("Left on");
		//#endif
		writePin(leftPin, HIGH);
		break;
		case leftOff:
		//#ifdef DEBUG
		//Serial.println("Left off");
		//#endif
		writePin(leftPin, LOW);
		break;
		case Tdelay:
		if(!delaying){
			#ifdef DEBUG
			Serial.println("delay");
			#endif
			lastTime=micros();
			delaying=true;
			return false;
		}
		else{
			if(micros()-lastTime>constDelay*1000L){
				delaying=false;
			}
			else{
				return false;
			}
		}
		break;
		case playHit:
		if(!pewOverride){
			noToneAC();
			soundDelay=750;
			pewOverride=true;
			return false;
		}
		else if(soundDelay > 100){
			toneAC(soundDelay,10);
			soundDelay-=1;
			delayMicroseconds(50);
			return false;
		}
		else{
			noToneAC();
			soundDelay=0;
			pewOverride=false;
		}
		break;
		case playDead:
		if(!pewOverride){
			#ifdef DEBUG
			Serial.println("DEAD!");
			#endif
			noToneAC();
			soundDelay=1000;
			pewOverride=true;
			return false;
		}
		else if(soundDelay > 10){
			toneAC(soundDelay,10);
			soundDelay--;
			delay(1);
			return false;
		}
		else{
			noToneAC();
			soundDelay=0;
			pewOverride=false;
		}
		break;
		case playGameOn:
		if(!pewOverride){
			#ifdef DEBUG
			Serial.println("PEW");
			#endif
			noToneAC();
			soundDelay=50;
			pewOverride=true;
			return false;
		}
		else if(soundDelay < 2000){
			toneAC(soundDelay,10);
			soundDelay+=1;
			return false;
		}
		else{
			soundDelay=0;
			noToneAC();
			pewOverride=false;
		}
		break;
		case over:
		#ifdef DEBUG
		Serial.println("Light command over");
		#endif
		return true;
		break;
		default:
		return true;
		break;
	}
	currentStep++;
	return false;
}

void Arduino::pause(){
	paused = !paused;
}

void Arduino::reset(){
	currentStep=0;
	lastTime=0;
	healthDisp=0;
	ammoDisp=0;
	armorDisp=0;
	soundDelay=0;
	delaying=false;
	newHealth=false;
	newAmmo=false;
	currentPew=0;
	pewCommand=false;
	pewOverride=false;
	idle=false;
	for(int i=0; i<5; i++){
		commandBuffer[i] = null;
	}
	noToneAC();
	paused=false;
}

void Arduino::changeValues(double aHealth, double aAmmo, double	aArmor){
	if(healthDisp != aHealth){
		#ifdef DEBUG
		Serial.print("Changing health value to: ");
		Serial.println(aHealth);
		#endif
		healthDisp=aHealth;
		newHealth=true;
	}
	if(ammoDisp != aAmmo){
		#ifdef DEBUG
		Serial.print("Changing ammo value to: ");
		Serial.println(aAmmo);
		#endif
		ammoDisp=aAmmo;
		newAmmo=true;
	}
	if(armorDisp != aArmor){
		#ifdef DEBUG
		Serial.print("Changing armor value to: ");
		Serial.println(aArmor);
		#endif
		armorDisp=aArmor;
		newHealth=true;
	}
}
void Arduino::playPew(){
	pewCommand=true;
	currentPew=0;
	
}

bool Arduino::update(){
	//update display
	if(newHealth){
		#ifdef VERBOSE_DEBUG
		Serial.println("Updating health!");
		Serial.println(healthDisp);
		Serial.println(aMaxHealth);
		#endif
		for(double i=0; i<8.0; i++){
			if(i*(aMaxHealth/8.0) < healthDisp){
				neopix.setPixelColor(i, 0, 255, 0);
			}
			else{
				neopix.setPixelColor(i,0,0,0);
			}
		}
		
		#ifdef VERBOSE_DEBUG
		Serial.println("Updating armor!");
		Serial.println(armorDisp);
		Serial.println(aMaxArmor);
		#endif
		for(double i=0; i<8.0; i++){
			if(i*(aMaxArmor/8.0) < armorDisp){
				neopix.setPixelColor(i,200,100,0);
			}
			else{
				i=9;
			}
		}
		newHealth=false;
		neopix.show();
	}
	/* need to attach all neopixels before I can use this code do later
	if(newAmmo){
		#ifdef DEBUG
		Serial.println("Updating ammo!");
		#endif
		for(double i=0; i<8.0; i++){
			if(i*(aMaxAmmo/8.0) < ammoDisp){
				neopix.setPixelColor(i+8, 255, 255, 0);
			}
			else{
				neopix.setPixelColor(i+8,0,0,0);
			}
		}
		newAmmo=false;
		neopix.show();
	}
	*/
	//check for delay
	if(paused){
		if(idle && millis()-lastTime>500){
			left.clear();
			right.clear();
			left.setPixelColor(currentStep, 255, 0, 0);
			right.setPixelColor(currentStep, 255, 0, 0);
			left.show();
			right.show();
			currentStep++;
			if(currentStep>=8){
				currentStep=0;
			}
			lastTime=millis();
		}
		return false;
	}
	bool over = false;
	switch(commandBuffer[0]){
		case pLightsHit:
		{
			over = lightCommand(hit);
		}
		break;
		case pLightsGameOn:
		{
			over = lightCommand(gameOn);
		}
		break;
		case pLightsDead:
		{
			over = lightCommand(dead);
		}
		break;
		case pLightsGameOver:
		{
			over = lightCommand(gameOver);
		}
		break;
	}
	if(over){
		for(int i=1; i<5; i++){
			commandBuffer[i-1]=commandBuffer[i];
		}
		commandBuffer[4] = null;
		currentStep=0;
	}
	if(pewCommand){
		switch(currentPew){
			case 0:
			//muzzleOn
			digitalWrite(muzzlePin, HIGH);
			currentPew=1200;
			break;
			case 600:
			//muzzle off
			digitalWrite(muzzlePin, LOW);
			if(!pewOverride){
				noToneAC();
			}
			currentPew=0;
			pewCommand=false;
			break;
			default:
			//play sound
			if(!pewOverride){
				toneAC(currentPew,10);
				currentPew--;
			}
			else{
				currentPew--;
			}
				
			break;
		}
	}
	if(commandBuffer[0]==null && !pewCommand){
		return false;
	}
	else{
		return true;
	}
}

//lasercode
//header

//types, such as states are in main header file

/*
us = microsecond
milestag packets are constructed as below:

2400us header, 600us pause, then 1200us transmit for 1 and 600us transmit for 0,etc
shot packet consists of header+one bit for packet type (0 for shot packet), 7 more bits for player id, 2 bits for team id, then 4 bits for damage
message packet consists of one bit for packet type, 7 more bits for message ID, 8 more bits for data, then 8 more bits for EOF

below is a table of cloning data, which allows for one gun to effectively transmit it's settings to another gun
this will be the format for the lasergun class, without armor, heath, or other suit references

myByte number Description of data
4 Reserved
5 Reserved
6 Reserved
7 Team ID - See section 2.3.1
8 Reserved
9 Clips added by picking up an ammo box
10 Health added by picking up a medic box
11 Reserved (0x03 for 5.41)
12 Hit LED timeout in seconds
13 Sound set - See section 2.3.2
14 Overheat limit in rounds/min
15 Reserved 16 Reserved
17 Damage per shot - See section 2.3.3
18 Clip size - 0xFF is unlimited
19 Number of clips - 0xCA is unlimited
20 Fire selector - See 2.3.4
21 Number of rounds for burst mode
22 Cyclic RPM - See 2.3.5
23 Reload delay in seconds
24 IR power - See 2.3.6
25 IR range - See 2.3.7
26 Tagger on/o settings - See 2.3.8
27 Respawn health - See 2.3.9 28 Reserved
29 Respawn delay in tens of seconds
30 Armour value
31 Game on/o settings 1/2 - See 2.3.10
32 Game on/o settings 2/2 - See 2.3.11
33 Hit delay - See 2.3.12
34 Start delay in seconds
35 Death delay in seconds
36 Time limit in minutes
37 Maximum respawns
38 Reserved (0xFF in 5.41)
39 Checksum - See 2.3.13

copypastad from document on milestag
suit will control things such as hits, health, respawns, flag, etc.
lasergun will shoot and control ammo, reload, etc.
*/



//laser gun reference/triggers
//cpp

//for now, I will assume that the packet receiving is handled by something else
//let's write some functions
void intToBool(unsigned int input, unsigned int start, unsigned int len, bool * ray){
	unsigned int mod=2;
	start += len-1;
	while(input>0 || len>0){
		if(input%mod>0){
			ray[start] = true;
		}
		else{
			ray[start] = false;
		}
		input-=input%mod;
		start--;
		len--;
		mod*=2;
	}
}

int boolToInt(bool input[8]){
	int out=0;
	int adder=128;
	for(int i=7; i>=0; i--){
		if(input[i] == 1){
			out += adder;
		}
		adder/=2;
	}
	return out;
}

void Suit::setUpPacket(){
	bool packet[13];
	intToBool(playerID,0,7,packet);
	intToBool(teamID,7,2,packet);
	intToBool(damage,9,4,packet);
	#ifdef DEBUG
	Serial.println("String Packet: ");
	for(int i=0; i<13; i++){
		Serial.print(packet[i]);
		Serial.print(", ");
	}
	Serial.println("");
	#endif
	shotPacket[0] = 2400; //header
	shotPacket[1] = 600; //first space
	shotPacket[2] = 600; //first zero to say shot packet
	shotPacket[3] = 600; //secound space
	int bitcounter=0;
	for(int i=4; i<30; i++){
		if(!(i & 1)){ //alternating
			if(packet[bitcounter]==false){
				shotPacket[i] = 600; //0
			}
			else{
				shotPacket[i] = 1200; //1
			}
			bitcounter++;
		}
		else{
			shotPacket[i]=600;
		}
	}
}


bool Suit::setup(myByte iTeamID, myByte iPlayerID, IRrecv * showMe){
	//set all values to defaults except for the modifiers
	//first set modifiers
	
	teamID = iTeamID;
	playerID = iPlayerID;
	//now set the defaults!
	hitLEDTimeout = 0x04;
	startHealth = 0x24; //100
	respawnTime = 0x03; //30 secs
	armor = 50; //50
	friendlyFire = 0;
	zombieMode = 0;
	medic = 0;
	respawns = false;
	gamemode = 0;
	hitDelay = 0x00;
	startDelay = 0x3c; //60 sec
	deathDelay = 0x00;
	timeLimit = 0x1e; //30 min
	maxRespawn = 0x00;
	respawnLimit = 0;
	numRespawns=0;
	isDead=true;
	currentHealth=0;
	currentArmor=0;
	display.setup(milesHealth(startHealth), clipSize, armor, teamID);
	display.playIdle();
	//lasergun setup
	
	//clipNum = 5;
	//clipSize = 5; //This Doesnt work. I don't know why.
	//defaults
	overheat=0;
	damage=0x03; //5
	fireType=0x02; //full auto;
	burstRounds=0x00;
	rpm=0x00; //250 cyclic rpm
	reload=0x04;
	IRPower=0;
	gunSettings=0x01; //muzzle flash on, no overheat
	medic=0;
	ammoReset=1;
	currentDelay=0;
	tmpTime=0;
	currentAmmo=0;
	rpmDelay=1000/(milesRPM(rpm)/60);
	setUpPacket();
	//IR
	if(showMe!=NULL){
		recv = showMe;
	}
	return true;
	
}

parsedPacket Suit::readPacket(packet packetYay){
	#ifdef VERBOSE_DEBUG
	Serial.println("The packet is as follows: ");
	for(int i=7; i>=0; i--){
		Serial.print(packetYay.data1[i]);
		Serial.print(", ");
	}
	Serial.println("");
	Serial.println("Part 2: ");
	for(int i=7; i>=0; i--){
		Serial.print(packetYay.data2[i]);
		Serial.print(", ");
	}
	Serial.println("");
	#endif
	parsedPacket superYay;
	if(!packetYay.data1[7]){ //check very first (last?) bit for message type
		//shot packet
		//add stats here later
		int tmpTeamID = 2*packetYay.data2[7] + packetYay.data2[6];
		if(tmpTeamID == teamID){ //checks to see if shot by friend or foe, and if friendlyfire is on
			//take damage, and if I make a stats machine record player I
			#ifdef DEBUG
			Serial.println("teamID is the same as shooter teamid");
			#endif
			if(friendlyFire){
				packetYay.data2[6] = 0;
				packetYay.data2[7] = 0; //wipe the teamID so I can grab the damage
				superYay.amount= milesDamage((boolToInt(packetYay.data2)/4)); //grab the damage value, exclude the teamID, divide by 4 because damage isn't long enough to fill byte, and it leaves 2 zeros on the right side
				superYay.whatToDo=cShot;
			}
			else{
				superYay.whatToDo=cNull;
			}
		}
		else{
			packetYay.data2[6] = 0;
			packetYay.data2[7] = 0;
			superYay.amount= milesDamage((boolToInt(packetYay.data2)/4)); //grab the damage value, exclude the teamID, divide by 4 because damage isn't long enough to fill byte, and it leaves 2 zeros on the right side
			superYay.whatToDo=cShot;
		}
		
	}
	else{
		//b/c of the leading one in packet type, all messages are 128+whatever
		#ifdef DEBUG
		Serial.println("Mesasage packet!");
		#endif
		int testing1 = boolToInt(packetYay.data1);
		int testing2 = boolToInt(packetYay.data2);
		#ifdef DEBUG
		Serial.println(boolToInt(packetYay.data1));
		Serial.println(boolToInt(packetYay.data2));
		#endif
		int amountOrCommand = testing2; 
		switch(testing1){
			case 128:
			superYay.whatToDo = cAddHealth;
			superYay.amount = amountOrCommand;
			break;
			case 129:
			superYay.whatToDo = cAddAmmo;
			superYay.amount = amountOrCommand;
			break;
			case 130: //reserved
			break;
			case 131:
			switch(amountOrCommand){
				case 0:
				superYay.whatToDo=cKill;
				break;
				case 1:
				superYay.whatToDo=cPause;
				break;
				case 2:
				superYay.whatToDo=cStartGame;
				break;
				case 3:
				superYay.whatToDo=cDefaults;
				break;
				case 4:
				superYay.whatToDo=cRespawn;
				break;
				case 5:
				superYay.whatToDo=cINewGame;
				break;
				case 6:
				superYay.whatToDo=cFullAmmo;
				break;
				case 7:
				superYay.whatToDo=cEndGame;
				break;
				case 8:
				superYay.whatToDo=cResetClock;
				break;
				//9 is reserved
				//10 is an unused command
				case 11:
				superYay.whatToDo=cExplodePlayer;
				break;
				case 12:
				superYay.whatToDo=cReadyUp;
				break;
				case 13:
				superYay.whatToDo=cFullHealth;
				break;
				//14 is reserved
				case 15:
				superYay.whatToDo=cFullArmor;
				break;
				//16, 17, 18, and 19 are reserved
				case 20:
				superYay.whatToDo=cClearScores;
				break;
				case 21:
				superYay.whatToDo=cTestSensors;
				break;
				case 22:
				superYay.whatToDo=cStun;
				break;
				case 23:
				superYay.whatToDo=cDisarm;
				break;
				default:
				superYay.whatToDo=cNull;
				break;
			} //bunch of admin commands
			break;
			//132,135,136 are reserved
			case 135:
			//sytstem data.  This includes cloning and score transfer
			//don't want to tackle at the moment do later
			break;
			//the rest are clips, health, and flag pickup
			//will do later
			default:
			superYay.whatToDo=cNull;
			break;
		}
		
	}
	if(superYay.amount > 500){
		#ifdef DEBUG
		Serial.println("amount not initilized, so set to zero");
		#endif
		superYay.amount = 0;
	}
	return superYay;
	
}

bool Suit::action(packet packetYay){
	parsedPacket out;
	out = readPacket(packetYay);
	if(out.whatToDo != cNull){
		sCommand(out.whatToDo, out.amount);
		return true;
	}
	else{
		return false;
	}
	
}

void Suit::sCommand(SuitCommmands command, int amount){
	//ugh, here we go
	//I guess I have to add arduino commands in here, so library might not function as expected
	//functions only in arduino libraries: delayMicroseconds(), delay()
	switch(command){
		case cShot:
		{
			//play hit animation, subtract health, pause?
			if(!isDead){
				delayMicroseconds(MHitDelay(hitDelay)); //hit delay function turns hit delay myByte into seconds
				if(currentArmor <= 0){
					currentHealth-= amount;
				}
				else{
					currentArmor -= amount;
					currentHealth -= amount/2;
				}
				#ifdef DEBUG
				Serial.print("Hit! Damage was: ");
				Serial.println(amount);
				#endif
				#ifdef VERBOSE_DEBUG
				Serial.print("Current Health is: ");
				Serial.println(currentHealth);
				Serial.print("And armor is: ");
				Serial.print(currentArmor);
				Serial.println("");
				#endif
				display.playLights(pLightsHit);
			}
			#ifdef DEBUG
			else{
				Serial.println("Hit, but very dead!");
			}
			#endif
		}
		break;
		case cAddHealth:
		{
			currentHealth+=amount;
			display.playLights(pLightsGameOn);
		}
		break;
		case cAddAmmo:
		{
			gunCommand(gAddAmmo, amount);
			display.playLights(pLightsGameOn);
		}
		break;
		case cKill:
		{
			isDead=true;
			gunCommand(gStop,0);
			display.playLights(pLightsDead);
			while(display.update()){}
			display.playIdle();
			display.changeValues(0,0,0);
			display.update(); //no health left!
		}
		break;
		case cPause:
		{
			//not implemented
		}
		break;
		case cStartGame:
		{
			if(!respawns){
				//varibles! yay!
				delayMicroseconds(startDelay);
				respawns=true;
				currentHealth=milesHealth(startHealth);
				currentArmor=armor;
				isDead=false;
				gunCommand(gFullAmmo,0);
				display.setup(milesHealth(startHealth), clipSize, armor, teamID);
				display.playLights(pLightsGameOn);
				while(display.update()){}
				#ifdef DEBUG
				Serial.println("Game started!");
				#endif
			}
		}
		break;
		case cDefaults:
		{
			setup(teamID, playerID, NULL);
			display.setup(milesHealth(startHealth), clipSize, armor, teamID);
			display.playLights(pLightsGameOn);
		}
		break;
		case cRespawn:
		{
			if(respawns&&isDead){
				if(respawnLimit){
					numRespawns++;
				}
				if(numRespawns <= maxRespawn){
					#ifdef DEBUG
					Serial.println("Respawn!");
					#endif
					isDead=false;
					currentHealth=milesHealth(startHealth);
					currentArmor=armor;
					gunCommand(gFullAmmo,0);
					display.setup(milesHealth(startHealth), clipSize, armor, teamID);
					display.playLights(pLightsGameOn);
				}
			}
		}
		break;
		case cINewGame:
		{
			respawns=true;
			currentHealth=milesHealth(startHealth);
			currentArmor=armor;
			isDead=false;
			gunCommand(gFullAmmo,0);
			display.reset();
			display.playLights(pLightsGameOn);
		}
		break;
		case cFullAmmo:
		{
			gunCommand(gFullAmmo,0);
			display.playPew();
		}
		break;
		case cEndGame:
		{
			gunCommand(gStop,0);
			isDead=true;
			respawns=false;
			display.playLights(pLightsGameOver);
		}
		break;
		case cResetClock:
		{
			//will do later
		}
		break;
		case cExplodePlayer:
		{
			isDead=true;
			gunCommand(gStop,0);
			display.playLights(pLightsDead);
		}
		break;
		case cReadyUp:
		{
			//not implemented
		}
		break;
		case cFullHealth:
		{
			currentHealth=milesHealth(startHealth);
			display.playLights(pLightsGameOn);
		}
		break;
		case cFullArmor:
		{
			currentArmor=armor;
			display.playLights(pLightsGameOn);
		}
		break;
		case cClearScores:
		{
			//not implemented
		}
		break;
		case cTestSensors:
		{
			//I have no idea how to do this yet, so whatever.
		}
		break;
		case cStun:
		{
			gunCommand(gStop,0);
			display.playLights(pLightsHit);
			delay(2500);
		}
		break;
		case cDisarm:
		{
			gunCommand(gStop,0);
			display.playLights(pLightsHit);
			delay(2500);
		}
		break;
		case cNull:
		{
			#ifdef DEBUG
			Serial.println("null command recieved");
			#endif
		}
		break;
		
	}
}

bool Suit::gunCommand(GunCommands command, int amount){
	switch(command){
		case gStop:
		{
			display.reset();
			currentClip=0;
			return true;
		}
		break;
		case gShoot:
		{
			if(currentDelay>0){
				#ifdef VERBOSE_DEBUG
				Serial.println("Rpm delayed! Shot may not fire!");
				Serial.println((double)currentDelay);
				Serial.println("minus");
				Serial.println((millis()-tmpTime));
				#endif
				currentDelay=currentDelay - (millis()-tmpTime);
				tmpTime=millis();
			}
			if(currentClip>0&&!isDead&&currentDelay<=0){
				if(clipSize!=999){
					currentClip--;
				}
				delayMicroseconds(100);
				irsend.sendRaw(shotPacket, 30, frequency);
				delayMicroseconds(100);
				recv->enableIRIn();
				#ifdef DEBUG
				Serial.println("BANG");
				Serial.println(rpmDelay);
				#endif
				currentDelay=rpmDelay;
				display.playPew();
				return true;
			}
			else{
				#ifdef DEBUG
				if(isDead){
					Serial.println("Shot not fired because death!");
				}
				else if(currentClip<=0){
					Serial.println("Shot not fired because currentClip<0");
				}
				else{
					Serial.println("Shot not fired because RPM delay!");
				}
				#endif
				if(currentDelay>0){
					return true;
				}
				return false;
			}
		}
		break;
		case gReload:
		{
			while(display.update() == true){
				display.changeValues(currentHealth,currentAmmo,currentArmor);
			}
			
			if(clipNum!=999){ //check for unlimited
				currentAmmo--;
			}
			if(currentAmmo > 0&&!isDead){
				currentReload=reload;
				#ifdef DEBUG
				Serial.println("RELOADING!");
				Serial.print("Time: ");
				Serial.println(currentReload);
				#endif
				long int lastTime=millis();
				int reloadStatus=0;
				while(currentReload>0){
					if(millis()-lastTime>1000){
						lastTime=millis();
						currentReload--;
						#ifdef DEBUG
						Serial.println(currentReload);
						#endif
					}
					reloadStatus = (currentReload * (100/reload))/(100/clipSize);
					display.changeValues(currentHealth,reloadStatus,currentArmor);
					checkStatus();
				}
				currentClip=clipSize;
				return true;
			}
			else{
				return false;
			}
		}
		break;
		case gOverheat:
		{
			//not implemented
			return false;
		}
		break;
		case gAddAmmo:
		{
			currentAmmo+=amount;
			return true;
		}
		break;
		case gFullAmmo:
		{
			currentClip=clipSize;
			currentAmmo=clipNum;
			return true;
		}
		break;
		case gTest:
		{
			//no entiendo, por favor (me no spanish)
			//not implemented
			return false;
		}
		break;
	}
}



bool Suit::checkStatus() { //this function will return is the user is dead, but will also check to see if any packets are ready for processing, and if so, proccess them and take apropriete action
	decode_results results;
	if(recv->decode(&results)){
		while(display.update()){}
		packet outPacket;
		int counter=15;
		int i=0;
		while((results.rawbuf[i]<40||results.rawbuf[i]>60)&&i<results.rawlen){
			i++;
		}
		#ifdef VERBOSE_DEBUG
		Serial.println("Raw Packet:");
		for(int o=i; o<results.rawlen; o++){
			Serial.print(results.rawbuf[o]);
			Serial.print(", ");
		}
		Serial.println("");
		#endif
		for(; i<results.rawlen && counter>=0; i+=2){
			switch(decodePulse(results.rawbuf[i])){
				case 0:
				if(counter<8 && counter >= 0){
					outPacket.data2[counter] = false;
					counter--;
				}
				else if(counter<16){
					outPacket.data1[counter-8] = false;
					counter--;
				}
				break;
				case 1:
				if(counter<8 && counter >= 0){
					outPacket.data2[counter] = true;
					counter--;
				}
				else if(counter<16){
					outPacket.data1[counter-8] = true;
					counter--;
				}
				break;
				default:
				break;
			}
		}
		if(counter < 0){
			/*
			int otherInt=7;
			//verify message terminator
			myByte terminator=0x00;
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
			myByte check = 0xe8;
			if(terminator != check){
				#ifdef DEBUG
				Serial.println("Terminators do not match!");
				for(int i=0; i<8; i++){
					Serial.print(terminator.test(i));
				}
				Serial.println(check.to_ulong());
				#endif
				if(currentHealth<1 && !isDead){
				isDead=true;
				display.playLights(pLightsDead);
				}
				recv->enableIRIn();
				recv->resume();
				return isDead;
			}
			#ifdef DEBUG
			Serial.println("Terminators Match!");
			#endif
			ARGHHHHHHHH
			*/
		}
		if(!action(outPacket)){
			#ifdef DEBUG
			Serial.println("Parsing packet failed!");
			#endif
		}
		#ifdef DEBUG
		else{
			Serial.println("Parsing packet succeded!");
		}
		#endif
		recv->enableIRIn();
		recv->resume();
	}
	if(currentHealth<1 && !isDead){
		display.playLights(pLightsDead);
		while(display.update()){}
		display.playIdle();
		delay(255);
		isDead=true;
	}
	if(currentClip>0){ //reload function will handle it
		display.changeValues(currentHealth,currentClip,currentArmor);
	}
	display.update();
	return isDead;
}