#include "bsp_init.h"
#include "shoot.h"

struct shoot shoot_17mm,shoot_42mm;
int a=1,b=0,c=0;
void shoot_mode(){
	switch(left_flag){
		case 1:{
			shoot_17mm.friction_flag=0;
			shoot_42mm.friction_flag=0;
			if(a){
				target_pitch_angle=PITCH_42MM_MID;
				b=1;c=1;a=0;
			}
			target_pitch_angle_42mm=target_pitch_angle;
			target_pitch_angle_17mm=PITCH_17MM_MID;
		}break;
		case 3:{
			shoot_17mm.friction_flag=2;
			shoot_42mm.friction_flag=0;
			if(b){
				target_pitch_angle=235;
				b=0;c=1;a=1;
			}
			target_pitch_angle_42mm=PITCH_42MM_MID;
			target_pitch_angle_17mm=target_pitch_angle;
		}break;
		case 2:{
			shoot_17mm.friction_flag=1;
			shoot_42mm.friction_flag=1;
			if(c){
				target_pitch_angle=PITCH_42MM_MID;
				b=1;c=0;a=1;
			}
			target_pitch_angle_42mm=target_pitch_angle;
			target_pitch_angle_17mm=target_pitch_angle_17mm;
		}break;
//		case shoot_all_flag:{
//			shoot_17mm.friction_flag=1;
//			shoot_42mm.friction_flag=1;
//			
//			target_pitch_angle_42mm=target_pitch_angle;
//			target_pitch_angle_17mm=target_pitch_angle_17mm;
//		}break;
		default:shoot_mode_choice=1;
	}
}

double this_208_angle=0;

float velo_208_output= 0.0;      
float pos_208_output = 0.0;

float velo_203_output= 0.0;
float velo_204_output= 0.0;
int pull_v=0;
int pull_17mm_v=0;
int pull_42mm_v=0;
int n=-1;
float error_v=1;
void shooting_42mm(void)
{
	int Heat_max;
	switch(extGameRobotstate_t.robotLevel){
		case 1:Heat_max=40;break;
		case 2:Heat_max=80;break;
		case 3:Heat_max=160;break;
	}
	error_v=extPowerHeatdata_t.shooterHeat1;
	if(left_flag==2&&right_flag==2)shoot_42mm.shoot_flag=1;
	if(shoot_42mm.shoot_flag&&shoot_42mm.friction_flag&&(extPowerHeatdata_t.shooterHeat1<Heat_max)){
		pull_v=30000;
		delay_ms(160);
		pull_v=0;
		delay_ms(150);
		if(extPowerHeatdata_t.shooterHeat1>error_v&&n>0)n-=1;
		shoot_42mm.shoot_flag=0;
	}
	else pull_v=0;
}
void pull_17mm(void)//maybe chao re liang
{
	int lv=0;
	switch(extGameRobotstate_t.maxHP){
			case 2000:lv=1;break;
			case 2500:lv=2;break;
			case 3000:lv=3;break;
	}
	if(shoot_17mm.shoot_flag&&shoot_17mm.friction_flag&&
		((120*(int)(lv)-(int)(extPowerHeatdata_t.shooterHeat0))>40)){
			switch(lv){
			case 1:pull_17mm_v=1500;break;
			case 2:pull_17mm_v=2000;break;
			case 3:pull_17mm_v=2500;break;
		}
	}
	else pull_17mm_v=0;
	shoot_17mm.shoot_flag=0;
	if((left_flag==2||left_flag==3)&&right_flag==2)pull_17mm_v=4000;
}
void pull_42mm(void)//maybe gai
{
	if(n<3&&shoot_42mm.friction_flag){
		pull_42mm_v=1000;
	}
	else pull_42mm_v=0;
  if(!Pull_scan&&n<6){
		n+=1;
		pull_42mm_v=0;
		delay_ms(100);
	}
	if(current_204<-4100) {
		velo_204_output=-500;
		delay_ms(50);
	}
}
void friction_17mm(void)
{
	switch(shoot_17mm.friction_flag){
		case 0:{
			TIM1->CCR1 =100;
			TIM1->CCR3 =100;	
		}break;
		case 1:{
				TIM1->CCR1 =130;//×óÄ¦²ÁÂÖ
				TIM1->CCR3 =130;//ÓÒÄ¦²ÁÂÖ
		}break;
		case 2:{
			if(TIM1->CCR1<(140) 
			||TIM1->CCR3<(140)){
				TIM1->CCR1 +=PWM_ADD;//×óÄ¦²ÁÂÖ
				TIM1->CCR3 +=PWM_ADD;//ÓÒÄ¦²ÁÂÖ
				}  
		}break;
		default:{
			TIM1->CCR1 =100;
			TIM1->CCR3 =100;	
		}
	}
}
void friction_42mm(void)
{
	int16_t v_Fir=0;
	if(shoot_42mm.friction_flag)v_Fir=5200;
	else v_Fir=0;
	
	target_v5=Velo_Con_42mm_l( phaseZ_5,v_Fir);
	target_v6=Velo_Con_42mm_r( phaseZ_6,-v_Fir); 
}

