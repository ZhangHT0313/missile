#ifndef __CAN_CONTROL_H__
#define __CAN_CONTROL_H__
#include "hitcrt_types.h"
#include "global.h"
#include "stm32f4xx_can.h"
#include "sys.h"

#define OPEN_VALVE(channel) if(1){(g_uiAirValve |= (0x01 << (channel-1))); SendAirMsgByCan2(&g_uiAirValve);}  //���׿���
#define CLOSE_VALVE(channel) if(1){g_uiAirValve &= (~(0x01 << (channel-1))); SendAirMsgByCan2(&g_uiAirValve);} //���׹ر�
#define CAN_AIR_ID 0X30

u8 CAN_Send_4015(CAN_TypeDef *CANx,u32 id,s16 data1,s16 data2,s16 data3,s16 data4);
s16 Get_Speed_4015(CanRxMsg* rx_message);
s16 Get_Pos_4015(CanRxMsg* rx_message);

void SendAirMsgByCan1(uint32_t* pAir);
void SendAirMsgByCan2(uint32_t* pAir);
u16 GetEncodeNumber(CanRxMsg* pCanRxMsg);
void Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value);
float Get_Temperature(CanRxMsg* rx_message);
s16 GetSpeed(CanRxMsg* pCanRxMsg);
void CAN_SendCurrent(CAN_TypeDef* CANx, u32 id, s16 current1, s16 current2, s16 current3, s16 current4);

u8 CAN1_Send_Msg(u8* msg,u8 len);
u8 CAN1_Receive_Msg(u8 *buf);

#endif
