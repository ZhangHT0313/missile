/*******************************************************************
版权声明：HITCRT(哈工大竞技机器人协会)
文件名：HITCRT_TYPES.h
最近修改日期：2023.05.24
版本：2.0
--------------------------------------------------------------------
模块描述：该模块申明常用的数据类型与通用的结构体。
--------------------------------------------------------------------
修改记录：
作者        时间            版本         说明
任伟        2010.3.5          1.0        建立此文件
路方正      2023.5.24       2.0
********************************************************************/
#ifndef  __HITCRT_TYPES_H__
#define	 __HITCRT_TYPES_H__
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_it.h"
#include <math.h>
#include "sys.h"




/****************************************常用常数****************************************/
/****************************************常用常数****************************************/
/****************************************常用常数****************************************/

#define PI 3.1415926536f
#define PI2 6.2831853072f
#define PISHORT 3.14f
#define RADIAN_10 0.00174532922f // PI/1800，多次需要运算，故单独提取出来
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


/****************************************常用数据类型****************************************/
/****************************************常用数据类型****************************************/
/****************************************常用数据类型****************************************/

#define TRUE		(1)
#define FALSE		(0)

#define DEC			(10)
#define HEX			(16)


typedef unsigned char  		UCHAR8;                    /* defined for unsigned 8-bits integer variable 	    无符号8位整型变量  */
typedef signed   char  		SCHAR8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short 		USHORT16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short 		SSHORT16;                  /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   		UINT32;                    /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef int   				    SINT32;                    /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          		FP32;                      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         		DB64;                      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */

typedef FP32              fp32;

union float2char
{
	uint8_t char_num[4];
	fp32 float_num;	
};


/****************************************常用结构体****************************************/
/****************************************常用结构体****************************************/
/****************************************常用结构体****************************************/

typedef struct
{
   	float kp,ki,kd;//三个系数
    float error,lastError;//误差、上次误差
    float integral,maxIntegral;//积分、积分限幅
    float output,maxOutput;//输出、输出限幅
}PID;

typedef struct
{
    PID inner;//内环
    PID outer;//外环
    float output;//串级输出，等于inner.output
}CascadePID;

/****************************************SYSTEM MONITOR************************************/
typedef struct
{
	uint16_t systemmonitor_fps;      //帧率监测
	uint16_t systemmonitor_counter;  //计数器

}SystemMonitor_struct;

#endif
