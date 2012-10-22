/* ----------------------------------------------------------------------------
 *         vteng ��˾�ṩ֧�� 
 * ----------------------------------------------------------------------------
 * ��Ȩ��2010 VTENG��˾
 *
 * ��˾��������Ȩ��
 *
 *task.h ���ļ�Ϊ�����ջ�����ȼ��������ǵ�Ҫ��������ط���һ�����������ȼ���һ����
 *�����ջ
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

//���ó�������
#define TASK_STACK_SIZE    128



/////////////////////////////////���ȼ����//////////////////////////////////////////
//���ȼ�����(��ö�ٱ���)
enum {
   MAX_TASK_PRIO = 0,
//�ļ�ϵͳ
#ifdef FEATURE_ED_EFS
   EFS_TASK_PRIO,
#endif 

//����ҵ��
#ifdef FEATURE_ED_DATA
   ALARM_TASK_PRIO,
#endif 

//SFN
#ifdef FEATURE_ED_SFN
   SFN_TASK_PRIO,
#endif 
//����ҵ��
#ifdef FEATURE_ED_DATA
   DATA_TASK_PRIO,
#endif 
//���������շ�����
#ifdef FEATURE_ED_SIO
   SIO_TASK_PRIO,
#endif 



};
////////////////////////////////////////////////////////////////




/////////////////////////////�����ջ����///////////////////////////////////////////////
////////////�뵽��������ĵط����¶�ջ��С�����ȼ��ȡ�///////

//���������շ�����
#ifdef FEATURE_ED_SIO
#ifndef SIO_TASK
#define EXT_SIO_TASK    extern 
#else
#define EXT_SIO_TASK
#endif
#define        SIO_TASK_STK_SIZE   TASK_STACK_SIZE       //��ջ��С
EXT_SIO_TASK   OS_STK   sio_task_stk[SIO_TASK_STK_SIZE]; //��ջ����
extern void  sio_task(void *p_arg);                      //��������
#endif//sio

//�ļ�ϵͳ
#ifdef FEATURE_ED_EFS
#ifndef EFS_TASK
#define EXT_EFS_TASK   extern 
#else
#define EXT_EFS_TASK
#endif
#define       EFS_TASK_STK_SIZE   TASK_STACK_SIZE       //��ջ��С
EXT_EFS_TASK  OS_STK              efs_task_stk[EFS_TASK_STK_SIZE];
extern void         efs_task(void *p_arg);
#endif //EFS

//����ҵ��
#ifdef FEATURE_ED_DATA
#ifndef DATA_TASK
#define EXT_DATA_TASK   extern 
#else
#define EXT_DATA_TASK
#endif
#define       DATA_TASK_STK_SIZE   TASK_STACK_SIZE       //��ջ��С
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
#define       ALARM_TASK_STK_SIZE   TASK_STACK_SIZE       //��ջ��С
EXT_ALARM_TASK  OS_STK              alarm_task_stk[ALARM_TASK_STK_SIZE];
extern void         alarm_task(void *p_arg);
#endif//DATA





#ifdef FEATURE_ED_SFN
#ifndef SFN_TASK
#define EXT_SFN_TASK   extern 
#else
#define EXT_SFN_TASK
#endif
#define       SFN_TASK_STK_SIZE   TASK_STACK_SIZE       //��ջ��С
EXT_SFN_TASK  OS_STK              sfn_task_stk[SFN_TASK_STK_SIZE];
extern void         sfn_task(void *p_arg);
#endif//SFN

////////////////////////////////////////////////////////////////////////////////////////////

#endif//TASK_H