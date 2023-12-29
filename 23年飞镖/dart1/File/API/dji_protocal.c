#include "dji_protocal.h"





#define EXT_GAME_ROBOT_STATUS_INIT(ID,level,remainHP,maxHP,shooterHeatCoolingRate,shooterHeatLimit,shooterSpeedLimit,chassis_power_limit) \
			{ID, level, remainHP, maxHP, 0, 0, 0, 0, 0, 0, shooterHeatCoolingRate, shooterHeatLimit, shooterSpeedLimit, chassis_power_limit, 0, 0, 0}

ST_Client_Sendata_Coe Client_Sendata_Coe;
extern ST_Client_Draw_Ruler Client_Draw_Ruler;
ST_RSYS_MONITOR RSYS_Monitor = {0};

ext_game_status_t				      	G_ST_Game_Status;//1.比赛状态数据
ext_game_result_t               G_ST_Game_Result;//2.比赛结果数据
ext_game_robot_HP_t             G_ST_Game_Robot_HP;//3.机器人血量数据
ext_dart_status_t			      		G_ST_Dart_Status;//4.飞镖发射状态
ext_event_data_t                G_ST_Event_Data;//6.场地事件数据
ext_supply_projectile_action_t  G_ST_Supply_Projectile_Action;//7.补给站动作标识
ext_referee_warning_t           G_ST_Referee_Warning;//8.裁判警告信息
ext_dart_remaining_time_t			  G_ST_Dart_Remaining_Time;//9.飞镖发射口倒计时
ext_game_robot_status_t         G_ST_Game_Robot_Status;//10.比赛机器人状态
ext_power_heat_data_t           G_ST_Power_Heat_Data;//11.实时功率热量数据
ext_game_robot_pos_t            G_ST_Game_Robot_Pos;//12.机器人位置
ext_buff_t						        	G_ST_Buff;//13.机器人增益
aerial_robot_energy_t           G_ST_Aerial_Robot_Energy;//14.空中机器人能量状态
ext_robot_hurt_t                G_ST_Robot_Hurt;//15.伤害状态
ext_shoot_data_t                G_ST_Shoot_Data;//16.实时射击信息
ext_bullet_remaining_t          G_ST_Bullet_Remaining;//17.子弹剩余发射数
ext_rfid_status_t				      	G_ST_RFID_Status;//18.机器人 RFID 状态
ext_dart_client_cmd_t           G_ST_Dart_Client_Cmd;//19.飞镖机器人客户端指令数据

ext_student_interactive_header_data_t		G_ST_Student_Interactive_Header_Data;//交互数据接收信息
robot_interactive_data_t					      G_ST_Robot_Interactive_Data;//1.交互数据
ext_client_custom_graphic_delete_t			G_ST_Client_Custom_Graphic_Delete;//2.客户端删除图形
ext_client_custom_graphic_single_t			G_ST_Client_Custom_Graphic_Single;//3.客户端绘制一个图形
ext_client_custom_graphic_double_t			G_ST_Client_Custom_Graphic_Double;//4.客户端绘制二个图形
ext_client_custom_graphic_five_t		  	G_ST_Client_Custom_Graphic_Five;//5.客户端绘制五个图形
ext_client_custom_graphic_seven_t			G_ST_Client_Custom_Graphic_Seven;//6.客户端绘制七个图形
ext_client_custom_character_t			    G_ST_Client_Custom_Character;//7.客户端绘制字符
ext_client_custom_character_t			    G_ST_Client_Custom_Character2;//7.客户端绘制字符

ext_aerial_data_t DataFromAerial 		=		{2}; //来自无人机的数据

u8 AimStar_Y[4] = "YYY";
u8 AimStar_X1[4] = "XX1";
u8 AimStar_X2[4] = "XX2";
u8 AimStar_X3[4] = "XX3";
u8 AimStar_X4[4] = "XX4";
u8 AimStar_X5[4] = "XX5";
u8 AimStar_X6[4] = "XX6";
u8 AimStar_X7[4] = "XX7";
u8* AimStar_X[] = {AimStar_X1,AimStar_X2,AimStar_X3,AimStar_X4,AimStar_X5,AimStar_X6,AimStar_X7};
u8 floatData1Name[4] = "FP1";
u8 floatData2Name[4] = "FP2";
u8 floatData3Name[4] = "FP3";
u8 floatData4Name[4] = "FP4";
u8 floatData5Name[4] = "FP5";
u8 floatData6Name[4] = "FP6";
u8 floatData7Name[4] = "FP7";
u8* floatDataName[] = {floatData1Name, floatData2Name, floatData3Name, floatData4Name, floatData5Name, floatData6Name, floatData7Name};
u8 Speed_Char[4] = "SP1";
u8 Enemy_Senor_F[4] = "FFF";
u8 Enemy_Senor_L[4] = "LLL";
u8 Enemy_Senor_B[4] = "BBB";
u8 Enemy_Senor_R[4] = "RRR";
u8 *Enemy_Senor[4] = {Enemy_Senor_F, Enemy_Senor_L, Enemy_Senor_B, Enemy_Senor_R};
int show_float(float x)
{
	int i,sum=0;
	byte_pointer start;
	start = (byte_pointer)&x;
	for(i=0;i<4;i++){
		sum+=start[i]<<8*i;
	}
	return sum;
}
/*------------------------------------------------------------------------
函 数 名：Rc_RsysProtocol(void)
函数功能：裁判系统数据接收校验函数
------------------------------------------------------------------------*/
u8  Rsys_RX_Status   = RSYS_RX_FREE;
u8  Rsys_RX_Buf[UART5_RXMB_LEN] = {0};
u8  usData           = 0;
u16 usDataLength     = 0;   //数据帧长度
u16 ucCmdID          = 0;
u8  Buf_num          = 0;
void Rc_RsysProtocol(void)
{
//    u8  Buf_num = 0;
    for(u32 i=0; i<UART5_Rcr.rxSize; i++)	   //循环次数和bufbuf长度一致
    {
        usData = UART5_Rcr.pMailbox[i];
        switch(Rsys_RX_Status)
        {
        case RSYS_RX_FREE:
            if(usData == 0xA5)
            {
                Buf_num = 0;
                Rsys_RX_Status = RSYS_RX_Length;//自由状态下接到0xA5认为开始
                Rsys_RX_Buf[Buf_num++] = 0xA5;
            }
            else
            {
                if(usData == 0xA5)
                    Rsys_RX_Status = RSYS_RX_Length;
                else
                    Rsys_RX_Status = RSYS_RX_FREE;
            }
            break;
        case RSYS_RX_Length:
            if(Buf_num <= 4)
            {
                Rsys_RX_Buf[Buf_num++] = usData;
            }
            else if(Buf_num > 4)
            {
                usDataLength = Rsys_RX_Buf[1] | (Rsys_RX_Buf[2]<<8);
//					Seq = Rsys_RX_Buf[3];
                Rsys_RX_Buf[Buf_num++]= usData;
//					CRC8 = Rsys_RX_Buf[4];

                if(Buf_num == usDataLength+5)
                {
                    if(Verify_CRC8_Check_Sum(Rsys_RX_Buf, 5) == 1)
                        Rsys_RX_Status = RSYS_RX_CRC16;
                    else
                    {
                        if(usData == 0xA5)
                            Rsys_RX_Status = RSYS_RX_Length;
                        else
                            Rsys_RX_Status = RSYS_RX_FREE;
                    }
                }
            }
            break;
        case RSYS_RX_CRC16:
            if(Buf_num < (9 + usDataLength))
            {
                Rsys_RX_Buf[Buf_num++] = usData;
            }
            if(Buf_num >= (9 + usDataLength))
            {
                Buf_num = 0;
                Rsys_RX_Status = RSYS_RX_FREE;
                if(Verify_CRC16_Check_Sum(Rsys_RX_Buf, usDataLength + 9))
                {
                    ucCmdID = Rsys_RX_Buf[5] | (Rsys_RX_Buf[6]<<8);
                    MonitorDataDeal(ucCmdID);
                }
            }
            break;
        default:
            break;
        }
    }
    memset(Rsys_RX_Buf, 0, UART5_RXMB_LEN);
    memset(UART5_Rcr.pMailbox, 0, UART5_RXMB_LEN);
}
/*------------------------------------------------------------------------
函 数 名：MonitorDataDeal(u16 usCmdID)
函数功能：裁判系统数据处理函数
------------------------------------------------------------------------*/
extern u8 Armor_Angle_Judgment(void); //YAW 1:1
u32 Last_Attacked = 0;
ext_supply_projectile_action_t  G_ST_Supply_Projectile_Action_Pre;//7.补给站动作标识
extern ST_Client_Sendata_Coe Client_Sendata_Coe;
//float Time2;
//extern float Time1;
void MonitorDataDeal(u16 usCmdID)
{
    RSYS_Monitor.total_cnt++;
    switch(usCmdID)
    {
    case GameStatus_ID:                 //1.比赛状态
        memcpy(&G_ST_Game_Status, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.GameStatus_cnt++;
        break;
    case GameResult_ID:                 //2.比赛结果
        memcpy(&G_ST_Game_Result, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.GameResult_cnt++;
        break;
    case GameRobotHP_ID:                //3.机器人血量数据
        memcpy(&G_ST_Game_Robot_HP, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.GameRobotHP_cnt++;
        break;
    case DartStatus_ID:                 //4.飞镖发射状态
        memcpy(&G_ST_Dart_Status, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.DartStatus_cnt++;
        break;
    case EventData_ID:                  //6.场地事件
        memcpy(&G_ST_Event_Data, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.EventData_cnt++;
        break;
    case SupplyProjectileAction_ID:     //7.补给站动作
        memcpy(&G_ST_Supply_Projectile_Action, &Rsys_RX_Buf[7], usDataLength);
		if((G_ST_Supply_Projectile_Action.supply_projectile_step ==2)&&(G_ST_Supply_Projectile_Action_Pre.supply_projectile_step ==0)
			&&(G_ST_Supply_Projectile_Action.supply_robot_id == MY_ID)&&(G_ST_Supply_Projectile_Action_Pre.supply_robot_id ==MY_ID))
		{
			Client_Sendata_Coe.Bullet_Num += G_ST_Supply_Projectile_Action.supply_projectile_num;
		}
		G_ST_Supply_Projectile_Action_Pre = G_ST_Supply_Projectile_Action;
        RSYS_Monitor.SupplyProjectileAction_cnt++;
        break;
    case RefereeWarning_ID:             //8.裁判警告信息
        memcpy(&G_ST_Referee_Warning, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.RefereeWarning_cnt++;
        break;
    case DartRemainingTime_ID:          //9.飞镖发射口倒计时
        memcpy(&G_ST_Dart_Remaining_Time, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.DartRemainingTime_cnt++;
    case GameRobotStatus_ID:            //10.比赛机器人状态
        memcpy(&G_ST_Game_Robot_Status, &Rsys_RX_Buf[7], usDataLength);
//		G_ST_Game_Robot_Status.max_chassis_power=50;
        RSYS_Monitor.GameRobotStatus_cnt++;
        break;
    case PowerHeatData_ID:              //11.实时功率热量数据
        memcpy(&G_ST_Power_Heat_Data, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.PowerHeatData_cnt++;
        break;
    case GameRobotPos_ID:               //12.机器人位置
        memcpy(&G_ST_Game_Robot_Pos, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.GameRobotPos_cnt++;
        break;
    case Buff_ID:                       //13.机器人增益
        memcpy(&G_ST_Buff, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.Buff_cnt++;
        break;
    case AerialRobotEnergy_ID:          //14.空中机器人能量状态
        memcpy(&G_ST_Aerial_Robot_Energy, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.AerialRobotEnergy_cnt++;
        break;
    case RobotHurt_ID:                  //15.伤害状态
        memcpy(&G_ST_Robot_Hurt, &Rsys_RX_Buf[7], usDataLength);
	    Armor_Angle_Judgment(); //YAW 1:1
		if(G_ST_Robot_Hurt.hurt_type == 0) Last_Attacked = TIM5->CNT;
        RSYS_Monitor.RobotHurt_cnt++;
        break;
    case ShootData_ID:                  //16.实时射击信息
        memcpy(&G_ST_Shoot_Data, &Rsys_RX_Buf[7], usDataLength);
//        PelletStatusUpdate();           //更新拨弹信息
//		Client_Sendata_Coe.Bullet_Num--;
//		Time2 = (float)(TIM5->CNT-Time1)/1000;
		RSYS_Monitor.ShootData_cnt++;
        break;
    case BulletRemaining_ID:            //17.子弹剩余发射数
        memcpy(&G_ST_Bullet_Remaining, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.BulletRemaining_cnt++;
        break;
    case RFIDStatus_ID:                 //18.机器人RFID状态
        memcpy(&G_ST_RFID_Status, &Rsys_RX_Buf[7], usDataLength);
        RSYS_Monitor.BulletRemaining_cnt++;
        break;
		case DartClientCmd_ID:							//19.飞镖机器人客户端指令数据
			memcpy(&G_ST_Dart_Client_Cmd, &Rsys_RX_Buf[7], usDataLength);
			RSYS_Monitor.DartClientCmd_cnt++;
			break;
    case Robot_Interactive_ID:           //机器人间交互数据
    {
        memcpy((void*)&G_ST_Robot_Interactive_Data.m_ReceiveData, &Rsys_RX_Buf[7], usDataLength);
        if( (G_ST_Robot_Interactive_Data.m_ReceiveData[0] | G_ST_Robot_Interactive_Data.m_ReceiveData[1] << 8) == 0x0200 && //数据段的内容ID
                (G_ST_Robot_Interactive_Data.m_ReceiveData[2] | G_ST_Robot_Interactive_Data.m_ReceiveData[3] << 8) == MY_ID-2 && //发送者的ID为无人机
                (G_ST_Robot_Interactive_Data.m_ReceiveData[4] | G_ST_Robot_Interactive_Data.m_ReceiveData[5] << 8) == MY_ID   //接收者的ID为飞镖
          )
        {
//					memcpy(&DataFromAerial,&Rsys_RX_Buf[13],1);
					DataFromAerial.Dart_Num = G_ST_Robot_Interactive_Data.m_ReceiveData[6];
        }
        RSYS_Monitor.RobotInteractive_cnt++;
        break;
    }
    default:
        break;
    }
}

/*------------------------------------------------------------------------
函 数 名：void Send_Data_To_Robot(void)
函数功能：向机器人发送自定义数据
重要参数：给谁发，发什么(给云台手发电容电量百分比和子弹)
------------------------------------------------------------------------*/
u8 Custom_DataBuf[16] = {0};
u16 Custom_Data_Length = NULL;
u16 Des_ID = NULL;
u16 Soldier_USE_ID = 0x203;//0x0200~0x02FF
u16 Friend_ID = NULL;
ST_Data_to_Ariel Data_to_Ariel;
ST_Data_to_Radar Data_to_Radar = {1};
u8 Data_to_Visual[6]={0};
ST_Data_From_Visual Data_From_Visual={0};

void float_to_u8(float f, u8* a) 
{
	int* p = (int*)&f;
	int t = *p;
	*a = 0xff & (t >> 24);
	*(a + 1) = 0xff & (t >> 16);
	*(a + 2) = 0xff & (t >> 8);
	*(a + 3) = 0xff & t;
	return;
}

float u8_to_float(u8* a)
{
	int b = 0;
	b = ((*a) << 24) | (*(a + 1) << 16) | (*(a + 2) << 8) | *(a + 3);
	float* ret = (float*)&b;
	return *ret;
}

void int_to_u8(int data,u8*tx ,int n)
{
    tx[4*n+0]= data>>24 & 0xff;
    tx[4*n+1]= data>>16 & 0xff;
    tx[4*n+2]= data>>8 & 0xff;
    tx[4*n+3]= data & 0xff;
}

int u8_to_int(u8*rx ,int n)
{
		int data;
    data=rx[4*n+0]<<24|rx[4*n+1]<<16|rx[4*n+2]<<8|rx[4*n+3];
	  return data;
}

//void Send_Data_To_Aerial(void)
//{
//	//对发送目标做判断
//	if(MY_ID>=Red_1_Hero&&MY_ID<=Red_9_Radar) Friend_ID = Red_6_Aerial;//Red_7_Sentry;
//	if(MY_ID>=Blue_1_Hero&&MY_ID<=Blue_9_Radar) Friend_ID = Blue_6_Aerial;
//	//获得信息
//	Data_to_Ariel.Dart_Remaining_Time                 =		G_ST_Dart_Remaining_Time.dart_remaining_time;
//	Data_to_Ariel.FrictionStable_Flag									=		FrictionStable_Flag;
//	Data_to_Ariel.Pitch_PosErr												=		g_stPitch_PosPID.fpDes - g_stPitch_PosPID.fpFB;
//	Data_to_Ariel.Yaw_PosErr													=		g_stYaw_PosPID.fpDes - g_stYaw_PosPID.fpFB;
//	Data_to_Ariel.Track_PosErr											=		g_stTrack_PosPID.fpDes - g_stTrack_PosPID.fpFB;
//	Data_to_Ariel.Friction_L1_SpeedErr								=		g_stFriction_L1_SpeedPID.fpDes - g_stFriction_L1_SpeedPID.fpFB;
//	Data_to_Ariel.Friction_L2_SpeedErr								=		g_stFriction_L2_SpeedPID.fpDes - g_stFriction_L2_SpeedPID.fpFB;
//	Data_to_Ariel.Friction_R1_SpeedErr								=		g_stFriction_R1_SpeedPID.fpDes - g_stFriction_R1_SpeedPID.fpFB;
//	Data_to_Ariel.Friction_R2_SpeedErr								=		g_stFriction_R2_SpeedPID.fpDes - g_stFriction_R2_SpeedPID.fpFB;
//	//配置
//    Custom_DataBuf[0] = 0xA5;                           //帧头
//    Custom_DataBuf[1] = (u8)(sizeof(Data_to_Ariel)+6);        //数据长度低8位
//    Custom_DataBuf[2] = (u8)((sizeof(Data_to_Ariel)+6)>>8);   //数据长度高8位
//    Custom_DataBuf[3] = 0x00;                           //Seq
//    Append_CRC8_Check_Sum(Custom_DataBuf, 5);           //CRC8
//    Custom_DataBuf[5] = 0x01;                           //机器人间通信ID低8位
//    Custom_DataBuf[6] = 0x03;                           //机器人间通信ID高8位
//    Custom_DataBuf[7] = (u8)Soldier_USE_ID;              //数据的内容ID低8位
//    Custom_DataBuf[8] = (u8)(Soldier_USE_ID>>8);         //数据的内容ID高8位
//    Custom_DataBuf[9] = (u8)MY_ID;                      //发送方ID低8位
//    Custom_DataBuf[10] = (u8)(MY_ID>>8);                //发送方ID高8位
//    Custom_DataBuf[11] = (u8)Friend_ID;                 //接收方ID低8位
//    Custom_DataBuf[12] = (u8)(Friend_ID>>8);            //接收方ID高8位
//		
//		Custom_DataBuf[13] = (u8)(Data_to_Ariel.Dart_Remaining_Time);            
//    Custom_DataBuf[14] = (u8)(Data_to_Ariel.FrictionStable_Flag);
//    

//		float_to_u8(Data_to_Ariel.Pitch_PosErr, Custom_DataBuf+15);
//		float_to_u8(Data_to_Ariel.Yaw_PosErr, Custom_DataBuf+19);	
//		float_to_u8(Data_to_Ariel.Track_PosErr, Custom_DataBuf+23);
//		float_to_u8(Data_to_Ariel.Friction_L1_SpeedErr, Custom_DataBuf+27);
//		float_to_u8(Data_to_Ariel.Friction_L2_SpeedErr, Custom_DataBuf+31);
//		float_to_u8(Data_to_Ariel.Friction_R1_SpeedErr, Custom_DataBuf+35);
//		float_to_u8(Data_to_Ariel.Friction_R2_SpeedErr, Custom_DataBuf+39);		


//    //数据
//	
//    Append_CRC16_Check_Sum(Custom_DataBuf, (sizeof(Data_to_Ariel)+15));			//长度6+9+X=15+X

////串口5发送
////    while(DMA_GetCurrDataCounter(UART5_TX_STREAM));          //等之前的发完
//    DMA_ClearITPendingBit(USART5_TX_STREAM, DMA_IT_TCIF7);      //开启DMA_Mode_Normal,即便没有使用完成中断也要软件清除，否则只发一次

//    DMA_Cmd(USART5_TX_STREAM, DISABLE);                         //设置当前计数值前先禁用DMA
//    USART5_TX_STREAM->M0AR = (uint32_t)Custom_DataBuf;         //设置当前待发数据基地址:Memory0 tARget
//    USART5_TX_STREAM->NDTR = (uint32_t)(sizeof(Data_to_Ariel)+15);         //设置当前待发的数据的数量:Number of Data units to be TRansferred
//    DMA_Cmd(USART5_TX_STREAM, ENABLE);                          //启用串口DMA发送
//}




/*------------------------------------------------------------------------
函 数 名：void sendPictureToClient(void* picture, u16 pictureNum)
函数功能：向客户端发送自定义图像
------------------------------------------------------------------------*/
void sendPictureToClient(void* picture, u16 pictureNum)
{
//    u16 dataLength = 6 + pictureNum * 15;

//    switch(pictureNum) //命令ID
//    {
//    case 1:
//        G_ST_Student_Interactive_Header_Data.data_cmd_id = 0x0101;
//        break;
//    case 2:
//        G_ST_Student_Interactive_Header_Data.data_cmd_id = 0x0102;
//        break;
//    case 5:
//        G_ST_Student_Interactive_Header_Data.data_cmd_id = 0x0103;
//        break;
//    case 7:
//        G_ST_Student_Interactive_Header_Data.data_cmd_id = 0x0104;
//        break;
//	case 3://客户端绘制字符
//        G_ST_Student_Interactive_Header_Data.data_cmd_id = 0x0110;
//        break;
//    default:
////        return;
//        break;
//    }

//    G_ST_Student_Interactive_Header_Data.sender_ID = MY_ID; //发送方ID
//    switch(G_ST_Student_Interactive_Header_Data.sender_ID) //接收方ID
//    {
//    case Red_3_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Red_3_Standard_Client;
//        break;
//    case Red_4_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Red_4_Standard_Client;
//        break;
//	case Red_5_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Red_5_Standard_Client;
//        break;
//	case Blue_3_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Blue_3_Standard_Client;
//        break;
//	case Blue_4_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Blue_4_Standard_Client;
//        break;
//	case Blue_5_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Blue_5_Standard_Client;
//        break;
//    default:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Red_3_Standard_Client;
//        break;
//    }
//    Custom_DataBuf[0] = 0xA5;                           //帧头
//    Custom_DataBuf[1] = (u8)dataLength;                 //数据长度低8位
//    Custom_DataBuf[2] = (u8)(dataLength >> 8);          //数据长度高8位
//    Custom_DataBuf[3] = 0x00;                           //Seq
//    Append_CRC8_Check_Sum(Custom_DataBuf, 5);           //CRC8
//    Custom_DataBuf[5] = 0x01;                           //cmd_id低八位
//    Custom_DataBuf[6] = 0x03;                           //cmd_id高八位
//    memcpy(&Custom_DataBuf[7], &G_ST_Student_Interactive_Header_Data, 6);//Header
//    memcpy(&Custom_DataBuf[13], picture, pictureNum * 15);     //数据
//    Append_CRC16_Check_Sum(Custom_DataBuf, dataLength + 9);    //CRC16

////	while(DMA_GetCurrDataCounter(UART5_TX_STREAM));            //等之前的发完
//    DMA_ClearITPendingBit(USART5_TX_STREAM, DMA_IT_TCIF7);      //开启DMA_Mode_Normal,即便没有使用完成中断也要软件清除，否则只发一次
//    DMA_Cmd(USART5_TX_STREAM, DISABLE);                         //设置当前计数值前先禁用DMA
//    USART5_TX_STREAM->M0AR = (uint32_t)&Custom_DataBuf;         //设置当前待发数据基地址:Memory0 tARget
//    USART5_TX_STREAM->NDTR = (uint32_t)(dataLength + 9);        //设置当前待发的数据的数量:Number of Data units to be TRansferred
//    DMA_Cmd(USART5_TX_STREAM, ENABLE);                          //启用串口DMA发送
}

/*------------------------------------------------------------------------
函 数 名：void sendDataToClient(float data1, float data2, float data3, float data4, float data5)
函数功能：发送五个自定义数据
------------------------------------------------------------------------*/
void sendDataToClient(float data1, float data2, float data3, float data4, float data5)
{
//    u32 data_1_u32;
//    u32 data_2_u32;
//    u32 data_3_u32;
//    u32 data_4_u32;
//    u32 data_5_u32;

//    memcpy(&data_1_u32, &data1, 4);
//    memcpy(&data_2_u32, &data2, 4);
//    memcpy(&data_3_u32, &data3, 4);
//    memcpy(&data_4_u32, &data4, 4);
//    memcpy(&data_5_u32, &data5, 4);

//    pictureOperation(&G_ST_Client_Custom_Graphic_Five.grapic_data_struct[0], floatData1Name, 2, 5, 2, 7, 10, 2, 2, 10, 10, data_1_u32 & 0x3FF, (data_1_u32 >> 10) & 0x7FF, (data_1_u32 >> 21) & 0x7FF);
//    pictureOperation(&G_ST_Client_Custom_Graphic_Five.grapic_data_struct[1], floatData2Name, 2, 5, 2, 7, 10, 2, 2, 10, 20, data_2_u32 & 0x3FF, (data_2_u32 >> 10) & 0x7FF, (data_2_u32 >> 21) & 0x7FF);
//    pictureOperation(&G_ST_Client_Custom_Graphic_Five.grapic_data_struct[2], floatData3Name, 2, 5, 2, 7, 10, 2, 2, 10, 30, data_3_u32 & 0x3FF, (data_3_u32 >> 10) & 0x7FF, (data_3_u32 >> 21) & 0x7FF);
//    pictureOperation(&G_ST_Client_Custom_Graphic_Five.grapic_data_struct[3], floatData4Name, 2, 5, 2, 7, 10, 2, 2, 10, 40, data_4_u32 & 0x3FF, (data_4_u32 >> 10) & 0x7FF, (data_4_u32 >> 21) & 0x7FF);
//    pictureOperation(&G_ST_Client_Custom_Graphic_Five.grapic_data_struct[1], floatData5Name, 2, 5, 2, 7, 10, 2, 2, 10, 50, data_5_u32 & 0x3FF, (data_5_u32 >> 10) & 0x7FF, (data_5_u32 >> 21) & 0x7FF);

//    sendPictureToClient(&G_ST_Client_Custom_Graphic_Five, 5);
}

/*------------------------------------------------------------------------
函 数 名：void Picture_Operation(graphic_data_struct_t* pGraphicData, u8* graphicName, u32 operateTpye, u32 graphicTpye,
								u32 layer, u32 color, u32 startAngle, u32 endAngle, u32 width, u32 startX, u32 startY,
								u32 radius, u32 endX, u32 endY)
函数功能：对图像进行操作
------------------------------------------------------------------------*/
void pictureOperation(graphic_data_struct_t* pGraphicData, u8* graphicName, u32 operateTpye, u32 graphicTpye,
                      u32 layer, u32 color, u32 startAngle, u32 endAngle, u32 width, u32 startX, u32 startY,
                      u32 radius, u32 endX, u32 endY)
{
    if((pGraphicData == NULL) || (graphicName == NULL))
    {
        return;
    }

    memcpy(pGraphicData->graphic_name, graphicName, 3); //图形名
    pGraphicData->operate_tpye = operateTpye; //图形操作
    pGraphicData->graphic_tpye = graphicTpye; //图形类型
    pGraphicData->layer = layer; //图层数，0-9
    pGraphicData->color = color; //颜色
    pGraphicData->start_angle = startAngle; //起始角度，单位：°，范围[0,360]
    pGraphicData->end_angle = endAngle; //终止角度，单位：°，范围[0,360]
    pGraphicData->width = width; //线宽
    pGraphicData->start_x = startX; //起点 x 坐标
    pGraphicData->start_y = startY; //起点 y 坐标
    pGraphicData->radius = radius; //字体大小或者半径
    pGraphicData->end_x = endX; //终点 x 坐标
    pGraphicData->end_y = endY; //终点 x 坐标
}

/*------------------------------------------------------------------------
函 数 名：void deletePictureOnClient(u8 operateTpye, u8 layer)
函数功能：删除客户端图形
------------------------------------------------------------------------*/
void deletePictureOnClient(u8 operateTpye, u8 layer)
{
//    G_ST_Student_Interactive_Header_Data.data_cmd_id = 0x0100; //数据的内容ID

//    G_ST_Student_Interactive_Header_Data.sender_ID = MY_ID; //发送者的ID

//    switch(G_ST_Student_Interactive_Header_Data.sender_ID) //接收者的ID
//    {
//    case Red_3_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Red_3_Standard_Client;
//        break;
//    case Red_4_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Red_4_Standard_Client;
//        break;
//	case Red_5_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Red_5_Standard_Client;
//        break;
//	case Blue_3_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Blue_3_Standard_Client;
//        break;
//	case Blue_4_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Blue_4_Standard_Client;
//        break;
//	case Blue_5_Standard:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Blue_5_Standard_Client;
//        break;
//    default:
//        G_ST_Student_Interactive_Header_Data.receiver_ID = Red_3_Standard_Client;
//    }

//    G_ST_Client_Custom_Graphic_Delete.operate_tpye = operateTpye;
//    G_ST_Client_Custom_Graphic_Delete.layer = layer;

//    Custom_DataBuf[0] = 0xA5;                           //帧头
//    Custom_DataBuf[1] = 8;                              //数据长度低8位
//    Custom_DataBuf[2] = 0;                              //数据长度高8位
//    Custom_DataBuf[3] = 0x00;                           //Seq
//    Append_CRC8_Check_Sum(Custom_DataBuf, 5);           //CRC8
//    Custom_DataBuf[5] = 0x01;                           //cmd_id低八位
//    Custom_DataBuf[6] = 0x03;                           //cmd_id高八位
//    memcpy(&Custom_DataBuf[7], &G_ST_Student_Interactive_Header_Data, 6); //Header
//    memcpy(&Custom_DataBuf[13], &G_ST_Client_Custom_Graphic_Delete, 2);   //数据
//    Append_CRC16_Check_Sum(Custom_DataBuf, 8 + 9);       //CRC16

//    //	while(DMA_GetCurrDataCounter(UART5_TX_STREAM));           //等之前的发完
//    DMA_ClearITPendingBit(USART5_TX_STREAM, DMA_IT_TCIF7);      //开启DMA_Mode_Normal,即便没有使用完成中断也要软件清除，否则只发一次
//    DMA_Cmd(USART5_TX_STREAM, DISABLE);                         //设置当前计数值前先禁用DMA
//    USART5_TX_STREAM->M0AR = (uint32_t)&Custom_DataBuf;         //设置当前待发数据基地址:Memory0 tARget
//    USART5_TX_STREAM->NDTR = (uint32_t)(8 + 9);                 //设置当前待发的数据的数量:Number of Data units to be TRansferred
//    DMA_Cmd(USART5_TX_STREAM, ENABLE);                          //启用串口DMA发送
}
void pictureOperation_float(graphic_data_struct_t* pGraphicData, u8* graphicName, u32 operateTpye, u32 graphicTpye,
                      u32 layer, u32 color, u32 startAngle, u32 endAngle, u32 width, u32 startX, u32 startY,
                      float FP_Num)
{
	if((pGraphicData == NULL) || (graphicName == NULL))
    {
        return;
    }

    memcpy(pGraphicData->graphic_name, graphicName, 3); //图形名
    pGraphicData->operate_tpye = operateTpye; //图形操作
    pGraphicData->graphic_tpye = graphicTpye; //图形类型
    pGraphicData->layer = layer; //图层数，0-9
    pGraphicData->color = color; //颜色
    pGraphicData->start_angle = startAngle; //起始角度，单位：°，范围[0,360]
    pGraphicData->end_angle = endAngle; //终止角度，单位：°，范围[0,360]
    pGraphicData->width = width; //线宽
    pGraphicData->start_x = startX; //起点 x 坐标
    pGraphicData->start_y = startY; //起点 y 坐标
    pGraphicData->radius = *(u32*)&(FP_Num); //字体大小或者半径
    pGraphicData->end_x = (FP_Num); //终点 x 坐标
    pGraphicData->end_y = (FP_Num); //终点 x 坐标
}
/*------------------------------------------------------------------------
函 数 名：u8 Armor_Angle_Judgment(void)
函数功能：判断伤害来源方向
------------------------------------------------------------------------*/
float attackAngle;//云台坐标系下受到伤害角度
u8 Armor_Angle_Judgment(void) //YAW 1:1
{
//    s16 armorAngle;//底盘坐标系下受到伤害角度
//    u8 attackDirection;

//    switch(G_ST_Robot_Hurt.armor_id)
//    {
//    case 0:
//        armorAngle = 0;          //前
//        break;
//    case 1:
//        armorAngle = 90;         //左
//        break;
//    case 2:
//        armorAngle = 180;        //后
//        break;
//    case 3:
//        armorAngle = 270;        //右
//        break;
//    default:
//        armorAngle = 0;
//        break;
//    }
//    attackAngle = Angle_Inf_To_180(armorAngle - Benjamin_Position);

//    if(attackAngle<45.0f && attackAngle >= -45.0f)         //前方受敌
//    {
//        attackDirection = 0;
//    }
//    else if(attackAngle<135.0f && attackAngle >= 45.0f)    //左方受敌
//    {
//        attackDirection = 1;
//    }
//    else if(attackAngle<-135.0f || attackAngle >= 135.0f)  //后方受敌
//    {
//        attackDirection = 2;
//    }
//    else                                                   //右方受敌
//    {
//        attackDirection = 3;
//    }
//    return attackDirection;
return 0;
}

/*------------------------------------------------------------------------
函 数 名：void drawPictureOnClient(void)
函数功能：在客户端上绘图
------------------------------------------------------------------------*/
void drawPictureOnClient(void)
{
//    static u8	IfFirstDraw = 1;
//	static bool Clinet_Link;

//	if(RSYS_Monitor.GameStatus_fps != 0) //裁判系统是否连接到服务器
//	Clinet_Link = 1;
//	
//   if(system_monitor.UART5_fps >= 10) //判断裁判系统帧率是否正常
//    {
//		my_watch=1;
//		if(IfFirstDraw==1)//)
//        {
//			/******超级电容图层******/
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[3], floatDataName[1], 1, 0, 7,  6, 0, 0, 
//							  /*线宽*/30, /*起点坐标X*/520, /*起点坐标Y*/135 , 0,/*终点坐标X*/1400,/*终点坐标Y*/135);	
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[4], floatDataName[2], 1, 0, 7,  Client_Sendata_Coe.Cap_Color, 0, 0, 
//							  /*线宽*/30, /*起点坐标X*/520, /*起点坐标Y*/135 , 0,/*终点坐标X*/520+Client_Sendata_Coe.Cap_Length,/*终点坐标Y*/135);
//			/******体积碰撞图层******/
//			/*从左下开始，逆时针画出各条直线
//			由于体积碰撞边界的实际模型未知，用实际测量加线性插值的方法逼近这一模型*/
//			//线性插值并根据角度算出横纵基准
//			Client_Sendata_Coe.Volume_X =  Client_Sendata_Coe.Volume_X0 + g_stGM_PitchPosPID.fpFB*(Client_Sendata_Coe.Volume_X1-Client_Sendata_Coe.Volume_X0)/Client_Sendata_Coe.Volume_Angle_Inc;
//			Client_Sendata_Coe.Volume_Y =  Client_Sendata_Coe.Volume_Y0 + g_stGM_PitchPosPID.fpFB*(Client_Sendata_Coe.Volume_Y1-Client_Sendata_Coe.Volume_Y0)/Client_Sendata_Coe.Volume_Angle_Inc;

//			//从左下开始，逆时针画出各条直线
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[0], floatDataName[1], 1, 0, 1, 0, 0, 0, 
//							  /*线宽*/10, /*起点坐标X*/(960-Client_Sendata_Coe.Volume_X), /*起点坐标Y*/0 , 0,
//										  /*终点坐标X*/(960-(Client_Sendata_Coe.Volume_X-Client_Sendata_Coe.Volume_K*Client_Sendata_Coe.Volume_Y)),/*终点坐标Y*/Client_Sendata_Coe.Volume_Y);
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[2], floatDataName[3], 1, 0, 1, 0, 0, 0, 
//							  /*线宽*/10, /*起点坐标X*/(960+(Client_Sendata_Coe.Volume_X-Client_Sendata_Coe.Volume_K*Client_Sendata_Coe.Volume_Y)), /*起点坐标Y*/Client_Sendata_Coe.Volume_Y , 0,
//										  /*终点坐标X*/(960+Client_Sendata_Coe.Volume_X),/*终点坐标Y*/0);
//			/******剩余弹量图层******/
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[5], floatDataName[1], 0, 0, 9, 0, 0, 0, 
//							  /*线宽*/1, /*起点坐标X*/1500, /*起点坐标Y*/150 , 0,
//										   /*终点坐标X*/1700, /*终点坐标Y*/150);
////			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[5], floatDataName[1], 1, 6, 9, 8, 
////							/*字体大小*/500, 0,/*线条宽度*/100, /*起点X坐标*/0, /*起点Y坐标*/100 , 0,0,0);
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[6], floatDataName[2], 0, 0, 9, 0, 0, 0, 
//							  /*线宽*/1, /*起点坐标X*/1500, /*起点坐标Y*/50 , 0,
//										   /*终点坐标X*/1700, /*终点坐标Y*/50);
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[1], Speed_Char, 1, 5, 9, en_White, 
//										  /*字体大小*/30, /*小数位数*/2, 
//										  /*线宽*/3, /*起点坐标X*/1280,
//													 /*起点坐标Y*/500, 0,
//													 /*终点坐标X*/0,
//													 /*终点坐标Y*/0);										   
//			sendPictureToClient(&G_ST_Client_Custom_Graphic_Seven, 7);
//            if(Clinet_Link==1)IfFirstDraw --;
//        }
//        else
//        {
//			/******超级电容图层:7(层数越高越容易显示)******/
//			{/*超级电容画面编写*/
//				Cap_State_Judge_DueTo_Vol();		//判断颜色理应对应的状态
//				if(CAP_STATE==CAN_FLY) Client_Sendata_Coe.Cap_Color = en_Pink;
//				if(CAP_STATE==CAN_GO) Client_Sendata_Coe.Cap_Color = en_Yellow;					
//				if(CAP_STATE==NOT_GO) 											//没电情况产生反向黑色条
//				{
//					Client_Sendata_Coe.Cap_Color = en_Black;
//					Client_Sendata_Coe.Cap_Energy = 2048.0f-200.0f;
//				}
//				Client_Sendata_Coe.Cap_Energy = (capacitor_msg.CAP_Vol*capacitor_msg.CAP_Vol-10.0f*10.0f)*2048/(32.0f*32.0f);//计算可用能量，认为总能量2048J，最大电压32.0V,	停止电压10.0V
//				Client_Sendata_Coe.Cap_Length = Client_Sendata_Coe.Cap_Energy/(2048.0f-200.0f)*Cap_Client_Length;			//可用能量/总可用能量*总长
//			}
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[3], floatDataName[1], 2, 0, 7,  6, 0, 0, 
//							  /*线宽*/30, /*起点坐标X*/530, /*起点坐标Y*/135 , 0,/*终点坐标X*/1410,/*终点坐标Y*/135);	
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[4], floatDataName[2], 2, 0, 7,  Client_Sendata_Coe.Cap_Color, 0, 0, 
//							  /*线宽*/30, /*起点坐标X*/530, /*起点坐标Y*/135 , 0,/*终点坐标X*/530+Client_Sendata_Coe.Cap_Length,/*终点坐标Y*/135);
//			/******体积碰撞图层:1******/
//			/*从左下开始，逆时针画出各条直线
//			由于体积碰撞边界的实际模型未知，用实际测量加线性插值的方法逼近这一模型*/
//			//根据是否小陀螺判断体积碰撞的横纵基准
//			if(Chassis_Turn_Mode == Top) 					
//			{
//				Client_Sendata_Coe.Volume_X0 = Client_VolumeX_Top_0;
//				Client_Sendata_Coe.Volume_X1 = Client_VolumeX_Top_20;
//				Client_Sendata_Coe.Volume_Y0 = Client_VolumeY_Top_0;
//				Client_Sendata_Coe.Volume_Y1 = Client_VolumeY_Top_20;
//				Client_Sendata_Coe.Volume_K	 = Client_VolumeX_Koffset_Top;
//				Client_Sendata_Coe.Volume_Angle_Inc = 20;
//			}
//			else
//			{
//				Client_Sendata_Coe.Volume_X0 = Client_VolumeX_Normal_0;
//				Client_Sendata_Coe.Volume_X1 = Client_VolumeX_Normal_24;
//				Client_Sendata_Coe.Volume_Y0 = Client_VolumeY_Normal_0;
//				Client_Sendata_Coe.Volume_Y1 = Client_VolumeY_Normal_24;
//				Client_Sendata_Coe.Volume_K	 = Client_VolumeX_Koffset_Normal;
//				Client_Sendata_Coe.Volume_Angle_Inc = 24;
//			}
//			//线性插值并根据角度算出横纵基准
//			Client_Sendata_Coe.Volume_X =  Client_Sendata_Coe.Volume_X0 + g_stGM_PitchPosPID.fpFB*(Client_Sendata_Coe.Volume_X1-Client_Sendata_Coe.Volume_X0)/Client_Sendata_Coe.Volume_Angle_Inc;
//			Client_Sendata_Coe.Volume_Y =  Client_Sendata_Coe.Volume_Y0 + g_stGM_PitchPosPID.fpFB*(Client_Sendata_Coe.Volume_Y1-Client_Sendata_Coe.Volume_Y0)/Client_Sendata_Coe.Volume_Angle_Inc;
//			
//			//从左下开始，逆时针画出各条直线
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[0], floatDataName[1], 2, 0, 1, 0, 0, 0, 
//							  /*线宽*/10, /*起点坐标X*/(960-Client_Sendata_Coe.Volume_X), /*起点坐标Y*/0 , 0,
//										  /*终点坐标X*/(960-(Client_Sendata_Coe.Volume_X-Client_Sendata_Coe.Volume_K*Client_Sendata_Coe.Volume_Y)),/*终点坐标Y*/Client_Sendata_Coe.Volume_Y);
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[2], floatDataName[3], 2, 0, 1, 0, 0, 0, 
//							  /*线宽*/10, /*起点坐标X*/(960+(Client_Sendata_Coe.Volume_X-Client_Sendata_Coe.Volume_K*Client_Sendata_Coe.Volume_Y)), /*起点坐标Y*/Client_Sendata_Coe.Volume_Y , 0,
//										  /*终点坐标X*/(960+Client_Sendata_Coe.Volume_X),/*终点坐标Y*/0);
//	
//			/******剩余弹量图层:6******/
//			//子弹数统计在其他地方完成，可以直接搜索查看
//			if(Client_Sendata_Coe.Bullet_Num>=40)Client_Bullet_240=100;
//			else if(Client_Sendata_Coe.Bullet_Num>=20)Client_Bullet_240=50;
//			else Client_Bullet_240 = 0;
//			if(Client_Sendata_Coe.Bullet_Num>=30)Client_Bullet_130=100;
//			else if(Client_Sendata_Coe.Bullet_Num>=10)Client_Bullet_130=50;
//			else Client_Bullet_130 = 0;
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[5], floatDataName[1], 2, 0, 9, 8, 0, 0, 
//							  /*线宽*/1, /*起点坐标X*/use_x, /*起点坐标Y*/0 , 0,
//										   /*终点坐标X*/use_x, /*终点坐标Y*/1080);
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[6], floatDataName[2], 2, 0, 9, 8, 0, 0, 
//							  /*线宽*/1, /*起点坐标X*/530, /*起点坐标Y*/30 , 0,
//										   /*终点坐标X*/530+Client_Bullet_130, /*终点坐标Y*/30);
////			/******车速图层:9******/
//			speed_Chassis = (-g_stCM1_SpeedPID.fpFB+g_stCM2_SpeedPID.fpFB+g_stCM3_SpeedPID.fpFB-g_stCM4_SpeedPID.fpFB)/Mec_Weel_Coe/4*1000;
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[1], Speed_Char, 2, 5, 9, en_White, 
//							  /*字体大小*/20, /*小数位数*/2, 
//							  /*线宽*/2, /*起点坐标X*/1350,
//										 /*起点坐标Y*/400, show_float(speed_Chassis),
//										 /*终点坐标X*/show_float(speed_Chassis)>>10,
//										 /*终点坐标Y*/show_float(speed_Chassis)>>21);		
//			sendPictureToClient(&G_ST_Client_Custom_Graphic_Seven,7);	
//        }
//    }
//    else
//    {
//        deletePictureOnClient(2, 0); //删除所有图形
//        IfFirstDraw = true; //准备重新画图
//    }
}
void drawRuler_OnClient(void)
{
//	static u8	IfFirstDraw = 1;
//	/******Client_Draw_Ruler******/
//	if(IfFirstDraw == 1)
//	{
//		/******主纵轴******/
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[0], AimStar_X[0], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X1+0.2f*(Client_Draw_Ruler.X1-Client_Draw_Ruler.X4),
//									 /*起点坐标Y*/Client_Draw_Ruler.Y1+0.2f*(Client_Draw_Ruler.Y1-Client_Draw_Ruler.Y4),0,
//									 /*终点坐标X*/Client_Draw_Ruler.X4+0.2f*(Client_Draw_Ruler.X4-Client_Draw_Ruler.X1),
//									 /*终点坐标Y*/Client_Draw_Ruler.Y4+0.2f*(Client_Draw_Ruler.Y4-Client_Draw_Ruler.Y1));	
//		/******子轴******/
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[1], AimStar_X[1], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X1-50,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y1,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X1+50,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y1);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[2], AimStar_X[2], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X2-40,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y2,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X2+40,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y2);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[3], AimStar_X[3], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X3-30,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y3,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X3+30,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y3);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[4], AimStar_X[4], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X4-20,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y4,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X4+20,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y4);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[5], AimStar_X[5], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/0.5f*(Client_Draw_Ruler.X2+Client_Draw_Ruler.X3)-35,
//									 /*起点坐标Y*/0.5f*(Client_Draw_Ruler.Y2+Client_Draw_Ruler.Y3),0,
//									 /*终点坐标X*/0.5f*(Client_Draw_Ruler.X2+Client_Draw_Ruler.X3)+35,
//									 /*终点坐标Y*/0.5f*(Client_Draw_Ruler.Y2+Client_Draw_Ruler.Y3));	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[6], AimStar_X[6], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/0.5f*(Client_Draw_Ruler.X4+Client_Draw_Ruler.X3)-25,
//									 /*起点坐标Y*/0.5f*(Client_Draw_Ruler.Y4+Client_Draw_Ruler.Y3),0,
//									 /*终点坐标X*/0.5f*(Client_Draw_Ruler.X4+Client_Draw_Ruler.X3)+25,
//									 /*终点坐标Y*/0.5f*(Client_Draw_Ruler.Y4+Client_Draw_Ruler.Y3));		
//    	 if(RSYS_Monitor.GameStatus_fps != 0) IfFirstDraw = 0;
//	}
//	else 	
//	{
//		/******主纵轴******/
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[0], AimStar_X[0], 2, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X1+0.2f*(Client_Draw_Ruler.X1-Client_Draw_Ruler.X4),
//									 /*起点坐标Y*/Client_Draw_Ruler.Y1+0.2f*(Client_Draw_Ruler.Y1-Client_Draw_Ruler.Y4),0,
//									 /*终点坐标X*/Client_Draw_Ruler.X4+0.2f*(Client_Draw_Ruler.X4-Client_Draw_Ruler.X1),
//									 /*终点坐标Y*/Client_Draw_Ruler.Y4+0.2f*(Client_Draw_Ruler.Y4-Client_Draw_Ruler.Y1));	
//		/******子轴******/
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[1], AimStar_X[1], 2, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X1-50,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y1,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X1+50,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y1);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[2], AimStar_X[2], 2, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X2-40,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y2,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X2+40,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y2);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[3], AimStar_X[3], 2, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X3-30,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y3,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X3+30,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y3);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[4], AimStar_X[4], 2, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X4-20,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y4,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X4+20,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y4);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[5], AimStar_X[5], 2, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/0.5f*(Client_Draw_Ruler.X2+Client_Draw_Ruler.X3)-35,
//									 /*起点坐标Y*/0.5f*(Client_Draw_Ruler.Y2+Client_Draw_Ruler.Y3),0,
//									 /*终点坐标X*/0.5f*(Client_Draw_Ruler.X2+Client_Draw_Ruler.X3)+35,
//									 /*终点坐标Y*/0.5f*(Client_Draw_Ruler.Y2+Client_Draw_Ruler.Y3));	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[6], AimStar_X[6], 2, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/0.5f*(Client_Draw_Ruler.X4+Client_Draw_Ruler.X3)-25,
//									 /*起点坐标Y*/0.5f*(Client_Draw_Ruler.Y4+Client_Draw_Ruler.Y3),0,
//									 /*终点坐标X*/0.5f*(Client_Draw_Ruler.X4+Client_Draw_Ruler.X3)+25,
//									 /*终点坐标Y*/0.5f*(Client_Draw_Ruler.Y4+Client_Draw_Ruler.Y3));	
//}		
//	sendPictureToClient(&G_ST_Client_Custom_Graphic_Seven, 7);
}
/*------------------------------------------------------------------------
函 数 名：void Send_Char_To_Client(void)
函数功能：给操作手界面发字符
------------------------------------------------------------------------*/
extern bool Is_Tank;
extern bool ifShoot;
u8 Send_Char[30] = " T:   C:   V:   P:   S:   O: ";
void Send_Char_To_Client(void)
{
//	static bool FirstDraw = 1;
//	int i;
//	while(i<=29)
//	{
//		G_ST_Client_Custom_Character.data[i] = Send_Char[i];
//		i++;
//	}
//	//电容，可用为1 3 8 13 18 23
//	if(Is_Tank) G_ST_Client_Custom_Character.data[3] = 'T';
//	else G_ST_Client_Custom_Character.data[3] = 'F';
//	if(system_monitor.CAN_Rx_Capatitor_fps<50) G_ST_Client_Custom_Character.data[8] = 'x';
//	else G_ST_Client_Custom_Character.data[8] = 'y';
//	//视觉，可用为1
//	if(system_monitor.USART6_fps<270) G_ST_Client_Custom_Character.data[13] = 'x';
//	else G_ST_Client_Custom_Character.data[13] = 'y';
//	//弹仓盖，关上为1
//	if(ReloadState == FALSE) G_ST_Client_Custom_Character.data[18] = 'y';
//	else G_ST_Client_Custom_Character.data[18] = 'O';
//	//堵转
//	if(ifShoot==1)G_ST_Client_Custom_Character.data[23] = 'Y';
//	else G_ST_Client_Custom_Character.data[23] = 'X';
//	//其他，正常为1
//	if(system_monitor.CAN1_fps>=3500&&(system_monitor.CAN2_fps>=5500)&&system_monitor.USART2_fps>=900)
//	G_ST_Client_Custom_Character.data[28] = 'y';
//	else G_ST_Client_Custom_Character.data[28] = 'x';
//	if(FirstDraw == 1)
//	{
//		pictureOperation(&G_ST_Client_Custom_Character.grapic_data_struct, floatDataName[0], 1, 7, 6, en_Green,
//						 /*字体大小*/20, /*字符长度*/30, /*线宽*/4, /*起点坐标X*/1300, /*起点坐标Y*/900 , 0,0,0);	
//		if(RSYS_Monitor.GameStatus_fps != 0) FirstDraw = 0;
//	}
//	else 		pictureOperation(&G_ST_Client_Custom_Character.grapic_data_struct, floatDataName[0], 2, 7, 6, en_Green,
//						 /*字体大小*/20, /*字符长度*/30, /*线宽*/4, /*起点坐标X*/1300, /*起点坐标Y*/900 , 0,0,0);	
//	sendPictureToClient(&G_ST_Client_Custom_Character, 3);
}
void New_Client(void)
{
//	static u8 New_Client_Cnt;
//	switch(New_Client_Cnt)
//	{
//		case 0:
//			/******超级电容图层******/
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[3], floatDataName[1], 1, 0, 7,  6, 0, 0, 
//							  /*线宽*/30, /*起点坐标X*/520, /*起点坐标Y*/135 , 0,/*终点坐标X*/1400,/*终点坐标Y*/135);	
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[4], floatDataName[2], 1, 0, 7,  Client_Sendata_Coe.Cap_Color, 0, 0, 
//							  /*线宽*/30, /*起点坐标X*/520, /*起点坐标Y*/135 , 0,/*终点坐标X*/520+Client_Sendata_Coe.Cap_Length,/*终点坐标Y*/135);
//			/******体积碰撞图层******/
//			/*从左下开始，逆时针画出各条直线
//			由于体积碰撞边界的实际模型未知，用实际测量加线性插值的方法逼近这一模型*/
//			//线性插值并根据角度算出横纵基准
//			Client_Sendata_Coe.Volume_X =  Client_Sendata_Coe.Volume_X0 + g_stGM_PitchPosPID.fpFB*(Client_Sendata_Coe.Volume_X1-Client_Sendata_Coe.Volume_X0)/Client_Sendata_Coe.Volume_Angle_Inc;
//			Client_Sendata_Coe.Volume_Y =  Client_Sendata_Coe.Volume_Y0 + g_stGM_PitchPosPID.fpFB*(Client_Sendata_Coe.Volume_Y1-Client_Sendata_Coe.Volume_Y0)/Client_Sendata_Coe.Volume_Angle_Inc;

//			//从左下开始，逆时针画出各条直线
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[0], floatDataName[1], 1, 0, 1, 0, 0, 0, 
//							  /*线宽*/10, /*起点坐标X*/(960-Client_Sendata_Coe.Volume_X), /*起点坐标Y*/0 , 0,
//										  /*终点坐标X*/(960-(Client_Sendata_Coe.Volume_X-Client_Sendata_Coe.Volume_K*Client_Sendata_Coe.Volume_Y)),/*终点坐标Y*/Client_Sendata_Coe.Volume_Y);
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[2], floatDataName[3], 1, 0, 1, 0, 0, 0, 
//							  /*线宽*/10, /*起点坐标X*/(960+(Client_Sendata_Coe.Volume_X-Client_Sendata_Coe.Volume_K*Client_Sendata_Coe.Volume_Y)), /*起点坐标Y*/Client_Sendata_Coe.Volume_Y , 0,
//										  /*终点坐标X*/(960+Client_Sendata_Coe.Volume_X),/*终点坐标Y*/0);
//			/******剩余弹量图层******/
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[5], floatDataName[1], 0, 0, 9, 0, 0, 0, 
//							  /*线宽*/20, /*起点坐标X*/1500, /*起点坐标Y*/150 , 0,
//										   /*终点坐标X*/1700, /*终点坐标Y*/150);
////			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[5], floatDataName[1], 1, 6, 9, 8, 
////							/*字体大小*/500, 0,/*线条宽度*/100, /*起点X坐标*/0, /*起点Y坐标*/100 , 0,0,0);
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[6], floatDataName[2], 0, 0, 9, 0, 0, 0, 
//							  /*线宽*/20, /*起点坐标X*/1500, /*起点坐标Y*/50 , 0,
//										   /*终点坐标X*/1700, /*终点坐标Y*/50);
//			pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[1], Speed_Char, 1, 5, 9, en_White, 
//										  /*字体大小*/30, /*小数位数*/2, 
//										  /*线宽*/3, /*起点坐标X*/1280,
//													 /*起点坐标Y*/500, 0,
//													 /*终点坐标X*/0,
//													 /*终点坐标Y*/0);
//			sendPictureToClient(&G_ST_Client_Custom_Graphic_Seven, 7);
//			break;
//		case 1:
//		/******主纵轴******/
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[0], AimStar_X[0], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X1+0.2f*(Client_Draw_Ruler.X1-Client_Draw_Ruler.X4),
//									 /*起点坐标Y*/Client_Draw_Ruler.Y1+0.2f*(Client_Draw_Ruler.Y1-Client_Draw_Ruler.Y4),0,
//									 /*终点坐标X*/Client_Draw_Ruler.X4+0.2f*(Client_Draw_Ruler.X4-Client_Draw_Ruler.X1),
//									 /*终点坐标Y*/Client_Draw_Ruler.Y4+0.2f*(Client_Draw_Ruler.Y4-Client_Draw_Ruler.Y1));	
//		/******子轴******/
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[1], AimStar_X[1], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X1-50,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y1,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X1+50,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y1);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[2], AimStar_X[2], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X2-40,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y2,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X2+40,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y2);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[3], AimStar_X[3], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X3-30,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y3,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X3+30,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y3);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[4], AimStar_X[4], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/Client_Draw_Ruler.X4-20,
//									 /*起点坐标Y*/Client_Draw_Ruler.Y4,0,
//									 /*终点坐标X*/Client_Draw_Ruler.X4+20,
//									 /*终点坐标Y*/Client_Draw_Ruler.Y4);	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[5], AimStar_X[5], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/0.5f*(Client_Draw_Ruler.X2+Client_Draw_Ruler.X3)-35,
//									 /*起点坐标Y*/0.5f*(Client_Draw_Ruler.Y2+Client_Draw_Ruler.Y3),0,
//									 /*终点坐标X*/0.5f*(Client_Draw_Ruler.X2+Client_Draw_Ruler.X3)+35,
//									 /*终点坐标Y*/0.5f*(Client_Draw_Ruler.Y2+Client_Draw_Ruler.Y3));	
//		pictureOperation(&G_ST_Client_Custom_Graphic_Seven.grapic_data_struct[6], AimStar_X[6], 1, 0, 8,  en_White, 0, 0, 
//						  /*线宽*/2, /*起点坐标X*/0.5f*(Client_Draw_Ruler.X4+Client_Draw_Ruler.X3)-25,
//									 /*起点坐标Y*/0.5f*(Client_Draw_Ruler.Y4+Client_Draw_Ruler.Y3),0,
//									 /*终点坐标X*/0.5f*(Client_Draw_Ruler.X4+Client_Draw_Ruler.X3)+25,
//									 /*终点坐标Y*/0.5f*(Client_Draw_Ruler.Y4+Client_Draw_Ruler.Y3));	
//		sendPictureToClient(&G_ST_Client_Custom_Graphic_Seven, 7);
//			break;
//		case 2:
//		pictureOperation(&G_ST_Client_Custom_Character.grapic_data_struct, floatDataName[0], 1, 7, 6, en_Green,
//			 /*字体大小*/20, /*字符长度*/30, /*线宽*/4, /*起点坐标X*/1300, /*起点坐标Y*/900 , 0,0,0);	
//		sendPictureToClient(&G_ST_Client_Custom_Character, 3);
//			break;
//	}
//	New_Client_Cnt++;
//	if(New_Client_Cnt==3)New_Client_Cnt=0;
	
}
/*------------------------------------------------------------------------
函 数 名：void Send_Char_To_Client(void)
函数功能：给操作手界面发电容警告
------------------------------------------------------------------------*/
u8 Send_Warning[30] = " CAP WARNING!!!              ";
u8 Send_[30] = "                             ";

extern u8 Send_Cap_Warning;
void Send_Warning_To_Client(void)
{
//	static bool FirstDraw = 1;
//	int i;
//	if(Data_to_Ariel.CAP_Energy<200&&Send_Cap_Warning!=0)
//	{
//		while(i<=29)
//		{
//			G_ST_Client_Custom_Character.data[i] = Send_Warning[i];
//			i++;
//		}
//	}
//	else 
//	{
//		while(i<=29)
//		{
//			G_ST_Client_Custom_Character.data[i] = ' ';
//			i++;
//		}
//	}
//	if(FirstDraw == 1)
//	{
//		pictureOperation(&G_ST_Client_Custom_Character.grapic_data_struct,floatDataName[1], 1, 7, 6, 7,
//						 /*字体大小*/80, /*字符长度*/30, /*线宽*/8, /*起点坐标X*/400, /*起点坐标Y*/740 , 0,0,0);	
//		if(RSYS_Monitor.GameStatus_fps != 0) FirstDraw = 0;
//	}
//	else 		pictureOperation(&G_ST_Client_Custom_Character.grapic_data_struct, floatDataName[1], 2, 7, 6, 7,
//						 /*字体大小*/80, /*字符长度*/30, /*线宽*/8, /*起点坐标X*/400, /*起点坐标Y*/740 , 0,0,0);	
//	sendPictureToClient(&G_ST_Client_Custom_Character, 3);
}
/*------------------------------------------------------------------------
函 数 名：void prepareAttackedDirectionPicture(void)
函数功能：更新受伤害方向的图像信息
------------------------------------------------------------------------*/
//u32 attackedTime[4] = {0};//四个装甲板最近一次被攻击时间
//void prepareAttackedDirectionPicture(void)
//{
//    for(u8 i = 0; i < 4; i++)
//    {
//        if( attackedTime[i] - TIM5->CNT < 1000)
//        {
//            //若1s内受到攻击,相应方向图形为紫红色
//            pictureOperation(&G_ST_Client_Custom_Graphic_Five.grapic_data_struct[i], Enemy_Senor[i], 2, 4, 1, 4,
//                             Angle_Inf_To_180(60 + 90*i), Angle_Inf_To_180(120 + 90*i), 2, 540, 960, 0, 600, 600);
//        }
//        else
//        {
//            //若1s内未受到攻击,相应方向图形为白色
//            pictureOperation(&G_ST_Client_Custom_Graphic_Five.grapic_data_struct[i], Enemy_Senor[i], 2, 4, 1, 8,
//                             Angle_Inf_To_180(60 + 90*i), Angle_Inf_To_180(120 + 90*i), 2, 540, 960, 0, 600, 600);
//        }
//    }
//}



