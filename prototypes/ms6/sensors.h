// pot: analog0
// servo: digital2

#define S0 45
#define S1 43
#define S2 41
#define S3 39
#define sensorOut 37

int potentiometer = A10;

#include <Servo.h>
Servo servo;

int frequency = 0;
int R = 0;
int G = 0;
int B = 0;

//Distance sensor
#include "Seeed_vl53l0x.h"
Seeed_vl53l0x VL53L0X;

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif

void initColorSensor() {
  Serial.println("Initializing sensors...");

  //Color setup
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.println("Color sensors set up.");
}

void initDistSensor() {
  //Distance setup
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;
  Status = VL53L0X.VL53L0X_common_init();
  if (VL53L0X_ERROR_NONE != Status) {
    SERIAL.println("start vl53l0x mesurement failed!");
    VL53L0X.print_pal_error(Status);
    while (1)
      ;
  }

  VL53L0X.VL53L0X_long_distance_ranging_init();

  if (VL53L0X_ERROR_NONE != Status) {
    SERIAL.println("start vl53l0x mesurement failed!");
    VL53L0X.print_pal_error(Status);
    while (1)
      ;
  }
}

void initPot() {
  pinMode(potentiometer, INPUT);
}

void initServo() {
  servo.attach(2);
  servo.write(180); //reset
  // servo.detach();
  // delay(1000); //let fall
  // servo.attach(2);
}

void detectAnomaly() {
  //Red
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  R = pulseIn(sensorOut, LOW);
  delay(100);

  //Green
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  G = pulseIn(sensorOut, LOW);
  delay(100);

  //Blue
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  B = pulseIn(sensorOut, LOW);
  delay(100);

  //Print results
  if (G - R > 40 && B - R > 50) {
    Serial.print("You ARE at the anomaly!\n");
  } else {
    Serial.print("You are NOT at the anomaly!\n");
  }
}

void findDistance() {
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;
  memset(&RangingMeasurementData, 0, sizeof(VL53L0X_RangingMeasurementData_t));
  Status = VL53L0X.PerformSingleRangingMeasurement(&RangingMeasurementData);
  SERIAL.print("Distance: ");
  SERIAL.print(RangingMeasurementData.RangeMilliMeter);
  SERIAL.println(" mm");
  delay(100);
}

float getDistance() {
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;
  memset(&RangingMeasurementData, 0, sizeof(VL53L0X_RangingMeasurementData_t));
  Status = VL53L0X.PerformSingleRangingMeasurement(&RangingMeasurementData);
  return RangingMeasurementData.RangeMilliMeter;
}

int getColor(int color) {
  // 0 for red, 1 for green, 2 for blue

  if (color == 0) {  //Red
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
  } else if (color == 1) {  //Green
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
  } else {  //Blue
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
  }

  return pulseIn(sensorOut, LOW);
}

float readPot() {
  int sensor_value = analogRead(potentiometer);    //Read the value from the potentiometer connected to the A0 pin
  int value = map(sensor_value, 0, 1023, 0, 100);  //Map the value from 0, 1023 to 0, 100
  return value;
}
