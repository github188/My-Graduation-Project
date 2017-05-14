#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    


/**********CAN_CH1**********/
#define		CAN_CH1_DEVICE				CAN1
#define		CAN_CH1_DEVICE_RX			GPIO_Pin_8
#define		CAN_CH1_DEVICE_TX			GPIO_Pin_9
#define		CAN_CH1_DEVICE_RCC		RCC_APB1Periph_CAN1
#define		CAN_CH1_PORT_RCC			RCC_AHB1Periph_GPIOB
#define		CAN_CH1_DEVICE_PORT		GPIOB
#define		CAN_CH1_DEVICE_AF			GPIO_AF_CAN1
#define		CAN_CH1_DEVICE_RX_PS	GPIO_PinSource9
#define		CAN_CH1_DEVICE_TX_PS	GPIO_PinSource8
#define		CAN_CH1_IRQn					CAN1_RX0_IRQn
/**********************/

/**********CAN_CH2**********/
#define		CAN_CH2_DEVICE				CAN2
#define		CAN_CH2_DEVICE_RX			GPIO_Pin_5
#define		CAN_CH2_DEVICE_TX			GPIO_Pin_6
#define		CAN_CH2_DEVICE_RCC		RCC_APB1Periph_CAN2
#define		CAN_CH1_PORT_RCC			RCC_AHB1Periph_GPIOB
#define		CAN_CH2_DEVICE_PORT		GPIOB
#define		CAN_CH2_DEVICE_AF			GPIO_AF_CAN2
#define		CAN_CH2_DEVICE_RX_PS	GPIO_PinSource5
#define		CAN_CH2_DEVICE_TX_PS	GPIO_PinSource6
#define		CAN_CH2_IRQn					CAN2_RX0_IRQn
/**********************/


typedef struct  {
  unsigned char   SrcMacId;           // 29 bit identifier
	unsigned char   DestMacId;
	unsigned char   ACK;
	unsigned char   FuncID;
	unsigned char   SourceID;
  unsigned char   data[8];            // Data field
  unsigned char   len;                // Length of data field in bytes
  //unsigned char   format;             // 0 - STANDARD, 1- EXTENDED IDENTIFIER
  //unsigned char  type;               // 0 - DATA FRAME, 1 - REMOTE FRAME
} iCAN_msg;











void CAN_waitReady (void)  ;


//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
void CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 CAN1_Send_Msg(u8 addrDest);						//发送数据
u8 CAN1_Receive_Msg(u8 *buf);							//接收数据

	
//CAN2接收RX0中断使能
#define CAN2_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
void CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 CAN2_Send_Msg(u8* msg,u8 len);						//发送数据
u8 CAN2_Receive_Msg(u8 *buf);							//接收数据

extern iCAN_msg	GKY_TxMsg;




#endif

















