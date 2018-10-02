/*********************************************\
*                   _ooOoo_                   *
*                  o8888888o                  *
*                  88" . "88                  *
*                  (| -_- |)                  *
*                  O\  =  /O                  *
*               ____/`---'\____               *
*             .'  \\|     |//  `.             *
*            /  \\|||  :  |||//  \            *
*           /  _||||| -:- |||||-  \           *
*           |   | \\\  -  /// |   |           *
*           | \_|  ''\---/''  |   |           *
*           \  .-\__  `-`  ___/-. /           *
*         ___`. .'  /--.--\  `. . __          *
*      ."" '<  `.___\_<|>_/___.'  >'"".       *
*     | | :  `- \`.;`\ _ /`;.`/ - ` : | |     *
*     \  \ `-.   \_ __\ /__ _/   .-` /  /     *
*======`-.____`-.___\_____/___.-`____.-'======*
*                   `=---='                   *
*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*
*           佛祖开光       永无BUG            *
*           无敌英雄			ARCHR必胜           *
\*********************************************/

#include "bsp_init.h"
#include "main.h"

int main(void)

{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	bsp_init();
//	Cmd_Rest();
	
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}


int i=0;
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{ 
		if(!Pull_scan){i++;}
		else i=0;
		if(i>29){n=5;i=0;}
		LED0=0;
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&err);
		LED0=1;
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&err);
	}
}

void MOVE_task(void *p_arg)
{ 
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		Spin_mode();
		follow_mode();
		common_mode();
		move();	
		Get_mode();		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err); 
	}
}
float posx=0;
float posy=0;
void READ_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		control();
		if((pos_x<-5||pos_x>5)&&(pos_y<-5||pos_y>5)&&(pos_z<-10||pos_z>10)){//数据正常
			posx=(float)(atan((pos_x+50)/(pos_z-140))*180/3.141592654);
			posy=-(float)(atan((pos_y-100)/(pos_z-140))*180/3.141592654);
		}
		else {posx=0;posy=0;}
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		MPU6050_17mm_ReadData();
		OSTimeDlyHMSM(0,0,0,7,OS_OPT_TIME_PERIODIC,&err);
	}
}

void AIM_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		aim();
		Cmd_205_208(velo_205_output,velo_206_output,velo_207_output,velo_208_output);
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
	}
}
void SHOOT_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		shoot_mode();
		pull_17mm();
		friction_17mm();
		friction_42mm();
		OSTimeDlyHMSM(0,0,0,7,OS_OPT_TIME_PERIODIC,&err); 
	}
}

void CAN_SEND_task(void *p_arg){
	OS_ERR err;
	p_arg = p_arg;
	while(1){	
		velo_208_output=Velo_Con_pull(this_velo_208,-pull_v);
		velo_204_output=Velo_pull_42mm(this_velo_204,-pull_42mm_v);
		velo_203_output=Velo_pull_17mm(this_velo_203,-pull_17mm_v);
		Cmd_201_204(target_v5,target_v6,velo_203_output,velo_204_output);
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
	}
}

void DEC_task(void *p_arg){
	OS_ERR err;
	p_arg = p_arg;
	while(1){	
		pull_42mm();
		shooting_42mm();	
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
	}
}


float pid_pitch=0,pid_yaw=0,pos_17mm=0;
char x_flag=0;
void AUTO_AIM_task(void *p_arg){
	OS_ERR err;
	p_arg = p_arg;
	while(1){
		if((keyboard&0x0200)&&get_mode_choice==get_over_flag){
			if(follow_flag) follow_flag=~follow_flag;
			if(posy<-1||posy>1)	target_pitch_angle=this_205_angle+posy;
			if(posx<-1||posx>1) target_yaw_angle_42mm=this_206_angle+posx;
			x_flag=1;
//				pid_pitch = SXT_pitch(posy,0); 
//				target_pitch_angle-=pid_pitch;//调整目标值可以实现枪管的固定偏移
//				pid_yaw=SXT_yaw(posx,0);
//				follow_yaw-=pid_yaw;
			
//			if(right_flag==1){
//			pos_17mm=90-asin(pos_z*sin(90-encoder_205* 0.044))/sqrt(abs(pos_z*pos_z+h*h-2*pos_z*h*cos(90-encoder_205* 0.044)));
//			target_pitch_angle=PITCH_17MM_MID-pos_17mm;
//			}
//				if((this_206_angle-YAW_MID)>-5&&(this_206_angle-YAW_MID)<5)
//					shoot_17mm.shoot_flag=1;
		}
		else if(!(keyboard&0x0200)&&x_flag){follow_flag=~follow_flag;x_flag=0;target_yaw_angle_42mm=YAW_MID;}
		else follow_flag=0;
//		if(right_flag==3) target_yaw_angle_42mm=YAW_MID;
	
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);
	}
}

void DEBUG_task(void *p_arg)
{
	unsigned char i;          //计数变量
	unsigned char Send_Count; //串口需要发送的数据个数 
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{	        
	   { 		    		    				 
			DataScope_Get_Channel_Data( target_v1 , 1 ); //将数据 1.0  写入通道 1
			DataScope_Get_Channel_Data( target_v2 , 2 ); //将数据 2.0  写入通道 2
			DataScope_Get_Channel_Data( target_v3 , 3 ); //将数据 3.0  写入通道 3
			DataScope_Get_Channel_Data( target_v4 , 4 ); //将数据 4.0  写入通道 4
			DataScope_Get_Channel_Data( 5.0 , 5 ); //将数据 5.0  写入通道 5
			DataScope_Get_Channel_Data( 6.0 , 6 ); //将数据 6.0  写入通道 6
			DataScope_Get_Channel_Data( 7.0 , 7 ); //将数据 7.0  写入通道 7
			DataScope_Get_Channel_Data( 8.0 , 8 ); //将数据 8.0  写入通道 8
			DataScope_Get_Channel_Data( 9.0 , 9 ); //将数据 9.0  写入通道 9
			DataScope_Get_Channel_Data( 10.0 , 10); //将数据 10.0 写入通道 10
			 
	  	Send_Count = DataScope_Data_Generate(10); //生成10个通道的 格式化帧数据，返回帧数据长度
		
		  for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕   
	 	  {
		    while((USART2->SR&0X40)==0)
					;
  	    USART2->DR = DataScope_OutPut_Buffer[i]; //从串口丢一个字节数据出去      
		  } 
  
     delay_ms(30); //20fps, 帧间隔时间。 不同电脑配置及 USB-TTL 设备的优劣均会影响此时间的长短，建议实测为准。  
    }
		OSTimeDlyHMSM(0,0,0,3,OS_OPT_TIME_PERIODIC,&err); //延时500ms
	}
}


