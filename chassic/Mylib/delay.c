#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
////���ʹ��OS,����������ͷ�ļ�����ucosΪ��������.

#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//֧��OSʱ��ʹ��	  
#include "task.h"
#endif



#if SYSTEM_SUPPORT_OS							//FreeRTOSϵͳ��ʱ

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��

extern void xPortSysTickHandler(void);

 
void SysTick_Handler(void)
{	
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)					
	{
		 xPortSysTickHandler();				
	}
}
			   

void delay_init(u8 SYSCLK)
{
	u32 reload;
	
	//SysTickƵ��ΪHCLK
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=SYSCLK;						
	reload=SYSCLK;						
 	
	reload*=1000000/configTICK_RATE_HZ;	
											
	fac_ms=1000/configTICK_RATE_HZ;		
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	
	SysTick->LOAD=reload; 					
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	

}					

void delay_xms(u16 nms)
{	 		  	  
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
} 
								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				   	 
	ticks=nus*fac_us; 						
	told=SysTick->VAL;        				
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			
		}  
	};
}  

void delay_ms(u16 nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
	{		
		if(nms>=fac_ms)						
		{ 
   			vTaskDelay(nms/fac_ms);	 		
		}
		nms%=fac_ms;						  
	}
	delay_us((u32)(nms*1000));				
}

#else										//��FreeRTOSϵͳ��ʱ
void delay_ms(unsigned int t)				
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

void delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

void delay_xms(u16 nms)
{	 		  	  
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
} 

#endif



























