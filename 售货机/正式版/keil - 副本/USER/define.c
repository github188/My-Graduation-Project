#include "define.h"


//冒泡排序-升序
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

//网络设备初始化
void NetWork_Init(void)
{
	unsigned char i;
	
	//ethernet init
	W5500SPIHardWareConfig();//w5500引脚配置及初始化
	ctlwizchip( CW_RESET_WIZCHIP,NULL);//初始化芯片
	ctlwizchip( CW_SET_INTRMASK,NULL);//掩饰中断
  ctlnetwork(CN_SET_TIMEOUT, (void*)&gWIZTIMEOUT);//设置溢出时间
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);//设置网络参数
	ctlwizchip(CW_INIT_WIZCHIP,(void*)TX_RX_BufferSize);//设置socket缓存大小
	
	for(i=0;i<SOCKET_NUM;i++)
	{
		IINCHIP_WRITE(Sn_KPALVTR(i),HEART_TIME);//启用通道0心跳监测功能——单位时间：5S
	}
	
	//end ethernet init	
}
