#ifndef _MOVE_H
#define _MOVE_H
#include <stm32f4xx.h>
#include "bsp_init.h"
void move(void);
void aim(void);

#define PITCH_17MM_MID 218
#define PITCH_17MM_EXTENT 20.0f
#define PITCH_17MM_MAX PITCH_17MM_MID+PITCH_EXTENT
#define PITCH_17MM_MIN PITCH_17MM_MID-PITCH_EXTENT

#define PITCH_42MM_MID 55
#define PITCH_EXTENT 40.0f
#define PITCH_MAX PITCH_42MM_MID+PITCH_EXTENT
#define PITCH_MIN PITCH_42MM_MID-PITCH_EXTENT

#define YAW_MID 55
#define YAW_EXTENT 40.0f
#define YAW_MAX YAW_MID+YAW_EXTENT
#define YAW_MIN YAW_MID-YAW_EXTENT
const enum get_mode{
	get_ready_flag=1,
	get_flag,
	stop_get,
	get_over_flag
};
extern int get_mode_choice;

const enum spin_mode{
	stop_spin=1,
	back_spin=2,
	defense_spin=3,
};
extern int spin_mode_choice;

void common_mode(void);
void Get_mode(void);
void Spin_mode(void);
void v_limit(int max);

#endif


