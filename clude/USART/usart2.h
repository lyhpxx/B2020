#ifndef __USART2_H__
#define __USART2_H__
#include "main.h"

//#define JudgeFrameHeader		0xA5		//����֡��ʼ�ֽڣ��̶�ֵΪ 0xA5
#define JudgeBufferLength       120


void USART2_Configuration(void);
void USART2_SendChar(unsigned char b);
void Appointment_bullet(int ID,int num);							//ԤԼ����վID��ԤԼ�ӵ�����
void Robot_interactive(int date1,int date2,int date3,int date4);	//�����˻���

//�洢��������ģʽ


//�ṹ��

typedef struct
{
	uint8_t 	type;					//��������
	uint8_t 	course;					//��������
	uint16_t	remaining_time;			//����ʣ��ʱ��
	uint8_t 	result;					//�������
	
}Game_state;							//����״̬

typedef struct
{
	uint8_t	Tarmac;						//ͣ��ƺ
	uint8_t	Supply_1;					//����վ1
	uint8_t	Supply_2;					//����վ2
	uint8_t	Supply_3;					//����վ3
	uint8_t	Big_energy;					//����� ��� ���� �������� ����״̬
	uint8_t	Strategic_pass;				//�ؿ�
	uint8_t	Pillbox;					//�ﱤ	
	uint8_t	Resource_island;			//��Դ��
	uint8_t	ICRA_red;					//ICRA��
	uint8_t	ICRA_blue;					//ICRA��
	uint8_t	Base;						//����
	
}Field_events;							//�����¼�

typedef struct
{
	/*����*/
	uint8_t projectile_id;				//����վ��
	uint8_t robot_id;					//ԤԼ������
	uint8_t projectile_state;			//�ӵ��ڿ���״̬
	uint8_t projectile_num;
	/*����*/
	uint8_t supply_projectile_id;		//ԤԼ����վ��
	uint8_t supply_num;					//ԤԼ�ӵ�����
	
}Supply_action;							//����վ����

typedef struct
{
    uint8_t 	Robot_id;			    //������id
	uint8_t 	Robot_level;			//�����˵ȼ�
	uint16_t	Remain_HP;				//ʣ��Ѫ��
	
	uint16_t	chassis_volt;			//���̵�ѹ
	uint16_t 	chassis_current;		//���̵���
	float 		chassis_power;			//���̹���
	uint16_t 	chassis_power_buffer;	//����ʣ�๦��
	uint16_t 	shooter_heat_17mm;		//17mmǹ������
	uint16_t 	shooter_heat_42mm;		//42mmǹ������
	
	uint8_t 	armor_id;				//����װ��ID
	uint8_t 	hurt_type;				//�˺�����
	
	float 		bullet_speed_17mm;		//17mm�ӵ�����
	float 		bullet_speed_42mm;		//42mm�ӵ�����
	
	uint8_t 	bullet_RF_17mm;			//17mm�ӵ���Ƶ
	uint8_t 	bullet_RF_42mm;			//42mm�ӵ���Ƶ

//	uint8_t 	Robot_id;				//������ID
//	uint16_t 	Max_HP;					//��Ѫ��
//	uint16_t	Cooling_rate_17mm;		//17mm��ȴ�ٶ�
//	uint16_t 	Cooling_rate_42mm;		//42mm��ȴ�ٶ�
//	uint16_t 	Heat_upper_17mm;		//17mm��������
//	uint16_t 	Heat_upper_42mm;		//42mm��������

	uint8_t 	gimbal_output;			//���ظ���������̨�����
	uint8_t 	chassis_output;			//���ظ��������
	uint8_t 	shooter_output;			//���ظ�ǹ�����
	
	uint16_t	receive_date_1;			//��������1
	uint16_t	receive_date_2;			//��������2
	uint16_t	receive_date_3;			//��������3
	uint16_t	receive_date_4;			//��������4
	
}Robot_stage;							//������״̬

typedef  struct
{
	uint8_t Energy;						//��������
	uint8_t Time;						//�ɹ���ʱ�䣨s��
	
}Airplane_stage;						//�ɻ�״̬


//��ʽת��������
typedef union
{
	char	T[2];
	uint16_t	Time;     
}Time_Remaining;		//ʣ��ʱ��

typedef union
{
	char	H[2];
	uint16_t	HP;     
}Blood_Remaining;		//ʣ��Ѫ��

typedef union
{
	char	V[2];
	uint16_t	VOTE;     
}Chassis_Volt;			//���̵�ѹ

typedef union
{
	char	C[2];
	uint16_t	CURRENT;     
}Chassis_Current;		//���̵���

typedef union
{
	char	P[4];
	float		POWER;     
}Chassis_Powe;			//���̹���

typedef union
{
	char	P_B[2];
	uint16_t	POWER_BUFER;     
}Chassis_Powe_Buffer;	//���̹��ʻ���

typedef union
{
	char	S_H_17[2];
	uint16_t	HEAT_17mm;     
}Shooter_Heat_17mm;		//17mmǹ������

typedef union
{
	char	S_H_42[2];
	uint16_t	HEAT_42mm;     
}Shooter_Heat_42mm;		//42mmǹ������	

typedef union
{
	char	T[2];
	uint16_t	TIME;     
}Airplane_Attack;		//�ɻ��ɹ���ʱ�� 

typedef union
{
	char	ST[4];
	float		SPEED;
}Shoot_Speed;			//����

typedef union
{
	char	D_1[2];
	float		DATE_1;
}Robot_Interactive_Receive_1;	//�����˽�����������1

typedef union
{
	char	D_2[2];
	float		DATE_2;
}Robot_Interactive_Receive_2;	//�����˽�����������2

typedef union
{
	char	D_3[2];
	float		DATE_3;
}Robot_Interactive_Receive_3;	//�����˽�����������3

typedef union
{
	char	D_4[2];
	float		DATE_4;
}Robot_Interactive_Receive_4;	//�����˽�����������4


extern	Game_state		game_state;						//����״̬
extern	Field_events	field_events	;				//�����¼�
extern	Supply_action	supply_action	;				//����վ����
extern	Robot_stage		robot_stage		;				//������״̬
extern	Airplane_stage	airplane_stage	;				//�ɻ�״̬


extern	Time_Remaining		time_remaining	;			//ʣ��ʱ��
extern	Blood_Remaining		blood_remaining	;			//ʣ��Ѫ��
extern	Chassis_Volt		chassis_volt;				//���̵�ѹ
extern	Chassis_Current		chassis_current;			//���̵���
extern	Chassis_Powe		chassis_powe;				//���̹���
extern	Chassis_Powe_Buffer	chassis_powe_buffer;		//���̹��ʻ���
extern	Shooter_Heat_17mm	shooter_heat_17mm;			//17mmǹ������
extern	Shooter_Heat_42mm	shooter_heat_42mm;			//42mmǹ������
extern	Airplane_Attack		airplane_attack;			//�ɻ��ɹ���ʱ�� 
extern	Shoot_Speed			shoot_speed;				//����


extern	Robot_Interactive_Receive_1	robot_interactive_receive_1;	//�����˽�������1
extern	Robot_Interactive_Receive_2	robot_interactive_receive_2;	//�����˽�������2
extern	Robot_Interactive_Receive_3	robot_interactive_receive_3;	//�����˽�������3
extern	Robot_Interactive_Receive_4	robot_interactive_receive_4;	//�����˽�������4




//��Ҫͨ������ϵͳ������ʾ��PC�˵�����

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


