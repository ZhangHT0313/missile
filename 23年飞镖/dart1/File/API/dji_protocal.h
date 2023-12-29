#ifndef __DJI_PROTOCAL_H__
#define __DJI_PROTOCAL_H__

#include "string.h"
//#include "stdlib.h"
#include "stdio.h"
#include "stm32f4xx.h"
#include "algorithm.h"
#include "hitcrt_types.h"
#include "math.h"
#include "verify.h"

#define Volt_mes            ((float)G_ST_Power_Heat_Data.chassis_volt/1000.0f)        //��ѹ
#define Current_mes         ((float)G_ST_Power_Heat_Data.chassis_current/1000.0f)     //����
#define PowerBuffer_mes     ((float)G_ST_Power_Heat_Data.chassis_power_buffer)        //���ʻ���
#define Power_mes           ((float)G_ST_Power_Heat_Data.chassis_power)               //����
#define ShooterHeat_mes     ((s32)G_ST_Power_Heat_Data.shooter_id1_17mm_cooling_heat)    //ǹ������
#define ShooterHeat_Rate    ((float)G_ST_Game_Robot_Status.shooter_id1_17mm_cooling_rate)//ǹ��������ȴ
#define ShooterHeat_Limit   ((s32)G_ST_Game_Robot_Status.shooter_id1_17mm_cooling_limit) //ǹ����������
#define ShooterSpeed_Limit  ((u8)G_ST_Game_Robot_Status.shooter_id1_17mm_speed_limit)    //ǹ����������
#define HP_Left_mes         ((u16)G_ST_Game_Robot_Status.remain_HP)                   //������ʣ��Ѫ��
#define RobotLevel_mes      ((u8)G_ST_Game_Robot_Status.robot_level)                  //�����˵ȼ�
#define HurtType_mes        ((u8)G_ST_Robot_Hurt.hurt_type)							  //Ѫ���仯����
#define PelletFreq_mes      ((u8)G_ST_Shoot_Data.bullet_freq)                         //��Ƶ
#define PelletSpeed_mes     ((float)G_ST_Shoot_Data.bullet_speed)                     //����
#define MY_ID               ((u8)G_ST_Game_Robot_Status.robot_id)                     //������ID
//#define MY_ID               ((u8)G_ST_Game_Robot_Status.robot_id)                     //������ID
//#define Bufftype_mes       //��¼Buff����
//#define BuffAddition_mes   //��¼Buff�ӳɰٷֱ�


#define RSYS_RX_FREE             0
#define RSYS_RX_Length           1
#define RSYS_RX_Num 		     2
#define RSYS_RX_CRC8             3
#define RSYS_RX_CmdID            4
#define RSYS_RX_Data             5
#define RSYS_RX_CRC16            6

#define G_ST_Game_Robot_Status_INIT(robot_id,robot_level) \
        {robot_id,robot_level,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define G_ST_Power_Heat_Data_INIT(chassis_power,chassis_power_buffer,shooter_heat0) \
        {0,0,chassis_power,chassis_power_buffer,shooter_heat0,0,0}
#define G_ST_Shoot_Data_INIT(bullet_freq,bullet_speed) \
        {0,0,bullet_freq,bullet_speed};

//�Զ�������
typedef struct
{
    u16 total_fps;
    u8 GameStatus_fps;
    u8 GameResult_fps;
    u8 GameRobotHP_fps;
    u8 DartStatus_fps;
    u8 EventData_fps;
    u8 SupplyProjectileAction_fps;
    u8 RefereeWarning_fps;
    u8 DartRemainingTime_fps;
    u8 GameRobotStatus_fps;
    u8 PowerHeatData_fps;
    u8 GameRobotPos_fps;
    u8 Buff_fps;
    u8 AerialRobotEnergy_fps;
    u8 RobotHurt_fps;
    u8 ShootData_fps;
    u8 BulletRemaining_fps;
    u8 RFIDStatus_fps;
    u8 DartClientCmd_fps;
    u8 RobotInteractive_fps;

    u16 total_cnt;
    u8 GameStatus_cnt;
    u8 GameResult_cnt;
    u8 GameRobotHP_cnt;
    u8 DartStatus_cnt;
    u8 EventData_cnt;
    u8 SupplyProjectileAction_cnt;
    u8 RefereeWarning_cnt;
    u8 DartRemainingTime_cnt;
    u8 GameRobotStatus_cnt;
    u8 PowerHeatData_cnt;
    u8 GameRobotPos_cnt;
    u8 Buff_cnt;
    u8 AerialRobotEnergy_cnt;
    u8 RobotHurt_cnt;
    u8 ShootData_cnt;
    u8 BulletRemaining_cnt;
    u8 RFIDStatus_cnt;
    u8 DartClientCmd_cnt;
    u8 RobotInteractive_cnt;
} ST_RSYS_MONITOR;

//����ϵͳ����ID
typedef enum
{
    GameStatus_ID               = 0x0001,  //����״̬��1Hz���ڷ���
    GameResult_ID               = 0x0002,  //���������������������
    GameRobotHP_ID              = 0x0003,  //������Ѫ�����ݣ�1Hz���ڷ���
    DartStatus_ID               = 0x0004,  //���ڷ���״̬�����ڷ���ʱ����
    ICRABuffDebuffZoneStatus_ID = 0x0005,  //�˹�������ս���ӳ���ͷ���״̬��1Hz���ڷ���
    EventData_ID                = 0x0101,  //�����¼����ݣ�1Hz���ڷ���
    SupplyProjectileAction_ID   = 0x0102,  //���ز���վ������ʶ���ݣ�������������
    RefereeWarning_ID           = 0x0104,  //���о������ݣ����淢������
    DartRemainingTime_ID        = 0x0105,  //���ڷ���ڵ���ʱ��1Hz���ڷ���
    GameRobotStatus_ID          = 0x0201,  //������״̬���ݣ�10Hz���ڷ���
    PowerHeatData_ID            = 0x0202,  //ʵʱ�����������ݣ�50Hz���ڷ���
    GameRobotPos_ID             = 0x0203,  //������λ�����ݣ�10Hz����
    Buff_ID                     = 0x0204,  //�������������ݣ�1Hz���ڷ���
    AerialRobotEnergy_ID        = 0x0205,  //���л���������״̬���ݣ�10Hz���ڷ��ͣ�ֻ�п��л��������ط���
    RobotHurt_ID                = 0x0206,  //�˺�״̬���ݣ��˺���������
    ShootData_ID                = 0x0207,  //ʵʱ������ݣ��ӵ��������
    BulletRemaining_ID          = 0x0208,  //����ʣ�෢�����������л����ˣ��ڱ��������Լ�ICRA�����˷��ͣ�1Hz���ڷ���
    RFIDStatus_ID               = 0x0209,  //������RFID״̬��1Hz���ڷ���
    DartClientCmd_ID            = 0x020A,  //���ڻ����˿ͻ���ָ�����ݣ�10Hz���ڷ���
    Robot_Interactive_ID        = 0x0301,  //�����˼佻�����ݣ����ͷ��������ͣ�����10Hz
} EN_RSYS_ID;

//������ID
typedef enum
{
    Red_1_Hero       = 1,
    Red_2_Engineer   = 2,
    Red_3_Standard   = 3,
    Red_4_Standard   = 4,
    Red_5_Standard   = 5,
    Red_6_Aerial     = 6,
    Red_7_Sentry     = 7,
	  Red_8_Dart       = 8,
    Red_9_Radar      = 9,

    Blue_1_Hero     = 101,
    Blue_2_Engineer = 102,
    Blue_3_Standard = 103,
    Blue_4_Standard = 104,
    Blue_5_Standard = 105,
    Blue_6_Aerial   = 106,
    Blue_7_Sentry   = 107,
	  Blue_8_Dart     = 108,
    Blue_9_Radar    = 109,
} EN_Robot_ID;

//�ͻ���ID
typedef enum
{
    Red_1_Hero_Client       = 0x101,
    Red_2_Engineer_Client   = 0x102,
    Red_3_Standard_Client   = 0x103,
    Red_4_Standard_Client   = 0x104,
    Red_5_Standard_Client   = 0x105,
    Red_6_Aerial_Client     = 0x106,

    Blue_1_Hero_Client      = 0x0165,
    Blue_2_Engineer_Client  = 0x0166,
    Blue_3_Standard_Client  = 0x0167,
    Blue_4_Standard_Client  = 0x0168,
    Blue_5_Standard_Client  = 0x0169,
    Blue_6_Aerial_Client    = 0x016A,
} EN_Client_ID;

//1.����״̬���ݣ�0x0001������Ƶ�ʣ�1Hz�����ͷ�Χ�����л����ˡ�
typedef __packed struct {
	uint8_t game_type: 4;
	uint8_t game_progress: 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
} ext_game_status_t;

//2.����������ݣ�0x0002������Ƶ�ʣ������������ͣ����ͷ�Χ�����л����ˡ�
typedef __packed struct {
	uint8_t winner;
} ext_game_result_t;

//3.������Ѫ�����ݣ�0x0003������Ƶ�ʣ�1Hz�����ͷ�Χ�����л����ˡ�
typedef __packed struct {
	uint16_t red_1_robot_HP;
	uint16_t red_2_robot_HP;
	uint16_t red_3_robot_HP;
	uint16_t red_4_robot_HP;
	uint16_t red_5_robot_HP;
	uint16_t red_7_robot_HP;
	uint16_t red_outpost_HP;
	uint16_t red_base_HP;
	uint16_t blue_1_robot_HP;
	uint16_t blue_2_robot_HP;
	uint16_t blue_3_robot_HP;
	uint16_t blue_4_robot_HP;
	uint16_t blue_5_robot_HP;
	uint16_t blue_7_robot_HP;
	uint16_t blue_outpost_HP;
	uint16_t blue_base_HP;
} ext_game_robot_HP_t;

//4.���ڷ���״̬��0x0004������Ƶ�ʣ����ڷ�����ͣ����ͷ�Χ�����л����ˡ�
typedef __packed struct
{
    uint8_t dart_belong;
    uint16_t stage_remaining_time;
} ext_dart_status_t;

//5.�˹�������ս���ӳ���ͷ���״̬��0x0005������Ƶ�ʣ�1Hz���ڷ��ͣ����ͷ�Χ�����л����ˡ�
typedef __packed struct {
	uint8_t F1_zone_status: 1;
	uint8_t F1_zone_buff_debuff_status: 3;
	uint8_t F2_zone_status: 1;
	uint8_t F2_zone_buff_debuff_status: 3;
	uint8_t F3_zone_status: 1;
	uint8_t F3_zone_buff_debuff_status: 3;
	uint8_t F4_zone_status: 1;
	uint8_t F4_zone_buff_debuff_status: 3;
	uint8_t F5_zone_status: 1;
	uint8_t F5_zone_buff_debuff_status: 3;
	uint8_t F6_zone_status: 1;
	uint8_t F6_zone_buff_debuff_status: 3;

	uint16_t red1_bullet_left;
	uint16_t red2_bullet_left;
	uint16_t blue1_bullet_left;
	uint16_t blue2_bullet_left;
} ext_ICRA_buff_debuff_zone_status_t;

//6.�����¼����ݣ�0x0101������Ƶ�ʣ�1Hz ���ڷ��ͣ����ͷ�Χ�����������ˡ�
typedef __packed struct
{
    uint32_t event_type;
} ext_event_data_t;

//7.����վ������ʶ��0x0102������Ƶ�ʣ������������ͣ����ͷ�Χ�����������ˡ�
typedef __packed struct
{
    uint8_t supply_projectile_id;
    uint8_t supply_robot_id;
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//8.���о�����Ϣ��cmd_id(0x0104)������Ƶ�ʣ����淢�����ͣ����ͷ�Χ�����������ˡ�
typedef __packed struct
{
    uint8_t level;
    uint8_t foul_robot_id;
} ext_referee_warning_t;

//9.���ڷ���ڵ���ʱ��cmd_id(0x0105)������Ƶ�ʣ�1Hz���ڷ��ͣ����ͷ�Χ�����������ˡ�
typedef __packed struct
{
    uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;

//10.����������״̬��0x0201������Ƶ�ʣ�10Hz�����ͷ�Χ����һ�����ˡ�
typedef __packed struct {
	uint8_t robot_id;
	uint8_t robot_level;
	uint16_t remain_HP;
	uint16_t max_HP;
	uint16_t shooter_id1_17mm_cooling_rate;
	uint16_t shooter_id1_17mm_cooling_limit;
	uint16_t shooter_id1_17mm_speed_limit;

	uint16_t shooter_id2_17mm_cooling_rate;
	uint16_t shooter_id2_17mm_cooling_limit;
	uint16_t shooter_id2_17mm_speed_limit;

	uint16_t shooter_id1_42mm_cooling_rate;
	uint16_t shooter_id1_42mm_cooling_limit;
	uint16_t shooter_id1_42mm_speed_limit;

	uint16_t chassis_power_limit;
	uint8_t mains_power_gimbal_output: 1;
	uint8_t mains_power_chassis_output: 1;
	uint8_t mains_power_shooter_output: 1;
} ext_game_robot_status_t;

//11.ʵʱ�����������ݣ�0x0202������Ƶ�ʣ�50Hz�����ͷ�Χ����һ�����ˡ�
typedef __packed struct {
	uint16_t chassis_volt;
	uint16_t chassis_current;
	float chassis_power;
	uint16_t chassis_power_buffer;
	uint16_t shooter_id1_17mm_cooling_heat;
	uint16_t shooter_id2_17mm_cooling_heat;
	uint16_t shooter_id1_42mm_cooling_heat;
} ext_power_heat_data_t;

//12.������λ�ã�0x0203������Ƶ�ʣ�10Hz�����ͷ�Χ����һ�����ˡ�
typedef __packed struct
{
    float x;	//λ�� x ���꣬��λ m
    float y;	//λ�� y ���꣬��λ m
    float z;	//λ�� z ���꣬��λ m
    float yaw;//λ��ǹ�ڣ���λ��
} ext_game_robot_pos_t;

//13.���������棺 0x0204������Ƶ�ʣ� 1Hz ���ڷ��ͣ����ͷ�Χ����һ�����ˡ�
typedef __packed struct
{
    uint8_t power_rune_buff;
} ext_buff_t;

//14.���л���������״̬��0x0205������Ƶ�ʣ�10Hz�����ͷ�Χ����һ�����ˡ�
typedef __packed struct {
	uint8_t attack_time;
} aerial_robot_energy_t;

//15.�˺�״̬��0x0206������Ƶ�ʣ��˺��������ͣ����ͷ�Χ����һ�����ˡ�
typedef __packed struct
{
    uint8_t armor_id : 4;
    uint8_t hurt_type : 4;
} ext_robot_hurt_t;

//16.ʵʱ�����Ϣ��0x0207������Ƶ�ʣ�������ͣ����ͷ�Χ����һ�����ˡ�
typedef __packed  struct {
	uint8_t bullet_type;
	uint8_t shooter_id;
	uint8_t bullet_freq;
	float bullet_speed;
} ext_shoot_data_t;

//17.�ӵ�ʣ�෢������0x0208������Ƶ�ʣ�1Hz ���ڷ��ͣ����л����ˣ��ڱ��������Լ� ICRA ���������ط��ͣ����ͷ�Χ����һ�����ˡ�
typedef __packed struct {
	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;
} ext_bullet_remaining_t;

//18.������ RFID ״̬��0x0209������Ƶ�ʣ�1Hz�����ͷ�Χ����һ�����ˡ�
typedef __packed struct
{
    uint32_t rfid_status;
} ext_rfid_status_t;

//19.���ڻ����˿ͻ���ָ�����ݣ�0x020A������Ƶ�ʣ�10Hz�����ͷ�Χ����һ�����ˡ�
typedef __packed struct
{
    uint8_t dart_launch_opening_status;
    uint8_t dart_attack_target;
    uint16_t target_change_time;
//    uint8_t first_dart_speed;
//    uint8_t second_dart_speed;
//    uint8_t third_dart_speed;
//    uint8_t fourth_dart_speed;
//    uint16_t last_dart_launch_time;
    uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;

//�������ݽ�����Ϣ�� 0x0301��
typedef __packed struct
{
    uint16_t data_cmd_id;
    uint16_t sender_ID;
    uint16_t receiver_ID;
} ext_student_interactive_header_data_t;

//1.�������� �����˼�ͨ�ţ� 0x0301��
typedef __packed struct
{
    uint8_t m_ReceiveData[113];
    uint8_t m_SendData[113];
} robot_interactive_data_t;

//2.�ͻ���ɾ��ͼ�� �����˼�ͨ�ţ� 0x0301��
typedef __packed struct
{
    uint8_t operate_tpye;
    uint8_t layer;
} ext_client_custom_graphic_delete_t;

//ͼ������
typedef __packed struct
{
    uint8_t graphic_name[3];
    uint32_t operate_tpye:3;
    uint32_t graphic_tpye:3;
    uint32_t layer:4;
    uint32_t color:4;
    uint32_t start_angle:9;
    uint32_t end_angle:9;
    uint32_t width:10;
    uint32_t start_x:11;
    uint32_t start_y:11;
    uint32_t radius:10;
    uint32_t end_x:11;
    uint32_t end_y:11;
} graphic_data_struct_t;

//3.�ͻ��˻���һ��ͼ�� �����˼�ͨ�ţ� 0x0301��
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct;
} ext_client_custom_graphic_single_t;

//4.�ͻ��˻��ƶ���ͼ�� �����˼�ͨ�ţ� 0x0301��
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct[2];
} ext_client_custom_graphic_double_t;

//5.�ͻ��˻������ͼ�� �����˼�ͨ�ţ� 0x0301��
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct[5];
} ext_client_custom_graphic_five_t;

//6.�ͻ��˻����߸�ͼ�� �����˼�ͨ�ţ� 0x0301��
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct[7];
} ext_client_custom_graphic_seven_t;

//7.�ͻ��˻����ַ� �����˼�ͨ�ţ� 0x0301��
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct;
    uint8_t data[30];
} ext_client_custom_character_t;

//�����������˻�����Ϣ
typedef __packed struct
{
	u8 Dart_Num;
} ext_aerial_data_t;

typedef enum
{
  en_Yellow 	= 1,
	en_Green	= 2,
	en_Orange	= 3,
	en_Purple	= 4,
	en_Pink		= 5,
  en_Grblue 	= 6,
  en_Black 	= 7,
	en_White 	= 8,
} EN_Client_PictureColor;
typedef struct
{
	EN_Client_PictureColor Cap_Color;				//����ͼ����ɫ
  float Cap_Energy;								//����ʣ������
	float Cap_Length;								//����ͼ�㳤��
	float Volume_Y0;								//�����ײ�����׼��ֵ
	float Volume_X0;								//�����ײ�����׼��ֵ
	float Volume_Y1;								//�����ײ�����׼ĩֵ
	float Volume_X1;								//�����ײ�����׼ĩֵ	
	float Volume_Y;									//�����ײ�����׼
	float Volume_X;									//�����ײ�����׼	
	float Volume_Angle_Inc;							//�����ײ��ֵб��
	float Volume_K;									//�����ײб��б��
	int Bullet_Num;
} ST_Client_Sendata_Coe;
typedef struct
{
	float X1;		
	float Y1;		
	float X2;			
	float Y2;		
	float X3;		
	float Y3;
	float X4;		
	float Y4;
} ST_Client_Draw_Ruler;
typedef struct
{
	u8 Dart_Remaining_Time;
	bool FrictionStable_Flag;
	float Pitch_PosErr;
	float Yaw_PosErr;
	float Track_PosErr;
	float Friction_L1_SpeedErr;
	float Friction_L2_SpeedErr;
	float Friction_R1_SpeedErr;
	float Friction_R2_SpeedErr;
} ST_Data_to_Ariel;

typedef struct
{
	u8 door_state;
} ST_Data_to_Radar;

typedef struct
{
	u8 Head1; //1
	u8 Head2; //1
	u8 ID;    //1
	u8 Fcn;	  //1
	float Data1;//4
	float Data2;//4
	u8 Tail1; //1 
	u8 Tail2; //1 
}ST_Data_From_Visual; //14

extern ST_Data_From_Visual Data_From_Visual;

typedef unsigned char *byte_pointer;

extern ST_RSYS_MONITOR RSYS_Monitor;

extern ext_game_status_t				      G_ST_Game_Status;//1.����״̬����
extern ext_game_result_t              G_ST_Game_Result;//2.�����������
extern ext_game_robot_HP_t            G_ST_Game_Robot_HP;//3.������Ѫ������
extern ext_dart_status_t			        G_ST_Dart_Status;//4.���ڷ���״̬
extern ext_event_data_t               G_ST_Event_Data;//6.�����¼�����
extern ext_supply_projectile_action_t G_ST_Supply_Projectile_Action;//7.����վ������ʶ
extern ext_referee_warning_t          G_ST_Referee_Warning;//8.���о�����Ϣ
extern ext_dart_remaining_time_t			G_ST_Dart_Remaining_Time;//9.���ڷ���ڵ���ʱ
extern ext_game_robot_status_t        G_ST_Game_Robot_Status;//10.����������״̬
extern ext_power_heat_data_t          G_ST_Power_Heat_Data;//11.ʵʱ������������
extern ext_game_robot_pos_t           G_ST_Game_Robot_Pos;//12.������λ��
extern ext_buff_t						        	G_ST_Buff;//13.����������
extern aerial_robot_energy_t          G_ST_Aerial_Robot_Energy;//14.���л���������״̬
extern ext_robot_hurt_t               G_ST_Robot_Hurt;//15.�˺�״̬
extern ext_shoot_data_t               G_ST_Shoot_Data;//16.ʵʱ�����Ϣ
extern ext_bullet_remaining_t         G_ST_Bullet_Remaining;//17.�ӵ�ʣ�෢����
extern ext_rfid_status_t				      G_ST_RFID_Status;//18.������ RFID ״̬
extern ext_dart_client_cmd_t          G_ST_Dart_Client_Cmd;//19.���ڻ����˿ͻ���ָ������

extern ext_student_interactive_header_data_t	G_ST_Student_Interactive_Header_Data;//�������ݽ�����Ϣ
extern robot_interactive_data_t					      G_ST_Robot_Interactive_Data;//1.��������
extern ext_client_custom_graphic_delete_t			G_ST_Client_Custom_Graphic_Delete;//2.�ͻ���ɾ��ͼ��
extern ext_client_custom_graphic_single_t			G_ST_Client_Custom_Graphic_Single;//3.�ͻ��˻���һ��ͼ��
extern ext_client_custom_graphic_double_t			G_ST_Client_Custom_Graphic_Double;//4.�ͻ��˻��ƶ���ͼ��
extern ext_client_custom_graphic_five_t		  	G_ST_Client_Custom_Graphic_Five;//5.�ͻ��˻������ͼ��
extern ext_client_custom_graphic_seven_t			G_ST_Client_Custom_Graphic_Seven;//6.�ͻ��˻����߸�ͼ��
extern ext_client_custom_character_t			    G_ST_Client_Custom_Character;//7.�ͻ��˻����ַ�

extern ext_aerial_data_t DataFromAerial;
extern ST_Data_to_Radar Data_to_Radar;

void RC_Protocol(void);
void Rc_RsysProtocol(void);
void MonitorDataDeal(u16 usCmdID);
void float_to_u8(float f, u8* a);
float u8_to_float(u8* a);

int u8_to_int(u8*rx ,int n);
void Send_Data_To_Aerial(void);

void Send_Data_To_Visual(void);

void sendDataToClient(float data1, float data2, float data3, float data4, float data5);
void sendPictureToClient(void* picture, u16 pictureNum);
void deletePictureOnClient(u8 operateTpye, u8 layer);
u8   attackedDirectionJudgment(void);
void attackedTimeRecord(void);
void drawPictureOnClient(void);
void drawRuler_OnClient(void);
void Send_Char_To_Client(void);
void Send_Warning_To_Client(void);
void New_Client(void);
extern u32 attackedTime[4];
void prepareAttackedDirectionPicture(void);
void pictureOperation(graphic_data_struct_t* pGraphicData, u8* graphicName, u32 operateTpye, u32 graphicTpye,
                      u32 layer, u32 color, u32 startAngle, u32 endAngle, u32 width, u32 startX, u32 startY,
                      u32 radius, u32 endX, u32 endY);
void pictureOperation_float(graphic_data_struct_t* pGraphicData, u8* graphicName, u32 operateTpye, u32 graphicTpye,
                      u32 layer, u32 color, u32 startAngle, u32 endAngle, u32 width, u32 startX, u32 startY,
                      float FP_Num);


#endif
