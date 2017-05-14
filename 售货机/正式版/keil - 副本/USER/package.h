#ifndef __PACKAGE__H_
#define __PACKAGE__H_


#define		AS_MASTER_STATION		1//��Ϊ��վ
#define		AS_SLAVE_STATION		0//��Ϊ��վ

#define		modbus_master_add				1	//��վ��ַ
#define 	modbus_cmd_Radio				0x00//�㲥��ַ
#define 	modbus_cmd_ReadKeepReg	0x03//�����ּĴ���
#define		modbus_cmd_WriteOneReg	0x06	//д�����Ĵ���
#define		modbus_cmd_WriteMultiReg	0x10	//д����Ĵ���
#define		modbus_cmd_ReadWriteReg	0x17	//��д����Ĵ���



//#define		PLC_ADDRESS_BASE1
//#define		PROTOCOL_ADDRESS_BASE0
//�Ĵ�����ַBASE_0 OR BASE_1


//#define		DATA_TYPE_FLOAT


//#ifdef		PLC_ADDRESS_BASE1		//BASE 1
//#define 	REG_ADDR_MIN			1	//�Ĵ�����С��ַ
//#define 	REG_ADDR_MAX			9 //�Ĵ�������ַ
//#define		BUFFER_OFFSET			2	//�Ĵ�����ַƫ��
//#else									  	 		//BASE 0

#define		DATABUF_MAXLEN		104//��ʵ����Ҫ�Ĵ�С+1<modbusЭ������>
#define 	REG_ADDR_MIN			0	//�Ĵ�����С��ַ
#define 	REG_ADDR_MAX			(REG_ADDR_MIN+DATABUF_MAXLEN-1) //�Ĵ�������ַ

//#define		BUFFER_OFFSET			1 //�Ĵ�����ַƫ��
//#endif


extern volatile unsigned short int Register_Buffer[DATABUF_MAXLEN];
extern volatile unsigned short int Register_Buffer_Channel1[DATABUF_MAXLEN];
extern volatile unsigned short int Register_Buffer_Channel2[DATABUF_MAXLEN];
extern volatile unsigned short int Register_Buffer_Channel3[DATABUF_MAXLEN];
extern float Register_BufferFloat[10];




int modbus_send( unsigned char *data);
unsigned long int slave_modbus_response( unsigned char *data);

#endif


