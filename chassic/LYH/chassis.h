#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "stm32f4xx.h"
#include "message.h"


void Chassis_Task(void *pvParameters);
void Chassis_PID_Init(void);
void Chassis_Calculate(float vx,float vy,float roll);
void accelerate(float setspeed,float *speed_transit,int16_t acc,int16_t  dec);
void Chassis_PID_Calculate(float t1,float t2,float t3,float t4);
void MODE_Choose(void);
void MODE_ENABLE(void);
void Chassic_Power_Limit(void);
float Follow(float set,float get);

#endif


