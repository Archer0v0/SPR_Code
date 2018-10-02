#include "bsp_init.h"

#define DIV_ROCKER		       6.0   //ҡ������
#define High_SPEED     	     130	 //��shift�������٣�*
#define Slow_SPEED           30	   //����(���Ҽ�)*
#define Normal_SPEED         60		 //����*
#define Accel_SPEED	   	     1.5

#define pingyispeed          80

#define pingyicacel          2.5     	//���ٶ� 2
#define Accel_shift_SPEED	   1.5	 		//���ٶ� 50
#define Decel_SPEED	         2.5	  	//���ٶ� 7
#define MOUSE_SENSE		       1.7	  //���������*
#define MID_VALUE 	  	     1024  

#define Shift_W  0x0011
#define Shift_A  0x0014
#define Shift_S  0x0012
#define Shift_D  0x0018
//#define Ctrl_W  0x0021
//#define Ctrl_S  0x0022
//#define Ctrl_A  0x0024
//#define Ctrl_D  0x0028

#define Keyboard_W  0x0001
#define Keyboard_S  0x0002
#define Keyboard_A  0x0004
#define Keyboard_D  0x0008
#define Keyboard_Shift  0x0010
#define Keyboard_Ctrl 0x0020
#define Keyboard_Q  0x0040
#define Keyboard_E  0x0080
#define Keyboard_R  0x0100
#define Keyboard_F  0x0200
#define Keyboard_G  0x0400
#define Keyboard_Z  0x0800
#define Keyboard_X  0x1000
#define Keyboard_C  0x2000
#define Keyboard_V  0x4000
#define Keyboard_B  0x8000
#define Keyboard_W_D  0x0009
#define Keyboard_W_A  0x0005
#define Keyboard_S_A  0x0006
#define Keyboard_S_D  0x000A
float record;


unsigned char sbus_rx_buffer[18];
uint16_t  sbus_channel_temp[15] = {0};  //  temp sbus decode channel data

int v1=0,v2=0,v3=0,v4=0; 
float	pitchy=0;                //�ֱ���Y
float yawx=0;                  //�ֱ���X
float mousex=0;           //����ٶ�
float mouse_x=0;
float v_x=0,v_y=0;             //�����ٶ�
float vx=0,vy=0,wz=0;          //ң���ٶ�

float mousey=0;
char return_flag;
char shooting_flag;
char left_flag=0;              //ң�����ϼ�
char right_flag=0;             //ң�����ϼ�
char zero_flag =0;				     //�����Ƿ���(���ر�־λ)
uint16_t keyboard=0x0000;               //����
double target_pitch_angle=190;

struct last_flag{
	int16_t Key_Q;
	int16_t Key_E;
	int16_t Key_V;
	int16_t Key_F;
	int16_t Key_G;
	int16_t Key_X;
	int16_t Key_R;
	int16_t Key_Ctrl;
	char Key_Ml;
}Last_flag;

int shoot_mode_choice=0;
int get_mode_choice=4;
int spin_mode_choice=stop_spin;

void USART6_Configuration(void)
{
		USART_InitTypeDef usart6;
		GPIO_InitTypeDef  gpio;
		NVIC_InitTypeDef  nvic;
		DMA_InitTypeDef   dma;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA2,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
		
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource7 ,GPIO_AF_USART6);
		
		gpio.GPIO_Pin = GPIO_Pin_7 ;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOC,&gpio);
    
		USART_DeInit(USART6);
		usart6.USART_BaudRate = 100000;                //SBUS 100K baudrate
		usart6.USART_WordLength = USART_WordLength_8b;
		usart6.USART_StopBits = USART_StopBits_1;
		usart6.USART_Parity = USART_Parity_Even;
		usart6.USART_Mode = USART_Mode_Rx;
		usart6.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART6,&usart6);
     
		USART_Cmd(USART6,ENABLE);
		USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);
    
		nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 0;//
		nvic.NVIC_IRQChannelSubPriority = 1;//
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
    
		DMA_DeInit(DMA2_Stream2);
		dma.DMA_Channel= DMA_Channel_5;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART6->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = 18;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream2,&dma);

		DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA2_Stream2,ENABLE);
}


void DMA2_Stream2_IRQHandler(void)
{
		u8 Res;
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif
	if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
	{
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
		//ң����������ͨ�����룬ÿ��ͨ��ռ11��bit���ӵ͵��߽�ȡ����
		sbus_channel_temp[0] = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;//��x
		sbus_channel_temp[1] = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;//��y
		sbus_channel_temp[2] = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff; //��x
		sbus_channel_temp[3] = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;      //��y
		sbus_channel_temp[4] = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;  // ���ϼ�
		sbus_channel_temp[5] = ((sbus_rx_buffer[5] >> 4)& 0x0003);       // ���ϼ�
		
		sbus_channel_temp[6] = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8);  //���X��
		sbus_channel_temp[7] = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8);  //���Y��(��������ƶ�������̨������)
		sbus_channel_temp[8] = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8);//���Z��
	
		sbus_channel_temp[9] = sbus_rx_buffer[12];   //������
		sbus_channel_temp[10] = sbus_rx_buffer[13];  //����Ҽ�
		sbus_channel_temp[11] = sbus_rx_buffer[14]| (sbus_rx_buffer[15] << 8);//W,S,A,D,SHIFT,CTRL,Q,E���̰���
		sbus_channel_temp[12] = sbus_rx_buffer[16] | (sbus_rx_buffer[17] << 8);//NULL
		
		wr=WWDG->CFR&0X7F;       
    tr=WWDG->CR&0X7F;       // ���Ź�ι����ʱ
	
		if((tr<wr)&&(sbus_channel_temp[0]>24)&&(sbus_channel_temp[0]<2124)&& 
			(sbus_channel_temp[1]>24)&&(sbus_channel_temp[1]<2124)&&
			(sbus_channel_temp[2]>24)&&(sbus_channel_temp[2]<2124)&&
			(sbus_channel_temp[3]>24)&&(sbus_channel_temp[3]<2124))   // ң��������������ʹ��ҾͲ�ι��
		{
		WWDG_SetCounter(0X7F);
		}
	}
#if SYSTEM_SUPPORT_OS  
OSIntExit();    	//�˳��ж�
#endif
}
int flag_follow=1;
void Get_yaw(){
	if(flag_follow){
		follow_yaw=yaw;
		flag_follow=0;}
	else;
}
void control(void)
{ 
		right_flag=sbus_channel_temp[5];				//ң�����ϼ�(1,3,2)
		left_flag=sbus_channel_temp[4];		      //ң�����ϼ�(1,3,2)
		shoot_17mm.shoot_flag=shooting_flag=sbus_channel_temp[9];	  	//��������־λ ����
    return_flag=sbus_channel_temp[10];	    //����Ҽ�	
		keyboard=sbus_channel_temp[11];         //����
	
		/*W     0001  S    0002    A 0004    D 0008*/
		/*shift 0010  ctrl 0020    Q 0040    E 0080*/
		/*R     0100  F    0200    G 0400    Z 0800*/
		/*X     1000  C    2000    V 4000    B 8000*/
    //��ϼ�����W+sift=0001+0010=0011
		mouse_x= sbus_channel_temp[6];
		mousex = sbus_channel_temp[6];
		mousey = sbus_channel_temp[7];
				
		if(keyboard==Keyboard_Q&&!Last_flag.Key_Q){	//ȡ��
			get_mode_choice+=3;
//			if(get_mode_choice>6) spin_mode_choice=back_spin;
			
			Last_flag.Key_Q=1;
			zero_flag=1;
		}
		else if(keyboard!=Keyboard_Q&&Last_flag.Key_Q){
			Last_flag.Key_Q=0;
		}
		
		if(keyboard==Keyboard_E&&!Last_flag.Key_E&&
			(get_mode_choice==get_ready_flag||get_mode_choice==stop_get)){	//ȡ��
			get_mode_choice=get_flag;
			
			Last_flag.Key_E=1;
			zero_flag=1;
		}
		else if(keyboard!=Keyboard_E&&Last_flag.Key_E){
			Last_flag.Key_E=0;
		}
		
//		if(keyboard==Keyboard_X&&!Last_flag.Key_X){	//��ת
//			spin_mode_choice+=2;
//			
//			Last_flag.Key_X=1;
//			zero_flag=1;
//		}
//		else if(keyboard!=Keyboard_X&&Last_flag.Key_X){
//			Last_flag.Key_X=0;
//		}		
		
		if(keyboard==Keyboard_V&&!Last_flag.Key_V){	//��ͷ
			spin_mode_choice=back_spin;
			
			Last_flag.Key_V=1;
			zero_flag=1;
		}
		else if(keyboard!=Keyboard_V&&Last_flag.Key_V){
			Last_flag.Key_V=0;
		}	
		
		if(shooting_flag&&!Last_flag.Key_Ml){	//���
			shoot_42mm.shoot_flag=1;
			shoot_17mm.shoot_flag=1;
			Last_flag.Key_Ml=1;
			zero_flag=1;
		}
		else if(!shooting_flag&&Last_flag.Key_Ml){
			shoot_42mm.shoot_flag=0;
			Last_flag.Key_Ml=0;
		}
		
//		if(keyboard==Keyboard_F&&!Last_flag.Key_F){	//ģʽѡ��
//			follow_flag=~follow_flag;
//			
//			Last_flag.Key_F=1;
//			zero_flag=1;
//		}
//		else if(keyboard!=Keyboard_F&&Last_flag.Key_F){
//			Last_flag.Key_F=0;
//		}
		
		if(keyboard==Keyboard_R&&!Last_flag.Key_R){	//�ֶ�װ��
			n=-1;
			
			Last_flag.Key_R=1;
			zero_flag=1;
		}
		else if(keyboard!=Keyboard_R&&Last_flag.Key_R){
			Last_flag.Key_R=0;
		}
		
		if(keyboard==Keyboard_Ctrl&&!Last_flag.Key_Ctrl){	
			UP_2 OPEN;
			get_mode_choice=get_ready_flag;
			delay_ms(1500);
			UP_2 CLOSE;
			get_mode_choice=get_over_flag;
			Last_flag.Key_Ctrl=1;
			zero_flag=1;
		}
		else if(keyboard!=Keyboard_Ctrl&&Last_flag.Key_Ctrl){
			Last_flag.Key_Ctrl=0;
		}
{		//move
		if(zero_flag==0){
			if(v_x<0) v_x+=Decel_SPEED;
			if(v_x>0) v_x-=Decel_SPEED;
			if(v_y<0) v_y+=Decel_SPEED;
			if(v_y>0) v_y-=Decel_SPEED;
			if((v_x<10)&&(v_x>-10))  v_x=0;
			if((v_y<10)&&(v_y>-10))  v_y=0;
		}				
		
	zero_flag=0;	//�Ƿ��а�������
		
	if(keyboard==Keyboard_W)	//�����ٶȴ�С�����٣�
		{
			v_y+=pingyicacel;
			if(v_y>pingyispeed) v_y=pingyispeed;
			if(v_x<0) v_x+=5;
			if(v_x>0) v_x-=5;
		
			zero_flag=1;
		}		
	if(keyboard==Keyboard_S)	//�����ٶȴ�С������  �� 
		{
			v_y-=pingyispeed;
			if(-v_y>pingyispeed) v_y=-pingyispeed;
			if(v_x<0) v_x+=5;
			if(v_x>0) v_x-=5;
			
			zero_flag=1;
		}
//	if((get_mode_choice==get_ready_flag||get_mode_choice==stop_get)//ȡ��ǰ��
//		&&keyboard==Keyboard_W){
//		v_y-=pingyispeed;
//			if(-v_y>pingyispeed) v_y=-pingyispeed;
//			if(v_x<0) v_x+=5;
//			if(v_x>0) v_x-=5;
//			
//			zero_flag=1;
//		}
//	if((get_mode_choice==get_ready_flag||get_mode_choice==stop_get)//ȡ������
//		&&keyboard==Keyboard_S){
//		v_y+=pingyicacel;
//			if(v_y>pingyispeed) v_y=pingyispeed;
//			if(v_x<0) v_x+=5;
//			if(v_x>0) v_x-=5;
//		
//			zero_flag=1;
//		}					
	if(keyboard==Keyboard_D)	//�����ٶȴ�С�����٣� 
		{
			v_x-=Accel_SPEED;
			if(-v_x>Normal_SPEED) v_x=-Normal_SPEED;
			if(v_y<0) v_y+=5;
			if(v_y>0) v_y-=5;
		
			zero_flag=1;
		}
	if(keyboard==Keyboard_A)	//�����ٶȴ�С�����٣�  
		{
			v_x+=Accel_SPEED;
			if(v_x>Normal_SPEED) v_x=Normal_SPEED;
			if(v_y<0) v_y+=5;
			if(v_y>0) v_y-=5;
			zero_flag=1;
		}
	if(keyboard==Keyboard_W_A)	//����ǰ�˶�
		{			v_y+=Accel_SPEED;
			if(v_y>Normal_SPEED) v_y=Normal_SPEED;
			v_x+=Accel_SPEED;
			if(v_x>Normal_SPEED) v_x=Normal_SPEED;
			zero_flag=1;
		}
		
		
	if(keyboard==Keyboard_S_A)	//������˶�
		{			v_y-=Accel_SPEED;
			if(-v_y>Normal_SPEED) v_y=-Normal_SPEED;
			v_x+=Accel_SPEED;
			if(v_x>Normal_SPEED) v_x=Normal_SPEED;
			zero_flag=1;
		}
	if(keyboard==Keyboard_S_D)	//���Һ��˶�
		{			v_y-=Accel_SPEED;
			if(-v_y>Normal_SPEED) v_y=-Normal_SPEED;
			v_x-=Accel_SPEED;
			if(-v_x>Normal_SPEED) v_x=-Normal_SPEED;
			zero_flag=1;
		}
	if(keyboard==Keyboard_W_D)	//����ǰ�˶�
		{			v_y+=Accel_SPEED;
			if(v_y>Normal_SPEED) v_y=Normal_SPEED;
			v_x-=Accel_SPEED;
			if(-v_x>Normal_SPEED) v_x=-Normal_SPEED;
			zero_flag=1;
		 }
	if(keyboard==Shift_W)	//�����ٶȴ�С�����٣� 
		{
			v_y+=Accel_shift_SPEED;
			if(v_y>High_SPEED) v_y=High_SPEED;
			if(v_x<0) v_x+=5;
			if(v_x>0) v_x-=5;
			zero_flag=1;			
		}			
	if(keyboard==Shift_S)	//�����ٶȴ�С�����٣�
		{
			v_y-=Accel_shift_SPEED;
			if(-v_y>High_SPEED) v_y=-High_SPEED;
			if(v_x<0) v_x+=5;
			if(v_x>0) v_x-=5;
			zero_flag=1;
		}			
	if(keyboard==Shift_D)	//�����ٶȴ�С�����٣�
		{
			v_x-=Accel_shift_SPEED;
			if(-v_x>High_SPEED) v_x=-High_SPEED;
			if(v_y<0) v_y+=5;
			if(v_y>0) v_y-=5;	
			zero_flag=1;
		}		
	if(keyboard==Shift_A)	//�����ٶȴ�С�����٣�  
		{
			v_x+=Accel_shift_SPEED;
			if(v_x>High_SPEED) v_x=High_SPEED;
			if(v_y<0) v_y+=5;
			if(v_y>0) v_y-=5;	
			zero_flag=1;
		}
	if((keyboard==Keyboard_W)&&return_flag==1)	//�����ٶȴ�С�����٣� ���¾�Ϊ����Ҽ�+�����
		{
			v_y+=Accel_SPEED;
			if(v_y>Slow_SPEED) v_y=Slow_SPEED;
			if(v_x<0) v_x+=5;
			if(v_x>0) v_x-=5;
			zero_flag=1;
		}	
	if((keyboard==Keyboard_S)&&return_flag==1)	//�����ٶȴ�С�����٣�
		{
			v_y-=Accel_SPEED;
			if(-v_y>Slow_SPEED) v_y=-Slow_SPEED;
			if(v_x<0) v_x+=5;
			if(v_x>0) v_x-=5;
			zero_flag=1;
		}
	if((keyboard==Keyboard_D)&&return_flag==1)	//�����ٶȴ�С�����٣�
		{
			v_x-=Accel_SPEED;
			if(-v_x>Slow_SPEED) v_x=-Slow_SPEED;
			if(v_y<0) v_y+=5;
			if(v_y>0) v_y-=5;
			zero_flag=1;
		}		
	if((keyboard==Keyboard_A)&&return_flag==1)	//�����ٶȴ�С�����٣�
		{
			v_x+=Accel_SPEED;
			if(v_x>Slow_SPEED) v_x=Slow_SPEED;
			if(v_y<0) v_y+=5;
			if(v_y>0) v_y-=5;
			zero_flag=1;
		}
}					
		/**********************��̨�Ƕȿ���*****************************/
				//pitch ��					
				pitchy=0;
        if((sbus_channel_temp[1]>1050)&&(target_pitch_angle_17mm<PITCH_17MM_MAX)
					&&(target_pitch_angle_42mm<PITCH_MAX))
            pitchy=0.05;
        if((sbus_channel_temp[1]<1000)&&(target_pitch_angle_17mm<PITCH_17MM_MAX)
					&&(target_pitch_angle_42mm>PITCH_MIN))
            pitchy=-0.05;
				
				if(mousey>32768) mousey=mousey-65535;
				if(mousey>0.3&&(target_pitch_angle_17mm<PITCH_17MM_MAX)&&
					(target_pitch_angle_42mm<PITCH_MAX))   mousey*=0.01;
				if(mousey<-0.3&&(target_pitch_angle_17mm>PITCH_17MM_MIN)&&
					(target_pitch_angle_42mm>PITCH_MIN))  mousey*=0.01;
				
				target_pitch_angle -=-pitchy+mousey;
				
				
			if(follow_flag){
				yawx=0;
				mousex=0;
				if((sbus_channel_temp[0]<1000)/*&&((this_206_angle-YAW_MID)>-YAW_EXTENT)*/)yawx=0.5;
				if((sbus_channel_temp[0]>1050)/*&&((this_206_angle-YAW_MID)<YAW_EXTENT)*/)yawx=-0.5;
				if(mouse_x>32768) mouse_x=mouse_x-65535;
				if((mouse_x>0.3)/*&&((this_206_angle-YAW_MID)>-YAW_EXTENT)*/)   mouse_x*=0.01;
				if((mouse_x<-0.3)/*&&((this_206_angle-YAW_MID)<YAW_EXTENT)*/)  mouse_x*=0.01;
				if(!follow_yaw){follow_yaw=yaw;record=follow_yaw;}
				else follow_yaw -=-yawx+mouse_x;//yaw ��
				
				if(follow_yaw>180){follow_yaw-=360;record=follow_yaw;}
				else if(follow_yaw<-180){follow_yaw+=360;record=follow_yaw;}
			}else{
				if(mousex>32768) mousex=mousex-65535;
				if(mousex>0.3)   mousex*=0.01;
				if(mousex<-0.3)  mousex*=0.01;
				follow_yaw=0;
				wz =  (sbus_channel_temp[0]-MID_VALUE)/DIV_ROCKER;		     //��ת���ٶ�
			}
		vy = (sbus_channel_temp[3]-MID_VALUE)/DIV_ROCKER;        //Y������ٶ�
		vx = (sbus_channel_temp[2]-MID_VALUE)/DIV_ROCKER;		      //X������ٶ�
		
    if(vx>-8&&vx<8)vx=0;//��ֹ��
    if(vy>-8&&vy<8)vy=0;//��ֹ��
		if(wz>-8&&wz<8)wz=0;//��ֹ��
}

