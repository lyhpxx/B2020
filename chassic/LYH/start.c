#include "start.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "chassis.h"
#include "can_task.h"
#include "repot.h"

SemaphoreHandle_t BinarySemaphore;


#define START_TASK_PRIO			1			//任务优先级
#define START_STK_SIZE 			128  		//任务堆栈大小	
TaskHandle_t StartTask_Handler;				//任务句柄


#define Chassis_TASK_PRIO			10			//任务优先级
#define Chassis_STK_SIZE 			256  		//任务堆栈大小	
TaskHandle_t ChassisTask_Handler;				//任务句柄

#define Can_TASK_PRIO			11			//任务优先级
#define Can_STK_SIZE 			256  		//任务堆栈大小	
TaskHandle_t CanTask_Handler;				//任务句柄

#define Printf_TASK_PRIO			8			//任务优先级
#define Printf_STK_SIZE 			256  		//任务堆栈大小	
TaskHandle_t PrintfTask_Handler;				//任务句柄





void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    
    
    
    xTaskCreate((TaskFunction_t )Chassis_Task,     	//任务函数
        (const char*    )"Chassis_Task",          //任务名称
        (uint16_t       )Chassis_STK_SIZE,        //任务堆栈大小
        (void*          )NULL,                  //传递给任务函数的参数
        (UBaseType_t    )Chassis_TASK_PRIO,       //任务优先级
        (TaskHandle_t*  )&ChassisTask_Handler);   //任务句柄
    xTaskCreate((TaskFunction_t )CAN_Send_Clude_Task,     	//任务函数
        (const char*    )"CAN_Send_Clude_Task",          //任务名称
        (uint16_t       )Can_STK_SIZE,        //任务堆栈大小
        (void*          )NULL,                  //传递给任务函数的参数
        (UBaseType_t    )Can_TASK_PRIO,       //任务优先级
        (TaskHandle_t*  )&CanTask_Handler);   //任务句柄
    xTaskCreate((TaskFunction_t )Printf_Task,     	//任务函数
        (const char*    )"Printf_Task",          //任务名称
        (uint16_t       )Printf_STK_SIZE,        //任务堆栈大小
        (void*          )NULL,                  //传递给任务函数的参数
        (UBaseType_t    )Printf_TASK_PRIO,       //任务优先级
        (TaskHandle_t*  )&PrintfTask_Handler);   //任务句柄
        
        


    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}


void startTast(void)
{  

    xTaskCreate((TaskFunction_t )start_task,     	//任务函数
        (const char*    )"start_task",          //任务名称
        (uint16_t       )START_STK_SIZE,        //任务堆栈大小
        (void*          )NULL,                  //传递给任务函数的参数
        (UBaseType_t    )START_TASK_PRIO,       //任务优先级
        (TaskHandle_t*  )&StartTask_Handler);   //任务句柄      		
}


