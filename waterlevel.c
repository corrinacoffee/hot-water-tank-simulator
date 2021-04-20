#include <stdbool.h>

// true if open, false if closed
bool inValve1 = false; // req 8
bool inValve2 = false;
bool outValve = false; // req 9

// in liters
int waterLevel = 0;
int sensor1WaterLevel = 10; // req 12
int sensor2WaterLevel = 20;
int sensor3WaterLevel = 30;
int sensor4WaterLevel = 40;
int MAX_WATER_LEVEL = 50; // req 23

// example code, would need wrapped in functions and timed/repeated

// req 11: "Water shall enter the tank faster when both inlets are open than when one is open."
// approach 1
if (inValve1 && inValve2) {
  waterLevel = waterLevel+2;
}
else if (inValve1 || inValve2) {
  waterLevel++;
} else {
  // no water flow
}
// approach 2
if (inValve1) {
  waterLevel++;
}
if (inValve2) {
  waterLevel++;
}

if (outValve) {
  waterLevel--;
}

// req 14: "The system shall open the outlet valve when the water level reaches the highest sensor."
if (waterLevel >= sensor4WaterLevel) {
  outValve = true;
}

// req 29: The system shall allow moving a water level sensor.
void setWaterLevelSensor(int sensorNumber, int newWaterLevel) {
  switch(sensorNumber) {
    case 1:
      if (newWaterLevel > 0 && newWaterLevel < sensor2WaterLevel) {
        sensor1WaterLevel = newWaterLevel;
      }
    case 2:
      if (newWaterLevel > sensor1WaterLevel && newWaterLevel < sensor3WaterLevel) {
        sensor2WaterLevel = newWaterLevel;
      }
    case 3:
      if (newWaterLevel > sensor2WaterLevel && newWaterLevel < sensor4WaterLevel) {
        sensor3WaterLevel = newWaterLevel;
      }
    case 4:
      if (newWaterLevel > sensor3WaterLevel && newWaterLevel < MAX_WATER_LEVEL) {
        sensor4WaterLevel = newWaterLevel;
      }
  }
}
