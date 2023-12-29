#ifndef __SD_H__
#define __SD_H__                
#include "global.h"
#include "hitcrt_types.h"
#include "TF_Card_SPI_DMA.h"
#include "stdarg.h"
#include "stdio.h"

extern FATFS FFats;
extern FRESULT Mount_Result;
extern FRESULT File_Result;

void SD_Mount(void);
void SD_printf(const char* fmt, ...);
void DataRecord_Control(void);

#endif
