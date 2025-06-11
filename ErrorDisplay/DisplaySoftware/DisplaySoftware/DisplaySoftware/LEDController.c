#include "LEDController.h"
#include "LEDPWM.h"
#include "TLC5973.h"

uint8_t buffer0[REGISTER_MAP_SIZE_BYTES];

void InitBacklightLEDs()
{
	InitPWM();
	InitTLC5973();
	SetBacklight(0x00ff);
}

void SetBacklight(uint16_t ambientAdcCounts)
{
	FillWriteBuffer(buffer0, ambientAdcCounts);
	SendPWM(buffer0, REGISTER_MAP_SIZE_BYTES);
}