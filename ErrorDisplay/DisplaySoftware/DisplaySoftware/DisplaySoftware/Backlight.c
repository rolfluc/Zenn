#include "Backlight.h"

#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"
#include "stm32g0xx_hal.h"
#include "LEDController.h"
#include "ADC.h"
#include "stdbool.h"

#define BACKLIGHT_BUFFER_SIZE 3
#define BACKLIGHT_STACK_SIZE configMINIMAL_STACK_SIZE
#define HBACKLIGHT_TASK_NAME "Backlight"

static const ADCChannel AmbientSensor = ADC_0;
static const uint32_t BACKLIGHT_CHECK_RATE_MS = 500;
static const uint32_t AMBIENT_WAIT_TIME_MS = 5;
static TaskHandle_t backlightTaskHandle = NULL;
static StackType_t backlightStack[BACKLIGHT_STACK_SIZE];
static StaticTask_t backlightTaskBuffer;

void adcReadCompleteCallback(uint16_t* readValue)
{
	BaseType_t higherPrioAwaken = false;
	uint16_t val = *readValue;
	xTaskNotifyFromISR(backlightTaskHandle, (uint32_t)val, eSetBits, &higherPrioAwaken);
}

//
// Thought Process is : Every BACKLIGHT_CHECK_RATE_MS, read the ADC, 
// Wait for the response, then send the filtered results to the LEDController
//
void backlightTask()
{
	bool hasRolledOver = false;
	uint8_t backlightBufferPointer = 0;
	uint16_t backlightBuffer[BACKLIGHT_BUFFER_SIZE] = { 0 };
	uint32_t lastAmbientValue = 0;
	vTaskDelay(BACKLIGHT_CHECK_RATE_MS);
	ReadADCWithCallback(AmbientSensor, adcReadCompleteCallback);
	
	if (xTaskNotifyWait(0, 0xffffffff, &lastAmbientValue, AMBIENT_WAIT_TIME_MS) == pdTRUE)
	{
		backlightBuffer[backlightBufferPointer] = lastAmbientValue;
		backlightBufferPointer = (backlightBufferPointer + 1) % BACKLIGHT_BUFFER_SIZE;
		hasRolledOver = hasRolledOver | (backlightBufferPointer == 0);
		if (hasRolledOver)
		{
			// Have enough samples to do something with. Send the average over to the LED Controller to determine the setting.
			
			
		}
	}
	else
	{
		// TODO error condition
		__ASM("BKPT 255");
	}
}



void CreateBacklightTask()
{
	InitADC(AmbientSensor);
	backlightTaskHandle = xTaskCreateStatic(
		backlightTask,
		HBACKLIGHT_TASK_NAME,
		BACKLIGHT_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY,
		backlightStack,
		&backlightTaskBuffer);
}