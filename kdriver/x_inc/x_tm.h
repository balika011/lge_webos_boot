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
 * $RCSfile: x_tm.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Table Manager specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_TM_H_
#define _X_TM_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_handle.h"
#include "x_common.h"
#include "x_sys_name.h"
#include "u_tm.h"

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

extern INT32 x_tm_change_src (HANDLE_T       h_obj,
                              TM_SRC_TYPE_T  e_src_type,
                              VOID*          pv_src_info,
                              TM_COND_T*     pe_cond);
extern INT32 x_tm_delete_obj (HANDLE_T  h_obj);
extern BOOL x_tm_find_root (const CHAR*  ps_name);
extern INT32 x_tm_free (HANDLE_T  h_obj);
extern INT32 x_tm_get_cond (HANDLE_T    h_obj,
                            TM_COND_T*  pe_cond);
extern INT32 x_tm_get_obj (HANDLE_T       h_obj,
                           TM_GET_TYPE_T  e_get_type,
                           VOID*          pv_get_info,
                           VOID*          pv_data,
                           SIZE_T*        pz_data_len,
                           TM_COND_T*     pe_cond);
extern INT32 x_tm_get_protection (HANDLE_T    h_obj,
                                  BOOL*       pb_protected,
                                  TM_COND_T*  pe_cond);
extern INT32 x_tm_get_tbl_version (HANDLE_T    h_obj,
                                   UINT8*      pui1_version,
                                   TM_COND_T*  pe_cond);
extern INT32 x_tm_load_obj (HANDLE_T        h_parent,
                            HANDLE_TYPE_T   e_obj_type,
                            TM_LOAD_TYPE_T  e_load_type,
                            VOID*           pv_load_info,
                            TM_SRC_TYPE_T   e_src_type,
                            VOID*           pv_src_info,
                            VOID*           pv_nfy_tag,
                            x_tm_nfy_fct    pf_nfy,
                            HANDLE_T*       ph_obj,
                            TM_COND_T*      pe_cond);
extern INT32 x_tm_monitor_obj (HANDLE_T       h_parent,
                               HANDLE_TYPE_T  e_obj_type,
                               TM_MON_TYPE_T  e_mon_type,
                               VOID*          pv_mon_info,
                               TM_SRC_TYPE_T  e_src_type,
                               VOID*          pv_src_info,
                               VOID*          pv_nfy_tag,
                               x_tm_nfy_fct   pf_nfy,
                               HANDLE_T*      ph_obj,
                               TM_COND_T*     pe_cond);
extern INT32 x_tm_open_root (const CHAR*    ps_name,
                             TM_SRC_TYPE_T  e_src_type,
                             VOID*          pv_src_info,
                             VOID*          pv_nfy_tag,
                             x_tm_nfy_fct   pf_nfy,
                             HANDLE_T*      ph_root,
                             TM_COND_T*     pe_cond);
extern INT32 x_tm_parse_obj (HANDLE_T         h_obj,
                             TM_PARSE_TYPE_T  e_parse_type,
                             VOID*            pv_parse_info,
                             VOID*            pv_data,
                             SIZE_T           z_data_len,
                             x_tm_parse_fct   pf_parse,
                             TM_COND_T*       pe_cond);
extern INT32 x_tm_set_obj (HANDLE_T       h_obj,
                           TM_SET_TYPE_T  e_set_type,
                           VOID*          pv_set_info,
                           SIZE_T         z_len,
                           TM_COND_T*     pe_cond);
extern INT32 x_tm_set_protection (HANDLE_T    h_obj,
                                  BOOL        b_protected,
                                  TM_COND_T*  pe_cond);
extern INT32 x_tm_store_obj (HANDLE_T         h_obj,
                             TM_STORE_TYPE_T  e_store_type,
                             VOID*            pv_store_info,
                             TM_COND_T*       pe_cond);


extern INT32 x_tm_refresh_obj (HANDLE_T    h_obj,
                        TM_COND_T*  pe_cond);


extern INT32 x_tm_get_conn_status(HANDLE_T     h_root,
                                  BOOL*        pb_connect);


#endif /* _X_TM_H */
