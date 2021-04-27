#include <stdio.h>

#include "pressure.h"
#include "temperature.h"

static pressure_t pressure_sensor;

// req 20: "The pressure shall increase when the water level surpasses the second to highest water level sensor."
void updatePressureBasedOnWaterLevel() {
//  if (waterLevel > sensor3WaterLevel) {
    pressure_sensor.pressure++;
//  }
}

// req 21: "The pressure shall increase faster when then temperature is within a range of 100C."
void updatePressureBasedOnTemp() {
//  if (tempSensor > tempSensor + PRESSURE_TEMP_RANGE) {
    pressure_sensor.pressure++;
//  }
}

// req 22: "The system shall open the outlet valve when the pressure reaches the critical pressure of 50 psi (344 kPa)."
void pressureSensorControlOutValve() {
//  if (pressure_sensor.current_pressure >= CRIT_PRESSURE) {
//    outValve = true;
//  }
}

void PRESSURE_StateMachine(void) {
	switch (pressure_sensor.state) {
	case PRESSURE_STATE_INIT:
		break;
	default:
		while (1); // should never reach here
	}
}

