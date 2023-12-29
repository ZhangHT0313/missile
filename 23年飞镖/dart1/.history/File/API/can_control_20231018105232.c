#include "stm32f4xx.h"                  // Device header
#include "can_control.h"

u16 GetEncodeNumber(CanRxMsg* pCanRxMsg)
{
	u16 EncodeNumber;
	EncodeNumber = (pCanRxMsg->Data[0]<<8 | pCanRxMsg->Data[1]);
	return EncodeNumber;
}


s16 GetSpeed(CanRxMsg* pCanRxMsg)
{
	s16 Speed;
	Speed = (pCanRxMsg->Data[2]<<8 | pCanRxMsg->Data[3]);
	return Speed;
}


void CAN_SendCurrent(CAN_TypeDef* CANx, u32 id, s16 current1, s16 current2, s16 current3, s16 current4)
{
	static CanTxMsg CanTxStruc;
	CanTxStruc.StdId = id;
	CanTxStruc.IDE = CAN_Id_Standard;
	CanTxStruc.RTR = CAN_RTR_Data;
	CanTxStruc.DLC = 0x08;
	
	CanTxStruc.Data[0] = current1>>8;
	CanTxStruc.Data[1] = current1;
	CanTxStruc.Data[2] = current2>>8;
	CanTxStruc.Data[3] = current2;
	CanTxStruc.Data[4] = current3>>8;
	CanTxStruc.Data[5] = current3;	
	CanTxStruc.Data[6] = current4>>8;
	CanTxStruc.Data[7] = current4;	
	
	CAN_Transmit(CANx, &CanTxStruc);
}


void Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value)
{
	pEncoder->PreRawValue = pEncoder->RawValue;
	pEncoder->RawValue = value;
	pEncoder->Diff = pEncoder->RawValue - pEncoder->PreRawValue;
	
	if(pEncoder->Diff < -pEncoder->Number/2)												//��ת��0
		pEncoder->Diff += pEncoder->Number;						
	else if(pEncoder->Diff > pEncoder->Number/2)										//��ת��0
		pEncoder->Diff -= pEncoder->Number;		
	
	pEncoder->SumValue += pEncoder->Diff;
}


void SendAirMsgByCan1(uint32_t* pAir)
{
	static CanTxMsg TxMessage = {CAN_AIR_ID, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8,0,0,0,0,0,0,0,0};
	static uint32_t s_ucLastAir = 0;
	s_ucLastAir = *pAir;
	*((uint32_t*)TxMessage.Data) = s_ucLastAir;
	CAN_Transmit(CAN1,&TxMessage);
}


void SendAirMsgByCan2(uint32_t* pAir)
{
	static CanTxMsg TxMessage = {CAN_AIR_ID, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8,0,0,0,0,0,0,0,0};
	static uint32_t s_ucLastAir = 0;
	s_ucLastAir = *pAir;
	*((uint32_t*)TxMessage.Data) = s_ucLastAir;
	CAN_Transmit(CAN2,&TxMessage);
}