#include "bsp_init.h"
void bsp_init(void)
{
	  LED_Init();         //LED初始化
		delay_init(168);  	//时钟初始化
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断分组配置
		Judge_System();
		uart1_init();
		CAN1_init();
		CAN2_init();
		USART6_Configuration();
	  PWM_SHOOT_Config();	
	  mpu_dmp_init();
		MPU6050_42mm_Gyro_calibration();
	
		MPU6050_Initialization();
  	MPU6050_Gyro_calibration();
		WWDG_Init(0x7F,0X5F,WWDG_Prescaler_8);
	/************************防止遥控器初始化时发送错乱数据造成战车暴走************************************/	
	while(1)
	{
			if ((sbus_channel_temp[0]>924)&&(sbus_channel_temp[0]<1124)&& 
				  (sbus_channel_temp[1]>924)&&(sbus_channel_temp[1]<1124)&&
			    (sbus_channel_temp[2]>924)&&(sbus_channel_temp[2]<1124)&&
				  (sbus_channel_temp[3]>924)&&(sbus_channel_temp[3]<1124))
			     break;
	}
	
}

