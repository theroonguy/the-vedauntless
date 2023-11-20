#include "Enes100.h"

float normalPS = 0;
float strafePS = 0;
float rotatePS = 0;

#include "movement.h"
#include "sensors.h"
#include "mission.h"

bool wifiModuleConnected = true;

// int angle = 10;

void setup() {
  Serial.begin(9600);

  // WIFI
  if (wifiModuleConnected) {
    Enes100.begin("The Vedauntless", CRASH_SITE, 205, 50, 51);
    Enes100.println("Connected...");
  }

  // SENSORS
  initColorSensor();
  initDistSensor();
  initPot();
  initServo();

  // CALIBRATE
  forwardMPS = calibrateNormal();
  strafeMPS = calibrateStrafe();
  rotateRPS = calibrateRotate();

  moveWithTime(pi/2, 1, 0, 0.5/forwardMPS)

  // navToSite();

  // moveWithTime(pi, 1, 0, 300);

  // maintainDist(200);
  // turnToFace(500, 10);
}

void loop() {

  // servoWrite(angle);

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
