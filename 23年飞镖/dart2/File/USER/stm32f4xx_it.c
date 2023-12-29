/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "global.h"
#include "hitcrt_types.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void USART1_IRQHandler(void)
{
}


/*与主控一通讯*/
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
    {
        USART2->SR;
        USART2->DR;		//先读SR后读DR才能清除IDLE中断标志位

				if(DMA_GetCurrDataCounter(USART2_RX_STREAM) ==UA2RxDMAbuf_LEN)
        {
					DataReceived_Flag = TRUE;
					Data_u8_to_int();						
        }
        else
        {
          DMA_Cmd(USART2_RX_STREAM, DISABLE);         //设置当前计数值前先禁用DMA
          USART2_RX_STREAM->NDTR = UA2RxDMAbuf_LEN;   //设置当前待发的数据的数量:Number of Data units to be TRansferred
          DMA_Cmd(USART2_RX_STREAM, ENABLE);          //启用串口DMA发送
        }	

    }
}

void Data_u8_to_int(void)
{
	int i=0;
	while(i<64)
	{
		Data_From_C1[i]=u8_to_int(UA2RxDMAbuf,i);
		i++;
	}
}

int u8_to_int(u8*rx ,int n)
{
		int data;
    data=rx[4*n+0]<<24|rx[4*n+1]<<16|rx[4*n+2]<<8|rx[4*n+3];
	  return data;
}
