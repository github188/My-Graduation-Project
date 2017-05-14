#include "Key.h"


#define KEY_PRESSED 0x12
#define KEY_RELEASED 0x00


#define KEY_VALUE_1 		10	
#define KEY_VALUE_2			11
#define KEY_VALUE_3			12
#define KEY_VALUE_4			13
#define KEY_VALUE_5			14
#define KEY_VALUE_6			15
#define KEY_VALUE_7			16
#define KEY_VALUE_8			17
#define KEY_VALUE_9			18
#define KEY_VALUE_0			20
#define KEY_VALUE_DOT		21	

#define KEY_VALUE_LOCK		19

#define KEY_VALUE_LEFT		25	
#define KEY_VALUE_RIGHT		27	
#define KEY_VALUE_UP		23	
#define KEY_VALUE_DOWN		29	
#define KEY_VALUE_EDIT		26	
#define KEY_VALUE_ESC		28	
#define KEY_VALUE_ENTER		30	



//
uint8_t KeyRead(void)
{
	uint8_t tKeyValue = 0;
	tKeyValue = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
	tKeyValue <<= 1;
	tKeyValue |= GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6);
	tKeyValue <<= 1;
	tKeyValue |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
	tKeyValue <<= 1;
	tKeyValue |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
	tKeyValue <<= 1;
	tKeyValue |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);

	return tKeyValue;
}

//
void KeyHardwareConfig(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

//	sysTickStartSoftTimer(KEY_TICK, 20, true);
}









