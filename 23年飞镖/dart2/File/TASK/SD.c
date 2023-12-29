#include "SD.h"

/*----------------------------------------------------------------------------------------
说明：
1.  为了减少flash存储空间和下载时间，文件系统设置为只支持ascii，只支持ascii
    意味着文件名只能是短文件名（文件名和文件夹名不超过8个字节），因此文件名
    中不能含有奇怪的符号，否则文件将不能打开或者文件名将被截断
2.  SD卡驱动使用SPI+DMA，但由于SPI_TX SPI_RX与USART3_TX UART4_TX的DMAx_Streamy
    冲突，故使用本SD卡读写则不能使用USART3_TX UART4_TX不能使用DMA
3.  使用f_sync定期保存数据，避免数据丢失，但由于同步文件很耗时，因此每若干次
    记录后保存一次
4.  控制好数据带宽，避免任务间相互堵塞，即单次存储的数据量大则降低储存帧率，
    储存帧率高则降低单次存储的数据量
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
函数功能：自动获取文件名
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
        file_num++; //计算文件数目，文件名以计数方式递增
    }while(my_file_info.fname[0]);
    
    sprintf(filename,"%s/%d.txt", filedir, file_num);
}

/*----------------------------------------------------------------------------------------
函数功能：SD卡挂载
----------------------------------------------------------------------------------------*/
void SD_Mount(void)
{
	SD_LowLevel_Init(); //初始化SD所需SPI DMA
	SD_Init();          //初始化SD通讯
  
	Mount_Result = f_mount(&my_fats, "", 1);    //使用FATFS的时候，必须先通过f_mount，函数注册一个工作区，才能开始后续API的使用
	
	if(Mount_Result!=FR_OK)
	{
//		printf("SD Card mount failed\r\n");     //挂载失败
	}
	else
	{
//		printf("SD Card mount succeed\r\n");    //挂载成功
		SD_GetCardInfo(&SD_CardInfo);           //获取SD信息
		ShowCardInfo(&SD_CardInfo);             //打印SD信息
        AutoGetFileName();                      //自动获取文件名
        File_Result = f_open( &my_file, filename, FA_OPEN_ALWAYS|FA_READ|FA_WRITE );    //打开文件
    }
}

/*----------------------------------------------------------------------------------------
函数功能：SD卡数据记录
注：ff.c自带的f_printf存在一定缺陷，不能打印浮点数，因此这里自己实现
----------------------------------------------------------------------------------------*/
char SD_Data_Buf[511] = {0};
u8 SD_Data_Len = 0;
void SD_printf(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SD_Data_Len = (u8)vsprintf(SD_Data_Buf, fmt, ap);	    //将格式化数据打到sz字符串上，返回格式化长度

    static u32 cnt = 0;

    if(File_Result == FR_OK)
    {
        f_lseek(&my_file, my_file.fsize);   //f_lseek到文件末尾，相当于文末追加内容
        f_puts(SD_Data_Buf, &my_file);
        
        cnt++;
        if(cnt==400)           //定期保存，避免数据丢失，但同步文件很耗时，因此每若干次记录保存一次
        {
            f_sync(&my_file);   //兼容f_close，与f_close的区别就是执行后如果想操作当前文件是否需要重新f_open
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
		1 比赛时间 min
		2 比赛时间 s
		3 对准标志位
		4 Yaw反馈值
		5 Yaw目标值		
		
		6 激光测距读数
		7 遥控模式 状态机
		8 比赛模式 状态机
		9 LaunchDnCnt
		10 LaunchTimeErr
		
		11 遥控连发模式标志位		
		12 Target 前哨站Yaw
		13 Target 基地  Yaw
		14 L1温度
		15 L2温度
		
		16 R1温度
		17 R2温度
		18 Yaw温度
		19 Track温度		
		20 前哨站补偿Yaw 1

		21 前哨站补偿Yaw 2
		22 前哨站补偿Yaw 3
		23 前哨站补偿Yaw 4
		24 前哨站补偿Friction 1
		25 前哨站补偿Friction 2
		
		26 前哨站补偿Friction 3
		27 前哨站补偿Friction 4
		28 基地补偿Yaw1		
		29 基地补偿Yaw2
		30 基地补偿Yaw3
		
		31 基地补偿Yaw4
		32 基地补偿Friction 1
		33 基地补偿Friction 2		
		34 基地补偿Friction 3
		35 基地补偿Friction 4
		
		36 L1目标值
		37 L1反馈值
		38 L2目标值
		39 L2反馈值
		40 R1目标值
		
		41 R1反馈值
		42 R2目标值
		43 R2反馈值
		44 Friction稳定标志位
		45 Yaw稳定标志位
		
		46 Track稳定标志位
		47 比赛模式 发射数量		
		48 比赛模式 击打目标
		49 比赛模式 闸门状态		
		50 比赛模式 闸门开启剩余时间
		
		51 比赛模式 最近一次操作时，比赛剩余时间
		52 比赛模式 最近一次切换时，比赛剩余时间
		53 比赛模式 比赛类型
		54 比赛模式 比赛阶段
		55 比赛模式 当前阶段剩余时间
		
		56 机器人id
		57 发给雷达的数
		58 Pitch电机反馈值
		59 Pitch电机目标值
*/
}

