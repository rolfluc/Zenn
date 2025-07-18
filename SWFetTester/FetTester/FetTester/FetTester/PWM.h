#pragma once
#include "stdint.h"
typedef enum 
{
	PWMPair_0,
	PWMPair_1,
	PWMPair_None,
}PWM;
void InitPWM();
void SetPWMPair(PWM pair, uint8_t percent);
void SetPWMArm(uint8_t percent);