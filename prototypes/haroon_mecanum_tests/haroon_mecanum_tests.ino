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


void moveInCircle(float totalTime) {
  moveTimer = millis();       //start timer

  float elapsedTime = 0;      // time since movement started
  float angle = 0;            // initial angle
  float deltaAngle = (2*pi)/totalTime; //deltaAngle is change in angle per millisecond

  while (elapsedTime <= totalTime) {
    elapsedTime = (millis() - moveTimer); // elapsed time is number of milliseconds since movement started
    angle = elapsedTime*deltaAngle;  // set angle to percentage of 2pi based on elapsed time
    move(angle, 1, 0);    // move at angle
  }
  
  move(0, 0, 0);      // end movement
}

void moveAndRotate(float theta, float speed, float rotDiv) {

  float trajectory = theta;
  
  float deltaAngle = 2*pi/rotDiv;
  float deltaTime = timeForAngle(deltaAngle);
   

  while ((trajectory - 2*pi) <= theta) {
    moveWithTime(trajectory, 1, 0, speed);      // move forward
    moveWithTime(0, 0, 1, deltaTime);               // rotate

    trajectory += deltaAngle;
  }

  move(0, 0, 0);

}

float timeForAngle(float theta) {
  return theta * timeFor2pi/(2*pi);
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

void demonstration(float interval) {
  moveWithTime(pi/2, 1, 0, interval);
  moveWithTime(0, 1, 0, interval);
  moveWithTime(-pi/2, 1, 0, interval);
  moveWithTime(pi, 1, 0, interval);
  moveWithTime(0, 0, 1, timeFor90);
  moveWithTime(0, 0, -1, timeFor90*2);
  moveWithTime(pi/4, 1, 0, interval);
  moveWithTime(3*pi/4, 1, 0, interval);
  moveWithTime(5*pi/4, 1, 0, interval);
  moveWithTime(7*pi/4, 1, 0, interval);
  moveInCircle(10000);
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
  // Serial.begin(9600);

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

  ////////// TEST 1 -- MOVE IN CIRCLE
  // moveInCircle(10000);

  ////////// TEST 2 -- MOVE WHILE ROTATING
  // moveAndRotate(pi/2, 250, 64);

  ////////// TEST 3 -- FIND VELOCITIES
  // moveWithTime(pi/2, 1, 0, timeFromY(0.5));

  ////////// TEST 4 -- MOVE XY
  // moveXY(0.5, 0.5);

  ////////// TEST 5 -- DEMONSTRATION
  // demonstration(1000);
  // moveInCircle(10000);


  pinMode(41, OUTPUT);
}

void loop() {
  digitalWrite(41, HIGH);
}


