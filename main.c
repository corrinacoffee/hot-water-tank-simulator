#include <stdio.h>
#include <sysLib.h>
#include <tickLib.h>

#include "commondefs.h"
#include "waterlevel.h"
#include "temperature.h"

// idea: use mutex semaphore so no one can turn the heater on
// until water has cooled to the desired temp (the mutex is released)
// req 15: "The system shall have a water heater."
static bool heater = false;

// TODO use this everywhere
// req 6: "The system shall capture/record/display the system events and actions."
// req 5: "The system shall use time-stamping for identifying time of events (both ticks and sec/nsec)."
void record(char *fmt) {
  int timeInTicks = tickGet();
  int timeInSec = sysClkRateGet() / timeInTicks;
  printf("%d %d %s", timeInTicks, timeInSec, fmt); // TODO format nicely
}

int main(void) {
	TEMP_Init();
	WATER_Init();
}
