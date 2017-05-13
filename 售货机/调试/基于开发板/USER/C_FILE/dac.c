
#include "dac.h"
#include "modbus.h" 


/*
**DACͨ��1�����ʼ��
**���룺��
**���أ���
*/
void Dac1_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DACͨ��1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //��ʼ״̬���0
}
/*
**��ȡģ�������
**���룺��
**���أ���ǰģ�������ֵ
*/
uint16_t GetAnalog_InputValue( void)
{
	
}
/*
**����ģ�������
**���룺��
**���أ���
*/
void SetAnalog_OutputValue( void)
{
	uint16_t temp;
	
	temp=DAC_GetDataOutputValue( DAC_Channel_1);
	
	if( temp!=MB_Read_Ch1_Struct.Data_Buf[0])//�����Ҫ�仯
	{
		if( MB_Read_Ch1_Struct.Data_Buf[0]<=4095)//���趨��Χ��
		{
			DAC_SetChannel1Data(DAC_Align_12b_R,MB_Read_Ch1_Struct.Data_Buf[0]);	
		}	
	}
}
/*
**����ͨ��1�����ѹ
**���룺vol:0~3300,����0~3.3V
**���أ���
*/
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}














































