/* File automatically created by MariaMole */
#define DEBUG
#include <DuinoTagV2.h>

using namespace std;
using namespace Sounds;

Suit laser;
IRrecv recver(recievePin);  // I KNOW I SPELLED IT WRONG
decode_results results;
long int lastTime = 0;

void setup() {
#ifdef DEBUG
	Serial.begin(115200);
	delay(200);
	Serial.println("Booting up...");
#endif
	recver.enableIRIn();
#ifdef DEBUG
	Serial.println("IR initilization done");
	Serial.println("Entering setup mode");
#endif
	//laser.waitForSetup(&recver);
	//pinMode(triggerPin, INPUT_PULLUP);
	pinMode(13, OUTPUT); //used as game indicator during setup
	laser.setup(0x02, 0x10, &recver);
	//laser.startHealth=0x24;
#ifdef DEBUG
	Serial.println("Suit setup done");
#endif
#ifdef DEBUG
	Serial.println("Starting game in 5 seconds...");
#endif
	digitalWrite(13, HIGH);
	delay(5000);
	digitalWrite(13, LOW);
	laser.sCommand(cStartGame, 0);
}


void loop() {
	if (millis() - lastTime > 200) {
		
		if (!laser.gunCommand(gShoot, 0) && !laser.isDead) {
			laser.gunCommand(gReload, 0);
		}
		
		//playSound(pPew);
#ifdef DEBUG
		Serial.println("BANG");
#endif
		lastTime = millis();
	}
	if (laser.checkStatus()) {
		laser.sCommand(cRespawn, 0);
	}
}
