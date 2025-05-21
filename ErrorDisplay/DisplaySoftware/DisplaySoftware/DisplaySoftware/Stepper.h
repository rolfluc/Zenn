#pragma once
#include "PinDefs.h"
typedef enum 
{
	State_0,
	State_1,
	State_2,
	State_3,
	State_4,
	State_5,
	State_Size
}StepperMotorState;

typedef enum 
{
	Clockwise = 0,   
	Counterclockwise
}Direction;


typedef struct 
{
	PinDef* P00;
	PinDef* P01;
	PinDef* P10;
	PinDef* P11;
	StepperMotorState currentState;	
}Stepper;

void InitStepper(Stepper* step);
void DriveHomeBlocking(Stepper* step);