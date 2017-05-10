/******************************************************************************
  * @file    RA8875.h
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


#ifndef RA8875_H_
#define RA8875_H_

#include "stm32f4xx.h"
#include "applicationTypes.h"
#include "Linker_FSMC.h"
#include <stdbool.h>

#define Color65k
#define USE_EMULATED_SPI
#define P480x272



#define	cSetD0		     0x01
#define	cSetD1		     0x02
#define	cSetD2		     0x04
#define	cSetD3		     0x08
#define	cSetD4		     0x10
#define	cSetD5		     0x20
#define	cSetD6		     0x40
#define	cSetD7		     0x80

#define	cClrD0		     0xfe
#define	cClrD1		     0xfd
#define	cClrD2		     0xfb
#define	cClrD3		     0xf7
#define	cClrD4		     0xef
#define	cClrD5		     0xdf
#define	cClrD6		     0xbf
#define	cClrD7		     0x7f

#define cClrD1D0         0xfc
#define cClrD1SetD0	     0x01
#define cSetD1ClrD0	     0x02
#define cSetD1D0		 0x03
#define cClrD3D2         0xf3
#define cClrD3SetD2      0x04
#define cSetD3ClrD2      0x08
#define cSetD3D2		 0x0c

#define cClrD2D1D0		 0xf8
#define cClrD2D1SetD0	 0x01
#define cClrD2SetD1ClrD0 0x02
#define cClrD2SetD1D0	 0x03
#define cSetD2ClrD1D0	 0x04
#define cSetD2ClrD1SetD0 0x05

#define cClrD6D5D4	     0x8f
#define cClrD6D5SetD4    0x10
#define cClrD6SetD5ClrD4 0x20
#define cClrD6SetD5D4	 0x30
#define cSetD6ClrD5D4	 0x40
#define cSetD6ClrD5SetD4 0x50
#define cSetD6D5ClrD4	 0x60
#define cSetD6D5D4		 0x70


#define color_grayscale1    2113
#define color_grayscale2    2113*2
#define color_grayscale3    2113*3
#define color_grayscale4    2113*4
#define color_grayscale5    2113*5
#define color_grayscale6    2113*6
#define color_grayscale7    2113*7
#define color_grayscale8    2113*8
#define color_grayscale9    2113*9
#define color_grayscale10   2113*10
#define color_grayscale11   2113*11
#define color_grayscale12   2113*12
#define color_grayscale13   2113*13
#define color_grayscale14   2113*14
#define color_grayscale15   2113*15
#define color_grayscale16   2113*16
#define color_grayscale17   2113*17
#define color_grayscale18   2113*18
#define color_grayscale19   2113*19
#define color_grayscale20   2113*20
#define color_grayscale21   2113*21
#define color_grayscale22   2113*22
#define color_grayscale23   2113*23
#define color_grayscale24   2113*24
#define color_grayscale25   2113*25
#define color_grayscale26   2113*26
#define color_grayscale27   2113*27
#define color_grayscale28   2113*28
#define color_grayscale29   2113*29
#define color_grayscale30   2113*30

void raDrawPixel(uint16_t x, uint16_t y, uint16_t tColor);

uint16_t raReadPixel(uint16_t x, uint16_t y);

void RA8875_PLL_ini(void) ;

void raInit(void);

void raDrawPixel(uint16_t x, uint16_t y, uint16_t tColor);

void raMemoryBusy(void);


//REG[BFh]
void Chk_DMA_Busy(void);

//REG[01h]

void raDispalyOnOff(bool tOn);
void raSleep(bool tYes);
void raSoftwareReset(void);


//REG[04h]
void raPCLKRisingEdgeSampling(void);
void raPCLKFallingEdgeSampling(void);
void raPCLKFrequencyDiv(uint8_t);

//REG[05h]
void raSerialROMSelect0(void);//bit7
void raSerialROMSelect1(void);//bit7

void raSerialROMAddress24bit(void);//bit6
void raSerialROMAddress32bit(void);//bit6

void raSerialWaveformMode0(void);//bit5
void raSerialWaveformMode3(void);//bit5

void raSerialROMReadCycleIs4bus(void); //bit4~3
void raSerialROMReadCycleIs5bus(void); //bit4~3
void raSerialROMReadCycleIs6bus(void); //bit4~3

void raSerialROMFontMode(void); //bit2
void raSerialROMDMAMode(void);  //bit2

void raSerialROMSignalMode(void); //bit1~0
void raSerialROMDualMode0(void);  //bit1~0
void raSerialROMDualMode1(void);  //bit1~0

//REG[06H]
void raSromClkDiv(uint8_t CLK_DIV);

//REG[10h]
void raSetColor256(void);
void raSetColor65K(void);
void raMCU8bitInterface(void);
void raMCU16bitInterface(void);


//REG[12h]

uint8_t raReadGPIData(void);
//REG[13h]

void raWriteGPOData(uint8_t setx);

//REG[14H]
void raSetDisplayHorizontalWidth(uint8_t tWidth);

//REG[15H]
//REG[16H]
//REG[17H]
//REG[18H]
//REG[19H] 
//REG[1AH] 
//REG[1BH] 
//REG[1CH] 
//REG[1DH] 
//REG[1EH] 
//REG[1FH] 

//REG[20h]
void raOneLayer(void);
void raTwoLayer(void);
void raHDirSEG0ToSEGn(void);
void raHDirSEGnToSEG0(void);
void raVDirCOM0ToCOMn(void);
void raVDirCOMnToCOM0(void);

//REG[21h]
void raFontControlCGROMFont(void);
void raFontControlCGRAMFont(void);
void raFontControlInternalCGROM(void);
void raFontControlExternalCGROM(void);
void raFontControlISO8859_1(void);
void raFontControlISO8859_2(void);
void raFontControlISO8859_3(void);
void raFontControlISO8859_4(void);

//REG[22h]
void raNoFontAlignment(void);
void raFontAlignment(void);
void raFontWithBackgroundColor(void);
void raFontWithBackgroundTransparency(void);

void raFontHorizontalEnlarge_x1(void);
void raFontHorizontalEnlarge_x2(void);
void raFontHorizontalEnlarge_x3(void);
void raFontHorizontalEnlarge_x4(void);
void raFontVerticalEnlarge_x1(void);
void raFontVerticalEnlarge_x2(void);
void raFontVerticalEnlarge_x3(void);
void raFontVerticalEnlarge_x4(void);

//REG[23h]
void CGRAM_Select_Number(uint8_t number);


//REG[24h]REG[25h]REG[26h]REG[27h]
void raScrollOffset(uint16_t X,uint16_t Y);

//REG[29h]
void raFontLineDistance(uint8_t setx);

//REG[2Ah]~REG[2Dh]
void raFontWriteCoordinate(uint16_t X,uint16_t Y);

//REG[2Eh]
void raFontSize16x16_8x16(void);
void raFontSize24x24_12x24(void);
void raFontSize32x32_16x32(void);
void raFontToFontWidth(uint8_t setx); //bit[5:0]

//REG[2Fh]
void raGTSROMGT21L16T1W(void);
void raGTSROMGT23L16U2W(void);
void raGTSROMGT23L24T3Y(void);
void raGTSROMGT23L24M1Z(void);
void raGTSROMGT23L32S4W(void);
void raFontCodeGB2312(void);
void raFontCodeGB12345(void);
void raFontCodeBIG5(void);
void raFontCodeUNICODE(void);
void raFontCodeASCII(void);
void raFontCodeUNIJIS(void);
void raFontCodeJIS0208(void);
void raFontCodeLATIN(void);
void raFontStandard(void);
void raFontArial(void);
void raFontRoman(void);
void raFontBold(void); //for ASCII

//REG[30h]~REG[37h]
void raActiveWindow(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB);

//REG[38h]~REG[3Fh]
void raScrollWindow(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB);
//REG[40h]
void raDisplayASCII(uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor, bool isTransparent, char *str );

/**
 * 在RA8875显存里画横线
 * @param x      X
 * @param y      Y
 * @param tWidth 宽度
 * @param arr    颜色缓存
 */
void raMemoryDrawHLine(uint16_t x, uint16_t y, uint16_t tWidth, uint16_t* arr);

void raGraphicMode(void);
void raTextMode(void);
void raTextCursorDisable(void);
void raTextCursorEnable(void);
void raTextCursorBlinkDisable(void);
void raTextCursorBlinkEnable(void);
void MemoryWrite_Position(uint16_t X,uint16_t Y);
void MemoryWrite_Begin(void);
void Memory_Write_Data(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const unsigned char* arr);
void raGraphicModeWrite_LeftRight_TopDown(void);
void raGraphicModeWrite_RightLeft_TopDown(void);
void raGraphicModeWrite_DownTop_LeftRight(void);
void raGraphicModeWrite_TopDown_LeftRight(void);
void MemoryWrite_Cursor_autoIncrease(void);
void MemoryWrite_Cursor_NoautoIncrease(void);
void MemoryRead_Cursor_autoIncrease(void);
void MemoryRead_Cursor_NoautoIncrease(void);

//REG[41h]
void raGraphicCursorOn(void);
void raGraphicCursorOff(void);
void raGraphicCursorStyle1(void);
void raGraphicCursorStyle2(void);
void raGraphicCursorStyle3(void);
void raGraphicCursorStyle4(void);
void raGraphicCursorStyle5(void);
void raGraphicCursorStyle6(void);
void raGraphicCursorStyle7(void);
void raGraphicCursorStyle8(void);

void raWriteToBank1and2(void);
void raWriteToCGRAM(void);
void raWriteToGraphicCursor(void);
void raWriteToPattern(void);
void raWriteToBank1(void);
void raWriteToBank2(void);


//REG[44h]
void raTextBlinkTime(uint8_t setx);

//REG[45h]
void raGraphicModeMemoryRead_LeftRight_TopDown(void);
void raGraphicModeMemoryRead_RightLeft_TopDown(void);
void raGraphicModeMemoryRead_TopDown_LeftRight(void);
void raGraphicModeMemoryRead_DownTop_LeftRight(void);

//REG[46h]~REG[49h]
void raMemoryWriteCoordinate(uint16_t X,uint16_t Y);


//REG[4Ah]~REG[4Dh]
void raMemoryReadCoordinate(uint16_t X,uint16_t Y);

//REG[4Eh]
void raTextCursorHorizontalSize(uint8_t setx);//bit[3:0]

//REG[4Fh]
void raTextCursorVerticalSize(uint8_t setx);//bit[3:0]

//REG[50h]
bool raBTEisBusy(void);
void raBTEEnable(void);
void raBTEDisable(void);
void raBTEReadContiguousData(void);
void raBTEReadRectangularData(void);
void raBTEWriteContiguousData(void);
void raBTEWriteRectangularData(void);

//REG[51h]
void BTE_ROP_Code(uint8_t setx);

//REG[52h]
void Layer1_Visible(void);
void Layer2_Visible(void);
void Transparent_Mode(void);
void Lighten_Overlay_Mode(void);
void Boolean_OR(void);
void Boolean_AND(void);
void Floating_window_mode(void);
void Floating_Window_transparent_with_BGTR_enable(void);
void Floating_Window_transparent_with_BGTR_disable(void);

void raLayer1and2Scroll(void);
void raLayer1Scroll(void);
void raLayer2Scroll(void);
void raLayerBufferScroll(void);



//REG[53h]

void raLayer1Transparency(uint8_t setx);
void raLayer2Transparency(uint8_t setx);

//REG[54h]~ [5Fh]
void BTE_Source_Destination	(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB);
void Source_Layer1(void);
void Source_Layer2(void);
void Destination_Layer1(void);
void Destination_Layer2(void);
void BTE_Size_setting(uint16_t X,uint16_t Y);


//REG[60h]~REG[67h]
void Text_color(uint16_t color);
void Background_color(uint16_t color);

// void Text_Background_Color(uint8_t setR,uint8_t setG,uint8_t setB);//new 20100810
// void Text_Foreground_Color(uint8_t setR,uint8_t setG,uint8_t setB);//new 20100810
void Text_Background_Color(u16 color);



void BTE_Background_red(uint8_t setx);
void BTE_Background_green(uint8_t setx);
void BTE_Background_blue(uint8_t setx);
void BTE_Foreground_red(uint8_t setx);
void BTE_Foreground_green(uint8_t setx);
void BTE_Foreground_blue(uint8_t setx);
void Pattern_Set_number(uint8_t setx);

void Pattern_Set_8x8(void);
void Pattern_Set_16x16(void);
void Pattern_Set_number(uint8_t setx);
void BackgroundColor_TransparentMode(uint8_t setR,uint8_t setG,uint8_t setB);




void raBTEBackgroundColor(uint16_t tColor);
void raBTEForegroundColor(uint16_t tColor);

void Pattern_Set_number(uint8_t setx);

void Pattern_Set_8x8(void);
void Pattern_Set_16x16(void);
void Pattern_Set_number(uint8_t setx);
void raTransparentModeBackgroundColor(uint8_t setR,uint8_t setG,uint8_t setB);

//********TP controller*******
//REG[70h]~REG[74h]
void Enable_TP(void);
void Disable_TP(void);
void Enable_Touch_WakeUp(void);
void Disable_Touch_WakeUp(void);
void TP_manual_mode(void);
void TP_auto_mode(void);
void TP_IDLE_mode(void);
void TP_event_mode(void);
void TP_latch_X(void);
void TP_latch_Y(void);
uint8_t ADC_X(void);
uint8_t ADC_Y(void);
uint8_t ADC_XY(void);


//REG[80h]~REG[85h]
void raGraphicCursorCoordinate(uint16_t X,uint16_t Y);
void raGraphicCursorColor0(uint8_t setx);
void raGraphicCursorColor1(uint8_t setx);

//REG[88H]
//REG[89H]

//REG[8Ah]
void raPWM1Enable(void);
void raPWM1Disable(void);
void raPWM1RetainLowWhenDisabled(void);
void raPWM1RetainHighWhenDisabled(void);
void raPWM1_fnuction_sel(void);
void raPWM1_system_clk_out(void);
void raPWM1ClockDiv(uint8_t setx); //bit0~3
//REG[8Bh]
void raPWM1DutyCycle(uint8_t setx); //bit0~7

//REG[8Ch]
void raPWM2Enable(void);
void raPWM2Disable(void);
void raPWM2RetainLowWhenDisabled(void);
void raPWM2RetainHighWhenDisabled(void);
void raPWM2_fnuction_sel(void);
void raPWM2_system_clk_out(void);
void raPWM2ClockDiv(uint8_t setx); //bit0~3
//REG[8Dh]
void raPWM2DutyCycle(uint8_t setx); //bit0~7

//REG[8Eh]
void raMemoryClearStart(void);
void raMemoryClearStop(void);
void raMemoryClearFullWindow(void);
void raMemoryClearActiveWindow(void);

//REG[90h]~REG[ACh]

void raDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) ;
void raDrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,  bool tIsfill);
void raDrawCircle(uint16_t X,uint16_t Y,uint16_t R,  bool tIsfill);
void raDrawEllipse(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B,  bool tIsfill);
void raDrawEllipseCurve(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B, bool tIsfill, uint8_t tPart);
void raDrawCircleSquare(uint16_t XL,uint16_t YT ,uint16_t XR ,uint16_t YB ,uint16_t ELL_A, uint16_t ELL_B, bool tIsfill);
void raDrawTriangle(uint16_t P1_X,uint16_t P1_Y,uint16_t P2_X,uint16_t P2_Y,uint16_t P3_X, uint16_t P3_Y,   bool tIsfill);


//REG[B0]~REG[B3]
void DMA_Start_address_setting(uint32_t set_address);
//REG[B4]~REG[B8]
void DMA_Continuous_mode_size_setting(uint32_t set_size);
void DMA_block_mode_size_setting(uint16_t BWR,uint16_t BHR,uint16_t SPWR);

//REG[BFh]
void DMA_Continuous_mode(void);
void DMA_Block_mode(void);
void DMA_Start_enable(void);


//REG[C0h]
void Key_Scan_Enable(void);
void Long_Key_Scan_enable(void);
void Key_Scan_setting(uint8_t setx);//bit[5:0]
void Key_Scan_Wakeup_Function_Enable(void);
void Long_Key_Timing_Adjustment(uint8_t setx);

//REG[D0]
void Floating_window_start_point(uint16_t X,uint16_t Y);
void Floating_window_size(uint16_t width_X,uint16_t high_Y);
void Floating_window_display_point(uint16_t X,uint16_t Y);


 //REG[E0]
void Serial_ROM_direct_access_mode_enable(void);
void Serial_ROM_direct_access_mode_disable(void);
void Direct_access_mode_Address_32bits(uint32_t set_address);
void Direct_access_mode_Address_24bits(uint32_t set_address);
uint8_t Direct_access_mode_Read_Data_buffer(void);

//REG[F0h]
void Font_BTE_read_write_Interrupt_Enable(void);
void Font_BTE_read_write_Interrupt_Disable(void);

void raKeyScanInterruptEnable(void);
void raKeyScanInterruptDisable(void);
void raDMAInterruptEnable(void);
void raDMAInterruptDisable(void);
void raTouchPanelInterruptEnable(void);
void raTouchPanelInterruptDisable(void);
void raBTEFinishInterruptEnable(void);
void taBTEFinishInterruptDisable(void);

//REG[F1h]
void Clear_keyscan_interrupt(void);
uint8_t Keyscan_Status(void);

void Clear_DMA_interrupt(void);
uint8_t DMA_Status(void);

void clear_TP_interrupt(void);
uint8_t Touch_Status(void);

void Clear_BTE_interrupt(void);
uint8_t BTE_Status(void);

void Clear_Font_BTE_read_write_Interrupt(void);
uint8_t Font_BTE_read_write_Status(void);


void Chk_Wait(void);
void Show_String(char *str,uint8_t n);
/**
 * 显示字符串
 * @param str 字符串
 */
void raDisplayString(char *str);
uint8_t CheckPointRectangleCollision(Point pt, Rectangle rect);
void blADJ(uint8_t tBL);

#endif
