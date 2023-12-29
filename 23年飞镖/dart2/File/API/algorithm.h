#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "string.h"
#include "global.h"
#include "hitcrt_types.h"
#include "math.h"

#define abs(x) ((x)>0? (x):(-(x)))
void Abp_PID_Friction(PID* pid);

void PID_CascadeCalc(CascadePID *pid,float outerRef,float outerFdb,float innerFdb);
void CalPID(PID *pstPid);
void CalISeparatedPID(PID *pstPid);
void CalIResistedPID(PID *pstPid);
void CalIWeakenPID(PID *pstPid);

FP32 Clip(FP32 fpValue, FP32 fpMin, FP32 fpMax);
SSHORT16 Round(FP32 fp_Round_Number);
SINT32 Sign_Judge(FP32 fp_Judge_Number);
void LpFilter(ST_LPF* lpf);
bool RampSignal(float* p_Output, float DesValue, float Step);
bool RampSignal_s16(s16* p_Output, s16 DesValue, s16 Step); //传入指针量向目标值按Step步进，最终稳在DesValue
void CalTD(ST_TD *pStTD);


void CalSMC(ST_SMC *pStSMC);
float SMC_SatFunc(float in, float d);
SINT32 Sign_Judge(FP32 fp_Judge_Number);

void FrictionSpeed_Filter_1order(ST_Filter_1order *Filter_1order);
void FrictionSpeed_Filter_2order(ST_Filter_2order *Filter_2order);
void Filter_Calculate(void);

void Data_Process(void);
#endif
