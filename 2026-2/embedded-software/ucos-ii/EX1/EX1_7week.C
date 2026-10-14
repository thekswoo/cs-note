/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #2
*                         7주차 자유과제: Event flag / Semaphore / Mailbox / Q 종합 (GEN task 4개)
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define TASK_STK_SIZE 512 /* Size of each task's stacks (# of WORDs)       */
#define N_TASK 6          /* DISPLAY 1 + CALC 1 + GEN 4                   */
#define N_GEN 4           /* GEN task 개수 (N_TASK - 2)                   */

/* task 1 ~ N_TASK-1 의 bit 전체 : 0011 1110 = 0x3E (bit 0 = DISPLAY 제외) */
#define DONE_FLAG_ALL ((OS_FLAGS)(((1 << N_TASK) - 1) & ~0x01))

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

OS_STK TaskStartStk[TASK_STK_SIZE];
OS_STK TaskStk[N_TASK][TASK_STK_SIZE];
char TaskData[N_TASK];

/* select == 1 : mailbox */
OS_EVENT *mbox_to_calc[N_GEN];
OS_EVENT *mbox_to_disp;

/* select == 2 : message queue (자유과제) */
OS_EVENT *queue_to_calc;
OS_EVENT *queue_to_disp;
void *calc_queue[N_GEN];
void *disp_queue[1];

/* 공통 */
OS_EVENT *sem;
OS_FLAG_GRP *done_grp;

INT8U select = 1; // mailbox: 1, message queue: 2

/* GEN i 가 최댓값일 때 보낼 문자 : GEN0 'R', GEN1 'B', GEN2 'G', GEN3 'P'(보라) */
const char gen_letter[N_GEN] = {'R', 'B', 'G', 'P'};

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void Task(void *data);
static void TaskStart(void *data); /* Function prototypes of tasks                  */
static void TaskStartCreateTasks(void);
static void TaskStartDispInit(void);
static void TaskStartDisp(void);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void main(void)
{
	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK); /* Clear the screen                         */

	OSInit(); /* Initialize uC/OS-II                      */

	PC_DOSSaveReturn();		   /* Save environment to return to DOS        */
	PC_VectSet(uCOS, OSCtxSw); /* Install uC/OS-II's context switch vector */

	OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

	OSStart(); /* Start multitasking                       */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               STARTUP TASK
*********************************************************************************************************
*/

static void TaskStart(void *pdata)
{
#if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
	OS_CPU_SR cpu_sr;
#endif
	INT16S key;
	INT8U i;
	INT8U err;

	pdata = pdata; /* Prevent compiler warning                 */

	TaskStartDispInit(); /* Setup the display                        */

	OS_ENTER_CRITICAL(); /* Install uC/OS-II's clock tick ISR        */
	PC_VectSet(0x08, OSTickISR);
	PC_SetTickRate(OS_TICKS_PER_SEC); /* Reprogram tick rate                      */
	OS_EXIT_CRITICAL();

	OSStatInit(); /* Initialize uC/OS-II's statistics         */

	if (select == 1)
	{
		for (i = 0; i < N_GEN; i++)
		{
			mbox_to_calc[i] = OSMboxCreate(0);
		}
		mbox_to_disp = OSMboxCreate(0);
	}
	else if (select == 2)
	{
		queue_to_calc = OSQCreate(&calc_queue[0], N_GEN);
		queue_to_disp = OSQCreate(&disp_queue[0], 1);
	}

	sem = OSSemCreate(1);
	done_grp = OSFlagCreate(0x00, &err);

	TaskStartCreateTasks(); /* Create all other tasks                   */

	for (;;)
	{
		TaskStartDisp(); /* Update the display                       */

		if (PC_GetKey(&key))
		{ /* See if key has been pressed              */
			if (key == 0x1B)
			{					/* Yes, see if it's the ESCAPE key          */
				PC_DOSReturn(); /* Yes, return to DOS                       */
			}
		}

		OSCtxSwCtr = 0;				 /* Clear context switch counter             */
		OSTimeDly(OS_TICKS_PER_SEC); /* Wait one second                          */
	}
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        INITIALIZE THE DISPLAY
*********************************************************************************************************
*/

static void TaskStartDispInit(void)
{
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
	PC_DispStr(0, 0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
	PC_DispStr(0, 1, "                                Jean J. Labrosse                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 2, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 3, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 9, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 10, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 14, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 20, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 21, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 22, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 23, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 24, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY + DISP_BLINK);
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                           UPDATE THE DISPLAY
*********************************************************************************************************
*/

static void TaskStartDisp(void)
{
	char s[80];

	sprintf(s, "%5d", OSTaskCtr); /* Display #tasks running               */
	PC_DispStr(18, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "%3d", OSCPUUsage); /* Display CPU usage in %               */
	PC_DispStr(36, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "%5d", OSCtxSwCtr); /* Display #context switches per second */
	PC_DispStr(18, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "V%4.2f", (float)OSVersion() * 0.01); /* Display uC/OS-II's version number    */
	PC_DispStr(75, 24, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	switch (_8087)
	{ /* Display whether FPU present          */
	case 0:
		PC_DispStr(71, 22, " NO  FPU ", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 1:
		PC_DispStr(71, 22, " 8087 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 2:
		PC_DispStr(71, 22, "80287 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 3:
		PC_DispStr(71, 22, "80387 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static void TaskStartCreateTasks(void)
{
	INT8U i;

	for (i = 0; i < N_TASK; i++)
	{
		TaskData[i] = '0' + i;
		OSTaskCreate(Task, (void *)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], i + 1);
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                                 TASK
*********************************************************************************************************
*/

void Task(void *pdata)
{
	INT8U err;
	INT8U i, j;

	int task_number = (int)(*(char *)pdata - '0'); /* 0 ~ 5 */
	int gen_index = task_number - 2;			   /* GEN task 전용: 0 ~ 3 */

	INT8U push_number;
	INT8U get_number[N_GEN];
	INT8U max;
	INT8U max_task;
	INT8U same; /* 같은 숫자가 하나라도 있으면 1 */

	char push_letter;
	char get_letter;
	int fgnd_color, bgnd_color;
	char s[10];

	if (task_number == 0)
	{
		/* =============================== DISPLAY TASK (priority 1) =============================== */
		for (;;)
		{
			// CALC task로부터 문자 수신
			if (select == 1) // mail box
			{
				get_letter = *(char *)OSMboxPend(mbox_to_disp, 0, &err);
			}
			else if (select == 2) // message queue
			{
				get_letter = *(char *)OSQPend(queue_to_disp, 0, &err);
			}

			// 'R' 빨강, 'B' 파랑, 'G' 녹색, 'P' 보라
			if (get_letter == 'B')
			{
				bgnd_color = DISP_BGND_BLUE;
				fgnd_color = DISP_FGND_BLUE;
			}
			else if (get_letter == 'G')
			{
				bgnd_color = DISP_BGND_GREEN;
				fgnd_color = DISP_FGND_GREEN;
			}
			else if (get_letter == 'P')
			{
				bgnd_color = DISP_BGND_PURPLE;
				fgnd_color = DISP_FGND_PURPLE;
			}
			else /* 'R' */
			{
				bgnd_color = DISP_BGND_RED;
				fgnd_color = DISP_FGND_RED;
			}

			// 화면 칠하기
			for (j = 5; j < 20; j++)
			{
				for (i = 0; i < 80; i++)
				{
					PC_DispChar(i, j, ' ', fgnd_color + bgnd_color);
				}
			}

			// 충분한 지연
			OSTimeDlyHMSM(0, 0, 5, 0);

			// task 1~5에게 완료 알림 (0x3E)
			OSFlagPost(done_grp, DONE_FLAG_ALL, OS_FLAG_SET, &err);
		}
	}
	else if (task_number == 1)
	{
		/* ============================= CALCULATION TASK (priority 2) ============================= */
		for (;;)
		{
			// GEN task 4개로부터 숫자 수신 → get_number[]
			for (i = 0; i < N_GEN; i++)
			{
				if (select == 1) // mail box
				{
					get_number[i] = *(INT8U *)OSMboxPend(mbox_to_calc[i], 0, &err);
				}
				else if (select == 2) // message queue
				{
					get_number[i] = *(INT8U *)OSQPend(queue_to_calc, 0, &err);
				}
			}

			// 최댓값 찾기 → max_task
			max = get_number[0];
			max_task = 0;
			for (i = 1; i < N_GEN; i++)
			{
				if (get_number[i] > max)
				{
					max = get_number[i];
					max_task = i;
				}
			}

			// 두 개 이상 같은 숫자가 있는지 검사 (모든 쌍 비교)
			same = 0;
			for (i = 0; i < N_GEN; i++)
			{
				for (j = i + 1; j < N_GEN; j++)
				{
					if (get_number[i] == get_number[j])
					{
						same = 1;
					}
				}
			}

			// 같은 숫자가 있으면 'R', 아니면 최댓값 GEN의 문자
			if (same)
			{
				push_letter = 'R';
			}
			else
			{
				push_letter = gen_letter[max_task];
			}

			// DISPLAY task에 문자 전송
			if (select == 1) // mail box
			{
				OSMboxPost(mbox_to_disp, &push_letter);
			}
			else if (select == 2) // message queue
			{
				OSQPost(queue_to_disp, &push_letter);
			}

			// DISPLAY의 완료 알림 대기 (자기 비트: 1 << task_number)
			OSFlagPend(done_grp, (OS_FLAGS)(1 << task_number),
					   OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
		}
	}
	else
	{
		/* ========================== NUMBER-GENERATION TASK (priority 3~6) ========================= */
		for (;;)
		{
			// 0 ~ 63 랜덤 숫자 생성 → push_number
			push_number = random(64);

			// 생성한 숫자 화면에 표시 (x = 0, 18, 36, 54)
			sprintf(s, "%2d", push_number);
			PC_DispStr(0 + 18 * gen_index, 4, "task", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispChar(4 + 18 * gen_index, 4, *(char *)pdata, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(6 + 18 * gen_index, 4, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

			// CALC task에 숫자 전송
			if (select == 1) // mailbox
			{
				OSMboxPost(mbox_to_calc[gen_index], &push_number);
			}
			else if (select == 2) // message queue
			{
				OSQPost(queue_to_calc, &push_number);
			}

			// DISPLAY의 완료 알림 대기 (자기 비트: 1 << task_number)
			OSFlagPend(done_grp, (OS_FLAGS)(1 << task_number),
					   OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
		}
	}
}