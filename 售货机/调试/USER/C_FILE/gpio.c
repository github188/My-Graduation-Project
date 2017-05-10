
#include "gpio.h" 
#include "modbus.h" 


//定义设备管脚
DigitalPort_Struct RTU_DigitalPort={  GPIOE,GPIO_Pin_2,			GPIOE,GPIO_Pin_3,
																	    GPIOF,GPIO_Pin_9,			GPIOF,GPIO_Pin_10,
																};

//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void DigitalGpio_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOE、F时钟
	
	/****************GPIOF9,F10作为数字量输入*********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3
	/************************************************************/
	
	/****************GPIOF9,F10作为数字量输出*********************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	GPIO_ResetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//初始状态置为0
  /************************************************************/
	
}

/*
**获取输入开关量状态
*/
uint16_t GetDigital_InputState( void)
{
	uint16_t val=0;
	//in1
	if( RESET!=GPIO_ReadInputDataBit( RTU_DigitalPort.Input1_Port,RTU_DigitalPort.Input1_Pin))	
		val|=(1<<0);
	else
		val&=~(1<<0);
	//in2
		if( RESET!=GPIO_ReadInputDataBit( RTU_DigitalPort.Input2_Port,RTU_DigitalPort.Input2_Pin))	
		val|=(1<<1);
	else
		val&=~(1<<1);
return val;
}
/*
**设置输出开关量状态
*/
void SetDigital_OutputState( void)
{
	uint16_t temp;
	//out1
	temp=MB_Read_Ch1_Struct.Data_Buf[4]&(1<<0);
	if( GPIO_ReadInputDataBit( RTU_DigitalPort.Output1_Port,RTU_DigitalPort.Output1_Pin)!=temp)
		GPIO_WriteBit( RTU_DigitalPort.Output1_Port,RTU_DigitalPort.Output1_Pin,temp);
	
	//out2
	temp=MB_Read_Ch1_Struct.Data_Buf[4]&(1<<1);
	if( GPIO_ReadInputDataBit( RTU_DigitalPort.Output2_Port,RTU_DigitalPort.Output2_Pin)!=temp)
	GPIO_WriteBit( RTU_DigitalPort.Output2_Port,RTU_DigitalPort.Output2_Pin,temp);
	
}
















