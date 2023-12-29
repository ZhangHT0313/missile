#include "SD.h"

/*----------------------------------------------------------------------------------------
˵����
1.  Ϊ�˼���flash�洢�ռ������ʱ�䣬�ļ�ϵͳ����Ϊֻ֧��ascii��ֻ֧��ascii
    ��ζ���ļ���ֻ���Ƕ��ļ������ļ������ļ�����������8���ֽڣ�������ļ���
    �в��ܺ�����ֵķ��ţ������ļ������ܴ򿪻����ļ��������ض�
2.  SD������ʹ��SPI+DMA��������SPI_TX SPI_RX��USART3_TX UART4_TX��DMAx_Streamy
    ��ͻ����ʹ�ñ�SD����д����ʹ��USART3_TX UART4_TX����ʹ��DMA
3.  ʹ��f_sync���ڱ������ݣ��������ݶ�ʧ��������ͬ���ļ��ܺ�ʱ�����ÿ���ɴ�
    ��¼�󱣴�һ��
4.  ���ƺ����ݴ�������������໥�����������δ洢�����������򽵵ʹ���֡�ʣ�
    ����֡�ʸ��򽵵͵��δ洢��������
----------------------------------------------------------------------------------------*/

FRESULT Mount_Result;
FRESULT File_Result;

FATFS my_fats;
DIR my_dir;
FIL my_file;
FILINFO my_file_info;

char* filedir = "RM_DATA";
char filename[255] = "0.txt";

/*----------------------------------------------------------------------------------------
�������ܣ��Զ���ȡ�ļ���
----------------------------------------------------------------------------------------*/
static void AutoGetFileName(void)
{
    int file_num = 0;

    if(f_opendir(&my_dir, filedir)!=FR_OK)
    {
        f_mkdir(filedir);
        f_opendir(&my_dir, filedir);
    }
        
    do
    {
        f_readdir(&my_dir, &my_file_info);
        file_num++; //�����ļ���Ŀ���ļ����Լ�����ʽ����
    }while(my_file_info.fname[0]);
    
    sprintf(filename,"%s/%d.txt", filedir, file_num);
}

/*----------------------------------------------------------------------------------------
�������ܣ�SD������
----------------------------------------------------------------------------------------*/
void SD_Mount(void)
{
	SD_LowLevel_Init(); //��ʼ��SD����SPI DMA
	SD_Init();          //��ʼ��SDͨѶ
  
	Mount_Result = f_mount(&my_fats, "", 1);    //ʹ��FATFS��ʱ�򣬱�����ͨ��f_mount������ע��һ�������������ܿ�ʼ����API��ʹ��
	
	if(Mount_Result!=FR_OK)
	{
//		printf("SD Card mount failed\r\n");     //����ʧ��
	}
	else
	{
//		printf("SD Card mount succeed\r\n");    //���سɹ�
		SD_GetCardInfo(&SD_CardInfo);           //��ȡSD��Ϣ
		ShowCardInfo(&SD_CardInfo);             //��ӡSD��Ϣ
        AutoGetFileName();                      //�Զ���ȡ�ļ���
        File_Result = f_open( &my_file, filename, FA_OPEN_ALWAYS|FA_READ|FA_WRITE );    //���ļ�
    }
}

/*----------------------------------------------------------------------------------------
�������ܣ�SD�����ݼ�¼
ע��ff.c�Դ���f_printf����һ��ȱ�ݣ����ܴ�ӡ����������������Լ�ʵ��
----------------------------------------------------------------------------------------*/
char SD_Data_Buf[511] = {0};
u8 SD_Data_Len = 0;
void SD_printf(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SD_Data_Len = (u8)vsprintf(SD_Data_Buf, fmt, ap);	    //����ʽ�����ݴ�sz�ַ����ϣ����ظ�ʽ������

    static u32 cnt = 0;

    if(File_Result == FR_OK)
    {
        f_lseek(&my_file, my_file.fsize);   //f_lseek���ļ�ĩβ���൱����ĩ׷������
        f_puts(SD_Data_Buf, &my_file);
        
        cnt++;
        if(cnt==400)           //���ڱ��棬�������ݶ�ʧ����ͬ���ļ��ܺ�ʱ�����ÿ���ɴμ�¼����һ��
        {
            f_sync(&my_file);   //����f_close����f_close���������ִ�к�����������ǰ�ļ��Ƿ���Ҫ����f_open
//            f_close(&my_file);
            cnt = 0;
        }
    }
    else
    {
//        printf("can't open\r\n");
    }
}

void DataRecord_Control(void)
{
		static u8 sd_cnt = 0;
	  if(sd_cnt < 20-1)
			sd_cnt++;
		else
		{
			sd_cnt = 0;
			if(Time_Second < 60-1)
				Time_Second++;
			else
			{
				Time_Second = 0;
				Time_Minute++;
			}
		}
		
    SD_printf(
				"\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d,%d,%d\n",
		
		
		
		
		
		
//		"\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d,%d,\
//%d,%d,%d,%d		\
//\n",
		
		Data_From_C1[0],
		Data_From_C1[1],
		Data_From_C1[2],
		Data_From_C1[3],
		Data_From_C1[4],		
		
		Data_From_C1[5],
		Data_From_C1[6],
		Data_From_C1[7],
		Data_From_C1[8],
		Data_From_C1[9],

		Data_From_C1[10],
		Data_From_C1[11],
		Data_From_C1[12],
		Data_From_C1[13]
		
		);
/*
		1 ����ʱ�� min
		2 ����ʱ�� s
		3 ��׼��־λ
		4 Yaw����ֵ
		5 YawĿ��ֵ		
		
		6 ���������
		7 ң��ģʽ ״̬��
		8 ����ģʽ ״̬��
		9 LaunchDnCnt
		10 LaunchTimeErr
		
		11 ң������ģʽ��־λ		
		12 Target ǰ��վYaw
		13 Target ����  Yaw
		14 L1�¶�
		15 L2�¶�
		
		16 R1�¶�
		17 R2�¶�
		18 Yaw�¶�
		19 Track�¶�		
		20 ǰ��վ����Yaw 1

		21 ǰ��վ����Yaw 2
		22 ǰ��վ����Yaw 3
		23 ǰ��վ����Yaw 4
		24 ǰ��վ����Friction 1
		25 ǰ��վ����Friction 2
		
		26 ǰ��վ����Friction 3
		27 ǰ��վ����Friction 4
		28 ���ز���Yaw1		
		29 ���ز���Yaw2
		30 ���ز���Yaw3
		
		31 ���ز���Yaw4
		32 ���ز���Friction 1
		33 ���ز���Friction 2		
		34 ���ز���Friction 3
		35 ���ز���Friction 4
		
		36 L1Ŀ��ֵ
		37 L1����ֵ
		38 L2Ŀ��ֵ
		39 L2����ֵ
		40 R1Ŀ��ֵ
		
		41 R1����ֵ
		42 R2Ŀ��ֵ
		43 R2����ֵ
		44 Friction�ȶ���־λ
		45 Yaw�ȶ���־λ
		
		46 Track�ȶ���־λ
		47 ����ģʽ ��������		
		48 ����ģʽ ����Ŀ��
		49 ����ģʽ բ��״̬		
		50 ����ģʽ բ�ſ���ʣ��ʱ��
		
		51 ����ģʽ ���һ�β���ʱ������ʣ��ʱ��
		52 ����ģʽ ���һ���л�ʱ������ʣ��ʱ��
		53 ����ģʽ ��������
		54 ����ģʽ �����׶�
		55 ����ģʽ ��ǰ�׶�ʣ��ʱ��
		
		56 ������id
		57 �����״����
		58 Pitch�������ֵ
		59 Pitch���Ŀ��ֵ
*/
}

