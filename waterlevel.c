#include <stdio.h>
#include <stdbool.h>

// true if open, false if closed
bool inValve1 = false; // req 8: "The system shall have two inlet valves."
bool inValve2 = false;
bool outValve = false; // req 9: "The system shall have one outlet valve."

// in liters
// sensor 1 is lowest, 4 is highest
int waterLevel = 0;
int sensor1WaterLevel = 10; // req 12: "The system shall have four water level sensors."
int sensor2WaterLevel = 20;
int sensor3WaterLevel = 30;
int sensor4WaterLevel = 40;
int MAX_WATER_LEVEL = 50; // req 23: "The volume of the tank shall be 50 L."

// req 10: "Water shall enter the tank if either inlet valve is open."
// req 11: "Water shall enter the tank faster when both inlets are open than when one is open."
void updateWaterLevel() {
  if (inValve1 && inValve2) {
    waterLevel = waterLevel+2;
  }
  else if (inValve1 || inValve2) {
    waterLevel++;
  } else {
    // no water flow
  }
  
  if (outValve) {
    waterLevel--;
  }
}

// req 14: "The system shall open the outlet valve when the water level reaches the highest sensor."
void updateOutValve() {
  if (waterLevel >= sensor4WaterLevel) {
    outValve = true;
  }
}

// req 12: "The system shall indicate when the water level reaches a certain sensor."
int reachedWaterLevelSensor() {
  if (waterLevel >= sensor4WaterLevel) {
    return 4;
  } else if (waterLevel >= sensor3WaterLevel) {
    return 3;
  } else if (waterLevel >= sensor2WaterLevel) {
    return 2;
  } else if (waterLevel >= sensor1WaterLevel) {
    return 1;
  } else {
    return 0;
  }
}

// req 29: "The system shall allow moving a water level sensor."
int setWaterLevelSensor(int sensorNumber, int newWaterLevel) {
  retVal = 0;
  switch(sensorNumber) {
    case 1:
      if (newWaterLevel > 0 && newWaterLevel < sensor2WaterLevel) {
        sensor1WaterLevel = newWaterLevel;
        retVal = 1;
      }
    case 2:
      if (newWaterLevel > sensor1WaterLevel && newWaterLevel < sensor3WaterLevel) {
        sensor2WaterLevel = newWaterLevel;
        retVal = 1;
      }
    case 3:
      if (newWaterLevel > sensor2WaterLevel && newWaterLevel < sensor4WaterLevel) {
        sensor3WaterLevel = newWaterLevel;
        retVal = 1;
      }
    case 4:
      if (newWaterLevel > sensor3WaterLevel && newWaterLevel < MAX_WATER_LEVEL) {
        sensor4WaterLevel = newWaterLevel;
        retVal = 1;
      }
  }
  return retVal;
}

// param open true if open, false if close
void setInValve1Open(bool open) {
  inValve1 = open;
}

// param open true if open, false if close
void setInValve2Open(bool open) {
  inValve1 = open;
}

// param open true if open, false if close
void setOutValveOpen(bool open) {
  if (waterLevel >= sensor4WaterLevel) {
    outValve = true;
  } else {
    outValve = open;
  }
}
