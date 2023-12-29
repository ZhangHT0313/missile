#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "global.h"
#include "hitcrt_types.h"

extern int CANID;

#define OPEN_VALVE(channel) if(1){(g_uiAirValve |= (0x01 << (channel-1))); SendAirMsgByCan2(&g_uiAirValve);}  //���׿���
#define CLOSE_VALVE(channel) if(1){g_uiAirValve &= (~(0x01 << (channel-1))); SendAirMsgByCan2(&g_uiAirValve);} //���׹ر�
#define CAN_AIR_ID 0X30




void CAN_Configuration(void);
u8 CAN1_Send_Msg(u8* msg,u8 len);
u8 CAN1_Receive_Msg(u8 *buf);
void CAN2_RX0_IRQHandler(void);

void SendAirMsgByCan1(uint32_t* pAir);
void SendAirMsgByCan2(uint32_t* pAir);
u16 GetEncodeNumber(CanRxMsg* pCanRxMsg);
void Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value);

s16 GetSpeed(CanRxMsg* pCanRxMsg);
void CAN_SendCurrent(CAN_TypeDef* CANx, u32 id, s16 current1, s16 current2, s16 current3, s16 current4);



#endif

















