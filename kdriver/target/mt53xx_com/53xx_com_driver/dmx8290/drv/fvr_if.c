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
 * $RCSfile: fvr_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_if.c
 *  Demux driver - FVR Driver Interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "fvr.h"

#include "x_assert.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


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

//-----------------------------------------------------------------------------
/** Enter FVR API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_ApiLock(void)
{
    _DMX_LockApi();
}


//-----------------------------------------------------------------------------
/** Leave FVR API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_ApiUnlock(void)
{
    _DMX_UnlockApi();
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** FVR_Init
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Init()
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_Init();
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_Start
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Start(UINT8 u1Idx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_Start(u1Idx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_Stop
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Stop(UINT8 u1Idx)
{
    BOOL fgRet = TRUE;

    // May call in HISR
    //_FVR_ApiLock();
    if(_FVR_IsRecording(u1Idx))
    {
        fgRet = _FVR_Stop(u1Idx);
    }
    //_FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetResidual
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetResidual(UINT8 u1Idx, UINT32 *pu4Addr, UINT32 *pu4Size, UINT32 *pu4Index)
{
    BOOL fgRet;

    if((pu4Addr == NULL) || (pu4Size == NULL) || (pu4Index == NULL))
    {
        return FALSE;
    }

    _FVR_ApiLock();
    fgRet = _FVR_GetResidual(u1Idx, pu4Addr, pu4Size, pu4Index);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetScrambleScheme
 *  @retval void
 */
//-----------------------------------------------------------------------------
void FVR_SetScrambleScheme(BOOL fgOrg)
{
    _FVR_SetScrambleScheme(fgOrg);
}


//-----------------------------------------------------------------------------
/** FVR_IsRecording
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_IsRecording(UINT8 u1Idx)
{
    BOOL fgRet;

    //_FVR_ApiLock();
    fgRet = _FVR_IsRecording(u1Idx);
    //_FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetGBL
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetGBL(UINT8 u1Idx,UINT32 u4Flags, const FVR_GBL_T* prGBL)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SetGBL(u1Idx, u4Flags, prGBL);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetGBL
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetGBL(UINT8 u1Idx,UINT32 u4Flags, FVR_GBL_T* prGBL)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_GetGBL(u1Idx, u4Flags, prGBL);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_FreeGBL
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_FreeGBL(UINT8 u1Idx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_FreeGBL(u1Idx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_DropScrambledPacket
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_DropScrambledPacket(UINT8 u1Idx, BOOL fgEnalbe)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_DropScrambledPacket(u1Idx, fgEnalbe);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetPid
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid)
{
    BOOL fgRet;

    _FVR_ApiLock();

    fgRet = _FVR_SetPid(u1Pidx, u4Flags, prPid);

    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetPid
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T* prPid)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_GetPid(u1Pidx, u4Flags, prPid);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_FreePid
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_FreePid(UINT8 u1Pidx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_FreePid(u1Pidx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetTimestamp
 *  @retval void
 */
//-----------------------------------------------------------------------------
void FVR_SetTimestamp(UINT32 u4Flag, const FVR_TIMESTAMP_T* prTimeStamp)
{
    _FVR_ApiLock();
    _FVR_SetTimestamp(u4Flag, prTimeStamp);
    _FVR_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** FVR_GetTimestamp
 *  @retval void
 */
//-----------------------------------------------------------------------------
void FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp)
{
    _FVR_ApiLock();
    _FVR_GetTimestamp(u4Flags, prTimeStamp);
    _FVR_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** FVR_FlushBuffer
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_FlushBuffer(UINT8 u1Idx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_FlushBuffer(u1Idx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetBuffer
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetBuffer(UINT8 u1Idx,UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 *pu4RetSize)
{
    BOOL fgRet;

    fgRet = _FVR_GetBuffer(u1Idx, u4Addr, u4Size, pu1Buf, pu4RetSize);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_UnlockBuffer
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_UnlockBuffer(UINT8 u1Idx,UINT32 u4Addr, UINT32 u4Size)
{
    BOOL fgRet;

    fgRet = _FVR_UnlockBuffer(u1Idx, u4Addr, u4Size);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_FreeBuffer
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL FVR_FreeBuffer(UINT8 u1Idx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_FreeBuffer(u1Idx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetIsOverflow
 *  @retval
 */
//-----------------------------------------------------------------------------
void FVR_SetIsOverflow(UINT8 u1Idx,BOOL fgIsOverflow)
{
    _FVR_SetIsOverflow(u1Idx, fgIsOverflow);
}


//-----------------------------------------------------------------------------
/** FVR_QueryStatus
 *  @retval
 */
//-----------------------------------------------------------------------------
void FVR_QueryStatus(UINT8 u1Idx, FVR_QUERY_INFO_T *prInfo)
{
    _FVR_ApiLock();
    _FVR_QueryStatus(u1Idx, prInfo);
    _FVR_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** FVR_SetByPass
 *  @retval
 */
//-----------------------------------------------------------------------------
void FVR_SetByPass(UINT8 u1TsIndex, UINT32 u4Val, BOOL fgEnable)
{
    _FVR_ApiLock();
    _FVR_SetByPass(u1TsIndex, u4Val, fgEnable);
    _FVR_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** FVR_SwapSetPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SwapSetPattern(UINT8 u1Pidx, UINT8* pu1Pattern, UINT16 u2Len)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SwapSetPattern(u1Pidx, pu1Pattern, u2Len);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SwapSetNullPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SwapSetNullPattern(UINT8 u1Pidx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SwapSetNullPattern(u1Pidx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SwapFreePattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SwapFreePattern(UINT8 u1Pidx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SwapFreePattern(u1Pidx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SwapFreePattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_DoPacketSwapOnce(UINT8 u1Pidx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_DoPacketSwapOnce(u1Pidx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetVideoTypeEx
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetVideoTypeEx(UINT8 u1TsIdx, FVR_VIDEO_TYPE_T eVideoType)
{
    DMX_VIDEO_TYPE_T eType = DMX_VIDEO_NONE;

    switch(eVideoType)
    {
    case FVR_VIDEO_MPEG:
        eType = DMX_VIDEO_MPEG;
        break;

    case FVR_VIDEO_H264:
        eType = DMX_VIDEO_H264;
        break;

    case FVR_VIDEO_AVS:
        eType = DMX_VIDEO_AVS;
        break;

    default:
        eType = DMX_VIDEO_NONE;
        break;
    }

    if(!_DMX_SetVideoType(u1TsIdx, eType))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** FVR_SetVideoTypeByPidx
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetVideoTypeByPidx(UINT8 u1Pidx, FVR_VIDEO_TYPE_T eVideoType)
{
    BOOL fgRet;

    fgRet = _FVR_SetVideoTypeByPidx(u1Pidx, eVideoType);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetVideoTypeEx
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
FVR_VIDEO_TYPE_T FVR_GetVideoTypeEx(UINT8 u1TsIdx)
{
    FVR_VIDEO_TYPE_T eType = FVR_VIDEO_NONE;

    // Use default
    switch(_DMX_GeteVideoType(u1TsIdx))
    {
    case DMX_VIDEO_MPEG:
        eType = FVR_VIDEO_MPEG;
        break;

    case DMX_VIDEO_H264:
        eType = FVR_VIDEO_H264;
        break;

    case DMX_VIDEO_AVS:
        eType = FVR_VIDEO_AVS;
        break;

    default:
        eType = FVR_VIDEO_NONE;
        break;
    }

    return eType;
}


//-----------------------------------------------------------------------------
/** FVR_GetVideoTypeByPidx
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
FVR_VIDEO_TYPE_T FVR_GetVideoTypeByPidx(UINT8 u1Pidx)
{
    FVR_VIDEO_TYPE_T eVideoType;

    eVideoType = _FVR_GetVideoTypeByPidx(u1Pidx);

    return eVideoType;
}

//-----------------------------------------------------------------------------
/** FVR_SetAesKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetAesKey(UINT8 u1KeyIdx, BOOL fgEven, UINT8 *pu1Key)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SetAesKey(u1KeyIdx, fgEven, pu1Key);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetAesEvenOddIV
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetAesEvenOddIV(UINT8 u1Idx, BOOL fgEven, UINT8 *pu1IV)
{
    BOOL fgRet;
    UINT32 au4IV[2];

    au4IV[0] = (UINT32)pu1IV[0] | ((UINT32)pu1IV[1] << 8) | ((UINT32)pu1IV[2]<<16) | ((UINT32)pu1IV[3]<<24);
    au4IV[1] = (UINT32)pu1IV[4] | ((UINT32)pu1IV[5] << 8) | ((UINT32)pu1IV[6]<<16) | ((UINT32)pu1IV[7]<<24);

    _FVR_ApiLock();
    fgRet = _FVR_SetAesEvenOddIV(u1Idx, fgEven, au4IV);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_SetCryptoKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetCryptoKey(UINT8 u1KeyIdx, BOOL fgEven, FVR_CRYPTO_KEY_TYPE_T eKeyType,
                    const UINT8 *pu1Key, UINT32 u4KeyLen)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SetCryptoKey(u1KeyIdx, fgEven, eKeyType, pu1Key, u4KeyLen);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_UpdateWritePointer
 *  Use in polling case
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_UpdateWritePointer(UINT8 u1Idx, UINT32 u4Wp)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_UpdateWritePointer(u1Idx, u4Wp);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SitSetPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SitSetPattern(UINT8 u1TsIdx, UINT16 u2Pid, const UINT8 *pu1Data, UINT16 u2Len)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SitSetPattern(u1TsIdx, u2Pid, pu1Data, u2Len);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_SitFreePattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SitFreePattern(UINT8 u1TsIdx)
{    
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SitFreePattern(u1TsIdx);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_SitInsertPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SitInsertPattern(UINT8 u1TsIdx)
{
    BOOL fgRet;

    fgRet = _FVR_SitInsertPattern(u1TsIdx, TRUE);
    
    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_SitInsertPatternByTimer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SitInsertPatternByTimer(UINT8 u1TsIdx, UINT32 u4MsDelay)
{
     BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SitInsertPatternByTimer(u1TsIdx, u4MsDelay);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_SitStopTimer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SitStopTimer(UINT8 u1TsIdx)
{
     BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SitStopTimer(u1TsIdx);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_StartInsertDIT
 *  Start inserting DIT
 *  DIT will be inserted if PCR diff is biger than u4PcrDiffMs
 *
 *  @param  u1Pidx          PID index
 *  @param  u4PcrDiffMs     PCR diff in millisecond 
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_StartInsertDIT(UINT8 u1Pidx, UINT32 u4PcrDiffMs)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_StartInsertDIT(u1Pidx, u4PcrDiffMs);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_StopInsertDIT
 *  Stop inserting DIT
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_StopInsertDIT(UINT8 u1Pidx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_StopInsertDIT(u1Pidx);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_GetScrambleState
 *  Get scramble state
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval The scramble state
 */
//-----------------------------------------------------------------------------
FVR_SCRAMBLE_STATE_T FVR_GetScrambleState(UINT8 u1Pidx)
{
    FVR_SCRAMBLE_STATE_T eState;

    _FVR_ApiLock();
    eState = _FVR_GetScrambleState(u1Pidx);
    _FVR_ApiUnlock();

    return eState;
}

//-----------------------------------------------------------------------------
/** FVR_GetRawScrambleState
 *  Get raw scramble state
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval The raw scramble state
 */
//-----------------------------------------------------------------------------
FVR_SCRAMBLE_STATE_T FVR_GetRawScrambleState(UINT8 u1Pidx)
{
    FVR_SCRAMBLE_STATE_T eState;

    _FVR_ApiLock();
    eState = _FVR_GetRawScrambleState(u1Pidx);
    _FVR_ApiUnlock();

    return eState;
}




