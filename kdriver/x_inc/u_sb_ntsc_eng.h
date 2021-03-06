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
/*! @file u_sb_ntsc_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         SVL Builder NTSC engine.
 */
/*----------------------------------------------------------------------------*/ 
 
#ifndef _U_SB_NTSC_ENG_H_
#define _U_SB_NTSC_ENG_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SB_NTSC_ENG NTSC Engine
 *
 *  @ingroup groupMW_SVL_BLDR
 *  @brief The ntsc engine module is the module of terrestrial/cable svl builder engine
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could create scan engine
 *   or start scan from the exported API of svl builder.
 *   
 *  @see groupMW_SVL_BLDR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define SB_NTSC_TER_ENG_NAME            "sb_ntsc_ter"  /**<  Define the NTSC terrestrial build engine name */
#define SB_NTSC_CAB_ENG_NAME            "sb_ntsc_cab"  /**<  Define the NTSCC cable build engine name */
                                    
#define SB_NTSC_FAKE_TSL_REC            ((UINT8) 1)

#define SB_NTSC_TER_SCAN_FIRST_CHANNEL  ((UINT16)  2)  /**<  MACRO to define the first channe no of air*/
#define SB_NTSC_TER_SCAN_LAST_CHANNEL   ((UINT16) 69)  /**<  MACRO to define the last channe no of air*/ 

#ifndef MW_SB_CABLE_LAST_CHANNEL
#define MW_SB_CABLE_LAST_CHANNEL             135   /**<  MACRO to define the last channe no of cable*/
#endif

#ifndef MW_SB_CABLE_FIRST_CHANNEL
#define MW_SB_CABLE_FIRST_CHANNEL             1    /**<  MACRO to define the first channe no of cable*/
#endif

#define SB_NTSC_CAB_SCAN_FIRST_CHANNEL  ((UINT16)   MW_SB_CABLE_FIRST_CHANNEL) /**<  MACRO to define the first channe no of cable*/
#define SB_NTSC_CAB_SCAN_LAST_CHANNEL   ((UINT16) MW_SB_CABLE_LAST_CHANNEL)  /**<  MACRO to define the last channe no of cable*/

/* This data type contains different configuration flags for the 
   scan process. */
typedef UINT32      SB_NTSC_CONFIG_T;    /**< Define the different configuration flags for the NTSC scan process*/

/** <  To support customer tuner LNA, when set this mask, means AP LNA cfg ON. else, OFF */
#define SB_NTSC_CONFIG_TUNER_LNA						(MAKE_BIT_MASK_32(0))


/*
 *  The structure SB_NTSC_OPEN_DATA_T is used as the argument
 *  pv_open_data of the API c_sb_open_builder()/x_sb_open_builder()
 *  when opening an SVL Builder with NTSC engine.
 *
 *  ui4_svl_id          The id of SVL to be loaded.
 *  ps_svl_name         The name of SVL to be loaded.
 *  ps_file_name        The file name from which the SVL/TSL
 *                      will be loaded.
 *  b_use_orig          A flag. When set to true, use the existed SVL,
 *                      instead of reloading it.  When set to false,
 *                      overwrite the SVL (if exists) by reloading.
 */
 /*------------------------------------------------------------------*/
/*! @struct SB_NTSC_OPEN_DATA_T
 *  @brief It is used as the argument pv_open_data of the API c_sb_open_builder()/x_sb_open_builder() when opening an SVL Builder with ATSC engine.
 *  @code
 *  typedef struct _SB_ATSC_OPEN_DATA_T
 *  {
 *      UINT16                          ui2_svl_id;
 *      CHAR*                           ps_svl_name;
 *      CHAR*                           ps_file_name;
 *      CHAR*                           ps_tuner_name;    
 *      BOOL                            b_use_orig;
 *  }   SB_NTSC_OPEN_DATA_T;
 *  @endcode
 *  @li@c  ui2_svl_id                                        - The id of SVL to be loaded.
 *  @li@c  ps_svl_name                                       - The name of SVL to be loaded.
 *  @li@c  ps_file_name                                      - The file name from which the SVL/TSL will be loaded.
 *  @li@c  ps_tuner_name                                     - The name of tuner to be connectted.
 *  @li@c  b_use_orig                                        - A flag. When set to true, use the existed SVL, instead of reloading it
 */
/*------------------------------------------------------------------*/
typedef struct _SB_NTSC_OPEN_DATA_T
{
    UINT16      ui2_svl_id;
    CHAR*       ps_svl_name;
    CHAR*       ps_file_name;
    CHAR*       ps_tuner_name;    
    BOOL        b_use_orig;
} SB_NTSC_OPEN_DATA_T;


/* NTSC scan type */
/*------------------------------------------------------------------*/
/*! @enum SB_NTSC_SCAN_TYPE_T
 *  @brief It contains different scan types that are used to initialize scan parameters.
 *  @code
 *  typedef enum
 *  {
 *     SB_NTSC_SCAN_TYPE_UNKNOWN = 0,
 *     SB_NTSC_SCAN_TYPE_FULL_MODE,
 *     SB_NTSC_SCAN_TYPE_QUICK_MODE,
 *     SB_NTSC_SCAN_TYPE_ADD_ON_MODE,
 *     SB_NTSC_SCAN_TYPE_SINGLE_RF_CHANNEL,
 *     SB_NTSC_SCAN_TYPE_RANGE_RF_CHANNEL,    
 *     SB_NTSC_SCAN_TYPE_NUM 
 *  } SB_NTSC_SCAN_TYPE_T;
 *  @endcode
 *  @li@c  SB_NTSC_SCAN_TYPE_UNKNOWN                         - Unknown.
 *  @li@c  SB_NTSC_SCAN_TYPE_FULL_MODE                       - Scan all frequency of all bands. It means from first scan index to last.
 *  @li@c  SB_NTSC_SCAN_TYPE_QUICK_MODE                      - Scan all frequency with skip the frequency that already scanned
 *  @li@c  SB_NTSC_SCAN_TYPE_ADD_ON_MODE                     - Add digital channels
 *  @li@c  SB_NTSC_SCAN_TYPE_SINGLE_RF_CHANNEL               - Single Rf scan
 *  @li@c  SB_NTSC_SCAN_TYPE_RANGE_RF_CHANNEL                - Range scan
 *  @li@c  SB_NTSC_SCAN_TYPE_NUM                             - The number of scan type.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_NTSC_SCAN_TYPE_UNKNOWN = 0,
    SB_NTSC_SCAN_TYPE_FULL_MODE,
    SB_NTSC_SCAN_TYPE_QUICK_MODE,
    SB_NTSC_SCAN_TYPE_ADD_ON_MODE,
    SB_NTSC_SCAN_TYPE_SINGLE_RF_CHANNEL,
    SB_NTSC_SCAN_TYPE_RANGE_RF_CHANNEL,    
    SB_NTSC_SCAN_TYPE_NUM /* number of scan types */
} SB_NTSC_SCAN_TYPE_T;

/* frequency plan */
#define SB_NTSC_TER_FREQ_PLAN       ((UINT8) 0x0)  /**<  MACRO to define the frequency plan of air program*/

#define SB_NTSC_CAB_FREQ_PLAN_STD   ((UINT8) 0x1)  /**<  MACRO to define the frequency plan of cable*/
#define SB_NTSC_CAB_FREQ_PLAN_IRC   ((UINT8) 0x2)  /**<  MACRO to define the frequency plan of cable*/
#define SB_NTSC_CAB_FREQ_PLAN_HRC   ((UINT8) 0x4)  /**<  MACRO to define the frequency plan of cable*/
#define SB_NTSC_CAB_FREQ_PLAN_AUTO  ((UINT8) 0x7)  /**<  MACRO to define the frequency plan of cable*/

/*
    callback to calculate the steps of frequency offset
*/

typedef INT16 (*x_sb_ntsc_calc_freq_offset_fct) (
      UINT32            ui4_freq,
      UINT32            ui4_central_freq);

/*
 *  The structure SB_NTSC_SCAN_DATA_T is used as the argument 
 *  pv_scan_data of the API x_sb_start_scan() when scaning 
 *  a media.
 *
 *  e_scan_type     Specifies the way to scan.
 *
 *  ui4_scan_info   Contains the required scan information. 
 * 
 *                  The most significant two bytes indicate the index where
 *                  an engine should start scan while the least significant
 *                  two bytes indicate the index where an engine should 
 *                  stop scan.
 *
 *                  For SB_NTSC_SCAN_TYPE_FULL_MODE, SB_NTSC_SCAN_TYPE_QUICK_MODE,
 *                  SB_NTSC_SCAN_TYPE_ADD_ON_MODE, and 
 *                  SB_NTSC_SCAN_TYPE_SINGLE_RF_CHANNEL and
 *                  SB_NTSC_SCAN_TYPE_RANGE_RF_CHANNELmodes, only the most 
 *                  significant two bytes are taken.       
 *
 *  ps_tuner_name   An optional parameter.  If not NULL, it will be
 *                  used to perform the connection routines. 
 */
 
 /*------------------------------------------------------------------*/
/*! @struct SB_NTSC_SCAN_DATA_T
 *  @brief The sub information for the range of scan index in NTSC scan process.
 *  @code
 *  typedef struct _SB_NTSC_SCAN_DATA_T
 * {
 *   SB_NTSC_SCAN_TYPE_T     e_scan_type;
 *   UINT16                  ui2_start_idx;
 *   UINT16                  ui2_end_idx;
 *   UINT8                   ui1_freq_plan;
 *  } SB_NTSC_SCAN_DATA_T;
 *  @endcode
 *  @li@c  e_scan_type                                      -Specifies the way to scan.
 *  @li@c  ui2_start_scan_idx                               -The start index in this scan process.
 *  @li@c  ui2_end_scan_idx                                 -The end index in this scan process.
 *  @li@c  ui1_freq_plan                                    -The frequency plan for NTSC standard, IRC, HRC
 */
/*------------------------------------------------------------------*/
typedef struct _SB_NTSC_SCAN_DATA_T
{
    HANDLE_T                h_svctx;   
    HANDLE_T                h_svctx_tv_scart_bypass;
    HANDLE_T                h_svctx_monitor_scart_bypass;
    SB_NTSC_SCAN_TYPE_T     e_scan_type;
    UINT16                  ui2_start_idx;
    UINT16                  ui2_end_idx;
    UINT8                   ui1_freq_plan;
	SB_NTSC_CONFIG_T        t_eng_cfg_flag;                  
} SB_NTSC_SCAN_DATA_T;

/*----------------------------------------------------------------------------*/
/*! @} */                                        /* end of groupMW_SB_NTSC_ENG*/
/*----------------------------------------------------------------------------*/

#endif /* _U_SB_NTSCs_ENG_H_ */
