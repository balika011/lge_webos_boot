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
 * $RCSfile: x_dsmcc_db.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains DSMCC Data Broadcast specific
 *         definitions, which are exported.
 *-------------------------------------------------------------------------*/

#ifndef _X_DSMCC_DB_H_
#define _X_DSMCC_DB_H_


/*---------------------------------------------------------------------------
                    include files
---------------------------------------------------------------------------*/

#include "x_handle_grp.h"
#include "x_common.h"
#include "x_tm.h"
#include "u_dsmcc_db.h"


/*---------------------------------------------------------------------------
                    functions declarations
 --------------------------------------------------------------------------*/

extern INT32 x_dsmcc_db_load_dsi_by_pid_ts_id (HANDLE_T       h_root,
                                               MPEG_2_PID_T   t_pid,
                                               UINT16         ui2_ts_id,
                                               TM_SRC_TYPE_T  e_src_type,
                                               VOID*          pv_src_info,
                                               VOID*          pv_nfy_tag,
                                               x_tm_nfy_fct   pf_nfy,
                                               HANDLE_T*      ph_dsi,
                                               TM_COND_T*     pe_cond);
extern INT32 x_dsmcc_db_load_dsi_by_pid_ts_descr (HANDLE_T       h_root,
                                                  MPEG_2_PID_T   t_pid,
                                                  TS_DESCR_T*    pt_ts_descr,
                                                  TM_SRC_TYPE_T  e_src_type,
                                                  VOID*          pv_src_info,
                                                  VOID*          pv_nfy_tag,
                                                  x_tm_nfy_fct   pf_nfy,
                                                  HANDLE_T*      ph_dsi,
                                                  TM_COND_T*     pe_cond);

extern INT32 x_dsmcc_db_load_module_async (HANDLE_T      h_dii,
                                           UINT16        ui2_module_idx,
                                           UINT32        ui4_offs,
                                           SIZE_T        z_buff_len,
                                           UINT8*        pui1_buff,
                                           VOID*         pv_nfy_tag,
                                           x_tm_nfy_fct  pf_nfy,
                                           HANDLE_T*     ph_db_module,
                                           TM_COND_T*    pe_cond);

extern INT32 x_dsmcc_db_load_module_async_ex (HANDLE_T      h_dii,
                                              UINT16        ui2_module_idx,
                                              UINT32        ui4_offs,
                                              SIZE_T        z_buff_len,
                                              UINT8*        pui1_buff,
                                              UINT32        ui4_offs_2,
                                              SIZE_T        z_buff_len_2,
                                              UINT8*        pui1_buff_2,
                                              VOID*         pv_nfy_tag,
                                              x_tm_nfy_fct  pf_nfy,
                                              HANDLE_T*     ph_db_module,
                                              TM_COND_T*    pe_cond);

extern INT32 x_dsmcc_db_get_num_groups (HANDLE_T    h_dsi,
                                        UINT16*     pui2_num_groups,
                                        TM_COND_T*  pe_cond);
extern INT32 x_dsmcc_db_get_group_info (HANDLE_T                h_dsi,
                                        UINT16                  ui2_group_idx,
                                        DSMCC_DB_SPEC_T         e_dsmcc_db_spec,
                                        DSMCC_DB_GROUP_INFO_T*  pt_group_info,
                                        TM_COND_T*              pe_cond);

extern INT32 x_dsmcc_db_get_module_by_idx (HANDLE_T    h_dii,
                                           UINT16      ui2_module_idx,
                                           UINT32      ui4_offs,
                                           SIZE_T      z_buff_len,
                                           UINT8*      pui1_buff,
                                           SIZE_T*     pz_buff_loaded,
                                           TM_COND_T*  pe_cond);
extern INT32 x_dsmcc_db_get_module_by_idx_timeout (HANDLE_T    h_dii,
                                                   UINT16      ui2_module_idx,
                                                   UINT32      ui4_offs,
                                                   SIZE_T      z_buff_len,
                                                   UINT8*      pui1_buff,
                                                   SIZE_T*     pz_buff_loaded,
                                                   UINT32      ui4_timeout,
                                                   TM_COND_T*  pe_cond);

extern INT32 x_dsmcc_db_parse_group_all_comp_descr (HANDLE_T        h_dsi,
                                                    UINT16          ui2_group_idx,
                                                    DSMCC_DB_SPEC_T e_dsmcc_db_spec,
                                                    VOID*           pv_data,
                                                    SIZE_T          z_data_len,
                                                    x_tm_parse_fct  pf_parse,
                                                    TM_COND_T*      pe_cond);
extern INT32 x_dsmcc_db_parse_group_info (HANDLE_T        h_dsi,
                                          UINT16          ui2_group_idx,
                                          DSMCC_DB_SPEC_T e_dsmcc_db_spec,
                                          VOID*           pv_data,
                                          SIZE_T          z_data_len,
                                          x_tm_parse_fct  pf_parse,
                                          TM_COND_T*      pe_cond);
extern INT32 x_dsmcc_db_parse_group_private_data (HANDLE_T        h_dsi,
                                                  DSMCC_DB_SPEC_T e_dsmcc_db_spec,
                                                  VOID*           pv_data,
                                                  SIZE_T          z_data_len,
                                                  x_tm_parse_fct  pf_parse,
                                                  TM_COND_T*      pe_cond);

extern INT32 x_dsmcc_db_new_ddb_status (DATA_OBJ_T*  pt_data);

extern INT32 x_dsmcc_db_new_ddb_status_ex (DATA_OBJ_T*  pt_data);

#endif /* _X_DSMCC_DB_H_ */

