#ifndef MIAN_H
#define MIAN_H
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define LED0_TASK_PRIO		4
//�����ջ��С	
#define LED0_STK_SIZE 		128
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//�������ȼ�
#define READ_TASK_PRIO		4
//�����ջ��С	
#define READ_STK_SIZE 		512
//������ƿ�
OS_TCB READTaskTCB;
//�����ջ	
CPU_STK READ_TASK_STK[READ_STK_SIZE];
//������
void READ_task(void *p_arg);

//�������ȼ�
#define CAN_SEND_TASK_PRIO		4
//�����ջ��С	
#define CAN_SEND_STK_SIZE 		128
//������ƿ�
OS_TCB CAN_SENDTaskTCB;
//�����ջ	
CPU_STK CAN_SEND_TASK_STK[START_STK_SIZE];
//������
void CAN_SEND_task(void *p_arg);

//�������ȼ�
#define AIM_TASK_PRIO		4
//�����ջ��С	
#define AIM_STK_SIZE 		128
//������ƿ�
OS_TCB AIMTaskTCB;
//�����ջ	
CPU_STK AIM_TASK_STK[AIM_STK_SIZE];
//������
void AIM_task(void *p_arg);

//�������ȼ�
#define MOVE_TASK_PRIO		4
//�����ջ��С	
#define MOVE_STK_SIZE 		256
//������ƿ�
OS_TCB MOVETaskTCB;
//�����ջ	
CPU_STK MOVE_TASK_STK[MOVE_STK_SIZE];
//������
void MOVE_task(void *p_arg);

//�������ȼ�
#define SHOOT_TASK_PRIO		4
//�����ջ��С	
#define SHOOT_STK_SIZE 		256
//������ƿ�
OS_TCB SHOOTTaskTCB;
//�����ջ	
CPU_STK SHOOT_TASK_STK[SHOOT_STK_SIZE];
//������
void SHOOT_task(void *p_arg);

//�������ȼ�
#define DEC_TASK_PRIO		4
//�����ջ��С	
#define DEC_STK_SIZE 		256
//������ƿ�
OS_TCB DECTaskTCB;
//�����ջ	
CPU_STK DEC_TASK_STK[DEC_STK_SIZE];
//������
void DEC_task(void *p_arg);

//�������ȼ�
#define AUTO_AIM_TASK_PRIO		4
//�����ջ��С	
#define AUTO_AIM_STK_SIZE 		128
//������ƿ�
OS_TCB AUTO_AIM_TaskTCB;
//�����ջ	
CPU_STK AUTO_AIM_TASK_STK[DEC_STK_SIZE];
//������
void AUTO_AIM_task(void *p_arg);

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���	
	
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
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�����ٽ���
}

#endif