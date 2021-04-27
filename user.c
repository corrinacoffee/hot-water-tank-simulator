#include <stdio.h>

// TODO have a way for the user to initiate being asked for input (call these functions)

// req 29: "The system shall allow moving a water level sensor."
void userSetWaterLevelSensor() {
  int sensorNumber;
  int newWaterLevel;
  printf("Enter water level sensor number 1, 2, 3, or 4 (1 being the lowest, 4 highest): ");
  printf("Enter new water level: ");
  setWaterLevelSensor(sensorNumber, newWaterLevel);
}

// req 30: "The system shall allow changing the temperature sensorâ€™s minimum value."
void userSetMinTemp() {
  int newMinTemp;
  printf("Enter new minimum temperature: ");
  setMinTemp(newMinTemp);
}

// req 31: "The system shall allow changing the temperature sensorâ€™s maximum value."
void userSetMaxTemp() {
  int newMaxTemp;
  printf("Enter new maximum temperature: ");
  setMaxTemp(newMaxTemp);
}
