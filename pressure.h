#ifndef __PRESSURE_H
#define __PRESSURE_H

/**
 * @file pressure.h
 * @brief All units are in psi
 */

/* Includes */
#include "waterlevel.h"

// idea: use an isr at crit pressure

/* Definitions and Macros */
#define PRESSURE_NOMINAL_MIN 20 // req 24: "The minimum nominal operating pressure shall be 20 psi (130 kPa)."
#define PRESSURE_NOMINAL_MAX 30 // req 25: "The maximum nominal operating pressure shall be 30 psi (206 kPa)."
#define PRESSURE_CRIT 50 		// req 26: "The critical pressure shall be 50 psi (344 kPa)."
#define PRESSURE_TEMP_RANGE 10 	// req 21: "The pressure shall increase faster when then temperature is within a range of 100°C"

/* Types, enum, and structure definitions */
typedef enum {
	PRESSURE_STATE_INIT,
	PRESSURE_STATE_LOW,
	PRESSURE_STATE_NOMINAL,
	PRESSURE_STATE_HIGH,
	PRESSURE_STATE_CRITICAL,
} pressure_state_t;

typedef struct {
	pressure_state_t state;
	int pressure;
} pressure_t;

/* Function prototypes */
void PRESSURE_StateMachine(void);

#endif
