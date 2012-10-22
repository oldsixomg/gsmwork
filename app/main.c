/* ----------------------------------------------------------------------------
*         vteng 公司提供支持 
* ----------------------------------------------------------------------------
* 版权：2010 VTENG公司
*
* 公司保留所有权力
*
*此文件为程序总入口，包括调用各种初始化函数，任务启动函数等
*
*
*
*
* ----------------------------------------------------------------------------
*/

//宏定义
#define  TASK_STK_SIZE       64
#define  TASK_START_PRIO    (OS_LOWEST_PRIO - 3)

//头文件

#include "bsp.h"
#include "drive.h"
#include "phe.h"
#include "sio.h"
#include "efs.h"
#include "os.h"

//变量引用
extern OS_FLAG_GRP  *data_cmd_event;


//变量定义
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

//函数引用
static void App_TaskCreate (void);

//函数定义
/*********************************************************************************************************
*说明：main函数，主函数。
* 参数：无
* 返回值：无
********************************************************************************************************/
void  main (void) {
 
  AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;  // 禁止所有中断
  OSInit();                               // 初始化 uC/OS-II
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
  
  OSStart();                              // 启动多任务
}

/*********************************************************************************************************
*说明：AppStartTask函数，用于任务。
* 参数：void *p_arg (参数由OSTaskCreate()传递给AppStartTask())
* 返回值：无
********************************************************************************************************/
static  void  AppStartTask (void *p_arg) {
  
  (void)p_arg;
  CPU_Init();                             // 初始化处理器指定块
  BSP_Init();                             // 初始化 BSP 函数
#if OS_TASK_STAT_EN > 0
  OSStatInit();                           // 确定CPU性能
#endif
  
  App_TaskCreate ();
  while (1) 
  {                   
  }
}




/*********************************************************************************************************
*说明：App_TaskCreate，用于创建所有任务
* 参数：void *p_arg (参数由OSTaskCreate()传递给AppStartTask())
* 返回值：无
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




