
#ifndef  __DEFINE_H_
#define  __DEFINE_H_
#include "stm32f4xx.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SPI_W500.h"//w5500Ӳ������
#include "wizchip_conf.h"//w5500��������
#include "loopback.h"//server�������
#include "socket.h"//socket��
#include "package.h"//modbusģ��

//���ͻ������-�ܹ�16K
#define		TX1_BUFSIZE		4
#define		TX2_BUFSIZE		4
#define		TX3_BUFSIZE		4
#define		TX4_BUFSIZE		4
#define		TX5_BUFSIZE		0
#define		TX6_BUFSIZE		0
#define		TX7_BUFSIZE		0
#define		TX8_BUFSIZE		0
//���ջ������-�ܹ�16K
#define		RX1_BUFSIZE		4
#define		RX2_BUFSIZE		4
#define		RX3_BUFSIZE		4
#define		RX4_BUFSIZE		4
#define		RX5_BUFSIZE		0
#define		RX6_BUFSIZE		0
#define		RX7_BUFSIZE		0
#define		RX8_BUFSIZE		0

//������Ҫ���͵�������
#define		SEND_DATA_LEN				8+1			//����:4��ADֵ+2����������ֵ(us)+1��ͨ����ʶ
//#define		AD_SAMPLIING_NUM		500			//ad����Ƶ��
#define		CLIENT_NUM					1				//�ͻ�������
#define		SOCKET_NUM					4				//�ͻ�������
#define		HEART_TIME					1				//����ʱ��=HEART_TIME*5S


//server s
extern uint8_t tcp_server_databuf[];   	//�������ݻ���	
extern uint8_t tcp_server_data_len;		//���ͻ��泤��
extern uint8_t tcp_server_sta[CLIENT_MAXNUM];//�ͻ���״̬

extern wiz_NetInfo gWIZNETINFO;
extern unsigned char TX_RX_BufferSize[][8];
extern wiz_NetTimeout  gWIZTIMEOUT;

void MySort(uint16_t *p,uint16_t len);//ð������
void NetWork_Init(void);//�����ʼ��


#endif




