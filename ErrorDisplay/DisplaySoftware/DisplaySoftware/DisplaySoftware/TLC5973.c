#include "TLC5973.h"
#include <stdint.h>
#include <memory.h>

void InitTLC5973()
{
	// TODO probably nothing actually. 
}

TLC5973Registers ConvertToReg(uint16_t level0, uint16_t level1, uint16_t level2)
{
	TLC5973Registers reg = { 0 };
	reg.Registers.GSOUT0 = level0;
	reg.Registers.GSOUT1 = level1;
	reg.Registers.GSOUT2 = level2;
	reg.Registers.WRTCMD = WriteCommand;
	return reg;
}