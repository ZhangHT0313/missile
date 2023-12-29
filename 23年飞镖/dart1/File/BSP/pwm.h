#ifndef __PWM_H__
#define __PWM_H__
#include "global.h"

void TIM3_PWM_Init(u32 arr,u32 psc);
void TIM9_PWM_Init(u32 arr,u32 psc);
void TIM8_PWM_Init(u32 arr,u32 psc);

#endif
