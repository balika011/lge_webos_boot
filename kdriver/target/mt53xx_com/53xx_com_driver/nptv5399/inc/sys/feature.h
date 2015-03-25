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
 * $Date  $
 * $RCSfile: feature.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _FEATURE_H
#define _FEATURE_H

#include "vendor.h"

/***************************************************************************
Define IC-related specific feature
****************************************************************************/

/* from config_video.h */

#if defined(CC_MT5391) || defined(CC_MT5387)
	#define SUPPORT_POP 1
	// For 8bit-8bit gamma, please define this in sys_build\customer\model.cfg file.
	#ifdef SUPPORT_8BIT_8BIT_GAMMA
		#define GAMMA_UPSAMPLE 0
	#else
		#define GAMMA_UPSAMPLE 1
	#endif
#else
	#define SUPPORT_POP 1
#endif
	#define SUPPORT_DVI 1
	#define SUPPORT_3DCOMB 1
	#define SUPPORT_TDN 1
	#define SUPPORT_GAMMA 1


/* following feature is enabled/tested in linux env,need to move to drvcust laters*/
#ifndef SUPPORT_ATV_SNOWSCREEN
#ifdef __KERNEL__
#define SUPPORT_ATV_SNOWSCREEN 1
#else
#define SUPPORT_ATV_SNOWSCREEN 1
#endif
#endif

#ifndef SUPPORT_DRAM_DUMP
#ifdef __KERNEL__
#define SUPPORT_DRAM_DUMP 1
#else
#define SUPPORT_DRAM_DUMP 0
#endif
#endif


#ifndef SUPPORT_DOT_TO_DOT_PATTERN
#define SUPPORT_DOT_TO_DOT_PATTERN 0
#else
#define SUPPORT_DOT_TO_DOT_PATTERN 0
#endif
#if defined(CC_MT5395)
#define SUPPORT_SBS_CONVERT 1
#else
#define SUPPORT_SBS_CONVERT 0
#endif

#ifndef SUPPORT_VGA_COLOR_MATRIX
#ifdef __KERNEL__
#define SUPPORT_VGA_COLOR_MATRIX 1
#else
#define SUPPORT_VGA_COLOR_MATRIX 0
#endif
#endif
#ifndef SUPPORT_VIDEO_CUSTOM_PARAM
#ifdef __KERNEL__
#define SUPPORT_VIDEO_CUSTOM_PARAM 1
#else
#define SUPPORT_VIDEO_CUSTOM_PARAM 0
#endif
#endif
#ifndef SUPPORT_SDAL_STYLE_PQ_SETTING
#ifdef __KERNEL__
#define SUPPORT_SDAL_STYLE_PQ_SETTING 1
#else
#define SUPPORT_SDAL_STYLE_PQ_SETTING 0
#endif
#endif

//VFE part
#define TVD_CLAMP_ON_BLANK        1
#define TVD_AUTO_DFT 		  1

//VGA part
//alway let SUPPORT_VGA_USERMODE ==1, feature now control by drvcust_if.h
#define SUPPORT_VGA_USERMODE 1

// PE part
#define SUPPORT_CINEMA_MODE	1
#define SUPPORT_SEAMLESS_MODE 1

//OSTG part
#define SUPPORT_FREERUN_DCLK_BY_STEP
#define SUPPORT_CW_CHANGE_BY_STEP 1
//#define SUPPORT_DDDS_STEP_TRACKING 1

 // PE bypass for testing.
#define BYPASS_PE_MDDI      1
#define BYPASS_PE_NR        1
#define BYPASS_PE_MLC       1

//Scaler Part
#define SUPPORT_EXTMJC_RGBTOYUV_OUTPUT 0

#define SUPPORT_HORIZONTAL_LBOX 1
#define SUPPORT_LUMACHROMA_CHECK 0

#define SUPPORT_MIB_3D_CONVERT_444_TO_422 0

//TVE part
#if (defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5882))
#define CC_TVE_SUPPORT_VDAC_SOFTWARE_CAL 0
#endif

#ifndef SUPPORT_CEC
#define SUPPORT_CEC 1
#endif

#ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
    #define CC_SUPPORT_OVERSAMPLE_1440 (1) /* porting, disable at first */
    #define CC_OVERSAMPLE_1440_CEN_DROP_PIXEL (0)
#else
    #define CC_SUPPORT_OVERSAMPLE_1440 (0) /* porting, disable at first */
    #define CC_OVERSAMPLE_1440_CEN_DROP_PIXEL (0)
    #define CC_SUPPORT_PRESCALE_DOWN
#endif

#define SUPPORT_VGA_422 (0) // Use YCbCr 422 color space for VGA input source.

// For MHEG5 DecodeOffset Performace Test
// Use old import protection mechanism to speed up import flow
//#ifdef CC_MHEG5_DECODE_OFFSET_PERFORMANCE 
//#define CC_FORCE_OLD_IMPORT     (3)   ==> already use import_sel =0
//#endif

// disable old pscan display mode (up-scaler no-fifo)
// #define CC_DISABLE_OLD_PSCAN_DISPMODE 1  

// Enable mute log when system boot up
#ifndef CC_BOOT_MUTE_LOG
#define CC_BOOT_MUTE_LOG (1)
#endif

#define CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP 1
#if CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
#define SCPIP_HCLIP_LENGTH 20
#define CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP 0
#else
#define CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP 1
#endif

#ifdef CC_MT5395
//#if !defined(CC_DISABLE_MIRROR_BY_MJC)
//#define CC_MJC_DO_ALL_MIRROR_FLIP
//#endif
#ifdef CC_SCPOS_3DTV_SUPPORT
//#define CC_SCALER_LR_SHIFT_WA
#endif
#endif


#if 0
#define MON_STA(module,item,value) \
    {static UINT32 item;UINT32 tmp;tmp=(value); \
        if(item!=tmp) \
            {LOG(2,"--"#module"--"#item" Change from %d to %d\n",(item),(tmp)); item = tmp;}\
    }
#else
    #define MON_STA(module,item,value)
#endif

#if defined(CC_MT5398)
//#define CC_SUPPORT_4K2K
#endif

#endif  /* End _FEATURE_H */

