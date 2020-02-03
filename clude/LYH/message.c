#include "message.h"

ALL X_X;

moto_message pit;
moto_message ya;
moto_message le_w;
moto_message ri_w;
moto_message pl;
gyro_measure_t gyr;
DBUS rc_remot;
DBUS rc_key;


const moto_message * get_PIT(void)
{
    return &pit;
}
const moto_message * get_YAW(void)
{
    return &ya;
}
const moto_message * get_LE_W(void)
{
    return &le_w;
}
const moto_message * get_RI_W(void)
{
    return &ri_w;
}
const moto_message * get_PL(void)
{
    return &pl;
}
const gyro_measure_t * get_ANG(void)
{
    return &gyr;
}
const DBUS * get_DBUS_REMOT(void)
{
    return &rc_remot;
}
const DBUS * get_DBUS_KEY(void)
{
    return &rc_key;
}
const MPU6050_REAL_DATA * get_gyr(void)
{
    return &MPU6050_Real_Data;
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
	Jxun->This = (rmf)(Jxun->temp_yaw_angle*0.1);
	Jxun->Angular_velocity = 1600*(Jxun->This - Jxun->Last_yaw_angle);
	Jxun->yaw_angle_nums += (Jxun->This- Jxun->Last_yaw_angle)*100 ;
}

void CAN2_Receive_Remot(ALL * all,DBUS * db,CanRxMsg *msg)
{
    all->MODE = msg->Data[0];
    db->rc.ch2 = (msg->Data[1] << 8 ) | msg->Data[2];
    db->rc.ch3 = (msg->Data[3] << 8 ) | msg->Data[4];
    db->res = (msg->Data[5] << 8 ) | msg->Data[6];
    all->wheel_speed_level = msg->Data[7];
}

void CAN2_Receive_KEY(ALL * all,DBUS * db,CanRxMsg *msg)
{
    all->MODE = msg->Data[0];
    db->mouse.x = (msg->Data[1] << 8 ) | msg->Data[2];
    db->mouse.y = (msg->Data[3] << 8 ) | msg->Data[4];
    db->key.v = (msg->Data[5] << 8 ) | msg->Data[6];
    db->mouse.l = msg->Data[7];
}



