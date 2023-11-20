#include "Enes100.h"
#include "util.h"
#include "movement.h"
#include "sensors.h"
#include "mission.h"

bool wifiModuleConnected = true;
bool calibrateAtStart = false;

void setup() {
  Serial.begin(9600);

  // WIFI
  if (wifiModuleConnected) {
    Enes100.begin("The Vedauntless", CRASH_SITE, 205, 50, 51);
    Enes100.println("Connected...");
  }

  // CALIBRATE SPEEDS
  if (calibrateAtStart) {
    calibrate(2000, 1000, 1.25);
  }

  // INIT SENSORS
  initColorSensor();
  initDistSensor();
  initPot();
  initServo();

  int height = detectHeight(150);
  Enes100.println(height);

  // moveWithTime(pi/2, 1, 0, (1/normalPS)*1000);    // drive a full meter
  // delay(10000);
  // moveWithTime(0, 0, 1, (2*pi)/(rotatePS)*1000);

  // // navToSite();

  // moveWithTime(pi, 1, 0, 300);

  // maintainDist(200);
  // turnToFace(500, 10);
}

void loop() {

  // scan from 0 to 180 degrees
  // for (angle = 10; angle < 180; angle++) {
  //   servo.write(angle);
  //   delay(15);
  //   Serial.println(readPot());
  // }
  // // now scan back from 180 to 0 degrees
  // for (angle = 180; angle > 10; angle--) {
  //   servo.write(angle);
  //   delay(15);
  //   Serial.println(readPot());
  // }
  
  // int val = getDistance();
  // Serial.println(val);
  // detectAnomaly();

  // delay(300);
}
