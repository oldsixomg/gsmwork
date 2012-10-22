/* ----------------------------------------------------------------------------
 *         vteng 公司提供支持 
 * ----------------------------------------------------------------------------
 * 版权：2010 VTENG公司
 *
 * 公司保留所有权力
 *
 *task.h 此文件为任务堆栈和优先级声明。记得要添加两个地方，一个是任务优先级，一个是
 *任务堆栈
 *
 *
 *
 *
 * ----------------------------------------------------------------------------
 */
#ifndef TASK_H
#define TASK_H

#include "os_cpu.h"
#include "feature.h"

//共用常量定义
#define TASK_STACK_SIZE    128



/////////////////////////////////优先级添加//////////////////////////////////////////
//优先级定义(用枚举变量)
enum {
   MAX_TASK_PRIO = 0,
//文件系统
#ifdef FEATURE_ED_EFS
   EFS_TASK_PRIO,
#endif 

//数据业务
#ifdef FEATURE_ED_DATA
   ALARM_TASK_PRIO,
#endif 

//SFN
#ifdef FEATURE_ED_SFN
   SFN_TASK_PRIO,
#endif 
//数据业务
#ifdef FEATURE_ED_DATA
   DATA_TASK_PRIO,
#endif 
//串型数据收发任务
#ifdef FEATURE_ED_SIO
   SIO_TASK_PRIO,
#endif 



};
////////////////////////////////////////////////////////////////




/////////////////////////////任务堆栈定义///////////////////////////////////////////////
////////////须到创建任务的地方更新堆栈大小，优先级等。///////

//串型数据收发任务
#ifdef FEATURE_ED_SIO
#ifndef SIO_TASK
#define EXT_SIO_TASK    extern 
#else
#define EXT_SIO_TASK
#endif
#define        SIO_TASK_STK_SIZE   TASK_STACK_SIZE       //堆栈大小
EXT_SIO_TASK   OS_STK   sio_task_stk[SIO_TASK_STK_SIZE]; //堆栈定义
extern void  sio_task(void *p_arg);                      //任务引用
#endif//sio

//文件系统
#ifdef FEATURE_ED_EFS
#ifndef EFS_TASK
#define EXT_EFS_TASK   extern 
#else
#define EXT_EFS_TASK
#endif
#define       EFS_TASK_STK_SIZE   TASK_STACK_SIZE       //堆栈大小
EXT_EFS_TASK  OS_STK              efs_task_stk[EFS_TASK_STK_SIZE];
extern void         efs_task(void *p_arg);
#endif //EFS

//数据业务
#ifdef FEATURE_ED_DATA
#ifndef DATA_TASK
#define EXT_DATA_TASK   extern 
#else
#define EXT_DATA_TASK
#endif
#define       DATA_TASK_STK_SIZE   TASK_STACK_SIZE       //堆栈大小
EXT_DATA_TASK  OS_STK              data_task_stk[DATA_TASK_STK_SIZE];
extern void         data_task(void *p_arg);
#endif//DATA

//
#ifdef FEATURE_ED_ALARM
#ifndef ALARM_TASK
#define EXT_ALARM_TASK   extern 
#else
#define EXT_ALARM_TASK
#endif
#define       ALARM_TASK_STK_SIZE   TASK_STACK_SIZE       //堆栈大小
EXT_ALARM_TASK  OS_STK              alarm_task_stk[ALARM_TASK_STK_SIZE];
extern void         alarm_task(void *p_arg);
#endif//DATA





#ifdef FEATURE_ED_SFN
#ifndef SFN_TASK
#define EXT_SFN_TASK   extern 
#else
#define EXT_SFN_TASK
#endif
#define       SFN_TASK_STK_SIZE   TASK_STACK_SIZE       //堆栈大小
EXT_SFN_TASK  OS_STK              sfn_task_stk[SFN_TASK_STK_SIZE];
extern void         sfn_task(void *p_arg);
#endif//SFN

////////////////////////////////////////////////////////////////////////////////////////////

#endif//TASK_H