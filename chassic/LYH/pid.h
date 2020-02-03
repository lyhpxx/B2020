#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx.h"

typedef struct
{
    float Trans;
    float add;
    float dec;
    
}Add;

typedef struct
{
    float set;              //设定值
    float get;              //当前值
    
    float error;            //当前误差
    float last_error;       //上次误差
    float sum_error;        //误差累计和
    
    float kp;               //比例系数
    float ki;               //积分系数
    float kd;               //微分系数
    
    float max_sum_error;    //误差之和上下限
    float max_out;          //输出之和上下限
    
    float out;              //输出
    
    Add ADD;
    
}PID;

#define     LIMIT(num, min, max)\
            if ( num <= min )\
            {\
                num = min;\
            }\
            else if ( num >= max )\
            {\
                num = max;\
            }\

void PID_Init(PID * pid,float kp,float ki,float kd,float max_ek_sum,float max_out);
void PID_Calculat(PID * pid,float current,float target);
            

#endif

