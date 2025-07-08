#include "CAN.h"
#include "stm32g0xx_hal.h"
#include "PinDefs.h"

static const uint16_t FILTER_ID = 0x03ff; // TODO
static const uint8_t FILTER_INDEX = 0;

FDCAN_HandleTypeDef hfdcan1;
FDCAN_FilterTypeDef rxFilter;

cancb canCallback;

void TIM16_FDCAN_IT0_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan1);
	__ASM("bkpt 255");
}

void TIM17_FDCAN_IT1_IRQHandler(void)
{
	HAL_FDCAN_IRQHandler(&hfdcan1);
	__ASM("bkpt 255");
}

/* TxEventFifoCallback */
void HAL_FDCAN_TxEventFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TxEventFifoITs)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_TxBufferAbortCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_TimestampWraparoundCallback(FDCAN_HandleTypeDef *hfdcan)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_TimeoutOccurredCallback(FDCAN_HandleTypeDef *hfdcan)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_HighPriorityMessageCallback(FDCAN_HandleTypeDef *hfdcan)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan)
{
	__ASM("bkpt 255");
}
void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
	__ASM("bkpt 255");
}

static void InitPins()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = CAN_MCU_TXD.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF3_FDCAN1;
	HAL_GPIO_Init(CAN_MCU_TXD.pinPort, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Pin = CAN_MCU_RXD.pinNumber;
	HAL_GPIO_Init(CAN_MCU_RXD.pinPort, &GPIO_InitStruct);
	
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
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

void InitCAN(cancb cb)
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
	hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV8;
	hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan1.Init.AutoRetransmission = DISABLE;
	hfdcan1.Init.TransmitPause = DISABLE;
	hfdcan1.Init.ProtocolException = DISABLE;
	hfdcan1.Init.NominalPrescaler = 1;		// Specifies the value by which the oscillator frequency is divided for generating the nominal bit time quanta.
	hfdcan1.Init.NominalSyncJumpWidth = 1;  // Specifies the maximum number of time quanta the FDCAN hardware is allowed to lengthen or shorten a bit to perform resynchronization.
	hfdcan1.Init.NominalTimeSeg1 = 2;		// Specifies the number of time quanta in Bit Segment 1.
	hfdcan1.Init.NominalTimeSeg2 = 2;		// Specifies the number of time quanta in Bit Segment 2.
	hfdcan1.Init.DataPrescaler = 1;			// Specifies the value by which the oscillator frequency is divided for generating the data bit time quanta.
	hfdcan1.Init.DataSyncJumpWidth = 1;		// Specifies the maximum number of time quanta the FDCAN hardware is allowed to lengthen or shorten a data bit to perform resynchronization.
	hfdcan1.Init.DataTimeSeg1 = 1;			// Specifies the number of time quanta in Data Bit Segment 1.
	hfdcan1.Init.DataTimeSeg2 = 1;			// Specifies the number of time quanta in Data Bit Segment 2.
	hfdcan1.Init.StdFiltersNbr = 1;			// Specifies the number of standard Message ID filters. 
	hfdcan1.Init.ExtFiltersNbr = 0;			// Specifies the number of extended Message ID filters.
	hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
	{
		__ASM("bkpt 255");
	}
	
	rxFilter.IdType = FDCAN_STANDARD_ID; // 0-> 0x7FF
	rxFilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	rxFilter.FilterType = FDCAN_FILTER_RANGE; //Range filter from FilterID1 to FilterID2
	rxFilter.FilterID1 = FILTER_ID; // TDOO 
	rxFilter.FilterID2 = FILTER_ID + 2; // TODO 
	rxFilter.FilterIndex = FILTER_INDEX;
	HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan1, FDCAN_RX_FIFO0, FDCAN_RX_FIFO_OVERWRITE);
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &rxFilter) != HAL_OK)
	{
		__ASM("bkpt 255");
	}
	HAL_NVIC_SetPriority(TIM16_FDCAN_IT0_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM16_FDCAN_IT0_IRQn);
	HAL_NVIC_SetPriority(TIM17_FDCAN_IT1_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM17_FDCAN_IT1_IRQn);
	
	const uint32_t IT_LIST = FDCAN_IT_GROUP_RX_FIFO0;
	HAL_FDCAN_ConfigInterruptLines(&hfdcan1,
		IT_LIST,
		FDCAN_INTERRUPT_LINE0);
	
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		__ASM("bkpt 255");
	}
	
	// TODO get with HAL_FDCAN_GetRxMessage
	// HAL_FDCAN_RegisterCallback
	canCallback = cb;
}