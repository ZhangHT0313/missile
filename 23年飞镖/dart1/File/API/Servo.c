#include "Servo.h"

void Servo_procontrol(void)
{
	static uint16_t time_now;

	time_now++;
	if ( time_now < servo_time)
	{
		Compare_Servo2 = 50;
//		TIM_SetCompare2(TIM9,Compare_Servo1);//PWM1
		TIM_SetCompare3(TIM8,Compare_Servo2);//PWM2
//		TIM_SetCompare4(TIM8,Compare_Servo3);//PWM3

	}

	if (time_now >= servo_time)
	{
		Compare_Servo2 = 0;
		TIM_SetCompare3(TIM8,Compare_Servo2);//PWM2
	}
	
}
