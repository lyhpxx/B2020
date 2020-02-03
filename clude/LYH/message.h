#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "stm32f4xx.h"
#include "pid.h"
#include "dbus.h"
#include "mpu6050_driver.h"

//电机数据结构体
typedef struct
{
    int16_t speed;                  //速度
    int16_t ecd;                    //当前编码器值
    int16_t last_ecd;               //上一次编码器值
    int16_t ecd_speed;              //编码器速度
    int     ecd_num;                //编码器累加和
    
    int16_t current;                //转子电流
    
    int8_t temp;                    //温度
    int8_t first;
    u8 flag;
}moto_message;



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
    const moto_message * Pitch;             //pitch角
    const moto_message * Yaw;               //yaw角
    const moto_message * left_wheel;        //左摩擦轮
    const moto_message * right_wheel;       //右摩擦轮
    const moto_message * pluck;             //拨盘电机
    
    const MPU6050_REAL_DATA * mpu;
    
    FUZZY_PID pitch_speed;                //pitch角速度（MPU6050）
    FUZZY_PID pitch_ang;                  //pitch角角度（电机编码器角度）
    PID yaw_speed;                  //yaw角速度（陀螺仪角速度）
    PID yaw_ang;                    //yaw角角度（陀螺仪角度）
    PID left_wheel_speed;           //左摩擦轮速度
    PID right_wheel_speed;          //右摩擦轮速度
    PID pluck_speed;                //拨盘电机速度
    PID pluck_ang;                  //拨盘电机角度
    
    const gyro_measure_t * gyro_yaw;        //小黑快信息
    
    const DBUS * remot;                   //遥控器信息
    const DBUS * key;
    
    uint8_t wheel_speed_level;
    
    int16_t MODE;
    
    float set_pitch;
    float pitch_add;
    float set_yaw;
    float wheel_speed;
    float set_pluck;
    
    
}ALL;

extern ALL X_X;

extern moto_message pit;
extern moto_message ya;
extern moto_message le_w;
extern moto_message ri_w;
extern moto_message pl;
extern gyro_measure_t gyr;
extern DBUS rc_remot;
extern DBUS rc_key;

extern const moto_message * get_PIT(void);
extern const moto_message * get_YAW(void);
extern const moto_message * get_LE_W(void);
extern const moto_message * get_RI_W(void);
extern const moto_message * get_PL(void);
extern const gyro_measure_t * get_ANG(void);
extern const DBUS * get_DBUS_REMOT(void);
extern const DBUS * get_DBUS_KEY(void);
extern const MPU6050_REAL_DATA * get_gyr(void);

void MOTO_Message(moto_message * mot,CanRxMsg * mes);
void get_gyro_measuer(gyro_measure_t *Jxun,CanRxMsg *msg);
void CAN2_Receive_Remot(ALL * all,DBUS * db,CanRxMsg *msg);
void CAN2_Receive_KEY(ALL * all,DBUS * db,CanRxMsg *msg);



#endif

