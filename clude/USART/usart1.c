#include "usart1.h"
/*-----USART1_TX-----PB6-----*/
/*-----USART1_RX-----PB7----*/
//cyq: for test

u16 USART_RX_STA=0;       //����״̬���	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.


void USART1_Configuration()
{
    USART_InitTypeDef usart1;
	GPIO_InitTypeDef  gpio;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6 ,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); 
	
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&gpio);

	usart1.USART_BaudRate = 115200;
	usart1.USART_WordLength = USART_WordLength_8b;
	usart1.USART_StopBits = USART_StopBits_1;
	usart1.USART_Parity = USART_Parity_No;
	usart1.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&usart1);

	USART_Cmd(USART1,ENABLE);
    
}



void USART1_SendChar(unsigned char a)
{
    while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);

	USART_SendData(USART1,a);    

}

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
    USART_SendData(USART1, (uint8_t)ch);    
    return ch;
}

int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USART1);
}


//void USART1_IRQHandler(void)			//�򿪴��ڽ����жϵĻ������CANͨ�����Ӱ�죻
//{
//	u8 Res;

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//��������� 
//				printf("%d/r/n",Res);
//			}
//			else //��û�յ�0X0D
//			{
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//					printf("%d/r/n",Res);
//				}
//			}
//		}		 
//	}
//}





