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
 * $RCSfile: x_mhp_eng.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains MHP engine specific definitions, which
 *         are exported.
 *------------------------------------------------------------------------*/

#ifndef _X_MHP_ENG_H_
#define _X_MHP_ENG_H_


/*--------------------------------------------------------------------------
                    include files
--------------------------------------------------------------------------*/

#include "x_handle_grp.h"
#include "x_common.h"
#include "x_tm.h"
#include "u_mhp_eng.h"

/*--------------------------------------------------------------------------
                    functions declarations
 -------------------------------------------------------------------------*/

extern INT32 x_mhp_get_app_entry (HANDLE_T          h_ait,
                                  UINT16            ui2_idx,
                                  MHP_APP_INFO_T*   pt_app_entry,
                                  TM_COND_T*        pe_cond);

extern INT32 x_mhp_get_num_apps (HANDLE_T    h_ait,
                                 UINT16*     pui2_num_apps,
                                 TM_COND_T*  pe_cond);

extern INT32 x_mhp_get_app_type (HANDLE_T    h_ait,
                                 UINT16*     pui2_app_type,
                                 TM_COND_T*  pe_cond);

extern INT32 x_mhp_init (const CHAR*    ps_name,
                         MHP_CONFIG_T*  pt_config);
#if 0
extern INT32 x_mhp_load_ait_by_pid_app_type_ver (HANDLE_T       h_root,
                                                 MPEG_2_PID_T   t_pid,
                                                 UINT16         ui2_app_type,
                                                 UINT8          ui1_version,
                                                 TS_DESCR_T*    pt_ts_descr,
                                                 TM_SRC_TYPE_T  e_src_type,
                                                 VOID*          pv_src_info,
                                                 VOID*          pv_nfy_tag,
                                                 x_tm_nfy_fct   pf_nfy,
                                                 HANDLE_T*      ph_ait,
                                                 TM_COND_T*     pe_cond);
#endif
extern INT32 x_mhp_load_ait_by_pid (HANDLE_T       h_root,
                                     MPEG_2_PID_T   t_pid,
                                     VOID*          pv_nfy_tag,
                                     x_tm_nfy_fct   pf_nfy,
                                     HANDLE_T*      ph_ait,
                                     TM_COND_T*     pe_cond);

extern INT32 x_mhp_load_ait_by_pid_ex (HANDLE_T       h_root,
                                         MPEG_2_PID_T   t_pid,
                                         VOID*          pv_nfy_tag,
                                         x_tm_nfy_fct   pf_nfy,
                                         HANDLE_T*      ph_ait,
                                         TM_COND_T*     pe_cond);

extern INT32 x_mhp_load_ait_by_pid_app_type (HANDLE_T       h_root,
                                             MPEG_2_PID_T   t_pid,
                                             UINT16         ui2_app_type,
                                             VOID*          pv_nfy_tag,
                                             x_tm_nfy_fct   pf_nfy,
                                             HANDLE_T*      ph_ait,
                                             TM_COND_T*     pe_cond);

extern INT32 x_mhp_load_ait_by_pid_app_type_ex (HANDLE_T       h_root,
                                             MPEG_2_PID_T   t_pid,
                                             UINT16         ui2_app_type,
                                             VOID*          pv_nfy_tag,
                                             x_tm_nfy_fct   pf_nfy,
                                             HANDLE_T*      ph_ait,
                                             TM_COND_T*     pe_cond);

#if 0
extern INT32 x_mhp_load_xait_by_pid_app_type_ver (HANDLE_T       h_root,
                                                  MPEG_2_PID_T   t_pid,
                                                  UINT16         ui2_app_type,
                                                  UINT8          ui1_version,
                                                  TS_DESCR_T*    pt_ts_descr,
                                                  TM_SRC_TYPE_T  e_src_type,
                                                  VOID*          pv_src_info,
                                                  VOID*          pv_nfy_tag,
                                                  x_tm_nfy_fct   pf_nfy,
                                                  HANDLE_T*      ph_xait,
                                                  TM_COND_T*     pe_cond);
#endif
extern INT32 x_mhp_load_xait_by_pid_app_type (HANDLE_T       h_root,
                                              MPEG_2_PID_T   t_pid,
                                              UINT16         ui2_app_type,
                                              VOID*          pv_nfy_tag,
                                              x_tm_nfy_fct   pf_nfy,
                                              HANDLE_T*      ph_xait,
                                              TM_COND_T*     pe_cond);

extern INT32 x_mhp_txt_loc_descr ( 
                    HANDLE_T            h_obj, 
                    const UINT8*        pui1_txt, 
                    SIZE_T              z_txt_len,
                    ISO_3166_COUNT_T    t_count_code,
                    HANDLE_T*           ph_txt
                    ); 

extern INT32 x_mhp_get_txt_len ( 
                    HANDLE_T            h_txt, 
                    MHP_GET_TXT_MODE_T e_get_mode, 
                    SIZE_T*             pz_txt_len
                    );

extern INT32 x_mhp_get_txt ( 
                    HANDLE_T            h_txt, 
                    MHP_GET_TXT_MODE_T e_get_mode, 
                    CHAR*               ps_txt, 
                    SIZE_T*             pz_txt_len
                    ); 

extern INT32 x_mhp_get_txt_style_size ( 
                    HANDLE_T            h_txt, 
                    SIZE_T*             pz_style_str_size
                    ); 

extern INT32 x_mhp_get_txt_style ( 
                    HANDLE_T            h_txt, 
                    STYLE_STR_T*        pt_style_str, 
                    SIZE_T*             pz_style_str_size
                    ); 

#endif /* _X_MHP_ENG_H_ */
