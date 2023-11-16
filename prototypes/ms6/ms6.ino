#include "Enes100.h"
#include "movement.h"
#include "sensors.h"
#include "mission.h"

bool wifiModuleConnected = false;

void setup() {
  // Serial.begin(9600);

  // WIFI
  if (wifiModuleConnected) {
    Enes100.begin("The Vedauntless", CRASH_SITE, 6, 51, 50);
    Enes100.println("Connected...");
  }

  // Serial.println("testingn");

  // SENSORS
  initColorSensor();
  initDistSensor();

  // float val = getDistance();
  // Serial.println(val);

  // navToSite();

  // moveWithTime(pi, 1, 0, 300);

}

void loop() {
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;

  memset(&RangingMeasurementData, 0, sizeof(VL53L0X_RangingMeasurementData_t));
  Status = VL53L0X.PerformSingleRangingMeasurement(&RangingMeasurementData);
  if (VL53L0X_ERROR_NONE == Status) {
      if (RangingMeasurementData.RangeMilliMeter >= 2000) {
          SERIAL.println("out of range!!");
      } else {
          SERIAL.print("Measured distance:");
          SERIAL.print(RangingMeasurementData.RangeMilliMeter);
          SERIAL.println(" mm");
      }
  } else {
      SERIAL.print("mesurement failed !! Status code =");
      SERIAL.println(Status);
  }

  detectAnomaly();

  delay(300);
}
