#include "uart4.h"
#include "dbus.h"
#include "stm32f4xx.h"
#include "stdio.h"


extern unsigned char dbus_buf0[DBUS_BUF_SIZE];

void UART4_Configuration(void)
{
		USART_InitTypeDef 	uart4;		//DBUS�Ĵ���
		GPIO_InitTypeDef  	gpio;		//DBUS������
		DMA_InitTypeDef  	dma;		//����DMA

		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

		GPIO_PinAFConfig(GPIOC,GPIO_PinSource11, GPIO_AF_UART4);

		gpio.GPIO_Pin = GPIO_Pin_11 ;
        gpio.GPIO_Mode = GPIO_Mode_AF;
        gpio.GPIO_OType = GPIO_OType_PP;
        gpio.GPIO_Speed = GPIO_Speed_100MHz;
        gpio.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC,&gpio);

		USART_DeInit(UART4);
		uart4.USART_BaudRate = 100000;   //SBUS 100K baudrate		//DBUS�����ʣ�����Ķ�
		uart4.USART_WordLength = USART_WordLength_8b;				
		uart4.USART_StopBits = USART_StopBits_1;
		uart4.USART_Parity = USART_Parity_Even;
		uart4.USART_Mode = USART_Mode_Rx;
		uart4.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(UART4,&uart4);

		USART_Cmd(UART4,ENABLE);
		USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
		USART_ITConfig(UART4,USART_IT_IDLE ,ENABLE);			//��������4�����ж�


		DMA_DeInit(DMA1_Stream2);								//dma��������ѡ��
		dma.DMA_Channel= DMA_Channel_4;							//ͨ��ѡ��
		dma.DMA_PeripheralBaseAddr = (u32)&(UART4->DR);			//DMA�����ַ
		dma.DMA_Memory0BaseAddr = (u32)dbus_buf0;				//DMA �洢��0��ַ
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;				//�洢��������ģʽ
		dma.DMA_BufferSize = DBUS_BUF_SIZE;						//���ݴ����� 
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;				//�洢������ģʽ
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݳ���:8λ
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		//�洢�����ݳ���:8λ
		dma.DMA_Mode = DMA_Mode_Circular;						//ʹ��ѭ��ģʽ 
		dma.DMA_Priority = DMA_Priority_VeryHigh;				//��ߵ����ȼ�
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;				//ֱ��ģʽ
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;	//	1/4, 1/2, 3/4, 1
		dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//�洢��ͻ�����δ���		
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//������ͻ�����δ���
		DMA_Init(DMA1_Stream2,&dma);							//��ʼ��DMA Stream������

//	DMA_ITConfig(DMA1_Stream2,DMA_IT_TC,ENABLE);			//DMA�ж�
    DMA_Cmd(DMA1_Stream2,ENABLE);

////����͵�����ģʽһ��
////�ڴ�0 ��Buffer0�ȱ�����
//DMA_DoubleBufferModeConfig(DMA1_Stream2,(uint32_t)&dbus_buf1,DMA_Memory_0);		//˫����
//DMA_DoubleBufferModeCmd(DMA1_Stream2,ENABLE);

}


