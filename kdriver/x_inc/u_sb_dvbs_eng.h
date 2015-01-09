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
/*! @file u_sb_dvbs_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         SVL Builder DVB-S engine.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_SB_DVBS_ENG_H_
#define _U_SB_DVBS_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_sb.h"
#include "u_tuner.h"
#include "u_satl.h"


/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVL_BLDR_SB_DVBS_ENG SVL Builder DVB-S engine
 *
 *  @ingroup groupMW_SVL_BLDR
 *  @brief The DVB-S engine module is used to scan all digital channel in DVB-S spec.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could create scan engine
 *   or start scan from the exported API of DVB-S svl builder.
 *
 *  @see groupMW_SVL_BLDR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* If define this macro, DVBS scan engine will print scan log directly */
//#define  MW_DVBS_SCAN_WITH_LOG


#define DVBS_ENG_NAME                                           "sb_dvbs"    /**< Define the DVB-S build engine name */
#define DVBS_DEFAULT_TUNER                                      "CT_DVBS"    /**< Define the default tuner in DVB-S build engine */
#define SB_DVBS_PROGRESS_START                                  0    /**< progress when start scan      */
#define SB_DVBS_PROGRESS_NIT_FOUND                              60    /**< progress number when NIT found      */
#define SB_DVBS_PROGRESS_FINISH                                 100    /**< progress when scan finnished       */


typedef UINT32 DVBS_OPERATOR_NAME_T;        /**<  operator name  */
#define DVBS_OPERATOR_NAME_OTHERS                               ((DVBS_OPERATOR_NAME_T) 0)
#define DVBS_OPERATOR_NAME_ASTRA_HD_PLUS                        ((DVBS_OPERATOR_NAME_T) 1)
#define DVBS_OPERATOR_NAME_SKY_DEUTSCHLAND                      ((DVBS_OPERATOR_NAME_T) 2)
#define DVBS_OPERATOR_NAME_AUSTRIASAT                           ((DVBS_OPERATOR_NAME_T) 3)
#define DVBS_OPERATOR_NAME_CANALDIGITAAL_HD                     ((DVBS_OPERATOR_NAME_T) 4)
#define DVBS_OPERATOR_NAME_CANALDIGITAAL_SD                     ((DVBS_OPERATOR_NAME_T) 5)
#define DVBS_OPERATOR_NAME_TV_VLAANDEREN_HD                     ((DVBS_OPERATOR_NAME_T) 6)
#define DVBS_OPERATOR_NAME_TV_VLAANDEREN_SD                     ((DVBS_OPERATOR_NAME_T) 7)
#define DVBS_OPERATOR_NAME_SYKLINK_CZ                           ((DVBS_OPERATOR_NAME_T) 8)
#define DVBS_OPERATOR_NAME_SYKLINK_SK                           ((DVBS_OPERATOR_NAME_T) 9)
#define DVBS_OPERATOR_NAME_ORS                                  ((DVBS_OPERATOR_NAME_T) 10)
#define DVBS_OPERATOR_NAME_TELESAT_BELGIUM                      ((DVBS_OPERATOR_NAME_T) 11)
#define DVBS_OPERATOR_NAME_TELESAT_LUXEMBOURG                   ((DVBS_OPERATOR_NAME_T) 12)
#define DVBS_OPERATOR_NAME_CANAL_DIGITAL                        ((DVBS_OPERATOR_NAME_T) 13)
#define DVBS_OPERATOR_NAME_NNK                                  ((DVBS_OPERATOR_NAME_T) 14)
#define DVBS_OPERATOR_NAME_DIGITURK_TURKSAT                     ((DVBS_OPERATOR_NAME_T) 15)
#define DVBS_OPERATOR_NAME_DIGITURK_EUTELSAT                    ((DVBS_OPERATOR_NAME_T) 16)
#define DVBS_OPERATOR_NAME_FRANSAT                              ((DVBS_OPERATOR_NAME_T) 17)
#define DVBS_OPERATOR_NAME_CYFRA_PLUS                           ((DVBS_OPERATOR_NAME_T) 18)
#define DVBS_OPERATOR_NAME_CYFROWY_POLSAT                       ((DVBS_OPERATOR_NAME_T) 19)
#define DVBS_OPERATOR_NAME_D_SMART                              ((DVBS_OPERATOR_NAME_T) 20)
#define DVBS_OPERATOR_NAME_NTV_PLUS                             ((DVBS_OPERATOR_NAME_T) 21)
#define DVBS_OPERATOR_NAME_ASTRA_INTERNATIONAL_LCN              ((DVBS_OPERATOR_NAME_T) 22)
#define DVBS_OPERATOR_NAME_SMART_HD_PLUS                        ((DVBS_OPERATOR_NAME_T) 23)
#define DVBS_OPERATOR_NAME_NC_PLUS                              ((DVBS_OPERATOR_NAME_T) 24)
#define DVBS_OPERATOR_NAME_TIVU_SAT                             ((DVBS_OPERATOR_NAME_T) 25)

typedef UINT16 SB_DVBS_NETWORK_SCAN_MASK_T;     /**<  network scan info arguments valid mask  */
#define SB_DVBS_NETWORK_SCAN_NIT_ACTUAL_VALID                   (MAKE_BIT_MASK_16(0))
#define SB_DVBS_NETWORK_SCAN_NW_ID_VALID                        (MAKE_BIT_MASK_16(1))
#define SB_DVBS_NETWORK_SCAN_TP_INFO_VALID                      (MAKE_BIT_MASK_16(2))
#define SB_DVBS_NETWORK_SCAN_SATL_REC_ID_VALID                  (MAKE_BIT_MASK_16(3))
#define SB_DVBS_SCAN_NUM_ASSOCIATED_SATL_REC_ID                 ((UINT16)3)
#define SB_DVBS_MAX_NUM_SAT                                     ((UINT16)4)
#define SB_DVBS_SPEC_MAX_NUM_SAT                                ((UINT16)8)


#define SB_MAX_BOUQUET_NAME_LENGTH                              ((UINT16)32)                              

/* Command to get the scan map and info */
/*------------------------------------------------------------------*/
/*! @enum DVBS_SCAN_MAPS_CMD_T
 *  @brief Command to get the dvb-s info.
 *  @code
 *  typedef enum {
 *      DVBS_SCAN_MAPS_CMD_GET_DEFAULT_TSL_REC_PARA = 0,
 *      DVBS_SCAN_MAPS_CMD_GET_DEFAULT_SATL_REC_PARA,
 *      DVBS_SCAN_MAPS_CMD_GET_NUM_DEFAULT_LNB_CONFIG,   
 *      DVBS_SCAN_MAPS_CMD_GET_DEFAULT_LNB_CONFIG,
 *      DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING,
 *      DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING,
 *      DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME,
 *      DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME,
 *      DVBS_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY,
 *      DVBS_SCAN_MAPS_CMD_IS_ASTRA_SCAN,
 *      DVBS_SCAN_MAPS_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID,
 *      DVBS_SCAN_MAPS_CMD_GET_CH_INDEX_START,
 *  }   DVBS_SCAN_MAPS_CMD_T;
 *  @endcode
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_DEFAULT_TSL_REC_PARA                 - Get default parameters of TSL record. note: will memset to 0 filrst, and fill transmission related paras.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_DEFAULT_SATL_REC_PARA                - Get default parameters of SATL record. note: will memset to 0 filrst, and fill transmission related paras.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_NUM_DEFAULT_LNB_CONFIG               - Get number of default LNB configurations.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_DEFAULT_LNB_CONFIG                   - Get default LNB configuration by index
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING                    - Check it support customized sorting or not.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING                   - Get the sorting rule with current country.
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME            - Check it support customized sorting by name or not.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME           - Get the rule of sorting by name with current country.
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY                       - Check it support LCN or not by current country.
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_ASTRA_SCAN                            - Check the satellite is adopted astra scan.
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_FREQ_RANGE_BY_TSL_REC_ID             - get upper/lower bound of the tsl record by record id
 *  @li@c  DVBS_SCAN_MAPS_CMD_GET_CH_INDEX_START                         - Get the started channel index for sorting
 *  @li@c  DVBS_SCAN_MAPS_CMD_IS_ACTIVE_SVL                                     - Indicate current SVL list is active or not
 */
/*------------------------------------------------------------------*/
typedef enum {
    DVBS_SCAN_MAPS_CMD_GET_DEFAULT_TSL_REC_PARA = 0,
    DVBS_SCAN_MAPS_CMD_GET_DEFAULT_SATL_REC_PARA,
    DVBS_SCAN_MAPS_CMD_GET_NUM_DEFAULT_LNB_CONFIG,   
    DVBS_SCAN_MAPS_CMD_GET_DEFAULT_LNB_CONFIG,
    DVBS_SCAN_MAPS_CMD_DETERMINE_PRIVATE_DATA_SPECIFIER,
    DVBS_SCAN_MAPS_CMD_IS_INSTALL_TEMP_NW_SVC_COUNTRY,
    DVBS_SCAN_MAPS_CMD_IS_SERVICE_REMOVAL_MUST_BE_CONFIRMED_COUNTRY,
    DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING,
    DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING,
    DVBS_SCAN_MAPS_CMD_IS_CUSTOMIZED_SORTING_BY_NAME,
    DVBS_SCAN_MAPS_CMD_GET_CUSTOMIZED_SORTING_BY_NAME,
    DVBS_SCAN_MAPS_CMD_IS_SORTING_BY_TUPLET,
    DVBS_SCAN_MAPS_CMD_GET_LCN_SORTING_BY_TUPLET,
    DVBS_SCAN_MAPS_CMD_SET_ORBIT_INFO,
    DVBS_SCAN_MAPS_CMD_IS_NON_LCN_COUNTRY,
    DVBS_SCAN_MAPS_CMD_GET_FREQUENCY_TOLERANCE,
    DVBS_SCAN_MAPS_CMD_SET_FREQUENCY_TOLERANCE,
    DVBS_SCAN_MAPS_CMD_IS_ASTRA_SCAN,
    DVBS_SCAN_MAPS_CMD_GET_FREQ_RANGE,
    DVBS_SCAN_MAPS_CMD_GET_DEFAULT_SCANMODE_TP_SAT_INFO,
    DVBS_SCAN_MAPS_CMD_IS_SGT_SEARCH,
    DVBS_SCAN_MAPS_CMD_IS_FST_FNT_SEARCH,
    DVBS_SCAN_MAPS_CMD_IS_SDT_OTHER_SEARCH,
    DVBS_SCAN_MAPS_CMD_GET_MAIN_SAT_IDX,
    DVBS_SCAN_MAPS_CMD_GET_CANDIDATE_HOME_TP,
    DVBS_SCAN_MAPS_CMD_IS_CANDIDATE_HOME_TP_PRIOR_USER_INPUT,
    DVBS_SCAN_MAPS_CMD_GET_CH_INDEX_START,
    DVBS_SCAN_MAPS_CMD_GET_PRESCAN_ENTRY_INFO,
    DVBS_SCAN_MAPS_CMD_IS_MDU_SEARCH,
    DVBS_SCAN_MAPS_CMD_IS_DIGITURK_SEARCH,
    DVBS_SCAN_MAPS_CMD_IS_CYFRAPLUS_SEARCH,
    DVBS_SCAN_MAPS_CMD_IS_POLSAT_SEARCH,
    DVBS_SCAN_MAPS_CMD_GET_MDU_DETECT_INFO,
    DVBS_SCAN_MAPS_CMD_GET_LO_FREQ_BY_MDU_TYPE,
    DVBS_SCAN_MAPS_CMD_IS_SEPARATE_BANK,
    DVBS_SCAN_MAPS_CMD_IS_MULTIBANK,    
    DVBS_SCAN_MAPS_CMD_IS_PAT_SEARCH,
    DVBS_SCAN_MAPS_CMD_SEPARATE_BANK_START_IDX,
    DVBS_SCAN_MAPS_CMD_COLLISION_AREA_INFO,
    DVBS_SCAN_MAPS_CMD_IS_ACTIVE_SVL,
    DVBS_SCAN_MAPS_CMD_IS_SORTING_ABSOLUTE_SORTING,
    DVBS_SCAN_MAPS_CMD_IS_SORTING_RELATIVE_SORTING,
    DVBS_SCAN_MAPS_CMD_GET_MDU_FREQ_RANGE_NUM,
    DVBS_SCAN_MAPS_CMD_GET_MDU_FREQ_RANGE_CONF,
    DVBS_SCAN_MAPS_CMD_IS_M7_OPERATOR,
    DVBS_SCAN_MAPS_CMD_GET_M7_SATELLITE_DETECT_INFO,
    DVBS_SCAN_MAPS_CMD_GET_M7_SATELLITE_DETECT_NUM,
    DVBS_SCAN_MAPS_CMD_GET_SPEC_DEFAULT_SCANMODE_TP_SAT_INFO,
    DVBS_SCAN_MAPS_CMD_NAME_SORTING_INIT,
    DVBS_SCAN_MAPS_CMD_SET_NAME_SORTING_PRINT_LEVEL,
    DVBS_SCAN_MAPS_CMD_SET_NAME_SORTING_DUMP_DATA,
} DVBS_SCAN_MAPS_CMD_T;
 
/* This data type contains different configuration flags for the 
   scan process. */
typedef UINT32      SB_DVBS_CONFIG_T;    /**< Define the different configuration flags for the DVB-S scan process*/

#define SB_DVBS_CONFIG_SUPPORT_MHEG5_SERVICES                   (MAKE_BIT_MASK_32(0))    
/**<  Install the MHEG-5 services as visible ones */
#define SB_DVBS_CONFIG_KEEP_DUPLICATE_CHANNELS                  (MAKE_BIT_MASK_32(1))    
/**< Do not remove duplicate channels */
#define SB_DVBS_CONFIG_NOT_SUPPORT_HDTV                         (MAKE_BIT_MASK_32(2)) 
/**< Do not support HDTV (H.264 & MPEG-2) */
#define SB_DVBS_CONFIG_INSTALL_FREE_SVC_ONLY                    (MAKE_BIT_MASK_32(3))
/*To install free service only*/
#define SB_DVBS_CONFIG_INSTALL_TV_SVC_ONLY                      (MAKE_BIT_MASK_32(4))
/*To install TV service only*/
#define SB_DVBS_CONFIG_INSTALL_RADIO_SVC_ONLY                   (MAKE_BIT_MASK_32(5))
/*To install Radio service only */
#define SB_DVBS_CONFIG_INSTALL_TV_RADIO_SVC_ONLY                (MAKE_BIT_MASK_32(6))
/*To install TV and Radio service only */
#define SB_DVBS_CONFIG_TV_RADIO_SEPARATE                        (MAKE_BIT_MASK_32(7)) 
/**< To support TV type and Raido type service in the its channel list */
#define SB_DVBS_CONFIG_COMP_OTHER_TS                            (MAKE_BIT_MASK_32(8))
#define SB_DVBS_CONFIG_RESERVE_CH_NUM_BEFORE_NON_LCN_CH         (MAKE_BIT_MASK_32(9))
#define SB_DVBS_CONFIG_SIMPLE_SORT_FOR_NON_LCN_CH               (MAKE_BIT_MASK_32(10))
#define SB_DVBS_CONFIG_SORTING_BY_FREQ                          (MAKE_BIT_MASK_32(11))
#define SB_DVBS_CONFIG_SUPPORT_MHP_SERVICES                     (MAKE_BIT_MASK_32(12))
#define SB_DVBS_CONFIG_NOT_INSTALL_DATA_SERVICE                 (MAKE_BIT_MASK_32(13))
/**< If the onid, tsid and serviceID is the same, But the frequency is not same do not filter this service.*/
#define SB_DVBS_CONFIG_UNCHECK_SVC_LOC_WITH_FREQ                (MAKE_BIT_MASK_32(14))
#define SB_DVBS_CONFIG_CUSTOMIZED_PRESET_CH_NUM                 (MAKE_BIT_MASK_32(15))
/* This is the BGM scan config for standby update sattilite scan.*/
#define SB_DVBS_CONFIG_BGM_SCAN                                 (MAKE_BIT_MASK_32(16))


/* Max satellite count */
#define SB_DVBS_SATELLITE_COUNT_MAX                              4


/* Macros for DVB-S channel number */
/* These macros are designed to conform to the ATSC ones so that using the ATSC macros can
   also retrieve the channel numbers */
#define SB_DVBS_CH_TYPE_RANGE                                   3         /*The last 3bit is used for service type*/ 
#define SB_DVBS_MAX_CHANNEL_INDEX_WITH_CH_TYPE                  ((UINT8) 0x7)
#define SB_DVBS_CH_NUM_FLAG                                     ((UINT32) 0x80)    /**< Define the flag for used DVB channel number. */
#define SB_DVBS_MAX_CHANNEL_INDEX                               ((UINT8) 0x3f)    /**<  Define the max DVB channel index. */
#define SB_DVBS_GEN_CHANNEL_ID(major, index) \
    ((((UINT32) (major) & 0x3fff) << 18) | \
     ((index & SB_DVBS_MAX_CHANNEL_INDEX)) | \
     SB_DVBS_CH_NUM_FLAG)    /**< Define the generated DVB channel ID without LCN.*/
#if 1
#define SB_DVBS_GEN_CHANNEL_ID_WITH_LCN(major, lcn, index) \
    ((((UINT32) (major) & 0x3fff) << 18) | \
     (((UINT32) (lcn) & 0x3ff)  <<  8) | \
     ((index & SB_DVBS_MAX_CHANNEL_INDEX)) | \
     SB_DVBS_CH_NUM_FLAG)    /**<  Define the generated DVB channel ID with LCN.*/
#define SB_DVBS_GET_LCN(channel_id) \
    ((UINT16)(((UINT32)(channel_id) >> 8) & 0x3ff))    /**<    Define the process of getting LCN by channel ID.*/

#endif

#define SB_DVBS_GET_CHANNEL_NUM(channel_id) \
    ((UINT16)(((UINT32)(channel_id) >> 18) & 0x3fff))    /**<  Define the process of getting channel number by channel ID.*/
#define SB_DVBS_GET_CHANNEL_INDEX(channel_id) \
    ((UINT8) (((UINT32)(channel_id)) & SB_DVBS_MAX_CHANNEL_INDEX))    /**<  Define the process of getting channel index by channel ID.*/

/*------------------------------------------------------------------*/
/*! @brief The callback function for getting customized information from DVB-S scan map.
 *  @param [in] t_cmd   The command to get the scan map and info.
 *  @param [in] t_country_code  The current country information.
 *  @param [in] pv_input    The input parameter for getting customized info.
 *  @param [out] pv_output  The buffer for output value.
 *  @param [out] pz_output_len  The length for output buffer.
 *  @return
 *  @retval   - SBR_INV_ARG  The current input value is invalid.
 *  @retval   - SBR_OUT_OF_RANGE The input value is out of range.
 *  @retval   - SBR_NOT_ENOUGH_SPACE  The output length is out of space.
 *  @retval   - SBR_NOT_FOUND  It can not found in this prcess.
 *  @retval   - SBR_OK  Routine successful.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sb_dvbs_scan_maps_fct) (DVBS_SCAN_MAPS_CMD_T     t_cmd,
                                         ISO_3166_COUNT_T    t_country_code,
                                         DVBS_OPERATOR_NAME_T    t_operator_name,
                                         VOID*               pv_input,
                                         VOID*               pv_output,
                                         SIZE_T*             pz_output_len);

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_ON_TS_SVC_ID_T
 *  @brief The locator of service in DVB system.
 *  @code
 *  typedef struct _SB_DVBS_ON_TS_SVC_ID_T
 *  {
 *      UINT16                  ui2_on_id;
 *      UINT16                  ui2_ts_id;
 *      UINT16                  ui2_svc_id;
 *  } SB_DVBS_ON_TS_SVC_ID_T;
 *  @endcode
 *  @li@c  ui2_on_id                                         - The original network id.
 *  @li@c  ui2_ts_id                                         - The transport stream id.
 *  @li@c  ui2_svc_id                                        - The service identify.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_ON_TS_SVC_ID
{
    UINT16                  ui2_on_id;
    UINT16                  ui2_ts_id;
    UINT16                  ui2_svc_id;
} SB_DVBS_ON_TS_SVC_ID_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_ON_PRESCAN_TP_INFO_T
 *  @brief The locator of service in DVB system.
 *  @code
 *  typedef struct _SB_DVBS_ON_PRESCAN_TP_INFO_T
 *  {
 *      TUNER_MDU_TYPE_T            e_mdu_type;
 *      TUNER_POLARIZATION_T     e_pol;
 *      UINT32                               ui4_freq;
 *      UINT32                               ui4_sym_rate;
 *      UINT16                               ui2_mdu_lo_freq;
 *  } SB_DVBS_ON_PRESCAN_TP_INFO_T;
 *  @endcode
 *  @li@c  e_mdu_type                                        - The MDU type, this field will be missed if not MDU.
 *  @li@c  e_pol                                                  - The  polarization
 *  @li@c  ui4_freq                                              - The frequency. 
 *  @li@c  ui2_mdu_lo_freq                                  - The MDU lo frequency.
 */
/*------------------------------------------------------------------*/

typedef struct _SB_DVBS_ON_PRESCAN_TP_INFO_T
{
   TUNER_MDU_TYPE_T       e_mdu_type;
   TUNER_POLARIZATION_T   e_pol;
   UINT32                 ui4_freq; 
   UINT32                 ui4_sym_rate;   
   UINT16                 ui2_mdu_lo_freq;
} SB_DVBS_ON_PRESCAN_TP_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_ON_PRESCAN_KEY_INFO_T
 *  @brief The locator of service in DVB system.
 *  @code
 *  typedef struct _SB_DVBS_ON_PRESCAN_KEY_INFO_T
 *  {
 *      UINT16                  ui2_nw_id;
 *      UINT16                  ui2_ts_id;
 *      UINT16                  ui2_bq_id;
 *      BOOL                    b_bqid_check;
 *      BOOL                    b_tsid_check;
 *      BOOL                    b_nwid_check;
 *  } SB_DVBS_ON_PRESCAN_KEY_INFO_T;
 *  @endcode
 *  @li@c  ui2_nw_id                                       - The  network id ,from NIT .
 *  @li@c  ui2_ts_id                                         - The transport stream id, from SDT.
 *  @li@c  ui2_bq_id                                        - The bouqet id, from BAT.
 *  @li@c  b_bqid_check                                  -  The bqid check status,the value is TRUE when no its checking.
 *  @li@c  b_tsid_check                                   -  The tsid check status,the value is TRUE when no its checking.
 *  @li@c  b_nwid_check                                  -  The nwid check status,the value is TRUE when no its checking.
 */
/*------------------------------------------------------------------*/

typedef struct _SB_DVBS_ON_PRESCAN_KEY_INFO_T
{
    UINT16                  ui2_nw_id;
    UINT16                  ui2_ts_id;
    UINT16                  ui2_bq_id;   
    BOOL                    b_bqid_checked;
    BOOL                    b_tsid_checked;
    BOOL                    b_nwid_checked;
} SB_DVBS_ON_PRESCAN_KEY_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_ON_PRESCAN_INFO_T
 *  @brief The locator of service in DVB system.
 *  @code
 *  typedef struct _SB_DVBS_ON_PRESCAN_INFO_T
 *  {
 *      CHAR                  *ps_satellite_name;
 *      INT16                  i2_orbit;
 *      SB_DVBS_ON_PRESCAN_TP_INFO_T                  t_tp_info;
 *      SB_DVBS_ON_PRESCAN_KEY_INFO_T                t_key_info;
 *  } SB_DVBS_ON_PRESCAN_INFO_T;
 *  @endcode
 *  @li@c  ps_satellite_name                            - The satellite name will return..
 *  @li@c  i2_orbit                                           - Orbit of satellite current prescan
 *  @li@c  t_tp_info                                         - Connect tp info.
 *  @li@c  t_key_info                                      -  Check key info.
 */
/*------------------------------------------------------------------*/

typedef struct _SB_DVBS_ON_PRESCAN_INFO_T
{
    CHAR                            *ps_satellite_name;
    INT16                           i2_orbit;
    SB_DVBS_ON_PRESCAN_TP_INFO_T    t_tp_info;
    SB_DVBS_ON_PRESCAN_KEY_INFO_T   t_key_info;
} SB_DVBS_ON_PRESCAN_INFO_T;


/*! @struct SB_DVBS_ON_ORBIT_T
 *  @brief The locator of service in DVB system.
 *  @code
 *  typedef struct _SB_DVBS_ON_ORBIT_T
 *  {
 *      INT16                  aui_scanned_orbit;
 *      SIZE_T                 z_len_orbit;
 *  } SB_DVBS_ON_ORBIT_T;
 *  @endcode
 *  @li@c  aui_scanned_orbit                                         - The array of scanned satellite info.
 *  @li@c  z_len_orbit                                                   - The satellite count.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_ON_ORBIT_T
{
    INT16                   aui_scanned_orbit[SB_DVBS_SATELLITE_COUNT_MAX];
    SIZE_T                  z_len_orbit;
} SB_DVBS_ON_ORBIT_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SORTING_BY_TUPLET_T
 *  @brief The locator of service in DVB system.
 *  @code
 *  typedef struct _SB_DVBS_SORTING_BY_TUPLET_TT
 *  {
 *      INT16                                        i2_current_orbit; 
 *      CHAR                                        ac_name
 *      SB_DVBS_ON_TS_SVC_ID_T      t_on_ts_svc_id;
 *  } SB_DVBS_SORTING_BY_TUPLET_T;
 *  @endcode
 *  @li@c  i2_current_orbit                                - The current satellite orbit.
 *  @li@c  ac_name                                          - The name of channel.
 *  @li@c  t_on_ts_svc_id                                  - The original network id,stream id ,svc id.
 *  @li@c  b_scramble                                       - The flag of service is scrambled or not.
 *  @li@c  e_serv_type                                      - The service type, details see struct SVL_SERVICE_TYPE_T.
 *  @li@c  ui1_type_value                                  - The value of service type parsed from bit-stream.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SORTING_BY_TUPLET_T
{
    INT16                   i2_current_orbit;    
    CHAR                    ac_name[MAX_PROG_NAME_LEN+1];
    SB_DVBS_ON_TS_SVC_ID_T  t_on_ts_svc_id;
    BOOL                    b_scramble;
    SVL_SERVICE_TYPE_T      e_serv_type;
    UINT8                   ui1_type_value;
    
} SB_DVBS_SORTING_BY_TUPLET_T;

/*------------------------------------------------------------------*/
/*! @struct _SB_DVBS_SORTING_OUTPUT_T
 *  @brief Sorting by tuplet, return the sorting information.
 *  @code
 *  typedef struct _SB_DVBS_SORTING_OUTPUT_T
 *  {
 *      UINT16                   ui2_lcn;  
 *      UINT16                   ui2_bank_idx;
 *  } SB_DVBS_SORTING_OUTPUT_T;
 *  @endcode
 *  @li@c  ui2_lcn                                             - The current sorting position.
 *  @li@c  ui2_bank_idx                                    - The bank index.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SORTING_OUTPUT_T
{
    UINT16                   ui2_lcn;  
    UINT16                   ui2_bank_idx;
    
} SB_DVBS_SORTING_OUTPUT_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_INIT_DATA_T
 *  @brief It is used as the argument of the API x_sb_dvbs_eng_init() when initializing an SVL Builder with DVB-S engine.
 *  @code
 *  typedef struct _SB_DVBS_INIT_DATA_T
 *  {
 *      SB_DVBS_CONFIG_T         t_eng_cfg_flag;
 *      x_sb_dvbs_scan_maps_fct  pf_scan_maps;
 *  } SB_DVBS_INIT_DATA_T;
 *  @endcode
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag.
 *  @li@c  pf_scan_maps                                      - Contains the scan map callback function.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_INIT_DATA_T
{
    SB_DVBS_CONFIG_T         t_eng_cfg_flag;
    x_sb_dvbs_scan_maps_fct  pf_scan_maps;
    UINT16                   ui2_max_num_svc;
} SB_DVBS_INIT_DATA_T;



/*! @struct SB_DVBS_OPEN_DATA_T
 *  @brief It is used as the argument pv_open_data of the API c_sb_open_builder() when opening an SVL Builder with DVB-S engine
 *  @code
 *  typedef struct _SB_DVBS_OPEN_DATA_T
 *  {
 *      UINT16                  ui2_svl_id;
 *      CHAR*                   ps_svl_name;
 *      CHAR*                   ps_file_name;
 *      CHAR*                   ps_tuner_name;    
 *      BOOL                    b_use_orig;
 *  } SB_DVBS_OPEN_DATA_T;
 *  @endcode
 *  @li@c  ui2_svl_id                                        - The id of SVL to be loaded.
 *  @li@c  ps_svl_name                                       - The name of SVL to be loaded.
 *  @li@c  ps_file_name                                      - The file name from which the SVL/TSL will be loaded.
 *  @li@c  ps_tuner_name                                     - The name of tuner to be connectted.
 *  @li@c  b_use_orig                                        - A flag. When set to true, use the existed SVL, instead of reloading it
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_OPEN_DATA_T
{
    UINT16                  ui2_svl_id;
    CHAR*                   ps_svl_name;
    CHAR*                   ps_file_name;
    CHAR*                   ps_tuner_name;    
    BOOL                    b_use_orig;
} SB_DVBS_OPEN_DATA_T;


/*------------------------------------------------------------------*/
/*! @enum SB_DVBS_SCAN_TYPE_T
 *  @brief It contains different scan types that are used to initialize scan parameters.
 *  @code
 *  typedef enum
 *  {
 *      SB_DVBS_SCAN_TYPE_UNKNOWN = 0,
 *      SB_DVBS_SCAN_TYPE_AUTO_MODE,
 *      SB_DVBS_SCAN_TYPE_PRESET_MODE,
 *      SB_DVBS_SCAN_TYPE_SINGLE_TP_SCAN,
 *      SB_DVBS_SCAN_TYPE_UPDATE_SCAN,
 *      SB_DVBS_SCAN_TYPE_FREQ_MANUAL_SCAN,
 *      SB_DVBS_SCAN_TYPE_SAT_CHECK,
 *      SB_DVBS_SCAN_TYPE_SAT_SEARCH,
 *      SB_DVBS_SCAN_TYPE_NUM
 *  } SB_DVBS_SCAN_TYPE_T;
 *  @endcode
 *  @li@c  SB_DVBS_SCAN_TYPE_UNKNOWN                          - Unknown.
 *  @li@c  SB_DVBS_SCAN_TYPE_AUTO_MODE                        - Blind Scan.
 *  @li@c  SB_DVBS_SCAN_TYPE_PRESET_MODE                      - Scan with preset transponders.
 *  @li@c  SB_DVBS_SCAN_TYPE_SINGLE_TP_SCAN                   - Scan single transponder.
 *  @li@c  SB_DVBS_SCAN_TYPE_UPDATE_SCAN                      - BGM scan
 *  @li@c  SB_DVBS_SCAN_TYPE_FREQ_MANUAL_SCAN             - frequency scan
 *  @li@c  SB_DVBS_SCAN_TYPE_SAT_CHECK                          - search satellite and get the orbit with single tp.
 *  @li@c  SB_DVBS_SCAN_TYPE_SAT_SEARCH                        - Search satellite signal for LNB
 *  @li@c  SB_DVBS_SCAN_TYPE_COMPLETE                            - Complete scan: network scan (operator scan) and auto scan
 *  @li@c  SB_DVBS_SCAN_TYPE_NETWORK_SCAN                     - search TPs listed in NIT or search a given TP and get channels info from FST,FNT
 *  @li@c  SB_DVBS_SCAN_TYPE_NUM                              - Number of scan types. 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_DVBS_SCAN_TYPE_UNKNOWN = 0,
    SB_DVBS_SCAN_TYPE_AUTO_MODE,
    SB_DVBS_SCAN_TYPE_PRESET_MODE,
    SB_DVBS_SCAN_TYPE_NETWORK_SCAN,
    SB_DVBS_SCAN_TYPE_SINGLE_TP_SCAN,
    SB_DVBS_SCAN_TYPE_UPDATE_SCAN,
    SB_DVBS_SCAN_TYPE_FREQ_MANUAL_SCAN,
    SB_DVBS_SCAN_TYPE_SAT_CHECK,
    SB_DVBS_SCAN_TYPE_SAT_SEARCH,
    SB_DVBS_SCAN_TYPE_COMPLETE,
    SB_DVBS_SCAN_TYPE_NUM
} SB_DVBS_SCAN_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum SB_DVBS_SAT_SEARCH_RANGE_T
 *  @brief 
 *  @code
 *  typedef enum
 *  {
 *  } SB_DVBS_SAT_SEARCH_RANGE_T;
 *  @endcode
 *  @li@c  SB_DVBS_SAT_SEARCH_RANGE_ALL                      - search all range
 *  @li@c  SB_DVBS_SAT_SEARCH_RANGE_LOW                     - search low band only
 *  @li@c  SB_DVBS_SAT_SEARCH_RANGE_HIGH                     - search high band only 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_DVBS_SAT_SEARCH_RANGE_ALL = 0,
    SB_DVBS_SAT_SEARCH_RANGE_LOW,
    SB_DVBS_SAT_SEARCH_RANGE_HIGH
} SB_DVBS_SAT_SEARCH_RANGE_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_PRESET_SCAN_INFO_T
 *  @brief the required information of DVB-S preset scan
 *  @code
 *  typedef struct _SB_DVBS_PRESET_SCAN_INFO_T{
 *        BOOL    b_network_search;
 *        UINT16  ui2_nw_id;
 *  }   SB_DVBS_PRESET_SCAN_INFO_T;
 *  @endcode
 *  @li@c  b_network_search                               - network search on/off
 *  @li@c  ui2_nw_id                                      - network id, if ui2_nw_id ==0, engine would load NIT actual
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_PRESET_SCAN_INFO_T{
    BOOL    b_network_search;
    UINT16  ui2_nw_id;
}   SB_DVBS_PRESET_SCAN_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_DLVR_INFO_T
 *  @brief The structure SB_DVBS_DLVR_INFO_T is the basic information about
 *   front end transmission
 *  @code
 *  typedef struct _SB_DVBS_DLVR_INFO_T{
 *      UINT32                  ui4_freq;
 *      UINT8                   e_pol;
 *      UINT32                  ui4_sym_rate;
 *  }SB_DVBC_DLVR_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                          - frequency
 *  @li@c  e_pol                                             - polarization
 *  @li@c  ui4_sym_rate                                      - symbol rate
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_DLVR_INFO_T{
    UINT32                  ui4_freq;
    TUNER_POLARIZATION_T    e_pol;
    UINT32                  ui4_sym_rate;
}SB_DVBS_DLVR_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SINGLE_TP_SCAN_INFO_T
 *  @brief the required information of DVB-S preset scan
 *  @code
 *  typedef struct _SB_DVBS_SINGLE_TP_SCAN_INFO_T{
 *        BOOL    b_network_search;
 *        UINT16  ui2_nw_id;
 *        UINT16  ui2_tsl_rec_id;
 *  }   SB_DVBS_SINGLE_TP_SCAN_INFO_T;
 *  @endcode
 *  @li@c  b_network_search                               - network search on/off
 *  @li@c  ui2_nw_id                                      - network id, if ui2_nw_id ==0, engine would load NIT actual
 *  @li@c  ui2_tsl_rec_id                                 - record id of the target transponder. the tsl id should be the same as satl id.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SINGLE_TP_SCAN_INFO_T{
    BOOL    b_network_search;
    UINT16  ui2_nw_id;  
    UINT16  ui2_tsl_rec_id;
    SB_DVBS_DLVR_INFO_T  t_tp_info;
}   SB_DVBS_SINGLE_TP_SCAN_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SAT_SEARCH_INFO_T
 *  @brief the required information of DVB-S satellite search
 *  @code
 *  typedef struct _SB_DVBS_SAT_SEARCH_INFO_T{
 *        SB_DVBS_SAT_SEARCH_RANGE_T   e_sat_search_range
 *  }   SB_DVBS_SAT_SEARCH_INFO_T;
 *  @endcode
 *  @li@c  e_sat_search_range                               - search range: all, low, high
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SAT_SEARCH_INFO_T{
    SB_DVBS_SAT_SEARCH_RANGE_T    e_sat_search_range;
}   SB_DVBS_SAT_SEARCH_INFO_T;

typedef struct _SB_DVBS_DES_DLVR_INFO_T{
    UINT32                  ui4_freq;
    TUNER_POLARIZATION_T    e_pol;
    UINT32                  ui4_sym_rate;
    INT16                   i2_orbit;
}SB_DVBS_DLVR_DES_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_FREQ_SCAN_INFO_T
 *  @brief the required information of DVB-S frequency manual scan
 *  @code
 *  typedef struct _SB_DVBS_FREQ_SCAN_INFO_T{
 *       UINT32                   ui4_freq;
 *       UINT32                   ui4_sym_rate;
 *       TUNER_POLARIZATION_T     e_pol;     
 *  }   SB_DVBS_FREQ_SCAN_INFO_T;
 *  @endcode
 *  @li@c  
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_FREQ_SCAN_INFO_T{
    UINT32                   ui4_freq;
    UINT32                   ui4_sym_rate;
    TUNER_POLARIZATION_T     e_pol;
    BOOL                     b_network_search;
    UINT16                   ui2_nw_id;    
}   SB_DVBS_FREQ_SCAN_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_NETWORK_SCAN_INFO_T
 *  @brief the required information of DVB-S fast scan
 *  @code
 *  typedef struct _SB_DVBS_NETWORK_SCAN_INFO_T{
 *        SB_DVBS_NETWORK_SCAN_MASK_T       t_valid_mask;
 *        BOOL                              b_nit_actual;
 *        UINT16                            ui2_nw_id;
 *        SB_DVBS_DLVR_INFO_T               t_tp_info;
 *        UINT16                            aui2_associated_satl_rec_id[SB_DVBS_SCAN_NUM_ASSOCIATED_SATL_REC_ID];
 *        UINT16                            ui2_bat_id;
 *  }   SB_DVBS_NETWORK_SCAN_INFO_T;
 *  @endcode
 *  @li@c  t_valid_mask                             - t_valid_mask indict below arguments value valid or not
 *  @li@c  b_nit_actual                               - is search in NIT actual
 *  @li@c  ui2_nw_id                                  - network id
 *  @li@c  t_tp_info                                    - tp info for scan
 *  @li@c  aui2_associated_satl_rec_id[]      - associated satllite rec id if doing M7 network scan which allows scan one TP only to get multiple satllites broadcasted channels info.
 *  @li@c  ui2_bat_id                                  - bouquet id
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_NETWORK_SCAN_INFO_T
{
    SB_DVBS_NETWORK_SCAN_MASK_T     t_valid_mask;
    BOOL                            b_nit_actual;
    UINT16                          ui2_nw_id;
    SB_DVBS_DLVR_INFO_T             t_tp_info;
    UINT16                          ui2_associated_satl_rec_num;
    UINT16                          aui2_associated_satl_rec_id[SB_DVBS_SCAN_NUM_ASSOCIATED_SATL_REC_ID];
    UINT16                          ui2_bat_id;
} SB_DVBS_NETWORK_SCAN_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SCAN_INFO_T
 *  @brief DVB-S scan info
 *  @code
 *  typedef struct _SB_DVBS_SCAN_INFO_T{
 *      UINT16                  ui2_satl_id;
 *      UINT16                  ui2_satl_rec_id;
 *      union{
 *          SB_DVBS_PRESET_SCAN_INFO         t_preset_scan_info;
 *          SB_DVBS_SINGLE_TP_SCAN_INFO      t_single_tp_scan_info;  
 *          SB_DVBS_SINGLE_TP_SCAN_INFO_T      t_sat_check_info;  
 *      }uinfo;
 *  }   SB_DVBS_SCAN_INFO_T;
 *  @endcode
 *  @li@c  ui2_satl_id                                    - SATL ID
 *  @li@c  ui2_satl_rec_id                                - the record id of selected satellite
 *  @li@c  t_preset_scan_info                             - the required information of preset scan
 *  @li@c  t_single_tp_scan_info                          - the required information of single TP scan        
 *  @li@c  t_sat_check_info                                - the required information of satellite checkscan        

 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_INFO_T{
    UINT16                  ui2_satl_id;
    UINT16                  ui2_satl_rec_id;
    union{
        SB_DVBS_PRESET_SCAN_INFO_T         t_preset_scan_info;
        SB_DVBS_SINGLE_TP_SCAN_INFO_T      t_single_tp_scan_info;  
        SB_DVBS_SINGLE_TP_SCAN_INFO_T      t_sat_check_info;  
        SB_DVBS_SAT_SEARCH_INFO_T          t_sat_search_info;
        SB_DVBS_FREQ_SCAN_INFO_T           t_freq_scan_info;
        SB_DVBS_NETWORK_SCAN_INFO_T        t_network_scan_info;
    }uinfo;
}   SB_DVBS_SCAN_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SVC_UPDATE_NFY_DATA
 *  @brief It is used as the argument of the service update notification function.
 *  @code
 *  typedef struct _SB_DVBS_SVC_UPDATE_NFY_DATA
 *  {
 *      UINT16                  ui2_num_tv_added_svc;
 *      UINT16                  ui2_num_tv_deleted_svc;
 *      UINT16                  ui2_num_radio_added_svc;
 *      UINT16                  ui2_num_radio_deleted_svc;
 *      UINT16                  ui2_num_app_added_svc;
 *      UINT16                  ui2_num_app_deleted_svc;
 *  } SB_DVBS_SVC_UPDATE_NFY_DATA;
 *  @endcode
 *  @li@c  ui2_num_added_svc                                 - The count of added service in this progress.
 *  @li@c  ui2_num_deleted_svc                               - The count of deleted service in this progress.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SVC_UPDATE_NFY_DATA
{
    UINT16                  ui2_num_tv_added_svc;
    UINT16                  ui2_num_tv_deleted_svc;
    UINT16                  ui2_num_radio_added_svc;
    UINT16                  ui2_num_radio_deleted_svc;
    UINT16                  ui2_num_app_added_svc;
    UINT16                  ui2_num_app_deleted_svc;
} SB_DVBS_SVC_UPDATE_NFY_DATA;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_AUTO_FREQ_RANGE
 *  @brief It is used as the argument of get the dvbs middle frequency range.
 *  @code
 *  typedef struct _SB_DVBS_AUTO_FREQ_RANGE
 *  {
 *      UINT16                  ui2_start_freq;
 *      UINT16                  ui2_end_freq;
 *  } SB_DVBS_AUTO_FREQ_RANGE;
 *  @endcode
 *  @li@c  ui2_start_freq                             - The start frequncy of the range in MHZ.
 *  @li@c  ui2_end_freq                               - The end frequncy of the range in MHZ.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_AUTO_FREQ_RANGE
{
    UINT16                  ui2_start_freq; /*MHZ*/
    UINT16                  ui2_end_freq;/*MHZ*/
} SB_DVBS_AUTO_FREQ_RANGE;


/*------------------------------------------------------------------*/
/*! @brief  Structure of tuner TUNER information for DVB-S
 *  @code
 *  typedef struct _SB_DVBS_TUNER_DATA_T
 *  {
 *      TUNER_LNB_TYPE_T    e_lnb_type; 
 *      UINT16              ui2_lnb_low_freq; 
 *      UINT16              ui2_lnb_high_freq; 
 *      UINT16              ui2_lnb_switch_freq; 
 *      UINT8               ui1_22k;
 *      UINT8               ui1_polarity_13v_18v;  
 *  } SB_DVBS_TUNER_DATA_T;
 *  @endcode
 *  @li@c  e_lnb_type                    - LNB type
 *  @li@c  ui2_lnb_low_freq          - LNB low frequency in MHz
 *  @li@c  ui2_lnb_high_freq         - LNB high frequency in MHz
 *  @li@c  ui2_lnb_switch_freq       - LNB switch frequency in MHz
 *  @li@c  ui1_22k                   - 22k on or off
 *  @li@c  ui1_polarity_13v_18v      - 13V or 18V
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_TUNER_DATA_T
{
    TUNER_LNB_TYPE_T    e_lnb_type; /*LNB_UNKNOWN, LNB_SINGLE_FREQ, LNB_DUAL_FREQ*/
    UINT16              ui2_lnb_low_freq; /*in MHz*/
    UINT16              ui2_lnb_high_freq; /*in MHz*/
    UINT16              ui2_lnb_switch_freq; /*in MHz, for dual freq*/
    UINT8               ui1_22k;/* 0-> 22k off,1->22k on */
    UINT8               ui1_polarity_13v_18v;   /* 0->V(13v) ,1->H(18v)*/
} SB_DVBS_TUNER_DATA_T;

/*------------------------------------------------------------------*/
/*! @brief The prototype of the service update notification function
 *  @param [in] pt_nfy_data The information for added and deleted services.
 *  @param [in] pv_nfy_tag The tag for notifying user.
 *  @param [out] None.
 *  @return
 *  @retval      - None.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_dvbs_svc_update_nfy_fct) (
        SB_DVBS_SVC_UPDATE_NFY_DATA* pt_nfy_data,
        VOID*                       pv_nfy_tag);


/*------------------------------------------------------------------*/
/*! @brief It is used for notifying user the updating network.
 *  @param [in] pv_nfy_tag The tag for notifying user.
 *  @param [out] None.
 *  @return
 *  @retval      - None.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_dvbs_nw_change_nfy_fct) (
        VOID*                       pv_nfy_tag);


typedef struct _SB_SAT_NAME_NFY_DATA
{
    UINT16  ui2_satl_rec_id;
    INT16   i2_orb_pos; 
    CHAR    ac_sat_name[MAX_SAT_NAME_LEN];
    TUNER_MDU_TYPE_T    e_mdu_type;
}SB_SAT_NAME_NFY_DATA;


/* The prototype of the sat name notification function */
/*------------------------------------------------------------------*/
/*! @brief The prototype of the new service notification function
 *  @param [in] The notified data SB_SAT_NAME_NFY_DATA
 *  @param [in] the tag value passed in to scan engine
 *  @return      VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_sat_name_nfy_fct)(
    SB_SAT_NAME_NFY_DATA*   pt_nfy_data,
    VOID*                   pv_nfy_tag
    );




typedef struct _SB_ONE_BOUQUET_DATA
{
    UINT16                          ui2_bat_id;
    CHAR                            sc_bat_name[SB_MAX_BOUQUET_NAME_LENGTH];

}SB_ONE_BOUQUET_DATA;

typedef struct _SB_BOUQUET_INFO_NFY_DATA
{
    UINT16                          ui2_bat_num;
    SB_ONE_BOUQUET_DATA*            pt_bat_info_list;

}SB_BOUQUET_INFO_NFY_DATA;


/* The prototype of the bouquet name notification function */
/*------------------------------------------------------------------*/
/*! @brief The prototype of the bouquet name notification function
 *  @param [in] The notified data SB_BOUQUET_INFO_NFY_DATA
 *  @param [in] the tag value passed in to scan engine
 *  @return      VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_bouquet_info_nfy_fct)(
    SB_BOUQUET_INFO_NFY_DATA*   pt_nfy_data,
    VOID*                       pv_nfy_tag);



typedef struct _SB_MDU_DETECT_INFO_NFY_DATA
{
    TUNER_MDU_TYPE_T      e_mdu_type;

}SB_MDU_DETECT_INFO_NFY_DATA;

/* The prototype of the mdu detect  notification function */
/*------------------------------------------------------------------*/
/*! @brief The prototype of the mdu detect  notification function
 *  @param [in] The notified data SB_MDU_DETECT_INFO_NFY_DATA
 *  @param [in] the tag value passed in to scan engine
 *  @return      VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_mdu_detect_nfy_fct)(
    SB_MDU_DETECT_INFO_NFY_DATA*    pt_nfy_data,
    VOID*                           pv_nfy_tag);

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SCAN_DATA_T
 *  @brief It is used as the argument pv_scan_data of the API x_sb_start_scan() when scaning a media.
 *  @code
 *  typedef struct _SB_DVBS_SCAN_DATA_T
 *  {
 *      ISO_3166_COUNT_T                t_country_code;
 *      SB_DVBS_SCAN_TYPE_T             e_scan_type;
 *      SB_DVBS_SCAN_INFO_T             t_scan_info;
 *      SB_DVBS_CONFIG_T                t_eng_cfg_flag;
 *      x_sb_dvbs_svc_update_nfy_fct    pf_svc_update_nfy;
 *      VOID*                           pv_svc_update_nfy_tag;
 *      x_sb_new_svc_nfy_fct        pf_new_svc_nfy;
 *      VOID*                           pv_new_svc_nfy_tag;
 *      x_sb_sat_name_nfy_fct           pf_sat_name_nfy;
 *      VOID*                           pv_sat_name_nfy_tag;
 *      x_sb_bat_info_nfy_fct           pf_bat_info_nfy;
 *      VOID*                           pv_bat_info_nfy_tag;
 *      x_sb_mdu_detect_nfy_fct         pf_mdu_detect_nfy;
 *      VOID*                           pv_mdu_detect_nfy_tag;
 *      BOOL                            b_is_bgm;
 *      BOOL                            b_get_info_step;
 *      BOOL                            b_mdu_detect;
 *      BOOL                            b_m7_port_detect;
 *  } SB_DVBS_SCAN_DATA_T;
 *  @endcode
 *  @li@c  t_country_code                                    - Contains an ISO 3166 3-character country code.
 *  @li@c  e_scan_type                                       - Specifies the way to scan.
 *  @li@c  t_scan_info                                       - Contains the required scan information.
 *  @li@c  t_eng_cfg_flag                                    - Contains the engine configuration flag.
 *  @li@c  pf_svc_update_nfy                                 - Contains the tag for the service update notification function.
 *  @li@c  pv_svc_update_nfy_tag                             - Contains the tag for the service update notification function.
 *  @li@c  pf_new_svc_nfy                                    - Notify app the information of every new service
 *  @li@c  pv_new_svc_nfy_tag                                - Contains the taf for the new service notification function.
 *  @li@c  pf_sat_name_nfy                                - Notify app the information of satellite name.
 *  @li@c  pv_sat_name_nfy_tag                                - Contains the taf for the satellite name notification function.
 *  @li@c  pf_bat_info_nfy                                - Notify app the information of bat name.
 *  @li@c  pv_bat_info_nfy_tag                                - Contains the taf for the bat name notification function.
 *  @li@c  pf_mdu_detect_nfy                            - Notify AP the mdu detect information
 *  @li@c  pv_mdu_detect_nfy_tag                          - Contain the taf for the mdu detect notification function.
 *  @li@c  b_is_bgm                                             - Specifies whether is bgm scan.
 *  @li@c  b_get_info_step                                    - flag of whether to get information from scan engine for current step
 *  @li@c  b_mdu_detect                                       - flag of whether to get mdu information from scan engine for current step
 *  @li@c  b_m7_port_detect                                  - flag of whether to get M7 port information from scan engine for current step
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_DATA_T
{
    ISO_3166_COUNT_T                t_country_code;
    DVBS_OPERATOR_NAME_T            t_operator_name;
    SB_DVBS_SCAN_TYPE_T             e_scan_type;
    SB_DVBS_SCAN_INFO_T             t_scan_info;
    SB_DVBS_CONFIG_T                t_eng_cfg_flag;
    x_sb_dvbs_svc_update_nfy_fct    pf_svc_update_nfy;
    VOID*                           pv_svc_update_nfy_tag;
    x_sb_new_svc_nfy_fct            pf_new_svc_nfy;
    VOID*                           pv_new_svc_nfy_tag;
    x_sb_sat_name_nfy_fct           pf_sat_name_nfy;
    VOID*                           pv_sat_name_nfy_tag;
    x_sb_bouquet_info_nfy_fct       pf_bat_info_nfy;
    VOID*                           pv_bat_info_nfy_tag;
    x_sb_mdu_detect_nfy_fct         pf_mdu_detect_nfy;
    VOID*                           pv_mdu_detect_nfy_tag;
    BOOL                            b_is_bgm;
    BOOL                            b_get_info_step;
    BOOL                            b_mdu_detect;
    BOOL                            b_m7_port_detect;
} SB_DVBS_SCAN_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_TSL_REC_ID_FREQ_RANGE_T
 *  @brief The sub information for range of lower and upper bound.
 *  @code
 *  typedef struct _SB_DVBS_TSL_REC_ID_FREQ_RANGE
 *  {
 *      UINT32                  ui4_freq_range_lbound;
 *      UINT32                  ui4_freq_range_ubound;
 *  } SB_DVBS_TSL_REC_ID_FREQ_RANGE_T;
 *  @endcode
 *  @li@c  ui4_freq_range_lbound                             - The frequency of lower bound.
 *  @li@c  ui4_freq_range_ubound                             - The frequency of upper bound.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_TSL_REC_ID_FREQ_RANGE
{
    UINT32                  ui4_freq_range_lbound;
    UINT32                  ui4_freq_range_ubound;
} SB_DVBS_TSL_REC_ID_FREQ_RANGE_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_NW_CHANGE_NFY_T
 *  @brief The structure for the notify information when network updated.
 *  @code
 *  typedef struct _SB_DVBS_NW_CHANGE_NFY_T {
 *      x_sb_dvbs_nw_change_nfy_fct  pf_nw_change_nfy;
 *      VOID*                        pv_nw_change_nfy_tag;
 *  } SB_DVBS_NW_CHANGE_NFY_T;
 *  @endcode
 *  @li@c  pf_nw_change_nfy                                  - The callback function for network updated.
 *  @li@c  pv_nw_change_nfy_tag                              - The private tag in notify progress.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_NW_CHANGE_NFY_T {
    x_sb_dvbs_nw_change_nfy_fct  pf_nw_change_nfy;
    VOID*                   pv_nw_change_nfy_tag;
} SB_DVBS_NW_CHANGE_NFY_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SAT_INFO_T
 *  @brief The structure for satllite information .
 *  @code
 *  typedef struct _SB_DVBS_SAT_INFO_T {
 *      UINT8                       ui1_port;
 *      INT16                       i2_orb_pos;
 *      CHAR                        ac_sat_name[MAX_SAT_NAME_LEN];
 *  } SB_DVBS_SAT_INFO_T;
 *  @endcode
 *  @li@c  ui1_port                                - 0:none, 1:A, 2:B, 3:C, 4:D.
 *  @li@c  i2_orb_pos                              - sattlite orbit position.
 *  @li@c  ui4_freq                                - sattlite frequency.
 *  @li@c  ac_sat_name[MAX_SAT_NAME_LEN]           - satllite name.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SAT_INFO_T {
    UINT8                       ui1_port;  
    INT16                       i2_orb_pos;
    INT32                       ui4_freq;
    CHAR                        ac_sat_name[MAX_SAT_NAME_LEN];
} SB_DVBS_SAT_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SCAN_DEFAULT_SCANMODE_TP_SAT_T
 *  @brief The structure contain information of default scan mode, satllite and TP for specific country and operator.
 *  @code
 *  typedef struct _SB_DVBS_SCAN_DEFAULT_SCANMODE_TP_SAT_T {
 *      SB_DVBS_SCAN_TYPE_T                        t_default_scan_type;
 *      SB_DVBS_DLVR_INFO_T                        t_default_tp_info;
 *      UINT16                                     ui2_sat_num;
 *      SB_DVBS_SAT_INFO_T                         t_sat_info[SB_DVBS_MAX_NUM_SAT];
 *  } SB_DVBS_SCAN_DEFAULT_SCANMODE_TP_SAT_T;
 *  @endcode
 *  @li@c  t_default_scan_type                      - dvbs scan type,network scan or auto scan.
 *  @li@c  t_default_tp_info                        - default tp info for scan.
 *  @li@c  ui2_sat_num                              - default number of configured satllites .
 *  @li@c  t_sat_info[SB_DVBS_MAX_NUM_SAT]          - satllite info of configured satllites.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_DEFAULT_SCANMODE_TP_SAT_T{
    SB_DVBS_SCAN_TYPE_T         t_default_scan_type;
    SB_DVBS_DLVR_INFO_T         t_default_tp_info;
    UINT16                      ui2_sat_num;
    SB_DVBS_SAT_INFO_T          t_sat_info[SB_DVBS_MAX_NUM_SAT];
} SB_DVBS_SCAN_DEFAULT_SCANMODE_TP_SAT_T;

/*------------------------------------------------------------------*/
/*! @struct _SB_DVBS_SCAN_SPEC_DEFAULT_SCANMODE_TP_SAT_T
 *  @brief The structure contain information of default scan mode, satllite and TP for specific country and operator.
 *  @code
 *  typedef struct _SB_DVBS_SCAN_SPEC_DEFAULT_SCANMODE_TP_SAT_T {
 *      SB_DVBS_SCAN_TYPE_T                        t_default_scan_type;
 *      SB_DVBS_DLVR_INFO_T                        t_default_tp_info[SB_DVBS_SPEC_MAX_NUM_SAT];
 *      UINT16                                     ui2_sat_num;
 *      SB_DVBS_SAT_INFO_T                         t_sat_info[SB_DVBS_MAX_NUM_SAT];
 *  } _SB_DVBS_SCAN_SPEC_DEFAULT_SCANMODE_TP_SAT_T;
 *  @endcode
 *  @li@c  t_default_scan_type                      - dvbs scan type,network scan or auto scan.
 *  @li@c  t_default_tp_info[SB_DVBS_SPEC_MAX_NUM_SAT]                        - default tp info for scan.
 *  @li@c  ui2_sat_num                              - default number of configured satllites .
 *  @li@c  t_sat_info[SB_DVBS_SPEC_MAX_NUM_SAT]          - satllite info of configured satllites.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_SPEC_DEFAULT_SCANMODE_TP_SAT_T{
    SB_DVBS_SCAN_TYPE_T         t_default_scan_type;
    SB_DVBS_DLVR_INFO_T         t_default_tp_info[SB_DVBS_SPEC_MAX_NUM_SAT];
    UINT16                      ui2_sat_num;
    SB_DVBS_SAT_INFO_T          t_sat_info[SB_DVBS_SPEC_MAX_NUM_SAT];
} SB_DVBS_SCAN_SPEC_DEFAULT_SCANMODE_TP_SAT_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SCAN_GET_CH_INDEX_START_T
 *  @brief The structure contain information of getting the default started channel index.
 *  @code
 *  typedef struct _SB_DVBS_SCAN_GET_CH_INDEX_START_T{
 *          BOOL                        b_is_country_valid;
 *          BOOL                        b_is_operator_valid;
 *          INT16                       i2_orb_pos;     
 *          BOOL                        b_is_orb_pos_valid;
 *
 *  }SB_DVBS_SCAN_GET_CH_INDEX_START_T;
 *  @endcode
 *  @li@c  b_is_country_valid                               - flag of country valid or not
 *  @li@c  b_is_operator_valid                              - flag of operator valid or not
 *  @li@c  i2_orb_pos                                          - degree*10 and range will be -1800~+1800, minus is West, plus is East
 *  @li@c  b_is_orb_pos_valid                              - flag of orbit position valid or not
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_GET_CH_INDEX_START_T{
    BOOL                        b_is_country_valid;
    BOOL                        b_is_operator_valid;
    INT16                       i2_orb_pos;                     
    BOOL                        b_is_orb_pos_valid;
    
}SB_DVBS_SCAN_GET_CH_INDEX_START_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SCAN_CH_INDEX_START_INFO_T
 *  @brief The structure contain output information of getting the default started channel index.
 *  @code
 *  typedef struct _SB_DVBS_SCAN_CH_INDEX_START_INFO_T{
 *          UINT16                        ui2_start_idx;
 *          BOOL                          b_is_applied;
 *
 *  }SB_DVBS_SCAN_CH_INDEX_START_INFO_T;
 *  @endcode
 *  @li@c  ui2_start_idx                                        - start index
 *  @li@c  b_is_applied                                        - flag of the start index is applied or not
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_CH_INDEX_START_INFO_T{
    UINT16                      ui2_start_idx;
    BOOL                        b_is_applied;
    
}SB_DVBS_SCAN_CH_INDEX_START_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_SCAN_MDU_DETECT_INFO_T
 *  @brief The structure contain information of detecting MDU type
 *  @code
 *  typedef struct _SB_DVBS_SCAN_MDU_DETECT_INFO_T{
 *          TUNER_MDU_TYPE_T       e_mdu_type;
 *          SB_DVBS_DLVR_INFO_T t_tp_info;
 *
 *  }SB_DVBS_SCAN_MDU_DETECT_INFO_T;
 *  @endcode
 *  @li@c  e_mdu_type                                        - mdu type
 *  @li@c  t_tp_info                                             - tp information
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_SCAN_MDU_DETECT_INFO_T{
    TUNER_MDU_TYPE_T    e_mdu_type;
    SB_DVBS_DLVR_INFO_T t_tp_info;
    
}SB_DVBS_SCAN_MDU_DETECT_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum SB_DVBS_GET_LO_FREQ_BY_T
 *  @brief Get Lo freq by radio frequency or intermediate frequency.
 *  @code
 *  typedef enum
 *  {
 *      DVBS_GET_LO_FREQ_BY_UNKNOWN = 0,
 *      DVBS_GET_LO_FREQ_BY_RF,
 *      DVBS_GET_LO_FREQ_BY_IF
 *  } SB_DVBS_GET_LO_FREQ_BY_T;
 *  @endcode
 *  @li@c  DVBS_GET_LO_FREQ_BY_UNKNOWN                          - Unknown.
 *  @li@c  DVBS_GET_LO_FREQ_BY_RF                                     - By radio frequency.
 *  @li@c  DVBS_GET_LO_FREQ_BY_IF                                      - By intermediate frequency.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DVBS_GET_LO_FREQ_BY_UNKNOWN,
    DVBS_GET_LO_FREQ_BY_RF,
    DVBS_GET_LO_FREQ_BY_IF
    
}SB_DVBS_GET_LO_FREQ_BY_T;

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_GET_LO_FREQ_INPUT_T
 *  @brief The structure contain input information for getting lo freq for MDU
 *  @code
 *  typedef struct _SB_DVBS_GET_LO_FREQ_INPUT_T{
 *          TUNER_MDU_TYPE_T        e_mdu_type;
 *          TUNER_POLARIZATION_T    e_pol;
 *
 *  }SB_DVBS_GET_LO_FREQ_INPUT_T;
 *  @endcode
 *  @li@c  e_mdu_type                                        - mdu type
 *  @li@c  e_pol                                                  - tuner polarization
 *  @li@c  ui2_freq                                              - value of RF or IF (Mhz)
 *  @li@c  e_by                                                   - by RF or IF
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_GET_LO_FREQ_INPUT_T{
    TUNER_MDU_TYPE_T            e_mdu_type;
    TUNER_POLARIZATION_T        e_pol;
    UINT16                      ui2_freq;
    SB_DVBS_GET_LO_FREQ_BY_T    e_by;

}SB_DVBS_GET_LO_FREQ_INPUT_T;



/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_BGM_SCAN_DATA_T
 *  @brief The structure contain bgm scan data for scanning once.
 *  @code
 *  typedef struct _SB_DVBS_BGM_SCAN_DATA_T{
 *          ISO_3166_COUNT_T                t_country_code;
 *          DVBS_OPERATOR_NAME_T       t_operator_name;
 *          SB_DVBS_SCAN_TYPE_T          e_scan_type;
 *          SB_DVBS_SCAN_INFO_T          t_scan_info;
 *          SB_DVBS_CONFIG_T                t_eng_cfg_flag;
 *
 *  }SB_DVBS_BGM_SCAN_DATA_T;
 *  @endcode
 *  @li@c  t_country_code                                        - country code
 *  @li@c  t_operator_name                                     - operator name
 *  @li@c  e_scan_type                                            - scan type
 *  @li@c  t_scan_info                                              - scan info details
 *  @li@c  t_eng_cfg_flag                                         - scan config setting by user
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_BGM_SCAN_DATA_T
{
    ISO_3166_COUNT_T                t_country_code;
    DVBS_OPERATOR_NAME_T            t_operator_name;
    SB_DVBS_SCAN_TYPE_T             e_scan_type;
    SB_DVBS_SCAN_INFO_T             t_scan_info;
    SB_DVBS_CONFIG_T                t_eng_cfg_flag;

}SB_DVBS_BGM_SCAN_DATA_T;

/* The maximum scan times supported for BGM */
#define SB_DVBS_BGM_SCAN_TOTAL_SUPPORT                          4

/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_BGM_INFO_T
 *  @brief The structure contain bgm info, when user do scanning, the info will be set, and do bgm scan, the info will be got.
 *  @code
 *  typedef struct _SB_DVBS_BGM_INFO_T{
 *          BOOL                                            b_scanned;
 *          UINT16                                          ui2_scan_total;
 *          SB_DVBS_BGM_SCAN_DATA_T       at_scan_data[SB_DVBS_BGM_SCAN_TOTAL_SUPPORT];
 *
 *  }SB_DVBS_BGM_INFO_T;
 *  @endcode
 *  @li@c  b_scanned                                          - User whether do a complete scan or not, Yes: can do bgm, No: can not do bgm.
 *  @li@c  ui2_scan_total                                     - How many times for bgm AP to scan.
 *  @li@c  at_scan_data                                       - Array for saving scan data for each time.
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_BGM_INFO_T
{
    BOOL                        b_scanned;
    UINT16                      ui2_scan_total;
    SB_DVBS_BGM_SCAN_DATA_T     at_scan_data[SB_DVBS_BGM_SCAN_TOTAL_SUPPORT];

}SB_DVBS_BGM_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_CLLSN_AREA_START_IDX_INFO
 *  @brief The structure contain collision area information.
 *  @code
 *  typedef struct _SB_DVBS_CLLSN_AREA_START_IDX_INFO{
 *          UINT32      ui4_mask;
 *          UINT16      ui2_tv_all_idx;
 *          UINT16      ui2_radio_idx;
 *          UINT16      ui2_other1_idx; 
 *          UINT16      ui2_other2_idx;
 *          UINT16      ui2_bank1_st_idx; 
 *          UINT16      ui2_bank2_st_idx;
 *          UINT16      ui2_bank3_st_idx; 
 *          UINT16      ui2_bank4_st_idx;
 *
 *  }SB_DVBS_CLLSN_AREA_START_IDX_INFO;
 *  @endcode
 *  @li@c  ui4_mask                                           - Specifies the mask below whether valid
 *  @li@c  ui2_bank1_st_idx                                    - Bank1 start index
 *  @li@c  ui2_tv_all_idx                                     - TV or all start index
 *  @li@c  ui2_bank2_st_idx                                    - Bank2 start index
 *  @li@c  ui2_radio_idx                                       - Radio start index
 *  @li@c  ui2_bank3_st_idx                                    - Bank3 start index
 *  @li@c  ui2_other1_idx                                     - Other1 start index
 *  @li@c  ui2_bank4_st_idx                                    - Bank4 start index
 *  @li@c  ui2_other2_idx                                      - Other2 start index
 */
/*------------------------------------------------------------------*/
#define COLLISION_AREA_START_IDX_FOR_TV_ALL                     MAKE_BIT_MASK_32(0)
#define COLLISION_AREA_START_IDX_FOR_RADIO                      MAKE_BIT_MASK_32(1)
#define COLLISION_AREA_START_IDX_FOR_OTHER_1                    MAKE_BIT_MASK_32(2)
#define COLLISION_AREA_START_IDX_FOR_OTHER_2                    MAKE_BIT_MASK_32(3)
#define COLLISION_AREA_START_IDX_FOR_BANK1                      MAKE_BIT_MASK_32(10)
#define COLLISION_AREA_START_IDX_FOR_BANK2                      MAKE_BIT_MASK_32(11)
#define COLLISION_AREA_START_IDX_FOR_BANK3                      MAKE_BIT_MASK_32(12)
#define COLLISION_AREA_START_IDX_FOR_BANK4                      MAKE_BIT_MASK_32(13)
typedef struct _SB_DVBS_CLLSN_AREA_START_IDX_INFO
{
    UINT32                          ui4_mask;
    UINT16                          ui2_tv_all_idx;
    UINT16                          ui2_radio_idx;
    UINT16                          ui2_other1_idx;
    UINT16                          ui2_other2_idx;
    
    UINT16                          ui2_bank1_st_idx;
    UINT16                          ui2_bank2_st_idx;
    UINT16                          ui2_bank3_st_idx;
    UINT16                          ui2_bank4_st_idx;
    
}SB_DVBS_CLLSN_AREA_START_IDX_INFO;


/*------------------------------------------------------------------*/
/*! @struct SB_DVBS_IS_M7_OPERATOR_INFO
 *  @brief The structure contains to judge whether is M7 operator information 
 *  @code
 *  typedef struct _SB_DVBS_IS_M7_OPERATOR_INFO{
 *          ISO_3166_COUNT_T                t_country_code;
 *          DVBS_OPERATOR_NAME_T       t_operator_name;
 *
 *  }SB_DVBS_IS_M7_OPERATOR_INFO;
 *  @endcode
 *  @li@c  t_country_code                                        - Specifies the country code
 *  @li@c  t_operator_name                                     - Specifies the operator
 */
/*------------------------------------------------------------------*/
typedef struct _SB_DVBS_IS_M7_OPERATOR_INFO
{
    ISO_3166_COUNT_T                t_country_code;
    DVBS_OPERATOR_NAME_T            t_operator_name;
    
}SB_DVBS_IS_M7_OPERATOR_INFO;

/*------------------------------------------------------------------*/
/*! @struct DVBS_MDU_FREQ_RANGE_CONF_T
 *  @brief The structure contains frequency range information of MDU 
 *  @code
 *  typedef struct _DVBS_MDU_FREQ_RANGE_CONF_T{
 *          UINT16              ui2_rf_low_freq; 
 *          UINT16              ui2_rf_high_freq; 
 *          UINT16              ui2_lo_freq;
 *          UINT8               ui1_polarity_13v_18v;   
 *          UINT16              ui2_start_freq; 
 *          UINT16              ui2_end_freq; 
 *  }DVBS_MDU_FREQ_RANGE_CONF_T;
 *  @endcode
 *  @li@c  ui2_rf_low_freq                                        - Specifies rf low frequency
 *  @li@c  ui2_rf_high_freq                                     - Specifies rf high frequency
 *  @li@c  ui2_lo_freq                                          - Specifies lo frequency
 *  @li@c  ui1_polarity_13v_18v                           - Specifies the POL
 *  @li@c  ui2_start_freq                                     - Specifies the start frequency
 *  @li@c  ui2_end_freq                                     - Specifies the end frequency
 */
/*------------------------------------------------------------------*/
typedef struct _DVBS_MDU_FREQ_RANGE_CONF_T 
{
    UINT16              ui2_rf_low_freq; /*in MHz*/
    UINT16              ui2_rf_high_freq; /*in MHz*/
    UINT16              ui2_lo_freq;/*in MHz*/
    UINT8               ui1_polarity_13v_18v;   /* 0->V(13v) ,1->H(18v)*/
    UINT16              ui2_start_freq; /*in MHz*/
    UINT16              ui2_end_freq; /*in MHz*/
} DVBS_MDU_FREQ_RANGE_CONF_T;

/*------------------------------------------------------------------*/
/*! @struct DVBS_GET_MDU_FREQ_RANGE_CONF_BY
 *  @brief The structure contains information for get mdu frequency range config by input parameters 
 *  @code
 *  typedef struct _DVBS_GET_MDU_FREQ_RANGE_CONF_BY{
 *                    TUNER_MDU_TYPE_T    e_mdu_type;
 *                    UINT16                        ui2_index;
 *  }DVBS_GET_MDU_FREQ_RANGE_CONF_BY;
 *  @endcode
 *  @li@c  e_mdu_type                                        - Specifies mdu type
 *  @li@c  ui2_index                                            - Specifies index to get
 */
/*------------------------------------------------------------------*/
typedef struct _DVBS_GET_MDU_FREQ_RANGE_CONF_BY
{
    TUNER_MDU_TYPE_T    e_mdu_type;
    UINT16              ui2_index;

}DVBS_GET_MDU_FREQ_RANGE_CONF_BY;

/*------------------------------------------------------------------*/
/*! @struct DVBS_M7_SATELLITE_DETECT_INFO_T
 *  @brief The structure contains information for M7 automatic satellite detect table information 
 *  @code
 *  typedef struct _DVBS_M7_SATELLITE_DETECT_INFO_T{
 *          INT16                       i2_orb_pos; 
 *          CHAR                        ac_sat_name[MAX_SAT_NAME_LEN];
 *          UINT16                      ui2_on_id;        
 *          UINT16                      ui2_ts_id;
 *          SB_DVBS_DLVR_INFO_T         t_tp_info;
 *          TUNER_DVB_S_S2_MODE_T       e_dvb_s_s2_mode;
 *          TUNER_DVB_S_S2_MODULATION_T e_dvb_s_s2_modulation;
 *          TUNER_FEC_INNER_T           e_fec_inner;
 *  }DVBS_M7_SATELLITE_DETECT_INFO_T;
 *  @endcode
 *  @li@c  i2_orb_pos                                          - Specifies satellite orbit
 *  @li@c  ac_sat_name                                       - Specifies satellite name
 *  @li@c  ui2_on_id                                            - Specifies ui2_on_id
 *  @li@c  ui2_ts_id                                             - Specifies ui2_ts_id
 *  @li@c  t_tp_info                                             - Specifies t_tp_info to connect
 *  @li@c  e_dvb_s_s2_mode                               - Specifies s or s2 mode
 *  @li@c  e_dvb_s_s2_modulation                       - Specifies modulation
 *  @li@c  e_fec_inner                                         - Specifies FEC
 */
/*------------------------------------------------------------------*/
typedef struct _DVBS_M7_SATELLITE_DETECT_INFO_T
{
    INT16                       i2_orb_pos; 
    CHAR                        ac_sat_name[MAX_SAT_NAME_LEN];
    UINT16                      ui2_on_id;        
    UINT16                      ui2_ts_id;
    SB_DVBS_DLVR_INFO_T         t_tp_info;
    TUNER_DVB_S_S2_MODE_T       e_dvb_s_s2_mode;
    TUNER_DVB_S_S2_MODULATION_T e_dvb_s_s2_modulation;
    TUNER_FEC_INNER_T           e_fec_inner;
    
}DVBS_M7_SATELLITE_DETECT_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum SB_DVBS_CONVERT_FREQ_BY_T
 *  @brief Get frequency by radio frequency or intermediate frequency.
 *  @code
 *  typedef enum
 *  {
 *      DVBS_CONVERT_FREQ_BY_UNKNOWN = 0,
 *      DVBS_CONVERT_FREQ_BY_RF,
 *      DVBS_CONVERT_FREQ_BY_IF
 *  } SB_DVBS_CONVERT_FREQ_BY_T;
 *  @endcode
 *  @li@c  DVBS_CONVERT_FREQ_BY_UNKNOWN                          - Unknown.
 *  @li@c  DVBS_CONVERT_FREQ_BY_RF                                     - By radio frequency.
 *  @li@c  DVBS_CONVERT_FREQ_BY_IF                                      - By intermediate frequency.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DVBS_CONVERT_FREQ_BY_UNKNOWN,
    DVBS_CONVERT_FREQ_BY_RF,
    DVBS_CONVERT_FREQ_BY_IF
    
}SB_DVBS_CONVERT_FREQ_BY_T;

/*------------------------------------------------------------------*/
/*! @struct DVBS_CONVERT_FREQ_INPUT_INFO_T
 *  @brief The structure contains information for converting frequency input parameters.(IF to RF or RF to IF)
 *  @code
 *  typedef struct _DVBS_CONVERT_FREQ_INPUT_INFO_T{
 *          SB_DVBS_GET_LO_FREQ_BY_T    e_by;
 *          UINT32                                      ui4_if_freq;
 *          UINT32                                      ui4_rf_freq;
 *          UINT16                                      ui2_lo_freq;
 *          TUNER_POLARIZATION_T            e_pol;
 *          SATL_REC_T                               t_satl_rec;
 *  }DVBS_CONVERT_FREQ_INPUT_INFO_T;
 *  @endcode
 *  @li@c  e_by                                                   - Specifies the method
 *  @li@c  ui4_if_freq                                           - Specifies if frequency (Mhz)
 *  @li@c  ui4_rf_freq                                           - Specifies rf frequency (Mhz)
 *  @li@c  ui2_lo_freq                                           - Specifies lo frequency (Mhz)
 *  @li@c  e_pol                                                  - Specifies Pol
 *  @li@c  t_satl_rec                                            - Specifies satellite record
 */
/*------------------------------------------------------------------*/
typedef struct _DVBS_CONVERT_FREQ_INPUT_INFO_T
{
    SB_DVBS_CONVERT_FREQ_BY_T   e_by;
    UINT32                      ui4_if_freq;
    UINT32                      ui4_rf_freq;
    UINT16                      ui2_lo_freq;
    TUNER_POLARIZATION_T        e_pol;
    SATL_REC_T                  t_satl_rec;
    
}DVBS_CONVERT_FREQ_INPUT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct DVBS_CONVERT_FREQ_OUTPUT_INFO_T
 *  @brief The structure contains information for converting frequency output parameters.
 *  @code
 *  typedef struct _DVBS_CONVERT_FREQ_OUTPUT_INFO_T{
 *          UINT32                      ui4_freq;
 *  }DVBS_CONVERT_FREQ_OUTPUT_INFO_T;
 *  @endcode
 *  @li@c  ui4_freq                                                   - Specifies the frequency (Mhz) output
 */
/*------------------------------------------------------------------*/
typedef struct _DVBS_CONVERT_FREQ_OUTPUT_INFO_T
{
    UINT32                      ui4_freq;
    
}DVBS_CONVERT_FREQ_OUTPUT_INFO_T;


#endif /* _U_SB_DVBS_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_SVL_BLDR_SB_DVBS_ENG */
/*----------------------------------------------------------------------------*/

