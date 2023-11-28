#include "Enes100.h"
#include "util.h"
#include "movement.h"
#include "sensors.h"
#include "mission.h"

bool wifiModuleConnected = true;
bool doNav = true;
bool calibrateAtStart = false;

int delayTime = 1000; // in ms

void setup() {

  Serial.begin(9600);

  // WIFI
  if (wifiModuleConnected) {
    Enes100.begin("The Vedauntless", CRASH_SITE, 12, 50, 51);
    Enes100.println("Connected...");

    // CALIBRATE SPEEDS -- only if wifi is enabled
    if (calibrateAtStart) {
      calibrate(2000, 1000, 1.25);
    }
  }

  // INIT SENSORS
  initColorSensor();
  initDistSensor();
  initPot();
  initServo();

  // CALIBRATE PLANK
  if (calibrateAtStart) {
    calibratePlank(180, 60);
    delay(delayTime);
  }

  delay(3000);


  if (doNav) {
    // NAV TO SITE
    navToSite(125); // within 125 mm
    delay(delayTime);
  }

  // ANOMALY DETECTION
  // turnToFace(250, 10);  // turn so movement will be parallel to the face
  // if (isAtAnomaly()) {
  //   //   Enes100.mission(DIRECTION, NORMAL_Y);
  // } else {
  //   //   Enes100.mission(DIRECTION, NORMAL_X);
  //   delay(delayTime);
  // }

  // HEIGHT MEASUREMENTS
  turnToFace(250, 10);  // turn so movement will be parallel to the face
  servo.attach(2);
  moveUntilBlocked(125, 0.5);
  int angle = detectHeight(10);  
  if (wifiModuleConnected) {
    Enes100.println(angle);
    if (angle == 13) {
      Enes100.mission(HEIGHT, 135);
    } else if (angle == 19) {
      Enes100.mission(HEIGHT, 180);
    } else {
      Enes100.mission(HEIGHT, 270);
    }
  }
  delay(delayTime);

  // LENGTH MEASUREMENTS
  int length = detectLength(100); // error distance
  if (length <= 150) {
    if (wifiModuleConnected) {Enes100.mission(LENGTH, 135);}
    moveWithTime(pi, 0.5, 0, 135/strafePS);
  } else if (length > 150 && length <= 245) {
    if (wifiModuleConnected) {Enes100.mission(LENGTH, 180);}
    moveWithTime(pi, 0.5, 0, 180/strafePS);
  } else {
    if (wifiModuleConnected) {Enes100.mission(LENGTH, 270);}
    moveWithTime(pi, 0.5, 0, 270/strafePS);
  }
  delay(delayTime);

  alignMiddle();


  // moveWithTime(pi/2, 1, 0, (1/normalPS)*1000);    // drive a full meter
  // delay(10000);
  // moveWithTime(0, 0, 1, (2*pi)/(rotatePS)*1000);

  // moveWithTime(pi, 1, 0, 300);

  // maintainDist(200);
  // turnToFace(500, 10);
}

void loop() {
}
