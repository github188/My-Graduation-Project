#ifndef __ICAN_H
#define __ICAN_H 
#include <stdint.h>
#include "stm32f4xx.h"

#define CANMACID 0

typedef struct 
{
	uint8_t SrcMACID;
	uint8_t DestMACID;
	uint8_t ACK;
	uint8_t FuncID;
	uint8_t SourceID;
	uint8_t Data[8];
}iCAN_MSG_S;

/**
 * ??iCAN??
 * @param CANx     CAN1?CAN2
 * @param tiCANmsg ???iCAN?????
 */
void iCANSendMsg(CAN_TypeDef* CANx, iCAN_MSG_S * tiCANmsg);

/**
 * iCAN????
 * @param CANx     CAN1?CAN2
 * @param tiCANmsg ????iCAN?????
 */
void iCANReceiveMsg(CAN_TypeDef * CANx, iCAN_MSG_S * tiCANmsg);
#endif
