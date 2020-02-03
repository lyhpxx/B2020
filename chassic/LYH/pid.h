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
    float set;              //�趨ֵ
    float get;              //��ǰֵ
    
    float error;            //��ǰ���
    float last_error;       //�ϴ����
    float sum_error;        //����ۼƺ�
    
    float kp;               //����ϵ��
    float ki;               //����ϵ��
    float kd;               //΢��ϵ��
    
    float max_sum_error;    //���֮��������
    float max_out;          //���֮��������
    
    float out;              //���
    
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

