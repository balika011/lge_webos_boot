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
 * $RCSfile: rtc_if.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains implementation of RTC related interface.
 *---------------------------------------------------------------------------*/
#include "x_assert.h"
#include "drvcust_if.h"
#include "rtc_sw.h"
#include "rtc_hw.h"

EXTERN RTC_FuncTbl* _RTC_GetFunc(void);
EXTERN RTC_FuncTbl* _RTC_EXT_GetFunc(void);
EXTERN BOOL _RTC_EXT_Init(void);
//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
UINT8 RTC_BcdToBinary(UINT8 u1Bcd);
UINT8 RTC_BinaryToBcd(UINT8 u1Binary);
BOOL RTC_RtcToUtc(UINT64* pu8Utc, const RTC_T* prRtc);
BOOL RTC_UtcToRtc(RTC_T* prRtc, UINT64 u8Utc);
BOOL RTC_Init(void);
BOOL RTC_GetTimeDate(UINT64* prTime);
BOOL RTC_SetTimeDate(const UINT64* prTime);
BOOL RTC_SetAlertTime(UINT64 u8Utc);
BOOL RTC_GetAlertTime(UINT64* prTime);
void RTC_HookAlertHandler(PFN_RTC_ALERT_HANDLER_T pfnHandler);
void RTC_SetWakeup(UINT32* pu4PowerDownEn);

//----------------------------------------------------------------------------
// Static variables
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Public interface
//----------------------------------------------------------------------------

RTC_FuncTbl* RTC_Func;

//----------------------------------------------------------------------------
/** RTC_BcdToBinary() Convert a BCD to a binary
 *  @param u1Bcd The BCD to be converted
 *  @retval The converted binary
 */
//----------------------------------------------------------------------------
UINT8 RTC_BcdToBinary(UINT8 u1Bcd)
{
    return _RTC_BcdToBinary(u1Bcd);  
}

//----------------------------------------------------------------------------
/** RTC_BinaryToBcd() Convert a binary to a BCD
 *  @param u1Binary The binary to be converted
 *  @retval The converted BCD
 */
//----------------------------------------------------------------------------
UINT8 RTC_BinaryToBcd(UINT8 u1Binary)
{
    return _RTC_BinaryToBcd(u1Binary);
}

//----------------------------------------------------------------------------
/** RTC_RtcToUtc() Translate RTC to UTC
 *  @param prRtc, Input, The RTC to be translated
 *  @param pu8Utc, Output, The translated UTC
 *  @retval TRUE, Succeed; FALSE, Fail
 */
//----------------------------------------------------------------------------
BOOL RTC_RtcToUtc(UINT64* pu8Utc, const RTC_T* prRtc)
{
   return _RTC_RtcToUtc(pu8Utc, prRtc);
}


//----------------------------------------------------------------------------
/** RTC_UtcToRtc() Translate UTC to RTC
 *  @param u8Utc, Input, The UTC to be translated
 *  @param prRtc, Output, The translated RTC
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
BOOL RTC_UtcToRtc(RTC_T* prRtc, UINT64 u8Utc)
{
    return _RTC_UtcToRtc(prRtc, u8Utc);
}

//----------------------------------------------------------------------------
/** RTC_Init() Initialize RTC driver and hardware
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
BOOL RTC_Init(void)
{
    BOOL fgRet;

    // Must initialize internal RTC.
	
    RTC_Func = _RTC_GetFunc();

    VERIFY(RTC_Func != NULL);
    VERIFY(RTC_Func->pfnInit != NULL);    
    fgRet = RTC_Func->pfnInit();

    if(DRVCUST_OptGet(eRtcHw) == (UINT32)RTC_INTERNAL)
    {
        return fgRet;
    }

    else if(DRVCUST_OptGet(eRtcHw) == (UINT32)RTC_EXT)
    {        
        RTC_Func = _RTC_EXT_GetFunc();
        VERIFY(RTC_Func != NULL);
        VERIFY(RTC_Func->pfnInit != NULL);          
        return _RTC_EXT_Init();
    }

    else
    {
        VERIFY(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return FALSE;
    }
}


//----------------------------------------------------------------------------
/** RTC_GetTimeDate() Get time from RTC
 *  @param prTime, Output, The time
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
BOOL RTC_GetTimeDate(UINT64* prTime)
{    
    VERIFY(RTC_Func != NULL);
    VERIFY(RTC_Func->pfnGetTimeDate != NULL);    
    return RTC_Func->pfnGetTimeDate(prTime);
}

//----------------------------------------------------------------------------
/** RTC_SetTimeDate() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
BOOL RTC_SetTimeDate(const UINT64* prTime)
{
    VERIFY(RTC_Func != NULL);
    VERIFY(RTC_Func->pfnSetTimeDate != NULL);
    return RTC_Func->pfnSetTimeDate(prTime);
}

//----------------------------------------------------------------------------
/** RTC_GetTimeDate() Get time from RTC
 *  @param prTime, Output, The time
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
#if 0
BOOL RTC_GetRtc(RTC_T* prRtc)
{    
    VERIFY(RTC_Func != NULL);
    VERIFY(RTC_Func->pfnGetRtc != NULL);    
    return RTC_Func->pfnGetRtc(prRtc);
}
#endif	
//----------------------------------------------------------------------------
/** RTC_SetTimeDate() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
#if 0
#ifdef CC_UP8032_ATV
extern BOOL _RTC_SetRtc(RTC_T rRtc);
#endif
BOOL RTC_SetRtc(RTC_T rRtc)
{
#ifdef CC_UP8032_ATV
    BOOL ret;
    ret = _RTC_SetRtc(rRtc);
    return ret;
#else
    VERIFY(RTC_Func != NULL);
    VERIFY(RTC_Func->pfnSetRtc != NULL);
    return RTC_Func->pfnSetRtc(rRtc);


#endif
}
#endif

//----------------------------------------------------------------------------
/** RTC_SetAlertTime() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
BOOL RTC_SetAlertTime(UINT64 u8Utc)
{
    VERIFY(RTC_Func != NULL);
    VERIFY(RTC_Func->pfnSetAlertTime != NULL);    
    return RTC_Func->pfnSetAlertTime(u8Utc);   
}

//----------------------------------------------------------------------------
/** RTC_GetAlertTime() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
BOOL RTC_GetAlertTime(UINT64* prTime)
{
    if((RTC_Func != NULL) && (RTC_Func->pfnGetAlertTime != NULL))
    {
        return RTC_Func->pfnGetAlertTime(prTime);
    }
    return FALSE;
}
//----------------------------------------------------------------------------
/** RTC_HookAlertHandler()
 *  @param pfnHandler the callback function.
 */
//----------------------------------------------------------------------------
void RTC_HookAlertHandler(PFN_RTC_ALERT_HANDLER_T pfnHandler)
{
    VERIFY(RTC_Func != NULL);
    VERIFY(RTC_Func->pfnHookAlertHandler!= NULL); 
    RTC_Func->pfnHookAlertHandler(pfnHandler);
}


//----------------------------------------------------------------------------
/** RTC_SetWakeup()
 *  @param pfnHandler the callback function.
 */
//----------------------------------------------------------------------------
void RTC_SetWakeup(UINT32* pu4PowerDownEn)
{
    VERIFY(RTC_Func != NULL);
    VERIFY(RTC_Func->pfnSetWakeup != NULL); 
    RTC_Func->pfnSetWakeup(pu4PowerDownEn);
}

