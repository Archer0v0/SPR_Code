#ifndef __Judge_System_H
#define __Judge_System_H
#include <stm32f4xx.h>
#include <stdio.h>

typedef __packed struct   //����������״̬2018
{
	uint16_t stageRemianTime;
  uint8_t gameProgress;
	uint8_t robotLevel;
	uint16_t remainHP;
	uint16_t maxHP;
	
}extGameRobotState_t;

typedef __packed struct    //�˺�����2018
{
  uint8_t armorType :4;
	uint8_t hurtType  :4;

}extRobotHurt_t;

typedef __packed struct    //ʵʱ�����Ϣ2018
{
  uint8_t bulletType;    //��������
	uint8_t bulletFreq;    //��Ƶ
	float bulletSpeed;     //����
}extShootData_t;

               

typedef __packed struct       //ʵʱ������������2018
{

  float chassisVolt;          //���������ѹ
  float chassisCurrent;       //�����������
  float chassisPower;         //�����������
	float chassisPowerBuffer;   //���̹��ʻ���
	uint16_t shooterHeat0;      //17mm��������
	uint16_t shooterHeat1;      //42mm
	
}extPowerHeatData_t;                 

typedef __packed struct    //���ؽ�������2018
{
  uint8_t cardType;
	uint8_t cardIdx;
	
}extRfidDetect_t;

typedef __packed struct    //����ʤ������2018
{
  uint8_t winner;

}extGameResult_t;

typedef __packed struct    //buff��ȡ����2018
{
  uint8_t buffType;
	uint8_t buffAddition;

}extGetBuff_t;

typedef __packed struct    //������λ�ó�����Ϣ2018
{
  float x;
	float y;
	float z;
	float yaw;

}extGameRobotPos_t;

typedef __packed struct    //�Զ�������2018
{
  float data1;
	float data2;
	float data3;
	float data4;

}extShowData_t;

extern u8 Judge_Data[100];

extern extPowerHeatData_t extPowerHeatdata_t;
extern extShootData_t extShootdata_t;
extern extGameRobotState_t extGameRobotstate_t;

void Judge_System(void);
void DATA_0001(u8 Res);
void DATA_0002(u8 Res);
void DATA_0003(u8 Res);
void DATA_0004(u8 Res);
void DATA_0005(u8 Res);
void DATA_0006(u8 Res);
void DATA_0007(u8 Res);
void DATA_0008(u8 Res);
void DATA_0100(u8 Res);
void Get_Data(void);

#endif




