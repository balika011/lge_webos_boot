/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *----------------------------------------------------------------------------*
 */
/************************************************************************
 *
 *  Arm vector table entry files
 *
 ************************************************************************
 */

#ifdef CC_MTK_PRELOADER

/************************************************************************
 * Function: RemapJumpResume
 * Description:
 *    Similar to RemapJumpToDram. 
 *    Use to jump to Resume function for Suspend To DRAM
 *    This code should be copied to SRAM, and jump to RemapJumpResume
 *  on SRAM with r0 holding resume data. The first word in resume data
 *  is the resume function.
 *
 */

.equ IO_VIRT                 ,0xf0000000          @ IO base address
.equ BIM_BASE                ,IO_VIRT  + 0x8000
.equ REG_RW_REMAP            ,BIM_BASE + 0x1C     @ Remap register
.equ BIT_REMAP               ,1
.equ BIT_RESET_REMAP         ,0
.equ ICACHE_ENABLE           ,(1 << 12)


    .global  RemapJumpResume
    .type RemapJumpResume,%function
    .global  RemapJumpResume_sz
    .type RemapJumpResume_sz,%function

RemapJumpResume:
    @ Invalidate ICache
    MOV     r1, #0
    MCR     p15, 0, r1, c7, c5, 0

    @ Disable ICache
    MRC     p15, 0, r1, c1, c0, 0
    BIC     r1, r1, #ICACHE_ENABLE
    MCR     p15, 0, r1, c1, c0, 0
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

@#ifdef CC_TRUSTZONE_SUPPORT /* ARM11 need to set PMRR for non-secure (peripheral memory remap) */
@    ldr     r1, =(0xf000000b)
@    mcr     p15, 0, r1, c15, c2, 4
@#endif /* TRUSTZONE_SUPPORT */

    @ Set remap register
    LDR     r4, =REG_RW_REMAP
    LDR     r5, =BIT_REMAP
    STR     r5, [r4, #0]            @ Trigger remapping

    MOV     r1, #0
    MCR     p15, 0, r1, c7, c10, 4  @ DSB, data synchronization barrier

    @ Run nop instruction for pipeline, arm11 has 8 level pipeline!
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

    LDR     r0, [r0]		    @ r1 point to CPU STAT
    LDR     r1, [r0]		    @ first in CPU stat is resume point

    @@@ Debug test, r1 should not be 0, not 4bytes aligned or >=0x10000000
    TST     r1, r1
    BEQ     DEAD
    TST     r1, #0x3
    BNE     DEAD
    @TST     r1, #0xF0000000
    @BNE     DEAD
    BX      r1

DEAD:
    @ Too bad, going back......
    @ Set remap register
    LDR     r5, =BIT_RESET_REMAP
    STR     r5, [r4, #0]            @ Trigger remapping

    MOV     r1, #0
    MCR     p15, 0, r1, c7, c10, 4  @ DSB, data synchronization barrier

    @ Run nop instruction for pipeline, arm11 has 8 level pipeline!
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    BX      lr

RemapJumpResume_sz:
    .word   . - RemapJumpResume + 16  @ +16 for possible LDR data.

#endif
