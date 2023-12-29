#ifndef __MAIN_H__
#define __MAIN_H__ 
#include "stm32f4xx.h" 
#include "os.h"
#include "BSPInit.h"

#include "data_send_task.h"


void Data_Send_MainControl2_Task(void);
void Data_Send_Sensor_Task(void);



#endif


