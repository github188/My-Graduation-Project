#include "mem.h"
#include "time.h"
#include "Global.h"


//unsigned int PendAdd[10]={260,280,300,324,344,364,388,408,428,452};
//加上车牌后的待发记录每条25个字节，连续存放
//
extern void lcd_text32(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void Dis_CarId(unsigned char id);
extern void EepRd(unsigned int ReadAddr,unsigned int NumByteToWrite);
extern void EepWr(unsigned int ReadAddr,unsigned int NumByteToWrite);
extern unsigned long calYFL(void);
extern void getWarnOnOff(void);
extern void Dis_GdsId(unsigned char tGdsId);

void ReadLJS(void);

unsigned char cntNewPendLst;
extern float YFL[2];
extern char chrCarId[2][9];//应发量，4位BCD码
extern char chrYFL[2][9];//应发量存储
extern char chrOilId[2][9];//提油密码存储
extern char idCarId[10];    //卡里车号[实际卡里没有，在待发确认界面中人工输入]
extern char idCarYFL[10];		//卡里应发量
extern char idCarTypeId[10];//卡里车型号码
//extern volatile float dYFL[2];

//用4个扇区,可以存放256条记录，
//0表示记录为空，1表示记录数据无效，不可用，期待下一次删除
//如果数据大于10则为记录号
//0,1,2,10~209
unsigned char flgLst[256];
unsigned char flgIdLst[256];
// 

unsigned int cntFM;
unsigned char flgFM=0;

extern unsigned long cal_YFL(char tBuf[9]);//只计算应发量

//读FM
void ReadFM(void)
{
	EepRd(0x35F0,5);
	if((EepRdBuf[0]==0xAA)&&(EepRdBuf[1]==0x55)&&(EepRdBuf[4]==0x3A))
	{
		cntFM=(EepRdBuf[2]<<8)+(EepRdBuf[3]);
		if(cntFM>=500)//
		{
		  flgFM=1;
		  cntFM=300; frmNum=FRM_LOGIN;
		}
	}
	else
	{
	 EepWrBuf[0]=0xAA;EepWrBuf[1]=0x55;EepWrBuf[2]=0x0;EepWrBuf[3]=0x0;EepWrBuf[4]=0x3A;
	 cntFM=0;flgFM=0;
	 EepWr(0x35F0,5);
	}
}

void Fill_Man_Lst(char *tBuf)
{
		unsigned char tAdd;
		float t1,t2;
		char tSqrBuf[80];
		unsigned char i;
		for(i=0;i<80;i++)
			tSqrBuf[i]=tBuf[i];
		tAdd=(tSqrBuf[0]<<4)|(tSqrBuf[1]&0x0F);
		//以下为CAN自动上传记录
    Fy2CanLst[0x00]=0x40;
    Fy2CanLst[0x01]=tSqrBuf[15];//员工号
    Fy2CanLst[0x02]=tSqrBuf[16];
    Fy2CanLst[0x03]=tSqrBuf[63];//路号
    Fy2CanLst[0x04]=tSqrBuf[64];//车号1
		Fy2CanLst[0x05]=tSqrBuf[65];//车号2
		Fy2CanLst[0x06]=tSqrBuf[66];//车号3
		Fy2CanLst[0x07]=tSqrBuf[67];//车号4
		Fy2CanLst[0x08]=0x81;
		Fy2CanLst[0x09]=tSqrBuf[68];//车号5
		Fy2CanLst[0x0A]=tSqrBuf[69];//车号6
		Fy2CanLst[0x0B]=tSqrBuf[70];//车号7
		Fy2CanLst[0x0C]=tSqrBuf[71];//车号8
		Fy2CanLst[0x0D]=tSqrBuf[25];//开始时间1
		Fy2CanLst[0x0E]=tSqrBuf[26];//开始时间2
		Fy2CanLst[0x0F]=tSqrBuf[27];//开始时间3
		Fy2CanLst[0x10]=0x82;
		Fy2CanLst[0x11]=tSqrBuf[7];//应发量1
		Fy2CanLst[0x12]=tSqrBuf[8];//应发量2
		Fy2CanLst[0x13]=tSqrBuf[9];//应发量3
		Fy2CanLst[0x14]=tSqrBuf[10];//应发量4
		Fy2CanLst[0x15]=tSqrBuf[28];//开始时间5
		Fy2CanLst[0x16]=tSqrBuf[29];//开始时间6
		Fy2CanLst[0x17]=tSqrBuf[30];//开始时间7
		Fy2CanLst[0x18]=0x83;
		Fy2CanLst[0x19]=tSqrBuf[11];//实发重量1
		Fy2CanLst[0x1A]=tSqrBuf[12];//实发重量2
		Fy2CanLst[0x1B]=tSqrBuf[13];//实发重量3
		Fy2CanLst[0x1C]=tSqrBuf[14];//实发重量4
		Fy2CanLst[0x1D]=0;//发油时间1
		Fy2CanLst[0x1E]=0;//发油时间2
		Fy2CanLst[0x1F]=tSqrBuf[72];//铅封1
		t1=ctof(tSqrBuf[62],tSqrBuf[61],tSqrBuf[60],tSqrBuf[59]);//体积表后数
		t2=ctof(tSqrBuf[58],tSqrBuf[57],tSqrBuf[56],tSqrBuf[55]);//体积表前数
		ftoc(t1-t2);
		Fy2CanLst[0x20]=0x84;
		Fy2CanLst[0x21]=c2f[0];//实发体积1
		Fy2CanLst[0x22]=c2f[1];//实发体积2
		Fy2CanLst[0x23]=c2f[2];//实发体积3
		Fy2CanLst[0x24]=c2f[3];//实发体积4
		Fy2CanLst[0x25]=tSqrBuf[73];//铅封2
		Fy2CanLst[0x26]=tSqrBuf[74];//铅封3
		Fy2CanLst[0x27]=tSqrBuf[75];//铅封4
		Fy2CanLst[0x28]=0x85;
		Fy2CanLst[0x29]=tSqrBuf[17];//前表数m1
		Fy2CanLst[0x2A]=tSqrBuf[18];//前表数m2
		Fy2CanLst[0x2B]=tSqrBuf[19];//前表数m3
		Fy2CanLst[0x2C]=tSqrBuf[20];//前表数m4
		Fy2CanLst[0x2D]=tSqrBuf[37];//介密1
		Fy2CanLst[0x2E]=tSqrBuf[38];//介密2
		Fy2CanLst[0x2F]=0x55;//发油单位:55H质量；0AAH体积
		Fy2CanLst[0x30]=0x86;
		Fy2CanLst[0x31]=tSqrBuf[55];//前表数v1
		Fy2CanLst[0x32]=tSqrBuf[56];//前表数v2
		Fy2CanLst[0x33]=tSqrBuf[57];//前表数v3
		Fy2CanLst[0x34]=tSqrBuf[58];//前表数v4
		Fy2CanLst[0x35]=tSqrBuf[39];//tMdBuf[2];//tSqrBuf[47];//介密3
		Fy2CanLst[0x36]=tSqrBuf[40];//tMdBuf[3];//tSqrBuf[48];//介密4
		Fy2CanLst[0x37]=tAdd;//记录
		Fy2CanLst[0x38]=0xC0;
		Fy2CanLst[0x39]=tSqrBuf[3];//密码1
		Fy2CanLst[0x3A]=tSqrBuf[4];//密码2
		Fy2CanLst[0x3B]=tSqrBuf[5];//密码3
		Fy2CanLst[0x3C]=tSqrBuf[6];//密码4
		Fy2CanLst[0x3D]=tSqrBuf[41];//tWdBuf[0];//0x55;//LstFyBuf[54];//
		Fy2CanLst[0x3E]=tSqrBuf[42];//tWdBuf[1];//0xAA;//LstFyBuf[55];//
		//FillLstCAN();//查询记录
		flgLstTFTok=tSqrBuf[63];//cntLstTFT=9;cntCanFb=8;canRtCnt=0;
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
		//以下为CAN自动上传记录
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
    Fy2CanLst[0x01]=tSqrBuf[15];//员工号
    Fy2CanLst[0x02]=tSqrBuf[16];
    Fy2CanLst[0x03]=tSqrBuf[51];//物料1
    Fy2CanLst[0x04]=tSqrBuf[52];//物料2
		Fy2CanLst[0x05]=tSqrBuf[53];//物料3
		Fy2CanLst[0x06]=tSqrBuf[54];//物料4
		Fy2CanLst[0x07]=tSqrBuf[55];//物料5
		Fy2CanLst[0x08]=0x81;
		Fy2CanLst[0x09]=tSqrBuf[56];//物料6
		Fy2CanLst[0x0A]=tSqrBuf[57];//物料7
		Fy2CanLst[0x0B]=tSqrBuf[58];//物料8
		Fy2CanLst[0x0C]=tSqrBuf[59];//物料9
		Fy2CanLst[0x0D]=tSqrBuf[27];//开始时间1
		Fy2CanLst[0x0E]=tSqrBuf[28];//开始时间2
		Fy2CanLst[0x0F]=tSqrBuf[29];//开始时间3
		Fy2CanLst[0x10]=0x82;
		Fy2CanLst[0x11]=tSqrBuf[7];//应发量1
		Fy2CanLst[0x12]=tSqrBuf[8];//应发量2
		Fy2CanLst[0x13]=tSqrBuf[9];//应发量3
		Fy2CanLst[0x14]=tSqrBuf[10];//应发量4
		Fy2CanLst[0x15]=tSqrBuf[30];//开始时间5
		Fy2CanLst[0x16]=tSqrBuf[31];//开始时间6
		Fy2CanLst[0x17]=tSqrBuf[32];//开始时间7
		Fy2CanLst[0x18]=0x83;
		Fy2CanLst[0x19]=tSqrBuf[11];//实发重量1
		Fy2CanLst[0x1A]=tSqrBuf[12];//实发重量2
		Fy2CanLst[0x1B]=tSqrBuf[13];//实发重量3
		Fy2CanLst[0x1C]=tSqrBuf[14];//实发重量4
		Fy2CanLst[0x1D]=tSqrBuf[33];//结束时间1
		Fy2CanLst[0x1E]=tSqrBuf[34];//结束时间2
		Fy2CanLst[0x1F]=tSqrBuf[35];//结束时间3
		Fy2CanLst[0x20]=0x84;
		Fy2CanLst[0x21]=0;//实发体积1
		Fy2CanLst[0x22]=0;//实发体积2
		Fy2CanLst[0x23]=0;//实发体积3
		Fy2CanLst[0x24]=0;//实发体积4
		Fy2CanLst[0x25]=tSqrBuf[36];//结束时间4
		Fy2CanLst[0x26]=tSqrBuf[37];//结束时间5
		Fy2CanLst[0x27]=tSqrBuf[38];//结束时间6
		Fy2CanLst[0x28]=0x85;
		Fy2CanLst[0x29]=flMet1[0];//前表数1
		Fy2CanLst[0x2A]=flMet1[1];//前表数2
		Fy2CanLst[0x2B]=flMet1[2];//前表数3
		Fy2CanLst[0x2C]=flMet1[3];//前表数4
		Fy2CanLst[0x2D]=tSqrBuf[45];//介密1
		Fy2CanLst[0x2E]=tSqrBuf[46];//介密2
		Fy2CanLst[0x2F]=0x55;//发油单位:55H质量；0AAH体积
		Fy2CanLst[0x30]=0x86;
		Fy2CanLst[0x31]=flMet2[0];//后表数1
		Fy2CanLst[0x32]=flMet2[1];//后表数2
		Fy2CanLst[0x33]=flMet2[2];//后表数3
		Fy2CanLst[0x34]=flMet2[3];//后表数4
		Fy2CanLst[0x35]=tSqrBuf[47];//介密3
		Fy2CanLst[0x36]=tSqrBuf[48];//介密4
		Fy2CanLst[0x37]=tAdd;//记录
		Fy2CanLst[0x38]=0xC0;
		Fy2CanLst[0x39]=tSqrBuf[3];//密码1
		Fy2CanLst[0x3A]=tSqrBuf[4];//密码2
		Fy2CanLst[0x3B]=tSqrBuf[5];//密码3
		Fy2CanLst[0x3C]=tSqrBuf[6];//密码4
		Fy2CanLst[0x3D]=0x55;//LstFyBuf[54];//
		Fy2CanLst[0x3E]=0xAA;//LstFyBuf[55];//
		//FillLstCAN(id);//查询记录   //需要增加
		flgLstTFTok=dualChNum;cntLstTFT=9;cntCanFb=8;canRtCnt=0;
}

void AddNewPendLst(unsigned char id)
{
	unsigned char ti,tj,i;
	unsigned char tflg[10];//标志位，0为记录无效，1为记录有效
	char tBuf[30];
	NewPendBuf[25]=0;
//	for(i=0;i<10;i++)
//		tPendAddr[i]=0x4000*id+PendAdd[i];
	for(ti=0;ti<10;ti++)
	{
		//EEP_Read(tBuf,PendAdd[ti],20);
		if(id==1)
			EepRd((250+PendAdd[ti]),25);
		else
			EepRd(PendAdd[ti],25);	

		for(i=0;i<25;i++)
			tBuf[i]=EepRdBuf[i];
		tflg[ti]=1;
		if(tBuf[0]!=(0xF1+ti))//记录号为0xF0+id
			tflg[ti]=0;
		if(tflg[ti]>0)
		{
		  	for(tj=1;tj<5;tj++)
				{	 //提油密码不正确
				 	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))
					{
						tflg[ti]=0;
					}
				}
		}
	}
	cntNewPendLst=0;	
	for(ti=9;ti>0;ti--)
	{
			//if(id==0)
			//  EepRd(PendAdd[ti-1],25);
			//else
			//  EepRd((250+PendAdd[ti-1]),25);
			//
			//for(i=0;i<25;i++)
			//	tBuf[i]=EepRdBuf[i];
			//if(tflg[ti-1]==1)
			//{
			//	tBuf[0]=0xF1+ti;
			//}
			for(i=0;i<25;i++)
				EepWrBuf[i]=0xFF;//清掉剩余提单，火车发油，只存一单//EepWrBuf[i]=tBuf[i];
			if(id==1)
				EepWr((250+PendAdd[ti]),25);
			else
				EepWr(PendAdd[ti],25);
	}
	tBuf[0]=0xF1;
	for(ti=0;ti<25;ti++)
	{
		tBuf[ti+1]=NewPendBuf[ti];
	}
	for(i=0;i<25;i++)
		EepWrBuf[i]=tBuf[i];

	if(id==1)
		EepWr((250+PendAdd[0]),25);	//EepWr(tPendAddr[0],25);
	else
		EepWr(PendAdd[0],25);	//EepWr(tPendAddr[0],25);
//	lcd_text16(290,427,0,GOLD,"New Pend List Add!          ");
	if(frmNum==FRM_PENDLST)
	{
			for(i=0;i<12;i++)
				Draw_Line( 30,80+i*30,790,80+i*30,1,BLACK);
			Draw_Line( 30,50,790,50,1,BLACK);
			Draw_Line( 30,50, 30,410,1,BLACK);
			Draw_Line( 70,80, 70,410,1,BLACK);
			Draw_Line(250,80,250,410,1,BLACK);
			Draw_Line(470,80,470,410,1,BLACK);
			Draw_Line(670,80,670,410,1,BLACK);
			Draw_Line(790,50,790,410,1,BLACK);
		Dis_Pend(dualChNum-1);
	}
//if(frmNum==FRM_PENDSEL)
//{
//	if((sys_Conn==1))//在联网方式下由安检卡直接刷入
//	{
//		Get_Pend_Lst(dualChNum);
//		for(ti=0;ti<8;ti++)
//		{	
//			idCarId[ti]=chrCarId[dualChNum-1][ti];
//			 idCarYFL[ti]=chrYFL[dualChNum-1][ti];
//		}
//		 idCarId[8]=0;idCarYFL[8]=0;
//		lcd_en48(393,176,BLUE,WHITE,idCarId);
//		lcd_en48(393,304,BLUE,WHITE,idCarYFL);
//	}
//}
	
	if(frmNum==FRM_MAIN)
	{
	// if(sys_Conn==1)
	 //{
		dualChNum=id+1;
		frmNumLast=0xFF;frmNum=FRM_MAIN;
		Get_Pend_Lst(dualChNum);
			/////Dis_CarId(1);
	 //}
	}
	else
	{
		dualChNum=id+1;
		frmNum=FRM_MAIN;
	}
	//if(frmNum==FRM_MON)
	//{
	//Dis_1_Pend(1);
	//}
	Fnd_PendLst();
	return ;
}

void Del_PendAll(unsigned char id)
{
	unsigned int i;
	for(i=260;i<510;i++)
	{
		if(id==1)
			EEPROM_ByteWrite(250+i,0xFF);
		else
			EEPROM_ByteWrite(i,0xFF);
	}
	dYFLv=0.0;
	YFL[id]=0.0;
	if(frmNum==FRM_PENDLST)
	{		Dis_Pend(dualChNum-1);	 	}
	if(frmNum==FRM_MAIN)
	{
		//ShowStr30(20,82,"        ",8,BLACK,WHITE);
		//ShowStr30(400,52,"        ",8,BLACK,WHITE);
		;//lcd_text32((unsigned short)30,(unsigned short)15,BLACK,WHITE,"        ");
		//TFT_Fill(355,87,355+128,112,BLUE);
  	//TFT_Fill(528,87,528+128+5,112,BLUE);
	}
	u8PendNum[0]=0;u8PendNum[1]=0;
}

//清除提单
void Del_Pend(unsigned char id,char *OrdId)
{
	char tDelBuf[9];
	char tChkBuf[9];
//	char tBuf[20];
//	char tDisBuf[10];
	unsigned char ti,tj;
//	unsigned char tflg[10];
	unsigned char dIndex=100;
//	unsigned char tNewCnt;
	unsigned char i;
	DelPendBuf[9]=0;
	for(ti=0;ti<8;ti++)
		tDelBuf[ti]=OrdId[ti];
	tDelBuf[8]=0;
	for(ti=0;ti<10;ti++)
	{
		if(id==1)
			EepRd((250+PendAdd[ti]),25);
	  else
			EepRd(PendAdd[ti],25);
		for(tj=0;tj<8;tj+=2)
		{	
			tChkBuf[tj+0]='0'+(EepRdBuf[(tj/2)+1]>>4);
			tChkBuf[tj+1]='0'+(EepRdBuf[(tj/2)+1]&0x0f);
		}
		tChkBuf[8]=0;
	 if(strcmp(tDelBuf,tChkBuf)==0)
	 {dIndex=ti;break; }		
	}
	if(dIndex==100)
	{
		//lcd_text16(290,427,0,GOLD,"No Pend List Del!          ");
		return;
	}

	for(ti=(dIndex+1);ti<10;ti++)
	{
		if(id==1)
			EepRd((250+PendAdd[ti]),25);
	  else
			EepRd(PendAdd[ti],25);
//		tBuf[0]=0xF0+ti;//0xF1+ti-1;
		for(i=0;i<25;i++)
			EepWrBuf[i]=EepRdBuf[i];
		EepWrBuf[0]=0xF0+ti;
		if(id==1)
			EepWr((250+PendAdd[ti-1]),25);
	  else
			EepWr(PendAdd[ti-1],25);
	}	
	for(i=0;i<25;i++)
		EepWrBuf[i]=0xFF;
	if(id==1)
		EepWr((250+PendAdd[9]),25);
  else
		EepWr(PendAdd[9],25);
	//lcd_text16(290,427,0,GOLD,"One Pend List Del!          ");
  u8PendNum[0]=dGetPendNum(0);u8PendNum[1]=dGetPendNum(1);//u8PendNum=GetPendNum();
	if(u8PendNum[id]==0)
	{
		Del_PendAll(id);
		for(i=0;i<8;i++)
		{
		 chrOilId[id][i]=' ';   idCarYFL[i]=' ';
		 chrYFL[id][i]=' ';	 idCarTypeId[i]=' ';
		 chrCarId[id][i]=' ';	   idCarId[i]=' ';
		 frmMainOrdId[i]=' ';
		 frmMainYFval[i]=' ';
		}
	}
	if((frmNum==FRM_MAIN)&&(dualChNum==(id+1)))
	{
		dYFLv=0.0;
		for(i=0;i<8;i++)
		{
			frmMainOrdId[i]=' ';
			frmMainYFval[i]=' ';
			CarId[i]=' ';
			LeadId[i]=' ';
		}
		/////Dis_CarId(1);
		//ShowStr30(20,82,frmMainOrdId,8,BLACK,WHITE);
		ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE);
	}
	if((frmNum==FRM_PENDSEL)&&(dualChNum==(id+1)))
	{
		dYFLv=0.0;
		for(i=0;i<8;i++)
		{	
			idCarId[i]=' ';
			chrCarId[dualChNum-1][i]=' ';
		  idCarYFL[i]=' ';
			chrYFL[dualChNum-1][i]=' ';
		 }
		 idCarId[8]=0;idCarYFL[8]=0;
		TFT_Fill(393,112+2,393+188,112+44,WHITE);
		TFT_Fill(393,176+2,393+188,176+44,WHITE);
		//TFT_Fill(393,240,393+192,240+48,WHITE);
		TFT_Fill(393,304+2,393+188,304+44,WHITE);
	}
	if(frmNum==FRM_PENDLST)
	{		Dis_Pend(dualChNum-1);	 	}
}

extern unsigned char sys_Conn;
void ClrPendYfl(void)
{
	unsigned char i;
	if(dYFLv==0)
	{
		for(i=0;i<8;i++)
		{
			frmMainOrdId[i]=0;
			frmMainYFval[i]=0;
		}
	}
	if(sys_Conn==1)//联网状态下没有提单就清单应发量
	{
		dYFLv=0.0;
		for(i=0;i<8;i++)
		{
			frmMainOrdId[i]=0;
			frmMainYFval[i]=0;
		}
	}
}
extern char lstOilId[2][4],chrLeadId[2][9];
void Fnd_PendLst(void)
{
	char *tmpYlf;
	char tBuf[30];
	unsigned char ti,tj,i;
	unsigned char tflg;//标志位，0为记录无效，1为记录有效
  unsigned long tLngDat,tDat,tLngDatHi,tLngDatLo;

	flgPendLst=1;
	//先读第一路提单
	EepRd(PendAdd[0],25);
	for(ti=0;ti<25;ti++)
		tBuf[ti]=EepRdBuf[ti];
	tflg=1;
	if(tBuf[0]!=0xF1) tflg=0;//记录号为0xF0+id
	if(tflg>0)//记录号正确
	{
		for(tj=1;tj<5;tj++)
		{	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))//提油密码不正确
			{		tflg=0;			}
		}
	}
	if(tflg>0)//提单有效
	{
		//提货单号
		tDat=tBuf[1]>>4;
		tLngDat =tDat*10000000;
		tDat=tBuf[1]&0x0F;
		tLngDat+=tDat*1000000;
		tDat=tBuf[2]>>4;
		tLngDat+=tDat*100000;
		tDat=tBuf[2]&0x0F;
		tLngDat+=tDat*10000;
		tDat=tBuf[3]>>4;
		tLngDat+=tDat*1000;
		tDat=tBuf[3]&0x0F;
		tLngDat+=tDat*100;
		tDat=tBuf[4]>>4;
		tLngDat+=tDat*10;
		tDat=tBuf[4]&0x0F;
		tLngDat+=tDat;
		tLngDatHi=tLngDat>>16;
		tLngDatLo=tLngDat&0xFFFF;
		canJobData[7]=tLngDatLo>>8;//canJobData[7],[8],[9],[10]提货单号
		canJobData[8]=tLngDatLo&0xFF;
		canJobData[9]=tLngDatHi>>8;
		canJobData[10]=tLngDatHi&0xFF;
		tDat=tBuf[6]>>4;
		tLngDat =tDat*1000000;
		tDat=tBuf[6]&0x0F;
		tLngDat+=tDat*100000;
		tDat=tBuf[7]>>4;
		tLngDat+=tDat*10000;
		tDat=tBuf[7]&0x0F;
		tLngDat+=tDat*1000;
		tDat=tBuf[8]>>4;
		tLngDat+=tDat*100;
		tDat=tBuf[8]&0x0F;
		tLngDat+=tDat*10;
		tDat=tBuf[9]>>4;
		tLngDat+=tDat;
		tLngDatHi=tLngDat>>16;
		tLngDatLo=tLngDat&0xFFFF;
		//32000=0x00007D00,[7D][00][00][00]
		//8888888=0x87A238,[A2][38][00][87]
		//canJobData[11]=0xA2,canJobData[12]=0x38,canJobData[13]=0x87
		//canJobData[7]=tLngDatLo>>8; 
		canJobData[13]=tLngDatHi&0xFF;//应发量 本应Lo[h] Lo[l] Hi[h] Hi[l]
		canJobData[11]=tLngDatLo>>8;
		canJobData[12]=tLngDatLo&0xFF;
		for(ti=0;ti<7;ti++)
		{			canJobData[ti]=tBuf[10+ti];}
		
	}
	else
	{
		for(ti=0;ti<14;ti++)
		{			canJobData[ti]=0;}
		
	}
	//读第二路
	EepRd((250+PendAdd[0]),25);
	for(ti=0;ti<25;ti++)
		tBuf[ti]=EepRdBuf[ti];
	tflg=1;
	if(tBuf[0]!=0xF1) tflg=0;//记录号为0xF0+id
	if(tflg>0)//记录号正确
	{
		for(tj=1;tj<5;tj++)
		{	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))//提油密码不正确
			{		tflg=0;			}
		}
	}
	if(tflg>0)//提单有效
	{
		//提货单号
		tDat=tBuf[1]>>4;
		tLngDat =tDat*10000000;
		tDat=tBuf[1]&0x0F;
		tLngDat+=tDat*1000000;
		tDat=tBuf[2]>>4;
		tLngDat+=tDat*100000;
		tDat=tBuf[2]&0x0F;
		tLngDat+=tDat*10000;
		tDat=tBuf[3]>>4;
		tLngDat+=tDat*1000;
		tDat=tBuf[3]&0x0F;
		tLngDat+=tDat*100;
		tDat=tBuf[4]>>4;
		tLngDat+=tDat*10;
		tDat=tBuf[4]&0x0F;
		tLngDat+=tDat;
		tLngDatHi=tLngDat>>16;
		tLngDatLo=tLngDat&0xFFFF;
		canJobData[21]=tLngDatLo>>8;
		canJobData[22]=tLngDatLo&0xFF;
		canJobData[23]=tLngDatHi>>8;
		canJobData[24]=tLngDatHi&0xFF;
		tDat=tBuf[6]>>4;
		tLngDat =tDat*1000000;
		tDat=tBuf[6]&0x0F;
		tLngDat+=tDat*100000;
		tDat=tBuf[7]>>4;
		tLngDat+=tDat*10000;
		tDat=tBuf[7]&0x0F;
		tLngDat+=tDat*1000;
		tDat=tBuf[8]>>4;
		tLngDat+=tDat*100;
		tDat=tBuf[8]&0x0F;
		tLngDat+=tDat*10;
		tDat=tBuf[9]>>4;
		tLngDat+=tDat;
		tLngDatHi=tLngDat>>16;
		tLngDatLo=tLngDat&0xFFFF;
		//canJobData[7]=tLngDatLo>>8;
		canJobData[25]=tLngDatLo>>8;//应发量 本应Lo[h] Lo[l] Hi[h-no] Hi[l]
		canJobData[26]=tLngDatLo&0xFF;
		canJobData[27]=tLngDatHi&0xFF;
		for(ti=0;ti<7;ti++)
		{			canJobData[14+ti]=tBuf[10+ti];}
		
	}
	else
	{
		for(ti=0;ti<14;ti++)
		{			canJobData[14+ti]=0;}
	}

		FillJobCAN();
}


void Dis_1_Pend(unsigned char IdPend)
{
	char *tmpYlf;
	char tBuf[30];
//	char tDisBuf[20];
//记录存储在10扇区与11扇区中
//	unsigned char tCrC;
	unsigned char ti,tj;
	unsigned char tflg;//标志位，0为记录无效，1为记录有效
	//if((frmNum!=FRM_MAIN)&&(frmNum!=FRM_MON))
	//	return;
	
 		flgPendLst=1;
	//if((frmNum==FRM_MON)&&(sys_Conn==1))//在监控界面时，同时显示最后一笔提单
	if((frmNum==FRM_MON))//在监控界面时，同时显示最后一笔提单
	{
		TFT_Fill(  6,404,134,429,BLUE);//TFT_Fill(137,404,264,429,BLUE);
		TFT_Fill(406,404,534,429,BLUE);//TFT_Fill(537,404,664,429,BLUE);
		Draw_Line(535, 375,535,431,2,BLACK);Draw_Line(664, 375,664,431,2,BLACK);
		Draw_Line(535, 375,535,431,2,BLACK);Draw_Line(664, 375,664,431,2,BLACK);
	  if(1)//if((sys_Conn==1)||((sys_Conn==0)&&(YFL[0]==0)))
		{
			//先读第一路提单
			EepRd(PendAdd[0],25);
			for(ti=0;ti<25;ti++)
				tBuf[ti]=EepRdBuf[ti];
			tflg=1;
			if(tBuf[0]!=0xF1) tflg=0;//记录号为0xF0+id
			if(tflg>0)//记录号正确
			{
				for(tj=1;tj<5;tj++)
				{	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))//提油密码不正确
					{		tflg=0;			}
				}
			}
			if(tflg>0)//验证该条记录是正确的
			{		
				for(ti=0;ti<4;ti++)
					lstOilId[0][ti]=tBuf[1+ti];
				chrOilId[0][0]='0'+(tBuf[1]>>4);
				chrOilId[0][1]='0'+(tBuf[1]&0x0F);
				chrOilId[0][2]='0'+(tBuf[2]>>4);
				chrOilId[0][3]='0'+(tBuf[2]&0x0F);
				chrOilId[0][4]='0'+(tBuf[3]>>4);
				chrOilId[0][5]='0'+(tBuf[3]&0x0F);
				chrOilId[0][6]='0'+(tBuf[4]>>4);
				chrOilId[0][7]='0'+(tBuf[4]&0x0F);
				//chrOilId[0][8]=0;
				//ShowStr30(20,82,frmMainOrdId,8,BLACK,WHITE);
				//ShowStr30(15,67,frmMainOrdId,8,BLACK,WHITE);
				chrYFL[0][1]='0'+(tBuf[6]>>4);
				chrYFL[0][2]='0'+(tBuf[6]&0x0F);
				chrYFL[0][3]='0'+(tBuf[7]>>4);
				chrYFL[0][4]='0'+(tBuf[7]&0x0F);
				chrYFL[0][5]='0'+(tBuf[8]>>4);
				chrYFL[0][6]='0'+(tBuf[8]&0x0F);
				chrYFL[0][7]='0'+(tBuf[9]>>4);
				//tDisBuf[7]='0'+(tBuf[9]&0x0F);
				//frmMainYFval[8]=0;
				chrYFL[0][0]='0';chrYFL[0][8]=0;
				for(tj=0;tj<8;tj++)
				{
					if(chrYFL[0][tj]=='0')			chrYFL[0][tj]=' ';
					else						break;
				}
				DisNum24(210,70,BLUE,WHITE,chrYFL[0]);//GREEN

				for(ti=0;ti<8;ti++)
				{
					chrCarId[0][ti]=tBuf[10+ti];
				}
				DisNum24( 20,405-3,WHITE,BLUE,chrCarId[0]);//GREEN
				//Draw_EN24(140,405,WHITE,BLUE,chrLeadId[0]);//GREEN
			}
		}
	  if(1)//if((sys_Conn==1)||((sys_Conn==0)&&(YFL[1]==0)))
		{
			//再读第二路提单
			EepRd((250+PendAdd[0]),25);
			for(ti=0;ti<25;ti++)
				tBuf[ti]=EepRdBuf[ti];
			tflg=1;
			if(tBuf[0]!=0xF1) tflg=0;//记录号为0xF0+id
			if(tflg>0)//记录号正确
			{
				for(tj=1;tj<5;tj++)
				{	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))//提油密码不正确
					{		tflg=0;			}
				}
			}
			if(tflg>0)//验证该条记录是正确的
			{		
				for(ti=0;ti<4;ti++)
					lstOilId[1][ti]=tBuf[1+ti];
				chrOilId[1][0]='0'+(tBuf[1]>>4);
				chrOilId[1][1]='0'+(tBuf[1]&0x0F);
				chrOilId[1][2]='0'+(tBuf[2]>>4);
				chrOilId[1][3]='0'+(tBuf[2]&0x0F);
				chrOilId[1][4]='0'+(tBuf[3]>>4);
				chrOilId[1][5]='0'+(tBuf[3]&0x0F);
				chrOilId[1][6]='0'+(tBuf[4]>>4);
				chrOilId[1][7]='0'+(tBuf[4]&0x0F);
				chrYFL[1][1]='0'+(tBuf[6]>>4);
				chrYFL[1][2]='0'+(tBuf[6]&0x0F);
				chrYFL[1][3]='0'+(tBuf[7]>>4);
				chrYFL[1][4]='0'+(tBuf[7]&0x0F);
				chrYFL[1][5]='0'+(tBuf[8]>>4);
				chrYFL[1][6]='0'+(tBuf[8]&0x0F);
				chrYFL[1][7]='0'+(tBuf[9]>>4);
				chrYFL[1][0]='0';chrYFL[1][8]=0;
				for(tj=0;tj<8;tj++)
				{
					if(chrYFL[1][tj]=='0')			chrYFL[1][tj]=' ';
					else						break;
				}
				DisNum24(610,70,BLUE,WHITE,chrYFL[1]);//GREEN
				for(ti=0;ti<8;ti++)
				{
					chrCarId[1][ti]=tBuf[10+ti];
				}
				DisNum24(420,405-3,WHITE,BLUE,chrCarId[1]);//GREEN
				//Draw_EN24(540,405,WHITE,BLUE,chrLeadId[1]);//GREEN
		 }
	 }
	}
if((frmNum==FRM_MAIN))
{
  if(1)//if((sys_Conn==1)||((sys_Conn==0)&&(YFL[dualChNum-1]==0)))
	{
		if(dualChNum==1)
			EepRd(PendAdd[0],25);
		if(dualChNum==2)
			EepRd((250+PendAdd[0]),25);
		for(ti=0;ti<25;ti++)
			tBuf[ti]=EepRdBuf[ti];
		tflg=1;
		if(tBuf[0]!=0xF1)//记录号为0xF0+id
			tflg=0;
		if(tflg>0)
		{//记录号正确
			if(tflg>0)
			{
				for(tj=1;tj<5;tj++)
				{	 //提油密码不正确
				 	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))
					{
						tflg=0;
					}
				}
			}
		}
		if(tflg>0)//验证该条记录是正确的
		{		
			for(ti=0;ti<4;ti++)
				lstOilId[dualChNum-1][ti]=tBuf[1+ti];
			frmMainOrdId[0]='0'+(tBuf[1]>>4);
			frmMainOrdId[1]='0'+(tBuf[1]&0x0F);
			frmMainOrdId[2]='0'+(tBuf[2]>>4);
			frmMainOrdId[3]='0'+(tBuf[2]&0x0F);
			frmMainOrdId[4]='0'+(tBuf[3]>>4);
			frmMainOrdId[5]='0'+(tBuf[3]&0x0F);
			frmMainOrdId[6]='0'+(tBuf[4]>>4);
			frmMainOrdId[7]='0'+(tBuf[4]&0x0F);
			frmMainOrdId[8]=0;
			//ShowStr30(20,82,frmMainOrdId,8,BLACK,WHITE);
			frmMainYFval[1]='0'+(tBuf[6]>>4);
			frmMainYFval[2]='0'+(tBuf[6]&0x0F);
			frmMainYFval[3]='0'+(tBuf[7]>>4);
			frmMainYFval[4]='0'+(tBuf[7]&0x0F);
			frmMainYFval[5]='0'+(tBuf[8]>>4);
			frmMainYFval[6]='0'+(tBuf[8]&0x0F);
			frmMainYFval[7]='0'+(tBuf[9]>>4);
			//tDisBuf[7]='0'+(tBuf[9]&0x0F);
			frmMainYFval[8]=0;
			frmMainYFval[0]='0';
			if(dualChNum==1)//第1路 提货单号
			{
				CanData[104]=tBuf[3];CanData[105]=tBuf[4];CanData[106]=tBuf[1];CanData[107]=tBuf[2];
				for(ti=0;ti<8;ti++)
					CanData[97+ti]=tBuf[10+ti];
			}
			if(dualChNum==2)//第2路 提货单号
			{
				CanData[133]=tBuf[3];CanData[134]=tBuf[4];CanData[135]=tBuf[1];CanData[136]=tBuf[2];
				for(ti=0;ti<8;ti++)
					CanData[125+ti]=tBuf[10+ti];
			}
			
			for(tj=0;tj<8;tj++)
			{
				if(frmMainYFval[tj]=='0')
					frmMainYFval[tj]=' ';
				else
					break;
			}
			for(ti=0;ti<9;ti++)
			{
				if((frmMainOrdId[ti]>0x2F)&&(frmMainOrdId[ti]<0x3A))//不为空
					nowPendLst[dualChNum-1][ti]=frmMainOrdId[ti];
				else
					nowPendLst[dualChNum-1][ti]=0x30;
			}
			tmpYlf=GetPendYFL(dualChNum-1,frmMainOrdId);//查询待发记录
	
			ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE);
			dYFLv=calYFL();dYFL[dualChNum-1]=dYFLv;
			YFL[dualChNum-1]=dYFLv;
			for(ti=0;ti<8;ti++)
			{
				chrCarId[dualChNum-1][ti]=tBuf[10+ti];
				CarId[ti]=tBuf[10+ti];
			}
	  	//TFT_Fill(355,87,355+128,112,BLUE);
			//Draw_EN24(365,88,WHITE,BLUE,CarId);//CarIdCnt=0;
			DisNum24(245+24,352,0xFFFF,BLUE,CarId);
		}
		else
		{
			for(ti=0;ti<8;ti++)
			{
				chrCarId[dualChNum-1][ti]=' ';;
				CarId[ti]=' ';
			}
	  	//TFT_Fill(355,87,355+128,112,BLUE);
			//Draw_EN24(365,88,WHITE,BLUE,CarId);//CarIdCnt=0;
		}
	}
	}
}
void Get_Pend_Lst(unsigned char ChNum)//待发确认界面下读取待发记录
{
	char *tmpYlf;
	char tBuf[30];
//	char tDisBuf[20];
//记录存储在10扇区与11扇区中
//	unsigned char tCrC;
	unsigned char ti,tj;
	unsigned char tflg;//标志位，0为记录无效，1为记录有效
			//先读第一路提单
	   if(ChNum==1)
		 {
			EepRd(PendAdd[0],25);
		 }
		 if(ChNum==2)
		 {
			EepRd((250+PendAdd[0]),25);
		 }
			for(ti=0;ti<25;ti++)
				tBuf[ti]=EepRdBuf[ti];
			tflg=1;
			if(tBuf[0]!=0xF1) tflg=0;//记录号为0xF0+id
			if(tflg>0)//记录号正确
			{
				for(tj=1;tj<5;tj++)
				{	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))//提油密码不正确
					{		tflg=0;			}
				}
			}
			if(tflg>0)//验证该条记录是正确的
			{		
				for(ti=0;ti<4;ti++)
					lstOilId[ChNum-1][ti]=tBuf[1+ti];
				chrOilId[ChNum-1][0]='0'+(tBuf[1]>>4);
				chrOilId[ChNum-1][1]='0'+(tBuf[1]&0x0F);
				chrOilId[ChNum-1][2]='0'+(tBuf[2]>>4);
				chrOilId[ChNum-1][3]='0'+(tBuf[2]&0x0F);
				chrOilId[ChNum-1][4]='0'+(tBuf[3]>>4);
				chrOilId[ChNum-1][5]='0'+(tBuf[3]&0x0F);
				chrOilId[ChNum-1][6]='0'+(tBuf[4]>>4);
				chrOilId[ChNum-1][7]='0'+(tBuf[4]&0x0F);
				//chrOilId[0][8]=0;
				//ShowStr30(20,82,frmMainOrdId,8,BLACK,WHITE);
				//ShowStr30(15,67,frmMainOrdId,8,BLACK,WHITE);
				chrYFL[ChNum-1][1]='0'+(tBuf[6]>>4);
				chrYFL[ChNum-1][2]='0'+(tBuf[6]&0x0F);
				chrYFL[ChNum-1][3]='0'+(tBuf[7]>>4);
				chrYFL[ChNum-1][4]='0'+(tBuf[7]&0x0F);
				chrYFL[ChNum-1][5]='0'+(tBuf[8]>>4);
				chrYFL[ChNum-1][6]='0'+(tBuf[8]&0x0F);
				chrYFL[ChNum-1][7]='0'+(tBuf[9]>>4);
				//tDisBuf[7]='0'+(tBuf[9]&0x0F);
				//frmMainYFval[8]=0;
				chrYFL[ChNum-1][0]='0';
				for(tj=0;tj<8;tj++)
				{
					if(chrYFL[ChNum-1][tj]=='0')			chrYFL[ChNum-1][tj]=' ';
					else						break;
				}
				for(ti=0;ti<8;ti++)
				{
					chrCarId[ChNum-1][ti]=tBuf[10+ti];
				}
				}
				else
				{
					for(ti=0;ti<8;ti++)
					{
					  chrCarId[ChNum-1][ti]=' ';
						  chrYFL[ChNum-1][ti]=' ';
					}
				}
}

unsigned char GetLstNum(void)
{
	unsigned int i,j;
	char tBuf[80];
	unsigned char tAvd,tLstCnt;
	//unsigned char tCrc;
  unsigned int tRtn;//返回值
	for(i=0;i<200;i++)
	{
		EepRd(0x340+i*80,3);
		for(j=0;j<3;j++)
			tBuf[j]=EepRdBuf[j];
		EepRd(0x340+79+i*80,1);
		tBuf[79]=EepRdBuf[0];
		tAvd=0;
		if(((tBuf[0]&0xF0)==0xA0)&&((tBuf[1]&0xF0)==0x50)&&(tBuf[2]==0x3A)&&(tBuf[79]==0x5F))
		{
			 tLstCnt=((tBuf[0]&0x0F)<<4)+(tBuf[1]&0x0F);
			 if((tLstCnt>0)&&(tLstCnt<210))//记录号1~210,超过为错误记录号
			 {
			 	 tAvd=1;
			 }
			}
			if(tAvd==1)//有效记录
			{
				flgLst[i]=tLstCnt;
			}
			else
			{
				flgLst[i]=0;
			}
	}
	tRtn=0;
 for(i=0;i<200;i++)
 {
 	if(flgLst[i]>0)
		tRtn++;
	if(tRtn>200)
		return 200;
 }
	return tRtn;
}

//显示最后一条记录
void Dis_Id_Pend(unsigned char IdPend)
{
//char *tmpYlf;
//char tBuf[30];
////	char tDisBuf[20];
////记录存储在10扇区与11扇区中
////	unsigned char tCrC;
//unsigned char ti,tj;
//unsigned char tflg;//标志位，0为记录无效，1为记录有效
//if((frmNum!=FRM_MAIN)&&(frmNum!=FRM_MON))
//	return;
//	flgPendLst=1;
//if(dualChNum==1)
//	EepRd(PendAdd[IdPend-1],25);
//if(dualChNum==2)
//	EepRd((250+PendAdd[IdPend-1]),25);
//for(ti=0;ti<25;ti++)
//	tBuf[ti]=EepRdBuf[ti];
//tflg=1;
//if(tBuf[0]!=(0xF1+(IdPend-1)))//记录号为0xF0+id
//	tflg=0;
//if(tflg>0)
//{//记录号正确
//	if(tflg>0)
//	{
//		for(tj=1;tj<5;tj++)
//		{	 //提油密码不正确
//		 	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))
//			{
//				tflg=0;
//			}
//		}
//	}
//}
//if(tflg>0)//验证该条记录是正确的
//{		
//	frmMainOrdId[0]='0'+(tBuf[1]>>4);
//	frmMainOrdId[1]='0'+(tBuf[1]&0x0F);
//	frmMainOrdId[2]='0'+(tBuf[2]>>4);
//	frmMainOrdId[3]='0'+(tBuf[2]&0x0F);
//	frmMainOrdId[4]='0'+(tBuf[3]>>4);
//	frmMainOrdId[5]='0'+(tBuf[3]&0x0F);
//	frmMainOrdId[6]='0'+(tBuf[4]>>4);
//	frmMainOrdId[7]='0'+(tBuf[4]&0x0F);
//	frmMainOrdId[8]=0;
//	ShowStr30(20,82,frmMainOrdId,8,BLACK,WHITE);
//	frmMainYFval[1]='0'+(tBuf[6]>>4);
//	frmMainYFval[2]='0'+(tBuf[6]&0x0F);
//	frmMainYFval[3]='0'+(tBuf[7]>>4);
//	frmMainYFval[4]='0'+(tBuf[7]&0x0F);
//	frmMainYFval[5]='0'+(tBuf[8]>>4);
//	frmMainYFval[6]='0'+(tBuf[8]&0x0F);
//	frmMainYFval[7]='0'+(tBuf[9]>>4);
//	//tDisBuf[7]='0'+(tBuf[9]&0x0F);
//	frmMainYFval[8]=0;
//	frmMainYFval[0]='0';
//	for(tj=0;tj<8;tj++)
//	{
//		if(frmMainYFval[tj]=='0')
//			frmMainYFval[tj]=' ';
//		else
//			break;
//	}
//	for(ti=0;ti<9;ti++)
//	{
//		if((frmMainOrdId[ti]>0x2F)&&(frmMainOrdId[ti]<0x3A))//不为空
//			nowPendLst[0][ti]=frmMainOrdId[ti];
//		else
//			nowPendLst[0][ti]=0x30;
//	}
//	tmpYlf=GetPendYFL(,frmMainOrdId);//查询待发记录
//
//ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE);
//dYFLv=calYFL();
//YFL[dualChNum-1]=dYFLv;
//}
}
/*取得有效提单数目*/
unsigned char dGetPendNum(unsigned char id)
{
//记录存储在10扇区与11扇区中
//	unsigned char tCrC;
	unsigned char ti,tj,i,j;
	unsigned char tflg[10];//标志位，0为记录无效，1为记录有效
	char tBuf[30];
	char tDisBuf[10];
	unsigned char tLstNum;
	unsigned long uwyfl;

	for(ti=0;ti<10;ti++)
	{
	 	if(id==1)
			EepRd((250+PendAdd[ti]),25);
		else
			EepRd(PendAdd[ti],25);

		for(i=0;i<25;i++)
		{
			PendBuf[ti][i]=EepRdBuf[i];
			        tBuf[i]=EepRdBuf[i];
		}
		tflg[ti]=1;
		if(tBuf[0]!=(0xF1+ti))//记录号为0xF0+id
			tflg[ti]=0;
		if(tflg[ti]>0)
		{//记录号正确
			if(tflg[ti]>0)
			{
				for(tj=1;tj<5;tj++)
				{	 //提油密码不正确
				 	if(((tBuf[tj]&0xF0)>0x90)||((tBuf[tj]&0x0F)>0x09))
					{
						tflg[ti]=0;
					}
				}
			 uwyfl =(tBuf[6]>>4)	*1000000;
			 uwyfl+=(tBuf[6]&0x0F)*100000;
			 uwyfl+=(tBuf[7]>>4)	*10000;
			 uwyfl+=(tBuf[7]&0x0F)*1000;
			 uwyfl+=(tBuf[8]>>4)	*100;
			 uwyfl+=(tBuf[8]&0x0F)*10;
			 uwyfl+=(tBuf[9]>>4)	*1;
			if(uwyfl==0)
				tflg[ti]=0;

			}
		}
	}
	tLstNum=0;
	for(ti=0;ti<10;ti++)
	{
		if(tflg[ti]==1)
			tLstNum++;
		else
		{
			for(j=0;j<25;j++)
			{
				EepWrBuf[j]=0x00;
				PendBuf[ti][j]=0x00;
			}
			if(id==1)
				EepWr((250+PendAdd[ti]),25);	//EepWr(tPendAddr[ti],20);
			else
				EepWr(PendAdd[ti],25);	//EepWr(tPendAddr[ti],20);
		}
	}
	//新增加在主界面上显示数量
	if(frmNum==FRM_MAIN)
	{
		if(tLstNum<99)
		{
			tDisBuf[0]='0'+(tLstNum/10);
			tDisBuf[1]='0'+(tLstNum%10);
			tDisBuf[2]=0;	
			if(tDisBuf[0]=='0')
				 tDisBuf[0]=' ';
		//	lcd_text16(740-8,75,0,WHITE,tDisBuf);
		}
	}
	return tLstNum;
}

 //查找待发记录
unsigned char Fnd_Pend(unsigned char id,char *OrdId)
{
	char tFndBuf[9];
	char tChkBuf[9];
	char tBuf[30];
	unsigned char ti,tj,i;
	unsigned char dIndex=100;
//	unsigned char tNewCnt;
//	unsigned char i;
	for(ti=0;ti<8;ti++)
		tFndBuf[ti]=OrdId[ti];
	tFndBuf[8]=0;
	for(ti=0;ti<10;ti++)
	{
	  if(id==1)
			EepRd((250+PendAdd[ti]),25);
		else
			EepRd(PendAdd[ti],25);

		for(i=0;i<25;i++)
			tBuf[i]=EepRdBuf[i];
//		EEP_Read(tBuf,PendAdd[ti],20);
		for(tj=0;tj<8;tj+=2)
		{	
			tChkBuf[tj+0]='0'+(tBuf[(tj/2)+1]>>4);
			tChkBuf[tj+1]='0'+(tBuf[(tj/2)+1]&0x0f);
		}
		tChkBuf[8]=0;
	 if(strcmp(tFndBuf,tChkBuf)==0)
	 {
	 		return 0x00;
	 }		
	}
	if(dIndex==100)
	{
		return 0xAA;
	}
	else
	{
	 	return 0x00;
	}
}
char *GetPendYFL(unsigned char id,char *tOrdId)
{
	//char tFndBuf[9];
	//char tSavBuf[9];
	//char tYflBuf[9];
	//char tBuf[25];
	////char tCarId[7];
	char *rtnBuf;
	//unsigned char ti,tj,i;
	//unsigned char dIndex=100;
	//for(ti=0;ti<8;ti++)
	//	tFndBuf[ti]=tOrdId[ti];
	//tFndBuf[8]=0;
	//for(ti=0;ti<10;ti++)
	//{
	//	if(id==0)
	//		EepRd(PendAdd[ti],25);
	//	else
	//		EepRd((250+PendAdd[ti]),25);
	//	for(i=0;i<25;i++)
	//		tBuf[i]=EepRdBuf[i];
	////		EEP_Read(tBuf,PendAdd[ti],20);
	//	for(tj=0;tj<8;tj+=2)
	//	{	
	//		tSavBuf[tj+0]='0'+(tBuf[(tj/2)+1]>>4);
	//		tSavBuf[tj+1]='0'+(tBuf[(tj/2)+1]&0x0f);
	//	}
	//	tSavBuf[8]=0;
	// if(strcmp(tFndBuf,tSavBuf)==0)
	// {
	// 		dIndex=ti;
	//		for(tj=0;tj<8;tj+=2)
	//		{	
	//			tYflBuf[tj+0]='0'+(tBuf[(tj/2)+6]>>4);
	//			tYflBuf[tj+1]='0'+(tBuf[(tj/2)+6]&0x0f);
	//		}
	//		tYflBuf[7]=0;	//小数点舍去
	//		tYflBuf[8]=0;
	//		for(tj=7;tj>0;tj--)
	//		{
	//			tYflBuf[tj]=tYflBuf[tj-1];
	//		}
	//		tYflBuf[0]='0';
	//		for(tj=0;tj<7;tj++)
	//		{
	//			if(tYflBuf[tj]=='0')
	//				tYflBuf[tj]=' ';
	//			else
	//			  break;
	//		}
	//		flgPendLst=1;	//查到记录
	//
	//		break;
	// }		
	//}
	//if(dIndex==100)
	//{
	////		lcd_text16(290,427,0,0,"Not this Pend List!");
	//	for(tj=0;tj<7;tj++)
	//		tYflBuf[tj]=' ';
	//	tYflBuf[7]='0';
	//	tYflBuf[8]=0;
	//	flgPendLst=0;
	//}
	//for(ti=0;ti<9;ti++)
	//{
	//	tmpYFL[ti]=tYflBuf[ti];
	//	YflBuf[ti]=tYflBuf[ti];
	//}
	//if(flgPendLst==1)//查到记录
	//{
	//	for(ti=0;ti<8;ti++)
	//		CarId[ti]=tBuf[10+ti];
	//	if((CarId[0]<'0')||(CarId[0]>'9'))
	//		CarId[0]=' ';
	//	CarId[8]=0;
	//	Dis_Pad(1);//Dis_Pad_Mini(1);
	//}
	//else
	//{
	//	for(ti=0;ti<8;ti++)
	//		CarId[ti]=0xAA;	 //车号无效
	//	CarId[8]=0;
	//	Dis_Pad(0);//Dis_Pad_Mini(0);
	//}
	////	dualChNum=tBuf[18];
	////	if(dualChNum==1)  Draw_EN24(182-160,54,WHITE,BLUE,"13");//GREEN
	////	if(dualChNum==2)	Draw_EN24(182-160,54,BRRED,BLUE,"14");
	//rtnBuf=tYflBuf;
	return rtnBuf;
	//return GetPendYFL;
}
void SavOilId_Goods(void)
{
	unsigned char i;
	for(i=0;i<10;i++)
		EepWrBuf[i]=SavOilId[i];
	EepWr(211,10);
	SavOilId[14]=0;
	sys_GoodsId=SavOilId[9];
	Dis_GdsId(sys_GoodsId);
}

//tAddWarnId:添加报警记录的报警号
unsigned char AddWarnLst(unsigned char id,unsigned char tAddWarnId)
{
	//	unsigned char tcnt;
	unsigned char ti,tj;
	//	//所有记录后移，新记录存入(1*4096+(10~17))
	if(id==0)
		EepRd(0x6000,120);
	if(id==1)
		EepRd(0x6000+120,120);
	for(ti=0;ti<160;ti++)
		FlashWarnBuf[ti]=EepRdBuf[ti];
	for(ti=12;ti>0;ti--)
	{
		for(tj=0;tj<10;tj++)
	 	{FlashWarnBuf[(ti*10)+tj]=FlashWarnBuf[((ti-1)*10)+tj];}
	}
	FlashWarnBuf[0]++;
	if(FlashWarnBuf[0]>12)
		FlashWarnBuf[0]=12;
	FlashWarnBuf[1]=sysY;
	FlashWarnBuf[2]=sysM;
	FlashWarnBuf[3]=sysD;
	FlashWarnBuf[4]=sysHH;
	FlashWarnBuf[5]=sysMM;
	FlashWarnBuf[6]=sysSS;
	FlashWarnBuf[7]=tAddWarnId;
	for(ti=0;ti<160;ti++)
		EepWrBuf[ti]=FlashWarnBuf[ti];
	if(id==0)
		EepWr(0x6000,120);
	if(id==1)
		EepWr(0x6000+120,120);
	return 0;
}

/*
已发记录保存在832(0x340)以后,
	//step 1:以流水号为依据判断记录是否合格？
	//step 1:寻找空扇区
	//step 2:寻找Index
	//step 3:校验数据
	返回值是四位，高8位为位置号（0~FF），低8位为记录号（0~200）

*/
unsigned int Fnd_Lst_Index(unsigned char id)
{
	unsigned long i,j;
	char tBuf[80];
	unsigned char tAvd_crc;
	unsigned int  tLstIndex1;//记录号
	unsigned int  tLstLos1;		 //记录对应的位置
  unsigned int  tRtn;//返回值
	unsigned char tLstCnt;
	unsigned char flgGet;
	//unsigned int tLastIndex,tLastLos;//最后保存的记录号与位置号
	//搜寻所有存储区
	//__disable_irq();
	for(i=0;i<200;i++)
	{
		EepRd(0x340+i*80,80);
		for(j=0;j<3;j++)
			tBuf[j]=EepRdBuf[j];
		//EepRd(0x340+79+i*80,2);
		tBuf[79]=EepRdBuf[79];
		tAvd_crc=0;
		//乌石化记录最后一位为0x4F
		if((((tBuf[0]&0xF0)==0xA0)&&((tBuf[1]&0xF0)==0x50)&&(tBuf[2]==0x3A))&&(tBuf[79]==0x5F))
		//扬子的记录最后一位为0x5F
		//if(((tBuf[0]&0xF0)==0xA0)&&((tBuf[1]&0xF0)==0x50)&&(tBuf[2]==0x3A)&&(tBuf[63]==0x5F))
		{
		 tLstCnt=((tBuf[0]&0x0F)<<4)+(tBuf[1]&0x0F);
		 if((tLstCnt>0)&&(tLstCnt<210))//记录号1~210,超过为错误记录号
		 {
		 	 tAvd_crc=1;
		 }
		}
		if(tAvd_crc==1)//有效记录
		{
			flgLst[i]=tLstCnt;
		}
		else
		{
			flgLst[i]=0;
		}																									                 
	}
	//查找最后记录点
	tLstIndex1=0;
	tLstLos1=0;
//	tLstIndex2=0;
//	tLstLos2=0;
	flgGet=0;
	for(i=0;i<200;i++)
	{
		if(flgLst[i]==0)
		{
			flgGet=2;//没有记录
			break;
		}
		if((flgLst[i]>flgLst[i+1])&&(flgLst[i+1]!=0)&&(flgLst[i]!=0))
		{
			if((flgLst[i]-flgLst[i+1])<208)
			{
				flgGet=1;//最后一条记录
				break;
			}	
		}
		if((flgLst[i]<flgLst[i+1])&&(flgLst[i+1]!=0)&&(flgLst[i]!=0))
		{
				if(flgLst[i+1]-flgLst[i]>1)
				{
					flgGet=1;//最后一条记录
					break;
				}
		}
	}
	//flgGet==0;//记录正好写满200条
	//flgGet==1;//记录未写满200条，找到最后一条记录位置
	//flgGet==2;//新的记录
	if(flgGet==0)//记录正好写满200条
	{
		tLstIndex1=flgLst[199];
		tLstLos1=199;
	}
	else if(flgGet==1)
	{
		tLstIndex1=flgLst[i];
		tLstLos1=i;
	}
	else if(flgGet==2)
	{
	  if(i>0)
		{
			tLstIndex1=flgLst[i-1];
			tLstLos1=i-1;
		}
		else
		{
			tLstIndex1=0;
			tLstLos1=199;
		}
	}
//	返回值是四位，高8位为位置号(0~199),低8位为记录号(0~219),低8位在存储时要作加1处理.
	tRtn=(tLstLos1<<8)+(tLstIndex1);
//	__enable_irq();
	return tRtn;
}

//保存新的提单
//首地址0x3600
//0xAA,0x55,提油密码(4byte),应发量(4byte)
//0..3:提油密码,4:单位,5..8:应发量,9..12:油密:13..14:油温:15:采温方式
extern unsigned char flgSysWarn;
extern char lstOilId[2][4];//提油密码
extern float YFL[2];
extern char idCarId[10];
//extern float rdLJS,dSSLv_pls_first,rtSSLv_pls,dSSLv_pls;
void GetPreWorkDat(void)
{
	unsigned char i;
	unsigned char chNum=0;//上次关闭时的通道号
	unsigned int  tx2;//	char twDat[60];
	EepRd(0x3600,2);
	if((EepRdBuf[0]==0xAA)&&(EepRdBuf[1]==0x55))
	{
		//说明上一次有提单没有执行结束;
		EepRd(0x3600,40);
		if(EepRdBuf[6]==0x55) chNum=1;
		if(EepRdBuf[6]==0xAA) chNum=2;
		LogUsrID[0]='0'+(EepRdBuf[33]>>4);
		LogUsrID[1]='0'+(EepRdBuf[33]&0x0F);
		LogUsrID[2]='0'+(EepRdBuf[34]>>4);
		LogUsrID[3]='0'+(EepRdBuf[34]&0x0F);
		lstOilId[chNum-1][0]=EepRdBuf[2];
		lstOilId[chNum-1][1]=EepRdBuf[3];
		lstOilId[chNum-1][2]=EepRdBuf[4];
		lstOilId[chNum-1][3]=EepRdBuf[5];
		nowPendLst[chNum-1][0]='0'+(lstOilId[chNum-1][0]>>4);
		nowPendLst[chNum-1][1]='0'+(lstOilId[chNum-1][0]&0x0F);
		nowPendLst[chNum-1][2]='0'+(lstOilId[chNum-1][1]>>4);
		nowPendLst[chNum-1][3]='0'+(lstOilId[chNum-1][1]&0x0F);
		nowPendLst[chNum-1][4]='0'+(lstOilId[chNum-1][2]>>4);
		nowPendLst[chNum-1][5]='0'+(lstOilId[chNum-1][2]&0x0F);
		nowPendLst[chNum-1][6]='0'+(lstOilId[chNum-1][3]>>4);
		nowPendLst[chNum-1][7]='0'+(lstOilId[chNum-1][3]&0x0F);
		nowPendLst[chNum-1][8]=0;
		for(i=0;i<9;i++)
			frmMainOrdId[i]=nowPendLst[chNum-1][i];
	 	lstYFL[chNum-1][0]=EepRdBuf[7];
	 	lstYFL[chNum-1][1]=EepRdBuf[8];
	 	lstYFL[chNum-1][2]=EepRdBuf[9];
	 	lstYFL[chNum-1][3]=EepRdBuf[10];

		dualChNum=chNum;
		if(dualChNum==1){tx2=223;}
		if(dualChNum==2){tx2=623;}
		frmMainYFval[0]='0'+(lstYFL[chNum-1][0]>>4);
		frmMainYFval[1]='0'+(lstYFL[chNum-1][0]&0x0F);
		frmMainYFval[2]='0'+(lstYFL[chNum-1][1]>>4);
		frmMainYFval[3]='0'+(lstYFL[chNum-1][1]&0x0F);
		frmMainYFval[4]='0'+(lstYFL[chNum-1][2]>>4);
		frmMainYFval[5]='0'+(lstYFL[chNum-1][2]&0x0F);
		frmMainYFval[6]='0'+(lstYFL[chNum-1][3]>>4);
		frmMainYFval[7]='0'+(lstYFL[chNum-1][3]&0x0F);
		frmMainYFval[8]=0;

		dYFLv=calYFL();dYFL[dualChNum-1]=dYFLv;
		YFL[dualChNum-1]=dYFLv;
		lng_KgNeed=dYFLv;
		for(i=0;i<7;i++)
		{
			if(frmMainYFval[i]=='0')
				frmMainYFval[i]=' ';
			else
				i=0xF0;//退出循环
		}
		//车牌
		for(i=0;i<8;i++)
		{
		  idCarId[i]=EepRdBuf[11+i];
		  chrCarId[dualChNum-1][i]=EepRdBuf[11+i];
		}
		//开始时间
		for(i=0;i<6;i++)
		{
		 FyDateSt[i]=EepRdBuf[27+i];
		}
		//要加质量表前量
		dLJSv_first=ctof(EepRdBuf[22],EepRdBuf[21],EepRdBuf[20],EepRdBuf[19]);
		dLJSm_first=ctof(EepRdBuf[26],EepRdBuf[25],EepRdBuf[24],EepRdBuf[23]);
		dLJSv= dLJSv_first;//此句没有意义
		frmNum=FRM_MAIN;//frmNum=FRM_MON;
		frmNumLast=FRM_MON;
		if(sys_Conn==1)	sys_ConnLast=1;		//没有这句，断电时会跳到选择界面
		flgDisDate=1;
		flgDisTime=1;
		Dis_Scr(FRM_MAIN);
		Dis_Pad(1);//Dis_Pad_Mini(1);
		//ShowStr30(20,82,frmMainOrdId,8,BLACK,WHITE);
		ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE); //小数点以后舍去
		sys_STA=7;flgUsrSp=1;flgAlert[6]=1;flgSysWarn=2;flgWrWarnLst=1;
		lcd_text24(tx2,442,BLUE,GOLD,"用户暂停");
		//if(WarnOnOff[9]==1)
		{
				ReadLJS();
				//rdLJS=ctof(EepRdBuf[39],EepRdBuf[38],EepRdBuf[37],EepRdBuf[36]);
				dSSLv_pls_first=0;
				dSSLv_pls=rdLJS;
				rtSSLv_pls=dSSLv_pls-dSSLv_pls_first;
		}
	}
}

//保存新的提单
//首地址0x3600
//0xAA,0x55,提油密码(4byte),应发量(4byte)
//0..3:提油密码,4:单位,5..8:应发量,9..12:油密:13..14:油温:15:采温方式
void SavNewWorkDat(void)
{
	unsigned char i;
	char twDat[40];
	twDat[0]=0xAA;
	twDat[1]=0x55;
	twDat[2]=nowOrdId[0];//发油密码
	twDat[3]=nowOrdId[1]; 
	twDat[4]=nowOrdId[2]; 
	twDat[5]=nowOrdId[3];
	if(dualChNum==1)  twDat[6]=0x55;
	if(dualChNum==2)  twDat[6]=0xAA;
	twDat[7]=lstYFL[dualChNum-1][0];//应发量
	twDat[8]=lstYFL[dualChNum-1][1];
	twDat[9]=lstYFL[dualChNum-1][2];
	twDat[10]=lstYFL[dualChNum-1][3];
	twDat[11]=idCarId[0];//chrCarId[dualChNum-1][0];//CarId[0];//车号
	twDat[12]=idCarId[1];//chrCarId[dualChNum-1][1];//CarId[1];
	twDat[13]=idCarId[2];//chrCarId[dualChNum-1][2];//CarId[2];
	twDat[14]=idCarId[3];//chrCarId[dualChNum-1][3];//CarId[3];
	twDat[15]=idCarId[4];//chrCarId[dualChNum-1][4];//CarId[4];
	twDat[16]=idCarId[5];//chrCarId[dualChNum-1][5];//CarId[5];
	twDat[17]=idCarId[6];//chrCarId[dualChNum-1][6];//CarId[6];
	twDat[18]=idCarId[7];//chrCarId[dualChNum-1][7];//CarId[7];
  ftoc(dLJSv_first);
		twDat[19]=c2f[0];//前表数
		twDat[20]=c2f[1];
		twDat[21]=c2f[2];
		twDat[22]=c2f[3];
  ftoc(dLJSm_first);
	 	twDat[23]=c2f[0];//前表数2
		twDat[24]=c2f[1];
		twDat[25]=c2f[2];
		twDat[26]=c2f[3];

	twDat[27]=FyDateSt[0];//开始时间
	twDat[28]=FyDateSt[1];
	twDat[29]=FyDateSt[2];
	twDat[30]=FyDateSt[3];
	twDat[31]=FyDateSt[4];
	twDat[32]=FyDateSt[5];
	twDat[33]=((LogUsrID[0]&0x0F)<<4)+(LogUsrID[1]&0x0F);//员工号 BCD	
	twDat[34]=((LogUsrID[2]&0x0F)<<4)+(LogUsrID[3]&0x0F);	
  ftoc(rtSSLv_pls);
		twDat[35]=c2f[0];
		twDat[36]=c2f[1];
		twDat[37]=c2f[2];
		twDat[38]=c2f[3];
	twDat[39]=0x5A;
	//清除预存记录			 
	for(i=0;i<40;i++)
		EepWrBuf[i]=twDat[i];
	EepWr(0x3600,40);
}
extern unsigned char flgMbWr;
extern unsigned int mbdata[200];
extern unsigned int mbBuf[200];
 float ReadMD(void)
{
		return 1.0;
}
float ReadWD(void)
{
	unsigned int thWD;
	float flWD;
	EepRd(50,8);
	if((EepRdBuf[0]==0x55)&&(EepRdBuf[1]==0xAA)&&(EepRdBuf[6]==0x55)&&(EepRdBuf[7]==0xAA))
	{
		thWD = EepRdBuf[2]&0x7F;
		thWD = thWD<<8;
		thWD+= EepRdBuf[3];
		flWD=(float)thWD/100;
		if((EepRdBuf[2]&0x80)==0x80)
		{
		 flWD=(-1.0)*flWD;  
		}
//		ftoc(flWD);
//		flgMbWr=1;
//		mbdata[18]=(c2f[1]<<8)+c2f[0];
//		mbdata[19]=(c2f[3]<<8)+c2f[2];
//		flgMbWr=0;
//		mbBuf[18]=mbdata[18];mbBuf[19]=mbdata[19];
		return flWD;
	}
	else
		return 0.0;

}
void SavMD(void)
{
	float tMD;
	unsigned int intMD;
	tMD=dMD;
	intMD=(unsigned int)(tMD*10000);
	EepWrBuf[0]=0x55;
	EepWrBuf[1]=0xAA;
	EepWrBuf[2]=0x00;
	EepWrBuf[3]=0x00;
	EepWrBuf[4]=intMD>>8;
	EepWrBuf[5]=intMD&0xFF;
	EepWrBuf[6]=0x55;
	EepWrBuf[7]=0xAA;
	EepWr(50,8);

}
//密度格式：1.2345
//温度格式：123.45
void SavMdWd(void)
{
	float tMD,tWD;
	unsigned int intMD,intWD;
	unsigned char flgWDsig;
	unsigned char tWDhi,tWDlo;
	if(dWD<0.0)
	{	
	  flgWDsig=1;
		tWD=(-1.0)*dWD;
	}
	else
	{
		flgWDsig=0;
		tWD=dWD;
	}
	if(dMD>6.535)
	  dMD=6.535;
	tMD=dMD;
	intMD=(unsigned int)(tMD*10000);
	intWD=(unsigned int)(tWD*100);
	if(intWD>30000)
	{
	 intWD=30000;
	}
	tWDhi=intWD>>8;
	tWDlo=intWD&0xFF;
	if(flgWDsig)
	  tWDhi=0x80|tWDhi;
	EepWrBuf[0]=0x55;
	EepWrBuf[1]=0xAA;
	EepWrBuf[2]=tWDhi;
	EepWrBuf[3]=tWDlo;
	EepWrBuf[4]=intMD>>8;
	EepWrBuf[5]=intMD&0xFF;
	EepWrBuf[6]=0x55;
	EepWrBuf[7]=0xAA;
	EepWr(50,8);

}
/*
累积数[脉冲]保存在[38~开头的区域,		[38]=0x55,[39]=0xAA;
FF FF FF FF FF FF
*/
extern float rdLJS;
void ReadLJS(void)
{
	unsigned long thLJS;
	float rtnLJS;
	EepRd(38,10);
	if((EepRdBuf[0]==0x55)&&(EepRdBuf[1]==0xAA)&&(EepRdBuf[8]==0x55)&&(EepRdBuf[9]==0xAA))
	{
		thLJS = EepRdBuf[2];
		thLJS = thLJS<<8;
		thLJS+= EepRdBuf[3];
		thLJS = thLJS<<8;
		thLJS+= EepRdBuf[4];
		thLJS = thLJS<<8;
		thLJS+= EepRdBuf[5];
		thLJS = thLJS<<8;
		thLJS+= EepRdBuf[6];
		thLJS = thLJS<<8;
		thLJS+= EepRdBuf[7];
		rtnLJS=(float)thLJS/10;
		rdLJS=rtnLJS;
	}
	else
		rdLJS=0.0;
}

extern float savLJS;
/*
FF FF FF FF FF FF
*/
void SavLJS(float tLJS)
{
	unsigned char t[6];
	unsigned long thLJS;
	thLJS=(unsigned long)(savLJS*10);
	t[0]=0x0;//(unsigned char)((thLJS>>40)&0xFF);
	//thLJS=thLJS&0x;
	t[1]=0x0;//(unsigned char)((thLJS>>32)&0xFF);
	//thLJS=thLJS>>8;
	t[2]=(unsigned char)((thLJS>>24)&0xFF);
	//thLJS=thLJS>>8;
	t[3]=(unsigned char)((thLJS>>16)&0xFF);
	//thLJS=thLJS>>8;
	t[4]=(unsigned char)((thLJS>>8)&0xFF);
	//thLJS=thLJS>>8;			
	t[5]=(unsigned char)((thLJS)&0xFF);
	EepWrBuf[0]=0x55;
	EepWrBuf[1]=0xAA;
	EepWrBuf[2]=t[0];
	EepWrBuf[3]=t[1];
	EepWrBuf[4]=t[2];
	EepWrBuf[5]=t[3];
	EepWrBuf[6]=t[4];
	EepWrBuf[7]=t[5];
	EepWrBuf[8]=0x55;
	EepWrBuf[9]=0xAA;
	EepWr(38,10);
}

extern unsigned int sysWorkAllTmr;
void Fill_New_Lst(unsigned char id,unsigned char tAdd)
{
	unsigned int i;
	unsigned int ttWD;
	unsigned long tLngDat;
	unsigned int  tIntDat;
	char tChrDat[10];
	char tBuf[80];
	//float flDat;
  float tsfl;//,flM1,flM2;
  tAdd++;
  if(tAdd>=210)
    tAdd=1;
	tBuf[0]=0xA0+(tAdd>>4);
	tBuf[1]=0x50+(tAdd&0x0F);
	tBuf[2]=0x3A;
	if(sys_Conn==1)
	{
		tBuf[3]=lstOilId[id][0];
		tBuf[4]=lstOilId[id][1];
		tBuf[5]=lstOilId[id][2];
		tBuf[6]=lstOilId[id][3];
	}
	else
	{
		tBuf[3]=0;tBuf[4]=0;tBuf[5]=0;tBuf[6]=0;
	}
	tLngDat=dYFLv;
	tChrDat[0]=(unsigned char)(tLngDat/10000000);
	tLngDat=tLngDat%10000000;
	tChrDat[1]=(unsigned char)(tLngDat/1000000);
	tLngDat=tLngDat%1000000;
	tChrDat[2]=(unsigned char)(tLngDat/100000);
	tLngDat=tLngDat%100000;
	tChrDat[3]=(unsigned char)(tLngDat/10000);
	tLngDat=tLngDat%10000;
	tChrDat[4]=(unsigned char)(tLngDat/1000);
	tLngDat=tLngDat%1000;
	tChrDat[5]=(unsigned char)(tLngDat/100);
	tLngDat=tLngDat%100;
	tChrDat[6]=(unsigned char)(tLngDat/10);
	tLngDat=tLngDat%10;
	tChrDat[7]=(unsigned char)(tLngDat);
 	//Lst[5~8]:应发量
	tBuf[7]=(tChrDat[0]<<4)+tChrDat[1];//应发量	BCD
	tBuf[8]=(tChrDat[2]<<4)+tChrDat[3];	
	tBuf[9]=(tChrDat[4]<<4)+tChrDat[5];	
	tBuf[10]=(tChrDat[6]<<4)+tChrDat[7];	
	//实发质量
	tsfl=dLJSm_last-dLJSm_first;
	tsfl*=10;
	tLngDat=(unsigned long)tsfl;
	tChrDat[0]=tLngDat/10000000;
	   tLngDat=tLngDat%10000000;
	tChrDat[1]=tLngDat/1000000;
	   tLngDat=tLngDat%1000000;
	tChrDat[2]=tLngDat/100000;
	tLngDat=tLngDat%100000;
	tChrDat[3]=tLngDat/10000;
	tLngDat=tLngDat%10000;
	tChrDat[4]=tLngDat/1000;
	tLngDat=tLngDat%1000;
	tChrDat[5]=tLngDat/100;
	tLngDat=tLngDat%100;
	tChrDat[6]=tLngDat/10;
	tLngDat=tLngDat%10;
	tChrDat[7]=tLngDat;
	//Lst[9~12]:实发量
	tBuf[11]=(tChrDat[0]<<4)+tChrDat[1];//实发量	BCD
	tBuf[12]=(tChrDat[2]<<4)+tChrDat[3];	
	tBuf[13]=(tChrDat[4]<<4)+tChrDat[5];	
	tBuf[14]=(tChrDat[6]<<4)+tChrDat[7];	
	//员工号 BCD
	//Lst[13~14]:员工号
	tBuf[15]=((LogUsrID[0]&0x0F)<<4)+(LogUsrID[1]&0x0F);//员工号 BCD	
	tBuf[16]=((LogUsrID[2]&0x0F)<<4)+(LogUsrID[2]&0x0F);	
	//表前数 rtLJS_first*10;
	//表前数[质量]
  ftoc(dLJSm_first);
	tBuf[17]=c2f[0];
	tBuf[18]=c2f[1];
	tBuf[19]=c2f[2];
	tBuf[20]=c2f[3];
	//表后数[质量]
  ftoc(dLJSm_last);
	tBuf[21]=c2f[0];
	tBuf[22]=c2f[1];
	tBuf[23]=c2f[2];
	tBuf[24]=c2f[3];

	//Lst[25~30]开始时间
	tBuf[25]=((FyDateSt[0]/10)<<4)+(FyDateSt[0]%10);//FyDateSt[0];	
	tBuf[26]=((FyDateSt[1]/10)<<4)+(FyDateSt[1]%10);//FyDateSt[1];	
	tBuf[27]=((FyDateSt[2]/10)<<4)+(FyDateSt[2]%10);//FyDateSt[2];	
	tBuf[28]=((FyDateSt[3]/10)<<4)+(FyDateSt[3]%10);//FyDateSt[3];	
	tBuf[29]=((FyDateSt[4]/10)<<4)+(FyDateSt[4]%10);//FyDateSt[4];	
	tBuf[30]=((FyDateSt[5]/10)<<4)+(FyDateSt[5]%10);//FyDateSt[5];	
	//Lst[31~36]结束时间
	tBuf[31]=((FyDateSp[0]/10)<<4)+(FyDateSp[0]%10);//FyDateSp[0];	
	tBuf[32]=((FyDateSp[1]/10)<<4)+(FyDateSp[1]%10);//FyDateSp[1];	
	tBuf[33]=((FyDateSp[2]/10)<<4)+(FyDateSp[2]%10);//FyDateSp[2];	
	tBuf[34]=((FyDateSp[3]/10)<<4)+(FyDateSp[3]%10);//FyDateSp[3];	
	tBuf[35]=((FyDateSp[4]/10)<<4)+(FyDateSp[4]%10);//FyDateSp[4];	
	tBuf[36]=((FyDateSp[5]/10)<<4)+(FyDateSp[5]%10);//FyDateSp[5];	

	//Lst[37..40]//实发体积,本次不用
//	tBuf[39]=0x55;
//	tBuf[40]=0x55;
	//Lst[41..44]介质密度	 0000.0000
	tLngDat=dMD*10000;
	tChrDat[0]=(unsigned char)(tLngDat/10000000);
	tLngDat=tLngDat%10000000;
	tChrDat[1]=(unsigned char)(tLngDat/1000000);
	tLngDat=tLngDat%1000000;
	tChrDat[2]=(unsigned char)(tLngDat/100000);
	tLngDat=tLngDat%100000;
	tChrDat[3]=(unsigned char)(tLngDat/10000);
	tLngDat=tLngDat%10000;
	tChrDat[4]=(unsigned char)(tLngDat/1000);
	tLngDat=tLngDat%1000;
	tChrDat[5]=(unsigned char)(tLngDat/100);
	tLngDat=tLngDat%100;
	tChrDat[6]=(unsigned char)(tLngDat/10);
	tLngDat=tLngDat%10;
	tChrDat[7]=(unsigned char)(tLngDat);
	tBuf[37]=(tChrDat[0]<<4)+tChrDat[1];
	tBuf[38]=(tChrDat[2]<<4)+tChrDat[3];
	tBuf[39]=(tChrDat[4]<<4)+tChrDat[5];	
	tBuf[40]=(tChrDat[6]<<4)+tChrDat[7];
	//Lst[45..48]介质温度			+000000.0
	if(dWD<0.0)
	{	tChrDat[0]=1;ttWD=(-10.0)*dWD;}
	else
	{	tChrDat[0]=0;ttWD=(10.0)*dWD;}
	tLngDat=ttWD;
	tChrDat[1]=(unsigned char)(tLngDat/100);
	tLngDat=tLngDat%100;
	tChrDat[2]=(unsigned char)(tLngDat/10);
	tLngDat=tLngDat%10;
	tChrDat[3]=(unsigned char)(tLngDat);
	tBuf[41]=(tChrDat[0]<<4)+tChrDat[1];
	tBuf[42]=(tChrDat[2]<<4)+tChrDat[3];

	//Lst[49] 货位编号
	tBuf[45]=sys_GoodsId;
	//Lst[50] 发油方式
//	tBuf[50]=0x55;
	//Lst[51..59] 物料编码
	tBuf[46]=OilCode[0];
	tBuf[47]=OilCode[1];
	tBuf[48]=OilCode[2];
	tBuf[49]=OilCode[3];
	tBuf[50]=OilCode[4];
	tBuf[51]=OilCode[5];
	tBuf[52]=OilCode[6];
	tBuf[53]=OilCode[7];
	tBuf[54]=OilCode[8];
  ftoc(dLJSv_first);
	tBuf[55]=c2f[0];
	tBuf[56]=c2f[1];
	tBuf[57]=c2f[2];
	tBuf[58]=c2f[3];
  ftoc(dLJSv_last);
	tBuf[59]=c2f[0];
	tBuf[60]=c2f[1];
	tBuf[61]=c2f[2];
	tBuf[62]=c2f[3];
	tBuf[63]=dualChNum;
	for(i=0;i<8;i++)
		tBuf[64+i]=idCarId[i];	//chrCarId[dualChNum-1][i];//
	for(i=0;i<4;i++)
		tBuf[72+i]=SavLeadId[i];//((LeadId[2*i]&0x0F)<<4)+(LeadId[2*i+1]&0x0F);;
	tBuf[76]=0x5F;
	tBuf[77]=0x5F;
	tBuf[78]=0x5F;
	tBuf[79]=0x5F;
	for(i=0;i<80;i++)
		LstFyBuf[i]=tBuf[i];

//以下为CAN自动上传记录
		if(dualChNum==1)//1#发油记录
		{
			for(i=0;i<4;i++)
				canLstData[i]=canJobData[7+i];//提货单号
			canLstData[4]=canJobData[11];		//应发量，3位扩展到标准4位
			canLstData[5]=canJobData[12];
			canLstData[6]=0;
			canLstData[7]=canJobData[13];
			for(i=0;i<7;i++)
				canLstData[8+i]=canJobData[i];//车号
			canLstData[55]=1;
		}
		if(dualChNum==2)
		{
			for(i=0;i<4;i++)
				canLstData[i]=canJobData[21+i];
			canLstData[4]=canJobData[25];
			canLstData[5]=canJobData[26];
			canLstData[6]=0;
			canLstData[7]=canJobData[27];
			for(i=0;i<7;i++)
				canLstData[8+i]=canJobData[14+i];//车号
			canLstData[55]=2;
			
		}
	    tLngDat=(unsigned long)(dSFLv*10.0);
			tIntDat=tLngDat>>16;
			//canRtData[22]=(unsigned char)(tIntDat>>8);
			canLstData[17]=(unsigned char)(tIntDat&0xFF);
			tIntDat=tLngDat&0xFFFF;
			canLstData[15]=(unsigned char)(tIntDat>>8);
			canLstData[16]=(unsigned char)(tIntDat&0xFF);
	    tLngDat=(unsigned long)(dSFLm*10.0);
			tIntDat=tLngDat>>16;
			//canRtData[22]=(unsigned char)(tIntDat>>8);
			canLstData[20]=(unsigned char)(tIntDat&0xFF);
			tIntDat=tLngDat&0xFFFF;
			canLstData[18]=(unsigned char)(tIntDat>>8);
			canLstData[19]=(unsigned char)(tIntDat&0xFF);
			ftoc(dLJSv_first);
			canLstData[21]=c2f[1];canLstData[22]=c2f[0];canLstData[23]=c2f[3];canLstData[24]=c2f[2];
			ftoc(dLJSv_last);
			canLstData[25]=c2f[1];canLstData[26]=c2f[0];canLstData[27]=c2f[3];canLstData[28]=c2f[2];
			ftoc(dLJSm_first);
			canLstData[29]=c2f[1];canLstData[30]=c2f[0];canLstData[31]=c2f[3];canLstData[32]=c2f[2];
			ftoc(dLJSm_last);
			canLstData[33]=c2f[1];canLstData[34]=c2f[0];canLstData[35]=c2f[3];canLstData[36]=c2f[2];
			if(dWD<0.0)//
			{
				tIntDat=(unsigned int)(dWD*(-100.0));
				tIntDat=tIntDat&0x7FFF;
				tIntDat=tIntDat|0x8000;
			}
			else
			{
				tIntDat=(unsigned int)(dWD*(100.0));
				tIntDat=tIntDat&0x7FFF;
				
			}
			canLstData[37]=tIntDat>>8;canLstData[38]=tIntDat&0xFF;
			tIntDat=(unsigned int)(dMD*10000.0);
			canLstData[39]=tIntDat>>8;canLstData[40]=tIntDat&0xFF;
			//Lst[41~46]开始时间
			canLstData[41]=FyDateSt[0];//((FyDateSt[0]/10)<<4)+(FyDateSt[0]%10);//FyDateSt[0];	
			canLstData[42]=FyDateSt[1];//((FyDateSt[1]/10)<<4)+(FyDateSt[1]%10);//FyDateSt[1];	
			canLstData[43]=FyDateSt[2];//((FyDateSt[2]/10)<<4)+(FyDateSt[2]%10);//FyDateSt[2];	
			canLstData[44]=FyDateSt[3];//((FyDateSt[3]/10)<<4)+(FyDateSt[3]%10);//FyDateSt[3];	
			canLstData[45]=FyDateSt[4];//((FyDateSt[4]/10)<<4)+(FyDateSt[4]%10);//FyDateSt[4];	
			canLstData[46]=FyDateSt[5];//((FyDateSt[5]/10)<<4)+(FyDateSt[5]%10);//FyDateSt[5];	
			//Lst[47~52]结束时间
			canLstData[47]=FyDateSp[0];//((FyDateSp[0]/10)<<4)+(FyDateSp[0]%10);//FyDateSp[0];	
			canLstData[48]=FyDateSp[1];//((FyDateSp[1]/10)<<4)+(FyDateSp[1]%10);//FyDateSp[1];	
			canLstData[49]=FyDateSp[2];//((FyDateSp[2]/10)<<4)+(FyDateSp[2]%10);//FyDateSp[2];	
			canLstData[50]=FyDateSp[3];//((FyDateSp[3]/10)<<4)+(FyDateSp[3]%10);//FyDateSp[3];	
			canLstData[51]=FyDateSp[4];//((FyDateSp[4]/10)<<4)+(FyDateSp[4]%10);//FyDateSp[4];	
			canLstData[52]=FyDateSp[5];//((FyDateSp[5]/10)<<4)+(FyDateSp[5]%10);//FyDateSp[5];	
			tIntDat =(LogUsrID[0]&0x0F)*1000;
			tIntDat+=(LogUsrID[1]&0x0F)*100;
			tIntDat+=(LogUsrID[2]&0x0F)*10;
			tIntDat+=(LogUsrID[3]&0x0F);
			canLstData[52]=tIntDat>>8;canLstData[53]=tIntDat&0xFF;
			FillLstData();
			//Fy2CanLst[0x00]=0x40;
			//Fy2CanLst[0x01]=LstFyBuf[15];//员工号
			//Fy2CanLst[0x02]=LstFyBuf[16];
			//Fy2CanLst[0x03]=dualChNum;//路号
			//Fy2CanLst[0x04]=CarId[0];//LstFyBuf[47];//物料2
			//Fy2CanLst[0x05]=CarId[1];//LstFyBuf[48];//物料3
			//Fy2CanLst[0x06]=CarId[2];//LstFyBuf[49];//物料4
			//Fy2CanLst[0x07]=CarId[3];//LstFyBuf[50];//物料5
			//Fy2CanLst[0x08]=0x81;
			//Fy2CanLst[0x09]=CarId[4];//LstFyBuf[51];//物料6
			//Fy2CanLst[0x0A]=CarId[5];//LstFyBuf[52];//物料7
			//Fy2CanLst[0x0B]=CarId[6];//LstFyBuf[53];//物料8
			//Fy2CanLst[0x0C]=CarId[7];//LstFyBuf[54];//物料9
			//Fy2CanLst[0x0D]=LstFyBuf[25];//开始时间1_年
			//Fy2CanLst[0x0E]=LstFyBuf[26];//开始时间2_月
			//Fy2CanLst[0x0F]=LstFyBuf[27];//开始时间3_日
			//Fy2CanLst[0x10]=0x82;
			//Fy2CanLst[0x11]=LstFyBuf[7];//应发量1
			//Fy2CanLst[0x12]=LstFyBuf[8];//应发量2
			//Fy2CanLst[0x13]=LstFyBuf[9];//应发量3
			//Fy2CanLst[0x14]=LstFyBuf[10];//应发量4
			//Fy2CanLst[0x15]=LstFyBuf[28];//开始时间5_时
			//Fy2CanLst[0x16]=LstFyBuf[29];//开始时间6_分
			//Fy2CanLst[0x17]=LstFyBuf[30];//开始时间7_秒
			//Fy2CanLst[0x18]=0x83;
			//Fy2CanLst[0x19]=LstFyBuf[11];//实发重量1
			//Fy2CanLst[0x1A]=LstFyBuf[12];//实发重量2
			//Fy2CanLst[0x1B]=LstFyBuf[13];//实发重量3
			//Fy2CanLst[0x1C]=LstFyBuf[14];//实发重量4
			//Fy2CanLst[0x1D]=sysWorkAllTmr>>8;//发油时间--结束时间1
			//Fy2CanLst[0x1E]=sysWorkAllTmr&&0xFF;//发油时间--结束时间2
			//Fy2CanLst[0x1F]=LstFyBuf[72];//铅封号1--结束时间3
			//Fy2CanLst[0x20]=0x84;
			//ftoc(dLJSv_last-dLJSv_first);
			//Fy2CanLst[0x21]=c2f[0];//实发体积1
			//Fy2CanLst[0x22]=c2f[1];//实发体积2
			//Fy2CanLst[0x23]=c2f[2];//实发体积3
			//Fy2CanLst[0x24]=c2f[3];//实发体积4
			//Fy2CanLst[0x25]=LstFyBuf[73];//结束时间4
			//Fy2CanLst[0x26]=LstFyBuf[74];//结束时间5
			//Fy2CanLst[0x27]=LstFyBuf[75];//结束时间6
			//Fy2CanLst[0x28]=0x85;
			//Fy2CanLst[0x29]=LstFyBuf[17];//flMet1[0];//前表数1
			//Fy2CanLst[0x2A]=LstFyBuf[18];//flMet1[1];//前表数2
			//Fy2CanLst[0x2B]=LstFyBuf[19];//flMet1[2];//前表数3
			//Fy2CanLst[0x2C]=LstFyBuf[20];//flMet1[3];//前表数4
			//Fy2CanLst[0x2D]=LstFyBuf[37];//介密1
			//Fy2CanLst[0x2E]=LstFyBuf[38];//介密2
			//Fy2CanLst[0x2F]=0x55;//发油单位:55H质量；0AAH体积
			//Fy2CanLst[0x30]=0x86;
			//Fy2CanLst[0x31]=LstFyBuf[55];//flMet2[0];//后表数1
			//Fy2CanLst[0x32]=LstFyBuf[56];//flMet2[1];//后表数2
			//Fy2CanLst[0x33]=LstFyBuf[57];//flMet2[2];//后表数3
			//Fy2CanLst[0x34]=LstFyBuf[58];//flMet2[3];//后表数4
			//Fy2CanLst[0x35]=LstFyBuf[39];//介密3
			//Fy2CanLst[0x36]=LstFyBuf[40];//介密4
			//Fy2CanLst[0x37]=tAdd;//记录
			//Fy2CanLst[0x38]=0xC0;
			//Fy2CanLst[0x39]=LstFyBuf[3];//密码1
			//Fy2CanLst[0x3A]=LstFyBuf[4];//密码2
			//Fy2CanLst[0x3B]=LstFyBuf[5];//密码3
			//Fy2CanLst[0x3C]=LstFyBuf[6];//密码4
			//Fy2CanLst[0x3D]=LstFyBuf[41];//
			//Fy2CanLst[0x3E]=LstFyBuf[42];//
			////FillLstCAN(id);//查询记录    //需要增加
			//flgLstTFTok=dualChNum;cntLstTFT=0;cntCanFb=8;canRtCnt=0;
}


void reNewSetDat(void)
{
	sys_GoodsId=OilCode[9];
	//sys_SavOvKg=SavFyDat[7];
	set_Ton =SavFyDat[0]; set_SpOp=SavFyDat[0];			//AB阀开阀时间
	set_Tall=SavFyDat[1]; set_SpCl=SavFyDat[1];			//AB阀开阀周期
	set_Ta_on=SavFyDat[2];set_SpAd=SavFyDat[2];			//AB阀调节周期
	set_Tadj=SavFyDat[3];  set_FUp=SavFyDat[3];			//AB阀调节周期
	//set_FUp =SavFyDat[3];			//开启稳流量:10Kg/s  	00000~99999Kg/s
	//set_FUp =SavFyDat[4];			//关闭稳流量: 1Kg/s  	00000~99999Kg/s
	set_FDn =SavFyDat[4];			//关闭稳流量: 1Kg/s  	00000~99999Kg/s
	set_FMx =SavFyDat[5];			//最大流量:	 40Kg/s   00000~99999Kg/s
	set_Ferr=SavFyDat[6];			//流量偏差:  (0~30)%  
	set_MDn =SavFyDat[7]*10;			//提前量:		200Kg     000~999Kg
	set_MOv =SavFyDat[8];			//过冲量:    20Kg  		000~999Kg
	set_Tsta=SavFyDat[9];		//开稳流时间:	10S				00~99s
	set_TOpBump=SavFyDat[10];	//提前开泵时间:	0			00~99s  
	set_TClBump=SavFyDat[11];	//延迟关泵时间:	0			00~99s
	set_Noise  =SavFyDat[12];flNoise=0.1*set_Noise;		//小信号切除
	set_ClValveTmr=SavFyDat[13];	//关阀超时
	set_OpValveTmr=SavFyDat[14];	//开阀超时
	set_FVtmr=SavFyDat[15];			//流量计超时
//
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
	Press_Up_Lmt=conf_YL_LmtUp/10.0;
	Press_Dn_Lmt=conf_YL_LmtDn/10.0;
	Md_Up_Lmt=conf_MD_LmtUp/10.0;
	Md_Dn_Lmt=conf_MD_LmtDn/10.0;
	Flow_On_Lmt=conf_FV_LmtDn;
	Flow_On_Lmt/=10.0;
	set_Config=SavConfDat[15];			//组态参数
	getWarnOnOff();
	cntABton=set_Ton*20;
	cntABtall=set_Tall*20;
	cntABtadj=set_Tadj*20;
	cntABta_on=set_Ta_on*20;
 	if(cntABton>cntABtall)
		cntABton=cntABtall;
//
	conf_YL_Scale=SavConfDat[4];
	conf_YL_LmtUp=SavConfDat[5];
	conf_YL_LmtDn=SavConfDat[6];
	conf_MD_LmtUp=SavConfDat[7];
	conf_MD_LmtDn=SavConfDat[8];
	Press_Up_Lmt=conf_YL_LmtUp;
	Press_Up_Lmt/=10.0;
	Press_Dn_Lmt=conf_YL_LmtDn;
  Press_Dn_Lmt/=10.0;
	Md_Up_Lmt= conf_MD_LmtUp;
	Md_Up_Lmt/=10.0;
	Md_Dn_Lmt= conf_MD_LmtDn;
	Md_Dn_Lmt/=10.0;
	set_Config=SavConfDat[16];			//组态参数
	getWarnOnOff();
}
//double DiffTime(void)
//{
// unsigned char time1[] =  { 10, 8, 31, 9, 26 };
// unsigned char time2[] =  { 10, 8, 31, 9, 50 };
// 
// struct tm t1 = {0};
// struct tm t2 = {0};
//
// time_t _t1;
// time_t _t2;
//
// double diff;
//
// t1.tm_year = time1[0] + 100;
// t1.tm_mon = time1[1];
// t1.tm_mday = time1[2];
// t1.tm_hour = time1[3];
// t1.tm_min = time1[4];
//
// t2.tm_year = time2[0] + 100;
// t2.tm_mon = time2[1];
// t2.tm_mday = time2[2];
// t2.tm_hour = time2[3];
// t2.tm_min = time2[4];
// 
// _t1 = _mkgmtime( &t1 );
// _t2 = _mkgmtime( &t2 );
//
// diff = difftime(_t2, _t1 );
// return diff;
//}

unsigned int sysWorkAllTmr;//发油所花费的总时间，单位为秒
//保存结算数据
void SavEndDat(void)
{
	unsigned int uwTemp;
	unsigned char i;
		//存入结束时间
		FyDateSp[0]=sysY;
		FyDateSp[1]=sysM;
		FyDateSp[2]=sysD;
		FyDateSp[3]=sysHH;
		FyDateSp[4]=sysMM;
		FyDateSp[5]=sysSS;
		//表后数
		dLJSv_last=dLJSv;dLJSm_last=dLJSm;//rtLJS_last1-rtLJS_last2;
		//保存记录
		uwTemp=Fnd_Lst_Index(dualChNum-1);
		Fill_New_Lst(dualChNum-1,uwTemp&0xFF);
		Wr_New_Lst(uwTemp);
		//cntFM++;
		//EepWrBuf[0]=0xAA;EepWrBuf[1]=0x55;EepWrBuf[4]=0x3A;
		//EepWrBuf[2]=cntFM>>8;EepWrBuf[3]=cntFM&0xFF;
		//EepWr(0x35F0,5);
		//SavLJS(0.0);
		//rdLJS=0.0;dSSLv_pls_first=0.0; dSSLv_pls=0.0;
		//清除预存记录
		for(i=0;i<40;i++)
			EepWrBuf[i]=0xFF;
		EepWr(0x3600,40);

}

char *Get_Pwd_Lst(unsigned int tIndex)
{
	unsigned char gi;
//	unsigned char thLos;
	char gBuf[64];
	char *rtnBuf;
	for(gi=0;gi<64;gi++)
		gBuf[gi]=0;
	rtnBuf=gBuf;
	if(tIndex>0xFF)
		return rtnBuf;
	EepRd(0x340+tIndex*64,64);
	
//	SPI2_Flash_Read(gBuf,0x5000+tIndex*64,64);
	for(gi=0;gi<64;gi++)
		LstFyBuf[gi]=EepRdBuf[gi];
	
	rtnBuf=LstFyBuf;
	return rtnBuf;
}

//根据提油密码查询记录
//返回记录位置,如果没有找到，则返回0xFFFF;
unsigned int Fnd_Pwd_Lst(char* OrdId)
{
//	unsigned char thCnt;
	unsigned char thi;
	unsigned char j;
	char tOrdId[9];
	char thBuf[64];
	char bufOrdId[9];
	for(thi=0;thi<8;thi++)
		tOrdId[thi]=OrdId[thi];
	tOrdId[8]=0;
	//最后记录在0~255中间
	for(thi=0;thi<200;thi++)
	{
		EepRd(0x340+thi*64,8);
		for(j=0;j<7;j++)
			thBuf[j]=EepRdBuf[j];	
		bufOrdId[0]='0'+(thBuf[3]>>4);
		bufOrdId[1]='0'+(thBuf[3]&0x0F);
		bufOrdId[2]='0'+(thBuf[4]>>4);
		bufOrdId[3]='0'+(thBuf[4]&0x0F);
		bufOrdId[4]='0'+(thBuf[5]>>4);
		bufOrdId[5]='0'+(thBuf[5]&0x0F);
		bufOrdId[6]='0'+(thBuf[6]>>4);
		bufOrdId[7]='0'+(thBuf[6]&0x0F);
		bufOrdId[8]=0;
		if(strcmp(tOrdId,bufOrdId)==0)//密码正确
		{
			return (thi);
		}
	}
	return 0xFFFF;
}




