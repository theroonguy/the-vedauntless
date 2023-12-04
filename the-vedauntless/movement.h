#include <AFMotor.h>
#define abs(x) ((x) > 0 ? (x) : -(x))

AF_DCMotor motor1(1);  //Front left wheel
AF_DCMotor motor2(2);  //Front right wheel
AF_DCMotor motor3(3);  //Back left wheel
AF_DCMotor motor4(4);  //Back right wheel

// timer variables
unsigned long startTime = 0;
unsigned long moveTimer = 0;

float pi = 3.1415926;

float strafePS = 0.190;
float normalPS = 0.183;
float rotatePS = 0.937;

float leftFrontMult = 1;
float rightFrontMult = 1;
float leftRearMult = 1;
float rightRearMult = 1;

// CALCULATIONS
float timeForAngle(float theta) {
  return theta / rotatePS;
}

float convertVisionTo2pi(float theta) {
  float thetaFinal = 0;

  if (theta <= pi && theta >= 0) {
    thetaFinal = theta;
  } else if (theta < 0 && theta >= -pi) {
    thetaFinal = 2 * pi + theta;
  }

  return thetaFinal;
}

// MOVE FUNCTIONS
void move(float theta, float power, float turn) {
  // define variables for calculations
  float sinV = sin(theta - pi / 4);
  float cosV = cos(theta - pi / 4);
  float maxval = max(abs(sinV), abs(cosV));

  // calculate motor power levels
  float leftFront = power * cosV / maxval + turn;
  float rightFront = power * sinV / maxval - turn;
  float leftRear = power * sinV / maxval + turn;
  float rightRear = power * cosV / maxval - turn;

  // normalize motor values if they exceed max power
  if ((power + abs(turn)) > 1) {
    leftFront /= power + turn;
    rightFront /= power + turn;
    leftRear /= power + turn;
    rightRear /= power + turn;
  }

  // multiply final values by 255 to get max speed
  leftFront *= 255*leftFrontMult;
  rightFront *= 255*rightFrontMult;
  leftRear *= 255*leftRearMult;
  rightRear *= 255*rightRearMult;

  // run motors at proper speeds
  motor1.setSpeed(abs(int(leftFront)));
  if (leftFront > 0) {
    motor1.run(FORWARD);
  } else {
    motor1.run(BACKWARD);
  }

  motor2.setSpeed(abs(int(rightFront)));
  if (rightFront > 0) {
    motor2.run(FORWARD);
  } else {
    motor2.run(BACKWARD);
  }

  motor3.setSpeed(abs(int(leftRear)));
  if (leftRear > 0) {
    motor3.run(FORWARD);
  } else {
    motor3.run(BACKWARD);
  }

  motor4.setSpeed(abs(int(rightRear)));
  if (rightRear > 0) {
    motor4.run(FORWARD);
  } else {
    motor4.run(BACKWARD);
  }

  // print motor speeds
  Serial.print("  Left Front: ");
  Serial.print(leftFront);
  Serial.print("  Right Front: ");
  Serial.print(rightFront);
  Serial.print("  Left Rear: ");
  Serial.print(leftRear);
  Serial.print("  Right Rear: ");
  Serial.print(rightRear);
  Serial.println("");
}

void moveWithTime(float theta, float power, float turn, float mS) {
  // function to move with a certain theta, power, and turn for a specific amount of time

  // start timer
  moveTimer = millis();  // set moveTimer variable to current time

  Serial.println("Moving started...");

  while ((millis() - moveTimer) <= mS) {  // while we haven't reached mS time
    move(theta, power, turn);             // move
  }

  // when timer has finished, stop moving
  move(0, 0, 0);
  Serial.println("Moving stopped.");

  // if ((millis() - moveTimer) > mS) {       // when timer has finished
  //   move(0, 0, 0);      // stop moving
  // }
}

void turnToTheta(float theta, float error) {
  float t = convertVisionTo2pi(Enes100.getTheta());
  bool overcorrect = true;
  float dir = 0.5;

  if (t > (theta-error) && t < (theta+error)) { overcorrect = false; }    // if already aligned, then don't overcorrect
  if (abs(theta - t) > pi) { dir = -0.5; }                                // optimize rotation
  while (t > (theta+error) || t < (theta-error)) {                        // if not aligned, rotate until aligned
    t = convertVisionTo2pi(Enes100.getTheta());
    move(0, 0, dir);
  }
  if (overcorrect) { moveWithTime(0, 0, -dir, (2*error)*1000*batteryLevel/rotatePS); } // overcorrect if needed
}

void signal() {
  moveWithTime(0, 0, 1, 100);
  moveWithTime(0, 0, -1, 100);
}

void sForward() {
  leftFrontMult = 0.84;
  rightFrontMult = 1;
  leftRearMult = 0.84;
  rightRearMult = 1;
}

void sStrafe() {
  leftFrontMult = 0.84;
  rightFrontMult = 0.84;
  leftRearMult = 1;
  rightRearMult = 1;
}

// NAVIGATION
void navigateObstacles(float speed) {

  // nav settings    
  int strafeDist = 300;       // (mm) distance to strafe
  int tooClose = 100;         // (mm) distance to start backing up
  int backUpTime = 400;       // (mS) duration of back up movement
  int clearTime = 2000;       // (mS) time to clear an obstacle
  int rotateTime = 100;       // (mS) duration of rotation correction
  float yBoundary = 0.4;      // (m) boundary to not exceed from top and bottom
  float xBoundary = 3.5;      // (m) end of course, when to stop navigating
  float thetaRange = pi/12;   // (rad) allowed range of theta

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
      moveWithTime(dir, speed, 0, clearTime/speed);     // clear the obstacle
      delay(2000);
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