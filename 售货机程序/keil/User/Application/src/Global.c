#include "GLobal.h"
unsigned char KeyCode,KeyCode_new,KeyCnt,KeyCnt_free,KeyPress,KeyRelease;
unsigned char KeyCommand;
unsigned char ManIndex=0,ManCur=0,ManPos=0;
unsigned char ManDoDat=0;
unsigned int  ManAoDat=0;
unsigned char flgManCon=0;
unsigned char flgFbFY;//需要返回数据给FY
unsigned char sysTxdCnt;
unsigned char flgWrOrdId;
unsigned char sysInfo;
unsigned char sysOldInfo;//Fy板上一状态
unsigned char flgConn;
unsigned char frmNum;
unsigned char flgAMT=0;//手自动标志
unsigned char frmNumLast;

/*--------Login var--------*/
char logUsr[2][7]={' ',' ',' ',' ',' ',' ','\0',' ',' ',' ',' ',' ',' ','\0'};
char tmplogUsr[2][7]={' ',' ',' ',' ',' ',' ','\0',' ',' ',' ',' ',' ',' ','\0'};
char tUsr[7]={' ',' ',' ',' ',' ',' ','\0'};
unsigned char curPos[2]={0,0};
unsigned char textPos=0;
unsigned char flgLogEdit=0;
char SavUsr[8][5];
char SavPwd[8][7];
char tmpSavUsr[8][5];
char tmpSavPwd[8][7];
char CardUsr[10];

/*--------Login var--------*/

unsigned char sys_STA;		//0:空闲,1:发油,2:开启,3:开稳流,4:全开,5:关稳流,6:发油结束,7:发油暂停.
unsigned char sys_Conn;		//0:本地,1:远程
unsigned char sys_ConnLast;

unsigned char UsrAvid=0;//
unsigned char UsrAvid_pre=0;

unsigned char flgCur=0;
unsigned char flgPendLst;//当前发油的提油密码在记录中存在，发油结束后需要删除
/*待发记录*/
unsigned char u8PendNum[2],u8PendNum_pre[2];
unsigned char Fy2CanLst[0x64];//下位机记录数据
char	c2f[4];

char nowOrdId[4];
char nowYFval[4];
char tmpYFL[9];
char tmpOrdId[9];
char frmMainOrdId[9]={' ',' ',' ',' ',' ',' ',' ',' ',' '};
char frmMainYFval[9]={' ',' ',' ',' ',' ',' ',' ',' ',' '};
char frm3ssl[7]={' ',' ',' ',' ',' ',' ','\0'};
char frm3ljs[12]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'};
char YflBuf[9];
//char nowPendLst[9];		//当前发油的提油密码
char nowPendLst[2][9];		//当前发油的提油密码
volatile float canSFLv[2];
volatile float dSSLv,dSSLm,dLJSv,dLJSm,dYFLv,dSFLv,dSFLm,dLJSv_first,dLJSm_first,dLJSv_last,dLJSm_last,dMD,dWD;
volatile float dSSLv_pre,dSSLm_pre,dLJSv_pre,dLJSm_pre,dYFLv_pre,dSFLv_pre,dSFLm_pre,dMD_pre,dWD_pre;
volatile float dSSLv_pls,rtSSLv_pls;
char FlashWarnBuf[256];
unsigned char SavOilId[15];	//由CAN总线得到了发油编码
unsigned char savY,savM,savD,savHH,savMM,savSS;
unsigned char sysY=11;
unsigned char sysM=1;
unsigned char sysD=10;
unsigned char sysHH=0;
unsigned char sysMM=0;
unsigned char sysSS=0;
unsigned char flgLstTFTok;//TFT保存完成
unsigned char flgDisDate,flgDisTime;
unsigned char FyDateSt[6],FyDateSp[6];
unsigned char UsrPos=0;
unsigned char PwdPos=0;
unsigned char IndexId=0;//0:工号输入,1:密码,2:保存,3:退出
unsigned char PwdCur=0;
unsigned char flgUsrSp=0;//用户暂停标志
//报警记录:0:静电,1:溢流,2:流量计,3:无流量,4:泄漏,5:密度,6:用户
unsigned char flgAlert[11];			
unsigned char flgAlertPre[11];
unsigned int  flgAlertCnt[11];
unsigned char flgAlertTmr[11];	
unsigned int  tmpSavConfWarn;	
unsigned char cntLstTFT;//TFT有记录发送
unsigned char flgCanFb;
unsigned char cntCanFb;
unsigned char canRtCnt;//实时发送
unsigned char NewPendBuf[30];

unsigned char flgFlash=0,flgFTA_IO=0,flgFTA_FV=0;
unsigned char flgDisTime;
unsigned char flgFlash_pre=0;
unsigned char sysHH_pre=99;
unsigned char sysMM_pre=99;
unsigned char sysSS_pre=99;
unsigned char sysY_pre=99;
unsigned char sysM_pre=99;
unsigned char sysD_pre=99;
unsigned char sys_STA_Last;		//0:空闲,1:发油,2:开启,3:开稳流,4:全开,5:关稳流,6:发油结束,7:发油暂停.
unsigned char flgWrRtc=0;
unsigned char NewPendBuf[30];
unsigned char PendBuf[10][25];
char NewPendAscBuf[9];//每收到一条待发记录，将待发记录拆分成ASC方式，以用作比较，当该待发记录验证通过后该缓存删除
char DelPendBuf[10];
unsigned char SavOilId[15];	//由CAN总线得到了发油编码
unsigned char flgMsgErr;

char PendCarId[9],CarId[9],LeadId[9];

float f_SSL_up,f_SSL_dn,f_SSL;
float f_Dn_Sp1,f_Dn_Sp2;//关阀速率1,2
float f_Seg1,f_Seg2;
float f_ExDn;//关稳超前
unsigned char flgSTA3to4=0;//状态从3改变到4
unsigned char flgSTA4to5=0;//状态从4改变到5
unsigned long  cntABta_on;
unsigned long  cntABtadj;
unsigned long  cntABton;
unsigned long  cntABtall;
unsigned int set_Ta_on;		  //AB阀调节宽度
unsigned int set_Tadj;				//AB阀调节周期
unsigned int set_Ton; 				//AB阀脉冲宽度
unsigned int set_Tall;				//AB阀脉冲周期
unsigned int set_Ta_on;				//AB阀调节周期
float flSeg1,flSeg2,flSeg3;			//分段流量
float  flSp1, flSp2;//,flSp3;			//分段调整速率

unsigned char set_AddMb=1,set_AddCa=5;
unsigned char sys_GoodsId=0;//货位号
char OilCode[10];//油品编码,用以作为记录保存的

unsigned char set_SpOp=20;		//开阀速率  :  20%/s  0~99%/s
unsigned char set_SpCl=20;		//关阀速度  :  20%/s  0~99%/s
unsigned char set_SpAd=20;		//关阀速度  :  20%/s  0~99%/s
unsigned int  set_FUp=10;			//开启稳流量: 10Kg/s  0~99999Kg/s
unsigned int  set_FDn=1;			//关闭稳流量:  1Kg/s  0~99999Kg/s
unsigned int  set_FMx=40;			//最大流量  : 40Kg/s  0~99999Kg/s
unsigned char set_Ferr=2;			//流量偏差  : 40Kg/s  0~99Kg/s
unsigned int  set_MDn=20;			//提前量    :200Kg    0~999Kg
unsigned int  set_MOv=0;			//过冲量    : 20Kg  	0~999Kg
unsigned char set_TOpBump=0;	//提前开泵时间:	0			0~99s  
unsigned char set_TClBump=0;	//延迟关泵时间:	0			0~99s
unsigned int  set_Tsta=10;		//开稳流时间:	10S			0~99s
unsigned int  set_KNeed=1;		//修正系数:			    0.9~1.100
unsigned int  set_BNeed=0;		//修正值:				      0~99Kg
unsigned int  set_Noise=0;		//小信号切除
unsigned int  set_ClValveTmr;	//关阀超时
unsigned int  set_OpValveTmr;	//开阀超时
unsigned int  set_FVtmr;			//流量计超时
unsigned int  set_Config;			//组态参数
float 				flNoise;				//flNoise=0.1*set_Noise;

unsigned char conf_MD_LmtUp;
unsigned char conf_MD_LmtDn;
unsigned char conf_YL_Scale;
unsigned char conf_YL_LmtUp;
unsigned char conf_YL_LmtDn;
unsigned int  conf_FV_LmtDn;//流量下限，专门用于全开阶段低流量限制

float Press_On_Lmt;//低压力值,全开时，低于此压力停车
float Flow_On_Lmt;//低流量值，全开时，流量低于此值，停车报警
float Press_Up_Lmt;
float Press_Dn_Lmt;
float Md_Up_Lmt;
float Md_Dn_Lmt;
float Fv_Dn_Lmt;//流量下限，专门用于全开阶段时

char NewLstBuf[80];//单个记录
char LstFyBuf[80];

float YFL[2],SFL[2];
char chrLeadId[2][9];//应发量，4位BCD码
char chrCarId[2][9];//应发量，4位BCD码
char chrYFL[2][9];//应发量存储
char chrOilId[2][9];//提油密码存储

char lstYFL[2][4];//应发量，4位BCD码
char lstSFL[3][4];//实发量，4位BCD码
char lstOilId[2][4];//提油密码
char lstMD[4]; //介质密度
char lstWD[4]; //介质温度
char bcdLJS[5];//前表数
unsigned char flgPendLst;//当前发油的提油密码在记录中存在，发油结束后需要删除

char LogUsrID[5];
unsigned char flgWrWarnLst;
unsigned long lng_KgNeed;//lng_KgNeed[2];
volatile float dSSLv_pls_first;
float rdLJS,savLJS;
unsigned char CarIdCnt,CarIdOK,LeadIdCnt,LeadIdOK,SavLeadId[4];
//SavConfDat组态参数 0:监控地址,1:管理地址,2:修正系数,3:修正值,4:压力量程,5:压力上限,6:压力下限,7:密度上限,8:密度下限,9:年,10:月,11:日,12:时,13:分,14:秒,15,组态
//日历算法???
char  SavConfDat[17]   ={2,5,100,0,5,5,5,5,5,11,12,0x03,0x07,0x08,0x01,0x01,0x00}; //default
char u8SavConfDat[17];
unsigned int  SavConfDatMax[17]={16,59,111,99,99,99,99,20,20,999,99,12,31,23,59,59,0xFF}; //max
unsigned int  SavConfDatMin[17]={ 2, 5, 90, 0, 0, 0, 0, 0, 0,  0, 0, 1, 1, 0, 0, 0,0}; //min
unsigned int u16SavConfDat;
char    SavConf[17][4];
char tmpSavConf[17][4];
char    u8SavConfBuf[7];
char    SavConfDate[2][7]; 		//日期与时间
char		tmpSavConfDate[2][7];	//日期与时间
//20150425
char idCarTypeId[10];//卡里车型号码
char idCarId[10];    //卡里车号[实际卡里没有，在待发确认界面中人工输入]
char idCarYFL[10];		//卡里应发量
unsigned char LstIndex=1;	//已发记录界面光标行数
unsigned int  ub250ms=0;
unsigned char flgFyEdit=0;
unsigned char flgOpValv;

unsigned char flgSFLdis;//实发量刷新,1:强制刷新,0:无作用
unsigned int  SavFyDat[18]={ 10, 10, 10,  5,  1, 10, 1,  5,  1,10, 5, 5, 0,10,10,10,100,100}; //default
const unsigned int  SavFyDatMax[18]={200,200,200,200,500,500,99,999, 99,99,99,99,30,99,99,999,150,150}; //max
const unsigned int  SavFyDatMin[18]={  1,  1,  1,  1,  1,  1, 0,  5,  0, 0, 0, 0, 0, 0, 0, 0, 80,80}; //min
char tmpSavFy[18][4];
char   tSavFyDat[34];
unsigned int u16SavFyDat;
char tmpSavMaxVol[8];
unsigned long SavMaxVol;
unsigned char SavMaxVol1,SavMaxVol2,SavMaxVol3;

unsigned int      SavManDat[4]={500,100,200,5};
unsigned int   SavManDatMax[4]={999,999,99,99};
unsigned int   SavManDatMin[4]={  1,  1, 1, 1};
char tmpSavMan[4][4];
char   tSavFyMan[8];
unsigned int u16SavManDat;
unsigned char EepWrBuf[200];
unsigned char EepRdBuf[200];
unsigned char flgReNew;//屏幕切换时强制更新;
//待发提单地址,1#货位保持原地址，2#货位地址=0x4000+PendAdd
unsigned int PendAdd[10]={260,285,310,335,360,385,410,435,460,485};
unsigned char dualChNum,dualChNum_pre;
unsigned char flgSysWarn;
unsigned char flgPwdEdit=0;
unsigned char WarnOnOff[11];	  //组态参数分拆
char bufSFLs[2][8];
char disSFLs[2][8];
unsigned char flgSFLs[2];

unsigned char frmMainCur=0,flgSdStart=0,flgSdEnd=0;
char bufSFL[11]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'};
char disSFL[11]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'};
unsigned char rioDat[50],rioDatpre[50],flgRioNew=0;
iCAN_msg      iCAN_TxMsg;                         // CAN messge for sending
iCAN_msg      iCAN_RxMsg;                         // CAN message for receiving                                
iCAN_msg      rtCAN_TxMsg[15];//原先为
iCAN_msg      lstCAN_TxMsg[10];
iCAN_msg      confCAN_TxMsg[3];//流量计数据
iCAN_msg      jobCAN_TxMsg[4];//作业数据

TH_MSG TH_Msg[2];
TH_MSG TH_Msg_Pre[2];
unsigned char flgFyEn,frmMainYFcur;
unsigned char LstCur;		//已发记录界面光标
unsigned char LstCnt;		//有效记录数
unsigned char LstPgNum;	//总页数
unsigned char LstDisPgNum;//显示的页
unsigned char LstPgNumIndex=0;//页数指针
unsigned char LstAllCnt;//总记录数
unsigned char LstDisCnt;//显示记录数
unsigned char DoDat,rtDoDat,valv_pump;

unsigned char u2Flg;
unsigned char u2NewTxdFlg;
//unsigned char u2RxdCnt;
unsigned char u2RxdEnd;
unsigned char flgFvETX;
unsigned char bufMdBuf[14][100];
unsigned char u4Buf[100];
unsigned char flgPkqDatOk;
unsigned int u2Tmr,u3Tmr;
unsigned char u2RxdCnt,u2RxdBuf[50],datMod2Buf[200];
unsigned char u3RxdCnt,u3RxdBuf[50],datMod3Buf[200];
unsigned char flgFvMbCrcOk,flgFvMbTmrOut,flgMb2CrcOk,flgMb3CrcOk;
unsigned int mbFvDat[50];
//unsigned int  mbdata[14][50];
unsigned int mbDat[100];

unsigned char flgU5Stx;
unsigned char u5STX;
unsigned int  u5Cnt;
char WeiDat[7],disWeiDat[7];
unsigned long lngWeiDat;

unsigned char u4STX,u4Dat,cntKK=0;
unsigned char u1STX,u1Dat;
unsigned char flgU4Stx1,flgU4Stx2,flgU4StRecv,flgU4StSend=0;
unsigned char flgU1Stx1,flgU1Stx2,flgU1StRecv,flgU1StSend=0;

unsigned char Txd2Buf[100],Txd3Buf[100];
unsigned char u3STX;
unsigned int  ftaDIdat,ftaDIdat_pre;
unsigned char ftaDOdat,ftaDOdat_pre;
unsigned char ftaDi[12],ftaDo[8];
												 
unsigned char Rxd1Buf[30],u1MbBuf[100];
unsigned char u1Cnt,u1SndCnt,u1Flag;//u2Cnt:????,u2Flag:????
unsigned char Rxd2Buf[30],u2MbBuf[100];
unsigned char u2Cnt,u2SndCnt,u2Flag;//u2Cnt:????,u2Flag:????
unsigned char Rxd3Buf[30],u3MbBuf[100];
unsigned char u3Cnt,u3SndCnt,u3Flag;//u2Cnt:????,u2Flag:????
unsigned char Rxd4Buf[30],u4MbBuf[100];
unsigned char u4Cnt,u4SndCnt,u4Flag;//u2Cnt:????,u2Flag:????
unsigned char Rxd6Buf[30],u6MbBuf[100];
unsigned char u6Cnt,u6SndCnt,u6Flag,u6Flag_pre;//u2Cnt:????,u2Flag:????
unsigned char u6Buf[100],u6FvBuf[100];

unsigned char u1TxCnt,u1RxCnt;
unsigned char u1TxDat[10],u1RxDat[100];
volatile unsigned char lenMbDat;
unsigned char u5RxdBuf[100];
unsigned char u6Flg,u6RxdCnt,u6NewTxdFlg;
unsigned char u3Flg,u3RxdCnt,u3NewTxdFlg,cntVconnTmr;
unsigned char datValv1[10],datValv2[10];
unsigned char cntValv,fbValv;//发送的阀体编号,反馈的阀体信号
unsigned char valvClosed[2],valvOpened[2];//关回讯，开回讯
unsigned char valvOpen[2];//开度
//0  1  2  3  4  5  6  7  8  9
//
//
//
unsigned char flgConnSTA[10],flgConnSTA_pre[10];
unsigned char rio_DO,rio_AoL1,rio_AoH1,rio_AoL2,rio_AoH2;

//测流速新增管径参数
unsigned int 	pipeSavConfDat=80; //管径缺省为80mm
unsigned int  pipeSavConfDatMax=999; //管径最大为999mm
unsigned int  pipeSavConfDatMin=10;  //管径最小为10mm
unsigned int  u16pipeSavConfDat;
char    pipeSavConf[4];
char tmppipeSavConf[4];
char    u8pipeSavConfBuf[4];

//考虑脉冲发油,新增两个参数,密度范围0.5000~3.0000,流量系数范围00.00~99.99
unsigned int 	plzSavConfDat[2]={0,100};
unsigned int  plzSavConfDatMax[2]={9999,65530};
unsigned int  plzSavConfDatMin[2]={ 0,0};
unsigned int  u16plzSavConfDat;
char    plzSavConf[2][6];
char tmpplzSavConf[2][6];
char    u8plzSavConfBuf[6];


//关阀参数新增5个
unsigned char 	dnSavConfDat[5]={10,60,10,80,1};
unsigned char  dnSavConfDatMax[5]={99,99,99,99,99};
unsigned char  dnSavConfDatMin[5]={1,1,1,1,0};
unsigned char  u8dnSavConfDat;
char tmpdnSavConf[5][3];
char    dnSavConf[5][3];
char  u8dnSavConfBuf[5];


//报警与联网组态7:,6:,5:,4:,3:,2:,1:,0:
//unsigned char    ConfWarn[7];
unsigned char tmpConfWarn[11];
//参数更新标志，1:更新,0:不更新,
unsigned char flgSavEn[17];
unsigned char ConfIndex=0;//0:0~9,1:10~19,1:
unsigned char ConfPos=0;
unsigned char flgConfEdit=0;
unsigned char ConfCur=0;
unsigned char AmsIndex=0;

volatile float rtPLZk;
volatile unsigned int rtPLerr;//脉冲偏差
unsigned char FyIndex,FyPos,flgManEdit;
float flAoDat;
unsigned int mAoDat,AoDat;
unsigned char flgPendEdit;
unsigned char PendPos=0;

volatile float dSFL[2],dYFL[2],dSFLpre[2],dSFL0,dSFL1,dSFL2,dYFL1,dYFL2,dSFLpre1,dSFLpre2;
volatile float dSSL[2],dSSL0,dSSL1,dSSL2;
volatile float dLJS[2],dLJS0,dLJS1,dLJS2;//累积数
volatile float dLJV[2],dLJV1,dLJV2;//累积体积
//volatile float dMD[2],dMD1,dMD2;
//volatile float dWD[2],dWD1,dWD2;
volatile float dRtd[2];
volatile float dLJS_first[2],dLJS_first1,dLJS_first2;
volatile float dLJS_last[2],dLJS_last1,dLJS_last2;
volatile float dLJV_first[2],dLJV_first1,dLJV_first2;
volatile float dLJV_last[2],dLJV_last1,dLJV_last2;

volatile float rtSFL=0.0;
volatile float rtSSL;
volatile float rtSSLv;
volatile float rtLJS;//累积数
volatile float rtLJV;//累积体积
volatile float rtMD;
volatile float rtWD;
volatile float rtRtd;
volatile float rtLJS_first;
volatile float rtLJS_last;
volatile float rtLJV_first;
volatile float rtLJV_last;
volatile float rtPLZk;

unsigned int  cntFVconnTmr;	 	//流量计通讯超时定时器
unsigned int  cntFV1connTmr;	 	//流量计通讯超时定时器
unsigned int  cntFV2connTmr;	 	//流量计通讯超时定时器
unsigned int  cntFVflow0;			//发油时流量为0超时定时器
unsigned int  cntFV1flow0;			//发油时流量为0超时定时器
unsigned int  cntFV2flow0;			//发油时流量为0超时定时器
unsigned int  cntOpValvTmr;		//开阀回讯超时
unsigned int  cntOpValv1Tmr;		//开阀回讯超时
unsigned int  cntOpValv2Tmr;		//开阀回讯超时
unsigned int  cntClValvTmr;		//关阀回讯超时
unsigned int  cntClValv1Tmr;		//关阀回讯超时
unsigned int  cntClValv2Tmr;		//关阀回讯超时
unsigned int  cntFlowLow;				//低流量超時

unsigned char err_PE;
unsigned char err_OvFlow;
unsigned char err_FlowNo=0;
unsigned char err_Leak=0;
unsigned char err_OvFlow1;
unsigned char err_OvFlow2;
unsigned char err_Dens;
unsigned char err_Press;
unsigned char err_FV=0;
unsigned char err_FlowLow; //低流量[液化气发油]
unsigned char err_PressLow;//低压力[液化气发油]

unsigned char TxdFvMdBuf[20];
WARN_LED WarnLedDat;
unsigned int WarnAvid;
unsigned int cntWarnAvid;
unsigned char flgRmCon=0,flgRmEnd=0;//远程控制发油，由机泵控制器控制
unsigned  int AoDat;
unsigned  int AoDat1,AoDat2;

unsigned char flgAlertDone;
unsigned char flgAlertRst;

unsigned char flgEnd;
unsigned int  cntEndTmr=0;
unsigned char cntRfid=0;
unsigned char cntRfid_pre=0;
unsigned char flgAMS;
unsigned int  uwCmpAMS;

unsigned char flgSysWarn=0;//系统故障时报警标志,1:声光,2:有光无声
unsigned long tYFLdat;
unsigned char flg1to5=0;//上电时关稳流

unsigned char flgABvalv;
unsigned long tRunCnt1,tRunCnt2;
unsigned long tTmr1,tTmr2;//提前开时间和提前关时间，等于秒数*倍率（200）
unsigned long tTmrCl,tTmrOp;//开阀故障与关阀故障定时器

unsigned char flgValv1=0,flgValv2=0,flgFValv=0;//阀1，阀2，副阀
unsigned int uwSp1,uwSp2,uwSp3;	//分段调整速率,第三段为
unsigned char dirErr;
unsigned int  sys_Cnt=0,sysTick=0,tickCnt=0,refCnt=0,tickTmrOut;
unsigned char flgWarnFlash=0;
unsigned char flg1S,sys_adj50ms;

/*------SD----Start--------*/
unsigned char flgSdCnt;
unsigned char flgSdMode;
unsigned char flgSdStart;
unsigned char flgSdEnd;
unsigned char SdAoDat;
unsigned int   sAoDat,mAoDat;//手动
/*------SD----End----------*/

unsigned char FyIndex=0;//0:0~9,1:10~19,1:
unsigned char FyPos=0;
unsigned char FyCur=0;
unsigned char flgFyEn=0;
unsigned char flgFyAllow=0,flgFyAllowPre=0xff;//
unsigned char LastProc,LastProc_pre;
unsigned char sysWarnLed[2],sysWarnLed_pre[2];//报警灯
unsigned char flgSt=0;//记录开始脉冲值
unsigned char flgKeySt=0;//
unsigned char swVer=1; //0:AO阀;1:AB阀,2:双路流量计AO阀
char    SavFy[18][4];
char    u8SavFyBuf[4];
unsigned int  bl_LCD=100;
unsigned char canChNum;
unsigned char delChNum,addChNum;
unsigned char flgAlertDone;
unsigned char flgAlertRst;
//unsigned char CarId[2][7],PendCarId[2][7];
char PendCarId[9],CarId[9],LeadId[9];
unsigned char CarIdCnt,CarIdOK,LeadIdCnt,LeadIdOK,SavLeadId[4];
unsigned char flgViceValv;
unsigned char DiDatHi,DiDatLo,DiDat;
unsigned char DoDat,rtDoDat;
unsigned char flgCANerr=0,flgIFIXerr=0,flgFVerr[2]={0,0};
unsigned long	tmrCAN=0,tmrIFIX=0;
unsigned long tmrFV[2]={0,0};
unsigned char iFix_id=0;
volatile float can_rtSSL,canSSL[2],canWD[2],canMD[2],canSFL[2],canLJS[2],can_YFL[2];
float can_rtWD;
float can_rtMD;
float can_rtSFL;
float can_rtLJS;
float can_rtYFL;
vu8 CanRxdBuf[100];
char tSqlBuf[10];
unsigned char CanData[220];
unsigned char canRtData[50];//实时数据
unsigned char canJobData[30];//作业数据:提单/车号/应发量
unsigned char canLstData[60];//记录数据
unsigned char canConfData[21];//组态数据
unsigned int  rtnLos;
unsigned char cntIC=0;
unsigned char flgIcDat1OK;
unsigned char flgU5Stx1;		  //开始接收到第一个0xFF
unsigned char flgU5Stx2;			//接收到0xFF后，再接收到0xCC
unsigned char flgU5StRecv;		//发送完成，开始接收
unsigned char u5RxdCnt;
unsigned char flg250ms;
unsigned char u5RxdLen,flgAll0;
unsigned char icBuf[10];
unsigned char frmMainEdit=0;
unsigned char flgEdit=0;
unsigned char ubKey_Command=0;
unsigned char frmMainCurX,frmMainCurY;
unsigned char PadCode[3],PadCodePre[3],flgPad;
unsigned char KeyCmd;
unsigned char LastDisLos;//显示记录界面中保存最后一个显示位，
unsigned char cntLstPage;				//当页记录数
char ManLstBuf[3][80];//手动上传表单数据缓存,每页3个记录，每个记录

unsigned char lst_WarnCnt;
unsigned char lst_DoneCnt;
unsigned char rtDi;
unsigned char KeyCon;
unsigned char flgWarnWr;
unsigned char flg_bl_LCD=1;
unsigned int  WarnID;
unsigned int  WarnAvid;
unsigned int  cntWarnAvid;
unsigned char flgFvConn,flgFvConn_pre,cntFvConn;
unsigned char flgFmConn,flgFmConn_pre,cntFmConn;

unsigned char flgCanSTA;//CAN状态，用以回复报文使用
//0:响应巡检,0x01:有作业数据待上传,0x02:有记录数据待上传
unsigned char cntCanSTA;//重要的事情传三遍
unsigned char cntJobCan;
unsigned char cntConfCan;
unsigned char cntLstCan;
unsigned char FvId;
unsigned long tmrU6,tmrU5;
unsigned long tDatInt,tConInt;
unsigned char FvWarnDat[6];
unsigned char FvClibDat[4];
unsigned char FvSetDat[12];
unsigned char FvZeroDat[4];
unsigned int  staValv[2];
/*[9]*[14]*/
const unsigned int RES_Top_Left_BIN[126] = {
0xffff,0xffff,0xffff,0xffff,0xfd14,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xfd14,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xfd14,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xfd14,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xfd14,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfb0c,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfb0c};

/*[14]*[9]*/
const unsigned int RES_left_up_BIN[126] = {
0xffff,0xffff,0xffff,0xffff,0xfd14,0xfd14,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff, 
0xffff,0xffff,0xffff,0xfd14,0xf800,0xf800,0xfd14,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff, 
0xffff,0xffff,0xfd14,0xf800,0xf800,0xf800,0xf800,0xfd14,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff, 
0xffff,0xfd14,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfd14,0xffff,0xffff,0xffff,0xffff,0xffff, 
0xfd14,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfd14,0xffff,0xffff,0xffff,0xffff, 
0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfd14,0xffff,0xffff,0xffff, 
0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfd14,0xffff,0xffff, 
0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfd14,0xffff,
0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfd14};

const unsigned int RES_dn_BIN[126] = {
0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfb0c, 
0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfb0c,0xffff, 
0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfb0c,0xffff,0xffff, 
0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xfb0c,0xffff,0xffff,0xffff, 
0xffff,0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800,0xfb0c,0xffff,0xffff,0xffff,0xffff, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800,0xfb0c,0xffff,0xffff,0xffff,0xffff,0xffff, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfb0c,0xfb0c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff};

const unsigned int RES_left_BIN[126] = {
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfd14, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xfd14,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xfd14,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xfd14,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xfd14,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xfd14,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xfd14,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xfb0c,0xf800,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xfb0c,0xf800, 
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfb0c};





