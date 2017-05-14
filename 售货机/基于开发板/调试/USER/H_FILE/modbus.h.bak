
#ifndef __MODBUS_H__
#define __MODBUS_H__


#define		AS_MASTER_STATION		1//作为主站
#define		AS_SLAVE_STATION		0//作为从站

#define		modbus_master_add				1	//主站地址
#define 	modbus_cmd_Radio				0x00//广播地址
#define 	modbus_cmd_ReadKeepReg	0x03//读保持寄存器
#define		modbus_cmd_WriteOneReg	0x06	//写单个寄存器
#define		modbus_cmd_WriteMultiReg	0x10	//写多个寄存器
#define		modbus_cmd_ReadWriteReg	0x17	//读写多个寄存器

//定义需要发送的数据量
#define		SEND_DATA_LEN				(100+2+1)//发送:2*50个AD值(2路模拟量输入值)+1个脉冲周期值(us)<占用2个寄存器>+开关量输入值(2路)
//定义需要接收的数据量
#define		RECV_DATA_LEN				(2+2+1)  //接收:2路模拟量输出值+1路PWM输出值<占用2个寄存器>+开关量输出值(2路)

#define		DATABUF_MAXLEN		(SEND_DATA_LEN+RECV_DATA_LEN+1)//比实际需要的大小+1<modbus协议问题>
#define 	REG_ADDR_MIN			0	//寄存器最小地址
#define 	REG_ADDR_MAX			(REG_ADDR_MIN+DATABUF_MAXLEN-1) //寄存器最大地址

//modbus数据帧结构体
typedef struct	
{
	unsigned char Slave_ID;//从站地址
	unsigned char Func_Code;//功能码
	unsigned int Reg_BeginAddr;//寄存器起始地址
	unsigned int Reg_Num;//寄存器长度
	volatile unsigned int *Data_Buf;//数据数组
}mb_info_struct;



extern volatile mb_info_struct MB_Read_Ch1_Struct;
extern volatile mb_info_struct MB_Write_Ch1_Struct;

extern volatile unsigned int TcpServer_WriteDataBuf1[SEND_DATA_LEN];
//extern volatile unsigned int TcpServer_WriteDataBuf2[SEND_DATA_LEN];
extern volatile unsigned int TcpServer_ReadDataBuf1[RECV_DATA_LEN];
//extern volatile unsigned int TcpServer_ReadDataBuf2[RECV_DATA_LEN];


//定义发送缓存，最大256Byte
extern unsigned char mb_send_buffer[256];

unsigned int get_crc_verify( char data_length, unsigned char *data_buf);
int modbus_send_cmd( volatile mb_info_struct *info_struct);
int recv_modbus_response( unsigned char *data);





#endif

