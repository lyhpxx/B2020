#include "chassis.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "condition.h"
#include "pid.h"
#include "can_task.h"
#include "dbus.h"
#include "usart2.h"

#define pi          (3.1415f)

void Chassis_Task(void *pvParameters)
{
    TickType_t weak_time;
    const TickType_t Frequency = 3;
    weak_time = xTaskGetTickCount();
    
    Chassis_PID_Init();
        
    while(1)
    {
        X_X.moto1_message = first();
        X_X.moto2_message = second();
        X_X.moto3_message = third();
        X_X.moto4_message = fourth();
        X_X.rc = RC_CTRL();
        X_X.GM_6020_Point_ECD_NUM = get_num();
        X_X.GM6020_sita_Point = get_sita();
        X_X.GM = get_GM();
        
        MODE_Choose();
        
        MODE_ENABLE();
        
        Chassis_Calculate(X_X.set_Vx,X_X.set_Vy,X_X.set_Vr);
        
        Chassic_Power_Limit();
        if(X_X.Robot_MODE == robo_follow)
        {
            accelerate(X_X.motor1.set,&X_X.motor1.ADD.Trans,20,50);
            accelerate(X_X.motor2.set,&X_X.motor2.ADD.Trans,20,50);
            accelerate(X_X.motor3.set,&X_X.motor3.ADD.Trans,20,50);
            accelerate(X_X.motor4.set,&X_X.motor4.ADD.Trans,20,50);
                
            Chassis_PID_Calculate(X_X.motor1.ADD.Trans,X_X.motor2.ADD.Trans,X_X.motor3.ADD.Trans,X_X.motor4.ADD.Trans);
        }
        else
        {
            Chassis_PID_Calculate(X_X.motor1.set,X_X.motor2.set,X_X.motor3.set,X_X.motor4.set);
        }
                  
        CAN1_Send(X_X.motor1.out,X_X.motor2.out,X_X.motor3.out,X_X.motor4.out);
        
        vTaskDelayUntil(&weak_time,Frequency);
    }
}


void Chassis_PID_Init(void)
{
    PID_Init(&X_X.motor1,moto1_pid[0],moto1_pid[1],moto1_pid[2],moto1_pid[3],moto1_pid[4]);
    PID_Init(&X_X.motor2,moto2_pid[0],moto2_pid[1],moto2_pid[2],moto2_pid[3],moto2_pid[4]);
    PID_Init(&X_X.motor3,moto3_pid[0],moto3_pid[1],moto3_pid[2],moto3_pid[3],moto3_pid[4]);
    PID_Init(&X_X.motor4,moto4_pid[0],moto4_pid[1],moto4_pid[2],moto4_pid[3],moto4_pid[4]);
}


void Chassis_Calculate(float vx,float vy,float roll)
{
    X_X.motor1.set = (vx + vy + roll);
    X_X.motor2.set = (vx - vy + roll);
    X_X.motor3.set = (-vx + vy + roll);
    X_X.motor4.set = (-vx - vy + roll);
}

void Chassis_PID_Calculate(float t1,float t2,float t3,float t4)
{
    PID_Calculat(&X_X.motor1,X_X.moto1_message->speed,t1);
    PID_Calculat(&X_X.motor2,X_X.moto2_message->speed,t2);
    PID_Calculat(&X_X.motor3,X_X.moto3_message->speed,t3);
    PID_Calculat(&X_X.motor4,X_X.moto4_message->speed,t4);
}

/**
  * @brief  加速度处理运算
  * @param  setspeed	 --目标速度
			speed_transit--速度环的设定速度
			acc			 --加速度			dec--减速度
  * @retval void
  * @note
**/	
void accelerate(float setspeed,float *speed_transit,int16_t acc,int16_t  dec)
{

	if ((*speed_transit > 0) || (*speed_transit == 0 && setspeed >= 0))
	{
		if ((setspeed - *speed_transit > acc) && (setspeed > *speed_transit))	//目标速度大于当前速度 加速
		{
			*speed_transit	+= acc;
		}
		else if (( *speed_transit - setspeed > dec) && (setspeed < *speed_transit))//目标速度小于当前速度 减速
		{
			*speed_transit	-= dec;
		}
		else
		{
			*speed_transit	 = setspeed;
		}	
		
		if (*speed_transit < 0) 
		{
			*speed_transit = 0;
		}
	}
	else
	{
		if (( *speed_transit - setspeed > acc) && (setspeed < *speed_transit))
		{
			*speed_transit	-= acc;
		}
		else if ((setspeed - *speed_transit > dec) && (setspeed > *speed_transit))	
		{
			*speed_transit	+= dec;
		}
		else
		{
			*speed_transit	 = setspeed;
		}	
		
		if (*speed_transit > 0) 
		{
			*speed_transit = 0;
		}
	}
}


void MODE_Choose(void)
{
    if(X_X.rc->rc.s1 == 1)
    {
        X_X.Robot_MODE = robo_follow;
    }
    else if(X_X.rc->rc.s1 == 3)
    {
        X_X.Robot_MODE = robo_roll;
    }
    else if(X_X.rc->rc.s1 == 2)
    {
        X_X.Robot_MODE = robo_death;
    }
    
    if(X_X.rc->rc.s2 == 1)
    {
        X_X.ctrl_mode =  robo_ctrl_all;
    }
    else if(X_X.rc->rc.s2 == 3)
    {
        X_X.ctrl_mode = robo_remote;
    }
    else if(X_X.rc->rc.s2 == 2)
    {
        X_X.ctrl_mode = robo_mouse_key;
    }
}

void MODE_ENABLE(void)
{
    float Vx = 0;
    float Vy = 0;
    switch(X_X.ctrl_mode)
    {
        case robo_remote    : Vx = X_X.rc->rc.ch0 * 50;Vy = X_X.rc->rc.ch1 * 50;break;
        case robo_mouse_key : Vx = (KEY_D - KEY_A) * 10000;Vy = (KEY_W - KEY_S) * 10000;break;
        case robo_ctrl_all  : Vx = X_X.rc->rc.ch0 * 50 + (KEY_D - KEY_A) * 10000;\
                              Vy = X_X.rc->rc.ch1 * 50 + (KEY_W - KEY_S) * 10000;           break;
    }
    switch(X_X.Robot_MODE)
    {
        case robo_follow    : X_X.set_Vx = Vx;X_X.set_Vy = Vy;
                              X_X.wheel_spee = Model_speed; if(*X_X.GM_6020_Point_ECD_NUM != 0)X_X.set_Vr = YAW_Init_Position - *X_X.GM_6020_Point_ECD_NUM/*Follow(YAW_Init_Position,*X_X.GM_6020_Point_ECD_NUM)*/;else X_X.set_Vr = 0;break;
        case robo_roll      : X_X.wheel_spee = Model_speed;
                              X_X.set_Vx = ((Vx / 10) * cos(*X_X.GM6020_sita_Point)) + ((Vy / 10) * sin(-*X_X.GM6020_sita_Point));\
                              X_X.set_Vy = ((Vx / 10) * sin(*X_X.GM6020_sita_Point)) + ((Vy / 10) * cos(*X_X.GM6020_sita_Point));
                              X_X.set_Vr = 2000;break;
        case robo_death     : X_X.set_Vr = 0;
                              X_X.set_Vx = 0;
                              X_X.set_Vy = 0;
                              X_X.wheel_spee = Dead_speed; break;
    }
}







/*************************************************限功率*************************************************************/



float Power_PID(float Tar,float Now)			//功率PID
{
    float Kp = 8.0f;
    float Ki = 1.0f;
    float Kd = 8.0f;
    float Ek;
    float PidOut;
    static float Ek_1;
    static float EkSum;
    static float Ek_2;
    static float Out;

    Ek = Tar - Now;
    EkSum+=Ek;
    if(EkSum >= 10000)
    {
        EkSum = 10000;
    }
    if(EkSum <= -10000)
    {
        EkSum = -10000;
    }
    PidOut = Kp*(Ek - Ek_1) + Ki*Ek + Kd*(Ek - 2*Ek_1 + Ek_2);
    Ek_2 = Ek_1;
    Ek_1 = Ek;
    Out += PidOut;
    	if( Out >= 32000)  Out = 32000;
	if( Out <= 500)  Out = 500;
    return Out;
}
float SetSum;
float LimitOut;
float Scale;
void Chassic_Power_Limit(void)					//限功率
{
	X_X.pow.power = X_X.pow.current * robot_stage.chassis_volt /1000/1000;
    SetSum = fabsf(X_X.motor1.set) + fabsf(X_X.motor2.set) + fabsf(X_X.motor3.set) + fabsf(X_X.motor4.set);//轮子设定和

	if ((robot_stage.chassis_powe_buffer- (X_X.pow.power - 80.0f)*0.2f < 20.0f)||X_X.pow.power>205)  //200ms之后即将超功率，对功率进行限制对起步功率稍微限制
	{
        LimitOut = Power_PID(70,X_X.pow.power);
		SetSum = fabsf(X_X.motor1.set) + fabsf(X_X.motor2.set) + fabsf(X_X.motor3.set) + fabsf(X_X.motor4.set);//轮子设定和
		if (SetSum <= 1)
		{
			Scale = 0;
		}
		else
		{
			  Scale = LimitOut/SetSum;
		}
      
        X_X.motor1.set *= Scale;
        X_X.motor2.set *= Scale;
        X_X.motor3.set *= Scale;
        X_X.motor4.set *= Scale;
		
		if (robot_stage.chassis_powe_buffer  <= 6.0f)
        {
			X_X.motor1.set = 0;
			X_X.motor2.set = 0;
			X_X.motor3.set = 0;
			X_X.motor4.set = 0;
        }    
    }
}

/**********************************地盘跟随PID*********************************************************/
float Follow(float set,float get)           //位置式，有抖动
{
    float kp = 1;
    float ki = 0;
    float kd = 0;
    
    float ek = 0;
    static float sum_ek = 0;
    static float last_ek = 0;
    
    float out = 0;
    
    ek = set - get;
    sum_ek += ek;
    if(sum_ek >= 5000)
    {
        sum_ek = 5000;
    }
    else if(sum_ek <= -5000)
    {
        sum_ek = -5000;
    }
    out = kp * ek + ki * sum_ek + kd * (ek - last_ek);
    if(out >= 5000)
    {
        out = 5000;
    }
    else if(out <= -5000)
    {
        out = -5000;
    }
    
    last_ek = ek;
    return (out);
}

//float Follow(float set,float get)
//{
//    float kp = 80,ki = 3.2f,kd = 1;
//    float ek = 0;
//    static float last_ek = 0;
//    static float llast_ek = 0;
//    float out = 0;
//    ek = set - get;
//    
//    out = kp * (ek - last_ek) + ki * ek + kd * (ek - 2 * last_ek + llast_ek);
//    
//    if(out >= 10000)
//    {
//        out = 10000;
//    }
//    else if(out <= -10000)
//    {
//        out = -10000;
//    }
//    llast_ek = last_ek;
//    last_ek = ek;
//    return (out);
//}
