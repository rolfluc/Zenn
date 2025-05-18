#include "ADC.h"
#include "stm32g0xx_hal.h"
#include <stdbool.h>

#include "PinDefs.h"

ADC_HandleTypeDef adc1Handle;

// TODO right now this is just for one channel, confirm there is only 1 ADC being needed, otherwise map adc -> cb. same with In progress.
static adcCallback callback;
static bool inProgress = false;


void ADC1_COMP_IRQHandler(void)
{
	inProgress = false;
	// TODO below might be wrong.
	uint16_t readValue = HAL_ADC_GetValue(&adc1Handle);
	callback(&readValue);
}


void InitADC(ADCChannel channel)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		__ASM("bkpt 255");
	}
	__HAL_RCC_ADC_CLK_ENABLE();
	
	GPIO_InitTypeDef init;
	
	adc1Handle.Instance = ADC1;
	adc1Handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
	adc1Handle.Init.Resolution = ADC_RESOLUTION_8B;
	adc1Handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
	adc1Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	adc1Handle.Init.LowPowerAutoWait = DISABLE;
	adc1Handle.Init.ContinuousConvMode = DISABLE;
	adc1Handle.Init.NbrOfConversion = 1;
	adc1Handle.Init.DiscontinuousConvMode = DISABLE;
	adc1Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adc1Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	adc1Handle.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	adc1Handle.Init.OversamplingMode = DISABLE;
	adc1Handle.Init.DataAlign = ADC_DATAALIGN_LEFT;
	adc1Handle.Init.LowPowerAutoPowerOff = DISABLE;
	adc1Handle.Init.DMAContinuousRequests = DISABLE;
	// adc1Handle.Init.SamplingTimeCommon1
	// adc1Handle.Init.SamplingTimeCommon2
	// adc1Handle.Init.Oversampling
	// adc1Handle.Init.TriggerFrequencyMode
	if (HAL_ADC_Init(&adc1Handle) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
	
	HAL_ADC_MspInit(&adc1Handle);
	
	init.Mode = GPIO_MODE_ANALOG; 
	init.Pull = GPIO_NOPULL;
	init.Pin = AMBIENT_PIN.pinNumber;
	HAL_GPIO_Init(AMBIENT_PIN.pinPort, &init);
	
	HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
	
	ADC_ChannelConfTypeDef sConfig = { 0 };
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	
	if (HAL_ADC_ConfigChannel(&adc1Handle, &sConfig) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
}


void ReadADCWithCallback(ADCChannel channel, adcCallback callback)
{
	// Do not interrupt. 
	if (inProgress)
	{
		// If below is hit, timing needs to be adjusted.
		__ASM("bkpt 255");
		return;
	}
	
	HAL_ADC_Start_IT(&adc1Handle);
	inProgress = true;
}
