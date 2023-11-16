void navToSite() {
  bool atMission = false;
  float x, y, t; bool v; // Declare variables to hold the data

  // detect which side of map we're on
  x = Enes100.getX();  // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
  y = Enes100.getY();  // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
  t = convertVisionTo2pi(Enes100.getTheta());  //Your theta! -pi to +pi, in radians, -1 if your aruco is not visible.
  v = Enes100.isVisible(); // Is your aruco visible? True or False.
  
  // face the other side
  if (v) {
    if ( y <= 2) {
      turnToTheta(t, pi/2);
    } else {
      turnToTheta(t, 3*pi/2);
    }
  }

  // move to mission in increments of 500 ms of movement
  while (atMission == false) {
    moveWithTime(pi/2, 1, 0, 500); // move forward for 500 ms
    if (getDistance() < 150) {
      atMission = true;            // if at mission, then stop
    }
  }
}

