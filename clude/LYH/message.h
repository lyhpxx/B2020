#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "stm32f4xx.h"
#include "pid.h"
#include "dbus.h"
#include "mpu6050_driver.h"

//������ݽṹ��
typedef struct
{
    int16_t speed;                  //�ٶ�
    int16_t ecd;                    //��ǰ������ֵ
    int16_t last_ecd;               //��һ�α�����ֵ
    int16_t ecd_speed;              //�������ٶ�
    int     ecd_num;                //�������ۼӺ�
    
    int16_t current;                //ת�ӵ���
    
    int8_t temp;                    //�¶�
    int8_t first;
    u8 flag;
}moto_message;



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
    const moto_message * Pitch;             //pitch��
    const moto_message * Yaw;               //yaw��
    const moto_message * left_wheel;        //��Ħ����
    const moto_message * right_wheel;       //��Ħ����
    const moto_message * pluck;             //���̵��
    
    const MPU6050_REAL_DATA * mpu;
    
    FUZZY_PID pitch_speed;                //pitch���ٶȣ�MPU6050��
    FUZZY_PID pitch_ang;                  //pitch�ǽǶȣ�����������Ƕȣ�
    PID yaw_speed;                  //yaw���ٶȣ������ǽ��ٶȣ�
    PID yaw_ang;                    //yaw�ǽǶȣ������ǽǶȣ�
    PID left_wheel_speed;           //��Ħ�����ٶ�
    PID right_wheel_speed;          //��Ħ�����ٶ�
    PID pluck_speed;                //���̵���ٶ�
    PID pluck_ang;                  //���̵���Ƕ�
    
    const gyro_measure_t * gyro_yaw;        //С�ڿ���Ϣ
    
    const DBUS * remot;                   //ң������Ϣ
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

