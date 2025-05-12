#include "TLC5973.h"
#include <stdint.h>

#define REGISTER_MAP_SIZE_BYTES 6
static const uint16_t WriteCommand = 0x03AA;
typedef union
{
	uint8_t raw[6];
	struct 
	{
		uint16_t GSOUT2 : 12;
		uint16_t GSOUT1 : 12;
		uint16_t GSOUT0 : 12;
		uint16_t WRTCMD : 12; // Data is only copied when this value is 0x03AA
	}Registers;
}TLC5973Registers;
void InitTLC5973();