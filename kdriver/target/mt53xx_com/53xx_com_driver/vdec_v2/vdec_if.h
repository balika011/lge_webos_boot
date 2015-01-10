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
 *-----------------------------------------------------------------------------
 * $Author: p4admin $
 * $Date: 2015/01/10 $
 * $RCSfile: vdec_if.h,v $
 * $Revision: #2 $
 *---------------------------------------------------------------------------*/

/** @file vdec_if.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_IF_H
#define VDEC_IF_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdp_if.h"
#include "b2r_drvif.h"
#include "b2r_if.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "x_typedef.h"
#include "x_os.h"
#include "u_common.h"
#include "x_mm_common.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VDEC_DRV_OK                 ((INT32) 0)
#define VDEC_DRV_SET_FAILED         ((INT32) -262)
#define VDEC_DRV_INV_GET_INFO       ((INT32) -258)
#define VDEC_DRV_INV_SET_INFO       ((INT32) -259) /**<the set operation is invalid or cannot be processed*/
#define VDEC_PRIORITY_DEFAULT          ((INT16) 10)

#ifndef VDEC_CRC_TEST
#define VDEC_CRC_TEST
#endif

#if defined(CC_MT5890)
#define VDEC_IS_POST_MT5890
#endif

#if defined(CC_MT5882) || defined(VDEC_IS_POST_MT5890)
#define VDEC_IS_POST_MT5882
#endif

#if defined(CC_MT5881) || defined(VDEC_IS_POST_MT5882)
#define VDEC_IS_POST_MT5881
#endif

#if defined(CC_MT5880) || defined(VDEC_IS_POST_MT5881)
#define VDEC_IS_POST_MT5880
#endif

#if defined(CC_MT5398) || defined(VDEC_IS_POST_MT5880) || defined(CC_MT5399) || defined(CC_MT5890)
#define VDEC_IS_POST_MT5398
#endif

#if defined(CC_MT5389) || defined(VDEC_IS_POST_MT5398)
#define VDEC_IS_POST_MT5389
#endif

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(VDEC_IS_POST_MT5389)
#define VDEC_IS_POST_MT5368
#endif

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
#define VDEC_IS_POST_MT5365
#endif

#if defined(CC_MT5363) || defined(VDEC_IS_POST_MT5365)
#define VDEC_IS_POST_MT5363
#endif

#if defined(VDEC_IS_POST_MT5882)
#define VDEC_4K_DEC_CHECK
#endif

#define VDEC_COMMON_ISR
#define VDEC_SUPPORT_ISR
// enable it only when JPEG isr is handled
//#define VDEC_REFINE_ISR_CONDITIONAL_BRANCH

#ifdef CC_REALD_3D_SUPPORT
#define VDEC_3D_CTRL_OFF    0
#define VDEC_3D_CTRL_FORCE_2D   1
#define VDEC_3D_CTRL_FORCE_3D_LR    2
#define VDEC_3D_CTRL_FORCE_3D_TB    3
#define VDEC_3D_CTRL_BYPASS    4
#define VDEC_3D_CTRL_FORCE_2D_LR   5    //force 2D, and indicate source is SBS
#define VDEC_3D_CTRL_FORCE_2D_TB   6   //force 2D, and indicate source is TB
#endif
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{

    VDEC_VLD_NOUSE=0,        //status: vdec thread didn't play
    VDEC_VLD_ACTIVE,         //status: vdec thread playing
    VDEC_VLD_PENDING,        //status: vdec thread have no working cyles to consume
    VDEC_VLD_FREE_RUN,       //status: thread schedule no using to this vdec thread,
    VDEC_VLD_PAUSE,          // status: vdec pause
    VDEC_VLD_FINISH          //status: vdec thread have send Eos
}  VDEC_VLD_STATUS_T;

/** the aspect ratio of video source.
*/
typedef enum
{
    VDEC_ASPECT_RATIO_NONE = 0,
    VDEC_ASPECT_RATIO_1_1, ///< Pixel Aspect ratio 1:1
    VDEC_ASPECT_RATIO_4_3, ///< Display Aspect ratio 4:3
    VDEC_ASPECT_RATIO_16_9, ///< Display Aspect ratio 16:9
    VDEC_ASPECT_RATIO_221_1, ///< Display Aspect ratio 2.21:1
    VDEC_ASPECT_RATIO_10_11, ///< Pixel Aspect ratio 10:11
    VDEC_ASPECT_RATIO_40_33, ///< Pixel Aspect ratio 40:33
    VDEC_ASPECT_RATIO_16_11, ///< Pixel Aspect ratio 16:11
    VDEC_ASPECT_RATIO_12_11, ///< Pixel Aspect ratio 12:11
    VDEC_ASPECT_RATIO_3_2,   ///< Pixel Aspect ratio 3:2
    VDEC_ASPECT_RATIO_24_11,
    VDEC_ASPECT_RATIO_20_11,
    VDEC_ASPECT_RATIO_32_11,
    VDEC_ASPECT_RATIO_80_33,
    VDEC_ASPECT_RATIO_18_11,
    VDEC_ASPECT_RATIO_15_11,
    VDEC_ASPECT_RATIO_64_33,
    VDEC_ASPECT_RATIO_160_99,
    VDEC_ASPECT_RATIO_TRANSMIT,
    VDEC_ASPECT_RATIO_2_1,
    VDEC_ASPECT_RATIO_MAX   
} VDEC_ASPECT_RATIO_T;

typedef enum
{
    VDEC_ASP_USR_TYPE_SRC = 1, /* source aspect ratio */
    VDEC_ASP_USR_TYPE_DISP     /* display aspect ratio */
} VDEC_ASP_USR_TYPE_T;


typedef enum
{
    VDEC_STREAM_TYPE_ORIGINAL,
    VDEC_STREAM_TYPE_IPB,
    VDEC_STREAM_TYPE_I,
    VDEC_STREAM_TYPE_UNKNOW
}VDEC_STREAM_TYPE_T;


typedef enum // pre-parse status
{
    VDEC_PREPS_ST_NONE,     //status none
    VDEC_PREPS_ST_DONE,     //status done
    VDEC_PREPS_ST_BEGIN,    //begin pre-parse
    VDEC_PREPS_ST_SKIP_ES,  //Skip Main msg-Q es
    VDEC_PREPS_ST_FPS_CALC, //frame rate unknow, use PTS to calc the frame rate
    VDEC_PREPS_ST_MAX
}VDEC_PREPS_ST_T;

/** vdec format.
*/
typedef enum
{
    VDEC_FMT_MPV,
    VDEC_FMT_MP4,
    VDEC_FMT_H264,
    VDEC_FMT_WMV,
    VDEC_FMT_H264VER,
    VDEC_FMT_MJPEG,
    VDEC_FMT_RV,
    VDEC_FMT_AVS,
    VDEC_FMT_VP6,
    VDEC_FMT_VP8,
    VDEC_FMT_RAW,
    VDEC_FMT_OGG,
    VDEC_FMT_JPG,
    VDEC_FMT_PNG,
    VDEC_FMT_H265,
    VDEC_FMT_VP9,
    VDEC_FMT_MAX
}   ENUM_VDEC_FMT_T;//if ENUM_VDEC_FMT_T was modified ,must tell B2R owner  ,B2R need keep this portion same with vdec 

/**vdec profile
*/
typedef enum
{
    VDEC_PROFILE_UNKNOWN = 0,
    VDEC_PROFILE_SIMPLE,
    VDEC_PROFILE_MAIN,
    VDEC_PROFILE_SNR_SCALABLE,
    VDEC_PROFILE_SPATIAL_SCALABLE,
    VDEC_PROFILE_HIGH,
    VDEC_PROFILE_MULTIVIEW,
    VDEC_PROFILE_4_2_2,
    //MPEG4
    VDEC_PROFILE_ADVANCED_CODING_EFFICIENCY,
    VDEC_PROFILE_SIMPLE_STUDIO,
    VDEC_PROFILE_CORE_STUDIO,
    VDEC_PROFILE_FINE_GRANULARITY_SCALABLE,
    VDEC_PROFILE_SIMPLE_FACE_ANIMATION,
    VDEC_PROFILE_SIMPLE_FBA,
    VDEC_PROFILE_BASIC_ANIMATED_TEXTURE,
    VDEC_PROFILE_SCALABLE_TEXTURE,
    VDEC_PROFILE_ADVANCED_SCALABLE_TEXTURE,
    VDEC_PROFILE_HYBRID,
    VDEC_PROFILE_SIMPLE_SCALABLE,
    VDEC_PROFILE_ADVANCED_SIMPLE,
    VDEC_PROFILE_CORE,
    VDEC_PROFILE_CORE_SCALABLE,
    VDEC_PROFILE_ADVANCED_CORE,
    VDEC_PROFILE_N_BIT,
    VDEC_PROFILE_REAL_TIME_SIMPLE,
    //H264
    VDEC_PROFILE_BASELINE,
    VDEC_PROFILE_EXTENDED,
    VDEC_PROFILE_HIGH_10,
    VDEC_PROFILE_HIGH_4_2_2,
    VDEC_PROFILE_HIGH_4_4_4,
    //VC1
    VDEC_PROFILE_ADVANCED,	
    //wmv
    VDEC_PROFILE_PC
}	ENUM_VDEC_PROFILE_T;

/**vdec level
*/
typedef enum
{
    VDEC_LEVEL_UNKNOWN = 0,
    VDEC_LEVEL_MAIN,
    VDEC_LEVEL_LOW,
    VDEC_LEVEL_HIGH,
    VDEC_LEVEL_HIGH_1440,
    VDEC_LEVEL_0,
    VDEC_LEVEL_1,
    VDEC_LEVEL_2,
    VDEC_LEVEL_3,
    VDEC_LEVEL_4,
    VDEC_LEVEL_5,
    VDEC_LEVEL_1_B,
    VDEC_LEVEL_1_1,
    VDEC_LEVEL_1_2,
    VDEC_LEVEL_1_3,
    VDEC_LEVEL_2_1,
    VDEC_LEVEL_2_2,
    VDEC_LEVEL_3_1,
    VDEC_LEVEL_3_2,
    VDEC_LEVEL_4_1,
    VDEC_LEVEL_4_2,
    VDEC_LEVEL_5_1,
    VDEC_LEVEL_LL,
    VDEC_LEVEL_ML,
    VDEC_LEVEL_HL,
    VDEC_LEVEL_L_0,
    VDEC_LEVEL_L_1,
    VDEC_LEVEL_L_2,
    VDEC_LEVEL_L_3,
    VDEC_LEVEL_L_4
}   ENUM_VDEC_LEVEL_T;

/** vdec push format.
*/
typedef enum
{
    VDEC_PUSH_FMT_UNUSED,
    VDEC_PUSH_FMT_MPV,
    VDEC_PUSH_FMT_MP4,
    VDEC_PUSH_FMT_H263,
    VDEC_PUSH_FMT_SORENSEN,
    VDEC_PUSH_FMT_DIVX311,
    VDEC_PUSH_FMT_H264,
    VDEC_PUSH_FMT_WMV7,
    VDEC_PUSH_FMT_WMV8,
    VDEC_PUSH_FMT_WMV9,
    VDEC_PUSH_FMT_VC1,
    VDEC_PUSH_FMT_H264VER,
    VDEC_PUSH_FMT_MJPEG,
    VDEC_PUSH_FMT_RV30,
    VDEC_PUSH_FMT_RV40,
    VDEC_PUSH_FMT_AVS,
    VDEC_PUSH_FMT_VP6,
    VDEC_PUSH_FMT_VP8,
    VDEC_PUSH_FMT_RAW,
    VDEC_PUSH_FMT_OGG,
    VDEC_PUSH_FMT_JPG,
    VDEC_PUSH_FMT_H265,
    VDEC_PUSH_FMT_MAX
}   ENUM_VDEC_PUSH_FMT_T;

/** MM parameter
*/
typedef enum
{
    VDEC_MM_PLAY_SPEED = 0,
    VDEC_MM_FRAME_RATE,
    VDEC_MM_CONTAINER_TYPE,
    VDEC_MM_SEQ_INFO,
    VDEC_MM_SRC_TYPE,
#ifdef CC_B2R_SENSIO_CONV
    VDEC_MM_SENSIO_FORMAT,
#endif    
    VDEC_MM_TPT_SEQ_INFO,
    VDEC_MM_RV_RPR_INFO,
    VDEC_MM_3D_INFO,
    VDEC_MM_3D_TRICK_TYPE,
    VDEC_MM_STREAM_TYPE,
    VDEC_MM_DIVX4_SIGNAL,
    VDEC_MM_XVID_SIGNAL,
    VDEC_MM_PARSING_INFO,

    VDEC_MM_PIC_DEC_IS_THUMBNAIL,
    VDEC_MM_PIC_OUTPUT_RESOLUTION,
    VDEC_MM_PIC_ROTATION,
    VDEC_MM_PIC_CLEARFRAME,
    VDEC_MM_PIC_FILESIZE,
    VDEC_MM_SWDMX_SEND_EOS_DONE,
    VDEC_MM_SWDMX_SEND_CHG,
    VDEC_MM_SWDMX_SEND_NO_FLUSH,
    VDEC_MM_START_PTS_INFO, //without es fllush.

    VDEC_MM_SET_SEAMLESS_TARGET,

    VDEC_MM_PVR_MODE,
    VDEC_MM_DROP_TIMEOUTFRAME,
    VDEC_MM_DUAL_CORE_ENABLE,
    VDEC_MM_UFO_MODE_ENABLE,
    VDEC_MM_HAL_PARAMETER,
    VDEC_MM_MAX
} VDEC_MM_PARAM_T;

typedef enum
{
	HAL_VDEC_PARAM_3DMODE = 0,
	HAL_VDEC_PARAM_NOT_USE_STC,
	HAL_VDEC_PARAM_AUDIO_CHANNEL,
	HAL_VDEC_PARAM_DISPLAY_DELAY,
	HAL_VDEC_PARAM_LIPSYNC_MASTER,
	HAL_VDEC_PARAM_MAX
}HAL_VDEC_PARAM_T;

/** the notify conditions.
  This structure match to the one in mw. Can't change it's sequence.
*/
typedef enum
{
    VDEC_COND_ERROR = -1,
    VDEC_COND_CTRL_DONE,
    VDEC_COND_RESOLUTION_CHG,
    VDEC_COND_I_FRAME_SET_BUFF_DONE,
    VDEC_COND_I_FRAME_DONE,
    VDEC_COND_DECODE_STATUS_CHG,
    VDEC_COND_SPEED_CHG,
    VDEC_COND_THUMBNAIL_DONE,
    VDEC_COND_THUMBNAIL_FAIL,
    VDEC_COND_BIT_RATE,
    VDEC_COND_PLAY_DONE,
    VDEC_DEC_DECODE_FRAMERATE_NOT_SUPPORT,
    VDEC_COND_THUMBNAIL_PLAY_EOS,
	VDEC_COND_WFD_DECODE_ERR,
#ifdef CC_SUPPORT_MTK_ANDROID_RM
    VDEC_MJPG_IMGRZ_RELEASE,
#endif
    VDEC_COND_VPUSH_IO_DONE,
#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
    VDEC_COND_CRC_CHECK_DONE,
    VDEC_COND_CRC_GET_DONE,
#endif
    VDEC_COND_FRAME_DONE,
} VDEC_DEC_COND_T;


/** the notify condition status.
  This structure match to the one in mw. Can't change it's sequence.
*/
typedef enum
{
    VDEC_DEC_DECODE_UNKNOWN = -1,
    VDEC_DEC_DECODE_NO_DATA,
    VDEC_DEC_DECODE_NORMAL,
    VDEC_DEC_DECODE_ERROR,
    VDEC_DEC_DECODE_HD_NOT_SUPPORT,
    VDEC_DEC_DECODE_CODEC_NOT_SUPPORT,
//#ifndef DIVX_PLUS_CER
    VDEC_DEC_DECODE_BITSTREAM_NOT_SUPPORT,
//#endif
    VDEC_DEC_DECODE_CODEC_TYPE_CHG,    
    VDEC_DEC_DECODE_RECEIVE_DATA,
    VDEC_DEC_DECODE_RES_NOT_SUPPORT,
    VDEC_DEC_DECODE_FORCE_STOP,
    VDEC_DEC_DECODE_FRAME_RATE_OVERFLOW
}   VDEC_DEC_DECODE_STATUS_T;

typedef enum 
{
    VDEC_DEC_ERROR_INVALID_ARG = -1,
    VDEC_DEC_ERROR_INTERNAL_ERROR = -2,
    VDEC_DEC_ERROR_WORNG_STATE = -3,
    VDEC_DEC_ERROR_NONE = 0,
    VDEC_DEC_ERROR_FIFO_FULL = 1,

}VDEC_DEC_ERROR_ID_T;

/** the notify condition status.
  This structure match to some structure, Can't change it's value
*/
typedef enum
{
    VDEC_PIC_TYPE_SEQ = 1,
    VDEC_PIC_TYPE_I   = 2,
    VDEC_PIC_TYPE_P   = 4,
    VDEC_PIC_TYPE_B   = 8,
    VDEC_PIC_TYPE_END = 16
}   VDEC_PIC_TYPE_T;


/** 3D type in container header.
*/
typedef enum
{
    VDEC_3D_NONE,
    VDEC_3D_SBS_LF, //side by side, left image first
    VDEC_3D_SBS_RF, //side by side, right image first
    VDEC_3D_TB_LF,   //top bottom, left image first
    VDEC_3D_TB_RF,   //top bottom, right image first
    VDEC_3D_DUAL_STREAM,
    VDEC_3D_MULTI_STREAM,
    VDEC_3D_MVC,
    VDEC_3D_REALD_DUAL_STREAM,
	VDEC_3D_SENSIO_LF,
	VDEC_3D_SENSIO_RF,
    VDEC_3D_CHECKER_BOARD,
	VDEC_3D_BIVL,
    VDEC_3D_MAX
}   VDEC_3D_TYPE_T;

typedef enum
{
    VDEC_3D_RESOLUTION_HALF,
    VDEC_3D_RESOLUTION_FULL,
    VDEC_3D_RESOLUTION_MAX
}   VDEC_3D_RESOLUTION_TYPE_T;

typedef enum
{
    VDEC_3D_FORCE2D_AUTO,
    VDEC_3D_FORCE2D_L_VIEW,
    VDEC_3D_FORCE2D_R_VIEW,
}   VDEC_3D_FORCE2D_TYPE_T;

typedef enum
{
    VDEC_3D_TRICK_L_VIEW,  //trick mode only L
    VDEC_3D_TRICK_R_VIEW, //trick mode only R
    VDEC_3D_TRICK_3D_VIEW, //trick mode 3D view
}   VDEC_3D_TRICK_TYPE_T;

typedef enum
{
    VDEC_AVC_SEI_FRAME = 0,
    VDEC_AVC_SEI_TOP_FIELD,
    VDEC_AVC_SEI_BOTTOM_FIELD,
    VDEC_AVC_SEI_TOP_BOTTOM,
    VDEC_AVC_SEI_BOTTOM_TOP,
    VDEC_AVC_SEI_TOP_BOTTOM_TOP,
    VDEC_AVC_SEI_BOTTOM_TOP_BOTTOM,
    VDEC_AVC_SEI_FRAME_DOUBLING,
    VDEC_AVC_SEI_FRAME_TRIPLING,
    VDEC_AVC_SEI_FRAME_UNKNOWN
} VDEC_AVC_SEI_PIC_STRUCT;

typedef struct {
    UINT16 u2UdfNs;
    UINT32 u4ESBufDataSize;
    UINT32 u4DisplayQNum;
    UINT32 u4DecodingPTS;
    UINT32 u4DisplayPTS;
} VDEC_GNL_INFO_T;


//vdec seamless mode
typedef enum 
{
    VDEC_SEAMLESS_NONE = 0,
    VDEC_SEAMLESS_SD,
    VDEC_SEAMLESS_HD,
    VDEC_SEAMLESS_FHD,
    VDEC_SEAMLESS_4K2K
} VDEC_SEAMLESS_MODE;


typedef struct {
    UINT32 au4DecTime[5][9]; //[3]P,B,I,SP,SI, [4]maxMs, minMs, totalMs, FrmNum, OverTimeNum, SWTime, DecCycle, MaxDecCycle, MaxDramCycle
    VDEC_ASPECT_RATIO_T eAspectRatio;
    UINT32 u4PixelAspectRatioWidth;
    UINT32 u4PixelAspectRatioHeight;
    VDEC_DEC_DECODE_STATUS_T eNotifyStatus;
    ENUM_VDEC_FMT_T eCodecType;	
    ENUM_VDEC_PROFILE_T eProfile;
    ENUM_VDEC_LEVEL_T eLevel;
    UINT32 u4BitRate;
    UINT32 u4FailCnt;
    UINT32 u4H264IdcInfo;
    UINT32 u4DropFrameCnt;
    //es fifo
    UINT32 u4DisplayQPicCnt;
    UINT32 u4FirstDispPTS;
    UINT32 u4MaxEsCnt;
    UINT32 u4CurEsCnt;
    UINT32 u4mvcEsCnt;
    UINT32 u4Profile;
    UINT32 u4Level;
    UINT32 u4IsCABAC;
    UINT32 u4Dbk1PixelCnt;
    UINT32 u4Dbk2PixelCnt;
    UINT32 u4Dbk3PixelCnt;
    //mpeg2
    UINT32 u4VbvSizeExt;
    UINT32 u4ChromaFmt;
    #ifdef CC_REALD_3D_SUPPORT
    UINT32 u13DUsrCtrlMode;
    UINT32 fgB2R3DEnable;
    UINT32 fgB2RForce2D;
    UINT32 e3DType;
    UINT32 u1B2R3DType;
    #endif
    //~mpeg2
    UINT16 u2OrgHeight;
    UINT16 u2OrgWidth;
    UINT16 u2Height;
    UINT16 u2Width;
    UINT16 u2DHS;
    UINT16 u2DVS;
    UINT16 u2FrmRate;
    UINT16 u2TempRef;
    UINT8 u1CurState;
    UCHAR ucVideoFmt;
    UCHAR ucPicType;
    UCHAR ucPicStruct;
    UCHAR ucAfd;
    UCHAR ucFbNum;
    //mpeg2
    UCHAR ucProfileId;
    UCHAR ucColorPrimaries;
    UCHAR ucTransferCharacteristics;
    UCHAR ucMatrixCoefficients;
    //~mpeg2
    BOOL fgProgressiveSeq;
    BOOL fgTFF;
    BOOL fgRFF;
    BOOL fgProgressiveFrm;
    BOOL fgHdrInvalid;
    BOOL fgNoOverScan;
    BOOL fgDeblocking;
    //mpeg2
    BOOL fgColorDescrip;
    BOOL fgContrained;
    BOOL fgMPEG2; // mpeg1 or mpeg2
    BOOL fgSeqDispExt;
    BOOL fgLowDelay;
    BOOL fgPixelAspectRatio;
    //~mpeg2
    //H264 PVR related
    BOOL fgSPSValid;
    BOOL fgVUIValid;
    BOOL fgSEIValid;
    BOOL fgSPSFrameMbsOnlyFlag;
    UINT32 u4VUITimeScale;
    UINT32 u4VUINumUnitsInTick;
    VDEC_AVC_SEI_PIC_STRUCT eSEIPicStruct;
    //~H264 PVR related
    //MPEG2 PVR related
    BOOL fgSeqValid;
    UINT16 u2SeqFrmRate;
    //end MPEG2 PVR related
    //cropping info
    BOOL fgCropping;
    UINT32 u4CropX;
    UINT32 u4CropY;
    UINT32 u4CropWidth;
    UINT32 u4CropHeight;
    UINT32 u4BitDepth;
    //3D full resolution support
    BOOL fgB2rWidthLimitation;
    UINT16 u2B2rDispWidth;
    //For seamless play zoom
    BOOL   fgResized;
    UINT16 u2SourceHeight;
    UINT16 u2SourceWidth;
    VDEC_ASPECT_RATIO_T eSourceAspectRatio;
    UINT32 u4SourcePixelAspectRatioWidth;
    UINT32 u4SourcePixelAspectRatioHeight;   

    // stagefright
    BOOL fgVPushFBGFromInst;

    //for Freeview HD subtitle sync
    UINT32 u4DelayTime;
    UINT32 u4SliceQP;
} VDEC_HDR_INFO_T;

typedef struct _VDEC_WFD_DECODE_ERROR_T
{
    INT32 i4TimeoutCnt;
    INT32 i4LackSeqHeaderCnt;
    INT32 i4OutOfSpecCnt;
}VDEC_WFD_DECODE_ERROR_T;

typedef struct
{
    UINT32 u4SrcAddr;
    UINT32 u4TgtYAddr;
    UINT32 u4TgtCAddr;
    UINT32 u4PWidth;
    UINT32 u4PHeight;
    UINT32 u4YPitchSize;
    UINT32 u4UVPitchSize;
} VDEC_YUV_RESIZE_INFO_T;

typedef struct 
{
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucRzFbId;
} VDEC_I_RESIZE_INFO_T;

typedef struct {
	BOOL fgChromaLocationInfoPresentFlag;
	UINT32 u4ChromaFormatIdc;
	UINT32 u4ChromaSampleLocTypeTopField;
	UINT32 u4ChromaSampleLocTypeBottomField;
} VDEC_CHROMA_SIMPLE_INFO_T;


typedef void (*PFN_VDEC_EVENT_NOTIFY)(
    UINT32 u4Arg1,
    UINT32 u4Arg2,  // ucEsId
    UINT32 u4Arg3,
    UINT32 u4Arg4
);

/**This notification function is called by the video decoder in response to some events.
*/
typedef void (*PFN_VDEC_DEC_NFY_FCT) (
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2
);

typedef void (*PFN_VDEC_NOT_SUPPORT_NFY_FCT) (
    UCHAR              ucEsId,
    BOOL                 fgSupport
);


/**This structure contains a video decoders notify function and tag.
*/
typedef struct
{
    void*  pvTag;
    PFN_VDEC_DEC_NFY_FCT  pfDecNfy;
}VDEC_DEC_NFY_INFO_T;


typedef struct {
    UINT64 u8Scale64;
    UINT64 u8Scale;
    UINT32 u4Rate64;
    UINT32 u4Rate;
    UINT64 u8Pts; //extension for 64bit PTS.
} VDEC_PTS_INFO_T;

/**
 *  VDEC position information
 */
typedef struct
{
    UINT64 u8PtsI;
    UINT64 u8Pts;

    UINT64 u8AudPts;
    UINT64 u8AudOffset;

    UINT64 u8OffsetI;
    UINT64 u8Offset;
    UINT64 u8OffsetDisp;
    INT32  i4TemporalRef;
    UINT16 u2DecodingOrder;
} VDEC_POS_INTO_T;

/**
 *  VDP AB position information
 */
typedef struct
{
    BOOL fgValid;
    BOOL fgPosition; // use position or pts.
    VDEC_POS_INTO_T rA;
    VDEC_POS_INTO_T rB;
} VDEC_AB_INTO_T;

typedef struct _VDEC_MODELOG_INFO_T
{
    UINT32  u4LogLevel;
    //VID_DEC_CTRL_T  eDecCtrl;
    BOOL    fgDecModeFin;
    UINT16  u2DecMode;
    //VID_DEC_SPEED_TYPE_T    e_speed_type;
    UINT16  u2DesiredWidth;
    UINT16  u2DesiredHeight;
    UINT16  u2OriginalWidth;
    UINT16  u2OriginalHeight;
    BOOL    fgDecChecksumOpen;
}VDEC_MODELOG_INFO_T;

typedef enum
{
    VDEC_PARAM_MIN                   = 0,           // LWB: If value in [0, 0x7FFF], for special codec
    // Note: Please add your set info. here for special codec
    
    VDEC_COM_PARAM_MIN               = 0x8000,
    VDEC_COM_PARAM_LOG_CTRL          = 0x8000       // LWB: >= 0x8000, use in VDEC commom part  
    // Note: Please add your set info. here for commom codec part
} ENUM_VDEC_PARAM_INFO_T;

#define VDEC_PRINT_RSV_INFO             0x1
#define VDEC_PRINT_PQ_INFO              0x2
#define VDEC_DEBUG_INFO                 0x4
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize VDEC module.
*/
EXTERN void VDEC_Init(void);

/** Terminate VDEC module.
*/
EXTERN void VDEC_Termint(void);

#if 0
/** Reset VDEC module.
*/
EXTERN void VDEC_Reset(void);
/** Change to PLAY mode.
*@param u4Fmt.
 */
EXTERN void VDEC_PlayVideoClip(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);
/** Flush decoded picture buffer.
*@param.
 */
EXTERN void VDEC_Flush_DPB(UCHAR ucEsId);
/** Get parameter.
*@param .
 */
EXTERN BOOL VDEC_IsPlayMM(UCHAR ucEsId);
/** Get parameter.
*@param .
 */
EXTERN BOOL VDEC_IsPlayNotDisplay(UCHAR ucEsId);
EXTERN VOID VDEC_SetResizeInfo(UCHAR ucEsId, BOOL fgResizeDispFrm, UCHAR ucRzID);
EXTERN BOOL VDEC_GetFrmNumInEsmQ(UCHAR ucEsId, UINT32* pu4FrmNumInQueue);
#endif
/** Change to PLAY mode.
*@param u4Fmt.
 */
EXTERN BOOL VDEC_Play(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);
EXTERN BOOL VDEC_GetVideoThumbnailDirect(UCHAR ucEsId, VDP_THUMBNAIL_INFO_T* prDstInfo,
   UINT32 u4WorkingBufAddr, UINT32 u4WorkingBufSize, BOOL fgKeepRation);

/** Change to PLAY mode.
*@param u4Fmt.
 */
EXTERN void VDEC_RePlayVdec(UCHAR ucEsId);

/** Change to PLAY I frame mode.
*@param u4Fmt.
 */
EXTERN void VDEC_Play_I_Frame(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);

/** Change to STOP mode.
*@param.
 */
EXTERN void VDEC_Stop(UCHAR ucEsId);

/** Change to PAUSE mode.
*@param.
 */
EXTERN void VDEC_Pause(UCHAR ucEsId);

/** Set Sync mode.
*@param.
 */
EXTERN void VDEC_SyncStc(UCHAR ucEsId, UCHAR ucMode, UCHAR ucStcId);

/** Register notify function.
*@param.
 */
EXTERN void VDEC_RegEventNotifyFunc(PFN_VDEC_EVENT_NOTIFY pfFunc);

/** Set decode notify function.
*@param ucEsId                  ES id.
*@param prDecNfyInfo            Video decoders notify function and tag.
*@retval VDEC_DRV_OK            Success.
*@retval VDEC_DRV_INV_SET_INFO  Fail.
*/
EXTERN INT32 VDEC_SetDecNfy(UCHAR ucEsId, const VDEC_DEC_NFY_INFO_T* prDecNfyInfo);

EXTERN INT32 VDEC_SetFrameToUser(UCHAR ucEsId, BOOL fgFrameToUser);

EXTERN INT32 VDEC_SetVdecNotSupportNfy(UCHAR ucEsId, const PFN_VDEC_NOT_SUPPORT_NFY_FCT prVdecNtyNotSupport);

/** Query status.
*@param .
 */
EXTERN BOOL VDEC_QueryInfo(UCHAR ucEsId, VDEC_HDR_INFO_T* prHdrInfo);

EXTERN BOOL VDEC_QueryChromaInfo(UCHAR ucEsId, VDEC_CHROMA_SIMPLE_INFO_T* prChromaSimpleInfo);

/** Query status.
*@param .
 */
EXTERN BOOL VDEC_GetGnlInfo(UCHAR ucEsId, VDEC_GNL_INFO_T* prGnlInfo);

/** Set parameter.
*@param .
 */
EXTERN void VDEC_SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3);

/** Get parameter.
*@param .
 */
EXTERN void VDEC_GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3);

#ifdef CC_MAPLE_CUST_DRV
EXTERN void VDEC_GetDataSizePerSecond(UCHAR ucEsId, UINT32* pu4Param1);
#endif
/** Get parameter.
*@param .
 */
EXTERN BOOL VDEC_IsLock(UCHAR ucEsId);

/** Get parameter.
*@param .
 */
EXTERN void VDEC_PlayNotDisplay(UCHAR ucEsId, BOOL fgNotDisplay);

/** Get parameter.
*@param .
 */
EXTERN void VDEC_PlayMM(UCHAR ucEsId, BOOL fgPlayMM);

/** Set MM related parameter.
*@param .
 */
EXTERN void VDEC_SetMMParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3);
/** Clr MM related parameter.
*@param .
 */
EXTERN void VDEC_ClrMMParam(UCHAR ucEsId, UINT32 u4Type);

/** Set  Pvr  parameter.
*@param .
 */
EXTERN BOOL VDEC_SetWaitPvrSps(UCHAR ucEsId, UINT8 u1PVRWaitSps);

/** Set  Pvr  parameter.
*@param .
 */
EXTERN void VDEC_SetTPTFlag(UCHAR ucEsId, BOOL fgIsTPT);

/** Set BigB related parameter.
*@param .
 */
EXTERN void VDEC_SetBigBParameter(UCHAR ucEsId, BOOL fgForceEnable, UINT32 u4BigBLevel1,
    UINT32 u4BigBLevel2, UINT32 u4BigBLevel3, UINT32 u4BigBBaseCnt);

/** Set MM related parameter.
*@param .
 */
EXTERN VOID VDEC_GetQueueInfo(UCHAR ucEsId,UINT16* pu2QueueSize,UINT16* pu2MaxQueueSize);

/** Get video display frame address.
*@param .
 */
EXTERN BOOL VDEC_GetDisplayFrameBufferAddr(UCHAR ucEsId, VDP_CAPTURE_INTO_T* prCapInfo);

/** Get video thumbnail.
*@param .
 */
EXTERN BOOL VDEC_GetVideoThumbnail(UCHAR ucEsId, UINT16 u2CompId, BOOL bIsOnePass,
    VDP_THUMBNAIL_INFO_T* prDstInfo, VDP_CAPTURE_INTO_T* prSrcInfo, BOOL fgKeepRation);

/** Get video thumbnail2.
*@param .
 */
EXTERN BOOL VDEC_GetVideoThumbnail2(UCHAR ucEsId, VDP_THUMBNAIL_INFO_T* prDstInfo,
    UINT32 u4WorkingBufAddr, UINT32 u4WorkingBufSize, BOOL fgKeepRation);

/** Stop video thumbnail2.
*@param .
 */
EXTERN BOOL  VDEC_StopVideoThumbnail2(UCHAR ucEsId);

/** Get video thumbnail3.
*@param .
 */

EXTERN BOOL  VDEC_GetVideoThumbnail3(UCHAR ucEsId, UINT16 u2CompId, BOOL bIsOnePass,
    VDP_THUMBNAIL_INFO_T* prDstInfo,  BOOL fgKeepRation);

/** Check video type capability.
*@param .
 */
EXTERN BOOL VDEC_ChkCodecCap(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);


/** Check 4k2k capability.
*@param .
 */
#if defined(VDEC_4K_DEC_CHECK)
EXTERN BOOL VDEC_Chk4K2KCap(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt);
#endif

/** Check DivX bonding.
*@param .
 */
EXTERN BOOL VDEC_ChkDivXBonding(VOID);

/** VDEC notify status to MW.
*@param .
 */
EXTERN BOOL VDEC_Notify(UCHAR ucEsId,VDEC_DEC_DECODE_STATUS_T notifyStatus);

/** MW set swdmx instance ID to vdec.
*@param .
 */
EXTERN VOID  VDEC_SetSrcId(UCHAR ucEsId, UINT8 u1SwdmxInstId );

EXTERN VOID VDEC_RmSetRenderVdp(UCHAR ucEsId, UINT32 u4VDPId);

#ifdef CC_VDEC_RM_SUPPORT
EXTERN VOID VDEC_RmSetPriority(UCHAR ucEsId, UINT32 u4Priority);

EXTERN VOID VDEC_RmSetKickDone(UCHAR ucEsId, BOOL fgSuccess);

EXTERN VOID VDEC_RmSetIsVdpRender(UCHAR ucEsId, BOOL fgIsVDPRender);

EXTERN VOID VDEC_RmSetImgrzIdMsk(UCHAR ucEsId, UCHAR ucImgrzId);

EXTERN VOID VDEC_RmGetImgrzIdMsk(UCHAR ucEsId, UCHAR *pucImgrzId);

#ifdef CC_INPUTSOURCE_VIDEO_ON_TEXTURE
EXTERN UCHAR VDEC_RmGetEsByImgrz(UCHAR ucImgrzId);

EXTERN VOID VDEC_RmSetTextureMode(UCHAR ucEsId);
#endif

#endif
EXTERN BOOL VDEC_ChkSeamlessModeChg(UCHAR ucEsId, UINT32 u4W, UINT32 u4H);

/** MW set swdmx AB info to vdec.
*@param .
 */
EXTERN VOID VDEC_SetABInfo(UCHAR ucEsId, VDEC_AB_INTO_T *prABInfo);
#ifdef CC_VDEC_PRIORITY_ENABLE
EXTERN VOID VDEC_WakeUpPendingThread(UCHAR ucEsId);
EXTERN BOOL VDEC_SetPriority(UCHAR ucEsId,INT16 Priority);
EXTERN BOOL VDEC_ThreadSchedule(UCHAR ucEsId);
#endif
EXTERN VOID VDEC_SetH264PVR(UCHAR ucEsId, UINT32 u4PVRMode);
EXTERN void VDEC_SetH264MMErrMask(UCHAR ucEsId, BOOL fgMMErrMask);
EXTERN VOID  VDEC_SetMVCCmpare(UCHAR ucEsId, BOOL fgParam);
#ifdef CC_REALD_3D_SUPPORT
/** set 3D display control by user
*@param u1Param EsId
*@param u1Param 3D mode (0: VDEC_3D_CTRL_OFF, 1: VDEC_3D_CTRL_FORCE_2D,
*                                          2: VDEC_3D_CTRL_FORCE_3D_LR, 3: VDEC_3D_CTRL_FORCE_3D_TB,
                                            4: VDEC_3D_CTRL_BYPASS, 5: VDEC_3D_CTRL_FORCE_2D_LR,
                                            6: VDEC_3D_CTRL_FORCE_2D_TB)
 */
EXTERN void VDEC_Set3DUserCtrl(UCHAR ucEsId, UINT8 u1Param);

#endif

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT

EXTERN VOID  VDEC_SetCrcGolden(UCHAR ucEsId, UINT32 uCrcGolden);
EXTERN INT32 VDEC_SetAutoTestDecNfy(UCHAR ucEsId, UINT32 prDecNfy, void *pvTag);

#ifdef VDEC_TIME_PROFILE
EXTERN  VOID VDEC_SetAutoTestCurFileDir(UCHAR ucEsId, CHAR *ucAutoPlayCurFileDir);
EXTERN  VOID VDEC_SetAutoTestCurFileName(UCHAR ucEsId, CHAR *ucAutoPlayCurFileName);
#endif

#endif

#ifdef VDEC_CRC_TEST
EXTERN BOOL VDEC_IsTestCRC(UCHAR ucEsId);
EXTERN VOID VDEC_SetTestCRC(UCHAR ucEsId, BOOL fgTestOrNot);
#endif
#ifdef CC_SUPPORT_STR
EXTERN BOOL VDEC_CkgenOnOff(BOOL fgEnable);
#endif
/** VDEC set resize/seamless resolution.
*@param .
 */
EXTERN BOOL VDEC_SetSkipMode(UCHAR ucEsId, BOOL fgSkipMode);
#ifdef CC_ITIMING_ALWAYS
EXTERN void VDEC_SetFixFHDDisplay(UCHAR ucEsId, BOOL fgInterlace);
#endif

EXTERN UINT32 VdecVdpClr2GfxClr(VDP_COLORMODE_T vdp_clr);
EXTERN UINT32 VdecCodecChangeHandle(UCHAR ucEsId);

/** 
   Tell VDEC flush ES Q and notify no data information to upper layer
 */
EXTERN BOOL VDEC_SuperFlush(UCHAR ucEsId);

/** Tell vdec to re-set start audio PTS
 *  @param:
 *  ucEsId: src id
 *  fgEnable: re-set or not
 */
EXTERN BOOL  VDEC_SetRestartAudioPts(UCHAR ucEsId, BOOL fgEnable);

/** find valid VDEC src, and lock it for use
 * param:
 * pucEsId: (InOut) input the expected src id. output the valid src id
 */
EXTERN BOOL VDEC_TryLockValidSrc(UCHAR *pucEsId);

/** Release VDEC src
 * param:
 * ucEsId: (In) Src id should be released
 */
EXTERN BOOL VDEC_UnlockSrc(UCHAR ucEsId);
EXTERN BOOL VDEC_ISH264_MVC(void);

/**
    Add for DTV MHEG5 I frame case. 
    Audio playing abnormally when VDEC in play I frm status: audio will do Av sync when rsv video mode change. In fact, we should not do it.
 */
BOOL VDEC_IsExistDtvIFrmCase(VOID);

/** force stop if necessary
 * ex. browser(omx) -> mmp(cmpb), vdec might not be released by omx
 *@param u1Param EsId
 */
BOOL VDEC_NotifyStop(UCHAR ucEsId);

EXTERN VOID VDEC_YUV2YCbCr_Resize(VDEC_YUV_RESIZE_INFO_T *pYUVRezInfo);

EXTERN void VDEC_INTER_RESIZE(VDEC_I_RESIZE_INFO_T *ResizeFRMT,BOOL fgRepeatTop);

EXTERN BOOL VDEC_QueryWFDErrInfo(UCHAR ucEsId, VDEC_WFD_DECODE_ERROR_T* prErrInfo);

EXTERN void VDEC_ResetWFDInfo(UCHAR ucEsId);

EXTERN BOOL VDEC_GetDelayTime(UCHAR ucEsId, VDEC_HDR_INFO_T* prHdrInfo);
EXTERN void VDEC_VP9LogOption(const CHAR* pTypeStr, const CHAR* pArg1, const CHAR* pArg2);

EXTERN void VDEC_GetHALParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32* u4Param2, UINT32 u4Param3);

#endif //VDEC_IF_H
