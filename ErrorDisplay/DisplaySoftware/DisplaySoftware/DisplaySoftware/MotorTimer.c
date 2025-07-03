#include "MotorTimer.h"
#include "stm32g0xx_hal.h"
#include "stdbool.h"
static const int16_t motorLimits[NUM_TIMEBOXES] = { 16, 64, 64 };
static int16_t motorPositions[NUM_TIMEBOXES] = { 0 };
static int16_t motorDesiredPositions[NUM_TIMEBOXES] = { 0 };
static Stepper* steppers[NUM_TIMEBOXES];
static stepcb* timerCallbacks[NUM_TIMEBOXES] = {};
static bool motorDirectionClockwise[NUM_TIMEBOXES] = { true, true, true };
static bool motorRunning[NUM_TIMEBOXES] = { false, false, false };
static uint8_t timerCounter = 0;

TIM_HandleTypeDef motorTimer;

void TIM6_DAC_LPTIM1_IRQHandler(void)
{
	__HAL_TIM_CLEAR_FLAG(&motorTimer, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&motorTimer);
	timerCounter++;
	
	if (motorRunning[0])
	{
		if (timerCounter % motorLimits[0])
		{
		}
	}
	else if (motorRunning[1]) 
	{
		if (timerCounter % motorLimits[1])
		{
		}
	}
	else if (motorRunning[2])
	{
		if (timerCounter % motorLimits[2])
		{
		}
	}
}

void InitTimers(Stepper* step0, Stepper* step1, Stepper* step2)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	motorTimer.Instance = TIM6;
	motorTimer.Init.Prescaler = 32;
	motorTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	motorTimer.Init.Period = 20000;
	motorTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	motorTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&motorTimer) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
	
	HAL_NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM6_DAC_LPTIM1_IRQn);
	timerCallbacks[0] = GetClockwiseCB();
	timerCallbacks[1] = GetClockwiseCB();
	timerCallbacks[2] = GetClockwiseCB();
	steppers[0] = step0;
	steppers[1] = step1;
	steppers[2] = step2;
	HAL_TIM_Base_Start_IT(&motorTimer);
}

static inline int16_t getMotorCounts(int16_t position)
{
	// TODO 
	return position;
}

static inline void SetNewPosition(uint8_t index, int16_t position)
{
	int16_t desiredMotorPos = getMotorCounts(position);
	if (!motorRunning[index])
	{
		// Starting a motor back up
		if (desiredMotorPos != motorPositions[index])
		{
			motorRunning[index] = true;
			// TODO what do when coming from static.
		}
		else
		{
			// If asked for the same pos that the motor is at... 
			return;
		}
	}
	// Motor is already moving.
	// Thought process:
	// If desiredMotorPos > motorDesiredPosition, and motorDirectionClockwise, just update.
	// If desiredMotorPos > motorDesiredPosition, and !motorDirectionClockwise, stop, then revert to counterclockwise.
	// If desiredMotorPos < motorDesiredPosition, and motorDirectionClockwise, stop, then revert to counterclockwise.
	// If desiredMotorPos < motorDesiredPosition, and !motorDirectionClockwise, just update.
	if (desiredMotorPos > motorDesiredPositions[index] && motorDirectionClockwise[index])
	{
		motorDesiredPositions[index] = desiredMotorPos;
	}
	else if (desiredMotorPos > motorDesiredPositions[index] && !motorDirectionClockwise[index])
	{
		//ClearMotor(step); 
		motorDesiredPositions[index] = desiredMotorPos;
		motorDirectionClockwise[index] = true;
		timerCallbacks[index] = GetClockwiseCB();
	}
	else if (desiredMotorPos < motorDesiredPositions[index] && motorDirectionClockwise[index])
	{
		ClearMotor(steppers[index]);
		motorDesiredPositions[index] = desiredMotorPos;
		motorDirectionClockwise[index] = false;
		timerCallbacks[index] = GetCounterClockwiseCB();
	}
	else if (desiredMotorPos < motorDesiredPositions[index] && !motorDirectionClockwise[index])
	{
		motorDesiredPositions[index] = desiredMotorPos;
	}
	// TODO what do when coming from static? Otherwise good to go?
}

void SetPosition(Timebox box, int16_t motorPosition_tenthsdegree)
{
	switch (box)
	{
	case Timebox0:
		{
			SetNewPosition(0, motorPosition_tenthsdegree);
			break;
		}
	case Timebox1:
		{
			SetNewPosition(1, motorPosition_tenthsdegree);
			break;
		}
	case Timebox2:
		{
			SetNewPosition(2, motorPosition_tenthsdegree);
			break;
		}
	default:
		break;
	}
}
