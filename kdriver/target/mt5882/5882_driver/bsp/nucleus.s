;************************************************************************
;*
;*               Copyright Mentor Graphics Corporation 2002
;*                         All Rights Reserved.
;*
;* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS
;* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS
;* SUBJECT TO LICENSE TERMS.
;*
;************************************************************************

;************************************************************************
;*
;* FILE NAME                                  VERSION
;*
;*      int.s                         Nucleus PLUS\Integrator\ADS 1.14.8
;*
;* COMPONENT
;*
;*      IN - Initialization
;*
;* DESCRIPTION
;*
;*      This file contains the target processor dependent initialization
;*      routines and data.
;*
;* DATA STRUCTURES
;*
;*      INT_Vectors                         Interrupt vector table
;*
;* FUNCTIONS
;*
;*      INT_Initialize                      Target initialization
;*      INT_Vectors_Loaded                  Returns a NU_TRUE if all the
;*                                            default vectors are loaded
;*      INT_Setup_Vector                    Sets up an actual vector
;*      INT_Retrieve_Shell                  Returns pointer to shell ISR
;*      INT_Undef_Inst                      Undefined Instruction stub
;*      INT_Software                        Software Interrupt stub
;*      INT_Prefetch_Abort                  Prefetch Abort stub
;*      INT_Data_Abort                      Data Abort interrupt stub
;*      INT_Reserved                        Reserved interrupt stub
;*      INT_IRQ                             IRQ Handler
;*      INT_FIQ                             FIQ Stub
;*      INT_Interrupt_Shell                 LISR Handler
;*      INT_Timer_IRQ                       Timer IRQ handler
;*      INT_Timer_FIQ                       Timer FIQ handler
;*      INT_Install_Vector_Table            Sets up Plus ISRs and vectors
;*      INT_Timer_Initilize                 Board specific initialize
;*
;************************************************************************

;*************************************************************************
;*   Header files inclusion
;*************************************************************************

    ; Define constants used in low-level initialization.

    [ :LNOT::DEF:CC_MTK_LOADER
                    GBLL    CC_MTK_LOADER
CC_MTK_LOADER      SETL    {FALSE}
    ]

    [ :LNOT:CC_MTK_LOADER

    INCLUDE def5371.inc
    INCLUDE arm_cpu.inc

;**************************************
;*   COMPILE-TIME SWITCHES            *
;**************************************

    ; BOOT_MSG: Output messages to UART while booting

    [ :LNOT::DEF:BOOT_MSG
                    GBLL    BOOT_MSG
BOOT_MSG            SETL    {FALSE}
    ]

    [ :LNOT::DEF:BOOT_MSG_TRUE
                    GBLL    BOOT_MSG_TRUE
BOOT_MSG_TRUE       SETL    {TRUE}
    ]

    ; Build as an ARM module

    CODE32

;*************************************************************************
;*   BSP Declarations
;*************************************************************************

    ; Declaration of external functions

    IMPORT  INT_Vectors
    IMPORT  DramInit
    ;IMPORT  DramSizing
    IMPORT  BSP_SetMemSize
    IMPORT  BSP_SetChBMemSize
    IMPORT  __main
    IMPORT  BSP_DumpContext

    ; Declaration of external variables

    IMPORT  |Image$$BSS$$ZI$$Base|
    IMPORT  |Image$$BSS$$ZI$$Limit|
    IMPORT  |Image$$RO$$Limit|
    IMPORT  |Image$$RW$$Base|
    IMPORT  |Image$$RW$$Limit|

;*************************************************************************
;*   Nucleus Declarations
;*************************************************************************

    ; Define the global system stack variable.  This is setup by the
    ; initialization routine.

    IMPORT  TCD_System_Stack
    IMPORT  TCT_System_Limit

    ; Define the global data structures that need to be initialized by this
    ; routine.  These structures are used to define the system timer management
    ; HISR.

    IMPORT  TMD_HISR_Stack_Ptr
    IMPORT  TMD_HISR_Stack_Size
    IMPORT  TMD_HISR_Priority

    ; Define extern function references.

    IMPORT  INC_Initialize
    IMPORT  TCT_Interrupt_Context_Save
    IMPORT  TCT_Interrupt_Context_Restore
    IMPORT  TCC_Dispatch_LISR
    IMPORT  TMT_Timer_Interrupt

    AREA INT_DATA, DATA, READWRITE

;*************************************************************************
;*   Glocal Variable Declarations
;*************************************************************************

; Define the initialization flag that indicates whether or not all of the
; default vectors have been loaded during initialization.

    EXPORT  INT_Loaded_Flag
INT_Loaded_Flag
    DCD     &00000000


;*************************************************************************
;*   Interrupt Tables
;*************************************************************************

; Define vector table used by INT_IRQ to branch to necessary ISR

INT_IRQ_Vectors
    DCD     INT_Interrupt_Shell         ; Vector 0:  PDWNC
    DCD     INT_Interrupt_Shell         ; Vector 1:  Serial interface
    DCD     INT_Interrupt_Shell         ; Vector 2:  NAND Flash
    DCD     INT_Timer_IRQ               ; Vector 3:  Timer 0
    DCD     INT_Interrupt_Shell         ; Vector 4:  Timer 1
    DCD     INT_Interrupt_Shell         ; Vector 5:  Timer 2
    DCD     INT_Interrupt_Shell         ; Vector 6:  Smart Card
    DCD     INT_Interrupt_Shell         ; Vector 7:  WDT
    DCD     INT_Interrupt_Shell         ; Vector 8:  Internal Ethernet MAC
    DCD     INT_Interrupt_Shell         ; Vector 9:  Ethernet DTCP
    DCD     INT_Interrupt_Shell         ; Vector 10: PSCAN
    DCD     INT_Interrupt_Shell         ; Vector 11: N/A
    DCD     INT_Interrupt_Shell         ; Vector 12: IMGRZ
    DCD     INT_Interrupt_Shell         ; Vector 13: GDMA
    DCD     INT_Interrupt_Shell         ; Vector 14: Audio: SPDIF/PSR
    DCD     INT_Interrupt_Shell         ; Vector 15: USB0
    DCD     INT_Interrupt_Shell         ; Vector 16: Audio DSP
    DCD     INT_Interrupt_Shell         ; Vector 17: RS232
    DCD     INT_Interrupt_Shell         ; Vector 18: N/A
    DCD     INT_Interrupt_Shell         ; Vector 19: OSD
    DCD     INT_Interrupt_Shell         ; Vector 20: VDOIN
    DCD     INT_Interrupt_Shell         ; Vector 21: B2R
    DCD     INT_Interrupt_Shell         ; Vector 22: Serial Flash
    DCD     INT_Interrupt_Shell         ; Vector 23: PP
    DCD     INT_Interrupt_Shell         ; Vector 24: VDEC
    DCD     INT_Interrupt_Shell         ; Vector 25: Graphic
    DCD     INT_Interrupt_Shell         ; Vector 26: Demuxer
    DCD     INT_Interrupt_Shell         ; Vector 27: Demod
    DCD     INT_Interrupt_Shell         ; Vector 28: FCI
    DCD     INT_Interrupt_Shell         ; Vector 29: POD
    DCD     INT_Interrupt_Shell         ; Vector 30: MISC2
    DCD     INT_Interrupt_Shell         ; Vector 31: MISC

; Define vector table used by INT_FIQ to branch to necessary ISR

INT_FIQ_Vectors
    DCD     INT_FIQ_Shell               ; Vector 0:  PDWNC
    DCD     INT_FIQ_Shell               ; Vector 1:  Serial interface
    DCD     INT_FIQ_Shell               ; Vector 2:  NAND Flash
    DCD     INT_Timer_FIQ               ; Vector 3:  Timer 0
    DCD     INT_FIQ_Shell               ; Vector 4:  Timer 1
    DCD     INT_FIQ_Shell               ; Vector 5:  Timer 2
    DCD     INT_FIQ_Shell               ; Vector 6:  Smart Card
    DCD     INT_FIQ_Shell               ; Vector 7:  WDT
    DCD     INT_FIQ_Shell               ; Vector 8:  Internal Ethernet MAC
    DCD     INT_FIQ_Shell               ; Vector 9:  Ethernet DTCP
    DCD     INT_FIQ_Shell               ; Vector 10: PSCAN
    DCD     INT_FIQ_Shell               ; Vector 11: N/A
    DCD     INT_FIQ_Shell               ; Vector 12: IMGRZ
    DCD     INT_FIQ_Shell               ; Vector 13: GDMA
    DCD     INT_FIQ_Shell               ; Vector 14: Audio: SPDIF/PSR
    DCD     INT_FIQ_Shell               ; Vector 15: USB0
    DCD     INT_FIQ_Shell               ; Vector 16: Audio DSP
    DCD     INT_FIQ_Shell               ; Vector 17: RS232
    DCD     INT_FIQ_Shell               ; Vector 18: N/A
    DCD     INT_FIQ_Shell               ; Vector 19: OSD
    DCD     INT_FIQ_Shell               ; Vector 20: VDOIN
    DCD     INT_FIQ_Shell               ; Vector 21: B2R
    DCD     INT_FIQ_Shell               ; Vector 22: Serial Flash
    DCD     INT_FIQ_Shell               ; Vector 23: PP
    DCD     INT_FIQ_Shell               ; Vector 24: VDEC
    DCD     INT_FIQ_Shell               ; Vector 25: Graphic
    DCD     INT_FIQ_Shell               ; Vector 26: Demuxer
    DCD     INT_FIQ_Shell               ; Vector 27: Demod
    DCD     INT_FIQ_Shell               ; Vector 28: FCI
    DCD     INT_FIQ_Shell               ; Vector 29: POD
    DCD     INT_FIQ_Shell               ; Vector 30: MISC2
    DCD     INT_FIQ_Shell               ; Vector 31: MISC

; Define the order in which the interrupts will be executed by software (INT_IRQ)

INT_IRQ_Priority

    ; Interrupts are serviced in the listed order
    DCD     IRQ_T1I         ; 4, Special timer
    ; Video 11 interrupts
    DCD     IRQ_DMXRI       ; 26
    DCD     IRQ_GRAI        ; 25
    DCD     IRQ_VDECI       ; 24
    DCD     IRQ_PSCANI      ; 10
    DCD     IRQ_GDMAI       ; 13
    DCD     IRQ_B2RI        ; 21
    DCD     IRQ_VDOIN       ; 20
    DCD     IRQ_PPI         ; 23
    DCD     IRQ_DEMODI      ; 27
    DCD     IRQ_MISC2I        ; 30
    ; Audio 2 interrupts
    DCD     IRQ_AUDIOI      ; 14
    DCD     IRQ_DSPI        ; 16
    ; Nucleus timer
    DCD     IRQ_T0I         ; 3
    ; High speed Periperal 6 interrupts
    DCD     IRQ_EMACI       ; 8
    DCD     IRQ_DTCPI       ; 9
    DCD     IRQ_IMGRZI      ; 12
    DCD     IRQ_USB0I       ; 15
    DCD     IRQ_USB1I       ; 11
    DCD     IRQ_FCII        ; 28
    ; Low speed Periperal 7 interrupts
    DCD     IRQ_PDWNCI      ; 0
    DCD     IRQ_PODI        ; 29
    DCD     IRQ_OSDI        ; 19
    DCD     IRQ_RS232I      ; 17
    DCD     IRQ_SMCARDI     ; 6
    DCD     IRQ_NFII        ; 2
    DCD     IRQ_MISCI       ; 31
    ; Almost no use 3 interrupts
    DCD     IRQ_SNORI       ; 22
    DCD     IRQ_SIFI        ; 1
    DCD     IRQ_WDTI       ; 7
    ; no used, just for boot time. Last Interrupt serviced in INT_IRQ
    DCD     IRQ_T2I         ; 5

IRQ_PRIORITY_END

; Define the order in which the interrupts will be executed by software (INT_FIQ)

INT_FIQ_Priority

    ; Interrupts are serviced in the listed order
    DCD     IRQ_T1I         ; 4, Special timer
    ; Video 11 interrupts
    DCD     IRQ_DMXRI       ; 26
    DCD     IRQ_GRAI        ; 25
    DCD     IRQ_VDECI       ; 24
    DCD     IRQ_PSCANI      ; 10
    DCD     IRQ_GDMAI       ; 13
    DCD     IRQ_B2RI        ; 21
    DCD     IRQ_VDOIN       ; 20
    DCD     IRQ_PPI         ; 23
    DCD     IRQ_DEMODI      ; 27
    DCD     IRQ_MISC2I        ; 30
    ; Audio 2 interrupts
    DCD     IRQ_AUDIOI      ; 14
    DCD     IRQ_DSPI        ; 16
    ; Nucleus timer
    DCD     IRQ_T0I         ; 3
    ; High speed Periperal 6 interrupts
    DCD     IRQ_EMACI       ; 8
    DCD     IRQ_DTCPI       ; 9
    DCD     IRQ_IMGRZI      ; 12
    DCD     IRQ_USB0I       ; 15
    DCD     IRQ_USB1I       ; 11
    DCD     IRQ_FCII        ; 28
    ; Low speed Periperal 7 interrupts
    DCD     IRQ_PDWNCI      ; 0
    DCD     IRQ_PODI        ; 29
    DCD     IRQ_OSDI        ; 19
    DCD     IRQ_RS232I      ; 17
    DCD     IRQ_SMCARDI     ; 6
    DCD     IRQ_NFII        ; 2
    DCD     IRQ_MISCI       ; 31
    ; Almost no use 3 interrupts
    DCD     IRQ_SNORI       ; 22
    DCD     IRQ_SIFI        ; 1
    DCD     IRQ_WDTI       ; 7
    ; no used, just for boot time. Last Interrupt serviced in INT_IRQ
    DCD     IRQ_T2I         ; 5

FIQ_PRIORITY_END

;*************************************************************************
;*  Begin of code section                                                *
;*************************************************************************

    ; Code section declaration

    PRESERVE8
    AREA    INIT, CODE, READONLY
    CODE32

BSS_Start_Ptr
    IMPORT  |Image$$BSS$$ZI$$Base|
    DCD     |Image$$BSS$$ZI$$Base|

    EXPORT BSS_End_Ptr
BSS_End_Ptr
    IMPORT  |Image$$BSS$$ZI$$Limit|
    DCD     |Image$$BSS$$ZI$$Limit|

ROM_Data_Start_Ptr
    IMPORT  |Image$$RO$$Limit|
    DCD     |Image$$RO$$Limit|

RAM_Start_Ptr
    IMPORT  |Image$$RW$$Base|
    DCD     |Image$$RW$$Base|

Loaded_Flag
    DCD     INT_Loaded_Flag

Module_IRQ_Base
    DCD     IRQ_BASE

Module_FIQ_Base
    DCD     FIQ_BASE

;*****************************************************************************
; The following should be dealt with __main specially, since __main will zero
; the content that they point.

HISR_Stack_Ptr
    DCD     TMD_HISR_Stack_Ptr

HISR_Stack_Size
    DCD     TMD_HISR_Stack_Size

HISR_Priority
    DCD     TMD_HISR_Priority

System_Stack
    DCD     TCD_System_Stack

System_Limit
    DCD     TCT_System_Limit

;----------------------------------------------------------------------
; The followings are used to keep the content, to restore values after
; __main

HISR_Stack_Ptr_V
    DCD     0

HISR_Stack_Size_V
    DCD     0

HISR_Priority_V
    DCD     0

    EXPORT System_Stack_V
System_Stack_V
    DCD     0

    EXPORT System_Limit_V
System_Limit_V
    DCD     0

;*****************************************************************************
; UART register definitions

UART_BASE           EQU     0xf000c000
REG_STATUS          EQU     0x4
STATUS_WRITING      EQU     0x2
; REG_MERGE           EQU     0x14 ; Obsolet at MT5371
REG_RS232_MODE      EQU     0x18
REG_BUFFER_SIZE     EQU     0x1c
REG_OUTSEL          EQU     0x3c
REG_CTR             EQU     0x48
REG_U0_STATUS       EQU     0x4c
; REG_U1_OUTPORT      EQU     0xc0
REG_U1_OUTPORT      EQU     0x00
; REG_U1_STATUS       EQU     0xcc
REG_U1_STATUS       EQU     0x04
STATUS_WRITABLE     EQU     0x1f00
; STATUS_WRITABLE     EQU     0x02
REG_U1_CTR          EQU     0xc8
REG_U2_CTR          EQU     0x108
REG_U1_BUFCTL       EQU     0xd0
REG_U2_BUFCTL       EQU     0x110

;----------------------------------------------------------------------
; Debug messages while booting

    [ BOOT_MSG_TRUE

MsgBooting
    DCB     "\n\rBooting\n\r", 0

    EXPORT  MsgHalting
MsgHalting
    DCB     "\n\rSystem halting\n\r", 0

    DCB     0            ; 0-3 bytes padding to avoid assembler warning

MsgDummy
    DCD     0

    ]

    [ BOOT_MSG

MsgInitBim
    DCB     "BIM initializing\r\n", 0

MsgInitDram
    DCB     "DRAM initializing\r\n", 0

MsgSoftInit
    DCB     "Start SoftInit\r\n", 0

;MsgDramSizing
    ;DCB     "DRAM sizing\r\n", 0

MsgIntInit
    DCB     "INT initializing\r\n", 0

MsgZeroBss
    DCB     "BSS zeroing\r\n", 0

MsgSetupStack
    DCB     "Stack setting\r\n", 0

MsgCMain
    DCB     "Starting C main\r\n", 0

    EXPORT  MsgNotEqual
MsgNotEqual
    DCB     "\n\rFlash is not equal to Dram\n\r", 0

    EXPORT  MsgDQSFail
MsgDQSFail
    DCB     "\n\rDQS Initial failed\n\r", 0

    DCB     0            ; 0-3 bytes padding to avoid assembler warning
    DCB     0            ; 0-3 bytes padding to avoid assembler warning
    DCB     0            ; 0-3 bytes padding to avoid assembler warning

MsgDummy2
    DCD     0

    ]

    [ BOOT_MSG_TRUE

;*************************************************************************
;*  Stop UART transparent mode
;*************************************************************************

    EXPORT  SerialStop
SerialStop
    ; wait tx buffer empty.
    LDR     r0, =0x0
    ADD     r0, r0, #0x1
    CMP     r1, #0x4000000
    BLS     SerialStop

    ; set it as normal mode.
    LDR     r0, =UART_BASE
    LDR     r1, =0x0
    STRB    r1, [r0, #REG_STATUS]
    BX      lr

;*************************************************************************
;*  Output a string to UART
;*************************************************************************

    IMPORT  IsInitBootVerbose       ; Query boot message enable/disable

    EXPORT  SerialPrint
SerialPrint

    MOV     r9, r0

    ;; Set RISC active mode
    LDR     r0, =UART_BASE
    LDR     r1, =0x02
    STR     r1, [r0, #REG_RS232_MODE]

    MOV     r10, lr
    BL      IsInitBootVerbose       ; Query boot message enable/disable
    MOV     lr, r10
    CMP     r0, #0
    BEQ     _PrintDone          ; if return 0, no print message.
    MOV     r0, r9
    LDR     r1, =UART_BASE

_NextChar

    LDRB    r2, [r0], #1        ; Load byte and update address
    CMP     r2, #0              ; Check for NULL terminator
    BEQ     _PrintDone

    ; Wait for write-enable

_PutCharLoop
    LDR     r3, [r1, #REG_U0_STATUS]
    ANDS    r3, r3, #STATUS_WRITABLE
    BEQ     _PutCharLoop

    ; Output chanracter

    STRB    r2, [r1, #REG_U1_OUTPORT]

    ; Wait for completion

    B       _NextChar           ; Process next character

_PrintDone

    BX      lr

    ]   ; BOOT_MSG_TRUE

;*************************************************************************
;*  Initialize UART
;*************************************************************************

SerialInit

    LDR     r0, =UART_BASE

    LDR     r1, =0x0
    STR     r1, [r0, #REG_CTR]
    STR     r1, [r0, #REG_U1_CTR]
    STR     r1, [r0, #REG_U2_CTR]

    LDR     r1, =0x0f1a
    STR     r1, [r0, #REG_U1_BUFCTL]
    STR     r1, [r0, #REG_U2_BUFCTL]

    LDR     r1, =0x0
    STRB    r1, [r0, #REG_OUTSEL]

    ;; Set transparent mode
    ; LDR     r1, =0xe2
    ;; Set command mode
    LDR     r1, =0x00
    STRB    r1, [r0, #REG_STATUS]

    ;; Set 0 for buffer size is 1 byte, 3 for 4 bytes.
    LDR     r1, =0x00
    STRB    r1, [r0, #REG_BUFFER_SIZE]

    ;; Merge mode in MT5351, no use in MT537x.
    ; LDR     r1, =0x74
    ; STR     r1, [r0, #REG_MERGE]

    BX      lr


;============================================================================
;
; Debug dumping
;
;============================================================================

; Define offsets of special registers in save area

NUM_GPR             EQU     13
STACK               EQU     NUM_GPR
LINK                EQU     (STACK + 1)
CPSR                EQU     (LINK + 1)
SPSR                EQU     (CPSR + 1)
FAR                 EQU     (SPSR + 1)
IFSR                EQU     (FAR + 1)
DFSR                EQU     (IFSR + 1)
ELR                 EQU     (DFSR + 1)
DABORT              EQU     (ELR + 1)
LAST_REG            EQU     (DABORT + 1)
NUM_REGS            EQU     LAST_REG

SaveCPUAddr         EQU     SRAM_ITCM_BASE + 0x40
ABT_STACK_PTR       EQU     SRAM_ITCM_BASE + 0x1000

    EXPORT  g_SaveCPUContext
g_SaveCPUContext
    ALIGN   4
    SPACE   4 * NUM_REGS        ; Register save area


;*************************************************************************
;*  Save all register at g_SaveCPUContext
;*      r1 is the abort mode input. if r1==1, DataAbort mode.
;*************************************************************************

    EXPORT  INT_FillSaveArea
INT_FillSaveArea
    STMDB   sp!, {r0,r2}
;    LDR     r0, =g_SaveCPUContext
    LDR     r0, =SaveCPUAddr
    LDR     r2, =0
    STR     r2, [r0, #DABORT*4]

    STR     r1, [r0, #1*4]
    STR     r3, [r0, #3*4]
    STR     r4, [r0, #4*4]
    STR     r5, [r0, #5*4]
    STR     r6, [r0, #6*4]
    STR     r7, [r0, #7*4]
    STR     r8, [r0, #8*4]
    STR     r9, [r0, #9*4]
    STR     r10, [r0, #10*4]
    STR     r11, [r0, #11*4]
    STR     r12, [r0, #12*4]

    LDMIA   sp!, {r0,r2}
    STMDB   sp!, {r0-r3}
;    LDR     r1, =g_SaveCPUContext
    LDR     r1, =SaveCPUAddr
    STR     r0, [r1, #0*4]
    STR     r2, [r1, #2*4]

    ; Save special registers

    STR     lr, [r1, #ELR*4]            ; Exception mode LR

    MRC     p15, 0, r0, c6, c0, 0       ; FAR
    STR     r0, [r1, #FAR*4]

    MRC     p15, 0, r0, c5, c0, 0       ; DFSR
    STR     r0, [r1, #DFSR*4]

    MRC     p15, 0, r0, c5, c0, 1       ; IFSR
    STR     r0, [r1, #IFSR*4]

    MRS     r0, CPSR                    ; CPSR
    STR     r0, [r1, #CPSR*4]
    MRS     r0, SPSR                    ; SPSR
    STR     r0, [r1, #SPSR*4]

    ; Swtich to previous mode to get SP/LR

    MRS     r2, CPSR                    ; r2 = CPSR
    AND     r0, r0, #MODE_MASK          ; r0 = previous mode
    BIC     r3, r2, #MODE_MASK
    ORR     r3, r3, r0
    MSR     CPSR_cxsf, r3               ; Switch to previous mode
    STR     sp, [r1, #STACK*4]          ; Save SP
    STR     lr, [r1, #LINK*4]           ; Save LR
    MSR     CPSR_cxsf, r2               ; Switch mode back
    LDMIA   sp!, {r0-r3}

    STMDB   sp!, {r0,r2}
    LDR     r0, =g_SaveCPUContext
    LDR     r2, =0
    STR     r2, [r0, #DABORT*4]

    STR     r1, [r0, #1*4]
    STR     r3, [r0, #3*4]
    STR     r4, [r0, #4*4]
    STR     r5, [r0, #5*4]
    STR     r6, [r0, #6*4]
    STR     r7, [r0, #7*4]
    STR     r8, [r0, #8*4]
    STR     r9, [r0, #9*4]
    STR     r10, [r0, #10*4]
    STR     r11, [r0, #11*4]
    STR     r12, [r0, #12*4]

    LDMIA   sp!, {r0,r2}
    STMDB   sp!, {r0-r3}
    LDR     r1, =g_SaveCPUContext
    STR     r0, [r1, #0*4]
    STR     r2, [r1, #2*4]

    ; Save special registers

    STR     lr, [r1, #ELR*4]            ; Exception mode LR

    MRC     p15, 0, r0, c6, c0, 0       ; FAR
    STR     r0, [r1, #FAR*4]

    MRC     p15, 0, r0, c5, c0, 0       ; DFSR
    STR     r0, [r1, #DFSR*4]

    MRC     p15, 0, r0, c5, c0, 1       ; IFSR
    STR     r0, [r1, #IFSR*4]

    MRS     r0, CPSR                    ; CPSR
    STR     r0, [r1, #CPSR*4]
    MRS     r0, SPSR                    ; SPSR
    STR     r0, [r1, #SPSR*4]

    ; Swtich to previous mode to get SP/LR

    MRS     r2, CPSR                    ; r2 = CPSR
    AND     r0, r0, #MODE_MASK          ; r0 = previous mode
    BIC     r3, r2, #MODE_MASK
    ORR     r3, r3, r0
    MSR     CPSR_cxsf, r3               ; Switch to previous mode
    STR     sp, [r1, #STACK*4]          ; Save SP
    STR     lr, [r1, #LINK*4]           ; Save LR
    MSR     CPSR_cxsf, r2               ; Switch mode back
    LDMIA   sp!, {r0-r3}

    LDR     r0, =g_SaveCPUContext
    BX      lr

;============================================================================
;
; Debug dumping functions
;
;============================================================================

DataAbortDebugDumping

    STMDB   sp!, {r0,r2}
;    LDR     r0, =g_SaveCPUContext
    LDR     r0, =SaveCPUAddr
    LDR     r2, =1
    STR     r2, [r0, #DABORT*4]
    B       _SaveGPR

DebugDumping

    STMDB   sp!, {r0,r2}
;    LDR     r0, =g_SaveCPUContext
    LDR     r0, =SaveCPUAddr
    LDR     r2, =0
    STR     r2, [r0, #DABORT*4]

_SaveGPR

    ; Save general registers

    STR     r1, [r0, #1*4]
    STR     r3, [r0, #3*4]
    STR     r4, [r0, #4*4]
    STR     r5, [r0, #5*4]
    STR     r6, [r0, #6*4]
    STR     r7, [r0, #7*4]
    STR     r8, [r0, #8*4]
    STR     r9, [r0, #9*4]
    STR     r10, [r0, #10*4]
    STR     r11, [r0, #11*4]
    STR     r12, [r0, #12*4]

    LDMIA   sp!, {r0,r2}
;    LDR     r1, =g_SaveCPUContext
    LDR     r1, =SaveCPUAddr
    STR     r0, [r1, #0*4]
    STR     r2, [r1, #2*4]

    ; Save special registers

    STR     lr, [r1, #ELR*4]            ; Exception mode LR

    MRC     p15, 0, r0, c6, c0, 0       ; FAR
    STR     r0, [r1, #FAR*4]

    MRS     r0, CPSR                    ; CPSR
    STR     r0, [r1, #CPSR*4]
    MRS     r0, SPSR                    ; SPSR
    STR     r0, [r1, #SPSR*4]

    ; Swtich to previous mode to get SP/LR

    MRS     r2, CPSR                    ; r2 = CPSR
    AND     r0, r0, #MODE_MASK          ; r0 = previous mode
    BIC     r3, r2, #MODE_MASK
    ORR     r3, r3, r0
    MSR     CPSR_cxsf, r3               ; Switch to previous mode
    STR     sp, [r1, #STACK*4]          ; Save SP
    STR     lr, [r1, #LINK*4]           ; Save LR
    MSR     CPSR_cxsf, r2               ; Switch mode back

    ; Dump context

    MOV     r0, r1
    BL      BSP_DumpContext

    [ BOOT_MSG_TRUE

    ; Halt system
    LDR     r0, =MsgHalting
    BL      SerialPrint
    BL      SerialStop

    ]

_Halt
    B       _Halt

;*************************************************************************
;*  Reset handler
;*************************************************************************

    IMPORT  BootInit            ; Init boot up register & environment.
    IMPORT  BootBimInitial      ; Init BIM 
    IMPORT  CHIP_CheckBoard

    EXPORT  INT_Reset
INT_Reset

    ; Set to supervisor mode and disable interrupts
    MOV     r1, #MODE_SVC:OR:BIT_I:OR:BIT_F
    MSR     CPSR_c, r1

    ; Remap Peripheral Port
    LDR     r0, =0xf000000b ; 1MB
    MCR     p15, 0, r0, c15, c2, 4

    ; To Setup timer for booting measuring.
    BL      BootInit

    ; disable L2C
    ;LDR     r0, =0xf2000100
    ;LDR     r1, =0x0
    ;STR     r1, [r0]

    ; Disable caches and MMU
    MRC     p15, 0, r0, c1, c0, 0       ; Read CP15 control register
    BIC     r0, r0, #CR_ICACHE
    BIC     r0, r0, #CR_DCACHE
    BIC     r0, r0, #CR_MMU
    ORR     r0, r0, #CR_BPRDCT
    ;ORR     r0, r0, #CR_ALIGNMENT       ; Enable alignment fault checking
    MCR     p15, 0, r0, c1, c0, 0       ; Write back control register

    ; enable UR0 pad & disable interrupt@BIM.
    BL      BootBimInitial

    ; Initialize UART, make RISC active mode on.
    BL      SerialInit

    [ BOOT_MSG

    LDR     r0, =MsgBooting
    BL      SerialPrint

    ]

    ; Check if remapped, initialize BIM and DRAM for the first time only

    LDR     r0, =REG_RW_REMAP
    LDR     r0, [r0, #0]
    STR     r0, g_u4BootRemap
    TST     r0, #BIT_REMAP
    BNE     _SoftInit

    ; Initializa BIM
    BL      _BimInit

    [ BOOT_MSG

    LDR     r0, =MsgInitBim
    BL      SerialPrint

    ]

    ; Initializa DRAM controller

    [ BOOT_MSG

    LDR     r0, =MsgInitDram
    BL      SerialPrint

    ]

    BL      DramInit

_SoftInit

    ; Clear Misc. register, suggested by William

    [ BOOT_MSG

;    LDR     r0, =MsgSoftInit
;    BL      SerialPrint

    ]

    LDR     r0, =REG_RW_MISC
    LDR     r1, [r0, #0]
    ORR     r1, r1, #BIT_INT_ST     ; Avoid interrupt status being hidden by
                                    ; interrupt mask
    STR     r1, [r0, #0]

    ; Probe DRAM size, store size in variable DramSize

    ;[ BOOT_MSG

    ;LDR     r0, =MsgDramSizing
    ;BL      SerialPrint

    ;]

    ;BL      DramSizing
    ;STR     r0, g_u4DramSize
    BL       BSP_SetMemSize
    BL       BSP_SetChBMemSize
    
    ; Branch to INT_Intialize and start initializing Nucleus RTOS

    [ BOOT_MSG

    LDR     r0, =MsgIntInit
    BL      SerialPrint

    ]

    B       INT_Initialize

    EXPORT  g_u4DramSize
    EXPORT  g_u4DramChBSize
    EXPORT  g_u4BootRemap

g_u4DramSize
    DCD     0
g_u4DramChBSize
    DCD     0
g_u4BootRemap
    DCD     0


;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Initialize
;*
;* DESCRIPTION
;*
;*      This function sets up the global system stack variable and
;*      transfers control to the target independent initialization
;*      function INC_Initialize.  Responsibilities of this function
;*      include the following:
;*
;*             - Setup necessary processor/system control registers
;*             - Initialize the vector table
;*             - Setup the system stack pointers
;*             - Setup the timer interrupt
;*             - Calculate the timer HISR stack and priority
;*             - Calculate the first available memory address
;*             - Transfer control to INC_Initialize to initialize all of
;*               the system components.
;*
;*      Major Revision:
;*
;*          M. Kyle Craig, Accelerated Technology, Inc.
;*
;*
;*
;*
;* CALLED BY
;*
;*      Nothing. This function is the ENTRY point for Nucleus PLUS.
;*
;* CALLS
;*
;*      INC_Initialize                      Common initialization
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      W. Lamie        08-27-1994      Created initial version 1.0
;*      D. Lamie        08-27-1994      Verified version 1.0
;*      R. Baum         04-17-2002      Added code to determine which
;*                                        core module is being used to
;*                                        add support for stackable
;*                                        daugther cards
;*
;************************************************************************

; If not running from flash, this will be the entry point and the code
; necessary to change mode to the supervisor mode and lock-out interrupts
; is executed here.  If running from flash, the entry point will be at
; the INT_ROM_ENTRY function above, and changing modes and locking out
; interrupts occurs there.

    ;IMPORT  BIM_SetTimeLog          ; Set Time stamp function.

    EXPORT INT_Initialize
INT_Initialize

    ; Set to supervisor mode and disable interrupts.

    MRS     r0,CPSR                         ; Pickup current CPSR
    BIC     r0,r0,#MODE_MASK                ; Clear the mode bits
    ORR     r0,r0,#MODE_SVC                 ; Set the supervisor mode bits
    ORR     r0,r0,#LOCKOUT                  ; Insure IRQ/FIQ interrupts are
                                            ; locked out
    MSR     CPSR_cxsf,r0                    ; Setup the new CPSR

    ; Zero BSS section

    [ BOOT_MSG

    LDR     r0, =MsgZeroBss
    BL      SerialPrint

    ]

    [ {FALSE}

    ; If ARM lib is linked in, it will zero BSS in __main. Thus it's not
    ; necessary to do it here.

    MOV     r0, #0
    LDR     r1, BSS_Start_Ptr
    LDR     r2, BSS_End_Ptr

    MOV     r3, #TOTAL_STACK_SIZE           ; Also zero stacks, for debug
    ADD     r2, r2, r3

Clear_Loop

    CMP     r1, r2
    STRCC   r0, [r1], #4
    BCC     Clear_Loop

    ]

    ; Setup the vectors loaded flag to indicate to other routines in the
    ; system whether or not all of the default vectors have been loaded.
    ; If INT_Loaded_Flag is 1, all of the default vectors have been loaded.
    ; Otherwise, if INT_Loaded_Flag is 0, registering an LISR cause the
    ; default vector to be loaded.  In the ARM60 this variable is always
    ; set to 1.  All vectors must be setup by this function.

    MOV     r0,#1                           ; All vectors are assumed loaded
    LDR     r1,Loaded_Flag                  ; Build address of loaded flag
    STR     r0,[r1,#0]                      ; Initialize loaded flag

    ; Initialize the system stack pointers.  This is done after the BSS is
    ; clear because the TCD_System_Stack pointer is a BSS variable!  It is
    ; assumed that available memory starts immediately after the end of the
    ; BSS section.

    [ BOOT_MSG

    LDR     r0, =MsgSetupStack
    BL      SerialPrint

    ]

    LDR     r0,BSS_End_Ptr                  ; Pickup the ending address of BSS

    ; Setup the system stack.

    MOV     r1,#SVC_STACK_SIZE              ; Pickup system stack size
    SUB     r1,r1,#4                        ; Subtract one word for first addr
    ADD     r2,r0,r1                        ; Build start of system stack area
    BIC     r2,r2,#3                        ; Insure word aligment of stack
    MOV     r10,r0                          ; Setup initial stack limit

;    LDR     r3,System_Limit                 ; Pickup sys stack limit addr
;    STR     r10,[r3, #0]                    ; Save stack limit
    STR     r10, System_Limit_V
    MOV     sp,r2                           ; Setup initial stack pointer

;    LDR     r3,System_Stack                 ; Pickup system stack address
;    STR     sp,[r3, #0]                     ; Save stack pointer
    STR     sp, System_Stack_V

    ; Setup the IRQ stack.

    MOV     r1,#IRQ_STACK_SIZE              ; Pickup IRQ stack size in bytes
    ADD     r2,r2,r1                        ; Allocate IRQ stack area
    BIC     r2,r2,#3                        ; Insure word alignment
    MRS     r0,CPSR                         ; Pickup current CPSR
    BIC     r0,r0,#MODE_MASK                ; Clear the mode bits
    ORR     r0,r0,#MODE_IRQ                 ; Set the IRQ mode bits
    MSR     CPSR_cxsf,r0                    ; Move to IRQ mode
    MOV     sp,r2                           ; Setup IRQ stack pointer

    ; Setup the FIQ stack.

    MOV     r1,#FIQ_STACK_SIZE              ; Pickup FIQ stack size in bytes
    ADD     r2,r2,r1                        ; Allocate FIQ stack area
    BIC     r2,r2,#3                        ; Insure word alignment
    MRS     r0,CPSR                         ; Pickup current CPSR
    BIC     r0,r0,#MODE_MASK                ; Clear the mode bits
    ORR     r0,r0,#MODE_FIQ                 ; Set the FIQ mode bits
    MSR     CPSR_cxsf,r0                    ; Move to the FIQ mode
    MOV     sp,r2                           ; Setup FIQ stack pointer

    ; Setup the ABORT stack.

    MOV     r1,#ABT_STACK_SIZE
    ADD     r2,r2,r1
    BIC     r2,r2,#3
    MRS     r0,CPSR
    BIC     r0,r0,#MODE_MASK
    ORR     r0,r0,#MODE_ABT
    MSR     CPSR_cxsf,r0
;    MOV     sp,r2
    LDR     sp,=ABT_STACK_PTR

    ; Setup the UNDEFINED stack.

    MOV     r1,#UDF_STACK_SIZE
    ADD     r2,r2,r1
    BIC     r2,r2,#3
    MRS     r0,CPSR
    BIC     r0,r0,#MODE_MASK
    ORR     r0,r0,#MODE_UDF
    MSR     CPSR_cxsf,r0
;    MOV     sp,r2
    LDR     sp,=ABT_STACK_PTR

    ; Return to supervisor mode.

    MRS     r0,CPSR                         ; Pickup current CPSR
    BIC     r0,r0,#MODE_MASK                ; Clear mode bits
    ORR     r0,r0,#MODE_SVC                 ; Set the supervisor mode bits
    MSR     CPSR_cxsf,r0                    ; All interrupt stacks are setup,
                                            ; return to supervisor|system mode

    ; Define the global data structures that need to be initialized by this
    ; routine.  These structures are used to define the system timer
    ; management HISR.

    ADD     r2,r2,#4                        ; Increment to next available word
    STR     r2, HISR_Stack_Ptr_V
    MOV     r1,#TIMER_SIZE                  ; Pickup the timer HISR stack size
    BIC     r1,r1,#3                        ; Insure word alignment
    ADD     r2,r2,r1                        ; Allocate the timer HISR stack
                                            ; from available memory
    STR     r1, HISR_Stack_Size_V
    MOV     r1,#TIMER_PRIORITY              ; Pickup timer HISR priority (0-2)
    STR     r1, HISR_Priority_V

    STR     r2, g_u4ArmLibHeapStart         ; Set heap address for ARM library
    MOV     r0, #ARM_LIB_HEAP_SIZE
    STR     r0, g_u4ArmLibHeapSize
    ADD     r2, r2, #ARM_LIB_HEAP_SIZE
    STR     r2, g_u4HeapStart               ; Save the first available memory
                                            ; pointer

    ; Make a call to begin all board specific initialization.
    ; Begin with Initializing the Vector table and replacing
    ; default interrupts with Plus IRQs.  Then set up the core
    ; module. Then setup the timer and begin the system clock.

    BL      INT_Install_Vector_Table

    [ BOOT_MSG

    LDR     r0, =MsgCMain
    BL      SerialPrint

    ]

    ; Time measure point 5, before arm library
    ;LDR     r1, =ADDR_TIME_ARRAY
    ;LDR     r0, [r1, #0]
    ;BL      BIM_SetTimeLog
    BL      CHIP_CheckBoard

    ; Initialize runtime libraries, then call to main

    BL      __main                           ; Never returned

    ; Call INC_Initialize with a pointer to the first available memory
    ; address after the compiler's global data.  This memory may be used
    ; by the application.

;    LDMIA   sp!, {r0}                       ; Restore and pass the first
;    LDR     r3,=INC_Initialize              ; available memory pointer to
;    BX      r3                              ; high-level initialization

    EXPORT  g_u4HeapStart
g_u4HeapStart
    DCD     0

    EXPORT  g_u4ArmLibHeapStart
g_u4ArmLibHeapStart
    DCD     0

    EXPORT  g_u4ArmLibHeapSize
g_u4ArmLibHeapSize
    DCD     0

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Vectors_Loaded
;*
;* DESCRIPTION
;*
;*      This function returns the flag that indicates whether or not
;*      all the default vectors have been loaded.  If it is false,
;*      each LISR register also loads the ISR shell into the actual
;*      vector table.
;*
;* CALLED BY
;*
;*      TCC_Register_LISR                   Register LISR for vector
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      B. Sellew       12-18-1995      Created initial version 1.0
;*      B. Sellew       12-20-1995      Verified version 1.0
;*      D. Driscoll     05-10-2001      Implemented working version.
;*
;************************************************************************

    EXPORT  INT_Vectors_Loaded
INT_Vectors_Loaded

    ; Return the loaded vectors flag.

    LDR     r1, Loaded_Flag                 ; Load the address of the Loaded_Flag
    LDR     r0, [r1, #0]                    ; Put the value of the Loaded_Flag in
                                            ; the return register

    BX      lr                              ; Return to caller

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Setup_Vector
;*
;* DESCRIPTION
;*
;*      This function sets up the specified vector with the new vector
;*      value.  The previous vector value is returned to the caller.
;*
;* CALLED BY
;*
;*      Application
;*      TCC_Register_LISR                   Register LISR for vector
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      vector                              Vector number to setup
;*      new                                 Pointer to new assembly
;*                                            language ISR
;*
;* OUTPUTS
;*
;*      old vector contents
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      B. Sellew       12-18-1995      Created initial version 1.0
;*      B. Sellew       12-20-1995      Verified version 1.0
;*      D. Driscoll     05-10-2001      Implemented working version.
;*
;************************************************************************

    EXPORT  INT_Setup_Vector
INT_Setup_Vector

    LDR     r2, =INT_IRQ_Vectors            ; Load the vector table address
    MOV     r0, r0, LSL #2                  ; Multiply vector by 4 to get offset into table
    LDR     r3, [r2,r0]                     ; Load the old pointer
    STR     r1, [r2,r0]                     ; Store the new pointer into the vector table

    MOV     r0, r3                          ; Put the old pointer into the return register

    BX      lr                              ; Return to caller


;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Retrieve_Shell
;*
;* DESCRIPTION
;*
;*      This function retrieves the pointer to the shell interrupt
;*      service routine.  The shell interrupt service routine calls
;*      the LISR dispatch routine.
;*
;* CALLED BY
;*
;*      TCC_Register_LISR                   Register LISR for vector
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      vector                              Vector number to setup
;*
;* OUTPUTS
;*
;*      shell pointer
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      B. Sellew       12-18-1995      Created initial version 1.0
;*      B. Sellew       12-20-1995      Verified version 1.0
;*      D. Driscoll     05-10-2001      Implemented working version.
;*
;************************************************************************

    EXPORT  INT_Retrieve_Shell
INT_Retrieve_Shell

    ; Return the LISR Shell interrupt routine.

    LDR     r1, =INT_IRQ_Vectors            ; Load the vector table address
    MOV     r0, r0, LSL #2                  ; Multiply vector by 4 to get offset into table
    LDR     r0, [r1,r0]                     ; Load interrupt handler pointer into return register

    BX      lr                              ; Return to caller

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Undef_Inst
;*
;* DESCRIPTION
;*
;*     This is a stub for the Undefined Instruction Interrupt
;*
;* CALLED BY
;*
;*      None
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*     D. Driscoll       5-10-2001            Implemented stub
;*
;************************************************************************

    EXPORT  INT_Undef_Inst
INT_Undef_Inst
    B       DebugDumping

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Software
;*
;* DESCRIPTION
;*
;*     This is a stub for the Software Interrupt
;*
;* CALLED BY
;*
;*      None
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*     D. Driscoll       5-10-2001            Implemented stub
;*
;************************************************************************

    EXPORT  INT_Software
INT_Software
    B       INT_Software

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Prefetch_Abort
;*
;* DESCRIPTION
;*
;*     This is a stub for the Prefetch Abort Interrupt
;*
;* CALLED BY
;*
;*      None
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*     D. Driscoll       5-10-2001            Implemented stub
;*
;************************************************************************

    EXPORT  INT_Prefetch_Abort
INT_Prefetch_Abort
    B       DebugDumping

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Data_Abort
;*
;* DESCRIPTION
;*
;*     This is a stub for the Data Abort Interrupt
;*
;* CALLED BY
;*
;*      None
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*     D. Driscoll       5-10-2001            Implemented stub
;*
;************************************************************************

    EXPORT  INT_Data_Abort
INT_Data_Abort
    B       DataAbortDebugDumping

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Reserved
;*
;* DESCRIPTION
;*
;*     This is a stub for the Reserved Interrupt
;*
;* CALLED BY
;*
;*      None
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*     D. Driscoll       5-10-2001            Implemented stub
;*
;************************************************************************

    EXPORT  INT_Reserved
INT_Reserved
    B       INT_Reserved

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_IRQ
;*
;* DESCRIPTION
;*
;*      This function handles all IRQ calls.  This IRQ Handler will be
;*      called from the IRQ vector in the Vector Table installed by
;*      INT_Install_Vector_Table
;*
;*      This function uses two tables to service IRQs.  The
;*      INT_IRQ_Priority sets what order the pending register will
;*      be checked for interrupts (ie if more than one interrupt
;*      pending, which one is serviced first).
;*
;*      INT_IRQ_Vectors contains the address which each interrupt
;*      will branch to in order to service that particular interrupt.
;*
;* CALLED BY
;*      ExceptionHandlerTable
;*
;* CALLS
;*
;*
;* INPUTS
;*
;*
;* OUTPUTS
;*
;*      vector (in register R0)
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*      D. Driscoll     06-06-2001     Changed interrupt handling to
;*                                     conform to NUCLEUS API (uses
;*                                     NU_Dispatch_LISR calls)
;*      D. Driscoll     01-04-2002     Added ability to handle nested /
;*                                     prioritied IRQ exceptions
;*      R. Baum         04-17-2002     Modified IRQ_Base for support of
;*                                     multiple core modules
;*
;************************************************************************

    EXPORT  INT_IRQ
INT_IRQ

    ; This Code is used to correctly handle interrupts and
    ; is necessary due to the nature of the ARM7 architecture

    STMDB   sp!, {r1}
    MRS     r1, SPSR
    TST     r1, #BIT_I
    LDMIA   sp!, {r1}
    SUBNES  pc,lr,#4

    ; End ARM7 Fix

    SUB     sp,sp,#4                        ; Save a space for the interrupt
                                            ; enable register

    STMDB   sp!,{r0-r5}                     ; Save r0-r5 on temporary IRQ stack
    SUB     lr,lr,#4                        ; Adjust IRQ return address

    ; Get IRQ enable register value and save it on the bottom of the stack.

    LDR     r3, Module_IRQ_Base
    LDR     r4, [r3,#INT_IRQ_ENABLE_OFFSET]
    STR     r4, [sp,#24]

    ; Get the value of the IRQ status and enable register

    LDR     r2, [r3, #INT_IRQ_STATUS_OFFSET]
    LDR     r1, [r3, #INT_IRQ_ENABLE_OFFSET]

    ; Compare value to 0 to make sure that an interrupt actually fired.  If not
    ; branch to a spurrious interrupt handler.

    AND     r2, r2, r1
    CMP     r2, #0
    BEQ     INT_Spurious_Interrupt

    ; Get the priority table address.

    LDR     r3, =INT_IRQ_Priority

    ; Loop until the correct vector from the priority table matches the
    ; Interrupt pending bit.

    LDR     r5, =IRQ_PRIORITY_END

IRQ_VECTOR_LOOP
    LDR     r0, [r3,#0]                     ; Load first vector to be checked
                                            ; from priority table
    MOV     r1, #1                          ; Build mask
    MOV     r1, r1, LSL r0                  ; Use vector number to set mask to
                                            ; correct bit position
    BIC     r4, r4, r1                      ; Clear the enable bit to keep
                                            ; higher priority ints active
    TST     r1, r2                          ; Test if pending bit is set
    BNE     IRQ_VECTOR_FOUND                ; If bit is set, branch to found
                                            ; section...
    ADD     r3, r3, #4                      ; Move to next word in the priority
                                            ; table
    CMP     r5, r3
    BNE     IRQ_VECTOR_LOOP                 ; Continue to loop if not at the
                                            ; end of the table

    B       INT_Spurious_Interrupt

IRQ_VECTOR_FOUND

    ; Disable lower priority interrupts

;    LDR     r3, Module_IRQ_Base             ; Get IRQ base register address
;    STR     r4, [r3,#INT_IRQ_ENABLE_OFFSET]     ; Disable all lower priority
                                            ; interrupts

    ; Load the branch address from the vector table

    LDR     r3,=INT_IRQ_Vectors             ; Get IRQ vector table address
    MOV     r2, r0, LSL #2                  ; Multiply vector by 4 to get
                                            ; offset into table
    ADD     r3, r3, r2                      ; Adjust vector table address to
                                            ; correct offset
    LDR     r2, [r3,#0]                     ; Load branch address from vector
                                            ; table
    BX      r2                              ; Jump to correct branch location
                                            ; based on vector table

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_FIQ
;*
;* DESCRIPTION
;*
;*     This is a stub for the FIQ Interrupts
;*
;* CALLED BY
;*
;*      None
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*
;************************************************************************
    EXPORT  INT_FIQ
INT_FIQ

    ; This Code is used to correctly handle interrupts and
    ; is necessary due to the nature of the ARM7 architecture

;   MRS     r8, SPSR
;   TST     r8, #BIT_F
;   SUBNES  pc,lr,#4

    ; End ARM7 Fix


    SUB     sp,sp,#4                        ; Save a space for the interrupt
                                            ; enable register

    STMDB   sp!,{r0-r5}                     ; Save r0-r5 on temporary IRQ stack
    SUB     lr,lr,#4                        ; Adjust FIQ return address

    ; Get FIQ enable register value and save it on the bottom of the stack.

    LDR     r3, Module_FIQ_Base
    LDR     r4, [r3,#INT_FIQ_ENABLE_OFFSET]
    STR     r4, [sp,#24]

    ; Get the value of the FIQ status register

    LDR     r2, [r3, #INT_FIQ_STATUS_OFFSET]
    LDR     r1, [r3, #INT_FIQ_ENABLE_OFFSET]

    ; Compare value to 0 to make sure that an interrupt actually fired.  If not
    ; branch to a spurrious interrupt handler.

    AND     r2, r2, r1
    CMP     r2, #0
    BEQ     INT_Spurious_Interrupt

    ; Get the priority table address.

    LDR     r3,=INT_FIQ_Priority

    ; Loop until the correct vector from the priority table matches the
    ; Interrupt pending bit.

    LDR     r5, =FIQ_PRIORITY_END

FIQ_VECTOR_LOOP
    LDR     r0, [r3,#0]                     ; Load first vector to be checked
                                            ; from priority table
    MOV     r1, #1                          ; Build mask
    MOV     r1, r1, LSL r0                  ; Use vector number to set mask to
                                            ; correct bit position
    BIC     r4, r4, r1                      ; Clear the enable bit to keep
                                            ; higher priority ints active
    TST     r1, r2                          ; Test if pending bit is set
    BNE     FIQ_VECTOR_FOUND                ; If bit is set, branch to found
                                            ; section...
    ADD     r3, r3, #4                      ; Move to next word in the priority
                                            ; table
    CMP     r5, r3
    BNE     FIQ_VECTOR_LOOP                 ; Continue to loop if not at the
                                            ; end of the table

    B       INT_Spurious_Interrupt

FIQ_VECTOR_FOUND

    ; Disable lower priority interrupts

    LDR     r3, Module_FIQ_Base             ; Get FIQ base register address
    STR     r4, [r3,#INT_FIQ_ENABLE_OFFSET]     ; Disable all lower priority
                                            ; interrupts

    ; Load the branch address from the vector table

    LDR     r3,=INT_FIQ_Vectors             ; Get FIQ vector table address
    MOV     r2, r0, LSL #2                  ; Multiply vector by 4 to get
                                            ; offset into table
    ADD     r3, r3, r2                      ; Adjust vector table address to
                                            ; correct offset
    LDR     r2, [r3,#0]                     ; Load branch address from vector
                                            ; table
    BX      r2                              ; Jump to correct branch location
                                            ; based on vector table

;************************************************************************
;*
;* FUNCTION
;*
;*     INT_Interrupt_Shell
;*
;* DESCRIPTION
;*
;*     Handles all interrupts which use NU_Register_LISR.
;*
;*
;* CALLED BY
;*
;*      INT_IRQ
;*
;* CALLS
;*
;*      TCT_Dispatch_LISR
;*      TCT_Interrupt_Context_Restore
;*
;* INPUTS
;*
;*      vector (register r0)
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*      D. Driscoll     01-04-2002     Added ability to handle nested /
;*                                     prioritied IRQ exceptions
;*      R. Baum         04-17-2002     Modified IRQ_Base for support of
;*                                     multiple core modules
;************************************************************************

INT_Interrupt_Shell

    MOV     r4,lr                           ; Put IRQ return address into r4

    ; Lock-out all interrupts

    MRS     r1,CPSR
    ORR     r1,r1,#LOCKOUT
    MSR     CPSR_cxsf,r1

    ; Do full context save

    BL      TCT_Interrupt_Context_Save

    ; Re-enable all interrupts

;    MRS     r1,CPSR
;    BIC     r1,r1,#LOCKOUT
;    MSR     CPSR_cxsf,r1

    ; Dispatch the LISR (vector in R0)

    BL      TCC_Dispatch_LISR

    ; Lock-out all interrupts

    MRS     r1,CPSR
    ORR     r1,r1,#LOCKOUT
    MSR     CPSR_cxsf,r1

    ; Switch to IRQ mode and unnest interrupt.

    MOV     r0, #MODE_IRQ
    BL      INT_Unnest_Interrupt

    ; Do full context restore

    B       TCT_Interrupt_Context_Restore

;************************************************************************
;*
;* FUNCTION
;*
;*     INT_FIQ_Shell
;*
;* DESCRIPTION
;*
;*     Handles all interrupts which use NU_Register_LISR.
;*
;*
;* CALLED BY
;*
;*      INT_IRQ
;*
;* CALLS
;*
;*      TCT_Dispatch_LISR
;*      TCT_Interrupt_Context_Restore
;*
;* INPUTS
;*
;*      vector (register r0)
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*      D. Driscoll     01-04-2002     Added ability to handle nested /
;*                                     prioritied IRQ exceptions
;*      R. Baum         04-17-2002     Modified IRQ_Base for support of
;*                                     multiple core modules
;************************************************************************

INT_FIQ_Shell

    MOV     r4,lr                           ; Put IRQ return address into r4

    ; Do full context save

    BL       TCT_Interrupt_Context_Save

    ; Re-enable FIQs to allow nesting

    MRS     r1,CPSR
    BIC     r1,r1,#BIT_F
    MSR     CPSR_cxsf,r1

    ; Dispatch the LISR for this vector (vector in R0)

    BL      TCC_Dispatch_LISR

    ; Lock-out all interrupts

    MRS     r1,CPSR
    ORR     r1,r1,#LOCKOUT
    MSR     CPSR_cxsf,r1

    ; Switch to FIQ mode and unnest interrupt.

    MOV     r0, #MODE_FIQ

    BL      INT_Unnest_Interrupt

    ; Do full context restore

    B       TCT_Interrupt_Context_Restore

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Timer_IRQ
;*
;* DESCRIPTION
;*
;*      This routine is the board-specific section of the timer
;*      interrupt handling
;*
;* CALLED BY
;*
;*      None
;*
;* CALLS
;*
;*      TMT_Timer_Interrupt
;*
;* INPUTS
;*
;*      None
;*
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      M.Kyle Craig    12-01-1998     Created initial version 1.0
;*      D. Driscoll     01-04-2002     Added ability to handle nested /
;*                                     prioritied IRQ exceptions
;*      R. Baum         04-17-2002     Modified IRQ_Base for support of
;*                                     multiple core modules
;*
;************************************************************************

INT_Timer_IRQ

    ; Lock-out all interrupts

    MRS     r1,CPSR
    ORR     r1,r1,#LOCKOUT
    MSR     CPSR_cxsf,r1

    ; Clear the timer interrupt

    LDR     r1, Module_IRQ_Base
    MOV     r2, #IRQ_T0I                    ; Get timer 0 clear bit
    MOV     r3, #1
    MOV     r3, r3, LSL r2
    STR     r3, [r1, #INT_IRQ_CLEAR_OFFSET]     ; Clear timer interrupt

    ; Branch to timer interrupt routine (don't re-enable interrupts)

    B      TMT_Timer_Interrupt

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Timer_FIQ
;*
;* DESCRIPTION
;*
;*      This routine is the board-specific section of the timer
;*      interrupt handling
;*
;* CALLED BY
;*
;*      None
;*
;* CALLS
;*
;*      TMT_Timer_Interrupt
;*
;* INPUTS
;*
;*      None
;*
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      M.Kyle Craig    12-01-1998     Created initial version 1.0
;*      D. Driscoll     01-04-2002     Added ability to handle nested /
;*                                     prioritied IRQ exceptions
;*      R. Baum         04-17-2002     Modified IRQ_Base for support of
;*                                     multiple core modules
;*
;************************************************************************

INT_Timer_FIQ

    ; Lock-out all interrupts

    MRS     r1,CPSR
    ORR     r1,r1,#LOCKOUT
    MSR     CPSR_cxsf,r1

    ; Clear the timer interrupt

    LDR     r1, Module_FIQ_Base
    MOV     r2, #IRQ_T0I                    ; Get timer 0 clear bit
    STR     r2, [r1, #INT_FIQ_CLEAR_OFFSET]     ; Clear timer interrupt

    ; Branch to timer interrupt routine (don't re-enable interrupts)

    B      TMT_Timer_Interrupt

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Install_Vector_Table
;*
;* DESCRIPTION
;*
;*      This function stores all old addresses to previously set up
;*      interrupt service routines and sets up the vector table with
;*      the Plus interrupt sevice routine.
;*
;*
;*
;* CALLED BY
;*
;*      INT_Initialize
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      D. Phillips      1-12-2000      Created initial version 1.0
;*
;************************************************************************

    EXPORT INT_Install_Vector_Table
INT_Install_Vector_Table

    STMDB    sp!,{r0-r9,lr}                 ; Save registers on stack

    MOV     r8, #0x00                       ; Pickup address of vector table (0x00000000)
    LDR     r9, =INT_Vectors                ; Pickup address of our vector table
    LDMIA   r9!,{r0-r7}                     ; Load vector table values into registers
    STMIA   r8!,{r0-r7}                     ; Store vector table values at correct address
    LDMIA   r9!,{r0-r7}                     ; Load vector table values into registers
    STMIA   r8!,{r0-r7}                     ; Store vector table values at correct address

    ; copy [0x00000044] to [0x00000004] to use loader norflash Undef_Instr_Addr
    ;MOV     r1, #0x44
    ;LDR     r0, [r1]                        ; LDR     r0, Nor_Undef_Inst at loader
    ;MOV     r1, #0x24
    ;STR     r0, [r1]                        ; Undef_Instr_Addr at loader
    ; copy [0x00000048] to [0x0000000c] to use loader norflash Prefetch_Abort_Addr
    ;MOV     r1, #0x48
    ;LDR     r0, [r1]                        ; LDR     r0, Nor_Prefetch_Abort at loader
    ;MOV     r1, #0x2C
    ;STR     r0, [r1]                        ; Prefetch_Abort_Addr at loader
    ; copy [0x0000004c] to [0x00000010] to use loader norflash Data_Abort_Addr
    ;MOV     r1, #0x4C
    ;LDR     r0, [r1]                        ; LDR     r0, Nor_Data_Abort at loader
    ;MOV     r1, #0x30
    ;STR     r0, [r1]                        ; Data_Abort_Addr at loader

    LDMIA   sp!,{r0-r9,lr}                  ; Restore registers

    BX      lr                              ; Return to caller

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Unnest_Interrupt
;*
;* DESCRIPTION
;*
;*      This routine initializes any board-specific information.
;*      This may include timers, UARTs, interrupt handling, etc.
;*
;* CALLED BY
;*
;*      INT_Initialize
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      D. Phillips      01-12-2000     Created initial version 1.0
;*      D. Driscoll      06-06-2001     Updated to use Timer 1
;*      R. Baum          04-17-2002     Modified IRQ_Base for support of
;*                                      multiple core modules
;*
;************************************************************************

    EXPORT  INT_Unnest_Interrupt
INT_Unnest_Interrupt

    MRS     r2,CPSR                         ; Pickup current CPSR
    BIC     r1,r2,#MODE_MASK                ; Clear the mode bits
    ORR     r1,r1,r0                        ; Set the passed in mode bits
    MSR     CPSR_cxsf,r1                    ; Change to passed-in entry mode

INT_No_Mode_Switch

    LDMIA   sp!,{r1}                        ; Get FIQ/IRQ enable value off FIQ/IRQ
                                            ; stack

    ; Get the base address based on mode (IRQ/FIQ)

    CMP     r0, #MODE_IRQ
    LDREQ   r3,Module_IRQ_Base              ; Get IRQ base register address
    LDRNE   r3,Module_FIQ_Base              ; Get FIQ base register address

;    STR     r1,[r3,#INT_ENABLE_OFFSET]      ; Re-enable all lower priority
                                            ; interrupts

    MSR     CPSR_cxsf,r2                    ; Change to passed-in exit mode

    BX      lr

;************************************************************************
;*
;* FUNCTION
;*
;*      INT_Spurious_Interrupt
;*
;* DESCRIPTION
;*
;*      This function returns to the point of the interrupt.  If you
;*      have reached this function, it is because an interrupt went
;*      off without the status register getting updated.
;*
;* CALLED BY
;*
;*      INT_IRQ
;*      INT_FIQ
;*
;* CALLS
;*
;*      None
;*
;* INPUTS
;*
;*      None
;*
;*
;* OUTPUTS
;*
;*      None
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;************************************************************************

INT_Spurious_Interrupt

    STR     lr,[sp,#24]                     ; Put the interrupt return address
                                            ; in stack frame (overwrite enable
                                            ; reg)
    LDMIA   sp!,{r0-r5,pc}^                 ; Restore r0-r5 and return to point
                                            ; of interrupt


;*************************************************************************
;*  Implementation of __user_initial_stackheap
;*  This function is necessary if malloc() or new operator is used
;*************************************************************************

    IMPORT  __use_two_region_memory
    EXPORT  __user_initial_stackheap

__user_initial_stackheap

    ; Note that on return
    ;   r0: HB (heap base address)
    ;   r1: SB (stack base address)
    ;   r2: HL (heap limit address)
    ;   r3: SL (stack limit address)
    ; For one region model, heap is checked against SB. HL is not used.
    ; For two region model, heap is checked against HL
    ; SL is only used if software stack checking is enabled

    ; Setup heap region reserved for native malloc()

    LDR     r0, g_u4ArmLibHeapStart         ; Heap start address
    ADD     r2, r0, #ARM_LIB_HEAP_SIZE      ; Heap limit address

    ; Software stacking checking does not work in multithread environments
    ; since thread stacks are allocated from heap, not a pre-defined stack
    ; memory. Thus the stack memory is set to empty here

    MOV     r1, r2                          ; Stack base address
    MOV     r3, r2                          ; Stack limit address

    BX      lr

;*************************************************************************
;  Restore certain data initialized before __main, which were zeroed in
;  __main
;*************************************************************************

    EXPORT  Restore_System_Data

Restore_System_Data

    LDR     r0, HISR_Stack_Ptr_V
    LDR     r1, HISR_Stack_Ptr
    STR     r0, [r1, #0]

    LDR     r0, HISR_Stack_Size_V
    LDR     r1, HISR_Stack_Size
    STR     r0, [r1, #0]

    LDR     r0, HISR_Priority_V
    LDR     r1, HISR_Priority
    STR     r0, [r1, #0]

    LDR     r0, System_Stack_V
    LDR     r1, System_Stack
    STR     r0, [r1, #0]

    LDR     r0, System_Limit_V
    LDR     r1, System_Limit
    STR     r0, [r1, #0]

    BX      lr

;*************************************************************************
;*  Initial BIM
;*************************************************************************

_BimInit

    ; Set Stack
    LDR     r2, =0xfB000000         ; setup DTCM at 0x60000000
    ADD     r2, r2, #0x40           ; r2: The begin of heap
    MRS     r0, CPSR                ; Pickup CPSR
    BIC     r0, r0, #MODE_MASK      ; r0: CPSR with mode bits cleared

    ; Setup IRQ stack
    ORR     r1, r0, #MODE_IRQ       ; Switch to IRQ mode
    MSR     CPSR_c, r1
    ADD     r2, r2, #128
    MOV     sp, r2                  ; Setup IRQ stack pointer

    ; Setup FIQ stack
    ORR     r1, r0, #MODE_FIQ
    MSR     CPSR_c, r1              ; Switch to FIQ mode
    ADD     r2, r2, #128
    MOV     sp, r2                  ; Setup FIQ stack pointer

    ; Setup Abort stack
    ORR     r1, r0, #MODE_ABT
    MSR     CPSR_c, r1              ; Switch to Abort mode
    ADD     r2, r2, #128
    MOV     sp, r2                  ; Setup Abort stack pointer

    ; Setup Undef stack
    ORR     r1, r0, #MODE_UDF
    MSR     CPSR_c, r1              ; Switch to Undef mode
    ADD     r2, r2, #128
    MOV     sp, r2                  ; Setup Undef stack pointer

    ; Setup SVC stack
    ORR     r1, r0, #MODE_SVC
    MSR     CPSR_c, r1              ; Switch to SVC mode
    LDR     sp, =0xfB001000             ; Setup DTCM stack

    BX      lr

Sim
    ;enable p-port
    LDR     r0, =0xf000000b ; 1MB
    MCR     p15, 0, r0, c15, c2, 4
    ;start test
    MOV     r6,#0
    B       {pc} + 0x20  ; 0x50
    MOV     r0,#1
    MOV     r1,#0x1000000
    STR     r0,[r1,#0]
    STR     r0,[r1,#0]
    STR     r0,[r1,#0]
    STR     r0,[r1,#0]
    ADD     r6,r6,#1
    CMP     r6,#0x100000
    BCC     {pc} - 0x20  ; 0x34
    ;end test
    LDR     r0, =0xf00080e0
    LDR     r1, =0x5a
    STR     r1, [r0]

;************************************************************************

    ]

    END
