#include "stm32f4xx.h"                  // Device header
#include "algorithm.h"

/*friction的PID算法*/

void Abp_PID_Friction(PID* pid)
{

	
	pid->fpE=pid->fpDes-pid->fpFB;
  if(pid->fpE<60)
	{
		if(pid->fpE>-60)
	pid->fpSumE+=pid->fpE;

			else
	{
	   pid->fpSumE = 0;
	}
	}
//	    if(pid->fpE<0.01f)
//	{
//		if(pid->fpE>-0.01f)
//	pid->fpSumE = 0;

//	}
//	else
//	{
//	   pid->fpSumE = 0;
//	}
	pid->fpUi=pid->fpKi*pid->fpSumE/1000;//发现可能的置零原因是超过float类型的存储范围，故将除1000变成除500，无效,最终原因是达到零但机械原因导致不行
	pid->fpUp=pid->fpKp*pid->fpE;
	pid->fpUd=pid->fpKd*(pid->fpE-pid->fpPreE)*1000;
	
	
	
	pid->fpU = pid->fpUi+pid->fpUd+pid->fpUp;
	
	
	if(pid->fpUMax>pid->fpU>-pid->fpUMax)
	
		pid->fpU=pid->fpU;
	
	 if(pid->fpU>pid->fpUMax)
	 
		 pid->fpU=pid->fpUMax;
	 
	 if(pid->fpU<-pid->fpUMax)
	 
		 pid->fpU=-pid->fpUMax;
	 
	 	   if(fabs(pid->fpPreE-pid->fpE)<200)
	{	
		pid->StopFlag = TRUE;
	}
	else
	{
	  pid->StopFlag = FALSE;
	
	}
	 
	pid->fpPreE=pid->fpE;
}

void Abp_PID(PID* pid)
{

	
	pid->fpE=pid->fpDes-pid->fpFB;
	if(PIDKiSign)
	{
  if(pid->fpE<60)
	{
		if(pid->fpE>-60)
	pid->fpSumE+=pid->fpE;

			else
	{
	   pid->fpSumE = 0;
	}
	}
}
	else
	{
	   pid->fpSumE = 0;
	}
	pid->fpUi=pid->fpKi*pid->fpSumE/1000;//发现可能的置零原因是超过float类型的存储范围，故将除1000变成除500，无效,最终原因是达到零但机械原因导致不行
	pid->fpUp=pid->fpKp*pid->fpE;
	pid->fpUd=pid->fpKd*(pid->fpE-pid->fpPreE)*1000;
	
	
	
	pid->fpU = pid->fpUi+pid->fpUd+pid->fpUp;
	
	
	if(pid->fpUMax>pid->fpU>-pid->fpUMax)
	
		pid->fpU=pid->fpU;
	
	 if(pid->fpU>pid->fpUMax)
	 
		 pid->fpU=pid->fpUMax;
	 
	 if(pid->fpU<-pid->fpUMax)
	 
		 pid->fpU=-pid->fpUMax;
	 
	 	   if(fabs(pid->fpPreE-pid->fpE)<200)
	{	
		pid->StopFlag = TRUE;
	}
	else
	{
	  pid->StopFlag = FALSE;
	
	}
	 
	pid->fpPreE=pid->fpE;
}

//串级PID的计算函数
//参数(PID结构体,外环目标值,外环反馈值,内环反馈值)
void PID_CascadeCalc(CascadePID *pid,float outerRef,float outerFdb,float innerFdb)
{
    Abp_PID(&pid->outer);//计算外环
//	pid->outer.fpU = pid->inner.fpDes;
    Abp_PID(&pid->inner);//计算内环
    pid->output=pid->inner.fpU;//内环输出就是串级PID的输出
}





/*位置式PID算法 u(k)=Kp*E(K)+Ki*sum(E(K))+Kd*(E(K)-E(K-1))*/

/*******************************************************************
函数名称：CalPID(ST_PID *pstPid)
函数功能：普通的PID算法计算PID量
备    注：
********************************************************************/
void CalPID(PID *pstPid)
{
    pstPid->fpE = pstPid->fpDes - pstPid->fpFB;//计算当前偏差
    if(fabs(pstPid->fpE) <= pstPid->fpEMin)//偏差死区限制
    {
        pstPid->fpE = 0;
    }
    pstPid->fpSumE += pstPid->fpE;
    /*位置式PID计算公式*/
    pstPid->fpU = pstPid->fpKp * pstPid->fpE
                  + pstPid->fpKi * pstPid->fpSumE
                  + pstPid->fpKd * (pstPid->fpE - pstPid->fpPreE);
    pstPid->fpPreE = pstPid->fpE;//保存本次偏差
    /*PID运算输出限幅*/
    if(pstPid->fpU > pstPid->fpUMax)
    {
        pstPid->fpU = pstPid->fpUMax;
    }
    else if(pstPid->fpU < -pstPid->fpUMax)
    {
        pstPid->fpU = -pstPid->fpUMax;
    }

}

/*******************************************************************
函数名称：CalISeparatedPID(ST_PID *pstPid)
函数功能：积分分离式PID算法计算PID量
备    注：积分分离式PID改进算法可减小启动、停止或大幅度增减时较大偏差
          对积分项的积累，从而避免出现较大的超调及振荡现象。
********************************************************************/
void CalISeparatedPID(PID *pstPid)
{
    UCHAR8 uck=1;

    pstPid->fpE=pstPid->fpDes-pstPid->fpFB;//计算当前偏差
    if(fabs(pstPid->fpE) <= pstPid->fpEMin)//偏差死区限制
    {
        pstPid->fpE = 0;
    }
    pstPid->fpSumE += pstPid->fpE;//计算偏差累积
    /*若偏差过大，则积分项不累积偏差*/
    if(fabs(pstPid->fpE) > pstPid->fpEiMax )//判断是否满足积分分离
    {
        uck=0;
    }
    /*位置式PID计算公式*/
    pstPid->fpU = pstPid->fpKp * pstPid->fpE
                  + pstPid->fpKi * pstPid->fpSumE * uck
                  + pstPid->fpKd * (pstPid->fpE - pstPid->fpPreE);
    pstPid->fpPreE = pstPid->fpE;//保存本次偏差
    /*PID运算输出限幅*/
    if(pstPid->fpU > pstPid->fpUMax)
    {
        pstPid->fpU = pstPid->fpUMax;
    }
    else if(pstPid->fpU < -pstPid->fpUMax)
    {
        pstPid->fpU = -pstPid->fpUMax;
    }
}

/*******************************************************************
函数名称：CalIResistedPID(ST_PID *pstPid)
函数功能：抗积分饱和PID算法
备    注：系统往一个方向运动会产生较大积分误差，会在几个周期内产生振荡或超调
********************************************************************/
void CalIResistedPID(PID *pstPid)
{

    pstPid->fpE=pstPid->fpDes-pstPid->fpFB;   //计算当前偏差
    pstPid->fpSumE += pstPid->fpE;   //计算偏差累积

    pstPid->fpSumE = Clip(pstPid->fpSumE, -pstPid->fpEiMax, pstPid->fpEiMax);
    pstPid->fpUi = pstPid->fpKi * pstPid->fpSumE;

    pstPid->fpUp = Clip(pstPid->fpKp * pstPid->fpE, -pstPid->fpEpMax, pstPid->fpEpMax);
    pstPid->fpUd = Clip(pstPid->fpKd * (pstPid->fpE - pstPid->fpPreE), -pstPid->fpEdMax, pstPid->fpEdMax);


    /*若偏差在死区之内，则清零积分累计项*/
    if(fabs(pstPid->fpE) < pstPid->fpEMin)   //判断是否满足积分饱和条件
    {
        pstPid->fpSumE = 0;   //清除偏差累积
    }
    /*位置式PID计算公式*/
    pstPid->fpU = pstPid->fpUp + pstPid->fpUi + pstPid->fpUd;

    pstPid->fpPreE = pstPid->fpE;//保存本次偏差
    /*PID运算输出限幅*/
    pstPid->fpU = Clip(pstPid->fpU, -pstPid->fpUMax, pstPid->fpUMax);
}

/*******************************************************************
函数名称：CalIWeakenPID(ST_PID *pstPid)
函数功能：遇限削弱积分PID改进算法计算PID量
备    注：
********************************************************************/
inline void CalIWeakenPID(PID *pstPid)
{
    pstPid->fpE=pstPid->fpDes-pstPid->fpFB;//计算当前偏差

    if(fabs(pstPid->fpE) < pstPid->fpEMin)
    {
        pstPid->fpSumE += pstPid->fpE;//计算偏差累积
    }

    pstPid->fpSumE = Clip(pstPid->fpSumE, -pstPid->fpEiMax, pstPid->fpEiMax);
    pstPid->fpUi = pstPid->fpKi * pstPid->fpSumE;

    pstPid->fpUp = Clip(pstPid->fpKp * pstPid->fpE, -pstPid->fpEpMax, pstPid->fpEpMax);
    pstPid->fpUd = Clip(pstPid->fpKd * (pstPid->fpE - pstPid->fpPreE), -pstPid->fpEdMax, pstPid->fpEdMax);

    /*位置式PID计算公式*/
    pstPid->fpU = pstPid->fpUp + pstPid->fpUi + pstPid->fpUd;

    pstPid->fpPreE = pstPid->fpE;//保存本次偏差

    /*PID运算输出限幅*/
    pstPid->fpU = Clip(pstPid->fpU, -pstPid->fpUMax, pstPid->fpUMax);
}

/*--------------------------------------------------------------------------------------------------
函数名称：Clip()
函数功能：削波函数，去除超出最大值与最小值之间的值，代之以最大或最小值
--------------------------------------------------------------------------------------------------*/
FP32 Clip(FP32 fpValue, FP32 fpMin, FP32 fpMax)
{
    if(fpValue <= fpMin)
    {
        return fpMin;
    }
    else if(fpValue >= fpMax)
    {
        return fpMax;
    }
    else
    {
        return fpValue;
    }
}

/*******************************************************************
函数名称：Round()
函数功能：将浮点数四舍五入，返回32位整型数
********************************************************************/
SSHORT16 Round(FP32 fp_Round_Number)
{
    if (fp_Round_Number >= 0)
    {
        return (SSHORT16)(fp_Round_Number + 0.5f);
    }
    else
    {
        return (SSHORT16)(fp_Round_Number - 0.5f);
    }
}

/*******************************************************************
函数名称：Sign_Judge(FP32 fp_Any_Number)
函数功能：判断正负
备    注：返回值为1和-1，来改变数的符号
********************************************************************/
SINT32 Sign_Judge(FP32 fp_Judge_Number)
{
    if(fp_Judge_Number > 0)
        return 1;
    else if(fp_Judge_Number < 0)
        return -1;
    else
        return 0;
}

bool RampSignal(float* p_Output, float DesValue, float Step)    //传入指针量向目标值按Step步进，最终稳在DesValue
{
    if(fabs(*p_Output-DesValue) <= Step)
    {
        *p_Output = DesValue;
        return TRUE;
    }
    else
    {
        if(*p_Output < DesValue)
            *p_Output += Step;
        else
            *p_Output -= Step;

        return FALSE;
    }
}

bool RampSignal_s16(s16* p_Output, s16 DesValue, s16 Step)  //传入指针量向目标值按Step步进，最终稳在DesValue
{
    if(abs(*p_Output-DesValue) <= Step)
    {
        *p_Output = DesValue;
        return TRUE;
    }
    else
    {
        if(*p_Output < DesValue)
            *p_Output += Step;
        else
            *p_Output -= Step;

        return FALSE;
    }
}
/**
* @brief 	通过TD跟踪微分器计算速度x2和位置x1
* @param 	pStTD	TD数据结构体 
* @comment	m_aim为真实期望位置，x1为过渡期望位置，x2为过渡期望速度
 */
void CalTD(ST_TD *pStTD)
{
//	float d,d0,y,a0,a=0,fhan;
//	pStTD->m_x = pStTD->m_x1 - pStTD->m_aim;
//	d = pStTD->m_r * pStTD->m_h;
//	d0 = pStTD->m_h * d;
//	y = pStTD->m_x + pStTD->m_h * pStTD->m_x2;
//	a0 = sqrt(d * d + 8 * pStTD->m_r * fabs(y));

//	if(fabs(y) > d0)
//		a = pStTD->m_x2 + (a0 - d) * Sgn(y) / 2;
//	else
//		a = pStTD->m_x2 + y / pStTD->m_h;

//	if(fabs(a) > d)
//		fhan = -1 * pStTD->m_r * Sgn(a);
//	else
//		fhan = -1 * pStTD->m_r * a / d;

//	pStTD->m_x1 += pStTD->m_T * pStTD->m_x2;
//	pStTD->m_x2 += pStTD->m_T * fhan;
	
	float d,d0,y,a0,a=0;
    pStTD->m_x = pStTD->m_x1 - pStTD->m_aim;
    d = pStTD->m_r * pStTD->m_h;
    d0=pStTD->m_h * d;
    y = pStTD->m_x + pStTD->m_h * pStTD->m_x2;
    a0 = sqrt(d*d+8*pStTD->m_r * fabs(y));

    if(fabs(y)>d0)
        a = pStTD->m_x2 +(a0-d) * Sign_Judge(y)/2;
    else
        a = pStTD->m_x2 + y/pStTD->m_h;

    if(fabs(a)>d)
        y=-1*pStTD->m_r * Sign_Judge(a);
    else
        y=-1*pStTD->m_r * a/d;

    pStTD->m_x1 +=  pStTD->m_T*pStTD->m_x2;
    pStTD->m_x2 +=  pStTD->m_T*y;
	
}



/*******************************************************************
函数名称：CalSMC(ST_SMC *pStSMC)
函数功能：滑模控制算法
备    注：
********************************************************************/
void CalSMC(ST_SMC *pStSMC)
{
		pStSMC->TD. m_aim= pStSMC->fpDes;	
		CalTD(&pStSMC->TD);
		pStSMC->fpE = pStSMC->TD.m_x1- pStSMC->fpFB;
		pStSMC->fpU = 1 / pStSMC->b * (pStSMC->TD.m_x2
		+ pStSMC->eps * SMC_SatFunc(pStSMC->fpE, pStSMC->dead)
		+ pStSMC->gain * pStSMC->fpE);
		pStSMC->fpU = Clip(pStSMC->fpU, -pStSMC->fpUMax, pStSMC->fpUMax);
}



float SMC_SatFunc(float in, float d)
{
		if(fabs(in) >= d)
			return Sign_Judge(in);
		else
			return in / d;
}
//这里将符号函数sgn()换成了饱和函数sat()，目的是防止抖震。




u16 USART_Receive(USART_RX_TypeDef* USARTx)
{
    USARTx->rxConter = USARTx->DMALen - DMA_GetCurrDataCounter(USARTx->DMAy_Streamx);   //本次DMA缓冲区填充到的位置

    USARTx->rxBufferPtr += USARTx->rxSize;      //上次DMA缓冲区填充到的位置

    if(USARTx->rxBufferPtr >= USARTx->DMALen)   //说明DMA缓冲区已经满了一次
    {
        USARTx->rxBufferPtr %= USARTx->DMALen;
    }

    if(USARTx->rxBufferPtr < USARTx->rxConter)
    {
        USARTx->rxSize = USARTx->rxConter - USARTx->rxBufferPtr;//计算本次接收数据长度

        if(USARTx->rxSize <= USARTx->MbLen)
        {
            for(u16 i=0; i<USARTx->rxSize; i++)
                *(USARTx->pMailbox + i) = *(USARTx->pDMAbuf + USARTx->rxBufferPtr + i);
        }
    }
    else
    {
        USARTx->rxSize = USARTx->rxConter + USARTx->DMALen - USARTx->rxBufferPtr;   //计算本次接收数据长度

        if(USARTx->rxSize <= USARTx->MbLen)     //接收的数据长度与期望数据长度相同，则把数据写进邮箱
        {
            for(u16 i=0; i<USARTx->rxSize-USARTx->rxConter; i++)
                *(USARTx->pMailbox + i) = *(USARTx->pDMAbuf + USARTx->rxBufferPtr + i);

            for(u16 i=0; i<USARTx->rxConter; i++)
                *(USARTx->pMailbox + USARTx->rxSize-USARTx->rxConter + i) = *(USARTx->pDMAbuf + i);
        }
    }

    return USARTx->rxSize;                      //返回本次空闲中断一共接收多少字节
}

void Sensor_Data_Process(void)
{
	
	sensor_data1 = (USART2_Rx_DMAbuff[6] - 0x30) * 65536\
				+ (USART2_Rx_DMAbuff[5] - 0x30) * 4096\
				+ (USART2_Rx_DMAbuff[4] - 0x30) * 256\
				+ (USART2_Rx_DMAbuff[3] - 0x30) * 16\
				+ (USART2_Rx_DMAbuff[2] - 0x30);
	
	hexToBinary(USART2_Rx_DMAbuff[7],binaryArray1);
	if (binaryArray1[0] == 1 && binaryArray1[1] == 0)
	{
		sensor_data1 *= 0.1f;
	}
	
	
	sensor_data2 = (USART3_Rx_DMAbuff[6] - 0x30) * 65536\
				+ (USART3_Rx_DMAbuff[5] - 0x30) * 4096\
				+ (USART3_Rx_DMAbuff[4] - 0x30) * 256\
				+ (USART3_Rx_DMAbuff[3] - 0x30) * 16\
				+ (USART3_Rx_DMAbuff[2] - 0x30);
	hexToBinary(USART3_Rx_DMAbuff[7],binaryArray2);
	if (binaryArray2[0] == 1 && binaryArray2[1] == 0)
	{
		sensor_data1 *= 0.1f;
	}
	
}

void hexToBinary(uint8_t hexValue, uint16_t binaryArray[]) 
{
	int i;
	for (i = 7; i >= 0; i--) {
		binaryArray[i] = hexValue % 2;
		hexValue /= 2;
	}
}
