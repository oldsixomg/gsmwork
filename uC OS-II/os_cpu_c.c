
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
 *说 明：OSInitHookBegin函数，由OSInit()函数在开始时调用,调用期间应关中断。
 * 参数：无
 * 返回值：无
********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void){
}
#endif

/*********************************************************************************************************
 * 说 明：OSInitHookEnd函数，由OSInit()函数在结束时调用,调用期间应关中断。
 * 参数：无
 * 返回值：无
********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void) {
#if OS_CPU_INT_DIS_MEAS_EN > 0
    OS_CPU_IntDisMeasInit();
#endif
}
#endif

/*********************************************************************************************************
 * 说 明：OSTaskCreateHook函数，创建任务时调用,调用期间应关中断。
 * 参数：OS_TCB *ptcb，指向被创建任务任务控制块的指针
 * 返回值：无
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
 * 说明：OSTaskDelHook函数，删除任务时调用，调用期间应关中断。
 * 参数：OS_TCB *ptcb，指向被创建任务任务控制块的指针
 * 返回值：无
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
void  OSTaskDelHook (OS_TCB *ptcb) {
    (void)ptcb;
}
#endif

/*********************************************************************************************************
 * 说明：OSTaskIdleHook函数，由空闲任务调用,调用期间应关中断。加入该函数允许用户停止CPU以降低功耗。
 * 参数：无
 * 返回值：无
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void) {
}
#endif

/*********************************************************************************************************
* 说明：OSTaskStatHook函数，由统计任务每秒调用一次，允许用户在统计任务中加入其他功能。STATISTIC TASK HOOK
 * 参数：无
 * 返回值：无
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
void  OSTaskStatHook (void) {
}
#endif

/*********************************************************************************************************
 * 说明：*OSTaskStkInit函数，初始化任务堆栈。该函数由OSTaskCreate() 或 OSTaskCreateExt() 调用。
 * 参数：task  为指向任务代码的指针。
 *       p_arg 为指向用户数据区的指针，用户数据当任务首次执行时传递给任务。
 *       ptos  为指向栈顶的指针，假定ptos指向任务堆栈的'free'入口。如果 OS_STK_GROWTH 被置为1，则ptos将包
               含任务的最高有效地址。类似地，如果OS_STK_GROWTH 被置为0，则ptos将包含任务的最低有效地址。
 *       opt   指定用于改变OSTaskStkInit()作用的选项。
 * 返回值：一旦处理器的寄存器以合适的顺序放入堆栈，将返回新的栈顶值。
 * 注意： 1) 任务开始执行后，中断开放，
*         2) 所有任务以管理模式运行。
*********************************************************************************************************/
OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt) {
    OS_STK *stk;
    opt      = opt;                         // 参数opt未使用
    stk      = ptos;                        // 加载堆栈指针
    *(stk)   = (OS_STK)task;                // 入口
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
    *(--stk) = (INT32U)ARM_SVC_MODE;        // CPSR  (允许IRQ和FIQ中断)
    return (stk);
}

/*********************************************************************************************************
 * 说明：OSTaskSwHook函数，任务切换时调用本函数。允许用户在上下文切换期间执行其他操作。
 * 参数：无
 * 注意：1)函数调用期间要关中断。
 *       2)假定全局指针OSTCBHighRdy指向将被切换近来的最高优先级任务的TCB，并且OSTCBCur指向将被切换
 *         出去的任务。
*********************************************************************************************************/
#if (OS_CPU_HOOKS_EN > 0) && (OS_TASK_SW_HOOK_EN > 0)
void  OSTaskSwHook (void) {
#if OS_VIEW_MODULE > 0
    OSView_TaskSwHook();
#endif
}
#endif

/*********************************************************************************************************
 * 说明：OSTCBInitHook函数，建立起大部分TCB之后由OS_TCBInit()调用本函数。
 * 参数：ptcb 为指向将被创建任务TCB的指针。
 * 注意：函数调用期间可以开中断，也可以关中断。
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb) {
    (void)ptcb;
}
#endif

/*********************************************************************************************************
 * 说明：OSTimeTickHook函数，每个时钟节拍调用一次本函数。
 * 参数：无
 * 注意：函数调用期间可以开中断，也可以关中断。
*********************************************************************************************************/
#if (OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)
void  OSTimeTickHook (void) {
#if OS_VIEW_MODULE > 0
    OSView_TickHook();
#endif
}
#endif

/*********************************************************************************************************
 * 说明：OS_CPU_IntDisMeasInit函数。
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
