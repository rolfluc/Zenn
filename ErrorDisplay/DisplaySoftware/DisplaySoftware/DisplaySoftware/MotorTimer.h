#pragma once
#include <stdint.h>
#define NUM_TIMEBOXES 3
typedef enum
{
	Timebox0,
	Timebox1,
	Timebox2,
}Timebox;
typedef void(*timeboxCallback)();
void InitTimers();
void SetCallback(Timebox tb, timeboxCallback cb);
void SetPosition(Timebox box, int16_t motorPosition_tenthsdegree);