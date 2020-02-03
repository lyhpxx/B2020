#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "stm32f4xx.h"
#include "pid.h"
#include "dbus.h"

//������ݽṹ��
typedef struct
{
    int16_t speed;  //�ٶ�
    int16_t ecd;    //����ֵ
    int16_t last_ecd;//�ϴα���ֵ
    int16_t ecd_speed;
    int     ecd_num;    //����ֵ
    
    int16_t current;    //����
    
    int8_t temp;        //�¶�
    int16_t first;
    u8 flag;
    
}moto_message;

typedef struct
{
    int16_t current;
    int16_t votlage;
    
    float power;
}Power;



//С�ڿ����ݽṹ��
#define  rmf  float
#define  rmt  int	
typedef struct
{
    rmf yaw_angle_nums;
    rmf Angular_velocity;//С�ڿ���ٶ�
    rmt temp_yaw_angle;//�Ƕ�
    rmt angle_remainder;
    rmt angle_Business_value;
    rmf Last_yaw_angle;
    rmf This;
}gyro_measure_t;


typedef enum
{
    robo_follow = (uint8_t)(0x01<<0), //����
    robo_roll = (uint8_t)(0x01<<1),   //����
    robo_death = (uint8_t)(0x01<<2),  //����

}ROBO_Mode;


typedef enum
{
    robo_remote = (uint8_t)(0x01<<3),     //ֻʹ��ң����
    robo_mouse_key = (uint8_t)(0x01<<4),  //ֻʹ�ü������
    robo_ctrl_all = (uint8_t)(0x01<<5),   //�������ң����ͨ��
}Ctrl_MODE;

typedef enum
{
    High_speed = 0x00,
    Model_speed = 0x01,
    Low_speed = 0x02,
    Dead_speed = 0x03,
    
}Wheel_Speed;


typedef struct
{
    float set_Vx;
    float set_Vy;
    float set_Vr;
    
    PID motor1;
    PID motor2;
    PID motor3;
    PID motor4;     //���PID
    
    const moto_message * moto1_message;
    const moto_message * moto2_message;
    const moto_message * moto3_message;
    const moto_message * moto4_message;     //�����Ϣ
    const moto_message * GM; 
    
    const int * GM_6020_Point_ECD_NUM;
    
    const float * GM6020_sita_Point;             //6020����

    gyro_measure_t gyro_yaw;        //С�ڿ���Ϣ
    
    const DBUS * rc;                   //ң������Ϣ
    
    Wheel_Speed wheel_spee;         //Ħ���ֵ���ٶ�
    
    Power pow;                      //����
    
    ROBO_Mode Robot_MODE;              //������ģʽ
    
    Ctrl_MODE ctrl_mode;               //ң��ģʽ
    
}ALL;

extern ALL X_X;

extern moto_message MOTO_1;
extern moto_message MOTO_2;
extern moto_message MOTO_3;
extern moto_message MOTO_4;
extern DBUS rc_ctr;
extern moto_message MOTO_GM;
extern int ecd_n;
extern float sita;

extern const moto_message * first(void);
extern const moto_message * second(void);
extern const moto_message * third(void);
extern const moto_message * fourth(void);
extern const DBUS * RC_CTRL(void);
extern const int * get_num(void);
extern const float * get_sita(void);
extern const moto_message * get_GM(void);


void MOTO_Message(moto_message * mot,CanRxMsg * mes);
void get_gyro_measuer(gyro_measure_t *Jxun,CanRxMsg *msg);
void get_power(Power * pow,CanRxMsg *msg);
void GM6020(moto_message * mot,CanRxMsg *msg);


#endif

