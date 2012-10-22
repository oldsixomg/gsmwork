/* ----------------------------------------------------------------------------
*         vteng ��˾�ṩ֧�� 
* ----------------------------------------------------------------------------
* ��Ȩ��2010 VTENG��˾
*
* ��˾��������Ȩ��
*
*���ļ�ΪCPU�ײ����REG��ʼ������
*
*
*
*
*
* ----------------------------------------------------------------------------
*/
//�궨��
#define  BSP_UNDEF_INSTRUCTION_VECTOR_ADDR   (*(INT32U *)0x00000004L)
#define  BSP_SWI_VECTOR_ADDR                 (*(INT32U *)0x00000008L)
#define  BSP_PREFETCH_ABORT_VECTOR_ADDR      (*(INT32U *)0x0000000CL)
#define  BSP_DATA_ABORT_VECTOR_ADDR          (*(INT32U *)0x00000010L)
#define  BSP_IRQ_VECTOR_ADDR                 (*(INT32U *)0x00000018L)
#define  BSP_FIQ_VECTOR_ADDR                 (*(INT32U *)0x0000001CL)
#define  BSP_IRQ_ISR_ADDR                    (*(INT32U *)0x00000038L)
#define  BSP_FIQ_ISR_ADDR                    (*(INT32U *)0x0000003CL)
typedef  void (*BSP_PFNCT)(void);


//ͷ�ļ�
#include "bsp.h"
#include "button.h"
#include "twc.h"

//��������
//��������
//��������
static  void  BSP_DummyISR_Handler(void);
static  void  BSP_IntCtrlInit(void);
static  void  Tmr_TickInit(void);
static  void  Tmr_TickISR_Handler(void);
extern void uart_init(void);
extern void PIOA_IRQ_Handler(void);

//��������
/*
��������bsp_ed_init
������
����ֵ����
������
*/
static void  bsp_ed_init(void)
{
  //  enable the clock of the PIO
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA ) ;


  //OPEN�߼��жϿ���������
  AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                        AT91C_ID_PIOA,
                        AT91C_AIC_PRIOR_HIGHEST,
                        AT91C_AIC_SRCTYPE_EXT_NEGATIVE_EDGE,
                        PIOA_IRQ_Handler);	//����ǰ���һ��
  AT91F_AIC_ClearIt(AT91C_BASE_AIC,0xffffffff);
  AT91F_PIO_GetInterruptStatus(AT91C_BASE_PIOA);
  //���ж�
  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_PIOA);
  //ͨ��IO���ж�����
  AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
  //ʹ���жϣ��ؼ�
  __enable_interrupt();
  

}

/*
��������PIOA_IRQ_Handler
������
����ֵ����
������
*/
void  PIOA_IRQ_Handler(void)
{
  unsigned int interruptstatus;
  unsigned int maskStatus;
  
  AT91F_AIC_DisableIt(AT91C_BASE_AIC,AT91C_ID_PIOA);
  maskStatus=AT91F_PIO_GetInterruptMaskStatus(AT91C_BASE_PIOA);//��ֹ�ж�
  AT91F_AIC_ClearIt(AT91C_BASE_AIC,AT91C_ID_PIOA); //�߼��жϿ�������������жϱ�־���ͼ��Ķ�SR�ͽ��������
  interruptstatus=AT91F_PIO_GetInterruptStatus(AT91C_BASE_PIOA);//��ISR�����ǲ���P_SW�жϷ���
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
* ˵����BSP_DummyISR_Handler��������ԪIRQ��������ڴ���Ƿ�IRQ�쳣
* ��������
* ����ֵ����
********************************************************************************************************/
static  void  BSP_DummyISR_Handler (void) {
  AT91C_BASE_AIC->AIC_IVR = 0;
}






void  BSP_Init (void) {
  BSP_IntCtrlInit();                               // ��ʼ���жϿ�����
  Tmr_TickInit();                                  // ��ʼ�� uC/OS-II �Ľ����ٶ�
  timer_init();
  
  bsp_ed_init();
  
  
}

/********************************************************************************************************
* ˵����BSP_IntCtrlInit��������ʼ���жϿ����������ñ���������ֹ�����жϡ�
* ��������
* ����ֵ����
********************************************************************************************************/
static  void  BSP_IntCtrlInit (void) {
  INT16U  i;
  
  BSP_IRQ_VECTOR_ADDR               =  0xE59FF018;               // LDR PC,[PC,#0x18] ָ��
  //LWL ��������ͽ������Լ�������жϺ�����
  BSP_IRQ_ISR_ADDR                  =  (INT32U)OS_CPU_IRQ_ISR;   // IRQ �쳣������ַ
  BSP_FIQ_VECTOR_ADDR               =  0xE59FF018;               // LDR PC,[PC,#0x18] ָ��
  BSP_FIQ_ISR_ADDR                  =  (INT32U)OS_CPU_FIQ_ISR;   // FIQ �쳣������ַ
  BSP_UNDEF_INSTRUCTION_VECTOR_ADDR =  0xEAFFFFFE;               // ��ת������
  BSP_SWI_VECTOR_ADDR               =  0xEAFFFFFE;
  BSP_PREFETCH_ABORT_VECTOR_ADDR    =  0xEAFFFFFE;
  BSP_DATA_ABORT_VECTOR_ADDR        =  0xEAFFFFFE;
  BSP_FIQ_VECTOR_ADDR               =  0xEAFFFFFE;
  
  AT91C_BASE_AIC->AIC_EOICR         =  0x00000000;               // �����ж�����
  
  for (i = 0; i < 32; i++) {                                     // ��ֹ�����ж�
    AT91C_BASE_AIC->AIC_SVR[i] = (INT32U)BSP_DummyISR_Handler;
    AT91C_BASE_AIC->AIC_SMR[i] = 0;
  }
}

/********************************************************************************************************
* ˵����BSP_IntDisAll���������ñ���������ֹ�����жϡ�
* ��������
* ����ֵ����
********************************************************************************************************/
void  BSP_IntDisAll (void) {
  AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;                         // ��ֹ�����ж�
}


/********************************************************************************************************
* ˵��:OS_CPU_IRQ_ISR_Handler������IRQ�жϾ������OS_CPU_IRQ_ISR()�������ã�ȷ���ж�Դ������
* ��������
* ����ֵ����
********************************************************************************************************/
void  OS_CPU_IRQ_ISR_Handler (void) {
  BSP_PFNCT  pfnct;
  
  pfnct = (BSP_PFNCT)AT91C_BASE_AIC->AIC_IVR;     // ��AIC�ж�ȡ�ж�����
  if (pfnct != (BSP_PFNCT)0) {                    // ȷ��û�� NULL ָ��
    (*pfnct)();                                 // ִ���жϺ���
  }
  
}
/********************************************************************************************************
* ˵��:�жϽ�����������������������ʹ�ж��޷������������������л��ͻ�������⡣
* ��������
* ����ֵ����
********************************************************************************************************/

void IRQ_handler_exit(void)
{
  AT91C_BASE_AIC->AIC_EOICR         =  0x00000000;               // �����ж�����
}

/*********************************************************************************************************
*˵����OS_CPU_FIQ_ISR_Handler������FIQ�жϾ��,��OS_CPU_IRQ_ISR()�������ã�ȷ���ж�Դ������
* ��������
* ����ֵ����
********************************************************************************************************/
void  OS_CPU_FIQ_ISR_Handler (void) {
  BSP_PFNCT  pfnct;
#if 1
  pfnct = (BSP_PFNCT)AT91C_BASE_AIC->AIC_FVR;     // ��AIC�ж�ȡ�ж�����
  if (pfnct != (BSP_PFNCT)0) {                    // ȷ��û�� NULL ָ��
    (*pfnct)();                                 // ִ���жϺ���
  }
#else
  pfnct = (BSP_PFNCT)AT91C_BASE_AIC->AIC_FVR;     // ��AIC�ж�ȡ�ж�����
  while (pfnct != (BSP_PFNCT)0) {                 // ȷ��û�� NULL ָ��
    (*pfnct)();                                   // ִ���жϺ���
    pfnct = (BSP_PFNCT)AT91C_BASE_AIC->AIC_FVR; // ��AIC�ж�ȡ�ж�����
  }
#endif
}

/*********************************************************************************************************
*˵����Tmr_TickInit���������ڽ��н��ĳ�ʼ��(ͨ��ʹ��һ����ʱ��ÿ1-100ms����һ���ж�)��
* ��������
* ����ֵ����
* ע(1) PIT Interrupt frequency:
*                         MCLK        1
*               Freq =    ---- * -----------
*                          16     (PIV + 1)
*
*                         MCLK      1
*               PIV  =  ( ---- * ------ ) - 1
*                          16     Freq
*
*���У�MCLK = 48 MHz (i.e 48,000,000)
*      Freq = Desired frequency (i.e. OS_TICKS_PER_SEC)
********************************************************************************************************/
static  void  Tmr_TickInit (void) {
  INT32U  counts;
  // ����PIT������ַ
  AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (INT32U)Tmr_TickISR_Handler;
  AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL
    | AT91C_AIC_PRIOR_LOWEST;
  AT91C_BASE_AIC->AIC_ICCR              = 1 << AT91C_ID_SYS;
  AT91C_BASE_AIC->AIC_IECR              = 1 << AT91C_ID_SYS;
  counts                                = (48000000 / 16 / OS_TICKS_PER_SEC) - 1;
  AT91C_BASE_PITC->PITC_PIMR            = AT91C_PITC_PITEN | AT91C_PITC_PITIEN | counts;
}


/*********************************************************************************************************
*˵����Tmr_TickISR_Handler������PIT IRQ ��������ڴ������uC/OS-II���ĵ�PIT�жϡ�
* ��������
* ����ֵ����
*********************************************************************************************************/
static  void  Tmr_TickISR_Handler (void) {
  volatile  INT32U  status;
  status                    = AT91C_BASE_PITC->PITC_PIVR;
  AT91C_BASE_AIC->AIC_IVR   = 0;
  AT91C_BASE_AIC->AIC_ICCR  = AT91C_ID_SYS; // ���timer0 �ж�
  AT91C_BASE_AIC->AIC_EOICR = 0;            // �жϽ����ź�
  OSTimeTick();                             // ֪ͨ uC/OS-II ʱ�ӽ���
}





