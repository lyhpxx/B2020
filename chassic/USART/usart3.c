#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "usart3.h"
#include "semphr.h"  
#include "task.h"  
#include "main.h"

uint8_t USART3temp;


/*�����������*/
unsigned   char   RX_BUF[22];  
int  RX_BUF_1[12];

void USART3_Configuration(void)
{
	USART_InitTypeDef		usart3;
	GPIO_InitTypeDef 		gpio;
	DMA_InitTypeDef  	 	dma;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11, GPIO_AF_USART3);

	
	gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;	
	GPIO_Init(GPIOB,&gpio);
    
    
    gpio.GPIO_Pin = GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd=GPIO_PuPd_UP;	
	GPIO_Init(GPIOB,&gpio);
         

	USART_DeInit(USART3);
	usart3.USART_BaudRate = 115200;
	usart3.USART_WordLength = USART_WordLength_8b;
	usart3.USART_StopBits = USART_StopBits_1;
	usart3.USART_Parity = USART_Parity_No ;
	usart3.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
 
	USART_Init(USART3,&usart3);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART3,ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART3,USART_IT_IDLE ,ENABLE);		//��������6�����ж�
	
		
	/* ���� DMA Stream */    
	//USART2_RX
	DMA_DeInit(DMA1_Stream1);			//dma��������ѡ��
    dma.DMA_Channel= DMA_Channel_4;		//ͨ��ѡ��
    dma.DMA_PeripheralBaseAddr = (u32)&(USART3->DR);	//DMA�����ַ
    dma.DMA_Memory0BaseAddr =(u32)RX_BUF;			//DMA �洢��0��ַ
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;			//�洢��������ģʽ
    dma.DMA_BufferSize = 22;					//���ݴ����� 120
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//���������ģʽ
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;			//�洢������ģʽ
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݳ���:8λ
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//�洢�����ݳ���:8λ
    dma.DMA_Mode = DMA_Mode_Circular;					// ʹ��ѭ��ģʽ 
    dma.DMA_Priority = DMA_Priority_VeryHigh;			//��ߵ����ȼ�
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;			 //ֱ��ģʽ
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;		//	1/4, 1/2, 3/4, 1
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;		//�洢��ͻ�����δ���		
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//������ͻ�����δ���
    DMA_Init(DMA1_Stream1,&dma);	 //��ʼ��DMA Stream������
		
}

/**
  * @brief  ����ϵͳ���ݶ�ȡ�ж�
  * @param  void
  * @retval void
  * @note	Ϊ�˱�֤���ݰ�����ʧ��ע���жϵ����ȼ�
  */
extern SemaphoreHandle_t BinarySemaphore ;//��ֵ�ź������


void USART3_IRQHandler(void)
{     

     	BaseType_t xHigherPriorityTaskWoken;//��ֵ�ź������BaseType_t xHigherPriorityTaskWoken;
        USART3temp = USART3->DR;					//��������жϵı�־λ
        USART3temp = USART3->SR;
        DMA_Cmd(DMA1_Stream1, DISABLE);				//�ر�DMA����    
		if(DMA1_Stream1->NDTR ==22 )
		{		 
			if((RX_BUF[0]=='$')&&(RX_BUF[21]=='#'))			  
			{
				RX_BUF_1[0]=RX_BUF[1]<<8|RX_BUF[2];		
				RX_BUF_1[1]=RX_BUF[3]<<8|RX_BUF[4];

				RX_BUF_1[2]=RX_BUF[5]<<8|RX_BUF[6];		
				RX_BUF_1[3]=RX_BUF[7]<<8|RX_BUF[8];
			
				RX_BUF_1[4]=RX_BUF[9]<<8|RX_BUF[10];		
				RX_BUF_1[5]=RX_BUF[11]<<8|RX_BUF[12];
			
				RX_BUF_1[6]=RX_BUF[13]<<8|RX_BUF[14];		
				RX_BUF_1[7]=RX_BUF[15]<<8|RX_BUF[16];
						
				RX_BUF_1[8]=RX_BUF[17];	
				RX_BUF_1[9]=RX_BUF[18];	
				RX_BUF_1[10]=RX_BUF[19];	
				RX_BUF_1[11]=RX_BUF[20];	          
		   }
	   }
        //�ͷŶ�ֵ�ź���     
		//����DMA
		DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);		
		while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);					//����������Ƿ������
		DMA_SetCurrDataCounter(DMA1_Stream1, 22);	//д��Ҫ��DMA�������ϴ����������
		DMA_Cmd(DMA1_Stream1, ENABLE);
		if((DMA1_Stream1->NDTR ==22)&&(BinarySemaphore!=NULL))//���յ����ݣ����Ҷ�ֵ�ź�����Ч
        {
                xSemaphoreGiveFromISR(BinarySemaphore,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л�
        }         //���¿���DMA
          
}

void USART3_SendChar(unsigned char b)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
    USART_SendData(USART3,b);
}



