#include <stdio.h>
#include <taskLib.h>

#include "pressure.h"
#include "temperature.h"
#include "waterlevel.h"
#include "commondefs.h"

static pressure_t pressure_sensor;

static pressure_sim_vars_t pressure_sim;

TASK_ID pressure_tasks[3];

extern MSG_Q_ID water_queue;

void receiveWaterLevel(int param)
{
	message_struct_t message;
	
	while (1) {
		if (msgQReceive(water_queue, (char *)&message, MESSAGE_SIZE, NO_WAIT) > 0) {
			printf("Message received:\n");
			printf("State: %d\nTimestamp: %d\n", message.state, message.timestamp);
		}
		// TODO delay
	}
}

void pressureSim(int param) {
	while (1) {
		// req 20: "The pressure shall increase when the water level surpasses the second to highest water level sensor."
		if (pressure_sim.water_level == WATER_SENSOR_HIGH) {
			pressure_sensor.pressure++;
		}
		
		// req 21: "The pressure shall increase faster when then temperature is within a range of 100C."
		// TODO
		
		// TODO delay
	}
}

void pressureSensorSim(int param) {
	char message[255];
	
	while (1) {
		if (pressure_sensor.pressure >= PRESSURE_CRIT) {
			pressure_sensor.current_state = PRESSURE_STATE_CRITICAL;
			// req 22: "The system shall open the outlet valve when the pressure reaches the critical pressure of 50 psi (344 kPa)."
			// TODO signal
		} else if (pressure_sensor.pressure >= PRESSURE_NOMINAL_MAX) {
			pressure_sensor.current_state = PRESSURE_STATE_HIGH;
		} else if (pressure_sensor.pressure >= PRESSURE_NOMINAL_MIN) {
			pressure_sensor.current_state = PRESSURE_STATE_NOMINAL;
		} else if (pressure_sensor.pressure >= 0) {
			pressure_sensor.current_state = PRESSURE_STATE_LOW;
		} else {
			pressure_sensor.current_state = PRESSURE_STATE_ERROR;
		}
	
		if (pressure_sensor.current_state != pressure_sensor.previous_state) {
			sprintf(message, "Pressure has reached state %d", pressure_sensor.current_state);
			record(message);
			pressure_sensor.previous_state = pressure_sensor.current_state;
		}
		
		// TODO delay
	}
}

void PRESSURE_Init(void) {
	pressure_tasks[0] = taskSpawn("tPressureSensorSim", 95, 0x100, 2000, (FUNCPTR)pressureSensorSim, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	pressure_tasks[1] = taskSpawn("tPressureSim", 95, 0x100, 2000, (FUNCPTR)pressureSim, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	pressure_tasks[2] = taskSpawn("tPressureReceiveWaterQueue", 95, 0x100, 2000, (FUNCPTR)receiveWaterLevel, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
