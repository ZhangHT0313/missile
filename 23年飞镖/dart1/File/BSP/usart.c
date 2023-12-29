#include "usart.h"


/*--------------------------------------------------------------------------
�������ܣ�����������1ͨ��
--------------------------------------------------------------------------*/

void USART2_Configuration(void)
	{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��13579o
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//ʹ��USART1ʱ��13579o
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIO����ΪUSART113579o
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIO����ΪUSART113579o
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //13579o
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��GPIO13579o

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʹ���շ�ģʽ
	USART_Init(USART2,&USART_InitStructure); //��ʼ������113579o
	
//	USART_Cmd(USART1,ENABLE);  //ʹ�ܴ���13579o

  	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���?
	
	//���ý���DMA
	DMA_DeInit(USART2_RX_STREAM);
    DMA_InitStructure.DMA_Channel                 =    DMA_Channel_4;                 //ͨ��ѡ��
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
	
	
	//���÷���DMA
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
�������ܣ�����������2ͨ��
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
	usart.USART_BaudRate			=	9600;	//�˴�ʹ�ø߲����ʣ�1ms�ڷ����ֽ���������40�ֽ���
	usart.USART_WordLength			=	USART_WordLength_8b;
	usart.USART_StopBits			=	USART_StopBits_1;
	usart.USART_Parity				=	USART_Parity_No;
	usart.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
	usart.USART_Mode				=	USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &usart);

	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);				//����3���տ����ж�
	USART_Cmd(USART3, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

	nvic.NVIC_IRQChannel					=	USART3_IRQn; 	//����3���տ����ж�
    nvic.NVIC_IRQChannelPreemptionPriority	=	0;
    nvic.NVIC_IRQChannelSubPriority			=	10;
    nvic.NVIC_IRQChannelCmd					=	ENABLE;
    NVIC_Init(&nvic);

	//USART3_Tx
	DMA_DeInit(DMA1_Stream3);
	while( DMA_GetCmdStatus(DMA1_Stream3) == ENABLE );			//�ȴ�DMA������

	dma.DMA_Channel				=	DMA_Channel_4;
	dma.DMA_PeripheralBaseAddr	=	(uint32_t)&(USART3->DR);
	dma.DMA_Memory0BaseAddr		=	(u32)&USART3_Tx_DMAbuff;
	dma.DMA_DIR					=	DMA_DIR_MemoryToPeripheral;	//�ڴ浽����
	dma.DMA_BufferSize			=	USART3_Tx_DMAbuf_LEN;
	dma.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc			=	DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize	=	DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize		=	DMA_MemoryDataSize_Byte;
	dma.DMA_Mode				=	DMA_Mode_Normal;			//��������
	dma.DMA_Priority			=	DMA_Priority_VeryHigh;
	dma.DMA_FIFOMode			=	DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold		=	DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_MemoryBurst			=	DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst		=	DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream3, &dma);
	DMA_Cmd(DMA1_Stream3, DISABLE);

	//USART3_Rx
	DMA_DeInit(DMA1_Stream1);
	while( DMA_GetCmdStatus(DMA1_Stream1) == ENABLE );			//�ȴ�DMA������

	dma.DMA_Channel            = 	DMA_Channel_4;				//��DMA_Channel_4
	dma.DMA_PeripheralBaseAddr = 	(uint32_t)&(USART3->DR);
	dma.DMA_Memory0BaseAddr    = 	(uint32_t)USART3_Rx_DMAbuff;
	dma.DMA_DIR                = 	DMA_DIR_PeripheralToMemory;	//���赽�ڴ�
	dma.DMA_BufferSize         = 	USART3_Rx_DMAbuf_LEN;
	dma.DMA_Mode               = 	DMA_Mode_Circular;			//ѭ������
	dma.DMA_Priority           = 	DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Stream1, &dma);
	DMA_Cmd(DMA1_Stream1, ENABLE);
}


/*--------------------------------------------------------------------------
�������ܣ�����֮��ͨѶ�ײ����ã�
--------------------------------------------------------------------------*/
void UART4_Configuration(void)
{
	GPIO_InitTypeDef	gpio;
	USART_InitTypeDef	usart;
	NVIC_InitTypeDef 	nvic;
	DMA_InitTypeDef		dma;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);						//ʹ��UART4ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA1,ENABLE);	//ʹ��PC�˿�ʱ��

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);//Tx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);//Rx


    gpio.GPIO_OType = GPIO_OType_PP;//PP
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
	gpio.GPIO_Mode  = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOC, &gpio);

	nvic.NVIC_IRQChannel 					= UART4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority	= 0;		//��ռ���ȼ�
	nvic.NVIC_IRQChannelSubPriority			= 6;		//�����ȼ�
	nvic.NVIC_IRQChannelCmd					= ENABLE;	//IRQͨ��ʹ�� 
	NVIC_Init(&nvic);//����ָ���Ĳ�����ʼ��NVIC�Ĵ���

	usart.USART_BaudRate			= 460800;				//������
	usart.USART_WordLength			= USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	usart.USART_StopBits			= USART_StopBits_1;		//һ��ֹͣλ
	usart.USART_Parity				= USART_Parity_No;		//����żУ��λ
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	usart.USART_Mode				= USART_Mode_Tx|USART_Mode_Rx;		//�շ�ģʽ
    USART_Init(UART4, &usart);//��ʼ������

    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);	//���������ж�
    USART_Cmd(UART4, ENABLE);	//ʹ�ܴ���
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);

    //UART4_Tx
    DMA_DeInit(UART4_TX_STREAM);
    while( DMA_GetCmdStatus(UART4_TX_STREAM) == ENABLE );			//�ȴ�DMA������
    
    dma.DMA_Channel            =    DMA_Channel_4;                 //�����ַ
    dma.DMA_PeripheralBaseAddr =    (uint32_t)&(UART4->DR);
    dma.DMA_Memory0BaseAddr    =    (uint32_t)UART4_Tx_DMAbuff;
    dma.DMA_DIR                =    DMA_DIR_MemoryToPeripheral;    //DMA����Ϊ����
    dma.DMA_BufferSize         =    UART4_Tx_DMAbuf_LEN;               //����DMA�ڴ������ĳ���
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
	while( DMA_GetCmdStatus(UART4_RX_STREAM) == ENABLE );			//�ȴ�DMA������

	dma.DMA_Channel            =    DMA_Channel_4;				//��DMA_Channel_4
	dma.DMA_PeripheralBaseAddr = 	(uint32_t)&(UART4->DR);
	dma.DMA_Memory0BaseAddr    = 	(uint32_t)UART4_Rx_DMAbuff;
	dma.DMA_DIR                = 	DMA_DIR_PeripheralToMemory;	//���赽�ڴ�
	dma.DMA_BufferSize         = 	UART4_Rx_DMAbuf_LEN;
	dma.DMA_Mode               = 	DMA_Mode_Circular;			//ѭ������
	dma.DMA_Priority           = 	DMA_Priority_VeryHigh;
	DMA_Init(UART4_RX_STREAM, &dma);
	DMA_Cmd(UART4_RX_STREAM, ENABLE);	//ʹ��DMA
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
	usart.USART_BaudRate			= 115200;//����ϵͳ
	usart.USART_WordLength			= USART_WordLength_8b;
	usart.USART_StopBits			= USART_StopBits_1;
	usart.USART_Parity				= USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode				= USART_Mode_Rx | USART_Mode_Tx;//�����ж�
	USART_Init(UART5, &usart);
    
  USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);   //���������ж�
	USART_Cmd(UART5, ENABLE);                       //ʹ�ܴ���
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);   //ʹ�ܴ��ڷ���DMA
	USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);   //ʹ�ܴ��ڽ���DMA
    
  nvic.NVIC_IRQChannel					= UART5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority  = 0;
	nvic.NVIC_IRQChannelSubPriority			= 6;
	nvic.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&nvic);

    //UART5_Tx
  DMA_DeInit(DMA1_Stream7);
	while( DMA_GetCmdStatus(DMA1_Stream7) == ENABLE );			//�ȴ�DMA������

	dma.DMA_Channel				=   DMA_Channel_4;
	dma.DMA_PeripheralBaseAddr	=	(uint32_t)&(UART5->DR);
	dma.DMA_Memory0BaseAddr		=	(uint32_t)UART5_Tx_DMAbuff;//����
	dma.DMA_DIR					=	DMA_DIR_MemoryToPeripheral;	//�ڴ浽����
	dma.DMA_BufferSize			=	UART5_Rx_DMAbuf_LEN;//����
	dma.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc			=	DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize	=	DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize		=	DMA_MemoryDataSize_Byte;
	dma.DMA_Mode				=	DMA_Mode_Normal;			//��������
	dma.DMA_Priority			=	DMA_Priority_VeryHigh;
	dma.DMA_FIFOMode			=	DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold		=	DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_MemoryBurst			=	DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst		=	DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream7, &dma);
	DMA_Cmd(DMA1_Stream7, DISABLE);	//ʧ��DMA

	//UART5_Rx
	DMA_DeInit(DMA1_Stream0);
	while( DMA_GetCmdStatus(DMA1_Stream0) == ENABLE );			//�ȴ�DMA������

	dma.DMA_Channel            =    DMA_Channel_4;				//��DMA_Channel_5
	dma.DMA_PeripheralBaseAddr = 	(uint32_t)&(UART5->DR);
	dma.DMA_Memory0BaseAddr    = 	(uint32_t)UART5_Rx_DMAbuff;
	dma.DMA_DIR                = 	DMA_DIR_PeripheralToMemory;	//���赽�ڴ�
	dma.DMA_BufferSize         = 	UART5_Rx_DMAbuf_LEN;
	dma.DMA_Mode               = 	DMA_Mode_Circular;			//ѭ������
	dma.DMA_Priority           = 	DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Stream0, &dma);
	DMA_Cmd(DMA1_Stream0, ENABLE);	//ʹ��DMA
}



