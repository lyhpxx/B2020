#ifndef __CAN_TASK_H__
#define __CAN_TASK_H__

#include "stm32f4xx.h"

void CAN1_Send(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4 );	// ‰»ÎÀŸ∂»
void CAN_Send_Clude_Task(void *pvParameters);
void CAN2_Remote(uint8_t MODE,int16_t ch2,int16_t ch3,int16_t res_pluck,uint8_t speed_level);
void CAN2_KEYS(uint8_t MODE,int16_t mouse_x,int16_t mouse_y,int16_t key,uint8_t mou);

#endif

