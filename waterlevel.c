#include <stdio.h>
#include <logLib.h>
#include "waterlevel.h"
#include "commondefs.h"
#include "main.h"

// req 8: "The system shall have two inlet valves."
// req 9: "The system shall have one outlet valve."
// true if open, false if closed
static bool in_valve_a;
static bool in_valve_b;
static bool out_valve;

// req 12: "The system shall have four water level sensors."
// in liters
static int sensor_level_low;
static int sensor_level_lowmid;
static int sensor_level_midhigh;
static int sensor_level_high;

static water_sensor_t water;

TASK_ID waterlevel_tasks[2];

// req 10: "Water shall enter the tank if either inlet valve is open."
// req 11: "Water shall enter the tank faster when both inlets are open than when one is open."
static void waterFlowSimulator(int param) {
	if (in_valve_a && in_valve_b) {
		water.water_level += 2;
	} else if (in_valve_a || in_valve_b) {
		water.water_level++;
	}
	if (out_valve) {
		water.water_level--;
	}
}

// req 12: "The system shall indicate when the water level reaches a certain sensor."
static void waterLevelSensorSimulator(int param) {
	char message[255];
	if (water.water_level >= sensor_level_high) {
		  water.current_sensor = WATER_SENSOR_HIGH;
		  out_valve = true; // req 14: "The system shall open the outlet valve when the water level reaches the highest sensor."
	  } else if (water.water_level >= sensor_level_midhigh) {
		  water.current_sensor = WATER_SENSOR_MIDHIGH;
	  } else if (water.water_level >= sensor_level_lowmid) {
		  water.current_sensor = WATER_SENSOR_LOWMID;
	  } else if (water.water_level >= sensor_level_low) {
		  water.current_sensor = WATER_SENSOR_LOW;
	  }
	  if (water.current_sensor != water.previous_sensor) {
		  sprintf(message, "Water level has reached water sensor %d", water.current_sensor);
		  record(message);
		  water.previous_sensor = water.current_sensor;
	  }
}

// req 29: "The system shall allow moving a water level sensor."
void setWaterLevelSensor(int sensorNumber, int newWaterLevel) {
  switch(sensorNumber) {
    case 1:
      if (newWaterLevel > 0 && newWaterLevel < sensor_level_lowmid) {
        sensor_level_low = newWaterLevel;
      }
    case 2:
      if (newWaterLevel > sensor_level_low && newWaterLevel < sensor_level_midhigh) {
        sensor_level_lowmid = newWaterLevel;
      }
    case 3:
      if (newWaterLevel > sensor_level_lowmid && newWaterLevel < sensor_level_high) {
        sensor_level_midhigh = newWaterLevel;
      }
    case 4:
      if (newWaterLevel > sensor_level_midhigh && newWaterLevel < MAX_WATER_LEVEL) {
        sensor_level_high = newWaterLevel;
      }
  }
}

void WATER_Init(void) {
	water.previous_sensor = WATER_SENSOR_NONE;
	water.current_sensor = WATER_SENSOR_NONE;
	water.water_level = 0;
	
	in_valve_a = true;
	in_valve_b = true;
	
	out_valve = false;
	
	sensor_level_low = 10;
	sensor_level_lowmid = 20;
	sensor_level_midhigh = 30;
	sensor_level_high = 40;
	
	waterlevel_tasks[0] = taskSpawn("tWaterFlowSim", 95, 0x100, 2000, (FUNCPTR)waterFlowSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	waterlevel_tasks[1] = taskSpawn("tWaterSensorSim", 95, 0x100, 2000, (FUNCPTR)waterLevelSensorSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
