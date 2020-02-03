/***************************dbus.c*****************************/
#include "dbus.h"
#include "stdio.h"
#include "delay.h"
#include "uart4.h"
#include "message.h"


#define rc_abs(x) ((x)>0? (x):(-(x)))
char UART4temp;
char Access_buffer;
unsigned char dbus_buf0[DBUS_BUF_SIZE];
//unsigned char dbus_buf1[DBUS_BUF_SIZE];

int DBBUSS[4] = {0};
//ң������������
#define RC_Channal_error_value 660
#define RC_Channal_datas_deads 15


//ң�������Ʊ���
DBUS rc_ctrl = {0};

static int16_t rc_ctrl_data_dead(int16_t rc)
{
		if (rc_abs(rc) < RC_Channal_datas_deads)
		{
				rc = 0;
		}
		if (rc > RC_Channal_error_value)
		{
				rc = RC_Channal_error_value;
		}
		if (rc < -RC_Channal_error_value) 
		{
				rc = -RC_Channal_error_value;
		}
		return rc;
}



void DBUS_Enc(const DBUS* rc_ctrl,unsigned char* pbuf)
{	
    pbuf[0]  = rc_ctrl->rc.ch0&0xff;
    pbuf[1]  = (rc_ctrl->rc.ch1<<3) | (rc_ctrl->rc.ch0>>8);
    pbuf[2]  = (rc_ctrl->rc.ch2<<6) | (rc_ctrl->rc.ch1>>5);
    pbuf[3]  = rc_ctrl->rc.ch2;
    pbuf[4]  = (rc_ctrl->rc.ch3<<1) | (rc_ctrl->rc.ch2>>10);
    pbuf[5]  = (rc_ctrl->rc.s2<<6) | (rc_ctrl->rc.s1<<4) | (rc_ctrl->rc.ch3>>7);
    pbuf[6]  = rc_ctrl->mouse.x;
    pbuf[7]  = rc_ctrl->mouse.x>>8;
    pbuf[8]  = rc_ctrl->mouse.y;
    pbuf[9]  = rc_ctrl->mouse.y>>8;
    pbuf[10] = rc_ctrl->mouse.z;
    pbuf[11] = rc_ctrl->mouse.z>>8;
    pbuf[12] = rc_ctrl->mouse.l;
    pbuf[13] = rc_ctrl->mouse.r;
    pbuf[14] = rc_ctrl->key.v;
    pbuf[15] = rc_ctrl->key.v>>8;
    pbuf[16] = rc_ctrl->res;
    pbuf[17] = rc_ctrl->res>>8;
}

void DBUS_Dec(DBUS* rc_ctrl,volatile const unsigned char* pbuf)
{
	if (rc_ctrl == NULL || pbuf == NULL)
	{
		 return;
	}
	rc_ctrl->rc.ch0   = (pbuf[0] | (pbuf[1] << 8)) & 0x07ff;          //!< Channel 0  	����		w��
	rc_ctrl->rc.ch1   = ((pbuf[1] >> 3) | (pbuf[2] << 5)) & 0x07ff;   //!< Channel 1    ����		��
	rc_ctrl->rc.ch2   = ((pbuf[2] >> 6) | (pbuf[3] << 2) | (pbuf[4] << 10)) & 0x07ff;          //!< Channel 2    ����        ��         
											
	rc_ctrl->rc.ch3   = ((pbuf[4] >> 1) | (pbuf[5] << 7)) & 0x07ff;   //!< Channel 3   	����		��
	rc_ctrl->rc.s1    = ((pbuf[5] >> 4) & 0x000C) >> 2;                    //!< Switch left			������         
	rc_ctrl->rc.s2    = ((pbuf[5] >> 4) & 0x0003);                         //!< Switch right  		������

	rc_ctrl->mouse.x  = pbuf[6] | (pbuf[7] << 8);                    //��� X��
	rc_ctrl->mouse.y  = pbuf[8] | (pbuf[9] << 8);                    //��� y��
	rc_ctrl->mouse.z  = pbuf[10] | (pbuf[11] << 8);                  //��� z��
	rc_ctrl->mouse.l  = pbuf[12];                                        // ������ ����Ϊ1 ����Ϊ0
	rc_ctrl->mouse.r  = pbuf[13];                                        //��� ��  �� ����Ϊ1 ����Ϊ0

	rc_ctrl->key.v    = pbuf[14] | (pbuf[15] << 8);                    // W:1 S:2 A:4 D:8 Q:64 E:128 R:256 F:512 G:1024 Z:2048 X:4096 C:8192 V:16384 B:32768 Shift:16 ctrl:32 
	rc_ctrl->res      = pbuf[16] | (pbuf[17] << 8);                      //  ң���� ����

	rc_ctrl->rc.ch0   = rc_ctrl_data_dead(rc_ctrl->rc.ch0 - CH_VALUE_OFFSET);
	rc_ctrl->rc.ch1   = rc_ctrl_data_dead(rc_ctrl->rc.ch1 - CH_VALUE_OFFSET);
	rc_ctrl->rc.ch2   = rc_ctrl_data_dead(rc_ctrl->rc.ch2 - CH_VALUE_OFFSET);
	rc_ctrl->rc.ch3   = rc_ctrl_data_dead(rc_ctrl->rc.ch3 - CH_VALUE_OFFSET);
	
	
}


void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(UART4);
    }
	else if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	{
	static uint16_t this_time_rx_len = 0;
	USART_ReceiveData(UART4);
			
			Access_buffer=DMA_GetCurrentMemoryTarget(DMA1_Stream2);		//��ȡ���ڷ��ʵĻ�����
			if (Access_buffer == 0)
			{
					DMA_Cmd(DMA1_Stream2, DISABLE);								//�ر�DMA����
					this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream2);
					DMA_SetCurrDataCounter(DMA1_Stream2, DBUSLength + DBUSBackLength);
					DMA1_Stream2->CR |= DMA_SxCR_CT;
					DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);	
					DMA_Cmd(DMA1_Stream2, ENABLE);	
					if(this_time_rx_len == DBUS_BUF_SIZE)
					{
						//����ң��������
						DBUS_Dec(&rc_ctrl,dbus_buf0);
					}
			}
			else
			{
					DMA_Cmd(DMA1_Stream2, DISABLE);	
					this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream2);
					DMA_SetCurrDataCounter(DMA1_Stream2, SBUS_RX_BUF_NUM);
					DMA1_Stream2->CR &= ~(DMA_SxCR_CT);
					DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);	
					DMA_Cmd(DMA1_Stream2, ENABLE);	
					if(this_time_rx_len == DBUS_BUF_SIZE)
					{
						//����ң��������
						DBUS_Dec(&rc_ctrl,dbus_buf0);  
					}
			}
	}
}

