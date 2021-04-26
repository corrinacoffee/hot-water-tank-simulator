// FIXME fix Corrina's bad syntax, make it more C less Java

#include <stdio.h>
#include <stdbool.h>

// true if on, false if off
bool heater = false; // req 15: "The system shall have a water heater."

// in degrees celcius
int DEFAULT_MIN_TEMP = 10; // req 28: "The default minimum temperature shall be 10°C."
int DEFAULT_MAX_TEMP = 100; // req 27: "The default maximum temperature shall be 100°C."
int minTemp = DEFAULT_MIN_TEMP;
int maxTemp = DEFAULT_MAX_TEMP;
int tempSensor = 20; // user defined req 32: "The temperature of water entering the tank shall be room temperature, or 20°C."

// TODO use real time construct to perform periodically
void updateTemp() {
  if (heater) {
    tempSensor++;
  }
}

// TODO use real time construct to initiate action
// req 17: "The system shall turn on the water heater when the temperature is below the minimum."
// req 18: "The system shall turn off the water heater when the temperature is above the maximum."
void tempSensorControlHeater() {
  if (tempSensor < minTemp) {
    heater = true;
  }
  if (tempSensor > maxTemp) {
    heater = false;
  }
}

// req 30: "The system shall allow changing the temperature sensor’s minimum value."
void setMinTemp(int newMinTemp) {
  if (newMinTemp > 0 && newMinTemp < maxTemp) {
    minTemp = newMinTemp;
  }
}

// req 31: "The system shall allow changing the temperature sensor’s maximum value."
void setMaxTemp(int newMaxTemp) {
  if (newMaxTemp > 0 && newMaxTemp > minTemp) {
    maxTemp = newMaxTemp;
  }
}
