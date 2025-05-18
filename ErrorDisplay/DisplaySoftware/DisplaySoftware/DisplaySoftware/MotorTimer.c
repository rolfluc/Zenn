#include "MotorTimer.h"
static timeboxCallback callbacks[NUM_TIMEBOXES];
static int16_t motorPositions[NUM_TIMEBOXES] = { 0 };

//  Drives all of the motors home, one by one.
static void DriveHome()
{
	
}

void InitTimers()
{
	
}

void SetCallback(Timebox tb, timeboxCallback cb)
{
	callbacks[(uint8_t)tb] = cb;
}

void SetPosition(Timebox box, int16_t motorPosition_tenthsdegree)
{
	
}
