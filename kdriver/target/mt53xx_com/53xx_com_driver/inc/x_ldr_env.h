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
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifndef X_LDR_ENV_H
#define X_LDR_ENV_H

#include "x_bim.h"

#ifndef CC_LDR_ENV_OFFSET
#define CC_LDR_ENV_OFFSET            (0xf000) // use 60k~64k to save loader env.
#endif /* CC_LDR_ENV_OFFSET */

#define LDR_TIMESTAMP_SIZE 32

// for loader use
#define LDR_SetDramTimeLog(x)   ((LDR_ENV_T*)CC_LDR_ENV_OFFSET)->au4TimeStamp[(x)] = BIM_READ32(REG_RW_TIMER2_LOW)

#define USIG_VERIFY_FAIL 0
#define USIG_DO_UPGRADE '1'
#define USIG_RUN_ON_USB '2'
#define USIG_DO_DIAG		'3'

typedef struct
{
    CHAR    szCustomer[16];
    CHAR    szMagic[16];
    CHAR    szModel[32];
    CHAR    szVersion[64];
} LDR_ENV_HDR_T;

typedef union
{
    UINT8           au1Raw[128];
    LDR_ENV_HDR_T   rHdr;
} LDR_ENV_HDR_UNION_T;

typedef struct
{
    UINT32 u4LoaderPlainSize;
    UINT32 u4LoaderLzhsSize;
    UINT32 u4LoaderLzhsAddress;
    UINT32 u4CfgFlag;
    //0x010
    LDR_ENV_HDR_UNION_T u; //128byte
    //0x090
    UINT32 au4TimeStamp[LDR_TIMESTAMP_SIZE]; //128byte
    //0x110
    UINT8 au1DtvCfg[16]; //DTVCFG_T use 9bytes
    //0x120
    UINT32 au4CustKey[64]; //2048bit key
    //0x220
    CHAR szUenv[128]; //additional uboot env
    //0x2a0
#if defined(CC_EMMC_BOOT)
    UINT8 szMSDCenv[72];  // preloader -> loader
    //0x2E0
    UINT8 szMSDCenv1[64]; //loader -> uboot 
	//0x320
#endif
#if defined(CC_LOADER_CONSOLE_KEY_SUPPORT)

	UINT8 console_key[16];	// preloader -> loader
	UINT8 console_iv[16]; //preloader -> loader
#endif
#if defined(CC_SECURE_BOOT_SCRAMBLE)
	
	UINT8 aes_key2[16];  // preloader -> loader
	UINT8 aes_iv2[16]; //preloader -> loader
#endif
	
	CHAR LoadVesion[8];

} LDR_ENV_T;



#endif // X_LDR_ENV_H
