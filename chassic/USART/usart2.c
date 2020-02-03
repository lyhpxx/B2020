#include "usart2.h"
#include "judge.h"
//#include "Chassis.h"
//#include "variable.h"
/******************************		RM2019����ϵͳ		***********************************/

//����ϵͳ���ݻ���
	uint8_t JudgeDataBuffer[JudgeBufferLength];
//�������������
	uint8_t UART2temp;

//�ṹ��
	Game_state		game_state;						//����״̬
	Field_events	field_events	;				//�����¼�
	Supply_action	supply_action	;				//����վ����
	Robot_stage		robot_stage		;				//������״̬
	Airplane_stage	airplane_stage	;				//�ɻ�״̬

//������
	Time_Remaining		time_remaining	;			//ʣ��ʱ��
	Blood_Remaining		blood_remaining	;			//ʣ��Ѫ��
	Chassis_Volt		chassis_volt;				//���̵�ѹ
	Chassis_Current		chassis_current;			//���̵���
	Chassis_Powe		chassis_powe;				//���̹���
	Chassis_Powe_Buffer	chassis_powe_buffer;		//���̹��ʻ���
	Shooter_Heat_17mm	shooter_heat_17mm;			//17mmǹ������
	Shooter_Heat_42mm	shooter_heat_42mm;			//42mmǹ������
	Airplane_Attack		airplane_attack;			//�ɻ��ɹ���ʱ�� 
	Shoot_Speed			shoot_speed;				//17mm��42mm����

	Robot_Interactive_Receive_1	robot_interactive_receive_1;	//�����˽�����������1
	Robot_Interactive_Receive_2	robot_interactive_receive_2;	//�����˽�����������2
	Robot_Interactive_Receive_3	robot_interactive_receive_3;	//�����˽�����������3
	Robot_Interactive_Receive_4	robot_interactive_receive_4;	//�����˽�����������4


//��Ҫͨ������ϵͳ������ʾ��PC�˵�����
	xy1 XY1;  xy2  XY2;   xy3 XY3;
	uint8_t XY4;
	uint8_t Judge_System_Buffer[28];
	data_Judge_System  Data_Judge_System_Send;

/*-----USART2_TX-----PD5-----*/
/*-----USART2_RX-----PD6----*/
//cyq: for test

/**
  * @brief  USART2�������
  * @param  void+
  * @retval void
  * @note	��֮ǰ�Ļ����������DMA
  */
  
void USART2_Configuration(void)
{
    USART_InitTypeDef 				usart2;		//����ϵͳ�Ĵ���
	GPIO_InitTypeDef  				gpio;		//�������Ŷ���
	DMA_InitTypeDef   				dma;		//����ϵͳ��DAM
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	
    gpio.GPIO_Pin 	= 	GPIO_Pin_5 | GPIO_Pin_6;
    gpio.GPIO_Mode 	= 	GPIO_Mode_AF;
    gpio.GPIO_OType = 	GPIO_OType_PP;
    gpio.GPIO_Speed = 	GPIO_Speed_100MHz;
    gpio.GPIO_PuPd 	= 	GPIO_PuPd_UP;			//֮ǰΪGPIO_PuPd_NOPULL
    GPIO_Init(GPIOD,&gpio);

	usart2.USART_BaudRate 				= 	115200;
	usart2.USART_WordLength 			= 	USART_WordLength_8b;
	usart2.USART_StopBits 				= 	USART_StopBits_1;
	usart2.USART_Parity 				= 	USART_Parity_No;
	usart2.USART_Mode 					= 	USART_Mode_Rx|USART_Mode_Tx;
	usart2.USART_HardwareFlowControl 	= 	USART_HardwareFlowControl_None;
	USART_Init(USART2,&usart2);
		    
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);				//����ʱ���Թص�
	USART_Cmd(USART2,ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2,USART_IT_IDLE ,ENABLE);				//��������2�����ж�

	/* ���� DMA Stream */    
    //USART2_RX
	DMA_DeInit(DMA1_Stream5);
    dma.DMA_Channel           =   DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr=   (uint32_t)(&USART2->DR);		//DMA�����ַ
    dma.DMA_Memory0BaseAddr   =   (uint32_t)(JudgeDataBuffer);
    dma.DMA_DIR               =   DMA_DIR_PeripheralToMemory;	//���赽�洢��ģʽ
    dma.DMA_BufferSize        =   JudgeBufferLength;
    dma.DMA_PeripheralInc     =   DMA_PeripheralInc_Disable;	//���������ģʽ
    dma.DMA_MemoryInc         =   DMA_MemoryInc_Enable;			//�洢������ģʽ
    dma.DMA_MemoryDataSize    =   DMA_MemoryDataSize_Byte;		
    dma.DMA_PeripheralDataSize=   DMA_PeripheralDataSize_Byte;	//�������ݳ���:8λ=1�ֽ�
    dma.DMA_Mode              =   DMA_Mode_Normal;    			//ģʽ����ͨ��ѭ��
    dma.DMA_Priority          =   DMA_Priority_Medium;			//���ȼ�
    dma.DMA_FIFOMode          =   DMA_FIFOMode_Disable;			//ֱ��ģʽ
    dma.DMA_FIFOThreshold     =   DMA_FIFOThreshold_Full;		//��ֵ
    dma.DMA_MemoryBurst       =   DMA_MemoryBurst_Single;			
    dma.DMA_PeripheralBurst   =   DMA_PeripheralBurst_Single;		
    DMA_Init(DMA1_Stream5, &dma);
    DMA_Cmd(DMA1_Stream5, ENABLE);
		
	//USART2_TX
	DMA_DeInit(DMA1_Stream6);
    dma.DMA_Channel           =   DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr=   (uint32_t)(&USART2->DR);
    dma.DMA_Memory0BaseAddr   =   (uint32_t)(Judge_System_Buffer);
    dma.DMA_DIR               =   DMA_DIR_MemoryToPeripheral;		//�洢��������ģʽ
    dma.DMA_BufferSize        =   28;								//����22���ֽ�
    dma.DMA_PeripheralInc     =   DMA_PeripheralInc_Disable;		//���������ģʽ
    dma.DMA_MemoryInc         =   DMA_MemoryInc_Enable;				//�洢������ģʽ
    dma.DMA_MemoryDataSize    =   DMA_MemoryDataSize_Byte;
    dma.DMA_PeripheralDataSize=   DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode              =   DMA_Mode_Circular;    			//ѭ��ģʽ
    dma.DMA_Priority          =   DMA_Priority_Medium;				//�е����ȼ�
    dma.DMA_FIFOMode          =   DMA_FIFOMode_Disable;				//ֱ��ģʽ
    dma.DMA_FIFOThreshold     =   DMA_FIFOThreshold_Full;
    dma.DMA_MemoryBurst       =   DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst   =   DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream6, &dma);
		
		
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);					//����DMA�ж�
    DMA_Cmd(DMA1_Stream6, ENABLE);
}

/**
  * @brief  ����ϵͳ���ݶ�ȡ�ж�
  * @param  void
  * @retval void
  * @note	Ϊ�˱�֤���ݰ�����ʧ��ע���жϵ����ȼ�
 **/
/************************************************	��ȡ����ϵͳ����	**************************************************/
void USART2_IRQHandler(void)
{
	UART2temp = USART2->DR;
	UART2temp = USART2->SR;
	DMA_Cmd(DMA1_Stream5, DISABLE);										//�ر�DMA���� 
	
//	printf("usart2");
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X00 && JudgeDataBuffer[5] == 0X01)	//�������ͣ����̣�ʣ��ʱ��
	{
		/*	��������:	1:RM �Կ���		2:������		3:RM ICRA	*/
		game_state.type 	= (JudgeDataBuffer[7]&0X0F);            		
		/*	��������	:	0:δ��ʼ����		1:׼���׶�		2:�Լ�׶�		3:���뵹��ʱ		4:��ս��		5:����������	*/
		game_state.course	= ((JudgeDataBuffer[7]>>4)&0X0F);        
		/*	����ʣ��ʱ��	(s)	*/
		time_remaining.T[0] = JudgeDataBuffer[8];
		time_remaining.T[1] = JudgeDataBuffer[9];
		game_state.remaining_time = time_remaining.Time;				
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X00 && JudgeDataBuffer[5] == 0X02)	//�������			
	{
		/*	���������	0��ƽ��		1����ʤ		2����ʤ	*/
		game_state.result 	= (int)JudgeDataBuffer[7];            	
	}

	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X01 && JudgeDataBuffer[5] == 0X01)	//�����¼�����			
	{
		field_events.Tarmac			= (int)(JudgeDataBuffer[7]&0X03);			//ͣ��ƺ
		field_events.Supply_1		= (int)((JudgeDataBuffer[7]>>2)&0X01);		//����վ 1
		field_events.Supply_2		= (int)((JudgeDataBuffer[7]>>3)&0X01);		//����վ 2
		field_events.Supply_3		= (int)((JudgeDataBuffer[7]>>4)&0X01);		//����վ 3 
		field_events.Big_energy		= (int)((JudgeDataBuffer[7]>>5)&0X03);		//����������
		field_events.Strategic_pass	= (int)((JudgeDataBuffer[7]>>7)&0X01);		//�ؿ�
		
		field_events.Pillbox		= (int)(JudgeDataBuffer[8]&0X01);			//�ﱤ
		field_events.Resource_island= (int)((JudgeDataBuffer[8]>>1)&0X01);		//��Դ��
		field_events.Base			= (int)((JudgeDataBuffer[8]>>2)&0X03);		//����
		field_events.ICRA_red		= (int)((JudgeDataBuffer[8]>>4)&0X03);		//ICRA��
		field_events.ICRA_blue		= (int)((JudgeDataBuffer[8]>>6)&0X03);		//ICRA��
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X01 && JudgeDataBuffer[5] == 0X02)	//����վ������ʶ	
	{
		supply_action.projectile_id		=	(int)JudgeDataBuffer[7];	//����վ��
		supply_action.robot_id			=	(int)JudgeDataBuffer[8];	//ԤԼ������
		supply_action.projectile_state	=	(int)JudgeDataBuffer[9];	//�ӵ��ڿ���״̬
		supply_action.projectile_num	=	(int)JudgeDataBuffer[10];	//�ӵ�����
	}
	
	if(JudgeDataBuffer[48] == 0xA5 && JudgeDataBuffer[54] == 0X02 && JudgeDataBuffer[53] == 0X01)	//������״̬����
	{
		robot_stage.Robot_id	=	JudgeDataBuffer[55]	;		//������id
		
		robot_stage.Robot_level	=	JudgeDataBuffer[56]	;		//�����˵ȼ�

		blood_remaining.H[0]	=	JudgeDataBuffer[57]	;
		blood_remaining.H[1]	=	JudgeDataBuffer[58]	;
		robot_stage.Remain_HP	=	blood_remaining.HP;			//ʣ��Ѫ��
		
		robot_stage.gimbal_output	=	(JudgeDataBuffer[69]&0X01);			//���ظ���������̨�����
		robot_stage.chassis_output	=	((JudgeDataBuffer[69]>>1)&0X01);	//���ظ��������
		robot_stage.shooter_output	=	((JudgeDataBuffer[69]>>2)&0X01);	//���ظ�ǹ�����
//		printf("	HP:%d	\r\n",	robot_stage.Remain_HP	);
		
	}

	
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X02 && JudgeDataBuffer[5] == 0X02)	//ʵʱ��������
	{
		chassis_volt.V[0]			=	JudgeDataBuffer[7]	;
		chassis_volt.V[1]			=	JudgeDataBuffer[8]	;
		robot_stage.chassis_volt	=	chassis_volt.VOTE	;		//���̵�ѹ
		
		chassis_current.C[0]		=	JudgeDataBuffer[9]	;
		chassis_current.C[1]		=	JudgeDataBuffer[10]	;
		robot_stage.chassis_current	=	chassis_current.CURRENT;	//���̵���
		
		chassis_powe.P[0]			=	JudgeDataBuffer[11]	;
		chassis_powe.P[1]			=	JudgeDataBuffer[12]	;
		chassis_powe.P[2]			=	JudgeDataBuffer[13]	;
		chassis_powe.P[3]			=	JudgeDataBuffer[14]	;
		robot_stage.chassis_power	=	chassis_powe.POWER	;		//���̹���
		
		chassis_powe_buffer.P_B[0]	=	JudgeDataBuffer[15];
		chassis_powe_buffer.P_B[1]	=	JudgeDataBuffer[16];
		robot_stage.chassis_powe_buffer	=	chassis_powe_buffer.POWER_BUFER;	//���̹��ʻ���
		
		shooter_heat_17mm.S_H_17[0]	=	JudgeDataBuffer[17]	;
		shooter_heat_17mm.S_H_17[1]	=	JudgeDataBuffer[18]	;
		robot_stage.shooter_heat_17mm		=	shooter_heat_17mm.HEAT_17mm;		//17mmǹ������
		
		shooter_heat_42mm.S_H_42[0]	=	JudgeDataBuffer[19]	;
		shooter_heat_42mm.S_H_42[1]	=	JudgeDataBuffer[20]	;
		robot_stage.shooter_heat_42mm		=	shooter_heat_42mm.HEAT_42mm;		//42mmǹ������
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X02 && JudgeDataBuffer[5] == 0X05)		//�ɻ�״̬
	{
		airplane_stage.Energy	=	JudgeDataBuffer[7];			//�ɻ���������
		
		airplane_attack.T[0]	=	JudgeDataBuffer[8];
		airplane_attack.T[1]	=	JudgeDataBuffer[9];
		airplane_stage.Time		=	airplane_attack.TIME;		//�ɻ��ɹ���ʱ�� 
		
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X02 && JudgeDataBuffer[5] == 0X06)		//�˺�״̬
	{
		/*	0-4��������˵�1-5װ�װ�	*/
		robot_stage.armor_id	=	(JudgeDataBuffer[7]&0X0F);	//����װ��ID
		/*	0x0 װ���˺���Ѫ	0x1;	ģ����߿�Ѫ;	0x2 ��ǹ��������Ѫ;	0x3 �����̹��ʿ�Ѫ;	*/
		robot_stage.hurt_type	=	((JudgeDataBuffer[7]>>4)&0X0F);	//�˺�����
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X02 && JudgeDataBuffer[5] == 0X07)		//ʵʱ�����Ϣ
	{
		shoot_speed.ST[0]	=	JudgeDataBuffer[9]	;
		shoot_speed.ST[1]	=	JudgeDataBuffer[10]	;
		shoot_speed.ST[2]	=	JudgeDataBuffer[11]	;
		shoot_speed.ST[3]	=	JudgeDataBuffer[12]	;	
		switch(JudgeDataBuffer[7])
		{
			case(1):robot_stage.bullet_speed_17mm	=	shoot_speed.SPEED;				//17mm����
					robot_stage.bullet_RF_17mm		=	JudgeDataBuffer[8];		break;		//17mm��Ƶ
			case(2):robot_stage.bullet_speed_42mm	=	shoot_speed.SPEED;				//42mm����
					robot_stage.bullet_RF_42mm		=	JudgeDataBuffer[8];		break;		//42mm��Ƶ

		}
//		printf("%f	%d	\r\n",robot_stage.bullet_speed_17mm,robot_stage.bullet_RF_17mm);

	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X03 && JudgeDataBuffer[5] == 0X01)		//�����˻�����Ϣ������
	{
		if(JudgeDataBuffer[7] == 0X56 && JudgeDataBuffer[8] == 0X02)								//����λID
		{
			if(JudgeDataBuffer[9] == 0X01 && JudgeDataBuffer[10] == 0X00 && JudgeDataBuffer[11] == 0X07 && JudgeDataBuffer[12] == 0X00)	//�ڱ�����
			{				
				robot_interactive_receive_1.D_1[0]	=	JudgeDataBuffer[13];
				robot_interactive_receive_1.D_1[1]	=	JudgeDataBuffer[14];
				robot_stage.receive_date_1			=	robot_interactive_receive_1.DATE_1;				//��������1
				
				robot_interactive_receive_2.D_2[0]	=	JudgeDataBuffer[15];
				robot_interactive_receive_2.D_2[1]	=	JudgeDataBuffer[16];
				robot_stage.receive_date_2			=	robot_interactive_receive_2.DATE_2;				//��������2
				
				robot_interactive_receive_3.D_3[0]	=	JudgeDataBuffer[17];
				robot_interactive_receive_3.D_3[1]	=	JudgeDataBuffer[18];
				robot_stage.receive_date_3			=	robot_interactive_receive_3.DATE_3;				//��������3
				
				robot_interactive_receive_4.D_4[0]	=	JudgeDataBuffer[19];
				robot_interactive_receive_4.D_4[1]	=	JudgeDataBuffer[20];
				robot_stage.receive_date_4			=	robot_interactive_receive_4.DATE_4;				//��������4
			}
		}
	}

	 //����DMA
    DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5);
    while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream5, JudgeBufferLength);
    DMA_Cmd(DMA1_Stream5, ENABLE);
}



/************************************************	���ͻ��˷�������	**************************************************/
void Judge_System_SendData(data_Judge_System  Data_Judge_System)	//����ϵͳ�������ݸ��ͻ��� 
{
	int self_defining_data_ID 	=	0x0301;		//�ͻ����Զ�������ID
	int	date_content_ID			=	0xD180;		//��������ID
	int	sender_ID				=	0;			//������ID:�췽Ӣ��
	int	sreceiver_ID			=	0;		//������ID:�췽Ӣ�ۿͻ���
	
	
	int Data_Length = 19;						//19Ϊ����֡����
    
    
    switch(robot_stage.Robot_id)
	{
		case(1) : sender_ID=1 , sreceiver_ID = 0x0101;break;	
		case(2) : sender_ID=2 , sreceiver_ID = 0x0102;break;	
		case(3) : sender_ID=3 , sreceiver_ID = 0x0103;break;	
		case(4) : sender_ID=4 , sreceiver_ID = 0x0104;break;	
		case(5) : sender_ID=5 , sreceiver_ID = 0x0105;break;	
		case(6) : sender_ID=6 , sreceiver_ID = 0x0106 ;break;	
		case(11) : sender_ID=11 , sreceiver_ID = 0x0111;break;	
		case(12) : sender_ID=12 , sreceiver_ID = 0x0112;break;	
		case(13) : sender_ID=13	, sreceiver_ID = 0x0113;break;	
		case(14) : sender_ID=14 , sreceiver_ID = 0x0114;break;	
		case(15) : sender_ID=15 , sreceiver_ID = 0x0115;break;	
		case(16) : sender_ID=16 , sreceiver_ID = 0x0116;break;	
        default:		break;
        
	}
	XY1.y1 = Data_Judge_System.hk1;				//ǿ��ת����float��ת��Ϊchar������
	XY2.y2 = Data_Judge_System.hk2;
	XY3.y3 = Data_Judge_System.hk3;
	XY4    = Data_Judge_System.hk4;
	
	//FrameHeader	̨֡ͷ
	Judge_System_Buffer[0] = 0xA5;							//֡��ʼ
	Judge_System_Buffer[1] = (uint8_t)(Data_Length);		//����֡�ĵ�8λ
	Judge_System_Buffer[2] = (uint8_t)(Data_Length >> 8);	//����֡�ĸ�8λ
	Append_CRC8_Check_Sum(Judge_System_Buffer,5);			//5=3+2
	//���crc8У���뵽֡ͷβ����֡ͷӵ��5Bite			5λ
	
	//Command ID	����ID
	Judge_System_Buffer[5] = (uint8_t)(self_defining_data_ID);
	Judge_System_Buffer[6] = (uint8_t)(self_defining_data_ID >> 8);
	
	/*����Ϊ����֡����19��*/
	Judge_System_Buffer[7] = (uint8_t)(date_content_ID);
	Judge_System_Buffer[8] = (uint8_t)(date_content_ID >> 8);
	
	Judge_System_Buffer[9] = (uint8_t)(sender_ID);
	Judge_System_Buffer[10] = (uint8_t)(sender_ID >> 8);
	
	Judge_System_Buffer[11] = (uint8_t)(sreceiver_ID);
	Judge_System_Buffer[12] = (uint8_t)(sreceiver_ID >> 8);
	
	Judge_System_Buffer[13] = XY1.x1[0];		
	Judge_System_Buffer[14] = XY1.x1[1];
	Judge_System_Buffer[15] = XY1.x1[2];
	Judge_System_Buffer[16] = XY1.x1[3];

	Judge_System_Buffer[17] = XY2.x2[0];
	Judge_System_Buffer[18] = XY2.x2[1];
	Judge_System_Buffer[19] = XY2.x2[2];
	Judge_System_Buffer[20] = XY2.x2[3];

	Judge_System_Buffer[21] = XY3.x3[0];
	Judge_System_Buffer[22] = XY3.x3[1];
	Judge_System_Buffer[23] = XY3.x3[2];
	Judge_System_Buffer[24] = XY3.x3[3];

	Judge_System_Buffer[25] = XY4;

	//FrameTail	
	Append_CRC16_Check_Sum(Judge_System_Buffer,28);				//28=26+2
	//���crc16У���뵽֡β��
}

/**
  * @brief  �Զ������ݷ����ж�
  * @param  void
  * @retval void
  * @note	����Ҫ��ʾ�����ݴ���ṹ�� Data_Judge_System_Send
  */
extern int Now_YouCanShoot;
extern int AutoState;
void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
	{
//		Data_Judge_System_Send.hk1 = 	CapVoltage[0]/1000	;		//	float��32λ
//		Data_Judge_System_Send.hk2 =	Now_YouCanShoot;		//	float��32λ
//		Data_Judge_System_Send.hk3 =	AutoState;		//	float��32λ
		Data_Judge_System_Send.hk4 = 	1	;		//	�Զ���8λ����
		Judge_System_SendData(Data_Judge_System_Send);	
		
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
	}
}




/************************************************	������վ��������	**************************************************/
void USART2_SendChar(unsigned char b)
{
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);

	USART_SendData(USART2,b);    
}

/*
  ԤԼ����վ�� ID��
		0�� ���в����ڣ����� 1�� 2 ˳���ѯ�����������;
		1�� 1 �Ų�����;
		2�� 2 �Ų�����.
  num��	
		0-50 ΪԤԼ50���ӵ���
		51-100 ΪԤԼ 100 ���ӵ��� 
		101-150 ΪԤԼ150 ���ӵ���
		151-255 ΪԤԼ 200 ���ӵ��������� 200 ���ӵ���
*/	
uint8_t		bullet[11];
void Appointment_bullet(int ID,int num)		//ԤԼ����վID��ԤԼ�ӵ�����
{
	int i=0;
	int	Length	=	2;
	int commend	=	0X0103;						//���͸�����վ
	//FrameHeader	̨֡ͷ
	bullet[0]	=	0xA5;						//֡��ʼ
	bullet[1]	=	(uint8_t)(Length);			//����֡�ĵ�8λ
	bullet[2]	=	(uint8_t)(Length >> 8);		//����֡�ĸ�8λ
	Append_CRC8_Check_Sum(bullet,5);			//5=3+2
	//���crc8У���뵽֡ͷβ����֡ͷӵ��5Bite	5λ		
	bullet[5]	=	(uint8_t)(commend);			//������
	bullet[6]	=	(uint8_t)(commend>>8);		
	
	/*����Ϊ����֡����2��*/
	bullet[7]	=	(uint8_t)(ID);				
	bullet[8]	=	(uint8_t)(num);				
	Append_CRC16_Check_Sum(bullet,11);			//11λ
	
	for(i=0;i<11;i++)
	{
		USART2_SendChar(bullet[i]);
	}
}




/************************************************	������֮�佻������	**************************************************/
uint8_t	Robot_interactive_date[128];

void Robot_interactive(int date1,int date2,int date3,int date4)	//�����˻�����Ϣ������
{
	
	int	Length						=	14;			//����֡����

	int	robot_communication_ID		=	0X0301;		//�����˼�ͨ��ID
	int	Communication_content_ID	=	0X0256;		//��������ID��0x0200~0x02FF֮��
	int	Communication_sender_ID		=	0X0001;			//������ID:�췽Ӣ��
	int	Communication_sreceiver_ID	=	0X0007;			//������ID:�췽�ڱ�


	Robot_interactive_date[0]	=	 0XA5;
	Robot_interactive_date[1]	=	(uint8_t)(Length);
	Robot_interactive_date[2]	=	(uint8_t)(Length >> 8);		//����֡�ĸ�8λ
	Append_CRC8_Check_Sum(Robot_interactive_date,5);			//5=3+2

	Robot_interactive_date[5]	=	(uint8_t)(robot_communication_ID);
	Robot_interactive_date[6]	=	(uint8_t)(robot_communication_ID>>8);
	
	/*����Ϊ����֡����14��*/
	Robot_interactive_date[7]	=	(uint8_t)(Communication_content_ID);
	Robot_interactive_date[8]	=	(uint8_t)(Communication_content_ID>>8);
	
	Robot_interactive_date[9]	=	(uint8_t)(Communication_sender_ID);
	Robot_interactive_date[10]	=	(uint8_t)(Communication_sender_ID>>8);
	
	Robot_interactive_date[11]	=	(uint8_t)(Communication_sreceiver_ID);
	Robot_interactive_date[12]	=	(uint8_t)(Communication_sreceiver_ID>>8);
	
	Robot_interactive_date[13]	=	(uint8_t)(date1);
	Robot_interactive_date[14]	=	(uint8_t)(date1>>8);
	
	Robot_interactive_date[15]	=	(uint8_t)(date2);
	Robot_interactive_date[16]	=	(uint8_t)(date2>>8);
	
	Robot_interactive_date[17]	=	(uint8_t)(date3);
	Robot_interactive_date[18]	=	(uint8_t)(date3>>8);
	
	Robot_interactive_date[19]	=	(uint8_t)(date4);
	Robot_interactive_date[20]	=	(uint8_t)(date4>>8);

	Append_CRC16_Check_Sum(Robot_interactive_date,23);			//23λ
	
}



