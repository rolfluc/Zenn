#include "PWM.h"
#include "stm32g4xx_hal.h"
#include "stdbool.h"
#include "Err.h"

#define PWM_COUNTS 7500

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1_ch1;
DMA_HandleTypeDef hdma_tim1_ch2;
DMA_HandleTypeDef hdma_tim1_ch3;
DMA_HandleTypeDef hdma_tim1_ch4;

static PWM RunningPair = PWMPair_None;
static bool ArmRunning = false;

void DMA1_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_tim1_ch1);
}
void DMA1_Channel2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_tim1_ch2);
}
void DMA1_Channel3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_tim1_ch3);
}
void DMA1_Channel4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_tim1_ch4);
}

void DMAMUX_OVR_IRQHandler(void)
{
	// Handle DMA1_Channel1
	HAL_DMAEx_MUX_IRQHandler(&hdma_tim1_ch1);
	// Handle DMA1_Channel2
	HAL_DMAEx_MUX_IRQHandler(&hdma_tim1_ch2);
	// Handle DMA1_Channel3
	HAL_DMAEx_MUX_IRQHandler(&hdma_tim1_ch3);
	// Handle DMA1_Channel4
	HAL_DMAEx_MUX_IRQHandler(&hdma_tim1_ch4);
}


void InitPWM()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	/* TIM1_CH1 Init */
	hdma_tim1_ch1.Instance = DMA1_Channel1;
	hdma_tim1_ch1.Init.Request = DMA_REQUEST_TIM1_CH1;
	hdma_tim1_ch1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim1_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim1_ch1.Init.MemInc = DMA_MINC_DISABLE;
	hdma_tim1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim1_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim1_ch1.Init.Mode = DMA_CIRCULAR;
	hdma_tim1_ch1.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_tim1_ch1) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_CC1], hdma_tim1_ch1);
	
	/* TIM1_CH2 Init */
	hdma_tim1_ch2.Instance = DMA1_Channel2;
	hdma_tim1_ch2.Init.Request = DMA_REQUEST_TIM1_CH2;
	hdma_tim1_ch2.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim1_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim1_ch2.Init.MemInc = DMA_MINC_DISABLE;
	hdma_tim1_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim1_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim1_ch2.Init.Mode = DMA_CIRCULAR;
	hdma_tim1_ch2.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_tim1_ch2) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_CC2], hdma_tim1_ch2);

	/* TIM1_CH3 Init */
	hdma_tim1_ch3.Instance = DMA1_Channel3;
	hdma_tim1_ch3.Init.Request = DMA_REQUEST_TIM1_CH3;
	hdma_tim1_ch3.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim1_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim1_ch3.Init.MemInc = DMA_MINC_DISABLE;
	hdma_tim1_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim1_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim1_ch3.Init.Mode = DMA_CIRCULAR;
	hdma_tim1_ch3.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_tim1_ch3) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_CC3], hdma_tim1_ch3);

	/* TIM1_CH4 Init */
	hdma_tim1_ch4.Instance = DMA1_Channel4;
	hdma_tim1_ch4.Init.Request = DMA_REQUEST_TIM1_CH4;
	hdma_tim1_ch4.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim1_ch4.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim1_ch4.Init.MemInc = DMA_MINC_DISABLE;
	hdma_tim1_ch4.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim1_ch4.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim1_ch4.Init.Mode = DMA_CIRCULAR;
	hdma_tim1_ch4.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_tim1_ch4) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_CC4], hdma_tim1_ch4);

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	/* DMA1_Channel2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	/* DMA1_Channel3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	/* DMA1_Channel4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	/* DMAMUX_OVR_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMAMUX_OVR_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMAMUX_OVR_IRQn);
	
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = PWM_COUNTS;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF11_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

uint16_t buffer0 = 0;

static inline uint16_t PercentToCounts(uint8_t percent)
{
	uint32_t tmp = PWM_COUNTS * percent;
	return (uint16_t)(tmp / 100);
}

void SetPWMPair(PWM pair, uint8_t percent)
{
	// TODO potentially do not stop the inverted. Maybe just drive them to 1, ensuring the PFET is turned off. 
	// Might not be a big deal since both nfets should be off. but TBD.
	// If 0 is set, just kill them all. // TODO maybe stop the TIM? or maybe kill the GPIOs just to do it faster?
	if (percent == 0)
	{
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_4);
		RunningPair = PWMPair_None;
		return;
	}
	
	// Already running? Just update
	if (pair == RunningPair && RunningPair != PWMPair_None)
	{
		buffer0 = PercentToCounts(percent);
		return;
	}
	// Need to change direction? stop it all, then let the rest of the system manage.
	else
	{
		if (RunningPair == PWMPair_0)
		{
			HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_2);
			RunningPair = PWMPair_None;
		}
		else if (RunningPair == PWMPair_1)
		{
			HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_3);
			HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_4);
			RunningPair = PWMPair_None;
		}
	}
	buffer0 = PercentToCounts(percent);
	switch (pair)
	{
	case PWMPair_0:
		{
			RunningPair = PWMPair_0;
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)&buffer0, 1);
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2, (uint32_t*)&buffer0, 1);
			break;
		}
	case PWMPair_1:
		{
			RunningPair = PWMPair_1;
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t*)&buffer0, 1);
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_4, (uint32_t*)&buffer0, 1);
			break;
		}
	default:
		break;
	}
}

static uint16_t ArmBuffer = 0;
void SetPWMArm(uint8_t percent)
{
	if (percent == 0)
	{
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
		ArmRunning = false;
		return;
	}
	ArmBuffer = PercentToCounts(percent);
	if (!ArmRunning)
	{
		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)&ArmBuffer, 1);
	}
}