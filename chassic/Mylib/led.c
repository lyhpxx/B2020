#include "led.h"

/*----LED_GREEN----PA6-----'0' is on,'1' is off */
/*----LED_RED------PA7-----'0' is on,'1' is off */

void Led_Configuration(void)
{
    GPIO_InitTypeDef gpio;
    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOC,ENABLE);
		
		gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7;
		gpio.GPIO_Mode = GPIO_Mode_OUT;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOE,&gpio);
        gpio.GPIO_Pin = GPIO_Pin_4;
        GPIO_Init(GPIOC,&gpio);
    
  	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;
    gpio.GPIO_Mode = GPIO_Mode_IN;
	  gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	  gpio.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOC,&gpio);
  
    LED_GREEN_ON();
    LED_RED_ON();
}
