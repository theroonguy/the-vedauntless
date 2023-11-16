#include <AFMotor.h>

AF_DCMotor motor1 (1); //Front left wheel
AF_DCMotor motor2 (2); //Front right wheel
AF_DCMotor motor3 (3); //Back left wheel
AF_DCMotor motor4 (4); //Back right wheel

// timer variables
unsigned long startTime = 0;
unsigned long moveTimer = 0;

float pi = 3.1415926;
float timeFor90 = 6700/4;   // time it takes to rotate 90 degrees/ pi/2 rads
float timeFor2pi = 6700-1800;
float velX = 0.190;
float velY = 0.183;

float timeFromX(float meters) {
  return meters/velX;
}

float timeFromY(float meters) {
  return meters/velY * 1000;
}

void moveXY(float X, float Y) {

  float theta = atan(Y/X); // calculate angle
  float timeX = timeFromX(X);
  float timeY = timeFromY(Y);
  float time = sqrt(sq(timeY)+sq(timeX)); // calculate total time

  moveWithTime(theta, 1, 0, time);
}

float timeForAngle(float theta) {
  return theta * timeFor2pi/(2*pi);
}

void move(float theta, float power, float turn) {
  // define variables for calculations
  float pi = 3.1415926;
  float sinV = sin(theta - pi/4);
  float cosV = cos(theta - pi/4);
  float maxval = max(abs(sinV),abs(cosV));

  // calculate motor power levels
  float leftFront   = power * cosV/maxval + turn;
  float rightFront  = power * sinV/maxval - turn;
  float leftRear    = power * sinV/maxval + turn;
  float rightRear   = power * cosV/maxval - turn;

  // normalize motor values if they exceed max power
  if ((power + abs(turn)) > 1) {
    leftFront /= power + turn;
    rightFront /= power + turn;
    leftRear /= power + turn;
    rightRear /= power + turn;
  }

  // multiply final values by 255 to get max speed
  leftFront *= 255;
  rightFront *= 255;
  leftRear *= 255;
  rightRear *= 255;

  // run motors at proper speeds
  motor1.setSpeed(abs(int(leftFront)));
  if (leftFront > 0) {
    motor1.run(FORWARD);
  } else {
    motor1.run(BACKWARD);
    leftFront = -leftFront;
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
}

void moveWithTime(float theta, float power, float turn, float mS) {
  // function to move with a certain theta, power, and turn for a specific amount of time

  // start timer
  moveTimer = millis();   // set moveTimer variable to current time

  while ((millis() - moveTimer) <= mS) {  // while we haven't reached mS time
    move(theta, power, turn);  // move
  }

  // when timer has finished, stop moving
  move(0, 0, 0);

  // if ((millis() - moveTimer) > mS) {       // when timer has finished
  //   move(0, 0, 0);      // stop moving
  // }

}

float convertVisionTo2pi(float theta) {
  float thetaFinal = 0;
  
  if (theta <= pi && theta >= 0) {
    thetaFinal = theta;
  } else if (theta < 0 && theta >= -pi) {
    thetaFinal = 2*pi + theta;
  }

  return thetaFinal;
}

void turnToTheta(float thetaInital, float thetaFinal) {
  int turn = 0;

  if (thetaInitial > thetaFinal) {
    turn = 1;
  } else {
    turn = -1;
  }

  moveWithTime(0, 0, turn, timeForAngle(abs(thetaFinal - thetaInitial)));
}
