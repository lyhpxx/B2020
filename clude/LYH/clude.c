#include "clude.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pid.h"
#include "message.h"
#include "condition.h"
#include "can_task.h"


#define DEBUG_MODE              2 //1,加速度归零，2，加速度不归零

void Clude_Task(void *pvParameters)
{
    TickType_t weak_time;
    const TickType_t Frequency = 5;
    weak_time = xTaskGetTickCount();
    
    X_X.set_pitch = PITCH_Init_Position;
    X_X.pitch_add = 0;
    X_X.wheel_speed = 5000;
    Clude_PID_Init();
    
    while(1)
    {
        MPU6050_ReadData();
        X_X.mpu = get_gyr();
        X_X.Pitch = get_PIT();
        X_X.Yaw = get_YAW();
        X_X.left_wheel = get_LE_W();
        X_X.right_wheel = get_RI_W();
        X_X.pluck = get_PL();
        X_X.gyro_yaw = get_ANG();
        X_X.remot = get_DBUS_REMOT();
        X_X.key = get_DBUS_KEY();
        
        RC_CTRL_Test();
        
        Clude_PID_Cal();
        
        Surve_Send(X_X.pitch_speed.MOTO.out,0,X_X.yaw_speed.out,0);
        
        Bullet_Send(0,X_X.right_wheel_speed.out,X_X.left_wheel_speed.out,X_X.pluck_speed.out);	
        
        vTaskDelayUntil(&weak_time,Frequency);
    }
}

void Clude_PID_Init(void)
{
    PID_Init(&X_X.yaw_speed,yawspeed[0],yawspeed[1],yawspeed[2],yawspeed[3],yawspeed[4]);
    PID_Init(&X_X.yaw_ang,yawang[0],yawang[1],yawang[2],yawang[3],yawang[4]);
    
    PID_Init(&X_X.right_wheel_speed,r_wheel[0],r_wheel[1],r_wheel[2],r_wheel[3],r_wheel[4]);
    PID_Init(&X_X.left_wheel_speed,l_wheel[0],l_wheel[1],l_wheel[2],l_wheel[3],l_wheel[4]);
    
    PID_Init(&X_X.pluck_speed,pl_s[0],pl_s[1],pl_s[2],pl_s[3],pl_s[4]);
    PID_Init(&X_X.pluck_ang,pl_a[0],pl_a[1],pl_a[2],pl_a[3],pl_a[4]);
    
    FUZZY_PID_Init(&X_X.pitch_speed,Fuzzy_KP_s,Fuzzy_KI_s,Fuzzy_KD_s,PID_Ini,-5000,5000);
    FUZZY_PID_Init(&X_X.pitch_ang,Fuzzy_KP_a,Fuzzy_KI_a,Fuzzy_KD_a,PID_Ini,-1555,1555);
}

void Clude_PID_Cal(void)
{
    X_X.set_pitch = PITCH_Init_Position + X_X.pitch_add;
    if(X_X.set_pitch >= MAX_PITCH_ECD)
    {
        X_X.set_pitch = MAX_PITCH_ECD;
    }
    else if(X_X.set_pitch <= MIN_PITCH_ECD)
    {
        X_X.set_pitch = MIN_PITCH_ECD;
    }
    if(X_X.Pitch->ecd >= 400 && X_X.Pitch->ecd <= 2200)
    {
        #if DEBUG_MODE == 1
        
        accelerate(X_X.set_pitch,&X_X.pitch_ang.MOTO.AD.trans,X_X.pitch_ang.MOTO.AD.add,X_X.pitch_ang.MOTO.AD.dec);
        
        #else
        
        accelerate(X_X.set_pitch,&X_X.pitch_ang.MOTO.AD.trans,20,20);
        
        #endif
        
        FuzzyComputation(&X_X.pitch_ang,X_X.pitch_ang.MOTO.AD.trans,X_X.Pitch->ecd);
        
        #if DEBUG_MODE == 1
        
        accelerate(X_X.pitch_ang.MOTO.out,&X_X.pitch_speed.MOTO.AD.trans,X_X.pitch_speed.MOTO.AD.add,X_X.pitch_speed.MOTO.AD.dec);
        
        #else
        
        accelerate(X_X.pitch_ang.MOTO.out,&X_X.pitch_speed.MOTO.AD.trans,300,300);
        
        #endif
        
        FuzzyComputation(&X_X.pitch_speed,X_X.pitch_speed.MOTO.AD.trans,MPU6050_Real_Data.Gyro_X); //pitch
    }
    
    #if DEBUG_MODE == 1
    
    accelerate(X_X.set_yaw,&X_X.yaw_ang.AD.trans,X_X.yaw_ang.AD.add,X_X.yaw_ang.AD.dec);
    
    #else
    
//    accelerate(X_X.set_yaw,&X_X.yaw_ang.AD.trans,30,20);
    
    #endif
    
    PID_Calculat(&X_X.yaw_ang,X_X.gyro_yaw->temp_yaw_angle,X_X.set_yaw);
    
    #if DEBUG_MODE == 1
    
    accelerate(X_X.yaw_ang.out,&X_X.yaw_speed.AD.trans,X_X.yaw_speed.AD.add,X_X.yaw_speed.AD.dec);
    
    #else
    
//    accelerate(X_X.yaw_ang.out,&X_X.yaw_speed.AD.trans,200,200);
    
    #endif
    
    PID_Calculat(&X_X.yaw_speed,X_X.gyro_yaw->Angular_velocity,X_X.yaw_ang.out);  //yaw
    
    PID_Calculat(&X_X.pluck_ang,X_X.pluck->ecd_num,X_X.set_pluck);
    
    PID_Calculat(&X_X.pluck_speed,X_X.pluck->speed,X_X.pluck_ang.out); //pluck
    
    PID_Calculat(&X_X.left_wheel_speed,X_X.left_wheel->speed,X_X.wheel_speed);
    
    PID_Calculat(&X_X.right_wheel_speed,X_X.right_wheel->speed,-X_X.wheel_speed); //wheel
}

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


void REMOT(void)
{
    if(X_X.remot->rc.ch2 > 0)
    {
        X_X.set_yaw -= 50;
    }
    else if(X_X.remot->rc.ch2 < 0)
    {
        X_X.set_yaw += 50;
    }
    if(X_X.remot->rc.ch3 > 0)
    {
        X_X.pitch_add  -= 10;
        if(X_X.pitch_add <= -(MAX_PITCH_ECD - MIN_PITCH_ECD)/2)
        {
            X_X.pitch_add = -(MAX_PITCH_ECD - MIN_PITCH_ECD)/2;
        }
    }
    else if(X_X.remot->rc.ch3 < 0)
    {
        X_X.pitch_add += 10;
        if(X_X.pitch_add >= (MAX_PITCH_ECD - MIN_PITCH_ECD)/2)
        {
            X_X.pitch_add = (MAX_PITCH_ECD - MIN_PITCH_ECD)/2;
        }
    }

}

void KEYS(void)
{
    if(X_X.key->mouse.x > 0)
    {
        X_X.set_yaw -= 50;
    }
    else if(X_X.key->mouse.x < 0)
    {
        X_X.set_yaw += 50;
    }
    if(X_X.key->mouse.y < 0)
    {
        X_X.pitch_add  -= 10;
        if(X_X.pitch_add <= -(MAX_PITCH_ECD - MIN_PITCH_ECD)/2)
        {
            X_X.pitch_add = -(MAX_PITCH_ECD - MIN_PITCH_ECD)/2;
        }
    }
    else if(X_X.key->mouse.y > 0)
    {
        X_X.pitch_add += 10;
        if(X_X.pitch_add >= (MAX_PITCH_ECD - MIN_PITCH_ECD)/2)
        {
            X_X.pitch_add = (MAX_PITCH_ECD - MIN_PITCH_ECD)/2;
        }
    }
}

void Dead(void)
{
    X_X.wheel_speed_level = Dead_speed;
    X_X.pitch_add = (MAX_PITCH_ECD - MIN_PITCH_ECD)/2;
}

void REMOT_KEY(void)
{
    if(X_X.remot->rc.ch2 > 0 || X_X.key->mouse.x > 0)
    {
        X_X.set_yaw -= 50;
    }
    else if(X_X.remot->rc.ch2 < 0 || X_X.key->mouse.x < 0)
    {
        X_X.set_yaw += 50;
    }
    if(X_X.remot->rc.ch3 > 0 || X_X.key->mouse.y < 0)
    {
        X_X.pitch_add  -= 10;
        if(X_X.pitch_add <= -(MAX_PITCH_ECD - MIN_PITCH_ECD)/2)
        {
            X_X.pitch_add = -(MAX_PITCH_ECD - MIN_PITCH_ECD)/2;
        }
    }
    else if(X_X.remot->rc.ch3 < 0 || X_X.key->mouse.y > 0)
    {
        X_X.pitch_add += 10;
        if(X_X.pitch_add >= (MAX_PITCH_ECD - MIN_PITCH_ECD)/2)
        {
            X_X.pitch_add = (MAX_PITCH_ECD - MIN_PITCH_ECD)/2;
        }
    }
}


void RC_CTRL_Test(void)
{
    static u8 a = 0,r = 0,k = 0;
    if(a == 0)
    {
        if(r == 0)
        {
            if(X_X.key->mouse.l == 1)
            {
                r = 1;
                a = 1;
            }
        }
        if(k == 0)
        {
            if(X_X.remot->res > 1024)
            {
                k = 1;
                a = 1;
            }
        }
    }
    else
    {
        if(r == 1)
        {
            if(X_X.key->mouse.l == 0)
            {
                r = 0;
                a = 0;
                X_X.set_pluck += 32764;
            }
        }
        if(k == 1)
        {
            if(X_X.remot->res == 1024)
            {
                k = 0;
                a = 0;
                X_X.set_pluck += 32764;
            }
        }
    }
    switch(X_X.wheel_speed_level)
    {
        case High_speed     : X_X.wheel_speed = 7000;       break;
        case Model_speed    : X_X.wheel_speed = 6000;       break;
        case Low_speed      : X_X.wheel_speed = 5000;       break;
        case Dead_speed     : X_X.wheel_speed = 0;          break;
        default             :                               break;
    }
    switch(X_X.MODE)
    {
        case robo_follow + robo_remote      :   REMOT();    break;
        case robo_follow + robo_mouse_key   :   KEYS();     break;
        case robo_follow + robo_ctrl_all    :   REMOT_KEY();break;
            
        case robo_roll + robo_remote        :   REMOT();    break;
        case robo_roll + robo_mouse_key     :   KEYS();     break;
        case robo_roll + robo_ctrl_all      :   REMOT_KEY();break;
            
        case robo_death + robo_remote       :   Dead();     break;
        case robo_death + robo_mouse_key    :   Dead();     break;
        case robo_death + robo_ctrl_all     :   Dead();     break;
        default                             :               break;
    }
}
