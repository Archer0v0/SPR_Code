#ifndef _SHOOT_H
#define _SHOOT_H
#include <stm32f4xx.h>

#define  PWM_ADD  15
#define  current_42mm_max 1000
#define	 current_17mm_max	1000
enum shoot_mode{
	shoot_stop_flag=1,
	shoot_42mm_flag,
	shoot_17mm_flag
};
struct shoot{
	int  friction_flag;
	char shoot_flag;
};
extern int shoot_mode_choice;

void shoot_mode(void);
void shooting_17mm(void);
void shooting_42mm(void);
void  friction_17mm(void);
void  friction_42mm(void);
void pull_17mm(void);
void pull_42mm(void);
#endif
