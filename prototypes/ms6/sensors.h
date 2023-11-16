#define S0 45
#define S1 43
#define S2 41
#define S3 39
#define sensorOut 37

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

void detectAnomaly() {
  //Red
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  R = pulseIn(sensorOut, LOW);
  delay(100);

  //Green
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  G = pulseIn(sensorOut, LOW);
  delay(100);

  //Blue
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  B = pulseIn(sensorOut, LOW);
  delay(100);

  //Print results
  if(G-R>40 && B-R>50) {
    Serial.print("You ARE at the anomaly!\n");
  }
  else {
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
