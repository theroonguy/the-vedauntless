void navToSite() {
  bool atMission = false;
  float x, y, t;
  bool v;  // Declare variables to hold the data

  // detect which side of map we're on
  x = Enes100.getX();                          // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
  y = Enes100.getY();                          // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
  t = convertVisionTo2pi(Enes100.getTheta());  //Your theta! -pi to +pi, in radians, -1 if your aruco is not visible.
  v = Enes100.isVisible();                     // Is your aruco visible? True or False.

  // face the other side
  if (v) {
    if (y <= 2) {
      turnToTheta(t, pi / 2);
    } else {
      turnToTheta(t, 3 * pi / 2);
    }
  }

  // move to mission in increments of 500 ms of movement
  while (atMission == false) {
    moveWithTime(pi / 2, 1, 0, 500);  // move forward for 500 ms
    if (getDistance() < 150) {
      atMission = true;  // if at mission, then stop
    }
  }
}

void moveUntilBlocked(float minDist, float power) {

  float dist = getDistance();
  while (dist > minDist) {
    move(pi / 2, power, 0);
    dist = getDistance();
  }

}

void maintainDist(float dist) {
  moveUntilBlocked(dist, 1);              // move fast
  moveWithTime(3 * pi / 2, 0.5, 0, 500);  // back up
  moveUntilBlocked(dist, 0.5);            // then move slowly towards it
}

void turnToFace(float timerange, float error) {
  bool parallel = false;

  while (parallel == false) {
    float dist = getDistance();
    moveWithTime(0, 0.5, 0, timerange);     // move in parallel
    float newDist = getDistance();

    if ((abs(dist - newDist)) <= error) {
      parallel = true;
    } else {
      moveWithTime(pi, 0.5, 0, timerange);
      if ((newDist - dist) > 0) {
        moveWithTime(0, 0, -1, 100);
      } else {
        moveWithTime(0, 0, 1, 100);
      }
      
    }
  }

}