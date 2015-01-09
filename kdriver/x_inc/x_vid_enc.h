/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, MediaTek Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.  
 *-----------------------------------------------------------------------------
 * $RCSfile: x_vid_enc.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Personal Video Recorder driver exported definitions.
 *---------------------------------------------------------------------------*/

#ifndef _X_VID_ENC_H_
#define _X_VID_ENC_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_rm.h"
#include "x_mm_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------------*/

/* Get operations */
#define VID_ENC_GET_TYPE_CTRL              ( RM_GET_TYPE_LAST_ENTRY + ( ( DRV_GET_TYPE_T )  0 ) )


/* Set operations */
#define VID_ENC_SET_TYPE_CTRL              ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 0 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_SETTINGS          ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 1 ) )
#define VID_ENC_SET_TYPE_FRAME_RESOLUTION  ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 2 ) )
#define VID_ENC_SET_TYPE_FRAME_RATE        ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 3 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_BITRATE           ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 4 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_PROFILE           ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 5 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_LEVEL             ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 6 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_APP_TYPE          ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 7 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_RESTART           ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 8 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_INSERT_KEYFRAME   ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 9 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_ENABLE_OSTG       ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 10 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_ENCODE_OSD        ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 11 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define VID_ENC_SET_TYPE_INPUT_SOURCE      ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 12) ) 
#define VID_ENC_SET_TYPE_ENCODE_FIELD      ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T) 13 ) ) | RM_SET_TYPE_ARG_NO_REF )

/*for VID_ENC_FLD_TYPE_T*/
typedef enum
{
    VID_ENC_FLD_NORMAL = 0,
    VID_ENC_TOP_FLD,
    VID_ENC_BOTTOM_FLD,
} VID_ENC_FLD_TYPE_T;


/* Note refer to MUX_UTIL_VID_SRC_TYPE_T*/
typedef enum
{
    VID_ENC_SRC_TYPE_VDEC = 0, /*For Internal skype*/
    VID_ENC_SRC_TYPE_VDP,      /*for mt5396: support MSI, ATV PVR, ATV Backgorund PVR*/
} VID_SRC_TYPE_T;

/*for VID_ENC_SET_TYPE_APP_TYPE*/
typedef enum
{
    VID_ENC_AP_TYPE_SKYPE = 0,
    VID_ENC_AP_TYPE_ATVPVR,
    VID_ENC_AP_TYPE_MSI,  // multi-screen interaction, for mt5396
    VID_ENC_AP_TYPE_BGPVR,
} VID_AP_TYPE_T;

typedef enum
{
    VID_ENC_FMT_H264 = 0
    /* Others */
} VID_ENC_FMT_TYPE_T;

typedef enum 
{
    /* H264 */
    VID_ENC_H264_PROFILE_BASELINE = 0,
    VID_ENC_H264_PROFILE_MAIN 
    /* Others */
} VID_ENC_PROFILE_T;


typedef enum 
{
    /* H264 */
    VID_ENC_H264_LEVEL_21 = 0,
    VID_ENC_H264_LEVEL_31,
    VID_ENC_H264_LEVEL_41    
    /* Others */
} VID_ENC_LEVEL_T;

typedef struct _VID_ENC_INPUT_SOURCE_T
{
    DRV_TYPE_T         e_type;
    UINT16             ui2_comp_id;
}VID_ENC_INPUT_SOURCE_T;


/*-----------------------------------------------------------------------------
                    structures
-----------------------------------------------------------------------------*/
/* for VID_ENC_SET_TYPE_FRAME_RESOLUTION */
typedef struct _VID_FRAME_RESOLUTION_T
{
    UINT32              ui4_width;      /* Resolution: Width */
    UINT32              ui4_height;     /* Resolution: Height */
} VID_FRAME_RESOLUTION_T;

typedef struct _VID_ENC_COMMON_INFO_T
{
    VID_FRAME_RESOLUTION_T  t_resolution;   /*Resolution*/
    UINT32                  ui4_fps;        /* Frame Rate */
    UINT32                  ui4_bitrate;    /* Bitrate */
} VID_ENC_COMMON_INFO_T;

typedef struct _VID_ENC_H264_INFO_T
{
    VID_ENC_COMMON_INFO_T  t_vid_enc_info;
    VID_ENC_PROFILE_T      e_profile;
    VID_ENC_LEVEL_T        e_level;
} VID_ENC_H264_INFO_T;

/* for VID_ENC_SET_TYPE_SETTINGS */
typedef struct _VID_ENC_SETTINGS_T
{
    VID_SRC_TYPE_T              e_src_type; /* Source Type */
    VID_ENC_FMT_TYPE_T          e_enc_fmt;  /* Encode Format Type */
    /* Encoding Setting according to e_enc_fmt */
    union
    {
        VID_ENC_H264_INFO_T     t_fmt_h264;
    }u_fmt;
} VID_ENC_SETTINGS_T;

#endif /* _X_VID_ENC_H_ */

