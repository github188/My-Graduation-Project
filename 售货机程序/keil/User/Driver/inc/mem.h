#ifndef _MEM_H_
#define _MEM_H_
#pragma diag_suppress 870


#include "math.h"
#include "string.h"
//#include "comm.h"
extern  unsigned int PendAdd[10];
extern unsigned char LastDisLos;//

void Fill_Sqr_Lst(unsigned char id,char *tBuf);
void AddNewPendLst(unsigned char id);
void Del_PendAll(unsigned char id);
void Del_Pend(unsigned char id,char *OrdId);
void Dis_Id_Pend(unsigned char IdPend);
void Dis_1_Pend(unsigned char IdPend);
unsigned char GetPendNum(void);
char *GetPendYFL(unsigned char id,char *tOrdId);
void SavOilId_Goods(void);
unsigned char AddWarnLst(unsigned char id,unsigned char tAddWarnId);
unsigned int Fnd_Lst_Index(unsigned char id);
//unsigned int Fnd_Lst_Index_(void);
void GetPreWorkDat(void);
void SavNewWorkDat(void);
void Fill_New_Lst(unsigned char id,unsigned char tAdd);
void reNewSetDat(void);
void reNewSetDat_(void);
void SavEndDat(void);
char *Get_Pwd_Lst(unsigned int tIndex);
unsigned int Fnd_Pwd_Lst(char* OrdId);
void Fill_Man_Lst(char *tBuf);
unsigned char Fnd_Pend(unsigned char id,char *OrdId);
void Get_Pend_Lst(unsigned char ChNum);//待发确认界面下读取待发记录
unsigned char dGetPendNum(unsigned char id);
unsigned char GetLstNum(void);

#endif
