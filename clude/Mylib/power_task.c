#include "power_task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
//#include "Variable.h"
#include "semphr.h"  
#include "upper.h"
#include "task.h"
#include "math.h"
//#include "variable.h"
#include "usart2.h"
#include "moto.h"

extern MOTO moto;

/*功率限制pid*/
float Power_PID(float Tar,float Now)
{
    float Kp = 5.0f;
    float Ki = 1.0f;
    float Kd = 5.0f;
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

//void Chassic_Power_Limit(void)


void Chassic_Power_Limit(float set_pow)
{
	moto.Ctrl_Power.POWER = moto.ReadCurrent.CURRENT* robot_stage.chassis_volt /1000/1000;
 
    SetSum = fabsf(moto.w1) + fabsf(moto.w2) + fabsf(moto.w3) + fabsf(moto.w4);//轮子设定和
	  if((robot_stage.chassis_power_buffer- (moto.Ctrl_Power.POWER - set_pow)*0.1f < 15.0f)||moto.Ctrl_Power.POWER>250)  //200ms之后即将超功率，对功率进行限制对起步功率稍微限制
      {
        LimitOut = Power_PID(set_pow - 5,moto.Ctrl_Power.POWER);
		SetSum = fabsf(moto.w1) + fabsf(moto.w2) + fabsf(moto.w3) + fabsf(moto.w4);//轮子设定和

            Scale = LimitOut/SetSum;
            moto.w1 *= Scale;
            moto.w2 *= Scale;
            moto.w3 *= Scale;
            moto.w4 *= Scale;

        

		 if (robot_stage.chassis_power_buffer  <= 4.0f)
        {
			moto.w1 = 0;
			moto.w2 = 0;
			moto.w3 = 0;
			moto.w4 = 0;
        }    
    }
}


