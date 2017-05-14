#include "COMTimer.h"
#include "stm32f4xx.h"

#define COM_TIMER TIM4
#define COM_TIMER_IRQn TIM4_IRQn
#define COM_TIMER_IRQHandler TIM4_IRQHandler
#define COM_TIMER_RCC RCC_APB1Periph_TIM4

void COMTimerConfig(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	RCC_APB1PeriphClockCmd(COM_TIMER_RCC, ENABLE); 
	//通讯处理程序50us中断一次
	TIM_DeInit(COM_TIMER);												//重新将Timer设置为缺省值       
	TIM_InternalClockConfig(COM_TIMER);									//采用内部时钟给COM_TIMER提供时钟源       
	TIM_TimeBaseStructure.TIM_Prescaler = 42 - 1;					//42-1，这样计数器时钟为84MHz/42 = 2MHz      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//设置时钟分割      
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//设置计数器模式为向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;						//     50us更新一次
	TIM_TimeBaseInit(COM_TIMER,&TIM_TimeBaseStructure);					//将配置应用到COM_TIMER中      
	TIM_ClearFlag(COM_TIMER, TIM_FLAG_Update);							//清除溢出中断标志			
	TIM_ARRPreloadConfig(COM_TIMER, DISABLE);								//禁止ARR预装载缓冲器，当预装载缓冲器被禁止时，写入自动装入的值(TIMx_ARR)的数值会直接传送到对应的影子寄存器；如果使能预加载寄存器，则写入 ARR //的数值会在更新事件时，才会从预加载寄存器传送到对应的影子寄存器

  
	//中断配置
	NVIC_InitStructure.NVIC_IRQChannel = COM_TIMER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	TIM_ClearFlag(COM_TIMER, TIM_FLAG_Update); //清除溢出中断标志
    TIM_SetCounter(COM_TIMER,0x00);			//清零计数器值
    TIM_ITConfig(COM_TIMER,TIM_IT_Update,ENABLE);
    TIM_Cmd(COM_TIMER,ENABLE);
}


//50us中断
void COM_TIMER_IRQHandler(void)
{
	static uint32_t t1msCnt = 0;
       //检测是否发生溢出更新事件
       if(TIM_GetITStatus(COM_TIMER, TIM_IT_Update) != RESET)
       {
			//清除COM_TIMER的中断待处理位
			TIM_ClearITPendingBit(COM_TIMER , TIM_FLAG_Update);
			t1msCnt++;
			if(t1msCnt > 20)
			{
				t1msCnt = 0;

			}

			
       }
}

