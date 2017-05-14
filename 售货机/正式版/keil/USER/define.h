
#ifndef  __DEFINE_H_
#define  __DEFINE_H_
#include "stm32f4xx.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SPI_W500.h"//w5500硬件配置
#include "wizchip_conf.h"//w5500参数配置
#include "loopback.h"//server处理程序
#include "socket.h"//socket库
#include "package.h"//modbus模块

//发送缓存分配-总共16K
#define		TX1_BUFSIZE		4
#define		TX2_BUFSIZE		4
#define		TX3_BUFSIZE		4
#define		TX4_BUFSIZE		4
#define		TX5_BUFSIZE		0
#define		TX6_BUFSIZE		0
#define		TX7_BUFSIZE		0
#define		TX8_BUFSIZE		0
//接收缓存分配-总共16K
#define		RX1_BUFSIZE		4
#define		RX2_BUFSIZE		4
#define		RX3_BUFSIZE		4
#define		RX4_BUFSIZE		4
#define		RX5_BUFSIZE		0
#define		RX6_BUFSIZE		0
#define		RX7_BUFSIZE		0
#define		RX8_BUFSIZE		0

//定义需要发送的数据量
#define		SEND_DATA_LEN				8+1			//发送:4个AD值+2个脉冲周期值(us)+1个通道标识
//#define		AD_SAMPLIING_NUM		500			//ad采样频率
#define		CLIENT_NUM					1				//客户端数量
#define		SOCKET_NUM					4				//客户端数量
#define		HEART_TIME					1				//心跳时间=HEART_TIME*5S


//server s
extern uint8_t tcp_server_databuf[];   	//发送数据缓存	
extern uint8_t tcp_server_data_len;		//发送缓存长度
extern uint8_t tcp_server_sta[CLIENT_MAXNUM];//客户端状态

extern wiz_NetInfo gWIZNETINFO;
extern unsigned char TX_RX_BufferSize[][8];
extern wiz_NetTimeout  gWIZTIMEOUT;

void MySort(uint16_t *p,uint16_t len);//冒泡排序
void NetWork_Init(void);//网络初始化


#endif




