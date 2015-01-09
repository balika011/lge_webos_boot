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
 * $RCSfile: ca_svl_builder.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This header file contains private macros, defines, 
 *         typedefs, and enums, which are required by CA SVL Builder engine.
 *---------------------------------------------------------------------------*/

#ifndef _X_CA_SVL_BUILDER_H_
#define _X_CA_SVL_BUILDER_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_handle.h"

#include "x_sb.h"
#include "x_sb_eng.h"
#include "u_sb_dvb_cp_eng.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define CSBR_OK                         ((INT32)     0)
#define CSBR_ALREADY_INIT               ((INT32)    -1)
#define CSBR_BLDR_NOT_FOUND             ((INT32)    -2)
#define CSBR_DUP_NAME                   ((INT32)    -3)
#define CSBR_FILE_NOT_FOUND             ((INT32)    -4)
#define CSBR_INTERNAL_ERROR             ((INT32)    -5)
#define CSBR_INV_ARG                    ((INT32)    -6)
#define CSBR_INV_COND                   ((INT32)    -7)
#define CSBR_INV_HANDLE                 ((INT32)    -8)
#define CSBR_LIST_NOT_FOUND             ((INT32)    -9)
#define CSBR_NOT_INIT                   ((INT32)   -10)
#define CSBR_NOT_MATCH                  ((INT32)   -11)
#define CSBR_NOT_STARTER                ((INT32)   -12)
#define CSBR_OUT_OF_HANDLES             ((INT32)   -13)
#define CSBR_OUT_OF_MEM                 ((INT32)   -14)
#define CSBR_ERR_LOCK                   ((INT32)   -15)
#define CSBR_ERR_UNLOCK                 ((INT32)   -16)


typedef enum
{
    CA_SB_KEY_TYPE_UNKNOWN = 0,
    CA_SB_KEY_TYPE_SET_1,
    CA_SB_KEY_TYPE_GET_1,
}   CA_SB_KEY_TYPE_T;


typedef enum
{
    CA_SB_NFY_REASON_UNKNOWN = 0,
    CA_SB_NFY_REASON_FINE_TUNE_COMPLETE,
    CA_SB_NFY_REASON_SCAN_COMPLETE,
    CA_SB_NFY_REASON_SCAN_CANCEL,
    CA_SB_NFY_REASON_LAST_CASE
}   CA_SB_NFY_REASON_T;


typedef struct _CA_SB_OPERATOR_TUNE_STATUS_T
{
    UINT8                       ui1_signal_strength;
    UINT8                       ui1_signal_quality;
    UINT8                       ui1_status;
    UINT16                      ui2_descripor_number;
    UINT8*                      pui1_descriptor;
    UINT16                      ui2_descriptor_len;
} CA_SB_OPERATOR_TUNE_STATUS_T;


typedef struct _CA_SB_NFY_DATA_T
{
    CA_SB_NFY_REASON_T        e_nfy_reason;

    union
    {
        CA_SB_OPERATOR_TUNE_STATUS_T  t_tune_status;
    } u_data;    
} CA_SB_NFY_DATA_T;


typedef struct _CA_SB_OPEN_DATA_T
{
    UINT16                      ui2_svl_id;
    CHAR*                       ps_tuner_name;    
    SB_DVB_DELIVERY_TYPE        t_delivery_type;
} CA_SB_OPEN_DATA_T;


#if 0
typedef struct _CA_SB_CALLBACK_TAG_T
{
    VOID*                       pv_ca_sb_nfy_tag1;
    VOID*                       pv_ca_sb_nfy_tag2;
    VOID*                       pv_ca_sb_nfy_tag3;
    VOID*                       pv_ca_sb_nfy_tag4;
    VOID*                       pv_ca_sb_nfy_tag5;
} CA_SB_CALLBACK_TAG_T;
#endif


typedef VOID (*ca_sb_scan_nfy_fct) (
      HANDLE_T                  h_builder,
      CA_SB_NFY_DATA_T*         pt_nfy_data,
      VOID*                     pv_tag );


typedef struct _CA_SB_SCAN_DATA_T
{
    ISO_3166_COUNT_T            t_country_code;
    SB_DVB_CONFIG_T             t_cp_eng_cfg;
    ca_sb_scan_nfy_fct          pf_ca_sb_scan_nfy;
    VOID*                       pv_tag;
    SB_DVB_CP_STRING_PREFIX_T   t_cp_string_prefix;
    UINT16                      ui2_refer_brdcst_satl_id;
} CA_SB_SCAN_DATA_T;


typedef struct _CA_SB_TUNE_INFO_T
{
    UINT8*                      pui1_tune_data;
    UINT32                      ui4_data_len;
} CA_SB_TUNE_INFO_T;


typedef struct _CA_SB_CI_NIT_T
{
    UINT8*                      pui1_ci_nit_data;
    UINT32                      ui4_data_len;
} CA_SB_CI_NIT_T;


/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
INT32 ca_sb_open_builder(
        const CHAR*             ps_builder_name,
        CA_SB_OPEN_DATA_T*      pt_open_data,
        HANDLE_T*               ph_builder);


INT32 ca_sb_close_builder(
        HANDLE_T                h_builder);


INT32 ca_sb_start_scan(
        HANDLE_T                h_builder,
        CA_SB_SCAN_DATA_T*      pt_scan_data);


INT32 ca_sb_cancel_scan(
        HANDLE_T                h_builder);


INT32 ca_sb_start_monitor(
        HANDLE_T                h_builder);


INT32 ca_sb_stop_monitor(
        HANDLE_T                h_builder);


INT32 ca_sb_send_fine_tune_info(
        HANDLE_T                h_builder,
        CA_SB_TUNE_INFO_T*      pt_tune_info);


INT32 ca_sb_send_fine_tune_info_complete(
        HANDLE_T                h_builder );

INT32 ca_sb_send_ci_nit(
        HANDLE_T                h_builder,
        CA_SB_CI_NIT_T*         pt_ci_nit_info);


INT32 ca_sb_set(
        HANDLE_T                h_builder,
        CA_SB_KEY_TYPE_T        e_set_type,
        VOID*                   pv_set_type_info,
        SIZE_T                  z_set_content_len,
        VOID*                   pv_set_content);


INT32 ca_sb_get(
        HANDLE_T                h_builder,
        CA_SB_KEY_TYPE_T        e_get_type,
        VOID*                   pv_get_type_info,
        SIZE_T                  z_get_content_len,
        VOID*                   pv_get_content);


#endif /* _X_CA_SVL_BUILDER_H_ */



