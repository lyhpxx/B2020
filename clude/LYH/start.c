#include "start.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "clude.h"
#include "repot.h"
#include "can_task.h"


SemaphoreHandle_t BinarySemaphore;


#define START_TASK_PRIO			1			//�������ȼ�
#define START_STK_SIZE 			128  		//�����ջ��С	
TaskHandle_t StartTask_Handler;				//������


#define Clude_TASK_PRIO			10			//�������ȼ�
#define Clude_STK_SIZE 			256  		//�����ջ��С	
TaskHandle_t CludeTask_Handler;				//������

#define Printf_TASK_PRIO			8			//�������ȼ�
#define Printf_STK_SIZE 			256  		//�����ջ��С	
TaskHandle_t PrintfTask_Handler;				//������

#define CAN_TASK_PRIO			11			//�������ȼ�
#define CAN_STK_SIZE 			256  		//�����ջ��С	
TaskHandle_t CANTask_Handler;				//������



void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    
    
    
    xTaskCreate((TaskFunction_t )Clude_Task,     	//������
        (const char*    )"Clude_Task",          //��������
        (uint16_t       )Clude_STK_SIZE,        //�����ջ��С
        (void*          )NULL,                  //���ݸ��������Ĳ���
        (UBaseType_t    )Clude_TASK_PRIO,       //�������ȼ�
        (TaskHandle_t*  )&CludeTask_Handler);   //������
    xTaskCreate((TaskFunction_t )Printf_Task,     	//������
        (const char*    )"Printf_Task",          //��������
        (uint16_t       )Printf_STK_SIZE,        //�����ջ��С
        (void*          )NULL,                  //���ݸ��������Ĳ���
        (UBaseType_t    )Printf_TASK_PRIO,       //�������ȼ�
        (TaskHandle_t*  )&PrintfTask_Handler);   //������
    xTaskCreate((TaskFunction_t )CAN_Task,     	//������
        (const char*    )"CAN_Task",          //��������
        (uint16_t       )CAN_STK_SIZE,        //�����ջ��С
        (void*          )NULL,                  //���ݸ��������Ĳ���
        (UBaseType_t    )CAN_TASK_PRIO,       //�������ȼ�
        (TaskHandle_t*  )&CANTask_Handler);   //������
        
        


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


