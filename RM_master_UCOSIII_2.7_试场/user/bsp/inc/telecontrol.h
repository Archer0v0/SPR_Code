#ifndef __TELECONTROL_H
#define __TELECONTROL_H

#include <stm32f4xx.h>

void USART6_Configuration(void);
void DMA2_Stream2_IRQHandler(void);
void control(void);
void danzhou_mode(void);
void Spin_Mode(void);
void follow_mode(void);

#define FOLLOW_MODE right_flag==2
#define AUTO_MODE		0//left_flag==1

#endif
