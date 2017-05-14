
#include "BL_UI.h"
#include "LCD.h"
#include "Global.h"

extern void raDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);


//UI移植函数
uint8_t UI_Init( void)//初始化
{
	//
	lcdInit( );
	//
	UI_Fill( 0,0,LCD_WIDTH,LCD_HEIGHT,GRAY);//填充背景UI_COLOR_GRAY
	
return 0;
}
//开显示
void UI_DisplayOn(void)
{
	
}
//关显示
void UI_DisplayOff(void)
{
	
}
//清屏
void UI_Clear(uint16_t Color)
{
	
}
//设置光标
void UI_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	
}
//画点
void UI_DrawPoint(uint16_t x,uint16_t y)
{
}
//快速画点	
void UI_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	
}
//读点 
uint16_t  UI_ReadPoint(uint16_t x,uint16_t y)
{
	
return 0;
}
//画圆
void UI_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	
}
//画线
void UI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  raDrawLine( x1,  y1,  x2,  y2);
}
//画矩形
void UI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	
}
//填充单色
void UI_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{ 
	LcdDrawRectangle(  sx, sy, ex, ey, color,1);
}
//填充指定颜色
void UI_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{ 
	
}
//显示一个字符
void UI_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{ 
	
}
//显示一个数字
void UI_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{ 
	Dis_Num16(  x, y,UI_COLOR_GRAY , UI_COLOR_RED, num);
}
//显示 数字
void UI_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{
	
}
//显示一个字符串,12/16字体
void UI_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{ 
	LcdDisplayASCII(  x, y, UI_COLOR_BLACK , UI_COLOR_WHITE, 0, (char*)p);
}














