#ifndef __PRESSURE_H
#define __PRESSURE_H

/* Includes */
#include <sysLib.h>

#include "waterlevel.h"
#include "temperature.h"
#include "commondefs.h"

/* Definitions and Macros */
#define PRESSURE_AMBIENT		25

#define PRESSURE_NOMINAL_MIN 	20 	// req 24: "The minimum nominal operating pressure shall be 20 psi (130 kPa)."
#define PRESSURE_NOMINAL_MAX 	30 	// req 25: "The maximum nominal operating pressure shall be 30 psi (206 kPa)."
#define PRESSURE_CRIT 			50	// req 26: "The critical pressure shall be 50 psi (344 kPa)."
#define PRESSURE_TEMP_RANGE 	10 	// req 21: "The pressure shall increase faster when then temperature is within a range of 100Â°C"

#define PRESSURE_Q_MAX_MESSAGES 50

#define PRESSURE_DELAY	(sysClkRateGet())

/* Types, enum, and structure definitions */
typedef enum {
	PRESSURE_STATE_INIT,
	PRESSURE_STATE_LOW,
	PRESSURE_STATE_NOMINAL,
	PRESSURE_STATE_HIGH,
	PRESSURE_STATE_CRITICAL,
	PRESSURE_STATE_ERROR
} pressure_state_t;

typedef struct {
	pressure_state_t previous_state;
	pressure_state_t current_state;
	int pressure;
} pressure_t;

typedef struct {
	int water_level;
	int temperature;
} pressure_sim_vars_t;

/* Function prototypes */
void PRESSURE_Init(void);
void PRESSURE_TaskInit(void);

#endif
