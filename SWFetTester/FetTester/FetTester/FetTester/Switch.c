#include "Switch.h"
#include "stm32g4xx_hal.h"
void InitSwitches()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_PIN_6; 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5; 
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}

SwitchSetting ReadSwitch()
{
	SwitchSetting retSetting = Switch_None;
	GPIO_PinState pinStat0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
	GPIO_PinState pinStat1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	if (pinStat0 == GPIO_PIN_RESET && pinStat1 == GPIO_PIN_RESET)
	{
		return retSetting;
	}
	else if (pinStat0 == GPIO_PIN_RESET)
	{
		retSetting = Switch_0;
	}
	else if (pinStat1 == GPIO_PIN_RESET)
	{
		retSetting = Switch_1;
	}
	return retSetting;
}