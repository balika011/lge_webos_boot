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
 * $RCSfile: fbm_pool_config.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_pool_if.h
 *  API of FBM (Frame Buffer Manager)
 *  Details of file fbm_pool_if.h.
 */

#ifndef FBM_POOL_IF_H
#define FBM_POOL_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "fbm_drvif.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#ifdef CC_MEM_TOOL
#define __KERNEL__
#endif

#ifndef PAGE_SIZE
//#define PAGE_SIZE	4096
#endif

// -------- MDDI  --------

// MDDI Alignment
#define FBM_MDDI_ADDR_ALIGMENT                  (0x2000 - 1)       // 8K
#define FBM_MDDI_SIZE_ALIGMENT                  (0x4000 - 1)       // 8K

//Including RNR by multiply MV Flag by 3
#define FBM_MDDI_BI_FLAG_MV_SIZE(width, height)                     (((((((((width + 255) / 256) * 256) / 8 * height / 2) * 16 / 8) + 15) / 16) * 16) *3)
#define FBM_MDDI_BI_FLAG_LM_SIZE(width, height)                     ((((((((width + 255) / 256) * 256) / 8 * ((height / 2 + 3) / 4) * 4) * 16 / 8) + 15) / 16) * 16)

#define FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, height, yfld)          (((width + 255) / 256) * ((height / 2 * yfld + 7) / 8))
#define FBM_MDDI_BI_FLD_C422_MSB_SIZE(width, height, cfld)          (((width + 255) / 256) * ((height / 2 * cfld + 7) / 8))
#define FBM_MDDI_BI_FLD_C420_MSB_SIZE(width, height, cfld)          (((width + 255) / 256) * ((height / 2 * cfld / 2 + 7) / 8))

#if defined(CC_MT5398) || defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
#define FBM_MDDI_BI_FLD_LUMA_LSB_SIZE(width, height, yfld)          (((width + 255) / 256) * ((height / 2 * yfld + 255) / 256 *8))
#define FBM_MDDI_BI_FLD_C422_LSB_SIZE(width, height, cfld)          (((width + 255) / 256) * ((height / 2 * cfld + 255) / 256 *8))
#define FBM_MDDI_BI_FLD_C420_LSB_SIZE(width, height, cfld)          (((width + 255) / 256) * ((height / 2 * cfld / 2 + 255) / 256 * 8))
#else
#define FBM_MDDI_BI_FLD_LUMA_LSB_SIZE(width, height, yfld)          (((width + 255) / 256) * ((height / 2 * yfld + 31) / 32))
#define FBM_MDDI_BI_FLD_C422_LSB_SIZE(width, height, cfld)          (((width + 255) / 256) * ((height / 2 * cfld + 31) / 32))
#define FBM_MDDI_BI_FLD_C420_LSB_SIZE(width, height, cfld)          (((width + 255) / 256) * ((height / 2 * cfld / 2 + 31) / 32))
#endif

//Main Progressive
#define FBM_MDDI_NR_10BIT_SIZE(width, height, fld)                  ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_LUMA_LSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_C422_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_C422_LSB_SIZE(width, (height)*2, fld)  \
                                                                    + 16) * 2048) 

#define FBM_MDDI_BI_NR_10BIT_SIZE(width, height, fld)               ( FBM_MDDI_NR_10BIT_SIZE(width, height, fld) \
                                                                    + FBM_MDDI_BI_FLAG_MV_SIZE(width, height*2))

#define FBM_MDDI_NR_8BIT_SIZE(width, height, fld)                   ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_C422_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + 16) * 2048)

#define FBM_MDDI_BI_NR_8BIT_SIZE(width, height, fld)                ( FBM_MDDI_NR_8BIT_SIZE(width, height, fld)\
                                                                    + FBM_MDDI_BI_FLAG_MV_SIZE(width, height*2))


#define FBM_MDDI_NR_10BIT_420_SIZE(width, height, fld)              ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_LUMA_LSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_C420_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_C420_LSB_SIZE(width, (height)*2, fld)  \
                                                                    + 16) * 2048)


#define FBM_MDDI_BI_NR_10BIT_420_SIZE(width, height, fld)           (FBM_MDDI_NR_10BIT_420_SIZE(width, height, fld)\
                                                                    + FBM_MDDI_BI_FLAG_MV_SIZE(width, height*2))

#define FBM_MDDI_BI_NR_OFF_10BIT_420_SIZE(width, height, fld)       FBM_MDDI_NR_10BIT_420_SIZE(width, height, fld) 


#define FBM_MDDI_NR_8BIT_420_SIZE(width, height, fld)               ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_C420_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + 16) * 2048)

#define FBM_MDDI_BI_NR_8BIT_420_SIZE(width, height, fld)            (FBM_MDDI_NR_8BIT_420_SIZE(width, height, fld)\
                                                                     + FBM_MDDI_BI_FLAG_MV_SIZE(width, height*2))
                                                                   
#define FBM_MDDI_BI_NR_OFF_8BIT_420_SIZE(width, height, fld)        FBM_MDDI_NR_8BIT_420_SIZE(width, height, fld)

#define FBM_MDDI_BI_NR_10BIT_444_SIZE(width, height, fld)           ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + FBM_MDDI_BI_FLD_LUMA_LSB_SIZE(width, (height)*2, fld)  \
                                                                    + (FBM_MDDI_BI_FLD_C422_MSB_SIZE(width, (height)*2, fld) * 2)  \
                                                                    + (FBM_MDDI_BI_FLD_C422_LSB_SIZE(width, (height)*2, fld) * 2)  \
                                                                    + 16) * 2048)

#define FBM_MDDI_BI_NR_8BIT_444_SIZE(width, height, fld)            ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, (height)*2, fld)  \
                                                                    + (FBM_MDDI_BI_FLD_C422_MSB_SIZE(width, (height)*2, fld) * 2)  \
                                                                    + 16) * 2048)

//Main Interlace
#define FBM_MDDI_8BIT_422_SIZE(width, height, yfld, cfld)           ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, height, yfld)  \
                                                                    + FBM_MDDI_BI_FLD_C422_MSB_SIZE(width, height, cfld) + 16) * 2048)

#define FBM_MDDI_BI_8BIT_422_SIZE(width, height, yfld, cfld)        ( FBM_MDDI_8BIT_422_SIZE(width, height, yfld, cfld)\
                                                                    + FBM_MDDI_BI_FLAG_MV_SIZE(width, height)\
                                                                    + FBM_MDDI_BI_FLAG_LM_SIZE(width, height))

#define FBM_MDDI_10BIT_422_SIZE(width, height, yfld, cfld)          ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, height, yfld)  \
                                                                    + FBM_MDDI_BI_FLD_LUMA_LSB_SIZE(width, height, yfld)  \
                                                                    + FBM_MDDI_BI_FLD_C422_MSB_SIZE(width, height, cfld)  \
                                                                    + FBM_MDDI_BI_FLD_C422_LSB_SIZE(width, height, cfld) + 16) * 2048)

#define FBM_MDDI_BI_10BIT_422_SIZE(width, height, yfld, cfld)       (FBM_MDDI_10BIT_422_SIZE(width, height, yfld, cfld)\
                                                                    + FBM_MDDI_BI_FLAG_MV_SIZE(width, height)\
                                                                    + FBM_MDDI_BI_FLAG_LM_SIZE(width, height))
                                                                    
#define FBM_MDDI_10BIT_420_SIZE(width, height, yfld, cfld)          ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, height, yfld)  \
                                                                    + FBM_MDDI_BI_FLD_LUMA_LSB_SIZE(width, height, yfld)  \
                                                                    + FBM_MDDI_BI_FLD_C420_MSB_SIZE(width, height, cfld)  \
                                                                    + FBM_MDDI_BI_FLD_C420_LSB_SIZE(width, height, cfld)  + 16) * 2048) 

#define FBM_MDDI_BI_10BIT_420_SIZE(width, height, yfld, cfld)       ( FBM_MDDI_10BIT_420_SIZE(width, height, yfld, cfld) \
                                                                    + FBM_MDDI_BI_FLAG_MV_SIZE(width, height)\
                                                                    + FBM_MDDI_BI_FLAG_LM_SIZE(width, height))

#define FBM_MDDI_8BIT_420_SIZE(width, height, yfld, cfld)           ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, height, yfld)  \
                                                                    + FBM_MDDI_BI_FLD_C420_MSB_SIZE(width, height, cfld) + 16) * 2048)

#define FBM_MDDI_BI_8BIT_420_SIZE(width, height, yfld, cfld)        ( FBM_MDDI_8BIT_420_SIZE(width, height, yfld, cfld) \
                                                                    + FBM_MDDI_BI_FLAG_MV_SIZE(width, height) \
                                                                    + FBM_MDDI_BI_FLAG_LM_SIZE(width, height))

// Sub Interlace
#define FBM_MDDI_SUB_BI_8BIT_422_SIZE(width, height, yfld, cfld)    ((FBM_MDDI_BI_FLD_LUMA_MSB_SIZE(width, height, yfld)  \
                                                                    + FBM_MDDI_BI_FLD_C422_MSB_SIZE(width, height, cfld)  \
                                                                    + 16) * 2048)

// -------- SCPOS --------

#define FBM_SCPOS_MSB_SIZE(width, height, fbNum)        (((width + 255) / 256) * ((height * fbNum + 7) / 8))
#define FBM_SCPOS_LSB_SIZE(width, height, fbNum)        (((width + 255) / 256) * ((height * fbNum + 255) / 256))
// 422/  8 bit/ 3 Frames
#define FBM_SCPOS_SIZE(width, height)           ((((((FBM_SCPOS_MSB_SIZE(width, height, 3)+7)/8)*8) * 2) + 8) * 2048)
// 422/ 10 bit/ 3 Frames
#define FBM_SCPOS_10BIT_SIZE(width, height)     ((((((FBM_SCPOS_MSB_SIZE(width, height, 3)+7)/8)*8) * 2) * 2048) + ((FBM_SCPOS_LSB_SIZE(width, height, 3) * 2) * 16384) + (8 * 2048)) 
// 422/  8 bit/ 2 Frames
#define FBM_SCPOS_2FB_SIZE(width, height)       ((((((FBM_SCPOS_MSB_SIZE(width, height, 2)+7)/8)*8) * 2) + 8) * 2048)
// 444/  8 bit/ 3 Frame
#define FBM_SCPOS_444_3FB_SIZE(width, height)   ((((((FBM_SCPOS_MSB_SIZE(width, height, 3)+7)/8)*8) * 3) + 12) * 2048)

#define FBM_SCPOS_8BIT_SIZE_FLIP(width, height, frame)           FBM_ALIGN_MASK( ((((((FBM_SCPOS_MSB_SIZE(width, height, frame)+7)/8)*8) * 2) + 8) * 2048), FBM_SCPOS_SIZE_ALIGMENT)
#define FBM_SCPOS_10BIT_SIZE_FLIP(width, height, frame)          FBM_ALIGN_MASK( ((((((FBM_SCPOS_MSB_SIZE(width, height, frame)+7)/8)*8) * 2) * 2048) + ((FBM_SCPOS_LSB_SIZE(width, height, frame) * 2) * 16384) + (8 * 2048)),   FBM_SCPOS_SIZE_ALIGMENT)
#define FBM_SCPOS_444_3FB_POOL_SIZE_FLIP(width, height, frame)   FBM_ALIGN_MASK( ((((((FBM_SCPOS_MSB_SIZE(width, height, frame)+7)/8)*8) * 3) + 12) * 2048), FBM_SCPOS_SIZE_ALIGMENT)

// SCPOS Pool Size Macro
#define FBM_SCPOS_POOL_SIZE(align, width, height)           FBM_ALIGN_MASK(FBM_SCPOS_SIZE(width, height), align)
#define FBM_SCPOS_10BIT_POOL_SIZE(align, width, height)     FBM_ALIGN_MASK(FBM_SCPOS_10BIT_SIZE(width, height), align)
#define FBM_SCPOS_2FB_POOL_SIZE(align, width, height)       FBM_ALIGN_MASK(FBM_SCPOS_2FB_SIZE(width, height), align)
#define FBM_SCPOS_444_3FB_POOL_SIZE(align, width, height)   FBM_ALIGN_MASK(FBM_SCPOS_444_3FB_SIZE(width, height), align)

// SCPOS Alignment
#define FBM_SCPOS_ADDR_ALIGMENT                 (0x2000 - 1)       // 8K
#define FBM_SCPOS_SIZE_ALIGMENT                 (0x2000 - 1)


// -------- MJC --------
// MJC Alignment
#define FBM_MJC_ADDR_ALIGMENT                             (0x1000 - 1)     //((rpt444)? 63 : ((chanelA)? (0x1000 - 1) : (0x800 - 1))) // repeat mode 64byte,mjc mode chanel A 4k chanel B 2k
#define FBM_MJC_SIZE_ALIGMENT                             63 // 64bytw

#define FBM_MJC_HEIGHT_BASE(height)                       (FBM_ALIGN_MASK(height, 7) >> 3)
#define FBM_MJC_HEIGHT_YC_BASE(rpt444, height, dramCfg)   ((dramCfg & (FBM_MJC_DRAM_OSMP_FPR | FBM_MJC_DRAM_3D_HPR))? (rpt444 ? (height >> 1) : FBM_ALIGN_MASK((height >> 1), 7)) : height)       
#define FBM_MJC_WIDTH_YC_BASE(width)                       FBM_ALIGN_MASK(width, 15)

#define FBM_MJC_YC_FLD_BASE(chanelA, y8c8)                (((y8c8)? 8 : 6) << (chanelA))                                            
#define FBM_MJC_YC_SIZE(chanelA, y8c8, rpt444, bufNum, width ,height, dramCfg)                                                                                                                                                    \
                                                         (bufNum * ((FBM_ALIGN_MASK(((FBM_ALIGN_MASK(FBM_MJC_WIDTH_YC_BASE(width), 63) >> 6) * FBM_MJC_HEIGHT_BASE(FBM_MJC_HEIGHT_YC_BASE(rpt444, height, dramCfg))),          \
                                                          (FBM_MJC_YC_FLD_BASE(chanelA, y8c8) - 1))) / FBM_MJC_YC_FLD_BASE(chanelA, y8c8)) << 2 << 7 << (chanelA))

#define FBM_FRC_FULLQMBNUM(width)                        ((FBM_MJC_WIDTH_YC_BASE(width) >> 4) >> 2)
#define FBM_FRC_FULLQMBMBNUM(width)                      ((FBM_MJC_WIDTH_YC_BASE(width) >> 4) % 4)
#define FBM_FRC_BIT_NUM(y8c8, rpt422)                    ((y8c8)? 0 : ((rpt422)?  2 : 3))
#define FBM_FRC_YC_SIZE(chanelA, y8c8, rpt444, rpt422, bufNum, width , height, dramCfg)                                                                                                                                           \
                                                        (bufNum * FBM_MJC_HEIGHT_YC_BASE(rpt444, height, dramCfg) * FBM_ALIGN_MASK(((FBM_FRC_FULLQMBNUM(width) * (((rpt422)? 8 : 12) + FBM_FRC_BIT_NUM(y8c8, rpt422))) +      \
                                                         (((FBM_FRC_FULLQMBMBNUM(width) != 0)? 1 : 0) * (((rpt422)? FBM_FRC_FULLQMBMBNUM(width) * 2 : FBM_FRC_FULLQMBMBNUM(width) * 3) + FBM_FRC_BIT_NUM(y8c8, rpt422)))), 3))                                        

#define FBM_MJC_OSD_SIZE(width ,height)                 (FBM_ALIGN_MASK((FBM_ALIGN_MASK(width, 7) >> 2), 63) * FBM_MJC_HEIGHT_BASE(height)) 
#define FBM_MJC_MV_SIZE(width ,height)                  (FBM_ALIGN_MASK((FBM_ALIGN_MASK(width, 7) >> 2), 63) * FBM_MJC_HEIGHT_BASE(height) * 5)
#define FBM_MJC_TBA_SIZE(width ,height)                 (FBM_ALIGN_MASK((FBM_ALIGN_MASK(width, 7) >> 3), 63) * FBM_MJC_HEIGHT_BASE(height))

#define FBM_MJC_MEMC_SIZE(rpt444, width ,height)        ((rpt444)? 0 : ((FBM_MJC_OSD_SIZE(width ,height) << 1) + (FBM_MJC_MV_SIZE(width ,height) << 1) + (FBM_MJC_TBA_SIZE(width ,height) << 1)))
#define FBM_MJC_POOL_SIZE(chanelA, y8c8, rpt444, rpt422, bufNum, width ,height, dramCfg)                                                                                                                                           \
                                                        ((((((rpt444)? FBM_FRC_YC_SIZE(chanelA, y8c8, rpt444, rpt422, bufNum, width, height, dramCfg) : FBM_MJC_YC_SIZE(chanelA, y8c8, rpt444, bufNum, width , height, dramCfg))\
                                                         << ((dramCfg & FBM_MJC_DRAM_OSMP_FPR)? 2 : ((dramCfg & (FBM_MJC_DRAM_3D_SG | FBM_MJC_DRAM_3D_HPR))? 1 : 0))) << 4) + FBM_MJC_MEMC_SIZE(rpt444, width ,height)) + 0x1000)

#define FBM_MJC_WIDTH                                   (1920)   
#define FBM_MJC_HEIGHT                                  (1080)  //if is l2r2 pdp(1920 * 2160) panel setting height = 2160 / 2 = 1080;

#define FBM_MJC_8BIT_422_CHB_POOL_SIZE                  FBM_MJC_POOL_SIZE(0, 1, 0, 0, 9, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)   // Only  MJC_Y08_C08 case ;and 420 is same size as 422
#define FBM_MJC_8BIT_422_CHA_POOL_SIZE                  FBM_MJC_POOL_SIZE(1, 1, 0, 0, 9, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)
#define FBM_MJC_10BIT_422_CHB_POOL_SIZE                 FBM_MJC_POOL_SIZE(0, 0, 0, 0, 9, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)   // Not  MJC_Y08_C08 case ;and 420 is same size as 422
#define FBM_MJC_10BIT_422_CHA_POOL_SIZE                 FBM_MJC_POOL_SIZE(1, 0, 0, 0, 9, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)
#define FBM_FRC_8BIT_444_CHA_POOL_SIZE                  FBM_MJC_POOL_SIZE(1, 1, 1, 0, 3, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)
#define FBM_FRC_10BIT_444_CHA_POOL_SIZE                 FBM_MJC_POOL_SIZE(1, 0, 1, 0, 3, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)
#define FBM_FRC_10BIT_444_CHB_POOL_SIZE                 FBM_MJC_POOL_SIZE(0, 0, 1, 0, 3, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)
#define FBM_FRC_8BIT_422_CHA_POOL_SIZE                  FBM_MJC_POOL_SIZE(1, 1, 1, 1, 3, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)
#define FBM_FRC_10BIT_422_CHA_POOL_SIZE                 FBM_MJC_POOL_SIZE(1, 0, 1, 1, 3, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)
#define FBM_FRC_10BIT_422_CHB_POOL_SIZE                 FBM_MJC_POOL_SIZE(0, 0, 1, 1, 3, FBM_ALIGN_MASK(FBM_MJC_WIDTH, 15), FBM_ALIGN_MASK(FBM_MJC_HEIGHT, 7), FBM_MJC_DRAM_3D_CFG)

// -------- TDC --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_TDC_ADDR_ALIGMENT                   (PAGE_SIZE - 1)
#else
#define FBM_TDC_ADDR_ALIGMENT                   (2048 - 1)
#endif

#define FBM_TDC_SIZE_ALIGMENT                   (32 - 1)
#define FBM_TDC_SIZE                            0x2BFE90//0x2B4C10 // [10-bit] // 0x251ca0 [9-bit]
#define FBM_TDC_8BIT_SIZE                       0x251ca0
#define FBM_TDC_POOL_SIZE                       FBM_ALIGN_MASK((FBM_TDC_SIZE), FBM_TDC_SIZE_ALIGMENT)
#define FBM_TDC_8BIT_POOL_SIZE                  FBM_ALIGN_MASK((FBM_TDC_8BIT_SIZE), FBM_TDC_SIZE_ALIGMENT)

// -------- NR --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_NR_ADDR_ALIGMENT                    (PAGE_SIZE - 1)
#else
#define FBM_NR_ADDR_ALIGMENT                    31
#endif
#define FBM_NR_SIZE_ALIGMENT                    31

// -------- VBI --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_VBI_ADDR_ALIGMENT                   (PAGE_SIZE - 1)
#else
#define FBM_VBI_ADDR_ALIGMENT                   (16 - 1)
#endif
#define FBM_VBI_SIZE_ALIGMENT                   0
#define FBM_VBI_SIZE                            (0x17700 + 0xC00 + 0x10)  //=0x18310
#define FBM_VBI_POOL_SIZE                       FBM_ALIGN_MASK((FBM_VBI_SIZE), FBM_VBI_SIZE_ALIGMENT)

// -------- MPEG  --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_MPEG_ADDR_ALIGMENT                  (((FBM_FMG_Y_ALIGMENT) > (PAGE_SIZE - 1)) ? (FBM_FMG_Y_ALIGMENT) : (PAGE_SIZE - 1))
#else
#define FBM_MPEG_ADDR_ALIGMENT                  FBM_FMG_Y_ALIGMENT
#endif
#define FBM_MPEG_POOL_PAD                       (1920 * 32)
#define FBM_MPEG_720P_POOL_PAD                  (1280 * 32)
#define FBM_MPEG_SD_POOL_PAD                    (720 * 32)

#define FBM_JPE_VDP_SIZE                        (0x1000000)
#define FBM_JPG_VDP_ADDR_ALIGMENT               (0x1000 - 1)
#define FBM_JPE_VDP_POOL_SIZE                   FBM_ALIGN_MASK((FBM_JPE_VDP_SIZE), FBM_JPG_VDP_ADDR_ALIGMENT)

// -------- DIVX  --------
#define FBM_DIVX_HD_SIZE                        (0x600000)
#define FBM_DIVX_SIZE_ALIGMENT                   0
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_DIVX_ADDR_ALIGMENT               (PAGE_SIZE - 1)
#else
#define FBM_DIVX_ADDR_ALIGMENT               (16 - 1)
#endif
#define FBM_DIVX_POOL_SIZE                   FBM_ALIGN_MASK((FBM_DIVX_HD_SIZE), FBM_DIVX_SIZE_ALIGMENT)

// -------- DSP.bin --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_DSP_BIN_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_DSP_BIN_ADDR_ALIGMENT                  (0x40- 1)
#endif
#define FBM_DSP_BIN_SIZE_ALIGMENT                  (0x40- 1)

// -------- DSP  --------
//#define FBM_DSP_ADDR_ALIGMENT                   (0x1000000-1) // (256 - 1)
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_DSP_ADDR_ALIGMENT                   (PAGE_SIZE - 1)
#else
#define FBM_DSP_ADDR_ALIGMENT                   (1024 - 1)
#endif
#define FBM_DSP_SIZE_ALIGMENT                   0

// MT5365/95
// MT5365/95
#ifndef CC_AUD_SUPPORT_DUAL_DSP 
#if ( defined(CC_MT5399) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5882)||defined(CC_MT5890))
#if defined(CC_AUD_DSP_CHINA_MODEL) || defined(CC_AUD_DSP_US_MODEL)
#ifdef CC_MT5881
#define FBM_AUD_BASE                        (0x3c7c00)    //3871 kB
#else
#define FBM_AUD_BASE                        (0x2A8C00+0x8F000)    // 3295 kB
#endif
#else
#define FBM_AUD_BASE                        (0x407C00)    // 4127 kB
#endif
#else
#define FBM_AUD_BASE                        (0x2a4400+0x8F000)    // 3277 kb
#endif
#define FBM_AUD_AFIFO                       0x48000     // 288 kb
#ifdef __MODEL_slt__
#define FBM_AUD_AFIFO2                      0xf0000     // 960 kb
#else
#ifdef CC_AUD_KARAOKE_SUPPORT
#define FBM_AUD_AFIFO2                      0x48000     // 288 kb
#else
#ifdef CC_AUD_DDI
#define FBM_AUD_AFIFO2                      0x48000     // 288 kb
#else
#define FBM_AUD_AFIFO2                      0x21C00     // 135 kb
#endif
#endif
#endif
#ifdef CC_AUD_SKYPE_SUPPORT
#define FBM_AUD_AFIFO3                      0x2f800      // 190 kb
#else
#ifdef CC_AUD_DDI
#define FBM_AUD_AFIFO3                      0x48000     // 288 kb
#else
#define FBM_AUD_AFIFO3                      0x9c00      // 39 kb
#endif
#endif
#if (defined(CC_MT5399) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5882)||defined(CC_MT5890))
#if defined(CC_AUD_DSP_CHINA_MODEL) || defined(CC_AUD_DSP_US_MODEL)
#define FBM_AUD_AFIFO4                      0x0         // 0 kb
#else
#define FBM_AUD_AFIFO4                      0x2000      // 16 kb
#endif
#else
#define FBM_AUD_AFIFO4                      0x0         // 0 kb
#endif
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
#define FBM_AUD_HDMI_PARSER                 0x60000     // 126 kb
#else
#define FBM_AUD_HDMI_PARSER                 0x0         // 0 kb
#endif
#ifdef CC_AUD_MIXSOUND_SUPPORT
#ifdef CC_MT5881
#define FBM_AUD_MIXSNDBUF                   0xc000     // 208 kb
#else
#ifdef CC_AUD_DDI
#define FBM_AUD_MIXSNDBUF                   0xA0000//640k
#else
#define FBM_AUD_MIXSNDBUF                   0x58000     // 352 kb
#endif
#endif
#else
#define FBM_AUD_MIXSNDBUF                   0x0         // 0 kb
#endif
#if defined(CC_MT5398)
#define FBM_AUD_APROC_WORKSPACE              0x40000    // 256 kb
#else
#define FBM_AUD_APROC_WORKSPACE              0x0
#endif
#ifdef CC_AUD_APOLLO_SUPPORT
#define FBM_AUD_APROC_CODE_SZ          (0x100000) // 1M Bytes 
#define FBM_AUD_APROC_DATA_SZ          (0x100000) // 1M Bytes 
#define FBM_AUD_APROC_ALIGNBUF_SZ         (0x1000) //4k Bytes
#define FBM_AUD_APOLLO_DRAM_SZ (FBM_AUD_APROC_CODE_SZ+FBM_AUD_APROC_DATA_SZ+FBM_AUD_APROC_ALIGNBUF_SZ)
#else
#define FBM_AUD_APOLLO_DRAM_SZ 0x0
#endif
#define FBM_DSP_SIZE    (FBM_AUD_BASE+FBM_AUD_AFIFO+FBM_AUD_AFIFO2+FBM_AUD_AFIFO3+  \
                         FBM_AUD_AFIFO4+FBM_AUD_HDMI_PARSER+FBM_AUD_MIXSNDBUF+ \
                         FBM_AUD_APROC_WORKSPACE+FBM_AUD_APOLLO_DRAM_SZ)

#define FBM_DSP_POOL_SIZE                       FBM_ALIGN_MASK((FBM_DSP_SIZE), FBM_DSP_SIZE_ALIGMENT)
// Dual DSP size in MT5363.
#ifdef __MODEL_slt__
#define FBM_DUAL_DSP_POOL_SIZE                  FBM_ALIGN_MASK((0x4b0000), FBM_DSP_SIZE_ALIGMENT)
#else
#define FBM_DUAL_DSP_POOL_SIZE                  FBM_ALIGN_MASK((0x320000), FBM_DSP_SIZE_ALIGMENT)
#endif
#else
// new dual DSP case
#ifdef CC_AUD_ARM_RENDER
#define FBM_APROC_IO_BUFFER_SZ 0x24000*4
#else
#define FBM_APROC_IO_BUFFER_SZ 0
#endif

#ifdef CC_AUD_DISABLE_2ND_DSP
#ifdef CC_AUD_DDI
#define FBM_AUD_BASE                         (0x421C00+0x000000+0x4000+FBM_APROC_IO_BUFFER_SZ)    // 4231 kB + 0KB + 576kB (DSP0+DSP1+Arpoc_io)
#else
#define FBM_AUD_BASE                         (0x421C00+0x000000+FBM_APROC_IO_BUFFER_SZ)    // 4231 kB + 0KB + 576kB (DSP0+DSP1+Arpoc_io)
#endif
#define FBM_AUD_AFIFO_DSP0               0x48000     // 288 kb
#define FBM_AUD_AFIFO_DSP1               0x00000     // 039 kb
#ifdef __MODEL_slt__
#define FBM_AUD_AFIFO2_DSP0             0xf0000     // 960 kb
#define FBM_AUD_AFIFO2_DSP1             0x00000     // 960 kb
#else
#ifdef CC_AUD_DDI
#define FBM_AUD_AFIFO2_DSP0             0x48000     // 288 kb
#else
#define FBM_AUD_AFIFO2_DSP0             0x21C00     // 135 kb
#endif
#define FBM_AUD_AFIFO2_DSP1             0x00000     // 039 kb
#endif
#else
#ifdef CC_AUD_DDI
#define FBM_AUD_BASE                         (0x421C00+0x2A7000+0x4000+FBM_APROC_IO_BUFFER_SZ)    // 4231 kB + 2716KB + 576B (DSP0+DSP1+Arpoc_io)
#else
#define FBM_AUD_BASE                         (0x421C00+0x2A7000+FBM_APROC_IO_BUFFER_SZ)    // 4231 kB + 2716KB + 576B (DSP0+DSP1+Arpoc_io)
#endif
#define FBM_AUD_AFIFO_DSP0               0x48000     // 288 kb
#define FBM_AUD_AFIFO_DSP1               0x09C00     // 039 kb
#ifdef __MODEL_slt__
#define FBM_AUD_AFIFO2_DSP0             0xf0000     // 960 kb
#define FBM_AUD_AFIFO2_DSP1             0xf0000     // 960 kb
#else
#ifdef CC_AUD_DDI
#define FBM_AUD_AFIFO2_DSP0             0x48000     // 288 kb
#else
#define FBM_AUD_AFIFO2_DSP0             0x21C00     // 135 kb
#endif
#define FBM_AUD_AFIFO2_DSP1             0x09C00     // 039 kb
#endif
#endif // end CC_AUD_DISABLE_2ND_DSP
#ifdef CC_AUD_DDI
#define FBM_AUD_AFIFO3_DSP0             0x48000     // 288 kb
#else
#define FBM_AUD_AFIFO3_DSP0             0x09C00     // 039 kb
#endif
#define FBM_AUD_AFIFO3_DSP1             0x00000     // 000 kb
#define FBM_AUD_AFIFO4_DSP0             0x02000     // 008 kb
#define FBM_AUD_AFIFO4_DSP1             0x00000     // 000 kb
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
#define FBM_AUD_HDMI_PARSER                 0x60000     // 126 kb
#else
#define FBM_AUD_HDMI_PARSER                 0x0         // 0 kb
#endif
#ifdef CC_AUD_DDI
#define FBM_AUD_MIXSNDBUF                   0xA0000//640k
#else
#define FBM_AUD_MIXSNDBUF                   0x58000     // 352 kb
#endif
#if defined(CC_AUD_ARM_SUPPORT)
 #if defined (CC_AUD_DISABLE_2ND_DSP) && defined (CC_AUD_ENLARGE_APROC_DRAM)
#define FBM_AUD_APROC_WORKSPACE              (0x300000+0x2A3000)  // 3MB + 2.6MB(from 2nd DSP)
 #else
#define FBM_AUD_APROC_WORKSPACE              (0x400000)    // 3MB
 #endif
#else
#define FBM_AUD_APROC_WORKSPACE              0x40000    // 256 kb
#endif // CC_AUD_ARM11_SUPPORT
#define FBM_DSP_SIZE    (FBM_AUD_BASE+FBM_AUD_AFIFO_DSP0+FBM_AUD_AFIFO2_DSP0+FBM_AUD_AFIFO3_DSP0+  \
	                  +FBM_AUD_AFIFO_DSP1+FBM_AUD_AFIFO2_DSP1+FBM_AUD_AFIFO3_DSP1+FBM_AUD_AFIFO4_DSP1+ \
                         FBM_AUD_AFIFO4_DSP0+FBM_AUD_HDMI_PARSER+FBM_AUD_MIXSNDBUF+ \
                         FBM_AUD_APROC_WORKSPACE)

#define FBM_DSP_POOL_SIZE                       FBM_ALIGN_MASK((FBM_DSP_SIZE), FBM_DSP_SIZE_ALIGMENT)
#endif


// -------- DMX  --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_DMX_ADDR_ALIGMENT                   (PAGE_SIZE - 1)
#else
#define FBM_DMX_ADDR_ALIGMENT                   (64 - 1)
#endif
#define FBM_DMX_SIZE_ALIGMENT                   (64 - 1)

// -------- W2D R2R  --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_W2D_R2R_ADDR_ALIGMENT                   (PAGE_SIZE - 1)
#else
#define FBM_W2D_R2R_ADDR_ALIGMENT                   (64 - 1)
#endif
#define FBM_W2D_R2R_SIZE_ALIGMENT                   (64 - 1)

#define FBM_DMX_5381_FHD_SIZE                   0x280000
#define FBM_DMX_5371_SIZE                       0x180000
#define FBM_DMX_5380_16MB_SIZE                  0x100000
#define FBM_DMX_DVBT_SIZE                       0xC0000
#define FBM_DMX_POOL_SIZE                       FBM_ALIGN_MASK((FBM_DMX_SIZE), FBM_DMX_SIZE_ALIGMENT)


// -------- B2R  --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_B2R_ADDR_ALIGMENT                     (PAGE_SIZE - 1)
#else
#define FBM_B2R_ADDR_ALIGMENT                     (64 - 1)
#endif
#define FBM_B2R_SIZE_ALIGMENT                     0
#define FBM_B2R_POOL_SIZE(width, height, ns, bpp)   FBM_ALIGN_MASK(((((width * height) * bpp) + (FBM_B2R_ADDR_ALIGMENT + 1)) * ns), FBM_B2R_SIZE_ALIGMENT)
#define FBM_B2R_420_POOL_SIZE(width, height, ns)    FBM_ALIGN_MASK((((((width * height) * 3) / 2) + (FBM_B2R_ADDR_ALIGMENT + 1)) * ns), FBM_B2R_SIZE_ALIGMENT)

// -------- JPEG  --------
#ifdef __KERNEL__
#define FBM_JPEG_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#define FBM_JPEG_SIZE_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_JPEG_ADDR_ALIGMENT                  (256 - 1)
#define FBM_JPEG_SIZE_ALIGMENT                  0
#endif

#ifdef CC_OSD_USE_FBM
#ifdef CC_53XX_JPG_ARGB4444
#define FBM_JPEG_POOL_SIZE                      (0xc00000)	//(1920*1080*4)
#else
#define FBM_JPEG_POOL_SIZE                      (0x800000)	//(1920*1080*4)
#endif
#else
#define FBM_JPEG_POOL_SIZE                      (1920*1080*6)
#endif


// -------- DMXPID --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_DMXPID_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_DMXPID_ADDR_ALIGMENT                  (64-1)
#endif
#define FBM_DMXPID_SIZE_ALIGMENT                  (64-1)
#define FBM_DMXPID_POOL_SIZE                      (FBM_DMX_PID_BUF)
#ifdef __KERNEL__
#define FBM_PNG_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#define FBM_PNG_SIZE_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_PNG_ADDR_ALIGMENT                  (256 - 1)
#define FBM_PNG_SIZE_ALIGMENT                  0
#endif

// -------- PQ_PSCAN --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_PQ_PSCAN_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_PQ_PSCAN_ADDR_ALIGMENT                  (64-1)
#endif
#define FBM_PQ_PSCAN_SIZE_ALIGMENT                  (64-1)
#define FBM_PQ_PSCAN_POOL_SIZE                      (0x300000)

// -------- IMGRZ_3D_KR --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_IMGRZ_3D_KR_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_IMGRZ_3D_KR_ADDR_ALIGMENT                  (64-1)
#endif
#define FBM_IMGRZ_3D_KR_SIZE_ALIGMENT                  (64-1)
#define FBM_IMGRZ_3D_KR_POOL_SIZE                      (0x600000)

// -------- OSD  --------
#ifdef __KERNEL__
#define FBM_OSD_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#define FBM_OSD_SIZE_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_OSD_ADDR_ALIGMENT                  (16 - 1)
#define FBM_OSD_SIZE_ALIGMENT                  0
#endif

#define FBM_OSD_POOL_SIZE(width, height, ns, bpp)   FBM_ALIGN_MASK(((((width * height) * bpp) + (FBM_OSD_ADDR_ALIGMENT + 1)) * ns), FBM_OSD_SIZE_ALIGMENT)

// -------- Feeder  --------
#ifdef __KERNEL__
#define FBM_FEEDER_ADDR_ALIGMENT               (0x1000 - 1)
#define FBM_FEEDER_SIZE_ALIGMENT               (0x1000 - 1)
#else
#define FBM_FEEDER_ADDR_ALIGMENT               (16 - 1)
#define FBM_FEEDER_SIZE_ALIGMENT               0
#endif
#define FBM_FEEDER_4K_POOL_SIZE                FBM_ALIGN_MASK(0xd00000, FBM_FEEDER_SIZE_ALIGMENT)
#define FBM_FEEDER_POOL_SIZE                   FBM_ALIGN_MASK(0x300000, FBM_FEEDER_SIZE_ALIGMENT)
#define FBM_FEEDER_MMP_POOL_SIZE               FBM_ALIGN_MASK(0x90000, FBM_FEEDER_SIZE_ALIGMENT)

#define FBM_TVE_ADDR_ALIGMENT                 (0x2000 - 1)
#define FBM_TVE_SIZE_ALIGMENT                 (0x2000 - 1)
#ifdef CC_FLIP_MIRROR_SUPPORT
#if defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882) //422 10bit 3 frame buffer
#define FBM_TVE_POOL_SIZE                      FBM_ALIGN_MASK(FBM_TVE_POOL_FBM_SIZE, FBM_TVE_SIZE_ALIGMENT)
#else
#define FBM_TVE_POOL_SIZE                      FBM_ALIGN_MASK(0x580000, FBM_TVE_SIZE_ALIGMENT)
#endif
#else
#if defined(CC_MT5399) ||defined(CC_MT5890)||defined(CC_MT5882)//422 10bit 2 frame buffer
#define FBM_TVE_POOL_SIZE                      FBM_ALIGN_MASK(FBM_TVE_POOL_FBM_SIZE, FBM_TVE_SIZE_ALIGMENT)
#else
#define FBM_TVE_POOL_SIZE                      FBM_ALIGN_MASK(0x400000, FBM_TVE_SIZE_ALIGMENT)
#endif
#endif
// -------- PQ TOOL  --------
//#define FBM_PQ_TOOL_POOL_SIZE                    (0x800000)   // OD full dump need extra buffer 8MB
#ifdef CC_CDC_SUPPORT
#define FBM_PQ_TOOL_POOL_SIZE                      (0x1800000)    // OD partical dump (max: 1920*64)
#define FBM_PQ_TOOL_ADDR_ALIGMENT                  (4096 - 1)
#elif defined(CC_PQ_TOOL_8M)
#define FBM_PQ_TOOL_POOL_SIZE                    (0x800000)   //LG // OD full dump need extra buffer 8MB
#define FBM_PQ_TOOL_ADDR_ALIGMENT                  (4096 - 1)
#else
#define FBM_PQ_TOOL_POOL_SIZE                      (0)    // OD partical dump (max: 1920*64)
#define FBM_PQ_TOOL_ADDR_ALIGMENT                  (4096 - 1)
#endif
#define FBM_PQ_TOOL_SIZE_ALIGMENT                  0

// -------- OD --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_OD_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_OD_ADDR_ALIGMENT                  (0x40- 1)
#endif
#define FBM_OD_SIZE_ALIGMENT                  (0x40- 1)
#define FBM_OD_POOL_SIZE                      (540*248*16)//(1920*1080)

// -------- VENC --------
#define FBM_VENC_ADDR_ALIGMENT                  (2048- 1)
#define FBM_VENC_SIZE_ALIGMENT                  (2048- 1)
#if  defined(CC_MT5399) ||defined(CC_MT5890)||defined(CC_MT5882)
#define FBM_VENC_POOL_SIZE                      (0x1F00000)
#else
#define FBM_VENC_POOL_SIZE                      (0xE00000)
#endif

// -------- SWDMX --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_SWDMX_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_SWDMX_ADDR_ALIGMENT                  (16- 1)
#endif
#define FBM_SWDMX_SIZE_ALIGMENT                  (16- 1)
#define FBM_SWDMX_POOL_SIZE                      FBM_SWDMX_SIZE
// -------- VSS --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_VSS_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
//#define FBM_VSS_ADDR_ALIGMENT                  (16- 1)
//VSS always align 4k
#define FBM_VSS_ADDR_ALIGMENT                  (0x1000 - 1)
#endif
#define FBM_VSS_SIZE_ALIGMENT                  (16- 1)
#define FBM_VSS_POOL_SIZE                      (0xFD2000)

// -------- FACE DETECTION --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_FD_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_FD_ADDR_ALIGMENT                  (0x40- 1)
#endif
#define FBM_FD_SIZE_ALIGMENT                  (0x40- 1)
#define FBM_FD_POOL_SIZE                      (800*1024)

// -------- PVR  --------
#ifdef __KERNEL__
#define FBM_PVR_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#define FBM_PVR_SIZE_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_PVR_ADDR_ALIGMENT                  (256 - 1)
#define FBM_PVR_SIZE_ALIGMENT                  (256 - 1)
#endif
#if defined(CC_DUAL_PVR_SUPPORT)
#define FBM_PVR_POOL_SIZE                      (0x600000*2)
#else
#define FBM_PVR_POOL_SIZE                      (0x380000)
#endif
// -------- PVR2  --------
#ifdef __KERNEL__
#define FBM_PVR2_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#define FBM_PVR2_SIZE_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_PVR2_ADDR_ALIGMENT                  (256 - 1)
#define FBM_PVR2_SIZE_ALIGMENT                  (256 - 1)
#endif
#define FBM_PVR2_POOL_SIZE                      (3*1024*1024)

// -------- PQ_3DC --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_PQ_3DC_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_PQ_3DC_ADDR_ALIGMENT                  (64 - 1)
#endif
#define FBM_PQ_3DC_SIZE_ALIGMENT                  (64 - 1)
#define FBM_PQ_3DC_POOL_SIZE                      (0x80000)
// -------- EDB --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_EDB_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_EDB_ADDR_ALIGMENT                  (16- 1)
#endif
#define FBM_EDB_SIZE_ALIGMENT                  (16- 1)
#define FBM_EDB_POOL_SIZE                      FBM_EDB_SHARE_SIZE

// -------- SRS --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_SRS_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_SRS_ADDR_ALIGMENT                  (64 - 1)
#endif
#define FBM_SRS_POOL_SIZE                      (0x200000)

// -------- REMUX --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_REMUX_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_REMUX_ADDR_ALIGMENT                  (32 - 1)
#endif

// -------- hw demod --------
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#define FBM_HW_DEMOD_ADDR_ALIGMENT                  (PAGE_SIZE - 1)
#else
#define FBM_HW_DEMOD_ADDR_ALIGMENT                  (16 - 1)
#endif

#define FBM_REMUX_POOL_SIZE                      (0x500000)


#if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#define FBM_EXTRA_FOR_4K2K_SCL_DRAM         (0x4000000) 
#endif

#ifdef CC_MEM_TOOL
#undef __KERNEL__
#endif

#endif    // FBM_POOL_IF_H
