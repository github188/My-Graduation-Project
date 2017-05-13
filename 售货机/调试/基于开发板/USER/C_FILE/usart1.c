#include "sys.h"
#include "usart1.h"	
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "timer4_usart.h"

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif


volatile u16 USART1_RX_STA=0;  
volatile u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.
volatile u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.


//��ʼ��IO ����1 
//bound:������
/*
**���ڽ������ö�ʱ��������ݼ���ķ�ʽ�ж�һ֡�����Ƿ����--����ʹ�õ���tim3--�������Ϊ5ms<���ʱ�������Ӱ��ϵͳ���ܣ�����>
*/
void Usart_Init(u32 bound)
{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//��ʱ��3���ڴ������ݽ�����ɵ��ж�
	TIM4_Int_Init( FRAME_END_TIME_100US-1,8400-1);//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����100��Ϊ10ms   	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

//ʮ�����Ʒ���-��Ҫָ���������ݵĳ���
void USART_SendHex(USART_TypeDef* USARTx, u8* Hdata,u16 len)
{
	u16 r ,i;
	r=len;
	for(i=0;i<r;i++)
	{
		USART_ClearFlag(USARTx, USART_FLAG_TC);	//����жϱ�־λ,��֤��һ���ֽڵ����ݲ���ʧ
		USART_SendData(USARTx, Hdata[i]);//�򴮿�1��������	
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}
//�ַ�������-�ַ�������0Ϊ������������strlen�������ܼ��ʮ����������ʵ�ʳ���
void USART_SendStr(USART_TypeDef* USARTx,char* Pdata)
{
	u16 r ,i;
	r=strlen( Pdata);
	for(i=0;i<r;i++)
	{
		USART_ClearFlag(USARTx, USART_FLAG_TC);	//����жϱ�־λ,��֤��һ���ֽڵ����ݲ���ʧ
		USART_SendData(USARTx, Pdata[i]);//�򴮿�1��������	
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

void u1_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	USART_SendStr( USART1,(char*)USART1_TX_BUF);
}




