#ifndef		__UI_H__
#define		__UI_H__
#include "LCD.h"

    /* 有符号数据类型 */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed     long long int64_t;

    /* 无符号数据类型 */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned     long long uint64_t;

//提示信息高度分配
typedef enum
{
	TITLE_HEIG=50,
	CHECK_USB_HEIG=100,
	USB_RESU_HEIG=120,
	CHECK_FILE_HEIG=200,
	FILE_RESU_HEIG=220,
	VERIFY_FILE_HEIG=300,
	VERIFY_RESU_HEIG=320,
	UPDATE_SUCCESS_HEIG=400,
	LOADING_SYSTEM_HEIG=420
}UI_HEIGHT_DISTRIBUTE;

#define		LCD_WIDTH					800				//LCD宽度
#define		LCD_HEIGHT				480				//LCD高度

//color
#define UI_COLOR_WHITE         	 	WHITE
#define UI_COLOR_BLACK         		BLACK	  
#define UI_COLOR_BLUE         		BLUE  
#define UI_COLOR_BRED           	0XF81F
#define UI_COLOR_GRED 						0XFFE0
#define UI_COLOR_GBLUE			 		  0X07FF
#define UI_COLOR_RED           	 	RED
#define UI_COLOR_MAGENTA       	 	0xF81F
#define UI_COLOR_GREEN         	 	GREEN
#define UI_COLOR_CYAN          	 	0x7FFF
#define UI_COLOR_YELLOW        	 	0xFFE0
#define UI_COLOR_BROWN 			 			BROWN //棕色
#define UI_COLOR_BRRED 			 			0XFC07 //棕红色
#define UI_COLOR_GRAY  			 			GRAY //灰色
//GUI颜色

#define UI_COLOR_DARKBLUE      	 0X01CF	//深蓝色
#define UI_COLOR_LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define UI_COLOR_GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define UI_COLOR_LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define UI_COLOR_LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define UI_COLOR_LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define UI_COLOR_LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)




//UI移植函数
uint8_t UI_Init( void);//初始化
void UI_DisplayOn(void);//开显示
void UI_DisplayOff(void);//关显示
void UI_Clear(uint16_t Color);//清屏
void UI_SetCursor(uint16_t Xpos, uint16_t Ypos);//设置光标
void UI_DrawPoint(uint16_t x,uint16_t y);//画点
void UI_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//快速画点
uint16_t  UI_ReadPoint(uint16_t x,uint16_t y);//读点 
void UI_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);//画圆
void UI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);//画线
void UI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);//画矩形
void UI_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);//填充单色
void UI_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);//填充指定颜色
void UI_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);//显示一个字符
void UI_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);//显示一个数字
void UI_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);//显示 数字
void UI_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);//显示一个字符串,12/16字体



#endif




