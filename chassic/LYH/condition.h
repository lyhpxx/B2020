#ifndef __CONDITION_H__
#define __CONDITION_H__
#include "stm32f4xx.h"

#define Robot           1       //����������

#define Robot_Mode      1       //ģʽѡ��1������2���


#if Robot == 1

    #define YAW_Init_Position       7522
    #define DISTANCE

#elif Robot == 2

    #define YAW_Init_Position       2016
    #define DISTANCE
    
#elif Robot == 3

    #define YAW_Init_Position       2016
    #define DISTANCE
    
#endif

extern float moto1_pid[5];
extern float moto2_pid[5];
extern float moto3_pid[5];
extern float moto4_pid[5];

#endif

