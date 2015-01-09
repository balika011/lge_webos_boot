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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: hw_scpip_pq.h $
 *
 * Project:
 * --------
 *   MT5398
 *
 * Description:
 * ------------
 *   Scaler PQ register definition
 *
 * Author:
 * -------
 *   Benson Lee
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 07/04/06 1:22p $
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_SCPIP_PQ_H_
#define _HW_SCPIP_PQ_H_

#include "hw_vdoin.h"

#define IO_SCALER_BASE    (VDOIN_BASE+0x5000)

//Page CTIF
#define ECTIF_00 (IO_SCALER_BASE + 0x6A0)
    #define SEL_CTI_HSHARP Fld(1,31,AC_MSKB3)//[31:31]
    #define SEL_CTI_HSHARP_DIRECT Fld(1,30,AC_MSKB3)//[30:30]
    #define ECTIF_HUE_TIE Fld(1,28,AC_MSKB3)//[28:28]
    #define ECTIF_FLPF Fld(1,12,AC_MSKB1)//[12:12]
    #define ECTIF_FLPF_SEL Fld(1,11,AC_MSKB1)//[11:11]
    #define ECTIF_DX_SGN Fld(1,9,AC_MSKB1)//[9:9]
    #define ECTIF_PRT_ENA Fld(1,6,AC_MSKB0)//[6:6]
    #define ECTIF_SGN_PRT Fld(1,5,AC_MSKB0)//[5:5]
    #define ECTIF_HD Fld(1,4,AC_MSKB0)//[4:4]
    #define ECTIF_INK Fld(1,3,AC_MSKB0)//[3:3]
    #define ECTIF_ENA Fld(1,0,AC_MSKB0)//[0:0]
#define ECTIF_01 (IO_SCALER_BASE + 0x6A4)
    #define ECTIF_LPF3 Fld(1,26,AC_MSKB3)//[26:26]
    #define ECTIF_LPF3_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define ECTIF_LPF2 Fld(1,22,AC_MSKB2)//[22:22]
    #define ECTIF_LPF2_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define ECTIF_LPF1 Fld(1,18,AC_MSKB2)//[18:18]
    #define ECTIF_LPF1_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define ECTIF_FIX_SZ Fld(3,12,AC_MSKB1)//[14:12]
    #define ECTIF_FIX Fld(1,11,AC_MSKB1)//[11:11]
    #define ECTIF_SEXT_LARGE Fld(1,10,AC_MSKB1)//[10:10]
    #define ECTIF_SEXT Fld(1,9,AC_MSKB1)//[9:9]
    #define ECTIF_SGN Fld(1,8,AC_MSKB1)//[8:8]
    #define ECTIF_SGN_TH Fld(8,0,AC_FULLB0)//[7:0]
#define ECTIF_02 (IO_SCALER_BASE + 0x6A8)
    #define ECTIF_U_STB_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTIF_U_STB_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTIF_U_STB_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTIF_U_WND_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTIF_03 (IO_SCALER_BASE + 0x6AC)
    #define ECTIF_V_STB_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTIF_V_STB_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTIF_V_STB_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTIF_V_WND_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTIF_04 (IO_SCALER_BASE + 0x6B0)
    #define ECTIF_FLAT_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTIF_FLAT_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTIF_FLAT_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTIF_FLAT_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define ECTIF_FLAT_TIE Fld(1,5,AC_MSKB0)//[5:5]
    #define ECTIF_FLAT_BYPASS Fld(1,4,AC_MSKB0)//[4:4]
    #define ECTIF_FLAT_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTIF_05 (IO_SCALER_BASE + 0x6B4)
    #define ECTIF_COR Fld(8,24,AC_FULLB3)//[31:24]

// HSHARP_MAIN
#define HSHARP_00 (IO_SCALER_BASE + 0x600)
    #define SHARP_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define SHARP_INK Fld(1,30,AC_MSKB3)//[30:30]
    #define LOW_BAND_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define HIGH_BAND_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define SHARP_HIGH Fld(8,16,AC_FULLB2)//[23:16]
    #define SHARP_MID Fld(8,8,AC_FULLB1)//[15:8]
    #define SHARP_LOW Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_01 (IO_SCALER_BASE + 0x604)
    #define SHARP_SEP_GAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define SHARP_HIGH_NEG Fld(8,16,AC_FULLB2)//[23:16]
    #define SHARP_MID_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define SHARP_LOW_NEG Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_02 (IO_SCALER_BASE + 0x608)
    #define SHARP_PREC_HIGH Fld(3,28,AC_MSKB3)//[30:28]
    #define SHARP_HIGH_CORING Fld(8,16,AC_FULLB2)//[23:16]
    #define SHARP_MID_CORING Fld(8,8,AC_FULLB1)//[15:8]
    #define SHARP_LOW_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_03 (IO_SCALER_BASE + 0x60C)
    #define SHARP_PREC_MID Fld(3,28,AC_MSKB3)//[30:28]
    #define SHARP_HIGH_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define SHARP_MID_LIMIT_POS Fld(8,8,AC_FULLB1)//[15:8]
    #define SHARP_LOW_LIMIT_POS Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_04 (IO_SCALER_BASE + 0x610)
    #define SHARP_PREC_LOW Fld(3,28,AC_MSKB3)//[30:28]
    #define SHARP_HIGH_LIMIT_NEG Fld(8,16,AC_FULLB2)//[23:16]
    #define SHARP_MID_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define SHARP_LOW_LIMIT_NEG Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_05 (IO_SCALER_BASE + 0x614)
    #define CLIP_POS_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define CLIP_NEG_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define LC_CLIP_POS_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define LC_CLIP_NEG_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define GB_CLIP_POS_EN_H Fld(1,27,AC_MSKB3)//[27:27]
    #define GB_CLIP_NEG_EN_H Fld(1,26,AC_MSKB3)//[26:26]
    #define GB_CLIP_POS_EN_M Fld(1,25,AC_MSKB3)//[25:25]
    #define GB_CLIP_NEG_EN_M Fld(1,24,AC_MSKB3)//[24:24]
    #define GB_CLIP_POS_EN_L Fld(1,23,AC_MSKB2)//[23:23]
    #define GB_CLIP_NEG_EN_L Fld(1,22,AC_MSKB2)//[22:22]
    #define CLIP_POS_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define CLIP_NEG_TH Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_06 (IO_SCALER_BASE + 0x618)
    #define LC_WIN_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define LC_TABLE_GAIN Fld(3,28,AC_MSKB3)//[30:28]
    #define LC_TABLE_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define LC_TABLE_OFFSET Fld(8,16,AC_FULLB2)//[23:16]
    #define CLIP_FRONT_THR_HIGH Fld(8,8,AC_FULLB1)//[15:8]
    #define CLIP_FRONT_THR_LOW Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_07 (IO_SCALER_BASE + 0x61C)
    #define LC_SHARP_HIGH Fld(8,16,AC_FULLB2)//[23:16]
    #define LC_SHARP_MID Fld(8,8,AC_FULLB1)//[15:8]
    #define LC_SHARP_LOW Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_08 (IO_SCALER_BASE + 0x620)
    #define LC_SHARP_HIGH_NEG Fld(8,16,AC_FULLB2)//[23:16]
    #define LC_SHARP_MID_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define LC_SHARP_LOW_NEG Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_09 (IO_SCALER_BASE + 0x624)
    #define LC_PREC_HIGH Fld(3,28,AC_MSKB3)//[30:28]
    #define LC_SHARP_HIGH_CORING Fld(8,16,AC_FULLB2)//[23:16]
    #define LC_SHARP_MID_CORING Fld(8,8,AC_FULLB1)//[15:8]
    #define LC_SHARP_LOW_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_0A (IO_SCALER_BASE + 0x628)
    #define LC_PREC_MID Fld(3,28,AC_MSKB3)//[30:28]
    #define LC_SHARP_HIGH_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define LC_SHARP_MID_LIMIT_POS Fld(8,8,AC_FULLB1)//[15:8]
    #define LC_SHARP_LOW_LIMIT_POS Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_0B (IO_SCALER_BASE + 0x62C)
    #define LC_PREC_LOW Fld(3,28,AC_MSKB3)//[30:28]
    #define LC_SHARP_HIGH_LIMIT_NEG Fld(8,16,AC_FULLB2)//[23:16]
    #define LC_SHARP_MID_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define LC_SHARP_LOW_LIMIT_NEG Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_0C (IO_SCALER_BASE + 0x630)
    #define UV_CORING Fld(7,24,AC_MSKB3)//[30:24]
    #define UV_GAIN_LEVEL Fld(4,20,AC_MSKB2)//[23:20]
    #define UV_TYPE_SELN Fld(2,18,AC_MSKB2)//[19:18]
    #define UV_POS_DEGRADE_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define UV_NEG_DEGRADE_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define Y_BND_PRT_DIS Fld(1,12,AC_MSKB1)//[12:12]
    #define BND_EXT_ENA Fld(1,11,AC_MSKB1)//[11:11]
    #define C_BND_PRT_ENA Fld(1,10,AC_MSKB1)//[10:10]
    #define DEG_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define CORE_SHIFT Fld(2,6,AC_MSKB0)//[7:6]
    #define UVDIFF_SHIFT Fld(2,4,AC_MSKB0)//[5:4]
    #define MAXMIN_RNG Fld(2,2,AC_MSKB0)//[3:2]
    #define LC_MAXMIN_RNG Fld(2,0,AC_MSKB0)//[1:0]
#define HSHARP_0D (IO_SCALER_BASE + 0x634)
    #define UV_IS_444 Fld(1,31,AC_MSKB3)//[31:31]
    #define SYNC_BYPASS Fld(1,30,AC_MSKB3)//[30:30]
    #define SYNC_TYPE Fld(1,29,AC_MSKB3)//[29:29]
    #define C_VSYNC_DELAY_OFF Fld(1,28,AC_MSKB3)//[28:28]
    #define SHP_NRM Fld(1,21,AC_MSKB2)//[21:21]
    #define SHP_TBL_SMOOTH Fld(1,20,AC_MSKB2)//[20:20]
    #define LC_HALT_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define LC_HALT_START Fld(7,12,AC_MSKW21)//[18:12]
    #define LC_HALT_END Fld(7,4,AC_MSKW10)//[10:4]
    #define ATPG_CONTROL Fld(1,2,AC_MSKB0)//[2:2]
    #define HBOUND_INV Fld(1,1,AC_MSKB0)//[1:1]
    #define DEMO_EN Fld(1,0,AC_MSKB0)//[0:0]
#define HSHARP_0E (IO_SCALER_BASE + 0x638)
    #define DEMO_LBOUND Fld(12,20,AC_MSKW32)//[31:20]
    #define DEMO_RBOUND Fld(12,8,AC_MSKW21)//[19:8]
#define HSHARP_0F (IO_SCALER_BASE + 0x63C)
    #define DEMO_TBOUND Fld(12,20,AC_MSKW32)//[31:20]
    #define DEMO_BBOUND Fld(12,8,AC_MSKW21)//[19:8]
    #define SHARP_AC_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SHARP_LBAND_SEL Fld(1,2,AC_MSKB0)//[2:2]
    #define SHARP_MBAND_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define SHARP_HBAND_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define HSHARP_10 (IO_SCALER_BASE + 0x640)
    #define SHARP_AC_COEF Fld(5,24,AC_MSKB3)//[28:24]
    #define SHARP_SOFT_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    
#if defined(CC_MT5399)
//Page IRR
#define IRR_00 (IO_SCALER_BASE + 0x0E0)
    #define IRR_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define IRR_BYPASS Fld(1,30,AC_MSKB3)//[30:30]
    #define IRR_TAP Fld(2,28,AC_MSKB3)//[29:28]
    #define IRR_DEBUG Fld(4,24,AC_MSKB3)//[27:24]
    #define IRR_EDGE_TH Fld(10,14,AC_MSKW21)//[23:14]
    #define IRR_EDGE_SLOPE Fld(3,11,AC_MSKB1)//[13:11]
    #define IRR_HF_L_TH Fld(8,3,AC_MSKW10)//[10:3]
    #define IRR_HF_L_SLOPE Fld(3,0,AC_MSKB0)//[2:0]
#define IRR_01 (IO_SCALER_BASE + 0x0E4)
    #define IRR_HF_C_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define IRR_HF_C_SLOPE Fld(3,21,AC_MSKB2)//[23:21]
    #define IRR_LF_L_TH Fld(8,12,AC_MSKW21)//[19:12]
    #define IRR_LF_L_SLOPE Fld(3,8,AC_MSKB1)//[10:8]
    #define IRR_LF_C_TH Fld(8,0,AC_FULLB0)//[7:0]
#define IRR_02 (IO_SCALER_BASE + 0x0E8)
    #define IRR_LF_C_SLOPE Fld(3,29,AC_MSKB3)//[31:29]
    #define IRR_LF_L_TAP Fld(2,26,AC_MSKB3)//[27:26]
    #define IRR_LF_C_TAP Fld(2,24,AC_MSKB3)//[25:24]
    #define IRR_DEACY_ER Fld(4,16,AC_MSKB2)//[19:16]
    #define IRR_DEACY_TH Fld(4,12,AC_MSKB1)//[15:12]
    #define IRR_RIPPLE_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define IRR_RIPPLE_TAP Fld(1,7,AC_MSKB0)//[7:7]
    #define IRR_FLAT_TAP Fld(1,6,AC_MSKB0)//[6:6]
    #define IRR_LOW_IRE_GAIN Fld(3,3,AC_MSKB0)//[5:3]
    #define IRR_LOW_IRE_SLOPE Fld(3,0,AC_MSKB0)//[2:0]
#define IRR_03 (IO_SCALER_BASE + 0x0EC)
    #define IRR_FLAT_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define IRR_FLAT_SLOPE Fld(3,21,AC_MSKB2)//[23:21]
    #define IRR_LOW_IRE_THRESHOLD Fld(8,12,AC_MSKW21)//[19:12]
    #define IRR_FILTER Fld(3,9,AC_MSKB1)//[11:9]
    #define IRR_BSL_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define IRR_BSL_TH Fld(8,0,AC_FULLB0)//[7:0]
#define IRR_04 (IO_SCALER_BASE + 0x0F0)
    #define IRR_HF_OFF Fld(1,31,AC_MSKB3)//[31:31]
    #define IRR_LF_OFF Fld(1,30,AC_MSKB3)//[30:30]
    #define IRR_EDGE_OFF Fld(1,29,AC_MSKB3)//[29:29]
    #define IRR_LEVEL Fld(5,24,AC_MSKB3)//[28:24]
    #define IRR_DECAY_OFF Fld(1,22,AC_MSKB2)//[22:22]
    #define IRR_RIPPLE_OFF Fld(1,21,AC_MSKB2)//[21:21]
    #define IRR_HF_C_OFF Fld(1,20,AC_MSKB2)//[20:20]
    #define IRR_LF_C_OFF Fld(1,19,AC_MSKB2)//[19:19]
    #define IRR_FLAT_LEVEL Fld(4,12,AC_MSKB1)//[15:12]
    #define IRR_FILTER_CLIP Fld(10,0,AC_MSKW10)//[9:0]
#define IRR_05 (IO_SCALER_BASE + 0x0F4)
    #define IRR_HIGH_STRONG_LINK Fld(8,24,AC_FULLB3)//[31:24]
    #define IRR_LOW_STRONG_LINK Fld(8,16,AC_FULLB2)//[23:16]
    #define IRR_RESERVED Fld(16,0,AC_FULLW10)//[15:0]
#define IRR_06 (IO_SCALER_BASE + 0x0F8)
    #define IRR_ATPG_CT Fld(1,1,AC_MSKB0)//[1:1]
    #define IRR_ATPG_OB Fld(1,0,AC_MSKB0)//[0:0]
//Page PEI
#define SC_PEI_00 (IO_SCALER_BASE + 0xD00)
    #define PEI_INK_SEL Fld(3,29,AC_MSKB3)//[31:29]
    #define PEI_FORCE_USER_DIR Fld(1,26,AC_MSKB3)//[26:26]
    #define PEI_USER_DIR_SIGN Fld(1,25,AC_MSKB3)//[25:25]
    #define PEI_USER_DIR_VAL Fld(5,20,AC_MSKW32)//[24:20]
    #define PEI_S1_SMOOTH_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define PEI_S2_SMOOTH_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SC_PEI_01 (IO_SCALER_BASE + 0xD04)
    #define PEI_HOTIZONTAL_EDGE_TH Fld(5,20,AC_MSKW32)//[24:20]
    #define PEI_BACKGROUND_COST_TH Fld(4,12,AC_MSKB1)//[15:12]
    #define PEI_BACKGROUND_CNT_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define PEI_VERTICAL_VAR_TH Fld(3,2,AC_MSKB0)//[4:2]
    #define PEI_VERTICAL_BASE_TH Fld(2,0,AC_MSKB0)//[1:0]
#define SC_PEI_02 (IO_SCALER_BASE + 0xD08)
    #define PEI_LEVEL_TOLERANCE Fld(3,29,AC_MSKB3)//[31:29]
    #define PEI_RESOLUTION_TOLERANCE Fld(3,26,AC_MSKB3)//[28:26]
    #define PEI_NOISE_TOLERANCE Fld(3,23,AC_MSKW32)//[25:23]
    #define PEI_ALIAS_PERIOD_TH Fld(9,8,AC_MSKW21)//[16:8]
    #define PEI_ALIAS_VAR_TH Fld(8,0,AC_FULLB0)//[7:0]
#define SC_PEI_03 (IO_SCALER_BASE + 0xD0C)
    #define PEI_SCOST_WEIGHT Fld(4,16,AC_MSKB2)//[19:16]
    #define PEI_MCOST_WEIGHT Fld(4,12,AC_MSKB1)//[15:12]
    #define PEI_BCOST_WEIGHT Fld(4,8,AC_MSKB1)//[11:8]
    #define PEI_TCOST_WEIGHT Fld(4,4,AC_MSKB0)//[7:4]
    #define PEI_ACOST_WEIGHT Fld(4,0,AC_MSKB0)//[3:0]
#define SC_PEI_04 (IO_SCALER_BASE + 0xD10)
    #define PEI_S1_PASS_TH Fld(8,12,AC_MSKW21)//[19:12]
    #define PEI_S1_HIGH_CONF_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define PEI_S1_MID_CONF_TH Fld(4,4,AC_MSKB0)//[7:4]
    #define PEI_S1_LOW_CONF_TH Fld(4,0,AC_MSKB0)//[3:0]
#define SC_PEI_05 (IO_SCALER_BASE + 0xD14)
    #define PEI_KCOST_WEIGHT Fld(4,24,AC_MSKB3)//[27:24]
    #define PEI_PCOST_WEIGHT Fld(4,20,AC_MSKB2)//[23:20]
    #define PEI_CCOST_WEIGHT Fld(4,16,AC_MSKB2)//[19:16]
    #define PEI_FCOST_WEIGHT Fld(4,12,AC_MSKB1)//[15:12]
    #define PEI_S2_HIGH_CONF_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define PEI_S2_LOW_CONF_TH Fld(4,4,AC_MSKB0)//[7:4]
    #define PEI_S2_AMBIGUOUS_DIR_TH Fld(4,0,AC_MSKB0)//[3:0]
#define SC_PEI_08 (IO_SCALER_BASE + 0xD20)
    #define PEI_DIR_HIST_FREEZE Fld(1,30,AC_MSKB3)//[30:30]
    #define PEI_DIR_HIST_QUANT Fld(2,28,AC_MSKB3)//[29:28]
    #define PEI_DIR_HIST_START_X Fld(11,16,AC_MSKW32)//[26:16]
    #define PEI_DIR_HIST_END_X Fld(11,0,AC_MSKW10)//[10:0]
#define SC_PEI_09 (IO_SCALER_BASE + 0xD24)
    #define PEI_DIR_HIST_START_Y Fld(11,16,AC_MSKW32)//[26:16]
    #define PEI_DIR_HIST_END_Y Fld(11,0,AC_MSKW10)//[10:0]
#define SC_PEI_10 (IO_SCALER_BASE + 0xD40)
    #define PEI_ALIAS_CNT_FREEZE Fld(1,30,AC_MSKB3)//[30:30]
    #define PEI_ALIAS_CNT_QUANT Fld(2,28,AC_MSKB3)//[29:28]
    #define PEI_ALIAS_CNT_START_X Fld(11,16,AC_MSKW32)//[26:16]
    #define PEI_ALIAS_CNT_END_X Fld(11,0,AC_MSKW10)//[10:0]
#define SC_PEI_11 (IO_SCALER_BASE + 0xD44)
    #define PEI_ALIAS_CNT_START_Y Fld(11,16,AC_MSKW32)//[26:16]
    #define PEI_ALIAS_CNT_END_Y Fld(11,0,AC_MSKW10)//[10:0]
#define SC_PEI_12 (IO_SCALER_BASE + 0xD48)
    #define PURE_EDGE_RATIO_0 Fld(6,25,AC_MSKB3)//[30:25]
    #define PURE_EDGE_RATIO_1 Fld(6,19,AC_MSKW32)//[24:19]
    #define PURE_EDGE_RATIO_2 Fld(6,13,AC_MSKW21)//[18:13]
    #define PURE_EDGE_MAX_RATIO Fld(6,7,AC_MSKW10)//[12:7]
    #define PURE_EDGE_LONG_RATIO Fld(6,0,AC_MSKB0)//[5:0]
#define SC_PEI_13 (IO_SCALER_BASE + 0xD4C)
    #define PURE_EDGE_45_RANGE Fld(4,28,AC_MSKB3)//[31:28]
    #define PURE_EDGE_45_CORING Fld(4,24,AC_MSKB3)//[27:24]
    #define PURE_EDGE_INK_SEL Fld(3,21,AC_MSKB2)//[23:21]
    #define PURE_EDGE_CLIP_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define PURE_EDGE_CLIP_THRESHOLD Fld(6,14,AC_MSKW21)//[19:14]
    #define PURE_EDGE_CLIP_RATIO Fld(8,6,AC_MSKW10)//[13:6]
    #define PURE_EDGE_CLIP_BASE Fld(6,0,AC_MSKB0)//[5:0]
#define SC_PEI_14 (IO_SCALER_BASE + 0xD50)
    #define ALIAS_CNT Fld(16,0,AC_FULLW10)//[15:0]
#endif

//Page SCFIR
#define SCFIR_00 (IO_SCALER_BASE + 0x500)
    #define FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define RDATA_BYTE_EN_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define WDATA_BYTE_EN_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define SCFIR_01 (IO_SCALER_BASE + 0x504)
    #define CPURW_ACTIVE_VPP Fld(1,8,AC_MSKB1)//[8:8]
    #define CPU_SEL_SRAM_0 Fld(1,7,AC_MSKB0)//[7:7]
    #define CPURW_ACTIVE Fld(1,6,AC_MSKB0)//[6:6]
    #define TABLE_WAIT_VSYNC Fld(1,5,AC_MSKB0)//[5:5]
    #define FILL_TABLE_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define READ_FINISH Fld(1,3,AC_MSKB0)//[3:3]
    #define READ Fld(1,2,AC_MSKB0)//[2:2]
    #define WRITE_FINISH Fld(1,1,AC_MSKB0)//[1:1]
    #define WRITE Fld(1,0,AC_MSKB0)//[0:0]
#define SCFIR_02 (IO_SCALER_BASE + 0x508)
    #define COE_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define CPU_SRAM_ADDR Fld(8,16,AC_FULLB2)//[23:16]
    #define R_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define SCFIR_03 (IO_SCALER_BASE + 0x50C)
    #define WDATA_00 Fld(32,0,AC_FULLDW)//[31:0]
#define SCFIR_04 (IO_SCALER_BASE + 0x510)
    #define WDATA_01 Fld(32,0,AC_FULLDW)//[31:0]
#define SCFIR_05 (IO_SCALER_BASE + 0x514)
    #define WDATA_02 Fld(32,0,AC_FULLDW)//[31:0]
#define SCFIR_06 (IO_SCALER_BASE + 0x518)
    #define RDATA_00 Fld(32,0,AC_FULLDW)//[31:0]
#define SCFIR_07 (IO_SCALER_BASE + 0x51C)
    #define RDATA_01 Fld(32,0,AC_FULLDW)//[31:0]
#define SCFIR_08 (IO_SCALER_BASE + 0x520)
    #define RDATA_02 Fld(32,0,AC_FULLDW)//[31:0]
#define SCFIR_09 (IO_SCALER_BASE + 0x524)
    #define DE_RING_SLOPE_V Fld(4,20,AC_MSKB2)//[23:20]
    #define DE_RING_MAX_THRESHOLD_V Fld(8,12,AC_MSKW21)//[19:12]
    #define DE_RING_SLOPE_V_UV Fld(4,8,AC_MSKB1)//[11:8]
    #define DE_RING_MAX_THRESHOLD_V_UV Fld(8,0,AC_FULLB0)//[7:0]
#define SCFIR_0A (IO_SCALER_BASE + 0x528)
    #define DE_RING_SLOPE_H_Y Fld(4,20,AC_MSKB2)//[23:20]
    #define DE_RING_MAX_THRESHOLD_H_Y Fld(8,12,AC_MSKW21)//[19:12]
    #define DE_RING_FIRST_SLOPE_H_Y Fld(4,8,AC_MSKB1)//[11:8]
    #define DE_RING_FIRST_THRESHOLD_H_Y Fld(8,0,AC_FULLB0)//[7:0]
#define SCFIR_0B (IO_SCALER_BASE + 0x52C)
    #define RING_THR_H_UV Fld(8,20,AC_MSKW32)//[27:20]
    #define SLOPE_SEL_H_UV Fld(4,16,AC_MSKB2)//[19:16]
    #define RING_1ST_THR_H_UV Fld(8,8,AC_FULLB1)//[15:8]
    #define SLOPE_1ST_SEL_H_UV Fld(4,4,AC_MSKB0)//[7:4]
#define SCFIR_0C (IO_SCALER_BASE + 0x530)
    #define V_PR_COEFF Fld(1,3,AC_MSKB0)//[3:3]
    #define H_CV422_COEFF Fld(1,2,AC_MSKB0)//[2:2]
    #define H_SRAM_COEFF_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define V_SRAM_COEFF_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCFIR_0D (IO_SCALER_BASE + 0x534)
    #define WDATA_03 Fld(24,0,AC_MSKDW)//[23:0]
#define SCFIR_0E (IO_SCALER_BASE + 0x538)
    #define RDATA_03 Fld(24,0,AC_MSKDW)//[23:0]
#define SCFIR_0F (IO_SCALER_BASE + 0x53C)
    #define ADAPT_2D_RATIO Fld(4,28,AC_MSKB3)//[31:28]
    #define ADAPT_SLOPE_H Fld(4,24,AC_MSKB3)//[27:24]
    #define ADAPT_EDGE_THR_H Fld(6,18,AC_MSKB2)//[23:18]
    #define ADAPT_DC_CORING_H Fld(5,13,AC_MSKW21)//[17:13]
    #define ADAPT_VAR_CORING_H Fld(5,8,AC_MSKB1)//[12:8]
    #define ADAPT_BACK_RATIO_H Fld(6,2,AC_MSKB0)//[7:2]
#define SCFIR_10 (IO_SCALER_BASE + 0x540)
    #define ADAPT_INK_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define ADAPT_SLOPE_V Fld(4,24,AC_MSKB3)//[27:24]
    #define ADAPT_EDGE_THR_V Fld(6,18,AC_MSKB2)//[23:18]
    #define ADAPT_DC_CORING_V Fld(5,13,AC_MSKW21)//[17:13]
    #define ADAPT_VAR_CORING_V Fld(5,8,AC_MSKB1)//[12:8]
    #define ADAPT_BACK_RATIO_V Fld(6,2,AC_MSKB0)//[7:2]
#define SCFIR_11 (IO_SCALER_BASE + 0x544)
    #define VERT_CTI_CORING Fld(6,26,AC_MSKB3)//[31:26]
    #define VERT_CTI_GAIN Fld(8,18,AC_MSKW32)//[25:18]
    #define VERT_CTI_LIMIT Fld(8,10,AC_MSKW21)//[17:10]
    #define VERT_CTI_CLIP_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define VERT_CTI_CLIP_TAP Fld(1,8,AC_MSKB1)//[8:8]
    #define VERT_CTI_CLIP_THR Fld(6,2,AC_MSKB0)//[7:2]
#define SCFIR_12 (IO_SCALER_BASE + 0x548)
    #define VERT_CTI_CLIP_RATIO Fld(4,28,AC_MSKB3)//[31:28]
    #define VERT_CTI_INK_MODE Fld(2,26,AC_MSKB3)//[27:26]

//Page SCPQ
#define SCPQ_03 (IO_SCALER_BASE + 0x00C)
    #define SC_DITHER_FPHASE_BIT Fld(3,29,AC_MSKB3)//[31:29]
    #define SC_DITHER_FPHASE_SEL Fld(2,27,AC_MSKB3)//[28:27]
    #define SC_DITHER_FPHASE_CTRL Fld(2,25,AC_MSKB3)//[26:25]
    #define SC_DITHER_FPHASE Fld(6,19,AC_MSKW32)//[24:19]
    #define SC_DITHER_FPHASE_R Fld(1,18,AC_MSKB2)//[18:18]
    #define SC_DITHER_FPHASE_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define SC_DITHER_SUB_B Fld(2,15,AC_MSKW21)//[16:15]
    #define SC_DITHER_SUB_G Fld(2,13,AC_MSKB1)//[14:13]
    #define SC_DITHER_SUB_R Fld(2,11,AC_MSKB1)//[12:11]
    #define SC_DITEHR_SUBPIX_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define SC_DITHER_TABLE_EN Fld(2,8,AC_MSKB1)//[9:8]
    #define SC_DITHER_ROUND_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define SC_DITHER_LFSR_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define SC_DITHER_RDITHER_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define SC_DITHER_ETHER_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define SC_DITHER_LEFT_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SC_DITHER_DRMOD Fld(2,0,AC_MSKB0)//[1:0]
#define SCPQ_04 (IO_SCALER_BASE + 0x010)
    #define SC_DITHER_INK_DATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define SC_DITHER_INK_DATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define SCPQ_05 (IO_SCALER_BASE + 0x014)
    #define SC_DITHER_INK Fld(1,31,AC_MSKB3)//[31:31]
    #define SC_DITHER_LSB_OFF Fld(1,30,AC_MSKB3)//[30:30]
    #define SC_DITHER_H_ACTIVE Fld(16,12,AC_MSKDW)//[27:12]
    #define SC_DITHER_INK_DATA_B Fld(12,0,AC_MSKW10)//[11:0]
#endif

