#pragma once
#include "stm32g0xx_hal.h"
typedef struct
{
	uint32_t pinNumber;
	GPIO_TypeDef* pinPort;
}PinDef;

#pragma region CANBUS

static const PinDef CAN_MCO = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOF };
static const PinDef CAN_nRX0BF = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOF };
static const PinDef CAN_nRX1BF = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOF };

static const PinDef CAN_nINT = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOB };
static const PinDef CAN_nRST = { .pinNumber = GPIO_PIN_7, .pinPort = GPIOB };
static const PinDef CAN_nTX0RTS = { .pinNumber = GPIO_PIN_8, .pinPort = GPIOB };
static const PinDef CAN_MCU_TXD = { .pinNumber = GPIO_PIN_9, .pinPort = GPIOB };
static const PinDef CAN_MCU_RXD = { .pinNumber = GPIO_PIN_9, .pinPort = GPIOA };
static const PinDef CAN_STDBY = { .pinNumber = GPIO_PIN_3, .pinPort = GPIOA };

static const PinDef CAN_MISO = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOA };
static const PinDef CAN_MOSI = { .pinNumber = GPIO_PIN_2, .pinPort = GPIOA };
static const PinDef CAN_SCK = { .pinNumber = GPIO_PIN_1, .pinPort = GPIOA };
static const PinDef CAN_nCS = { .pinNumber = GPIO_PIN_0, .pinPort = GPIOA };
				  
#pragma endregion // CANBUS

#pragma region Motors
static const PinDef P1_0 = { .pinNumber = GPIO_PIN_4, .pinPort = GPIOB };
static const PinDef P2_0 = { .pinNumber = GPIO_PIN_5, .pinPort = GPIOB };
static const PinDef P3_0 = { .pinNumber = GPIO_PIN_15, .pinPort = GPIOA };
static const PinDef P4_0 = { .pinNumber = GPIO_PIN_3, .pinPort = GPIOB };

static const PinDef P1_1 = { .pinNumber = GPIO_PIN_14, .pinPort = GPIOC };
static const PinDef P2_1 = { .pinNumber = GPIO_PIN_5, .pinPort = GPIOA };
static const PinDef P3_1 = { .pinNumber = GPIO_PIN_7, .pinPort = GPIOA };
static const PinDef P4_1 = { .pinNumber = GPIO_PIN_15, .pinPort = GPIOC };

static const PinDef P1_2 = { .pinNumber = GPIO_PIN_10, .pinPort = GPIOA };
static const PinDef P2_2 = { .pinNumber = GPIO_PIN_12, .pinPort = GPIOA };
static const PinDef P3_2 = { .pinNumber = GPIO_PIN_8, .pinPort = GPIOA };
static const PinDef P4_2 = { .pinNumber = GPIO_PIN_9, .pinPort = GPIOA };				  
#pragma endregion //Motors

static const PinDef LED_PIN = { .pinNumber = GPIO_PIN_2, .pinPort = GPIOB };
static const PinDef HB_PIN = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOC };
static const PinDef AMBIENT_PIN = { .pinNumber = GPIO_PIN_4, .pinPort = GPIOA };



