#ifndef __CAN_TASK_H__
#define __CAN_TASK_H__

#include "stm32f4xx.h"

void Bullet_Send(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4 );
void Surve_Send(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4 );	
void CAN_Task(void *pvParameters);
void CAN1_Send_Message(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4);

#endif

