#include <stdio.h>
#include <stdlib.h>
#include "waterlevel.h"
#include "temperature.h"
// TODO have a way for the user to initiate being asked for input (call these functions)

// req 29: "The system shall allow moving a water level sensor."
void userSetWaterLevelSensor() {
  int sensorNumber = -1;
  int newWaterLevel = -1;
  
  printf("Enter water level sensor number 1, 2, 3, or 4 (1 being the lowest, 4 highest): ");
  scanf("%d", &sensorNumber);
  
  printf("Enter new water level: ");
  scanf("%d", &newWaterLevel);
  
  setWaterLevelSensor(sensorNumber, newWaterLevel);
}

// req 30: "The system shall allow changing the temperature sensorâ€™s minimum value."
void userSetMinTemp() {
  int newMinTemp = -1;
  
  printf("Enter new minimum temperature: ");
  scanf("%d", &newMinTemp);
  
  setMinTemp(newMinTemp);
}

// req 31: "The system shall allow changing the temperature sensorâ€™s maximum value."
void userSetMaxTemp() {
  int newMaxTemp = -1;
  
  printf("Enter new maximum temperature: ");
  scanf("%d", &newMaxTemp);
  
  setMaxTemp(newMaxTemp);
}
