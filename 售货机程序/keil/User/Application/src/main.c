//20170101 first ver

#include <stdbool.h>
#include <stdlib.h>
//
#include "Global.h"
#include "stm32f4xx.h"
#include "systick.h"
#include "color.h"
#include "CAN.h"
#include "GUI.h"
#include "KeyPad.h"
#include "GTFont.h"
#include "LCD.h"
#include "Master.h"
#include "eep.h"
#include "mem.h"
#include "App.h"
#include "RTC.h"
//W5500 START
#define Sn_RXMEM_SIZE(ch)               (0x001E08 + (ch<<5))
#define Sn_TXMEM_SIZE(ch)               (0x001F08 + (ch<<5))
//uint32_t start = 58070*142+4;
//uint32_t num = 58070 ;
//uint8_t tLogTime[500];
//uint8_t rev_buf[2000];
//unsigned char txsize[8] = {2,2,2,2,2,2,2,2};
//unsigned char rxsize[8] = {2,2,2,2,2,2,2,2};

//W5500 END
unsigned char flg1ms=0;//1ms标志，调节时间
unsigned int ub10ms=0;
//unsigned int ub250ms=0;
//unsigned char flg250ms;
unsigned char sysSavDat[10];
char disAddrBuf[3];
unsigned char AddrIfix,AddrIfix_pre;
extern void Dis_Lcd(void);
extern void btnHandler(void);//????
extern void CANHardwareConfig(void);
extern void prvKeyboardHandler(void);
extern void KeyPad_Config(void);
extern void padHandler(void);
extern void Dis_Num_8b(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint32_t Dat);
extern void Draw_Button_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen);
extern void U6_EN(unsigned char tDat);
unsigned char u4RxdBuf[100],u4TxdBuf[10];
unsigned int  u4RxdCnt,u4TxdCnt;
extern void SendU1Dat(unsigned char u1Dat);
extern void SendU6Dat(unsigned char u6Dat);
extern void SendValv(unsigned char id);
extern void CAN_EVENTS(void);

//unsigned char frmNum;
//unsigned int  tickCnt=0;
//
unsigned long Hz[1000],HzCnt;

//extern unsigned int u2Tmr;
//extern unsigned char u2RxdCnt,u2RxdBuf[20],datMod2Buf[200],datMod3Buf[200];
//extern unsigned char flgMbCrcOk,flgMb2CrcOk,flgMb3CrcOk;

void GPIO_CONFIG(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE , ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	//ARMKEY7,8
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15| GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
}
void TIM_Config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4 , ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Period = 9; 					//间隔时间为1毫秒=（（1+TIM_Prescaler）/72M）×（1+TIM_Period）
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	 	// 定时器基准频率36MHz
	TIM_TimeBaseStructure.TIM_Prescaler = 719;			// 计数频率为1KHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	// 向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	//TIM_TimeBaseStructure.TIM_Period = 3999; 				// TIMx->ARR，延时开始时重新设定即可
	TIM_TimeBaseStructure.TIM_Period = 55; 							// TIMx->ARR，延时开始时重新设定即可
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Period = 49; 					//间隔时间为1毫秒=（（1+TIM_Prescaler）/72M）×（1+TIM_Period）
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	

	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_ARRPreloadConfig(TIM4,DISABLE);
	TIM_Cmd(TIM4,ENABLE);

	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ARRPreloadConfig(TIM3,DISABLE);
	TIM_Cmd(TIM3,ENABLE);

	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ARRPreloadConfig(TIM2,DISABLE);
	TIM_Cmd(TIM2,ENABLE);

    //配置TIM中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    //配置TIM中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  //配置TIM中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
unsigned long Span[1000];
unsigned char flgHz;
unsigned int cntOpDo;
unsigned int OpDoCNT=200;
extern unsigned char u2Flag,u2Cnt,u2SndCnt,u2MbBuf[100];
extern unsigned char u3Flag,u3Cnt,u3SndCnt,u3MbBuf[100];
extern unsigned char u4Flag,u4Cnt,u4SndCnt,u4MbBuf[100];
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(u1Flag==1)
		{
			SendU1Dat(u1MbBuf[u1Cnt]);
			u1Cnt++;
			if(u1Cnt>=11)
			{
				u1Flag=0;
				u1SndCnt=0;
				u1Cnt=0;		
			}
			
		}
		if(u6Flag==1)
		{
			SendU6Dat(u6MbBuf[u6Cnt]);
			u6Cnt++;
			if(u6Cnt>8)
			{
				u6Flag=0;
				u6SndCnt=0;
				u6Cnt=0;		
				GPIO_SetBits(GPIOD,GPIO_Pin_13);//U6_EN(0);
			}
		}

			if(u4Flag)
			{
				SendU4Dat(u4MbBuf[u4Cnt]);
				u4Cnt++;
				if(u4Cnt>u4SndCnt)
				{
					 u4Flag=0;
					 u4SndCnt=0;
					 u4Cnt=0;		
					 GPIO_ResetBits(GPIOE,GPIO_Pin_2);//SetU4Recv();
				}
			}
			if(u2Flag)
			{
				SendU2Dat(u2MbBuf[u2Cnt]);
				u2Cnt++;
				if(u2Cnt>u2SndCnt)
				{
					u2Flag=0;
					u2SndCnt=0;
					u2Cnt=0;		
					GPIO_ResetBits(GPIOE,GPIO_Pin_3);//SetU3Recv();
				}
			}
//			if(u3Flag)
//			{
//				SendU3Dat(u3MbBuf[u3Cnt]);
//				u3Cnt++;
//				if(u3Cnt>u3SndCnt)
//				{
//					u3Flag=0;
//					u3SndCnt=0;
//					u3Cnt=0;		
//					GPIO_SetBits(GPIOA,GPIO_Pin_1);//GPIO_ResetBits(GPIOC,GPIO_Pin_4);//SetU3Recv();
//				}
//			}
	}	
}

//1ms
void TIM3_IRQHandler(void)
{
	unsigned char i;
	unsigned char tDatLen;
//	unsigned char mbLen;
	u16 crcresult,tcrc3;
	u8  crcHi=0,tcrch3,crcLo=0;//;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		sysTick++;
		tickCnt++;
		refCnt++;
		tickTmrOut++;	tmrU6++;tDatInt++;
		if((flgEnd==1)&&(dSSLv<0.1))
			cntEndTmr++;
		if(++sys_Cnt>=50)
		{
			sys_Cnt=0;
			sys_adj50ms=1;
		}
		if(++tickTmrOut>500)
		{
			flgFlash=0;
			if(tickTmrOut>1000)
				tickTmrOut=0;
		}
		else
		{
			flgFlash=1;
		}
		if(++tickCnt>=500)
		{
			tickCnt=0;
			flg1S=1;
		}
		if(++ub10ms >= 20)
		{
		 	ub10ms=0;
			flg1ms=1;
		}
		if(++ub250ms >= 550)
		{
			 ub250ms=0;
			flg250ms=1;
		}
//		//流量计通讯
		if((tmrU6>50)&&(u6RxdCnt>5))//超时，处理MODBUS
		{
			if(u6RxdCnt==(u6Buf[2]+4))
			{
			 	//数据长度正确
				u6RxdCnt=0;
			}
			u6RxdCnt=0;
			tDatLen=u6Buf[2]+4;
			for(i=0;i<=tDatLen;i++)
				u6FvBuf[i]=u6Buf[i];
			crcresult = CRC16(u6FvBuf,tDatLen-1);
			crcHi=(crcresult & 0xff);
			crcLo=(crcresult & 0xFF00)>>8;
			if((crcHi==u6FvBuf[tDatLen-1])&&(crcLo==u6FvBuf[tDatLen]))
				//modbus 数据正确
			{	flgFvMbCrcOk=1;cntFVconnTmr=0;err_FV=0;flgAlert[2]=0;}//FvSqrWaitCnt=0;
			else
			{	flgFvMbCrcOk=0;}
			for(i=0;i<100;i++)
			{
				 u6Buf[i]=0;
			} 
			u6RxdCnt=0;
		}
		if(tDatInt>800)
		{
			u6RxdCnt=0;flgFvMbTmrOut=1;tDatInt=0;
		}
		
	}	
}

void TIM4_IRQHandler(void)
{
	unsigned char i;
	unsigned char tDatLen;
//	unsigned char mbLen;
	u16 crcresult,tcrc3;
	u8  crcHi=0,tcrch3;//,crcLo=0;

	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		//u2Tmr++;u3Tmr++; 
//		if(u3Flag==1)
//		{
//			SendU3Dat(u3MbBuf[u3Cnt]);
//			u3Cnt++;
//			if(u3Cnt>8)
//			{
//				u3Flag=0;
//				u3SndCnt=0;
//				u3Cnt=0;		
//				GPIO_SetBits(GPIOA,GPIO_Pin_1);//U6_EN(0);
//			}
//			
//		}
	}	
}


void Rtc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* PB 6,为输出*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;  //IO????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* PD 3,为RTC RST输出*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//PB7为DAT，如果配置为开漏模式则需要外接上拉电阻，此模式下可以实现真正的双向IO
	//由于硬件上无上拉电阻，故手动设置输入和输出模式
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//读FLASH数据，
void ReadFlash(void)
{
	unsigned char ti,tj;
	unsigned int  tInt;
	unsigned char dSavDat[100];
	//unsigned char tDat;
	//EEP_Read(FlashBuf,10,130);
	//SPI2_Flash_Read(FlashBuf,0,130);
	//tInt=GetPwrCnt();
	EepRd(2,32);//用户名
	for(ti=0;ti<8;ti++)
		for(tj=0;tj<4;tj++)
			SavUsr[ti][tj]=EepRdBuf[ti*4+tj];
	EepRd(66,48);//密码
	for(ti=0;ti<8;ti++)
		for(tj=0;tj<6;tj++)
			SavPwd[ti][tj]=EepRdBuf[(ti*6)+tj];
	EepRd(130,32);//发油参数
	for(ti=0;ti<32;ti++)
		dSavDat[ti]=EepRdBuf[ti];
	for(ti=0;ti<16;ti++)
	{
		tInt=(dSavDat[2*ti]<<8)+dSavDat[2*ti+1];
		SavFyDat[ti]=tInt;
	}
	EepRd(194,17);//设置参数
	for(ti=0;ti<17;ti++)
			SavConfDat[ti]=EepRdBuf[ti];
	sys_ConnLast=0;
	//组态参数
	//    D7    D6
	//
	if((SavConfDat[16]&0x40)==0x40)
	{
	 	sys_Conn=1;
	}
	else
	{
	  sys_Conn=0;
	}
	EepRd(224,5);//关阀参数
	for(ti=0;ti<5;ti++)
	{
		if((EepRdBuf[ti]<=dnSavConfDatMax[ti])&&(EepRdBuf[ti]>=dnSavConfDatMin[ti]))
		{
			 dnSavConfDat[ti]=EepRdBuf[ti];
		}
	}
	EepRd(234,2);
	tInt=EepRdBuf[0];
	tInt=(tInt<<8)+EepRdBuf[1];
	if((tInt>=pipeSavConfDatMin)&&(tInt<=pipeSavConfDatMax))
	{ pipeSavConfDat=tInt;}
	else
	{ pipeSavConfDat=80;}
	EepRd(220,4);//脉冲发油参数
		plzSavConfDat[0]=(EepRdBuf[0]<<8)+EepRdBuf[1];
		plzSavConfDat[1]=(EepRdBuf[2]<<8)+EepRdBuf[3];
//if(WarnOnOff[9]==1)
//{dMD[0]=(float)plzSavConfDat[0]/10000;}
//rtPLZk=(float)plzSavConfDat[1]/1000;
	
	if(rtPLZk<0.01)
	{	  rtPLZk=1.0;	 	}
	EepRd(211,10);//油品编码//油品编码存储位置改变
	for(ti=0;ti<10;ti++)
			OilCode[ti]=EepRdBuf[ti];
	sys_GoodsId=OilCode[9];
	
	set_SpOp=SavFyDat[0];			//开阀速率:20%/s    	00~99%/s
	set_SpCl=SavFyDat[1];			//关阀速度:20%/s    	00~99%/s
	set_FUp=SavFyDat[2];			//开启稳流量:10Kg/s  	00000~99999Kg/s
	set_FDn=SavFyDat[3];			//关闭稳流量: 1Kg/s  	00000~99999Kg/s
	set_FMx=SavFyDat[4];			//最大流量:	 40Kg/s   00000~99999Kg/s
	set_Ferr=SavFyDat[5];			//流量偏差:  (0~30)%  
	set_MDn=SavFyDat[6];			//提前量:		200Kg     000~999Kg
	set_MOv=SavFyDat[7];			//过冲量:    20Kg  		000~999Kg
	set_TOpBump=SavFyDat[9];	//提前开泵时间:	0			00~99s  
	set_TClBump=SavFyDat[10];	//延迟关泵时间:	0			00~99s
	set_Tsta=SavFyDat[9];		//开稳流时间:	10S				00~99s
	set_Noise=SavFyDat[12];flNoise=0.1*set_Noise;		//小信号切除
	set_ClValveTmr=SavFyDat[13];	//关阀超时
	set_OpValveTmr=SavFyDat[14];	//开阀超时
	set_FVtmr=SavFyDat[15];			//流量计超时
	bl_LCD=SavFyDat[16];
	if(bl_LCD<SavFyDatMin[16])bl_LCD=SavFyDatMin[16];
	if(bl_LCD>SavFyDatMax[16])bl_LCD=SavFyDatMax[16];
	set_AddMb=SavConfDat[0];			//modbus地址
	set_AddCa=SavConfDat[1];			//can地址
	set_KNeed=SavConfDat[2];		//修正系数:			0.900~1.100
	set_BNeed=SavConfDat[3];		//修正值:				00~99Kg
	conf_YL_Scale=SavConfDat[4];
	conf_YL_LmtUp=SavConfDat[5];
	conf_YL_LmtDn=SavConfDat[6];
	conf_MD_LmtUp=SavConfDat[7];
	conf_MD_LmtDn=SavConfDat[8];
	conf_FV_LmtDn=SavConfDat[9];
	Press_Up_Lmt=conf_YL_LmtUp;
	Press_Up_Lmt/=10.0;
	Press_Dn_Lmt=conf_YL_LmtDn;
  Press_Dn_Lmt/=10.0;
	Md_Up_Lmt= conf_MD_LmtUp;
	Md_Up_Lmt/=10.0;
	Md_Dn_Lmt= conf_MD_LmtDn;
	Md_Dn_Lmt/=10.0;
	Flow_On_Lmt=conf_FV_LmtDn;
	Flow_On_Lmt/=10.0;
	set_Config=SavConfDat[16];			//组态参数
	getWarnOnOff();
}

extern CanTxMsg TxMsg;
extern unsigned char frmNum;
extern void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
extern void Dis_Num16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint16_t Dat);
extern void blADJ(uint8_t tBL);

unsigned char tPwm=0;
unsigned long intDat[1000];
unsigned long sumHz,sumCnt;
unsigned long lngHz;
float flHz;
unsigned char idPKQ=1;
extern unsigned char flgPkqDatOk;
extern PKQ_MSG PKQ_Msg[14];
extern unsigned char flgU5Stx;
extern unsigned char flgU4StSend;
extern unsigned long lngWeiDat;
unsigned char wrFtaDoDat=0,wrFtaDoDat_pre;
unsigned int tmrUp=0;//上位机通讯超时
unsigned char flgDO[8];
unsigned int  cntDO[8];
unsigned char FTAdat=0;
unsigned char tBLdat;
extern void EepRd(unsigned int ReadAddr,unsigned int NumByteToWrite);
extern void EepWr(unsigned int ReadAddr,unsigned int NumByteToWrite);
extern void sysTickInit(void);
extern unsigned char EepWrBuf[200];
extern unsigned char EepRdBuf[200];
static u32 CpuID[3];



extern float FvDat[10];


//
int main(void)
{
	SystemInit();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	KeyHardwareConfig();
	sysTickInit(); 
	U6_Init();
	U5_Init();
	U4_Init();
	U1_Init();
	U2_Init();
	U3_Init();
	
	GUI_Init();
	CANHardwareConfig();

  TIM_Config();//TIM2_ICConfig();
	TFT_Fill( 0, 0,799,479,0xFFFF);
	Dis_Lcd();ftaDIdat_pre=0xFF;ftaDOdat_pre=0xFF;FvId=0;

  while(1)
  {
		
  }
}














