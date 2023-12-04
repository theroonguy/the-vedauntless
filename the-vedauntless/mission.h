// NAVIGATION
void moveUntilBlocked(float minDist, float power, float xVal = 4) {
  sForward();
  // in mm

  float dist = getDistance();
  while (dist > minDist && Enes100.getX() < xVal) {
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

  // moveWithTime(0, 0, 1, 1000);

  // face the other side
  if (v) {
    if (y <= 1) {
      turnToTheta(pi / 2, pi / 20);
    } else {
      turnToTheta(3 * pi / 2, pi / 20);
    }
  }
  moveWithTime(pi / 2, .5, 0, 3500);
  turnToMission(1500, 300);

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

  moveWithTime(pi, 0.5, 0, timerange / 2);  //center

  while (parallel == false) {
    float dist = getDistance();
    moveWithTime(0, 0.5, 0, timerange);  // move in parallel
    float newDist = getDistance();

    if ((abs(dist - newDist)) <= error) {
      parallel = true;
    } else {
      moveWithTime(pi, 0.5, 0, timerange);
      if ((newDist - dist) > 0) {  // rotate if not parallel
        moveWithTime(0, 0, -1, 100);
      } else {
        moveWithTime(0, 0, 1, 100);
      }
    }
  }

  moveWithTime(pi, 0.5, 0, timerange / 2);  // recenter
}

void moveToOtherFace() {
  moveWithTime(0, 1, 0, 500 / strafePS);
  moveWithTime(0, 0, -1, (pi / 2) / (rotatePS / 1000));
  moveWithTime(0, 1, 0, 500 / strafePS);
}

void alignY(float yVal, float error, float dir) {
  sStrafe();

  float y = Enes100.getY();
  // float overcorrect = 2000;

  if (y < (yVal - error)) {  // if below the target y
    while (y < (yVal - error)) {
      y = Enes100.getY();
      move(pi, speed, 0);
    }
    // moveWithTime(0, 0.5, 0, overcorrect);
  } else if (y > (yVal + error)) {  // if above target y
    while (y > (yVal + error)) {
      y = Enes100.getY();
      move(0, speed, 0);
    }
    // moveWithTime(pi, 0.5, 0, overcorrect);
  }

  // if (y > (yVal-error)){
  //   while (y > (yVal-error)) {
  //   y = Enes100.getY();
  //   move(0, 0.5, 0);

  // }
  // } else if (y < (yVal+error)) {
  //   while (y < (yVal+error)) {
  //     y = Enes100.getY();
  //     move(pi, 0.5, 0);
  //   }
  // }

  // turnToTheta(dir, pi/20);    // ensure facing right direction after move
}

void alignX(float xVal, float error) {
  float x = Enes100.getX();

  while (x > (xVal + error)) {
    x = Enes100.getX();
    move(3 * pi / 2, 0.5, 0);
  }
  while (x < (xVal + error)) {
    x = Enes100.getX();
    move(pi / 2, 0.5, 0);
  }
}

// CRASH SITE
int detectHeight(int angModifier = 0) {
  moveWithTime(pi, 0.5, 0, 500);

  for (int ang = 180; ang > (60 - angModifier); ang--) {
    servo.write(ang);
    delay(30);
  }
  servo.detach();
  delay(3000);

  int angle = readPot();

  servo.attach(2);
  for (int ang = (60 - angModifier); ang < 180; ang++) {
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

  float xI = Enes100.getX();
  float yI = Enes100.getY();
  //float startTime = millis();                     // record starting time
  while (abs(currentDist - startDist) < error) {  // strafe to the right until distance sensor reads a spike in readings -- the edge has been reached
    move(0, 0.5, 0);                              // strafe to the right
    currentDist = getDistance();
  }
  float xF = Enes100.getX();
  float yF = Enes100.getY();
  //float endTime = millis();  // record ending time

  //float deltaTime = endTime - startTime;

  moveWithTime(pi, 0.5, 0, 200);  // move back into range of wall

  return int(1000 * norm(xI, yI, xF, yF));  // return estimate of mm of length
}

void findPath(float error) {
  float speed = 1;

  float startDist = getDistance();
  float currentDist = getDistance();

  if (Enes100.getY() > 1.25 && getDistance() < 300 && getDistance() > 100) {                             //If on upper path
    while (getDistance() < 300 && Enes100.getY() > .4 && getDistance() > 100) {  //MAYBE SHOULD BE 5????
      move(0, speed, 0);
    }
    if ( Enes100.getY() > 0.5 ) {
      moveWithTime(0, speed, 0, 2000 / speed);
    }
    delay(2000);
  }
  if (Enes100.getY() <= 1.25 && getDistance() < 300 && getDistance() > 100) {
    while (getDistance() < 300 && Enes100.getY() < 1.6 && getDistance() > 100) {
      move(pi, speed, 0);
    }
    if ( Enes100.getY() < 1.5) {
      moveWithTime(pi, speed, 0, 2000 / speed);
    }
    delay(2000);
  }
  if (getDistance() < 120) {                 // back up if too close
    moveWithTime(3*pi/2, speed, 0, 400);
    // turnToTheta(pi/4, 0.1);
    // turnToTheta(0, 0.1);
    turnToFace(250, 10);
  }
}

int anomalyLogic(int height, int length) {
  if (height == 270) {
    if (length == 180) {
      length = 135;
    } else if (length == 135) {
      length = 180;
    } else {
      Enes100.print("Dimension logic error");
    }
  }
  if (height == 180) {
    if (length == 270) {
      length = 135;
    } else if (length == 135) {
      length = 270;
    } else {
      Enes100.print("Dimension logic error");
    }
  }
  if (height == 135) {
    if (length == 180) {
      length = 270;
    } else if (length == 270) {
      length = 180;
    } else {
      Enes100.print("Dimension logic error");
    }
  }
  return length;
}

// NAVIGATION
void navigateObstacles(float speed) {

  // nav settings    
  int strafeDist = 200;       // (mm) distance to strafe
  int tooClose = 100;         // (mm) distance to start backing up
  int backUpTime = 400;       // (mS) duration of back up movement
  int clearTime = 1000;       // (mS) time to clear an obstacle
  int rotateTime = 100;       // (mS) duration of rotation correction
  float yBoundary = 0.5;      // (m) boundary to not exceed from top and bottom
  float xBoundary = 3.5;      // (m) end of course, when to stop navigating
  float thetaRange = pi/16;   // (rad) allowed range of theta

  float dir = pi;    // direction of sideways movement -- either pi or 0
  bool aligning = false;
  bool clearedOb = false;
  bool navigate = true;

  while (navigate) {
    // this while loop runs extremely fast. every time it runs, it prioritizes not running into anything, then aligning rotation, then clearing obstacles, and lastly moving forward.

    float y = Enes100.getY();
    float x = Enes100.getX();
    float t = Enes100.getTheta();
    float dist = getDistance();

    // keep distance alignment
    if (dist < tooClose) {                        // if too close
      move(3*pi/2, speed, 0);                       // back up
      continue;
    }
    
    // theta alignment
    if (t > thetaRange) {                         // if above max theta range
      // move(0, 0, 0.5);
      moveWithTime(0, 0, speed/2, rotateTime);
      continue;
    } else if (t < -thetaRange) {                 // if below min theta range 
      // move(0, 0, -0.5);
      moveWithTime(0, 0, -speed/2, rotateTime);
      continue;
    }

    // y alignment
    if (y > (2-yBoundary)) {                      // if reaches highest position
      dir = 0;                                      // switch direction to going downwards
      move(0, speed, 0);                            // move downwards
    } else if (y < yBoundary) {                   // if reaches lowest...
      dir = pi;                                     // switch direction to going upwards
      move(pi, speed, 0);                           // move upwards
    }

    // if no alignments needed, but blocked, then strafe
    if (dist < strafeDist && dist > tooClose) {   // if blocked
      sStrafe();                                    // set strafe wheel speeds
      move(dir, speed, 0);                          // move in direction -- either up or down
      clearedOb = false;                            // note that there is an obstacle ahead
    }

    // if not detecting an obstacle anymore
    if (dist > strafeDist && clearedOb == false) {    // if we have cleared the edge of an obstacle
      sStrafe();
      Enes100.println("Cleared obstacle");
      moveWithTime(dir, speed, 0, clearTime/speed);     // clear the obstacle
      delay(clearTime/speed);
      clearedOb = true;                                 // note that there is no longer an obstacle ahead
    }

    // if cleared obstacle, then move forward
    if (clearedOb == true) {                          // if there is no obstacle ahead..
      sForward();
      move(pi/2, speed, 0);                             // move forwards
    }

    // once reached the end of the arena, stop navigating
    if (x > xBoundary) {
      navigate = false;
    }

  }

  // ending sequence
  moveUntilBlocked(200, speed);
  servo.attach(2);
  for (int ang = 180; ang > (60); ang--) {
    servo.write(ang);
    delay(30);
  }
  servo.detach();
}