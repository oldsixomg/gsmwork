/* ----------------------------------------------------------------------------
*         vteng 公司提供支持 
* ----------------------------------------------------------------------------
* 版权：2010 VTENG公司
*
* 公司保留所有权力
*
*此文件为CPU底层相关REG初始化操作
*
*
*
*
*
* ----------------------------------------------------------------------------
*/
//宏定义
#define  BSP_UNDEF_INSTRUCTION_VECTOR_ADDR   (*(INT32U *)0x00000004L)
#define  BSP_SWI_VECTOR_ADDR                 (*(INT32U *)0x00000008L)
#define  BSP_PREFETCH_ABORT_VECTOR_ADDR      (*(INT32U *)0x0000000CL)
#define  BSP_DATA_ABORT_VECTOR_ADDR          (*(INT32U *)0x00000010L)
#define  BSP_IRQ_VECTOR_ADDR                 (*(INT32U *)0x00000018L)
#define  BSP_FIQ_VECTOR_ADDR                 (*(INT32U *)0x0000001CL)
#define  BSP_IRQ_ISR_ADDR                    (*(INT32U *)0x00000038L)
#define  BSP_FIQ_ISR_ADDR                    (*(INT32U *)0x0000003CL)
typedef  void (*BSP_PFNCT)(void);


//头文件
#include "bsp.h"
#include "button.h"
#include "twc.h"

//变量引用
//变量定义
//函数引用
static  void  BSP_DummyISR_Handler(void);
static  void  BSP_IntCtrlInit(void);
static  void  Tmr_TickInit(void);
static  void  Tmr_TickISR_Handler(void);
extern void uart_init(void);
extern void PIOA_IRQ_Handler(void);

//函数定义
/*
函数名：bsp_ed_init
参数：
返回值：无
描述：
*/
static void  bsp_ed_init(void)
{
  //  enable the clock of the PIO
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA ) ;


  //OPEN高级中断控制器配置
  AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                        AT91C_ID_PIOA,
                        AT91C_AIC_PRIOR_HIGHEST,
                        AT91C_AIC_SRCTYPE_EXT_NEGATIVE_EDGE,
                        PIOA_IRQ_Handler);	//工作前清除一次
  AT91F_AIC_ClearIt(AT91C_BASE_AIC,0xffffffff);
  AT91F_PIO_GetInterruptStatus(AT91C_BASE_PIOA);
  //打开中断
  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_PIOA);
  //通用IO口中断配置
  AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
  //使能中断，必加
  __enable_interrupt();
  

}

/*
函数名：PIOA_IRQ_Handler
参数：
返回值：无
描述：
*/
void  PIOA_IRQ_Handler(void)
{
  unsigned int interruptstatus;
  unsigned int maskStatus;
  
  AT91F_AIC_DisableIt(AT91C_BASE_AIC,AT91C_ID_PIOA);
  maskStatus=AT91F_PIO_GetInterruptMaskStatus(AT91C_BASE_PIOA);//禁止中断
  AT91F_AIC_ClearIt(AT91C_BASE_AIC,AT91C_ID_PIOA); //高级中断控制器里面清除中断标志（低级的读SR就进行清除）
  interruptstatus=AT91F_PIO_GetInterruptStatus(AT91C_BASE_PIOA);//读ISR，看是不是P_SW中断发生
  if ((interruptstatus & (GLOBE_RST_KEY|GLOBE_SET_KEY)) & maskStatus)
  {
    DR_key_isr_cb();
  }    
  if ((interruptstatus & GLOBE_TWC_RX) & maskStatus)
  {
    DR_twc_isr_cb();
  } 
  AT91F_PIO_GetInterruptStatus(AT91C_BASE_PIOA);
  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_PIOA);
}




/********************************************************************************************************
* 说明：BSP_DummyISR_Handler函数，哑元IRQ句柄，用于处理非法IRQ异常
* 参数：无
* 返回值：无
********************************************************************************************************/
static  void  BSP_DummyISR_Handler (void) {
  AT91C_BASE_AIC->AIC_IVR = 0;
}






void  BSP_Init (void) {
  BSP_IntCtrlInit();                               // 初始化中断控制器
  Tmr_TickInit();                                  // 初始化 uC/OS-II 的节拍速度
  timer_init();
  
  bsp_ed_init();
  
  
}

/********************************************************************************************************
* 说明：BSP_IntCtrlInit函数，初始化中断控制器。调用本函数将禁止所有中断。
* 参数：无
* 返回值：无
********************************************************************************************************/
static  void  BSP_IntCtrlInit (void) {
  INT16U  i;
  
  BSP_IRQ_VECTOR_ADDR               =  0xE59FF018;               // LDR PC,[PC,#0x18] 指令
  //LWL 加了下面就进不了自己定义的中断函数。
  BSP_IRQ_ISR_ADDR                  =  (INT32U)OS_CPU_IRQ_ISR;   // IRQ 异常向量地址
  BSP_FIQ_VECTOR_ADDR               =  0xE59FF018;               // LDR PC,[PC,#0x18] 指令
  BSP_FIQ_ISR_ADDR                  =  (INT32U)OS_CPU_FIQ_ISR;   // FIQ 异常向量地址
  BSP_UNDEF_INSTRUCTION_VECTOR_ADDR =  0xEAFFFFFE;               // 跳转到自身
  BSP_SWI_VECTOR_ADDR               =  0xEAFFFFFE;
  BSP_PREFETCH_ABORT_VECTOR_ADDR    =  0xEAFFFFFE;
  BSP_DATA_ABORT_VECTOR_ADDR        =  0xEAFFFFFE;
  BSP_FIQ_VECTOR_ADDR               =  0xEAFFFFFE;
  
  AT91C_BASE_AIC->AIC_EOICR         =  0x00000000;               // 结束中断命令
  
  for (i = 0; i < 32; i++) {                                     // 禁止所有中断
    AT91C_BASE_AIC->AIC_SVR[i] = (INT32U)BSP_DummyISR_Handler;
    AT91C_BASE_AIC->AIC_SMR[i] = 0;
  }
}

/********************************************************************************************************
* 说明：BSP_IntDisAll函数，调用本函数将禁止所有中断。
* 参数：无
* 返回值：无
********************************************************************************************************/
void  BSP_IntDisAll (void) {
  AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;                         // 禁止所有中断
}


/********************************************************************************************************
* 说明:OS_CPU_IRQ_ISR_Handler函数，IRQ中断句柄，由OS_CPU_IRQ_ISR()函数调用，确定中断源并处理。
* 参数：无
* 返回值：无
********************************************************************************************************/
void  OS_CPU_IRQ_ISR_Handler (void) {
  BSP_PFNCT  pfnct;
  
  pfnct = (BSP_PFNCT)AT91C_BASE_AIC->AIC_IVR;     // 从AIC中读取中断向量
  if (pfnct != (BSP_PFNCT)0) {                    // 确保没有 NULL 指针
    (*pfnct)();                                 // 执行中断函数
  }
  
}
/********************************************************************************************************
* 说明:中断结束命令函数，不加这个函数会使中断无法正常结束，那任务切换就会出现问题。
* 参数：无
* 返回值：无
********************************************************************************************************/

void IRQ_handler_exit(void)
{
  AT91C_BASE_AIC->AIC_EOICR         =  0x00000000;               // 结束中断命令
}

/*********************************************************************************************************
*说明：OS_CPU_FIQ_ISR_Handler函数，FIQ中断句炳,由OS_CPU_IRQ_ISR()函数调用，确定中断源并处理。
* 参数：无
* 返回值：无
********************************************************************************************************/
void  OS_CPU_FIQ_ISR_Handler (void) {
  BSP_PFNCT  pfnct;
#if 1
  pfnct = (BSP_PFNCT)AT91C_BASE_AIC->AIC_FVR;     // 从AIC中读取中断向量
  if (pfnct != (BSP_PFNCT)0) {                    // 确保没有 NULL 指针
    (*pfnct)();                                 // 执行中断函数
  }
#else
  pfnct = (BSP_PFNCT)AT91C_BASE_AIC->AIC_FVR;     // 从AIC中读取中断向量
  while (pfnct != (BSP_PFNCT)0) {                 // 确保没有 NULL 指针
    (*pfnct)();                                   // 执行中断函数
    pfnct = (BSP_PFNCT)AT91C_BASE_AIC->AIC_FVR; // 从AIC中读取中断向量
  }
#endif
}

/*********************************************************************************************************
*说明：Tmr_TickInit函数，用于进行节拍初始化(通常使用一个定时器每1-100ms产生一次中断)。
* 参数：无
* 返回值：无
* 注(1) PIT Interrupt frequency:
*                         MCLK        1
*               Freq =    ---- * -----------
*                          16     (PIV + 1)
*
*                         MCLK      1
*               PIV  =  ( ---- * ------ ) - 1
*                          16     Freq
*
*其中：MCLK = 48 MHz (i.e 48,000,000)
*      Freq = Desired frequency (i.e. OS_TICKS_PER_SEC)
********************************************************************************************************/
static  void  Tmr_TickInit (void) {
  INT32U  counts;
  // 设置PIT向量地址
  AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (INT32U)Tmr_TickISR_Handler;
  AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL
    | AT91C_AIC_PRIOR_LOWEST;
  AT91C_BASE_AIC->AIC_ICCR              = 1 << AT91C_ID_SYS;
  AT91C_BASE_AIC->AIC_IECR              = 1 << AT91C_ID_SYS;
  counts                                = (48000000 / 16 / OS_TICKS_PER_SEC) - 1;
  AT91C_BASE_PITC->PITC_PIMR            = AT91C_PITC_PITEN | AT91C_PITC_PITIEN | counts;
}


/*********************************************************************************************************
*说明：Tmr_TickISR_Handler函数，PIT IRQ 句柄。用于处理产生uC/OS-II节拍的PIT中断。
* 参数：无
* 返回值：无
*********************************************************************************************************/
static  void  Tmr_TickISR_Handler (void) {
  volatile  INT32U  status;
  status                    = AT91C_BASE_PITC->PITC_PIVR;
  AT91C_BASE_AIC->AIC_IVR   = 0;
  AT91C_BASE_AIC->AIC_ICCR  = AT91C_ID_SYS; // 清除timer0 中断
  AT91C_BASE_AIC->AIC_EOICR = 0;            // 中断结束信号
  OSTimeTick();                             // 通知 uC/OS-II 时钟节拍
}





