#include "LEDPWM.h"
#include "stm32g0xx_hal.h"
#include "PinDefs.h"

TIM_HandleTypeDef htim;
static const uint8_t LED_BUFFER_LENGTH_MAX = 8;
static uint64_t OutputBuffer = 0;
static uint8_t OutputBufferLength = 0;
static uint64_t outputBufferIndex = 0;
	
static void MX_TIM16_Init(void)
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
		// TODO restart
	}
}

void InitPWM()
{
	htim.Instance = TIM16;
	htim.Init.Prescaler = 0;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 2000;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim.Init.RepetitionCounter = 0;
	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
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
	// TODO start.
}