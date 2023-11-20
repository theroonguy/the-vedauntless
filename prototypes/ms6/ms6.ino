#include "Enes100.h"
#include "movement.h"
#include "sensors.h"
#include "mission.h"

bool wifiModuleConnected = false;

void setup() {
  Serial.begin(9600);

  // WIFI
  if (wifiModuleConnected) {
    Enes100.begin("The Vedauntless", CRASH_SITE, 6, 51, 50);
    Enes100.println("Connected...");
  }

  // SENSORS
  initColorSensor();
  initDistSensor();
  initPot();

  // navToSite();

  // moveWithTime(pi, 1, 0, 300);

  // maintainDist(200);
  // turnToFace(500, 10);

}

void loop() {

  Serial.println(readPot());
  servoWrite(90);

  // int val = getDistance();
  // Serial.println(val);
  // detectAnomaly();

  // delay(300);
}
