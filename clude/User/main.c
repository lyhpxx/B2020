#include "main.h"
#include "led.h"
#include "uart4.h"
#include "usart2.h"
#include "start.h"
	/*FreeRTOSϵͳ*/
#if SYSTEM_SUPPORT_OS					//����FreeRTOSϵͳ�Ĵ򿪺͹ر�
#include "FreeRTOS.h"					//֧��OSʱ��ʹ��	  
#include "task.h"
#endif

#include "message.h"

int main(void)
{
	
/***************************************	��ʼ��		***********************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	
#if SYSTEM_SUPPORT_OS
	delay_init(168);                               //��ʼ����ʱ������ϵͳʱ��Ƶ�ʣ�
#endif		
	USART1_Configuration();			//USART1	���ڴ�ӡ���ݡ�����115200		���Գɹ�	

    delay_ms(2000);	
	while(MPU6050_Initialization());			//���Գɹ�		
	MPU6050_Gyro_calibration();							//�ȴ�����أ���R�ȶ�

	CAN1_Configuration();			//CAN1		
	CAN2_Configuration();			//CAN2
    
    
    USART2_Configuration();
	USART3_Configuration();			//USART3	����			δ����
    UART4_Configuration();
    
    TIM2_Configuration();
    Led_Configuration();
	
	Nvic();							//��������ж����ã��������� ���ȼ�
	printf("/*-------------------- TITR-RM2019-HERO-GIMBAL-ORDER ---------------------*/\r\n");
    
    
    
/**************************************		����ʼ����		********************************************/
#if SYSTEM_SUPPORT_OS
	startTast();					//����ϵͳ����
	vTaskStartScheduler();          //�����������
#else
	while(1){;}	
#endif						

}


