

#include "stm32f4xx.h"
#include "modbus.h"
#include "delay.h"		////��Ҫʹ����ʱ����ȥ��ע�ͼ���
#include "float_hex.h"
#include "string.h"


//���ͻ���
volatile unsigned int TcpServer_WriteDataBuf1[SEND_DATA_LEN];
//volatile unsigned int TcpServer_WriteDataBuf2[DATABUF_MAXLEN];
//���ջ���
volatile unsigned int TcpServer_ReadDataBuf1[RECV_DATA_LEN];
//volatile unsigned int TcpServer_ReadDataBuf2[DATABUF_MAXLEN];

//#endif

//�������ܣ�����modbus_crcУ��ֵ
//���������������ֽ�������������
//��������ֵ��crcУ��ֵ-2Byte-0xHHLL
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


//�������ܣ�modbus�������ݸ���վ
//����������
//��������ֵ��0-������-1-����
int modbus_send_cmd( volatile mb_info_struct *info_struct)
{
	char data_index=0,i;
	unsigned int reg_addr,reg_num;//�Ĵ�����ʼ��ַ���Ĵ�������
	unsigned int crc_result;
	unsigned char Modbus_CmdState_Flag=0x00;//�����־������ָʾ��Ӧ״̬
	
	memset( mb_send_buffer,0,sizeof(mb_send_buffer));//��ջ���
	//�жϹ������Ƿ���Ч
	if( (info_struct->Func_Code==modbus_cmd_Radio)|(info_struct->Func_Code==modbus_cmd_ReadKeepReg)|(info_struct->Func_Code==modbus_cmd_WriteMultiReg))
	{
		mb_send_buffer[data_index]=info_struct->Slave_ID;//��Ӵ�վ��ַ
		//���ݹ�����������ݳ��Ⱥ����ݸ�ʽ��8bit/16bit��
		switch( info_struct->Func_Code)
		{
			//�㲥��ַ
			case modbus_cmd_Radio:
			break;
	
			//д����Ĵ���
			case modbus_cmd_WriteMultiReg:
				//�жϵ�ַ��Χ
			//�жϵ�ַ��Χ
				reg_addr=info_struct->Reg_BeginAddr;//�Ĵ�����ʼ��ַ
			  reg_num=info_struct->Reg_Num;//�Ĵ�������		
		  	if(( reg_addr>=REG_ADDR_MIN)&&( (reg_addr+reg_num)<=REG_ADDR_MAX))
				{
					Modbus_CmdState_Flag=0x00;//��λ���������λ
				}
				else
				{
					Modbus_CmdState_Flag=0x10;//��λ���������λ
				}
				//׼����Ҫ���͵�����
				data_index++;															//����ʹ�������Լ�
				mb_send_buffer[data_index]=info_struct->Func_Code|Modbus_CmdState_Flag;//���1���ֽڹ�����
				data_index++;
				mb_send_buffer[data_index]=(info_struct->Reg_BeginAddr>>8)&0x00ff;//��ӼĴ�����ַ��8λ
				data_index++;		
				mb_send_buffer[data_index]=info_struct->Reg_BeginAddr&0x00ff;	//��ӼĴ�����ַ��8λ
				data_index++;	
				mb_send_buffer[data_index]=(info_struct->Reg_Num>>8)&0x00ff;//��ӼĴ���������8λ
				data_index++;		
				mb_send_buffer[data_index]=info_struct->Reg_Num&0x00ff;//��ӼĴ���������8λ
				data_index++;	
				mb_send_buffer[data_index]=info_struct->Reg_Num*2;//����ֽ���
				for( i=0;i<info_struct->Reg_Num;i++)
				{
					data_index++;		
					mb_send_buffer[data_index]=(info_struct->Data_Buf[i]>>8)&0x00ff;//��ӼĴ������ݸ�8λ
					data_index++;		
					mb_send_buffer[data_index]=(info_struct->Data_Buf[i])&0x00ff;//��ӼĴ������ݵ�8λ
				}
				//���㲢����CRC
				crc_result=get_crc_verify( data_index+1,mb_send_buffer);
				data_index++;		
				mb_send_buffer[data_index]=crc_result&0xff;	 //���CRC��8λ
				data_index++;		
				mb_send_buffer[data_index]=(crc_result>>8)&0xff;	 //���CRC��8λ
				break;
				
			//������Ĵ���
			case modbus_cmd_ReadKeepReg:
				//�жϵ�ַ��Χ
				reg_addr=info_struct->Reg_BeginAddr;//�Ĵ�����ʼ��ַ
			  reg_num=info_struct->Reg_Num;//�Ĵ�������		
		  	if(( reg_addr>=REG_ADDR_MIN)&&( (reg_addr+reg_num)<=REG_ADDR_MAX))
				{
					Modbus_CmdState_Flag=0x00;//��λ���������λ
				}
				else
				{
					Modbus_CmdState_Flag=0x10;//��λ���������λ
				}
				//׼����Ҫ���͵�����
				data_index++;															//����ʹ�������Լ�
				mb_send_buffer[data_index]=info_struct->Func_Code|Modbus_CmdState_Flag;//���1���ֽڹ�����
				data_index++;
				mb_send_buffer[data_index]=(info_struct->Reg_BeginAddr>>8)&0x00ff;//��ӼĴ�����ַ��8λ
				data_index++;		
				mb_send_buffer[data_index]=info_struct->Reg_BeginAddr&0x00ff;	//��ӼĴ�����ַ��8λ
				data_index++;	
				mb_send_buffer[data_index]=(info_struct->Reg_Num>>8)&0x00ff;//��ӼĴ���������8λ
				data_index++;		
				mb_send_buffer[data_index]=info_struct->Reg_Num&0x00ff;//��ӼĴ���������8λ
				//���㲢����CRC
				crc_result=get_crc_verify( data_index+1,mb_send_buffer);
				data_index++;		
				mb_send_buffer[data_index]=crc_result&0xff;	 //���CRC��8λ
				data_index++;		
				mb_send_buffer[data_index]=(crc_result>>8)&0xff;	 //���CRC��8λ
				break;
				
			default:
				break;//�������
		}
		
		//������Ӧ����
		USART_GetFlagStatus( USART1,USART_FLAG_TC);
		//һ֡���Ϳ�ʼ
//		delay_ms( 5);//֡��ʼ�ӳ�ʱ��t_3_5
		TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE); //����ʱ��3�����ж�
		for( i=0;i<data_index+1;i++)
		{
			USART_SendData( USART1,mb_send_buffer[i]);
			while( USART_GetFlagStatus( USART1,USART_FLAG_TC)!=SET);//�ȴ��������			
		}
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//		delay_ms( 5);//֡�����ӳ�ʱ��t_3_5
		//һ֡���ͽ���
	}
	return 0;
}


unsigned char mb_send_buffer[256];//���巢�ͻ��棬���256Byte

/*
**���ڴ�ʱ�豸��Ϊ�ͻ��ˣ�������Ӧ���ƺ���Ϊ�������˲�ͬ
**�豸�������Ĵ���ָ��󣬽��յ�����������Ӧ�����ݽ��
*/
//�������ܣ�modbus��Ӧ��վ
//����������
//��������ֵ��0:����������:����
int recv_modbus_response( unsigned char *data)
{
	char data_index=0,i;
	unsigned int reg_addr,reg_num;
	
	//�жϹ������Ƿ���Ч
	if( (*(data+1)==modbus_cmd_Radio)||(*(data+1)==modbus_cmd_ReadKeepReg))
	{
		mb_send_buffer[data_index]=*(data+data_index);//�����վ��ַ
		//���ݹ�����������ݳ��Ⱥ����ݸ�ʽ��8bit/16bit��
		switch( data[1])
		{
			//�㲥��ַ
			case modbus_cmd_Radio:
			break;
		  
			//������Ĵ���-hex
			case modbus_cmd_ReadKeepReg:
			//����Ĵ�������
			  reg_num=(*(data+2))>>1;
				for( i=1;i<=reg_num;i++)
				{							
					TcpServer_ReadDataBuf1[i-1]=*(data+3+(i-1)*2);//��λ����д��
					TcpServer_ReadDataBuf1[i-1]<<=8;//��λ����8λ
					TcpServer_ReadDataBuf1[i-1]|=*(data+4+(i-1)*2);//��λ����д��
				}
				break;
	
				default:
					break;//�������
		}
	}
	return 0;
}



