#ifndef __GPIO_H
#define __GPIO_H

#include "sys.h"

typedef struct
{
	/*��������*/
	uint16_t Input_Pin1;
	uint16_t Input_Pin2;
	/*�������*/
	uint16_t Output_Pin1;
	uint16_t Output_Pin2;
}Port__Struct;
/*
**���ֿ�����״̬����ṹ��
*/
typedef struct
{
	/*��������*/
	GPIO_TypeDef *Input1_Port;
	uint16_t Input1_Pin;
	
	GPIO_TypeDef *Input2_Port;
	uint16_t Input2_Pin;
	
	/*�������*/
	GPIO_TypeDef *Output1_Port;
	uint16_t Output1_Pin;
	
	GPIO_TypeDef *Output2_Port;
	uint16_t Output2_Pin;
	
}DigitalPort_Struct;



//�����豸���������Ų���
extern DigitalPort_Struct RTU_DigitalPort;



//LED�˿ڶ���
#define LED0 PFout(9)	  // DS0
#define LED1 PFout(10)	// DS1	 

void DigitalGpio_Init(void);//��ʼ��		 			


uint16_t GetDigital_InputState( void);
void SetDigital_OutputState( void);



#endif
