#pragma once
#include <stdint.h>
typedef enum
{
	ADC_0,
	ADC_1
}ADCNum;

void InitADC();
uint8_t ReadADC_Percent(ADCNum adc);