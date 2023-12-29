#include "stm32f4xx.h"                  // Device header
#include "systemmonitor.h"
#include "global.h"

void SystemMonitorTask(void)
{
	systemmonitor.data_main2_fps = systemmonitor.data_main2_counter;
	systemmonitor.data_main2_counter = 0;
	
	systemmonitor.data_sensor_fps = systemmonitor.data_sensor_counter;
	systemmonitor.data_sensor_counter = 0;
	
}

