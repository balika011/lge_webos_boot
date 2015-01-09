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
 * $RCSfile: _sm_ttx_engine.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: This file is for attributes of S/W use by Pibben
 *         
 *         
 *---------------------------------------------------------------------------*/

#ifndef _X_SM_TTX_COMMON_EX_H_
#define _X_SM_TTX_COMMON_EX_H_

#include "x_sm_ttx_common.h"

#if 0
/* SW Mosaics */
#define SM_TTX_WGL_CELL_HAS_SW_MOS(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00080000)
#define SM_TTX_WGL_CELL_SET_SW_MOS(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00080000)
#define SM_TTX_WGL_CELL_CLR_SW_MOS(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00080000)

/* G0 Select */
#define SM_TTX_WGL_CELL_HAS_G0_SEL(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00100000)
#define SM_TTX_WGL_CELL_SET_G0_SEL(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00100000)
#define SM_TTX_WGL_CELL_CLR_G0_SEL(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00100000)

/* SW Foreground Set */
#define SM_TTX_WGL_CELL_HAS_SW_FG(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00200000)
#define SM_TTX_WGL_CELL_SET_SW_FG(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00200000)
#define SM_TTX_WGL_CELL_CLR_SW_FG(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00200000)

/* SW Background Set */
#define SM_TTX_WGL_CELL_HAS_SW_BG(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00400000)
#define SM_TTX_WGL_CELL_SET_SW_BG(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00400000)
#define SM_TTX_WGL_CELL_CLR_SW_BG(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00400000)

/* SW Conceal */
#define SM_TTX_WGL_CELL_HAS_SW_CCL(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x00800000)
#define SM_TTX_WGL_CELL_SET_SW_CCL(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x00800000)
#define SM_TTX_WGL_CELL_CLR_SW_CCL(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x00800000)

/* SW Invert */
#define SM_TTX_WGL_CELL_HAS_SW_INV(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x01000000)
#define SM_TTX_WGL_CELL_SET_SW_INV(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x01000000)
#define SM_TTX_WGL_CELL_CLR_SW_INV(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x01000000)

/* SW Double-Height */
#define SM_TTX_WGL_CELL_HAS_SW_DH(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x02000000)
#define SM_TTX_WGL_CELL_SET_SW_DH(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x02000000)
#define SM_TTX_WGL_CELL_CLR_SW_DH(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x02000000)

/* SW Double-Width */
#define SM_TTX_WGL_CELL_HAS_SW_DW(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x04000000)
#define SM_TTX_WGL_CELL_SET_SW_DW(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x04000000)
#define SM_TTX_WGL_CELL_CLR_SW_DW(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x04000000)

/* SW Double-Height/Size Upper */
#define SM_TTX_WGL_CELL_HAS_SW_DHDS_UP(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x08000000)
#define SM_TTX_WGL_CELL_SET_SW_DHDS_UP(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x08000000)
#define SM_TTX_WGL_CELL_CLR_SW_DHDS_UP(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x08000000)

/* SW Double-Height/Size Lower */
#define SM_TTX_WGL_CELL_HAS_SW_DHDS_LOW(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x10000000)
#define SM_TTX_WGL_CELL_SET_SW_DHDS_LOW(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x10000000)
#define SM_TTX_WGL_CELL_CLR_SW_DHDS_LOW(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x10000000)

/* SW Newsflash Set */
#define SM_TTX_WGL_CELL_HAS_SW_NS(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x20000000)
#define SM_TTX_WGL_CELL_SET_SW_NS(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x20000000)
#define SM_TTX_WGL_CELL_CLR_SW_NS(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x20000000)

/* SW Flash Set */
#define SM_TTX_WGL_CELL_HAS_SW_FLASH(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x40000000)
#define SM_TTX_WGL_CELL_SET_SW_FLASH(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x40000000)
#define SM_TTX_WGL_CELL_CLR_SW_FLASH(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x40000000)

/* SW Separate under Set */
#define SM_TTX_WGL_CELL_HAS_SW_SEP_UNDER(pt_cell)  \
    SM_TTX_WGL_CELL_HAS_B2(pt_cell, 0x80000000)
#define SM_TTX_WGL_CELL_SET_SW_SEP_UNDER(pt_cell) \
    SM_TTX_WGL_CELL_SET_FLAG_B2(pt_cell, 0x80000000)
#define SM_TTX_WGL_CELL_CLR_SW_SEP_UNDER(pt_cell) \
    SM_TTX_WGL_CELL_CLR_B2(pt_cell, 0x80000000)


#else

/* SW Mosaics */
#define SM_TTX_WGL_CELL_HAS_SW_MOS(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_MOS(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_MOS(pt_cell)

/* G0 Select */
#define SM_TTX_WGL_CELL_HAS_G0_SEL(pt_cell)
#define SM_TTX_WGL_CELL_SET_G0_SEL(pt_cell)
#define SM_TTX_WGL_CELL_CLR_G0_SEL(pt_cell)

/* SW Foreground Set */
#define SM_TTX_WGL_CELL_HAS_SW_FG(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_FG(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_FG(pt_cell)

/* SW Background Set */
#define SM_TTX_WGL_CELL_HAS_SW_BG(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_BG(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_BG(pt_cell)

/* SW Conceal */
#define SM_TTX_WGL_CELL_HAS_SW_CCL(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_CCL(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_CCL(pt_cell)

/* SW Invert */
#define SM_TTX_WGL_CELL_HAS_SW_INV(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_INV(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_INV(pt_cell)

/* SW Double-Height */
#define SM_TTX_WGL_CELL_HAS_SW_DH(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_DH(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_DH(pt_cell)

/* SW Double-Width */
#define SM_TTX_WGL_CELL_HAS_SW_DW(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_DW(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_DW(pt_cell)

/* SW Double-Height/Size Upper */
#define SM_TTX_WGL_CELL_HAS_SW_DHDS_UP(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_DHDS_UP(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_DHDS_UP(pt_cell)

/* SW Double-Height/Size Lower */
#define SM_TTX_WGL_CELL_HAS_SW_DHDS_LOW(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_DHDS_LOW(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_DHDS_LOW(pt_cell)

/* SW Newsflash Set */
#define SM_TTX_WGL_CELL_HAS_SW_NS(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_NS(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_NS(pt_cell)

/* SW Flash Set */
#define SM_TTX_WGL_CELL_HAS_SW_FLASH(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_FLASH(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_FLASH(pt_cell)

/* SW Separate under Set */
#define SM_TTX_WGL_CELL_HAS_SW_SEP_UNDER(pt_cell)
#define SM_TTX_WGL_CELL_SET_SW_SEP_UNDER(pt_cell)
#define SM_TTX_WGL_CELL_CLR_SW_SEP_UNDER(pt_cell)

#endif

#endif /* _X_SM_TTX_COMMON_EX_H_ */

