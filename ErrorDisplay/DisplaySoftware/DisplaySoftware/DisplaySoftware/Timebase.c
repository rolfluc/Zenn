#include "Timebase.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_tim.h"

TIM_HandleTypeDef htim2;

void TIM2_IRQHandler(void)
{
	/* USER CODE BEGIN TIM2_IRQn 0 */

	/* USER CODE END TIM2_IRQn 0 */
	HAL_TIM_IRQHandler(&htim2);
	/* USER CODE BEGIN TIM2_IRQn 1 */

	/* USER CODE END TIM2_IRQn 1 */
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	RCC_ClkInitTypeDef    clkconfig;
	uint32_t              uwTimclock = 0;
	uint32_t              uwPrescalerValue = 0;
	uint32_t              pFLatency;
	/*Configure the TIM2 IRQ priority */
	HAL_NVIC_SetPriority(TIM2_IRQn, TickPriority, 0);

	/* Enable the TIM2 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	/* Enable TIM2 clock */
	__HAL_RCC_TIM2_CLK_ENABLE();

	/* Get clock configuration */
	HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

	/* Compute TIM2 clock */
	uwTimclock = HAL_RCC_GetPCLK1Freq();
	/* Compute the prescaler value to have TIM2 counter clock equal to 1MHz */
	uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

	/* Initialize TIM2 */
	htim2.Instance = TIM2;

	/* Initialize TIMx peripheral as follow:
	+ Period = [(TIM2CLK/1000) - 1]. to have a (1/1000) s time base.
	+ Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
	+ ClockDivision = 0
	+ Counter direction = Up
	*/
	htim2.Init.Period = (1000000U / 1000U) - 1U;
	htim2.Init.Prescaler = uwPrescalerValue;
	htim2.Init.ClockDivision = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;

	if (HAL_TIM_Base_Init(&htim2) == HAL_OK)
	{
		/* Start the TIM time Base generation in interrupt mode */
		return HAL_TIM_Base_Start_IT(&htim2);
	}

	/* Return function status */
	return HAL_ERROR;
}

void HAL_SuspendTick(void)
{
	/* Disable TIM2 update Interrupt */
	__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void)
{
	/* Enable TIM2 Update interrupt */
	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
}
