#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	     			    





void Dac1_Init(void);		//DACͨ��1��ʼ��	 	 
void Dac1_Set_Vol(u16 vol);	//����ͨ��1�����ѹ


uint16_t GetAnalog_InputValue( void);
void SetAnalog_OutputValue( void);




#endif

















