#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 


#define		USART1_MAX_RECV_LEN		1024					//���ջ���,���USART1_MAX_RECV_LEN�ֽ�
#define		USART1_MAX_SEND_LEN		1024					//���ջ���,���USART1_MAX_RECV_LEN�ֽ�
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.
extern u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.
//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
extern u16 USART1_RX_STA;  
	  	
	
	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void usart_init(u32 bound);
void u1_printf(char* fmt,...);
void USART_SendStr(USART_TypeDef* USARTx,const char* Pdata);




#endif










