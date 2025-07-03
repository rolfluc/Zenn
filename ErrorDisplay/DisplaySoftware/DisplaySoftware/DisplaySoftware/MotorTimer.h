#pragma once
#include <stdint.h>
#include "Stepper.h"
#define NUM_TIMEBOXES 3
typedef enum
{
	Timebox0,
	Timebox1,
	Timebox2,
}Timebox;
typedef void(*timeboxCallback)();
void InitTimers(Stepper* step0, Stepper* step1, Stepper* step2);
void SetCallback(Timebox tb, timeboxCallback cb);
void SetPosition(Timebox box, int16_t motorPosition_tenthsdegree);