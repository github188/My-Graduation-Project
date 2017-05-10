#include "sys.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "timer3.h"
#include "sim900a.h"


#define		FRAME_END_TIME_100US		100		//帧结束时间间隔，单位100US		

extern u16 USART1_RX_STA; 
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.
extern u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.

int main(void)
{ 
	u8 *p1,*p2;
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	
	delay_init(168);		//延时初始化 
	LCD_Init( );
	usart_init(115200);	//串口初始化波特率为115200
	TIM3_Int_Init( FRAME_END_TIME_100US-1,8400-1);//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数50次为5ms   	
	USART1_RX_STA=0;
	LCD_ShowString( 100,0,200,16,16,(u8 *)"sim900a test!");
	while(1)
	{
		
		if(sim900a_send_cmd("AT+CGMI","OK",200)==0)				//查询制造商名称
		{ 
			p1=(u8*)strstr((const char*)(USART1_RX_BUF),"\r\n");
//			p1[0]=0;//加入结束符
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







