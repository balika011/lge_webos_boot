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
 * $RCSfile: x_sm_cc_hdlr.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/11 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: f6ce0384875c26c9670d7f9f39606dc7 $
 *
 * Description: 
 *         This header file contains Closed Caption Stream handler specific 
 *         definitions, which are exported to applications.
 *---------------------------------------------------------------------------*/
     


#ifndef _X_SM_CC_HDLR_H_
#define _X_SM_CC_HDLR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_sm_cc_hdlr.h"
#include "u_sm_event.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define SM_EVN_CCS_GRP_SIZE  ((SM_EVN_TYPE_T) 0x00010000)
    
#define SM_EVN_CCS_GRP_APP   ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + SM_EVN_CCS_GRP_SIZE*0 )
#define SM_EVN_CCS_GRP_DEC   ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + SM_EVN_CCS_GRP_SIZE*1 )
#define SM_EVN_CCS_GRP_TVE   ((SM_EVN_TYPE_T) SM_EVN_GRP_CCS + SM_EVN_CCS_GRP_SIZE*2 )
    
#define SM_EVN_CCS_CODE_DRV_OPENED       ((SM_EVN_TYPE_T) 0)
#define SM_EVN_CCS_CODE_DRV_CLOSED       ((SM_EVN_TYPE_T) 1)
#define SM_EVN_CCS_CODE_DRV_PLAYED       ((SM_EVN_TYPE_T) 2)
#define SM_EVN_CCS_CODE_DRV_STOPPED      ((SM_EVN_TYPE_T) 3)
    
#define SM_MAKE_DRV_EVN( grp, code ) ((SM_EVN_TYPE_T) grp + code )
    
#define SM_EVN_CCS_DEC_OPENED         SM_MAKE_DRV_EVN( SM_EVN_CCS_GRP_DEC, SM_EVN_CCS_CODE_DRV_OPENED )
#define SM_EVN_CCS_DEC_CLOSED         SM_MAKE_DRV_EVN( SM_EVN_CCS_GRP_DEC, SM_EVN_CCS_CODE_DRV_CLOSED )
#define SM_EVN_CCS_DEC_CC_PLAYED      SM_MAKE_DRV_EVN( SM_EVN_CCS_GRP_DEC, SM_EVN_CCS_CODE_DRV_PLAYED )
#define SM_EVN_CCS_DEC_CC_STOPPED     SM_MAKE_DRV_EVN( SM_EVN_CCS_GRP_DEC, SM_EVN_CCS_CODE_DRV_STOPPED )
    
#define SM_EVN_CCS_TVE_OPENED         SM_MAKE_DRV_EVN( SM_EVN_CCS_GRP_TVE, SM_EVN_CCS_CODE_DRV_OPENED )
#define SM_EVN_CCS_TVE_CLOSED         SM_MAKE_DRV_EVN( SM_EVN_CCS_GRP_TVE, SM_EVN_CCS_CODE_DRV_CLOSED )
#define SM_EVN_CCS_TVE_PLAYED         SM_MAKE_DRV_EVN( SM_EVN_CCS_GRP_TVE, SM_EVN_CCS_CODE_DRV_PLAYED )
#define SM_EVN_CCS_TVE_STOPPED        SM_MAKE_DRV_EVN( SM_EVN_CCS_GRP_TVE, SM_EVN_CCS_CODE_DRV_STOPPED )



#define SM_CC_HDLR_MSGQ_DFLT_MAX_SIZE           1024
#define SM_CC_HDLR_THRD_DFLT_PRIORITY           127
#define SM_CC_HDLR_THRD_DFLT_STACK_SIZE         4096*2


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
extern INT32 x_sm_cc_hdlr_init( const SM_CCH_CONFIG_T* pt_config );

extern INT32 x_sm_cc_hdlr_set_dtv_cc_att( 
    const SM_CCH_DTVCC_ATTRIBS_INFO_T* pt_dtvcc_att );

#endif /* _X_SM_CC_HDLR_H_ */
