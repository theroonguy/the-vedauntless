#include <AFMotor.h>

AF_DCMotor motor1 (1); //Front left wheel
AF_DCMotor motor2 (2); //Front right wheel
AF_DCMotor motor3 (3); //Back left wheel
AF_DCMotor motor4 (4); //Back right wheel

void setup() {
}

void loop() {
  //goForward(100, 1);
  //movementtest(90, 255, 0);
  simple_movement(0,0,255); //speed: 0-255, turn: 0-1, strafe: ?-?
  delay(1000);
}

//Low-level functions
void runMotor(float value, int motor) {
  if (motor == 1) { //check for which motor is being called
    motor1.setSpeed(int(abs(value))); //set speed to input speed
    if (value>0) { //determine whether to run motor forward or backward
      motor1.run(FORWARD);
    }
    else {
      motor1.run(BACKWARD);
    }
  }
  if (motor == 2) {
    motor2.setSpeed(int(abs(value)));
    if (value>0) {
      motor2.run(FORWARD);
    }
    else {
      motor2.run(BACKWARD);
    }
  }
  if (motor == 3) {
    motor3.setSpeed(int(abs(value)));
    if (value>0) {
      motor3.run(FORWARD);
    }
    else {
      motor3.run(BACKWARD);
    }
  }
  if (motor == 4) {
    motor4.setSpeed(int(abs(value)));
    if (value>0) {
      motor4.run(FORWARD);
    }
    else {
      motor4.run(BACKWARD);
    }
  }
}

void simple_movement(int speed, int turn, int strafe) {
  float frontLeft = speed + turn + strafe;
  float frontRight = speed - turn - strafe;
  float rearLeft = speed + turn - strafe;
  float rearRight = speed - turn + strafe;

  runMotor(frontLeft,1);
  runMotor(frontRight,2);
  runMotor(rearLeft,3);
  runMotor(rearRight,4);
}

//High-level functions
void goForward(float speed_percent, float seconds) { //input PERCENTAGE of 100%, time in SECONDS
  int en_value = 255*(speed_percent/100); //convert percentage to 0-255 value
  float milliseconds = seconds*1000; //convert seconds to milliseconds
  simple_movement(en_value, 0, 0);
  delay(milliseconds); //run for desired time
}

void goBackward(float speed_percent, float seconds) { //input PERCENTAGE of 100%, time in SECONDS
  int en_value = 255*(speed_percent/100); //convert percentage to 0-255 value
  float milliseconds = seconds*1000; //convert seconds to milliseconds
  simple_movement(en_value, 0, 0);
  delay(milliseconds); //run for desired time
}



//OLD
void movementtest(int theta, int power, int turn) {
  //input: theta, power, and turn
  float pi = 3.1415926;
  float sinValue = sin(theta - pi/4);
  float cosValue = cos(theta - pi/4);
  float max = max(abs(sinValue),abs(cosValue));

  //how do I calculate power and turn??
  float motor1power = int(power * cosValue/max + turn);
  float motor2power = power * sinValue/max + turn;
  float motor3power = power * sinValue/max + turn;
  float motor4power = power * cosValue/max + turn;

  Serial.print(" ");
  Serial.print(motor1power);
  Serial.print(motor2power);
  Serial.print(motor3power);
  Serial.print(motor4power);


  // if ((power + abs(turn)) > 1) {
  //   motor1power /= power + turn;
  //   motor2power /= power + turn;
  //   motor3power /= power + turn;
  //   motor4power /= power + turn;
  // }

  Serial.print(motor1power);

  motor1.setSpeed(int(motor1power));
  motor1.run(FORWARD);

  motor2.setSpeed(int(motor2power));
  motor2.run(FORWARD);

  motor3.setSpeed(int(motor3power));
  motor3.run(FORWARD);

  motor4.setSpeed(int(motor4power));
  motor4.run(FORWARD);
  delay(1000);
}