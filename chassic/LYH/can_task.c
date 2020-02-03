#include "can_task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "message.h"
#include "condition.h"


void CAN_Send_Clude_Task(void *pvParameters)
{
    TickType_t weak_time;
    const TickType_t Fuqency = 2;
    weak_time = xTaskGetTickCount();
    while(1)
    {
        CAN2_Remote(X_X.Robot_MODE + X_X.ctrl_mode,X_X.rc->rc.ch2,X_X.rc->rc.ch3,X_X.rc->res,X_X.wheel_spee);
        
        CAN2_KEYS(X_X.Robot_MODE + X_X.ctrl_mode,X_X.rc->mouse.x,X_X.rc->mouse.y,X_X.rc->key.v,X_X.rc->mouse.l + X_X.rc->mouse.r);
        
        vTaskDelayUntil(&weak_time,Fuqency);
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
            case 0x201 : MOTO_Message(&MOTO_1,&rx_message);break;
            case 0x202 : MOTO_Message(&MOTO_2,&rx_message);break;
            case 0x203 : MOTO_Message(&MOTO_3,&rx_message);break;
            case 0x204 : MOTO_Message(&MOTO_4,&rx_message);break;
            
            
            case 0x309 : get_power(&X_X.pow,&rx_message);             break;
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
            case 0x305 : GM6020(&MOTO_GM,&rx_message);
                         ecd_n = (int)fabs((double)MOTO_GM.ecd_num + 8192) % 16384;
                         sita = (ecd_n - YAW_Init_Position) * 0.000383f;
                         break;
            default : break;
        }
        
	}
}

void CAN1_Send(int16_t Vel_1,int16_t Vel_2,int16_t Vel_3,int16_t Vel_4 )	//输入速度
{
	CanTxMsg tx_message;
	
	tx_message.StdId = 0x200;		  			//标准标识符
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


//遥控器控制             模式        通道2           通道3         拨盘             速度等级
void CAN2_Remote(uint8_t MODE,int16_t ch2,int16_t ch3,int16_t res_pluck,uint8_t speed_level)	//输入速度
{
	CanTxMsg tx_message;
	
	tx_message.StdId = 0x303;		  			//标准标识符
	tx_message.Data[0] = (uint8_t)MODE;
	tx_message.Data[1] = (uint8_t)(ch2 >> 8);
	tx_message.Data[2] = (uint8_t)(ch2);
	tx_message.Data[3] = (uint8_t)(ch3 >> 8);
	tx_message.Data[4] = (uint8_t)(ch3);
	tx_message.Data[5] = (uint8_t)(res_pluck >> 8);
	tx_message.Data[6] = (uint8_t)(res_pluck);
	tx_message.Data[7] = (uint8_t)(speed_level);

	tx_message.ExtId	= CAN_Id_Standard ;		//拓展标识符
	tx_message.IDE		= CAN_ID_STD;			//用来设定消息标识符的类型    
	tx_message.RTR		= CAN_RTR_DATA;			//用来设定待传输消息的帧类型	数据帧或远程帧
	tx_message.DLC		= 8;					//用来设定待传输消息的帧长度 0-8

	CAN_Transmit(CAN2,&tx_message);
}


//键盘鼠标控制          模式            鼠标x坐标      鼠标y坐标  键盘按键     鼠标左右键
void CAN2_KEYS(uint8_t MODE,int16_t mouse_x,int16_t mouse_y,int16_t key,uint8_t mou)
{
    CanTxMsg tx_message;
	
	tx_message.StdId = 0x304;		  			//标准标识符
	tx_message.Data[0] = (uint8_t)MODE;
	tx_message.Data[1] = (uint8_t)(mouse_x >> 8);
	tx_message.Data[2] = (uint8_t)(mouse_x);
	tx_message.Data[3] = (uint8_t)(mouse_y >> 8);
	tx_message.Data[4] = (uint8_t)(mouse_y);
	tx_message.Data[5] = (uint8_t)(key >> 8);
	tx_message.Data[6] = (uint8_t)(key);
	tx_message.Data[7] = (uint8_t)(mou);

	tx_message.ExtId	= CAN_Id_Standard ;		//拓展标识符
	tx_message.IDE		= CAN_ID_STD;			//用来设定消息标识符的类型    
	tx_message.RTR		= CAN_RTR_DATA;			//用来设定待传输消息的帧类型	数据帧或远程帧
	tx_message.DLC		= 8;					//用来设定待传输消息的帧长度 0-8

	CAN_Transmit(CAN2,&tx_message);
}


