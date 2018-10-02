#ifndef MIAN_H
#define MIAN_H
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//任务优先级
#define LED0_TASK_PRIO		4
//任务堆栈大小	
#define LED0_STK_SIZE 		128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//任务优先级
#define READ_TASK_PRIO		4
//任务堆栈大小	
#define READ_STK_SIZE 		512
//任务控制块
OS_TCB READTaskTCB;
//任务堆栈	
CPU_STK READ_TASK_STK[READ_STK_SIZE];
//任务函数
void READ_task(void *p_arg);

//任务优先级
#define CAN_SEND_TASK_PRIO		4
//任务堆栈大小	
#define CAN_SEND_STK_SIZE 		128
//任务控制块
OS_TCB CAN_SENDTaskTCB;
//任务堆栈	
CPU_STK CAN_SEND_TASK_STK[START_STK_SIZE];
//任务函数
void CAN_SEND_task(void *p_arg);

//任务优先级
#define AIM_TASK_PRIO		4
//任务堆栈大小	
#define AIM_STK_SIZE 		128
//任务控制块
OS_TCB AIMTaskTCB;
//任务堆栈	
CPU_STK AIM_TASK_STK[AIM_STK_SIZE];
//任务函数
void AIM_task(void *p_arg);

//任务优先级
#define MOVE_TASK_PRIO		4
//任务堆栈大小	
#define MOVE_STK_SIZE 		256
//任务控制块
OS_TCB MOVETaskTCB;
//任务堆栈	
CPU_STK MOVE_TASK_STK[MOVE_STK_SIZE];
//任务函数
void MOVE_task(void *p_arg);

//任务优先级
#define SHOOT_TASK_PRIO		4
//任务堆栈大小	
#define SHOOT_STK_SIZE 		256
//任务控制块
OS_TCB SHOOTTaskTCB;
//任务堆栈	
CPU_STK SHOOT_TASK_STK[SHOOT_STK_SIZE];
//任务函数
void SHOOT_task(void *p_arg);

//任务优先级
#define DEC_TASK_PRIO		4
//任务堆栈大小	
#define DEC_STK_SIZE 		256
//任务控制块
OS_TCB DECTaskTCB;
//任务堆栈	
CPU_STK DEC_TASK_STK[DEC_STK_SIZE];
//任务函数
void DEC_task(void *p_arg);

//任务优先级
#define AUTO_AIM_TASK_PRIO		4
//任务堆栈大小	
#define AUTO_AIM_STK_SIZE 		128
//任务控制块
OS_TCB AUTO_AIM_TaskTCB;
//任务堆栈	
CPU_STK AUTO_AIM_TASK_STK[DEC_STK_SIZE];
//任务函数
void AUTO_AIM_task(void *p_arg);

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区	
	
	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	
								 
	OSTaskCreate((OS_TCB 	* )&CAN_SENDTaskTCB,		
				 (CPU_CHAR	* )"CAN_SEND task", 		
                 (OS_TASK_PTR )CAN_SEND_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )CAN_SEND_TASK_PRIO,     
                 (CPU_STK   * )&CAN_SEND_TASK_STK[0],	
                 (CPU_STK_SIZE)CAN_SEND_STK_SIZE/10,	
                 (CPU_STK_SIZE)CAN_SEND_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	
								 
	OSTaskCreate((OS_TCB 	* )&AIMTaskTCB,		
				 (CPU_CHAR	* )"AIM task", 		
                 (OS_TASK_PTR )AIM_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )AIM_TASK_PRIO,     	
                 (CPU_STK   * )&AIM_TASK_STK[0],	
                 (CPU_STK_SIZE)AIM_STK_SIZE/10,	
                 (CPU_STK_SIZE)AIM_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);

	OSTaskCreate((OS_TCB 	* )&READTaskTCB,		
				 (CPU_CHAR	* )"READ task", 		
                 (OS_TASK_PTR )READ_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )READ_TASK_PRIO,     	
                 (CPU_STK   * )&READ_TASK_STK[0],	
                 (CPU_STK_SIZE)READ_STK_SIZE/10,	
                 (CPU_STK_SIZE)READ_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);

	OSTaskCreate((OS_TCB 	* )&SHOOTTaskTCB,		
				 (CPU_CHAR	* )"SHOOT task", 		
                 (OS_TASK_PTR )SHOOT_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SHOOT_TASK_PRIO,     	
                 (CPU_STK   * )&SHOOT_TASK_STK[0],	
                 (CPU_STK_SIZE)SHOOT_STK_SIZE/10,	
                 (CPU_STK_SIZE)SHOOT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);								 
	
	OSTaskCreate((OS_TCB 	* )&MOVETaskTCB,		
				 (CPU_CHAR	* )"MOVE task", 		
                 (OS_TASK_PTR )MOVE_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )MOVE_TASK_PRIO,     	
                 (CPU_STK   * )&MOVE_TASK_STK[0],	
                 (CPU_STK_SIZE)MOVE_STK_SIZE/10,	
                 (CPU_STK_SIZE)MOVE_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								
		OSTaskCreate((OS_TCB 	* )&DECTaskTCB,		
				 (CPU_CHAR	* )"DEC task", 		
                 (OS_TASK_PTR )DEC_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )DEC_TASK_PRIO,     	
                 (CPU_STK   * )&DEC_TASK_STK[0],	
                 (CPU_STK_SIZE)DEC_STK_SIZE/10,	
                 (CPU_STK_SIZE)DEC_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								 
OSTaskCreate((OS_TCB 	* )&AUTO_AIM_TaskTCB,		
				 (CPU_CHAR	* )"AUTO_AIM task", 		
                 (OS_TASK_PTR )AUTO_AIM_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )AUTO_AIM_TASK_PRIO,     	
                 (CPU_STK   * )&AUTO_AIM_TASK_STK[0],	
                 (CPU_STK_SIZE)AUTO_AIM_STK_SIZE/10,	
                 (CPU_STK_SIZE)AUTO_AIM_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	                     
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//进入临界区
}

#endif