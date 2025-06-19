#include "LEDPWM.h"
#include "stm32g0xx_hal.h"
#include "PinDefs.h"
#include <stdbool.h>

TIM_HandleTypeDef ledTimer;
static const uint8_t LED_BUFFER_LENGTH_MAX = 8;
static uint32_t OutputBuffer = 0;
static uint8_t OutputBufferLength = 0;
static uint32_t outputBufferIndex = 0;

static inline void FillBit(uint8_t* byte, uint8_t pos, bool isHigh)
{
	static const uint8_t bitHigh = 0x0A;
	static const uint8_t bitLow = 0x08;
	if (isHigh)
	{
		*byte |= bitHigh << pos;
	}
	else
	{
		*byte |= bitLow << pos;
	}
}
	
void TIM14_IRQHandler(void)
{
	if ((OutputBuffer & (1 << outputBufferIndex)) > 0)
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
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	GPIO_InitStructure.Pin = LED_PIN.pinNumber;
	HAL_GPIO_Init(LED_PIN.pinPort, &GPIO_InitStructure);
	
	ledTimer.Instance = TIM14;
	ledTimer.Init.Prescaler = 0;
	ledTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	ledTimer.Init.Period = 2000;
	ledTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	ledTimer.Init.RepetitionCounter = 0;
	ledTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&ledTimer) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
	HAL_NVIC_SetPriority(TIM14_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);
}

void PopulatePwmBuffer(TLC5973Registers reg, uint8_t* data, uint8_t bufferSize)
{
	OutputBuffer = 0;
	uint8_t bufferIndex = 0;
	for (uint8_t i = 0; i < REGISTER_MAP_SIZE_BYTES; i++)
	{
		uint8_t pos = 0;
		for (uint8_t j = 7; j != 0; j--)
		{
			FillBit(&data[bufferIndex], pos, (reg.raw[i] & (1 << j)) > 0);
			pos = (pos + 4) % 8;
		}
	}
}

void SendPWM(uint8_t* data, uint8_t length)
{
	if (length > LED_BUFFER_LENGTH_MAX)
	{
		__ASM("BKPT 255");
	}
	// 8 bits per byte. 
	OutputBufferLength = 8 * length;
	outputBufferIndex = 0;
	HAL_TIM_Base_Start_IT(&ledTimer);
}