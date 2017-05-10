

#include "stm32f4xx.h"
#include "modbus_tcp.h"
#include "delay.h"		////需要使用延时函数去掉注释即可
#include "float_hex.h"
#include "string.h"


//发送缓存
volatile unsigned int TcpServer_WriteDataBuf1[SEND_DATA_LEN];
//接收缓存
volatile unsigned int TcpServer_ReadDataBuf1[RECV_DATA_LEN];

//#endif

//函数功能：计算modbus_crc校验值
//函数参数：数据字节数、数据数组
//函数返回值：crc校验值-2Byte-0xHHLL
unsigned int get_crc_verify( char data_length, unsigned char *data_buf)
{
	unsigned int original_crc = 0xffff;
	unsigned char i,j;
	
	for (j = 0; j < data_length; j++)
	{
		i=0;
		original_crc ^= data_buf[j];
		do
		{
			if ((original_crc & 0x0001 )!= 0)
			{
				original_crc >>= 1;
				original_crc ^= 0xa001;
			}
			else
			{
				original_crc >>= 1;
			}
			i++;
		} while (i < 8);
	}
	
	return original_crc;
}


//函数功能：modbus发送数据给从站
//函数参数：
//函数返回值：0-正常；-1-错误
int modbus_send_cmd( volatile mb_info_struct *info_struct)
{
	char data_index=0,i;
	unsigned int reg_addr,reg_num;//寄存器起始地址、寄存器数量
	unsigned int trans_id;
	unsigned char Modbus_CmdState_Flag=0x00;//错误标志，用于指示响应状态
	
	//通用部分
	data_index=6;
	memset( mb_send_buffer,0,sizeof(mb_send_buffer));//清空缓存
	//判断功能码是否有效
	if((info_struct->Func_Code==modbus_cmd_Radio)||(info_struct->Func_Code==\
		modbus_cmd_ReadKeepReg)||(info_struct->Func_Code==modbus_cmd_WriteMultiReg))
	{
		mb_send_buffer[data_index]=info_struct->Slave_ID;//添加从站地址
		//根据功能码计算数据长度和数据格式（8bit/16bit）
		switch( info_struct->Func_Code)
		{
			//广播地址
			case modbus_cmd_Radio:
			break;
	
			//写多个寄存器
			case modbus_cmd_WriteMultiReg:
				//判断地址范围
			//判断地址范围
				reg_addr=info_struct->Reg_BeginAddr;//寄存器起始地址
			  reg_num=info_struct->Reg_Num;//寄存器数量		
		  	if(( reg_addr>=REG_ADDR_MIN)&&( (reg_addr+reg_num)<=REG_ADDR_MAX))
				{
					Modbus_CmdState_Flag=0x00;//置位功能码最高位
				}
				else
				{
					Modbus_CmdState_Flag=0x10;//置位功能码最高位
				}
				//准备需要发送的数据
				data_index++;															//避免使用自增自减
				mb_send_buffer[data_index]=info_struct->Func_Code|Modbus_CmdState_Flag;//添加1个字节功能码
				data_index++;
				mb_send_buffer[data_index]=(info_struct->Reg_BeginAddr>>8)&0x00ff;//添加寄存器地址高8位
				data_index++;		
				mb_send_buffer[data_index]=info_struct->Reg_BeginAddr&0x00ff;	//添加寄存器地址低8位
				data_index++;	
				mb_send_buffer[data_index]=(info_struct->Reg_Num>>8)&0x00ff;//添加寄存器数量高8位
				data_index++;		
				mb_send_buffer[data_index]=info_struct->Reg_Num&0x00ff;//添加寄存器数量低8位
				data_index++;	
				mb_send_buffer[data_index]=info_struct->Reg_Num*2;//添加字节数
				for( i=0;i<info_struct->Reg_Num;i++)
				{
					data_index++;		
					mb_send_buffer[data_index]=(info_struct->Data_Buf[i]>>8)&0x00ff;//添加寄存器内容高8位
					data_index++;		
					mb_send_buffer[data_index]=(info_struct->Data_Buf[i])&0x00ff;//添加寄存器内容低8位
				}			
				break;
				
			//读多个寄存器
			case modbus_cmd_ReadKeepReg:
				//判断地址范围
				reg_addr=info_struct->Reg_BeginAddr;//寄存器起始地址
			  reg_num=info_struct->Reg_Num;//寄存器数量		
		  	if(( reg_addr>=REG_ADDR_MIN)&&( (reg_addr+reg_num)<=REG_ADDR_MAX))
				{
					Modbus_CmdState_Flag=0x00;//置位功能码最高位
				}
				else
				{
					Modbus_CmdState_Flag=0x10;//置位功能码最高位
				}
				//准备需要发送的数据
				data_index++;															//避免使用自增自减
				mb_send_buffer[data_index]=info_struct->Func_Code|Modbus_CmdState_Flag;//添加1个字节功能码
				data_index++;
				mb_send_buffer[data_index]=(info_struct->Reg_BeginAddr>>8)&0x00ff;//添加寄存器地址高8位
				data_index++;		
				mb_send_buffer[data_index]=info_struct->Reg_BeginAddr&0x00ff;	//添加寄存器地址低8位
				data_index++;	
				mb_send_buffer[data_index]=(info_struct->Reg_Num>>8)&0x00ff;//添加寄存器数量高8位
				data_index++;		
				mb_send_buffer[data_index]=info_struct->Reg_Num&0x00ff;//添加寄存器数量低8位
				break;
				
			default:
				break;//错误代码
		}
		
		//通用部分
		trans_id=0xaabb;//分配TID给数据帧
		mb_send_buffer[0]= (trans_id>>8)&0xff;//事务处理标识符H
		mb_send_buffer[1]=trans_id&0xff;//事务处理标识符L
		mb_send_buffer[2]=0X00;//协议标识符H
		mb_send_buffer[3]=0X00;//协议标识符L
		mb_send_buffer[4]=((data_index-5)>>8)&0xff;//长度H
		mb_send_buffer[5]=(data_index-5)&0xff;//长度L
		mb_send_buffer[6]=info_struct->Slave_ID;//添加从站地址
		//发送响应数据
		USART_GetFlagStatus( USART1,USART_FLAG_TC);
		//一帧发送开始
		for( i=0;i<data_index+1;i++)
		{
			USART_SendData( USART1,mb_send_buffer[i]);
			while( USART_GetFlagStatus( USART1,USART_FLAG_TC)!=SET);//等待发送完毕
		}
		//一帧发送结束
	}
	return 0;
}


unsigned char mb_send_buffer[256];//定义发送缓存，最大256Byte

/*
**由于此时设备作为客户端，所以响应机制和作为服务器端不同
**设备发出读寄存器指令后，接收到服务器的响应后将数据解包
*/
//函数功能：modbus响应主站
//函数参数：
//函数返回值：0:正常；其他:错误
int recv_modbus_response( unsigned char *data)
{
	char data_index=0,i;
	unsigned int reg_num;
	
	//
	memset((unsigned int*)TcpServer_ReadDataBuf1,0,sizeof(TcpServer_ReadDataBuf1));
	//
	data_index=7;
	//判断功能码是否有效
	if( (*(data+data_index)==modbus_cmd_Radio)||(*(data+data_index)==modbus_cmd_ReadKeepReg))
	{
		//根据功能码计算数据长度和数据格式（8bit/16bit）
		switch( data[data_index])
		{
			//广播地址
			case modbus_cmd_Radio:
			break;
		  
			//读多个寄存器-hex
			case modbus_cmd_ReadKeepReg:
			//计算寄存器数量
			  reg_num=(*(data+8))>>1;
				for( i=1;i<=reg_num;i++)
				{							
					TcpServer_ReadDataBuf1[i-1]=*(data+9+(i-1)*2);//高位数据写入
					TcpServer_ReadDataBuf1[i-1]<<=8;//高位左移8位
					TcpServer_ReadDataBuf1[i-1]|=*(data+10+(i-1)*2);//低位数据写入
				}
				break;
	
				default:
					break;//错误代码
		}
	}
	return 0;
}



