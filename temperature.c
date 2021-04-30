#include <taskLib.h>
#include <semLib.h>

#include "temperature.h"
#include "commondefs.h"

// idea: use mutex semaphore so no one can turn the heater on
// until water has cooled to the desired temp (the mutex is released)
// req 15: "The system shall have a water heater."
// req 15: "The system shall have a water heater."
// true if on, false if off
bool heater;

static int min_target_temp;
static int max_target_temp;

static temp_sensor_t temp_sensor;

MSG_Q_ID temp_queue;

TASK_ID temp_tasks[2];

SEM_ID heater_sem;

//created temp sending function - eli
void send_temp(void) 
{
	message_struct_t message;
	
	message.state = 0;
	message.value = 0;
	
	message.state = (int)temp_sensor.current_state;
	message.value = temp_sensor.temp;
	
	msgQSend(temp_queue, (char *)&message, MESSAGE_SIZE, NO_WAIT, MSG_PRI_NORMAL);
}

void tempSimulator(int param) {
	while (1) {
		if (heater) {
			temp_sensor.temp += 2;
		} else {
			temp_sensor.temp--;
		}
		// TODO delay
		taskDelay(sysClkRateGet()*2); // quarter second delay - eli
		
	}
}

// req 17: "The system shall turn on the water heater when the temperature is below the minimum."
// req 18: "The system shall turn off the water heater when the temperature is above the maximum."
static void tempSensorSimulator(int param) {
	char message[MESSAGE_STRING_SIZE];
	
	while (1) {
		if (temp_sensor.temp >= MAX_TEMP) {
			// state is only included for safety purposes
			temp_sensor.current_state = TEMP_CRIT_HIGH;
			
			if (semTake(heater_sem, NO_WAIT) == OK) {
				heater = false;
				semGive(heater_sem);
			}
		} else if (temp_sensor.temp >= max_target_temp) {
			temp_sensor.current_state = TEMP_HIGH;
			
			if (semTake(heater_sem, NO_WAIT) == OK) {
				heater = false;
				semGive(heater_sem);
			}
		} else if (temp_sensor.temp >= min_target_temp) {
			temp_sensor.current_state = TEMP_IN_TARGET;
		} else if (temp_sensor.temp >= MIN_TEMP) {
			temp_sensor.current_state = TEMP_LOW;
			
			if (semTake(heater_sem, NO_WAIT) == OK) {
				heater = true;
				semGive(heater_sem);
			}
		} else if (temp_sensor.temp >= 0) {
			temp_sensor.current_state = TEMP_CRIT_LOW;
			
			if (semTake(heater_sem, NO_WAIT) == OK) {
				heater = true;
				semGive(heater_sem);
			}
		} else {
			temp_sensor.current_state = TEMP_ERROR;
		}
		
		if (temp_sensor.current_state != temp_sensor.previous_state) {
			temp_sensor.previous_state = temp_sensor.current_state;
			sprintf(message, "Temperature has reached state %d\n", temp_sensor.current_state);
			record(message);
			send_temp();
		}
		sprintf(message, "Temperature: %d\n", temp_sensor.temp);
		record(message);
		// TODO delay
		taskDelay(sysClkRateGet()*2); // half second delay - eli
	}
}

void TEMP_Init(void) {
	temp_sensor.temp = ROOM_TEMP;
	
	min_target_temp = DEFAULT_MIN_TARGET_TEMP;
	max_target_temp = DEFAULT_MAX_TARGET_TEMP;
	
	heater = false;
	
	heater_sem = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE);
	
	if ((temp_queue = msgQCreate(MESSAGE_Q_SIZE, MESSAGE_SIZE, MSG_Q_FIFO)) == MSG_Q_ID_NULL)
			record("Temperature level to pressure queue not created.");
	
}

void TEMP_TaskInit(void) {	
	temp_tasks[0] = taskSpawn("tTempSim", 100, 0x100, 2000, (FUNCPTR)tempSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	temp_tasks[1] = taskSpawn("tTempSensorSim", 100, 0x100, 2000, (FUNCPTR)tempSensorSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
