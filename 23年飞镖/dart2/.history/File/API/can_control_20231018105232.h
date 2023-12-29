#ifndef __CAN_CONTROL_H__
#define __CAN_CONTROL_H__
#include "hitcrt_types.h"
#include "global.h"
#include "stm32f4xx_can.h"
#include "sys.h"


void SendAirMsgByCan1(uint32_t* pAir);
void SendAirMsgByCan2(uint32_t* pAir);
u16 GetEncodeNumber(CanRxMsg* pCanRxMsg);
void Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value);

s16 GetSpeed(CanRxMsg* pCanRxMsg);
void CAN_SendCurrent(CAN_TypeDef* CANx, u32 id, s16 current1, s16 current2, s16 current3, s16 current4);

#endif
