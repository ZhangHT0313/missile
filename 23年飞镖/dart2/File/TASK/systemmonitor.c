#include "stm32f4xx.h"                  // Device header
#include "systemmonitor.h"
#include "global.h"

void SystemMonitorTask(void)
{
	systemmonitor.sd_fps = systemmonitor.sd_counter;
	systemmonitor.sd_counter = 0;
	
	
}

