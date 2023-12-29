#include "stm32f4xx.h"                  // Device header
#include "main.h"

int main(void)
{ 
	Bsp_Init();
	
	OS_RUN();
	
}

/*****TASKS******/

void SD_Task(void)
{
	DataRecord_Control();
	systemmonitor.sd_counter++;
}





