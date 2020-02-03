#include "xiaohei.h"


void get_gyro_measuer(gyro_measure_t *Jxun,CanRxMsg *msg)		//小黑块数据
{
	Jxun->temp_yaw_angle = (int32_t)(msg->Data[0]<<24)|(int32_t)(msg->Data[1]<<16)
														| (int32_t)(msg->Data[2]<<8) | (int32_t)(msg->Data[3]);
	Jxun->Last_yaw_angle = Jxun->This;
	Jxun->This = (rmf)(Jxun->temp_yaw_angle*0.01);
	Jxun->Angular_velocity = 1600*(Jxun->This - Jxun->Last_yaw_angle);
	Jxun->yaw_angle_nums += (Jxun->This- Jxun->Last_yaw_angle)*100 ;
}



void Motor_YT_Can1TX(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4 )	//输入速度
{
	CanTxMsg tx_message;
	
	tx_message.StdId = 0x1ff;		  			//标准标识符
	tx_message.Data[0] = (uint8_t)(Vel_1 >> 8);
	tx_message.Data[1] = (uint8_t) Vel_1;
	tx_message.Data[2] = (uint8_t)(Vel_2 >> 8);
	tx_message.Data[3] = (uint8_t) Vel_2;
	tx_message.Data[4] = (uint8_t)(Vel_3>> 8);
	tx_message.Data[5] = (uint8_t) Vel_3;
	tx_message.Data[6] = (uint8_t)(Vel_4 >> 8);
	tx_message.Data[7] = (uint8_t) Vel_4;

	tx_message.ExtId	= CAN_Id_Standard ;		//拓展标识符
	tx_message.IDE		= CAN_ID_STD;			//用来设定消息标识符的类型    
	tx_message.RTR		= CAN_RTR_DATA;			//用来设定待传输消息的帧类型	数据帧或远程帧
	tx_message.DLC		= 8;					//用来设定待传输消息的帧长度 0-8

	CAN_Transmit(CAN1,&tx_message);
}


