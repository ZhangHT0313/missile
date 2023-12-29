#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "global.h"
#include "hitcrt_types.h"

extern int CANID;


void CAN2_Configuration(void);
void CAN1_Configuration(void);

#endif

















