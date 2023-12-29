#include "stm32f4xx.h"                  // Device header
#include "global.h"
u8 PIDKiSign = 1;


/******************一代飞镖******************/
/******************一代飞镖******************/
/******************一代飞镖******************/
u8 USART2_Tx_DMAbuff[USART2_Tx_DMAbuf_LEN]; 
u8 USART2_Rx_DMAbuff[USART2_Rx_DMAbuf_LEN];
u8 UART4_Rx_DMAbuff[UART4_Rx_DMAbuf_LEN];
u8 UART4_Tx_DMAbuff[UART4_Tx_DMAbuf_LEN];
u8 USART3_Rx_DMAbuff[USART3_Rx_DMAbuf_LEN]; 
u8 USART3_Tx_DMAbuff[USART3_Tx_DMAbuf_LEN]; 	

/***SD***/
bool DataReceived_Flag=FALSE;
int Data_From_C1[16]={0};
u8 UA2RxDMAbuf[UA2RxDMAbuf_LEN]  = {0};     //串口2接收变量
u8 UA2TxDMAbuf[UA2TxDMAbuf_LEN]  = {0x46};
u32 Time_Minute = 0;
u8  Time_Second = 0;











/**********************USART*************************/
/**********************USART*************************/
/**********************USART*************************/

u16 USART_RX_STA=0;       //接收状态标记	

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目

uint16_t Res1;//定义接收量

/***********************CAN**************************/
/***********************CAN**************************/
/***********************CAN**************************/


uint32_t g_uiAirValve =0;

int16_t current_lift;     	
int16_t current_clip;
int16_t current_rotate;



/****************systemmonitor****************/
/****************systemmonitor****************/
/****************systemmonitor****************/
SystemMonitor_struct systemmonitor={0};
