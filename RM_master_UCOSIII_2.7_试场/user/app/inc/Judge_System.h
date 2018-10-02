#ifndef __Judge_System_H
#define __Judge_System_H
#include <stm32f4xx.h>
#include <stdio.h>

typedef __packed struct   //比赛机器人状态2018
{
	uint16_t stageRemianTime;
  uint8_t gameProgress;
	uint8_t robotLevel;
	uint16_t remainHP;
	uint16_t maxHP;
	
}extGameRobotState_t;

typedef __packed struct    //伤害数据2018
{
  uint8_t armorType :4;
	uint8_t hurtType  :4;

}extRobotHurt_t;

typedef __packed struct    //实时射击信息2018
{
  uint8_t bulletType;    //弹丸类型
	uint8_t bulletFreq;    //射频
	float bulletSpeed;     //射速
}extShootData_t;

               

typedef __packed struct       //实时功率热量数据2018
{

  float chassisVolt;          //底盘输出电压
  float chassisCurrent;       //底盘输出电流
  float chassisPower;         //底盘输出功率
	float chassisPowerBuffer;   //底盘功率缓冲
	uint16_t shooterHeat0;      //17mm弹丸热量
	uint16_t shooterHeat1;      //42mm
	
}extPowerHeatData_t;                 

typedef __packed struct    //场地交互数据2018
{
  uint8_t cardType;
	uint8_t cardIdx;
	
}extRfidDetect_t;

typedef __packed struct    //比赛胜负数据2018
{
  uint8_t winner;

}extGameResult_t;

typedef __packed struct    //buff获取数据2018
{
  uint8_t buffType;
	uint8_t buffAddition;

}extGetBuff_t;

typedef __packed struct    //机器人位置朝向信息2018
{
  float x;
	float y;
	float z;
	float yaw;

}extGameRobotPos_t;

typedef __packed struct    //自定义数据2018
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




