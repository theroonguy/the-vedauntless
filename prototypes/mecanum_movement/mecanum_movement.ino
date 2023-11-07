#include <AFMotor.h>

AF_DCMotor motor1 (1); //Front left wheel
AF_DCMotor motor2 (2); //Front right wheel
AF_DCMotor motor3 (3); //Back left wheel
AF_DCMotor motor4 (4); //Back right wheel

unsigned long startTime = 0;
int function_num = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //goForward(100, 1);
  //movementtest(90, 255, 0);
  // simple_movement(0,0,255); //speed: 0-255, turn: 0-1, strafe: ?-?
  // delay(1000);

  float pi = 3.1415926;
  float power = 1;

  // depending on what the function number is, continuously run that function
  if (function_num == 0) {
    move(pi/2, power, 0); // should move forward
  } else if (function_num == 1) {
    move(0, power, 0); // should move right
  } else if (function_num == 2) {
    move(-pi/2, power, 0); // should move backward
  } else if (function_num == 3) {
    move(pi, power, 0); // should move left
  } else if (function_num == 4) {
    move(0, 0, power); // should rotate right
  } else if (function_num == 5) {
    move(0, 0, -power); // should rotate left
  } else if (function_num == 6) {
    move(5*pi/4, 1, 1);
  } else if (function_num > 8) {
    move(0, 0, 0);
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
  if (leftFront > 0) {
    motor1.run(FORWARD);
  } else {
    motor1.run(BACKWARD);
    leftFront = -leftFront;
  }
  motor1.setSpeed(int(leftFront));

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

}


//////// OLD //////////////

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

