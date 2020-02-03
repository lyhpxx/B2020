#include "message.h"


ALL X_X;

moto_message MOTO_1;
moto_message MOTO_2;
moto_message MOTO_3;
moto_message MOTO_4;
moto_message MOTO_GM;

DBUS rc_ctr;
int ecd_n;
float sita;

const moto_message * first(void)
{
    return &MOTO_1;
}
const moto_message * second(void)
{
    return &MOTO_2;
}
const moto_message * third(void)
{
    return &MOTO_3;
}
const moto_message * fourth(void)
{
    return &MOTO_4;
}
const DBUS * RC_CTRL(void)
{
    return &rc_ctr;
}
const int * get_num(void)
{
    return &ecd_n;
}
const float * get_sita(void)
{
    return &sita;
}
const moto_message * get_GM(void)
{
    return &MOTO_GM;
}

//电机数据处理函数
void MOTO_Message(moto_message * mot,CanRxMsg * mes)
{
    mot->last_ecd = mot->ecd;
    mot->ecd = (mes->Data[0]<<8 ) | mes->Data[1];
    mot->speed = (mes->Data[2]<<8 ) | mes->Data[3];
    mot->current = (mes->Data[4]<<8 ) | mes->Data[5];
    mot->temp = mes->Data[6];
    
    mot->ecd_speed = mot->ecd - mot->last_ecd;
    
    if(mot->ecd_speed > 5000)
    {
        mot->ecd_speed -= 8192;
    }
    if(mot->ecd_speed  < -5000)
    {
        mot->ecd_speed += 8192;
    }
    mot->ecd_num += mot->ecd_speed;
}


//小黑块数据
void get_gyro_measuer(gyro_measure_t *Jxun,CanRxMsg *msg)		
{
	Jxun->temp_yaw_angle = (int32_t)(msg->Data[0]<<24)|(int32_t)(msg->Data[1]<<16)
														| (int32_t)(msg->Data[2]<<8) | (int32_t)(msg->Data[3]);
	Jxun->Last_yaw_angle = Jxun->This;
	Jxun->This = (rmf)(Jxun->temp_yaw_angle*0.01);
	Jxun->Angular_velocity = 1600*(Jxun->This - Jxun->Last_yaw_angle);
	Jxun->yaw_angle_nums += (Jxun->This- Jxun->Last_yaw_angle)*100 ;
}

void get_power(Power * pow,CanRxMsg *msg)
{
    pow->current = ((msg->Data[0]<<8) | msg->Data[1]);
    pow->votlage = ((msg->Data[2]<<8) | msg->Data[3])/1000;
    
    pow->power = pow->current * pow->votlage;
}

void GM6020(moto_message * mot,CanRxMsg *msg)
{
    if(mot->flag == 0)
    {
        mot->first = (msg->Data[0]<<8 ) | msg->Data[1];        
        if(mot->first != 0)
        {
            mot->ecd = mot->first;
            mot->flag = 1;
        }

    }
    if(mot->flag == 1)
    {
        mot->last_ecd = mot->ecd;
        mot->ecd = (msg->Data[0]<<8 ) | msg->Data[1];
        mot->ecd_speed = mot->ecd - mot->last_ecd;
            
        if(mot->ecd_speed > 5000)
        {
            mot->ecd_speed -= 8192;
        }
        if(mot->ecd_speed  < -5000)
        {
            mot->ecd_speed += 8192;
        }
        mot->ecd_num += mot->ecd_speed;
    }
    
}

