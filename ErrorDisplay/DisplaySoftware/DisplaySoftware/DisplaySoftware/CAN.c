#include "CAN.h"
#include "stm32g0xx_hal.h"
#include "PinDefs.h"
FDCAN_HandleTypeDef hfdcan1;

void TIM16_FDCAN_IT0_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan1);
}

void TIM17_FDCAN_IT1_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan1);
}

static void InitPins()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = CAN_MCU_TXD.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_FDCAN1;
	HAL_GPIO_Init(CAN_MCU_TXD.pinPort, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = CAN_MCU_RXD.pinNumber;
	HAL_GPIO_Init(CAN_MCU_RXD.pinPort, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = CAN_STDBY.pinNumber;
	HAL_GPIO_Init(CAN_STDBY.pinPort, &GPIO_InitStruct);
	
	// TODO 
	HAL_GPIO_WritePin(CAN_STDBY.pinPort, CAN_STDBY.pinNumber, GPIO_PIN_SET);
	
	HAL_NVIC_SetPriority(TIM16_FDCAN_IT0_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM16_FDCAN_IT0_IRQn);
	HAL_NVIC_SetPriority(TIM17_FDCAN_IT1_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM17_FDCAN_IT1_IRQn);
}

void InitCAN()
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
	PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		__ASM("bkpt 255");
	}
	
	__HAL_RCC_FDCAN_CLK_ENABLE();
	hfdcan1.Instance = FDCAN1;
	hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV2;
	hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan1.Init.AutoRetransmission = DISABLE;
	hfdcan1.Init.TransmitPause = DISABLE;
	hfdcan1.Init.ProtocolException = DISABLE;
	hfdcan1.Init.NominalPrescaler = 1;
	hfdcan1.Init.NominalSyncJumpWidth = 1;
	hfdcan1.Init.NominalTimeSeg1 = 2;
	hfdcan1.Init.NominalTimeSeg2 = 2;
	hfdcan1.Init.DataPrescaler = 1;
	hfdcan1.Init.DataSyncJumpWidth = 1;
	hfdcan1.Init.DataTimeSeg1 = 1;
	hfdcan1.Init.DataTimeSeg2 = 1;
	hfdcan1.Init.StdFiltersNbr = 0;
	hfdcan1.Init.ExtFiltersNbr = 0;
	hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
	{
		__ASM("bkpt 255");
	}
}