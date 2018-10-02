#include "bsp_init.h"

int16_t current_203 = 0;
int16_t current_204 = 0;
int16_t current_205 = 0;
int16_t current_206 = 0;
int16_t current_207 = 0;
int16_t current_208 = 0;

int16_t this_velo_203 = 0; 
int16_t this_velo_204 = 0; 
int16_t this_velo_205 = 0; 			
int16_t this_velo_206 = 0; 
int16_t this_velo_207 = 0; 
int16_t this_velo_208 = 0; 

int16_t encoder_203 = 0;
int16_t encoder_204 = 0;
int16_t encoder_205 = 0;		 
int16_t encoder_206 = 0;
int16_t encoder_207 = 0;
int16_t encoder_208_= 0;
int16_t encoder_208 = 0;

int16_t phaseZ_1 ; 
int16_t phaseZ_2 ; 
int16_t phaseZ_3 ; 
int16_t phaseZ_4 ; 
int16_t phaseZ_5 ; 
int16_t phaseZ_6 ; 

double flag_change=0;
double flag_add=0;
double change_check[2]={0,0};

void CAN1_init(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
 
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOD, &gpio);
    
    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic); 
	
 		nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 0;
		nvic.NVIC_IRQChannelSubPriority =3;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &can);

		can_filter.CAN_FilterNumber=0;
		can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
		can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
		can_filter.CAN_FilterIdHigh=0x0000;
		can_filter.CAN_FilterIdLow=0x0000;
		can_filter.CAN_FilterMaskIdHigh=0x0000;
		can_filter.CAN_FilterMaskIdLow=0x0000;
		can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
		can_filter.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&can_filter);
			
		CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}
void CAN2_init(void)
{
   CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef 	   gpio;
    NVIC_InitTypeDef   	   nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &gpio);

    nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	
		nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
    CAN_DeInit(CAN2);
    CAN_StructInit(&can);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN2, &can);

    can_filter.CAN_FilterNumber = 14;
    can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh = 0x0000;
    can_filter.CAN_FilterIdLow = 0x0000;
    can_filter.CAN_FilterMaskIdHigh = 0x0000;
    can_filter.CAN_FilterMaskIdLow = 0x0000;
    can_filter.CAN_FilterFIFOAssignment = 0;
    can_filter.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&can_filter);

    CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
		CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
}
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
OSIntEnter();    
#endif
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
			switch (rx_message.StdId)
        {
					case DRV01_RETURN_ID://摩擦轮
						phaseZ_5 = (rx_message.Data[2]<<8)|rx_message.Data[3];break;
					case DRV02_RETURN_ID://摩擦轮
						phaseZ_6 = (rx_message.Data[2]<<8)|rx_message.Data[3];break;
					case DRV03_RETURN_ID://17mm 拨弹
					{
						encoder_203 = (rx_message.Data[0]<<8)|rx_message.Data[1];
						this_velo_203 = (rx_message.Data[2]<<8)|rx_message.Data[3];
						current_203 = (rx_message.Data[4]<<8)|rx_message.Data[5];
					}break;	
					case DRV04_RETURN_ID://42mm 拨弹
					{
						encoder_204 = (rx_message.Data[0]<<8)|rx_message.Data[1];
						this_velo_204 = (rx_message.Data[2]<<8)|rx_message.Data[3];
						current_204 = (rx_message.Data[4]<<8)|rx_message.Data[5];
					}break;	
					case DRV05_RETURN_ID://42mm 云台
					{
						encoder_205 = (rx_message.Data[0]<<8)|rx_message.Data[1];
						this_velo_205 = (rx_message.Data[2]<<8)|rx_message.Data[3];
						current_205 = (rx_message.Data[4]<<8)|rx_message.Data[5];
					}break;
					case DRV06_RETURN_ID:
					{
						encoder_206 = (rx_message.Data[0]<<8)|rx_message.Data[1];
						this_velo_206 = (rx_message.Data[2]<<8)|rx_message.Data[3];
						current_206 = (rx_message.Data[4]<<8)|rx_message.Data[5];
					}break;			
					case DRV07_RETURN_ID://17mm 云台
					{
						encoder_207 = (rx_message.Data[0]<<8)|rx_message.Data[1];
						this_velo_207 = (rx_message.Data[2]<<8)|rx_message.Data[3];
						current_207 = (rx_message.Data[4]<<8)|rx_message.Data[5];
					}break;							
					case DRV08_RETURN_ID://17mm 拨弹
					{
						encoder_208_ = (rx_message.Data[0]<<8)|rx_message.Data[1];
						
						encoder_208 =flag_add + encoder_208_;
						if(flag_change<-7500){flag_add+=8191;}
						if(flag_change>7500){flag_add-=8191;}
						change_check[0]=change_check[1];
						change_check[1]=encoder_208_;
						flag_change=change_check[1]-change_check[0];
						
						if((encoder_208>=294876))
						{encoder_208-=294876;
						flag_add=0;
						change_check[1]=0;}
						
						if((encoder_208<=0))
						{encoder_208+=294876;
						flag_add=286685;
						change_check[1]=8191;}
						
						this_velo_208 = (rx_message.Data[2]<<8)|rx_message.Data[3];
						current_208 = (rx_message.Data[4]<<8)|rx_message.Data[5];
					}break;	
				}
		}
#if SYSTEM_SUPPORT_OS  
OSIntExit();    	//退出中断
#endif
}
void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
OSIntEnter();    
#endif
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
			  CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
        switch (rx_message.StdId)
        {
					case DRV01_RETURN_ID://1号轮
						phaseZ_1 = (rx_message.Data[2]<<8)|rx_message.Data[3];
					break;
					case DRV02_RETURN_ID://2号轮
						phaseZ_2 = (rx_message.Data[2]<<8)|rx_message.Data[3];
					break;
					case DRV03_RETURN_ID://3号轮
						phaseZ_3 = (rx_message.Data[2]<<8)|rx_message.Data[3];break;
					case DRV04_RETURN_ID://4号轮
						phaseZ_4 = (rx_message.Data[2]<<8)|rx_message.Data[3];break;
				}
		}
#if SYSTEM_SUPPORT_OS  
OSIntExit();    	//退出中断 
#endif
}

void CAN2_TX_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
OSIntEnter();    
#endif
    if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET)
    {
        CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
    }
#if SYSTEM_SUPPORT_OS  
OSIntExit();    	//退出中断
#endif
}
unsigned char can_tx_success_flag = 0;
void CAN1_TX_IRQHandler(void) //CAN TX
{
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
OSIntEnter();    
#endif
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET)
	{
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
		can_tx_success_flag=1;
    }
#if SYSTEM_SUPPORT_OS  
OSIntExit();    	//退出中断
#endif
}
//云台
void Cmd_205_208(int16_t _current_205,int16_t _current_206,int16_t _current_207,int16_t _current_208)
{
	CanTxMsg tx_message;

	tx_message.StdId = 0x1ff;//0x1ff云台，0x200底盘电机
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
 
	tx_message.Data[0] = (unsigned char)(_current_205 >> 8);
	tx_message.Data[1] = (unsigned char)_current_205;
	tx_message.Data[2] = (unsigned char)(_current_206 >> 8);
	tx_message.Data[3] = (unsigned char)_current_206;
	tx_message.Data[4] = (unsigned char)(_current_207 >> 8);;
	tx_message.Data[5] = (unsigned char)_current_207;
	tx_message.Data[6] = (unsigned char)(_current_208 >> 8);
	tx_message.Data[7] = (unsigned char)_current_208;
	
	CAN_Transmit(CAN1,&tx_message);
}
void Cmd_201_204(int16_t _current_201,int16_t _current_202,int16_t _current_203,int16_t _current_204)
{
	CanTxMsg tx_message;

	tx_message.StdId = 0x200;//0x1ff云台，0x200底盘电机
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
 
	tx_message.Data[0] = (unsigned char)(_current_201 >> 8);
	tx_message.Data[1] = (unsigned char)_current_201;
	tx_message.Data[2] = (unsigned char)(_current_202 >> 8);
	tx_message.Data[3] = (unsigned char)_current_202;
	tx_message.Data[4] = (unsigned char)(_current_203 >> 8);;
	tx_message.Data[5] = (unsigned char)_current_203;
	tx_message.Data[6] = (unsigned char)(_current_204 >> 8);
	tx_message.Data[7] = (unsigned char)_current_204;
	
	CAN_Transmit(CAN1,&tx_message);
}
void Cmd_Rest()
{
	CanTxMsg tx_message;

	tx_message.StdId = 0xaf0;
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
 
	tx_message.Data[0] = 'c';
	tx_message.Data[1] = 0x00;
	tx_message.Data[2] = 0x00;
	tx_message.Data[3] = 0x00;
	tx_message.Data[4] = 0x00;
	tx_message.Data[5] = 0x00;
	tx_message.Data[6] = 0x00;
	tx_message.Data[7] = 0x00;
	
	CAN_Transmit(CAN1,&tx_message);
}
//底盘
void CAN2_SetMsg(int16_t _current_1,int16_t _current_2,
	int16_t _current_3,int16_t _current_4)//电流值范围 -32768-32767,四个电机一起发送电流
{
	CanTxMsg TxMsg;
	TxMsg.IDE = CAN_ID_STD;
	TxMsg.RTR = CAN_RTR_DATA;
	TxMsg.StdId = 0x200;
	TxMsg.DLC = 0x08;
	
	TxMsg.Data[0] =(unsigned char)(_current_1 >> 8); 
	TxMsg.Data[1]=(unsigned char)_current_1;      
	TxMsg.Data[2] = (unsigned char)(_current_2 >> 8);
	TxMsg.Data[3] =(unsigned char)_current_2;
	TxMsg.Data[4] = (unsigned char)(_current_3 >> 8);
	TxMsg.Data[5] =(unsigned char)_current_3;
	TxMsg.Data[6] = (unsigned char)(_current_4 >> 8);
	TxMsg.Data[7] =(unsigned char)_current_4;
	
	CAN_Transmit(CAN2, &TxMsg);
}




