#include "can.h"
#include "delay.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_iwdg.h"
#include "Global.h"
//CANET100 190032
//GC-CAN   2CC01C
//CAN2_RX:PB5  CAN2_TX:PB6
#define CAN2_TX_CLK 			RCC_AHB1Periph_GPIOB
#define CAN2_TX_PIN				GPIO_Pin_6
#define CAN2_TX_PINSOURCE		GPIO_PinSource6
#define CAN2_TX_GPIO			GPIOB

#define CAN2_RX_CLK				RCC_AHB1Periph_GPIOB
#define CAN2_RX_PIN				GPIO_Pin_5
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


//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//则波特率为:42M/((6+7+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 

iCAN_msg      GKY_TxMsg;
//iCAN_msg      iCAN_RxMsg;                         // CAN message for receiving                                
extern char *Get_Pwd_Lst(unsigned int tIndex);
void FillLstCAN(unsigned char id);
void CAN_waitReady (void);
void FillLstCAN(unsigned char id);
void FillRtCAN(unsigned char id);
void RtnNoLst(char *tBuf,unsigned char tRlt);
void SendRtCAN(unsigned char rtCnt);
void iCAN_wrMsg (iCAN_msg *msg)  ;



void iCAN_wrMsg (iCAN_msg *msg)  
{
	vu32 tId; 
	CAN1->sTxMailBox[0].TIR  = (unsigned int)0;
	tId=msg->SrcMacId<<24;
	tId=tId | (msg->DestMacId<<16);
	tId=tId |	(msg->ACK<<15);
	tId=tId | (msg->FuncID<<11);
	tId=tId | (msg->SourceID<<3); 
	tId=tId | CAN_ID_EXT; 
	CAN1->sTxMailBox[0].TIR=tId;
	CAN1->sTxMailBox[0].TDLR = (((unsigned int)msg->data[3] << 24) | 
	                         ((unsigned int)msg->data[2] << 16) |
	                         ((unsigned int)msg->data[1] <<  8) | 
	                         ((unsigned int)msg->data[0])        );
	CAN1->sTxMailBox[0].TDHR = (((unsigned int)msg->data[7] << 24) | 
	                         ((unsigned int)msg->data[6] << 16) |
	                         ((unsigned int)msg->data[5] <<  8) |
	                         ((unsigned int)msg->data[4])        );
	                                              // Setup length
	CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT1R_DLC;
	CAN1->sTxMailBox[0].TDTR |=  (msg->len & CAN_TDT1R_DLC);

	CAN1->IER |= CAN_IER_TMEIE;
	CAN1->sTxMailBox[0].TIR |=  CAN_TI1R_TXRQ;
}

void SendRtCAN(unsigned char rtCnt)
{
	iCAN_wrMsg(&(rtCAN_TxMsg[rtCnt]));	
}
//*tBuf????,tRlt[AA:???,00:??]
void RtnNoLst(char *tBuf,unsigned char tRlt)
{
	rtCAN_TxMsg[0].data[0]=0x00;
	rtCAN_TxMsg[0].data[1]=((tBuf[0]&0x0F)<<4)+(tBuf[1]&0x0F);
	rtCAN_TxMsg[0].data[2]=((tBuf[2]&0x0F)<<4)+(tBuf[3]&0x0F);
	rtCAN_TxMsg[0].data[3]=((tBuf[4]&0x0F)<<4)+(tBuf[5]&0x0F);
	rtCAN_TxMsg[0].data[4]=((tBuf[6]&0x0F)<<4)+(tBuf[7]&0x0F);
	rtCAN_TxMsg[0].data[5]=tRlt;//??
	rtCAN_TxMsg[0].data[6]=0x71;
	rtCAN_TxMsg[0].data[7]=canChNum;
	flgCanFb=10;cntCanFb=1;canRtCnt=0;
}

void CAN_waitReady (void)  
{
	  while ((CAN1->TSR & CAN_TSR_TME0) == 0);         // Transmit mailbox 0 is empty
	  CAN_TxRdy = 1;
}
void Fill_Sqr_Lst(unsigned char id,char *tBuf)
{
		unsigned long lngMet1,lngMet2;
		unsigned char tAdd;
		char flMet1[4];
		char flMet2[4];
		float flMeter1,flMeter2;
		char tSqrBuf[80];
		unsigned char i;
		for(i=0;i<80;i++)
			tSqrBuf[i]=tBuf[i];
		tAdd=(tSqrBuf[0]<<4)|(tSqrBuf[1]&0x0F);
		//???CAN??????
	 	lngMet1 =(unsigned long)(tSqrBuf[17]>>4)*	((unsigned long)1000000000);
	 	lngMet1+=(unsigned long)(tSqrBuf[17]&0x0F)*((unsigned long)100000000);
	 	lngMet1+=(unsigned long)(tSqrBuf[18]>>4)*	((unsigned long)10000000);
	 	lngMet1+=(unsigned long)(tSqrBuf[18]&0x0F)*((unsigned long)1000000);
	 	lngMet1+=(unsigned long)(tSqrBuf[19]>>4)*	((unsigned long)100000);
	 	lngMet1+=(unsigned long)(tSqrBuf[19]&0x0F)*((unsigned long)10000);
	 	lngMet1+=(unsigned long)(tSqrBuf[20]>>4)*	((unsigned long)1000);
	 	lngMet1+=(unsigned long)(tSqrBuf[20]&0x0F)*((unsigned long)100);
	 	lngMet1+=(unsigned long)(tSqrBuf[21]>>4)*	((unsigned long)10);
	 	lngMet1+=(unsigned long)(tSqrBuf[21]&0x0F);
		flMeter1=lngMet1*0.1;
		ftoc(flMeter1);
		for(i=0;i<4;i++)
			flMet1[i]=c2f[i];
	 	lngMet2 =(unsigned long)(tSqrBuf[22]>>4)*	((unsigned long)1000000000);
	 	lngMet2+=(unsigned long)(tSqrBuf[22]&0x0F)*((unsigned long)100000000);
	 	lngMet2+=(unsigned long)(tSqrBuf[23]>>4)*	((unsigned long)10000000);
	 	lngMet2+=(unsigned long)(tSqrBuf[23]&0x0F)*((unsigned long)1000000);
	 	lngMet2+=(unsigned long)(tSqrBuf[24]>>4)*	((unsigned long)100000);
	 	lngMet2+=(unsigned long)(tSqrBuf[24]&0x0F)*((unsigned long)10000);
	 	lngMet2+=(unsigned long)(tSqrBuf[25]>>4)*	((unsigned long)1000);
	 	lngMet2+=(unsigned long)(tSqrBuf[25]&0x0F)*((unsigned long)100);
	 	lngMet2+=(unsigned long)(tSqrBuf[26]>>4)*	((unsigned long)10);
	 	lngMet2+=(unsigned long)(tSqrBuf[26]&0x0F);
		flMeter2=lngMet2*0.1;
		ftoc(flMeter2);
		for(i=0;i<4;i++)
			flMet2[i]=c2f[i];
    Fy2CanLst[0x00]=0x40;
    Fy2CanLst[0x01]=tSqrBuf[15];//???
    Fy2CanLst[0x02]=tSqrBuf[16];
    Fy2CanLst[0x03]=tSqrBuf[51];//??1
    Fy2CanLst[0x04]=tSqrBuf[52];//??2
		Fy2CanLst[0x05]=tSqrBuf[53];//??3
		Fy2CanLst[0x06]=tSqrBuf[54];//??4
		Fy2CanLst[0x07]=tSqrBuf[55];//??5
		Fy2CanLst[0x08]=0x81;
		Fy2CanLst[0x09]=tSqrBuf[56];//??6
		Fy2CanLst[0x0A]=tSqrBuf[57];//??7
		Fy2CanLst[0x0B]=tSqrBuf[58];//??8
		Fy2CanLst[0x0C]=tSqrBuf[59];//??9
		Fy2CanLst[0x0D]=tSqrBuf[27];//????1
		Fy2CanLst[0x0E]=tSqrBuf[28];//????2
		Fy2CanLst[0x0F]=tSqrBuf[29];//????3
		Fy2CanLst[0x10]=0x82;
		Fy2CanLst[0x11]=tSqrBuf[7];//???1
		Fy2CanLst[0x12]=tSqrBuf[8];//???2
		Fy2CanLst[0x13]=tSqrBuf[9];//???3
		Fy2CanLst[0x14]=tSqrBuf[10];//???4
		Fy2CanLst[0x15]=tSqrBuf[30];//????5
		Fy2CanLst[0x16]=tSqrBuf[31];//????6
		Fy2CanLst[0x17]=tSqrBuf[32];//????7
		Fy2CanLst[0x18]=0x83;
		Fy2CanLst[0x19]=tSqrBuf[11];//????1
		Fy2CanLst[0x1A]=tSqrBuf[12];//????2
		Fy2CanLst[0x1B]=tSqrBuf[13];//????3
		Fy2CanLst[0x1C]=tSqrBuf[14];//????4
		Fy2CanLst[0x1D]=tSqrBuf[33];//????1
		Fy2CanLst[0x1E]=tSqrBuf[34];//????2
		Fy2CanLst[0x1F]=tSqrBuf[35];//????3
		Fy2CanLst[0x20]=0x84;
		Fy2CanLst[0x21]=0;//????1
		Fy2CanLst[0x22]=0;//????2
		Fy2CanLst[0x23]=0;//????3
		Fy2CanLst[0x24]=0;//????4
		Fy2CanLst[0x25]=tSqrBuf[36];//????4
		Fy2CanLst[0x26]=tSqrBuf[37];//????5
		Fy2CanLst[0x27]=tSqrBuf[38];//????6
		Fy2CanLst[0x28]=0x85;
		Fy2CanLst[0x29]=flMet1[0];//???1
		Fy2CanLst[0x2A]=flMet1[1];//???2
		Fy2CanLst[0x2B]=flMet1[2];//???3
		Fy2CanLst[0x2C]=flMet1[3];//???4
		Fy2CanLst[0x2D]=tSqrBuf[45];//??1
		Fy2CanLst[0x2E]=tSqrBuf[46];//??2
		Fy2CanLst[0x2F]=0x55;//????:55H??;0AAH??
		Fy2CanLst[0x30]=0x86;
		Fy2CanLst[0x31]=flMet2[0];//???1
		Fy2CanLst[0x32]=flMet2[1];//???2
		Fy2CanLst[0x33]=flMet2[2];//???3
		Fy2CanLst[0x34]=flMet2[3];//???4
		Fy2CanLst[0x35]=tSqrBuf[47];//??3
		Fy2CanLst[0x36]=tSqrBuf[48];//??4
		Fy2CanLst[0x37]=tAdd;//??
		Fy2CanLst[0x38]=0xC0;
		Fy2CanLst[0x39]=tSqrBuf[3];//??1
		Fy2CanLst[0x3A]=tSqrBuf[4];//??2
		Fy2CanLst[0x3B]=tSqrBuf[5];//??3
		Fy2CanLst[0x3C]=tSqrBuf[6];//??4
		Fy2CanLst[0x3D]=0x55;//LstFyBuf[54];//
		Fy2CanLst[0x3E]=0xAA;//LstFyBuf[55];//
		FillLstCAN(id);//????
		flgLstTFTok=dualChNum;cntLstTFT=9;cntCanFb=8;canRtCnt=0;
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

void FillRtCAN(unsigned char id)
{
//	unsigned char ci=0;
	unsigned char t[10];
	float tFlDat;
	//float	rtSFV;
	unsigned long tLngDat;
	rtCAN_TxMsg[0].data[0]=0x40;
	rtCAN_TxMsg[1].data[0]=0x81;
	rtCAN_TxMsg[2].data[0]=0x82;
	rtCAN_TxMsg[3].data[0]=0x83;
	rtCAN_TxMsg[4].data[0]=0xC0;
	//????,msg[0].data[1~4],
	//tFlDat=(dSSLv+0.00005)*10000;
	//tFlDat=(can_rtSSL+0.00005)*10000;
	tFlDat=(canSSL[id]+0.00005)*10000;
	tLngDat=(unsigned long)tFlDat;
	t[0]=(unsigned char)(tLngDat/10000000);
	tLngDat=tLngDat %           10000000;
	t[1]=(unsigned char)(tLngDat/1000000);
	tLngDat=tLngDat %           1000000;
	t[2]=(unsigned char)(tLngDat/100000);
	tLngDat=tLngDat %           100000;
	t[3]=(unsigned char)(tLngDat/10000);
	tLngDat=tLngDat %           10000;
	t[4]=(unsigned char)(tLngDat/1000);
	tLngDat=tLngDat %           1000;
	t[5]=(unsigned char)(tLngDat/100);
	tLngDat=tLngDat %           100;
	t[6]=(unsigned char)(tLngDat/10);
	tLngDat=tLngDat %           10;
	t[7]=(unsigned char)tLngDat;
	rtCAN_TxMsg[0].data[1]=(t[0]<<4)+t[1];
	rtCAN_TxMsg[0].data[2]=(t[2]<<4)+t[3];
	rtCAN_TxMsg[0].data[3]=(t[4]<<4)+t[5];
	rtCAN_TxMsg[0].data[4]=(t[6]<<4)+t[7];
	//????,msg[0].data[5~6],1????,2???,1???
	//tFlDat=(can_rtWD+0.05)*10;
	tFlDat=dWD*10;//(can_rtWD)*10;
	tLngDat=(unsigned long)tFlDat;
	t[0]=0;
	if(dWD<0)//if(can_rtWD<0)
		t[0]=1;
	t[1]=(unsigned char)(tLngDat/100);
	tLngDat=tLngDat %           100;
	t[2]=(unsigned char)(tLngDat/10);
	tLngDat=tLngDat %           10;
	t[3]=(unsigned char)tLngDat;
	rtCAN_TxMsg[0].data[5]=(t[0]<<4)+t[1];
	rtCAN_TxMsg[0].data[6]=(t[2]<<4)+t[3];
 	//??,msg[0].data[7],msg[1].data[1~3],4???,4???
	//tFlDat=(can_rtMD+0.00005)*10000;
	tFlDat=(dMD+0.00005)*10000;
	tLngDat=(unsigned long)tFlDat;
	t[0]=(unsigned char)(tLngDat/10000000);
	tLngDat=tLngDat %           10000000;
	t[1]=(unsigned char)(tLngDat/1000000);
	tLngDat=tLngDat %           1000000;
	t[2]=(unsigned char)(tLngDat/100000);
	tLngDat=tLngDat %           100000;
	t[3]=(unsigned char)(tLngDat/10000);
	tLngDat=tLngDat %           10000;
	t[4]=(unsigned char)(tLngDat/1000);
	tLngDat=tLngDat %           1000;
	t[5]=(unsigned char)(tLngDat/100);
	tLngDat=tLngDat %           100;
	t[6]=(unsigned char)(tLngDat/10);
	tLngDat=tLngDat %           10;
	t[7]=(unsigned char)tLngDat;
	rtCAN_TxMsg[0].data[7]=(t[0]<<4)+t[1];
	rtCAN_TxMsg[1].data[1]=(t[2]<<4)+t[3];
	rtCAN_TxMsg[1].data[2]=(t[4]<<4)+t[5];
	rtCAN_TxMsg[1].data[3]=(t[6]<<4)+t[7];
  //???,msg[1].data[4~7],7???,1???
	rtCAN_TxMsg[1].data[4]=lstYFL[canChNum-1][0];
	rtCAN_TxMsg[1].data[5]=lstYFL[canChNum-1][1];
	rtCAN_TxMsg[1].data[6]=lstYFL[canChNum-1][2];
	rtCAN_TxMsg[1].data[7]=lstYFL[canChNum-1][3];
  //????,msg[2].data[1~4],7???,1???
	//tFlDat=(can_rtSFL+0.05)*10;
	tFlDat=(canSFLv[id])*10;//tFlDat=(dSFLv)*10;//tFlDat=(can_rtSFL)*10;
	tLngDat=(unsigned long)tFlDat;
	t[0]=(unsigned char)(tLngDat/10000000);
	tLngDat=tLngDat %           10000000;
	t[1]=(unsigned char)(tLngDat/1000000);
	tLngDat=tLngDat %           1000000;
	t[2]=(unsigned char)(tLngDat/100000);
	tLngDat=tLngDat %           100000;
	t[3]=(unsigned char)(tLngDat/10000);
	tLngDat=tLngDat %           10000;
	t[4]=(unsigned char)(tLngDat/1000);
	tLngDat=tLngDat %           1000;
	t[5]=(unsigned char)(tLngDat/100);
	tLngDat=tLngDat %           100;
	t[6]=(unsigned char)(tLngDat/10);
	tLngDat=tLngDat %           10;
	t[7]=(unsigned char)tLngDat;
	rtCAN_TxMsg[2].data[1]=(t[0]<<4)+t[1];
	rtCAN_TxMsg[2].data[2]=(t[2]<<4)+t[3];
	rtCAN_TxMsg[2].data[3]=(t[4]<<4)+t[5];
	rtCAN_TxMsg[2].data[4]=(t[6]<<4)+t[7];
  //????,msg[2].data[5~7],msg[3].data[1],????
	ftoc(dLJSv);
	rtCAN_TxMsg[2].data[5]=c2f[0];
	rtCAN_TxMsg[2].data[6]=c2f[1];
	rtCAN_TxMsg[2].data[7]=c2f[2];
	rtCAN_TxMsg[3].data[1]=c2f[3];
	//dSFLv=dLJSv_last-dLJSv_first;
	//tFlDat=(dSFLv)*10;
	//tLngDat=(unsigned long)tFlDat;
	//t[0]=(unsigned char)(tLngDat/10000000);
	//tLngDat=tLngDat %           10000000;
	//t[1]=(unsigned char)(tLngDat/1000000);
	//tLngDat=tLngDat %           1000000;
	//t[2]=(unsigned char)(tLngDat/100000);
	//tLngDat=tLngDat %           100000;
	//t[3]=(unsigned char)(tLngDat/10000);
	//tLngDat=tLngDat %           10000;
	//t[4]=(unsigned char)(tLngDat/1000);
	//tLngDat=tLngDat %           1000;
	//t[5]=(unsigned char)(tLngDat/100);
	//tLngDat=tLngDat %           100;
	//t[6]=(unsigned char)(tLngDat/10);
	//tLngDat=tLngDat %           10;
	//t[7]=(unsigned char)tLngDat;
	//rtCAN_TxMsg[2].data[5]=(t[0]<<4)+t[1];
	//rtCAN_TxMsg[2].data[6]=(t[2]<<4)+t[3];
	//rtCAN_TxMsg[2].data[7]=(t[4]<<4)+t[5];
	//rtCAN_TxMsg[3].data[1]=(t[6]<<4)+t[7];
  //????,msg[3].data[2~5],????
	
	rtCAN_TxMsg[3].data[2]=lstOilId[id][0];//nowOrdId[0];
	rtCAN_TxMsg[3].data[3]=lstOilId[id][1];//nowOrdId[1];
	rtCAN_TxMsg[3].data[4]=lstOilId[id][2];//nowOrdId[2];
	rtCAN_TxMsg[3].data[5]=lstOilId[id][3];//nowOrdId[3];
  //????,msg[3].data[6~7],msg[4].data[1~2],????+????
	rtCAN_TxMsg[3].data[7]=id+1;//u8PendNum[id];
	if(dualChNum==(id+1))	 
	{
		if(sys_STA==0)
			rtCAN_TxMsg[3].data[6]=0x00;
		else 
		if(((sys_STA>0)&&(sys_STA<=6))||(sys_STA==8))
			rtCAN_TxMsg[3].data[6]=0x55;
		else 
		if((sys_STA==7))
			rtCAN_TxMsg[3].data[6]=0xAA;
	}
	else
	{
			rtCAN_TxMsg[3].data[6]=0x00;
	}
	rtCAN_TxMsg[4].data[1]=0xAA; //0x55:KG 0xAA:L
	if(sys_Conn==1)
	rtCAN_TxMsg[4].data[2]=0xAA;
	if(sys_Conn==0)
	rtCAN_TxMsg[4].data[2]=0x55;
  //????,msg[4].data[3~6],???
	rtCAN_TxMsg[4].data[3]=0x00;
	rtCAN_TxMsg[4].data[4]=0x00;
	rtCAN_TxMsg[4].data[5]=mbdata[40+id]>>8;//??????,???1
	rtCAN_TxMsg[4].data[6]=mbdata[40+id]&0xFF;

}

//id???,0:??1;1:??2?
void FillFBCan(unsigned char id,unsigned char FbId)
{
	unsigned char i;

	switch(FbId)
	{
		case 1:
			FillRtCAN(id);
			cntCanFb=5;
			break;
		case 2:
			//TBD
			break;
		case 3://??????
			//
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x42;
			rtCAN_TxMsg[0].data[2]=id+1;
			cntCanFb=1;
			canRtCnt=0;
			break;
		case 4://??????
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x43;
			rtCAN_TxMsg[0].data[2]=0x55;
			rtCAN_TxMsg[0].data[3]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 0x84://??????
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x43;
			rtCAN_TxMsg[0].data[2]=0xAA;
			rtCAN_TxMsg[0].data[3]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 5://??????
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x44;
			rtCAN_TxMsg[0].data[2]=0x55;
			rtCAN_TxMsg[0].data[3]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 0x85://??????
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x44;
			rtCAN_TxMsg[0].data[2]=0xAA;
			rtCAN_TxMsg[0].data[3]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 6://????
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x46;
			rtCAN_TxMsg[0].data[2]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 7://??????
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x4F;
			rtCAN_TxMsg[0].data[2]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 8://??????
			for(i=0;i<8;i++)
				rtCAN_TxMsg[0].data[i]=0x00;
			rtCAN_TxMsg[0].data[1]=0x4C;
			rtCAN_TxMsg[0].data[2]=id+1;
			cntCanFb=1;canRtCnt=0;
			break;
		case 12://????
			//TBD
			break;
		case 13://????
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
	CAN_InitStructure.CAN_SJW = CAN_SJW_3tq;//CAN_SJW_3tq;

	/* CAN Baudrate = 50KBps (CAN clocked at 42MHz)*/
	CAN_InitStructure.CAN_BS1= CAN_BS1_13tq;//CAN_InitStructure.CAN_BS1= CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2= CAN_BS2_2tq;//CAN_InitStructure.CAN_BS2= CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler= 54;//CAN_InitStructure.CAN_Prescaler= 140;
//	CAN_InitStructure.CAN_BS1= CAN_BS1_2tq;//CAN_BS1_2tq;
//	CAN_InitStructure.CAN_BS1= CAN_BS1_14tq;//CAN_InitStructure.CAN_BS1= CAN_BS1_2tq;
//	CAN_InitStructure.CAN_BS2= CAN_BS2_7tq;//CAN_InitStructure.CAN_BS2= CAN_BS2_3tq;
//	CAN_InitStructure.CAN_Prescaler= 40;//CAN_InitStructure.CAN_Prescaler= 140;
//	CAN_InitStructure.CAN_BS2= CAN_BS2_3tq;//CAN_BS2_3tq;
//	CAN_InitStructure.CAN_Prescaler= 14;//14;
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

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_3tq;//CAN_SJW_1tq;

	/* CAN Baudrate = 50KBps (CAN clocked at 42MHz)*/
	CAN_InitStructure.CAN_BS1= CAN_BS1_13tq;//CAN_InitStructure.CAN_BS1= CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2= CAN_BS2_2tq;//CAN_InitStructure.CAN_BS2= CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler= 54;//CAN_InitStructure.CAN_Prescaler= 140;
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
	//RxMsgCnt=0;
}

/************************************************************************************/
/************************************CAN1********************************************/
/************************************************************************************/
void CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	//使能相关时钟
	RCC_AHB1PeriphClockCmd(CAN_CH1_PORT_RCC, ENABLE);//使能PORTB时钟	                   											 
	RCC_APB1PeriphClockCmd(CAN_CH1_DEVICE_RCC, ENABLE);//使能CAN1时钟	

	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin = CAN_CH1_DEVICE_RX| CAN_CH1_DEVICE_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(CAN_CH1_DEVICE_PORT, &GPIO_InitStructure);//初始化PA8,PA9

	//引脚复用映射配置
	GPIO_PinAFConfig(CAN_CH1_DEVICE_PORT,CAN_CH1_DEVICE_RX_PS,CAN_CH1_DEVICE_AF); //GPIOA8复用为CAN1
	GPIO_PinAFConfig(CAN_CH1_DEVICE_PORT,CAN_CH1_DEVICE_TX_PS,CAN_CH1_DEVICE_AF); //GPIOA9复用为CAN1
	
	//CAN单元设置
	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
	CAN_Init(CAN_CH1_DEVICE, &CAN_InitStructure);   // 初始化CAN1 
	
	//配置过滤器
	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
	
	CAN_ITConfig(CAN_CH1_DEVICE,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    

	NVIC_InitStructure.NVIC_IRQChannel = CAN_CH1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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

//void iCAN_wrMsg(iCAN_msg *msg)
//{
//	vu32 tId; 
//  if (0 != CAN_TxStatus_NoMailBox)
//  {
//  
//		CAN1->sTxMailBox[0].TIR &= (uint32_t)0x00000001;//TMIDxR_TXRQ;
//		
//	//CAN1->sTxMailBox[0].TIR  = (unsigned int)0;
//	tId=msg->SrcMacId<<24;
//	tId=tId | (msg->DestMacId<<16);
//	tId=tId |	(msg->ACK<<15);
//	tId=tId | (msg->FuncID<<11);
//	tId=tId | (msg->SourceID<<3); 
//	tId=tId | 4;//CAN_ID_EXT; 
//	CAN1->sTxMailBox[0].TIR=tId;
//	CAN1->sTxMailBox[0].TDLR = (((unsigned int)msg->data[3] << 24) | 
//	                         ((unsigned int)msg->data[2] << 16) |
//	                         ((unsigned int)msg->data[1] <<  8) | 
//	                         ((unsigned int)msg->data[0])        );
//	CAN1->sTxMailBox[0].TDHR = (((unsigned int)msg->data[7] << 24) | 
//	                         ((unsigned int)msg->data[6] << 16) |
//	                         ((unsigned int)msg->data[5] <<  8) |
//	                         ((unsigned int)msg->data[4])        );
//	                                              // Setup length
//	msg->len &= (uint8_t)0x0000000F;
//	CAN1->sTxMailBox[0].TDTR &= (uint32_t)0xFFFFFFF0;
//	CAN1->sTxMailBox[0].TDTR |= msg->len;
//	//CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT1R_DLC;
//	//CAN1->sTxMailBox[0].TDTR |=  (msg->len & CAN_TDT1R_DLC);

//	//CAN1->IER |= CAN_IER_TMEIE;
//	CAN1->sTxMailBox[0].TIR |=  ((uint32_t)0x00000001);//TMIDxR_TXRQ;//CAN_TI1R_TXRQ;
//	}
//}
//中断服务函数			    
void CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
	int i=0;
 	iCAN_rdMsg (&bufCAN_RxMsg[cntCAN_RxMsg]);
	cntCAN_RxMsg++;
	if(cntCAN_RxMsg>=49)
		cntCAN_RxMsg=0;
  CAN_RxRdy = 1;                                // set receive flag
 
	//CAN_Receive(CAN1, 0, &RxMessage);
	//for(i=0;i<8;i++)
	//;
		//处理接收到的数据
}

u8 CAN1_Send_Msg(u8 addrDest)
{	
  u8 mbox;
  u16 i=0;
  //CanTxMsg TxMessage;
	GKY_TxMsg.SrcMacId=3;
	GKY_TxMsg.DestMacId=addrDest;
	GKY_TxMsg.FuncID =9;
	GKY_TxMsg.SourceID =10;
	GKY_TxMsg.ACK =1;
	GKY_TxMsg.len =8;
//  for(i=0;i<8;i++) GKY_TxMsg.data[i]=0;				 // 第一帧信息          
	iCAN_wrMsg(&GKY_TxMsg);
  //mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		

}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}

/************************************************************************************/
/************************************CAN2********************************************/
/************************************************************************************/
void CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	//使能相关时钟
	RCC_AHB1PeriphClockCmd(CAN_CH1_PORT_RCC, ENABLE);//使能PORTB时钟	                   											 
	RCC_APB1PeriphClockCmd(CAN_CH1_DEVICE_RCC, ENABLE);//使能CAN1时钟	
	RCC_APB1PeriphClockCmd(CAN_CH2_DEVICE_RCC, ENABLE);//使能CAN1时钟	

	//CAN2
	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin = CAN_CH2_DEVICE_RX| CAN_CH2_DEVICE_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(CAN_CH2_DEVICE_PORT, &GPIO_InitStructure);//初始化PA8,PA9

	//引脚复用映射配置
	GPIO_PinAFConfig(CAN_CH2_DEVICE_PORT,CAN_CH2_DEVICE_RX_PS,CAN_CH2_DEVICE_AF); //GPIOA8复用为CAN1
	GPIO_PinAFConfig(CAN_CH2_DEVICE_PORT,CAN_CH2_DEVICE_TX_PS,CAN_CH2_DEVICE_AF); //GPIOA9复用为CAN1
	
	//CAN单元设置
	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
	CAN_Init(CAN_CH2_DEVICE, &CAN_InitStructure);   // 初始化CAN1 
	
	//配置过滤器
	CAN_FilterInitStructure.CAN_FilterNumber=14;	  //过滤器0
	CAN_FilterInitStructure.CAN_FilterMode =CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
	
	CAN_ITConfig(CAN_CH2_DEVICE,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    

	NVIC_InitStructure.NVIC_IRQChannel = CAN_CH2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}   
 
//中断服务函数			    
void CAN2_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
	int i=0;
  
	CAN_Receive(CAN1, 0, &RxMessage);
	for(i=0;i<8;i++)
	;
		//处理接收到的数据
}
//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
//u8 CAN2_Send_Msg(u8 addrDest)
//{	
//  u8 mbox;
//  u16 i=0;
//  //CanTxMsg TxMessage;
//	GKY_TxMsg.SrcMacId=3;
//	GKY_TxMsg.DestMacId=addrDest;
//	GKY_TxMsg.FuncID =2;
//	GKY_TxMsg.SourceID =10;
//	GKY_TxMsg.ACK =1;
//	GKY_TxMsg.len =8;
//  for(i=0;i<8;i++)
//		GKY_TxMsg.data[i]=0;				 // 第一帧信息          
//	iCAN_wrMsg(&GKY_TxMsg);
//  //mbox= CAN_Transmit(CAN1, &TxMessage);   
//  i=0;
//  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
//  if(i>=0XFFF)return 1;
//  return 0;		

//}
//u8 CAN2_Send_Msg(u8* msg,u8 len)
//{	
//  u8 mbox;
//  u16 i=0;
//  CanTxMsg TxMessage;
//  TxMessage.StdId=0x12;	 // 标准标识符为0
//  TxMessage.ExtId=0x12;	 // 设置扩展标示符（29位）
//  TxMessage.IDE=CAN_Id_Standard;		  //使用标准标识符
//  TxMessage.RTR=CAN_RTR_Data;		  // 消息类型为数据帧，一帧8位
//  TxMessage.DLC=len;							 // 帧长
//  for(i=0;i<len;i++)
//  TxMessage.Data[i]=msg[i];				 // 第一帧信息          
//  mbox= CAN_Transmit(CAN2, &TxMessage);   
//  i=0;
//  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
//  if(i>=0XFFF)return 1;
//  return 0;		
//}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN2_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}
void CAN_RECV(unsigned char id)
{
	unsigned char tSrcMacId=0;
	unsigned char tDestMacId=0;
//	unsigned char tAck=0;
	unsigned char tFuncId=0;
	unsigned char tSourceId=0;
	unsigned char ci;
	unsigned char temp;
		 //??
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
				case 2://???
				{
				//????????,??????,???5???
					if(tSourceId==10)
					{																	
						canChNum=iCAN_RxMsg.data[1];
						if(canChNum==1)
						{
							rtAi=ctof(iCAN_RxMsg.data[5],iCAN_RxMsg.data[4],iCAN_RxMsg.data[3],iCAN_RxMsg.data[2]);
					    flPress=rtAi;
							mbdata[22]=(unsigned int)(rtAi*1000.0);
						}
						//if(frmNum==FRM_MAIN)
						//{
						//	Dis_Num16(500,380,0,0xFFFF,tDestMacId);
						//	Dis_Num16(530,380,0,0xFFFF,tSrcMacId);
						//	Dis_Num16(560,380,0,0xFFFF,tFuncId);
						//	Dis_Num16(590,380,0,0xFFFF,tSourceId);
						//	Dis_Num16(500,400,0,0xFFFF,iCAN_RxMsg.data[0]);
						//	Dis_Num16(500+30,400,0,0xFFFF,iCAN_RxMsg.data[1]);
						//	Dis_Num16(500+60,400,0,0xFFFF,iCAN_RxMsg.data[2]);
						//	Dis_Num16(500+90,400,0,0xFFFF,iCAN_RxMsg.data[3]);
						//	Dis_Num16(500+120,400,0,0xFFFF,iCAN_RxMsg.data[4]);
						//	Dis_Num16(500+150,400,0,0xFFFF,iCAN_RxMsg.data[5]);
						//	//Dis_Num16(500+180,400,0,0xFFFF,iCAN_RxMsg.data[6]);
						//}
						if((flgLstTFTok==0)||(canChNum!=flgLstTFTok))
						{		
							for(ci=0;ci<5;ci++)
							{
								rtCAN_TxMsg[ci].len				=8;
								rtCAN_TxMsg[ci].SrcMacId	=tDestMacId;
								rtCAN_TxMsg[ci].DestMacId	=tSrcMacId;
								rtCAN_TxMsg[ci].ACK				=0;
								rtCAN_TxMsg[ci].FuncID		=2;
								rtCAN_TxMsg[ci].SourceID	=0x0A;
							}
							 if((sys_STA==0))
							 {  canSSL[0]=dSSLv;canSSL[1]=dSSLv;}
							 if((sys_STA>0)&&(sys_STA<9))
							 {
								 	if(dualChNum==1)
									{
									   canSSL[0]=dSSLv;canSSL[1]=0.0;
										 canSFLv[0]=dSFLv;//canSFLv[1]=0.0;
									}
								 	if(dualChNum==2)
									{
									   canSSL[1]=dSSLv;canSSL[0]=0.0;
										 canSFLv[1]=dSFLv;//canSFLv[0]=0.0;
									}
								}
								can_rtSFL=dSFLv;
								can_rtWD =dWD;//dWD[id];//rtWD
								can_rtMD =dMD;//dMD[id];//rtMD
								can_rtLJS=dLJSm;//dLJS[id];
								can_rtYFL=YFL[canChNum-1];//lng_KgNeed[id];
								//intSFVmbH=mbdata[id*100+24];
								//intSFVmbL=mbdata[id*100+25];
								if(canChNum>0)
									FillRtCAN(canChNum-1);
								//canEnSendRt=1;
								flgCanFb=1;
								cntCanFb=5;
								canRtCnt=0;
							}
						if(canChNum==flgLstTFTok)
						{
							FillLstCAN(id);
							flgCanFb=0xFF; 
							cntCanFb=8;canRtCnt=0;
						}
					}
				}break;
				case 4:break;//????
				case 5:break;//????
				case 6:break;//????
				case 7:break;//MacID????
				case 8://????
				{
					if(tSourceId==0x01)//?????+????B,fb5?//??
					{
						flgCanFb=0x82;cntCanFb=5;canRtCnt=0;		
						break;
					}
					if(tSourceId==3)	 //??????,fb??
					{
						canChNum=iCAN_RxMsg.data[3];
						if(iCAN_RxMsg.data[2]==0x42)
						{
							if(iCAN_RxMsg.data[1]==0x55)
							{
								sys_Conn=0;WarnOnOff[10]=0;tmpConfWarn[10]=0;
								EepRd(210,1);
								EepWrBuf[0]=EepRdBuf[0]&0xBF;
								EepWr(210,1);
								flgCanFb=3;cntCanFb=1;canRtCnt=0;
							}
							if(iCAN_RxMsg.data[1]==0xAA)
							{
								sys_Conn=1;WarnOnOff[10]=1;tmpConfWarn[10]=1;
								EepRd(210,1);
								EepWrBuf[0]=EepRdBuf[0]|0x40;
								EepWr(210,1);
								flgCanFb=3;cntCanFb=1;canRtCnt=0;
							}
						}
						break;
					}
					if(tSourceId==4)   //??????,fb??
					{
						//????3??,?????:0-???;1-?1???;2-?2???;0xff-????,????
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
							canChNum=iCAN_RxMsg.data[4];
							addChNum=canChNum;
							for(ci=1;ci<8;ci++)
								CanRxdBuf[ci+13]=iCAN_RxMsg.data[ci];
							if(CanRxdBuf[0x19]==2)
								CanRxdBuf[0x19]=3;
							else 
								CanRxdBuf[0x19]=0;
							if(CanRxdBuf[0x19]==3)
							{
							//0..3:????,4:??,5..8:???,9..12:??:13..14:??:15:????	--??
							//0..3:????,4:??,5..8:???,9..12:??:13..14:??:15:????
								CanRxdBuf[0x19]=0;
								for(ci=0;ci<22;ci++)
									NewPendBuf[ci]=CanRxdBuf[ci];
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
					if(tSourceId==5)   //????,fb??
					{
						canChNum=iCAN_RxMsg.data[7];
						delChNum=canChNum; 
						DelPendBuf[9]=0x1;
						DelPendBuf[0]=(iCAN_RxMsg.data[1]>>4)+'0';
						DelPendBuf[1]=(iCAN_RxMsg.data[1]&0x0F)+'0';
						DelPendBuf[2]=(iCAN_RxMsg.data[2]>>4)+'0';
						DelPendBuf[3]=(iCAN_RxMsg.data[2]&0x0F)+'0';
						DelPendBuf[4]=(iCAN_RxMsg.data[3]>>4)+'0';
						DelPendBuf[5]=(iCAN_RxMsg.data[3]&0x0F)+'0';
						DelPendBuf[6]=(iCAN_RxMsg.data[4]>>4)+'0';
						DelPendBuf[7]=(iCAN_RxMsg.data[4]&0x0F)+'0';
						DelPendBuf[8]=iCAN_RxMsg.data[6];
						if(sys_STA==0)
						{
							DelPendBuf[9]=0xFA;	//????
							flgCanFb=0x05;cntCanFb=1;canRtCnt=0;
						}
						else
						{
							flgCanFb=0x85;cntCanFb=1;canRtCnt=0;
						}
						break;
					}
					if(tSourceId==6)		//????,fb??
					{
						canChNum=iCAN_RxMsg.data[7];
						temp=iCAN_RxMsg.data[1];
						sysY=(temp>>4)*10+(temp&0x0F);
						temp=iCAN_RxMsg.data[2];
						sysM=(temp>>4)*10+(temp&0x0F);
						temp=iCAN_RxMsg.data[3];
						sysD=(temp>>4)*10+(temp&0x0F);
						temp=iCAN_RxMsg.data[4];
						sysHH=(temp>>4)*10+(temp&0x0F);
						temp=iCAN_RxMsg.data[5];
						sysMM=(temp>>4)*10+(temp&0x0F);
						temp=iCAN_RxMsg.data[6];
						sysSS=(temp>>4)*10+(temp&0x0F);

						flgCanFb=6;cntCanFb=1;canRtCnt=0;
						flgWrRtc=1;
						break;				
					}
					if(tSourceId==7)    //??????,fb??
					{
						canChNum=iCAN_RxMsg.data[3];
						flgCanFb=7;cntCanFb=1;canRtCnt=0;
						if(frmNum==FRM_MAIN)
						{
							if((sys_STA>0)&&(sys_STA<6)&&(iCAN_RxMsg.data[1]==3)&&(canChNum==dualChNum))//????
							{
								flgRmCon=2;ubKey_Command=0xff;
								//DAC_Out(AoDat);
								AoDat=0;
								sys_STA=7;flgSysWarn=2;flgAlert[6]=1;flgWrWarnLst=1;
							}
							if((flgFyAllow==1)&&(iCAN_RxMsg.data[1]==4)&&(canChNum==dualChNum))//????
							{
								flgRmCon=1;ubKey_Command=0xff;
							}
						}
						break;
					}							 
					if(tSourceId==8)		//??????,fb??
					{
						if(iCAN_RxMsg.data[0]==0x40)
						{
							SavOilId[14]=1;
							for(ci=1;ci<8;ci++)
								SavOilId[ci-1]=iCAN_RxMsg.data[ci];
						}
						if(iCAN_RxMsg.data[0]==0xC0)
						{
							canChNum=iCAN_RxMsg.data[5];
							SavOilId[7]=iCAN_RxMsg.data[1];
							SavOilId[8]=iCAN_RxMsg.data[2];
							SavOilId[9]=iCAN_RxMsg.data[3];
							if(SavOilId[14]==1)
							{	SavOilId[14]=0xFA;
								flgCanFb=8;cntCanFb=1;canRtCnt=0;}
						}
						break;
					}
					if(tSourceId==10)		//????????????,fb1?
					{
						canChNum=iCAN_RxMsg.data[6];
						flgSqrLst=1;
						tSqlBuf[0]='0'+(iCAN_RxMsg.data[1]>>4);
						tSqlBuf[1]='0'+(iCAN_RxMsg.data[1]&0x0F);
						tSqlBuf[2]='0'+(iCAN_RxMsg.data[2]>>4);
						tSqlBuf[3]='0'+(iCAN_RxMsg.data[2]&0x0F);
						tSqlBuf[4]='0'+(iCAN_RxMsg.data[3]>>4);
						tSqlBuf[5]='0'+(iCAN_RxMsg.data[3]&0x0F);
						tSqlBuf[6]='0'+(iCAN_RxMsg.data[4]>>4);
						tSqlBuf[7]='0'+(iCAN_RxMsg.data[4]&0x0F);
						tSqlBuf[8]=0;
						IWDG_ReloadCounter();
						rtnLos=Fnd_Pwd_Lst(tSqlBuf);
						if(rtnLos<=199) //??????
						{	
							IWDG_ReloadCounter();
							gSqlBuf=Get_Pwd_Lst(rtnLos);
							Fill_Sqr_Lst(canChNum-1,gSqlBuf);
						 	flgCanFb=10;
						}
						else//??????
						{
							IWDG_ReloadCounter();
						 	tmpDat=Fnd_Pend(canChNum-1,tSqlBuf);//0x00??????,0xAA????
						 	//Fill_No_Lst(tSqlBuf,tmpDat);
							RtnNoLst(tSqlBuf,tmpDat);
						
						}
						//flgCanFb=9;cntCanFb=1;canRtCnt=0;
						break;
					}
					if(tSourceId==11)		//??????,fb2?
					{
						//TBD
					 	break;
					}
					if(tSourceId==12)		//??????,fb3?
					{
						//TBD
					 	break;
					}
					if(tSourceId==13)		//????,fb??
					{
						canChNum=iCAN_RxMsg.data[2];
					 	flgCanFb=13;
						break;
					}
					if(tSourceId==2)		//??????????
					{
						canChNum=iCAN_RxMsg.data[7];
						CanRxdBuf[0x3A]=0xFA;//???,??????
						if(canChNum==flgLstTFTok)
						{
							flgLstTFTok=0;
						}
						break;
					}
				}		
			}
				FillFBCan(canChNum-1,flgCanFb);
}
//unsigned char   SrcMacId;           // 29 bit identifier
//unsigned char   DestMacId;
//unsigned char   ACK;
//unsigned char   FuncID;
//unsigned char   SourceID;
//unsigned char   data[8];            // Data field
//unsigned char   len;                // Length of data field in bytes
void CAN_EVENTS(void)
{
	unsigned char i,j;
	unsigned char tAddr;
	unsigned char tRegAddr;
	unsigned char tDatLen;
	if(CAN_RxRdy) 
	{
	  CAN_RxRdy = 0;
		for(i=0;i<cntCAN_RxMsg;i++)
		{
			datCAN_RxMsg[i].SrcMacId	=bufCAN_RxMsg[i].SrcMacId ;
			datCAN_RxMsg[i].DestMacId	=bufCAN_RxMsg[i].DestMacId  ;
			datCAN_RxMsg[i].ACK 			=bufCAN_RxMsg[i].ACK ;
			datCAN_RxMsg[i].FuncID  	=bufCAN_RxMsg[i].FuncID  ;
			datCAN_RxMsg[i].SourceID  =bufCAN_RxMsg[i].SourceID  ;
			datCAN_RxMsg[i].len  			=bufCAN_RxMsg[i].len  ;
			for(j=0;j<8;j++)
				datCAN_RxMsg[i].data[j]	=bufCAN_RxMsg[i].data[j];
			if(datCAN_RxMsg[i].FuncID ==9)//查询实时数据
			{
				tAddr=datCAN_RxMsg[i].SrcMacId-5;
				switch(datCAN_RxMsg[i].data[0])
				{
					case 0x40:tRegAddr= 0;bufCanData[tAddr][103]|=0x01;break;
					case 0x81:tRegAddr= 7;bufCanData[tAddr][103]|=0x02;break;
					case 0x82:tRegAddr=14;bufCanData[tAddr][103]|=0x04;break;
					case 0x83:tRegAddr=21;bufCanData[tAddr][103]|=0x08;break;
					case 0x84:tRegAddr=28;bufCanData[tAddr][103]|=0x10;break;
					case 0x85:tRegAddr=35;bufCanData[tAddr][103]|=0x20;break;
					case 0x86:tRegAddr=42;bufCanData[tAddr][103]|=0x40;break;
					case 0x87:tRegAddr=49;bufCanData[tAddr][103]|=0x80;break;
					case 0x88:tRegAddr=56;bufCanData[tAddr][102]|=0x01;break;
					case 0x89:tRegAddr=63;bufCanData[tAddr][102]|=0x02;break;
					case 0x8A:tRegAddr=70;bufCanData[tAddr][102]|=0x04;break;
					case 0x8B:tRegAddr=77;bufCanData[tAddr][102]|=0x08;break;
					case 0x8C:tRegAddr=84;bufCanData[tAddr][102]|=0x10;break;
					case 0x8D:tRegAddr=91;bufCanData[tAddr][102]|=0x20;break;
					case 0xC0:tRegAddr=98;bufCanData[tAddr][102]|=0x40;break;
				}
				if(datCAN_RxMsg[i].data[0]==0x40)
				{	tRegAddr=0;tDatLen=7;	}
				if((datCAN_RxMsg[i].data[0]>0x80)&&(datCAN_RxMsg[i].data[0]<0x8E))
				{	tRegAddr=0+(datCAN_RxMsg[i].data[0]-0x80)*7;tDatLen=7;}
				if(datCAN_RxMsg[i].data[0]==0xC0)
				{	tRegAddr=98;tDatLen=3;}
				for(j=0;j<tDatLen;j++)
				{
					bufCanData[tAddr][tRegAddr+j]=datCAN_RxMsg[i].data[j+1];
				}
				if((bufCanData[tAddr][102]==0x7F)&&(bufCanData[tAddr][103]==0xFF))
				{
					bufCanData[tAddr][102]=0;cntCAN_RxMsg=0;
					for(j=0;j<101;j++)
					{
						CanData[tAddr][40+j]=bufCanData[tAddr][j];
					}
					for(j=0;j<34;j++)
					{
						MB[2*tAddr][40+j]=bufCanData[tAddr][j];
						MB[2*tAddr+1][40+j]=bufCanData[tAddr][j];
					}
				}
			}
		}
		//
	}
}
void CAN_EVENTS_(void)
{
	unsigned char tSrcMacId=0;
	unsigned char tDestMacId=0;
	unsigned char i;
	if(CAN_RxRdy) 
	{
	  CAN_RxRdy = 0;
		for(i=0;i<cntCAN_RxMsg;i++)
		{
			datCAN_RxMsg[i].SrcMacId=bufCAN_RxMsg[i].SrcMacId ;
		}
		cntCAN_RxMsg=0;
		tSrcMacId=iCAN_RxMsg.SrcMacId;
		tDestMacId=iCAN_RxMsg.DestMacId;
		//tAck=iCAN_RxMsg.ACK;
		//tFuncId=iCAN_RxMsg.FuncID;
		//tSourceId=iCAN_RxMsg.SourceID;
		//?????4???,???
		if(tSrcMacId==3)
		{
			//if((tDestMacId==set_AddCa)||(tDestMacId==set_AddCa+1)) //??
			if((tDestMacId==set_AddCa))//??
			{
			  	CAN_RECV(0);//(tDestMacId-set_AddCa);
			}
		}
	}
	if(flgCanFb>0)
	{
		if(canRtCnt<cntCanFb)
		{
			CAN_waitReady();
			if(CAN_TxRdy==1)
			{
				SendRtCAN(canRtCnt);
				canRtCnt++;
				if(canRtCnt==cntCanFb)
				{	
					flgCanFb=0;
					cntCanFb=0;
					if((flgLstTFTok>0)&&(flgLstTFTok==canChNum))//???????,?????,TBD!!!!!!
					{
				  	cntLstTFT++;
						if(cntLstTFT>9)
						{	cntLstTFT=0; 	flgLstTFTok=0; }
					}
				}
			}
		 }
	 }
}












