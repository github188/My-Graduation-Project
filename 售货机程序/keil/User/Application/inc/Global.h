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

//�˼��̽����Ǵ�����Դʹ�ã�
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
#define FRM_LOGIN  	1	//��½
#define FRM_PWD			2	//����
#define FRM_MAIN		3	//������
#define FRM_PENDLST	4	//������¼
#define FRM_DONELST	5	//�ѷ���¼
#define FRM_WARN		6	//������¼
#define FRM_SETTING	7	//�����趨
#define FRM_MAN			8	//���Խ���
#define FRM_SD			9	//�ֶ�����
#define FRM_CONFIG  10//��̬����
#define FRM_AMS  		11//AMS����
#define FRM_MON			12//˫·��ؽ���
#define FRM_PENDSEL	13//����ȷ�Ͻ���
#define FRM_CALC    14//���������

#define TmrRatio 20
//#define FRM_LOGIN  	21	//��½
//#define FRM_PWD			22	//����
//#define FRM_MAIN		23	//������
//#define FRM_PENDLST	24	//������¼
//#define FRM_DONELST	25	//�ѷ���¼
//#define FRM_WARN		26	//������¼
//#define FRM_DETAIL	26	//������¼
//#define FRM_SETTING	27	//�����趨
//#define FRM_MAN			28	//���Խ���
//#define FRM_SD			29	//�ֶ�����
//#define FRM_CONFIG  30//��̬����
//#define FRM_MON     31//��̬����

extern unsigned char KeyCode,KeyCode_new,KeyCnt,KeyCnt_free,KeyPress,KeyRelease;
extern unsigned char KeyCommand;
extern unsigned char ManIndex,ManCur,ManPos;
extern unsigned char ManDoDat;
extern unsigned int  ManAoDat;
extern unsigned char flgManCon;
extern unsigned char flgFbFY;//��Ҫ�������ݸ�FY
extern unsigned char sysTxdCnt;
extern unsigned char flgWrOrdId;
extern unsigned char sysInfo;
extern unsigned char sysOldInfo;//Fy����һ״̬
extern unsigned char flgConn;
extern unsigned char frmNum;
extern unsigned char flgAMT;//���Զ���־
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

extern unsigned char sys_STA;		//0:����,1:����,2:����,3:������,4:ȫ��,5:������,6:���ͽ���,7:������ͣ.
extern unsigned char sys_Conn;		//0:����,1:Զ��
extern unsigned char sys_ConnLast;

extern unsigned char UsrAvid;//
extern unsigned char UsrAvid_pre;

extern unsigned char flgCur;
extern unsigned char flgPendLst;//��ǰ���͵����������ڼ�¼�д��ڣ����ͽ�������Ҫɾ��
extern unsigned char u8PendNum[2],u8PendNum_pre[2];
extern unsigned char Fy2CanLst[0x64];//��λ����¼����
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
//char nowPendLst[9];		//��ǰ���͵���������
extern char nowPendLst[2][9];		//��ǰ���͵���������
extern volatile float canSFLv[2];
extern volatile float dSSLv,dSSLm,dLJSv,dLJSm,dYFLv,dSFLv,dSFLm,dLJSv_first,dLJSm_first,dLJSv_last,dLJSm_last,dMD,dWD;
extern volatile float dSSLv_pre,dSSLm_pre,dLJSv_pre,dLJSm_pre,dYFLv_pre,dSFLv_pre,dSFLm_pre,dMD_pre,dWD_pre;
extern volatile float dSSLv_pls,rtSSLv_pls;

extern char FlashWarnBuf[256];
extern unsigned char SavOilId[15];	//��CAN���ߵõ��˷��ͱ���
extern unsigned char savY,savM,savD,savHH,savMM,savSS;
extern unsigned char sysY;
extern unsigned char sysM;
extern unsigned char sysD;
extern unsigned char sysHH;
extern unsigned char sysMM;
extern unsigned char sysSS;
extern unsigned char flgLstTFTok;//TFT�������

extern unsigned char flgDisDate,flgDisTime;
extern unsigned char FyDateSt[6],FyDateSp[6];
extern unsigned char UsrPos;
extern unsigned char PwdPos;
extern unsigned char IndexId;//0:��������,1:����,2:����,3:�˳�
extern unsigned char PwdCur;
extern unsigned char flgUsrSp;//�û���ͣ��־
//������¼:0:����,1:����,2:������,3:������,4:й©,5:�ܶ�,6:�û�
extern unsigned char flgAlert[11];			
extern unsigned char flgAlertPre[11];
extern unsigned int  flgAlertCnt[11];
extern unsigned char flgAlertTmr[11];	
extern unsigned int  tmpSavConfWarn;	
extern unsigned char cntLstTFT;//TFT�м�¼����
extern unsigned char flgCanFb;
extern unsigned char cntCanFb;
extern unsigned char canRtCnt;//ʵʱ����
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
extern unsigned char sys_STA_Last;		//0:����,1:����,2:����,3:������,4:ȫ��,5:������,6:���ͽ���,7:������ͣ.
extern unsigned char flgWrRtc;
extern unsigned char NewPendBuf[30];
extern unsigned char PendBuf[10][25];
extern char NewPendAscBuf[9];//ÿ�յ�һ��������¼����������¼��ֳ�ASC��ʽ���������Ƚϣ����ô�����¼��֤ͨ����û���ɾ��
extern char DelPendBuf[10];
extern unsigned char SavOilId[15];	//��CAN���ߵõ��˷��ͱ���
extern unsigned char flgMsgErr;
extern char PendCarId[9],CarId[9],LeadId[9];

extern float f_SSL_up,f_SSL_dn,f_SSL;
extern float f_Dn_Sp1,f_Dn_Sp2;//�ط�����1,2
extern float f_Seg1,f_Seg2;
extern float f_ExDn;//���ȳ�ǰ
extern unsigned char flgSTA3to4;//״̬��3�ı䵽4
extern unsigned char flgSTA4to5;//״̬��4�ı䵽5
extern unsigned long  cntABta_on;
extern unsigned long  cntABtadj;
extern unsigned long  cntABton;
extern unsigned long  cntABtall;
extern unsigned int set_Ta_on;		  //AB�����ڿ��
extern unsigned int set_Tadj;				//AB����������
extern unsigned int set_Ton; 				//AB��������
extern unsigned int set_Tall;				//AB����������
extern unsigned int set_Ta_on;				//AB����������
extern float flSeg1,flSeg2,flSeg3;			//�ֶ�����
extern float  flSp1, flSp2;//,flSp3;			//�ֶε�������

extern unsigned char set_AddMb,set_AddCa;
extern unsigned char sys_GoodsId;//��λ��
extern char OilCode[10];//��Ʒ����,������Ϊ��¼�����

extern unsigned char set_SpOp;		//��������  :  20%/s  0~99%/s
extern unsigned char set_SpCl;		//�ط��ٶ�  :  20%/s  0~99%/s
extern unsigned char set_SpAd;		//�ط��ٶ�  :  20%/s  0~99%/s
extern unsigned int  set_FUp;			//����������: 10Kg/s  0~99999Kg/s
extern unsigned int  set_FDn;			//�ر�������:  1Kg/s  0~99999Kg/s
extern unsigned int  set_FMx;			//�������  : 40Kg/s  0~99999Kg/s
extern unsigned char set_Ferr;			//����ƫ��  : 40Kg/s  0~99Kg/s
extern unsigned int  set_MDn;			//��ǰ��    :200Kg    0~999Kg
extern unsigned int  set_MOv;			//������    : 20Kg  	0~999Kg
extern unsigned char set_TOpBump;	//��ǰ����ʱ��:	0			0~99s  
extern unsigned char set_TClBump;	//�ӳٹر�ʱ��:	0			0~99s
extern unsigned int  set_Tsta;		//������ʱ��:	10S			0~99s
extern unsigned int  set_KNeed;		//����ϵ��:			    0.9~1.100
extern unsigned int  set_BNeed;		//����ֵ:				      0~99Kg
extern unsigned int  set_Noise;		//С�ź��г�
extern unsigned int  set_ClValveTmr;	//�ط���ʱ
extern unsigned int  set_OpValveTmr;	//������ʱ
extern unsigned int  set_FVtmr;			//�����Ƴ�ʱ
extern unsigned int  set_Config;			//��̬����
extern float 				flNoise;				//flNoise=0.1*set_Noise;
extern unsigned char conf_MD_LmtUp;
extern unsigned char conf_MD_LmtDn;
extern unsigned char conf_YL_Scale;
extern unsigned char conf_YL_LmtUp;
extern unsigned char conf_YL_LmtDn;
extern unsigned int  conf_FV_LmtDn;//�������ޣ�ר������ȫ���׶ε���������
extern float Press_On_Lmt;//��ѹ��ֵ,ȫ��ʱ�����ڴ�ѹ��ͣ��
extern float Flow_On_Lmt;//������ֵ��ȫ��ʱ���������ڴ�ֵ��ͣ������
extern float Press_Up_Lmt;
extern float Press_Dn_Lmt;
extern float Md_Up_Lmt;
extern float Md_Dn_Lmt;
extern float Fv_Dn_Lmt;//�������ޣ�ר������ȫ���׶�ʱ
extern char NewLstBuf[80];//������¼
extern char LstFyBuf[80];

extern float YFL[2],SFL[2];
extern char chrLeadId[2][9];//Ӧ������4λBCD��
extern char chrCarId[2][9];//Ӧ������4λBCD��
extern char chrYFL[2][9];//Ӧ�����洢
extern char chrOilId[2][9];//��������洢

extern char lstYFL[2][4];//Ӧ������4λBCD��
extern char lstSFL[3][4];//ʵ������4λBCD��
extern char lstOilId[2][4];//��������
extern char lstMD[4]; //�����ܶ�
extern char lstWD[4]; //�����¶�
extern char bcdLJS[5];//ǰ����
extern unsigned char flgPendLst;//��ǰ���͵����������ڼ�¼�д��ڣ����ͽ�������Ҫɾ��

extern char LogUsrID[5];
extern unsigned char flgWrWarnLst;
extern unsigned long lng_KgNeed;//lng_KgNeed[2];
extern volatile float dSSLv_pls_first;
extern float rdLJS,savLJS;
extern unsigned char CarIdCnt,CarIdOK,LeadIdCnt,LeadIdOK,SavLeadId[4];
//SavConfDat��̬���� 0:��ص�ַ,1:�����ַ,2:����ϵ��,3:����ֵ,4:ѹ������,5:ѹ������,6:ѹ������,7:�ܶ�����,8:�ܶ�����,9:��,10:��,11:��,12:ʱ,13:��,14:��,15,��̬
//�����㷨???
extern char  SavConfDat[17];
extern char u8SavConfDat[17];
extern unsigned int  SavConfDatMax[17];
extern unsigned int  SavConfDatMin[17];
extern unsigned int u16SavConfDat;
extern char    SavConf[17][4];
extern char tmpSavConf[17][4];
extern char    u8SavConfBuf[7];
extern char    SavConfDate[2][7]; 		//������ʱ��
extern char		tmpSavConfDate[2][7];	//������ʱ��
//20150425
extern char idCarTypeId[10];//���ﳵ�ͺ���
extern char idCarId[10];    //���ﳵ��[ʵ�ʿ���û�У��ڴ���ȷ�Ͻ������˹�����]
extern char idCarYFL[10];		//����Ӧ����
extern unsigned char LstIndex;	//�ѷ���¼����������
extern unsigned int  ub250ms;
extern unsigned char flgFyEdit;
extern unsigned char flgOpValv;
extern unsigned char flgSFLdis;//ʵ����ˢ��,1:ǿ��ˢ��,0:������
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
extern unsigned char flgReNew;//��Ļ�л�ʱǿ�Ƹ���;
extern unsigned int PendAdd[10];
extern unsigned char dualChNum,dualChNum_pre;
extern unsigned char flgSysWarn;
extern unsigned char flgPwdEdit;
extern unsigned char WarnOnOff[11];	  //��̬�����ֲ�
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
extern iCAN_msg      rtCAN_TxMsg[15];//ԭ��Ϊ
extern iCAN_msg      lstCAN_TxMsg[10];
extern iCAN_msg      confCAN_TxMsg[3];//����������
extern iCAN_msg      jobCAN_TxMsg[4];//��ҵ����
extern unsigned char cntJobCan;
extern unsigned char cntCanSTA;//��Ҫ�����鴫����
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
extern unsigned char canLstData[60];//��¼����
extern unsigned char canConfData[21];//��̬����


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
  unsigned char  fvID;		//վ��
  unsigned char  fvConn;	//���ӱ�־λ
  unsigned char  unitMassFv;//����������λ
  unsigned char  unitVolFv;	//����������λ
  unsigned char  unitTemp;	//�¶ȵ�λ
  	float  fvMass;	//��������
		float  fvLJSm;	//��������
		float  fvVol;		//�ݻ�����
		float  fvLJSv;	//�ݻ�����
		float    fvWD;		//
		float    fvMD;		//		
		float    fvZero;	//���ֵ
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
extern unsigned char LstCur;		//�ѷ���¼������
extern unsigned char LstCnt;		//��Ч��¼��
extern unsigned char LstPgNum;	//��ҳ��
extern unsigned char LstDisPgNum;//��ʾ��ҳ
extern unsigned char LstPgNumIndex;//ҳ��ָ��
extern unsigned char LstAllCnt;//�ܼ�¼��
extern unsigned char LstDisCnt;//��ʾ��¼��
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
extern unsigned char cntValv,fbValv;//���͵ķ�����,�����ķ����ź�
extern unsigned char valvClosed[2],valvOpened[2];//�ػ�Ѷ������Ѷ
extern unsigned char valvOpen[2];//����
extern unsigned char flgConnSTA[10],flgConnSTA_pre[10];
extern unsigned char rio_DO,rio_AoL1,rio_AoH1,rio_AoL2,rio_AoH2;

//�����������ܾ�����
extern unsigned int 	pipeSavConfDat; //�ܾ�ȱʡΪ80mm
extern unsigned int  pipeSavConfDatMax; //�ܾ����Ϊ999mm
extern unsigned int  pipeSavConfDatMin;  //�ܾ���СΪ10mm
extern unsigned int  u16pipeSavConfDat;
extern char    pipeSavConf[4];
extern char tmppipeSavConf[4];
extern char    u8pipeSavConfBuf[4];

//�������巢��,������������,�ܶȷ�Χ0.5000~3.0000,����ϵ����Χ00.00~99.99
extern unsigned int 	plzSavConfDat[2];
extern unsigned int  plzSavConfDatMax[2];
extern unsigned int  plzSavConfDatMin[2];
extern unsigned int  u16plzSavConfDat;
extern char    plzSavConf[2][6];
extern char tmpplzSavConf[2][6];
extern char    u8plzSavConfBuf[6];

//�ط���������5��
extern unsigned char 	dnSavConfDat[5];
extern unsigned char  dnSavConfDatMax[5];
extern unsigned char  dnSavConfDatMin[5];
extern unsigned char  u8dnSavConfDat;
extern char tmpdnSavConf[5][3];
extern char    dnSavConf[5][3];
extern char  u8dnSavConfBuf[5];

//������������̬7:,6:,5:,4:,3:,2:,1:,0:
//unsigned char    ConfWarn[7];
extern unsigned char tmpConfWarn[11];
//�������±�־��1:����,0:������,
extern unsigned char flgSavEn[17];
extern unsigned char ConfIndex;//0:0~9,1:10~19,1:
extern unsigned char ConfPos;
extern unsigned char flgConfEdit;
extern unsigned char ConfCur;
extern unsigned char AmsIndex;

extern volatile float rtPLZk;
extern volatile unsigned int rtPLerr;//����ƫ��
extern unsigned char FyIndex,FyPos,flgManEdit;

extern float flAoDat;
extern unsigned int mAoDat,AoDat;
extern unsigned char flgPendEdit;
extern unsigned char PendPos;

extern volatile float dSFL[2],dYFL[2],dSFLpre[2],dSFL0,dSFL1,dSFL2,dYFL1,dYFL2,dSFLpre1,dSFLpre2;
extern volatile float dSSL[2],dSSL0,dSSL1,dSSL2;
extern volatile float dLJS[2],dLJS0,dLJS1,dLJS2;//�ۻ���
extern volatile float dLJV[2],dLJV1,dLJV2;//�ۻ����
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
extern volatile float rtLJS;//�ۻ���
extern volatile float rtLJV;//�ۻ����
extern volatile float rtMD;
extern volatile float rtWD;
extern volatile float rtRtd;
extern volatile float rtLJS_first;
extern volatile float rtLJS_last;
extern volatile float rtLJV_first;
extern volatile float rtLJV_last;
extern volatile float rtPLZk;

extern unsigned int  cntFVconnTmr;	 	//������ͨѶ��ʱ��ʱ��
extern unsigned int  cntFV1connTmr;	 	//������ͨѶ��ʱ��ʱ��
extern unsigned int  cntFV2connTmr;	 	//������ͨѶ��ʱ��ʱ��
extern unsigned int  cntFVflow0;			//����ʱ����Ϊ0��ʱ��ʱ��
extern unsigned int  cntFV1flow0;			//����ʱ����Ϊ0��ʱ��ʱ��
extern unsigned int  cntFV2flow0;			//����ʱ����Ϊ0��ʱ��ʱ��
extern unsigned int  cntOpValvTmr;		//������Ѷ��ʱ
extern unsigned int  cntOpValv1Tmr;		//������Ѷ��ʱ
extern unsigned int  cntOpValv2Tmr;		//������Ѷ��ʱ
extern unsigned int  cntClValvTmr;		//�ط���Ѷ��ʱ
extern unsigned int  cntClValv1Tmr;		//�ط���Ѷ��ʱ
extern unsigned int  cntClValv2Tmr;		//�ط���Ѷ��ʱ
extern unsigned int  cntFlowLow;				//���������r

extern unsigned char err_PE;
extern unsigned char err_OvFlow;
extern unsigned char err_FlowNo;
extern unsigned char err_Leak;
extern unsigned char err_OvFlow1;
extern unsigned char err_OvFlow2;
extern unsigned char err_Dens;
extern unsigned char err_Press;
extern unsigned char err_FV;
extern unsigned char err_FlowLow; //������[Һ��������]
extern unsigned char err_PressLow;//��ѹ��[Һ��������]


extern unsigned char TxdFvMdBuf[20];
extern WARN_LED WarnLedDat;
extern unsigned int WarnAvid;
extern unsigned int cntWarnAvid;
extern unsigned char flgRmCon,flgRmEnd;//Զ�̿��Ʒ��ͣ��ɻ��ÿ���������
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

extern unsigned char flgSysWarn;//ϵͳ����ʱ������־,1:����,2:�й�����
extern unsigned long tYFLdat;
extern unsigned char flg1to5;//�ϵ�ʱ������

extern unsigned char flgABvalv;
extern unsigned long tRunCnt1,tRunCnt2;
extern unsigned long tTmr1,tTmr2;//��ǰ��ʱ�����ǰ��ʱ�䣬��������*���ʣ�200��
extern unsigned long tTmrCl,tTmrOp;//����������ط����϶�ʱ��

extern unsigned char flgValv1,flgValv2,flgFValv;//��1����2������
extern unsigned int uwSp1,uwSp2,uwSp3;	//�ֶε�������,������Ϊ
extern unsigned char dirErr;
extern unsigned int  sys_Cnt,sysTick,tickCnt,refCnt,tickTmrOut;
extern unsigned char flg1S,sys_adj50ms;

/*------SD----Start--------*/
extern unsigned char flgSdCnt;
extern unsigned char flgSdMode;
extern unsigned char flgSdStart;
extern unsigned char flgSdEnd;
extern unsigned char SdAoDat;
extern unsigned int   sAoDat,mAoDat;//�ֶ�
/*------SD----End----------*/

extern unsigned char FyIndex;//0:0~9,1:10~19,1:
extern unsigned char FyPos;
extern unsigned char FyCur;
extern unsigned char flgFyEn;
extern unsigned char flgFyAllow,flgFyAllowPre;//
extern unsigned char LastProc,LastProc_pre;
extern unsigned char sysWarnLed[2],sysWarnLed_pre[2];//������
extern unsigned char flgSt;//��¼��ʼ����ֵ
extern unsigned char flgKeySt;//
extern unsigned char swVer; //0:AO��;1:AB��,2:˫·������AO��
extern char    SavFy[18][4];
extern char    u8SavFyBuf[4];
extern unsigned int  bl_LCD;
extern unsigned char cntIC;
extern unsigned char flgIcDat1OK;
extern unsigned char flgU5Stx1;		  //��ʼ���յ���һ��0xFF
extern unsigned char flgU5Stx2;			//���յ�0xFF���ٽ��յ�0xCC
extern unsigned char flgU5StRecv;		//������ɣ���ʼ����
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
extern unsigned char LastDisLos;//��ʾ��¼�����б������һ����ʾλ��
extern unsigned char cntLstPage;				//��ҳ��¼��
extern char ManLstBuf[3][80];//�ֶ��ϴ������ݻ���,ÿҳ3����¼��ÿ����¼

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
extern unsigned char flgCanSTA;//CAN״̬�����Իظ�����ʹ��
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
extern void Dis_Pend(unsigned char id);//�´����ᵥ���г�����ʾ
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

