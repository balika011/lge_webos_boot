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
/* Portable C version of des3() function
 * This version is very similar to that in Part V of Applied Cryptography
 * by Bruce Schneier.
 *
 * This information is in the public domain 12/15/95 P. Karn
 */

/*
#include "stdafx.h"
#include "3desTS.h"
#include "3desTSDlg.h"
*/

#include "dmx_drm_drvif.h"

int Asmversion = 0;

/* Combined SP lookup table, linked in
 * For best results, ensure that this is aligned on a 32-bit boundary;
 * Borland C++ 3.1 doesn't guarantee this!
 */
extern unsigned long Spbox[8][64];		/* Combined S and P boxes */

/* Primitive function F.
 * Input is r, subkey array in keys, output is XORed into l.
 * Each round consumes eight 6-bit subkeys, one for
 * each of the 8 S-boxes, 2 longs for each round.
 * Each long contains four 6-bit subkeys, each taking up a byte.
 * The first long contains, from high to low end, the subkeys for
 * S-boxes 1, 3, 5 & 7; the second contains the subkeys for S-boxes
 * 2, 4, 6 & 8 (using the origin-1 S-box numbering in the standard,
 * not the origin-0 numbering used elsewhere in this code)
 * See comments elsewhere about the pre-rotated values of r and Spbox.
 */
#define	F(l,r,key){\
	work = ((r >> 4) | (r << 28)) ^ key[0];\
	l ^= Spbox[6][work & 0x3f];\
	l ^= Spbox[4][(work >> 8) & 0x3f];\
	l ^= Spbox[2][(work >> 16) & 0x3f];\
	l ^= Spbox[0][(work >> 24) & 0x3f];\
	work = r ^ key[1];\
	l ^= Spbox[7][work & 0x3f];\
	l ^= Spbox[5][(work >> 8) & 0x3f];\
	l ^= Spbox[3][(work >> 16) & 0x3f];\
	l ^= Spbox[1][(work >> 24) & 0x3f];\
}
/* Encrypt or decrypt a block of data in ECB mode */
void
des3(DES3_KS ks,unsigned char block[8])
//DES3_KS ks;			/* Key schedule */
//unsigned char block[8];		/* Data block */
{
	unsigned long left,right,work;

	/* Read input block and place in left/right in big-endian order */
	left = ((unsigned long)block[0] << 24)
	 | ((unsigned long)block[1] << 16)
	 | ((unsigned long)block[2] << 8)
	 | (unsigned long)block[3];
	right = ((unsigned long)block[4] << 24)
	 | ((unsigned long)block[5] << 16)
	 | ((unsigned long)block[6] << 8)
	 | (unsigned long)block[7];

	/* Hoey's clever initial permutation algorithm, from Outerbridge
	 * (see Schneier p 478)
	 *
	 * The convention here is the same as Outerbridge: rotate each
	 * register left by 1 bit, i.e., so that "left" contains permuted
	 * input bits 2, 3, 4, ... 1 and "right" contains 33, 34, 35, ... 32
	 * (using origin-1 numbering as in the FIPS). This allows us to avoid
	 * one of the two rotates that would otherwise be required in each of
	 * the 16 rounds.
	 */
	work = ((left >> 4) ^ right) & 0x0f0f0f0f;
	right ^= work;
	left ^= work << 4;
	work = ((left >> 16) ^ right) & 0xffff;
	right ^= work;
	left ^= work << 16;
	work = ((right >> 2) ^ left) & 0x33333333;
	left ^= work;
	right ^= (work << 2);
	work = ((right >> 8) ^ left) & 0xff00ff;
	left ^= work;
	right ^= (work << 8);
	right = (right << 1) | (right >> 31);
	work = (left ^ right) & 0xaaaaaaaa;
	left ^= work;
	right ^= work;
	left = (left << 1) | (left >> 31);

	/* First key */
	F(left,right,ks[0]);
	F(right,left,ks[1]);
	F(left,right,ks[2]);
	F(right,left,ks[3]);
	F(left,right,ks[4]);
	F(right,left,ks[5]);
	F(left,right,ks[6]);
	F(right,left,ks[7]);
	F(left,right,ks[8]);
	F(right,left,ks[9]);
	F(left,right,ks[10]);
	F(right,left,ks[11]);
	F(left,right,ks[12]);
	F(right,left,ks[13]);
	F(left,right,ks[14]);
	F(right,left,ks[15]);

	/* Second key (must be created in opposite mode to first key) */
	F(right,left,ks[16]);
	F(left,right,ks[17]);
	F(right,left,ks[18]);
	F(left,right,ks[19]);
	F(right,left,ks[20]);
	F(left,right,ks[21]);
	F(right,left,ks[22]);
	F(left,right,ks[23]);
	F(right,left,ks[24]);
	F(left,right,ks[25]);
	F(right,left,ks[26]);
	F(left,right,ks[27]);
	F(right,left,ks[28]);
	F(left,right,ks[29]);
	F(right,left,ks[30]);
	F(left,right,ks[31]);

	/* Third key */
	F(left,right,ks[32]);
	F(right,left,ks[33]);
	F(left,right,ks[34]);
	F(right,left,ks[35]);
	F(left,right,ks[36]);
	F(right,left,ks[37]);
	F(left,right,ks[38]);
	F(right,left,ks[39]);
	F(left,right,ks[40]);
	F(right,left,ks[41]);
	F(left,right,ks[42]);
	F(right,left,ks[43]);
	F(left,right,ks[44]);
	F(right,left,ks[45]);
	F(left,right,ks[46]);
	F(right,left,ks[47]);

	/* Inverse permutation, also from Hoey via Outerbridge and Schneier */
	right = (right << 31) | (right >> 1);
	work = (left ^ right) & 0xaaaaaaaa;
	left ^= work;
	right ^= work;
	left = (left >> 1) | (left  << 31);
	work = ((left >> 8) ^ right) & 0xff00ff;
	right ^= work;
	left ^= work << 8;
	work = ((left >> 2) ^ right) & 0x33333333;
	right ^= work;
	left ^= work << 2;
	work = ((right >> 16) ^ left) & 0xffff;
	left ^= work;
	right ^= work << 16;
	work = ((right >> 4) ^ left) & 0x0f0f0f0f;
	left ^= work;
	right ^= work << 4;

	/* Put the block back into the user's buffer with final swap */
	block[0] = (unsigned char) (right >> 24);
	block[1] = (unsigned char) (right >> 16);
	block[2] = (unsigned char) (right >> 8);
	block[3] = (unsigned char) (right);
	block[4] = (unsigned char) (left >> 24);
	block[5] = (unsigned char) (left >> 16);
	block[6] = (unsigned char) (left >> 8);
	block[7] = (unsigned char) (left);
}

