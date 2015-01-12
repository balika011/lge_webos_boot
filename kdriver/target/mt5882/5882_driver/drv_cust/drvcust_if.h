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
 * $Date: 2015/01/12 $
 * $RCSfile: drvcust_if.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file drvcust_if.h
 *  Interface of driver customization parameter.
 */


#ifndef DRVCUST_IF_H
#define DRVCUST_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_lint.h"
#include "x_printf.h"
#include "x_util.h"
#include "x_ckgen.h"
#include "../fbm8290/fbm_pool_config.h"
//-----------------------------------------------------------------------------
// Constant config
//-----------------------------------------------------------------------------
#define FBM_MEM_CFG_MT5387_VERF             (0x5600000)
/* 
formula:
(Ref+dec+lock)*W*H*1.5 + (Ref+dec)*W*H/4
ex.
(1920*1088)*1.5*9+(1920*1088)*7/4 = 31856640.0 = 0x1E61800 ~= 0x1F00000
C Total size: 
  (1920*1088)*0.5*9 = 0x8F7000
  FBM_MPEG_C_SIZE : ~=0x90000
*/

#define FBM_MPEG_Y_SIZE  (0x2680000) //0x1F00000 + 0x280000(cabac) + 0x600000(2FB,seamless) = 0x2780000
#define FBM_MPEG_C_SIZE (0)  
#if defined(CC_FBM_SUPPORT_HDMI_4K2K30)
#define FBM_SCPOS_MAIN_SIZE (0x5500000)
#else
#define FBM_SCPOS_MAIN_SIZE (0x2100000)
#endif
#if defined(CC_FBM_SUPPORT_AMAZON_APP)
#define FBM_FEEDER1_SIZE  (0xd00000)
#else
#define FBM_FEEDER1_SIZE  (0x300000)
#endif
#define FBM_DMX1_SIZE     (0x500000)

#define FBM_SIZE_FORMULAR(x)    (x  + FBM_DMX1_SIZE + FBM_SCPOS_MAIN_SIZE + FBM_MPEG_Y_SIZE  + FBM_PQ_TOOL_POOL_SIZE + FBM_HW_DEMOD_SIZE + FBM_OD_SIZE + FBM_JPEVDP_POOL_SIZE + FBM_DMX_PID_BUF_SIZE + FBM_VENC_SIZE  + FBM_TWO_FBP_SIZE + FBM_FD_SIZE + FBM_DIVX_SIZE +VOMX_EXTRA_FRAME_BUFFER_SZ + FBM_PVR_SIZE + FBM_SWDMX_SIZE + FBM_VSS_SIZE + FBM_DRM_BUF_SIZE + FBM_MUXER_SIZE + FBM_IMGRZ_3D_KR_BUF + FBM_SLT_EXTRA_SIZE + FBM_PNG_POOL_SIZE + FBM_TDCAV_POOL_SIZE  + FBM_PQ_TOOL_SIZE + FBM_DMX_ADD_SIZE + FBM_EXTRA_FOR_4K2K_FEEDER_DMX + FBM_DSP_BIN_POOL_SIZE + FBM_TVE_POOL_FBM_SIZE  +  FBM_JPEG_SIZE + FBM_DSP_POOL_SIZE - FBM_REDUCE_FBM_DMX_SIZE)

#ifdef __MODEL_slt__
#define FBM_SLT_EXTRA_SIZE      0xD0000
#else
#define FBM_SLT_EXTRA_SIZE      0x0
#endif

#ifdef CC_FBM_HW_DEMOD
#define FBM_HW_DEMOD_SIZE 0x800000 
#else
#define FBM_HW_DEMOD_SIZE 0
#endif 

#ifdef CC_CDC_SUPPORT
#define FBM_PQ_TOOL_SIZE 0x1800000 //24M = 8M*3
#else
#define FBM_PQ_TOOL_SIZE 0
#endif   

#if defined(CC_SUPPORT_OD)
#define FBM_OD_SIZE             0x200000
#else
#define FBM_OD_SIZE             0x0
#endif

#if defined(CC_SUPPORT_VSS)
#define FBM_VSS_SIZE            0xFE0000
#else
#define FBM_VSS_SIZE            0x0
#endif

#if defined(CC_SUPPORT_MONITOR_OUT_TVE)
#if defined(CC_FLIP_MIRROR_SUPPORT)
#define FBM_TVE_POOL_FBM_SIZE  0x350000
#else
#define FBM_TVE_POOL_FBM_SIZE  0x240000
#endif
#else
#define FBM_TVE_POOL_FBM_SIZE  0
#endif

#if defined(TIME_SHIFT_SUPPORT)
#ifndef CC_FBM_SUPPORT_PVR
#define CC_FBM_SUPPORT_PVR
#endif
#endif

#define CC_FBM_SUPPORT_PNG

#ifndef FBM_PNG_POOL_SIZE
#if defined(CC_FBM_SUPPORT_PNG)
#define FBM_PNG_POOL_SIZE       0x800000
#else
#define FBM_PNG_POOL_SIZE       0x0
#endif
#endif

#ifdef ADSP_BIN_SUPPORT
#define FBM_DSP_BIN_POOL_SIZE                      (0x200000)
#else
#define FBM_DSP_BIN_POOL_SIZE                      (0)
#endif

#if defined(CC_FBM_SUPPORT_PVR)
#define FBM_PVR_SIZE            0x380000
#else
#define FBM_PVR_SIZE            0x0
#endif

#if defined(CC_FBM_SUPPORT_SWDMX)
#define FBM_SWDMX_SIZE          0x240000
#else
#define FBM_SWDMX_SIZE          0x0
#endif

#if defined(CC_FBM_SUPPORT_DMXPID)
#define FBM_DMX_PID_BUF_SIZE    0x200000
#else
#define FBM_DMX_PID_BUF_SIZE    0x0
#endif

#ifdef ADSP_BIN_SUPPORT
#define FBM_DSP_BIN_POOL_SIZE                      (0x200000)
#else
#define FBM_DSP_BIN_POOL_SIZE                      (0)
#endif

#define FBM_IC_TAG              0x4000

#if defined(CC_SUPPORT_VENC)
#define FBM_VENC_SIZE           (0x1F00000)
#else
#define FBM_VENC_SIZE           0x0
#endif

#if defined(CC_FBM_ADD_VFIFO_SIZE)|| defined(CC_SSIF_SUPPORT)
#define FBM_DMX_ADD_SIZE       0x500000
#else
#define FBM_DMX_ADD_SIZE       0
#endif

#define FBM_DMX_MPEG_SIZE                           (0x300000)
#define FBM_DMX_H264_SIZE                           (0x500000)

#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
    #define FBM_DMX_4K_2K_SIZE                      (0x2300000)
#else
    #define FBM_DMX_4K_2K_SIZE                      FBM_DMX_H264_SIZE
#endif


#if defined(CC_TRUSTZONE_SUPPORT)&& defined(CC_SVP_SUPPORT)
         
          #define FBM_DMX_SIZE             (FBM_DMX_H264_SIZE)
          #if defined(CC_FBM_TWO_FBP)
                #define FBM_DMX2_SIZE      (FBM_DMX_H264_SIZE)
          #else
                #define FBM_DMX2_SIZE       0
          #endif

    #if defined(ANDROID)
                #define FBM_DMX_SIZE_TZ    FBM_DMX_H264_SIZE
                #define FBM_DMX2_SIZE_TZ   FBM_DMX_4K_2K_SIZE
    #else
                #define FBM_DMX_SIZE_TZ    FBM_DMX_4K_2K_SIZE
                #define FBM_DMX2_SIZE_TZ   FBM_DMX_H264_SIZE
    #endif
#else
                #define FBM_DMX_SIZE_TZ    0
                #define FBM_DMX2_SIZE_TZ   0
    
    #if defined(ANDROID)
          
          #if defined(CC_FBM_TWO_FBP)
                #define FBM_DMX_SIZE       (FBM_DMX_H264_SIZE)
                #define FBM_DMX2_SIZE      (FBM_DMX_4K_2K_SIZE)
          #else
                #define FBM_DMX_SIZE       (FBM_DMX_4K_2K_SIZE)
                #define FBM_DMX2_SIZE       0
          #endif
    #else
                #define FBM_DMX_SIZE       (FBM_DMX_4K_2K_SIZE)
          
          #if defined(CC_FBM_TWO_FBP)
                #define FBM_DMX2_SIZE      (FBM_DMX_H264_SIZE)
          #else
                #define FBM_DMX2_SIZE       0
          #endif
    #endif
#endif

#if defined(CC_FBM_SUPPORT_MUXER)
#define FBM_MUXER_SIZE          0x400000
#else
#define FBM_MUXER_SIZE          0x0
#endif

#if defined(CC_SUPPORT_DUAL_3D_BROADCASTING)
#define FBM_IMGRZ_3D_KR_BUF     0x600000
#else
#define FBM_IMGRZ_3D_KR_BUF     0
#endif

#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#define FBM_EXTRA_FOR_4K2K_VIDEO    (0x2b20000) //the 0x1f00000 values comes from system fail-booting hint! // 0x120d800+0x160ac00
#else
#define FBM_EXTRA_FOR_4K2K_VIDEO    (0) 
#endif

#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#if defined(ANDROID)
#define FBM_EXTRA_FOR_4K2K_FEEDER_DMX (0x1e00000 +0xA00000) //request by jun.cui
#else
#define FBM_EXTRA_FOR_4K2K_FEEDER_DMX (0x1e00000 +0xA00000 +0x300000) //request by cong.wang 3m for feeder change to R
#endif
#else
#define FBM_EXTRA_FOR_4K2K_FEEDER_DMX (0)
#endif

#define FBM_MPEG_2_SIZE  0x2680000

#if defined(CC_FBM_TWO_FBP)
#define FBM_TWO_FBP_SIZE        (0x800000 + FBM_MPEG_2_SIZE + FBM_EXTRA_FOR_4K2K_VIDEO)
#else
#define FBM_TWO_FBP_SIZE        (0)
#endif

#if defined(CC_FBM_3D_OSD)||defined(CC_MHP_SUPPORT)
#define FBM_JPEVDP_POOL_SIZE (0x1000000)
//#elif defined(CC_TVP_SUPPORT) 
//#define FBM_JPEVDP_POOL_SIZE (0x1000000)
#else
#define FBM_JPEVDP_POOL_SIZE (0)
#endif

#ifdef CC_TVP_SUPPORT
#ifndef FBM_FEEDER_SIZE
#define FBM_FEEDER_SIZE           0x300000
#endif
#else
#define FBM_FEEDER_SIZE           0x0
#endif


#if defined(CC_53XX_FACE_DETECTION)
#define FBM_FD_SIZE             (800*1024)
#else
#define FBM_FD_SIZE             (0)
#endif

#define FBM_DRM_BUF_SIZE        0x80000

//#ifdef CC_TVP_SUPPORT
#ifndef FBM_JPEG_SIZE
//#define FBM_JPEG_SIZE           0x800000
//#endif
//#else
#define FBM_JPEG_SIZE           0x0
#endif
#if defined(CC_H264_LV5_SUPPORT_SINGLE_POOL)
#define FBM_EXTRA_FRAME_NUM     2
#define FMB_EXTRA_FOR_H264_LV5  ((((FBM_EXTRA_FRAME_NUM)*1920*1088*3)>>1) + (((FBM_EXTRA_FRAME_NUM)*1920*1088)>>2))
#else
#define FMB_EXTRA_FOR_H264_LV5  0
#endif


#if defined(DIVX_PLUS_CER) || defined(CC_NETFLIX_CE3_SUPPORT) || defined(CC_FBM_FULL_3D_SUPPORT) || defined(CC_ENABLE_SEAMLESS_FOR_2D)
#if defined(CC_ENABLE_SEAMLESS_FOR_MVC)
#define FBM_DIVX_SIZE           (0xC00000 + FMB_EXTRA_FOR_H264_LV5)
#else
#define FBM_DIVX_SIZE           (0x600000 + FMB_EXTRA_FOR_H264_LV5)
#endif
#else
#define FBM_DIVX_SIZE           (0 + FMB_EXTRA_FOR_H264_LV5)
#endif

#ifdef CC_SUPPORT_RECORD_AV
#define FBM_TDCAV_POOL_SIZE     (0x2c6000)
#else
#define FBM_TDCAV_POOL_SIZE     (0)
#endif

#if defined(CC_TRUSTZONE_SUPPORT)&& defined(CC_SVP_SUPPORT)
#ifdef CC_FBM_TWO_FBP
#define FBM_REDUCE_FBM_DMX_SIZE (0x700000 +FBM_DMX_ADD_SIZE)// 2M for dmx,5M for dmx2
#else
#define FBM_REDUCE_FBM_DMX_SIZE 0x200000
#endif
#else
#define FBM_REDUCE_FBM_DMX_SIZE 0
#endif

#define FBM_MJC_RSV_POOL_SIZE   (0x7800000)

#if defined CC_SUPPORT_TV_STAGEFRIGHT
#if defined(CC_VOMX_TV_COEXIST)
#define VOMX_EXTRA_FRAME_BUFFER_SZ  0x1200000
#else
#define VOMX_EXTRA_FRAME_BUFFER_SZ  0x900000
#endif
#else
#define VOMX_EXTRA_FRAME_BUFFER_SZ  0x0
#endif



// mt5398 FBM size define----------------------------------------------------------------------
#if defined(CC_TVP_SUPPORT)
#define FBM_MEM_CFG_MT5882_3DTV             FBM_ALIGN_MASK(FBM_SIZE_FORMULAR(0x100000 + 0x1000000),4095)
#else
#define FBM_MEM_CFG_MT5882_3DTV             FBM_ALIGN_MASK(FBM_SIZE_FORMULAR(0x100000),4095)
#endif
#define FBM_MEM_CFG_MT5890_3DTV             FBM_SIZE_FORMULAR(0)
#define FBM_MEM_CFG_MT5861_3DTV             FBM_SIZE_FORMULAR(0)
#define FBM_MEM_CFG_MT5890_3DTV_FHD       FBM_SIZE_FORMULAR(0)

#if defined(CC_TVP_SUPPORT)
#define FBM_MEM_CFG_MT5399_A3               FBM_SIZE_FORMULAR(0x58B0000+0xD7000+0x1780000 + 0x101000 + 0x1000000)
#else
#define FBM_MEM_CFG_MT5399_A3               FBM_SIZE_FORMULAR(0x58B0000+0xD7000+0x1780000 + 0x101000)
#endif
// mt5881 FBM size define----------------------------------------------------------------------
#ifdef CC_FLIP_MIRROR_SUPPORT
#define FBM_MEM_CFG_MT5881_128Mx1       FBM_SIZE_FORMULAR(0x3305000)
#else
#define FBM_MEM_CFG_MT5881_128Mx1       FBM_SIZE_FORMULAR(0x3185000)
#endif
#define FBM_MEM_CFG_MT5881_64Mx1        FBM_SIZE_FORMULAR(0x2780000)
#define FBM_MEM_CFG_MT5881_SA_64Mx1        FBM_SIZE_FORMULAR(0x27C2000)

// Audio volume define
#define VOL_POS_14_DB               (14*2)
#define VOL_POS_11_DB               (11*2)
#define VOL_POS_10_DB               (10*2)
#define VOL_POS_9_5_DB              (9*2+1)
#define VOL_POS_6_DB                (6*2)
#define VOL_POS_5_5_DB              (5*2+1)
#define VOL_POS_5_DB                (5*2)
#define VOL_POS_4_DB                (4*2)
#define VOL_POS_3_DB                (3*2)
#define VOL_0_DB                    (0)
#define VOL_NEG_3_DB                (-3*2)
#define VOL_NEG_6_DB                (-6*2)
#define VOL_NEG_12_DB               (-12*2)

#define MT8292_SPEAKER_PGA_080V     (0x14)
#define MT8292_SPEAKER_PGA_288V     (0x16)

#define SCPOS_SPECIAL_VGA_DISABLE           0
#define SCPOS_SPECIAL_VGA_USE_DRAM          1   //both Main/Sub go through normal path
#define SCPOS_SPECIAL_VGA_USE_DISPMODE      2   //main=>display mode, sub=>normal mode

#define PANEL_CHANNEL_SWAP_ALWAYS           1
#define PANEL_CHANNEL_SWAP_IN_SIGNAL_PORT   2
#define PANEL_CHANNEL_SWAP_IN_DUAL_PORT     3

#define SCART_FB_IN_SOY0    0
#define SCART_FB_IN_SOY1    1
#define SCART_FB_IN_SOG     2
#define SCART_FB_IN_NONE    3

#define SCART_FULL_TYPE     0   //Scart 1 &2 : CVBS/S-Video/RGB
#define SCART_HALF_TYPE     1   //Scart 1 : CVBS/RGB, Scart2: CVBS/S-Video

#define SCART_SC_FROM_EXT   (0)
#define SCART_SC_FROM_CH1   (1)
#define SCART_SC_FROM_CH3   (2)


#define INTERNAL_PODCI      (0)     //mt5360 pcmcia pinmux
#define mt5360_NAND_PMX     (7)     //mt5360 nand i/f pinmux
#define MT8295_IO_0mA       (0)
#define MT8295_IO_4mA       (4)
#define MT8295_IO_8mA       (8)
#define MT8295_IO_12mA      (12)

#if  defined(CC_MT5399)
#define DEFAULT_HDMIHPD0_GPIONUM            (217)
#define DEFAULT_HDMIHPD1_GPIONUM            (221)
#define DEFAULT_HDMIHPD2_GPIONUM            (225)
#define DEFAULT_HDMIHPD3_GPIONUM            (229)
#elif defined(CC_MT5882)
#define DEFAULT_HDMIHPD0_GPIONUM            (217)
#define DEFAULT_HDMIHPD1_GPIONUM            (222)
#define DEFAULT_HDMIHPD2_GPIONUM            (225)
#endif

//-----------------------------------------------------------------------------
// Enum constant
//-----------------------------------------------------------------------------

#if defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER)

#include "c_model.h"
#include "../drv_cust/drv_default.h"

#define DRVCUST_InitGet(x)          (x)
#define eDdrClock                   (DEFAULT_DDR_CLOCK)
#define eDramType                   (DEFAULT_DRAM_TYPE)
#define eDramColAddr                (DEFAULT_DRAM_COLADDR)
#define eDdrCL                      (DEFAULT_DDR_CL)
#define eDdrBusX8                   (DEFAULT_DDR_BUS_X8)
#define eFlagDDRQfp                 (FLAG_DDR_QFP)
#define eFlagDDR16BitSwap           (FLAG_DDR_16BITSWAP)
#define eFlagDDRDCBalance			(FLAG_DDR_DCBALANCE)
#define eFlag1GBitSupport           (DEFAULT_DRAM_8_BANKS)
#define eFlagReadODT                (DEFAULT_DRAM_RODT)
#define eFlagWriteODT               (DEFAULT_DRAM_WODT)
#define eDmpllSpectrumPermillage    (DMPLL_SPECTRUM_PERMILLAGE)
#define eDmpllSpectrumFrequency     (DMPLL_SPECTRUM_FREQUENCY)
#define ePcieResetGpio              (PCIE_RESET_GPIO)

typedef UINT32 QUERY_TYPE_T;

#else /* defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER) */

typedef enum 
{
    eZeroStart,                 // Default Value

    // Common information
    eBuildDate,                 // __DATE__
    eBuildTime,                 // __TIME__
    eBrdCstIf,                  // $(BRDCST_IF)
    eDrvCust,                   // $(DRV_CUST)

    // Initial configuration
    eFlagBootVerbose,           // 0
    eNorCpuClock,               // 162000000
    eDramType,                  // DDR1x2
    eDramColAddr,               // COL_ADDR_BIT_10
    eDdrClock,                  // 297000000
    eDdrCL,                     // 6
    eDdrBusX8,                  // 0
    eDmpllSpectrumPermillage,   // 0
    eDmpllSpectrumFrequency,    // 0
    eFlag1GBitSupport,          // 0
    eFlagReadODT,               // 150
    eFlagWriteODT,              // 50
    eFlagDDRQfp,                // 0
    eFlagDDRDCBalance,
    eFlagDDR16BitSwap,          // 0
    eSysResetTimer,             // 0xf00000.  (15/27 second)

    // Tuner driver customization.
    eTunerCtrl,
    eTunerCtrlAna,
    eTunerTypeDefault,
    eTunerTypeOfst0,
    eTunerTypeOfst1,
    eTunerTypeOfst2,
    eDemodResetGpio,
    eDemodTypeOfst,
	eDvbsDemodResetGpio,
    eRfTunerPinGpio,            // GPIO pin for sharp tuner control.
    eBrdcstSrcOfst0,
    eDTMBIFAGCGpio,             //tuner AGC switch
    eTunerI2cBusID,             //tuner use which i2c bus
    eDemodI2cBusID,             //demod use which i2c bus
    eChinaATVTuner30v,
    eTunerRFAGCSwitchGpio,      //GPIO pin for tuner RF AGC switch(For ATV,RF AGC use external voltage from ATD;For DTV,RF AGC use internal )
    
    // Dram setting
    eDramAgentPriority,         // priority list
    eDramBurstLen,              // 15
    eDramBurstLen0,             // 
    eDramBurstLen1, 
    eDramGroup1ArbiterTime,     // 5
    eDramGroup2ArbiterTime,     // 4
    eDramGroup3ArbiterTime,     // 15

    // Release version setting
    eReleaseEnableWatchDog,         // 0, only enable at MP.
    eReleaseWatchdogTimeout,        // 5 seconds.
    eReleaseWatchdogPriority,       // 100
	eWatchdogResetWaitTime,         // 3 seconds.
    eReleaseLoaderEnableWatchDog,   // 0, only enable at MP.
    eReleaseLoaderWatchdogTimeout,  // 10 seconds.

    // Memory setting
    eFbmMemAddr,                // g_u4DramSize - FBM_MEM_CFG_SIZE - 0x4000
    eFbmMemSize,                // 0x1300000
    eFbmMemChBSize,

    // PDWNC setting
    ePdwncPolarity,             // 0
    ePcdWakeupTheKey,           // XXX
    eWakeupReasonGpio,          // 0, 0 is disable.
    eWakeupReasonGpioPolarity,  // 0
    eSWPowerDetectVoltage,
    eEthernetWakeupSupport,     // 1

    //PWM setting
    ePWM0Port,
    ePWM1Port,
    ePWM2Port,

    // USB upgrade setting
    eAutoUsbUpgradeEnable,              // 0
    eEepromUsbUpgradeEnable,            // 1
    eUsbUpgradeFileName,                // upgrade.pkg
    eUsbUpgradeGoldenFileName,          // upgrade.golden
    eUsbUpgradeFileNamePrefix,          // mtk
    eUsbUpgradeFileNameSuffix,          // .bin
    eUsbUpgradeGolednFileNameSuffix,    // .golden
    eUsbEepromAutoTestOffset,
    eUsbUpgradeForceDisable,            // 0
    eLoaderStandbyEnableIfUpgradeBreak, // 0
    eLoaderUpgradeBreakStateOffset,
    eLoaderUpgradeBreakFunc,            // NULL
    eLoaderUpgradeVerCmpFunc,           // NULL
    eLoaderUpgradeDlmCbFunc,            // NULL
    eLoaderUpgradeDlmGetTagVerCbFunc,   // NULL
	eLoaderUpgradeDlmSetTagVerCbFunc,	// NULL
    eLoaderUpgradeDlmCmpTagVerCbFunc,   // NULL
    eLoaderUpgradeDlmVerDataOff,        // 8
    eLoaderUpgradeDlmVerDataLen,        // 8
    eLoaderUpgradeDlmForceVer,          // ffffffff
    eloaderUpgradeOnlyACon,

    // Loader Misc
    eLoaderEnvInitFunc,         // NULL
    eLoaderCECConfigQueryFunc,
    eLoaderUpgradeCheckFunc,    // NULL
    eLoaderCheckStandByFunc,    // NULL
    
    // Loader logo
    eLoaderLogoNumber,          // 0
    eLoaderLogoBackground,      // 0xeeeeee
    eLoaderMsgBackground,       // 0xeeeeee
    eLoaderLogoFlashAddr,       // 0
    eLoaderLogoColorMode,       // 0xd
    eLoaderLogoWidthArray,      // NULL
    eLoaderLogoHightArray,      // NULL
    eLoaderLogoDataIdx0,        // LOADER_LOGO_DATAIDX0 in XXX_bootlogo.h
    eLoaderLogoDataIdx1,        // LOADER_LOGO_DATAIDX1 in XXX_bootlogo.h
    eLoaderLogoDataIdx2,        // LOADER_LOGO_DATAIDX2 in XXX_bootlogo.h
    eLoaderLogoDataIdx3,        // LOADER_LOGO_DATAIDX3 in XXX_bootlogo.h
    eLoaderLogoDataIdx4,        // LOADER_LOGO_DATAIDX4 in XXX_bootlogo.h
    eLoaderLogoDataIdx5,        // LOADER_LOGO_DATAIDX5 in XXX_bootlogo.h
    eLoaderLogoDataIdx6,        // LOADER_LOGO_DATAIDX6 in XXX_bootlogo.h
    eLoaderLogoDataIdx7,        // LOADER_LOGO_DATAIDX7 in XXX_bootlogo.h
    eLoaderLogoDataIdx8,        // LOADER_LOGO_DATAIDX8 in XXX_bootlogo.h
    eLoaderLogoDataIdx9,        // LOADER_LOGO_DATAIDX9 in XXX_bootlogo.h
    eLoaderDualBootOffset,      // 0
    eLoaderPanelLogoCheckFunc,  //NULL
    
    // Panel customization
    eDefaultPanelSelect,        // PANEL_DEFAULT
    eLvdsMsbSw,                 // option
    eBackLightGpio,             // 69
    eBackLightGpio2,            // option
    eBackLightOnVal,            // 0
    ePanelPowerGpio,            // Gpio 3
    ePanelPowerOnVal,           // 0
    ePanelDCRGpio,              // Gpio 5
    ePanelDCROnVal,             // 1
    ePanelPdpVsGpio,            // Gpio 3
    ePanelPdpVsOnVal,           // 0
    eTimerDimmingFreq,          // 180~260 (Hz)
    eDimmingFreq60,             // hz
    eDimmingFreq50,             // hz
    eBacklightHigh,             // 0~255
    eBacklightMiddle,           // 0~255
    eBacklightLow,              // 0~255
    eSpreadSpectrumPermillage,  // 0~1000 (0/00)
    ePanelPwmPolarity,          // PWM_LOW_PANEL_BRIGHT
    ePanelPwmPrescalar,         // set bTimeBase to control PWM freq
    ePanelIndexFromEeprom,      // option
    ePanelIndexOffset1,         // option
    ePanelIndexOffset2,         // option
    ePanelIndexOffset3,         // option
    ePanelTableNandFlashPart,   // option Load Panel table from NAND flash PART ID
    ePanelBitSelectCtrlGpio,
    ePanelLvdsSelectCtrlGpio,
    ePanelDisableDimming,       // disable using PWM1, let gpio68 free
    ePanelChannelSwap,          // change even and odd channel
    ePanelIndexFromGpioNum,     // option
    ePanelIndexFromGpioIdx,     // option
    ePanelIndexFromGpioMap,     // option
    ePanelTTLOutput,            // option
    ePanelLvds8bitPinMap,       // option
    ePanelLvds10bitPinMap,      // option
    ePanelLvds10bitPNSwap,      // option
    ePanelVB1LaneSwap,  		// option
    ePanelVB1LanePNSwap,   		// option    
    ePanelFlashTableSize,       // option
    ePanelFlashTableAddress,    // option
    ePanelEepromToFlashIndexMap,// option
    eLvdsDrivingCurrent,        // option
    ePanelBacklightPwmPort,     // [0/1/2]
    ePanelBacklightGPIO,        // GPIO or PWM output
    eLvdsACommonModeVoltage,    // option
    eLvdsBCommonModeVoltage,    // option

    eLvdsACtrlEven,
    eLvdsACtrlOdd,
    eLvdsBCtrlEven,
    eLvdsBCtrlOdd,
    ePanelPixelShiftHMax,
    ePanelPixelShiftVMax,

    ePanel242530pOutput,

    eFlagBacklightBoostCtrl,        // Support backlight boost control.
    eFlagBacklightBoostPort,        // Select backlight boost port.
    eFlagBacklightBoostInvert,      // Support boost signal invert by BJT circuit.
    eFlagBacklightDimmingLock,      // Support backlight dimming lock to output v-sync.
    eFlagBacklightDimmingInvert,    // Support dimming signal invert by BJT circuit.

    // Audio customization
    eAudioOutPadNormal,
    eAudioOutPadDrvCur,
    eAudioSpkType,
    eAudioSurroundSpkType,
    eAudioCenterSpkType,
    eAudioCh910SpkType,
    eAudioSubwooferOn,
    eAudioBassMngCutOffFreq,
    eAudioSoftMuteOrder,
    eAudioBbeLevel,
    eAudioBbeProcess,
    eAudioBbeLoContour,
    eAudioBbe3dGain,
    eAudioBypassPostProc,
    eAudioDacMclk,
    eAudioDacUsage,
    eAudioInterDacChlConfig,
    eAudioInterDacChlSpecialConfig,
    eAudioAoutPadConfig,
    eAudioOpctrlMuteGpioNum,
    eAudioOpctrlMuteGpioPolarity,
    eAudioMuteGpioNum,
    eAudioAmpEnableGpioNum,
    eAudioMuteGpioPolarity,
    eAudioAmpGpioPolarity,
    eAudioAmpResetGpioNum,
    eAudioAmpResetGpioPolarity,
    eAudioArcGpioPolarity,
    eAudioArcEnableGpioNum,
    eAudioScartEnableGpioNum,
    eAudioSCcartGpioPolarity,
    eAudioInputSwGpioSel,
    eAudioInputMuxSelTable,
    eAudioOutFmtTable,
    eAudioOutFmtTable2,
    eAudioInFmtTable,
    eAudioInputVolTable,
    eAudioAvInputSrcVolTable,
    eAudioInputLineOutVolTable,
    eAudioAvChlEqTable,
    eAudioAvChlEqCoefTable,
    eAudioPeqFrCfg,
    eAudioPeqBwCfg,
    eAudioPeqGainCfg,
    eAudioBassTreble,
    eAudioBassTrebleCoefTable,
    eAudioVSModeCfg,
    eAudioPSMCfg,
    eAudioInputSrcAgcFlag,
    eAudioInputSrcAgcVol,
    eAudioVolBaseLevel,
    eAudioLineOutFixVol,
    eAudioPowerOnMusicVol,
    eAudioVolTable,
    eAudioChannelVolTable,
    eAudioChlVolExtraGain,
    eAudioChannelDelay,
    eAudioIecCopyright,
    eAudioIecCategory,
    eAudioIecWordLength,
    eAudioIecChannelStatusBySource,
    eAudioIecRawDelayFlag,
    eAudioIecRawDelay,
    eAudioTableCfg,
    eAudioAdacCfg,
    eAudioHeadphonePlugInGpio,
    eAudioHeadPhonePlugInPolarity,
    eAudioHeadPhonePlugInLineOutMute,
    eAudioDcDetectGpio,
    eAudioDcDetectPolarity,
    eAudioLineOutUseInterDac,
    eAudioMuteAllSource,
    eAudioDvdGpio,
    eAudioDvdPolarity,
    eAudioDvdInputChl,
    eAudioSpkAutoOffWhenVolMute,
    eAudioCustSpecGpioCtrl,
    eAudioCustSpec2GpioCtrl,
    eAudioCustHpGpioCtrl,
    eAudioSpecGpioPolarity,
    eAudioSpec2GpioPolarity,
    eAudioHpGpioPolarity,
    eAudioSpkPath,
    eAudioAvcTarget,
    eAudioAvcGainUp,
    eAudioAvcAttackThres,
    eAudioAvcAdjustRate,
    eAudioAvcUiAdjustRate,
    eAudioMT8292PGA0GAIN,
    eAudioMT8292PGA1GAIN,
    eAudioMT8292PGA2GAIN,
    eAudioMT8292HPGAIN,
    eAudioLineInLrSwap,
    eAudioVolumeOffset,
    eAudioTvSysFineTuneVol,
    eAudioMtsFactory,
    eAudioA2Factory,
    eAudioPalFactory,
    eAudioNvmEnable,
    eAudioNvmLength,
    eAudioNvmData,
    eAudioNvmGetCountry,
    eAudioProfileDefault,
    eAudioProfileStart,
    eAudioProfileEnd,
    eAudioLimiterMode,                  // 0: off, 1: Adaptive mode, 2: Fix mode
    eAudioLimiterFixThreshold,          // the max. value is 0x100000
    eAudioDigitalDataConfig,
    eAudioTunerATD,
    eAudioSTA335ResetGpio,
    eAudioSTA335ResetGpioPolarity,
    eAudioADCNegDigitalGain,            // [Joel]20101026
    eAudioAL1AR1PhaseInverse,
    eAudioAL2AR2PhaseInverse,
    eAudioAL3AR3PhaseInverse,
    eAudioAL4AR4PhaseInverse,
    eAudioPowerstageUseAL0AR0,
    eAudioPowerstageUseAL1AR1,
    eAudioPowerstageUseAL2AR2,
    eAudioPowerstageUseAL3AR3,
#ifdef CC_AUDIO_EU_PWM_OUTPUT_AUXDEC
    eAudioAuxOutAtAL0AR0,
    eAudioAuxOutAtAL1AR1,
    eAudioAuxOutAtAL2AR2,
    eAudioAuxOutAtAL3AR3,
#endif
    eAudioDacAL0AR0Enable,
    eAudioDacAL1AR1Enable,
    eAudioDacAL2AR2Enable,
    eAudioDacAL3AR3Enable,
    eAudioInternalLDOEnable,
    eAudioDigiAMPResetGpio,
    eAudioDigiAMPResetGpioPolarity,
    eAudioADFlag,
    eAudioDownmixPosition,
    eAudioDownmixLR,
    eAudioDownmixWithBalance,
    eAudioPanFadeControl,
    eAudioMultiPairOutput,
    eAudioBI2SEnable,
    eAudioPcmPrebufBankNum,
    eAudioAI2SuseforSpeaker,
    eAudioMultichlDDCO,
    eAudioAmpFadeOutTime,
    eAudioLoaderMusicData,
    eAudioCustomerMusicTable,
    eAudioFlashAQPartition,
    eAudioFlashAQSize,
    eAudioFlashAQData,
    eAudioFlashAQDataSize,
    eAudioFlashAQDataDscrpt,
    eAudioFlashAQDataDscrptSize,
    eAudioAdspBinPartition,
   
    // sunman_tshd
    eAudioTSHDInputGain,
    eAudioTSHDOutputGain,
    eAudioTSHDSurrLvl,
    eAudioTSHDSpkSize,
    eAudioTSHDTbCtrl,
    eAudioTSHDDefCtrl,
    eAudioTSHDElevCtrl,
    eAudioTSHDAnaSize,
    eAudioTSHDComprCtrl,
    eAudioTSHDLevelDep,

    // bofeng_cc3d
    eAudioCC3Dflag,
    eAudioCC3Dmode,
    eAudioCC3DInputGain,
    eAudioCC3DOutputGain,
    eAudioCC3DSpkSize,
    eAudioCC3DTbCtrl,
    eAudioCC3DAnaSize,
    eAudioCC3DComprCtrl,
    eAudioCC3DLevelDep,
    eAudioCC3DHpfSize,
    eAudioCC3DIniSize,

    // MTK Virtual Surround
    eAudioMtkVsClarity,
    eAudioMtkVsWidth,
    eAudioMtkVsLRGain,
    eAudioMtkVsXtalk,
    eAudioMtkVsOutputGain,
    eAudioMtkVsBassGain,
    eAudioMtkVsFo,

    // CC_AUD_DSP_SUPPORT_AUDYSSEY
    eAudioAdvChCalbGain_L,
    eAudioAdvChCalbGain_R,
    eAudioAbxFilterSet,
    eAudioAbxDryGainIndx,
    eAudioAbxWetGainIndx,
    eAudioAeq5BandTable,
    eAudioMixsoundGain,
	eAudioMixsoundLength,

#ifdef CC_AUD_SILENCE_SUPPORT
    // add for silence support, should customize to shm.
    eAudioSilenceMode,
    eAudioSilenceFixThreshold,
    eAudioSilenceAttackWait,
    eAudioSilenceAttackFixGain,
    eAudioSilenceReleaseStep,
    eAudioSilenceAttackStep,
#endif

    eAudioAprocEnable,
    eAudioPeqEnable,


    // Other GPIO customization
    eExternalStatus,            // 0    bool, 1 to enable loader standby at AC on.
    eStandbySetGpio,            // 207  AC-on detect circuit, only work on IC_AB.
    ePowerStatusGpio,           // 206  AC-on detect circuit, only work on IC_AB.
    eStdbyUsePowerBtnCount,     // 0    Use Power button State to set standby
    eStdbyUsePrevPowerState,    // 0    Use previous power state to set standby
    eStdbyUseVGAStatus,         // 0    Use VGA status to enter standby mode when power key trigger.
    eGpiotimerInitFunc,         // NULL

    // Boot/PowerDown setup GPIOs, boot set OnVal, PowerDown set !OnVal.
    ePowerLedGpio,              // NULL
    ePowerLedOnVal,             // 0
    eT8032CtrlErrLed,           //NULL
    eT8032CtrlErrLedOnVal,      //NULL
    eBackupLedGpio,             // NULL
    eBackupLedOnVal,            // 0
    ePowerCtrlDelayMs,          // 5 ms.
    ePowerCtrlGpio,             // NULL
    ePowerCtrlOnVal,            // 0
    eDvdCtrlGpio,               // NULL
    eDvdCtrlOnVal,              // 0
    eSupportDarkLed,            // 0
    ePowerLedDarkVal,           // XXX
    eBackupLedDarkVal,          // XXX
    eSpecialWakeupGpio,         // XXX
    eSpecialWakeupOnVal,        // XXX
    eWakeupOnWLanGpio,
	eResetWLanGpio,
    eWakeupOnWLanPol,
    eGpioPowerOnFunc,           // GPIO_MtkPowerOnFunc()
    eGpioPowerOffFunc,          // GPIO_MtkPowerOffFunc()
    eGpioLedCtrlFunc,           // GPIO_MtkLedCtrlFunc()
    eFlagSupportPowerLedOff,    // 0
    eCompanionChipGpio,         // 0    Check Companion chip existed gpio
    eCompanionChipGpioPolarity, // 0
    eFlagBootBlinkingLedSupport,
    eBootBlinkingLed,
    eBootBlinkingLedOnVal,
    eBootBlinkingLedPeriod,

    // MT8292 GPIO support
    eFlagMt8292GpioSupport,     // 0
    eMt8292ResetGpio,
    eMt8292MuteGpio,

    // MT8280 GPIO
    eMt8280ResetGpio,           // 0
    eMt8280ResetGpioPolarity,
    eMT8280CmdAckGpio,
    eMT8280CmdAckGpioPolarity,
    eMT8280SendPanel,

    // EEPROM customization
    eSystemEepromBus,           // 0
    eSystemEepromAddress,       // 0xA0
    eSystemEepromSize,          // 0x800 => 2048 KB.
    eSystemEepromClkDiv,        // 0x200 => 27 Mhz/512.
    eSystemEepromPageSize,      // 8
    eSysEepromWPGpio,
    eSysEepromWPEnablePolarity,
    eSysEepromResetTime,
    eSysEepromEnableRetry,

    eHdcpEepromBus,             // 0
    eHdcpEepromAddress,         // 0xA0
    eHdcpEepromSize,
    eHdcpEepromClkDiv,
    eHdcpEepromOffset,
    eHdcpEepromWPGpio,
    eHdcpEepromWPEnablePolarity,

    // HDMI EDID PA address
    eHdmiEdidPARx0,
    eHdmiEdidPARx1,
    eHdmiEdidPARx2,
    eHdmiEdidPARx3,

    eEdidEepromWPGpio,
    eEdidEepromWPEnablePolarity,
    eEdidVgaIndexOffset,
    eEdidHdmiIndexOffset,
    eEdidVgaSize,
    eEdidHdmiSize,
    eEdidVgaStorageSize,
    eEdidHdmiStorageSize,
    eEepromHdmiHdcpOffset ,
    eEepromHdmiHdcpSize ,
    eEepromHdmiEdidOffset ,
    eEepromVgaEdidOffset ,
    eEepromDtvDriverOffset,
    eEepromDtvDriverSize,
    eEepromNptvDriverOffset,
    eEepromNptvDriverSize,
    eEepromDriverSize,
    eEepromMwOffset,
    eEepromMwSize,
#if 1
    eEepromHDCP2xOffset,
    eEepromHDCP2xSize,
#endif
    eEepromErrorCodeOffset,
    eEepromErrorCodeSize,
    eEepromStateCodeOffset,
    eEepromStateCodeSize,
    eEepromCECMenuLanguageOffset,
    eEepromCECEnableOffset,    
    eEepromNHLRCPOffset,
    eEepromCECOneTouchPlayEnableOffset,
    eEepromDataMemOffset,
    eEepromGammaAddr,
    eEepromGammaSize,
    eEepromGammaCheckAddr,
    eEepromGammaCheckAddr1,
    eEepromWatchdogFlagAddr,
    eEepromDIAGFlagAddr,
    eEepromStoreDemoAddr,
    eEepromVLogoAddr,
    eEepromFactoryModeAddr,

    // NOR flash customization
    eNorFlashTotalSize,             // 4*1024*1024 = 4 Mbyte
    eNorFlashLoaderSize,            // 512*1024 =  512 Kbyte
    eNorFlashAppImageSize,          // 3*1024*1024 = 3 Mbyte
    eNorFlashDynamicPartitionSize,  // 3*1024*1024 = 3 Mbyte
    eNorFlashStaticPartitionSize,   // 3*1024*1024 = 3 Mbyte
    eHDCPNorPartId,                 // 0, if 0/1, HDCP on EEPROM, otherwise on NORFlash.
    eVGAEDIDNorPartId,              // 0, if 0/1, vga EDID on EEPROM, otherwise on NORFlash.
    eVGAEDIDNorOffset,              // vga edid offset in the nor partition
    eHDMIEDIDNorPartId,             // 0, if 0/1, hdmi EDID on EEPROM, otherwise on NORFlash.
    eHDMIEDIDNorOffset,             // hdmi edid offset in the nor parition

    // NOR flash partition size customization
    eNorFlashPartSize0,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize1,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize2,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize3,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize4,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize5,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize6,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize7,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize8,
    eNorFlashPartSize9,
    eNorFlashPartSize10,
    eNorFlashPartSize11,
    eNorFlashPartSize12,
    eNorFlashPartSize13,
    eNorFlashPartSize14,
    eNorFlashPartSize15,
    eNorFlashPartSize16,
    eNorFlashPartSize17,
    eNorFlashPartSize18,
    eNorFlashPartSize19,
    eNorFlashPartSize20,
    eNorFlashPartSize21,

    // NOR flash partition offset customization
    eNorFlashPartOffset0,
    eNorFlashPartOffset1,
    eNorFlashPartOffset2,
    eNorFlashPartOffset3,
    eNorFlashPartOffset4,
    eNorFlashPartOffset5,
    eNorFlashPartOffset6,
    eNorFlashPartOffset7,
    eNorFlashPartOffset8,
    eNorFlashPartOffset9,
    eNorFlashPartOffset10,
    eNorFlashPartOffset11,
    eNorFlashPartOffset12,
    eNorFlashPartOffset13,
    eNorFlashPartOffset14,
    eNorFlashPartOffset15,
    eNorFlashPartOffset16,
    eNorFlashPartOffset17,
    eNorFlashPartOffset18,
    eNorFlashPartOffset19,
    eNorFlashPartOffset20,
    eNorFlashPartOffset21,

    // NAND flash customization
    eNANDFlashTotalSize,
    eNANDFlashLoaderSize,
    eNANDFlashAppImageSize,
    eNANDFlashDynamicPartitionSize,
    eNANDFlashDynamicPartitionID,
    eNANDFlashStaticPartitionSize,
    eNANDFlashStaticPartitionLastID,
    eHDCPNANDPartId,                    // 0, if 0/1, HDCP on EEPROM, otherwise on NORFlash.
    eVGAEDIDNANDPartId,                 // 0, if 0/1, VGAEDID on EEPROM, otherwise on NORFlash.
    eVGAEDIDNANDOffset,
	eHDMIEDIDNANDPartId,                // 0, if 0/1, VGAEDID on EEPROM, otherwise on NORFlash.
    eHDMIEDIDNANDOffset,

    // NAND flash partition id
    eNANDFlashPartIdSysImgA,
    eNANDFlashPartIdUpA,
    eNANDFlashPartIdSysImgB,
    eNANDFlashPartIdUpB,
    eNANDFlashPartIdChannelDataA,
    eNANDFlashPartIdChannelDataB,
    eNANDFlashPartIdEepromA,
    eNANDFlashPartIdEepromB,
    eNANDFlashPartEnd,

    // NAND flash partition offset customization
    eNANDFlashPartOffset0,
    eNANDFlashPartOffset1,
    eNANDFlashPartOffset2,
    eNANDFlashPartOffset3,
    eNANDFlashPartOffset4,
    eNANDFlashPartOffset5,
    eNANDFlashPartOffset6,
    eNANDFlashPartOffset7,
    eNANDFlashPartOffset8,
    eNANDFlashPartOffset9,
    eNANDFlashPartOffset10,
    eNANDFlashPartOffset11,
    eNANDFlashPartOffset12,
    eNANDFlashPartOffset13,
    eNANDFlashPartOffset14,
    eNANDFlashPartOffset15,
    eNANDFlashPartOffset16,
    eNANDFlashPartOffset17,
    eNANDFlashPartOffset18,
    eNANDFlashPartOffset19,
    eNANDFlashPartOffset20,
    eNANDFlashPartOffset21,
    eNANDFlashPartOffset22,
    eNANDFlashPartOffset23,
    eNANDFlashPartOffset24,
    eNANDFlashPartOffset25,
    eNANDFlashPartOffset26,
    eNANDFlashPartOffset27,
    eNANDFlashPartOffset28,
    eNANDFlashPartOffset29,
    eNANDFlashPartOffset30,
    eNANDFlashPartOffset31,
#ifdef CC_MTD_SUPPORT_64_PARTITION    
    eNANDFlashPartOffset32,
    eNANDFlashPartOffset33,
    eNANDFlashPartOffset34,
    eNANDFlashPartOffset35,
    eNANDFlashPartOffset36,
    eNANDFlashPartOffset37,
    eNANDFlashPartOffset38,
    eNANDFlashPartOffset39,
    eNANDFlashPartOffset40,
    eNANDFlashPartOffset41,
    eNANDFlashPartOffset42,
    eNANDFlashPartOffset43,
    eNANDFlashPartOffset44,
    eNANDFlashPartOffset45,
    eNANDFlashPartOffset46,
    eNANDFlashPartOffset47,
    eNANDFlashPartOffset48,
    eNANDFlashPartOffset49,
    eNANDFlashPartOffset50,
    eNANDFlashPartOffset51,
    eNANDFlashPartOffset52,
    eNANDFlashPartOffset53,
    eNANDFlashPartOffset54,
    eNANDFlashPartOffset55,
    eNANDFlashPartOffset56,
    eNANDFlashPartOffset57,
    eNANDFlashPartOffset58,
    eNANDFlashPartOffset59,
    eNANDFlashPartOffset60,
    eNANDFlashPartOffset61,
    eNANDFlashPartOffset62,
    eNANDFlashPartOffset63,
#endif
    // NAND flash partition size customization
    eNANDFlashPartSize0,
    eNANDFlashPartSize1,
    eNANDFlashPartSize2,
    eNANDFlashPartSize3,
    eNANDFlashPartSize4,
    eNANDFlashPartSize5,
    eNANDFlashPartSize6,
    eNANDFlashPartSize7,
    eNANDFlashPartSize8,
    eNANDFlashPartSize9,
    eNANDFlashPartSize10,
    eNANDFlashPartSize11,
    eNANDFlashPartSize12,
    eNANDFlashPartSize13,
    eNANDFlashPartSize14,
    eNANDFlashPartSize15,
    eNANDFlashPartSize16,
    eNANDFlashPartSize17,
    eNANDFlashPartSize18,
    eNANDFlashPartSize19,
    eNANDFlashPartSize20,
    eNANDFlashPartSize21,
    eNANDFlashPartSize22,
    eNANDFlashPartSize23,
    eNANDFlashPartSize24,
    eNANDFlashPartSize25,
    eNANDFlashPartSize26,
    eNANDFlashPartSize27,
    eNANDFlashPartSize28,
    eNANDFlashPartSize29,
    eNANDFlashPartSize30,
    eNANDFlashPartSize31,
#ifdef CC_MTD_SUPPORT_64_PARTITION    
    eNANDFlashPartSize32,
    eNANDFlashPartSize33,
    eNANDFlashPartSize34,
    eNANDFlashPartSize35,
    eNANDFlashPartSize36,
    eNANDFlashPartSize37,
    eNANDFlashPartSize38,
    eNANDFlashPartSize39,
    eNANDFlashPartSize40,
    eNANDFlashPartSize41,
    eNANDFlashPartSize42,
    eNANDFlashPartSize43,
    eNANDFlashPartSize44,
    eNANDFlashPartSize45,
    eNANDFlashPartSize46,
    eNANDFlashPartSize47,
    eNANDFlashPartSize48,
    eNANDFlashPartSize49,
    eNANDFlashPartSize50,
    eNANDFlashPartSize51,
    eNANDFlashPartSize52,
    eNANDFlashPartSize53,
    eNANDFlashPartSize54,
    eNANDFlashPartSize55,
    eNANDFlashPartSize56,
    eNANDFlashPartSize57,
    eNANDFlashPartSize58,
    eNANDFlashPartSize59,
    eNANDFlashPartSize60,
    eNANDFlashPartSize61,
    eNANDFlashPartSize62,
    eNANDFlashPartSize63,
#endif
    // NAND flash partition name customization
    eNANDFlashPartName0,
    eNANDFlashPartName1,
    eNANDFlashPartName2,
    eNANDFlashPartName3,
    eNANDFlashPartName4,
    eNANDFlashPartName5,
    eNANDFlashPartName6,
    eNANDFlashPartName7,
    eNANDFlashPartName8,
    eNANDFlashPartName9,
    eNANDFlashPartName10,
    eNANDFlashPartName11,
    eNANDFlashPartName12,
    eNANDFlashPartName13,
    eNANDFlashPartName14,
    eNANDFlashPartName15,
    eNANDFlashPartName16,
    eNANDFlashPartName17,
    eNANDFlashPartName18,
    eNANDFlashPartName19,
    eNANDFlashPartName20,
    eNANDFlashPartName21,
    eNANDFlashPartName22,
    eNANDFlashPartName23,
    eNANDFlashPartName24,
    eNANDFlashPartName25,
    eNANDFlashPartName26,
    eNANDFlashPartName27,
    eNANDFlashPartName28,
    eNANDFlashPartName29,
    eNANDFlashPartName30,
    eNANDFlashPartName31,
#ifdef CC_MTD_SUPPORT_64_PARTITION    
    eNANDFlashPartName32,
    eNANDFlashPartName33,
    eNANDFlashPartName34,
    eNANDFlashPartName35,
    eNANDFlashPartName36,
    eNANDFlashPartName37,
    eNANDFlashPartName38,
    eNANDFlashPartName39,
    eNANDFlashPartName40,
    eNANDFlashPartName41,
    eNANDFlashPartName42,
    eNANDFlashPartName43,
    eNANDFlashPartName44,
    eNANDFlashPartName45,
    eNANDFlashPartName46,
    eNANDFlashPartName47,
    eNANDFlashPartName48,
    eNANDFlashPartName49,
    eNANDFlashPartName50,
    eNANDFlashPartName51,
    eNANDFlashPartName52,
    eNANDFlashPartName53,
    eNANDFlashPartName54,
    eNANDFlashPartName55,
    eNANDFlashPartName56,
    eNANDFlashPartName57,
    eNANDFlashPartName58,
    eNANDFlashPartName59,
    eNANDFlashPartName60,
    eNANDFlashPartName61,
    eNANDFlashPartName62,
    eNANDFlashPartName63,

#endif
    // NAND flash partition encypt customization
    eNANDFlashPartEncypt0,
    eNANDFlashPartEncypt1,
    eNANDFlashPartEncypt2,
    eNANDFlashPartEncypt3,
    eNANDFlashPartEncypt4,
    eNANDFlashPartEncypt5,
    eNANDFlashPartEncypt6,
    eNANDFlashPartEncypt7,
    eNANDFlashPartEncypt8,
    eNANDFlashPartEncypt9,
    eNANDFlashPartEncypt10,
    eNANDFlashPartEncypt11,
    eNANDFlashPartEncypt12,
    eNANDFlashPartEncypt13,
    eNANDFlashPartEncypt14,
    eNANDFlashPartEncypt15,
    eNANDFlashPartEncypt16,
    eNANDFlashPartEncypt17,
    eNANDFlashPartEncypt18,
    eNANDFlashPartEncypt19,
    eNANDFlashPartEncypt20,
    eNANDFlashPartEncypt21,
    eNANDFlashPartEncypt22,
    eNANDFlashPartEncypt23,
    eNANDFlashPartEncypt24,
    eNANDFlashPartEncypt25,
    eNANDFlashPartEncypt26,
    eNANDFlashPartEncypt27,
    eNANDFlashPartEncypt28,
    eNANDFlashPartEncypt29,
    eNANDFlashPartEncypt30,
    eNANDFlashPartEncypt31,
#ifdef CC_MTD_SUPPORT_64_PARTITION    
    eNANDFlashPartEncypt32,
    eNANDFlashPartEncypt33,
    eNANDFlashPartEncypt34,
    eNANDFlashPartEncypt35,
    eNANDFlashPartEncypt36,
    eNANDFlashPartEncypt37,
    eNANDFlashPartEncypt38,
    eNANDFlashPartEncypt39,
    eNANDFlashPartEncypt40,
    eNANDFlashPartEncypt41,
    eNANDFlashPartEncypt42,
    eNANDFlashPartEncypt43,
    eNANDFlashPartEncypt44,
    eNANDFlashPartEncypt45,
    eNANDFlashPartEncypt46,
    eNANDFlashPartEncypt47,
    eNANDFlashPartEncypt48,
    eNANDFlashPartEncypt49,
    eNANDFlashPartEncypt50,
    eNANDFlashPartEncypt51,
    eNANDFlashPartEncypt52,
    eNANDFlashPartEncypt53,
    eNANDFlashPartEncypt54,
    eNANDFlashPartEncypt55,
    eNANDFlashPartEncypt56,
    eNANDFlashPartEncypt57,
    eNANDFlashPartEncypt58,
    eNANDFlashPartEncypt59,
    eNANDFlashPartEncypt60,
    eNANDFlashPartEncypt61,
    eNANDFlashPartEncypt62,
    eNANDFlashPartEncypt63,
#endif
    // 8032 customization
    eT8032uPOffset,             // 0
    eT8032uPDualOffset,         // 0
    eT8032uPSize,               // 0

    // Reboot reason location in eeprom.
    eRebootEepromAddress,
    eRebootEepromSize,

    // USB Loader polling time in ms.
    eUSBLoaderWaitTime,

    // USB VBUS0 enable
    eUSBVbus0Gpio,
    eUSBVbus0EnablePolarity,

    // USB VBUS1 enable
    eUSBVbus1Gpio,
    eUSBVbus1EnablePolarity,

    // USB VBUS2 enable
    eUSBVbus2Gpio,
    eUSBVbus2EnablePolarity,

    // USB VBUS3 enable
    eUSBVbus3Gpio,
    eUSBVbus3EnablePolarity,

    // USB0 over current flag enable.
    eUSB0OCGpio,
    eUSB0OCEnablePolarity,

    // USB1 over current flag enable.
    eUSB1OCGpio,
    eUSB1OCEnablePolarity,

    // USB2 over current flag enable.
    eUSB2OCGpio,
    eUSB2OCEnablePolarity,

    // USB3 over current flag enable.
    eUSB3OCGpio,
    eUSB3OCEnablePolarity,
        //usb hub reset     
	eUSBHubResetGpio,
	eUSBHubResetPolarity,

    // USB Support flag Configure in HW for Loader
    eUSB0LoaderConfig,
    eUSB1LoaderConfig,
    eUSB2LoaderConfig,
    eUSB3LoaderConfig,
    
	// USB Support flag Configure in HW
	eUSB0Config,
	eUSB1Config,
	eUSB2Config,
	eUSB3Config,

    // USB HSTERMC impedance control.
    eUSBHSTERMC,

    // USB vbus turn on delay.
    eUSBVbusTurnOnDelay,

    // USB Full speed only enable.
    eUSBFullSpeedOnlyEnable,

    // USB disable suspend mode.
    eUSBDisableSuspendMode,
    // MSDC gpio
    eMSDC0DetectGpio,
    eMSDC0WriteProtectGpio,
    eMSDC0PoweronoffDetectGpio,
    eMSDC0VoltageSwitchGpio,
    eMSDCbackup1Gpio,
    eMSDCbackup2Gpio,
    // FCI card detect pin
    eFCICardDetectGpio,

    // Button customization
    eFlagButtonSupport,         // 0
    eSingleGpioKeyList,         // NULL
    eSingleServoKeyList,        // NULL
    eGreedGpioKeyList,          // NULL
    eMultiKeyList,
    eCombKeyList,
    eKeyPadWakeupBit,           // DKBIT_POWER
    eKeyPadPollingTime,         // 50ms
    eKeyPadCheckTime,           // 2

    // P/I Gain customization
    ePIGainSetting,             //PIGain table Modify by W.C Shih
    ePIGainTLSize,              //PIGain table Modify by W.C Shih

    // Auto Check Polarity
    ePolitaryCheck,             // VGA Politary Check Modify by W.C Shih2006/10/30

    eVGAADSpec,                 // VGA AD Spec Modify by W.C Shih 2006/12/01
    eVGAHFreqSpecHeight,        // VGA H-Frequence Modify by W.C Shih 2007/01/08
    eVGAHFreqSpecLow,           // VGA H-Frequence Modify by W.C Shih 2007/01/08
    eVGAVFreqSpecHeight,        // VGA H-Frequence Modify by W.C Shih 2007/01/08
    eVGAVFreqSpecLow,           // VGA H-Frequence Modify by W.C Shih 2007/01/08

    // IRRX customization
    eIrrxType,                  // NEC
    eIrrxKeyUpTimeout,          // 200 ms.
    eIrrxRepeatValidTime,       // 200 ms.
    eIrrxNecKeyMap,             // All NEC key maps
    eIrrxNecMapArraySize,       //
    eIrrxNecPulse1,             // 8, 0xff to disable
    eIrrxNecPulse2,             // 0, 0xff to disable
    eIrrxNecPulse3,             // 0, 0xff to disable
    eIrrxRc5KeyMap,             // All RC5 key maps
    eIrrxRc5MapArraySize,       //
    eIrrxHotkeyArray,           // Hotkey Array
    eIrrxHotkeyArraySize,       // Hotkey Size
    eIrrxFlagPowerOnWakeup,     //
    eIrrxRc5KeyMapFunc,         // NULL, Key map function
    eIrrxRc6KeyMapFunc,         // NULL, Key map function
    eIrrxXferUsrToCrystalFunc,  // NULL, Raw data to Key xfer function.
	eIrrxUsrGetCystalKeyFunc,
    eIrrxSetUsrWakeupKeyFunc,   // NULL, Set wakeup key in PDWNC register
    eIrrxFlagBlinking,          // 0, flag to blink led when ir receiving.
    eIrrxFlagPrgUpDownWakeup,   // 0, flag to enable IR CH+/- wakeup.
    eIrrxFlagAllWakeup,         // 0, flag to enable all key wakeup.
    eIrrxFlagUserDefineWakeup,  // 0, flag to enable user defined wakeup key settings.
    eIrrxUsrConfig,             // 0x4121
    eIrrxUsrSaperiod,           // 0x0032
    eIrrxUsrThreshold,          // 0x0001
    eUsrIRClkSel,
    eIrrxRepeatPulseUp,         // 5
    eIrrxRepeatPulseDown,       // 2
    eIrrxSetUsrNotifyFunc,
    eIrrxLedBlinkGpio,
    eIrrxLedBlinkOnVal,
    eIrrxLedBlinkDuration,
    eSetSignalUsrNotifyFunc,
    eFlagIrrxSoundSupport,
    eFlagIrrxSoundSupportAlwaysEnable,
    eIrrcIsUsrLongPress,
    eIrrcCheckRepeatByRaw,      // 0
	eIrrcNfyBlinkCfgChg,
    eIrHotKeyWakeupSupport,
    eIrHotKeyNumEepOffset,

    // Factory
    eFlagFactoryEdid,               // 0
    eFlagFactoryMode2,              // 0
    eMagicMaxInputForceFactory,     // NULL, define the magic char to force factory mode. must > 0x80
    eMagicMinInputForceFactory,     // NULL, define the magic char to force factory mode. must < 0x20

    // SCPOS_SUPPORT_SPECIAL_VGA
    eFlagScposSpecalVGA,            // 0
    eFlagMuteDispFmtChg,

    // Non-linear Scaling Factor
    eFlagScposFixedNonLinearFactor,
    eFlagScposNonLinearSlope,
    eFlagScposNonLinearEndPointRatio,

    // VGA Max Pixel Rate for Dispmode
    eVGAMaxRateForDispmode,

    // VGA Wakeup setting by the external controller
    eVGAMaxHSyncFreqForExtCtrl,
    eVGAMinHSyncFreqForExtCtrl,
    eVGAMaxVSyncFreqForExtCtrl,
    eVGAMinVSyncFreqForExtCtrl,

    // Quality customization
    eAdpBacklightAplDark,           // APL lower bound for auto backlight.
    eAdpBacklightAplBright,         // APL higher bound for auto backlight.
    eFlagBind2DNRWithMpegNR,        // Bind 2D NR With Mpeg NR
    eFlagAutoNR,                    // Auto NR enable toggle.
    eFlagUIQTYLinearMap,            // Use Linear Calculation instead of Using Min,Def,Max Directly
    eFlagBindContrastSaturation,    // Bind Contrast and Saturation
    eFlagFlashPqEnable,             // FlashPQ enable toggle.
    eFlashPqUseNandFalsh,           // FlashPQ base partition for NAND flash
    eFlashPqBasePartition,          // FlashPQ base partition for NAND flash
    eFlashPqBaseAddress,            // FlashPQ base address, 0x28??????
    eFlashPqOffset,                 // FlashPQ additional offset address.
    eFlashPqBlockSize,              // FlashPQ block size, unit: 64KB.
    eFlagFlashPqEnableGamma,        // FlashPQ load gamma from image.
    eFlagFlashPqEnableSCE,          // FlashPQ load SCE from image.
    eFlagFlashPqEnableQty,          // FlashPQ load Qty table from image.
    eFlagFlashPqEnableQtyMaxMin,    // FlashPQ load min/max table from image.
    eFlagFlashPqEnableQtySmartPic,  // FlashPQ load smartPic table from image.
	eFlagFlashPqQtySmartPicNewMode,	// FlashPQ smartpic new mode
    eFlagFlashPqSmartPicOffset,     // FlashPQ additional image offset for smartPic.

    eFlagFlashPqSCEMax,             // FlashPQ Use Base and Max to Calculate SCE
    eFlagFlashPqSCEMin,             // FlashPQ Use Base and Min to Calculate SCE

    eFlagFlashPanelEnable,

    eFlashPq2ndpartBasePartition,           // FlashPQ base partition for NAND flash
    eFlashPq2ndpartBaseAddress,             // FlashPQ base address, 0x28??????
    eFlashPq2ndpartOffset,                  // FlashPQ additional offset address.
    eFlashPq2ndpartBlockSize,               // FlashPQ block size, unit: 64KB.
    eFlagFlashPq2ndpartEnableGamma,         // FlashPQ load gamma from image.
    eFlagFlashPq2ndpartEnableSCE,           // FlashPQ load SCE from image.
    eFlagFlashPq2ndpartEnableQty,           // FlashPQ load Qty table from image.
    eFlagFlashPq2ndpartEnableQtyMaxMin,     // FlashPQ load min/max table from image.
    eFlagFlashPq2ndpartEnableQtySmartPic,   // FlashPQ load smartPic table from image.
    eFlagFlashPq2ndpartQtySmartPicNewMode,  // FlashPQ smartpic new mode
    
    eFlagLinkM4ContrastSaturation,          // Link matrix4 saturation with contrast.
    eFlagGain1AsColorTemp,
    eFlagVideoLvlExtPosition,       // 0: RGB gain1 as video level extend; RGB gain2 as color temp.
                                    // 1: Matrix4 as video level extend; RGB gain1 as color temp.
                                    // 2: Matrix4 as video level extend; RGB gain2 as color temp.
                                    
    eBrightInterval,        // Brightness Register Value = 0x400 + (Dummy - 0x80) << (BIRGHT_INTERVAL)
    eHueInterval,           // SCE Hue Register Value = 0x200 + (Dummy - 0x80) << (HUE_INTERVAL)
    eFlagUseMatrixPQAdj,    // Use OS matrix for global Cont/Bri/Sat/Hue adjustment
    eFlagxvYCCClip,         // Clip to RGB unsign 10 bit after xvYCC
    eOSOptionPos,           // Set OS RGB Gain/Gamma position
    eMatrixHDYCbCr709,      // Use YCbCr709 color space in video path for HD timing
    eSupportHdmiYCbCr444,   // Support YCbCr444 format in video path for HDMI input
    eSupportVgaYCbCr444,    // Support YCbCr444 format in video path for VGA input
    eShareBnrHwCfg,         // Use HW BNR setting for BNR FW

    eNVMGamma,              // Read gamma from EEPROM
    eCustSetGammaTopPoint,  // Costomization of Gamma toppest point
    eFlagControlBacklightPWM,
    eFlagSupportOD,

    // RTC external H/W Option
    eRtcHw,
    eRtcExtIntGpio,
    eRtcExtClkDiv,

    // SCART FS Status Servo ADC
    eScart1FSServoAdc,
    eScart2FSServoAdc,

    // CEC
    eCECFunctionPinmuxEn,

    // HDMI
    eHDMISupportExtSwitch,
    eHDMIBypassInitialFlow,
    eHDMIOffOnMuteCount,
    eDVIWaitStableCount,
    eDVIWaitNosignalCount,
    eDVIWaitSCDTStableCount,
    eHDMIEQZEROVALUE,   // josh
    eHDMIEQBOOSTVALUE,  // josh
    eHDMIEQSELVALUE,
    eHDMIEQGAINVALUE,
    eHDMILBWVALUE,
    eHDMIHDCPMASK1,
    eHDMIHDCPMASK2,
    eHDMIEQ_LOW1_KPGAIN,
    eHDMIEQ_LOW1_RXMODE,
    eHDMIEQ_LOW1_EQGAIN,
    eHDMIEQ_LOW1_ZSEL,
    eHDMIEQ_LOW1_VREF,
    eHDMIEQ_LOW2_KPGAIN,
    eHDMIEQ_LOW2_RXMODE,
    eHDMIEQ_LOW2_EQGAIN,
    eHDMIEQ_LOW2_ZSEL,
    eHDMIEQ_LOW2_VREF,
    eHDMIEQ_MIDDLE_KPGAIN,
    eHDMIEQ_MIDDLE_RXMODE,
    eHDMIEQ_MIDDLE_EQGAIN,
    eHDMIEQ_MIDDLE_ZSEL,
    eHDMIEQ_MIDDLE_VREF,
    eHDMIEQ_HIGH_KPGAIN,
    eHDMIEQ_HIGH_RXMODE,
    eHDMIEQ_HIGH_EQGAIN,
    eHDMIEQ_HIGH_ZSEL,
    eHDMIEQ_HIGH_VREF,

    // Run on NOR flash or not
    eFlagRunOnNor,
	eHDMIPortMappingHDMI1,//fll
    eHDMIPortMappingHDMI2,//fll
    eHDMIPortMappingHDMI3,//fll
    eHDMIPortMappingHDMI4,

    // TVD
    eTVDSupportNTSCOnly,
    eTVDSupportPedEn,
    eTVDPalNtscMixAutosearch,
    eTVDLLockFreeRun,
    eTVDCCNRLThresholdBlock,    //block CC if NRL beyong this,  (range: 0~80)
    eTVDCCNRLThresholdUnblock,  //unblock CC if NRL below this, (range: 0~eTVDCCNRLThresholdBlock)
    eTVDCCPERThresholdBlock,    //block CC if PER beyong this,  (range: 0~128)
    eTVDCCPERThresholdUnblock,  //unblock CC if PER below this, (range: 0~eTVDCCPERThresholdBlock)
    eTVDSupportOutput216,
    eTVDSupportEFuseCVBS,
    eTVDSupportEFuseCVBSOffset,
    eTVDUseVPres4,
    eTVDScan_Diff_Min_TH,                   //Auto Search Diff Min TH
    eTVDMenu_Av_Delay_PAL_N,                //PAL_N H Porch Value
    eTVDMenu_Av_Delay_PAL,                  //PAL H Porch Value
    eTVDMenu_Av_Delay_PAL_M,                //PAL_M H Porch Value
    eTVDMenu_Av_Delay_NTSC358,              //NTSC358 H Porch Value
    eTVDMenu_Av_Delay_SECAM,                //SECAM H Porch Value
    eTVDMenu_Av_Delay_PAL_60,               //PAL_60 H Porch Value
    eTVDMenu_Av_Delay_NTSC443,              //NTSC443 H Porch Value
    eTVDMenu_Av_Delay_SV_CS_UNSTABLE,       //SV_CS_UNSTABLE H Porch Value
    eTVDMenu_Av_Delay_SHNA_PAL_N,           //SHNA PAL_N H Porch Value
    eTVDMenu_Av_Delay_SHNA_PAL,             //SHNA PAL H Porch Value
    eTVDMenu_Av_Delay_SHNA_PAL_M,           //SHNA PAL_M H Porch Value
    eTVDMenu_Av_Delay_SHNA_NTSC358,         //SHNA NTSC358 H Porch Value
    eTVDMenu_Av_Delay_SHNA_SECAM,           //SHNA SECAM H Porch Value
    eTVDMenu_Av_Delay_SHNA_PAL_60,          //SHNA PAL_60 H Porch Value
    eTVDMenu_Av_Delay_SHNA_NTSC443,         //SHNA NTSC443 H Porch Value
    eTVDMenu_Av_Delay_SHNA_SV_CS_UNSTABLE,  //SHNA SV_CS_UNSTABLE H Porch Value
    eTVDMenu_Av_V_Delay,                    //V Porch Value
    eTVDTgt_Sync_Height_625,                //PAL Sync Height
    eTVDTgt_Sync_Height_525,                //NTSC Sync Height
    eTVDPAL_Ip_Tgt_A,
    eTVDPAL_Ip_Tgt_B,
    eTVDNTSC_Ip_Tgt_A,
    eTVDNTSC_Ip_Tgt_B,
    eTVDNTSC_J_Ip_Tgt_A,
    eTVDNTSC_J_Ip_Tgt_B,
    eTVDSECAM_Ip_Tgt_A,
    eTVDSECAM_Ip_Tgt_B,
    eTVD_BstLock_Thr1_LN_525,  //Low NA burst lock --> unlock
    eTVD_BstLock_Thr2_LN_525,  //Low NA burst unlock --> lock
    eTVD_BstLock_Thr1_HN_525,  //High&Super NA burst lock --> unlock
    eTVD_BstLock_Thr2_HN_525,  //High&Super NA burst unlock --> lock
    eTVD_BstLock_Thr1_LN_625,  //Low NA burst lock --> unlock
    eTVD_BstLock_Thr2_LN_625,  //Low NA burst unlock --> lock
    eTVD_BstLock_Thr1_HN_625,  //High&Super NA burst lock --> unlock
    eTVD_BstLock_Thr2_HN_625,  //High&Super NA burst unlock --> lock
    eTVD_BstLock_Thr1_PAL_N,   //High&Super NA burst lock --> unlock
    eTVD_BstLock_Thr2_PAL_N,   //High&Super NA burst unlock --> lock
    eTVD_SCkill_UP,            //Secam CKILL UP TH
    eTVD_SCkill_DN,            //Secam CKILL DN TH

    // SCART
    eSCART1FBInSelect,
    eSCART2FBInSelect,
    eScart1Type,
    eScart2Type,
    eScart1IntScCh,
    eScart2IntScCh,

    // MT8295
    eMT8295ResetGpio,
    eMT8295DrivingCurrent,
    eMT8295TSDrivingCurrent,
    eMT8295PBIDrivingCurrent,
    eCI_PMX_CFG,
    eMT8295IntGpio,
    eCI5VPowerOnGpio,
    eCI5VPowerOnPolarity,
    eCI5VOcGpio,
    eCI5VOcPolarity,
    eCISysClock,
    eCITsClock,
    eCISpiClock,

    // Smart Card
    eSMCHotplugGpio,
    eSMCHotplugGpioPolarity,

    // UART
    eUART1Support,
    eUART2Support,

    // VGA AD Spec
    eVFE_HW_SPECIAL_TARGET,
    eVFE_HW_YPbPrMapToVGA,
    eVGAADSPECSupport,
    eVFE_HW_075ohm,
    eYPBPR_ADC_SUPPORT_120,
    eDoAutoPhaseAfterAutoColor,
    eSCART_DO_AUTOCOLOR,                 //MC20081115 add for SCART
    eEFuseAutocolor,
    eAUTOCOLOR_FAIL_SHOW_PATTERN,
#ifdef CC_YPBPR_ADAPTIVE_SLICER_REDUCE_TIME
    eYDetect_GPIO,
#endif

    // SCART RGB
    eSCARTRGBOrder,
    eSCARTPin8LowThres,
    eSCARTPin8HiThres,

    // ON_CHIP_AUTO_COLOR
    eOnChipAutoColor,
    eMtkPublicVersion,
    eOnChipYPbPrCounterAddr,
    eOnChipVGACounterAddr,
    eOnChipOffsetCounter,

    // Notification function
    eNotifyMuteFunc,
    eNotifyMixerVideoLayerFunc,
    eNotifyMixerFunc,
    eNotifyVideoPropertyFunc,
    eNotifyVideoAutoDoneFunc,
    eNotifyFEColorSystem,
    eNotifyFECombFilter,
    eNotifyFESoundSys,
    eCiCustomQueryFunc,
    eNotifyAFSoundProperties,
    eNotifyAFDRCAllowed,

    // Trustzone customization.
    eTrustzoneEnable,
    eTzCodeBase,
    eTzFlashPartition,

    // Linux memory usage related customization
    eLinuxKernelMemSize,
    eTotalMemSize,
    eFBMemSize,
    eDirectFBMemSize,
    e_config_osd_plane1_mem_size,
    e_config_osd_plane2_mem_size,
    e_config_osd_plane3_mem_size,
    e_config_osd_plane4_mem_size,
    e_config_osd_plane5_mem_size,
    e_config_osd_3d_plane1_mem_size,
    e_config_osd_3d_plane2_mem_size,
    e_config_osd_3d_plane3_mem_size,
    e_config_osd_3d_plane4_mem_size,
    e_config_osd_3d_plane5_mem_size,
    e_config_osd_dec_plane1_mem_size,
    e_config_osd_dec_plane2_mem_size,
    e_config_osd_dec_plane3_mem_size,
    e_config_osd_dec_plane4_mem_size,
    e_config_osd_dec_plane5_mem_size,
    e_config_gfx_temp_mem_size,
    eDmxSharedMemSize,
    eReservedMemSize,
    eTrustzoneMemSize,

    // system setting
    eCustomerPassword,
    eCustomerName,

    // SIF
    eSifEnableSclStretch,
    eSifDisableOpenDrain,
    eSifEnablePdwncMaster,
    eSifEnablePdwncMaster1,
    eSifEnable8295bMaster0,
    eSifEnable8295bMaster1,

    eSifCfgBeforeAction,
    eSifCfgAfterAction,
    eSifEnableVsyncHigh,
    eSifCfgPinSetFunc,
    eSifCfgGetConfigFunc,
    eSifcfgFixClkDiv,

    // SIF SCL stretch workaround
    eSifCfgTRIDelay,
    eSifCfgTRIDelayDevAddr,

    // For supporting slave device unable to handle by H/W engine
    eSifCfgSWBusId,
    eSifCfgSWSDA,
    eSifCfgSWSCL,
    eSifCfgNumOfSlaveUsingSW,
    eSifCfgArrayOfSlaveAddrUsingSW,

    // Support Function Query
    eIsSupportDivX,
    eIsSupportMP2,
    eIsSupportMPEG4,
    eIsSupportVC1,

    // Driver Logo
    eDrvLogoEnable,
    eDrvLogoWidth,
    eDrvLogoHeight,
    eDrvLogoColorMode,
    eDrvLogoBmpAddr,
    eDrvLogoPalAddr,
    eDrvLogoVdecEnable,
    eDrvLogoVdecDefault,
    eDrvLogoVdecMode,
    eDrvLogoVdecDataAddr,
    eDrvLogoVdecDataSize,
    eDrvLogoVdecFlashAddr,
    eDrvLogoVdecFlashSize,
    eDrvLogoVdecFlashSlotNum,
    eDrvLogoVdecFlashDevId,
    eDrvLogoVdecTimeStartDelay, // to control sync with audio sound. Ex. 0 (ms)
    eDrvLogoVdecTimeFadeIn,     // alpha: 0 -> 255 duration. Ex. 1000 (ms)
    eDrvLogoVdecTimeDisplay,    // alpha: 255 duration. Ex. 1000 (ms)
    eDrvLogoVdecTimeFadeOut,    // alpha: 255 -> 0 duration. Ex. 1000 (ms)
    eDrvLogoEarlyReturnCounter,

    eTVESupportAFD,

    eCountryDefaultSetting,
    eCountryFromEEPROM,         // Save target country information in EEPROM
    eCountryEEPROMOffset1,
    eCountryEEPROMOffset2,
    eCountryEEPROMOffset3,
    eCustomerPowerOnSequence,
    eCustomerPowerOffSequence,

    eDDDSErrorLimit,
    eDDDSLockOnOthers,
    eFrameTrackMode,
    eFrameTrackStableCnt,
    ePowerStateQueryFunc,

    eDISP_RLVDSEncodeLink,
    eDmxFramerSet0PostiveEdge,
    eDmxFramerSet0ExternalSync,
    eDmxFramerSet0ParallelInput,
    eDmxFramerSet1PostiveEdge,
    eDmxFramerSet1ExternalSync,
    eDmxFramerSet1ParallelInput,
    eDmxFrontendConfigTable,
    eDmxScrambleDetectTable,

    // sunman_viq
    eAudioVIQRefLvl,
    eAudioVIQMode,
    eAudioVIQMaxGain,
    eAudioVIQInputGain,
    eAudioVIQOutputGain,
    eAudioVIQNoiseTh,
    
    // sunman_peq
    eAudioSRSPeqHpfFactory,
    eAudioSRSPEQInputGain,
    eAudioSRSPEQOutputGain,
    eAudioSRSPEQBands,
    eAudioMtkHpfFc,
    eAudioMtkHpf2Fc,
    
    // sunman_lmt
    eAudioSRSLmtInputGain,
    eAudioSRSLmtOutputGain,
    eAudioSRSLmtRefBoost,
    eAudioSRSLmtRefLevel,
    eAudioSRSLmtRefDelay,
    eAudioSRSLmtRefCoeff,
    eAudioSRSLmtRefThresh,

   // bofeng_aeq
	eAudioSRSAeqInputGain,
	eAudioSRSAeqOutputGain,
	eAudioSRSAeqFilterSize,

    // Gfx FB partition size
    eGfxFBPartitionSize,

    // 3D Enable GPIO
    e3DEnableGPIO,
    e3GlassResetGPIO,

    // PCIE reset gpio
    ePcieResetGpio,       // Low = reset, high = release.

    // CC_AUD_QUERY_VDO_INFO
    eAudioVdoDelayTableDtv,
    eAudioVdoDelayTableHdmi,
    eAudioExtraMute,

    eHidKbCountry,
    // HDMI HPD NUM
    eHDMIHPD0GPIONUM,
    eHDMIHPD1GPIONUM,
    eHDMIHPD2GPIONUM,
    eHDMIHPD3GPIONUM,

    eDisplayOffCtrlByAp,
    eDisplayOffCtrlByDriver,
    
#if defined (CC_ENABLE_MTK_MODEL_INDEX)
    eModelIndexOffset1,         // option
    eModelIndexOffset2,         // option
    eModelIndexOffset3,         // option
#endif

    eLastOne
} QUERY_TYPE_T;
#endif /* defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER) */

typedef enum _LVDS_MODE
{
    eLvdsVidNormal        = 0,      // (mode 0) Video normal mode
    eLvdsVidYuv422OnePort = 1,      // (mode 1) Video YUV 422 one-channel
    eLvdsVidRgb444OnePort = 2,      // (mode 2) Video RGB 444 one-channel
    eLvdsVidYuv422TwoPort = 3,      // (mode 3) Video YUV 422 two-channel
    eLvdsVidRgb444TwoPort = 4,      // (mode 4) Video RGB 444 two-channel
    eLvdsVidYuv422OnePort8Bit = 5,  // (mode 5) Video YUV 422 one-channel
    eLvdsVidRgb444OnePort8Bit = 6,  // (mode 6) Video RGB 444 one-channel
    eLvdsVidYuv422TwoPort8Bit = 7   // (mode 7) Video YUV 422 two-channel
} LVDS_MODE_T;

typedef enum _LVDS_COLOR_DOMAIN
{
    eColorDomainYUV = 1,
    eColorDomainRGB = 2
} LVDS_COLOR_DOMAIN_T;

typedef enum
{
    VDP_ARG_BACKLT_UI,
    VDP_ARG_BACKLT_DBL,
    VDP_ARG_BACKLT_LIGHT_SNSR,
    VDP_ARG_BACKLT_OPC,
    VDP_ARG_BACKLT_SYSTEM_SET,
    VDP_ARG_BACKLT_MAX
} BACKLIGHT_SRC_INDEX;

//-----------------------------------------------------------------------------
// Dram enum
//-----------------------------------------------------------------------------
enum
{
    COL_ADDR_MIN = 7,
    COL_ADDR_BIT_8,     ///< Dram chip column address 8 bit setting
    COL_ADDR_BIT_9,     ///< Dram chip column address 9 bit setting
    COL_ADDR_BIT_10,    ///< Dram chip column address 10 bit setting
    COL_ADDR_BIT_11,    ///< Dram chip column address 10 bit setting
    COL_ADDR_MAX
};

// DDR type definitions
#define DDR_MIN_VAL     	0
#define SDR_x1              1
#define DDR_I_x1            2
#define DDR_I_x2            3
#define DDR_II_x1          	4
#define DDR_II_x2           5
#define DDR_II_x3           6
#define DDR_II_x4			7
#define DDR_III_x1         	8
#define DDR_III_x2          9
#define DDR_III_x3          10
#define DDR_III_x4 			11
#define DDR_MAX_VAL     	12

enum
{
    LVDS_A0 = 0,
    LVDS_A1 = 1,
    LVDS_A2 = 2,
    LVDS_A3 = 3,
    LVDS_A4 = 4,
    LVDS_A5 = 5,
    LVDS_A6 = 6,
    LVDS_A7 = 7,
    LVDS_A8 = 8,
    LVDS_A9 = 9,
    LVDS_ACLK1 = 0xa,
    LVDS_ACLK2 = 0xb,
    LVDS_DEFAULT = 0xf
};


//-----------------------------------------------------------------------------
// Loader Misc
//-----------------------------------------------------------------------------
typedef void (* PFN_LDR_ENVINIT_FUNC)(void *prLdrData, UINT32 fgValidFlag);
typedef void (* PFN_GPIO_TIMERINIT_FUNC)(void);

// customization enter stnadby or not
typedef UINT32 (* PFN_LDR_StandbyCheck_FUNC)(void);
#define DEFINED_PFN_LDR_StandbyCheck_FUNC

// customization show loader logo or not
typedef UINT32 (* PFN_LDR_PanelLogoCheck_FUNC)(void);
#define DEFINED_PFN_LDR_PanelLogoCheck_FUNC

typedef UINT32 (* PFN_LDR_USB_UpgradeCheck_FUNC)(void);
#define DEFINED_PFN_LDR_USB_UpgradeCheck_FUNC

//-----------------------------------------------------------------------------
// GPIO & ServoADC pins
//-----------------------------------------------------------------------------

// Total number definitions
#define TOTAL_SERVOADC_NUMBER       (9)
#define TOTAL_GPIO_NUMBER           (200)
#define TOTAL_MT8292_GPIO_NUMBER    (10)
#define TOTAL_MT8295_GPIO_NUMBER    (15)
#define TOTAL_OPCTRL_NUMBER         (66)

// ServoADC
#define MAX_SERVOADC_NUM            TOTAL_SERVOADC_NUMBER

// GPIO alias
#define GPIO_OPWM0              (86)
#define GPIO_OPWM1              (87)
#define GPIO_OPWM2              (88)
#define GPIO(x)                 (x)
#define ADIN(x)                 (SERVO(x))
#define SERVO(x)                (SERVO0 + (x))
#define OPCTRL(x)               (OPCTRL0 + (x))
#define MT8292_GPIO(x)          (MT8292_GPIO0 + (x))
#define MT8295_GPIO(x)          (MT8295_GPIO0 + (x))
#define MT5135_GPIO(x)          (MT5135_GPIO0 + (x))
//#define GPIO_LVDS_PWR_ON      (79)
//#define GPIO_USB_PWR_EN0      (46)
//#define GPIO_USB_PWR_ERR0     (47)
#define GPIO_HDMI_HPD0          (217)
#define GPIO_HDMI_HPD1          (221)
#define GPIO_HDMI_HPD2          (225)
//#define GPIO_LVDS_CTRL1       (80)
//#define GPIO_LVDS_CTRL2       (81)

#define GPIO_JTDO               (125)
#define GPIO_JTRST_             (129)

// PDWNC PWR GPIO 0~7
#define PWR_GPIO0               (200)           ///< define gpio macro for OPCTRL0
#define PWR_GPIO1               (201)           ///< define gpio macro for OPCTRL1
#define PWR_GPIO2               (202)           ///< define gpio macro for OPCTRL2
#define PWR_GPIO3               (203)           ///< define gpio macro for OPCTRL3
#define PWR_GPIO4               (204)           ///< define gpio macro for OPCTRL4
#define PWR_GPIO5               (205)           ///< define gpio macro for OPCTRL5
#define PWR_GPIO6               (206)           ///< define gpio macro for OPCTRL6
#define PWR_GPIO7               (207)           ///< define gpio macro for OPCTRL7
#define PWR_GPIO8               (208)           ///< define gpio macro for OPCTRL8
#define PWR_GPIO9               (209)           ///< define gpio macro for OPCTRL9
#define PWR_GPIO10              (210)           ///< define gpio macro for OPCTRL10
#define PWR_GPIO11              (211)           ///< define gpio macro for OPCTRL11
#define PWR_GPIO12              (212)           ///< define gpio macro for OPCTRL12
#define PWR_GPIO13              (213)           ///< define gpio macro for OPCTRL13
#define PWR_GPIO14              (214)           ///< define gpio macro for OPCTRL14
#define PWR_GPIO15              (215)           ///< define gpio macro for OPCTRL15

// PDWNC OPCTRL 0~7
#define OPCTRL0                 (200)           ///< define gpio macro for OPCTRL0
#define OPCTRL1                 (201)           ///< define gpio macro for OPCTRL1
#define OPCTRL2                 (202)           ///< define gpio macro for OPCTRL2
#define OPCTRL3                 (203)           ///< define gpio macro for OPCTRL3
#define OPCTRL4                 (204)           ///< define gpio macro for OPCTRL4
#define OPCTRL5                 (205)           ///< define gpio macro for OPCTRL5
#define OPCTRL6                 (206)           ///< define gpio macro for OPCTRL6
#define OPCTRL7                 (207)           ///< define gpio macro for OPCTRL7
#define OPCTRL8                 (208)           ///< define gpio macro for OPCTRL8
#define OPCTRL9                 (209)           ///< define gpio macro for OPCTRL9
#define OPCTRL10                (210)           ///< define gpio macro for OPCTRL10
#define OPCTRL11                (211)           ///< define gpio macro for OPCTRL11
#define OPCTRL12                (212)           ///< define gpio macro for OPCTRL12
#define OPCTRL13                (213)           ///< define gpio macro for OPCTRL13
#define OPCTRL14                (214)           ///< define gpio macro for OPCTRL14
#define OPCTRL15                (215)           ///< define gpio macro for OPCTRL15

#define SERVO_0_ALIAS           (400)

#define SERVO0                  (400)
#define SERVO1                  (401)
#define SERVO2                  (402)
#define SERVO3                  (403)
#define SERVO4                  (404)
#define SERVO5                  (405)
#define SERVO6                  (406)
#define SERVO7                  (407)

//#define SERVO_GPIO0             (227)	//not real gpio, just for judge ather sevoad_gpio function
#define SERVO_GPIO1             (228)
#define SERVO_GPIO2             (229)
#define SERVO_GPIO3             (230)
#define SERVO_GPIO4             (231)
#define SERVO_GPIO5             (232)
#define SERVO_GPIO6             (233)
#define SERVO_GPIO7             (234)

#define ADIN0                   SERVO(0)        ///< define ADIN0
#define ADIN1                   SERVO(1)        ///< define ADIN1
#define ADIN2                   SERVO(2)        ///< define ADIN2
#define ADIN3                   SERVO(3)        ///< define ADIN3
#define ADIN4                   SERVO(4)        ///< define ADIN4
#define ADIN5                   SERVO(5)        ///< define ADIN5
#define ADIN6                   SERVO(6)        ///< define ADIN6
#define ADIN7                   SERVO(7)        ///< define ADIN7

#define MT5135_GPIO0    (700)
#define MT5135_GPIO1    (701)
#define MT5135_GPIO2    (702)
#define MT5135_GPIO3    (703)
#define MT5135_GPIO4    (704)
#define MT5135_GPIO5    (705)
#define MT5135_GPIO6    (706)
#define MT5135_GPIO7    (707)
#define MT5135_GPIO8    (708)
#define MT5135_GPIO9    (709)
#define MT5135_GPIO10   (710)
#define MT5135_GPIO11   (711)
#define MT5135_GPIO12   (712)
#define MT5135_GPIO13   (713)
#define MT5135_GPIO14   (714)
#define MT5135_GPIO15   (715)
#define MT5135_GPIO16   (716)
#define MT5135_GPIO17   (717)
#define MT5135_GPIO18   (718)
#define MT5135_GPIO30   (730)
#define MT5135_GPIO_MAX MT5135_GPIO30

#define WT6720_GPIO0    (800)
#define WT6720_GPIO1    (801)
#define WT6720_GPIO2    (802)
#define WT6720_GPIO3    (803)
#define WT6720_GPIO4    (804)
#define WT6720_GPIO5    (805)
#define WT6720_GPIO6    (806)
#define WT6720_GPIO7    (807)
#define WT6720_GPIO8    (808)
#define WT6720_GPIOB    WT6720_GPIO8
#define WT6720_GPIO_MAX WT6720_GPIO8

#define MT8292_GPIO0    (600)
#define MT8292_GPIO1    (601)
#define MT8292_GPIO2    (602)
#define MT8292_GPIO3    (603)
#define MT8292_GPIO4    (604)
#define MT8292_GPIO5    (605)
#define MT8292_GPIO6    (606)
#define MT8292_GPIO7    (607)
#define MT8292_GPIO8    (608)
#define MT8292_GPIO9    (609)

// MT8295
#define MT8295_GPIO0    (900)
#define MT8295_GPIO1    (901)
#define MT8295_GPIO2    (902)
#define MT8295_GPIO3    (903)
#define MT8295_GPIO4    (904)
#define MT8295_GPIO5    (905)
#define MT8295_GPIO6    (906)
#define MT8295_GPIO7    (907)
#define MT8295_GPIO8    (908)
#define MT8295_GPIO9    (909)
#define MT8295_GPIO10   (910)
#define MT8295_GPIO11   (911)
#define MT8295_GPIO12   (912)
#define MT8295_GPIO13   (913)
#define MT8295_GPIO14   (914)
#define MT8295_GPIO_MAX MT8295_GPIO14

#define GPIO_INIT_CUSTOM_MODE0      (0)     // normal mode
#define GPIO_INIT_CUSTOM_MODE1      (1)     // special panel mode
#define GPIO_INIT_CUSTOM_MODE2      (2)     // panel mode
#define GPIO_INIT_CUSTOM_MODE3      (3)     // sdm mode
#define GPIO_INIT_CUSTOM_MASK       (0xfU)
#define GET_INIT_CUSTOM_MODE()      (DRVCUST_GetBootupMode() & GPIO_INIT_CUSTOM_MASK)
#define GPIO_INIT_COMPANION_MASK    (0xffff0000)
#define GPIO_INIT_COMPANION_CHIP    (0x82800000)
#define IS_COMPANION_CHIP_ON()      ((DRVCUST_GetBootupMode() & GPIO_INIT_COMPANION_MASK) == GPIO_INIT_COMPANION_CHIP)

typedef enum
{
    eLedZero,
    eLedLoaderPowerOn,
    eLedLoaderPowerOff,
    eLedSystemPowerOn,
    eLedSystemPowerOff,
    eLedDarkLed,
    eLedIrOn,
    eLedIrOff,
    eLedVgaStandby,
    eLedUpgradeBlink,
    eLedDetectBlink,
    eLedUpgradeFail,
    eLedUpgradeFinish,
    eLedLast
} LED_STATUS_T;

#define INTERNAL_PODCI  (0)     //mt5387 pcmcia pinmux
#define MT5387_NAND_PMX (7)     //mt5391 nand i/f pinmux
#define MT8295_IO_0mA   (0)
#define MT8295_IO_4mA   (4)
#define MT8295_IO_8mA   (8)
#define MT8295_IO_12mA  (12)

//-----------------------------------------------------------------------------
// Driver Key Pad data structure
//-----------------------------------------------------------------------------

#define DKBIT_POWER         (1U << 0)       ///< Driver internal power button id
#define DKBIT_MENU          (1U << 1)       ///< Driver internal menu button id
#define DKBIT_INPUT_SRC     (1U << 2)       ///< Driver internal input soruce button id
#define DKBIT_PRG_UP        (1U << 3)       ///< Driver internal channel up button id
#define DKBIT_PRG_DOWN      (1U << 4)       ///< Driver internal channel down button id
#define DKBIT_VOL_UP        (1U << 5)       ///< Driver internal volume up button id
#define DKBIT_VOL_DOWN      (1U << 6)       ///< Driver internal volume down button id
#define DKBIT_EJECT         (1U << 7)       ///< Driver internal dvd eject button id
#define DKBIT_PLAY_PAUSE    (1U << 8)       ///< Driver internal dvd play/pause button id
#define DKBIT_STOP          (1U << 9)       ///< Driver internal dvd stop button id
#define DKBIT_FACTORY       (1U << 10)      ///< Driver internal factory button id
#define DKBIT_ENTER         (1U << 11)      ///< Driver internal enter button id
#define DKBIT_EXIT          (1U << 12)      ///< Driver internal enter button id
#define DKBIT_NONE          (0xffffffff)    ///< Driver internal button id for none

typedef struct _MultiKey 
{
    UINT32 u4CrystalKey;    ///< System internal key id.
    UINT32 u4DrvKeyBits;    ///< Driver internal button id.
} MULTI_KEY_T;

typedef struct _CombIrKp 
{
    UINT32 u4CrystalKey;    ///< System internal key id.
    UINT32 u4IrBtn;         ///< Driver internal button id.
    UINT32 u4KeyPadBtn;     ///< Driver internal button id.
} COMB_IR_KP_T;

//-----------------------------------------------------------------------------
// GPIO Key Pad data structure
//-----------------------------------------------------------------------------
typedef struct _SingleGpio 
{
    UINT32 u4Gpio;          ///< Gpio number
    UINT32 u4Val;           ///< Gpio polarity
    UINT32 u4Key;           ///< Gpio pin/polarity for System internal key id.
} SGL_GPIO_T;

//-----------------------------------------------------------------------------
// ServoADC Key Pad data structure
//-----------------------------------------------------------------------------
typedef struct _SingleServo 
{
    UINT32 u4Servo;         ///< ServoADC pin
    UINT32 u4Top;           ///< ServoADC top value
    UINT32 u4Btm;           ///< ServoADC button value
    UINT32 u4Key;           ///< ServoADC pin/range for System internal key id.
} SGL_SERVO_T;

//-----------------------------------------------------------------------------
// Greed GPIO Key Pad data structure
//-----------------------------------------------------------------------------
typedef struct _GreedKey 
{
    UINT32 u4GpioOut;       ///< Greed GPIO output pin
    UINT32 u4Val;           ///< Greed GPIO polarity
    UINT32 u4GpioIn;        ///< Greed GPIO input pin
    UINT32 u4Key;           ///< System key for this Greed GPIO setting.
} GREED_KEY_T;

///< GPIO power function pointer type.
typedef void (* PFN_GPIO_POWER_FUNC)(void);
///< GPIO setup at power on stage function pointer type.
typedef void (* PFN_GPIO_POWERON_FUNC)(void);
///< GPIO setup at power off stage function pointer type.
typedef void (* PFN_GPIO_POWEROFF_FUNC)(UINT32);
///< GPIO setup at led status function pointer type.
typedef void (* PFN_GPIO_LEDCTRL_FUNC)(LED_STATUS_T);
///< GPIO external control function by customization.
typedef INT32 (* PFN_CUSTOM_EXT_CTRL_FUNC)(VOID*, UINT32);

//-----------------------------------------------------------------------------
// Sif Config Before/After Action
//-----------------------------------------------------------------------------
#define SIF_MAX_NUM_BUS     7

typedef struct _SIF_CONFIG_T
{
    UINT8 u1NumBus;
    UINT8 au1HwVersion[SIF_MAX_NUM_BUS];
} SIF_CONFIG_T;

///< SIF config before action function pointer type.
typedef void (* PFN_SIF_CFG_BEFORE_ACTION_FUNC)(UINT8,UINT16, UINT8, UINT8 *, UINT32,UINT8 *, UINT32,UINT8);
///< < SIF config after action function pointer type.
typedef void (* PFN_SIF_CFG_AFTER_ACTION_FUNC)(UINT8,UINT16, UINT8, UINT8 *, UINT32,UINT8 *, UINT32,UINT8);
/// Pin mux configuration
typedef void (* PFN_SIF_CFG_PIN_SET_FUNC)(UINT8);
/// Configurations
typedef SIF_CONFIG_T* (* PFN_SIF_CFG_GET_CONFIG_FUNC)(void);

//-----------------------------------------------------------------------------
// Flash address
//-----------------------------------------------------------------------------
#define ADDR_NOR_FLASH      (0x28000000)
#define ADDR_NAND_FLASH     (0x90000000)

//-----------------------------------------------------------------------------
// Notification Function
//-----------------------------------------------------------------------------

#define NOTIFICATION_MUTE_ACTUALMUTECHANGED         0
#define NOTIFICATION_MUTE_ACTUALMUTECOLORCHANGED    1
#define NOTIFICATION_MUTE_BLANKEDCHANGED            2

typedef struct _NotifyMute 
{
    UINT32 u4NotifyId;
    UINT32 u4VdpId;
    UINT32 u4Data1;
    UINT32 u4Data2;
} NOTIFY_MUTE_T;

///< Mute status notification
typedef void (* PFN_NOTIFICATION_MUTE_FUNC)(NOTIFY_MUTE_T);


#define NOTIFICATION_MIXER_NEWPROPERTIES            0
#define NOTIFICATION_MIXER_NEWSETTINGSCOMPLETED     1
#define NOTIFICATION_MIXER_NEWSETTINGINTERRUPTED    2

typedef struct _NotifyMixer 
{
    UINT32 u4NotifyId;
    UINT32 u4VdpId;
    UINT32 u4Data1;
} NOTIFY_MIXER_VIDEOLAYER_T;

///< Mute status notification
typedef void (* PFN_NOTIFICATION_MIXER_VIDEOLAYER_FUNC)(NOTIFY_MIXER_VIDEOLAYER_T);

#define NOTIFICATION_VIDEO_SIGNALCHANGED                0
#define NOTIFICATION_VIDEO_VIDEOPRESENTCHANGED          1
#define NOTIFICATION_VIDEO_SIGNALPRESENTCHANGED         2
#define NOTIFICATION_VIDEO_FIELDFREQUENCYCHANGED        3
#define NOTIFICATION_VIDEO_NUMBEROFVISIBLELINESCHANGED  4
#define NOTIFICATION_VIDEO_NUMBEROFVISIBLEPIXELSCHANGED 5
#define NOTIFICATION_VIDEO_NUMBEROFFRAMELINESCHANGED    6
#define NOTIFICATION_VIDEO_INTERLACEDCHANGED            7
#define NOTIFICATION_VIDEO_IMAGEFORMATCHANGED           8
#define NOTIFICATION_VIDEO_FILMMODECHANGED              9
#define NOTIFICATION_VIDEO_FROMVCRCHANGED               10
#define NOTIFICATION_VIDEO_VIDEOCODINGCHANGED           11
#define NOTIFICATION_VIDEO_VIDEOPROTECTIONCHANGED       12

typedef struct _NotifyVideo 
{
    UINT32 u4NotifyId;
    UINT32 u4VdpId;
    UINT32 u4Data1;
    UINT32 u4Data2;
} NOTIFY_VIDEO_T;

///< vip status notification
typedef void (* PFN_NOTIFICATION_VIDEO_FUNC)(NOTIFY_VIDEO_T);


#define NOTIFICATION_VIDEO_AUTODONE         0
#define NOTIFICATION_VIDEO_AUTOCOLORDONE    1

typedef struct _NotifyVideoAutoDone 
{
    UINT32 u4NotifyId;
    UINT32 u4VdpId;
    UINT32 u4Data1;
} NOTIFY_VIDEO_AUTO_DONE_T;

///< video auto done notification
typedef void (* PFN_NOTIFICATION_VIDEO_AUTO_DONE_FUNC)(NOTIFY_VIDEO_AUTO_DONE_T);

typedef struct _CiCustomInfo_T 
{
    BOOL fgDisableCI;
} CiCustomInfo_T;
typedef void (* PFN_CI_CUSTOM_QUERY_FUNC)(CiCustomInfo_T *);

//-----------------------------------------------------------------------------
// IRRX Key Map data structure and Key Map array.
//-----------------------------------------------------------------------------

#define BTN_NONE                ((UINT32) 0xffffffff)

enum 
{
    IRRX_NEC_TYPE = 1,          ///< IR receiver NEC type setting
    IRRX_RC5_TYPE,              ///< IR receiver RC5 type setting
    IRRX_SHARP_TYPE,            ///< IR receiver SHARP type setting
    IRRX_FUNAI_FACTORY_TYPE,
    IRRX_USR_TYPE,              ///< IR receiver User define type.
    IRRX_END_TYPE
};

#define WAKEUP_KEY_TYPE_RC5     (1 << 0)
#define WAKEUP_KEY_TYPE_RC6     (1 << 1)
#define WAKEUP_KEY_TYPE_NEC     (1 << 2)
#define WAKEUP_KEY_TYPE_SHARP   (1 << 3)
#define WAKEUP_KEY_TYPE_RC5_RC6 ((1 << 0) | (1 << 1))

/*! type define for IR key unit.
 */
typedef struct _KeyMapUnit 
{
    UINT32  u4Key;              // Only Use 8bits.
    UINT32  u4Crystal;
} KEYUNIT_T;

/*! type define for key table with group id and key table size.
 */
typedef struct _KeyArray 
{
    UINT32  u4GroupId;
    UINT32  u4Size;
    const   KEYUNIT_T *prKeyMap;
} KEYARRAY_T;

/*! type define for IR wakeup key unit.
 */
typedef struct _WakeupKeyMapUnit 
{
    UINT32  u4Key;              // Only Use 8bits.
    UINT32  u4Crystal;
    UINT8   u1Type;
} WAKEUPKEYUNIT_T;

/*! type define for key table with group id and wakeup key table size.
 */
typedef struct _WakeupKeyArray 
{
    UINT32  u4GroupId;
    UINT32  u4Size;
    const   WAKEUPKEYUNIT_T *prKeyMap;
} WAKEUPKEYARRAY_T;

/* Definition of maximum raw data size */
#define IRRX_MAX_RAW_DATA_SIZE                  ((UINT8) 11) /* byte */

typedef struct _IRRX_RAW_DATA_T
{
    UINT8   u1Protocol;
    UINT32  u4Len;      /* byte */
    UINT8   au1Data[IRRX_MAX_RAW_DATA_SIZE];
} IRRX_RAW_DATA_T;

/*! function pointer for user define type.
 */
typedef UINT32 (* PFN_XFER_USR_TO_CRYSTAL)(UINT32 u4Into, const UINT8 *pu1Data, IRRX_RAW_DATA_T *prRaw);
typedef UINT32 (* PFN_IS_USR_LONG_PRESS)(UINT32 rDelta_MS, IRRX_RAW_DATA_T *rRaw, IRRX_RAW_DATA_T *rPrevRaw);
typedef UINT32 (*PFN_USR_GET_CRYSTAL_KEY)(UINT32 u4GroupId, UINT8 u1Data, UINT32 u4BitCnt);
typedef void (*PFN_IRRC_LED_BLINK_CONFIG_CHG)(BOOL bEnable);
/*! function type for user define wakeup function pointer.
 */
typedef void (* PFN_SET_USR_WAKEUP_KEY)(void);
typedef void (* PFN_USR_POWER_SEQUENCE)(void);

//-----------------------------------------------------------------------------
//PIGain table Modify by W.C Shih
//-----------------------------------------------------------------------------
typedef struct _PIUnit
{
    UINT8 LowerBound;
    UINT8 UpperBound;
    UINT8 PValue;
    UINT8 IValue;
} PIUNIT_T;


//-----------------------------------------------------------------------------
// VGA table structure
//-----------------------------------------------------------------------------
#ifndef _VGAMODE_STRUCT_
#define _VGAMODE_STRUCT_
typedef struct VGAMODE      // 14 bytes
{
    UINT16  IHF;            // Horizontal Frequency for timing search
    UINT8   IVF;            // Vertical Frequency for timing search
    UINT16  ICLK;           // Pixel Frequency
    UINT16  IHTOTAL;        // H Total
    UINT16  IVTOTAL;        // V Total
    UINT16  IPH_SYNCW;      // H Sync Width
    UINT16  IPH_WID;        // H Resolution
    UINT16  IPH_BP;         // H Back Porch
    UINT16  IPV_STA;        // V Back Porch + Sync Width
    UINT16  IPV_LEN;        // V Resolution
    UINT16  COMBINE;        // ??
    //UINT32 CLKIN_CWCODE;  // ??
}  VGAMODE ;
#endif

// non-linear drv_cust table
typedef struct
{
    UINT8   bDecType;
    UINT32  u4PanelW;
    UINT32  u4PanelH;
    UINT32  u4Width;
    UINT32  u4Height;
    UINT32  u4Interlace;
    /* Non Linear Factor */
    BOOL    bAuto;
    UINT32  u4Start;
    UINT32  u4Slope;
    UINT32  u4Middle;
    UINT32  u4Stop;
} NONLINEAR_PRM_T;

//-----------------------------------------------------------------------------
// Driver customization macro function for key map
//-----------------------------------------------------------------------------
#ifndef NDEBUG
    #define DBGLOG(STR...) Printf(STR)
#else
    #define DBGLOG(STR...)
#endif

/*! Define macro for IR RC5 driver customization.
 */
#define DECLARE_MAP_BEGIN(MAPFUNC) \
    UINT32 MAPFUNC(UINT32 u4GroupCode, UINT32 u4KeyCode) \
    { \
        UINT32 u4CrystalCode = (UINT32)0xffffffff; /* BTN_NONE */ \
        switch(u4GroupCode) \
        { \

    /* the line above is intendedly to be blanc */

#define DECLARE_MAP_END() \
     default: \
         DBGLOG("keymap : group none\n"); \
            break; \
        } \
        return u4CrystalCode; \
    } \

    /* the line above is intendedly to be blanc */

#define MAP_GROUP_AS_BELOW(GROUPCODE) \
    case (GROUPCODE): \

    /* the line above is intendedly to be blanc */

#define MAP_GROUP_BEGIN(GROUPCODE) \
    case (GROUPCODE): \
    { \
        switch (u4KeyCode) \
        { \

    /* the line above is intendedly to be blanc */

#define MAP_GROUP_END() \
        default: \
            DBGLOG("keymap : key none\n"); \
            return u4CrystalCode; \
        } \
    } \

    /* the line above is intendedly to be blanc */

#define MAP_FACTORY1_END()          \
        default:                    \
            if (u4KeyCode < 128)    \
            {                       \
                u4CrystalCode = BTN_DRIVER1_FACTORY(u4KeyCode); \
                DBGLOG("keymap : factory1 key %d\n", u4KeyCode); \
            }                       \
            else                    \
            {                       \
                DBGLOG("keymap : key none\n"); \
            }                       \
            break; \
        } \
    } \

    /* the line above is intendedly to be blanc */

#define MAP_FACTORY2_END() \
        default: \
            if (u4KeyCode < 128)    \
            {                       \
                u4CrystalCode = BTN_DRIVER2_FACTORY(u4KeyCode); \
                DBGLOG("keymap : factory2 key %d\n", u4KeyCode); \
            }                       \
            else                    \
            {                       \
                DBGLOG("keymap : key none\n"); \
            }                       \
            break; \
        } \
    } \

    /* the line above is intendedly to be blanc */


#define MAP_INSERT(KEYCODE, CRYSTALCODE, KEYDESCRIPTION) \
    case (KEYCODE) : \
        u4CrystalCode = (CRYSTALCODE); \
        DBGLOG("keymap : " KEYDESCRIPTION "\n"); \
        return u4CrystalCode; \

    /* the line above is intendedly to be blanc */

typedef enum
{
    RTC_INTERNAL = 0,
    RTC_EXT = 1
}   RTC_HW_T;

typedef void (* PFN_UPGRADE_BREAK_FUNC)(void);
typedef INT32 (* PFN_VER_CMP_FUNC)(CHAR *szCurVer, CHAR *szUpgVer);
typedef void (* PFN_DLM_CB_FUNC)(UINT32);
typedef void (* PFN_DLM_GET_TAG_VER_CB_FUNC)(UCHAR *ucTagName, CHAR *szCurVer);
typedef void (* PFN_DLM_SET_TAG_VER_CB_FUNC)(UCHAR *ucTagName, CHAR *szUpgVer);
typedef INT32 (* PFN_DLM_CMP_TAG_VER_CB_FUNC)(UCHAR *ucTagName, CHAR *szCurVer, CHAR *szUpgVer);
typedef UINT32 (* PFN_POWER_STATE_QUERY_FUNC)(void);
typedef BOOL (* PFN_CEC_CONFIG_QUERY_FUNC)(void);

//-----------------------------------------------------------------------------
// Driver customization interface function.
//-----------------------------------------------------------------------------

#if !defined(CC_NAND_BOOT) || !defined(CC_MTK_PRELOADER)

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB at boot stage.
 *
 *  @param  eQryType        The query type definition.
 *  @param  pu4Data         The pointer points to the setting data of the type.
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. There is no setting.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_InitQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB at boot stage.
 *
 *  @param  eQryType        The query type definition.
 *  @retval The value of the query type setting data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DRVCUST_InitGet(QUERY_TYPE_T eQryType);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB at boot stage.
 *
 *  @param  eQryType        The query type definition.
 *  @param  pu4Data         The pointer points to the setting data of the type.
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. There is no setting.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_InitQuery64(QUERY_TYPE_T eQryType, UINT64 *pu8Data);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB at boot stage.
 *
 *  @param  eQryType        The query type definition.
 *  @retval The value of the query type setting data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT64 DRVCUST_InitGet64(QUERY_TYPE_T eQryType);

#endif /* !defined(CC_NAND_BOOT) || !defined(CC_MTK_PRELOADER) */

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB.
 *
 *  @param  eQryType        The query type definition.
 *  @param  pu4Data         The pointer points to the setting data of the type.
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. There is no setting.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_OptQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB.
 *
 *  @param  eQryType        The query type definition.
 *  @retval The value of the query type setting data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DRVCUST_OptGet(QUERY_TYPE_T eQryType);

EXTERN NONLINEAR_PRM_T* DRVCUST_NonLinearTblGet(void);

EXTERN UINT32 DRVCUST_NonLinearTblSizeGet(void);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB.
 *
 *  @param  eQryType        The query type definition.
 *  @param  pu4Data         The pointer points to the setting data of the type.
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. There is no setting.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_PanelQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB.
 *
 *  @param  eQryType        The query type definition.
 *  @retval The value of the query type setting data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DRVCUST_PanelGet(QUERY_TYPE_T eQryType);

//-----------------------------------------------------------------------------
/** Set DrvInit Flag
 */
//-----------------------------------------------------------------------------
EXTERN void DRVCUST_SetDrvInit(UINT32 fgFlag);

//-----------------------------------------------------------------------------
/** Get DrvInit Flag
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DRVCUST_GetDrvInit(void);

//-----------------------------------------------------------------------------
/** Get Bootup mode
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_GetBootupMode(void);

//-----------------------------------------------------------------------------
/** Set DRVCUST Bootup Checking.
 */
//-----------------------------------------------------------------------------
EXTERN void DRVCUST_BootupCheck(void);

//-----------------------------------------------------------------------------
/** Set user defined LISR handler
 */
//-----------------------------------------------------------------------------
EXTERN void DRVCUST_UserLISR(INT32 i4Vector);

//-----------------------------------------------------------------------------
/** Query DRVCUST Logo setting.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_IsLogoDisableInEeprom(void);

//-----------------------------------------------------------------------------
/** Set GPIO For DCR
 */
//-----------------------------------------------------------------------------
EXTERN void DRVCUST_LDMHWInit(void);   //mtk70763, 2010-7-21

EXTERN void DRVCUST_HwInit(void);
EXTERN void DRVCUST_HUPSCaleCoeff(UINT8 bPath);
EXTERN void DRVCUST_VUPSCaleCoeff(UINT8 bPath);
EXTERN void DRVCUST_VideoInit(void);
EXTERN void DRVCUST_VdoModeChgDone(UINT8 bPath);
EXTERN void DRVCUST_VideoMainloop(void);
EXTERN void DRVCUST_OutVSyncISR(void);
EXTERN void DRVCUST_VdoInISR(void);
EXTERN void DRVCUST_PostOutVSyncISR(void);
EXTERN void DRVCUST_PostDEEndISR(void);
EXTERN UINT32 DRVCUST_OpCodeQuery(QUERY_TYPE_T eQryType);

//-----------------------------------------------------------------------------
/** Audio AMP related settings
 */
//-----------------------------------------------------------------------------
typedef enum
{
    FORMAT_RJ,
    FORMAT_LJ,
    FORMAT_I2S
} DATA_FORMAT_T;

typedef enum
{
    MCLK_128FS,
    MCLK_192FS,
    MCLK_256FS,
    MCLK_384FS,
    MCLK_512FS,
    MCLK_768FS
} MCLK_FREQUENCY_T;

typedef void (*AMP_INIT)(void);
typedef void (*AMP_MUTE)(BOOL fgMute);
typedef void (*AMP_SET_DAC_IN_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*AMP_SET_DAC_GAIN)(UINT8 u1HpId, INT16 i2Gain);
typedef void (*AMP_REG_WRITE)(UINT8 u1Offset, UINT8 u1Data);
typedef UINT8 (*AMP_REG_READ)(UINT8 u1Offset);
typedef void (*AMP_DISABLE_EFFCT)(void);

// driver call back
typedef struct
{
    AMP_DISABLE_EFFCT   pfnDisableEffect;
} AMP_DRV_CB_T;

/// Audio amplify function table.
typedef struct
{
    AMP_INIT           pfnInit;
    AMP_MUTE           pfnMute;
    AMP_SET_DAC_IN_FMT pfnSetDacInFmt;
    AMP_SET_DAC_GAIN   pfnSetDacGain;
    AMP_REG_WRITE      pfnRegWrite;
    AMP_REG_READ       pfnRegRead;
    AMP_DRV_CB_T      *prAmpDrvCBFunc;  // driver call back funciton table
} AMP_FUNCTBL_T;

/// Audio amplify configuration table.
typedef struct
{
    CHAR            szName[32];
    INT16           i2MaxGain;
    INT16           i2MinGain;
    UINT8           u1DacNum;
    AMP_FUNCTBL_T  *prAmpFunc;
} AMP_T;

EXTERN AMP_T *_AMP_GetAmpCustFunTbl(void);

#endif /* DRVCUST_IF_H */

