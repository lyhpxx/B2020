#include "condition.h"

#if Robot == 1

////                    ϵ�� ϵ������ ���ֵ
//    float Fuzzy_KP_s[3] = {20,20,20};
//    float Fuzzy_KI_s[3] = {0.2,10,0.2};
//    float Fuzzy_KD_s[3] = {10,5,5};     //�ٶȻ�ϵ��
//    
//    float Fuzzy_KP_a[3] = {8,10,5};
//    float Fuzzy_KI_a[3] = {0.2,0.2,1};
//    float Fuzzy_KD_a[3] = {10,5,5};     //�ǶȻ�ϵ��
//    
//    float yawspeed[5] = {0,0,0,10000,30000};
//    float yawang[5] = {0,0,0,10000,30000};//yaw��
//    
//    float r_wheel[5] = {15,0.6,10,10000,16000};
//    float l_wheel[5] = {15,0.6,10,10000,16000};//Ħ����
//    
//    float pl_s[5] = {4,0.5,10,5000,10000};
//    float pl_a[5] = {0.3,0.01,0,5000,10000};//���̵��

//    float PID_Ini[2] = {10000,30000};

//                    ϵ�� ϵ������ ���ֵ
    float Fuzzy_KP_s[3] = {35,0,0};
    float Fuzzy_KI_s[3] = {1,2,2};
    float Fuzzy_KD_s[3] = {0,0,0};     //�ٶȻ�ϵ��
    
    float Fuzzy_KP_a[3] = {3.8,0,0};
    float Fuzzy_KI_a[3] = {0.08,0,};
    float Fuzzy_KD_a[3] = {10,0,0};     //�ǶȻ�ϵ��
    
    float yawspeed[5] = {6.5,0.04,0,10000,30000};
    float yawang[5] = {3,0.07,40,10000,30000};//yaw��
    
    float r_wheel[5] = {15,1,10,10000,16000};
    float l_wheel[5] = {15,1.6,10,10000,16000};//Ħ����
    
    float pl_s[5] = {4,0.5,10,5000,10000};
    float pl_a[5] = {0.3,0.01,0,5000,10000};//���̵��

    float PID_Ini[2] = {10000,30000};
    

#elif Robot == 2

//                    ϵ�� ϵ������ ���ֵ
    float Fuzzy_KP_s[3] = {20,20,20};
    float Fuzzy_KI_s[3] = {0.2,10,0.2};
    float Fuzzy_KD_s[3] = {10,5,5};     //�ٶȻ�ϵ��
    
    float Fuzzy_KP_a[3] = {8,10,5};
    float Fuzzy_KI_a[3] = {0.2,0.2,1};
    float Fuzzy_KD_a[3] = {10,5,5};     //�ǶȻ�ϵ��
    
    float yawspeed[5] = {};
    float yawang[5] = {};
    
    float r_wheel[5] = {15,0.6,10,10000,16000};
    float l_wheel[5] = {15,0.6,10,10000,16000};
    
    float pl_s[5] = {4,0.5,10,5000,10000};
    float pl_a[5] = {0.3,0.01,0,5000,10000};

    float PID_Ini[2] = {10000,30000};


#elif Robot == 3

//                    ϵ�� ϵ������ ���ֵ
    float Fuzzy_KP_s[3] = {20,20,20};
    float Fuzzy_KI_s[3] = {0.2,10,0.2};
    float Fuzzy_KD_s[3] = {10,5,5};     //�ٶȻ�ϵ��
    
    float Fuzzy_KP_a[3] = {8,10,5};
    float Fuzzy_KI_a[3] = {0.2,0.2,1};
    float Fuzzy_KD_a[3] = {10,5,5};     //�ǶȻ�ϵ��
    
    float yawspeed[5] = {};
    float yawang[5] = {};
    
    float r_wheel[5] = {15,0.6,10,10000,16000};
    float l_wheel[5] = {15,0.6,10,10000,16000};
    
    float pl_s[5] = {4,0.5,10,5000,10000};
    float pl_a[5] = {0.3,0.01,0,5000,10000};

    float PID_Ini[2] = {10000,30000};

#endif
    
    
    