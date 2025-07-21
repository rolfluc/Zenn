#pragma once
typedef enum 
{
	Switch_0,
	Switch_None,
	Switch_1
}SwitchSetting;
void InitSwitches();
SwitchSetting ReadSwitch();