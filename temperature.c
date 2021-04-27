#include "temperature.h"
#include "commondefs.h"

// req 15: "The system shall have a water heater."
// true if on, false if off
bool heater = false;

static int temp_sensor;
static int min_target_temp;
static int max_target_temp;

static temp_sensor_t temp_sensor;

TASK_ID temp_tasks[2];

void TEMP_Init(void) {
	temp_sensor.temp = ROOM_TEMP;
	min_target_temp = DEFAULT_MIN_TARGET_TEMP;
	max_target_temp = DEFAULT_MAX_TARGET_TEMP;
	
	waterlevel_tasks[0] = taskSpawn("tTempSim", 95, 0x100, 2000, (FUNCPTR)tempSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
//	waterlevel_tasks[1] = taskSpawn("tTempSensor". 95, 0x100, 2000, (FUNCPTR))
}

void tempSimulator() {
	if (heater) {
		temp_sensor++;
	} else {
		temp_sensor--;
	}
}

// req 17: "The system shall turn on the water heater when the temperature is below the minimum."
// req 18: "The system shall turn off the water heater when the temperature is above the maximum."
void tempSensorControlHeater() {
  if (temp_sensor < min_target_temp) {
    heater = true;
  }
  if (temp_sensor > max_target_temp) {
    heater = false;
  }
}

// req 30: "The system shall allow changing the temperature sensors minimum value."
void setMinTemp(int new_min_temp) {
  if (new_min_temp > 0 && new_min_temp < max_target_temp) {
    min_target_temp = new_min_temp;
  }
}

// req 31: "The system shall allow changing the temperature sensors maximum value."
void setMaxTemp(int new_max_temp) {
  if (new_max_temp > 0 && new_max_temp > min_target_temp) {
    max_target_temp = new_max_temp;
  }
}
