#include "stm32f4xx.h"                  // Device header
#include "algorithm.h"

/*friction��PID�㷨*/

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
	pid->fpUi=pid->fpKi*pid->fpSumE/1000;//���ֿ��ܵ�����ԭ���ǳ���float���͵Ĵ洢��Χ���ʽ���1000��ɳ�500����Ч,����ԭ���Ǵﵽ�㵫��еԭ���²���
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
	pid->fpUi=pid->fpKi*pid->fpSumE/1000;//���ֿ��ܵ�����ԭ���ǳ���float���͵Ĵ洢��Χ���ʽ���1000��ɳ�500����Ч,����ԭ���Ǵﵽ�㵫��еԭ���²���
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

//����PID�ļ��㺯��
//����(PID�ṹ��,�⻷Ŀ��ֵ,�⻷����ֵ,�ڻ�����ֵ)
void PID_CascadeCalc(CascadePID *pid,float outerRef,float outerFdb,float innerFdb)
{
    Abp_PID(&pid->outer);//�����⻷
//	pid->outer.fpU = pid->inner.fpDes;
    Abp_PID(&pid->inner);//�����ڻ�
    pid->output=pid->inner.fpU;//�ڻ�������Ǵ���PID�����
}





/*λ��ʽPID�㷨 u(k)=Kp*E(K)+Ki*sum(E(K))+Kd*(E(K)-E(K-1))*/

/*******************************************************************
�������ƣ�CalPID(ST_PID *pstPid)
�������ܣ���ͨ��PID�㷨����PID��
��    ע��
********************************************************************/
void CalPID(PID *pstPid)
{
    pstPid->fpE = pstPid->fpDes - pstPid->fpFB;//���㵱ǰƫ��
    if(fabs(pstPid->fpE) <= pstPid->fpEMin)//ƫ����������
    {
        pstPid->fpE = 0;
    }
    pstPid->fpSumE += pstPid->fpE;
    /*λ��ʽPID���㹫ʽ*/
    pstPid->fpU = pstPid->fpKp * pstPid->fpE
                  + pstPid->fpKi * pstPid->fpSumE
                  + pstPid->fpKd * (pstPid->fpE - pstPid->fpPreE);
    pstPid->fpPreE = pstPid->fpE;//���汾��ƫ��
    /*PID��������޷�*/
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
�������ƣ�CalISeparatedPID(ST_PID *pstPid)
�������ܣ����ַ���ʽPID�㷨����PID��
��    ע�����ַ���ʽPID�Ľ��㷨�ɼ�С������ֹͣ����������ʱ�ϴ�ƫ��
          �Ի�����Ļ��ۣ��Ӷ�������ֽϴ�ĳ�����������
********************************************************************/
void CalISeparatedPID(PID *pstPid)
{
    UCHAR8 uck=1;

    pstPid->fpE=pstPid->fpDes-pstPid->fpFB;//���㵱ǰƫ��
    if(fabs(pstPid->fpE) <= pstPid->fpEMin)//ƫ����������
    {
        pstPid->fpE = 0;
    }
    pstPid->fpSumE += pstPid->fpE;//����ƫ���ۻ�
    /*��ƫ������������ۻ�ƫ��*/
    if(fabs(pstPid->fpE) > pstPid->fpEiMax )//�ж��Ƿ�������ַ���
    {
        uck=0;
    }
    /*λ��ʽPID���㹫ʽ*/
    pstPid->fpU = pstPid->fpKp * pstPid->fpE
                  + pstPid->fpKi * pstPid->fpSumE * uck
                  + pstPid->fpKd * (pstPid->fpE - pstPid->fpPreE);
    pstPid->fpPreE = pstPid->fpE;//���汾��ƫ��
    /*PID��������޷�*/
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
�������ƣ�CalIResistedPID(ST_PID *pstPid)
�������ܣ������ֱ���PID�㷨
��    ע��ϵͳ��һ�������˶�������ϴ���������ڼ��������ڲ����񵴻򳬵�
********************************************************************/
void CalIResistedPID(PID *pstPid)
{

    pstPid->fpE=pstPid->fpDes-pstPid->fpFB;   //���㵱ǰƫ��
    pstPid->fpSumE += pstPid->fpE;   //����ƫ���ۻ�

    pstPid->fpSumE = Clip(pstPid->fpSumE, -pstPid->fpEiMax, pstPid->fpEiMax);
    pstPid->fpUi = pstPid->fpKi * pstPid->fpSumE;

    pstPid->fpUp = Clip(pstPid->fpKp * pstPid->fpE, -pstPid->fpEpMax, pstPid->fpEpMax);
    pstPid->fpUd = Clip(pstPid->fpKd * (pstPid->fpE - pstPid->fpPreE), -pstPid->fpEdMax, pstPid->fpEdMax);


    /*��ƫ��������֮�ڣ�����������ۼ���*/
    if(fabs(pstPid->fpE) < pstPid->fpEMin)   //�ж��Ƿ�������ֱ�������
    {
        pstPid->fpSumE = 0;   //���ƫ���ۻ�
    }
    /*λ��ʽPID���㹫ʽ*/
    pstPid->fpU = pstPid->fpUp + pstPid->fpUi + pstPid->fpUd;

    pstPid->fpPreE = pstPid->fpE;//���汾��ƫ��
    /*PID��������޷�*/
    pstPid->fpU = Clip(pstPid->fpU, -pstPid->fpUMax, pstPid->fpUMax);
}

/*******************************************************************
�������ƣ�CalIWeakenPID(ST_PID *pstPid)
�������ܣ�������������PID�Ľ��㷨����PID��
��    ע��
********************************************************************/
inline void CalIWeakenPID(PID *pstPid)
{
    pstPid->fpE=pstPid->fpDes-pstPid->fpFB;//���㵱ǰƫ��

    if(fabs(pstPid->fpE) < pstPid->fpEMin)
    {
        pstPid->fpSumE += pstPid->fpE;//����ƫ���ۻ�
    }

    pstPid->fpSumE = Clip(pstPid->fpSumE, -pstPid->fpEiMax, pstPid->fpEiMax);
    pstPid->fpUi = pstPid->fpKi * pstPid->fpSumE;

    pstPid->fpUp = Clip(pstPid->fpKp * pstPid->fpE, -pstPid->fpEpMax, pstPid->fpEpMax);
    pstPid->fpUd = Clip(pstPid->fpKd * (pstPid->fpE - pstPid->fpPreE), -pstPid->fpEdMax, pstPid->fpEdMax);

    /*λ��ʽPID���㹫ʽ*/
    pstPid->fpU = pstPid->fpUp + pstPid->fpUi + pstPid->fpUd;

    pstPid->fpPreE = pstPid->fpE;//���汾��ƫ��

    /*PID��������޷�*/
    pstPid->fpU = Clip(pstPid->fpU, -pstPid->fpUMax, pstPid->fpUMax);
}

/*--------------------------------------------------------------------------------------------------
�������ƣ�Clip()
�������ܣ�����������ȥ���������ֵ����Сֵ֮���ֵ����֮��������Сֵ
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
�������ƣ�Round()
�������ܣ����������������룬����32λ������
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
�������ƣ�Sign_Judge(FP32 fp_Any_Number)
�������ܣ��ж�����
��    ע������ֵΪ1��-1�����ı����ķ���
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

bool RampSignal(float* p_Output, float DesValue, float Step)    //����ָ������Ŀ��ֵ��Step��������������DesValue
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

bool RampSignal_s16(s16* p_Output, s16 DesValue, s16 Step)  //����ָ������Ŀ��ֵ��Step��������������DesValue
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
* @brief 	ͨ��TD����΢���������ٶ�x2��λ��x1
* @param 	pStTD	TD���ݽṹ�� 
* @comment	m_aimΪ��ʵ����λ�ã�x1Ϊ��������λ�ã�x2Ϊ���������ٶ�
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
�������ƣ�CalSMC(ST_SMC *pStSMC)
�������ܣ���ģ�����㷨
��    ע��
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
//���ｫ���ź���sgn()�����˱��ͺ���sat()��Ŀ���Ƿ�ֹ����




u16 USART_Receive(USART_RX_TypeDef* USARTx)
{
    USARTx->rxConter = USARTx->DMALen - DMA_GetCurrDataCounter(USARTx->DMAy_Streamx);   //����DMA��������䵽��λ��

    USARTx->rxBufferPtr += USARTx->rxSize;      //�ϴ�DMA��������䵽��λ��

    if(USARTx->rxBufferPtr >= USARTx->DMALen)   //˵��DMA�������Ѿ�����һ��
    {
        USARTx->rxBufferPtr %= USARTx->DMALen;
    }

    if(USARTx->rxBufferPtr < USARTx->rxConter)
    {
        USARTx->rxSize = USARTx->rxConter - USARTx->rxBufferPtr;//���㱾�ν������ݳ���

        if(USARTx->rxSize <= USARTx->MbLen)
        {
            for(u16 i=0; i<USARTx->rxSize; i++)
                *(USARTx->pMailbox + i) = *(USARTx->pDMAbuf + USARTx->rxBufferPtr + i);
        }
    }
    else
    {
        USARTx->rxSize = USARTx->rxConter + USARTx->DMALen - USARTx->rxBufferPtr;   //���㱾�ν������ݳ���

        if(USARTx->rxSize <= USARTx->MbLen)     //���յ����ݳ������������ݳ�����ͬ���������д������
        {
            for(u16 i=0; i<USARTx->rxSize-USARTx->rxConter; i++)
                *(USARTx->pMailbox + i) = *(USARTx->pDMAbuf + USARTx->rxBufferPtr + i);

            for(u16 i=0; i<USARTx->rxConter; i++)
                *(USARTx->pMailbox + USARTx->rxSize-USARTx->rxConter + i) = *(USARTx->pDMAbuf + i);
        }
    }

    return USARTx->rxSize;                      //���ر��ο����ж�һ�����ն����ֽ�
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
