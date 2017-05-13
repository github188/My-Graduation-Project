/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
 

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "usart1.h"
#include "string.h"
#include "modbus_tcp.h"


//extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.
//extern u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.
//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
//extern u16 USART1_RX_STA;  

//#define			Data_Cover_Mode		//数据覆盖模式

//USART2中断程序
void USART1_IRQHandler()
{
	if( USART_GetITStatus( USART1,USART_IT_RXNE)!=RESET)//接受到数据
	{			
#ifdef		Data_Cover_Mode			////数据覆盖，新数据到来后清空之前数据,不论数据是否已经使用
		//接收到新数据时如果之前数据未处理，则覆盖
		if(0!=(USART1_RX_STA&0x8000))
		{
			USART1_RX_STA=0;//清除之前接收状态
			memset( USART1_RX_BUF,0,USART1_MAX_RECV_LEN);//清除之前接收数据
		}
		TIM_Cmd(TIM4,DISABLE); //关闭定时器计时
		//存储数据到缓冲数组
		USART1_RX_BUF[USART1_RX_STA++]=USART_ReceiveData( USART1);
		TIM_SetCounter( TIM4,0 );//清零计数器
		TIM_Cmd(TIM4,ENABLE); //开启定时器计时
#else						//数据不覆盖，等用户处理完后才能接收新数据
		if(0==(USART1_RX_STA&0x8000))
		{
			TIM_Cmd(TIM4,DISABLE); //关闭定时器计时
			//存储数据到缓冲数组
			USART1_RX_BUF[USART1_RX_STA++]=USART_ReceiveData( USART1);
			TIM_SetCounter( TIM4,0 );//清零计数器
			TIM_Cmd(TIM4,ENABLE); //开启定时器计时
		}
#endif		
		
	//清零接收中断标志		
	USART_ClearITPendingBit( USART1,USART_IT_RXNE) ;
	}
}

//定时器4中断服务程序--------10ms
void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
			
			//接收数据过程中超过10ms没有新数据则认为是一帧数据结束-置接收完成标志位
			USART1_RX_STA |= 0x8000;
			TIM_Cmd(TIM4,DISABLE); //关闭定时器计时-等待新的数据帧启动定时器
			recv_modbus_response((unsigned char*)USART1_RX_BUF);

//			//定时器测试程序
//			tt++;
//			if( tt>=50)
//			{
//				tt=0;
//				
//				USART_SendStr( USART1,"siemens test!\r\n");
//			}
		}
}




/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
 
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
