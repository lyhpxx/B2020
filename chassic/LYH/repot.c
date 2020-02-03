#include "repot.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "message.h"
#include "mpu6050_driver.h"
#include "usart2.h"

void Printf_Task(void *pvParameters)
{
    TickType_t weak_time;
    const TickType_t Frequency = 5;
    weak_time = xTaskGetTickCount();
    while(1)
    {
        printf("%d    %f\r\n",*X_X.GM_6020_Point_ECD_NUM,*X_X.GM6020_sita_Point);
//        printf("%d    %d\r\n",X_X.GM->ecd,X_X.GM->ecd_num + 8192);
//        printf("%d  %f\r\n",*X_X.GM_6020_Point_ECD_NUM,*X_X.GM6020_sita_Point);
//        printf("ecd %d buffer %d      HP %d     power %f\r\n",X_X.GM6020_ecd,chassis_powe_buffer.POWER_BUFER,blood_remaining.HP,chassis_powe.POWER);
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

