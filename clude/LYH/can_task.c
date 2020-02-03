#include "can_task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "message.h"
#include "condition.h"


void CAN_Task(void *pvParameters)
{
    TickType_t weak_time;
    const TickType_t Fquency = 2;
    weak_time = xTaskGetTickCount();
    while(1)
    {
        CAN1_Send_Message(X_X.Yaw->ecd,0,0,0);
        vTaskDelayUntil(&weak_time,Fquency);
    }
}   

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        switch(rx_message.StdId)
        {
            case 0x206 : MOTO_Message(&ri_w,&rx_message);break;
            case 0x207 : MOTO_Message(&le_w,&rx_message);break;
            case 0x208 : MOTO_Message(&pl,&rx_message);break;
            
            case 0x209 : MOTO_Message(&pit,&rx_message);break;
            case 0x20b : MOTO_Message(&ya,&rx_message);break;
            
            case 0x401 : get_gyro_measuer(&gyr,&rx_message);break;
            
//            case 0x20c : X_X.rc_ctrl.rc.ch3 = ( rx_message.Data[0]<<8 ) | rx_message.Data[1];
//                         X_X.rc_ctrl.rc.ch2 = ( rx_message.Data[2]<<8 ) | rx_message.Data[3];
//                         X_X.rc_ctrl.mouse.l = ( rx_message.Data[4]<<8 ) | rx_message.Data[5];
//                         X_X.rc_ctrl.rc.s1 = ( rx_message.Data[6]<<8 ) | rx_message.Data[7];break;
            case 0x303 : CAN2_Receive_Remot(&X_X,&rc_remot,&rx_message);break;
            
            case 0x304 : CAN2_Receive_KEY(&X_X,&rc_key,&rx_message);break;
            

            default : break;
        }
        
	}
}


void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        switch(rx_message.StdId)
        {
            default : break;
        }
        
	}
}

//vel_1 null,vel_2 right_wheel,vel_3 left_wheel, vel_4 pluck

void Bullet_Send(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4 )	
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

//Vel_1 pitch,Vel_3 yaw
void Surve_Send(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4 )	
{
	CanTxMsg tx_message;
	
	tx_message.StdId = 0x2ff;		  			//标准标识符
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

void CAN1_Send_Message(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4)	
{
	CanTxMsg tx_message;
	
	tx_message.StdId = 0x305;		  			//标准标识符
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


