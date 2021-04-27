#ifndef __WATERLEVEL_H
#define __WATERLEVEL_H

/* Includes */

/* Definitions and Macros */
#define MAX_WATER_LEVEL 50	// req 23: "The volume of the tank shall be 50 L."

/* Type, enum, and struct definitions */
typedef enum {
	WATER_SENSOR_NONE,
	WATER_SENSOR_LOW,
	WATER_SENSOR_LOWMID,
	WATER_SENSOR_MIDHIGH,
	WATER_SENSOR_HIGH
} water_sensor_level_t;

typedef struct {
	water_sensor_level_t previous_sensor;
	water_sensor_level_t current_sensor;
	int water_level;
} water_sensor_t;

/* Function prototypes */
void WATER_Init(void);
void setWaterLevelSensor(int sensorNumber, int newWaterLevel);

#endif
