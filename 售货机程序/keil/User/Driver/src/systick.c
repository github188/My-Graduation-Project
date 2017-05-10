
#include "systick.h"

#define ENABLE_SYSTICK_INT()	__set_PRIMASK(0)	/* ʹ���ж� */
#define DISABLE_SYSTICK_INT()	__set_PRIMASK(1)	/* ��ֹ�ж� */

/*
	�ڴ˶������ɸ������ʱ��ȫ�ֱ���
	ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
*/
#define TMR_COUNT	4		/* �����ʱ���ĸ�������1��������bsp_DelayMS()ʹ�� */


static SoftTimer_S timer[TMR_COUNT];

/* ȫ������ʱ�䣬��λ10ms������uIP */
 __IO uint32_t SysTickRunTime = 0;
static void prvSoftTimerDec(SoftTimer_S *_tmr);

/*
*********************************************************************************************************
*	�� �� ��: sysTickInit
*	����˵��: ����systick�жϣ�����ʼ�������ʱ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sysTickInit(void)
{
	uint8_t i;

	/* �������е������ʱ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		timer[i].Count = 0;
		timer[i].PreLoad = 0;
		timer[i].Flag = 0;
		timer[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
	}

	/*
		����systic�ж�����Ϊ1ms��������systick�жϡ�
    */
	SysTick_Config(SystemCoreClock / 1000);
}

/*
*********************************************************************************************************
*	�� �� ��: SysTick_ISR
*	����˵��: SysTick�жϷ������ÿ��1ms����1��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SysTick_Handler(void)
{
	uint8_t i;

	/* ÿ��1ms����1�� */

	for (i = 0; i < TMR_COUNT; i++)
	{
		prvSoftTimerDec(&timer[i]);
	}

	SysTickRunTime++;	/* ȫ������ʱ��ÿ1ms��1 */
	if (SysTickRunTime == 0xFFFFFFFF)
	{
		SysTickRunTime = 0;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: prvSoftTimerDec
*	����˵��: ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
*	��    �Σ�_tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static __INLINE void prvSoftTimerDec(SoftTimer_S *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* ������Զ�ģʽ�����Զ���װ������ */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}
/*********************************************************************************************************
*	�� �� ��: sysTickDelayus
*	����˵��: us���ӳ٣��ӳپ���Ϊ����1us
*	��    �Σ�n : �ӳٳ��ȣ���λ1 us
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sysTickDelayus(uint32_t n)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload = SysTick->LOAD; /* LOAD��ֵ */
		    	 
	ticks = n * 168; 			     /* ��Ҫ�Ľ����� */	  		 
	tcnt = 0;
	told = SysTick->VAL;             /* �ս���ʱ�ļ�����ֵ */

	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	
		    /* ����ע��һ��SYSTICK��һ���ݼ��ļ����� */    
			if(tnow < told)
			{
				tcnt += told - tnow;	
			}
			/* ����װ�صݼ� */
			else 
			{
				tcnt += reload - tnow + told;	
			}	    
			told = tnow;

			/*ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
			if(tcnt >= ticks)break;
		}  
	}
}
/**********************************************************************************************************
*	�� �� ��: sysTickDelayms
*	����˵��: ms���ӳ٣��ӳپ���Ϊ����1ms
*	��    �Σ�n : �ӳٳ��ȣ���λ1 ms��
*	�� �� ֵ: ��
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
*	�� �� ��: sysTickStartSoftTimer
*	����˵��: ����һ����ʱ���������ö�ʱ���ڡ�
*	��    �Σ�	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sysTickStartSoftTimer(uint8_t _id, uint32_t _period, bool _auto)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); ���� */
		return;
	}

	DISABLE_SYSTICK_INT();  			/* ���ж� */

	timer[_id].Count = _period;			/* ʵʱ��������ֵ */
	timer[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	timer[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	if(_auto)
		timer[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */
	else
		timer[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	ENABLE_SYSTICK_INT();  				/* ���ж� */
}


/*
*********************************************************************************************************
*	�� �� ��: sysTickStopSoftTimer
*	����˵��: ֹͣһ����ʱ��
*	��    �Σ�	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sysTickStopSoftTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); ���� */
		return;
	}

	DISABLE_SYSTICK_INT();  	/* ���ж� */

	timer[_id].Count = 0;				/* ʵʱ��������ֵ */
	timer[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	timer[_id].Mode = TMR_ONCE_MODE;	/* �Զ�����ģʽ */

	ENABLE_SYSTICK_INT();  		/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: sysTickTimerIsTimeout
*	����˵��: ��ⶨʱ���Ƿ�ʱ
*	��    �Σ�	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ���� false ��ʾ��ʱδ���� true��ʾ��ʱ��
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
*	�� �� ��: sysTickGetRunTime
*	����˵��: ��ȡCPU����ʱ�䣬��λ1ms
*	��    �Σ���
*	�� �� ֵ: CPU����ʱ�䣬��λ1ms
*********************************************************************************************************
*/
uint32_t sysTickGetRunTime(void)
{
	int32_t runtime;

	DISABLE_SYSTICK_INT();  	/* ���ж� */

	runtime = SysTickRunTime;	/* ������Systick�жϱ���д����˹��жϽ��б��� */

	ENABLE_SYSTICK_INT();  		/* ���ж� */

	return runtime;
}


