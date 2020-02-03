#ifndef __CONDITION_H__
#define __CONDITION_H__
#include "stm32f4xx.h"

#define Robot           1       //步兵机器人

#define Robot_Mode      1       //模式选择：1正常，2打符


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

