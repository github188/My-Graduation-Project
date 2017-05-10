
#ifndef __MODBUS_H__
#define __MODBUS_H__


#define		AS_MASTER_STATION		1//��Ϊ��վ
#define		AS_SLAVE_STATION		0//��Ϊ��վ

#define		modbus_master_add				1	//��վ��ַ
#define 	modbus_cmd_Radio				0x00//�㲥��ַ
#define 	modbus_cmd_ReadKeepReg	0x03//�����ּĴ���
#define		modbus_cmd_WriteOneReg	0x06	//д�����Ĵ���
#define		modbus_cmd_WriteMultiReg	0x10	//д����Ĵ���
#define		modbus_cmd_ReadWriteReg	0x17	//��д����Ĵ���

//������Ҫ���͵�������
#define		SEND_DATA_LEN				(100+2+1)//����:2*50��ADֵ(2·ģ��������ֵ)+1����������ֵ(us)<ռ��2���Ĵ���>+����������ֵ(2·)
//������Ҫ���յ�������
#define		RECV_DATA_LEN				(2+2+1)  //����:2·ģ�������ֵ+1·PWM���ֵ<ռ��2���Ĵ���>+���������ֵ(2·)

#define		DATABUF_MAXLEN		(SEND_DATA_LEN+RECV_DATA_LEN+1)//��ʵ����Ҫ�Ĵ�С+1<modbusЭ������>
#define 	REG_ADDR_MIN			0	//�Ĵ�����С��ַ
#define 	REG_ADDR_MAX			(REG_ADDR_MIN+DATABUF_MAXLEN-1) //�Ĵ�������ַ

//modbus����֡�ṹ��
typedef struct	
{
	unsigned char Slave_ID;//��վ��ַ
	unsigned char Func_Code;//������
	unsigned int Reg_BeginAddr;//�Ĵ�����ʼ��ַ
	unsigned int Reg_Num;//�Ĵ�������
	volatile unsigned int *Data_Buf;//��������
}mb_info_struct;



extern volatile mb_info_struct MB_Read_Ch1_Struct;
extern volatile mb_info_struct MB_Write_Ch1_Struct;

extern volatile unsigned int TcpServer_WriteDataBuf1[SEND_DATA_LEN];
//extern volatile unsigned int TcpServer_WriteDataBuf2[SEND_DATA_LEN];
extern volatile unsigned int TcpServer_ReadDataBuf1[RECV_DATA_LEN];
//extern volatile unsigned int TcpServer_ReadDataBuf2[RECV_DATA_LEN];


//���巢�ͻ��棬���256Byte
extern unsigned char mb_send_buffer[256];

unsigned int get_crc_verify( char data_length, unsigned char *data_buf);
int modbus_send_cmd( volatile mb_info_struct *info_struct);
int recv_modbus_response( unsigned char *data);





#endif

