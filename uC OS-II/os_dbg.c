
#include <ucos_ii.h>
#define  OS_COMPILER_OPT  __root

/*********************************************************************************************************
*                                             ��������
*********************************************************************************************************/
OS_COMPILER_OPT  INT16U  const  OSDebugEn          = OS_DEBUG_EN;                // ���涨����Գ���
#if OS_DEBUG_EN > 0
OS_COMPILER_OPT  INT32U  const  OSEndiannessTest   = 0x12345678L;                // ����CPU��С�˷�ʽ�ı���
OS_COMPILER_OPT  INT16U  const  OSEventMax         = OS_MAX_EVENTS;              // �¼����ƿ���
OS_COMPILER_OPT  INT16U  const  OSEventNameSize    = OS_EVENT_NAME_SIZE;         // �¼�����С(�ֽ�)
OS_COMPILER_OPT  INT16U  const  OSEventEn          = OS_EVENT_EN;
#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)
OS_COMPILER_OPT  INT16U  const  OSEventSize        = sizeof(OS_EVENT);           // OS_EVENT�Ĵ�С(�ֽ�)
OS_COMPILER_OPT  INT16U  const  OSEventTblSize     = sizeof(OSEventTbl);         // OSEventTbl[]�Ĵ�С(�ֽ�)
#else
OS_COMPILER_OPT  INT16U  const  OSEventSize        = 0;
OS_COMPILER_OPT  INT16U  const  OSEventTblSize     = 0;
#endif

OS_COMPILER_OPT  INT16U  const  OSFlagEn           = OS_FLAG_EN;
#if (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
OS_COMPILER_OPT  INT16U  const  OSFlagGrpSize      = sizeof(OS_FLAG_GRP);        // OS_FLAG_GRP�Ĵ�С(�ֽ�)
OS_COMPILER_OPT  INT16U  const  OSFlagNodeSize     = sizeof(OS_FLAG_NODE);       // OS_FLAG_NODE�Ĵ�С(�ֽ�)
OS_COMPILER_OPT  INT16U  const  OSFlagWidth        = sizeof(OS_FLAGS);           // OS_FLAGS�Ŀ��(�ֽ�)
#else
OS_COMPILER_OPT  INT16U  const  OSFlagGrpSize      = 0;
OS_COMPILER_OPT  INT16U  const  OSFlagNodeSize     = 0;
OS_COMPILER_OPT  INT16U  const  OSFlagWidth        = 0;
#endif
OS_COMPILER_OPT  INT16U  const  OSFlagMax          = OS_MAX_FLAGS;
OS_COMPILER_OPT  INT16U  const  OSFlagNameSize     = OS_FLAG_NAME_SIZE;          // ��־���Ĵ�С(�ֽ�)

OS_COMPILER_OPT  INT16U  const  OSLowestPrio       = OS_LOWEST_PRIO;

OS_COMPILER_OPT  INT16U  const  OSMboxEn           = OS_MBOX_EN;

OS_COMPILER_OPT  INT16U  const  OSMemEn            = OS_MEM_EN;
OS_COMPILER_OPT  INT16U  const  OSMemMax           = OS_MAX_MEM_PART;            // �洢��������
OS_COMPILER_OPT  INT16U  const  OSMemNameSize      = OS_MEM_NAME_SIZE;           // �������Ĵ�С(�ֽ�)
#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
OS_COMPILER_OPT  INT16U  const  OSMemSize          = sizeof(OS_MEM);             // �洢������ͷ�Ĵ�С(�ֽ�)
OS_COMPILER_OPT  INT16U  const  OSMemTblSize       = sizeof(OSMemTbl);
#else
OS_COMPILER_OPT  INT16U  const  OSMemSize          = 0;
OS_COMPILER_OPT  INT16U  const  OSMemTblSize       = 0;
#endif
OS_COMPILER_OPT  INT16U  const  OSMutexEn          = OS_MUTEX_EN;

OS_COMPILER_OPT  INT16U  const  OSPtrSize          = sizeof(void *);             // ָ��Ĵ�С(�ֽ�)
OS_COMPILER_OPT  INT16U  const  OSQEn              = OS_Q_EN;
OS_COMPILER_OPT  INT16U  const  OSQMax             = OS_MAX_QS;                  // ������
#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)
OS_COMPILER_OPT  INT16U  const  OSQSize            = sizeof(OS_Q);               // OS_Q�ṹ��Ĵ�С(�ֽ�)
#else
OS_COMPILER_OPT  INT16U  const  OSQSize            = 0;
#endif

OS_COMPILER_OPT  INT16U  const  OSRdyTblSize       = OS_RDY_TBL_SIZE;            // ��������ֽ���

OS_COMPILER_OPT  INT16U  const  OSSemEn            = OS_SEM_EN;

OS_COMPILER_OPT  INT16U  const  OSStkWidth         = sizeof(OS_STK);             // ��ջ��ڵĴ�С(�ֽ�)

OS_COMPILER_OPT  INT16U  const  OSTaskCreateEn     = OS_TASK_CREATE_EN;
OS_COMPILER_OPT  INT16U  const  OSTaskCreateExtEn  = OS_TASK_CREATE_EXT_EN;
OS_COMPILER_OPT  INT16U  const  OSTaskDelEn        = OS_TASK_DEL_EN;
OS_COMPILER_OPT  INT16U  const  OSTaskIdleStkSize  = OS_TASK_IDLE_STK_SIZE;
OS_COMPILER_OPT  INT16U  const  OSTaskProfileEn    = OS_TASK_PROFILE_EN;
OS_COMPILER_OPT  INT16U  const  OSTaskMax          = OS_MAX_TASKS + OS_N_SYS_TASKS;  // ���������
OS_COMPILER_OPT  INT16U  const  OSTaskNameSize     = OS_TASK_NAME_SIZE;              // �������Ĵ�С(�ֽ�)
OS_COMPILER_OPT  INT16U  const  OSTaskStatEn       = OS_TASK_STAT_EN;
OS_COMPILER_OPT  INT16U  const  OSTaskStatStkSize  = OS_TASK_STAT_STK_SIZE;
OS_COMPILER_OPT  INT16U  const  OSTaskStatStkChkEn = OS_TASK_STAT_STK_CHK_EN;
OS_COMPILER_OPT  INT16U  const  OSTaskSwHookEn     = OS_TASK_SW_HOOK_EN;

OS_COMPILER_OPT  INT16U  const  OSTCBPrioTblMax    = OS_LOWEST_PRIO + 1;             // OSTCBPrioTbl[]�������
OS_COMPILER_OPT  INT16U  const  OSTCBSize          = sizeof(OS_TCB);                 // OS_TCB�Ĵ�С(�ֽ�)
OS_COMPILER_OPT  INT16U  const  OSTicksPerSec      = OS_TICKS_PER_SEC;
OS_COMPILER_OPT  INT16U  const  OSTimeTickHookEn   = OS_TIME_TICK_HOOK_EN;
OS_COMPILER_OPT  INT16U  const  OSVersionNbr       = OS_VERSION;

#endif

/*$PAGE*/
/*********************************************************************************************************
*                                             ��������
*                                 uC/OS-II��ʹ�õ�������(RAM)�ռ�
*********************************************************************************************************/
#if OS_DEBUG_EN > 0
OS_COMPILER_OPT  INT16U  const  OSDataSize = sizeof(OSCtxSwCtr)
#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)
                                           + sizeof(OSEventFreeList)
                                           + sizeof(OSEventTbl)
#endif
#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
                                           + sizeof(OSFlagTbl)
                                           + sizeof(OSFlagFreeList)
#endif
#if OS_TASK_STAT_EN > 0
                                           + sizeof(OSCPUUsage)
                                           + sizeof(OSIdleCtrMax)
                                           + sizeof(OSIdleCtrRun)
                                           + sizeof(OSStatRdy)
                                           + sizeof(OSTaskStatStk)
#endif
#if OS_TICK_STEP_EN > 0
                                           + sizeof(OSTickStepState)
#endif
#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
                                           + sizeof(OSMemFreeList)
                                           + sizeof(OSMemTbl)
#endif
#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)
                                           + sizeof(OSQFreeList)
                                           + sizeof(OSQTbl)
#endif
#if OS_TIME_GET_SET_EN > 0
                                           + sizeof(OSTime)
#endif
                                           + sizeof(OSIntNesting)
                                           + sizeof(OSLockNesting)
                                           + sizeof(OSPrioCur)
                                           + sizeof(OSPrioHighRdy)
                                           + sizeof(OSRdyGrp)
                                           + sizeof(OSRdyTbl)
                                           + sizeof(OSRunning)
                                           + sizeof(OSTaskCtr)
                                           + sizeof(OSIdleCtr)
                                           + sizeof(OSTaskIdleStk)
                                           + sizeof(OSTCBCur)
                                           + sizeof(OSTCBFreeList)
                                           + sizeof(OSTCBHighRdy)
                                           + sizeof(OSTCBList)
                                           + sizeof(OSTCBPrioTbl)
                                           + sizeof(OSTCBTbl);

#endif

/*$PAGE*/
/*********************************************************************************************************
 * ˵����OSDebugInit���������ڱ�֤Ӧ�ó���δʹ�õĵ��Ա���û�б��Ż�������Щ����������Ҫ����������ʱ����
 *       ɾ������룬����������������
 * ��������
 * ����ֵ����
*********************************************************************************************************/
#if OS_VERSION >= 270 && OS_DEBUG_EN > 0
void  OSDebugInit (void) {
    void  *ptemp;
    ptemp = (void *)&OSDebugEn;
    ptemp = (void *)&OSEndiannessTest;
    ptemp = (void *)&OSEventMax;
    ptemp = (void *)&OSEventNameSize;
    ptemp = (void *)&OSEventEn;
    ptemp = (void *)&OSEventSize;
    ptemp = (void *)&OSEventTblSize;
    ptemp = (void *)&OSFlagEn;
    ptemp = (void *)&OSFlagGrpSize;
    ptemp = (void *)&OSFlagNodeSize;
    ptemp = (void *)&OSFlagWidth;
    ptemp = (void *)&OSFlagMax;
    ptemp = (void *)&OSFlagNameSize;
    ptemp = (void *)&OSLowestPrio;
    ptemp = (void *)&OSMboxEn;
    ptemp = (void *)&OSMemEn;
    ptemp = (void *)&OSMemMax;
    ptemp = (void *)&OSMemNameSize;
    ptemp = (void *)&OSMemSize;
    ptemp = (void *)&OSMemTblSize;
    ptemp = (void *)&OSMutexEn;
    ptemp = (void *)&OSPtrSize;
    ptemp = (void *)&OSQEn;
    ptemp = (void *)&OSQMax;
    ptemp = (void *)&OSQSize;
    ptemp = (void *)&OSRdyTblSize;
    ptemp = (void *)&OSSemEn;
    ptemp = (void *)&OSStkWidth;
    ptemp = (void *)&OSTaskCreateEn;
    ptemp = (void *)&OSTaskCreateExtEn;
    ptemp = (void *)&OSTaskDelEn;
    ptemp = (void *)&OSTaskIdleStkSize;
    ptemp = (void *)&OSTaskProfileEn;
    ptemp = (void *)&OSTaskMax;
    ptemp = (void *)&OSTaskNameSize;
    ptemp = (void *)&OSTaskStatEn;
    ptemp = (void *)&OSTaskStatStkSize;
    ptemp = (void *)&OSTaskStatStkChkEn;
    ptemp = (void *)&OSTaskSwHookEn;
    ptemp = (void *)&OSTCBPrioTblMax;
    ptemp = (void *)&OSTCBSize;
    ptemp = (void *)&OSTicksPerSec;
    ptemp = (void *)&OSTimeTickHookEn;
    ptemp = (void *)&OSVersionNbr;
    ptemp = (void *)&OSDataSize;
    ptemp = ptemp;
}
#endif
