#include "bsp_init.h"

/*******发弹无刷电机PWM——TIM1-PE9-PE11*********/ 
void PWM_SHOOT_Config(void)
{
	TIM_TimeBaseInitTypeDef tim;
	GPIO_InitTypeDef gpio;
	TIM_OCInitTypeDef ocx;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);	
	
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_Speed=GPIO_Speed_100MHz;
	gpio.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_9;
	GPIO_Init(GPIOE,&gpio);
	
	tim.TIM_Period=2000-1;      //周期10ms/CCRx=0-150（180）
	tim.TIM_Prescaler=1680-1;   //预分频1680//100KHz     
	tim.TIM_ClockDivision=TIM_CKD_DIV1;      //不分频
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&tim);                        /*2*///配置定时器
	
	ocx.TIM_OCMode=TIM_OCMode_PWM1;
	ocx.TIM_OutputState=TIM_OutputState_Enable;
	ocx.TIM_OutputNState=TIM_OutputNState_Disable;
	ocx.TIM_Pulse=0;
	ocx.TIM_OCPolarity=TIM_OCPolarity_High;
	ocx.TIM_OCNPolarity=TIM_OCPolarity_High;
	ocx.TIM_OCIdleState=TIM_OCIdleState_Reset;
	ocx.TIM_OCNIdleState=TIM_OCIdleState_Set;
	TIM_OC1Init(TIM1,&ocx);
	TIM_OC3Init(TIM1,&ocx);
	TIM_OC4Init(TIM1,&ocx);
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	/*************************************/
	
	
	
	/*********************************/
	
	TIM_Cmd(TIM1,ENABLE);  
	TIM_SetCompare1(TIM1,0);	
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare4(TIM1,150);
	
	
	TIM1->CCR3 =20;
	TIM1->CCR1 =20;
	delay(10);


	TIM1->CCR3 =40;
	TIM1->CCR1 =40;
	delay(10);

	TIM1->CCR3 =60;
	TIM1->CCR1 =60;
	delay(10);

	TIM1->CCR3 =80;
	TIM1->CCR1 =80;
	delay(10);
	
	TIM1->CCR3 =100;
	TIM1->CCR1 =100;
	delay(50);
}

