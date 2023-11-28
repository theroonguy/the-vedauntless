// THE VEDAUNTLESS -- FINAL CODE

// Import modules
#include "Enes100.h"
#include "util.h"
#include "movement.h"
#include "sensors.h"
#include "mission.h"

// INPUTS
bool doMission = true;
int arucoID = 217;

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

    // ANOMALY DETECTION
    if (isAtAnomaly()) {
      Enes100.mission(DIRECTION, pi/2);
      Enes100.println("Normal Y");
    } else {
      Enes100.mission(DIRECTION, 0);
      Enes100.println("Normal X");
    }

    // LENGTH MEASUREMENT
    turnToFace(250, 10);  // turn so movement will be parallel to the face
    int length = detectLength(100); // error distance
    if (length <= 150) { length = 135; } 
    else if (length > 150 && length <= 245) { length = 180; } 
    else { length = 270; }
    Enes100.mission(LENGTH, length);
    Enes100.print("Length: ");
    Enes100.print(length);
    moveWithTime(pi, 0.5, 0, length/strafePS); // move to middle of face

    // HEIGHT MEASUREMENT
    servo.attach(2);
    int height = detectHeight(10);  
    Enes100.println(height);
    if (height == 13) { height = 135; } 
    else if (height == 19) { height = 180; } 
    else { height = 270; }
    Enes100.mission(HEIGHT, height);
    Enes100.print("Height: ");
    Enes100.print(height);

    // OBSTACLE NAVIGATION
    bool midBlocked = false;
    bool topBlocked = false;

    float xmid = 2.5;
    float error = 0.1;

    turnToTheta(0, pi/20);          // face towards obstacles
    moveWithTime(pi/2, 1, 0, 2000); // move forwards a bit
    alignY(1, error, 0);            // align to middle of field
    turnToTheta(0, pi/20);          // reset orientation again 
    alignX(1, error);               // move until in front of middle obstacle

    if (getDistance() < 500) {      // if there is an obstacle there, know that there isn't one behind it
      midBlocked = true;
    }

    turnToTheta(pi/4, pi/20);       // turn to check for topfront obstacle
    if (getDistance() < 1000) {
      topBlocked = true;
    }
    turnToTheta(0, pi/20);          // turn back to straight
    
    if (midBlocked && topBlocked) {     // therefore, lower front is open and top back is open
      alignY(0.5, error, 0);
      alignX(xmid, error);
      alignY(1.5, error, 0);
      moveUntilBlocked(150, 1);     // move through limbo
    } else if (midBlocked && topBlocked == false) {   // go through top, then through middle, and then through limbo
      alignY(1.5, error, 0);
      alignX(xmid, error);
      alignY(1, error, 0);
      moveUntilBlocked(200, 1);
      alignY(1.5, error, 0);
      moveUntilBlocked(150, 1);
    } else if (midBlocked == false && topBlocked) {   // go through middle, then through top
      alignX(xmid, error);
      alignY(1.5, error, 0);
      moveUntilBlocked(150, 1);
    } 
    
  }
}

void loop() {
}
