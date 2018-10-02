#ifndef _PID_H
#define _PID_H
#include <stm32f4xx.h>

#define ESC_MAX 9500
#define ESC_MAX_AIM 4000
#define ERROR_MAX 5000


float Velo_Con_201(float curr_velo_201,float target_velo_201);
float Velo_Con_202(float curr_velo_202,float target_velo_202);
float Velo_Con_203(float curr_velo_203,float target_velo_203);
float Velo_Con_204(float curr_velo_204,float target_velo_204);

float Velo_Con_42mm_l(float curr_velo_42mm_l,float target_velo_42mm_l);
float Velo_Con_42mm_r(float curr_velo_42mm_r,float target_velo_42mm_r);

float Velo_pull_42mm(float curr_pos_pull,float target_pos_pull);
float Velo_pull_17mm(float curr_velo_pull,float target_velo_pull);

float Pos_Con_205(float curr_pos_205,float target_pos_205);
float Velo_Con_205(float curr_pos_205,float target_pos_205);
float Velo_Con_206(float curr_velo_206,float target_velo_206);
float Pos_Con_206(float curr_pos_206,float target_pos_206);
float PITCH_Velo_Con_207(float curr_velo_207,float target_velo_207);
float PITCH_Pos_Con_207(float curr_pos_207,float target_pos_207);

float Velo_Con_pull(float curr_velo_pull,float target_velo_pull);
float Pos_Con_pull(float curr_pos_pull,float target_pos_pull);

float SXT_pitch(float target_pitch_position,float middle_pitch_position);
float SXT_yaw(float target_yaw_position,float middle_yaw_position);
#endif
