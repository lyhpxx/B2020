#include "tim7.h"

void TIM7_Configuration(void)
{
	TIM_TimeBaseInitTypeDef 	tim7;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  					//ʹ��TIM7ʱ��
	
	tim7.TIM_Period 		= 	10 - 1; 									//�Զ���װ��ֵ	1ms
	tim7.TIM_Prescaler		= 	8400 - 1;  									//��ʱ����Ƶ
	tim7.TIM_CounterMode	=	TIM_CounterMode_Up; 						//���ϼ���ģʽ
	tim7.TIM_ClockDivision	=	TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&tim7);											//��ʼ��TIM7

	
//	TIM7_��ʱ��2�ж�
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); 								//����ʱ��2�����ж�		�����ж�ʱ��
	TIM_Cmd(TIM7,ENABLE); 													//ʹ�ܶ�ʱ��2
}

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) 
	{

	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
}

