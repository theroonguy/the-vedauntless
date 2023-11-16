#include "Enes100.h"
#include "movement.h"
#include "sensors.h"

void navToMission() {
  bool atMission = false;
  float x, y, t; bool v; // Declare variables to hold the data

  // detect which side of map we're on
  x = Enes100.getX();  // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
  y = Enes100.getY();  // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
  t = convertVisionTo2pi(Enes100.getTheta());  //Your theta! -pi to +pi, in radians, -1 if your aruco is not visible.
  v = Enes100.isVisible(); // Is your aruco visible? True or False.
  if (v) {
    if ( y <= 2) {
      turnToTheta(t, pi/2);
    } else {
      turnToTheta(t, 3*pi/2);
    }
  }

}

void setup() {
  // WIFI
  Enes100.begin("The Vedauntless", CRASH_SITE, 6, 51, 50);
  Enes100.println("Connected...");

  // SENSORS
  //Color setup
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.begin(9600);

  //Distance setup
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;
  SERIAL.begin(9600);
  Status = VL53L0X.VL53L0X_common_init();
  if (VL53L0X_ERROR_NONE != Status) {
      SERIAL.println("start vl53l0x mesurement failed!");
      VL53L0X.print_pal_error(Status);
      while (1);
  }

  VL53L0X.VL53L0X_high_accuracy_ranging_init();

  if (VL53L0X_ERROR_NONE != Status) {
    SERIAL.println("start vl53l0x mesurement failed!");
    VL53L0X.print_pal_error(Status);
    while (1);
  }

  navToMission();

}

void loop() {
  
  // Transmit the height of the payload in mm
  Enes100.mission(HEIGHT, 270);
  // Transmit the length of the payload in mm
  Enes100.mission(LENGTH, 180);
  // Transmit the direction of the abnormality for normal in the x direction.
  Enes100.mission(DIRECTION, NORMAL_X);
  delay(1000);

}
