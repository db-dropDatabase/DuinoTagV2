#ifndef LASERWIFI_H
#define LASERWIFI_H

#define MESH_NOMASTER

#include <RF24-driver\RF24.h>
#include <RF24-network\RF24Network.h>
#include <RF24-mesh\RF24Mesh.h>
#include <SPI.h>
#include <GameMode.h>

/*
RF24 based class structure which will translate and handle all wifi communication in duinoTag
*/
//pins
#define RADIO_PIN_1 7
#define RADIO_PIN_2 8
//node config
#define NODE_ID 1
//debug
#define DEBUG
//some nice serial statments

typedef unsigned int unt;

namespace LaserWifi {
	extern RF24 radio;
	extern RF24Network network;
	extern RF24Mesh mesh;

	//Packet types, to make universal
	const uint16_t suitCommand = 'C';
	const uint16_t statCommand = 'S';
	const uint16_t objCommand = 'O';
	const uint16_t setupCommand = 'X';

	extern bool begin(uint8_t nodeID);
	extern bool update();
	extern uint8_t getPacketType();
	extern uint8_t getPacket();
	extern bool sendMaster(uint16_t type, uint8_t message);
	extern bool sendNode(uint8_t nodeID, uint16_t type, uint8_t message);
};


#endif //guard
