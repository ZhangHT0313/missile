#include "data_send_task.h"

void Send_Data_To_MainControl2(void)
{
	DMA_ClearITPendingBit(UART4_TX_STREAM, DMA_IT_TCIF4);      //����DMA_Mode_Normal,����û��ʹ������ж�ҲҪ������������ֻ��һ��
    DMA_Cmd(UART4_TX_STREAM, DISABLE);                         //���õ�ǰ����ֵǰ�Ƚ���DMA

	/*for example*/
//  int_to_u8((int)yaw_pid.outer.fpFB,UART4_Tx_DMAbuff,0);	
//	int_to_u8((int)6666,UART4_Tx_DMAbuff,49);
	
	while (DMA_GetCmdStatus(UART4_TX_STREAM) != DISABLE){}
	UART4_TX_STREAM->M0AR = (uint32_t)&UART4_Tx_DMAbuff;         //���õ�ǰ�������ݻ���ַ:Memory0 tARget
	UART4_TX_STREAM->NDTR = (uint32_t)UART4_Tx_DMAbuf_LEN;         //���õ�ǰ���������ݵ�����:Number of Data units to be TRansferred
	DMA_Cmd(UART4_TX_STREAM, ENABLE);
		
}    

void Send_Data_To_Sensor1(void)
{
	DMA_ClearITPendingBit(USART2_TX_STREAM, DMA_IT_TCIF6);      //����DMA_Mode_Normal,����û��ʹ������ж�ҲҪ������������ֻ��һ��
    DMA_Cmd(USART2_TX_STREAM, DISABLE);                         //���õ�ǰ����ֵǰ�Ƚ���DMA
	switch(sensor_mode1)
	{
		case 0:
			USART2_Tx_DMAbuff[0] = 0x11;
			USART2_Tx_DMAbuff[1] = 0x42;
			USART2_Tx_DMAbuff[2] = 0x3F;
			USART2_Tx_DMAbuff[3] = 0x12;
			USART2_Tx_DMAbuff[4] = 0x0D;
		case 1:
			USART2_Tx_DMAbuff[0] = 0x11;
			USART2_Tx_DMAbuff[1] = 0x43;
			USART2_Tx_DMAbuff[2] = 0x3F;
			USART2_Tx_DMAbuff[3] = 0x13;
			USART2_Tx_DMAbuff[4] = 0x0D;
	}
	while (DMA_GetCmdStatus(USART2_TX_STREAM) != DISABLE){}
	USART2_TX_STREAM->M0AR = (uint32_t)&USART2_Tx_DMAbuff;         //���õ�ǰ�������ݻ���ַ:Memory0 tARget
	USART2_TX_STREAM->NDTR = (uint32_t)USART2_Tx_DMAbuf_LEN;         //���õ�ǰ���������ݵ�����:Number of Data units to be TRansferred
	DMA_Cmd(USART2_TX_STREAM, ENABLE);
}


void Send_Data_To_Sensor2(void)
{
	DMA_ClearITPendingBit(USART3_TX_STREAM, DMA_IT_TCIF3);      //����DMA_Mode_Normal,����û��ʹ������ж�ҲҪ������������ֻ��һ��
    DMA_Cmd(USART3_TX_STREAM, DISABLE);                         //���õ�ǰ����ֵǰ�Ƚ���DMA
	switch(sensor_mode1)
	{
		case 0:
			USART3_Tx_DMAbuff[0] = 0x11;
			USART3_Tx_DMAbuff[1] = 0x42;
			USART3_Tx_DMAbuff[2] = 0x3F;
			USART3_Tx_DMAbuff[3] = 0x12;
			USART3_Tx_DMAbuff[4] = 0x0D;
		case 1:
			USART3_Tx_DMAbuff[0] = 0x11;
			USART3_Tx_DMAbuff[1] = 0x43;
			USART3_Tx_DMAbuff[2] = 0x3F;
			USART3_Tx_DMAbuff[3] = 0x13;
			USART3_Tx_DMAbuff[4] = 0x0D;
	}
	while (DMA_GetCmdStatus(USART3_TX_STREAM) != DISABLE){}
	USART3_TX_STREAM->M0AR = (uint32_t)&USART3_Tx_DMAbuff;         //���õ�ǰ�������ݻ���ַ:Memory0 tARget
	USART3_TX_STREAM->NDTR = (uint32_t)USART3_Tx_DMAbuf_LEN;         //���õ�ǰ���������ݵ�����:Number of Data units to be TRansferred
	DMA_Cmd(USART3_TX_STREAM, ENABLE);
}

