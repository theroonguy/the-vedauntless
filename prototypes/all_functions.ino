//VISION SYSTEM
#include "Enes100.h"

//COLOR SENSOR
#define S0 45
#define S1 43
#define S2 41
#define S3 39
#define sensorOut 37

int frequency = 0;
int R = 0;
int G = 0;
int B = 0;
// *****


//DISTANCE SENSOR
#include "Seeed_vl53l0x.h"
Seeed_vl53l0x VL53L0X;

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif
// *****


void setup() {
  //VISION SYSTEM
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, Wifi Module RX Pin, Wifi Module TX Pin
    Enes100.begin("The Vedauntless", CRASH_SITE, 6, 51, 50);
    // At this point we know we are connected.
    Enes100.println("Connected...");

  //COLOR SENSOR
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
    Serial.begin(9600);

  //DISTANCE SENSOR
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
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
}

void loop() {
  //Preamble for Vision System - MUST have
  float x, y, t; bool v; // Declare variables to hold the data
  x = Enes100.getX();  // Your X coordinate! 0-4, in meters, -1 if no aruco is not visibility (but you should use Enes100.isVisible to check that instead)
  y = Enes100.getY();  // Your Y coordinate! 0-2, in meters, also -1 if your aruco is not visible.
  t = Enes100.getTheta();  //Your theta! -pi to +pi, in radians, -1 if your aruco is not visible.
  v = Enes100.isVisible(); // Is your aruco visible? True or False.
  //Reports vehicle info to Vision computer
  enesBureaucracy(x,y,t,v);

  Find and print distance and anomaly status
  bool isAtAnomlay = detectAnomaly();
  int distance = findDistance();
}

bool detectAnomaly() {
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
    Serial.print("You are at the anomaly!\n");
    return true;
  }
  else {
    Serial.print("You are NOT at the anomaly!\n");
    return false;
  }
}

int findDistance() {
      VL53L0X_RangingMeasurementData_t RangingMeasurementData;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    memset(&RangingMeasurementData, 0, sizeof(VL53L0X_RangingMeasurementData_t));
    Status = VL53L0X.PerformSingleRangingMeasurement(&RangingMeasurementData);
    if (VL53L0X_ERROR_NONE == Status) {
        if (RangingMeasurementData.RangeMilliMeter >= 2000) {
            SERIAL.println("out of range!!");
        } else {
            SERIAL.print("Distance: ");
            SERIAL.print(RangingMeasurementData.RangeMilliMeter);
            SERIAL.println(" mm");
            return RangingMeasurementData.RangeMilliMeter;
        }
    } else {
        SERIAL.print("mesurement failed !! Status code =");
        SERIAL.println(Status);
    }
    delay(300);
}

void enesBureaucracy(float x, float y, float t, bool v) {
  if (v) { // If the ArUco marker is visible
    Enes100.print(x); // print out the location
    Enes100.print(",");
    Enes100.print(y);
    Enes100.print(",");
    Enes100.println(t);
  }
  else { // otherwise
    Enes100.println("Not visible"); // print not visible
  }
}
