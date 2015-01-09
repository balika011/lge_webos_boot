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
 * $RCSfile: x_mtp.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 34076dbf72c1e756a741024f89fac47f $
 *
 * Description: 
 *         This file contains File Manager exported API's.
 *---------------------------------------------------------------------------*/
#ifndef _X_MTP_H_
#define _X_MTP_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_lnk_list.h"
#include "u_dm.h"
#include "x_dm.h"
#include "u_mtp.h"
#include "x_mtp_req.h"
#include "x_rm_dev_types.h"
#include "x_rm.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/
extern INT32 mtp_init(VOID);
extern INT32 mtp_uninit(VOID);


typedef struct
{
    INT32 (*mtp_init) (VOID);
    INT32 (*mtp_uninit) (VOID);
} MTP_IF_T;

extern INT32 x_mtp_get_if(VOID **pv_interface);

extern INT32 x_mtp_dev_open(DM_DEV_STAT_T*  pt_cfg,
                            HANDLE_T*       ph_mtp_dev);

extern INT32 x_mtp_dev_open_by_type (DRV_TYPE_T     e_drv_type,
                                     UINT16         ui2_unit,
                                     HANDLE_T*      ph_mtp_dev);
                              

extern VOID x_mtp_close(HANDLE_T    h_mtp_dev);

extern INT32 x_mtp_request (HANDLE_T            h_mtp_dev,
                            MTP_REQ_TYPE_T      e_req_type,
                            VOID*               pv_req_info,
                            UINT16*             pui2_response_code,
                            UINT8               ui1_pri,
                            x_mtp_async_fct     pf_nfy_fct,
                            VOID*               pv_tag,
                            VOID*               pv_data,
                            HANDLE_T*           ph_req);


extern INT32 x_mtp_dev_heap_status (HANDLE_T    h_mtp_dev,
                                    HANDLE_T*   ph_mem_part,
                                    SIZE_T*     pz_total_free,
                                    SIZE_T*     pz_biggest_free,
                                    SIZE_T*     pz_free_obj,
                                    SIZE_T*     pz_allocated_obj,
                                    SIZE_T*     pz_total_size,
                                    SIZE_T*     pz_alloc_size,
                                    CHAR*       s_name);

extern INT32 x_mtp_request_sync (HANDLE_T           h_mtp_dev,
                                 MTP_REQ_TYPE_T     e_req_type,
                                 VOID*              pv_req_info,
                                 UINT16*            pui2_response_code,
                                 UINT8              ui1_pri,
                                 MTP_ASYNC_COND_T*  pe_async_cond);

              
#endif /* _X_MTP_H_ */
