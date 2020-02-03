#ifndef __REPOT_H__
#define __REPOT_H__

#include "stm32f4xx.h"

void Printf_Task(void *pvParameters);
void Data_sends(int16_t data1,int16_t data2,int16_t data3,int16_t data4,int16_t data5,int16_t data6);

#endif
