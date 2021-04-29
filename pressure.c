#include <stdio.h>
#include <taskLib.h>
#include <sigLib.h> //added signals library

#include "pressure.h"
#include "temperature.h"
#include "waterlevel.h"
#include "commondefs.h"

static pressure_t pressure_sensor;

extern bool out_valve;
extern MSG_Q_ID temp_queue;

static pressure_sim_vars_t pressure_sim;

TASK_ID pressure_tasks[4]; //changed to 4 tasks to accomodate the recieve temperature task

extern MSG_Q_ID water_queue;
extern MSG_Q_ID temp_queue; //added temp queue extern def - eli

void receiveWaterLevel(int param)
{
	message_struct_t message;
	
	while (1) {
		if (msgQReceive(water_queue, (char *)&message, MESSAGE_SIZE, NO_WAIT) > 0) {
			printf("PRESSURE - Water Message received:\n");
			printf("State: %d\n", message.state);
		}
		// TODO delay
		taskDelay(PRESSURE_DELAY); // quarter second delay - eli
	}
}

//added temp recieve message - eli
void receiveTempLevel(int param)
{
	message_struct_t message;
	
	while (1) {
		if (msgQReceive(temp_queue, (char *)&message, MESSAGE_SIZE, NO_WAIT) > 0) {
			pressure_sim.temperature = message.value;
			printf("PRESSURE - Temp Message received:\n");
			printf("Temp: %d\n", message.value);
		}
		// TODO delay
		taskDelay(PRESSURE_DELAY); // quarter second delay - eli
	}
}

void pressureSim(int param) {
	while (1) {
		// req 20: "The pressure shall increase when the water level surpasses the second to highest water level sensor."
		if (pressure_sim.temperature <= MAX_TEMP && pressure_sim.temperature >= DEFAULT_MAX_TARGET_TEMP) {
			pressure_sensor.pressure += 2;
		} else if (pressure_sim.water_level == WATER_SENSOR_MIDHIGH) {
			pressure_sensor.pressure++;
		} else if (out_valve == true) {
			if (pressure_sensor.pressure != 0)
				pressure_sensor.pressure--;
		}
		
		// req 21: "The pressure shall increase faster when then temperature is within a range of 100C."
		// TODO
		//possible implementation - eli -- could also just use integer values to set the range
		//if (pressure_sim.temperature >=95) 
		// TODO delay
		taskDelay(PRESSURE_DELAY); // quarter second delay - eli
	}
}

void pressureSensorSim(int param) {
	char message[255];
	
	while (1) {
		if (pressure_sensor.pressure >= PRESSURE_CRIT) {
			pressure_sensor.current_state = PRESSURE_STATE_CRITICAL;
			// req 22: "The system shall open the outlet valve when the pressure reaches the critical pressure of 50 psi (344 kPa)."
			// TODO signal
			//possible implementation kill(tid,SIGNO); --> kill(waterlevel_tasks[1],SIGNO);
			kill(WATER_getTaskId(),SIGNO);
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
			printf("Pressure has reached state %d\n", pressure_sensor.current_state);
//			record(message);
			pressure_sensor.previous_state = pressure_sensor.current_state;
		}
		printf("Pressure: %d\n", pressure_sensor.pressure);
		// TODO delay
		taskDelay(PRESSURE_DELAY); // quarter second delay - eli
	}
}

void PRESSURE_Init(void) {
	pressure_sensor.current_state = PRESSURE_STATE_INIT;
	pressure_sensor.previous_state = PRESSURE_STATE_INIT;
	pressure_sensor.pressure = 25;
	
	
	pressure_tasks[0] = taskSpawn("tPressureSensorSim", 95, 0x100, 2000, (FUNCPTR)pressureSensorSim, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	pressure_tasks[1] = taskSpawn("tPressureSim", 95, 0x100, 2000, (FUNCPTR)pressureSim, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	pressure_tasks[2] = taskSpawn("tPressureReceiveWaterQueue", 95, 0x100, 2000, (FUNCPTR)receiveWaterLevel, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	pressure_tasks[3] = taskSpawn("tPressureReceiveTempQueue", 95, 0x100, 2000, (FUNCPTR)receiveTempLevel, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); //added task for temp queue recieve - eli
}

void PRESSURE_Init_Critical(void) {
	WATER_Init();
	PRESSURE_Init();

	pressure_sensor.current_state = PRESSURE_STATE_INIT;
	pressure_sensor.previous_state = PRESSURE_STATE_INIT;
	pressure_sensor.pressure = PRESSURE_CRIT;
}

void PRESSURE_SetState(pressure_state_t state)
{
	pressure_sensor.current_state = state;
}
