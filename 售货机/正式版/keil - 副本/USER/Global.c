#include "Global.h"
#include "can.h"

unsigned char canChNum;
unsigned char delChNum,addChNum;
unsigned int  CAN_TxRdy = 0;                      // CAN HW ready to transmit a message
unsigned int  CAN_RxRdy = 0;                      // CAN HW received a message
iCAN_msg      iCAN_RxMsg;                         // CAN message for receiving                                
iCAN_msg      iCAN_TxMsg;                         // CAN message for receiving                                
iCAN_msg      rtCAN_TxMsg[10];//???
iCAN_msg      lstCAN_TxMsg[10];
iCAN_msg      bufCAN_RxMsg[100];
iCAN_msg      datCAN_RxMsg[100];

unsigned char bufCanData[24][105];
unsigned char CanData[24][220];
unsigned char MB[48][200];
unsigned char cntCAN_RxMsg;
unsigned char rdCanBuf[200];
unsigned char wrCanBuf[200];
char tSqlBuf[10];
float can_rtWD;
float can_rtMD;
float can_rtSFL;
float can_rtLJS;
float can_rtYFL;

unsigned char set_AddCa=5;
unsigned char flgCanFb;
unsigned char cntCanFb;
unsigned int  rtnLos;
vu32 tId;

unsigned char CarIdCnt,CarIdOK,LeadIdCnt,LeadIdOK,SavLeadId[4];
unsigned char flgViceValv;
unsigned char DiDatHi,DiDatLo,DiDat;
unsigned char DoDat,rtDoDat;
unsigned char flgCANerr=0,flgIFIXerr=0,flgFVerr[2]={0,0};
unsigned long	tmrCAN=0,tmrIFIX=0;
unsigned long tmrFV[2]={0,0};
unsigned char iFix_id=0;
char PendCarId[9],CarId[9],LeadId[9];
unsigned char flgAlertDone;
unsigned char flgAlertRst;

float rtAi;
float flPress;
unsigned int mbdata[200];//?100?1#??,?100?2#??
unsigned int mbBuf[200]; //?100?1#??,?100?2#??

unsigned char flgLstTFTok;
unsigned char canRtCnt=0;//????
unsigned char WarnOnOff[11],tmpConfWarn[11],sys_Conn;	  //??????
unsigned char EepWrBuf[200];
unsigned char EepRdBuf[200];
unsigned char CanRxdBuf[100];
char	c2f[4];
unsigned char flgDisTime;
u8 flgFlash_pre=0;
u8 sysHH_pre=99;
u8 sysMM_pre=99;
u8 sysSS_pre=99;
// extern unsigned char flgFlash;
vu16	AD_Value[10];

u8 sysY_pre=99;
u8 sysM_pre=99;
u8 sysD_pre=99;
unsigned char sys_STA_Last;		//0:??,1:??,2:??,3:???,4:??,5:???,6:????,7:????.
u8 flgWrRtc=0;
unsigned char NewPendBuf[30];
unsigned char PendBuf[10][25];
char NewPendAscBuf[9];//?????????,????????ASC??,?????,????????????????
char DelPendBuf[10];
unsigned char SavOilId[15];	//?CAN?????????
unsigned char flgMsgErr;

unsigned char flgU5RecvEn,u5Cnt,u5RxdBuf[200];

unsigned char flgSTX;
unsigned char flgETX;
unsigned char flgSlaveTmrOut;	//??TimeOut
unsigned int  uwSlaveTmrOut;	//??TimeOut
unsigned char cntLstTFT;//TFT?????
unsigned char flgLstTFTok;//TFT????
unsigned char flgFvETX;
unsigned char flgFv1ETX;
unsigned char flgFv2ETX;
unsigned char sys_adj50ms=0;
//unsigned int  StaDat=0;
unsigned char cntIC=0;
unsigned char flgFvRxd=0;
unsigned int  cntFvErr=0;
unsigned int  cntFv1Err;
unsigned int  cntFv2Err;
unsigned int  cntLastTmr;
unsigned int  cntLastTmrPre;//???????
unsigned char flgFcDis=0;//????
unsigned char frmNum=1;
unsigned char flgDisDate;
unsigned char flgDisTime;
unsigned char flgReNew;//屏幕切换时强制更新;
unsigned char flgCur;//屏幕切换时强制更新;
unsigned char set_AddMb=1;
unsigned char sys_GoodsId=0;//货位号
unsigned char flgDisDate;
unsigned char sysY_pre,sysY,sysM_pre,sysM,sysD_pre,sysD;
unsigned char u8PendNum;
unsigned char sys_STA;
unsigned char ConfIndex,ConfPos,FyIndex,FyPos;
unsigned char frmMainCurX=0,frmMainCurY=0;
unsigned char disSlaveId;//子站显示ID号
char	c2f[4];
unsigned char frmNum;
unsigned char frmNumLast;
unsigned int  SavFyDat[16]   ={ 1, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15}; //default
unsigned int  SavFyDatMax[16]={32,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15}; //max
unsigned int  SavFyDatMin[16]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //min
char tmpSavFy[16][4];
char   tSavFyDat[32];
unsigned int  u16SavFyDat;
unsigned char pollSpan=20;	//轮询间隔
unsigned char retryCnt=2;	//重试次数
unsigned char PlcDelay=16; //设置延时时间
char tmpPollSpan[3][2];	//暂存以上三个变量
//char tmpRetry[4];
char tmpDateTime[2][6];
char  SavDateTmrDat[6];
unsigned long sysTmrOut=1000;//sysTmrOut=pollSpan*100;
char    SavFy[16][4];
char    u8SavFyBuf[4];
unsigned char  sysHH,sysMM,sysSS,sysHH_pre,sysMM_pre,sysSS_pre;
unsigned char flgRmCon=0,flgRmEnd=0;//??????,????????
unsigned char flgWrWarnLst;//??????,????????
volatile float can_rtSSL,canSSL[2],canWD[2],canMD[2],canSFL[2],canLJS[2],can_YFL[2];
volatile float canSFLv[2];
volatile float dSSLv,dSSLm,dLJSv,dLJSm,dYFLv,dSFLv,dSFLm,dLJSv_first,dLJSm_first,dLJSv_last,dLJSm_last,dMD,dWD;
volatile float dSSLv_pls,rtSSLv_pls;
volatile float rtPLZk;
volatile unsigned int rtPLerr;//????
char *gSqlBuf;
unsigned char tmpDat;
char NewLstBuf[80];//????
char LstFyBuf[80];
unsigned char flgEndLst;//??????,?????1,?????????0
unsigned char flgSqrLst;//????
unsigned char ubKey_Command; //Key Command
unsigned char FyIndex=0;//0:0~9,1:10~19,1:
unsigned char FyPos=0;
unsigned char FyCur=0;
unsigned char flgFyEn=0;
unsigned char flgFyAllow=0,flgFyAllowPre=0xff;//
unsigned char dualChNum;//???,???0,?FRM_MON???,??1?dualChNum=1,????1,??2?????2
unsigned char dualChNum_pre;//????????
unsigned char flgAlert[11];			
unsigned char flgSysWarn;
unsigned int  AoDat,AoDat1,AoDat2;
float YFL[2],SFL[2];
unsigned char flgU5Stx;
unsigned char u5STX;
unsigned char pkqDat[24][190];
void ftoc(float tta);
float ctof(u8 dt1,u8 dt2,u8 dt3,u8 dt4);

void ftoc(float tta)
{
 unsigned char i;
 char *px;
 //unsigned char x[4];
 void *pf;
 px=c2f;
 pf=&tta;
 /*a=34.25;*/
 for(i=0;i<4;i++)
 *(px+i)=*((char *)pf+i);
}

float ctof(u8 dt1,u8 dt2,u8 dt3,u8 dt4)
{
float tta;
u8 i,*px;
u8 x[4];
void *pf;
x[0]=dt4;
x[1]=dt3;
x[2]=dt2;
x[3]=dt1;

px=x;             //px??????x
pf=&tta;
for(i=0;i<4;i++)
 *((char *)pf+i)=*(px+i);
return tta;
}

float YFL[2],SFL[2];
char chrLeadId[2][9];//???,4?BCD?
char chrCarId[2][9];//???,4?BCD?
char chrYFL[2][9];//?????
char chrOilId[2][9];//??????

char lstYFL[2][4];//???,4?BCD?
char lstSFL[3][4];//???,4?BCD?
char lstOilId[2][4];//????
char lstMD[4]; //????
char lstWD[4]; //????
char bcdLJS[5];//???
unsigned char mdRxdBuf[50];
unsigned char Fy2CanLst[0x64];//???????
unsigned char canEnSendRt=0;//????CAN????
vu8 lstOrdId[4];
unsigned int PendAdd[10]={260,285,310,335,360,385,410,435,460,485};


