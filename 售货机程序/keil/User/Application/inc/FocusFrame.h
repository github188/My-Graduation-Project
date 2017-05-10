#ifndef __FOCUSFRAME_H
#define __FOCUSFRAME_H 

#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"

typedef struct ForceDiretion_s * PforceDirection;

typedef struct ForceDiretion_s
{
  PforceDirection left;
  PforceDirection right;
  PforceDirection up;
  PforceDirection down;
  WM_HWIN  value;
  uint16_t x;
  uint16_t y;
  GUI_BITMAP * focusP; 
}ForceDiretion_S;


extern ForceDiretion_S  * currentForce ;
extern ForceDiretion_S  button;
extern ForceDiretion_S  checkbox;
extern ForceDiretion_S  dropdown;
extern ForceDiretion_S  edit;
extern ForceDiretion_S  listbox;
extern ForceDiretion_S  listview;
extern ForceDiretion_S  radio;
extern ForceDiretion_S  scroll;
extern ForceDiretion_S  slider;
extern ForceDiretion_S  spinbox;

/**
 * 改变聚焦框的颜色
 * @param tBitMap 需要改变的聚焦框的bitmap
 * @param tNum    颜色序号
 */
void focusChangeFrameColor(GUI_BITMAP * tBitMap, uint16_t tNum);
//整个框
void focusWholeFrameInit( GUI_BITMAP * tBitMap, uint16_t tLength, uint16_t tWidth);
/**
 * 向上的小半个聚焦框
 * @param tBitMap 返回的聚焦框的bitmap
 * @param tLength 长度
 */
void focusUpFrameInit( GUI_BITMAP * tBitMap, uint16_t tLength);

/**
 * 向下的小半个聚焦框
 * @param tBitMap 返回的聚焦框的bitmap
 * @param tXSize 控件的长度
 */
void focusDownFrameInit( GUI_BITMAP * tBitMap, uint16_t tXSize);

/**
 * 改变聚焦框的颜色
 * @param tBitMap 需要改变的聚焦框的bitmap
 * @param tNum    颜色序号
 */
void focusChangeFrameColor(GUI_BITMAP * tBitMap, uint16_t tNum);

/**
 * 初始化个元素的方向和值
 * @param tCreateInfo 设定的值
 * @param tDir        需要初始化的元素
 * @param tItem       元素在WM中的句柄
 * @param tUp         上方向的值
 * @param tDown       下方向的值
 * @param tLeft       左方向的值
 * @param tRight      右方向的值
 */
void focusDirectionInit(GUI_WIDGET_CREATE_INFO tCreateInfo, ForceDiretion_S * tDir,WM_HWIN tItem , ForceDiretion_S * tUp, ForceDiretion_S * tDown, ForceDiretion_S * tLeft, ForceDiretion_S * tRight);

/**
 * 失去焦点
 */
void focusDoLose(void);
#endif

