#include "encoder.h"

//TIM4_CH1 ----- PD12
//TIM4_CH2 ----- PD13

//TIM5_CH1 ----- PA0
//TIM5_CH2 ----- PA1

//������
void Encoder_1_Configuration(void)
{
	GPIO_InitTypeDef         gpio; 
	TIM_TimeBaseInitTypeDef  tim;
	TIM_ICInitTypeDef        tim_ic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,	ENABLE);//����TIM4ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//����GPIOBʱ��

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);//PD12���Ÿ���
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);//PD13���ŷ���

	gpio.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; 
	gpio.GPIO_Mode  = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_OD;
	gpio.GPIO_PuPd  = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD,&gpio); 

	tim.TIM_Period = 60000; //������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	tim.TIM_Prescaler = 0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	tim.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	tim.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &tim); 

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	TIM_ICStructInit(&tim_ic);
	tim_ic.TIM_ICFilter = 0;  //�����˲���
	TIM_ICInit(TIM4, &tim_ic);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);  //������б�־λ

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //�����жϸ���
	TIM4->CNT = 30000;
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

void Encoder_2_Configuration(void)
{
	GPIO_InitTypeDef         gpio; 
    TIM_TimeBaseInitTypeDef  tim;
    TIM_ICInitTypeDef        tim_ic;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//����TIM5ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//����GPIOBʱ��
  
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);//PA0���Ÿ���
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);//PA1���ŷ���
 
    gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_OType = GPIO_OType_OD;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA,&gpio); 
 
    tim.TIM_Period = 60000; //������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    tim.TIM_Prescaler = 0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
    tim.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    tim.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM5, &tim); 
 
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
    TIM_ICStructInit(&tim_ic);
    tim_ic.TIM_ICFilter = 0;  //�����˲���
    TIM_ICInit(TIM5, &tim_ic);
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);  //������б�־λ
	
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //�����жϸ���
    TIM5->CNT = 30000;
    TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5
}























//void Encoder1_Configuration(void)		//	������
//{
//	GPIO_InitTypeDef        gpio;
//	TIM_TimeBaseInitTypeDef tim;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5 ,ENABLE);

//	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOA,&gpio);

//	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOB,&gpio);

//	tim.TIM_Period = 60000;
//	tim.TIM_Prescaler = 0;//��Prescaler+1>1ʱ����Ƶ�������ã���˼���Ƶ��Ϊ36M*2/(Prescaler+1)
//	tim.TIM_ClockDivision = TIM_CKD_DIV1;
//	tim.TIM_CounterMode = TIM_CounterMode_Up;				//�����ؼ���
//	TIM_TimeBaseInit(TIM4, &tim);
//	TIM_TimeBaseInit(TIM5, &tim);

//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6,  GPIO_AF_TIM4);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7,  GPIO_AF_TIM4);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0,  GPIO_AF_TIM5);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,  GPIO_AF_TIM5);
//	
//	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//	TIM_SetCounter(TIM4,30000); 
//	TIM_SetCounter(TIM5,30000); 
//	TIM_Cmd(TIM4, ENABLE);
//	TIM_Cmd(TIM5, ENABLE);
//}

