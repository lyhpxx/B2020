#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "stm32f4xx.h"
#include "pid.h"
#include "dbus.h"

//电机数据结构体
typedef struct
{
    int16_t speed;  //速度
    int16_t ecd;    //编码值
    int16_t last_ecd;//上次编码值
    int16_t ecd_speed;
    int     ecd_num;    //增量值
    
    int16_t current;    //电流
    
    int8_t temp;        //温度
    int16_t first;
    u8 flag;
    
}moto_message;

typedef struct
{
    int16_t current;
    int16_t votlage;
    
    float power;
}Power;



//小黑快数据结构体
#define  rmf  float
#define  rmt  int	
typedef struct
{
    rmf yaw_angle_nums;
    rmf Angular_velocity;//小黑块角速度
    rmt temp_yaw_angle;//角度
    rmt angle_remainder;
    rmt angle_Business_value;
    rmf Last_yaw_angle;
    rmf This;
}gyro_measure_t;


typedef enum
{
    robo_follow = (uint8_t)(0x01<<0), //跟随
    robo_roll = (uint8_t)(0x01<<1),   //自旋
    robo_death = (uint8_t)(0x01<<2),  //死亡

}ROBO_Mode;


typedef enum
{
    robo_remote = (uint8_t)(0x01<<3),     //只使用遥控器
    robo_mouse_key = (uint8_t)(0x01<<4),  //只使用键盘鼠标
    robo_ctrl_all = (uint8_t)(0x01<<5),   //键盘鼠标遥控器通用
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
    PID motor4;     //电机PID
    
    const moto_message * moto1_message;
    const moto_message * moto2_message;
    const moto_message * moto3_message;
    const moto_message * moto4_message;     //电机信息
    const moto_message * GM; 
    
    const int * GM_6020_Point_ECD_NUM;
    
    const float * GM6020_sita_Point;             //6020反馈

    gyro_measure_t gyro_yaw;        //小黑快信息
    
    const DBUS * rc;                   //遥控器信息
    
    Wheel_Speed wheel_spee;         //摩擦轮电机速度
    
    Power pow;                      //功率
    
    ROBO_Mode Robot_MODE;              //机器人模式
    
    Ctrl_MODE ctrl_mode;               //遥控模式
    
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

