#include "Enes100.h"
#include "movement.h"
// #include "sensors.h"

// void navToMission() {
//   bool atMission = false;
//   float x, y, t; bool v; // Declare variables to hold the data

//   // detect which side of map we're on
//   x = Enes100.getX();  // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
//   y = Enes100.getY();  // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
//   t = convertVisionTo2pi(Enes100.getTheta());  //Your theta! -pi to +pi, in radians, -1 if your aruco is not visible.
//   v = Enes100.isVisible(); // Is your aruco visible? True or False.
//   if (v) {
//     if ( y <= 2) {
//       turnToTheta(t, pi/2);
//     } else {
//       turnToTheta(t, 3*pi/2);
//     }
//   }

// }

void setup() {
  // moveWithTime(pi/2, 1, 0, 2000);
  Serial.begin(9600);

  // WIFI
  Enes100.begin("The Vedauntless", CRASH_SITE, 6, 51, 50);
  Enes100.println("Connected...");

  // // SENSORS
  // //Color setup
  // pinMode(S0, OUTPUT);
  // pinMode(S1, OUTPUT);
  // pinMode(S2, OUTPUT);
  // pinMode(S3, OUTPUT);
  // pinMode(sensorOut, INPUT);
  // // Setting frequency-scaling to 20%
  // digitalWrite(S0,HIGH);
  // digitalWrite(S1,LOW);
  // Serial.begin(9600);

  // //Distance setup
  // VL53L0X_Error Status = VL53L0X_ERROR_NONE;
  // SERIAL.begin(9600);
  // Status = VL53L0X.VL53L0X_common_init();
  // if (VL53L0X_ERROR_NONE != Status) {
  //     SERIAL.println("start vl53l0x mesurement failed!");
  //     VL53L0X.print_pal_error(Status);
  //     while (1);
  // }

  // VL53L0X.VL53L0X_high_accuracy_ranging_init();

  // if (VL53L0X_ERROR_NONE != Status) {
  //   SERIAL.println("start vl53l0x mesurement failed!");
  //   VL53L0X.print_pal_error(Status);
  //   while (1);
  // }

  // navToMission();

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

void loop() {
  
  moveWithTime(pi/2, 1, 0);


  // // Transmit the height of the payload in mm
  // Enes100.mission(HEIGHT, 270);
  // // Transmit the length of the payload in mm
  // Enes100.mission(LENGTH, 180);
  // // Transmit the direction of the abnormality for normal in the x direction.
  // Enes100.mission(DIRECTION, NORMAL_X);
  // delay(1000);

}
