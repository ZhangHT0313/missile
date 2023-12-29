#include "usart.h"


/*--------------------------------------------------------------------------
函数功能：与力传感器1通信
--------------------------------------------------------------------------*/

void USART2_Configuration(void)
	{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟13579o
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//使能USART1时钟13579o
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIO复用为USART113579o
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIO复用为USART113579o
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //13579o
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//功能模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化GPIO13579o

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使能收发模式
	USART_Init(USART2,&USART_InitStructure); //初始化串口113579o
	
//	USART_Cmd(USART1,ENABLE);  //使能串口13579o

  	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器?
	
	//配置接受DMA
	DMA_DeInit(USART2_RX_STREAM);
    DMA_InitStructure.DMA_Channel                 =    DMA_Channel_4;                 //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr      =    (uint32_t)&(USART2->DR);				// ??DMA???????(?DMA?????1????)
    DMA_InitStructure.DMA_Memory0BaseAddr         =    (uint32_t)&USART2_Rx_DMAbuff;				// ??DMA???????(?????,??1????????????)
    DMA_InitStructure.DMA_DIR                     =    DMA_DIR_PeripheralToMemory;			// ????????(???????)
    DMA_InitStructure.DMA_BufferSize              =    USART2_Rx_DMAbuf_LEN;						// ??DMA??????????(500??)
    DMA_InitStructure.DMA_PeripheralInc           =    DMA_PeripheralInc_Disable;			// ????????
    DMA_InitStructure.DMA_MemoryInc               =    DMA_MemoryInc_Enable;				// ????????
    DMA_InitStructure.DMA_PeripheralDataSize      =    DMA_PeripheralDataSize_Byte;			// ????????????(8bits)
    DMA_InitStructure.DMA_MemoryDataSize          =    DMA_MemoryDataSize_Byte;				// ????????????(8bits)
    DMA_InitStructure.DMA_Mode                    =    DMA_Mode_Circular;					// ??DMA???????
    DMA_InitStructure.DMA_Priority                =    DMA_Priority_VeryHigh;				// ??DMA????????????
    DMA_InitStructure.DMA_FIFOMode                =    DMA_FIFOMode_Disable;				// ???FIFO??
    DMA_InitStructure.DMA_FIFOThreshold           =    DMA_FIFOThreshold_1QuarterFull;		// FIFO??
    DMA_InitStructure.DMA_MemoryBurst             =    DMA_Mode_Normal;						// ??????
    DMA_InitStructure.DMA_PeripheralBurst         =    DMA_PeripheralBurst_Single;			// ????????
	DMA_Init(USART2_RX_STREAM,&DMA_InitStructure);
	
	
	//配置发送DMA
	DMA_DeInit(USART2_TX_STREAM);
	while( DMA_GetCmdStatus(USART2_TX_STREAM) == ENABLE);									// ??DMA???
	
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART2->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)&USART2_Tx_DMAbuff;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_MemoryToPeripheral;				// ?????
	DMA_InitStructure.DMA_BufferSize 			= USART2_Tx_DMAbuf_LEN;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;				// ???????
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;						// ??????
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;							// ??????
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	DMA_Init(USART2_TX_STREAM, &DMA_InitStructure);


	/*?????DMA???*/
	USART_Cmd(USART2, ENABLE);  															// ????2
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);											// ??????	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);											// ????2??DMA
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);											// ????2??DMA
	DMA_Cmd(USART2_RX_STREAM, ENABLE);														// ????DMA
	DMA_Cmd(USART2_TX_STREAM, DISABLE);														// ????DMA(????????)
}

/*--------------------------------------------------------------------------
函数功能：与力传感器2通信
--------------------------------------------------------------------------*/

void USART3_Configuration(void)
{
	GPIO_InitTypeDef	gpio = {0};
	USART_InitTypeDef	usart = {0};
	NVIC_InitTypeDef 	nvic = {0};
	DMA_InitTypeDef		dma = {0};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3); //TX	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3); //RX
	
	gpio.GPIO_OType	=	GPIO_OType_PP;//PP
	gpio.GPIO_PuPd	=	GPIO_PuPd_UP;
	gpio.GPIO_Mode	=	GPIO_Mode_AF;
	gpio.GPIO_Pin	=	GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &gpio);
	
	USART_DeInit(USART3);
	usart.USART_BaudRate			=	9600;	//此处使用高波特率，1ms内发送字节数控制在40字节内
	usart.USART_WordLength			=	USART_WordLength_8b;
	usart.USART_StopBits			=	USART_StopBits_1;
	usart.USART_Parity				=	USART_Parity_No;
	usart.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
	usart.USART_Mode				=	USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &usart);

	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);				//串口3接收空闲中断
	USART_Cmd(USART3, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

	nvic.NVIC_IRQChannel					=	USART3_IRQn; 	//串口3接收空闲中断
    nvic.NVIC_IRQChannelPreemptionPriority	=	0;
    nvic.NVIC_IRQChannelSubPriority			=	10;
    nvic.NVIC_IRQChannelCmd					=	ENABLE;
    NVIC_Init(&nvic);

	//USART3_Tx
	DMA_DeInit(DMA1_Stream3);
	while( DMA_GetCmdStatus(DMA1_Stream3) == ENABLE );			//等待DMA可配置

	dma.DMA_Channel				=	DMA_Channel_4;
	dma.DMA_PeripheralBaseAddr	=	(uint32_t)&(USART3->DR);
	dma.DMA_Memory0BaseAddr		=	(u32)&USART3_Tx_DMAbuff;
	dma.DMA_DIR					=	DMA_DIR_MemoryToPeripheral;	//内存到外设
	dma.DMA_BufferSize			=	USART3_Tx_DMAbuf_LEN;
	dma.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc			=	DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize	=	DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize		=	DMA_MemoryDataSize_Byte;
	dma.DMA_Mode				=	DMA_Mode_Normal;			//正常发送
	dma.DMA_Priority			=	DMA_Priority_VeryHigh;
	dma.DMA_FIFOMode			=	DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold		=	DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_MemoryBurst			=	DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst		=	DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream3, &dma);
	DMA_Cmd(DMA1_Stream3, DISABLE);

	//USART3_Rx
	DMA_DeInit(DMA1_Stream1);
	while( DMA_GetCmdStatus(DMA1_Stream1) == ENABLE );			//等待DMA可配置

	dma.DMA_Channel            = 	DMA_Channel_4;				//即DMA_Channel_4
	dma.DMA_PeripheralBaseAddr = 	(uint32_t)&(USART3->DR);
	dma.DMA_Memory0BaseAddr    = 	(uint32_t)USART3_Rx_DMAbuff;
	dma.DMA_DIR                = 	DMA_DIR_PeripheralToMemory;	//外设到内存
	dma.DMA_BufferSize         = 	USART3_Rx_DMAbuf_LEN;
	dma.DMA_Mode               = 	DMA_Mode_Circular;			//循环接收
	dma.DMA_Priority           = 	DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Stream1, &dma);
	DMA_Cmd(DMA1_Stream1, ENABLE);
}


/*--------------------------------------------------------------------------
函数功能：主控之间通讯底层配置，
--------------------------------------------------------------------------*/
void UART4_Configuration(void)
{
	GPIO_InitTypeDef	gpio;
	USART_InitTypeDef	usart;
	NVIC_InitTypeDef 	nvic;
	DMA_InitTypeDef		dma;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);						//使能UART4时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA1,ENABLE);	//使能PC端口时钟

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);//Tx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);//Rx


    gpio.GPIO_OType = GPIO_OType_PP;//PP
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
	gpio.GPIO_Mode  = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOC, &gpio);

	nvic.NVIC_IRQChannel 					= UART4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority	= 0;		//抢占优先级
	nvic.NVIC_IRQChannelSubPriority			= 6;		//子优先级
	nvic.NVIC_IRQChannelCmd					= ENABLE;	//IRQ通道使能 
	NVIC_Init(&nvic);//根据指定的参数初始化NVIC寄存器

	usart.USART_BaudRate			= 460800;				//波特率
	usart.USART_WordLength			= USART_WordLength_8b;	//字长为8位数据格式
	usart.USART_StopBits			= USART_StopBits_1;		//一个停止位
	usart.USART_Parity				= USART_Parity_No;		//无奇偶校验位
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	usart.USART_Mode				= USART_Mode_Tx|USART_Mode_Rx;		//收发模式
    USART_Init(UART4, &usart);//初始化串口

    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);	//开启空闲中断
    USART_Cmd(UART4, ENABLE);	//使能串口
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);

    //UART4_Tx
    DMA_DeInit(UART4_TX_STREAM);
    while( DMA_GetCmdStatus(UART4_TX_STREAM) == ENABLE );			//等待DMA可配置
    
    dma.DMA_Channel            =    DMA_Channel_4;                 //外设地址
    dma.DMA_PeripheralBaseAddr =    (uint32_t)&(UART4->DR);
    dma.DMA_Memory0BaseAddr    =    (uint32_t)UART4_Tx_DMAbuff;
    dma.DMA_DIR                =    DMA_DIR_MemoryToPeripheral;    //DMA传输为单向
    dma.DMA_BufferSize         =    UART4_Tx_DMAbuf_LEN;               //设置DMA在传输区的长度
    dma.DMA_PeripheralInc      =    DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc          =    DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize =    DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize     =    DMA_MemoryDataSize_Byte;
    dma.DMA_Mode               =    DMA_Mode_Normal;
    dma.DMA_Priority           =    DMA_Priority_High;
    dma.DMA_FIFOMode           =    DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold      =    DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst        =    DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst    =    DMA_PeripheralBurst_Single;
    DMA_Init(UART4_TX_STREAM, &dma);
    DMA_Cmd(UART4_TX_STREAM, DISABLE);
    
    //UART4_Rx
	DMA_DeInit(UART4_RX_STREAM);
	while( DMA_GetCmdStatus(UART4_RX_STREAM) == ENABLE );			//等待DMA可配置

	dma.DMA_Channel            =    DMA_Channel_4;				//即DMA_Channel_4
	dma.DMA_PeripheralBaseAddr = 	(uint32_t)&(UART4->DR);
	dma.DMA_Memory0BaseAddr    = 	(uint32_t)UART4_Rx_DMAbuff;
	dma.DMA_DIR                = 	DMA_DIR_PeripheralToMemory;	//外设到内存
	dma.DMA_BufferSize         = 	UART4_Rx_DMAbuf_LEN;
	dma.DMA_Mode               = 	DMA_Mode_Circular;			//循环接收
	dma.DMA_Priority           = 	DMA_Priority_VeryHigh;
	DMA_Init(UART4_RX_STREAM, &dma);
	DMA_Cmd(UART4_RX_STREAM, ENABLE);	//使能DMA
}



void UART5_Configuration(void)
{
	GPIO_InitTypeDef	gpio = {0};
	USART_InitTypeDef	usart = {0};
	NVIC_InitTypeDef 	nvic = {0};
	DMA_InitTypeDef		dma = {0};

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); 	//TX
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5); 	//RX
	
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_PuPd 	= GPIO_PuPd_UP;
  gpio.GPIO_Mode 	= GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_Pin 	= GPIO_Pin_12;
	GPIO_Init(GPIOC, &gpio);//TX
    
	gpio.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &gpio);//RX
	
	USART_DeInit(UART5);
	usart.USART_BaudRate			= 115200;//裁判系统
	usart.USART_WordLength			= USART_WordLength_8b;
	usart.USART_StopBits			= USART_StopBits_1;
	usart.USART_Parity				= USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode				= USART_Mode_Rx | USART_Mode_Tx;//发送中断
	USART_Init(UART5, &usart);
    
  USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);   //开启空闲中断
	USART_Cmd(UART5, ENABLE);                       //使能串口
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);   //使能串口发送DMA
	USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);   //使能串口接收DMA
    
  nvic.NVIC_IRQChannel					= UART5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority  = 0;
	nvic.NVIC_IRQChannelSubPriority			= 6;
	nvic.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&nvic);

    //UART5_Tx
  DMA_DeInit(DMA1_Stream7);
	while( DMA_GetCmdStatus(DMA1_Stream7) == ENABLE );			//等待DMA可配置

	dma.DMA_Channel				=   DMA_Channel_4;
	dma.DMA_PeripheralBaseAddr	=	(uint32_t)&(UART5->DR);
	dma.DMA_Memory0BaseAddr		=	(uint32_t)UART5_Tx_DMAbuff;//暂无
	dma.DMA_DIR					=	DMA_DIR_MemoryToPeripheral;	//内存到外设
	dma.DMA_BufferSize			=	UART5_Rx_DMAbuf_LEN;//暂无
	dma.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc			=	DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize	=	DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize		=	DMA_MemoryDataSize_Byte;
	dma.DMA_Mode				=	DMA_Mode_Normal;			//正常发送
	dma.DMA_Priority			=	DMA_Priority_VeryHigh;
	dma.DMA_FIFOMode			=	DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold		=	DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_MemoryBurst			=	DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst		=	DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream7, &dma);
	DMA_Cmd(DMA1_Stream7, DISABLE);	//失能DMA

	//UART5_Rx
	DMA_DeInit(DMA1_Stream0);
	while( DMA_GetCmdStatus(DMA1_Stream0) == ENABLE );			//等待DMA可配置

	dma.DMA_Channel            =    DMA_Channel_4;				//即DMA_Channel_5
	dma.DMA_PeripheralBaseAddr = 	(uint32_t)&(UART5->DR);
	dma.DMA_Memory0BaseAddr    = 	(uint32_t)UART5_Rx_DMAbuff;
	dma.DMA_DIR                = 	DMA_DIR_PeripheralToMemory;	//外设到内存
	dma.DMA_BufferSize         = 	UART5_Rx_DMAbuf_LEN;
	dma.DMA_Mode               = 	DMA_Mode_Circular;			//循环接收
	dma.DMA_Priority           = 	DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Stream0, &dma);
	DMA_Cmd(DMA1_Stream0, ENABLE);	//使能DMA
}



