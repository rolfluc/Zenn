#pragma once
#include "stm32g0b1xx.h"
typedef struct
{
	uint32_t pinNumber;
	GPIO_TypeDef* pinPort;
}PinDef;

