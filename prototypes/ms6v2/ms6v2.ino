#include <Enes100.h>
#include <AFMotor.h>


AF_DCMotor motor1 (1); //Front left wheel
AF_DCMotor motor2 (2); //Front right wheel
AF_DCMotor motor3 (3); //Back left wheel
AF_DCMotor motor4 (4); //Back right wheel

// timer variables
unsigned long startTime = 0;
unsigned long moveTimer = 0;
int function_num = 0;

float pi = 3.1415926;
float timeFor90 = 6700/4;   // time it takes to rotate 90 degrees/ pi/2 rads
float timeFor2pi = 6700-1800;
float velX = 0.190;
float velY = 0.183;

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

  // print motor speeds
  // Serial.print("  Left Front: ");
  // Serial.print(leftFront);
  // Serial.print("  Right Front: ");
  // Serial.print(rightFront);
  // Serial.print("  Left Rear: ");
  // Serial.print(leftRear);
  // Serial.print("  Right Rear: ");
  // Serial.print(rightRear);

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

void setup() {
  // initialize serial port
  Serial.begin(9600);

  // WIFI
  // Enes100.begin("The Vedauntless", CRASH_SITE, 6, 51, 50);
  // Enes100.println("Connected...");


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
}


