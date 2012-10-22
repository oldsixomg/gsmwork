/* ----------------------------------------------------------------------------
*         vteng ��˾�ṩ֧�� 
* ----------------------------------------------------------------------------
* ��Ȩ��2010 VTENG��˾
*
* ��˾��������Ȩ��
*
*���ļ�Ϊ��������ڣ��������ø��ֳ�ʼ����������������������
*
*
*
*
* ----------------------------------------------------------------------------
*/

//�궨��
#define  TASK_STK_SIZE       64
#define  TASK_START_PRIO    (OS_LOWEST_PRIO - 3)

//ͷ�ļ�

#include "bsp.h"
#include "drive.h"
#include "phe.h"
#include "sio.h"
#include "efs.h"
#include "os.h"

//��������
extern OS_FLAG_GRP  *data_cmd_event;


//��������
OS_STK        AppStartTaskStk[TASK_STK_SIZE];
static  void  AppStartTask(void *p_arg);

OS_STK        sio_task_stk[TASK_STK_SIZE];
#define SIO_TASK_STK_SIZE TASK_STK_SIZE
#define  SIO_TASK_PRIO    (OS_LOWEST_PRIO - 4)

OS_STK        phe_task_stk[TASK_STK_SIZE];
#define PHE_TASK_STK_SIZE TASK_STK_SIZE
#define  PHE_TASK_PRIO    (OS_LOWEST_PRIO - 5)


OS_STK        efs_task_stk[TASK_STK_SIZE];
#define EFS_TASK_STK_SIZE TASK_STK_SIZE
#define  EFS_TASK_PRIO    (OS_LOWEST_PRIO - 6)

//��������
static void App_TaskCreate (void);

//��������
/*********************************************************************************************************
*˵����main��������������
* ��������
* ����ֵ����
********************************************************************************************************/
void  main (void) {
 
  AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;  // ��ֹ�����ж�
  OSInit();                               // ��ʼ�� uC/OS-II
  os_init();
  
  OSTaskCreateExt(AppStartTask,
                  (void *)0,
                  (OS_STK *)&AppStartTaskStk[TASK_STK_SIZE-1],
                  TASK_START_PRIO,
                  TASK_START_PRIO,
                  (OS_STK *)&AppStartTaskStk[0],
                  TASK_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
  
  OSStart();                              // ����������
}

/*********************************************************************************************************
*˵����AppStartTask��������������
* ������void *p_arg (������OSTaskCreate()���ݸ�AppStartTask())
* ����ֵ����
********************************************************************************************************/
static  void  AppStartTask (void *p_arg) {
  
  (void)p_arg;
  CPU_Init();                             // ��ʼ��������ָ����
  BSP_Init();                             // ��ʼ�� BSP ����
#if OS_TASK_STAT_EN > 0
  OSStatInit();                           // ȷ��CPU����
#endif
  
  App_TaskCreate ();
  while (1) 
  {                   
  }
}




/*********************************************************************************************************
*˵����App_TaskCreate�����ڴ�����������
* ������void *p_arg (������OSTaskCreate()���ݸ�AppStartTask())
* ����ֵ����
********************************************************************************************************/
static void App_TaskCreate (void)
{

  OSTaskCreateExt(TS_sio_task,
                  (void *)0,
                  (OS_STK *)&sio_task_stk[SIO_TASK_STK_SIZE-1],
                  SIO_TASK_PRIO,
                  SIO_TASK_PRIO,
                  (OS_STK *)&sio_task_stk[0],
                  SIO_TASK_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
  

  OSTaskCreateExt(TS_phe_task,
                  (void *)0,
                  (OS_STK *)&phe_task_stk[PHE_TASK_STK_SIZE-1],
                  PHE_TASK_PRIO,
                  PHE_TASK_PRIO,
                  (OS_STK *)&phe_task_stk[0],
                  PHE_TASK_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
  
  

  
  OSTaskCreateExt(TS_efs_task,
                  (void *)0,
                  (OS_STK *)&efs_task_stk[EFS_TASK_STK_SIZE-1],
                  EFS_TASK_PRIO,
                  EFS_TASK_PRIO,
                  (OS_STK *)&efs_task_stk[0],
                  EFS_TASK_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

}




