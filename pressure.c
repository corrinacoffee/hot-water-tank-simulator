#include <stdio.h>
#include <string.h>
#include <taskLib.h>
#include <sigLib.h>
#include <semLib.h>

#include "pressure.h"
#include "temperature.h"
#include "waterlevel.h"
#include "commondefs.h"
#include "main.h"

extern bool out_valve;
extern bool heater;

extern MSG_Q_ID water_queue;
extern MSG_Q_ID temp_queue;

extern SEM_ID out_valve_sem;
extern SEM_ID heater_sem;

static pressure_t pressure_sensor;
static pressure_sim_vars_t pressure_sim;

TASK_ID pressure_tasks[4];
WDOG_ID pressure_watchdog;

void receiveWaterLevel(int param)
{
	message_struct_t message;
	char record_message[MESSAGE_STRING_SIZE];
	
	while (1) {
		if (msgQReceive(water_queue, (char *)&message, MESSAGE_SIZE, NO_WAIT) > 0) {
			memset(record_message, 0, sizeof(char) * MESSAGE_STRING_SIZE);
			sprintf(record_message, "PRESSURE - Message Received:\n * Water Level: %d\n * State: %d\n", message.value, message.state);
			record(record_message);
		}
		taskDelay(PRESSURE_DELAY);
	}
}

void receiveTempLevel(int param)
{
	message_struct_t message;
	char record_message[MESSAGE_STRING_SIZE];
	
	while (1) {
		if (msgQReceive(temp_queue, (char *)&message, MESSAGE_SIZE, NO_WAIT) > 0) {
			memset(record_message, 0, sizeof(char) * MESSAGE_STRING_SIZE);
			pressure_sim.temperature = message.value;
			sprintf(record_message, "PRESSURE - Message received:\n * Temp: %d\n * State %d\n", message.value);
			record(record_message);
		}
		taskDelay(PRESSURE_DELAY);
	}
}

void pressureSim(int param) {
	while (1) {
		if (pressure_sim.temperature <= MAX_TEMP && pressure_sim.temperature >= DEFAULT_MAX_TARGET_TEMP) {
			// req 21: "The pressure shall increase faster when then temperature is within a range of 100C."
			pressure_sensor.pressure += 2;
		} else if (pressure_sim.water_level == WATER_SENSOR_MIDHIGH) {
			// req 20: "The pressure shall increase when the water level surpasses the second to highest water level sensor."
			pressure_sensor.pressure++;
		} else if (out_valve == true) {
			if (pressure_sensor.pressure != 0)
				pressure_sensor.pressure--;
		}
		
		taskDelay(PRESSURE_DELAY);
	}
}

void pressure_handle_missed_deadline(int param) {
	char log_message[MESSAGE_STRING_SIZE];
	
	if (pressure_sensor.pressure >= PRESSURE_CRIT)
		kill(WATER_getTaskId(), SIGNO);

	record("Pressure sensor missed deadline; restarting task");
	
	// Restart pressure sensor task
	taskRestart(pressure_tasks[0]);
}

void pressureSensorSim(int param) {
	char message[MESSAGE_STRING_SIZE];
	
	while (1) {
		wdStart(pressure_watchdog, PRESSURE_DELAY, (FUNCPTR)pressure_handle_missed_deadline, 0);
	
		if (pressure_sensor.pressure >= PRESSURE_CRIT) {
			pressure_sensor.current_state = PRESSURE_STATE_CRITICAL;
			// req 22: "The system shall open the outlet valve when the pressure reaches the critical pressure of 50 psi (344 kPa)."
			if (out_valve != true && semTake(out_valve_sem, NO_WAIT) == OK)
				kill(WATER_getTaskId(), SIGNO);
			// turn the heater off too, if possible; goal is to lower pressure which depends on water level and temperature
			if (semTake(heater_sem, NO_WAIT) == OK)
				heater = false;
		} else if (pressure_sensor.pressure >= PRESSURE_NOMINAL_MAX) {
			pressure_sensor.current_state = PRESSURE_STATE_HIGH;
			semGive(out_valve_sem);
			semGive(heater_sem);
		} else if (pressure_sensor.pressure >= PRESSURE_NOMINAL_MIN) {
			pressure_sensor.current_state = PRESSURE_STATE_NOMINAL;
		} else if (pressure_sensor.pressure >= 0) {
			pressure_sensor.current_state = PRESSURE_STATE_LOW;
		} else {
			pressure_sensor.current_state = PRESSURE_STATE_ERROR;
		}
	
		if (pressure_sensor.current_state != pressure_sensor.previous_state) {
			pressure_sensor.previous_state = pressure_sensor.current_state;
			
			sprintf(message, "Pressure has reached state %d\n", pressure_sensor.current_state);
			record(message);
			
			sprintf(message, "Pressure: %d\n", pressure_sensor.pressure);
			record(message);
		}
	}
}

void PRESSURE_Init(void) {
	pressure_sensor.current_state = PRESSURE_STATE_INIT;
	pressure_sensor.previous_state = PRESSURE_STATE_INIT;
	pressure_sensor.pressure = PRESSURE_AMBIENT;
	
	if ((pressure_watchdog = wdCreate()) == NULL)
		record("Error creating pressure watchdog.\n");
}

void PRESSURE_TaskInit(void) {	
	pressure_tasks[0] = taskSpawn("tPressureSensorSim", 95, 0x100, 2000, (FUNCPTR)pressureSensorSim, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	pressure_tasks[1] = taskSpawn("tPressureSim", 95, 0x100, 2000, (FUNCPTR)pressureSim, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	pressure_tasks[2] = taskSpawn("tPressureReceiveWaterQueue", 95, 0x100, 2000, (FUNCPTR)receiveWaterLevel, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	pressure_tasks[3] = taskSpawn("tPressureReceiveTempQueue", 95, 0x100, 2000, (FUNCPTR)receiveTempLevel, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void PRESSURE_TestCriticalPressure(void) {
	WATER_Init();
	TEMP_Init();
	PRESSURE_Init();

	pressure_sensor.pressure = PRESSURE_CRIT;
	
	WATER_TaskInit();
	TEMP_TaskInit();
	PRESSURE_TaskInit();
}
