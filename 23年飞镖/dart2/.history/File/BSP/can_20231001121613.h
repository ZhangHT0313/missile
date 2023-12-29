#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

extern uint32_t g_uiAirValve;
extern int CANID;

#define OPEN_VALVE(channel) if(1){(g_uiAirValve |= (0x01 << (channel-1))); SendAirMsgByCan2(&g_uiAirValve);}  //气缸开启
#define CLOSE_VALVE(channel) if(1){g_uiAirValve &= (~(0x01 << (channel-1))); SendAirMsgByCan2(&g_uiAirValve);} //气缸关闭
#define CAN_AIR_ID 0X30

typedef struct ST_ENCODER
{
	u32 RawValue;						//本次编码的原始值
	u32 PreRawValue;					//上一次编码器的原始值
	s32 Diff;							//编码器两次原始量差值
	s32 Number;							//编码器线数
	s32 SumValue;						//编码器累加值						
}ST_ENCODER;

extern int16_t current_lift;     	//电机的传入电流值
extern int16_t current_clip;
extern int16_t current_rotate;

extern ST_ENCODER encode1,encode12,encode3,encode4;     //电机编码器的结构体       
extern ST_ENCODER encode5,encode6,encode7,encode8;     //电机编码器的结构体                               
extern ST_ENCODER encode9,encodeA,encodeB,encode2;     			//电机编码器的结构体                               

extern u16 value1, value12, value3,value4;              //电机的编码器的值
extern u16 value5, value6, value7,value8;              //电机的编码器的值
extern u16 value9, valueA, valueB,value2;              		//电机的编码器的值
extern s16 speed1, speed12, speed3, speed4;             //电机的速度值
extern s16 speed5, speed6, speed7, speed8;             //电机的速度值
extern s16 speed9, speedA, speedB,speed2;             			//电机的速度值

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

















