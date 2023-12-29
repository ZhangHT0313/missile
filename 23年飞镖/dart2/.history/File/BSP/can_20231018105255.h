#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "global.h"
#include "hitcrt_types.h"

extern int CANID;





void CAN_Configuration(void);
u8 CAN1_Send_Msg(u8* msg,u8 len);
u8 CAN1_Receive_Msg(u8 *buf);
void CAN2_RX0_IRQHandler(void);




#endif

















