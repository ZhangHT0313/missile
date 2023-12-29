#ifndef  __HITCRT_TYPES_H__
#define	 __HITCRT_TYPES_H__
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_it.h"
#include <math.h>
#include "sys.h"




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

typedef struct
{
   	float kp,ki,kd;//����ϵ��
    float error,lastError;//���ϴ����
    float integral,maxIntegral;//���֡������޷�
    float output,maxOutput;//���������޷�
}PID;

typedef struct
{
    PID inner;//�ڻ�
    PID outer;//�⻷
    float output;//�������������inner.output
}CascadePID;

/****************************************SYSTEM MONITOR************************************/
typedef struct
{
	uint16_t systemmonitor_fps;      //֡�ʼ��
	uint16_t systemmonitor_counter;  //������

}SystemMonitor_struct;


/**************************************CAN***********************************************/
typedef struct ST_ENCODER
{
	u32 RawValue;						//���α����ԭʼֵ
	u32 PreRawValue;					//��һ�α�������ԭʼֵ
	s32 Diff;							//����������ԭʼ����ֵ
	s32 Number;							//����������
	s32 SumValue;						//�������ۼ�ֵ						
}ST_ENCODER;

#endif
