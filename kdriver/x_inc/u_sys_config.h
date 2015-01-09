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
/*! @file sys_config.h
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * @par Description:
 *         This header file contains sorting and searching function API for
 *         the applications.
 *
 *---------------------------------------------------------------------------*/


#ifndef _U_SYS_CONFIG_H_
#define _U_SYS_CONFIG_H_


/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "u_common.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/


/*=============================For system config customization =====================*/
typedef enum
{
    /********** read only part ***********/
    /* in fact. DIVX is controled by DIVX module */
    MW_CFG_IDX_DIVX,
    
    /********** read-write part ***********/
    MW_CFG_IDX_DUAL_BANK,
    MW_CFG_IDX_CH_RECORD,    
    MW_CFG_IDX_LAST_SCR_MODE, /*Last screen mode of last source.*/
    MW_CFG_IDX_INP_SRC_BLOCK, /*the input source block status*/
    MW_CFG_IDX_VOLUME,
    MW_CFG_IDX_MAX
}MW_CFG_IDX_T;

#define MW_CFG_SIZE_DIVX                ((UINT8)    64)
#define MW_CFG_SIZE_DUAL_BANK           ((UINT8)    1)
#define MW_CFG_SIZE_CH_RECORD           ((UINT8)    32) /*reduce pre_select EEP after mem_shrink.*/
#define MW_CFG_SIZE_LST_SCRM            ((UINT8)    6) /* for Pre-unmute feature*/
#define MW_CFG_SIZE_INP_SRC_BLOCK       ((UINT8)    1) 
#define MW_CFG_SIZE_VOLUME              ((UINT8)    1)

/*=================================================================================*/


#define SYS_CFGR_OK             ((INT32)   0)
#define SYS_CFGR_INV_ARG        ((INT32)  -1)
#define SYS_CFGR_OUT_OF_RANGE   ((INT32)  -2)
#define SYS_CFGR_NOT_FOUND      ((INT32)  -3)
#define SYS_CFGR_READ_FAIL      ((INT32)  -4)
#define SYS_CFGR_WRITE_FAIL     ((INT32)  -5)
#define SYS_CFGR_SEEK_FAIL      ((INT32)  -6)
#define SYS_CFGR_FAIL           ((INT32)  -7)
#define SYS_CFGR_INIT_FAIL      ((INT32)  -8)


#endif /* _U_SYS_CONFIG_H_ */





