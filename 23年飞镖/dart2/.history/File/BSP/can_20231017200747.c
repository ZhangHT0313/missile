#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"


uint32_t g_uiAirValve =0;

int16_t current_lift;     	//电机的传入电流值
int16_t current_clip;
int16_t current_rotate;

/*
 *4个编码器的结构体初始化
 *之后的电机编码器有关的值请用这个结构体来修改
*/
ST_ENCODER encode1 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode12 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode2 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode3 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode4 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode5 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode6 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode7 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode8 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode9 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encodeA = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encodeB = {.Number = 8191, .SumValue = 0, .RawValue = 0};
/*
 *以下部分为电机编码器的编码值、电机的转速值。
 *他们在can通信的中断服务函数中被调用并赋值
*/
u16 value1;      
u16 value12;
u16 value2;
u16 value3;
u16 value4;
u16 value5;
u16 value6;
u16 value7;
u16 value8;
u16 value9;
u16 valueA;
u16 valueB;

s16 speed1;
s16 speed12;
s16 speed2;
s16 speed3;
s16 speed4;
s16 speed5;
s16 speed6;
s16 speed7;
s16 speed8;
s16 speed9;
s16 speedA;
s16 speedB;

/*
 *函数功能：can通信初始化
 *入口参数：NONE
 *出口参数：NONE
 *其他说明：这段代码同时初始化了can通讯1和2
*/
void CAN_Configuration(void)																											//CAN底层配置
{
	CAN_InitTypeDef CAN_InitStructure;																							//结构体定义			
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterStructure;
	NVIC_InitTypeDef NVIC_InitStructure1;
	NVIC_InitTypeDef NVIC_InitStructure2;
	//时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2,ENABLE);			//使能CAN时钟（CAN2是从CAN，使用前必须使能CAN1的始时钟）
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);		//使能GPIO时钟
	//引脚初始化
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;																		//GPIO初始化
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;                       //初始化5 ,6 ,8，9引脚
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6;                       //初始化5，6引脚
//	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	//引脚复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);												//设置复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2);
	//CAN初始化
	CAN_InitStructure.CAN_TTCM=DISABLE;	                                       //事件触发	
	CAN_InitStructure.CAN_ABOM=ENABLE;				                           //自动离线管理																		
	CAN_InitStructure.CAN_AWUM=ENABLE;                                         //自动唤醒
	CAN_InitStructure.CAN_NART=DISABLE;	                                       //错误重传
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;                                                             //CAN初始化
	
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;	
	CAN_InitStructure.CAN_Prescaler=3;                             //波特设置，42MHz/3/(1+9+4)=1MHz
	CAN_Init(CAN1,&CAN_InitStructure);
	CAN_Init(CAN2,&CAN_InitStructure);
	//筛选器初始化
	CAN_FilterStructure.CAN_FilterNumber=0;
	CAN_FilterStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterStructure.CAN_FilterMaskIdHigh=0x0000;                  //掩码置0不筛选
	CAN_FilterStructure.CAN_FilterMaskIdLow=0x0000;	
	CAN_FilterStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterStructure);
	CAN_FilterStructure.CAN_FilterNumber=14;
	CAN_FilterInit(&CAN_FilterStructure);	
	//中断初始化+使能
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                                        //使能CAN_IT中断
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
 *函数功能：can通信发送数据
 *入口参数：msg：要发送的数据      len：发送数据的长度
 *出口参数：1 -> 成功发送      0 -> 未成功发送
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
	while((CAN_TransmitStatus(CAN1 , mbox)==CAN_TxStatus_Failed)&&(i<0xFFF))i++;                        //等待发送结束
	if(i>=0xFFF)return 1 ;
	return 0;
	
}

/*
 *函数功能：can通信接收数据
 *入口参数：buf：接收数据存储的数组
 *出口参数：RxMessage.DLC
*/
u8 CAN1_Receive_Msg(u8 *buf)
{
	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0) return 0;      //没收到数据，直接退出
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);        //读取数据
	for(i=0; i<RxMessage.DLC; i++)
	buf[i]=RxMessage.Data[i];
	return RxMessage.DLC;
	
}

/*
 *函数功能：can1通信中断服务函数
 *入口参数：NONE
 *出口参数：NONE
*/
void CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)		//获取空闲中断标志位
	{
//		CanRxMsg CanRxStruc1;
//		CanRxMsg CanRxStruc2;
		CanRxMsg CanRxStruc;
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);		//清除中断标志位
		CAN_Receive(CAN1,CAN_FIFO0,&CanRxStruc);
//		CAN_Receive(CAN1,CAN_FIFO0,&CanRxStruc1);		//从FIFO中接收消息CanRxStruc
//		CAN_Receive(CAN1,CAN_FIFO0,&CanRxStruc2);		//从FIFO中接收消息CanRxStruc
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
		switch(CanRxStruc.StdId)											//根据id决定存储到哪个对应的结构体
		{
			//电调ID，620对应1号，6020无对应
			case 0x201:
				value1 = GetEncodeNumber(&CanRxStruc);
				speed1 = GetSpeed(&CanRxStruc) ;
				Abs_Encoder_Process(&encode1,value1);
				break;
			//电调ID，620对应2号，6020无对应
			case 0x202:
				value2 = GetEncodeNumber(&CanRxStruc);
				speed2 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode2,value2);			
				break;
			//电调ID，620对应3号，6020无对应
			case 0x203:
				value3 = GetEncodeNumber(&CanRxStruc);
				speed3 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode3,value3);
				break;
			//电调ID，620对应4号，6020无对应
			case 0x204:
				value4 = GetEncodeNumber(&CanRxStruc);
				speed4 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode4,value4);
				break;
			//电调ID，620对应5号，6020对应1号
			case 0x205:
				value5 = GetEncodeNumber(&CanRxStruc);
				speed5 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode5,value5);
				break;
			//电调ID，620对应6号，6020对应2号
			case 0x206:
				value6 = GetEncodeNumber(&CanRxStruc);
				speed6 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode6,value6);
				break;
			//电调ID，620对应7号，6020对应3号
			case 0x207:
				value7 = GetEncodeNumber(&CanRxStruc);
				speed7 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode7,value7);
				break;
			//电调ID，620对应8号，6020对应4号
			case 0x208:
				value8 = GetEncodeNumber(&CanRxStruc);
				speed8 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode8,value8);
				break;
			//电调ID，620无对应，6020对应5号
			case 0x209:
				value9 = GetEncodeNumber(&CanRxStruc);
				speed9 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode9,value9);
				break;
			//电调ID，620无对应，6020对应6号
			case 0x20A:
				valueA = GetEncodeNumber(&CanRxStruc);
				speedA = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encodeA,valueA);
				break;
			//电调ID，620无对应，6020对应7号
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
 *函数功能：can2通信中断服务函数
 *入口参数：NONE
 *出口参数：NONE
*/
void CAN2_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=RESET)		//获取空闲中断标志位
	{
		CanRxMsg CanRxStruc;
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);			//清除中断标志位
		CAN_Receive(CAN2,CAN_FIFO0,&CanRxStruc);			//从FIFO中接收消息CanRxStruc
		switch(CanRxStruc.StdId)											//根据id决定存储到哪个对应的结构体
		{
			//电调ID，620对应1号，6020无对应
			case 0x201:
				value1 = GetEncodeNumber(&CanRxStruc);
				speed1 = GetSpeed(&CanRxStruc) ;
				Abs_Encoder_Process(&encode1,value1);
				break;
			//电调ID，620对应2号，6020无对应
			case 0x202:
				value2 = GetEncodeNumber(&CanRxStruc);
				speed2 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode2,value2);			
				break;
			//电调ID，620对应3号，6020无对应
			case 0x203:
				value3 = GetEncodeNumber(&CanRxStruc);
				speed3 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode3,value3);
				break;
			//电调ID，620对应4号，6020无对应
			case 0x204:
				value4 = GetEncodeNumber(&CanRxStruc);
				speed4 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode4,value4);
				break;
			//电调ID，620对应5号，6020对应1号
			case 0x205:
				value5 = GetEncodeNumber(&CanRxStruc);
				speed5 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode5,value5);
				break;
			//电调ID，620对应6号，6020对应2号
			case 0x206:
				value6 = GetEncodeNumber(&CanRxStruc);
				speed6 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode6,value6);
				break;
			//电调ID，620对应7号，6020对应3号
			case 0x207:
				value7 = GetEncodeNumber(&CanRxStruc);
				speed7 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode7,value7);
				break;
			//电调ID，620对应8号，6020对应4号
			case 0x208:
				value8 = GetEncodeNumber(&CanRxStruc);
				speed8 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode8,value8);
				break;
			//电调ID，620无对应，6020对应5号
			case 0x209:
				value9 = GetEncodeNumber(&CanRxStruc);
				speed9 = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encode9,value9);
				break;
			//电调ID，620无对应，6020对应6号
			case 0x20A:
				valueA = GetEncodeNumber(&CanRxStruc);
				speedA = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&encodeA,valueA);
				break;
			//电调ID，620无对应，6020对应7号
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
 *函数功能：获取电机编码器的值
 *入口参数：CanRxMsg的地址
 *出口参数：EncodeNumber，电机编码器的值
 *其他说明：这个函数在can通讯的中断服务函数中使用过
*/
u16 GetEncodeNumber(CanRxMsg* pCanRxMsg)
{
	u16 EncodeNumber;
	EncodeNumber = (pCanRxMsg->Data[0]<<8 | pCanRxMsg->Data[1]);
	return EncodeNumber;
}

/*
 *函数功能：获取电机转速
 *入口参数：CanRxMsg的地址
 *出口参数：Speed，电机转速
 *其他说明：这个函数在can通讯的中断服务函数中使用过
*/
s16 GetSpeed(CanRxMsg* pCanRxMsg)
{
	s16 Speed;
	Speed = (pCanRxMsg->Data[2]<<8 | pCanRxMsg->Data[3]);
	return Speed;
}

/*
 *函数功能：通过can通讯发送电流值驱动电机
 *入口参数：CANx：使用的can通讯号（can1、can2等）；id：电机id号；current1等：受控电机要传入的电流（GM6020为电压）值：
 *出口参数：NONE
 *其他说明：C620中的current至少为100才能看到电机转动，而GM6020中则至少要1000才能看到电机转动
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
 *函数功能：处理编码器溢出
 *入口参数：ST_ENCODER* pEncoder：ST_ENCODER结构体的地址；value：获取到的电机编码器的值
 *出口参数：NONE
 *其他说明：此函数在can通信的中断服务函数中调用过
*/
void Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value)
{
	pEncoder->PreRawValue = pEncoder->RawValue;
	pEncoder->RawValue = value;
	pEncoder->Diff = pEncoder->RawValue - pEncoder->PreRawValue;
	
	if(pEncoder->Diff < -pEncoder->Number/2)												//正转过0
		pEncoder->Diff += pEncoder->Number;						
	else if(pEncoder->Diff > pEncoder->Number/2)										//反转过0
		pEncoder->Diff -= pEncoder->Number;		
	
	pEncoder->SumValue += pEncoder->Diff;
}

/*
 *函数功能：can1的气缸发送
 *入口参数：uint32_t* pAir
 *出口参数：NONE
 *其他说明：主函数中已经利用O_VALVE和C_VALVE对该函数进行了简化，不必深究
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
 *函数功能：can2的气缸发送
 *入口参数：uint32_t* pAir
 *出口参数：NONE
 *其他说明：主函数中已经利用O_VALVE和C_VALVE对该函数进行了简化，不必深究
*/
void SendAirMsgByCan2(uint32_t* pAir)
{
	static CanTxMsg TxMessage = {CAN_AIR_ID, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8,0,0,0,0,0,0,0,0};
	static uint32_t s_ucLastAir = 0;
	s_ucLastAir = *pAir;
	*((uint32_t*)TxMessage.Data) = s_ucLastAir;
	CAN_Transmit(CAN2,&TxMessage);
}

