#include "Enes100.h"

float normalPS = 0;
float strafePS = 0;
float rotatePS = 0;

#include "util.h"
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

  moveUntilBlocked(150, 0.5);

  servo.write(150);
  delay(500);
  servo.write(120);
  delay(500);
  servo.write(90);
  delay(500);
  servo.write(60);
  delay(500);
  servo.detach();
  delay(1000);
  Enes100.println(readPot());


  // int interval = 2000;
  // int time = 1000;

  // // CALIBRATE
  // float multiplier = 1.25;
  // normalPS = calibrateNormal(time, interval) / multiplier;
  // strafePS = calibrateStrafe(time, interval) / multiplier;
  // rotatePS = calibrateRotate(time, interval) / multiplier;

  // Enes100.println(normalPS);
  // Enes100.println(strafePS);
  // Enes100.println(rotatePS);

  // moveWithTime(pi/2, 1, 0, (1/normalPS)*1000);    // drive a full meter
  // delay(10000);
  // moveWithTime(0, 0, 1, (2*pi)/(rotatePS)*1000);

  // // navToSite();

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
