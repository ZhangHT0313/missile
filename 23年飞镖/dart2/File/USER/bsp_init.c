#include "stm32f4xx.h"                  // Device header
#include "BSPInit.h"
void Bsp_Init(void)
{
	/*OSʱ��ʹ��*/
	TIM5_Configuration();
	
	
	/*canͨѶʹ��*/
	CAN1_Configuration();
	CAN2_Configuration();
	
	
	/*PWM Init*/
	TIM9_PWM_Init(2000-1,1680-1);//�������20ms
	TIM8_PWM_Init(2000-1,1680-1);//�������20ms

	/*���ڳ�ʼ��*/
	USART2_Configuration();

	
}
