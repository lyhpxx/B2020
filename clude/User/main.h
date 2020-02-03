#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"
#include "delay.h"

#include "laser.h"
//#include "led.h"
#include "tim2.h"

#include "Nvic.h"			//��� ���� �ж� ���ã��������� ���ȼ�

#include "usart1.h"			//���� 1 ���� ��ӡ���ݡ�����
#include "usart3.h"			//���� 6 ���� ������2

#include "dbus.h"			/*  ң����  */


//#include "judge.h"			/* ����ϵͳ */

/*��ʱ������*/
#include "pwm1.h"			//TIM1			���1-4

#include "can1.h"			//CAN1
#include "can2.h"			//CAN2


/*	MPU6050	���� */
#include "mpu6050_driver.h"
#include "mpu6050_i2c.h"
#include "mpu6050_interrupt.h"
#include "mpu6050_process.h"


#include "start.h"


#include "stdint.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "stdlib.h"	 
#include "sys.h" 


#endif 


