#ifndef __LCD_H
#define __LCD_H 
#include <stdint.h>
#include "color.h"
#include <stdbool.h>
//#include "GTFont.h"
#pragma diag_suppress 870

#define SHOW_TIME  	88	//ÏÔÊ¾Ê±¼ä

void Dis_Lcd(void);


void Draw_Logo(unsigned int x,unsigned int y);
void Draw_Pkq_STA(unsigned char id,unsigned char sta);
void Dis_RtDat(void);

float ctof(unsigned char dt1,unsigned char dt2,unsigned char dt3,unsigned char dt4);
void ftoc(float tta);

/**
 * LCD初始化
 */
void lcdInit(void);
/**
 * lcd写像素函数
 * @param x      X
 * @param y      Y
 * @param tColor 颜色
 */
void lcdDrawPixel(uint16_t x, uint16_t y, uint16_t tColor);
/**
 * 读点像素
 * @param  x 位置X
 * @param  y 位置Y
 * @return   返回像素数据
 */
uint16_t lcdReadPixel(uint16_t x, uint16_t y);

/**
 * 调节背光
 * @param tPercent 0-100 亮度
 */
void lcdBacklightAdjust(uint8_t tPercent);

/**
 * 显示ASCII字符
 * @param x             X坐标
 * @param y             Y坐标
 * @param tForeColor    前景色
 * @param tBackColor    后景色
 * @param isTransparent 是否透明
 * @param str           字符
 */
void LcdDisplayASCII(uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor, bool isTransparent, char *str );


/**
 * 画线函数
 * @param x1     起点X
 * @param y1     起点Y
 * @param x2     终点X
 * @param y2     终点Y
 * @param tColor 颜色
 */
void LcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t tColor);
/**
 *  在LCD显存上画线
 * @param x      X
 * @param y      Y
 * @param tWidth 宽
 * @param arr    缓存
 */
void LcdMemoryDrawHLine(uint16_t x, uint16_t y, uint16_t tWidth, uint16_t* arr);
/**
 * 画矩形
 * @param x1      X1
 * @param y1      Y1
 * @param x2      X2
 * @param y2      Y2
 * @param		颜色
 * @param tIsfill 是否填充
 */
void LcdDrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t tColor, bool tIsfill);

/**
 * 画圆
 * @param X       圆心X
 * @param Y       圆心Y
 * @param R       半径
 * @param		颜色
 * @param tIsfill 是否填充
 */
void LcdDrawCircle(uint16_t X,uint16_t Y,uint16_t R, uint16_t tColor, bool tIsfill);

/**
 * 画椭圆
 * @param X       中心点X
 * @param Y       中心点Y
 * @param ELL_A   ELL_A
 * @param ELL_B   ELL_B
 * @param		颜色
 * @param tIsfill 是否填充
 */
void LcdDrawEllipse(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B, uint16_t tColor, bool tIsfill);

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
void LcdDrawEllipseCurve(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B,uint16_t tColor, bool tIsfill, uint8_t tPart);


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
void LcdDrawCircleSquare(uint16_t XL,uint16_t YT ,uint16_t XR ,uint16_t YB ,uint16_t ELL_A,uint16_t ELL_B, uint16_t tColor,bool tIsfill);

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
void LcdDrawTriangle(uint16_t P1_X,uint16_t P1_Y,uint16_t P2_X,uint16_t P2_Y,uint16_t P3_X,uint16_t P3_Y,  uint16_t tColor, bool tIsfill);

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
//void LcdDisplayStringZH(char * zh, uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,bool tTransparency, GTFont_type tType );
void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
void Dis_Num16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint16_t Dat);
void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
void DrDIO(unsigned int diDat,unsigned char doDat);
void ReNewDIO(void);


#endif

