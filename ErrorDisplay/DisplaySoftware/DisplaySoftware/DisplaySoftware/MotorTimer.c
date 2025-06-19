#include "MotorTimer.h"
#include "stm32g0xx_hal.h"
#include "stdbool.h"
static const int16_t motorLimits[NUM_TIMEBOXES] = { 16, 64, 64 };
static timeboxCallback callbacks[NUM_TIMEBOXES];
static int16_t motorPositions[NUM_TIMEBOXES] = { 0 };
static bool motorRunning[NUM_TIMEBOXES] = { false, false, false };
static uint8_t timerCounter = 0;

TIM_HandleTypeDef motorTimer;

void TIM6_DAC_LPTIM1_IRQHandler(void)
{
	__HAL_TIM_CLEAR_FLAG(&motorTimer, TIM_FLAG_UPDATE);
	timerCounter++;
	// TODO reissue?
	
	if (motorRunning[0])
	{
		if (timerCounter % motorLimits[0])
		{
			callbacks[0]();
		}
	}
	else if (motorRunning[1]) 
	{
		if (timerCounter % motorLimits[1])
		{
			callbacks[1]();
		}
	}
	else if (motorRunning[2])
	{
		if (timerCounter % motorLimits[2])
		{
			callbacks[2]();
		}
	}
}

void InitTimers()
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
}

void SetCallback(Timebox tb, timeboxCallback cb)
{
	callbacks[(uint8_t)tb] = cb;
}

void SetPosition(Timebox box, int16_t motorPosition_tenthsdegree)
{
	switch (box)
	{
	case Timebox0:
		{
			break;
		}
	case Timebox1:
		{
			break;
		}
	case Timebox2:
		{
			break;
		}
	default:
		break;
	}
}
