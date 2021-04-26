// FIXME fix Corrina's bad syntax, make it more C less Java

#include <stdio.h>
#include <stdbool.h>

// req 5: "The system shall use time-stamping for identifying time of events (both ticks and sec/nsec)."
// req 6: "The system shall capture/record/display the system events and actions."
void log(string msg) {
  int timeInTics = tickGet();
  int timeInSec = sysClkRateGet()/timeInTics;
  printf("%d %d %s", timeInTics, timeInSec, msg);
}

// TODO have a way to initiate these functions that ask the user for input

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
