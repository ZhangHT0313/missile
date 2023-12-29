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


/** --------------------------------------------------------------------------
  * @brief  USART1中断服务函数
			
  * @note	遥控器
 -------------------------------------------------------------------------- **/

void USART1_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
	//clear the idle pending flag 
		USART1->SR;
		USART1->DR;
	//Target is Memory0

		RemoteDataProcess((u8*)sbus_rx_buffer[0]);
		
		DMA_Cmd(USART1_RX_STREAM, DISABLE);
		USART1_RX_STREAM->NDTR = (uint16_t)RC_FRAME_LENGTH; //relocate the dma memory pointer to the beginning position
//		USART1_RX_STREAM->CR |= (uint32_t)(DMA_SxCR_CT); //enable the current selected memory is Memory 1
		DMA_Cmd(USART1_RX_STREAM, ENABLE);
		
		systemmonitor.usart1_rx++;
	}
}


void USART2_IRQHandler(void)                	
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
	//clear the idle pending flag 
		USART2->SR;
		USART2->DR;
	//Target is Memory0

		Sensor_Data_Process();
		
		DMA_Cmd(USART2_RX_STREAM, DISABLE);
		USART2_RX_STREAM->NDTR = (uint16_t)RC_FRAME_LENGTH; //relocate the dma memory pointer to the beginning position
//		USART1_RX_STREAM->CR |= (uint32_t)(DMA_SxCR_CT); //enable the current selected memory is Memory 1
		DMA_Cmd(USART2_RX_STREAM, ENABLE);
		
		systemmonitor.usart2_rx++;
	}
}


void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
	//clear the idle pending flag 
		USART3->SR;
		USART3->DR;
	//Target is Memory0

		Sensor_Data_Process();
		
		DMA_Cmd(USART3_RX_STREAM, DISABLE);
		USART3_RX_STREAM->NDTR = (uint16_t)RC_FRAME_LENGTH; //relocate the dma memory pointer to the beginning position
//		USART1_RX_STREAM->CR |= (uint32_t)(DMA_SxCR_CT); //enable the current selected memory is Memory 1
		DMA_Cmd(USART3_RX_STREAM, ENABLE);
		
		systemmonitor.usart3_rx++;
	}
}


/** --------------------------------------------------------------------------
  * @brief  USART4中断服务函数
			
  * @note	与主控2通信
 -------------------------------------------------------------------------- **/

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
   {
		//clear the idle pending flag 
		UART4->SR;
		UART4->DR;
	}


}


/** --------------------------------------------------------------------------
  * @brief  USART5中断服务函数
			
  * @note	与裁判系统通信
 -------------------------------------------------------------------------- **/
void UART5_IRQHandler(void)
{
    if(USART_GetITStatus(UART5, USART_IT_IDLE) == SET)
    {
        UART5->SR;
        UART5->DR;		//先读SR后读DR才能清除IDLE中断标志位
        if(1)
        {
					USART_Receive(&UART5_Rcr);
					Rc_RsysProtocol();          //运算最大消耗时间不超过40us
					systemmonitor.uart5_rx++;
        }
    }
}


void CAN1_RX0_IRQHandler(void)
{
	
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)		//获取空闲中断标志位
	{
		CanRxMsg CanRxStruc;
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);		//清除中断标志位
		CAN_Receive(CAN1,CAN_FIFO0,&CanRxStruc);
		switch(CanRxStruc.StdId)
		{
			//track
			case 0x201:
				Track_Encoder1.RawValue = GetEncodeNumber(&CanRxStruc);
				Track_Encoder1.fpSpeed = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&Track_Encoder1,Track_Encoder1.RawValue);
				break;
			case 0x202:
				Track_Encoder2.RawValue = GetEncodeNumber(&CanRxStruc);
				Track_Encoder2.fpSpeed = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&Track_Encoder2,Track_Encoder2.RawValue);
			
			//pitch
			case 0x203:
				pitch_encoder.RawValue = GetEncodeNumber(&CanRxStruc);
				pitch_encoder.fpSpeed = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&pitch_encoder,pitch_encoder.RawValue);
			
			default:
				break;
		}
	}
	
}

void CAN2_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=RESET)		//获取空闲中断标志位
	{
		CanRxMsg CanRxStruc;
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);		//清除中断标志位
		CAN_Receive(CAN2,CAN_FIFO0,&CanRxStruc);
		switch(CanRxStruc.StdId)
		{
			//trigger
			case 0x201:
				trigger_encoder.RawValue = GetEncodeNumber(&CanRxStruc);
				trigger_encoder.fpSpeed = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&trigger_encoder,trigger_encoder.RawValue);
				break;
			
			//synchronous
			case 0x202:
				synchronous_encoder1.RawValue = GetEncodeNumber(&CanRxStruc);
				synchronous_encoder1.fpSpeed = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&synchronous_encoder1,synchronous_encoder1.RawValue);
			case 0x203:
				synchronous_encoder2.RawValue = GetEncodeNumber(&CanRxStruc);
				synchronous_encoder2.fpSpeed = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&synchronous_encoder2,synchronous_encoder2.RawValue);
			
			case 0x205:
				yaw_encoder.RawValue = GetEncodeNumber(&CanRxStruc);
				yaw_encoder.fpSpeed = GetSpeed(&CanRxStruc);
				Abs_Encoder_Process(&yaw_encoder,yaw_encoder.RawValue);
			default:
				break;
		}
	}
}

