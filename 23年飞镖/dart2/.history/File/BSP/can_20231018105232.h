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




#endif

















