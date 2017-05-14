#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"



typedef struct 
{
	uint8_t KeyValue;
	uint8_t KeyState;
}KEYINFO_S;


extern uint8_t KeyRead(void);
extern void KeyHardwareConfig(void);










#endif





