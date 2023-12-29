#include "stm32f4xx.h"                  // Device header
#include "BSPInit.h"
void Bsp_Init(void)
{
	/*OS时钟使能*/
	TIM5_Configuration();
	
	/*GPIO初始化*/
	GPIO_Configuration();
	
	/*can通讯使能*/
	CAN1_Configuration();
	CAN2_Configuration();
	
	
	/*PWM Init*/
	TIM9_PWM_Init(2000-1,1680-1);//舵机周期20ms
	TIM8_PWM_Init(2000-1,1680-1);//舵机周期20ms

	/*串口初始化*/
	USART3_Configuration();
	USART2_Configuration();
	UART4_Configuration();
	UART5_Configuration();

	
}
