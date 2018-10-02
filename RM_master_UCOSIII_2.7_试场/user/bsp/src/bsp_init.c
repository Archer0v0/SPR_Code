#include "bsp_init.h"
void bsp_init(void)
{
	  LED_Init();         //LED��ʼ��
		delay_init(168);  	//ʱ�ӳ�ʼ��
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�жϷ�������
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
	/************************��ֹң������ʼ��ʱ���ʹ����������ս������************************************/	
	while(1)
	{
			if ((sbus_channel_temp[0]>924)&&(sbus_channel_temp[0]<1124)&& 
				  (sbus_channel_temp[1]>924)&&(sbus_channel_temp[1]<1124)&&
			    (sbus_channel_temp[2]>924)&&(sbus_channel_temp[2]<1124)&&
				  (sbus_channel_temp[3]>924)&&(sbus_channel_temp[3]<1124))
			     break;
	}
	
}

