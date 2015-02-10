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

    [ :LNOT::DEF:CC_MTK_PRELOADER
                    GBLL    CC_MTK_PRELOADER
CC_MTK_PRELOADER      SETL    {FALSE}
    ]

    [ :LNOT::DEF:CC_ROM_BOOT
                    GBLL    CC_ROM_BOOT
CC_ROM_BOOT      SETL    {FALSE}
    ]

    [ :LNOT::DEF:CC_FPGA
                    GBLL    CC_FPGA
CC_FPGA      SETL    {FALSE}
    ]

    INCLUDE arm_cpu.inc
    INCLUDE def5371.inc

;*************************************************************************
;*   Declarations
;*************************************************************************

    EXPORT  ARM_EnableICache
    [ :LNOT:CC_MTK_PRELOADER
    EXPORT  ARM_EnableDCache
    ]   ; Preloader don't enable DCache.
    EXPORT  ARM_StackITCM
    EXPORT  ARM_InvalidateICache

;*************************************************************************
;*  Begin of code section                                                *
;*************************************************************************

    ; Code section declaration
    PRESERVE8
	AREA	INIT, CODE, READONLY
    CODE32

;*************************************************************************
;*  Functions
;*************************************************************************

ARM_EnableICache
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c5, 0       ; Invalidate I-Cache
    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #CR_ICACHE
    MCR     p15, 0, r0, c1, c0, 0
    B       InstNop

ARM_InvalidateICache
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c5, 0       ; Invalidate I-Cache

InstNop
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    BX      lr

;*************************************************************************

    [ :LNOT:CC_MTK_PRELOADER
ARM_EnableDCache
    MOV     r0, #0
    MCR     p15, 0, r0, c7, c6, 0       ; Invalidate D-Cache
    MRC     p15, 0, r0, c1, c0, 0
    ORR     r0, r0, #CR_DCACHE
    MCR     p15, 0, r0, c1, c0, 0
    BX      lr
    ]   ; Preloader don't enable DCache.

;*************************************************************************

    [ CC_MTK_PRELOADER
    IMPORT  |Image$$GLOBAL$$Base|            ; LOAD of RW
    IMPORT  |Image$$GLOBAL$$Limit|           ; EXEC of RW    
    ]
    
    [ :LNOT:CC_MTK_PRELOADER
    IMPORT  |Load$$RW$$Base|            ; LOAD of RW
    IMPORT  |Image$$RW$$Base|           ; EXEC of RW
    IMPORT  |Image$$RW$$Length|
    IMPORT  |Image$$BSS$$ZI$$Base|
    IMPORT  |Image$$BSS$$ZI$$Limit|
    ]

;*************************************************************************
; Enable ITCM and Setup Stack to ITCM.
; Input: r2 is the SRAM stack pointer
;*************************************************************************
ARM_StackITCM
    MOV     r1, r2                      ; move R2 to R1, the new ITCM address.

    [ CC_MTK_PRELOADER
    
    LDR     r2, =|Image$$GLOBAL$$Base|
    LDR     r3, =|Image$$GLOBAL$$Limit|    
    MOV     r4, #0

_PreZeroLoop
    CMP     r2, r3
    STMIALT r2!, {r4}
    BLT     _PreZeroLoop
    ]

    [ :LNOT:CC_MTK_PRELOADER

    ; Zero all ZI
    LDR     r2, =|Image$$BSS$$ZI$$Base|
    LDR     r3, =|Image$$BSS$$ZI$$Limit|
    MOV     r4, #0
    MOV     r5, #0
    MOV     r6, #0
    MOV     r7, #0

_ZeroLoop
    STMIA   r2!, {r4-r7}
    CMP     r2, r3
    BLS     _ZeroLoop

    ; Move RW from |Load$$RW$$Base| to |Image$$RW$$Base| with length |Image$$RW$$Length|
    LDR     r0, =|Load$$RW$$Base|
    MOV     r3, r0                      ; r3 = Start of RW (Load)
    LDR     r2, =|Image$$RW$$Length|
    ADD     r0, r3, r2                  ; r0 = End of RW(Load)
    LDR     r2, =|Image$$RW$$Base|      ; r2 = Start of RW (Exec)

_RWCopy
    LDMIA   r3!, {r4-r7}
    STMIA   r2!, {r4-r7}
    CMP     r3, r0
    BLS     _RWCopy

    ]   ; Only zero at loader, preloader don't need.

_SkipITCMSetup
    ; Here, R1 is the TCM start address. Setup All mode Stacks.
    MRS     r0, CPSR
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_SVC
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x1000             ; SVC stack is TCM+4KB.
    MOV     sp, r2
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_FIQ
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x400              ; SVC stack is TCM+1KB.
    MOV     sp, r2
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_IRQ
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x400              ; SVC stack is TCM+1KB.
    MOV     sp, r2
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_ABT
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x800              ; SVC stack is TCM+2KB.
    MOV     sp, r2
    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_UDF
    MSR     CPSR_cxsf, r0
    ADD     r2, r1, #0x800              ; SVC stack is TCM+2KB.
    MOV     sp, r2

    BIC     r0, r0, #MODE_MASK
    ORR     r0, r0, #MODE_SVC
    MSR     CPSR_cxsf, r0

    BX      lr

;*************************************************************************
;*  END
;*************************************************************************

    END
