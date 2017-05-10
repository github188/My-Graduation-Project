#include "RTC.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_MAX_SIZE 20
static char  _RTCTimeString[STRING_MAX_SIZE];
static char  _RTCDateString[STRING_MAX_SIZE];
static char  _RTCDateAndTimeString[STRING_MAX_SIZE * 2];
extern unsigned char sysY,sysM,sysD,sysHH,sysMM,sysSS;
/**
 * RTC硬件配置
 */
void RTCHardwareConfig(void)
{
	RTC_InitTypeDef RTC_InitStructure;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RCC_LSEConfig(RCC_LSE_ON); 
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待外部时钟准备好
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
{
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);	

	RTC_TimeStructure.RTC_Seconds = 0x00;
    RTC_TimeStructure.RTC_Minutes = 42;
    RTC_TimeStructure.RTC_Hours = 8;
    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);
  
    RTC_DateStructure.RTC_Date = 16;
    RTC_DateStructure.RTC_Month = 11;
    RTC_DateStructure.RTC_WeekDay= RTC_Weekday_Tuesday;
    RTC_DateStructure.RTC_Year = 16;
    RTC_SetDate(RTC_Format_BCD,&RTC_DateStructure);

    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}

//#warning 要用示波器测
//RTC_CalibOutputConfig(RTC_CalibOutput_512Hz);
//RTC_CalibOutputCmd(DISABLE);
// RTC_SmoothCalibConfig(RTC_SmoothCalibPeriod_8sec, RTC_SmoothCalibPlusPulses_Reset, 0XFF);
}

/**
 * 返回RTC的字符串， 日期格式 YY-MM-DD
 * 参数 ：返回包含日期的结构体，可以为NULL
 * @return 包含RTC信息的字符串
 */
char * RTCGetDateString(RTC_DateTypeDef *returnRTCdata)
{
	RTC_DateTypeDef tRTCdata;
	RTC_GetDate(RTC_Format_BIN, &tRTCdata);
  	memset(_RTCDateString, 0, STRING_MAX_SIZE);
  	char tmp[STRING_MAX_SIZE];
	char * tDash = " - ";
	sprintf(tmp, "%02i", (uint8_t)(tRTCdata.RTC_Year));
	strcat(_RTCDateString, tmp);
	strcat(_RTCDateString, tDash);
	sprintf(tmp, "%02i", (uint8_t)(tRTCdata.RTC_Month));
	strcat(_RTCDateString, tmp);
	strcat(_RTCDateString, tDash);
	sprintf(tmp, "%02i", (uint8_t)(tRTCdata.RTC_Date));
	strcat(_RTCDateString, tmp);
	if(returnRTCdata != NULL)
	{
		 returnRTCdata->RTC_WeekDay  = tRTCdata.RTC_WeekDay; 
		 returnRTCdata->RTC_Month    = tRTCdata.RTC_Month;   
		 returnRTCdata->RTC_Date     = tRTCdata.RTC_Date;    
		 returnRTCdata->RTC_Year     = tRTCdata.RTC_Year;    
	}
	return _RTCDateString;
}

/**
 * 返回RTC的字符串， 日期格式 HH:MM:SS
 * 参数 ：返回包含时间的结构体，可以为NULL
 * @return 包含RTC信息的字符串
 */
char * RTCGetTimeString(RTC_TimeTypeDef *returnRTCtime)
{
  	RTC_TimeTypeDef tRTCtime;
  	RTC_GetTime(RTC_Format_BIN, &tRTCtime);
  	memset(_RTCTimeString, 0, STRING_MAX_SIZE);
  	char tmp[STRING_MAX_SIZE];
	char * tColon = " : ";
	sprintf(tmp, "%02i", (uint8_t)(tRTCtime.RTC_Hours));
	strcat(_RTCTimeString, tmp);
	strcat(_RTCTimeString, tColon);
	sprintf(tmp, "%02i", (uint8_t)(tRTCtime.RTC_Minutes));
	strcat(_RTCTimeString, tmp);
	strcat(_RTCTimeString, tColon);
	sprintf(tmp, "%02i", (uint8_t)(tRTCtime.RTC_Seconds));
	strcat(_RTCTimeString, tmp);
	if(returnRTCtime != NULL)
	{
		 returnRTCtime->RTC_Hours  	= tRTCtime.RTC_Hours; 
		 returnRTCtime->RTC_Minutes  = tRTCtime.RTC_Minutes;   
		 returnRTCtime->RTC_Seconds  = tRTCtime.RTC_Seconds;    
		 returnRTCtime->RTC_H12     	= tRTCtime.RTC_H12;    

	}
	return _RTCTimeString;
}


/**
 * 返回RTC的字符串， 日期格式 YY-MM-DD HH:MM:SS
 * @param  tArray 把日期值赋值给此数组
 * @return        返回包含RTC信息的字符串
 */
char * RTCGetDateAndTimeString(uint8_t * tArray)
{
	RTC_DateTypeDef tRTCdata;
  	RTC_TimeTypeDef tRTCtime;
  	char *tTimeString;
  	char *tDataString;
	char * tSpace = "  ";
  	memset(_RTCTimeString, 0, (STRING_MAX_SIZE << 1));
  	tTimeString = RTCGetTimeString(&tRTCtime);//一定要写读时间再读日期
  	tDataString = RTCGetDateString(&tRTCdata);
	strcpy(_RTCDateAndTimeString , tDataString);
	strcat(_RTCDateAndTimeString, tSpace);
	strcat(_RTCDateAndTimeString, tTimeString);
	if(tArray != NULL)
	{
		tArray[0] = tRTCdata.RTC_Year;	 	 sysY=tArray[0];
		tArray[1] = tRTCdata.RTC_Month;	 	 sysM=tArray[1];
		tArray[2] = tRTCdata.RTC_Date;	 	 sysD=tArray[2];
		tArray[3] = tRTCtime.RTC_Hours; 	sysHH=tArray[3]; 
		tArray[4] = tRTCtime.RTC_Minutes; sysMM=tArray[4];  
		tArray[5] = tRTCtime.RTC_Seconds; sysSS=tArray[5];
	}

	return _RTCDateAndTimeString;
}

void RTCSetDate(unsigned char tY,unsigned char tM,unsigned char tD)
{
	RTC_DateTypeDef tRTCdata;
	tRTCdata.RTC_Year = tY;
	tRTCdata.RTC_Month =tM;
	tRTCdata.RTC_Date = tD;
	tRTCdata.RTC_WeekDay= 1;
	RTC_SetDate(RTC_Format_BIN, &tRTCdata);
}
void RTCSetTime(unsigned char tH,unsigned char tM,unsigned char tS)
{
	RTC_TimeTypeDef tRTCtime;
	tRTCtime.RTC_Hours =  tH;
	tRTCtime.RTC_Minutes =tM;
	tRTCtime.RTC_Seconds =tS;
	RTC_SetTime(RTC_Format_BIN, &tRTCtime);
}

/**
 * 获取第几周
 * @param  year  [description]
 * @param  month [description]
 * @param  day   [description]
 * @return       [description]
 */
uint8_t RTCGetWeekFromDay(uint8_t year,uint8_t month,uint8_t day) 
{     
  if(month < 3) { month += 12; if(year > 0) year--; else year = 4; } 
  
  day = day + (month<<1) + (uint8_t)(((month+1u)*154u)>>8) + year + (year>>2); 
  
  if(day >= 112) day -= 112; //day%7
  if(day >= 56)  day -= 56; 
  if(day >= 28)  day -= 28; 
  if(day >= 14)  day -= 14; 
  if(day >= 7)   day -= 7;
  
  return day+1;  // 返回星期几(星期一用1表示，而星期天用7表示)
}

/**
 * 年月日转换到时分秒
 * @param tString [description]
 * @param tArray  [description]
 */
void RTCConvertToString(char * tString, uint8_t * tArray)
{

  	char tmp[STRING_MAX_SIZE];
  	strcpy(tString, "");
	sprintf(tmp, "%02i", tArray[0]);
	strcat(tString, tmp);
	strcat(tString, "年");
	sprintf(tmp, "%02i", tArray[1]);
	strcat(tString, tmp);
	strcat(tString, "月");
	sprintf(tmp, "%02i", tArray[2]);
	strcat(tString, tmp);
	strcat(tString, "日");
	sprintf(tmp, "%02i", tArray[3]);
	strcat(tString, tmp);
	strcat(tString, "时");
	sprintf(tmp, "%02i", tArray[4]);
	strcat(tString, tmp);
	strcat(tString, "分");
	sprintf(tmp, "%02i", tArray[5]);
	strcat(tString, tmp);
	strcat(tString, "秒");
}