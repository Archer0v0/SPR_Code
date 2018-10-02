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
*           ���濪��       ����BUG            *
*           �޵�Ӣ��			ARCHR��ʤ           *
\*********************************************/

#include "bsp_init.h"
#include "main.h"

int main(void)

{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	bsp_init();
//	Cmd_Rest();
	
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
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
		if((pos_x<-5||pos_x>5)&&(pos_y<-5||pos_y>5)&&(pos_z<-10||pos_z>10)){//��������
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
//				target_pitch_angle-=pid_pitch;//����Ŀ��ֵ����ʵ��ǹ�ܵĹ̶�ƫ��
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
	unsigned char i;          //��������
	unsigned char Send_Count; //������Ҫ���͵����ݸ��� 
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{	        
	   { 		    		    				 
			DataScope_Get_Channel_Data( target_v1 , 1 ); //������ 1.0  д��ͨ�� 1
			DataScope_Get_Channel_Data( target_v2 , 2 ); //������ 2.0  д��ͨ�� 2
			DataScope_Get_Channel_Data( target_v3 , 3 ); //������ 3.0  д��ͨ�� 3
			DataScope_Get_Channel_Data( target_v4 , 4 ); //������ 4.0  д��ͨ�� 4
			DataScope_Get_Channel_Data( 5.0 , 5 ); //������ 5.0  д��ͨ�� 5
			DataScope_Get_Channel_Data( 6.0 , 6 ); //������ 6.0  д��ͨ�� 6
			DataScope_Get_Channel_Data( 7.0 , 7 ); //������ 7.0  д��ͨ�� 7
			DataScope_Get_Channel_Data( 8.0 , 8 ); //������ 8.0  д��ͨ�� 8
			DataScope_Get_Channel_Data( 9.0 , 9 ); //������ 9.0  д��ͨ�� 9
			DataScope_Get_Channel_Data( 10.0 , 10); //������ 10.0 д��ͨ�� 10
			 
	  	Send_Count = DataScope_Data_Generate(10); //����10��ͨ���� ��ʽ��֡���ݣ�����֡���ݳ���
		
		  for( i = 0 ; i < Send_Count; i++)  //ѭ������,ֱ���������   
	 	  {
		    while((USART2->SR&0X40)==0)
					;
  	    USART2->DR = DataScope_OutPut_Buffer[i]; //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ      
		  } 
  
     delay_ms(30); //20fps, ֡���ʱ�䡣 ��ͬ�������ü� USB-TTL �豸�����Ӿ���Ӱ���ʱ��ĳ��̣�����ʵ��Ϊ׼��  
    }
		OSTimeDlyHMSM(0,0,0,3,OS_OPT_TIME_PERIODIC,&err); //��ʱ500ms
	}
}


