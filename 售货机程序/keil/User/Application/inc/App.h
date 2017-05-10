#ifndef __APP_H
#define __APP_H 

#include "stm32f4xx.h"
#include "Global.h"

unsigned Main_Con(unsigned char id);
void TickEvents(void);
void RxdEvents(void);
extern void lcd_text32(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);

#endif

