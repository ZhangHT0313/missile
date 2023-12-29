#include "stm32f4xx.h"                  // Device header
#include "can_control.h"

/*瓴控电机  4015*/
//can发送（4015）
u8 CAN_Send_4015(CAN_TypeDef *CANx,u32 id,s16 data1,s16 data2,s16 data3,s16 data4)
{
    CanTxMsg tx_message;
    tx_message.StdId = id;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
	
    tx_message.Data[1] = (u8)(data1 >> 8);
    tx_message.Data[0] = (u8)data1;
    tx_message.Data[3] = (u8)(data2 >> 8);
    tx_message.Data[2] = (u8)data2;
    tx_message.Data[5] = (u8)(data3 >> 8);
    tx_message.Data[4] = (u8)data3;
    tx_message.Data[7] = (u8)(data4 >> 8);
    tx_message.Data[6] = (u8)data4;
    return CAN_Transmit(CANx,&tx_message);
}

s16 Get_Speed_4015(CanRxMsg* rx_message)
{
    s16 speed_temp;
    s16 base_value=0xFFFF;
    if(rx_message->Data[5] & 0x01<<7)
    {
        speed_temp = (base_value<<16 | rx_message->Data[5]<<8 | rx_message->Data[4]);
    }
    else
    {
        speed_temp = (rx_message->Data[5]<<8 | rx_message->Data[4]);//rpm
    }
    return speed_temp;
}

s16 Get_Pos_4015(CanRxMsg* rx_message)
{
    s16 pos_temp;
    s16 base_value=0xFFFF;
    if(rx_message->Data[7] & 0x01<<7)
    {
        pos_temp = (base_value<<16 | rx_message->Data[7]<<8 | rx_message->Data[6]);
//			  FbCurAngel = ( pos_temp+32725)/65535*360;///65535*360;
    }
    else
    {
        pos_temp = (rx_message->Data[7]<<8 | rx_message->Data[6]);//rpm
    }
    return pos_temp;
}



/*************************************************************************
函 数 名：Get_Temperature
函数功能：接收3508电调板返回的电机温度值
备    注：单位 摄氏度
*************************************************************************/
float Get_Temperature(CanRxMsg* rx_message)
{
    float temperature_temp;
	temperature_temp = rx_message->Data[6];
	return temperature_temp;
}




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



u8 CAN1_Send_Msg(u8* msg,u8 len)
{
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x12;
	TxMessage.ExtId = 0x12;
	TxMessage.IDE = 0;
	TxMessage.RTR = 0;
	TxMessage.DLC = len;
	for(i=0 ; i<len ;i++)
	TxMessage.Data[i] = msg[i];                 
	mbox = CAN_Transmit(CAN1, &TxMessage);
	i=0;
	while((CAN_TransmitStatus(CAN1 , mbox)==CAN_TxStatus_Failed)&&(i<0xFFF))i++;                        //�ȴ����ͽ���
	if(i>=0xFFF)return 1 ;
	return 0;
	
}


u8 CAN1_Receive_Msg(u8 *buf)
{
	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0) return 0;      //û�յ����ݣ�ֱ���˳�
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);        //��ȡ����
	for(i=0; i<RxMessage.DLC; i++)
	buf[i]=RxMessage.Data[i];
	return RxMessage.DLC;
	
}


