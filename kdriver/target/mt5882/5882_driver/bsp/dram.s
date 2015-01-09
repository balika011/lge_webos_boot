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
; *
; * Description:
; *
; *--------------------------------------------------------------------------*/

    [ :LNOT::DEF:CC_MTK_LOADER
                    GBLL    CC_MTK_LOADER
CC_MTK_LOADER      SETL    {FALSE}
    ]

    [ :LNOT:CC_MTK_LOADER

    INCLUDE def5371.inc

;**************************************
;*   COMPILE-TIME SWITCHES            *
;**************************************

    ; BOOT_MSG: Output messages to UART while booting

    [ :LNOT::DEF:BOOT_MSG
                    GBLL    BOOT_MSG
BOOT_MSG            SETL    {FALSE}
    ]

;*************************************************************************
;*   Configurations
;*************************************************************************

    [ :LNOT::DEF:FORCE_32BIT_DRAM
                    GBLL    FORCE_32BIT_DRAM
FORCE_32BIT_DRAM    SETL    {TRUE}      ; {TRUE}:  Force 32 bits DRAM
                                        ; {FALSE}: Auto-detect 32 or 64 bits
    ]

;*************************************************************************
;*   Constant definitions
;*************************************************************************

SDR_32_NORMAL		EQU		0x00000500	; REG1[10] = 1, for shorter burst
SDR_32_TURBO		EQU		0x00000570
SDR_64_NORMAL		EQU		0x00000501
SDR_64_TURBO		EQU		0x00000571

DRAM_TEST_DATA1		EQU		0x12345678	; An arbitrary test pattern
DRAM_TEST_DATA2		EQU		0x87654321	; An arbitrary test pattern

DRAM_MINIMAL_SIZE	EQU		0x800000	; 8M, Minimal DRAM size
DRAM_MAXIMAL_SIZE	EQU		0x20000000	; 512M, Maximal DRAM size

NOR_FLASH_TIMING    EQU             0x00010f01  ; for Bus clock 120MHz below

;*************************************************************************
;*   Declarations
;*************************************************************************

    ; Declaration of external variables

    [ BOOT_MSG

    ; Declaration of external functions
    IMPORT  SerialPrint
    IMPORT  SerialStop
    IMPORT  MsgHalting
    IMPORT  MsgNotEqual
    IMPORT  MsgDQSFail

    ]

    IMPORT  CLK_SetDdr
;    IMPORT  BIM_SetTimeLog          ; Set Time stamp function.

    ; Declaration of exported functions

    EXPORT  DramInit
	;EXPORT	DramSizing

;*************************************************************************
;*  Begin of code section
;*************************************************************************

    ; Code section declaration

    PRESERVE8
	AREA	INIT, CODE, READONLY
    CODE32

;*************************************************************************
;*   Local variables
;*************************************************************************

;*************************************************************************
;*  Initial DRAM
;*************************************************************************

DramInit

    MOV     r11, lr                 ; Save return address

    ; Determine FPGA or IC

    LDR     r0, =CTL_BASE
    LDR     r1, [r0, #0]
    LDR     r0, =0
    CMP     r1, r0
    BNE     _DdrInit

    ; SDR calibration

    BL      SdrCalibrate            ; SDR DRAM calibration
    B       _Shadow

_DdrInit

    ; Adjust Flash timing faster
    BL      SetFlashTiming

    ;      Set DDR related configuration, => comment for CCode setup
    BL      CLK_SetDdr

_Shadow
    ; Time measure point 3.
    MOV     r0, #3
;    BL      BIM_SetTimeLog

    ; Set arbitration timers of all agents to maximum
    ;LDR     r0, =DRAM_BASE
    ;LDR     r1, =0xff4000a9         ; Set from 6 to 4 for OSD.
    ;LDR     r1, =0xf44000a9         ; Set from 6 to 4 for OSD. from f to 4 for Group2.
    ;LDR     r1, [r0, #0x50]
    ;LDR     r2, =0xf0ffffff
    ;AND     r1, r1, r2
    ;LDR     r2, =0x04000000
    ;ORR     r1, r1, r2
    ;STR     r1, [r0, #0x50]
    ;LDR     r1, =0xff1ffff0         ; Only DSP is 1 and Gfx is 0
    ;STR     r1, [r0, #0x54]
    ;LDR     r1, =0xffffffff
    ;STR     r1, [r0, #0x58]

    BL      ShadowMemory			; Copy image from Flash to DRAM

    ; Time measure point 4, end of run on norflash.
    MOV     r0, #4
;    BL      BIM_SetTimeLog

    BL      Remap					; Swap addresses of Flash and DRAM

    BX      r11


;*************************************************************************
;*  SDR calibration
;*************************************************************************

SdrCalibrate

    ; The following code is suggested by HI

    LDR     r0, =DRAM_BASE
    LDR     r1, =0x22661154
    STR     r1, [r0, #0]

;	LDR     r1, =0x00002200
	LDR		r1, =0x00002100			; REG2[15:8] = 0x21, for shorter burst

    STR     r1, [r0, #8]
    LDR     r1, =0x0
    STR     r1, [r0, #12]
    LDR     r1, =0x00010000
    STR     r1, [r0, #4]            ; Trigger auto initialization

    ; Wait a moment

    LDR     r1, =0x05               ; How long should we wait?
_SdrLoop
    SUBS    r1, r1, #1
    BNE     _SdrLoop

    [ FORCE_32BIT_DRAM

;	LDR		r1, =SDR_32_NORMAL		; For 32-bit normal mode
	LDR		r1, =SDR_32_TURBO		; For 32-bit turbo mode

    |       ; FORCE_32BIT_DRAM

;	LDR     r1, =SDR_64_NORMAL		; For 64-bit normal mode
	LDR		r1, =SDR_64_TURBO		; For 64-bit turbo mode

	]       ; FORCE_32BIT_DRAM

    STR     r1, [r0, #4]

;	LDR     r1, =0x00002205
	LDR		r1, =0x00002105			; REG2[15:8] = 0x21, for shorter burst
    STR     r1, [r0, #8]
    LDR     r1, =0x00001000
    STR     r1, [r0, #16]

    [ :LNOT:FORCE_32BIT_DRAM

	; Detect if 64-bit mode (2 DIMMs)

	LDR		r1, =INIT_DRAM_B_BASE	; For any DRAM address
	LDR		r2, =DRAM_TEST_DATA1
	STR		r2, [r1, #0]			; Write a 256-bit test pattern
	STR		r2, [r1, #4]
	STR		r2, [r1, #8]
	STR		r2, [r1, #12]
	LDR		r2, =DRAM_TEST_DATA2
	STR		r2, [r1, #16]
	STR		r2, [r1, #20]
	STR		r2, [r1, #24]
	STR		r2, [r1, #28]

	LDMIA	r1, {r3-r10}			; Read the content back
	LDR		r2, =DRAM_TEST_DATA1
	CMP		r2, r3					; Check if it's correct
	BNE		Sdr_32bit
	CMP		r2, r4
	BNE		Sdr_32bit
	CMP		r2, r5
	BNE		Sdr_32bit
	CMP		r2, r6
	BNE		Sdr_32bit
	LDR		r2, =DRAM_TEST_DATA2
	CMP		r2, r7
	BNE		Sdr_32bit
	CMP		r2, r8
	BNE		Sdr_32bit
	CMP		r2, r9
	BNE		Sdr_32bit
	CMP		r2, r10
	BNE		Sdr_32bit

	B		Sdr_Done

Sdr_32bit
	LDR		r1, =SDR_32_TURBO		; Set to 32-bit turbo mode (1 DIMM)
	STR		r1, [r0, #4]

    ]       ; FORCE_32BIT_DRAM

Sdr_Done
    BX      lr

;*************************************************************************
;*  Shadow memory
;*************************************************************************

    ; Copy [BOOT - RW] from ROM to RAM

    IMPORT  |Image$$BOOT$$Base|         ; EXEC of BOOT
    IMPORT  |Image$$RO$$Base|           ; EXEC of RO
    IMPORT  |Load$$RO$$Base|            ; LOAD of RO
    IMPORT  |Image$$RO$$Length|         ; Length of RO
    IMPORT  |Load$$RW$$Base|            ; LOAD of RO
    IMPORT  |Image$$RW$$Length|
    IMPORT  |Image$$BSS$$ZI$$Base|      ; EXEC of BSS

ShadowMemory

    LDR     r0, =|Load$$RW$$Base|
    LDR     r1, =|Image$$RW$$Length|
    ADD     r0, r0, r1                  ; r0 = End of RW (LOAD)
    LDR     r1, =INIT_PBI_B_BASE        ; r1 = ROM
    LDR     r2, =INIT_DRAM_B_BASE       ; r2 = RAM

_BlockCopy
    LDMIA   r1!, {r4-r7}
    STMIA   r2!, {r4-r7}
    CMP     r1, r0
    BLS     _BlockCopy

    [ BOOT_MSG

    MOV     r1, #INIT_PBI_B_BASE        ; r1 = ROM
    MOV     r2, #INIT_DRAM_B_BASE       ; r2 = RAM

_BlockCmp
    LDR     r3, [r1]
    LDR     r4, [r2]
    CMP     r3, r4
    BEQ     _BlockEqual

    MOV     r5, r1
    MOV     r6, r2
    MOV     r7, r3
    MOV     r8, r4

    [ BOOT_MSG

    LDR     r0, =MsgNotEqual
    BL      SerialPrint
    LDR     r0, =MsgHalting
    BL      SerialPrint
    BL      SerialStop

    ]

_BlockHalt
    B       _BlockHalt

_BlockEqual
    ADD     r1, r1, #4
    ADD     r2, r2, #4
    CMP     r1, r0
    BLS     _BlockCmp

    ]

    BX      lr

;*************************************************************************
;*  Remap
;*************************************************************************

Remap

    LDR     r0, =REG_RW_REMAP
    LDR     r1, =BIT_REMAP
    STR     r1, [r0, #0]            ; Trigger remapping

    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

    ; Note that after remapping, the program counter is not changed, but the
    ; memory is now mapping to DRAM rather than Flash.

    BX      lr

;*************************************************************************
;*  Set Flash timing
;*************************************************************************

SetFlashTiming

    LDR     r0, =BIM_BASE
    LDR     r1, =NOR_FLASH_TIMING
    STR     r1, [r0, #0x50]
    STR     r1, [r0, #0x54]
    STR     r1, [r0, #0x58]
    BX      lr

;*************************************************************************
;*  DRAM sizing
;*************************************************************************

;DramSizing

;	LDR		r0, =DRAM_MINIMAL_SIZE	; r0 = supposed DRAM size
;	LDR		r2, =DRAM_TEST_DATA1	; r2 = test pattern 1
;	LDR		r3, =DRAM_TEST_DATA2	; r3 = test pattern 2

;_SizingLoop

;	MOV		r1, r0, LSL #1			; r1 = 2 * r0
;    CMP     r0, #DRAM_MAXIMAL_SIZE
;	BEQ     _SizingDone
;    LDR		r5, [r0, #-4]			; Preserve content at r0
;	LDR		r6, [r1, #-4]			; Preserve content at r1
;	STR		r2, [r0, #-4]			; Write test pattern 1 at r0
;	STR		r3, [r1, #-4]			; Write test pattern 2 at r1
;	LDR		r4, [r0, #-4]			; Read back address r0
;	STR		r5, [r0, #-4]			; Restore content at r0
;	STR		r6, [r1, #-4]			; Restore content at r1
;	CMP		r4, r2					; Compare with pattern 1
;	BNE		_SizingDone				; Changed if DRAM wrapping

	; Double size, try again

;	MOV		r0, r0, LSL #1
;	B		_SizingLoop
;_SizingDone

;    CMP     r0, #DRAM_MAXIMAL_SIZE
;    BNE     _Check192Done
;    MOV     r0, #0x0C000000
;    MOV     r1, #0x10000000
;    LDR		r5, [r0, #-4]			; Preserve content at r0
;	LDR		r6, [r1, #-4]			; Preserve content at r1
;	STR		r2, [r0, #-4]			; Write test pattern 1 at r0
;	STR		r3, [r1, #-4]			; Write test pattern 2 at r1
;	LDR		r4, [r0, #-4]			; Read back address r0
;	STR		r5, [r0, #-4]			; Restore content at r0
;	STR		r6, [r1, #-4]			; Restore content at r1
;	CMP		r4, r2					; Compare with pattern 1
;    BNE     _Check192Done
;    MOV     r0, #DRAM_MAXIMAL_SIZE

;_Check192Done

;    BX      lr


;*************************************************************************

    ]

	END

