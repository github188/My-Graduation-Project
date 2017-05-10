#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 


#define		FRAME_END_TIME_100US		100		//֡����ʱ��������λ100US		

#define		USART1_MAX_RECV_LEN		128					//���ջ���,���USART1_MAX_RECV_LEN�ֽ�
#define		USART1_MAX_SEND_LEN		128					//���ջ���,���USART1_MAX_RECV_LEN�ֽ�
//���ݻ���
extern volatile u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.
extern volatile u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms(ͨ��tim4�ж�)�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
extern volatile u16 USART1_RX_STA;  
	  	
	
	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void Usart_Init(u32 bound);
void u1_printf(char* fmt,...);//���о�

void USART_SendHex(USART_TypeDef* USARTx, u8* Hdata,u16 len);
void USART_SendStr(USART_TypeDef* USARTx,char* Pdata);



#endif










