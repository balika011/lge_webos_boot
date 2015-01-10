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
 * $RCSfile: dmx_ddi_isr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_ddi_isr.c
 *  Demux DDI driver - DDI Interrupt Handler
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_arm.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//#define VECTOR_DDI              30

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static VOID _DmxDDIIrqHandler(UINT16 u2Vector)
{
    UINT32 u4Reg, u4Status, u4Mask;
    UNUSED(u2Vector);

    u4Status = DDI_READ32(DDI_REG_DCR_INT_SET);
    u4Mask = DDI_READ32(DDI_REG_DCR_INT_MASK);

    u4Reg = u4Status & (~u4Mask);

    if (u4Reg & 0x2)        // Buffer is empty. (Empty INT)
    {
        DMXLOG(DMX_LOG_DDI 7, "DDI Empty interrupt.\n");
    }

    if (u4Reg & 0x4)        // Buffer has some more space. (Alert INT)
    {
        DMXLOG(DMX_LOG_DDI 7, "DDI Alert interrupt.\n");
    }

    // It is essential to clear the interrupt before notifying users.
    DDI_WRITE32(DDI_REG_DCR_INT_CLR, u4Status);     // Clear the interrupt

    if (u4Reg & 0x6)         // Empty or Alert INT(s)
    {
#ifdef CC_DMX_EMULATION
        _DDI_EMU_Notify();
#else
        if (!_DMX_DDI_Notify())
        {
            LOG(3, "DDI callback function returns FALSE!\n");
        }
#endif
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_DDI_InitISR
 *  Initialize interrupt handler
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DDI_InitISR(void)
{
    static BOOL _fgInited = FALSE;
    x_os_isr_fct pfnOldIsr;

    // Register ISR
    if (!_fgInited)
    {
        if (x_reg_isr(VECTOR_DDI, _DmxDDIIrqHandler, &pfnOldIsr) != OSR_OK)
        {
            LOG(1, "Error: failed to register DMX DDI ISR!\n");
            return FALSE;
        }
        UNUSED(pfnOldIsr);

        _fgInited = TRUE;
    }

    DMXLOG(DMX_LOG_DDI 7, "DDI ISR is hooked.\n");
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DDI_SetDMAInt
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_DDI_SetDMAInt(BOOL fgEmpty, BOOL fgAlert)
{
    UINT32 u4Reg;

    _DMX_Lock();
    // DDI_WRITE32(DDI_REG_DCR_INT_CLR, 0x7);
    u4Reg = DDI_READ32(DDI_REG_DCR_INT_MASK);
    _DMX_Unlock();

    if(fgEmpty)
    {
        u4Reg &= ~0x2;
    }
    else
    {
        u4Reg |= 0x2;
    }

    if(fgAlert)
    {
        u4Reg &= ~0x4;
    }
    else
    {
        u4Reg |= 0x4;
    }

    _DMX_Lock();
    DDI_WRITE32(DDI_REG_DCR_INT_MASK, u4Reg);
    _DMX_Unlock();
}

//-----------------------------------------------------------------------------
/**_DMX_DDI_SetDMAAlert_Pointer
 *   set dma alert _pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_DDI_SetDMAAlert_Pointer(UINT32 u4AlertPoint)
{
    _DMX_Lock();
    DDI_WRITE32(DDI_REG_DMA_AP, u4AlertPoint);
    _DMX_Unlock();
}



