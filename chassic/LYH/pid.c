#include "pid.h"


//PID初始化函数
void PID_Init(PID * pid,float kp,float ki,float kd,float max_ek_sum,float max_out)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    
    pid->max_sum_error = max_ek_sum;
    pid->max_out = max_out;
}


//PID计算函数
void PID_Calculat(PID * pid,float current,float target)
{
    pid->set = target;
    pid->get = current;
    
    pid->error = pid->set - pid->get;
    
    pid->sum_error += pid->error;
    
    LIMIT(pid->sum_error,-pid->max_sum_error,pid->max_sum_error);
    
    pid->out = pid->kp * pid->error
              +pid->ki * pid->sum_error
              +pid->kd * (pid->error - pid->last_error);
    
    LIMIT(pid->out,-pid->max_out,pid->max_out);
    
    pid->last_error = pid->error;
}

