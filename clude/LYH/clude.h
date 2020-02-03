#ifndef __CLUDE_H__
#define __CLUDE_H__

#include "stm32f4xx.h"

void Clude_Task(void *pvParameters);
void Clude_PID_Init(void);
void Clude_PID_Cal(void);
void accelerate(float setspeed,float *speed_transit,int16_t acc,int16_t  dec);
void RC_CTRL_Test(void);

#endif

