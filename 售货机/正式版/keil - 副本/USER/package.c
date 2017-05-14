
#include "string.h"
#include "package.h"
#include "float_hex.h"
#include <stdint.h> 
#include "stm32f4xx_gpio.h"
#include "Global.h"


//server 
extern uint8_t tcp_server_databuf[200];   	//�������ݻ���	
extern uint8_t tcp_server_data_len;		//���ͻ��泤��
extern unsigned int mbDat[200];
//8λ�Ĵ���-����ģ��
#define		MAX_DATALEN		200
volatile unsigned char LocalData_Buf[MAX_DATALEN];//ģ�����ݻ���
/*
#define		MAX_TDATALEN		200
volatile unsigned char ReadData_Buf[MAX_TDATALEN];
#define		MAX_RDATALEN		200
volatile unsigned char WriteData_Buf[MAX_RDATALEN];
*/


//������һ֡modbus��Ϣʱ������һ����ʶ������Ӧ֡������1���յ���Ӧʱ���ٽ��ñ�ʶ������
//ͨ��������δ����λ��Ԫ�أ��õ����б�ʶ��
#define		TID_MAX		20
unsigned char TID[TID_MAX];//�������ʶ��

//������������ʶ��
unsigned char distrub_freeTID(void)
{
	unsigned char i;
	
	for( i=0;i<TID_MAX;	i++) 
	{
		if( TID[i]!=1)
		{
			TID[i]=1;
			return i;
		}
	}
	if( TID_MAX==i)
	{
		//�����ʶ��������
		//֮ǰ���ͳ�ȥ��֡��û����Ӧ
		//����Ӧ������
	}
return NULL;
}

//�����յ���Ӧ�������ʶ��
void free_distrubedTID(unsigned char t)
{
	TID[t]=0;
}
//�жϷ��ص��Ƿ�Ϊ�ͻ��˷���������ʶ��
//0-��ȷ��Ӧ
//1-��Ч��Ӧ
unsigned char check_isdistrubed_TID( unsigned char t)
{
	if( TID[t]==0)
		return 1;
return 0;
}

//����ָ����С�ֽڴ�ԭ���鵽Ŀ������
void byte_copy(unsigned char  *des,unsigned char *src,unsigned short size)
{		
	unsigned char i;
	for( i=0;i<size;i++)
	{
		des[i]=src[i];
	}
}
//����ָ����С�ֽڴ�ԭ���鵽Ŀ������
void u16_copy(unsigned short  *des,unsigned short *src,unsigned short size)
{		
	unsigned short i;
	for( i=0;i<size;i++)
	{
		des[i]=src[i];
	}
}

//�������ܣ�modbus�������ݸ���վ
//����������
//��������ֵ��0-������-1-����
int modbus_send( unsigned char *data)
{
	unsigned char data_index=0,temp,i;
	unsigned int trans_id;
	unsigned char Modbus_CmdState_Flag=0x00;//�����־������ָʾ��Ӧ״̬
	unsigned char mb_send_buffer[200];//����������ʱ�������
	unsigned int reg_addr,reg_num;//�Ĵ�����ʼ��ַ���Ĵ�������
	
	//ͨ�ò���
	data_index=7;
	temp=*(data+data_index);
	//�жϹ������Ƿ���Ч
	if((temp==modbus_cmd_Radio)|(temp==modbus_cmd_ReadKeepReg)|(temp==modbus_cmd_WriteOneReg))//(temp==modbus_cmd_Radio)|(temp==modbus_cmd_ReadKeepReg)|(temp==modbus_cmd_WriteOneReg)
	{
		//���ݹ�����������ݳ��Ⱥ����ݸ�ʽ��8bit/16bit��
		switch( temp)
		{
			//�㲥��ַ
			case modbus_cmd_Radio:
			break;
			
			//д������Ȧ
			case modbus_cmd_WriteOneReg:
			//�жϵ�ַ��Χ
				reg_addr=*(data+2)*256+(*(data+3));
				if((REG_ADDR_MIN<=reg_addr)&&(reg_addr<=REG_ADDR_MAX))//REG_ADDR_MIN<=reg_addr<=REG_ADDR_MAX
				{
					mb_send_buffer[data_index++]=temp;//������
					mb_send_buffer[data_index++]=*( data+2);//�Ĵ�����ַ��8λ
					mb_send_buffer[data_index++]=*( data+3);//�Ĵ�����ַ��8λ			
					mb_send_buffer[data_index++]=*( data+4);//data high 8 bit
					mb_send_buffer[data_index++]=*( data+5);//data low 8bit
					Modbus_CmdState_Flag=0x00;//����λ���������λ
				}
				else
				{
					Modbus_CmdState_Flag=0x10;//��λ���������λ
				}
				break;
	
				default:
				return -1;//�������
		}

		//ͨ�ò���
		trans_id=distrub_freeTID( );//����TID������֡
		mb_send_buffer[0]= (trans_id>>8)&0xff;//�������ʶ��H
		mb_send_buffer[1]=trans_id&0xff;//�������ʶ��L
		mb_send_buffer[2]=0X00;//Э���ʶ��H
		mb_send_buffer[3]=0X00;//Э���ʶ��L
		mb_send_buffer[4]=((data_index-6)>>8)&0xff;//����H
		mb_send_buffer[5]=(data_index-6)&0xff;//����L
		mb_send_buffer[6]=*(data);//��Ӵ�վ��ַ

		//��Ҫ���͵����ݸ��Ƶ�Ӧ�ó��򻺳������趨����
		memset(tcp_server_databuf,0, 200);//��ʼ�����ͻ�����
		byte_copy(tcp_server_databuf ,mb_send_buffer,data_index);//��������
		tcp_server_data_len=data_index;//�趨���ݳ���
	}
	return 0;
}


//��վ����վ��Ӧ
//�ж������ʶ���Ƿ���֮ǰ�����һ��
void master_checkACK(  )
{
	/*****************************************/
//	trans_id=(*(data)*256)+(*(data+1));
//	//�Ƿ�Ϊ�ͻ��˷���������ʶ��
//	if(!check_isdistrubed_TID(trans_id ))
//	{
//		//�����յ�����ӦTID
//		free_distrubedTID(trans_id);
//	}
//	else	return;//ֱ�Ӷ�������Ӧ
///*****************************************/
	
}


//�������ܣ�modbus��Ϊ��վ��Ӧ��վ
//����������
//��������ֵ����
extern void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
unsigned char iAddr;
unsigned long int slave_modbus_response( unsigned char *data)
{
	char data_index=0,temp,i;//�����ַ����
	unsigned int tLen;
//	unsigned int trans_id;//CRCУ������ʱֵ
	unsigned char Modbus_CmdState_Flag=0x00;//�����־������ָʾ��Ӧ״̬
	unsigned int reg_addr,reg_num;//�Ĵ�����ʼ��ַ���Ĵ�������
	unsigned int rec_temp;//��ʱ����
	unsigned char mb_send_buffer[500];//����������ʱ�������
	unsigned long int return_value=0;
	//ͨ�ò���
	data_index=6;
	temp=*(data+data_index+1);	
	//�жϹ������Ƿ���Ч
	if(( temp==modbus_cmd_Radio)|(temp==modbus_cmd_ReadKeepReg)|(temp==modbus_cmd_WriteOneReg)\
												 |(temp==modbus_cmd_WriteMultiReg)|(temp==modbus_cmd_ReadWriteReg) )
	{
		mb_send_buffer[data_index]=*(data+data_index);//�����վ��ַ
		iAddr=mb_send_buffer[data_index];
		if(iAddr<1) return 0xFF;
		switch( temp)
		{
			//�㲥��ַ
			case modbus_cmd_Radio:
			break;
			case modbus_cmd_WriteOneReg:
				reg_addr=(data[data_index+2]*256)+(data[data_index+3]);//�Ĵ�����ʼ��ַ
				reg_num=((data[data_index+4]*256)+(data[data_index+5]));//�Ĵ�������		
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//��书����
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//��书����
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//��书����
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//��书����
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//��书����
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//��书����
			  if(reg_addr==31)
				{
					if((reg_num&0x00FF)==0x0055)//DO0
					{
						GPIO_SetBits(GPIOC, GPIO_Pin_9);mbDat[31]=0x0055;		
					}
					else
					{
						GPIO_ResetBits(GPIOC, GPIO_Pin_9);mbDat[31]=0x0000;
						LcdDrawRectangle(204,208,312,274,0x001F,1);
						LcdDrawRectangle(204,308,312,344,0x001F,1);

					}
//					if((reg_num&0xFF00)==0x5500)//DO1
//					{
//						
//					}
				}
			break;
			//д����Ĵ���
			case modbus_cmd_WriteMultiReg:
			//�жϵ�ַ��Χ
				reg_addr=(data[data_index+2]*256)+(data[data_index+3]);//�Ĵ�����ʼ��ַ
				reg_num=((data[data_index+4]*256)+(data[data_index+5]));//�Ĵ�������		
			if((REG_ADDR_MIN<=reg_addr)&&( (reg_addr+reg_num)<=REG_ADDR_MAX*2 ))
				{
					return_value=reg_addr;
					return_value=(return_value<<16)|(reg_num/2);
					for( i=0;i<reg_num<<1;i++)
					{
						//LocalData_Buf[reg_addr*2+i]=data[data_index+7+i];
						//mbDat[reg_addr*2+i]=data[data_index+7+i];
						MB[iAddr-1][reg_addr*2+i]=data[data_index+7+i];
					}
					if((reg_addr==0)&&(reg_num==3))
					{
						RTCSetTime(MB[iAddr-1][3],MB[iAddr-1][4],MB[iAddr-1][5]);					
						RTCSetDate(MB[iAddr-1][0],MB[iAddr-1][1],MB[iAddr-1][2],1);					
					}
				}
				else
				{
					Modbus_CmdState_Flag=0x10;//��λ���������λ
				}
				//׼����Ӧ����
				data_index++;															//����ʹ�������Լ�
				mb_send_buffer[data_index]=(*(data+data_index))|Modbus_CmdState_Flag;//���1���ֽڹ�����
				data_index++;
				mb_send_buffer[data_index]=*(data+data_index);//��ӼĴ�����ַ��8λ
				data_index++;		
				mb_send_buffer[data_index]=*(data+data_index);	//��ӼĴ�����ַ��8λ
				data_index++;	
				mb_send_buffer[data_index]=*(data+data_index);//������ݵ�ַ��8λ
				data_index++;		
				mb_send_buffer[data_index]=*(data+data_index);	 //������ݵ�ַ��8λ
				data_index++;
				break;
				
				//�����ּĴ���
			case modbus_cmd_ReadKeepReg:
				reg_addr=(data[data_index+2]*256)+(data[data_index+3]);//�Ĵ�����ʼ��ַ
				reg_num=((data[data_index+4]*256)+(data[data_index+5]));//�Ĵ�������		
			  tLen=reg_num<<1;
				if((REG_ADDR_MIN<=reg_addr)&&( (reg_addr+reg_num)<=REG_ADDR_MAX ))//��ʼѰַ��Χ�����Ѱַ��֤
				{			
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//��书����
				data_index++;
				mb_send_buffer[data_index]=reg_num*2;;//��䷵���ֽ���-�Ĵ�������*2						
				//��䷵������
				for( i=0;i<reg_num;i++)
				{
					//mb_send_buffer[++data_index]=LocalData_Buf[reg_addr*2+i];
					mb_send_buffer[++data_index]=MB[iAddr-1][2*reg_addr+2*i];
					mb_send_buffer[++data_index]=MB[iAddr-1][2*reg_addr+2*i+1];
				}	
				data_index++;
				}
				else//������Ӧ
				{
					data_index=6;
					mb_send_buffer[data_index]=*(data+data_index);//���ش�����
					data_index++;
					Modbus_CmdState_Flag=0x10;//��λ���������λ
					mb_send_buffer[data_index]=*(data+data_index)|Modbus_CmdState_Flag;
					mb_send_buffer[++data_index]=0x01;//���ش�����
				}
				break;				
				default:
				break;//�������
		}

/*******************************************************************/		
/*******************************************************************/
		//��������
		mb_send_buffer[0]=tcp_server_databuf[0];//�������ʶ��H
		mb_send_buffer[1]=tcp_server_databuf[1];//�������ʶ��L
		mb_send_buffer[2]=0X00;//Э���ʶ��H
		mb_send_buffer[3]=0X00;//Э���ʶ��L
		mb_send_buffer[4]=((data_index-6)>>8)&0xff;//����H
		mb_send_buffer[5]=(data_index-6)&0xff;//����L

		//��Ҫ���͵����ݸ��Ƶ�Ӧ�ó��򻺳������趨����
		memset(tcp_server_databuf,0, 1024);//��ʼ�����ͻ�����-����Ҫע�⻺��ʵ�ʴ�С
		byte_copy(tcp_server_databuf ,mb_send_buffer,data_index);//��������
		tcp_server_data_len=data_index;//�趨���ݳ���	
/*******************************************************************/		
/*******************************************************************/
	}
return return_value;
}







