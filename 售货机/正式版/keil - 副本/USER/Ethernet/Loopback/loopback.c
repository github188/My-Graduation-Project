
#include "loopback.h"
#include "../../Ethernet/socket.h"
//add
#include "package.h"
#include "lcd.h"
#include "string.h" 

//server
uint8_t tcp_server_databuf[1024];   	//�������ݻ���	
uint8_t tcp_server_data_len;		//���ͻ��泤��
uint8_t tcp_server_sta[CLIENT_MAXNUM];				//�ͻ���״̬

uint8_t read_result;
//TCP server backup
int32_t loopback_server(uint8_t sn, uint8_t* buf, uint16_t port)
{
		int32_t ret;
	
		read_result=getSn_SR(sn);
		switch(read_result)	//��ȡsocket��״̬
		{
			//socket��ʼ�����
			case SOCK_INIT:	
				listen(sn);			//����TCP�ͻ��˳�������
				break;
			
//			case SOCK_LISTEN:	
//				listen(sn);			//����TCP�ͻ��˳�������
//				break;
			
			
			//socket���ӽ���
			case SOCK_ESTABLISHED:
				//״̬��Ϊ���Ӻ��ж������жϱ�־�Ƿ�����֤�����ӳɹ���������жϱ�־λ<д��λ>
				//�����һ�����ӽ������Ͽ���ִֻ��һ��
				if(getSn_IR(sn) & Sn_IR_CON)
				{
						setSn_IR(sn,Sn_IR_CON);//д��λ������жϱ�־λ
						tcp_server_sta[sn]|=1<<7;		//��־���ӳɹ�
				}
				if( getSn_RX_RSR(sn) > 0)//�жϽ���״̬�Ĵ����Ƿ���յ�����
				{
						if((tcp_server_sta[sn]&(1<<6))==0)//û��δ�������ݣ������������
						{
								//���շ���������
								ret = recv(sn,tcp_server_databuf,RECV_MAX_DATALEN);	//W5200��������Client������
								if( ret<=0)	break;//���մ���
								slave_modbus_response( tcp_server_databuf);
								tcp_server_sta[sn]|=1<<5;//�����������Ҫ����
								tcp_server_sta[sn]|=1<<6;//��ʾ�յ��ͻ������� 
						}
				}
				if(tcp_server_sta[sn]&(1<<5))//��������Ҫ����
				{
						//���������������
						ret=send(sn,tcp_server_databuf,tcp_server_data_len);
						if( ret<tcp_server_data_len)
						{
								//���ʹ���
								break;
						}
						tcp_server_sta[sn]&=~(1<<5);//������
						tcp_server_sta[sn]&=~(1<<6);//������
				}	
			 break;
			//socket�ȴ��ر�״̬
			case SOCK_CLOSE_WAIT:
         if((ret=disconnect(sn)) != SOCK_OK){	
					 break;
				 }
				 tcp_server_sta[sn]&=~(1<<7);	//��־û������    
				 break;
			//socket�ر�
			case SOCK_CLOSED:
				tcp_server_sta[sn]&=~(1<<7);	//��־û������   
				ret = socket(sn,Sn_MR_TCP,port,Sn_MR_ND);//��socket0��һ���˿�
				if(ret != sn){
//					while(1);
				}
				break;
	}
return 0;				
}





