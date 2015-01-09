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
 * $RCSfile: sif_hw_v2.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_hw_v2.c
 *  This file implement serial interface SIF function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "sif_debug.h"
#include "sif_hw.h"
#include "sif_isr.h"
#include "sif_if.h"

#include "drvcust_if.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_pinmux.h"
#include "x_hal_arm.h"
#include "x_printf.h"
#include "x_timer.h"

#include "x_pdwnc.h"
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
#include "pcmcia_if.h"
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

/// For driver customization options
/// 1. open drain (default use open drain mode)
/// 2. scl stretch (default don't use clock stretch mode)

/// Default use interrupt mode for OSAI mode
#define CC_SIF_USE_INTERRUPT_MODE

/// Use OSAI function option
#if defined (CC_MTK_LOADER)
#define SIF_USE_OSAI            0
#ifdef CC_SIF_USE_INTERRUPT_MODE
#undef CC_SIF_USE_INTERRUPT_MODE
#endif // CC_SIF_USE_INTERRUPT_MODE
#else // defined (CC_MTK_LOADER)
#define SIF_USE_OSAI            1
#endif // defined (CC_MTK_LOADER)

#if defined (USE_NEW_MODE)
UINT32 u4FLAG_Read_Second = 0;
#endif

/// Default use multiple data read/write
#define CC_SIF_MULTIPLE_DATA_BYTE_ACCESS

/// Support delay between TRI for SCL stretch workaround
// #define CC_SIF_TRI_DELAY_SUPPORT

#ifdef CC_SIF_TRI_DELAY_SUPPORT
/// Current slave device to communicate to
static UINT8 _u1CurrDevAddr = 0;
static UINT8 _u1TRIDelayDevAddr = 0;
/// The delay between, in micro seconds.
static UINT32 _u4TRIDelay = 0;
#if SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)
static HAL_TIME_T _rTriTime = {0, 0}, _rIsrTime = {0, 0};
#endif // SIF_USE_OSAI
void SIF_SetTRIDelay(UINT32 u4Delay)
{
    _u4TRIDelay = u4Delay;
}
#endif // CC_SIF_TRI_DELAY_SUPPORT

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/// SIF has 2 data port register. Total 8 bytes, so page read/write unit is 8 bytes.
#if SIF_USE_LENGTHER_PAGE
#define SIF_PAGE_SIZE           ((UINT32)36)
#else
#define SIF_PAGE_SIZE           ((UINT32)8)
#endif

/// Regular timeout checkings for SIF interrupt status, time is in ms.
/// For Nucleus, minimum is 5 ms.
#define SIF_SEMA_TIMEOUT        ((UINT32)10)

#define PINTNO_SIF      PDWNC_INTNO_SIFM			// SIF interrupt number inside Pdwnc
#define PINTNO_SIF1     PDWNC_INTNO_SIFM1			// SIF1 interrupt number inside Pdwnc


#define SIF_TIMEOUT_MAX  (2000)

#define  SIF_OK    (0)

#define  SIF_DEGLITCHLEVEL    (0x03)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef struct _SIFDEVPARAM_T
{
    UINT16  u2ClkDiv;        //< parameter of divider to divide 27 Mhz for SCL. 12 bits width.
    UINT8   u1DevAddr;       //< device address: 7 bits width.
    UINT32  u4WordAddrNum;   //< number of word address:  2 bits width, 0, 1, 2, 3.
    UINT32  u4WordAddr;      //< word address:  24 bits width.
    UINT8*   pu1Buf;         //< point to user's buffer.
    UINT16  u2ByteCnt;       //< the data length that user want to read/write.
    UINT16  u2ProcCnt;       //< the data length that SIF read/write completely.
    BOOL    fgTuner;         //< output pins are TUNER_DATA & TUNER_CLK.
    BOOL    fgNoStart;       //< send start bit or not>
    BOOL    fgNoStop;        //< send stop bit or not>
    BOOL    fgOpenDrain;     //< OpenDrain>
    BOOL    fgSclStretch;    //< SCL_STRETCH bit> byte/bit synchronization
    BOOL    fgSystemI2C;     //< I2C Master 0 (system i2c)
    BOOL    fgSecondI2C;     //< I2C Master 1 (second i2c)
    BOOL    fgPdwncI2C;      //< I2C Master 2 (pdwnc  i2c)
    BOOL    fgPdwncI2C1;      //< I2C Master 3 (pdwnc  i2c 1)//lili_sif
    BOOL    fg8295BI2C0;     //<I2C Master 4 (8295B i2c 0)
    BOOL    fg8295BI2C1;     //<I2C Master5 (8295B i2c 1)
} SIFDEVPARAM_T;

#ifndef SIF_CHECK_SDA_LOW
typedef struct _SIF_BUS_STATUS_
{
    UINT8 SIF_BUS_SDA;
    UINT8 SIF_BUS_SCL;
} SIF_BUS_STATUS_T;
#endif

typedef enum
{
    SIF_CTL_NORMAL    =  0,       /* Normal I2C operation         */
    SIF_CTL_NO_START  =  1 << 1,  /* Do not generate start signal */
    SIF_CTL_NO_STOP   =  1 << 2,  /* Do not generate stop signal  */
} SIF_BUS_CONTROL_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
#ifndef PCMCIA_RawWrite32MSK
#define PCMCIA_RawWrite32MSK(offset, value, msk)  PCMCIA_RawWrite32((offset),((PCMCIA_RawRead32(offset) & ~(msk)) | (value)))
#endif
#endif
#endif


//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Forward declarations
//---------------------------------------------------------------------------
#if SIF_USE_OSAI
// static VOID _SIF_AsyncThread(VOID);
#endif
static VOID SIF_V1_Init(VOID);
static UINT16 SIF_V1_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                            UINT8* pu1Buf, UINT16 u2ByteCnt);
static UINT16 SIF_V1_X_Sp_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                               UINT8* pu1Buf, UINT16 u2ByteCnt);
static UINT16 SIF_V1_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                             UINT8* pu1Buf, UINT16 u2ByteCnt);
#ifdef CC_CLI
static VOID SIF_V1_Stop(VOID);
#endif
static VOID SIF_V1_ISRHandler(UINT16 u2Vector);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static SIFDEVPARAM_T _rSifDevParm;

/// SIF initialized flag.
static BOOL _fgSifInit            = FALSE;
static BOOL _fgSifInitStart       = FALSE;
static BOOL _fgSifInitWait        = FALSE;
static BOOL _fgIsrEnabled         = FALSE;

#if SIF_USE_OSAI
/// SIF multi-task protection semaphore.
static HANDLE_T _hSifUserSema     = NULL_HANDLE;

static HANDLE_T _hSifAsyncSema     = NULL_HANDLE;
// static HANDLE_T _hSifAsyncMsg;
// static HANDLE_T _hSifAsyncThread;
#endif // SIF_USE_OSAI

static BOOL _fgEnableSclStretch   = FALSE;
static BOOL _fgDisableOpenDrain   = FALSE;

static PFN_SIF_CFG_BEFORE_ACTION_FUNC _pfnSifCfgBeforeActionFunc = NULL;
static PFN_SIF_CFG_AFTER_ACTION_FUNC _pfnSifCfgAfterActionFunc = NULL;
static PFN_SIF_CFG_PIN_SET_FUNC _pfnSifCfgPinSetFunc = NULL;

static SIF_STRUCT_T _rSifStruct =
{
    1, // SIF engine ver.2
    {
        SIF_V1_Init,
#ifdef CC_CLI
        SIF_V1_Stop,
#else
        NULL,
#endif
        SIF_V1_X_Read,
        SIF_V1_X_Sp_Read,
        SIF_V1_X_Write,
        SIF_V1_ISRHandler,
    }
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static VOID _SIF_WRITE32(UINT32 u4Offset, UINT32 u4Value)
{
    if (_rSifDevParm.fgSystemI2C)
    {
        IO_WRITE32(BIM_BASE, (u4Offset), (u4Value));
    }
    else if (_rSifDevParm.fgTuner)
    {
        IO_WRITE32(BIM_BASE, (u4Offset), (u4Value));
    }
    else if (_rSifDevParm.fgSecondI2C)
    {
        IO_WRITE32(BIM_BASE, (u4Offset), (u4Value));
    }
    else if (_rSifDevParm.fgPdwncI2C)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            IO_WRITE32(PDWNC_BASE, (u4Offset - IO_VAL - 0x80), (u4Value));
        }
        else
        {
            IO_WRITE32(PDWNC_BASE, (u4Offset - IO_VAL - 0x88), (u4Value));
        }
    }
    else if (_rSifDevParm.fgPdwncI2C1)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            IO_WRITE32(PDWNC_BASE, (u4Offset - IO_VAL - 0x80 - 0x60), (u4Value));
        }
        else
        {
            IO_WRITE32(PDWNC_BASE, (u4Offset - IO_VAL - 0x88 - 0x60), (u4Value));
        }
    }
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
    else if (_rSifDevParm.fg8295BI2C0)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            PCMCIA_RawWrite32((u4Offset + 0x75c0), (u4Value));
        }
        else if ((u4Offset == RW_SM0_D0_REG) || (u4Offset == RW_SM0_D1_REG))
        {
            PCMCIA_RawWrite32((u4Offset + 0x75b8), (u4Value));
        }

    }
    else if (_rSifDevParm.fg8295BI2C1)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            PCMCIA_RawWrite32((u4Offset + 0x79c0), (u4Value));
        }
        else if ((u4Offset == RW_SM0_D0_REG) || (u4Offset == RW_SM0_D1_REG))
        {
            PCMCIA_RawWrite32((u4Offset + 0x79b8), (u4Value));
        }
    }
#endif
#endif

}


static UINT32 _SIF_READ32(UINT32 u4Offset)
{
    if (_rSifDevParm.fgSystemI2C)
    {
        return IO_READ32(BIM_BASE, (u4Offset));
    }
    else if (_rSifDevParm.fgTuner)
    {
        return IO_READ32(BIM_BASE, (u4Offset));
    }
    else if (_rSifDevParm.fgSecondI2C)
    {
        return IO_READ32(BIM_BASE, (u4Offset));
    }
    else if (_rSifDevParm.fgPdwncI2C)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            return IO_READ32(PDWNC_BASE, (u4Offset - IO_VAL - 0x80));
        }
        else
        {
            return IO_READ32(PDWNC_BASE, (u4Offset - IO_VAL - 0x88));
        }
    }
    else if (_rSifDevParm.fgPdwncI2C1)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            return IO_READ32(PDWNC_BASE, (u4Offset - IO_VAL - 0x80 - 0x60));
        }
        else
        {
            return IO_READ32(PDWNC_BASE, (u4Offset - IO_VAL - 0x88 - 0x60));
        }
    }
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
    else if (_rSifDevParm.fg8295BI2C0)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            return PCMCIA_RawRead32((u4Offset + 0x75c0));
        }
        else if ((u4Offset == RW_SM0_D0_REG) || (u4Offset == RW_SM0_D1_REG))
        {
            return PCMCIA_RawRead32((u4Offset + 0x75b8));
        }
    }
    else if (_rSifDevParm.fg8295BI2C1)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            return PCMCIA_RawRead32((u4Offset + 0x79c0));
        }
        else if ((u4Offset == RW_SM0_D0_REG) || (u4Offset == RW_SM0_D1_REG))
        {
            return PCMCIA_RawRead32((u4Offset + 0x79b8));
        }
    }
#endif
#endif
    return IO_READ32(BIM_BASE, (u4Offset));
}

static VOID _SIF_WRITE32MSK(UINT32 u4Offset, UINT32 u4Value, UINT32 u4Mask)
{
    if (_rSifDevParm.fgSystemI2C)
    {
        IO_WRITE32MSK(BIM_BASE, (u4Offset), (u4Value), (u4Mask));
    }
    else if (_rSifDevParm.fgTuner)
    {
        IO_WRITE32MSK(BIM_BASE, (u4Offset), (u4Value), (u4Mask));
    }
    else if (_rSifDevParm.fgSecondI2C)
    {
        IO_WRITE32MSK(BIM_BASE, (u4Offset), (u4Value), (u4Mask));
    }
    else if (_rSifDevParm.fgPdwncI2C)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            IO_WRITE32MSK(PDWNC_BASE, (u4Offset - IO_VAL - 0x80), (u4Value), (u4Mask));
        }
        else
        {
            IO_WRITE32MSK(PDWNC_BASE, (u4Offset - IO_VAL - 0x88), (u4Value), (u4Mask));
        }
    }
    else if (_rSifDevParm.fgPdwncI2C1)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            IO_WRITE32MSK(PDWNC_BASE, (u4Offset - IO_VAL - 0x80 - 0x60), (u4Value), (u4Mask));
        }
        else
        {
            IO_WRITE32MSK(PDWNC_BASE, (u4Offset - IO_VAL - 0x88 - 0x60), (u4Value), (u4Mask));
        }
    }
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
    else if (_rSifDevParm.fg8295BI2C0)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            PCMCIA_RawWrite32MSK((u4Offset + 0x75c0), (u4Value), (u4Mask));
        }
        else if ((u4Offset == RW_SM0_D0_REG) || (u4Offset == RW_SM0_D1_REG))
        {
            PCMCIA_RawWrite32MSK((u4Offset + 0x75b8), (u4Value), (u4Mask));
        }
    }
    else if (_rSifDevParm.fg8295BI2C1)
    {
        if ((u4Offset == RW_SM0_CTRL0_REG) || (u4Offset == RW_SM0_CTRL1_REG))
        {
            PCMCIA_RawWrite32MSK((u4Offset + 0x79c0), (u4Value), (u4Mask));
        }
        else if ((u4Offset == RW_SM0_D0_REG) || (u4Offset == RW_SM0_D1_REG))
        {
            PCMCIA_RawWrite32MSK((u4Offset + 0x79b8), (u4Value), (u4Mask));
        }
    }
#endif
#endif

}

#ifdef CC_SIF_TRI_DELAY_SUPPORT
#if SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)
/** _SIF_GetIsrTime
 *  Get the time stamp when SIF ISR happens
 *  @param  void
 *  @retval void
 */
static VOID _SIF_GetIsrTime(VOID)
{
    if ((_u1CurrDevAddr & 0xFE) == _u1TRIDelayDevAddr)
    {
        HAL_GetTime(&_rIsrTime);
    }
}
#endif // SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)
/** _SIF_CheckAndDelay
 *  Check time stamp and delay between TRI
 *  @param  void
 *  @retval void
 */
static VOID _SIF_CheckAndDelay(VOID)
{
    UINT32 u4Clk_2 = (_rSifDevParm.u2ClkDiv << 1) / 27;
#if SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)
    UINT32 u4Delay;
    HAL_TIME_T rDeltaTime;
    if ((_u1CurrDevAddr & 0xFE) == _u1TRIDelayDevAddr)
    {
        HAL_GetTime(&_rTriTime);
        HAL_GetDeltaTime(&rDeltaTime, &_rIsrTime, &_rTriTime);
        // If the delta time between TRI already > TRI_DELAY, extra delay is not needed
        if (rDeltaTime.u4Seconds >= 1 || rDeltaTime.u4Micros >= _u4TRIDelay + u4Clk_2)
        {
            return;
        }
        u4Delay = _u4TRIDelay + u4Clk_2 - rDeltaTime.u4Micros;
        // Busy delay here since the slave device will only stretch SCL in micro second order
        HAL_Delay_us(u4Delay);
    }
#else
    if ((_u1CurrDevAddr & 0xFE) == _u1TRIDelayDevAddr)
    {
        HAL_Delay_us(_u4TRIDelay + u4Clk_2);
    }
#endif // SIF_USE_OSAI && defined(CC_SIF_USE_INTERRUPT_MODE)       
}
#endif // CC_SIF_TRI_DELAY_SUPPORT

#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
static VOID _SIF_Control(VOID);
#if defined(CC_FAST_INIT)
VOID _SIF_DisableAndClearInterrupt(VOID);
#else
static VOID _SIF_DisableAndClearInterrupt(VOID);
#endif
static VOID _SIF_TimeOutReset(VOID)
{
    UINT32 u4Reg;
    UINT32 u4Flag;

    //Enter critical section,
    u4Flag = HalCriticalStart();

    // Disable and clear interrupt
    _SIF_DisableAndClearInterrupt();

    //SIF HW Reset
    if (_rSifDevParm.fgSystemI2C)
    {
        u4Reg = BIM_READ32(RW_PERMISC_REG);
        BIM_WRITE32(RW_PERMISC_REG, u4Reg | SIF0_SW_RST);
        u4Reg = BIM_READ32(RW_PERMISC_REG);
        BIM_WRITE32(RW_PERMISC_REG, u4Reg & ~SIF0_SW_RST);

        // Enable SIF master
        IO_WRITE32MSK(BIM_BASE, RW_SM0_CTRL0_REG, SM_EN, SM_EN);
    }
    else if (_rSifDevParm.fgSecondI2C)
    {
        u4Reg = BIM_READ32(RW_PERMISC_REG);
        BIM_WRITE32(RW_PERMISC_REG, u4Reg | SIF1_SW_RST);
        u4Reg = BIM_READ32(RW_PERMISC_REG);
        BIM_WRITE32(RW_PERMISC_REG, u4Reg & ~SIF1_SW_RST);

        // Enable SIF master
        IO_WRITE32MSK(BIM_BASE, RW_SM0_CTRL0_REG + 0x30, SM_EN, SM_EN);
    }

    HAL_Delay_us(100);

    //Leave critical section
    HalCriticalEnd(u4Flag);
    // Restore SIF control register value
    _SIF_Control();

    // Protect: If SIF ISR comes after timeout but before timeout reset, we need to reset sema in lock state
    if (_SIF_ISR_Lock() != OSR_WOULD_BLOCK)
    {
        _SIF_ISR_DeleteSema();
        _SIF_ISR_CreateSema();
    }

}
#endif
#endif



// Lock SIF semaphore
static VOID _SIF_UserLock(VOID)
{
    SIF_Init();
#if SIF_USE_OSAI
    // wait semaphore
    VERIFY(x_sema_lock(_hSifUserSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif // SIF_USE_OSAI
}


// Unlock SIF semaphore
static VOID _SIF_UserUnlock(VOID)
{
#if SIF_USE_OSAI
    // release semaphore
    VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);
#endif //SIF_USE_OSAI
}


// Wait tirgger bit becomes 0
static INT32 _SIF_WaitHwDone(VOID)
{
    UINT32 u4Reg;

#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    if (_SIF_ISR_Lock() != OSR_OK)
    {
        Printf("_SIF_WaitHwDone isr semaphore lock timeout!\n");
        _SIF_TimeOutReset();
        return SIF_ERROR_TIMEOUT;
    }

#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI 


    u4Reg = TRI;

    while (u4Reg & TRI)
    {
        u4Reg = _SIF_READ32(RW_SM1_CTRL1_REG);
    }

    return SIF_OK;
}


#if defined(CC_FAST_INIT)
VOID _SIF_DisableAndClearInterrupt(VOID)
#else
static VOID _SIF_DisableAndClearInterrupt(VOID)
#endif
{
    if ((!_fgIsrEnabled) && (_fgSifInit))
    {
        return;
    }

    _fgIsrEnabled = FALSE;

    IO_WRITE32MSK(BIM_BASE, RW_PINTEN_REG, 0, SIF_M0_INTEN);
    IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M0_INTCLR, SIF_M0_INTCLR);

    IO_WRITE32MSK(BIM_BASE, RW_PINTEN_REG, 0, SIF_M1_INTEN);
    IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M1_INTCLR, SIF_M1_INTCLR);

    VERIFY(BIM_DisableIrq(VECTOR_SERIAL));
    VERIFY(BIM_ClearIrq(VECTOR_SERIAL));

    if (_SIF_ISR_IsPdwncMasterEnable())
    {
        IO_WRITE32MSK(PDWNC_BASE, RW_AINTEN_REG, 0, SIF_PD_M0_INTEN);
        IO_WRITE32MSK(PDWNC_BASE, RW_AINTCLR_REG, SIF_PD_M0_INTCLR, SIF_PD_M0_INTCLR);
    }

    if (_SIF_ISR_IsPdwncMaster1Enable())
    {
        IO_WRITE32MSK(PDWNC_BASE, RW_AINTEN_REG, 0, SIF_PD_M1_INTEN);
        IO_WRITE32MSK(PDWNC_BASE, RW_AINTCLR_REG, SIF_PD_M1_INTCLR, SIF_PD_M1_INTCLR);
    }

#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
    if (_SIF_ISR_Is8295bMaster0Enable())
    {
        PCMCIA_RawWrite32MSK(RW_WINTEN_REG, 0, SIF_M4_INTEN);

        PCMCIA_RawWrite32MSK(RW_WINTCLR_REG, SIF_M4_INTCLR, SIF_M4_INTCLR);
    }
    if (_SIF_ISR_Is8295bMaster1Enable())
    {
        PCMCIA_RawWrite32MSK(RW_WINTEN_REG, 0, SIF_M5_INTEN);

        PCMCIA_RawWrite32MSK(RW_WINTCLR_REG, SIF_M5_INTCLR, SIF_M5_INTCLR);
    }
#endif
#endif
}


static VOID _SIF_EnableAndClearInterrupt(VOID)
{
    if ((_fgIsrEnabled) && (_fgSifInit))
    {
        return;
    }

    _fgIsrEnabled = TRUE;

    IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M0_INTCLR, SIF_M0_INTCLR);
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    IO_WRITE32MSK(BIM_BASE, RW_PINTEN_REG, SIF_M0_INTEN, SIF_M0_INTEN);
#endif
#endif

    IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M1_INTCLR, SIF_M1_INTCLR);
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    IO_WRITE32MSK(BIM_BASE, RW_PINTEN_REG, SIF_M1_INTEN, SIF_M1_INTEN);
#endif
#endif

    if (_SIF_ISR_IsPdwncMasterEnable())
    {
        IO_WRITE32MSK(PDWNC_BASE, RW_AINTCLR_REG, SIF_PD_M0_INTCLR, SIF_PD_M0_INTCLR);
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
        IO_WRITE32MSK(PDWNC_BASE, RW_AINTEN_REG, SIF_PD_M0_INTEN, SIF_PD_M0_INTEN);
#endif
#endif
    }

    if (_SIF_ISR_IsPdwncMaster1Enable())
    {
        IO_WRITE32MSK(PDWNC_BASE, RW_AINTCLR_REG, SIF_PD_M1_INTCLR, SIF_PD_M1_INTCLR);
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
        IO_WRITE32MSK(PDWNC_BASE, RW_AINTEN_REG, SIF_PD_M1_INTEN, SIF_PD_M1_INTEN);
#endif
#endif
    }

#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
    if (_SIF_ISR_Is8295bMaster0Enable())
    {
        PCMCIA_RawWrite32MSK(RW_WINTCLR_REG, SIF_M4_INTCLR, SIF_M4_INTCLR);
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
        PCMCIA_RawWrite32MSK(RW_WINTEN_REG, SIF_M4_INTEN, SIF_M4_INTEN);
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI		
    }
    if (_SIF_ISR_Is8295bMaster1Enable())
    {
        PCMCIA_RawWrite32MSK(RW_WINTCLR_REG, SIF_M5_INTCLR, SIF_M5_INTCLR);
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
        PCMCIA_RawWrite32MSK(RW_WINTEN_REG, SIF_M5_INTEN, SIF_M5_INTEN);
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI			
    }
#endif
#endif

    VERIFY(BIM_ClearIrq(VECTOR_SERIAL));
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    VERIFY(BIM_EnableIrq(VECTOR_SERIAL));
#endif
#endif

}


// Send one start bit
static INT32 _SIF_PutStartBit(VOID)
{
    UINT32 u4Reg, u4Msk;

    _SIF_EnableAndClearInterrupt();

#ifdef CC_SIF_TRI_DELAY_SUPPORT
    _SIF_CheckAndDelay();
#endif // CC_SIF_TRI_DELAY_SUPPORT

    u4Reg = TRI | SIF_MODE(MODE_START);
    u4Msk = TRI_MSK | SIF_MODE_MSK;
    _SIF_WRITE32MSK(RW_SM1_CTRL1_REG, u4Reg, u4Msk);

    return _SIF_WaitHwDone();
}


// Send one stop bit
static INT32 _SIF_PutStopBit(VOID)
{
    UINT32 u4Reg, u4Msk;

    _SIF_EnableAndClearInterrupt();

#ifdef CC_SIF_TRI_DELAY_SUPPORT
    _SIF_CheckAndDelay();
#endif // CC_SIF_TRI_DELAY_SUPPORT

    u4Reg = TRI | SIF_MODE(MODE_STOP);
    u4Msk = TRI_MSK | SIF_MODE_MSK;
    _SIF_WRITE32MSK(RW_SM1_CTRL1_REG, u4Reg, u4Msk);

    return _SIF_WaitHwDone();
}


// Send one data byte
// Return: 1 success, 0 fail, <0 error.
static INT32 _SIF_PutByte(UINT8 u1Data)
{
    UINT32 u4Reg, u4Msk, u4Ack;
    INT32 i4Ret = 0;

#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    // Make sure SIF semaphore is in lock state.
    if (_SIF_ISR_TryLock() != OSR_WOULD_BLOCK)
    {
        Printf("SIF_PutByte ISR lock status error!\n");
        _SIF_TimeOutReset();
        return SIF_ERROR_DATA;
    }
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

    _SIF_EnableAndClearInterrupt();

    // fill data port
    _SIF_WRITE32(RW_SM1_D0_REG, u1Data);

#ifdef CC_SIF_TRI_DELAY_SUPPORT
    _SIF_CheckAndDelay();
#endif // CC_SIF_TRI_DELAY_SUPPORT

    // fill control word
    u4Reg = TRI | SIF_MODE(MODE_WR_DATA) | PGLEN(0);           // PGLEN(0) = 1 byte
    u4Msk = TRI_MSK | SIF_MODE_MSK | PGLEN_MSK;
    _SIF_WRITE32MSK(RW_SM1_CTRL1_REG, u4Reg, u4Msk);

    // wait for hardware
    i4Ret = _SIF_WaitHwDone();
    if (i4Ret != SIF_OK)
    {
        LOG(3, "SIF put byte: sema time out ");
        return i4Ret;
    }

    u4Ack = ACK(_SIF_READ32(RW_SM1_CTRL1_REG));

    return ((u4Ack & 0x01) ? 1 : 0);
}


#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
// Send multiple data bytes
// Return: how many bytes were written, 0 fail, <1 error
static INT32 _SIF_PutMultipleBytes(UINT8* pu1Data, INT32 i4Bytes)
{
    UINT32 u4Reg, u4Msk, u4Ack, u4D0, u4D1;
    INT32 i4Ret, i4RetTimeout, i, j, i4SplitBytes, i4TotalLoops;
#if SIF_USE_LENGTHER_PAGE
    UINT32 u4AckH, u4AckL, u4D2, u4D3, u4D4, u4D5, u4D6, u4D7, u4D8;
#endif
    ASSERT(pu1Data);

#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    // Make sure SIF semaphore is in lock state.
    if (_SIF_ISR_TryLock() != OSR_WOULD_BLOCK)
    {
        Printf("SIF_PutMultipleBytes ISR lock status error!\n");
        _SIF_TimeOutReset();
        return SIF_ERROR_DATA;
    }
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

    i4Ret = 0;
    i4RetTimeout = 0;


    i4TotalLoops = (i4Bytes + SIF_PAGE_SIZE - 1) / SIF_PAGE_SIZE;

    for (i = 0; i < i4TotalLoops; i++)
    {
        // Last <= SIF_PAGE_SIZE bytes
        if (i == (i4TotalLoops - 1))
        {
            i4SplitBytes = i4Bytes % SIF_PAGE_SIZE;
        }
        else
        {
            i4SplitBytes = (INT32) SIF_PAGE_SIZE;
        }

        if (i4SplitBytes == 0)
        {
            i4SplitBytes = (INT32) SIF_PAGE_SIZE;
        }

        u4D0 = 0;
        u4D1 = 0;
#if SIF_USE_LENGTHER_PAGE
        u4D2 = 0;
        u4D3 = 0;
        u4D4 = 0;
        u4D5 = 0;
        u4D6 = 0;
        u4D7 = 0;
        u4D8 = 0;
#endif
        // setup data bytes
#if SIF_USE_LENGTHER_PAGE
        for (j = 0; j < ((i4SplitBytes > 4) ? 4 : i4SplitBytes); j++)
        {
            u4D0 |= ((*(pu1Data + j) << (8 * j)) & (0xFF << (8 * j)));
        }

        if (i4SplitBytes > 4)
        {
            for (j = 0; j < ((i4SplitBytes > 8) ? 4 : (i4SplitBytes - 4)); j++)
            {
                u4D1 |= ((*(pu1Data + 4 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }

        if (i4SplitBytes > 8)
        {
            for (j = 0; j < ((i4SplitBytes > 12) ? 4 : (i4SplitBytes - 8)); j++)
            {
                u4D2 |= ((*(pu1Data + 8 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }

        if (i4SplitBytes > 12)
        {
            for (j = 0; j < ((i4SplitBytes > 16) ? 4 : (i4SplitBytes - 12)); j++)
            {
                u4D3 |= ((*(pu1Data + 12 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }

        if (i4SplitBytes > 16)
        {
            for (j = 0; j < ((i4SplitBytes > 20) ? 4 : (i4SplitBytes - 16)); j++)
            {
                u4D4 |= ((*(pu1Data + 16 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }

        if (i4SplitBytes > 20)
        {
            for (j = 0; j < ((i4SplitBytes > 24) ? 4 : (i4SplitBytes - 20)); j++)
            {
                u4D5 |= ((*(pu1Data + 20 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }

        if (i4SplitBytes > 24)
        {
            for (j = 0; j < ((i4SplitBytes > 28) ? 4 : (i4SplitBytes - 24)); j++)
            {
                u4D6 |= ((*(pu1Data + 24 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }

        if (i4SplitBytes > 28)
        {
            for (j = 0; j < ((i4SplitBytes > 32) ? 4 : (i4SplitBytes - 28)); j++)
            {
                u4D7 |= ((*(pu1Data + 28 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }

        if (i4SplitBytes > 32)
        {
            for (j = 0; j < (i4SplitBytes - 32); j++)
            {
                u4D8 |= ((*(pu1Data + 32 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }
#else
        for (j = 0; j < ((i4SplitBytes > 4) ? 4 : i4SplitBytes); j++)
        {
            u4D0 |= ((*(pu1Data + j) << (8 * j)) & (0xFF << (8 * j)));
        }

        if (i4SplitBytes > 4)
        {
            for (j = 0; j < (i4SplitBytes - 4); j++)
            {
                u4D1 |= ((*(pu1Data + 4 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }
#endif
        pu1Data += i4SplitBytes;

        _SIF_EnableAndClearInterrupt();

        // fill data port
        _SIF_WRITE32(RW_SM1_D0_REG, u4D0);
        _SIF_WRITE32(RW_SM1_D1_REG, u4D1);
#if SIF_USE_LENGTHER_PAGE
        _SIF_WRITE32(RW_SM1_D2_REG, u4D2);
        _SIF_WRITE32(RW_SM1_D3_REG, u4D3);
        _SIF_WRITE32(RW_SM1_D4_REG, u4D4);
        _SIF_WRITE32(RW_SM1_D5_REG, u4D5);
        _SIF_WRITE32(RW_SM1_D6_REG, u4D6);
        _SIF_WRITE32(RW_SM1_D7_REG, u4D7);
        _SIF_WRITE32(RW_SM1_D8_REG, u4D8);
#endif

#ifdef CC_SIF_TRI_DELAY_SUPPORT
        _SIF_CheckAndDelay();
#endif // CC_SIF_TRI_DELAY_SUPPORT

        // fill control word, use old mode
#if defined (USE_NEW_MODE)
        if (i4Bytes > 8)
        {
            u4Reg = TRI | SIF_MODE(MODE_WR_DATA) | PGLEN(i4SplitBytes - 1);

        }
        //use new mode
        else
        {
            u4Reg = TRI |  PGLEN(i4SplitBytes - 1);

        }
#else
        u4Reg = TRI | SIF_MODE(MODE_WR_DATA) | PGLEN(i4SplitBytes - 1);
#endif
        u4Msk = TRI_MSK | SIF_MODE_MSK | PGLEN_MSK;
        _SIF_WRITE32MSK(RW_SM1_CTRL1_REG, u4Reg, u4Msk);

        // wait for hardware
        i4RetTimeout = _SIF_WaitHwDone();
        if (i4RetTimeout != SIF_OK)
        {
            LOG(3, "SIF put mutiple byte: sema time out ");
            return i4RetTimeout;
        }
#if SIF_USE_LENGTHER_PAGE
        u4AckL = ACK(_SIF_READ32(RW_SM1_CTRL1_REG));		//from 0x0 ~ 0x0ffff
        u4AckH = ACKH(_SIF_READ32(RW_SM1_CTRL3_REG));		//from 0x0 ~ 0x0fffff
        u4Ack =  (u4AckH << 16) | u4AckL;					//from 0x0 ~ 0x0ffffffff

        if (u4AckH > 0x0FFFF)							//u4Ack > 32
        {
            if (u4AckL != 0x0FFFF)
            {
                LOG(0, "SIF put mutiple byte: Abnormal device ack sequence 0x%x.\n ", u4Ack);
                u4Ack = 0;
                ASSERT(0);
            }
            else
            {
                switch (u4AckH)
                {
                    case 0x01FFFF:
                        u4Ack = 33;
                        break;

                    case 0x03FFFF:
                        u4Ack = 34;
                        break;

                    case 0x07FFFF:
                        u4Ack = 35;
                        break;

                    case 0xFFFFF:
                        u4Ack = 36;
                        break;

                    default:
                        LOG(0, "SIF put mutiple byte: Abnormal device ack sequence 0x%x.\n ", u4Ack);
                        u4Ack = 0;
                        ASSERT(0);

                }
            }
        }

        if (u4AckH <= 0x0FFFF)
        {

            switch (u4Ack)
            {
                case 0x00:
                    u4Ack = 0;
                    break;

                case 0x01:
                    u4Ack = 1;
                    break;

                case 0x03:
                    u4Ack = 2;
                    break;

                case 0x07:
                    u4Ack = 3;
                    break;

                case 0x0F:
                    u4Ack = 4;
                    break;

                case 0x1F:
                    u4Ack = 5;
                    break;

                case 0x3F:
                    u4Ack = 6;
                    break;

                case 0x7F:
                    u4Ack = 7;
                    break;

                case 0xFF:
                    u4Ack = 8;
                    break;

                case 0x1FF:
                    u4Ack = 9;
                    break;

                case 0x3FF:
                    u4Ack = 10;
                    break;

                case 0x7FF:
                    u4Ack = 11;
                    break;

                case 0xFFF:
                    u4Ack = 12;
                    break;

                case 0x1FFF:
                    u4Ack = 13;
                    break;

                case 0x3FFF:
                    u4Ack = 14;
                    break;

                case 0x7FFF:
                    u4Ack = 15;
                    break;

                case 0xFFFF:
                    u4Ack = 16;
                    break;

                case 0x1FFFF:
                    u4Ack = 17;
                    break;

                case 0x3FFFF:
                    u4Ack = 18;
                    break;

                case 0x7FFFF:
                    u4Ack = 19;
                    break;

                case 0xFFFFF:
                    u4Ack = 20;
                    break;

                case 0x1FFFFF:
                    u4Ack = 21;
                    break;

                case 0x3FFFFF:
                    u4Ack = 22;
                    break;

                case 0x7FFFFF:
                    u4Ack = 23;
                    break;

                case 0xFFFFFF:
                    u4Ack = 24;
                    break;

                case 0x1FFFFFF:
                    u4Ack = 25;
                    break;

                case 0x3FFFFFF:
                    u4Ack = 26;
                    break;

                case 0x7FFFFFF:
                    u4Ack = 27;
                    break;

                case 0xFFFFFFF:
                    u4Ack = 28;
                    break;

                case 0x1FFFFFFF:
                    u4Ack = 29;
                    break;

                case 0x3FFFFFFF:
                    u4Ack = 30;
                    break;

                case 0x7FFFFFFF:
                    u4Ack = 31;
                    break;

                case 0xFFFFFFFF:
                    u4Ack = 32;
                    break;


                default:
                    LOG(0, "SIF put mutiple byte: Abnormal device ack sequence 0x%x.\n ", u4Ack);
                    u4Ack = 0;
                    ASSERT(0);
            }
        }
#else
        u4Ack = ACK(_SIF_READ32(RW_SM1_CTRL1_REG));
        u4Ack &= 0xFF;

        // count how many acks
        switch (u4Ack)
        {
            case 0x00:
                u4Ack = 0;
                break;

            case 0x01:
                u4Ack = 1;
                break;

            case 0x03:
                u4Ack = 2;
                break;

            case 0x07:
                u4Ack = 3;
                break;

            case 0x0F:
                u4Ack = 4;
                break;

            case 0x1F:
                u4Ack = 5;
                break;

            case 0x3F:
                u4Ack = 6;
                break;

            case 0x7F:
                u4Ack = 7;
                break;

            case 0xFF:
                u4Ack = 8;
                break;

            default:
                LOG(0, "SIF put mutiple byte: Abnormal device ack sequence 0x%x.\n ", u4Ack);
                u4Ack = 0;
                ASSERT(0);
        }
#endif

        i4Ret += u4Ack;

        if (u4Ack == 0)
        {
            return i4Ret;
        }
    }

    return i4Ret;
}
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS


#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
// Read multiple data bytes
// Return: how many bytes were read, 0 fail
static INT32 _SIF_GetMultipleBytes(UINT8* pu1Data, INT32 i4Bytes)
{
    UINT32 u4Reg, u4Msk, u4D0, u4D1;
    INT32 i4Ret, i4RetTimeout, i, j, i4SplitBytes, i4TotalLoops;
#if SIF_USE_LENGTHER_PAGE
    UINT32 u4D2, u4D3, u4D4, u4D5, u4D6, u4D7, u4D8;
#endif

    ASSERT(pu1Data);

#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    // Make sure SIF semaphore is in lock state.
    if (_SIF_ISR_TryLock() != OSR_WOULD_BLOCK)
    {
        Printf("SIF_GetMultipleBytes ISR lock status error!\n");
        _SIF_TimeOutReset();
        return SIF_ERROR_DATA;
    }
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

    i4Ret = 0;
    i4RetTimeout = 0;


    i4TotalLoops = (i4Bytes + SIF_PAGE_SIZE - 1) / SIF_PAGE_SIZE;

    for (i = 0; i < i4TotalLoops; i++)
    {
        // Last <= SIF_PAGE_SIZE bytes
        if (i == (i4TotalLoops - 1))
        {
            i4SplitBytes = i4Bytes % SIF_PAGE_SIZE;
        }
        else
        {
            i4SplitBytes = (INT32) SIF_PAGE_SIZE;
        }

        if (i4SplitBytes == 0)
        {
            i4SplitBytes = (INT32) SIF_PAGE_SIZE;
        }

        if (i == (i4TotalLoops - 1))
        {
#if defined (USE_NEW_MODE)
            //use old mode
            if (i4Bytes > 8)
            {
                u4Reg = TRI | SIF_MODE(MODE_RD_DATA_LAST) | PGLEN(i4SplitBytes - 1);
            }
            //use new mode
            else
            {
                u4Reg = TRI | PGLEN(i4SplitBytes - 1);
            }
#else
            u4Reg = TRI | SIF_MODE(MODE_RD_DATA_LAST) | PGLEN(i4SplitBytes - 1);
#endif
        }
        else
        {
            u4Reg = TRI | SIF_MODE(MODE_RD_DATA) | PGLEN(i4SplitBytes - 1);
        }

        _SIF_EnableAndClearInterrupt();

#ifdef CC_SIF_TRI_DELAY_SUPPORT
        _SIF_CheckAndDelay();
#endif // CC_SIF_TRI_DELAY_SUPPORT

        // fill control word
        u4Msk = TRI_MSK | SIF_MODE_MSK | PGLEN_MSK;
        _SIF_WRITE32MSK(RW_SM1_CTRL1_REG, u4Reg, u4Msk);

        // wait for hardware
        i4RetTimeout = _SIF_WaitHwDone();
        if (i4RetTimeout != SIF_OK)
        {
            LOG(3, "SIF get mutiple byte: sema time out ");
            return i4RetTimeout;
        }

        u4D0 = _SIF_READ32(RW_SM1_D0_REG);
        u4D1 = _SIF_READ32(RW_SM1_D1_REG);

#if SIF_USE_LENGTHER_PAGE
        u4D2 = _SIF_READ32(RW_SM1_D2_REG);
        u4D3 = _SIF_READ32(RW_SM1_D3_REG);
        u4D4 = _SIF_READ32(RW_SM1_D4_REG);
        u4D5 = _SIF_READ32(RW_SM1_D5_REG);
        u4D6 = _SIF_READ32(RW_SM1_D6_REG);
        u4D7 = _SIF_READ32(RW_SM1_D7_REG);
        u4D8 = _SIF_READ32(RW_SM1_D8_REG);
#endif
        // save to buffer
#if defined (USE_NEW_MODE)
        if (u4FLAG_Read_Second == 1)
        {
            for (j = 0; j < ((i4SplitBytes > 4) ? 4 : i4SplitBytes); j++)
            {
                *(pu1Data + 8) = (UINT8) (u4D0 & 0xFF);
                u4D0 >>= 8;
                pu1Data++;
                i4Ret++;
            }

            if (i4SplitBytes > 4)
            {
                for (j = 0; j < (i4SplitBytes - 4); j++)
                {
                    *(pu1Data + 8)  = (UINT8) (u4D1 & 0xFF);
                    u4D1 >>= 8;
                    pu1Data++;
                    i4Ret++;
                }
            }
        }
        else
        {
            for (j = 0; j < ((i4SplitBytes > 4) ? 4 : i4SplitBytes); j++)
            {
                *pu1Data = (UINT8) (u4D0 & 0xFF);
                u4D0 >>= 8;
                pu1Data++;
                i4Ret++;
            }

            if (i4SplitBytes > 4)
            {
                for (j = 0; j < (i4SplitBytes - 4); j++)
                {
                    *pu1Data = (UINT8) (u4D1 & 0xFF);
                    u4D1 >>= 8;
                    pu1Data++;
                    i4Ret++;
                }
            }

        }
#else

#if SIF_USE_LENGTHER_PAGE
        for (j = 0; j < ((i4SplitBytes > 4) ? 4 : i4SplitBytes); j++)
        {
            *pu1Data = (UINT8) (u4D0 & 0xFF);
            u4D0 >>= 8;
            pu1Data++;
            i4Ret++;
        }
        if (i4SplitBytes > 4)
        {
            for (j = 0; j < ((i4SplitBytes > 8) ? 4 : (i4SplitBytes - 4)); j++)
            {
                *pu1Data = (UINT8) (u4D1 & 0xFF);
                u4D1 >>= 8;
                pu1Data++;
                i4Ret++;
            }
        }

        if (i4SplitBytes > 8)
        {
            for (j = 0; j < ((i4SplitBytes > 12) ? 4 : (i4SplitBytes - 8)); j++)
            {
                *pu1Data = (UINT8) (u4D2 & 0xFF);
                u4D2 >>= 8;
                pu1Data++;
                i4Ret++;

            }
        }
        if (i4SplitBytes > 12)
        {
            for (j = 0; j < ((i4SplitBytes > 16) ? 4 : (i4SplitBytes - 12)); j++)
            {
                *pu1Data = (UINT8) (u4D3 & 0xFF);
                u4D3 >>= 8;
                pu1Data++;
                i4Ret++;

            }
        }
        if (i4SplitBytes > 16)
        {
            for (j = 0; j < ((i4SplitBytes > 20) ? 4 : (i4SplitBytes - 16)); j++)
            {
                *pu1Data = (UINT8) (u4D4 & 0xFF);
                u4D4 >>= 8;
                pu1Data++;
                i4Ret++;

            }
        }
        if (i4SplitBytes > 20)
        {
            for (j = 0; j < ((i4SplitBytes > 24) ? 4 : (i4SplitBytes - 20)); j++)
            {
                *pu1Data = (UINT8) (u4D5 & 0xFF);
                u4D5 >>= 8;
                pu1Data++;
                i4Ret++;

            }
        }
        if (i4SplitBytes > 24)
        {
            for (j = 0; j < ((i4SplitBytes > 28) ? 4 : (i4SplitBytes - 24)); j++)
            {
                *pu1Data = (UINT8) (u4D6 & 0xFF);
                u4D6 >>= 8;
                pu1Data++;
                i4Ret++;

            }
        }
        if (i4SplitBytes > 28)
        {
            for (j = 0; j < ((i4SplitBytes > 32) ? 4 : (i4SplitBytes - 28)); j++)
            {
                *pu1Data = (UINT8) (u4D7 & 0xFF);
                u4D7 >>= 8;
                pu1Data++;
                i4Ret++;

            }
        }

        if (i4SplitBytes > 32)
        {
            for (j = 0; j < (i4SplitBytes - 32); j++)
            {
                *pu1Data = (UINT8) (u4D8 & 0xFF);
                u4D8 >>= 8;
                pu1Data++;
                i4Ret++;
            }
        }

#else
        for (j = 0; j < ((i4SplitBytes > 4) ? 4 : i4SplitBytes); j++)
        {
            *pu1Data = (UINT8) (u4D0 & 0xFF);
            u4D0 >>= 8;
            pu1Data++;
            i4Ret++;
        }

        if (i4SplitBytes > 4)
        {
            for (j = 0; j < (i4SplitBytes - 4); j++)
            {
                *pu1Data = (UINT8) (u4D1 & 0xFF);
                u4D1 >>= 8;
                pu1Data++;
                i4Ret++;
            }
        }
#endif
#endif

    }

    return i4Ret;
}
#else // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
// Get one data byte from SIF
static INT32 _SIF_GetByte(UINT8* pu1Data, BOOL fgLast)
{
    UINT32 u4Reg, u4Msk;
    INT32 i4Ret = 0;

#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    // Make sure SIF semaphore is in lock state.
    if (_SIF_ISR_TryLock() != OSR_WOULD_BLOCK)
    {
        Printf("SIF_GetByte ISR lock status error!\n");
        _SIF_TimeOutReset();
        return SIF_ERROR_DATA;
    }
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

    ASSERT(pu1Data);

    _SIF_EnableAndClearInterrupt();

#ifdef CC_SIF_TRI_DELAY_SUPPORT
    _SIF_CheckAndDelay();
#endif // CC_SIF_TRI_DELAY_SUPPORT

    // fill control word
    if (fgLast == TRUE) //last byte
    {
        u4Reg = TRI | SIF_MODE(MODE_RD_DATA_LAST) | PGLEN(0);  // PGLEN(0) = 1 byte
        u4Msk = TRI_MSK | SIF_MODE_MSK | PGLEN_MSK;
        _SIF_WRITE32MSK(RW_SM1_CTRL1_REG, u4Reg, u4Msk);
    }
    else
    {
        u4Reg = TRI | SIF_MODE(MODE_RD_DATA) | PGLEN(0);       // PGLEN(0) = 1 byte
        u4Msk = TRI_MSK | SIF_MODE_MSK | PGLEN_MSK;
        _SIF_WRITE32MSK(RW_SM1_CTRL1_REG, u4Reg, u4Msk);
    }

    // wait for hardware
    i4Ret = _SIF_WaitHwDone();
    if (i4Ret != SIF_OK)
    {
        LOG(3, "SIF get byte: sema time out ");
        return i4Ret;
    }

    *pu1Data = _SIF_READ32(RW_SM1_D0_REG) & 0xff;
    return i4Ret;
}
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS


// Check SIF bus status
static SIF_BUS_STATUS_T _SIF_CheckBus (VOID)
{
    UINT32 u4Reg;
    SIF_BUS_STATUS_T tRet;

    x_memset((VOID*)(&tRet), 0, sizeof(SIF_BUS_STATUS_T));

    u4Reg = _SIF_READ32(RW_SM1_CTRL0_REG);
    tRet.SIF_BUS_SDA = (UINT8) ((u4Reg & SDA_STATE) ? 1 : 0);
    tRet.SIF_BUS_SCL = (UINT8) ((u4Reg & SCL_STATE) ? 1 : 0);

    return tRet;
};


/** _SIF_OptRead
 *  Read data with options
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1DataBuf  Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @param  bus_ctrl    Options for read
 *  @retval > 0         Actual number of bytes read.
 *  @retval = 0         Device acked
 *  @retval < 0         Error
 */
static INT32 _SIF_OptRead(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pu1DataBuf, UINT32 u4DataLen,
                          SIF_BUS_CONTROL_T bus_ctrl)
{
#ifndef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    BOOL fgLastByte = FALSE;
#endif
    INT32 i4Cnt = 0, i4Ret = 0;

    if (!(bus_ctrl & SIF_CTL_NO_START))
    {
        i4Ret = _SIF_PutStartBit();
        if (i4Ret != SIF_OK)                 // start
        {
            LOG(3, "_SIF_OptRead: Start Bit error, %d.\n", i4Ret);
            return i4Ret;
        }
        if (_SIF_PutByte(u1DevAddr) != 1)   // dev addr
        {
            LOG(3, "_SIF_OptRead: dev addr 0x%x does not ack.\n", u1DevAddr);

            // stop
            if (!(bus_ctrl & SIF_CTL_NO_STOP))
            {
                i4Ret = _SIF_PutStopBit();
                if ( i4Ret != SIF_OK)
                {
                    LOG(3, "_SIF_OptRead: Stop Bit error, %d.\n", i4Ret);
                    return i4Ret;
                }
            }

            return SIF_ERROR_NODEV;
        }
    }


    // data
#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    i4Cnt = _SIF_GetMultipleBytes(pu1DataBuf, (INT32) u4DataLen);
    if (i4Cnt != (INT32) u4DataLen)
    {
        LOG(5, "_SIF_OptRead: only %d byte were read, expected %d.\n", i4Cnt, u4DataLen);
    }
#else // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    while (i4Cnt < u4DataLen)
    {
        if (i4Cnt == (u4DataLen - 1))
        {
            fgLastByte = TRUE;
        }
        i4Ret = _SIF_GetByte((pu1DataBuf + i4Cnt), fgLastByte);
        if (i4Ret != SIF_OK)
        {
            LOG(5, "_SIF_OptRead: terminated at %d byte due to no ACK. return %d\n", i4Cnt, i4Ret);
            break;
        }
        // i4Cnt == Number of data bytes sent on bus
        i4Cnt += 1;
    }
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS

    // stop
    if (!(bus_ctrl & SIF_CTL_NO_STOP))
    {
        i4Ret = _SIF_PutStopBit();
        if (i4Ret != SIF_OK)
        {
            LOG(3, "_SIF_OptRead: Stop Bit error, %d.\n", i4Ret);
            return i4Ret;
        }
    }

    return i4Cnt;
}


/** _SIF_OptWrite
 *  Write data with options
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1DataBuf  Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @param  bus_ctrl    Options for read
 *  @retval > 0         Actual number of bytes (sub-address + data) written.
 *  @retval = 0         Device acked
 *  @retval < 0         Error
 */
static INT32 _SIF_OptWrite(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pu1DataBuf, UINT32 u4DataLen,
                           SIF_BUS_CONTROL_T bus_ctrl)
{
    INT32 i4Cnt = 0, i4Ret = 0;

    if (!(bus_ctrl & SIF_CTL_NO_START))
    {
        i4Ret = _SIF_PutStartBit();
        if (i4Ret != SIF_OK)                 // start
        {
            LOG(3, "_SIF_OptWrite: Start Bit error, %d.\n", i4Ret);
            return i4Ret;
        }

        if (_SIF_PutByte(u1DevAddr) != 1)   // dev addr
        {
            LOG(3, "_SIF_OptWrite: dev addr 0x%x does not ack.\n", u1DevAddr);

            // stop
            if (!(bus_ctrl & SIF_CTL_NO_STOP))
            {
                i4Ret = _SIF_PutStopBit();
                if (i4Ret != SIF_OK)
                {
                    LOG(3, "_SIF_OptWrite: Stop Bit error, %d.\n", i4Ret);
                    return i4Ret;
                }
            }

            return SIF_ERROR_NODEV;
        }
    }

#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    // data
    i4Cnt = _SIF_PutMultipleBytes(pu1DataBuf, (INT32) u4DataLen);
    if (i4Cnt != (INT32) u4DataLen)
    {
        LOG(5, "_SIF_OptWrite: only %d byte were written, expected %d.\n", i4Cnt, u4DataLen);
    }
#else // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    // data
    for (i4Cnt = 0; i4Cnt < u4DataLen; i4Cnt++)
    {
        if (_SIF_PutByte(*(pu1DataBuf + i4Cnt)) != 1)
        {
            LOG(5, "_SIF_OptWrite: terminated at %d byte due to no ACK.\n", i4Cnt);
            break;
        }
    }
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS

    // stop
    if (!(bus_ctrl & SIF_CTL_NO_STOP))
    {
        i4Ret = _SIF_PutStopBit();
        if (i4Ret != SIF_OK)
        {
            LOG(3, "_SIF_OptWrite: Stop Bit error, %d.\n", i4Ret);
            return i4Ret;
        }
    }

    return i4Cnt;
}

#if defined (USE_NEW_MODE)
/** _SIF_Read
 *  Read data with options
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1DataBuf  Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @param  bus_ctrl    Options for read
 *  @retval > 0         Actual number of bytes read.
 *  @retval = 0         Device acked
 *  @retval < 0         Error
 */
static INT32 _SIF_Read(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pu1DataBuf, UINT32 u4DataLen,
                       UINT8 Bus_Mode)
{
#ifndef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    BOOL fgLastByte = FALSE;
#endif
    UINT32 i4Cnt = 0;
    //INT32 i4Ret = 0;
    UINT32 u4Reg, u4Msk;
    if (Bus_Mode == 2)
    {
        u4Reg = (0x03 << 16);
        u4Msk = 0x7f0000;
        _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL1_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL1_REG));
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL3_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL3_REG));

    }
    else if (Bus_Mode == 3)
    {
        u4Reg = (0x05 << 16);
        u4Msk = 0x7f0000;
        _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL1_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL1_REG));
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL3_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL3_REG));

    }
    else if (Bus_Mode == 4)
    {
        u4Reg = (0x09 << 16);
        u4Msk = 0x7f0000;
        _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL1_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL1_REG));
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL3_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL3_REG));

    }
    else	//other mode
    {
        u4Reg = (0x00 << 16);
        u4Msk = 0x7f0000;
        _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);

    }
    // data
#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    i4Cnt = _SIF_GetMultipleBytes(pu1DataBuf, (INT32) u4DataLen);
    if (i4Cnt != (INT32) u4DataLen)
    {
        LOG(5, "_SIF_OptRead: only %d byte were read, expected %d.\n", i4Cnt, u4DataLen);
    }
#else // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    while (i4Cnt < u4DataLen)
    {
        LOG(3, "dd	3_SIF_Read CC_SIF_NOT_MULTIPLE_DATA_BYTE_ACCESS \n");
        if (i4Cnt == (u4DataLen - 1))
        {
            fgLastByte = TRUE;
        }
        i4Ret = _SIF_GetByte((pu1DataBuf + i4Cnt), fgLastByte);
        if (i4Ret != SIF_OK)
        {
            LOG(5, "_SIF_OptRead: terminated at %d byte due to no ACK. return %d\n", i4Cnt, i4Ret);
            break;
        }
        // i4Cnt == Number of data bytes sent on bus
        i4Cnt += 1;
    }
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS

    // stop
#if 0
    if (!(bus_ctrl & SIF_CTL_NO_STOP))
    {
        i4Ret = _SIF_PutStopBit();
        if (i4Ret != SIF_OK)
        {
            LOG(3, "_SIF_OptRead: Stop Bit error, %d.\n", i4Ret);
            return i4Ret;
        }
    }
#endif


    return i4Cnt;

}

/** _SIF_Write
 *  Write data with options
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1DataBuf  Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @param  Bus_Mode    Options mode for bus
 *  @retval > 0         Actual number of bytes (sub-address + data) written.
 *  @retval = 0         Device acked
 *  @retval < 0         Error
 */
static INT32 _SIF_Write(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pu1DataBuf, UINT32 u4DataLen,
                        UINT8 Bus_Mode)
{
    INT32 i4Cnt = 0;
    //INT32 i4Ret = 0;
    UINT32 u4Reg, u4Msk;
    if (Bus_Mode == 5)
    {
        //u4Reg = 0x00;
        //u4Msk = 0x70;
        //_SIF_WRITE32MSK(RW_SM0_CTRL1_REG, u4Reg, u4Msk);  //244

        u4Reg = (0x11 << 16);
        u4Msk = 0x7f0000;
        _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);  //284
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL1_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL1_REG));
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL3_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL3_REG));

    }
    else if (Bus_Mode == 6)
    {
        //u4Reg = 0x00;
        //u4Msk = 0x70;
        //_SIF_WRITE32MSK(RW_SM0_CTRL1_REG, u4Reg, u4Msk);  //244

        u4Reg = (0x21 << 16);
        u4Msk = 0x7f0000;
        _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);  //284
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL1_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL1_REG));
        LOG(3, "dd	_SIF_Write RW_SM0_CTRL3_REG = 0x%x \n", _SIF_READ32(RW_SM1_CTRL3_REG));

    }
    else if (Bus_Mode == 7)
    {
        u4Reg = (0x41 << 16);
        u4Msk = 0x7f0000;
        _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);  //284

    }
    else	//other mode
    {
        u4Reg = (0x00 << 16);
        u4Msk = 0x7f0000;
        _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);  //284

    }

#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    // data
    u4Reg = _SIF_READ32(RW_SM1_CTRL3_REG);
    LOG(5, "dd SIF_Write_CC_SIF_MULTIPLE_DATA_BYTE_ACCESS: u4regval = 0x%x ,Bus_Mode = %d , DataLented = %d.\n", u4Reg, Bus_Mode, u4DataLen);
    i4Cnt = _SIF_PutMultipleBytes(pu1DataBuf, (INT32) u4DataLen);
    LOG(5, "dd 2_SIF_Write: i4Cnt = %d ,DataLented = %d.\n", i4Cnt, u4DataLen);
    if (i4Cnt != (INT32) u4DataLen)
    {
        LOG(5, "_SIF_Write: only %d byte were written, expected %d.\n", i4Cnt, u4DataLen);
    }
#else // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    // data
    u4Reg = _SIF_READ32(RW_SM1_CTRL3_REG);
    LOG(5, "dd SIF_Write_NOT_CC_SIF_MULTIPLE_DATA_BYTE_ACCESS: u4regval = 0x%x ,Bus_Mode = %d , DataLented = %d.\n", u4Reg, Bus_Mode, u4DataLen);
    for (i4Cnt = 0; i4Cnt < u4DataLen; i4Cnt++)
    {
        if (_SIF_PutByte(*(pu1DataBuf + i4Cnt)) != 1)
        {
            LOG(5, "_SIF_Write: terminated at %d byte due to no ACK.\n", i4Cnt);
            break;
        }
    }
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    return i4Cnt;
}
#endif


// Fill control register
static VOID _SIF_Control(VOID)
{
    UINT32 u4Reg, u4Msk;

    ASSERT(_rSifDevParm.u2ClkDiv);
    if (_fgEnableSclStretch)
    {
        _rSifDevParm.u2ClkDiv -= ((SIF_DEGLITCHLEVEL) << 1);

        u4Reg = CLK_DIV(_rSifDevParm.u2ClkDiv);
        u4Reg |= SCL_STRETCH;
    }
    else
    {
        u4Reg = CLK_DIV(_rSifDevParm.u2ClkDiv);
    }


    if (_fgDisableOpenDrain)
    {
        u4Reg |= ODRAIN;
    }

#if 0
    u4Msk = (UINT32) (CLK_DIV_MSK | ODRAIN |  SCL_STRETCH);
#else
    u4Reg |= DEGLITCH_EN;
    u4Reg |= DEGLITCH_SET(SIF_DEGLITCHLEVEL);//DEGLITCH_SET(0x1f);
    u4Msk = (UINT32) (CLK_DIV_MSK | DEGLITCH_SET_MSK | ODRAIN |  SCL_STRETCH | DEGLITCH_EN );
#endif

    _SIF_WRITE32MSK(RW_SM1_CTRL0_REG, u4Reg, u4Msk);
}


/** _SIF_DrvRawRead
 *  Raw read SIF data
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pSUbAddr    Pointer to sub addresses
 *  @param  u4SubAddrLen  Sub address length
 *  @param  pData       Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @retval > 0         Actual number of data bytes read.
 *  @retval <= 0        Error
 */
static INT32 _SIF_DrvRawRead(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pSubAddr, UINT32 u4SubAddrLen,
                             UINT8* pData, UINT32 u4DataLen)
{
    INT32 i4Ret = SIF_ERROR_BUS_BUSY;
    SIF_BUS_STATUS_T BusSts;
    UINT8 u1BusNo = 0;
#if defined (USE_NEW_MODE)
    INT32 i4RetFirst, i4RetSecond;
    UINT32 u4Dev;
    UINT32 u4Reg, u4Msk;
#endif
    SIF_Init();

    if (_rSifDevParm.fgSystemI2C)
    {
        u1BusNo = 0;
    }
    else if (_rSifDevParm.fgSecondI2C)
    {
        u1BusNo = 2;
    }
    else if (_rSifDevParm.fgTuner)
    {
        u1BusNo = 1;
    }
    else if (_rSifDevParm.fgPdwncI2C)
    {
        u1BusNo = 3;
    }
    else if (_rSifDevParm.fgPdwncI2C1)
    {
        u1BusNo = 4;
    }

    if (_pfnSifCfgBeforeActionFunc != NULL)
    {

        _pfnSifCfgBeforeActionFunc(u1BusNo, u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, pData, u4DataLen, 1);
    }
    else
    {
        LOG(7, "_pfnSifCfgBeforeActionFunc is NULL\n");
    }

    _rSifDevParm.u2ClkDiv = u2ClkDiv;

    _SIF_Control();
#ifdef TRI_DELAY
    _u1CurrDevAddr = u1DevAddr;
#endif

    // Set Pin mux
    if (_pfnSifCfgPinSetFunc)
    {
        _pfnSifCfgPinSetFunc(u1BusNo);
    }

    // check bus status
    BusSts = _SIF_CheckBus();
    if ((BusSts.SIF_BUS_SCL == 0) || (BusSts.SIF_BUS_SDA == 0))
    {
        i4Ret = _SIF_PutStopBit();
        if (i4Ret != SIF_OK)
        {
            LOG(3, "_SIF_DrvRawRead: Stop Bit error, %d.\n", i4Ret);
            //can't return,because of after callback.
        }
        BusSts = _SIF_CheckBus();                                   // check bus status, again.
        if ((BusSts.SIF_BUS_SCL == 0) || (BusSts.SIF_BUS_SDA == 0)) // still bus busy...
        {
            i4Ret = SIF_ERROR_BUS_BUSY;                             // let's return error
        }
    }
#if defined (USE_NEW_MODE)

    // do ...
    if ((BusSts.SIF_BUS_SCL == 1) && (BusSts.SIF_BUS_SDA == 1)) 	// bus free
    {
        //u4DataLen > 16,use normal mode
        if (u4DataLen > 16)
        {
            //make sure new feasure mode can't be trig
            u4Reg = (0x0000 << 16);
            u4Msk = 0xff7f0000;
            _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);
            if (u4SubAddrLen)											  // sub-addr
            {
                i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, SIF_CTL_NO_STOP);
                if (i4Ret == (INT32) u4SubAddrLen)
                {
                    i4Ret = _SIF_OptRead(u2ClkDiv, u1DevAddr | 0x1, pData, u4DataLen, SIF_CTL_NORMAL);
                    if (u4DataLen != (UINT32) i4Ret)
                    {
                        i4Ret = SIF_ERROR_DATA;
                    }
                }
                else													// status != SIF_OK
                {
                    i4Ret = _SIF_PutStopBit();
                    if (i4Ret != SIF_OK)
                    {
                        LOG(3, "_SIF_DrvRawRead: Stop Bit error, %d.\n", i4Ret);
                        //can't return,because of after callback.
                    }
                    i4Ret = SIF_ERROR_SUBADDR;
                }
            }
            else														// No sub-addr
            {
                i4Ret = _SIF_OptRead(u2ClkDiv, u1DevAddr | 0x1, pData, u4DataLen, SIF_CTL_NORMAL);
                if (u4DataLen != (UINT32) i4Ret)
                {
                    i4Ret = SIF_ERROR_DATA;
                }
            }
        }
        else
        {
            //dev device address put in register RW_SM0_DEV_REG bit[31:24],read bit 24 is 1
            u4Dev = (UINT32)u1DevAddr | 0x1;
            u4Dev = (u4Dev) << 24 ;
            _SIF_WRITE32(RW_SM1_DEV_REG, u4Dev);
            if (u4SubAddrLen)											  // sub-addr
            {
                i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, SIF_CTL_NO_STOP);
                if (i4Ret == (INT32) u4SubAddrLen)
                {
                    i4Ret = _SIF_Read(u2ClkDiv, u1DevAddr | 0x1, pData, u4DataLen, 2);
                    if (u4DataLen != (INT32) i4Ret)
                    {
                        i4Ret = SIF_ERROR_DATA;
                    }
                }
                else													// status != SIF_OK
                {
                    i4Ret = _SIF_PutStopBit();
                    if (i4Ret != SIF_OK)
                    {
                        LOG(3, "_SIF_DrvRawRead: Stop Bit error, %d.\n", i4Ret);
                        //can't return,because of after callback.
                    }
                    i4Ret = SIF_ERROR_SUBADDR;
                }
            }
            else														// No sub-addr
            {
                //u4DataLen > 8 use mode 3 & 4 ,SM0C3[2] = 1,SM0C3[3] = 1
                if (u4DataLen > 8)
                {

                    i4RetFirst = _SIF_Read(u2ClkDiv, u1DevAddr | 0x1, pData, SIF_PAGE_SIZE, 3);
                    if (SIF_PAGE_SIZE != (UINT32) i4RetFirst)
                    {
                        i4Ret = SIF_ERROR_DATA;
                    }
                    u4FLAG_Read_Second = 1;
                    i4RetSecond = _SIF_Read(u2ClkDiv, u1DevAddr | 0x1, pData, u4DataLen - SIF_PAGE_SIZE, 4);
                    if ((u4DataLen - SIF_PAGE_SIZE) != (UINT32) i4RetSecond)
                    {
                        i4Ret = SIF_ERROR_DATA;
                    }
                    i4Ret = i4RetFirst + i4RetSecond;
                    u4FLAG_Read_Second = 0;
                }
                //u4DataLen < 8 use mode 2 ,SM0C3[1] = 1
                else
                {
                    i4Ret = _SIF_Read(u2ClkDiv, u1DevAddr | 0x1, pData, u4DataLen, 2);
                    if (u4DataLen != (UINT32) i4Ret)
                    {
                        i4Ret = SIF_ERROR_DATA;
                    }
                }
            }
        }
    }
#else
    //do
    if ((BusSts.SIF_BUS_SCL == 1) && (BusSts.SIF_BUS_SDA == 1)) 	// bus free
    {
        if (u4SubAddrLen)											  // sub-addr
        {
            i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, SIF_CTL_NO_STOP);
            if (i4Ret == (INT32) u4SubAddrLen)
            {
                i4Ret = _SIF_OptRead(u2ClkDiv, u1DevAddr | 0x1, pData, u4DataLen, SIF_CTL_NORMAL);
                if (u4DataLen != (UINT32) i4Ret)
                {
                    i4Ret = SIF_ERROR_DATA;
                }
            }
            else													// status != SIF_OK
            {
                i4Ret = _SIF_PutStopBit();
                if (i4Ret != SIF_OK)
                {
                    LOG(3, "_SIF_DrvRawRead: Stop Bit error, %d.\n", i4Ret);
                    //can't return,because of after callback.
                }
                i4Ret = SIF_ERROR_SUBADDR;
            }
        }
        else														// No sub-addr
        {
            i4Ret = _SIF_OptRead(u2ClkDiv, u1DevAddr | 0x1, pData, u4DataLen, SIF_CTL_NORMAL);
            if (u4DataLen != (UINT32) i4Ret)
            {
                i4Ret = SIF_ERROR_DATA;
            }
        }
    }

#endif
    if (_pfnSifCfgAfterActionFunc != NULL)
    {
        _pfnSifCfgAfterActionFunc(u1BusNo, u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, pData,  u4DataLen, 1);
    }
    else
    {
        LOG(7, "_pfnSifCfgAfterActionFunc is NULL\n");
    }
    return i4Ret;
}


/** _SIF_DrvRawWrite
 *  Raw write SIF data
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pSUbAddr    Pointer to sub addresses
 *  @param  u4SubAddrLen  Sub address length
 *  @param  pData       Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @retval > 0         Actual number of data bytes written.
 *  @retval <= 0        Error
 */
static INT32 _SIF_DrvRawWrite(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pSubAddr, UINT32 u4SubAddrLen,
                              UINT8* pData, UINT32 u4DataLen)
{
    INT32 i4Ret = SIF_ERROR_PARAM;
    SIF_BUS_STATUS_T BusSts;
    UINT8 u1BusNo = 0;
#if defined (USE_NEW_MODE)
    UINT32 u4Dev;
    UINT32 u4Reg, u4Msk;
#endif
    ASSERT(u4DataLen > 0);

    SIF_Init();

    if (_rSifDevParm.fgSystemI2C)
    {
        u1BusNo = 0;
    }
    else if (_rSifDevParm.fgSecondI2C)
    {
        u1BusNo = 2;
    }
    else if (_rSifDevParm.fgTuner)
    {
        u1BusNo = 1;
    }
    else if (_rSifDevParm.fgPdwncI2C)
    {
        u1BusNo = 3;
    }
    else if (_rSifDevParm.fgPdwncI2C1)
    {
        u1BusNo = 4;
    }

    if (_pfnSifCfgBeforeActionFunc != NULL)
    {
        _pfnSifCfgBeforeActionFunc(u1BusNo, u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, pData,  u4DataLen, 0);
    }
    else
    {
        LOG(7, "_pfnSifCfgBeforeActionFunc is NULL\n");
    }

    _rSifDevParm.u2ClkDiv = u2ClkDiv;

    _SIF_Control();
#ifdef TRI_DELAY
    _u1CurrDevAddr = u1DevAddr;
#endif

    // Set Pin mux
    if (_pfnSifCfgPinSetFunc)
    {
        _pfnSifCfgPinSetFunc(u1BusNo);
    }

    // check bus status
    BusSts = _SIF_CheckBus();
    if ((BusSts.SIF_BUS_SCL == 0) || (BusSts.SIF_BUS_SDA == 0))
    {
        i4Ret = _SIF_PutStopBit();
        if (i4Ret != SIF_OK)
        {
            LOG(3, "_SIF_DrvRawWrite: Stop Bit error, %d.\n", i4Ret);
            //can't return,because of after callback.
        }
        BusSts = _SIF_CheckBus();                                   // check bus status, again.
        if ((BusSts.SIF_BUS_SCL == 0) || (BusSts.SIF_BUS_SDA == 0)) // still bus busy...
        {
            i4Ret = SIF_ERROR_BUS_BUSY;                             // let's return error
        }
    }
#if defined (USE_NEW_MODE)
    if ((BusSts.SIF_BUS_SCL == 1) && (BusSts.SIF_BUS_SDA == 1)) 	// bus free
    {
        if (u4DataLen > 16) //nor right
        {
            //make sure new feasure mode can't be trig
            u4Reg = (0x0000 << 16);
            u4Msk = 0xff7f0000;
            _SIF_WRITE32MSK(RW_SM1_CTRL3_REG, u4Reg, u4Msk);  //284
            // sub address
            if (u4SubAddrLen)
            {
                //i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, SIF_CTL_NO_STOP);
                i4Ret = _SIF_Write(u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, 6);
                if (i4Ret != (INT32) u4SubAddrLen)
                {
                    i4Ret = _SIF_PutStopBit();
                    if (i4Ret != SIF_OK)
                    {
                        LOG(3, "_SIF_DrvRawWrite: Stop Bit error, %d.\n", i4Ret);
                        //can't return,because of after callback.
                    }
                    i4Ret = SIF_ERROR_SUBADDR;
                }
                else
                {
                    i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pData, u4DataLen, SIF_CTL_NO_START);
                    if (i4Ret != (INT32) u4DataLen)
                    {
                        i4Ret = SIF_ERROR_DATA;
                    }
                }
            }
            else
            {
                i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pData, u4DataLen, SIF_CTL_NORMAL);
                if (i4Ret != (INT32) u4DataLen)
                {
                    i4Ret = SIF_ERROR_DATA;
                }
            }

        }
        else
        {
            //dev device address put in register RW_SM0_DEV_REG bit[31:24]
            u4Dev = (UINT32)u1DevAddr;
            u4Dev = (u4Dev) << 24 ;
            _SIF_WRITE32(RW_SM1_DEV_REG, u4Dev);
            // sub address
            if (u4SubAddrLen)
            {
                LOG(3, "dd _SIF_DrvRawWrite RW_SM0_DEV_REG = 0x%x \n", _SIF_READ32(RW_SM1_DEV_REG));
                i4Ret = _SIF_Write(u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, 6);
                if (i4Ret != (INT32) u4SubAddrLen)
                {
                    i4Ret = _SIF_PutStopBit();
                    if (i4Ret != SIF_OK)
                    {
                        LOG(3, "_SIF_DrvRawWrite: Stop Bit error, %d.\n", i4Ret);
                        //can't return,because of after callback.
                    }
                    i4Ret = SIF_ERROR_SUBADDR;
                }
                else
                {
                    i4Ret = _SIF_Write(u2ClkDiv, u1DevAddr, pData, u4DataLen, 7);
                    if (i4Ret != (INT32) u4DataLen)
                    {
                        i4Ret = SIF_ERROR_DATA;
                    }
                }
            }
            else
            {

                //device address is at RW_SM0_DEV_REG bit[31:24]
                LOG(3, "dd	_SIF_DrvRawWrite no address RW_SM0_DEV_REG = 0x%x \n", _SIF_READ32(RW_SM1_DEV_REG));

                i4Ret = _SIF_Write(u2ClkDiv, u1DevAddr, pData, u4DataLen, 5);
                if (i4Ret != (INT32) u4DataLen)
                {
                    i4Ret = SIF_ERROR_DATA;
                }
            }

        }

    }
#else
    if ((BusSts.SIF_BUS_SCL == 1) && (BusSts.SIF_BUS_SDA == 1)) 	// bus free
    {
        if (u4SubAddrLen)
        {
            i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, SIF_CTL_NO_STOP);
            if (i4Ret != (INT32) u4SubAddrLen)
            {
                i4Ret = _SIF_PutStopBit();
                if (i4Ret != SIF_OK)
                {
                    LOG(3, "_SIF_DrvRawWrite: Stop Bit error, %d.\n", i4Ret);
                    //can't return,because of after callback.
                }
                i4Ret = SIF_ERROR_SUBADDR;
            }
            else
            {
                i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pData, u4DataLen, SIF_CTL_NO_START);
                if (i4Ret != (INT32) u4DataLen)
                {
                    i4Ret = SIF_ERROR_DATA;
                }
            }
        }
        else
        {
            i4Ret = _SIF_OptWrite(u2ClkDiv, u1DevAddr, pData, u4DataLen, SIF_CTL_NORMAL);
            if (i4Ret != (INT32) u4DataLen)
            {
                i4Ret = SIF_ERROR_DATA;
            }
        }
    }
#endif

    if (_pfnSifCfgAfterActionFunc != NULL)
    {
        _pfnSifCfgAfterActionFunc(u1BusNo, u2ClkDiv, u1DevAddr, pSubAddr, u4SubAddrLen, pData, u4DataLen, 0);
    }
    else
    {
        LOG(7, "_pfnSifCfgAfterActionFunc is NULL\n");
    }

    return i4Ret;
}


static INT32 _SIF_RawReadMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                                         UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT8 pu1SubAddr[36] = {0, 0, 0, 0, 0, 0, 0, 0};

    ASSERT(u1WordAddrNum <= 3);

    if (u1WordAddrNum == 1)
    {
        pu1SubAddr[0] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else if (u1WordAddrNum == 2)
    {
        pu1SubAddr[0] = (UINT8) ((u4WordAddr & 0x0000FF00) >> 8);
        pu1SubAddr[1] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else
    {
        pu1SubAddr[0] = (UINT8) ((u4WordAddr & 0x00FF0000) >> 16);
        pu1SubAddr[1] = (UINT8) ((u4WordAddr & 0x0000FF00) >> 8);
        pu1SubAddr[2] = (UINT8) ((u4WordAddr & 0x000000FF));
    }

    return _SIF_DrvRawRead(u2ClkDiv, u1DevAddr, (UINT8*)pu1SubAddr, u1WordAddrNum, pu1Buf, u2ByteCnt);
}


static INT32 _SIF_RawWriteMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum,
                                          UINT32 u4WordAddr, UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT8 pu1SubAddr[36] = {0, 0, 0, 0, 0, 0, 0, 0};

    ASSERT(u1WordAddrNum <= 3);

    if (u1WordAddrNum == 1)
    {
        pu1SubAddr[0] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else if (u1WordAddrNum == 2)
    {
        pu1SubAddr[0] = (UINT8) ((u4WordAddr & 0x0000FF00) >> 8);
        pu1SubAddr[1] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else
    {
        pu1SubAddr[0] = (UINT8) ((u4WordAddr & 0x00FF0000) >> 16);
        pu1SubAddr[1] = (UINT8) ((u4WordAddr & 0x0000FF00) >> 8);
        pu1SubAddr[2] = (UINT8) ((u4WordAddr & 0x000000FF));
    }

    return _SIF_DrvRawWrite(u2ClkDiv, u1DevAddr, (UINT8*)pu1SubAddr, u1WordAddrNum, pu1Buf, u2ByteCnt);
}


/** _SIF_X_RawRead
 *  Raw read data with channel ID, word address, of which length up to three, from SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes read.
 *  @retval < 0             Error
 */
static INT32 _SIF_X_RawRead(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum,
                            UINT32 u4WordAddr, UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    INT32 i4Ret = SIF_ERROR_PARAM;

    ASSERT(u1WordAddrNum < 4);
    ASSERT(pu1Buf);
    ASSERT(u2ByteCnt > 0);
    if (u1CHannelID == SIF_BUS_SYSTEM_I2C)
    {
        _rSifDevParm.fgSystemI2C = TRUE;
        i4Ret = _SIF_RawReadMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                            pu1Buf, u2ByteCnt);
        _rSifDevParm.fgSystemI2C = FALSE;
    }
    else if (u1CHannelID == SIF_BUS_TUNER_I2C)
    {
        _rSifDevParm.fgTuner = TRUE;
        i4Ret = _SIF_RawReadMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                            pu1Buf, u2ByteCnt);
        _rSifDevParm.fgTuner = FALSE;
    }
    else if (u1CHannelID == SIF_BUS_SECOND_I2C)
    {
        _rSifDevParm.fgSecondI2C = TRUE;
        i4Ret = _SIF_RawReadMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                            pu1Buf, u2ByteCnt);
        _rSifDevParm.fgSecondI2C = FALSE;
    }
    else if (u1CHannelID == SIF_BUS_PDWNC_I2C)
    {
        if (_SIF_ISR_IsPdwncMasterEnable())
        {
            _rSifDevParm.fgPdwncI2C = TRUE;
            i4Ret = _SIF_RawReadMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                                pu1Buf, u2ByteCnt);
            _rSifDevParm.fgPdwncI2C = FALSE;
        }
        else
        {
            i4Ret = SIF_ERROR_NODEV;
        }
    }
    else if (u1CHannelID == SIF_BUS_PDWNC_I2C_1)
    {
        if (_SIF_ISR_IsPdwncMaster1Enable())
        {
            _rSifDevParm.fgPdwncI2C1 = TRUE;
            i4Ret = _SIF_RawReadMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                                pu1Buf, u2ByteCnt);
            _rSifDevParm.fgPdwncI2C1 = FALSE;
        }
        else
        {
            i4Ret = SIF_ERROR_NODEV;
        }
    }
    else if (u1CHannelID == SIF_BUS_8295B_I2C_0)
    {
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
        if (_SIF_ISR_Is8295bMaster0Enable())
        {
            _rSifDevParm.fg8295BI2C0 = TRUE;
            i4Ret = _SIF_RawReadMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                                pu1Buf, u2ByteCnt);
            _rSifDevParm.fg8295BI2C0 = FALSE;
        }
#endif
#endif
    }
    else if (u1CHannelID == SIF_BUS_8295B_I2C_1)
    {
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
        if (_SIF_ISR_Is8295bMaster1Enable())
        {
            _rSifDevParm.fg8295BI2C1 = TRUE;
            i4Ret = _SIF_RawReadMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                                pu1Buf, u2ByteCnt);
            _rSifDevParm.fg8295BI2C1 = FALSE;
        }
#endif
#endif
    }

    return i4Ret;
}


static INT32 _SIF_X_RawWrite(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum,
                             UINT32 u4WordAddr, UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    INT32 i4Ret = SIF_ERROR_PARAM;

    ASSERT(u1WordAddrNum < 4);
    ASSERT(pu1Buf);
    ASSERT(u2ByteCnt > 0);

    if (u1CHannelID == SIF_BUS_SYSTEM_I2C)
    {
        _rSifDevParm.fgSystemI2C = TRUE;
        i4Ret = _SIF_RawWriteMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                             pu1Buf, u2ByteCnt);
        _rSifDevParm.fgSystemI2C = FALSE;
    }
    else if (u1CHannelID == SIF_BUS_TUNER_I2C)
    {
        _rSifDevParm.fgTuner = TRUE;
        i4Ret = _SIF_RawWriteMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                             pu1Buf, u2ByteCnt);
        _rSifDevParm.fgTuner = FALSE;
    }
    else if (u1CHannelID == SIF_BUS_SECOND_I2C)
    {
        _rSifDevParm.fgSecondI2C = TRUE;
        i4Ret = _SIF_RawWriteMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                             pu1Buf, u2ByteCnt);
        _rSifDevParm.fgSecondI2C = FALSE;
    }
    else if (u1CHannelID == SIF_BUS_PDWNC_I2C)
    {
        if (_SIF_ISR_IsPdwncMasterEnable())
        {
            _rSifDevParm.fgPdwncI2C = TRUE;
            i4Ret = _SIF_RawWriteMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                                 pu1Buf, u2ByteCnt);
            _rSifDevParm.fgPdwncI2C = FALSE;
        }
        else
        {
            i4Ret = SIF_ERROR_NODEV;
        }
    }
    else if (u1CHannelID == SIF_BUS_PDWNC_I2C_1)
    {
        if (_SIF_ISR_IsPdwncMaster1Enable())
        {
            _rSifDevParm.fgPdwncI2C1 = TRUE;
            i4Ret = _SIF_RawWriteMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                                 pu1Buf, u2ByteCnt);
            _rSifDevParm.fgPdwncI2C1 = FALSE;
        }
        else
        {
            i4Ret = SIF_ERROR_NODEV;
        }
    }
    else if (u1CHannelID == SIF_BUS_8295B_I2C_0)
    {
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
        if (_SIF_ISR_Is8295bMaster0Enable())
        {
            _rSifDevParm.fg8295BI2C0 = TRUE;
            i4Ret = _SIF_RawWriteMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                                 pu1Buf, u2ByteCnt);
            _rSifDevParm.fg8295BI2C0 = FALSE;
        }
        else
#endif
#endif
        {
            i4Ret = SIF_ERROR_NODEV;
        }
    }
    else if (u1CHannelID == SIF_BUS_8295B_I2C_1)
    {
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
        if (_SIF_ISR_Is8295bMaster1Enable())
        {
            _rSifDevParm.fg8295BI2C1 = TRUE;
            i4Ret = _SIF_RawWriteMultipleSubAddr(u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                                                 pu1Buf, u2ByteCnt);
            _rSifDevParm.fg8295BI2C1 = FALSE;
        }
        else
#endif
#endif
        {
            i4Ret = SIF_ERROR_NODEV;
        }
    }

    return i4Ret;
}


#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
static VOID _SIF_HandleISR(UINT16 u2Vector)
{
#ifdef CC_SIF_TRI_DELAY_SUPPORT
    _SIF_GetIsrTime();
#endif // CC_SIF_TRI_DELAY_SUPPORT
    if (u2Vector == VECTOR_SERIAL)
    {
        IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M0_INTCLR, SIF_M0_INTCLR);
        IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M1_INTCLR, SIF_M1_INTCLR);
        VERIFY(BIM_ClearIrq(VECTOR_SERIAL));

        // release semaphore
        VERIFY(_SIF_ISR_Unlock() == OSR_OK);
    }
    else if (u2Vector == PINTNO_SIF)
    {
        if (_SIF_ISR_IsPdwncMasterEnable())
        {
            IO_WRITE32MSK(PDWNC_BASE, RW_AINTCLR_REG, SIF_PD_M0_INTCLR, SIF_PD_M0_INTCLR);
        }
        // release semaphore
        VERIFY(_SIF_ISR_Unlock() == OSR_OK);
    }
    else if (u2Vector == PINTNO_SIF1)
    {
        if (_SIF_ISR_IsPdwncMaster1Enable())
        {
            IO_WRITE32MSK(PDWNC_BASE, RW_AINTCLR_REG, SIF_PD_M1_INTCLR, SIF_PD_M1_INTCLR);
        }
        // release semaphore
        VERIFY(_SIF_ISR_Unlock() == OSR_OK);
    }
#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
    else if (1)
    {
        if (_SIF_ISR_Is8295bMaster0Enable())
        {
            PCMCIA_RawWrite32MSK(RW_WINTCLR_REG, SIF_M4_INTCLR, SIF_M4_INTCLR);
        }
        // release semaphore
        VERIFY(_SIF_ISR_Unlock() == OSR_OK);
    }
    else if (1)
    {
        if (_SIF_ISR_Is8295bMaster1Enable())
        {
            PCMCIA_RawWrite32MSK(RW_WINTCLR_REG, SIF_M5_INTCLR, SIF_M5_INTCLR);
        }
        // release semaphore
        VERIFY(_SIF_ISR_Unlock() == OSR_OK);
    }
#endif
#endif
}
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
SIF_STRUCT_T* SIF_V1_GetStruct(void)
{
    return &_rSifStruct;
}

//-------------------------------------------------------------------------
/** SIF_Init
 *  Initialize SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID SIF_V1_Init(VOID)
{
    UINT32 u4Reg, u4Var;
#if defined(CC_MTK_LOADER)
#else
    UINT32 u4Ret;
#endif

    u4Reg = HalCriticalStart();

    if (_fgSifInit)
    {
        HalCriticalEnd(u4Reg);
        return;
    }

    if (!_fgSifInitStart)
    {
        _fgSifInitStart = TRUE;
    }
    else
    {
        _fgSifInitWait = TRUE;
    }
    HalCriticalEnd(u4Reg);

    if (!_fgSifInitWait)
    {
        _pfnSifCfgBeforeActionFunc = NULL;
        _pfnSifCfgAfterActionFunc = NULL;
#if defined(CC_MTK_LOADER)
        u4Var = DRVCUST_InitGet(eSifEnableSclStretch);
        _fgEnableSclStretch = u4Var;

        u4Var = DRVCUST_InitGet(eSifDisableOpenDrain);
        _fgDisableOpenDrain = u4Var;

        if (DRVCUST_InitQuery(eSifCfgBeforeAction, (UINT32*)(void*)&_pfnSifCfgBeforeActionFunc) != 0)
        {
            _pfnSifCfgBeforeActionFunc = NULL;
        }

        if (DRVCUST_InitQuery(eSifCfgAfterAction, (UINT32*)(void*)&_pfnSifCfgAfterActionFunc) != 0)
        {
            _pfnSifCfgAfterActionFunc = NULL;
        }
#else
        u4Ret = (UINT32) DRVCUST_OptQuery(eSifEnableSclStretch, &u4Var);
        if (u4Ret == 0)
        {
            _fgEnableSclStretch = u4Var;
        }

        u4Ret = (UINT32) DRVCUST_OptQuery(eSifDisableOpenDrain, &u4Var);
        if (u4Ret == 0)
        {
            _fgDisableOpenDrain = u4Var;
        }

        if (DRVCUST_OptQuery(eSifCfgBeforeAction, (UINT32*)(void*)&_pfnSifCfgBeforeActionFunc) != 0)
        {
            _pfnSifCfgBeforeActionFunc = NULL;
        }

        if (DRVCUST_OptQuery(eSifCfgAfterAction, (UINT32*)(void*)&_pfnSifCfgAfterActionFunc) != 0)
        {
            _pfnSifCfgAfterActionFunc = NULL;
        }
#endif // defined(CC_MTK_LOADER)

#ifdef CC_SIF_TRI_DELAY_SUPPORT
        if (DRVCUST_InitQuery(eSifCfgTRIDelay, (UINT32*)(void*)&u4Var) == 0)
        {
            _u4TRIDelay = u4Var; // Stored in micro seconds
        }

        if (DRVCUST_InitQuery(eSifCfgTRIDelayDevAddr, (UINT32*)(void*)&u4Var) == 0)
        {
            _u1TRIDelayDevAddr = (UINT8)u4Var;
        }
#endif
        if (DRVCUST_InitQuery(eSifCfgPinSetFunc, (UINT32*)(void*)&_pfnSifCfgPinSetFunc) != 0)
        {
            _pfnSifCfgPinSetFunc = NULL;
        }

        if (_fgEnableSclStretch && _fgDisableOpenDrain)
        {
            LOG(1, "SIF_Init: error, SCL stretch and non open-drain functions are both enabled!\n");
            LOG(1, "SIF_Init: disable SCL stretch!\n");
            ASSERT(0);
            _fgEnableSclStretch = FALSE;
        }

        LOG(7, "SIF_Init: SCL stretch enabled %d, open drain mode %d.\n",
            _fgEnableSclStretch, _fgDisableOpenDrain ? 0 : 1);

        _SIF_DisableAndClearInterrupt();

#if SIF_USE_OSAI
        // create semaphore
        VERIFY(x_sema_create(&_hSifUserSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif // SIF_USE_OSAI

        Printf("SIF_Master1: V1 design\n");
        //IO_WRITE32MSK(BIM_BASE, RW_PERMISC_REG, SIF0_VER, SIF0_VER);

        // Enable SIF masters
        IO_WRITE32MSK(BIM_BASE, RW_SM0_CTRL0_REG, SM_EN, SM_EN);
        IO_WRITE32MSK(BIM_BASE, RW_SM0_CTRL0_REG + 0x30, SM_EN, SM_EN);

        if (_SIF_ISR_IsPdwncMasterEnable())
        {
            IO_WRITE32MSK(PDWNC_BASE, RW_SM0_CTRL0_REG - IO_VAL - 0x80, SM_EN, SM_EN);

            IO_WRITE32MSK(PDWNC_BASE, RW_AINTEN_REG, SIF_PD_M0_INTEN, SIF_PD_M0_INTEN);
        }

        if (_SIF_ISR_IsPdwncMaster1Enable())
        {

            IO_WRITE32MSK(PDWNC_BASE, RW_SM0_CTRL0_REG - IO_VAL - 0x80 - 0x60, SM_EN, SM_EN);
            IO_WRITE32MSK(PDWNC_BASE, RW_AINTEN_REG, SIF_PD_M1_INTEN, SIF_PD_M1_INTEN);
        }

#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
        if (_SIF_ISR_Is8295bMaster0Enable())
        {
            PCMCIA_RawWrite32MSK((RW_SM0_CTRL0_REG + 0x75c0), SM_EN, SM_EN);
        }
        if (_SIF_ISR_Is8295bMaster1Enable())
        {
            PCMCIA_RawWrite32MSK((RW_SM0_CTRL0_REG + 0x79c0), SM_EN, SM_EN);
        }
#endif
#endif

#if SIF_USE_OSAI

        // Create thread semaphore
        VERIFY(x_sema_create(&_hSifAsyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

#endif // SIF_USE_OSAI

        // init local variable
        _fgSifInit = TRUE;

    }
    else
    {
        /* Wait until SIF_Init() finish */
        while (!_fgSifInit)
        {
            static UINT32 u4Count = 0;
            x_thread_delay(10);
            u4Count++;
            ASSERT(u4Count < 1000);
        }
    }
}


//-------------------------------------------------------------------------
/** SIF_Stop
 *  terminate SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
#ifdef CC_CLI
static VOID SIF_V1_Stop(VOID)
{
    UINT32 u4Reg;
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    x_os_isr_fct pfnOldIsr;
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

    u4Reg = HalCriticalStart();

    ASSERT(_fgSifInit);

    if (_fgSifInit)
    {
        _SIF_DisableAndClearInterrupt();

        // Disable SIF masters
        IO_WRITE32MSK(BIM_BASE, RW_SM0_CTRL0_REG, 0, SM_EN);
        IO_WRITE32MSK(BIM_BASE, RW_SM0_CTRL0_REG + 0x30, 0, SM_EN);
        if (_SIF_ISR_IsPdwncMasterEnable())
        {
            IO_WRITE32MSK(PDWNC_BASE, RW_SM0_CTRL0_REG - IO_VAL - 0x80, 0, SM_EN);
        }

        if (_SIF_ISR_IsPdwncMaster1Enable())
        {
            IO_WRITE32MSK(PDWNC_BASE, RW_SM0_CTRL0_REG - IO_VAL - 0x80 - 0x60, 0, SM_EN);
        }

#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
        if (_SIF_ISR_Is8295bMaster0Enable())
        {
            PCMCIA_RawWrite32MSK((RW_SM0_CTRL0_REG + 0x75c0), 0, SM_EN);
        }
        if (_SIF_ISR_Is8295bMaster1Enable())
        {
            PCMCIA_RawWrite32MSK((RW_SM0_CTRL0_REG + 0x79c0), 0, SM_EN);
        }
#endif
#endif

#if SIF_USE_OSAI
        // Delete semaphore
        VERIFY(x_sema_delete(_hSifUserSema) == OSR_OK);
#ifdef CC_SIF_USE_INTERRUPT_MODE
        // Unregister ISR
        VERIFY(x_reg_isr(VECTOR_SERIAL, NULL, &pfnOldIsr) == OSR_OK);
        if (_SIF_ISR_IsPdwncMasterEnable())
        {
            VERIFY(PDWNC_RegIsr(PINTNO_SIF, NULL, &pfnOldIsr) == PDWNC_OK);
        }

        if (_SIF_ISR_IsPdwncMaster1Enable())
        {
            VERIFY(PDWNC_RegIsr(PINTNO_SIF1, NULL, &pfnOldIsr) == PDWNC_OK);
        }


#ifndef CC_MTK_LOADER
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
        if ((_SIF_ISR_Is8295bMaster0Enable()) || (_SIF_ISR_Is8295bMaster1Enable()))
        {
            PCMCIA_RegIsr(PCMCIA_ISR_SIF, NULL, &pfnOldIsr);
        }
#endif
#endif

        // Delete semaphore
        _SIF_ISR_DeleteSema();
#endif // CC_SIF_USE_INTERRUPT_MODE
#endif // SIF_USE_OSAI

        _fgSifInit = FALSE;
    }

    HalCriticalEnd(u4Reg);
}
#endif


/** SIF_X_Read
 *  Read data with channel ID, word address, of which length up to three, from SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes read.
 *  @retval = 0             Error
 */
static UINT16 SIF_V1_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                            UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    INT32 i4Ret;
    _SIF_UserLock();
    i4Ret = _SIF_X_RawRead(u1CHannelID, u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                           pu1Buf, u2ByteCnt);
    _SIF_UserUnlock();
    if (i4Ret > 0)
    {
        return (UINT16) i4Ret;
    }
    else
    {
        return 0;
    }
}

UINT16 SIF_V1_X_Sp_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                        UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    printf("....use V1, the code is not prepar");
    u2Ret = u2ByteCnt;
    return u2Ret;
}

/** SIF_X_Write
 *  Write data with channel ID, word address, of which length up to three, to SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to write. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes written.
 *  @retval = 0             Error
 */
static UINT16 SIF_V1_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                             UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    INT32 i4Ret;

    _SIF_UserLock();
    i4Ret = _SIF_X_RawWrite(u1CHannelID, u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                            pu1Buf, u2ByteCnt);
    _SIF_UserUnlock();

    if (i4Ret > 0)
    {
        return (UINT16) i4Ret;
    }
    else
    {
        return 0;
    }
}

static VOID SIF_V1_ISRHandler(UINT16 u2Vector)
{
#if SIF_USE_OSAI
#ifdef CC_SIF_USE_INTERRUPT_MODE
    _SIF_HandleISR(u2Vector);
#endif
#endif
}

#if 0 // Do not use async
#if SIF_USE_OSAI
//-------------------------------------------------------------------------
/** _SIF_AsyncThread
 *  Dedicated thread for async read/write requests.
 *  @param  VOID
 *  @return   VOID
 */
//-------------------------------------------------------------------------
static VOID _SIF_AsyncThread(VOID)
{

    UINT16 u2MsgQIdx, u2Result = 0;
    SIZE_T zMsgSize;
    SIF_ASYNC_REQ_T rReq;

    while (1)
    {
        zMsgSize = sizeof(SIF_ASYNC_REQ_T);

        // receive request message.
        VERIFY(x_msg_q_receive(&u2MsgQIdx, &rReq, &zMsgSize, &_hSifAsyncMsg, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
        ASSERT(zMsgSize == sizeof(SIF_ASYNC_REQ_T));

        VERIFY(x_sema_lock(_hSifAsyncSema, X_SEMA_OPTION_WAIT) == OSR_OK);

        if (rReq.u1RequestType == SIF_ASYNC_REQ_READ)
        {
            LOG(7, "AsyncThread call SIF_X_Read: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", rReq.u1CHannelID, rReq.u2ClkDiv,
                rReq.u1DevAddr, rReq.u1WordAddrNum, rReq.u4WordAddr, rReq.u2ByteCnt);

            u2Result = SIF_X_Read(rReq.u1CHannelID, rReq.u2ClkDiv, rReq.u1DevAddr, rReq.u1WordAddrNum,
                                  rReq.u4WordAddr, rReq.pu1Buf, rReq.u2ByteCnt);
        }
        else if (rReq.u1RequestType == SIF_ASYNC_REQ_WRITE)
        {
            LOG(7, "AsyncThread call SIF_X_Write: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", rReq.u1CHannelID, rReq.u2ClkDiv,
                rReq.u1DevAddr, rReq.u1WordAddrNum, rReq.u4WordAddr, rReq.u2ByteCnt);

            u2Result = SIF_X_Write(rReq.u1CHannelID, rReq.u2ClkDiv, rReq.u1DevAddr, rReq.u1WordAddrNum,
                                   rReq.u4WordAddr, rReq.pu1Buf, rReq.u2ByteCnt);
        }

        VERIFY(x_sema_unlock(_hSifAsyncSema) == OSR_OK);

        if (rReq.pfnNfy)
        {
            // call user defined callback function
            rReq.pfnNfy(rReq.pvNfyData, u2Result);
        }
    }

}

/** SIF_AsyncRead
 *  Read data with channel ID, word address, of which length up to three, from SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @param  pfnNfy       Callback function pointer.
 *  @param  pvNfyData       Callback function parameter
 *  @retval > 0             Success
 *  @retval = 0             Error
 */
UINT16 SIF_V2_X_AsyncRead(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                          UINT8* pu1Buf, UINT16 u2ByteCnt, SIF_ASYNC_NFY_FCT pfnNfy, VOID* pvNfyData)
{
    SIF_ASYNC_REQ_T rReq;

    if ( (u1WordAddrNum >= 4) || (!pu1Buf) || (u2ByteCnt <= 0) || (u1CHannelID > 2) )
    {
        Printf("Incorrect parameter!\n");
        return 0;
    }

    rReq.u1RequestType = SIF_ASYNC_REQ_READ;
    rReq.u1CHannelID = u1CHannelID;
    rReq.u2ClkDiv = u2ClkDiv;
    rReq.u1DevAddr = u1DevAddr;
    rReq.u1WordAddrNum = u1WordAddrNum;
    rReq.u4WordAddr = u4WordAddr;
    rReq.pu1Buf = pu1Buf;
    rReq.u2ByteCnt = u2ByteCnt;
    rReq.pfnNfy = pfnNfy;
    rReq.pvNfyData = pvNfyData;

    LOG(7, "Send request to AsyncThread for read: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", rReq.u1CHannelID, rReq.u2ClkDiv,
        rReq.u1DevAddr, rReq.u1WordAddrNum, rReq.u4WordAddr, rReq.u2ByteCnt);

    // Send request to aync thread
    if (x_msg_q_send(_hSifAsyncMsg, &rReq, sizeof(SIF_ASYNC_REQ_T), 1) != OSR_OK)
    {
        Printf("SIF_X_AsyncRead fail!\n");
        return 0;
    }

    return 1;
}

/** SIF_X_AsyncWrite
 *  Write data with channel ID, word address, of which length up to three, to SIF module.
 *  @param  u1ChannelID     SIF bus ID.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to write. Max = 256 bytes.
 *  @param  pfnNfy       Callback function pointer.
 *  @param  pvNfyData       Callback function parameter
 *  @retval > 0             Success
 *  @retval = 0             Error
 */
UINT16 SIF_V2_X_AsyncWrite(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                           UINT8* pu1Buf, UINT16 u2ByteCnt, SIF_ASYNC_NFY_FCT pfnNfy, VOID* pvNfyData)
{
    SIF_ASYNC_REQ_T rReq;

    if ( (u1WordAddrNum >= 4) || (!pu1Buf) || (u2ByteCnt <= 0) || (u1CHannelID > 2) )
    {
        Printf("Incorrect parameter!\n");
        return 0;
    }

    rReq.u1RequestType = SIF_ASYNC_REQ_WRITE;
    rReq.u1CHannelID = u1CHannelID;
    rReq.u2ClkDiv = u2ClkDiv;
    rReq.u1DevAddr = u1DevAddr;
    rReq.u1WordAddrNum = u1WordAddrNum;
    rReq.u4WordAddr = u4WordAddr;
    rReq.pu1Buf = pu1Buf;
    rReq.u2ByteCnt = u2ByteCnt;
    rReq.pfnNfy = pfnNfy;
    rReq.pvNfyData = pvNfyData;

    LOG(7, "Send request to AsyncThread for write: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", rReq.u1CHannelID, rReq.u2ClkDiv,
        rReq.u1DevAddr, rReq.u1WordAddrNum, rReq.u4WordAddr, rReq.u2ByteCnt);

    // Send request to aync thread
    if (x_msg_q_send(_hSifAsyncMsg, &rReq, sizeof(SIF_ASYNC_REQ_T), 1) != OSR_OK)
    {
        Printf("SIF_X_AsyncWrite fail!\n");
        return 0;
    }

    return 1;
}
#endif  // SIF_USE_OSAI
#endif

INT32 SIF_V1_Diag(VOID)
{
    UINT32 u4Reg;
#ifdef CC_MT5387
    Printf("This is mt5387\n");
#endif
    u4Reg = _SIF_READ32(RW_SM1_CTRL0_REG);

    Printf("Master 0 %s\n", (u4Reg | SM_EN) ? "Enable" : "Disable");

    Printf("Bus clock pull high by: %s \n",
           (u4Reg | ODRAIN) ? "Master" : "Open Drain (pull-high register");

    Printf("SCL-Stretch %s\n", (u4Reg | SCL_STRETCH) ? "On" : "Off");

    Printf("SCL= %s, SDA= %s\n", (u4Reg | SCL_STATE) ? "High" : "Low",
           (u4Reg | SDA_STATE) ? "High" : "Low");

    return 0;
}


