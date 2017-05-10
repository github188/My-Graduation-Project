#ifndef __KEYPAD_H
#define __KEYPAD_H 
#include "stm32f4xx.h"
#include <stddef.h>
#include "Global.h"

extern unsigned char u5RxdBuf[100];
extern unsigned int u5Cnt;
extern unsigned char PadCode[3],PadCodePre[3],flgPad;
extern unsigned char KeyCmd;

void KeyPad_Config(void);
void padHandler(void);
void KeyHandler(void);
void Key_Main(void);
void Key_Set(void);
void DrSetSel(unsigned char x,unsigned char y,unsigned char id);
void KeyHardwareConfig(void);
unsigned char LogIn(void);
void GetUsrID(char *tUsrID);
void ReadUsrPwd(void);
void Dis_CarId(unsigned char id);



#endif
