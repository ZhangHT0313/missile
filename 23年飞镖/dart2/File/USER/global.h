#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "stm32f4xx.h"                  // Device header
#include "hitcrt_types.h"

#define UART5_RX_STREAM        	DMA1_Stream0
#define USART3_RX_STREAM        DMA1_Stream1
#define UART4_RX_STREAM         DMA1_Stream2
#define USART3_TX_STREAM        DMA1_Stream3
#define UART4_TX_STREAM         DMA1_Stream4
#define USART2_RX_STREAM        DMA1_Stream5
#define USART2_TX_STREAM        DMA1_Stream6
#define UART5_TX_STREAM       	DMA1_Stream7
#define UART8_TX_STREAM         DMA1_Stream0
#define UART7_TX_STREAM         DMA1_Stream1
#define UART7_RX_STREAM         DMA1_Stream3
#define UART8_RX_STREAM         DMA1_Stream6
#define USART6_RX_STREAM        DMA2_Stream1
#define USART1_RX_STREAM        DMA2_Stream2
#define USART6_TX_STREAM        DMA2_Stream6
#define USART1_TX_STREAM        DMA2_Stream7


#define UA2RxDMAbuf_LEN 200   //主控
#define UA2TxDMAbuf_LEN 1 

#define USART_REC_LEN  			50  	//定义最大接收字节数 50
#define EN_USART1_RX 			1		//串口1接收使能
extern u8 PIDKiSign;


/******************一代飞镖******************/

extern u8 USART2_Tx_DMAbuff[USART2_Tx_DMAbuf_LEN]; 
extern u8 USART2_Rx_DMAbuff[USART2_Rx_DMAbuf_LEN]; 
extern u8 UART4_Rx_DMAbuff[UART4_Rx_DMAbuf_LEN];
extern u8 UART4_Tx_DMAbuff[UART4_Tx_DMAbuf_LEN];
extern u8 USART3_Rx_DMAbuff[USART3_Rx_DMAbuf_LEN]; 
extern u8 USART3_Tx_DMAbuff[USART3_Tx_DMAbuf_LEN]; 	 


extern bool DataReceived_Flag;
extern int Data_From_C1[16];
extern u8 UA2RxDMAbuf[UA2RxDMAbuf_LEN];     //串口2接收变量
extern u8 UA2TxDMAbuf[UA2TxDMAbuf_LEN];
extern u32 Time_Minute;
extern u8  Time_Second;


extern SystemMonitor_struct systemmonitor;
#endif
