#ifndef __JUDGE_H__
#define __JUDGE_H__

#include "main.h"

/*
	������ ID ˵��
	������ ���ܽ���
	0��0001 ����������Ϣ
	0��0002 ʵʱѪ���仯����
	0��0003 ʵʱ�������
	0��0005 ѧ���ϴ��Զ�������
*/

//λ��״̬�ṹ��
typedef __packed struct
{
	uint8_t flag;	//0��Ч		1��Ч
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t compass;
	
}tLocData;				//˵������λ���ݵ�λΪ�ף�С�������λΪ��Ч����

//����������Ϣ�ṹ��
typedef __packed struct
{
	uint32_t remainTime;		//����ʣ��ʱ�䣨�ӵ���ʱ�����ӿ�ʼ���㣬��λ s��
	uint16_t remainLifeValue;	//������ʣ��Ѫ��
	float realChassisOutV;		//ʵʱ���������ѹ����λ V��
	float realChassisOutA;		//ʵʱ���������������λ A��
	tLocData locData;			//LOC ״̬���� tLocData �ṹ�嶨��
	float remainPower;			//remainPower: ʣ����������ֵ 60J��
	
}tGameInfo;

//ʵʱѪ���仯��Ϣ�ṹ��
typedef __packed struct
{
	uint8_t weakId:4;
	uint8_t way:4;
	uint16_t value;
	
}tRealBloodChangedData;

//ʵʱ�����Ϣ�ṹ��
typedef __packed struct
{
	float realBulletShootSpeed;		//�ӵ�ʵʱ���٣�m/s��
	float realBulletShootFreq;		//�ӵ�ʵʱ��Ƶ���� /s��
	float realGolfShootSpeed;		//�߶���ʵʱ���� (m/s Ӣ�ۻ����� )
	float realGolfShootFreq;		//�߶���ʵʱ��Ƶ ( �� /s?Ӣ�ۻ����� )
	
}tRealShootData;

unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);

#endif
