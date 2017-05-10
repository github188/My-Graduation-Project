#include "UKeyBoard.h"
#include <stdbool.h>

#include "DIALOG.h"
//UART5 TX:PC12 RX:PD2 
#define KEY_RX_PIN GPIO_Pin_2
#define KEY_TX_PIN GPIO_Pin_12
#define KEY_RX_PINSOURCE GPIO_PinSource2
#define KEY_RX_GPIO GPIOD
#define KEY_TX_PINSOURCE GPIO_PinSource12
#define KEY_TX_GPIO GPIOC

#define KEY_AF	GPIO_AF_UART5

#define KEY_RX_CLK RCC_AHB1Periph_GPIOD
#define KEY_TX_CLK RCC_AHB1Periph_GPIOC
#define KEY_USART_CLK RCC_APB1Periph_UART5
#define KEY_USART UART5
#define KEY_USART_IRQ UART5_IRQn
#define KEY_USART_IRQHandler UART5_IRQHandler

#define KEY_TIMER TIM1
#define KEY_TIMER_IRQn TIM1_UP_TIM10_IRQn
#define KEY_TIMER_IRQHandler TIM1_UP_TIM10_IRQHandler
#define KEY_TIMER_RCC RCC_APB2Periph_TIM1

#define KEY_PRESSED 0x12
#define KEY_RELEASED 0x00


#define KEY_VALUE_1 		10	
#define KEY_VALUE_2			11
#define KEY_VALUE_3			12
#define KEY_VALUE_4			13
#define KEY_VALUE_5			14
#define KEY_VALUE_6			15
#define KEY_VALUE_7			16
#define KEY_VALUE_8			17
#define KEY_VALUE_9			18
#define KEY_VALUE_0			20
#define KEY_VALUE_DOT		21	

#define KEY_VALUE_LOCK		19

#define KEY_VALUE_LEFT		25	
#define KEY_VALUE_RIGHT		27	
#define KEY_VALUE_UP		23	
#define KEY_VALUE_DOWN		29	
#define KEY_VALUE_EDIT		26	
#define KEY_VALUE_ESC		28	
#define KEY_VALUE_ENTER		30	



static volatile  bool _KeyTimerCountEnable = false;
static volatile uint16_t _KeyTimerCnt = 0;
static volatile uint8_t _ReceivedData[10];
static volatile uint8_t _ReceivedDataIndex = 0;

static bool  _KeyEnterLocked = false;

static GUI_HSPRITE _CurrentFocusHandle;


void keyHardWareConfig(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;       

  	RCC_AHB1PeriphClockCmd(KEY_RX_CLK, ENABLE);
  	RCC_AHB1PeriphClockCmd(KEY_TX_CLK, ENABLE);
  	RCC_APB1PeriphClockCmd(KEY_USART_CLK, ENABLE);
  	RCC_APB2PeriphClockCmd(KEY_TIMER_RCC, ENABLE);




	GPIO_InitStructure.GPIO_Pin = KEY_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	// GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_TX_GPIO, &GPIO_InitStructure);

	/* Configure USART Rx  as input floating */

	GPIO_InitStructure.GPIO_Pin = KEY_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	// GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_RX_GPIO, &GPIO_InitStructure);

	/* Configure USART Tx  as alternate function push-pull */
  	GPIO_PinAFConfig(KEY_RX_GPIO, KEY_RX_PINSOURCE, KEY_AF);
  	GPIO_PinAFConfig(KEY_TX_GPIO, KEY_TX_PINSOURCE, KEY_AF);

	TIM_DeInit(KEY_TIMER);												//���½�Timer����Ϊȱʡֵ       
	TIM_InternalClockConfig(KEY_TIMER);									//�����ڲ�ʱ�Ӹ�KEY_TIMER�ṩʱ��Դ       
	TIM_TimeBaseStructure.TIM_Prescaler = 42000 - 1;					//Ԥ��Ƶϵ��Ϊ36000-1������������ʱ��Ϊ168MHz/36000 = 2kHz      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//����ʱ�ӷָ�      
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ü�����ģʽΪ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = 8 - 1;						//���ü��������С��ÿ��4�����Ͳ���һ�������¼�    
	TIM_TimeBaseInit(KEY_TIMER,&TIM_TimeBaseStructure);					//������Ӧ�õ�KEY_TIMER��      
	TIM_ClearFlag(KEY_TIMER, TIM_FLAG_Update);							//�������жϱ�־			
	TIM_ARRPreloadConfig(KEY_TIMER, DISABLE);								//��ֹARRԤװ�ػ���������Ԥװ�ػ���������ֹʱ��д���Զ�װ���ֵ(TIMx_ARR)����ֵ��ֱ�Ӵ��͵���Ӧ��Ӱ�ӼĴ��������ʹ��Ԥ���ؼĴ�������д�� ARR //����ֵ���ڸ����¼�ʱ���Ż��Ԥ���ؼĴ������͵���Ӧ��Ӱ�ӼĴ���
	TIM_ITConfig(KEY_TIMER,TIM_IT_Update,ENABLE);							//����BATCHTIMER���ж�

	//TIMER�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = KEY_TIMER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 


	//USART�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = KEY_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = 9600 ;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			
	/* Configure the USART */
	USART_Init(KEY_USART, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(KEY_USART, ENABLE);

	USART_ITConfig(KEY_USART,USART_IT_RXNE,ENABLE);

	TIM_Cmd(KEY_TIMER,ENABLE);



}






/**
 * ����������
 */
void prvKeyboardHandler(void)
{
	static WM_HWIN tFocus = 0;
	static WM_CALLBACK * tCb;
	if((_ReceivedData[0] == 0x5a) && (_ReceivedData[3] == (_ReceivedData[0] |_ReceivedData[1] |_ReceivedData[2])))
	{
		switch(_ReceivedData[1])
		{
			case	KEY_VALUE_1		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '1');
										}
										// GUI_CURSOR_Select(&GUI_CursorArrowS);
										// GUI_CURSOR_SetPosition(63, 50);
										// GUI_CURSOR_Show();
									
										// WMSETTEXT();										

										break;
			case	KEY_VALUE_2		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '2');
										}
										
										// GUI_SPRITE_Show(a);
										// GUI_CURSOR_SetPosition(165, 46);

										break;
			case	KEY_VALUE_3		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '3');
										}
										
										break;
			case	KEY_VALUE_4		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '4');
										}
										break;
			case	KEY_VALUE_5		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '5');
										}
										break;
			case	KEY_VALUE_6		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '6');
										}
										break;
			case	KEY_VALUE_7		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '7');
										}
										break;
			case	KEY_VALUE_8		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '8');
										}
										break;
			case	KEY_VALUE_9		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '9');
										}
										break;
			case	KEY_VALUE_0		:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback) 
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '0');
										}
										break;
			case	KEY_VALUE_DOT	:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if (tCb == EDIT_Callback)
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												EDIT_AddKey(tFocus, '.');
										}
										break;	
			case	KEY_VALUE_LEFT	:	
										if(!_KeyEnterLocked)
										{
											if(_ReceivedData[2] == KEY_PRESSED)
											{
												if(currentForce == NULL)
													currentForce = &button;
												else
												{
													if(currentForce->left != NULL)
													{
														currentForce = currentForce->left;
													}
												}
		  										// WM_SetFocus(currentForce->value);
												GUI_SPRITE_Delete(_CurrentFocusHandle);
												_CurrentFocusHandle = GUI_SPRITE_Create(currentForce->focusP, currentForce->x,currentForce->y);
											}
										}
										else
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												GUI_SendKeyMsg( GUI_KEY_LEFT,1);
											else
												GUI_SendKeyMsg( GUI_KEY_LEFT,0);
										}	
										break;
			case	KEY_VALUE_RIGHT	:	
										if(!_KeyEnterLocked)
										{
											if(_ReceivedData[2] == KEY_PRESSED)
											{
												if(currentForce == NULL)
													currentForce = &button;
												else
												{
													if(currentForce->right != NULL)
														currentForce = currentForce->right;
												}
		  										// WM_SetFocus(currentForce->value);
		  										GUI_SPRITE_Delete(_CurrentFocusHandle);
												_CurrentFocusHandle = GUI_SPRITE_Create(currentForce->focusP, currentForce->x,currentForce->y);
		  									}
		  								}
		  								else
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												GUI_SendKeyMsg( GUI_KEY_RIGHT,1);
											else
												GUI_SendKeyMsg( GUI_KEY_RIGHT,0);
										}
										break;

			case	KEY_VALUE_UP	:	
										if(!_KeyEnterLocked)
										{
											if(_ReceivedData[2] == KEY_PRESSED)
											{
												if(currentForce == NULL)
													currentForce = &button;
												else
												{
													if(currentForce->up != NULL)
														currentForce = currentForce->up;
												}
		  										// WM_SetFocus(currentForce->value);
												GUI_SPRITE_Delete(_CurrentFocusHandle);
												_CurrentFocusHandle = GUI_SPRITE_Create(currentForce->focusP, currentForce->x,currentForce->y);
		  									}
		  								}
		  								else
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												GUI_SendKeyMsg( GUI_KEY_UP,1);
											else
												GUI_SendKeyMsg( GUI_KEY_UP,0);
										}
										break;

			case	KEY_VALUE_DOWN	:	
										tFocus = WM_GetFocussedWindow();
										tCb = WM_GetCallback(tFocus);
										if(!_KeyEnterLocked)
										{
											if(_ReceivedData[2] == KEY_PRESSED)
											{
												if(currentForce == NULL)
													currentForce = &button;
												else
												{
													if(currentForce->down != NULL)
														currentForce = currentForce->down;
												}
		  										// WM_SetFocus(currentForce->value);
		  										
												GUI_SPRITE_Delete(_CurrentFocusHandle);
												_CurrentFocusHandle = GUI_SPRITE_Create(currentForce->focusP, currentForce->x,currentForce->y);
		  									}
		  								}
		  								else
										{
											if(_ReceivedData[2] == KEY_PRESSED)
												GUI_SendKeyMsg( GUI_KEY_DOWN,1);
											else
												GUI_SendKeyMsg( GUI_KEY_DOWN,0);
										}
										break;
			case 	KEY_VALUE_ENTER		:
											
												tFocus = WM_GetFocussedWindow();
												tCb = WM_GetCallback(tFocus);

												if((currentForce->value == button.value) ||(currentForce->value == checkbox.value))
												{
													
																	WM_SetFocus(currentForce->value );
																	if(_ReceivedData[2] == KEY_PRESSED)
																		GUI_SendKeyMsg( GUI_KEY_SPACE,1);
																	else
																		GUI_SendKeyMsg( GUI_KEY_SPACE,0);
												}
												else if(currentForce->value == dropdown.value)
												{	
																	if(_ReceivedData[2] == KEY_PRESSED)
																	{
																		_KeyEnterLocked = !_KeyEnterLocked;
																		if(_KeyEnterLocked)
																		{	
																			WM_SetFocus(currentForce->value );
																			focusChangeFrameColor(currentForce->focusP, 1);
																			GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);
																			GUI_SendKeyMsg( GUI_KEY_SPACE,1);
																		}
																		else
																		{
																			focusChangeFrameColor(currentForce->focusP, 0);
																			GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);
																			GUI_SendKeyMsg( GUI_KEY_ENTER,1);
																			focusDoLose();
																			
																		}
																	}
																	else
																	{
																		if(_KeyEnterLocked)
																		GUI_SendKeyMsg( GUI_KEY_SPACE,0);
																		else
																		GUI_SendKeyMsg( GUI_KEY_ENTER,0);
																	}
												}
												else if(currentForce->value == edit.value)
												{
																	if(_ReceivedData[2] == KEY_PRESSED)
																		{
																			_KeyEnterLocked = !_KeyEnterLocked;
																			if(_KeyEnterLocked)
																			{
																				WM_SetFocus(currentForce->value );
																				focusChangeFrameColor(currentForce->focusP, 1);
																				GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);
																			}
																			else
																			{
																				 // WM_SetFocus(0x0b);
																				focusDoLose();
																				focusChangeFrameColor(currentForce->focusP, 0);
																				GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);

																			}
																			
																		}
												}
												else if(currentForce->value == listbox.value)
												{
													if(_ReceivedData[2] == KEY_PRESSED)
													{
														
														_KeyEnterLocked = !_KeyEnterLocked;
														if(_KeyEnterLocked)
														{
															WM_SetFocus(currentForce->value );
															focusChangeFrameColor(currentForce->focusP, 1);
															GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);
														}
														else
														{
															 // WM_SetFocus(0x0b);
															focusDoLose();
															focusChangeFrameColor(currentForce->focusP, 0);
															GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);

														}
													}
													
												}
												else if(currentForce->value == listview.value)
												{
													if(_ReceivedData[2] == KEY_PRESSED)
													{
														
														_KeyEnterLocked = !_KeyEnterLocked;
														if(_KeyEnterLocked)
														{
															WM_SetFocus(currentForce->value );
															focusChangeFrameColor(currentForce->focusP, 1);
															GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);
														}
														else
														{
															focusDoLose();
															focusChangeFrameColor(currentForce->focusP, 0);
															GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);

														}
													}
													
												}
												else if(currentForce->value == radio.value)
												{
													if(_ReceivedData[2] == KEY_PRESSED)
													{
														
														_KeyEnterLocked = !_KeyEnterLocked;
														if(_KeyEnterLocked)
														{
															WM_SetFocus(currentForce->value );
															focusChangeFrameColor(currentForce->focusP, 1);
															GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);
														}
														else
														{
															focusDoLose();
															focusChangeFrameColor(currentForce->focusP, 0);
															GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);

														}
													}
													
												}
												else
												{
													if(_ReceivedData[2] == KEY_PRESSED)
													{
														
														_KeyEnterLocked = !_KeyEnterLocked;
														if(_KeyEnterLocked)
														{
															WM_SetFocus(currentForce->value );
															focusChangeFrameColor(currentForce->focusP, 1);
															GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);
														}
														else
														{
															 // WM_SetFocus(0x0b);
															focusDoLose();
															focusChangeFrameColor(currentForce->focusP, 0);
															GUI_SPRITE_SetBitmap(_CurrentFocusHandle, currentForce->focusP);

														}
													}
												}


											break;
			case 	KEY_VALUE_ESC		:
											tFocus = WM_GetFocussedWindow();
											tCb = WM_GetCallback(tFocus);
											if (tCb == EDIT_Callback) 
											{
												if(_ReceivedData[2] == KEY_PRESSED)
													EDIT_AddKey(tFocus, GUI_KEY_BACKSPACE);
											}
											break;
			case 	KEY_VALUE_EDIT		:
											break;

		}

	}
}


void KEY_USART_IRQHandler(void)
{
	uint8_t tRecive;
	if(USART_GetITStatus(KEY_USART, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(KEY_USART,USART_IT_RXNE);
		tRecive = USART_ReceiveData(KEY_USART);
		_KeyTimerCnt = 0;
		_KeyTimerCountEnable = true;
		_ReceivedData[_ReceivedDataIndex] = tRecive;
		if(_ReceivedDataIndex < 9)
			_ReceivedDataIndex++;
		else
			_ReceivedDataIndex = 0;

	}
   //�������
 	if(USART_GetFlagStatus(KEY_USART,USART_IT_ORE) != RESET)
	{
		USART_ClearFlag(KEY_USART,USART_IT_ORE);
		tRecive = USART_ReceiveData(KEY_USART);
		
	}
}

void KEY_TIMER_IRQHandler(void)
{
	//����Ƿ�����������¼�
	if(TIM_GetITStatus(KEY_TIMER, TIM_IT_Update) != RESET)
	{
		//���KEY_TIMER���жϴ�����λ
		TIM_ClearITPendingBit(KEY_TIMER , TIM_FLAG_Update);
		if(_KeyTimerCountEnable)
		{
			_KeyTimerCnt++;
			if(_KeyTimerCnt > 50)// ֡���
			{
				prvKeyboardHandler();
				_KeyTimerCnt = 0;
				_KeyTimerCountEnable = false;
				_ReceivedDataIndex = 0;
			}
		}
		
	}
}




