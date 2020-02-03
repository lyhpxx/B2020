#ifndef __CONDITION_H__
#define __CONDITION_H__

#define Robot           1       //步兵机器人

#define Robot_Mode      1       //模式选择：1正常，2打符


#if Robot == 1
    
    #define MAX_PITCH_ECD             2016
    #define MIN_PITCH_ECD             461
    #define PITCH_Init_Position       1239
    

#elif Robot == 2

    #define MAX_PITCH_ECD             500
    #define MIN_PITCH_ECD             3000
    #define PITCH_Init_Position       ((MAX_PITCH_ECD - MIN_PITCH_ECD) / 2 + MIN_PITCH_ECD)
    
    
#elif Robot == 3

    #define MAX_PITCH_ECD             500
    #define MIN_PITCH_ECD             3000
    #define PITCH_Init_Position       ((MAX_PITCH_ECD - MIN_PITCH_ECD) / 2 + MIN_PITCH_ECD)
    
    
#endif

extern float Fuzzy_KP_s[3];
extern float Fuzzy_KI_s[3];
extern float Fuzzy_KD_s[3];
extern float Fuzzy_KP_a[3];
extern float Fuzzy_KI_a[3];
extern float Fuzzy_KD_a[3];
extern float yawspeed[5];
extern float yawang[5];
extern float r_wheel[5];
extern float l_wheel[5];
extern float pl_s[5];
extern float pl_a[5];
extern float PID_Ini[2];

#endif

