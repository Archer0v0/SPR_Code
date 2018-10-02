#ifndef __BSP_INIT_H
#define __BSP_INIT_H
#include "arm_math.h"
#include <stm32f4xx.h>
#include "DataScope_DP.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
extern float pos_x,pos_y,pos_z;
extern float posx,posy;

#include "includes.h"
#include "os_app_hooks.h"

#include "Judge_System.h"
#include "wwdg.h"
extern u8 tr, wr;

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "myiic.h"
#include "mpu6050.h"
extern float yaw,roll,pitch;

#include "mpu6050_driver.h"
#include "mpu6050_i2c.h"

#include "pid.h"
#include "move.h"
extern char follow_flag;
extern float velo_203_output;
extern float velo_204_output;
extern float velo_207_output;     //小枪管云台
extern float pos_207_output ;
extern float velo_206_output;     //小枪管云台
extern float pos_206_output ;     
extern float velo_205_output;     //大枪管云台
extern float pos_205_output ;
extern float velo_208_output;     //小枪管拨弹盘
extern float pos_208_output ;

#include "shoot.h"
extern struct shoot shoot_17mm,shoot_42mm;
extern int pull_v;
extern int pull_17mm_v;
extern int pull_42mm_v;
extern int n;
#include "pwm.h"
#include "led.h"
#include "can.h"
extern int16_t current_203;
extern int16_t current_204;
extern int16_t phaseZ_1 ; 
extern int16_t phaseZ_2 ; 
extern int16_t phaseZ_3 ; 
extern int16_t phaseZ_4 ; 
extern int16_t phaseZ_5 ; 
extern int16_t phaseZ_6 ; 
extern int target_v1;
extern int target_v2;
extern int target_v3;
extern int target_v4;
extern int target_v5;
extern int target_v6;
extern int16_t encoder_205; 			
extern int16_t encoder_206; 
extern int16_t encoder_207; 	
extern int16_t encoder_208;
extern int16_t this_velo_203; 
extern int16_t this_velo_204;
extern int16_t this_velo_205; 			
extern int16_t this_velo_206;
extern int16_t this_velo_207; 
extern int16_t this_velo_208;
extern double this_206_angle;
extern double this_205_angle;
extern int16_t current_07;
extern int16_t velocity_07;
extern int32_t position_07;

#include "telecontrol.h"
extern unsigned char sbus_rx_buffer[18];
extern uint16_t  sbus_channel_temp[15]; 
extern int v1,v2,v3,v4; 
extern float	pitchy;             //手柄右Y
extern float yawx;                //手柄右X   
extern float v_x,v_y;             //键盘速度
extern float vx,vy,wz;            //遥控速度
extern float vsx,vsy,angle;
extern float mousex,mousey;
extern float Yaw_middle_angle;    //116
extern char shooting_flag;
extern char return_flag;
extern char left_flag;             //遥控左上键
extern char right_flag;            //遥控右上键
extern char zero_flag;				     //按键是否按下(本地标志位)
extern uint16_t keyboard;              //键盘

extern double target_pitch_angle;
extern double target_pull_angle;
extern double target_pitch_angle_42mm;     // PITCH轴角度范围
extern double target_yaw_angle_42mm; 
extern double target_pitch_angle_17mm;         //yaw轴目标角度范围
extern double follow_yaw;

void bsp_init(void);	 				    
#endif


