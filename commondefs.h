#ifndef __COMMONDEFS_H
#define __COMMONDEFS_H

/* Includes */
#include <stdint.h>
#include <tickLib.h>
#include <sysLib.h>

/* Definitions and Macros */
#define TIME_TICKS 		(tickGet())
#define TIME_SECS		(tickGet() / sysClkRateGet())

#define true 			((uint8_t)1)
#define false 			((uint8_t)0)

#define MESSAGE_SIZE 	sizeof(message_struct_t)
#define MESSAGE_Q_SIZE 	50
#define MESSAGE_STRING_SIZE 256

/* Type, enumerator, and structure definitions */
typedef uint8_t bool;

typedef enum {
	MODULE_TEMP,
	MODULE_WATER,
} sending_module_t;

typedef struct {
	int state;
	int value;
} message_struct_t;

#endif
