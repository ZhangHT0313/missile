#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "stm32f4xx.h"                  // Device header
#include "hitcrt_types.h"

extern SystemMonitor_struct ledmonitor;
extern uint16_t ledpwm_counter;

extern uint32_t g_uiAirValve;

extern int16_t current_lift;     	//����Ĵ������ֵ
extern int16_t current_clip;
extern int16_t current_rotate;

extern ST_ENCODER encode1,encode12,encode3,encode4;     //����������Ľṹ��       
extern ST_ENCODER encode5,encode6,encode7,encode8;     //����������Ľṹ��                               
extern ST_ENCODER encode9,encodeA,encodeB,encode2;     			//����������Ľṹ��                               

extern u16 value1, value12, value3,value4;              //����ı�������ֵ
extern u16 value5, value6, value7,value8;              //����ı�������ֵ
extern u16 value9, valueA, valueB,value2;              		//����ı�������ֵ
extern s16 speed1, speed12, speed3, speed4;             //������ٶ�ֵ
extern s16 speed5, speed6, speed7, speed8;             //������ٶ�ֵ
extern s16 speed9, speedA, speedB,speed2;             			//������ٶ�ֵ

#endif
