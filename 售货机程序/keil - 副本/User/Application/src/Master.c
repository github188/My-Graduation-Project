#include <stdbool.h>
#include <stdlib.h>
//
#include "stm32f4xx.h"

#include "Master.h"
#include "stm32f4xx_usart.h"
#include "CAN.h"
#include "Global.h"

#define MB_NUM 41
#define GOLD  0xFEA0
#define RED	  0xF800
#define GREEN 0x07E0
#define BLUE  0x001F  
#define BRED  0xF81F
#define GRED  0xFFE0
#define GBLUE 0x07FF
#define WHITE 0xFFFF //??
#define BLACK 0x0000 //??
#define BROWN 0xBC40 //??
#define BRRED 0xFC07 //???
#define GRAY  0x8430 //??
#define LGRAY 0xC618 //???

extern unsigned char wrFtaDoDat;
extern unsigned char u4RxdBuf[100],u4TxdBuf[10];
extern unsigned int  u4RxdCnt;
unsigned char u4TxCnt,u4RxCnt;
unsigned char u4TxDat[10],u4RxDat[10];
float sSSL,sLJS,sMD,sWD;

void SendU6Dat(unsigned char u6Dat);

extern void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
extern void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void delay(unsigned long nCount);
FV_MSG FV_Msg[14];
PKQ_MSG PKQ_Msg[14];
extern void Dis_Hex(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint16_t Dat);

//0:接收  1:发送
void U2_EN(unsigned char tDat)
{
  if(tDat==1)
      GPIO_ResetBits(GPIOA,GPIO_Pin_0);//U2准备接受PC指令
  else
      GPIO_SetBits(GPIOA,GPIO_Pin_0);//U2准备接受PC指令
}
//0:接收  1:发送
void U3_EN(unsigned char tDat)
{
  if(tDat==1)
      GPIO_ResetBits(GPIOA,GPIO_Pin_1);//U3准备接受PC指令
  else
      GPIO_SetBits(GPIOA,GPIO_Pin_1);//U3准备接受PC指令
}
//0:接收  1:发送
void U4_EN(unsigned char tDat)
{
  if(tDat==1)
      GPIO_ResetBits(GPIOD,GPIO_Pin_12);//U4准备接受PC指令
  else
      GPIO_SetBits(GPIOD,GPIO_Pin_12);//U4准备接受PC指令
}
void SendU1Dat(unsigned int u1Dat)
{
	USART_SendData(USART1,u1Dat);
}

void SendU2Dat(unsigned int u2Dat)
{
	USART_SendData(USART2,u2Dat);
}
void SendU3Dat(unsigned int u3Dat)
{
	USART_SendData(USART3,u3Dat);
}
void SendU4Dat(unsigned int u4Dat)
{
	USART_SendData(UART4,u4Dat);
}
void SendU6Dat(unsigned char u6Dat)
{
	USART_SendData(USART6,u6Dat);
}

/*

*/
/*
17,72,73,74,75,76,80,81,82,83,84,85,86,159,160,161,162,163,164,165,166,199,200,201,202,245,246,247,248,249,250,251,252,259,260,261,262,279,280,293,294,
*/
//unsigned int  mbpos[43]={ 17, 72, 73, 74, 75, 76, 80, 81, 82, 83,
//												  84, 85, 86,159,160,161,162,163,164,165,
//												 166,199,200,201,202,245,246,247,248,249,
//                         250,251,252,259,260,261,262,279,280,293,
//                         294,54, 55};
void U1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;      

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* Configure USART Tx  as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART Rx  as input floating */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//USART中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			
	/* Configure the USART */
	USART_Init(USART1, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}
void U2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;      

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* Configure USART Tx  as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART Rx  as input floating */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//USART中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			
	/* Configure the USART */
	USART_Init(USART2, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//准备接受IFIX
}
void U3_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;      

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	/* Configure USART Tx  as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART Rx  as input floating */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//USART中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			
	/* Configure the USART */
	USART_Init(USART3, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(USART3, ENABLE);

	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//准备发送给FTA
}
void U4_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;      

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

	/* Configure USART Tx  as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART Rx  as input floating */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	//USART中断配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			
	/* Configure the USART */
	USART_Init(UART4, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(UART4, ENABLE);

	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);//U4准备接受PC指令

}
void U5_Init_Cust(uint16_t bps,uint8_t bitLen,uint8_t bitStop,uint8_t bitParity)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure USART Rx  as input floating */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure USART Tx  as alternate function push-pull */


	//USART中断配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = bps ;
//	if(bitParity>1)//有校验
//	{
//		
//		
//	}
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			
	USART_InitStructure.USART_Mode = USART_Mode_Rx;			
	/* Configure the USART */
	USART_Init(UART5, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(UART5, ENABLE);
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);
}
//0:接收  1:发送
void U5_EN(unsigned char tDat)
{
  if(tDat==1)
        GPIO_SetBits(GPIOE,GPIO_Pin_1);
  else
      GPIO_ResetBits(GPIOE,GPIO_Pin_1);
}
void U5_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure USART Rx  as input floating */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/* Configure USART Tx  as alternate function push-pull */


	//USART中断配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = 9600 ;//1200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			
	//USART_InitStructure.USART_Mode = USART_Mode_Rx;			
	/* Configure the USART */
	USART_Init(UART5, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(UART5, ENABLE);
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	GPIO_SetBits(GPIOE,GPIO_Pin_1);

}
//0:接收  1:发送
void U6_EN(unsigned char tDat)
{
  if(tDat==1)
  	GPIO_ResetBits(GPIOD,GPIO_Pin_13);
  else
  	GPIO_ResetBits(GPIOD,GPIO_Pin_13);
}
void U6_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure USART Rx  as input floating */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  //PD13 EN6
	/* Configure USART Tx  as alternate function push-pull */


	//USART中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = 38400; //9600 ;//1200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			
	//USART_InitStructure.USART_Mode = USART_Mode_Rx;			
	/* Configure the USART */
	USART_Init(USART6, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(USART6, ENABLE);
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
	GPIO_ResetBits(GPIOD,GPIO_Pin_13);

}

unsigned char function;
unsigned char chk_U2Sqr(void)
{
	unsigned int address;
	unsigned int data;
	unsigned int i,j, length;
	unsigned int tCrc;
	unsigned char crch,crcl;
	unsigned char tLen;
	unsigned int  txdLen;
	unsigned char sl_id;
	unsigned char thBuf[100];
	for(i=0;i<20;i++)
		thBuf[i]=datMod2Buf[i];
	function = thBuf[1];
	sl_id=thBuf[0];
//	if(pkqDisUnit[sl_id-1].cntConn==1)
//		return 20;
//	if(sl_id!=sysSavDat[4])//地址不同
//	{	}
	if((function!=3)&&(function!=4)&&(function!=6))
		return 20;
	if((function==3)||(function==4))
		tLen=6;
	if(function==6)
		tLen=6;
	tCrc=CRC16(thBuf,tLen);
	crch= (tCrc & 0xff);
	crcl=	(tCrc & 0xFF00)>>8;
  if((thBuf[tLen]!=crch)||(thBuf[tLen+1]!=crcl))
	{
	 return 2;
	}
  if (function == 0x03) 
	{		  //03 Read Holding Registers
		address = (thBuf[2] << 8)	+ thBuf[3];
		//if((address<39)||(address>98))//??141
		//	return 3;
		//address -=39;
		length = (thBuf[4] << 8) + thBuf[5];
		for(i=0;i<100;i++)
			Txd2Buf[i]=0;
		Txd2Buf[0]=sl_id;
		Txd2Buf[1]=0x03;
		txdLen=length*2;
		Txd2Buf[2] = (unsigned char) txdLen;
			for (i=0; i<length; i++) 
			{
				for(j=0;j<43;j++)
				{
					if((address+i+1)<9)
					{
					Txd2Buf[3+i*2] = (unsigned char) (mbDat[i] >> 8);
					Txd2Buf[4+i*2] = (unsigned char) (mbDat[i] & 0xff);
					}
				}
			}
		txdLen=txdLen + 3;
		tCrc=CRC16(Txd2Buf,txdLen);
		crch= (tCrc & 0xff);
		crcl=	(tCrc & 0xFF00)>>8;
		Txd2Buf[txdLen]=crch;
		txdLen++;
		Txd2Buf[txdLen]=crcl;
		txdLen++;
		Txd2Buf[txdLen]=0;
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
		delay(10);
		for(i=0; i<=txdLen; i++) 
		{
			u2MbBuf[i]=Txd2Buf[i];
		}
		u2Cnt=0;u2SndCnt=txdLen;u2Flag=1;

		return 0;
	}
	else if (function == 0x06)
	{	//06 Preset Single Register
		address = (thBuf[2] << 8)	+ thBuf[3];
		if(address!=3)
		{ return 4;	}
		data = (thBuf[4] << 8) + thBuf[5];
		mbDat[3]=data;
		wrFtaDoDat=data;
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
		delay(10);
		thBuf[8]=0;
		//?????,??CRC??
		for(i=0; i<8; i++) 
		{
			u2MbBuf[i]=thBuf[i];	
		}
		u2Cnt=0;u2SndCnt=8;u2Flag=1;
		return 0;
	} 
}
unsigned char Func;
/*unsigned char chk_U3Sqr(void)
{
	unsigned int address;
	unsigned int data;
	unsigned int i,j, length;
	unsigned int tCrc;
	unsigned char crch,crcl;
	unsigned char tLen;
	unsigned int  txdLen;
	unsigned char sl_id;
	unsigned char thBuf[100];
	for(i=0;i<20;i++)
		thBuf[i]=datMod3Buf[i];
	Func = thBuf[1];
	sl_id=thBuf[0];
//	if(pkqDisUnit[sl_id-1].cntConn==1)
//		return 20;
	if((Func!=3)&&(Func!=6)&&(Func!=7)&&(Func!=16))
		return 20;
	if(Func==3)
		tLen=6;
	if(Func==6)
		tLen=6;
	if(Func==7)	//???,??ESD,???
		tLen=6;
	if(Func==16)
		tLen=11;
	tCrc=CRC16(thBuf,tLen);
	crch= (tCrc & 0xff);
	crcl=	(tCrc & 0xFF00)>>8;
  if((thBuf[tLen]!=crch)||(thBuf[tLen+1]!=crcl))
	{
	 return 2;
	}
  if (Func == 0x03) 
	{		  //03 Read Holding Registers
		address = (thBuf[2] << 8)	+ thBuf[3];
		//if((address<39)||(address>98))//??141
		//	return 3;
		//address -=39;
		length = (thBuf[4] << 8) + thBuf[5];
		for(i=0;i<100;i++)
			Txd3Buf[i]=0;
		Txd3Buf[0]=sl_id;
		Txd3Buf[1]=0x03;
		txdLen=length*2;
		Txd3Buf[2] = (unsigned char) txdLen;
			for (i=0; i<length; i++) 
			{
				for(j=0;j<43;j++)
				{
					if((address+i+1)==mbpos[j])
					{
					Txd3Buf[3+i*2] = (unsigned char) (mbdata[sl_id-1][j] >> 8);
					Txd3Buf[4+i*2] = (unsigned char) (mbdata[sl_id-1][j] & 0xff);
					}
				}
			}
		txdLen=txdLen + 3;
		tCrc=CRC16(Txd3Buf,txdLen);
		crch= (tCrc & 0xff);
		crcl=	(tCrc & 0xFF00)>>8;
		Txd3Buf[txdLen]=crch;
		txdLen++;
		Txd3Buf[txdLen]=crcl;
		txdLen++;
		Txd3Buf[txdLen]=0;
		GPIO_SetBits(GPIOC,GPIO_Pin_4);
		delay(10);

		//?????,??CRC??
		for(i=0; i<=txdLen; i++) 
		{
			u3MbBuf[i]=Txd3Buf[i];
		}
		u3Cnt=0;u3SndCnt=txdLen;u3Flag=1;
		//SetU2Recv();
	return 0;
	}
	
}
*/


float CalIEEE754(u8 dt1,u8 dt2,u8 dt3,u8 dt4,u8 tFlg)
{
	float tta;
	float lmtMin=0.0,lmtMax=400.0;
	u8 i,*px;
	u8 x[4];
	void *pf;
	x[0]=dt4;
	x[1]=dt3;
	x[2]=dt2;
	x[3]=dt1;
	
	px=x;             //px指针指向数组x
	pf=&tta;
	for(i=0;i<4;i++)
	 *((char *)pf+i)=*(px+i);
	switch(tFlg)
	{
		case 1://瞬时量
			lmtMin=0.01;
			lmtMax=400.0;
		break;
		case 2://累积数
			lmtMin=0.01;
			lmtMax=999999999.9;
		break;
		case 3://密度
			lmtMin=0.00001;
			lmtMax=4.0;
		break;
		case 4://介质温度
			lmtMin=-40.0;
			lmtMax=200.0;
		break;
		case 5://累积体积
			lmtMin=0.01;
			lmtMax=999999999.9;
		break;
	}
	if(tta<lmtMin)
	{
		if(tFlg!=4)
			tta=0;
		else
			tta=lmtMin;
	}
	if(tta>lmtMax)
		tta=lmtMax;
	return tta;
}



extern PKQ_DIS  pkqDisUnit[14];//14组批控器
extern unsigned char frmNum;
extern void Draw_Pkq_STA(unsigned char id,unsigned char sta);
extern void Draw_Cell_Conn(unsigned char id,unsigned char sta);
extern void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);

//称重软件
void UART4_IRQHandler(void)
{
	char disBuf[3];
	unsigned char u4,i;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(UART4,USART_IT_RXNE);
		u4Dat=USART_ReceiveData(UART4);
		
		if(flgU4StRecv==1)
		{
			u4RxDat[u4RxCnt]=u4Dat;
			u4RxCnt++;
			if(u4RxCnt>=8)
			{
				flgU4StRecv=0;
				if((u4RxDat[2]==0x01)&&(u4RxDat[3]==0x01)&&((u4RxDat[5]==0x40)||(u4RxDat[5]==0x41)))
				{
					if(u4RxDat[5]==0x40)
							flgU4StSend=1;
					if(u4RxDat[5]==0x41)
					{
							flgU4StSend=2;
							wrFtaDoDat=u4RxDat[6];
					}
				}
			}
		}
		if(flgU4StRecv==0)
		{
			if((flgU4Stx2==0)&&(flgU4Stx1==1)&&(u4Dat==0xCC))
			{
				flgU4Stx2=1;
				flgU4StRecv=1;
				u4RxCnt=2;	//u4RxdLen=13;
			}
			else
			{
				flgU4Stx1=0;flgU4Stx2=0;flgU4StRecv=0;
			}
			if(u4Dat==0xFF)//STX
			{
				flgU4Stx1=1;flgU4Stx2=0;
				u4STX=1;u4RxCnt=0;
				for(u4=0;u4<10;u4++)
				{
					u4RxDat[u4]=0;
				}	
			}
		}		
	}
   //溢出处理
 	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(UART4,USART_FLAG_ORE);
		USART_ReceiveData(UART4);
		
	}
}

void CalWei(void)
{
	unsigned char i,flgDatOk=1;
	for(i=0;i<6;i++)
	{
		if(((WeiDat[i]<'0')||(WeiDat[i]>'9'))&&(WeiDat[i]!=0x20))
		{
			WeiDat[i]=0;
			flgDatOk=0;
			break;
		}
		else
		{
			if(WeiDat[i]==0x20)
			{
				WeiDat[i]=0;
			}
			else
				WeiDat[i]-=0x30;
		}
	}
	if(flgDatOk==1)
	{
		lngWeiDat =WeiDat[0]*100000;
		lngWeiDat+=WeiDat[1]*10000;
		lngWeiDat+=WeiDat[2]*1000;
		lngWeiDat+=WeiDat[3]*100;
		lngWeiDat+=WeiDat[4]*10;
		lngWeiDat+=WeiDat[5]*1;
		mbDat[0]=lngWeiDat&0xFFFF;
		mbDat[1]=lngWeiDat>>16;
	}
}
//苏A12345
void Dis_Pad_(unsigned char tCar)
{
	unsigned char ti,tpos;
	unsigned int tx;
	char ttBuf[25];
	char PadCode[7];
	//           303132333435363738393A3B3C3D3E3F
	char HZ[62]="冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽粤川青藏琼宁渝京津沪贵";
	//char ZM[52]="ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ";
	//char SZ[20]="０１２３４５６７８９";
	if((CarId[0]==0)&&(CarId[1]==0)&&(CarId[2]==0)&&(CarId[3]==0)&&(CarId[4]==0)&&(CarId[5]==0)&&(CarId[6]==0))
		tCar=0;
	if(tCar)
	{
	for(ti=0;ti<7;ti++)
		PadCode[ti]=CarId[ti];
//冀豫云辽黑湘皖鲁新苏
//浙赣鄂桂甘晋蒙陕吉闽
//粤川青藏琼宁渝京津沪
	tx=180;
	for(ti=0;ti<7;ti++)
	{
		if(PadCode[ti]<31)	
		{
			ttBuf[0]=HZ[PadCode[0]*2];
			ttBuf[1]=HZ[PadCode[0]*2+1];
			ttBuf[2]=0;
			lcd_text24(tx,355,BLACK,WHITE,ttBuf);
			tx=tx+24;
		}
		else
		{
			ttBuf[0]=PadCode[ti];
			ttBuf[1]=0;
			lcd_text24(tx,355,BLACK,WHITE,ttBuf);
			tx=tx+12;
		}
	}
	}
 else
 {
	//lcd_text24(30,380-12,0x001F,0x001F,"车号");
	if(frmNum==FRM_MAIN)
	{
		lcd_text24(173,355,BLACK,WHITE,"车号");
		Draw_Rect(45,358,192,44,3);
		TFT_Fill(45,358,272-35,404,0x001F);
	}
 }
}

extern unsigned char sysSavDat[10];
//UART5 秤重传感器
void UART5_IRQHandler(void)
{
	u16 u;
	u8 i;
	u8 j;
	u8 flgEnDat;
	u8 tDat;//flgDat1Ok=0,flgDat2Ok=0,
	u8 tBuf[37];
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		u=USART_ReceiveData(UART5);
   		USART_ClearFlag(UART5,USART_IT_RXNE);
		if(flgU5StRecv==1)
		{
			u5RxdBuf[u5RxdCnt]=u;
			u5RxdCnt++;
			//if(u==0x27)
			//  u5RxdLen=37;//不指定鹤位
			switch(u5RxdBuf[3])
			{
				case 0x62:
					u5RxdLen=9;break;
			  case 0x69:
					u5RxdLen=20;//应该是21,//车号共16字节，前2字节为车号长度后面是车辆代号，汉字2字节
				break;
			  case 0x66:
					u5RxdLen=15;////前6个0后4位为工号,10byte,15=10+5
				break;
			  case 0x67:
					u5RxdLen=13;////提油密码,8byte13=8+5
				break;
			  default:
					u5RxdLen=13;
				break;
			}
			if(u5RxdCnt>=u5RxdLen)   //13
			{
				//FF CC 01 52 38 31 30 36 31 38 99
				//FF CC 01 62 31 32 33 34 AC
				if(u5RxdBuf[3]==0x62)//IC卡号
				{
				  if((frmNum==FRM_LOGIN)||(frmNum==FRM_MON))
					{
						flgEnDat=1;
						for(i=4;i<8;i++)
						{
						  if((u5RxdBuf[i]&0xF0)!=0x30)
						    flgEnDat=0;
						}
					  if(flgEnDat==1)
						{
						   for(i=4;i<8;i++)
						   {
						   	CardUsr[i-4]=u5RxdBuf[i];
						   }
						   CardUsr[4]=0;CardUsr[5]=0xFA;
						}
						//ff cc 01 67 30 30 30 30 30 30 3x 3x 3x 3x AC
					}
				}
				if(u5RxdBuf[3]==0x69)//卡号命令：
				{
				  if((flgIcDat1OK==1)&&(u5RxdBuf[4]==0x31)&&(u5RxdBuf[5]==0x34))
					{
					  tDat=(u5RxdBuf[13]-'0')*10;
						tDat=tDat+u5RxdBuf[14]-'0';
						if(1)//(tDat==dnSavConfDat[4])//关阀参数[4]替换成"油品编号"
						{
							//if((u5RxdBuf[4]==0x31)&&(u5RxdBuf[5]==0x34))
							//{
							  CarId[0]=u5RxdBuf[6]-'0'; 
								for(i=1;i<7;i++)
								  CarId[i]=u5RxdBuf[6+i];
								for(i=0;i<5;i++)
								  frmMainYFval[i]=u5RxdBuf[15+i];
								dYFL[dualChNum-1]=calYFL();
								//			lcd_text24(181,53,BLACK,WHITE,"应发量");
							  //ShowStr30(30+10,82,frmMainOrdId,8,BLACK,WHITE);
								ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE); //小数点以后舍去
								Dis_Pad(1);
							//}
						}
						flgIcDat1OK=0;
					}
				}
				if(u5RxdBuf[3]==0x67)//提油密码
				{
					
					 if(((u5RxdBuf[4]&0xF0)==0x30)&&((u5RxdBuf[5]&0xF0)==0x30)&&((u5RxdBuf[6]&0xF0)==0x30)&&
					 ((u5RxdBuf[7]&0xF0)==0x30)&&((u5RxdBuf[8]&0xF0)==0x30)&&((u5RxdBuf[9]&0xF0)==0x30)&&
					 ((u5RxdBuf[10]&0xF0)==0x30)&&((u5RxdBuf[11]&0xF0)==0x30))
						{
							if(WarnOnOff[9]==1)
							{
								flgAll0=1;
								for(j=4;j<12;j++)
								{
									if(u5RxdBuf[j]!=0x30)
									{	flgAll0=0;break;}
								}
								if(flgAll0==0)
								{
									for(i=4;i<12;i++)
										icBuf[i-4]=u5RxdBuf[i];
									icBuf[8]=0;icBuf[9]=0xFA;
								}
							}
							if((WarnOnOff[9]==0)&&(frmNum==FRM_MAIN))//不指定鹤位
							{
								flgIcDat1OK=1;
								for(i=0;i<8;i++)
								{
								  frmMainOrdId[i]=u5RxdBuf[4+i];
									frmMainYFval[i]=' ';
								}
								dYFL[dualChNum-1]=calYFL();
							  //ShowStr30(30+10,82,frmMainOrdId,8,BLACK,WHITE);
								ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE); //小数点以后舍去
								Dis_Pad(0);
							}
						}
				}
				if(u5RxdBuf[3]==0x66)//工号卡
				{
				  if(frmNum==FRM_LOGIN)
					{
						flgEnDat=1;
						for(i=4;i<10;i++)
						{
						  if(u5RxdBuf[i]!=0x30)
						    flgEnDat=0;
						}
					  if(flgEnDat==1)
						{
						if(((u5RxdBuf[10]&0xF0)==0x30)&&((u5RxdBuf[11]&0xF0)==0x30)&&
					     ((u5RxdBuf[12]&0xF0)==0x30)&&((u5RxdBuf[13]&0xF0)==0x30))
						 {
						   for(i=10;i<14;i++)
						   {
						   	CardUsr[i-10]=u5RxdBuf[i];
						   }
						   CardUsr[4]=0;CardUsr[5]=0xFA;
						 }
						}
						//ff cc 01 67 30 30 30 30 30 30 3x 3x 3x 3x AC
					}
				}
			flgU5Stx1=0;
			flgU5Stx2=0;
			flgU5StRecv=0;
			u5RxdCnt=0;
			}
		}
		if(flgU5StRecv==0)
		{
			if((flgU5Stx2==0)&&(flgU5Stx1==1)&&(u==0xCC))
			{
				flgU5Stx2=1;
				flgU5StRecv=1;
				u5RxdCnt=2;	u5RxdLen=13;u5RxdBuf[0]=0xFF;u5RxdBuf[1]=0xCC;
			}
			else
			{
				flgU5Stx1=0;flgU5Stx2=0;flgU5StRecv=0;
			}
			if(u==0xFF)
			{
				flgU5Stx1=1;flgU5Stx2=0;flgU5StRecv=0;
			}
		}
	}
   //溢出处理
 	if(USART_GetFlagStatus(UART5,USART_IT_ORE) != RESET)
	{
		USART_ClearFlag(UART5,USART_IT_ORE);
		u = USART_ReceiveData(UART5);
	}
}

extern void re_WDm(float tfSSL);
extern void re_LJSm(char id,double tfSSL);
extern float ctof(unsigned char dt1,unsigned char dt2,unsigned char dt3,unsigned char dt4);

void USART2_IRQHandler(void)
{
	vu32 tdelay1;
	vu8 u2,u2i;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART2,USART_IT_RXNE);
		u2=USART_ReceiveData(USART2);
	}
	 if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
	  USART_ClearFlag(USART2,USART_FLAG_ORE); 
		USART_ReceiveData(USART2);
	}
////	vu32 tdelay1;
////	vu8 u2,u2i;
////	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
////	{
////		USART_ClearFlag(USART2,USART_IT_RXNE);
////		u2=USART_ReceiveData(USART2);
////		u2Tmr=0; 
////		u2RxdBuf[u2RxdCnt]=u2;
////		u2RxdCnt++;
////	}
////	 if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
////	{
////	  USART_ClearFlag(USART2,USART_FLAG_ORE); 
////		USART_ReceiveData(USART2);
////	}
//  	vu32 tdelay1;
//	vu8 u2,u2i;
//	u16 crcresult;
//	u8  crcHi=0;
////	u8  crcLo=0;
////	u8  i;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//	USART_ClearFlag(USART2,USART_IT_RXNE);
//	u2=USART_ReceiveData(USART2);
//	 if(u2Flg==1)
//	 {
//		 u2RxdCnt++;
//		 u2RxdBuf[u2RxdCnt]=u2;
////01 03 20 00 00 00 00 97 d6 3f 5a ee b0 41 b5 00 00 00 00 00 00 00 00 00 00 00 00 d2 b5 4a 50 77 01 45  7e
//		 if(u2RxdCnt>=36)//mdDat[1]
//		 {
//				crcresult =CRC16(u2RxdBuf,35);
//				crcHi=(crcresult & 0xff);
//				//crcLo=(crcresult & 0xFF00)>>8;
//		 		u2RxdCnt=0;
//				u2Flg=0;
//				u2NewTxdFlg=0;
//				//if ((crcHi==u3RxdBuf[35])&&(crcLo==u3RxdBuf[36]))
//				if ((crcHi==u2RxdBuf[35]))//	&&(crcLo==u3RxdBuf[36])
//				//if((u3RxdBuf[1]==0x03)&&(u3RxdBuf[2]==0x20))
//				{
//					flgFvETX=1;
//					//flgMbWr=1;
//					//流量计通讯故障复位
////					cntFVconnTmr=0;
////					err_FV=0;
////					flgFvRxd=0;
//          sSSL=CalIEEE754(u2RxdBuf[6],u2RxdBuf[5],u2RxdBuf[4],u2RxdBuf[3],1);
//          sWD=CalIEEE754(u2RxdBuf[14],u2RxdBuf[13],u2RxdBuf[12],u2RxdBuf[11],4);re_WD(sWD);
//          sMD=CalIEEE754(u2RxdBuf[10],u2RxdBuf[9],u2RxdBuf[8],u2RxdBuf[7],3);
//          sLJS=ctof(u2RxdBuf[30],u2RxdBuf[29],u2RxdBuf[28],u2RxdBuf[27]);//re_LJS(sLJS);
//					mbdata[0][14]=(u2RxdBuf[3]<<8)+u2RxdBuf[4];//瞬时流量
//					mbdata[0][15]=(u2RxdBuf[5]<<8)+u2RxdBuf[6];
//					mbdata[0][18]=(u2RxdBuf[11]<<8)+u2RxdBuf[12];//介质温度
//					mbdata[0][19]=(u2RxdBuf[13]<<8)+u2RxdBuf[14];
//					mbdata[0][20]=(u2RxdBuf[7]<<8)+u2RxdBuf[8];//介质密度
//					mbdata[0][21]=(u2RxdBuf[9]<<8)+u2RxdBuf[10];
//					mbdata[0][22]=(u2RxdBuf[27]<<8)+u2RxdBuf[28];//累计流量质量 
//					mbdata[0][23]=(u2RxdBuf[29]<<8)+u2RxdBuf[30];
//					mbdata[0][24]=(u2RxdBuf[31]<<8)+u2RxdBuf[32];//累计流量体积
//					mbdata[0][25]=(u2RxdBuf[33]<<8)+u2RxdBuf[34];
//				 	//RxdHandler();
//					//RxdEvents();
//				}
//		 }
//	 }
//	 if (u2NewTxdFlg==0)
//	 {
//			if(u2==0x01)
//			{
//			 	u2Flg=1;
//				for(u2i=0;u2i<100;u2i++)
//				{
//					 u2RxdBuf[u2i]=0;
//				} 
//				u2RxdCnt=0;
//				u2NewTxdFlg=1;
//				u2RxdBuf[0]=0x01;
//				//flgFvCrcErr=0;
//				flgFvETX=0;
//			}
//	 } 
//   //USART_ClearFlag(USART3,USART_IT_RXNE);
//	}
//	 if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
//	{
//	  USART_ClearFlag(USART2,USART_FLAG_ORE); 
//		USART_ReceiveData(USART2);
//	}

}

//USART3 FTA
void USART3_IRQHandler(void)
{
	vu32 tdelay1;
	vu8 u3,u3i,crcHi=0,crcLo=0;
	vu16 tdat;
	u16 crcresult;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART3,USART_IT_RXNE);
		u3=USART_ReceiveData(USART3);
		if(u3Flg==1)
		{
			u3RxdCnt++;
			u3RxdBuf[u3RxdCnt]=u3;
			if(u3RxdCnt>=16)
			{
				crcresult =CRC16(u3RxdBuf,15);
				crcHi=(crcresult & 0xff);
				//crcLo=(crcresult & 0xFF00)>>8;
		 		u3RxdCnt=0;
				u3Flg=0;
				u3NewTxdFlg=0;
				//if ((crcHi==u3RxdBuf[35])&&(crcLo==u3RxdBuf[36]))
//02 03 0C 00 00 06 C0 01 0E 00 00 00 00 00 00 4E 67
				if (crcHi==u3RxdBuf[15])//	&&(crcLo==u3RxdBuf[36])
				{
					//028614-Tx:02 03 08 00 01 00 00 00 00 00 00 8A 53
					//400160
					//b0 全关位置，1有效
					fbValv=u3RxdBuf[0]-2;
					if((u3RxdBuf[6]&0x01)==0x01)
					{	valvClosed[fbValv]=1;	}
					else
					{	valvClosed[fbValv]=0;	}
					if((u3RxdBuf[6]&0x02)==0x02)
					{	valvOpened[fbValv]=1;	}
					else
					{	valvOpened[fbValv]=0;	}
					tdelay1=(u3RxdBuf[9]&0x03);//说明书是400162，测试时是400163
					tdelay1=tdelay1<<8;
					tdelay1=tdelay1+u3RxdBuf[10];
					valvOpen[fbValv]=tdelay1/10;
					if(fbValv==0)	
					{
						CanData[78]=valvOpen[0];canRtData[35]=u3RxdBuf[5];canRtData[36]=u3RxdBuf[6];
					 staValv[0]=(u3RxdBuf[5]<<8)+u3RxdBuf[6];
					}//
					if(fbValv==1)	
					{
						CanData[80]=valvOpen[1];canRtData[37]=u3RxdBuf[5];canRtData[38]=u3RxdBuf[6];
					 staValv[1]=(u3RxdBuf[5]<<8)+u3RxdBuf[6];
					}
					canRtData[20]=valvOpen[0];
					canRtData[27]=valvOpen[1];
					
					u3RxdCnt=0;
					u3Flg=0;
					//if(frmNum==FRM_MAIN)
					//{
					//	if(u3RxdBuf[0]==2)
					//	{
					//		Dis_Hex(410,380,RED,WHITE,u3RxdBuf[3]); Dis_Hex(410+16,380,RED,WHITE,u3RxdBuf[4]); Dis_Hex(420+32,380,RED,WHITE,u3RxdBuf[5]);Dis_Hex(420+48,380,RED,WHITE,u3RxdBuf[6]);
					//		Dis_Hex(430+64,380,RED,WHITE,u3RxdBuf[7]&0x03); Dis_Hex(430+80,380,RED,WHITE,u3RxdBuf[8]);
					//	}
					//	if(u3RxdBuf[0]==3)
					//	{
					//		Dis_Hex(410,400,RED,WHITE,u3RxdBuf[3]); Dis_Hex(410+16,400,RED,WHITE,u3RxdBuf[4]); Dis_Hex(420+32,400,RED,WHITE,u3RxdBuf[5]);Dis_Hex(420+48,400,RED,WHITE,u3RxdBuf[6]);
					//		Dis_Hex(430+64,400,RED,WHITE,u3RxdBuf[7]&0x03); Dis_Hex(430+80,400,RED,WHITE,u3RxdBuf[8]);
					//	}
					//}
				}
			}
		}
	  if(u3NewTxdFlg==0)
		{
			if((u3==2)||(u3==3))//1#valv
			{
				for(u3i=0;u3i<30;u3i++)
				{u3RxdBuf[u3i]=0;}
				u3RxdBuf[0]=u3;
				u3RxdCnt=0;
			  u3Flg=1;u3NewTxdFlg=1;
			}	
		}
	}
	 if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
	{
	  USART_ClearFlag(USART3,USART_FLAG_ORE); 
		USART_ReceiveData(USART3);
	}
}

void USART1_IRQHandler(void)
{
	vu32 tdelay1;
	vu8 u1,u1i;
	vu16 tdat;
	unsigned char tXOR=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART1,USART_IT_RXNE);
		u1=USART_ReceiveData(USART1);
		if(flgU1StRecv==1)
		{
			u1RxDat[u1RxCnt]=u1;
			u1RxCnt++;
			if(u1RxCnt>=25)
			{
				flgU1StRecv=0;
				for(u1i=5;u1i<24;u1i++)
				  tXOR ^=u1RxDat[u1i];
				if((u1RxDat[2]==0x01)&&(u1RxDat[3]==0x03)&&(u1RxDat[4]==0x12)&&(tXOR==u1RxDat[24]))
				{
					flgConnSTA[0]=1;
					rioDat[0]=u1RxDat[5];CanData[72]=u1RxDat[5];
					rioDat[1]=u1RxDat[6];CanData[73]=u1RxDat[6];
					rioDat[2]=u1RxDat[7];CanData[74]=u1RxDat[7];
					//Dis_Hex(100,100,RED,WHITE,u1RxDat[5]); Dis_Hex(140,100,RED,WHITE,u1RxDat[6]); Dis_Hex(180,100,RED,WHITE,u1RxDat[7]);
					//Dis_Hex(100,120,RED,WHITE,u1RxDat[16]);Dis_Hex(140,120,RED,WHITE,u1RxDat[17]);Dis_Hex(180,120,RED,WHITE,u1RxDat[18]);Dis_Hex(220,120,RED,WHITE,u1RxDat[19]);
				}
        flgU1Stx2=0;flgU1Stx1=0;flgU1StRecv=0;
			}
		}
		if(flgU1StRecv==0)
		{
			if((flgU1Stx2==0)&&(flgU1Stx1==1)&&(u1==0xCC))
			{
				flgU1Stx2=1;
				flgU1StRecv=1;
				u1RxCnt=2;	//u4RxdLen=13;
			}
			else
			{
				flgU1Stx1=0;flgU1Stx2=0;flgU1StRecv=0;
			}
			if(u1==0xFF)//STX
			{
				flgU1Stx1=1;flgU1Stx2=0;
				u1STX=1;u1RxCnt=0;
				for(u1i=0;u1i<10;u1i++)
				{
					u1RxDat[u1i]=0;
				}	
			}
		}		
	}
	 if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
	  USART_ClearFlag(USART1,USART_FLAG_ORE); 
		USART_ReceiveData(USART1);
	}
  
}
/*
000004-Tx:01 03 00 F6 00 0E 24 3C //247 -14 质量流量,密度,温度,体积流量,压力（不用）
000003-Tx:01 03 01 A2 00 04 E4 17 //419 - 4 报警值，从419开始读，420-422有效
000000-Tx:01 03 01 96 00 02 25 DB //407 - 2 流量校准系数
000001-Tx:01 03 01 16 00 06 25 F0 //279 - 6 质量流量仪表系数,体积流量仪表系数,密度仪表系数
000002-Tx:01 03 00 E8 00 02 44 3F //233 - 2 零点
*/

unsigned char ConfDat=0;
unsigned char flgConfDat=0;
void chkFvMsg(void)
{
	unsigned char tLen,i;
	unsigned char tFvId;
	tFvId=FvId;
	switch(FvId)
	{
		case 0:tLen=32;tFvId=1;break;
		case 1:tLen= 8;
		if(sys_STA>0)
			tFvId=0;
		else
			tFvId=2;
		break;
		case 2:tLen= 4;tFvId=3;break;
		case 3:tLen=12;tFvId=4;break;
		case 4:tLen= 4;tFvId=0;break;
		default:tLen=0;tFvId=0;break;
	}
	if(u6FvBuf[2]==tLen)//数据长度符合，则数据正确
	{
		switch(FvId)
		{
			case 0://过程值
			/*
			SSLm: 3, 4, 5, 6;
			  MD: 7, 8, 9,10;
			  WD:11,12,13,14;
			SSLv:15,16,17,18;
			  NC:19,20,21,22;
		    YL:23,24,25,26;
			LJSm:27,28,29,30;
			LJSv:31,32,33,34;
			*/
					//瞬时体积量
					//CanData[40]=u6RxdBuf[15];CanData[41]=u6RxdBuf[16];CanData[42]=u6RxdBuf[17];CanData[43]=u6RxdBuf[18];
					canRtData[0]=u6FvBuf[15];canRtData[1]=u6FvBuf[16];canRtData[2]=u6FvBuf[17];canRtData[3]=u6FvBuf[18];
					//瞬时质量量
					//CanData[44]=u6RxdBuf[ 3];CanData[45]=u6RxdBuf[ 4];CanData[46]=u6RxdBuf[ 5];CanData[47]=u6RxdBuf[ 6];
					//介质密度
					canRtData[8]=u6FvBuf[ 7];canRtData[9]=u6FvBuf[ 8];canRtData[10]=u6FvBuf[ 9];canRtData[11]=u6FvBuf[10];
					//介质温度
					canRtData[4]=u6FvBuf[11];canRtData[5]=u6FvBuf[12];canRtData[6]=u6FvBuf[13];canRtData[7]=u6FvBuf[14];
					//累积体积
					canRtData[12]=u6FvBuf[31];canRtData[13]=u6FvBuf[32];canRtData[14]=u6FvBuf[33];canRtData[15]=u6FvBuf[34];
					//累积质量
					canRtData[16]=u6FvBuf[27];canRtData[17]=u6FvBuf[28];canRtData[18]=u6FvBuf[29];canRtData[19]=u6FvBuf[30];
				
					dSSLm=ctof(u6FvBuf[ 5],u6FvBuf[ 6],u6FvBuf[ 3],u6FvBuf[ 4]);//质量瞬时量
					dMD=ctof(u6FvBuf[ 9],u6FvBuf[10],u6FvBuf[ 7],u6FvBuf[ 8]);//密度
					dWD=ctof(u6FvBuf[13],u6FvBuf[14],u6FvBuf[11],u6FvBuf[12]);//温度
					dSSLv=ctof(u6FvBuf[17],u6FvBuf[18],u6FvBuf[15],u6FvBuf[16]);//体积瞬时量
					dLJSm=ctof(u6FvBuf[29],u6FvBuf[30],u6FvBuf[27],u6FvBuf[28]);//质量总量
					dLJSv=ctof(u6FvBuf[33],u6FvBuf[34],u6FvBuf[31],u6FvBuf[32]);//体积总量

			break;
			case 1://报警值,从419读，420有效
				for(i=0;i<6;i++)
				{
    				 FvWarnDat[i]=u6FvBuf[5+i];
					canRtData[35+i]=u6FvBuf[5+i];
				}
			break;
			case 2://流量校准系数
				for(i=0;i<4;i++)
				{
					  FvClibDat[i]=u6FvBuf[3+i];
				}
					ConfDat=1;flgConfDat=0;
			break;
			case 3://仪表系数
				for(i=0;i<12;i++)
				{
					  FvSetDat[i]=u6FvBuf[3+i];
				}
					ConfDat|=2;
					break;
			case 4://零点值
				for(i=0;i<4;i++)
				{
					   FvZeroDat[i]=u6FvBuf[3+i];
				}
					ConfDat|=4;
			break;
			
		}
		if(ConfDat==7)
		{
				for(i=0;i<4;i++)
				{
					canConfData[i]= FvClibDat[i];
				}
				for(i=0;i<12;i++)
				{
					 canConfData[4+i]=FvClibDat[i];
				}
				for(i=0;i<4;i++)
				{
				  canConfData[16+i]=FvZeroDat[i];
				}
				flgConfDat=1;
		}			
		FvId=tFvId;
		SendFvCmd(FvId);
	}
}
unsigned char u6RxdBuf[100]; 
float FvDat[6],disFvDat[6];
void USART6_IRQHandler(void)
{
	vu32 tdelay1;
	vu8 u6,u6i,crcHi=0,crcLo=0;
	u16 crcresult;
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART6,USART_IT_RXNE);
		u6=USART_ReceiveData(USART6);
		tmrU6=0;tDatInt=0;
		u6Buf[u6RxdCnt]=u6;
		u6RxdCnt++;
	}
	 if(USART_GetFlagStatus(USART6,USART_FLAG_ORE)==SET)
	{
	    USART_ClearFlag(USART6,USART_FLAG_ORE); 
		USART_ReceiveData(USART6);
	}
}
void USART6_IRQHandler_(void)
{
	vu32 tdelay1;
	vu8 u6,u6i,crcHi=0,crcLo=0;
	u16 crcresult;
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART6,USART_IT_RXNE);
		u6=USART_ReceiveData(USART6);
		if(u6Flg==1)
		{
		 u6RxdCnt++;
		 u6RxdBuf[u6RxdCnt]=u6;
//01 03 20 00 00 00 00 97 d6 3f 5a ee b0 41 b5 00 00 00 00 00 00 00 00 00 00 00 00 d2 b5 4a 50 77 01 45  7e
		 if(u6RxdCnt>=36)//mdDat[1]
		 {
				crcresult =CRC16(u6RxdBuf,35);
				crcHi=(crcresult & 0xff);
				//crcLo=(crcresult & 0xFF00)>>8;
		 		u6RxdCnt=0;
				u6Flg=0;
				u6NewTxdFlg=0;
				//if ((crcHi==u3RxdBuf[35])&&(crcLo==u3RxdBuf[36]))
				if (crcHi==u6RxdBuf[35])//	&&(crcLo==u3RxdBuf[36])
				//if((u3RxdBuf[1]==0x03)&&(u3RxdBuf[2]==0x20))
				{
					flgFvETX=1;
					//flgMbWr=1;
					//?????????
					cntFVconnTmr=0;
					//err_FV=0;
					//flgFvRxd=0;
					FvDat[0]=ctof(u6RxdBuf[ 5],u6RxdBuf[ 6],u6RxdBuf[ 3],u6RxdBuf[ 4]);//质量瞬时量
					FvDat[1]=ctof(u6RxdBuf[ 9],u6RxdBuf[10],u6RxdBuf[ 7],u6RxdBuf[ 8]);//密度
					FvDat[2]=ctof(u6RxdBuf[13],u6RxdBuf[14],u6RxdBuf[11],u6RxdBuf[12]);//温度
					FvDat[3]=ctof(u6RxdBuf[17],u6RxdBuf[18],u6RxdBuf[15],u6RxdBuf[16]);//体积瞬时量
					FvDat[4]=ctof(u6RxdBuf[29],u6RxdBuf[30],u6RxdBuf[27],u6RxdBuf[28]);//质量总量
					FvDat[5]=ctof(u6RxdBuf[33],u6RxdBuf[34],u6RxdBuf[31],u6RxdBuf[32]);//体积总量
					dSSLv=FvDat[3];dSSLm=FvDat[0];dLJSv=FvDat[5];dLJSm=FvDat[4];
					dMD=FvDat[1];dWD=FvDat[2];
					for(u6i=0;u6i<6;u6i++)
					  disFvDat[u6i]=FvDat[u6i];
					//瞬时体积量
					//CanData[40]=u6RxdBuf[15];CanData[41]=u6RxdBuf[16];CanData[42]=u6RxdBuf[17];CanData[43]=u6RxdBuf[18];
					canRtData[0]=u6RxdBuf[15];canRtData[1]=u6RxdBuf[16];canRtData[2]=u6RxdBuf[17];canRtData[3]=u6RxdBuf[18];
					//瞬时质量量
					//CanData[44]=u6RxdBuf[ 3];CanData[45]=u6RxdBuf[ 4];CanData[46]=u6RxdBuf[ 5];CanData[47]=u6RxdBuf[ 6];
					//介质密度
					canRtData[8]=u6RxdBuf[ 7];canRtData[9]=u6RxdBuf[ 8];canRtData[10]=u6RxdBuf[ 9];canRtData[11]=u6RxdBuf[10];
					//介质温度
					canRtData[4]=u6RxdBuf[11];canRtData[5]=u6RxdBuf[12];canRtData[6]=u6RxdBuf[13];canRtData[7]=u6RxdBuf[14];
					//累积体积
					canRtData[12]=u6RxdBuf[31];canRtData[13]=u6RxdBuf[32];canRtData[14]=u6RxdBuf[33];canRtData[15]=u6RxdBuf[34];
					//累积质量
					canRtData[16]=u6RxdBuf[27];canRtData[17]=u6RxdBuf[28];canRtData[18]=u6RxdBuf[29];canRtData[19]=u6RxdBuf[30];
					//零点值
					//CanData[64]=u6RxdBuf[27];CanData[65]=u6RxdBuf[28];CanData[66]=u6RxdBuf[29];CanData[67]=u6RxdBuf[30];
					//压力值
					//CanData[68]=u6RxdBuf[27];CanData[69]=u6RxdBuf[28];CanData[70]=u6RxdBuf[29];CanData[71]=u6RxdBuf[30];
					//mbFvDat[14]=(u6RxdBuf[3]<<8)+u6RxdBuf[4];//质量瞬时量
					//mbFvDat[15]=(u6RxdBuf[5]<<8)+u6RxdBuf[6];
					//mbFvDat[18]=(u6RxdBuf[11]<<8)+u6RxdBuf[12];//密度
					//mbFvDat[19]=(u6RxdBuf[13]<<8)+u6RxdBuf[14];
					//mbFvDat[20]=(u6RxdBuf[7]<<8)+u6RxdBuf[8];//温度
					//mbFvDat[21]=(u6RxdBuf[9]<<8)+u6RxdBuf[10];
					//mbFvDat[22]=(u6RxdBuf[27]<<8)+u6RxdBuf[28];//体积瞬时量
					//mbFvDat[23]=(u6RxdBuf[29]<<8)+u6RxdBuf[30];
					//mbFvDat[24]=(u6RxdBuf[31]<<8)+u6RxdBuf[32];//质量总量
					//mbFvDat[25]=(u6RxdBuf[33]<<8)+u6RxdBuf[34];
				 	//RxdHandler();
					//RxdEvents();
				}
//				else
//				{
//					cntFvErr+=10;
//					if(cntFvErr>0xFFF)
//						cntFvErr=0;
//				}
		 }
	 }
	 if (u6NewTxdFlg==0)
	 {
			if(u6==0x01)
			{
			 	u6Flg=1;
				for(u6i=0;u6i<100;u6i++)
				{
					 u6RxdBuf[u6i]=0;
				} 
				u6RxdCnt=0;
				u6NewTxdFlg=1;
				u6RxdBuf[0]=0x01;
				//flgFvCrcErr=0;
				flgFvETX=0;
			}
	 } 
   //USART_ClearFlag(USART3,USART_IT_RXNE);
	}
}

unsigned char u3TxDat[50];
void SendValv(unsigned char id)
{
  unsigned char i;
	//000331-Tx:02 03 00 9F 00 04 74 14
  //000330-Tx:03 03 00 9F 00 04 75 C5
	//02 03 00 9F 00 06 F5 D5
	//03 03 00 9F 00 06 F4 04
  //unsigned char MdTxdBuf[8]={0x01,0x04,0x00,0xF6,0x00,0x10,0x11,0xF4};
  unsigned char MdTxdBuf[2][8]={0x02,0x03,0x00,0x9F,0x00,0x06,0xF5,0xD5,0x03,0x03,0x00,0x9F,0x00,0x06,0xF4,0x04};
  //unsigned char MdTxdBuf[2][8]={0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
  //000002-Tx:01 03 00 F6 00 10 A4 34

  GPIO_ResetBits(GPIOA,GPIO_Pin_1);//U3准备接受PC指令
  delay(0xFFF);
	for(i=0;i<8;i++)
  {		
		u3MbBuf[i]=MdTxdBuf[id][i];  
    USART_SendData(USART3,u3MbBuf[i]);
    delay(0x6FF);
		
	}
  GPIO_SetBits(GPIOA,GPIO_Pin_1);//U3准备接受PC指令
  //delay(0xFFF);
	//u3Cnt=0;u3SndCnt=8;u3Flag=1;
  u3NewTxdFlg=0;
	
}

/*
000004-Tx:01 03 00 F6 00 0E 24 3C //247 -14 质量流量,密度,温度,体积流量,压力（不用）
000003-Tx:01 03 01 A2 00 04 E4 17 //419 - 4 报警值，从419开始读，420-422有效
000000-Tx:01 03 01 96 00 02 25 DB //407 - 2 流量校准系数
000001-Tx:01 03 01 16 00 06 25 F0 //279 - 6 质量流量仪表系数,体积流量仪表系数,密度仪表系数
000002-Tx:01 03 00 E8 00 02 44 3F //233 - 2 零点
*/
void SendFvCmd(unsigned char pvId)
{
  unsigned char i;
	unsigned int  tcrc;
  unsigned char txdBuf[9]={0x01,0x03,0x00,0xF6,0x00,0x10,0xA4,0x34,0x00};
  switch(pvId)
  {
	  case  0:txdBuf[1]=3;txdBuf[2]=0;txdBuf[3]=0xF6;txdBuf[5]=16; break;//247 -16 质量流量,密度,温度,体积流量,压力（不用）
	  case  1:txdBuf[1]=3;txdBuf[2]=1;txdBuf[3]=0xA2;txdBuf[5]= 4; break;//419 - 4 报警值，从419开始读，420-422有效
	  case  2:txdBuf[1]=3;txdBuf[2]=1;txdBuf[3]=0x96;txdBuf[5]= 2; break;//407 - 2 流量校准系数
	  case  3:txdBuf[1]=3;txdBuf[2]=1;txdBuf[3]=0x16;txdBuf[5]= 6; break;//279 - 6 质量流量仪表系数,体积流量仪表系数,密度仪表系数
	  case  4:txdBuf[1]=3;txdBuf[2]=0;txdBuf[3]=0xE8;txdBuf[5]= 2; break;//233 - 2 零点
  }
  tcrc=CRC16(txdBuf,6);
  txdBuf[6]=(tcrc & 0xff); //0xA4;
  txdBuf[7]=(tcrc & 0xFF00)>>8; //0x34;
  GPIO_ResetBits(GPIOD,GPIO_Pin_13);
	for(i=0;i<8;i++)
		u6MbBuf[i]=txdBuf[i];
	u6Cnt=0;u6SndCnt=8;u6Flag=1;
  u6NewTxdFlg=0;
}


void SendAllMd(void)
{
  unsigned char i;
  //unsigned char MdTxdBuf[8]={0x01,0x04,0x00,0xF6,0x00,0x10,0x11,0xF4};
  unsigned char MdTxdBuf[9]={0x01,0x03,0x00,0xF6,0x00,0x10,0xA4,0x34,0x00};
  //000002-Tx:01 03 00 F6 00 10 A4 34

  GPIO_ResetBits(GPIOD,GPIO_Pin_13);
  delay(0xF);
  for(i=0;i<8;i++)
  {
    //USART_SendData(USART6,MdTxdBuf[i]);
    //delay(0x12F);
		u6MbBuf[i]=MdTxdBuf[i];
  }
	u6Cnt=0;u6SndCnt=8;u6Flag=1;
  u6NewTxdFlg=0;
}
/*
地址,颜色,周期,蜂鸣器
空闲:0,灭
发油允许:1,绿闪
发油过程:2,绿
人工暂停:3,黄闪
报警暂停:4,红闪+BEEP
报警暂停:5,红闪
剩余量  :6,蓝闪
*/

void WrWarnLed(unsigned char id,unsigned char wDat)
{
  unsigned char i;
  unsigned int  tcrc;
  unsigned char txdBuf[15]={1,16,0,9,0,3,6,0,0,0,0,0,0,0,0};
  WarnLedDat.wlAdd=id+1;
  switch(wDat)
  {
  	case 0: WarnLedDat.wlColor=0;WarnLedDat.wlTmr=0x00;WarnLedDat.wlBeep=0;break;
  	case 1: WarnLedDat.wlColor=3;WarnLedDat.wlTmr=2000;WarnLedDat.wlBeep=0;break;
  	case 2: WarnLedDat.wlColor=3;WarnLedDat.wlTmr=0x8005;WarnLedDat.wlBeep=0;break;
  	case 3: WarnLedDat.wlColor=6;WarnLedDat.wlTmr=800;WarnLedDat.wlBeep=0;break;
  	case 4: WarnLedDat.wlColor=1;WarnLedDat.wlTmr=500;WarnLedDat.wlBeep=1;break;
  	case 5: WarnLedDat.wlColor=1;WarnLedDat.wlTmr=500;WarnLedDat.wlBeep=0;break;
  	case 6: WarnLedDat.wlColor=2;WarnLedDat.wlTmr=300;WarnLedDat.wlBeep=0;break;

  }
  for(i=0;i<15;i++)
  	TxdFvMdBuf[i]=txdBuf[i];
  TxdFvMdBuf[0]=WarnLedDat.wlAdd;
  TxdFvMdBuf[8]=WarnLedDat.wlColor;
  TxdFvMdBuf[9]=WarnLedDat.wlTmr>>8;
  TxdFvMdBuf[10]=WarnLedDat.wlTmr&0x00FF;
  TxdFvMdBuf[12]=WarnLedDat.wlBeep;
  tcrc=CRC16(TxdFvMdBuf,13);
  TxdFvMdBuf[13]=(tcrc & 0xff); //0xA4;
  TxdFvMdBuf[14]=(tcrc & 0xFF00)>>8; //0x34;
  TxdFvMdBuf[15]=0;
  //GPIO_ResetBits(GPIOA,GPIO_Pin_8);
  //GPIO_ResetBits(GPIOC,GPIO_Pin_8);
  GPIO_ResetBits(GPIOA,GPIO_Pin_0);//U2准备接受PC指令
  delay(0x2F0);
  for(i=0;i<15;i++)
  {
		USART_SendData(USART2,TxdFvMdBuf[i]);
		delay(0x6FF);
  }
  //GPIO_SetBits(GPIOA,GPIO_Pin_8);
  //GPIO_SetBits(GPIOC,GPIO_Pin_8);
  GPIO_SetBits(GPIOA,GPIO_Pin_0);//U2准备接受PC指令
 delay(0x1F);

}

void SendRIO(void)
{
	unsigned char i;
	//------------------------------------------------DIO--AoL1-AoH1-AoL2-AoH2-----
	unsigned char MdTxd[11]={0xFF,0xCC,0x01,0x03,0x05,0x55,0x00,0x00,0x00,0x00,0xFF};
	unsigned char tXOR=0;
	if((sys_STA>0)&&(sys_STA<6))
	{
		if(dualChNum==1){	rio_AoH1=AoDat1>>8;rio_AoL1=AoDat1&0x0FF;rio_AoH2=0x02;rio_AoL2=0x58;}
		if(dualChNum==2){	rio_AoH2=AoDat2>>8;rio_AoL2=AoDat1&0x0FF;rio_AoH1=0x02;rio_AoL1=0x58;}		
	}
	else
	{
		AoDat1=600;
		rio_AoH1=0x02;rio_AoL1=0x58;rio_AoH2=0x02;rio_AoL2=0x58;
	}
	MdTxd[5]=rio_DO;
	MdTxd[6]=rio_AoL1;
	MdTxd[7]=rio_AoH1;
	MdTxd[8]=rio_AoL2;
	MdTxd[9]=rio_AoH2;
	for(i=5;i<10;i++)
		tXOR ^=MdTxd[i];
	MdTxd[10]=tXOR;
	for(i=0;i<11;i++)
	{		u1MbBuf[i]=MdTxd[i];	}
	u1Cnt=0;u1SndCnt=11;u1Flag=1;
	
	
}
void SendFTA(unsigned char ftaDat)
{
	//unsigned char i;
	////GPIO_SetBits(GPIOC,GPIO_Pin_4);//U3
	//u3TxDat[0]=0x4A;
	//u3TxDat[1]=0x30+(ftaDat>>4);
	//u3TxDat[2]=0x30+(ftaDat&0x0F);
	//u3TxDat[3]=0x4B;
	//u3TxDat[4]=0;
	//GPIO_SetBits(GPIOC,GPIO_Pin_4);//U3  ADM2587E 发送为1，RSM3485 发送为0
	////GPIO_ResetBits(GPIOE,GPIO_Pin_2);//U4
	//delay(0x2F0);
	//for(i=0;i<4;i++)
	//{
	//	//u4MbBuf[i]=u3TxDat[i];
	//	USART_SendData(USART3,u3TxDat[i]);
	//	delay(0x2FFF);
	//}
	////delay(0x2F0);
	////u4Cnt=0;u4SndCnt=5;u4Flag=1;
	//GPIO_ResetBits(GPIOC,GPIO_Pin_4);//U3
	////GPIO_ResetBits(GPIOE,GPIO_Pin_2);//U4
	////GPIO_SetBits(GPIOE,GPIO_Pin_2);//U4
	////delay(0x2FF);
	
}
void FbRoadGate(void)
{
		//unsigned long tLngDat;
		//unsigned char tDat[8],i;
		//unsigned char WeiDatBCD[4],DiDat,DoDat,CrcDat;
		//u4MbBuf[0]=0xFF;u4MbBuf[1]=0xCC;u4MbBuf[2]=0x01;u4MbBuf[3]=0x01;
		//u4MbBuf[4]=0x03;u4MbBuf[5]=0x60;u4MbBuf[6]=0x41;u4MbBuf[7]=0x55;
		//CrcDat=0;
		//for(i=2;i<8;i++)
		//{
		//	CrcDat=(CrcDat + u4MbBuf[i])&0xFF;
		//}
		//u4MbBuf[8]=CrcDat;
		//GPIO_SetBits(GPIOE,GPIO_Pin_2);
		//u4Cnt=0;u4SndCnt=9;u4Flag=1;
}
void FbPatrol(void)
{
		//unsigned long tLngDat;
		//unsigned char tDat[8],i;
		//unsigned char WeiDatBCD[4],DiDat,DoDat,CrcDat;
		//tLngDat=lngWeiDat*10;
		//tDat[0]=0;
		//tDat[1]=tLngDat/1000000;
		//tLngDat=tLngDat%1000000;
		//tDat[2]=tLngDat/100000;
		//tLngDat=tLngDat%100000;
		//tDat[3]=tLngDat/10000;
		//tLngDat=tLngDat%10000;
		//tDat[4]=tLngDat/1000;
		//tLngDat=tLngDat%1000;
		//tDat[5]=tLngDat/100;
		//tLngDat=tLngDat%100;
		//tDat[6]=tLngDat/10;
		//tLngDat=tLngDat%10;
		//tDat[7]=tLngDat;
		//WeiDatBCD[0]=(tDat[0]<<4)+tDat[1];//0x00;//
		//WeiDatBCD[1]=(tDat[2]<<4)+tDat[3];//0x41;//
		//WeiDatBCD[2]=(tDat[4]<<4)+tDat[5];//0x89;//
		//WeiDatBCD[3]=(tDat[6]<<4)+tDat[7];//0x20;//
		//u4MbBuf[0]=0xFF;u4MbBuf[1]=0xCC;u4MbBuf[2]=0x01;u4MbBuf[3]=0x01;
		//u4MbBuf[4]=0x07;u4MbBuf[5]=0x61;
		//u4MbBuf[6]=WeiDatBCD[0];
		//u4MbBuf[7]=WeiDatBCD[1];
		//u4MbBuf[8]=WeiDatBCD[2];
		//u4MbBuf[9]=WeiDatBCD[3];
		//u4MbBuf[10]=(unsigned char)(ftaDIdat&0xFF);
		//u4MbBuf[11]=ftaDOdat;
		//CrcDat=0;
		//for(i=2;i<12;i++)
		//{
		//	CrcDat=(CrcDat + u4MbBuf[i])&0xFF;
		//}
		//u4MbBuf[12]=CrcDat;
		//GPIO_SetBits(GPIOE,GPIO_Pin_2);
		//u4Cnt=0;u4SndCnt=13;u4Flag=1;
}
void SendCard(unsigned char tDat)
{
	unsigned char ti;
	unsigned char IcTxdBuf[12]={0xFF,0xCC,0x01,0x57,0x38,0x31,0x30,0x36,0x31,0x38,0x9C,0x00};
	unsigned char tXorDat;
	if(tDat==0x01)//发油密码
	{
	 IcTxdBuf[3]=0x57;
	 tXorDat=0xCC;
	 for(ti=2;ti<10;ti++)
	  tXorDat=tXorDat ^ IcTxdBuf[ti];
	 IcTxdBuf[10]=tXorDat;
	}
	if(tDat==0x02)
	{
		IcTxdBuf[3]=0x52;//原用户密码改成IC卡号
		tXorDat=0xCC;
		for(ti=2;ti<10;ti++)
		  tXorDat=tXorDat ^ IcTxdBuf[ti];
		IcTxdBuf[10]=tXorDat;
	}
	if(tDat==0x03)//车号【汉字+5字母】+油品编码+
	{
	 IcTxdBuf[3]=0x59;
	 tXorDat=0xCC;
	 for(ti=2;ti<10;ti++)
	  tXorDat=tXorDat ^ IcTxdBuf[ti];
	 IcTxdBuf[10]=tXorDat;
	}
	IcTxdBuf[11]=0x00;
	GPIO_SetBits(GPIOE,GPIO_Pin_1);
	delay(0x2F0);
	for(ti=0;ti<11;ti++)
	{
		USART_SendData(UART5,IcTxdBuf[ti]);
		delay(0x6FFF);
	}
	GPIO_ResetBits(GPIOE,GPIO_Pin_1);
	//Delay(0x1FF);
	flgU5Stx1=0;
	flgU5Stx2=0;
	flgU5StRecv=0;
	u5RxdCnt=0;
}


void SendPKQ(unsigned char id)
{
	//unsigned char MdTxdBuf[9]={0x01,0x03,0x00,51,0x00,41,0x11,0xF4,0};
	//u16 crcresult;
	//u8  crcHi=0,crcLo=0;
	//unsigned char i;
	//MdTxdBuf[0]=id;
	//MdTxdBuf[5]=41;
	//crcresult = CRC16(MdTxdBuf,6);
	//crcHi=(crcresult & 0xff);
	//crcLo=(crcresult & 0xFF00)>>8;
	//MdTxdBuf[6]=crcHi;MdTxdBuf[7]=crcLo;
	//GPIO_ResetBits(GPIOE,GPIO_Pin_2);
	//u4RxdCnt=0;
	//lenMbDat=MB_NUM*2+5;
	//delay(0x2F0);
	//for(i=0;i<8;i++)
	//{
	//	USART_SendData(UART4,MdTxdBuf[i]);
	//	delay(0x3FFF);
	//}
	//GPIO_SetBits(GPIOE,GPIO_Pin_2);
	//delay(0x2FF);
	//PKQ_Msg[id].pTmr++;
}

/*******************************************************************************
* Function Name  : crc16
* Description	 : Configures the different system clocks.
* Input 		 : *puchMsg(数据地址)    usDataLen
* Return		 : 2 byte   
*******************************************************************************/
unsigned short CRC16(unsigned char *puchMsg,unsigned short	usDataLen)
{

	unsigned char uchCRCHi = 0xFF ; /* 高CRC字节初始化 */
	unsigned char uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */
	unsigned uIndex ; /* CRC循环中的索引 */
	while (usDataLen--) /* 传输消息缓冲区 */
	{
		uIndex = uchCRCLo ^ *puchMsg++;
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
	}
   return (uchCRCHi << 8 | uchCRCLo) ;
}





