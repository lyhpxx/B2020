#ifndef __USART2_H__
#define __USART2_H__
#include "main.h"

//#define JudgeFrameHeader		0xA5		//数据帧起始字节，固定值为 0xA5
#define JudgeBufferLength       120


void USART2_Configuration(void);
void USART2_SendChar(unsigned char b);
void Appointment_bullet(int ID,int num);							//预约补给站ID，预约子弹个数
void Robot_interactive(int date1,int date2,int date3,int date4);	//机器人互交

//存储器到外设模式


//结构体

typedef struct
{
	uint8_t 	type;					//比赛类型
	uint8_t 	course;					//比赛进程
	uint16_t	remaining_time;			//比赛剩余时间
	uint8_t 	result;					//比赛结果
	
}Game_state;							//比赛状态

typedef struct
{
	uint8_t	Tarmac;						//停机坪
	uint8_t	Supply_1;					//补给站1
	uint8_t	Supply_2;					//补给站2
	uint8_t	Supply_3;					//补给站3
	uint8_t	Big_energy;					//打击点 情况 及大 能量机关 激活状态
	uint8_t	Strategic_pass;				//关口
	uint8_t	Pillbox;					//碉堡	
	uint8_t	Resource_island;			//资源岛
	uint8_t	ICRA_red;					//ICRA红
	uint8_t	ICRA_blue;					//ICRA蓝
	uint8_t	Base;						//基地
	
}Field_events;							//场地事件

typedef struct
{
	/*接收*/
	uint8_t projectile_id;				//补给站口
	uint8_t robot_id;					//预约机器人
	uint8_t projectile_state;			//子弹口开闭状态
	uint8_t projectile_num;
	/*发送*/
	uint8_t supply_projectile_id;		//预约补给站口
	uint8_t supply_num;					//预约子弹数量
	
}Supply_action;							//补给站动作

typedef struct
{
    uint8_t 	Robot_id;			    //机器人id
	uint8_t 	Robot_level;			//机器人等级
	uint16_t	Remain_HP;				//剩余血量
	
	uint16_t	chassis_volt;			//底盘电压
	uint16_t 	chassis_current;		//底盘电流
	float 		chassis_power;			//底盘功率
	uint16_t 	chassis_power_buffer;	//底盘剩余功率
	uint16_t 	shooter_heat_17mm;		//17mm枪口热量
	uint16_t 	shooter_heat_42mm;		//42mm枪口热量
	
	uint8_t 	armor_id;				//受伤装甲ID
	uint8_t 	hurt_type;				//伤害类型
	
	float 		bullet_speed_17mm;		//17mm子弹射速
	float 		bullet_speed_42mm;		//42mm子弹射速
	
	uint8_t 	bullet_RF_17mm;			//17mm子弹射频
	uint8_t 	bullet_RF_42mm;			//42mm子弹射频

//	uint8_t 	Robot_id;				//机器人ID
//	uint16_t 	Max_HP;					//满血量
//	uint16_t	Cooling_rate_17mm;		//17mm冷却速度
//	uint16_t 	Cooling_rate_42mm;		//42mm冷却速度
//	uint16_t 	Heat_upper_17mm;		//17mm热量上限
//	uint16_t 	Heat_upper_42mm;		//42mm热量上限

	uint8_t 	gimbal_output;			//主控给其它（云台）输出
	uint8_t 	chassis_output;			//主控给底盘输出
	uint8_t 	shooter_output;			//主控给枪口输出
	
	uint16_t	receive_date_1;			//接收数据1
	uint16_t	receive_date_2;			//接收数据2
	uint16_t	receive_date_3;			//接收数据3
	uint16_t	receive_date_4;			//接收数据4
	
}Robot_stage;							//机器人状态

typedef  struct
{
	uint8_t Energy;						//积累能量
	uint8_t Time;						//可攻击时间（s）
	
}Airplane_stage;						//飞机状态


//格式转换联合体
typedef union
{
	char	T[2];
	uint16_t	Time;     
}Time_Remaining;		//剩余时间

typedef union
{
	char	H[2];
	uint16_t	HP;     
}Blood_Remaining;		//剩余血量

typedef union
{
	char	V[2];
	uint16_t	VOTE;     
}Chassis_Volt;			//底盘电压

typedef union
{
	char	C[2];
	uint16_t	CURRENT;     
}Chassis_Current;		//底盘电流

typedef union
{
	char	P[4];
	float		POWER;     
}Chassis_Powe;			//底盘功率

typedef union
{
	char	P_B[2];
	uint16_t	POWER_BUFER;     
}Chassis_Powe_Buffer;	//底盘功率缓冲

typedef union
{
	char	S_H_17[2];
	uint16_t	HEAT_17mm;     
}Shooter_Heat_17mm;		//17mm枪口热量

typedef union
{
	char	S_H_42[2];
	uint16_t	HEAT_42mm;     
}Shooter_Heat_42mm;		//42mm枪口热量	

typedef union
{
	char	T[2];
	uint16_t	TIME;     
}Airplane_Attack;		//飞机可攻击时间 

typedef union
{
	char	ST[4];
	float		SPEED;
}Shoot_Speed;			//射速

typedef union
{
	char	D_1[2];
	float		DATE_1;
}Robot_Interactive_Receive_1;	//机器人交互接受数据1

typedef union
{
	char	D_2[2];
	float		DATE_2;
}Robot_Interactive_Receive_2;	//机器人交互接受数据2

typedef union
{
	char	D_3[2];
	float		DATE_3;
}Robot_Interactive_Receive_3;	//机器人交互接受数据3

typedef union
{
	char	D_4[2];
	float		DATE_4;
}Robot_Interactive_Receive_4;	//机器人交互接受数据4


extern	Game_state		game_state;						//比赛状态
extern	Field_events	field_events	;				//场地事件
extern	Supply_action	supply_action	;				//补给站动作
extern	Robot_stage		robot_stage		;				//机器人状态
extern	Airplane_stage	airplane_stage	;				//飞机状态


extern	Time_Remaining		time_remaining	;			//剩余时间
extern	Blood_Remaining		blood_remaining	;			//剩余血量
extern	Chassis_Volt		chassis_volt;				//底盘电压
extern	Chassis_Current		chassis_current;			//底盘电流
extern	Chassis_Powe		chassis_powe;				//底盘功率
extern	Chassis_Powe_Buffer	chassis_powe_buffer;		//底盘功率缓冲
extern	Shooter_Heat_17mm	shooter_heat_17mm;			//17mm枪口热量
extern	Shooter_Heat_42mm	shooter_heat_42mm;			//42mm枪口热量
extern	Airplane_Attack		airplane_attack;			//飞机可攻击时间 
extern	Shoot_Speed			shoot_speed;				//射速


extern	Robot_Interactive_Receive_1	robot_interactive_receive_1;	//机器人交互数据1
extern	Robot_Interactive_Receive_2	robot_interactive_receive_2;	//机器人交互数据2
extern	Robot_Interactive_Receive_3	robot_interactive_receive_3;	//机器人交互数据3
extern	Robot_Interactive_Receive_4	robot_interactive_receive_4;	//机器人交互数据4




//需要通过裁判系统发送显示在PC端的数据

typedef struct
{
	float hk1;
	float hk2;
	float hk3;
	uint8_t hk4;
}data_Judge_System;

typedef union
{
	char x1[4];
	float y1;
}xy1;     

typedef union
{
	char x2[4];
	float y2;
}xy2;     

typedef union
{
	char x3[4];
	float y3;
}xy3;     

void Judge_System_SendData(data_Judge_System  Data_Judge_System);
void DMA1_Stream6_IRQHandler(void);


#endif


