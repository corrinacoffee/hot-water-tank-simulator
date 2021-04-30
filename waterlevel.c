#include <stdio.h>
#include <logLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <sigLib.h>
#include <semLib.h>

#include "waterlevel.h"
#include "commondefs.h"
#include "pressure.h"
#include "main.h"

// req 8: "The system shall have two inlet valves."
// req 9: "The system shall have one outlet valve."
// true if open, false if closed
bool in_valve_a;
bool in_valve_b;
bool out_valve;


// req 12: "The system shall have four water level sensors."
// in liters
static int sensor_level_low;
static int sensor_level_lowmid;
static int sensor_level_midhigh;
static int sensor_level_high;

static water_sensor_t water;

TASK_ID waterlevel_tasks[3];

MSG_Q_ID water_queue;

SEM_ID out_valve_sem;

static void send_water(void)
{
	message_struct_t message;
	
	message.state = 0;
	message.value = 0;
	
	message.state = (int)water.current_sensor;
	message.value = water.water_level;
	
	msgQSend(water_queue, (char *)&message, MESSAGE_SIZE, NO_WAIT, MSG_PRI_NORMAL);
}

// req 10: "Water shall enter the tank if either inlet valve is open."
// req 11: "Water shall enter the tank faster when both inlets are open than when one is open."
static void waterFlowSimulator(int param) {
	while (1) {
		if (in_valve_a && in_valve_b) {
			water.water_level += 2;
		} else if (in_valve_a || in_valve_b) {
			water.water_level++;
		}
		
		if (out_valve) {
			water.water_level--;
		}
		
		taskDelay(sysClkRateGet()*2);
	}
}

// req 12: "The system shall indicate when the water level reaches a certain sensor."
static void waterLevelSensorSimulator(int param) {
	char message[MESSAGE_STRING_SIZE];
	
	signal(SIGNO, WATER_SignalHandler);
	
	while (1) {
		if (water.water_level >= sensor_level_high) {
			  water.current_sensor = WATER_SENSOR_HIGH;
			  
			  if (semTake(out_valve_sem, NO_WAIT) == OK)
				  out_valve = true; // req 14: "The system shall open the outlet valve when the water level reaches the highest sensor."
			  
			  in_valve_a = false;
			  in_valve_b = false;
		  } else if (water.water_level >= sensor_level_midhigh) {
			  water.current_sensor = WATER_SENSOR_MIDHIGH;
			  
			  if (semTake(out_valve_sem, NO_WAIT) == OK)
				  out_valve = false;
			  
			  in_valve_a = false;
			  in_valve_b = false;
		  } else if (water.water_level >= sensor_level_lowmid) {
			  water.current_sensor = WATER_SENSOR_LOWMID;
			  
			  if (semTake(out_valve_sem, NO_WAIT) == OK)
				  out_valve = false;\
				  
			  in_valve_a = true;
			  in_valve_b = false;
		  } else if (water.water_level >= sensor_level_low) {
			  water.current_sensor = WATER_SENSOR_LOW;
			  
			  if (semTake(out_valve_sem, NO_WAIT) == OK)
				  out_valve = false;
			  
			  in_valve_a = true;
			  in_valve_b = true;
		  } else if (water.water_level >= 0) {
			  water.current_sensor = WATER_SENSOR_NONE;
			  
			  if (semTake(out_valve_sem, NO_WAIT) == OK)
				  out_valve = false;
			  
			  in_valve_a = true;
			  in_valve_b = true;
		  } 
		
		  if (water.current_sensor != water.previous_sensor) {
			  semGive(out_valve_sem);
			  water.previous_sensor = water.current_sensor;
			  
			  sprintf(message, "Water level has reached water sensor %d\n", water.current_sensor);
			  record(message);

			  sprintf(message, "Water level: %d\n", water.water_level);
			  record(message);

			  send_water();
		  }
		  
		  taskDelay(sysClkRateGet()*2);
	}
}

// req 29: "The system shall allow moving a water level sensor."
void WATER_SetSensorToLevel(int sensorNumber, int newWaterLevel) {
  switch(sensorNumber) {
    case WATER_SENSOR_LOW:
      if (newWaterLevel > 0 && newWaterLevel < sensor_level_lowmid)
        sensor_level_low = newWaterLevel;
      break;
    case WATER_SENSOR_LOWMID:
      if (newWaterLevel > sensor_level_low && newWaterLevel < sensor_level_midhigh)
        sensor_level_lowmid = newWaterLevel;
      break;
    case WATER_SENSOR_MIDHIGH:
      if (newWaterLevel > sensor_level_lowmid && newWaterLevel < sensor_level_high)
        sensor_level_midhigh = newWaterLevel;
      break;
    case WATER_SENSOR_HIGH:
      if (newWaterLevel > sensor_level_midhigh && newWaterLevel < MAX_WATER_LEVEL)
        sensor_level_high = newWaterLevel;
      break;
    default:
    	record("Invalid sensor\n");
    	return;
  }
}

void WATER_Init(void) {
	int i;
	
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
	
	out_valve_sem = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE);
	
	if ((water_queue = msgQCreate(MESSAGE_Q_SIZE, MESSAGE_SIZE, MSG_Q_FIFO)) == MSG_Q_ID_NULL)
		record("Water level to pressure queue not created.");
	
	
}

void WATER_TaskInit(void) {	
	waterlevel_tasks[0] = taskSpawn("tWaterFlowSim", 100, 0x100, 2000, (FUNCPTR)waterFlowSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	waterlevel_tasks[1] = taskSpawn("tWaterSensorSim", 100, 0x100, 2000, (FUNCPTR)waterLevelSensorSimulator, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void WATER_TestVeryLowWaterLevel(void) {
	WATER_Init();
	water.water_level = 1;
	
	WATER_TaskInit();
}

void WATER_TestLowWaterLevel(void) {
	WATER_Init();
	water.water_level = 10;
	
	WATER_TaskInit();
}

void WATER_TestAvgWaterLevel(void) {
	WATER_Init();
	water.water_level = 20;
	
	WATER_TaskInit();
}

void WATER_TestHighWaterLevel(void) {
	WATER_Init();
	water.water_level = 40;
	
	WATER_TaskInit();
}

//added signal handler - eli
void WATER_SignalHandler(int sigNo){
	switch(sigNo){
	case(30):
			//might need to take semaphore
			out_valve = true;
			in_valve_a = false;
			in_valve_b = false;
			logMsg("CRITICAL PRESSURE -- Out valve opened.\n",0,0,0,0,0,0);
	}
}

TASK_ID WATER_getTaskId(void)
{
	return waterlevel_tasks[1];
}
