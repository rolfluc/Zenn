#include "LEDController.h"
#include "LEDPWM.h"
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"
#include "TLC5973.h"
#include "ADC.h"


#define ADC_BUFFER_COUNT 8
#define TX_BUFFER_SIZE // 4 bits per bit
static uint16_t adcBuffer[ADC_BUFFER_COUNT];
uint8_t buffer0[REGISTER_MAP_SIZE_BYTES];

#define LED_STACK_SIZE configMINIMAL_STACK_SIZE * 2
#define LED_TASK_NAME "LED"
// LED wait time is indefinite. If no one comes calling, don't wake.
static const uint32_t LED_TIME_MS = 0xffffffff;
static TaskHandle_t ledTaskHandle = NULL;
static StackType_t ledStack[LED_STACK_SIZE];
static StaticTask_t ledTaskBuffer;

static const uint32_t LEDControllerDelay_ms = 100;

typedef union 
{
	struct 
	{
		uint16_t adcVal;
		uint16_t RSVD;
	}Data;
	uint32_t raw;
}TaskNotification;

void callback(uint16_t* retVal)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	TaskNotification not = { 0 };
	not.Data.adcVal = *retVal;
	xTaskNotifyFromISR(ledTaskHandle,
		not.raw,
		eSetBits,
		&xHigherPriorityTaskWoken);
}

static void LEDTask(void* argument)
{
	uint8_t ledBufferIndex = 0;
	TaskNotification not;
	InitADC(ADC_0);
	InitPWM();
	InitTLC5973();
	
	for (;;)
	{
		ReadADCWithCallback(ADC_0, callback);
		if (xTaskNotifyWait(0, 0xffffffff, (uint32_t*)&not, LED_TIME_MS) == pdTRUE)
		{
			uint32_t bufferSum = 0;
			adcBuffer[ledBufferIndex] = not.Data.adcVal;
			ledBufferIndex = (ledBufferIndex + 1) % ADC_BUFFER_COUNT; 
			for (uint8_t i = 0; i < ADC_BUFFER_COUNT; i++)
			{
				bufferSum += adcBuffer[i];
			}
			// Divide by 8.
			bufferSum >>= 3;
			SetBacklight(bufferSum);
		}
		vTaskDelay(LEDControllerDelay_ms);
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

// Not used anymore. Reverting to using PWM on Fets.
void SetBacklight(uint16_t ambientAdcCounts)
{
	// TODO adapt to PWM
	//TLC5973Registers reg = ConvertToReg(ambientAdcCounts, ambientAdcCounts, ambientAdcCounts);
	//PopulatePwmBuffer(reg, buffer0, REGISTER_MAP_SIZE_BYTES);
	//SendPWM(buffer0, REGISTER_MAP_SIZE_BYTES);
}