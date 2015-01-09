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
 * $RCSfile: hw_vbi.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
#ifndef _HW_VBI_H_
#define _HW_VBI_H_

#include "typedef.h"
#include "hw_vdoin.h"

#define SUPPORT_PALCC_LINE18    1
#define SUPPORT_AUTO_CC_DETECT  1
#define SUPPORT_HD_TIMING_VBI   1
#define SUPPORT_TYPE_B_VBI      1


#define VBI_IO_BASE (0x28c0 + VDOIN_BASE)
#define VBI_STA_IO_BASE (0x2240 +VDOIN_BASE)
/*****************************************************************************
  VBI Dram Allocation
  ****************************************************************************/
#define VBI_NORMAL_BASE	(VBI_DRAM_BASE )

#define VBI_FIFO_SIZE        0x17700    /* (2000 * 48 = 96000 = 0x17700) => for FIFO mode processing */

#define LINE_SHIFT      1

/*  Register Map Definition Start */
#define VBI_00 (VBI_IO_BASE+0x0000)
  #define VBI_00_0 (VBI_IO_BASE+0x0000)
  #define VBI_00_1 (VBI_IO_BASE+0x0001)
  #define VBI_00_2 (VBI_IO_BASE+0x0002)
  #define VBI_00_3 (VBI_IO_BASE+0x0003)
    #define VBI_VPS_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI_CC_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI_WSS_EN Fld(1, 29, AC_MSKB3) //29
    #define VBI_TT_EN Fld(1, 28, AC_MSKB3) //28
    #define VBI_ZC_THR Fld(8, 20, AC_MSKW32) //27:20
    #define VBI_THR_DET_WIDTH Fld(6, 14, AC_MSKW21) //19:14
    #define VBI_PE_GAIN Fld(2, 12, AC_MSKB1) //13:12
    #define VBI_CC_ECC_EN Fld(1, 11, AC_MSKB1) //11
    #define VBI_TT_ECC_EN Fld(1, 10, AC_MSKB1) //10
    #define VBI_TT_ECC_SEL Fld(1, 9, AC_MSKB1) //9
    #define VBI_ERR_SEL Fld(1, 8, AC_MSKB1) //8
    #define VBI_ERR_RST Fld(1, 7, AC_MSKB0) //7
    #define VBI_DC_OFFSET Fld(3, 4, AC_MSKB0) //6:4
    #define VBI_PROBLEM_TH Fld(4, 0, AC_MSKB0) //3:0
#define VBI_01 (VBI_IO_BASE+0x0004)
  #define VBI_01_0 (VBI_IO_BASE+0x0004)
  #define VBI_01_1 (VBI_IO_BASE+0x0005)
  #define VBI_01_2 (VBI_IO_BASE+0x0006)
  #define VBI_01_3 (VBI_IO_BASE+0x0007)
    #define VBI_PSCAN Fld(1, 31, AC_MSKB3) //31
    #define VBI_ALL_ADAPTIVE Fld(1, 30, AC_MSKB3) //30
    #define VBI_PDET_END Fld(6, 24, AC_MSKB3) //29:24
    #define VBI_ERR_POW_SEL Fld(1, 23, AC_MSKB2) //23
    #define VBI_ERR_NO_ZERO Fld(1, 22, AC_MSKB2) //22
    #define VBI_ECC_SEL3 Fld(1, 21, AC_MSKB2) //21
    #define VBI_ECC_SEL2 Fld(1, 20, AC_MSKB2) //20
    #define VBI_CNI0_STA_SEL Fld(1, 19, AC_MSKB2) //19
    #define VBI_HDTV_SYNC_DIV Fld(1, 18, AC_MSKB2) //18
    #define VBI_VGA_CKO_DIV2 Fld(1, 17, AC_MSKB2) //17
    #define VBI_VSRC Fld(2, 15, AC_MSKW21) //16:15
    #define VBI_TT_LINE_INFO Fld(1, 14, AC_MSKB1) //14
    #define VBI_DIS_OFFSET Fld(1, 13, AC_MSKB1) //13
    #define VBI_STC_ERR Fld(1, 12, AC_MSKB1) //12
    #define VBI_TT_ZC_SEL Fld(1, 11, AC_MSKB1) //11
    #define VBI_3D_SEL Fld(1, 10, AC_MSKB1) //10
    #define VBI_IMODE Fld(3, 7, AC_MSKW10) //9:7
    #define VBI_TVD_HSEL Fld(1, 6, AC_MSKB0) //6
    #define VBI_INIT_ZC Fld(1, 5, AC_MSKB0) //5
    #define VBI_BLANK_EN Fld(1, 4, AC_MSKB0) //4
    #define VBI_DEBUG_SEL Fld(2, 2, AC_MSKB0) //3:2
    #define VBI_CLR_OVF Fld(1, 1, AC_MSKB0) //1
    #define VBI_TT_STC_ERR_ONLY Fld(1, 0, AC_MSKB0) //0
#define VBI_02 (VBI_IO_BASE+0x0008)
  #define VBI_02_0 (VBI_IO_BASE+0x0008)
  #define VBI_02_1 (VBI_IO_BASE+0x0009)
  #define VBI_02_2 (VBI_IO_BASE+0x000a)
  #define VBI_02_3 (VBI_IO_BASE+0x000b)
    #define VBI_POW_LINE_SEL Fld(7, 25, AC_MSKB3) //31:25
    #define VBI_MBASE0 Fld(25, 0, AC_MSKDW) //24:0
#define VBI_03 (VBI_IO_BASE+0x000c)
  #define VBI_03_0 (VBI_IO_BASE+0x000c)
  #define VBI_03_1 (VBI_IO_BASE+0x000d)
  #define VBI_03_2 (VBI_IO_BASE+0x000e)
  #define VBI_03_3 (VBI_IO_BASE+0x000f)
    #define VBI_LSC Fld(8, 24, AC_FULLB3) //31:24
    #define VBI_RUNIN_WIDTH Fld(4, 20, AC_MSKB2) //23:20
    #define VBI_RUNIN_SEL Fld(1, 19, AC_MSKB2) //19
    #define VBI_STC_PTHR Fld(3, 16, AC_MSKB2) //18:16
    #define VBI_STC_PROBLEM Fld(1, 15, AC_MSKB1) //15
    #define VBI_POW_DIFF Fld(3, 12, AC_MSKB1) //14:12
    #define VBI_DDET_WIN Fld(2, 10, AC_MSKB1) //11:10
    #define VBI_CC2X Fld(1, 9, AC_MSKB1) //9
    #define VBI_DEBUG_SEL2 Fld(1, 8, AC_MSKB1) //8
    #define VBI_CCH_VBI_EN Fld(1, 7, AC_MSKB0) //7
    #define VBI_HDTV_H_SEL Fld(1, 6, AC_MSKB0) //6
    #define VBI_HDTV_V_SEL Fld(1, 5, AC_MSKB0) //5
    #define ANCI_EN Fld(1, 4, AC_MSKB0) //4
    #define ANCI_MODE Fld(2, 2, AC_MSKB0) //3:2
    #define ANCI_HEADER_LSB8 Fld(1, 1, AC_MSKB0) //1
    #define ANCI_LINE_WRITE Fld(1, 0, AC_MSKB0) //0
#define VBI_04 (VBI_IO_BASE+0x0010)
  #define VBI_04_0 (VBI_IO_BASE+0x0010)
  #define VBI_04_1 (VBI_IO_BASE+0x0011)
  #define VBI_04_2 (VBI_IO_BASE+0x0012)
  #define VBI_04_3 (VBI_IO_BASE+0x0013)
    #define VBI_OMODE Fld(1, 31, AC_MSKB3) //31
    #define VBI_MFIFORST Fld(1, 30, AC_MSKB3) //30
    #define VBI_MFIFO Fld(1, 29, AC_MSKB3) //29
    #define VBI_POW_END Fld(4, 25, AC_MSKB3) //28:25
    #define VBI_MOFFSET Fld(25, 0, AC_MSKDW) //24:0
#define VBI_05 (VBI_IO_BASE+0x0014)
  #define VBI_05_0 (VBI_IO_BASE+0x0014)
  #define VBI_05_1 (VBI_IO_BASE+0x0015)
  #define VBI_05_2 (VBI_IO_BASE+0x0016)
  #define VBI_05_3 (VBI_IO_BASE+0x0017)
    #define VBI_CC_DRAM_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI_CC_LPF_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI_CC_DC_SEL Fld(1, 29, AC_MSKB3) //29
    #define VBI_CC_INIT_SEL Fld(1, 28, AC_MSKB3) //28
    #define VBI_CC_SLICE_AVG Fld(1, 27, AC_MSKB3) //27
    #define VBI_CC_STC_END Fld(7, 20, AC_MSKW32) //26:20
    #define VBI_CC_START0 Fld(5, 15, AC_MSKW21) //19:15
    #define VBI_CC_END0 Fld(5, 10, AC_MSKB1) //14:10
    #define VBI_CC_START1 Fld(5, 5, AC_MSKW10) //9:5
    #define VBI_CC_END1 Fld(5, 0, AC_MSKB0) //4:0
#define VBI_06 (VBI_IO_BASE+0x0018)
  #define VBI_06_0 (VBI_IO_BASE+0x0018)
  #define VBI_06_1 (VBI_IO_BASE+0x0019)
  #define VBI_06_2 (VBI_IO_BASE+0x001a)
  #define VBI_06_3 (VBI_IO_BASE+0x001b)
    #define VBI_CC_DC_SRC Fld(1, 31, AC_MSKB3) //31
    #define VBI_CC_LEN Fld(7, 24, AC_MSKB3) //30:24
    #define VBI_CC_STCODE Fld(8, 16, AC_FULLB2) //23:16
    #define VBI_CC_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI_07 (VBI_IO_BASE+0x001c)
  #define VBI_07_0 (VBI_IO_BASE+0x001c)
  #define VBI_07_1 (VBI_IO_BASE+0x001d)
  #define VBI_07_2 (VBI_IO_BASE+0x001e)
  #define VBI_07_3 (VBI_IO_BASE+0x001f)
    #define VBI_VPS_DRAM_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI_VPS_LPF_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI_VPS_DC_SEL Fld(1, 29, AC_MSKB3) //29
    #define VBI_VPS_INIT_SEL Fld(1, 28, AC_MSKB3) //28
    #define VBI_DDET_SEL_1 Fld(1, 27, AC_MSKB3) //27
    #define VBI_VPS_STC_END Fld(7, 20, AC_MSKW32) //26:20
    #define VBI_VPS_START1 Fld(5, 15, AC_MSKW21) //19:15
    #define VBI_VPS_END1 Fld(5, 10, AC_MSKB1) //14:10
    #define VBI_VPS_START0 Fld(5, 5, AC_MSKW10) //9:5
    #define VBI_VPS_END0 Fld(5, 0, AC_MSKB0) //4:0
#define VBI_08 (VBI_IO_BASE+0x0020)
  #define VBI_08_0 (VBI_IO_BASE+0x0020)
  #define VBI_08_1 (VBI_IO_BASE+0x0021)
  #define VBI_08_2 (VBI_IO_BASE+0x0022)
  #define VBI_08_3 (VBI_IO_BASE+0x0023)
    #define VBI_DDET_SEL_0 Fld(1, 31, AC_MSKB3) //31
    #define VBI_VPS_LEN Fld(7, 24, AC_MSKB3) //30:24
    #define VBI_VPS_STCODE Fld(8, 16, AC_FULLB2) //23:16
    #define VBI_VPS_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI_09 (VBI_IO_BASE+0x0024)
  #define VBI_09_0 (VBI_IO_BASE+0x0024)
  #define VBI_09_1 (VBI_IO_BASE+0x0025)
  #define VBI_09_2 (VBI_IO_BASE+0x0026)
  #define VBI_09_3 (VBI_IO_BASE+0x0027)
    #define VBI_WSS_DRAM_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI_WSS_LPF_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI_WSS_DC_SEL Fld(1, 29, AC_MSKB3) //29
    #define VBI_WSS_INIT_SEL Fld(1, 28, AC_MSKB3) //28
    #define VBI_WSS_RUNIN_EN Fld(1, 27, AC_MSKB3) //27
    #define VBI_WSS_CRISEL Fld(1, 26, AC_MSKB3) //26
    #define VBI_WSS625_STSEL Fld(1, 25, AC_MSKB3) //25
    #define VBI_WSS_LPF_SEL Fld(1, 24, AC_MSKB3) //24
    #define VBI_WSS625_STCHK_SEL Fld(1, 23, AC_MSKB2) //23
    #define VBI_WSS_STC_END Fld(7, 16, AC_MSKB2) //22:16
    #define VBI_WSS625_BSTART_SEL Fld(1, 15, AC_MSKB1) //15
    #define VBI_WSS625_STRICT_RUNIN Fld(1, 14, AC_MSKB1) //14
    #define VBI_WSS625_BB_SEL Fld(1, 13, AC_MSKB1) //13
    #define VBI_WSS625_F1_SEL Fld(1, 12, AC_MSKB1) //12
    #define VBI_WSS625_LERR_EN Fld(1, 11, AC_MSKB1) //11
    #define VBI_WSS625_IGNORE_AMBI Fld(1, 10, AC_MSKB1) //10
    #define VBI_WSS_SEL0 Fld(5, 5, AC_MSKW10) //9:5
    #define VBI_WSS_SEL1 Fld(5, 0, AC_MSKB0) //4:0
#define VBI_0A (VBI_IO_BASE+0x0028)
  #define VBI_0A_0 (VBI_IO_BASE+0x0028)
  #define VBI_0A_1 (VBI_IO_BASE+0x0029)
  #define VBI_0A_2 (VBI_IO_BASE+0x002a)
  #define VBI_0A_3 (VBI_IO_BASE+0x002b)
    #define VBI_WSS_PSCAN Fld(1, 31, AC_MSKB3) //31
    #define VBI_WSS_LEN Fld(7, 24, AC_MSKB3) //30:24
    #define VBI_WSS_STCODE Fld(8, 16, AC_FULLB2) //23:16
    #define VBI_WSS_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI_0B (VBI_IO_BASE+0x002c)
  #define VBI_0B_0 (VBI_IO_BASE+0x002c)
  #define VBI_0B_1 (VBI_IO_BASE+0x002d)
  #define VBI_0B_2 (VBI_IO_BASE+0x002e)
  #define VBI_0B_3 (VBI_IO_BASE+0x002f)
    #define VBI_GRPDLY_CMP_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI_TT_LPF_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI_TT_DC_SEL Fld(1, 29, AC_MSKB3) //29
    #define VBI_TT_INIT_SEL Fld(1, 28, AC_MSKB3) //28
    #define VBI_GRPDLY_CMP_RST Fld(1, 27, AC_MSKB3) //27
    #define VBI_TT_STC_END Fld(7, 20, AC_MSKW32) //26:20
    #define VBI_TT_START1 Fld(5, 15, AC_MSKW21) //19:15
    #define VBI_TT_END1 Fld(5, 10, AC_MSKB1) //14:10
    #define VBI_TT_START0 Fld(5, 5, AC_MSKW10) //9:5
    #define VBI_TT_END0 Fld(5, 0, AC_MSKB0) //4:0
#define VBI_0C (VBI_IO_BASE+0x0030)
  #define VBI_0C_0 (VBI_IO_BASE+0x0030)
  #define VBI_0C_1 (VBI_IO_BASE+0x0031)
  #define VBI_0C_2 (VBI_IO_BASE+0x0032)
  #define VBI_0C_3 (VBI_IO_BASE+0x0033)
    #define VBI_TT_LEN Fld(7, 24, AC_MSKB3) //30:24
    #define VBI_TT_STCODE Fld(8, 16, AC_FULLB2) //23:16
    #define VBI_TT_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI_0D (VBI_IO_BASE+0x0034)
  #define VBI_0D_0 (VBI_IO_BASE+0x0034)
  #define VBI_0D_1 (VBI_IO_BASE+0x0035)
  #define VBI_0D_2 (VBI_IO_BASE+0x0036)
  #define VBI_0D_3 (VBI_IO_BASE+0x0037)
    #define VBI_TVD_GAIN Fld(4, 28, AC_MSKB3) //31:28
    #define VBI_TT_FAST_ZC Fld(1, 27, AC_MSKB3) //27
    #define VBI_TT_SLICE_SEL Fld(1, 26, AC_MSKB3) //26
    #define VBI_GRP_LIMIT Fld(2, 24, AC_MSKB3) //25:24
    #define VBI_BEGIN_DET Fld(7, 17, AC_MSKB2) //23:17
    #define VBI_RISC_RST Fld(1, 16, AC_MSKB2) //16
    #define VBI_CEDGE_TH Fld(4, 12, AC_MSKB1) //15:12
    #define VBI_YEDGE_TH Fld(4, 8, AC_MSKB1) //11:8
    #define VBI_ADAPTIVE_INIT_ZC Fld(1, 7, AC_MSKB0) //7
    #define VBI_GRPDLY_BW_SEL Fld(1, 6, AC_MSKB0) //6
    #define VBI_WAND_YC Fld(1, 5, AC_MSKB0) //5
    #define VBI_YEDGE_EN Fld(1, 4, AC_MSKB0) //4
    #define VBI_BLANK_ZC_THR Fld(4, 0, AC_MSKB0) //3:0
#define VBI_0E (VBI_IO_BASE+0x0038)
  #define VBI_0E_0 VBI_IO_BASE+0x0038
  #define VBI_0E_1 VBI_IO_BASE+0x0039
  #define VBI_0E_2 VBI_IO_BASE+0x003a
  #define VBI_0E_3 VBI_IO_BASE+0x003b
    #define VBI_MFIFORST1 Fld(1, 31, AC_MSKB3) //31
    #define VBI2_MOFFSET1 Fld(2, 6, AC_MSKB0) //7:6
    #define VBI2_MBASE01 Fld(2, 4, AC_MSKB0) //5:4
    #define VBI_MOFFSET1 Fld(2, 2, AC_MSKB0) //3:2
    #define VBI_MBASE01 Fld(2, 0, AC_MSKB0) //1:0
#define VBI2_00 (VBI_IO_BASE+0x0040)
  #define VBI2_00_0 (VBI_IO_BASE+0x0040)
  #define VBI2_00_1 (VBI_IO_BASE+0x0041)
  #define VBI2_00_2 (VBI_IO_BASE+0x0042)
  #define VBI2_00_3 (VBI_IO_BASE+0x0043)
    #define VBI2_VPS_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI2_CC_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI2_WSS_EN Fld(1, 29, AC_MSKB3) //29
    #define VBI2_TT_EN Fld(1, 28, AC_MSKB3) //28
    #define VBI2_ZC_THR Fld(8, 20, AC_MSKW32) //27:20
    #define VBI2_THR_DET_WIDTH Fld(6, 14, AC_MSKW21) //19:14
    #define VBI2_PE_GAIN Fld(2, 12, AC_MSKB1) //13:12
    #define VBI2_CC_ECC_EN Fld(1, 11, AC_MSKB1) //11
    #define VBI2_TT_ECC_EN Fld(1, 10, AC_MSKB1) //10
    #define VBI2_TT_ECC_SEL Fld(1, 9, AC_MSKB1) //9
    #define VBI2_ERR_SEL Fld(1, 8, AC_MSKB1) //8
    #define VBI2_ERR_RST Fld(1, 7, AC_MSKB0) //7
    #define VBI2_DC_OFFSET Fld(3, 4, AC_MSKB0) //6:4
    #define VBI2_PROBLEM_TH Fld(4, 0, AC_MSKB0) //3:0
#define VBI2_01 (VBI_IO_BASE+0x0044)
  #define VBI2_01_0 (VBI_IO_BASE+0x0044)
  #define VBI2_01_1 (VBI_IO_BASE+0x0045)
  #define VBI2_01_2 (VBI_IO_BASE+0x0046)
  #define VBI2_01_3 (VBI_IO_BASE+0x0047)
    #define VBI2_PSCAN Fld(1, 31, AC_MSKB3) //31
    #define VBI2_ALL_ADAPTIVE Fld(1, 30, AC_MSKB3) //30
    #define VBI2_PDET_END Fld(6, 24, AC_MSKB3) //29:24
    #define VBI2_ERR_POW_SEL Fld(1, 23, AC_MSKB2) //23
    #define VBI2_ERR_NO_ZERO Fld(1, 22, AC_MSKB2) //22
    #define VBI2_ECC_SEL3 Fld(1, 21, AC_MSKB2) //21
    #define VBI2_ECC_SEL2 Fld(1, 20, AC_MSKB2) //20
    #define VBI2_CNI0_STA_SEL Fld(1, 19, AC_MSKB2) //19
    #define VBI2_VSRC Fld(2, 15, AC_MSKW21) //16:15
    #define VBI2_TT_LINE_INFO Fld(1, 14, AC_MSKB1) //14
    #define VBI2_DIS_OFFSET Fld(1, 13, AC_MSKB1) //13
    #define VBI2_STC_ERR Fld(1, 12, AC_MSKB1) //12
    #define VBI2_TT_ZC_SEL Fld(1, 11, AC_MSKB1) //11
    #define VBI2_IMODE Fld(3, 7, AC_MSKW10) //9:7
    #define VBI2_INIT_ZC Fld(1, 5, AC_MSKB0) //5
    #define VBI2_BLANK_EN Fld(1, 4, AC_MSKB0) //4
    #define VBI2_DEBUG_SEL Fld(2, 2, AC_MSKB0) //3:2
    #define VBI2_CLR_OVF Fld(1, 1, AC_MSKB0) //1
    #define VBI2_TT_STC_ERR_ONLY Fld(1, 0, AC_MSKB0) //0
#define VBI2_02 (VBI_IO_BASE+0x0048)
  #define VBI2_02_0 (VBI_IO_BASE+0x0048)
  #define VBI2_02_1 (VBI_IO_BASE+0x0049)
  #define VBI2_02_2 (VBI_IO_BASE+0x004a)
  #define VBI2_02_3 (VBI_IO_BASE+0x004b)
    #define VBI2_POW_LINE_SEL Fld(7, 25, AC_MSKB3) //31:25
    #define VBI2_MBASE0 Fld(25, 0, AC_MSKDW) //24:0
#define VBI2_03 (VBI_IO_BASE+0x004c)
  #define VBI2_03_0 (VBI_IO_BASE+0x004c)
  #define VBI2_03_1 (VBI_IO_BASE+0x004d)
  #define VBI2_03_2 (VBI_IO_BASE+0x004e)
  #define VBI2_03_3 (VBI_IO_BASE+0x004f)
    #define VBI2_LSC Fld(8, 24, AC_FULLB3) //31:24
    #define VBI2_RUNIN_WIDTH Fld(4, 20, AC_MSKB2) //23:20
    #define VBI2_RUNIN_SEL Fld(1, 19, AC_MSKB2) //19
    #define VBI2_STC_PTHR Fld(3, 16, AC_MSKB2) //18:16
    #define VBI2_STC_PROBLEM Fld(1, 15, AC_MSKB1) //15
    #define VBI2_POW_DIFF Fld(3, 12, AC_MSKB1) //14:12
    #define VBI2_DDET_WIN Fld(2, 10, AC_MSKB1) //11:10
    #define VBI2_CC2X Fld(1, 9, AC_MSKB1) //9
    #define VBI2_DEBUG_SEL2 Fld(1, 8, AC_MSKB1) //8
    #define VBI2_ANCI_EN Fld(1, 4, AC_MSKB0) //4
    #define VBI2_ANCI_MODE Fld(2, 2, AC_MSKB0) //3:2
    #define VBI2_ANCI_HEADER_LSB8 Fld(1, 1, AC_MSKB0) //1
    #define VBI2_ANCI_LINE_WRITE Fld(1, 0, AC_MSKB0) //0
#define VBI2_04 (VBI_IO_BASE+0x0050)
  #define VBI2_04_0 (VBI_IO_BASE+0x0050)
  #define VBI2_04_1 (VBI_IO_BASE+0x0051)
  #define VBI2_04_2 (VBI_IO_BASE+0x0052)
  #define VBI2_04_3 (VBI_IO_BASE+0x0053)
    #define VBI2_OMODE Fld(1, 31, AC_MSKB3) //31
    #define VBI2_MFIFORST Fld(1, 30, AC_MSKB3) //30
    #define VBI2_MFIFO Fld(1, 29, AC_MSKB3) //29
    #define VBI2_POW_END Fld(4, 25, AC_MSKB3) //28:25
    #define VBI2_MOFFSET Fld(25, 0, AC_MSKDW) //24:0
#define VBI2_05 (VBI_IO_BASE+0x0054)
  #define VBI2_05_0 (VBI_IO_BASE+0x0054)
  #define VBI2_05_1 (VBI_IO_BASE+0x0055)
  #define VBI2_05_2 (VBI_IO_BASE+0x0056)
  #define VBI2_05_3 (VBI_IO_BASE+0x0057)
    #define VBI2_CC_DRAM_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI2_CC_LPF_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI2_CC_DC_SEL Fld(1, 29, AC_MSKB3) //29
    #define VBI2_CC_INIT_SEL Fld(1, 28, AC_MSKB3) //28
    #define VBI2_CC_SLICE_AVG Fld(1, 27, AC_MSKB3) //27
    #define VBI2_CC_STC_END Fld(7, 20, AC_MSKW32) //26:20
    #define VBI2_CC_START0 Fld(5, 15, AC_MSKW21) //19:15
    #define VBI2_CC_END0 Fld(5, 10, AC_MSKB1) //14:10
    #define VBI2_CC_START1 Fld(5, 5, AC_MSKW10) //9:5
    #define VBI2_CC_END1 Fld(5, 0, AC_MSKB0) //4:0
#define VBI2_06 (VBI_IO_BASE+0x0058)
  #define VBI2_06_0 (VBI_IO_BASE+0x0058)
  #define VBI2_06_1 (VBI_IO_BASE+0x0059)
  #define VBI2_06_2 (VBI_IO_BASE+0x005a)
  #define VBI2_06_3 (VBI_IO_BASE+0x005b)
    #define VBI2_CC_DC_SRC Fld(1, 31, AC_MSKB3) //31
    #define VBI2_CC_LEN Fld(7, 24, AC_MSKB3) //30:24
    #define VBI2_CC_STCODE Fld(8, 16, AC_FULLB2) //23:16
    #define VBI2_CC_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI2_07 (VBI_IO_BASE+0x005c)
  #define VBI2_07_0 (VBI_IO_BASE+0x005c)
  #define VBI2_07_1 (VBI_IO_BASE+0x005d)
  #define VBI2_07_2 (VBI_IO_BASE+0x005e)
  #define VBI2_07_3 (VBI_IO_BASE+0x005f)
    #define VBI2_VPS_DRAM_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI2_VPS_LPF_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI2_VPS_DC_SEL Fld(1, 29, AC_MSKB3) //29
    #define VBI2_VPS_INIT_SEL Fld(1, 28, AC_MSKB3) //28
    #define VBI2_DDET_SEL_1 Fld(1, 27, AC_MSKB3) //27
    #define VBI2_VPS_STC_END Fld(7, 20, AC_MSKW32) //26:20
    #define VBI2_VPS_START1 Fld(5, 15, AC_MSKW21) //19:15
    #define VBI2_VPS_END1 Fld(5, 10, AC_MSKB1) //14:10
    #define VBI2_VPS_START0 Fld(5, 5, AC_MSKW10) //9:5
    #define VBI2_VPS_END0 Fld(5, 0, AC_MSKB0) //4:0
#define VBI2_08 (VBI_IO_BASE+0x0060)
  #define VBI2_08_0 (VBI_IO_BASE+0x0060)
  #define VBI2_08_1 (VBI_IO_BASE+0x0061)
  #define VBI2_08_2 (VBI_IO_BASE+0x0062)
  #define VBI2_08_3 (VBI_IO_BASE+0x0063)
    #define VBI2_DDET_SEL_0 Fld(1, 31, AC_MSKB3) //31
    #define VBI2_VPS_LEN Fld(7, 24, AC_MSKB3) //30:24
    #define VBI2_VPS_STCODE Fld(8, 16, AC_FULLB2) //23:16
    #define VBI2_VPS_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI2_09 (VBI_IO_BASE+0x0064)
  #define VBI2_09_0 (VBI_IO_BASE+0x0064)
  #define VBI2_09_1 (VBI_IO_BASE+0x0065)
  #define VBI2_09_2 (VBI_IO_BASE+0x0066)
  #define VBI2_09_3 (VBI_IO_BASE+0x0067)
    #define VBI2_WSS_DRAM_EN Fld(1, 31, AC_MSKB3) //31
    #define VBI2_WSS_LPF_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI2_WSS_DC_SEL Fld(1, 29, AC_MSKB3) //29
    #define VBI2_WSS_INIT_SEL Fld(1, 28, AC_MSKB3) //28
    #define VBI2_WSS_RUNIN_EN Fld(1, 27, AC_MSKB3) //27
    #define VBI2_WSS_CRISEL Fld(1, 26, AC_MSKB3) //26
    #define VBI2_WSS625_STSEL Fld(1, 25, AC_MSKB3) //25
    #define VBI2_WSS_LPF_SEL Fld(1, 24, AC_MSKB3) //24
    #define VBI2_WSS625_STCHK_SEL Fld(1, 23, AC_MSKB2) //23
    #define VBI2_WSS_STC_END Fld(7, 16, AC_MSKB2) //22:16
    #define VBI2_WSS625_BSTART_SEL Fld(1, 15, AC_MSKB1) //15
    #define VBI2_WSS625_STRICT_RUNIN Fld(1, 14, AC_MSKB1) //14
    #define VBI2_WSS625_BB_SEL Fld(1, 13, AC_MSKB1) //13
    #define VBI2_WSS625_F1_SEL Fld(1, 12, AC_MSKB1) //12
    #define VBI2_WSS625_LERR_EN Fld(1, 11, AC_MSKB1) //11
    #define VBI2_WSS625_IGNORE_AMBI Fld(1, 10, AC_MSKB1) //10
    #define VBI2_WSS_SEL0 Fld(5, 5, AC_MSKW10) //9:5
    #define VBI2_WSS_SEL1 Fld(5, 0, AC_MSKB0) //4:0
#define VBI2_0A (VBI_IO_BASE+0x0068)
  #define VBI2_0A_0 (VBI_IO_BASE+0x0068)
  #define VBI2_0A_1 (VBI_IO_BASE+0x0069)
  #define VBI2_0A_2 (VBI_IO_BASE+0x006a)
  #define VBI2_0A_3 (VBI_IO_BASE+0x006b)
    #define VBI2_WSS_PSCAN Fld(1, 31, AC_MSKB3) //31
    #define VBI2_WSS_LEN Fld(7, 24, AC_MSKB3) //30:24
    #define VBI2_WSS_STCODE Fld(8, 16, AC_FULLB2) //23:16
    #define VBI2_WSS_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI2_0B (VBI_IO_BASE+0x006c)
  #define VBI2_0B_0 (VBI_IO_BASE+0x006c)
  #define VBI2_0B_1 (VBI_IO_BASE+0x006d)
  #define VBI2_0B_2 (VBI_IO_BASE+0x006e)
  #define VBI2_0B_3 (VBI_IO_BASE+0x006f)
    #define VBI2_TT_LPF_EN Fld(1, 30, AC_MSKB3) //30
    #define VBI2_TT_DC_SEL Fld(1, 29, AC_MSKB3) //29
    #define VBI2_TT_INIT_SEL Fld(1, 28, AC_MSKB3) //28
    #define VBI2_TT_STC_END Fld(7, 20, AC_MSKW32) //26:20
    #define VBI2_TT_START1 Fld(5, 15, AC_MSKW21) //19:15
    #define VBI2_TT_END1 Fld(5, 10, AC_MSKB1) //14:10
    #define VBI2_TT_START0 Fld(5, 5, AC_MSKW10) //9:5
    #define VBI2_TT_END0 Fld(5, 0, AC_MSKB0) //4:0
#define VBI2_0C (VBI_IO_BASE+0x0070)
  #define VBI2_0C_0 (VBI_IO_BASE+0x0070)
  #define VBI2_0C_1 (VBI_IO_BASE+0x0071)
  #define VBI2_0C_2 (VBI_IO_BASE+0x0072)
  #define VBI2_0C_3 (VBI_IO_BASE+0x0073)
    #define VBI2_TT_LEN Fld(7, 24, AC_MSKB3) //30:24
    #define VBI2_TT_STCODE Fld(8, 16, AC_FULLB2) //23:16
    #define VBI2_TT_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI2_0D (VBI_IO_BASE+0x0074)
  #define VBI2_0D_0 (VBI_IO_BASE+0x0074)
  #define VBI2_0D_1 (VBI_IO_BASE+0x0075)
  #define VBI2_0D_2 (VBI_IO_BASE+0x0076)
  #define VBI2_0D_3 (VBI_IO_BASE+0x0077)
    #define VBI2_TT_FAST_ZC Fld(1, 27, AC_MSKB3) //27
    #define VBI2_TT_SLICE_SEL Fld(1, 26, AC_MSKB3) //26
    #define VBI2_BEGIN_DET Fld(7, 17, AC_MSKB2) //23:17
    #define VBI2_RISC_RST Fld(1, 16, AC_MSKB2) //16
    #define VBI2_CEDGE_TH Fld(4, 12, AC_MSKB1) //15:12
    #define VBI2_YEDGE_TH Fld(4, 8, AC_MSKB1) //11:8
    #define VBI2_ADAPTIVE_INIT_ZC Fld(1, 7, AC_MSKB0) //7
    #define VBI2_WAND_YC Fld(1, 5, AC_MSKB0) //5
    #define VBI2_YEDGE_EN Fld(1, 4, AC_MSKB0) //4
    #define VBI2_BLANK_ZC_THR Fld(4, 0, AC_MSKB0) //3:0
#define VBI_STA_00 (VBI_STA_IO_BASE+0x0000)
  #define VBI_STA_00_0 (VBI_STA_IO_BASE+0x0000)
  #define VBI_STA_00_1 (VBI_STA_IO_BASE+0x0001)
  #define VBI_STA_00_2 (VBI_STA_IO_BASE+0x0002)
  #define VBI_STA_00_3 (VBI_STA_IO_BASE+0x0003)
    #define STA_VBI_ADR Fld(25, 7, AC_MSKDW) //31:7
    #define STA_VBI_CNI_RDY Fld(1, 6, AC_MSKB0) //6
    #define STA_VBI_RDY Fld(1, 5, AC_MSKB0) //5
    #define STA_VBI_FIELD Fld(1, 4, AC_MSKB0) //4
    #define STA_VBI_CC_RDY Fld(1, 3, AC_MSKB0) //3
    #define STA_VBI_WSS_RDY Fld(1, 2, AC_MSKB0) //2
    #define STA_VBI_VPS_RDY Fld(1, 1, AC_MSKB0) //1
    #define STA_VBI_TT_RDY Fld(1, 0, AC_MSKB0) //0
#define VBI_STA_01 (VBI_STA_IO_BASE+0x0004)
  #define VBI_STA_01_0 (VBI_STA_IO_BASE+0x0004)
  #define VBI_STA_01_1 (VBI_STA_IO_BASE+0x0005)
  #define VBI_STA_01_2 (VBI_STA_IO_BASE+0x0006)
  #define VBI_STA_01_3 (VBI_STA_IO_BASE+0x0007)
    #define STA_VBI_W_0 Fld(11, 21, AC_MSKW32) //31:21
    #define STA_VBI_FOVF Fld(1, 20, AC_MSKB2) //20
    #define STA_VBI_WSS_OUT Fld(20, 0, AC_MSKDW) //19:0
#define VBI_STA_02 (VBI_STA_IO_BASE+0x0008)
  #define VBI_STA_02_0 (VBI_STA_IO_BASE+0x0008)
  #define VBI_STA_02_1 (VBI_STA_IO_BASE+0x0009)
  #define VBI_STA_02_2 (VBI_STA_IO_BASE+0x000a)
  #define VBI_STA_02_3 (VBI_STA_IO_BASE+0x000b)
    #define STA_VBI_VPS_OUT_0 Fld(32, 0, AC_FULLDW) //31:0
#define VBI_STA_03 (VBI_STA_IO_BASE+0x000c)
  #define VBI_STA_03_0 (VBI_STA_IO_BASE+0x000c)
  #define VBI_STA_03_1 (VBI_STA_IO_BASE+0x000d)
  #define VBI_STA_03_2 (VBI_STA_IO_BASE+0x000e)
  #define VBI_STA_03_3 (VBI_STA_IO_BASE+0x000f)
    #define STA_VBI_VPS_OUT_1 Fld(32, 0, AC_FULLDW) //31:0
#define VBI_STA_04 (VBI_STA_IO_BASE+0x0010)
  #define VBI_STA_04_0 (VBI_STA_IO_BASE+0x0010)
  #define VBI_STA_04_1 (VBI_STA_IO_BASE+0x0011)
  #define VBI_STA_04_2 (VBI_STA_IO_BASE+0x0012)
  #define VBI_STA_04_3 (VBI_STA_IO_BASE+0x0013)
    #define STA_VBI_VPS_OUT_2 Fld(32, 0, AC_FULLDW) //31:0
#define VBI_STA_05 (VBI_STA_IO_BASE+0x0014)
  #define VBI_STA_05_0 (VBI_STA_IO_BASE+0x0014)
  #define VBI_STA_05_1 (VBI_STA_IO_BASE+0x0015)
  #define VBI_STA_05_2 (VBI_STA_IO_BASE+0x0016)
  #define VBI_STA_05_3 (VBI_STA_IO_BASE+0x0017)
    #define STA_VBI_CC_OUT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_VBI_CC_OUT_B1 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI_CC_OUT_B0 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI_CNI_OUT_1 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI_VPS_OUT_3 Fld(8, 0, AC_FULLB0) //7:0
#define VBI_STA_06 (VBI_STA_IO_BASE+0x0018)
  #define VBI_STA_06_0 (VBI_STA_IO_BASE+0x0018)
  #define VBI_STA_06_1 (VBI_STA_IO_BASE+0x0019)
  #define VBI_STA_06_2 (VBI_STA_IO_BASE+0x001a)
  #define VBI_STA_06_3 (VBI_STA_IO_BASE+0x001b)
    #define STA_VBI_CNI_OUT_0 Fld(32, 0, AC_FULLDW) //31:0
#define VBI_STA_07 (VBI_STA_IO_BASE+0x001c)
  #define VBI_STA_07_0 (VBI_STA_IO_BASE+0x001c)
  #define VBI_STA_07_1 (VBI_STA_IO_BASE+0x001d)
  #define VBI_STA_07_2 (VBI_STA_IO_BASE+0x001e)
  #define VBI_STA_07_3 (VBI_STA_IO_BASE+0x001f)
    #define STA_VBI_CNI0_OUT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_VBI_DEBUG_OUT Fld(16, 0, AC_FULLW10) //15:0
#define VBI2_STA_00 (VBI_STA_IO_BASE+0x0040)
  #define VBI2_STA_00_0 (VBI_STA_IO_BASE+0x0040)
  #define VBI2_STA_00_1 (VBI_STA_IO_BASE+0x0041)
  #define VBI2_STA_00_2 (VBI_STA_IO_BASE+0x0042)
  #define VBI2_STA_00_3 (VBI_STA_IO_BASE+0x0043)
    #define STA_VBI2_ADR Fld(25, 7, AC_MSKDW) //31:7
    #define STA_VBI2_CNI_RDY Fld(1, 6, AC_MSKB0) //6
    #define STA_VBI2_RDY Fld(1, 5, AC_MSKB0) //5
    #define STA_VBI2_FIELD Fld(1, 4, AC_MSKB0) //4
    #define STA_VBI2_CC_RDY Fld(1, 3, AC_MSKB0) //3
    #define STA_VBI2_WSS_RDY Fld(1, 2, AC_MSKB0) //2
    #define STA_VBI2_VPS_RDY Fld(1, 1, AC_MSKB0) //1
    #define STA_VBI2_TT_RDY Fld(1, 0, AC_MSKB0) //0
#define VBI2_STA_01 (VBI_STA_IO_BASE+0x0044)
  #define VBI2_STA_01_0 (VBI_STA_IO_BASE+0x0044)
  #define VBI2_STA_01_1 (VBI_STA_IO_BASE+0x0045)
  #define VBI2_STA_01_2 (VBI_STA_IO_BASE+0x0046)
  #define VBI2_STA_01_3 (VBI_STA_IO_BASE+0x0047)
    #define STA_VBI2_W_0 Fld(11, 21, AC_MSKW32) //31:21
    #define STA_VBI2_FOVF Fld(1, 20, AC_MSKB2) //20
    #define STA_VBI2_WSS_OUT Fld(20, 0, AC_MSKDW) //19:0
#define VBI2_STA_02 (VBI_STA_IO_BASE+0x0048)
  #define VBI2_STA_02_0 (VBI_STA_IO_BASE+0x0048)
  #define VBI2_STA_02_1 (VBI_STA_IO_BASE+0x0049)
  #define VBI2_STA_02_2 (VBI_STA_IO_BASE+0x004a)
  #define VBI2_STA_02_3 (VBI_STA_IO_BASE+0x004b)
    #define STA_VBI2_VPS_OUT_0 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI2_VPS_OUT_B3 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI2_VPS_OUT_B2 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI2_VPS_OUT_B1 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI2_VPS_OUT_B0 Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_STA_03 (VBI_STA_IO_BASE+0x004c)
  #define VBI2_STA_03_0 (VBI_STA_IO_BASE+0x004c)
  #define VBI2_STA_03_1 (VBI_STA_IO_BASE+0x004d)
  #define VBI2_STA_03_2 (VBI_STA_IO_BASE+0x004e)
  #define VBI2_STA_03_3 (VBI_STA_IO_BASE+0x004f)
    #define STA_VBI2_VPS_OUT_1 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI2_VPS_OUT_B7 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI2_VPS_OUT_B6 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI2_VPS_OUT_B5 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI2_VPS_OUT_B4 Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_STA_04 (VBI_STA_IO_BASE+0x0050)
  #define VBI2_STA_04_0 (VBI_STA_IO_BASE+0x0050)
  #define VBI2_STA_04_1 (VBI_STA_IO_BASE+0x0051)
  #define VBI2_STA_04_2 (VBI_STA_IO_BASE+0x0052)
  #define VBI2_STA_04_3 (VBI_STA_IO_BASE+0x0053)
    #define STA_VBI2_VPS_OUT_2 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI2_VPS_OUT_B11 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI2_VPS_OUT_B10 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI2_VPS_OUT_B9 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI2_VPS_OUT_B8 Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_STA_05 (VBI_STA_IO_BASE+0x0054)
  #define VBI2_STA_05_0 (VBI_STA_IO_BASE+0x0054)
  #define VBI2_STA_05_1 (VBI_STA_IO_BASE+0x0055)
  #define VBI2_STA_05_2 (VBI_STA_IO_BASE+0x0056)
  #define VBI2_STA_05_3 (VBI_STA_IO_BASE+0x0057)
    #define STA_VBI2_CC_OUT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_VBI2_CC_OUT_B1 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI2_CC_OUT_B0 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI2_CNI_OUT_1 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI2_VPS_OUT_3 Fld(8, 0, AC_FULLB0) //7:0
    #define STA_VBI2_VPS_OUT_B12 Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_STA_06 (VBI_STA_IO_BASE+0x0058)
  #define VBI2_STA_06_0 (VBI_STA_IO_BASE+0x0058)
  #define VBI2_STA_06_1 (VBI_STA_IO_BASE+0x0059)
  #define VBI2_STA_06_2 (VBI_STA_IO_BASE+0x005a)
  #define VBI2_STA_06_3 (VBI_STA_IO_BASE+0x005b)
    #define STA_VBI2_CNI_OUT_0 Fld(32, 0, AC_FULLDW) //31:0
#define VBI2_STA_07 (VBI_STA_IO_BASE+0x005c)
  #define VBI2_STA_07_0 (VBI_STA_IO_BASE+0x005c)
  #define VBI2_STA_07_1 (VBI_STA_IO_BASE+0x005d)
  #define VBI2_STA_07_2 (VBI_STA_IO_BASE+0x005e)
  #define VBI2_STA_07_3 (VBI_STA_IO_BASE+0x005f)
    #define STA_VBI2_CNI0_OUT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_VBI2_DEBUG_OUT Fld(16, 0, AC_FULLW10) //15:0

#if SUPPORT_TYPE_B_VBI
#define VBI_TYPE_B_00 (VBI_IO_BASE+0x0080)
  #define VBI_TYPE_B_00_0 (VBI_IO_BASE+0x0080)
  #define VBI_TYPE_B_00_1 (VBI_IO_BASE+0x0081)
  #define VBI_TYPE_B_00_2 (VBI_IO_BASE+0x0082)
  #define VBI_TYPE_B_00_3 (VBI_IO_BASE+0x0083)
    #define VBI_B_LSC_MSB Fld(1, 31, AC_MSKB3) //31
    #define VBI_B_HD_WSS_EN Fld(1, 28, AC_MSKB3) //28
    #define VBI_B_HD_DC_SEL Fld(1, 27, AC_MSKB3) //27
    #define VBI_B_HD_INIT_SEL Fld(1, 26, AC_MSKB3) //26
    #define VBI_B_HD_LINE_SEL1 Fld(5, 21, AC_MSKW32) //25:21
    #define VBI_B_HD_LINE_SEL0 Fld(5, 16, AC_MSKB2) //20:16
    #define VBI_B_HD_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI_TYPE_B_01 (VBI_IO_BASE+0x0084)
  #define VBI_TYPE_B_01_0 (VBI_IO_BASE+0x0084)
  #define VBI_TYPE_B_01_1 (VBI_IO_BASE+0x0085)
  #define VBI_TYPE_B_01_2 (VBI_IO_BASE+0x0086)
  #define VBI_TYPE_B_01_3 (VBI_IO_BASE+0x0087)
    #define VBI_B_HD_RUNIN_EN Fld(1, 27, AC_MSKB3) //27
    #define VBI_B_HD_LPF_EN Fld(1, 26, AC_MSKB3) //26
    #define VBI_B_HD_LPF_SEL Fld(1, 25, AC_MSKB3) //25
    #define VBI_B_HD_DRAM_EN Fld(1, 24, AC_MSKB3) //24
    #define VBI_B_HD_SAMPLE_SEL Fld(2, 22, AC_MSKB2) //23:22
    #define VBI_B_HD_LEN Fld(7, 15, AC_MSKW21) //21:15
    #define VBI_B_STC_END Fld(7, 8, AC_MSKB1) //14:8
    #define VBI_B_STC Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_TYPE_B_00 (VBI_IO_BASE+0x0088)
  #define VBI2_TYPE_B_00_0 (VBI_IO_BASE+0x0088)
  #define VBI2_TYPE_B_00_1 (VBI_IO_BASE+0x0089)
  #define VBI2_TYPE_B_00_2 (VBI_IO_BASE+0x008a)
  #define VBI2_TYPE_B_00_3 (VBI_IO_BASE+0x008b)
    #define VBI2_B_LSC_MSB Fld(1, 31, AC_MSKB3) //31
    #define VBI2_B_HD_WSS_EN Fld(1, 28, AC_MSKB3) //28
    #define VBI2_B_HD_DC_SEL Fld(1, 27, AC_MSKB3) //27
    #define VBI2_B_HD_INIT_SEL Fld(1, 26, AC_MSKB3) //26
    #define VBI2_B_HD_LINE_SEL1 Fld(5, 21, AC_MSKW32) //25:21
    #define VBI2_B_HD_LINE_SEL0 Fld(5, 16, AC_MSKB2) //20:16
    #define VBI2_B_HD_FREQ Fld(16, 0, AC_FULLW10) //15:0
#define VBI2_TYPE_B_01 (VBI_IO_BASE+0x008c)
  #define VBI2_TYPE_B_01_0 (VBI_IO_BASE+0x008c)
  #define VBI2_TYPE_B_01_1 (VBI_IO_BASE+0x008d)
  #define VBI2_TYPE_B_01_2 (VBI_IO_BASE+0x008e)
  #define VBI2_TYPE_B_01_3 (VBI_IO_BASE+0x008f)
    #define VBI2_B_HD_RUNIN_EN Fld(1, 27, AC_MSKB3) //27
    #define VBI2_B_HD_LPF_EN Fld(1, 26, AC_MSKB3) //26
    #define VBI2_B_HD_LPF_SEL Fld(1, 25, AC_MSKB3) //25
    #define VBI2_B_HD_DRAM_EN Fld(1, 24, AC_MSKB3) //24
    #define VBI2_B_HD_SAMPLE_SEL Fld(2, 22, AC_MSKB2) //23:22
    #define VBI2_B_HD_LEN Fld(7, 15, AC_MSKW21) //21:15
    #define VBI2_B_STC_END Fld(7, 8, AC_MSKB1) //14:8
    #define VBI2_B_STC Fld(8, 0, AC_FULLB0) //7:0
#define VBI_TYPE_B_STA_00 (VBI_STA_IO_BASE+0x0020)
  #define VBI_TYPE_B_STA_00_0 (VBI_STA_IO_BASE+0x0020)
  #define VBI_TYPE_B_STA_00_1 (VBI_STA_IO_BASE+0x0021)
  #define VBI_TYPE_B_STA_00_2 (VBI_STA_IO_BASE+0x0022)
  #define VBI_TYPE_B_STA_00_3 (VBI_STA_IO_BASE+0x0023)
    #define STA_VBI__B_WSS_RDY Fld(1, 31, AC_MSKB3) //31
    #define STA_VBI_B_WSS_OUT_4 Fld(6, 0, AC_MSKB0) //5:0
    #define STA_VBI_B_WSS_OUT_B16 Fld(6, 0, AC_MSKB0) //5:0
#define VBI_TYPE_B_STA_01 (VBI_STA_IO_BASE+0x0024)
  #define VBI_TYPE_B_STA_01_0 (VBI_STA_IO_BASE+0x0024)
  #define VBI_TYPE_B_STA_01_1 (VBI_STA_IO_BASE+0x0025)
  #define VBI_TYPE_B_STA_01_2 (VBI_STA_IO_BASE+0x0026)
  #define VBI_TYPE_B_STA_01_3 (VBI_STA_IO_BASE+0x0027)
    #define STA_VBI_B_WSS_OUT_3 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI_B_WSS_OUT_B15 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI_B_WSS_OUT_B14 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI_B_WSS_OUT_B13 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI_B_WSS_OUT_B12 Fld(8, 0, AC_FULLB0) //7:0
#define VBI_TYPE_B_STA_02 (VBI_STA_IO_BASE+0x0028)
  #define VBI_TYPE_B_STA_02_0 (VBI_STA_IO_BASE+0x0028)
  #define VBI_TYPE_B_STA_02_1 (VBI_STA_IO_BASE+0x0029)
  #define VBI_TYPE_B_STA_02_2 (VBI_STA_IO_BASE+0x002a)
  #define VBI_TYPE_B_STA_02_3 (VBI_STA_IO_BASE+0x002b)
    #define STA_VBI_B_WSS_OUT_2 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI_B_WSS_OUT_B11 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI_B_WSS_OUT_B10 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI_B_WSS_OUT_B9 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI_B_WSS_OUT_B8 Fld(8, 0, AC_FULLB0) //7:0
#define VBI_TYPE_B_STA_03 (VBI_STA_IO_BASE+0x002c)
  #define VBI_TYPE_B_STA_03_0 (VBI_STA_IO_BASE+0x002c)
  #define VBI_TYPE_B_STA_03_1 (VBI_STA_IO_BASE+0x002d)
  #define VBI_TYPE_B_STA_03_2 (VBI_STA_IO_BASE+0x002e)
  #define VBI_TYPE_B_STA_03_3 (VBI_STA_IO_BASE+0x002f)
    #define STA_VBI_B_WSS_OUT_1 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI_B_WSS_OUT_B7 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI_B_WSS_OUT_B6 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI_B_WSS_OUT_B5 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI_B_WSS_OUT_B4 Fld(8, 0, AC_FULLB0) //7:0
#define VBI_TYPE_B_STA_04 (VBI_STA_IO_BASE+0x0030)
  #define VBI_TYPE_B_STA_04_0 (VBI_STA_IO_BASE+0x0030)
  #define VBI_TYPE_B_STA_04_1 (VBI_STA_IO_BASE+0x0031)
  #define VBI_TYPE_B_STA_04_2 (VBI_STA_IO_BASE+0x0032)
  #define VBI_TYPE_B_STA_04_3 (VBI_STA_IO_BASE+0x0033)
    #define STA_VBI_B_WSS_OUT_0 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI_B_WSS_OUT_B3 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI_B_WSS_OUT_B2 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI_B_WSS_OUT_B1 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI_B_WSS_OUT_B0 Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_TYPE_B_STA_00 (VBI_STA_IO_BASE+0x0068)
  #define VBI2_TYPE_B_STA_00_0 (VBI_STA_IO_BASE+0x0068)
  #define VBI2_TYPE_B_STA_00_1 (VBI_STA_IO_BASE+0x0069)
  #define VBI2_TYPE_B_STA_00_2 (VBI_STA_IO_BASE+0x006a)
  #define VBI2_TYPE_B_STA_00_3 (VBI_STA_IO_BASE+0x006b)
    #define STA_VBI2__B_WSS_RDY Fld(1, 31, AC_MSKB3) //31
    #define STA_VBI2_B_WSS_OUT_4 Fld(6, 0, AC_MSKB0) //5:0
    #define STA_VBI2_B_WSS_OUT_B16 Fld(6, 0, AC_MSKB0) //5:0
#define VBI2_TYPE_B_STA_01 (VBI_STA_IO_BASE+0x006c)
  #define VBI2_TYPE_B_STA_01_0 (VBI_STA_IO_BASE+0x006c)
  #define VBI2_TYPE_B_STA_01_1 (VBI_STA_IO_BASE+0x006d)
  #define VBI2_TYPE_B_STA_01_2 (VBI_STA_IO_BASE+0x006e)
  #define VBI2_TYPE_B_STA_01_3 (VBI_STA_IO_BASE+0x006f)
    #define STA_VBI2_B_WSS_OUT_3 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI2_B_WSS_OUT_B15 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI2_B_WSS_OUT_B14 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI2_B_WSS_OUT_B13 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI2_B_WSS_OUT_B12 Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_TYPE_B_STA_02 (VBI_STA_IO_BASE+0x0070)
  #define VBI2_TYPE_B_STA_02_0 (VBI_STA_IO_BASE+0x0070)
  #define VBI2_TYPE_B_STA_02_1 (VBI_STA_IO_BASE+0x0071)
  #define VBI2_TYPE_B_STA_02_2 (VBI_STA_IO_BASE+0x0072)
  #define VBI2_TYPE_B_STA_02_3 (VBI_STA_IO_BASE+0x0073)
    #define STA_VBI2_B_WSS_OUT_2 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI2_B_WSS_OUT_B11 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI2_B_WSS_OUT_B10 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI2_B_WSS_OUT_B9 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI2_B_WSS_OUT_B8 Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_TYPE_B_STA_03 (VBI_STA_IO_BASE+0x0074)
  #define VBI2_TYPE_B_STA_03_0 (VBI_STA_IO_BASE+0x0074)
  #define VBI2_TYPE_B_STA_03_1 (VBI_STA_IO_BASE+0x0075)
  #define VBI2_TYPE_B_STA_03_2 (VBI_STA_IO_BASE+0x0076)
  #define VBI2_TYPE_B_STA_03_3 (VBI_STA_IO_BASE+0x0077)
    #define STA_VBI2_B_WSS_OUT_1 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI2_B_WSS_OUT_B7 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI2_B_WSS_OUT_B6 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI2_B_WSS_OUT_B5 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI2_B_WSS_OUT_B4 Fld(8, 0, AC_FULLB0) //7:0
#define VBI2_TYPE_B_STA_04 (VBI_STA_IO_BASE+0x0078)
  #define VBI2_TYPE_B_STA_04_0 (VBI_STA_IO_BASE+0x0078)
  #define VBI2_TYPE_B_STA_04_1 (VBI_STA_IO_BASE+0x0079)
  #define VBI2_TYPE_B_STA_04_2 (VBI_STA_IO_BASE+0x007a)
  #define VBI2_TYPE_B_STA_04_3 (VBI_STA_IO_BASE+0x007b)
    #define STA_VBI2_B_WSS_OUT_0 Fld(32, 0, AC_FULLDW) //31:0
    #define STA_VBI2_B_WSS_OUT_B3 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_VBI2_B_WSS_OUT_B2 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_VBI2_B_WSS_OUT_B1 Fld(8, 8, AC_FULLB1) //15:8
    #define STA_VBI2_B_WSS_OUT_B0 Fld(8, 0, AC_FULLB0) //7:0
#endif
/* End of Register Map Definition Start */


 /* VBI_00 & VBI2_00 */
#define VPS_LINE_EN     ((UINT32)0x01 << (Fld_shft(VBI_VPS_EN)))
#define CC_LINE_EN      ((UINT32)0x01 << (Fld_shft(VBI_CC_EN)))
#define WSS_LINE_EN     ((UINT32)0x01 << (Fld_shft(VBI_WSS_EN)))
#define TT_LINE_EN      ((UINT32)0x01 << (Fld_shft(VBI_TT_EN)))
    
/* VBI_05 & VBI2_05 */
#define VBI_OMODE_NO    0
#define VBI_OMODE_DEC   1

/* VBI_05 & VBI2_05 */
#define VBI_LINE_MODE   0
#define VBI_FIFO_MODE   1

/* VBI_01 & VBI2_01 */
#define VBI_VSRC_TVD3D  0x00
#define VBI_VSRC_HDTV   0x02 
#define VBI_VSRC_DVI    0x03

/* VBI slicer working mode */
#define VBI_NTSC_MODE   0
#define VBI_PAL_MODE    1
#define VBI_CNI_MODE    2

/* VBI_IMODE*/
#define VBI_IMODE_PALN  0
#define VBI_IMODE_NTSC  3

/* These Line Selections is for 536x setting */
#define HDTV_FIELD_INVERSE  1

/* AV Field Inverse Setting */
#define AV_IVS_NTSC_CC_SEL0     (19) // 5393 (O) 
#define AV_IVS_NTSC_CC_SEL1     (18) // 5393 (O)
#define AV_IVS_NTSC_WSS_SEL0    (18) // 5393 (O)
#define AV_IVS_NTSC_WSS_SEL1    (17) // 5393 (O)

#ifdef SUPPORT_PALCC_LINE18
#define AV_IVS_PAL_CC_SEL0      (19) // 5393 (O)
#define AV_IVS_PAL_CC_SEL1      (19) // 5393 (O) 
#else
#define AV_IVS_PAL_CC_SEL0      (23) // 5393 (O)
#define AV_IVS_PAL_CC_SEL1      (23) // 5393 (O) 
#endif

#define AV_IVS_PAL_WSS_SEL0     (24) // 5393 (O)
#define AV_IVS_PAL_WSS_SEL1     (24) // 5393 (O) /* WSS of PAL is avialable in field 0 only */

#define AV_IVS_PAL_TT0_START    (6)  // 5393 (O)
#define AV_IVS_PAL_TT0_END      (23) // 5393 (O)
#define AV_IVS_PAL_TT1_START    (7)  // 5393 (O)
#define AV_IVS_PAL_TT1_END      (23) // 5393 (O)

#define AV_IVS_NTSC_CC2X0_START (17) // 5393 (O)
#define AV_IVS_NTSC_CC2X0_END   (17) // 5393 (O)
#define AV_IVS_NTSC_CC2X1_START (16) // 5393 (O)
#define AV_IVS_NTSC_CC2X1_END   (16) // 5393 (O)

#define AV_IVS_PAL_VPS_SEL      (17) // 5393 (O) /* VPS is in field0 only */ 

/* AV Field Normal Setting */
#define AV_NML_NTSC_CC_SEL0     (18) // 5393 (O) 
#define AV_NML_NTSC_CC_SEL1     (18) // 5393 (O)
#define AV_NML_NTSC_WSS_SEL0    (17) // 5393 (O)
#define AV_NML_NTSC_WSS_SEL1    (17) // 5393 (O)

#ifdef SUPPORT_PALCC_LINE18
#define AV_NML_PAL_CC_SEL0      (18) // 5393 (O)
#define AV_NML_PAL_CC_SEL1      (19) // 5393 (O) 
#else
#define AV_NML_PAL_CC_SEL0      (22) // 5393 (O)
#define AV_NML_PAL_CC_SEL1      (23) // 5393 (O) 
#endif

#define AV_NML_PAL_WSS_SEL0     (23) // 5393 (O)
#define AV_NML_PAL_WSS_SEL1     (24) /* WSS of PAL is avialable in field 0 only */

#define AV_NML_PAL_TT0_START    (6)  // 5393 (O)
#define AV_NML_PAL_TT0_END      (22) // 5393 (O)
#define AV_NML_PAL_TT1_START    (6)  // 5393 (O)
#define AV_NML_PAL_TT1_END      (23) // 5393 (O)

#define AV_NML_NTSC_CC2X0_START (16) //(17) // 5393 (?)
#define AV_NML_NTSC_CC2X0_END   (16) //(17) // 5393 (?)
#define AV_NML_NTSC_CC2X1_START (16) //(16) // 5393 (?)
#define AV_NML_NTSC_CC2X1_END   (16) //(16) // 5393 (?)

#define AV_NML_PAL_VPS_SEL      (16) /* VPS is in field0 only */

/* HDTV Field Setting */
#if HDTV_FIELD_INVERSE
#define HDTV_NTSC_CC_SEL0   (15) // 5393 (O) 
#define HDTV_NTSC_CC_SEL1   (14) // 5393 (O) 
#define HDTV_NTSC_WSS_SEL0  (14) // 5393 (O) 
#define HDTV_NTSC_WSS_SEL1  (13) // 5393 (O) 

#define HDTV_PAL_CC_SEL0    (19) // 5393 (O)
#define HDTV_PAL_CC_SEL1    (19) // 5393 (O)
#define HDTV_PAL_WSS_SEL0   (20) // 5393 (O)
#define HDTV_PAL_WSS_SEL1   (20) // 5393 /* WSS of PAL is available in field 0 only */

#define HDTV_PAL_TT0_START  (2)  // 5393 (O)
#define HDTV_PAL_TT0_END    (19) // 5393 (O)
#define HDTV_PAL_TT1_START  (3)  // 5393 (O)
#define HDTV_PAL_TT1_END    (19) // 5393 (O)

#define HDTV_NTSC_WSS_PSCAN_SEL 28 //(O)Line 41 for 480 p
/* 37 = 32 + 5 */
#define HDTV_PAL_WSS_PSCAN_SEL 5   //(O) Line 43 for 576 P
//#define HDTV_PAL_WSS_PSCAN_SEL_OVER32 1
#define HDTV_720P_WSS_SEL 18 //(O)Line 24 for 720 p
#define HDTV_1080I_WSS_SEL0  (14) // 5393 (O) 
#define HDTV_1080I_WSS_SEL1  (13) // 5393 (O) 

#define HDTV_NTSC_TYPE_B_WSS_PSCAN_SEL 27 //(O)Line 40 for 480 p
#define HDTV_720P_TYPE_B_WSS_SEL 17 //(O)Line 23 for 720 p
#define HDTV_1080I_TYPE_B_WSS_SEL0  (13) // 5393 (O) 
#define HDTV_1080I_TYPE_B_WSS_SEL1  (12) // 5393 (O) 
#else
#define HDTV_NTSC_CC_SEL0   (14) // 5393 (?) 
#define HDTV_NTSC_CC_SEL1   (15) // 5393 (?) 
#define HDTV_NTSC_WSS_SEL0  (13) // 5393 (?) 
#define HDTV_NTSC_WSS_SEL1  (14) // 5393 (?) 

#define HDTV_PAL_CC_SEL0    (19) // 5393 (?)
#define HDTV_PAL_CC_SEL1    (19) // 5393 (?)
#define HDTV_PAL_WSS_SEL0   (20) // 5393 (?)
#define HDTV_PAL_WSS_SEL1   (20) // 5393 /* WSS of PAL is available in field 0 only */

#define HDTV_PAL_TT0_START  (3)  // 5393 (?)
#define HDTV_PAL_TT0_END    (19) // 5393 (?)
#define HDTV_PAL_TT1_START  (2)  // 5393 (?)
#define HDTV_PAL_TT1_END    (19) // 5393 (?)

#define HDTV_NTSC_WSS_PSCAN_SEL 28 //Line 41 for 480 p

/* 37 = 32 + 5 */
#define HDTV_PAL_WSS_PSCAN_SEL 5   //Line 43 for 576 P
//#define HDTV_PAL_WSS_PSCAN_SEL_OVER32 1
#define HDTV_720P_WSS_SEL 18 //(O)Line 24 for 720 p
#define HDTV_1080I_WSS_SEL0  (13) // 5393 (O) 
#define HDTV_1080I_WSS_SEL1  (14) // 5393 (O) 

#define HDTV_NTSC_TYPE_B_WSS_PSCAN_SEL 28 //Line 40 for 480 p
#define HDTV_720P_TYPE_B_WSS_SEL 17 //(O)Line 23 for 720 p
#define HDTV_1080I_TYPE_B_WSS_SEL0  (12) // 5393 (O) 
#define HDTV_1080I_TYPE_B_WSS_SEL1  (13) // 5393 (O) 

#endif

/* VBI CLI CMD defined */
#define VBI_CLI_SUPPORT 1 
#ifdef SUPPORT_AUTO_CC_DETECT
#define AUTO_CC_DETECT 1
#else
#define AUTO_CC_DETECT 0
#endif
// For ToFu display
#define SUPPORT_TOFU_DISPLAY    0

/* Set which VBI services are actvated in which VBI slicer(VBI0 or VBI2) */
EXTERN void vHwVbiSetLineEnable(UINT8 bVBISuite, UINT32 u4LineEn);

/* Set  VBI DRAM output mode */
EXTERN void vHwVbiSetOutputMode(UINT8 bVBISuite, UINT32 u4Mode);

/* Set VBI source */
EXTERN void vHwVbiSetSrc(UINT8 bVBISuite, UINT8 bSrc);

/* Get VBI source */
EXTERN UINT8 bHwVbiGetSrc(UINT8 bVBISuite);

/* Set VBI DRAM starting address */
EXTERN void vHwVbiSetMbase0(UINT8 bVBISuite, UINT32 u4BaseAddr);

/* Set VBI DRAM FIFO length */
EXTERN void vHwVbiSetMOffset(UINT8 bVBISuite, UINT32 u4offset);

/* Set VBI DRAM FIFO or LINE mode */
EXTERN void vHwVbiSetFifoMode(UINT8 bVBISuite, UINT32 u4FifoMode);

/* Reset VBI DRAM FIFO pointer */
EXTERN void vHwVbiResetFifo(UINT8 bVBISuite);

/* Clear Reset VBI DRAM FIFO pointer */
EXTERN void vHwVbiClrResetFifo(UINT8 bVBISuite);

/* Read VBI DRAM FIFO pointer */
EXTERN UINT32 u4HwVbiReadWritePtr(UINT8 bVBISuite);

/* Config VBI slicer setting */
EXTERN void vHwVbiSetVbiMode(UINT8 bVBISuite, UINT8 bVbiMode) ;

#if AUTO_CC_DETECT
/* Config VBI slicer setting */
EXTERN void vHwVbiSetVbiModeForAutoCC(UINT8 bVbiMode) ;
EXTERN void vHwVbiGetCCLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End);
#endif

/* Set CC line selection */
EXTERN void vHwVbiSetCCLine(UINT8 bVBISuite, UINT8 u1F0LineSel, UINT8 u1F1LineSel);

/* Set WSS line selection */
EXTERN void vHwVbiSetWSSLine(UINT8 bVBISuite, UINT8 u1F0LineSel, UINT8 u1F1LineSel);

#if SUPPORT_TYPE_B_VBI
/* Set Type B WSS line selection */
EXTERN void vHwVbiSetTypeBWSSLine(UINT8 bVBISuite, UINT8 u1F0LineSel, UINT8 u1F1LineSel);
#endif

/* Set TTX interval in Field 0 */
EXTERN void vHwVbiSetTTInterval0(UINT8 bVBISuite, UINT8 u1F0Start, UINT8 u1F0End);

/* Set TTX interval in Field 1 */
EXTERN void vHwVbiSetTTInterval1(UINT8 bVBISuite, UINT8 u1F1Start, UINT8 u1F1End);

/* Set VPS interval in Field 0 and Field 1 */
EXTERN void vHwVbiSetVPSInterval(UINT8 bVBISuite, UINT8 u1F0Start, UINT8 u1F0End);

/* Set Initial Zero-Crossing threshold */
EXTERN void vHwVbiSetInitZCThr(UINT8 bVBISuite, UINT8 u1ZCThr);

/* Get Initial Zero-Crossing threshold */
EXTERN UINT8 bHwVbiGetInitZCThr(UINT8 bVBISuite);

/* Set Line Start Code */
EXTERN void vHwVbiSetLSC(UINT8 bVBISuite, UINT32 u4LSC);

/* Set/Clear PScan */
EXTERN void vHwVbiSetClrPScan(UINT8 bVBISuite, UINT8 bSetClr);

/* Set WSS Frequency */
EXTERN void vHwVbiSetWSSFreq(UINT8 bVBISuite, UINT32 u4Freq);

#if SUPPORT_TYPE_B_VBI
/* Set TYPE B WSS Frequency */
EXTERN void vHwVbiSetTypeBWSSFreq(UINT8 bVBISuite, UINT32 u4Freq);
#endif

/* Set VBI Input Mode, only distinguish 525/625 mode */
EXTERN void vHwVbiSetIMode(UINT8 bVBISuite, UINT8 bImode);

/* Set/Clear HDTV sub-sample */
EXTERN void vHwVbiHDTVSubSample(UINT8 u1Set);

/* Set/Clear HDTV H/V position adjustable */
EXTERN void vHwVbiHVPosAdjust(UINT8 u1Set);

/* VBI core reset */
EXTERN void vHwVbiCoreReset(UINT8 bVBISuite);

/* Set VBI initial zero-crossing auto detect enable */
EXTERN void vHwVbiSetInitZC(UINT8 bVBISuite, UINT8 bSet);

/* Set VBI ECC OnOff */
EXTERN void vHwVBISetCCECC(UINT8 u1OnOff);

/* Set VBI PE_Gain */
EXTERN void vHwVbiSetPEGain(UINT8 bVBISuite, UINT8 bSet);

#if VBI_CLI_SUPPORT
/* Get  VBI DRAM output mode */
EXTERN UINT32 u4HwVbiGetOutputMode(UINT8 bVBISuite);

/* Get VBI DRAM starting address */
EXTERN UINT32 u4HwVbiGetMbase0(UINT8 bVBISuite);

/* Get VBI DRAM FIFO Offset */
EXTERN UINT32 u4HwVbiGetMOffset(UINT8 bVBISuite);

/* Get actvated VBI services */
EXTERN UINT32 u4HwVbiGetLineEnable(UINT8 bVBISuite);

/* VBI Get LSC */
EXTERN UINT32 vHwVbiGetLSC(UINT8 bVBISuite);

/* VBI Set CC line range */
EXTERN void vHwVbiSetCCLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End);

/* VBI Get CC line range */
EXTERN void vHwVbiGetCCLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End);

/* VBI Set WSS line range*/ 
EXTERN void vHwVbiSetWSSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Line);

/* VBI Get WSS line range*/ 
EXTERN void vHwVbiGetWSSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Line);

#if SUPPORT_TYPE_B_VBI
/* VBI Set Type B WSS line range*/ 
EXTERN void vHwVbiSetTypeBWSSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Line);

/* VBI Get Type B WSS line range*/ 
EXTERN void vHwVbiGetTypeBWSSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Line);
#endif

/* VBI Set Debug Port */
EXTERN void vHwVbiSetDbgPort(UINT32 u4Mode);

/* Get TTX Line Ragne */
EXTERN void vHwVbiGetTTXLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End);

/* Get VPS Line Ragne */
EXTERN void vHwVbiGetVPSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End);

/* Set VPS Line Ragne */
EXTERN void vHwVbiSetVPSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End);

/* Read VBI Register */
EXTERN UINT32 vHwVBIReadReg(UINT32 u4VbiReg);

/* Write VBI Register */
EXTERN void vHwVBIWriteReg(UINT32 u4VbiReg, UINT32 u4Value);

EXTERN void vHwVbiSetVBIGain(UINT8 bVBISuite, UINT8 bVBIGain);

EXTERN void vHwVbiSetCCInitSel(UINT8 bVBISuite, UINT8 u1Value);

#endif

/*****************************************************************************
  HW Parameter Define
  ****************************************************************************/
//--------------LSC Setting----------------//
#define AV_EU_LSC           0xC1
#define AV_US_LSC           0xC1
#define HDTV_LSC            0xD0
#define HDTV_PSCAN_480P_LSC 0xCD
#define HDTV_PSCAN_1080I_LSC 0xD


//--------------ZC Th----------------------//
#define AV_ZC_TH            0x50
#define HDTV_ZC_TH          0x2F
#define HDTV_PSCAN_ZC_TH    0x2F

//-------------VBI Gain--------------------//
#define AV_DFT_GAIN         0x4   //x1 
#define AV_US_GAIN          0x6   //x1.5

/*****************************************************************************
  Macro Declaration
  ****************************************************************************/
//#define IsVBI0ISR()  (IO32ReadFldAlign(VBI_STA_00, STA_VBI_RDY))
//#define IsVBI2ISR()  (IO32ReadFldAlign(VBI2_STA_00, STA_VBI2_RDY))
#define IsVBI0ISR()  (fgIsVdoIntCC1())
#define IsVBI2ISR()  (fgIsVdoIntCC2())

#define IsCCRdy()   (IO32ReadFldAlign(VBI_STA_00, STA_VBI_CC_RDY))
#define IsWSSRdy()  (IO32ReadFldAlign(VBI_STA_00, STA_VBI_WSS_RDY))
#define IsVPSRdy()  (IO32ReadFldAlign(VBI_STA_00, STA_VBI_VPS_RDY))
#define IsTTRdy()   (IO32ReadFldAlign(VBI_STA_00, STA_VBI_TT_RDY))
#define IsCC2XRdy() (IO32ReadFldAlign(VBI_STA_00, STA_VBI_TT_RDY))
    
#define IsCC2XEnable() (IO32ReadFldAlign(VBI_00, VBI_TT_EN))
#define IsTTXEnable()  (IO32ReadFldAlign(VBI_00, VBI_TT_EN))
#define IsWSSEnable()  (IO32ReadFldAlign(VBI_00, VBI_WSS_EN))
#define IsCCEnable()   (IO32ReadFldAlign(VBI_00, VBI_CC_EN))
#define IsVPSEnable()  (IO32ReadFldAlign(VBI_00, VBI_VPS_EN))
    
#define IsCC2Rdy()   (IO32ReadFldAlign(VBI2_STA_00, STA_VBI2_CC_RDY))
#define IsWSS2Rdy()  (IO32ReadFldAlign(VBI2_STA_00, STA_VBI2_WSS_RDY))
#define IsVPS2Rdy()  (IO32ReadFldAlign(VBI2_STA_00, STA_VBI2_VPS_RDY))
#define IsTT2Rdy()   (IO32ReadFldAlign(VBI2_STA_00, STA_VBI2_TT_RDY))
#define IsCC2X2Rdy() (IO32ReadFldAlign(VBI2_STA_00, STA_VBI2_TT_RDY))
    
#define IsCC2X2Enable() (IO32ReadFldAlign(VBI2_00, VBI2_TT_EN))
#define IsTTX2Enable()  (IO32ReadFldAlign(VBI2_00, VBI2_TT_EN))
#define IsWSS2Enable()  (IO32ReadFldAlign(VBI2_00, VBI2_WSS_EN))
#define IsCC2Enable()   (IO32ReadFldAlign(VBI2_00, VBI2_CC_EN))
#define IsVPS2Enable()  (IO32ReadFldAlign(VBI2_00, VBI2_VPS_EN))
#if SUPPORT_TYPE_B_VBI
#define IsTypeBWSSRdy()  (IO32ReadFldAlign(VBI_TYPE_B_STA_00, STA_VBI__B_WSS_RDY))
#define IsTypeBWSS2Rdy()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_00, STA_VBI2__B_WSS_RDY))
#define IsTypeBWSSEnable()  (IO32ReadFldAlign(VBI_TYPE_B_00, VBI_B_HD_WSS_EN))
#define IsTypeBWSS2Enable()  (IO32ReadFldAlign(VBI2_TYPE_B_00, VBI2_B_HD_WSS_EN))

#endif
    
#define u4GetVbiField()  (IO32ReadFldAlign(VBI_STA_00, STA_VBI_FIELD))
#define u4GetVbi2Field() (IO32ReadFldAlign(VBI2_STA_00, STA_VBI2_FIELD))
    
#define u1GetCC_B0() (IO32ReadFldAlign(VBI_STA_05, STA_VBI_CC_OUT_B0))
#define u1GetCC_B1() (IO32ReadFldAlign(VBI_STA_05, STA_VBI_CC_OUT_B1))
    
#define u1GetCC2_B0() (IO32ReadFldAlign(VBI2_STA_05, STA_VBI2_CC_OUT_B0))
#define u1GetCC2_B1() (IO32ReadFldAlign(VBI2_STA_05, STA_VBI2_CC_OUT_B1))
    
#define u4GetWSS()  (IO32ReadFldAlign(VBI_STA_01, STA_VBI_WSS_OUT))
#define u4GetWSS2() (IO32ReadFldAlign(VBI2_STA_01, STA_VBI2_WSS_OUT))
    
#define u4GetVbiWP()  (IO32ReadFldAlign(VBI_STA_00, STA_VBI_ADR)<<4)
#define u4GetVbi2WP() (IO32ReadFldAlign(VBI2_STA_00, STA_VBI2_ADR)<<4)
    
#define u4GetVPSPart() (IO32ReadFldAlign(VBI2_STA_04, STA_VBI2_VPS_OUT_2))
    
#define u1GetVPS_B0()  (IO32ReadFldAlign(VBI2_STA_02, STA_VBI2_VPS_OUT_B0))
#define u1GetVPS_B1()  (IO32ReadFldAlign(VBI2_STA_02, STA_VBI2_VPS_OUT_B1))
#define u1GetVPS_B2()  (IO32ReadFldAlign(VBI2_STA_02, STA_VBI2_VPS_OUT_B2))
#define u1GetVPS_B3()  (IO32ReadFldAlign(VBI2_STA_02, STA_VBI2_VPS_OUT_B3))
#define u1GetVPS_B4()  (IO32ReadFldAlign(VBI2_STA_03, STA_VBI2_VPS_OUT_B4))
#define u1GetVPS_B5()  (IO32ReadFldAlign(VBI2_STA_03, STA_VBI2_VPS_OUT_B5))
#define u1GetVPS_B6()  (IO32ReadFldAlign(VBI2_STA_03, STA_VBI2_VPS_OUT_B6))
#define u1GetVPS_B7()  (IO32ReadFldAlign(VBI2_STA_03, STA_VBI2_VPS_OUT_B7))
#define u1GetVPS_B8()  (IO32ReadFldAlign(VBI2_STA_04, STA_VBI2_VPS_OUT_B8))
#define u1GetVPS_B9()  (IO32ReadFldAlign(VBI2_STA_04, STA_VBI2_VPS_OUT_B9))
#define u1GetVPS_B10() (IO32ReadFldAlign(VBI2_STA_04, STA_VBI2_VPS_OUT_B10))
#define u1GetVPS_B11() (IO32ReadFldAlign(VBI2_STA_04, STA_VBI2_VPS_OUT_B11))
#define u1GetVPS_B12() (IO32ReadFldAlign(VBI2_STA_05, STA_VBI2_VPS_OUT_B12))

#define u1GetCCRunInMax()  (IO32ReadFldAlign(VBI_STA_07, STA_VBI_CNI0_OUT)>>8);               
#define u1GetCCRunInMin()  (IO32ReadFldAlign(VBI_STA_07, STA_VBI_CNI0_OUT)&0xFF);
#define u1GetCC2RunInMax() (IO32ReadFldAlign(VBI2_STA_07, STA_VBI2_CNI0_OUT)>>8);               
#define u1GetCC2RunInMin() (IO32ReadFldAlign(VBI2_STA_07, STA_VBI2_CNI0_OUT)&0xFF);
#if SUPPORT_TYPE_B_VBI
#define u1GetType_B_WSS_B0()  (IO32ReadFldAlign(VBI_TYPE_B_STA_04, STA_VBI_B_WSS_OUT_B0))
#define u1GetType_B_WSS_B1()  (IO32ReadFldAlign(VBI_TYPE_B_STA_04, STA_VBI_B_WSS_OUT_B1))
#define u1GetType_B_WSS_B2()  (IO32ReadFldAlign(VBI_TYPE_B_STA_04, STA_VBI_B_WSS_OUT_B2))
#define u1GetType_B_WSS_B3()  (IO32ReadFldAlign(VBI_TYPE_B_STA_04, STA_VBI_B_WSS_OUT_B3))
#define u1GetType_B_WSS_B4()  (IO32ReadFldAlign(VBI_TYPE_B_STA_03, STA_VBI_B_WSS_OUT_B4))
#define u1GetType_B_WSS_B5()  (IO32ReadFldAlign(VBI_TYPE_B_STA_03, STA_VBI_B_WSS_OUT_B5))
#define u1GetType_B_WSS_B6()  (IO32ReadFldAlign(VBI_TYPE_B_STA_03, STA_VBI_B_WSS_OUT_B6))
#define u1GetType_B_WSS_B7()  (IO32ReadFldAlign(VBI_TYPE_B_STA_03, STA_VBI_B_WSS_OUT_B7))
#define u1GetType_B_WSS_B8()  (IO32ReadFldAlign(VBI_TYPE_B_STA_02, STA_VBI_B_WSS_OUT_B8))
#define u1GetType_B_WSS_B9()  (IO32ReadFldAlign(VBI_TYPE_B_STA_02, STA_VBI_B_WSS_OUT_B9))
#define u1GetType_B_WSS_B10() (IO32ReadFldAlign(VBI_TYPE_B_STA_02, STA_VBI_B_WSS_OUT_B10))
#define u1GetType_B_WSS_B11() (IO32ReadFldAlign(VBI_TYPE_B_STA_02, STA_VBI_B_WSS_OUT_B11))
#define u1GetType_B_WSS_B12() (IO32ReadFldAlign(VBI_TYPE_B_STA_01, STA_VBI_B_WSS_OUT_B12))
#define u1GetType_B_WSS_B13() (IO32ReadFldAlign(VBI_TYPE_B_STA_01, STA_VBI_B_WSS_OUT_B13))
#define u1GetType_B_WSS_B14() (IO32ReadFldAlign(VBI_TYPE_B_STA_01, STA_VBI_B_WSS_OUT_B14))
#define u1GetType_B_WSS_B15() (IO32ReadFldAlign(VBI_TYPE_B_STA_01, STA_VBI_B_WSS_OUT_B15))
#define u1GetType_B_WSS_B16() (IO32ReadFldAlign(VBI_TYPE_B_STA_00, STA_VBI_B_WSS_OUT_B16))

#define u1GetType_B_WSS2_B0()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_04, STA_VBI2_B_WSS_OUT_B0))
#define u1GetType_B_WSS2_B1()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_04, STA_VBI2_B_WSS_OUT_B1))
#define u1GetType_B_WSS2_B2()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_04, STA_VBI2_B_WSS_OUT_B2))
#define u1GetType_B_WSS2_B3()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_04, STA_VBI2_B_WSS_OUT_B3))
#define u1GetType_B_WSS2_B4()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_03, STA_VBI2_B_WSS_OUT_B4))
#define u1GetType_B_WSS2_B5()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_03, STA_VBI2_B_WSS_OUT_B5))
#define u1GetType_B_WSS2_B6()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_03, STA_VBI2_B_WSS_OUT_B6))
#define u1GetType_B_WSS2_B7()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_03, STA_VBI2_B_WSS_OUT_B7))
#define u1GetType_B_WSS2_B8()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_02, STA_VBI2_B_WSS_OUT_B8))
#define u1GetType_B_WSS2_B9()  (IO32ReadFldAlign(VBI2_TYPE_B_STA_02, STA_VBI2_B_WSS_OUT_B9))
#define u1GetType_B_WSS2_B10() (IO32ReadFldAlign(VBI2_TYPE_B_STA_02, STA_VBI2_B_WSS_OUT_B10))
#define u1GetType_B_WSS2_B11() (IO32ReadFldAlign(VBI2_TYPE_B_STA_02, STA_VBI2_B_WSS_OUT_B11))
#define u1GetType_B_WSS2_B12() (IO32ReadFldAlign(VBI2_TYPE_B_STA_01, STA_VBI2_B_WSS_OUT_B12))
#define u1GetType_B_WSS2_B13() (IO32ReadFldAlign(VBI2_TYPE_B_STA_01, STA_VBI2_B_WSS_OUT_B13))
#define u1GetType_B_WSS2_B14() (IO32ReadFldAlign(VBI2_TYPE_B_STA_01, STA_VBI2_B_WSS_OUT_B14))
#define u1GetType_B_WSS2_B15() (IO32ReadFldAlign(VBI2_TYPE_B_STA_01, STA_VBI2_B_WSS_OUT_B15))
#define u1GetType_B_WSS2_B16() (IO32ReadFldAlign(VBI2_TYPE_B_STA_00, STA_VBI2_B_WSS_OUT_B16))
#endif
#endif
