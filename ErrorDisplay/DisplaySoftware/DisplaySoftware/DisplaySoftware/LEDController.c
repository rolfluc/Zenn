#include "LEDController.h"
#include "LEDPWM.h"
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"
#include "TLC5973.h"
#include "ADC.h"

uint8_t buffer0[REGISTER_MAP_SIZE_BYTES];

#define LED_STACK_SIZE configMINIMAL_STACK_SIZE * 2
#define LED_TASK_NAME "LED"
// Motor wait time is indefinite. If no one comes calling, don't.
static const uint32_t LED_TIME_MS = 0xffffffff;
static TaskHandle_t ledTaskHandle = NULL;
static StackType_t ledStack[LED_STACK_SIZE];
static StaticTask_t ledTaskBuffer;

void callback(uint16_t* retVal)
{
	
}

static void LEDTask(void* argument)
{
	InitADC(ADC_0);
	InitPWM();
	InitTLC5973();
	ReadADCWithCallback(ADC_0, callback);
	SetBacklight(0x00ff);
	for (;;)
	{
		//if (xTaskNotifyWait(0, 0xffffffff, (uint32_t*)&not, MOT_TIME_MS) == pdTRUE)
		//
			
		//
	}
}

void InitBacklightLEDs()
{
	ledTaskHandle = xTaskCreateStatic(
		LEDTask,
		LED_TASK_NAME,
		LED_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY,
		ledStack,
		&ledTaskBuffer);
}

void SetBacklight(uint16_t ambientAdcCounts)
{
	FillWriteBuffer(buffer0, ambientAdcCounts);
	SendPWM(buffer0, REGISTER_MAP_SIZE_BYTES);
}