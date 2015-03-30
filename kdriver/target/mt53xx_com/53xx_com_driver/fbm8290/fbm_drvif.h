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
 *
 * $Author: p4admin $
 * $Date: 2015/03/30 $
 * $RCSfile: fbm_drvif.h,v $
 * $Revision: #10 $

 *
 *---------------------------------------------------------------------------*/

/** @file fbm_drvif.h
 *  API of FBM (Frame Buffer Manager)
 *  Details of file fbm_drvif.h (optional).
 */

#ifndef FBM_DRVIF_H
#define FBM_DRVIF_H


//#define FBM_INHERITANCE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "stc_drvif.h"
#include "x_lint.h"
#include "x_typedef.h"
#include "x_timer.h"
//#include "panel_cust.h"

#if defined(CC_MT5399) ||defined(CC_MT5890)
#define CC_FBM_2ND_CHANNEL
#endif
#if defined(CC_MT5890)
#define CC_FBM_3ND_CHANNEL
#endif

//#define YC_MERGE_IN_CHB 
#ifdef CC_MT5882
#define FBM_MPEG_C_SIZE_IN_CHB (0x900000) // support Y/C in the same Channel 
#else
#define FBM_MPEG_C_SIZE_IN_CHB (0xf00000) // support Y/C in the same Channel 
#endif
#if defined(CC_FBM_SUPPORT_4K2K_SEAMLESS)
#define FBM_MPEG_SEAMLESS_SIZE (0x1B00000) // seamless buff (2FB,27M)
#elif defined(DIVX_PLUS_CER) || defined(CC_NETFLIX_CE3_SUPPORT) || defined(CC_ENABLE_SEAMLESS_FOR_2D)
#define FBM_MPEG_SEAMLESS_SIZE (0x600000) // seamless buff (2FB,6M)
#else
#define FBM_MPEG_SEAMLESS_SIZE (0x0) // seamless buff (0)
#endif

#ifndef CC_SUPPORT_NPTV_SEAMLESS //Enalbe test code, please enable in *.cfg
#define CC_SUPPORT_NPTV_SEAMLESS
#endif


#ifndef NDEBUG
#define FBM_FB_LOG
#endif


#if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#define FBM_4K2K_SUPPORT
#define FBM_4K2K_JPG_SUPPORT  
#define FBM_4K1K_VIDEO_SUPPORT
#if 0//def CC_SUPPORT_4K2K_SEAMLESS
#define CC_FBM_SUPPORT_4K2K_SEAMLESS //4K2K FBM & Two image resizer
#define DUAL_IMAGE_RESIZE
#endif
#endif

#define INVALID_DISPTAG ((UINT32)-1)
#define OMX_PENDING_DISPTAG ((UINT32)0)


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

// Video Plane Full ISR
// For DTV00007209, Tearing
//#define CC_VDP_FULL_ISR

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// Frame Buffer Alignment
#define FBM_ALIGN_MASK(value, mask)     ((value + mask) & (~mask))
typedef enum
{
    FBM_TYPE_DMX,
    FBM_TYPE_MPEG,
    FBM_TYPE_FEEDER,
    FBM_TYPE_B2R,
    FBM_TYPE_JPEG,
    FBM_TYPE_TZ_DMX,
    FBM_TYPE_TOTAL,
} FBM_TYPE_T;

/** Brief of FBM_POOL_TYPE_T
 *  FBM Memory Pool Index
 */
typedef enum
  {
    ColorSpace_NS,
    ColorSpace_sRGB,
    ColorSpace_sYCC,
    ColorSpace_AdobeRGB,
    ColorSpace_Normal,
    ColorSpace_XVColor,
    ColorSpace_BT202C,
    ColorSpace_BT2020
  } FBM_COLOR_SPACE_ID;

typedef enum
{
    FBM_POOL_TYPE_DMX,
    FBM_POOL_TYPE_TDC,
    FBM_POOL_TYPE_TDC_2,
    FBM_POOL_TYPE_VBI,
    FBM_POOL_TYPE_VBI_2,
    FBM_POOL_TYPE_MPEG,
    FBM_POOL_TYPE_MJC_1,
    FBM_POOL_TYPE_MPEG_TV,
    FBM_POOL_TYPE_MPEG_NET,
    FBM_POOL_TYPE_SCPOS_MAIN,//
    FBM_POOL_TYPE_DMX_TV,
    FBM_POOL_TYPE_DMX_NET,
    FBM_POOL_TYPE_FEEDER_NET,
    FBM_POOL_TYPE_SCPOS_PIP,
    FBM_POOL_TYPE_FEEDER,
    FBM_POOL_TYPE_FEEDER_MMP,
    FBM_POOL_TYPE_MDDI_1,
    FBM_POOL_TYPE_PQ_MM,
    FBM_POOL_TYPE_PQ_NET,
    FBM_POOL_TYPE_PQ_TV,
    FBM_POOL_TYPE_NET_SWAP,
    FBM_POOL_TYPE_TDC_DYN,
    FBM_POOL_TYPE_SCPOS_DYN_MAIN,
    FBM_POOL_TYPE_MDDI_DYN_MAIN,
    FBM_POOL_TYPE_SCPOS_DYN_SUB,
    FBM_POOL_TYPE_MDDI_DYN_SUB,
    FBM_POOL_TYPE_MDDI_PIP,
    FBM_POOL_TYPE_NR_1,
    FBM_POOL_TYPE_DSP,
    FBM_POOL_TYPE_B2R,
    FBM_POOL_TYPE_JPEG,
    FBM_POOL_TYPE_JPEG_OSD1,
    FBM_POOL_TYPE_JPEG_OSD2,
    FBM_POOL_TYPE_JPEG_OSD3,
    FBM_POOL_TYPE_JPEG_OSD4,
    FBM_POOL_TYPE_PQ_TOOL,//
    FBM_POOL_TYPE_OD,
    FBM_POOL_TYPE_VENC,
    FBM_POOL_TYPE_SWDMX,
    FBM_POOL_TYPE_FACEDET,
    FBM_POOL_TYPE_MPEG_FREE,
    FBM_POOL_TYPE_DIVA,
    FBM_POOL_TYPE_FEEDER2,
    FBM_POOL_TYPE_DMX2,
    FBM_POOL_TYPE_MPEG2,
    FBM_POOL_TYPE_PVR,
    FBM_POOL_TYPE_PVR2,
    FBM_POOL_TYPE_JPG_VDP,
    FBM_POOL_TYPE_PNG,
    FBM_POOL_TYPE_DIVX,
    FBM_POOL_TYPE_MPEG3,//
    FBM_POOL_TYPE_VSS,
    FBM_POOL_TYPE_DMXPID,
    FBM_POOL_TYPE_IMGRZ_3D_KR,
    FBM_POOL_TYPE_DRM,
    FBM_POOL_TYPE_MUXER,
    FBM_POOL_TYPE_PQ_3DC,
    FBM_POOL_TYPE_PQ_PSCAN,
    FBM_POOL_TYPE_SUBTITLE,
    FBM_POOL_TYPE_DMX3,
    FBM_POOL_TYPE_FEEDER3,
    FBM_POOL_TYPE_EDB,
    FBM_POOL_TYPE_TVE,
    FBM_POOL_TYPE_4K_2K_SCALER,
    FBM_POOL_TYPE_DSP_BIN,
    FBM_POOL_TYPE_PHOTO_SHARE,
    FBM_POOL_TYPE_BEIC,
    FBM_POOL_TYPE_MPEG4,//
    FBM_POOL_TYPE_FEEDER4,
    FBM_POOL_TYPE_DMX4,
    FBM_POOL_TYPE_MPEG_C,
    FBM_POOL_TYPE_MPEG2_C,
    FBM_POOL_TYPE_MPEG3_C,
    FBM_POOL_TYPE_MPEG4_C,
    FBM_POOL_TYPE_W2D_R2R,
    FBM_POOL_TYPE_W2D_R2R_Y,
    FBM_POOL_TYPE_SCPOS_PIP2,
    FBM_POOL_TYPE_SRS,
    FBM_POOL_TYPE_REMUX,
    FBM_POOL_TYPE_HW_DEMOD,
    FBM_POOL_TYPE_SECURE_FEEDER,
    FBM_POOL_TYPE_TOTAL2,
    FBM_POOL_TYPE_TOTAL3,
    FBM_POOL_TYPE_TOTAL,
    FBM_POOL_TYPE_NS,
    FBM_POOL_TYPE_NONE = 0xFF
} FBM_POOL_TYPE_T;

typedef enum
{
#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
    FBM_POOL_TYPE_TZ_DMX,
#if defined(CC_VOMX_TV_COEXIST) || defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)
    FBM_POOL_TYPE_TZ_DMX2,
#endif
#endif
    FBM_POOL_TYPE_TZ_NS,
    FBM_POOL_TYPE_TZ_NONE = 0xFF
} FBM_POOL_TYPE_TZ_T;


#define FBM_POOL_TYPE_NR FBM_POOL_TYPE_NR_1
#define FBM_POOL_TYPE_MJC FBM_POOL_TYPE_MJC_1
#define FBM_POOL_TYPE_MDDI_MAIN FBM_POOL_TYPE_MDDI_1

#define FBM_POOL_MODE_8BIT                                              (1 << 0)
#define FBM_POOL_MODE_9BIT                                              (1 << 1)
#define FBM_POOL_MODE_10BIT                                             (1 << 2)
#define FBM_POOL_MODE_C_10BIT                                           (1 << 3)
#define FBM_POOL_MODE_420_FB                                            (1 << 4)
#define FBM_POOL_MODE_422_FB                                            (1 << 5)
#define FBM_POOL_MODE_444_FB                                            (1 << 6)
#define FBM_POOL_MODE_SD                                                (1 << 8)
#define FBM_POOL_MODE_720HD                                             (1 << 9)
#define FBM_POOL_MODE_1080HD                                            (1 << 10)
#define FBM_POOL_MODE_EPG                                               (1 << 11)
#define FBM_POOL_MODE_SHARE                                             (1 << 13)
#define FBM_POOL_MODE_TOGGLE_TRIGGER                                    (1 << 14)
#define FBM_POOL_MODE_UNSTABLE                                          (1 << 15)

#define FBM_POOL_MODE_MPEG_NO_COMP                                      (1 << 16)
#define FBM_POOL_MODE_MPEG_COMP_75                                      (1 << 17)
#define FBM_POOL_MODE_MPEG_COMP_50                                      (1 << 18)
#define FBM_POOL_MODE_MPEG_DBK                                          (1 << 19)

#define FBM_POOL_MODE_MJC_DROP                                          (1 << 16)
#define FBM_POOL_MODE_MJC_NO_DROP                                       (1 << 17)
#define FBM_POOL_MODE_MJC_NO_COMP                                       (1 << 18)
#define FBM_POOL_MODE_MJC_Y_COMP4                                       (1 << 19)
#define FBM_POOL_MODE_MJC_Y_COMP3                                       (1 << 20)

#define FBM_POOL_MODE_NR_3D_Y_C                                         (1 << 16)
#define FBM_POOL_MODE_NR_3D_Y_ONLY                                      (1 << 17)
#define FBM_POOL_MODE_NR_2D                                             (1 << 18)
#define FBM_POOL_MODE_NR_BYPASS                                         (1 << 19)

#define FBM_POOL_MODE_SCPOS_1FB                                         (1 << 16)
#define FBM_POOL_MODE_SCPOS_2FB                                         (1 << 17)
#define FBM_POOL_MODE_SCPOS_3FB                                         (1 << 18)
#define FBM_POOL_MODE_SCPOS_DISP                                        (1 << 19)
#define FBM_POOL_MODE_SCPOS_FULL                                        (1 << 20)
#define FBM_POOL_MODE_SCPOS_PSCAN_DISP                                  (1 << 21)
#define FBM_POOL_MODE_SCPOS_MJC_DISP                                    (1 << 22)
//#define FBM_POOL_MODE_SCPOS_MODE_MASK                                   (0xf << 19)
#define FBM_POOL_MODE_SCPOS_MODE_MASK                                   (0x18f << 19)
#define FBM_POOL_MODE_SCPOS_MM_BUFCHG                                   (1 << 23)
#define FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR                              (1 << 24)
#define FBM_POOL_MODE_SCPOS_PROG_DISP                                   (1 << 25)
#define FBM_POOL_MODE_SCPOS_LINESYNC_DISP                               (1 << 26)
#define FBM_POOL_MODE_SCPOS_HYBRID_DISP                                 (1 << 27)   //for gazelle hybrid mode
#define FBM_POOL_MODE_SCPOS_DUAL_HYBRID_DISP                            (1 << 28)   //for gazelle hybrid mode


#define FBM_POOL_MODE_MDDI_BOB                                          (1 << 16)
#define FBM_POOL_MODE_MDDI_FULL                                         (1 << 17)
#define FBM_POOL_MODE_MDDI_CBOB                                         (1 << 18)
#define FBM_POOL_MODE_MDDI_MODEMSK                                      (0x7 << 16)
// used in progressive mode
#define FBM_POOL_MODE_MDDI_NR_Y_C                                       (1 << 19)
#define FBM_POOL_MODE_MDDI_NR_Y_ONLY                                    (1 << 20)//Outdated, should not be used
#define FBM_POOL_MODE_MDDI_NR_OFF                                       (1 << 21)
#define FBM_POOL_MODE_MDDI_NR_MODEMSK                                   (0x7 << 19)
#define FBM_POOL_MODE_MDDI_NR_10BIT                                     (1 << 22)
#define FBM_POOL_MODE_MDDI_COMP                                         (1 << 23)
#define FBM_POOL_MODE_MDDI_DISP                                         (1 << 24)
#define FBM_POOL_MODE_MDDI_P2I                                          (1 << 25)
#define FBM_POOL_MODE_MDDI_MM_BUFCHG                                    (1 << 26)
#define FBM_POOL_MODE_MDDI_HYBRID                                       (1 << 27)
#define FBM_POOL_MODE_MDDI_DUAL_HYBRID                                  (1 << 28)




// TDC mode
#define FBM_POOL_MODE_TDC_FULL                                          (1 << 16)
#define FBM_POOL_MODE_TDC_GAME                                       (1 << 17)

//mjc dram buffer setting
#define FBM_MJC_DRAM_2D                                                 (1<<0)
#define FBM_MJC_DRAM_3D_SG                                              (1<<1)    
#define FBM_MJC_DRAM_3D_HPR                                             (1<<2) 
#define FBM_MJC_DRAM_OSMP_FPR                                           (1<<3)
#define FBM_MJC_DRAM_3D_CFG                                             FBM_MJC_DRAM_OSMP_FPR  //if is l2r2 pdp panel choose FBM_MJC_DRAM_3D_SG

#define FBM_POOL_MODE_MJC_Y10_C10_422_0RDC_3FRM                         (1 << 16)
#define FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM                         (1 << 17)
#define FBM_POOL_MODE_MJC_Y10_C10_420_0RDC_3FRM                         (1 << 18)
#define FBM_POOL_MODE_MJC_Y10_C10_420_1RDC_3FRM                         (1 << 19)
#define FBM_POOL_MODE_MJC_Y10_C08_422_0RDC_3FRM                         (1 << 20)
#define FBM_POOL_MODE_MJC_Y10_C08_422_1RDC_3FRM                         (1 << 21)
#define FBM_POOL_MODE_MJC_Y10_C08_420_0RDC_3FRM                         (1 << 22)
#define FBM_POOL_MODE_MJC_Y10_C08_420_1RDC_3FRM                         (1 << 23)
#define FBM_POOL_MODE_MJC_Y08_C08_422_0RDC_3FRM                         (1 << 24)
#define FBM_POOL_MODE_MJC_Y08_C08_422_1RDC_3FRM                         (1 << 25)
#define FBM_POOL_MODE_FRC_Y10_C10_444                                   (1 << 26)
#define FBM_POOL_MODE_FRC_Y08_C08_444                                   (1 << 27)
#define FBM_POOL_MODE_FRC_Y08_C08_422									(1 << 28) // add a type of y8c8-422 for 4kX 2K 	
//#define FBM_POOL_MODE_MJC_Y08_C08_420_0RDC_3FRM                         (1 << 28) // no 420 FMT for mustang, so mark it and instead by y8c8-422, due to no any other bit  can be used
#define FBM_POOL_MODE_MJC_Y08_C08_420_1RDC_3FRM                         (1 << 29)
#define FBM_POOL_MODE_FRC_Y10_C10_422                                   (1 << 30)

#define FBM_POOL_HYBRID_DRAM_FRAME_NUM        3


#define FBM_TV_MODE_TYPE_NORMAL                                         0
#define FBM_TV_MODE_TYPE_PIP                                            1
#define FBM_TV_MODE_TYPE_POP                                            2


#define FBM_POOL_INHERIT                                                1
#define FBM_POOL_ROOT                                                   0

#define B2R_MAX_HEIGHT                  (2304)
#define B2R_MAX_WIDTH                   (4096)


// Maximum frame buffer number per frame buffer group
#ifdef DEBUG_MAX_FB
#define FBM_MAX_FB_NS_PER_GROUP		64//32
#else
//for H264
//#define FBM_MAX_FB_NS_PER_GROUP     10
//#define FBM_MAX_FB_NS_PER_GROUP     18
#define FBM_MAX_FB_NS_PER_GROUP     44 //22
#endif


#ifdef CC_B2R_RM_SUPPORT

#define  Max_Capability      (2)
#define  Limit_Capability    (1)
#define  No_Capability      (0)
#define  UFD_INPUT        (3840 * 2160) // (0x7E9000)
#define  FHD_INPUT       (1920 * 1088) //(0x1FE000)
#define  MaxB2RNum       2   
#define  B2RHD           0
#define  B2RSD           1
#define  HD_INPUT        (1280 * 720) // (0x7E9000)

typedef enum
{
    VDEC_MPV,
    VDEC_MP4,
    VDEC_H264,
    VDEC_WMV,
    VDEC_H264VER,
    VDEC_MJPEG,
    VDEC_RV,
    VDEC_AVS,
    VDEC_VP6,
    VDEC_VP8,
    VDEC_RAW,
    VDEC_OGG,
    VDEC_JPG,
    VDEC_PNG,
    VDEC_H265,
    VDEC_MAX
}   VdecType_Table;
    
typedef struct
{
    UINT16 u2HActive;
    UINT16 u2VActive;
    UINT16 u2Fps;
    BOOL   fg8Bit;
    BOOL   fgUFOEnable;
    BOOL   fg4K2k;
    VdecType_Table eVdecType;
    UINT16 u2CapabilityLvl;
    UINT16 u2BlockMode;
    UCHAR  ucEsId;
}B2R_RM_T;
    
    
typedef struct
{
    B2R_RM_T NB2R_RM[MaxB2RNum];
    B2R_RM_T OldB2R_RM[MaxB2RNum];
    BOOL    ufgOldB2RStatus;
    BOOL    ufgNewB2RStatus;
    BOOL    fgconflict;
    UINT16  u2ConflictVdpid;
}B2R_Capability_Map;
    
typedef enum
{
    Old_B2R0 =0,
    Old_B2R1,
    New_B2R0,
    New_B2R1,
    B2R_NULL
}B2R_Table;
#endif

#ifdef CC_B2R_RES_SUPPORT

#define VDP_MAIN  0
#define VDP_SUB   1
#define VDP_MAX   2

#define GRP_1     0
#define GRP_2     1
#define GRP_MAX   2

#define  FHD_TIMING    (1920 * 1088)
#define  UHD_TIMING    (4096 * 4096)

#define B2R_HW_MAX_ID 4

#define RES_R    (1 << 0)
#define RES_RW   (1 << 1)

typedef enum
{
    VDEC_NONE,
    VDEC_H265,
    VDEC_JPEG,
    VDEC_OTHER
}   CODEC_INFO_T;

typedef struct
{
    BOOL  fgUfoEn;
    UINT8 u1PixDepth;
    UINT8 u1FrmRate;
    UINT32 u4Resolution;
    CODEC_INFO_T eCodec;
}B2R_VD_INFO_T;

typedef struct
{
    UINT8 u1VdpId;
    B2R_VD_INFO_T t_vd;
}B2R_IPT_INFO_T;

typedef struct
{
    UINT8 u1HwId;
    UINT8 u1GrpId;
    UINT8 u1AsscId;
    UINT32 u4MaxRes;
}B2R_HW_CFG_T;

typedef struct
{
    BOOL  b_used;
    UINT8 u1FbgId;
    UINT8 u1VdpId;
}B2R_HW_PRM_T;

typedef struct
{
    B2R_HW_CFG_T t_cfg;
    B2R_HW_PRM_T t_prm;
    B2R_VD_INFO_T t_src;
}B2R_HW_RES_T;
#endif

/** Brief of FBM_POOL_T
 *  FBM Memory Pool Info
 */
typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Mode;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Inherit;
    BOOL fgAlloced;//temp flag for FBM_Alloc/Free
	FBM_POOL_TYPE_T ePoolType;// feeder use it.
} FBM_POOL_T;

typedef struct
{
    UINT8 u1AspectRatioType;
    UINT16 u2AspectHorizSize;
    UINT16 u2AspectVertSize;
} FBM_ASPECT_RATIO_T;

/** Brief of FBM_POOL_MODULE_INFO_T
 *  FBM Memory Pool Info
 */
typedef struct
{
    UINT32 u4SourceType;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Interlace;
    UINT32 u4ColorFormat444;
    UINT32 u4VgaTiming;
    UINT32 u4PipPop;
    UINT32 u4Resolution;
} FBM_POOL_MODULE_INFO_T;

/**
 *  FBM internal buf
 */
typedef struct
{
    UCHAR ucBuf[254];
    UCHAR ucFbgId;
    UCHAR ucFbId;
} FBM_INTERNAL_BUF_T;

//Pool padding pattern
#define FBM_POOL_PADDING                             0xAA
#define FBM_POOL_PADDING_SIZE                        0x8

// Frame Buffer Group Id
#define FBM_FBG_ID_UNKNOWN                           0xFF

// Frame Buffer Id
#define FBM_FB_ID_UNKNOWN                            0xFF

// Frame Buffer Group Type
#define FBM_FBG_TYPE_UNKNOWN                         0xFF
#define FBM_FBG_TYPE_1080HD                          0
#define FBM_FBG_TYPE_720HD                           1
#define FBM_FBG_TYPE_PAL                             2
#define FBM_FBG_TYPE_NTSC                            3
#define FBM_FBG_TYPE_1080HD_RR                       17      // RR
#define FBM_FBG_TYPE_720HD_RR                        18      // RR
#define FBM_FBG_TYPE_PAL_RR                          19      // RR
#define FBM_FBG_TYPE_SBSHD                           21
#define FBM_FBG_TYPE_TNBHD                           22
#define FBM_FBG_TYPE_4K2K_JPEG                       25   //FBM_SUPPORT_4K2K
#define FBM_FBG_TYPE_4K1K_VIDEO                      26   //FBM_SUPPORT_4K2K
#define FBM_FBG_TYPE_4K2K                            27   //FBM_SUPPORT_4K2K
#define FBM_FBG_TYPE_4K2K_EXTRA                      28   //FBM_SUPPORT_4K2K

// Frame Buffer Alignment
#define FBM_FMG_Y_ALIGMENT                          (8192 - 1)
#define FBM_FMG_C_ALIGMENT                          (8192 - 1)
#define FBM_MPV_Y_ALIGMENT                          (8192 - 1)
#define FBM_MPV_C_ALIGMENT                          (8192 - 1)
#define FBM_GFX_Y_ALIGMENT                          (8192 - 1)
#define FBM_GFX_C_ALIGMENT                          (8192 - 1)

#define FBM_VDEC_WORKING_ALIGNMENT                  (8192 - 1)

#define FBM_B2R_H_PITCH                             (64-1)
#define FBM_MPV_V_ALIGMENT                          (32-1)

// Frame Buffer Line Size
#define FBM_FBG_SBSHD_H                              3840
#define FBM_FBG_SBSHD_V                              1088
#define FBM_FBG_TNBHD_H                              1920
#define FBM_FBG_TNBHD_V                              2176
#define FBM_FBG_1080HD_V                             1088
#define FBM_FBG_1080HD_H                             1920
#define FBM_FBG_1080HD_V                             1088
#define FBM_FBG_720HD_H                              1280
#define FBM_FBG_720HD_V                               768
#define FBM_FBG_PAL_H                                 768
#define FBM_FBG_PAL_V                                 576
#define FBM_FBG_NTSC_H                                768
#define FBM_FBG_NTSC_V                                480
#define FBM_FBG_4K2K_H                               4096   //FBM_SUPPORT_4K2K
#define FBM_FBG_4K2K_V                               2304    //?? 

#define FBM_NORMAL_FB_NS                              5 // 4, for b2r lock 2 fb,add 1!
#define FBM_DBK_FB_NS                                 5

#define FBM_LOCK_FB_NS                                (2)

#define FBM_NPTV_SEAMLESS_KEEP_CNT  (2)

// H.264 Reference Information
#define FBM_FBG_H264_MVC_FHD_REFERNCE_NS            (8)     // Reference Frame Number for 1920x1088
#define FBM_FBG_H264_MVC_REFERNCE_MAX_NS            (32)
#ifdef CC_H264_LV5_SUPPORT
#define FBM_FBG_H264_FHD_REFERNCE_NS                (8)     // Reference Frame Number for 1920x1088
#else
#define FBM_FBG_H264_FHD_REFERNCE_NS                (6)     // Reference Frame Number for 1920x1088
#endif
#define FBM_FBG_H265_FHD_REFERNCE_NS                (6)     // Reference Frame Number for 1920x1088

#define FBM_FBG_H264_REFERNCE_MAX_NS                (16)
#define FBM_FBG_H264_4K2K_REFERNCE_NS               (6)     // Reference Frame Number for 4k2k
#define FBM_FBG_H265_4K2K_REFERNCE_NS               (6)     // Reference Frame Number for 4k2k

#define FBM_FBG_TYPE_1080HD_H264_R_NS            (FBM_FBG_H264_FHD_REFERNCE_NS)
#define FBM_FBG_TYPE_720HD_H264_R_NS             ((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V * FBM_FBG_H264_FHD_REFERNCE_NS)/(1280 * 720))
#define FBM_FBG_TYPE_PAL_H264_R_NS               ((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V * FBM_FBG_H264_FHD_REFERNCE_NS)/(FBM_FBG_PAL_H * FBM_FBG_PAL_V))

#define FBM_FBG_TYPE_1080HD_H265_R_NS            (FBM_FBG_H265_FHD_REFERNCE_NS)
#define FBM_FBG_TYPE_720HD_H265_R_NS             ((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V * FBM_FBG_H265_FHD_REFERNCE_NS)/(1280 * 720))
#define FBM_FBG_TYPE_PAL_H265_R_NS               ((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V * FBM_FBG_H265_FHD_REFERNCE_NS)/(FBM_FBG_PAL_H * FBM_FBG_PAL_V))


#if defined(CC_H264_LV5_SUPPORT_SHARE_POOL)
#define FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL                           (8)     // Reference Frame Number for 1920x1088
#ifdef CC_H264_LV5_SUPPORT
#error "CC_H264_LV5_SUPPORT shall not coexist with FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL!"
#endif
#endif

#if defined(CC_H264_LV5_SUPPORT_SINGLE_POOL)
#define FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL                           (6)     // Reference Frame Number for 1920x1088
#ifdef CC_H264_LV5_SUPPORT
#error "CC_H264_LV5_SUPPORT shall not coexist with FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL!"
#endif
#endif

// Frame Buffer Size
#define FBM_FBG_TYPE_SBSHD_Y_SIZE                    FBM_ALIGN_MASK((FBM_FBG_SBSHD_H * FBM_FBG_SBSHD_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_SBSHD_C_SIZE                    (FBM_FBG_TYPE_SBSHD_Y_SIZE >> 1)
#define FBM_FBG_TYPE_TNBHD_Y_SIZE                    FBM_ALIGN_MASK((FBM_FBG_TNBHD_H * FBM_FBG_TNBHD_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_TNBHD_C_SIZE                    (FBM_FBG_TYPE_TNBHD_Y_SIZE >> 1)
#define FBM_FBG_TYPE_1080HD_Y_SIZE                    FBM_ALIGN_MASK((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_1080HD_C_SIZE                    (FBM_FBG_TYPE_1080HD_Y_SIZE >> 1)
#define FBM_FBG_TYPE_720HD_Y_SIZE                     FBM_ALIGN_MASK((FBM_FBG_720HD_H * FBM_FBG_720HD_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_720HD_C_SIZE                     (FBM_FBG_TYPE_720HD_Y_SIZE >> 1)
#define FBM_FBG_TYPE_PAL_Y_SIZE                       FBM_ALIGN_MASK((FBM_FBG_PAL_H * FBM_FBG_PAL_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_PAL_C_SIZE                       (FBM_FBG_TYPE_PAL_Y_SIZE >> 1)
#define FBM_FBG_TYPE_NTSC_Y_SIZE                      FBM_ALIGN_MASK((FBM_FBG_NTSC_H * FBM_FBG_NTSC_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_NTSC_C_SIZE                      (FBM_FBG_TYPE_NTSC_Y_SIZE >> 1)
#define FBM_FBG_TYPE_4K2K_Y_SIZE                      FBM_ALIGN_MASK((FBM_FBG_4K2K_H * FBM_FBG_4K2K_V), FBM_FMG_Y_ALIGMENT)
#define FBM_FBG_TYPE_4K2K_C_SIZE                      (FBM_FBG_TYPE_4K2K_Y_SIZE>>1)  //FBM_SUPPORT_4K2K;C size will *2 in FBM_ConfigColorMode if 422 mode!
#define FBM_FBG_TYPE_4K1K_Y_SIZE                      FBM_FBG_TYPE_SBSHD_Y_SIZE   // FOR 4K1K VIDEO
#define FBM_FBG_TYPE_4K1K_C_SIZE                      FBM_FBG_TYPE_SBSHD_C_SIZE  //FBM_SUPPORT_4K2K

#define FBM_FBG_TYPE_CABAC_SIZE                                         FBM_ALIGN_MASK(0x140000,FBM_FMG_C_ALIGMENT) // error table buffer:(2304/16*32) = 5K
                                                                                                                     //cabac info buffer :(4096/16)*tile_number*896 = 2M
                                                                                                                     //tile_number = 16
                                                                                                                     //1.25M *2 
//Extra 2 MB row buffer for B pic deblocking (field pic need 4 MB row)
#define FBM_TWO_MB_ROW_V                                                64

#define FBM_SBSHD_Y_EXTRA_SIZE                        FBM_ALIGN_MASK((FBM_FBG_SBSHD_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_TBNHD_Y_EXTRA_SIZE                        FBM_ALIGN_MASK((FBM_FBG_TNBHD_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_1080HD_Y_EXTRA_SIZE                       FBM_ALIGN_MASK((FBM_FBG_1080HD_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_720HD_Y_EXTRA_SIZE                        FBM_ALIGN_MASK((FBM_FBG_720HD_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_PAL_Y_EXTRA_SIZE                          FBM_ALIGN_MASK((FBM_FBG_PAL_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_NTSC_Y_EXTRA_SIZE                         FBM_ALIGN_MASK((FBM_FBG_NTSC_H * FBM_TWO_MB_ROW_V), FBM_MPV_Y_ALIGMENT)
#define FBM_1080HD_C_EXTRA_SIZE                       (FBM_ALIGN_MASK((FBM_FBG_1080HD_H * FBM_TWO_MB_ROW_V), FBM_MPV_C_ALIGMENT) >> 1)
#define FBM_720HD_C_EXTRA_SIZE                        (FBM_ALIGN_MASK((FBM_FBG_720HD_H * FBM_TWO_MB_ROW_V), FBM_MPV_C_ALIGMENT) >> 1)
#define FBM_PAL_C_EXTRA_SIZE                          (FBM_ALIGN_MASK((FBM_FBG_PAL_H * FBM_TWO_MB_ROW_V), FBM_MPV_C_ALIGMENT) >> 1)
#define FBM_NTSC_C_EXTRA_SIZE                         (FBM_ALIGN_MASK((FBM_FBG_NTSC_H * FBM_TWO_MB_ROW_V), FBM_MPV_C_ALIGMENT) >> 1)

#define FBM_1080HD_EXTRA_SIZE                         (FBM_1080HD_Y_EXTRA_SIZE + FBM_1080HD_C_EXTRA_SIZE)
#define FBM_720HD_EXTRA_SIZE                          (FBM_720HD_Y_EXTRA_SIZE + FBM_720HD_C_EXTRA_SIZE)
#define FBM_PAL_EXTRA_SIZE                            (FBM_PAL_Y_EXTRA_SIZE + FBM_PAL_C_EXTRA_SIZE)
#define FBM_NTSC_EXTRA_SIZE                           (FBM_NTSC_Y_EXTRA_SIZE + FBM_NTSC_C_EXTRA_SIZE)

//MPEG4/VC1 working buffer size
#define FBM_WORK_BUF_SIZE                             (1152 * 1024)

//VP9 Start
#define FBM_WORK_BUF_VP9_FHD_SIZE                     (300 * 1024)

#define FBM_WORK_BUF_VP9_4K2K_SIZE                    (1024 * 1024 * 3 / 2)

#define FBM_WORK_BUF_VP9_MISC_TBL_SIZE                (0x49000)
// V9 End

// Frame Buffer Group Size

#define FBM_FBG_TYPE_1080HD_H264_B_NS                                   4
#define FBM_FBG_TYPE_1080HD_H264_B_64M_NS                               3

// Size = (R + B) * (Y Size +C Size) + (R + 1) * (Y Size/4)
#define FBM_FBG_TYPE_1080HD_H264_FB_SIZE                                (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_FBG_TYPE_1080HD_H264_R_NS + FBM_FBG_TYPE_1080HD_H264_B_NS))  * 3) >> 1)
#define FBM_FBG_TYPE_1080HD_H264_MV_SIZE                                ((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_FBG_TYPE_1080HD_H264_R_NS + 1)) >> 2)
//#define FBM_FBG_TYPE_1080HD_H264_SIZE                                   (FBM_FBG_TYPE_1080HD_H264_FB_SIZE + FBM_FBG_TYPE_1080HD_H264_MV_SIZE)

#define FBM_FBG_TYPE_1080HD_H264_FB_64M_SIZE                            (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_FBG_TYPE_1080HD_H264_R_NS + FBM_FBG_TYPE_1080HD_H264_B_64M_NS))  * 3) >> 1)
#define FBM_FBG_TYPE_1080HD_H264_64M_SIZE                               (FBM_FBG_TYPE_1080HD_H264_FB_64M_SIZE + FBM_FBG_TYPE_1080HD_H264_MV_SIZE)
#define FBM_FBG_TYPE_1080HD_H264_64M_C_SIZE                             (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_FBG_TYPE_1080HD_H264_R_NS + FBM_FBG_TYPE_1080HD_H264_B_64M_NS))) >> 1)


// size = (ref frame 12mb) + (decode frame+ two lock frame) + (ref frame + decoder)/4 ---> for motion vector

#if defined(DIVX_PLUS_CER) || defined(CC_NETFLIX_CE3_SUPPORT) || defined(CC_ENABLE_SEAMLESS_FOR_2D) || defined(CC_FBM_SUPPORT_FHD_SEAMLESS)
#ifndef CC_ENABLE_SEAMLESS_FOR_2D
#define CC_ENABLE_SEAMLESS_FOR_2D
#endif
#endif


#ifdef CC_H264_LV5_SUPPORT_SINGLE_POOL
#define FMB_EXTRA_FOR_SINGLE_H264_LV5 ((((FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL - FBM_FBG_H264_FHD_REFERNCE_NS)*1920*1088*3)>>1) + (((FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL - FBM_FBG_H264_FHD_REFERNCE_NS)*1920*1088)>>2))
#else
#define FMB_EXTRA_FOR_SINGLE_H264_LV5 (0)
#endif
#ifdef CC_SUPPORT_NPTV_SEAMLESS 
#define NPTV_SEAMLESS_EXTRA_FB_NS   3
#else
#define NPTV_SEAMLESS_EXTRA_FB_NS   0
#endif

#ifdef CC_SUPPORT_TV_STAGEFRIGHT_4K2K
//TBD
#define STAGEFRIGHT_EXTRA_FB_NS   0  
#elif defined(CC_SUPPORT_TV_STAGEFRIGHT)  
// liuqu add for distinguish,20121206
#define STAGEFRIGHT_EXTRA_FB_NS   0
#else
#define STAGEFRIGHT_EXTRA_FB_NS   0
#endif
#define FBM_FBG_TYPE_1080HD_H264_SIZE ((((FBM_FBG_TYPE_1080HD_H264_R_NS+1+STAGEFRIGHT_EXTRA_FB_NS+NPTV_SEAMLESS_EXTRA_FB_NS+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_1080HD_Y_SIZE*3)>>1) + (((FBM_FBG_TYPE_1080HD_H264_R_NS+1)*FBM_FBG_TYPE_1080HD_Y_SIZE) >>2))
#define FBM_FBG_TYPE_1080HD_H264_C_SIZE ((((FBM_FBG_TYPE_1080HD_H264_R_NS+1+STAGEFRIGHT_EXTRA_FB_NS+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_1080HD_Y_SIZE)>>1) + ((FBM_FBG_TYPE_CABAC_SIZE)<<1))
#define FBM_FBG_TYPE_720HD_H264_SIZE  ((((FBM_FBG_TYPE_720HD_H264_R_NS+1+STAGEFRIGHT_EXTRA_FB_NS+NPTV_SEAMLESS_EXTRA_FB_NS+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_720HD_Y_SIZE*3)>>1) + (((FBM_FBG_TYPE_720HD_H264_R_NS+1)*FBM_FBG_TYPE_720HD_Y_SIZE) >>2))
#define FBM_FBG_TYPE_720HD_H264_C_SIZE  (((((FBM_FBG_TYPE_720HD_H264_R_NS+1+STAGEFRIGHT_EXTRA_FB_NS+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_720HD_Y_SIZE)>>1)) + ((FBM_FBG_TYPE_CABAC_SIZE)<<1))
#define FBM_FBG_TYPE_PAL_H264_SIZE    ((((FBM_FBG_TYPE_PAL_H264_R_NS+1+STAGEFRIGHT_EXTRA_FB_NS+NPTV_SEAMLESS_EXTRA_FB_NS+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_PAL_Y_SIZE*3)>>1) + (((FBM_FBG_TYPE_PAL_H264_R_NS+1)*FBM_FBG_TYPE_PAL_Y_SIZE) >>2))
#define FBM_FBG_TYPE_PAL_H264_C_SIZE    ((((FBM_FBG_TYPE_PAL_H264_R_NS+1+STAGEFRIGHT_EXTRA_FB_NS+FBM_LOCK_FB_NS)*FBM_FBG_TYPE_PAL_Y_SIZE)>>1)+ ((FBM_FBG_TYPE_CABAC_SIZE)<<1))

#define FBM_FBG_TYPE_1080HD_H264_RR_FB_SIZE                             (FBM_FBG_TYPE_1080HD_H264_FB_SIZE >> 1)
#define FBM_FBG_TYPE_1080HD_H264_RR_SIZE                                   (FBM_FBG_TYPE_1080HD_H264_RR_FB_SIZE + FBM_FBG_TYPE_1080HD_H264_MV_SIZE)

#define FBM_FBG_TYPE_1080HD_H264_RR_FB_64M_SIZE                         (FBM_FBG_TYPE_1080HD_H264_FB_64M_SIZE >> 1)
#define FBM_FBG_TYPE_1080HD_H264_RR_64M_SIZE                            (FBM_FBG_TYPE_1080HD_H264_RR_FB_64M_SIZE + FBM_FBG_TYPE_1080HD_H264_MV_SIZE)
#define FBM_FBG_TYPE_1080HD_H264_RR_64M_C_SIZE                           (FBM_FBG_TYPE_1080HD_H264_64M_C_SIZE >> 1)

#define FBM_FBG_TYPE_PAL_H264_RR_FB_SIZE                                   (FBM_FBG_TYPE_PAL_H264_FB_SIZE >> 1)
#define FBM_FBG_TYPE_PAL_H264_RR_SIZE                                      (FBM_FBG_TYPE_PAL_H264_RR_FB_SIZE + FBM_FBG_TYPE_PAL_H264_MV_SIZE)


#define FBM_FBG_TYPE_1080HD_SIZE                                        (((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_NORMAL_FB_NS*3))>>1) + FBM_WORK_BUF_SIZE)
#define FBM_FBG_TYPE_1080HD_C_TOTAL_SIZE                                ((FBM_FBG_TYPE_1080HD_Y_SIZE * (FBM_NORMAL_FB_NS))>>1)
#define FBM_FBG_TYPE_720HD_SIZE                                         (FBM_FBG_TYPE_720HD_Y_SIZE * ((FBM_NORMAL_FB_NS*3)>>1))
#define FBM_FBG_TYPE_720HD_C_TOTAL_SIZE                                 ((FBM_FBG_TYPE_720HD_Y_SIZE * FBM_NORMAL_FB_NS)>>1)
#define FBM_FBG_TYPE_PAL_SIZE                                           (FBM_FBG_TYPE_PAL_Y_SIZE * ((FBM_NORMAL_FB_NS*3)>>1))
#define FBM_FBG_TYPE_PAL_C_TOTAL_SIZE                                   ((FBM_FBG_TYPE_PAL_Y_SIZE * FBM_NORMAL_FB_NS)>>1)
#define FBM_FBG_TYPE_NTSC_SIZE                                          (FBM_FBG_TYPE_NTSC_Y_SIZE * ((FBM_NORMAL_FB_NS*3)>>1))
#define FBM_FBG_TYPE_1080HD_3FB_SIZE                                    ((FBM_FBG_TYPE_1080HD_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_720HD_3FB_SIZE                                     ((FBM_FBG_TYPE_720HD_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_PAL_3FB_SIZE                                       ((FBM_FBG_TYPE_PAL_Y_SIZE * 9) /2)
#define FBM_FBG_TYPE_PAL_2_5FB_SIZE                                     ((FBM_FBG_TYPE_PAL_Y_SIZE * 15) /4)

#define FBM_FBG_TYPE_1080HD_1_5FB_SIZE                                  (FBM_FBG_TYPE_1080HD_3FB_SIZE / 2)
#define FBM_FBG_TYPE_1080HD_2_FB_SIZE                                   (FBM_FBG_TYPE_1080HD_Y_SIZE * 3)

#define FBM_FBG_TYPE_1080HD_VOMX_ARGB_NS                                (4)
#define FBM_FBG_TYPE_1080HD_VOMX_ARGB_SIZE                              (FBM_FBG_TYPE_1080HD_Y_SIZE * 4) * (FBM_FBG_TYPE_1080HD_VOMX_ARGB_NS + 1)
#define FBM_FBG_TYPE_1080HD_VOMX_NV12_NS                                (4)
#define FBM_FBG_TYPE_1080HD_VOMX_NV12_SIZE                              (FBM_FBG_TYPE_1080HD_Y_SIZE * 3 / 2) * (FBM_FBG_TYPE_1080HD_VOMX_ARGB_NS)


#define FBM_FBG_TYPE_4K2K_JPEG_SIZE                                     (FBM_FBG_TYPE_4K2K_Y_SIZE*2*2) // 2 frame,422 mode.  this is for 4k2k jpeg!!!//FBM_SUPPORT_4K2K      
#define FBM_FBG_TYPE_4K2K_JPEG_C_SIZE                                   ((FBM_FBG_TYPE_4K2K_Y_SIZE*2*2)/2)
#define FBM_FBG_TYPE_4K2K_MPEG_SIZE                                     (FBM_FBG_TYPE_1080HD_H264_SIZE*4)


#ifdef CC_SUPPORT_NPTV_SEAMLESS 
#define NPTV_4K2K_SEAMLESS_EXTRA_FB_NS   0
#else
#define NPTV_4K2K_SEAMLESS_EXTRA_FB_NS   0
#endif


#define FBM_FBG_TYPE_4K2K_H264_MV_SIZE        ( (FBM_FBG_TYPE_4K2K_Y_SIZE) >> 2)
#define FBM_FBG_TYPE_4K2K_H264_FB_SIZE        (((FBM_FBG_TYPE_4K2K_Y_SIZE) * 3) >> 1)
#define FBM_FBG_TYPE_4K2K_H264_FB_NS          (FBM_FBG_H264_4K2K_REFERNCE_NS + FBM_LOCK_FB_NS + NPTV_4K2K_SEAMLESS_EXTRA_FB_NS + 1)//ref + stage fright lock + lock + decode
#define FBM_FBG_TYPE_4K2K_H264_TOTAL_SIZE     ((FBM_FBG_TYPE_4K2K_H264_FB_SIZE * FBM_FBG_TYPE_4K2K_H264_FB_NS) + \
                                               (FBM_FBG_TYPE_4K2K_H264_MV_SIZE * (FBM_FBG_H264_4K2K_REFERNCE_NS + 1)))
#define FBM_FBG_TYPE_4K2K_7FB_SIZE_PART1      (FBM_FBG_TYPE_4K2K_H264_FB_SIZE * (FBM_FBG_TYPE_4K2K_H264_FB_NS/2 + 1)) //TBD
#define FBM_FBG_TYPE_4K2K_7FB_SIZE_C_PART1    ((FBM_FBG_TYPE_4K2K_Y_SIZE >>1) * (FBM_FBG_TYPE_4K2K_H264_FB_NS/2 + 1)) //TBD
#define FBM_FBG_TYPE_4K2K_7FB_SIZE_PART2      (FBM_FBG_TYPE_4K2K_H264_TOTAL_SIZE - FBM_FBG_TYPE_4K2K_7FB_SIZE_PART1)
#define FBM_FBG_TYPE_4K2K_7FB_SIZE_C_PART2    ( (FBM_FBG_TYPE_4K2K_Y_SIZE >>1) * FBM_FBG_TYPE_4K2K_H264_FB_NS - FBM_FBG_TYPE_4K2K_7FB_SIZE_C_PART1)
#define FBM_FBG_TYPE_4K2K_H264_SIZE           (FBM_FBG_TYPE_4K2K_H264_TOTAL_SIZE) // 4 frame,422 mode.  this is for 4k2k jpeg!!!//FBM_SUPPORT_4K2K      

/* VP9 */

#define FBM_FBG_TYPE_4K2K_VP9_MV_SIZE          ((FBM_FBG_TYPE_4K2K_Y_SIZE) >> 2)
#define FBM_FBG_TYPE_4K2K_VP9_FB_SIZE          (((FBM_FBG_TYPE_4K2K_Y_SIZE) * 3) >> 1)
#define FBM_FBG_TYPE_4K2K_VP9_FB_NS            (FBM_FBG_H264_4K2K_REFERNCE_NS + FBM_LOCK_FB_NS + 1 + 2 + NPTV_4K2K_SEAMLESS_EXTRA_FB_NS)  // 11 FBs + 1(4K seamless)
#define FBM_FBG_TYPE_4K2K_VP9_TOTAL_SIZE       ((FBM_FBG_TYPE_4K2K_VP9_FB_SIZE * FBM_FBG_TYPE_4K2K_VP9_FB_NS) + (FBM_FBG_TYPE_4K2K_VP9_MV_SIZE * (1 + 1)))
#define FBM_FBG_TYPE_4K2K_SIZE_PART1           (FBM_FBG_TYPE_4K2K_VP9_FB_SIZE * (FBM_FBG_TYPE_4K2K_VP9_FB_NS/2 + 1)) //TBD
#define FBM_FBG_TYPE_4K2K_SIZE_C_PART1         ((FBM_FBG_TYPE_4K2K_Y_SIZE >>1) * (FBM_FBG_TYPE_4K2K_VP9_FB_NS/2 + 1)) //TBD
#define FBM_FBG_TYPE_4K2K_SIZE_PART2           (FBM_FBG_TYPE_4K2K_VP9_TOTAL_SIZE - FBM_FBG_TYPE_4K2K_SIZE_PART1)
#define FBM_FBG_TYPE_4K2K_SIZE_C_PART2         ( (FBM_FBG_TYPE_4K2K_Y_SIZE >>1) * FBM_FBG_TYPE_4K2K_VP9_FB_NS - FBM_FBG_TYPE_4K2K_SIZE_C_PART1)
#define FBM_FBG_TYPE_4K2K_VP9_SIZE             (FBM_FBG_TYPE_4K2K_VP9_TOTAL_SIZE) // 4 frame,422 mode.  this is for 4k2k jpeg!!!//FBM_SUPPORT_4K2K      



// ES ID
#define FBM_VIN_ES_ID                                                4    // For Video Input Hardware, Mpeg 0~3
#define FBM_DV_ES_ID                                                 5    // For DV Hardware, Mpeg 0~3

// Frame Buffer Group Flag
#define FBM_FLAG_CLR_ALL                                             0xFFFFFFFF // Clear all flag
#define FBM_FLAG_DISP_READY                                          (1 << 0)   // Display Ready, video plane will get FB from queue
#define FBM_FLAG_RESET                                               (1 << 1)   // Reset corresponding FBG
#define FBM_FLAG_SEQ_CHG                                             (1 << 2)   // Sequence Header Change
#define FBM_FLAG_SKIP_PIC                                            (1 << 3)   // Skip one picture
#define FBM_FLAG_DIGEST_MODE                                         (1 << 4)   // Digest Mode, obsolete
#define FBM_FLAG_CLEAR_FRC_PRM                                       (1 << 5)   // After Mpv reset, ask vdp frc to reset prm
#define FBM_FLAG_DEINTERLACE                                         (1 << 6)   // Deinterlace, add one more frame buffer
#define FBM_FLAG_NOT_READY                                           (1 << 7)   // No FB status change in this mode
#define FBM_FLAG_THREE_B                                             (1 << 8)   // 3 B pics between two Ref pics in this sequence
#define FBM_FLAG_DISPLAYING                                          (1 << 9)   // Video plane displaying
#define FBM_FLAG_ES_FIFO_FULL                                        (1 << 10)  // MPEG Elementary Stream's FIFO Full
#define FBM_FLAG_FREEZE                                              (1 << 14)  // MPEG should still decode, if no empty fb, get fb from display Q
#define FBM_FLAG_SKIP_PIC_NO_PTS                                     (1 << 15)  // Skip one picture with PTS = 0
#define FBM_FLAG_DEBLOCKING                                          (1 << 16)  // Deblocking mode
#define FBM_FLAG_REDUCE_REF                                          (1 << 17)  // Reduce Reference mode
#define FBM_FLAG_RELEASE_FBG                                         (1 << 18)  // Release FBG
#define FBM_FLAG_SEEK_MODE                                           (1 << 19)  // MM seek mode
#define FBM_FLAG_SUPER_FREEZE                                        (1 << 20)  // middleware super freeze mode
#define FBM_FLAG_3D_SEQ_CHG                                          (1 << 21)  //3D state sequence change notify
#define FBM_FLAG_3D_ERROR_HANDLE_DISABLE                             (1 << 22)  // enable 3D error handling
#define FBM_FLAG_SEQ_CHG_SPEEDUP                                     (1 << 24)  // New Sequence header change, early notify start play to video path
#define FBM_FLAG_THUMBNAIL_MODE                                      (1 << 25)  // New Thumbnail mode without VDP.
#define FBM_FLAG_AUTO_RENDER                                         (1 << 26)  // FBG without render path, need auto consume dispQ FB.
#define FBM_FLAG_GETEMPTY_NOWAIT                                     (1 << 27)  // FBG get Empty buffer not wait

//Wfd Ajust STC Flag
#define FBM_WFD_FRAME_MONITOR                                         (1 << 0)
#define FBM_WFD_FRAME_COUNTING                                        (1 << 1)

// Picture Coding Type
#define MPEG_PIC_TYPE_I                                              1
#define MPEG_PIC_TYPE_P                                              2
#define MPEG_PIC_TYPE_B                                              3

// Picture Structure
#define MPEG_PIC_TOP_FIELD                                           1
#define MPEG_PIC_BOTTOM_FIELD                                        2
#define MPEG_PIC_FRAME                                               3
#define MPEG_PIC_FIELD                                               4


// Frame Rate Code
#define MPEG_FRAME_RATE_24_                                          1    // minus
#define MPEG_FRAME_RATE_24                                           2
#define MPEG_FRAME_RATE_25                                           3
#define MPEG_FRAME_RATE_30_                                          4    // minus
#define MPEG_FRAME_RATE_30                                           5
#define MPEG_FRAME_RATE_50                                           6
#define MPEG_FRAME_RATE_60_                                          7    // minus
#define MPEG_FRAME_RATE_60                                           8
#define MPEG_FRAME_RATE_12                                           9
#define MPEG_FRAME_RATE_15                                           10    // minus
#define MPEG_FRAME_RATE_20                                           11
#define MPEG_FRAME_RATE_120_                                         12
#define MPEG_FRAME_RATE_120                                          13
#define MPEG_FRAME_RATE_240_                                         14
#define MPEG_FRAME_RATE_240                                          15
#define MPEG_FRAME_RATE_UNKNOWN                                      0xFF

// Aspect Ratio
#define MPEG_ASPECT_RATIO_1_1          1   // SAR, 1:1
#define MPEG_ASPECT_RATIO_4_3          2   // DAR, 4:3
#define MPEG_ASPECT_RATIO_16_9         3   // DAR, 16:9
#define MPEG_ASPECT_RATIO_221_1        4   // DAR, 2.21:1

#define MPEG_ASPECT_RATIO_10_11        5 // SAR, 10:11
#define MPEG_ASPECT_RATIO_40_33        6 // SAR, 40:33
#define MPEG_ASPECT_RATIO_16_11        7 // SAR, 16:11
#define MPEG_ASPECT_RATIO_12_11        8 // SAR, 12:11
#define MPEG_ASPECT_RATIO_3_2          9 // SAR 3:2
#define MPEG_ASPECT_RATIO_24_11       10 // SAR 24:11
#define MPEG_ASPECT_RATIO_20_11       11 // SAR 20:11
#define MPEG_ASPECT_RATIO_32_11       12 // SAR 32:11
#define MPEG_ASPECT_RATIO_80_33       13 // SAR 80:33
#define MPEG_ASPECT_RATIO_18_11       14 // SAR 18:11
#define MPEG_ASPECT_RATIO_15_11       15 // SAR 15:11
#define MPEG_ASPECT_RATIO_64_33       16 // SAR 64:33
#define MPEG_ASPECT_RATIO_160_99      17 // SAR 160:99
#define MPEG_ASPECT_RATIO_TRANSMIT    18

//Duration
#define MPEG_DURATION_24_           1877
#define MPEG_DURATION_25            1800
#define MPEG_DURATION_30_           1502
#define MPEG_DURATION_30            1500

// Colour Primary
#define MPEG_COLOR_PRIMARY_709                                       1

// Frame Rate
#define FBM_FRAME_RATE_UNKNOWN                                       0xFF
#define FBM_FRAME_RATE_24                                            24
#define FBM_FRAME_RATE_25                                            25
#define FBM_FRAME_RATE_30                                            30
#define FBM_FRAME_RATE_50                                            50
#define FBM_FRAME_RATE_60                                            60
#define FBM_FRAME_RATE_12                                            12
#define FBM_FRAME_RATE_15                                            15
#define FBM_FRAME_RATE_20                                            20
#define FBM_FRAME_RATE_120                                           120
#define FBM_FRAME_RATE_240                                           240


// Frame Buffer Color Mode
#define FBM_CM_UNKNOWN                                               0xFF
#define FBM_CM_420                                                   0
#define FBM_CM_422                                                   1
#define FBM_CM_444                                                   2
#define FBM_CM_422C                                                  8
#define FBM_CM_YUV24                                                 12
#define FBM_CM_RGB24                                                 13
#define FBM_CM_YUV32                                                 14
#define FBM_CM_RGB32                                                 15
#define FBM_CM_DEFAULT                                               FBM_CM_420

// Frame Buffer Status
#define FBM_FB_STATUS_UNKNOWN                                        0xFF
#define FBM_FB_STATUS_EMPTY                                          0
#define FBM_FB_STATUS_DECODE                                         1
#define FBM_FB_STATUS_READY                                          2
#define FBM_FB_STATUS_DISPLAYQ                                       3
#define FBM_FB_STATUS_LOCK                                           4
#define FBM_FB_STATUS_DIGEST                                         5
#define FBM_FB_STATUS_FIELD_READY                                    6
#define FBM_FB_STATUS_RESIZE                                         7
#define FBM_FB_STATUS_RESIZE_DONE                                    8
#define FBM_FB_STATUS_RECYCLE                                        9

// Number of Pan Scan Vector
#define FBM_PANSCAN_VECTOR_NS                                        3

//Active format
#define ACT_FMT_DEFAULT                                              0x8

//MT539x Video decoder format
#define FBM_VDEC_UNKNOWN                                             0xFF
#define FBM_VDEC_H264                                                0x1
#define FBM_VDEC_MPEG2                                               0x2
#define FBM_VDEC_MPEG4                                               0x3
#define FBM_VDEC_VC1                                                 0x4
#define FBM_VDEC_JPEG                                                0x5
#define FBM_VDEC_RV                                                  0x6
#define FBM_VDEC_AVS                                                 0x7
#define FBM_VDEC_RAW                                                 0x8
#define FBM_VDEC_VP6                                                 0x9
#define FBM_VDEC_VP8                                                 0xA
#define FBM_VDEC_H265                                                0xB
#define FBM_VDEC_VP9                                                 0xC


//FBM flag for MM
#define FBM_MM_EOS_FLAG                                              (1 << 0)
#define FBM_MM_SYNC_POINT_FLAG                                       (1 << 1)
#define FBM_MM_NOT_DISPLAY_FLAG                                      (1 << 2)
#define FBM_MM_PSEUDO_EOS_FLAG                                       (1 << 3)
#define FBM_MM_3D_CHG_FLAG                                           (1 << 4)
#define FBM_MM_BACKWARD_FLAG                                         (1 << 5) // only valid when EOS flag is on

// FBM Picture Header ucTrueZeroPTS
#define FBM_USE_ZERO_PTS_IF_ZERO    0x1
#define FBM_USE_LAST_PTS_IF_ZERO    0x0

// FBG Play mode
#define FBM_FBG_DTV_MODE        0
#define FBM_FBG_MM_MODE         1
#define FBM_FBG_NOTDISPLAY_MODE 2
#define FBM_FBG_MODE_NS         3

// Application Mode
#define FBM_FBG_APP_NORMAL          (0)
#define FBM_FBG_APP_MHP             (1)
#define FBM_FBG_APP_OMX             (2)
#define FBM_FBG_APP_SKYPE           (3)
#define FBM_FBG_APP_VP6_ALPHA       (4)
#define FBM_FBG_APP_Frame_Pack_3D   (5)
#define FBM_FBG_APP_OMX_DISP        (6)
#define FBM_FBG_APP_MTIMAGE         (7)

// Vdec profile
#define FBM_H264_LV5_PROFILE        (1 << 0)

// FBG AV sync mode
#define FBM_FBG_AVSYNC_DISABLE        0
#define FBM_FBG_AVSYNC_STC_INVALID    1
#define FBM_FBG_AVSYNC_WAIT_1ST       2
#define FBM_FBG_AVSYNC_RESERVE        3
#define FBM_FBG_AVSYNC_NORMAL         4
#define FBM_FBG_AVSYNC_REPEAT         5
#define FBM_FBG_AVSYNC_DROP           6
#define FBM_FBG_AVSYNC_PTS_JITTER     7
#define FBM_FBG_AVSYNC_NO_PTS         8
#define FBM_FBG_AVSYNC_UNKNOWN        9


/* FBM global flag, not related to FBG index */
#define FBM_FLAG_FB_NO_TIMEOUT        (UINT8)(1 << 0)  // FB wait not timeout
#define FBM_FLAG_FB_DECODE_ONLY       (UINT8)(1 << 1)  //

// FBM resize small picture for scalar h/w limitation
// Scalar only accept width bigger thant 180 pixels
// but actually, it can low to 64 pixel.
// it only fail with 32x32 or lower resolution
#define CC_RESIZE_SMALL_IMAGE

// Speedup decoder sequence change time
#define CC_ENABLE_SEQ_CHG_SPEEDUP

#if defined(CC_MT5395)
#define FBM_IMG_RESIZE_LIMITATION (192)
#elif  defined(CC_MT5399)|| defined(CC_MT5880)|| defined(CC_MT5860) ||defined(CC_MT5890)||defined(CC_MT5882)
#define FBM_IMG_RESIZE_LIMITATION (112)
#else
#define FBM_IMG_RESIZE_LIMITATION (192)
#endif
#define FBM_IMG_RESIZE_LIMITATION_MAX (1920)
#define FBM_IMG_RESIZE_NETFLIX_WIDTH (1280)
#define FBM_IMG_RESIZE_NETFLIX_HEIGHT (720)

//B2R 3D ERROR Detectation
#define B2R_3D_ERROR_COUNT (15)
#define B2R_3D_ERROR_RECOVER_COUNT (15)

//B2R 3D Type
#define B2R_3D_NONE (0)
#define B2R_3D_FRAME_SEQUENTIAL (1)
#define B2R_3D_SIDE_BY_SIDE_REAL_D (2)
#define B2R_3D_SIDE_BY_SIDE (3)
#define B2R_3D_TOP_N_BOTTOM (4)

#define B2R_3D_MODE_ALWAYS_L_VIEW (1)
#define B2R_3D_MODE_AUTO_TOGGLE (2)
#define B2R_3D_MODE_MANUAL_SET (3)

//B2R Trick Mode Type
#define B2R_3D_TRICK_MODE_L_VIEW (0)
#define B2R_3D_TRICK_MODE_R_VIEW (1)
#define B2R_3D_TRICK_MODE_3D_VIEW (2)


//B2R force 2d type
#define B2R_FORCE_2D_L_VIEW (0)
#define B2R_FORCE_2D_R_VIEW (1)

//B2R option define
//#define B2R_SD_TEST
#define CC_NETFLIX_SUPPORT
//#define CC_B2R_ENABLE_SEAMLESS_THREAD
//#define CC_B2R_ENALBE_OUTPUT_FRC
//#define CC_B2R_DISABLE_PTS_IIR
//#define CC_B2R_FORCE_UPDATE_STC

//#define CC_B2R_3D_MVC_INTERLACE_SUPPORT
#define CC_B2R_DISABLE_ONE_SECOND_FRC
//#define CC_B2R_GFX_COPY_FIELD
//#define FBM_VIRTUAL_MPEG
//#define MAIN_SUB_OPEN
//#define CC_B2R_3D_SUPPROT
//#define CC_B2R_3D_ERROR_SUPPORT
//#define VDP_AVS_ADJUST_STC
#define VERIFICATION_REALD  (FALSE)
#define CC_B2R_ENABLE_CHG_FRAME_MSG

#define FBM_DI_HYBRID_WIDTH     3840
#define FBM_DI_PIP_FIFO_WIDTH  1920
#define FBM_DI_POP_FIFO_WIDTH  960

#ifndef __MODEL_slt__
#define CC_B2R_WAIT_NPTV_STABLE
#endif


// Frame Buffer Pool Configuration
#ifdef CC_2HD_SYSTEM
// Maximum frame buffer group number
#define FBG_MAX_NS					10

#elif defined (CC_1HD_1SD_SYSTEM)
// Maximum frame buffer group number
#define FBG_MAX_NS					7

#elif defined (CC_1SD_SYSTEM)
// Maximum frame buffer group number
#define FBG_MAX_NS					3

#else
// Maximum frame buffer group number
#define FBG_MAX_NS					10
#endif

// Decoder puts this ascii into put buffer
#define FBG_INTERNAL_VOMX_BUFFER_ID    "FBG_INTERNAL_VOMX_BUFFER"
#define FBG_INTERNAL_VOMX_BUFFER_SIZE  (20)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#if 0
#define  Max_Capability      (2)
#define  Limit_Capability    (1)
#define  No_Capability      (0)
#define  UFD_INPUT        (3840 * 2160) // (0x7E9000)
#define  FHD_INPUT       (1920 * 1088) //(0x1FE000)
#define  HD_INPUT        (1280 * 720) // (0x7E9000)

#define  MaxB2RNum       2   
#define  B2RHD           0
#define  B2RSD           1

typedef enum
{
    HEVC =0,
    H264,
    VC1,
    VDEC_NS
}VdecType_Table;
    
typedef struct
{
    UINT16 u2HActive;
    UINT16 u2VActive;
    UINT16 u2Fps;
    BOOL u28Bit;
    VdecType_Table eVdecType;
    UINT16 u2CapabilityLvl;
    UINT16 u2BlockMode;
}B2R_RM_T;
    
    
typedef struct
{
    B2R_RM_T NB2R_RM[MaxB2RNum];
    B2R_RM_T OldB2R_RM[MaxB2RNum];
    BOOL    ufgOldB2RStatus;
    BOOL    ufgNewB2RStatus;
}B2R_Capability_Map;
    
typedef enum
{
    Old_B2R0 =0,
    Old_B2R1,
    New_B2R0,
    New_B2R1,
    B2R_NULL
}B2R_Table;
#endif

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
typedef struct _VDP_CHAPTER_T
{
    UINT64      u8StartPts;
    UINT64      u8EndPts;
    BOOL        fgIsPending;
    BOOL        fgIsSetEndPts;
    BOOL        fgPending;
    BOOL        fgFirstSetChapter;
} VDP_CHAPTER_T;
#endif
#endif

//3D infromation update structure
typedef struct
{
    UINT8 u1B2R3DType;
    UINT8 u1B2RForce2DType;
    BOOL fgB2R3DEnable;
    BOOL fgB2RForce2D;
    UINT32 u4B2R3DWidthOffset;
    UINT32 u4B2R3DHeightOffset;
} FBM_3D_INFO_T;

typedef enum
{
    FBM_SM_NO_SWAP,
    FBM_SM_SWAP0,
    FBM_SM_MAX
} FBM_SWAP_MODE_T;

typedef enum
{
    FBM_PM_NORMAL,
    FBM_PM_COMPACT,
    FBM_PM_UNPACK,
    FBM_PM_PACK,
    FBM_PM_MAX
} FBM_PACK_MODE_T;

/** Brief of FBM_SEQ_HDR_T
 *  Sequence & GOP Header Structure
 */
typedef struct
{
    BOOL fgLoadIntraMatrix;                         // load_intra_quantizer_matrix;
    BOOL fgLoadNonIntraMatrix;                      // load_non_intra_quantizer_matrix;
    BOOL fgProgressiveSeq;                          // progressive_sequence
    BOOL fgClosedGop;
    BOOL fgBrokenLink;
    BOOL fgYRatio34;
    BOOL fgCRatio34;
    BOOL fgRRYEn;
    BOOL fgRRCEn;
    BOOL fgNoOverScan;
    BOOL fgColorDescrip;
    BOOL fgNoPts;
    BOOL fgResizeSmallPic;
    BOOL fgRasterOrder;
    BOOL fgB2R3DEnable;
    BOOL fgB2RForce2D;
    BOOL fg422Mode;
    BOOL fgRealDEnhance;                            // RealD enhancement
    BOOL fgPixelAspectRatio;
    BOOL fgSwDecoder;

	UCHAR ucBlockType;
    UCHAR ucAspRatInf;                              // aspect_ratio_information
    UCHAR ucOrgAsp;
    UCHAR ucFrmRatCod;                              // frame_rate_code
    UCHAR ucCrmaFmt;                                // chroma_format;
    UCHAR ucColourPrimaries;
    UCHAR ucTransCharacter;
    UCHAR ucMatrixCoeff;
    UCHAR ucActFmt;                                 //active_format;
    UCHAR ucHours;
    UCHAR ucMinutes;
    UCHAR ucSeconds;
    UCHAR ucPictures;
    UINT8 u1B2R3DType;
    UINT8 u1B2RForce2DType;
    UINT8 u1B2R3DTrickModeType;

    UINT16 u2LineSize;                              // horizontal line size, due to block color mode
    UINT16 u2HSize;                                 // horizontal size value to dispaly
    UINT16 u2VSize;                                 // vertical size value to display
    UINT16 u2DHS;                                 // display_horizontal_size in Sequence display extension
    UINT16 u2DVS;                                 // display_vertical_size in Sequence display extension
    UINT16 u2OrgHSize;                              // original horizontal size value in Sequence Header
    UINT16 u2OrgVSize;                              // original vertical size value in Sequence Header
    UINT16 u2OrgWidth;
    UINT16 u2OrgHeight;

    UINT32 u4BitRatVal;                             // bit_rate_value;
    UINT32 u4LastPTS;                               //record the last PTS
    UINT32 u4Duration;
    UINT32 u4FrmCropTop;
    UINT32 u4FrmCropBtm;
    UINT32 u4AspectRatioWidth;
    UINT32 u4AspectRatioHeight;
    UINT32 u4OrgAspWidth;
    UINT32 u4OrgAspHeight;
    UINT32 u4ResizeWidth;
    UINT32 u4ResizeHeight;
    UINT32 u4YAddrOffset;
    UINT32 u4CAddrOffset;
    UINT32 u4B2R3DWidthOffset;
    UINT32 u4B2R3DHeightOffset;

    UINT64 u8LastPTS;                               //record the last PTS
    
    //cropping info
    BOOL fgCropping;
    UINT32 u4CropX;
    UINT32 u4CropY;
    UINT32 u4CropWidth;
    UINT32 u4CropHeight;
    #if 1
    UINT32 u4resolutionChg;
    UINT32 u4ChgPreStatus;
    #endif
    // Additional aspect ratio idc
    UINT32 u4AspectRatioIdc;
    BOOL   fgUfoEn;
    UINT8  u1Depth;
    FBM_PACK_MODE_T ePackMode;
    FBM_SWAP_MODE_T eSwapMode;
    //Seamless playback info
    BOOL fgSeamlessPlay;
    UINT32 eSeamlessMode;
    UINT32 u41stPicInfoCbPts;
	UINT32 u4AVSyncNGCnt;
} FBM_SEQ_HDR_T;

// This structure must be same with VDEC_PIC_EXTRA_INFO_T
/** Brief of FBM_PIC_EXTRA_HDR_T
 *  Picture Header Structure
 */
typedef struct
{
    BOOL fgValid;

    UINT64 u8PTSI;                                  //  I frame pts
    UINT64 u8AudPTS;
    UINT64 u8AudOffset;
} FBM_PIC_EXTRA_INFO_T;

/** Brief of FBM_PIC_HDR_T
 *  Picture Header Structure
 */
typedef struct
{
    UCHAR ucPicCdTp;
    UCHAR ucFullPelFordVec;
    UCHAR ucFordFCode;
    UCHAR ucFullPelBackVec;
    UCHAR ucBackFCode;
    UCHAR ucIntraDcPre;
    UCHAR ucPicStruct;
    UCHAR ucTrueZeroPTS; /* If set to 1, Not use last PTS if current is Zero */
    UCHAR ucSeamlessResizeFbid;
    UCHAR ucSeamlessResize2Fbid;    
    UCHAR ucSeamlessTaskId;

    BOOL fgTopFldFirst;
    BOOL fgFrmPredFrmDct;
    BOOL fgConcMotionVec;
    BOOL fgQScaleType;
    BOOL fgIntraVlcFmt;
    BOOL fgAltScan;
    BOOL fgRepFirstFld;
    BOOL fgProgressiveFrm;
    BOOL fgSeamlessResized;

    UCHAR ucfcode[2][2];

    INT32 i4TemporalRef;
    UINT16 u2DecodingOrder;

    UINT16 au2HOffset[FBM_PANSCAN_VECTOR_NS];
    UINT16 au2VOffset[FBM_PANSCAN_VECTOR_NS];

    UINT32 u4PTS;                                   //  90 KHz PTS value, 33/32 bit ?
    UINT32 u4QpAvg;

    UINT32 u4TotlaTimeOffset;
    UINT32 u4TickNum;

    UINT32 u4MmFlag;

    UINT32 u4Timestap;
    UINT64 u8PTS;
    UINT64 u8Offset;
    UINT64 u8OffsetDisp;
    UINT64 u8OffsetI;
    //for image resizer (netflix and small resolution)
    BOOL fgSeamlessDisp;
    BOOL fgResizeDispFrm;
    UCHAR ucRzId;
    UINT32 u4PicWidth;
    UINT32 u4PicWidthPitch;
    UINT32 u4PicHeight;
    UINT32 u4AddrOffset;
    FBM_PIC_EXTRA_INFO_T rExtra;
    BOOL fgDummyFrame;
    BOOL fgTrickTopField;
    BOOL fgSeqHeader;
    BOOL fgProgressive2Interlace;
    BOOL fgWFDTMSLogEnable;
    FBM_COLOR_SPACE_ID eColorSpace;
    UINT64 u8_vid_pts;
    UINT64 u8_push_time;//ms
    HAL_TIME_T rTimePutDispQ;
    HAL_TIME_T rTimeB2rReleaseRzFB;
    HAL_TIME_T rTimeRzGetFB;
    HAL_TIME_T rTimeGotFBRzStart;
    HAL_TIME_T rTimeRzEnd;
    HAL_TIME_T rTimeB2rGotFB;
    HAL_TIME_T rTimeB2rReleaseFB;

    HAL_TIME_T rTimeHwS;
    HAL_TIME_T rTimeFromDecoder;

    BOOL fgCropping;
    UINT32 u4CropX;
    UINT32 u4CropY;
    UINT32 u4CropWidth;
    UINT32 u4CropHeight;
    BOOL fgNotDisplay;
    BOOL fgBivl3DLR;
	BOOL fgPushModeTrick;
	BOOL fgUfoEn;
	UCHAR ucSrcDepth;
	
	//for dash bandwidth change
	BOOL fgBandwidthChanged;
} FBM_PIC_HDR_T;


/** Brief of FBM_PIC_NTF_INFO_T
 *  Picture Notify Infomation Structure
 */
typedef struct
{
    UCHAR   ucFbgId;
    UCHAR   ucFbId;
    BOOL    fgEos;
} FBM_PIC_NTF_INFO_T;

typedef struct
{
    UINT8 u1VdpId;
    UINT8 u1B2rId;
    BOOL  b_change;
}B2R_CHG_INFO_T;

typedef struct
{
    UCHAR ucFbgId;
    UCHAR ucFbid;
}FB_CHG_INFO_T;

// FBM indication function
typedef VOID (*FBM_FBG_CHG_IND_FUNC)(UCHAR ucFbgId, UCHAR ucEsId);
typedef VOID (*FBM_PICSTRUCT_CHG_IND_FUNC)(UCHAR ucEsId, UCHAR ucPicStruct);
typedef VOID (*FBM_FB_READY_IND_FUNC)(FB_CHG_INFO_T *pt_fb);
//typedef VOID (*FBM_GFX_MEMSET_FUNC)(UINT8 *pu1Addr, UINT32 u4Size, UINT32 u4Color);
typedef VOID (*FBM_GFX_MEMSET_FUNC)(UINT8 *pu1Addr, UINT32 u4Width, UINT32 u4Height, UINT32 u4Color);
typedef UINT64 (*FBM_GFX_TASK_MEMSET_FUNC)(UINT8 *pu1Addr, UINT32 u4Width, UINT32 u4Height, UINT32 u4Color);
typedef UINT32 (*FBM_GFX_TASK_MEMSET_DONE_FUNC)(UINT64 u8TaskId);
typedef VOID (*FBM_CB_POOL_CHANGE_FUNC)(FBM_POOL_TYPE_T ePool);
typedef VOID (*FBM_FB_READY_IND_FUNC_EX)(UINT32 u4Tag, FBM_PIC_NTF_INFO_T *prPicNfy);
typedef BOOL (*FBM_FB_SEQ_CHG_PRE_FUNC)(UINT32 u4Tag);

typedef VOID* (*FBM_GDMA_MEMSET_FUNC)(VOID * s, UINT32 c , UINT32 n);
typedef UINT32 (*FBM_GDMA_TASK_MEMSET_FUNC)(VOID * s, UINT32 c , UINT32 n);
typedef UINT32 (*FBM_GDMA_TASK_MEMSET_DONE_FUNC)(UINT64 u8TaskId);

typedef INT32 (*FBM_B2R_RES_CHG_FUNC)(UCHAR ucFbgId, B2R_CHG_INFO_T* pt_change);
typedef VOID  (*FBM_FB_DISP_START_FUNC)(UCHAR ucEsId,UCHAR ucFbgId,UCHAR ucFbId);
typedef VOID  (*FBM_FB_FAME_TYPEINFO_FUNC)(UCHAR ucEsId,UCHAR ucFbgId,UCHAR ucFbId);


/** Brief of FBM_CB_FUNC_ENUM_T
 */
typedef enum
{
    FBM_CB_FUNC_FBG_CHG_IND,
    FBM_CB_FUNC_PICSTRUCT_CHG_IND,
    FBM_CB_FUNC_FB_READY_IND,
    FBM_CB_FUNC_GFX_MEMSET,
    FBM_CB_FUNC_POOL_CHANGE,
    FBM_CB_FUNC_FB_READY_EX_IND,
    FBM_CB_FUNC_FBG_CHG_EX_IND,
    FBM_CB_FUNC_GFX_TASK_MEMSET, //As 4K2K big pool memset take much long time, change async mode to memset.
    FBM_CB_FUNC_GFX_TASK_MEMSET_DONE, //Co-work with GFX_TASK_MEMSET, query task memset done or not
    FBM_CB_FUNC_SEQ_CHG_PRE_CB,  //Direct callback while FBM get seq chg info.
    FBM_CB_FUNC_GDMA_MEMSET,
    FBM_CB_FUNC_B2R_HW_CHG_CB,
    FBM_CB_FUNC_GDMA_TASK_MEMSET,
    FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE,
    FBM_CB_FUNC_FB_DISPLAY_START,
    FBM_CB_FUNC_FB_FRAME_TYPEINFOR_CB,
    FBM_CB_FUNC_NS
} FBM_CB_FUNC_ENUM_T;

/** Brief of FBM_CB_FUNC_T
 *  FBM Call Back Function Structure
 */
typedef struct
{
    UINT32 au4CbFunc[FBM_CB_FUNC_NS];
    UINT32 au4CbFuncCRC[FBM_CB_FUNC_NS];
    UINT32 aau4CbFunc[FBG_MAX_NS][FBM_CB_FUNC_NS];
    UINT32 aau4CbFuncCRC[FBG_MAX_NS][FBM_CB_FUNC_NS];
    UINT32 aau4CbFuncTag[FBG_MAX_NS][FBM_CB_FUNC_NS];
} FBM_CB_FUNC_T;

/** Brief of FBM_CREATE_FBG_PAR_T
 *  Create group parameter
 */
typedef struct
{
    UINT8 u1AppMode;
    UINT8 u1VdecProfile;
    UINT8 u1VdecId;
    BOOL fgThumbnailMode;
    BOOL fgCreateFromInst;    
    BOOL fgSwHEVC;
    //Seamless Info
    UCHAR ucCftFbgId;
    UINT32 eSeamlessMode;
    UINT32 u4TargetW;
    UINT32 u4TargetH;
    BOOL fgUFO; /*TRUE: UFO,FALSE NOT UFO */
    BOOL fg10Bit; /*TRUE 10Bit,FALSE 8Bit */
    BOOL fgExternalAddr; // Not alloc Pool, FB addr set by external
    UINT8 u1RequestFbNs; // Set need FB Ns.
} FBM_CREATE_FBG_PAR_T;
#if 1
typedef enum
{
    E_VDEC_CODEC_MPEG1,
    E_VDEC_CODEC_MPEG2,
    E_VDEC_CODEC_MPEG4_VISUAL,
    E_VDEC_CODEC_AVC,
    E_VDEC_CODEC_HEVC,
    E_VDEC_CODEC_AVS,
    E_VDEC_CODEC_XVID,
    E_VDEC_CODEC_VC1,
    E_VDEC_CODEC_REAL,
    E_VDEC_CODEC_VP8,
    E_VDEC_CODEC_MOTION_JPEG,
    E_VDEC_CODEC_JPEG,
    E_VDEC_CODEC_MPO,
    E_VDEC_CODEC_RAW,
    E_VDEC_CODEC_UNKNOWN,
}   ENUM_CODEC_INFO_T;

typedef struct
{
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4OrgWidth;
    UINT32 u4OrgHeight;
} FBM_CODEC_RESOLUTION_T;

#endif
//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define FBM_CHECK_CB_FUNC_VERIFY(func, crc)         ((UINT32)func == ~((UINT32)crc))


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

EXTERN VOID FBM_Init(VOID);

EXTERN UCHAR FBM_GetFbgNs(VOID);

EXTERN UCHAR FBM_SelectGroupType(UINT32 u4Width, UINT32 u4Height);

EXTERN UCHAR FBM_CreateGroup(UCHAR ucFbgType, UINT32 u4VDecFmt, UINT32 u4HSize, UINT32 u4VSize);

EXTERN UCHAR FBM_CreateGroupExt(UCHAR ucFbgType, UINT32 u4VDecFmt,
                                UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar);

EXTERN VOID FBM_ReleaseGroup(UCHAR ucFbgId);

EXTERN VOID FBM_ResetGroup(UCHAR ucFbgId);

EXTERN BOOL _FBM_FbgRemap(UCHAR ucFbgId, UINT32 u4Width, UINT32 u4Height);

EXTERN UINT32 FBM_CalcBufNum(UCHAR ucFbgId, UCHAR ucFbgType, UINT32 u4VDecFmt, UINT32 u4Width, UINT32 u4Height,
    UINT32 u4PoolSize, UINT32 u4YSize, UINT32 u4CSize, UINT8 u1AppMode, UINT32* pu4ExtFbNs);

EXTERN VOID FBM_InvalidateRefFrameBuffer(UCHAR ucFbgId);

EXTERN VOID FBM_WaitUnlockFrameBuffer(UCHAR ucFbgId, UINT32 u4Timeout);

EXTERN VOID FBM_ConfigColorMode(UCHAR ucFbgId, UCHAR ucFbgCm,
                                UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar);

EXTERN VOID FBM_SetFrameBufferColor(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Color);

EXTERN VOID FBM_SetFrameBufferZebraColor(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN VOID FBM_SetFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_ChkFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_ChkFrameBufferFlagIsr(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN VOID FBM_ClrFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

EXTERN UCHAR FBM_ChkFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag);

EXTERN VOID FBM_SetFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag);

EXTERN VOID FBM_ClrFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag);

EXTERN UCHAR FBM_GetFrameBufferNs(UCHAR ucFbgId);

EXTERN UCHAR FBM_GetFrameBufferMvNs(UCHAR ucFbgId);

EXTERN VOID FBM_GetFrameBufferSize(UCHAR ucFbgId, UINT32 *pu4Width, UINT32 *pu4Height);

EXTERN UCHAR FBM_GetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_CheckFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);

EXTERN void FBM_SetFrameBufferLayout(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Addr, BOOL fgIsY);

EXTERN VOID FBM_SetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);

#ifdef FBM_FB_LOG
EXTERN VOID FBM_SetFrameBufferStatus_dbg(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus, const CHAR* sFunc, const UINT32 u4Line);

#define FBM_SetFrameBufferStatus(FbgId, FbId, Status) FBM_SetFrameBufferStatus_dbg(FbgId, FbId, Status, __func__, __LINE__)

#endif

EXTERN VOID FBM_FrameDisplayStart(UCHAR ucFbgId,UCHAR ucFbId);
EXTERN VOID FBM_FlushLockFrameBuffer(UCHAR ucFbgId);

EXTERN UCHAR FBM_GetFrameBufferFromDispQ(UCHAR ucFbgId);

EXTERN UCHAR FBM_PreLookFrameBufferFromDispQ(UCHAR ucFbgId);

EXTERN UCHAR FBM_PreLookNthFBFromDispQ(UCHAR ucFbgId, UCHAR ucTheNth);
EXTERN UCHAR FBM_PreLookBFromDispQ(UCHAR ucFbgId, UCHAR ucTheNth);

EXTERN UCHAR FBM_PreLook2ndFrameBufferFromDispQ(UCHAR ucFbgId);

EXTERN UCHAR FBM_ChkSeamlessMode(UCHAR ucFbgId, UINT32 u4Mode);

EXTERN UCHAR FBM_CheckFrameBufferDispQ(UCHAR ucFbgId);

EXTERN UINT32 FBM_GetNextDispQPts(UCHAR ucFbgId);

EXTERN UCHAR  FBM_CheckFbg(UCHAR ucFbgId);

EXTERN UCHAR FBM_CalFrmRate(UCHAR ucFbgId);

EXTERN VOID FBM_GetFrameBufferAtIsr(UCHAR ucFbgId, UINT32* pu4AddrY, UINT32* pu4AddrC);

EXTERN FBM_PIC_HDR_T *FBM_GetFrameBufferPicHdr(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN FBM_SEQ_HDR_T *FBM_GetFrameBufferSeqHdr(UCHAR ucFbgId);

EXTERN VOID FBM_GetFrameBufferAddr(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC);
EXTERN VOID FBM_GetFrameBufferAddrExt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrYExt, UINT32 *pu4AddrCExt);
EXTERN VOID FBM_GetFrameBufferAddrMv(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrMv);

EXTERN void FBM_GetWorkingBuffer(UCHAR ucFbgId, UINT32 *pu4Addr, UINT32 *pu4Size);

EXTERN void FBM_GetExtraFrameBuffer(UCHAR ucFbgId, UINT32* pu4YAddr, UINT32* pu4CAddr);

EXTERN VOID FBM_QueryStatus(UCHAR ucFbgId);

EXTERN BOOL _FbmQueryFromEmptyQ(UCHAR ucFbgId);

EXTERN BOOL _FbmQueryFromDisplayQ(UCHAR ucFbgId);


#ifdef CC_VDEC_FBM_DYNAMIC_MAPPING
EXTERN VOID * FBM_WorkBufPartMap(UCHAR ucFbgId, UINT32 u4Addr, UINT32 u4Size);
#endif

EXTERN void FBM_UpdateReferenceList(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgReference);

EXTERN UCHAR FBM_GetDecFrameBuffer(UCHAR ucFbgId);

EXTERN VOID FBM_GetRefFrameBuffer(UCHAR ucFbgId, UCHAR* ucFRefFbId, UCHAR* ucBRefFbId);

EXTERN UCHAR FBM_GetEmptyRefFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_GetEmptyBFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_GetEmptyFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN UCHAR FBM_CanGetEmptyRefFrameBuffer(UCHAR ucFbgId);

EXTERN UCHAR FBM_CanGetEmptyBFrameBuffer(UCHAR ucFbgId);

EXTERN VOID FBM_SetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs, UCHAR ucChangeFieldNs);

EXTERN UCHAR FBM_GetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_GetFrameBufferChangeFieldNs(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN UCHAR FBM_ChkFrameBufferLastPlayBack(UCHAR ucFbgId, UCHAR ucFbId);

// This function is called back by mpeg decode to indicate that
// Frame buffer group ID is change. This function will by pass this notify
// to video plane drive
EXTERN VOID FBM_FbgChgNotify(UCHAR ucFbgId, UCHAR ucEsId);

EXTERN VOID FBM_PicStructChgNotify(UCHAR ucEsId, UCHAR ucPicStruct);

EXTERN VOID FBM_RegCbFunc(FBM_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr);
EXTERN VOID FBM_RegFbgCbFunc(UCHAR ucFbgId, FBM_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr, UINT32 u4Tag);

EXTERN FBM_CB_FUNC_T* FBM_GetCbFuncTbl(VOID);

// This function is implemented for mpeg error concealment, it will return FbId in
// display-q state or lock state
EXTERN UCHAR FBM_GetReadyFrameBuffer(UCHAR ucFbgId);

EXTERN VOID FBM_SetSyncStc(UCHAR ucFbgId, UCHAR ucMode, UCHAR ucStcId);

EXTERN VOID FBM_GetSyncStc(UCHAR ucFbgId, UCHAR* pucMode, UCHAR* pucStcId);

EXTERN void FBM_SetPlayMode(UCHAR ucFbgId, UCHAR ucPlayMode);
EXTERN void FBM_GetPlayMode(UCHAR ucFbgId, UCHAR* pucPlayMode);

EXTERN UINT8 FBM_GetDecoderSrcId(UCHAR ucFbgId);

EXTERN UINT8 FBM_GetFbgAppMode(UCHAR ucFbgId);


EXTERN void FBM_SetAVSyncStatus(UCHAR ucFbgId, UINT32 u4AVSyncStatus);

EXTERN void FBM_GetAVSyncStatus(UCHAR ucFbgId, UINT32* pu4AVSyncStatus);

EXTERN FBM_POOL_T *FBM_GetPoolInfo(UCHAR ucPoolType);

EXTERN FBM_POOL_T* FBM_GetPoolInfoAuto(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* pInfo);

EXTERN FBM_POOL_T* FBM_GetPoolMpeg(VOID);

EXTERN FBM_POOL_T* FBM_Alloc(UCHAR);
EXTERN UINT32 FBM_Free(FBM_POOL_T* pAllocedPool);
EXTERN UCHAR FBM_GetPoolCurrent(UCHAR ucPoolType);

EXTERN VOID FBM_CheckPadding(VOID);
#if defined(CC_MT5399) ||defined(CC_MT5890)||defined(CC_MT5882)
EXTERN VOID FBM_CheckBufferdefine(VOID);
#endif

EXTERN VOID FBM_ReleaseDispQ(UCHAR ucFbgId);

EXTERN void FBM_GetEmptyQCount(UCHAR ucFbgId, UCHAR* pucCount);

EXTERN BOOL FBM_ForceReleaseFrameBuffer(UCHAR ucFbgId,UCHAR ucFbId);

#if 1
EXTERN UCHAR FBM_GetDbkSmallBFrameBuffer(UCHAR ucFbgId);
#endif

#if 1
EXTERN void FBM_SetFrameBufferFtrCnt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4DbkOfStCnt, UINT32 u4Dbk4pCnt);

EXTERN VOID FBM_GetFrameBufferFtrCnt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4DbkOfStCnt, UINT32 *pu4Dbk4pCnt);
#endif

EXTERN UCHAR FBM_GetDbkEmptyRefFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

EXTERN INT32 FBM_ResetFbChangeField(UCHAR ucFbgId);

EXTERN INT32 FBM_WaitFbChangeField(UCHAR ucFbgId, UINT32 u4Timeout);

EXTERN VOID FBM_FbChangeField(UCHAR ucFbgId);

EXTERN VOID FBM_SetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs);

EXTERN UCHAR FBM_GetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId);

EXTERN void FBM_SetFrameBufferGlobalFlag(UCHAR ucFbgId, UINT8 ucFlag);
EXTERN UCHAR FBM_ChkFrameBufferGlobalFlagIsr(UCHAR ucFbgId, UINT8 ucFlag);
EXTERN void FBM_ClrFrameBufferGlobalFlag(UCHAR ucFbgId, UINT8 ucFlag);

EXTERN UINT32 FBM_GetVDecMemSize(UCHAR ucFbgId);
EXTERN UINT32 FBM_GetRunningVdecMemSize(UCHAR ucFbgId);

EXTERN UINT32 FBM_GetCabacMemAddr(UCHAR ucFbgId,UCHAR ucCabNs);
EXTERN UINT32  FBM_GetMiscMemAddr(UCHAR ucFbgId);


EXTERN BOOL FBM_ChkFbgCreateFromInst(UCHAR ucFbgId);
EXTERN void FBM_SetColorMode(UINT8 u1ColorMode);
EXTERN void FBM_SetDecoderFB(UINT8 u1DecoderType,UINT8 u1NumOfFB);

#ifdef FBM_VIRTUAL_DISPLAY
EXTERN VOID FBM_VirtualDispStart(UINT32 u4Delay, UINT32 u4VirtualDispPath);

EXTERN VOID FBM_VirtualDispStop(VOID);
#endif

#ifdef FBM_VIRTUAL_MPEG
EXTERN VOID FBM_VirtualMpegStart(UINT32 u4Delay, const CHAR* pcFile);
EXTERN VOID FBM_VirtualMpegStop(VOID);
EXTERN INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);
EXTERN VOID FBM_VirtualINTTest(VOID);
#endif

#ifdef FBM_FB_LOG
EXTERN VOID FBM_FlushFbLog(UINT8 u4FbIdx, UINT8 u4FbStatus);
#endif

#ifdef FBM_CS_LOG
EXTERN VOID FBM_FlushFbCsLog(void);
#endif

EXTERN void FBM_VdecMemProtect(UINT8 u1Mode, BOOL fgDBKDisable);

EXTERN CHAR* FBM_GetPoolName(UCHAR ucPoolType);
EXTERN UINT32 FBM_GetPoolSize(UCHAR ucPoolType);

EXTERN VOID FBM_SetSeqChgFbId(UCHAR ucFbgId, UCHAR ucFbId);
EXTERN BOOL FBM_InsertFB(UCHAR ucFbgId, UINT32 u4Addr);
EXTERN UCHAR FBM_GetFreeFB(UCHAR ucFbgId);
EXTERN UCHAR FBM_GetDisplayFB(UCHAR ucFbgId);
EXTERN BOOL FBM_GetInsertFBCount(UCHAR ucFbgId, UINT32 *pu4Count);
EXTERN VOID FBM_GetFbByAddr(UINT32 u4Addr, UCHAR *pucFbgId, UCHAR *pucFbId);
EXTERN BOOL FBM_ReleaseFBFlush(UCHAR ucFbgId);
EXTERN BOOL FBM_FlushEmptyBufToDispQ(UCHAR ucFbgId);
#if 1
EXTERN void FBM_SetCodecInfo(UCHAR ucFbgId, ENUM_CODEC_INFO_T rCodecInfo);
EXTERN ENUM_CODEC_INFO_T FBM_GetCodecInfo(UCHAR ucFbgId);
EXTERN BOOL FBM_GetCodecResolution(UCHAR ucFbgId,FBM_CODEC_RESOLUTION_T* rResolution);
#endif

// New Seamless
EXTERN VOID FBM_TriggerSmlsByResizer(UCHAR ucFbgId, UCHAR ucFbId);
EXTERN UCHAR FBM_GetResizeFrameBufferNs(UCHAR ucFbgId);
EXTERN VOID FBM_GetResizeFrameBufferSize(UCHAR ucFbgId, UINT32 *pu4Width, UINT32 *pu4Height);
EXTERN UCHAR FBM_GetResizeFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId);
EXTERN UCHAR FBM_CheckResizeFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);
EXTERN VOID FBM_SetResizeFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);
EXTERN VOID FBM_SetResize2FrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);
EXTERN VOID FBM_GetResizeFrameBufferAddr(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC);
EXTERN VOID FBM_GetResize2FrameBufferAddr(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC);
EXTERN UCHAR FBM_GetEmptyResizeFrameBuffer(UCHAR ucFbgId, UCHAR ucSrcFbId, UCHAR ucTaskId, UINT32 u4Delay, UCHAR* pucRetValue);
EXTERN UCHAR FBM_GetEmptyResize2FrameBuffer(UCHAR ucFbgId, UCHAR ucSrcFbId, UCHAR ucTaskId, UINT32 u4Delay, UCHAR* pucRetValue);

EXTERN void FBM_GetResizeEmptyQCount(UCHAR ucFbgId, UCHAR* pucCount);
EXTERN BOOL FBM_IsSeamlessFB(UCHAR ucFbgId, UCHAR ucFbId);

void FBM_SetWfdFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

INT32 FBM_ChkWfdFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

void FBM_ClrWfdFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag);

void FBM_AccumulateWfdFrameCnt(UCHAR ucFbgId);

UINT32 FBM_WfdFrameStatistics(UCHAR ucFbgId);

EXTERN BOOL FBM_SetDispTag(UCHAR ucFbgId, UCHAR ucFbId,UINT32 u4Tag);
EXTERN BOOL FBM_GetFbByDispTag(UINT32 u4Tag, UCHAR *pucFbgId, UCHAR *pucFbId);
EXTERN VOID FBM_ReleaseBufferByDispTag(UINT32 u4Tag);
EXTERN VOID FBM_ReleaseBufferByIndex(UCHAR ucFbgId, UCHAR ucFbId);
EXTERN UCHAR FBM_GetFbgByEs(UCHAR ucEsId);

EXTERN BOOL FBM_GetFixedImgRzBuffer(UINT32 *pu4AddrY, UINT32 *pu4AddrC);
EXTERN BOOL FBM_FreeFixedImgRzBuffer(UINT32 u4AddrY, UINT32 u4AddrC);
EXTERN void FBM_SetPtsSync(UCHAR ucFbgId,BOOL fgAvsync);
EXTERN BOOL FBM_GetPtsSync(UCHAR ucFbgId);
EXTERN BOOL  FBM_FbgValid(UCHAR ucFbgId);
EXTERN BOOL FBM_FbValid(UCHAR ucFbgId,UCHAR ucFbId);
EXTERN BOOL  FBM_DoSeqChanging(UCHAR ucFbgId,BOOL fgValue,BOOL fgQuery);

#ifdef CC_B2R_RES_SUPPORT
EXTERN UINT8 FBM_B2rResIdAccess(UCHAR ucFbgId, 
                                          UINT32 u4Mask,
                                          B2R_IPT_INFO_T* pt_src);
EXTERN UCHAR _FBM_Fbg2B2r(UCHAR ucFbgId);
#endif

typedef enum
{
    SEAMLESS_NONE        = 0,           //None Seamless
    SEAMLESS_ONE_FBS     = (1<<1),      //Seamless with one FBs, Limitation is >ref 4 wll seamless fail.
    SEAMLESS_TWO_FBS     = (1<<2),      //Seamless with two FBs. Limitation is StageFright currenlty can't handle the FB Ns change.
    
    SEAMLESS_2FBS_1POOL  = SEAMLESS_TWO_FBS | (1<<3),      //Seamless with two FBs. with only MPEG 1 pool, conflict FB while seq chg.
    SEAMLESS_2FBS_2POOL  = SEAMLESS_TWO_FBS | (1<<4),      //Seamless with two FBs. with 2 pools. Make sure no multi instance while seq chg.
    
    SEAMLESS_BY_RESIZER  = (1<<5),      //Seamless by resizer. 
    SEAMLESS_BY_NPTV     = (1<<6),      //Seamless by NPTV. Developing now. Limitation is 3D can't seamless by NPTV. and need add 4FB for prepare for NPTV.

    SEAMLESS_ONE_RESIZER = SEAMLESS_BY_RESIZER | (1<<7),      //Seamless with one resizer
    SEAMLESS_TWO_RESIZER = SEAMLESS_BY_RESIZER | (1<<8),      //Seamless with two resizer. Currently for 4K2K only.
    
} FBM_SEAMLESS_MODE;


/* FBM calculated-once size table - begin */
typedef enum
{
    FBM_TYPE_MAIN_PIP_MODE_1080I,
    FBM_TYPE_MAIN_PIP_MODE_576I,
    FBM_TYPE_MAIN_PIP_MODE_1080P_422,
    FBM_TYPE_MAIN_PIP_MODE_1080P_444,
    FBM_TYPE_SUB_PIP_MODE_1080I,
    FBM_TYPE_SUB_PIP_MODE_576I,
    FBM_TYPE_SUB_PIP_MODE_1080P_422,
    FBM_TYPE_SUB_PIP_MODE_1080P_444,
    FBM_TYPE_MAIN_POP_MODE_1080I,
    FBM_TYPE_MAIN_POP_MODE_576I,
    FBM_TYPE_MAIN_POP_MODE_1080P_422,
    FBM_TYPE_MAIN_POP_MODE_1080P_444,
    FBM_TYPE_SUB_POP_MODE_1080I,
    FBM_TYPE_SUB_POP_MODE_576I,
    FBM_TYPE_SUB_POP_MODE_1080P_422,
    FBM_TYPE_SUB_POP_MODE_1080P_444,
    FBM_TYPE_MM_1080I_MODE,
    FBM_TYPE_MM_576I_MODE,
    FBM_TYPE_MM_1080P_MODE,
    FBM_TYPE_ENV_MAX
} FBM_POOL_ENV_TYPE_T;

typedef enum
{
    FBM_MAIN_PSCAN,
    FBM_MAIN_SCALER,
    FBM_SUB_PSCAN,
    FBM_SUB_SCALER,
    FBM_TDC,
    FBM_VIDEO_AGENT_MAX
} FBM_NPTV_VIDEO_AGNET_T;

EXTERN FBM_POOL_T _FBM_NPTV_VIDEO_CONF[FBM_TYPE_ENV_MAX][FBM_VIDEO_AGENT_MAX];
/* FBM calculated-once size table - end */

EXTERN UINT32 SRMFBM_GetConf(void);
#if defined(CC_DYNAMIC_FBMSRM_CONF)
EXTERN void SRMFBM_SetConf(UINT32 conf);
EXTERN void SRMFBM_CHB_SetConf(UINT32 conf);
EXTERN void SRMFBM_CHC_SetConf(UINT32 conf);
EXTERN UINT32 SRMFBM_CHB_GetConf(void);
EXTERN UINT32 SRMFBM_CHC_GetConf(void);

#endif

#if  defined(CC_MT5399) || defined(CC_MT5398)|| defined(CC_MT5882)||defined(CC_MT5890)
typedef enum {
    FLIP_NONE =0,
    FLIP_BY_SCALER = 0x1,
    FLIP_BY_PSCAN  = 0x2,
    FLIP_BY_B2R    = 0x4,
    FLIP_BY_MJC    = 0x8,
    FLIP_BY_3D     = 0x10,
#ifdef DRV_USE_EXTERNAL_3D_FRC // MT8283_FLIP_MIRROR
    FLIP_BY_EXT3DFRC = 0x20
#endif

} FLIP_MODULE_T;
UINT32 u4QueryFlipModule(UINT32 u4VdpId);
#endif

#if defined(CC_MT5395) || defined(CC_MT5396)
UINT8 FBM_IsAddrInChB(UINT32 u4Addr);
#endif

EXTERN void FBM_SetPscanTestMode(BOOL fgIsTestMode);
EXTERN void FBM_SetPscanTestModeSubPIP(BOOL fgIsTestMode);
EXTERN void SRM_SetCurrentFBMMode(UINT32 u4FBMMode);

#if defined(CC_EDB_FBM_SHARE_MEM)
EXTERN VOID FBM_SetEDBSize(UINT8 u1EDBSize);
EXTERN VOID FBM_GetEDBAddr(UINT32* pu4EDBAddr);
#endif

#if defined(CC_FBM_2ND_CHANNEL)
EXTERN UINT32 FBM_GetCHBStartAddr(void);
#define DRAM_CHB_START_ADDR FBM_GetCHBStartAddr()
#endif
#if defined(CC_FBM_3ND_CHANNEL)
EXTERN UINT32 FBM_GetCHCStartAddr(void);
#define DRAM_CHC_START_ADDR FBM_GetCHCStartAddr()
#endif

EXTERN UINT32 FBM_GetCaptureDstAddr(void);

void FBM_FixedScalerDram(UINT8 fgOnOff);
UINT8 FBM_GetFixedScalerDram(void);

BOOL fgFBM_MDDI_FIFO_LIMIT(UINT32 u4VdpId);

#endif    // FBM_DRVIF_H

