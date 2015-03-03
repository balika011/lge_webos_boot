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
 * $Date: 2015/03/03 $
 * $RCSfile: dmx_if.c,v $
 * $Revision: #8 $
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
#include "gcpu_if.h"
#include "msdc_if.h"
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


//#define SNAPSHOT_DEBUG
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


#endif

//#define SNAPSHOT_DEBUG
extern int lgemmc_get_partnum(const char *name);

int sign_snapshot(char* blkdev, unsigned int size, unsigned int snapoffset,unsigned int frag_unit_size,unsigned int num_of_frag)
{
    UINT8 u1AesKey[16];
	UINT8 pu1RetIv[16];
 //   UINT8* buffer;
//   UINT32 bufsize=4096;	
//    UINT32 bufsize=0x10000;
 #ifdef SNAPSHOT_DEBUG
    UINT32 i;
 #endif
//    INT32 u4Offset=0;
//    UINT32 u4MidEnd;
	GCPU_SHA_HDL_T rShaHdl;
	UINT8 au1Digest[32];
	UINT8 au1Digest_dst[32];
//    UINT32 u4ImageSize;
	UINT64 u4fullImageSize  ;
	UINT32	u4PartId = 0;
	UINT32 phy_group_buf;
    unsigned char* group_buf;
    unsigned int group_size = 0, group_num = 0,frag_idx = 0;
		//,group_idx = 0;
	UINT64	offset = 0;
	u4PartId =lgemmc_get_partnum(blkdev)+1;
	
	if(u4PartId <=0)
		{
			LOG(0,"\n get partition id failed \n");
			return 0;
		}

	u4fullImageSize  = (UINT64)size + (UINT64)snapoffset;

#ifdef SNAPSHOT_DEBUG
    LOG(0,"snapshot debug : full image size: %lld \n", u4fullImageSize);
    LOG(0,"###########snapshot debug : u4PartId: %d \n", u4PartId);
#endif	

	//u4PartId = 43;  //hard code for hib partition 

    x_memset(u1AesKey, 0x11, 16);


//    u4ImageSize = size;
	

	//u4fullImageSize += 0x1000; // add another header size --> don't know the reason
	
	u4fullImageSize += 32;     // add fullimage signature size 
	
//for fragment signture check
	group_size = (frag_unit_size*num_of_frag);
	group_num = 1;// only implement partitial verify , so set group num -->1
	
    LOG(0,"snapshot debug  partial sign: group_num: 0x%x\n", group_num);
	
	//group_buf = (unsigned char*)x_mem_alloc(group_num*frag_unit_size);
	phy_group_buf = BSP_AllocAlignedDmaMemory(group_num*frag_unit_size, 32);
	group_buf = (unsigned char*)VIRTUAL(phy_group_buf);
	if (!group_buf)
	{
		LOG(0,"malloc %d bytes fail!!!\n", group_num*frag_unit_size);
		return 1;
	}

	
	GCPU_Init(0);
    GCPU_SHA256_Init((UINT32)&rShaHdl,TRUE);

	x_memset(group_buf, 0, 32);
	
	//GCPU_SHA256_Update((UINT32)&rShaHdl, group_buf, 32,TRUE);
	//GCPU_SHA256_Final((UINT32)&rShaHdl,au1Digest);
#ifdef SNAPSHOT_DEBUG
	
		Printf("snapshot debug sha256: \n");
		for (i=0; i<32; i++)
		{
			Printf("%02x ", au1Digest[i]);
		}
		
		Printf("\n");
#endif
	
	
//	num_of_frag = 1; //for test only one frag


	for (frag_idx = 0; frag_idx < num_of_frag; frag_idx++)
	{
		
		offset = frag_idx*frag_unit_size  + snapoffset;
#ifdef SNAPSHOT_DEBUG
				LOG(0,"snapshot debug: frag %d \n", frag_idx);
				LOG(0,"snapshot debug: offset %lld \n", offset);
		
#endif

		Snapshot_ReadMsdc(u4PartId,offset,group_buf,frag_unit_size);
#ifdef SNAPSHOT_DEBUG
		
		LOG(0,"snapshot debug : read image data test (only 32bytes) \n");
		
		for (i=0; i<32; i++)
		{
			LOG(0,"%02x ", group_buf[i]);
		}
		LOG(0,"\n");
#endif		
		GCPU_SHA256_Update((UINT32)&rShaHdl, group_buf,frag_unit_size,TRUE);
		GCPU_SHA256_Final((UINT32)&rShaHdl,au1Digest);
#ifdef SNAPSHOT_DEBUG
	
		LOG(0,"snapshot debug before aes Digest data: \n");
		for (i=0; i<32; i++)
		{
			LOG(0,"%02x ", au1Digest[i]);
		}
		
		LOG(0,"\n");
#endif
	
		GCPU_AES_CBC(u1AesKey, 16, u1AesKey,pu1RetIv,au1Digest,au1Digest_dst,32,TRUE);

#ifdef SNAPSHOT_DEBUG

		LOG(0,"snapshot debug after aes Digest data: \n");
		for (i=0; i<32; i++)
		{
			LOG(0,"%02x ", au1Digest_dst[i]);
		}

		LOG(0,"\n");
#endif
		
#ifdef SNAPSHOT_DEBUG
		LOG(0,"snapshot debug: u4fullImageSize %lld \n", u4fullImageSize);
#endif
    	Snapshot_WriteMsdc(u4PartId,u4fullImageSize,au1Digest_dst,32);
		u4fullImageSize += 32;

	}
	
    //x_mem_free(group_buf);
    BSP_FreeAlignedDmaMemory(phy_group_buf);

    return 0;
}
#endif
