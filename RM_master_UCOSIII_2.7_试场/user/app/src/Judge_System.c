#include "Judge_System.h"


u8 Judge_Data[100]={0};

extPowerHeatData_t extPowerHeatdata_t;
extShootData_t extShootdata_t;
extGameRobotState_t extGameRobotstate_t;

void Judge_System(void)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	
	gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;	
	gpio.GPIO_OType = GPIO_OType_PP; 
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&gpio);
	
	usart.USART_BaudRate = 115200;//波特率设置
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART2, &usart); 
 
	
	  USART_ClearFlag(USART2, USART_IT_RXNE);
  	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//  USART_ClearFlag(USART2, USART_FLAG_TC);
	nvic.NVIC_IRQChannel = USART2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=0;
	nvic.NVIC_IRQChannelSubPriority =1;		
	nvic.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&nvic);	
	  
	USART_Cmd(USART2, ENABLE); 
}



u8 Res;
void USART2_IRQHandler(void)
{ 
	 
	 if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)  //发送中断
	 {
		 //USART_ClearFlag(USART2, USART_FLAG_TC);
			USART_ClearITPendingBit(USART2,USART_IT_TC);
		//　 USART_ClearFlag(USART2, USART_IT_TC);
		
	 }
	 if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	 {
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
			Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
			DATA_0001(Res); 
			DATA_0002(Res);
			DATA_0003(Res);
		  DATA_0004(Res);
		  Get_Data();
	 }  
}

void Get_Data(void)
{
		extGameRobotstate_t.maxHP=Judge_Data[6]|(Judge_Data[7]<<8);//最大血量
		extGameRobotstate_t.robotLevel=Judge_Data[3];
    extShootdata_t.bulletType=Judge_Data[13];//弹丸类型
    extShootdata_t.bulletFreq=Judge_Data[14];//射频
    *((int *)&extShootdata_t.bulletSpeed)=Judge_Data[15]|(Judge_Data[16]<<8)|(Judge_Data[17]<<16)|(Judge_Data[18]<<24);//射速
    *((int *)&extPowerHeatdata_t.chassisVolt)=Judge_Data[21]|(Judge_Data[22]<<8)|(Judge_Data[23]<<16)|(Judge_Data[24]<<24);//电压
	  *((int *)&extPowerHeatdata_t.chassisCurrent)=Judge_Data[25]|(Judge_Data[26]<<8)|(Judge_Data[27]<<16)|(Judge_Data[28]<<24);//电流
	  *((int *)&extPowerHeatdata_t.chassisPower)=Judge_Data[29]|(Judge_Data[30]<<8)|(Judge_Data[31]<<16)|(Judge_Data[32]<<24);//功率
	  *((int *)&extPowerHeatdata_t.chassisPowerBuffer)=Judge_Data[33]|(Judge_Data[34]<<8)|(Judge_Data[35]<<16)|(Judge_Data[36]<<24);//剩余能量
	  extPowerHeatdata_t.shooterHeat0=Judge_Data[37]|(Judge_Data[38]<<8);//17mm弹丸热量
	  extPowerHeatdata_t.shooterHeat1=Judge_Data[39]|(Judge_Data[40]<<8);//42mm弹丸热量
	if(extPowerHeatdata_t.chassisPowerBuffer>60.0f)
		extPowerHeatdata_t.chassisPowerBuffer=60.0f;
}


void DATA_0001(u8 Res)//比赛机器人状态
{
	
	static u8 count=0;
	static u8 i=0;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x08)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==2)
						{
						 count++;
						 i=0;
						}
             break;
         case 4:
            if(Res==0x01)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==10)
						{
							i=0;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}


void DATA_0002(u8 Res)
{
	
	static u8 count=0;
	static u8 i=10;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x01)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==12)
						{
						 count++;
						 i=10;
						}
             break;
         case 4:
            if(Res==0x02)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==13)
						{
							i=10;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}




void DATA_0003(u8 Res)
{
	
	static u8 count=0;
	static u8 i=13;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x06)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==15)
						{
						 count++;
						 i=13;
						}
             break;
         case 4:
            if(Res==0x03)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==21)
						{
							i=13;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}


void DATA_0004(u8 Res)
{
	
	static u8 count=0;
	static u8 i=21;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x14)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==23)
						{
						 count++;
						 i=21;
						}
             break;
         case 4:
            if(Res==0x04)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==43)
						{
							i=21;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}

void DATA_0005(u8 Res)
{
	
	static u8 count=0;
	static u8 i=43;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x02)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==45)
						{
						 count++;
						 i=43;
						}
             break;
         case 4:
            if(Res==0x05)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==47)
						{
							i=43;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}

void DATA_0006(u8 Res)
{
	
	static u8 count=0;
	static u8 i=47;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x02)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==49)
						{
						 count++;
						 i=47;
						}
             break;
         case 4:
            if(Res==0x05)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==51)
						{
							i=47;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}

void DATA_0007(u8 Res)
{
	
	static u8 count=0;
	static u8 i=51;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x03)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==53)
						{
						 count++;
						 i=51;
						}
             break;
         case 4:
            if(Res==0x05)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==56)
						{
							i=51;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}

void DATA_0008(u8 Res)
{
	
	static u8 count=0;
	static u8 i=56;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x10)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==58)
						{
						 count++;
						 i=56;
						}
             break;
         case 4:
            if(Res==0x05)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==74)
						{
							i=56;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}

void DATA_0100(u8 Res)
{
	
	static u8 count=0;
	static u8 i=74;
	switch(count)
				{
					case 0:
						if(Res==0xa5)
							count++;
						else
							count=0;
						break;
					case 1:
						if(Res==0x0D)
							count++;
						else
							count=0;
						break;
          case 2:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
          case 3:
						i++;
						if(i==76)
						{
						 count++;
						 i=74;
						}
             break;
         case 4:
            if(Res==0x05)
							count++;
						else
							count=0;
						break;
				 case 5:
            if(Res==0x00)
							count++;
						else
							count=0;
						break;
					case 6:
            Judge_Data[i]=Res;
					  i++;
					  if(i==89)
						{
							i=74;
							count=0;
						}
						break;
        default:
            count=0;
            break;
        }
}