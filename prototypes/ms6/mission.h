// NAVIGATION
void moveUntilBlocked(float minDist, float power) {
  // in mm

  float dist = getDistance();
  while (dist > minDist) {
    move(pi / 2, power, 0);
    dist = getDistance();
  }
  move(0, 0, 0);
}

void navToSite(float distance) {
  bool atMission = false;
  float x, y, t;
  bool v;  // Declare variables to hold the data

  // detect which side of map we're on
  x = Enes100.getX();                          // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
  y = Enes100.getY();                          // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
  t = convertVisionTo2pi(Enes100.getTheta());  // Your theta! Edited to be from 0 to 2pi, originally -pi to +pi, in radians, -1 if your aruco is not visible.
  v = Enes100.isVisible();                     // Is your aruco visible? True or False.

  Enes100.println(t);

  // moveWithTime(0, 0, 1, 1000);

  // face the other side
  if (v) {
    if (y <= 1) {
      // turnToTheta(t, pi / 2);
      while (t > (11*pi/20) || t < (9*pi/20)) {
        t = convertVisionTo2pi(Enes100.getTheta());
        move(0, 0, 0.5);
      }
      moveWithTime(0, 0, -0.5, (1.5*pi/12)*1000/rotatePS);
    } else {
      // turnToTheta(t, 3 * pi / 2);
      while (t > (31*pi/20) || t < (29*pi/20)) {
        t = convertVisionTo2pi(Enes100.getTheta());
        move(0, 0, 0.5);
      }
      moveWithTime(0, 0, -0.5, (1.5*pi/12)*1000/rotatePS);
    }
  }

  // move to certain distance away from crash site
  moveUntilBlocked(distance, 0.5);
}

void maintainDist(float dist) {
  moveUntilBlocked(dist, 1);              // move fast
  moveWithTime(3 * pi / 2, 0.5, 0, 500);  // back up
  moveUntilBlocked(dist, 0.5);            // then move slowly towards it
}

void turnToFace(float timerange, float error) {
  // timerange: strafe movement duration
  // error: mm of error allowed

  bool parallel = false;

  moveWithTime(pi, 0.5, 0, timerange/2); //center

  while (parallel == false) {
    float dist = getDistance();
    moveWithTime(0, 0.5, 0, timerange);  // move in parallel
    float newDist = getDistance();

    if ((abs(dist - newDist)) <= error) {
      parallel = true;
    } else {
      moveWithTime(pi, 0.5, 0, timerange);
      if ((newDist - dist) > 0) {   // rotate if not parallel
        moveWithTime(0, 0, -1, 100);
      } else {
        moveWithTime(0, 0, 1, 100);
      }
    }
  }

  moveWithTime(pi, 0.5, 0, timerange/2); // recenter
}

void moveToOtherFace() {
  moveWithTime(0, 1, 0, 500/strafePS);
  moveWithTime(0, 0, -1, (pi/2)/(rotatePS/1000));
  moveWithTime(0, 1, 0, 500/strafePS);
}

void alignMiddle() {
  float x, y, t;
  bool v;  // Declare variables to hold the data

  // detect which side of map we're on
  x = Enes100.getX();                          // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
  y = Enes100.getY();                          // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
  t = convertVisionTo2pi(Enes100.getTheta());  // Your theta! Edited to be from 0 to 2pi, originally -pi to +pi, in radians, -1 if your aruco is not visible.
  v = Enes100.isVisible();                     // Is your aruco visible? True or False.

  if (v) {
    while (t > (39*pi/20) || t < (1*pi/20)) {
      t = convertVisionTo2pi(Enes100.getTheta());
      move(0, 0, 0.5);
    }
    moveWithTime(0, 0, -0.5, (1.5*pi/12)*1000/rotatePS);

    moveWithTime(pi/2, 1, 0, 2000);

    while (y > 1.1) {
      y = Enes100.getY();                          // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
      move(0, 0.5, 0);
    }
    while (y < 0.9) {
      y = Enes100.getY();                          // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
      move(pi, 0.5, 0);
    }

    while (t > (39*pi/20) || t < (1*pi/20)) {
      t = convertVisionTo2pi(Enes100.getTheta());
      move(0, 0, 0.5);
    }
    moveWithTime(0, 0, -0.5, (1.5*pi/12)*1000/rotatePS);
  }
}

// CRASH SITE
int detectHeight(int angModifier = 0) {
  moveWithTime(pi, 0.5, 0, 500);

  for (int ang = 180; ang > (60-angModifier); ang--) {
    servo.write(ang);
    delay(30);
  }
  servo.detach();
  delay(3000);

  int angle = readPot();

  servo.attach(2);
  for (int ang = (60-angModifier); ang < 180; ang++) {
    servo.write(ang);
    delay(30);
  }
  servo.detach();
  delay(1000);

  return angle;
}

int detectLength(float error) {

  float startDist = getDistance();
  float currentDist = getDistance();

  while (abs(currentDist - startDist) < error) {  // strafe to the left until distance sensor reads a spike in readings -- the edge has been reached
    move(pi, 0.5, 0);                             // strafe to the left
    currentDist = getDistance();
  }

  moveWithTime(0, 0.5, 0, 200);  // move back into range of wall
  currentDist = getDistance();
  delay(1000);

  float startTime = millis();                     // record starting time
  while (abs(currentDist - startDist) < error) {  // strafe to the right until distance sensor reads a spike in readings -- the edge has been reached
    move(0, 0.5, 0);                              // strafe to the right
    currentDist = getDistance();
  }
  float endTime = millis();  // record ending time

  float deltaTime = endTime - startTime;

  moveWithTime(pi, 0.5, 0, 200);  // move back into range of wall

  return deltaTime * (strafePS / 2);  // return estimate of mm of length
}