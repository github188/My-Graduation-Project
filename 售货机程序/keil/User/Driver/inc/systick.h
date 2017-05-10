
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stm32f4xx.h>
#include <stdbool.h>

#define MAIN_TICK 0

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
enum
{
	TMR_ONCE_MODE = 0,	/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
};

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint32_t Count;	/* ������ */
	volatile uint32_t PreLoad;	/* ������Ԥװֵ */
}SoftTimer_S;

/* ���ⲿ���õĺ������� */
void sysTickInit(void);
void sysTickDelayus(uint32_t);
void sysTickDelayms(uint32_t);
void sysTickStartSoftTimer(uint8_t _id, uint32_t _period, bool _auto);
void sysTickStopSoftTimer(uint8_t _id);
bool sysTickTimerIsTimeout(uint8_t _id);
uint32_t sysTickGetRunTime(void);


#endif

