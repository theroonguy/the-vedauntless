// THE VEDAUNTLESS -- FINAL CODE

// Import modules
#include "Enes100.h"
#include "util.h"
#include "movement.h"
#include "sensors.h"
#include "mission.h"

// INPUTS
bool doMission = true;
int arucoID = 13;

void setup() {

  // Intialize serial port
  Serial.begin(9600);

  initMagnet();

  if (doMission) {
    // WIFI
    Enes100.begin("The Vedauntless", CRASH_SITE, arucoID, 51, 50);
    Enes100.println("Connected...");

    signal();

    // INIT SENSORS
    initColorSensor();
    initDistSensor();
    initPot();
    initServo();

    delay(5000);

    // NAVIGATE TO SITE
    Enes100.println("Navigating to Crash Site...");
    navToSite(125); // navigate within 125 mm

    // ANOMALY DETECTION
    if (detectAnomaly()) {
      Enes100.mission(DIRECTION, pi/2);
      digitalWrite(52, LOW);
    } else {
      Enes100.mission(DIRECTION, 0);
      moveWithTime(0, 0, 0.5, (pi/2)*2000/rotatePS);
      digitalWrite(52, LOW);
      moveWithTime(0, 0, -0.5, (pi/2)*2000/rotatePS);      
    }

    // LENGTH MEASUREMENT
    Enes100.println("Detecting Length...");
    turnToFace(250, 10);  // turn so movement will be parallel to the face
    int length = detectLength(100); // error distance
    Enes100.print("LENGTH: ");
    Enes100.print(length);
    Enes100.println("");
    if (length <= 150) { length = 135; } 
    else if (length > 150 && length <= 245) { length = 180; } 
    else { length = 270; }
    Enes100.mission(LENGTH, length);
    moveWithTime(pi, 0.5, 0, length/strafePS); // move to middle of face

    // HEIGHT MEASUREMENT
    Enes100.println("Detecting Height...");
    servo.attach(2);
    int height = detectHeight(10); 
    Enes100.println(height); 
    if (height == 13) { height = 135; } 
    else if (height == 14) { height = 180; } 
    else { height = 270; }
    Enes100.mission(HEIGHT, height);

    delay(2000);

    // OBSTACLE NAVIGATION
    Enes100.println("Navigating Obstacles...");
    bool midBlocked = false;
    bool topBlocked = false;

    float xmid = 1.96;
    float error = 0.1;

    turnToTheta(0, pi/20);          // face towards obstacles
    delay(500);
    alignY(1, error, 0);            // align to middle of field
    delay(500);
    turnToTheta(pi/4, pi/20);
    delay(500);
    turnToTheta(0, pi/20);
    delay(500);
    moveWithTime(pi/2, 0.5, 0, 3000);

    // alignX(1, error);
    // turnToTheta(0, pi/20);          // reset orientation again 

    delay(2000);

    if (getDistance() < 500) {      // if there is an obstacle there, know that there isn't one behind it
      midBlocked = true;
      Enes100.println("Middle path is blocked.");
    } else { Enes100.println("Middle path is open."); }

    delay(4000);

    // turnToTheta(pi/4, pi/20);       // turn to check for topfront obstacle
    alignY(1.5, error, 0);

    if (getDistance() < 1000) {
      topBlocked = true;
      Enes100.println("Top path is blocked.");
    } else { Enes100.println("Top path is open."); }

    delay(1000);

    // turnToTheta(0, pi/20);          // turn back to straight
    alignY(1.1, error, 0);

    if (midBlocked && topBlocked) {     // therefore, lower front is open and top back is open
      alignY(0.5, error, 0);
      moveUntilBlocked(80, 0.5);
      alignY(1.5, error, 0);
      moveUntilBlocked(150, 1);     // move through limbo
    } else if (midBlocked && topBlocked == false) {   // go through top, then through middle, and then through limbo
      alignY(1.5, error, 0);
      moveUntilBlocked(80, 0.5);
      alignY(1, error, 0);
      moveUntilBlocked(200, 1);
      alignY(1.5, error, 0);
      moveUntilBlocked(150, 1);
    } else if (midBlocked == false && topBlocked) {   // go through middle, then through top
      moveUntilBlocked(80, 0.5);
      alignY(1.5, error, 0);
      turnToTheta(pi/4, pi/20);
      turnToTheta(0, pi/20);
      moveUntilBlocked(120, 0.5);
    } 
    
  }
}

void loop() {

}
