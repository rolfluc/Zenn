#include "Err.h"
#include "stm32g4xx_hal.h"
void Error_Handler()
{
	__ASM("bkpt 255");
}