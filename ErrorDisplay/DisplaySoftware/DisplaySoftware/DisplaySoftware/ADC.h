#pragma once
#include <stdint.h>
typedef enum 
{
	ADC_0
}ADCChannel;

typedef void (*adcCallback)(uint16_t*);

void InitADC(ADCChannel channel);
void ReadADCWithCallback(ADCChannel channel, adcCallback callback);