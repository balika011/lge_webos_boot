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
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: osd_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_drvif.h
 *  This header file declares public function prototypes of OSD.
 */

#ifndef OSD_DRVIF_H
#define OSD_DRVIF_H

/*lint -e717 -e572*/

/*
 e717 : do ... while(0)
 e572 : Excessive shift value (precision Integer shifted right by Integer)
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_assert.h"
#include "x_common.h"
#include "x_os.h"
#include "x_printf.h"
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif
LINT_EXT_HEADER_END

#if defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389)
#define OSD_IC_GROUP_ONE            (0x1)
#elif defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5860)||defined(CC_MT5881)
#define OSD_IC_GROUP_TWO            (0x2)
#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
#define OSD_IC_GROUP_THREE          (0x3)
#else
#endif

#if defined(CC_MT5396)                                   /* MT5396 Feature */
#define OSD_PLA1_SUPPORT
#define OSD_BASE_SCRN_VSTART_TOP    (16)
#define OSD_BASE_HSART_ADJ          (-2)
#define OSD_BASE_HSART_ADJ_CSR      (0)
#define OSD_BASE_HS_EDGE            (0)
#define OSD_BASE_VS_EDGE            (1)
#define OSD_BASE_UPDATE_VIDEO_DATA  (1)
#define OSD_BASE_SYNC_SRC_SEL       (0)


#elif defined(CC_MT5368)                                 /* MT5368 Feature */
#define OSD_BASE_SCRN_VSTART_TOP    (16)
#define OSD_BASE_HSART_ADJ          (0x294)
#define OSD_BASE_HSART_ADJ_CSR      (0)
#define OSD_BASE_HS_EDGE            (0)
#define OSD_BASE_VS_EDGE            (1)
#define OSD_BASE_SYNC_SRC_SEL       (0)


#elif defined(CC_MT5389)                                 /* MT5389 Feature */
#define OSD_BASE_SCRN_VSTART_TOP    (17)
#define OSD_BASE_HSART_ADJ          (0x1ae)
#define OSD_BASE_HSART_ADJ_CSR      (0x1aa)
#define OSD_BASE_HS_EDGE            (0)
#define OSD_BASE_VS_EDGE            (1)
#define OSD_BASE_SYNC_SRC_SEL       (0)


#elif defined(CC_MT5398)                                 /* MT5398 Feature */
#define OSD_PLA1_SUPPORT
#ifdef CC_GPU_SUPPORT
#define OSD_BASE_SCRN_VSTART_TOP    (62)
#define OSD_BASE_HSART_ADJ          (fgIsMJCToOSTG()?(-2):(30))
#else
#define OSD_BASE_HSART_ADJ          (fgIsMJCToOSTG()?(-2):(-8))
#ifdef CC_FBM_BYPASS_MJC
#define OSD_BASE_SYNC_SRC_SEL       (2)
#define OSD_BASE_SCRN_VSTART_TOP    (-1)
#else
#define OSD_BASE_SYNC_SRC_SEL       (0)
#define OSD_BASE_SCRN_VSTART_TOP    (6)
#endif
#endif
#define OSD_BASE_HSART_ADJ_CSR      (0)
#define OSD_BASE_HS_EDGE            (0)
#define OSD_BASE_VS_EDGE            (1)



#elif defined(CC_MT5880)                                 /* MT5880 Feature */
#define OSD_BASE_SCRN_VSTART_TOP    (0x9)
#define OSD_BASE_HSART_ADJ          (IS_IC_5880()?(0x1df):(0x1E1))
#define OSD_BASE_HSART_ADJ_CSR      (0x1aa)
#define OSD_BASE_HS_EDGE            (1)
#define OSD_BASE_VS_EDGE            (0)
#define OSD_BASE_SYNC_SRC_SEL       (0)


#elif defined(CC_MT5881)                                 /* MT5881 Feature */
#define OSD_BASE_SCRN_VSTART_TOP    (0x9)
#define OSD_BASE_HSART_ADJ          (0x142)
#define OSD_BASE_HSART_ADJ_CSR      (0)
#define OSD_BASE_HS_EDGE            (1)
#define OSD_BASE_VS_EDGE            (0)
#define OSD_BASE_SYNC_SRC_SEL       (0)

#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
#define OSD_PLA1_SUPPORT
#if defined(CC_MT5890) || defined(CC_MT5882)
#define OSD_BASE_SCRN_VSTART_TOP    (0x2)
#else
#define OSD_BASE_SCRN_VSTART_TOP    (0x6)// internal clock (0x24)
#endif
#define OSD_BASE_HSART_ADJ          (0xb)
#define OSD_BASE_HSART_ADJ_CSR      (0x90)
#define OSD_BASE_HS_EDGE            (0)
#define OSD_BASE_VS_EDGE            (1)
#define OSD_BASE_SYNC_SRC_SEL       (0)


#else
#define OSD_BASE_SCRN_VSTART_TOP    (9)
#define OSD_BASE_HSART_ADJ          (0)
#define OSD_BASE_HSART_ADJ_CSR      (0)
#define OSD_BASE_HS_EDGE            (1)
#define OSD_BASE_VS_EDGE            (0)
#define OSD_BASE_SYNC_SRC_SEL       (0)

#endif

#if (defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5398) || defined(CC_MT5882))
#define OSD_4K2K_SUPPORT
#endif

#if defined(OSD_4K2K_SUPPORT)
#define OSD_MAX_WIDTH          (8192)
#define OSD_MAX_HEIGHT         (4096)
#else
#define OSD_MAX_WIDTH          (2048)
#define OSD_MAX_HEIGHT         (2048)
#endif


#if defined(OSD_IC_GROUP_ONE)                           /* Only group one  Feature */
#define CURSOR_SUPPORT
#define OSD_BASE_SCRN_HS_EDGE       (0)

#elif defined(OSD_IC_GROUP_TWO)                           /* Only group two  Feature */
#define OSD_BASE_INIT_FRC_HEADER
#define OSD_SCALER_DOWN_SUPPORT
#define OSD_PLA4_SUPPORT
#define OSD_PLA5_SUPPORT
#define OSD_BASE_SCRN_HS_EDGE        (fgIsMJCToOSTG()?(1):(0))

#elif defined(OSD_IC_GROUP_THREE)
#define CURSOR_SUPPORT
#define OSD_BASE_INIT_FRC_HEADER
#define OSD_SCALER_DOWN_SUPPORT
#define OSD_PLA4_SUPPORT
#define OSD_PLA5_SUPPORT
#define OSD_BASE_SCRN_HS_EDGE        (fgIsMJCToOSTG()?(1):(0))

#else
#define OSD_BASE_SCRN_HS_EDGE       (0)

#endif

#define OSD_LOCAL_ABITOR_RESET
#define OSD_RESET_BASE_ZERO
#define OSD_CSR_H_ACTIVE_SUPPORT

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#ifndef LG_OSD_PATH_TEST
#define  LG_OSD_PATH_TEST 0
#endif

#define OSD_DMA_ALIGN_SIZE             63  // 64 byte alignment
#define OSD_INVALID_VALUE_U32          (0xFFFFFFFF)

#define OSD_BASE_REG                   OSD_BASE

#define OSD_SC_CLOCK_BASE              0xf000d000
#define FSC_OSD_VSYNC_SEL              0xf0025350
#define FSC_OSD_POST_SCALER_SET        0xf005e350
#define OSD_POST_SCALER_SET_H_REVERSE  0xf005e344



// skip update and reset register
#define OSD_BASE_SKIP                  2
#define OSD_BASE_REG_NUM               18
#define OSD_BASE_SKIP_START            11
#define OSD_BASE_SKIP_END              14

#define OSD_PLA1_REG                   (OSD_BASE_REG + 0x100)
#define OSD_PLA1_RGN_REG               (OSD_PLA1_REG + 0x40)
#define OSD_PLA2_REG                   (OSD_BASE_REG + 0x200)
#define OSD_PLA3_REG                   (OSD_BASE_REG + 0x300)
#define OSD_PLA2_REG_OFF               (0x200)
#define OSD_PLA3_REG_OFF               (0x300)
#define OSD_SC2_REG_OFF                (0x500)
#define OSD_SC3_REG_OFF                (0x600)
#define OSD_DEC_DELAY_MAX              (0x20)
#define OSD_PLA4_REG                   (OSD_BASE_REG + 0x900)
#define OSD_PLA5_REG                   (OSD_BASE_REG + 0x700)  //for cursor

#define OSD_3D_REG                     (OSD_BASE_REG + 0x80)
#define OSD_SC1_REG                    (OSD_BASE_REG + 0x400)
#define OSD_SC2_REG                    (OSD_BASE_REG + 0x500)
#define OSD_SC3_REG                    (OSD_BASE_REG + 0x600)
#define OSD_FIFO_MONITOR_REG           (OSD_BASE_REG + 0xFC)

#define OSD_SC_REG_NUM                 9
#define OSD_SC_STEP_BIT                14
#define OSD_SC_STEP_BASE               0x4000

#define OSD_LPF_PARAM_NUM              3
#define OSD_DEFAULT_LPF_C1             -7
#define OSD_DEFAULT_LPF_C2             28
#define OSD_DEFAULT_LPF_C3             88
#define OSD_DEFAULT_LPF_C4             0
#define OSD_DEFAULT_LPF_C5             0

#define OSD_CSR_REG                    (OSD_BASE_REG + 0x700)

#define OSD_CSR_DATA_REG               (OSD_BASE_REG + 0x800)
#define OSD_CSR_DATA_SIZE              0x100
#define OSD_CSR_COLOR_REG              (OSD_CSR_REG + 0x4)
#ifdef OSD_CSR_H_ACTIVE_SUPPORT
#define OSD_CSR_HACTIVE_REG              (OSD_CSR_REG + 0x40)
#define OSD_CSR_POSITION                 (OSD_CSR_REG + 0x44)
#endif

#define OSD_READ32(offset)                  IO_READ32(OSD_BASE_REG,(offset))
#define OSD_WRITE32(offset, value)          IO_WRITE32(OSD_BASE_REG,(offset),(value))
#define OSD_CSR_REG_NUM                1

#define OSD_RGN_REG_NUM                8
#define OSD_PLA_REG_NUM                7

#ifdef CC_MINI_DRIVER
#define OSD_MAX_NUM_RGN            10
#define OSD_MAX_NUM_RGN_LIST       10
#else
#define OSD_MAX_NUM_RGN            120
#define OSD_MAX_NUM_RGN_LIST       90
#endif
#define OSD_CK_REG                     IO_ADDR(0xd220)
#define OSD_CKEN_REG                   IO_ADDR(0xd2a8)

#define OSD_DE_S_REG                   IO_ADDR(0x8094)
#define OSD_DE_S_OSD2_VAL               (0x91)
#define OSD_DE_S_OSD3_VAL               (0xd1)
#define OSD_FIFO_MONITOR_OFFSET         (0xFC)


#define OSD_CK_XTAL                    0
#define OSD_CK_SYS                     1 // 162 or 216Mhz
#define OSD_CK_TVD                     1 //tvd:180Mhz
#define OSD_CK_OCLK                    6 //vop
#define OSD_FMT_08                     (OSD_BASE_REG + 0x8)
#define YUV_OUTPUT                     (1 << 9)

#define OSD_FIELD_TOP                  1
#define OSD_FIELD_BOT                  0
#define OSD_FRAME_MODE                 2

#define OSD_INT_ENABLE_REG         (OSD_BASE_REG + 0x30)
#define OSD1_CFUD_IEN              (1 << 8)
#define OSD2_CFUD_IEN              (1 << 7)
#define OSD2_PFUD_IEN              (1 << 6)
#define OSD2_VUTO_IEN              (1 << 5)
#define OSD2_VDTO_IEN              (1 << 4)
#define OSD3_CFUD_IEN              (1 << 3)
#define OSD3_PFUD_IEN              (1 << 2)
#define OSD3_VUTO_IEN              (1 << 1)
#define OSD3_VDTO_IEN              (1 << 0)
#define OSD_INT_STATUS_REG         (OSD_BASE_REG + 0x34)
#define OSD_INT                    (1 << 16)
#define OSD1_CFUD_IID              (1 << 8)
#define OSD2_CFUD_IID              (1 << 7)
#define OSD2_PFUD_IID              (1 << 6)
#define OSD2_VUTO_IID              (1 << 5)
#define OSD2_VDTO_IID              (1 << 4)
#define OSD3_CFUD_IID              (1 << 3)
#define OSD3_PFUD_IID              (1 << 2)
#define OSD3_VUTO_IID              (1 << 1)
#define OSD3_VDTO_IID              (1 << 0)

#define OSD_CHECKSUM               (OSD_BASE_REG + 0x50)

#if defined(OSD_4K2K_SUPPORT)
#define OSD_SC_MAX_SRCWIDTH         4096
#define OSD_SC_MAX_SRCHEIGHT        2160
#else
#define OSD_SC_MAX_SRCWIDTH         1920
#define OSD_SC_MAX_SRCHEIGHT        1080
#endif
#define OSD_INVALID_VALUE_U32       (0xFFFFFFFF)

#define OSD_DE_S_OSD2_VAL           (0x91)
#define OSD_DE_S_OSD3_VAL           (0xd1)
#define OSD_DE_ADJ_VAL              (0xc)

#define OSD_BASE_MONITOR            (0xfc)
#define UPDATE_TIMING_BIT           (1 << 23)
#define DISABLE_SW_UPDATE           (1<<5)


//#define CC_CONFIG_OSD_SBS_PLANE1
//#define CC_CONFIG_OSD_SBS_PLANE2
//#define CC_CONFIG_OSD_SBS_PLANE3

//#define CC_CONFIG_OSD_TB_PLANE1
//#define CC_CONFIG_OSD_TB_PLANE2
//#define CC_CONFIG_OSD_TB_PLANE3


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


/** OSD plane enum.
 */
typedef enum
{
    OSD_PLANE_1 = 0x0,
    OSD_PLANE_2 = 0x1,
    OSD_PLANE_3 = 0x2,
    OSD_PLANE_4 = 0x3,
    OSD_PLANE_5 = 0x4,
    OSD_PLANE_MAX_NUM
} OSD_PLANE_T;

/** OSD scaler enum.
 */
typedef enum
{
    OSD_SCALER_2, OSD_SCALER_3, OSD_SCALER_1, OSD_SCALER_MAX_NUM
} OSD_SCALER_T;

/** OSD function return code.
 */
typedef enum
{
    OSD_RET_OK,
    OSD_RET_INV_ARG,
    OSD_RET_OUT_OF_MEM,
    OSD_RET_OUT_OF_REGION,
    OSD_RET_OUT_OF_LIST,
    OSD_RET_UNINIT,
    OSD_RET_INV_PLANE,
    OSD_RET_INV_SCALER,
    OSD_RET_INV_REGION,
    OSD_RET_INV_BITMAP,
    OSD_RET_INV_LIST,
    OSD_RET_INV_DISPLAY_MODE,
    OSD_RET_NO_ATTACH_REGION,
    OSD_RET_REGION_COLLISION,
    OSD_RET_ERR_INTERNAL,
    OSD_RET_UNDEF_ERR
} OSD_RET_CODE_T;

/**OSD 4K2K information
*/
#ifdef OSD_4K2K_SUPPORT
typedef struct _OSD_4K2KOriInformation
{
    UINT32 u4OSDPlane;
    UINT32 u4RegionId;
    UINT32 u4OriRegionWidth;
    UINT32 u4OriRegionHeight;
    UINT32 u4OriRegionPitch;
    UINT32 u4OriRegionAddr;
    UINT32 u4OriScalerEnable;
    UINT32 u4OriScalerSrcWidth;
    UINT32 u4OriScalerSrcHeight;
    UINT32 u4OriScalerDstWidth;
    UINT32 u4OriScalerDstHeight;
    UINT32 u4OriScalerIs16Bpp;
}OSD_4K2KOriInformation;
typedef struct _OSD_4K2KJpecModeCurrentSet
{
    UINT32 u4FirstFrameAddress;
    UINT32 u4SecondFrameAddress;
    UINT32 u4ThirdFrameAddress;
    UINT32 u4ForthFrameAddress;
}OSD_4K2KJpecModeCurrentSet;

typedef struct _OSD_4K2KMvcModeCurrentSet
{
    UINT32 u4LeftFrameAddress;
    UINT32 u4RightFrameAddress;
}OSD_4K2KMvcModeCurrentSet;
#endif
/**OSD 4k2k mode 
*/
typedef enum
{
    OSD_4K2K_MODE_NONE=0,
    OSD_4K2K_MODE_HDMI_4K2K=11,
    OSD_4K2K_MODE_MVC,
    OSD_4K2K_MODE_JPEG
}OSD_4K2K_MODE_T;

/** OSD color mode.
 */
typedef enum
{
    OSD_CM_YCBCR_CLUT2,
    OSD_CM_YCBCR_CLUT4,
    OSD_CM_YCBCR_CLUT8,
    OSD_CM_RESERVED_0,
    OSD_CM_CBYCRY422_DIRECT16,
    OSD_CM_YCBYCR422_DIRECT16,
    OSD_CM_AYCBCR8888_DIRECT32,
    OSD_CM_RESERVED_1,
    OSD_CM_RGB_CLUT2,
    OSD_CM_RGB_CLUT4,
    OSD_CM_RGB_CLUT8,
    OSD_CM_RGB565_DIRECT16,
    OSD_CM_ARGB1555_DIRECT16,
    OSD_CM_ARGB4444_DIRECT16,
    OSD_CM_ARGB8888_DIRECT32,
    OSD_CM_ALPHA_BIT
} OSD_COLOR_MODE_T;

/** OSD display mode. for FPGA emulation
 */
typedef enum
{
    OSD_DM_480I,
    OSD_DM_576I,
    OSD_DM_480P,
    OSD_DM_576P,
    OSD_DM_720P,
    OSD_DM_768P,
    OSD_DM_1080I,
    OSD_DM_1080P,
    OSD_DM_MAX_NUM
} OSD_DISPLAY_MODE_T;

/** OSD blending mode.
 */
typedef enum
{
    OSD_BM_NONE, OSD_BM_PIXEL, OSD_BM_REGION, OSD_BM_PLANE
} OSD_BLEND_MODE_T;

/** OSD region list cmd.
 */
typedef enum
{
    OSD_RGN_LIST_HEAD, OSD_RGN_LIST_TAIL, OSD_RGN_LIST_COUNT, OSD_LIST_FLAGS
} OSD_RGN_LIST_CMD_T;

/** OSD region cmd.
 */
typedef enum
{
    OSD_RGN_ALLOC,
    OSD_RGN_PREV,
    OSD_RGN_NEXT,
    OSD_RGN_FLAGS,
    OSD_RGN_POS_X,
    OSD_RGN_POS_Y,
    OSD_RGN_BMP_W,
    OSD_RGN_BMP_H,
    OSD_RGN_DISP_W,
    OSD_RGN_DISP_H,
    OSD_RGN_OUT_W,
    OSD_RGN_OUT_H,
    OSD_RGN_COLORMODE,
    OSD_RGN_ALPHA,
    OSD_RGN_BMP_ADDR,
    OSD_RGN_BMP_PITCH,
    OSD_RGN_CLIP_V,
    OSD_RGN_CLIP_H,
    OSD_RGN_V_FLIP,
    OSD_RGN_H_MIRROR,
    OSD_RGN_PAL_LOAD,
    OSD_RGN_PAL_ADDR,
    OSD_RGN_PAL_LEN,
    OSD_RGN_STEP_V,
    OSD_RGN_STEP_H,
    OSD_RGN_COLOR_KEY,
    OSD_RGN_COLOR_KEY_EN,
    OSD_RGN_MIX_SEL,
    OSD_RGN_BIG_ENDIAN,
    OSD_RGN_ALPHA_SEL,
    OSD_RGN_YR_SEL,
    OSD_RGN_UG_SEL,
    OSD_RGN_VB_SEL,
    OSD_RGN_NEXT_EN,
    OSD_RGN_NEXT_HDR_ADDR,
    OSD_RGN_FIFO_EX,
    OSD_RGN_HPX_EXT,
    OSD_RGN_ENDIAN,
    OSD_RGN_HINTP_MODE,
    OSD_RGN_VINTP_MODE,
    OSD_RGN_HSTEP_OFFSET,
    OSD_RGN_VSTEP_OFFSET,
    OSD_RGN_DEC_MODE,
    OSD_RGN_DEC_LINE,
    OSD_RGN_DEC_EN
} OSD_RGN_CMD_T;

typedef enum
{
    OSD_FRCHD_FORMAT,
    OSD_FRCHD_PIXORDER,
    OSD_FRCHD_LRINDEX,
    OSD_FRCHD_BMPINDEX,
    OSD_FRCHD_FRAMENUM,
    OSD_FRCHD_PREFIX,
    OSD_FRCHD_BMPHEIGHT,
    OSD_FRCHD_BMPWIDTH,
    OSD_FRCHD_GLOBALALPHA,
    OSD_FRCHD_PIXELALPHAEN,
    OSD_FRCHD_GLOBALALPHAEN,
    OSD_FRCHD_ODPUPPER,
    OSD_FRCHD_DISPYOFF,
    OSD_FRCHD_DISPXOFF,
    OSD_FRCHD_DISPHEIGHT,
    OSD_FRCHD_DISPWIDTH
} OSD_FRCHEADER_CMD_T;

typedef enum _OSD_CPBLTY_E
{
    E_OSD_CPBLTY_NONE,
    E_OSD_CPBLTY_CMPRSS,
    E_OSD_CPBLTY_CMPRSS_PLANE,
    E_OSD_CPBLTY_SBS_PLANE,
    E_OSD_CPBLTY_3D_STATUS,
    E_OSD_CPBLTY_FLIP,
    E_OSD_CPBLTY_MIRROR,
    E_OSD_CPBLTY_MAX
} OSD_CPBLTY_E;

typedef enum _OSD_RGN_STATE_E
{
    OSD_RGN_STATE_IDLE,
    OSD_RGN_STATE_DISP,
    OSD_RGN_STATE_CMP_DISP,
    OSD_RGN_STATE_CMP,
    OSD_RGN_STATE_UNCMP,
    OSD_RGN_STATE_MAX
}OSD_RGN_STATE_E;


/** OSD base register bit-field type.
 */

typedef struct _OSD_CPBLTY
{
    UINT32 u4_cmprss;
    UINT32 u4_cmprss_plane[OSD_PLANE_MAX_NUM];
    UINT32 u4_sbs_plane[OSD_PLANE_MAX_NUM];
    UINT32 u4_3d_status[OSD_PLANE_MAX_NUM];
    UINT32 u4_flip;
    UINT32 u4_mirror;
}OSD_CPBLTY;

typedef struct _OSD_RGN_CREATE_T
{
    UINT32 * pu4Region;
    UINT32 u4BmpWidth;
    UINT32 u4BmpHeight;
    void * pvBitmap;
    UINT32 eColorMode;
    UINT32 u4BmpPitch;
    UINT32 u4DispX;
    UINT32 u4DispY;
    UINT32 u4DispW;
    UINT32 u4DispH;
    UINT32 u4Flip;
    UINT32 u4Mirror;
    UINT32 u4Dec;
}OSD_RGN_CREATE_T;


/** OSD region register bit-field type.
 */

#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
typedef struct _OSD_RGN_FIELD_T
{
    /* DWORD - 000 */
    UINT32 u4NextOsdAddr :28;
    UINT32 u4ColorMode :4;
    /* DWORD - 004 */
    UINT32 u4DataAddr :28;
    UINT32 fgColorKeyEn :1;
    UINT32 :1;
    UINT32 u4MixSel :2;
    /* DWORD - 008 */
    UINT32 u4LineSize :11;
    UINT32 :5;
    UINT32 u4MixWeight :8;
    UINT32 u4VbSel :2;
    UINT32 u4UgSel :2;
    UINT32 u4YrSel :2;
    UINT32 u4AlphaSel :2;
    /* DWORD - 00C */
    UINT32 :22;
    UINT32 fgNewPalette :1;
    UINT32 fgNextOsdEn :1;
    UINT32 fgDecMode :2;
    UINT32 u4PaletteLen :2;
    UINT32 fgDecEn :1;
    UINT32 fgDecLine :1;
    UINT32 fgHMirror :1;
    UINT32 fgVFlip :1;
    /* DWORD - 010 */
    UINT32 u4PaletteAddr :28;
    UINT32 :4;
    /* DWORD - 014 */
    UINT32 u4HStep :16;
    UINT32 u4VStep :16;
    /* DWORD - 018 */
    UINT32 :32;
    /* DWORD - 01C */
    UINT32 u4ColorKey :32;
    /* DWORD - 020 */
    UINT32 u4HClip :13;
    UINT32 :3;
    UINT32 u4VClip :12;
    UINT32 :4;
    /* DWORD - 024 */
    UINT32 u4Ihw :13;
    UINT32 :3;
    UINT32 u4Ivw :12;
    UINT32 :4;
    /* DWORD - 028 */
    UINT32 u4Ovw :12;
    UINT32 :4;
    UINT32 u4Ovs :12;
    UINT32 :4;
    /* DWORD - 02c */
    UINT32 u4Ohw :13;
    UINT32 :3;
    UINT32 u4Ohs :13;
    UINT32 :3;
    LINT_SUPPRESS_NEXT_EXPRESSION(950)
}__attribute__ ((packed)) OSD_RGN_FIELD_T;
#else

typedef struct _OSD_RGN_FIELD_T
{
    /* DWORD - 000 */
    UINT32 u4NextOsdAddr :27;
    UINT32 fgNextOsdEn :1;
    UINT32 u4ColorMode :4;

    /* DWORD - 004 */
    UINT32 u4DataAddr :24;
    UINT32 u4MixWeight :8;

    /* DWORD - 008 */
    UINT32 u4HClip :12;
    UINT32 u4VClip :11;
    UINT32 fgLineSize9 :1;
    UINT32 data_adr28 :2;
    UINT32 u4UgSel :2;
    UINT32 u4YrSel :2;
    UINT32 u4AlphaSel :2;

    /* DWORD - 00C */
    UINT32 u4PaletteAddr :24;
    UINT32 fgDecMode :2;
    UINT32 u4PaletteLen :2;
    UINT32 fgNewPalette :1;
    UINT32 fgDecLine :1;
    UINT32 fgHMirror :1;
    UINT32 fgVFlip :1;

    /* DWORD - 010 */
    UINT32 u4Ihw :12;
    UINT32 u4Ivw :11;
    UINT32 u4LineSize :9;

    /* DWORD - 014 */
    UINT32 u4HStep :16;
    UINT32 u4VStep :16;

    /* DWORD - 018 */
    UINT32 u4Ovw :11;
    UINT32 fgDecEn :1;
    UINT32 u4Ovs :11;
    UINT32 fgColorKeyEn :1;
    UINT32 u4ColorKey :8;

    /* DWORD - 01C */
    UINT32 u4Ohw :11;
    UINT32 :1;
    UINT32 u4Ohs :11;
    UINT32 pal_adr28 :4;
    UINT32 data_adr30 :2;
    UINT32 nextOsd_adr31 :1;
    UINT32 u4MixSel :2;

    LINT_SUPPRESS_NEXT_EXPRESSION(950)
}__attribute__ ((packed)) OSD_RGN_FIELD_T;
#endif
typedef struct _OSD_BASE_FIELD_T
{
    /* DWORD - 000 */
    UINT32 fgUpdate :1;
    UINT32 fgAlwaysUpdate :1;
    UINT32 fgForceCLKEn :1;
    UINT32 fgTveUpdate :1;
    UINT32 fgOSD3FifoSel :1;
    UINT32 :27;

    /* DWORD - 004 */
    //UINT32          fgResetFsm          :   1;
    UINT32 fgResetAll :2;
    UINT32 :30;

    /* DWORD - 008 */
    UINT32 fgHsEdge :1;
    UINT32 fgVsEdge :1;
    UINT32 fgFldPol :1;
    UINT32 fgPrgs :1;
    UINT32 fgExtTGen :1;
    UINT32 fgIntTGen :1;
    UINT32 fgChecksumEn :1;
    UINT32 fgOSD12Ex :1;
    UINT32 fgAutoPos :1;
    UINT32 fgYuvOut :1;
    UINT32 :2;
    UINT32 u4Sc2ChkSumSel :2;
    UINT32 fgSyncSrcSel :2;
    UINT32 fgTGenMethod :1;
    UINT32 fgCMPREn :1;
    UINT32 fgTVEEn :1;
    UINT32 fgYCC709En :1;
    UINT32 fgAlphaBackEn :1;
    UINT32 :3;
    UINT32 fgTransferCtl :2;
    UINT32 fgOSD1Switch :2;
    UINT32 fgOSD2Switch :2;
    UINT32 fgOSD3Switch :2;

    /* DWORD - 00C */
    UINT32 u4Ovt :11;
    UINT32 :5;
    UINT32 u4Oht :12;
    UINT32 :4;

    /* DWORD - 010 */
    UINT32 u4VsWidth :8;
    UINT32 u4HsWidth :8;
    UINT32 u4ScrnHStartOsd1 :13;
    UINT32 u4DelayAdj :3;

    /* DWORD - 014 */
    UINT32 u4ScrnHStartOsd3 :13;
    UINT32 :3;
    UINT32 u4ScrnHStartOsd2 :13;
    UINT32 :3;

    /* DWORD - 018 */
    UINT32 u4ScrnVStartBot :11;
    UINT32 :5;
    UINT32 u4ScrnVStartTop :8;
    UINT32 fgAutoPosFld :1;
    UINT32 :7;

    /* DWORD - 01C */
    UINT32 u4ScrnVSize :14;
    UINT32 :2;
    UINT32 u4ScrnHSize :14;
    UINT32 :2;

    /* DWORD - 020 */
    UINT32 u4Osd1VStart :11;
    UINT32 :5;
    UINT32 u4Osd1HStart :11;
    UINT32 :5;

    /* DWORD - 024 */
    UINT32 u4Osd2VStart :11;
    UINT32 :5;
    UINT32 u4Osd2HStart :11;
    UINT32 :5;

    /* DWORD - 028 */
    UINT32 u4Osd3VStart :11;
    UINT32 :5;
    UINT32 u4Osd3HStart :11;
    UINT32 :5;

    /* DWORD - 02C */
    UINT32 :32;
    /* DWORD - 030 */
    UINT32 :32;
    /* DWORD - 034 */
    UINT32 :32;
    /* DWORD - 038 */
    UINT32 :16;
    UINT32 u4ScrnHStartCSR :15;
    UINT32 :1;
    /* DWORD - 03C */
    UINT32 :32;
    /* DWORD - 040 */
    UINT32 :32;
    /* DWORD - 044 */
    UINT32 u4LineShiftEn :1;
    UINT32 u4LineShiftINV :1;
    UINT32 u4PlaneSwitchEn :1;
    UINT32 u4PlaneSwitchInv :1;
    UINT32 u4AutoLR :1;
    UINT32 :1;
    UINT32 u4LineDup :1; //3D line double
    UINT32 :1;
    UINT32 u4Plane2ShiftValue :8;
    UINT32 u4Plane3ShiftValue :8;
    UINT32 :7;
    UINT32 u4LRFlag :1;
    /* DWORD - 048 */
    UINT32 u4VcntStrobe:14;
    UINT32 :2;
    UINT32 u4HcntStrobe:14;
    UINT32 :2;
    /* DWORD - 04c */
    UINT32 u4VcntStatus:14;
    UINT32 :2;
    UINT32 u4HcntStatus:14;
    UINT32 :2;

    LINT_SUPPRESS_NEXT_EXPRESSION(950)
}__attribute__ ((packed)) OSD_BASE_FIELD_T;

typedef struct _OSD_3D_BASE_FIELD_T
{
    /* DWORD - 080 */
    UINT32 fgOsd2LShiftEn :1;
    UINT32 fgOsd2LShiftInv :1;
    UINT32 fgPlaneSwitchEn :1;
    UINT32 fgPlaneSwitchInv :1;
    UINT32 fgTgenLREn :1;
    UINT32 fgTgenLLRREn :1;
    UINT32 fgTgenLRDly :1;
    UINT32 fgTgenLRInv :1;
    UINT32 fgShutterShiftEn :1;
    UINT32 :1;
    UINT32 fgOsd1LShiftEn :1;
    UINT32 fgOsd1LShiftInv :1;
    UINT32 fgOsd3LShiftEn :1;
    UINT32 fgOsd3LShiftInv :1;
    UINT32 fgCsrLShiftEn :1;
    UINT32 fgCsrLShiftInv :1;
    UINT32 fgTwoChEn :1;
    UINT32 fgTwoMaskDummy :1;
    UINT32 :5;
    UINT32 fgShutterLRFlag :1;
    UINT32 :8;

    /* DWORD - 084 */
    UINT32 fgOsd1ShiftValue :8;
    UINT32 fgOsd2ShiftValue :8;
    UINT32 fgOsd3ShiftValue :8;
    UINT32 fgCsrShiftValue :8;

    LINT_SUPPRESS_NEXT_EXPRESSION(950)
}__attribute__ ((packed)) OSD_3D_BASE_FIELD_T;

/** OSD plane register bit-field type.
 */
typedef struct _OSD_PLA_FIELD_T
{
    /* DWORD - 000 */
    UINT32 fgEn :1;
    UINT32 :1;
    UINT32 fgFakeHdr :1;
    UINT32 fgPrngEn :1;
    UINT32 :1;
    UINT32 :1;
    UINT32 :1;
    UINT32 fgPrngMode :1;
    UINT32 fgAEdgeDetEn :1;
    UINT32 fgCLKeySel :1;
    UINT32 :1;
    UINT32 fgFastMute :1;
    UINT32 fgPremulEn :1;

    UINT32 fgShutter :1;
    #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    UINT32 : 1;
    UINT32 fgEnHwRstDec :1;
    UINT32 : 4;
    UINT32 fgEnNewFastMute :1;
    UINT32 fgEnHdrIrq :1;
    UINT32 fgClrHdrIrq :1;
    UINT32 : 1;
    UINT32 fgHdrIrqStatus :1;
	UINT32 : 7;
    #else
    UINT32 :18;
    #endif

    /* DWORD - 004 */
    UINT32 u4HeaderAddr :28; //5363 address 25bits
    UINT32 :4;

    /* DWORD - 008 */
    UINT32 u4Blending :8;
    UINT32 u4Fading :8;
    UINT32 fgHFilter :1;
    UINT32 fgColor_Exp_sel :1;
    UINT32 :14;

    /* DWORD - 00c */
    UINT32 u4ContReqLmt :4;
    UINT32 u4FifoSize :4;
    UINT32 u4VacTh :4;
    UINT32 u4PauseCnt :4;
    UINT32 u4ContReqLmt0 :4;
    UINT32 fgBurstDis :1;
    UINT32 :1;
    UINT32 :1;
    UINT32 :1;
    UINT32 LarbReqEn :1;
    UINT32 :1;
    UINT32 :1;
    UINT32 :1;
    UINT32 LarbTim :4;

    /* DWORD - 010 */
    UINT32 :32;

    /* DWORD - 014 */
    UINT32 u4RealHeaderAddr :28; //5363 address 25bits
    UINT32 :4;
    /* DWORD - 018 */
    UINT32 u4RightHeaderAddr :28;
    UINT32 :4;
    /* DWORD - 01C */
    UINT32 :32;
    /* DWORD - 020 */
    OSD_RGN_FIELD_T t_rgn;

    LINT_SUPPRESS_NEXT_EXPRESSION(950)
}__attribute__ ((packed)) OSD_PLA_FIELD_T;

/** OSD scaler register bit-field type.
 */
typedef struct _OSD_SC_FIELD_T
{
    /* DWORD - 000 */
    UINT32 :2;
    UINT32 fgVuscEn :1;
    UINT32 fgVdscEn :1;
    UINT32 fgHuscEn :1;
    UINT32 fgHdscEn :1;
    UINT32 :1;
    UINT32 fgScEn :1;
    UINT32 fgScAEdgeEn :1;
    UINT32 :10;
    UINT32 fgAlphaCountEn :1;
    UINT32 :12;

    /* DWORD - 004 */
    UINT32 u4SrcVSize :14;
    UINT32 :2;
    UINT32 u4SrcHSize :13;
    UINT32 :3;

    /* DWORD - 008 */
    UINT32 u4DstVSize :14;
    UINT32 :2;
    UINT32 u4DstHSize :14;
    UINT32 :2;

    /* DWORD - 00C */
    UINT32 u4VscHSize :14;
    UINT32 :18;

    /* DWORD - 010 */
    UINT32 u4HdscStep :14;
    UINT32 :2;
    UINT32 u4HdscOfst :14;
    UINT32 :2;

    /* DWORD - 014 */
    UINT32 u4HuscStep :14;
    UINT32 :2;
    UINT32 u4HuscOfst :14;
    UINT32 :2;

    /* DWORD - 018 */
    UINT32 u4VscOfstBot :14;
    UINT32 :2;
    UINT32 u4VscOfstTop :14;
    UINT32 :2;

    /* DWORD - 01C */
    UINT32 u4VscStep :14;
    UINT32 :18;
    LINT_SUPPRESS_NEXT_EXPRESSION(950)
}__attribute__ ((packed)) OSD_SC_FIELD_T;

/** OSD cursor register bit-field type.
 */
#ifdef CURSOR_SUPPORT
typedef struct _OSD_CSR_FIELD_T
{
    /* DWORD - 000 */
#ifdef OSD_CSR_H_ACTIVE_SUPPORT //20101129
    UINT32 u4VStart : 11;
    UINT32 : 1;
    UINT32 u4HStart : 11;
    UINT32 : 1;
#else
    UINT32 :24;
#endif

    UINT32 fgV2x :1;
    UINT32 fgH2x :1;
    UINT32 fgEn :1;
    UINT32 :1;
    UINT32 fgH4Bit :1;
    UINT32 fgV4Bit :1;
    UINT32 :2;LINT_SUPPRESS_NEXT_EXPRESSION(950)
}__attribute__ ((packed)) OSD_CSR_FIELD_T;
#endif

typedef struct _OSD_FRC_HEADER_FIELD_T
{
    /* DWORD - 000 */
    UINT32 fgFormat :4;//[0000]ARGB6888;[0001]AYUV6888;[0101]AYUV8888;[0100]ARGB8888
    UINT32 fgPixelOrder :4;//[0001]AGBR/AUVY;[0010]BGRA/VUYA;[0001]ABGR/AVUY;[0000]ARGB/AYUV
    UINT32 :2;
    UINT32 fgLR :1;//1:R,0:L
    UINT32 fg3DBmp :1;//1:3D ; 0:2D
    UINT32 :4;
    UINT32 fgOSDFramenum :4;
    UINT32 fgOSDPrefix :4;//[1010]valid osd;[0000]valid osd end
    UINT32 :8;

    /* DWORD - 001 */
    UINT32 fgBmpHeight :13;
    UINT32 :3;
    UINT32 fgBmpWidth :13;
    UINT32 :3;

    /* DWORD - 002 */
    UINT32 fgGlobalAlpha :8;
    UINT32 :4;
    UINT32 fgPixelAlphaEn :1;
    UINT32 fgGlobalAlphaEn :1;
    UINT32 :1;
    UINT32 fgOsdUpper :1;//[1]:osd over vedio;[0]:osd under vedio
    UINT32 :16;

    /* DWORD - 003 */
    UINT32 :32;

    /* DWORD - 004 */
    UINT32 fgDispYOFF :13;
    UINT32 :3;
    UINT32 fgDispXOFF :13;
    UINT32 :3;

    /* DWORD - 005 */
    UINT32 fgDispHeight :13;
    UINT32 :3;
    UINT32 fgDispWidth :13;
    UINT32 :3;

    LINT_SUPPRESS_NEXT_EXPRESSION(950)
}__attribute__ ((packed)) OSD_FRC_HEADER_FIELD_T;

/** OSD base register union type.
 */
/*lint -save -e960 */
typedef union _OSD_BASE_UNION_T
{
    UINT32 au4Reg[OSD_BASE_REG_NUM];
    OSD_BASE_FIELD_T rField;
} OSD_BASE_UNION_T;
/*lint -restore */

/** OSD plane register union type.
 */
/*lint -save -e960 */
typedef union _OSD_PLA_UNION_T
{
    UINT32 au4Reg[OSD_PLA_REG_NUM];
    OSD_PLA_FIELD_T rField;
} OSD_PLA_UNION_T;
/*lint -restore */
/* 3D related register */

typedef union _OSD_3D_UNION_T
{
    UINT32 au4Reg[2];
    OSD_3D_BASE_FIELD_T rField;
} OSD_3D_UNION_T;

/** OSD plane scaler union type.
 */
/*lint -save -e960 */
typedef union _OSD_SC_UNION_T
{
    UINT32 au4Reg[OSD_SC_REG_NUM];
    OSD_SC_FIELD_T rField;
} OSD_SC_UNION_T;
/*lint -restore */

/** OSD region register union type.
 */
/*lint -save -e960 */
typedef union _OSD_RGN_UNION_T
{
    UINT32 au4Reg[OSD_RGN_REG_NUM];
    OSD_RGN_FIELD_T rField;
} OSD_RGN_UNION_T;
/*lint -restore */

/** OSD cursor register union type.
 */
/*lint -save -e960 */
#ifdef CURSOR_SUPPORT
typedef union _OSD_CSR_UNION_T
{
    UINT32 au4Reg[OSD_CSR_REG_NUM];
    OSD_CSR_FIELD_T rField;
}OSD_CSR_UNION_T;
#endif
/*lint -restore */

// this struct is uesd to create palette
typedef struct _OSD_ARGB_T
{
    UINT8 u1Alpha;
    UINT8 u1Red;
    UINT8 u1Green;
    UINT8 u1Blue;
} OSD_ARGB_T;

typedef union _OSD_FRC_HEADER_UNION_T
{
    UINT32 au4Reg[6];
    OSD_FRC_HEADER_FIELD_T rField;
} OSD_FRC_HEADER_UNION_T;

typedef struct _OSD_SCALER_DATA_T
{
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4DstWidth;
    UINT32 u4DstHeight;
} OSD_SCALER_DATA_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define PARAM(IDX) (UINT32)StrToInt(szArgv[IDX+1])

#define OSD_DMA_ALIGN(X) (((X) + OSD_DMA_ALIGN_SIZE) & (~OSD_DMA_ALIGN_SIZE))
#define OSD_PALETTE_SIZE 1024

#define OSD_CHECK_NULL(X) \
    do \
    { \
        if ((X) == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
    } while (0)

#define OSD_VERIFY_PLANE(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_PLANE_MAX_NUM) \
        { \
            return -(INT32)OSD_RET_INV_PLANE; \
        } \
    } while (0)

#define OSD_VERIFY_SCALER(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_SCALER_MAX_NUM) \
        { \
            return -(INT32)OSD_RET_INV_SCALER; \
        } \
    } while (0)

#define OSD_VERIFY_REGION(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_MAX_NUM_RGN) \
        { \
            return -(INT32)OSD_RET_INV_REGION; \
        } \
    } while (0)

#define OSD_VERIFY_RGNLIST(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_MAX_NUM_RGN_LIST) \
        { \
            return -(INT32)OSD_RET_INV_LIST; \
        } \
    } while (0)

#define OSD_VERIFY_DISPLAY_MODE(X) \
    do \
    { \
        if ((UINT32)(X) >= (UINT32)OSD_DM_MAX_NUM) \
        { \
            return -(INT32)OSD_RET_INV_DISPLAY_MODE; \
        } \
    } while (0)


#define IGNORE_RET(X) \
    do \
    { \
        INT32 i4Ignore; \
        i4Ignore = (INT32)(X); \
        UNUSED(i4Ignore); \
    } while (0)

#define OSD_GET_LPF_SIGN_NUM(NUM, SIGN, DST) \
    do { \
        if ((NUM) > (SIGN)) \
        { \
            (DST) = (SIGN) - (NUM); \
        } \
        else \
        { \
            (DST) = (NUM); \
        } \
    } while (0)

#define OSD_SET_LPF_SIGN_NUM(NUM, SIGN, SRC) \
    do { \
        if (SRC < 0) \
        { \
            (NUM) = (UINT16)((SIGN) - SRC); \
        } \
        else \
        { \
            (NUM) = (UINT16)SRC; \
        } \
        if ((NUM) >= ((SIGN) << 1)) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
    } while (0)

#define OSD_BYTE_PER_PIXEL(CM, BPP) \
    do \
    { \
        switch ((CM)) \
        { \
        case OSD_CM_CBYCRY422_DIRECT16: \
        case OSD_CM_YCBYCR422_DIRECT16: \
        case OSD_CM_RGB565_DIRECT16: \
        case OSD_CM_ARGB1555_DIRECT16: \
        case OSD_CM_ARGB4444_DIRECT16: \
            (BPP) = 2; \
            break; \
        case OSD_CM_AYCBCR8888_DIRECT32: \
        case OSD_CM_ARGB8888_DIRECT32: \
            (BPP) = 4; \
            break; \
        default: \
            (BPP) = 1; \
        } \
    } while (0)

#define OSD_PIXEL_PER_BYTE(CM, PIXEL, MASK, BIT) \
            do \
            { \
                switch ((CM)) \
                { \
                case OSD_CM_YCBCR_CLUT2: \
                case OSD_CM_RGB_CLUT2: \
                    (PIXEL) = 4; \
                    (MASK) = 0x3; \
                    (BIT)  = 2; \
                    break; \
                case OSD_CM_YCBCR_CLUT4: \
                case OSD_CM_RGB_CLUT4: \
                    (PIXEL) = 2; \
                    (MASK) = 0xf; \
                    (BIT)  = 4; \
                    break; \
                default: \
                    (PIXEL) = 1; \
                    (MASK) = 0xff; \
                    (BIT)  = 8; \
                } \
            } while (0)

#define OSD_GET_PITCH_SIZE(CM, WIDTH, SIZE) \
    do \
    { \
        switch ((CM)) \
        { \
        case OSD_CM_CBYCRY422_DIRECT16: \
        case OSD_CM_YCBYCR422_DIRECT16: \
        case OSD_CM_RGB565_DIRECT16: \
        case OSD_CM_ARGB1555_DIRECT16: \
        case OSD_CM_ARGB4444_DIRECT16: \
            (SIZE) = (WIDTH) * 2; \
            break; \
        case OSD_CM_AYCBCR8888_DIRECT32: \
        case OSD_CM_ARGB8888_DIRECT32: \
            (SIZE) = (WIDTH) * 4; \
            break; \
        case OSD_CM_YCBCR_CLUT8: \
        case OSD_CM_RGB_CLUT8: \
            (SIZE) = (WIDTH); \
            break; \
        case OSD_CM_YCBCR_CLUT4: \
        case OSD_CM_RGB_CLUT4: \
            (SIZE) = (WIDTH) >> 1; \
            break; \
        case OSD_CM_YCBCR_CLUT2: \
        case OSD_CM_RGB_CLUT2: \
            (SIZE) = (WIDTH) >> 2; \
            break; \
        default: \
            ASSERT(0); \
        } \
    } while (0)


#define OSD_DMA_PLANE                       (OSD_PLANE_2)


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** osd global initialization function
 *  @param  void
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_Init(void);

EXTERN INT32 OSD_Reset(void);

EXTERN INT32 OSD_SwInit(void);

EXTERN INT32 OSD_SwReset(void);
EXTERN INT32 OSD_UpdateTiming(void);
EXTERN void _OSD_AlwaysUpdateReg(BOOL fgEnable);

EXTERN void OSD_WaitVsync(void);
EXTERN void OSD_WaitVsync2(void);
EXTERN void OSD_UpdateReg(void);
EXTERN void OSD_TimingNfy(void);
EXTERN void OSD_DoOutISR(void);
EXTERN void OSD_UnlockVsyncSema(void);

/** set whether osd1/osd2 is top plane
 *  @param  u4TopPlane OSD_PLANE_1/OSD_PLANE_2/OSD_PLANE_NONE(video plane is on top)
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_BASE_SetTopPlane(UINT32 u4TopPlane);

EXTERN INT32 OSD_BASE_SetDisplayMode(UINT32 u4DisplayMode);

EXTERN INT32 OSD_BASE_SetPlaneSwitchOrder(UINT32 u4TopPlane, UINT32 u4MidPlane,
        UINT32 u4BotPlane);

/** set osd plane position
 *  @param  u4Plane plane id
 *  @param  u4X x position
 *  @param  u4Y y position
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_BASE_SetOsdPosition(UINT32 u4Plane, UINT32 u4X, UINT32 u4Y);

/** get osd plane position
 *  @param  u4Plane plane id
 *  @param  pu4X x position pointer
 *  @param  pu4Y y position pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32
OSD_BASE_GetOsdPosition(UINT32 u4Plane, UINT32* pu4X, UINT32* pu4Y);

EXTERN INT32 OSD_BASE_SetClock(UINT32 u4Clock);
EXTERN INT32 OSD_BASE_SetYuvOutput(BOOL fgYuvOutput);
EXTERN void OSD_BASE_Dump(void);

EXTERN INT32 OSD_BASE_SetLineShift(UINT32 u4Enable, UINT32 u4Shift);
EXTERN INT32 OSD_Base_SetHStart(void);
EXTERN void OSD_Base_LogoSetHStart(BOOL value);
EXTERN void OSD_BASE_SwitchResolution(void);

EXTERN INT32 OSD_BASE_SetPlaneLineShift(UINT32 u4Plane, BOOL fgEnable,
        INT32 i4Value);

EXTERN INT32 OSD_BASE_GetHTotalPixel(UINT32 *pu4Value);

EXTERN UINT32 OSD_BASE_GetDisplayWidth(VOID);
EXTERN UINT32 OSD_BASE_GetDisplayHeight(VOID);

EXTERN INT32 OSD_SetTVEOut(BOOL fgEnable);
EXTERN INT32 OSD_SetPowerDwn(BOOL fgEnable);
EXTERN INT32 OSD_ShowLogo(void);

EXTERN INT32 OSD_PLA_Init(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_Reset(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_SwReset(UINT32 u4Plane);

EXTERN VOID OSD_PLA_SetAvailable(UINT32 u4Plane, BOOL fgVailable);
EXTERN INT32 OSD_PLA_CacheRngList(UINT32 u4Plane, UINT32 u4RgnList);
EXTERN INT32 OSD_PLA_CacheScaler(UINT32 u4Scaler, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4DstWidth, UINT32 u4DstHeight);


/** enable/disable osd plane, it will also disable scaler when disable plane
 *  @param  u4Plane plane id
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_Enable(UINT32 u4Plane, BOOL fgEnable);

/** attach a specific list to osd plane
 *  @param  u4Plane plane id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_FlipTo(UINT32 u4Plane, UINT32 u4RgnList);

EXTERN INT32 OSD_PLA_FlipToNone(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_Reflip(UINT32 u4RgnList);

/** get osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  pu1BlendLevel aplha level pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_GetBlendLevel(UINT32 u4Plane, UINT8* pu1BlendLevel);

/** set osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  u1BlendLevel aplha level(0~255)
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_SetBlendLevel(UINT32 u4Plane, UINT8 u1BlendLevel);

/** get osd plane fading value
 *  @param  u4Plane plane id
 *  @param  pu1Fading fading value pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_GetFading(UINT32 u4Plane, UINT8* pu1Fading);

/** set osd plane fading value
 *  @param  u4Plane plane id
 *  @param  u1Fading fading value
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_SetFading(UINT32 u4Plane, UINT8 u1Fading);

EXTERN INT32 OSD_PLA_SetHFilter(UINT32 u4Plane, BOOL fgEnable);
EXTERN INT32 OSD_PLA_SetVFilter(UINT32 u4Plane, BOOL fgEnable);
EXTERN INT32 OSD_PLA_SetFifo(UINT32 u4Plane, BOOL fgFastReq, UINT8 u1ExVacThr,
        UINT8 u1VacThr, UINT8 u1FullThr);
EXTERN INT32 OSD_PLA_SetGlobeEnable(UINT32 u4Plane, BOOL fgGlobeEnable);
EXTERN void OSD_PLA_Mute(void);
EXTERN void OSD_PLA_Unmute(void);
EXTERN INT32 OSD_PLA_SetFastMute(UINT32 u4Plane, UINT32 u4Mute);
EXTERN INT32 OSD_PLA_GetFastMute(UINT32 u4Plane, UINT32* pu4Mute);

EXTERN void OSD_PLA_SetWorkaround(UINT32 u4Plane, BOOL fgEnable);

/** dump osd debug info
 *  @param  u4Plane osd plane
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_PLA_Dump(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_DisableLoaderLogo(UINT32 u4Plane);
EXTERN UINT32 OSD_PLANE_TO_SCALER(UINT32 u4Plane);

/** get the first region id of osd plane
 *  @param  u4Plane osd plane
 *  @retval region id, -1 is no region
 */
EXTERN INT32 OSD_PLA_GetFirstRegion(UINT32 u4Plane);
EXTERN INT32 _OSD_PLA_Get_Hw_RealHeaderAddr(UINT32 u4Plane,
        volatile UINT32 *pu4Value);
EXTERN UINT32 _OSD_PLA_Get_Hw_RealBmpAddr(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN UINT32 _OSD_PLA_Get_Hw_RealClipH(UINT32 u4Plane, UINT32 *pu4Value);

EXTERN UINT32 OSD_PLA_GetRgnPlane(INT32 i4FirstRegion);

EXTERN INT32 OSD_PLA_GetFirstRegionList(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_GetEnable(UINT32 u4Plane, BOOL* pfgEnable);

/** get the first region address of osd plane
 *  @param  u4Plane osd plane
 *  @retval region address, 0 is no region
 */
EXTERN UINT32 OSD_PLA_GetFirstRegionAddress(UINT32 u4Plane);

EXTERN void OSD_PLA_SetCompressMode(UINT32 u4Plane, UINT32 u4DecEnable,
        UINT32 u4AlphaMode);
EXTERN void OSD_PLA_GetCompressMode(UINT32 u4Plane, UINT32 *pu4DecEnable,
        UINT32 *pu4AlphaMode);
EXTERN INT32 OSD_PLA_GetAllRegion(UINT32 u4Plane, UINT32* u4Rgn1,
        UINT32* u4Rgn2);
EXTERN INT32 OSD_PLA_GetSrcContentRegion(UINT32 u4Plane, UINT32* u4Rgn1,
        UINT32* u4Rgn2);
EXTERN INT32 OSD_PLA_GetSrcContentSize(UINT32 u4Plane, UINT32 *pu4Width,
        UINT32 *pu4Height);

EXTERN UINT32 OSD_PLA_GetHStart(UINT32 u4Plane);
EXTERN UINT32 OSD_PLA_GetVStart(UINT32 u4Plane);



#if 0
EXTERN INT32 OSD_PLA_Set3DAddress(UINT32 u4Plane, UINT32 u4L_Address, UINT32 u4R_Address);
EXTERN INT32 OSD_Vsync_3DUpdateLR(UINT32 u4L_R);
#endif
EXTERN INT32 OSD_Set3DMode(UINT8 u1Mode);

EXTERN INT32 OSD_RGN_LIST_Set(UINT32 u4RgnList, INT32 i4Cmd, UINT32 u4Value);
EXTERN INT32 OSD_RGN_LIST_Get(UINT32 u4RgnList, INT32 i4Cmd, UINT32 * pu4Value);

/** create an osd list
 *  @param pu4RgnList list id pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_Create(UINT32 * pu4RgnList);

/** delete an osd list
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_Delete(UINT32 u4RgnList);

/** get first region id of list
 *  @param  u4RgnList list id
 *  @param  pi4HeadRegion region id pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_LIST_GetHead(UINT32 u4RgnList, INT32 * phHeadRegion);

EXTERN INT32 OSD_RGN_LIST_GetNext(UINT32 u4RgnList, INT32 hRgnCurr,
        INT32 *phRgnNext);
EXTERN INT32 OSD_RGN_LIST_DetachAll(UINT32 u4RgnList);
EXTERN INT32 OSD_RGN_LIST_GetCount(UINT32 u4RgnList, INT32 * pi4Count);

EXTERN INT32 OSD_RGN_LIST_Init(void);
EXTERN INT32 OSD_RGN_Init(void);
EXTERN INT32 OSD_RGN_LIST_SCALER_Init(void);
EXTERN OSD_SCALER_DATA_T* OSD_RGN_LIST_SCALER_Get(UINT32 u4_idx);
EXTERN INT32 OSD_RGN_LIST_SCALER_Set(UINT32 u4_idx,OSD_SCALER_DATA_T* pt_this);
EXTERN INT32 OSD_RGN_LIST_LineScaler(UINT32 u4RgnList, UINT32 u4BmpWidth, UINT32 u4BmpHeight,
        			void * pvBitmap, UINT32 eColorMode, UINT32 u4BmpPitch,
        			UINT32 u4DispX,UINT32 u4DispY, UINT32 u4DispW, UINT32 u4DispH);
EXTERN INT32 OSD_RGN_SetColorOrder(UINT32 u4Region,UINT32 u4AOrder,UINT32 u4ROrder,
							UINT32 u4GOrder,UINT32 u4BOrder);
/** get osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  pu4Value attribute value pointer
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_Get(UINT32 u4Region, INT32 i4Cmd, UINT32 * pu4Value);

EXTERN INT32 OSD_RGN_Set(UINT32 u4Region, INT32 i4Cmd, UINT32 u4Value);

/** create an osd region
 *  @param  pu4Region region id pointer
 *  @param  u4BmpWidth source bitmap width
 *  @param  u4BmpHeight source bitmap height
 *  @param  pvBitmap source bitmap address, MUST align with 16bytes
 *  @param  eColorMode, region color mode, refer to OSD_COLOR_MODE_T
 *  @param  u4BmpPitch, bytes per line, MUST align with 16bytes
 *  @param  u4DispX, region x position
 *  @param  u4DispY, region y position
 *  @param  u4DispW, output width
 *  @param  u4DispH, output height

 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_ARG invalid argument
 */
EXTERN INT32 OSD_RGN_Create(UINT32 * pu4Region, UINT32 u4BmpWidth,
        UINT32 u4BmpHeight, void * pvBitmap, UINT32 eColorMode,
        UINT32 u4BmpPitch, UINT32 u4DispX, UINT32 u4DispY, UINT32 u4DispW,
        UINT32 u4DispH);

/** delete an osd region
 *  @param  u4Region region id
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_RGN_Delete(UINT32 u4Region);

EXTERN INT32 OSD_RGN_AttachTail(UINT32 u4Region, UINT32 u4RgnList);

/** attach region to list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_LIST invalid list
 */
EXTERN INT32 OSD_RGN_Insert(UINT32 u4Region, UINT32 u4RgnList);

/** dettach region from list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_REGION invalid region
 */
EXTERN INT32 OSD_RGN_Detach(UINT32 u4Region, UINT32 u4RgnList);

EXTERN INT32 OSD_RGN_SetDisplayWidth(UINT32 u4Region, UINT32 u4Width);
EXTERN INT32 OSD_RGN_SetDisplayHeight(UINT32 u4Region, UINT32 u4Height);
EXTERN INT32 OSD_RGN_SetBigEndian(UINT32 u4Region, BOOL fgBE);
EXTERN INT32 OSD_RGN_Dump(UINT32 u4Region);

//add for setting/getting the region list used by linux frame buffer
EXTERN INT32 OSD_Set_FrameBuffer_RGN_LIST(UINT32 u4RgnList);
EXTERN INT32 OSD_Get_FrameBuffer_RGN_LIST(UINT32 * pu4Value);

//add for getting the region resolution used by linux frame buffer
EXTERN INT32
OSD_Get_FrameBuffer_Resolution(UINT32* pu4Width, UINT32* pu4Height);

EXTERN INT32 OSD_RGN_GetOriginalY(UINT32 u4Region, UINT32 * pu4Value);
EXTERN INT32 OSD_RGN_GetOriginalX(UINT32 u4Region, UINT32 * pu4Value);
EXTERN INT32 OSD_RGN_MirrorFlipUpdate(UINT32 u4Region, BOOL updateMirror, BOOL updateFlip);

/** cinfugure osd scaler
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @param  u4SrcWidth osd input width
 *  @param  u4SrcHeight osd input height
 *  @param  u4DstWidth osd output width
 *  @param  u4DstHeight osd output height
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_Scale(UINT32 u4Scaler, UINT32 u4Enable, UINT32 u4SrcWidth,
        UINT32 u4SrcHeight, UINT32 u4DstWidth, UINT32 u4DstHeight);
EXTERN UINT32 _OSD_SC_GetHwReg(UINT32 u4Scaler);

#ifdef CC_DEBUG
EXTERN INT32 OSD_SC_HDown(UINT32 u4Scaler, UINT32 u4SrcWidth, UINT32 u4Step);
EXTERN INT32 OSD_SC_HUp(UINT32 u4Scaler, UINT32 u4SrcWidth, UINT32 u4Step);
EXTERN INT32 OSD_SC_VDown(UINT32 u4Scaler, UINT32 u4SrcHeight, UINT32 u4Step);
EXTERN INT32 OSD_SC_VUp(UINT32 u4Scaler, UINT32 u4SrcHeight, UINT32 u4Step);
#endif

/** get osd scaler infomation
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  pu4Enable enable pointer
 *  @param  pu4SrcW osd input width pointer
 *  @param  pu4SrcH osd input height pointer
 *  @param  pu4DstW osd output width pointer
 *  @param  pu4DstH osd output height pointer
 *  @param  pu4Is16Bpp don't care
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_GetScalerInfo(UINT32 u4Scaler, UINT32* pu4Enable,
        UINT32* pu4SrcW, UINT32* pu4SrcH, UINT32* pu4DstW, UINT32* pu4DstH,
        UINT32* pu4Is16Bpp);

EXTERN INT32 OSD_SC_SetLpfInfo(UINT32 u4Scaler, UINT32 u4Enable, INT16 i2C1,
        INT16 i2C2, INT16 i2C3, INT16 i2C4, INT16 i2C5);

/** enable low pass filter
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
EXTERN INT32 OSD_SC_SetLpf(UINT32 u4Scaler, UINT32 u4Enable);

EXTERN INT32 OSD_SC_GetLpfInfo(UINT32 u4Scaler, UINT32* pu4Enable,
        INT16* pi2C1, INT16* pi2C2, INT16* pi2C3, INT16* pi2C4, INT16* pi2C5);
EXTERN void OSD_SC_UpdateDstSize(UINT32 u4Scaler);
EXTERN INT32
OSD_SC_CheckCapability(UINT32 u4SrcW, UINT32 u4SrcH, UINT32 u4DstH);
EXTERN INT32 OSD_SC_SetSramConfiguration(UINT32 u4Mode);
EXTERN INT32 OSD_SC_GetSramConfiguration(void);
EXTERN INT32 OSD_SC_Dump(UINT32 u4Scaler);
//#ifdef CC_MT5363
//EXTERN INT32 OSD_SC_Swap(BOOL fgSwap);
//#endif
EXTERN INT32 OSD_SC_SetRegion(UINT32 u4Scaler, INT32 i4Region);

EXTERN void OSD_DrawVLine(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT8 u1Alpha);
EXTERN void OSD_DrawHLine(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode);
EXTERN void OSD_DrawBorder(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode);
EXTERN void OSD_DrawColorbar(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT8 u1Alpha);
EXTERN void OSD_DrawSlt(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode);
EXTERN void OSD_DrawRamp(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT8 u1Alpha, BOOL fgPremultipled);
EXTERN void OSD_DrawMute(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4MuteColor);
EXTERN void OSD_CreatePaleTable(OSD_ARGB_T *prPaleTable, UINT32 u4ColorMode);
EXTERN void OSD_DrawColor(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4Color);
EXTERN void OSD_DrawColorbar2(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern0(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4BgColor);
EXTERN void OSD_DrawPattern1(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4Pattern);
EXTERN void OSD_DrawPattern2(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4Pattern);
EXTERN void OSD_DrawPattern3(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4Pattern, UINT32 u4BgColor);
EXTERN void OSD_DrawPattern4(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4Pattern);
EXTERN void OSD_DrawPattern5(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern6(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern7(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern8(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 uPattern);
EXTERN void OSD_DrawPattern9(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern10(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode);

EXTERN void OSD_DrawPattern11(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern12(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern13(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode);
EXTERN void
OSD_DrawPattern14(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4Pattern, UINT32 u4BgColor);
EXTERN void OSD_DrawPattern15(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode);
EXTERN void OSD_DrawPattern16(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode);
void OSD_DrawPixLinePattern0(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern1(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern2(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern3(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern4(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern5(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern6(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern7(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern8(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);
EXTERN void OSD_DrawPixLinePattern9(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4R, UINT32 u4G, UINT32 u4B);

EXTERN void OSD_LockFlipTo(void);
EXTERN void OSD_UnLockFlipTo(void);

EXTERN void OSD_LockLogoDisplaying(void);
EXTERN void OSD_UnLockLogoDisplaying(void);
EXTERN VOID OSD_BASE_GetPanelSize(UINT32 *u4Width, UINT32 *u4Height);
EXTERN void OSD_DrawPatternL92(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode, UINT8 ui1_grid_width,
        UINT8 ui1_grid_heigh, UINT32 u4BgColor);

EXTERN void OSD_DrawPatternGomi(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode, UINT32 u4BgColor, UINT8 ui1_mode);
EXTERN void OSD_DrawPatternCrosstalk(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode, UINT32 u4BgColor);
EXTERN void OSD_DrawPatternFlicker(UINT8* pu1Canvas, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4ColorMode, UINT32 u4BgColor);
EXTERN void OSD_DrawPatternFlicker1(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                            UINT32 u4ColorMode, UINT32 u4BgColor);
EXTERN void OSD_DrawPatternFlicker2(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                            UINT32 u4ColorMode, UINT32 u4BgColor);
EXTERN void OSD_DrawPatternFlicker3(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                            UINT32 u4ColorMode, UINT32 u4BgColor);
EXTERN void OSD_DrawPatternStripe1(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                            UINT32 u4ColorMode);
EXTERN void OSD_DrawPatternStripe2(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                            UINT32 u4ColorMode);
EXTERN void OSD_DrawPatternStripe3(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                            UINT32 u4ColorMode);

/** OSD operation mode.
 */
typedef enum
{
    OSD_OP_NORMAL, OSD_OP_DMA, OSD_OP_GFX
} OSD_OP_MODE_T;

EXTERN void OSD_DMA_Init(void);

EXTERN void OSD_DMA_SetMode(OSD_OP_MODE_T eMode);

EXTERN void OSD_DMA_SetTarget(UINT32 *au4TargetAddr);

EXTERN INT32 OSD_DMA_TransferRegion(UINT32 u4Plane);

EXTERN INT32 OSD_DMA_WriteRegister(UINT32 u4Reg, UINT32 u4Value,
        BOOL fg8bitData, UINT32 u4Parity);

EXTERN INT32 OSD_DMA_Copy(UINT32 u4DstAddress, UINT32 u4SrcAddress,
        UINT32 u4Size);

EXTERN void OSD_DMA_OnOutputVsync(void);

EXTERN void OSD_DMA_SetUpdatePeriod(UINT32 u4DmaUpdatePeriod);

EXTERN void OSD_DMA_Dump(void);

EXTERN OSD_OP_MODE_T OSD_DMA_GetMode(void);

EXTERN void OSD_DMA_EnableI2C(BOOL fgEnable);

EXTERN void OSD_DMA_AfterPaletteUpdate(UINT32 u4PaletteId,
        UINT32 u4PaletteAddress);

EXTERN INT32 OSD_DMA_Check_Plane(UINT32 u4_plane);


//Add for Set DRAM parameters
EXTERN void OSD_PLA_SetContReqLmt(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_SetFifoSize(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_SetVacTh(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_SetPauseCnt(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_SetContReqLmt0(UINT32 u4Plane, UINT32 u4Value);

EXTERN INT32 OSD_DMA_Set8281(UINT32 u4addr, UINT32 u4value);

EXTERN BOOL OSD_GetUnderflowStatus(void);

EXTERN void OSD_ClearUnderflowStatus(void);

//For OSD scroll
EXTERN INT32 OSD_Vsync_UpdateScroll(void);
EXTERN INT32 OSD_RGN_SetScrollInfo(UINT32 u4Rgn, UINT32 u4BaseAddress,
        UINT32 u4Pitch, UINT32 u4Step);
EXTERN INT32 OSD_SetScrollMode(UINT8 u1Mode);
EXTERN INT32 OSD_BASE_Get3DOsd2LShiftEn(UINT32 *pu4Value);
EXTERN INT32 OSD_BASE_Get3DOsdShiftEn(UINT32 OSD_Plane,UINT32 *pu4Value);
EXTERN void OSD_RGN_Set_Bmp_Addr_Wait(UINT32 u4Plane, UINT32 u4Value);
EXTERN void OSD_PLA_WaitStable(UINT32 u4_plane);


typedef enum
{
    OSD_3D_NONE,
    OSD_3D_SBSHALF_PR,
    OSD_3D_TPANDBT_PR,
    OSD_3D_SBSHALF_SG,
    OSD_3D_TPANDBT_SG,
    OSD_3D_MAX
}OSD_3D_MODE_T;


#ifdef SYS_3D_SUPPORT

#ifndef CC_MTK_LOADER
extern INT32 OSD_PLA_Set3DFlip(UINT32 u4Plane, OSD_3D_MODE_T e3DMode, UINT32 u4Shift,
        UINT32 u4BmpWidth, UINT32 u4BmpHeight,UINT32 u4BmpPitch,
        UINT32 u4BmpAddr, UINT32 eColorMode);

EXTERN INT32 OSD_PLA_Set3DBlit(UINT32 u4Plane, OSD_3D_MODE_T e3DMode, UINT32 u4Shift,
        UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
        UINT32 u4DstAddr, UINT32 eColorMode, UINT32 u4SrcAddr);

EXTERN INT32 OSD_PLA_Redraw3D(OSD_3D_MODE_T e3DMode);
#endif

EXTERN void OSD_SC_ForceSize(BOOL fgForceSize);

#endif

EXTERN UINT32 OSD_Base_GetVsyncTime(void);

#ifndef CC_MTK_LOADER

#ifdef OSD_DSC_PDP

EXTERN INT32 OSD_PLA_DsBlit(UINT32 u4SrcAddr, UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4SrcPitch,
        UINT32 u4DstAddr, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstWidth, UINT32 u4DstHeight,UINT32 u4DstpPitch,UINT32 eColorMode );
#endif
#endif

EXTERN INT32 OSD_BASE_SetPlaneLineShift(UINT32 u4Plane, BOOL fgEnable,
        INT32 i4Value);
EXTERN INT32 OSD_PLA_Set3DMode(UINT32 u4Plane, UINT8 u1Mode);
EXTERN INT32 OSD_PLA_Set3DRegion(UINT32 u4Plane, UINT32 u4LeftRgn,
        UINT32 u4RightRgn);
EXTERN INT32 OSD_BASE_EnableAutoLR(UINT32 u4Enable);
EXTERN INT32 OSD_BASE_GetLRStatus(UINT32* pu4Status);
EXTERN BOOL OSD_BASE_Get_3DOSD2PLsEn(void);
EXTERN void OSD_BASE_Set_3DOSD2PLsEn(BOOL u4Enable);
EXTERN INT32 OSD_3D_SetOsd2LShiftEn(UINT32 u4Value);
EXTERN INT32 OSD_3D_SetOsd1LShiftEn(UINT32 u4Value);
EXTERN INT32 OSD_3D_SetOsd3LShiftEn(UINT32 u4Value);
EXTERN BOOL OSD_BASE_IsDisplaying(void);
EXTERN void vSetOsdFlipMirror(BOOL fgMirrorEn, BOOL fgFlipEn);
EXTERN INT32 OSD_3D_SetPlaneSwitch(UINT32 u4Enable);
#ifdef CC_ANDROID_3D_UI_SUPPORT


EXTERN UINT32 OSD_PLA_SetUIMode(UINT32 u4Mode, UINT32 u4WIdth, UINT32 u4Height);
EXTERN INT32 OSD_PLA_Set3DSwitchRegion(UINT32 u4LeftRgn, UINT32 u4RightRgn);
EXTERN INT32 OSD_PLA_Set3DSwitchRgnAddr(UINT32 u4LeftRgn, UINT32 u4RightRgn,UINT32 u4LeftAddr, UINT32 u4RightAddr);
EXTERN INT32 OSD_PLA_DsBlit(UINT32 u4SrcAddr, UINT32 u4SrcX, UINT32 u4SrcY,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4SrcPitch,
        UINT32 u4DstAddr, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4DstWidth, UINT32 u4DstHeight,UINT32 u4DstpPitch,UINT32 eColorMode);

#endif

EXTERN UINT32 OSD_BASE_GetTranBuf_Current(void);
EXTERN UINT32 OSD_BASE_GetTranBuf_Back(void);
EXTERN INT32 OSD_BASE_SetFRCHeader(UINT32 u4SetCMD, UINT32 u4Value);
EXTERN INT32 OSD_BASE_SetTransferCtl(UINT32 u4Enable);
EXTERN INT32 OSD_BASE_GetTransferCtl(UINT32 * u4Enable);
EXTERN INT32 OSD_BASE_TransferTo(UINT32 u4SrcAddr, UINT32 u4DstAddr);
EXTERN INT32 OSD_BASE_COPY_FRCHeader(UINT32 u4DstAddr);
EXTERN INT32 OSD_BASE_InitFRCHeader(void);
EXTERN INT32 OSD_BASE_GetFRCHeader(UINT32 u4SetCMD, UINT32 * u4Value);
EXTERN INT32 OSD_BASE_TransferTo_SYNC_Header(UINT32 u4Region);
EXTERN UINT32 OSD_BASE_GetFifoMonitor(void);
EXTERN UINT32 _OSD_BASE_GetHcntStrobe(void);
EXTERN UINT32 _OSD_BASE_GetVcntStrobe(void);
EXTERN UINT32 _OSD_BASE_GetHcntStatus(void);
EXTERN UINT32 _OSD_BASE_GetVcntStatus(void);
EXTERN UINT32 _OSD_BASE_Get3DReg(void);
EXTERN UINT32 _OSD_BASE_Get_SHUTTER_LR_FLAG(void);
EXTERN UINT32 _OSD_PLA_Get_HWReg(UINT32 u4Plane);
EXTERN INT32 _OSD_RGN_GetRegionId(UINT32 u4RegionAdress , UINT32 *pu4RegionId);

EXTERN UINT32 OSD_GetCpblty(UINT32 u4_type,UINT32 u4_idx);
EXTERN INT32 OSD_SetCpblty(UINT32 u4_type,UINT32 u4_idx,UINT32 u4_val);
EXTERN void  OSD_InitCpblty(void);
EXTERN void OSD_PLA_Info_Init(void);
EXTERN BOOL OSD_Base_Is_Osd_Do_Flip_Mirror(UINT32 u4_plane);
EXTERN BOOL OSD_Base_Is_Gfx_Do_Flip_Mirror(UINT32 u4_plane);
EXTERN void _UpdatePlaneEnableState(UINT32 u4Plane);
EXTERN INT32 OSD_SC_RGNLIST_Set(UINT32 u4Plane,UINT32 u4_rgn_list,UINT32 u4_enable);
EXTERN INT32 OSD_PLA_Flip_RgnList(UINT32 u4Plane, UINT32 u4List);

EXTERN void OSD_UpdateReg(void);
EXTERN void OSD_TimingNfy(void);
EXTERN void OSD_DoOutISR(void);
EXTERN void OSD_PLA_WaitStable(UINT32 u4_plane);
EXTERN INT32 OSD_PLA_SetPremulEn(UINT32 u4Plane, UINT32 u4Value);
EXTERN INT32 OSD_PLA_GetPremulEn(UINT32 u4Plane, UINT32 *pu4Value);
EXTERN INT32 OSD_BASE_SetScrnHStart(UINT32 u4_plane,UINT32 u4_value);
EXTERN INT32 OSD_BASE_SetScrnVStartTop(UINT32 u4_value);
EXTERN INT32 OSD_BASE_UpdateHwReg(void);

#ifdef LINUX_TURNKEY_SOLUTION
EXTERN INT32 fb_osd_reupdate_bmp_addr(UINT32 u4_plane);
#endif
EXTERN INT32 OSD_SetFb3DCfg(UINT32 u4_plane, UINT32 u4_3d_mode, UINT32 u4_sync);

EXTERN void OSD_Reset_UnderFlow(void);
EXTERN UINT32 OSD_Base_Check_Abnormal_Status(void);
EXTERN void OSD_Reset_FSM_Step1(void);
EXTERN void OSD_Reset_FSM_Step2(void);
EXTERN void OSD_Base_Check_Underflow_Action(void);
EXTERN void OSD_Base_Reg_Isr(void);
EXTERN void OSD_Base_UnderFlow_Isr_Action(void);
EXTERN void OSD_Base_Check_Underflow_Isr_Action(void);
EXTERN void OSD_Set_Dbg_Info(UINT32 u4_idx,UINT32 u4_val);
EXTERN UINT32 OSD_Get_Dbg_Info(UINT32 u4_idx);
EXTERN UINT32 OSD_PLA_Get_Dec(UINT32 u4Plane);
EXTERN INT32 OSD_PLA_Get_En(UINT32 u4Plane);
EXTERN void OSD_PLA_Chg_Clock_Action(UINT32 u4_enable);
EXTERN UINT32 OSD_SC_GetPlane(UINT32 u4_scaler);
EXTERN INT32 OSD_PLA_LineScalerShow(UINT32 u4Plane,UINT32 u4BufferAddr,UINT32 u4SrcWidth,
		UINT32 u4SrcHeight,UINT32 u4SrcPitch,UINT32 u4DstX,UINT32 u4DstY,
		UINT32 u4DstWidth,UINT32 u4DstHeight,UINT32 u4ColorMode);
#ifdef OSD_4K2K_SUPPORT
EXTERN UINT32 OSD_PLA_SetLinuxUIMode(UINT32 u4Mode, UINT32 u4WIdth, UINT32 u4Height, UINT32 u4Value1, UINT32 u4Value2);
EXTERN INT32 OSD_PLA_Get4K2KOriScalerInformation(UINT32 u4OSDPlane,OSD_4K2KOriInformation *pOSD4K2KInfo);
EXTERN INT32 OSD_PLA_Get4K2KOriRegionInformation(UINT32 u4OSDPlane,OSD_4K2KOriInformation *pOSD4K2KInfo);
EXTERN INT32 OSD_PLA_GetJpegFramesAddr(UINT32 u4OSDPlane,OSD_4K2KJpecModeCurrentSet *pJpecModeCurrentSet);
EXTERN INT32 OSD_PLA_GetMVCFrameAddr(UINT32 u4OSDPlane,OSD_4K2KMvcModeCurrentSet *pMvcModeSet);
EXTERN INT32 OSD_PLA_SetJpecRegionAttri(OSD_4K2KOriInformation *pJpecRegionOriSet);
EXTERN INT32 OSD_PLA_SetMVCRegionAttri(OSD_4K2KOriInformation *pMvcRegionOriSet);
EXTERN BOOL OSD_PLA_IsSupport4K2K(void);
EXTERN INT32 OSD_PLA_ReductOriRGNInfo(UINT32 u4Plane,OSD_4K2KOriInformation *pOSD4K2KInfo);
EXTERN void OSD_BaseDisplayFor4K2K(HANDLE_T pt_tm_handle, VOID* pv_tag);
EXTERN UINT32 OSD_GetVsyncDetectionTimer(void);
EXTERN INT32 OSD_PLA_Set4K2kSwitchRgnAddr(UINT32 u4LeftRgn, UINT32 u4RightRgn,UINT32 u4LeftAddr, UINT32 u4RightAddr);
#endif

EXTERN BOOL OSD_BASE_SonyIsDisplaying(void);

EXTERN UINT32 OSD_PLA_GetScaler(UINT32 u4Plane);
EXTERN INT32 OSD_BASE_AdjustPlanePosition(UINT32 u4TopPlane, UINT32 u4MidPlane,
        UINT32 u4BotPlane,INT32 i4Position,UINT32 delay);
EXTERN void OSD_DrawSonyPattern(UINT8* pu1Canvas,UINT32 u4Width,UINT32 u4Height,
	                    UINT32 u4ColorMode,UINT32 u4BgColor);
EXTERN UINT32 OSD_PLA_Get_Hw_RealBmpAddr(UINT32 u4Plane, UINT32 *pu4Value);
#ifdef CC_FASTBOOT_DRIVER_LOGO
EXTERN INT32 OSD_SetFastLogoPlane(void);
#endif
EXTERN INT32 OSD_Base_AdjustOSDPosition(INT32 i4Adjust);
EXTERN INT32 OSD_Base_AdjustOSDVPosition(INT32 i4Adjust);

#ifdef CC_CONFIG_OSD_SBS
EXTERN INT32 OSD_PLA_Set3DDFB(UINT32 u4Plane, UINT32 e3DMode, UINT32 u4Shift,
        UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
        UINT32 u4DstAddr, UINT32 eColorMode, UINT32 u4SrcAddr);
#endif

#ifndef CC_MTK_LOADER
#ifdef LINUX_TURNKEY_SOLUTION
EXTERN UINT32 fb_osd_get_buffer_mode(UINT32 u4_plane);
EXTERN UINT32 fb_osd_get_dfb_addr(void);
EXTERN UINT32 fb_osd_get_dfb_size(void);
EXTERN UINT32 fb_osd_get_fb_addr(void);
EXTERN UINT32 fb_osd_get_fb_size(void);
#endif
#endif

#endif //OSD_DRVIF_H

