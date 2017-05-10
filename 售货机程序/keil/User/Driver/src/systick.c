
#include "systick.h"

#define ENABLE_SYSTICK_INT()	__set_PRIMASK(0)	/* 使能中断 */
#define DISABLE_SYSTICK_INT()	__set_PRIMASK(1)	/* 禁止中断 */

/*
	在此定义若干个软件定时器全局变量
	注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
*/
#define TMR_COUNT	4		/* 软件定时器的个数，第1个保留给bsp_DelayMS()使用 */


static SoftTimer_S timer[TMR_COUNT];

/* 全局运行时间，单位10ms，用于uIP */
 __IO uint32_t SysTickRunTime = 0;
static void prvSoftTimerDec(SoftTimer_S *_tmr);

/*
*********************************************************************************************************
*	函 数 名: sysTickInit
*	功能说明: 配置systick中断，并初始化软件定时器变量
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void sysTickInit(void)
{
	uint8_t i;

	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		timer[i].Count = 0;
		timer[i].PreLoad = 0;
		timer[i].Flag = 0;
		timer[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
	}

	/*
		配置systic中断周期为1ms，并启动systick中断。
    */
	SysTick_Config(SystemCoreClock / 1000);
}

/*
*********************************************************************************************************
*	函 数 名: SysTick_ISR
*	功能说明: SysTick中断服务程序，每隔1ms进入1次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SysTick_Handler(void)
{
	uint8_t i;

	/* 每隔1ms进来1次 */

	for (i = 0; i < TMR_COUNT; i++)
	{
		prvSoftTimerDec(&timer[i]);
	}

	SysTickRunTime++;	/* 全局运行时间每1ms增1 */
	if (SysTickRunTime == 0xFFFFFFFF)
	{
		SysTickRunTime = 0;
	}

}

/*
*********************************************************************************************************
*	函 数 名: prvSoftTimerDec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参：_tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static __INLINE void prvSoftTimerDec(SoftTimer_S *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}
/*********************************************************************************************************
*	函 数 名: sysTickDelayus
*	功能说明: us级延迟，延迟精度为正负1us
*	形    参：n : 延迟长度，单位1 us
*	返 回 值: 无
*********************************************************************************************************
*/
void sysTickDelayus(uint32_t n)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload = SysTick->LOAD; /* LOAD的值 */
		    	 
	ticks = n * 168; 			     /* 需要的节拍数 */	  		 
	tcnt = 0;
	told = SysTick->VAL;             /* 刚进入时的计数器值 */

	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	
		    /* 这里注意一下SYSTICK是一个递减的计数器 */    
			if(tnow < told)
			{
				tcnt += told - tnow;	
			}
			/* 重新装载递减 */
			else 
			{
				tcnt += reload - tnow + told;	
			}	    
			told = tnow;

			/*时间超过/等于要延迟的时间,则退出 */
			if(tcnt >= ticks)break;
		}  
	}
}
/**********************************************************************************************************
*	函 数 名: sysTickDelayms
*	功能说明: ms级延迟，延迟精度为正负1ms
*	形    参：n : 延迟长度，单位1 ms。
*	返 回 值: 无
*********************************************************************************************************
*/
void sysTickDelayms(uint32_t ms)
{

	int i;
	for(i = 0;i < ms;i ++)
	sysTickDelayus(1000);
}


/*
*********************************************************************************************************
*	函 数 名: sysTickStartSoftTimer
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参：	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 无
*********************************************************************************************************
*/
void sysTickStartSoftTimer(uint8_t _id, uint32_t _period, bool _auto)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); 死机 */
		return;
	}

	DISABLE_SYSTICK_INT();  			/* 关中断 */

	timer[_id].Count = _period;			/* 实时计数器初值 */
	timer[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	timer[_id].Flag = 0;				/* 定时时间到标志 */
	if(_auto)
		timer[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */
	else
		timer[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	ENABLE_SYSTICK_INT();  				/* 开中断 */
}


/*
*********************************************************************************************************
*	函 数 名: sysTickStopSoftTimer
*	功能说明: 停止一个定时器
*	形    参：	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*	返 回 值: 无
*********************************************************************************************************
*/
void sysTickStopSoftTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); 死机 */
		return;
	}

	DISABLE_SYSTICK_INT();  	/* 关中断 */

	timer[_id].Count = 0;				/* 实时计数器初值 */
	timer[_id].Flag = 0;				/* 定时时间到标志 */
	timer[_id].Mode = TMR_ONCE_MODE;	/* 自动工作模式 */

	ENABLE_SYSTICK_INT();  		/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: sysTickTimerIsTimeout
*	功能说明: 检测定时器是否超时
*	形    参：	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 返回 false 表示定时未到， true表示定时到
*********************************************************************************************************
*/
bool sysTickTimerIsTimeout(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return false;
	}

	if (timer[_id].Flag == 1)
	{
		timer[_id].Flag = 0;
		return true;
	}
	else
	{
		return false;
	}
}

/*
*********************************************************************************************************
*	函 数 名: sysTickGetRunTime
*	功能说明: 获取CPU运行时间，单位1ms
*	形    参：无
*	返 回 值: CPU运行时间，单位1ms
*********************************************************************************************************
*/
uint32_t sysTickGetRunTime(void)
{
	int32_t runtime;

	DISABLE_SYSTICK_INT();  	/* 关中断 */

	runtime = SysTickRunTime;	/* 由于在Systick中断被改写，因此关中断进行保护 */

	ENABLE_SYSTICK_INT();  		/* 开中断 */

	return runtime;
}


