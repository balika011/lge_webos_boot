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
 * $RCSfile: x_rec_demux.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Record Demux driver specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_REC_DEMUX_H_
#define _X_REC_DEMUX_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Get operations */
#define RECDEMUX_GET_TYPE_PID               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  0))
#define RECDEMUX_GET_TYPE_CTRL              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  1))

/* Set operations */
#define RECDEMUX_SET_TYPE_PID              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  0)) | RM_SET_TYPE_ARG_NO_REF)
#define RECDEMUX_SET_TYPE_DEST_ID           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  1))
#define RECDEMUX_SET_TYPE_NFY_FCT           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  2))
#define RECDEMUX_SET_TYPE_CTRL             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  3)) | RM_SET_TYPE_ARG_NO_REF)
#define RECDEMUX_SET_TYPE_SCRS_NFY_FCT      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  4))
#define RECDEMUX_SET_TYPE_STREAM_TYPE_MASK ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  5)) | RM_SET_TYPE_ARG_NO_REF)

/* record demux condition */
typedef enum _RECDEMUX_COND_T
{
    RECDEMUX_COND_ERROR,
    RECDEMUX_COND_SCRS    
} RECDEMUX_COND_T;

typedef enum
{
    RECDEMUX_SCRS_UNKNOWN = 0,
    RECDEMUX_SCRS_CLEAR,
    RECDEMUX_SCRS_SCRAMBLED
}   RECDEMUX_SCRS_T;

/* Notify function */
typedef BOOL (*x_recdemux_nfy_fct) (VOID*            pv_nfy_tag,
                                    RECDEMUX_COND_T  e_nfy_cond,
                                    UINT32           ui4_data);


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/* Destination type and id info */
typedef struct _RECDEMUX_DEST_INFO_T
{
    DRV_TYPE_T      e_type;

    UINT16          ui2_id;
    
}   RECDEMUX_DEST_INFO_T;

/* Notify info setting */
typedef struct _RECDEMUX_NFY_INFO_T
{
    VOID*               pv_tag;

    x_recdemux_nfy_fct  pf_recdemux_nfy;
    
} RECDEMUX_NFY_INFO_T;


#endif /* _X_REC_DEMUX_H_ */
