#ifndef __GLOBAL_H
#define __GLOBAL_H 
#include "can.h"

#define FRM_LOGIN  	21	//登陆
#define FRM_PWD			22	//密码
#define FRM_MAIN		23	//主界面
#define FRM_PENDLST	24	//待发记录
#define FRM_DONELST	25	//已发记录
#define FRM_WARN		26	//报警记录
#define FRM_DETAIL	26	//报警记录
#define FRM_SETTING	27	//参数设定
#define FRM_MAN			28	//测试界面
#define FRM_SD			29	//手动操作
#define FRM_CONFIG  30//组态界面

#define GOLD  0xFEA0
#define RED	  0xF800
#define GREEN 0x07E0
#define BLUE  0x001F  
#define BRED  0xF81F
#define GRED  0xFFE0
#define GBLUE 0x07FF
#define WHITE 0xFFFF //??
#define BLACK 0x0000 //??
#define BROWN 0xBC40 //??
#define BRRED 0xFC07 //???
#define GRAY  0x8430 //??
#define LGRAY 0xC618 //???


extern unsigned char canChNum;
extern unsigned char delChNum,addChNum;
extern unsigned int  CAN_TxRdy;                      // CAN HW ready to transmit a message
extern unsigned int  CAN_RxRdy;                      // CAN HW received a message
extern iCAN_msg      iCAN_RxMsg;                         // CAN message for receiving                                
extern iCAN_msg      iCAN_TxMsg;                         // CAN message for receiving                                
extern iCAN_msg      rtCAN_TxMsg[10];//???
extern iCAN_msg      lstCAN_TxMsg[10];
extern iCAN_msg      bufCAN_RxMsg[100];
extern iCAN_msg      datCAN_RxMsg[100];

extern unsigned int  rtDatChk[24];
extern unsigned char bufCanData[24][105];
extern unsigned char CanData[24][220];
extern unsigned char MB[48][200];
extern unsigned char cntCAN_RxMsg;

extern unsigned char rdCanBuf[200];
extern unsigned char wrCanBuf[200];
extern  char tSqlBuf[10];

extern float can_rtWD;
extern float can_rtMD;
extern float can_rtSFL;
extern float can_rtLJS;
extern float can_rtYFL;

extern unsigned char set_AddCa;
extern unsigned char flgCanFb;
extern unsigned char cntCanFb;
extern unsigned int  rtnLos;
extern vu32 tId;

extern unsigned char CarIdCnt,CarIdOK,LeadIdCnt,LeadIdOK,SavLeadId[4];
extern unsigned char flgViceValv;
extern unsigned char DiDatHi,DiDatLo,DiDat;
extern unsigned char DoDat,rtDoDat;
extern unsigned char flgCANerr,flgIFIXerr,flgFVerr[2];
extern unsigned long	tmrCAN,tmrIFIX;
extern unsigned long tmrFV[2];
extern unsigned char iFix_id;
extern char PendCarId[9],CarId[9],LeadId[9];
extern unsigned char flgAlertDone;
extern unsigned char flgAlertRst;
extern float rtAi;
extern float flPress;
extern unsigned int mbdata[200];//?100?1#??,?100?2#??
extern unsigned int mbBuf[200]; //?100?1#??,?100?2#??

extern unsigned char flgLstTFTok;
extern unsigned char canRtCnt;//????
extern unsigned char WarnOnOff[11],tmpConfWarn[11],sys_Conn;	  //??????
extern unsigned char EepWrBuf[200];
extern unsigned char EepRdBuf[200];
extern unsigned char CanRxdBuf[100];
extern char	c2f[4];
extern unsigned char flgDisTime;
extern unsigned char flgFlash_pre;
extern unsigned char sysHH_pre;
extern unsigned char sysMM_pre;
extern unsigned char sysSS_pre;
// extern extern  char flgFlash;
extern vu16	AD_Value[10];

extern unsigned char sysY_pre;
extern unsigned char sysM_pre;
extern unsigned char sysD_pre;
extern unsigned char sys_STA_Last;		//0:??,1:??,2:??,3:???,4:??,5:???,6:????,7:????.
extern unsigned char flgWrRtc;
extern unsigned char NewPendBuf[30];
extern unsigned char PendBuf[10][25];
extern char NewPendAscBuf[9];//?????????,????????ASC??,?????,????????????????
extern char DelPendBuf[10];
extern unsigned char SavOilId[15];	//?CAN?????????
extern unsigned char flgMsgErr;

extern unsigned char flgU5RecvEn,u5Cnt,u5RxdBuf[200];

extern unsigned char flgSTX;
extern unsigned char flgETX;
extern unsigned char flgSlaveTmrOut;	//??TimeOut
extern unsigned int  uwSlaveTmrOut;	//??TimeOut
extern unsigned char cntLstTFT;//TFT?????
extern unsigned char flgLstTFTok;//TFT????
extern unsigned char flgFvETX;
extern unsigned char flgFv1ETX;
extern unsigned char flgFv2ETX;
extern unsigned char sys_adj50ms;
//extern unsigned int  StaDat=0;
extern unsigned char cntIC;
extern unsigned char flgFvRxd;
extern unsigned int  cntFvErr;
extern unsigned int  cntFv1Err;
extern unsigned int  cntFv2Err;
extern unsigned int  cntLastTmr;
extern unsigned int  cntLastTmrPre;//???????
extern unsigned char flgFcDis;//????
extern unsigned char frmNum;
extern unsigned char flgDisDate;
extern unsigned char flgDisTime;
extern unsigned char flgReNew;//屏幕切换时强制更新;
extern unsigned char flgCur;//屏幕切换时强制更新;
extern unsigned char set_AddMb;
extern unsigned char sys_GoodsId;//货位号
extern unsigned char flgDisDate;
extern unsigned char sysY_pre,sysY,sysM_pre,sysM,sysD_pre,sysD;
extern unsigned char u8PendNum;
extern unsigned char sys_STA;
extern unsigned char ConfIndex,ConfPos,FyIndex,FyPos;
extern unsigned char frmMainCurX,frmMainCurY;
extern unsigned char disSlaveId;//子站显示ID号
extern char	c2f[4];
extern unsigned char frmNum;
extern unsigned char frmNumLast;
extern unsigned int  SavFyDat[16]; //default
extern unsigned int  SavFyDatMax[16]; //max
extern unsigned int  SavFyDatMin[16]; //min
extern char tmpSavFy[16][4];
extern char   tSavFyDat[32];
extern unsigned int  u16SavFyDat;
extern unsigned char pollSpan;	//轮询间隔
extern unsigned char retryCnt;	//重试次数
extern unsigned char PlcDelay; //设置延时时间
extern char tmpPollSpan[3][2];	//暂存以上三个变量
//extern char tmpRetry[4];
extern char tmpDateTime[2][6];
extern char  SavDateTmrDat[6];
extern unsigned long sysTmrOut;//sysTmrOut=pollSpan*100;
extern char    SavFy[16][4];
extern char    u8SavFyBuf[4];
extern unsigned char  sysHH,sysMM,sysSS,sysHH_pre,sysMM_pre,sysSS_pre;
extern unsigned char flgRmCon,flgRmEnd;//??????,????????
extern unsigned char flgWrWarnLst;//??????,????????
extern volatile float can_rtSSL,canSSL[2],canWD[2],canMD[2],canSFL[2],canLJS[2],can_YFL[2];
extern volatile float canSFLv[2];
extern volatile float dSSLv,dSSLm,dLJSv,dLJSm,dYFLv,dSFLv,dSFLm,dLJSv_first,dLJSm_first,dLJSv_last,dLJSm_last,dMD,dWD;
extern volatile float dSSLv_pls,rtSSLv_pls;
extern volatile float rtPLZk;
extern volatile unsigned int rtPLerr;//????
extern char *gSqlBuf;
extern unsigned char tmpDat;
extern char NewLstBuf[80];//????
extern char LstFyBuf[80];
extern unsigned char flgEndLst;//??????,?????1,?????????0
extern unsigned char flgSqrLst;//????
extern unsigned char ubKey_Command; //Key Command
extern unsigned char FyIndex;//0:0~9,1:10~19,1:
extern unsigned char FyPos;
extern unsigned char FyCur;
extern unsigned char flgFyEn;
extern unsigned char flgFyAllow,flgFyAllowPre;//
extern unsigned char dualChNum;//???,???0,?FRM_MON???,??1?dualChNum=1,????1,??2?????2
extern unsigned char dualChNum_pre;//????????
extern unsigned char flgAlert[11];			
extern unsigned char flgSysWarn;
extern unsigned int  AoDat,AoDat1,AoDat2;
extern float YFL[2],SFL[2];
extern unsigned char flgU5Stx;
extern unsigned char u5STX;
extern unsigned char pkqDat[24][190];

extern float YFL[2],SFL[2];
extern char chrLeadId[2][9];//???,4?BCD?
extern char chrCarId[2][9];//???,4?BCD?
extern char chrYFL[2][9];//?????
extern char chrOilId[2][9];//??????

extern char lstYFL[2][4];//???,4?BCD?
extern char lstSFL[3][4];//???,4?BCD?
extern char lstOilId[2][4];//????
extern char lstMD[4]; //????
extern char lstWD[4]; //????
extern char bcdLJS[5];//???
extern unsigned char mdRxdBuf[50];
extern unsigned char Fy2CanLst[0x64];//???????
extern unsigned char canEnSendRt;//????CAN????
extern vu8 lstOrdId[4];
extern unsigned int PendAdd[10];

extern void ftoc(float tta);
extern float ctof(u8 dt1,u8 dt2,u8 dt3,u8 dt4);
extern void CAN_EVENTS(void);
extern void RTCSetDate(unsigned char tY,unsigned char tM,unsigned char tD,unsigned char tW);
extern void RTCSetTime(unsigned char tH,unsigned char tM,unsigned char tS);
extern void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void Dis_Date(void);
extern void Dis_Time(void);

#endif


