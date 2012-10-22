
#define  OS_CPU_GLOBALS
#include <ucos_ii.h>

#if      OS_VIEW_MODULE > 0
#include <OS_VIEWc.H>
#include <OS_VIEW.H>
#endif

#define  ARM_MODE_ARM     0x00000000
#define  ARM_MODE_THUMB   0x00000020

#if __CPU_MODE__ == 1
#define  ARM_SVC_MODE    (0x00000013L + ARM_MODE_THUMB)
#else
#define  ARM_SVC_MODE    (0x00000013L + ARM_MODE_ARM)
#endif

/*********************************************************************************************************
 *˵ ����OSInitHookBegin��������OSInit()�����ڿ�ʼʱ����,�����ڼ�Ӧ���жϡ�
 * ��������
 * ����ֵ����
********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void){
}
#endif

/*********************************************************************************************************
 * ˵ ����OSInitHookEnd��������OSInit()�����ڽ���ʱ����,�����ڼ�Ӧ���жϡ�
 * ��������
 * ����ֵ����
********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void) {
#if OS_CPU_INT_DIS_MEAS_EN > 0
    OS_CPU_IntDisMeasInit();
#endif
}
#endif

/*********************************************************************************************************
 * ˵ ����OSTaskCreateHook��������������ʱ����,�����ڼ�Ӧ���жϡ�
 * ������OS_TCB *ptcb��ָ�򱻴�������������ƿ��ָ��
 * ����ֵ����
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
void  OSTaskCreateHook (OS_TCB *ptcb) {
#if OS_VIEW_MODULE > 0
    OSView_TaskCreateHook(ptcb);
#else
    (void)ptcb;
#endif
}
#endif


/*********************************************************************************************************
 * ˵����OSTaskDelHook������ɾ������ʱ���ã������ڼ�Ӧ���жϡ�
 * ������OS_TCB *ptcb��ָ�򱻴�������������ƿ��ָ��
 * ����ֵ����
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
void  OSTaskDelHook (OS_TCB *ptcb) {
    (void)ptcb;
}
#endif

/*********************************************************************************************************
 * ˵����OSTaskIdleHook�������ɿ����������,�����ڼ�Ӧ���жϡ�����ú��������û�ֹͣCPU�Խ��͹��ġ�
 * ��������
 * ����ֵ����
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void) {
}
#endif

/*********************************************************************************************************
* ˵����OSTaskStatHook��������ͳ������ÿ�����һ�Σ������û���ͳ�������м����������ܡ�STATISTIC TASK HOOK
 * ��������
 * ����ֵ����
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
void  OSTaskStatHook (void) {
}
#endif

/*********************************************************************************************************
 * ˵����*OSTaskStkInit��������ʼ�������ջ���ú�����OSTaskCreate() �� OSTaskCreateExt() ���á�
 * ������task  Ϊָ����������ָ�롣
 *       p_arg Ϊָ���û���������ָ�룬�û����ݵ������״�ִ��ʱ���ݸ�����
 *       ptos  Ϊָ��ջ����ָ�룬�ٶ�ptosָ�������ջ��'free'��ڡ���� OS_STK_GROWTH ����Ϊ1����ptos����
               ������������Ч��ַ�����Ƶأ����OS_STK_GROWTH ����Ϊ0����ptos����������������Ч��ַ��
 *       opt   ָ�����ڸı�OSTaskStkInit()���õ�ѡ�
 * ����ֵ��һ���������ļĴ����Ժ��ʵ�˳������ջ���������µ�ջ��ֵ��
 * ע�⣺ 1) ����ʼִ�к��жϿ��ţ�
*         2) ���������Թ���ģʽ���С�
*********************************************************************************************************/
OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt) {
    OS_STK *stk;
    opt      = opt;                         // ����optδʹ��
    stk      = ptos;                        // ���ض�ջָ��
    *(stk)   = (OS_STK)task;                // ���
    *(--stk) = (INT32U)0x14141414L;         // R14 (LR)
    *(--stk) = (INT32U)0x12121212L;         // R12
    *(--stk) = (INT32U)0x11111111L;         // R11
    *(--stk) = (INT32U)0x10101010L;         // R10
    *(--stk) = (INT32U)0x09090909L;         // R9
    *(--stk) = (INT32U)0x08080808L;         // R8
    *(--stk) = (INT32U)0x07070707L;         // R7
    *(--stk) = (INT32U)0x06060606L;         // R6
    *(--stk) = (INT32U)0x05050505L;         // R5
    *(--stk) = (INT32U)0x04040404L;         // R4
    *(--stk) = (INT32U)0x03030303L;         // R3
    *(--stk) = (INT32U)0x02020202L;         // R2
    *(--stk) = (INT32U)0x01010101L;         // R1
    *(--stk) = (INT32U)p_arg;               // R0 : argument
    *(--stk) = (INT32U)ARM_SVC_MODE;        // CPSR  (����IRQ��FIQ�ж�)
    return (stk);
}

/*********************************************************************************************************
 * ˵����OSTaskSwHook�����������л�ʱ���ñ������������û����������л��ڼ�ִ������������
 * ��������
 * ע�⣺1)���������ڼ�Ҫ���жϡ�
 *       2)�ٶ�ȫ��ָ��OSTCBHighRdyָ�򽫱��л�������������ȼ������TCB������OSTCBCurָ�򽫱��л�
 *         ��ȥ������
*********************************************************************************************************/
#if (OS_CPU_HOOKS_EN > 0) && (OS_TASK_SW_HOOK_EN > 0)
void  OSTaskSwHook (void) {
#if OS_VIEW_MODULE > 0
    OSView_TaskSwHook();
#endif
}
#endif

/*********************************************************************************************************
 * ˵����OSTCBInitHook������������󲿷�TCB֮����OS_TCBInit()���ñ�������
 * ������ptcb Ϊָ�򽫱���������TCB��ָ�롣
 * ע�⣺���������ڼ���Կ��жϣ�Ҳ���Թ��жϡ�
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb) {
    (void)ptcb;
}
#endif

/*********************************************************************************************************
 * ˵����OSTimeTickHook������ÿ��ʱ�ӽ��ĵ���һ�α�������
 * ��������
 * ע�⣺���������ڼ���Կ��жϣ�Ҳ���Թ��жϡ�
*********************************************************************************************************/
#if (OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)
void  OSTimeTickHook (void) {
#if OS_VIEW_MODULE > 0
    OSView_TickHook();
#endif
}
#endif

/*********************************************************************************************************
 * ˵����OS_CPU_IntDisMeasInit������
*********************************************************************************************************/
#if OS_CPU_INT_DIS_MEAS_EN > 0
void  OS_CPU_IntDisMeasInit (void) {
    OS_CPU_IntDisMeasNestingCtr = 0;
    OS_CPU_IntDisMeasCntsEnter  = 0;
    OS_CPU_IntDisMeasCntsExit   = 0;
    OS_CPU_IntDisMeasCntsMax    = 0;
    OS_CPU_IntDisMeasCntsDelta  = 0;
    OS_CPU_IntDisMeasCntsOvrhd  = 0;
    OS_CPU_IntDisMeasStart();
    OS_CPU_IntDisMeasStop();
    OS_CPU_IntDisMeasCntsOvrhd  = OS_CPU_IntDisMeasCntsDelta;
}

void  OS_CPU_IntDisMeasStart (void) {
    OS_CPU_IntDisMeasNestingCtr++;
    if (OS_CPU_IntDisMeasNestingCtr == 1) {
        OS_CPU_IntDisMeasCntsEnter = OS_CPU_IntDisMeasTmrRd();
    }
}

void  OS_CPU_IntDisMeasStop (void) {
    OS_CPU_IntDisMeasNestingCtr--;
    if (OS_CPU_IntDisMeasNestingCtr == 0) {
        OS_CPU_IntDisMeasCntsExit  = OS_CPU_IntDisMeasTmrRd();
        OS_CPU_IntDisMeasCntsDelta = OS_CPU_IntDisMeasCntsExit - OS_CPU_IntDisMeasCntsEnter;
        if (OS_CPU_IntDisMeasCntsDelta > OS_CPU_IntDisMeasCntsOvrhd) {
            OS_CPU_IntDisMeasCntsDelta -= OS_CPU_IntDisMeasCntsOvrhd;
        } else {
            OS_CPU_IntDisMeasCntsDelta  = OS_CPU_IntDisMeasCntsOvrhd;
        }
        if (OS_CPU_IntDisMeasCntsDelta > OS_CPU_IntDisMeasCntsMax) {
            OS_CPU_IntDisMeasCntsMax = OS_CPU_IntDisMeasCntsDelta;
        }
    }
}
#endif
