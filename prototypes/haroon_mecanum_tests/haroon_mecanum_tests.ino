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

void setup() {
  // initialize serial port
  Serial.begin(9600);

  // turn for time it takes to turn 90 degrees
  // moveWithTime(0, 0, 1, timeFor90);

  // test with hardcoded rotate with trajectory function
  // moveWithTime(4*pi/8, 1, 0, timeFor90/4);
  // moveWithTime(0, 0, 1, timeFor90/4);
  // moveWithTime(5*pi/8, 1, 0, timeFor90/4);
  // moveWithTime(0, 0, 1, timeFor90/4);
  // moveWithTime(6*pi/8, 1, 0, timeFor90/4);
  // moveWithTime(0, 0, 1, timeFor90/4);
  // moveWithTime(7*pi/8, 1, 0, timeFor90/4);
  // moveWithTime(0, 0, 1, timeFor90/4);
  // moveWithTime(8*pi/8, 1, 0, timeFor90/4);

  // moveWithTime(pi/2, 1, 1, timeFor90);

  // ang vel is (pi/2) * (milliseconds to turn 90 degrees)

  // rotateWithTrajectory(pi/2, 1, 1, 10000);
  // moveWithTime(0, 1, 1, timeFor90/2); // rotates to 45 degrees
  // moveWithTime(3*pi/4, 1, );
  // moveWithTrajectory2(pi/2, 1, pi/4, 5000);

  moveInCircle(10000);
}

void moveInCircle(float totalTime) {
  moveTimer = millis();       //start timer

  float elapsedTime = 0;      // time since movement started
  float angle = 0;            // initial angle
  float deltaAngle = (2pi)/totalTime; //deltaAngle is change in angle per millisecond

  while (elapsedTime <= totalTime) {
    elapsedTime = (millis() - moveTimer); // elapsed time is number of milliseconds since movement started
    angle = elapsedTime*deltaAngle;  // set angle to percentage of 2pi based on elapsed time
    move(angle, 1, 0);    // move at angle
  }
  
  move(0, 0, 0);      // end movement
}

void moveInCircleFacingInwards(float breadth, float totalTime) {
  moveTimer = millis();

  while ((millis() - moveTimer) <= totalTime) {
    move(0, 1, 0); // always move perpendicular
    move(0, 0, 1);
  }

  move(0, 0, 0);
}

void moveWithTrajectory2(float theta, float power, float angle, float totalTime) {
  moveTimer = millis();

  float ang_vel = (pi/2)/timeFor90;

  while ((millis() - moveTimer) <= totalTime) {
    moveWithTime(theta, 1, 0, (angle*2*timeFor90/pi));
    moveWithTime(0, 0, 1, (angle*2*timeFor90/pi)); //rotate certain angle
    theta -= angle;
  }

  move(0, 0, 0);
}

void demonstration() {
  // initialize variables
  float power = 1;

  // depending on what the function number is, continuously run that function
  if (function_num == 0) {
    move(pi/2, power, 0);       // move forward
  } else if (function_num == 1) {
    move(0, power, 0);          // move right
  } else if (function_num == 2) {
    move(-pi/2, power, 0);      // move backward
  } else if (function_num == 3) {
    move(pi, power, 0);         // move left
  } else if (function_num == 4) {
    move(0, 0, power);          // rotate right
  } else if (function_num == 5) {
    move(0, 0, -power);         // rotate left
  } else if (function_num == 6) {
    move(pi/4, 1, 0);           // northeast
  } else if (function_num == 7) {
    move(3*pi/4, 1, 0);         // northwest
  } else if (function_num == 8) {
    move(5*pi/4, 1, 0);         // southwest
  } else if (function_num == 9) {
    move(7*pi/4, 1, 0);         // southeast
  } else if (function_num == 10) {
    move(0, 0, 0);              // stop moving
  }

  if (millis() - startTime >= 1000) { // when 1 second has passed
    function_num += 1;                // increase function num
    startTime = millis();             // and restart timer
  }
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

void rotateWithTrajectory(float theta, float power, float turn, float mS) {
  // start timer
  startTime = millis();
  moveTimer = millis();

  float trajectory = theta;
  float delta_ang_vel = 0;

  while ((millis() - moveTimer) <= mS) {  // while we haven't reached mS time
    move(trajectory, power, turn);  // move
    delta_ang_vel = (((millis() - startTime) / 1000) * (pi/2)/timeFor90) * turn; // calculate ang velocity in opposite direction to cancel out rotation
    Serial.println(trajectory);
    trajectory += delta_ang_vel; // change trajectory
  }

  // when timer has finished, stop moving
  move(0, 0, 0);
}
