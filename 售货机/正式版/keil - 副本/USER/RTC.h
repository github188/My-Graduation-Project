#ifndef _RTC_H
#define _RTC_H 
#include "stm32f4xx.h"

void RTCHardwareConfig(void);
/**
 * 返回RTC的字符串， 日期格式 YY-MM-DD
 * @return 包含RTC信息的字符串
 */
char * RTCGetDateString(RTC_DateTypeDef *returnRTCdata);

/**
 * 返回RTC的字符串， 日期格式 HH:MM:SS
 * @return 包含RTC信息的字符串
 */
char * RTCGetTimeString(RTC_TimeTypeDef *returnRTCtime);


/**
 * 返回RTC的字符串， 日期格式 YY-MM-DD HH:MM:SS
 * @param  tArray 把日期值赋值给此数组
 * @return        返回包含RTC信息的字符串
 */
char * RTCGetDateAndTimeString(uint16_t * tArray);

uint8_t RTCGetWeekFromDay(uint8_t year,uint8_t month,uint8_t day);
void ReNewRTC(void);
void RTCSetTime(unsigned char tH,unsigned char tM,unsigned char tS);

#endif

