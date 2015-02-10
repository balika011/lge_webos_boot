;*----------------------------------------------------------------------------*
;* Copyright Statement:                                                       *
;*                                                                            *
;*   This software/firmware and related documentation ("MediaTek Software")   *
;* are protected under international and related jurisdictions'copyright laws *
;* as unpublished works. The information contained herein is confidential and *
;* proprietary to MediaTek Inc. Without the prior written permission of       *
;* MediaTek Inc., any reproduction, modification, use or disclosure of        *
;* MediaTek Software, and information contained herein, in whole or in part,  *
;* shall be strictly prohibited.                                              *
;* MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
;*                                                                            *
;*   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
;* AGREES TO THE FOLLOWING:                                                   *
;*                                                                            *
;*   1)Any and all intellectual property rights (including without            *
;* limitation, patent, copyright, and trade secrets) in and to this           *
;* Software/firmware and related documentation ("MediaTek Software") shall    *
;* remain the exclusive property of MediaTek Inc. Any and all intellectual    *
;* property rights (including without limitation, patent, copyright, and      *
;* trade secrets) in and to any modifications and derivatives to MediaTek     *
;* Software, whoever made, shall also remain the exclusive property of        *
;* MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
;* title to any intellectual property right in MediaTek Software to Receiver. *
;*                                                                            *
;*   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
;* representatives is provided to Receiver on an "AS IS" basis only.          *
;* MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
;* including but not limited to any implied warranties of merchantability,    *
;* non-infringement and fitness for a particular purpose and any warranties   *
;* arising out of course of performance, course of dealing or usage of trade. *
;* MediaTek Inc. does not provide any warranty whatsoever with respect to the *
;* software of any third party which may be used by, incorporated in, or      *
;* supplied with the MediaTek Software, and Receiver agrees to look only to   *
;* such third parties for any warranty claim relating thereto.  Receiver      *
;* expressly acknowledges that it is Receiver's sole responsibility to obtain *
;* from any third party all proper licenses contained in or delivered with    *
;* MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
;* releases made to Receiver's specifications or to conform to a particular   *
;* standard or open forum.                                                    *
;*                                                                            *
;*   3)Receiver further acknowledge that Receiver may, either presently       *
;* and/or in the future, instruct MediaTek Inc. to assist it in the           *
;* development and the implementation, in accordance with Receiver's designs, *
;* of certain softwares relating to Receiver's product(s) (the "Services").   *
;* Except as may be otherwise agreed to in writing, no warranties of any      *
;* kind, whether express or implied, are given by MediaTek Inc. with respect  *
;* to the Services provided, and the Services are provided on an "AS IS"      *
;* basis. Receiver further acknowledges that the Services may contain errors  *
;* that testing is important and it is solely responsible for fully testing   *
;* the Services and/or derivatives thereof before they are used, sublicensed  *
;* or distributed. Should there be any third party action brought against     *
;* MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
;* to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
;* mutually agree to enter into or continue a business relationship or other  *
;* arrangement, the terms and conditions set forth herein shall remain        *
;* effective and, unless explicitly stated otherwise, shall prevail in the    *
;* event of a conflict in the terms in any agreements entered into between    *
;* the parties.                                                               *
;*                                                                            *
;*   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
;* cumulative liability with respect to MediaTek Software released hereunder  *
;* will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
;* MediaTek Software at issue.                                                *
;*                                                                            *
;*   5)The transaction contemplated hereunder shall be construed in           *
;* accordance with the laws of Singapore, excluding its conflict of laws      *
;* principles.  Any disputes, controversies or claims arising thereof and     *
;* related thereto shall be settled via arbitration in Singapore, under the   *
;* then current rules of the International Chamber of Commerce (ICC).  The    *
;* arbitration shall be conducted in English. The awards of the arbitration   *
;* shall be final and binding upon both parties and shall be entered and      *
;* enforceable in any court of competent jurisdiction.                        *
;*----------------------------------------------------------------------------*
;
;  - Our FW work in SVC Mode
;

    [ :LNOT::DEF:CC_MTK_LOADER
                    GBLL    CC_MTK_LOADER
CC_MTK_LOADER      SETL    {FALSE}
    ]

    [ :LNOT::DEF:CC_MTK_PRELOADER
                    GBLL    CC_MTK_PRELOADER
CC_MTK_PRELOADER      SETL    {FALSE}
    ]

    [ CC_MTK_LOADER

    INCLUDE arm_cpu.inc
    INCLUDE def5371.inc


;*************************************************************************
;*   Declarations
;*************************************************************************

    ; Declaration of external functions

    ; arm.s: do arm related coprocessor settings
    IMPORT  ARM_EnableICache
    [ :LNOT:CC_MTK_PRELOADER
    IMPORT  ARM_EnableDCache
    ]
    IMPORT  CHIP_GetStack
    IMPORT  ARM_StackITCM

    ; chip_boot.c: do setup register only
    IMPORT  CHIP_RS232Init
    IMPORT  CHIP_DisplayChar
    IMPORT  CHIP_DisplayString

    [ :LNOT:CC_MTK_LOADER
    IMPORT  CHIP_CheckBoard
    ]

    [ :LNOT:CC_MTK_PRELOADER
    IMPORT  CIrqHandler
    IMPORT  CFiqHandler
    ]
    IMPORT  BSP_Start

    ; Declaration of external variables

    [ :LNOT:CC_MTK_PRELOADER
    IMPORT  |Image$$BSS$$ZI$$Base|
    ;IMPORT  |Image$$BSS$$ZI$$Limit|
    IMPORT  |Image$$BSS$$ZI$$Length|
    IMPORT  |Image$$RO$$Limit|
    IMPORT  |Image$$RW$$Base|
    IMPORT  |Image$$RW$$Limit|
    ]


;*************************************************************************
;*  Begin of code section                                                *
;*************************************************************************

    ; Code section declaration

    PRESERVE8
    AREA    INIT, CODE, READONLY
    CODE32


;*************************************************************************
;*   Local variables
;*************************************************************************

; Definitions of section address

    [ :LNOT:CC_MTK_PRELOADER
BssStart
    DCD     |Image$$BSS$$ZI$$Base|

BssEnd
    ;DCD     |Image$$BSS$$ZI$$Limit|
    DCD     |Image$$BSS$$ZI$$Base| + |Image$$BSS$$ZI$$Length|

RomStart
    DCD     |Image$$RO$$Limit|

RamStart
    DCD     |Image$$RW$$Base|

RamEnd
    DCD     |Image$$RW$$Limit|

    EXPORT  g_u4HeapStart
g_u4HeapStart
    DCD     0
    ]


;----------------------------------------------------------------------
; Debug messages while booting

    EXPORT  MsgCMain
MsgCMain
    [ :LNOT:CC_MTK_PRELOADER
    DCB     "\n\rStart Lmain\n\r", 0
    |
    DCB     "\n\rStart Pmain\n\r", 0
    ]

    EXPORT  MsgHalting
MsgHalting
    DCB     "\n\rHlt\n\r", 0

;*************************************************************************
;*  Exception handlers
;*************************************************************************
    EXPORT  INT_Undef_Inst
    EXPORT  INT_Software
    EXPORT  INT_Prefetch_Abort
    EXPORT  INT_Data_Abort
    EXPORT  INT_Reserved
    EXPORT  INT_IRQ
    EXPORT  INT_FIQ

INT_Undef_Inst
    B       INT_Undef_Inst

INT_Software
    B       INT_Software

INT_Prefetch_Abort
    B       INT_Prefetch_Abort

INT_Data_Abort
    B       INT_Data_Abort

INT_Reserved
    B       INT_Reserved

INT_IRQ
    [ :LNOT:CC_MTK_PRELOADER
    B       CIrqHandler
    |
    B       INT_IRQ
    ]

INT_FIQ
    [ :LNOT:CC_MTK_PRELOADER
    B       CFiqHandler
    |
    B       INT_FIQ
    ]

;*************************************************************************
;*  Reset handler
;*************************************************************************
    EXPORT  INT_Reset

INT_Reset

    ; Set to supervisor mode and disable interrupts
    MOV     r1, #MODE_SVC:OR:BIT_I:OR:BIT_F
    MSR     CPSR_c, r1

    ; Disable caches and MMU
    MRC     p15, 0, r0, c1, c0, 0       ; Read CP15 control register
    BIC     r0, r0, #CR_ICACHE
    BIC     r0, r0, #CR_DCACHE
    BIC     r0, r0, #CR_MMU
    ORR     r0, r0, #CR_BPRDCT
    MCR     p15, 0, r0, c1, c0, 0       ; Write back control register

    ; let core1/2/3 sleep, only core 0 will go ahead
    MRC     p15, 0, r1, c0, c0, 5
	LDR     r2, =0xfff		; filter out clusterID[11:8] and CPUID[7:0]
	AND		r1, r1, r2
    LDR     r0, =0x0        ; Check if multi core, clusterID=0 and CPUID=0
    CMP     r0, r1
    BEQ     CORE_0

CORE_1
    LDR     r0, =0xf00170c0             ; Enable DMX SRAM
    LDR     r1, [r0]
    LDR     r2, =0x4000000
    ORR     r1, r1, r2
    STR     r1, [r0]
    ;ISB
    ;DSB
    MOV     r3, #0
    MCR     p15,0,r3,c7,c10,4
    LDR     r0, =SRAM_CORE1_BASE        ; write WFE to the lask 256bytes of DMX SRAM
    LDR     r1, =0xe320f002             ; WFE instruction
    STR     r1, [r0]
    ADD     r0,r0,#4
    LDR     r1, =0xf57ff06f             ; ISB
    STR     r1, [r0]
    ;ADD     r0,r0,#4
    ;LDR     r1, =0xf57ff04f             ; DSB
    ;STR     r1, [r0]
    ;ADD     r0,r0,#4
    ;LDR     r1, =0xf57ff05f             ; DMB
    ;STR     r1, [r0]
    LDR     r0, =SRAM_CORE1_BASE        ; write WFE to the lask 256bytes of DMX SRAM
    ;DSB                                 ; to assure WFE is in SRAM
    MOV     r3, #0
    MCR     p15,0,r3,c7,c10,4
    BX      r0


CORE_0

    ; disable L2C
    LDR     r0, =0xf2000100
    LDR     r1, =0x0
    STR     r1, [r0]

    ; Enable I-Cache
    BL      ARM_EnableICache

LOOP
;    B       LOOP

VFP_ENABLE
	MRC 	p15, 0, r0, c1, c1, 2
	ORR 	r0, r0, #(3<<10) 			; Enable Non-secure access to CP10 and CP11
	BIC 	r0, r0, #(3<<14) 			; Clear NSASEDIS bit
	MCR 	p15, 0, r0, c1, c1, 2
	ISB
    MRC     p15, 0, r0, c1, c0, 2       ; Read CP15 control register
    ORR     r0, r0, #CR_CP1011_PRI_ONLY       ; Enable CP10/11 ACCESS
    MCR     p15, 0, r0, c1, c0, 2       ; Write back control register
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c5, 4		;ISB
    MOV     r0,#0x40000000  ; VFP enable bit
    ;FMXR    FPEXC,r0

L2_LATENCY
	; Program L2 latency to small value
	mrc     p15, 1, r11, c9, c0, 2
	bfc     r11, #0, #18
	ldr		r12, =(0x00009251)
	orr		r11, r11, r12
	mcr     p15, 1, r11, c9, c0, 2
    isb
    dsb

NoRemapPPort
    BL      mt53xx_SRAMSetup

    ; Query SRAM stack pointer address, and pass to ARM_StackITCM by r2.
    BL      CHIP_GetStack
    MOV     r2, r0

    ; Enable ITCM and Setup Stack to ITCM.
    BL      ARM_StackITCM

    ; Call CHIP_BootupInit
    BL      CHIP_RS232Init
    MOV     r1, #0
    CMP     r0, r1
    BEQ     BootMsg
    BX      r1

BootMsg
    ; Show 'B' to note that console uart is ok.
    MOV     r0, #0x42
    BL      CHIP_DisplayChar

    ; Jump to C Code
    BL      BSP_Start

mt53xx_SRAMSetup
    ;enable UART
    LDR     r0, =0xf000c020
    LDR     r1, =0x0
    STR     r1, [r0]
    ; Enable 16K OSD SRAM
    LDR     r0, =0xf0021008             ; Enable 16K OSD SRAM
    LDR     r1, [r0]
    LDR     r2, =0x00200000
    ORR     r1, r1, r2
    STR     r1, [r0]
    LDR     r0, =0xf00170c0             ; Enable DMX SRAM
    LDR     r1, [r0]
    LDR     r2, =0x4000000
    ORR     r1, r1, r2
    STR     r1, [r0]
    BX      lr

HaltSystem
    ; Done, halt system
    LDR     r0, =MsgHalting
    BL      CHIP_DisplayString

SystemDone
    B       SystemDone

    EXPORT  ARM_SendEvent
ARM_SendEvent
    ;SEV
    BX      lr


    ]

    END

