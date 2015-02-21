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
 * $Date: 2015/02/21 $
 * $RCSfile: dmx_if.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_if.h
 *  Interface of demux driver
 */


#ifndef DMX_DRM_IF_H
#define DMX_DRM_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dmx_if.h"
#include "dmx_mm_if.h"

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
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** DMX_NAND_AES_INIT
 *  NAND AES Initialization
 *
 *  @param  pu1Key          key pointer
 *  @param  u2KeyLen        key length [128|192|256]
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_NAND_AES_INIT(UINT8 *pu1Key, UINT16 u2KeyLen);

#ifndef CC_MTK_LOADER
int sign_snapshot(char* blkdev, unsigned int size, unsigned int snapoffset,unsigned int frag_unit_size,unsigned int num_of_frag);
#endif
//-----------------------------------------------------------------------------
/** DMX_NAND_AES_Encryption
 *  NAND AES encryption
 *
 *  @param  u4InBufStart    input buffer address
 *  @param  u4OutBufStart   output buffer address
 *  @param  u4BufSize       buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_NAND_AES_Encryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize);


//-----------------------------------------------------------------------------
/** DMX_NAND_AES_Decryption
 *  NAND AES decryption
 *
 *  @param  u4InBufStart    input buffer address
 *  @param  u4OutBufStart   output buffer address
 *  @param  u4BufSize       buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_NAND_AES_Decryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize);

//-----------------------------------------------------------------------------
/** DMX_SHA1_Manytimes
 *  DMX_SHA1_Manytimes
 *
 *  @param  fgFirstPacket    the flag of first packet
 *  @param  fgLastPacket    the flag of last packet
 *  @param  u4BufStart    input buffer address
 *  @param  u4BufSize      buffer size ,no need 64 byte alignment
 *  @param  u1HashValue       hashvalue output buffer size;
 *  @param  u4HashValueSize       u4HashValueSize
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SHA1_Manytimes(BOOL fgFirstPacket, BOOL fgLastPacket, UINT32 *u4BufStart,
						UINT32 u4BufSize, UINT8 *u1HashValue, UINT32 *u4HashValueSize);


//-----------------------------------------------------------------------------
/** _DMX_SHA1_Onece
 *  NAND AES decryption
 *
 *  @param  u4BufStart    input buffer address
 *  @param  u4BufSize      buffer size ,no need 64 byte alignment
 *  @param  u1HashValue       hashvalue output buffer size;
 *  @param  u4HashValueSize       u4HashValueSize
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------

BOOL DMX_SHA1_Onece(UINT32 *u4BufStart, UINT32 u4BufSize, UINT8 *u1HashValue, UINT32 *u4HashValueSize);


//-----------------------------------------------------------------------------
/** DMX_Encrypt_Snapshot
 *  DMX_Encrypt_Snapshot
 *
 *  @param  u1HashValueIn       input clear hash value data
 *  @param  u4HashValueSize    hash value data size
 *  @param  u1HashValueOut    encrypt random aeskey address
  *  @param  u1AeskeyAddr    encrypt random aeskey address
 *  @param  u4AeskeySize    aeskey size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_Encrypt_Snapshot(UINT8 *u1HashValueIn, UINT32 u4HashValueSize, UINT8 *u1HashValueOut, 
									UINT8 *u1AeskeyAddr, UINT32 *u4AeskeySize);


//-----------------------------------------------------------------------------
/** DMX_Decrypt_Snapshot
 *  DMX_Decrypt_Snapshot
 *
 *  @param  u1HashValue_Encrypt    input encrypt hash value buffer
 *  @param  u4HashValueSize     	     hashvalue buffer size
 *  @param  u1HashValue_Clear       input cacullate hash value
 *  @param  u1AeskeyAddr    	     input aes key addr
 *  @param  u4AeskeySize  		    aed key size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------


BOOL DMX_Decrypt_Snapshot(UINT8 *u1HashValue_Encrypt, UINT32 u4HashValueSize, UINT8 *u1HashValue_Clear, 
									UINT8 *u1AeskeyAddr, UINT32 u4AeskeySize);





#endif  // DMX_DRM_IF_H

