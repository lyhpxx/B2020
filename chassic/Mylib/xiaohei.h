#ifndef __XIAOHEI_H__
#define __XIAOHEI_H__

#include "stm32f4xx.h"

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

void get_gyro_measuer(gyro_measure_t *Jxun,CanRxMsg *msg);
void Motor_YT_Can1TX(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4 );

#endif
