#include "tim2.h"

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef 	tim2;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  					//ʹ��TIM2ʱ��
	
	
	tim2.TIM_Period 		= 	50 - 1; 									//�Զ���װ��ֵ	1ms
	tim2.TIM_Prescaler		= 	8400 - 1;  									//��ʱ����Ƶ
	tim2.TIM_CounterMode	=	TIM_CounterMode_Up; 						//���ϼ���ģʽ
	tim2.TIM_ClockDivision	=	TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&tim2);											//��ʼ��TIM2
	
		//	TIM2_��ʱ��2�ж�

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 								//����ʱ��2�����ж�		�����ж�ʱ��
	TIM_Cmd(TIM2,ENABLE); 													//ʹ�ܶ�ʱ��2
}

int time_2=0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) 
	{
time_2++;

	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}

