#include "stm32f4xx.h"                  // Device header
#include "global.h"

//led任务
SystemMonitor_struct ledmonitor={0};
uint16_t ledpwm_counter = 0;

/***********************CAN**************************/
/***********************CAN**************************/
/***********************CAN**************************/


uint32_t g_uiAirValve =0;

int16_t current_lift;     	
int16_t current_clip;
int16_t current_rotate;


ST_ENCODER encode1 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode12 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode2 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode3 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode4 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode5 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode6 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode7 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode8 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encode9 = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encodeA = {.Number = 8191, .SumValue = 0, .RawValue = 0};
ST_ENCODER encodeB = {.Number = 8191, .SumValue = 0, .RawValue = 0};


u16 value1;      
u16 value12;
u16 value2;
u16 value3;
u16 value4;
u16 value5;
u16 value6;
u16 value7;
u16 value8;
u16 value9;
u16 valueA;
u16 valueB;

s16 speed1;
s16 speed12;
s16 speed2;
s16 speed3;
s16 speed4;
s16 speed5;
s16 speed6;
s16 speed7;
s16 speed8;
s16 speed9;
s16 speedA;
s16 speedB;

