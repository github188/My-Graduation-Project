#ifndef		__UI_H__
#define		__UI_H__
#include "LCD.h"

    /* �з����������� */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed     long long int64_t;

    /* �޷����������� */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned     long long uint64_t;

//��ʾ��Ϣ�߶ȷ���
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

#define		LCD_WIDTH					800				//LCD���
#define		LCD_HEIGHT				480				//LCD�߶�

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
#define UI_COLOR_BROWN 			 			BROWN //��ɫ
#define UI_COLOR_BRRED 			 			0XFC07 //�غ�ɫ
#define UI_COLOR_GRAY  			 			GRAY //��ɫ
//GUI��ɫ

#define UI_COLOR_DARKBLUE      	 0X01CF	//����ɫ
#define UI_COLOR_LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define UI_COLOR_GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define UI_COLOR_LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define UI_COLOR_LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define UI_COLOR_LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define UI_COLOR_LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)




//UI��ֲ����
uint8_t UI_Init( void);//��ʼ��
void UI_DisplayOn(void);//����ʾ
void UI_DisplayOff(void);//����ʾ
void UI_Clear(uint16_t Color);//����
void UI_SetCursor(uint16_t Xpos, uint16_t Ypos);//���ù��
void UI_DrawPoint(uint16_t x,uint16_t y);//����
void UI_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//���ٻ���
uint16_t  UI_ReadPoint(uint16_t x,uint16_t y);//���� 
void UI_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);//��Բ
void UI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);//����
void UI_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);//������
void UI_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);//��䵥ɫ
void UI_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);//���ָ����ɫ
void UI_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);//��ʾһ���ַ�
void UI_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);//��ʾһ������
void UI_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);//��ʾ ����
void UI_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);//��ʾһ���ַ���,12/16����



#endif




