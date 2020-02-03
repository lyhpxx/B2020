#include "repot.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "message.h"
#include "mpu6050_driver.h"

void Printf_Task(void *pvParameters)
{
    TickType_t weak_time;
    const TickType_t Frequency = 5;
    weak_time = xTaskGetTickCount();
    while(1)
    {
        printf("%d\r\n",X_X.remot->res);
//        Data_sends(X_X.gyro_yaw->temp_yaw_angle,X_X.set_yaw,0,0,0,0);
//        Data_sends(X_X.set_pitch,X_X.Pitch.ecd,X_X.set_yaw,X_X.gyro_yaw.temp_yaw_angle,0,0);
//        printf("%d      %d\r\n",X_X.Yaw->ecd,X_X.Yaw->ecd_num);
        vTaskDelayUntil(&weak_time,Frequency);
    }
}

void Data_sends(int16_t data1,int16_t data2,int16_t data3,int16_t data4,int16_t data5,int16_t data6)
{
		uint8_t _cnt = 0;
		int8_t i = 0;
		uint8_t sum = 0;
        u8 testdataosend[30];
		testdataosend[_cnt++] = 0xAA;
		testdataosend[_cnt++] = 0x30;
		testdataosend[_cnt++] = 0xAF;
		testdataosend[_cnt++] = 0x02;
	
		testdataosend[_cnt++] = 0;
		testdataosend[_cnt++] = (uint8_t)(data1 >> 8);
		testdataosend[_cnt++] = (uint8_t) data1;
	
		testdataosend[_cnt++] = (uint8_t)(data2 >> 8);
		testdataosend[_cnt++] = (uint8_t) data2;
	
		testdataosend[_cnt++] = (uint8_t)(data3 >> 8);
		testdataosend[_cnt++] = (uint8_t) data3;
	
		testdataosend[_cnt++] = (uint8_t)(data4 >> 8);
		testdataosend[_cnt++] = (uint8_t) data4;
	
		testdataosend[_cnt++] = (uint8_t)(data5 >> 8);
		testdataosend[_cnt++] = (uint8_t) data5;
	
		testdataosend[_cnt++] = (uint8_t)(data6 >> 8);
		testdataosend[_cnt++] = (uint8_t) data6;	
		testdataosend[4] = _cnt - 5;
		for ( i = 0; i < _cnt; i++)
		{
				sum += testdataosend[i];	
		}
		testdataosend[_cnt++] = sum;
		for (uint8_t k = 0; k < _cnt; k++)
		{
				while ((USART1->SR&0x40) == 0);
				USART1->DR= testdataosend[k];	
		}
}

