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

#define USART_REC_LEN  			50  	//定义最大接收字节数 50
#define EN_USART1_RX 			1		//串口1接收使能



extern u8 PIDKiSign;


extern u8 UART5_Rx_DMAbuff[UART5_Rx_DMAbuf_LEN];
extern u8 UART5_Tx_DMAbuff[UART5_Tx_DMAbuf_LEN];


/******************一代飞镖******************/

extern u8 USART2_Tx_DMAbuff[USART2_Tx_DMAbuf_LEN]; 
extern u8 USART2_Rx_DMAbuff[USART2_Rx_DMAbuf_LEN]; 
extern u8 UART4_Rx_DMAbuff[UART4_Rx_DMAbuf_LEN];
extern u8 UART4_Tx_DMAbuff[UART4_Tx_DMAbuf_LEN];
extern u8 USART3_Rx_DMAbuff[USART3_Rx_DMAbuf_LEN]; 
extern u8 USART3_Tx_DMAbuff[USART3_Tx_DMAbuf_LEN]; 	 

extern uint16_t sensor_mode1;
extern uint16_t sensor_mode2;

extern uint16_t sensor_buff1;
extern uint16_t sensor_buff2;

extern fp32 sensor_data1;
extern fp32 sensor_data2;
extern uint16_t binaryArray1[8];
extern uint16_t binaryArray2[8];

extern ST_ENCODER Track_Encoder1;
extern ST_ENCODER Track_Encoder2;

extern ST_ENCODER pitch_encoder;

extern ST_ENCODER trigger_encoder;

extern ST_ENCODER synchronous_encoder1;
extern ST_ENCODER synchronous_encoder2;

extern ST_ENCODER yaw_encoder;

extern s16 Compare_Servo1;
extern s16 Compare_Servo2;
extern s16 Compare_Servo3;




extern uint16_t servo_time;


extern u8 UA5RxMailbox[UART5_RXMB_LEN];    //串口5接收变量
extern USART_RX_TypeDef UART5_Rcr; //串口5接收变量
extern bool Autodart_Flag;  // 比赛时，飞镖的目标选取是手动还是自动选取，0为手动模式，1为自动模式


/**********RC********/
extern volatile unsigned char sbus_rx_buffer[2][RC_FRAME_LENGTH]; //double sbus rx buffer to save data
extern RC_Ctl_t RC_CtrlData;


extern SystemMonitor_struct systemmonitor;
#endif
