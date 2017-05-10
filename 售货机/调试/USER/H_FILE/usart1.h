#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 


#define		FRAME_END_TIME_100US		100		//帧结束时间间隔，单位100US		

#define		USART1_MAX_RECV_LEN		128					//接收缓冲,最大USART1_MAX_RECV_LEN字节
#define		USART1_MAX_SEND_LEN		128					//接收缓冲,最大USART1_MAX_RECV_LEN字节
//数据缓存
extern volatile u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.
extern volatile u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.

//通过判断接收连续2个字符之间的时间差不大于10ms(通过tim4判断)来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
extern volatile u16 USART1_RX_STA;  
	  	
	
	
//如果想串口中断接收，请不要注释以下宏定义
void Usart_Init(u32 bound);
void u1_printf(char* fmt,...);//待研究

void USART_SendHex(USART_TypeDef* USARTx, u8* Hdata,u16 len);
void USART_SendStr(USART_TypeDef* USARTx,char* Pdata);



#endif










