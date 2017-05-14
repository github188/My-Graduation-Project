
#include "loopback.h"
#include "../../Ethernet/socket.h"
//add
#include "package.h"
#include "lcd.h"
#include "string.h" 

//server
uint8_t tcp_server_databuf[1024];   	//发送数据缓存	
uint8_t tcp_server_data_len;		//发送缓存长度
uint8_t tcp_server_sta[CLIENT_MAXNUM];				//客户端状态

uint8_t read_result;
//TCP server backup
int32_t loopback_server(uint8_t sn, uint8_t* buf, uint16_t port)
{
		int32_t ret;
	
		read_result=getSn_SR(sn);
		switch(read_result)	//获取socket的状态
		{
			//socket初始化完成
			case SOCK_INIT:	
				listen(sn);			//监听TCP客户端程序连接
				break;
			
//			case SOCK_LISTEN:	
//				listen(sn);			//监听TCP客户端程序连接
//				break;
			
			
			//socket连接建立
			case SOCK_ESTABLISHED:
				//状态变为连接后，判断连接中断标志是否置起，证明连接成功！并清除中断标志位<写该位>
				//这里从一次连接建立到断开，只执行一次
				if(getSn_IR(sn) & Sn_IR_CON)
				{
						setSn_IR(sn,Sn_IR_CON);//写该位，清除中断标志位
						tcp_server_sta[sn]|=1<<7;		//标志连接成功
				}
				if( getSn_RX_RSR(sn) > 0)//判断接收状态寄存器是否接收到数据
				{
						if((tcp_server_sta[sn]&(1<<6))==0)//没有未处理数据，则接收新数据
						{
								//接收服务器数据
								ret = recv(sn,tcp_server_databuf,RECV_MAX_DATALEN);	//W5200接收来自Client的数据
								if( ret<=0)	break;//接收错误
								slave_modbus_response( tcp_server_databuf);
								tcp_server_sta[sn]|=1<<5;//标记有数据需要发送
								tcp_server_sta[sn]|=1<<6;//表示收到客户端数据 
						}
				}
				if(tcp_server_sta[sn]&(1<<5))//有数据需要发送
				{
						//向服务器发送数据
						ret=send(sn,tcp_server_databuf,tcp_server_data_len);
						if( ret<tcp_server_data_len)
						{
								//发送错误
								break;
						}
						tcp_server_sta[sn]&=~(1<<5);//清除标记
						tcp_server_sta[sn]&=~(1<<6);//清除标记
				}	
			 break;
			//socket等待关闭状态
			case SOCK_CLOSE_WAIT:
         if((ret=disconnect(sn)) != SOCK_OK){	
					 break;
				 }
				 tcp_server_sta[sn]&=~(1<<7);	//标志没有连接    
				 break;
			//socket关闭
			case SOCK_CLOSED:
				tcp_server_sta[sn]&=~(1<<7);	//标志没有连接   
				ret = socket(sn,Sn_MR_TCP,port,Sn_MR_ND);//打开socket0的一个端口
				if(ret != sn){
//					while(1);
				}
				break;
	}
return 0;				
}





