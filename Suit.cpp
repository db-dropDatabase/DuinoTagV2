#ifndef SUIT_CPP
#define SUIT_CPP

#include <Suit.h>

Suit::Suit():ir() {
	teamID = 0;
	playerID = 0;
}

void Suit::setup() {
	LaserCom::begin();
	while (true) {
		while (!LaserCom::update());
		LaserCom::I2CPacket stuff = LaserCom::read();
		if (stuff.head == LaserCom::briefSetupCommand) {
			if (stuff.data1 << 2 == 0 && stuff.data2 << 7 == 0) {
				LaserCom::sendConfirmation();
				LaserCom::clearBuffer();
				teamID = stuff.data1;
				playerID = stuff.data2;
				return;
			}
		}
		else if (stuff.head == LaserCom::longSetupCommand) {
			//TODO: Long setup command
			LaserCom::clearBuffer();
			LaserCom::sendConfirmation();
			lightsOn();
		}
		SER_PRINTLN("Bad Team and PlayerID!");
		LaserCom::clearBuffer();
	}
}

void Suit::setup(uint8_t iTeamID, uint8_t iPlayerID) {
	teamID = iTeamID;
	playerID = iPlayerID;
}

void Suit::waitForStart() {
	while (true) {
		while (!LaserCom::update());
		LaserCom::I2CPacket things = LaserCom::read();
		if (things.head == LaserCom::I2CSuitCommand && things.data2 == cStartGame) {
			LaserCom::sendConfirmation();
			lightsOn();
			irOn();
			return;
		}
		else SER_PRINTLN("Bad Game Start Command!");
	}
}

void Suit::startGame() {
	lightsOn();
	irOn();
	return;
}

parsedPacket Suit::readPacket(packet packetYay) {
#ifdef DEBUG
	Serial.println("The packet is as follows: ");
	for (int i = 7; i >= 0; i--) {
		Serial.print(packetYay.data1.grab(i));
		Serial.print(", ");
	}
	Serial.println("");
	Serial.println("Part 2: ");
	for (int i = 7; i >= 0; i--) {
		Serial.print(packetYay.data2.grab(i));
		Serial.print(", ");
	}
	Serial.println("");
#endif
	parsedPacket superYay;
	superYay.amount = 0;
	superYay.whatToDo = 0;
	if (!packetYay.data1.grab(7)) { //check very first (last?) bit for message type
									//shot packet
									//add stats here later
		if (packetYay.data2.store >> 4 == teamID) { //checks to see if shot by friend or foe, and if friendlyfire is on
													//take damage, and if I make a stats machine record player Id
#ifdef DEBUG
			Serial.println("teamID is the same as shooter teamid");
#endif
#if FRIENDLY_FIRE == true
			packetYay.data2.flip(6, 0);
			packetYay.data2.flip(7, 0); //wipe the teamID so I can grab the damage
			superYay.amount = milesDamage(packetYay.data2.store / 4); //grab the damage value, exclude the teamID, divide by 4 because damage isn't long enough to fill byte, and it leaves 2 zeros on the right side
			superYay.whatToDo = cShot;
#if USE_STATS == true
			stat.addValue(sHit, packetYay.data1.store - 128);
#endif
#else
			superYay.whatToDo = cNull;
#endif
		}
		else {
			packetYay.data2.store = packetYay.data2.store << 2;
			superYay.amount = milesDamage(packetYay.data2.store >> 4);
#ifdef VERBOSE_DEBUG
			Serial.print("Thing! :");
			Serial.println(packetYay.data2.store >> 4);
#endif
			superYay.whatToDo = cShot;
#if USE_STATS == true
			stat.addHit(packetYay.data1.store);
#endif
		}

	}
	else {
		//b/c of the leading one in packet type, all messages are 128+whatever
#ifdef DEBUG
		Serial.println("Mesasage packet!");
#endif
		superYay.whatToDo = packetYay.data1.store;
		superYay.amount = packetYay.data2.store;
#ifdef DEBUG
		Serial.println(packetYay.data1.store);
		Serial.println(packetYay.data2.store);
#endif
	}
	return superYay;

}

bool Suit::I2CSuitCommand(I2CSuitCommands command) {
	switch (command) {
	case cIrOn:
		irOn();
		break;
	case cIrOff:
		irOff();
		break;
	case cLightsOn:
		lightsOn();
		break;
	case cLightsOff:
		lightsOff();
		break;
	case cResetValues:
		setup();
		break;
#if USE_STATS == true
	case cResetStats:
		stat.reset();
		break;
	case cReportStats:
		LaserCom::clearBuffer();
		uint8_t hits[127];
		stat.returnHits(*hits);
		for (uint8_t i = 0; i < 128; i++) {
			if (hits[i] != 0) LaserCom::send(LaserCom::statCommand, i, hits[i], false);
		}
		break;
#endif
	case cStartGame:
		//play starting lights
		lightsOn();
		irOn();
		lights.playLights(pLightsGameOn);
		break;
	default:
		return false;
		break;
	}
	return true;
}

bool Suit::I2CSuitCommand(I2CSuitCommands command, I2CSuitCommands command2) {
	if (command2 != 0) {
		if (command == cPlayLights) return I2CLightCommand((I2CLightCommands)command);
		if (I2CSuitCommand(command)) return I2CSuitCommand(command2);
		return false;
	}
	else return I2CSuitCommand(command);
}

bool Suit::I2CLightCommand(I2CLightCommands command) {
	switch (command) {
	case lightsHit:
		lights.playLights(pLightsHit);
		break;
	case lightsGameOn:
		lights.playLights(pLightsGameOn);
		break;
	case lightsDead:
		lights.playLights(pLightsDead);
		break;
	case lightsGameOver:
		lights.playLights(pLightsGameOver);
		break;
	default:
		return false;
		break;
	}
	return true;
}

void Suit::updateIR() {
	packet out;
	if (ir.checkRecieve(out)) {
		parsedPacket someData = readPacket(out);
		LaserCom::send(LaserCom::irCommand, someData.whatToDo, someData.amount, false);
	}
}

void Suit::updateI2C() {
	if (LaserCom::peek()) {
		LaserCom::I2CPacket bob = LaserCom::read();
		if (!I2CSuitCommand((I2CSuitCommands)bob.data1, (I2CSuitCommands)bob.data2)) SER_PRINTLN("Bad Packet over I2C");
		LaserCom::clearBuffer();
	}
}

void Suit::updateLights() {
	lights.update();
}

inline void Suit::irOn() {
	ir.clearBuffer();
	irEnabled = true;
}

inline void Suit::irOff() {
	irEnabled = false;
}

inline void Suit::lightsOff() {
	if (lightsEnabled) {
		lightsEnabled = false;
		lights.pause();
	}
}

inline void Suit::lightsOn() {
	if (!lightsEnabled) {
		lightsEnabled = true;
		lights.pause();
	}
}

#if USE_STATS == true
SuitStats::SuitStats() {
	reset();
}

void SuitStats::reset() {
	for (uint8_t i = 0; i < 128; i++) hitCount[i] = 0;
}

void SuitStats::addHit(uint8_t input) {
	hitCount[input]++;
}

void SuitStats::returnHits(uint8_t & ray) {
	ray = *hitCount;
}
#endif

#endif //guard