#include <stdbool.h>
#include <stdlib.h>
//
#include "stm32f4xx.h"
#include "Global.h"

#include "Master.h"
#include "stm32f4xx_usart.h"
//#include "CAN.h"
#define MB_NUM 41

//extern char chrYFL[8],chrCarId[8];
unsigned char u2RxdLen, flgU2StRecv;
unsigned char flgU2Stx1, flgU2Stx2;
//extern unsigned char CarId[7],PendCarId[7];
unsigned char OilCode[10];
unsigned long idOilCode;

unsigned char wrFtaDoDat;
extern unsigned char u4RxdBuf[100], u4TxdBuf[10];
extern unsigned int  u4RxdCnt;
unsigned char u4TxCnt, u4RxCnt;
unsigned char u4TxDat[20], u4RxDat[20];
unsigned char CamCard[10];
volatile unsigned char lenMbDat;
//unsigned char u5RxdBuf[100];

extern void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void TFT_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);
extern void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void delay(unsigned long nCount);
FV_MSG FV_Msg[14];
PKQ_MSG PKQ_Msg[14];
unsigned char bufMdBuf[14][100];
unsigned char u4Buf[100];
unsigned char flgPkqDatOk;
unsigned int u2Tmr, u3Tmr;
unsigned char u2RxdCnt, u2RxdBuf[20], datMod2Buf[200];
unsigned char u3RxdCnt, u3RxdBuf[20], datMod3Buf[200];
unsigned char flgMbCrcOk, flgMb2CrcOk, flgMb3CrcOk;
//unsigned int  mbdata[14][50];
unsigned int mbDat[100];

unsigned char flgIcDat1OK = 0;

/*

*/
/*
17,72,73,74,75,76,80,81,82,83,84,85,86,159,160,161,162,163,164,165,166,199,200,201,202,245,246,247,248,249,250,251,252,259,260,261,262,279,280,293,294,
*/
//unsigned int  mbpos[43]={ 17, 72, 73, 74, 75, 76, 80, 81, 82, 83,
//												  84, 85, 86,159,160,161,162,163,164,165,
//												 166,199,200,201,202,245,246,247,248,249,
//                         250,251,252,259,260,261,262,279,280,293,
//                         294,54, 55};
unsigned char Txd2Buf[100], Txd3Buf[100];

unsigned char Rxd2Buf[30], u2MbBuf[100];
unsigned char u2Cnt, u2SndCnt, u2Flag; //u2Cnt:????,u2Flag:????
unsigned char Rxd3Buf[30], u3MbBuf[100];
unsigned char u3Cnt, u3SndCnt, u3Flag; //u2Cnt:????,u2Flag:????
unsigned char Rxd4Buf[30], u4MbBuf[100];
unsigned char u4Cnt, u4SndCnt, u4Flag; //u2Cnt:????,u2Flag:????





void Dis_Pad(unsigned char tCar)
{
    char HZ[60];//="ÂÜÄË±´‰∫ëËæΩÈªëÊπòÁöñÈ≤ÅÊñ∞ËãèÊµôËµ£ÈÑÇÊ°ÇÁîòÊôãËíôÈôïÂêâÈóΩÁ≤§Â∑ùÈùíËóèÁêºÂÆÅÊ∏ù‰∫¨Ê¥•Ê≤™";
    char ZM[52];//="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char SZ[20];//="0123456789";
    unsigned char ti;
    char ttBuf[8];
    char PadCode[8];
    char tBuf[3];
    unsigned int hzCode;
    //char HZ[60]="ÂÜÄË±´‰∫ëËæΩÈªëÊπòÁöñÈ≤ÅÊñ∞ËãèÊµôËµ£ÈÑÇÊ°ÇÁîòÊôãËíôÈôïÂêâÈóΩÁ≤§Â∑ùÈùíËóèÁêºÂÆÅÊ∏ù‰∫¨Ê¥•Ê≤™";
    if((CarId[0] == 0) && (CarId[1] == 0) && (CarId[2] == 0) && (CarId[3] == 0) && (CarId[4] == 0) && (CarId[5] == 0) && (CarId[6] == 0))
        tCar = 0;
    if(tCar)
    {
        for(ti = 0; ti < 8; ti++)
            PadCode[ti] = CarId[ti];
        hzCode = (PadCode[0] << 8) + PadCode[1];
        for(ti = 2; ti < 8; ti++)
            ttBuf[ti - 2] = PadCode[ti];
        ttBuf[7] = 0;
        lcd_text24(210 + 24, 214, WHITE, BLUE, ttBuf);
//??????????
//??????????
//??????????

        switch(hzCode)
        {
        case  0xBCBD:
            lcd_text24(210, 214, WHITE, BLUE, "ºΩ");
            break;
        case  0xD4A5:
            lcd_text24(210, 214, WHITE, BLUE, "‘•");
            break;
        case  0xD4C6:
            lcd_text24(210, 214, WHITE, BLUE, "‘∆");
            break;
        case  0xC1C9:
            lcd_text24(210, 214, WHITE, BLUE, "¡…");
            break;
        case  0xBADA:
            lcd_text24(210, 214, WHITE, BLUE, "∫⁄");
            break;
        case  0xCFE6:
            lcd_text24(210, 214, WHITE, BLUE, "œÊ");
            break;
        case  0xCDEE:
            lcd_text24(210, 214, WHITE, BLUE, "ÕÓ");
            break;
        case  0xC2B3:
            lcd_text24(210, 214, WHITE, BLUE, "¬≥");
            break;
        case  0xD0C2:
            lcd_text24(210, 214, WHITE, BLUE, "–¬");
            break;
        case  0xCBD5:
            lcd_text24(210, 214, WHITE, BLUE, "À’");
            break;
        case  0xD5E3:
            lcd_text24(210, 214, WHITE, BLUE, "’„");
            break;
        case  0xB8D3:
            lcd_text24(210, 214, WHITE, BLUE, "∏”");
            break;
        case  0xB6F5:
            lcd_text24(210, 214, WHITE, BLUE, "∂ı");
            break;
        case  0xB9F0:
            lcd_text24(210, 214, WHITE, BLUE, "π");
            break;
        case  0xB8CA:
            lcd_text24(210, 214, WHITE, BLUE, "∏ ");
            break;
        case  0xBDFA:
            lcd_text24(210, 214, WHITE, BLUE, "Ω˙");
            break;
        case  0xC3C9:
            lcd_text24(210, 214, WHITE, BLUE, "√…");
            break;
        case  0xC9C2:
            lcd_text24(210, 214, WHITE, BLUE, "…¬");
            break;
        case  0xBCAA:
            lcd_text24(210, 214, WHITE, BLUE, "º™");
            break;
        case  0xC3F6:
            lcd_text24(210, 214, WHITE, BLUE, "√ˆ");
            break;
        case  0xD4C1:
            lcd_text24(210, 214, WHITE, BLUE, "‘¡");
            break;
        case  0xB4A8:
            lcd_text24(210, 214, WHITE, BLUE, "¥®");
            break;
        case  0xC7E0:
            lcd_text24(210, 214, WHITE, BLUE, "«‡");
            break;
        case  0xB2D8:
            lcd_text24(210, 214, WHITE, BLUE, "≤ÿ");
            break;
        case  0xC7ED:
            lcd_text24(210, 214, WHITE, BLUE, "«Ì");
            break;
        case  0xC4FE:
            lcd_text24(210, 214, WHITE, BLUE, "ƒ˛");
            break;
        case  0xD3E5:
            lcd_text24(210, 214, WHITE, BLUE, "”Â");
            break;
        case  0xBEA9:
            lcd_text24(210, 214, WHITE, BLUE, "æ©");
            break;
        case  0xBDF2:
            lcd_text24(210, 214, WHITE, BLUE, "ΩÚ");
            break;
        case  0xBBA6:
            lcd_text24(210, 214, WHITE, BLUE, "ª¶");
            break;
        }
        if(0)
        {
            ttBuf[0] = HZ[PadCode[0] * 2];
            ttBuf[1] = HZ[PadCode[0] * 2 + 1];
            if((PadCode[1] > 0x40) && (PadCode[1] < 0x5B))
            {
                ttBuf[2] = ZM[(PadCode[1] - 0x41) * 2];
                ttBuf[3] = ZM[(PadCode[1] - 0x41) * 2 + 1];
                if((PadCode[2] > 0x40) && (PadCode[2] < 0x5B))
                {
                    ttBuf[4] = ZM[(PadCode[2] - 0x41) * 2];
                    ttBuf[5] = ZM[(PadCode[2] - 0x41) * 2 + 1];
                }
                else if((PadCode[2] >= 0x30) && (PadCode[2] <= 0x39))
                {
                    ttBuf[4] = SZ[(PadCode[2] - 0x30) * 2];
                    ttBuf[5] = SZ[(PadCode[2] - 0x30) * 2 + 1];
                }

                if((PadCode[3] > 0x40) && (PadCode[3] < 0x5B))
                {
                    ttBuf[6] = ZM[(PadCode[3] - 0x41) * 2];
                    ttBuf[7] = ZM[(PadCode[3] - 0x41) * 2 + 1];
                }
                else if((PadCode[3] >= 0x30) && (PadCode[3] <= 0x39))
                {
                    ttBuf[6] = SZ[(PadCode[3] - 0x30) * 2];
                    ttBuf[7] = SZ[(PadCode[3] - 0x30) * 2 + 1];
                }

                if((PadCode[4] > 0x40) && (PadCode[4] < 0x5B))
                {
                    ttBuf[8] = ZM[(PadCode[4] - 0x41) * 2];
                    ttBuf[9] = ZM[(PadCode[4] - 0x41) * 2 + 1];
                }
                else if((PadCode[4] >= 0x30) && (PadCode[4] <= 0x39))
                {
                    ttBuf[8] = SZ[(PadCode[4] - 0x30) * 2];
                    ttBuf[9] = SZ[(PadCode[4] - 0x30) * 2 + 1];
                }

                if((PadCode[5] > 0x40) && (PadCode[5] < 0x5B))
                {
                    ttBuf[10] = ZM[(PadCode[5] - 0x41) * 2];
                    ttBuf[11] = ZM[(PadCode[5] - 0x41) * 2 + 1];
                }
                else if((PadCode[5] >= 0x30) && (PadCode[5] <= 0x39))
                {
                    ttBuf[10] = SZ[(PadCode[5] - 0x30) * 2];
                    ttBuf[11] = SZ[(PadCode[5] - 0x30) * 2 + 1];
                }

                if((PadCode[6] > 0x40) && (PadCode[6] < 0x5B))
                {
                    ttBuf[12] = ZM[(PadCode[6] - 0x41) * 2];
                    ttBuf[13] = ZM[(PadCode[6] - 0x41) * 2 + 1];
                }
                else if((PadCode[6] >= 0x30) && (PadCode[6] <= 0x39))
                {
                    ttBuf[12] = SZ[(PadCode[6] - 0x30) * 2];
                    ttBuf[13] = SZ[(PadCode[6] - 0x30) * 2 + 1];
                }
            }
            ttBuf[14] = 0;
            lcd_text24(72 - 15, 380 - 12, 0xFFFF, 0xFFFF, ttBuf);
//	if(frmNum==FRM_MAIN)
//	{
//		lcd_text24(15,380-26,0x001F,0x001F,"?");
//		lcd_text24(15,380+2,0x001F,0x001F,"?");
//		Draw_Rect(45,358,192,44,3);
//		TFT_Fill(45,358,272-35,404,0x001F);
//	}
        }
    }
    else
    {
        //lcd_text24(30,380-12,0x001F,0x001F,"??");
//	if(frmNum==FRM_MAIN)
//	{
//		lcd_text24(15,380-26,0x001F,0x001F,"?");
//		lcd_text24(15,380+2,0x001F,0x001F,"?");
//		Draw_Rect(45,358,192,44,3);
//		TFT_Fill(45,358,272-35,404,0x001F);
//	}
    }
}

void U2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

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

    //USART÷–∂œ≈‰÷√
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
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



void U3_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    /* Configure USART Tx  as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure USART Rx  as input floating */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    //USART÷–∂œ≈‰÷√
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* Configure the USART */
    USART_Init(USART3, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(USART3, ENABLE);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    GPIO_SetBits(GPIOC, GPIO_Pin_4); //◊º±∏∑¢ÀÕ∏¯FTA
}
void U4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

    /* Configure USART Tx  as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART Rx  as input floating */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);


    //USART÷–∂œ≈‰÷√
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* Configure the USART */
    USART_Init(UART4, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(UART4, ENABLE);

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    GPIO_ResetBits(GPIOE, GPIO_Pin_2); //U4◊º±∏Ω” ‹PC÷∏¡Ó

}
void U5_Init_Cust(uint16_t bps, uint8_t bitLen, uint8_t bitStop, uint8_t bitParity)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure USART Rx  as input floating */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configure USART Tx  as alternate function push-pull */


    //USART÷–∂œ≈‰÷√
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = bps ;
//	if(bitParity>1)//”––£—È
//	{
//
//
//	}
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    //USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Mode = USART_Mode_Rx;
    /* Configure the USART */
    USART_Init(UART5, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(UART5, ENABLE);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    GPIO_ResetBits(GPIOD, GPIO_Pin_6);
}
void U5_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure USART Rx  as input floating */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configure USART Tx  as alternate function push-pull */


    //USART÷–∂œ≈‰÷√
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600 ;//1200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    //USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Mode = USART_Mode_Rx;
    /* Configure the USART */
    USART_Init(UART5, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(UART5, ENABLE);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    GPIO_ResetBits(GPIOD, GPIO_Pin_6);

}

unsigned char function;
unsigned char chk_U2Sqr(void)
{
    unsigned int address;
    unsigned int data;
    unsigned int i, j, length;
    unsigned int tCrc;
    unsigned char crch, crcl;
    unsigned char tLen;
    unsigned int  txdLen;
    unsigned char sl_id;
    unsigned char thBuf[100];
    for(i = 0; i < 20; i++)
        thBuf[i] = datMod2Buf[i];
    function = thBuf[1];
    sl_id = thBuf[0];
//	if(pkqDisUnit[sl_id-1].cntConn==1)
//		return 20;
//	if(sl_id!=sysSavDat[4])//µÿ÷∑≤ªÕ¨
//	{	}
    if((function != 3) && (function != 4) && (function != 6))
        return 20;
    if((function == 3) || (function == 4))
        tLen = 6;
    if(function == 6)
        tLen = 6;
    tCrc = CRC16(thBuf, tLen);
    crch = (tCrc & 0xff);
    crcl =	(tCrc & 0xFF00) >> 8;
    if((thBuf[tLen] != crch) || (thBuf[tLen + 1] != crcl))
    {
        return 2;
    }
    if (function == 0x03)
    {   //03 Read Holding Registers
        address = (thBuf[2] << 8)	+ thBuf[3];
        //if((address<39)||(address>98))//??141
        //	return 3;
        //address -=39;
        length = (thBuf[4] << 8) + thBuf[5];
        for(i = 0; i < 100; i++)
            Txd2Buf[i] = 0;
        Txd2Buf[0] = sl_id;
        Txd2Buf[1] = 0x03;
        txdLen = length * 2;
        Txd2Buf[2] = (unsigned char) txdLen;
        for (i = 0; i < length; i++)
        {
            for(j = 0; j < 43; j++)
            {
                if((address + i + 1) < 9)
                {
                    Txd2Buf[3 + i * 2] = (unsigned char) (mbDat[i] >> 8);
                    Txd2Buf[4 + i * 2] = (unsigned char) (mbDat[i] & 0xff);
                }
            }
        }
        txdLen = txdLen + 3;
        tCrc = CRC16(Txd2Buf, txdLen);
        crch = (tCrc & 0xff);
        crcl =	(tCrc & 0xFF00) >> 8;
        Txd2Buf[txdLen] = crch;
        txdLen++;
        Txd2Buf[txdLen] = crcl;
        txdLen++;
        Txd2Buf[txdLen] = 0;
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
        delay(10);
        for(i = 0; i <= txdLen; i++)
        {
            u2MbBuf[i] = Txd2Buf[i];
        }
        u2Cnt = 0;
        u2SndCnt = txdLen;
        u2Flag = 1;

        return 0;
    }
    else if (function == 0x06)
    {   //06 Preset Single Register
        address = (thBuf[2] << 8)	+ thBuf[3];
        if(address != 3)
        {
            return 4;
        }
        data = (thBuf[4] << 8) + thBuf[5];
        mbDat[3] = data;
        wrFtaDoDat = data;
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
        delay(10);
        thBuf[8] = 0;
        //?????,??CRC??
        for(i = 0; i < 8; i++)
        {
            u2MbBuf[i] = thBuf[i];
        }
        u2Cnt = 0;
        u2SndCnt = 8;
        u2Flag = 1;
        return 0;
    }
}
unsigned char Func;
/*unsigned char chk_U3Sqr(void)
{
	unsigned int address;
	unsigned int data;
	unsigned int i,j, length;
	unsigned int tCrc;
	unsigned char crch,crcl;
	unsigned char tLen;
	unsigned int  txdLen;
	unsigned char sl_id;
	unsigned char thBuf[100];
	for(i=0;i<20;i++)
		thBuf[i]=datMod3Buf[i];
	Func = thBuf[1];
	sl_id=thBuf[0];
//	if(pkqDisUnit[sl_id-1].cntConn==1)
//		return 20;
	if((Func!=3)&&(Func!=6)&&(Func!=7)&&(Func!=16))
		return 20;
	if(Func==3)
		tLen=6;
	if(Func==6)
		tLen=6;
	if(Func==7)	//???,??ESD,???
		tLen=6;
	if(Func==16)
		tLen=11;
	tCrc=CRC16(thBuf,tLen);
	crch= (tCrc & 0xff);
	crcl=	(tCrc & 0xFF00)>>8;
  if((thBuf[tLen]!=crch)||(thBuf[tLen+1]!=crcl))
	{
	 return 2;
	}
  if (Func == 0x03)
	{		  //03 Read Holding Registers
		address = (thBuf[2] << 8)	+ thBuf[3];
		//if((address<39)||(address>98))//??141
		//	return 3;
		//address -=39;
		length = (thBuf[4] << 8) + thBuf[5];
		for(i=0;i<100;i++)
			Txd3Buf[i]=0;
		Txd3Buf[0]=sl_id;
		Txd3Buf[1]=0x03;
		txdLen=length*2;
		Txd3Buf[2] = (unsigned char) txdLen;
			for (i=0; i<length; i++)
			{
				for(j=0;j<43;j++)
				{
					if((address+i+1)==mbpos[j])
					{
					Txd3Buf[3+i*2] = (unsigned char) (mbdata[sl_id-1][j] >> 8);
					Txd3Buf[4+i*2] = (unsigned char) (mbdata[sl_id-1][j] & 0xff);
					}
				}
			}
		txdLen=txdLen + 3;
		tCrc=CRC16(Txd3Buf,txdLen);
		crch= (tCrc & 0xff);
		crcl=	(tCrc & 0xFF00)>>8;
		Txd3Buf[txdLen]=crch;
		txdLen++;
		Txd3Buf[txdLen]=crcl;
		txdLen++;
		Txd3Buf[txdLen]=0;
		GPIO_SetBits(GPIOC,GPIO_Pin_4);
		delay(10);

		//?????,??CRC??
		for(i=0; i<=txdLen; i++)
		{
			u3MbBuf[i]=Txd3Buf[i];
		}
		u3Cnt=0;u3SndCnt=txdLen;u3Flag=1;
		//SetU2Recv();
	return 0;
	}

}
*/

void SendU2Dat(unsigned int u2Dat)
{
    USART_SendData(USART2, u2Dat);
}
void SendU3Dat(unsigned int u3Dat)
{
    USART_SendData(USART3, u3Dat);
}
void SendU4Dat(unsigned int u4Dat)
{
    USART_SendData(UART4, u4Dat);
}

float CalIEEE754(u8 dt1, u8 dt2, u8 dt3, u8 dt4, u8 tFlg)
{
    float tta;
    float lmtMin = 0.0, lmtMax = 400.0;
    u8 i, *px;
    u8 x[4];
    void *pf;
    x[0] = dt4;
    x[1] = dt3;
    x[2] = dt2;
    x[3] = dt1;

    px = x;           //px÷∏’Î÷∏œÚ ˝◊Èx
    pf = &tta;
    for(i = 0; i < 4; i++)
        *((char *)pf + i) = *(px + i);
    switch(tFlg)
    {
    case 1://À≤ ±¡ø
        lmtMin = 0.01;
        lmtMax = 400.0;
        break;
    case 2://¿€ª˝ ˝
        lmtMin = 0.01;
        lmtMax = 999999999.9;
        break;
    case 3://√‹∂»
        lmtMin = 0.00001;
        lmtMax = 4.0;
        break;
    case 4://ΩÈ÷ Œ¬∂»
        lmtMin = -40.0;
        lmtMax = 200.0;
        break;
    case 5://¿€ª˝ÃÂª˝
        lmtMin = 0.01;
        lmtMax = 999999999.9;
        break;
    }
    if(tta < lmtMin)
    {
        if(tFlg != 4)
            tta = 0;
        else
            tta = lmtMin;
    }
    if(tta > lmtMax)
        tta = lmtMax;
    return tta;
}

//extern PKQ_DIS  pkqDisUnit[14];//14◊È≈˙øÿ∆˜
extern unsigned char frmNum;
extern void Draw_Pkq_STA(unsigned char id, unsigned char sta);
extern void Draw_Cell_Conn(unsigned char id, unsigned char sta);


extern void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);

unsigned char u4STX, u4Dat, cntKK = 0;
unsigned char flgU4Stx1, flgU4Stx2, flgU4StRecv, flgU4StSend = 0, flgU4OK;
//BB 88 AA 00 12 41 52 35 36 37 38 00 00 00 A
void UART4_IRQHandler(void)//≥µ≈∆ ∂±
{
    char disBuf[3];
    unsigned char u4, i, tXorDat;
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        USART_ClearFlag(UART4, USART_IT_RXNE);
        u4Dat = USART_ReceiveData(UART4);

        if(flgU4StRecv == 1)
        {
            u4RxDat[u4RxCnt] = u4Dat;
            u4RxCnt++;
            if(u4RxCnt > 14)
            {
                flgU4StRecv = 0;
                if(u4RxDat[2] == 0xAA) // ∂± ˝æ›”––ß
                {
                    tXorDat = u4RxDat[2];
                    for(i = 3; i < 14; i++)
                        tXorDat = tXorDat ^ u4RxDat[i];
                    if(tXorDat == u4RxDat[14])
                    {
                        for(i = 4; i < 11; i++)
                            CamCard[i - 4] = u4RxDat[i];
                        mbDat[18] = (CamCard[0] << 8) + CamCard[1];
                        mbDat[19] = (CamCard[2] << 8) + CamCard[3];
                        mbDat[20] = (CamCard[4] << 8) + CamCard[5];
                        mbDat[21] = (CamCard[6] << 8) + CamCard[7];
                        flgU4OK = 1;
                    }
                }
            }
        }
        if(flgU4StRecv == 0)
        {
            if((flgU4Stx2 == 0) && (flgU4Stx1 == 1) && (u4Dat == 0x88))
            {
                flgU4Stx2 = 1;
                flgU4StRecv = 1;
                u4RxCnt = 2;	//u4RxdLen=13;
            }
            else
            {
                flgU4Stx1 = 0;
                flgU4Stx2 = 0;
                flgU4StRecv = 0;
            }
            if(u4Dat == 0xBB) //STX
            {
                flgU4Stx1 = 1;
                flgU4Stx2 = 0;
                u4STX = 1;
                u4RxCnt = 0;
                for(u4 = 0; u4 < 10; u4++)
                {
                    u4RxDat[u4] = 0;
                }
            }
        }
    }
    //“Á≥ˆ¥¶¿Ì
    if(USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)
    {
        USART_ClearFlag(UART4, USART_FLAG_ORE);
        USART_ReceiveData(UART4);

    }
}



//≥∆÷ÿ»Ìº˛
void UART4_IRQHandler_(void)
{
    char disBuf[3];
    unsigned char u4, i;
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        USART_ClearFlag(UART4, USART_IT_RXNE);
        u4Dat = USART_ReceiveData(UART4);

        if(flgU4StRecv == 1)
        {
            u4RxDat[u4RxCnt] = u4Dat;
            u4RxCnt++;
            if(u4RxCnt >= 8)
            {
                flgU4StRecv = 0;
                if((u4RxDat[2] == 0x01) && (u4RxDat[3] == 0x01) && ((u4RxDat[5] == 0x40) || (u4RxDat[5] == 0x41)))
                {
                    if(u4RxDat[5] == 0x40)
                        flgU4StSend = 1;
                    if(u4RxDat[5] == 0x41)
                    {
                        flgU4StSend = 2;
                        wrFtaDoDat = u4RxDat[6];
                    }
                }
            }
        }
        if(flgU4StRecv == 0)
        {
            if((flgU4Stx2 == 0) && (flgU4Stx1 == 1) && (u4Dat == 0xCC))
            {
                flgU4Stx2 = 1;
                flgU4StRecv = 1;
                u4RxCnt = 2;	//u4RxdLen=13;
            }
            else
            {
                flgU4Stx1 = 0;
                flgU4Stx2 = 0;
                flgU4StRecv = 0;
            }
            if(u4Dat == 0xFF) //STX
            {
                flgU4Stx1 = 1;
                flgU4Stx2 = 0;
                u4STX = 1;
                u4RxCnt = 0;
                for(u4 = 0; u4 < 10; u4++)
                {
                    u4RxDat[u4] = 0;
                }
            }
        }
    }
    //“Á≥ˆ¥¶¿Ì
    if(USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)
    {
        USART_ClearFlag(UART4, USART_FLAG_ORE);
        USART_ReceiveData(UART4);

    }
}
char WeiDat[7], disWeiDat[7];
unsigned long lngWeiDat;

void CalWei(void)
{
    unsigned char i, flgDatOk = 1;
    for(i = 0; i < 6; i++)
    {
        if(((WeiDat[i] < '0') || (WeiDat[i] > '9')) && (WeiDat[i] != 0x20))
        {
            WeiDat[i] = 0;
            flgDatOk = 0;
            break;
        }
        else
        {
            if(WeiDat[i] == 0x20)
            {
                WeiDat[i] = 0;
            }
            else
                WeiDat[i] -= 0x30;
        }
    }
    if(flgDatOk == 1)
    {
        lngWeiDat = WeiDat[0] * 100000;
        lngWeiDat += WeiDat[1] * 10000;
        lngWeiDat += WeiDat[2] * 1000;
        lngWeiDat += WeiDat[3] * 100;
        lngWeiDat += WeiDat[4] * 10;
        lngWeiDat += WeiDat[5] * 1;
        mbDat[12] = lngWeiDat & 0xFFFF;
        mbDat[13] = lngWeiDat >> 16;
        mbDat[11] = 0x0055;
    } else
    {
        mbDat[11] = 0x0000;
    }
}

void DisCamCard(void)
{
    unsigned char i;
    char HZ[72] = "æ©ΩÚºΩΩ˙√…¡…º™∫⁄ª¶À’’„ÕÓ√ˆ∏”¬≥‘•∂ıœÊ‘¡π«Ì”Â¥®πÛ‘∆≤ÿ…¬∏ «‡ƒ˛–¬∏€∞ƒÃ®æØ π"; //
    char HzBuf[20];
    HzBuf[0] = HZ[CamCard[0] * 2];
    HzBuf[1] = HZ[CamCard[0] * 2 + 1];
    for(i = 1; i < 7; i++)
        HzBuf[1 + i] = CamCard[i];
    HzBuf[8] = 0;
    lcd_text24(210, 314, WHITE, BLUE, HzBuf);
}
extern unsigned char sysSavDat[10];
//UART5 ≥”÷ÿ¥´∏–∆˜
void UART5_IRQHandler(void)
{
    uint8_t u5, i, u5Dat;
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        USART_ClearFlag(UART5, USART_IT_RXNE);
        u5Dat = USART_ReceiveData(UART5);
        u5 = u5Dat & 0x7F;
        if(u5STX == 1)
        {
            u5RxdBuf[u5Cnt] = u5;
            u5Cnt++;
            if((u5 == 0x0D) && (u5Cnt == 0x11))
            {
                u5STX = 0;
                for(i = 4; i < 10; i++)
                {
                    WeiDat[i - 4] = u5RxdBuf[i];
                    disWeiDat[i - 4] = u5RxdBuf[i];
                }
                flgU5Stx = 1;
            }
            if(u5Cnt > 0x20)
            {
                u5STX = 0;
                u5Cnt = 0;
            }
        }
        if(u5 == 0x02)
        {
            u5STX = 1;
            for(i = 0; i < 100; i++)
            {
                u5RxdBuf[i] = 0;
            }
            u5Cnt = 1;
            u5RxdBuf[0] = 2;
        }
    }
    //“Á≥ˆ¥¶¿Ì
    if(USART_GetFlagStatus(UART5, USART_IT_ORE) != RESET)
    {
        USART_ClearFlag(UART5, USART_IT_ORE);
        u5 = USART_ReceiveData(UART5);
    }
}

//void USART2_IRQHandler(void)
//{
//    u16 u;
//    u8 i;
//    u8 j;
//    u8 flgEnDat;
//    u8 tDat;//flgDat1Ok=0,flgDat2Ok=0,
//    u8 tBuf[37];
//    u32 tLngDat;
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//    {
//        u = USART_ReceiveData(USART2);
//        USART_ClearFlag(USART2, USART_IT_RXNE);
//        if(flgU2StRecv == 1)
//        {
//            u2RxdBuf[u2RxdCnt] = u;
//            u2RxdCnt++;
//            //if(u==0x27)
//            //  u5RxdLen=37;//?????
//            switch(u2RxdBuf[3])
//            {
//            case 0x69:
//                u2RxdLen = 20; //???21,//???16??,?2??????????????,??2??
//                break;
//            case 0x66:
//                u2RxdLen = 15; ////?6?0?4????,10byte,15=10+5
//                break;
//            case 0x67:
//                u2RxdLen = 13; ////????,8byte13=8+5
//                break;
//            default:
//                u2RxdLen = 13;
//                break;
//            }
//            if(u2RxdCnt >= u2RxdLen) //13
//            {
//                //FF CC 01 57 38 31 30 36 31 38 9C
//                //ff cc 01 67 31 32 33 34 35 36 37 36 AC

//                if(u2RxdBuf[3] == 0x69) //????:
//                {
//                    if(flgIcDat1OK == 1)
//                    {
//                        tDat = u2RxdBuf[13] - '0';
//                        //CarId[0]=u2RxdBuf[6];//-'0';
//                        for(i = 0; i < 8; i++)
//                            CarId[i] = u2RxdBuf[6 + i];
//                        Dis_Pad(1);
//                        mbDat[9] = 0x55;
//                        flgIcDat1OK = 0;
//                    }
//                    else
//                    {
//                        mbDat[9] = 0x00;
//                    }
//                }
//                if(u2RxdBuf[3] == 0x67) //Ã·”Õ√‹¬Î
//                {

//                    if(((u2RxdBuf[4] & 0xF0) == 0x30) && ((u2RxdBuf[5] & 0xF0) == 0x30) && ((u2RxdBuf[6] & 0xF0) == 0x30) &&
//                            ((u2RxdBuf[7] & 0xF0) == 0x30) && ((u2RxdBuf[8] & 0xF0) == 0x30) && ((u2RxdBuf[9] & 0xF0) == 0x30) &&
//                            ((u2RxdBuf[10] & 0xF0) == 0x30) && ((u2RxdBuf[11] & 0xF0) == 0x30))
//                    {
//                        for(i = 0; i < 8; i++)
//                        {   OilCode[i] = u2RxdBuf[i + 4];
//                            tBuf[i] = OilCode[i];
//                        }
//                        tBuf[8] = 0;
//                        lcd_text24(210, 244, WHITE, BLUE, tBuf);
//                        tLngDat = 0;
//                        for(i = 0; i < 8; i++)
//                        {
//                            tLngDat = tLngDat + OilCode[i] - '0';
//                            tLngDat = tLngDat * 10;
//                        }
//                        tLngDat = tLngDat / 10;
//                        mbDat[24] = tLngDat & 0xFFFF;
//                        mbDat[25] = tLngDat >> 16;
//                        //mbDat[24]=tLngDat
//                        flgIcDat1OK = 1;

//                    }
//                }
//                //if(u5RxdBuf[3]==0x66)//???
//                //{
//                //if(frmNum==FRM_LOGIN)
//                //{
//                //	flgEnDat=1;
//                //	for(i=4;i<10;i++)
//                //	{
//                //		if(u5RxdBuf[i]!=0x30)
//                //			flgEnDat=0;
//                //	}
//                //	if(flgEnDat==1)
//                //	{
//                //	if(((u5RxdBuf[10]&0xF0)==0x30)&&((u5RxdBuf[11]&0xF0)==0x30)&&
//                //		 ((u5RxdBuf[12]&0xF0)==0x30)&&((u5RxdBuf[13]&0xF0)==0x30))
//                //	 {
//                //		 for(i=10;i<14;i++)
//                //		 {
//                //			CardUsr[i-10]=u5RxdBuf[i];
//                //		 }
//                //		 CardUsr[4]=0;CardUsr[5]=0xFA;
//                //	 }
//                //	}
//                //	//ff cc 01 67 30 30 30 30 30 30 3x 3x 3x 3x AC
//                //}
//                //}
//                flgU2Stx1 = 0;
//                flgU2Stx2 = 0;
//                flgU2StRecv = 0;
//                u2RxdCnt = 0;
//            }
//        }
//        if(flgU2StRecv == 0)
//        {
//            if((flgU2Stx2 == 0) && (flgU2Stx1 == 1) && (u == 0xCC))
//            {
//                flgU2Stx2 = 1;
//                flgU2StRecv = 1;
//                u2RxdCnt = 2;
//                u2RxdLen = 13;
//                u2RxdBuf[0] = 0xFF;
//                u2RxdBuf[1] = 0xCC;
//            }
//            else
//            {
//                flgU2Stx1 = 0;
//                flgU2Stx2 = 0;
//                flgU2StRecv = 0;
//            }
//            if(u == 0xFF)
//            {
//                flgU2Stx1 = 1;
//                flgU2Stx2 = 0;
//                flgU2StRecv = 0;
//            }
//        }

//    }
//}

//USART3 FTA
unsigned char u3STX, u3Cnt;
unsigned int  ftaDIdat, ftaDIdat_pre;
unsigned char ftaDOdat, ftaDOdat_pre;
unsigned char ftaDi[12], ftaDo[8];
void USART3_IRQHandler(void)
{
    vu32 tdelay1;
    vu8 u3, u3i;
    vu16 tdat;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearFlag(USART3, USART_IT_RXNE);
        u3 = USART_ReceiveData(USART3);
        if(u3STX == 1)
        {
            u3RxdBuf[u3Cnt] = u3;
            u3Cnt++;
            if((u3 == 0x4B) || (u3Cnt > 18))
            {
                u3STX = 0;
                if(u3 == 0x4B)
                {
                    if(((u3RxdBuf[1] & 0xF0) == 0x30) && (((u3RxdBuf[2] & 0xF0) == 0x30)) && (((u3RxdBuf[3] & 0xF0) == 0x30)))
                    {
                        ftaDIdat = (u3RxdBuf[1] & 0x0F);
                        ftaDIdat = ftaDIdat << 4;
                        ftaDIdat += (u3RxdBuf[2] & 0x0F);
                        ftaDIdat = ftaDIdat << 4;
                        ftaDIdat += (u3RxdBuf[3] & 0x0F);
                        mbDat[2] = ftaDIdat;
                    }
                    if(((u3RxdBuf[4] & 0xF0) == 0x30) && (((u3RxdBuf[5] & 0xF0) == 0x30)))
                    {
                        ftaDOdat = (u3RxdBuf[4] & 0x0F);
                        ftaDOdat = ftaDOdat << 4;
                        ftaDOdat += (u3RxdBuf[5] & 0x0F);
                        mbDat[3] = ftaDOdat;
                        if(ftaDOdat > 0)
                            mbDat[4] = 0x55;
                        else
                            mbDat[4] = 0xAA;
                    }
                    u3Cnt = 0;
                }
            }
        }
        if(u3 == 0x4A)
        {
            u3STX = 1;
            u3Cnt = 1;
            u3RxdBuf[0] = 0x4A;
        }
    }
    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET)
    {
        USART_ClearFlag(USART3, USART_FLAG_ORE);
        USART_ReceiveData(USART3);
    }
}
unsigned char u3TxDat[50];
void SendFTA(unsigned char ftaDat)
{
    unsigned char i;
    //GPIO_SetBits(GPIOC,GPIO_Pin_4);//U3
    u3TxDat[0] = 0x4A;
    u3TxDat[1] = 0x30 + (ftaDat >> 4);
    u3TxDat[2] = 0x30 + (ftaDat & 0x0F);
    u3TxDat[3] = 0x4B;
    u3TxDat[4] = 0;
    GPIO_SetBits(GPIOC, GPIO_Pin_4); //U3  ADM2587E ∑¢ÀÕŒ™1£¨RSM3485 ∑¢ÀÕŒ™0
    //GPIO_ResetBits(GPIOE,GPIO_Pin_2);//U4
    delay(0x2F0);
    for(i = 0; i < 4; i++)
    {
        //u4MbBuf[i]=u3TxDat[i];
        USART_SendData(USART3, u3TxDat[i]);
        delay(0x2FFF);
    }
    //delay(0x2F0);
    //u4Cnt=0;u4SndCnt=5;u4Flag=1;
    GPIO_ResetBits(GPIOC, GPIO_Pin_4); //U3
    //GPIO_ResetBits(GPIOE,GPIO_Pin_2);//U4
    //GPIO_SetBits(GPIOE,GPIO_Pin_2);//U4
    //delay(0x2FF);

}
void FbRoadGate(void)
{
    unsigned long tLngDat;
    unsigned char tDat[8], i;
    unsigned char WeiDatBCD[4], DiDat, DoDat, CrcDat;
    u4MbBuf[0] = 0xFF;
    u4MbBuf[1] = 0xCC;
    u4MbBuf[2] = 0x01;
    u4MbBuf[3] = 0x01;
    u4MbBuf[4] = 0x03;
    u4MbBuf[5] = 0x60;
    u4MbBuf[6] = 0x41;
    u4MbBuf[7] = 0x55;
    CrcDat = 0;
    for(i = 2; i < 8; i++)
    {
        CrcDat = (CrcDat + u4MbBuf[i]) & 0xFF;
    }
    u4MbBuf[8] = CrcDat;
    GPIO_SetBits(GPIOE, GPIO_Pin_2);
    u4Cnt = 0;
    u4SndCnt = 9;
    u4Flag = 1;
}
void FbPatrol(void)
{
    unsigned long tLngDat;
    unsigned char tDat[8], i;
    unsigned char WeiDatBCD[4], DiDat, DoDat, CrcDat;
    tLngDat = lngWeiDat * 10;
    tDat[0] = 0;
    tDat[1] = tLngDat / 1000000;
    tLngDat = tLngDat % 1000000;
    tDat[2] = tLngDat / 100000;
    tLngDat = tLngDat % 100000;
    tDat[3] = tLngDat / 10000;
    tLngDat = tLngDat % 10000;
    tDat[4] = tLngDat / 1000;
    tLngDat = tLngDat % 1000;
    tDat[5] = tLngDat / 100;
    tLngDat = tLngDat % 100;
    tDat[6] = tLngDat / 10;
    tLngDat = tLngDat % 10;
    tDat[7] = tLngDat;
    WeiDatBCD[0] = 0x00; //(tDat[0]<<4)+tDat[1];
    WeiDatBCD[1] = 0x41; //(tDat[2]<<4)+tDat[3];
    WeiDatBCD[2] = 0x89; //(tDat[4]<<4)+tDat[5];
    WeiDatBCD[3] = 0x20; //(tDat[6]<<4)+tDat[7];
    u4MbBuf[0] = 0xFF;
    u4MbBuf[1] = 0xCC;
    u4MbBuf[2] = 0x01;
    u4MbBuf[3] = 0x01;
    u4MbBuf[4] = 0x07;
    u4MbBuf[5] = 0x61;
    u4MbBuf[6] = WeiDatBCD[0];
    u4MbBuf[7] = WeiDatBCD[1];
    u4MbBuf[8] = WeiDatBCD[2];
    u4MbBuf[9] = WeiDatBCD[3];
    u4MbBuf[10] = (unsigned char)(ftaDIdat & 0xFF);
    u4MbBuf[11] = ftaDOdat;
    CrcDat = 0;
    for(i = 2; i < 12; i++)
    {
        CrcDat = (CrcDat + u4MbBuf[i]) & 0xFF;
    }
    u4MbBuf[12] = CrcDat;
    GPIO_SetBits(GPIOE, GPIO_Pin_2);
    u4Cnt = 0;
    u4SndCnt = 13;
    u4Flag = 1;
}

void SendPKQ(unsigned char id)
{
    unsigned char MdTxdBuf[9] = {0x01, 0x03, 0x00, 51, 0x00, 41, 0x11, 0xF4, 0};
    u16 crcresult;
    u8  crcHi = 0, crcLo = 0;
    unsigned char i;
    MdTxdBuf[0] = id;
    MdTxdBuf[5] = 41;
    crcresult = CRC16(MdTxdBuf, 6);
    crcHi = (crcresult & 0xff);
    crcLo = (crcresult & 0xFF00) >> 8;
    MdTxdBuf[6] = crcHi;
    MdTxdBuf[7] = crcLo;
    GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    u4RxdCnt = 0;
    lenMbDat = MB_NUM * 2 + 5;
    delay(0x2F0);
    for(i = 0; i < 8; i++)
    {
        USART_SendData(UART4, MdTxdBuf[i]);
        delay(0x3FFF);
    }
    GPIO_SetBits(GPIOE, GPIO_Pin_2);
    delay(0x2FF);
    PKQ_Msg[id].pTmr++;
}

/*******************************************************************************
* Function Name  : crc16
* Description	 : Configures the different system clocks.
* Input 		 : *puchMsg( ˝æ›µÿ÷∑)    usDataLen
* Return		 : 2 byte
*******************************************************************************/
unsigned short CRC16(unsigned char *puchMsg, unsigned short	usDataLen)
{

    unsigned char uchCRCHi = 0xFF ; /* ∏ﬂCRC◊÷Ω⁄≥ı ºªØ */
    unsigned char uchCRCLo = 0xFF ; /* µÕCRC ◊÷Ω⁄≥ı ºªØ */
    unsigned uIndex ; /* CRC—≠ª∑÷–µƒÀ˜“˝ */
    while (usDataLen--) /* ¥´ ‰œ˚œ¢ª∫≥Â«¯ */
    {
        uIndex = uchCRCLo ^ *puchMsg++;
        uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
        uchCRCHi = auchCRCLo[uIndex];
    }
    return (uchCRCHi << 8 | uchCRCLo) ;
}

unsigned char flgU2Stx1, flgU2Stx2, flgU2StRecv, u2RxdCnt;

void SendCard(unsigned char tDat)
{
    unsigned char ti;
    unsigned char IcTxdBuf[12] = {0xFF, 0xCC, 0x01, 0x57, 0x38, 0x31, 0x30, 0x36, 0x31, 0x38, 0x9C, 0x00};
    unsigned char tXorDat;
    if(tDat == 0x04)
    {
        IcTxdBuf[3] = 0x17;
        tXorDat = 0xCC;
        for(ti = 2; ti < 10; ti++)
            tXorDat = tXorDat ^ IcTxdBuf[ti];
        IcTxdBuf[10] = tXorDat;
    }
    if(tDat == 0x03)
    {
        IcTxdBuf[3] = 0x59;
        tXorDat = 0xCC;
        for(ti = 2; ti < 10; ti++)
            tXorDat = tXorDat ^ IcTxdBuf[ti];
        IcTxdBuf[10] = tXorDat;
    }
    if(tDat == 0x02)
    {
        IcTxdBuf[3] = 0x56;
        tXorDat = 0xCC;
        for(ti = 2; ti < 10; ti++)
            tXorDat = tXorDat ^ IcTxdBuf[ti];
        IcTxdBuf[10] = tXorDat;
    }
    IcTxdBuf[11] = 0x00;
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    delay(0x2F0);
    for(ti = 0; ti < 11; ti++)
    {
        USART_SendData(USART2, IcTxdBuf[ti]);
        delay(0xFFFF);
    }
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    //Delay(0x1FF);
    flgU2Stx1 = 0;
    flgU2Stx2 = 0;
    flgU2StRecv = 0;
    u2RxdCnt = 0;
}



//void TIM4_IRQHandler(void)
//{
//	unsigned char i;
//	unsigned char tDatLen;
////	unsigned char mbLen;
//	u16 crcresult,tcrc3;
//	u8  crcHi=0,tcrch3;//,crcLo=0;

//	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//		u2Tmr++;u3Tmr++;
//////Uart2
////if((u2RxdCnt>=8)&&(u2Tmr>20))//??,??MODBUS
////{
////	if(u2RxdCnt==(u2RxdBuf[2]+4))
////	{
////		u2RxdCnt=0;
////	}
////	u2RxdCnt=0;
////	tDatLen=u2RxdBuf[2]+4;
////	crcresult = CRC16(u2RxdBuf,6);
////	crcHi=(crcresult & 0xff);
////	//crcLo=(crcresult & 0xFF00)>>8;
////	if((crcHi==u2RxdBuf[6]))
////	{
////		flgMb2CrcOk=1;
////		for(i=0;i<=7;i++)
////		datMod2Buf[i]=u2RxdBuf[i];
////		}
////	else
////	{	flgMb2CrcOk=0;}
////	u2RxdCnt=0;
////}
////else if(u2Tmr>100)
////{
////	u2RxdCnt=0;u2Tmr=0;
////}
//
//////Uart3
////if((u3RxdCnt>=8)&&(u3Tmr>20))//??,??MODBUS
////{
////	if(u3RxdCnt==(u3RxdBuf[2]+4))
////	{
////		u3RxdCnt=0;
////	}
////	u3RxdCnt=0;
////	tDatLen=u3RxdBuf[2]+4;
////	crcresult = CRC16(u3RxdBuf,6);
////	crcHi=(crcresult & 0xff);
////	//crcLo=(crcresult & 0xFF00)>>8;
////	if((crcHi==u3RxdBuf[6]))
////	{
////		flgMb3CrcOk=1;
////		for(i=0;i<=7;i++)
////		datMod3Buf[i]=u3RxdBuf[i];
////		}
////	else
////	{	flgMb3CrcOk=0;}
////	u3RxdCnt=0;
////}
////else if(u3Tmr>100)
////{
////	u3RxdCnt=0;u3Tmr=0;
////}
//	}
//}





