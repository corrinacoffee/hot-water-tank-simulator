#include <stdio.h>
#include <sysLib.h>
#include <tickLib.h>

#include "commondefs.h"
#include "waterlevel.h"
#include "temperature.h"

// TODO use this everywhere
// req 6: "The system shall capture/record/display the system events and actions."
// req 5: "The system shall use time-stamping for identifying time of events (both ticks and sec/nsec)."
void record(char *fmt) {
	printf("[%d ticks - %d secs] %s\n", TIME_TICKS, TIME_SECS, fmt); // TODO format nicely
}

int main(void) {
	WATER_Init();
	TEMP_Init();
	PRESSURE_Init();
	
	WATER_TaskInit();
	TEMP_TaskInit();
	PRESSURE_TaskInit();
}
