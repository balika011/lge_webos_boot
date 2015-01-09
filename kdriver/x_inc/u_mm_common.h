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
/*! @file u_mm_common.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *    This header file contains Device Type definitions, which are exported
 *    to other Middleware components and applications.
 *         
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_MM_COMMON_H_
#define _U_MM_COMMON_H_

#ifndef __KERNEL__
 
#include "u_common.h"

#else

#include "u_common.h"

#endif

/********* Mutimedia Format Definition ***********************/
/*----------------------------------------------------------------------------*/
/*! @enum   MEDIA_SVC_TYPE_T 
 *  @brief  Media service type
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_SVC_TYPE_UNKNOWN = 0,
 *      MEDIA_SVC_TYPE_AUDIO_ONLY,
 *      MEDIA_SVC_TYPE_VIDEO_ONLY
 *  }   MEDIA_SVC_TYPE_T; 
 *  @endcode
 *   
 *  @li@c MEDIA_SVC_TYPE_UNKNOWN                      - Unknown service type
 *  @li@c MEDIA_SVC_TYPE_AUDIO_ONLY                  - Audio service 
 *  @li@c MEDIA_SVC_TYPE_VIDEO_ONLY                  - Video service 
 */
/*----------------------------------------------------------------------------*/

typedef enum
{
    MEDIA_SVC_TYPE_UNKNOWN = 0,
    MEDIA_SVC_TYPE_AUDIO_ONLY,
    MEDIA_SVC_TYPE_VIDEO_ONLY
} MEDIA_SVC_TYPE_T;

/********* Mutimedia Format Definition ***********************/
/*----------------------------------------------------------------------------*/
/*! @enum   MEDIA_TYPE_T 
 *  @brief  Media type
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_TYPE_UNKNOWN = 0,
 *      MEDIA_TYPE_AUDIO_ES,
 *      MEDIA_TYPE_VIDEO_ES,
 *      MEDIA_TYPE_IMAGE,
 *      MEDIA_TYPE_CONTAINER,
 *      MEDIA_TYPE_PS,
 *      MEDIA_TYPE_TS
 *  }   MEDIA_TYPE_T; 
 *  @endcode
 *   
 *  @li@c MEDIA_TYPE_UNKNOWN               - Unknown media type
 *  @li@c MEDIA_TYPE_AUDIO_ES               - Audio es  
 *  @li@c MEDIA_TYPE_VIDEO_ES               - Video es 
 *  @li@c MEDIA_TYPE_IMAGE                    - image type
 *  @li@c MEDIA_TYPE_CONTAINER             - container type
 *  @li@c MEDIA_TYPE_PS                           - PS
 *  @li@c MEDIA_TYPE_TS                           - TS
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MEDIA_TYPE_UNKNOWN = 0,
    MEDIA_TYPE_AUDIO_ES,
    MEDIA_TYPE_VIDEO_ES,
    MEDIA_TYPE_IMAGE,
    MEDIA_TYPE_CONTAINER,
    MEDIA_TYPE_PS,
    MEDIA_TYPE_TS
} MEDIA_TYPE_T;

/* This structure enumerates all audio subtype types for media format. */
/*----------------------------------------------------------------------------*/
/*! @enum   MEDIA_AUD_SUBTYPE_T 
 *  @brief  Audio subtype
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_AUD_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_AUD_SUBTYPE_MP2,
 *      MEDIA_AUD_SUBTYPE_MP3,
 *      MEDIA_AUD_SUBTYPE_WMA,
 *      MEDIA_AUD_SUBTYPE_LPCM,
 *      MEDIA_AUD_SUBTYPE_AAC,
 *      MEDIA_AUD_SUBTYPE_WAVE,
 *      MEDIA_AUD_SUBTYPE_AIF,
 *      MEDIA_AUD_SUBTYPE_AC3
 *  }   MEDIA_AUD_SUBTYPE_T; 
 *  @endcode
 *   
 *  @li@c MEDIA_AUD_SUBTYPE_UNKNOWN          - Unknown audio subtype
 *  @li@c MEDIA_AUD_SUBTYPE_MP2                   - MP2 
 *  @li@c MEDIA_AUD_SUBTYPE_MP3                   - MP3 
 *  @li@c MEDIA_AUD_SUBTYPE_WMA                  - WMA 
 *  @li@c MEDIA_AUD_SUBTYPE_LPCM                 - LPCM 
 *  @li@c MEDIA_AUD_SUBTYPE_AAC                  - AAC
 *  @li@c MEDIA_AUD_SUBTYPE_WAVE                 - WAVE
 *  @li@c MEDIA_AUD_SUBTYPE_AIF                     - AIF 
 *  @li@c MEDIA_AUD_SUBTYPE_AC3                    - AC3
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MEDIA_AUD_SUBTYPE_UNKNOWN = 0,
    MEDIA_AUD_SUBTYPE_MP2,
    MEDIA_AUD_SUBTYPE_MP3,
    MEDIA_AUD_SUBTYPE_WMA,
    MEDIA_AUD_SUBTYPE_LPCM,
    MEDIA_AUD_SUBTYPE_AAC,
    MEDIA_AUD_SUBTYPE_WAVE,  /*add for wave support*/
    MEDIA_AUD_SUBTYPE_AIF,
    MEDIA_AUD_SUBTYPE_AC3,
    MEDIA_AUD_SUBTYPE_DTS,
    MEDIA_AUD_SUBTYPE_AMR,
    MEDIA_AUD_SUBTYPE_AWB,
    MEDIA_AUD_SUBTYPE_FLAC,  //paul_flac
    MEDIA_AUD_SUBTYPE_OGG,
    MEDIA_AUD_SUBTYPE_APE
/*    MEDIA_AUD_SUBTYPE_ES ???*/
} MEDIA_AUD_SUBTYPE_T;

/* This structure enumerates all audio subtype types for media format. */

/*----------------------------------------------------------------------------*/
/*! @enum   MEDIA_IMG_SUBTYPE_T 
 *  @brief  image subtype
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_IMG_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_IMG_SUBTYPE_PNG,
 *      MEDIA_IMG_SUBTYPE_JPG
 *  }   MEDIA_IMG_SUBTYPE_T; 
 *  @endcode
 *   
 *  @li@c MEDIA_IMG_SUBTYPE_UNKNOWN          - Unknown image subtype
 *  @li@c MEDIA_IMG_SUBTYPE_PNG                   - PNG 
 *  @li@c MEDIA_IMG_SUBTYPE_JPG                    - JPG  
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MEDIA_IMG_SUBTYPE_UNKNOWN = 0,
    MEDIA_IMG_SUBTYPE_PNG,
    MEDIA_IMG_SUBTYPE_JPG
} MEDIA_IMG_SUBTYPE_T;

/* This structure enumerates all video  subtype types for media format. */

/*----------------------------------------------------------------------------*/
/*! @enum   MEDIA_VID_SUBTYPE_T 
 *  @brief  Video subtype
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_VID_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_VID_SUBTYPE_M1V,
 *      MEDIA_VID_SUBTYPE_M2V,
 *      MEDIA_VID_SUBTYPE_M4V,
 *      MEDIA_VID_SUBTYPE_H264,
 *      MEDIA_VID_SUBTYPE_VC1
 *  }   MEDIA_VID_SUBTYPE_T; 
 *  @endcode
 *   
 *  @li@c MEDIA_VID_SUBTYPE_UNKNOWN          - Unknown video subtype
 *  @li@c MEDIA_VID_SUBTYPE_M1V                   - M1V 
 *  @li@c MEDIA_VID_SUBTYPE_M2V                   - M2V 
 *  @li@c MEDIA_VID_SUBTYPE_M4V                  - M4V 
 *  @li@c MEDIA_VID_SUBTYPE_H264                 - H264 
 *  @li@c MEDIA_VID_SUBTYPE_VC1                  - VC1
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MEDIA_VID_SUBTYPE_UNKNOWN = 0,
    MEDIA_VID_SUBTYPE_M1V,
    MEDIA_VID_SUBTYPE_M2V,
    MEDIA_VID_SUBTYPE_M4V,
    MEDIA_VID_SUBTYPE_H264,
    MEDIA_VID_SUBTYPE_VC1,
    MEDIA_VID_SUBTYPE_AVS,
    MEDIA_VID_SUBTYPE_H265
} MEDIA_VID_SUBTYPE_T;

/* This structure enumerates all audio subtype types for media format. */
/*----------------------------------------------------------------------------*/
/*! @enum   MEDIA_CONTNR_SUBTYPE_T 
 *  @brief  container subtype
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_CONTNR_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_CONTNR_SUBTYPE_AVI,
 *      MEDIA_CONTNR_SUBTYPE_ASF,
 *      MEDIA_CONTNR_SUBTYPE_MP4,
 *      MEDIA_CONTNR_SUBTYPE_MATROSKA,
 *      MEDIA_CONTNR_SUBTYPE_RM,
 *	MEDIA_CONTNR_SUBTYPE_FLV,
 *	MEDIA_CONTNR_SUBTYPE_CMPB,
 *	MEDIA_CONTNR_SUBTYPE_OGM
 *  }   MEDIA_CONTNR_SUBTYPE_T; 
 *  @endcode
 *   
 *  @li@c MEDIA_CONTNR_SUBTYPE_UNKNOWN          - Unknown subtype
 *  @li@c MEDIA_CONTNR_SUBTYPE_AVI                   - AVI 
 *  @li@c MEDIA_CONTNR_SUBTYPE_ASF                   - ASF 
 *  @li@c MEDIA_CONTNR_SUBTYPE_MP4                  - MP4 
 *  @li@c MEDIA_CONTNR_SUBTYPE_MATROSKA        - MKV 
 *  @li@c MEDIA_CONTNR_SUBTYPE_RM                   - RM
 *  @li@c MEDIA_CONTNR_SUBTYPE_FLV                  - FLV
 *  @li@c MEDIA_CONTNR_SUBTYPE_CMPB               - CMPB
 *  @li@c MEDIA_CONTNR_SUBTYPE_OGM                 - OGM
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MEDIA_CONTNR_SUBTYPE_UNKNOWN = 0,
    MEDIA_CONTNR_SUBTYPE_AVI,
    MEDIA_CONTNR_SUBTYPE_ASF,    
    MEDIA_CONTNR_SUBTYPE_MP4,
    MEDIA_CONTNR_SUBTYPE_MATROSKA,
    MEDIA_CONTNR_SUBTYPE_RM,
    MEDIA_CONTNR_SUBTYPE_FLV, /*add for flv*/
    MEDIA_CONTNR_SUBTYPE_CMPB,
    MEDIA_CONTNR_SUBTYPE_OGM  /*add for ogm*/
} MEDIA_CONTNR_SUBTYPE_T;

/* This structure enumerates all PS  subtype types for media format. */
/*----------------------------------------------------------------------------*/
/*! @enum   MEDIA_PS_SUBTYPE_T 
 *  @brief  PS subtype
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_PS_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_PS_SUBTYPE_MPG,
 *      MEDIA_PS_SUBTYPE_VOB,
 *      MEDIA_PS_SUBTYPE_DAT
 *  }   MEDIA_PS_SUBTYPE_T; 
 *  @endcode
 *   
 *  @li@c MEDIA_PS_SUBTYPE_UNKNOWN          - Unknown subtype
 *  @li@c MEDIA_PS_SUBTYPE_MPG                   - Mpeg  
 *  @li@c MEDIA_PS_SUBTYPE_VOB                   - VOB   
 *  @li@c MEDIA_PS_SUBTYPE_DAT                  - DAT 
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MEDIA_PS_SUBTYPE_UNKNOWN = 0,
    MEDIA_PS_SUBTYPE_MPG,
    MEDIA_PS_SUBTYPE_VOB,
    MEDIA_PS_SUBTYPE_DAT
    /*MEDIA_PS_SUBTYPE_CAM*/
} MEDIA_PS_SUBTYPE_T;

/* This structure enumerates all TS  subtype types for media format. */
/*----------------------------------------------------------------------------*/
/*! @enum   MEDIA_TS_SUBTYPE_T 
 *  @brief  TS subtype
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_TS_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_TS_SUBTYPE_TS_188,
 *      MEDIA_TS_SUBTYPE_TS_192,
 *      MEDIA_TS_SUBTYPE_TS_192_ZERO,
 *      MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION,
 *      MEDIA_TS_SUBTYPE_TS_TIME_SHIFT
 *  }   MEDIA_TS_SUBTYPE_T; 
 *  @endcode
 *   
 *  @li@c MEDIA_TS_SUBTYPE_UNKNOWN                       - Unknown subtype
 *  @li@c MEDIA_TS_SUBTYPE_TS_188                           - TS 188 
 *  @li@c MEDIA_TS_SUBTYPE_TS_192                           - TS 192 
 *  @li@c MEDIA_TS_SUBTYPE_TS_192_ZERO                  - TS 192 zero 
 *  @li@c MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION        - TS 192 encryption 
 *  @li@c MEDIA_TS_SUBTYPE_TS_TIME_SHIFT                - Time shift file
 *  @li@c MEDIA_TS_SUBTYPE_TS_204                   - TS 204
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MEDIA_TS_SUBTYPE_UNKNOWN = 0,
    MEDIA_TS_SUBTYPE_TS_188,
    MEDIA_TS_SUBTYPE_TS_192,
    MEDIA_TS_SUBTYPE_TS_192_SYSTEM_B,
    MEDIA_TS_SUBTYPE_TS_192_ZERO,
    MEDIA_TS_SUBTYPE_TS_130,
    MEDIA_TS_SUBTYPE_TS_134,
    MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION,
    MEDIA_TS_SUBTYPE_TS_TIME_SHIFT,
    MEDIA_TS_SUBTYPE_TS_TIME_SHIFT_CMPB,
    MEDIA_TS_SUBTYPE_TS_204,
    MEDIA_TS_SUBTYPE_TS_206                         /*MPEG_TS_JP_T*/
 } MEDIA_TS_SUBTYPE_T;

/* This structure contains the subtype of the media. */
/* This is a union structure to specify the subtype of the media and it could be 
   audio, video and so on. */

/*----------------------------------------------------------------------------*/
/*! @struct MEDIA_SUBTYPE_T 
 *  @brief  Media subtype structure
 *  @code
 *  typedef struct 
 * {
 *     union
 *    {  
 *        MEDIA_AUD_SUBTYPE_T         e_aud_subtype;
 *        MEDIA_VID_SUBTYPE_T         e_vid_subtype;
 *        MEDIA_IMG_SUBTYPE_T         e_img_subtype;
 *        MEDIA_CONTNR_SUBTYPE_T      e_contnr_subtype;
 *        MEDIA_PS_SUBTYPE_T          e_ps_subtype;
 *        MEDIA_TS_SUBTYPE_T          e_ts_subtype;
 *    }u; 
 *  } MEDIA_SUBTYPE_T;  
 *  @endcode
 *
 *  @li@c e_aud_subtype        - audio subtype
 *  @li@c e_vid_subtype         - video subtype
 *  @li@c e_img_subtype        - image subtype
 *  @li@c e_contnr_subtype    - container subtype
 *  @li@c e_ps_subtype          - PS subtype
 *  @li@c e_ts_subtype           - TS subtype
 */
/*----------------------------------------------------------------------------*/
typedef struct 
{
    union
    {  
         MEDIA_AUD_SUBTYPE_T         e_aud_subtype;
         MEDIA_VID_SUBTYPE_T         e_vid_subtype;
         MEDIA_IMG_SUBTYPE_T         e_img_subtype;
         MEDIA_CONTNR_SUBTYPE_T      e_contnr_subtype;
         MEDIA_PS_SUBTYPE_T          e_ps_subtype;
         MEDIA_TS_SUBTYPE_T          e_ts_subtype;
    }u; 
} MEDIA_SUBTYPE_T;  

/* This structure contains the format of the media. */

/*----------------------------------------------------------------------------*/
/*! @struct MEDIA_FORMAT_T 
 *  @brief  Media format structure
 *  @code
 *  typedef struct  _MEDIA_FORMAT_T
 * {
 *        MEDIA_TYPE_T            e_media_type;
 *        MEDIA_SUBTYPE_T         t_media_subtype; 
 *        MEDIA_SVC_TYPE_T        e_mm_svc_type;
 *  } MEDIA_FORMAT_T;  
 *  @endcode
 *
 *  @li@c e_media_type        - Media type
 *  @li@c t_media_subtype    - Media subtype
 *  @li@c e_mm_svc_type     - Media service type
 */
/*----------------------------------------------------------------------------*/
typedef struct _MEDIA_FORMAT_T
{
    MEDIA_TYPE_T            e_media_type;
    MEDIA_SUBTYPE_T         t_media_subtype; 
    MEDIA_SVC_TYPE_T        e_mm_svc_type;
    BOOL                    b_fav_recg_only;
    BOOL                    b_only_has_hdr;
    BOOL                    b_is_network;
    UINT64                  ui8_format_magic;
	UINT32                  ui4_ts_byte_length;      /*Only used in MPEG_TS_JP_T format, as the byte_length of the first packet*/
    
}   MEDIA_FORMAT_T;

#define MM_IS_TIME_SHIFT( t_media_fmt ) ( t_media_fmt.e_media_type == MEDIA_TYPE_TS && \
                                          t_media_fmt.t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT )  /**< Is time shift or not */

#define MM_IS_TIME_SHIFT_CMPB( t_media_fmt ) ( t_media_fmt.e_media_type == MEDIA_TYPE_TS && \
                                          t_media_fmt.t_media_subtype.u.e_ts_subtype == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT_CMPB )  /**< Is CMPB time shift or not */

                                          
#define FMT_IS_AUDIO_CATEGORY( t_media_fmt ) ((t_media_fmt.e_mm_svc_type == MEDIA_SVC_TYPE_UNKNOWN && \
                                               t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES) || \
                                               t_media_fmt.e_mm_svc_type == MEDIA_SVC_TYPE_AUDIO_ONLY)    /**< Is audio category or not */

#define FMT_IS_VIDEO_CATEGORY( t_media_fmt ) ((t_media_fmt.e_mm_svc_type == MEDIA_SVC_TYPE_UNKNOWN && \
                                              (t_media_fmt.e_media_type == MEDIA_TYPE_VIDEO_ES || \
                                               t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER || \
                                               t_media_fmt.e_media_type == MEDIA_TYPE_PS || \
                                               t_media_fmt.e_media_type == MEDIA_TYPE_TS)) ||\
                                               t_media_fmt.e_mm_svc_type == MEDIA_SVC_TYPE_VIDEO_ONLY)   /**< Is video category or not */

#define FMT_IS_NEW_TRICK_CATEGORY(t_media_fmt) ((t_media_fmt.e_media_type == MEDIA_TYPE_CONTAINER) && \
                                              (t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF ||\
                                               t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI ||\
                                               t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_FLV ||\
                                               t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA ||\
                                               t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4 ||\
                                               t_media_fmt.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_RM))

/* Mutimedia minfo index type */
#define MM_MINFO_IDX_TYPE_NONE            ((UINT32) 0x00000001) 
#define MM_MINFO_IDX_TYPE_COMMON          ((UINT32) 0x00000002)
#define MM_MINFO_IDX_TYPE_STRM            ((UINT32) 0x00000004)
#define MM_MINFO_IDX_TYPE_BUFFER          ((UINT32) 0x00000008) //container buffer the index data or not ? for SINGLE_URI_HANDLE  case

/***********  Mutimedia Stream Speed Definition ***********************/
/*----------------------------------------------------------------------------*/
/*! @enum MM_SPEED_TYPE_T 
 *  @brief  Media speed type
 *  @code
 *   typedef enum
 *   {
 *       MM_SPEED_TYPE_REWIND_1024X = 1,   
 *       MM_SPEED_TYPE_REWIND_512X,        
 *        MM_SPEED_TYPE_REWIND_256X,        
 *        MM_SPEED_TYPE_REWIND_128X,       
 *        MM_SPEED_TYPE_REWIND_64X,         
 *        MM_SPEED_TYPE_REWIND_32X,         
 *        MM_SPEED_TYPE_REWIND_16X,         
 *        MM_SPEED_TYPE_REWIND_8X,          
 *        MM_SPEED_TYPE_REWIND_4X,          
 *        MM_SPEED_TYPE_REWIND_2X,          
 *        MM_SPEED_TYPE_REWIND_1X,          
 *        MM_SPEED_TYPE_REWIND_0_DOT_9X,     
 *        MM_SPEED_TYPE_REWIND_0_DOT_8X,     
 *        MM_SPEED_TYPE_REWIND_0_DOT_7X,     
 *        MM_SPEED_TYPE_REWIND_0_DOT_6X,     
 *        MM_SPEED_TYPE_REWIND_1_DIV_2X,     
 *        MM_SPEED_TYPE_REWIND_1_DIV_4X,     
 *        MM_SPEED_TYPE_REWIND_1_DIV_8X,     
 *        MM_SPEED_TYPE_REWIND_1_DIV_16X,    
 *        MM_SPEED_TYPE_REWIND_1_DIV_32X,    
 *        MM_SPEED_TYPE_STEP_REVERSE,     
 *        MM_SPEED_TYPE_PAUSE,              
 *        MM_SPEED_TYPE_STEP,                
 *        MM_SPEED_TYPE_FORWARD_1_DIV_32X,   
 *        MM_SPEED_TYPE_FORWARD_1_DIV_16X,   
 *        MM_SPEED_TYPE_FORWARD_1_DIV_8X,    
 *        MM_SPEED_TYPE_FORWARD_1_DIV_4X,    
 *        MM_SPEED_TYPE_FORWARD_1_DIV_2X,    
 *        MM_SPEED_TYPE_FORWARD_0_DOT_6X,    
 *        MM_SPEED_TYPE_FORWARD_0_DOT_7X,    
 *        MM_SPEED_TYPE_FORWARD_0_DOT_8X,    
 *        MM_SPEED_TYPE_FORWARD_0_DOT_9X,    
 *        MM_SPEED_TYPE_FORWARD_1X,          
 *        MM_SPEED_TYPE_FORWARD_1_DOT_1X,    
 *        MM_SPEED_TYPE_FORWARD_1_DOT_2X,    
 *        MM_SPEED_TYPE_FORWARD_1_DOT_3X,    
 *        MM_SPEED_TYPE_FORWARD_1_DOT_4X,    
 *        MM_SPEED_TYPE_FORWARD_1_DOT_5X,    
 *        MM_SPEED_TYPE_FORWARD_2X,          
 *        MM_SPEED_TYPE_FORWARD_4X,          
 *        MM_SPEED_TYPE_FORWARD_8X,          
 *        MM_SPEED_TYPE_FORWARD_16X,        
 *        MM_SPEED_TYPE_FORWARD_32X,        
 *        MM_SPEED_TYPE_FORWARD_64X,        
 *        MM_SPEED_TYPE_FORWARD_128X,      
 *        MM_SPEED_TYPE_FORWARD_256X,       
 *        MM_SPEED_TYPE_FORWARD_512X,       
 *        MM_SPEED_TYPE_FORWARD_1024X       
 *    } MM_SPEED_TYPE_T;
 *  @endcode
 *
 *  @li@c   MM_SPEED_TYPE_REWIND_1024X                -  Fast Rewind 1024X 
 *  @li@c   MM_SPEED_TYPE_REWIND_512X                  -  Fast Rewind 512X 
 *  @li@c   MM_SPEED_TYPE_REWIND_256X                  -  Fast Rewind 256X 
 *  @li@c   MM_SPEED_TYPE_REWIND_128X                  -  Fast Rewind 128X 
 *  @li@c   MM_SPEED_TYPE_REWIND_64X                    -  Fast Rewind 64X 
 *  @li@c   MM_SPEED_TYPE_REWIND_32X                    -  Fast Rewind 32X 
 *  @li@c   MM_SPEED_TYPE_REWIND_16X                    -  Fast Rewind 16X 
 *  @li@c   MM_SPEED_TYPE_REWIND_8X                     -   Fast Rewind 8X 
 *  @li@c   MM_SPEED_TYPE_REWIND_4X                     -   Fast Rewind 4X 
 *  @li@c   MM_SPEED_TYPE_REWIND_2X                     -   Fast Rewind 2X 
 *  @li@c   MM_SPEED_TYPE_REWIND_1X                     -   Rewind 1X 
 *  @li@c   MM_SPEED_TYPE_REWIND_0_DOT_9X          -   Slow Rewind 0.9X 
 *  @li@c   MM_SPEED_TYPE_REWIND_0_DOT_8X          -   Slow Rewind 0.8X 
 *  @li@c   MM_SPEED_TYPE_REWIND_0_DOT_7X          -   Slow Rewind 0.7X 
 *  @li@c   MM_SPEED_TYPE_REWIND_0_DOT_6X          -   Slow Rewind 0.6X 
 *  @li@c   MM_SPEED_TYPE_REWIND_1_DIV_2X           -   Slow Rewind 1/2X 
 *  @li@c   MM_SPEED_TYPE_REWIND_1_DIV_4X           -   Slow Rewind 1/4X 
 *  @li@c   MM_SPEED_TYPE_REWIND_1_DIV_8X           -   Slow Rewind 1/8X 
 *  @li@c   MM_SPEED_TYPE_REWIND_1_DIV_16X         -   Slow Rewind 1/16X 
 *  @li@c   MM_SPEED_TYPE_REWIND_1_DIV_32X         -   Slow Rewind 1/32X 
 *  @li@c   MM_SPEED_TYPE_STEP_REVERSE                 -   Step Reverse 
 *  @li@c   MM_SPEED_TYPE_PAUSE                              -  Pause 
 *  @li@c   MM_SPEED_TYPE_STEP                                -   Step 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DIV_32X       -   Slow Forward 1/32X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DIV_16X       -  Slow Forward 1/16X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DIV_8X         -  Slow Forward 1/8X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DIV_4X         -  Slow Forward 1/4X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DIV_2X         -  Slow Forward 1/2X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_0_DOT_6X        -  Slow Forward 0.6X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_0_DOT_7X        -   Slow Forward 0.7X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_0_DOT_8X        -   Slow Forward 0.8X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_0_DOT_9X        -   Slow Forward 0.9X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1X                   -    Forward 1X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DOT_1X        -    Fast Forward 1.1X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DOT_2X        -    Fast Forward 1.2X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DOT_3X        -    Fast Forward 1.3X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DOT_4X        -    Fast Forward 1.4X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_1_DOT_5X        -    Fast Forward 1.5X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_2X                    -    Fast Forward 2X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_4X                    -    Fast Forward 4X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_8X                    -    Fast Forward 8X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_16X                   -    Fast Forward 16X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_32X                   -    Fast Forward 32X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_64X                   -    Fast Forward 64X 
 *  @li@c   MM_SPEED_TYPE_FORWARD_128X                 -    Fast Forward 128X
 *  @li@c   MM_SPEED_TYPE_FORWARD_256X                 -    Fast Forward 2566X
 *  @li@c   MM_SPEED_TYPE_FORWARD_512X                 -    Fast Forward 512X
 *  @li@c   MM_SPEED_TYPE_FORWARD_1024X               -    Fast Forward 1024X
 * 
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MM_SPEED_TYPE_REWIND_1024X = 1,   /* Fast Rewind 1024X */
    MM_SPEED_TYPE_REWIND_512X,        /* Fast Rewind 512X */
    MM_SPEED_TYPE_REWIND_256X,        /* Fast Rewind 256X */
    MM_SPEED_TYPE_REWIND_128X,        /* Fast Rewind 128X */
    MM_SPEED_TYPE_REWIND_64X,         /* Fast Rewind 64X */
    MM_SPEED_TYPE_REWIND_32X,         /* Fast Rewind 32X */
    MM_SPEED_TYPE_REWIND_16X,         /* Fast Rewind 16X */
    MM_SPEED_TYPE_REWIND_8X,          /* Fast Rewind 8X */
    MM_SPEED_TYPE_REWIND_4X,          /* Fast Rewind 4X */
    MM_SPEED_TYPE_REWIND_3X,          /* Fast Rewind 3X */
    MM_SPEED_TYPE_REWIND_2X,          /* Fast Rewind 2X */
    MM_SPEED_TYPE_REWIND_1X,          /* Rewind 1X */
    MM_SPEED_TYPE_REWIND_0_DOT_9X,     /* Slow Rewind 0.9X */
    MM_SPEED_TYPE_REWIND_0_DOT_8X,     /* Slow Rewind 0.8X */
    MM_SPEED_TYPE_REWIND_0_DOT_7X,     /* Slow Rewind 0.7X */
    MM_SPEED_TYPE_REWIND_0_DOT_6X,     /* Slow Rewind 0.6X */
    MM_SPEED_TYPE_REWIND_1_DIV_2X,     /* Slow Rewind 1/2X */
    MM_SPEED_TYPE_REWIND_1_DIV_3X,     /* Slow Rewind 1/3X */
    MM_SPEED_TYPE_REWIND_1_DIV_4X,     /* Slow Rewind 1/4X */
    MM_SPEED_TYPE_REWIND_1_DIV_8X,     /* Slow Rewind 1/8X */
    MM_SPEED_TYPE_REWIND_1_DIV_16X,    /* Slow Rewind 1/16X */
    MM_SPEED_TYPE_REWIND_1_DIV_32X,    /* Slow Rewind 1/32X */
    MM_SPEED_TYPE_STEP_REVERSE,        /* Step Reverse */
    MM_SPEED_TYPE_PAUSE,               /* Pause */
    MM_SPEED_TYPE_STEP,                /* Step */
    MM_SPEED_TYPE_FORWARD_1_DIV_32X,   /* Slow Forward 1/32X */
    MM_SPEED_TYPE_FORWARD_1_DIV_16X,   /* Slow Forward 1/16X */
    MM_SPEED_TYPE_FORWARD_1_DIV_8X,    /* Slow Forward 1/8X */
    MM_SPEED_TYPE_FORWARD_1_DIV_4X,    /* Slow Forward 1/4X */
    MM_SPEED_TYPE_FORWARD_1_DIV_3X,    /* Slow Forward 1/3X */
    MM_SPEED_TYPE_FORWARD_1_DIV_2X,    /* Slow Forward 1/2X */
    MM_SPEED_TYPE_FORWARD_0_DOT_6X,    /* Slow Forward 0.6X */
    MM_SPEED_TYPE_FORWARD_0_DOT_7X,    /* Slow Forward 0.7X */
    MM_SPEED_TYPE_FORWARD_0_DOT_8X,    /* Slow Forward 0.8X */
    MM_SPEED_TYPE_FORWARD_0_DOT_9X,    /* Slow Forward 0.9X */
    MM_SPEED_TYPE_FORWARD_1X,          /* Forward 1X */
    MM_SPEED_TYPE_FORWARD_1_DOT_1X,    /* Fast Forward 1.1X */
    MM_SPEED_TYPE_FORWARD_1_DOT_2X,    /* Fast Forward 1.2X */
    MM_SPEED_TYPE_FORWARD_1_DOT_3X,    /* Fast Forward 1.3X */
    MM_SPEED_TYPE_FORWARD_1_DOT_4X,    /* Fast Forward 1.4X */
    MM_SPEED_TYPE_FORWARD_1_DOT_5X,    /* Fast Forward 1.5X */
    MM_SPEED_TYPE_FORWARD_2X,          /* Fast Forward 2X */
    MM_SPEED_TYPE_FORWARD_3X,          /* Fast Forward 3X */
    MM_SPEED_TYPE_FORWARD_4X,          /* Fast Forward 4X */
    MM_SPEED_TYPE_FORWARD_8X,          /* Fast Forward 8X */
    MM_SPEED_TYPE_FORWARD_16X,         /* Fast Forward 16X */
    MM_SPEED_TYPE_FORWARD_32X,         /* Fast Forward 32X */
    MM_SPEED_TYPE_FORWARD_64X,         /* Fast Forward 64X */
    MM_SPEED_TYPE_FORWARD_128X,        /* Fast Forward 128X */
    MM_SPEED_TYPE_FORWARD_256X,        /* Fast Forward 2566X */
    MM_SPEED_TYPE_FORWARD_512X,        /* Fast Forward 512X */
    MM_SPEED_TYPE_FORWARD_1024X        /* Fast Forward 1024X */
} MM_SPEED_TYPE_T;

typedef enum
{
   MM_PLAY_MODE_TYPE_NORMAL,
   MM_PLAY_MODE_TYPE_IFRAME
}MM_PLAY_MODE_TYPE_T;

#define MM_TRICK_NOINDEX_MAXSPEED MM_SPEED_TYPE_FORWARD_8X
typedef struct
{
   MM_SPEED_TYPE_T      eSpeed;
   MM_PLAY_MODE_TYPE_T  ePlayMode;
}MM_PLAY_MODE_PL_T;

/***********  Mutimedia Stream Priority Definition ***********************/
/*----------------------------------------------------------------------------*/
/*! @enum MM_PRIORITY_TYPE_T 
 *  @brief  Media priority type
 *  @code
 *   typedef enum
 *   {
 *       MM_PRIORITY_DEFAULT = 0,   
 *       MM_PRIORITY_HIGH,        
 *        MM_PRIORITY_LOW
 *    } MM_PRIORITY_TYPE_T;
 *  @endcode
 *
 *  @li@c   MM_PRIORITY_DEFAULT           -  default priority 
 *  @li@c   MM_PRIORITY_HIGH                 -  high priority 
 *  @li@c   MM_PRIORITY_LOW                  -  low priority
 * 
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MM_PRIORITY_DEFAULT = 0,
    MM_PRIORITY_HIGH,
    MM_PRIORITY_LOW
}MM_PRIORITY_TYPE_T;


/* buffer control mode: by 3rd or mw */
typedef enum
{
    MM_BUF_CTRL_MODE_DFT = 0,
    MM_BUF_CTRL_MODE_BY_3RD
} MM_BUFFER_CONTROL_MODE_T;

#define SUPPORT_3D_MEDIA
/*add for 3d media, stereoscopic layout type*/
typedef enum
{
    MM_SCOPIC_UNKNOWN=0,
    MM_SCOPIC_RF,   /*"SideBySideRF" (side-by-side, right image first)*/
    MM_SCOPIC_LF,   /*"SideBySideLF" (side-by-side, left image first)  */
    MM_SCOPIC_RT,   /*"OverUnderRT" (over/under, right image top)   */
    MM_SCOPIC_LT,   /*"OverUnderLT", (over/under, left image top)     */
    MM_SCOPIC_DS,   /*"DualStream", (separate left/right streams)      */
    MM_SCOPIC_MS,   /*"MultiStream" (more than two streams)            */
    MM_SCOPIC_NORMAL,   /*2D video            */
    MM_SCOPIC_SENSIO_RF,   /*"SideBySideRF" (side-by-side, right image first     */
    MM_SCOPIC_SENSIO_LF   /*"SideBySideLF" (side-by-side, left image first     */
} MM_SCOPIC_TYPE_T;

typedef enum
{
    MM_SRC_TAG3D_2D = 0,
    MM_SRC_TAG3D_MVC,          /* MVC = Multi-View Codec */
    MM_SRC_TAG3D_FP,           /* FP = Frame Packing */
    MM_SRC_TAG3D_FS,           /* FS = Frame Sequential */
    MM_SRC_TAG3D_TB,           /* TB = Top-and-Bottom */
    MM_SRC_TAG3D_SBS,          /* SBS = Side-by-Side */
    MM_SRC_TAG3D_REALD,
    MM_SRC_TAG3D_SENSIO,
    MM_SRC_TAG3D_TTDO,        /* TTD only */
    MM_SRC_TAG3D_NOT_SUPPORT
}   MM_SRC_TAG3D_TYPE_T;

/* buffer control mode: by 3rd or mw */
typedef enum
{
    MM_ENCRYPT_METHORD_UNKNOWN = 0,
    MM_ENCRYPT_METHORD_DRM_MLN,
    MM_ENCRYPT_METHORD_DRM_DCF,
    MM_ENCRYPT_METHORD_DRM_CENC,
    MM_ENCRYPT_METHORD_LOCAL_KEY
} MM_ENCRYPT_METHORD_T;
/* seek type for DLNA */
typedef enum
{
    MM_TYPE_BYTE_SEEK = 0,
	MM_TYPE_TIME_SEEK
}MM_SEEK_TYPE;
/***********  MM Encryption/Decryption Definition ***********************/
/* Encryption/Descryption algorithm */
typedef enum
{
    MM_CRYPT_MODE_INVALID = 0,
    MM_CRYPT_MODE_DES_EBC,
    MM_CRYPT_MODE_DES_CBC,
    MM_CRYPT_MODE_3_DES_EBC,
    MM_CRYPT_MODE_3_DES_CBC,
    MM_CRYPT_MODE_DVB,
    MM_CRYPT_MODE_DVB_CONF,
    MM_CRYPT_MODE_MULTI2_BIG,
    MM_CRYPT_MODE_MULTI2_LITTLE,
    MM_CRYPT_MODE_AES_ECB,
    MM_CRYPT_MODE_AES_CBC,
    MM_CRYPT_MODE_AES_CTR
}   MM_CRYPT_MODE_T;

/*! @enum   IMTK_PB_CTRL_RTB_MODE_T
 *  @brief  residual termination block
 */
typedef enum
{
    MM_RTB_MODE_CLEAR = 0,
    MM_RTB_MODE_CTS,
    MM_RTB_MODE_SCTE52,
    MM_RTB_MODE_MAX,
} MM_RTB_MODE_T;

#define MM_AES_ENCRYPT_IV        1
#define MM_AES_ENCRYPT_UNIQUE    2
#define MM_AES_ENCRYPT_EVEN      4
#define MM_AES_ENCRYPT_ODD       8

/*! @struct IMTK_PB_CTRL_AES_ENCRYPT_INFO_T
 *  @brief  AES crypto structure
 */
typedef struct
{
    UINT8     u1Mask;
    UINT8     u1KeyLen;
    BOOL      fgWarpKey;
    BOOL      fgWarpIV;
    UINT8     au1IV[16];
    UINT8     au1UniqueKey[16];
    UINT8     au1EvenKey[32];
    UINT8     au1Odd_key[32];
    MM_RTB_MODE_T       eRtbMode;
    MM_CRYPT_MODE_T     e_aes_mode;
} MM_AES_ENCRYPT_INFO_T;


/* Encryption/Descryption key index */
typedef enum
{
    MM_CRYPT_KEY_ODD = 0,
    MM_CRYPT_KEY_EVEN,
    MM_CRYPT_KEY_ALTERNATE
}   MM_CRYPT_KEY_INDEX_T;

/* Encryption/Descryption info */
typedef struct 
{
	BOOL                     b_is_encrypt;
    MM_CRYPT_MODE_T       	 e_mode;
    UINT32                   aui4_odd_key_1[2];
    UINT32                   aui4_odd_key_2[2];
    UINT32                   aui4_even_key_1[2];
    UINT32                   aui4_even_key_2[2];
    MM_CRYPT_KEY_INDEX_T     e_key_idx;    
}MM_PVR_CRYPT_INFO_T;

typedef struct 
{
    MM_CRYPT_MODE_T     e_mode;
    STREAM_TYPE_T       e_strm_type;
    UINT32              ui4_content_id;
    UINT16              ui2_key_idx;
    UINT8               aui1_key[16];
}MM_TRACK_MARLIN_CRYPT_INFO_T;

typedef struct
{
    BOOL        b_hash_content_id;      ///< pui1_content_id hashed or not. If hashed, only fetch  ::IMTK_PB_CTRL_MARLIN_HASH_CONTENT_ID_LENGTH in aui1_content_id.    
    UINT32      ui4_content_id_len;     ///< pui1_content_id length not include null terminated character
    UINT8*      pui1_content_id;        ///< pui1_content_id string
    UINT8       aui1_content_key[16];
}MM_TRACK_CENC_CRYPT_INFO_T;

#define MAX_MARLIN_TRACK_NUM        17/* marlin max number of track : 1(video) + 2(audio) + 4(subtitle) */
typedef struct 
{
    UINT8                   ui1_valid_cnt;
    MM_TRACK_MARLIN_CRYPT_INFO_T    t_track_drm[MAX_MARLIN_TRACK_NUM];
}MM_MLN_CRYPT_INFO_T;

typedef struct 
{
    UINT8                           ui1_valid_cnt;
    MM_TRACK_MARLIN_CRYPT_INFO_T    t_track_drm[MAX_MARLIN_TRACK_NUM];
}MM_PDCF_CRYPT_INFO_T;

typedef struct{
    UINT8                           ui1_valid_cnt;
    MM_TRACK_CENC_CRYPT_INFO_T      t_track_drm[MAX_MARLIN_TRACK_NUM];
}MM_CENC_CRYPT_INFO_T;

/*! @enum   MM_PRDY_KEY_TYPE
 *  @brief  Playready key type
 */
typedef enum
{
    MM_PRDY_ENCRPT_KEY_UNKNOWN = 0,
    MM_PRDY_ENCRPT_KEY_RC4,
    MM_PRDY_ENCRPT_KEY_AES
} MM_PRDY_KEY_TYPE;

typedef struct
{
    MM_PRDY_KEY_TYPE    e_key_type;
    UINT8               aui1_key_info[128];
    UINT32              ui4_key_size;
}MM_PRDY_ENCRPT_KEY_INFO_T;


#define MARLIN_CONTENT_ID_LENGTH_MAX    256
#define MARLIN_CONTENT_STRM_NUM_MAX     (1 + 8 + 8)/* 1 video + 8 audio + 8 subtitle*/
#define RESEVED_KEY_LENGTH_MAX          512

typedef struct
{
    UINT8*  paui1_content_id;
    UINT8   aui1_content_key[16];
} MM_MLN_KEY_ENTRY_T;

typedef struct
{
    UINT8                  ui1_valid_entry_cnt;
    MM_MLN_KEY_ENTRY_T     t_key_entry[MARLIN_CONTENT_STRM_NUM_MAX];
} MM_MLN_KEY_TABLE_T;

typedef struct
{
    BOOL                   b_valid;
    BOOL                   b_permission;
    MM_MLN_KEY_TABLE_T     t_marlin_key_table;
    UINT8*                 pui1_ms3_content_url;
    //struct tm              t_not_before;
    //struct tm              t_not_after;
} MM_MLN_MS3_INFO_T;

typedef struct
{
    BOOL        b_hash_content_id;      ///< pui1_content_id hashed or not. If hashed, only fetch  ::IMTK_PB_CTRL_MARLIN_HASH_CONTENT_ID_LENGTH in aui1_content_id.
    UINT32      ui4_content_id_len;     ///< pui1_content_id length not include null terminated character
    UINT8*      pui1_content_id;        ///< pui1_content_id string
    UINT8       aui1_content_key[16];
} MM_MLN_TS_INFO_T;

typedef struct
{
    UINT8             b_is_even;
    UINT8             aui1_current_key[16];
    UINT8             aui1_current_iv[16];
    UINT8             aui1_next_key[16];
    UINT8             aui1_next_iv[16];
}MM_MLN_TS_ECM_T;

typedef enum
{
    MM_CRYPT_TYPE_UNKNOWN = 0,
    MM_CRYPT_TYPE_PVR,
    MM_CRYPT_TYPE_MLN,
    MM_CRYPT_TYPE_AES,
    MM_CRYPT_TYPE_PDCF,
    MM_CRYPT_TYPE_PRDY,
    MM_CRYPT_TYPE_CENC,
    //MM_CRYPT_TYPE_MS3,
    MM_CRYPT_TYPE_RSV,
    MM_CRYPT_TYPE_TSMLN,
    MM_CRYPT_TYPE_LOCAL_KEY
} MM_CRYPT_TYPE_T;
    
typedef struct 
{ 
    MM_CRYPT_TYPE_T         e_mm_crypt_type;
    union
    {
        MM_PVR_CRYPT_INFO_T         t_pvr_crypt;
        MM_MLN_CRYPT_INFO_T         t_ml_crypt;
        MM_CENC_CRYPT_INFO_T        t_cenc_crypt;
        MM_AES_ENCRYPT_INFO_T       t_aes;
        MM_PDCF_CRYPT_INFO_T        t_pdcf_crypt;
        MM_PRDY_ENCRPT_KEY_INFO_T   t_prdy_crypt;
        MM_MLN_MS3_INFO_T           t_ml_ms3_crypt;
        MM_MLN_TS_INFO_T            t_ml_ts_crypt;
        MM_MLN_TS_ECM_T             t_ml_ecm_crypt;
        UINT8                       aui1Reserved[RESEVED_KEY_LENGTH_MAX];
    }u;
}   MM_CRYPT_INFO_T;


#define MM_MAX_VIDEO_STRM_NUM        (1)
#define MM_MAX_AUDIO_STRM_NUM        (8)
#define MM_MAX_SUBTITLE_STRM_NUM     (8)

#endif/*_U_MM_COMMON_H_*/
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW*/
/*----------------------------------------------------------------------------*/
