#include "App.h"
#include "Global.h"
extern void ShowStr30(unsigned int x,unsigned int y,char *p,unsigned char slen,unsigned int ForeColor,unsigned int BakColor);
extern void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
extern void Draw_AoBar(unsigned int tvalue);
extern unsigned char AddWarnLst(unsigned char id,unsigned char tAddWarnId);
extern void Dis_EN_FY(unsigned char val);

void ReIO(void)
{
	unsigned char i;
	{
		flgRioNew=0;
		for(i=0;i<3;i++)
		{
		 if(rioDat[i]!=rioDatpre[i])
		 { rioDatpre[i]=rioDat[i]; flgRioNew=1;}
		}
		if(flgRioNew==1)//RIO 数据变化
		{
			CanData[72]=rioDat[1];
			CanData[73]=rioDat[0];
			CanData[74]=rioDat[2];
			if(dualChNum==1)
			{
				if((rioDat[0]&0x01)==0x01)
					err_PE=0;
				else
					err_PE=1;
				if((rioDat[0]&0x02)==0x02)
					err_OvFlow=0;
				else
					err_OvFlow=1;
			}
			if(dualChNum==2)
			{
				if((rioDat[1]&0x01)==0x01)
					err_PE=0;
				else
					err_PE=1;
				if((rioDat[1]&0x02)==0x02)
					err_OvFlow=0;
				else
					err_OvFlow=1;
			}
	if(err_PE&&WarnOnOff[0])
		flgAlert[0]=1;
	else
		flgAlert[0]=0;

	if(err_OvFlow&&WarnOnOff[1])
		flgAlert[1]=1;
	else
		flgAlert[1]=0;

		}
	}
}

unsigned Main_Con(unsigned char id)
{
//50ms
//unsigned int tWarnCode;
//unsigned char tLeadOk;
unsigned char mi,ci=0;
unsigned char tBuf[8];
unsigned int  tUpCnt;
unsigned char tStep;
unsigned long tLngDat;
unsigned int  tIntDat;
//unsigned char tmId;
float f_tmp;
float flFup  = 0.0;//开稳流
float flFdn  = 0.0;//关稳流
float flFMx  = 0.0;//最大流量
float flFerr = 0.0;//偏差
float flMDn  = 0.0;//提前量
float flMOv  = 0.0;//过冲量
float f_SpUp = 0.0;//开关频率
float f_SpDn = 0.0;//调节频率
float f_SpAd = 0.0;//调节频率
//float f_SpOp = 0.0;//开关频率
//float f_SpAdj = 0.0;//调节频率
float f_KgNeed;
//tmId=id;
f_SpUp=set_SpOp;
f_SpUp*=1.575;	//f_SpUp*=0.63;
f_SpDn=set_SpCl;
f_SpDn*=0.1575;//f_SpDn*=0.063;
f_SpAd=set_SpAd;
f_SpAd*=0.1575;//f_SpDn*=0.063;
flFup=set_FUp*0.1;
flFdn=set_FDn*0.1;
flFMx=set_FMx;
flFerr=set_Ferr*0.01;
flMDn=set_MDn;
flMOv=set_MOv;
//关阀提前量处理
f_tmp=(float)dnSavConfDat[4]*0.1;
f_tmp=(1.0+f_tmp)*flFerr;
f_ExDn=flFdn*(1.0+f_tmp);
//flMOv=0.0;
//sys_KgNeed=lng_KgNeed;
//if(lng_KgNeed[id]>set_BNeed)
//tYFLdat=lng_KgNeed;//[id-1];
//tYFLdat=tYFLdat-set_BNeed;
//f_KgNeed=(float)tYFLdat*1.0;
f_KgNeed=(float)lng_KgNeed*1.0;
tStep=sys_STA;AoDat2=AoDat1;
tUpCnt=set_Tsta;
if((sys_STA==0)||(sys_STA==6)||(sys_STA==8))
{
	//warn_Flow=0;
	err_FV=0;
	cntFVconnTmr=0;
	cntFVflow0=0;
	WarnAvid=0;
}

//
//总共七个可以导致暂停：0静电，1溢流，2流量计，3无流量, 4汽化,5汽化,6用户,7压力,8低流量,9低压力
//											0x40	 0x20	  0x10		  0x08				0x04	0x02	0x01
//if(set_OpValveTmr<1)//如果流量计超时设置过小，则关闭流量计超时报警
//	tWarnOnOff[2]=0;
if(sys_STA>0)
{
	WarnAvid=0;
	for(mi=0;mi<6;mi++)
	{
		if(WarnOnOff[mi]==0)
			flgAlert[mi]=0;
	}
	if(WarnOnOff[6]==0)flgAlert[7]=0;
	if(WarnOnOff[7]==0)flgAlert[8]=0;
	if(WarnOnOff[8]==0)flgAlert[9]=0;
	for(mi=0;mi<9;mi++)
	{
		WarnAvid+=flgAlert[mi];
		WarnAvid<<=1;
	}
	if(WarnAvid>0)
	{
		if(cntWarnAvid<50)
			cntWarnAvid++;
	}
	else
		cntWarnAvid=0;
	if(((sys_STA>0)&&(sys_STA<=6))&&(flgRmCon==2))
	{
			AoDat=0;AoDat1=0;flAoDat=600.0;//AoDat2=0;
			//DAC_SetChannel1Data(DAC_Align_12b_R,AoDat);
			sys_STA=7;flgSysWarn=2;flgAlert[6]=1;flgWrWarnLst=1;
			tStep=sys_STA;
			//OpWarnBeep;
		  flgAlertRst=0;
	}
	if(cntWarnAvid>20)
	{
		if((sys_STA>0)&&(sys_STA<=6))
		{
			AoDat=0;AoDat1=0;flAoDat=600.0;//AoDat2=0;
			//DAC_SetChannel1Data(DAC_Align_12b_R,AoDat);
			sys_STA=7;flgSysWarn=1;flgWrWarnLst=1;//flgAlert[6]=1;
			tStep=sys_STA;
			//OpWarnBeep;
		  flgAlertRst=0;
		}
	}
	//if(sys_STA<6)
	//{
			//mbdata[20]=0x55; 	
		//if(WarnOnOff[9]==1)
		//{
		//	if(dSFLv>rtSSLv_pls)
		//	{
		//	 if(dSFLv-rtSSLv_pls>rtPLerr)
		//	 {sys_STA=7;flgAlert[10]=1;flgWrWarnLst=1;}
		//	}
		//	else
		//	{
		//	 if(rtSSLv_pls-dSFLv>rtPLerr)
		//	 {sys_STA=7;flgAlert[10]=1;flgWrWarnLst=1;}
		//	}
		//}
	//}	//正在发油
}
if(tStep>0)
{
	dSFLv=dLJSv-dLJSv_first;
	dSFLm=dLJSm-dLJSm_first;
	if(dSFLv<0.0)dSFLv=0.0;
	if(dSFLm<0.0)dSFLm=0.0;
	canSFLv[dualChNum-1]=dSFLv;
	if(tStep<=6)
	{
	 valv_pump|=1;
	}
	canRtData[24]=dualChNum;
	tLngDat=(unsigned long)dSFLv;
	tIntDat=tLngDat>>16;
	//canRtData[22]=(unsigned char)(tIntDat>>8);
	canRtData[23]=(unsigned char)(tIntDat&0xFF);
	tIntDat=tLngDat&0xFFFF;
	canRtData[21]=(unsigned char)(tIntDat>>8);
	canRtData[22]=(unsigned char)(tIntDat&0xFF);
	//flgMbWr=1;
	//ftoc(dSFLm);
	//mbdata[2]=(c2f[1]<<8)+c2f[0];
	//mbdata[3]=(c2f[3]<<8)+c2f[2];
	//ftoc(dSFLv);
	//mbdata[4]=(c2f[1]<<8)+c2f[0];
	//mbdata[5]=(c2f[3]<<8)+c2f[2];
	//flgMbWr=0;
	//mbBuf[2]=mbdata[2];mbBuf[3]=mbdata[3];
	//mbBuf[4]=mbdata[4];mbBuf[5]=mbdata[5];
}
//结束键按下,等到流量小于0.1Kg/s时认为阀门关闭，作结算，
//还应该考虑定时处理,防止阀门损坏的情况	
if(flgEnd==1)//结束的第一阶段：等待阀门完全关闭，流量小于0.1m3/h
{
	//if((dSSLv<0.01)&&(cntEndTmr>=10000))
	if((dSSLv<0.1)&&(cntEndTmr>=10))
	{
			for(mi=0;mi<8;mi++)
			{
			 frmMainYFval[mi]=' ';
			 frmMainOrdId[mi]=' ';
			}
			flgEnd=0;flgSdStart=0;flgSdEnd=0;
			cntEndTmr=0;valv_pump=0;
			SavEndDat();
			Del_Pend(dualChNum-1,nowPendLst[dualChNum-1]);
			UsrAvid=0;cntRfid=1;
			sys_STA=0;flgAMS=0;//mbdata[20]=0xAA;
			WrWarnLed(dualChNum-1,0);
			flgUsrSp=0;
			tStep=0;
			dYFLv=0.0;//rtYFL=0;
			AoDat=0;AoDat1=0;flAoDat=600.0;//AoDat2=0;
		  //Op_FTA_Do(AB_Empty,0xAB);
			/////Dis_CarId(0);//Dis_Pad(0);//Dis_Pad_Mini(0);
			for(mi=0;mi<8;mi++)
			{
				 CarId[mi]=0xAA;	 //车号无效
				LeadId[mi]=0xFF;   //铅封号无效
			  chrLeadId[dualChNum-1][mi]=' ';
				 chrCarId[dualChNum-1][mi]=' ';
				 chrOilId[dualChNum-1][mi]=' ';
				   chrYFL[dualChNum-1][mi]=' ';
					 idCarId[mi]=' ';idCarYFL[mi]=' ';
				idCarTypeId[mi]=' ';
			}
			//ShowStr30(530,82,"          ",8,BLACK,WHITE);
		  //TFT_Fill(528,87,528+128+5,112,BLUE);
			//ShowStr30(20,82,frmMainOrdId,8,BLACK,WHITE);
			ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE); //小数点以后舍去
			//mbdata[2]=0;mbdata[3]=0;mbdata[4]=0;mbdata[5]=0;
			//mbBuf[2] =0;mbBuf[3] =0;mbBuf[4] =0;mbBuf[5] =0;
			frmNum=FRM_MON;//mbdata[21]=0x00;mbdata[20]=0x00;
	}
}
	switch(tStep)
	{
		case 0:
			for(mi=0;mi<11;mi++)
				flgAlert[mi]=0;
			AoDat=0;AoDat1=0;flAoDat=0.0;//AoDat2=0;
			//warn_Flow=0;
			err_FV=0;
			cntFVconnTmr=0;
			cntFVflow0=0;
			break;
		case 1://开泵,提前开泵。
			flgAlertRst=0;flg1to5=0;
			flgUsrSp=0;
			//OpBump;
			//OpFValv;
			valv_pump=1;//valv_pump|=0x03;//open vice valve
			tTmr1=set_TOpBump*20;//TmrRatio;
			//处于提前量与过冲量之间，转为关稳流状态
			if((dSFLv>=(f_KgNeed-flMDn))&&(dSFLv<=(f_KgNeed-flMOv)))
			{
				flg1to5=1;
				sys_STA=5; AoDat=600;AoDat1=600;flAoDat=600.0;//AoDat2=0;
				break;
			}
			if(dSFLv>=(f_KgNeed-flMOv))
			{
				//AoDat=0;flAoDat=0.0;
				AoDat=0;AoDat1=0;flAoDat=0.0;//AoDat2=0;
				flgABvalv=AB_Empty;//Op_FTA_Do(flgABvalv,0xAB);
				sys_STA=6;
				tRunCnt2=0;
				break;
			}
			if(++tRunCnt1>tTmr1)
			{
				tRunCnt1=0;
				tTmrOp=0;
				sys_STA=2; 
				flgABvalv=AB_In;flgFValv=1;//Op_FTA_Do(flgABvalv,0xAB);
				AoDat=0;AoDat1=600;flAoDat=600.0;//AoDat2=0;
			}
			flgValv1=1;flgValv2=0;

			//DoDat=0x01;
			//Op_FTA_Do(DoDat,2);
			break;						
		case 2://开启,以开阀速率开主阀
			if((dSFLv>(f_KgNeed-flMDn))&&(dSFLv<(f_KgNeed-flMOv)))
			{
				dirErr=0;sys_STA=5;
				break;
			}
			if(dSFLv>=(f_KgNeed-flMOv))
			{
				dirErr=0;
				AoDat=0;AoDat1=600;flAoDat=600.0;//AoDat2=0;
				flgABvalv=AB_Empty;//Op_FTA_Do(flgABvalv,0xAB);
				sys_STA=6;
				tRunCnt2=0;
				break;
			}
			if((dSSLv<flFup)&&(AoDat<AO_LMT))	//&&(AoDat<4090)
			{
				flAoDat+=f_SpUp;
				flgABvalv=AB_In;//Op_FTA_Do(flgABvalv,0xAB);
				AoDat1=(unsigned int)flAoDat;
				if(AoDat1>AO_LMT)AoDat1=AO_LMT;
				if(flAoDat>AO_LMTf)flAoDat=AO_LMTf;
			}
			else
			{
				flgFValv=1;
				flgABvalv=AB_Hold;sys_STA=3;dirErr=0;//Op_FTA_Do(flgABvalv,0xAB);SaveRtSta(sys_STA);
				tRunCnt1=0;//为开稳流进行计时
				sys_STA=3;				dirErr=0;				tRunCnt1=0;//为开稳流进行计时
			}
			flgValv1=1;flgValv2=0;
			//DoDat=(rtDoDat&0xFD)|0x05;
			//Op_FTA_Do(DoDat,2);
			break;
case 3://开稳流
			if((dSFLv>=(f_KgNeed-flMDn))&&(dSFLv<(f_KgNeed-flMOv)))
			{
				sys_STA=5;
				break;
			}
			if(dSFLv>=(f_KgNeed-flMOv))
			{
				AoDat1=0;flAoDat=0.0;
				flgABvalv=AB_Empty;//Op_FTA_Do(flgABvalv,0xAB);
				sys_STA=6;
				tRunCnt2=0;
				break;
			}
			tTmr1=tUpCnt*TmrRatio;
		if(dirErr==1)
		{	
			if(dSSLv<=flFup)
			{
			 	dirErr=0;flgABvalv=AB_Hold;//Op_FTA_Do(flgABvalv,0xAB);
			}
			flAoDat-=f_SpAd;
			AoDat1=(unsigned int)flAoDat;
			if(AoDat1<600)	AoDat1=600;
			if(flAoDat<600.0)	flAoDat=600.0;

		}
		if(dirErr==2)
		{
			if(dSSLv>=flFup)
			{dirErr=0;flgABvalv=AB_Hold;}//Op_FTA_Do(flgABvalv,0xAB);
			else
			{flgABvalv=AB_dIn;
				flAoDat+=f_SpAd;
				AoDat1=(unsigned int)flAoDat;
				//AoDat+=int_SpOpCl;
				if(AoDat1>AO_LMT)AoDat1=AO_LMT;
				if(flAoDat>AO_LMTf)flAoDat=AO_LMTf;
			
			}//Op_FTA_Do(flgABvalv,0xAB);
		}
		//当瞬时量大于1.Err倍的稳流量时向下调节
//		if(rtSSL>((1+flFerr)*flFup))		{	dirErr=1;	}//调节到小于等于flFup时停止
			f_SSL_up=(1+flFerr)*flFup;
		if(dSSLv>f_SSL_up)		{	dirErr=1;	}//调节到小于等于flFup时停止

		//当瞬时量小于1.Err倍的稳流量时向上调节
			f_SSL_dn=(1-flFerr)*flFup;
		if(dSSLv<f_SSL_dn)	{	dirErr=2;	}//调节到大于等于flFup时停止
	
		if(++tRunCnt1>tTmr1)		{		sys_STA=4;flgSTA3to4=0;		}
		// DoDat=(rtDoDat&0xFD)|0x01;
		//	Op_FTA_Do(DoDat,2);
		flgValv1=1;flgValv2=0;
			break;
case 4://开稳流结束,阀门开到最大流量
				//如果实发量大于提前量,同时流量小于关稳流值
			if(dSFLv>=(f_KgNeed-flMOv))
			{
				AoDat1=0;flAoDat=0.0;
		  	flgABvalv=AB_Empty;//Op_FTA_Do(flgABvalv,0xAB);
				sys_STA=6;
				tRunCnt2=0;
				break;
			}
			if((dSFLv<(f_KgNeed-flMDn)))//提前量之前，最大流量调节
			{							 
				if(flgSTA3to4==0)
				{
					flAoDat+=f_SpUp;
					AoDat1=(unsigned int)flAoDat;
					if(flAoDat<600.0)	flAoDat=600.0;
					if(flAoDat>AO_LMTf)flAoDat=AO_LMTf;
					if(AoDat1<600)	AoDat1=600;
					if(AoDat1>AO_LMT)AoDat1=AO_LMT;
				  flgABvalv=AB_In;//Op_FTA_Do(flgABvalv,0xAB);
					if(dSSLv>=flFMx)//最大流量时进入调节状态
					{	flgSTA3to4=1;	flgSTA4to5=0;flgABvalv=AB_Hold;}//Op_FTA_Do(flgABvalv,0xAB);	
				}
				else
				{
				if(dirErr==1)//高于偏差值
				{
					if(dSSLv<=flFMx)
					{	dirErr=0;flgABvalv=AB_Hold;}//Op_FTA_Do(flgABvalv,0xAB);
					else
					{	flgABvalv=AB_dOut;
						flAoDat-=f_SpAd;
						AoDat1=(unsigned int)flAoDat;
						//AoDat-=int_SpAdj;
						if(AoDat1<600)	AoDat1=600;
						if(flAoDat<600.0)	flAoDat=600.0;
					
					}//Op_FTA_Do(flgABvalv,0xAB);
				}
				if(dirErr==2)	//低于偏差值
				{
					if(dSSLv>=flFMx)
					{dirErr=0;flgABvalv=AB_Hold;}//Op_FTA_Do(flgABvalv,0xAB);
					else
					{flgABvalv=AB_dIn;
						flAoDat+=f_SpAd;
						AoDat1=(unsigned int)flAoDat;
						//AoDat+=int_SpOpCl;
						if(AoDat1>AO_LMT)AoDat1=AO_LMT;
						if(flAoDat>AO_LMTf)flAoDat=AO_LMTf;
					}//Op_FTA_Do(flgABvalv,0xAB);
				}
				if(dSSLv>((1+flFerr)*flFMx))
				{	dirErr=1;		}
				if(dSSLv<(flFMx-flFMx*flFerr))
				{	dirErr=2;		}
				}
			flgSTA4to5=0;
			}//提前量之前，最大流量调节
			else
			{
				if(flgSTA4to5==0)
				{
			 		flgSTA4to5=1;
					flSeg1=dnSavConfDat[0];	//分段点1
					flSeg2=dnSavConfDat[2];	//分段点2
					flSp1 =dnSavConfDat[1]/100.0;	//速率1 40
					flSp2 =dnSavConfDat[3]/100.0;	//速率2	70
					flSeg1=flSp1*flFMx;	//以流量作为分段点
					flSeg2=flSp2*flFMx;	//以流量作为分段点
					flSeg3=(1.0+(dnSavConfDat[4]/100.0))*flFdn;
					uwSp3=set_Ton*20;
					if(dnSavConfDat[0]>0)
						uwSp1=(uwSp3*dnSavConfDat[0])/10;
					if(uwSp1>(set_Tall*20))uwSp1=set_Tall*20;
					if(uwSp1<20) uwSp1=20;
						
					if(dnSavConfDat[2]>0)
						uwSp2=(uwSp3*dnSavConfDat[2])/10;
	
					if(uwSp2>(set_Tall*20))uwSp2=(set_Tall*20);
					if(uwSp2<20) uwSp2=20;
				}
				if(dSSLv>flSeg2) //70%
					{ 
						flgABvalv=AB_Out;cntABton=uwSp2;//Op_FTA_Do(AB_Out,0xAB);
						flAoDat-=f_SpDn;
						AoDat1=(unsigned int)flAoDat;
						if(AoDat1<600)AoDat1=600;
						if(flAoDat<600.0)flAoDat=600.0;
					}
				else if(dSSLv>flSeg1)//40<x<70
				{
					flgABvalv=AB_Out;cntABton=uwSp3;//Op_FTA_Do(AB_Out,0xAB);
					flAoDat-=f_SpDn;
					AoDat1=(unsigned int)flAoDat;
					if(AoDat1<600)AoDat1=600;
					if(flAoDat<600.0)flAoDat=600.0;
				}
				else if(dSSLv<flSeg1)
				{
					flgABvalv=AB_Out;cntABton=uwSp1;//Op_FTA_Do(AB_Out,0xAB);//SaveRtSta(43);
					flAoDat-=f_SpDn;
					AoDat1=(unsigned int)flAoDat;
					if(AoDat1<600)AoDat1=600;
					if(flAoDat<600.0)flAoDat=600.0;
				} 
				if(dSSLv<=flSeg3)//分段点
				{
				 flgABvalv=AB_Hold;sys_STA=5;
				 break;
				}
			}
			//过提前量,流量小于关稳流,进入
			if((dSFLv>(f_KgNeed-flMDn))&&(dSSLv<=flFdn))
			{
				sys_STA=5;//SaveRtSta(sys_STA);//关稳流
				break;
			}
			//如果实发量大于提前量，同时流量大于关稳流量，
			if((dSFLv>(f_KgNeed-flMDn))&&(dSSLv>flFdn))
			{
	  		flgABvalv=AB_Out;//Op_FTA_Do(AB_Out,0xAB);
				flAoDat-=f_SpDn;
				AoDat1=(unsigned int)flAoDat;
				if(AoDat1<600)AoDat1=600;
				if(flAoDat<600.0)flAoDat=600.0;
			}
			flgValv1=0;flgValv2=1;
			break;
		case 5://关稳流区
		if(dSFLv>=(f_KgNeed-flMOv))
		{
  		flgABvalv=AB_Empty;//Op_FTA_Do(AB_Empty,0xAB);
			AoDat1=0;flAoDat=0.0;
			sys_STA=6;valv_pump=0;
			tRunCnt2=0;	flgValv1=0;flgValv2=0;flgFValv=0;
			break;
		}

		if(dirErr==1)//高于
		{
			if(dSSLv<=flFdn)
			{	dirErr=0;flgABvalv=AB_Hold;}//Op_FTA_Do(AB_Hold,0xAB);
			else
			{	flgABvalv=AB_dOut;
				flAoDat-=f_SpAd;
				AoDat1=(unsigned int)flAoDat;
				if(AoDat1<600)AoDat1=600;
				if(flAoDat<600.0)flAoDat=600.0;
			
			}//Op_FTA_Do(AB_dOut,0xAB);
		}
		if(dirErr==2)
		{
			if(dSSLv>=flFdn)
			{dirErr=0;flgABvalv=AB_Hold;flg1to5=0;}//Op_FTA_Do(AB_Hold,0xAB);
			else
			{flgABvalv=AB_dIn;
			  if(flg1to5==0)
					flAoDat+=f_SpAd;
				else
					flAoDat+=f_SpUp;
				AoDat1=(unsigned int)flAoDat;
				if(AoDat1>AO_LMT)AoDat1=AO_LMT;
				if(flAoDat>AO_LMTf)flAoDat=AO_LMTf;
			
			}//Op_FTA_Do(AB_dIn,0xAB);

		}
		if(dSSLv>((1+flFerr)*flFdn))
		{	dirErr=1;		}
		if(dSSLv<(flFdn-flFdn*flFerr))
		{	dirErr=2;		}
		flgValv1=1;flgValv2=0;
		//DoDat=(rtDoDat&0xFD)|0x01;
		//Op_FTA_Do(DoDat,2);
		break;
		//关主阀，延时关副阀与机泵,同时检测关阀故障，
		//关阀超时时间内没有检测到回讯，视为关阀故障，关阀回讯为DI5
		case 6:
			tTmr2=set_TClBump*TmrRatio;
			if(++tRunCnt2>=tTmr2)
			{
				//ClBump;ClFValv;
				flgABvalv=AB_Empty;//Op_FTA_Do(AB_Empty,0xAB);
				sys_STA=8;	valv_pump=0;flgEnd=1;
			}
		AoDat1=0;flAoDat=0.0;
		flgValv1=0;flgValv2=0;flgFValv=0;
		flgABvalv=AB_Empty;//Op_FTA_Do(AB_Empty,0xAB);
		break;
		case 7:
			AoDat1=0;flAoDat=0.0;flgABvalv=AB_Empty;DoDat=0x00;flgValv1=0;flgValv2=0;flgFValv=0;//Op_FTA_Do(AB_Empty,0xAB);
		break;
		case 8:
			AoDat1=0;flAoDat=0.0;
			flgABvalv=AB_Empty;//ClFValv;DoDat=0x00;//Op_FTA_Do(DoDat,2);	Op_FTA_Do(AB_Empty,0xAB);
			valv_pump=0;//close vice valve
			flgAlertRst=0;
			//for(mi=0;mi<8;mi++)
			//{ frmMainYFval[mi]=' ';}
			//ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE); //小数点以后舍去
		break;
	}
	if(id==1)
	{
		rio_AoL1=AoDat1&0xFF;rio_AoH1=AoDat1>>8;
		rio_AoL2=0;rio_AoH2=0;
	}
	if(id==2)
	{
		rio_AoL1=0;rio_AoH1=0;
		rio_AoL2=AoDat1&0xFF;rio_AoH2=AoDat1>>8;
	}
//	if(tStep<6)
//		Op_FTA_Do(flgABvalv,0xAB);
//	else
//	{
//	 if((cntWarnAvid==0)&&(flgSysWarn==2))
//	 {flgSysWarn=0;Op_FTA_Do(0x04,0x02);}
//	 if((cntWarnAvid>20)&&(flgSysWarn==0))
//	 {flgSysWarn=2;Op_FTA_Do(0x14,0x02);}
//	 if(flgSysWarn==1) {  Op_FTA_Do(0x34,0x02);	 }
//	 if(flgSysWarn==2) {  Op_FTA_Do(0x14,0x02);	 }
//	}
	
//	flgMbWr=1;
////	mbdata[6]=mbdata[6];//
////	mbdata[7]=mbdata[6];//
//	flgMbWr=0;
//	mbBuf[6]=mbdata[6];mbBuf[7]=mbdata[7];
	
return tStep;

}

/*慢速事件*/
void TickEvents(void)
{
	unsigned char i;//ti,
	char tDisBuf[5];
	float tfDat;
	flgFyAllow=1;
	if((frmNum==FRM_MAIN)||(frmNum==FRM_SD))//处理发油允许信号
	{
		if((sys_STA!=0)&&(sys_STA!=7))
			flgFyAllow=0;
//		if((sys_Conn==1)&&(UsrAvid==0)&&(sys_STA==0))
//			flgFyAllow=0;
		if((dualChNum>0)&&(dYFL[dualChNum-1]==0))
			flgFyAllow=0;
		//if((WarnOnOff[8]==1))
		//{
		//	if(/*(flgAMS==1)||*/(flgDiffAMS==1)||(uwCmpAMS!=0))
		// 		flgFyAllow=0;
		//}
		if((sys_STA==7)&&(flgAlertRst==0))
			flgFyAllow=0;
		if(flgFyAllowPre!=flgFyAllow)
		{
		  flgFyAllowPre=flgFyAllow;
			Dis_EN_FY(flgFyAllow);
			//if(flgFyAllow==1)
			// mbdata[21]=0x55;
			//else
			// mbdata[21]=0x00;
		}
		if(dualChNum>0)
		{
			if(dYFLv>0)
			{
				if(dSFLv<0.0)dSFLv=0.0;
				tfDat=dSFLv/dYFLv;
			  tfDat*=100.0;//tfDat+=0.5;
				if(tfDat>100.0)tfDat=100.0;
				LastProc=(unsigned char)(tfDat);
				if((sys_STA>0)&&(LastProc!=LastProc_pre)&&((frmNum==FRM_MAIN)||(frmNum==FRM_SD)))
				{
					//TFT_Fill(18,170,122,190,0x0000);TFT_Fill(20,172,20+LastProc,188,GREEN);
					//TFT_Fill(240-2+93,395,342+93,410,0x0000);	//TFT_Fill(70,140,170,160,0x0000);
				  if(LastProc>100)LastProc=100;
					//TFT_Fill(240+93,397,240+93+LastProc,408,GREEN);//	TFT_Fill(70,142,70+LastProc,158,GREEN);
					LastProc_pre=LastProc;
					tDisBuf[0]=LastProc/100;
					i=LastProc%100;
					tDisBuf[1]=i/10;
					tDisBuf[2]=i%10;
					tDisBuf[4]=0;
					if((tDisBuf[1]==0)&&(tDisBuf[0]==0))	tDisBuf[1]=' ';	else	tDisBuf[1]+='0';
					if(tDisBuf[0]==0)	tDisBuf[0]=' ';	else	tDisBuf[0]+='0';
					tDisBuf[2]+='0';tDisBuf[3]='%';tDisBuf[4]=0;
					//lcd_text24( 13, 88,BLACK,WHITE,"进度");
					DisNum24( 62,88,BLUE,WHITE,tDisBuf);
					/////Draw_EN24(346+93,390,BLUE,0xFFFF,tDisBuf);
				}
			}
		}
		//if(sys_STA==0)
		//{	mbdata[20]=0x00;}
	}
	else
	{flgFyAllow=0;}//mbdata[21]=0x00;mbdata[25]=0x00;}
	
//	if(frmNum==FRM_MAN)
//	{		Dis_Man();	}
if((flgWrWarnLst==1)&&(sys_STA==7))
{
	flgWrWarnLst=0;
	for(i=0;i<11;i++)
	{
			if((flgAlert[i]==1)&&(flgAlertPre[i]==0))
			{
				AddWarnLst(dualChNum-1,i+1);
				flgAlertPre[i]=1;
			}
			if(flgAlert[i]==0)
				flgAlertPre[i]=0;
	}
}
//if(0)//if(sys_STA==0)
//{
//	if(UsrAvid!=UsrAvid_pre)
//	{
//		UsrAvid_pre=UsrAvid;
//		if(UsrAvid==0)
//		{
//			TFT_Fill(223,423+20,222+165,448+20,GOLD);
//			lcd_text24(225,422+20,RED,GOLD,"等待授权");
//			Dis_EN_Lic(0);
//		}
//		else
//		{
//			TFT_Fill(223,423+20,222+165,448+20,GOLD);
//			lcd_text24(225,422+20,0,GOLD,"授权成功");
//			Dis_EN_Lic(1);
//		}
//	}
//	if(cntRfid_pre!=cntRfid)
//	{
//		cntRfid_pre=cntRfid;
//		if((cntRfid==1)&&(UsrAvid==0))
//		{
//			TFT_Fill(223,423+20,222+165,448+20,GOLD);
//			lcd_text24(265,422+20,BLUE,GOLD,"        ");
//			Dis_EN_Lic(0);
//		}
//		if(cntRfid==2)
//		{
//			TFT_Fill(223,423+20,222+165,448+20,GOLD);
//			lcd_text24(265,422+20,RED,GOLD,"等待授权");
//			Dis_EN_Lic(0);
//		}
//	}
//}
}



void RxdEvents(void)
{
unsigned int tx1,tx2;
unsigned char tLen,terr=0;
//char tBuf[3];
if(dualChNum==1){tx1= 12;tx2=173;}
if(dualChNum==2){tx1=412;tx2=573;}
//if((frmNum==FRM_MAIN))
//	Draw_AoBar(AoDat1);
sysWarnLed[0]=0;sysWarnLed[1]=0;	
if(sys_STA==7)
{
	if(flgAlert[6]==1)
		sysWarnLed[dualChNum-1]=3;
	else
	{
		terr=0;
		if(flgAlert[0]==1)terr=1;
		if(flgAlert[1]==1)terr=1;
		if(flgAlert[2]==1)terr=1;
		if(flgAlert[3]==1)terr=1;
		if(flgAlert[4]==1)terr=1;
		if(flgAlert[5]==1)terr=1;
		if(flgAlert[7]==1)terr=1;
		if(flgAlert[8]==1)terr=1;
		if(flgAlert[9]==1)terr=1;
		if(terr==0)
			sysWarnLed[dualChNum-1]=5;
		else
			sysWarnLed[dualChNum-1]=4;
	}
}
if((flgFyAllow==1)&&(flgFyAllowPre==1))
{sysWarnLed[dualChNum-1]=1;}
if(sys_STA==5)
{sysWarnLed[dualChNum-1]=6;}
if((sys_STA>0)&&(sys_STA<5))
{sysWarnLed[dualChNum-1]=2;}
if(frmNum!=FRM_MAIN)
{sysWarnLed[0]=0;sysWarnLed[1]=0;}

//if(sys_STA<7){WarnID=0;flgWarnWr=0;}
if(sys_STA_Last!=sys_STA)
{
	switch(sys_STA)
	{
		case 0:
		if(flgSdMode==0)
		{
			TFT_Fill( 12,422+20, 12+ 73,422+25+20,GOLD);
			TFT_Fill(412,423+20,412+ 73,448+20,GOLD);
			TFT_Fill(173,423+20,173+165,448+20,GOLD);
			TFT_Fill(573,423+20,573+165,448+20,GOLD);
			lcd_text24(12,422+23,BLUE,GOLD,"空闲");//空闲
			lcd_text24(412,422+23,BLUE,GOLD,"空闲");//空闲 
		}
		break;
		case 1:
		{	
			TFT_Fill(tx1,422+20,tx1+ 73,422+25+20,GOLD);
			TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);
			lcd_text24(tx1,422+23,BLUE,GOLD,"发油");
		}
		break;
		case 2:
		{	
			TFT_Fill(tx1,422+20,tx1+93,422+25+20,GOLD);
			TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);
			lcd_text24(tx1,422+23,BLUE,GOLD,"开启");
		}
		break;		
		case 3:
		{
			TFT_Fill(tx1,422+20,tx1+73,422+25+20,GOLD);
			TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);
			lcd_text24(tx1,422+23,BLUE,GOLD,"开稳流");
		}
		break;
		case 4:
		{
			TFT_Fill(tx1,422+20,tx1+73,422+25+20,GOLD);
			TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);
			lcd_text24(tx1,422+23,BLUE,GOLD,"全开");
		}
		break;
		case 5:
		{ 
			TFT_Fill(tx1,422+20,tx1+73,422+25+20,GOLD);
			TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);
			lcd_text24(tx1,422+23,BLUE,GOLD,"关稳流");
		}
		break;
		case 6:
		{	
			TFT_Fill(tx1,422+20,tx1+73,422+25+20,GOLD);
			TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);
			lcd_text24(tx1,422+23,BLUE,GOLD,"关主阀");
		}
		break;
		case 7:
			TFT_Fill(tx1,422+20,tx1+73,422+25+20,GOLD);lcd_text24(tx1,422+23,BLUE,GOLD,"暂停");tLen=0;		
			if(flgAlert[0]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"静电报警");tLen=4;}
			if(flgAlert[1]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"溢流报警");tLen=4;}
			if(flgAlert[2]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"流量计通讯故障");tLen=7;}
			if(flgAlert[3]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"无流量报警");tLen=5;}
			if(flgAlert[4]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"泄漏报警");tLen=4;}
			if(flgAlert[5]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"密度报警");tLen=4;}
			if(flgAlert[6]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"用户暂停");tLen=4;}
			if(flgAlert[7]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"压力过低");tLen=4;}
			if(flgAlert[8]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"流量过低");tLen=4;}
			if(flgAlert[9]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"ＡＭＳ报警");tLen=5;}
			if(flgAlert[10]==1){TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);lcd_text24(tx2+3,422+23,BLUE,GOLD,"急停报警");tLen=5;}
			//if(err_Press==1)				{TFT_Fill(tx2,423+20,223+160,448+20,GOLD);lcd_text24(265,422,BLUE,GOLD,"压力过低");}
			if(tLen!=7)
			{
				if(dualChNum==1)	lcd_text24(tx2+3+tLen*24,422+23,BLUE,GOLD,"１");
				if(dualChNum==2)	lcd_text24(tx2+3+tLen*24,422+23,BLUE,GOLD,"２");
			}
		break;
		case 8:
			{	TFT_Fill(tx1,422+20,tx1+73,422+25+20,GOLD);	lcd_text24(tx1,422+23,BLUE,GOLD,"结束");	
				if(flgEnd==0)TFT_Fill(tx2,423+20,tx2+165,448+20,GOLD);			}
		break;
	}
	sys_STA_Last=sys_STA;
	//flgSndFTA=1;
}
	//	1	 2	4	 8	10 20	40 80
	//di0  1  2  3  4  5  6  7 
		//频 机 副 静 主 溢 主 溢 
		//率 泵 阀 电 阀 流	阀 流
		//            关 2	开 1
		//if(sys_Warn==0)		flgWarn=0;
		/*
		王世豪HB  16:05:43
测试界面      DO0   DO1   DO2   DO3   DO4   DO5   DO6   DO7
   FTA灯      LDO5  LDO6  LDO7  LDO8  LDO1  LDO2  LDO3  LDO4
																			主A		主B		副    泵 
	 */																	
}

void Rfid(void)
{
	unsigned char ti,i;
	//char *tmpYlf;
	char tBuf1[9];
		icBuf[9]=0;
	 	for(i=0;i<9;i++)
		{
			frmMainOrdId[i]=icBuf[i];
			icBuf[i]=0;
		}
		if(frmNum==FRM_MAIN)
		{
			//TFT_Fill(41,82,10*16+3,24,WHITE);
			ShowStr30(30+10,82,frmMainOrdId,8,BLACK,WHITE);
		}
		if(frmNum==FRM_MAN)
		{
			lcd_text16(400,95+4*40,0,WHITE,"Rfid Card:");
			lcd_text16(400,75+5*40,0,WHITE,frmMainOrdId);
		
		}
		frmMainEdit=0;
		//tmpYlf=//查询待发记录
		GetPendYFL(frmMainOrdId);
		for(i=0;i<9;i++)
			tBuf1[i]=YflBuf[i];//tBuf1[i]=tmpYlf[i];
		//nowPendLst=GetPendYFL(frmMainOrdId);//查询待发记录
		for(ti=0;ti<9;ti++)
		{	
			frmMainYFval[ti]=YflBuf[ti];//tmpYFL[ti];
			//if((tmpYFL[ti]>0x2F)&&(tmpYFL[ti]<0x3A))//不为空
			if((YflBuf[ti]>0x2F)&&(YflBuf[ti]<0x3A))//不为空
			{
				tBuf1[ti]=YflBuf[ti];
			}
			else
			{
				tBuf1[ti]='0';
			}
			//tmpYFL[ti]=0;
		}
		if(flgPendLst==1)//查到待发记录
		{
		  cntRfid=2;
			UsrAvid=0;
			dYFL[dualChNum-1]=calYFL();

		}
		for(ti=0;ti<9;ti++)
		{
			if((frmMainOrdId[ti]>0x2F)&&(frmMainOrdId[ti]<0x3A))//不为空
				nowPendLst[dualChNum-1][ti]=frmMainOrdId[ti];
			else
				nowPendLst[dualChNum-1][ti]=0x30;
		}
		for(ti=0;ti<4;ti++)
		{
			nowOrdId[ti]=(nowPendLst[dualChNum-1][ti*2]<<4)+(nowPendLst[dualChNum-1][ti*2+1]&0x0F);
			nowYFval[ti]=(tBuf1[ti*2]<<4)+(tBuf1[ti*2+1]&0x0F);
		}
		if(frmNum==FRM_MAIN)
		ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE); //小数点以后舍去
		if(frmNum==FRM_MAN)
			lcd_text16(400,75+6*40,0,WHITE,frmMainYFval);

}



