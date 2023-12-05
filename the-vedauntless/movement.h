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
  leftFront *= 255 * leftFrontMult;
  rightFront *= 255 * rightFrontMult;
  leftRear *= 255 * leftRearMult;
  rightRear *= 255 * rightRearMult;

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
  moveTimer = millis();                   // set moveTimer variable to current time
  while ((millis() - moveTimer) <= mS) {  // while we haven't reached mS time
    move(theta, power, turn);             // move
  }

  // when timer has finished, stop moving
  move(0, 0, 0);
}

void turnToTheta(float theta, float error) {
  float t = convertVisionTo2pi(Enes100.getTheta());
  bool overcorrect = true;
  float dir = 0.5;

  if (t > (theta - error) && t < (theta + error)) { overcorrect = false; }  // if already aligned, then don't overcorrect
  if (abs(theta - t) > pi) { dir = -0.5; }                                  // optimize rotation
  while (t > (theta + error) || t < (theta - error)) {                      // if not aligned, rotate until aligned
    t = convertVisionTo2pi(Enes100.getTheta());
    move(0, 0, dir);
  }
  if (overcorrect) { moveWithTime(0, 0, -dir, (2 * error) * 1000 * batteryLevel / rotatePS); }  // overcorrect if needed
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

bool turnToMission(float totalTime, float error) {
  // assume already turned in the general direction of the mission.

  float startTime = millis();

  moveWithTime(pi, 0.5, 0, totalTime);          // move to the left
  while (getDistance() > error) { move(0, 0.5, 0); }
  moveWithTime(0, 0.5, 0, 200);
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

void moveUntilBlocked(float minDist, float power, float xVal = 4.5) {
  sForward();
  // in mm
  float dist = getDistance();
  while (dist < minDist && Enes100.getX() < xVal) {
    move(pi / 2, power, 0);
    dist = getDistance();
  }
  dist = getDistance();
  while (dist > minDist && Enes100.getX() < xVal) {
    move(pi / 2, power, 0);
    dist = getDistance();
  }
  move(0, 0, 0);
}
