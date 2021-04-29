#include <taskLib.h>
#include "temperature.h"
#include "commondefs.h"

// req 15: "The system shall have a water heater."
// true if on, false if off
bool heater = false;

static int min_target_temp;
static int max_target_temp;

static temp_sensor_t temp_sensor;

TASK_ID temp_tasks[3];

//named message queue - eli
MSG_Q_ID temp_queue;

//created temp sending function - eli
void send_temp(void) 
{
	message_struct_t message;
	
	message.state = temp_sensor.current_state;
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
	char message[255];
	
	while (1) {
		if (temp_sensor.temp >= MAX_TEMP) {
			// state is only included for safety purposes
			temp_sensor.current_state = TEMP_CRIT_HIGH;
			heater = false;
		} else if (temp_sensor.temp >= max_target_temp) {
			temp_sensor.current_state = TEMP_HIGH;
			heater = false;
		} else if (temp_sensor.temp >= min_target_temp) {
			temp_sensor.current_state = TEMP_IN_TARGET;
		} else if (temp_sensor.temp >= MIN_TEMP) {
			temp_sensor.current_state = TEMP_LOW;
			heater = true;
		} else if (temp_sensor.temp >= 0) {
			temp_sensor.current_state = TEMP_CRIT_LOW;
			heater = true;
		} else {
			temp_sensor.current_state = TEMP_ERROR;
		}
		
		if (temp_sensor.current_state != temp_sensor.previous_state) {
			printf("Temperature has reached state %d\n", temp_sensor.current_state);
			send_temp();
			//record(message);
			//temp_sensor.previous_state = temp_sensor.current_state;
		}
		printf("Temperature: %d\n", temp_sensor.temp);
		// TODO delay
		taskDelay(sysClkRateGet()*2); // quarter second delay - eli
	}
}

void TEMP_Init(void) {
	temp_sensor.temp = ROOM_TEMP;
	min_target_temp = DEFAULT_MIN_TARGET_TEMP;
	max_target_temp = DEFAULT_MAX_TARGET_TEMP;
	
	temp_tasks[0] = taskSpawn("tTempSim", 95, 0x100, 2000, (FUNCPTR)tempSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	temp_tasks[1] = taskSpawn("tTempSensorSim", 95, 0x100, 2000, (FUNCPTR)tempSensorSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	//created msgQ for temp - eli
	if ((temp_queue = msgQCreate(MESSAGE_Q_SIZE, MESSAGE_SIZE, MSG_Q_FIFO)) == MSG_Q_ID_NULL)
			record("Temperature level to pressure queue not created.");
		temp_tasks[2] = taskSpawn("tSendTempToPressureQueue", 95, 0x100, 2000, (FUNCPTR)send_temp, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
