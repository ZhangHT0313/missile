#include "usart.h"


void USART2_Configuration(void)
{
    GPIO_InitTypeDef	gpio;
    USART_InitTypeDef	usart;
    NVIC_InitTypeDef 	nvic;
    DMA_InitTypeDef		dma;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);      //ʹ��USART2ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);   //TX
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);   //RX

    gpio.GPIO_OType = GPIO_OType_PP;//PP
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOD, &gpio);

    USART_DeInit(USART2);
    usart.USART_BaudRate			= 460800;//��������
    usart.USART_WordLength		=	USART_WordLength_8b;
    usart.USART_StopBits			=	USART_StopBits_1;
    usart.USART_Parity				=	USART_Parity_No;
    usart.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
    usart.USART_Mode			  	=	USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &usart);

    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	//���������ж�
    USART_Cmd(USART2, ENABLE);                      //ʹ�ܴ���
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  //ʹ�ܴ��ڷ���DMA
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);  //ʹ�ܴ��ڽ���DMA

    nvic.NVIC_IRQChannel                   = USART2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;     //��ռ���ȼ�
    nvic.NVIC_IRQChannelSubPriority        = 2;     //�����ȼ�
    nvic.NVIC_IRQChannelCmd                = ENABLE;//IRQͨ��ʹ��
    NVIC_Init(&nvic);           //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART2_Tx
    DMA_DeInit(USART2_TX_STREAM);
    while( DMA_GetCmdStatus(USART2_TX_STREAM) == ENABLE );			//�ȴ�DMA������

    dma.DMA_Channel			      	=  DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr	=	(uint32_t)&(USART2->DR);
    dma.DMA_Memory0BaseAddr		  =	(uint32_t)UA2TxDMAbuf;//
    dma.DMA_DIR				        	=	DMA_DIR_MemoryToPeripheral;	//�ڴ浽����
    dma.DMA_BufferSize		     	=	UA2TxDMAbuf_LEN;//
    dma.DMA_PeripheralInc		    =	DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc		      	=	DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize	=	DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize		  =	DMA_MemoryDataSize_Byte;
    dma.DMA_Mode				        =	DMA_Mode_Normal;			//ѭ������
    dma.DMA_Priority			      =	DMA_Priority_VeryHigh;
    dma.DMA_FIFOMode			      =	DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold		    =	DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst			    =	DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst		  =	DMA_PeripheralBurst_Single;
    DMA_Init(USART2_TX_STREAM, &dma);
    DMA_Cmd(USART2_TX_STREAM, DISABLE);	//ʧ��DMA

    //USART2_Rx
    DMA_DeInit(USART2_RX_STREAM);
    while( DMA_GetCmdStatus(USART2_RX_STREAM) == ENABLE );			//�ȴ�DMA������

    dma.DMA_Channel            =   DMA_Channel_4;				//��DMA_Channel_4
    dma.DMA_PeripheralBaseAddr = 	(uint32_t)&(USART2->DR);
    dma.DMA_Memory0BaseAddr    = 	(uint32_t)UA2RxDMAbuf;
    dma.DMA_DIR                = 	DMA_DIR_PeripheralToMemory;	//���赽�ڴ�
    dma.DMA_BufferSize         = 	UA2RxDMAbuf_LEN;
    dma.DMA_Mode               = 	DMA_Mode_Circular;			//ѭ������
    dma.DMA_Priority           = 	DMA_Priority_VeryHigh;
    DMA_Init(USART2_RX_STREAM, &dma);
    DMA_Cmd(USART2_RX_STREAM, ENABLE);	//ʹ��DMA
}

