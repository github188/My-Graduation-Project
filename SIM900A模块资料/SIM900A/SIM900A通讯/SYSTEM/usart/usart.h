#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 


#define		USART1_MAX_RECV_LEN		1024					//接收缓冲,最大USART1_MAX_RECV_LEN字节
#define		USART1_MAX_SEND_LEN		1024					//接收缓冲,最大USART1_MAX_RECV_LEN字节
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.
extern u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.
//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
extern u16 USART1_RX_STA;  
	  	
	
	
//如果想串口中断接收，请不要注释以下宏定义
void usart_init(u32 bound);
void u1_printf(char* fmt,...);
void USART_SendStr(USART_TypeDef* USARTx,const char* Pdata);




#endif










