#include "GTFont.h"

#include "LCD.h"
extern GUI_SADDR GUI_CONTEXT * GUI_pContext;


/**
 * 24位BBGGRR转16为RRRRRGGGGGGBBBBB
 * @param  color 颜色
 * @return       返回16位颜色
 */
uint16_t prvColorCovert(uint32_t color)
{
    uint32_t R,G,B;
    B = (color&0xF80000) >> 19;
    G = (color&0x00FC00) >> 10;
    R = (color&0x0000F8) >>3;

    R = R << 11;
    G = G << 5;

    return((uint16_t)R+G+B);
}

void GTFont_15X16_8x16_HZ_GuiDispChar(U16 c)
{
}

int GTFont_15X16_8x16_HZ_GuiGetCharDispX(U16 t, int * pSizeX)
{
    *pSizeX = 16;    //返回实际字符宽度，也可以返回0
    return 8;
}

void GTFont_15X16_8x16_HZ_GuiGetFontInfo(const GUI_FONT GUI_UNI_PTR * pFont,  GUI_FONTINFO * pfi)
{//如果不调用GUI_GetFontInfo，可以为空函数
  GUI_USE_PARA(pFont);
    pfi->Flags = GUI_FONTINFO_FLAG_PROP;
}

char GTFont_15X16_8x16_HZ_GuiIsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c)
{//如果不调用GUI_IsInFont，可以为空函数
    const GUI_FONT_PROP GUI_UNI_PTR* pProp = (*GUI_pContext).pAFont->p.pProp;
    if((c >= pProp->First) && (c <= pProp->Last)) return (1);
    return (0);
}

int GTFont_15X16_8x16_HZ_GuiGetCharInfo(U16 c, GUI_CHARINFO_EXT * pInfo)
{
   return 0;
}

int GTFont_15X16_8x16_HZ_GuiGetLineDistX(const char  GUI_UNI_PTR *s, int Len)
{
int Dist =0;
  if (s) {
    U8 c0;
    while (((c0=*(const U8*)s) !=0) && Len >=0) {
      s++; Len--;
      if (c0 > 127) {
        U8  c1 = *(const U8*)s++;
        Len--;
        Dist +=16;
      } else {
        Dist += 8;
      }
    }
  }
  return Dist;
    // return (Len*8);
}

int GTFont_15X16_8x16_HZ_GuiGetLineLen(const char GUI_UNI_PTR *s, int MaxLen)
{
    // return (strlen(s));
      int Len =0;
  U8 c0;
  while (((c0=*(const U8*)s) !=0) && Len < MaxLen) {
    s++;
    if (c0 > 127) {
      Len++; s++;
    } else {
      switch (c0) {
      case '\n': return Len;
      }
    }
    Len++;
  }
  return Len;
}


    
void GTFont_15X16_8x16_HZ_GuiDispLine( char GUI_UNI_PTR *s, int Len)
{//在这个函数里面实现你自己的字符显示函数
    if((*GUI_pContext).TextMode == GUI_TEXTMODE_TRANS)
       LcdDisplayStringZH(s, (*GUI_pContext).DispPosX, (*GUI_pContext).DispPosY, prvColorCovert((*GUI_pContext).Color),prvColorCovert((*GUI_pContext).BkColor), true,  GB2312_15X16_8x16_HZ);
   else
	   LcdDisplayStringZH(s, (*GUI_pContext).DispPosX, (*GUI_pContext).DispPosY, prvColorCovert((*GUI_pContext).Color),prvColorCovert((*GUI_pContext).BkColor), false,  GB2312_15X16_8x16_HZ);
}

static const tGUI_ENC_APIList GTFont_15X16_8x16_HZ_API_LIST = 
{
     GTFont_15X16_8x16_HZ_GuiGetLineDistX,
     GTFont_15X16_8x16_HZ_GuiGetLineLen,
     GTFont_15X16_8x16_HZ_GuiDispLine,
};



GUI_FONT GTFont_15X16_8x16_HZ= 
{
     GTFont_15X16_8x16_HZ_GuiDispChar,
     GTFont_15X16_8x16_HZ_GuiGetCharDispX,
     GTFont_15X16_8x16_HZ_GuiGetFontInfo,
     GTFont_15X16_8x16_HZ_GuiIsInFont,
     GTFont_15X16_8x16_HZ_GuiGetCharInfo,
     &GTFont_15X16_8x16_HZ_API_LIST,
     16, 16, 1, 1,
      0, 
      14, 8, 12
};

/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
void GTFont_24x24_12x24_HZ_GuiDispChar(U16 c)
{
    
}

int GTFont_24x24_12x24_HZ_GuiGetCharDispX(U16 t, int * pSizeX)
{
    *pSizeX = 24;    //返回实际字符宽度，也可以返回0
    return 12;
}

void GTFont_24x24_12x24_HZ_GuiGetFontInfo(const GUI_FONT GUI_UNI_PTR * pFont,  GUI_FONTINFO * pfi)
{//如果不调用GUI_GetFontInfo，可以为空函数
  GUI_USE_PARA(pFont);
    pfi->Flags = GUI_FONTINFO_FLAG_PROP;
}

char GTFont_24x24_12x24_HZ_GuiIsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c)
{//如果不调用GUI_IsInFont，可以为空函数
    const GUI_FONT_PROP GUI_UNI_PTR* pProp = (*GUI_pContext).pAFont->p.pProp;
    if((c >= pProp->First) && (c <= pProp->Last)) return (1);
    return (0);
}

int GTFont_24x24_12x24_HZ_GuiGetCharInfo(U16 c, GUI_CHARINFO_EXT * pInfo)
{
   return 0;
}

int GTFont_24x24_12x24_HZ_GuiGetLineDistX(const char  GUI_UNI_PTR *s, int Len)
{
    int Dist =0;
  if (s) {
    U8 c0;
    while (((c0=*(const U8*)s) !=0) && Len >=0) {
      s++; Len--;
      if (c0 > 127) {
        U8  c1 = *(const U8*)s++;
        Len--;
        Dist +=24;
      } else {
        Dist += 12;
      }
    }
  }
  return Dist;
    // return (Len);
}

int GTFont_24x24_12x24_HZ_GuiGetLineLen(const char GUI_UNI_PTR *s, int MaxLen)
{
    // return (strlen(s));
  int Len =0;
  U8 c0;
  while (((c0=*(const U8*)s) !=0) && Len < MaxLen) {
    s++;
    if (c0 > 127) {
      Len++; s++;
    } else {
      switch (c0) {
      case '\n': return Len;
      }
    }
    Len++;
  }
  return Len;
}

void GTFont_24x24_12x24_HZ_GuiDispLine( char GUI_UNI_PTR *s, int Len)
{//在这个函数里面实现你自己的字符显示函数
    if((*GUI_pContext).TextMode == GUI_TEXTMODE_TRANS)
       LcdDisplayStringZH(s, (*GUI_pContext).DispPosX, (*GUI_pContext).DispPosY, prvColorCovert((*GUI_pContext).Color),prvColorCovert((*GUI_pContext).BkColor), true,  GB2312_24x24_12x24_HZ);
   else
       LcdDisplayStringZH(s, (*GUI_pContext).DispPosX, (*GUI_pContext).DispPosY, prvColorCovert((*GUI_pContext).Color),prvColorCovert((*GUI_pContext).BkColor), false,  GB2312_24x24_12x24_HZ);
}

static const tGUI_ENC_APIList GTFont_24x24_12x24_HZ_API_LIST = 
{
     GTFont_24x24_12x24_HZ_GuiGetLineDistX,
     GTFont_24x24_12x24_HZ_GuiGetLineLen,
     GTFont_24x24_12x24_HZ_GuiDispLine,
};

GUI_FONT GTFont_24x24_12x24_HZ= 
{
    GTFont_24x24_12x24_HZ_GuiDispChar,
    GTFont_24x24_12x24_HZ_GuiGetCharDispX,
    GTFont_24x24_12x24_HZ_GuiGetFontInfo,
    GTFont_24x24_12x24_HZ_GuiIsInFont,
    GTFont_24x24_12x24_HZ_GuiGetCharInfo,
    &GTFont_24x24_12x24_HZ_API_LIST,
    24, 24, 1, 1,
     0, 
     14, 14, 19
};

/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
void GTFont_32X32_16x32_HZ_GuiDispChar(U16 c)
{
    // LcdDisplayStringZH("你", 100, 100, 0, 0xffff);
    
}

int GTFont_32X32_16x32_HZ_GuiGetCharDispX(U16 t, int * pSizeX)
{
    *pSizeX = 32;    //返回实际字符宽度，也可以返回0
    return 16;
}

void GTFont_32X32_16x32_HZ_GuiGetFontInfo(const GUI_FONT GUI_UNI_PTR * pFont,  GUI_FONTINFO * pfi)
{//如果不调用GUI_GetFontInfo，可以为空函数
  GUI_USE_PARA(pFont);
    pfi->Flags = GUI_FONTINFO_FLAG_PROP;
}

char GTFont_32X32_16x32_HZ_GuiIsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c)
{//如果不调用GUI_IsInFont，可以为空函数
   const GUI_FONT_PROP GUI_UNI_PTR* pProp = (*GUI_pContext).pAFont->p.pProp;
    if((c >= pProp->First) && (c <= pProp->Last)) return (1);
    return (0);
}

int GTFont_32X32_16x32_HZ_GuiGetCharInfo(U16 c, GUI_CHARINFO_EXT * pInfo)
{
   return 0;
}

int GTFont_32X32_16x32_HZ_GuiGetLineDistX(const char  GUI_UNI_PTR *s, int Len)
{
    // return (Len);
    int Dist =0;
  if (s) {
    U8 c0;
    while (((c0=*(const U8*)s) !=0) && Len >=0) {
      s++; Len--;
      if (c0 > 127) {
        U8  c1 = *(const U8*)s++;
        Len--;
        Dist +=32;
      } else {
        Dist += 16;
      }
    }
  }
  return Dist;
}

int GTFont_32X32_16x32_HZ_GuiGetLineLen(const char GUI_UNI_PTR *s, int MaxLen)
{
    // return (strlen(s));
  int Len =0;
  U8 c0;
  while (((c0=*(const U8*)s) !=0) && Len < MaxLen) {
    s++;
    if (c0 > 127) {
      Len++; s++;
    } else {
      switch (c0) {
      case '\n': return Len;
      }
    }
    Len++;
  }
  return Len;
}

void GTFont_32X32_16x32_HZ_GuiDispLine( char GUI_UNI_PTR *s, int Len)
{//在这个函数里面实现你自己的字符显示函数
     if((*GUI_pContext).TextMode == GUI_TEXTMODE_TRANS)
       LcdDisplayStringZH(s, (*GUI_pContext).DispPosX, (*GUI_pContext).DispPosY, prvColorCovert((*GUI_pContext).Color),prvColorCovert((*GUI_pContext).BkColor), true,  GB2312_32X32_16x32_HZ);
   else
       LcdDisplayStringZH(s, (*GUI_pContext).DispPosX, (*GUI_pContext).DispPosY, prvColorCovert((*GUI_pContext).Color),prvColorCovert((*GUI_pContext).BkColor), false,  GB2312_32X32_16x32_HZ);
}

static const tGUI_ENC_APIList GTFont_32X32_16x32_HZ_API_LIST = 
{
     GTFont_32X32_16x32_HZ_GuiGetLineDistX,
     GTFont_32X32_16x32_HZ_GuiGetLineLen,
     GTFont_32X32_16x32_HZ_GuiDispLine,
};
GUI_FONT GTFont_32X32_16x32_HZ= 
{
    GTFont_32X32_16x32_HZ_GuiDispChar,
    GTFont_32X32_16x32_HZ_GuiGetCharDispX,
    GTFont_32X32_16x32_HZ_GuiGetFontInfo,
    GTFont_32X32_16x32_HZ_GuiIsInFont,
    GTFont_32X32_16x32_HZ_GuiGetCharInfo,
    &GTFont_32X32_16x32_HZ_API_LIST,
    32, 32, 1, 1,
     0, 
     14, 20, 26
};
