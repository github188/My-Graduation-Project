/******************************************************************************
  * @file    RA8875.c
  * @author  George Vigelette
  * @version V1.0.0
  * @date    OCT 1, 2013
  * @brief   This file contains the headers for timing delay functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 Vigeltek Inc</center></h2>
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "RA8875.h"

#define RA8875_RST_CLK  RCC_AHB1Periph_GPIOC
#define RA8875_RST_GPIO GPIOC
#define RA8875_RST_PIN	GPIO_Pin_5

#define RA8875_INT_CLK	RCC_AHB1Periph_GPIOB
#define RA8875_INT_GPIO GPIOB
#define RA8875_INT_PIN	GPIO_Pin_0

#define RA8875_WAIT_CLK	RCC_AHB1Periph_GPIOB
#define RA8875_WAIT_GPIO GPIOB
#define RA8875_WAIT_PIN	GPIO_Pin_1


__INLINE void raReset(void);
__INLINE static void prvRACmdWrite(uint16_t tCmd);
__INLINE static void prvRADataWrite(uint16_t tData);
static uint16_t prvRADataRead(void);
__INLINE static uint8_t prvRAStatusRead(void);

void lcdBacklightAdjust(uint8_t tPercent);

void prvDelay1ms(uint16_t i)
{
	uint32_t j;
	while(i--)
	for(j=0;j<50000;j++);
}

void raHardwareConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RA8875_RST_CLK |RA8875_INT_CLK |RA8875_WAIT_CLK, ENABLE);
	//RST
	GPIO_InitStructure.GPIO_Pin = RA8875_RST_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_Init(RA8875_RST_GPIO, &GPIO_InitStructure);
  	//INT
	GPIO_InitStructure.GPIO_Pin = RA8875_INT_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_Init(RA8875_INT_GPIO, &GPIO_InitStructure);
  	//WAIT
	GPIO_InitStructure.GPIO_Pin = RA8875_WAIT_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  	GPIO_Init(RA8875_WAIT_GPIO, &GPIO_InitStructure);
}

/**
 * PLL��·���ã�FINΪ25M
 */
void RA8875_PLL_Init(void)
{
//SYS_CLK = FIN*(PLLDIVN[4:0] +1)/((PLLDIVM+1)*(2^PLLDIVK[2:0])) = 25����11+1��/��2��4�� = 37.5M
//AT070TN83
//  prvRACmdWrite(0x88);
//  prvRADataWrite(0x0b);
//  prvDelay1ms(1);
//  prvRACmdWrite(0x89);
//  prvRADataWrite(0x03);
//  prvDelay1ms(1);

  //AT070TN92
  prvRACmdWrite(0x88);
  prvRADataWrite(0x0C);
  prvDelay1ms(1);
  prvRACmdWrite(0x89);
  prvRADataWrite(0x02);
  prvDelay1ms(10);
  prvRACmdWrite(0x04);
  prvRADataWrite(0x81);
  prvDelay1ms(1);
}

/**
 * RA8875��ʼ��
 */
void raInit(void)
{

	FSMC_Init();
	prvDelay1ms(100);
	raHardwareConfig();
	raReset();

	RA8875_PLL_Init();

	raSetColor65K();
	raMCU16bitInterface();

	raPCLKFallingEdgeSampling();
	//raPCLKFrequencyDiv(2);//PLCK = SYSCLK*2 = 37.5 * 2M  AT070TN83 MAXclk = 50M
  //AT070TN92
  prvRACmdWrite(0x04);
  prvRADataWrite(0x81);
  prvDelay1ms(1);
	prvDelay1ms(5);

/*****************************����������Ҫ�ο�Һ������ʱ������*****************/
	//Horizontal set
	prvRACmdWrite(0x14);	 //HDWR//Horizontal Display Width Setting Bit[6:0]
	prvRADataWrite(0x63); //Horizontal display width(pixels) = (HDWR + 1)*8       
	prvDelay1ms(5);
	prvRACmdWrite(0x15);	 //HNDFCR//Horizontal Non-Display Period fine tune Bit[3:0]
	prvRADataWrite(0x00); //(HNDR + 1)*8 +HNDFCR
	prvDelay1ms(1);
	prvRACmdWrite(0x16);	 //HNDR//Horizontal Non-Display Period Bit[4:0]
	prvRADataWrite(0x03); //Horizontal Non-Display Period (pixels) = (HNDR + 1)*8
	prvDelay1ms(1);
	prvRACmdWrite(0x17);	 //HSTR//HSYNC Start Position[4:0]
	prvRADataWrite(0x03); //HSYNC Start Position(PCLK) = (HSTR + 1)*8
	prvDelay1ms(1);
	prvRACmdWrite(0x18);	 //HPWR//HSYNC Polarity ,The period width of HSYNC.
	prvRADataWrite(0x0b); //HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8
	prvDelay1ms(1);

	//Vertical set
	prvRACmdWrite(0x19); //VDHR0 //Vertical Display Height Bit [7:0]
	prvRADataWrite(0xDF); //Vertical pixels = VDHR + 1	0xef
	prvDelay1ms(1);
	prvRACmdWrite(0x1a); //VDHR1 //Vertical Display Height Bit [8]
	prvRADataWrite(0x01); //Vertical pixels = VDHR + 1 	0x00
	prvDelay1ms(1);
	prvRACmdWrite(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]
	prvRADataWrite(0x1F); //Vertical Non-Display area = (VNDR + 1)
	prvDelay1ms(1);
	prvRACmdWrite(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]
	prvRADataWrite(0x00); //Vertical Non-Display area = (VNDR + 1)
	prvDelay1ms(1);
	prvRACmdWrite(0x1d); //VSTR0 //VSYNC Start Position[7:0]
	prvRADataWrite(0x16); //VSYNC Start Position(PCLK) = (VSTR + 1)
	prvDelay1ms(1);
	prvRACmdWrite(0x1e); //VSTR1 //VSYNC Start Position[8]
	prvRADataWrite(0x00); //VSYNC Start Position(PCLK) = (VSTR + 1)
	prvDelay1ms(1);
	prvRACmdWrite(0x1f);	 //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	prvRADataWrite(0x01); //VSYNC Pulse Width(PCLK) = (VPWR + 1)
	prvDelay1ms(1);
////	//Horizontal set
////	prvRACmdWrite(0x14);	 //HDWR//Horizontal Display Width Setting Bit[6:0]
////	prvRADataWrite(0x63); //Horizontal display width(pixels) = (HDWR + 1)*8       
////	prvDelay1ms(5);
////	prvRACmdWrite(0x15);	 //HNDFCR//Horizontal Non-Display Period fine tune Bit[3:0]
////	prvRADataWrite(0x03); //(HNDR + 1)*8 +HNDFCR
////	prvDelay1ms(1);
////	prvRACmdWrite(0x16);	 //HNDR//Horizontal Non-Display Period Bit[4:0]
////	prvRADataWrite(0x03); //Horizontal Non-Display Period (pixels) = (HNDR + 1)*8
////	prvDelay1ms(1);
////	prvRACmdWrite(0x17);	 //HSTR//HSYNC Start Position[4:0]
////	prvRADataWrite(0x02); //HSYNC Start Position(PCLK) = (HSTR + 1)*8
////	prvDelay1ms(1);
////	prvRACmdWrite(0x18);	 //HPWR//HSYNC Polarity ,The period width of HSYNC.
////	prvRADataWrite(0x00); //HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8
////	prvDelay1ms(1);

////	//Vertical set
////	prvRACmdWrite(0x19); //VDHR0 //Vertical Display Height Bit [7:0]
////	prvRADataWrite(0xDF); //Vertical pixels = VDHR + 1	0xef
////	prvDelay1ms(1);
////	prvRACmdWrite(0x1a); //VDHR1 //Vertical Display Height Bit [8]
////	prvRADataWrite(0x01); //Vertical pixels = VDHR + 1 	0x00
////	prvDelay1ms(1);
////	prvRACmdWrite(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]
////	prvRADataWrite(0x14); //Vertical Non-Display area = (VNDR + 1)
////	prvDelay1ms(1);
////	prvRACmdWrite(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]
////	prvRADataWrite(0x00); //Vertical Non-Display area = (VNDR + 1)
////	prvDelay1ms(1);
////	prvRACmdWrite(0x1d); //VSTR0 //VSYNC Start Position[7:0]
////	prvRADataWrite(0x16); //VSYNC Start Position(PCLK) = (VSTR + 1)
////	prvDelay1ms(1);
////	prvRACmdWrite(0x1e); //VSTR1 //VSYNC Start Position[8]
////	prvRADataWrite(0x00); //VSYNC Start Position(PCLK) = (VSTR + 1)
////	prvDelay1ms(1);
////	prvRACmdWrite(0x1f);	 //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
////	prvRADataWrite(0x01); //VSYNC Pulse Width(PCLK) = (VPWR + 1)
////	prvDelay1ms(1);
/**************************************************************************/
	raActiveWindow(0,799,0,479);
	raMemoryWriteCoordinate(0,0);
	raMemoryClearActiveWindow();
	raMemoryClearStart();
	raDispalyOnOff(true);
	lcdBacklightAdjust(100);
	raDispalyOnOff(true);
	
}


/**
 * RA8875Ӳ����λ
 */
__INLINE void raReset(void)
{
	GPIO_ResetBits(RA8875_RST_GPIO, RA8875_RST_PIN);
	prvDelay1ms(10);
	GPIO_SetBits(RA8875_RST_GPIO, RA8875_RST_PIN);
	prvDelay1ms(10);

}


/**
 * д�����
 * @param tCmd Ҫд������
 */
__INLINE static void prvRACmdWrite(uint16_t tCmd)
{
	 FSMC_WriteCommand(tCmd);
}

/**
 * д���ݺ���
 * @param tData Ҫд������
 */
__INLINE static void prvRADataWrite(uint16_t tData)
{
	FSMC_WriteData(tData);
}

/**
 * �����ݺ���
 * @return  Ҫ��������
 */
static uint16_t prvRADataRead(void)
{
	return ((uint16_t)FSMC_ReadData());
}

/**
 * ��״̬����
 * @return  ���ص�״̬
 */
__INLINE static uint8_t prvRAStatusRead(void)
{
	return ((uint8_t)FSMC_ReadStatus());
}

/**
 * д���ݽ��Ĵ���
 * @param Cmd  �Ĵ���
 * @param Data ����
 */
void raWriteCmdAndData(uint16_t Cmd,uint16_t Data)
{
	prvRACmdWrite(Cmd);
	prvRADataWrite(Data);
}

/**
 * ����
 * @param x      X����
 * @param y      Y����
 * @param tColor ��ɫ
 */
void raDrawPixel(uint16_t x, uint16_t y, uint16_t tColor)
{
	raMemoryWriteCoordinate(x, y);
	raWriteCmdAndData(0x02, tColor);

}

/**
 * ����
 * @param  x X����
 * @param  y Y����
 * @return   ���ص����ɫ
 */
uint16_t raReadPixel(uint16_t x, uint16_t y)
{
	uint16_t tmp;
	// raWriteCmdAndData(0x40, 00);
	// raGraphicMode();
	// MemoryRead_Cursor_NoautoIncrease();
	raMemoryReadCoordinate(x, y);
	prvRACmdWrite(0x02);
	tmp = prvRADataRead() ;
	tmp = prvRADataRead() ;
	// tmp |= (prvRADataRead() << 8);
	return tmp;
}

/**
 * Ӳ���ܽ�æ
 */
void raMemoryBusy(void)
{
	while(!GPIO_ReadInputDataBit(RA8875_WAIT_GPIO, RA8875_WAIT_PIN));
}
//------------------------//
//STATUS

/**
 * �ڴ�æ
 */
void prvMemoryBusy(void)
{
	uint8_t temp;
	do
	{
	temp=prvRAStatusRead();
	}while((temp&0x80)==0x80);
	prvDelay1ms(5);
}

/**
 * �ڴ��Ƿ�æµ
 * @return  0������ 1��æµ
 */
bool prvMemoryIsBusy(void)
{
	uint8_t tmp;
	tmp = prvRAStatusRead();
	if((tmp & 0x80) == 0x80)
		return true;
	else
		return false;
}






bool raBTEBusy(void)
{
	uint8_t tmp;
	tmp = prvRAStatusRead();
	if((tmp & 0x40) == 0x40)
		return true;
	else
		return false;

}

bool raSROMBusy(void)
{
	uint8_t tmp;
	tmp = prvRAStatusRead();
	if((tmp & 0x01) == 0x01)
		return true;
	else
		return false;

}
//REG[BFh]
void Chk_DMA_Busy(void)
{
	uint8_t temp;
	do
	{
	prvRACmdWrite(0xBF);
	temp = prvRADataRead();
	}while((temp&0x01)==0x01);
}
//REG[01h]

/**
 * ��ʾ����
 * @param tOn 0:��ʾ�أ� 1:��ʾ��
 */
void raDispalyOnOff(bool tOn)
{
	if(tOn)
	{
	    prvRACmdWrite(0x01);//PWRR
		prvRADataWrite(0x80);
	}
	else
	{
			prvRACmdWrite(0x01);//PWRR
	prvRADataWrite(0x00);
	}
}

/**
 * ˯��ģʽ
 * @param tYes 0:˯��ģʽ�� 1:˯��ģʽ��
 */
void raSleep(bool tYes)
{
	if(tYes)
	{
		prvRACmdWrite(0x01);//PWRR
		prvRADataWrite(0x02);
	}
	else
	{
		prvRACmdWrite(0x01);//PWRR
	    prvRADataWrite(0x00);
	}
}

/**
 * �����λ
 */
void raSoftwareReset(void)
{
	prvRACmdWrite(0x01);//PWRR
    prvRADataWrite(0x01);
    prvRADataWrite(0x00);
    prvDelay1ms(1);//
}


/************************��ʼ�����õ�****************************/
//REG[04h]
/**
 * PCLK�����ز���
 */
void raPCLKRisingEdgeSampling(void)
{
	uint8_t temp;
  	prvRACmdWrite(0x04);//PWRR
	temp = prvRADataRead();
	temp |= cSetD7;
	prvRADataWrite(temp);
}

/**
 * PCLK�½��ز���
 */
void raPCLKFallingEdgeSampling(void)
{	
	uint8_t temp;
  	prvRACmdWrite(0x04);//PWRR
	temp = prvRADataRead();
	temp &= cClrD7;
	prvRADataWrite(temp);

}

/**
 * PCLK��Ƶ
 * @param tWidth [description]
 */
void raPCLKFrequencyDiv(uint8_t tWidth)
{
	uint8_t temp;
  	prvRACmdWrite(0x04);//PWRR
	temp = prvRADataRead();
	temp &=0x80;
    temp |= tWidth;
	prvRADataWrite(temp);
}
/*******************************************************************/

/*********************************SerialROM����*********************/
//REG[05h]
void raSerialROMSelect0(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=cClrD7;
	prvRADataWrite(temp);
}

void raSerialROMSelect1(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp |=cSetD7;
	prvRADataWrite(temp);
}

void raSerialROMAddress24bit(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=cClrD6;
	prvRADataWrite(temp);
}

void raSerialROMAddress32bit(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp |=cSetD6;
	prvRADataWrite(temp);
}

void raSerialWaveformMode0(void)
{	uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=cClrD5;
	prvRADataWrite(temp);
}

void raSerialWaveformMode3(void)
{	uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp |=cSetD5;
	prvRADataWrite(temp);
}

void raSerialROMReadCycleIs4bus(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=0xE7;
	prvRADataWrite(temp);
}

void raSerialROMReadCycleIs5bus(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=0xE7;
    temp |=cSetD3;
	prvRADataWrite(temp);
}

void raSerialROMReadCycleIs6bus(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=0xE7;
    temp |=cSetD4;
	prvRADataWrite(temp);
}

void raSerialROMFontMode(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=cClrD2;
	prvRADataWrite(temp);
}

void raSerialROMDMAMode(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp |=cSetD2;
	prvRADataWrite(temp);
}

void raSerialROMSignalMode(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=0xFC;
	prvRADataWrite(temp);
}

void raSerialROMDualMode0(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=0xFC;
    temp |=cSetD1;
	prvRADataWrite(temp);
}

void raSerialROMDualMode1(void)
{   uint8_t temp;
  	prvRACmdWrite(0x05);//PWRR
	temp = prvRADataRead();
	temp &=0xFC;
    temp |=cSetD1;
    temp |=cSetD0;
	prvRADataWrite(temp);
}

//REG[06H]
void raSromClkDiv(uint8_t CLK_DIV)
{
	prvRACmdWrite(0x06);
    prvRADataWrite(CLK_DIV);
}
/*******************************************************************/


/***********************************ɫ����ӿ�����******************/
//REG[10h]
/**
 * ����Ϊ256ɫ 
 */
void raSetColor256(void)
{
	uint8_t temp;
	prvRACmdWrite(0x10);//SYSR
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	prvRADataWrite(temp);
}
/**
 * ����Ϊ65kɫ
 */
void raSetColor65K(void)
{
	uint8_t temp;
	prvRACmdWrite(0x10);//SYSR
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	prvRACmdWrite(0x10);//SYSR
	prvRADataWrite(temp);
}

/**
 * ������MCU�Ľӿ�Ϊ8λ
 */
void raMCU8bitInterface(void)
{
	uint8_t temp;
	prvRACmdWrite(0x10);//SYSR
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	prvRADataWrite(temp);
}

/**
 * ������MCU�Ľӿ�λ16λ
 */
void raMCU16bitInterface(void)
{
	uint8_t temp;
	prvRACmdWrite(0x10);//SYSR
	temp = prvRADataRead();
	temp |= cSetD1ClrD0 ;
	prvRADataWrite(temp);
}
/*******************************************************************/


/*********************************GP������**************************/
//REG[12h]
uint8_t raReadGPIData(void)
{   uint8_t temp;
	prvRACmdWrite(0x12);//SYSR
	temp = prvRADataRead();
	return temp;
}

//REG[13H]
void raWriteGPOData(uint8_t setx)
{
  prvRACmdWrite(0x13);
  prvRADataWrite(setx);
}
/*******************************************************************/


/******************************LCD��ʾģʽ**************************/
//REG[20h]

void raOneLayer(void)
{
	uint8_t temp;
	prvRACmdWrite(0x20);//DPCR
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
}

void raTwoLayer(void)
{	uint8_t temp;
	prvRACmdWrite(0x20);//DPCR
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
}

void raHDirSEG0ToSEGn(void)
{	uint8_t temp;
	prvRACmdWrite(0x20);//DPCR
	temp = prvRADataRead();
	temp &= cClrD3 ;
	prvRADataWrite(temp);
}
void raHDirSEGnToSEG0(void)
{
	uint8_t temp;

	prvRACmdWrite(0x20);//DPCR
	temp = prvRADataRead();
	temp |= cSetD3 ;
	prvRADataWrite(temp);
}
void raVDirCOM0ToCOMn(void)
{
	uint8_t temp;

	prvRACmdWrite(0x20);//DPCR
	temp = prvRADataRead();
	temp &= cClrD2 ;
	prvRADataWrite(temp);
}
void raVDirCOMnToCOM0(void)
{
	uint8_t temp;

	prvRACmdWrite(0x20);//DPCR
	temp = prvRADataRead();
	temp |= cSetD2 ;
	prvRADataWrite(temp);
}
/*******************************************************************/


/*************************������ƼĴ���***************************/
//REG[21h]
void raFontControlCGROMFont(void)
{
	uint8_t temp;
	prvRACmdWrite(0x21);//FNCR0
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
}
void raFontControlCGRAMFont(void)
{
	uint8_t temp;
	prvRACmdWrite(0x21);//FNCR0
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
}

void raFontControlInternalCGROM(void)
{
	uint8_t temp;
	prvRACmdWrite(0x21);//FNCR0
	temp = prvRADataRead();
	temp &= cClrD5 ;
	prvRADataWrite(temp);
}
void raFontControlExternalCGROM(void)
{
	uint8_t temp;
	prvRACmdWrite(0x21);//FNCR0
	temp = prvRADataRead();
	temp |= cSetD5 ;
	prvRADataWrite(temp);
}

void raFontControlISO8859_1(void)
{
	uint8_t temp;
	prvRACmdWrite(0x21);//FNCR0
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	prvRADataWrite(temp);
}
void raFontControlISO8859_2(void)
{
	uint8_t temp;
	prvRACmdWrite(0x21);//FNCR0
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	prvRADataWrite(temp);
}
void raFontControlISO8859_3(void)
{
	uint8_t temp;
	prvRACmdWrite(0x21);//FNCR0
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1ClrD0 ;
	prvRADataWrite(temp);
}
void raFontControlISO8859_4(void)
{
	uint8_t temp;
	prvRACmdWrite(0x21);//FNCR0
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1D0 ;
	prvRADataWrite(temp);
}

//REG[22h]
void raNoFontAlignment(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
}
void raFontAlignment(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
}
void raFontWithBackgroundColor(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD6 ;
	prvRADataWrite(temp);
}
void raFontWithBackgroundTransparency(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp |= cSetD6 ;
	prvRADataWrite(temp);
}


void raFontRotate0(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD4 ;
	prvRADataWrite(temp);
}
void raFontRotate90(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp |= cSetD4 ;
	prvRADataWrite(temp);
}


void raFontHorizontalEnlarge_x1(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	prvRADataWrite(temp);
}
void raFontHorizontalEnlarge_x2(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	prvRADataWrite(temp);
}
void raFontHorizontalEnlarge_x3(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	prvRADataWrite(temp);
}
void raFontHorizontalEnlarge_x4(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	prvRADataWrite(temp);
}
void raFontVerticalEnlarge_x1(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	prvRADataWrite(temp);
}
void raFontVerticalEnlarge_x2(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	prvRADataWrite(temp);
}
void raFontVerticalEnlarge_x3(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1ClrD0 ;
	prvRADataWrite(temp);
}
void raFontVerticalEnlarge_x4(void)
{
	uint8_t temp;
	prvRACmdWrite(0x22);//FNCR1
	temp = prvRADataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1D0 ;
	prvRADataWrite(temp);
}

//REG[23h]
void CGRAM_Select_Number(uint8_t setx)
{
    prvRACmdWrite(0x23);//CGSR
	prvRADataWrite(setx);
}
/*******************************************************************/

/*****************************��ƫ��*****************************/
//REG[24h]REG[25h]REG[26h]REG[27h]
void raScrollOffset(uint16_t X,uint16_t Y)
{

    prvRACmdWrite(0x24);//HOFS0
	prvRADataWrite(X);

    prvRACmdWrite(0x25);//HOFS1
	prvRADataWrite(X>>8);


    prvRACmdWrite(0x26);//VOFS0
	prvRADataWrite(Y);

    prvRACmdWrite(0x27);//VOFS1
	prvRADataWrite(Y>>8);
}
/*******************************************************************/


//-------------------------------------------//
//REG[29h]
void raFontLineDistance(uint8_t setx)
{
    prvRACmdWrite(0x29);//LDS
	prvRADataWrite(setx);
}
//-------------------------------------------//
//REG[2Ah]REG[2Bh]REG[2Ch]REG[2Dh]
void raFontWriteCoordinate(uint16_t X,uint16_t Y)
{

    prvRACmdWrite(0x2A);//HOFS0
	prvRADataWrite(X);

    prvRACmdWrite(0x2B);//HOFS1
	prvRADataWrite(X>>8);


    prvRACmdWrite(0x2C);//VOFS0
	prvRADataWrite(Y);

    prvRACmdWrite(0x2D);//VOFS1
	prvRADataWrite(Y>>8);
}


//-------------------------------------------//
//REG[2Eh]
void raFontSize16x16_8x16(void)
{
 	uint8_t temp;
	prvRACmdWrite(0x2E);//FNCR1
	temp = prvRADataRead();
	temp &= 0x3f;
	prvRADataWrite(temp);
}

void raFontSize24x24_12x24(void)
{
 	uint8_t temp;
	prvRACmdWrite(0x2E);//FNCR1
	temp = prvRADataRead();
	temp &= 0x3f;
	temp |= 0x40;
	prvRADataWrite(temp);
}

void raFontSize32x32_16x32(void)
{
 	uint8_t temp;
	prvRACmdWrite(0x2E);//FNCR1
	temp = prvRADataRead();
	temp &= 0x3f;
	temp |= 0x80;
	prvRADataWrite(temp);
}


void raFontToFontWidth(uint8_t setx) //bit[5:0]
{   uint8_t temp,temp1;
    temp1=setx&0x3F;
   	prvRACmdWrite(0x2E);//FNCR1
	temp = prvRADataRead();
	temp &= 0xc0;
	temp |= temp1;
	prvRADataWrite(temp);

}

/*************************����ROM����******************************/
//REG[2Fh]
void raGTSROMGT21L16T1W(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0x1f;
  prvRADataWrite(temp);
}

void raGTSROMGT23L16U2W(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0x1f;
  temp |= 0x20;
  prvRADataWrite(temp);
}

void raGTSROMGT23L24T3Y(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0x1f;
  temp |= 0x40;
  prvRADataWrite(temp);
}

void raGTSROMGT23L24M1Z(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0x1f;
  temp |= 0x60;
  prvRADataWrite(temp);
}

void raGTSROMGT23L32S4W(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0x1f;
  temp |= 0x80;
  prvRADataWrite(temp);
}

void raFontCodeGB2312(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xE3;
  prvRADataWrite(temp);
}

void raFontCodeGB12345(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xE3;
   temp |= 0x04;
  prvRADataWrite(temp);
}

void raFontCodeBIG5(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xE3;
  temp |= 0x08;
  prvRADataWrite(temp);
}

void raFontCodeUNICODE(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xE3;
  temp |= 0x0C;
  prvRADataWrite(temp);
}

void raFontCodeASCII(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xE3;
  temp |= 0x10;
  prvRADataWrite(temp);
}

void raFontCodeUNIJIS(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xE3;
  temp |= 0x14;
  prvRADataWrite(temp);
}

void raFontCodeJIS0208(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xE3;
  temp |= 0x18;
  prvRADataWrite(temp);
}

void raFontCodeLATIN(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xE3;
  temp |= 0x1C;
  prvRADataWrite(temp);
}

void raFontStandard(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xFC;
  prvRADataWrite(temp);
}

void raFontArial(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xFC;
  temp |= 0x01;
  prvRADataWrite(temp);
}

void raFontRoman(void)
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= 0xFC;
  temp |= 0x02;
  prvRADataWrite(temp);
}

void raFontBold(void) //for ASCII
{ uint8_t temp;
  prvRACmdWrite(0x2F);//FNCR1
  temp = prvRADataRead();
  temp &= cClrD1D0;
  temp |= cSetD1D0;
  prvRADataWrite(temp);
}
/*******************************************************************/

/*****************************�������ں;���������***************/
//REG[30h]~REG[37h]
void raActiveWindow(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB)
{
    prvRACmdWrite(0x30);//HSAW0
	prvRADataWrite(XL);
    prvRACmdWrite(0x31);//HSAW1
	prvRADataWrite(XL>>8);

    prvRACmdWrite(0x34);//HEAW0
	prvRADataWrite(XR);
    prvRACmdWrite(0x35);//HEAW1
	prvRADataWrite(XR>>8);

    //setting active window Y

    prvRACmdWrite(0x32);//VSAW0
	prvRADataWrite(YT);
    prvRACmdWrite(0x33);//VSAW1
	prvRADataWrite(YT>>8);

    prvRACmdWrite(0x36);//VEAW0
	prvRADataWrite(YB);
    prvRACmdWrite(0x37);//VEAW1
	prvRADataWrite(YB>>8);
}
//--------------------------------------------//
//REG[38h]~REG[3Fh]

void raScrollWindow(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB)
{

    prvRACmdWrite(0x38);//HSSW0
	prvRADataWrite(XL);
    prvRACmdWrite(0x39);//HSSW1
	prvRADataWrite(XL>>8);

    prvRACmdWrite(0x3c);//HESW0
	prvRADataWrite(XR);
    prvRACmdWrite(0x3d);//HESW1
	prvRADataWrite(XR>>8);

    prvRACmdWrite(0x3a);//VSSW0
	prvRADataWrite(YT);
    prvRACmdWrite(0x3b);//VSSW1
	prvRADataWrite(YT>>8);

    prvRACmdWrite(0x3e);//VESW0
	prvRADataWrite(YB);
    prvRACmdWrite(0x3f);//VESW1
	prvRADataWrite(YB>>8);
}
/*******************************************************************/


/*******************************�������****************************/
//REG[40h]
//

/**
 * ��ʾASCII�ַ�
 * @param x             X����
 * @param y             Y����
 * @param tForeColor    ǰ��ɫ
 * @param tBackColor    ��ɫ
 * @param isTransparent �Ƿ�͸��
 * @param str           �ַ�
 */
void raDisplayASCII(uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor, bool isTransparent, char *str )
{
	raTextMode();
	raFontControlCGROMFont();
	raFontControlInternalCGROM();
	raBTEBackgroundColor(tBackColor);
	raBTEForegroundColor(tForeColor);
	// raTextCursorEnable();
	// raTextCursorBlinkEnable();
	// raTextBlinkTime(20);
	// raFontHorizontalEnlarge_x4();
	// raFontVerticalEnlarge_x4();
	if(isTransparent)
		raFontWithBackgroundTransparency();
	else
		raFontWithBackgroundColor();

	raFontWriteCoordinate(x,y);

   while(((*str) != '\0')||(*str)) 
   {
   		raMemoryBusy();
		prvRACmdWrite(0x02);
		prvRADataWrite(*str);
		str++;
   }
}

void raGraphicMode(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
}

void raTextMode(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
}

void raTextCursorDisable(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD6 ;
	prvRADataWrite(temp);
}

void raTextCursorEnable(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp |= cSetD6 ;
	prvRADataWrite(temp);
}

void raTextCursorBlinkDisable(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD5 ;
	prvRADataWrite(temp);
}

void raTextCursorBlinkEnable(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp |= cSetD5 ;
	prvRADataWrite(temp);
}


void raMemoryWriteBegin(void)
{
	raWriteCmdAndData(0x21,0x00);//Recovery of register
	raWriteCmdAndData(0x40,0x00);//Recovery of register
	prvRACmdWrite(0x02);//start data write	
}

// void MemoryWrite_Position(uint16_t X,uint16_t Y)
// {
// 	uint8_t temp;

// 	temp=X;
//     prvRACmdWrite(0x46);
// 	prvRADataWrite(temp);
// 	temp=X>>8;
//     prvRACmdWrite(0x47);
// 	prvRADataWrite(temp);

// 	temp=Y;
//     prvRACmdWrite(0x48);
// 	prvRADataWrite(temp);
// 	temp=Y>>8;
//     prvRACmdWrite(0x49);
// 	prvRADataWrite(temp);
// }

/**
 * ��RA8875�Դ��ﻭ����
 * @param x      X
 * @param y      Y
 * @param tWidth ���
 * @param arr    ��ɫ����
 */
void raMemoryDrawHLine(uint16_t x, uint16_t y, uint16_t tWidth, uint16_t* arr)
{
	uint32_t i;
	raMemoryWriteCoordinate(x,y);
	prvRACmdWrite(0x02);//start data write
	// raMemoryWriteBegin();
	for(i = 0; i < tWidth ; i++)
	{
		prvRADataWrite(arr[i]);
	}
}
void Memory_Write_Data(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const unsigned char* arr)
{
	int i;

	raWriteCmdAndData(0x21,0x00);//Recovery of register
	raWriteCmdAndData(0x40,0x00);//Recovery of register
	raActiveWindow(x,x+width-1,y, y+height-1);
	raMemoryWriteCoordinate(x,y);
	prvRACmdWrite(0x02);//start data write

	for(i = 0; i < width*height*2; i++)
	{
		prvRADataWrite(arr[i]);
	}
}

void raGraphicModeWrite_LeftRight_TopDown(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	prvRADataWrite(temp);
}

void raGraphicModeWrite_RightLeft_TopDown(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	prvRADataWrite(temp);
}

void raGraphicModeWrite_DownTop_LeftRight(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	prvRADataWrite(temp);
}

void raGraphicModeWrite_TopDown_LeftRight(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	prvRADataWrite(temp);
}

void MemoryWrite_Cursor_autoIncrease(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD1 ;
	prvRADataWrite(temp);
}

void MemoryWrite_Cursor_NoautoIncrease(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp |= cSetD1 ;
	prvRADataWrite(temp);
}

void MemoryRead_Cursor_autoIncrease(void)
{	uint8_t temp;
	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp &= cClrD0 ;
	prvRADataWrite(temp);
}

void MemoryRead_Cursor_NoautoIncrease(void)
{
	uint8_t temp;

	prvRACmdWrite(0x40);//MWCR0
	temp = prvRADataRead();
	temp |= cSetD0 ;
	prvRADataWrite(temp);
}

//--------------------------------------------//
//REG[41h]
void raGraphicCursorOff(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
}

void raGraphicCursorOn(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
}

void raGraphicCursorStyle1(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD6D5D4;
	prvRADataWrite(temp);
}

void raGraphicCursorStyle2(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6D5SetD4 ;
	prvRADataWrite(temp);
}

void raGraphicCursorStyle3(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5ClrD4 ;
	prvRADataWrite(temp);
}

void raGraphicCursorStyle4(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5D4 ;
	prvRADataWrite(temp);
}

void raGraphicCursorStyle5(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5D4 ;
	prvRADataWrite(temp);
}

void raGraphicCursorStyle6(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5SetD4 ;
	prvRADataWrite(temp);
}

void raGraphicCursorStyle7(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5ClrD4 ;
	prvRADataWrite(temp);
}

void raGraphicCursorStyle8(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5D4 ;
	prvRADataWrite(temp);
}

void raWriteToBank1and2(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD3D2 ;
	prvRADataWrite(temp);
}

void raWriteToCGRAM(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= 0xf3;
	temp |= cClrD3SetD2 ;
	prvRADataWrite(temp);
}

void raWriteToGraphicCursor(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= 0xf3;
	temp |= cSetD3ClrD2 ;
	prvRADataWrite(temp);
}

void raWriteToPattern(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= 0xf3;
	temp |= cSetD3D2 ;
	prvRADataWrite(temp);
}

void raWriteToBank1(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp &= cClrD0 ;
	prvRADataWrite(temp);
}

void raWriteToBank2(void)
{	uint8_t temp;
	prvRACmdWrite(0x41);//MWCR1
	temp = prvRADataRead();
	temp |= cSetD0 ;
	prvRADataWrite(temp);
}

//--------------------------------------------//
//--------------------------------------------//
//REG[44h]
void raTextBlinkTime(uint8_t setx)
{
	prvRACmdWrite(0x44);//CURS
	prvRADataWrite(setx);
}

//--------------------------------------------//
//REG[45h]
void raGraphicModeMemoryRead_LeftRight_TopDown(void)
{	uint8_t temp;
	prvRACmdWrite(0x45);//MRCD
	temp = prvRADataRead();
	temp &= cClrD1D0;
	prvRADataWrite(temp);
}
void raGraphicModeMemoryRead_RightLeft_TopDown(void)
{	uint8_t temp;
	prvRACmdWrite(0x45);//MRCD
	temp = prvRADataRead();
	temp |= cClrD1SetD0;
	prvRADataWrite(temp);
}
void raGraphicModeMemoryRead_TopDown_LeftRight(void)
{	uint8_t temp;
	prvRACmdWrite(0x45);//MRCD
	temp = prvRADataRead();
	temp |= cSetD1ClrD0;
	prvRADataWrite(temp);
}
void raGraphicModeMemoryRead_DownTop_LeftRight(void)
{	uint8_t temp;
	prvRACmdWrite(0x45);//MRCD
	temp = prvRADataRead();
	temp |= cSetD1D0;
	prvRADataWrite(temp);
}

//--------------------------------------------//
//REG[46h]~REG[49h]
void raMemoryWriteCoordinate(uint16_t X,uint16_t Y)
{
    prvRACmdWrite(0x46);
	prvRADataWrite(X);
    prvRACmdWrite(0x47);
	prvRADataWrite(X>>8);

    prvRACmdWrite(0x48);
	prvRADataWrite(Y);
    prvRACmdWrite(0x49);
	prvRADataWrite(Y>>8);
}



//--------------------------------------------//
//REG[4Ah]~REG[4Dh]
void raMemoryReadCoordinate(uint16_t X,uint16_t Y)
{

    prvRACmdWrite(0x4a);
	prvRADataWrite(X);
    prvRACmdWrite(0x4b);
	prvRADataWrite(X>>8);

    prvRACmdWrite(0x4c);
	prvRADataWrite(Y);
    prvRACmdWrite(0x4d);
	prvRADataWrite(Y>>8);
}

//--------------------------------------------//
//REG[4Eh]
void raTextCursorHorizontalSize(uint8_t setx)//bit[3:0]
{
    prvRACmdWrite(0x4E);//CURS
	prvRADataWrite(setx);
}

//REG[4Fh]
void raTextCursorVerticalSize(uint8_t setx)//bit[3:0]
{
    prvRACmdWrite(0x4F);//CURS
	prvRADataWrite(setx);
}

/*******************************************************************/


/******************************BTE����******************************/
//REG[50h]
void raBTEDisable(void)
{	uint8_t temp;
	prvRACmdWrite(0x50);//BECR0
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
}

void raBTEEnable(void)
{	uint8_t temp;
	prvRACmdWrite(0x50);//BECR0
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
}

void raBTEReadContiguousData(void)
{	uint8_t temp;
	prvRACmdWrite(0x50);//BECR0
	temp = prvRADataRead();
	temp |= cSetD6 ;
	prvRADataWrite(temp);
}

void raBTEReadRectangularData(void)
{	uint8_t temp;
	prvRACmdWrite(0x50);//BECR0
	temp = prvRADataRead();
	temp &= cClrD6 ;
	prvRADataWrite(temp);
}

//---------------------------------------------//
//REG[51h]
void raBTEROPCode(uint8_t setx)
{
    prvRACmdWrite(0x51);//BECR1
	prvRADataWrite(setx);
}

//---------------------------------------------//
//REG[52h]
void Layer1_Visible(void)
{	uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0xf8;
	prvRADataWrite(temp);
}

void Layer2_Visible(void)
{   uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0xf8;
	temp|=0x01;
	prvRADataWrite(temp);
}

void Transparent_Mode(void)
{	uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0xf8;
	temp|=0x03;
	prvRADataWrite(temp);
}

void Lighten_Overlay_Mode(void)
{   uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0xf8;
	temp|=0x02;
	prvRADataWrite(temp);
}

void Boolean_OR(void)
{   uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0xf8;
	temp|=0x04;
	prvRADataWrite(temp);
}

void Boolean_AND(void)
{	uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0xf8;
	temp|=0x05;
	prvRADataWrite(temp);
}


void Floating_window_mode(void)
{	uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0xf8;
	temp|=0x06;
	prvRADataWrite(temp);
}

void Floating_Window_transparent_with_BGTR_enable(void)
{   uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp|=cSetD5;
	prvRADataWrite(temp);
}

void Floating_Window_transparent_with_BGTR_disable(void)
{   uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=cClrD5;
	prvRADataWrite(temp);
}


void raLayer1and2Scroll(void)
{	uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0x3f;
	prvRADataWrite(temp);
}

void raLayer1Scroll(void)
{	uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0x3f;
	temp|=0x40;
	prvRADataWrite(temp);
}

void raLayer2Scroll(void)
{	uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0x3f;
	temp|=0x80;
	prvRADataWrite(temp);
}

void raLayerBufferScroll(void)
{	uint8_t temp;
	prvRACmdWrite(0x52);//LTPR0
	temp = prvRADataRead();
	temp&=0x3f;
	temp|=0xC0;
	prvRADataWrite(temp);
}

//---------------------------------------------//
//REG[53h]
void raLayer1Transparency(uint8_t setx)
{
	uint8_t temp;
	prvRACmdWrite(0x53);//LTPR1
	temp = prvRADataRead();
	temp &= 0x0f;
	temp |= (setx & 0x0f);
	prvRADataWrite(temp);
}

void raLayer2Transparency(uint8_t setx)
{
	uint8_t temp;
	prvRACmdWrite(0x53);//LTPR1
	temp = prvRADataRead();
	temp &= 0xf0;
	temp |= (setx >> 4);
	prvRADataWrite(temp);
}
//---------------------------------------------//
//REG[54h]~ [5Fh]
void BTE_Source_Destination	(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB)
{
	uint8_t temp,temp1;

	temp=XL;
    prvRACmdWrite(0x54);//HSBE0
	prvRADataWrite(temp);
	temp=XL>>8;
    prvRACmdWrite(0x55);//HSBE1
	prvRADataWrite(temp);

	temp=XR;
    prvRACmdWrite(0x58);//HDBE0
	prvRADataWrite(temp);
	temp=XR>>8;
    prvRACmdWrite(0x59);//HDBE1
	prvRADataWrite(temp);

	temp=YT;
    prvRACmdWrite(0x56);//VSBE0
	prvRADataWrite(temp);
	temp=YT>>8;
    prvRACmdWrite(0x57);//VSBE1
	temp1 = prvRADataRead();
	temp1 &= 0x80;
    temp=temp|temp1;
	prvRACmdWrite(0x57);//VSBE1
	prvRADataWrite(temp);


	temp=YB;
    prvRACmdWrite(0x5a);//VDBE0
	prvRADataWrite(temp);
	temp=YB>>8;
    prvRACmdWrite(0x5b);//VDBE1
	temp1 = prvRADataRead();
	temp1 &= 0x80;
	temp=temp|temp1;
	prvRACmdWrite(0x5b);//VDBE1
	prvRADataWrite(temp);
}

void Source_Layer1(void)
{   uint8_t temp;
	prvRACmdWrite(0x57);//VSBE1
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
}

void Source_Layer2(void)
{	uint8_t temp;
	prvRACmdWrite(0x57);//VSBE1
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
}

void Destination_Layer1(void)
{	uint8_t temp;
	prvRACmdWrite(0x5b);//VDBE1
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
}

void Destination_Layer2(void)
{	uint8_t temp;
	prvRACmdWrite(0x5b);//VDBE1
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
}

//---------------------------------------------//
//---------------------------------------------//
void BTE_Size_setting(uint16_t X,uint16_t Y)
{
    prvRACmdWrite(0x5c);//BEWR0
	prvRADataWrite(X);
    prvRACmdWrite(0x5d);//BEWR1
	prvRADataWrite(X>>8);

    prvRACmdWrite(0x5e);//BEHR0
	prvRADataWrite(Y);
    prvRACmdWrite(0x5f);//BEHR1
	prvRADataWrite(Y>>8);
}

//---------------------------------------------//
//REG[60h]~REG[67h]



void raBTEBackgroundColor(uint16_t tColor)
{
    prvRACmdWrite(0x60);
	prvRADataWrite((tColor&0xf800)>>11);

    prvRACmdWrite(0x61);
	prvRADataWrite((tColor&0x07e0)>>5);

	prvRACmdWrite(0x62);
	prvRADataWrite((tColor&0x001f));
}

void raBTEForegroundColor(uint16_t tColor)
{

    prvRACmdWrite(0x63);
	prvRADataWrite((tColor&0xf800)>>11);

    prvRACmdWrite(0x64);
	prvRADataWrite((tColor&0x07e0)>>5);

	prvRACmdWrite(0x65);
	prvRADataWrite((tColor&0x001f));
}

void Text_Background_Color(u16 color)
{
	prvRACmdWrite(0x60);//BGCR0
	prvRADataWrite((color&0x001f));

	prvRACmdWrite(0x61);//BGCR0
	prvRADataWrite((color&0x07e0)>>5);
	
	prvRACmdWrite(0x62);//BGCR0
	prvRADataWrite((color&0xf800)>>11);
	
	// RA8875_WAITSTATUS();
} 

void Text_Foreground_Color(u16 color)
{	    
	prvRACmdWrite(0x63);//BGCR0
	prvRADataWrite((color&0x001f));
	
	prvRACmdWrite(0x64);//BGCR0
	prvRADataWrite((color&0x07e0)>>5);
	
	prvRACmdWrite(0x65);//BGCR0
	prvRADataWrite((color&0xf800)>>11);
	// RA8875_WAITSTATUS();
}

//======================================
//for test
void BTE_Background_red(uint8_t setx)
{
    prvRACmdWrite(0x60);//BGCR0
	prvRADataWrite(setx);
}

void BTE_Background_green(uint8_t setx)
{
    prvRACmdWrite(0x61);//BGCR1
	prvRADataWrite(setx);
}

void BTE_Background_blue(uint8_t setx)
{
    prvRACmdWrite(0x62);//BGCR2
	prvRADataWrite(setx);
}

void BTE_Foreground_red(uint8_t setx)
{
    prvRACmdWrite(0x63);//FGCR0
	prvRADataWrite(setx);
}

void BTE_Foreground_green(uint8_t setx)
{
    prvRACmdWrite(0x64);//FGCR1
	prvRADataWrite(setx);
}

void BTE_Foreground_blue(uint8_t setx)
{
    prvRACmdWrite(0x65);//FGCR2
	prvRADataWrite(setx);
}

//======================================

void Pattern_Set_8x8(void)
{  uint8_t temp;

 prvRACmdWrite(0x66);//PTNO
 temp = prvRADataRead();
 temp &= cClrD7;
 prvRADataWrite(temp);
}

void Pattern_Set_16x16(void)
{  uint8_t  temp;

 prvRACmdWrite(0x66);//PTNO
 temp = prvRADataRead();
 temp |= cSetD7;
 prvRADataWrite(temp);
}


void Pattern_Set_number(uint8_t setx)
{   uint8_t temp;

    prvRACmdWrite(0x66);//PTNO
    temp = prvRADataRead();
    temp &= 0x80;
	prvRADataWrite(temp|setx);
}

void BackgroundColor_TransparentMode(uint8_t setR,uint8_t setG,uint8_t setB)
{
    prvRACmdWrite(0x67);//BGTR_R
	prvRADataWrite(setR);

    prvRACmdWrite(0x68);//BGTR_G
	prvRADataWrite(setG);

    prvRACmdWrite(0x69);//BGTR_B
	prvRADataWrite(setB);

}
/*******************************************************************/

/******************************����������*****************************/
//REG[70h]~REG[74h]
//==============================================================================
//Subroutine:	Enable_TP
//==============================================================================
void Enable_TP(void)
{   uint8_t temp;
	prvRACmdWrite(0x70);//TPCR0
	temp = prvRADataRead();
	temp |= cSetD7;
	prvRADataWrite(temp);
}
//==============================================================================
//Subroutine:	Disable_TP
//==============================================================================
void Disable_TP(void)
{ uint8_t temp;
  prvRACmdWrite(0x70);//TPCR0
  temp = prvRADataRead();
  temp &= cClrD7;
  prvRADataWrite(temp);
 }

//==============================================================================
//Subroutine:	Enable_Touch_WakeUp
//==============================================================================
void Enable_Touch_WakeUp(void)
{
    uint8_t temp;
	prvRACmdWrite(0x70);//TPCR0
	temp = prvRADataRead();
	temp |= cSetD3;   //cSetb3 = 00001000
	prvRADataWrite(temp);
}
//==============================================================================
//Subroutine:	Disable_Touch_WakeUp
//==============================================================================
void Disable_Touch_WakeUp(void)
{	uint8_t temp;
	prvRACmdWrite(0x70);//TPCR0
	temp = prvRADataRead();
	temp &= cClrD3;
	prvRADataWrite(temp);
}

//==============================================================================
//Subroutine:	TP manual or auto  mode enable
//==============================================================================
void TP_manual_mode(void)
{	uint8_t temp;
  	prvRACmdWrite(0x71);//TPCR1
	temp = prvRADataRead();
	temp |= cSetD6;
	prvRADataWrite(temp);
}
void TP_auto_mode(void)
{	uint8_t temp;
  	prvRACmdWrite(0x71);//TPCR1
	temp = prvRADataRead();
	temp &= cClrD6;
	prvRADataWrite(temp);
}
//==============================================================================
//Subroutine:	TP_IDLE_mode
//==============================================================================
void TP_IDLE_mode(void)
{ uint8_t temp;
  prvRACmdWrite(0x71);//TPCR1
  temp=prvRADataRead();
  temp &= 0xf0;
  prvRADataWrite(temp);
 }
//==============================================================================
//Subroutine:	TP_event_mode
//==============================================================================
void TP_event_mode(void)
{ uint8_t temp;
  prvRACmdWrite(0x71);//TPCR1
  temp=prvRADataRead();
  temp &= 0xf0;
  temp |= 0x01;
  prvRADataWrite(temp);
//  prvRADataWrite(0xc1);
}
//==============================================================================
//Subroutine:	TP_latch_X
//==============================================================================
void TP_latch_X(void)
{ uint8_t temp;
  prvRACmdWrite(0x71);//TPCR1
  temp=prvRADataRead();
  temp &= 0xc0;
  temp |= 0x02;
  prvRADataWrite(temp);
 }
 //==============================================================================
//Subroutine:	TP_latch_Y
//==============================================================================
void TP_latch_Y(void)
{ uint8_t temp;
  prvRACmdWrite(0x71);//TPCR1
  temp=prvRADataRead();
  temp &= 0xc0;
  temp |= 0x03;
  prvRADataWrite(temp);
 }
//==============================================================================
//Subroutine:	Detect_TP_X_Y coordinate
//==============================================================================
uint8_t ADC_X(void)
{	uint8_t temp;
	prvRACmdWrite(0x72);//TPXH	 X_coordinate high byte
	//prvMemoryBusy();
	temp=prvRADataRead();
	return temp;
}

uint8_t ADC_Y(void)
{	uint8_t temp;
	prvRACmdWrite(0x73);//TPYH	  Y_coordinate high byte
	//prvMemoryBusy();
	temp=prvRADataRead();
	return temp;
}

uint8_t ADC_XY(void)
{	uint8_t temp;
 	prvRACmdWrite(0x74);//TPXYL	  bit[3:2] Y_coordinate low byte  bit[1:0] X_coordinate low byte
	temp=prvRADataRead();
	return temp;
}

/*******************************************************************/

/**************************ͼ�ι��********************************/
//REG[80h]~REG[83h]
void raGraphicCursorCoordinate(uint16_t X,uint16_t Y)
{
    prvRACmdWrite(0x80);
	prvRADataWrite(X);
    prvRACmdWrite(0x81);
	prvRADataWrite(X>>8);

    prvRACmdWrite(0x82);
	prvRADataWrite(Y);
    prvRACmdWrite(0x83);
	prvRADataWrite(Y>>8);
}

//REG[84h]
void raGraphicCursorColor0(uint8_t setx)
{
    prvRACmdWrite(0x84);//GCC0
	prvRADataWrite(setx);
}

void raGraphicCursorColor1(uint8_t setx)
{
    prvRACmdWrite(0x85);//GCC1
	prvRADataWrite(setx);
}
/*******************************************************************/

/****************************PWM����*******************************/
//REG[8A]
void raPWM1Enable(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8a);//MCLR
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
 }

void raPWM1Disable(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8a);//MCLR
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
 }

void raPWM1RetainLowWhenDisabled(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8a);//MCLR
	temp = prvRADataRead();
	temp &= cClrD6 ;
	prvRADataWrite(temp);
}

void raPWM1RetainHighWhenDisabled(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8a);//MCLR
	temp = prvRADataRead();
	temp |= cSetD6 ;
	prvRADataWrite(temp);
}

 void raPWM1_fnuction_sel(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8a);//MCLR
	temp = prvRADataRead();
	temp &= cClrD4 ;
	prvRADataWrite(temp);
 }

 void raPWM1_system_clk_out(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8a);//MCLR
	temp = prvRADataRead();
	temp |= cSetD4 ;
	prvRADataWrite(temp);
 }

 void raPWM1ClockDiv(uint8_t setx) //bit0~3
{
	uint8_t temp,temp1;
	temp1= setx&0x0f;
	prvRACmdWrite(0x8a);//MCLR
	temp = prvRADataRead();
	temp &= 0xf0;
	temp |= temp1 ;
	prvRADataWrite(temp);
 }

 void raPWM1DutyCycle(uint8_t setx) //bit0~7
{

    prvRACmdWrite(0x8b);//PTNO
	prvRADataWrite(setx);
}

void raPWM2Enable(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8c);//MCLR
	temp = prvRADataRead();
	temp |= cSetD7 ;
	prvRADataWrite(temp);
 }

void raPWM2Disable(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8c);//MCLR
	temp = prvRADataRead();
	temp &= cClrD7 ;
	prvRADataWrite(temp);
 }

void raPWM2RetainLowWhenDisabled(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8c);//MCLR
	temp = prvRADataRead();
	temp &= cClrD6 ;
	prvRADataWrite(temp);
}

void raPWM2RetainHighWhenDisabled(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8c);//MCLR
	temp = prvRADataRead();
	temp |= cSetD6 ;
	prvRADataWrite(temp);
}

 void raPWM2_fnuction_sel(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8c);//MCLR
	temp = prvRADataRead();
	temp &= cClrD4 ;
	prvRADataWrite(temp);
 }

  void raPWM2_system_clk_out(void)
{
	uint8_t temp;
	prvRACmdWrite(0x8c);//MCLR
	temp = prvRADataRead();
	temp |= cSetD4 ;
	prvRADataWrite(temp);
 }

  void raPWM2ClockDiv(uint8_t setx) //bit0~3
{
	uint8_t temp,temp1;
	temp1= setx&0x0f;
	prvRACmdWrite(0x8c);//MCLR
	temp = prvRADataRead();
	temp &= 0xf0;
	temp |= temp1 ;
	prvRADataWrite(temp);
 }
 void raPWM2DutyCycle(uint8_t setx) //bit0~7
{
    prvRACmdWrite(0x8d);//PTNO
	prvRADataWrite(setx);
}

/*******************************************************************/

/**********************************�ڴ����************************/
//REG[8Eh]
void raMemoryClearStop(void)
{
//	uint8_t temp;
	prvRACmdWrite(0x8e);//MCLR
//	temp = prvRADataRead();
//	temp &= cClrD7 ;
	prvRADataWrite(0x00);
}
void raMemoryClearStart(void)
{
//	uint8_t temp;
	prvRACmdWrite(0x8e);//MCLR
	prvRADataWrite(0X80);
	prvMemoryBusy();
}
void raMemoryClearFullWindow(void)
{
//	uint8_t temp;
	prvRACmdWrite(0x8e);//MCLR
	prvRADataWrite(0X80);
	prvMemoryBusy();
}
void raMemoryClearActiveWindow(void)
{
//	uint8_t temp;
	prvRACmdWrite(0x8e);//MCLR
	prvRADataWrite(0xC0);
	prvMemoryBusy();
}
/*******************************************************************/


/**************************��ͼ������*******************************/

/**
 * ����
 * @param x1     X1
 * @param y1     Y1
 * @param x2     X2
 * @param y2     Y2
 */
void raDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	//x1
	prvRACmdWrite(0x91);
	prvRADataWrite((uint8_t)(x1));
	prvRACmdWrite(0x92);
	prvRADataWrite((uint8_t)(x1>>8));
	//y1
	prvRACmdWrite(0x93);
	prvRADataWrite((uint8_t)(y1));
	prvRACmdWrite(0x94);
	prvRADataWrite((uint8_t)(y1>>8));
	//x2
	prvRACmdWrite(0x95);
	prvRADataWrite((uint8_t)(x2));
	prvRACmdWrite(0x96);
	prvRADataWrite((uint8_t)(x2>>8));
	//y2
	prvRACmdWrite(0x97);
	prvRADataWrite((uint8_t)(y2));
	prvRACmdWrite(0x98);
	prvRADataWrite((uint8_t)(y2>>8));

	prvRACmdWrite(0x90);//��ʼ�ź�
	prvRADataWrite(0x00);
	prvRADataWrite(0x80);
}

/**
 * ������
 * @param x1      X1
 * @param y1      Y1
 * @param x2      X2
 * @param y2      Y2
 * @param tIsfill �Ƿ����
 */
void raDrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,  bool tIsfill)
{
	//x1
	prvRACmdWrite(0x91);
	prvRADataWrite((uint8_t)(x1));
	prvRACmdWrite(0x92);
	prvRADataWrite((uint8_t)(x1>>8));
	//y1
	prvRACmdWrite(0x93);
	prvRADataWrite((uint8_t)(y1));
	prvRACmdWrite(0x94);
	prvRADataWrite((uint8_t)(y1>>8));
	//x2
	prvRACmdWrite(0x95);
	prvRADataWrite((uint8_t)(x2));
	prvRACmdWrite(0x96);
	prvRADataWrite((uint8_t)(x2>>8));
	//y2
	prvRACmdWrite(0x97);
	prvRADataWrite((uint8_t)(y2));
	prvRACmdWrite(0x98);
	prvRADataWrite((uint8_t)(y2>>8));

	if(tIsfill)
	{
	  prvRACmdWrite(0x90);//DCR
	  prvRADataWrite(0x10);
	  prvRADataWrite(0xb0);
	}
	else
	{
	  prvRACmdWrite(0x90);//DCR
	  prvRADataWrite(0x10);
	  prvRADataWrite(0x90);

	}
}

/**
 * ��Բ
 * @param X       Բ��X
 * @param Y       Բ��Y
 * @param R       �뾶
 * @param tIsfill �Ƿ����
 */
void raDrawCircle(uint16_t X,uint16_t Y,uint16_t R,  bool tIsfill)
{

    prvRACmdWrite(0x99);
	prvRADataWrite(X);
    prvRACmdWrite(0x9a);
	prvRADataWrite(X>>8);

    prvRACmdWrite(0x9b);
	prvRADataWrite(Y);
    prvRACmdWrite(0x9c);
	prvRADataWrite(Y>>8);

    prvRACmdWrite(0x9d);
	prvRADataWrite(R);


	if(tIsfill)
	{
	  prvRACmdWrite(0x90);//DCR
	  prvRADataWrite(0x60);
	}
	else
	{
	  prvRACmdWrite(0x90);//DCR
	  prvRADataWrite(0x40);
	}
}

/**
 * ����Բ
 * @param X       ���ĵ�X
 * @param Y       ���ĵ�Y
 * @param ELL_A   ELL_A
 * @param ELL_B   ELL_B
 * @param tIsfill �Ƿ����
 */
void raDrawEllipse(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B,  bool tIsfill)
{

    prvRACmdWrite(0xA5);
	prvRADataWrite(X);
    prvRACmdWrite(0xA6);
	prvRADataWrite(X>>8);

    prvRACmdWrite(0xA7);
	prvRADataWrite(Y);
    prvRACmdWrite(0xA8);
	prvRADataWrite(Y>>8);

    prvRACmdWrite(0xA1);
	prvRADataWrite(ELL_A);
	prvRACmdWrite(0xA2);
	prvRADataWrite(ELL_A>>8);

    prvRACmdWrite(0xA3);
	prvRADataWrite(ELL_B);
	prvRACmdWrite(0xA4);
	prvRADataWrite(ELL_B>>8);



	if(tIsfill)
	{
	   prvRACmdWrite(0xA0);//DCR
	   prvRADataWrite(0x40);
	   prvRADataWrite(0xC0);
	}
	else
	{
		prvRACmdWrite(0xA0);//DCR
		prvRADataWrite(0x00);
		prvRADataWrite(0x80);
	}

}

/**
 * ����Բ����
 * @param X       ���ĵ�X
 * @param Y       ���ĵ�
 * @param ELL_A   ELL_A
 * @param ELL_B   ELL_B
 * @param tIsfill �Ƿ����
 * @param tPart   00������ 01������ 10������ 11������
 */
void raDrawEllipseCurve(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B, bool tIsfill, uint8_t tPart)
{
	uint8_t temp;

    prvRACmdWrite(0xA5);
	prvRADataWrite(X);
    prvRACmdWrite(0xA6);
	prvRADataWrite(X>>8);

    prvRACmdWrite(0xA7);
	prvRADataWrite(Y);
    prvRACmdWrite(0xA8);
	prvRADataWrite(Y>>8);

    prvRACmdWrite(0xA1);
	prvRADataWrite(ELL_A);
	prvRACmdWrite(0xA2);
	prvRADataWrite(ELL_A>>8);

    prvRACmdWrite(0xA3);
	prvRADataWrite(ELL_B);
	prvRACmdWrite(0xA4);
	prvRADataWrite(ELL_B>>8);


	if(tIsfill)
	{
	   	prvRACmdWrite(0xA0);//DCR
	   	temp = 0x50 + tPart;
		prvRADataWrite(temp);
		temp = 0xD0 + tPart;
	   prvRADataWrite(temp);
  
	}
	else
	{
	   prvRACmdWrite(0xA0);//DCR
	   temp = 0x10 + tPart;
		prvRADataWrite(temp);
		temp = 0x90 + tPart;
	   prvRADataWrite(temp);
	}

}


/**
 * Բ�Ǿ���
 * @param XL      ���X����
 * @param YT      ����Y����
 * @param XR      �ұ�X����
 * @param YB      �ײ�Y����
 * @param ELL_A   ELL_A
 * @param ELL_B   ELL_B
 * @param tIsfill �Ƿ����
 */
void raDrawCircleSquare(uint16_t XL,uint16_t YT ,uint16_t XR ,uint16_t YB ,uint16_t ELL_A,uint16_t ELL_B, bool tIsfill)
{

    prvRACmdWrite(0x91);
	prvRADataWrite(XL);
    prvRACmdWrite(0x92);
	prvRADataWrite(XL>>8);

    prvRACmdWrite(0x95);
	prvRADataWrite(XR);
    prvRACmdWrite(0x96);
	prvRADataWrite(XR>>8);

    prvRACmdWrite(0x93);
	prvRADataWrite(YT);
    prvRACmdWrite(0x94);
	prvRADataWrite(YT>>8);

    prvRACmdWrite(0x97);
	prvRADataWrite(YB);
    prvRACmdWrite(0x98);
	prvRADataWrite(YB>>8);

    prvRACmdWrite(0xA1);
	prvRADataWrite(ELL_A);
	prvRACmdWrite(0xA2);
	prvRADataWrite(ELL_A>>8);

    prvRACmdWrite(0xA3);
	prvRADataWrite(ELL_B);
	prvRACmdWrite(0xA4);
	prvRADataWrite(ELL_B>>8);


	if(tIsfill)
	{
	   prvRACmdWrite(0xA0);//DCR
	   prvRADataWrite(0x60);
	   prvRADataWrite(0xE0);

	}
	else
	{
	   prvRACmdWrite(0xA0);//DCR
	   prvRADataWrite(0x20);
	   prvRADataWrite(0xA0);
	}
}

/**
 * ��������
 * @param P1_X    P1��X
 * @param P1_Y    P1��Y
 * @param P2_X    P2��X
 * @param P2_Y    P2��Y
 * @param P3_X    P3��X
 * @param P3_Y    P3��Y
 * @param tIsfill ���
 */
void raDrawTriangle(uint16_t P1_X,uint16_t P1_Y,uint16_t P2_X,uint16_t P2_Y,uint16_t P3_X,uint16_t P3_Y, bool tIsfill)
{

    prvRACmdWrite(0x91);
	prvRADataWrite(P1_X);
    prvRACmdWrite(0x92);
	prvRADataWrite(P1_X>>8);

    prvRACmdWrite(0x93);
	prvRADataWrite(P1_Y);
    prvRACmdWrite(0x94);
	prvRADataWrite(P1_Y>>8);

    prvRACmdWrite(0x95);
	prvRADataWrite(P2_X);
    prvRACmdWrite(0x96);
	prvRADataWrite(P2_X>>8);

    prvRACmdWrite(0x97);
	prvRADataWrite(P2_Y);
    prvRACmdWrite(0x98);
	prvRADataWrite(P2_Y>>8);

    prvRACmdWrite(0xA9);
	prvRADataWrite(P3_X);
    prvRACmdWrite(0xAA);
	prvRADataWrite(P3_X>>8);

    prvRACmdWrite(0xAB);
	prvRADataWrite(P3_Y);
    prvRACmdWrite(0xAC);
	prvRADataWrite(P3_Y>>8);


	if(tIsfill)
	{
	   prvRACmdWrite(0x90);//DCR
	   prvRADataWrite(0x01);
	   prvRADataWrite(0xA1);

	}
	else
	{
	   prvRACmdWrite(0x90);//DCR
	   prvRADataWrite(0x00);
	   prvRADataWrite(0x81);

	}
}


/*******************************************************************/



/**************************DMA������******************************/
//REG[B0]~REG[B3]
void DMA_Start_address_setting(uint32_t set_address)
{
  prvRACmdWrite(0xB0);
  prvRADataWrite(set_address);

  prvRACmdWrite(0xB1);
  prvRADataWrite(set_address>>8);

  prvRACmdWrite(0xB2);
  prvRADataWrite(set_address>>16);

//  prvRACmdWrite(0xB3);
//  prvRADataWrite(set_address>>24);

}

//input pic data quantity  for example  pic size = 800*480 = 384000
void DMA_Continuous_mode_size_setting(uint32_t set_size)
{
  prvRACmdWrite(0xB4);
  prvRADataWrite(set_size);

  prvRACmdWrite(0xB6);
  prvRADataWrite(set_size>>8);

  prvRACmdWrite(0xB8);
  prvRADataWrite(set_size>>16);

}

void DMA_block_mode_size_setting(uint16_t BWR,uint16_t BHR,uint16_t SPWR)
{
  prvRACmdWrite(0xB4);
  prvRADataWrite(BWR);
  prvRACmdWrite(0xB5);
  prvRADataWrite(BWR>>8);

  prvRACmdWrite(0xB6);
  prvRADataWrite(BHR);
  prvRACmdWrite(0xB7);
  prvRADataWrite(BHR>>8);

  prvRACmdWrite(0xB8);
  prvRADataWrite(SPWR);
  prvRACmdWrite(0xB9);
  prvRADataWrite(SPWR>>8);
}

//---------------------------------------------//
//REG[BFh]
void DMA_Continuous_mode(void)
{
  prvRACmdWrite(0xBF);
  prvRADataWrite(0x00);
}

void DMA_Block_mode(void)
{
  prvRACmdWrite(0xBF);
  prvRADataWrite(0x02);
}

void DMA_Start_enable(void)
{  uint8_t temp;
  prvRACmdWrite(0xBF);
  temp = prvRADataRead();
  temp |= 0x01;
  prvRADataWrite(temp);
}

/***********************����ɨ����IO������*******************/
//REG[C0h]
void Key_Scan_Enable(void)
{ uint8_t temp;
  prvRACmdWrite(0xC0);
  temp = prvRADataRead();
  temp |= 0x80;
  prvRADataWrite(temp);
}


void Key_Scan_setting(uint8_t setx)//bit[2:0]
{ uint8_t temp;
  prvRACmdWrite(0xC0);
  temp = prvRADataRead();
  temp &= 0xf0;
  temp|= (setx&0x07);
  prvRADataWrite(temp);
}

void Long_Key_Scan_enable(void)
{ uint8_t temp;
  prvRACmdWrite(0xC0);
  temp = prvRADataRead();
  temp |= 0x40;
  prvRADataWrite(temp);
}

void Key_Scan_Wakeup_Function_Enable(void)
{ uint8_t temp;
  prvRACmdWrite(0xC1);
  temp = prvRADataRead();
  temp |= 0x80;	//
  prvRADataWrite(temp);
}

void Long_Key_Timing_Adjustment(uint8_t setx)
{
   uint8_t temp,temp1;
  temp = setx & 0x1c;
  prvRACmdWrite(0xC1);
  temp1 = prvRADataRead();
  temp1|=temp;
  prvRADataWrite(temp1);

}
/****************************************************************/

/************************�������ڿ�����**************************/
//REG[D0]
void Floating_window_start_point(uint16_t X,uint16_t Y)
{
   prvRACmdWrite(0xD0);
   prvRADataWrite(X);
   prvRACmdWrite(0xD1);
   prvRADataWrite(X>>8);

   prvRACmdWrite(0xD2);
   prvRADataWrite(Y);
   prvRACmdWrite(0xD3);
   prvRADataWrite(Y>>8);
}


void Floating_window_size(uint16_t width_X,uint16_t high_Y)
{

   prvRACmdWrite(0xD4);
   prvRADataWrite(width_X);
   prvRACmdWrite(0xD5);
   prvRADataWrite(width_X>>8);

   prvRACmdWrite(0xD6);
   prvRADataWrite(high_Y);
   prvRACmdWrite(0xD7);
   prvRADataWrite(high_Y>>8);

}

void Floating_window_display_point(uint16_t X,uint16_t Y)
{

   prvRACmdWrite(0xD8);
   prvRADataWrite(X);
   prvRACmdWrite(0xD9);
   prvRADataWrite(X>>8);

   prvRACmdWrite(0xDA);
   prvRADataWrite(Y);
   prvRACmdWrite(0xDB);
   prvRADataWrite(Y>>8);

}
/*********************************************************************/

/*****************************SerialROM���ƻ�����***********************/
//REG[E0h]
void Serial_ROM_direct_access_mode_enable(void)
{
   prvRACmdWrite(0xE0);
   prvRADataWrite(0x01);
}

void Serial_ROM_direct_access_mode_disable(void)
{
   prvRACmdWrite(0xE0);
   prvRADataWrite(0x00);
}

void Direct_access_mode_Address_32bits(uint32_t set_address)
{
  uint8_t temp;
  temp = set_address>>24;
  prvRACmdWrite(0xE1);
  prvRADataWrite(temp);
  temp = set_address>>16;
  prvRADataWrite(temp);
  temp = set_address>>8;
  prvRADataWrite(temp);
  temp = set_address;
  prvRADataWrite(temp);

}

void Direct_access_mode_Address_24bits(uint32_t set_address)
{
  uint8_t temp=0;

  temp = set_address>>16;
  prvRACmdWrite(0xE1);
  prvRADataWrite(temp);
  temp = set_address>>8;
  prvRACmdWrite(0xE1);
  prvRADataWrite(temp);
  temp = set_address;
  prvRACmdWrite(0xE1);
  prvRADataWrite(temp);

}

uint8_t Direct_access_mode_Read_Data_buffer(void)
{
   uint8_t Serial_Data;

   prvRACmdWrite(0xE2);
   Serial_Data=prvRADataRead();
   return Serial_Data;
}
/*********************************************************************/




/**********************************�жϿ���***************************/
//REG[F0h]
void raKeyScanInterruptEnable(void)
{ uint8_t temp;
  prvRACmdWrite(0xF0);
  temp = prvRADataRead();
  temp |= 0x10;	//
  prvRADataWrite(temp);
}

void raKeyScanInterruptDisable(void)
{ uint8_t temp;
  prvRACmdWrite(0xF0);
  temp = prvRADataRead();
  temp &= 0x0F;	//
  prvRADataWrite(temp);
}


void raDMAInterruptEnable(void)
{ uint8_t temp;
  prvRACmdWrite(0xF0);
  temp = prvRADataRead();
  temp |= 0x08;	//
  prvRADataWrite(temp);
}

void raDMAInterruptDisable(void)
{ uint8_t temp;
  prvRACmdWrite(0xF0);
  temp = prvRADataRead();
  temp &= 0x17;	//
  prvRADataWrite(temp);
}

void raTouchPanelInterruptEnable(void)
{ uint8_t temp;
  prvRACmdWrite(0xF0);//INTC
  temp=prvRADataRead();
  temp |= 0x04;
  prvRADataWrite(temp);
 }

void raTouchPanelInterruptDisable(void)
{ uint8_t temp;
  prvRACmdWrite(0xF0);//INTC
  temp=prvRADataRead();
  temp &= 0x1B;
  prvRADataWrite(temp);
 }

void raBTEFinishInterruptEnable(void)
{
  uint8_t temp;
  prvRACmdWrite(0xF0);//INTC
  temp=prvRADataRead();
  temp |= 0x02;
  prvRADataWrite(temp);
}

void taBTEFinishInterruptDisable(void)
{
  uint8_t temp;
  prvRACmdWrite(0xF0);//INTC
  temp=prvRADataRead();
  temp &= 0x1D;
  prvRADataWrite(temp);
}

void Font_BTE_read_write_Interrupt_Enable(void)
{
  uint8_t temp;
  prvRACmdWrite(0xF0);//INTC
  temp=prvRADataRead();
  temp |= 0x01;
  prvRADataWrite(temp);
}

void Font_BTE_read_write_Interrupt_Disable(void)
{
  uint8_t temp;
  prvRACmdWrite(0xF0);//INTC
  temp=prvRADataRead();
  temp &= 0x1E;
  prvRADataWrite(temp);
}

//---------------------------------------------//
//REG[F1h]
void raClearKeyscanInterrupt(void)
{ uint8_t temp;
  prvRACmdWrite(0xF1);//INTC
  temp=prvRADataRead();
  temp |=cSetD4 ;
  prvRADataWrite(temp);
}

uint8_t Keyscan_Status(void)
{   uint8_t temp;
	prvRACmdWrite(0xF1);//INTC
	temp = prvRADataRead();
	if ((temp&0x10)==0x10)
	return 1;
	else
	return 0;
}

void raClearDMAInterrupt(void)
{ uint8_t temp;
  prvRACmdWrite(0xF1);//INTC
  temp=prvRADataRead();
  temp |=cSetD3 ;
  prvRADataWrite(temp);

}

uint8_t DMA_Status(void)
{   uint8_t temp;
	prvRACmdWrite(0xF1);//INTC
	temp = prvRADataRead();
	if ((temp&0x08)==0x08)
	return 1;
	else
	return 0;
}

void raClearTouchPanelInterrupt(void)
{ uint8_t temp;
  prvRACmdWrite(0xF1);//INTC
  temp=prvRADataRead();
  temp |= cSetD2;
  prvRADataWrite(temp);
 }


uint8_t Touch_Status(void)
{	uint8_t temp;
	prvRACmdWrite(0xF1);//INTC
	temp = prvRADataRead();
	if ((temp&0x04)==0x04)
		return 1;
	else
		return 0;
}

void raClearBTEFinishInterrupt(void)
{ uint8_t temp;
  prvRACmdWrite(0xF1);//INTC
  temp=prvRADataRead();
  temp |= cSetD1;
  prvRADataWrite(temp);
}

uint8_t BTE_Status(void)
{	uint8_t temp;
	prvRACmdWrite(0xF1);//INTC
	temp = prvRADataRead();
	if ((temp&0x02)==0x02)
	return 1;
	else
	return 0;
}

void Clear_Font_BTE_read_write_Interrupt(void)
{ uint8_t temp;
  prvRACmdWrite(0xF1);//INTC
  temp=prvRADataRead();
  temp |= cSetD0;
  prvRADataWrite(temp);
}

uint8_t Font_BTE_read_write_Status(void)
{	uint8_t temp;
	prvRACmdWrite(0xF1);//INTC
	temp = prvRADataRead();
	if ((temp&0x01)==0x01)
	return 1;
	else
	return 0;
}
/***************************************************************/


void Show_String(char *str,uint8_t n)
{
	raTextMode();
	prvRACmdWrite(0x02);
	while(*str != '\0')
	{
	 prvRADataWrite(*str);
	 ++str;
	 prvMemoryBusy();
	}
   prvDelay1ms(n);
}

/**
 * ��ʾ�ַ���
 * @param str �ַ���
 */
void raDisplayString(char *str)
{
	raTextMode();
	prvRACmdWrite(0x02);
	while(*str)
	{
	 	prvRADataWrite((u8)*str++);
	 	raMemoryBusy();
	}
	prvMemoryBusy();
}

/**
 * ��ʾ�ַ�
 * @param ch �ַ�
 */
void raDisplayChar(char ch)
{
	raTextMode();
	prvRACmdWrite(0x02);
	prvRADataWrite((u8)ch);
	raMemoryBusy();
}



uint8_t CheckPointRectangleCollision(Point pt, Rectangle rect)
{
	if((rect.x<= pt.x && pt.x <= (rect.height+rect.width))&&(rect.y <= pt.y && pt.y <= (rect.y + rect.height)))
		return 1;
	else
		return 0;
}
