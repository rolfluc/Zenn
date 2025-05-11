#include "CAN.h"
#include "stm32g0xx_hal.h"
FDCAN_HandleTypeDef hfdcan1;

void TIM16_FDCAN_IT0_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan1);
}

void TIM17_FDCAN_IT1_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan1);
}