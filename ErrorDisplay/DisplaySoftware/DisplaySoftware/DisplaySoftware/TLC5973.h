#pragma once
#include "stdint.h"
#define REGISTER_MAP_SIZE_BYTES 6
typedef enum
{
	LEDNone,
	LED_ONE,
	LED_TWO,
	LED_THREE,
}LEDNNumber;

static const uint16_t WriteCommand = 0x03AA;
typedef union
{
	uint8_t raw[REGISTER_MAP_SIZE_BYTES];
	struct 
	{
		uint16_t GSOUT2 : 12;
		uint16_t GSOUT1 : 12;
		uint16_t GSOUT0 : 12;
		uint16_t WRTCMD : 12; // Data is only copied when this value is 0x03AA
	}Registers;
}TLC5973Registers;

void InitTLC5973();
void FillWriteBuffer(uint8_t* buffer, uint16_t level);
