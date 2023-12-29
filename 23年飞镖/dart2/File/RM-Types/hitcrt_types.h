#ifndef  __HITCRT_TYPES_H__
#define	 __HITCRT_TYPES_H__
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "sys.h"
#include "stm32f4xx.h"                  // Device header
/****************一代飞镖********************************/
#define UA4RxDMAbuf_LEN 45
#define UA4TxDMAbuf_LEN 13
#define UART4_Rx_DMAbuf_LEN  45  //主控2
#define UART4_Tx_DMAbuf_LEN  200  //主控2
#define USART3_Rx_DMAbuf_LEN  6  //串口屏
#define USART3_Tx_DMAbuf_LEN  300  //串口屏
#define USART2_Tx_DMAbuf_LEN  10
#define USART2_Rx_DMAbuf_LEN  10
/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

#define RC_CH_VALUE_SHOOT_DEAD    	((u16)200)
#define RC_CH_VALUE_GIMBAL_DEAD     ((u16)20)
#define RC_CH_VALUE_RANGE           ((u16)660)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)
#define RC_FRAME_LENGTH 18u


/****************************************���ó���****************************************/
/****************************************���ó���****************************************/
/****************************************���ó���****************************************/

#define PI 3.1415926536f
#define PI2 6.2831853072f
#define PISHORT 3.14f
#define RADIAN_10 0.00174532922f // PI/1800�������Ҫ���㣬�ʵ�����ȡ����
#define RADIAN_100 0.000174532922f
#define DEG 57.29578f
#define DEG_10 572.9578f     // DEG*10
#define RADIAN 0.0174532922f // PI/180
#define RAD10 572.9578f
#define PI_2 1.570796326795f
#define PI_3 1.0471975512f
#define PI_4 0.785398163f
#define PI_6 0.523598775f
#define RADIAN_15 0.261799387f
#define RADIAN_45 0.785398163f
#define RADIAN_75 1.308996939f
#define RADIAN_105 1.832595715f
#define RADIAN_135 2.356194490f
#define RADIAN_165 2.879793266f



/****************************************������������****************************************/
/****************************************������������****************************************/
/****************************************������������****************************************/

#define TRUE		(1)
#define FALSE		(0)

#define DEC			(10)
#define HEX			(16)


typedef unsigned char  		UCHAR8;                    /* defined for unsigned 8-bits integer variable 	    �޷���8λ���ͱ���  */
typedef signed   char  		SCHAR8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short 		USHORT16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short 		SSHORT16;                  /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   		UINT32;                    /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef int   				    SINT32;                    /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float          		FP32;                      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         		DB64;                      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

typedef FP32              fp32;

union float2char
{
	uint8_t char_num[4];
	fp32 float_num;	
};



/****************************************���ýṹ��****************************************/
/****************************************���ýṹ��****************************************/
/****************************************���ýṹ��****************************************/

//PID结构体
typedef struct{
	FP32 fpDes; //控制变量目标值
	FP32 fpDesMAX;
	FP32 fpDesMIN;
	FP32 fpFB;	//控制变量反馈值
	
	FP32 fpKp;	//比例系数
	FP32 fpKi;
	FP32 fpKd;
	
	FP32 fpUp;	//比例输出
	FP32 fpUi;	//积分输出
	FP32 fpUd;
	
	FP32 fpE;		//本次偏差
	FP32 fpPreE;	//上次偏差
	FP32 fpPreUd;
	FP32 fpSumE;	//总偏差
	FP32 fpU;		//本次pid运算结果
	
	FP32 fpUMax;	//PID运算后输出上限值
	FP32 fpEpMax;//比例项输出最大值
	FP32 fpEMax;
	FP32 fpEMin;
	FP32 fpEiMax;	//积分上限
	FP32 fpEiMin;	//积分上限
    FP32 fpEdMax;
	FP32 fpSumEMax; //积分累计上限
	
	FP32 fpDt;	//控制周期
	bool StopFlag;

}PID;
typedef struct
{
    PID inner;//内环
    PID outer;//外环
    float output;//串级输出，等于inner.output
}CascadePID;


typedef struct
{
    float preout;
    float out;
    float in;
    float off_freq;
    float samp_tim;
} ST_LPF;

// TD（微分跟踪器）参数结构体
typedef struct
{
	float m_x1;		//位置
	float m_x2;		//速度
	float m_x;		//位移
	float m_r;		//TD跟踪因子（决定跟踪速度，r越大跟得越快，如果追求快速响应，微分预测的滤波效果会变差）
	float m_h;		//TD滤波因子（算法式中的h0，h0越大微分预测的滤波效果越好）
	float m_T;		//TD积分步长（h为步长,h越小滤波效果越好，这个值应该与采样周期一致）
	float m_aim;	//目标位置
}ST_TD;



typedef struct
{
	float T; //采样周期
	float t; //一阶滤波器时间常数τ
	float ukd1;
	float uk;
	float ykd1;
	float yk;
}ST_Filter_1order;


typedef struct
{
	float T; //采样周期
	float t; //二阶滤波器时间常数τ
	float zeta; //阻尼比
	float ukd2;
	float ukd1;
	float uk;
	float ykd2;
	float ykd1;
	float yk;
}ST_Filter_2order;

/************************************滑模控制******************************/
//滑模控制
typedef struct
{
		FP32 fpDes;	//控制变量目标值
		FP32 fpFB;	//控制变量反馈值
		FP32 fpE;	//本次偏差
		FP32 fpU;	//本次运算结果
		FP32 fpUMax;	//输出上限
		
		//SMC参数
		FP32 b;//转动惯量倒数
		FP32 eps;//饱和函数增益项，作用是扰动补偿
		FP32 gain;//比例增益项
		FP32 dead;//饱和函数死区，在死区内为放大环节，死区外为固定值
		ST_TD TD; //使用TD来获取微分信号
}ST_SMC;







/****************************************SYSTEM MONITOR************************************/
typedef struct
{
	uint16_t sd_fps;
	uint16_t sd_counter;
	
	uint16_t usart2_tx;
	uint16_t usart2_rx;
	
}SystemMonitor_struct;


/**************************************RC***********************************************/
/**************************************RC***********************************************/
/**************************************RC***********************************************/

/* ----------------------- Data Struct ------------------------------------- */
typedef __packed struct
{
 __packed struct
 {
 uint16_t ch0;
 uint16_t ch1;
 uint16_t ch2;
 uint16_t ch3;
 uint16_t ch4; 
 uint8_t s1;
 uint8_t s2;
 }rc;
 __packed struct
 {
 int16_t x;
 int16_t y;
 int16_t z;
 uint8_t press_l;
 uint8_t press_r;
 }mouse;
 __packed struct
 {
 uint16_t v;
 }key;
}RC_Ctl_t;


/**************************************CAN***********************************************/
typedef struct 
{
	s32 RawValue;						//本次编码的原始值
	s32 PreRawValue;				//上一次编码器的原始值
	s32 Diff;								//编码器两次原始量差值
	s32 Number;							//编码器线数
	s32 SumValue;					//编码器累加值
	s16 state;
	s32 ReductionRatio;    //减速比
	FP32  fpSpeed;	
}ST_ENCODER;



#endif
