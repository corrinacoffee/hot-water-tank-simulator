#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

/* Definitions and Macros */

// in degrees celcius
#define MIN_TEMP 				10	// req 28: "The default minimum temperature shall be 10C."
#define MAX_TEMP 				100	// req 27: "The default maximum temperature shall be 100C."
#define ROOM_TEMP 				20
#define DEFAULT_MIN_TARGET_TEMP 70
#define DEFAULT_MAX_TARGET_TEMP 80

/* Type, enum, and struct definitions */
typedef enum temp_state_e {
	TEMP_CRIT_LOW = 0,
	TEMP_LOW,
	TEMP_IN_TARGET,
	TEMP_HIGH,
	TEMP_CRIT_HIGH,
	TEMP_ERROR
} temp_state_t;

typedef struct temp_sensor_s {
	temp_state_t previous_state;
	temp_state_t current_state;
	int temp;
} temp_sensor_t;

/* Function prototyes */
void TEMP_Init(void);
void TEMP_TaskInit(void);
void TEMP_SetMin(int new_min_temp);
void TEMP_SetMax(int new_max_temp);

/* Test Cases */
void TEMP_TestCritLowTemp(void);
void TEMP_TestLowTemp(void);
void TEMP_TestHighTemp(void);
void TEMP_TestCritHighTemp(void);

#endif
