#include "usart2.h"
#include "judge.h"
//#include "Chassis.h"
//#include "variable.h"
/******************************		RM2019裁判系统		***********************************/

//裁判系统数据缓存
	uint8_t JudgeDataBuffer[JudgeBufferLength];
//保存读到的数据
	uint8_t UART2temp;

//结构体
	Game_state		game_state;						//比赛状态
	Field_events	field_events	;				//场地事件
	Supply_action	supply_action	;				//补给站动作
	Robot_stage		robot_stage		;				//机器人状态
	Airplane_stage	airplane_stage	;				//飞机状态

//共用体
	Time_Remaining		time_remaining	;			//剩余时间
	Blood_Remaining		blood_remaining	;			//剩余血量
	Chassis_Volt		chassis_volt;				//底盘电压
	Chassis_Current		chassis_current;			//底盘电流
	Chassis_Powe		chassis_powe;				//底盘功率
	Chassis_Powe_Buffer	chassis_powe_buffer;		//底盘功率缓冲
	Shooter_Heat_17mm	shooter_heat_17mm;			//17mm枪口热量
	Shooter_Heat_42mm	shooter_heat_42mm;			//42mm枪口热量
	Airplane_Attack		airplane_attack;			//飞机可攻击时间 
	Shoot_Speed			shoot_speed;				//17mm与42mm射速

	Robot_Interactive_Receive_1	robot_interactive_receive_1;	//机器人交互接受数据1
	Robot_Interactive_Receive_2	robot_interactive_receive_2;	//机器人交互接受数据2
	Robot_Interactive_Receive_3	robot_interactive_receive_3;	//机器人交互接受数据3
	Robot_Interactive_Receive_4	robot_interactive_receive_4;	//机器人交互接受数据4


//需要通过裁判系统发送显示在PC端的数据
	xy1 XY1;  xy2  XY2;   xy3 XY3;
	uint8_t XY4;
	uint8_t Judge_System_Buffer[28];
	data_Judge_System  Data_Judge_System_Send;

/*-----USART2_TX-----PD5-----*/
/*-----USART2_RX-----PD6----*/
//cyq: for test

/**
  * @brief  USART2相关配置
  * @param  void+
  * @retval void
  * @note	在之前的基础上增添成DMA
  */
  
void USART2_Configuration(void)
{
    USART_InitTypeDef 				usart2;		//裁判系统的串口
	GPIO_InitTypeDef  				gpio;		//串口引脚定义
	DMA_InitTypeDef   				dma;		//裁判系统的DAM
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	
    gpio.GPIO_Pin 	= 	GPIO_Pin_5 | GPIO_Pin_6;
    gpio.GPIO_Mode 	= 	GPIO_Mode_AF;
    gpio.GPIO_OType = 	GPIO_OType_PP;
    gpio.GPIO_Speed = 	GPIO_Speed_100MHz;
    gpio.GPIO_PuPd 	= 	GPIO_PuPd_UP;			//之前为GPIO_PuPd_NOPULL
    GPIO_Init(GPIOD,&gpio);

	usart2.USART_BaudRate 				= 	115200;
	usart2.USART_WordLength 			= 	USART_WordLength_8b;
	usart2.USART_StopBits 				= 	USART_StopBits_1;
	usart2.USART_Parity 				= 	USART_Parity_No;
	usart2.USART_Mode 					= 	USART_Mode_Rx|USART_Mode_Tx;
	usart2.USART_HardwareFlowControl 	= 	USART_HardwareFlowControl_None;
	USART_Init(USART2,&usart2);
		    
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);				//测试时可以关掉
	USART_Cmd(USART2,ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC);
		
	USART_ITConfig(USART2,USART_IT_IDLE ,ENABLE);				//开启串口2空闲中断

	/* 配置 DMA Stream */    
    //USART2_RX
	DMA_DeInit(DMA1_Stream5);
    dma.DMA_Channel           =   DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr=   (uint32_t)(&USART2->DR);		//DMA外设地址
    dma.DMA_Memory0BaseAddr   =   (uint32_t)(JudgeDataBuffer);
    dma.DMA_DIR               =   DMA_DIR_PeripheralToMemory;	//外设到存储器模式
    dma.DMA_BufferSize        =   JudgeBufferLength;
    dma.DMA_PeripheralInc     =   DMA_PeripheralInc_Disable;	//外设非增量模式
    dma.DMA_MemoryInc         =   DMA_MemoryInc_Enable;			//存储器增量模式
    dma.DMA_MemoryDataSize    =   DMA_MemoryDataSize_Byte;		
    dma.DMA_PeripheralDataSize=   DMA_PeripheralDataSize_Byte;	//外设数据长度:8位=1字节
    dma.DMA_Mode              =   DMA_Mode_Normal;    			//模式：普通，循环
    dma.DMA_Priority          =   DMA_Priority_Medium;			//优先级
    dma.DMA_FIFOMode          =   DMA_FIFOMode_Disable;			//直接模式
    dma.DMA_FIFOThreshold     =   DMA_FIFOThreshold_Full;		//阈值
    dma.DMA_MemoryBurst       =   DMA_MemoryBurst_Single;			
    dma.DMA_PeripheralBurst   =   DMA_PeripheralBurst_Single;		
    DMA_Init(DMA1_Stream5, &dma);
    DMA_Cmd(DMA1_Stream5, ENABLE);
		
	//USART2_TX
	DMA_DeInit(DMA1_Stream6);
    dma.DMA_Channel           =   DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr=   (uint32_t)(&USART2->DR);
    dma.DMA_Memory0BaseAddr   =   (uint32_t)(Judge_System_Buffer);
    dma.DMA_DIR               =   DMA_DIR_MemoryToPeripheral;		//存储器到外设模式
    dma.DMA_BufferSize        =   28;								//发送22个字节
    dma.DMA_PeripheralInc     =   DMA_PeripheralInc_Disable;		//外设非增量模式
    dma.DMA_MemoryInc         =   DMA_MemoryInc_Enable;				//存储器增量模式
    dma.DMA_MemoryDataSize    =   DMA_MemoryDataSize_Byte;
    dma.DMA_PeripheralDataSize=   DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode              =   DMA_Mode_Circular;    			//循环模式
    dma.DMA_Priority          =   DMA_Priority_Medium;				//中等优先级
    dma.DMA_FIFOMode          =   DMA_FIFOMode_Disable;				//直接模式
    dma.DMA_FIFOThreshold     =   DMA_FIFOThreshold_Full;
    dma.DMA_MemoryBurst       =   DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst   =   DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream6, &dma);
		
		
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);					//开启DMA中断
    DMA_Cmd(DMA1_Stream6, ENABLE);
}

/**
  * @brief  裁判系统数据读取中断
  * @param  void
  * @retval void
  * @note	为了保证数据包不丢失，注意中断的优先级
 **/
/************************************************	读取裁判系统数据	**************************************************/
void USART2_IRQHandler(void)
{
	UART2temp = USART2->DR;
	UART2temp = USART2->SR;
	DMA_Cmd(DMA1_Stream5, DISABLE);										//关闭DMA传输 
	
//	printf("usart2");
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X00 && JudgeDataBuffer[5] == 0X01)	//比赛类型，进程，剩余时间
	{
		/*	比赛类型:	1:RM 对抗赛		2:单项赛		3:RM ICRA	*/
		game_state.type 	= (JudgeDataBuffer[7]&0X0F);            		
		/*	比赛进程	:	0:未开始比赛		1:准备阶段		2:自检阶段		3:五秒倒计时		4:对战中		5:比赛结算中	*/
		game_state.course	= ((JudgeDataBuffer[7]>>4)&0X0F);        
		/*	比赛剩余时间	(s)	*/
		time_remaining.T[0] = JudgeDataBuffer[8];
		time_remaining.T[1] = JudgeDataBuffer[9];
		game_state.remaining_time = time_remaining.Time;				
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X00 && JudgeDataBuffer[5] == 0X02)	//比赛结果			
	{
		/*	比赛结果：	0：平局		1：红胜		2：蓝胜	*/
		game_state.result 	= (int)JudgeDataBuffer[7];            	
	}

	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X01 && JudgeDataBuffer[5] == 0X01)	//场地事件数据			
	{
		field_events.Tarmac			= (int)(JudgeDataBuffer[7]&0X03);			//停机坪
		field_events.Supply_1		= (int)((JudgeDataBuffer[7]>>2)&0X01);		//补给站 1
		field_events.Supply_2		= (int)((JudgeDataBuffer[7]>>3)&0X01);		//补给站 2
		field_events.Supply_3		= (int)((JudgeDataBuffer[7]>>4)&0X01);		//补给站 3 
		field_events.Big_energy		= (int)((JudgeDataBuffer[7]>>5)&0X03);		//大能量机关
		field_events.Strategic_pass	= (int)((JudgeDataBuffer[7]>>7)&0X01);		//关口
		
		field_events.Pillbox		= (int)(JudgeDataBuffer[8]&0X01);			//碉堡
		field_events.Resource_island= (int)((JudgeDataBuffer[8]>>1)&0X01);		//资源岛
		field_events.Base			= (int)((JudgeDataBuffer[8]>>2)&0X03);		//基地
		field_events.ICRA_red		= (int)((JudgeDataBuffer[8]>>4)&0X03);		//ICRA红
		field_events.ICRA_blue		= (int)((JudgeDataBuffer[8]>>6)&0X03);		//ICRA蓝
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X01 && JudgeDataBuffer[5] == 0X02)	//补给站动作标识	
	{
		supply_action.projectile_id		=	(int)JudgeDataBuffer[7];	//补给站口
		supply_action.robot_id			=	(int)JudgeDataBuffer[8];	//预约机器人
		supply_action.projectile_state	=	(int)JudgeDataBuffer[9];	//子弹口开闭状态
		supply_action.projectile_num	=	(int)JudgeDataBuffer[10];	//子弹数量
	}
	
	if(JudgeDataBuffer[48] == 0xA5 && JudgeDataBuffer[54] == 0X02 && JudgeDataBuffer[53] == 0X01)	//机器人状态数据
	{
		robot_stage.Robot_id	=	JudgeDataBuffer[55]	;		//机器人id
		
		robot_stage.Robot_level	=	JudgeDataBuffer[56]	;		//机器人等级

		blood_remaining.H[0]	=	JudgeDataBuffer[57]	;
		blood_remaining.H[1]	=	JudgeDataBuffer[58]	;
		robot_stage.Remain_HP	=	blood_remaining.HP;			//剩余血量
		
		robot_stage.gimbal_output	=	(JudgeDataBuffer[69]&0X01);			//主控给其它（云台）输出
		robot_stage.chassis_output	=	((JudgeDataBuffer[69]>>1)&0X01);	//主控给底盘输出
		robot_stage.shooter_output	=	((JudgeDataBuffer[69]>>2)&0X01);	//主控给枪口输出
//		printf("	HP:%d	\r\n",	robot_stage.Remain_HP	);
		
	}

	
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X02 && JudgeDataBuffer[5] == 0X02)	//实时功率热量
	{
		chassis_volt.V[0]			=	JudgeDataBuffer[7]	;
		chassis_volt.V[1]			=	JudgeDataBuffer[8]	;
		robot_stage.chassis_volt	=	chassis_volt.VOTE	;		//底盘电压
		
		chassis_current.C[0]		=	JudgeDataBuffer[9]	;
		chassis_current.C[1]		=	JudgeDataBuffer[10]	;
		robot_stage.chassis_current	=	chassis_current.CURRENT;	//底盘电流
		
		chassis_powe.P[0]			=	JudgeDataBuffer[11]	;
		chassis_powe.P[1]			=	JudgeDataBuffer[12]	;
		chassis_powe.P[2]			=	JudgeDataBuffer[13]	;
		chassis_powe.P[3]			=	JudgeDataBuffer[14]	;
		robot_stage.chassis_power	=	chassis_powe.POWER	;		//底盘功率
		
		chassis_powe_buffer.P_B[0]	=	JudgeDataBuffer[15];
		chassis_powe_buffer.P_B[1]	=	JudgeDataBuffer[16];
		robot_stage.chassis_powe_buffer	=	chassis_powe_buffer.POWER_BUFER;	//底盘功率缓冲
		
		shooter_heat_17mm.S_H_17[0]	=	JudgeDataBuffer[17]	;
		shooter_heat_17mm.S_H_17[1]	=	JudgeDataBuffer[18]	;
		robot_stage.shooter_heat_17mm		=	shooter_heat_17mm.HEAT_17mm;		//17mm枪口热量
		
		shooter_heat_42mm.S_H_42[0]	=	JudgeDataBuffer[19]	;
		shooter_heat_42mm.S_H_42[1]	=	JudgeDataBuffer[20]	;
		robot_stage.shooter_heat_42mm		=	shooter_heat_42mm.HEAT_42mm;		//42mm枪口热量
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X02 && JudgeDataBuffer[5] == 0X05)		//飞机状态
	{
		airplane_stage.Energy	=	JudgeDataBuffer[7];			//飞机积累能量
		
		airplane_attack.T[0]	=	JudgeDataBuffer[8];
		airplane_attack.T[1]	=	JudgeDataBuffer[9];
		airplane_stage.Time		=	airplane_attack.TIME;		//飞机可攻击时间 
		
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X02 && JudgeDataBuffer[5] == 0X06)		//伤害状态
	{
		/*	0-4代表机器人的1-5装甲板	*/
		robot_stage.armor_id	=	(JudgeDataBuffer[7]&0X0F);	//受伤装甲ID
		/*	0x0 装甲伤害扣血	0x1;	模块掉线扣血;	0x2 超枪口热量扣血;	0x3 超底盘功率扣血;	*/
		robot_stage.hurt_type	=	((JudgeDataBuffer[7]>>4)&0X0F);	//伤害类型
	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X02 && JudgeDataBuffer[5] == 0X07)		//实时射击信息
	{
		shoot_speed.ST[0]	=	JudgeDataBuffer[9]	;
		shoot_speed.ST[1]	=	JudgeDataBuffer[10]	;
		shoot_speed.ST[2]	=	JudgeDataBuffer[11]	;
		shoot_speed.ST[3]	=	JudgeDataBuffer[12]	;	
		switch(JudgeDataBuffer[7])
		{
			case(1):robot_stage.bullet_speed_17mm	=	shoot_speed.SPEED;				//17mm射速
					robot_stage.bullet_RF_17mm		=	JudgeDataBuffer[8];		break;		//17mm射频
			case(2):robot_stage.bullet_speed_42mm	=	shoot_speed.SPEED;				//42mm射速
					robot_stage.bullet_RF_42mm		=	JudgeDataBuffer[8];		break;		//42mm射频

		}
//		printf("%f	%d	\r\n",robot_stage.bullet_speed_17mm,robot_stage.bullet_RF_17mm);

	}
	
	if(JudgeDataBuffer[0] == 0xA5 && JudgeDataBuffer[6] == 0X03 && JudgeDataBuffer[5] == 0X01)		//机器人互交信息·接收
	{
		if(JudgeDataBuffer[7] == 0X56 && JudgeDataBuffer[8] == 0X02)								//数据位ID
		{
			if(JudgeDataBuffer[9] == 0X01 && JudgeDataBuffer[10] == 0X00 && JudgeDataBuffer[11] == 0X07 && JudgeDataBuffer[12] == 0X00)	//哨兵接收
			{				
				robot_interactive_receive_1.D_1[0]	=	JudgeDataBuffer[13];
				robot_interactive_receive_1.D_1[1]	=	JudgeDataBuffer[14];
				robot_stage.receive_date_1			=	robot_interactive_receive_1.DATE_1;				//接收数据1
				
				robot_interactive_receive_2.D_2[0]	=	JudgeDataBuffer[15];
				robot_interactive_receive_2.D_2[1]	=	JudgeDataBuffer[16];
				robot_stage.receive_date_2			=	robot_interactive_receive_2.DATE_2;				//接收数据2
				
				robot_interactive_receive_3.D_3[0]	=	JudgeDataBuffer[17];
				robot_interactive_receive_3.D_3[1]	=	JudgeDataBuffer[18];
				robot_stage.receive_date_3			=	robot_interactive_receive_3.DATE_3;				//接收数据3
				
				robot_interactive_receive_4.D_4[0]	=	JudgeDataBuffer[19];
				robot_interactive_receive_4.D_4[1]	=	JudgeDataBuffer[20];
				robot_stage.receive_date_4			=	robot_interactive_receive_4.DATE_4;				//接收数据4
			}
		}
	}

	 //重启DMA
    DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5);
    while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream5, JudgeBufferLength);
    DMA_Cmd(DMA1_Stream5, ENABLE);
}



/************************************************	给客户端发送数据	**************************************************/
void Judge_System_SendData(data_Judge_System  Data_Judge_System)	//裁判系统发送数据给客户端 
{
	int self_defining_data_ID 	=	0x0301;		//客户端自定义数据ID
	int	date_content_ID			=	0xD180;		//数据内容ID
	int	sender_ID				=	0;			//发送者ID:红方英雄
	int	sreceiver_ID			=	0;		//接收者ID:红方英雄客户端
	
	
	int Data_Length = 19;						//19为数据帧长度
    
    
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
	XY1.y1 = Data_Judge_System.hk1;				//强制转换将float型转换为char型数组
	XY2.y2 = Data_Judge_System.hk2;
	XY3.y3 = Data_Judge_System.hk3;
	XY4    = Data_Judge_System.hk4;
	
	//FrameHeader	帧台头
	Judge_System_Buffer[0] = 0xA5;							//帧起始
	Judge_System_Buffer[1] = (uint8_t)(Data_Length);		//数据帧的低8位
	Judge_System_Buffer[2] = (uint8_t)(Data_Length >> 8);	//数据帧的高8位
	Append_CRC8_Check_Sum(Judge_System_Buffer,5);			//5=3+2
	//添加crc8校验码到帧头尾部，帧头拥有5Bite			5位
	
	//Command ID	命令ID
	Judge_System_Buffer[5] = (uint8_t)(self_defining_data_ID);
	Judge_System_Buffer[6] = (uint8_t)(self_defining_data_ID >> 8);
	
	/*以下为数据帧，共19组*/
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
	//添加crc16校验码到帧尾部
}

/**
  * @brief  自定义数据发送中断
  * @param  void
  * @retval void
  * @note	将需要显示的数据存入结构体 Data_Judge_System_Send
  */
extern int Now_YouCanShoot;
extern int AutoState;
void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
	{
//		Data_Judge_System_Send.hk1 = 	CapVoltage[0]/1000	;		//	float型32位
//		Data_Judge_System_Send.hk2 =	Now_YouCanShoot;		//	float型32位
//		Data_Judge_System_Send.hk3 =	AutoState;		//	float型32位
		Data_Judge_System_Send.hk4 = 	1	;		//	自定义8位数据
		Judge_System_SendData(Data_Judge_System_Send);	
		
		DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
	}
}




/************************************************	给补给站发送数据	**************************************************/
void USART2_SendChar(unsigned char b)
{
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);

	USART_SendData(USART2,b);    
}

/*
  预约补给站口 ID：
		0： 空闲补给口，依照 1， 2 顺序查询补给空闲情况;
		1： 1 号补给口;
		2： 2 号补给口.
  num：	
		0-50 为预约50颗子弹，
		51-100 为预约 100 颗子弹， 
		101-150 为预约150 颗子弹，
		151-255 为预约 200 颗子弹。（上限 200 颗子弹）
*/	
uint8_t		bullet[11];
void Appointment_bullet(int ID,int num)		//预约补给站ID，预约子弹个数
{
	int i=0;
	int	Length	=	2;
	int commend	=	0X0103;						//发送给补给站
	//FrameHeader	帧台头
	bullet[0]	=	0xA5;						//帧起始
	bullet[1]	=	(uint8_t)(Length);			//数据帧的低8位
	bullet[2]	=	(uint8_t)(Length >> 8);		//数据帧的高8位
	Append_CRC8_Check_Sum(bullet,5);			//5=3+2
	//添加crc8校验码到帧头尾部，帧头拥有5Bite	5位		
	bullet[5]	=	(uint8_t)(commend);			//命令码
	bullet[6]	=	(uint8_t)(commend>>8);		
	
	/*以下为数据帧，共2组*/
	bullet[7]	=	(uint8_t)(ID);				
	bullet[8]	=	(uint8_t)(num);				
	Append_CRC16_Check_Sum(bullet,11);			//11位
	
	for(i=0;i<11;i++)
	{
		USART2_SendChar(bullet[i]);
	}
}




/************************************************	机器人之间交互数据	**************************************************/
uint8_t	Robot_interactive_date[128];

void Robot_interactive(int date1,int date2,int date3,int date4)	//机器人互交信息·发送
{
	
	int	Length						=	14;			//数据帧长度

	int	robot_communication_ID		=	0X0301;		//机器人间通信ID
	int	Communication_content_ID	=	0X0256;		//数据内容ID：0x0200~0x02FF之间
	int	Communication_sender_ID		=	0X0001;			//发送者ID:红方英雄
	int	Communication_sreceiver_ID	=	0X0007;			//接收者ID:红方哨兵


	Robot_interactive_date[0]	=	 0XA5;
	Robot_interactive_date[1]	=	(uint8_t)(Length);
	Robot_interactive_date[2]	=	(uint8_t)(Length >> 8);		//数据帧的高8位
	Append_CRC8_Check_Sum(Robot_interactive_date,5);			//5=3+2

	Robot_interactive_date[5]	=	(uint8_t)(robot_communication_ID);
	Robot_interactive_date[6]	=	(uint8_t)(robot_communication_ID>>8);
	
	/*以下为数据帧，共14组*/
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

	Append_CRC16_Check_Sum(Robot_interactive_date,23);			//23位
	
}



