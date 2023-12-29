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



/*
 *�������ܣ�can1ͨ���жϷ�����
 *��ڲ�����NONE
 *���ڲ�����NONE
*/
void CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)		//��ȡ�����жϱ�־λ
	{
//		CanRxMsg CanRxStruc1;
//		CanRxMsg CanRxStruc2;
		CanRxMsg CanRxStruc;
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);		//����жϱ�־λ
		CAN_Receive(CAN1,CAN_FIFO0,&CanRxStruc);
//		CAN_Receive(CAN1,CAN_FIFO0,&CanRxStruc1);		//��FIFO�н�����ϢCanRxStruc
//		CAN_Receive(CAN1,CAN_FIFO0,&CanRxStruc2);		//��FIFO�н�����ϢCanRxStruc
//		if (CanRxStruc1.StdId == 0x201)
//		{
//			value1 = GetEncodeNumber(&CanRxStruc1);
//				speed1 = GetSpeed(&CanRxStruc1) ;
//				Abs_Encoder_Process(&encode1,value1);
//		}
//		if (CanRxStruc2.StdId == 0x202)
//		{
//			value12 = GetEncodeNumber(&CanRxStruc2);
//				speed12 = GetSpeed(&CanRxStruc2);
//				Abs_Encoder_Process(&encode12,value12);
//		}
		switch(CanRxStruc.StdId)											//����id�����洢���ĸ���Ӧ�Ľṹ��
		{
			//���ID��620��Ӧ1�ţ�6020�޶�Ӧ
			case 0x201:
				value1 = GetEncodeNumber(&CanRxStruc);
				speed1 = GetSpeed(&CanRxStruc) ;
				Abs_Encoder_Process(&encode1,value1);
				break;
			//���ID��620��Ӧ2�ţ�6020�޶�Ӧ
			case 0x202:
				value2 = GetEncodeNumber(&CanRxStruc);
				speed2 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode2,value2);			
				break;
			//���ID��620��Ӧ3�ţ�6020�޶�Ӧ
			case 0x203:
				value3 = GetEncodeNumber(&CanRxStruc);
				speed3 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode3,value3);
				break;
			//���ID��620��Ӧ4�ţ�6020�޶�Ӧ
			case 0x204:
				value4 = GetEncodeNumber(&CanRxStruc);
				speed4 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode4,value4);
				break;
			//���ID��620��Ӧ5�ţ�6020��Ӧ1��
			case 0x205:
				value5 = GetEncodeNumber(&CanRxStruc);
				speed5 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode5,value5);
				break;
			//���ID��620��Ӧ6�ţ�6020��Ӧ2��
			case 0x206:
				value6 = GetEncodeNumber(&CanRxStruc);
				speed6 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode6,value6);
				break;
			//���ID��620��Ӧ7�ţ�6020��Ӧ3��
			case 0x207:
				value7 = GetEncodeNumber(&CanRxStruc);
				speed7 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode7,value7);
				break;
			//���ID��620��Ӧ8�ţ�6020��Ӧ4��
			case 0x208:
				value8 = GetEncodeNumber(&CanRxStruc);
				speed8 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode8,value8);
				break;
			//���ID��620�޶�Ӧ��6020��Ӧ5��
			case 0x209:
				value9 = GetEncodeNumber(&CanRxStruc);
				speed9 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode9,value9);
				break;
			//���ID��620�޶�Ӧ��6020��Ӧ6��
			case 0x20A:
				valueA = GetEncodeNumber(&CanRxStruc);
				speedA = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encodeA,valueA);
				break;
			//���ID��620�޶�Ӧ��6020��Ӧ7��
			case 0x20B:
				valueB = GetEncodeNumber(&CanRxStruc);
				speedB = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encodeB,valueB);
				break;
			default:
				break;
		}
	}
}


void CAN2_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=RESET)		//��ȡ�����жϱ�־λ
	{
		CanRxMsg CanRxStruc;
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);			//����жϱ�־λ
		CAN_Receive(CAN2,CAN_FIFO0,&CanRxStruc);			//��FIFO�н�����ϢCanRxStruc
		switch(CanRxStruc.StdId)											//����id�����洢���ĸ���Ӧ�Ľṹ��
		{
			//���ID��620��Ӧ1�ţ�6020�޶�Ӧ
			case 0x201:
				value1 = GetEncodeNumber(&CanRxStruc);
				speed1 = GetSpeed(&CanRxStruc) ;
				Abs_Encoder_Process(&encode1,value1);
				break;
			//���ID��620��Ӧ2�ţ�6020�޶�Ӧ
			case 0x202:
				value2 = GetEncodeNumber(&CanRxStruc);
				speed2 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode2,value2);			
				break;
			//���ID��620��Ӧ3�ţ�6020�޶�Ӧ
			case 0x203:
				value3 = GetEncodeNumber(&CanRxStruc);
				speed3 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode3,value3);
				break;
			//���ID��620��Ӧ4�ţ�6020�޶�Ӧ
			case 0x204:
				value4 = GetEncodeNumber(&CanRxStruc);
				speed4 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode4,value4);
				break;
			//���ID��620��Ӧ5�ţ�6020��Ӧ1��
			case 0x205:
				value5 = GetEncodeNumber(&CanRxStruc);
				speed5 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode5,value5);
				break;
			//���ID��620��Ӧ6�ţ�6020��Ӧ2��
			case 0x206:
				value6 = GetEncodeNumber(&CanRxStruc);
				speed6 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode6,value6);
				break;
			//���ID��620��Ӧ7�ţ�6020��Ӧ3��
			case 0x207:
				value7 = GetEncodeNumber(&CanRxStruc);
				speed7 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode7,value7);
				break;
			//���ID��620��Ӧ8�ţ�6020��Ӧ4��
			case 0x208:
				value8 = GetEncodeNumber(&CanRxStruc);
				speed8 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode8,value8);
				break;
			//���ID��620�޶�Ӧ��6020��Ӧ5��
			case 0x209:
				value9 = GetEncodeNumber(&CanRxStruc);
				speed9 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode9,value9);
				break;
			//���ID��620�޶�Ӧ��6020��Ӧ6��
			case 0x20A:
				valueA = GetEncodeNumber(&CanRxStruc);
				speedA = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encodeA,valueA);
				break;
			//���ID��620�޶�Ӧ��6020��Ӧ7��
			case 0x20B:
				valueB = GetEncodeNumber(&CanRxStruc);
				speedB = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encodeB,valueB);
				break;
			default:
				break;
		}
	}
}
