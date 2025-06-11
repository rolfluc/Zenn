#include "LEDPWM.h"
#include "stm32g0xx_hal.h"
#include "PinDefs.h"

TIM_HandleTypeDef ledTimer;
static const uint8_t LED_BUFFER_LENGTH_MAX = 8;
static uint64_t OutputBuffer = 0;
static uint8_t OutputBufferLength = 0;
static uint64_t outputBufferIndex = 0;
	
void TIM14_IRQHandler(void)
{
	if (OutputBuffer && 1 << outputBufferIndex)
	{
		LED_PIN.pinPort->BSRR = LED_PIN.pinNumber;
	}
	else
	{
		LED_PIN.pinPort->BRR = LED_PIN.pinNumber;
	}
	outputBufferIndex++;
	if (outputBufferIndex != OutputBufferLength)
	{
		HAL_TIM_Base_Start_IT(&ledTimer);
	}
	else
	{
		HAL_TIM_Base_Stop_IT(&ledTimer);
	}
}

void InitPWM()
{
	__HAL_RCC_TIM14_CLK_ENABLE();
	
	ledTimer.Instance = TIM14;
	ledTimer.Init.Prescaler = 0;
	ledTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	ledTimer.Init.Period = 2000;
	ledTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	ledTimer.Init.RepetitionCounter = 0;
	ledTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&ledTimer) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
	HAL_NVIC_SetPriority(TIM14_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);
}

void SendPWM(uint8_t* data, uint8_t length)
{
	if (length > LED_BUFFER_LENGTH_MAX)
	{
		__ASM("BKPT 255");
	}
	for (uint8_t i = 0; i < length; i++)
	{
		OutputBuffer = data[i] << (7 - i);
	}
	OutputBufferLength = 8 * length;
	outputBufferIndex = 0;
	HAL_TIM_Base_Start_IT(&ledTimer);
}