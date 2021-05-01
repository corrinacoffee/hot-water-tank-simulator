#ifndef __COMMONDEFS_H
#define __COMMONDEFS_H

/* Includes */
#include <stdint.h>
#include <tickLib.h>
#include <sysLib.h>

/* Definitions and Macros */
#define MESSAGE_Q_SIZE 	50
#define MESSAGE_STRING_SIZE 256

#define TIME_TICKS 		(tickGet())
#define TIME_SECS		(tickGet() / sysClkRateGet())
#define MESSAGE_SIZE 	(sizeof(message_struct_t))

#define true 			((uint8_t)1)
#define false 			((uint8_t)0)

/* Type, enumerator, and structure definitions */
typedef uint8_t bool;

typedef struct {
	int state;
	int value;
} message_struct_t;

#endif
