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
 *---------------------------------------------------------------------------*/

#include "x_typedef.h"
#include "x_bim.h"
#include "x_dram.h"
#include "drvcust_if.h"
#if defined(CC_SUPPORT_STR_CORE_OFF)
#include "hw_pdwnc.h"
#endif

EXTERN VOID INT_Reset(VOID);
EXTERN VOID INT_Vectors(VOID);

EXTERN VOID CHIP_BootEntry0(VOID);
EXTERN VOID CHIP_BootEntry1(VOID);
EXTERN VOID CHIP_BootEntry2(VOID);
EXTERN VOID CHIP_BootEntry3(VOID);


VOID BSP_Start(VOID);
UINT32 au4BSS[4];
#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT)
EXTERN VOID ARM_InvalidateICache(VOID);
EXTERN VOID __aeabi_llsl(VOID);
EXTERN VOID __aeabi_memclr4(VOID);
EXTERN VOID __aeabi_uidivmod(VOID);
EXTERN VOID x_memcpy(VOID);
EXTERN VOID CHIP_NANDCopyToDRAM(UINT32 *pu4DRAMBuf, UINT32 u4StartOffset, UINT32 u4Len);
#endif /* CC_NANDBOOT */

VOID BSPLinkSymbol(UINT32 fgDisplay)
{
    if (fgDisplay)
    {
        INT_Reset();
        INT_Vectors();
        BSPLinkSymbol(au4BSS[0]);
#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT)
        __aeabi_llsl();     // for llshl.o(c_5.l)
        x_memcpy();         // mem_mgr.o
        __aeabi_uidivmod(); // aeabi_sdiv.o(.text_udiv)
        __aeabi_memclr4();  // rt_memclr_w.o
#endif /* CC_NANDBOOT */
    }
}

#define TIME_START(x) \
    BIM_WRITE32((x), 0x12345678); \
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, 0, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN); \
    BIM_WRITE32(REG_RW_TIMER1_LLMT, 0xffffffff); \
    BIM_WRITE32(REG_RW_TIMER1_LOW , 0xffffffff); \
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN);

#define TIME_END(x) \
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, 0, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN); \
    BIM_WRITE32((x), ~BIM_READ32(REG_RW_TIMER1_LOW));

VOID BSP_Start()
{
#ifdef CC_FPGA_SIM

    HalInitMMU(TCMGET_CHANNELA_SIZE() * 0x100000 - 0x4000);
    DhrystoneTest(10);
    do
    {
    }
    while (1);

#endif

    CHIP_BootEntry0();

#if defined(CC_SUPPORT_STR_CORE_OFF) && defined(CC_MTK_PRELOADER)
    // Check for resume from suspend to dram
    extern void CHIP_ResumeFormSuspendMode();
	CHIP_ResumeFormSuspendMode();
#endif   /* CC_SUPPORT_STR_CORE_OFF && CC_MTK_PRELOADER */

    if (!BSP_IsFPGA())
    {
        CHIP_BootEntry1();
        CHIP_BootEntry2();
    }
    CHIP_BootEntry3();
}

