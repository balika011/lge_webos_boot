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
 * $Author: p4admin $
 * $Date: 2015/02/13 $
 * $RCSfile: dmx_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file gcpu_if.c
 *  GCPU driver - public interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "gcpu.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_os.h"

LINT_EXT_HEADER_END


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

/// API semaphore
#ifndef CC_MTK_LOADER
static HANDLE_T _hApiSem = NULL_HANDLE;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Enter demux API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _ApiLock(void)
{
#ifndef CC_MTK_LOADER
    static BOOL _fgInit = FALSE;

    if (!_fgInit)
    {
        VERIFY(x_sema_create(&_hApiSem, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _fgInit = TRUE;
    }

    VERIFY(x_sema_lock(_hApiSem, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif
}


//-----------------------------------------------------------------------------
/** Leave demux API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _ApiUnlock(void)
{
#ifndef CC_MTK_LOADER
    VERIFY(x_sema_unlock(_hApiSem) == OSR_OK);
#endif
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//
// Initialization
//


//-----------------------------------------------------------------------------
/** DMX_Init
 *  Initialize demux driver
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL GCPU_Init(UINT32 u4Mode)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = (_GCPU_Init(u4Mode) == S_GCPU_OK)?TRUE:FALSE;
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** GCPU_Cmd
 */
//-----------------------------------------------------------------------------
INT32 GCPU_Cmd(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam)
{
    INT32 i4Ret;

    _ApiLock();
    i4Ret = _GCPU_Cmd(u4Handle, u4Cmd, pvParam);
    _ApiUnlock();

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** GCPU_CmdEx
 */
//-----------------------------------------------------------------------------
INT32 GCPU_CmdEx(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam, BOOL fgIOMMU)
{
    INT32 i4Ret;

    _ApiLock();
    i4Ret = _GCPU_CmdEx(u4Handle, u4Cmd, pvParam, fgIOMMU);
    _ApiUnlock();

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** GCPU_Cmd
 */
//-----------------------------------------------------------------------------
INT32 GCPU_CmdSw(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam)
{
    INT32 i4Ret;

    _ApiLock();
    i4Ret = _GCPU_CmdSw(u4Handle, u4Cmd, pvParam);
    _ApiUnlock();

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** GCPU_AllocSecureSlot
 */
//-----------------------------------------------------------------------------
UINT32 GCPU_AllocSecureSlot(UINT32 u4Size)
{
    UINT32 u4Ret;

    _ApiLock();
    u4Ret = _GCPU_AllocSecureSlot(u4Size);
    _ApiUnlock();

    return u4Ret;
}


//-----------------------------------------------------------------------------
/** GCPU_FreeSecureSlot
 */
//-----------------------------------------------------------------------------
INT32 GCPU_FreeSecureSlot(UINT32 u4SlotPtr)
{
    INT32 i4Ret;

    _ApiLock();
    i4Ret = _GCPU_FreeSecureSlot(u4SlotPtr);
    _ApiUnlock();

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** GCPU_SetSecureSlot
 */
//-----------------------------------------------------------------------------
INT32 GCPU_SetSecureSlot(UINT32 u4SlotPtr, UINT8* pu1Data, UINT32 u4Size)
{
    INT32 i4Ret;

    _ApiLock();
    i4Ret = _GCPU_SetSecureSlot(u4SlotPtr, pu1Data, u4Size);
    _ApiUnlock();

    return i4Ret;
}


#if defined(CC_SUPPORT_STR) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
//-----------------------------------------------------------------------------
/** GCPU_pm_str_suspend
 */
//-----------------------------------------------------------------------------
void GCPU_pm_str_suspend(void)
{
    _ApiLock();
    _GCPU_pm_str_suspend();
    //_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** GCPU_pm_str_resume
 */
//-----------------------------------------------------------------------------
void GCPU_pm_str_resume(void)
{
    //_ApiLock();
    _GCPU_pm_str_resume();
    _ApiUnlock();
}
#endif // CC_SUPPORT_STR

#ifdef CC_MTK_LOADER
BOOL GCPU_LoaderInit(UINT32 u4Mode)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = (_GCPU_LoaderInit(u4Mode) == S_GCPU_OK)?TRUE:FALSE;
    _ApiUnlock();

    return fgRet;
}


BOOL GCPU_SetAesKey(UINT8 *pu1Key, KEY_BIT_LEN eKeyBitLen, UINT8 *pu1Iv)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _GCPU_SetAesKey(pu1Key, eKeyBitLen, pu1Iv);
    _ApiUnlock();

    return fgRet;
}


BOOL GCPU_AesDecrypt(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _GCPU_AesDecrypt(u4InBufStart, u4OutBufStart, u4BufSize);
    _ApiUnlock();

    return fgRet;
}


BOOL GCPU_AesEncrypt(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _GCPU_AesEncrypt(u4InBufStart, u4OutBufStart, u4BufSize);
    _ApiUnlock();

    return fgRet;
}


BOOL GCPU_SHA256(UINT8 *pu1Buf, UINT32 u4Size, UINT8 Message_Digest[32])
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _GCPU_SHA256(pu1Buf, u4Size, Message_Digest);
    _ApiUnlock();

    return fgRet;
}
#endif // CC_MTK_LOADER

BOOL GCPU_SHA256_Init(UINT32 u4Handle, BOOL fgIOMMU)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _GCPU_SHA256_Init(u4Handle, fgIOMMU);
    _ApiUnlock();

    return fgRet;
}

BOOL GCPU_SHA256_Update(UINT32 u4Handle, UINT8 *pu1Buf, UINT32 u4Size, BOOL fgLastPacket)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _GCPU_SHA256_Update(u4Handle, pu1Buf, u4Size, fgLastPacket);
    _ApiUnlock();

    return fgRet;
}

BOOL GCPU_SHA256_Final(UINT32 u4Handle, UINT8 au1Digest[32])
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _GCPU_SHA256_Final(u4Handle, au1Digest);
    _ApiUnlock();

    return fgRet;
}

BOOL GCPU_AES_CBC(const UINT8 *pu1Key, UINT32 u4Keylen, 
                  const UINT8 *pu1Iv, UINT8 *pu1RetIv,
                  const UINT8 *pu1Src, UINT8 *pu1Dst, UINT32 u4Len,
                  BOOL fgEncrypt)
{
    AES_PARAM_T rAes = {0};
    KEY_BIT_LEN eKeyLen;
    BOOL fgRet = TRUE;
    UINT8 i;

    if (!pu1Key || !pu1Iv || !pu1RetIv || !pu1Src || !pu1Dst)
    {
        return FALSE;
    }

    switch (u4Keylen)
    {
    case 16:
        eKeyLen = KEY_BIT_LEN_128;
        break;
    case 24:
        eKeyLen = KEY_BIT_LEN_192;
        break;
    case 32:
        eKeyLen = KEY_BIT_LEN_256;
        break;
    default:
        return FALSE;
    }

    memcpy(rAes.au1Key, pu1Key, u4Keylen);
    memcpy(rAes.au1Iv, pu1Iv, 16);

    rAes.u4SrcStartAddr = (UINT32)pu1Src;
    rAes.u4SrcBufStart = GCPU_LINER_BUFFER_START((UINT32)pu1Src);
    rAes.u4SrcBufEnd = GCPU_LINER_BUFFER_END((UINT32)pu1Src + u4Len);
    rAes.u4DstStartAddr = (UINT32)pu1Dst;
    rAes.u4DstBufStart = GCPU_LINER_BUFFER_START((UINT32)pu1Dst);
    rAes.u4DstBufEnd = GCPU_LINER_BUFFER_END((UINT32)pu1Dst + u4Len);
    rAes.u4DatLen = u4Len;
    rAes.eKeyBitLen = eKeyLen;
    rAes.eMode = BLOCK_CIPHER_MODE_CBC;
    rAes.fgEncrypt = fgEncrypt;

    if (S_GCPU_OK != GCPU_CmdEx(0, GCPU_AES, &rAes, TRUE))
    {
        fgRet = FALSE;
    }
    else
    {
        if (pu1RetIv)
        {
            for(i = 0; i < 16; i++)
            {
                pu1RetIv[i] = rAes.au1Iv[i];
            }
        }
    }

    return fgRet;
}
