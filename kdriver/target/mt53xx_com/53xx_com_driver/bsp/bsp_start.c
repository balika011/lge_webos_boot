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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: bsp_start.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file board.c
 *  board.c provides functions of board initialization, board detection, and
 *
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifdef CC_MTK_LOADER

#include "x_typedef.h"
#include "x_lint.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_serial.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_pinmux.h"
#include "x_printf.h"
#if defined(CC_MT5396) || defined(CC_MT5398)
#include "pcie_if.h"
#endif
//-----------------------------------------------------------------------------
// External functions
//-----------------------------------------------------------------------------

#ifndef CC_LOADER_MEM_OFFSET
#define CC_LOADER_MEM_OFFSET            (0xd00000)
#endif /* CC_LOADER_MEM_OFFSET */

#if !defined(__GCC__)
EXTERN UINT32 Image$$BSS$$ZI$$Base;
EXTERN UINT32 Image$$BSS$$ZI$$Length;
#else
EXTERN UINT32 __bss_start;
EXTERN UINT32 __bss_end;
#endif
EXTERN void *x_memset(void *s, int c, UINT32 n);
EXTERN void *x_memcpy(void *dest, const void *src, UINT32 z_size);
EXTERN void CHIP_DisplayString(const INT8 *szString);
EXTERN void CHIP_DisplayInteger(INT32 i);
EXTERN UINT32 CLK_SetDdr(void);
EXTERN void RemapJumpToDram(void);
EXTERN void CMain(void);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** BSP_Remap() Call assembly function to remap flash/dram and jump to 0
 */
//-----------------------------------------------------------------------------
void BSP_Remap(void)
{
    RemapJumpToDram();
}

//-----------------------------------------------------------------------------
/** BSP_Main() The entry point of BSP main routine
 */
//-----------------------------------------------------------------------------
void BSP_Main(void)
{
#if 0
    UINT32 u4Time;

    u4Time = *((UINT32 *)0xF00080E4);
    u4Time -= *((UINT32 *)0xF00080E8);
    CHIP_DisplayString("Calibration Time = ");
    CHIP_DisplayInteger((u4Time/27000));
    CHIP_DisplayString(" ms.\n");
#endif

#ifndef CC_MTK_PRELOADER

#if defined(CC_MT5396) || defined(CC_MT5398)
    // Set pcie phy and pull pcie reset pin.
    PCIE_PreInit();
#endif

   #if defined(CC_DISABLE_UART_UPGRADE)
    // turn off UART into RX off
    //vIO32Write4B(0xf000c020, 0x0000005a);
    UINT32 current = 0;
    current = IO_READ32(0xf0028000,0x8);
    IO_WRITE32(0xf0028000,0x8, current | 0x2000); // Disable UART RX with 0xF0028008[13]=0
   #endif
    // Copy loader exception table for IRQ handler on loader
    x_memcpy((void *)0, (void *)CC_LOADER_MEM_OFFSET, 0x40);
    // BSS zeroing.
    #if !defined(__GCC__)
    x_memset(&Image$$BSS$$ZI$$Base, 0, (UINT32)(&Image$$BSS$$ZI$$Length));
    #else
    x_memset(&__bss_start, 0, (UINT32)(&__bss_end) - (UINT32)(&__bss_start));
    #endif

    // Initial Timer again after stack is clear
    VERIFY(HAL_InitTimer());

    BSP_CkgenInit();
#endif /* CC_MTK_PRELOADER */

    // Call loader CMain() function.
    CMain();

#ifndef CC_MTK_PRELOADER
    // Never goes here
    ASSERT(FALSE);
#else
    while (1) { }
#endif /* CC_MTK_PRELOADER */
}

//-----------------------------------------------------------------------------
/** BSP_HaltSystem() Halt the system
 */
//-----------------------------------------------------------------------------
void BSP_HaltSystem(void)
{
    UINT32 u4Loop;

//    SerWaitTxBufClear();

    // Disable interrupts
#ifndef CC_MTK_PRELOADER
    HalDisableIRQ();
    HalDisableFIQ();

    SerEnd();
#endif /* CC_MTK_PRELOADER */

    u4Loop = 1;

    // Halt the system
    while (u4Loop)
    {
    }
}

#endif /* CC_MTK_LOADER */
