#ifndef __GPIO_H
#define __GPIO_H

#include "sys.h"

typedef struct
{
	/*输入引脚*/
	uint16_t Input_Pin1;
	uint16_t Input_Pin2;
	/*输出引脚*/
	uint16_t Output_Pin1;
	uint16_t Output_Pin2;
}Port__Struct;
/*
**数字开关量状态管理结构体
*/
typedef struct
{
	/*输入引脚*/
	GPIO_TypeDef *Input1_Port;
	uint16_t Input1_Pin;
	
	GPIO_TypeDef *Input2_Port;
	uint16_t Input2_Pin;
	
	/*输出引脚*/
	GPIO_TypeDef *Output1_Port;
	uint16_t Output1_Pin;
	
	GPIO_TypeDef *Output2_Port;
	uint16_t Output2_Pin;
	
}DigitalPort_Struct;



//定义设备开关量引脚参数
extern DigitalPort_Struct RTU_DigitalPort;



//LED端口定义
#define LED0 PFout(9)	  // DS0
#define LED1 PFout(10)	// DS1	 

void DigitalGpio_Init(void);//初始化		 			


uint16_t GetDigital_InputState( void);
void SetDigital_OutputState( void);



#endif
