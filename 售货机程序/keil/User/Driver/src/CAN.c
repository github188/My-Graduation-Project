#include "CAN.h"
#include "Global.h"
//CANET100 190032
//GC-CAN
//CAN2_RX:PB5  CAN2_TX:PB6
#define CAN2_TX_CLK 			  RCC_AHB1Periph_GPIOB
#define CAN2_TX_PIN				  GPIO_Pin_6
#define CAN2_TX_PINSOURCE		GPIO_PinSource6
#define CAN2_TX_GPIO			  GPIOB

#define CAN2_RX_CLK				  RCC_AHB1Periph_GPIOB
#define CAN2_RX_PIN				  GPIO_Pin_5
#define CAN2_RX_PINSOURCE		GPIO_PinSource5
#define CAN2_RX_GPIO			GPIOB	


//CAN1_RX:PB8  CAN1_TX:PB8
#define CAN1_TX_CLK 			RCC_AHB1Periph_GPIOB
#define CAN1_TX_PIN				GPIO_Pin_9
#define CAN1_TX_PINSOURCE		GPIO_PinSource9
#define CAN1_TX_GPIO			GPIOB

#define CAN1_RX_CLK				RCC_AHB1Periph_GPIOB
#define CAN1_RX_PIN				GPIO_Pin_8
#define CAN1_RX_PINSOURCE		GPIO_PinSource8
#define CAN1_RX_GPIO			GPIOB

extern void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);

unsigned char RxMsgCnt;
CanRxMsg RxMsgDat[50];
CanRxMsg RxMsgBuf[50];
CanRxMsg RxMessage;
CanTxMsg TxMessage;
CanTxMsg TxMsg;
//iCAN_msg      iCAN_TxMsg;                         // CAN messge for sending
iCAN_msg      GKY_TxMsg;
//iCAN_msg      rtCAN_TxMsg[10];//原先为
//iCAN_msg      lstCAN_TxMsg[10];
unsigned char rdCanBuf[200];
unsigned char wrCanBuf[200];
extern iCAN_msg     iCAN_RxMsg;
//iCAN_msg iCAN_RxMsg;
PKQ_DIS  pkqDisUnit[14];//14组批控器
//extern PKQ_MSG PKQ_Msg[14];

extern float ctof(unsigned char dt1,unsigned char dt2,unsigned char dt3,unsigned char dt4);
unsigned int  CAN_TxRdy = 0;                      // CAN HW ready to transmit a message
unsigned int  CAN_RxRdy = 0;                      // CAN HW received a message
u8 CAN1_Send_Job(u8 rtCnt);
u8 CAN1_Send_Conf(u8 rtCnt);
u8 CAN1_Send_Lst(u8 rtCnt);

unsigned char pkqDat[15][40];
void FillFBCan(unsigned char id,unsigned char FbId);
void FillRtCAN(unsigned char id);
void SendRtCAN(unsigned char rtCnt);
void SendLstCAN(unsigned char rtCnt);
void FillRtnTmr(void);//查询时钟返回值
void RtnNoLst(char *tBuf,unsigned char tRlt);
void FillLstCAN(unsigned char id);
extern unsigned int Fnd_Pwd_Lst(char* OrdId);
extern char *Get_Pwd_Lst(unsigned int tIndex);
extern void Fill_Sqr_Lst(unsigned char id,char *tBuf);
extern unsigned char Fnd_Pend(unsigned char id,char *OrdId);
char *gSqlBuf;
u8  tmpDat;

//CAN_msg       CAN_TxMsg;                          // CAN messge for sending

void CANHardwareConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	CAN_InitTypeDef  CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	RCC_AHB1PeriphClockCmd(CAN2_TX_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(CAN1_TX_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);

	GPIO_PinAFConfig(CAN1_RX_GPIO, CAN1_RX_PINSOURCE, GPIO_AF_CAN1);
	GPIO_PinAFConfig(CAN1_TX_GPIO, CAN1_TX_PINSOURCE, GPIO_AF_CAN1); 

	GPIO_PinAFConfig(CAN2_TX_GPIO, CAN2_TX_PINSOURCE, GPIO_AF_CAN2);
	GPIO_PinAFConfig(CAN2_RX_GPIO, CAN2_RX_PINSOURCE, GPIO_AF_CAN2);

	/* Configure CAN1 RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN | CAN1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN1_RX_GPIO, &GPIO_InitStructure);


/**********************************************************/
	/* CAN register init */
	CAN_DeInit(CAN1);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//CAN_SJW_3tq;//CAN_SJW_3tq;

	/* CAN Baudrate = 50KBps (CAN clocked at 42MHz)*/
	CAN_InitStructure.CAN_BS1= CAN_BS1_7tq;//CAN_BS1_13tq;//CAN_InitStructure.CAN_BS1= CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2= CAN_BS2_6tq;//CAN_InitStructure.CAN_BS2= CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler= 24;//54;//CAN_InitStructure.CAN_Prescaler= 140;
	CAN_Init(CAN1, &CAN_InitStructure);

	/* CAN filter init CA15N1 :0-13, CAN2: 14-27*/
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

/******************************************************/

		/* Configure CAN2 RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CAN2_RX_PIN | CAN2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN2_RX_GPIO, &GPIO_InitStructure);

	/* CAN register init */
	CAN_DeInit(CAN2);
//CAN_SJW_3tq--CAN_BS1_8tq--CAN_BS2_3tq--15  nok
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//CAN_SJW_3tq;//CAN_SJW_3tq;

//	/* CAN Baudrate = 50KBps (CAN clocked at 42MHz)*/
	CAN_InitStructure.CAN_BS1= CAN_BS1_7tq;
	CAN_InitStructure.CAN_BS2= CAN_BS2_6tq;
	CAN_InitStructure.CAN_Prescaler= 24;
	CAN_Init(CAN2, &CAN_InitStructure);

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 14;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);


	/* Enable FIFO 1 message pending Interrupt */
	CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	RxMsgCnt=0;
}


extern void ftoc(float tta);
extern char c2f[4];
//extern unsigned int mbdata[14][50];
unsigned char datCan[14][50];
extern void Draw_Cell_Conn(unsigned char i,unsigned char sta);
void CAN_waitReady (void)  
{
	  //while ((CAN1->TSR & CAN_TSR_TME0) == 0);         // Transmit mailbox 0 is empty
	  CAN_TxRdy = 1;
}
void SendCAN(void)
{
	GKY_TxMsg.SrcMacId=3;
	GKY_TxMsg.DestMacId=5;
	GKY_TxMsg.FuncID =2;
	GKY_TxMsg.SourceID =10;
	GKY_TxMsg.ACK =1;
	GKY_TxMsg.len =8;
	iCAN_wrMsg(&GKY_TxMsg);
}

void iCAN_rdMsg (iCAN_msg *msg)  
{
	vu32 thId;
	vu32 tlId;
	thId=(u32)0x000007FF & (CAN1->sFIFOMailBox[0].RIR>>21);
	tlId=(u32)0x0003FFFF & (CAN1->sFIFOMailBox[0].RIR>>3);
	msg->SrcMacId  	=  0x003F & thId>>3;
	msg->DestMacId 	= ((0x0007 & thId)<<5)|(tlId>>13);
	msg->ACK       	= (u32)0x00000001 & (tlId>>12);
	msg->FuncID    	= (u32)0x0000000F & (tlId>>8);
	msg->SourceID  	= (u32)0x000000FF & tlId;
	msg->len 			 	= (unsigned char)0x0000000F & CAN1->sFIFOMailBox[0].RDTR;
	msg->data[0] 	 	= (unsigned int)0x000000FF & (CAN1->sFIFOMailBox[0].RDLR);
	msg->data[1] 		= (unsigned int)0x000000FF & (CAN1->sFIFOMailBox[0].RDLR >> 8);
	msg->data[2] 		= (unsigned int)0x000000FF & (CAN1->sFIFOMailBox[0].RDLR >> 16);
	msg->data[3] 		= (unsigned int)0x000000FF & (CAN1->sFIFOMailBox[0].RDLR >> 24);
	msg->data[4] 		= (unsigned int)0x000000FF & (CAN1->sFIFOMailBox[0].RDHR);
	msg->data[5] 		= (unsigned int)0x000000FF & (CAN1->sFIFOMailBox[0].RDHR >> 8);
	msg->data[6] 		= (unsigned int)0x000000FF & (CAN1->sFIFOMailBox[0].RDHR >> 16);
	msg->data[7] 		= (unsigned int)0x000000FF & (CAN1->sFIFOMailBox[0].RDHR >> 24);

	CAN1->RF0R |= CAN_RF0R_RFOM0;                    // Release FIFO 0 output mailbox
}

void CAN_RECV(unsigned char id)
{
	unsigned long ulngDat;
	unsigned char tBuf[10];
	unsigned char tSrcMacId=0;
	unsigned char tDestMacId=0;
//	unsigned char tAck=0;
	unsigned char tFuncId=0;
	unsigned char tSourceId=0;
	unsigned char ci;
	unsigned char temp;
	unsigned char rLen;
			tmrCAN=0;flgCANerr=0;
			iCAN_TxMsg.SrcMacId =iCAN_RxMsg.DestMacId;
			iCAN_TxMsg.DestMacId=iCAN_RxMsg.SrcMacId;
			iCAN_TxMsg.ACK=iCAN_RxMsg.ACK;
			iCAN_TxMsg.FuncID=iCAN_RxMsg.FuncID;
			iCAN_TxMsg.SourceID=iCAN_RxMsg.SourceID;
			iCAN_TxMsg.len = 8;
			tSrcMacId=iCAN_RxMsg.SrcMacId;
			tDestMacId=iCAN_RxMsg.DestMacId;
			//tAck=iCAN_RxMsg.ACK;
			tFuncId=iCAN_RxMsg.FuncID;
			tSourceId=iCAN_RxMsg.SourceID;
			rtCAN_TxMsg[0].SrcMacId=tDestMacId;
			rtCAN_TxMsg[0].DestMacId=tSrcMacId;
			rtCAN_TxMsg[0].ACK=0;
			rtCAN_TxMsg[0].FuncID=tFuncId;
			rtCAN_TxMsg[0].SourceID=tSourceId;
			rtCAN_TxMsg[0].len = 8;
	
			switch(iCAN_TxMsg.FuncID)
			{
				case 9://查询实时数据
					for(ci=0;ci<6;ci++)
					{
						rtCAN_TxMsg[ci].len				=8;
						rtCAN_TxMsg[ci].SrcMacId	=tDestMacId;
						rtCAN_TxMsg[ci].DestMacId	=tSrcMacId;
						rtCAN_TxMsg[ci].ACK				=0;
						rtCAN_TxMsg[ci].FuncID		=9;
						rtCAN_TxMsg[ci].SourceID	=0xA;
					}
					FillRtCAN(0);
					flgCanFb=1;
					cntCanFb=6;
					canRtCnt=0;
				break;
				case 10://写时间
					
				break;
				case 11://写提单
					if(tSourceId==0x0A)   //下传提油密码,fb单帧
					{
						//提油密码3个包，标志位处理：0-已处理;1-第1包数据;2-第2包数据;0xff-数据收完，等待处理
						if(iCAN_RxMsg.data[0]==0x40)
						{
							for(ci=1;ci<8;ci++)
								CanRxdBuf[ci-1]=iCAN_RxMsg.data[ci];
							CanRxdBuf[0x19]=1;
						}
						if(iCAN_RxMsg.data[0]==0x81)
						{
							for(ci=1;ci<8;ci++)
								CanRxdBuf[ci+6]=iCAN_RxMsg.data[ci];
							if(CanRxdBuf[0x19]==1)
								CanRxdBuf[0x19]=2;
							else 
								CanRxdBuf[0x19]=0;
						}
						if(iCAN_RxMsg.data[0]==0xC0)
						{
							canChNum=iCAN_RxMsg.data[6];
							addChNum=canChNum;
							for(ci=1;ci<8;ci++)
								CanRxdBuf[ci+13]=iCAN_RxMsg.data[ci];
							if(CanRxdBuf[0x19]==2)
								CanRxdBuf[0x19]=3;
							else 
								CanRxdBuf[0x19]=0;
							if(CanRxdBuf[0x19]==3)
							{
							//0..3:提油密码,4:单位,5..8:应发量,9..12:油密:13..14:油温:15:采温方式	--老的
								//0..3:提油密码,4:单位,5..8:应发量,9..15:车号,16：备用,17:物料编码,18：路号
								CanRxdBuf[0x19]=0;
								ulngDat =CanRxdBuf[17]<<24;//提货单号
								ulngDat+=CanRxdBuf[18]<<16;
								ulngDat+=CanRxdBuf[15]<<8;
								ulngDat+=CanRxdBuf[16];
								if(ulngDat>99999999)ulngDat=0;
								tBuf[0]=ulngDat/10000000;
								ulngDat=ulngDat%10000000;
								tBuf[1]=ulngDat/1000000;
								ulngDat=ulngDat%1000000;
								tBuf[2]=ulngDat/100000;
								ulngDat=ulngDat%100000;
								tBuf[3]=ulngDat/10000;
								ulngDat=ulngDat%10000;
								tBuf[4]=ulngDat/1000;
								ulngDat=ulngDat%1000;
								tBuf[5]=ulngDat/100;
								ulngDat=ulngDat%100;
								tBuf[6]=ulngDat/10;
								ulngDat=ulngDat%10;
								tBuf[7]=ulngDat;
								for(ci=0;ci<4;ci++)
								{
								if(CanRxdBuf[19]==1)//第1路
								  CanData[104+ci]=CanRxdBuf[15+ci];
								if(CanRxdBuf[19]==2)//第1路
								  CanData[133+ci]=CanRxdBuf[15+ci];
								
								}
								NewPendBuf[0]=(tBuf[0]<<4)+ tBuf[1];
								NewPendBuf[1]=(tBuf[2]<<4)+ tBuf[3];
								NewPendBuf[2]=(tBuf[4]<<4)+ tBuf[5];
								NewPendBuf[3]=(tBuf[6]<<4)+ tBuf[7];
								ulngDat =CanRxdBuf[5]<<24;
								ulngDat+=CanRxdBuf[6]<<16;
								ulngDat+=CanRxdBuf[3]<<8;
								ulngDat+=CanRxdBuf[4];ulngDat*=10;
								if(ulngDat>99999999)ulngDat=0;
								tBuf[0]=ulngDat/10000000;
								ulngDat=ulngDat%10000000;
								tBuf[1]=ulngDat/1000000;
								ulngDat=ulngDat%1000000;
								tBuf[2]=ulngDat/100000;
								ulngDat=ulngDat%100000;
								tBuf[3]=ulngDat/10000;
								ulngDat=ulngDat%10000;
								tBuf[4]=ulngDat/1000;
								ulngDat=ulngDat%1000;
								tBuf[5]=ulngDat/100;
								ulngDat=ulngDat%100;
								tBuf[6]=ulngDat/10;
								ulngDat=ulngDat%10;
								tBuf[7]=ulngDat;
								NewPendBuf[5]=(tBuf[0]<<4)+ tBuf[1];
								NewPendBuf[6]=(tBuf[2]<<4)+ tBuf[3];
								NewPendBuf[7]=(tBuf[4]<<4)+ tBuf[5];
								NewPendBuf[8]=(tBuf[6]<<4)+ tBuf[7];
								//for(ci=0;ci<4;ci++)
								//  NewPendBuf[ci]=CanRxdBuf[15+ci];
								NewPendBuf[4]=CanRxdBuf[1];
								//for(ci=5;ci<9;ci++)//应发量
								//  NewPendBuf[ci]=CanRxdBuf[ci-2];
								for(ci=9;ci<16;ci++)//车号
								{
									NewPendBuf[ci]=CanRxdBuf[ci-2];	
								}
								for(ci=7;ci<14;ci++)//车号
								{
									if(CanRxdBuf[19]==1)//第1路
										CanData[96+ci-7]=CanRxdBuf[ci];
									if(CanRxdBuf[19]==2)//第2路
										CanData[125+ci-7]=CanRxdBuf[ci];
								}
								NewPendBuf[17]=CanRxdBuf[2];
								NewPendBuf[18]=CanRxdBuf[19];
								for(ci=0;ci<4;ci++)
								{
								  NewPendAscBuf[2*ci]='0'+(NewPendBuf[ci]>>4);
								  NewPendAscBuf[2*ci+1]='0'+(NewPendBuf[ci]&0x0F);
								}
								DelPendBuf[0]=(NewPendBuf[0]>>4)+'0';
								DelPendBuf[1]=(NewPendBuf[0]&0x0F)+'0';
								DelPendBuf[2]=(NewPendBuf[1]>>4)+'0';
								DelPendBuf[3]=(NewPendBuf[1]&0x0F)+'0';
								DelPendBuf[4]=(NewPendBuf[2]>>4)+'0';
								DelPendBuf[5]=(NewPendBuf[2]&0x0F)+'0';
								DelPendBuf[6]=(NewPendBuf[3]>>4)+'0';
								DelPendBuf[7]=(NewPendBuf[3]&0x0F)+'0';
								if(sys_STA==0)
								{
									NewPendAscBuf[8]=0;
									NewPendBuf[25]=0xFA;
									cntCanFb=1;canRtCnt=0;
									flgCanFb=4;
								}
								else
								{
									cntCanFb=1;canRtCnt=0;
									flgCanFb=0x84;
								}
							}
						}
						break;
					}				
				break;
				case 12://查提单
					
				break;
				case 13:
					
				break;
				case 14://远程控制
					if((dualChNum==1)&&(iCAN_RxMsg.data[1]==0x01))//当1#-->data[0]=0x55 开阀
					{
						flgRmCon=1;ubKey_Command=0xff;
					}
					if((dualChNum==2)&&(iCAN_RxMsg.data[2]==0x01))//当2#-->data[1]=0x55 开阀
					{
						flgRmCon=1;ubKey_Command=0xff;
					}
					if((dualChNum==1)&&(iCAN_RxMsg.data[1]==0x02))//当1#-->data[0]=0x55 开阀
					{
						flgRmCon=2;ubKey_Command=0xff;
					}
					if((dualChNum==2)&&(iCAN_RxMsg.data[2]==0x02))//当1#-->data[0]=0x55 开阀
					{
						flgRmCon=2;ubKey_Command=0xff;
					}
					if((dualChNum==1)&&(iCAN_RxMsg.data[1]==0x03))//当1#-->data[0]=0x55 开阀
					{
						flgRmCon=3;ubKey_Command=0xff;
					}
					if((dualChNum==2)&&(iCAN_RxMsg.data[2]==0x03))//当1#-->data[0]=0x55 开阀
					{
						flgRmCon=3;ubKey_Command=0xff;
					}
					break;
			}
}

void CAN_EVENTS(void)
{
	unsigned char tSrcMacId=0;
	unsigned char tDestMacId=0;
	unsigned char tFuncId=0;
	if(CAN_RxRdy)
	{
	  CAN_RxRdy = 0;
		tSrcMacId=iCAN_RxMsg.SrcMacId;
		tDestMacId=iCAN_RxMsg.DestMacId;
		tFuncId=iCAN_RxMsg.FuncID;
		if(tSrcMacId<5)
		{
			if((tDestMacId==0x01)&&(tFuncId==10))
			{
				sysY =iCAN_RxMsg.data[1];
				sysM =iCAN_RxMsg.data[2];
				sysD =iCAN_RxMsg.data[3];
				sysHH=iCAN_RxMsg.data[4];
				sysMM=iCAN_RxMsg.data[5];
				sysSS=iCAN_RxMsg.data[6];
				RTCSetTime(sysHH,sysMM,sysSS);					
				RTCSetDate(sysY,sysM,sysD);		
			}
			if((tDestMacId==set_AddCa)||(tDestMacId==set_AddCa+1)) //双路
			//if(tDestMacId==set_AddCa)//if(tDestMacId>3)//单路
			{
			  	CAN_RECV(0);//(tDestMacId-set_AddCa);
			}
		}
	}
	if(flgCanSTA==0)
	{	
		if(flgCanFb>0)
		{
			if(canRtCnt<cntCanFb)
			{
				CAN_waitReady();
				if(CAN_TxRdy==1)
				{
					CAN1_Send_Msg(canRtCnt);//SendRtCAN(canRtCnt);
					canRtCnt++;
					if(canRtCnt>=cntCanFb)
					{	
						flgCanFb=0;
						cntCanFb=0;
					}
				}
			 }
		 }
	 }
	else
	{
		if((flgCanSTA&0x01)==0x01)//上传作业单
		{
			if(cntJobCan<4)
			{
				CAN_waitReady();
				if(CAN_TxRdy==1)
				{
					CAN1_Send_Job(cntJobCan);//SendRtCAN(canRtCnt);
					cntJobCan++;
					if(cntJobCan>=4)
					{	
						cntJobCan=0;
						flgCanSTA=flgCanSTA&0xFE;
					}
				}
			}
		}
		if((flgCanSTA&0x02)==0x02)//上传记录值
		{
			if(cntLstCan<8)
			{
				CAN_waitReady();
				if(CAN_TxRdy==1)
				{
					CAN1_Send_Lst(cntLstCan);//SendRtCAN(canRtCnt);
					cntLstCan++;
					if(cntLstCan>=8)
					{	
						cntLstCan=0;
						flgCanSTA=flgCanSTA&0xFD;
					}
				}
			}
		}
		if((flgCanSTA&0x04)==0x04)//上传组态值
		{
			if(cntConfCan<3)
			{
				CAN_waitReady();
				if(CAN_TxRdy==1)
				{
					CAN1_Send_Conf(cntConfCan);//SendRtCAN(canRtCnt);
					cntConfCan++;
					if(cntConfCan>=3)
					{	
						cntConfCan=0;
						flgCanSTA=flgCanSTA&0xFB;
					}
				}
			}
		}
	}
}
void FillRtCAN(unsigned char id)
{
//	unsigned char ci=0;
	unsigned char i;
	unsigned char t[10];
	float tFlDat;
	//float	rtSFV;
	unsigned long tLngDat;
	rtCAN_TxMsg[0].data[0]=0x40;
	rtCAN_TxMsg[1].data[0]=0x81;
	rtCAN_TxMsg[2].data[0]=0x82;
	rtCAN_TxMsg[3].data[0]=0x83;
	rtCAN_TxMsg[4].data[0]=0x84;
	rtCAN_TxMsg[5].data[0]=0xC0;
	for(i=0;i<7;i++)
	{
		rtCAN_TxMsg[ 0].data[i+1]=canRtData[   i];
		rtCAN_TxMsg[ 1].data[i+1]=canRtData[ 7+i];
		rtCAN_TxMsg[ 2].data[i+1]=canRtData[14+i];
		rtCAN_TxMsg[ 3].data[i+1]=canRtData[21+i];
		rtCAN_TxMsg[ 4].data[i+1]=canRtData[28+i];
		rtCAN_TxMsg[ 5].data[i+1]=canRtData[35+i];
	}
}
void FillRtCAN_(unsigned char id)
{
//	unsigned char ci=0;
	unsigned char i;
	unsigned char t[10];
	float tFlDat;
	//float	rtSFV;
	unsigned long tLngDat;
	rtCAN_TxMsg[ 0].data[0]=0x40;
	rtCAN_TxMsg[ 1].data[0]=0x81;
	rtCAN_TxMsg[ 2].data[0]=0x82;
	rtCAN_TxMsg[ 3].data[0]=0x83;
	rtCAN_TxMsg[ 4].data[0]=0x84;
	rtCAN_TxMsg[ 5].data[0]=0x85;
	rtCAN_TxMsg[ 6].data[0]=0x86;
	rtCAN_TxMsg[ 7].data[0]=0x87;
	rtCAN_TxMsg[ 8].data[0]=0x88;
	rtCAN_TxMsg[ 9].data[0]=0x89;
	rtCAN_TxMsg[10].data[0]=0x8A;
	rtCAN_TxMsg[11].data[0]=0x8B;
	rtCAN_TxMsg[12].data[0]=0x8C;
	rtCAN_TxMsg[13].data[0]=0x8D;
	rtCAN_TxMsg[14].data[0]=0xC0;
	for(i=0;i<7;i++)
	{
		rtCAN_TxMsg[ 0].data[i+1]=CanData[ 40+i];
		rtCAN_TxMsg[ 1].data[i+1]=CanData[ 47+i];
		rtCAN_TxMsg[ 2].data[i+1]=CanData[ 54+i];
		rtCAN_TxMsg[ 3].data[i+1]=CanData[ 61+i];
		rtCAN_TxMsg[ 4].data[i+1]=CanData[ 68+i];
		rtCAN_TxMsg[ 5].data[i+1]=CanData[ 75+i];
		rtCAN_TxMsg[ 6].data[i+1]=CanData[ 82+i];
		rtCAN_TxMsg[ 7].data[i+1]=CanData[ 89+i];
		rtCAN_TxMsg[ 8].data[i+1]=CanData[ 96+i];
		rtCAN_TxMsg[ 9].data[i+1]=CanData[103+i];
		rtCAN_TxMsg[10].data[i+1]=CanData[110+i];
		rtCAN_TxMsg[11].data[i+1]=CanData[117+i];
		rtCAN_TxMsg[12].data[i+1]=CanData[124+i];
		rtCAN_TxMsg[13].data[i+1]=CanData[131+i];
		rtCAN_TxMsg[14].data[i+1]=CanData[138+i];
	}


}
void FillJobCAN(void)
{
	unsigned char i;
	for(i=0;i<4;i++)
	{
		jobCAN_TxMsg[i].len				=8;
		jobCAN_TxMsg[i].SrcMacId	=set_AddCa;
		jobCAN_TxMsg[i].DestMacId	=3;
		jobCAN_TxMsg[i].ACK				=0;
		jobCAN_TxMsg[i].FuncID		=8;
		jobCAN_TxMsg[i].SourceID	=0xA;
	}
	jobCAN_TxMsg[0].data[0]=0x40;
	jobCAN_TxMsg[1].data[0]=0x81;
	jobCAN_TxMsg[2].data[0]=0x82;
	jobCAN_TxMsg[3].data[0]=0xC0;
	for(i=0;i<7;i++)
	{
	jobCAN_TxMsg[0].data[1+i]=canJobData[i];
	jobCAN_TxMsg[1].data[1+i]=canJobData[7+i];
	jobCAN_TxMsg[2].data[1+i]=canJobData[14+i];
	jobCAN_TxMsg[3].data[1+i]=canJobData[21+i];
	}
	cntJobCan=0;flgCanSTA|=1;
}
void FillConfData(void)
{
	unsigned char i;
	for(i=0;i<3;i++)
	{
		confCAN_TxMsg[i].len				=8;
		confCAN_TxMsg[i].SrcMacId		=set_AddCa;
		confCAN_TxMsg[i].DestMacId	=3;
		confCAN_TxMsg[i].ACK				=0;
		confCAN_TxMsg[i].FuncID			=7;
		confCAN_TxMsg[i].SourceID		=0xA;
	}
	confCAN_TxMsg[0].data[0]=0x40;
	confCAN_TxMsg[1].data[0]=0x81;
	confCAN_TxMsg[2].data[0]=0xC0;
	for(i=0;i<7;i++)
	{
	confCAN_TxMsg[0].data[1+i]=canConfData[i];
	confCAN_TxMsg[1].data[1+i]=canConfData[7+i];
	confCAN_TxMsg[2].data[1+i]=canConfData[14+i];
	}
	cntConfCan=0;flgCanSTA|=4;
	
}
void FillLstData(void)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		lstCAN_TxMsg[i].len					=8;
		lstCAN_TxMsg[i].SrcMacId		=set_AddCa;
		lstCAN_TxMsg[i].DestMacId		=3;
		lstCAN_TxMsg[i].ACK					=0;
		lstCAN_TxMsg[i].FuncID			=6;
		lstCAN_TxMsg[i].SourceID		=i+1;
	}
		lstCAN_TxMsg[0].data[0]=0x40;
		lstCAN_TxMsg[1].data[0]=0x81;
		lstCAN_TxMsg[2].data[0]=0x82;
		lstCAN_TxMsg[3].data[0]=0x83;
		lstCAN_TxMsg[4].data[0]=0x84;
		lstCAN_TxMsg[5].data[0]=0x85;
		lstCAN_TxMsg[6].data[0]=0x86;
		lstCAN_TxMsg[7].data[0]=0xC0;
	for(i=0;i<7;i++)
	{
		lstCAN_TxMsg[0].data[1+i]=canLstData[i];
		lstCAN_TxMsg[1].data[1+i]=canLstData[7+i];
		lstCAN_TxMsg[2].data[1+i]=canLstData[14+i];
		lstCAN_TxMsg[3].data[1+i]=canLstData[21+i];
		lstCAN_TxMsg[4].data[1+i]=canLstData[28+i];
		lstCAN_TxMsg[5].data[1+i]=canLstData[35+i];
		lstCAN_TxMsg[6].data[1+i]=canLstData[42+i];
		lstCAN_TxMsg[7].data[1+i]=canLstData[49+i];
	}
	cntLstCan=0;flgCanSTA|=2;
	
}

//id为路号，0:路号1；1:路号2。
void FillFBCan(unsigned char id,unsigned char FbId)
{
	unsigned char i;

	switch(FbId)
	{
		case 1:
			FillRtCAN(id);
			cntCanFb=15;
			break;
		case 2:
			//TBD
			break;
		case 3://发油工作方式
			//
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x42;
			rtCAN_TxMsg[0].data[2]=id+1;
			cntCanFb=1;
			canRtCnt=0;
			break;
		case 4://下传提油密码
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x43;
			rtCAN_TxMsg[0].data[2]=0x55;
			rtCAN_TxMsg[0].data[3]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 0x84://下传提油密码
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x43;
			rtCAN_TxMsg[0].data[2]=0xAA;
			rtCAN_TxMsg[0].data[3]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 5://清除提单成功
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x44;
			rtCAN_TxMsg[0].data[2]=0x55;
			rtCAN_TxMsg[0].data[3]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 0x85://清除提单失败
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x44;
			rtCAN_TxMsg[0].data[2]=0xAA;
			rtCAN_TxMsg[0].data[3]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 6://下传时钟
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x46;
			rtCAN_TxMsg[0].data[2]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 7://远程发油控制
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x4F;
			rtCAN_TxMsg[0].data[2]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 8://下发油品编码
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x4C;
			rtCAN_TxMsg[0].data[2]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 12://组态参数
			//TBD
			break;
		case 13://查询时钟
			rtCAN_TxMsg[0].data[0]=0x00;
			rtCAN_TxMsg[0].data[1]=sysSS;
			rtCAN_TxMsg[0].data[2]=sysMM;
			rtCAN_TxMsg[0].data[3]=sysHH;
			rtCAN_TxMsg[0].data[4]=sysD;
			rtCAN_TxMsg[0].data[5]=sysM;
			rtCAN_TxMsg[0].data[6]=sysY;
			rtCAN_TxMsg[0].data[7]=id+1;
			cntCanFb=1;canRtCnt=0;
		  break;
	}	
}

void FillDisBuf(unsigned char id)
{
//	char tDis[20];
//	unsigned char i;
//	unsigned char tDat[50];
//	unsigned long tlngDat;
//	unsigned char tMbId;
//	float flDat;
//	for(i=0;i<50;i++)
//	{
//		tDat[i]=pkqDat[id][i];
//		
//	}
//	tMbId=tDat[39]-1;//PKQ,MB address,批控器的MB地址
//	pkqDisUnit[tMbId].NewDat=0;
//	Draw_Cell_Conn(tMbId,0);pkqDisUnit[tMbId].cntConn=0;

//	for(i=0;i<50;i++)
//	{
//		if(datCan[id][i]!=tDat[i])
//		{
//			pkqDisUnit[tMbId].NewDat=1;
//			datCan[id][i]=tDat[i];
//		}
//	}
//	pkqDisUnit[tMbId].disSTA[0]=tDat[30];//sys_STA
//	pkqDisUnit[tMbId].disSTA[1]=tDat[34];//local conn
//	//温度  WD
//	flDat=((tDat[6]&0xF0)>>4)*1.0;
//	flDat+=((tDat[5]&0x0F)%10)*10.0;
//	flDat+=((tDat[5]&0x0F)/10)*100.0;
//	flDat+=(tDat[6]&0x0F)*0.1;
//	if(0x10==(tDat[5]&0xF0))
//	{	tDis[0]='-';flDat=-1.0*flDat;}
//	else
//	{	tDis[0]=' ';}
//	ftoc(flDat);
//	mbdata[tMbId][31]=(c2f[1]<<8)+c2f[0];
//	mbdata[tMbId][32]=(c2f[3]<<8)+c2f[2];
//	tDis[1]=0x30+((tDat[5]&0x0F)/10);
//	tDis[2]=0x30+((tDat[5]&0x0F)%10);
//	tDis[3]=0x30+((tDat[6]&0xF0)>>4);
//	tDis[4]='.';
//	tDis[5]=0x30+(tDat[6]&0x0F);
//	tDis[6]=0;

//	if(tDis[1]=='0')
//	{
//		tDis[1]=' ';
//		if(tDis[2]=='0')
//		{
//			tDis[2]=' ';
//		}
//	}
//	for(i=0;i<=6;i++)
//	{
//		pkqDisUnit[tMbId].disWD[i]=tDis[i];
//	}
//	
//	//瞬时量
//	flDat=(tDat[2]&0x0F)*1.0;
//	flDat+=((tDat[2]&0xF0)>>4)*10.0;
//	flDat+=((tDat[1]&0x0F))*100.0;
//	flDat+=((tDat[3]&0xF0)>>4)*0.1;
//	flDat+=((tDat[3]&0x0F))*0.01;
//	flDat+=((tDat[4]&0xF0)>>4)*0.001;
//	flDat+=((tDat[4]&0x0F))*0.0001;
//	ftoc(flDat);
//	mbdata[tMbId][27]=(c2f[1]<<8)+c2f[0];
//	mbdata[tMbId][28]=(c2f[3]<<8)+c2f[2];
//	flDat=(float)(flDat*3.6);
//	tlngDat=(unsigned long)(flDat*10000);
//	tDis[0]='0'+(tlngDat/1000000);
//	tlngDat=tlngDat%1000000;
//	tDis[1]='0'+(tlngDat/100000);
//	tlngDat=tlngDat%100000;
//	tDis[2]='0'+(tlngDat/10000);
//	tDis[3]='.';
//	tlngDat=tlngDat%10000;
//	tDis[4]='0'+(tlngDat/1000);
//	tlngDat=tlngDat%1000;
//	tDis[5]='0'+(tlngDat/100);
//	tlngDat=tlngDat%100;
//	tDis[6]='0'+(tlngDat/10);
//	tlngDat=tlngDat%10;
//	tDis[7]='0'+tlngDat;
//	tDis[8] =0;
//	//tDis[0] =0x30+((tDat[1]&0x0F));
//	//tDis[1] =0x30+((tDat[2]&0xF0)>>4);
//	//tDis[2] =0x30+((tDat[2]&0x0F));
//	//tDis[3] ='.';
//	//tDis[4] =0x30+((tDat[3]&0xF0)>>4);
//	//tDis[5] =0x30+((tDat[3]&0x0F));
//	//tDis[6] =0x30+((tDat[4]&0xF0)>>4);
//	//tDis[7] =0x30+((tDat[4]&0x0F));
//	for(i=0;i<2;i++)
//	{		if(tDis[i]=='0')			tDis[i]=' ';			else			break;	}
//	for(i=0;i<=8;i++)
//	{
//		pkqDisUnit[tMbId].disSSL[i]=tDis[i];
//	}
//	//密度
//	flDat =(tDat[9]&0x0F)*1.0;
//	flDat+=(tDat[9]>>4)*10.0;
//	flDat+=(tDat[7]&0x0F)*100.0;
//	flDat+=(tDat[7]>>4)*1000.0;
//	flDat+=(tDat[10]>>4)*0.1;
//	flDat+=(tDat[10]&0x0F)*0.01;
//	flDat+=(tDat[11]>>4)*0.001;
//	flDat+=(tDat[11]&0x0F)*0.0001;
//	ftoc(flDat);
//	mbdata[tMbId][29]=(c2f[1]<<8)+c2f[0];
//	mbdata[tMbId][30]=(c2f[3]<<8)+c2f[2];
//	tDis[0]=0x30+(tDat[7]>>4);
//	tDis[1]=0x30+(tDat[7]&0x0F);
//	tDis[2]=0x30+(tDat[9]>>4);
//	tDis[3]=0x30+(tDat[9]&0x0F);
//	tDis[4]='.';
//	tDis[5]=0x30+(tDat[10]>>4);
//	tDis[6]=0x30+(tDat[10]&0x0F);
//	tDis[7]=0x30+(tDat[11]>>4);
//	tDis[8]=0x30+(tDat[11]&0x0F);
//	tDis[9]=0;
//	for(i=0;i<3;i++)
//	{		if(tDis[i]=='0')			tDis[i]=' ';			else			break;	}
//	for(i=0;i<=9;i++)
//	{
//		pkqDisUnit[tMbId].disMD[i]=tDis[i];
//	}
//	//应发量
//	tDis[0]=0x30+(tDat[12]>>4);
//	tDis[1]=0x30+(tDat[12]&0xF);
//	tDis[2]=0x30+(tDat[13]>>4);
//	tDis[3]=0x30+(tDat[13]&0xF);
//	tDis[4]=0x30+(tDat[14]>>4);
//	tDis[5]=0x30+(tDat[14]&0xF);
//	tDis[6]=0x30+(tDat[15]>>4);
//	tDis[7]=0x30+(tDat[15]&0xF);
//	tDis[8]=0;
//	for(i=0;i<7;i++)
//	{		if(tDis[i]=='0')			tDis[i]=' ';			else			break;	}
//	for(i=0;i<=8;i++)
//	{
//		pkqDisUnit[tMbId].disYFL[i]=tDis[i];
//	}
//	//实发量
//	tDis[0]=0x30+(tDat[17]>>4);
//	tDis[1]=0x30+(tDat[17]&0xF);
//	tDis[2]=0x30+(tDat[18]>>4);
//	tDis[3]=0x30+(tDat[18]&0xF);
//	tDis[4]=0x30+(tDat[19]>>4);
//	tDis[5]=0x30+(tDat[19]&0xF);
//	tDis[6]=0x30+(tDat[20]>>4);
//	tDis[7]='.';
//	tDis[8]=0x30+(tDat[20]&0xF);
//	tDis[9]=0;
//	for(i=0;i<6;i++)
//	{		if(tDis[i]=='0')			tDis[i]=' ';			else			break;	}
//	for(i=0;i<=8;i++)
//	{
//		pkqDisUnit[tMbId].disSFL[i]=tDis[i];
//	}
//	//累积数
//	//flDat=ctof(tDat[23]);
//	flDat =(tDat[25]>>4)*1.0;
//	flDat+=(tDat[23]&0xF)*10.0;
//	flDat+=(tDat[23]>>4)*100.0;
//	flDat+=(tDat[22]&0xF)*1000.0;
//	flDat+=(tDat[22]>>4)*10000.0;
//	flDat+=(tDat[21]&0xF)*10000.0;
//	flDat+=(tDat[21]>>4)*10000.0;
//	ftoc(flDat);
//	mbdata[tMbId][33]=(c2f[1]<<8)+c2f[0];
//	mbdata[tMbId][34]=(c2f[3]<<8)+c2f[2];
//	tDis[0]=0x30+(tDat[21]>>4);
//	tDis[1]=0x30+(tDat[21]&0xF);
//	tDis[2]=0x30+(tDat[22]>>4);
//	tDis[3]=0x30+(tDat[22]&0xF);
//	tDis[4]=0x30+(tDat[23]>>4);
//	tDis[5]=0x30+(tDat[23]&0xF);
//	tDis[6]=0x30+(tDat[25]>>4);
//	tDis[7]='.';
//	tDis[8]=0x30+(tDat[25]&0xF);
//	tDis[9]=0;
//	for(i=0;i<6;i++)
//	{		if(tDis[i]=='0')			tDis[i]=' ';			else			break;	}
//	for(i=0;i<=8;i++)
//	{
//		pkqDisUnit[tMbId].disLJS[i]=tDis[i];
//	}

}
unsigned char pkqDatLst=0;
unsigned long tRxdExtId;
void RdCanMsg(void)
{
	unsigned char i,j;
	unsigned char tId;
	for(i=0;i<RxMsgCnt;i++)
	{
		tRxdExtId=RxMsgBuf[i].ExtId ;
		tRxdExtId=tRxdExtId&0x001FEFFF;//tRxdExtId=tRxdExtId&0x07E00FFF;
		if(tRxdExtId==0x0000620A)//从站发出来的巡测报文
		{ 
			tId=0x3F&(RxMsgBuf[i].ExtId>>21);
			if(tId>=5)
			{
				if(RxMsgBuf[i].Data[0]==0x40)
					pkqDatLst=0;
				//tMbId=RxMsgBuf[i].Data[7];
				for(j=0;j<8;j++)
				{	
					pkqDat[tId-5][pkqDatLst]=RxMsgBuf[i].Data[j];
					pkqDatLst++;
					if(pkqDatLst>=40)
					{
						pkqDatLst=0;
						FillDisBuf(tId-5);
					}
				}
			}
		}
	}
	RxMsgCnt=0;
}	

void CAN1_RX0_IRQHandler(void)
{
	unsigned char i,j;
	unsigned char tId;
  char tBuf[6];
	iCAN_rdMsg (&iCAN_RxMsg);   
  CAN_RxRdy = 1;                                // set receive flag
	//CAN_EVENTS();
	
	////CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
//tRxdExtId=RxMessage.ExtId;
//tRxdExtId=tRxdExtId&0x001FEFFF;//tRxdExtId=tRxdExtId&0x07E00FFF;
//if(tRxdExtId==0x0000A20A)//从站发出来的巡测报文
//{
//	tId=0x3F&(RxMessage.ExtId>>21);
//	tBuf[0]='I';tBuf[1]='D';tBuf[2]='=';tBuf[3]=tId/10+'0';tBuf[4]=tId%10+'0';tBuf[5]=0;
//	lcd_text16(200,200-16,0xF800,0,tBuf);
//	lcd_text16(200,200,0xF800,0,"CAN1 OK!");
//}
}

u8 CAN1_Send_Job(u8 rtCnt)
{
		unsigned int i=0;
	unsigned char mbox;
	mbox=iCAN_Transmit(CAN1,&jobCAN_TxMsg[rtCnt]);//iCAN_wrMsg(&GKY_TxMsg);
	if(mbox==4)
	{
		return 2;//no mail box
	}
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))
		i++;	
  if(i>=0XFFF)
		return 1;
	else
		return 0;
}
u8 CAN1_Send_Conf(u8 rtCnt)
{
		unsigned int i=0;
	unsigned char mbox;
	mbox=iCAN_Transmit(CAN1,&confCAN_TxMsg[rtCnt]);//iCAN_wrMsg(&GKY_TxMsg);
	if(mbox==4)
	{
		return 2;//no mail box
	}
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))
		i++;	
  if(i>=0XFFF)
		return 1;
	else
		return 0;
}
u8 CAN1_Send_Lst(u8 rtCnt)
{
		unsigned int i=0;
	unsigned char mbox;
	mbox=iCAN_Transmit(CAN1,&lstCAN_TxMsg[rtCnt]);//iCAN_wrMsg(&GKY_TxMsg);
	if(mbox==4)
	{
		return 2;//no mail box
	}
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))
		i++;	
  if(i>=0XFFF)
		return 1;
	else
		return 0;
}

u8 CAN1_Send_Msg(u8 rtCnt)
{
		unsigned int i=0;
	unsigned char mbox;
	mbox=iCAN_Transmit(CAN1,&rtCAN_TxMsg[rtCnt]);//iCAN_wrMsg(&GKY_TxMsg);
	if(mbox==4)
	{
		return 2;//no mail box
	}
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))
		i++;	
  if(i>=0XFFF)
		return 1;
	else
		return 0;

}

void CAN2_RX1_IRQHandler(void)  
{
    unsigned char i,j;
    unsigned char tId;
     char tBuf[6];
	  CAN_Receive(CAN2, CAN_FIFO1, &RxMessage);
    tRxdExtId=RxMessage.ExtId;
		tRxdExtId=tRxdExtId&0x001FEFFF;//tRxdExtId=tRxdExtId&0x07E00FFF;
		if(tRxdExtId==0x0000A20A)//从站发出来的巡测报文
    {
			tId=0x3F&(RxMessage.ExtId>>21);
      tBuf[0]='I';tBuf[1]='D';tBuf[2]='=';tBuf[3]=tId/10+'0';tBuf[4]=tId%10+'0';tBuf[5]=0;
      lcd_text16(600,200-16,0xF800,0,tBuf);
      lcd_text16(600,200,0xF800,0,"CAN2 OK!");
    }
	  //CAN_Receive(CAN2, CAN_FIFO1, &RxMsgBuf[RxMsgCnt++]);
		//RdCanMsg();
}   
void FillLstCAN(unsigned char id)
{
	unsigned char fi;
	
	for(fi=0;fi<8;fi++)
	{
		rtCAN_TxMsg[fi].SrcMacId		= set_AddCa;
		rtCAN_TxMsg[fi].DestMacId		= 3;
		rtCAN_TxMsg[fi].ACK					= 0;
		rtCAN_TxMsg[fi].FuncID			= 3;
		rtCAN_TxMsg[fi].SourceID		= 10;
		rtCAN_TxMsg[fi].len 				= 8;
	 }
	rtCAN_TxMsg[0].data[0]=0x40;
	rtCAN_TxMsg[7].data[0]=0xC0;
	for(fi=1;fi<7;fi++)
		rtCAN_TxMsg[fi].data[0]=0x80+fi;
	for(fi=1;fi<8;fi++)
		rtCAN_TxMsg[0].data[fi]=Fy2CanLst[fi+0x00];
	for(fi=1;fi<8;fi++)
		rtCAN_TxMsg[1].data[fi]=Fy2CanLst[fi+0x08];
	for(fi=1;fi<8;fi++)
		rtCAN_TxMsg[2].data[fi]=Fy2CanLst[fi+0x10];
	for(fi=1;fi<8;fi++)
		rtCAN_TxMsg[3].data[fi]=Fy2CanLst[fi+0x18];
	for(fi=1;fi<8;fi++)
		rtCAN_TxMsg[4].data[fi]=Fy2CanLst[fi+0x20];
	for(fi=1;fi<8;fi++)
		rtCAN_TxMsg[5].data[fi]=Fy2CanLst[fi+0x28];
	for(fi=1;fi<8;fi++)
		rtCAN_TxMsg[6].data[fi]=Fy2CanLst[fi+0x30];
	for(fi=1;fi<8;fi++)
		rtCAN_TxMsg[7].data[fi]=Fy2CanLst[fi+0x38];
}

//*tBuf提单密码,tRlt[AA:不存在,00:待发]
void RtnNoLst(char *tBuf,unsigned char tRlt)
{
	rtCAN_TxMsg[0].data[0]=0x00;
	rtCAN_TxMsg[0].data[1]=((tBuf[0]&0x0F)<<4)+(tBuf[1]&0x0F);
	rtCAN_TxMsg[0].data[2]=((tBuf[2]&0x0F)<<4)+(tBuf[3]&0x0F);
	rtCAN_TxMsg[0].data[3]=((tBuf[4]&0x0F)<<4)+(tBuf[5]&0x0F);
	rtCAN_TxMsg[0].data[4]=((tBuf[6]&0x0F)<<4)+(tBuf[7]&0x0F);
	rtCAN_TxMsg[0].data[5]=tRlt;//标志
	rtCAN_TxMsg[0].data[6]=0x71;
	rtCAN_TxMsg[0].data[7]=canChNum;
	flgCanFb=10;cntCanFb=1;canRtCnt=0;
}

void FillRtnTmr(void)//查询时钟返回值
{
//	rtCAN_TxMsg[0].data[0]=0x00;
//	rtCAN_TxMsg[0].data[1]=CanData[0xF5];
//	rtCAN_TxMsg[0].data[2]=CanData[0xF4];
//	rtCAN_TxMsg[0].data[3]=CanData[0xF3];
//	rtCAN_TxMsg[0].data[4]=CanData[0xF2];
//	rtCAN_TxMsg[0].data[5]=CanData[0xF1];
//	rtCAN_TxMsg[0].data[6]=CanData[0xF0];
//	rtCAN_TxMsg[0].data[7]=0x4D;
//	flgCanFb=13;cntCanFb=1;canRtCnt=0;
}
void SendLstCAN(unsigned char rtCnt)
{
	iCAN_wrMsg(&(lstCAN_TxMsg[rtCnt]));	

}
void SendRtCAN(unsigned char rtCnt)
{
	iCAN_wrMsg(&(rtCAN_TxMsg[rtCnt]));	
}

