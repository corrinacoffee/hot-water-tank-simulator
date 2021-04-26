#include <stdio.h>
#include <stdbool.h>

// req 29: "The system shall allow moving a water level sensor."
void userSetWaterLevelSensor() {
  int sensorNumber;
  int newWaterLevel;
  printf("Enter water level sensor number 1, 2, 3, or 4 (1 being the lowest, 4 highest): ");
  scanf("%d", &sensorNumber);
  printf("Enter new water level: ");
  scanf("%d", &newWaterLevel);
  setWaterLevelSensor(sensorNumber, newWaterLevel)
}

// req 30: "The system shall allow changing the temperature sensor’s minimum value."
void userSetMinTemp() {
  int newMinTemp;
  printf("Enter new minimum temperature: ");
  scanf("%d", &newMinTemp);
  setMinTemp(newMinTemp)
}

// req 31: "The system shall allow changing the temperature sensor’s maximum value."
void userSetMaxTemp() {
  int newMaxTemp;
  printf("Enter new maximum temperature: ");
  scanf("%d", &newMaxTemp);
  setMaxTemp(newMaxTemp)
}
