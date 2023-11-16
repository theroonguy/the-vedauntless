#include <Enes100.h>
#include "movement.h"

void setup() {
  // initialize serial port
  Serial.begin(9600);

  // WIFI
  // Enes100.begin("The Vedauntless", CRASH_SITE, 6, 51, 50);
  // Serial.println("beginning");
  // Enes100.println("Connected...");
  // Serial.println("connected");


  moveWithTime(pi/2, 1, 0, 1000);

}

void loop() {
  float x, y, t; bool v; // Declare variables to hold the data
  //Enes100.getX will make sure you get the latest data available to you about your OTV's location.
  //The first time getX is called, X, Y, theta and visibility are queried and cached.
  //Subsequent calls return from the cache, so there is no performance gain to saving the function response to a variable.

  // x = Enes100.getX();  // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
  // y = Enes100.getY();  // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
  // t = Enes100.getTheta();  //Your theta! -pi to +pi, in radians, -1 if your aruco is not visible.
  // v = Enes100.isVisible(); // Is your aruco visible? True or False.

  // if (v) // If the ArUco marker is visible
  // {
  //     Enes100.print(x); // print out the location
  //     Enes100.print(",");
  //     Enes100.print(y);
  //     Enes100.print(",");
  //     Enes100.println(t);
  // } else { // otherwise
  //     Enes100.println("Not visible"); // print not visible
  // }

  // motor1.setSpeed(255);
  // motor1.run(FORWARD);

  move(1, 1, 0);
  // move2(1, 1, 0);

  // Serial.println("hi");

}


