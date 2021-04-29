#ifndef __COMMONDEFS_H
#define __COMMONDEFS_H

/* Includes */
#include <stdint.h>

/* Definitions and Macros */
#define true ((uint8_t)1)
#define false ((uint8_t)0)

#define MESSAGE_SIZE sizeof(message_struct_t)
#define MESSAGE_Q_SIZE 50

/* Type, enumerator, and structure definitions */
typedef uint8_t bool;

typedef struct {
	int state;
	int value;
} message_struct_t;

#endif
