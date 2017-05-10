#include "sys.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "timer3.h"
#include "sim900a.h"


#define		FRAME_END_TIME_100US		100		//֡����ʱ��������λ100US		

extern u16 USART1_RX_STA; 
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.
extern u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.

int main(void)
{ 
	u8 *p1,*p2;
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
	delay_init(168);		//��ʱ��ʼ�� 
	LCD_Init( );
	usart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	TIM3_Int_Init( FRAME_END_TIME_100US-1,8400-1);//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����50��Ϊ5ms   	
	USART1_RX_STA=0;
	LCD_ShowString( 100,0,200,16,16,(u8 *)"sim900a test!");
	while(1)
	{
		
		if(sim900a_send_cmd("AT+CGMI","OK",200)==0)				//��ѯ����������
		{ 
			p1=(u8*)strstr((const char*)(USART1_RX_BUF),"\r\n");
//			p1[0]=0;//���������
			LCD_ShowString( 50,400,200,16,16,(u8 *)p1);//USART1_RX_BUF+2
			USART1_RX_STA=0;		
		} 
		
//		USART1_RX_STA=0;
//		if( sim900a_send_cmd( (u8*)"AT+CSQ",(u8*)"OK",200))
//		{
//			LCD_ShowString( 50,100,200,16,16,(u8 *)"send failed!");
//		}
//		else
//		{
//			LCD_ShowString( 50,100,200,16,16,(u8 *)"send success!");
//		}
		
		
		
		
		
		
		
		
		while (1);	

	}
}







