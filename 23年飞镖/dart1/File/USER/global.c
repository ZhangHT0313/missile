#include "stm32f4xx.h"                  // Device header
#include "global.h"
u8 PIDKiSign = 1;


/*****************通信****************/

/*主控1至裁判系统*/
u8 UART5_Rx_DMAbuff[UART5_Rx_DMAbuf_LEN]={0};
u8 UART5_Tx_DMAbuff[UART5_Tx_DMAbuf_LEN]={0};

/*主控1与力传感器*/
uint16_t sensor_mode1;
uint16_t sensor_mode2;

uint16_t sensor_buff1;
uint16_t sensor_buff2;

fp32 sensor_data1;
fp32 sensor_data2;

uint16_t binaryArray1[8];
uint16_t binaryArray2[8];

/******************一代飞镖******************/
/******************一代飞镖******************/
/******************一代飞镖******************/
u8 USART2_Tx_DMAbuff[USART2_Tx_DMAbuf_LEN]; 
u8 USART2_Rx_DMAbuff[USART2_Rx_DMAbuf_LEN];

u8 UART4_Rx_DMAbuff[UART4_Rx_DMAbuf_LEN];
u8 UART4_Tx_DMAbuff[UART4_Tx_DMAbuf_LEN];

u8 USART3_Rx_DMAbuff[USART3_Rx_DMAbuf_LEN]; 
u8 USART3_Tx_DMAbuff[USART3_Tx_DMAbuf_LEN]; 	


/*track 3508*/
ST_ENCODER Track_Encoder1;
ST_ENCODER Track_Encoder2;

/*pitch 3508*/
ST_ENCODER pitch_encoder;


/*trigger 2006*/
ST_ENCODER trigger_encoder;

/*synchronous 2006*/
ST_ENCODER synchronous_encoder1;
ST_ENCODER synchronous_encoder2;


/***********yaw_encoder***************/
ST_ENCODER yaw_encoder;

/***********servo***************/
s16 Compare_Servo1 = 0;
s16 Compare_Servo2 = 0;
s16 Compare_Servo3 = 0;



uint16_t servo_time;


/*裁判系统*/
u8 UA5RxMailbox[UART5_RXMB_LEN]  = {0};    //串口5接收变量
USART_RX_TypeDef UART5_Rcr = {UART5,UART5_RX_STREAM,UA5RxMailbox,UART5_Rx_DMAbuff,UART5_RXMB_LEN,UART5_Rx_DMAbuf_LEN,0,0,0}; //串口5接收变量
bool Autodart_Flag = FALSE;  // 比赛时，飞镖的目标选取是手动还是自动选取，0为手动模式，1为自动模式


/*********************RC*******************************/
/*********************RC*******************************/
/*********************RC*******************************/
/* ----------------------- Internal Data ----------------------------------- */
volatile unsigned char sbus_rx_buffer[2][RC_FRAME_LENGTH]; //double sbus rx buffer to save data
RC_Ctl_t RC_CtrlData;









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
