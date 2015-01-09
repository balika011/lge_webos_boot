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
 * $RCSfile: bim_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file bim_if.c
 *  bim_if.c The BIM (Bus Interface Module) interface
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_ckgen.h"

#ifndef CC_MTK_PRELOADER

#include "x_hal_arm.h"
#include "x_timer.h"

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef CC_ARM_GIC
static void GIC_EnableIrq(UINT32 u4Vector)
{
#ifdef CC_MT5882
	UINT32 u4Irq = u4Vector + 64;
#else
    UINT32 u4Irq = u4Vector + 32;
#endif
	UINT32 u4Mask = 1 << (u4Irq % 32);

	GICD_WRITE32(GIC_DIST_ENABLE_SET + (u4Irq / 32) * 4, u4Mask);
}

static void GIC_DisableIrq(UINT32 u4Vector)
{
#ifdef CC_MT5882
	UINT32 u4Irq = u4Vector + 64;
#else
	UINT32 u4Irq = u4Vector + 32;
#endif
	UINT32 u4Mask = 1 << (u4Irq % 32);

	GICD_WRITE32(GIC_DIST_ENABLE_CLEAR + (u4Irq / 32) * 4, u4Mask);
}

static void GIC_ClearIrq(UINT32 u4Vector)
{
#ifdef CC_MT5882
	UINT32 u4Irq = u4Vector + 64;
#else
	UINT32 u4Irq = u4Vector + 32;
#endif
	UINT32 u4Mask = 1 << (u4Irq % 32);

	GICC_WRITE32(GIC_CPU_EOI, u4Irq);
}

static BOOL GIC_IsIrqEnabled(UINT32 u4Vector)
{
#ifdef CC_MT5882
	UINT32 u4Irq = u4Vector + 64;
#else
    UINT32 u4Irq = u4Vector + 32;
#endif
	UINT32 u4Mask = 1 << (u4Irq % 32);

    return ((GICD_READ32(GIC_DIST_ENABLE_SET + (u4Irq / 32) * 4) & u4Mask) != 0)?TRUE:FALSE;
}
#endif /* CC_ARM_GIC */

//-----------------------------------------------------------------------------
/** _BimIsVectorValid() to check the vector value is valid.
 *  @param u4Vector the checking vector value.
 */
//-----------------------------------------------------------------------------
static BOOL _BimIsVectorValid(UINT32 u4Vector)
{
    return (u4Vector <= MAX_IRQ_VECTOR);
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#ifdef CC_ARM_GIC
void GIC_Init(void)
{
	UINT32 i;

	//
	// Init gic distributor
	//
	GICD_WRITE32(GIC_DIST_CTRL, 0);

	// Set all global interrupts to be level triggered, active low.
	for (i = 32; i < GIC_IRQS; i += 16)
	{
		GICD_WRITE32(GIC_DIST_CONFIG + i * 4 / 16, 0);
	}

	// Set all global interrupts to CPU core 0 only.
	for (i = 32; i < GIC_IRQS; i += 4)
	{
		GICD_WRITE32(GIC_DIST_TARGET + i * 4 / 4, 0x01010101);
	}

	// Set priority on all global interrupts.
	for (i = 32; i < GIC_IRQS; i += 4)
	{
		GICD_WRITE32(GIC_DIST_PRI + i * 4 / 4, 0xa0a0a0a0);
	}

	// Disable all interrupts.  Leave the PPI and SGIs alone as these enables are banked registers.
	for (i = 32; i < GIC_IRQS; i += 32)
	{
		GICD_WRITE32(GIC_DIST_ENABLE_CLEAR + i * 4 / 32, 0xffffffff);
	}

	GICD_WRITE32(GIC_DIST_CTRL, 1);

	// Deal with the banked PPI and SGI interrupts - disable all PPI interrupts, ensure all SGI interrupts are enabled.
	GICD_WRITE32(GIC_DIST_ENABLE_CLEAR, 0xffff0000);
	GICD_WRITE32(GIC_DIST_ENABLE_SET, 0x0000ffff);

	// Set priority on PPI and SGI interrupts
	for (i = 0; i < 32; i += 4)
	{
		GICD_WRITE32(GIC_DIST_PRI + i * 4 / 4, 0xa0a0a0a0);
	}

	GICC_WRITE32(GIC_CPU_PRIMASK, 0xf0);
	GICC_WRITE32(GIC_CPU_CTRL, 1);
}
#endif /* CC_ARM_GIC */

//-----------------------------------------------------------------------------
/** BIM_EnableIrq() Enable the interrupt of a given vector
 *  @param u4Vector: The vector
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL BIM_EnableIrq(UINT32 u4Vector)
{
	#ifndef CC_UBOOT
    UINT32 u4State;
	#endif /* CC_UBOOT */
    UINT32 u4Irq;

    if (!_BimIsVectorValid(u4Vector))
    {
        return FALSE;
    }

    #ifdef CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC3_BASE)
    {
        u4Irq = _MISC3IRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_REG32(REG_MISC3_IRQEN) |= u4Irq;
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */
    } else
    #endif // CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC2_BASE)
    {
        u4Irq = _MISC2IRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_REG32(REG_MISC2_IRQEN) |= u4Irq;
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */
    }
    else if (u4Vector > VECTOR_MISC)
    {
        u4Irq = _MISCIRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_REG32(REG_MISC_IRQEN) |= u4Irq;
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */
    }
    else
    {
        u4Irq = _IRQ(u4Vector);

        // Should be in critical section to avoid race condition
    	#ifndef CC_UBOOT
        u4State = HalCriticalStart();
    	#endif /* CC_UBOOT */
        BIM_REG32(REG_IRQEN) |= u4Irq;
    	#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
    	#endif /* CC_UBOOT */
    }

	#ifdef CC_ARM_GIC
	GIC_EnableIrq(u4Vector);
	#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
/** BIM_DisableIrq() Disable the interrupt of a given vector
 *  @param u4Vector: The vector
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL BIM_DisableIrq(UINT32 u4Vector)
{
	#ifndef CC_UBOOT
    UINT32 u4State;
	#endif /* CC_UBOOT */
    UINT32 u4Irq;

    if (!_BimIsVectorValid(u4Vector))
    {
        return FALSE;
    }

	#ifdef CC_ARM_GIC
	GIC_DisableIrq(u4Vector);
	#endif

    #ifdef CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC3_BASE)
    {
        u4Irq = _MISC3IRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_REG32(REG_MISC3_IRQEN) &= ~u4Irq;
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */

        return TRUE;
    } else
    #endif // CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC2_BASE)
    {
        u4Irq = _MISC2IRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_REG32(REG_MISC2_IRQEN) &= ~u4Irq;
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */

        return TRUE;
    }
    else if (u4Vector > VECTOR_MISC)
    {
        u4Irq = _MISCIRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_REG32(REG_MISC_IRQEN) &= ~u4Irq;
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */

        return TRUE;
    }
    else
    {
        u4Irq = _IRQ(u4Vector);

        // Should be in critical section to avoid race condition
    	#ifndef CC_UBOOT
        u4State = HalCriticalStart();
    	#endif /* CC_UBOOT */
        BIM_REG32(REG_IRQEN) &= ~u4Irq;
    	#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
    	#endif /* CC_UBOOT */
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** BIM_IsIrqEnabled Query if the interrupt of a given vector is enabled
 *  @param u4Vector: The vector
 *  @retval TRUE: Interrupt is enabled
 *  @retval FALSE: Interrupt is disabled, or the vector is invalid
 */
//-----------------------------------------------------------------------------
BOOL BIM_IsIrqEnabled(UINT32 u4Vector)
{
#ifdef CC_ARM_GIC
    return GIC_IsIrqEnabled(u4Vector);
#else
    UINT32 u4State;

    if (!_BimIsVectorValid(u4Vector))
    {
        return FALSE;
    }

    #ifdef CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC3_BASE)
    {
        u4State = BIM_READ32(REG_MISC3_IRQEN);
        return (u4State & _MISC3IRQ(u4Vector)) != 0;
    } else
    #endif // CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC2_BASE)
    {
        u4State = BIM_READ32(REG_MISC2_IRQEN);
        return (u4State & _MISC2IRQ(u4Vector)) != 0;
    }
    else if (u4Vector > VECTOR_MISC)
    {
        u4State = BIM_READ32(REG_MISC_IRQEN);

        return (u4State & _MISCIRQ(u4Vector)) != 0;
    }

    u4State = BIM_READ32(REG_IRQEN);

    return (u4State & _IRQ(u4Vector)) != 0;
#endif // CC_ARM_GIC
}


//-----------------------------------------------------------------------------
/** BIM_IsIrqPending Query if the interrupt of a given vector is pending
 *  @param u4Vector: The vector
 *  @retval TRUE: The interrupt is pending
 *  @retval FALSE: The interrupt is not asserted, or the given vector is invalid
 */
//-----------------------------------------------------------------------------
BOOL BIM_IsIrqPending(UINT32 u4Vector)
{
    UINT32 u4State;

    if (!_BimIsVectorValid(u4Vector))
    {
        return FALSE;
    }

    #ifdef CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC3_BASE)
    {
        u4State = BIM_READ32(REG_MISC3_IRQST);
        return (u4State & _MISC3IRQ(u4Vector)) != 0;
    } else
    #endif // CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC2_BASE)
    {
        u4State = BIM_READ32(REG_MISC2_IRQST);
        return (u4State & _MISC2IRQ(u4Vector)) != 0;
    }
    else if (u4Vector > VECTOR_MISC)
    {
        u4State = BIM_READ32(REG_MISC_IRQST);

        return (u4State & _MISCIRQ(u4Vector)) != 0;
    }

    u4State = BIM_READ32(REG_IRQST);

    return (u4State & _IRQ(u4Vector)) != 0;
}


//-----------------------------------------------------------------------------
/** BIM_ClearIrq() Clear the pending interrupt of a given vector
 *  @param u4Vector: The vector
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL BIM_ClearIrq(UINT32 u4Vector)
{

    UINT32 u4Irq;
	#ifndef CC_UBOOT
    UINT32 u4State;
	#endif /* CC_UBOOT */


    if (!_BimIsVectorValid(u4Vector))
    {
        return FALSE;
    }

    #ifdef CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC3_BASE)
    {
        u4Irq = _MISC3IRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_WRITE32(REG_MISC3_IRQCLR, u4Irq);
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */
    } else
    #endif // CC_VECTOR_HAS_MISC3
    if (u4Vector >= VECTOR_MISC2_BASE)
    {
        u4Irq = _MISC2IRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_WRITE32(REG_MISC2_IRQCLR, u4Irq);
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */
    }
    else if (u4Vector > VECTOR_MISC)
    {
        u4Irq = _MISCIRQ(u4Vector);
		#ifndef CC_UBOOT
        u4State = HalCriticalStart();
		#endif /* CC_UBOOT */
        BIM_WRITE32(REG_MISC_IRQCLR, u4Irq);
		#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
		#endif /* CC_UBOOT */
    }
    else
    {
        u4Irq = _IRQ(u4Vector);

    	#ifndef CC_UBOOT
        u4State = HalCriticalStart();
    	#endif /* CC_UBOOT */
        BIM_WRITE32(REG_IRQCL, u4Irq);
    	#ifndef CC_UBOOT
        HalCriticalEnd(u4State);
    	#endif /* CC_UBOOT */
    }

	#ifdef CC_ARM_GIC
	GIC_ClearIrq(u4Vector);
	#endif

    return TRUE;
}

#ifndef CC_SECURE_WORLD
//-----------------------------------------------------------------------------
/** BIM_Set7Seg() Set the 7-segment display LED value.
 *  @param u4Value the last 4 digits of u4Value will display on the 7-Seg LED.
 */
//-----------------------------------------------------------------------------
void BIM_Set7Seg(UINT32 u4Value)
{
    // no support in 5387
    //BIM_WRITE32(REG_RW_SEG7_DATA, u4Value);
}

//-----------------------------------------------------------------------------
/** BIM_GetOscillator() Get ring oscillator raw value.
 *  @retval return the ring oscillator raw value.
 */
//-----------------------------------------------------------------------------
UINT32 BIM_GetOscillator()
{
    BIM_WRITE32(REG_RW_ROSCCTL, ROSC_CLR);
    BIM_WRITE32(REG_RW_ROSCCTL, ROSC_SEL);
    BIM_WRITE32(REG_RW_ROSCCTL, (ROSC_SEL | ROSC_TRIGGER));
    HAL_Delay_us(10000);
    return BIM_READ32(REG_RO_ROSCCNT);
}

//-----------------------------------------------------------------------------
/** BIM_LockModifyReg32() Read/and/or/Write register with lock held
 *  to prevent modify register from other threads.
 */
//-----------------------------------------------------------------------------
void BIM_LockModifyReg32(UINT32 reg, UINT32 mask, UINT32 setVal)
{
    UINT32 u4State, u4Val;

	#ifndef CC_UBOOT
    u4State = HalCriticalStart();
	#endif /* CC_UBOOT */
    u4Val = BIM_READ32(reg);
    u4Val &= mask;
    u4Val |= setVal;
    BIM_WRITE32(reg, u4Val);
	#ifndef CC_UBOOT
    HalCriticalEnd(u4State);
	#endif /* CC_UBOOT */
}

#if defined(CC_SUPPORT_STR_CORE_OFF) && !defined(CC_MTK_LOADER) && !defined(CC_UBOOT)
//-----------------------------------------------------------------------------
/** BIM_pm_suspend()
 *  Suspend function, save IRQ enable setting.
 */
//-----------------------------------------------------------------------------
static UINT32 aBimSuspendSaves[4];
void BIM_pm_suspend(void)
{
    aBimSuspendSaves[0] = BIM_REG32(REG_RW_MISC);
    aBimSuspendSaves[1] = BIM_REG32(REG_IRQEN);
    aBimSuspendSaves[2] = BIM_REG32(REG_MISC_IRQEN);
	#ifdef REG_MISC2_IRQEN
    aBimSuspendSaves[3] = BIM_REG32(REG_MISC2_IRQEN);
	#endif
}

//-----------------------------------------------------------------------------
/** BIM_pm_suspend()
 *  Resume function, restore IRQ settings
 */
//-----------------------------------------------------------------------------
void BIM_pm_resume(void)
{
    BIM_REG32(REG_RW_MISC) = aBimSuspendSaves[0];
    BIM_REG32(REG_IRQEN) = aBimSuspendSaves[1];
    BIM_REG32(REG_MISC_IRQEN) = aBimSuspendSaves[2];
	#ifdef REG_MISC2_IRQEN
    BIM_REG32(REG_MISC2_IRQEN) = aBimSuspendSaves[3];
	#endif
}
#endif /* CC_SUPPORT_STR_CORE_OFF && !loader && !uboot */
#endif
#endif /*CC_MTK_PRELOADER*/
