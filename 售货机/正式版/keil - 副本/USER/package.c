
#include "string.h"
#include "package.h"
#include "float_hex.h"
#include <stdint.h> 
#include "stm32f4xx_gpio.h"
#include "Global.h"


//server 
extern uint8_t tcp_server_databuf[200];   	//发送数据缓存	
extern uint8_t tcp_server_data_len;		//发送缓存长度
extern unsigned int mbDat[200];
//8位寄存器-用于模拟
#define		MAX_DATALEN		200
volatile unsigned char LocalData_Buf[MAX_DATALEN];//模拟数据缓存
/*
#define		MAX_TDATALEN		200
volatile unsigned char ReadData_Buf[MAX_TDATALEN];
#define		MAX_RDATALEN		200
volatile unsigned char WriteData_Buf[MAX_RDATALEN];
*/


//当发送一帧modbus消息时，分配一个标识符给对应帧，并置1，收到响应时，再将该标识符销毁
//通过遍历，未被置位的元素，得到空闲标识符
#define		TID_MAX		20
unsigned char TID[TID_MAX];//事务处理标识符

//分配空闲事务标识符
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
		//事务标识符不够用
		//之前发送出去的帧都没有响应
		//启动应急处理
	}
return NULL;
}

//销毁收到响应的事务标识符
void free_distrubedTID(unsigned char t)
{
	TID[t]=0;
}
//判断返回的是否为客户端分配的事务标识符
//0-正确响应
//1-无效响应
unsigned char check_isdistrubed_TID( unsigned char t)
{
	if( TID[t]==0)
		return 1;
return 0;
}

//拷贝指定大小字节从原数组到目的数组
void byte_copy(unsigned char  *des,unsigned char *src,unsigned short size)
{		
	unsigned char i;
	for( i=0;i<size;i++)
	{
		des[i]=src[i];
	}
}
//拷贝指定大小字节从原数组到目的数组
void u16_copy(unsigned short  *des,unsigned short *src,unsigned short size)
{		
	unsigned short i;
	for( i=0;i<size;i++)
	{
		des[i]=src[i];
	}
}

//函数功能：modbus发送数据给从站
//函数参数：
//函数返回值：0-正常；-1-错误
int modbus_send( unsigned char *data)
{
	unsigned char data_index=0,temp,i;
	unsigned int trans_id;
	unsigned char Modbus_CmdState_Flag=0x00;//错误标志，用于指示响应状态
	unsigned char mb_send_buffer[200];//返回数据临时存放数组
	unsigned int reg_addr,reg_num;//寄存器起始地址、寄存器数量
	
	//通用部分
	data_index=7;
	temp=*(data+data_index);
	//判断功能码是否有效
	if((temp==modbus_cmd_Radio)|(temp==modbus_cmd_ReadKeepReg)|(temp==modbus_cmd_WriteOneReg))//(temp==modbus_cmd_Radio)|(temp==modbus_cmd_ReadKeepReg)|(temp==modbus_cmd_WriteOneReg)
	{
		//根据功能码计算数据长度和数据格式（8bit/16bit）
		switch( temp)
		{
			//广播地址
			case modbus_cmd_Radio:
			break;
			
			//写单个线圈
			case modbus_cmd_WriteOneReg:
			//判断地址范围
				reg_addr=*(data+2)*256+(*(data+3));
				if((REG_ADDR_MIN<=reg_addr)&&(reg_addr<=REG_ADDR_MAX))//REG_ADDR_MIN<=reg_addr<=REG_ADDR_MAX
				{
					mb_send_buffer[data_index++]=temp;//功能码
					mb_send_buffer[data_index++]=*( data+2);//寄存器地址高8位
					mb_send_buffer[data_index++]=*( data+3);//寄存器地址低8位			
					mb_send_buffer[data_index++]=*( data+4);//data high 8 bit
					mb_send_buffer[data_index++]=*( data+5);//data low 8bit
					Modbus_CmdState_Flag=0x00;//不置位功能码最高位
				}
				else
				{
					Modbus_CmdState_Flag=0x10;//置位功能码最高位
				}
				break;
	
				default:
				return -1;//错误代码
		}

		//通用部分
		trans_id=distrub_freeTID( );//分配TID给数据帧
		mb_send_buffer[0]= (trans_id>>8)&0xff;//事务处理标识符H
		mb_send_buffer[1]=trans_id&0xff;//事务处理标识符L
		mb_send_buffer[2]=0X00;//协议标识符H
		mb_send_buffer[3]=0X00;//协议标识符L
		mb_send_buffer[4]=((data_index-6)>>8)&0xff;//长度H
		mb_send_buffer[5]=(data_index-6)&0xff;//长度L
		mb_send_buffer[6]=*(data);//添加从站地址

		//将要发送的数据复制到应用程序缓冲区并设定长度
		memset(tcp_server_databuf,0, 200);//初始化发送缓冲区
		byte_copy(tcp_server_databuf ,mb_send_buffer,data_index);//复制数据
		tcp_server_data_len=data_index;//设定数据长度
	}
	return 0;
}


//主站检查从站响应
//判断事务标识符是否与之前分配的一致
void master_checkACK(  )
{
	/*****************************************/
//	trans_id=(*(data)*256)+(*(data+1));
//	//是否为客户端分配的事务标识符
//	if(!check_isdistrubed_TID(trans_id ))
//	{
//		//销毁收到的响应TID
//		free_distrubedTID(trans_id);
//	}
//	else	return;//直接丢弃该响应
///*****************************************/
	
}


//函数功能：modbus作为从站响应主站
//函数参数：
//函数返回值：无
extern void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
unsigned char iAddr;
unsigned long int slave_modbus_response( unsigned char *data)
{
	char data_index=0,temp,i;//数组地址索引
	unsigned int tLen;
//	unsigned int trans_id;//CRC校验结果临时值
	unsigned char Modbus_CmdState_Flag=0x00;//错误标志，用于指示响应状态
	unsigned int reg_addr,reg_num;//寄存器起始地址、寄存器数量
	unsigned int rec_temp;//临时变量
	unsigned char mb_send_buffer[500];//返回数据临时存放数组
	unsigned long int return_value=0;
	//通用部分
	data_index=6;
	temp=*(data+data_index+1);	
	//判断功能码是否有效
	if(( temp==modbus_cmd_Radio)|(temp==modbus_cmd_ReadKeepReg)|(temp==modbus_cmd_WriteOneReg)\
												 |(temp==modbus_cmd_WriteMultiReg)|(temp==modbus_cmd_ReadWriteReg) )
	{
		mb_send_buffer[data_index]=*(data+data_index);//添加主站地址
		iAddr=mb_send_buffer[data_index];
		if(iAddr<1) return 0xFF;
		switch( temp)
		{
			//广播地址
			case modbus_cmd_Radio:
			break;
			case modbus_cmd_WriteOneReg:
				reg_addr=(data[data_index+2]*256)+(data[data_index+3]);//寄存器起始地址
				reg_num=((data[data_index+4]*256)+(data[data_index+5]));//寄存器数量		
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//填充功能码
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//填充功能码
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//填充功能码
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//填充功能码
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//填充功能码
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//填充功能码
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
			//写多个寄存器
			case modbus_cmd_WriteMultiReg:
			//判断地址范围
				reg_addr=(data[data_index+2]*256)+(data[data_index+3]);//寄存器起始地址
				reg_num=((data[data_index+4]*256)+(data[data_index+5]));//寄存器数量		
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
					Modbus_CmdState_Flag=0x10;//置位功能码最高位
				}
				//准备响应数据
				data_index++;															//避免使用自增自减
				mb_send_buffer[data_index]=(*(data+data_index))|Modbus_CmdState_Flag;//添加1个字节功能码
				data_index++;
				mb_send_buffer[data_index]=*(data+data_index);//添加寄存器地址高8位
				data_index++;		
				mb_send_buffer[data_index]=*(data+data_index);	//添加寄存器地址低8位
				data_index++;	
				mb_send_buffer[data_index]=*(data+data_index);//添加数据地址高8位
				data_index++;		
				mb_send_buffer[data_index]=*(data+data_index);	 //添加数据地址低8位
				data_index++;
				break;
				
				//读保持寄存器
			case modbus_cmd_ReadKeepReg:
				reg_addr=(data[data_index+2]*256)+(data[data_index+3]);//寄存器起始地址
				reg_num=((data[data_index+4]*256)+(data[data_index+5]));//寄存器数量		
			  tLen=reg_num<<1;
				if((REG_ADDR_MIN<=reg_addr)&&( (reg_addr+reg_num)<=REG_ADDR_MAX ))//起始寻址范围和最大寻址验证
				{			
				data_index++;
				mb_send_buffer[data_index]=data[data_index];//填充功能码
				data_index++;
				mb_send_buffer[data_index]=reg_num*2;;//填充返回字节数-寄存器数量*2						
				//填充返回数据
				for( i=0;i<reg_num;i++)
				{
					//mb_send_buffer[++data_index]=LocalData_Buf[reg_addr*2+i];
					mb_send_buffer[++data_index]=MB[iAddr-1][2*reg_addr+2*i];
					mb_send_buffer[++data_index]=MB[iAddr-1][2*reg_addr+2*i+1];
				}	
				data_index++;
				}
				else//错误响应
				{
					data_index=6;
					mb_send_buffer[data_index]=*(data+data_index);//返回错误码
					data_index++;
					Modbus_CmdState_Flag=0x10;//置位功能码最高位
					mb_send_buffer[data_index]=*(data+data_index)|Modbus_CmdState_Flag;
					mb_send_buffer[++data_index]=0x01;//返回错误码
				}
				break;				
				default:
				break;//错误代码
		}

/*******************************************************************/		
/*******************************************************************/
		//公共部分
		mb_send_buffer[0]=tcp_server_databuf[0];//事务处理标识符H
		mb_send_buffer[1]=tcp_server_databuf[1];//事务处理标识符L
		mb_send_buffer[2]=0X00;//协议标识符H
		mb_send_buffer[3]=0X00;//协议标识符L
		mb_send_buffer[4]=((data_index-6)>>8)&0xff;//长度H
		mb_send_buffer[5]=(data_index-6)&0xff;//长度L

		//将要发送的数据复制到应用程序缓冲区并设定长度
		memset(tcp_server_databuf,0, 1024);//初始化发送缓冲区-这里要注意缓存实际大小
		byte_copy(tcp_server_databuf ,mb_send_buffer,data_index);//复制数据
		tcp_server_data_len=data_index;//设定数据长度	
/*******************************************************************/		
/*******************************************************************/
	}
return return_value;
}







