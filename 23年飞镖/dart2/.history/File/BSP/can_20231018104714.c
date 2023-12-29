#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"


void CAN_Configuration(void)																											//CAN�ײ�����
{
	CAN_InitTypeDef CAN_InitStructure;																							//�ṹ�嶨��			
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterStructure;
	NVIC_InitTypeDef NVIC_InitStructure1;
	NVIC_InitTypeDef NVIC_InitStructure2;
	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2,ENABLE);			//ʹ��CANʱ�ӣ�CAN2�Ǵ�CAN��ʹ��ǰ����ʹ��CAN1��ʼʱ�ӣ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);		//ʹ��GPIOʱ��
	//���ų�ʼ��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;																		//GPIO��ʼ��
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;                       //��ʼ��5 ,6 ,8��9����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6;                       //��ʼ��5��6����
//	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	//���Ÿ���
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);												//���ø���
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2);
	//CAN��ʼ��
	CAN_InitStructure.CAN_TTCM=DISABLE;	                                       //�¼�����	
	CAN_InitStructure.CAN_ABOM=ENABLE;				                           //�Զ����߹���																		
	CAN_InitStructure.CAN_AWUM=ENABLE;                                         //�Զ�����
	CAN_InitStructure.CAN_NART=DISABLE;	                                       //�����ش�
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;                                                             //CAN��ʼ��
	
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;	
	CAN_InitStructure.CAN_Prescaler=3;                             //�������ã�42MHz/3/(1+9+4)=1MHz
	CAN_Init(CAN1,&CAN_InitStructure);
	CAN_Init(CAN2,&CAN_InitStructure);
	//ɸѡ����ʼ��
	CAN_FilterStructure.CAN_FilterNumber=0;
	CAN_FilterStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterStructure.CAN_FilterMaskIdHigh=0x0000;                  //������0��ɸѡ
	CAN_FilterStructure.CAN_FilterMaskIdLow=0x0000;	
	CAN_FilterStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterStructure);
	CAN_FilterStructure.CAN_FilterNumber=14;
	CAN_FilterInit(&CAN_FilterStructure);	
	//�жϳ�ʼ��+ʹ��
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                                        //ʹ��CAN_IT�ж�
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	
	NVIC_InitStructure1.NVIC_IRQChannel=CAN1_RX0_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure1.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
	
	NVIC_InitStructure2.NVIC_IRQChannel=CAN2_RX0_IRQn;
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure2.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure2);
	
}

/*
 *�������ܣ�canͨ�ŷ�������
 *��ڲ�����msg��Ҫ���͵�����      len���������ݵĳ���
 *���ڲ�����1 -> �ɹ�����      0 -> δ�ɹ�����
*/
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

/*
 *�������ܣ�canͨ�Ž�������
 *��ڲ�����buf���������ݴ洢������
 *���ڲ�����RxMessage.DLC
*/
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

/*
 *�������ܣ�can2ͨ���жϷ�����
 *��ڲ�����NONE
 *���ڲ�����NONE
*/
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


/*
 *�������ܣ���ȡ�����������ֵ
 *��ڲ�����CanRxMsg�ĵ�ַ
 *���ڲ�����EncodeNumber�������������ֵ
 *����˵�������������canͨѶ���жϷ�������ʹ�ù�
*/
u16 GetEncodeNumber(CanRxMsg* pCanRxMsg)
{
	u16 EncodeNumber;
	EncodeNumber = (pCanRxMsg->Data[0]<<8 | pCanRxMsg->Data[1]);
	return EncodeNumber;
}

/*
 *�������ܣ���ȡ���ת��
 *��ڲ�����CanRxMsg�ĵ�ַ
 *���ڲ�����Speed�����ת��
 *����˵�������������canͨѶ���жϷ�������ʹ�ù�
*/
s16 GetSpeed(CanRxMsg* pCanRxMsg)
{
	s16 Speed;
	Speed = (pCanRxMsg->Data[2]<<8 | pCanRxMsg->Data[3]);
	return Speed;
}

/*
 *�������ܣ�ͨ��canͨѶ���͵���ֵ�������
 *��ڲ�����CANx��ʹ�õ�canͨѶ�ţ�can1��can2�ȣ���id�����id�ţ�current1�ȣ��ܿص��Ҫ����ĵ�����GM6020Ϊ��ѹ��ֵ��
 *���ڲ�����NONE
 *����˵����C620�е�current����Ϊ100���ܿ������ת������GM6020��������Ҫ1000���ܿ������ת��
*/
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

/*
 *�������ܣ��������������
 *��ڲ�����ST_ENCODER* pEncoder��ST_ENCODER�ṹ��ĵ�ַ��value����ȡ���ĵ����������ֵ
 *���ڲ�����NONE
 *����˵�����˺�����canͨ�ŵ��жϷ������е��ù�
*/
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

/*
 *�������ܣ�can1�����׷���
 *��ڲ�����uint32_t* pAir
 *���ڲ�����NONE
 *����˵�������������Ѿ�����O_VALVE��C_VALVE�Ըú��������˼򻯣������
*/
void SendAirMsgByCan1(uint32_t* pAir)
{
	static CanTxMsg TxMessage = {CAN_AIR_ID, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8,0,0,0,0,0,0,0,0};
	static uint32_t s_ucLastAir = 0;
	s_ucLastAir = *pAir;
	*((uint32_t*)TxMessage.Data) = s_ucLastAir;
	CAN_Transmit(CAN1,&TxMessage);
}

/*
 *�������ܣ�can2�����׷���
 *��ڲ�����uint32_t* pAir
 *���ڲ�����NONE
 *����˵�������������Ѿ�����O_VALVE��C_VALVE�Ըú��������˼򻯣������
*/
void SendAirMsgByCan2(uint32_t* pAir)
{
	static CanTxMsg TxMessage = {CAN_AIR_ID, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8,0,0,0,0,0,0,0,0};
	static uint32_t s_ucLastAir = 0;
	s_ucLastAir = *pAir;
	*((uint32_t*)TxMessage.Data) = s_ucLastAir;
	CAN_Transmit(CAN2,&TxMessage);
}

