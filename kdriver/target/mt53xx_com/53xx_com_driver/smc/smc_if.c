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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: smc_if.c,v $
 * $Revision: #1 $
 *
 * Description: 
 *         This header file contains Interfaces of SmartCard.
 *---------------------------------------------------------------------------*/

/** @file smc_if.c
 *  smc_if.c is the Interfaces of SmartCard.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_gpio.h"
#include "x_pinmux.h"
#include "smc_debug.h"
#include "smc_if.h"
#include "smc_apdu.h"
#include "smc_hp.h"
#include "smc_drvif.h"
#include "drvcust_if.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define SMC_ISSUE_IFS_AFTER_RESET (1)

#define SMC_SLOT_CONTROLLER0      (0)
#define SMC_SLOT_CONTROLLER1      (1)

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
INT32 SMC_Init (void)
{
    INT32 i4Ret = 0;    
         
    i4Ret = GPIO_Init();

    SmcInitDrv();
 
    i4Ret = _SMC_InitHotPlugDetector();

    return i4Ret;
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
void SMC_SetHotPlugCb (PFN_SMC_HOTPLUG_CALLBACK pfnCb, UINT8 u1SlotNo)
{
    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        return _SMC_SetHotPlugCb(pfnCb);
    }
#ifdef CC_SMC1    
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        return _SMC_SetHotPlugCbEx(pfnCb);
    }
#endif
    else
    {
        return;
    }
}

INT32 SMC_Reset (UINT8 u1SlotNo)
{
    INT32 i4Ret = 0;

    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        _SMC_Op_Lock();
        
        if((i4Ret = _SMC_Reactivate()) == 0)
        {    
#if SMC_ISSUE_IFS_AFTER_RESET            
            i4Ret = _SMC_RequestIFS(0xFE);                                  
#endif
        }
        else
        {
            i4Ret = SMCR_CARD_ERROR;
        }
        
        _SMC_Op_Unlock();
    }
#ifdef CC_SMC1
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        _SMC_Op_LockEx();
        
        if((i4Ret = _SMC_ReactivateEx()) == 0)
        {     
#if SMC_ISSUE_IFS_AFTER_RESET            
            i4Ret = _SMC_RequestIFSEx(0xFE);                                  
#endif
        }
        else
        {       
            i4Ret = SMCR_CARD_ERROR;
        }
     
        _SMC_Op_UnlockEx();
    }
#endif
    else
    {
        return SMCR_INV_ARG;
    }
    
    return i4Ret;
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
INT32 SMC_Activate (UINT8 u1SlotNo)
{
    static UINT8   pucATR[256];
    UINT32 u4ATRLen;
    INT32 i4Ret = SMCR_NO_CARD;


    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        _SMC_Op_Lock();    
     
        if((i4Ret = SmcActCard(pucATR, 256, &u4ATRLen)) == 0 && u4ATRLen >= 13)
        {
            _SMC_SetBWT(11 + (2 << (((pucATR[9] & 0xF0) >> 4) - 1)) * 960);
            _SMC_SetCWT(11 + (2 << ((pucATR[9] & 0xF) - 1)));     
        }
        else
        {
            i4Ret = SMCR_CARD_ERROR;        
        }
    
        _SMC_Op_Unlock();
    }
#ifdef CC_SMC1
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        _SMC_Op_LockEx();    
           
        if((i4Ret = SmcActCardEx(pucATR, 256, &u4ATRLen)) == 0 && u4ATRLen >= 13)
        {
            _SMC_SetBWTEx(11 + (2 << (((pucATR[9] & 0xF0) >> 4) - 1)) * 960);
            _SMC_SetCWTEx(11 + (2 << ((pucATR[9] & 0xF) - 1)));                        
        }
        else
        { 
            i4Ret = SMCR_CARD_ERROR;        
        }
        
        _SMC_Op_UnlockEx();
    }
#endif
    else
    {
        return SMCR_INV_ARG;
    }
    
    return i4Ret;
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
INT32 SMC_Deactivate (UINT8 u1SlotNo)
{
    INT32 i4Ret;

    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        _SMC_Op_Lock();    
        i4Ret = SmcDeactCard();      
        _SMC_Op_Unlock();     
    }
#ifdef CC_SMC1    
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        _SMC_Op_LockEx();    
        i4Ret = SmcDeactCardEx();      
        _SMC_Op_UnlockEx();        
    }
#endif
    else
    {
        return SMCR_INV_ARG;
    }
    
    return i4Ret;
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
SMC_CARD_STATUS_T SMC_GetStatus (UINT8 u1SlotNo)
{
    SMC_CARD_STATUS_T rRet;

    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        _SMC_Op_Lock();    
    
        rRet = _SMC_GetStatus();

        _SMC_Op_Unlock();    
    }
#ifdef CC_SMC1
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        _SMC_Op_LockEx();    
    
        rRet = _SMC_GetStatusEx();

        _SMC_Op_UnlockEx();      
    }
#endif
    else
    {
        return SMC_CARD_ABSENT;
    }
    
    return rRet;
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
SMC_CARD_STATUS_T SMC_GetCardPresense (UINT8 u1SlotNo)
{
    SMC_CARD_STATUS_T rStatus = SMC_GetStatus(u1SlotNo);
    
    if(rStatus == SMC_CARD_PRESENT || rStatus == SMC_CARD_ACTIVATED)
    {
        return SMC_CARD_PRESENT;
    }
    return SMC_CARD_ABSENT;
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
INT32 SMC_SendAPDU (UINT8 u1SlotNo, SMC_CMD_SEND_T rCommand, UINT8 *pu1SendData, UINT8 u1SendLength)
{
    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        return _SMC_SendAPDU(u1SlotNo, rCommand, pu1SendData, u1SendLength);
    }
#ifdef CC_SMC1    
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        return _SMC_SendAPDUEx(u1SlotNo, rCommand, pu1SendData, u1SendLength);
    }
#endif
    else
    {
        return SMCR_INV_ARG;
    }
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
INT32 SMC_ReceiveAPDU  (UINT8 u1SlotNo, SMC_CMD_SEND_T *prCmd, UINT8 *pu1ReceiveData, UINT8 *pu1ReceiveLength)
{
    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        return _SMC_ReceiveAPDU(u1SlotNo, prCmd, pu1ReceiveData, pu1ReceiveLength);
    }
#ifdef CC_SMC1
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        return _SMC_ReceiveAPDUEx(u1SlotNo, prCmd, pu1ReceiveData, pu1ReceiveLength);
    }
#endif
    else
    {
        return SMCR_INV_ARG;
    }
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
INT32 SMC_SendReceiveAPDU (UINT8 u1SlotNo,
    UINT8 *pu1SendData, UINT8 u1SendLength, UINT8 *pu1ResponseData, UINT8 *pu1ResponseLength)
{
    INT32 i4Ret;

    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        _SMC_Op_Lock();
    
        i4Ret = _SMC_SendReceiveAPDU (u1SlotNo, pu1SendData, u1SendLength, pu1ResponseData, pu1ResponseLength);
    
        _SMC_Op_Unlock();
    }
#ifdef CC_SMC1    
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        _SMC_Op_LockEx();
    
        i4Ret = _SMC_SendReceiveAPDUEx(u1SlotNo, pu1SendData, u1SendLength, pu1ResponseData, pu1ResponseLength);
    
        _SMC_Op_UnlockEx();
    }
#endif
    else
    {
        return SMCR_INV_ARG;
    }
    
    return i4Ret;
}

/*
 * u1SlotNo
 * 0:          Smart Card Controller 0
 * 1:          Smart Card Controller 1
*/
INT32 SMC_RequestIFS(UINT8 u1Len, UINT8 u1SlotNo)
{
    INT32 i4Ret;

    if (u1SlotNo == SMC_SLOT_CONTROLLER0)
    {
        _SMC_Op_Lock();
    
        i4Ret =  _SMC_RequestIFS(u1Len);
    
        _SMC_Op_Unlock();    
    }
#ifdef CC_SMC1    
    else
    if (u1SlotNo == SMC_SLOT_CONTROLLER1)
    {
        _SMC_Op_LockEx();
    
        i4Ret =  _SMC_RequestIFSEx(u1Len);
    
        _SMC_Op_UnlockEx();        
    }
#endif
    else
    {
        return SMCR_INV_ARG;
    }
    
    return i4Ret;
}

