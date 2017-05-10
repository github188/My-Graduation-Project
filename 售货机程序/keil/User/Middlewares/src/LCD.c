#include "LCD.h"
#include "RA8875.h"
#include "Global.h"

#pragma diag_suppress 870

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


//unsigned char frmNum=1;
//unsigned char flgCur;//屏幕切换时强制更新;
extern void reNewSetDat(void);
extern void ReadUsrPwd(void);
extern void EepWr(unsigned int ReadAddr,unsigned int NumByteToWrite);
void Draw_Btn(char *p);
void re_monSFL(unsigned char id, double tfSFL);
void Dis_Pend(unsigned char id);//新待发提单，有车号显示
void Dis_Fy(void);
void WrSetSel(unsigned char x,unsigned char y,char *s);

extern unsigned char LastDisLos;
extern void re_LLXS(float tfSSL);
extern volatile float rtPLZk;
unsigned char disSlaveId;//子站显示ID号
extern void Dis_Detail(unsigned char tId);
void reDrawCell(void);
extern void EepRd(unsigned int ReadAddr,unsigned int NumByteToWrite);
void Draw_EN24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
void Draw_EN32(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
void DisEN32(uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,char * zh);
void Draw_AoBar(unsigned int tvalue);
void Draw_Beep(unsigned int x0,unsigned int y0,unsigned int r,unsigned char tBeep,unsigned char tWarn);
void DrawBigPoint(unsigned int x,unsigned int y,unsigned int tColor);
void Draw_Readed(unsigned int x,unsigned int y);
void Draw_unRead(unsigned int x,unsigned int y);
void re_SFL_float(double tfSFL);
void DisDigNum(unsigned int x,unsigned int y,unsigned char tNum);
void TFT_DrawPoint(unsigned int x,unsigned int y,unsigned int fontcolor);
void re_SSL(unsigned char id,float tfSSL);
void lcd_en48(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
void SelPendSel(unsigned char x,unsigned char y,unsigned int color);
void DisConfSel(unsigned char x,unsigned char y,unsigned int tColor);
void ClrConfSel(unsigned char x,unsigned char y);

unsigned char pollSpan=20;	//轮询间隔
unsigned char retryCnt=2;	//重试次数
unsigned char PlcDelay=16; //设置延时时间
char tmpPollSpan[3][2];	//暂存以上三个变量
//char tmpRetry[4];
char tmpDateTime[2][6];
char  SavDateTmrDat[6];
unsigned long sysTmrOut=1000;//sysTmrOut=pollSpan*100;
//char    SavFy[16][4];
//char    u8SavFyBuf[4];

extern void EepRd(unsigned int ReadAddr,unsigned int NumByteToWrite);

void Draw_Sel(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned int tColor);
void Draw_Frame(u8 x,u8 y,u16 color);
void Draw_Pkq_STA(unsigned char id,unsigned char sta);
void LcdDrawTriangle(uint16_t P1_X,uint16_t P1_Y,uint16_t P2_X,uint16_t P2_Y,uint16_t P3_X,uint16_t P3_Y,  uint16_t tColor, bool tIsfill);
void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
void LcdDrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t tColor, bool tIsfill);
void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
void lcd_text32(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);

extern float disFvDat[6];
void Dis_Time(void)
{
	char tTimeBuf[4];
	if(flgDisTime)
	{
		flgDisTime=0;sysHH_pre=99;sysMM_pre=99;sysSS_pre=99;
	}
	if(sysHH!=sysHH_pre)
	{
		flgDisTime=0;
		tTimeBuf[0]='0'+(sysHH/10);//565
		tTimeBuf[1]='0'+(sysHH%10);//565+16
		tTimeBuf[2]=':';
		tTimeBuf[3]=0;
		//ShowStr30(565+55,0,tTimeBuf,3,BLACK,GOLD);
		//lcd_text24(680,4,BLUE,GOLD,tTimeBuf);
		DisNum24(680,0,BLUE,GOLD,tTimeBuf);
		sysHH_pre=sysHH;
	}
	if(sysMM!=sysMM_pre)
	{tTimeBuf[0]='0'+(sysMM/10);//565
		tTimeBuf[1]='0'+(sysMM%10);//565+
		tTimeBuf[2]=':';
		tTimeBuf[3]=0;
		//ShowStr30(565+55+40,0,tTimeBuf,3,BLACK,GOLD);
		//lcd_text24(680+36,4,BLUE,GOLD,tTimeBuf);
		DisNum24(680+42,0,BLUE,GOLD,tTimeBuf);
		sysMM_pre=sysMM;
	}
	if(sysSS!=sysSS_pre)
	{
		tTimeBuf[0]='0'+(sysSS/10);//565
		tTimeBuf[1]='0'+(sysSS%10);//565+
		tTimeBuf[2]=0;
		//tTimeBuf[3]=0;
		//ShowStr30(565+55+80,0,tTimeBuf,2,BLACK,GOLD);
		//lcd_text24(680+72,4,BLUE,GOLD,tTimeBuf);
		DisNum24(680+84,0,BLUE,GOLD,tTimeBuf);
		sysSS_pre=sysSS;
	}
}

void Dis_Date(void)
{
	char tDateBuf[11];
 	if((flgDisDate)||(sysY_pre!=sysY)||(sysM_pre!=sysM)||(sysD_pre!=sysD))
	{
		flgDisDate=0;
		tDateBuf[0]='2';
		tDateBuf[1]='0';
		tDateBuf[2]='0'+(sysY/10);
		tDateBuf[3]='0'+(sysY%10);
		tDateBuf[4]='-';
		tDateBuf[5]='0'+(sysM/10);
		tDateBuf[6]='0'+(sysM%10);
		tDateBuf[7]='-';
		tDateBuf[8]='0'+(sysD/10);
		tDateBuf[9]='0'+(sysD%10);
		tDateBuf[10]=0;
		//if((sysD_pre!=1)&&(sysD==1))//每月1号清除累积数
		//{
		//flgClrLJS=1;
		//}
		sysY_pre=sysY;sysM_pre=sysM;sysD_pre=sysD;
		//ShowStr30(365+64,0,tDateBuf,10,BLACK,GOLD);
		//lcd_text24(550,4,BLUE,GOLD,tDateBuf);
		DisNum24(530,0,BLUE,GOLD,tDateBuf);

	}
}


void DisPendSel(unsigned char y,unsigned int tColor)
{
	if(y==0)
	{
		Draw_Sel(720-5,56-5,60+10,18+10,tColor);
	  	//Draw_Button_Cap( 700,116+ti*30,60,18,"删除",2);
	}
	else
	{
		Draw_Sel(700-5,86+y*30-5,60+10,18+10,tColor);
	}

}


/*
ClrManSel(SetIndex,SetPos,Color)
只对x=2，第三列的输入框有效。
*/
void ClrManSel(unsigned char x,unsigned char y)
{
	if(x==2)//组态参数
	{
		TFT_Fill(460+2,72+2+y*40,460+40,72+2+y*40+16,WHITE);
	}
}
/*
WrSetSel(SetIndex,SetPos,Color)
发油界面，填入数据
*/
void WrManSel(unsigned char x,unsigned char y,char *s)
{
	if(x==2)
	{
		TFT_Fill(460+2,72+2+y*40,460+40,72+2+y*40+16,WHITE);
		lcd_text16(460+17,75+y*40,0,0,s);	 //17=(42-8)/2
	}
}					 

void DisManSel(unsigned char x,unsigned char y,unsigned int tColor)
{
  if(x==0)//DO列
	{Draw_Sel(122-4,72-4+y*40,50+8,22+8,tColor);	}
  else if(x==1)//AO列
	{Draw_Sel(302-4,72-4,60+8,22+8,tColor);	}
  else if(x==2)//自动调整
	{Draw_Sel(460-4,72-4+y*40,50+8,22+8,tColor);	}
  else if(x==3)//返回
	{Draw_Sel(710-4,72-4+y*30,60+8,22+8,tColor);	}
}

/*
u8SavSetBuf[4]-->u16SavSetDat
仪表参数设置界面,由ASCII码转变成数据
*/

void ConfChr2Dat(void)
{
	unsigned char tii;
	unsigned char tBuf[3];
	//原来保存的数据
	if(u8SavConfBuf[0]!=' ')
	{
		if(u8SavConfBuf[1]==' ')
		{
			u8SavConfBuf[2]=u8SavConfBuf[0];
			u8SavConfBuf[0]='0';
			u8SavConfBuf[1]='0';
		}
		else if(u8SavConfBuf[2]==' ')
		{
			u8SavConfBuf[2]=u8SavConfBuf[1];
			u8SavConfBuf[1]=u8SavConfBuf[0];
			u8SavConfBuf[0]='0';
		}
	}
	for(tii=0;tii<3;tii++)
	{
		tBuf[tii]=u8SavConfBuf[tii];
		if((tBuf[tii]>'9')||(tBuf[tii]<'0'))
		{
		  tBuf[tii]='0';
		}
		tBuf[tii]-='0';
	}
	u16SavConfDat=tBuf[0]*100;
	u16SavConfDat+=tBuf[1]*10;
	u16SavConfDat+=tBuf[2];
}

void ConfChr2DatMd(void)
{
	unsigned char ti,tj;
	unsigned char tBuf[5];
//01234
	for(ti=4;ti>0;ti--)
	{
		if((u8plzSavConfBuf[4]==' ')||(u8plzSavConfBuf[4]==0))
		{
			for(tj=4;tj>(4-ti);tj--)
			{
			 	u8plzSavConfBuf[tj]=u8plzSavConfBuf[tj-1];
			}
			u8plzSavConfBuf[4-ti]=' ';
		}
	}

	for(ti=0;ti<5;ti++)
	{
		if((u8plzSavConfBuf[ti]<'0')||(u8plzSavConfBuf[ti]>'9'))
			tBuf[ti]=0;
		else
			tBuf[ti]=u8plzSavConfBuf[ti]-'0';
	}
	u16plzSavConfDat =tBuf[0]*10000;
	u16plzSavConfDat+=tBuf[1]*1000;
	u16plzSavConfDat+=tBuf[2]*100;
	u16plzSavConfDat+=tBuf[3]*10;
	u16plzSavConfDat+=tBuf[4];
}

void ConfChr2DatPlz(void)
{
	unsigned char ti,tj;
	unsigned char tBuf[5];
//01234
	for(ti=4;ti>0;ti--)
	{
		if((u8plzSavConfBuf[3]==' ')||(u8plzSavConfBuf[3]==0))
		{
			for(tj=4;tj>(4-ti);tj--)
			{
			 	u8plzSavConfBuf[tj]=u8plzSavConfBuf[tj-1];
			}
			u8plzSavConfBuf[4-ti]=' ';
		}
	}

	for(ti=0;ti<5;ti++)
	{
		if((u8plzSavConfBuf[ti]<'0')||(u8plzSavConfBuf[ti]>'9'))
			tBuf[ti]=0;
		else
			tBuf[ti]=u8plzSavConfBuf[ti]-'0';
	}
	u16plzSavConfDat =tBuf[0]*10000;
	u16plzSavConfDat+=tBuf[1]*1000;
	u16plzSavConfDat+=tBuf[2]*100;
	u16plzSavConfDat+=tBuf[3]*10;
	u16plzSavConfDat+=tBuf[4];
}

/*
WrSetSel(SetIndex,SetPos,Color)
*/
void WrConfSel(unsigned char x,unsigned char y,char *s)
{
	//Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,WHITE);
	if(x==0)//0~9
	{
		TFT_Fill(210,62+y*32,210+34,62+y*32+16,WHITE);
		lcd_text16(234,62+y*32,0,0xFFFF,s);
	}
	else if(x==1)
	{
		TFT_Fill(490,62+y*32,490+48,62+y*32+14,WHITE);
		if(y==9)
			lcd_text16(506,62+y*32,0,0xFFFF,s);
		else
			lcd_text16(490,62+y*32,0,0xFFFF,s);
	}
	else if(x==2)
	{
		TFT_Fill(620+2,58+1+y*32,620+40,58+1+y*32+18,WHITE);
		if(y==10)
			lcd_text16(620+5,61+y*32,0,0xFFFF,s);	 //17=(42-8)/2
		else
			lcd_text16(620+17,61+y*32,0,0xFFFF,s);	 //17=(42-8)/2
	}
}					 
void ConfDat2ChrMd(void)
{
	unsigned int tDat1;
	unsigned char tii;
	tDat1=u16plzSavConfDat;
	u8plzSavConfBuf[0]=tDat1/10000;
	tDat1=tDat1%10000;
	u8plzSavConfBuf[1]=tDat1/1000;
	tDat1=tDat1%1000;
	u8plzSavConfBuf[2]=tDat1/100;
	tDat1=tDat1%100;
	u8plzSavConfBuf[3]=tDat1/10;
	tDat1=tDat1%10;
	u8plzSavConfBuf[4]=tDat1;

	for(tii=0;tii<5;tii++)
		u8plzSavConfBuf[tii]+=0x30;
	u8plzSavConfBuf[5]='\0';
	for(tii=0;tii<4;tii++)
	{
		if(u8plzSavConfBuf[tii]=='0')
	 	  u8plzSavConfBuf[tii]=' ';
		else 
		  break;
	 }
}

void ConfDat2ChrPlz(void)
{
	unsigned int tDat1;
	unsigned char tii;
	tDat1=u16plzSavConfDat;
	u8plzSavConfBuf[0]=tDat1/10000;
	tDat1=tDat1%10000;
	u8plzSavConfBuf[1]=tDat1/1000;
	tDat1=tDat1%1000;
	u8plzSavConfBuf[2]=tDat1/100;
	tDat1=tDat1%100;
	u8plzSavConfBuf[3]=tDat1/10;
	tDat1=tDat1%10;
	u8plzSavConfBuf[4]=tDat1;

	for(tii=0;tii<5;tii++)
		u8plzSavConfBuf[tii]+=0x30;
	u8plzSavConfBuf[5]='\0';
	for(tii=0;tii<4;tii++)
	{
		if(u8plzSavConfBuf[tii]=='0')
	 	  u8plzSavConfBuf[tii]=' ';
		else 
		  break;
	 }
}

/*
u16SavConfDat-->u8SavConfBuf[4]
组态设置界面，由数据变成ASCII码
*/
void ConfDat2Chr(void)
{
	unsigned int tDat1;
	unsigned char tii;
	tDat1=u16SavConfDat;
	u8SavConfBuf[0]=tDat1/100;
	tDat1=tDat1%100;
	u8SavConfBuf[1]=tDat1/10;
	tDat1=tDat1%10;
	u8SavConfBuf[2]=tDat1;
	for(tii=0;tii<8;tii++)
		u8SavConfBuf[tii]+=0x30;
	u8SavConfBuf[3]='\0';
	if(u8SavConfBuf[0]=='0')
	{
		if(u8SavConfBuf[1]=='0')
		{
			u8SavConfBuf[1]=' ';	
		}
			u8SavConfBuf[0]=' ';	
	}
}

/*
ClrSetSel(SetIndex,SetPos,Color)
*/
void ClrConfSel(unsigned char x,unsigned char y)
{
	//Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,WHITE);
	if(x==0)//0~9
	{
		TFT_Fill(202,62+y*32,202+54,62+y*32+16,WHITE);
		//Draw_Sel(200-4,58-4+y*35,56+8,22+8,tColor);
	}
	else if(x==1)//0~9
	{
		//TFT_Fill(490,62+y*32,490+48,62+y*32+14,RED);
		TFT_Fill(490,62+y*32,490+50,62+y*32+15,WHITE);
		//Draw_Sel(200-4,58-4+y*35,56+8,22+8,tColor);
	}
	else if(x==2)//组态参数
	{
		TFT_Fill(620+2,58+2+y*32,620+40,58+2+y*32+16,WHITE);
	}
}

void DisConfSel(unsigned char x,unsigned char y,unsigned int tColor)
{
	if(x==0)//
	{
		Draw_Sel(200-4,58-4+y*32,60+8,22+8,tColor);
	}
	else if(x==1)//
	{
		Draw_Sel(480-4,58-4+y*32,62+8,22+8,tColor);
	}
	else if(x==2)
	{
		Draw_Sel(620-5,58-5+y*32,42+10,22+10,tColor);
	}
	else if(x==3)
	{
		Draw_Sel(700-5,58-5+y*35,80+10,22+10,tColor);
	}	
}
void Draw_Rect_Color(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned char bw,unsigned int tColor)
{
	unsigned int x1,x2,y1,y2;
	unsigned char i;
	x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
	for(i=0;i<bw;i++)
	{
		TFT_DrawLine(x1-i,y1-i,x1-i,y2+i,tColor);	 
		TFT_DrawLine(x1-i,y1-i,x2+i,y1-i,tColor);
	 	TFT_DrawLine(x2+i,y1-i,x2+i,y2+i,tColor);
		TFT_DrawLine(x2+i,y2+i,x1-i,y2+i,tColor);
	}
}

void ShowStr30(unsigned int x,unsigned int y,char *p,unsigned char slen,unsigned int ForeColor,unsigned int BakColor)
{
	//lcd_text32(x,y,ForeColor,BakColor,p);
	unsigned char i,j;
	unsigned short k;
	y=y+5;
	while(*p) {


			for (k=0;k<num24_num;k++) {
			  if (num24[k].Index[0]==*(p))
					{ 
				    for(i=0;i<24;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(num24[k].Msk[i*2]&(0x80>>j))	TFT_DrawPoint(x+j,y+i,ForeColor);
								else {
									if (ForeColor!=BakColor) TFT_DrawPoint(x+j,y+i,BakColor);
								}
							}
						for(j=0;j<4;j++) 
							{
						    	if(num24[k].Msk[i*2+1]&(0x80>>j))	TFT_DrawPoint(x+j+8,y+i,ForeColor);
								else {
									if (ForeColor!=BakColor) TFT_DrawPoint(x+j+8,y+i,BakColor);
								}
							}
				    }
				}
			}
			p+=1;x+=14;
		}
	
}
void Draw_Cell_Conn(unsigned char id,unsigned char sta)
{
	unsigned int tLeft[6]={0,160,320,480,640,800};
	unsigned int tTop[4]={0,160,320,480};
	unsigned int tx,ty;
	unsigned char tid;
	char ch[3];
	tx=(id)%5;
	ty=(id)/5;
	id++;
	if(id!=0)
	{
		if(id<10)
			ch[0]=' ';
		else
			ch[0]=id/10+0x30;
		ch[1]=id % 10+ '0';
		ch[2]=0;
		if(sta==1)
		{
			TFT_Fill(tLeft[tx+1]-24,tTop[ty]+3,tLeft[tx+1],tTop[ty]+24,RED);
			lcd_text16(tLeft[tx+1]-18,tTop[ty]+6,WHITE,RED,ch);//	switch(tid)
		}
		else
		{
			TFT_Fill(tLeft[tx+1]-24,tTop[ty]+3,tLeft[tx+1],tTop[ty]+24,BLUE);
			lcd_text16(tLeft[tx+1]-18,tTop[ty]+6,WHITE,BLUE,ch);//	switch(tid)
		}
	}
}
void Draw_Pkq_STA(unsigned char id,unsigned char sta)
{
	unsigned int tLeft[6]={0,160,320,480,640,800};
	unsigned int tTop[4]={0,160,320,480};
	unsigned int tx,ty;
	tx=id%5;
	ty=id/5;
	if(sta==0x00)
	{	TFT_Fill(tLeft[tx]+10,tTop[ty]+147,tLeft[tx]+19,tTop[ty]+157,WHITE);	}
	if(sta==0x55)
	{
		TFT_Fill(tLeft[tx]+10,tTop[ty]+147,tLeft[tx]+19,tTop[ty]+157,WHITE);	
			LcdDrawTriangle(tLeft[tx]+10,tTop[ty]+147,tLeft[tx]+10,tTop[ty]+157,tLeft[tx]+19,tTop[ty]+152,  BLUE, 1);
  }
		if(sta==0xAA)
	{
		TFT_Fill(tLeft[tx]+10,tTop[ty]+147,tLeft[tx]+19,tTop[ty]+157,WHITE);	
		LcdDrawRectangle(tLeft[tx]+11,tTop[ty]+148,tLeft[tx]+13,tTop[ty]+156,GOLD,1);
		LcdDrawRectangle(tLeft[tx]+17,tTop[ty]+148,tLeft[tx]+19,tTop[ty]+156,GOLD,1);
	}		
}
/**
 * LCD初始化
 */
void lcdInit(void)
{
	raInit();
}


/**
 * lcd写像素函数
 * @param x      X
 * @param y      Y
 * @param tColor 颜色
 */
void lcdDrawPixel(uint16_t x, uint16_t y, uint16_t tColor)
{
	raMemoryBusy();	
	raGraphicMode();
	raDrawPixel(x, y, tColor);
}

/**
 * 读点像素
 * @param  x 位置X
 * @param  y 位置Y
 * @return   返回像素数据
 */
uint16_t lcdReadPixel(uint16_t x, uint16_t y)
{
	raMemoryBusy();	
	raGraphicMode();
	return raReadPixel(x, y);
}

/**
 * 调节背光
 * @param tPercent 0-100 亮度
 */
void lcdBacklightAdjust(uint8_t tPercent)
{
	float tF = tPercent *2.56;
	if(tF > 255)
		tF = 255;
	raPWM1Enable();
	raPWM1RetainLowWhenDisabled();
	raPWM1_fnuction_sel();
	raPWM1ClockDiv(0X02);
	raPWM1DutyCycle((uint16_t)(tF));
}
void blADJ(uint8_t tBL)
{
	raPWM1DutyCycle(tBL);
}

void LCD_BL(unsigned int bl_Level)
{
	lcdBacklightAdjust(100);
}
/**
 * 显示ASCII字符
 * @param x             X坐标
 * @param y             Y坐标
 * @param tForeColor    前景色
 * @param tBackColor    后景色
 * @param isTransparent 是否透明
 * @param str           字符
 */
void LcdDisplayASCII(uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor, bool isTransparent, char *str )
{
	raDisplayASCII( x,  y,  tForeColor,  tBackColor,  isTransparent, str );
}

/**
 * 画线函数
 * @param x1     起点X
 * @param y1     起点Y
 * @param x2     终点X
 * @param y2     终点Y
 * @param tColor 颜色
 */
void LcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t tColor)
{
	raMemoryBusy();	
	raGraphicMode();
	raBTEForegroundColor(tColor);//颜色
	if((x1 == x2)&&(y1 == y2))
		lcdDrawPixel(x1, y1, tColor);
	else
		raDrawLine( x1,  y1,  x2,  y2);
}

/**
 *  在LCD显存上画线
 * @param x      X
 * @param y      Y
 * @param tWidth 宽
 * @param arr    缓存
 */
void LcdMemoryDrawHLine(uint16_t x, uint16_t y, uint16_t tWidth, uint16_t* arr)
{
	raGraphicMode();
	raGraphicMode();
	raMemoryDrawHLine( x,  y,  tWidth,  arr);
}


/**
 * 画矩形
 * @param x1      X1
 * @param y1      Y1
 * @param x2      X2
 * @param y2      Y2
 * @param		颜色
 * @param tIsfill 是否填充
 */
void LcdDrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t tColor, bool tIsfill)
{
	raMemoryBusy();	
	raGraphicMode();
	raBTEForegroundColor(tColor);//颜色
	if((x1 == x2)&&(y1 == y2))
		lcdDrawPixel(x1, y1,tColor);
	else
		raDrawRectangle( x1,  y1,  x2,  y2,   tIsfill);	
}

/**
 * 画圆
 * @param X       圆心X
 * @param Y       圆心Y
 * @param R       半径
 * @param		颜色
 * @param tIsfill 是否填充
 */
void LcdDrawCircle(uint16_t X,uint16_t Y,uint16_t R, uint16_t tColor, bool tIsfill)
{
	raMemoryBusy();	
	raGraphicMode();
	raBTEForegroundColor(tColor);//颜色
	raDrawCircle( X, Y, R,   tIsfill);
}

/**
 * 画椭圆
 * @param X       中心点X
 * @param Y       中心点Y
 * @param ELL_A   ELL_A
 * @param ELL_B   ELL_B
 * @param		颜色
 * @param tIsfill 是否填充
 */
void LcdDrawEllipse(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B, uint16_t tColor, bool tIsfill)
{
	raMemoryBusy();	
	raGraphicMode();
	raBTEForegroundColor(tColor);//颜色
	raDrawEllipse( X, Y, ELL_A, ELL_B,   tIsfill);
}

/**
 * 画椭圆扇形
 * @param X       中心点X
 * @param Y       中心点
 * @param ELL_A   ELL_A
 * @param ELL_B   ELL_B
 * @param tIsfill 是否填充
 * @param		颜色
 * @param tPart   00：左下 01：左上 10：右上 11；右下
 */
void LcdDrawEllipseCurve(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B,uint16_t tColor, bool tIsfill, uint8_t tPart)
{
	raMemoryBusy();	
	raGraphicMode();
	raBTEForegroundColor(tColor);//颜色
	raDrawEllipseCurve( X, Y, ELL_A, ELL_B,  tIsfill,  tPart);
}

/**
 * 圆角矩形
 * @param XL      左边X坐标
 * @param YT      顶部Y坐标
 * @param XR      右边X坐标
 * @param YB      底部Y坐标
 * @param ELL_A   ELL_A
 * @param ELL_B   ELL_B
 * @param		颜色
 * @param tIsfill 是否填充
 */
void LcdDrawCircleSquare(uint16_t XL,uint16_t YT ,uint16_t XR ,uint16_t YB ,uint16_t ELL_A,uint16_t ELL_B, uint16_t tColor,bool tIsfill)
{
	raMemoryBusy();	
	raGraphicMode();
	raBTEForegroundColor(tColor);//颜色
	raDrawCircleSquare( XL, YT , XR , YB , ELL_A,  ELL_B,  tIsfill);
}

/**
 * 画三角形
 * @param P1_X    P1点X
 * @param P1_Y    P1点Y
 * @param P2_X    P2点X
 * @param P2_Y    P2点Y
 * @param P3_X    P3点X
 * @param P3_Y    P3点Y
 * @param tIsfill 填充
 */
void LcdDrawTriangle(uint16_t P1_X,uint16_t P1_Y,uint16_t P2_X,uint16_t P2_Y,uint16_t P3_X,uint16_t P3_Y,  uint16_t tColor, bool tIsfill)
{
	raMemoryBusy();	
	raGraphicMode();
	raBTEForegroundColor(tColor);//颜色
	raDrawTriangle( P1_X, P1_Y, P2_X, P2_Y, P3_X,  P3_Y,    tIsfill);
}
//void LcdDisplayStringEN(char * zh, uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,bool tTransparency, GTFont_type tType )
void DisEN48(char * zh,uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,bool tTransparency)
{
	raMemoryBusy();	
	raTextMode();
	raFontControlCGROMFont();
	raFontControlExternalCGROM();
	raSromClkDiv(0X03);
	raSerialROMSelect0();
  raFontVerticalEnlarge_x2();
  raFontHorizontalEnlarge_x2();
  raFontArial();//raFontRoman();//raFontCodeGB2312();
  raFontSize24x24_12x24();
	if(tTransparency)
		raFontWithBackgroundTransparency();
	else
		raFontWithBackgroundColor();
	raGTSROMGT23L32S4W();
	raFontWriteCoordinate(x, y);
	raBTEBackgroundColor(tBackColor);
	raBTEForegroundColor(tForeColor);
	raDisplayString(zh);
  raFontVerticalEnlarge_x1();
  raFontHorizontalEnlarge_x1();
  raFontCodeGB2312();
}
void DisEN24(char * zh,uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,bool tTransparency)
{
	raMemoryBusy();	
	raTextMode();
	raFontControlCGROMFont();
	raFontControlExternalCGROM();
	raSromClkDiv(0X03);
	raSerialROMSelect0();
  //raFontVerticalEnlarge_x2();
  //raFontHorizontalEnlarge_x2();
  raFontArial();//raFontRoman();//raFontCodeGB2312();
  raFontSize24x24_12x24();
	if(tTransparency)
		raFontWithBackgroundTransparency();
	else
		raFontWithBackgroundColor();
	raGTSROMGT23L32S4W();
	raFontWriteCoordinate(x, y);
	raBTEBackgroundColor(tBackColor);
	raBTEForegroundColor(tForeColor);
	raDisplayString(zh);
  raFontVerticalEnlarge_x1();
  raFontHorizontalEnlarge_x1();
  raFontCodeGB2312();
}
void DisEN32(uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,char * zh)
{
	raMemoryBusy();	
	raTextMode();
	raFontControlCGROMFont();
	raFontControlExternalCGROM();
	raSromClkDiv(0X03);
	raSerialROMSelect0();
  //raFontVerticalEnlarge_x2();
  //raFontHorizontalEnlarge_x2();
  raFontArial();//raFontRoman();//
  raFontSize32x32_16x32();
	//if(tTransparency)
	//	raFontWithBackgroundTransparency();
	//else
		raFontWithBackgroundColor();
	raGTSROMGT23L32S4W();
	raFontWriteCoordinate(x, y);
	raBTEBackgroundColor(tBackColor);
	raBTEForegroundColor(tForeColor);
	raDisplayString(zh);
  raFontVerticalEnlarge_x1();
  raFontHorizontalEnlarge_x1();
  raFontCodeGB2312();
}
void LcdDisplayStringEN(char * zh, uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,bool tTransparency)
{
	raMemoryBusy();	
	raTextMode();
	raFontControlCGROMFont();
	raFontControlExternalCGROM();
	raSromClkDiv(0X03);
	raSerialROMSelect0();
  //raFontVerticalEnlarge_x4();
  //raFontHorizontalEnlarge_x4();
  raFontRoman();//raFontCodeGB2312();//raFontArial();
  raFontSize32x32_16x32();
	if(tTransparency)
		raFontWithBackgroundTransparency();
	else
		raFontWithBackgroundColor();
	raGTSROMGT23L32S4W();
	raFontWriteCoordinate(x,y);
	raBTEBackgroundColor(tBackColor);
	raBTEForegroundColor(tForeColor);
	raDisplayString(zh);
  raFontVerticalEnlarge_x1();
  raFontHorizontalEnlarge_x1();
	raFontCodeGB2312();
}

/**
 * 在指定位置显示中文
 * @param zh         要显示的中文字符串
 * @param x          X
 * @param y          Y
 * @param tForeColor 前景色
 * @param tBackColor 背景色
 * @param tTransparency 背景是否透明
 * @param	GTFont_type 字体类型
 */
void LcdDisplayStringZH(char * zh, uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,bool tTransparency, GTFont_type tType )
{
	raMemoryBusy();	
	raTextMode();
	raFontControlCGROMFont();
	raFontControlExternalCGROM();
	raSromClkDiv(0X03);
	raSerialROMSelect0();
	switch(tType)
	{
		case GB2312_15X16_8x16_HZ:
								raFontCodeGB2312();
								raFontSize16x16_8x16();
								break; 		
		case GB2312_24x24_12x24_HZ:
								raFontCodeGB2312();
								raFontSize24x24_12x24();
								break; 		
		case GB2312_32X32_16x32_HZ:
								raFontCodeGB2312();
								raFontSize32x32_16x32();
								break; 		

	}
	if(tTransparency)
		raFontWithBackgroundTransparency();
	else
		raFontWithBackgroundColor();
	raGTSROMGT23L32S4W();
	raFontWriteCoordinate(x, y);
	raBTEBackgroundColor(tBackColor);
	raBTEForegroundColor(tForeColor);
	raDisplayString(zh);
}
extern unsigned char tPwm;
void Dis_Num_8b(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint32_t Dat)
{
	char tDisBuf[9];
	uint32_t tDat;
	uint16_t i;
	tDat=Dat;
	tDisBuf[0]='0'+tDat/10000000;
        tDat=tDat%10000000;
	tDisBuf[1]='0'+tDat/1000000;
        tDat=tDat%1000000;
	tDisBuf[2]='0'+tDat/100000;
        tDat=tDat%100000;
	tDisBuf[3]='0'+tDat/10000;
        tDat=tDat%10000;
	tDisBuf[4]='0'+tDat/1000;
        tDat=tDat%1000;
	tDisBuf[5]='0'+tDat/100;
        tDat=tDat%100;
	tDisBuf[6]='0'+tDat/10;
        tDat=tDat%10;
	tDisBuf[7]='0'+tDat;
	tDisBuf[8]=0;
	for(i=0;i<7;i++)
	{
		if(tDisBuf[i]=='0')
		{
			tDisBuf[i]=' ';
		}
		else
		{
			break;
		}
	}
	//	Dis_Num16(400,440,0,GOLD,PadCode[0]);
	//LcdDisplayStringZH(tDisBuf,x,y,fc,bc,0,GB2312_24x24_12x24_HZ);
  lcd_text16(x, y, fc, bc,  tDisBuf );
}
void Dis_Hex(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint16_t Dat)
{
  char tDisBuf[3];
  uint16_t tDat;
  tDat=Dat;
  if((tDat>>4)<10)
    tDisBuf[0]='0'+(tDat>>4);
  else
    tDisBuf[0]='A'+((tDat>>4)-10);
  if((tDat&0x0F)<10)
    tDisBuf[1]='0'+(tDat&0x0F);
  else
    tDisBuf[1]='A'+((tDat&0x0F)-10);
  tDisBuf[2]=0; 
	TFT_Fill(x,y,x+18,y+16,bc);
	LcdDisplayStringZH(tDisBuf,x,y,fc,bc,0,GB2312_15X16_8x16_HZ);
}  
void Dis_Num16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint16_t Dat)
{
		char tDisBuf[6];
	uint16_t tDat;
	tDat=Dat;
	tDisBuf[0]='0'+tDat/10000;
        tDat=tDat%10000;
	tDisBuf[1]='0'+tDat/1000;
        tDat=tDat%1000;
	tDisBuf[2]='0'+tDat/100;
        tDat=tDat%100;
	tDisBuf[3]='0'+tDat/10;
        tDat=tDat%10;
	tDisBuf[4]='0'+tDat;
	tDisBuf[5]=0;
		lcd_text16(x,y,0,GOLD,tDisBuf);
	//LcdDisplayStringZH(tDisBuf,x,y,fc,bc,0,GB2312_15X16_8x16_HZ);
}
/*
void Dis_Curve(void)
{
  unsigned int i;
	unsigned int posx,posy;
	unsigned int endx,endy;
	unsigned char axial_x;
	unsigned char tdelta;
	char tDisBuf[4];
	//SenDat[0]=0;y=400; SenDat[0]=2048;y=190;
	for(i=0;i<360;i++)
	{
		posx=30+(i<<1);		posy=400-(bufSen1[i]/10);	//???200Mpa
		if(posy<190)posy=190;
		if(posy>400)posy=400;
		endx=30+((i+1)<<1);endy=400-(bufSen1[i+1]/10);
		if(endy<190)endy=190;
		if(endy>400)endy=400;
		if((posy==endy)&&(endy==400))
		 {posy=endy;}
		else
		 {TFT_DrawLine(posx,posy,endx,endy,0xFFFF);}

//		posx=30+(i<<1);		posy=400-(bufSen2[i]/10);	//???200Mpa
//		if(posy<190)posy=190;
//		if(posy>400)posy=400;
//		endx=30+((i+1)<<1);endy=400-(bufSen2[i+1]/10);
//		if(endy<190)endy=190;
//		if(endy>400)endy=400;
//		if((posy==endy)&&(endy==400))
//		 {posy=endy;}
//		else
//		 {TFT_DrawLine(posx,posy,endx,endy,0xFFFF);}

		posx=30+(i<<1);		posy=400-(bufSen3[i]/10);	//???200Mpa
		if(posy<190)posy=190;
		if(posy>400)posy=400;
		endx=30+((i+1)<<1);endy=400-(bufSen3[i+1]/10);
		if(endy<190)endy=190;
		if(endy>400)endy=400;
		if((posy==endy)&&(endy==400))
		 {posy=endy;}
		else
		 {TFT_DrawLine(posx,posy,endx,endy,0xFFFF);}
	}
	for(i=360;i>0;i--)
	{
	  bufSen1[i]=bufSen1[i-1];
	  bufSen2[i]=bufSen2[i-1];
	  bufSen3[i]=bufSen3[i-1];
	}
	bufSen1[0]=paSenDat[0];
	bufSen2[0]=SenDat[1];
	bufSen3[0]=SenDat[2];
		  for(i=10;i<730;i+=10)
			{
			 TFT_DrawLine(30+i,190,30+i,399,GRED);//LGRAY
			}
			for(i=50;i<730;i+=50)
			{
				axial_x=i/10;
				tDisBuf[0]=(axial_x/10)+'0';
				tDisBuf[1]=(axial_x%10)+'0';
				tDisBuf[2]=0;tdelta=8;
				if(tDisBuf[0]=='0'){tDisBuf[0]=' ';tdelta=12;}
				lcd_text16(30+i-tdelta,405,BLUE,BLUE,tDisBuf);
			 	//TFT_DrawLine(30+i,390,30+i,399,GRED);//LGRAY
				Draw_Line(30+i, 390,30+i, 399,2,RED);
				Draw_Line(30+i, (400-sys_SetP-5),30+i, (400-sys_SetP+5),2,RED);
			}
			TFT_DrawLine(30,(400-sys_SetP),765,(400-sys_SetP),RED);			
			Draw_Line(30, 190,26, 210,1,BLACK);
			Draw_Line(30, 190,34, 210,1,BLACK);
			Draw_Line(30, 190,30, 420,1,BLACK);//??
			Draw_Line(785,400,765,396,1,BLACK);
			Draw_Line(785,400,765,404,1,BLACK);
			Draw_Line(785,400, 10,400,1,BLACK);//??

	for(i=0;i<360;i++)
	{
		posx=30+(i<<1);posy=400-(bufSen1[i]/10);//???200Mpa,?????1Mpa
		if(posy<190)posy=190;
		if(posy>400)posy=400;
		endx=30+((i+1)<<1);endy=400-(bufSen1[i+1]/10);//???200Mpa,?????1Mpa
		if(endy<190)endy=190;
		if(endy>400)endy=400;
		if((posy==endy)&&(endy==400))
		 {posy=endy;}
		else
		 {TFT_DrawLine(posx,posy,endx,endy,BLUE);}

//		posx=30+(i<<1);posy=400-(bufSen2[i]/10);//???200Mpa,?????1Mpa
//		if(posy<190)posy=190;
//		if(posy>400)posy=400;
//		endx=30+((i+1)<<1);endy=400-(bufSen2[i+1]/10);//???200Mpa,?????1Mpa
//		if(endy<190)endy=190;
//		if(endy>400)endy=400;
//		if((posy==endy)&&(endy==400))
//		 {posy=endy;}
//		else
//		 {TFT_DrawLine(posx,posy,endx,endy,BLACK);}

		posx=30+(i<<1);posy=400-(bufSen3[i]/10);//???200Mpa,?????1Mpa
		if(posy<190)posy=190;
		if(posy>400)posy=400;
		endx=30+((i+1)<<1);endy=400-(bufSen3[i+1]/10);//???200Mpa,?????1Mpa
		if(endy<190)endy=190;
		if(endy>400)endy=400;
		if((posy==endy)&&(endy==400))
		 {posy=endy;}
		else
		 {TFT_DrawLine(posx,posy,endx,endy,GREEN);}
	}
}
*/
void TFT_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,u16 color)
{
	LcdDrawLine(x1, y1,  x2,  y2, color);
}

void Draw_Line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned char bw,unsigned int color )
{
	unsigned char i;
	for (i=0;i<bw;i++)
	{
		if(x1!=x2)
			LcdDrawLine(x1, y1+i,  x2,  y2+i, color);
		else
			LcdDrawLine(x1+i, y1,  x2+i,  y2, color);
	} 	
}
void Draw_Rect(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned int color)
{
	unsigned int x1,x2,y1,y2;
	x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;

	LcdDrawLine(x1,y1,x1,y2,color);	 
	LcdDrawLine(x1,y1,x2,y1,color);	
	//LcdDrawLine(x1+1,y1+1,x1+1,y2-1,color);	
	//LcdDrawLine(x1+1,y1+1,x2-1,y1+1,color);	
	LcdDrawLine(x2,y1,x2,y2,color);	
	LcdDrawLine(x2,y2,x1,y2,color);	
	//LcdDrawLine(x2-1,y1+1,x2-1,y2-1,color);	
	//LcdDrawLine(x2-1,y2-1,x1+1,y2-1,color);	

}
void Draw_TextBox(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight)
{
	unsigned int x1,x2,y1,y2;
	x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;

	LcdDrawLine(x1,y1,x1,y2,0);	 
	LcdDrawLine(x1,y1,x2,y1,0);	
	LcdDrawLine(x1+1,y1+1,x1+1,y2-1,0);	//0x42080x8C30
	LcdDrawLine(x1+1,y1+1,x2-1,y1+1,0);	
	LcdDrawLine(x2,y1,x2,y2,0xE6FB);	
	LcdDrawLine(x2,y2,x1,y2,0xE6FB);	
	LcdDrawLine(x2-1,y1+1,x2-1,y2-1,0x8C30);	
	LcdDrawLine(x2-1,y2-1,x1+1,y2-1,0x8C30);	
//		TFT_DrawLine(x1,y1,x1,y2,0);
//	  TFT_DrawLine(x1,y1,x2,y1,0);	
}
void DisNum24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,char * s)
{
	unsigned char i,j;
	unsigned short k;
  y=y+5;
	while(*s) 
		{
			for (k=0;k<num24_num;k++) 
			{
			  if(*(s)=='.')
				{
					TFT_Fill(x+2,y+17,x+4,y+21,fc);
					TFT_Fill(x+1,y+18,x+5,y+20,fc);
					s+=1;x+=8;
				}
				else
				{	
					if (num24[k].Index[0]==*(s))
					{ 
							for(i=0;i<24;i++)
							{
							for(j=0;j<8;j++) 
								{
										if(num24[k].Msk[i*2]&(0x80>>j))	TFT_DrawPoint(x+j,y+i,fc);
									else {
										if (fc!=bc) TFT_DrawPoint(x+j,y+i,bc);
									}
								}
							for(j=0;j<4;j++) 
								{
										if(num24[k].Msk[i*2+1]&(0x80>>j))	TFT_DrawPoint(x+j+8,y+i,fc);
									else {
										if (fc!=bc) TFT_DrawPoint(x+j+8,y+i,bc);
									}
								}
							}
					s+=1;x+=14;
					}
			}
		}
		}
}

void lcd_text32_(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	LcdDisplayStringZH(s,x,y,fc,bc,0,GB2312_32X32_16x32_HZ);
}

void lcd_text32(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	LcdDisplayStringEN(s,x,y,fc,bc,0);
}
void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) 
		{

			for (k=0;k<hz24_num;k++) {
			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1))){ 
				    for(i=0;i<24;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3]&(0x80>>j))	TFT_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) TFT_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	TFT_DrawPoint(x+j+8,y+i,fc);
								else {
									if (fc!=bc) TFT_DrawPoint(x+j+8,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	TFT_DrawPoint(x+j+16,y+i,fc);
								else {
									if (fc!=bc) TFT_DrawPoint(x+j+16,y+i,bc);
								}
							}
				    }
				}
			}
			s+=2;x+=24;
		}
}

void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) {
		if( *s < 0x80 ) {
			k=*s;
			if (k>=32) k-=32; else k=0;

		    for(i=0;i<16;i++)
			for(j=0;j<8;j++) 
				{
			    	if(asc16[k*16+i]&(0x80>>j))	TFT_DrawPoint(x+j,y+i,fc);
					else {
						if (fc!=bc) TFT_DrawPoint(x+j,y+i,bc);
					}
				}
			s++;x+=8;
			}
		else {

			for (k=0;k<hz16_num;k++) {
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1))){ 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	TFT_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) TFT_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	TFT_DrawPoint(x+j+8,y+i,fc);
								else {
									if (fc!=bc) TFT_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			}
			s+=2;x+=16;
			}
		}
}

void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{
  LcdDrawRectangle(xsta, ysta, xend, yend, color, 1);
}

void Draw_Lamp(unsigned int x,unsigned int y,unsigned char tColor)
{
	if(tColor==0)
		LcdDrawCircle(x+8,y+8,8, BLUE,1);
	if(tColor==1)
		LcdDrawCircle(x+8,y+8,8, RED,1);
}
void Draw_Lamp_Color(unsigned int x,unsigned int y,unsigned char tColor)
{
  LcdDrawCircle(x,y,8, tColor,1);
}

void Dis_SysAdd(unsigned char tSysAdd)
{
	char tSysAddBuff[3];
	unsigned int x,y,fc,bc;
	tSysAddBuff[0]='0'+(tSysAdd/10);
	tSysAddBuff[1]='0'+(tSysAdd%10);
	tSysAddBuff[2]=0;
	x=30;y=0;fc=BLACK;bc=GOLD;
	//LcdDisplayStringZH(tSysAddBuff,x,y,fc,bc,0,GB2312_24x24_12x24_HZ);
	//lcd_text24(70,4,BLUE,GOLD,tSysAddBuff);
	DisNum24(30,0,BLUE,GOLD,tSysAddBuff);
	
	//ShowStr30(30,15,tSysAddBuff,2,BLACK,GOLD);
}
//sysY_pre,sysY,sysM_pre,sysM,sysD_pre,sysD,															   
//void Dis_Date(void)
//{
//	char tDateBuf[10];
//	unsigned int x,y,fc,bc;
// 	if((flgDisDate)||(sysY_pre!=sysY)||(sysM_pre!=sysM)||(sysD_pre!=sysD))
//	{
//		flgDisDate=0;
//		tDateBuf[0]='2';
//		tDateBuf[1]='0';
//		tDateBuf[2]='0'+(sysY/10);
//		tDateBuf[3]='0'+(sysY%10);
//		tDateBuf[4]='-';
//		tDateBuf[5]='0'+(sysM/10);
//		tDateBuf[6]='0'+(sysM%10);
//		tDateBuf[7]='-';
//		tDateBuf[8]='0'+(sysD/10);
//		tDateBuf[9]='0'+(sysD%10);
//		sysY_pre=sysY;sysM_pre=sysM;sysD_pre=sysD;
//		x=365+64;y=0;fc=BLACK;bc=GOLD;
//		LcdDisplayStringZH(tDateBuf,x,y,fc,bc,0,GB2312_24x24_12x24_HZ);
//		//ShowStr30(365+64,15,tDateBuf,10,BLACK,GOLD);
//	}
//}

void Frm_Init(void)
{
	//TFT_Fill(0,0,799,15,0x0000);	
	//TFT_Fill(0,0,24,479,0x0000);	
	//TFT_Fill(0,460,799,479,0x0000);	
	TFT_Fill(0,0,799,49-15,GOLD);	
	TFT_Fill(0,432,799,479,GOLD);	
	//Dis_SysAdd(set_AddMb);
	TFT_Fill(0,49-15,799,431,WHITE);	
	Draw_Line(0,34,799,34,3,BLACK);Draw_Line(0,432,799,432,3,BLACK);
	Draw_Line(    6,438,394,438,3,BLACK);Draw_Line(406,438,788,438,3,BLACK);
	Draw_Line(    6,474,394,474,3,BLACK);Draw_Line(406,474,788,474,3,BLACK);
	Draw_Line(    6,438,  6,474,3,BLACK);Draw_Line(394,438,394,474,3,BLACK);
	Draw_Line(406,438,406,477,3,BLACK);Draw_Line(788,438,788,477,3,BLACK);
	Dis_SysAdd(set_AddCa);
}
void Draw_Frame(u8 x,u8 y,u16 color)
{
	//unsigned int tLeft[6]={55-55,233-55,411-55,589-55,767-55,740-55};
	unsigned int tLeft[6]={0,160,320,480,640,800};
	unsigned int tTop[4]={0,160,320,480};
	if(x<5)
	{
		Draw_Line(tLeft[x],tTop[y],tLeft[x+1],tTop[y],3,color);
		Draw_Line(tLeft[x],tTop[y+1],tLeft[x+1]+3,tTop[y+1],3,color);
		Draw_Line(tLeft[x],tTop[y],tLeft[x],tTop[y+1],3,color);
		Draw_Line(tLeft[x+1],tTop[y],tLeft[x+1],tTop[y+1],3,color);
	}
	else
	{
		Draw_Sel(750-5,210+y*50-5,50,40,color);	
	}
}
void Draw_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen)
{
unsigned char i;
	//unsigned int x1,x2,y1,y2;
unsigned int hl,ht;
	//x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
	//700+(80-32)/2;
	//330+(60-16)/2
	i=tLen;
	{
		hl=tLeft+(tWidth-i*16)/2;
		ht=tTop+(tHeight-16)/2;
	  lcd_text16(hl,ht,0,0xFFFF,p);
	}

}

void Draw_Button_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen)
{
unsigned char i;
unsigned int x1,x2,y1,y2;
unsigned int hl,ht;
x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
//0xffff,0xe6fb,0x4208,0x0000
//0xFFFF,0xEF5D,0xDEFC,0xA535,0x8431,0x0000
	TFT_DrawLine(x1,y1,x1,y2,0xFFFF);	//left3 
	TFT_DrawLine(x1,y1,x2,y1,0xFFFF);	//top3 
	TFT_DrawLine(x1-1,y1-1,x1-1,y2+1,0xEF5D);	//left2 
	TFT_DrawLine(x1-1,y1-1,x2+1,y1-1,0xEF5D);	//top2 
	TFT_DrawLine(x1-2,y1-2,x1-2,y2+2,0xDEFC);	//left1 
	TFT_DrawLine(x1-2,y1-2,x2+2,y1-2,0xDEFC);	//top1 

 	TFT_DrawLine(x2,y1,x2,y2,0xA535);//right1	
	TFT_DrawLine(x2,y2,x1,y2,0xA535);//buttom1	
 	TFT_DrawLine(x2+1,y1-1,x2+1,y2+1,0x8431);//right2	
	TFT_DrawLine(x2+1,y2+1,x1-1,y2+1,0x8431);//buttom2	
 	TFT_DrawLine(x2+2,y1-2,x2+2,y2+2,0x0000);//right3	
	TFT_DrawLine(x2+2,y2+2,x1-2,y2+2,0x0000);//buttom3	

	i=tLen;
	{
		hl=x1+(tWidth-i*16)/2;
		ht=y1+(tHeight-16)/2;
	  lcd_text16(hl,ht,0,0xFFFF,p);
	}
}
void Draw_Button(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight)
{
unsigned char i;
unsigned int x1,x2,y1,y2;
x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
//0xffff,0xe6fb,0x4208,0x0000
//0xFFFF,0xEF5D,0xDEFC,0xA535,0x8431,0x0000
	TFT_DrawLine(x1,y1,x1,y2,0xFFFF);	//left3 
	TFT_DrawLine(x1,y1,x2,y1,0xFFFF);	//top3 
	TFT_DrawLine(x1-1,y1-1,x1-1,y2+1,0xEF5D);	//left2 
	TFT_DrawLine(x1-1,y1-1,x2+1,y1-1,0xEF5D);	//top2 
	TFT_DrawLine(x1-2,y1-2,x1-2,y2+2,0xDEFC);	//left1 
	TFT_DrawLine(x1-2,y1-2,x2+2,y1-2,0xDEFC);	//top1 

 	TFT_DrawLine(x2,y1,x2,y2,0xA535);//right1	
	TFT_DrawLine(x2,y2,x1,y2,0xA535);//buttom1	
 	TFT_DrawLine(x2+1,y1-1,x2+1,y2+1,0x8431);//right2	
	TFT_DrawLine(x2+1,y2+1,x1-1,y2+1,0x8431);//buttom2	
 	TFT_DrawLine(x2+2,y1-2,x2+2,y2+2,0x0000);//right3	
	TFT_DrawLine(x2+2,y2+2,x1-2,y2+2,0x0000);//buttom3	

	for(i=0;i<3;i++)
	{
		TFT_DrawLine(x1-3-i,y1-3-i,x1-3-i,y2+3+i,0xFFFF);	 
		TFT_DrawLine(x1-3-i,y1-3-i,x2+3+i,y1-3-i,0xFFFF);
	 	TFT_DrawLine(x2+3+i,y1-3-i,x2+3+i,y2+3+i,0xFFFF);
		TFT_DrawLine(x2+3+i,y2+3+i,x1-3-i,y2+3+i,0xFFFF);
	}
}

void Draw_Sel(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned int tColor)
{
	unsigned int x1,x2,y1,y2;
	unsigned char i;
	x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
	for(i=0;i<3;i++)
	{
		TFT_DrawLine(x1,y1+i,x2,y1+i,tColor);	//TOP LINE
		TFT_DrawLine(x1+i,y1,x1+i,y2,tColor);	//LEFT LINE
		TFT_DrawLine(x1,y2-i,x2,y2-i,tColor);	//Buttom LINE
		TFT_DrawLine(x2-i,y1,x2-i,y2,tColor);	//Right LINE
	}
}

/*
u8SavSetBuf[4]-->u16SavSetDat
发油设置界面,由ASCII码转变成数据
*/
unsigned int FyChr2Dat(void)
{
	unsigned char tii;
	unsigned char tBuf[4];
	if(u8SavFyBuf[0]!=' ')
	{
		if(u8SavFyBuf[1]==' ')
		{
			u8SavFyBuf[2]=u8SavFyBuf[0];
			u8SavFyBuf[0]='0';
			u8SavFyBuf[1]='0';
		}
		else if(u8SavFyBuf[2]==' ')
		{
			u8SavFyBuf[2]=u8SavFyBuf[1];
			u8SavFyBuf[1]=u8SavFyBuf[0];
			u8SavFyBuf[0]='0';
		}
	}
	for(tii=0;tii<3;tii++)
	{
		tBuf[tii]=u8SavFyBuf[tii];
		if((tBuf[tii]>'9')||(tBuf[tii]<'0'))
		{
		  tBuf[tii]='0';
		}
		tBuf[tii]-='0';
	}
	u16SavFyDat=tBuf[0]*100;
	u16SavFyDat+=tBuf[1]*10;
	u16SavFyDat+=tBuf[2];
	return u16SavFyDat;
}
void CalSavMaxVol(void)
{
	unsigned char ti,tj;
	unsigned char tDat[7];
	unsigned long lngDat=0;
	for(ti=0;ti<6;ti++)
	{
	 if((tmpSavMaxVol[6]<='9')&&(tmpSavMaxVol[6]>='0'))
	 {
		 break;
	 }
	 else
	 {
		 for(tj=6;tj>0;tj--)
		 {
			tmpSavMaxVol[tj]=tmpSavMaxVol[tj-1];
		 }
		  tmpSavMaxVol[0]=' ';
	 }
	}
	for(ti=0;ti<7;ti++)
	{
	 if((tmpSavMaxVol[ti]<='9')&&(tmpSavMaxVol[ti]>='0'))
	 {
		tDat[ti]=tmpSavMaxVol[ti]-'0';
	 }
	 else
	 {
		tDat[ti]=0;
	 }
	}
	lngDat =tDat[0]*1000000;
	lngDat+=tDat[1]*100000;
	lngDat+=tDat[2]*10000;
	lngDat+=tDat[3]*1000;
	lngDat+=tDat[4]*100;
	lngDat+=tDat[5]*10;
	lngDat+=tDat[6];
	if(lngDat<9999999)
	{
	  SavMaxVol=lngDat;
		SavMaxVol1=(unsigned char)((lngDat>>16)&0xFF);
		SavMaxVol2=(unsigned char)((lngDat>>8)&0xFF);
		SavMaxVol3=(unsigned char)((lngDat)&0xFF);
		for(ti=0;ti<7;ti++)
			EepWrBuf[ti]=tDat[ti];
		EepWrBuf[7]=SavMaxVol1;EepWrBuf[8]=SavMaxVol2;EepWrBuf[9]=SavMaxVol3;
		EepWr(170,10);
	}
//	else
//	{
//
//	}
}
void Dis_GdsId(unsigned char tGdsId)
{
	char tGdsIdBuf[3];
	tGdsIdBuf[0]='0'+(tGdsId/10);
	tGdsIdBuf[1]='0'+(tGdsId%10);
	ShowStr30(30,0,tGdsIdBuf,2,BLACK,GOLD);
}

void funcSavFyDat(void)
{
	unsigned char ti,i;
//	unsigned char u8SavFyDat[32];

	//将Int数据转成char型，准备保存
	//EEP_Write(FlashBuf,10,130);//	SPI2_Flash_Read(FlashBuf,0,130);
	for(ti=0;ti<16;ti++)
	{
		tSavFyDat[2*ti]  =SavFyDat[ti]>>8;
		tSavFyDat[2*ti+1]=SavFyDat[ti]&0xFF;
	}
	for(i=0;i<32;i++)
		EepWrBuf[i]=tSavFyDat[i];
	EepWr(130,32);
	reNewSetDat();
}

/*
ClrSetSel(SetIndex,SetPos,Color)
*/
void ClrSetSel(unsigned char x,unsigned char y)
{
	//Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,WHITE);
	if(x==0)//0~9
	{
		TFT_Fill(220,62+y*35,220+24,62+y*35+16,WHITE);
		//Draw_Sel(200-4,58-4+y*35,56+8,22+8,tColor);
	}
	else if(x==1)//10~19
	{
		if(y!=8)
		TFT_Fill(480,62+y*35,480+24,62+y*35+16,WHITE);
		else
		TFT_Fill(454,62+y*35,454+56,62+y*35+16,WHITE);
	}
	else if(x==2)//组态参数
	{
		TFT_Fill(620+2,58+2+y*35,620+40,58+2+y*35+16,WHITE);
	}
}

void DisSetSel(unsigned char x,unsigned char y,unsigned int tColor)
{
	//Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,WHITE);
	if(x==0)//0~9
	{		Draw_Sel(200-4,58-4+y*35,56+8,22+8,tColor);	}
	else if(x==1)//10~19
	{
		  if(y<8)
			Draw_Sel(460-4,58-4+y*35,56+8,22+8,tColor);	
			else
			Draw_Sel(450-4,58-4+y*35,66+8,22+8,tColor);
	}
	else if(x==2)
	{		Draw_Sel(600-5,58-5+y*35,120+10,22+10,tColor);}	
}
void Dis_Detail(unsigned char tId)
{
//	char disBuf[15];
//	IdDetail=tId;
//	disBuf[0]=IdSlave[tId]/10+'0';
//	if(disBuf[0]=='0')disBuf[0]=' ';
//	disBuf[1]=IdSlave[tId]%10+'0';
//	disBuf[2]=0;
// 	lcd_text16(120, 80,RED,RED,disBuf);
//	DrawSetDat(tId);
//	DrawDiagLamp(tId);
}
void FyDat2Chr(void)
{
	unsigned int tDat1;
	unsigned char tii;
	tDat1=u16SavFyDat;
	u8SavFyBuf[0]=tDat1/100;
	tDat1=tDat1%100;
	u8SavFyBuf[1]=tDat1/10;
	tDat1=tDat1%10;
	u8SavFyBuf[2]=tDat1;
	for(tii=0;tii<3;tii++)
		u8SavFyBuf[tii]+=0x30;
	u8SavFyBuf[3]='\0';
	if(u8SavFyBuf[0]=='0')
	{
		if(u8SavFyBuf[1]=='0')
		{
			u8SavFyBuf[1]=' ';	
		}
			u8SavFyBuf[0]=' ';	
	}
}
void WrSetSel(unsigned char x,unsigned char y,char *s)
{
	//Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,WHITE);
	if(x==0)//0~9
	{
		//TFT_Fill(220,62+y*35,220+24,62+y*35+16,WHITE);
		TFT_Fill(220,62+y*35,220+24,62+y*35+16,WHITE);
		lcd_text16(220,62+y*35,0,0xffff,s);
		//Draw_Sel(200-4,58-4+y*35,56+8,22+8,tColor);
	}
	else if(x==1)//10~19
	{
		if(y!=8)
		{
			TFT_Fill(480,62+y*35,480+24,62+y*35+16,WHITE);
			lcd_text16(480,62+y*35,0,0xffff,s);
		}
		else
		{
			TFT_Fill(454,62+y*35,454+56,62+y*35+16,WHITE);
			lcd_text16(454,62+y*35,0,0xffff,s);
		}//Draw_Sel(460-4,58-4+y*35,56+8,22+8,tColor);
	}
	else if(x==2)
	{
		TFT_Fill(620+2,58+2+y*35,620+40,58+2+y*35+16,WHITE);
		lcd_text16(620+17,62+y*35,0,0xffff,s);	 //17=(42-8)/2
	}
}

extern unsigned int  ftaDIdat,ftaDIdat_pre;
extern unsigned char ftaDOdat,ftaDOdat_pre,wrFtaDoDat,FTAdat;
unsigned char tDoDat,tDoDat_pre;
void ReNewDIO(void)
{
	unsigned int  tDiDat,tdat;
	unsigned char i;
	tDiDat=ftaDIdat;tDoDat=FTAdat;//tDoDat=ftaDOdat;
	if((ftaDIdat!=ftaDIdat_pre)||(tDoDat!=tDoDat_pre))//if((ftaDIdat!=ftaDIdat_pre)||(ftaDOdat!=ftaDOdat_pre))
	{
		ftaDIdat_pre=ftaDIdat;
		for(i=0;i<12;i++)
		{
				tdat=1<<i;
				if((tDiDat&tdat)==tdat)
					LcdDrawCircle(515,76+(i*30),6,RED,1);
				else
					LcdDrawCircle(515,76+(i*30),6,BLUE,1);
		}
	//}
	//if(ftaDOdat!=ftaDOdat_pre)
	//{
		tDoDat_pre=tDoDat;
		for(i=0;i<8;i++)
		{
				tdat=1<<i;
				if((tDoDat&tdat)==tdat)
					LcdDrawCircle(715,76+(i*30),6,RED,1);
				else
					LcdDrawCircle(715,76+(i*30),6,BLUE,1);
		}
	
	}
}
void DrDIO(unsigned int diDat,unsigned char doDat)
{
	unsigned int  tDiDat,tdat;
	unsigned char tDoDat,i;
	tDiDat=diDat;tDoDat=doDat;
	for(i=0;i<12;i++)
	{
			tdat=1<<i;
			if((tDiDat&tdat)==tdat)
				LcdDrawCircle(515,76+(i*30),6,RED,1);
			else
				LcdDrawCircle(515,76+(i*30),6,BLUE,1);
	}
}
void Draw_DigNum(unsigned int x,unsigned int y)
{
unsigned int tColor;
	tColor=GRAY;
	TFT_DrawLine(x+13,y+10,x+18,y+5,tColor);	//TOP LINE
	TFT_DrawLine(x+18,y+5,x+54,y+5,tColor);	//TOP LINE
	TFT_DrawLine(x+54,y+5,x+59,y+10,tColor);	//TOP LINE
	TFT_DrawLine(x+50,y+19,x+59,y+10,tColor);	//TOP LINE
	TFT_DrawLine(x+50,y+19,x+22,y+19,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+10,x+22,y+19,tColor);	//TOP LINE
	
	TFT_DrawLine(x+ 6,y+17,x+11,y+12,tColor);	//TOP LINE
	TFT_DrawLine(x+ 6,y+17,x+ 6,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+69,x+ 6,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+69,x+20,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+20,y+21,x+20,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+20,y+21,x+11,y+12,tColor);	//TOP LINE
	
	TFT_DrawLine(x+23,y+63,x+16,y+70,tColor);	//TOP LINE
	TFT_DrawLine(x+23,y+63,x+49,y+63,tColor);	//TOP LINE
	TFT_DrawLine(x+56,y+70,x+49,y+63,tColor);	//TOP LINE
	TFT_DrawLine(x+56,y+70,x+49,y+77,tColor);	//TOP LINE
	TFT_DrawLine(x+23,y+77,x+49,y+77,tColor);	//TOP LINE
	TFT_DrawLine(x+23,y+77,x+16,y+70,tColor);	//TOP LINE
	
	TFT_DrawLine(x+66,y+17,x+61,y+12,tColor);	//TOP LINE
	TFT_DrawLine(x+66,y+17,x+66,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+59,y+69,x+66,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+59,y+69,x+52,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+52,y+21,x+52,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+52,y+21,x+61,y+12,tColor);	//TOP LINE
	
	TFT_DrawLine(x+ 6,y+123,x+11,y+128,tColor);	//TOP LINE
	TFT_DrawLine(x+ 6,y+123,x+ 6,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+ 71,x+ 6,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+ 71,x+20,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+20,y+119,x+20,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+20,y+119,x+11,y+128,tColor);	//TOP LINE
	
	TFT_DrawLine(x+66,y+123,x+61,y+128,tColor);	//TOP LINE
	TFT_DrawLine(x+66,y+123,x+66,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+59,y+ 71,x+66,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+59,y+ 71,x+52,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+52,y+119,x+52,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+52,y+119,x+61,y+128,tColor);	//TOP LINE
	
	TFT_DrawLine(x+13,y+130,x+18,y+135,tColor);	//TOP LINE
	TFT_DrawLine(x+18,y+135,x+54,y+135,tColor);	//TOP LINE
	TFT_DrawLine(x+54,y+135,x+59,y+130,tColor);	//TOP LINE
	TFT_DrawLine(x+50,y+121,x+59,y+130,tColor);	//TOP LINE
	TFT_DrawLine(x+50,y+121,x+22,y+121,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+130,x+22,y+121,tColor);	//TOP LINE
}

void Draw_DigNumS(unsigned int x,unsigned int y)
{
unsigned int tColor;
	tColor=GRAY;
	TFT_DrawLine(x+13,y+15,x+18,y+10,tColor);	//TOP LINE
	TFT_DrawLine(x+18,y+10,x+48,y+10,tColor);	//TOP LINE
	TFT_DrawLine(x+48,y+10,x+53,y+15,tColor);	//TOP LINE
	TFT_DrawLine(x+44,y+24,x+53,y+15,tColor);	//TOP LINE
	TFT_DrawLine(x+44,y+24,x+22,y+24,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+15,x+22,y+24,tColor);	//TOP LINE
	
	TFT_DrawLine(x+ 6,y+22,x+11,y+17,tColor);	//TOP LINE	 ////
	TFT_DrawLine(x+ 6,y+22,x+ 6,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+69,x+ 6,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+69,x+20,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+20,y+26,x+20,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+20,y+26,x+11,y+17,tColor);	//TOP LINE
	
	TFT_DrawLine(x+23,y+63,x+16,y+70,tColor);	//TOP LINE
	TFT_DrawLine(x+23,y+63,x+43,y+63,tColor);	//TOP LINE
	TFT_DrawLine(x+50,y+70,x+43,y+63,tColor);	//TOP LINE
	TFT_DrawLine(x+50,y+70,x+43,y+77,tColor);	//TOP LINE
	TFT_DrawLine(x+23,y+77,x+43,y+77,tColor);	//TOP LINE
	TFT_DrawLine(x+23,y+77,x+16,y+70,tColor);	//TOP LINE
	
	TFT_DrawLine(x+60,y+22,x+55,y+17,tColor);	//TOP LINE
	TFT_DrawLine(x+60,y+22,x+60,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+53,y+69,x+60,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+53,y+69,x+46,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+46,y+26,x+46,y+62,tColor);	//TOP LINE
	TFT_DrawLine(x+46,y+26,x+55,y+17,tColor);	//TOP LINE
	
	TFT_DrawLine(x+ 6,y+118,x+11,y+123,tColor);	//TOP LINE
	TFT_DrawLine(x+ 6,y+118,x+ 6,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+ 71,x+ 6,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+ 71,x+20,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+20,y+114,x+20,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+20,y+114,x+11,y+123,tColor);	//TOP LINE
	
	TFT_DrawLine(x+60,y+118,x+55,y+123,tColor);	//TOP LINE
	TFT_DrawLine(x+60,y+118,x+60,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+53,y+ 71,x+60,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+53,y+ 71,x+46,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+46,y+114,x+46,y+ 78,tColor);	//TOP LINE
	TFT_DrawLine(x+46,y+114,x+55,y+123,tColor);	//TOP LINE
	
	TFT_DrawLine(x+13,y+125,x+18,y+130,tColor);	//TOP LINE
	TFT_DrawLine(x+18,y+130,x+48,y+130,tColor);	//TOP LINE
	TFT_DrawLine(x+48,y+130,x+53,y+125,tColor);	//TOP LINE
	TFT_DrawLine(x+44,y+116,x+53,y+125,tColor);	//TOP LINE
	TFT_DrawLine(x+44,y+116,x+22,y+116,tColor);	//TOP LINE
	TFT_DrawLine(x+13,y+125,x+22,y+116,tColor);	//TOP LINE
}


void re_SSL(unsigned char id,float tfSSL)
{
//???,??Kg/s,??T/h,1Kg/s=3.6T/h
	unsigned char i;
	unsigned char tFlgSig=0;//?????
	unsigned long intSSL=0;
	unsigned int temp;
	char tSSL[8];
	char zsCH[8],xsCH[8];
	
//	delay(10);
	if(tfSSL<999.0)
	{
//		 intSSL=tfSSL*100;
		 intSSL=tfSSL*100;
	}
	temp=intSSL;
	tSSL[0]=temp/10000+0x30;
	   temp=temp%10000;
	tSSL[1]=temp/1000 +0x30;
	   temp=temp%1000;
	tSSL[2]=temp/100  +0x30;
	   temp=temp%100;
	tSSL[3]='.';
	tSSL[4]=temp/10   +0x30;
	   temp=temp%10;
	tSSL[5]=temp+0x30;
	tSSL[6]='\0';
	tFlgSig=0;
	for(i=0;i<3;i++)
	{
		if(tFlgSig==0)
		{
		 	if((tSSL[i]=='0')&&(i!=2))
			{tSSL[i]=' ';}
			else
			{tFlgSig=1;  }
		}
	}
	for(i=0;i<4;i++)
		zsCH[i]=tSSL[i];
	zsCH[4]=0;
	for(i=0;i<2;i++)
		xsCH[i]=tSSL[4+i];
	xsCH[2]=0;
	if(id==0)
	{
		//TFT_Fill(15,270,15+16*6-8,270+32,WHITE);
		//TFT_Fill(415,270,415+16*6-8,270+32,WHITE);
		DisNum24(15,270,BLUE,WHITE,tSSL);
		DisNum24(415,270,BLUE,WHITE,tSSL);
		//if(tfSSL<10.0)
		//{
		//lcd_text32( 15+24,270,BLUE,WHITE,xsCH);
		//lcd_text32(415+24,270,BLUE,WHITE,xsCH);
		//}
		//else
		//{
		//if(tfSSL<)
		//}	
		//lcd_text32(415,270,BLUE,WHITE,zsCH);
	}
  if(id==1)
	{
		//TFT_Fill(15,340,15+16*6-8,340+32,WHITE);
		//TFT_Fill(415,340,415+16*6-8,340+32,WHITE);
		DisNum24(15,340,BLUE,WHITE,tSSL);
		//lcd_text32(15,340,BLUE,WHITE,zsCH);
		//lcd_text32(15+56,340,BLUE,WHITE,xsCH);
		DisNum24(415,340,BLUE,WHITE,tSSL);
		//lcd_text32(415,340,BLUE,WHITE,zsCH);
		//lcd_text32(415+56,340,BLUE,WHITE,xsCH);
	}
  if(id==2)
	{
		//TFT_Fill(35,315,35+16*6-8,315+32,WHITE);
		DisNum24(35,315,BLUE,WHITE,tSSL);
		//lcd_text32(35,315,BLUE,WHITE,zsCH);
		//lcd_text32(35+56,315,BLUE,WHITE,xsCH);
		//lcd_text32(35,315-3,BLACK,WHITE,tSSL);
	}
	if(id==3)
	{
		//TFT_Fill(375,315,375+16*6-8,315+32,WHITE);
		DisNum24(375,315,BLUE,WHITE,tSSL);
		//lcd_text32(375,315,BLUE,WHITE,zsCH);
		//lcd_text32(375+56,315,BLUE,WHITE,xsCH);
		//lcd_text32(375,315-3,BLACK,WHITE,tSSL);
	}
	}


void re_LJS(char id,double tfSSL)
{
	unsigned char chrXS;
	unsigned char i;
	unsigned char tFlgSig=0;//前导零删除
	float         flXS;
	unsigned long temp;
	char tSSL[15];

	temp=tfSSL;
	flXS=(tfSSL-temp);//+0.05;
	chrXS=flXS*10;
	tSSL[0]=temp/1000000000+0x30;
	temp=(unsigned long)(temp%1000000000);
	tSSL[1]=temp/100000000+0x30;
	temp=temp%100000000;
	tSSL[2]=temp/10000000+0x30;
	temp=temp%10000000;
	tSSL[3]=temp/1000000+0x30;
	temp=temp%1000000;
	tSSL[4]=temp/100000+0x30;
	temp=temp%100000;
	tSSL[5]=temp/10000+0x30;
	temp=temp%10000;
	tSSL[6]=temp/1000+0x30;
	temp=(unsigned long)(temp%1000);
	tSSL[7]=temp/100+0x30;
	temp=temp%100;
	tSSL[8]=temp/10+0x30;
	temp=temp%10;
	tSSL[9]=temp+0x30;
	tSSL[10]='.';
	tSSL[11]=chrXS+0x30;
	tSSL[12]='\0';
	tFlgSig=0;
	for(i=0;i<9;i++)
	{
		if(tFlgSig==0)
		{
		 	if(tSSL[i]=='0')
			{
			  tSSL[i]=' ';
			}
			else
				tFlgSig=1;
		}
	}
	//ShowStr30(177,312,tSSL,11,BLACK,WHITE);
	//lcd_text24(177,312,BLACK,WHITE,tSSL);
	if(id==0)//mon 体积
	{
	  /*TFT_Fill(107,270,107+16*11,270+32,WHITE);*/DisNum24(127,270,BLUE,WHITE,tSSL);
	  /*TFT_Fill(507,270,507+16*11,270+32,WHITE);*/DisNum24(527,270,BLUE,WHITE,tSSL);
	}
  if(id==1)//mon 质量
	{
	  /*TFT_Fill(107,340,107+16*11,340+32,WHITE);*/DisNum24(127,340,BLUE,WHITE,tSSL);
	  /*TFT_Fill(507,340,507+16*11,340+32,WHITE);*/DisNum24(527,340,BLUE,WHITE,tSSL);
	
	}
	if(id==2)//main 体积
	{/*TFT_Fill(170,315,170+16*11,315+32,WHITE);*/DisNum24(170,315,BLUE,WHITE,tSSL);}
	if(id==3)//main 体积
	{/*TFT_Fill(510,315,510+16*11,315+32,WHITE);*/DisNum24(510,315,BLUE,WHITE,tSSL);}
	
}


void re_MD(unsigned char id,float tfSSL)
{
	unsigned char i;
	unsigned char tFlgSig=0;//?????
	unsigned long intSSL=0;
	unsigned int temp;
	char tSSL[8];
//	delay(10);
	if(tfSSL<900.0)
	{
		 intSSL=tfSSL*10000;	 //900000
	}
	temp=intSSL;
	tSSL[0]=temp/10000+0x30;
	temp=temp%10000;
	tSSL[1]='.';
	tSSL[2]=temp/1000+0x30;
	temp=temp%1000;
	tSSL[3]=temp/100+0x30;
	temp=temp%100;
	tSSL[4]=temp/10+0x30;
	temp=temp%10;
	tSSL[5]=temp+0x30;
	tSSL[6]=0;

//	tFlgSig=0;
//	for(i=0;i<2;i++)
//	{
//		if(tFlgSig==0)
//		{
//		 	if((tSSL[i]=='0')&&(i!=1))
//			{
//			  tSSL[i]=' ';
//			}
//			else
//			{
//				tFlgSig=1;
//			}
//		}
//	}
	if(id==0)
	{
		/*TFT_Fill(300,270,300+16*6-8,270+32,WHITE);*/DisNum24(310,270,BLUE,WHITE,tSSL);
		/*TFT_Fill(700,270,700+16*6-8,270+32,WHITE);*/DisNum24(710,270,BLUE,WHITE,tSSL);
	}
	if(id==2)
	{
		/*TFT_Fill(62,169,62+12*6,169+24,WHITE);*/DisNum24(62,169,BLUE,WHITE,tSSL);
	}
}

void re_WD(unsigned char id,float tfSSL)
{
	unsigned char i;
	unsigned char tFlgSig=0;//前导零删除
	unsigned char tFlgFS=0;//负数标志
	unsigned int intSSL=0;
	unsigned int temp;
	char tSSL[8];
//	delay(10);
	tFlgFS=0;
	if(tfSSL<0.0)
	{	tFlgFS=1;tfSSL=-1.0*tfSSL;}
	if(tfSSL<600.0)
	{
	 intSSL=tfSSL*100;
	 //intSSL+=5;
	 intSSL/=10;
	}
	temp=intSSL;
	tSSL[0]=temp/10000+0x30;
	temp=temp%10000;
	tSSL[1]=temp/1000+0x30;
	temp=temp%1000;
	tSSL[2]=temp/100+0x30;
	temp=temp%100;
	tSSL[3]=temp/10+0x30;
	temp=temp%10;
	tSSL[4]='.';
	tSSL[5]=temp+0x30;
	tSSL[6]='\0';
	tFlgSig=0;
	for(i=0;i<3;i++)
	{
		if(tFlgSig==0)
		{
		 	if(tSSL[i]==0x30)
			{  tSSL[i]=' ';	}
			else
			{
				tFlgSig=1;
			}
		}
	}
	if(tFlgFS==1)
	{
		//if(i>0)
			tSSL[1]='-';
	}
	//lcd_text24(297,342,BLACK,WHITE,tSSL);
	if(id==0)
	{	
		/*TFT_Fill(297,340,297+16*6-8,340+32,WHITE);*/DisNum24(297,340,BLUE,WHITE,tSSL);
		/*TFT_Fill(697,340,697+16*6-8,340+32,WHITE);*/DisNum24(697,340,BLUE,WHITE,tSSL);
	}
	if(id==2)
	{
		/*TFT_Fill(62,209-5,62+16*6-8,209-5+32,WHITE);*/DisNum24( 62,209-5,BLUE,WHITE,tSSL);
	}
	}


void Dis_Mon(unsigned char id)
{
unsigned int tx;
unsigned int fc,bc;
fc=0xFFFF;
bc=GRAY;
tx=(id==0)?0:400;
			TFT_Fill(tx+  6, 34,tx+ 54, 105,BLUE);//	
			//TFT_Fill(tx+  6,377,tx+ 55,430,BLUE);//	
			TFT_Fill(tx+360, 37,tx+396, 64,GRAY);//应发量单位底色	
			
			TFT_Fill(tx+ 79,235,tx+105,265,GRAY);//瞬时量单位底色	
			TFT_Fill(tx+ 84,305,tx+105,335,GRAY);//瞬时量单位底色	
			
			TFT_Fill(tx+270,235,tx+294,265,GRAY);//累积数单位底色	
			TFT_Fill(tx+270,305,tx+294,335,GRAY);//累积数单位底色	
			
			TFT_Fill(tx+347,235,tx+395,265,GRAY);//密度单位底色	
			TFT_Fill(tx+347,305,tx+395,335,GRAY);//温度单位底色	
			//Draw_Line(tx+88,  35,tx+88,105,2,BLACK);
			Draw_Line(tx+105, 235,tx+105,375,2,BLACK);
			Draw_Line(tx+295, 235,tx+295,375,2,BLACK);
			//提货单号
			//ShowHz(tx+ 60, 40,0);
			//ShowHz(tx+ 60+20, 40,1);
			//ShowHz(tx+ 60+40, 40,2);
			//ShowHz(tx+ 60+60, 40,3);
			//应发量
			lcd_text24(tx+210,40,0,0xFFFF,"应发量");
      //ShowHz(tx+210, 40,4);
			//ShowHz(tx+210+20, 40,5);
			//ShowHz(tx+210+40, 40,6);
			lcd_text16(tx+370,42,fc,bc,"L");

			//瞬时量v
			lcd_text24(tx+6,240,0,0xFFFF,"瞬时量");
			//ShowHz(tx+ 10, 240,7);
			//ShowHz(tx+ 10+20, 240,8);
			//ShowHz(tx+ 10+40, 240,6);
			lcd_text16(tx+ 75+5,242-6,fc,bc,"m{");
			lcd_text16(tx+ 75+22,242+6,fc,bc,"h");
			Draw_Line(tx+101,244,tx+91,260,1,fc);
			//ShowStr30(tx+15,267,"823.45",6,BLACK,WHITE);
      //lcd_text32(tx+15,267,0,0xFFFF,"823.45");
      //LcdDisplayStringEN("     0.0",tx+5,100,0,0xFFFF,0);
			//瞬时量m
			lcd_text24(tx+6,310,0,0xFFFF,"瞬时量");
			//ShowHz(tx+ 10, 310,7);
			//ShowHz(tx+ 10+20, 310,8);
			//ShowHz(tx+ 10+40, 310,6);
			lcd_text16(tx+75+13,312-6,fc,bc,"t");
			lcd_text16(tx+75+22,312+6,fc,bc,"h");
			Draw_Line(tx+101,314,tx+91,330,1,fc);
			//ShowStr30(tx+15,267,"823.45",6,BLACK,WHITE);
			//累积数v
			lcd_text24(tx+115,240,0,0xFFFF,"累积体积");
			//ShowHz(tx+115, 240,9);
			//ShowHz(tx+115+20, 240,10);
			//ShowHz(tx+115+40, 240,11);
			lcd_text16(tx+280,242,fc,bc,"L");
			//ShowStr30(tx+205,267,"8234567890.45",12,BLACK,WHITE);

			//累积数m
			lcd_text24(tx+115,310,0,0xFFFF,"累积质量");
			//ShowHz(tx+115, 310,9);
			//ShowHz(tx+115+20, 310,10);
			//ShowHz(tx+115+40, 310,11);
			lcd_text16(tx+270+8,312,fc,bc,"Kg");
			//密度
			lcd_text24(tx+297,240,0,0xFFFF,"密度");
			//ShowHz(tx+297, 240,12);
			//ShowHz(tx+297+20, 240,13);
			lcd_text16(tx+347+8,242-6,fc,bc,"g");
			lcd_text16(tx+347+22,242+6,fc,bc,"cm{");
			Draw_Line(tx+347+24+5,244,tx+350+5,260,1,fc);
			//ShowStr30(tx+10,337,"1.2345",6,BLACK,WHITE);
      
			//温度
			lcd_text24(tx+297,310,0,0xFFFF,"温度");
			//ShowHz(tx+297, 310,16);
			//ShowHz(tx+297+20, 310,13);
			lcd_text16(tx+297+50+12,312,fc,bc,"~C");
 			
			TFT_Fill(tx+6,404,tx+134,432,BLUE);	 //车号
			//TFT_Fill(tx+137,404,tx+264,429,BLUE);//铅封号
			//Draw_EN24(tx+10,405,WHITE,BLUE,chrCarId[id]);//GREEN
			//Draw_EN24(tx+140,405,WHITE,BLUE,chrLeadId[id]);//GREEN
			Draw_Line(tx+135, 375,tx+135,431,2,BLACK);
			//Draw_Line(tx+264, 375,tx+264,431,2,BLACK);
			//Draw_Line(tx+264, 375,tx+264,431,2,BLACK);
			Draw_Line(tx+6,   403,tx+394,403,2,BLACK);
			lcd_text16(tx+143,383,BLACK,WHITE,"静电");
			lcd_text16(tx+185,383,BLACK,WHITE,"溢油");
			lcd_text16(tx+227,383,BLACK,WHITE,"主开");
			lcd_text16(tx+269,383,BLACK,WHITE,"主关");
			lcd_text16(tx+311,383,BLACK,WHITE,"副开");
			lcd_text16(tx+353,383,BLACK,WHITE,"副关");
			if(id==0)
 			{
        DisNum24(tx+20,46,WHITE,BLUE,"1");//GREEN
			}	
			else
      { 
				DisNum24(tx+20,46,WHITE,BLUE,"2");//GREEN
      }			
}
void Dis_Pwd(void)
{
	char tDisChr[10];
	unsigned char ti;
	unsigned char tj;
		
	//SPI2_Flash_Read(SavUsr[0],0,32);
	//SPI2_Flash_Read(SavPwd[0],33,48);
	ReadUsrPwd();

	for(ti=0;ti<8;ti++)
	{
		for(tj=0;tj<7;tj++)
		{tmpSavPwd[ti][tj]=SavPwd[ti][tj];}
	}
	for(ti=0;ti<8;ti++)
	{
		for(tj=0;tj<5;tj++)
		{tmpSavUsr[ti][tj]=SavUsr[ti][tj];}
	}

	for(ti=0;ti<8;ti++)
	{
		for(tj=0;tj<5;tj++)
		{	
			tDisChr[tj]=tmpSavUsr[ti][tj];
		}
		lcd_text24(152,85+ti*40,BLACK,WHITE,tDisChr);//GREEN
		//ShowStr30(152,81+ti*40,tDisChr,4,BLACK,WHITE);
		for(tj=0;tj<8;tj++)
		{	
			tDisChr[tj]=tmpSavPwd[ti][tj];
		}
		lcd_text24(302,85+ti*40,BLACK,WHITE,tDisChr);//GREEN
		//ShowStr30(302,81+ti*40,tDisChr,6,BLACK,WHITE);
	}
}

void DisPwdSel(unsigned char x,unsigned char y,unsigned int tColor)
{
	if(IndexId==0)//usr
	{
		Draw_Sel(137,97-20+x*40,106,40,tColor);
	}
	else if(IndexId==1)//pwd
	{
		Draw_Sel(287,97-20+y*40,206,40,tColor);
	}
	else if(IndexId==2)
	{
		Draw_Sel(547,97-20,86,41,tColor);
	}
	else if(IndexId==3)
	{
		Draw_Sel(647,97-20,86,41,tColor);
	}	
	
}
//发油参数设置，AO阀为0,AB阀为1
//AO阀为4~20mA控制，DA输出连续控制
//AB阀为开关控制，A阀为进气阀，正逻辑，得电进气;B阀为排气阀,反逻辑，失电排气
//
void Frm_Fy(unsigned char ubVer)
{
	TFT_Fill(0,50,799,411,0xffff);	
	switch(ubVer)
	{
	case 0:
//			Draw_Cap(70,55+0*35,64,30,"开关速率",4);
			Draw_Cap(70,55+0*35,64,30,"脉冲宽度",4);
			lcd_text16(140,62+0*35,0,0xFFFF,"[1-099]");
			Draw_TextBox(200,58+0*35,56,22);

//			Draw_Cap(70,55+1*35,64,30,"调节速率",4);
			Draw_Cap(70,55+1*35,64,30,"脉冲周期",4);
			lcd_text16(140,62+1*35,0,0xFFFF,"[1-099]");
			Draw_TextBox(200,58+1*35,56,22);

			Draw_Cap(70,55+2*35,64,30,"调节速率",4);//调节宽度
//			Draw_Cap(70,55+2*35,64,30,"开稳流量",4);
			lcd_text16(140,62+2*35,0,0xFFFF,"[1-500]");
			Draw_TextBox(200,58+2*35,56,22);

			Draw_Cap(70,55+3*35,64,30,"稳流流量",4);
			lcd_text16(140,62+3*35,0,0xFFFF,"[1-500]");
			Draw_TextBox(200,58+3*35,56,22);

			Draw_Cap(70,55+4*35,64,30,"最大流量",4);
			lcd_text16(140,62+4*35,0,0xFFFF,"[1-500]");
			Draw_TextBox(200,58+4*35,56,22);

			Draw_Cap(70,55+5*35,64,30,"流量偏差",4);
			lcd_text16(140,62+5*35,0,0xFFFF,"[0-030]");
			Draw_TextBox(200,58+5*35,56,22);

			Draw_Cap(70,55+6*35,64,30,"提前量  ",4);
			lcd_text16(140,62+6*35,0,0xFFFF,"[5-099]");
			Draw_TextBox(200,58+6*35,56,22);

			Draw_Cap(70,55+7*35,64,30,"过冲量  ",4);
			lcd_text16(140,62+7*35,0,0xFFFF,"[0-099]");
			Draw_TextBox(200,58+7*35,56,22);
	break;
	case 1:
			Draw_Cap(40,55+0*35,64,30,"开阀速率",4); //	""脉冲宽度
			lcd_text16(110,62+0*35,0,0xFFFF,"[1-200]");
			Draw_TextBox(200,58+0*35,56,22);

			Draw_Cap(40,55+1*35,64,30,"关阀速率",4);//""脉冲周期
			lcd_text16(110,62+1*35,0,0xFFFF,"[1-200]");
			Draw_TextBox(200,58+1*35,56,22);

			Draw_Cap(40,55+2*35,64,30,"调节速率",4);//""调节宽度
			lcd_text16(110,62+2*35,0,0xFFFF,"[1-200]");
			Draw_TextBox(200,58+2*35,56,22);

			Draw_Cap(40,55+3*35,64,30,"开稳流量",4);//"" 调节周期
			lcd_text16(110,62+3*35,0,0xFFFF,"[1-500]");
			Draw_TextBox(200,58+3*35,56,22);

			Draw_Cap(40,55+4*35,64,30,"关稳流量",4);//"" 稳流流量
			lcd_text16(110,62+4*35,0,0xFFFF,"[1-500]");
			Draw_TextBox(200,58+4*35,56,22);

			Draw_Cap(40,55+5*35,64,30,"最大流量",4);
			lcd_text16(110,62+5*35,0,0xFFFF,"[1-500]");
			Draw_TextBox(200,58+5*35,56,22);

			Draw_Cap(40,55+6*35,64,30,"流量偏差",4);
			lcd_text16(110,62+6*35,0,0xFFFF,"[0-099]");
			Draw_TextBox(200,58+6*35,56,22);

			Draw_Cap(40,55+7*35,64,30,"提前量  ",4);
			lcd_text16(110,62+7*35,0,0xFFFF,"[5-999]");
			Draw_TextBox(200,58+7*35,56,22);
			lcd_text16(265,62+7*35,0,0xFFFF,"*10");

			Draw_Cap(40,55+8*35,64,30,"过冲量  ",4);
			lcd_text16(110,62+8*35,0,0xFFFF,"[0-099]");
			Draw_TextBox(200,58+8*35,56,22);
	break;
	}
//			lcd_text16(260,58+0*35,BLUE,0xFFFF,"%/s");
//			lcd_text16(260,58+1*35,BLUE,0xFFFF,"0.1%/s");
//			lcd_text16(260,58+2*35,BLUE,0xFFFF,"kg/s");
//			lcd_text16(260,58+3*35,BLUE,0xFFFF,"kg/s");
//			lcd_text16(260,58+4*35,BLUE,0xFFFF,"kg/s");
//			lcd_text16(260,58+5*35,BLUE,0xFFFF,"x%");
//			lcd_text16(260,58+6*35,BLUE,0xFFFF,"kg");
//			lcd_text16(260,58+7*35,BLUE,0xFFFF,"kg");
			Draw_Cap(320,55+0*35,64,30,"开稳时间",5);
			lcd_text16(395,62+0*35,0,0xffff,"[ 0-099]");
			Draw_TextBox(460,58+0*35,56,22);

			Draw_Cap(320,55+1*35,64,30,"先开时间",5);
			lcd_text16(395,62+1*35,0,0xFFFF,"[ 0-099]");
			Draw_TextBox(460,58+1*35,56,22);

			Draw_Cap(320,55+2*35,64,30,"后关时间",5);
			lcd_text16(395,62+2*35,0,0xFFFF,"[ 0-099]");
			Draw_TextBox(460,58+2*35,56,22);

			Draw_Cap(320,55+3*35,64,30,"小信号切除",5);
			lcd_text16(395,62+3*35,0,0xFFFF,"[ 0- 30]");
			Draw_TextBox(460,58+3*35,56,22);

			Draw_Cap(320,55+4*35,64,30,"关阀超时",5);
			lcd_text16(395,62+4*35,0,0xFFFF,"[ 0-099]");
			Draw_TextBox(460,58+4*35,56,22);		

			Draw_Cap(320,55+5*35,64,30,"开阀超时",5);
			lcd_text16(395,62+5*35,0,0xFFFF,"[ 0-099]");
			Draw_TextBox(460,58+5*35,56,22);

			Draw_Cap(320,55+6*35,64,30,"流量计超时",5);
			lcd_text16(395,62+6*35,0,0xFFFF,"[ 0-999]");
			Draw_TextBox(460,58+6*35,56,22);

			Draw_Cap(320,55+7*35,64,30,"背光调节",5);
			lcd_text16(395,62+7*35,0,0xFFFF,"[80-120]");
			Draw_TextBox(460,58+7*35,56,22);

			Draw_Cap(320,55+8*35,64,30,"有效容积",5);	//有效容积限制了最大应发量
			lcd_text16(375,62+8*35,0,0xFFFF,"[<999999]");
			Draw_TextBox(450,58+8*35,66,22);
			//lcd_text16(454,62+8*35,0,0xFFFF,"1234567");

			Draw_Button_Cap(600,58+0*35,120,22,"保  存",3);
			Draw_Button_Cap(600,58+1*35,120,22,"缺  省",3);
			Draw_Button_Cap(600,58+2*35,120,22,"返  回",3);
			Draw_Button_Cap(600,58+3*35,120,22,"  AMS ",3);
}	


void Dis_Conf(void)
{
unsigned char ti;
unsigned char tj;
unsigned int  tInt;
char tDisBuf[10];
//char dSavDat[42];
EepRd(234,2);
tInt=EepRdBuf[0];
tInt=(tInt<<8)+EepRdBuf[1];
if((tInt>=pipeSavConfDatMin)&&(tInt<=pipeSavConfDatMax))
{ pipeSavConfDat=tInt;}
else
{ pipeSavConfDat=80;}
tInt=pipeSavConfDat;
tmppipeSavConf[0]=(tInt/100)+'0';
tInt=tInt%100;
tmppipeSavConf[1]=(tInt/10)+'0';
tInt=tInt%10;
tmppipeSavConf[2]=(tInt)+'0';
tmppipeSavConf[3]=0;
for(tj=0;tj<4;tj++)
	pipeSavConf[tj]=tmppipeSavConf[tj];
if(pipeSavConf[0]=='0')	pipeSavConf[0]=' ';
WrConfSel(1,9,pipeSavConf);
EepRd(220,4);
plzSavConfDat[0]=(EepRdBuf[0]<<8)+EepRdBuf[1];
plzSavConfDat[1]=(EepRdBuf[2]<<8)+EepRdBuf[3];

u16plzSavConfDat=plzSavConfDat[0];
//u16plzSavConfDat=(unsigned int)(rtMD*10000);//
ConfDat2ChrMd();
for(tj=0;tj<5;tj++)
{	
	   plzSavConf[0][tj]=u8plzSavConfBuf[tj];
	tmpplzSavConf[0][tj]=u8plzSavConfBuf[tj];
}
WrConfSel(1,7,u8plzSavConfBuf);

u16plzSavConfDat=plzSavConfDat[1];
ConfDat2ChrPlz();
for(tj=0;tj<5;tj++)
{	
	   plzSavConf[1][tj]=u8plzSavConfBuf[tj];
	tmpplzSavConf[1][tj]=u8plzSavConfBuf[tj];
}
WrConfSel(1,8,u8plzSavConfBuf);

EepRd(194,17);
for(ti=0;ti<17;ti++)
{
   //dSavDat[ti]=EepRdBuf[ti];
SavConfDat[ti]=EepRdBuf[ti];
}
EepRd(224,5);//关阀参数
for(ti=0;ti<5;ti++)
{
	if((EepRdBuf[ti]<=dnSavConfDatMax[ti])&&(EepRdBuf[ti]>=dnSavConfDatMin[ti]))
	{
		 dnSavConfDat[ti]=EepRdBuf[ti];
	}
}
if(dnSavConfDat[3]<dnSavConfDat[1])
{
  dnSavConfDat[3]=dnSavConfDat[1];
}
for(ti=0;ti<5;ti++)
{
	u8dnSavConfBuf[0]=(dnSavConfDat[ti]/10)+'0';
	u8dnSavConfBuf[1]=(dnSavConfDat[ti]%10)+'0';
	tDisBuf[0]=' ';tDisBuf[1]=' ';tDisBuf[2]=' ';tDisBuf[5]='\0';
	tDisBuf[3]=u8dnSavConfBuf[0];tDisBuf[4]=u8dnSavConfBuf[1];
	tmpdnSavConf[ti][0]=u8dnSavConfBuf[0];tmpdnSavConf[ti][1]=u8dnSavConfBuf[1];tmpdnSavConf[ti][2]=0;
	WrConfSel(1,ti,tDisBuf);	
}



EepRd(210,1);
tmpSavConfWarn=EepRdBuf[0];
//sys_Conn=(tmpSavConfWarn&0x40)>>6;
//为了保证与以前的兼容
WarnOnOff[0]=(tmpSavConfWarn&0x20)>>5;//静电
WarnOnOff[1]=(tmpSavConfWarn&0x10)>>4;//溢流
WarnOnOff[2]=(tmpSavConfWarn&0x08)>>3;//流量计
WarnOnOff[3]=(tmpSavConfWarn&0x04)>>2;//无流量
WarnOnOff[4]=(tmpSavConfWarn&0x02)>>1;//泄漏
WarnOnOff[5]=(tmpSavConfWarn&0x01);		//汽化
EepRd(230,1);
tmpSavConfWarn=EepRdBuf[0];
WarnOnOff[6]=(tmpSavConfWarn&0x80)>>7; //低流量
WarnOnOff[7]=(tmpSavConfWarn&0x40)>>6; //低压力
WarnOnOff[8]=(tmpSavConfWarn&0x20)>>5; //AMS
WarnOnOff[9]=(tmpSavConfWarn&0x10)>>4; //自调整
//WarnOnOff[10]=(tmpSavConfWarn&0x08)>>3; //本地/联网

for(ti=0;ti<10;ti++)
{
	tmpConfWarn[ti]=WarnOnOff[ti];	
//	if(frmNum==FRM_CONFIG)
	{
		if(WarnOnOff[ti]==0)
		{
			ClrConfSel(2,ti);
			WrConfSel(2,ti,"N");
		}
		else
		{
			ClrConfSel(2,ti);
			WrConfSel(2,ti,"Y");
		}
	}
}

if(sys_Conn)
{	WrConfSel(2,10,"联网");WarnOnOff[10]=1;tmpConfWarn[10]=1;}
else
{	WrConfSel(2,10,"本地");WarnOnOff[10]=0;tmpConfWarn[10]=0;}

	for(ti=0;ti<10;ti++)
	{
		u16SavConfDat=SavConfDat[ti];
		ConfDat2Chr();
		for(tj=0;tj<4;tj++)
		{	
			   SavConf[ti][tj]=u8SavConfBuf[tj];
			tmpSavConf[ti][tj]=u8SavConfBuf[tj];
		}
		if(frmNum==FRM_CONFIG)
			WrConfSel(0,ti,u8SavConfBuf);
	}
 tmpSavConfDate[0][0]=(sysY/10)+0x30;
 tmpSavConfDate[0][1]=(sysY%10)+0x30;
 tmpSavConfDate[0][2]=(sysM/10)+0x30;
 tmpSavConfDate[0][3]=(sysM%10)+0x30;
 tmpSavConfDate[0][4]=(sysD/10)+0x30;
 tmpSavConfDate[0][5]=(sysD%10)+0x30;
 for(ti=0;ti<7;ti++)
 	u8SavConfBuf[ti]=tmpSavConfDate[0][ti];
 u8SavConfBuf[6]='\0';
	if(frmNum==FRM_CONFIG)
		WrConfSel(1,5,u8SavConfBuf);
 tmpSavConfDate[1][0]=(sysHH/10)+0x30;
 tmpSavConfDate[1][1]=(sysHH%10)+0x30;
 tmpSavConfDate[1][2]=(sysMM/10)+0x30;
 tmpSavConfDate[1][3]=(sysMM%10)+0x30;
 tmpSavConfDate[1][4]=(sysSS/10)+0x30;
 tmpSavConfDate[1][5]=(sysSS%10)+0x30;
 for(ti=0;ti<7;ti++)
 	u8SavConfBuf[ti]=tmpSavConfDate[1][ti];
 u8SavConfBuf[6]='\0';
	if(frmNum==FRM_CONFIG)
		WrConfSel(1,6,u8SavConfBuf);
}

extern unsigned char xpos,ypos;
extern void DrSetSel(unsigned char x,unsigned char y,unsigned char id);
extern char disAddrBuf[3];
extern unsigned char AddrIfix,AddrIfix_pre;
extern unsigned char sysSavDat[10];
void Dis_Scr(unsigned char pgNum)
{
	unsigned int i=0;
	unsigned char dx;
	unsigned char tx;
	unsigned char ti;
//	unsigned char tempDat;
	char tDisBuf[10];
	//char *tStr;
	__disable_irq();
	//LCD_Init();
	//Frm_Init();
	dx=10;
	///flgManCon=0;			lcd_text16(430,10,0,GOLD,"AWz-SDHR005");

	switch(pgNum)
	{//2016年0107第二次修改
		case FRM_CALC:
			Draw_Line(185, 0,185,480,2,BLACK);
		  Draw_Rect(200,40,100,100,BLACK);
		  Draw_Rect(315,40,100,100,BLACK);
	  	Draw_Rect(430,40,100,100,BLACK);
			Draw_Rect(200,150,100,100,BLACK);
			Draw_Rect(315,150,100,100,BLACK);
			Draw_Rect(430,150,100,100,BLACK);
			Draw_Rect(545,150,100,100,BLACK);
			Draw_Rect(200,260,100,100,BLACK);
			Draw_Rect(315,260,100,100,BLACK);
	  	Draw_Rect(430,260,100,100,BLACK);
		  Draw_Rect(545,260,100,100,BLACK);
		  Draw_Rect(200,370,100,100,BLACK);
		  Draw_Rect(315,370,100,100,BLACK);
		  Draw_Rect(430,370,100,100,BLACK);
	  	Draw_Rect(545,370,100,100,BLACK);
		  Draw_Rect(545,40,100,100,BLACK);
		//Draw_Rect(50,10,80,460,BLACK);//第四次修改17，2.26
	  lcd_text16( 250,90,BLACK,WHITE,"*");
		lcd_text16( 365,90,BLACK,WHITE,"-");
		lcd_text16( 480,90,BLACK,WHITE,"+");
		lcd_text16( 595,90,BLACK,WHITE,"/");
		lcd_text16( 250,200,BLACK,WHITE,"1");
		lcd_text16( 365,200,BLACK,WHITE,"4");
		lcd_text16( 480,200,BLACK,WHITE,"7");
		lcd_text16( 595,200,BLACK,WHITE,"=");
		lcd_text16( 250,310,BLACK,WHITE,"2");
		lcd_text16( 365,310,BLACK,WHITE,"5");
		lcd_text16( 480,310,BLACK,WHITE,"8");
		lcd_text16( 595,310,BLACK,WHITE,"0");
		lcd_text16( 250,420,BLACK,WHITE,"3");
		lcd_text16( 365,420,BLACK,WHITE,"6");
		lcd_text16( 480,420,BLACK,WHITE,"9");
		lcd_text16( 595,420,BLACK,WHITE,"C");
		break;
		case FRM_LOGIN:
			//TFT_Fill(30,50,799,401,0xffff);	
			TFT_Fill(0,35,799,37,0x0000);	
			//TFT_Fill(790,0,799,479,0x0000);	
			Draw_TextBox(150,100,200,45);
			Draw_TextBox(500,100,200,45);
			Draw_Sel(146,96,208,53,RED);
			lcd_text24(100-20,111-3,BLUE,WHITE,"用户");	
			lcd_text24(450-20,111-3,BLUE,WHITE,"密码");

			if(flgAMT==1)//从主界面切换到手动模式
			{
				lcd_text16( 87+122,200,0,0xFFFF,"提示:请输入密码进入手动模式,或返回自动模式.");	
			}
			textPos=0;flgLogEdit=0;
			Draw_Sel(146,96,208,53,RED);
			Draw_Sel(496,96,208,53,WHITE);
		break;
		case FRM_PWD:
 			//TFT_CLEAR(0xFFFF);
			TFT_Fill(30,50,799,401,0xffff);	
			TFT_Fill(30,50,799,401,0xffff);	
			TFT_Fill(0,50,799,53,0x0000);	
			TFT_Fill(790,0,799,479,0x0000);	
			tx=40;
			Draw_Cap(140,50,100,30,"四位工号",4);
			Draw_Cap(290,50,200,30,"六位密码",4);

			Draw_TextBox(140,100-20+0*tx,100,34);
			Draw_TextBox(140,100-20+1*tx,100,34);
			Draw_TextBox(140,100-20+2*tx,100,34);
			Draw_TextBox(140,100-20+3*tx,100,34);
			Draw_TextBox(140,100-20+4*tx,100,34);
			Draw_TextBox(140,100-20+5*tx,100,34);
			Draw_TextBox(140,100-20+6*tx,100,34);
			Draw_TextBox(140,100-20+7*tx,100,34);
		
			Draw_TextBox(290,100-20+0*tx,200,34);
			Draw_TextBox(290,100-20+1*tx,200,34);
			Draw_TextBox(290,100-20+2*tx,200,34);
			Draw_TextBox(290,100-20+3*tx,200,34);
			Draw_TextBox(290,100-20+4*tx,200,34);
			Draw_TextBox(290,100-20+5*tx,200,34);
			Draw_TextBox(290,100-20+6*tx,200,34);
			Draw_TextBox(290,100-20+7*tx,200,34);
																				
			Draw_Cap(10,100-20+0*40,150,34,"发货作业1",4);
			Draw_Cap(10,100-20+1*40,150,34,"发货作业2",4);
			Draw_Cap(10,100-20+2*40,150,34,"发货作业3",4);
			Draw_Cap(10,100-20+3*40,150,34,"发货作业4",4);
			Draw_Cap(10,100-20+4*40,150,34,"组态参数",4);
			Draw_Cap(10,100-20+5*40,150,34,"仪表参数",4);
			Draw_Cap(10,100-20+6*40,150,34,"测试界面",4);
			Draw_Cap(10,100-20+7*40,150,34,"手动操作",4);
			Draw_Button_Cap(550,100-20,80,35,"保存",2);
			Draw_Button_Cap(650,100-20,80,35,"返回",2);
			Dis_Pwd(); //缺省值
			IndexId=0;UsrPos=0;PwdPos=0;
			DisPwdSel(0,0,RED);
			//Draw_Sel(40-4,100-4,155+8,30+8,RED);
//			Draw_Sel(15,120,150,40,WHITE);
//			Draw_Sel(15,200,150,40,WHITE);
//			Draw_Sel(15,280,150,40,WHITE);
//			Draw_Sel(15,360,150,40,WHITE);
		break;
		case FRM_MON:

			Draw_Line(395, 35,395, 432,2,BLACK);
			Draw_Line(404, 35,404, 432,2,BLACK);
			flgSFLs[0]=1;flgSFLs[1]=1;
			Dis_Mon(0);Dis_Mon(1);
			Draw_Line(795, 35,795,432,2,BLACK);
			Draw_Line(  4, 35,  4,432,2,BLACK);
			Draw_Line(54, 65,395, 65,2,BLACK);Draw_Line(454, 65,799, 65,2,BLACK);
			Draw_Line(0, 105,798, 105,2,BLACK);
			Draw_Line(0, 235,798, 235,2,BLACK);
			Draw_Line(0, 265,798, 265,2,BLACK);
			Draw_Line(0, 305,798, 305,2,BLACK);
			Draw_Line(0, 335,798, 335,2,BLACK);
			Draw_Line(0, 375,798, 375,2,BLACK);
			TFT_Fill(0,0,4,479,GOLD);	
			TFT_Fill(396,35,404,479,GOLD);	
			TFT_Fill(796,0,799,479,GOLD);	
			Draw_Line(395, 435,395, 375,2,BLACK);

			lcd_text24( 11,378,BLUE,WHITE,"车号");//lcd_text24(141,378,BLUE,BLUE,"铅封号");
			lcd_text24(411,378,BLUE,WHITE,"车号");//lcd_text24(541,378,BLUE,BLUE,"铅封号");
			Dis_SysAdd(set_AddCa);
			Dis_1_Pend(1);
		break;
		case FRM_MAIN:
		case FRM_SD: //manual mode
			Draw_Line(10, 50,690, 50,2,BLACK); //row line 1
			Draw_Line(10, 80,171, 80,2,BLACK); //row line 2		 ??
			Draw_Line(171, 90,690, 90,2,BLACK); //row line 2		 ??
			//Draw_Line(dx,120,680+dx,120,2,BLACK); //row line 3
			Draw_Line(dx,280,680+dx,280,2,BLACK); //row line 4//under SFL
			Draw_Line(dx,310,680+dx,310,2,BLACK); //row line 5
			Draw_Line(dx,350,680+dx,350,2,BLACK); //row line 6
			Draw_Line(dx,420,680+dx,420,2,BLACK); //row line 7
			//Draw_Line(dx,380,235+dx,380,2,BLACK);	
			Draw_Line(dx,50, dx,420,2,BLACK);
			Draw_Line(680+dx,50,680+dx,420,2,BLACK);

			Draw_Line(171, 50,171,280,2,BLACK);//"作业进度"右边线

			Draw_Line( 10,200,171,200,2,BLACK);//"实发量"左边线
			Draw_Line( 10,160,171,160,2,BLACK);//"作业进度"分割线
			Draw_Line( 10,240,171,240,2,BLACK);//"倒计时"分割线

			//lcd_text24( 55,88,BLUE,BLUE,"倒计时");//lcd_text24( 13,168,BLUE,BLUE,"倒计时");
			lcd_text24( 13, 88,BLACK,WHITE,"进度");
			lcd_text24( 13,128,BLACK,WHITE,"剩时");
			Draw_Line( 10,120,171,120,2,BLACK);

			lcd_text24( 13,168,BLACK,WHITE,"密度");
			//TFT_Fill(128,160,171,176,BLACK);//???????!!!!!!
			//lcd_text16(131,160,0xFFFF,0,"g/cm3");
			//LcdDisplayASCII(131,160,0xFFFF,0,0,"g/cm3");
			lcd_text16(131,160,0xFFFF,0,"g/cm{");
			//lcd_text16(131+22,160+6,0xFFFF,0,"");
			//Draw_Line(131+24+5,160+2,326+24+5,160+18,1,0);
			//LcdDisplayASCII(tx+347+8,242-6,fc,bc,0,"g");
			//LcdDisplayASCII(tx+347+22,242+6,fc,bc,0,"cm3");
			//Draw_Line(tx+347+24+5,244,tx+350+5,260,1,fc);
			//LcdDisplayASCII(131,160,0xFFFF,0,0,"g");
			//LcdDisplayASCII(131+8,160+12,0xFFFF,0,0,"cm3");
			//Draw_Line(tx+347+24+5,244,tx+350+5,260,1,0xFFFF);
			lcd_text24( 13,208,BLACK,WHITE,"介温");
			lcd_text24( 13,248,BLACK,WHITE,"流速");
			Draw_Line( 60, 80,60,120,2,BLACK);
			Draw_Line( 60,120,60,160,2,BLACK);
			Draw_Line( 60,160,60,200,2,BLACK);
			Draw_Line( 60,200,60,240,2,BLACK);
			Draw_Line( 60,240,60,280,2,BLACK);
			//TFT_Fill(70,140,170,160,0x0000);
			TFT_Fill(146, 350,171,380,0x0000);
			lcd_text16(148,358,0xFFFF,0x0000,"Kg");
			TFT_Fill(146,208,171,232,0x0000);
			lcd_text16(152,210,0xFFFF,0x0000,"~C");
			TFT_Fill(146,248,171,272,0x0000);
			lcd_text16(148,250,0xFFFF,0x0000,"m/s");

			//Draw_Line(235+dx,350,235+dx,420,2,BLACK);//"车牌"右边线
			//Draw_Line(264,350,264,420,2,BLACK);//"副阀"左边线
			Draw_Line(680+dx, 50,680+dx,420,2,BLACK);
			Draw_Line(780+dx, 50,780+dx,420,2,BLACK);
			Draw_Line(680+dx, 50,780+dx, 50,2,BLACK);
			//Draw_Line(680+dx,410,780+dx,410,2,BLACK);
			//unit backplace
			Draw_Line(171, 350,171,420,2,BLACK);//"实发质量"右边线
			Draw_Line(dx,  380,171,380,2,BLACK);//"实发质量"下划线
			//TFT_Fill(172-160, 51, 216-160, 79,BLUE);//	
			//Draw_EN24(250,380,GREEN,BLUE,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-+:.`");
			TFT_Fill(11, 51, 60, 79,BLUE);
			if(dualChNum==1)
        DisNum24(22,50,WHITE,BLUE,"1");//GREEN
 			  //Draw_EN24(182-160,54,WHITE,BLUE,"13");//GREEN
			if(dualChNum==2)
        DisNum24(22,50,WHITE,BLUE,"2");//GREEN
				//Draw_EN24(182-160,54,BRRED,BLUE,"14");
			lcd_text24(13,354,BLACK,WHITE,"实发质量");
			//lcd_text24(173,354,BLACK,WHITE,"压力");//lcd_text24(402,354,BLUE,BLUE,"压力");
			//TFT_Fill(221,350,264,380,0x0000);//压力单位
			//lcd_text16(231,357,0xFFFF,0,"MPa");

			//提货单号
			lcd_text24(72,55,BLACK,WHITE,"路");
			//应发量
			lcd_text24(181,53,BLACK,WHITE,"应发量");
			//为增加压力值车号与进度
			lcd_text24(173,355,BLACK,WHITE,"车号");//lcd_text24(181+93,355,BLACK,WHITE,"车号");
			Draw_Line(173,385,400,385,2,BLACK);//Draw_Line(171+93,385,400+93,385,2,BLACK);
			Draw_Line(400,350,400,420,2,BLACK);
			//lcd_text24(173,390,BLACK,WHITE,"进度");
			//TFT_Fill(240-2,395,342,410,0x0000);	//TFT_Fill(70,140,170,160,0x0000);
			//Draw_EN24(346,390,BLACK,0xFFFF,"100%");//12*4
			
			TFT_Fill(666, 50,690, 80,0x0000); //应发量单位“L”
			lcd_text16(670,60,0xFFFF,0x0000,"L");
			//lcd_text16(664+dx,80,RED,0,"Kg");
			//lcd_text24(645+dx,50,0xFFFF,0xFFFF,"已发");
			//2FV中不显示实时状态标签
			Draw_Line(153,280,153,350,2,BLACK);//瞬时体积右边线
			Draw_Line(350,280,350,350,2,BLACK);//累积体积右边线
			Draw_Line(493,280,493,350,2,BLACK);//累积体积右边线
			TFT_Fill(110,280,153,310,0x0000);//瞬时体积单位
			TFT_Fill(315,280,350,310,0x0000);//累积体积单位
			TFT_Fill(110+340,280,153+340,310,0x0000);//瞬时质量单位
			TFT_Fill(315+340,280,350+340,310,0x0000);//累积质量单位
			lcd_text24( 13,285,BLACK,WHITE,"瞬时量");
			lcd_text16( 115,290,0xFFFF,0x0000,"m{/h");
			lcd_text24(190,285,BLACK,WHITE,"累积体积");
			lcd_text16(327,290,0xFFFF,0x0000,"L");

			lcd_text24(353,285,BLACK,WHITE,"瞬时量");
			lcd_text16(457,290,0xFFFF,0x0000,"t/h");
			lcd_text24(530,285,BLACK,WHITE,"累积质量");
			lcd_text16(667,290,0xFFFF,0x0000,"Kg");

			//Draw_DigNum(40+0*72,130);
			//Draw_DigNum(40+1*72,130);
			Draw_DigNum(40+2*72,130-15);
			Draw_DigNum(40+3*72,130-15);
			Draw_DigNum(40+4*72,130-15);
			Draw_DigNum(40+5*72,130-15);
			Draw_DigNum(40+6*72,130-15);
			Draw_DigNum(40+7*72,130-15);
			Draw_DigNum(40+8*72,130-15);
			/*待发记录*/
			Draw_unRead(690+dx,  60);
			/*已发记录*/
			Draw_Readed(690+dx, 125);
			/*报警记录*/
			Draw_Beep(700+40,230,25,0,0);
			//Draw_TimBar(0);
			Draw_AoBar(0);
			//Draw_Line(690,50,790,410,1,BLACK);

			Draw_Button(690+dx, 60+  0,80,60);
			Draw_Button(690+dx, 60+ 70,80,60);
			Draw_Button(690+dx, 60+140,80,60);
			Draw_Button(690+dx, 60+210,80,60);
			Draw_Button(690+dx, 60+280,80,60);
			//Draw_Button(690+dx, 60+310,80,40);

			if(frmNum==FRM_MAIN)
			{	//自动
				lcd_text24(708,288,RED,0xFFFF,"Ａ");
				lcd_text24(728,288,0,0xFFFF,"／");
				lcd_text24(708+38,288,0,0xFFFF,"Ｍ");
			}
			else
			{	//手动
				lcd_text24(708,288,0,0xFFFF,"Ａ");
				lcd_text24(728,288,0,0xFFFF,"／");
				lcd_text24(708+38,288,RED,0xFFFF,"Ｍ");
				Draw_Button(510,380+4,170,24);
				Draw_Cap(510,378+4,170,24,"主阀",3);
				//Draw_Sel(470,378,165+45,15+16,RED);
			}
				Draw_Cap(690+dx,340,80,60,"返回",2);
				//Draw_Lamp(750,360,0); 
			 
			lcd_text16(408+0*48,355,BLACK,WHITE,"静电");
			lcd_text16(408+1*48,355,BLACK,WHITE,"溢油");
			lcd_text16(408+2*48,355,BLACK,WHITE,"主开");
			lcd_text16(408+3*48,355,BLACK,WHITE,"主关");
			lcd_text16(408+4*48,355,BLACK,WHITE,"副开");
			lcd_text16(408+5*48,355,BLACK,WHITE,"副关");
			TFT_Fill(416+0*48,375,432+0*48,375+16,BLUE);//主阀开回讯			
			TFT_Fill(416+1*48,375,432+1*48,375+16,BLUE);//主阀开回讯			
			TFT_Fill(416+2*48,375,432+2*48,375+16,BLUE);//主阀开回讯			
			TFT_Fill(416+3*48,375,432+3*48,375+16,BLUE);//主阀开回讯			
			TFT_Fill(416+4*48,375,432+4*48,375+16,BLUE);//主阀开回讯			
			TFT_Fill(416+5*48,375,432+5*48,375+16,BLUE);//主阀开回讯			
			Draw_Line(400,396,690,396,2,BLACK);//瞬时体积右边线
			lcd_text16(610,400,BLACK,WHITE,"主阀");
			/*4 lamp 3 back signal*/
			//Draw_Lamp(510+0*25,356,0);  //机泵 (Do3)
			//Draw_Lamp(510+1*25,356,0);	//副阀 (Do4)
			//Draw_Lamp(510+2*25,356,0);	//备用
			////Draw_Lamp(480+3*30,356,0);	//机泵 (Do3)
			//TFT_Fill(510+3*25,356,510+3*25+16,356+16,0x0000);//主阀开回讯
			//TFT_Fill(510+4*25,356,510+4*25+16,356+16,0x0000);//主阀关回讯
			//TFT_Fill(510+5*25,356,510+5*25+16,356+16,0x0000);//静电
			//TFT_Fill(510+6*25,356,510+6*25+16,356+16,0x0000);//溢流
			flgSFLdis=1;
				re_SFL_float(dSFLv);
			frmMainCur=0;
			flgSdStart=0;
			flgSdEnd=0;
			for(ti=0;ti<6;ti++)
			{	
				   logUsr[0][ti]=' ';
				   logUsr[1][ti]=' ';
				tmplogUsr[0][ti]=' ';
				tmplogUsr[1][ti]=' ';
			}	
			TFT_Fill(240,350,395,385,BLUE);

			Dis_1_Pend(1);
		  //Dis_CarId(1);//Dis_Pad(1);	// Dis_Pad_Mini(0);//
//		  if((((rtDoDat&0x30)==0x10)&&(dualChNum==1))||(((rtDoDat&0xC0)==0x40)&&(dualChNum==2)))
//			{
//				Draw_TextBox(405,378,60,24);
//				Draw_Cap(405,378,60,24,"副阀开",3);
//			}
//		  if((((rtDoDat&0x30)==0x20)&&(dualChNum==1))||(((rtDoDat&0xC0)==0x80)&&(dualChNum==2)))
//			{
//				Draw_Button(405,378,60,24);
//				Draw_Cap(405,378,60,24,"副阀关",3);
//			}

			//lcd_text16( 330,356,LGRAY,LGRAY," AMS使能");
			//lcd_text16( 330,356+18,LGRAY,LGRAY,"压力使能");
			//lcd_text16( 330,356+36,LGRAY,LGRAY,"授权使能");

		break;
//		case FRM_PENDSEL:
//			TFT_Fill(6, 37,88, 105,BLUE);//	
//			if(dualChNum==1)
//				lcd_en48(20,46,WHITE,BLUE,"13");//GREEN
//			else
// 				lcd_en48(20,46,WHITE,BLUE,"14");//GREEN
//			Draw_Line(0,104,799,104,3,BLACK);
//			Draw_Line(0,168,799,168,3,BLACK);
//			Draw_Line(0,232,799,232,3,BLACK);
//			Draw_Line(0,296,799,296,3,BLACK);
//			Draw_Line(0,360,799,360,3,BLACK);
//			Draw_Line(300,104,300,360,4,BLACK);

//			lcd_text48(232,48,0,0,"待");lcd_text48(328,48,0,0,"发");lcd_text48(424,48,0,0,"确");lcd_text48(520,48,0,0,"认"); 
//			//lcd_text24(352,138,0,0,"待发确认");
//			lcd_text48(116,112,0,0,"车型");
//			lcd_text48(116,176,0,0,"车号");
//			lcd_text48( 68,240,0,0,"有效容积");
//			lcd_text48( 92,304,0,0,"应发量");
//			lcd_text48(254,373,BLUE,BLUE,"确认");
//			lcd_text48(450,373,BLUE,BLUE,"取消");
//			Draw_Rect(244,373,116,47,2);
//			Draw_Rect(440,373,116,47,2);
//			TFT_Fill(393,112+2,393+188,112+44,WHITE);
//			TFT_Fill(393,176+2,393+188,176+44,WHITE);
//			//TFT_Fill(393,240,393+192,240+48,WHITE);
//			TFT_Fill(393,304+2,393+188,304+44,WHITE);
//			if(sys_Conn==0)
//			{
//				 for(ti=0;ti<10;ti++)
//				 {
//					idCarTypeId[ti]=0;idCarId[ti]=0;idCarYFL[ti]=0;
//				 }
//			}
//			for(ti=0;ti<8;ti++)
//			{
//			 tDisBuf[ti]=idCarTypeId[ti];
//			}
//			/*tDisBuf[0]=' ';tDisBuf[1]=' ';tDisBuf[2]=' ';tDisBuf[3]=' ';tDisBuf[4]='G';tDisBuf[5]='7';tDisBuf[6]='0';tDisBuf[7]='A';*/
//			tDisBuf[8]=0;
//			lcd_en48(393,112,0,WHITE,tDisBuf);
//			for(ti=0;ti<8;ti++)
//			{
//			 tDisBuf[ti]=' ';
//			 idCarId[ti]=' ';
//			}
//			tDisBuf[8]=0;idCarId[8]=0;
//			lcd_en48(393,176,BLUE,WHITE,tDisBuf);

//			for(ti=0;ti<8;ti++)
//			{
//			 tDisBuf[ti]=' ';
//			}
//			tDisBuf[8]=0;
//			//tDisBuf[0]=' ';tDisBuf[1]=' ';tDisBuf[2]=' ';tDisBuf[3]='6';
//			//tDisBuf[4]='7';tDisBuf[5]='7';tDisBuf[6]='0';tDisBuf[7]='0';tDisBuf[8]=0;
//			lcd_en48(393,240,0,WHITE,tmpSavMaxVol);
//			for(ti=0;ti<8;ti++)
//			{
//			 tDisBuf[ti]=idCarYFL[ti];
//			}
//			tDisBuf[8]=0;
//			lcd_en48(393,304,BLUE,WHITE,tDisBuf);
//			SelPendSel(0,0,RED);
//			//frmPendSelx=0;frmPendSely=0;
//			//flgReadCard=0;
//		break;
		case FRM_PENDLST:
			TFT_Fill(33,50,799,411,0xffff);	
			for(i=0;i<12;i++)
				Draw_Line( 30,80+i*30,790,80+i*30,1,BLACK);
			if(dualChNum==1) lcd_text16(400-48-32,60,0,0xffff,"1#");
			if(dualChNum==2) lcd_text16(400-48-32,60,0,0xffff,"2#");
			lcd_text16(400-48,60,0,0xFFFF,"待发记录");
			Draw_Line( 30,50,790,50,1,BLACK);
			Draw_Line( 30,50, 30,410,1,BLACK);
			Draw_Line( 70,80, 70,410,1,BLACK);
			Draw_Line(250,80,250,410,1,BLACK);
			Draw_Line(400,80,400,410,1,BLACK);
			Draw_Line(470,80,470,410,1,BLACK);
			Draw_Line(670,80,670,410,1,BLACK);
			Draw_Line(790,50,790,410,1,BLACK);
			lcd_text16( 32,88,0,0xFFFF,"序号");
			lcd_text16(100,88,0,0xFFFF,"车号");
			lcd_text16(280,88,0,0xFFFF,"提单号");
			lcd_text16(419,88,0,0xFFFF,"路号");
			lcd_text16(520,88,0,0xFFFF,"应发量");
			Draw_Button_Cap(720,55,60,20,"返回",2);
			Draw_Sel(720-5,55-5,60+10,20+10,RED);
			Dis_Pend(dualChNum-1);
			//TFT_Fill(33,50,799,411,0xffff);	
			//for(i=0;i<10;i++)
			//	Draw_Line( 30+30,80+i*33,790,80+i*33,1,BLACK);
			////if(dualChNum==1) lcd_text16(400-48-32,60,0,0,"13#");
			////if(dualChNum==2) lcd_text16(400-48-32,60,0,0,"14#");
			////lcd_text16(400-48,60,0,0,"待发记录");
			//Draw_Line( 20,50,790, 50,1,BLACK);
			//Draw_Line( 20,50, 20,413,1,BLACK);
			//Draw_Line( 30+30,50, 30+30,413,1,BLACK);
			//Draw_Line( 30+70,50, 30+70,413,1,BLACK);
			//Draw_Line(30+200,50,30+200,413,1,BLACK);
			//Draw_Line(30+330,50,30+330,413,1,BLACK);
			//Draw_Line(30+460,50,30+460,413,1,BLACK);
			//Draw_Line(30+590,50,30+590,413,1,BLACK);
			//Draw_Line(790,50,790,413,1,BLACK);
			//Draw_Line( 20,413,790,413,1,BLACK);
			//lcd_text16( 30+34,58,0,0,"序号");
			//lcd_text16( 30+106,58,0,0,"车型代码");
			//lcd_text16( 30+236,58,0,0,"有效容积(L)");
			//lcd_text16( 30+366,58,0,0,"装载率(%)");
			//lcd_text16( 30+496,58,0,0,"预发量(L)");
			//lcd_text16( 15+674,58,0,0,"操作");
			//lcd_text16(32,200,0,0,"车");
			//lcd_text16(32,216,0,0,"型");
			//lcd_text16(32,232,0,0,"维");
			//lcd_text16(32,248,0,0,"护");
			//
			////Draw_Button_Cap(720,55,60,20,"返回",2);
			////Draw_Sel(720-5,55-5,60+10,20+10,RED);
			////Dis_Pend(dualChNum-1);
		break;
		case FRM_DONELST:
			TFT_Fill(33,50,799,411,0xffff);	
			Draw_Line( 30,50,790,50,1,BLACK);
			for(i=0;i<11;i++)
				Draw_Line( 30,80+i*30,790,80+i*30,1,BLACK);

			Draw_Line( 30,50, 30,380,1,BLACK);
			Draw_Line(100,80,100,380,1,BLACK);
			Draw_Line(320,80,320,380,1,BLACK);
			Draw_Line(550,80,550,380,1,BLACK);
			Draw_Line(790,50,790,380,1,BLACK);
			Draw_Line(100+115,80+5*30,100+115,80+8*30,1,BLACK);
	
			Draw_Cap( 33, 50,     760,30,"已发记录",4);
			Draw_Cap( 33, 80,      64,30,"序号",2);
			Draw_Cap( 33, 80+ 1*30,64,30,"路号",2);
			//Draw_Cap( 33+115-20, 80+ 2*30,64,30,"工号",2);
			Draw_Cap( 33, 80+ 2*30,64,30,"提单密码",4);
			Draw_Cap( 33, 80+ 3*30,64,30,"车号",2);
			Draw_Cap( 33, 80+ 4*30,64,30,"应发量",3);
			Draw_Cap( 33, 80+ 5*30,64,30,"实发量",3);
			Draw_Cap( 33, 80+ 6*30,64,30,"表前数",3);
			Draw_Cap( 33, 80+ 7*30,64,30,"表后数",3);
			Draw_Cap( 33, 80+ 8*30,64,30,"开始时间",4);
			Draw_Cap( 33, 80+ 9*30,64,30,"结束时间",4);
			Draw_Button_Cap(710,390,80,30,"返回",2);
			Draw_Button_Cap( 33,390,80,30,"上一页",3);
			Draw_Button_Cap(133,390,80,30,"下一页",3);
			LstDisPgNum=0;
			LastDisLos=(Fnd_Lst_Index(0)>>8);
			Dis_New_Lst(LstDisPgNum);
			LstCur=0;
			Draw_Sel(33-5,390-5,80+10,30+10,RED);
		break;
		case FRM_WARN:
 			//TFT_CLEAR(0xFFFF);
			TFT_Fill(33,50,799,411,0xffff);	
//			for(i=0;i<12;i+=2)
//				TFT_Fill(33,80+i*30,680,80+(i+1)*30,LGRAY);
			for(i=0;i<12;i++)
				Draw_Line( 33,80+i*30,680,80+i*30,1,BLACK);
			Draw_Line( 33,50,680,50,1,BLACK);
			for(i=0;i<12;i++)
			{
				Draw_Line( 33,80+i*30,680,80+i*30,1,BLACK);
			}
			for(i=0;i<9;i++)
			{
				tDisBuf[0]='1'+i;
				tDisBuf[1]='\0';
				lcd_text16(50,60-3+i*30,0,0,tDisBuf);
			}
			tDisBuf[0]='1';
			for(i=0;i<3;i++)
			{
				tDisBuf[1]='0'+i;
				tDisBuf[2]='\0';
				lcd_text16(50-4,60-3+(9+i)*30,0,0,tDisBuf);
			}

			//Draw_Button_Cap(700,310,60,30,"提  数",3);
			Draw_Button_Cap(710,360,60,30,"返  回",3);
			Draw_Sel(710-5,360-5,60+10,30+10,RED);
			Draw_Line( 33, 50, 33,410,1,BLACK);
			Draw_Line(680, 50,680,410,1,BLACK);
			Draw_Line( 80, 50, 80,410,1,BLACK);
			Draw_Line(280, 50,280,410,1,BLACK);
			
			Dis_WarnLst();
					

		break;
		case FRM_CONFIG:
			TFT_Fill(0,50,799,411,0xffff);	
			DisConfSel(0,0,RED);
			Draw_Cap(40,55+ 0*32,64,30,"监控地址",4);
			Draw_Cap(40,55+ 1*32,64,30,"管理地址",4);
			Draw_Cap(40,55+ 2*32,64,30,"修正系数",4);
			Draw_Cap(40,55+ 3*32,64,30,"修正值",4);
			Draw_Cap(40,55+ 4*32,64,30,"压力量程",4);
			Draw_Cap(40,55+ 5*32,64,30,"启动压力",4);
			Draw_Cap(40,55+ 6*32,64,30,"联锁压力",4);
			Draw_Cap(40,55+ 7*32,64,30,"密度上限",4);
			Draw_Cap(40,55+ 8*32,64,30,"密度下限",4);
			Draw_Cap(40,55+ 9*32,64,30,"流量下限",4);
			//Draw_Cap(40,55+10*32,64,30,"设置时间",4);

			lcd_text16(110,62+ 0*32,0,0xffff,"( 2 - 16 )");
			lcd_text16(110,62+ 1*32,0,0xffff,"( 5 - 59 )");
			lcd_text16(110,62+ 2*32,0,0xffff,"(90 - 110)");
			lcd_text16(110,62+ 3*32,0,0xffff,"( 0 - 99 )");
			lcd_text16(110,62+ 4*32,0,0xffff,"( 0 - 99 )");
			lcd_text16(110,62+ 5*32,0,0xffff,"( 0 - 99 )");
			lcd_text16(110,62+ 6*32,0,0xffff,"( 0 - 99 )");
			lcd_text16(110,62+ 7*32,0,0xffff,"( 0 - 20 )");
			lcd_text16(110,62+ 8*32,0,0xffff,"( 0 - 20 )");
			lcd_text16(110,62+ 9*32,0,0xffff,"( 0 - 999)");
			//lcd_text16(110,62+10*32,0,0,"( HHMMSS )");

			Draw_TextBox(200,58+ 0*32,60,22);
			Draw_TextBox(200,58+ 1*32,60,22);
			Draw_TextBox(200,58+ 2*32,60,22);
			Draw_TextBox(200,58+ 3*32,60,22);
			Draw_TextBox(200,58+ 4*32,60,22);
			Draw_TextBox(200,58+ 5*32,60,22);
			Draw_TextBox(200,58+ 6*32,60,22);
			Draw_TextBox(200,58+ 7*32,60,22);
			Draw_TextBox(200,58+ 8*32,60,22);
			Draw_TextBox(200,58+ 9*32,60,22);
			//Draw_TextBox(200,58+10*32,60,22);
			lcd_text16(265,62+ 2*32,BLUE,0xFFFF,"x0.01");
			lcd_text16(265,62+ 3*32,BLUE,0xFFFF,"Kg");
			lcd_text16(265,62+ 4*32,BLUE,0xFFFF,"x0.1Mpa");
			lcd_text16(265,62+ 5*32,BLUE,0xFFFF,"x0.1Mpa");
			lcd_text16(265,62+ 6*32,BLUE,0xFFFF,"x0.1Mpa");
			lcd_text16(265,62+ 7*32,BLUE,0xFFFF,"x0.1g/cm{");
			lcd_text16(265,62+ 8*32,BLUE,0xFFFF,"x0.1g/cm{");
			lcd_text16(265,62+ 9*32,BLUE,0xFFFF,"x0.1Kg/s");

			Draw_Line(340, 50,340,370 ,1,BLACK);

			Draw_Cap(352,55+ 0*32,64,30,"分速率1",4);
			Draw_Cap(352,55+ 1*32,64,30,"分段点1",4);
			Draw_Cap(352,55+ 2*32,64,30,"分速率2",4);
			Draw_Cap(352,55+ 3*32,64,30,"分段点2",4);
			Draw_Cap(352,55+ 4*32,64,30,"关稳超前",4);
			Draw_Cap(352,55+ 5*32,64,30,"日期",4);
			Draw_Cap(352,55+ 6*32,64,30,"时间",4);
			lcd_text16(416,62+ 0*32,0,0xffff,"(01-99)");
			lcd_text16(416,62+ 1*32,0,0xffff,"(10-99)");
			lcd_text16(416,62+ 2*32,0,0xffff,"(01-99)");
			lcd_text16(416,62+ 3*32,0,0xffff,"(10-99)");
			lcd_text16(416,62+ 4*32,0,0xffff,"(0 -99)");
			lcd_text16(400,62+ 5*32,0,0xffff,"(yymmdd)");
			lcd_text16(400,62+ 6*32,0,0xffff,"(hhmmss)");
			Draw_TextBox(480,58+ 0*32,62,22);
			Draw_TextBox(480,58+ 1*32,62,22);
			Draw_TextBox(480,58+ 2*32,62,22);
			Draw_TextBox(480,58+ 3*32,62,22);
			Draw_TextBox(480,58+ 4*32,62,22);
			Draw_TextBox(480,58+ 5*32,62,22);
			Draw_TextBox(480,58+ 6*32,62,22);

			Draw_Cap(352,55+ 7*32,64,30,"脉冲偏差",4);
			Draw_Cap(352,55+ 8*32,64,30,"脉冲系数",4);
			Draw_Cap(352,55+ 9*32,64,30,"主管管径",4);
			lcd_text16(416,62+ 7*32,0,0xffff,"(0-9999)");
			lcd_text16(416,62+ 8*32,0,0xffff,"(xx.xxx)");
			lcd_text16(416,62+ 9*32,0,0xffff,"(10-999)");
			Draw_TextBox(480,58+ 7*32,62,22);
			Draw_TextBox(480,58+ 8*32,62,22);
			Draw_TextBox(480,58+ 9*32,62,22);

			Draw_Cap(574, 55+0*32,64,30,"静  电",5);//WarnOnOff[0]:flgAlert[0]
			Draw_Cap(574, 55+1*32,64,30,"溢  流",5);//WarnOnOff[1]:flgAlert[1]
			Draw_Cap(574, 55+2*32,64,30,"流量计",5);//WarnOnOff[2]:flgAlert[2]
			Draw_Cap(574, 55+3*32,64,30,"无流量",5);//WarnOnOff[3]:flgAlert[3]
			Draw_Cap(574, 55+4*32,64,30,"泄  漏",5);//WarnOnOff[4]:flgAlert[4]
			Draw_Cap(574, 55+5*32,64,30,"密  度",5);//WarnOnOff[5]:flgAlert[5]
			Draw_Cap(574, 55+6*32,64,30,"低压力",5);//WarnOnOff[6]:flgAlert[7]
			Draw_Cap(574, 55+7*32,64,30,"低流量",5);//WarnOnOff[7]:flgAlert[8]
			Draw_Cap(574, 55+8*32,64,30," A M S ",5);//WarnOnOff[8]:flgAlert[9]
			Draw_Cap(574-16, 55+9*32,64,30,"脉冲比对",5);//WarnOnOff[9]flgAlert[10]
			Draw_Cap(574,46+10*32,64,30,"本  地",5);
			Draw_Cap(574,64+10*32,64,30,"联  网",5);
			Draw_Line(562,69+10*32,610,69+10*32,1,BLACK);


			Draw_Button_Cap(620,58+0*32,42,22," ",1);
			Draw_Button_Cap(620,58+1*32,42,22," ",1);
			Draw_Button_Cap(620,58+2*32,42,22," ",1);
			Draw_Button_Cap(620,58+3*32,42,22," ",1);
			Draw_Button_Cap(620,58+4*32,42,22," ",1);
			Draw_Button_Cap(620,58+5*32,42,22," ",1);
			Draw_Button_Cap(620,58+6*32,42,22," ",1);
			Draw_Button_Cap(620,58+7*32,42,22," ",1);
			Draw_Button_Cap(620,58+8*32,42,22," ",1);
			Draw_Button_Cap(620,58+9*32,42,22," ",1);
			Draw_Button_Cap(620,58+10*32,42,22," ",1);

			lcd_text16(620+17,62+0*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+1*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+2*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+3*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+4*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+5*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+6*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+7*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+8*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+9*32,0,0xffff,"Y");	 //17=(42-8)/2
			lcd_text16(620+ 5,62+10*32,0,0xffff,"本地");	 //17=(42-8)/2

			Draw_Button_Cap(700,58+0*35, 80,22,"保存",2);
			Draw_Button_Cap(700,58+1*35, 80,22,"缺省",2);
			Draw_Button_Cap(700,58+2*35, 80,22,"返回",2);
			Dis_Conf();
			ConfIndex=0;
			ConfPos=0;
			DisConfSel(ConfIndex,ConfPos,RED);

		break;
		case FRM_AMS:
			TFT_Fill(33,50,799,411,0xffff);	
			Draw_Line( 0, 50,799, 50,3,BLACK);
			Draw_Line( 0,412,799,412,3,BLACK);
			Dis_AMS();
		break;
//		//显示
//		case FRM_AMS:
//			TFT_Fill(33,50,799,411,0xffff);	
//			Draw_Line( 0, 50,799, 50,3,BLACK);
//			Draw_Line( 0,412,799,412,3,BLACK);
////			for(i=0;i<15;i++)
////			{
////				Draw_Lamp(60+i*45,380,0);
////				tDisBuf[0]=(i+1)/10+'0';
////				if(tDisBuf[0]=='0')	tDisBuf[0]=' ';
////				tDisBuf[1]=(i+1)%10+'0';
////				tDisBuf[2]=0;
////				if(tDisBuf[0]==' ')
////					lcd_text16(40+i*45+16, 360,RED,RED,tDisBuf);
////				else
////					lcd_text16(44+i*45+16, 360,RED,RED,tDisBuf);
////			}

//			//TFT_Fill( 92, 76,164,100,0);
//			TFT_Fill( 92,106,164,130,0);
//			TFT_Fill(218,106,322,130,0);
//			TFT_Fill(392,106,480,130,0);
//			TFT_Fill(518,106,638,130,0);
//			TFT_Fill(676,106,798,130,0);
//			//lcd_text16( 40, 80,BLUE,BLUE,"FV地址");
//			lcd_text16( 40,110,BLUE,BLUE,"温  度");	lcd_text16(148,110,GOLD,0,"~C");
//			lcd_text16(182,110,BLUE,BLUE,"密度");		lcd_text16(282,110,GOLD,0,"g/cm{");
//			lcd_text16(340,110,BLUE,BLUE,"瞬时量");	lcd_text16(456,110,GOLD,0,"t/h");
//			lcd_text16(498,110,BLUE,BLUE,"盛");			lcd_text16(630,110,GOLD,0,"t");
//			//lcd_text16(656,110,BLUE,BLUE,"盛");			lcd_text16(774,110,GOLD,0,"kM{");
//			
//			Draw_Line(25, 140,799,140,2,BLACK);
//			lcd_text16( 40,160,0,0,"最大流量");
//			lcd_text16( 40,210,0,0,"流体方向");
//			lcd_text16( 40,260,0,0,"团流上限");
//			lcd_text16( 40,310,0,0,"团流下限");
//			lcd_text16(240,160,0,0,"密度修正D1");
//			lcd_text16(240,210,0,0,"密度修正D2");
//			lcd_text16(240,260,0,0,"密度系数");
//			lcd_text16(240,310,0,0,"零点值");
//			lcd_text16(456,160,0,0,"质量流量系数");
//			lcd_text16(456,210,0,0,"质量校验系数");
//			lcd_text16(456,260,0,0,"温度校验系数");
//			//lcd_text16(456,310,0,0,"流量系数");
//			TFT_Fill(108,156,204,180,0);
//			TFT_Fill(108,156+50,204,180+50,0);
//			TFT_Fill(108,156+100,204,180+100,0);
//			TFT_Fill(108,156+150,204,180+150,0);
//			TFT_Fill(324,156,420,180,0);
//			TFT_Fill(324,156+50,420,180+50,0);
//			TFT_Fill(324,156+100,420,180+100,0);
//			TFT_Fill(324,156+150,420,180+150,0);
//			TFT_Fill(572,156,718,180,0);
//			TFT_Fill(572,156+50,718,180+50,0);
//			TFT_Fill(572,156+100,718,180+100,0);
//			//TFT_Fill(572,156+150,668,180+150,0);

//			Draw_Line(25, 360,799,360,2,BLACK);
//			Draw_Button_Cap(710,60,60,30,"返回",2);
//			Draw_Button_Cap(710-80,60,60,30,"确认",2);
//			Draw_Button_Cap(710-160,60,60,30,"提取",2);
//			Draw_Sel(710-5,60-5,60+10,30+10,RED);
//			AmsIndex=2;
//		 	Dis_Detail();
//		break;
//		//case FRM_SET_PLC
		case FRM_SETTING:
			Frm_Fy(1);//Frm_Fy(swVer);
			Dis_Fy();
			DisSetSel(0,0,RED);
			FyIndex=0;FyPos=0;

		break;
		case FRM_MAN:
			TFT_Fill(26,49,799,401,0xffff);	
			lcd_text16(40,55+0*30,0,0xffff,"Di1");
			lcd_text16(40,55+1*30,0,0xffff,"Di2");
			lcd_text16(40,55+2*30,0,0xffff,"Di3");
			lcd_text16(40,55+3*30,0,0xffff,"Di4");
			lcd_text16(40,55+4*30,0,0xffff,"Di5");
			lcd_text16(40,55+5*30,0,0xffff,"Di6");
			lcd_text16(40,55+6*30,0,0xffff,"Di7");
			lcd_text16(40,55+7*30,0,0xffff,"Di8");
			lcd_text16(40,55+8*30,0,0xffff,"Di9");
			lcd_text16(40,55+9*30,0,0xffff,"Di10");
			lcd_text16(40,55+10*30,0,0xffff,"Di11");
			lcd_text16(40,55+11*30,0,0xffff,"Di12");
			for(i=0;i<12;i++)
			  Draw_Lamp(74,55+i*30,0);
			lcd_text16(125,75+0*40,0,0xffff,"Do1");
			lcd_text16(125,75+1*40,0,0xffff,"Do2");
			lcd_text16(125,75+2*40,0,0xffff,"Do3");
			lcd_text16(125,75+3*40,0,0xffff,"Do4");
			lcd_text16(125,75+4*40,0,0xffff,"Do5");
			lcd_text16(125,75+5*40,0,0xffff,"Do6");
			lcd_text16(125,75+6*40,0,0xffff,"Do7");
			lcd_text16(125,75+7*40,0,0xffff,"Do8");
			for(i=0;i<8;i++)
			{ 
			 Draw_Lamp(154,75+i*40,0);
			 Draw_Button(122,72+i*40,50,22);
			 }
			lcd_text16(205,75+0*40,0,0xffff,"Ai1:");
			lcd_text16(205,75+1*40,0,0xffff,"Ai2:");
			lcd_text16(205,75+2*40,0,0xffff,"Ai3:");
			lcd_text16(205,75+3*40,0,0xffff,"Ai4:");

			lcd_text16(240,75+0*40,0,0xffff,"0000");
			lcd_text16(240,75+1*40,0,0xffff,"0000");
			lcd_text16(240,75+2*40,0,0xffff,"0000");
			lcd_text16(240,75+3*40,0,0xffff,"0000");

			lcd_text16(300,75+0*40,0,0xffff,"AO1:");lcd_text16(330,75+0*40,0,0xffff,"0000");
			lcd_text16(300,75+1*40,0,0xffff,"AO2:");lcd_text16(330,75+1*40,0,0xffff,"0000");
			lcd_text16(300,75+2*40,0,0xffff,"Fi1:");lcd_text16(330,75+2*40,0,0xffff,"0000");
			lcd_text16(300,75+3*40,0,0xffff,"Fi2:");lcd_text16(330,75+3*40,0,0xffff,"0000");
			

			lcd_text16(430-50,75+0*40,0,0xffff,"提前量max");
			lcd_text16(430-50,75+1*40,0,0xffff,"提前量min");
			lcd_text16(430-50,75+2*40,0,0xffff,"关稳期max");
			lcd_text16(430-50,75+3*40,0,0xffff,"关稳期min");

			Draw_TextBox(460,72+0*40,50,22);
			Draw_TextBox(460,72+1*40,50,22);
			Draw_TextBox(460,72+2*40,50,22);
			Draw_TextBox(460,72+3*40,50,22);
			Draw_Button_Cap(710,72,60,22,"保存",2);
			Draw_Button_Cap(710,72+30,60,22,"返回",2);
			Draw_Rect(205,235,500,150,2);
			Draw_Rect(545,130,140, 80,2);
			DisManSel(0,0,RED);
			flgManCon=1;
			ManIndex=0;ManCur=0;ManPos=0;
		break;
	}
	__enable_irq();
}


void Dis_Scr_(unsigned char pgNum)
{
	unsigned int i=0;
//	unsigned char dx;
	unsigned char tx;
//	unsigned char ti;
//	unsigned char cx,cy;
//	unsigned char tempDat;
	char tDisBuf[10];
	//char *tStr;
	__disable_irq();
//	Frm_Init();
//	dx=10;
	//flgManCon=0;
	//raInit();
	TFT_Fill(0,50,799,430,WHITE);	
	TFT_Fill(0,0,799,36,BLUE);	
	Draw_Line( 0,36,799,36,3,BLACK);
	lcd_text24(320,2,WHITE,BLUE,"批");
	lcd_text24(354,2,WHITE,BLUE,"量");
	lcd_text24(388,2,WHITE,BLUE,"控");
	lcd_text24(422,2,WHITE,BLUE,"制");
	lcd_text24(456,2,WHITE,BLUE,"器");
	
	TFT_Fill(0,430,799,479,BLUE);	
	Draw_Line( 0,430,799,430,3,BLACK);
	lcd_text24( 10,443,GOLD,BLUE,"AWz-1234");
	lcd_text24(480,443,WHITE,BLUE,"邯郸市奥图威尔仪表有限公司");
	switch(pgNum)
	{

		case FRM_MON:
			//flgSFLs[0]=1;flgSFLs[1]=1;
			Dis_Mon(1);Dis_Mon(0);
			Draw_Line(88, 65,395, 65,2,BLACK);Draw_Line(488, 65,799, 65,2,BLACK);
			Draw_Line(0, 105,798, 105,2,BLACK);
			Draw_Line(0, 235,798, 235,2,BLACK);
			Draw_Line(0, 265,798, 265,2,BLACK);
			Draw_Line(0, 305,798, 305,2,BLACK);
			Draw_Line(0, 335,798, 335,2,BLACK);
			Draw_Line(0, 375,798, 375,2,BLACK);

			lcd_text24( 11,378,0xFFFF,BLUE,"车号");//lcd_text24(141,378,BLUE,BLUE,"铅封号");
			lcd_text24(411,378,0xFFFF,BLUE,"车号");//lcd_text24(541,378,BLUE,BLUE,"铅封号");

			TFT_Fill(0,0,4,479,GOLD);	
			TFT_Fill(796,0,799,479,GOLD);	
			Draw_Line(795, 35,795,432,2,BLACK);
			Draw_Line(  4, 35,  4,432,2,BLACK);

			TFT_Fill(396,35,404,479,GOLD);	
			Draw_Line(395, 35,395, 432,2,BLACK);
			Draw_Line(404, 35,404, 432,2,BLACK);
			//if(sys_Conn==1)Dis_1_Pend(1);
		break;
		case FRM_MAIN:
			Draw_Line(  7, 67,  7,363,3,BLACK);
			Draw_Line(  7, 67,413, 67,3,BLACK);
			Draw_Line(413,363,413, 67,3,BLACK);
			Draw_Line(413,363,  7,363,3,BLACK);
			lcd_text24(  7,385,BLACK,WHITE,"示重：");
			//lcd_text24(210,385,BLACK,WHITE,"皮重：");
		tDisBuf[0]='D';tDisBuf[1]='i';tDisBuf[2]='1';tDisBuf[3]=':';tDisBuf[4]=':';tDisBuf[5]='\0';
			for(i=0;i<12;i++)
			{
				Draw_Line(450, 60+(i*30),600,60+(i*30),1,BLACK);
				tDisBuf[2]=0x30+((i+1)/10);	tDisBuf[3]=0x30+((i+1)%10);
				if(tDisBuf[2]=='0')tDisBuf[2]=' ';
				lcd_text16(465,68+(i*30),BLACK,WHITE,tDisBuf);
			}
			tDisBuf[0]='D';tDisBuf[1]='i';tDisBuf[2]='1';tDisBuf[3]=':';tDisBuf[4]='\0';
			for(i=0;i<8;i++)
			{
				Draw_Line(600, 60+(i*30),750,60+(i*30),1,BLACK);
				tDisBuf[2]=0x31+i;	
				tDisBuf[1]='o';
				lcd_text16(615,68+(i*30),BLACK,WHITE,tDisBuf);
			}
				Draw_Line(600, 60+(8*30),750,60+(8*30),1,BLACK);
				Draw_Line(450,420,750,420,1,BLACK);
				Draw_Line(450, 60,450,420,1,BLACK);
				Draw_Line(600, 60,600,420,1,BLACK);
				Draw_Line(750, 60,750,420,1,BLACK);
		break;
		case FRM_SETTING:
				LcdDrawRectangle( 30,150,120,330,0,0);
			  lcd_text16( 46,142,BLACK,WHITE,"波特率");
			  LcdDrawCircle(45,186,6,0,0);lcd_text16( 55,180,BLACK,WHITE,"1200bps");
				LcdDrawCircle(45,226,6,0,0);lcd_text16( 55,220,BLACK,WHITE,"2400bps");
			  LcdDrawCircle(45,266,6,0,0);lcd_text16( 55,260,BLACK,WHITE,"4800bps");	
			  LcdDrawCircle(45,306,6,0,0);lcd_text16( 55,300,BLACK,WHITE,"9600bps");
				tx=sysSavDat[0]-1;tx=tx*40;
				LcdDrawCircle(45,tx+186,3,0,1);

				LcdDrawRectangle(130,150,220,330,0,0);
			  lcd_text16(146,142,BLACK,WHITE,"数据位");
			  LcdDrawCircle(150,186,6,0,0);lcd_text16(160,180,BLACK,WHITE,"5位");
			  LcdDrawCircle(150,226,6,0,0);lcd_text16(160,220,BLACK,WHITE,"6位");
			  LcdDrawCircle(150,266,6,0,0);lcd_text16(160,260,BLACK,WHITE,"7位");
			  LcdDrawCircle(150,306,6,0,0);lcd_text16(160,300,BLACK,WHITE,"8位");
				tx=sysSavDat[1]-1;tx=tx*40;
				LcdDrawCircle(150,tx+186,3,0,1);
				//LcdDrawCircle(150,306,3,0,1);

				LcdDrawRectangle(230,150,320,330,0,0);
			  lcd_text16(246,142,BLACK,WHITE,"停止位");
			  LcdDrawCircle(250,186,6,0,0);lcd_text16(260,180,BLACK,WHITE,"1位");
			  LcdDrawCircle(250,226,6,0,0);lcd_text16(260,220,BLACK,WHITE,"1.5位");
			  LcdDrawCircle(250,266,6,0,0);lcd_text16(260,260,BLACK,WHITE,"2位");
				tx=sysSavDat[2]-1;tx=tx*40;
				LcdDrawCircle(250,tx+186,3,0,1);
				//LcdDrawCircle(250,186,3,0,1);

				LcdDrawRectangle(330,150,420,330,0,0);
			  lcd_text16(346,142,BLACK,WHITE,"校验位");
			  LcdDrawCircle(350,186,6,0,0);lcd_text16(360,180,BLACK,WHITE,"无校验");
			  LcdDrawCircle(350,226,6,0,0);lcd_text16(360,220,BLACK,WHITE,"奇校验");
			  LcdDrawCircle(350,266,6,0,0);lcd_text16(360,260,BLACK,WHITE,"偶校验");
				tx=sysSavDat[3]-1;tx=tx*40;
				LcdDrawCircle(350,tx+186,3,0,1);
				//LcdDrawCircle(350,186,3,0,1);

				LcdDrawRectangle( 20,120,430,340,0,0);
			  lcd_text16(177,112,BLACK,WHITE,"地磅通讯设置");
				LcdDrawRectangle(460,120,780,340,0,0);
			  lcd_text16(583,112,BLACK,WHITE,"监控通讯设置");

				LcdDrawRectangle(470,150,560,330,0,0);
			  lcd_text16(486,142,BLACK,WHITE,"地址");
				Draw_TextBox(490,183,50,24 );
			  disAddrBuf[0]='0'+(AddrIfix/10);
			  disAddrBuf[1]='0'+(AddrIfix%10);
				disAddrBuf[2]=0;
				if(disAddrBuf[0]=='0')
					disAddrBuf[0]=' ';
				lcd_text16(518,189,BLACK,WHITE,disAddrBuf);

				LcdDrawRectangle(570,150,670,330,0,0);
			  lcd_text16(586,142,BLACK,WHITE,"波特率");
			  LcdDrawCircle(585,186,6,0,0);lcd_text16(595,180,BLACK,WHITE,"2400bps");
				LcdDrawCircle(585,226,6,0,0);lcd_text16(595,220,BLACK,WHITE,"4800bps");
			  LcdDrawCircle(585,266,6,0,0);lcd_text16(595,260,BLACK,WHITE,"9600bps");	
			  LcdDrawCircle(585,306,6,0,0);lcd_text16(595,300,BLACK,WHITE,"19200bps");
				tx=sysSavDat[5]-1;tx=tx*40;
				LcdDrawCircle(585,tx+186,3,0,1);
				//LcdDrawCircle(585,306,3,0,1);

				LcdDrawRectangle(680,150,770,330,0,0);
			  lcd_text16(696,142,BLACK,WHITE,"校验位");
			  LcdDrawCircle(700,186,6,0,0);lcd_text16(710,180,BLACK,WHITE,"无校验");
			  LcdDrawCircle(700,226,6,0,0);lcd_text16(710,220,BLACK,WHITE,"奇校验");
			  LcdDrawCircle(700,266,6,0,0);lcd_text16(710,260,BLACK,WHITE,"偶校验");
				LcdDrawCircle(700,186,3,0,1);
				tx=sysSavDat[6]-1;tx=tx*40;
				LcdDrawCircle(700,tx+186,3,0,1);
				DrSetSel(0,0,1);
break;
		break;
	}
	__enable_irq();
}
extern char disWeiDat[7];
void Dis_RtDat(void)
{
	unsigned char i;
	
	float tflDat;
	unsigned char flgDiff=0;//如果采样值不同，则刷新显示
	unsigned char ti;
	unsigned char tempDat;//,tdat
	unsigned long tLngDat;
	unsigned int  tIntDat;
	char tDisBuf[20];
	//if(cntFvConn>100)
	//{
	//flgFvConn=1;
	//if(flgFvConn_pre!=flgFvConn)
	//{
	//	lcd_text24();
	//}
	//}
	for(i=0;i<10;i++)
	{
		if(flgConnSTA[i]!=flgConnSTA_pre[i])
		{
			if(flgConnSTA[i]==1)
			{
				
			}
		}
	}
	if((sys_STA>0)&&(sys_STA<6))//stop
{
	if(dualChNum==1)
	{
		canRtData[25]=canRtData[25]&0x0F;canRtData[25]=0x30|canRtData[25];
		canRtData[25]=canRtData[25]&0xF0;;
	}
	if(dualChNum==2)
	{
		canRtData[25]=canRtData[25]&0xF0;canRtData[25]=0x03|canRtData[25];
		canRtData[25]=canRtData[25]&0x0F;
	}
}	//working
if(sys_STA==7)//stop
{
	if(dualChNum==1)
	{
		canRtData[25]=canRtData[25]&0x0F;canRtData[25]=0x40|canRtData[25];
		canRtData[25]=canRtData[25]&0xF0;;
	}
	if(dualChNum==2)
	{
		canRtData[25]=canRtData[25]&0xF0;canRtData[25]=0x04|canRtData[25];
		canRtData[25]=canRtData[25]&0x0F;
	}
	if(flgFyAllow==1)
	{
		if(dualChNum==1) 
		{
		canRtData[25]=canRtData[25]&0x0F;canRtData[25]=0x20|canRtData[25];
		canRtData[25]=canRtData[25]&0xF0;;
		}
		if(dualChNum==2)
		{
			canRtData[25]=canRtData[25]&0xF0;canRtData[25]=0x02|canRtData[25];
			canRtData[25]=canRtData[25]&0x0F;
		}
	}
	
}	//stop
if((sys_STA==8)||(sys_STA==6))//end
{	
	if(dualChNum==1)
	{
		canRtData[25]=canRtData[25]&0x0F;canRtData[25]=0x50|canRtData[25];
		canRtData[25]=canRtData[25]&0xF0;;
	}
	if(dualChNum==2)
	{
		canRtData[25]=canRtData[25]&0xF0;canRtData[25]=0x05|canRtData[25];
		canRtData[25]=canRtData[25]&0x0F;
	}
}
if(sys_STA==0)//stop
{	
	canRtData[25]=0;	
	if(frmNum!=FRM_MAIN)
		flgFyAllow=0;
	if(flgFyAllow==1)
	{
		if(dualChNum==1) 		canRtData[25]=0x20|canRtData[25];
		if(dualChNum==2) 		canRtData[25]=0x02|canRtData[25];
	}
}	//idle

	tLngDat=(unsigned long)dYFL[0];
	tIntDat=tLngDat>>16;
	CanData[85]=(unsigned char)(tIntDat>>8);
	CanData[86]=(unsigned char)(tIntDat&0xFF);
	tIntDat=tLngDat&0xFFFF;
	CanData[83]=(unsigned char)(tIntDat>>8);
	CanData[84]=(unsigned char)(tIntDat&0xFF);
	tLngDat=(unsigned long)dYFL[1];
	tIntDat=tLngDat>>16;
	CanData[114]=(unsigned char)(tIntDat>>8);
	CanData[115]=(unsigned char)(tIntDat&0xFF);
	tIntDat=tLngDat&0xFFFF;
	CanData[112]=(unsigned char)(tIntDat>>8);
	CanData[113]=(unsigned char)(tIntDat&0xFF);
	if(frmNum==FRM_AMS)
	{
		tflDat=ctof(FvClibDat[2],FvClibDat[3],FvClibDat[0],FvClibDat[1]);
		tLngDat=(unsigned long)(tflDat*100000);
		tDisBuf[0]=0x30+tLngDat/10000000;
		   tLngDat=tLngDat%10000000;
		tDisBuf[1]=0x30+tLngDat/1000000;
		   tLngDat=tLngDat%1000000;
		tDisBuf[2]=0x30+tLngDat/100000;
		   tLngDat=tLngDat%100000;
		tDisBuf[4]=0x30+tLngDat/10000;
		   tLngDat=tLngDat%10000;
		tDisBuf[5]=0x30+tLngDat/1000;
		   tLngDat=tLngDat%1000;
		tDisBuf[6]=0x30+tLngDat/100;
		   tLngDat=tLngDat%100;
		tDisBuf[7]=0x30+tLngDat/10;
		   tLngDat=tLngDat%10;
		tDisBuf[8]=0x30+tLngDat;
		tDisBuf[9]=0;
		tDisBuf[3]='.';
		for(i=0;i<2;i++)
		{
			if(tDisBuf[i]=='0')
				tDisBuf[i]=' ';
			else
				break;
		}
		lcd_text16(260,100,0,0xFFFF,tDisBuf);
		
		tflDat=ctof(FvSetDat[2],FvSetDat[3],FvSetDat[0],FvSetDat[1]);
		tLngDat=(unsigned long)(tflDat*100000);
		tDisBuf[0]=0x30+tLngDat/10000000;
		   tLngDat=tLngDat%10000000;
		tDisBuf[1]=0x30+tLngDat/1000000;
		   tLngDat=tLngDat%1000000;
		tDisBuf[2]=0x30+tLngDat/100000;
		   tLngDat=tLngDat%100000;
		tDisBuf[4]=0x30+tLngDat/10000;
		   tLngDat=tLngDat%10000;
		tDisBuf[5]=0x30+tLngDat/1000;
		   tLngDat=tLngDat%1000;
		tDisBuf[6]=0x30+tLngDat/100;
		   tLngDat=tLngDat%100;
		tDisBuf[7]=0x30+tLngDat/10;
		   tLngDat=tLngDat%10;
		tDisBuf[8]=0x30+tLngDat;
		tDisBuf[9]=0;
		tDisBuf[3]='.';
		for(i=0;i<2;i++)
		{
			if(tDisBuf[i]=='0')
				tDisBuf[i]=' ';
			else
				break;
		}
		lcd_text16(260,140,0,0xFFFF,tDisBuf);
		tflDat=ctof(FvSetDat[6],FvSetDat[7],FvSetDat[4],FvSetDat[5]);
		tLngDat=(unsigned long)(tflDat*100000);
		tDisBuf[0]=0x30+tLngDat/10000000;
		   tLngDat=tLngDat%10000000;
		tDisBuf[1]=0x30+tLngDat/1000000;
		   tLngDat=tLngDat%1000000;
		tDisBuf[2]=0x30+tLngDat/100000;
		   tLngDat=tLngDat%100000;
		tDisBuf[4]=0x30+tLngDat/10000;
		   tLngDat=tLngDat%10000;
		tDisBuf[5]=0x30+tLngDat/1000;
		   tLngDat=tLngDat%1000;
		tDisBuf[6]=0x30+tLngDat/100;
		   tLngDat=tLngDat%100;
		tDisBuf[7]=0x30+tLngDat/10;
		   tLngDat=tLngDat%10;
		tDisBuf[8]=0x30+tLngDat;
		tDisBuf[9]=0;
		tDisBuf[3]='.';
		for(i=0;i<2;i++)
		{
			if(tDisBuf[i]=='0')
				tDisBuf[i]=' ';
			else
				break;
		}
		lcd_text16(260,180,0,0xFFFF,tDisBuf);
		tflDat=ctof(FvSetDat[10],FvSetDat[11],FvSetDat[8],FvSetDat[9]);
		tLngDat=(unsigned long)(tflDat*100000);
		tDisBuf[0]=0x30+tLngDat/10000000;
		   tLngDat=tLngDat%10000000;
		tDisBuf[1]=0x30+tLngDat/1000000;
		   tLngDat=tLngDat%1000000;
		tDisBuf[2]=0x30+tLngDat/100000;
		   tLngDat=tLngDat%100000;
		tDisBuf[4]=0x30+tLngDat/10000;
		   tLngDat=tLngDat%10000;
		tDisBuf[5]=0x30+tLngDat/1000;
		   tLngDat=tLngDat%1000;
		tDisBuf[6]=0x30+tLngDat/100;
		   tLngDat=tLngDat%100;
		tDisBuf[7]=0x30+tLngDat/10;
		   tLngDat=tLngDat%10;
		tDisBuf[8]=0x30+tLngDat;
		tDisBuf[9]=0;
		tDisBuf[3]='.';
		for(i=0;i<2;i++)
		{
			if(tDisBuf[i]=='0')
				tDisBuf[i]=' ';
			else
				break;
		}
		lcd_text16(260,220,0,0xFFFF,tDisBuf);
		tflDat=ctof(FvZeroDat[2],FvZeroDat[3],FvZeroDat[0],FvZeroDat[1]);
		tLngDat=(unsigned long)(tflDat*100000);
		tDisBuf[0]=0x30+tLngDat/10000000;
		   tLngDat=tLngDat%10000000;
		tDisBuf[1]=0x30+tLngDat/1000000;
		   tLngDat=tLngDat%1000000;
		tDisBuf[2]=0x30+tLngDat/100000;
		   tLngDat=tLngDat%100000;
		tDisBuf[4]=0x30+tLngDat/10000;
		   tLngDat=tLngDat%10000;
		tDisBuf[5]=0x30+tLngDat/1000;
		   tLngDat=tLngDat%1000;
		tDisBuf[6]=0x30+tLngDat/100;
		   tLngDat=tLngDat%100;
		tDisBuf[7]=0x30+tLngDat/10;
		   tLngDat=tLngDat%10;
		tDisBuf[8]=0x30+tLngDat;
		tDisBuf[9]=0;
		tDisBuf[3]='.';
		for(i=0;i<2;i++)
		{
			if(tDisBuf[i]=='0')
				tDisBuf[i]=' ';
			else
				break;
		}
		lcd_text16(660,220,0,0xFFFF,tDisBuf);

		tLngDat=(FvWarnDat[0]<<8)+FvWarnDat[1];
		if((tLngDat&0x8000)==0x8000)tDisBuf[0]='1';else tDisBuf[0]='0';
		if((tLngDat&0x4000)==0x4000)tDisBuf[1]='1';else tDisBuf[1]='0';
		if((tLngDat&0x2000)==0x2000)tDisBuf[2]='1';else tDisBuf[2]='0';
		if((tLngDat&0x1000)==0x1000)tDisBuf[3]='1';else tDisBuf[3]='0';
		if((tLngDat&0x0800)==0x0800)tDisBuf[4]='1';else tDisBuf[4]='0';
		if((tLngDat&0x0400)==0x0400)tDisBuf[5]='1';else tDisBuf[5]='0';
		if((tLngDat&0x0200)==0x0200)tDisBuf[6]='1';else tDisBuf[6]='0';
		if((tLngDat&0x0100)==0x0100)tDisBuf[7]='1';else tDisBuf[7]='0';
		if((tLngDat&0x0080)==0x0080)tDisBuf[8]='1';else tDisBuf[8]='0';
		if((tLngDat&0x0040)==0x0040)tDisBuf[9]='1';else tDisBuf[9]='0';
		if((tLngDat&0x0020)==0x0020)tDisBuf[10]='1';else tDisBuf[10]='0';
		if((tLngDat&0x0010)==0x0010)tDisBuf[11]='1';else tDisBuf[11]='0';
		if((tLngDat&0x0008)==0x0008)tDisBuf[12]='1';else tDisBuf[12]='0';
		if((tLngDat&0x0004)==0x0004)tDisBuf[13]='1';else tDisBuf[13]='0';
		if((tLngDat&0x0002)==0x0002)tDisBuf[14]='1';else tDisBuf[14]='0';
		if((tLngDat&0x0001)==0x0001)tDisBuf[15]='1';else tDisBuf[15]='0';
		tDisBuf[16]=0;
		lcd_text16(660,100,0,0xFFFF,tDisBuf);
		tLngDat=(FvWarnDat[2]<<8)+FvWarnDat[3];
		if((tLngDat&0x8000)==0x8000)tDisBuf[0]='1';else tDisBuf[0]='0';
		if((tLngDat&0x4000)==0x4000)tDisBuf[1]='1';else tDisBuf[1]='0';
		if((tLngDat&0x2000)==0x2000)tDisBuf[2]='1';else tDisBuf[2]='0';
		if((tLngDat&0x1000)==0x1000)tDisBuf[3]='1';else tDisBuf[3]='0';
		if((tLngDat&0x0800)==0x0800)tDisBuf[4]='1';else tDisBuf[4]='0';
		if((tLngDat&0x0400)==0x0400)tDisBuf[5]='1';else tDisBuf[5]='0';
		if((tLngDat&0x0200)==0x0200)tDisBuf[6]='1';else tDisBuf[6]='0';
		if((tLngDat&0x0100)==0x0100)tDisBuf[7]='1';else tDisBuf[7]='0';
		if((tLngDat&0x0080)==0x0080)tDisBuf[8]='1';else tDisBuf[8]='0';
		if((tLngDat&0x0040)==0x0040)tDisBuf[9]='1';else tDisBuf[9]='0';
		if((tLngDat&0x0020)==0x0020)tDisBuf[10]='1';else tDisBuf[10]='0';
		if((tLngDat&0x0010)==0x0010)tDisBuf[11]='1';else tDisBuf[11]='0';
		if((tLngDat&0x0008)==0x0008)tDisBuf[12]='1';else tDisBuf[12]='0';
		if((tLngDat&0x0004)==0x0004)tDisBuf[13]='1';else tDisBuf[13]='0';
		if((tLngDat&0x0002)==0x0002)tDisBuf[14]='1';else tDisBuf[14]='0';
		if((tLngDat&0x0001)==0x0001)tDisBuf[15]='1';else tDisBuf[15]='0';
		tDisBuf[16]=0;
		lcd_text16(660,140,0,0xFFFF,tDisBuf);
		tLngDat=(FvWarnDat[4]<<8)+FvWarnDat[5];
		if((tLngDat&0x8000)==0x8000)tDisBuf[0]='1';else tDisBuf[0]='0';
		if((tLngDat&0x4000)==0x4000)tDisBuf[1]='1';else tDisBuf[1]='0';
		if((tLngDat&0x2000)==0x2000)tDisBuf[2]='1';else tDisBuf[2]='0';
		if((tLngDat&0x1000)==0x1000)tDisBuf[3]='1';else tDisBuf[3]='0';
		if((tLngDat&0x0800)==0x0800)tDisBuf[4]='1';else tDisBuf[4]='0';
		if((tLngDat&0x0400)==0x0400)tDisBuf[5]='1';else tDisBuf[5]='0';
		if((tLngDat&0x0200)==0x0200)tDisBuf[6]='1';else tDisBuf[6]='0';
		if((tLngDat&0x0100)==0x0100)tDisBuf[7]='1';else tDisBuf[7]='0';
		if((tLngDat&0x0080)==0x0080)tDisBuf[8]='1';else tDisBuf[8]='0';
		if((tLngDat&0x0040)==0x0040)tDisBuf[9]='1';else tDisBuf[9]='0';
		if((tLngDat&0x0020)==0x0020)tDisBuf[10]='1';else tDisBuf[10]='0';
		if((tLngDat&0x0010)==0x0010)tDisBuf[11]='1';else tDisBuf[11]='0';
		if((tLngDat&0x0008)==0x0008)tDisBuf[12]='1';else tDisBuf[12]='0';
		if((tLngDat&0x0004)==0x0004)tDisBuf[13]='1';else tDisBuf[13]='0';
		if((tLngDat&0x0002)==0x0002)tDisBuf[14]='1';else tDisBuf[14]='0';
		if((tLngDat&0x0001)==0x0001)tDisBuf[15]='1';else tDisBuf[15]='0';
		tDisBuf[16]=0;
		lcd_text16(660,180,0,0xFFFF,tDisBuf);
	
		tLngDat=staValv[0];
		if((tLngDat&0x8000)==0x8000)tDisBuf[0]='1';else tDisBuf[0]='0';
		if((tLngDat&0x4000)==0x4000)tDisBuf[1]='1';else tDisBuf[1]='0';
		if((tLngDat&0x2000)==0x2000)tDisBuf[2]='1';else tDisBuf[2]='0';
		if((tLngDat&0x1000)==0x1000)tDisBuf[3]='1';else tDisBuf[3]='0';
		if((tLngDat&0x0800)==0x0800)tDisBuf[4]='1';else tDisBuf[4]='0';
		if((tLngDat&0x0400)==0x0400)tDisBuf[5]='1';else tDisBuf[5]='0';
		if((tLngDat&0x0200)==0x0200)tDisBuf[6]='1';else tDisBuf[6]='0';
		if((tLngDat&0x0100)==0x0100)tDisBuf[7]='1';else tDisBuf[7]='0';
		if((tLngDat&0x0080)==0x0080)tDisBuf[8]='1';else tDisBuf[8]='0';
		if((tLngDat&0x0040)==0x0040)tDisBuf[9]='1';else tDisBuf[9]='0';
		if((tLngDat&0x0020)==0x0020)tDisBuf[10]='1';else tDisBuf[10]='0';
		if((tLngDat&0x0010)==0x0010)tDisBuf[11]='1';else tDisBuf[11]='0';
		if((tLngDat&0x0008)==0x0008)tDisBuf[12]='1';else tDisBuf[12]='0';
		if((tLngDat&0x0004)==0x0004)tDisBuf[13]='1';else tDisBuf[13]='0';
		if((tLngDat&0x0002)==0x0002)tDisBuf[14]='1';else tDisBuf[14]='0';
		if((tLngDat&0x0001)==0x0001)tDisBuf[15]='1';else tDisBuf[15]='0';
		tDisBuf[16]=0;
		lcd_text16(660,340,0,0xFFFF,tDisBuf);
		
		tLngDat=staValv[1];
		if((tLngDat&0x8000)==0x8000)tDisBuf[0]='1';else tDisBuf[0]='0';
		if((tLngDat&0x4000)==0x4000)tDisBuf[1]='1';else tDisBuf[1]='0';
		if((tLngDat&0x2000)==0x2000)tDisBuf[2]='1';else tDisBuf[2]='0';
		if((tLngDat&0x1000)==0x1000)tDisBuf[3]='1';else tDisBuf[3]='0';
		if((tLngDat&0x0800)==0x0800)tDisBuf[4]='1';else tDisBuf[4]='0';
		if((tLngDat&0x0400)==0x0400)tDisBuf[5]='1';else tDisBuf[5]='0';
		if((tLngDat&0x0200)==0x0200)tDisBuf[6]='1';else tDisBuf[6]='0';
		if((tLngDat&0x0100)==0x0100)tDisBuf[7]='1';else tDisBuf[7]='0';
		if((tLngDat&0x0080)==0x0080)tDisBuf[8]='1';else tDisBuf[8]='0';
		if((tLngDat&0x0040)==0x0040)tDisBuf[9]='1';else tDisBuf[9]='0';
		if((tLngDat&0x0020)==0x0020)tDisBuf[10]='1';else tDisBuf[10]='0';
		if((tLngDat&0x0010)==0x0010)tDisBuf[11]='1';else tDisBuf[11]='0';
		if((tLngDat&0x0008)==0x0008)tDisBuf[12]='1';else tDisBuf[12]='0';
		if((tLngDat&0x0004)==0x0004)tDisBuf[13]='1';else tDisBuf[13]='0';
		if((tLngDat&0x0002)==0x0002)tDisBuf[14]='1';else tDisBuf[14]='0';
		if((tLngDat&0x0001)==0x0001)tDisBuf[15]='1';else tDisBuf[15]='0';
		tDisBuf[16]=0;
		lcd_text16(660,380,0,0xFFFF,tDisBuf);

		tLngDat=(unsigned long)(valvOpen[0]);
		tDisBuf[0]=0x30+tLngDat/100;
		   tLngDat=tLngDat%10000000;
		tDisBuf[1]=0x30+tLngDat/1000000;
		   tLngDat=tLngDat%1000000;
		tDisBuf[2]=0x30+tLngDat/100000;
		   tLngDat=tLngDat%100000;
		tDisBuf[4]=0x30+tLngDat/10000;
		   tLngDat=tLngDat%10000;
		tDisBuf[5]=0x30+tLngDat/1000;
		   tLngDat=tLngDat%1000;
		tDisBuf[6]=0x30+tLngDat/100;
		   tLngDat=tLngDat%100;
		tDisBuf[7]=0x30+tLngDat/10;
		   tLngDat=tLngDat%10;
		tDisBuf[8]=0x30+tLngDat;
		tDisBuf[9]=0;
		tDisBuf[3]='.';
		for(i=0;i<2;i++)
		{
			if(tDisBuf[i]=='0')
				tDisBuf[i]=' ';
			else
				break;
		}
		lcd_text16(260,340,0,0xFFFF,tDisBuf);
	
	}
	if(frmNum==FRM_MON)
	{
		if(dSSLv_pre!=dSSLv)
		{re_SSL(0,dSSLv);dSSLv_pre=dSSLv;}
		if(dSSLm_pre!=dSSLm)
		{re_SSL(1,dSSLm);dSSLm_pre=dSSLm;}
		if(dLJSv_pre!=dLJSv)
		{re_LJS(0,dLJSv);dLJSv_pre=dLJSv;}
		if(dLJSm_pre!=dLJSm)
		{re_LJS(1,dLJSm);dLJSm_pre=dLJSm;}//re_LJS(0,dLJSv);re_LJS(1,dLJSm);
		if(dWD_pre!=dWD)
		{re_WD(0,dWD);dWD_pre=dWD;}		
		if(dMD_pre!=dMD)
		{re_MD(0,dMD);dMD_pre=dMD;}
		re_monSFL(0,0.0);
		re_monSFL(1,0.0);
	 //if(dualChNum==1) {re_monSFL(0,dSFLv);	 re_monSFL(1,0.0);}
	 //if(dualChNum==2) {re_monSFL(0,0.0);	 re_monSFL(1,dSFLv);}
	 //DI1-PE1 DI2-OV1 DI3-ZFOP1 DI4-ZFCL1 DI5-FFOP1 DI6-FFCL1 DI7-NC DI8-NC
	 //DI1-PE2 DI2-OV2 DI3-ZFOP2 DI4-ZFCL2 DI5-FFOP2 DI6-FFCL2 DI7-NC DI8-NC
				if((rioDat[0]&0x01)==0x01)TFT_Fill(151+0*42,409,151+0*42+16,409+16,BLUE); //静电1
				else 				              TFT_Fill(151+0*42,409,151+0*42+16,409+16,RED);//DI1
			
				if((rioDat[0]&0x02)==0x02)TFT_Fill(151+1*42,409,151+1*42+16,409+16,BLUE); //主阀1关回讯
				else 				              TFT_Fill(151+1*42,409,151+1*42+16,409+16,RED);//DI9
			
				if((rioDat[0]&0x04)==0x04)TFT_Fill(151+2*42,409,151+2*42+16,409+16,RED);//回阀1开回讯
				else 				              TFT_Fill(151+2*42,409,151+2*42+16,409+16,BLUE);//DI4
			
				if((rioDat[0]&0x08)==0x08)TFT_Fill(151+3*42,409,151+3*42+16,409+16,RED);//回阀1关回讯
				else 				              TFT_Fill(151+3*42,409,151+3*42+16,409+16,BLUE);//DI10
			
				if((rioDat[0]&0x10)==0x10)TFT_Fill(151+4*42,409,151+4*42+16,409+16,RED);//静电1报警
				else 				              TFT_Fill(151+4*42,409,151+4*42+16,409+16,BLUE);//DI1
	
				if((rioDat[0]&0x20)==0x20)TFT_Fill(151+5*42,409,151+5*42+16,409+16,RED);//溢流1报警
				else 				              TFT_Fill(151+5*42,409,151+5*42+16,409+16,BLUE);//DI2
			
				if((rioDat[1]&0x01)==0x01)TFT_Fill(551+0*42,409,551+0*42+16,409+16,BLUE);//主阀1开回讯
				else 				              TFT_Fill(551+0*42,409,551+0*42+16,409+16,RED);//DI3
			
				if((rioDat[1]&0x02)==0x02)TFT_Fill(551+1*42,409,551+1*42+16,409+16,BLUE);//主阀1关回讯
				else 				              TFT_Fill(551+1*42,409,551+1*42+16,409+16,RED);//DI9
			
				if((rioDat[1]&0x04)==0x04)TFT_Fill(551+2*42,409,551+2*42+16,409+16,RED);//回阀1开回讯
				else 				              TFT_Fill(551+2*42,409,551+2*42+16,409+16,BLUE);//DI4
			
				if((rioDat[1]&0x08)==0x08)TFT_Fill(551+3*42,409,551+3*42+16,409+16,RED);//回阀1关回讯
				else 				              TFT_Fill(551+3*42,409,551+3*42+16,409+16,BLUE);//DI10
			
				if((rioDat[1]&0x10)==0x10)TFT_Fill(551+4*42,409,551+4*42+16,409+16,RED);//静电1报警
				else 				              TFT_Fill(551+4*42,409,551+4*42+16,409+16,BLUE);//DI1
	
				if((rioDat[1]&0x20)==0x20)TFT_Fill(551+5*42,409,551+5*42+16,409+16,RED);//溢流1报警
				else 				              TFT_Fill(551+5*42,409,551+5*42+16,409+16,BLUE);//DI2
	}
//	if(frmNum==FRM_MAN)
//	{
//		if(flgFVerr==1)
//		{
//			lcd_text16(240,75+6*40,0,WHITE,"FV off Line!");
//			lcd_text16(400,75+6*40,0,WHITE,"                    ");
//		}
//		else
//		{
//			tIntDat=(unsigned long)(dSSLv*10.0);
//			tDisBuf[0]='0'+tIntDat/100000;
//			       tIntDat=tIntDat%100000;
//			tDisBuf[1]='0'+tIntDat/10000;
//			       tIntDat=tIntDat%10000;
//			tDisBuf[2]='0'+tIntDat/1000;
//			       tIntDat=tIntDat%1000;
//			tDisBuf[3]='0'+tIntDat/100;
//			       tIntDat=tIntDat%100;
//			tDisBuf[4]='0'+tIntDat/10;
//			       tIntDat=tIntDat%10;
//			tDisBuf[5]='.';
//			tDisBuf[6]='0'+tIntDat;
//			tDisBuf[7]=0;
//			lcd_text16(240,75+6*40,0,WHITE,"FV  on Line!");
//			lcd_text16(400,75+6*40,0,WHITE,tDisBuf);
//		}
//		if(flgCANerr==1)
//		{
//			lcd_text16(240,65+5*40,0,WHITE,"CAN off Line!");
//		}
//		else
//		{
//			lcd_text16(240,65+5*40,0,WHITE,"CAN  on Line!");
//		}
//		if(flgIFIXerr==1)
//		{
//			lcd_text16(240,55+6*40,0,WHITE,"IFIX off Line!");
//		}
//		else
//		{
//			lcd_text16(240,55+6*40,0,WHITE,"IFIX  on Line!");
//		}
//		tempDat=rioDat[1];//GetDiDat();
//		for(ti=0;ti<8;ti++)
//		{
//			if((tempDat&(1<<(ti)))==(1<<(ti)))
//			  Draw_Lamp(74,55+ti*30,1);//Draw_Lamp(20+55+26+ti*80,80,1);
//			else
//				Draw_Lamp(74,55+ti*30,0);//Draw_Lamp(20+55+26+ti*80,80,0);
//		}
//		tempDat=rioDat[0];//DiDatHi;//GetDiDat();
//		for(ti=0;ti<4;ti++)
//		{
//			if((tempDat&(1<<(ti)))==(1<<(ti)))
//			  Draw_Lamp(74,55+(8+ti)*30,1);//Draw_Lamp(20+55+26+ti*80,80,1);
//			else
//				Draw_Lamp(74,55+(8+ti)*30,0);//Draw_Lamp(20+55+26+ti*80,80,0);
//		}
//		tempDat=rioDat[2];////GetDoDat();
//		for(ti=0;ti<8;ti++)
//		{
//			if((tempDat&(1<<(ti)))==(1<<(ti)))
//				Draw_Lamp(154,75+ti*40,1);//Draw_Lamp(20+55+26+ti*80,150,1);
//			else
//				Draw_Lamp(154,75+ti*40,0);//Draw_Lamp(20+55+26+ti*80,150,0);
//		}
//			Dis_Num16(240,75+2*40,0,WHITE,rio_uw_AiDat[0]);
//			Dis_Num16(240,75+3*40,0,WHITE,rio_uw_AiDat[1]);
//			Dis_Num16(330,75+2*40,0,WHITE,rio_uw_FiDat[0]);
//			Dis_Num16(330,75+3*40,0,WHITE,rio_uw_FiDat[1]);
//	}
	for(i=0;i<10;i++)
	{
	  if(flgAlert[i])
		{
			if(flgAlertCnt[i]<0xFF)
			{	flgAlertCnt[i]++;flgAlertTmr[i]=1;}
			else
				flgAlertTmr[i]=1;
		}
		else
		{
		  flgAlertCnt[i]=0;
			flgAlertTmr[i]=0;
		}
	}
	if((frmNum==FRM_MAIN)||(frmNum==FRM_SD))
	{
		if((dualChNum==1)||(dualChNum==2))
		{
			if((rioDat[dualChNum-1]&0x01)==0x01)TFT_Fill(416+0*48,375,432+0*48,375+16,BLUE); //静电1
			else 				              TFT_Fill(416+0*48,375,432+0*48,375+16,RED);//DI1
		
			if((rioDat[dualChNum-1]&0x02)==0x02)TFT_Fill(416+1*48,375,432+1*48,375+16,BLUE); //主阀1关回讯
			else 				              TFT_Fill(416+1*48,375,432+1*48,375+16,RED);//DI9
		
			if((rioDat[dualChNum-1]&0x04)==0x04)TFT_Fill(416+2*48,375,432+2*48,375+16,RED);//回阀1开回讯
			else 				              TFT_Fill(416+2*48,375,432+2*48,375+16,BLUE);//DI4
		
			if((rioDat[dualChNum-1]&0x08)==0x08)TFT_Fill(416+3*48,375,432+3*48,375+16,RED);//回阀1关回讯
			else 				              TFT_Fill(416+3*48,375,432+3*48,375+16,BLUE);//DI10
		
			if((rioDat[dualChNum-1]&0x10)==0x10)TFT_Fill(416+4*48,375,432+4*48,375+16,RED);//静电1报警
			else 				              TFT_Fill(416+4*48,375,432+4*48,375+16,BLUE);//DI1

			if((rioDat[dualChNum-1]&0x20)==0x20)TFT_Fill(416+5*48,375,432+5*48,375+16,RED);//溢流1报警
			else 				              TFT_Fill(416+5*48,375,432+5*48,375+16,BLUE);//DI2
		}
		//此处AMS联锁
		//TBD
		
		if(dSSLv_pre!=dSSLv)
		{re_SSL(2,dSSLv);dSSLv_pre=dSSLv;}
		if(dSSLm_pre!=dSSLm)
		{re_SSL(3,dSSLm);dSSLm_pre=dSSLm;}
		if(dLJSv_pre!=dLJSv)
		{re_LJS(2,dLJSv);dLJSv_pre=dLJSv;}
		if(dLJSm_pre!=dLJSm)
		{re_LJS(3,dLJSm);dLJSm_pre=dLJSm;}//re_LJS(0,dLJSv);re_LJS(1,dLJSm);
		if(dWD_pre!=dWD)
		{re_WD(2,dWD);dWD_pre=dWD;}		
		if(dMD_pre!=dMD)
		{re_MD(2,dMD);dMD_pre=dMD;}
		if(dualChNum==1)
		{		
				Draw_AoBar(valvOpen[0]);
				if(sysWarnLed[0]!=sysWarnLed_pre[0])	
				{
					WrWarnLed(0,sysWarnLed[0]);
					sysWarnLed_pre[0]=sysWarnLed[0];
				}
		}
		if(dualChNum==2)
		{		
				Draw_AoBar(valvOpen[1]);
				if(sysWarnLed[1]!=sysWarnLed_pre[1])	
				{
					WrWarnLed(1,sysWarnLed[1]);
					sysWarnLed_pre[1]=sysWarnLed[1];
				}
		}
		//re_YL(rtAi);//re_YL(0.0);
		//re_LS();
		//rtSSL=dSSLv;
		//tPress=(float)SavConfDat[4];

		if((sys_STA>0)||((frmNum==FRM_SD)&&(flgSdStart==1)))
		{	
			dSFLv=dLJSv-dLJSv_first;dSFLm=dLJSm-dLJSm_first;
			if(dSFLv<0.0)dSFLv=0.0;if(dSFLm<0.0)dSFLm=0.0;
				re_SFL_float(dSFLv);
			canSFLv[dualChNum-1]=dSFLv;
		}
	}
	if(frmNum==FRM_AMS)
	{
		//if(TH_Msg_Pre[0].fvMass	!=TH_Msg[0].fvMass)
		//{  TH_Msg_Pre[0].fvMass	 =TH_Msg[0].fvMass;flgDiff=1;}
		//if(TH_Msg_Pre[0].fvMD	  !=TH_Msg[0].fvMD)
		//{  TH_Msg_Pre[0].fvMD	   =TH_Msg[0].fvMD;flgDiff=1;}
		//if(TH_Msg_Pre[0].fvWD 	!=TH_Msg[0].fvWD)
		//{  TH_Msg_Pre[0].fvWD	   =TH_Msg[0].fvWD;flgDiff=1;}
		//if(TH_Msg_Pre[0].fvLJSm	!=TH_Msg[0].fvLJSm)
		//{  TH_Msg_Pre[0].fvLJSm	 =TH_Msg[0].fvLJSm;flgDiff=1;}
		//if(TH_Msg_Pre[0].fvZero	!=TH_Msg[0].fvZero)
		//{  TH_Msg_Pre[0].fvZero	 =TH_Msg[0].fvZero;flgDiff=1;}
		//if((flgDiff))
		//{
		//	if((frmNum==FRM_AMS))
		//		{reDrawDetail();DrawDiagLamp();}
		//}
	}
}
void Dis_RtDat_(void)
{
	unsigned char i,flgDatOk=1;
	char disBuf[10];
	if(frmNum==FRM_MAIN)
	{
		for(i=0;i<6;i++)
		{
			disBuf[i]=disWeiDat[i];
			if(((disBuf[i]<'0')||(disBuf[i]>'9'))&&(disBuf[i]!=0x20))
			{
				disBuf[i]=' ';
				flgDatOk=0;
				break;
			}
		}		
		disBuf[6]=' ';disBuf[7]='K';disBuf[8]='g';disBuf[9]=0;
		if(flgDatOk==1)
			lcd_text24(79,385,BLACK,WHITE,disBuf);		
	}
}

void Dis_Lcd(void)
{
	unsigned char ti;
	if(frmNumLast!=frmNum)
	{
		flgDisDate=1;
		flgDisTime=1;
		dSSLv_pre=-999.9;dSSLm_pre=-999.9;
		dLJSv_pre=-9.9;dLJSm_pre=-9.9;
		dWD_pre=-99.9;dMD_pre=-9.99;
		if(dualChNum>0)
			WrWarnLed(dualChNum-1,0);

		if((frmNum==FRM_MAIN))
		{
			flgReNew=1;flgFyAllowPre=0xFF;
			Fnd_PendLst();
		}
		Dis_Scr(frmNum);
		sys_STA_Last=0xFF;
		//RxdEvents();
		frmNumLast=frmNum;

	}
}
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

float ctof(unsigned char dt1,unsigned char dt2,unsigned char dt3,unsigned char dt4)
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


void lcd_en48(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	DisEN24(s, x,  y, fc, bc,0);
}
void Draw_EN24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	DisEN24(s, x,  y, fc, bc,0);
}
void Dis_Pad(unsigned char tCar)
{
	unsigned char ti;
	char ttBuf[20];
	char PadCode[8];
	char HZ[62]="冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽粤川青藏琼宁渝京津沪贵";
	//char ZM[52]="ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ";
	//char SZ[20]="０１２３４５６７８９";
	if(CarId[0]==0xAA)//车号无效
	 tCar=0;
	if((CarId[0]==0)&&(CarId[1]==0)&&(CarId[2]==0)&&(CarId[3]==0)&&(CarId[4]==0)&&(CarId[5]==0)&&(CarId[6]==0))
		tCar=0;
	if(tCar)
	{
	for(ti=0;ti<8;ti++)
		PadCode[ti]=CarId[ti];
//冀豫云辽黑湘皖鲁新苏
//浙赣鄂桂甘晋蒙陕吉闽
//粤川青藏琼宁渝京津沪
	if(frmNum==FRM_MAIN)
	{
	  TFT_Fill(240,354,395,383,BLUE);
		if(PadCode[0]<=0x1d)
		{
			ttBuf[0]=HZ[PadCode[0]*2];
			ttBuf[1]=HZ[PadCode[0]*2+1];
			ttBuf[2]=0;
			lcd_text24(245,355,0xFFFF,BLUE,ttBuf);
			for(ti=0;ti<8;ti++)
				ttBuf[ti]=PadCode[1+ti];
			ttBuf[8]=0;
			Draw_EN24(245+24,355,0xFFFF,BLUE,ttBuf);
		}
		else 
		{
			for(ti=0;ti<8;ti++)
				ttBuf[ti]=PadCode[ti];
			ttBuf[8]=0;
	  	 TFT_Fill(240,354,395,383,BLUE);
			Draw_EN24(245,355,0xFFFF,BLUE,ttBuf);
		}
	}
 }
 else
 {
	if(frmNum==FRM_MAIN)
	{
	  	 TFT_Fill(240,354,395,383,BLUE);
	}
 }
}

void Dis_EN_FY(unsigned char val)
{
	unsigned int tx,ty;
	tx=365+2;ty=385+2;
  if(val==0)
	{
		TFT_Fill(tx,ty,tx+32,ty+32,LGRAY);
		lcd_text16( tx,ty,WHITE,LGRAY,"发油");
		lcd_text16( tx,ty+16,WHITE,LGRAY,"允许");
	
	}
	else
	{
		TFT_Fill(tx,ty,tx+32,ty+32,0);
		lcd_text16( tx,ty,GREEN,BLACK,"发油");
		lcd_text16( tx,ty+16,GREEN,BLACK,"允许");
	}	
}
void Dis_AMS(void)
{
	lcd_text16(360, 60,0,0xFFFF,"流量计诊断");
	lcd_text16(100,100,0,0xFFFF,"流量校准系数");
	lcd_text16(100,140,0,0xFFFF,"质量流量仪表系数");
	lcd_text16(100,180,0,0xFFFF,"体积流量仪表系数");
	lcd_text16(100,220,0,0xFFFF,"密度仪表系数");
	lcd_text16(500,100,0,0xFFFF,"流量计报警系数1");
	lcd_text16(500,140,0,0xFFFF,"流量计报警系数2");
	lcd_text16(500,180,0,0xFFFF,"流量计报警系数3");
	lcd_text16(500,220,0,0xFFFF,"零点值");
	Draw_Line(0, 300,799, 300,2,BLACK);
	lcd_text16(360, 305,0,0xFFFF,"电动阀诊断");
	lcd_text16(100, 340,0,0xFFFF,"阀门1开度");
	lcd_text16(100, 380,0,0xFFFF,"阀门2开度");
	lcd_text16(500, 340,0,0xFFFF,"阀门1状态");
	lcd_text16(500, 380,0,0xFFFF,"阀门2状态");
}
void Dis_Pend(unsigned char id)//新待发提单，有车号显示
{
	char tBuf[25];
	char tDisBuf[25];
//记录存储在10扇区与11扇区中
//	unsigned char tCrC;
	unsigned char ti,tj,i;
	unsigned char tflg[10];	//标志位，0为记录无效，1为记录有效
//	unsigned int tPendAddr[10];
	unsigned long  uwyfl;

	for(ti=0;ti<10;ti++)
	{
		if(id==1)
			EepRd((250+PendAdd[ti]),25);//EepRd(tPendAddr[ti],25);
		else
			EepRd(PendAdd[ti],25);//EepRd(tPendAddr[ti],25);
		for(i=0;i<25;i++)
			tBuf[i]=EepRdBuf[i];
		tflg[ti]=1;
		if(tBuf[0]!=(0xF1+ti))//记录号为0xF0+id
			tflg[ti]=0;
		if(tflg[ti]>0)
		{
		//记录号正确
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
	 
	for(ti=0;ti<10;ti++)
	{
		//lcd_text16( 32,118+ti*30,0,0xFFFF,"    ");
		//lcd_text16( 100,118+ti*30,0,0xFFFF,"          ");
		//lcd_text16( 280,118+ti*30,0,0xFFFF,"          ");
		//lcd_text16( 520,118+ti*30,0,0xFFFF,"          ");
	  TFT_Fill(  32,118+ti*30, 32+32,134+ti*30,0xFFFF);
	  TFT_Fill( 100,118+ti*30,100+80,134+ti*30,0xFFFF);
	  TFT_Fill( 280,118+ti*30,280+80,134+ti*30,0xFFFF);
	  TFT_Fill( 410,118+ti*30,410+45,134+ti*30,0xFFFF);
	  TFT_Fill( 520,118+ti*30,520+80,134+ti*30,0xFFFF);
	  TFT_Fill( 680,111+ti*30,780,139+ti*30,0xFFFF);
	}	 
	for(ti=0;ti<10;ti++)
	{
		if(id==1)
			EepRd((250+PendAdd[ti]),25);
		else
			EepRd(PendAdd[ti],25);
		for(i=0;i<25;i++)
			tBuf[i]=EepRdBuf[i];
		for(tj=0;tj<8;tj++)
		  PendCarId[tj]=tBuf[10+tj];
//		EEP_Read(tBuf,PendAdd[ti],20);
		if((tflg[ti]==0)||((tBuf[0]&0x0F)>0x0A))
		{
			//lcd_text16( 32,118+ti*30,0xFFFF,0xFFFF,"    ");
			//lcd_text16( 100,118+ti*30,0xFFFF,0xFFFF,"          ");
			//lcd_text16( 280,118+ti*30,0xFFFF,0xFFFF,"          ");
			//lcd_text16( 520,118+ti*30,0xFFFF,0xFFFF,"          ");
			TFT_Fill(  32,118+ti*30, 32+32,134+ti*30,0xFFFF);
			TFT_Fill( 100,118+ti*30,100+80,134+ti*30,0xFFFF);
			TFT_Fill( 280,118+ti*30,280+80,134+ti*30,0xFFFF);
			TFT_Fill( 410,118+ti*30,410+45,134+ti*30,0xFFFF);
			TFT_Fill( 520,118+ti*30,520+80,134+ti*30,0xFFFF);
			for(tj=0;tj<9;tj++)
			  PendCarId[tj]=0;
			//Draw_Cap( 680,116+ti*30,60,20,"        ",8);
			//Draw_Button_Cap( 700,116+ti*30,60,18,"删除",2);
			break;
		}	
		else
		{
		tDisBuf[0]='0'+((tBuf[0]&0x0F)/10);
		tDisBuf[1]='0'+((tBuf[0]&0x0F)%10);
		tDisBuf[2]=0;
		//Draw_Cap( 34,110+ti*30,40,30,tDisBuf,2);//序号
		lcd_text16( 34,118+ti*30,0,0xffff,tDisBuf);//序号
		/*PendCarId[0]=' ';*/PendCarId[8]=0;
		lcd_text16(100,118+ti*30,0,0xffff,PendCarId);
		if(tBuf[19]==1)
			lcd_text16(419,118+ti*30,0,0xffff,"1");
		if(tBuf[19]==2)
			lcd_text16(419,118+ti*30,0,0xffff,"2");
		//Dis_PendPad(ti);
		tDisBuf[0]='0'+(tBuf[1]>>4);
		tDisBuf[1]='0'+(tBuf[1]&0x0F);
		tDisBuf[2]='0'+(tBuf[2]>>4);
		tDisBuf[3]='0'+(tBuf[2]&0x0F);
		tDisBuf[4]='0'+(tBuf[3]>>4);
		tDisBuf[5]='0'+(tBuf[3]&0x0F);
		tDisBuf[6]='0'+(tBuf[4]>>4);
		tDisBuf[7]='0'+(tBuf[4]&0x0F);
		tDisBuf[8]=0;
		//Draw_Cap( 260,110+ti*30,200,30,tDisBuf,8);//提单号
		lcd_text16( 280,118+ti*30,0,0xffff,tDisBuf);//提单号
		tDisBuf[0]='0'+(tBuf[6]>>4);
		tDisBuf[1]='0'+(tBuf[6]&0x0F);
		tDisBuf[2]='0'+(tBuf[7]>>4);
		tDisBuf[3]='0'+(tBuf[7]&0x0F);
		tDisBuf[4]='0'+(tBuf[8]>>4);
		tDisBuf[5]='0'+(tBuf[8]&0x0F);
		tDisBuf[6]='0'+(tBuf[9]>>4);
		//tDisBuf[7]='0'+(tBuf[9]&0x0F);
		tDisBuf[7]=0;
		tDisBuf[8]=0;
	  Draw_Button_Cap( 700,116+ti*30,60,18,"删除",2);
		}
		for(tj=0;tj<8;tj++)
		{
			if(tDisBuf[tj]=='0')
				tDisBuf[tj]=' ';
			else
				break;
		}
		//Draw_Cap( 470,110+ti*30,200,30,tDisBuf,8);//应发量
		lcd_text16( 520,118+ti*30,0,0xffff,tDisBuf);//应发量
	}
}
void Draw_AoBar(unsigned int tvalue)
{
	char tdig[5]={' ',' ',' ','%','\0'};
	unsigned int tx,ty,tL;
	ty=tvalue;
	tdig[0]=ty/100;
	ty=ty%100;
	tdig[1]=ty/10;
	ty=ty%10;
	tdig[2]=0x30+ty;
	if(tdig[0]==0)		 
	{	
		tdig[0]=' ';
		if(tdig[1]==0)
			{tdig[1]=' ';}
		else
			{tdig[1]+=0x30;}
	}
	else
	{tdig[0]+=0x30;tdig[1]+=0x30;}
	TFT_Fill(648,400,648+38,416,WHITE);
	lcd_text16(648,400,BLUE,WHITE,tdig);
}

/*
AO输出指示条
输入0~0xFFF
输出3.6mA~20mA
*/
void Draw_AoBar__(unsigned int tvalue)
{
	unsigned int tLeft,tTop;
	unsigned int tx,ty,tL;
	float tk;
	char tdig[5]={' ',' ',' ','%','\0'};
	tLeft=509+68;tTop=388;
	TFT_Fill(tLeft-35+50,tTop,tLeft-11,tTop+16,0xFFFF);
	ty=tvalue;
	if(ty>100)
	{	 ty=100;	}
	tx=ty;tL=ty;
	tdig[0]=ty/100;
	ty=ty%100;
	tdig[1]=ty/10;
	ty=ty%10;
	tdig[2]=0x30+ty;
	if(tdig[0]==0)		 
	{	
		tdig[0]=' ';
		if(tdig[1]==0)
			{tdig[1]=' ';}
		else
			{tdig[1]+=0x30;}
	}
	else
	{tdig[0]+=0x30;tdig[1]+=0x30;}
	lcd_text16(tLeft-35,tTop,BLUE,WHITE,tdig);
	Draw_Rect(tLeft+1,tTop,100-1,15,2);
//	Draw_Line(tLeft+  0,tTop,tLeft+  0,tTop-9,2,BLACK);
//	Draw_Line(tLeft+ 18,tTop,tLeft+ 18,tTop-4,2,BLACK);
//	Draw_Line(tLeft+ 37,tTop,tLeft+ 37,tTop-6,2,BLACK);
//	Draw_Line(tLeft+ 56,tTop,tLeft+ 56,tTop-4,2,BLACK);
//	Draw_Line(tLeft+ 74,tTop,tLeft+ 74,tTop-9,2,BLACK);
//	Draw_Line(tLeft+ 92,tTop,tLeft+ 92,tTop-4,2,BLACK);
//	Draw_Line(tLeft+111,tTop,tLeft+111,tTop-6,2,BLACK);
//	Draw_Line(tLeft+130,tTop,tLeft+130,tTop-4,2,BLACK);
//	Draw_Line(tLeft+148,tTop,tLeft+148,tTop-9,2,BLACK);
	if(tL>0)
	{
		tx=tLeft+(tL*100+4)/100;
		TFT_Fill(tLeft+2,tTop+1,tx,tTop+14,BLUE);
		if(tL!=100)
			TFT_Fill(tx+2,tTop+1,tLeft+99,tTop+14,WHITE);
	}
	else
	{	TFT_Fill(tLeft+2,tTop+1,tLeft+99,tTop+14,WHITE);}
}

//void Draw_AoBar(unsigned int tvalue)
//{
//	unsigned int tLeft,tTop;
//	unsigned int tx,ty,tL;
//	float tk;
//	char tdig[5]={' ',' ',' ','%','\0'};
//	tLeft=509+68;tTop=388;
//	TFT_Fill(tLeft-35+50,tTop,tLeft-11,tTop+16,0xFFFF);
//	ty=tvalue;
//	if(ty<600)//650
//	{
//	 ty=600;//650
//	}
//	if(ty>2600)//3800
//		ty=2600; //3800
//	//tk=ty*0.038462-23.07692;//tk=ty*0.031746-20.63492;
//	tk=ty*0.05-30.0;
//	tx=tk;tL=tk;
//	if(tx>100)tx=100;
//	//if(tx<0)tx=0;

//	tdig[0]=tx/100;
//	ty=tx%100;
//	tdig[1]=ty/10;
//	ty=ty%10;
//	tdig[2]=0x30+ty;
//	if(tdig[0]==0)		 
//	{	
//		tdig[0]=' ';
//		if(tdig[1]==0)
//			{tdig[1]=' ';}
//		else
//			{tdig[1]+=0x30;}
//	}
//	else
//	{tdig[0]+=0x30;tdig[1]+=0x30;}
//	lcd_text16(tLeft-35,tTop,BLUE,WHITE,tdig);
//	Draw_Rect(tLeft+1,tTop,100-1,15,2);
////	Draw_Line(tLeft+  0,tTop,tLeft+  0,tTop-9,2,BLACK);
////	Draw_Line(tLeft+ 18,tTop,tLeft+ 18,tTop-4,2,BLACK);
////	Draw_Line(tLeft+ 37,tTop,tLeft+ 37,tTop-6,2,BLACK);
////	Draw_Line(tLeft+ 56,tTop,tLeft+ 56,tTop-4,2,BLACK);
////	Draw_Line(tLeft+ 74,tTop,tLeft+ 74,tTop-9,2,BLACK);
////	Draw_Line(tLeft+ 92,tTop,tLeft+ 92,tTop-4,2,BLACK);
////	Draw_Line(tLeft+111,tTop,tLeft+111,tTop-6,2,BLACK);
////	Draw_Line(tLeft+130,tTop,tLeft+130,tTop-4,2,BLACK);
////	Draw_Line(tLeft+148,tTop,tLeft+148,tTop-9,2,BLACK);
//	if(tL>0)
//	{
//		tx=tLeft+(tL*100+4)/100;
//		TFT_Fill(tLeft+2,tTop+1,tx,tTop+14,BLUE);
//		if(tL!=100)
//			TFT_Fill(tx+2,tTop+1,tLeft+99,tTop+14,WHITE);
//	}
//	else
//	{	TFT_Fill(tLeft+2,tTop+1,tLeft+99,tTop+14,WHITE);}
//}

void Draw_AoBar_(unsigned int tvalue)
{
	unsigned int tLeft,tTop;
	unsigned int tx,ty,tL;
	float tk;
	char tdig[5]={' ',' ',' ','%','\0'};
	tLeft=509;tTop=388;
	TFT_Fill(tLeft-35,tTop,tLeft-11,tTop+16,0xFFFF);
	ty=tvalue;
	if(ty<600)//650
	{
	 ty=600;//650
	}
	if(ty>2600)//3800
		ty=2600; //3800
	//tk=ty*0.038462-23.07692;//tk=ty*0.031746-20.63492;
	tk=ty*0.05-30.0;
	tx=tk;tL=tk;
	if(tx>100)tx=100;
	//if(tx<0)tx=0;

	tdig[0]=tx/100;
	ty=tx%100;
	tdig[1]=ty/10;
	ty=ty%10;
	tdig[2]=0x30+ty;
	if(tdig[0]==0)		 
	{	
		tdig[0]=' ';
		if(tdig[1]==0)
			{tdig[1]=' ';}
		else
			{tdig[1]+=0x30;}
	}
	else
	{tdig[0]+=0x30;tdig[1]+=0x30;}
	lcd_text16(tLeft-35,tTop,0,0,tdig);
	Draw_Rect(tLeft+1,tTop,166-1,15,2);
	Draw_Line(tLeft+  0,tTop,tLeft+  0,tTop-9,2,BLACK);
	Draw_Line(tLeft+ 21,tTop,tLeft+ 21,tTop-4,2,BLACK);
	Draw_Line(tLeft+ 42,tTop,tLeft+ 42,tTop-6,2,BLACK);
	Draw_Line(tLeft+ 62,tTop,tLeft+ 62,tTop-4,2,BLACK);
	Draw_Line(tLeft+ 83,tTop,tLeft+ 83,tTop-9,2,BLACK);
	Draw_Line(tLeft+104,tTop,tLeft+104,tTop-4,2,BLACK);
	Draw_Line(tLeft+125,tTop,tLeft+125,tTop-6,2,BLACK);
	Draw_Line(tLeft+145,tTop,tLeft+145,tTop-4,2,BLACK);
	Draw_Line(tLeft+166,tTop,tLeft+166,tTop-9,2,BLACK);
	if(tL>0)
	{
		tx=tLeft+(tL*166+4)/100;
		TFT_Fill(tLeft+2,tTop+1,tx,tTop+14,BLUE);
		if(tL!=100)
			TFT_Fill(tx+2,tTop+1,tLeft+165,tTop+14,WHITE);
	}
	else
	{	TFT_Fill(tLeft+2,tTop+1,tLeft+165,tTop+14,WHITE);}
}
void Draw_Beep(unsigned int x0,unsigned int y0,unsigned int r,unsigned char tBeep,unsigned char tWarn)
{
	int a,b;
	int di;
	int i;
	unsigned int tBeepColor=0x0000;
	unsigned int tWarnColor=0x0000;
	a=0;	b=r;	di=3-2*r;             //脜脨露脧脧脗赂枚碌茫脦禄脰脙碌脛卤锚脰戮
  /*draw full circle*/
	while(a<=b)
	{
	/*full circle*/
		DrawBigPoint(x0-b,y0-a,BLACK);             //3           
		DrawBigPoint(x0+b,y0-a,BLACK);             //0           
		DrawBigPoint(x0-a,y0+b,BLACK);             //1       
		DrawBigPoint(x0-b,y0-a,BLACK);             //7           
		DrawBigPoint(x0-a,y0-b,BLACK);             //2             
		DrawBigPoint(x0+b,y0+a,BLACK);             //4               
		DrawBigPoint(x0+a,y0-b,BLACK);             //5
		DrawBigPoint(x0+a,y0+b,BLACK);             //6 
		DrawBigPoint(x0-b,y0+a,BLACK);             
		a++;
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		DrawBigPoint(x0+a,y0+b,BLACK);
	}
	/*脠莽鹿没脫脨卤篓戮炉碌脝脡脕拢卢禄颅脢碌脨脛脭虏*/
	if (tWarn)
		tWarnColor=0xFFE0;//禄脝脡芦
	else
		tWarnColor=0xFFFF;//掳脳脡芦

	for(i=r-6;i>0;i-=4)
	{
		a=0;b=i;di=3-2*i;             //脜脨露脧脧脗赂枚碌茫脦禄脰脙碌脛卤锚脰戮
	  /*draw full circle*/
		while(a<=b)
		{
			DrawBigPoint(x0-b,y0-a,tWarnColor);             //3           
			DrawBigPoint(x0+b,y0-a,tWarnColor);             //0           
			DrawBigPoint(x0-a,y0+b,tWarnColor);             //1       
			DrawBigPoint(x0-b,y0-a,tWarnColor);             //7           
			DrawBigPoint(x0-a,y0-b,tWarnColor);             //2             
			DrawBigPoint(x0+b,y0+a,tWarnColor);             //4               
			DrawBigPoint(x0+a,y0-b,tWarnColor);             //5
			DrawBigPoint(x0+a,y0+b,tWarnColor);             //6 
			DrawBigPoint(x0-b,y0+a,tWarnColor);             
			a++;
			if(di<0)di +=4*a+6;	  
			else
			{
				di+=10+4*(a-b);   
				b--;
			} 
			DrawBigPoint(x0+a,y0+b,tWarnColor);
		}
	}	
	
	if(tBeep)
		tBeepColor=RED;
	else
		tBeepColor=BLACK;
	for(i=r-5;i>5;i-=6)
	{
		a=0;b=i;di=3-2*i;             //脜脨露脧脧脗赂枚碌茫脦禄脰脙碌脛卤锚脰戮
	  /*draw full circle*/
		while(a<=b)
		{
			//DrawBigPoint(x0-b,y0-a,RED);             //3           
			DrawBigPoint(x0+b,y0-a,tBeepColor);             //0           
			DrawBigPoint(x0-a,y0+b,tBeepColor);             //1       
			//DrawBigPoint(x0-b,y0-a,tColor);             //7           
			//DrawBigPoint(x0-a,y0-b,tColor);             //2             
			//DrawBigPoint(x0+b,y0+a,tColor);             //4               
			DrawBigPoint(x0+a,y0-b,tBeepColor);             //5
			//DrawBigPoint(x0+a,y0+b,tColor);             //6 
			DrawBigPoint(x0-b,y0+a,tBeepColor);             
			a++;
			if(di<0)di +=4*a+6;	  
			else
			{
				di+=10+4*(a-b);   
				b--;
			} 
			//DrawBigPoint(x0+a,y0+b,tColor);
		}
	}
} 
void DrawBigPoint(unsigned int x,unsigned int y,unsigned int tColor)
{
		TFT_DrawPoint(x,y,tColor);//脰脨脨脛碌茫 
		TFT_DrawPoint(x+1,y,tColor);
		TFT_DrawPoint(x,y+1,tColor);
		TFT_DrawPoint(x+1,y+1,tColor);	
}
void Draw_Readed(unsigned int x,unsigned int y)
{
 	Draw_Line(x+40,y+12,x+28,y+18,3,BLACK);
 	Draw_Line(x+40,y+12,x+69,y+27,3,BLACK);
 	Draw_Line(x+20,y+22,x+11,y+27,3,BLACK);
 	Draw_Line(x+11,y+53,x+11,y+27,3,BLACK);
 	Draw_Line(x+11,y+53,x+69,y+53,3,BLACK);
 	Draw_Line(x+69,y+27,x+69,y+56,3,BLACK);
 	Draw_Line(x+69,y+27,x+11,y+53,3,BLACK);
 	Draw_Line(x+11,y+27,x+69,y+53,3,BLACK);
 	Draw_Line(x+18,y+30,x+21,y+17,4,BLACK);
 	Draw_Line(x+63,y+26,x+21,y+17,3,BLACK);
 	Draw_Line(x+63,y+26,x+21,y+17,3,BLACK);
 	Draw_Line(x+63,y+26,x+62,y+30,4,BLACK);
}
void Draw_unRead(unsigned int x,unsigned int y)
{
	Draw_Rect(x+11,y+13,58,34,3);
	Draw_Line(x+11,y+22,x+40,y+39,3,BLACK);
	Draw_Line(x+69,y+22,x+40,y+39,3,BLACK);
	Draw_Line(x+33,y+35,x+11,y+47,3,BLACK);
	Draw_Line(x+47,y+35,x+69,y+47,3,BLACK);
}
void re_SFL_float(double tfSFL)
{
	unsigned int  tx,ty;
	unsigned long intSFL=0;
	
	unsigned long temp;
	unsigned char tFlgSig;//前导零删除
	unsigned char i;
	if((frmNum==FRM_MAIN)||(frmNum==FRM_SD))
	{
		if(tfSFL>=0.0)
		{
		tx=40+144;
		ty=130-15;
		if(tfSFL<200000000)
		{
			 intSFL=tfSFL*10;
		}
		for(i=0;i<11;i++)
			bufSFL[i]=disSFL[i];
		temp=intSFL;
		disSFL[0]=temp/100000000+0x30;
		temp=temp%100000000;
		disSFL[1]=temp/10000000+0x30;
		temp=temp%10000000;
		disSFL[2]=temp/1000000+0x30;
		temp=temp%1000000;
		disSFL[3]=temp/100000+0x30;
		temp=temp%100000;
		disSFL[4]=temp/10000+0x30;
		temp=temp%10000;
		disSFL[5]=temp/1000+0x30;
		temp=temp%1000;
		disSFL[6]=temp/100+0x30;
		temp=temp%100;
		disSFL[7]=temp/10+0x30;
		temp=temp%10;
		//tSFL[8]='.';
		disSFL[8]=temp+0x30;
		disSFL[9]='\0';
		//tflgH=0;
		
		tFlgSig=0;
		for(i=0;i<7;i++)
		{
			if(tFlgSig==0)
			{
			 	if(disSFL[i]=='0')
				{
				  disSFL[i]=' ';
				}
				else
					tFlgSig=1;
			}
		}
		if(disSFL[7]==' ')disSFL[7]='0';
		if(disSFL[8]==' ')disSFL[8]='0';
		for(i=2;i<9;i++)
		{
			if((bufSFL[i]!=disSFL[i])||(flgSFLdis==1))//	
			{
				TFT_Fill(tx+5,ty+3,tx+67,ty+135,0xFFFF);
				Draw_DigNum(tx,ty);
				DisDigNum(tx,ty,disSFL[i]);
			}
			tx+=72;
		}
		flgSFLdis=0;
		TFT_Fill(7*72+37+71,125+133-15,7*72+37+81,125+137-15,0xF800);
		TFT_Fill(7*72+37+74,125+130-15,7*72+37+78,125+140-15,0xF800);
		TFT_Fill(7*72+37+73,125+132-15,7*72+37+79,125+138-15,0xF800);
		}
	}	
}
unsigned int TFT_GetPoint(unsigned short x,unsigned short y)
{
	return raReadPixel(x,y);	
}

void TFT_DrawPoint(unsigned int x,unsigned int y,unsigned int fontcolor)
{
	raDrawPixel(x,y,fontcolor);
}
void ram_icon1(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<14;j++) //h=0~14
	{
		for(k=0;k<9;k++)
		{
			tx=x+13+k;
			ty=y+5+j;
			tPt=RES_Top_Left_BIN[j*9+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+22,y+5,x+50,y+19,0xF800);
	for(j=0;j<14;j++) //h=0~8
	{
		for(k=0;k<9;k++)
		{
		tx=x+50+k;
		ty=y+5+j;
		tPt=RES_Top_Left_BIN[j*9+8-k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_icon2(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<9;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
			tx=x+6+k;
			ty=y+12+j;
			tPt=RES_left_up_BIN[j*14+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+6,y+21,x+19,y+62,0xF800);
	for(j=0;j<7;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
		tx=x+6+k;
		ty=y+62+j;
		tPt=RES_dn_BIN[j*14+k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_icon3(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<9;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
			tx=x+52+k;
			ty=y+12+j;
			tPt=RES_left_up_BIN[j*14+13-k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+52,y+21,x+65,y+62,0xF800);
	for(j=0;j<7;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
		tx=x+52+k;
		ty=y+62+j;
		tPt=RES_dn_BIN[j*14+k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_icon4(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<14;j++) //h=0~14
	{
		for(k=0;k<7;k++)
		{
			tx=x+16+k;
			ty=y+63+j;
			tPt=RES_left_BIN[j*7+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+23,y+63,x+49,y+76,0xF800);
	for(j=0;j<14;j++) //h=0~8
	{
		for(k=0;k<7;k++)
		{
		tx=x+49+k;
		ty=y+63+j;
		tPt=RES_left_BIN[j*7+6-k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_icon5(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	for(j=0;j<7;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
		tx=x+6+k;
		ty=y+71+j;
		tPt=RES_dn_BIN[(6-j)*14+k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+6,y+78,x+19,y+119,0xF800);
	//get point;
	for(j=0;j<9;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
			tx=x+6+k;
			ty=y+119+j;
			tPt=RES_left_up_BIN[(8-j)*14+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}
void ram_icon6(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<7;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
		tx=x+52+k;
		ty=y+71+j;
		tPt=RES_dn_BIN[(6-j)*14+k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+52,y+78,x+65,y+119,0xF800);
	for(j=0;j<9;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
			tx=x+52+k;
			ty=y+119+j;
			tPt=RES_left_up_BIN[(8-j)*14+13-k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_icon7(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<14;j++) //h=0~14
	{
		for(k=0;k<9;k++)
		{
			tx=x+13+k;
			ty=y+121+j;
			tPt=RES_Top_Left_BIN[(13-j)*9+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+22,y+121,x+49,y+134,0xF800);
	for(j=0;j<14;j++) //h=0~8
	{
		for(k=0;k<9;k++)
		{
		tx=x+49+k;
		ty=y+121+j;
		tPt=RES_Top_Left_BIN[(13-j)*9+8-k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

/*
ram_icon:id
 -1-
|   | 
2   3
|   |
 -4-
|   |
5   6
|   |
 -7-
*/
void ram_iconS1(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<14;j++) //h=0~14
	{
		for(k=0;k<9;k++)
		{
			tx=x+13+k;
			ty=y+10+j;
			tPt=RES_Top_Left_BIN[j*9+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+22,y+10,x+44,y+24,0xF800);
	for(j=0;j<14;j++) //h=0~8
	{
		for(k=0;k<9;k++)
		{
		tx=x+44+k;
		ty=y+10+j;
		tPt=RES_Top_Left_BIN[j*9+8-k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_iconS2(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<9;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
			tx=x+6+k;
			ty=y+17+j;
			tPt=RES_left_up_BIN[j*14+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+6,y+26,x+19,y+62,0xF800);//原先61改成62，白线没有了，OK!!!
	for(j=0;j<7;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
		tx=x+6+k;
		ty=y+62+j;
		tPt=RES_dn_BIN[j*14+k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_iconS3(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<9;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
			tx=x+46+k;
			ty=y+17+j;
			tPt=RES_left_up_BIN[j*14+13-k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+46,y+26,x+59,y+62,0xF800);//原先61改成62，白线没有了，OK!!!
	for(j=0;j<7;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
		tx=x+46+k;
		ty=y+62+j;
		tPt=RES_dn_BIN[j*14+k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_iconS4(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<14;j++) //h=0~14
	{
		for(k=0;k<7;k++)
		{
			tx=x+16+k;
			ty=y+63+j;
			tPt=RES_left_BIN[j*7+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+23,y+63,x+43,y+76,0xF800);
	for(j=0;j<14;j++) //h=0~8
	{
		for(k=0;k<7;k++)
		{
		tx=x+43+k;
		ty=y+63+j;
		tPt=RES_left_BIN[j*7+6-k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_iconS5(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	for(j=0;j<7;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
		tx=x+6+k;
		ty=y+71+j;
		tPt=RES_dn_BIN[(6-j)*14+k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+6,y+78,x+19,y+114,0xF800);
	//get point;
	for(j=0;j<9;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
			tx=x+6+k;
			ty=y+114+j;
			tPt=RES_left_up_BIN[(8-j)*14+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}
void ram_iconS6(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<7;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
		tx=x+46+k;
		ty=y+71+j;
		tPt=RES_dn_BIN[(6-j)*14+k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+46,y+78,x+59,y+114,0xF800);
	for(j=0;j<9;j++) //h=0~8
	{
		for(k=0;k<14;k++)
		{
			tx=x+46+k;
			ty=y+114+j;
			tPt=RES_left_up_BIN[(8-j)*14+13-k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void ram_iconS7(unsigned short x,unsigned short y)
{ 
  unsigned short j,k;//,fwh[3];
	unsigned int tmp_buf[100];
	unsigned int tPt;
	unsigned int tx,ty;
	//get point;
	for(j=0;j<14;j++) //h=0~14
	{
		for(k=0;k<9;k++)
		{
			tx=x+13+k;
			ty=y+116+j;
			tPt=RES_Top_Left_BIN[(13-j)*9+k];
			if(tPt!=0xFFFF)
				tmp_buf[k]=tPt;
			else
				tmp_buf[k]=TFT_GetPoint(tx,ty);
			TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
	TFT_Fill(x+22,y+116,x+43,y+129,0xF800);
	for(j=0;j<14;j++) //h=0~8
	{
		for(k=0;k<9;k++)
		{
		tx=x+43+k;
		ty=y+116+j;
		tPt=RES_Top_Left_BIN[(13-j)*9+8-k];
		if(tPt!=0xFFFF)
			tmp_buf[k]=tPt;
		else
			tmp_buf[k]=TFT_GetPoint(tx,ty);
		TFT_DrawPoint(tx,ty,tmp_buf[k]);
		}
	}	
}

void DisDigNumS(unsigned int x,unsigned int y,unsigned char tNum)
{
	if(1)//x<680
	{
	switch(tNum)
	{
		case 0:
		case 0x30:
			ram_iconS1(x,y);
			ram_iconS2(x,y);
			ram_iconS3(x,y);
			//ram_iconS4(x,y);
			ram_iconS5(x,y);
			ram_iconS6(x,y);
			ram_iconS7(x,y);
		break;
		case 1:
		case 0x31:
			//ram_iconS1(x,y);
			//ram_iconS2(x,y);
			ram_iconS3(x,y);
			//ram_iconS4(x,y);
			//ram_iconS5(x,y);
			ram_iconS6(x,y);
			//ram_iconS7(x,y);
		break;
		case 2:
		case 0x32:
			ram_iconS1(x,y);
			//ram_iconS2(x,y);
			ram_iconS3(x,y);
			ram_iconS4(x,y);
			ram_iconS5(x,y);
			//ram_iconS6(x,y);
			ram_iconS7(x,y);
		break;
		case 3:
		case 0x33:
			ram_iconS1(x,y);
			//ram_iconS2(x,y);
			ram_iconS3(x,y);
			ram_iconS4(x,y);
			//ram_iconS5(x,y);
			ram_iconS6(x,y);
			ram_iconS7(x,y);
		break;
		case 4:
		case 0x34:
			//ram_iconS1(x,y);
			ram_iconS2(x,y);
			ram_iconS3(x,y);
			ram_iconS4(x,y);
			//ram_iconS5(x,y);
			ram_iconS6(x,y);
			//ram_iconS7(x,y);
		break;
		case 5:
		case 0x35:
			ram_iconS1(x,y);
			ram_iconS2(x,y);
			//ram_iconS3(x,y);
			ram_iconS4(x,y);
			//ram_iconS5(x,y);
			ram_iconS6(x,y);
			ram_iconS7(x,y);
		break;
		case 6:
		case 0x36:
			ram_iconS1(x,y);
			ram_iconS2(x,y);
			//ram_iconS3(x,y);
			ram_iconS4(x,y);
			ram_iconS5(x,y);
			ram_iconS6(x,y);
			ram_iconS7(x,y);
		break;
		case 7:
		case 0x37:
			ram_iconS1(x,y);
			//ram_iconS2(x,y);
			ram_iconS3(x,y);
			//ram_iconS4(x,y);
			//ram_icon5(x,y);
			ram_iconS6(x,y);
			//ram_icon7(x,y);
		break;
		case 8:
		case 0x38:
			ram_iconS1(x,y);
			ram_iconS2(x,y);
			ram_iconS3(x,y);
			ram_iconS4(x,y);
			ram_iconS5(x,y);
			ram_iconS6(x,y);
			ram_iconS7(x,y);
		break;
		case 9:
		case 0x39:
			ram_iconS1(x,y);
			ram_iconS2(x,y);
			ram_iconS3(x,y);
			ram_iconS4(x,y);
			//ram_icon5(x,y);
			ram_iconS6(x,y);
			ram_iconS7(x,y);
		break;
		default:

		break;
	}
	}
}

void re_monSFL(unsigned char id, double tfSFL)
{
	unsigned int  tx,ty;
	unsigned long intSFL=0;
	
	unsigned long temp;
	unsigned char tFlgSig;//前导零删除
	unsigned char i;
	unsigned char tdat;
	if(tfSFL>=0.0)
	{
		if(id==0)
			tx=4;
		else if(id==1)
		  tx=404;
	ty=100;
	if(tfSFL<200000000)
	{
		 intSFL=tfSFL*10;
	}
	for(i=0;i<6;i++)
		bufSFLs[id][i]=disSFLs[id][i];
	temp=intSFL;
//	disSFL[0]=temp/100000000+0x30;
//	temp=temp%100000000;
//	disSFL[1]=temp/10000000+0x30;
//	temp=temp%10000000;
//	disSFL[2]=temp/1000000+0x30;
//	temp=temp%1000000;
	disSFLs[id][0]=temp/100000+0x30;
	temp=temp%100000;
	disSFLs[id][1]=temp/10000+0x30;
	temp=temp%10000;
	disSFLs[id][2]=temp/1000+0x30;
	temp=temp%1000;
	disSFLs[id][3]=temp/100+0x30;
	temp=temp%100;
	disSFLs[id][4]=temp/10+0x30;
	temp=temp%10;
	//tSFL[8]='.';
	disSFLs[id][5]=temp+0x30;
	disSFLs[id][6]='\0';
	//tflgH=0;
	
	tFlgSig=0;
	for(i=0;i<4;i++)
	{
		if(tFlgSig==0)
		{
		 	if(disSFLs[id][i]=='0')
			{
			  disSFLs[id][i]=' ';
			}
			else
				tFlgSig=1;
		}
	}
	if(disSFLs[id][4]==' ')disSFLs[id][4]='0';
	if(disSFLs[id][5]==' ')disSFLs[id][5]='0';
	for(i=0;i<6;i++)
	{
		if((bufSFLs[id][i]!=disSFLs[id][i])||(flgSFLs[id]==1))//	
		{
			bufSFLs[id][i]=disSFLs[id][i];
			TFT_Fill(tx+5,ty+9,tx+61,ty+131,0xFFFF);
			Draw_DigNumS(tx,ty);
			tdat=disSFLs[id][i];
			DisDigNumS(tx,ty,tdat);
		}
		tx+=64;
	}
	flgSFLs[id]=0;
	if(id==0)
	{
		TFT_Fill(322,221,330,227,0xF800);
		TFT_Fill(323,220,329,228,0xF800);
	}
	else if(id==1)
	{
		TFT_Fill(722,221,730,227,0xF800);
		TFT_Fill(723,220,729,228,0xF800);
	}

}	
}

void DisDigNum(unsigned int x,unsigned int y,unsigned char tNum)
{
	if(x<680)
	{
	switch(tNum)
	{
		case 0:
		case 0x30:
			ram_icon1(x,y);
			ram_icon2(x,y);
			ram_icon3(x,y);
			//ram_icon4(x,y);
			ram_icon5(x,y);
			ram_icon6(x,y);
			ram_icon7(x,y);
		break;
		case 1:
		case 0x31:
			//ram_icon1(x,y);
			//ram_icon2(x,y);
			ram_icon3(x,y);
			//ram_icon4(x,y);
			//ram_icon5(x,y);
			ram_icon6(x,y);
			//ram_icon7(x,y);
		break;
		case 2:
		case 0x32:
			ram_icon1(x,y);
			//ram_icon2(x,y);
			ram_icon3(x,y);
			ram_icon4(x,y);
			ram_icon5(x,y);
			//ram_icon6(x,y);
			ram_icon7(x,y);
		break;
		case 3:
		case 0x33:
			ram_icon1(x,y);
			//ram_icon2(x,y);
			ram_icon3(x,y);
			ram_icon4(x,y);
			//ram_icon5(x,y);
			ram_icon6(x,y);
			ram_icon7(x,y);
		break;
		case 4:
		case 0x34:
			//ram_icon1(x,y);
			ram_icon2(x,y);
			ram_icon3(x,y);
			ram_icon4(x,y);
			//ram_icon5(x,y);
			ram_icon6(x,y);
			//ram_icon7(x,y);
		break;
		case 5:
		case 0x35:
			ram_icon1(x,y);
			ram_icon2(x,y);
			//ram_icon3(x,y);
			ram_icon4(x,y);
			//ram_icon5(x,y);
			ram_icon6(x,y);
			ram_icon7(x,y);
		break;
		case 6:
		case 0x36:
			ram_icon1(x,y);
			ram_icon2(x,y);
			//ram_icon3(x,y);
			ram_icon4(x,y);
			ram_icon5(x,y);
			ram_icon6(x,y);
			ram_icon7(x,y);
		break;
		case 7:
		case 0x37:
			ram_icon1(x,y);
			//ram_icon2(x,y);
			ram_icon3(x,y);
			//ram_icon4(x,y);
			//ram_icon5(x,y);
			ram_icon6(x,y);
			//ram_icon7(x,y);
		break;
		case 8:
		case 0x38:
			ram_icon1(x,y);
			ram_icon2(x,y);
			ram_icon3(x,y);
			ram_icon4(x,y);
			ram_icon5(x,y);
			ram_icon6(x,y);
			ram_icon7(x,y);
		break;
		case 9:
		case 0x39:
			ram_icon1(x,y);
			ram_icon2(x,y);
			ram_icon3(x,y);
			ram_icon4(x,y);
			//ram_icon5(x,y);
			ram_icon6(x,y);
			ram_icon7(x,y);
		break;
		default:

		break;
	}
	}
}
void Sel_Button(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight)
{
unsigned char i;
unsigned int x1,x2,y1,y2;
x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
//0xFFFF,0xEF5D,0xDEFC,0xA535,0x8431,0x0000
	TFT_DrawLine(x1,y1,x1,y2,0xFFFF);	//left3 
	TFT_DrawLine(x1,y1,x2,y1,0xFFFF);	//top3 
	TFT_DrawLine(x1-1,y1-1,x1-1,y2+1,0xEF5D);	//left2 
	TFT_DrawLine(x1-1,y1-1,x2+1,y1-1,0xEF5D);	//top2 
	TFT_DrawLine(x1-2,y1-2,x1-2,y2+2,0xDEFC);	//left1 
	TFT_DrawLine(x1-2,y1-2,x2+2,y1-2,0xDEFC);	//top1 

 	TFT_DrawLine(x2,y1,x2,y2,0xA535);//right1	
	TFT_DrawLine(x2,y2,x1,y2,0xA535);//buttom1	
 	TFT_DrawLine(x2+1,y1-1,x2+1,y2+1,0x8431);//right2	
	TFT_DrawLine(x2+1,y2+1,x1-1,y2+1,0x8431);//buttom2	
 	TFT_DrawLine(x2+2,y1-2,x2+2,y2+2,0x0000);//right3	
	TFT_DrawLine(x2+2,y2+2,x1-2,y2+2,0x0000);//buttom3	

	for(i=0;i<3;i++)
	{
		TFT_DrawLine(x1-3-i,y1-3-i,x1-3-i,y2+3+i,0xF800);	 
		TFT_DrawLine(x1-3-i,y1-3-i,x2+3+i,y1-3-i,0xF800);
	 	TFT_DrawLine(x2+3+i,y1-3-i,x2+3+i,y2+3+i,0xF800);
		TFT_DrawLine(x2+3+i,y2+3+i,x1-3-i,y2+3+i,0xF800);
	}
}

void Sel_Btn(char *p)
{
unsigned int x1,y1,tw,th;
	x1=700;y1=90;tw=80;th=60;
	if(strcmp(p,"dfjl")==0)y1=60;
	if(strcmp(p,"yfjl")==0)y1=130;
	if(strcmp(p,"bjjl")==0)y1=200;
	if(strcmp(p,"AMS")==0)y1=270;
	if(strcmp(p,"czfs")==0)y1=340;
	Sel_Button(x1,y1,tw,th);
}

void Draw_Btn(char *p)
{

unsigned int x1,y1,tw,th,btnID=0;
x1=700;y1=60;tw=80;th=60;btnID=0;
	if(strcmp(p,"dfjl")==0)btnID=1;//待发记录
	if(strcmp(p,"yfjl")==0)btnID=2;//已发记录
	if(strcmp(p,"bjjl")==0)btnID=3;//报警记录
	if(strcmp(p,"AMS")==0)btnID=4;//报警记录
	if(strcmp(p,"czfs")==0)btnID=5;//
	switch(btnID)
	{
	 case 1:
	 	 y1=60;
	 break;
	 case 2:
	 	 y1=130;
	 break;
	 case 3:
	 	 y1=200;
	 break;
	 case 4:
	 	 y1=270;
	 break;
	 case 5:
	 	 y1=340;
	 break;
	}
	 Draw_Button(x1,y1,tw,th);
}
void SelPendSel(unsigned char x,unsigned char y,unsigned int color)
{
 unsigned int x1,y1,x2,y2;
 switch(y)
 {
	case 0:
		x1=393-3;y1=176-1;x2=x1+2+192;y2=y1+48;  
	break;
	case 1:
		x1=393-3;y1=304-1;x2=x1+2+192;y2=y1+48;  
	break;
	case 2:
	  if(x==0)			x1=244-4;
	  if(x==1)			x1=440-4;
		x2=x1+7+116;y1=373-4;y2=y1+7+47;  
	break;
 }
 Draw_Line(x1,y1,x2,y1,2,color);Draw_Line(x1,y1,x1,y2,2,color);
 Draw_Line(x1,y2,x2,y2,2,color);Draw_Line(x2,y1,x2,y2+2,2,color);
}

//AB发油参数
void Dis_Fy(void)
{
unsigned char ti;
unsigned char tj;
unsigned int  tDat1;
char dSavDat[34];
	unsigned long lngDat=0,lngDat1;
	EepRd(170,10);
	for(ti=0;ti<10;ti++)
	{
	 dSavDat[ti]=EepRdBuf[ti];
	}
	lngDat =dSavDat[0]*1000000;
	lngDat+=dSavDat[1]*100000;
	lngDat+=dSavDat[2]*10000;
	lngDat+=dSavDat[3]*1000;
	lngDat+=dSavDat[4]*100;
	lngDat+=dSavDat[5]*10;
	lngDat+=dSavDat[6];
	lngDat1=dSavDat[7];
	lngDat1=lngDat1<<8;
	lngDat1+=dSavDat[8];
	lngDat1=lngDat1<<8;
	lngDat1+=dSavDat[9];
	if(lngDat==lngDat1)//最大容积核对正确
	{
		SavMaxVol=lngDat;
		for(ti=0;ti<7;ti++)
		{
		  tmpSavMaxVol[ti]='0'+dSavDat[ti];
		}	  
		for(ti=0;ti<6;ti++)
		{
		 if(tmpSavMaxVol[ti]=='0')
		   tmpSavMaxVol[ti]=' ';
		 else
		   break;
		}
		tmpSavMaxVol[7]=0;
	 lcd_text16(454,62+8*35,0,WHITE,tmpSavMaxVol);
	}

EepRd(130,36);
for(ti=0;ti<34;ti++)//如果是AO阀则改成ti<32
{
	dSavDat[ti]=EepRdBuf[ti];
}

	//AB阀17个参数，比AO阀多一个参数
	for(ti=0;ti<17;ti++)//如果是AO阀则改成ti<16
	{
		tDat1=(dSavDat[2*ti]<<8)+dSavDat[2*ti+1];
		SavFyDat[ti]=tDat1;
		if(SavFyDat[16]<80)
		  SavFyDat[16]=80;
		u16SavFyDat=SavFyDat[ti];
		FyDat2Chr();
		for(tj=0;tj<4;tj++)
		{	
			   SavFy[ti][tj]=u8SavFyBuf[tj];
			//tmpSavFy[ti][tj]=SavFy[ti][tj];
		}
		if(frmNum==FRM_SETTING)
		{
			if(ti<9)
				WrSetSel(0,ti,u8SavFyBuf);
			else
				WrSetSel(1,ti-9,u8SavFyBuf);
		}
	}
	for(ti=0;ti<17;ti++)//如果是AO阀则改成ti<16
	{
		for(tj=0;tj<4;tj++)
		{	
			tmpSavFy[ti][tj]=SavFy[ti][tj];
		}
	}
}

