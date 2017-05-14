#include "LCD.h"
#include "RA8875.h"
#include "logo1.h"

#include "Global.h"

RTC_TimeTypeDef RTC_TimeStruct;//定义时间成员结构体

void reDrawCell(void);
void Dis_Lcd(void);


extern unsigned char EepWrBuf[200];
extern unsigned char EepRdBuf[200];
extern void EepRd(u16 ReadAddr,u16 NumByteToWrite);

void Draw_Sel(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned int tColor);
void Draw_Frame(u8 x,u8 y,u16 color);
void Draw_Pkq_STA(unsigned char id,unsigned char sta);
void LcdDrawTriangle(uint16_t P1_X,uint16_t P1_Y,uint16_t P2_X,uint16_t P2_Y,uint16_t P3_X,uint16_t P3_Y,  uint16_t tColor, bool tIsfill);
void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
void LcdDrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t tColor, bool tIsfill);
void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
void Draw_EN24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);

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
		Draw_EN24(680,4,BLUE,WHITE,tTimeBuf);
		sysHH_pre=sysHH;
	}
	if(sysMM!=sysMM_pre)
	{tTimeBuf[0]='0'+(sysMM/10);//565
		tTimeBuf[1]='0'+(sysMM%10);//565+
		tTimeBuf[2]=':';
		tTimeBuf[3]=0;
		//ShowStr30(565+55+40,0,tTimeBuf,3,BLACK,GOLD);
		//lcd_text24(680+36,4,BLUE,GOLD,tTimeBuf);
		Draw_EN24(680+36,4,BLUE,WHITE,tTimeBuf);
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
		Draw_EN24(680+72,4,BLUE,WHITE,tTimeBuf);
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
		//if((sysD_pre!=1)&&(sysD==1))//??1??????
		//{
		//flgClrLJS=1;
		//}
		sysY_pre=sysY;sysM_pre=sysM;sysD_pre=sysD;
		//ShowStr30(365+64,0,tDateBuf,10,BLACK,GOLD);
		//lcd_text24(550,4,BLUE,GOLD,tDateBuf);
		Draw_EN24(550,4,BLUE,WHITE,tDateBuf);

	}
}

void Draw_Logo(unsigned int x,unsigned int y)
{
	unsigned int i,j,tDat1,tDat2,tDat3;
	unsigned int tDat;
  for(i=0;i<88;i++)
	{
		for(j=0;j<31;j++)
		{
			tDat3=AWZ_Logo[(j*88+i)*3+2];
			tDat2=AWZ_Logo[(j*88+i)*3+1];
			tDat1=AWZ_Logo[(j*88+i)*3];
			tDat =(tDat3>>3)<<11;
			tDat+=(tDat2>>2)<<5;
			tDat+=(tDat1>>3);
		  lcdDrawPixel(x+i,y+30-j,tDat);
		}
	}
}
void Draw_Logo_(unsigned int x,unsigned int y)
{
	unsigned int i,j,tDat1,tDat2,tDat3;
	unsigned int tDat;
  for(i=0;i<660;i++)
	{
		for(j=0;j<74;j++)
		{
			tDat3=AWZ_Logo[(j*660+i)*3+2];tDat2=AWZ_Logo[(j*660+i)*3+1];tDat1=AWZ_Logo[(j*660+i)*3];
			tDat =(tDat3>>3)<<11;
			tDat+=(tDat2>>2)<<5;
			tDat+=(tDat1>>3);
		  lcdDrawPixel(x+i,y+73-j,tDat);
		}
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
void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	raMemoryBusy();	
	raTextMode();
	raFontControlCGROMFont();
	raFontControlExternalCGROM();
	raSromClkDiv(0X03);
	raSerialROMSelect0();
	raFontArial();//
	raFontSize24x24_12x24();
//	switch(tType)
//	{
//		case GB2312_15X16_8x16_HZ:
//								raFontCodeGB2312();
//								raFontSize16x16_8x16();
//								break; 		
//		case GB2312_24x24_12x24_HZ:
//								raFontCodeGB2312();
//								raFontSize24x24_12x24();
//								break; 		
//		case GB2312_32X32_16x32_HZ:
//								raFontCodeGB2312();
//								raFontSize32x32_16x32();
//								break; 		
//	}
//	if(tTransparency)
//		raFontWithBackgroundTransparency();
//	else
//		raFontWithBackgroundColor();
	raFontWithBackgroundColor();
	raGTSROMGT23L32S4W();
	raFontWriteCoordinate(x, y);
	raBTEBackgroundColor(bc);
	raBTEForegroundColor(fc);
	raDisplayString(s);raFontCodeGB2312();
	//	LcdDisplayStringZH(s,x,y,fc,bc,0,GB2312_24x24_12x24_HZ);
}
void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	raMemoryBusy();	
	raTextMode();
	raFontControlCGROMFont();
	raFontControlExternalCGROM();
	raSromClkDiv(0X03);
	raSerialROMSelect0();
	raFontCodeGB2312();
	raFontSize16x16_8x16();
//	switch(tType)
//	{
//		case GB2312_15X16_8x16_HZ:
//								raFontCodeGB2312();
//								raFontSize16x16_8x16();
//								break; 		
//		case GB2312_24x24_12x24_HZ:
//								raFontCodeGB2312();
//								raFontSize24x24_12x24();
//								break; 		
//		case GB2312_32X32_16x32_HZ:
//								raFontCodeGB2312();
//								raFontSize32x32_16x32();
//								break; 		
//	}
//	if(tTransparency)
//		raFontWithBackgroundTransparency();
//	else
//		raFontWithBackgroundColor();
	raFontWithBackgroundColor();
	raGTSROMGT23L32S4W();
	raFontWriteCoordinate(x, y);
	raBTEBackgroundColor(bc);
	raBTEForegroundColor(fc);
	raDisplayString(s);
	//	LcdDisplayStringZH(s,x,y,fc,bc,0,GB2312_24x24_12x24_HZ);
}
//void LcdDisplayStringZH(char * zh, uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,bool tTransparency, GTFont_type tType )
//{
//	raMemoryBusy();	
//	raTextMode();
//	raFontControlCGROMFont();
//	raFontControlExternalCGROM();
//	raSromClkDiv(0X03);
//	raSerialROMSelect0();
//	switch(tType)
//	{
//		case GB2312_15X16_8x16_HZ:
//								raFontCodeGB2312();
//								raFontSize16x16_8x16();
//								break; 		
//		case GB2312_24x24_12x24_HZ:
//								raFontCodeGB2312();
//								raFontSize24x24_12x24();
//								break; 		
//		case GB2312_32X32_16x32_HZ:
//								raFontCodeGB2312();
//								raFontSize32x32_16x32();
//								break; 		

//	}
//	if(tTransparency)
//		raFontWithBackgroundTransparency();
//	else
//		raFontWithBackgroundColor();
//	raGTSROMGT23L32S4W();
//	raFontWriteCoordinate(x, y);
//	raBTEBackgroundColor(tBackColor);
//	raBTEForegroundColor(tForeColor);
//	raDisplayString(zh);
//}
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
  LcdDisplayASCII(x, y, fc, bc, 0, tDisBuf );
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
	LcdDisplayASCII( x,y,fc,bc,0,tDisBuf);
	//	Dis_Num16(400,440,0,GOLD,PadCode[0]);
//	LcdDisplayStringZH(tDisBuf,x,y,fc,bc,0,GB2312_15X16_8x16_HZ);
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
			//TFT_DrawLine(x1,y1+i,x2,y2+i,color);	//TOP LINE
		else
			LcdDrawLine(x1+i, y1,  x2+i,  y2, color);
		//TFT_DrawLine(x1+i,y1,x2+i,y2,color);	//TOP LINE
	} 	
}
void Draw_Rect(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned int color)
{
	unsigned int x1,x2,y1,y2;
	x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;

	LcdDrawLine(x1,y1,x1,y2,color);	 
	LcdDrawLine(x1,y1,x2,y1,color);	
	LcdDrawLine(x1+1,y1+1,x1+1,y2-1,color);	
	LcdDrawLine(x1+1,y1+1,x2-1,y1+1,color);	
	LcdDrawLine(x2,y1,x2,y2,color);	
	LcdDrawLine(x2,y2,x1,y2,color);	
	LcdDrawLine(x2-1,y1+1,x2-1,y2-1,color);	
	LcdDrawLine(x2-1,y2-1,x1+1,y2-1,color);	

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

//void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
//{
//	LcdDisplayStringZH(s,x,y,fc,bc,0,GB2312_24x24_12x24_HZ);
//}

//void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
//{
//	LcdDisplayStringZH(s,x,y,fc,bc,0,GB2312_15X16_8x16_HZ);
//}

void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{
  LcdDrawRectangle(xsta, ysta, xend, yend, color, 1);
}

void Draw_Lamp(unsigned int x,unsigned int y,unsigned char tColor)
{
  LcdDrawCircle(x,y,8, tColor,1);
}

extern char disWeiDat[7];
void Dis_RtDat(void)
{
	unsigned char i,flgDatOk=1;
	char disBuf[10];
	//if(frmNum==FRM_MAIN)
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
			lcd_text24(210,114,RED,WHITE,disBuf);		
	}
}



//void Dis_SysAdd(unsigned char tSysAdd)
//{
//	char tSysAddBuff[3];
//	unsigned int x,y,fc,bc;
//	tSysAddBuff[0]='0'+(tSysAdd/10);
//	tSysAddBuff[1]='0'+(tSysAdd%10);
//	x=30;y=0;fc=BLACK;bc=GOLD;
////	LcdDisplayStringZH(tSysAddBuff,x,y,fc,bc,0,GB2312_24x24_12x24_HZ);
//	//ShowStr30(30,15,tSysAddBuff,2,BLACK,GOLD);
//}
////sysY_pre,sysY,sysM_pre,sysM,sysD_pre,sysD,															   
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
////		LcdDisplayStringZH(tDateBuf,x,y,fc,bc,0,GB2312_24x24_12x24_HZ);
//		//ShowStr30(365+64,15,tDateBuf,10,BLACK,GOLD);
//	}
//}

//void Frm_Init(void)
//{
//	//TFT_Fill(0,0,799,15,0x0000);	
//	//TFT_Fill(0,0,24,479,0x0000);	
//	//TFT_Fill(0,460,799,479,0x0000);	
//	TFT_Fill(0,0,799,49-15,GOLD);	
//	TFT_Fill(0,412,799,459,GOLD);	
//	//Dis_SysAdd(set_AddMb);

//	//Dis_Date();

//	Draw_Line( 6,418,794,418,3,BLACK);
//	Draw_Line( 6,454,797,454,3,BLACK);
//	Draw_Line( 6,418, 6,454,3,BLACK);
//	Draw_Line(794,418,794,457,3,BLACK);

////	lcd_text24( 15,424,BLUE,GOLD,"路号");//   发油  应发量          实发量");
////	lcd_text24(120,424,BLUE,GOLD,"状态");//   发油  应发量          实发量");
////	lcd_text24(220,424,BLUE,GOLD,"应发量");//   发油  应发量          实发量");
////	lcd_text24(510,424,BLUE,GOLD,"实发量");//   发油  应发量          实发量");
////	TFT_Fill(510-20,418,509,418+18,BLACK);
////	TFT_Fill(794-20,418,794,418+18,BLACK);
////	lcd_text16(510-17,418,0xFFFF,0,"T");
////	lcd_text16(794-17,418,0xFFFF,0,"T");
//}
//void Draw_Frame(u8 x,u8 y,u16 color)
//{
//	//unsigned int tLeft[6]={55-55,233-55,411-55,589-55,767-55,740-55};
//	unsigned int tLeft[6]={0,160,320,480,640,800};
//	unsigned int tTop[4]={0,160,320,480};
//	if(x<5)
//	{
//		Draw_Line(tLeft[x],tTop[y],tLeft[x+1],tTop[y],3,color);
//		Draw_Line(tLeft[x],tTop[y+1],tLeft[x+1]+3,tTop[y+1],3,color);
//		Draw_Line(tLeft[x],tTop[y],tLeft[x],tTop[y+1],3,color);
//		Draw_Line(tLeft[x+1],tTop[y],tLeft[x+1],tTop[y+1],3,color);
//	}
//	else
//	{
//		Draw_Sel(750-5,210+y*50-5,50,40,color);	
//	}
//}
//void Draw_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen)
//{
//unsigned char i;
//	//unsigned int x1,x2,y1,y2;
//unsigned int hl,ht;
//	//x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
//	//700+(80-32)/2;
//	//330+(60-16)/2
//	i=tLen;
//	{
//		hl=tLeft+(tWidth-i*16)/2;
//		ht=tTop+(tHeight-16)/2;
//	  lcd_text16(hl,ht,0,0xFFFF,p);
//	}

//}

//void Draw_Button_Cap(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,char *p,unsigned char tLen)
//{
//unsigned char i;
//unsigned int x1,x2,y1,y2;
//unsigned int hl,ht;
//x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
////0xffff,0xe6fb,0x4208,0x0000
////0xFFFF,0xEF5D,0xDEFC,0xA535,0x8431,0x0000
//	TFT_DrawLine(x1,y1,x1,y2,0xFFFF);	//left3 
//	TFT_DrawLine(x1,y1,x2,y1,0xFFFF);	//top3 
//	TFT_DrawLine(x1-1,y1-1,x1-1,y2+1,0xEF5D);	//left2 
//	TFT_DrawLine(x1-1,y1-1,x2+1,y1-1,0xEF5D);	//top2 
//	TFT_DrawLine(x1-2,y1-2,x1-2,y2+2,0xDEFC);	//left1 
//	TFT_DrawLine(x1-2,y1-2,x2+2,y1-2,0xDEFC);	//top1 

// 	TFT_DrawLine(x2,y1,x2,y2,0xA535);//right1	
//	TFT_DrawLine(x2,y2,x1,y2,0xA535);//buttom1	
// 	TFT_DrawLine(x2+1,y1-1,x2+1,y2+1,0x8431);//right2	
//	TFT_DrawLine(x2+1,y2+1,x1-1,y2+1,0x8431);//buttom2	
// 	TFT_DrawLine(x2+2,y1-2,x2+2,y2+2,0x0000);//right3	
//	TFT_DrawLine(x2+2,y2+2,x1-2,y2+2,0x0000);//buttom3	

//	i=tLen;
//	{
//		hl=x1+(tWidth-i*16)/2;
//		ht=y1+(tHeight-16)/2;
//	  lcd_text16(hl,ht,0,0xFFFF,p);
//	}
//}
//void Draw_Button(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight)
//{
//unsigned char i;
//unsigned int x1,x2,y1,y2;
//x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
////0xffff,0xe6fb,0x4208,0x0000
////0xFFFF,0xEF5D,0xDEFC,0xA535,0x8431,0x0000
//	TFT_DrawLine(x1,y1,x1,y2,0xFFFF);	//left3 
//	TFT_DrawLine(x1,y1,x2,y1,0xFFFF);	//top3 
//	TFT_DrawLine(x1-1,y1-1,x1-1,y2+1,0xEF5D);	//left2 
//	TFT_DrawLine(x1-1,y1-1,x2+1,y1-1,0xEF5D);	//top2 
//	TFT_DrawLine(x1-2,y1-2,x1-2,y2+2,0xDEFC);	//left1 
//	TFT_DrawLine(x1-2,y1-2,x2+2,y1-2,0xDEFC);	//top1 

// 	TFT_DrawLine(x2,y1,x2,y2,0xA535);//right1	
//	TFT_DrawLine(x2,y2,x1,y2,0xA535);//buttom1	
// 	TFT_DrawLine(x2+1,y1-1,x2+1,y2+1,0x8431);//right2	
//	TFT_DrawLine(x2+1,y2+1,x1-1,y2+1,0x8431);//buttom2	
// 	TFT_DrawLine(x2+2,y1-2,x2+2,y2+2,0x0000);//right3	
//	TFT_DrawLine(x2+2,y2+2,x1-2,y2+2,0x0000);//buttom3	

//	for(i=0;i<3;i++)
//	{
//		TFT_DrawLine(x1-3-i,y1-3-i,x1-3-i,y2+3+i,0xFFFF);	 
//		TFT_DrawLine(x1-3-i,y1-3-i,x2+3+i,y1-3-i,0xFFFF);
//	 	TFT_DrawLine(x2+3+i,y1-3-i,x2+3+i,y2+3+i,0xFFFF);
//		TFT_DrawLine(x2+3+i,y2+3+i,x1-3-i,y2+3+i,0xFFFF);
//	}
//}

//void Draw_Sel(unsigned int tLeft,unsigned int tTop,unsigned int tWidth,unsigned int tHeight,unsigned int tColor)
//{
//	unsigned int x1,x2,y1,y2;
//	unsigned char i;
//	x1=tLeft;y1=tTop;x2=tLeft+tWidth;y2=tTop+tHeight;
//	for(i=0;i<3;i++)
//	{
//		TFT_DrawLine(x1,y1+i,x2,y1+i,tColor);	//TOP LINE
//		TFT_DrawLine(x1+i,y1,x1+i,y2,tColor);	//LEFT LINE
//		TFT_DrawLine(x1,y2-i,x2,y2-i,tColor);	//Buttom LINE
//		TFT_DrawLine(x2-i,y1,x2-i,y2,tColor);	//Right LINE
//	}
//}

//void DisSetSel(unsigned char x,unsigned char y,unsigned int tColor)
//{
//	//Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,WHITE);
//	if(x==0)//0~9
//	{		Draw_Sel(150-5,58-5+y*35,56+10,22+10,tColor);	}
//	else if(x==1)//10~19	
//	{		Draw_Sel(350-5,58-5+y*35,56+10,22+10,tColor);	}
//	else if(x==2)
//	{		Draw_Sel(565-5,58-5+y*35,76+10,22+10,tColor);}	
//	else if(x==3)
//	{		Draw_Sel(700-5,58-5+y*35,80+10,22+10,tColor);}	
//}
//void Dis_Detail(unsigned char tId)
//{
////	char disBuf[15];
////	IdDetail=tId;
////	disBuf[0]=IdSlave[tId]/10+'0';
////	if(disBuf[0]=='0')disBuf[0]=' ';
////	disBuf[1]=IdSlave[tId]%10+'0';
////	disBuf[2]=0;
//// 	lcd_text16(120, 80,RED,RED,disBuf);
////	DrawSetDat(tId);
////	DrawDiagLamp(tId);
//}
//void FyDat2Chr(void)
//{
//	unsigned int tDat1;
//	unsigned char tii;
//	tDat1=u16SavFyDat;
//	u8SavFyBuf[0]=tDat1/100;
//	tDat1=tDat1%100;
//	u8SavFyBuf[1]=tDat1/10;
//	tDat1=tDat1%10;
//	u8SavFyBuf[2]=tDat1;
//	for(tii=0;tii<3;tii++)
//		u8SavFyBuf[tii]+=0x30;
//	u8SavFyBuf[3]='\0';
//	if(u8SavFyBuf[0]=='0')
//	{
//		if(u8SavFyBuf[1]=='0')
//		{
//			u8SavFyBuf[1]=' ';	
//		}
//			u8SavFyBuf[0]=' ';	
//	}
//}
//void WrSetSel(unsigned char x,unsigned char y,char *s)
//{
//	if(frmNum==FRM_SETTING)
//	{
//	//Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,WHITE);
//	if(x==0)//0~9
//	{
//		TFT_Fill(170,62+y*35,170+24,62+y*35+16,WHITE);
//		lcd_text16(170,62+y*35,0,0,s);
//	}
//	else if(x==1)//10~19
//	{
//		TFT_Fill(370,62+y*35,370+24,62+y*35+16,WHITE);
//		lcd_text16(370,62+y*35,0,0,s);
//	}
//	else if(x==2)
//	{
//		TFT_Fill(565+2,58+2+y*35,565+64,58+2+y*35+16,WHITE);
//		lcd_text16(565+17,62+y*35,0,0,s);	 //17=(42-8)/2
//	}
//	}
//}

//extern unsigned int  ftaDIdat,ftaDIdat_pre;
//extern unsigned char ftaDOdat,ftaDOdat_pre,wrFtaDoDat,FTAdat;
//unsigned char tDoDat,tDoDat_pre;
//void ReNewDIO(void)
//{
//	unsigned int  tDiDat,tdat;
//	unsigned char i;
//	tDiDat=ftaDIdat;tDoDat=FTAdat;//tDoDat=ftaDOdat;
//	if((ftaDIdat!=ftaDIdat_pre)||(tDoDat!=tDoDat_pre))//if((ftaDIdat!=ftaDIdat_pre)||(ftaDOdat!=ftaDOdat_pre))
//	{
//		ftaDIdat_pre=ftaDIdat;
//		for(i=0;i<12;i++)
//		{
//				tdat=1<<i;
//				if((tDiDat&tdat)==tdat)
//					LcdDrawCircle(515,76+(i*30),6,RED,1);
//				else
//					LcdDrawCircle(515,76+(i*30),6,BLUE,1);
//		}
//	//}
//	//if(ftaDOdat!=ftaDOdat_pre)
//	//{
//		tDoDat_pre=tDoDat;
//		for(i=0;i<8;i++)
//		{
//				tdat=1<<i;
//				if((tDoDat&tdat)==tdat)
//					LcdDrawCircle(715,76+(i*30),6,RED,1);
//				else
//					LcdDrawCircle(715,76+(i*30),6,BLUE,1);
//		}
//	
//	}
//}
//void DrDIO(unsigned int diDat,unsigned char doDat)
//{
//	unsigned int  tDiDat,tdat;
//	unsigned char tDoDat,i;
//	tDiDat=diDat;tDoDat=doDat;
//	for(i=0;i<12;i++)
//	{
//			tdat=1<<i;
//			if((tDiDat&tdat)==tdat)
//				LcdDrawCircle(515,76+(i*30),6,RED,1);
//			else
//				LcdDrawCircle(515,76+(i*30),6,BLUE,1);
//	}
//}
//extern unsigned char xpos,ypos;
//extern void DrSetSel(unsigned char x,unsigned char y,unsigned char id);
//extern char disAddrBuf[3];
//extern unsigned char AddrIfix,AddrIfix_pre;
//extern unsigned char sysSavDat[10];
//void Dis_Scr(unsigned char pgNum)
//{
//	unsigned int i=0;
////	unsigned char dx;
//	unsigned char tx;
////	unsigned char ti;
////	unsigned char cx,cy;
////	unsigned char tempDat;
//	char tDisBuf[10];
//	//char *tStr;
//	__disable_irq();
////	Frm_Init();
////	dx=10;
//	//flgManCon=0;
//	//raInit();
//	TFT_Fill(0,50,799,430,WHITE);	
//	TFT_Fill(0,0,799,50,BLUE);	
//	Draw_Line( 0,50,799,50,3,BLACK);
//	lcd_text24(320,13,WHITE,BLUE,"称");
//	lcd_text24(354,13,WHITE,BLUE,"重");
//	lcd_text24(388,13,WHITE,BLUE,"管");
//	lcd_text24(422,13,WHITE,BLUE,"控");
//	lcd_text24(456,13,WHITE,BLUE,"仪");
//	
//	TFT_Fill(0,430,799,479,BLUE);	
//	Draw_Line( 0,430,799,430,3,BLACK);
//	lcd_text24( 10,443,WHITE,BLUE,"Gz-333");
//	lcd_text24(480,443,WHITE,BLUE,"南京格致自动化技术有限公司");
//	switch(pgNum)
//	{

//		case FRM_MAIN:
//			Draw_Line(  7, 67,  7,363,3,BLACK);
//			Draw_Line(  7, 67,413, 67,3,BLACK);
//			Draw_Line(413,363,413, 67,3,BLACK);
//			Draw_Line(413,363,  7,363,3,BLACK);
//			lcd_text24(  7,385,BLACK,WHITE,"示重：");
//			//lcd_text24(210,385,BLACK,WHITE,"皮重：");
//		tDisBuf[0]='D';tDisBuf[1]='i';tDisBuf[2]='1';tDisBuf[3]=':';tDisBuf[4]=':';tDisBuf[5]='\0';
//			for(i=0;i<12;i++)
//			{
//				Draw_Line(450, 60+(i*30),600,60+(i*30),1,BLACK);
//				tDisBuf[2]=0x30+((i+1)/10);	tDisBuf[3]=0x30+((i+1)%10);
//				if(tDisBuf[2]=='0')tDisBuf[2]=' ';
//				lcd_text16(465,68+(i*30),BLACK,WHITE,tDisBuf);
//			}
//			tDisBuf[0]='D';tDisBuf[1]='i';tDisBuf[2]='1';tDisBuf[3]=':';tDisBuf[4]='\0';
//			for(i=0;i<8;i++)
//			{
//				Draw_Line(600, 60+(i*30),750,60+(i*30),1,BLACK);
//				tDisBuf[2]=0x31+i;	
//				tDisBuf[1]='o';
//				lcd_text16(615,68+(i*30),BLACK,WHITE,tDisBuf);
//			}
//				Draw_Line(600, 60+(8*30),750,60+(8*30),1,BLACK);
//				Draw_Line(450,420,750,420,1,BLACK);
//				Draw_Line(450, 60,450,420,1,BLACK);
//				Draw_Line(600, 60,600,420,1,BLACK);
//				Draw_Line(750, 60,750,420,1,BLACK);
//		break;			
//		case FRM_DETAIL://详细信息
//		break;
//		case FRM_SETTING:
//				LcdDrawRectangle( 30,150,120,330,0,0);
//			  lcd_text16( 46,142,BLACK,WHITE,"波特率");
//			  LcdDrawCircle(45,186,6,0,0);lcd_text16( 55,180,BLACK,WHITE,"1200bps");
//				LcdDrawCircle(45,226,6,0,0);lcd_text16( 55,220,BLACK,WHITE,"2400bps");
//			  LcdDrawCircle(45,266,6,0,0);lcd_text16( 55,260,BLACK,WHITE,"4800bps");	
//			  LcdDrawCircle(45,306,6,0,0);lcd_text16( 55,300,BLACK,WHITE,"9600bps");
//				tx=sysSavDat[0]-1;tx=tx*40;
//				LcdDrawCircle(45,tx+186,3,0,1);

//				LcdDrawRectangle(130,150,220,330,0,0);
//			  lcd_text16(146,142,BLACK,WHITE,"数据位");
//			  LcdDrawCircle(150,186,6,0,0);lcd_text16(160,180,BLACK,WHITE,"5位");
//			  LcdDrawCircle(150,226,6,0,0);lcd_text16(160,220,BLACK,WHITE,"6位");
//			  LcdDrawCircle(150,266,6,0,0);lcd_text16(160,260,BLACK,WHITE,"7位");
//			  LcdDrawCircle(150,306,6,0,0);lcd_text16(160,300,BLACK,WHITE,"8位");
//				tx=sysSavDat[1]-1;tx=tx*40;
//				LcdDrawCircle(150,tx+186,3,0,1);
//				//LcdDrawCircle(150,306,3,0,1);

//				LcdDrawRectangle(230,150,320,330,0,0);
//			  lcd_text16(246,142,BLACK,WHITE,"停止位");
//			  LcdDrawCircle(250,186,6,0,0);lcd_text16(260,180,BLACK,WHITE,"1位");
//			  LcdDrawCircle(250,226,6,0,0);lcd_text16(260,220,BLACK,WHITE,"1.5位");
//			  LcdDrawCircle(250,266,6,0,0);lcd_text16(260,260,BLACK,WHITE,"2位");
//				tx=sysSavDat[2]-1;tx=tx*40;
//				LcdDrawCircle(250,tx+186,3,0,1);
//				//LcdDrawCircle(250,186,3,0,1);

//				LcdDrawRectangle(330,150,420,330,0,0);
//			  lcd_text16(346,142,BLACK,WHITE,"校验位");
//			  LcdDrawCircle(350,186,6,0,0);lcd_text16(360,180,BLACK,WHITE,"无校验");
//			  LcdDrawCircle(350,226,6,0,0);lcd_text16(360,220,BLACK,WHITE,"奇校验");
//			  LcdDrawCircle(350,266,6,0,0);lcd_text16(360,260,BLACK,WHITE,"偶校验");
//				tx=sysSavDat[3]-1;tx=tx*40;
//				LcdDrawCircle(350,tx+186,3,0,1);
//				//LcdDrawCircle(350,186,3,0,1);

//				LcdDrawRectangle( 20,120,430,340,0,0);
//			  lcd_text16(177,112,BLACK,WHITE,"地磅通讯设置");
//				LcdDrawRectangle(460,120,780,340,0,0);
//			  lcd_text16(583,112,BLACK,WHITE,"监控通讯设置");

//				LcdDrawRectangle(470,150,560,330,0,0);
//			  lcd_text16(486,142,BLACK,WHITE,"地址");
//				Draw_TextBox(490,183,50,24 );
//			  disAddrBuf[0]='0'+(AddrIfix/10);
//			  disAddrBuf[1]='0'+(AddrIfix%10);
//				disAddrBuf[2]=0;
//				if(disAddrBuf[0]=='0')
//					disAddrBuf[0]=' ';
//				lcd_text16(518,189,BLACK,WHITE,disAddrBuf);

//				LcdDrawRectangle(570,150,670,330,0,0);
//			  lcd_text16(586,142,BLACK,WHITE,"波特率");
//			  LcdDrawCircle(585,186,6,0,0);lcd_text16(595,180,BLACK,WHITE,"2400bps");
//				LcdDrawCircle(585,226,6,0,0);lcd_text16(595,220,BLACK,WHITE,"4800bps");
//			  LcdDrawCircle(585,266,6,0,0);lcd_text16(595,260,BLACK,WHITE,"9600bps");	
//			  LcdDrawCircle(585,306,6,0,0);lcd_text16(595,300,BLACK,WHITE,"19200bps");
//				tx=sysSavDat[5]-1;tx=tx*40;
//				LcdDrawCircle(585,tx+186,3,0,1);
//				//LcdDrawCircle(585,306,3,0,1);

//				LcdDrawRectangle(680,150,770,330,0,0);
//			  lcd_text16(696,142,BLACK,WHITE,"校验位");
//			  LcdDrawCircle(700,186,6,0,0);lcd_text16(710,180,BLACK,WHITE,"无校验");
//			  LcdDrawCircle(700,226,6,0,0);lcd_text16(710,220,BLACK,WHITE,"奇校验");
//			  LcdDrawCircle(700,266,6,0,0);lcd_text16(710,260,BLACK,WHITE,"偶校验");
//				LcdDrawCircle(700,186,3,0,1);
//				tx=sysSavDat[6]-1;tx=tx*40;
//				LcdDrawCircle(700,tx+186,3,0,1);
//				DrSetSel(0,0,1);
//break;
//	}
//	__enable_irq();
//}
//extern char disWeiDat[7];

//void Dis_RtDat(void)
//{
//	unsigned char i,flgDatOk=1;
//	char disBuf[10];
//	if(frmNum==FRM_MAIN)
//	{
//		for(i=0;i<6;i++)
//		{
//			disBuf[i]=disWeiDat[i];
//			if(((disBuf[i]<'0')||(disBuf[i]>'9'))&&(disBuf[i]!=0x20))
//			{
//				disBuf[i]=' ';
//				flgDatOk=0;
//				break;
//			}
//		}		
//		disBuf[6]=' ';disBuf[7]='K';disBuf[8]='g';disBuf[9]=0;
//		if(flgDatOk==1)
//			lcd_text24(79,385,BLACK,WHITE,disBuf);		
//	}
//}

//void Dis_Lcd(void)
//{
//	unsigned char ti;
//	static unsigned int OneSec_flag;
//	OneSec_flag++;
//	if( OneSec_flag>=50)
//	{
//		OneSec_flag=0;
//		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
////				TFT_Fill(200,170,300,300,WHITE);
//		lcd_text16( 50,150,BLACK,WHITE,"TIME TEST!");
//		Dis_Num16( 200,170,BLACK,WHITE,RTC_TimeStruct.RTC_Hours);
//		Dis_Num16( 224,190,BLACK,WHITE,RTC_TimeStruct.RTC_Minutes);
//		Dis_Num16( 248,210,BLACK,WHITE,RTC_TimeStruct.RTC_Seconds);
//	}
//	if(frmNumLast!=frmNum)
//	{
//		flgDisDate=1;
//		flgDisTime=1;
//		//flgFcDis=1;
//		if((frmNum==FRM_MAIN)||(frmNum==FRM_DETAIL))
//		{
//			flgReNew=1;
//		}
//		Dis_Scr( frmNum);//frmNum
//		sys_STA_Last=0xFF;
//		//RxdEvents();
//		frmNumLast=frmNum;
//	}
//}
//void ftoc(float tta)
//{
// unsigned char i;
// char *px;
// //unsigned char x[4];
// void *pf;
// px=c2f;
// pf=&tta;
// /*a=34.25;*/
// for(i=0;i<4;i++)
// *(px+i)=*((char *)pf+i);
//}

//float ctof(unsigned char dt1,unsigned char dt2,unsigned char dt3,unsigned char dt4)
//{
//float tta;
//u8 i,*px;
//u8 x[4];
//void *pf;
//x[0]=dt4;
//x[1]=dt3;
//x[2]=dt2;
//x[3]=dt1;

//px=x;             //px??????x
//pf=&tta;
//for(i=0;i<4;i++)
// *((char *)pf+i)=*(px+i);
//return tta;
//}


// /**
//  * 在指定位置显示中文
//  * @param zh         要显示的中文字符串
//  * @param x          X
//  * @param y          Y
//  * @param tForeColor 前景色
//  * @param tBackColor 背景色
//  * @param	GTFont_type 字体类型
//  */
// void LcdDisplayCharZH(char  zh, uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,GTFont_type tType )
// {
// 	raTextMode();
// 	raFontControlCGROMFont();
// 	raFontControlExternalCGROM();
// 	raSromClkDiv(0X03);
// 	raSerialROMSelect0();
// 	switch(tType)
// 	{
// 		case GB2312_15X16_8x16_HZ:
// 								raFontCodeGB2312();
// 								raFontSize16x16_8x16();
// 								break; 		
// 		case GB2312_24x24_12x24_HZ:
// 								raFontCodeGB2312();
// 								raFontSize24x24_12x24();
// 								break; 		
// 		case GB2312_32X32_16x32_HZ:
// 								raFontCodeGB2312();
// 								raFontSize32x32_16x32();
// 								break; 		

// 	}
// 	raGTSROMGT23L32S4W();
// 	raFontWriteCoordinate(x, y);
// 	raBTEBackgroundColor(tBackColor);
// 	raBTEForegroundColor(tForeColor);
// 	raDisplayChar(zh);
// }
void Dis_Lcd(void)
{
	unsigned char i;
	unsigned int  idDelta=183;
	char disBuf[10];
	for(i=0;i<12;i++)
	{
		 Draw_Line(0,48+i*36,799,48+i*36,1,0);
		 disBuf[0]=((i+1)/10)+0x30;
		 disBuf[1]=((i+1)%10)+0x30;
		 disBuf[2]=0;
		 lcd_text24(  5,54+i*36,0,0xFFFF,disBuf);
		 lcd_text16( 39,58+i*36,0,0xFFFF,"1#");
		 lcd_text16(222,58+i*36,0,0xFFFF,"2#");
		 lcd_text24(101,54+i*36,0,0xFFFF,"0%");
		 lcd_text24(147,54+i*36,0,0xFFFF,"离线");
		 lcd_text24(284,54+i*36,0,0xFFFF,"0%");
		 lcd_text24(330,54+i*36,0,0xFFFF,"离线");
		 disBuf[0]=((i+13)/10)+0x30;
		 disBuf[1]=((i+13)%10)+0x30;
		 disBuf[2]=0;
		 lcd_text24(405,54+i*36,0,0xFFFF,disBuf);
		 lcd_text16(439,58+i*36,0,0xFFFF,"1#");
		 lcd_text16(622,58+i*36,0,0xFFFF,"2#");
		 lcd_text24(501,54+i*36,0,0xFFFF,"0%");
		 lcd_text24(547,54+i*36,0,0xFFFF,"离线");
		 lcd_text24(684,54+i*36,0,0xFFFF,"0%");
		 lcd_text24(730,54+i*36,0,0xFFFF,"离线");
	}
		 Draw_Line(398,48,398,480,4,0);
		 Draw_Line( 34,48, 34,480,1,0);
		 Draw_Line(434,48,434,480,1,0);
		 Draw_Line(217,48,217,480,1,0);
		 Draw_Line(617,48,617,480,1,0);
	   
    //disBuf[0]= 
}

void Draw_EN24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s)
{
	DisEN24(s, x,  y, fc, bc,0);
}




