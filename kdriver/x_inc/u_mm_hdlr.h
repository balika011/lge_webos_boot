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
/*! @file u_mm_hdlr.h
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVCTX_MM_HDLR Multi Media Handler
 *
 *  @ingroup groupMW_SVCTX
 *  @brief Provides external function TV Application
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_SVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_MM_HDLR_H_
#define _U_MM_HDLR_H_
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_mm_common.h"
#include "u_handle.h"
#include "u_drm.h"
#include "u_divx_drm.h"
#include "u_drm_drv.h"
#include "u_minfo.h"
#include "u_sm_cc_hdlr.h"
#if 1//MM_LYRIC_SUPPORT
#include "u_mm_sbtl_engine.h"
#endif

#include "u_svctx_src_desc.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define HT_MEDIA_STORGE_ELM_TYPE                (HT_GROUP_MSVCTX+3)    /**<        */
#define HT_MEDIA_MEM_ELM_TYPE                   (HT_GROUP_MSVCTX+4)    /**<        */
#define HT_MEDIA_NETWORK_ELM_TYPE               (HT_GROUP_MSVCTX+5)    /**<        */
#define SVCTX_MM_VID_PROFILE_LEVEL_INFO_T   VSH_SRC_PROFILE_LEVEL_INFO_T
#define SVCTX_MM_AUDIO_INFO_T               ASH_AUDIO_INFO_T

#define MM_AB_REPEAT_SUPPORT                    1    /**<        */

typedef enum _MM_SVC_PROTOCOL_T
{
    PROTOCL_MODE_UNKNOWN,
    PROTOCL_MODE_PULL,
    PROTOCL_MODE_PUSH,
    PROTOCL_MODE_URI,
    PROTOCL_MODE_DLNA
} MM_SVC_PROTOCOL_T;

/******************** Fill SCDB Mode *******************************/
typedef UINT32 MM_SVC_FILL_SCDB_MODE;

#define MM_SVC_FILL_SCDB_UPDATE              ((MM_SVC_FILL_SCDB_MODE) 0)
#define MM_SVC_FILL_SCDB_APPEND              ((MM_SVC_FILL_SCDB_MODE) 1)

#if 1//def DIVX_PLUS_CER
#define MM_DIVX_PLUS_ID_TO_POS(_pl_id, _chap_id)    ((UINT64)((UINT64)(((UINT64)_pl_id) << 32) | (UINT64)_chap_id))
#define MM_DIVX_PLUS_POS_TO_PL_ID(_pos)             ((UINT32)(((UINT64)_pos) >> 32))
#define MM_DIVX_PLUS_POS_TO_CHAP_ID(_pos)           ((UINT32)(((UINT64)_pos) & 0xFFFFFFFF))
#endif



/*! @enum MM_PLAY_MODE_T
 *  @brief play mode
 *  @code
 *  typedef enum
 *  {
 *      MM_PLAY_MODE_PREV = 0,
 *      MM_PLAY_MODE_PLAY
 *  } MM_PLAY_MODE_T;
 *  @endcode
 *  @li@c  MM_PLAY_MODE_PREV                             - preview mode.
 *  @li@c  MM_PLAY_MODE_PLAY                              - play mode.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_PLAY_MODE_PLAY = 0,
    MM_PLAY_MODE_PREV
} MM_PLAY_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum MM_EXT_SBTL_TYPE_T
 *  @brief External subtitle type
 *  @code
 *  typedef enum
 *  {
 *      MM_EXT_SBTL_TYPE_FULL = 0,
 *      MM_EXT_SBTL_TYPE_LST
 *  } MM_EXT_SBTL_TYPE_T;
 *  @endcode
 *  @li@c  MM_EXT_SBTL_TYPE_FULL                             - Full.
 *  @li@c  MM_EXT_SBTL_TYPE_LST                              - List.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_EXT_SBTL_TYPE_FULL = 0,              /*full path is valid.*/
    MM_EXT_SBTL_TYPE_LST                    /*extension list is valid.*/
} MM_EXT_SBTL_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum MM_LYRIC_TYPE_T
 *  @brief Lyric type
 *  @code
 *  typedef enum
 *  {
 *      MM_LYRIC_TYPE_LRC = 0,
 *  } MM_LYRIC_TYPE_T;
 *  @endcode
 *  @li@c  MM_LYRIC_TYPE_LRC                             - LRC format.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_LYRIC_TYPE_LRC = 0,
    MM_LYRIC_TYPE_LRC_FULL_PATH
} MM_LYRIC_TYPE_T;
/*------------------------------------------------------------------*/
/*! @struct MM_EXT_SBTL_INFO
 *  @brief Subtitle information.
 *  @code
 *  typedef struct
 *  {
 *      MM_EXT_SBTL_TYPE_T  e_type;
 *      union
 *      {
 *          CHAR*           ps_full;
 *          CHAR*           ps_ext_lst;
 *      } u;
 *      CHAR*   ps_prf_lang_lst;
 *  } MM_EXT_SBTL_INFO;
 *  @endcode
 *  @li@c  e_type                                            - Type.
 *  @li@c  ps_full                                           - Contains the pull path of the external subtitle
 *  @li@c  ps_ext_lst                                        - Contains the extension list and extensions are separated by ';'
 *  @li@c  ps_prf_lang_lst                                   - ISO 639 3 character code. EX: "eng;chi"
 */
/*------------------------------------------------------------------*/
typedef struct
{
    MM_EXT_SBTL_TYPE_T  e_type;
    union
    {
        CHAR*           ps_full;            /*A null character terminated string that contains the pull path of the external subtitle*/
        CHAR*           ps_ext_lst;         /*A null character terminated extension list and extensions are separated by ';'.
                                                 The order represents the extension priority. Ex: "srt;sub;ssa"*/
    } u;
    CHAR*   ps_prf_lang_lst;                  /*ISO 639 3 character code. EX: "eng;chi"*/
} MM_EXT_SBTL_INFO;
/*------------------------------------------------------------------*/
/*! @struct MM_USER_SEL_EXT_SBTL_INFO
 *  @brief user select ext subtitle info after play
 *  @code
 *  typedef struct
 *   {
 *       BOOL            b_is_related;
 *       BOOL            b_is_http;
 *       BOOL            b_is_add;
 *       BOOL            b_is_update_scdb;
 *       CHAR*           ps_full_path;           
 *       CHAR*           ps_related_full_path;
 *       HANDLE_T        h_ext_subtitle;
 *       HANDLE_T        h_related_subtitle;
 *       HANDLE_T        h_last_ext_subtitle;
 *       HANDLE_T        h_last_related_subtitle;
 *       BOOL            b_is_last_http;
 *       BOOL            b_is_xml;
 *    } MM_USER_SEL_EXT_SBTL_INFO;
 *  @endcode
 *  @li@c  b_is_related;                                         - related subtitle sub+idx
 *  @li@c  b_is_http;                                       - http flag
 *  @li@c  b_is_add;                                          - add info flag
 *  @li@c  b_is_update_scdb;                                        - update scdb flag
 *  @li@c  ps_full_path;                                             - full subtitle path
 *  @li@c  ps_related_full_path;                                           - related subtitle path
 *  @li@c  h_ext_subtitle;                                     - handle of subtitle file
 *  @li@c  h_related_subtitle;                                         - handle of related subtitle file
 *  @li@c  h_last_ext_subtitle;                                             - save for free resource 
 *  @li@c  h_last_related_subtitle;                                         - save for free resource 
 *  @li@c  b_is_last_http;                                    - save for free resource 
 *  @li@c  b_is_xml;                                         - xml subtitle flag
 */
/*------------------------------------------------------------------*/
typedef struct
{
    BOOL            b_is_related;
    BOOL            b_is_http;
    BOOL            b_is_add;
    BOOL            b_is_update_scdb;
    CHAR*           ps_full_path;           
    CHAR*           ps_related_full_path;
    HANDLE_T        h_ext_subtitle;
    HANDLE_T        h_related_subtitle;
    HANDLE_T        h_last_ext_subtitle;
    HANDLE_T        h_last_related_subtitle;
    BOOL            b_is_last_http;
    BOOL            b_is_xml;
} MM_USER_SEL_EXT_SBTL_INFO;


/*------------------------------------------------------------------*/
/*! @struct MM_LYRIC_INFO
 *  @brief Lyric information.
 *  @code
 *  typedef struct
 *  {
 *      MM_LYRIC_TYPE_T  e_type;
 *      union
 *      {
 *          CHAR*           ps_lyric_list;
 *      } u;
 *      CHAR*   ps_prf_lang_lst;
 *  } MM_LYRIC_INFO;
 *  @endcode
 *  @li@c  e_type                                               - Type.
 *  @li@c  ps_lyric_list                                        - Contains the extension list and extensions are separated by ';'
 *  @li@c  ps_prf_lang_lst                                   - ISO 639 3 character code. EX: "eng;chi"
 */
/*------------------------------------------------------------------*/
typedef struct
{
    MM_LYRIC_TYPE_T  e_type;
    union
    {
        CHAR*           ps_full;
        CHAR*           ps_lyric_list;         /*A null character terminated extension list and extensions are separated by ';'.
                                                 The order represents the extension priority. Ex: "txt;lrc;ssa"*/
    } u;
    CHAR*   ps_prf_lang_lst;                  /*ISO 639 3 character code. EX: "eng;chi"*/
} MM_LYRIC_INFO;

/*------------------------------------------------------------------*/
/*! @struct MM_SVC_DASH_VIDEO_SEQHEADER_INFO
 *  @brief Sevice command.
 *  @code
 *  typedef struct
 *  {
 *          UINT32    ui4_width;
 *          UINT32    ui4_height;
 *          UINT32    ui4_frame_rate;
 *          UINT8     pui1_seq_buf[512];
 *          UINT32    ui4_buffer_size;
 *          UINT32    ui4_time_scale;
 *          BOOL      b_is_raw;
 *          BOOL      b_is_second_raw;
 *  } MM_SVC_DASH_VIDEO_SEQHEADER_INFO;
 *  @endcode
 *  @li@c  ui4_width       
 *  @li@c  ui4_height                   
 *  @li@c  ui4_frame_rate           
 *  @li@c  pui1_seq_buf               
 *  @li@c  ui4_buffer_size            
 *  @li@c  ui4_time_scale                  
 *  @li@c  b_is_raw                          
 *  @li@c  b_is_second_raw                     
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32    ui4_width;
    UINT32    ui4_height;
    UINT32    ui4_frame_rate;
    UINT8     pui1_seq_buf[512];
    UINT32    ui4_buffer_size;       
    UINT32    ui4_time_scale;
    BOOL      b_is_raw;
    BOOL      b_is_second_raw;
}MM_SVC_DASH_VIDEO_SEQHEADER_INFO;


/*------------------------------------------------------------------*/
/*! @struct MM_SVC_DASH_AUDIO_SEQHEADER_INFO
 *  @brief Sevice command.
 *  @code
 *  typedef struct
 *  {
 *          INT16     i2_channle;
 *          UINT32    ui4_sample_rate;
 *          UINT32    ui4_profile;
 *          BOOL        b_frame_length;
 *          BOOL      b_is_audio_change;
 *  } MM_SVC_DASH_AUDIO_SEQHEADER_INFO;
 *  @endcode
 *  @li@c  i2_channle;
 *  @li@c  ui4_sample_rate
 *  @li@c  ui4_profile
 *  @li@c  b_frame_length
 *  @li@c  b_is_audio_change
 */
/*------------------------------------------------------------------*/
typedef struct
{
    INT16     i2_channle;
    UINT32    ui4_sample_rate;
    UINT32    ui4_profile;
    BOOL        b_frame_length;
    BOOL      b_is_audio_change;  //for multi description.
}MM_SVC_DASH_AUDIO_SEQHEADER_INFO;


/*------------------------------------------------------------------*/
/*! @struct MM_SVC_DASH_SEQHEADER_INFO
 *  @brief Sevice command.
 *  @code
 *  typedef struct
 *  {
 *      STREAM_TYPE_T    e_strm_type;
 *      union
 *      {
 *          MM_SVC_DASH_VIDEO_SEQHEADER_INFO t_vid_seqheader_info;
 *          MM_SVC_DASH_AUDIO_SEQHEADER_INFO t_aud_seqheader_info;
 *      } u;
 *  } MM_SVC_DASH_SEQHEADER_INFO;
 *  @endcode
 *  @li@c  e_strm_type
 *  @li@c  t_vid_seqheader_info
 *  @li@c  t_aud_seqheader_info
 */
/*------------------------------------------------------------------*/
typedef struct
{
    STREAM_TYPE_T    e_strm_type;
    union
    {
        MM_SVC_DASH_VIDEO_SEQHEADER_INFO t_vid_seqheader_info;
        MM_SVC_DASH_AUDIO_SEQHEADER_INFO t_aud_seqheader_info;
    }u;
}MM_SVC_DASH_SEQHEADER_INFO;


#define SVCTX_GRP_MM                   (4<<24)    /**<24 = SVCTX_TYPE_CODE_BITS        */
/****************** Multimedia handler set types *****************************/
typedef enum
{
    SVCTX_MM_SET_TYPE_TRICK = SVCTX_GRP_MM,
    SVCTX_MM_SET_TYPE_BTN,
    SVCTX_MM_SET_TYPE_REPEAT,
    SVCTX_MM_SET_TYPE_SEEK,
    SVCTX_MM_SET_TYPE_SEND_PUSH_BUF,
    SVCTX_MM_SET_TYPE_REL_PUSH_BUF,
    SVCTX_MM_SET_TYPE_SEND_IBC,
    SVCTX_MM_SBTL_SET_TYPE_DISP_MODE,
    SVCTX_MM_SBTL_SET_TYPE_HILT_STL,
    SVCTX_MM_SBTL_SET_TYPE_TIME_OFST,
    SVCTX_MM_SBTL_SET_TYPE_FONT_ENC,
    SVCTX_MM_SBTL_SET_TYPE_HILT_ATTR,
    SVCTX_MM_SBTL_SET_TYPE_NRML_ATTR,
    SVCTX_MM_SBTL_SET_TYPE_SHOW_HIDE,
    SVCTX_MM_SBTL_SET_TYPE_DEFAULT,
    SVCTX_MM_SET_TYPE_PULL_INFO,
    SVCTX_MM_SET_TYPE_STRM_INFO,
    SVCTX_MM_SET_TYPE_STRM_CODEC_INFO,
    SVCTX_MM_SET_TYPE_CHANGE_STRM,
    SVCTX_MM_SET_TYPE_AUTOTEST_INFO,
    SVCTX_MM_SET_TYPE_TICK_CTRL_BLK,
    SVCTX_MM_SET_TYPE_3D_INFO,
    SVCTX_MM_SET_SUBTITLE_ENCODING,
    SVCTX_MM_SET_TYPE_LANGUAGE,
    SVCTX_MM_SET_TYPE_SEQENCE_INFO,
    SVCTX_MM_SBTL_SET_FONT_BG_COLOR,
    SVCTX_MM_SET_TYPE_ID3_ENCOD,
    SVCT_MM_SET_SUBTITLE_REGION,
    SVCTX_MM_SBTL_SET_PRDY_ENCRPT_KEY_INFO,
    SVCTX_MM_SET_TYPE_DOWNLOAD_SPEED,
    SVCTX_MM_SET_CANCEL_URI_READ,
    SVCTX_MM_SET_ENCRYPT_INFO,
    SVCTX_MM_SET_DASH_SEQ_HEADER_INFO,
    SVCTX_MM_SET_FLUSH_FEEDER,
    SVCTX_MM_SET_TYPE_FLUSH_AUDIO,
    SVCTX_DRM_SET_TYPE_PLAYBACK,
    SVCTX_DRM_SET_TYPE_EXT_SUBTITLE_PREFER,
    SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_DEACTIVATION,
    SVCTX_MM_SET_SBTL_CC_SET_INFO,              /*for ipcc feature*/
    SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_MEMORY_RW_FUNC,
    SVCTX_MM_SET_SBTL_USER_SET_INFO,
    SVCTX_MM_SET_TYPE_SET_PCR,
    SVCTX_MM_SET_IFRAME_MODE,
    SVCTX_MM_SET_REPEAT_MODE,
    SVCTX_MM_SET_DEBUG_INFO,
    SVCTX_MM_SET_SBTL_DUMMY_FUNC,
    SVCTX_MM_SET_GUIDANCE_FLAG,
    SVCTX_MM_SET_CC_IDX,
    SVCTX_MM_SET_CC_CANVAS,
    SVCTX_MM_SET_OSD_INDEX,
    SVCTX_MM_SET_AUDIO_AD_TO_INDEPENDENT_AD,
    SVCTX_MM_SET_RATING_UNBLOCK,
    SVCTX_MM_SET_DASH_SBTL_INFO,
    SVCTX_MM_SET_SP_CB,
    SVCTX_MM_SET_SP_REL_DATA,
    SVCTX_MM_SET_TYPE_STRM_FLAG,
    SVCTX_MM_SET_RESET_WFD_INFO,    
    SVCTX_MM_SET_FORCE_RATING
} SVCTX_MM_SET_TYPE_T;


/******************* Multimedia handler get types ****************************/
typedef enum
{
    SVCTX_MM_GET_TYPE_TIME_CODE = SVCTX_GRP_MM,
    SVCTX_MM_GET_TYPE_DUR,
    SVCTX_MM_GET_TYPE_TT_INFO,
    SVCTX_MM_GET_TYPE_CH_INFO,
    SVCTX_MM_GET_TYPE_POSIT_INFO,
    SVCTX_MM_GET_TYPE_MEDIA_FORMAT,
    SVCTX_MM_GET_TYPE_MENU_INFO,
    SVCTX_MM_GET_TYPE_UOP_CAP_INFO,
    SVCTX_MM_GET_TYPE_SPEED,
    SVCTX_MM_GET_TYPE_VIDEO_INFO,
    SVCTX_MM_GET_TYPE_META_DATA,
    SVCTX_MM_GET_TYPE_CURR_PROCESS_PROGRESS,
    SVCTX_MM_GET_TYPE_TIMESHIFT_RANGE_INFO,
    SVCTX_MM_GET_BRDCST_TYPE,
    SVCTX_MM_GET_TYPE_POS_PTS,
    SVCTX_MM_GET_TYPE_VID_CODEC_INFO,
    SVCTX_MM_GET_TYPE_BUFFCTRL_INFO,
    SVCTX_MM_GET_TYPE_STRM_INFO,
    SVCTX_MM_GET_TYPE_GENERIC_INFO,
    SVCTX_MM_GET_TYPE_PUSH_BUFF,
    SVCTX_MM_GET_TYPE_TITLE_NUM,
    SVCTX_MM_GET_TYPE_TITLE_INFO,
    SVCTX_MM_GET_TYPE_PLAYLIST_INFO,
    SVCTX_MM_GET_TYPE_CHAP_INFO,
    SVCTX_MM_GET_TYPE_DISP_INFO,
    SVCTX_MM_GET_TYPE_CUR_CHAP_INFO,
    SVCTX_MM_GET_TYPE_CUR_POS,
    SVCTX_MM_SBTL_GET_TYPE_LYRIC_INFO,
    SVCTX_MM_GET_TYPE_TIME_TO_SIZE,
    SVCTX_MM_GET_TYPE_PTS,
    SVCTX_MM_GET_VS_BIT_RATE,
    SVCTX_MM_GET_MEM_BAND_STATUS,
    SVCTX_MM_GET_PROGRAM_NUM,
    SVCTX_MM_GET_MM_IDENTIFIER,
    SVCTX_MM_GET_STOP_INFO,
    SVCTX_MM_GET_SUBTITLE_TYPE,
    SVCTX_MM_GET_TYPE_RAW_DATA,
    SVCTX_MM_GET_TYPE_ID3_RAW_DATA,
    SVCTX_MM_GET_TYPE_TIME_CODE_MS,
    SVCTX_MM_GET_BUF_STATUS,
    SVCTX_MM_GET_THUMBNAIL,
    SVCTX_MM_GET_HTTP_DOWNLOAD_SPEED,
    SVCTX_MM_GET_TYPE_3D_VID_INFO,
    SVCTX_MM_GET_TYPE_AFIFO_AMOUNT,
    SVCTX_MM_GET_HTTP_SERVER_INFO,
    SVCTX_MM_GET_TYPE_BUF_UNDERFLOW,
    SVCTX_MM_GET_TYPE_SRC_SUB_TYPE,
    SVCTX_MM_GET_TYPE_IS_AUD,
    SVCTX_DRM_GET_TYPE_INFO,
    SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_REGISTRATION_CODE,
    SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_UI_HELP_INFO,
    SVCTX_MM_GET_TYPE_RESUME_TRACK_INFO,
    SVCTX_MM_GET_TYPE_AUD_INFO,
    SVCTX_MM_GET_SIGNAL_STATUS,
    SVCTX_MM_GET_CURRENT_TICK,
    SVCTX_MM_GET_TYPE_IDX_TYPE,
    SVCTX_MM_GET_TYPE_VID_PROFILE_LEVEL,
    SVCTX_MM_GET_TYPE_RUNNING_SBTL_TYPE,
    SVCTX_MM_GET_TYPE_CHANGED_SBTL_IDX,
    SVCTX_MM_GET_TYPE_CHANGED_CC_ATTR,
    SVCTX_MM_GET_TYPE_CC_CANVAS,
    SVCTX_MM_GET_GUIDANDCE_INFO,
    SVCTX_MM_GET_SP_CB,
    SVCTX_MM_GET_RM_RES_NUMBER,
    SVCTX_MM_GET_TYPE_WFD_ERROR_INFO,
    SVCTX_MM_GET_AUX_AUDIO_STATUS
} SVCTX_MM_GET_TYPE_T;

typedef struct _MM_SVC_WFD_ERR_INFO_T
{
	INT32 i4TimeoutCnt;
	INT32 i4LackSeqHeaderCnt;
	INT32 i4OutOfSpecCnt;
} MM_SVC_WFD_ERR_INFO_T;


/* This structure defines divx drm info for svctx. */
/*------------------------------------------------------------------*/
/*! @struct DIVX_DRM_INFO_SVCTX_T
 *  @brief Divx drm information
 *  @code
 *  typedef struct _DIVX_DRM_INFO_SVCTX_T
 *  {
 *      DIVX_DRM_BASIC_INFO_T* pt_divx_drm_basic_info;
 *      DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T* pt_drm_output_signal_protection_info;
 *  } DIVX_DRM_INFO_SVCTX_T;
 *  @endcode
 *  @li@c  pt_divx_drm_basic_info                            - Basic information.
 *  @li@c  pt_drm_output_signal_protection_info              - Output signal protection infor.
 */
/*------------------------------------------------------------------*/
typedef struct _DIVX_DRM_INFO_SVCTX_T
{
    DIVX_DRM_BASIC_INFO_T* pt_divx_drm_basic_info;
    DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T* pt_drm_output_signal_protection_info;
} DIVX_DRM_INFO_SVCTX_T;

/* This structure defines drm info for svctx. */
/*------------------------------------------------------------------*/
/*! @struct DRM_INFO_SVCTX_T
 *  @brief DRM information.
 *  @code
 *  typedef struct _DRM_INFO_SVCTX_T
 *  {
 *      DRM_TYPE_T t_drm_type;
 *      union _DRM_INFO_UNION_SVCTX_T
 *      {
 *          DIVX_DRM_INFO_SVCTX_T t_divx_drm_info_svctx;
 *      } u;
 *  } DRM_INFO_SVCTX_T;
 *  @endcode
 *  @li@c  t_drm_type                                        - Type.
 *  @li@c  t_divx_drm_info_svctx                             - DRM infor.
 */
/*------------------------------------------------------------------*/
typedef struct _DRM_INFO_SVCTX_T
{
    DRM_TYPE_T t_drm_type;
    UINT32     ui4_idx;
    union _DRM_INFO_UNION_SVCTX_T
    {
        DIVX_DRM_INFO_SVCTX_T t_divx_drm_info_svctx;
        MINFO_RAW_DRM_OBJ_T t_raw_drm_info;
    } u;
} DRM_INFO_SVCTX_T;


/******************* Multimedia Search Type **********************************/
#if 1//MM_LYRIC_SUPPORT
typedef enum
{
    MM_SBTL_DISP_MODE_OFF = 0,
    MM_SBTL_DISP_MODE_SINGLE_LINE,
    MM_SBTL_DISP_MODE_MULTI_LINE
} MM_SBTL_DISP_MODE_T;

typedef struct _SVC_MM_SBTL_DISP_MODE_T
{
    MM_SBTL_DISP_MODE_T          e_disp_mode;
    UINT16                       ui2_param;
} MM_SVC_SBTL_DISP_MODE_T;

typedef enum
{
    MM_SBTL_HILT_STL_BY_LINE = 0,
    MM_SBTL_HILT_STL_KARAOKE
} MM_SBTL_HILT_STL_T;

typedef struct _SVC_MM_SBTL_HLT_STL_T
{
    MM_SBTL_HILT_STL_T       e_hlt_style;
    UINT16                   ui2_param;
} MM_SVC_SBTL_HILT_STL_T;

typedef enum
{
    MM_SBTL_TIME_OFST_OFF = 0,
    MM_SBTL_TIME_OFST_AUTO,
    MM_SBTL_TIME_OFST_USER_DEF
} MM_SBTL_TIME_OFST_T;

typedef struct _SVC_MM_SBTL_TIME_OFST_T
{
    MM_SBTL_TIME_OFST_T       e_time_ofst;
    UINT32                    ui4_ofst_value;
} MM_SVC_SBTL_TIME_OFST_T;

typedef enum
{
    MM_SBTL_FONT_ENC_AUTO = 0,
    MM_SBTL_FONT_ENC_UTF8,
    MM_SBTL_FONT_ENC_UTF16,
    MM_SBTL_FONT_ENC_BIG5,
    MM_SBTL_FONT_ENC_GB,
    MM_SBTL_FONT_ENC_GB18030,
    MM_SBTL_FONT_ENC_CODEPAGE1250,
    MM_SBTL_FONT_ENC_CODEPAGE1251,
    MM_SBTL_FONT_ENC_CODEPAGE1252,
    MM_SBTL_FONT_ENC_CODEPAGE1253,
    MM_SBTL_FONT_ENC_CODEPAGE1254,
    MM_SBTL_FONT_ENC_CODEPAGE1255,
    MM_SBTL_FONT_ENC_CODEPAGE1256,
    MM_SBTL_FONT_ENC_CODEPAGE1257,
    MM_SBTL_FONT_ENC_CODEPAGE1258,
    MM_SBTL_FONT_ENC_ISO8859_2,
    MM_SBTL_FONT_ENC_ISO8859_5,
    MM_SBTL_FONT_ENC_ISO8859_7,
    MM_SBTL_FONT_ENC_ISO8859_8,
    MM_SBTL_FONT_ENC_ISO8859_9,
    MM_SBTL_FONT_ENC_ISO8859_16,
    MM_SBTL_FONT_ENC_KOI8_R
} MM_SBTL_FONT_ENC_T;

typedef struct _SVC_MM_SBTL_FONT_ENC_T
{
    MM_SBTL_FONT_ENC_T           e_enc_type;
    UINT16                       ui2_param;
} MM_SVC_SBTL_FONT_ENC_T;

/* define a enum type to set border type of each sentence of lyric */
typedef enum
{
    MM_SBTL_BDR_TYPE_NULL = 0,
    MM_SBTL_BDR_TYPE_SOLID_LINE
} MM_SBTL_BDR_TYPE_T;

/* define a enum type to set roll type for multiline display mode */
typedef enum
{
    MM_SBTL_ROLL_TYPE_DEF = 0
} MM_SBTL_ROLL_TYPE_T;

/* set more than one attributes at a time with MM_SBTL_ATTR_T type */
typedef UINT16 MM_SBTL_ATTR_T;

#define MM_SBTL_ATTR_FNT_INFO             ((MM_SBTL_ATTR_T) (1 << 0))
#define MM_SBTL_ATTR_BKG_CLR              ((MM_SBTL_ATTR_T) (1 << 1))
#define MM_SBTL_ATTR_TXT_CLR              ((MM_SBTL_ATTR_T) (1 << 2))
#define MM_SBTL_ATTR_BDR_TYPE             ((MM_SBTL_ATTR_T) (1 << 3))
#define MM_SBTL_ATTR_BDR_WIDTH            ((MM_SBTL_ATTR_T) (1 << 4))
#define MM_SBTL_ATTR_ROLL_TYPE            ((MM_SBTL_ATTR_T) (1 << 5))
#define MM_SBTL_ATTR_DISP_RECT            ((MM_SBTL_ATTR_T) (1 << 6))
#define MM_SBTL_ATTR_OSD_OFFSET           ((MM_SBTL_ATTR_T) (1 << 7))
#define MM_SBTL_ATTR_EDG_CLR              ((MM_SBTL_ATTR_T) (1 << 8))
#define MM_SBTL_ATTR_FG_HLT_CLR           ((MM_SBTL_ATTR_T) (1 << 9))
#define MM_SBTL_ATTR_BG_HLT_CLR           ((MM_SBTL_ATTR_T) (1 << 10))


typedef struct _SVC_MM_SBTL_ATTR_T
{
    MM_SBTL_ATTR_T            t_attr_field;
    WGL_FONT_INFO_T           t_font_info;
    GL_COLOR_T                t_bkg_clr;
    GL_COLOR_T                t_txt_clr;
    GL_COLOR_T                t_edge_clr;
    MM_SBTL_BDR_TYPE_T        e_bdr_type;
    UINT16                    ui4_bdr_width;
    MM_SBTL_ROLL_TYPE_T       e_roll_type;
    GL_POINT_T                t_osd_offset;
    GL_RECT_T                 t_disp_rect;
	GL_COLOR_T                t_fg_hlt_clr;
	GL_COLOR_T                t_bg_hlt_clr;
} MM_SVC_SBTL_DISP_ATTR_T;

typedef struct
{
    UINT8 ui1_idx;
    BOOL  b_enable;
}MM_SVC_CC_IDX_INFO_T;


/* set subtitle to be show(TRUE) or hide(FALSE) */
typedef struct _SVC_MM_SBTL_SHOW_HIDE_T
{
    BOOL                      b_sbtl_show;
} MM_SVC_SBTL_SHOW_HIDE_T;

/* set subtitle information */
typedef struct _SVC_MM_SBTL_LYRIC_INFO_T
{
    WGL_FONT_INFO_T           t_font;
    MM_SBTL_DISP_PAGE_T       t_page;
    MM_SBTL_TM_OFST_INF       t_tm_ofst;
    WGL_COLOR_INFO_T          t_bg_clr;
    WGL_COLOR_INFO_T          t_fg_clr;
    WGL_COLOR_INFO_T          t_edg_clr;
    GL_RECT_T                 t_rect;
    GL_POINT_T                t_osd_offset;
    MM_SBTL_ENCODING_T        e_enc;
    BOOL                      b_hide;
    BOOL                      b_has_lyric;
    BOOL                      b_is_font_bg_clr_set;
    WGL_COLOR_INFO_T          t_font_bg_color;
    SM_CCH_DTVCC_ATTRIBS_INFO_T      t_sbtl_cc_info;    /*for ipcc feature*/
	WGL_INSET_T               t_bdr_inset;
	MM_SBTL_ATTR_T            t_attr_field;
}SVC_MM_SBTL_LYRIC_INFO_T;
#endif


/******************** Multimedia Repeat Type *********************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_REPEAT_TYPE_T
 *  @brief Repeat type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_REPEAT_ALL = 1,
 *      MM_SVC_REPEAT_CHAPTER,
 *      MM_SVC_REPEAT_TITLE,
 *      MM_SVC_REPEAT_AB,
 *      MM_SVC_REPEAT_OFF
 *  } MM_SVC_REPEAT_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_REPEAT_ALL                                 - All.
 *  @li@c  MM_SVC_REPEAT_CHAPTER                             - Chapter.
 *  @li@c  MM_SVC_REPEAT_TITLE                               - Title.
 *  @li@c  MM_SVC_REPEAT_AB                                  - A to B.
 *  @li@c  MM_SVC_REPEAT_OFF                                 - Off.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_REPEAT_ALL = 1,/*file or channel*/
    MM_SVC_REPEAT_CHAPTER,
    MM_SVC_REPEAT_TITLE,
    MM_SVC_REPEAT_AB,
    MM_SVC_REPEAT_OFF
} MM_SVC_REPEAT_TYPE_T;

/********************* Multimedia Duration Type ******************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_DUR_TYPE_T
 *  @brief Duration type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_DUR_TYPE_TITLE = 1,
 *      MM_SVC_DUR_TYPE_CHAPTER,
 *      MM_SVC_DUR_TYPE_ALL,
 *      MM_SVC_DUR_TYPE_TITLE_OFFSET,
 *      MM_SVC_DUR_TYPE_CHAPTER_OFFSET,
 *      MM_SVC_DUR_TYPE_ALL_OFFSET,
 *      MM_SVC_DUR_TYPE_BUFFERED
 *  } MM_SVC_DUR_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_DUR_TYPE_TITLE                             - Title.
 *  @li@c  MM_SVC_DUR_TYPE_CHAPTER                           - Chapter.
 *  @li@c  MM_SVC_DUR_TYPE_ALL                               - All.
 *  @li@c  MM_SVC_DUR_TYPE_TITLE_OFFSET                      - Title offset.
 *  @li@c  MM_SVC_DUR_TYPE_CHAPTER_OFFSET                    - Chapter offset.
 *  @li@c  MM_SVC_DUR_TYPE_ALL_OFFSET                        - All offset.
 *  @li@c  MM_SVC_DUR_TYPE_BUFFERED                          - Buffered.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_DUR_TYPE_TITLE = 1,
    MM_SVC_DUR_TYPE_CHAPTER,
    MM_SVC_DUR_TYPE_ALL, /*file or channel*/
    MM_SVC_DUR_TYPE_TITLE_OFFSET,
    MM_SVC_DUR_TYPE_CHAPTER_OFFSET,
    MM_SVC_DUR_TYPE_ALL_OFFSET,
    MM_SVC_DUR_TYPE_BUFFERED
} MM_SVC_DUR_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum MM_SVC_TM_CODE_TYPE_T
 *  @brief Time code type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_TM_CODE_TYPE_TITLE = 1,
 *      MM_SVC_TM_CODE_TYPE_CHAPTER,
 *      MM_SVC_TM_CODE_TYPE_ALL
 *  } MM_SVC_TM_CODE_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_TM_CODE_TYPE_TITLE                         - Title.
 *  @li@c  MM_SVC_TM_CODE_TYPE_CHAPTER                       - Chapter.
 *  @li@c  MM_SVC_TM_CODE_TYPE_ALL                           - All.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_TM_CODE_TYPE_TITLE = 1,
    MM_SVC_TM_CODE_TYPE_CHAPTER,
    MM_SVC_TM_CODE_TYPE_ALL /*file or channel*/
} MM_SVC_TM_CODE_TYPE_T;

/******************* Multimedia  Button Type **********************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_BTN_TYPE_T
 *  @brief Button type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_BTN_TYPE_UP = 1,
 *      MM_SVC_BTN_TYPE_DOWN,
 *      MM_SVC_BTN_TYPE_LEFT,
 *      MM_SVC_BTN_TYPE_RIGHT,
 *      MM_SVC_BTN_TYPE_ENTER,
 *      MM_SVC_BTN_TYPE_STARTING_MENU,
 *      MM_SVC_BTN_TYPE_ROOT_MENU,
 *      MM_SVC_BTN_TYPE_TITLE_MENU,
 *      MM_SVC_BTN_TYPE_CHAPTER_MENU,
 *      MM_SVC_BTN_TYPE_MENU_ENDING_PLAYBACK,
 *      MM_SVC_BTN_TYPE_SELECT_STREAM
 *  } MM_SVC_BTN_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_BTN_TYPE_UP                                - Up.
 *  @li@c  MM_SVC_BTN_TYPE_DOWN                              - Down.
 *  @li@c  MM_SVC_BTN_TYPE_LEFT                              - Left.
 *  @li@c  MM_SVC_BTN_TYPE_RIGHT                             - Right.
 *  @li@c  MM_SVC_BTN_TYPE_ENTER                             - Enter.
 *  @li@c  MM_SVC_BTN_TYPE_STARTING_MENU                     - Start Menu.
 *  @li@c  MM_SVC_BTN_TYPE_ROOT_MENU                         - Root Menu.
 *  @li@c  MM_SVC_BTN_TYPE_TITLE_MENU                        - Title Menu.
 *  @li@c  MM_SVC_BTN_TYPE_CHAPTER_MENU                      - Chapter Menu.
 *  @li@c  MM_SVC_BTN_TYPE_MENU_ENDING_PLAYBACK              - Menu ending play.
 *  @li@c  MM_SVC_BTN_TYPE_SELECT_STREAM                     - Select stream.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_BTN_TYPE_UP = 1,
    MM_SVC_BTN_TYPE_DOWN,
    MM_SVC_BTN_TYPE_LEFT,
    MM_SVC_BTN_TYPE_RIGHT,
    MM_SVC_BTN_TYPE_ENTER,
    MM_SVC_BTN_TYPE_STARTING_MENU, /*return back the first present menu "WMEN" */
    MM_SVC_BTN_TYPE_ROOT_MENU,     /*return back the root menu "WMEN" */
    MM_SVC_BTN_TYPE_TITLE_MENU, /*not define yet for divx*/
    MM_SVC_BTN_TYPE_CHAPTER_MENU,
    MM_SVC_BTN_TYPE_MENU_ENDING_PLAYBACK,
    MM_SVC_BTN_TYPE_SELECT_STREAM, /*return tex*/
    MM_SVC_BTN_TYPE_SELECT_DIGTS,
    MM_SVC_BTN_TYPE_SELECT_NEXT,
    MM_SVC_BTN_TYPE_SELECT_PREV
} MM_SVC_BTN_TYPE_T;



/******************* Multimedia ZOOM Type **********************************/

/*typedef enum
{

} MM_SVC_ZOOM_TYPE_T;
*/


/********************* Video Info *********************************/
/*------------------------------------------------------------------*/
/*! @struct MM_VIDEO_INFO_T
 *  @brief Video information.
 *  @code
 *  typedef struct _MM_VIDEO_INFO_T
 *  {
 *     UINT32               ui4_width;
 *     UINT32               ui4_height;
 *  } MM_VIDEO_INFO_T;
 *  @endcode
 *  @li@c  ui4_width                                         - Width.
 *  @li@c  ui4_height                                        - Height.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_VIDEO_INFO_T
{
   UINT32               ui4_width;
   UINT32               ui4_height;    

   UINT8                ui1_par_w;
   UINT8                ui1_par_h;
   BOOL                 b_src_asp;
} MM_VIDEO_INFO_T;

typedef struct _MM_PUSH_BUF_INFO_T
{
   UINT8                ui1_id;
   UINT32               ui4_buf_size;
   UINT8*               pu1_push_buf;
} MM_PUSH_BUF_INFO_T;


#if 1//def DIVX_PLUS_CER
typedef struct _MM_SVC_CUR_CHAP_ID_T
{
    UINT32                  ui4_title_idx;
    UINT32                  ui4_playlist_id;
    UINT32                  ui4_chapter_id;
}MM_SVC_CUR_CHAP_ID_T;
#endif
/********************** Multimedia Duration Info *****************************
 * t_dur_type :
 *      represent the duration type.
 * ui4_tt_ch_num :
 *      represent the CH,TT,number,if the
 *      t_srh_type is ALL(Channle or File),the value should be zero.
 * ui2_pos_val_2 :
 *      retrieve the total time duration value in seconds.
 ****************************************************************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_DUR_INFO_T
 *  @brief Service Duration information.
 *  @code
 *  typedef struct _MM_SVC_DUR_INFO_T
 *  {
 *     MM_SVC_DUR_TYPE_T    t_dur_type;
 *     UINT16               ui2_idx;
 *     UINT32               ui4_duration;
 *  } MM_SVC_DUR_INFO_T;
 *  @endcode
 *  @li@c  t_dur_type                                        - Duration type.
 *  @li@c  ui2_idx                                           - Index.
 *  @li@c  ui4_duration                                      - Duration.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_DUR_INFO_T
{
   MM_SVC_DUR_TYPE_T    t_dur_type;    /*(I)*/
   UINT16               ui2_idx;       /*(I)*/
   UINT64               ui8_duration;  /*(O)*/
} MM_SVC_DUR_INFO_T;


/********************** Multimedia Timecode Info ******************************
 *t_tm_code_type:
 *      represent the time code type
 *ui2_idx:
 *      represent the CH,TT number , if the t_tm_code_type is
 *      ALL(Channel or File),the value should be zero
 *pui4_time
 *      retrieve the time offest.
 ******************************************************************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_TIME_CODE_INFO_T
 *  @brief Service time code information.
 *  @code
 *  typedef struct _MM_SVC_TIME_CODE_INFO_T
 *  {
 *     MM_SVC_TM_CODE_TYPE_T   t_tm_code_type;
 *     UINT16                  ui2_idx;
 *     UINT64                  ui8_time;
 *  } MM_SVC_TIME_CODE_INFO_T;
 *  @endcode
 *  @li@c  t_tm_code_type                                    - Time code type.
 *  @li@c  ui2_idx                                           - Index.
 *  @li@c  ui8_time                                          - Time.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_TIME_CODE_INFO_T
{
   MM_SVC_TM_CODE_TYPE_T   t_tm_code_type;   /*(I)*/
   UINT16                  ui2_idx;          /*(I)*/
   UINT64                  ui8_time;         /*(O)*/
} MM_SVC_TIME_CODE_INFO_T;

/********************** Multimedia PTS Info ******************************
 *ui8_aud_pts:
 *      aud pts value
 *ui8_vid_pts:
 *      video pts value
 ******************************************************************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_PTS_INFO_T
 *  @brief Service pts information.
 *  @code
 *  typedef struct _MM_SVC_PTS_INFO_T
 *  {
 *     UINT64                  ui8_aud_pts;        
 *     UINT64                  ui8_vid_pts;       
 *  } MM_SVC_PTS_INFO_T;
 *  @endcode
 *  @li@c  ui8_aud_pts                                    - audio pts.
 *  @li@c  ui8_vid_pts                                     - video pts.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_PTS_INFO_T
{
   UINT64                  ui8_aud_pts;         /*(O)*/
   UINT64                  ui8_vid_pts;         /*(O)*/
} MM_SVC_PTS_INFO_T;

/* ****************** Multimedia Repeat Info *********************************/
#if MM_AB_REPEAT_SUPPORT
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_REPEAT_AB_INFO
 *  @brief AB repeat setting type infomation.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_REPEAT_AB_INFO_NONE = 0,
 *      MM_SVC_REPEAT_AB_INFO_SET_A,
 *      MM_SVC_REPEAT_AB_INFO_SET_B,
 *      MM_SVC_REPEAT_AB_INFO_CANCEL_ALL,
 *  } MM_SVC_REPEAT_AB_INFO;
 *  @endcode
 *  @li@c  MM_SVC_REPEAT_AB_INFO_NONE                                - None.
 *  @li@c  MM_SVC_REPEAT_AB_INFO_SET_A                              - Set a point.
 *  @li@c  MM_SVC_REPEAT_AB_INFO_SET_B                              - Set b point.
 *  @li@c  MM_SVC_REPEAT_AB_INFO_CANCEL_ALL                     - Cancel all point.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_REPEAT_AB_TYPE_NONE = 0,
    MM_SVC_REPEAT_AB_TYPE_SET_A,
    MM_SVC_REPEAT_AB_TYPE_SET_B,
    MM_SVC_REPEAT_AB_TYPE_CANCEL_ALL
} MM_SVC_REPEAT_AB_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct MM_REPEAT_AB_INFO_T;
 *  @brief Repeat a/b position infomation.
 *  @code
 *  typedef struct _MM_REPEAT_AB_INFO_T
 *  {
 *      BOOL                            b_set_repeat_a;
 *      BOOL                            b_set_repeat_b;
 *      MM_SVC_POS_INFO_T    t_repeat_info_a;
 *      MM_SVC_POS_INFO_T    t_repeat_info_b;
 *  } MM_REPEAT_AB_INFO_T;
 *  @endcode
 *  @li@c  b_set_repeat_a                                     - If it is A set.
 *  @li@c  b_set_repeat_a                                     - If it is B set.
 *  @li@c  t_repeat_info_a                                     - Repeat a position.
 *  @li@c  t_repeat_info_b                                     - Repeat b position.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_REPEAT_AB_INFO_T
{
    BOOL                 b_set_repeat_a;
    BOOL                 b_set_repeat_b;
    BOOL                 b_seeking;
    MM_SVC_POS_INFO_T    t_repeat_info_a;
    MM_SVC_POS_INFO_T    t_repeat_info_b;
} MM_REPEAT_AB_INFO_T;

#endif

/*------------------------------------------------------------------*/
/*! @struct MM_SVC_REPEAT_INFO_T;
 *  @brief Repeat type.
 *  @code
 *  typedef struct _MM_SVC_REPEAT_INFO_T
 *  {
 *      MM_SVC_REPEAT_TYPE_T    t_repeat_type;
 *      union
 *      {
 *          MM_SVC_REPEAT_AB_TYPE_T   e_repeat_ab_type;
 *      } u;
 *  } MM_SVC_REPEAT_INFO_T;
 *  @endcode
 *  @li@c  t_repeat_type                                     - Type.
 *  @li@c  e_repeat_ab_type                               - Repeat ab sub-type.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_REPEAT_INFO_T
{
    MM_SVC_REPEAT_TYPE_T    t_repeat_type;  /*(I)*/
#if MM_AB_REPEAT_SUPPORT
    union
    {
        MM_SVC_REPEAT_AB_TYPE_T   e_repeat_ab_type;
    } u;
#endif
} MM_SVC_REPEAT_INFO_T;

typedef struct _MM_SVC_SUBTITLE_INFO_T
{
    union
    {
        BOOL                    b_is_extra_subtitle;
        MM_SBTL_ENCODING_T      t_mm_sbtl_encoding;
    } u;
} MM_SVC_SUBTITLE_INFO_T;

#define MAX_USER_SELECT_SBTL_LENGTH 1024

/*------------------------------------------------------------------*/
/*! @struct MM_SVC_SUBTITLE_INFO_USER_SET_T;
 *  @brief user set info for rpc
 *  @code
 *  typedef struct _MM_SVC_SUBTITLE_INFO_USER_SET_T
 *  {
 *    CHAR                       ps_full_path[256];
 *    CHAR                       ps_related_path[256];   
 *  } MM_SVC_SUBTITLE_INFO_USER_SET_T;
 *  @endcode
 *  @li@c  ps_full_path                                     - user set subtitle path.
 *  @li@c  ps_related_path                               - the related subtitle path
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_SUBTITLE_INFO_USER_SET_T
{
    CHAR                       ps_full_path[MAX_USER_SELECT_SBTL_LENGTH];
    CHAR                       ps_related_path[MAX_USER_SELECT_SBTL_LENGTH];
} MM_SVC_SUBTITLE_INFO_USER_SET_T;
/********************* Multimedia Trick Info **********************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_TRICK_INFO_T
 *  @brief Speed type.
 *  @code
 *  typedef struct _MM_SVC_TRICK_INFO_T
 *  {
 *      MM_SPEED_TYPE_T     e_speed_type;
 *  } MM_SVC_TRICK_INFO_T;
 *  @endcode
 *  @li@c  e_speed_type                                      - Speed type.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_TRICK_INFO_T
{
    MM_SPEED_TYPE_T     e_speed_type;      /*(I)*/
    union
    {
        BOOL         b_boolean;
        UINT32      ui4_data;
    } u;
} MM_SVC_TRICK_INFO_T;

/********************* Multimedia CHPATER Info **********************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_CHAPTER_INFO_T
 *  @brief Service chapter information.
 *  @code
 *  typedef struct _MM_SVC_CHAPTER_INFO_T
 *  {
 *      UINT16   ui2_ch_total_ns;
 *      UINT16   ui2_curr_ch_num;
 *  } MM_SVC_CHAPTER_INFO_T;
 *  @endcode
 *  @li@c  ui2_ch_total_ns                                   - Chapter total time in ns.
 *  @li@c  ui2_curr_ch_num                                   - Current chapter number.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_CHAPTER_INFO_T
{
    UINT16   ui2_ch_total_ns;
    UINT16   ui2_curr_ch_num;
} MM_SVC_CHAPTER_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MM_SVC_TITLE_INFO_T
 *  @brief Service title information.
 *  @code
 *  typedef struct _MM_SVC_TITLE_INFO_T
 *  {
 *      UINT16   ui2_tt_ns;
 *      UINT16   ui2_curr_tt_num;
 *  } MM_SVC_TITLE_INFO_T;
 *  @endcode
 *  @li@c  ui2_tt_ns                                         - Title number.
 *  @li@c  ui2_curr_tt_num                                   - Current title number.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_TITLE_INFO_T
{
    UINT16   ui2_tt_ns;
    UINT16   ui2_curr_tt_num;
} MM_SVC_TITLE_INFO_T;


/********************* Multimedia Button Info **********************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_BTN_INFO_T
 *  @brief Service button type.
 *  @code
 *  typedef struct _MM_SVC_BTN_INFO_T
 *  {
 *      MM_SVC_BTN_TYPE_T   t_btn_type;
  *      union
 *      {
 *          UINT32                    ui4_value;
 *          CHAR*                    ps_next;
 *      }u;
 *  } MM_SVC_BTN_INFO_T;
 *  @endcode
 *  @li@c  t_btn_type                                        - Button type.
  *  @li@c  ui4_value                                      - auto chapter value 0~9.
 *  @li@c  ps_next                                      - Reserved.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_BTN_INFO_T
{
    MM_SVC_BTN_TYPE_T   t_btn_type;      /*(I)*/
    union
    {
        UINT32                    ui4_value;  /*(I)*/
        CHAR*                     ps_next;    /*(I)*/
    }u;
} MM_SVC_BTN_INFO_T;


/********************* Btn Action Type ******************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_BTN_ACTION_TYPE_T
 *  @brief Btn Type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_BTN_ACTION_TYPE_SEEK = 1,
 *      MM_SVC_BTN_ACTION_TYPE_TEXT_DISPLAY,
 *  } MM_SVC_BTN_ACTION_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_BTN_ACTION_TYPE_SEEK                             - auto chapter select.
 *  @li@c  MM_SVC_BTN_ACTION_TYPE_TEXT_DISPLAY               - Reserved.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_BTN_ACTION_TYPE_SEEK = 1,
    MM_SVC_BTN_ACTION_TYPE_TEXT_DISPLAY
    /*to be extend*/
}MM_SVC_BTN_ACTION_TYPE_T;

/******************* Auto chapter Button Action Info**********************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_BTN_ACTION_INFO_T
 *  @brief Button Action Info.
 *  @code
 *  typedef struct _MM_SVC_BTN_INFO_T
 *  {
 *      MM_SVC_BTN_TYPE_T                t_btn_type;
 *      MM_SVC_BTN_ACTION_TYPE_T        t_btn_act_type;
 *      union
 *      {
 *          UINT32                    ui4_value;
 *          CHAR*                    ps_next;
 *      }u;
 *  }MM_SVC_BTN_INFO_T;
 *  @endcode
 *  @li@c  MM_SVC_BTN_TYPE_T                                    - BTN type.
 *  @li@c  MM_SVC_BTN_ACTION_TYPE_T                        -return BTN action type.
 *  @li@c  ui4_value                                              - auto chapter value 0~9.
 *  @li@c  ps_next                                              - Reserved.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_BTN_ACTION_INFO_T
{
    MM_SVC_BTN_INFO_T            t_btn_type;
    MM_SVC_BTN_ACTION_TYPE_T    t_btn_act_type;
    union
    {
        UINT32                    ui4_value;
        CHAR*                    ps_next;
    }u;
}MM_SVC_BTN_ACTION_INFO_T;

typedef struct _MM_SVC_TIME_TO_SIZE_T
{
    UINT32  ui4_ms; /*IN: time in miliseconds*/
    UINT32  ui4_size; /*OUT: size in bytes*/
} MM_SVC_TIME_TO_SIZE_T;

typedef struct _MM_SVC_BUF_STATUS_T
{
    UINT64  ui8_begin_pos;
    UINT64  ui8_end_pos;
    UINT64  ui8_data_ofst; /* Need minus header size*/
} MM_SVC_BUF_STATUS_T;


/* SVCTX_MM_GET_TYPE_TIMESHIFT_RANGE_INFO data info *************************/
/*------------------------------------------------------------------*/
/*! @enum MM_TIMESHIFT_RANGE_INFO_TYPE_T
 *  @brief Time shift range information type.
 *  @code
 *  typedef enum
 *  {
 *      MM_TIMESHIFT_RANGE_INFO_TYPE_TICK = 1,
 *  } MM_TIMESHIFT_RANGE_INFO_TYPE_T;
 *  @endcode
 *  @li@c  MM_TIMESHIFT_RANGE_INFO_TYPE_TICK                 - Type tick.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_TIMESHIFT_RANGE_INFO_TYPE_TICK = 1,
    /*MM_TIMESHIFT_RANGE_INFO_TYPE_LBA*/
} MM_TIMESHIFT_RANGE_INFO_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct MM_TIMESHIFT_RANGE_INFO_T
 *  @brief Time shift range information.
 *  @code
 *  typedef struct _MM_TIMESHIFT_RANGE_INFO_T
 *  {
 *     MM_TIMESHIFT_RANGE_INFO_TYPE_T   e_type;
 *     UINT32                           ui4_start;
 *     UINT32                           ui4_end;
 *     UINT32                           ui4_current;
 *     BOOL                             b_bad_tick;
 *     TIME_T                           t_curr_sys_time;
 *  } MM_TIMESHIFT_RANGE_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            - Type.
 *  @li@c  ui4_start                                         - Start tick.
 *  @li@c  ui4_end                                           - End tick.
 *  @li@c  ui4_current                                       - Current tick.
 *  @li@c  b_bad_tick                                        - Is bad tick.
 *  @li@c  t_curr_sys_time                                   - Current system time.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_TIMESHIFT_RANGE_INFO_T
{
   MM_TIMESHIFT_RANGE_INFO_TYPE_T   e_type;            /*(I)*/
   UINT32                           ui4_start;         /*(O) in tick index*/
   UINT32                           ui4_end;           /*(O) in tick index*/
   UINT32                           ui4_current;       /*(O) in tick index*/
   BOOL                             b_bad_tick;        /*(O)*/
   TIME_T                           t_curr_sys_time;   /*(O)*/
} MM_TIMESHIFT_RANGE_INFO_T;


/********************* Multimedia Specific Stream Configuraton ****************/

/*------------------------------------------------------------------*/
/*! @struct MM_STRM_ATRBT_T
 *  @brief Stream attribute.
 *
 *  @code
 *  typedef struct _MM_STRM_ATRBT_T
 *  {
 *      STREAM_TYPE_T                       e_type;
 *      MINFO_TYPE_STM_ATRBT_T              t_stm_atrbt;
 *  } MM_STRM_ATRBT_T;
 *  @endcode
 *
 *  @li@c  e_type                                          - Type.
 *  @li@c  t_stm_atrbt                                     - Attribute information.
 */
/*------------------------------------------------------------------*/

typedef struct _MM_STRM_ATRBT_T
{
    STREAM_TYPE_T                       e_type;
    MINFO_TYPE_STM_ATRBT_T              t_stm_atrbt;    
    BOOL                                b_crypto;
    MM_CRYPT_INFO_T*                    pt_crypt_info;
} MM_STRM_ATRBT_T;

/*------------------------------------------------------------------*/
/*! @struct MM_STRM_INFO_T
 *  @brief MM stream information.
 *
 *  @code
 *  typedef struct _MM_STRM_INFO_T
 *  {
 *      MEDIA_FORMAT_T                      t_format;
 *      UINT16                              ui2_num_stm;
 *      MM_STRM_ATRBT_T*                    pt_stm_atrbt;
 *  } MM_STRM_INFO_T;
 *  @endcode
 *
 *  @li@c  t_format                                          - Media format.
 *  @li@c  ui2_num_stm                                       - Number of stream.
 *  @li@c  pt_stm_atrbt                                      - Stream attribute.
 */
/*------------------------------------------------------------------*/

#define SUPPORTED_STRM_NUM_MAX          (MM_MAX_VIDEO_STRM_NUM+MM_MAX_AUDIO_STRM_NUM+MM_MAX_SUBTITLE_STRM_NUM) /*1 Video + 8 Audio + 20 Subtitle*/
typedef struct _MM_STRM_INFO_T
{
    MEDIA_FORMAT_T                      t_format;
    /* we can not support more than one video or audio or subtilte stream */
    UINT16                              ui2_num_stm;
    MM_STRM_ATRBT_T                     at_stm_atrbt[SUPPORTED_STRM_NUM_MAX];
    BOOL                                b_pcr;  
    UINT16                              ui2_pcr_pid;
} MM_STRM_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MM_3D_VID_INFO_T
 *  @brief 3D vid stream information.
 *
 *  @code
 *  typedef struct _MM_3D_VID_INFO_T
 *  {
 *      MM_SRC_TAG3D_TYPE_T                e_tag3d_type;
 *      
 *  } MM_STRM_INFO_T;
 *  @endcode
 *
 *  @li@c  MM_SRC_TAG3D_TYPE_T                e_tag3d_type.
 *
 */
/*------------------------------------------------------------------*/
typedef struct _MM_3D_VID_INFO_T
{
    MM_SRC_TAG3D_TYPE_T                e_tag3d_type;
} MM_3D_VID_INFO_T;

typedef struct _MM_STRM_CODEC_INFO_T
{
    UINT8       ui1_strm_idx;
    UINT32      ui4_codec_info_size;
    UINT8       aui1_codec_info[2048];
}MM_STRM_CODEC_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MM_TS_STRM_COND_T
 *  @brief Current playing audio and video stream cond of TS file.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_TS_STRM_UNKNOWN,
    MM_TS_STRM_VIDEO_STARTED_AUDIO_STARTED,
    MM_TS_STRM_VIDEO_STARTED_AUDIO_NOT_STARTED,
    MM_TS_STRM_VIDEO_NOT_STARTED_AUDIO_STARTED
}MM_TS_STRM_COND_T;

typedef struct _BUF_AGT_FLOW_CTRL_T
{    
    UINT32  ui4_buffer_len;             /*Buffer size in Bytes. */    
    UINT32  ui4_keep_buffer_threshold;  /*Keep threshold in Bytes, at least keep this size of data which can used by seek backward. */    
    UINT32  ui4_re_buferf_threshold;    /*Re-Buffer threshold in Bytes, at least this size of space before pulling data from Application */
} MM_BUF_AGT_FLOW_CTRL_T;

typedef struct _MM_SVC_STOP_INFO
{
    UINT64              ui8_clip_id;
    UINT32              ui4_title_idx;
    UINT32              ui4_playlist_idx;
    UINT32              ui4_chap_idx;
    UINT8               ui2_aud_idx;
    UINT8               ui2_sub_idx;
    MM_SVC_POS_INFO_T   t_mm_svc_pos_info;
} MM_SVC_STOP_INFO;    

typedef enum 
{
    ID_CNDTN_UNKNOWN,
    ID_CNDTN_SRC,
    ID_CNDTN_END
}ID_CNDTN_TYPE;

typedef struct _ID_CVT_CNDTN_T
{
    ID_CNDTN_TYPE e_cndtn_type;
    union
    {
        UINT8 t_src_val;
    }u;
}ID_CVT_CNDTN_T;

#define MAX_SERVER_INFO_LENGTH 128
typedef struct 
{
    char     ac_server_name[MAX_SERVER_INFO_LENGTH];
    char     ac_content_type[MAX_SERVER_INFO_LENGTH];
} MM_PB_CTRL_HTTP_SERVER_INFO_T;

typedef struct
{
    MINFO_INFO_AUDIO_ENC_T      e_aud_fmt;
    MINFO_INFO_AUD_CHANNELS_T   e_channels;
    UINT32                      ui4_sample_rate;
    UINT32                      ui4_data_rate;
    UINT8                       ui1_bit_depth;
} MM_AUDIO_INFO_T;

typedef enum
{
    DEBUG_LOG_LEVEL,
    DEBUG_QUERY
}DEBUG_TYPE;

typedef INT32 (*cmpb_debug_mnt_cb)(UINT32,DEBUG_TYPE,UINT32);

typedef struct 
{
    cmpb_debug_mnt_cb     debug_cb;
    void*                 pv_tag;
}CMPB_DEBUG_INFO;

typedef struct _CMPB_DEBUG_INFO_T
{
    CMPB_DEBUG_INFO   t_info;
    BOOL              b_valid;
    
}CMPB_DEBUG_INFO_T;

/*add code:for plug-in subtitle*/

#define MM_DASH_SBTL_URL_MAX_LEN    (1024) /*this size should algin with dash cmpb url max length*/

typedef enum
{
    MM_DASH_SBTL_ATTR_TYPE_HIDE = 0,
    MM_DASH_SBTL_ATTR_TYPE_SHOW,
    MM_DASH_SBTL_ATTR_TYPE_FORCED
}MM_DASH_SBTL_ATTR_TYPE_T;

typedef struct _MM_DASH_SBTL_INFO_T
{
    UINT64                                  ui8_start_time;                                 /*the start time*/
    UINT64                                  ui8_end_time;                                   /*the start time*/
    UINT8                                   pu1_head_url[MM_DASH_SBTL_URL_MAX_LEN];         /*head url*/
    UINT8                                   pu1_data_url[MM_DASH_SBTL_URL_MAX_LEN];                                   /*data url*/
    MM_DASH_SBTL_ATTR_TYPE_T                e_sbtl_attr;                                    /*attribute*/
    BOOL                                    b_first;                                        /*the first flag for first play and change track/seek etc*/
    UINT32                                  ui4_ver;                                        /*message version for get&set in different thread*/
	uint8_t                                 pui1_entry_key[MM_DASH_SBTL_URL_MAX_LEN];     /*entry key for finde entry in mpd*/
	
}MM_DASH_SBTL_INFO_T;

typedef struct
{
	UINT8* pui1_pes_data;
	UINT32 ui4_pes_len;
}MM_SP_CB_PES_DATA;

typedef enum
{
	MM_SP_DVB_PES = 0,
}MM_SP_CB_DATA_TYPE;

typedef INT32 (*MM_SP_CB)(
	        HANDLE_T           h_handle,
        	MM_SP_CB_DATA_TYPE e_cb_type,
        	VOID*              pv_cb_data,
        	VOID*              pv_user_arg);

typedef struct
{
	MM_SP_CB pf_cb;
	HANDLE_T h_handle;
	VOID*    pv_phy;
	UINT32   ui4_len;
	VOID*    pv_tag;
}MM_SP_CB_INFO;

typedef struct
{
    UINT16                  ui2_pid;
    MINFO_INFO_VIDEO_ENC_T  e_vid_codec;
}MM_RM_RES_NUMBER_INFO;

/*end add code*/
#endif /* _U_MM_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW_SVCTX_MM_HDLR*/
/*----------------------------------------------------------------------------*/


