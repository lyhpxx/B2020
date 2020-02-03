#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx.h"

#define NB          -6
#define NM          -4
#define NS          -2
#define ZO           0
#define PS           2
#define PM           4
#define PB           6

#if 1

#define KP_NB       -3
#define KP_NM       -2
#define KP_NS       -1
#define KP_ZO        0
#define KP_PS        1
#define KP_PM        2
#define KP_PB        3

#define KI_NB       -3
#define KI_NM       -2
#define KI_NS       -1
#define KI_ZO        0
#define KI_PS        1
#define KI_PM        2
#define KI_PB        3

#define KD_NB       -3
#define KD_NM       -2
#define KD_NS       -1
#define KD_ZO        0
#define KD_PS        1
#define KD_PM        2
#define KD_PB        3

static const float ruleKp[7][7] = {
    
    KP_PB,KP_PB,KP_PM,KP_PM,KP_PS,KP_ZO,KP_ZO,
    KP_PB,KP_PB,KP_PM,KP_PS,KP_PS,KP_ZO,KP_NS,
    KP_PM,KP_PM,KP_PM,KP_PS,KP_ZO,KP_NS,KP_NS,
    KP_PM,KP_PM,KP_PS,KP_ZO,KP_NS,KP_NM,KP_NM,
    KP_PS,KP_PS,KP_ZO,KP_NS,KP_NS,KP_NM,KP_NM,
    KP_PS,KP_ZO,KP_NS,KP_NM,KP_NM,KP_NM,KP_NB,
    KP_ZO,KP_ZO,KP_NM,KP_NM,KP_NM,KP_NB,KP_NB
};

static const float ruleKi[7][7] = {
    KI_NB,KI_NB,KI_NM,KI_NM,KI_NS,KI_ZO,KI_ZO,
    KI_NB,KI_NB,KI_NM,KI_NS,KI_NS,KI_ZO,KI_ZO,
    KI_NB,KI_NM,KI_NS,KI_NS,KI_ZO,KI_PS,KI_PS,
    KI_NM,KI_NM,KI_NS,KI_ZO,KI_PS,KI_PM,KI_PM,
    KI_NM,KI_NS,KI_ZO,KI_PS,KI_PS,KI_PM,KI_PB,
    KI_ZO,KI_ZO,KI_PS,KI_PS,KI_PM,KI_PB,KI_PB,
    KI_ZO,KI_ZO,KI_PS,KI_PM,KI_PM,KI_PB,KI_PB
};

static const float ruleKd[7][7] = {

   KD_PS,KD_NS,KD_NB,KD_NB,KD_NB,KD_NM,KD_PS,
   KD_PS,KD_NS,KD_NB,KD_NM,KD_NM,KD_NS,KD_ZO,
   KD_ZO,KD_NS,KD_NM,KD_NM,KD_NS,KD_NS,KD_ZO,
   KD_ZO,KD_NS,KD_NS,KD_NS,KD_NS,KD_NS,KD_ZO,
   KD_ZO,KD_ZO,KD_ZO,KD_ZO,KD_ZO,KD_ZO,KD_ZO,
   KD_PB,KD_NS,KD_PS,KD_PS,KD_PS,KD_PS,KD_PB,
   KD_PB,KD_PM,KD_PM,KD_PM,KD_PS,KD_PS,KD_PB
};
#else
static const float ruleKp[7][7] = {
    
    PB,PB,PM,PM,PS,ZO,ZO,
    PB,PB,PM,PS,PS,ZO,NS,
    PM,PM,PM,PS,ZO,NS,NS,
    PM,PM,PS,ZO,NS,NM,NM,
    PS,PS,ZO,NS,NS,NM,NM,
    PS,ZO,NS,NM,NM,NM,NB,
    ZO,ZO,NM,NM,NM,NB,NB
};

static const float ruleKi[7][7] = {
    NB,NB,NM,NM,NS,ZO,ZO,
    NB,NB,NM,NS,NS,ZO,ZO,
    NB,NM,NS,NS,ZO,PS,PS,
    NM,NM,NS,ZO,PS,PM,PM,
    NM,NS,ZO,PS,PS,PM,PB,
    ZO,ZO,PS,PS,PM,PB,PB,
    ZO,ZO,PS,PM,PM,PB,PB
};

static const float ruleKd[7][7] = {

   PS,NS,NB,NB,NB,NM,PS,
   PS,NS,NB,NM,NM,NS,ZO,
   ZO,NS,NM,NM,NS,NS,ZO,
   ZO,NS,NS,NS,NS,NS,ZO,
   ZO,ZO,ZO,ZO,ZO,ZO,ZO,
   PB,NS,PS,PS,PS,PS,PB,
   PB,PM,PM,PM,PS,PS,PB
};
#endif

typedef struct
{
    float add;  //���ٶ�
    float dec;  //���ٶ�
    
    float trans;
    
}Add;  //���ٶ�

typedef struct
{
    float set;              //�趨ֵ
    float get;              //��ǰֵ
    
    float error;            //��ǰ���
    float last_error;       //�ϴ����
    float sum_error;        //����ۼƺ�
    
    float kp;               //����ϵ��
    float ki;               //����ϵ��
    float kd;               //΢��ϵ��
    
    float max_sum_error;    //���֮��������
    float max_out;          //���֮��������
    
    float out;              //���
    
    Add AD;                 //���ٶ�
    
}PID;

#define     LIMIT(num, min, max)\
            if ( num <= min )\
            {\
                num = min;\
            }\
            else if ( num >= max )\
            {\
                num = max;\
            }\
            
           


typedef struct
{
    float set;
    float get;
    
    float kp;
    float delt_kp;          //��������
    float dkp;              //����ϵ��
    float max_delt_kp;      //�������ֵ
    float ki;
    float delt_ki;
    float dki;
    float max_delt_ki;
    float kd;
    float delt_kd;
    float dkd;
    float max_delt_kd;
    
    float max_num;          //��Χ
    float min_num;
    
    float this_error;
    float qu_error;
    
    float this_ec;
    float qu_ec;
    
    float last_error;
    
    PID MOTO;
    
}FUZZY_PID;


static void LinearQuantization(FUZZY_PID * FU_PID,float set,float get);
static void CalcMembership(float input,int * index,float * num);
void FuzzyComputation (FUZZY_PID *vPID,float set,float get);
void FUZZY_PID_Init(FUZZY_PID * FU_PID,float * kp,float * ki,float * kd,float * pid,float mind,float maxd);
void PID_Calculate_F(FUZZY_PID * pid_n);            
void PID_Init(PID * pid,float kp,float ki,float kd,float max_ek_sum,float max_out);
void PID_Calculat(PID * pid,float current,float target);
            

#endif

