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
/*----------------------------------------------------------------------------*/
/*! @file u_sb_dvb_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         SVL Builder DVB engine.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_SB_DVB_CP_ENG_H_
#define _U_SB_DVB_CP_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_tuner.h"

#include "u_sb.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVL_BLDR_SB_DVB_ENG SVL Builder DVB engine
 *
 *  @ingroup groupMW_SVL_BLDR
 *  @brief The DVB-T engine module is used to scan all digital channel in DVB-T spec.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could create scan engine
 *   or start scan from the exported API of DVB-T svl builder.
 *
 *  @see groupMW_SVL_BLDR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_tuner.h"

#include "u_sb_dvb_eng.h"


#if 1
#define DVB_CP_ENG_NAME             "sb_dvb_cp"    /**< Define the DVB build engine name */
#else
#define DVB_CP_ENG_NAME             DVB_ENG_NAME    /**< Define the DVB build engine name */
#endif

#define DVB_CP_ENG_SATELLITE_START_REC_ID                   ((UINT32)  0xCACA)
#define DVB_CP_ENG_SATELLITE_START_NAME                     "CA_reserved"


#define SB_DVB_CP_LOL_RECORD_ID_FOR_CONTENT_LABEL           ((UINT8)   100)
#define SB_DVB_CP_LOL_DATA_TAG_FOR_CONTENT_LABEL            ((UINT32)  100)

#define SB_DVB_CP_CONFIG_USER_TUNE_INFO                     (MAKE_BIT_MASK_32(0))
#define SB_DVB_CP_CONFIG_CAM_NIT                            (MAKE_BIT_MASK_32(1))
#define SB_DVB_CP_CONFIG_USER_OPERATOR                      (MAKE_BIT_MASK_32(2))
#define SB_DVB_CP_CONFIG_TBL_SRC                            (MAKE_BIT_MASK_32(3))
#define SB_DVB_CP_CONFIG_USER_OPERATOR_NFY                  (MAKE_BIT_MASK_32(4))



typedef enum _SB_DVB_DELIVERY_TYPE
{
    SB_DVB_DELIVERY_UNKNOWN = 0,
    SB_DVB_DELIVERY_DVBC,
    SB_DVB_DELIVERY_DVBC2,
    SB_DVB_DELIVERY_DVBS,
    SB_DVB_DELIVERY_DVBS2,
    SB_DVB_DELIVERY_DVBT,
    SB_DVB_DELIVERY_DVBT2,
    SB_DVB_DELIVERY_DVBSH,
    SB_DVB_DELIVERY_LAST_ONE
}   SB_DVB_DELIVERY_TYPE;


typedef struct _SB_DVB_C_DELIVERY_T
{
    UINT32                          ui4_frequency;
    UINT8                           ui1_FEC_outer;    
    TUNER_MODULATION_T              e_mod;
    UINT32                          ui4_symbol_rate;
    UINT8                           ui1_FEC_inner;
}   SB_DVB_C_DELIVERY_T;


typedef struct _SB_DVB_C2_DELIVERY_T
{
    UINT8                           ui1_plp_id;
    UINT8                           ui1_data_slice_id;
    UINT32                          ui4_C2_tuning_frequency;
    UINT8                           ui1_C2_tuning_frequency_type;
    UINT8                           ui1_active_OFDM_symbol_duration;
    UINT8                           ui1_guard_interval;
}   SB_DVB_C2_DELIVERY_T;


typedef struct _SB_DVB_S_DELIVERY_T
{
    UINT32                          ui4_frequency;          /* MHZ */
    UINT16                          ui2_orbital_position;   /* 10 degree */
    UINT8                           ui1_west_east_flag;     /* "1" indicates the eastern position */
    TUNER_POLARIZATION_T            t_polarization;
    UINT8                           ui1_roll_off;
    UINT8                           ui1_modulation_system;
    UINT8                           ui1_modulation_type;
    UINT32                          ui4_symbol_rate;        /* Ksymbol/s */
    UINT8                           ui1_FEC_inner;
}   SB_DVB_S_DELIVERY_T;


typedef struct _SB_DVB_S2_DELIVERY_T
{
    UINT8                           ui1_scrambling_sequence_selector;
    UINT8                           ui1_multiple_input_stream_flag;
    UINT8                           ui1_backwards_compatibility_indicator;
    UINT32                          ui4_scrambling_sequence_index;
    UINT8                           ui1_input_stream_identifier;
}   SB_DVB_S2_DELIVERY_T;


typedef struct _SB_DVB_T_DELIVERY_T
{
    UINT32                          ui4_centre_frequency;
    TUNER_BANDWIDTH_T               e_bandwidth;
    UINT8                           ui1_priority;
    UINT8                           ui1_time_slicing_indicator;
    UINT8                           ui1_MPE_FEC_indicator;
    UINT8                           ui1_constellation;
    UINT8                           ui1_hierarchy_information;
    UINT8                           ui1_code_rate_HP_stream;
    UINT8                           ui1_code_rate_LP_stream;
    UINT8                           ui1_guard_interval;
    UINT8                           ui1_transmission_mode;
    UINT8                           ui1_other_frequency_flag;
}   SB_DVB_T_DELIVERY_T;


typedef struct _SB_DVB_T2_DELIVERY_T
{
    UINT8                           ui1_plp_id;
    UINT16                          ui2_T2_system_id;
    BOOL                            fg_is_len_bigger_than4;
    
    UINT8                           ui1_SISO_MISO;
    TUNER_BANDWIDTH_T               e_bandwidth;
    UINT8                           ui1_reserved_future_use;
    UINT8                           ui1_guard_interval;
    UINT8                           ui1_transmission_mode;
    UINT8                           ui1_other_frequency_flag;
    UINT8                           ui1_tfs_flag;
    UINT16                          ui2_cell_id;
    UINT32                          ui4_centre_frequency;
}   SB_DVB_T2_DELIVERY_T;


typedef struct _SB_DVB_DELIVERY_T
{
    SB_DVB_DELIVERY_TYPE            t_type;
    
    union {
        SB_DVB_T_DELIVERY_T         t_t_delivery;
        SB_DVB_C_DELIVERY_T         t_c_delivery;
        SB_DVB_S_DELIVERY_T         t_s_delivery;
        SB_DVB_T2_DELIVERY_T        t_t2_delivery;
        SB_DVB_C2_DELIVERY_T        t_c2_delivery;
        SB_DVB_S2_DELIVERY_T        t_s2_delivery;
    } data;
}   SB_DVB_DELIVERY_T;


typedef enum _SB_DVB_CP_CUR_SIGNAL_STATUS
{
    DVB_CP_SIGNAL_STATUS_UNKNOWN    = 0,
    DVB_CP_SIGNAL_STATUS_AVAILABLE,
    DVB_CP_SIGNAL_STATUS_NO_SUPPORT,
    DVB_CP_SIGNAL_STATUS_PARA_INVALID,
    DVB_CP_SIGNAL_STATUS_NO_SIGNAL,
    DVB_CP_SIGNAL_STATUS_LAST_ONE
}   SB_DVB_CP_CUR_SIGNAL_STATUS;


typedef struct _SB_DVB_CP_USER_OP_DATA_T
{
    UINT8                           ui1_signal_strength;
    UINT8                           ui1_signal_quality;
    SB_DVB_CP_CUR_SIGNAL_STATUS     t_signal_status;
    UINT16                          ui2_descripor_number;
    UINT8*                          pui1_descriptor;
    UINT16                          ui2_descriptor_len;
} SB_DVB_CP_USER_OP_DATA_T;


typedef VOID (*FUNC_SB_DVBCP_USER_OP_NFY) (
      SB_DVB_CP_USER_OP_DATA_T* pt_dvbcp_user_op,
      VOID*                     pv_tag );


typedef UINT32                      SB_DVB_SRC_TBL_MASK;
#define SB_DVB_SRC_NONE_MASK        ((UINT32) 0)
#define SB_DVB_SRC_SDT_MASK         MAKE_BIT_MASK_32 (0)
#define SB_DVB_SRC_NIT_MASK         MAKE_BIT_MASK_32 (1)
#define SB_DVB_SRC_CAM_SDT_MASK     MAKE_BIT_MASK_32 (2)
#define SB_DVB_SRC_CAM_NIT_MASK     MAKE_BIT_MASK_32 (3)

#define SB_DVB_SRC_TABLE            (SB_DVB_SRC_SDT_MASK | SB_DVB_SRC_NIT_MASK)


#define SB_DVB_CP_STRING_PREFIX_MAX_LEN     10
typedef struct _SB_DVB_CP_STRING_PREFIX_T
{
    CHAR                            ac_prefix[SB_DVB_CP_STRING_PREFIX_MAX_LEN];
    UINT16                          ui2_valid_len;
}   SB_DVB_CP_STRING_PREFIX_T;


typedef struct _SB_DVB_CP_SCAN_DATA_T
{
    SB_DVB_SCAN_DATA_T              t_dvbt_scan_data;
    SB_DVB_CONFIG_T                 t_cp_eng_cfg;
    SB_DVB_SRC_TBL_MASK             t_table_mask;
    FUNC_SB_DVBCP_USER_OP_NFY       pf_sb_dvbcp_user_op_nfy;
    VOID*                           pv_user_op_nfy_tag;
    SB_DVB_CP_STRING_PREFIX_T       t_cp_string_prefix;
    UINT16                          ui2_refer_brdcst_satl_id;
} SB_DVB_CP_SCAN_DATA_T;


#define CP_ENG_SCAN_CFG(x)      (x & pt_cp_scan_data->t_cp_eng_cfg)
#define CP_ENG_CFG(x)           (x & pt_cp_eng_data->t_user_scan_data.t_cp_eng_cfg)


typedef struct _SB_DVB_CAM_DELIVERY_T
{
    UINT16                          ui2_descripor_number;
    UINT16                          ui2_desc_len;
    UINT8*                          pui1_delivery_desc;
}   SB_DVB_CAM_DELIVERY_T;


typedef struct _SB_DVB_CP_CONN_T
{
    BOOL                            fg_connected;
    SB_DVB_CAM_DELIVERY_T*          pt_cur_cam_delivery;    /* CAM operator_tune_status */
    
    BRDCST_TYPE_T                   e_bcst_type;
    BRDCST_MEDIUM_T                 e_bcst_medium;
    UINT16                          ui2_satl_id;            /* valid when BRDCST_MEDIUM_DIG_SATELLITE == e_bcst_medium */
    UINT16                          ui2_satl_rec_id;        /* valid when BRDCST_MEDIUM_DIG_SATELLITE == e_bcst_medium*/
    
    SB_DVB_DELIVERY_T               t_delivery;
    INT32                           i4_cur_ssi;
    INT32                           i4_cur_sqi;
}   SB_DVB_CP_CONN_T;


typedef struct _SB_DVB_CP_TUNE_STATUS_T
{
    SB_DVB_CP_CONN_T                t_cp_conn;

    UINT8*                          pui1_delivery_desc;
    UINT16                          ui2_delivery_desc_len;
}   SB_DVB_CP_TUNE_STATUS_T;


#define         MAX_DVB_CP_CHAR_LEN            40
typedef struct _SB_DVB_CP_CONTENT_LABEL_T
{
    UINT8                           ui1_content_byte_min;
    UINT8                           ui1_content_byte_max;
    ISO_639_LANG_T                  t_lang_code;
    CHAR                            s_label_char[MAX_DVB_CP_CHAR_LEN];
}   SB_DVB_CP_CONTENT_LABEL_T;


typedef struct _SB_DVB_CP_OPEN_DATA_T
{
    SB_DVB_OPEN_DATA_T              t_dvb_open_data;
    SB_DVB_DELIVERY_TYPE            t_delivery_type;
} SB_DVB_CP_OPEN_DATA_T;


#endif /* _U_SB_DVB_CP_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_SVL_BLDR_SB_DVB_ENG */
/*----------------------------------------------------------------------------*/

