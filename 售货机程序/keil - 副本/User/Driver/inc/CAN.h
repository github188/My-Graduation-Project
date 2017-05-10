#ifndef __CAN_H
#define __CAN_H 

#include "stm32f4xx.h"
#include "iCAN.h"

#define CAN_ID_STANDARD		0
#define CAN_ID_EXTENDED		1

//#define CAN_RTR_DATA		0
//#define CAN_RTR_REMOTE		1

typedef struct  {
  unsigned int   id;                 // 29 bit identifier
  unsigned char  data[8];            // Data field
  unsigned char  len;                // Length of data field in bytes
  unsigned char  format;             // 0 - STANDARD, 1- EXTENDED IDENTIFIER
  unsigned char  type;               // 0 - DATA FRAME, 1 - REMOTE FRAME
} CAN_msg;
typedef struct {
	char disWD[12];//123.45
	char disMD[12];//1.2345
	char disSSL[12];//123.456
	char disLJS[12];//1234567.890;
	char disYFL[12];//1234567.890
	char disSFL[12];//1234567.890
	char disSTA[2];	
	unsigned char  NewDat;
	unsigned char  cntConn;
}PKQ_DIS;

typedef struct{
	float rtWD;
	float rtMD;
	float rtSSL;
	float rtLJS;
	float rtYFL;
	float rtSFL;
	float rtZero;
	
}PKQ_DAT;
extern PKQ_DIS  pkqDisUnit[14];//14×éÅú¿ØÆ÷
extern unsigned int  CAN_TxRdy;                      // CAN HW ready to transmit a message
extern unsigned int  CAN_RxRdy;                      // CAN HW received a message

void CANHardwareConfig(void);
void SendCAN(void);
void iCAN_rdMsg (iCAN_msg *msg);
void FillFBCan(unsigned char id,unsigned char FbId);

#endif

