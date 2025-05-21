#include "Stepper.h"
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/task.h"

// Coil0:
//  012345678910
//	--    --
//    -  -  -
//     --    --

// Coil1
//  012345678910
//	-    --
//   -  -  -  -
//    --    --
#define STEP_COUNT 6
static const uint32_t maxDegrees = 315; 
static const uint32_t maxSteps = maxDegrees * 3; 
static const uint32_t HOME_STEP_COUNT = 1;
static const uint32_t HOME_DELAY_MS = 10;

typedef void(*stepcb)(Stepper*);

static inline void Step1(Stepper* step)
{
	//1001
	// Set
	step->P00->pinPort->BSRR = step->P00->pinNumber;
	step->P11->pinPort->BSRR = step->P11->pinNumber;
	// Reset
	step->P01->pinPort->BSRR = step->P01->pinNumber;
	step->P10->pinPort->BSRR = step->P10->pinNumber;
}

static inline void Step2(Stepper* step)
{
	//1000
	// Set
	step->P00->pinPort->BSRR = step->P00->pinNumber;
	// Reset
	step->P01->pinPort->BSRR = step->P01->pinNumber;
	step->P10->pinPort->BSRR = step->P10->pinNumber;
	step->P11->pinPort->BSRR = step->P11->pinNumber;
}

static inline void Step3(Stepper* step)
{
	//1110
	// Set
	step->P00->pinPort->BSRR = step->P00->pinNumber;
	step->P01->pinPort->BSRR = step->P01->pinNumber;
	step->P10->pinPort->BSRR = step->P10->pinNumber;
	// Reset
	step->P11->pinPort->BSRR = step->P11->pinNumber;
}

static inline void Step4(Stepper* step)
{
	//0110
	// Set
	step->P01->pinPort->BSRR = step->P01->pinNumber;
	step->P10->pinPort->BSRR = step->P10->pinNumber;
	// Reset
	step->P00->pinPort->BSRR = step->P00->pinNumber;
	step->P11->pinPort->BSRR = step->P11->pinNumber;
}

static inline void Step5(Stepper* step)
{
	//0111
	// Set
	step->P01->pinPort->BSRR = step->P01->pinNumber;
	step->P10->pinPort->BSRR = step->P10->pinNumber;
	step->P11->pinPort->BSRR = step->P11->pinNumber;
	// Reset
	step->P00->pinPort->BSRR = step->P00->pinNumber;
}

static inline void Step6(Stepper* step)
{
	//0001
	// Set
	step->P11->pinPort->BSRR = step->P11->pinNumber;
	// Reset
	step->P00->pinPort->BSRR = step->P00->pinNumber;
	step->P01->pinPort->BSRR = step->P01->pinNumber;
	step->P10->pinPort->BSRR = step->P10->pinNumber;
}

static inline void StepNone(Stepper* step)
{
	step->P00->pinPort->BRR = step->P00->pinNumber;
	step->P01->pinPort->BRR = step->P01->pinNumber;
	step->P10->pinPort->BRR = step->P10->pinNumber;
	step->P11->pinPort->BRR = step->P11->pinNumber;
}

stepcb clockwiseCallbacks[STEP_COUNT] = {
	Step1,
	Step2,
	Step3,
	Step4,
	Step5,
	Step6
};
stepcb counterClockwiseCallbacks[STEP_COUNT] = { 
	Step6,
	Step5,
	Step4,
	Step3,
	Step2,
	Step1
};

void InitStepper(Stepper* step)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	GPIO_InitStructure.Pin = step->P00->pinNumber;
	HAL_GPIO_Init(step->P00->pinPort, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = step->P01->pinNumber;
	HAL_GPIO_Init(step->P01->pinPort, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = step->P10->pinNumber;
	HAL_GPIO_Init(step->P10->pinPort, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = step->P11->pinNumber;
	HAL_GPIO_Init(step->P11->pinPort, &GPIO_InitStructure);
	
	DriveHomeBlocking(step);
}

void DriveHomeBlocking(Stepper* step)
{
	uint8_t currentCbIndex = 0;
	for (uint32_t i = 0; i < HOME_STEP_COUNT; i++)
	{
		counterClockwiseCallbacks[currentCbIndex](step);
		currentCbIndex = (currentCbIndex + 1) % HOME_STEP_COUNT;
		vTaskDelay(HOME_DELAY_MS);
	}
	// Clear after home.
	StepNone(step);
}
