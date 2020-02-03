#include "FreeRTOS_task.h"
#include "main.h"



//	vTaskSuspend(Task1Task_Handler);//��������1			������Ϊ������
//	printf("��������1������!\r\n");
			
//	vTaskResume(Task1Task_Handler);	//�ָ�����1			������Ϊ������
//	printf("�ָ�����1������!\r\n");
			

void FreeRTOS_Task()
{
	
	/*������ʼ����*/
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}
/*��ʼ����������*/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
	

    //����KS103����
    xTaskCreate((TaskFunction_t )ks103_task,             
                (const char*    )"ks103_task",           
                (uint16_t       )KS103_STK_SIZE,                                                                                                               
                (void*          )NULL,                  
                (UBaseType_t    )KS103_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   
				
    //����Vl53l0X����
    xTaskCreate((TaskFunction_t )vl53l0x_task,     
                (const char*    )"vl53l0x_task",   
                (uint16_t       )Vl53l0X_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Vl53l0X_TASK_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler); 
				
	//�����жϲ�������
    xTaskCreate((TaskFunction_t )interrupt_task,  			
                (const char*    )"interrupt_task", 			
                (uint16_t       )INTERRUPT_STK_SIZE,		
                (void*          )NULL,						
                (UBaseType_t    )INTERRUPT_TASK_PRIO,		
                (TaskHandle_t*  )&INTERRUPTTask_Handler); 	
				
	/*		����QUERY����						�ú������ڶ�ȡ������Ϣ					�ú�������ʽ���������޷�ִ��		*/													
//    xTaskCreate((TaskFunction_t )query_task,     
//                (const char*    )"query_task",   
//                (uint16_t       )QUERY_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )QUERY_TASK_PRIO,
//                (TaskHandle_t*  )&QueryTask_Handler); 
	
	
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


//ks103������
void ks103_task (void *pvParameters)
{
	int range;
	while(1)
	{
			/*	��������ʼ��	*/
		KS103_WriteOneByte(0Xd0,0X02,0Xb0); 
		delay_ms(20);															//80
		range = KS103_ReadOneByte(0xd0, 0x02);
		range <<= 8;
		range += KS103_ReadOneByte(0xd0, 0x03);
		printf("���룺%d \r\n",range);
		delay_ms(20);						
	}
}

//vl53l0x������
void vl53l0x_task(void *pvParameters)
{
	while(1)
	{
			vl53l0x_test();					//vl53l0x����
	}
}

//�жϲ���������
/*
	�ú�����Ŀ���ǣ����ڴ򿪺͹ر� ��ѡ �жϣ���"FreertosConfig.h"����ļ���
	���С�configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY��
	����궨�����ϵͳ�����������ж����ȼ���
*/
void interrupt_task(void *pvParameters)
{
	static u32 total_num=0;
    while(1)
    {
		total_num+=1;
		if(total_num==5) 
		{
			printf("�ر��ж�.............\r\n");
			portDISABLE_INTERRUPTS();				//�ر��жϺ���
			
			delay_xms(5000);						//��ʱ5s
			
			printf("���ж�.............\r\n");		
			portENABLE_INTERRUPTS();				//���жϺ���
		}
        vTaskDelay(1000);
    }
}

/*��ȡ������Ϣ��������Ҫ���ڻ�ȡʣ����ջ��С���Դ˾�������ʵ�������ջ��С*/
void query_task(void *pvParameters)
{
	
	//����vTaskGetInfo()��ʹ��
	TaskHandle_t TaskHandle;	
	TaskStatus_t TaskStatus;
	
	printf("/************  ����vTaskGetInfo()��ʹ��  **************/\r\n");
	
	TaskHandle=xTaskGetHandle("ks103_task");										//������������ȡ��������
	
	//��ȡks103_task��������Ϣ
	vTaskGetInfo((TaskHandle_t	)TaskHandle, 										//������
				 (TaskStatus_t*	)&TaskStatus, 										//������Ϣ�ṹ��
				 (BaseType_t	)pdTRUE,											//����ͳ�������ջ��ʷ��Сʣ���С
			     (eTaskState	)eInvalid);											//�����Լ���ȡ��������׳̬
	//ͨ�����ڴ�ӡ��ָ��������й���Ϣ��
	printf("������:                %s\r\n",TaskStatus.pcTaskName);
	printf("������:              %d\r\n",(int)TaskStatus.xTaskNumber);
	printf("����׳̬:              %d\r\n",TaskStatus.eCurrentState);
	printf("����ǰ���ȼ�:        %d\r\n",(int)TaskStatus.uxCurrentPriority);
	printf("��������ȼ�:          %d\r\n",(int)TaskStatus.uxBasePriority);
	printf("�����ջ����ַ:        %#x\r\n",(int)TaskStatus.pxStackBase);
	printf("�����ջ��ʷʣ����Сֵ:%d\r\n",TaskStatus.usStackHighWaterMark);
	printf("/**************************����***************************/\r\n");
	
}





