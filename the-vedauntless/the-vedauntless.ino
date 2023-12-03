// THE VEDAUNTLESS -- FINAL CODE

float batteryLevel = 1.5;  //Full battery ~1.7
float speed = 1;           //ALSO CHANGE SPEED IN findPath(); (mission.h)

// Import modules
#include "Enes100.h"
#include "util.h"
#include "movement.h"
#include "sensors.h"
#include "temp.h"
#include "mission.h"

// INPUTS
bool doMission = true;
int arucoID = 13;

void setup() {

  // Intialize serial port
  Serial.begin(9600);

  // INIT SENSORS
  initColorSensor();
  initDistSensor();
  initPot();
  initServo();
  initMagnet();

  while (getDistance() > 50) {
    Serial.println("waiting for trigger...");
  }

  if (doMission) {
    // WIFI
    Enes100.begin("The Vedauntless", CRASH_SITE, arucoID, 51, 50);
    Enes100.println("Connected...");

    signal();

    delay(1000);

    // NAVIGATE TO SITE
    Enes100.println("Navigating to Crash Site...");
    sForward();
    navToSite(150);  // navigate within 150 mm

    // LENGTH MEASUREMENT
    sStrafe();
    Enes100.println("Detecting Length...");
    turnToFace(250, 10);             // turn so movement will be parallel to the face
    int length = detectLength(100);  // error distance
    if (length <= 150) {
      length = 135;
    } else if (length > 150 && length <= 245) {
      length = 180;
    } else {
      length = 270;
    }
    moveWithTime(pi, 0.5, 0, length / strafePS);  // move to middle of face

    // HEIGHT MEASUREMENT
    Enes100.println("Detecting Height...");
    servo.attach(2);
    int height = detectHeight(10);
    if (height <= 9) {
      height = 135;
    } else if (height < 19 && height > 10) {
      height = 180;
    } else {
      height = 270;
    }
    // Enes100.mission(HEIGHT, height);
    servo.detach();

    // ANOMALY DETECTION & DIMENSION LOGIC
    if (detectAnomaly()) {
      Enes100.println("MISSION MESSAGE: The direction of the abnormality is in the +Y direction");
      digitalWrite(52, LOW);
      sForward();
      moveWithTime(3 * pi / 2, speed, 0, 2500 / speed);
      turnToTheta(pi / 4, pi / 20);
      turnToTheta(0, pi / 20);
      delay(500);
      sForward();
      moveWithTime(pi / 2, speed, 0, 3000 / speed);

    } else {
      Enes100.mission(DIRECTION, 0);
      turnToTheta(0, pi / 20);
      delay(500);
      turnToTheta(0, pi / 20);
      digitalWrite(52, LOW);
      sForward();
      moveWithTime(pi / 2, speed, 0, 3000 / speed);
      turnToTheta(pi / 4, pi / 20);
      turnToTheta(0, pi / 20);
      delay(500);
      //logic
      length = anomalyLogic(height, length);
    }

    Enes100.mission(LENGTH, length);
    Enes100.mission(HEIGHT, height);

    delay(1000);

    // OBSTACLE NAVIGATION
    Enes100.println("Navigating Obstacles...");
    bool midBlocked = false;
    bool topBlocked = false;

    float xmid = 1.96;
    float error = 0.1;
    float errorTurn = pi / 10;

    turnToTheta(0, errorTurn);  // face towards obstacles
    delay(500);

    sStrafe();
    alignY(1, error, 0);

    // align to either top or bottom
    if (Enes100.getY() > 1) {
      alignY(.5, .05, 0);
    } else {
      alignY(1.5, .05, 0);
    }

    // first set of obstacles
    while (Enes100.getX() < 1.8) {  //While before second obstacles
      sForward();
      turnToTheta(pi / 4, pi / 20);
      turnToTheta(0, pi / 20);
      moveUntilBlocked(200, speed, 2);  //Move until hit block
      sStrafe();
      findPath(error);
      sForward();
      Enes100.println("Realigning...");
    }

    // after first set of obstacles
    while (Enes100.getX() < 2.8) {  //While before limbo
      turnToTheta(pi / 4, pi / 20);
      turnToTheta(0, pi / 20);
      sForward();
      moveUntilBlocked(200, speed, 3);  //Move until hit block or reaches final x
      if (Enes100.getX() < 2.8) {         //While before limbo
        sStrafe();
        findPath(error);
        sForward();
        Enes100.println("Realigning...");
      }
    }

    alignY(1.5, error, 0);
    turnToTheta(pi / 4, pi / 20);
    turnToTheta(0, pi / 20);

    moveUntilBlocked(200, speed);

    servo.attach(2);
    for (int ang = 180; ang > (60); ang--) {
      servo.write(ang);
      delay(30);
    }
    servo.detach();
  }
}

void loop() {
}