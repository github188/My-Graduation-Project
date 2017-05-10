#include "sys.h"
#include "string.h"
#include "delay.h"
#include "lcd.h"//lcd显示单元
#include "modbus_tcp.h"//mb协议单元
#include "usart1.h"////串口单元1
#include "timer4_usart.h"//串口单元2
#include "sim900a.h"//GPRS单元
#include "gpio.h"//gpio单元
#include "timer.h"//周期定时、频率测量
#include "transaction.h"//周期定时、频率测量


//是否使用SIM900A发送数据
#define		USE_SIM900A					1


/*
**从站地址
**功能：0x10写多个寄存器
**寄存器起始地址：0
**寄存器数量：
**数据缓存：
*/
volatile mb_info_struct MB_Write_Ch1_Struct={0x01,0x10,0,SEND_DATA_LEN,TcpServer_WriteDataBuf1};//mb写操作-子站信息
//volatile mb_info_struct MB_Write_Ch2_Struct={0x02,0x10,0,SEND_DATA_LEN,TcpServer_WriteDataBuf1};//mb写操作-子站信息
/*
**从站地址
**功能：0x03读多个寄存器
**寄存器起始地址：0
**寄存器数量：
**数据缓存：
*/
volatile mb_info_struct MB_Read_Ch1_Struct={0x01,0x03,SEND_DATA_LEN,RECV_DATA_LEN,TcpServer_ReadDataBuf1};//mb读操作-子站信息

/*
****
****server public ip:	115.159.180.213				port:502
****
*/
const char *Server_IpAddr="115.159.180.213";	//服务器ip
const char *Server_Port="502";							//服务器监听端口



int main(void)
{ 
//	static uint32_t mb_sendread_tt=0,mb_sendwrite_tt=0;//发送间隔
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置系统中断优先级分组2
	
	delay_init(168);		//延时初始化 
	delay_ms( 100);
	Usart_Init(115200);	//串口初始化波特率为115200	
	delay_ms( 200);
	LCD_Init( );				//lcd初始化
	delay_ms( 200);
#if		USE_SIM900A
	sim900a_init();		//sim900a模块初始化
	delay_ms( 200);
#endif

	LCD_ShowString( 100,0,200,16,16,(u8 *)"VM Test");
	USART1_RX_STA=0;		//初始状态为0
	
	while(1)
	{	

		//输入扫描
		Input_Key=Key_Scan();
		//
		if(Input_Key!=KEY_NULL)
		{
			Transaction_Process();//交易流程
		}
		//
		if(Transaction_Finished!=NO)
		{
			Transaction_Finished=NO;
			//通知发货机构动作
			NoticeDevice_Action();
		}
	}
}















