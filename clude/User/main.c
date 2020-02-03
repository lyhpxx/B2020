#include "main.h"
#include "led.h"
#include "uart4.h"
#include "usart2.h"
#include "start.h"
	/*FreeRTOS系统*/
#if SYSTEM_SUPPORT_OS					//控制FreeRTOS系统的打开和关闭
#include "FreeRTOS.h"					//支持OS时，使用	  
#include "task.h"
#endif

#include "message.h"

int main(void)
{
	
/***************************************	初始化		***********************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	
#if SYSTEM_SUPPORT_OS
	delay_init(168);                               //初始化延时函数（系统时钟频率）
#endif		
	USART1_Configuration();			//USART1	串口打印数据·蓝牙115200		测试成功	

    delay_ms(2000);	
	while(MPU6050_Initialization());			//测试成功		
	MPU6050_Gyro_calibration();							//等待（电池）电圧稳定

	CAN1_Configuration();			//CAN1		
	CAN2_Configuration();			//CAN2
    
    
    USART2_Configuration();
	USART3_Configuration();			//USART3	妙算			未测试
    UART4_Configuration();
    
    TIM2_Configuration();
    Led_Configuration();
	
	Nvic();							//存放所有中断配置，便于配置 优先级
	printf("/*-------------------- TITR-RM2019-HERO-GIMBAL-ORDER ---------------------*/\r\n");
    
    
    
/**************************************		任务开始函数		********************************************/
#if SYSTEM_SUPPORT_OS
	startTast();					//进入系统任务
	vTaskStartScheduler();          //开启任务调度
#else
	while(1){;}	
#endif						

}


