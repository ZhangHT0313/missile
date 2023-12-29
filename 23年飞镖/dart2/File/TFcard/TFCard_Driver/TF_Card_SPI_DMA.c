/*--------------------------------------------------------------------------
TF����ʼ�����ã�SPI+DMA
--------------------------------------------------------------------------*/

#include "TF_Card_SPI_DMA.h"


#define RESERVED_MASK (uint32_t)0x0F7D0F7D //ԭ����λ��stm32f4xx_dma.c


FATFS fs1;
FIL file1;


u8 buffer[2000]; //��ȡ���壬ע���С����ҪԽ��

char str[20]; 

int i;
UINT br;
FRESULT res;

void SD_Test(void)
{
	FRESULT fr;
	SD_LowLevel_Init(); //��ʼ��SD����SPI DMA
	SD_Init();          //��ʼ��SDͨѶ
  
//	printf("SD Card mount start\r\n");
  
	fr = f_mount(&fs1, "", 1);//ʹ��FATFS��ʱ�򣬱�����ͨ��f_mount������ע��һ�������������ܿ�ʼ����API��ʹ��
	
	if(fr)
	{
//		printf("SD Card mount failed\r\n");
	}
	else
	{
//		printf("SD Card mount succeed\r\n");
		SD_GetCardInfo(&SD_CardInfo);
		ShowCardInfo(&SD_CardInfo);

		res = f_open( &file1, "0:/Test.txt", FA_OPEN_ALWAYS|FA_READ|FA_WRITE );

		if(res != FR_OK)
		{
//			printf("f_open() fail \r\n");
		}
		else
		{
//			printf("f_open() success \r\n");
		}
			
		br = file1.fsize;
//		printf("Intial file size:%d\r\n",br);    
		res = f_read(&file1, buffer, file1.fsize, &br);
		
		if(res == FR_OK)
		{
//			printf("Read text:%s\r\n", buffer);
		}
		else
		{
//			printf(" f_read() fail \r\n");
		}

		if(buffer[10]<0x30)
			buffer[10]=0x30;
		if(buffer[10]>0x39)
			buffer[10]=0x39;

		i=atoi((const char*)&buffer[10]);
        sprintf(str,"%d",i+1);

		res = f_lseek(&file1,0);
		br = f_puts("TFCARDTEST", &file1);

		if(br < 1)
		{
//			printf("f_puts() fail \r\n");
		}
		else
		{
//			printf("f_puts() success \r\n");
		}

		res = f_lseek(&file1,10);
		br = f_puts((const char*)str, &file1);

		if(br < 1) 
		{
//			printf("f_puts() fail \r\n");
		}
		else
		{
//			printf("f_puts() success \r\n");
		}

		f_close(&file1);
	}
}

SD_CARDINFO SD_CardInfo;

//��� SD ����Ϣ
void ShowCardInfo(SD_CARDINFO *CardInfo)
{
	SD_CSD *csd;
	SD_CID *cid;

	csd = &(CardInfo->CSD);
	cid = &(CardInfo->CID);
	
//	printf("--------SD Card Info--------\r\n");
//	printf("SD_Type ");
	switch(CardInfo->Type)
	{
		case SD_TYPE_MMC3:
//			printf("MMC3\r\n");
			break;
		case SD_TYPE_SDV1:
//			printf("SDV1\r\n");
			break;
		case SD_TYPE_SDV2:
//			printf("SDV2\r\n");
			break;
		case SD_TYPE_SDHC:
//			printf("SDHC\r\n");
			break;
		default:
//			printf("Unknow\r\n");
			break;
	}
//	printf("CardCapacity %llu\r\n", CardInfo->Capacity);
//	printf("CardBlockSize %d\r\n", CardInfo->BlockSize);
//	
//	printf("CSDStruct %d\r\n", csd->CSDStruct);
//	printf("SysSpecVersion %d\r\n", csd->SysSpecVersion);
//	printf("TAAC %d\r\n", csd->TAAC);
//	printf("NSAC %d\r\n", csd->NSAC);
//	printf("MaxBusClkFrec %d\r\n", csd->MaxBusClkFrec);
//	printf("CardComdClasses %d\r\n", csd->CardComdClasses);
//	printf("RdBlockLen %d\r\n", csd->RdBlockLen);
//	printf("PartBlockRead %d\r\n", csd->PartBlockRead);
//	printf("WrBlockMisalign %d\r\n", csd->WrBlockMisalign);
//	printf("RdBlockMisalign %d\r\n", csd->RdBlockMisalign);
//	printf("DSRImpl %d\r\n", csd->DSRImpl);
//	printf("DeviceSize %d\r\n", csd->DeviceSize);
//	printf("MaxRdCurrentVDDMin %d\r\n", csd->MaxRdCurrentVDDMin);
//	printf("MaxRdCurrentVDDMax %d\r\n", csd->MaxRdCurrentVDDMax);
//	printf("MaxWrCurrentVDDMin %d\r\n", csd->MaxWrCurrentVDDMin);
//	printf("MaxWrCurrentVDDMax %d\r\n", csd->MaxWrCurrentVDDMax);
//	printf("DeviceSizeMul %d\r\n", csd->DeviceSizeMul);
//	printf("EraseGrSize %d\r\n", csd->EraseGrSize);
//	printf("EraseGrMul %d\r\n", csd->EraseGrMul);
//	printf("WrProtectGrSize %d\r\n", csd->WrProtectGrSize);
//	printf("WrProtectGrEnable %d\r\n", csd->WrProtectGrEnable);
//	printf("ManDeflECC %d\r\n", csd->ManDeflECC);
//	printf("WrSpeedFact %d\r\n", csd->WrSpeedFact);
//	printf("MaxWrBlockLen %d\r\n", csd->MaxWrBlockLen);
//	printf("WriteBlockPaPartial %d\r\n", csd->WriteBlockPaPartial);
//	printf("ContentProtectAppli %d\r\n", csd->ContentProtectAppli);
//	printf("FileFormatGroup %d\r\n", csd->FileFormatGroup);
//	printf("CopyFlag %d\r\n", csd->CopyFlag);
//	printf("PermWrProtect %d\r\n", csd->PermWrProtect);
//	printf("TempWrProtect %d\r\n", csd->TempWrProtect);
//	printf("FileFormat %d\r\n", csd->FileFormat);
//	printf("ECC %d\r\n", csd->ECC);
//	printf("CSD_CRC %d\r\n", csd->CSD_CRC);

//	printf("ManufacturerID %d\r\n", cid->ManufacturerID);
//	printf("OEM_AppliID %d\r\n", cid->OEM_AppliID);
//	printf("ProdName1 %d\r\n", cid->ProdName1);
//	printf("ProdName2 %d\r\n", cid->ProdName2);
//	printf("ProdRev %d\r\n", cid->ProdRev);
//	printf("ProdSN %d\r\n", cid->ProdSN);
//	printf("ManufactDate %d\r\n", cid->ManufactDate);
//	printf("CID_CRC %d\r\n", cid->CID_CRC);

//	printf("--------SD Card Info--------\r\n");
}

static uint8_t Dummy_Send = SD_DUMMY_BYTE;
static uint8_t Dummy_Recv = SD_DUMMY_BYTE;

/**
  * @brief  Initializes the SPI and GPIOs resources used to drive the microSD card
  * @param  None
  * @retval None
  */
void SD_LowLevel_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// SD GPIO Periph clock enable
	RCC_AHB1PeriphClockCmd(SD_CS_RCC|SD_MOSI_RCC|SD_MISO_RCC|SD_SCK_RCC, ENABLE);
	// SD_SPI Periph clock enable
	RCC_APB1PeriphClockCmd(SD_SPI_RCC, ENABLE);

	GPIO_PinAFConfig(SD_SCK_GPIO, SD_SCK_SOURCE, SD_SCK_AF);
	GPIO_PinAFConfig(SD_MISO_GPIO, SD_MISO_SOURCE, SD_MISO_AF);
	GPIO_PinAFConfig(SD_MOSI_GPIO, SD_MOSI_SOURCE, SD_MOSI_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// Configure SD_SPI pins: SCK
	GPIO_InitStructure.GPIO_Pin = SD_SCK_PIN;
	GPIO_Init(SD_SCK_GPIO, &GPIO_InitStructure);

	// Configure SD_SPI pins: MOSI/DI
	GPIO_InitStructure.GPIO_Pin = SD_MOSI_PIN;
	GPIO_Init(SD_MOSI_GPIO, &GPIO_InitStructure);

	// Configure SD_SPI pins: MISO/DO
	GPIO_InitStructure.GPIO_Pin = SD_MISO_PIN;
	GPIO_Init(SD_MISO_GPIO, &GPIO_InitStructure);

	// Configure SD_SPI pins: CS
	GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(SD_CS_GPIO, &GPIO_InitStructure);

	SD_SPI_Config();
	SD_DMA_Config();

	// SD_SPI enable
	SPI_Cmd(SD_SPI, ENABLE);
	SD_CardInfo.Status |= STA_NOINIT;
}
/**
  * @brief  DeInitializes the SPI interface.
  * @param  None
  * @retval None
  */
void SD_LowLevel_DeInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	SD_CardInfo.Status |= STA_NOINIT;
	SPI_Cmd(SD_SPI, DISABLE); // SD_SPI disable
	SPI_I2S_DeInit(SD_SPI); // DeInitializes the SD_SPI
	RCC_APB1PeriphClockCmd(SD_SPI_RCC, DISABLE); // SD_SPI Periph clock disable

	// Configure SD_SPI pins: SCK
	GPIO_InitStructure.GPIO_Pin = SD_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SD_SCK_GPIO, &GPIO_InitStructure);

	// Configure SD_SPI pins: MISO
	GPIO_InitStructure.GPIO_Pin = SD_MISO_PIN;
	GPIO_Init(SD_MISO_GPIO, &GPIO_InitStructure);

	// Configure SD_SPI pins: MOSI
	GPIO_InitStructure.GPIO_Pin = SD_MOSI_PIN;
	GPIO_Init(SD_MOSI_GPIO, &GPIO_InitStructure);

	// Configure SD_SPI pins: CS
	GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
	GPIO_Init(SD_CS_GPIO, &GPIO_InitStructure);
}
/**
  * @brief  Configure SPI of the SD card.
  * @param  None
  * @retval None
  */
void SD_SPI_Config(void)
{
	SPI_InitTypeDef   SPI_InitStructure;

	// SD_SPI Config
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SD_SPI, &SPI_InitStructure);
}
//SPI������DMA����
void SD_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(SD_DMA_RCC, ENABLE);

	DMA_DeInit(SD_DMA_RX_STREAM);
	DMA_DeInit(SD_DMA_TX_STREAM);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SD_SPI->DR));
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //!!!
	DMA_InitStructure.DMA_BufferSize = 0; //!!!
	
	DMA_InitStructure.DMA_Channel = SD_DMA_RX_CHANNEL;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0; //!!!
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_Init(SD_DMA_RX_STREAM, &DMA_InitStructure);

	DMA_InitStructure.DMA_Channel = SD_DMA_TX_CHANNEL;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0; //!!!
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_Init(SD_DMA_TX_STREAM, &DMA_InitStructure);

	SD_SPI->CR2 |= SPI_I2S_DMAReq_Rx;
	SD_SPI->CR2 |= SPI_I2S_DMAReq_Tx;
	//SPI_I2S_DMACmd(SD_SPI, SPI_I2S_DMAReq_Rx, ENABLE);
	//SPI_I2S_DMACmd(SD_SPI, SPI_I2S_DMAReq_Tx, ENABLE);
}
// DMA �������ȴ��������־��ֹͣ
void SD_DMAProcess(void)
{
	// ����
	SD_DMA_RX_STREAM->CR |= DMA_SxCR_EN; //����������
	//DMA_Cmd(SD_DMA_RX_STREAM, ENABLE);
	while(!(SD_DMA_RX_STREAM->CR & DMA_SxCR_EN));
	//while(DMA_GetCmdStatus(SD_DMA_RX_STREAM) == DISABLE);
	SD_DMA_TX_STREAM->CR |= DMA_SxCR_EN; //����������
	//DMA_Cmd(SD_DMA_TX_STREAM, ENABLE);
	while(!(SD_DMA_TX_STREAM->CR & DMA_SxCR_EN));
	//while(DMA_GetCmdStatus(SD_DMA_TX_STREAM) == DISABLE);
	
	// �ȴ����
	while(!((SD_DMA->HISR & RESERVED_MASK) & SD_DMA_TX_TCIF));
	//while(!DMA_GetFlagStatus(SD_DMA_RX_STREAM, SD_DMA_TX_TCIF));
	while(!((SD_DMA->LISR & RESERVED_MASK) & SD_DMA_RX_TCIF));
	//while(!DMA_GetFlagStatus(SD_DMA_RX_STREAM, SD_DMA_RX_TCIF));

	// �����ɱ�־
	SD_DMA->LIFCR = SD_DMA_RX_TCIF & RESERVED_MASK; //���������ɱ�־
	//DMA_ClearFlag(SD_DMA_RX_STREAM, SD_DMA_RX_TCIF);
	SD_DMA->HIFCR = SD_DMA_TX_TCIF & RESERVED_MASK; //���������ɱ�־
	//DMA_ClearFlag(SD_DMA_TX_STREAM, SD_DMA_TX_TCIF);
	
	// ֹͣ
	SD_DMA_TX_STREAM->CR &= ~DMA_SxCR_EN;
	//DMA_Cmd(SD_DMA_TX_STREAM, DISABLE);
	while(SD_DMA_TX_STREAM->CR & DMA_SxCR_EN);
	//while(DMA_GetCmdStatus(SD_DMA_TX_STREAM) == ENABLE);
	SD_DMA_RX_STREAM->CR &= ~DMA_SxCR_EN;
	//DMA_Cmd(SD_DMA_RX_STREAM, DISABLE);
	while(SD_DMA_RX_STREAM->CR & DMA_SxCR_EN);
	//while(DMA_GetCmdStatus(SD_DMA_TX_STREAM) == ENABLE);
}
//����SPI�ٶ� SPI_BaudRatePrescaler_2 ~ SPI_BaudRatePrescaler_256 => (0~7)*8
void SD_SpeedSet(uint16_t speed)
{
	SD_SPI->CR1 = ((uint16_t)(SD_SPI->CR1 & 0xFFC7)) | (speed & 0x0038);
	SD_SPI->CR1 |= SPI_CR1_SPE;
	//SPI_Cmd(SD_SPI, ENABLE);
}

/**
* @brief  DeInitializes the SD/SD communication.
* @param  None
* @retval None
*/
void SD_DeInit(void)
{
	SD_LowLevel_DeInit();
}

/**
* @brief  Initializes the SD/SD communication.
* @param  None
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_Init(void)
{
	uint32_t i;
	SD_Error res;

/*	if (SD_CardInfo.Status & STA_NODISK) return SD_RESPONSE_FAILURE;
	//�Ӵ��������ܵ��� SD �����ʧЧ, �ʴ˴������, ���ڵ��ô˺���ǰ���ɾ����Ƿ���
	if (SD_Detect() == SD_NOT_PRESENT) return SD_RESPONSE_FAILURE;
	SD_LowLevel_Init();
*/
	//�Ѿ���ʼ�������ظ���ʼ������
	if ((SD_CardInfo.Status & STA_NOINIT) == 0x00) return SD_RESPONSE_NO_ERROR;
	
	//��ʼ��ʼ��
	SD_CardInfo.Type = SD_TYPE_UNKNOW;
	SD_SpeedSet(SPI_BaudRatePrescaler_256); //Low speed

	SD_CS_HIGH(); // SD chip select high
	// Rise CS and MOSI for 80(>74) clocks cycles
	for (i = 0; i < 10; i++)
	{
		SD_ReadByte();
	}

	// Send CMD0 to put SD in SPI mode
	res = SD_CmdRsp(SD_CMD_GO_IDLE_STATE, 0, 0x95);
	if (res != SD_IN_IDLE_STATE) return SD_RESPONSE_FAILURE;

	// CMD0 switching period is within 8 clocks after the end bit
	// of the CMD0 command R1 response
	SD_ReadByte(); //��߿ɿ���
	SD_ReadByte();
	// SDv2 ���Ժ�汾֧�� CMD8, IF=Interface
	res = SD_CmdRsp(SD_CMD_IF_COND, 0x01AA, 0x87);
	if (res == SD_IN_IDLE_STATE)
	{	// SDv2 Card or later
		uint8_t ocr[4]; //OCR (Operation Conditions Register)
		ocr[0] = SD_ReadByte();
		ocr[1] = SD_ReadByte();
		ocr[2] = SD_ReadByte();
		ocr[3] = SD_ReadByte();
		if (ocr[2] == 0x01 && ocr[3] == 0xAA) //��ѹ��Χ2.7V~3.6V
		{
			i = INIT_RETRY_MAX; // wait up to 2s
			do // ��ʼ��
			{
				res = SD_CmdRsp(SD_CMD_APP_CMD, 0, 0xFF); // CMD55
				if (res == SD_IN_IDLE_STATE)
				{
					res = SD_CmdRsp(41, 0x40000000, 0xFF); // ACMD41 ע�����
				}
			}
			while ((res != SD_RESPONSE_NO_ERROR) && --i);
			if(i)
			{
				// CMD58 ��ȡ OCR
				res = SD_CmdRsp(SD_CMD_READ_OCR, 0, 0xFF);
				if (res == SD_RESPONSE_NO_ERROR)
				{
					ocr[0] = SD_ReadByte();
					ocr[1] = SD_ReadByte();
					ocr[2] = SD_ReadByte();
					ocr[3] = SD_ReadByte();
					if (ocr[0] & 0x40) // CCS bit (Card Capacity Status)
					{
						SD_CardInfo.Type = SD_TYPE_SDHC; // or SDXC
					}
					else
					{
						SD_CardInfo.Type = SD_TYPE_SDV2;
					}
				}
			}
		}
	}
	else // CMD8 ������Ӧ
	{
		i = INIT_RETRY_MAX;
		do // ��ʼ��
		{
			res = SD_CmdRsp(SD_CMD_APP_CMD, 0, 0xFF); // CMD55
			if (res == SD_IN_IDLE_STATE)
			{
				res = SD_CmdRsp(41, 0x00, 0xFF); // ACMD41 ������ SDv2 ��ͬ
			}
		}
		while ((res != SD_RESPONSE_NO_ERROR) && --i);
		if (i) // �л�Ӧ�� SDv1, �ҳ�ʼ�����
		{
			SD_CardInfo.Type = SD_TYPE_SDV1;
		}
		else // û�л�Ӧ�� MMC
		{
			i = INIT_RETRY_MAX;
			do // ��ʼ��
			{
				res = SD_CmdRsp(SD_CMD_SEND_OP_COND, 0x00, 0xFF); //CMD1 ������ SDv2 ��ͬ
			}
			while ((res != SD_RESPONSE_NO_ERROR) && --i);
			if (i) SD_CardInfo.Type = SD_TYPE_MMC3;
		}
	}
	if (SD_CardInfo.Type != SD_TYPE_UNKNOW)
	{
		SD_CardInfo.Status &= ~STA_NOINIT; //��ʼ���ɹ�
		// CMD59 �ر� CRC У��
		SD_CmdRsp(SD_CMD_CRC_ON_OFF, 0x00, 0xFF);

		// CMD16 ���� BLOCK Ϊ 512 Bytes (�� SDHC/SDHX ��Ч)
		SD_CmdRsp(SD_CMD_SET_BLOCKLEN, SD_BLOCK_SIZE, 0xFF);
		
		res = SD_RESPONSE_NO_ERROR;
	}
	else
	{
		res = SD_RESPONSE_FAILURE;
	}
	SD_DeAssert();
	//����SD����Ӳ��������ȷ�� SPI_BaudRatePrescaler_4
	SD_SpeedSet(SPI_BaudRatePrescaler_2);

	return res;
}

/**
* @brief  Returns information about specific card.
* @param  CardInfo: pointer to a SD_CARDINFO structure that contains all SD
*         card information.
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_GetCardInfo(SD_CARDINFO *CardInfo)
{
	SD_Error res;
	SD_Type sd_type = CardInfo->Type;

	res = SD_GetCSDRegister(&(CardInfo->CSD));
	res = SD_GetCIDRegister(&(CardInfo->CID));

	if (sd_type == SD_TYPE_SDHC)
	{
		CardInfo->Capacity = ((uint64_t)CardInfo->CSD.DeviceSize + 1) * 512 * 1024;
        CardInfo->BlockSize = 512;
	}
	else if (sd_type == SD_TYPE_SDV2 || sd_type == SD_TYPE_SDV1)
	{
		CardInfo->Capacity = (CardInfo->CSD.DeviceSize + 1);
		CardInfo->Capacity *= (1 << (CardInfo->CSD.DeviceSizeMul + 2));
		CardInfo->BlockSize = 1 << (CardInfo->CSD.RdBlockLen);
		CardInfo->Capacity *= CardInfo->BlockSize;
	}

	return res;
}

/**
* @brief  Read the CSD card register.
*         Reading the contents of the CSD register in SPI mode is a simple
*         read-block transaction.
* @param  sd_csd: pointer on an SCD register structure
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_GetCSDRegister(SD_CSD* sd_csd)
{
	uint32_t i = 0;
	SD_Error res;
	uint8_t CSD_Tab[16]; // Card Specific Data
	SD_Type sd_type = SD_CardInfo.Type;

	// Send CMD9 (CSD register)
	res = SD_CmdRsp(SD_CMD_SEND_CSD, 0, 0xFF);
	if (!res)
	{	// Wait for response in the R1 format (0x00 is no errors)
		res = SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ);
		if (!res)
		{
			for (i = 0; i < 16; i++) // Store CSD register value on CSD_Tab
			{
				CSD_Tab[i] = SD_ReadByte();
			}
			// Get CRC bytes (not really needed by us, but required by SD)
			SD_ReadByte();
			SD_ReadByte();
		}
		SD_DeAssert();
	}
	
	/*!< Byte 0 */
	sd_csd->CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
	sd_csd->SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
	sd_csd->Reserved1 = CSD_Tab[0] & 0x03;

	/*!< Byte 1 */
	sd_csd->TAAC = CSD_Tab[1];

	/*!< Byte 2 */
	sd_csd->NSAC = CSD_Tab[2];

	/*!< Byte 3 */
	sd_csd->MaxBusClkFrec = CSD_Tab[3];

	/*!< Byte 4 */
	sd_csd->CardComdClasses = CSD_Tab[4] << 4;

	/*!< Byte 5 */
	sd_csd->CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
	sd_csd->RdBlockLen = CSD_Tab[5] & 0x0F;

	/*!< Byte 6 */
	sd_csd->PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
	sd_csd->WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
	sd_csd->RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
	sd_csd->DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
	sd_csd->Reserved2 = 0; /*!< Reserved */

	if (sd_type == SD_TYPE_SDHC) // or SDXC CSD v2.0
	{
		/*!< Byte 7 */
        sd_csd->DeviceSize = (CSD_Tab[7] & 0x3F) << 16;

        /*!< Byte 8 */
        sd_csd->DeviceSize |= (CSD_Tab[8] << 8);

        /*!< Byte 9 */
        sd_csd->DeviceSize |= (CSD_Tab[9]);
	}
	else if (sd_type == SD_TYPE_SDV1 || sd_type == SD_TYPE_SDV2) //CSD v1.0
	{
		sd_csd->DeviceSize = (CSD_Tab[6] & 0x03) << 10;
		/*!< Byte 7 */
		sd_csd->DeviceSize |= (CSD_Tab[7]) << 2;

		/*!< Byte 8 */
		sd_csd->DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;

		sd_csd->MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
		sd_csd->MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);

		/*!< Byte 9 */
		sd_csd->MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
		sd_csd->MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
		sd_csd->DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;

		/*!< Byte 10 */
		sd_csd->DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
	}

	sd_csd->EraseGrSize = (CSD_Tab[10] & 0x40) >> 6;
	sd_csd->EraseGrMul = (CSD_Tab[10] & 0x3F) << 1;

	/*!< Byte 11 */
	sd_csd->EraseGrMul |= (CSD_Tab[11] & 0x80) >> 7;
	sd_csd->WrProtectGrSize = (CSD_Tab[11] & 0x7F);

	/*!< Byte 12 */
	sd_csd->WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
	sd_csd->ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
	sd_csd->WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
	sd_csd->MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;

	/*!< Byte 13 */
	sd_csd->MaxWrBlockLen |= (CSD_Tab[13] & 0xC0) >> 6;
	sd_csd->WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
	sd_csd->Reserved3 = 0;
	sd_csd->ContentProtectAppli = (CSD_Tab[13] & 0x01);

	/*!< Byte 14 */
	sd_csd->FileFormatGroup = (CSD_Tab[14] & 0x80) >> 7;
	sd_csd->CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
	sd_csd->PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
	sd_csd->TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
	sd_csd->FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
	sd_csd->ECC = (CSD_Tab[14] & 0x03);

	/*!< Byte 15 */
	sd_csd->CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
	sd_csd->Reserved4 = 1;

	/*!< Return the reponse */
	return res;
}

/**
* @brief  Read the CID card register.
*         Reading the contents of the CID register in SPI mode is a simple
*         read-block transaction.
* @param  sd_cid: pointer on an CID register structure
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_GetCIDRegister(SD_CID* sd_cid)
{
	uint32_t i;
	SD_Error res;
	uint8_t CID_Tab[16];

	// Send CMD10 (CID register)
	res = SD_CmdRsp(SD_CMD_SEND_CID, 0, 0xFF);
	if (!res)
	{
		res = SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ);
		if (!res)
		{
			for (i = 0; i < 16; i++) // Store CID register value on CID_Tab
			{
				CID_Tab[i] = SD_ReadByte();
			}
			// Get CRC bytes (not really needed by us, but required by SD)
			SD_ReadByte();
			SD_ReadByte();
		}
		SD_DeAssert();
	}
	
	/*!< Byte 0 */
	sd_cid->ManufacturerID = CID_Tab[0];

	/*!< Byte 1 */
	sd_cid->OEM_AppliID = CID_Tab[1] << 8;

	/*!< Byte 2 */
	sd_cid->OEM_AppliID |= CID_Tab[2];

	/*!< Byte 3 */
	sd_cid->ProdName1 = CID_Tab[3] << 24;

	/*!< Byte 4 */
	sd_cid->ProdName1 |= CID_Tab[4] << 16;

	/*!< Byte 5 */
	sd_cid->ProdName1 |= CID_Tab[5] << 8;

	/*!< Byte 6 */
	sd_cid->ProdName1 |= CID_Tab[6];

	/*!< Byte 7 */
	sd_cid->ProdName2 = CID_Tab[7];

	/*!< Byte 8 */
	sd_cid->ProdRev = CID_Tab[8];

	/*!< Byte 9 */
	sd_cid->ProdSN = CID_Tab[9] << 24;

	/*!< Byte 10 */
	sd_cid->ProdSN |= CID_Tab[10] << 16;

	/*!< Byte 11 */
	sd_cid->ProdSN |= CID_Tab[11] << 8;

	/*!< Byte 12 */
	sd_cid->ProdSN |= CID_Tab[12];

	/*!< Byte 13 */
	sd_cid->Reserved1 |= (CID_Tab[13] & 0xF0) >> 4;
	sd_cid->ManufactDate = (CID_Tab[13] & 0x0F) << 8;

	/*!< Byte 14 */
	sd_cid->ManufactDate |= CID_Tab[14];

	/*!< Byte 15 */
	sd_cid->CID_CRC = (CID_Tab[15] & 0xFE) >> 1;
	sd_cid->Reserved2 = 1;

	/*!< Return the reponse */
	return res;
}

/**
* @brief  Reads a block of data from the SD.
* @param  pBuffer: pointer to the buffer that receives the data read from the
*                  SD.
* @param  ReadAddr: SD's internal address to read from.
* @param  BlockSize: the SD card Data block size.
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_ReadBlock(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t BlockSize)
{
	uint32_t i;
	SD_Error res;

	// Send CMD17 to read single block
	res = SD_CmdRsp(SD_CMD_READ_SINGLE_BLOCK, ReadAddr, 0xFF);
	if (!res)
	{
		// Now look for the data token(0xFE) to signify the start of the data
		i = READ_BLOCK_MAX; //200ms
		while ((SD_ReadByte() != SD_START_DATA_SINGLE_BLOCK_READ) && --i);
		if (i)
		{
			// ���ý���ͨ��
			SD_DMA_RX_STREAM->CR |= DMA_MemoryInc_Enable; //����
			SD_DMA_RX_STREAM->M0AR = (uint32_t)pBuffer;
			SD_DMA_RX_STREAM->NDTR = BlockSize;
			//DMA_SetCurrDataCounter(SD_DMA_RX_STREAM, BlockSize);
			
			// ���÷���ͨ��
			SD_DMA_TX_STREAM->CR &= ~DMA_MemoryInc_Enable; //������
			SD_DMA_TX_STREAM->M0AR = (uint32_t)&Dummy_Send;
			SD_DMA_TX_STREAM->NDTR = BlockSize;
			//DMA_SetCurrDataCounter(SD_DMA_TX_STREAM, BlockSize);
			
			SD_DMAProcess();

			// Get CRC bytes (not really needed by us, but required by SD)
			SD_ReadByte();
			SD_ReadByte();
			// Set response value to success
			res = SD_RESPONSE_NO_ERROR;
		}
		else
		{
			res = SD_RESPONSE_FAILURE;
		}
		SD_DeAssert();
	}
	
	return res;
}

/**
* @brief  Reads multiple block of data from the SD.
* @param  pBuffer: pointer to the buffer that receives the data read from the
*                  SD.
* @param  ReadAddr: SD's internal address to read from.
* @param  BlockSize: the SD card Data block size.
* @param  NumberOfBlocks: number of blocks to be read.
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_ReadMultiBlocks(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	uint32_t i, j;
	SD_Error res;

	// Send CMD18 to read multiple blocks
	res = SD_CmdRsp(SD_CMD_READ_MULTIPLE_BLOCK, ReadAddr, 0xFF);
	if (!res)
	{
		// ���ý���ͨ��
		SD_DMA_RX_STREAM->CR |= DMA_MemoryInc_Enable; //����
		// ���÷���ͨ��
		SD_DMA_TX_STREAM->M0AR = (uint32_t)&Dummy_Send;
		SD_DMA_TX_STREAM->CR &= ~DMA_MemoryInc_Enable; //������
		
		for (i = 0; i < NumberOfBlocks; i++)
		{
			// Now look for the data token(0xFE) to signify the start of the data
			j = READ_BLOCK_MAX; //200ms
			while ((SD_ReadByte() != SD_START_DATA_MULTIPLE_BLOCK_READ) && --j);
			if (j > 0)
			{
				// ���ý���ͨ��
				SD_DMA_RX_STREAM->M0AR = (uint32_t)(pBuffer) + i*BlockSize;
				SD_DMA_RX_STREAM->NDTR = BlockSize;
				// ���÷���ͨ��
				SD_DMA_TX_STREAM->NDTR = BlockSize;

				SD_DMAProcess();
				
				// Get CRC bytes (not really needed by us, but required by SD)
				SD_ReadByte();
				SD_ReadByte();
				// Set response value to success
				res = SD_RESPONSE_NO_ERROR;
			}
			else
			{
				res = SD_RESPONSE_FAILURE; // Set response value to failure
			}
		}
		// CMD12 ֹͣ����
		if (!SD_SendCmd(SD_CMD_STOP_TRANSMISSION, 0x00, 0xFF))
		{
			SD_ReadByte(); //R1b
			SD_GetResponse(SD_RESPONSE_NO_ERROR);
		}
		SD_DeAssert();
	}

	return res;
}

/**
* @brief  Writes a block on the SD
* @param  pBuffer: pointer to the buffer containing the data to be written on
*                  the SD.
* @param  WriteAddr: address to write on.
* @param  BlockSize: the SD card Data block size.
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_WriteBlock(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t BlockSize)
{
	SD_Error res;
	// Send CMD24 to write single block
	res = SD_CmdRsp(SD_CMD_WRITE_SINGLE_BLOCK, WriteAddr, 0xFF);
	if (!res)
	{
		res = SD_WaitReady();
		if (!res)
		{
			// Send the data token(0xFE) to signify the start of the data
			SD_WriteByte(SD_START_DATA_SINGLE_BLOCK_WRITE);
			
			// Write the block data to SD
			// ���ý���ͨ��
			SD_DMA_RX_STREAM->CR &= ~DMA_MemoryInc_Enable; //������
			SD_DMA_RX_STREAM->M0AR = (uint32_t)&Dummy_Recv;
			SD_DMA_RX_STREAM->NDTR = BlockSize;
			// ���÷���ͨ��
			SD_DMA_TX_STREAM->CR |= DMA_MemoryInc_Enable; //����
			SD_DMA_TX_STREAM->M0AR = (uint32_t)pBuffer;
			SD_DMA_TX_STREAM->NDTR = BlockSize;
			
			SD_DMAProcess();

			// Put CRC bytes (not really needed by us, but required by SD)
			SD_ReadByte();
			SD_ReadByte();
			// ��ȡ������Ӧ
			if ((SD_ReadByte() & 0x15) == SD_DATA_OK)
			{
				res = SD_RESPONSE_NO_ERROR;
			}
			else
			{
				res = SD_RESPONSE_FAILURE;
			}
		}
	}
	else
	{	// ��ʱ SD ���ѷ�����Ӧ������Ƭ���� SPI �޷���ȷ������
		// ��ʱ SD �����ڽ�������״̬���ܿ��ܽ�δ֪����д�� SD ����
		// Ϊ���������ǿ�Ʒ��� CMD12 ֹͣ���䣬ʹ SD �����½��봫��״̬��
		if (!SD_SendCmd(SD_CMD_STOP_TRANSMISSION, 0x00, 0xFF))
		{
			SD_ReadByte();
			SD_GetResponse(SD_RESPONSE_NO_ERROR);
		}
	}
	SD_DeAssert();
	return res;
}

/**
* @brief  Writes many blocks on the SD
* @param  pBuffer: pointer to the buffer containing the data to be written on
*                  the SD.
* @param  WriteAddr: address to write on.
* @param  BlockSize: the SD card Data block size.
* @param  NumberOfBlocks: number of blocks to be written.
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_WriteMultiBlocks(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	uint32_t i;
	SD_Error res;

	if (SD_CardInfo.Type > SD_TYPE_MMC3) // д��Ԥ��������
	{	// CMD55 Application-Specific Command
		if (!SD_CmdRsp(SD_CMD_APP_CMD, 0x00, 0xFF))
		{	// ACMD23 ����Ԥ������ĸ���
			SD_CmdRsp(SD_CMD_SET_BLOCK_COUNT, NumberOfBlocks, 0xFF);
		}
	}

	// Send CMD25 to write multiple blocks
	res = SD_CmdRsp(SD_CMD_WRITE_MULTIPLE_BLOCK, WriteAddr, 0xFF);
	if (!res)
	{
		// ���ý���ͨ��
		SD_DMA_RX_STREAM->M0AR = (uint32_t)&Dummy_Recv;
		SD_DMA_RX_STREAM->CR &= ~DMA_MemoryInc_Enable; //������
		// ���÷���ͨ��
		SD_DMA_TX_STREAM->CR |= DMA_MemoryInc_Enable; //����
		
		for (i = 0; i < NumberOfBlocks; i++)
		{
			if (SD_WaitReady()) break;
			// Send the data token(0xFC) to signify the start of the data
			SD_WriteByte(SD_START_DATA_MULTIPLE_BLOCK_WRITE);
			
			// ���ý���ͨ��
			SD_DMA_RX_STREAM->NDTR = BlockSize;
			// ���÷���ͨ��
			SD_DMA_TX_STREAM->M0AR = (uint32_t)(pBuffer) + i*BlockSize;
			SD_DMA_TX_STREAM->NDTR = BlockSize;
			
			SD_DMAProcess();
			
			// Put CRC bytes (not really needed by us, but required by SD)
			SD_ReadByte();
			SD_ReadByte();
			// ��ȡ������Ӧ if (SD_GetDataResponse() != SD_DATA_OK)
			if ((SD_ReadByte() & 0x1F) != SD_DATA_OK) break;
		}
		if (i == NumberOfBlocks)
		{
			res = SD_RESPONSE_NO_ERROR;
		}
		else
		{
			res = SD_RESPONSE_FAILURE;
		}
		if (SD_WaitReady()) res = SD_RESPONSE_FAILURE;
		// Send the data token(0xFD) to signify the end of the data
		SD_WriteByte(SD_STOP_DATA_MULTIPLE_BLOCK_WRITE);
	}
	else
	{	// ��ʱ SD ���ѷ�����Ӧ������Ƭ���� SPI �޷���ȷ������
		// ��ʱ SD �����ڽ�������״̬���ܿ��ܽ�δ֪����д�� SD ����
		// Ϊ���������ǿ�Ʒ��� CMD12 ֹͣ���䣬ʹ SD �����½��봫��״̬???
		SD_WaitReady();
		// Send the data token(0xFD) to signify the end of the data
		SD_WriteByte(SD_STOP_DATA_MULTIPLE_BLOCK_WRITE);
		if (!SD_SendCmd(SD_CMD_STOP_TRANSMISSION, 0x00, 0xFF))
		{
			SD_ReadByte();
			SD_GetResponse(SD_RESPONSE_NO_ERROR);
		}
	}
	SD_DeAssert();
	return res;
}
/*
//���淽������������SD_WriteBlock��Ч�ʽϵ�
SD_Error SD_WriteMultiBlocks(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	uint32_t i;

	if (SD_CardInfo.Type != SD_TYPE_SDHC) // SDHC �� BLOCK Ϊ��λ���������� BYTE Ϊ��λ
	{
		for (i = 0; i < NumberOfBlocks; i++)
		{
			if (SD_WriteBlock(pBuffer + BlockSize * i, WriteAddr, BlockSize))
			{
				return SD_RESPONSE_FAILURE;
			}
			WriteAddr += BlockSize;
		}
	}
	else
	{
		for (i = 0; i < NumberOfBlocks; i++)
		{
			if (SD_WriteBlock(pBuffer + BlockSize * i, WriteAddr, BlockSize))
			{
				return SD_RESPONSE_FAILURE;
			}
			WriteAddr++;
		}
	}
	return SD_RESPONSE_NO_ERROR;
}
*/
/**
* @brief  Send 5 bytes command to the SD card.
* @param  Cmd: The user expected command to send to SD card.
* @param  Arg: The command argument.
* @param  Crc: The CRC.
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Failed for timeout
*         - SD_RESPONSE_NO_ERROR: Succeed
*/
SD_Error SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
	uint32_t i;
	uint8_t Frame[6];

	SD_DeAssert();
	if (SD_Assert()) return SD_RESPONSE_FAILURE;

	Frame[0] = (Cmd | 0x40); // Construct byte 1
	Frame[1] = (uint8_t)(Arg >> 24); // Construct byte 2
	Frame[2] = (uint8_t)(Arg >> 16); // Construct byte 3
	Frame[3] = (uint8_t)(Arg >> 8); // Construct byte 4
	Frame[4] = (uint8_t)(Arg); // Construct byte 5
	Frame[5] = (Crc); // Construct CRC: byte 6

	for (i = 0; i < 6; i++)
	{
		SD_WriteByte(Frame[i]); // Send the Cmd bytes
	}

	return SD_RESPONSE_NO_ERROR;
}
/**
* @brief  Send 5 bytes command to the SD card.
* @param  Cmd: The user expected command to send to SD card.
* @param  Arg: The command argument.
* @param  Crc: The CRC.
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Failed for timeout
*         - other: Response from SD card
*/
SD_Error SD_CmdRsp(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
	uint32_t i;
	SD_Error res = SD_RESPONSE_FAILURE;
	uint8_t Frame[6];

	SD_DeAssert();
	if (SD_Assert()) return SD_RESPONSE_FAILURE;

	Frame[0] = (Cmd | 0x40); // Construct byte 1
	Frame[1] = (uint8_t)(Arg >> 24); // Construct byte 2
	Frame[2] = (uint8_t)(Arg >> 16); // Construct byte 3
	Frame[3] = (uint8_t)(Arg >> 8); // Construct byte 4
	Frame[4] = (uint8_t)(Arg); // Construct byte 5
	Frame[5] = (Crc); // Construct CRC: byte 6

	for (i = 0; i < 6; i++)
	{
		SD_WriteByte(Frame[i]); // Send the Cmd bytes
	}

	i = 10; // ���� 10 �Σ��涨���������ȴ���Ӧʱ��NcrΪ 1~8 ��
	do
	{
		res = (SD_Error)SD_ReadByte();
	}while ((res & 0x80) && --i);
	
	return res;
}

/**
* @brief  Returns the SD response.
* @param  Response: response want to receive
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Sequence failed
*         - SD_RESPONSE_NO_ERROR: Sequence succeed
*/
SD_Error SD_GetResponse(uint8_t Response)
{
	uint32_t i = 10; // ���� 10 �Σ��涨���������ȴ���Ӧʱ��NcrΪ 1~8 ��
	// Check if response is got or a timeout is happen
	while ((SD_ReadByte() != Response) && --i);

	if (i)
	{
		return SD_RESPONSE_NO_ERROR; // Right response got
	}
	else
	{
		return SD_RESPONSE_FAILURE; // After time out
	}
}

/**
* @brief  Get SD card data response.
* @param  None
* @retval The SD status: Read data response xxx0<status>1
*         - status 010: Data accepted
*         - status 101: Data rejected due to a crc error
*         - status 110: Data rejected due to a Write error.
*         - status 111: Data rejected due to other error.
*/
uint8_t SD_GetDataResponse(void)
{
	uint8_t res;

	res = SD_ReadByte(); // Read response
	switch (res & 0x1F) // Mask unused bits
	{
		case SD_DATA_OK:
			return SD_DATA_OK;
		case SD_DATA_CRC_ERROR:
			return SD_DATA_CRC_ERROR;
		case SD_DATA_WRITE_ERROR:
			return SD_DATA_WRITE_ERROR;
		default:
			return res;
	}
}

/**
* @brief  Returns the SD status.
* @param  None
* @retval The SD status.
*/
uint16_t SD_GetStatus(void)
{
	uint16_t Status = 0xFFFF;

	// Send CMD13 to get SD status
	if (!SD_CmdRsp(SD_CMD_SEND_STATUS, 0x00, 0xFF))
	{
		Status = SD_RESPONSE_NO_ERROR;
		Status |= (uint16_t)(SD_ReadByte() << 8);
		SD_DeAssert();
	}

	return Status;
}


/**
* @brief  Write a byte on the SD.
* @param  Data: byte to send.
* @retval None
*/
uint8_t SD_WriteByte(uint8_t Data)
{
	while ((SD_SPI->SR & SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SD_SPI, Data);
	
	while ((SD_SPI->SR & SPI_I2S_FLAG_RXNE) == RESET);
	return (uint8_t)SPI_I2S_ReceiveData(SD_SPI);
}

/**
* @brief  Read a byte from the SD.
* @param  None
* @retval The received byte.
*/
uint8_t SD_ReadByte(void)
{
	while ((SD_SPI->SR & SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SD_SPI, SD_DUMMY_BYTE);
	
	while ((SD_SPI->SR & SPI_I2S_FLAG_RXNE) == RESET);
	return (uint8_t)SPI_I2S_ReceiveData(SD_SPI);
}

/**
* @brief  De-assert SD card
* @param  None
* @retval None
*/
void SD_DeAssert(void)
{
	SD_CS_HIGH(); // SD chip select high
	SD_ReadByte(); // Send dummy byte 0xFF
}

/**
* @brief  Wait for SD card to be asserted
* @param  None
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Failed for timeout
*         - SD_RESPONSE_NO_ERROR: Succeed
*/
SD_Error SD_Assert(void)
{
	SD_CS_LOW();
	if (!SD_WaitReady()) return SD_RESPONSE_NO_ERROR;
	
	SD_DeAssert();
	return SD_RESPONSE_FAILURE;
}

/**
* @brief  Wait for SD card to not busy status
* @param  None
* @retval The SD Response:
*         - SD_RESPONSE_FAILURE: Failed for timeout
*         - SD_RESPONSE_NO_ERROR: Succeed
*/
SD_Error SD_WaitReady(void)
{
	uint32_t i = WAIT_READY_MAX;
	//500ms, equal to 250 000 if SD_ReadByte() costs 2us
	while ((SD_ReadByte() != 0xFF) && --i);

	if (i)
	{
		return SD_RESPONSE_NO_ERROR;
	}
	else
	{
		return SD_RESPONSE_FAILURE;
	}
}
