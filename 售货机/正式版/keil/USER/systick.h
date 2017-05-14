
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stm32f4xx.h>
#include <stdbool.h>

#define MAIN_TICK 0

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
enum
{
	TMR_ONCE_MODE = 0,	/* 一次工作模式 */
	TMR_AUTO_MODE = 1		/* 自动定时工作模式 */
};

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint8_t Mode;		/* 计数器模式，1次性 */
	volatile uint8_t Flag;		/* 定时到达标志  */
	volatile uint32_t Count;	/* 计数器 */
	volatile uint32_t PreLoad;	/* 计数器预装值 */
}SoftTimer_S;

/* 供外部调用的函数声明 */
void sysTickInit(void);
void sysTickDelayus(uint32_t);
void sysTickDelayms(uint32_t);
void sysTickStartSoftTimer(uint8_t _id, uint32_t _period, bool _auto);
void sysTickStopSoftTimer(uint8_t _id);
bool sysTickTimerIsTimeout(uint8_t _id);
uint32_t sysTickGetRunTime(void);


#endif

