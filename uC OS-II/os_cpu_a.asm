// os_cpu.asm

;
;由两个ASM文件合并过来（Startup.s和os_cpu_a.asm）保持main前的上电初始化和IRQ入口
;
;* ----------------------------------------------------------------------------
;*         ATMEL Microcontroller Software Support  -  ROUSSET  -
;* ----------------------------------------------------------------------------
;* Copyright (c) 2006, Atmel Corporation
;
;* All rights reserved.
;*
;* Redistribution and use in source and binary forms, with or without
;* modification, are permitted provided that the following conditions are met:
;*
;* - Redistributions of source code must retain the above copyright notice,
;* this list of conditions and the disclaimer below.
;*
;* - Redistributions in binary form must reproduce the above copyright notice,
;* this list of conditions and the disclaimer below in the documentation and/or
;* other materials provided with the distribution.
;*
;* Atmel's name may not be used to endorse or promote products derived from
;* this software without specific prior written permission.
;*
;* DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
;* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
;* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
;* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
;* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
;* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
;* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
;* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
;* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;* ----------------------------------------------------------------------------
;* 无此文件会使看门狗不停的重启，开机16秒后自动复位
;*因为TUMB和ARM模式运行的代码有差异，要求将处理器调整到ARM模式
;* 在进行AT91F_LowLevelInit初始化是加延时，否则间断很久后上电会使CPU处于不稳定状态
;* 
;------------------------------------------------------------------------------
; Include your AT91 Library files
;------------------------------------------------------------------------------
#include "AT91SAM7S64_inc.h"
;------------------------------------------------------------------------------

#define TOP_OF_MEMORY    (AT91C_ISRAM + AT91C_ISRAM_SIZE)
#define IRQ_STACK_SIZE   (3*8*4)
     ; 3 words to be saved per interrupt priority level

; Mode, correspords to bits 0-5 in CPSR
MODE_BITS DEFINE  0x1F    ; Bit mask for mode bits in CPSR
USR_MODE  DEFINE  0x10    ; User mode
FIQ_MODE  DEFINE  0x11    ; Fast Interrupt Request mode
IRQ_MODE  DEFINE  0x12    ; Interrupt Request mode
SVC_MODE  DEFINE  0x13    ; Supervisor mode
ABT_MODE  DEFINE  0x17    ; Abort mode
UND_MODE  DEFINE  0x1B    ; Undefined Instruction mode
SYS_MODE  DEFINE  0x1F    ; System mode

I_BIT     DEFINE  0x80
F_BIT     DEFINE  0x40


            EXTERN  OSRunning                    ; 外部参考
            EXTERN  OSPrioCur
            EXTERN  OSPrioHighRdy
            EXTERN  OSTCBCur
            EXTERN  OSTCBHighRdy
            EXTERN  OSIntNesting
            EXTERN  OSIntExit
            EXTERN  OSTaskSwHook
            EXTERN  OS_CPU_IRQ_ISR_Handler
            EXTERN  OS_CPU_FIQ_ISR_Handler
            EXTERN  IRQ_handler_exit

            PUBLIC  OS_CPU_SR_Save               ; 本文件中定义的函数
            PUBLIC  OS_CPU_SR_Restore
            PUBLIC  OSStartHighRdy
            PUBLIC  OSCtxSw
            PUBLIC  OSIntCtxSw
            PUBLIC  OS_CPU_IRQ_ISR
            PUBLIC  OS_CPU_FIQ_ISR


NO_INT      EQU     0xC0                         ; 用于禁止FIQ和IRQ中断的屏蔽码
SVC32_MODE  EQU     0x13
FIQ32_MODE  EQU     0x11
IRQ32_MODE  EQU     0x12


;------------------------------------------------------------------------------
; ?RESET
; Reset Vector.
; Normally, segment INTVEC is linked at address 0.
; For debugging purposes, INTVEC may be placed at other addresses.
; A debugger that honors the entry point will start the
; program in a normal way even if INTVEC is not at address 0.
;------------------------------------------------------------------------------
        SECTION .intvec:CODE:NOROOT(2)
        PUBLIC  __vector
        PUBLIC  __iar_program_start

        ARM
__vector:
        ldr  pc,[pc,#+24]             ;; Reset

__und_handler:
        ldr  pc,[pc,#+24]             ;; Undefined instructions
__swi_handler:
        ldr  pc,[pc,#+24]             ;; Software interrupt (SWI/SVC)
__prefetch_handler:
        ldr  pc,[pc,#+24]             ;; Prefetch abort
__data_handler:
        ldr  pc,[pc,#+24]             ;; Data abort
        DC32  0xFFFFFFFF              ;; RESERVED
__irq_handler:
        ldr  pc,[pc,#+24]             ;; IRQ
__fiq_handler:
        ldr  pc,[pc,#+24]             ;; FIQ

        DC32  __iar_program_start
        DC32  __und_handler
        DC32  __swi_handler
        DC32  __prefetch_handler
        DC32  __data_handler
        B .
        DC32  IRQ_Handler_Entry
        DC32  FIQ_Handler_Entry


;*********************************************************************************************************
;                                         方式3函数的临界部分
; 说明：通过保存中断状态来禁止/允许中断。一般来说应该将中断禁止标志保存在变量cpu_sr中，然后禁止中断。
;       将cpu_sr复制回CPU的状态寄存器即可恢复中断禁止状态。
;
; 原型：OS_CPU_SR  OS_CPU_SR_Save(void);
;       void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
;
; 注意：：1) 函数一般使用方法容下：
;                 void Task (void *p_arg) {
;                 #if OS_CRITICAL_METHOD == 3          // 分配CPU状态寄存器存储单元
;                     OS_CPU_SR  cpu_sr;
;                 #endif
;                          :
;                          :
;                     OS_ENTER_CRITICAL();             // cpu_sr = OS_CPU_SaveSR();
;                          :
;                          :
;                     OS_EXIT_CRITICAL();              // OS_CPU_RestoreSR(cpu_sr);
;                          :
;                          :
;                 }
;
;              2) OS_CPU_SaveSR()                      // 禁止中断
;*********************************************************************************************************
        RSEG CODE:CODE:NOROOT(2)
        CODE32

OS_CPU_SR_Save
        MRS     R0,CPSR                     ; 设置CPSR中的IRQFIQ位以禁止所有中断
        ORR     R1,R0,#NO_INT
        MSR     CPSR_c,R1
        MRS     R1,CPSR                     ; 确认CPSR中包含了合适的中断禁止标志
        AND     R1,R1,#NO_INT
        CMP     R1,#NO_INT
        BNE     OS_CPU_SR_Save              ; 没有恰当禁止，重试
        BX      LR                          ; 已禁止

OS_CPU_SR_Restore
        MSR     CPSR_c,R0
        BX      LR

;*********************************************************************************************************
;                                            启动多任务
; 原型：void OSStartHighRdy(void)
;
; 注意：1) OSStartHighRdy()函数必须：
;              a) 调用 OSTaskSwHook()，
;              b) 然后将 OSRunning 设置为 TRUE,
;              c) 切换到最高优先级任务
;*********************************************************************************************************
        RSEG CODE:CODE:NOROOT(2)
        CODE32

OSStartHighRdy
        LDR     R0, ??OS_TaskSwHook     ; OSTaskSwHook();
        MOV     LR, PC
        BX      R0
        MSR     CPSR_cxsf, #0xD3        ; 切换到管理模式，禁止 IRQ 和 FIQ
        LDR     R4, ??OS_Running        ; OSRunning = TRUE
        MOV     R5, #1
        STRB    R5, [R4]
                                        ; 切换到最高优先级任务
        LDR     R4, ??OS_TCBHighRdy     ; 获得最高优先级任务的TCB地址
        LDR     R4, [R4]                ; 获得堆栈指针
        LDR     SP, [R4]                ; 切换到新堆栈

        LDR     R4,  [SP], #4           ; 弹出新任务的 CPSR
        MSR     SPSR_cxsf,R4
        LDMFD   SP!, {R0-R12,LR,PC}^    ; 弹出新任务的上下文(context)

;*********************************************************************************************************
;                                     执行任务切换(任务级) - OSCtxSw()
;
; 注意：1) OSCtxSw() 应在禁止FIQ和IRQ中断的条件下以系统模式调用
;
;       2) OSCtxSw() 的伪代码如下:
;              a) 将当前任务的上下文(context)保存到当前任务的堆栈中
;              b) OSTCBCur->OSTCBStkPtr = SP;
;              c) OSTaskSwHook();
;              d) OSPrioCur             = OSPrioHighRdy;
;              e) OSTCBCur              = OSTCBHighRdy;
;              f) SP                    = OSTCBHighRdy->OSTCBStkPtr;
;              g) 从新任务的堆栈中恢复新任务的上下文(context)
;              h) 返回到新任务的代码中
;
;           3) 入口：
;              OSTCBCur      指向要挂起任务的 OS_TCB
;              OSTCBHighRdy  指向要恢复任务的 OS_TCB
;*********************************************************************************************************
        RSEG CODE:CODE:NOROOT(2)
        CODE32
OSCtxSw
                                        ; 保存当前任务的上下文(CONTEXT)
        STMFD   SP!, {LR}               ; 当前地址压栈
        STMFD   SP!, {LR}
        STMFD   SP!, {R0-R12}           ; 寄存器压栈
        MRS     R4,  CPSR               ; 当前CPSR压栈
        TST     LR, #1                  ; 测试是否从Thumb模式下调用
        ORRNE   R4,  R4, #0x20          ; 是，置1 T 标志
        STMFD   SP!, {R4}

        LDR     R4, ??OS_TCBCur         ; OSTCBCur->OSTCBStkPtr = SP;
        LDR     R5, [R4]
        STR     SP, [R5]

        LDR     R0, ??OS_TaskSwHook     ; OSTaskSwHook();
        MOV     LR, PC
        BX      R0

        LDR     R4, ??OS_PrioCur        ; OSPrioCur = OSPrioHighRdy
        LDR     R5, ??OS_PrioHighRdy
        LDRB    R6, [R5]
        STRB    R6, [R4]

        LDR     R4, ??OS_TCBCur         ; OSTCBCur  = OSTCBHighRdy;
        LDR     R6, ??OS_TCBHighRdy
        LDR     R6, [R6]
        STR     R6, [R4]

        LDR     SP, [R6]                ; SP = OSTCBHighRdy->OSTCBStkPtr;

                                        ; 恢复新任务的上下文(CONTEXT)
        LDMFD   SP!, {R4}               ; 新任务的CPSR出栈
        MSR     SPSR_cxsf, R4

        LDMFD   SP!, {R0-R12,LR,PC}^    ; 新任务的下文(CONTEXT)出栈

;*********************************************************************************************************
;                                执行任务切换(中断级) - OSIntCtxSw()
;
; 注意： 1) OSIntCtxSw()  应在禁止FIQ和IRQ中断的条件下以系统模式调用
;
;        2) OSCtxSw() 的伪代码如下:
;              a) OSTaskSwHook();
;              b) OSPrioCur             = OSPrioHighRdy;
;              c) OSTCBCur              = OSTCBHighRdy;
;              d) SP                    = OSTCBHighRdy->OSTCBStkPtr;
;              e) 从新任务的堆栈中恢复新任务的上下文(context)
;              f) 返回到新任务的代码中
;
;           3) 入口：
;              OSTCBCur      指向要挂起任务的 OS_TCB
;              OSTCBHighRdy  指向要恢复任务的 OS_TCB
;*********************************************************************************************************
        RSEG CODE:CODE:NOROOT(2)
        CODE32
OSIntCtxSw
        LDR     R0, ??OS_TaskSwHook     ; OSTaskSwHook();
        MOV     LR, PC
        BX      R0

        LDR     R4,??OS_PrioCur         ; OSPrioCur = OSPrioHighRdy
        LDR     R5,??OS_PrioHighRdy
        LDRB    R6,[R5]
        STRB    R6,[R4]

        LDR     R4,??OS_TCBCur          ; OSTCBCur  = OSTCBHighRdy;
        LDR     R6,??OS_TCBHighRdy
        LDR     R6,[R6]
        STR     R6,[R4]

        LDR     SP,[R6]                 ; SP = OSTCBHighRdy->OSTCBStkPtr;

                                        ; 恢复新任务的上下文(CONTEXT)
        LDMFD   SP!, {R4}               ; 新任务的CPSR出栈
        MSR     SPSR_cxsf, R4

        LDMFD   SP!, {R0-R12,LR,PC}^    ; 新任务的上下文(context)出栈





;------------------------------------------------------------------------------
;- Manage exception: The exception must be ensure in ARM mode
;------------------------------------------------------------------------------
        SECTION text:CODE:NOROOT(2)
        ARM
;------------------------------------------------------------------------------
;- Function             : FIQ_Handler_Entry
;- Treatments           : FIQ Controller Interrupt Handler.
;- Called Functions     : AIC_FVR[interrupt]
;------------------------------------------------------------------------------
        DATA
??OS_TaskSwHook:
        DC32    OSTaskSwHook
??OS_CPU_IRQ_ISR_Handler:
        DC32    OS_CPU_IRQ_ISR_Handler
??IRQ_handler_exit:
        DC32    IRQ_handler_exit  
??OS_CPU_FIQ_ISR_Handler:
        DC32    OS_CPU_FIQ_ISR_Handler
??OS_IntExit:
        DC32    OSIntExit
??OS_IntNesting:
        DC32    OSIntNesting
??OS_PrioCur:
        DC32    OSPrioCur
??OS_PrioHighRdy:
        DC32    OSPrioHighRdy
??OS_Running:
        DC32    OSRunning
??OS_TCBCur:
        DC32    OSTCBCur
??OS_TCBHighRdy:
        DC32    OSTCBHighRdy

FIQ_Handler_Entry:
OS_CPU_FIQ_ISR

        STMFD   SP!, {R1-R3}                   ; 工作寄存器压入FIQ堆栈
        MOV     R1, SP                         ; 保存FIQ堆栈指针
        ADD     SP, SP,#12                     ; 调整FIQ堆栈指针
        SUB     R2, LR,#4                      ; 调整PC返回地址

        MRS     R3, SPSR                       ; 将SPSR(即中断任务的CPSR)复制到R3
        MSR     CPSR_c, #(NO_INT | SVC32_MODE) ; 切换到管理模式

                                               ; 将任务的上下文(CONTEXT)保存到任务堆栈
        STMFD   SP!, {R2}                      ; 任务的返回地址压栈
        STMFD   SP!, {LR}                      ; 任务的LR压栈
        STMFD   SP!, {R4-R12}                  ; 任务的R12-R4压栈
        LDMFD   R1!, {R4-R6}                   ; 将任务的R1-R3从IRQ堆栈移动到管理堆栈
        STMFD   SP!, {R4-R6}
        STMFD   SP!, {R0}                      ; 任务的R0压入任务堆栈
        STMFD   SP!, {R3}                      ; 任务的CPSR(即FIQ's SPSR)压栈

                                               ; 处理嵌套计数器
        LDR     R0, ??OS_IntNesting            ; OSIntNesting++;
        LDRB    R1, [R0]
        ADD     R1, R1,#1
        STRB    R1, [R0]

        CMP     R1, #1                         ; if (OSIntNesting == 1) {
        BNE     OS_CPU_FIQ_ISR_1
        LDR     R4, ??OS_TCBCur                ;     OSTCBCur->OSTCBStkPtr = SP
        LDR     R5, [R4]
        STR     SP, [R5]                       ; }

OS_CPU_FIQ_ISR_1
        MSR     CPSR_c, #(NO_INT | FIQ32_MODE) ; 切换到FIQ模式(使用FIQ堆栈处理中断)
        LDR     R0, ??OS_CPU_FIQ_ISR_Handler   ; OS_CPU_FIQ_ISR_Handler();
        MOV     LR, PC
        BX      R0

        MSR     CPSR_c, #(NO_INT | SVC32_MODE) ; 切换到管理模式
        LDR     R0, ??OS_IntExit               ; OSIntExit();
        MOV     LR, PC
        BX      R0

                                               ; 恢复新任务的上下文(CONTEXT)
        LDMFD   SP!, {R4}                      ; 新任务的CPSR出栈
        MSR     SPSR_cxsf, R4
        LDMFD   SP!, {R0-R12,LR,PC}^           ; 新任务的上下文(context)出栈



;------------------------------------------------------------------------------
;- Exception Vectors
;------------------------------------------------------------------------------
    PUBLIC    AT91F_Default_FIQ_handler
    PUBLIC    AT91F_Default_IRQ_handler
    PUBLIC    AT91F_Spurious_handler

    ARM      ; Always ARM mode after exeption

AT91F_Default_FIQ_handler
    b         AT91F_Default_FIQ_handler

AT91F_Default_IRQ_handler
    b         AT91F_Default_IRQ_handler

AT91F_Spurious_handler
    b         AT91F_Spurious_handler



;------------------------------------------------------------------------------
; ?INIT
; Program entry.
;------------------------------------------------------------------------------

    SECTION FIQ_STACK:DATA:NOROOT(3)
    SECTION IRQ_STACK:DATA:NOROOT(3)
    SECTION SVC_STACK:DATA:NOROOT(3)
    SECTION ABT_STACK:DATA:NOROOT(3)
    SECTION UND_STACK:DATA:NOROOT(3)
    SECTION CSTACK:DATA:NOROOT(3)
    SECTION text:CODE:NOROOT(2)
    REQUIRE __vector
    EXTERN  ?main
    PUBLIC  __iar_program_start
    EXTERN  AT91F_LowLevelInit
;------------------------------------------------------------------------------
;- Function             : IRQ_Handler_Entry
;- Treatments           : IRQ Controller Interrupt Handler.
;- Called Functions     : AIC_IVR[interrupt]
;------------------------------------------------------------------------------

IRQ_Handler_Entry:
OS_CPU_IRQ_ISR:
;-------------------------
;- Manage Exception Entry
;-------------------------
;- Adjust and save LR_irq in IRQ stack


        STMFD   SP!, {R1-R3}                   ; 工作寄存器压入IRQ堆栈
        MOV     R1, SP                         ; 保存IRQ堆栈指针
        ADD     SP, SP,#12                     ; 调整IRQ堆栈指针
        SUB     R2, LR,#4                      ; 调整PC返回地址

        MRS     R3, SPSR                       ; 将SPSR(即中断任务的CPSR)复制到R3
        MSR     CPSR_c, #(NO_INT | SVC32_MODE) ; 切换到管理模式

                                               ; 将任务的上下文(CONTEXT)保存到任务堆栈
        STMFD   SP!, {R2}                      ; 任务的返回地址压栈
        STMFD   SP!, {LR}                      ; 任务的LR压栈
        STMFD   SP!, {R4-R12}                  ; 任务的R12-R4压栈

        LDMFD   R1!, {R4-R6}                   ; 将任务的R1-R3从IRQ堆栈移动到管理堆栈
        STMFD   SP!, {R4-R6}
        STMFD   SP!, {R0}                      ; 任务的R0压入任务堆栈
        STMFD   SP!, {R3}                      ; 任务的CPSR(即IRQ's SPSR)压栈

                                               ; 处理嵌套计数器
        LDR     R0, ??OS_IntNesting            ; OSIntNesting++;
        LDRB    R1, [R0]
        ADD     R1, R1,#1
        STRB    R1, [R0]

        CMP     R1, #1                         ; if (OSIntNesting == 1) {
        BNE     OS_CPU_IRQ_ISR_1
        LDR     R4, ??OS_TCBCur                ;   OSTCBCur->OSTCBStkPtr = SP
        LDR     R5, [R4]
        STR     SP, [R5]                       ; }

OS_CPU_IRQ_ISR_1
        MSR     CPSR_c, #(NO_INT | IRQ32_MODE) ; 切换到IRQ模式(使用IRQ堆栈处理中断)

        LDR     R0, ??OS_CPU_IRQ_ISR_Handler   ; OS_CPU_IRQ_ISR_Handler();
        MOV     LR, PC
        BX      R0

        MSR     CPSR_c, #(NO_INT | SVC32_MODE) ; 切换到管理模式

        LDR     R0, ??IRQ_handler_exit               ; IRQ_handler_exit();
        MOV     LR, PC
        BX      R0
        
        LDR     R0, ??OS_IntExit               ; OSIntExit();
        MOV     LR, PC
        BX      R0

                                               ; 恢复新任务的上下文(CONTEXT)
        LDMFD   SP!, {R4}                      ; 新任务的CPSR出栈
        MSR     SPSR_cxsf, R4

        LDMFD   SP!, {R0-R12,LR,PC}^           ; 新任务的上下文(context)出栈
        MOV     LR, PC

__iar_program_start:

;------------------------------------------------------------------------------
;- Low level Init is performed in a C function: AT91F_LowLevelInit
;- Init Stack Pointer to a valid memory area before calling AT91F_LowLevelInit
;------------------------------------------------------------------------------

;- Retrieve end of RAM address
;ADD DELAYS
#if 0
                mov     r0,#2000
DELAYS:         sub     r0,r0,#1
                cmp     r0,#0
                BNE     DELAYS


                ldr     r13,=TOP_OF_MEMORY          ;- Temporary stack in internal RAM for Low Level Init execution
                ldr     r0,=AT91F_LowLevelInit
                mov     lr, pc
                bx      r0                          ;- Branch on C function (with interworking)

#endif
; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.

                mrs     r0,cpsr                             ; Original PSR value
                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#SVC_MODE                     ; Set SVC mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(SVC_STACK)                  ; End of SVC_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#UND_MODE                     ; Set UND mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(UND_STACK)                  ; End of UND_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#ABT_MODE                     ; Set ABT mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(ABT_STACK)                  ; End of ABT_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#FIQ_MODE                     ; Set FIQ mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(FIQ_STACK)                  ; End of FIQ_STACK
                ;- Init the FIQ register
            	  ldr     r8, =AT91C_BASE_AIC

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#IRQ_MODE                     ; Set IRQ mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(IRQ_STACK)                  ; End of IRQ_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#SYS_MODE                     ; Set System mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(CSTACK)                     ; End of CSTACK

#ifdef __ARMVFP__
; Enable the VFP coprocessor.
                mov     r0, #0x40000000                 ; Set EN bit in VFP
                fmxr    fpexc, r0                       ; FPEXC, clear others.

; Disable underflow exceptions by setting flush to zero mode.
; For full IEEE 754 underflow compliance this code should be removed
; and the appropriate exception handler installed.
                mov     r0, #0x01000000		        ; Set FZ bit in VFP
                fmxr    fpscr, r0                       ; FPSCR, clear others.
#endif

; Add more initialization here


; Continue to ?main for more IAR specific system startup

                ldr     r0,=?main
                bx      r0


    END         ;- Terminates the assembly of the last module in a file

