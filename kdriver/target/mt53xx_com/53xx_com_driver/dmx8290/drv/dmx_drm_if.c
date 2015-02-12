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
 * $Date: 2015/02/12 $
 * $RCSfile: dmx_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_if.c
 *  Demux driver - public interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dmx.h"
#include "dmx_drm_drvif.h"
#include "dmx_drm_if.h"
#include "x_assert.h"
#include "dmx_debug.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
// Uboot use this file, define below to avoid compilation error
#ifdef FUNC_ENTRY
#undef FUNC_ENTRY
#define FUNC_ENTRY
#endif
#ifdef FUNC_EXIT
#undef FUNC_EXIT
#define FUNC_EXIT
#endif

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

BOOL DMX_NAND_AES_INIT(UINT8 *pu1Key, UINT16 u2KeyLen)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_NAND_AES_INIT(pu1Key, u2KeyLen);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}

BOOL DMX_NAND_AES_Encryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_NAND_AES_Encryption(u4InBufStart, u4OutBufStart, u4BufSize);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}

BOOL DMX_NAND_AES_Decryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_NAND_AES_Decryption(u4InBufStart, u4OutBufStart, u4BufSize) ;
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}

BOOL DMX_SHA1_Manytimes(BOOL fgFirstPacket, BOOL fgLastPacket, UINT32 *u4BufStart,
									UINT32 u4BufSize, UINT8 *u1HashValue, UINT32 *u4HashValueSize)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_SHA1_Manytimes(fgFirstPacket, fgLastPacket, u4BufStart, u4BufSize, u1HashValue, u4HashValueSize);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}

BOOL DMX_SHA1_Onece(UINT32 *u4BufStart, UINT32 u4BufSize, UINT8 *u1HashValue, UINT32 *u4HashValueSize)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_SHA1_Onece(u4BufStart, u4BufSize, u1HashValue, u4HashValueSize);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


BOOL DMX_Encrypt_Snapshot(UINT8 *u1HashValueIn, UINT32 u4HashValueSize, UINT8 *u1HashValueOut,
									UINT8 *u1AeskeyAddr, UINT32 *u4AeskeySize)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_Encrypt_Snapshot(u1HashValueIn, u4HashValueSize, u1HashValueOut, u1AeskeyAddr, u4AeskeySize);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}

BOOL DMX_Decrypt_Snapshot(UINT8 *u1HashValue_Encrypt, UINT32 u4HashValueSize, UINT8 *u1HashValue_Clear,
									UINT8 *u1AeskeyAddr, UINT32 u4AeskeySize)

{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_Decrypt_Snapshot(u1HashValue_Encrypt, u4HashValueSize, u1HashValue_Clear, u1AeskeyAddr, u4AeskeySize);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}

#ifndef CC_MTK_LOADER
#if 0
extern void readMMCblock(u32* buffer, u32 offset, u32 size);
extern void writeMMCblock(u8* buffer, u32 offset, u32 size);
#else
#include "drvcust_if.h"

void readMMCblock(UINT32* buffer, UINT32 offset, UINT32 size) {}
void writeMMCblock(UINT8* buffer, UINT32 offset, UINT32 size) {}
INT32 GetPartIDByName(char *szPartName, UINT32  *pu4PartId)
{
    UINT32 u4PartId;
    char *szName;

    for (u4PartId = 0; u4PartId < 64; u4PartId++)
    {
        szName = (char *)DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartName0 + u4PartId));
        if (strcmp(szPartName, szName) == 0)
        {
            *pu4PartId = u4PartId;
            return 0;
        }
    }
    return -1;
}

#endif
extern int Loader_ReadMsdc(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen);
extern int Loader_WriteMsdc(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen);

int sign_snapshot( char* blkdev, int size, unsigned int snapoffset)
{
    UINT8 u1HashValue[32], u1HashValueOut[32];
    UINT32 u4HashValueSize;
    UINT8 u1AesKey[16];
    UINT32 u4AesKeySize;
    UINT8* buffer;
    UINT32 bufsize=4096;
    UINT32 i;
    UINT32 u4Offset=0;
    UINT32 u4MidEnd;

    UINT32 u4ImageSize;
	UINT32	u4PartId;
	if(GetPartIDByName(blkdev, &u4PartId)!=0)
		{
			LOG(0,"\n get partition id failed \n");
			return 0;
		}

    x_memset(u1HashValueOut, 0x0, 32);
    x_memset(u1AesKey, 0x0, 16);

    u4ImageSize = size;
    LOG(0,"image size: 0x%x\n", u4ImageSize);
    u4MidEnd = u4Offset + u4ImageSize - bufsize;

    buffer = x_mem_alloc(bufsize);

    LOG(0,"buffer address: 0x%x\n", buffer);

    // begin
    ASSERT((bufsize & (0x40-1)) == 0);
    Loader_ReadMsdc(u4PartId,u4Offset,(UINT32*)buffer, bufsize);
    DMX_SHA1_Manytimes(TRUE, FALSE, (UINT32*)buffer, bufsize, u1HashValue , &u4HashValueSize);
    u4Offset += bufsize;

    // middle
    while (u4Offset < u4MidEnd)
    {
        ASSERT((bufsize & (0x40-1)) == 0);
		Loader_ReadMsdc(u4PartId,u4Offset,(UINT32*)buffer, bufsize);
        DMX_SHA1_Manytimes(FALSE, FALSE, (UINT32*)buffer, bufsize, u1HashValue , &u4HashValueSize);
        u4Offset += bufsize;
        LOG(0,".");
    }

    // end
    ASSERT((bufsize & (0x40-1)) == 0);
    Loader_ReadMsdc(u4PartId,u4Offset,(UINT32*)buffer, bufsize);
    DMX_SHA1_Manytimes(FALSE, TRUE, (UINT32*)buffer, bufsize, u1HashValue , &u4HashValueSize);
    //u4Offset += bufsize;

    ASSERT(u4HashValueSize==20 || u4HashValueSize==32);

    //output result: u1HashValue[32], u4HashValueSize
    LOG(0,"(yjdbg) u4HashValueSize: %d\n", u4HashValueSize);
    for (i=0; i<u4HashValueSize; i++)
    {
        LOG(0,"%02x ", u1HashValue[i]);
    }
    LOG(0,"\n");

    DMX_Encrypt_Snapshot(u1HashValue, u4HashValueSize, u1HashValueOut, u1AesKey, &u4AesKeySize);
    LOG(0,"(yjdbg) u4AesKeySize: %d\n", u4AesKeySize);
    for (i=0; i<u4AesKeySize; i++)
    {
        LOG(0,"%02x ", u1AesKey[i]);
    }

#if 0 // verification
    cmpresult = DMX_Decrypt_Snapshot(u1HashValueOut, u4HashValueSize, u1HashValue, u1AesKey, u4AesKeySize);
    printf("1st cmpresult: %x\n", cmpresult);

    u1HashValue[0] = 0x12;
    cmpresult = DMX_Decrypt_Snapshot(u1HashValueOut, u4HashValueSize, u1HashValue, u1AesKey, u4AesKeySize);
    printf("2nd cmpresult: %x\n", cmpresult);
#endif

    x_memset(buffer, 0, bufsize);
    x_memcpy(buffer, u1AesKey, u4AesKeySize);
    x_memcpy(buffer+u4AesKeySize, u1HashValueOut, u4HashValueSize);

    Loader_WriteMsdc(u4PartId,snapoffset,buffer,bufsize);

    x_mem_free(buffer);

    return 0;
}
#endif
