#include "MotorTask.h"
#include "stm32g0xx_hal.h"
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"

#include "PinDefs.h"

#define MOT_STACK_SIZE configMINIMAL_STACK_SIZE * 2
#define MOT_TASK_NAME "MOTOR"
// Motor wait time is indefinite. If no one comes calling, don't.
static const uint32_t MOT_TIME_MS = 0xffffffff;
static TaskHandle_t motTaskHandle = NULL;
static StackType_t motStack[MOT_STACK_SIZE];
static StaticTask_t motTaskBuffer;

typedef union 
{
	struct 
	{
		Motor mtr : 16;
		int16_t motorPosition_tenthsdegrees : 16;
	}Data;
	uint32_t raw;
}TaskNotification;

//Structure here is as follows. Someone will request a new motor position, and this task will receive it, wake up, and set the position at the lower level.
//This stream requires the smarts of the motor processing to be occuring inside of a timer, so the CPU is not responsible for maintaining timing or position in that way.

static void MotorTask(void* argument)
{
	static TaskNotification not = { 0 };
	for (;;)
	{
		if (xTaskNotifyWait(0, 0xffffffff, (uint32_t*)&not, MOT_TIME_MS) == pdTRUE)
		{
			
		}
	}
}

void InitMotors()
{
	motTaskHandle = xTaskCreateStatic(
		MotorTask,
		MOT_TASK_NAME,
		MOT_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY,
		motStack,
		&motTaskBuffer);
}

void SendMotorPosition(Motor motor, int16_t motorPosition_tenthsdegrees)
{
	// TODO notify the task from here.
	TaskNotification notification = { 0 };
	notification.Data.mtr = motor;
	notification.Data.motorPosition_tenthsdegrees = motorPosition_tenthsdegrees;
}