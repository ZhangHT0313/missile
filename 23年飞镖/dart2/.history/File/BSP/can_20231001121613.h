#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

extern uint32_t g_uiAirValve;
extern int CANID;

#define OPEN_VALVE(channel) if(1){(g_uiAirValve |= (0x01 << (channel-1))); SendAirMsgByCan2(&g_uiAirValve);}  //���׿���
#define CLOSE_VALVE(channel) if(1){g_uiAirValve &= (~(0x01 << (channel-1))); SendAirMsgByCan2(&g_uiAirValve);} //���׹ر�
#define CAN_AIR_ID 0X30

typedef struct ST_ENCODER
{
	u32 RawValue;						//���α����ԭʼֵ
	u32 PreRawValue;					//��һ�α�������ԭʼֵ
	s32 Diff;							//����������ԭʼ����ֵ
	s32 Number;							//����������
	s32 SumValue;						//�������ۼ�ֵ						
}ST_ENCODER;

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

















