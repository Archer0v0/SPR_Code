#include "bsp_init.h"
#include "move.h"
char follow_flag=0;
char auto_aim_flag=0;
double spin_z=0;
double this_205_angle=0;
double this_206_angle=0;
double this_207_angle=0;

int target_v1;
int target_v2;
int target_v3;
int target_v4;
int target_v5;
int target_v6;

double target_pitch_angle_42mm=PITCH_42MM_MID;     // 175-210，PITCH轴角度范围
double target_yaw_angle_42mm=YAW_MID; //77-255 ，yaw轴目标角度范围
double target_pitch_angle_17mm=PITCH_17MM_MID;         
double follow_yaw=0;
float velo_207_output= 0.0;       //小枪管pitch轴速度环函数输出值
float pos_207_output = 0.0;       //小枪管pitch轴位置环函数输出值
float velo_206_output= 0.0;       //pitch轴速度环函数输出值
float pos_206_output = 0.0;       //pitch轴位置环函数输出值
float velo_205_output= 0.0;       //yaw轴速度环函数输出值
float pos_205_output = 0.0;  

void aim(void)
{	
	this_205_angle =encoder_205* 0.044;        //pitch
	this_206_angle =encoder_206* 0.044;        //yaw
	this_207_angle =encoder_207* 0.044;        //yaw

	
	if (target_pitch_angle_42mm>PITCH_MAX)	target_pitch_angle_42mm = PITCH_MAX;//77
  if (target_pitch_angle_42mm<PITCH_MIN)	target_pitch_angle_42mm = PITCH_MIN;
	pos_205_output =Pos_Con_205(this_205_angle,target_pitch_angle_42mm);                 
  velo_205_output =Velo_Con_205(MPU6050_42mm_Real_Data.Gyro_Y,pos_205_output);       

	
	if (target_yaw_angle_42mm>YAW_MAX)	target_yaw_angle_42mm = YAW_MAX;
  if (target_yaw_angle_42mm<YAW_MIN)	target_yaw_angle_42mm = YAW_MIN;
	if(follow_flag) pos_206_output = Pos_Con_206(yaw,follow_yaw);
	else pos_206_output = Pos_Con_206(-this_206_angle,-target_yaw_angle_42mm);
  velo_206_output =Velo_Con_206(MPU6050_42mm_Real_Data.Gyro_Z,pos_206_output); 	
	
		
	if (target_pitch_angle_17mm>PITCH_17MM_MAX)	target_pitch_angle_17mm = PITCH_17MM_MAX;
  if (target_pitch_angle_17mm<PITCH_17MM_MIN)	target_pitch_angle_17mm = PITCH_17MM_MIN;//235
	pos_207_output =PITCH_Pos_Con_207(this_207_angle,target_pitch_angle_17mm);                 
  velo_207_output =PITCH_Velo_Con_207(MPU6050_17mm_Real_Data.Gyro_Y,pos_207_output);
}



void common_mode(void)
{
	if((get_mode_choice==get_ready_flag||get_mode_choice==stop_get||get_mode_choice==get_flag)){
		v1=  50*(vy+vx+wz-70*mousex-v_x-v_y+spin_z);//vy前后，vx左右，wz旋转
		v2=  50*(vy-vx+wz-70*mousex+v_x-v_y+spin_z);
		v3=  -50*(vy+vx-wz+70*mousex-v_x-v_y-spin_z);
		v4=  -50*(vy-vx-wz+70*mousex+v_x-v_y-spin_z);
	}
	else{
		if(mousex>1.5)mousex=1.5;
	  if(mousex<-1.5)mousex=-1.5;
	  v1=  50*(vy+vx+wz+70*mousex-v_x+v_y+spin_z);//vy前后，vx左右，wz旋转
		v2=  50*(vy-vx+wz+70*mousex+v_x+v_y+spin_z);
		v3=  -50*(vy+vx-wz-70*mousex-v_x+v_y-spin_z);
		v4=  -50*(vy-vx-wz-70*mousex+v_x+v_y-spin_z);
	}
}
void follow_mode(void)
{
	if(follow_flag)spin_z=4*(this_206_angle-YAW_MID)*sqrt(abs(this_206_angle-YAW_MID)/YAW_EXTENT);
	if(spin_z>100)spin_z=100;
	if(spin_z<-100)spin_z=-100;
}
void move(void)
{
		target_v1=Velo_Con_201( phaseZ_1,v1);
		target_v2=Velo_Con_202( phaseZ_2,v2); 
		target_v3=Velo_Con_203( phaseZ_3,v3);
		target_v4=Velo_Con_204( phaseZ_4,v4);

		CAN2_SetMsg(target_v1,target_v2,target_v3,target_v4);
}

void v_limit(int max)
{
	if(v1>max)v1=max;
	if(v1<-max)v1=-max;
	if(v2>max)v2=max;
	if(v2<-max)v2=-max;
	if(v3>max)v3=max;
	if(v3<-max)v3=-max;
	if(v4>max)v4=max;
	if(v4<-max)v4=-max;
}
void Get_mode(void)
{
	switch(get_mode_choice){
		case get_ready_flag:	{Up;TIM_SetCompare4(TIM1,249);}												break;	
		case get_flag:  			{Get;delay(1000);Throw;get_mode_choice=3;}						break;
		case stop_get:				;																											break;
		case get_over_flag:		{Down;TIM_SetCompare4(TIM1,150);}											break;
		default:							get_mode_choice=1;
	}
		
}
float yaw_target=0;
void Spin_mode(void)
{
	switch(spin_mode_choice){
		case back_spin:{if(follow_flag) follow_flag=~follow_flag;
										if(!yaw_target){
											if(yaw<0)	yaw_target=yaw+180;
											else	if(yaw>0)yaw_target=yaw-180;
										}
										else;
										
										if((yaw-yaw_target)>6)
											spin_z=+100;
										else if((yaw-yaw_target)<-6)
											spin_z=-100;
										else	{
											spin_z=0;
											spin_mode_choice=stop_spin;
										}
									}																		break;
		case defense_spin:	if(follow_flag) follow_flag=~follow_flag;
									spin_z=100;yaw_target=0;						break;
		case stop_spin:			spin_z=0;	yaw_target=0;				break;
		default:						spin_mode_choice=stop_spin;
	}	
}
//void get_mode(void)
//{
//	int i=0;
//	while(i==3)
//	{
//		vx=-100;
//	  while(!Left_scan)common_mode();
//	  while(Left_scan)common_mode();
//	  vx=80;
//	  while(Right_scan)common_mode();
//	  Get;delay_ms(1000);Throw;
//	  i++;
//	}
//}

