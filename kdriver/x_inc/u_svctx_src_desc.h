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
/*! @file u_svctx.h 
 *  $File: //DTV/RELEASE_BR/r_DTV_X_IDTV1401_002306_12_001_37_001/boot/kdriver/x_inc/u_svctx_src_desc.h $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_SVCTX_SRC_DESC_H_
#define _U_SVCTX_SRC_DESC_H_

/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_mm_common.h"
#include "u_analog_brdcst.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVCTX Service Context
 *
 *  @ingroup groupMW
 *  @brief For user change input source, change channel, select multi media and so on.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could change input source, 
 *   change channel, select multi media and so on.
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
 /* source type begin */
#define SRC_TYPE_BRDCST_SVL                     ((SRC_TYPE_T) 0)    /**< select channel by svl */
#define SRC_TYPE_BRDCST_FREQ                    ((SRC_TYPE_T) 1)    /**< select channel by frequency */

#define SRC_TYPE_AVC                            ((SRC_TYPE_T) 4)    /**<        */

/*SRC_TYPE_BRDCST_SVL , SRC_TYPE_BRDCST_FREQ,   SRC_TYPE_AVC  SRC_TYPE_RECORD*/
#define SRC_TYPE_MEDIA_STORGE                   ((SRC_TYPE_T) 5) /*source is from file storge*/    /**<        */
#define SRC_TYPE_MEDIA_MEM_CLIP                 ((SRC_TYPE_T) 6) /*source is from memory clip*/    /**<        */
#define SRC_TYPE_MEDIA_NETWORK                  ((SRC_TYPE_T) 7) /*source is from network*/    /**<        */
#define SRC_TYPE_MEDIA_TIME_SHIFT               ((SRC_TYPE_T) 8) /*source is from time-shift*/    /**<        */
#define SRC_TYPE_MEDIA_PVR                      ((SRC_TYPE_T) 9) /*source is from PVR*/    /**<        */
#define SRC_TYPE_MEDIA_ATV_TIME_SHIFT           ((SRC_TYPE_T) 10) /*source is from ATV time-shift*/    /**<        */
#define SRC_TYPE_MEDIA_IPTV                     ((SRC_TYPE_T) 11) /*source is from iptv*/    /**<        */

/* source type end*/


 /* pv_get_info for SVCTX_CH_GET_TYPE_CH_SRC */
 /*------------------------------------------------------------------*/
 /*! @enum   CH_SRC_TYPE_T
  *  @brief  signal from tuner or scart
  *  @code
  *  typedef enum
  *  {
  *      CH_SRC_TYPE_UNKNOWN = 0,
  *      CH_SRC_TYPE_TUNER,
  *      CH_SRC_TYPE_SCART
  *  } CH_SRC_TYPE_T;
  *  @endcode
  *  @li@c  CH_SRC_TYPE_UNKNOWN                              unknown- unknown  type
  *  @li@c  CH_SRC_TYPE_TUNER                                    tuner- signal  from tuner
  *  @li@c  CH_SRC_TYPE_SCART                                    scart- signal  from scart
  */
 /*------------------------------------------------------------------*/
 typedef enum
 {
     CH_SRC_TYPE_UNKNOWN = 0,
     CH_SRC_TYPE_TUNER,
     CH_SRC_TYPE_SCART
 } CH_SRC_TYPE_T;
 
 /*------------------------------------------------------------------*/
 /*! @struct SILENT_SELECT_INFO_T
  *  @brief  no message nofity for select servcie
  *  @code
  *  typedef struct _SILENT_SELECT_INFO_T
  *  {
  *      BOOL                            b_enable;
  *      UINT16                          ui2_svl_rec_id;
  *  } SILENT_SELECT_INFO_T;
  *  @endcode
  *  @li@c  b_enable                                             b_enable-  flag for open or close silent 
  *                                                                          
 function
  *  @li@c  ui2_svl_rec_id                                    ui2_svl_rec_id-  identify for svl record
  */
 /*------------------------------------------------------------------*/
 typedef struct _SILENT_SELECT_INFO_T
 {
     BOOL                            b_enable;
     UINT16                          ui2_svc_lst_id; // just for SYS_MHP_SUPPORT
     UINT16                          ui2_svl_rec_id;
 } SILENT_SELECT_INFO_T;
 
 /*------------------------------------------------------------------*/
 /*! @enum   CHANNEL_DECODE_TYPE_T
  *  @brief  the type for channel decode
  *  @code
  *  typedef enum
  *  {
  *      CHANNEL_DECODE_TYPE_DISABLE = 0,
  *      CHANNEL_DECODE_TYPE_MANUAL,
  *      CHANNEL_DECODE_TYPE_AUTO,
  *      CHANNEL_DECODE_TYPE_LAST_VALID_ENTRY
  *  } CHANNEL_DECODE_TYPE_T;
  *  @endcode
  *  @li@c  CHANNEL_DECODE_TYPE_DISABLE                       disalbe- disable
  *  @li@c  CHANNEL_DECODE_TYPE_MANUAL                        manual- user  operation
  *  @li@c  CHANNEL_DECODE_TYPE_AUTO                            auto- auto use  ch_dec
  *  @li@c  CHANNEL_DECODE_TYPE_LAST_VALID_ENTRY     last- skip
  */
 /*------------------------------------------------------------------*/
 typedef enum
 {
     CHANNEL_DECODE_TYPE_DISABLE = 0,
     CHANNEL_DECODE_TYPE_MANUAL,
     CHANNEL_DECODE_TYPE_AUTO,
     CHANNEL_DECODE_TYPE_LAST_VALID_ENTRY
 } CHANNEL_DECODE_TYPE_T;
 
 /*------------------------------------------------------------------*/
 /*! @struct CHANNEL_DECODE_DESC_T
  *  @brief  description for channel decode
  *  @code
  *  typedef struct _CHANNEL_DECODE_DESC_T
  *  {
  *      CHANNEL_DECODE_TYPE_T           e_type;
  *      CHAR                            s_scart_name[CONN_SRC_NAME_MAX_STRLEN+ 1];
  *  } CHANNEL_DECODE_DESC_T;
  *  @endcode
  *  @li@c  e_type                                            e_type- ref  CHANNEL_DECODE_TYPE_T
  *  @li@c  s_scart_name[CONN_SRC_NAME_MAX_STRLEN+1]          name- scart name
  */
 /*------------------------------------------------------------------*/
 typedef struct _CHANNEL_DECODE_DESC_T
 {
     CHANNEL_DECODE_TYPE_T           e_type;
     CHAR                            s_scart_name[CONN_SRC_NAME_MAX_STRLEN+1];
 } CHANNEL_DECODE_DESC_T;
 
 /*------------------------------------------------------------------*/
 /*! @struct SRC_SVL_INFO_T
  *  @brief  the detail info for select service
  *  @code
  *  typedef struct _SRC_SVL_INFO_T  
  *  {
  *      UINT16                          ui2_svc_lst_id;
  *      UINT16                          ui2_svl_rec_id;
  *      UINT32                          ui4_channel_id;
  *      SILENT_SELECT_INFO_T            t_silent;
  *      CHANNEL_DECODE_DESC_T           t_ch_encode;
  *      BOOL                            b_detect_tv_sys;
  *      ANALOG_BRDCST_SCART_MODE_T      e_scart_mode;
  *      BOOL                            b_enable_fake_stereo;
  *      BOOL                            b_enable_signal_booster;
  *  } SRC_SVL_INFO_T;
  *  @endcode
  *  @li@c  ui2_svc_lst_id                            ui2_svc_lst_id- service  list identify
  *  @li@c  ui2_svl_rec_id                            ui2_svl_rec_id- svl  record identify
  *  @li@c  ui4_channel_id                           ui4_channel_id- channel  identify
  *  @li@c  t_silent                                        t_silent- silent  config for select service
  *  @li@c  t_ch_encode                               t_ch_encode- channel  decode config
  *  @li@c  b_detect_tv_sys                         b_detect_tv_sys- is or not  detect tv system
  *  @li@c  e_scart_mode                             e_scart_mode- scart mode
  *  @li@c  b_enable_fake_stereo                b_enable_fake_stereo- open or  close fake 
  *                                                                  stereo  configuration
  *  @li@c  b_enable_signal_booster            b_enable_signal_booster- open  or close
  *                                                                   signal  booster configuration
     @li@c  b_ad_to_independent_ad             let cm set ad's mix type to 
 independent or not  
  */
 /*------------------------------------------------------------------*/
 typedef struct _SRC_SVL_INFO_T  /* SRC_DESC_T's pv_details when e_type = SRC_TYPE_BRDCST_SVL*/
 {
     UINT16                          ui2_svc_lst_id;
     UINT16                          ui2_svl_rec_id;
     UINT32                          ui4_channel_id;
 
     SILENT_SELECT_INFO_T            t_silent;
     CHANNEL_DECODE_DESC_T           t_ch_encode;
     BOOL                            b_detect_tv_sys;
     ANALOG_BRDCST_SCART_MODE_T      e_scart_mode;
     BOOL                            b_enable_fake_stereo;
     BOOL                            b_enable_signal_booster;
     BOOL                            b_ninja_change;
     BOOL                            b_force_change; /* disconnect and connect */
     BOOL                            b_substream_fake_stereo;
     BOOL                            b_ad_to_independent_ad;
 } SRC_SVL_INFO_T;
 
 /* frequency info type */
 /*------------------------------------------------------------------*/
 /*! @enum   FREQ_INFO_TYPE_T
  *  @brief  freqence detail info for select service
  *  @code
  *  typedef enum
  *  {
  *      FREQ_INFO_TYPE_DIG_TER = 0,
  *      FREQ_INFO_TYPE_DIG_CAB,
  *      FREQ_INFO_TYPE_DIG_SAT,
  *      FREQ_INFO_TYPE_ANA_TER,
  *      FREQ_INFO_TYPE_ANA_CAB,
  *      FREQ_INFO_TYPE_ANA_SAT,
  *      FREQ_INFO_TYPE_FM_RADIO,
  *      FREQ_INFO_TYPE_LAST_VALID_ENTRY
  *  } FREQ_INFO_TYPE_T;
  *  @endcode
  *  @li@c  FREQ_INFO_TYPE_DIG_TER   FREQ_INFO_TYPE_DIG_TER- digital terrestrial
  *  @li@c  FREQ_INFO_TYPE_DIG_CAB   FREQ_INFO_TYPE_DIG_CAB-digital cable
  *  @li@c  FREQ_INFO_TYPE_DIG_SAT   FREQ_INFO_TYPE_DIG_SAT- digital satellite
  *  @li@c  FREQ_INFO_TYPE_ANA_TER   FREQ_INFO_TYPE_ANA_TER-analog terrestrial
  *  @li@c  FREQ_INFO_TYPE_ANA_CAB   FREQ_INFO_TYPE_ANA_CAB-analog cable
  *  @li@c  FREQ_INFO_TYPE_ANA_SAT   FREQ_INFO_TYPE_ANA_SAT- analog satellite
  *  @li@c  FREQ_INFO_TYPE_FM_RADIO  FREQ_INFO_TYPE_FM_RADIO- fm radio
  *  @li@c  FREQ_INFO_TYPE_LAST_VALID_ENTRY   FREQ_INFO_TYPE_LAST_VALID_ENTRY-      skip
  */
 /*------------------------------------------------------------------*/
 typedef enum
 {
     FREQ_INFO_TYPE_DIG_TER = 0,
     FREQ_INFO_TYPE_DIG_CAB,
     FREQ_INFO_TYPE_DIG_SAT,
     FREQ_INFO_TYPE_ANA_TER,
     FREQ_INFO_TYPE_ANA_CAB,
     FREQ_INFO_TYPE_ANA_SAT,
     FREQ_INFO_TYPE_FM_RADIO,
     FREQ_INFO_TYPE_LAST_VALID_ENTRY
 } FREQ_INFO_TYPE_T;
 
/* frequency  application scenarios */
/*------------------------------------------------------------------*/
/*! @enum   FREQ_APP_SCENE_T
*  @brief  freqence detail info for select service
*  @code
*  typedef enum
*  {
*      FREQ_APP_SCENE_USUAL = 0,
*      FREQ_APP_SCENE_DIG_MHP,
*      FREQ_APP_SCENE_LAST_VALID_ENTRY,
*  } FREQ_APP_SCENE_T;
*  @endcode
*  @li@c  FREQ_APP_SCENE_USUAL                               FREQ_APP_SCENE_USUAL- usual
*  @li@c  FREQ_APP_SCENE_DIG_MHP                            FREQ_APP_SCENE_DIG_MHP- digital mhp
*  @li@c  FREQ_APP_SCENE_LAST_VALID_ENTRY              FREQ_APP_SCENE_LAST_VALID_ENTRY- skip
*/
/*------------------------------------------------------------------*/
typedef enum
{
    FREQ_APP_SCENE_USUAL = 0,
    FREQ_APP_SCENE_DIG_MHP, /*just use when define  SYS_MHP_SUPPORT */
    FREQ_APP_SCENE_LAST_VALID_ENTRY
} FREQ_APP_SCENE_T;
 
 /* frequency information */
 /*------------------------------------------------------------------*/
 /*! @struct SRC_FREQ_INFO_T
  *  @brief  freqence detail info for select service
  *  @code
  *  typedef struct _SRC_FREQ_INFO_T 
  *  {
  *      FREQ_INFO_TYPE_T                e_type;
  *      union {
  *          TUNER_TER_DIG_TUNE_INFO_T   t_dig_ter;
  *          TUNER_SAT_DIG_TUNE_INFO_T   t_dig_sat;
  *          TUNER_CAB_DIG_TUNE_INFO_T   t_dig_cab;
  *          TUNER_TER_ANA_TUNE_INFO_T   t_ana_ter;
  *          TUNER_SAT_ANA_TUNE_INFO_T   t_ana_sat;
  *          TUNER_CAB_ANA_TUNE_INFO_T   t_ana_cab;
  *      } u_data;
  *  } SRC_FREQ_INFO_T;
  *  @endcode
  *  @li@c  e_type                                            e_type-  frequence source type
  *  @li@c  t_dig_ter                                         t_dig_ter-  config digital terrestrial
  *  @li@c  t_dig_sat                                         t_dig_sat- config digital satellite
  *  @li@c  t_dig_cab                                         t_dig_cab- config digital cable
  *  @li@c  t_ana_ter                                         t_ana_ter- config analog terrestrial
  *  @li@c  t_ana_sat                                         t_ana_sat- config analog satellite
  *  @li@c  t_ana_cab                                         t_ana_cab- config analog cable
  */
 /*------------------------------------------------------------------*/
 typedef struct _SRC_FREQ_INFO_T /* SRC_DESC_T's pv_details when e_type =  SRC_TYPE_BRDCST_FREQ */
 {
     FREQ_INFO_TYPE_T                e_type;
     FREQ_APP_SCENE_T                e_app_scene; /*just use when define  SYS_MHP_SUPPORT */
     UINT16                          ui2_sat_lst_rec_id; // lst_rec_id & lst_id just for MW_DVBS_DEV_ENALBE
     UINT16                          ui2_sat_lst_id;
     
     union {
         TUNER_TER_DIG_TUNE_INFO_T   t_dig_ter;
         TUNER_SAT_DIG_TUNE_INFO_T   t_dig_sat;
         TUNER_CAB_DIG_TUNE_INFO_T   t_dig_cab;
         TUNER_TER_ANA_TUNE_INFO_T   t_ana_ter;
         TUNER_SAT_ANA_TUNE_INFO_T   t_ana_sat;
         TUNER_CAB_ANA_TUNE_INFO_T   t_ana_cab;
     } u_data;
 } SRC_FREQ_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct CH_CUR_SVC_INFO_T
*  @brief  current svc channel info for mhp app 
*  @code
*  typedef struct _CH_CUR_SVC_INFO_T
*  {
*     BRDCST_TYPE_T       e_brdcst_type;
*     UINT16                    ui2_on_id;
*     UINT16                    ui2_ts_id;
*     UINT16                    ui2_svc_id;
*     BOOL                      b_by_freq;
*  }CH_CUR_SVC_INFO_T;
*  @endcode
*  @li@c  e_brdcst_type                                  e_brdcst_type- 
*  @li@c  ui2_on_id                                         ui2_on_id- l
*  @li@c  ui2_ts_id                                         ui2_ts_id-
*  @li@c  ui2_svc_id                                       ui2_svc_id-
*  @li@c  b_by_freq                                        b_by_freq-
*/
/*------------------------------------------------------------------*/
typedef struct _CH_CUR_SVC_INFO_T
{
    UINT16              ui2_on_id;
    UINT16              ui2_ts_id;
    UINT16              ui2_svc_id;
    BRDCST_TYPE_T       e_brdcst_type;
    BOOL                b_by_freq;
}CH_CUR_SVC_INFO_T;
 
/*---------------------------------------------------------------------
  *--------------------End SRC_FREQ_INFO_T-----------------------------------
   ---------------------------------------------------------------------*/

/*---------------------------------------------------------------------
  *--------------------Multimedia Src description ----------------------------
   ---------------------------------------------------------------------*/
/******************** Command set definitions *******************************/
typedef enum
{
    MM_SVC_CMD_CODE_END,
    MM_SVC_CMD_CODE_SVL_NAME,
    MM_SVC_CMD_CODE_SVL_ID,
    MM_SVC_CMD_CODE_SRC_TYPE,
    MM_SVC_CMD_CODE_MEDIA_PATH,
    MM_SVC_CMD_CODE_SET_STRM_INFO,
    MM_SVC_CMD_CODE_EXT_SBTL_PATH,
    MM_SVC_CMD_CODE_TIME_SHIFT_UNIT_TEST,
    MM_SVC_CMD_CODE_SET_BOOT_ANIM,
    MM_SVC_CMD_CODE_EXT_LYRIC_PATH,
    MM_SVC_CMD_CODE_SET_PROTOCL,
    MM_SVC_CMD_CODE_SET_META_INFO,
    MM_SVC_CMD_CODE_SET_PULL_MODE_INFO,
    MM_SVC_CMD_CODE_SET_PUSH_MODE_INFO,
    MM_SVC_CMD_CODE_SET_SRC_SUB_TYPE,
    MM_SVC_CMD_CODE_SET_PLAY_MODE,
    MM_SVC_CMD_CODE_SET_DRM_INFO,
/*------------ DO NOT INSERT OTHER TYPE TO THE FOLLOWING URL--------------*/
    MM_SVC_CMD_CODE_SET_PROXY_URL,
    MM_SVC_CMD_CODE_SET_AGENT_URL,
    MM_SVC_CMD_CODE_SET_COOKIE_URL,
    
    MM_SVC_CMD_CODE_SET_FILL_SCDB_MODE,
    MM_SVC_CMD_CODE_SET_ENCRYPT_INFO,
    MM_SVC_CMD_CODE_SET_IMGRSZ_ID,
    MM_SVC_CMD_CODE_SET_SEAMLESS_TYPE,
    
/*for https server and client certifications*/
    MM_SVC_CMD_CODE_SET_HTTPS_CLT_CERT,
    MM_SVC_CMD_CODE_SET_HTTPS_CLT_KEY,
    MM_SVC_CMD_CODE_SET_HTTPS_SVR_FILE,
    MM_SVC_CMD_CODE_SET_HTTPS_SVR_PATH 
} MM_SVC_CMD_CODE_T;

/*------------------------------------------------------------------*/
/*! @struct MM_SVC_COMMAND_T
 *  @brief Sevice command.
 *  @code
 *  typedef struct
 *  {
 *      MM_SVC_CMD_CODE_T  e_code;
 *      union
 *      {
 *          BOOL       b_boolean;
 *          UINT8      ui1_data;
 *          UINT16     ui2_data;
 *          UINT32     ui4_data;
 *          HANDLE_T   h_obj;
 *          CHAR*      ps_text;
 *          VOID*      pv_data;
 *      } u;
 *  } MM_SVC_COMMAND_T;
 *  @endcode
 *  @li@c  e_code                                            - Command type.
 *  @li@c  b_boolean                                         - True or false.
 *  @li@c  ui1_data                                          - Data1.
 *  @li@c  ui2_data                                          - Data2.
 *  @li@c  ui4_data                                          - Data3.
 *  @li@c  h_obj                                             - Object handle.
 *  @li@c  ps_text                                           - Text.
 *  @li@c  pv_data                                           - Data.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    MM_SVC_CMD_CODE_T  e_code;
    union
    {
        BOOL       b_boolean;
        UINT8      ui1_data;
        UINT16     ui2_data;
        UINT32     ui4_data;
        HANDLE_T   h_obj;
        CHAR*      ps_text;
        VOID*      pv_data;
    } u;
} MM_SVC_COMMAND_T;

/*------------------------------------------------------------------*/
/*! @enum MM_SVC_POS_TYPE_T
 *  @brief Service position type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_POS_TYPE_TT_TIME = 1,
 *      MM_SVC_POS_TYPE_CH_TIME,
 *      MM_SVC_POS_TYPE_ALL_TIME,
 *      MM_SVC_POS_TYPE_TT_OFFSET,
 *      MM_SVC_POS_TYPE_CH_OFFSET,
 *      MM_SVC_POS_TYPE_ALL_OFFSET,
 *      MM_SVC_POS_TYPE_PICTURE_IDX
 *  } MM_SVC_POS_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_POS_TYPE_TT_TIME                           - Total time.
 *  @li@c  MM_SVC_POS_TYPE_CH_TIME                           - Channel time.
 *  @li@c  MM_SVC_POS_TYPE_ALL_TIME                          - All offset.
 *  @li@c  MM_SVC_POS_TYPE_TT_OFFSET                         - Total offset.
 *  @li@c  MM_SVC_POS_TYPE_CH_OFFSET                         - Channel offset.
 *  @li@c  MM_SVC_POS_TYPE_ALL_OFFSET                        - All offset.
 *  @li@c  MM_SVC_POS_TYPE_PICTURE_IDX                       - Picture index.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_POS_TYPE_TT_TIME = 1,
    MM_SVC_POS_TYPE_CH_TIME,
    MM_SVC_POS_TYPE_ALL_TIME, /*file or channel*/
    MM_SVC_POS_TYPE_ALL_TIME_MS,
    MM_SVC_POS_TYPE_TT_OFFSET,
    MM_SVC_POS_TYPE_CH_OFFSET,
    MM_SVC_POS_TYPE_ALL_OFFSET,/*file or channel*/
    MM_SVC_POS_TYPE_PICTURE_IDX
#if 1//def DIVX_PLUS_CER
    ,MM_SVC_POS_TYPE_CHAPTER
#endif
    ,MM_SVC_POS_TYPE_RANGE_AB_START
} MM_SVC_POS_TYPE_T;
/* MM_LAST_MEMORY_SUPPORT or MM_AB_REPEAT_SUPPORT ,is valid */
/* #if (defined(MM_LAST_MEMORY_SUPPORT) || MM_AB_REPEAT_SUPPORT) */ 
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_RENDER_INFO_T
 *  @brief Render information.
 *  @code
 *  typedef struct _MM_SVC_RENDER_INFO_T
 *  {
 *      UINT64  ui8_pts_info;
 *      UINT64  ui8_i_frame_position;
 *      UINT64  ui8_frame_position;
 *      INT32   i4_temporal_reference;
 *      UINT16  ui2_decoding_order;
 *  }MM_SVC_RENDER_INFO_T;
 *  @endcode
 *  @li@c  ui8_pts_info                                      - Pts infor.
 *  @li@c  ui8_i_frame_position                              - I frame position.
 *  @li@c  ui8_frame_position                                - Frame position.
 *  @li@c  i4_temporal_reference                             - Temp reference.
 *  @li@c  ui2_decoding_order                                - Decoding order.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_RENDER_INFO_T
{
    UINT64  ui8_stc;
    UINT64  ui8_aud_pts_info;
    UINT64  ui8_aud_frame_position;
    UINT64  ui8_i_pts_info;
    UINT64  ui8_pts_info;
    UINT64  ui8_i_frame_position;
    UINT64  ui8_frame_position;
    UINT64  ui8_frame_position_disp;
    INT32   i4_temporal_reference;
    UINT16  ui2_decoding_order;
    UINT32  ui4_timestap;
}MM_SVC_RENDER_INFO_T;
/* #endif */

/*------------------------------------------------------------------*/
/*! @struct MM_SVC_POS_INFO_T
 *  @brief Service position information.
 *  @code
 *  typedef struct _MM_SVC_POS_INFO_T
 *  {
 *      MM_SVC_POS_TYPE_T     t_pos_type;
 *      UINT16                ui2_idx;
 *      UINT64                ui8_pos_val;
 *      union
 *      {
 *          MM_SVC_RENDER_INFO_T render_info;
 *      } u;
 *  } MM_SVC_POS_INFO_T;
 *  @endcode
 *  @li@c  t_pos_type                                        - Position type.
 *  @li@c  ui2_idx                                           - Index.
 *  @li@c  ui8_pos_val                                       - Position value.
 *  @li@c  render_info                                       - Render infor.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_POS_INFO_T
{
    MM_SVC_POS_TYPE_T     t_pos_type;      /*(I)*/
    UINT16                ui2_idx;         /*(I)*/
    UINT64                ui8_pos_val;     /*(I)*/
    union
    {
        MM_SVC_RENDER_INFO_T render_info;
    } u;
} MM_SVC_POS_INFO_T;


typedef enum
{
    SVCTX_NTFY_CODE_MEDIA_IS_BUFFERING2PLAY =1 
    /*to be extend*/
}
SVCTX_BUFCTL_NTFY_CODE_T;

typedef UINT32 (*x_svctx_buffctrl_nfy_fct)(
                    HANDLE_T                   h_svctx,          /* in  */
                    SVCTX_BUFCTL_NTFY_CODE_T   e_code,            /* in  */
                    VOID*                      pv_nfy_tag,       /* in  */
                    UINT32                     ui4_data           /*in , ptr with carry inform*/
                    );

typedef enum
{
    MM_SVC_BUFFCTRL_TYPE_BYTE = 1,
    MM_SVC_BUFFCTRL_TYPE_SEC
} MM_SVC_BUFFCTRL_TYPE_T;

typedef struct _MM_SVC_BUFFCTRL_INFO_T
{
  /*------ DO NOT ADD NEW MEMBER HERE---- start --*/ 
   union { 
   UINT32   ui4_bytes;  
   UINT32   ui4_duration; 
   }  ui4_buf_size ;
   UINT32  ui4_keepbuf_threshold;                    /*I*/ 
   UINT32  ui4_rebuf_threshold;                     /*I*/
  /*------ DO NOT ADD NEW MEMBER HERE---- end --*/ 
   MM_SVC_BUFFCTRL_TYPE_T e_ctrl_type;
   BOOL    b_is_prebuff;                            /*I*/
   UINT32  ui4_buff_fullness;                       /*O*/
   UINT32  ui4_buff_emptyness;                      /*O*/   
   x_svctx_buffctrl_nfy_fct       pf_nfy;           /*I*/  
} MM_SVC_BUFFCTRL_INFO_T;

/********************* For Marlin Mp4 Use  **************************/
typedef enum
{
    MM_DRM_TYPE_UNKNOWN  = 0,
    MM_DRM_TYPE_DIVX_DRM,
    MM_DRM_TYPE_MLN
} MM_DRM_TYPE_T;

typedef struct 
{
    HANDLE_T        h_timer;
    UINT32          ui4_max_view_time;
} MM_DRM_CTRL_INFO_T;

typedef struct 
{
    UINT8*              pui1_act_tkn_url;                  /* action token url from CAD file */
    UINT16              ui2_act_tkn_url_length;            /* length of action token url, include null charater '\0' */
    UINT8*              pui1_afl_tkn;                      /* affiliation token from CAD file */
    UINT16              ui2_afl_tkn_length;                /* length of affiliation token, include null charater '\0' */
    UINT8*              pui1_cad_url;                      /* CAD url */
    UINT16              ui2_cad_url_length;                /* length of CAD url, include null charater '\ 0' */
    UINT8*              pui1_act_tkn_data;                 /* action token content, */
    UINT16              ui2_act_tkn_data_length;           /* length of action token content, include null charater '\ 0' */
} MM_MLN_DRM_INFO_T;

typedef struct 
{
    MM_DRM_TYPE_T       e_drm_type;
    union
    {
        MM_MLN_DRM_INFO_T   t_mln_drm_inf;
    }u;
} MM_DRM_INFO_T;

/********************* Multimedia Info *********************************/
/*------------------------------------------------------------------*/
/*! @struct SRC_MM_INFO_T
 *  @brief MM information
 *  @code
 *  typedef struct _SRC_MM_INFO_T
 *  {
 *      HANDLE_T                        h_media;
 *      MM_SPEED_TYPE_T                 t_speed;
 *      MM_SVC_POS_INFO_T               t_pos_info;
 *      MM_SVC_BUFFCTRL_INFO_T    t_bufctrl_info;
 *  #if 0
 *      UINT16                          ui2_svc_lst_id;
 *      UINT16                          ui2_svl_rec_id;
 *  #endif
 *  } SRC_MM_INFO_T;
 *  @endcode
 *  @li@c  h_media                                           - Media handle.
 *  @li@c  t_speed                                           - Speed.
 *  @li@c  t_pos_info                                        - Position information.
 *  @li@c  ui2_svc_lst_id                                    - Service list ID.
 *  @li@c  ui2_svl_rec_id                                    - Service record ID.
 */
/*------------------------------------------------------------------*/
typedef struct _SRC_MM_INFO_T
{
    HANDLE_T                        h_media;
    MM_SPEED_TYPE_T                 t_speed;
    MM_SVC_POS_INFO_T               t_pos_info;
    MM_SVC_BUFFCTRL_INFO_T          t_bufctrl_info;
    BOOL                            b_still_at_last;
    BOOL                            b_need_vid_mirror;
    BOOL                            b_sub_free_run;
    /* TS file Program ID */
    UINT16                          ui2_pmt_idx;

    /* buffer control mode: by 3rd or mw */
    MM_BUFFER_CONTROL_MODE_T        e_buf_ctrl_mode;
    UINT32                          ui4_title_idx;
    UINT32                          ui4_playlist_idx;
    UINT32                          ui4_chap_idx;
    UINT16                          ui2_aud_idx;
    UINT16                          ui2_sub_idx;
    MM_DRM_INFO_T                   t_ml_drm;
} SRC_MM_INFO_T;

#endif /* endif _U_SVCTX_SRC_DESC_H_ */

