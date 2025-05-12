#include "Heartbeat.h"
#include "stm32g0xx_hal.h"
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"

#define HB_STACK_SIZE configMINIMAL_STACK_SIZE
#define HB_TASK_NAME "HB"
static const uint32_t HB_TIME_MS = 500;
static TaskHandle_t hbTaskHandle = NULL;
static StackType_t hbStack[HB_STACK_SIZE];
static StaticTask_t hbTaskBuffer;

static void Heatbeat(void *argument)\
{
	for (;;)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
		vTaskDelay(HB_TIME_MS);
	}
}

void StartHeartbeat()
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	hbTaskHandle = xTaskCreateStatic(
		Heatbeat,
		HB_TASK_NAME,
		HB_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY,
		hbStack,
		&hbTaskBuffer);
}