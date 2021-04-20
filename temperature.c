#include <stdbool.h>

// true if on, false if off
bool heater = false; // req 15

// in degrees celcius
int DEFAULT_MIN_TEMP = 10;
int DEFAULT_MAX_TEMP = 100;
int temp = 20;
int minTemp = DEFAULT_MIN_TEMP;
int maxTemp = DEFAULT_MAX_TEMP;

void updateTemp() {
  if (temp < minTemp) {
    heater = true;
  }
  if (temp > maxTemp) {
    heater = false;
  }
  if (heater) {
    temp++;
  }
}

void setMinTemperature(int newMinTemp) {
  if (newMinTemp > 0 && newMinTemp < maxTemp) {
    minTemp = newMinTemp;
  }
}
      
void setMaxTemperature(int newMaxTemp) {
  if (newMaxTemp > 0 && newMaxTemp > minTemp) {
    maxTemp = newMaxTemp;
  }
}
