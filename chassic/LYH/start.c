#include "start.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "chassis.h"
#include "can_task.h"
#include "repot.h"

SemaphoreHandle_t BinarySemaphore;


#define START_TASK_PRIO			1			//�������ȼ�
#define START_STK_SIZE 			128  		//�����ջ��С	
TaskHandle_t StartTask_Handler;				//������


#define Chassis_TASK_PRIO			10			//�������ȼ�
#define Chassis_STK_SIZE 			256  		//�����ջ��С	
TaskHandle_t ChassisTask_Handler;				//������

#define Can_TASK_PRIO			11			//�������ȼ�
#define Can_STK_SIZE 			256  		//�����ջ��С	
TaskHandle_t CanTask_Handler;				//������

#define Printf_TASK_PRIO			8			//�������ȼ�
#define Printf_STK_SIZE 			256  		//�����ջ��С	
TaskHandle_t PrintfTask_Handler;				//������





void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    
    
    
    xTaskCreate((TaskFunction_t )Chassis_Task,     	//������
        (const char*    )"Chassis_Task",          //��������
        (uint16_t       )Chassis_STK_SIZE,        //�����ջ��С
        (void*          )NULL,                  //���ݸ��������Ĳ���
        (UBaseType_t    )Chassis_TASK_PRIO,       //�������ȼ�
        (TaskHandle_t*  )&ChassisTask_Handler);   //������
    xTaskCreate((TaskFunction_t )CAN_Send_Clude_Task,     	//������
        (const char*    )"CAN_Send_Clude_Task",          //��������
        (uint16_t       )Can_STK_SIZE,        //�����ջ��С
        (void*          )NULL,                  //���ݸ��������Ĳ���
        (UBaseType_t    )Can_TASK_PRIO,       //�������ȼ�
        (TaskHandle_t*  )&CanTask_Handler);   //������
    xTaskCreate((TaskFunction_t )Printf_Task,     	//������
        (const char*    )"Printf_Task",          //��������
        (uint16_t       )Printf_STK_SIZE,        //�����ջ��С
        (void*          )NULL,                  //���ݸ��������Ĳ���
        (UBaseType_t    )Printf_TASK_PRIO,       //�������ȼ�
        (TaskHandle_t*  )&PrintfTask_Handler);   //������
        
        


    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


void startTast(void)
{  

    xTaskCreate((TaskFunction_t )start_task,     	//������
        (const char*    )"start_task",          //��������
        (uint16_t       )START_STK_SIZE,        //�����ջ��С
        (void*          )NULL,                  //���ݸ��������Ĳ���
        (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
        (TaskHandle_t*  )&StartTask_Handler);   //������      		
}


