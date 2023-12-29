#include "stm32f4xx.h"                  // Device header
#include "main.h"

int main(void)
{ 
	Bsp_Init();
	
	OS_RUN();
	
}

/*****TASKS******/


void Data_Send_MainControl2_Task(void)
{
	Send_Data_To_MainControl2();
	systemmonitor.data_main2_counter++;
}

void Data_Send_Sensor_Task(void)
{
	Send_Data_To_Sensor1();
	Send_Data_To_Sensor2();
	systemmonitor.data_sensor_counter++;
}





