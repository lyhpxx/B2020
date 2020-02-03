#ifndef __POWER_TASK_H__
#define __POWER_TASK_H__
#define YawMiddle 6807

extern float Power_PID(float Tar,float Now);

void Chassic_Power_Limit(float set_pow);

#endif 
