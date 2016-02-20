#ifndef LASERWIFI_CPP
#define LASERWIFI_CPP

#include <LaserWiFi.h>

RF24 LaserWifi::radio(RADIO_PIN_1, RADIO_PIN_2);
RF24Network LaserWifi::network(LaserWifi::radio);
RF24Mesh LaserWifi::mesh(LaserWifi::radio, LaserWifi::network);

bool LaserWifi::begin(uint8_t nodeID) {
	mesh.setNodeID(nodeID);
	return mesh.begin();
}

bool LaserWifi::update() {
	LaserWifi::mesh.update();
	if (!mesh.checkConnection()) {
		mesh.renewAddress();
	}
	return network.available();
}

uint8_t LaserWifi::getPacketType() {
	RF24NetworkHeader packet;
	network.peek(packet);
	return packet.type;
}

uint8_t LaserWifi::getPacket() {
	RF24NetworkHeader packet;
	uint8_t out=0;
	network.read(packet, &out, sizeof(uint8_t));
	return out;
}

bool LaserWifi::sendMaster(uint16_t type, uint8_t message) {
	LaserWifi::mesh.write((uint8_t)00, &message, type, sizeof(uint8_t));
}

bool LaserWifi::sendNode(uint8_t nodeID, uint16_t type, uint8_t message) {
	LaserWifi::mesh.write(nodeID, &message, type, sizeof(uint8_t));
}







#endif //guard