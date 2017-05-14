#include "define.h"


//ð������-����
void MySort(uint16_t *p,uint16_t len)
{
	uint16_t temp;
	for(uint16_t i=0;i<len-1;i++)
	{
		for(uint16_t j=0;j<len-i-1;j++)
		{
			if( *(p)>*(p+j))
			{
				temp=*(p);
				*(p)=*(p+j);
				*(p+j)=temp;
			}
		}
	}
}

//�����豸��ʼ��
void NetWork_Init(void)
{
	unsigned char i;
	
	//ethernet init
	W5500SPIHardWareConfig();//w5500�������ü���ʼ��
	ctlwizchip( CW_RESET_WIZCHIP,NULL);//��ʼ��оƬ
	ctlwizchip( CW_SET_INTRMASK,NULL);//�����ж�
  ctlnetwork(CN_SET_TIMEOUT, (void*)&gWIZTIMEOUT);//�������ʱ��
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);//�����������
	ctlwizchip(CW_INIT_WIZCHIP,(void*)TX_RX_BufferSize);//����socket�����С
	
	for(i=0;i<SOCKET_NUM;i++)
	{
		IINCHIP_WRITE(Sn_KPALVTR(i),HEART_TIME);//����ͨ��0������⹦�ܡ�����λʱ�䣺5S
	}
	
	//end ethernet init	
}
