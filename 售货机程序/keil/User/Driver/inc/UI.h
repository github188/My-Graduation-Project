#ifndef _UI_H_
#define _UI_H_
#pragma diag_suppress 870

#include "stm32f40x.h"
#include "Disp.h"
#include "delay.h"
#include "Key.h"
#include "comm.h"
#include "main.h"

#define AB_Valv     1
#define AO_Valv     0
#define FRM_LOGIN  	1	//????
#define FRM_PWD			2	//????
#define FRM_MAIN		3	//??????
#define FRM_PENDLST	4	//????????
#define FRM_DONELST	5	//????????
#define FRM_WARN		6	//????????
#define FRM_SETTING	7	//????????
#define FRM_MAN			8	//????????
#define FRM_SD			9	//????????
#define FRM_CONFIG  10//????????
#define FRM_AMS  		11//AMS????
#define FRM_MON			12//??????

#define FRM_YZA_MAIN 20		//???
#define FRM_TJR_SET  21		//????
#define FRM_TJR_LST  22		//????
#define FRM_TJR_PWR  23		//????????
#define FRM_TJR_BTN  24		//??????
#define FRM_TJR_SYS  25		//????
#define FRM_TJR_QX1  26		//????
#define FRM_TJR_QX2  27		//????
#define FRM_TJR_JOB  28   //????
#define FRM_TJR_TEST 29   //????

#define P0_TEXT1_LEFT		160
#define P0_TEXT1_TOP		100
#define P0_TEXT1_WIDTH	220
#define P0_TEXT1_HEIGH	65

#define P0_TEXT2_LEFT		510
#define P0_TEXT2_TOP		100
#define P0_TEXT2_WIDTH	220
#define P0_TEXT2_HEIGH	65
 
//#define KEY_U_ARROW			0x11 //
//#define KEY_D_ARROW			0x12
//#define KEY_L_ARROW			0x13
//#define KEY_R_ARROW			0x14
//#define KEY_U_ARROW			0x14
//#define KEY_D_ARROW			0x11
//#define KEY_L_ARROW			0x13
//#define KEY_R_ARROW			0x12
//#define KEY_SET					0x21
//#define KEY_ENTER				0x22
//#define KEY_END					0x23
//#define KEY_DEL					0x24
//#define KEY_RTN					0x25
//#define KEY_START1			0x31
//#define KEY_STOP1				0x32
//#define KEY_START2			0x33
//#define KEY_STOP2				0x34
 

extern unsigned char sys_Add;
extern void draw_circle(unsigned int x0,unsigned int y0,unsigned int r,unsigned int tColor);
extern void Draw_Beep(unsigned int x0,unsigned int y0,unsigned int r,unsigned char tBeep,unsigned char tWarn);


extern void LCD_Init(void);
extern void Dis_Pwd(void);
extern void Dis_Set(void);
extern void Dis_Conf(void);
extern void Dis_Fy(void);

extern unsigned char FyIndex;//0:0~9,1:10~19,1:
extern unsigned char FyPos;
extern unsigned char ConfIndex;//0:0~9,1:10~19,1:
extern unsigned char ConfPos;

extern unsigned char UsrPos;
extern unsigned char PwdPos;
extern unsigned char IndexId;//0:????????,1:????,2:????,3:????
extern void DisPwdSel(unsigned char x,unsigned char y,unsigned int tColor);

extern unsigned char WarnOnOff[11];	  //????????????



void Draw_Cur(unsigned int x,unsigned int y,unsigned char len,unsigned char en,unsigned char OnOff);//x,y,????????,????
void Draw_Line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned char bw,unsigned int color );
void Draw_Sel(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned int tColor);
void Draw_Rect(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned char bw);
void Sel_Fill_Button(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight);
void Sel_Button(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight);
void Draw_Fill_Button(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight);
void Draw_Button(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight);
void Draw_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen);
void Draw_Button_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen);
void Del_TextBox(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight);
void Draw_TextBox(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight);
void Draw_Btn(char *p);
void Sel_Btn(char *p);
void Frm_Init(void);
void Draw_unRead(unsigned int x,unsigned int y);
void Draw_Readed(unsigned int x,unsigned int y);
void Draw_Warn(unsigned int x,unsigned int y,unsigned int tColor);
void Draw_DigNum(unsigned int x,unsigned int y);
void Draw_TimBar(unsigned int tvalue);
void Draw_AoBar(unsigned int tvalue);
void Dis_Lcd(void);
void Dis_Scr(unsigned char pgNum);
void Frm_Fy(unsigned char ubVer);
void Dis_Date(void);
void Disp_SFL(void);


extern unsigned int  sys_SavOvKg;	//??????????

extern volatile float rtSFL;
extern unsigned char flgSFLdis;
extern unsigned char frmMainCur;

extern unsigned int  SavFyDat[18];

extern char logUsr[2][7];
extern char tmplogUsr[2][7];

extern unsigned char flgSdMode;
extern unsigned char flgAMS,staMdPv,staMdSet;
extern unsigned int  flgSetGet;//????????????????????
extern unsigned char flgPendEdit,flgConfEdit,flgFyEdit,flgPwdEdit,frmMainEdit;

extern char frmMainOrdId[9],frmMainYFval[9];
extern unsigned char flgDiffAMS,UsrAvid;

extern unsigned char LstCur;		//????????????????
extern unsigned char LstDisPgNum;//????????
extern unsigned char flgSdStart;
extern unsigned char flgSdEnd;
extern unsigned char flgManCon;
extern unsigned char ManIndex,ManCur,ManPos;
extern unsigned char AmsIndex;
extern unsigned char flgFyAllow,flgFyAllowPre;
#endif 
