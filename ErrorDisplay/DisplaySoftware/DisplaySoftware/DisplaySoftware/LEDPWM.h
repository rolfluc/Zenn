#pragma once
#include <stdint.h>
#include "TLC5973.h"
void InitPWM();
void SendPWM(uint8_t* data, uint8_t length);
void PopulatePwmBuffer(TLC5973Registers reg, uint8_t* data, uint8_t bufferSize);