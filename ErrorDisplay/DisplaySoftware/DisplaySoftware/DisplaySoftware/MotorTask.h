#pragma once
#include <stdint.h>
typedef enum
{
	Motor0 = 0,
	Motor1,
	Motor2,
}Motor;
void InitMotors();
void SendMotorPosition(Motor motor, int16_t motorPosition_tenthsdegrees);