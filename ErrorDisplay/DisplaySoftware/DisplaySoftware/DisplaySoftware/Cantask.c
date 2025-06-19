#include "Cantask.h"
#include "CAN.h"
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"
#include "MotorTask.h"

#define CAN_STACK_SIZE configMINIMAL_STACK_SIZE * 2
#define CAN_TASK_NAME "CAN"
// Motor wait time is indefinite. If no one comes calling, don't.
static const uint32_t CAN_TIME_MS = 0xffffffff;
static TaskHandle_t canTaskHandle = NULL;
static StackType_t canStack[CAN_STACK_SIZE];
static StaticTask_t canTaskBuffer;
static const int16_t MotorHomeSteps = 0x83ff; // TODO 
static const uint32_t InitDelay_ms = 10000;

typedef union 
{
	struct 
	{
		int32_t canData : 32;
	}Data;
	uint32_t raw;
}TaskNotification;

void CanCallback()
{
	// TODO notify.
}

static void CanTask(void* argument)
{
	// InitCAN(CanCallback);
	SendMotorPosition(Motor0, MotorHomeSteps);
	vTaskDelay(InitDelay_ms);
	SendMotorPosition(Motor1, MotorHomeSteps);
	vTaskDelay(InitDelay_ms);
	SendMotorPosition(Motor2, MotorHomeSteps);
	vTaskDelay(InitDelay_ms);
	static TaskNotification not = { 0 };
	for (;;)
	{
		if (xTaskNotifyWait(0, 0xffffffff, (uint32_t*)&not, CAN_TIME_MS) == pdTRUE)
		{
			
		}
	}
}

void InitCanTask()
{
	canTaskHandle = xTaskCreateStatic(
	CanTask,
		CAN_TASK_NAME,
		CAN_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY,
		canStack,
		&canTaskBuffer);
}