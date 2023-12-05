// THE VEDAUNTLESS -- FINAL CODE

float norm(float xI, float yI, float xF, float yF) {
  return float(sqrt(pow((xF - xI), 2) + pow((yF - yI), 2)));
}

float batteryLevel = 1.5;  //Full battery ~1.7
float speed = 1;           //ALSO CHANGE SPEED IN findPath(); (mission.h)

// Import modules
#include "Enes100.h"
#include "sensors.h"
#include "movement.h"
#include "mission.h"

// INPUTS
bool doMission = true;
bool testColor = false;
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
      turnToTheta(0, pi / 20);
      // delay(500);
      // sForward();
      // moveWithTime(pi / 2, speed, 0, 2000 / speed);

    } else {
      Enes100.mission(DIRECTION, 0);
      turnToTheta(0, pi / 20);
      digitalWrite(52, LOW);
      // sForward();
      // moveWithTime(pi / 2, speed, 0, 2000 / speed);
      length = anomalyLogic(height, length);
    }

    Enes100.mission(LENGTH, length);
    Enes100.mission(HEIGHT, height);

    // OBSTACLE NAVIGATION
    navigateObstacles(speed);
    moveUntilBlocked(1, 100);

    // FINAL SEQUENCE
    servo.attach(2);
    for (int ang = 180; ang > (60); ang--) {
      servo.write(ang);
      delay(30);
    }
    servo.detach();
  }
}

void loop() {
  if (testColor) {
    detectAnomaly();
    Serial.println("dist");
    Serial.println(getDistance());
  }
}