#include "TLC5973.h"
#include <stdint.h>
#include <memory.h>

void InitTLC5973()
{
	
}

void FillWriteBuffer(uint8_t* buffer, uint16_t level)
{
	TLC5973Registers reg = { 0 };
	reg.Registers.GSOUT0 = level;
	reg.Registers.GSOUT1 = level;
	reg.Registers.GSOUT2 = level;
	reg.Registers.WRTCMD = WriteCommand;
	memcpy((void*)buffer, (void*)reg.raw, sizeof(TLC5973Registers));
}