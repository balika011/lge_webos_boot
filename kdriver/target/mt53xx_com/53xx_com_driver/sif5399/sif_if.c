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
 * $RCSfile: sif_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_if.c
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
#include "x_hal_5381.h"

#include "x_pdwnc.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
/// Use OSAI function option
#if defined (CC_MTK_LOADER)
#define SIF_USE_OSAI            0
#ifdef CC_SIF_USE_INTERRUPT_MODE
#undef CC_SIF_USE_INTERRUPT_MODE
#endif // CC_SIF_USE_INTERRUPT_MODE
#else // defined (CC_MTK_LOADER)
#define SIF_USE_OSAI            1
#endif // defined (CC_MTK_LOADER)


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define  SIF_OK    (0)


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
EXTERN SIF_STRUCT_T* SIF_V1_GetStruct(void);
EXTERN SIF_STRUCT_T* SIF_V2_GetStruct(void);


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static SIF_STRUCT_T* _aprSifStruct[SIF_MAX_NUM_BUS] = {NULL, NULL, NULL, NULL, NULL, NULL};
static SIF_STRUCT_T* _prCurrentActiveSifStruct = NULL;
static BOOL _fgInit = FALSE;
static BOOL _fgInitStart       = FALSE;
static BOOL _fgInitWait        = FALSE;

#if SIF_USE_OSAI
/// SIF multi-task protection semaphore.
static HANDLE_T _hSifLockSema     = NULL_HANDLE;
#endif

static SIF_CONFIG_T* _prSifConfig = NULL;
static PFN_SIF_CFG_GET_CONFIG_FUNC _pfnSifCfgGetConfigFunc = NULL;

// This flag will be set upon standby request to avoid SIF opeartion being interrupted
static BOOL _fgSIFDisabled = FALSE;


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
// Lock SIF semaphore
static VOID _SIF_Lock(VOID)
{
    SIF_Init();
#if SIF_USE_OSAI
    // wait semaphore
    VERIFY(x_sema_lock(_hSifLockSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif // SIF_USE_OSAI
}


// Unlock SIF semaphore
static VOID _SIF_Unlock(VOID)
{
#if SIF_USE_OSAI
    // release semaphore
    VERIFY(x_sema_unlock(_hSifLockSema) == OSR_OK);
#endif //SIF_USE_OSAI
}

// Lock SIF semaphore
static VOID _SIF_LockTimeout(UINT32 u4Timeout)
{
    SIF_Init();
#if SIF_USE_OSAI
    // wait semaphore
    VERIFY(x_sema_lock_timeout(_hSifLockSema, u4Timeout) == OSR_OK);
#endif // SIF_USE_OSAI
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
SIF_STRUCT_T* _SIF_GetActiveChannel(void)
{
    return _prCurrentActiveSifStruct;
}

void _SIF_SetActiveChannel(SIF_STRUCT_T* prStruct)
{
    _prCurrentActiveSifStruct = prStruct;
}

#if defined(CC_FAST_INIT)
extern VOID _SIF_DisableAndClearInterrupt(VOID);
//LH for suspend/resume
void SIF_pm_suspend(void)
{
}
void SIF_pm_resume(void)
{
    Printf("SIF_Master: init\n");

    _SIF_DisableAndClearInterrupt();

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

}
#endif

#ifdef SIF_CHECK_SDA_LOW
// Set each bus as SIF function(Oryx)
void SIF_PinSetFunc(UINT8 u1Channel)
{
    UINT32 u4Reg = 0;
#ifndef CC_MTK_LOADER
    CRIT_STATE_T rCrit;
    rCrit = x_crit_start();
#endif
    if (u1Channel == 0)	//osda0/oscl0, use V2 engine
    {
        u4Reg = CKGEN_READ32(0x600);
        //  [21:20]  != 2; [19:18] = 1
        if ((u4Reg & 0x00300000) == 0x00200000)
        {
            u4Reg &= ~(0x00300000);
        }
        u4Reg &= ~(0x000C0000);
        u4Reg |= 0x00040000;
        CKGEN_WRITE32(0x600, u4Reg);
        u4Reg = CKGEN_READ32(0x604);
        // [1:0] != 1
        if ((u4Reg & 0x00000003) == 0x00000001)
        {
            u4Reg &= ~(0x00000003);
        }
        CKGEN_WRITE32(0x604, u4Reg);
    }
    else if (u1Channel == 1)	//osda2/oscl2 for tuner, use V2 engine
    {
        u4Reg = CKGEN_READ32(0x600);
        // [21:20] != 2 ; [19:18]  != 1
        if ((u4Reg & 0x00300000) == 0x00200000)
        {
            u4Reg &= ~(0x00300000);
        }
        if ((u4Reg & 0x000C0000) == 0x00040000)
        {
            u4Reg &= ~(0x000C0000);
        }
        CKGEN_WRITE32(0x600, u4Reg);
        u4Reg = CKGEN_READ32(0x604);
        // [1:0] = 1
        u4Reg &= ~(0x00000003);
        u4Reg |= 0x00000001;
        CKGEN_WRITE32(0x604, u4Reg);
    }
    else if (u1Channel == 2)	//osda1/oscl1, use V2 engine
    {
        u4Reg = CKGEN_READ32(0x600);
        // [21:20] = 2; [19:18]  != 1
        u4Reg &= ~(0x00300000);
        u4Reg |= 0x00200000;
        if ((u4Reg & 0x000C0000) == 0x00040000)
        {
            u4Reg &= ~(0x000C0000);
        }
        CKGEN_WRITE32(0x600, u4Reg);
        u4Reg = CKGEN_READ32(0x604);
        // [1:0]  != 1
        if ((u4Reg & 0x00000003) == 0x00000001)
        {
            u4Reg &= ~(0x00000003);
        }
        CKGEN_WRITE32(0x604, u4Reg);
    }

#ifndef CC_MTK_LOADER
    x_crit_end(rCrit);
#endif
}

/*
* 5890(Oryx)    (SIF0, SIF1, SIF2)
*/
void SIF_CheckEachBusSDA(void)
{
    SIF_BUS_STATUS_T Bus_Status;
    UINT32 u4CheckTime;
    UINT32 u4BusIndex;
    UINT32 u4Error;

    for (u4BusIndex = 0; u4BusIndex < 3; u4BusIndex++)
    {
        SIF_PinSetFunc(u4BusIndex);//
        Bus_Status = SIF_CheckBusStatus();
        for (u4CheckTime = 0; u4CheckTime < 5; u4CheckTime++)
        {
            if (Bus_Status.SIF_BUS_SDA == 0) //SDA is low
            {
                x_thread_delay(1);// 100KHz means 0.01ms 1T. 20*0.01 = 0.2
                // send a STOP
                SIF_SendStopBit();

                //check the bus again
                Bus_Status = SIF_CheckBusStatus();
                if (Bus_Status.SIF_BUS_SDA == 0) //SDA is still low
                {
                    Printf("[SIF]:SDA is low\n");
                }
                else
                {
                    Printf("[SIF]:SDA is high\n");
                    break;
                }
            }
        }
    }

    Bus_Status = SIF_CheckBusStatus();
    if (Bus_Status.SIF_BUS_SDA == 0) //SDA is low at last
    {
        // Error conditions
    }
}
#endif

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** SIF_Init
 *  Initialize SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID SIF_Init(VOID)
{
    //    UINT16 u2Ret = 0;
    UINT32 i;
    UINT32 u4Reg;

    u4Reg = HalCriticalStart();

    if (_fgInit)
    {
        HalCriticalEnd(u4Reg);
        return;
    }

    if (!_fgInitStart)
    {
        _fgInitStart = TRUE;
    }
    else
    {
        _fgInitWait = TRUE;
    }
    HalCriticalEnd(u4Reg);
    /*cobra eco IC master0 new mode will use 0xf0029284 bit31:24 & 22:16 ,so this register must be 0 at first*/
    //if(BSP_GetIcVersion() == IC_VER_5398_AB)
    {
        IO_WRITE32MSK(BIM_BASE, RW_SM0_DEV_REG, 0x0, 0xffffffff);
    }

    if (!_fgInitWait)
    {
#if SIF_USE_OSAI
        // create semaphore
        VERIFY(x_sema_create(&_hSifLockSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif // SIF_USE_OSAI

        // TODO: Query Bus Configuration and save to u4Numbus & au4BusVer
        if ((DRVCUST_InitQuery(eSifCfgGetConfigFunc, (UINT32*)(void*)&_pfnSifCfgGetConfigFunc) == 0) &&
            (_pfnSifCfgGetConfigFunc != NULL) )
        {
            _prSifConfig = _pfnSifCfgGetConfigFunc();
        }

        _SIF_ISR_Init();

        for (i = 0; i < (_prSifConfig->u1NumBus > SIF_MAX_NUM_BUS ? SIF_MAX_NUM_BUS : _prSifConfig->u1NumBus); i++)
        {
            if (_prSifConfig->au1HwVersion[i] == 2)
            {
                _aprSifStruct[i] = SIF_V2_GetStruct();
                _aprSifStruct[i]->rSifOps.pfn_Init();
            }
            else if (_prSifConfig->au1HwVersion[i] == 1)
            {
                _aprSifStruct[i] = SIF_V1_GetStruct();
                _aprSifStruct[i]->rSifOps.pfn_Init();
            }
            else
            {
                Printf("Un-recognized H/W version: %d\n", _prSifConfig->au1HwVersion[i]);
                VERIFY(0);
            }
        }
        _fgInit = TRUE;

    }
    else
    {
        /* Wait until SIF_Init() finish */
        while (!_fgInit)
        {
            static UINT32 u4Count = 0;
            x_thread_delay(10);
            u4Count++;
            ASSERT(u4Count < 1000);
        }
    }
#ifdef CC_SIF_OUTPUT_LOW_WAITING
    //change the output level L when SIF master in waiting state
    {
        UINT32 u4Tmp;
        u4Tmp = *((volatile unsigned int*)0xF0029240);
        u4Tmp &= ~(1 << 6); //clear bit6
        *((volatile unsigned int*)0xF0029240) = u4Tmp;
    }
#endif

#ifdef SIF_CHECK_SDA_LOW
    SIF_CheckEachBusSDA();
#endif

}


//-------------------------------------------------------------------------
/** SIF_Stop
 *  terminate SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
#ifdef CC_CLI
VOID SIF_Stop(VOID)
{
    UINT32 i;

    // TODO: Query Bus Configuration and save to u4Numbus & au4BusVer
    if ((DRVCUST_InitQuery(eSifCfgGetConfigFunc, (UINT32*)(void*)&_pfnSifCfgGetConfigFunc) == 0) &&
        (_pfnSifCfgGetConfigFunc != NULL) )
    {
        _prSifConfig = _pfnSifCfgGetConfigFunc();
    }

    for (i = 0; i < (_prSifConfig->u1NumBus > SIF_MAX_NUM_BUS ? SIF_MAX_NUM_BUS : _prSifConfig->u1NumBus); i++)
    {
        _aprSifStruct[i]->rSifOps.pfn_Stop();
    }
}
#endif

UINT16 SIF_Read(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
                UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;

    u2Ret = SIF_X_Read(SIF_BUS_SYSTEM_I2C, u2ClkDiv, u1DevAddr, 1, u1WordAddr,
                       pu1Buf, u2ByteCnt);
    return u2Ret;
}


/** SIF_ReadNoSubAddr
 *  Read data without word address from SIF module from SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to read. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes read.
 *  @retval = 0         Error
 */
UINT16 SIF_ReadNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pu1Buf,
                         UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    u2Ret = SIF_X_Read(SIF_BUS_SYSTEM_I2C, u2ClkDiv, u1DevAddr, 0, 0,
                       pu1Buf, u2ByteCnt);
    return u2Ret;
}


/** SIF_TunerReadNoSubAddr
 *  NIM module api: read data without word address from SIF module from SIF_BUS_TUNER_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to read. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes read.
 *  @retval = 0         Error
 */
UINT16 SIF_TunerReadNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pu1Buf,
                              UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    u2Ret = SIF_X_Read(SIF_BUS_TUNER_I2C, u2ClkDiv, u1DevAddr, 0, 0,
                       pu1Buf, u2ByteCnt);
    return u2Ret;
}


/** SIF_Write
 *  Write data with word address to SIF module on SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  u1WordAddr  Serial interface word address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to write. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes written.
 *  @retval = 0         Error
 */
UINT16 SIF_Write(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
                 UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    u2Ret = SIF_X_Write(SIF_BUS_SYSTEM_I2C, u2ClkDiv, u1DevAddr, 1, u1WordAddr,
                        pu1Buf, u2ByteCnt);
    return u2Ret;
}


/** SIF_WriteNoSubAddr
 *  Write data without word address to SIF module on SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to write. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes written.
 *  @retval = 0         Error
 */
UINT16 SIF_WriteNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pu1Buf,
                          UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    u2Ret = SIF_X_Write(SIF_BUS_SYSTEM_I2C, u2ClkDiv, u1DevAddr, 0, 0,
                        pu1Buf, u2ByteCnt);
    return u2Ret;
}


/** SIF_TunerWriteNoSubAddr
 *  NIM module api: write data without word address to SIF module on SIF_BUS_TUNER_I2C.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1Buf      Pointer to user's buffer.
 *  @param  u2ByteCnt   Number of byte to write. Max = 256 bytes.
 *  @retval > 0         Actual number of bytes written.
 *  @retval = 0         Error
 */
UINT16 SIF_TunerWriteNoSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8* pu1Buf,
                               UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    u2Ret = SIF_X_Write(SIF_BUS_TUNER_I2C, u2ClkDiv, u1DevAddr, 0, 0,
                        pu1Buf, u2ByteCnt);
    return u2Ret;
}


/** SIF_ReadMultipleSubAddress
 *  Read data with word address, of which length up to three, from SIF module from SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes read.
 *  @retval = 0             Error
 */
UINT16 SIF_ReadMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                               UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    u2Ret = SIF_X_Read(SIF_BUS_SYSTEM_I2C, u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                       pu1Buf, u2ByteCnt);
    return u2Ret;
}


/** SIF_WriteMultipleSubAddress
 *  Write data with word address, of which length up to three, to SIF module on SIF_BUS_SYSTEM_I2C.
 *  @param  u2ClkDiv        Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr       Serial interface device address.
 *  @param  u1WordAddrNum   Serial interface word address length.
 *  @param  u4WordAddr      Serial interface word address. Format XX B2 B1 B0. B2 is sent on SIF bus first.
 *  @param  pu1Buf          Pointer to user's buffer.
 *  @param  u2ByteCnt       Number of byte to read. Max = 256 bytes.
 *  @retval > 0             Actual number of bytes written.
 *  @retval = 0             Error
 */
UINT16 SIF_WriteMultipleSubAddr(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                                UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    u2Ret = SIF_X_Write(SIF_BUS_SYSTEM_I2C, u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr,
                        pu1Buf, u2ByteCnt);
    return u2Ret;
}


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
UINT16 SIF_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                  UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;

#ifdef CC_FIXED_SIF_CLK_DIV
    UINT32 u4FixClkDiv;
    u4FixClkDiv = DRVCUST_InitGet(eSifcfgFixClkDiv);
    u2ClkDiv = u4FixClkDiv;
#endif
    if (_aprSifStruct[u1CHannelID] == NULL)
    {
        goto X_READ_ERROR;
    }
    if (_aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Read == NULL)
    {
        goto X_READ_ERROR;
    }
    _SIF_Lock();

    if (_fgSIFDisabled)
    {
        _SIF_Unlock();
        return u2ByteCnt;
    }

    _SIF_SetActiveChannel(_aprSifStruct[u1CHannelID]);
    u2Ret = _aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Read(u1CHannelID, u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr, pu1Buf, u2ByteCnt);
    _SIF_Unlock();

#ifdef SIF_CHECK_SDA_LOW
    if (u2Ret != u2ByteCnt)
    {
        Printf("[SIF][Read_Error]Channle(%d)Device_Addr(0x%x) want read(%d) bytes. Actually read(%d) bytes\n", u1CHannelID, u1DevAddr, u2ByteCnt, u2Ret);
    }
#endif

X_READ_ERROR:
    return u2Ret;
}

/** SIF_X_SpecialRead
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
 *   start + slave address + w +tnflg + target address + w +  register address
 *
 restart + slave address + w +tnflg + target address + r
 *    restart + slave address + r +data ... +stop
 */
UINT16 SIF_X_SpecialRead(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                         UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;


#ifdef CC_FIXED_SIF_CLK_DIV
    UINT32 u4FixClkDiv;
    u4FixClkDiv = DRVCUST_InitGet(eSifcfgFixClkDiv);
    u2ClkDiv = u4FixClkDiv;
#endif
    if (_aprSifStruct[u1CHannelID] == NULL)
    {
        goto X_READ_ERROR;
    }
    if (_aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Sp_Read == NULL)
    {
        goto X_READ_ERROR;
    }
    _SIF_Lock();

    if (_fgSIFDisabled)
    {
        _SIF_Unlock();
        Printf("SIF_X_Read _fgSIFDisabled:\n");
        return u2ByteCnt;
    }

    _SIF_SetActiveChannel(_aprSifStruct[u1CHannelID]);
    u2Ret = _aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Sp_Read(u1CHannelID, u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr, pu1Buf, u2ByteCnt);
    _SIF_Unlock();
X_READ_ERROR:
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
UINT16 SIF_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                   UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
#ifdef CC_FIXED_SIF_CLK_DIV
    UINT32 u4FixClkDiv;
    u4FixClkDiv = DRVCUST_InitGet(eSifcfgFixClkDiv);
    u2ClkDiv = u4FixClkDiv;
#endif

    if (_aprSifStruct[u1CHannelID] == NULL)
    {
        goto X_WRITE_ERROR;
    }
    if (_aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Write == NULL)
    {
        goto X_WRITE_ERROR;
    }
    _SIF_Lock();

    if (_fgSIFDisabled)
    {
        _SIF_Unlock();
        return u2ByteCnt;
    }

    _SIF_SetActiveChannel(_aprSifStruct[u1CHannelID]);
    u2Ret = _aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Write(u1CHannelID, u2ClkDiv, u1DevAddr, u1WordAddrNum, u4WordAddr, pu1Buf, u2ByteCnt);
    _SIF_Unlock();

#ifdef SIF_CHECK_SDA_LOW
    if (u2Ret != u2ByteCnt)
    {
        Printf("[SIF][Write_Error]Channle(%d)Device_Addr(0x%x) want write(%d) bytes. Actually write(%d) bytes\n", u1CHannelID, u1DevAddr, u2ByteCnt, u2Ret);
    }
#endif

X_WRITE_ERROR:
    return u2Ret;
}


UINT16 SIF_X_Write_SpecialDemod(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr, UINT8* pu1Buf, UINT16 u2ByteCnt)
{
    UINT16 u2Ret = 0;
    UINT8 au1Data[4];
    if (_aprSifStruct[u1CHannelID] == NULL)
    {
        goto X_WRITE_SD_ERROR;
    }
    if (_aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Write == NULL)
    {
        goto X_WRITE_SD_ERROR;
    }
    _SIF_Lock();

    if (_fgSIFDisabled)
    {
        _SIF_Unlock();
        return u2ByteCnt;
    }

    _SIF_SetActiveChannel(_aprSifStruct[u1CHannelID]);
    if (u1WordAddrNum == 1)
    {
        au1Data[0] = (UINT8)(u4WordAddr & 0xFF);
    }
    if (u1WordAddrNum == 2)
    {
        au1Data[0] = (UINT8)((u4WordAddr >> 8) & 0xFF);
        au1Data[1] = (UINT8)(u4WordAddr & 0xFF);
    }
    else if (u1WordAddrNum == 3)
    {
        au1Data[0] = (UINT8)((u4WordAddr >> 16) & 0xFF);
        au1Data[1] = (UINT8)((u4WordAddr >> 8) & 0xFF);
        au1Data[2] = (UINT8)(u4WordAddr & 0xFF);
    }
    else
    {
        _SIF_Unlock();
        goto X_WRITE_SD_ERROR;
    }
    u2Ret = _aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Write(u1CHannelID, u2ClkDiv, u1DevAddr, 0, 0, au1Data, (UINT16)u1WordAddrNum);
    if (u2Ret != (UINT16)u1WordAddrNum)
    {
        _SIF_Unlock();
        goto X_WRITE_SD_ERROR;
    }
    u2Ret = _aprSifStruct[u1CHannelID]->rSifOps.pfn_X_Write(u1CHannelID, u2ClkDiv, u1DevAddr, 0, 0, pu1Buf, u2ByteCnt);
    _SIF_Unlock();
X_WRITE_SD_ERROR:
    return u2Ret;
}


#if 0
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
UINT16 SIF_X_AsyncRead(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
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
UINT16 SIF_X_AsyncWrite(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
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

#ifdef __MODEL_slt__
INT32 SIF_Diag(VOID)
{
    UINT32 u4Reg = 0;
#ifdef CC_MT5387
    Printf("This is mt5387\n");
#endif
    //    u4Reg = _SIF_READ32(RW_SM0_CTRL0_REG);

    Printf("Master 0 %s\n", (u4Reg | SM_EN) ? "Enable" : "Disable");

    Printf("Bus clock pull high by: %s \n",
           (u4Reg | ODRAIN) ? "Master" : "Open Drain (pull-high register");

    Printf("SCL-Stretch %s\n", (u4Reg | SCL_STRETCH) ? "On" : "Off");

    Printf("SCL= %s, SDA= %s\n", (u4Reg | SCL_STATE) ? "High" : "Low",
           (u4Reg | SDA_STATE) ? "High" : "Low");

    return 0;
}
#endif

/** SIF_WaitLastTransaction
 *  Signal SIF driver to stop issueing any SIF read/write and wait the completion of the last SIF transaction
 *  @param  u4Timeout     Safety guard to avoid wait forever.
 *  @retval void
 */
void SIF_WaitLastTransaction(UINT32 u4Timeout)
{
    _SIF_LockTimeout(u4Timeout);
    _fgSIFDisabled = TRUE;
    _SIF_Unlock();
}

#if defined(CC_SUPPORT_STR)||defined(CC_LG_SNAP_SHOT)
extern VOID _SIF_DisableAndClearInterrupt(VOID);
//-----------------------------------------------------------------------------
/**SIF_pm_str_suspend
 */
//-----------------------------------------------------------------------------
void SIF_pm_str_suspend(void)
{
    _SIF_Lock();

    // power off
    CKGEN_WRITE32(0x1CC, CKGEN_READ32(0x1CC) & ~(1U << 11));

    _SIF_Unlock();
}


//-----------------------------------------------------------------------------
/** SIF_pm_str_resume
 */
//-----------------------------------------------------------------------------
void SIF_pm_str_resume(void)
{
    _SIF_Lock();

    CKGEN_WRITE32(0x1CC, CKGEN_READ32(0x1CC) | (1U << 11));
    _fgSIFDisabled = FALSE;
    _SIF_DisableAndClearInterrupt();

    // Enable SIF masters
    IO_WRITE32MSK(BIM_BASE, RW_SM0_CTRL0_REG, SM_EN, SM_EN);
    IO_WRITE32MSK(BIM_BASE, RW_SM0_CTRL0_REG + 0x30, SM_EN, SM_EN);

#ifdef CC_SIF_OUTPUT_LOW_WAITING
    //change the output level L when SIF master in waiting state
    UINT32 u4Tmp;
    u4Tmp = *((volatile unsigned int*)0xF0029240);
    u4Tmp &= ~(1 << 6); //clear bit6
    *((volatile unsigned int*)0xF0029240) = u4Tmp;
#endif

    _SIF_Unlock();
}
#endif // CC_SUPPORT_STR

#if !defined (CC_MTK_LOADER) && defined (__KERNEL__)
EXPORT_SYMBOL(SIF_X_Read);
EXPORT_SYMBOL(SIF_X_Write);
#endif

