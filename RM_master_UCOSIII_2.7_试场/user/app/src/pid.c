#include "bsp_init.h"

float Velo_Con_201(float curr_velo_201,float target_velo_201)//
{
	 float v_p =6.5;//16
//	v_p*=sqrt(extPowerHeatdata_t.chassisPowerBuffer/60.0f);
	const float v_i=0.01; //0.02
	const float v_d = 0;//
    
	static float error_v[2] = {0.0,0.0};
	static float error_sum=0;
	static float output = 0;

	error_v[0] = error_v[1];		
  error_v[1] = +target_velo_201-curr_velo_201;
	error_sum += error_v[1]; 
		
		if(error_sum>ERROR_MAX) error_sum = ERROR_MAX;
		if(error_sum<-ERROR_MAX) error_sum = -ERROR_MAX;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
    if(output > ESC_MAX)  output = ESC_MAX;
    if(output < -ESC_MAX) output = -ESC_MAX;
		output*=sqrt(extPowerHeatdata_t.chassisPowerBuffer/60.f);
    return output;
}

float Velo_Con_202(float curr_velo_202,float target_velo_202)
{
	float v_p =6.5;//16
//	v_p*=sqrt(extPowerHeatdata_t.chassisPowerBuffer/60.0f);
	const float v_i=0.01; //0.02
	const float v_d = 0;//
	static float error_v[2] = {0.0,0.0};
	static float error_sum=0;
	static float output = 0;
	
	error_v[0] = error_v[1];
  error_v[1] = target_velo_202-curr_velo_202; //
	error_sum += error_v[1]; 
		
		if(error_sum>ERROR_MAX) error_sum = ERROR_MAX;
		if(error_sum<-ERROR_MAX) error_sum = -ERROR_MAX;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
    if(output > ESC_MAX)  output = ESC_MAX;
    if(output < -ESC_MAX) output = -ESC_MAX;
		output*=sqrt(extPowerHeatdata_t.chassisPowerBuffer/60.f);
    return output;
}

float Velo_Con_203(float curr_velo_203,float target_velo_203)
 {
		float v_p = 6.5; //
//	 v_p*=sqrt(extPowerHeatdata_t.chassisPowerBuffer/60.0f);
		const float v_i = 0.01; //
		const float v_d = 0;   //
    
    static float error_v[2] = {0.0,0.0};
		static float error_sum=0;
    static float output = 0;
    
    error_v[0] = error_v[1];
		error_v[1] = target_velo_203-curr_velo_203; // 
		error_sum += error_v[1]; 
		
		if(error_sum>ERROR_MAX) error_sum = ERROR_MAX;
		if(error_sum<-ERROR_MAX) error_sum = -ERROR_MAX;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
		 
    if(output > ESC_MAX)  output = ESC_MAX;
    if(output < -ESC_MAX) output = -ESC_MAX;
		output*=sqrt(extPowerHeatdata_t.chassisPowerBuffer/60.f);
		return output;
}


float Velo_Con_204(float curr_velo_204,float target_velo_204)
 {
		float v_p =6.5;//     
//		v_p*=sqrt(extPowerHeatdata_t.chassisPowerBuffer/60.0f);	 
		const float v_i=0.01;  //
		const float v_d = 0;//
    
    static float error_v[2] = {0.0,0.0};
		static float error_sum=0;
    static float output = 0;

    error_v[0] = error_v[1];
		error_v[1] = target_velo_204-curr_velo_204; // 
		error_sum += error_v[1]; 
		
		if(error_sum>ERROR_MAX) error_sum = ERROR_MAX;
		if(error_sum<-ERROR_MAX) error_sum = -ERROR_MAX;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
   
    if(output > ESC_MAX)  output = ESC_MAX;
    if(output < -ESC_MAX) output = -ESC_MAX;
	  output*=sqrt(extPowerHeatdata_t.chassisPowerBuffer/60.f);
    return output;
}
 
float Velo_Con_42mm_l(float curr_velo_42mm_l,float target_velo_42mm_l)//
 {
		const float v_p =12;//              
		const float v_i=0;  //
		const float v_d =0;//
    
    static float error_v[2] = {0.0,0.0};
		static float error_sum=0;
    static float output = 0;

    error_v[0] = error_v[1];
		error_v[1] = target_velo_42mm_l-curr_velo_42mm_l; // 
		error_sum += error_v[1]; 
		
		if(error_sum>100) error_sum = 100;
		if(error_sum<-100) error_sum = -100;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
   
    if(output > 5500)  output = 5500;
    if(output < -5500) output = -5500;
	
    return output;
}
 
float Velo_Con_42mm_r(float curr_velo_42mm_r,float target_velo_42mm_r)
 {
		const float v_p =12;//              
		const float v_i=0;  //
		const float v_d = 0;//
    
    static float error_v[2] = {0.0,0.0};
		static float error_sum=0;
    static float output = 0;

    error_v[0] = error_v[1];
		error_v[1] = target_velo_42mm_r-curr_velo_42mm_r; // 
		error_sum += error_v[1]; 
		
		if(error_sum>100) error_sum = 100;
		if(error_sum<-100) error_sum = -100;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
   
    if(output > 5500)  output = 5500;
    if(output < -5500) output = -5500;
	
    return output;
}
/**************************云台双环pid控制位置******************************************************/
/***************小云台PITCH轴 位置环*************************/
float PITCH_Pos_Con_207(float curr_pos_207,float target_pos_207)
{
 
    const float P_p =40;//20
    const float P_i = 0;
    const float P_d = 0.1;													 
    static float error_p[2] = {0.0,0.0};
    static float output = 0;
    static float error_p_sum= 0;
    
    error_p[0] = error_p[1];
    error_p[1] = curr_pos_207-target_pos_207;
    error_p_sum += error_p[1]; 
    if(error_p_sum>500) error_p_sum = 500;//500000;
		if(error_p_sum<-500) error_p_sum = -500;
    output =  error_p[1]  * P_p 
           +  error_p_sum * P_i 
           + (error_p[1] - error_p[0]) * P_d;

    if(output > 15000)  
			output =  15000;
    if(output < -15000) 
			output = -15000;
	
	return output;
}
/***********************小云台PITCH轴 速度环*********************/
float PITCH_Velo_Con_207(float curr_velo_207,float target_velo_207)
{
	const float v_p =40;//28//60//10//60.0;               //20.0   //使偏差增大或减小
	const float v_i=0;//0.6//0.1//0                 //0.0002
	const float v_d =0;//3//0.5//2                     //0
    
	static float error_v[2] = {0.0,0.0};
	static float error_sum=0;
	static float output = 0;
 
	error_v[0] = error_v[1];		
  error_v[1] = curr_velo_207-	target_velo_207; 
	error_sum += error_v[1];
		
		if(error_sum>500) error_sum = 500;
		if(error_sum<-500) error_sum = -500;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
		   
     
    if(output > 15000)  output = 15000;
    if(output < -15000) output =-15000;
	
    return output;
}                  

/******************yaw 位置环****************************/

float Pos_Con_206(float curr_pos_206,float target_pos_206)
{
    const float P_p =40;//20
    const float P_i = 0.5;
    const float P_d = 1;//0
    static float error_p[2] = {0.0,0.0};
    static float output = 0;
    static float error_p_sum= 0;
    
    error_p[0] = error_p[1];
    error_p[1] =  curr_pos_206-target_pos_206;
		if(error_p[1]>350) error_p[1]-=360;							//消除突变			
		else if(error_p[1]<-350) error_p[1]+=360;
		else;
		
    error_p_sum += error_p[1]; 
    if(error_p_sum>500) error_p_sum = 500;//500000;
		if(error_p_sum<-500) error_p_sum = -500;
    output =  error_p[1]  * P_p 
           +  error_p_sum * P_i 
           + (error_p[1] - error_p[0]) * P_d;

    if(output > ESC_MAX_AIM)  
			output =  ESC_MAX_AIM;
    if(output < -ESC_MAX_AIM) 
			output = -ESC_MAX_AIM;
	
	return output;
}
/**********************yaw 速度环*********************/
float Velo_Con_206(float curr_velo_206,float target_velo_206)
{
	const float v_p = 25;//5.5;//60//10//60.0;               //20.0   //使偏差增大或减小
	const float v_i = 0; //0//0.1//0                 //0.0002
	const float v_d = 0.1;//2//0.5//2                     //0
    
    static float error_v[2] = {0.0,0.0};
		static float error_sum=0;
    static float output = 0;

		error_v[0] = error_v[1];
		error_v[1] = -curr_velo_206*10+target_velo_206; 
		error_sum += error_v[1];
		if(error_sum>ERROR_MAX) error_sum = ERROR_MAX;
		if(error_sum<-ERROR_MAX) error_sum = -ERROR_MAX;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式	   
    if(output > ESC_MAX_AIM)  output = ESC_MAX_AIM;
    if(output < -ESC_MAX_AIM) output = -ESC_MAX_AIM;
	
    return output;
}




/***************PITCH轴 位置环*************************/
float Pos_Con_205(float curr_pos_205,float target_pos_205)
{
 
    const float P_p =13;//20
    const float P_i = 0.05;
    const float P_d = 0.3;													 
    static float error_p[2] = {0.0,0.0};
    static float output = 0;
    static float error_p_sum= 0;
    
    error_p[0] = error_p[1];
    error_p[1] = - curr_pos_205+target_pos_205;
//		if(error_p[1]<30)
    error_p_sum += error_p[1]; 
    if(error_p_sum>500) error_p_sum = 500;//500000;
		if(error_p_sum<-500) error_p_sum = -500;
    output =  error_p[1]  * P_p 
           +  error_p_sum * P_i 
           + (error_p[1] - error_p[0]) * P_d;

    if(output > ESC_MAX_AIM)  
			output =  ESC_MAX_AIM;
    if(output < -ESC_MAX_AIM) 
			output = -ESC_MAX_AIM;
	
	return output;
}
/***********************PITCH轴 速度环*********************/
float Velo_Con_205(float curr_velo_205,float target_velo_205)
{
	const float v_p =13;//28//60//10//60.0;               //20.0   //使偏差增大或减小
	const float v_i=0;//0.6//0.1//0                 //0.0002
	const float v_d =0;//3//0.5//2                     //0
    
	static float error_v[2] = {0.0,0.0};
	static float error_sum=0;
	static float output = 0;
 
	error_v[0] = error_v[1];		
  error_v[1] = curr_velo_205*5-	target_velo_205; 
	error_sum += error_v[1];
		
		if(error_sum>ERROR_MAX) error_sum = ERROR_MAX;
		if(error_sum<-ERROR_MAX) error_sum = -ERROR_MAX;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
		   
     
    if(output > ESC_MAX_AIM)  output = ESC_MAX_AIM;
    if(output < -ESC_MAX_AIM) output =-ESC_MAX_AIM;
	
    return output;
}
/*******************************拨弹**********************/
float Velo_Con_pull(float curr_velo_pull,float target_velo_pull)
{
	const float v_p =3.5;//5.5;//60//10//60.0;               //20.0   //使偏差增大或减小
	const float v_i=0.2; //0//0.1//0                 //0.0002
	const float v_d = 0;//2//0.5//2                     //0
    
	static float error_v[2] = {0.0,0.0};
	static float error_sum=0;
	static float output = 0;
 
	error_v[0] = error_v[1];		
  error_v[1] = -curr_velo_pull+target_velo_pull; 
	error_sum += error_v[1];
		
		if(error_sum>1000) error_sum = 1000;
		if(error_sum<-1000) error_sum = -1000;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
		   
     
    if(output > ESC_MAX_AIM)  output = ESC_MAX_AIM;
    if(output < -ESC_MAX_AIM) output =-ESC_MAX_AIM;
	
    return output;
}
float Pos_Con_pull(float curr_pos_pull,float target_pos_pull)
{
 //17，0.1，5完全不抖的。
    const float P_p =0.1;
  const float P_i = 0.005;//0.0001//0.02//0.03   //0      //0.0001
    const float P_d =0;//5//5//12 													 //0
    static float error_p[2] = {0.0,0.0};
    static float output = 0;
    static float error_p_sum= 0;
    
		error_p[0] =  error_p[1];
//		if(curr_pos_pull>206413)
//			error_p[1] =  -(curr_pos_pull-294876)+target_pos_pull;
    error_p[1] =  -curr_pos_pull+target_pos_pull;
//		if(error_p[1]<30)
    error_p_sum += error_p[1]; 
    if(error_p_sum>1000) error_p_sum = 1000;//500000;
		if(error_p_sum<-1000) error_p_sum = -1000;
    output =  error_p[1]  * P_p 
           +  error_p_sum * P_i 
           + (error_p[1] - error_p[0]) * P_d;

    if(output > 8000)  
			output =  8000;
    if(output < -8000) 
			output = -8000;
	
	return output;
}
float Velo_pull_42mm(float curr_pos_pull,float target_pos_pull)
{
	const float v_p =6.4;        
	const float v_i=0.2;            
	const float v_d = 3;                   
    
	static float error_v[2] = {0.0,0.0};
	static float error_sum=0;
	static float output = 0;
 
	error_v[0] = error_v[1];		
  error_v[1] = -curr_pos_pull+target_pos_pull; 
	error_sum += error_v[1];
		
		if(error_sum>5000) error_sum = 5000;
		if(error_sum<-5000) error_sum = -5000;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
		   
     
    if(output > 8000)  output = 8000;
    if(output < -8000) output =-8000;
	
    return output;
}
float Velo_pull_17mm(float curr_pos_pull,float target_pos_pull)
{
	const float v_p =9;        
	const float v_i=1;            
	const float v_d = 0;                   
   
	static float error_v[2] = {0.0,0.0};
	static float error_sum=0;
	static float output = 0;
 
	error_v[0] = error_v[1];		
  error_v[1] = -curr_pos_pull+target_pos_pull; 
	error_sum += error_v[1];
		
		if(error_sum>500) error_sum = 500;
		if(error_sum<-500) error_sum = -500;
    output =   error_v[1] * v_p
		         + error_sum*v_i
             + (error_v[1] - error_v[0])*v_d;//PID公式
		   
     
    if(output > 12000)  output = 12000;
    if(output < -12000) output =-12000;
	
    return output;
}



