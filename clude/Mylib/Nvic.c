#include "Nvic.h"



/***********************		�ж����ȼ��趨		*****************************/
void Nvic()
{
	NVIC_InitTypeDef 		nvic;
	
	// ����2	�����жϣ�����   ���� ����ϵͳ����
	nvic.NVIC_IRQChannel 						= 	USART2_IRQn;		//����2�ж�ͨ��
    nvic.NVIC_IRQChannelPreemptionPriority 		= 	5;					//��ռ���ȼ�3
    nvic.NVIC_IRQChannelSubPriority 			= 	0;					//�����ȼ�2
    nvic.NVIC_IRQChannelCmd						= 	ENABLE;				//IRQͨ��ʹ��
    NVIC_Init(&nvic);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
//	// DMA1	������6	�жϣ����ڸ�  ����ϵͳ ���� ����
//	nvic.NVIC_IRQChannel           				= 	DMA1_Stream6_IRQn;
//	nvic.NVIC_IRQChannelPreemptionPriority		= 	3;
//	nvic.NVIC_IRQChannelSubPriority     		= 	0;
//	nvic.NVIC_IRQChannelCmd         			= 	ENABLE;
//	NVIC_Init(&nvic);

	// ����3	�����жϣ����� ���� ������ 1 ����
	nvic.NVIC_IRQChannel 						= 	USART3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority 		= 	5;
	nvic.NVIC_IRQChannelSubPriority 			= 	0;
	nvic.NVIC_IRQChannelCmd						= 	ENABLE;
	NVIC_Init(&nvic);
	
	// ����4	�����жϣ����� ���� ң��������
	nvic.NVIC_IRQChannel 						= 	UART4_IRQn;							
    nvic.NVIC_IRQChannelPreemptionPriority 		= 	4;
    nvic.NVIC_IRQChannelSubPriority 			=   0;
    nvic.NVIC_IRQChannelCmd 					=	 ENABLE;
    NVIC_Init(&nvic);
	
	// ����5	�����жϣ����� ���� ����ֵ����������
//	nvic.NVIC_IRQChannel 						= 	UART5_IRQn;
//	nvic.NVIC_IRQChannelPreemptionPriority 		= 	5;
//	nvic.NVIC_IRQChannelSubPriority 			= 	0;
//	nvic.NVIC_IRQChannelCmd						= 	ENABLE;
//	NVIC_Init(&nvic);
	
	// ����6	�����жϣ����� ���� ������ 2 ����
//	nvic.NVIC_IRQChannel 						= 	USART6_IRQn;
//	nvic.NVIC_IRQChannelPreemptionPriority 		= 	5;
//	nvic.NVIC_IRQChannelSubPriority 			= 	0;
//	nvic.NVIC_IRQChannelCmd						= 	ENABLE;
//	NVIC_Init(&nvic);
	
	//	CAN1	�ж�
	nvic.NVIC_IRQChannel 						= 	CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority 		= 	1;
    nvic.NVIC_IRQChannelSubPriority 			= 	0;
    nvic.NVIC_IRQChannelCmd 					= 	ENABLE;
    NVIC_Init(&nvic);
	
	//	CAN2	�ж�
	nvic.NVIC_IRQChannel 						= 	CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority 		= 	1;
    nvic.NVIC_IRQChannelSubPriority 			= 	0;
    nvic.NVIC_IRQChannelCmd 					= 	ENABLE;
    NVIC_Init(&nvic);
	
	nvic.NVIC_IRQChannel						=	TIM2_IRQn; 		
	nvic.NVIC_IRQChannelPreemptionPriority		=	5; 				
	nvic.NVIC_IRQChannelSubPriority				=	2; 				
	nvic.NVIC_IRQChannelCmd						=	ENABLE;	
	NVIC_Init(&nvic);
	
//	
//	//	TIM6_DAC	�ж�
//	nvic.NVIC_IRQChannel 						= 	TIM6_DAC_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority		= 	0;
//    nvic.NVIC_IRQChannelSubPriority				= 	0;
//    nvic.NVIC_IRQChannelCmd 					= 	DISABLE;
//    NVIC_Init(&nvic);
//	
//		
//	nvic.NVIC_IRQChannel						=	TIM7_IRQn; 		
//	nvic.NVIC_IRQChannelPreemptionPriority		=	1; 				
//	nvic.NVIC_IRQChannelSubPriority				=	3; 				
//	nvic.NVIC_IRQChannelCmd						=	ENABLE;	
//	NVIC_Init(&nvic);
//	
//	
//	
//	//	TIM8_��ʱ��8�ж�
//	nvic.NVIC_IRQChannel						=	 TIM8_CC_IRQn ; 		
//	nvic.NVIC_IRQChannelPreemptionPriority		=	0; 				
//	nvic.NVIC_IRQChannelSubPriority				=	0; 				
//	nvic.NVIC_IRQChannelCmd						=	ENABLE;	
//	NVIC_Init(&nvic);
//	
//	
//	//����	�ⲿ�ж�
//	nvic.NVIC_IRQChannel 						= 	EXTI0_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority 		= 	2;
//    nvic.NVIC_IRQChannelSubPriority 			= 	1;
//    nvic.NVIC_IRQChannelCmd 					= 	ENABLE;
//    NVIC_Init(&nvic);


}



