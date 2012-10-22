/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

//#pragma import(__use_c99_library)	  

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/
#define  OS_VIEW_MODULE                  DEF_ENABLED	     	/* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */
#define  GPS_MODULE						 DEF_DISABLED			/* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */
#define  DEBUG_MODULE					 DEF_ENABLED			/* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */

/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/
/*
#define  APP_TASK_START_PRIO                               3
#define  GPS_TASK_PRIO               				       4

#define  APP_TASK_TERMINAL_PRIO							  11

#define  OS_VIEW_TASK_PRIO               (OS_LOWEST_PRIO - 3)
#define  OS_VIEW_TASK_ID 				 (OS_LOWEST_PRIO - 3)
*/
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/
/*
#define  APP_TASK_START_STK_SIZE                         512

#define  APP_TASK_TERMINAL_STK_SIZE						 128

#define  OS_VIEW_TASK_STK_SIZE                           128

#define  GPS_TASK_STK_SIZE                           	 128
*/
/*
*********************************************************************************************************
*                                         uC/OS-View CONFIGURATION
*********************************************************************************************************
*/
/*
#define  OS_VIEW_UART_1                         1
#define  OS_VIEW_UART_2                         2
#define  OS_VIEW_UART_3                         3

#define  OS_VIEW_PARSE_TASK                     DEF_DISABLED           	// Parsing of received packets will be done by a task   

#define  OS_VIEW_TIMER_SEL                      2                     	// Choose an internal timer to use for OSView 0, 1, 2   
#define  OS_VIEW_COMM_SEL                  		OS_VIEW_UART_3          // Select UART1 for uC/OS-View to use                   


#define  GPS_UART_1                         	1
#define  GPS_UART_2                         	2
#define  GPS_UART_3                         	3
#define  GPS_COMM_SEL							GPS_UART_3


#define  DEBUG_UART_1                         	1
#define  DEBUG_UART_2                         	2
#define  DEBUG_UART_3                         	3
#define  DEBUG_COMM_SEL							DEBUG_UART_1
*/
/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED

#endif
