// THE VEDAUNTLESS -- FINAL CODE

// Import modules
#include "Enes100.h"
#include "util.h"
#include "movement.h"
#include "sensors.h"
#include "mission.h"

// INPUTS
bool doMission = true;
int arucoID = 12;

void setup() {

  // Intialize serial port
  Serial.begin(9600);

  if (doMission) {
    // WIFI
    Enes100.begin("The Vedauntless", CRASH_SITE, arucoID, 50, 51);
    Enes100.println("Connected...");

    // INIT SENSORS
    initColorSensor();
    initDistSensor();
    initPot();
    initServo();

    // NAVIGATE TO SITE
    navToSite(125); // navigate within 125 mm
    delay(1000);

    // ANOMALY DETECTION
    if (isAtAnomaly()) {
      //   Enes100.mission(DIRECTION, NORMAL_Y);
      Enes100.println("Normal Y");
    } else {
      //   Enes100.mission(DIRECTION, NORMAL_X);
      Enes100.println("Normal X");
    }

    // LENGTH MEASUREMENT
    turnToFace(250, 10);  // turn so movement will be parallel to the face
    int length = detectLength(100); // error distance
    if (length <= 150) { length = 135; } 
    else if (length > 150 && length <= 245) { length = 180; } 
    else { length = 270; }
    Enes100.mission(LENGTH, length);
    moveWithTime(pi, 0.5, 0, length/strafePS); // move to middle of face

    // HEIGHT MEASUREMENT
    servo.attach(2);
    int angle = detectHeight(10);  
    Enes100.println(angle);
    if (angle == 13) { angle = 135; } 
    else if (angle == 19) { angle = 180; } 
    else { angle = 270; }
    Enes100.mission(HEIGHT, angle);

    // OBSTACLE AVOIDANCE
    alignMiddle();


  }


}

void loop() {
}
