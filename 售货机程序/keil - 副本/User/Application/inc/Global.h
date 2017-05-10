#ifndef __GLOBAL_H_
#define __GLOBAL_H_
#include "stm32f4xx_can.h"
#include "font.h"

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

#define AB_Empty 0
#define AB_In    1
#define AB_Out   2
#define AB_Hold  3
#define AB_dIn   4
#define AB_dOut  5
#define AO_LMT   2600
#define AO_LMTf  2600.0

#define KEY_U_ARROW			0x11 		
#define KEY_D_ARROW			0x12
#define KEY_L_ARROW			0x13
#define KEY_R_ARROW			0x14

//此键盘仅仅是大连锦源使用，
//#define KEY_U_ARROW			0x14
//#define KEY_D_ARROW			0x11
//#define KEY_L_ARROW			0x13
//#define KEY_R_ARROW			0x12
#define KEY_SET				0x21
#define KEY_ENTER			0x22
#define KEY_END				0x23
#define KEY_DEL				0x24
#define KEY_RTN				0x25
#define KEY_START1		0x31
#define KEY_END1			0x32
#define KEY_START2		0x33
#define KEY_END2			0x34
#define KEY_Esc				0x25

#define AB_Valv     1
#define AO_Valv     0
#define FRM_LOGIN  	1	//登陆
#define FRM_PWD			2	//密码
#define FRM_MAIN		3	//主界面
#define FRM_PENDLST	4	//待发记录
#define FRM_DONELST	5	//已发记录
#define FRM_WARN		6	//报警记录
#define FRM_SETTING	7	//参数设定
#define FRM_MAN			8	//测试界面
#define FRM_SD			9	//手动操作
#define FRM_CONFIG  10//组态界面
#define FRM_AMS  		11//AMS界面
#define FRM_MON			12//双路监控界面
#define FRM_PENDSEL	13//待发确认界面
#define FRM_CALC    14//计算机界面

#define TmrRatio 20
//#define FRM_LOGIN  	21	//登陆
//#define FRM_PWD			22	//密码
//#define FRM_MAIN		23	//主界面
//#define FRM_PENDLST	24	//待发记录
//#define FRM_DONELST	25	//已发记录
//#define FRM_WARN		26	//报警记录
//#define FRM_DETAIL	26	//报警记录
//#define FRM_SETTING	27	//参数设定
//#define FRM_MAN			28	//测试界面
//#define FRM_SD			29	//手动操作
//#define FRM_CONFIG  30//组态界面
//#define FRM_MON     31//组态界面

extern unsigned char KeyCode,KeyCode_new,KeyCnt,KeyCnt_free,KeyPress,KeyRelease;
extern unsigned char KeyCommand;
extern unsigned char ManIndex,ManCur,ManPos;
extern unsigned char ManDoDat;
extern unsigned int  ManAoDat;
extern unsigned char flgManCon;
extern unsigned char flgFbFY;//需要返回数据给FY
extern unsigned char sysTxdCnt;
extern unsigned char flgWrOrdId;
extern unsigned char sysInfo;
extern unsigned char sysOldInfo;//Fy板上一状态
extern unsigned char flgConn;
extern unsigned char frmNum;
extern unsigned char flgAMT;//手自动标志
extern unsigned char frmNumLast;
extern unsigned char flgLogEdit;

/*--------Login var--------*/
extern char logUsr[2][7];
extern char tmplogUsr[2][7];
extern char tUsr[7];
extern unsigned char curPos[2];
extern unsigned char textPos;
extern unsigned char flgLogEdit;
extern char SavUsr[8][5];
extern char SavPwd[8][7];
extern char tmpSavUsr[8][5];
extern char tmpSavPwd[8][7];
extern char CardUsr[10];
/*--------Login var--------*/

extern unsigned char sys_STA;		//0:空闲,1:发油,2:开启,3:开稳流,4:全开,5:关稳流,6:发油结束,7:发油暂停.
extern unsigned char sys_Conn;		//0:本地,1:远程
extern unsigned char sys_ConnLast;

extern unsigned char UsrAvid;//
extern unsigned char UsrAvid_pre;

extern unsigned char flgCur;
extern unsigned char flgPendLst;//当前发油的提油密码在记录中存在，发油结束后需要删除
extern unsigned char u8PendNum[2],u8PendNum_pre[2];
extern unsigned char Fy2CanLst[0x64];//下位机记录数据
extern char	c2f[4];

extern char nowOrdId[4];
extern char nowYFval[4];
extern char tmpYFL[9];
extern char tmpOrdId[9];
extern char frmMainOrdId[9];
extern char frmMainYFval[9];
extern char frm3ssl[7];
extern char frm3ljs[12];
extern char YflBuf[9];
//char nowPendLst[9];		//当前发油的提油密码
extern char nowPendLst[2][9];		//当前发油的提油密码
extern volatile float canSFLv[2];
extern volatile float dSSLv,dSSLm,dLJSv,dLJSm,dYFLv,dSFLv,dSFLm,dLJSv_first,dLJSm_first,dLJSv_last,dLJSm_last,dMD,dWD;
extern volatile float dSSLv_pre,dSSLm_pre,dLJSv_pre,dLJSm_pre,dYFLv_pre,dSFLv_pre,dSFLm_pre,dMD_pre,dWD_pre;
extern volatile float dSSLv_pls,rtSSLv_pls;

extern char FlashWarnBuf[256];
extern unsigned char SavOilId[15];	//由CAN总线得到了发油编码
extern unsigned char savY,savM,savD,savHH,savMM,savSS;
extern unsigned char sysY;
extern unsigned char sysM;
extern unsigned char sysD;
extern unsigned char sysHH;
extern unsigned char sysMM;
extern unsigned char sysSS;
extern unsigned char flgLstTFTok;//TFT保存完成

extern unsigned char flgDisDate,flgDisTime;
extern unsigned char FyDateSt[6],FyDateSp[6];
extern unsigned char UsrPos;
extern unsigned char PwdPos;
extern unsigned char IndexId;//0:工号输入,1:密码,2:保存,3:退出
extern unsigned char PwdCur;
extern unsigned char flgUsrSp;//用户暂停标志
//报警记录:0:静电,1:溢流,2:流量计,3:无流量,4:泄漏,5:密度,6:用户
extern unsigned char flgAlert[11];			
extern unsigned char flgAlertPre[11];
extern unsigned int  flgAlertCnt[11];
extern unsigned char flgAlertTmr[11];	
extern unsigned int  tmpSavConfWarn;	
extern unsigned char cntLstTFT;//TFT有记录发送
extern unsigned char flgCanFb;
extern unsigned char cntCanFb;
extern unsigned char canRtCnt;//实时发送
extern unsigned char NewPendBuf[30];

extern unsigned char flgFlash,flgFTA_IO,flgFTA_FV;
extern unsigned char flgDisTime;
extern unsigned char flgFlash_pre;
extern unsigned char sysHH_pre;
extern unsigned char sysMM_pre;
extern unsigned char sysSS_pre;
extern unsigned char sysY_pre;
extern unsigned char sysM_pre;
extern unsigned char sysD_pre;
extern unsigned char sys_STA_Last;		//0:空闲,1:发油,2:开启,3:开稳流,4:全开,5:关稳流,6:发油结束,7:发油暂停.
extern unsigned char flgWrRtc;
extern unsigned char NewPendBuf[30];
extern unsigned char PendBuf[10][25];
extern char NewPendAscBuf[9];//每收到一条待发记录，将待发记录拆分成ASC方式，以用作比较，当该待发记录验证通过后该缓存删除
extern char DelPendBuf[10];
extern unsigned char SavOilId[15];	//由CAN总线得到了发油编码
extern unsigned char flgMsgErr;
extern char PendCarId[9],CarId[9],LeadId[9];

extern float f_SSL_up,f_SSL_dn,f_SSL;
extern float f_Dn_Sp1,f_Dn_Sp2;//关阀速率1,2
extern float f_Seg1,f_Seg2;
extern float f_ExDn;//关稳超前
extern unsigned char flgSTA3to4;//状态从3改变到4
extern unsigned char flgSTA4to5;//状态从4改变到5
extern unsigned long  cntABta_on;
extern unsigned long  cntABtadj;
extern unsigned long  cntABton;
extern unsigned long  cntABtall;
extern unsigned int set_Ta_on;		  //AB阀调节宽度
extern unsigned int set_Tadj;				//AB阀调节周期
extern unsigned int set_Ton; 				//AB阀脉冲宽度
extern unsigned int set_Tall;				//AB阀脉冲周期
extern unsigned int set_Ta_on;				//AB阀调节周期
extern float flSeg1,flSeg2,flSeg3;			//分段流量
extern float  flSp1, flSp2;//,flSp3;			//分段调整速率

extern unsigned char set_AddMb,set_AddCa;
extern unsigned char sys_GoodsId;//货位号
extern char OilCode[10];//油品编码,用以作为记录保存的

extern unsigned char set_SpOp;		//开阀速率  :  20%/s  0~99%/s
extern unsigned char set_SpCl;		//关阀速度  :  20%/s  0~99%/s
extern unsigned char set_SpAd;		//关阀速度  :  20%/s  0~99%/s
extern unsigned int  set_FUp;			//开启稳流量: 10Kg/s  0~99999Kg/s
extern unsigned int  set_FDn;			//关闭稳流量:  1Kg/s  0~99999Kg/s
extern unsigned int  set_FMx;			//最大流量  : 40Kg/s  0~99999Kg/s
extern unsigned char set_Ferr;			//流量偏差  : 40Kg/s  0~99Kg/s
extern unsigned int  set_MDn;			//提前量    :200Kg    0~999Kg
extern unsigned int  set_MOv;			//过冲量    : 20Kg  	0~999Kg
extern unsigned char set_TOpBump;	//提前开泵时间:	0			0~99s  
extern unsigned char set_TClBump;	//延迟关泵时间:	0			0~99s
extern unsigned int  set_Tsta;		//开稳流时间:	10S			0~99s
extern unsigned int  set_KNeed;		//修正系数:			    0.9~1.100
extern unsigned int  set_BNeed;		//修正值:				      0~99Kg
extern unsigned int  set_Noise;		//小信号切除
extern unsigned int  set_ClValveTmr;	//关阀超时
extern unsigned int  set_OpValveTmr;	//开阀超时
extern unsigned int  set_FVtmr;			//流量计超时
extern unsigned int  set_Config;			//组态参数
extern float 				flNoise;				//flNoise=0.1*set_Noise;
extern unsigned char conf_MD_LmtUp;
extern unsigned char conf_MD_LmtDn;
extern unsigned char conf_YL_Scale;
extern unsigned char conf_YL_LmtUp;
extern unsigned char conf_YL_LmtDn;
extern unsigned int  conf_FV_LmtDn;//流量下限，专门用于全开阶段低流量限制
extern float Press_On_Lmt;//低压力值,全开时，低于此压力停车
extern float Flow_On_Lmt;//低流量值，全开时，流量低于此值，停车报警
extern float Press_Up_Lmt;
extern float Press_Dn_Lmt;
extern float Md_Up_Lmt;
extern float Md_Dn_Lmt;
extern float Fv_Dn_Lmt;//流量下限，专门用于全开阶段时
extern char NewLstBuf[80];//单个记录
extern char LstFyBuf[80];

extern float YFL[2],SFL[2];
extern char chrLeadId[2][9];//应发量，4位BCD码
extern char chrCarId[2][9];//应发量，4位BCD码
extern char chrYFL[2][9];//应发量存储
extern char chrOilId[2][9];//提油密码存储

extern char lstYFL[2][4];//应发量，4位BCD码
extern char lstSFL[3][4];//实发量，4位BCD码
extern char lstOilId[2][4];//提油密码
extern char lstMD[4]; //介质密度
extern char lstWD[4]; //介质温度
extern char bcdLJS[5];//前表数
extern unsigned char flgPendLst;//当前发油的提油密码在记录中存在，发油结束后需要删除

extern char LogUsrID[5];
extern unsigned char flgWrWarnLst;
extern unsigned long lng_KgNeed;//lng_KgNeed[2];
extern volatile float dSSLv_pls_first;
extern float rdLJS,savLJS;
extern unsigned char CarIdCnt,CarIdOK,LeadIdCnt,LeadIdOK,SavLeadId[4];
//SavConfDat组态参数 0:监控地址,1:管理地址,2:修正系数,3:修正值,4:压力量程,5:压力上限,6:压力下限,7:密度上限,8:密度下限,9:年,10:月,11:日,12:时,13:分,14:秒,15,组态
//日历算法???
extern char  SavConfDat[17];
extern char u8SavConfDat[17];
extern unsigned int  SavConfDatMax[17];
extern unsigned int  SavConfDatMin[17];
extern unsigned int u16SavConfDat;
extern char    SavConf[17][4];
extern char tmpSavConf[17][4];
extern char    u8SavConfBuf[7];
extern char    SavConfDate[2][7]; 		//日期与时间
extern char		tmpSavConfDate[2][7];	//日期与时间
//20150425
extern char idCarTypeId[10];//卡里车型号码
extern char idCarId[10];    //卡里车号[实际卡里没有，在待发确认界面中人工输入]
extern char idCarYFL[10];		//卡里应发量
extern unsigned char LstIndex;	//已发记录界面光标行数
extern unsigned int  ub250ms;
extern unsigned char flgFyEdit;
extern unsigned char flgOpValv;
extern unsigned char flgSFLdis;//实发量刷新,1:强制刷新,0:无作用
extern unsigned int  SavFyDat[18];
extern const unsigned int  SavFyDatMax[18];
extern const unsigned int  SavFyDatMin[18];
extern char tmpSavFy[18][4];
extern char   tSavFyDat[34];
extern unsigned int u16SavFyDat;
extern char tmpSavMaxVol[8];
extern unsigned long SavMaxVol;
extern unsigned char SavMaxVol1,SavMaxVol2,SavMaxVol3;
extern unsigned int      SavManDat[4];
extern unsigned int   SavManDatMax[4];
extern unsigned int   SavManDatMin[4];
extern char tmpSavMan[4][4];
extern char   tSavFyMan[8];
extern unsigned int u16SavManDat;
extern unsigned char EepWrBuf[200];
extern unsigned char EepRdBuf[200];
extern unsigned char flgReNew;//屏幕切换时强制更新;
extern unsigned int PendAdd[10];
extern unsigned char dualChNum,dualChNum_pre;
extern unsigned char flgSysWarn;
extern unsigned char flgPwdEdit;
extern unsigned char WarnOnOff[11];	  //组态参数分拆
extern char bufSFLs[2][8];
extern char disSFLs[2][8];
extern unsigned char flgSFLs[2];
extern unsigned char frmMainCur,flgSdStart,flgSdEnd;
extern char bufSFL[11],disSFL[11];
extern const unsigned int RES_Top_Left_BIN[126];
extern const unsigned int RES_left_up_BIN[126];
extern const unsigned int RES_dn_BIN[126];
extern const unsigned int RES_left_BIN[126];
extern unsigned char rioDat[50],rioDatpre[50],flgRioNew;
extern unsigned char set_AddCa;
extern unsigned char canChNum;
extern unsigned char delChNum,addChNum;
extern iCAN_msg      iCAN_TxMsg;
extern iCAN_msg      iCAN_RxMsg;                         // CAN message for receiving                                
extern iCAN_msg      rtCAN_TxMsg[15];//原先为
extern iCAN_msg      lstCAN_TxMsg[10];
extern iCAN_msg      confCAN_TxMsg[3];//流量计数据
extern iCAN_msg      jobCAN_TxMsg[4];//作业数据
extern unsigned char cntJobCan;
extern unsigned char cntCanSTA;//重要的事情传三遍
extern unsigned char cntLstCan;
extern unsigned char cntConfCan;

extern unsigned char flgAlertDone;
extern unsigned char flgAlertRst;
//unsigned char CarId[2][7],PendCarId[2][7];
extern char PendCarId[9],CarId[9],LeadId[9];
extern unsigned char CarIdCnt,CarIdOK,LeadIdCnt,LeadIdOK,SavLeadId[4];
extern unsigned char flgViceValv;
extern unsigned char DiDatHi,DiDatLo,DiDat;
extern unsigned char DoDat,rtDoDat;
extern unsigned char flgCANerr,flgIFIXerr,flgFVerr[2];
extern unsigned long tmrCAN,tmrIFIX;
extern unsigned long tmrFV[2];
extern unsigned char iFix_id;
extern volatile float can_rtSSL,canSSL[2],canWD[2],canMD[2],canSFL[2],canLJS[2],can_YFL[2];
extern float can_rtWD;
extern float can_rtMD;
extern float can_rtSFL;
extern float can_rtLJS;
extern float can_rtYFL;
extern vu8 CanRxdBuf[100];
extern char tSqlBuf[10];
extern unsigned char CanData[220];
extern unsigned char canRtData[50];
extern unsigned char canJobData[30];
extern unsigned char canLstData[60];//记录数据
extern unsigned char canConfData[21];//组态数据


extern void DisNum24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,char * s);



extern unsigned char ubKey_Command;
extern unsigned int  rtnLos;

//typedef struct  {
//  unsigned char   SrcMacId;           // 29 bit identifier
//	unsigned char   DestMacId;
//	unsigned char   ACK;
//	unsigned char   FuncID;
//	unsigned char   SourceID;
//  unsigned char   data[8];            // Data field
//  unsigned char   len;                // Length of data field in bytes
//  //unsigned char   format;             // 0 - STANDARD, 1- EXTENDED IDENTIFIER
//  //unsigned char  type;               // 0 - DATA FRAME, 1 - REMOTE FRAME
//} iCAN_msg;
typedef struct {
  unsigned char  fvID;		//站号
  unsigned char  fvConn;	//联接标志位
  unsigned char  unitMassFv;//质量流量单位
  unsigned char  unitVolFv;	//容量流量单位
  unsigned char  unitTemp;	//温度单位
  	float  fvMass;	//质量流量
		float  fvLJSm;	//质量总量
		float  fvVol;		//容积流量
		float  fvLJSv;	//容积总量
		float    fvWD;		//
		float    fvMD;		//		
		float    fvZero;	//零点值
} TH_MSG;
typedef struct
{
	unsigned char wlAdd;
	unsigned char wlColor;
	unsigned int  wlTmr;
	unsigned char wlBeep;
} WARN_LED;

//typedef struct {
//	unsiged char 
//} VALV;
extern TH_MSG TH_Msg[2];
extern TH_MSG TH_Msg_Pre[2];
extern unsigned char flgFyEn,frmMainYFcur;
extern unsigned char LstCur;		//已发记录界面光标
extern unsigned char LstCnt;		//有效记录数
extern unsigned char LstPgNum;	//总页数
extern unsigned char LstDisPgNum;//显示的页
extern unsigned char LstPgNumIndex;//页数指针
extern unsigned char LstAllCnt;//总记录数
extern unsigned char LstDisCnt;//显示记录数
extern unsigned char DoDat,rtDoDat,valv_pump;

extern unsigned char u2Flg;
extern unsigned char u2NewTxdFlg;
extern unsigned char u2RxdCnt;
extern unsigned char u2RxdEnd;
extern unsigned char flgFvETX;
extern unsigned char bufMdBuf[14][100];
extern unsigned char u4Buf[100];
extern unsigned char flgPkqDatOk;
extern unsigned int u2Tmr,u3Tmr;
extern unsigned char u2RxdCnt,u2RxdBuf[50],datMod2Buf[200];
extern unsigned char u3RxdCnt,u3RxdBuf[50],datMod3Buf[200];
extern unsigned char flgMbCrcOk,flgMb2CrcOk,flgMb3CrcOk;
extern unsigned int mbFvDat[50];
//extern unsigned int  mbdata[14][50];
extern unsigned int mbDat[100];

extern unsigned char flgU5Stx;
extern unsigned char u5STX;
extern unsigned int  u5Cnt;
extern char WeiDat[7],disWeiDat[7];
extern unsigned long lngWeiDat;

extern unsigned char u4STX,u4Dat,cntKK;
extern unsigned char u1STX,u1Dat;
extern unsigned char flgU4Stx1,flgU4Stx2,flgU4StRecv,flgU4StSend;
extern unsigned char flgU1Stx1,flgU1Stx2,flgU1StRecv,flgU1StSend;

extern unsigned char Txd2Buf[100],Txd3Buf[100];
extern unsigned char u3STX;
extern unsigned int  ftaDIdat,ftaDIdat_pre;
extern unsigned char ftaDOdat,ftaDOdat_pre;
extern unsigned char ftaDi[12],ftaDo[8];
												 
extern unsigned char flgFvMbCrcOk,flgFvMbTmrOut,flgMb2CrcOk,flgMb3CrcOk;
extern unsigned char Rxd1Buf[30],u1MbBuf[100];
extern unsigned char u1Cnt,u1SndCnt,u1Flag;//u2Cnt:????,u2Flag:????
extern unsigned char Rxd2Buf[30],u2MbBuf[100];
extern unsigned char u2Cnt,u2SndCnt,u2Flag;
extern unsigned char Rxd3Buf[30],u3MbBuf[100];
extern unsigned char u3Cnt,u3SndCnt,u3Flag;
extern unsigned char Rxd4Buf[30],u4MbBuf[100];
extern unsigned char u4Cnt,u4SndCnt,u4Flag;
extern unsigned char Rxd6Buf[30],u6MbBuf[100];
extern unsigned char u6Cnt,u6SndCnt,u6Flag;
extern unsigned char u6Buf[100],u6FvBuf[100];

extern unsigned char u1TxCnt,u1RxCnt;
extern unsigned char u1TxDat[10],u1RxDat[100];
extern volatile unsigned char lenMbDat;
extern unsigned char u5RxdBuf[100];
extern unsigned char u6Flg,u6RxdCnt,u6NewTxdFlg;
extern unsigned char u3Flg,u3RxdCnt,u3NewTxdFlg,cntVconnTmr;
extern unsigned char datValv1[10],datValv2[10];
extern unsigned char cntValv,fbValv;//发送的阀体编号,反馈的阀体信号
extern unsigned char valvClosed[2],valvOpened[2];//关回讯，开回讯
extern unsigned char valvOpen[2];//开度
extern unsigned char flgConnSTA[10],flgConnSTA_pre[10];
extern unsigned char rio_DO,rio_AoL1,rio_AoH1,rio_AoL2,rio_AoH2;

//测流速新增管径参数
extern unsigned int 	pipeSavConfDat; //管径缺省为80mm
extern unsigned int  pipeSavConfDatMax; //管径最大为999mm
extern unsigned int  pipeSavConfDatMin;  //管径最小为10mm
extern unsigned int  u16pipeSavConfDat;
extern char    pipeSavConf[4];
extern char tmppipeSavConf[4];
extern char    u8pipeSavConfBuf[4];

//考虑脉冲发油,新增两个参数,密度范围0.5000~3.0000,流量系数范围00.00~99.99
extern unsigned int 	plzSavConfDat[2];
extern unsigned int  plzSavConfDatMax[2];
extern unsigned int  plzSavConfDatMin[2];
extern unsigned int  u16plzSavConfDat;
extern char    plzSavConf[2][6];
extern char tmpplzSavConf[2][6];
extern char    u8plzSavConfBuf[6];

//关阀参数新增5个
extern unsigned char 	dnSavConfDat[5];
extern unsigned char  dnSavConfDatMax[5];
extern unsigned char  dnSavConfDatMin[5];
extern unsigned char  u8dnSavConfDat;
extern char tmpdnSavConf[5][3];
extern char    dnSavConf[5][3];
extern char  u8dnSavConfBuf[5];

//报警与联网组态7:,6:,5:,4:,3:,2:,1:,0:
//unsigned char    ConfWarn[7];
extern unsigned char tmpConfWarn[11];
//参数更新标志，1:更新,0:不更新,
extern unsigned char flgSavEn[17];
extern unsigned char ConfIndex;//0:0~9,1:10~19,1:
extern unsigned char ConfPos;
extern unsigned char flgConfEdit;
extern unsigned char ConfCur;
extern unsigned char AmsIndex;

extern volatile float rtPLZk;
extern volatile unsigned int rtPLerr;//脉冲偏差
extern unsigned char FyIndex,FyPos,flgManEdit;

extern float flAoDat;
extern unsigned int mAoDat,AoDat;
extern unsigned char flgPendEdit;
extern unsigned char PendPos;

extern volatile float dSFL[2],dYFL[2],dSFLpre[2],dSFL0,dSFL1,dSFL2,dYFL1,dYFL2,dSFLpre1,dSFLpre2;
extern volatile float dSSL[2],dSSL0,dSSL1,dSSL2;
extern volatile float dLJS[2],dLJS0,dLJS1,dLJS2;//累积数
extern volatile float dLJV[2],dLJV1,dLJV2;//累积体积
//extern volatile float dMD[2],dMD1,dMD2;
//extern volatile float dWD[2],dWD1,dWD2;
extern volatile float dRtd[2];
extern volatile float dLJS_first[2],dLJS_first1,dLJS_first2;
extern volatile float dLJS_last[2],dLJS_last1,dLJS_last2;
extern volatile float dLJV_first[2],dLJV_first1,dLJV_first2;
extern volatile float dLJV_last[2],dLJV_last1,dLJV_last2;

extern volatile float rtSFL;
extern volatile float rtSSL;
extern volatile float rtSSLv;
extern volatile float rtLJS;//累积数
extern volatile float rtLJV;//累积体积
extern volatile float rtMD;
extern volatile float rtWD;
extern volatile float rtRtd;
extern volatile float rtLJS_first;
extern volatile float rtLJS_last;
extern volatile float rtLJV_first;
extern volatile float rtLJV_last;
extern volatile float rtPLZk;

extern unsigned int  cntFVconnTmr;	 	//流量计通讯超时定时器
extern unsigned int  cntFV1connTmr;	 	//流量计通讯超时定时器
extern unsigned int  cntFV2connTmr;	 	//流量计通讯超时定时器
extern unsigned int  cntFVflow0;			//发油时流量为0超时定时器
extern unsigned int  cntFV1flow0;			//发油时流量为0超时定时器
extern unsigned int  cntFV2flow0;			//发油时流量为0超时定时器
extern unsigned int  cntOpValvTmr;		//开阀回讯超时
extern unsigned int  cntOpValv1Tmr;		//开阀回讯超时
extern unsigned int  cntOpValv2Tmr;		//开阀回讯超时
extern unsigned int  cntClValvTmr;		//关阀回讯超时
extern unsigned int  cntClValv1Tmr;		//关阀回讯超时
extern unsigned int  cntClValv2Tmr;		//关阀回讯超时
extern unsigned int  cntFlowLow;				//低流量超時

extern unsigned char err_PE;
extern unsigned char err_OvFlow;
extern unsigned char err_FlowNo;
extern unsigned char err_Leak;
extern unsigned char err_OvFlow1;
extern unsigned char err_OvFlow2;
extern unsigned char err_Dens;
extern unsigned char err_Press;
extern unsigned char err_FV;
extern unsigned char err_FlowLow; //低流量[液化气发油]
extern unsigned char err_PressLow;//低压力[液化气发油]


extern unsigned char TxdFvMdBuf[20];
extern WARN_LED WarnLedDat;
extern unsigned int WarnAvid;
extern unsigned int cntWarnAvid;
extern unsigned char flgRmCon,flgRmEnd;//远程控制发油，由机泵控制器控制
extern unsigned int AoDat;
extern unsigned int AoDat1,AoDat2;

extern unsigned char flgAlertDone;
extern unsigned char flgAlertRst;

extern unsigned char flgEnd;
extern unsigned int  cntEndTmr;
extern unsigned char cntRfid;
extern unsigned char cntRfid_pre;
extern unsigned char flgAMS;
extern unsigned int  uwCmpAMS;

extern unsigned char flgSysWarn;//系统故障时报警标志,1:声光,2:有光无声
extern unsigned long tYFLdat;
extern unsigned char flg1to5;//上电时关稳流

extern unsigned char flgABvalv;
extern unsigned long tRunCnt1,tRunCnt2;
extern unsigned long tTmr1,tTmr2;//提前开时间和提前关时间，等于秒数*倍率（200）
extern unsigned long tTmrCl,tTmrOp;//开阀故障与关阀故障定时器

extern unsigned char flgValv1,flgValv2,flgFValv;//阀1，阀2，副阀
extern unsigned int uwSp1,uwSp2,uwSp3;	//分段调整速率,第三段为
extern unsigned char dirErr;
extern unsigned int  sys_Cnt,sysTick,tickCnt,refCnt,tickTmrOut;
extern unsigned char flg1S,sys_adj50ms;

/*------SD----Start--------*/
extern unsigned char flgSdCnt;
extern unsigned char flgSdMode;
extern unsigned char flgSdStart;
extern unsigned char flgSdEnd;
extern unsigned char SdAoDat;
extern unsigned int   sAoDat,mAoDat;//手动
/*------SD----End----------*/

extern unsigned char FyIndex;//0:0~9,1:10~19,1:
extern unsigned char FyPos;
extern unsigned char FyCur;
extern unsigned char flgFyEn;
extern unsigned char flgFyAllow,flgFyAllowPre;//
extern unsigned char LastProc,LastProc_pre;
extern unsigned char sysWarnLed[2],sysWarnLed_pre[2];//报警灯
extern unsigned char flgSt;//记录开始脉冲值
extern unsigned char flgKeySt;//
extern unsigned char swVer; //0:AO阀;1:AB阀,2:双路流量计AO阀
extern char    SavFy[18][4];
extern char    u8SavFyBuf[4];
extern unsigned int  bl_LCD;
extern unsigned char cntIC;
extern unsigned char flgIcDat1OK;
extern unsigned char flgU5Stx1;		  //开始接收到第一个0xFF
extern unsigned char flgU5Stx2;			//接收到0xFF后，再接收到0xCC
extern unsigned char flgU5StRecv;		//发送完成，开始接收
extern unsigned char u5RxdCnt;
extern unsigned char flg250ms;
extern unsigned char u5RxdLen,flgAll0;
extern unsigned char icBuf[10];
extern unsigned char frmMainEdit;
extern unsigned char flgEdit;
extern unsigned char ubKey_Command;
extern unsigned char frmMainCurX,frmMainCurY;
extern unsigned char PadCode[3],PadCodePre[3],flgPad;
extern unsigned char KeyCmd;
extern unsigned char LastDisLos;//显示记录界面中保存最后一个显示位，
extern unsigned char cntLstPage;				//当页记录数
extern char ManLstBuf[3][80];//手动上传表单数据缓存,每页3个记录，每个记录

extern unsigned char lst_WarnCnt;
extern unsigned char lst_DoneCnt;
extern unsigned char rtDi;
extern unsigned char KeyCon;
extern unsigned char flgWarnWr;
extern unsigned char flg_bl_LCD;
extern unsigned int WarnID;
extern unsigned int WarnAvid;
extern unsigned int cntWarnAvid;
extern unsigned char flgFvConn,flgFvConn_pre,cntFvConn;
extern unsigned char flgFmConn,flgFmConn_pre,cntFmConn;
extern unsigned char flgCanSTA;//CAN状态，用以回复报文使用
extern unsigned char FvId;
extern unsigned long tmrU6,tmrU5;
extern unsigned long tDatInt,tConInt;
extern unsigned char FvWarnDat[6];
extern unsigned char FvClibDat[4];
extern unsigned char FvSetDat[12];
extern unsigned char FvZeroDat[4];
extern unsigned int  staValv[2];


extern void SendCard(unsigned char tDat);
extern void ShowStr30(unsigned int x,unsigned int y,char *p,unsigned char slen,unsigned int ForeColor,unsigned int BakColor);
extern void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void Draw_Rect(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned int color);
extern void Rfid(void);
extern void lcd_text32(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void Dis_CarId(unsigned char id);
extern void EepRd(unsigned int ReadAddr,unsigned int NumByteToWrite);
extern void EepWr(unsigned int ReadAddr,unsigned int NumByteToWrite);
extern unsigned long calYFL(void);
extern void getWarnOnOff(void);
extern void Dis_GdsId(unsigned char tGdsId);
extern void Dis_Pad(unsigned char tCar);
extern unsigned char ChkUsrCard(void);
extern void RTCHardwareConfig(void);
extern void RTCSetDate(unsigned char tY,unsigned char tM,unsigned char tD);
extern void RTCSetTime(unsigned char tH,unsigned char tM,unsigned char tS);
extern void ConfChr2Dat(void);
extern void Dis_Date(void);
extern void Dis_Time(void);
extern void Wr_New_Lst(unsigned int tIndex);
extern void ClrConfSel(unsigned char x,unsigned char y);
extern void Draw_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen);
extern void Draw_Line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned char bw,unsigned int color );
extern void Draw_Button_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen);
extern float ctof(unsigned char dt1,unsigned char dt2,unsigned char dt3,unsigned char dt4);
extern void re_SFL_float(double tfSFL);
extern void Dis_Pend(unsigned char id);//新待发提单，有车号显示
extern void DisManSel(unsigned char x,unsigned char y,unsigned int tColor);
extern void ClrManSel(unsigned char x,unsigned char y);
extern void WrManSel(unsigned char x,unsigned char y,char *s);
extern void SavUsrPwd(void);

extern u8 CAN1_Send_Msg(u8 rtCnt);
extern void SendFvCmd(unsigned char pvId);
extern void chkFvMsg(void);
extern void Dis_1_Pend(unsigned char IdPend);
extern void Fnd_PendLst(void);
extern u8 CAN1_Send_Job(u8 rtCnt);
extern u8 CAN1_Send_Conf(u8 rtCnt);
extern u8 CAN1_Send_Lst(u8 rtCnt);
extern void FillJobCAN(void);
extern void Key_AMS(void);
extern void Dis_AMS(void);
extern void FillLstData(void);
extern void ftoc(float tta);
extern void Draw_Rect_Color(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned char bw,unsigned int tColor);
extern void TFT_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,u16 color);

#endif

