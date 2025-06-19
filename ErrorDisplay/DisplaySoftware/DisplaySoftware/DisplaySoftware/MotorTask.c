#include "MotorTask.h"
#include "stm32g0xx_hal.h"
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"

#include "MotorTimer.h"
#include "Stepper.h"
#include "PinDefs.h"

#define MOT_STACK_SIZE configMINIMAL_STACK_SIZE * 2
#define MOT_TASK_NAME "MOTOR"
// Motor wait time is indefinite. If no one comes calling, don't.
static const uint32_t MOT_TIME_MS = 0xffffffff;
static TaskHandle_t motTaskHandle = NULL;
static StackType_t motStack[MOT_STACK_SIZE];
static StaticTask_t motTaskBuffer;

static Stepper SpeedMotor = {};

static Stepper VoltageMotor = {};

static Stepper TemperatureMotor = {};

typedef union 
{
	struct 
	{
		Motor mtr : 8;
		int32_t motorPosition_tenthsdegrees : 24;
	}Data;
	uint32_t raw;
}TaskNotification;

//Structure here is as follows. Someone will request a new motor position, and this task will receive it, wake up, and set the position at the lower level.
//This stream requires the smarts of the motor processing to be occuring inside of a timer, so the CPU is not responsible for maintaining timing or position in that way.

static void MotorTask(void* argument)
{
	SpeedMotor.P00 = (PinDef*)&P1_0;
	SpeedMotor.P01 = (PinDef*)&P2_0;
	SpeedMotor.P10 = (PinDef*)&P3_0;
	SpeedMotor.P11 = (PinDef*)&P4_0;
	SpeedMotor.currentState = State_0;
	
	VoltageMotor.P00 = (PinDef*)&P1_1;
	VoltageMotor.P01 = (PinDef*)&P2_1;
	VoltageMotor.P10 = (PinDef*)&P3_1;
	VoltageMotor.P11 = (PinDef*)&P4_1;
	VoltageMotor.currentState = State_0;
	
	TemperatureMotor.P00 = (PinDef*)&P1_2;
	TemperatureMotor.P01 = (PinDef*)&P2_2;
	TemperatureMotor.P10 = (PinDef*)&P3_2;
	TemperatureMotor.P11 = (PinDef*)&P4_2;
	TemperatureMotor.currentState = State_0;
	// First Init motor GPIOs.
	InitStepper(&SpeedMotor);
	InitStepper(&VoltageMotor);
	InitStepper(&TemperatureMotor);
	
	static TaskNotification not = { 0 };
	for (;;)
	{
		if (xTaskNotifyWait(0, 0xffffffff, (uint32_t*)&not, MOT_TIME_MS) == pdTRUE)
		{
			switch (not.Data.mtr)
			{
			case Motor0:
				{
					SetPosition(Timebox0, not.Data.motorPosition_tenthsdegrees);
					break;
				}
			case Motor1:
				{
					SetPosition(Timebox1, not.Data.motorPosition_tenthsdegrees);
					break;
				}
			case Motor2:
				{
					SetPosition(Timebox2, not.Data.motorPosition_tenthsdegrees);
					break;
				}
			default:
				break;
			}
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
	xTaskNotify(motTaskHandle, notification.raw, eSetBits);
}