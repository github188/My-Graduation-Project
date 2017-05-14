#include "sys.h"
#include "usart1.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "timer4_usart.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
    USART1->DR = (u8) ch;
    return ch;
}
#endif


volatile u16 USART1_RX_STA = 0;
volatile u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.
volatile u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.


//初始化IO 串口1
//bound:波特率
/*
**串口接收利用定时器检测数据间隔的方式判断一帧数据是否结束--这里使用的是tim3--间隔上限为5ms<这个时间过长会影响系统性能！！！>
*/
void Usart_Init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //定时器3用于串口数据接收完成的判断
    TIM4_Int_Init( FRAME_END_TIME_100US - 1, 8400 - 1); //定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数100次为10ms

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    /* Configure USART Tx  as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART Rx  as input floating */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART中断配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* Configure the USART */
    USART_Init(USART2, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(USART2, ENABLE);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

//十六进制发送-需要指定发送数据的长度
void USART_SendHex(USART_TypeDef* USARTx, u8* Hdata, u16 len)
{
    u16 r , i;
    r = len;
    for(i = 0; i < r; i++)
    {
        USART_ClearFlag(USARTx, USART_FLAG_TC);	//清除中断标志位,保证第一个字节的数据不丢失
        USART_SendData(USARTx, Hdata[i]);//向串口1发送数据
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET); //等待发送结束
    }
}
//字符串发送-字符串是以0为结束符，故用strlen函数不能检测十六进制数组实际长度
void USART_SendStr(USART_TypeDef* USARTx, char* Pdata)
{
    u16 r , i;
    r = strlen( Pdata);
    for(i = 0; i < r; i++)
    {
        USART_ClearFlag(USARTx, USART_FLAG_TC);	//清除中断标志位,保证第一个字节的数据不丢失
        USART_SendData(USARTx, Pdata[i]);//向串口1发送数据
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET); //等待发送结束
    }
}

void u1_printf(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)USART1_TX_BUF, fmt, ap);
    va_end(ap);
    USART_SendStr( USART2, (char*)USART1_TX_BUF);
}



