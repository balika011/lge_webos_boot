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
 * $Date  $
 * $RCSfile: hw_scpos.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

// *********************************************************************
// Down Scaler related Register and Macro
// *********************************************************************
#ifndef _HW_SCPOS_H_
#define _HW_SCPOS_H_

#include "x_hal_5381.h"

// *********************************************************************
// HW register definitions
// *********************************************************************
#define IO_SCPIP_BASE     (IO_VIRT + 0x25000)

//Page SCCTRL1
#define SCPIP_SCCTRL1_00 (IO_SCPIP_BASE + 0x200)
    #define SCCTRL1_00_SC_H_PHASE_INIT_SIGN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_00_SC_H_PHASE_INIT_INTG_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_00_SC_H_PHASE_INIT_FRAC_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_01 (IO_SCPIP_BASE + 0x204)
    #define SCCTRL1_01_SC_H_PHASE_DELTA_INTG_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_01_SC_H_PHASE_DELTA_FRAC_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_02 (IO_SCPIP_BASE + 0x208)
    #define SCCTRL1_02_SC_V_PHASE_INIT_SIGN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_02_SC_V_PHASE_INIT_INTG_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_02_SC_V_PHASE_INIT_FRAC_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_03 (IO_SCPIP_BASE + 0x20C)
    #define SCCTRL1_03_SC_V_PHASE_DELTA_INTG_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_03_SC_V_PHASE_DELTA_FRAC_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_04 (IO_SCPIP_BASE + 0x210)
    #define SCCTRL1_04_SC_H_UV_PHASE_INIT_INTG_1 Fld(3,29,AC_MSKB3)//[31:29]
    #define SCCTRL1_04_SC_H_UV_PHASE_INIT_FRAC_1 Fld(21,8,AC_MSKDW)//[28:8]
    #define SCCTRL1_04_SC_H_UV_PHASE_INIT_SIGN_1 Fld(1,7,AC_MSKB0)//[7:7]
    #define SCCTRL1_04_SC_H_422_MODE_EN_1 Fld(1,6,AC_MSKB0)//[6:6]
    #define SCCTRL1_04_SC_REORDER_UV_INV_1 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_04_SC_COEF_PP_WRT_A Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL1_04_SC_COEF_PP_ENABLE Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL1_04_SC_FIR_DISABLE_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define SCCTRL1_04_SC_V_3D_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL1_04_SC_V_FIELD_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCCTRL1_05 (IO_SCPIP_BASE + 0x214)
    #define SCCTRL1_05_SC_NONL_H_START_POS Fld(12,20,AC_MSKW32)//[31:20]
    #define SCCTRL1_05_SC_NONL_H_STOP_POS Fld(12,8,AC_MSKW21)//[19:8]
    #define SCCTRL1_05_SC_NONL_H_ADD_MINUS_SEL Fld(1,7,AC_MSKB0)//[7:7]
    #define SCCTRL1_05_SC_NONL_H_ENABLE Fld(1,6,AC_MSKB0)//[6:6]
    #define SCCTRL1_05_SC_COEF_PP_RD_A Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_05_SC_MASTER_3D_DISPMODE Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL1_05_SC_3D_MODE Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_SCCTRL1_06 (IO_SCPIP_BASE + 0x218)
    #define SCCTRL1_06_SC_NONL_H_PHASE_DELTA Fld(20,12,AC_MSKDW)//[31:12]
    #define SCCTRL1_06_SC_NONL_H_PHASE_DELTA_FLOAT Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCCTRL1_07 (IO_SCPIP_BASE + 0x21C)
    #define SCCTRL1_07_SC_NONL_H_MIDDLE_POS Fld(12,20,AC_MSKW32)//[31:20]
    #define SCCTRL1_07_SC_H_C_COEF_TABLE_SEL Fld(3,6,AC_MSKW10)//[8:6]
    #define SCCTRL1_07_SC_H_COEF_TABLE_SEL Fld(3,3,AC_MSKB0)//[5:3]
    #define SCCTRL1_07_SC_V_COEF_TABLE_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_SCCTRL1_09 (IO_SCPIP_BASE + 0x224)
    #define SCCTRL1_09_SC_INPUT_WIDTH_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_09_SC_INPUT_HEIGHT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_0A (IO_SCPIP_BASE + 0x228)
    #define SCCTRL1_0A_SC_H_422_MIRROR_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define SCCTRL1_0A_SC_LR_PAIR_CTRL Fld(2,6,AC_MSKB0)//[7:6]
    #define SCCTRL1_0A_SC_CLR_STA_SET_RES_O_1 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_0A_SC_STA_SET_RES_O_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL1_0A_SC_CLR_STA_SET_RES_I_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL1_0A_SC_STA_SET_RES_I_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define SCCTRL1_0A_SC_SET_RES_TOGGLE_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL1_0A_SC_SET_RES_IMPORT_ENABLE_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCCTRL1_0B (IO_SCPIP_BASE + 0x22C)
    #define SCCTRL1_0B_SC_STA_DISPFF_OVFL_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define SCCTRL1_0B_SC_CLR_DISPFF_FLG_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define SCCTRL1_0B_SC_DRAM_RDREQ_DLY_1 Fld(4,20,AC_MSKB2)//[23:20]
    #define SCCTRL1_0B_SC_DISP_FF_WTLV_1 Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCCTRL1_0C (IO_SCPIP_BASE + 0x230)
    #define SCCTRL1_0C_SC_VSC_HEIGHT_MASK_EN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_0C_SC_VERT_CTI_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define SCCTRL1_0C_SC_DERING_V_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define SCCTRL1_0C_SC_DERING_H_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define SCCTRL1_0C_SC_TAP_ADAPT_V_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define SCCTRL1_0C_SC_TAP_ADAPT_H_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define SCCTRL1_0C_SC_MSK_DISPFF_MIB_DISP_R Fld(1,25,AC_MSKB3)//[25:25]
    #define SCCTRL1_0C_SC_PE_BLANK_NUM Fld(8,17,AC_MSKW32)//[24:17]
    #define SCCTRL1_0C_SC_PE_ENABLE Fld(1,16,AC_MSKB2)//[16:16]
    #define SCCTRL1_0C_SC_DRAM_SRC_W_WIDTH_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_11 (IO_SCPIP_BASE + 0x244)
    #define SCCTRL1_11_SC_OUTPUT_WIDTH_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_11_SC_OUTPUT_HEIGHT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_13 (IO_SCPIP_BASE + 0x24C)
    #define SCCTRL1_13_SC_DRAM_3D_ENABLE_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_13_SC_DRAM_YC2YUV_H_BOUND_EN_1 Fld(1,25,AC_MSKB3)//[25:25]
    #define SCCTRL1_13_SC_DRAM_YC2YUV_MIRROR_422_1 Fld(2,23,AC_MSKW32)//[24:23]
    #define SCCTRL1_13_SC_DRAM_YC2YUV_EN_1 Fld(1,22,AC_MSKB2)//[22:22]
    #define SCCTRL1_13_SC_DRAM_YC2YUV_UVINV_1 Fld(1,21,AC_MSKB2)//[21:21]
    #define SCCTRL1_13_SC_DRAM_YUV2YC_OPTION_1 Fld(1,17,AC_MSKB2)//[17:17]
    #define SCCTRL1_13_SC_DRAM_YUV2YC_EN_1 Fld(1,16,AC_MSKB2)//[16:16]
    #define SCCTRL1_13_SC_DRAM_YUV2YC_UVINV_1 Fld(1,15,AC_MSKB1)//[15:15]
    #define SCCTRL1_13_SC_BCK_DRAM_MODE_1 Fld(1,14,AC_MSKB1)//[14:14]
    #define SCCTRL1_13_SC_FRN_DRAM_MODE_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCCTRL1_13_SC_ENG_INPUT_END_DLY_1 Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_SCCTRL1_14 (IO_SCPIP_BASE + 0x250)
    #define SCCTRL1_14_SC_V_DS_PARTIAL_SUM_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_14_SC_H_DS_PARTIAL_SUM_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_15 (IO_SCPIP_BASE + 0x254)
    #define SCCTRL1_15_SC_V_DS_PARTIAL_SUM_EN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_15_SC_H_DS_PARTIAL_SUM_EN_1 Fld(1,30,AC_MSKB3)//[30:30]
#define SCPIP_SCCTRL1_18 (IO_SCPIP_BASE + 0x260)
    #define SCCTRL1_18_SC_V_PHASE_INIT_SIGN_BTM_LEFT_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_18_SC_V_PHASE_INIT_INTG_BTM_LEFT_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_18_SC_V_PHASE_INIT_FRAC_BTM_LEFT_1 Fld(20,8,AC_MSKDW)//[27:8]
    #define SCCTRL1_18_SC_V_PHASE_INIT_3D_FIELD_EN Fld(1,7,AC_MSKB0)//[7:7]
#define SCPIP_SCCTRL1_19 (IO_SCPIP_BASE + 0x264)
    #define SCCTRL1_19_SC_V_PHASE_INIT_SIGN_TOP_RIGHT_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_19_SC_V_PHASE_INIT_INTG_TOP_RIGHT_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_19_SC_V_PHASE_INIT_FRAC_TOP_RIGHT_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_1A (IO_SCPIP_BASE + 0x268)
    #define SCCTRL1_1A_SC_V_PHASE_INIT_SIGN_BTM_RIGHT_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_1A_SC_V_PHASE_INIT_INTG_BTM_RIGHT_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_1A_SC_V_PHASE_INIT_FRAC_BTM_RIGHT_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_1B (IO_SCPIP_BASE + 0x26C)
    #define SCCTRL1_1B_SC_RPT_GEN_AUTO_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define SCCTRL1_1B_SC_DRAM_IMPORT_ENABLE_1 Fld(1,18,AC_MSKB2)//[18:18]
    #define SCCTRL1_1B_SC_STA_DRAM_RPTR_1 Fld(4,14,AC_MSKW21)//[17:14]
    #define SCCTRL1_1B_SC_STA_DRAM_WPTR_1 Fld(4,10,AC_MSKB1)//[13:10]
    #define SCCTRL1_1B_SC_FIX_DRAM_RPTR_1 Fld(4,6,AC_MSKW10)//[9:6]
    #define SCCTRL1_1B_SC_FIX_DRAM_RPTR_EN_1 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_1B_SC_DRAM_FRM_DLY_1 Fld(2,3,AC_MSKB0)//[4:3]
    #define SCCTRL1_1B_SC_DRAM_WR_10B_SEL_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define SCCTRL1_1B_SC_DRAM_W_FREEZE_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL1_1B_SC_DRAM_W_444_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCCTRL1_1C (IO_SCPIP_BASE + 0x270)
    #define SCCTRL1_1C_SC_DRAM_W_WIDTH_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_1C_SC_DRAM_W_HEIGHT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_1D (IO_SCPIP_BASE + 0x274)
    #define SCCTRL1_1D_SC_DRAM_W_HSTR_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_1D_SC_DRAM_W_VSTR_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_1E (IO_SCPIP_BASE + 0x278)
    #define SCCTRL1_1E_SC_WR_DRAM_V_PITCH_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_1E_SC_RPT_GEN_VCNT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_1F (IO_SCPIP_BASE + 0x27C)
    #define SCCTRL1_1F_SC_WR_DRAM_H_PITCH_1 Fld(16,16,AC_FULLW32)//[31:16]
#define SCPIP_SCCTRL1_20 (IO_SCPIP_BASE + 0x280)
    #define SCCTRL1_20_SC_REG_FPHASE_BIT Fld(3,29,AC_MSKB3)//[31:29]
    #define SCCTRL1_20_SC_REG_FPHASE_SEL Fld(2,27,AC_MSKB3)//[28:27]
    #define SCCTRL1_20_SC_REG_FPHASE_CTRL Fld(2,25,AC_MSKB3)//[26:25]
    #define SCCTRL1_20_SC_REG_FPHASE Fld(6,19,AC_MSKW32)//[24:19]
    #define SCCTRL1_20_SC_REG_FPHASE_R Fld(1,18,AC_MSKB2)//[18:18]
    #define SCCTRL1_20_SC_REG_FPHASE_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define SCCTRL1_20_SC_REG_SUB_B Fld(2,15,AC_MSKW21)//[16:15]
    #define SCCTRL1_20_SC_REG_SUB_G Fld(2,13,AC_MSKB1)//[14:13]
    #define SCCTRL1_20_SC_REG_SUB_R Fld(2,11,AC_MSKB1)//[12:11]
    #define SCCTRL1_20_SC_REG_SUBPIX_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define SCCTRL1_20_SC_REG_TABLE_EN Fld(2,8,AC_MSKB1)//[9:8]
    #define SCCTRL1_20_SC_REG_LFSR_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define SCCTRL1_20_SC_REG_RDITHER_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_20_SC_REG_EDITHER_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL1_20_SC_REG_ROUND_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL1_20_SC_REG_LEFT_EN Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_SCCTRL1_21 (IO_SCPIP_BASE + 0x284)
    #define SCCTRL1_21_SC_REG_INK_DATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define SCCTRL1_21_SC_REG_INK_DATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCCTRL1_22 (IO_SCPIP_BASE + 0x288)
    #define SCCTRL1_22_SC_REG_INK Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_22_SC_REG_LSB_OFF Fld(1,30,AC_MSKB3)//[30:30]
    #define SCCTRL1_22_SC_REG_DRMOD Fld(2,12,AC_MSKB1)//[13:12]
    #define SCCTRL1_22_SC_REG_INK_DATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCCTRL1_23 (IO_SCPIP_BASE + 0x28C)
    #define SCCTRL1_23_SC_STA_DRAM_WT_CRC_1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL1_24 (IO_SCPIP_BASE + 0x290)
    #define SCCTRL1_24_SC_STA_DISPFIFO_W_CRC_1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL1_25 (IO_SCPIP_BASE + 0x294)
    #define SCCTRL1_25_SC_HMSK_STR_OFST_1 Fld(17,0,AC_MSKDW)//[16:0]
#define SCPIP_SCCTRL1_26 (IO_SCPIP_BASE + 0x298)
    #define SCCTRL1_26_SC_TG_DLY_1 Fld(2,17,AC_MSKB2)//[18:17]
    #define SCCTRL1_26_SC_TG_DLY_IMPORT_ENABLE_1 Fld(1,16,AC_MSKB2)//[16:16]
    #define SCCTRL1_26_SC_DRAM_SRC_W_HEIGHT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_27 (IO_SCPIP_BASE + 0x29C)
    #define SCCTRL1_27_SC_VMSK_STR_OFST_1 Fld(17,15,AC_MSKDW)//[31:15]
    #define SCCTRL1_27_VDNS_BLANK_NUM_1 Fld(8,7,AC_MSKW10)//[14:7]
#define SCPIP_SCCTRL1_28 (IO_SCPIP_BASE + 0x2A0)
    #define SCCTRL1_28_SC_VMSK_END_CROP_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SCCTRL1_28_SC_VMSK_STR_CROP_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SCCTRL1_28_SC_HMSK_END_CROP_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define SCCTRL1_28_SC_HMSK_STR_CROP_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCCTRL1_29 (IO_SCPIP_BASE + 0x2A4)
    #define SCCTRL1_29_PART_DNSC_V_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_29_PART_DNSC_V_INIT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_30 (IO_SCPIP_BASE + 0x2C0)
    #define SCCTRL1_30_PART_DNSC_V_INIT_RIGHT_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_30_VSC_PART_SEL_1 Fld(1,15,AC_MSKB1)//[15:15]

//Page SCCTRL2
#define SCPIP_SCCTRL2_00 (IO_SCPIP_BASE + 0x400)
    #define SCCTRL2_00_SC_H_PHASE_INIT_SIGN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_00_SC_H_PHASE_INIT_INTG_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL2_00_SC_H_PHASE_INIT_FRAC_2 Fld(15,12,AC_MSKDW)//[26:12]
#define SCPIP_SCCTRL2_01 (IO_SCPIP_BASE + 0x404)
    #define SCCTRL2_01_SC_H_PHASE_DELTA_INTG_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL2_01_SC_H_PHASE_DELTA_FRAC_2 Fld(15,12,AC_MSKDW)//[26:12]
#define SCPIP_SCCTRL2_02 (IO_SCPIP_BASE + 0x408)
    #define SCCTRL2_02_SC_V_PHASE_INIT_SIGN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_02_SC_V_PHASE_INIT_INTG_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL2_02_SC_V_PHASE_INIT_FRAC_2 Fld(15,12,AC_MSKDW)//[26:12]
#define SCPIP_SCCTRL2_03 (IO_SCPIP_BASE + 0x40C)
    #define SCCTRL2_03_SC_V_PHASE_DELTA_INTG_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL2_03_SC_V_PHASE_DELTA_FRAC_2 Fld(15,12,AC_MSKDW)//[26:12]
#define SCPIP_SCCTRL2_04 (IO_SCPIP_BASE + 0x410)
    #define SCCTRL2_04_SC_FIR_DISABLE_2 Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_SCCTRL2_09 (IO_SCPIP_BASE + 0x424)
    #define SCCTRL2_09_SC_INPUT_WIDTH_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_09_SC_INPUT_HEIGHT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_0A (IO_SCPIP_BASE + 0x428)
    #define SCCTRL2_0A_SC_TVE_RPT_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_0A_SC_TVE_RPT_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define SCCTRL2_0A_SC_INV_TVE_FIELD Fld(1,29,AC_MSKB3)//[29:29]
    #define SCCTRL2_0A_SC_CLR_STA_SET_RES_O_2 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL2_0A_SC_STA_SET_RES_O_2 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL2_0A_SC_CLR_STA_SET_RES_I_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL2_0A_SC_STA_SET_RES_I_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define SCCTRL2_0A_SC_SET_RES_TOGGLE_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL2_0A_SC_SET_RES_IMPORT_ENABLE_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCCTRL2_0B (IO_SCPIP_BASE + 0x42C)
    #define SCCTRL2_0B_SC_STA_DISPFF_OVFL_2 Fld(1,28,AC_MSKB3)//[28:28]
    #define SCCTRL2_0B_SC_CLR_DISPFF_FLG_2 Fld(1,27,AC_MSKB3)//[27:27]
    #define SCCTRL2_0B_SC_DRAM_RDREQ_DLY_2 Fld(4,20,AC_MSKB2)//[23:20]
    #define SCCTRL2_0B_SC_DISP_FF_WTLV_2 Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCCTRL2_0C (IO_SCPIP_BASE + 0x430)
    #define SCCTRL2_0C_SC_VSC_HEIGHT_MASK_EN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_0C_SC_DRAM_SRC_W_WIDTH_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_11 (IO_SCPIP_BASE + 0x444)
    #define SCCTRL2_11_SC_OUTPUT_WIDTH_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_11_SC_OUTPUT_HEIGHT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_13 (IO_SCPIP_BASE + 0x44C)
    #define SCCTRL2_13_SC_DRAM_3D_ENABLE_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_13_SC_DRAM_YC2YUV_H_BOUND_EN_2 Fld(1,25,AC_MSKB3)//[25:25]
    #define SCCTRL2_13_SC_DRAM_YC2YUV_MIRROR_422_2 Fld(2,23,AC_MSKW32)//[24:23]
    #define SCCTRL2_13_SC_DRAM_YC2YUV_EN_2 Fld(1,22,AC_MSKB2)//[22:22]
    #define SCCTRL2_13_SC_DRAM_YC2YUV_UVINV_2 Fld(1,21,AC_MSKB2)//[21:21]
    #define SCCTRL2_13_SC_DRAM_YUV2YC_OPTION_2 Fld(1,17,AC_MSKB2)//[17:17]
    #define SCCTRL2_13_SC_DRAM_YUV2YC_EN_2 Fld(1,16,AC_MSKB2)//[16:16]
    #define SCCTRL2_13_SC_DRAM_YUV2YC_UVINV_2 Fld(1,15,AC_MSKB1)//[15:15]
    #define SCCTRL2_13_SC_BCK_DRAM_MODE_2 Fld(1,14,AC_MSKB1)//[14:14]
    #define SCCTRL2_13_SC_FRN_DRAM_MODE_2 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCCTRL2_13_SC_ENG_INPUT_END_DLY_2 Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_SCCTRL2_14 (IO_SCPIP_BASE + 0x450)
    #define SCCTRL2_14_SC_V_DS_PARTIAL_SUM_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_14_SC_H_DS_PARTIAL_SUM_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_15 (IO_SCPIP_BASE + 0x454)
    #define SCCTRL2_15_SC_V_DS_PARTIAL_SUM_EN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_15_SC_H_DS_PARTIAL_SUM_EN_2 Fld(1,30,AC_MSKB3)//[30:30]
#define SCPIP_SCCTRL2_17 (IO_SCPIP_BASE + 0x45C)
    #define SCCTRL2_17_SC_V_DS_INTEGRAL_EN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_17_SC_H_DS_INTEGRAL_EN_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define SCCTRL2_17_SC_MIRROR_EN_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL2_17_SC_FLIP_EN_2 Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_SCCTRL2_1B (IO_SCPIP_BASE + 0x46C)
    #define SCCTRL2_1B_SC_RPT_GEN_AUTO_2 Fld(1,19,AC_MSKB2)//[19:19]
    #define SCCTRL2_1B_SC_DRAM_IMPORT_ENABLE_2 Fld(1,18,AC_MSKB2)//[18:18]
    #define SCCTRL2_1B_SC_STA_DRAM_RPTR_2 Fld(4,14,AC_MSKW21)//[17:14]
    #define SCCTRL2_1B_SC_STA_DRAM_WPTR_2 Fld(4,10,AC_MSKB1)//[13:10]
    #define SCCTRL2_1B_SC_FIX_DRAM_RPTR_2 Fld(4,6,AC_MSKW10)//[9:6]
    #define SCCTRL2_1B_SC_FIX_DRAM_RPTR_EN_2 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL2_1B_SC_DRAM_FRM_DLY_2 Fld(2,3,AC_MSKB0)//[4:3]
    #define SCCTRL2_1B_SC_DRAM_W_FREEZE_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL2_1B_SC_DRAM_W_444_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCCTRL2_1C (IO_SCPIP_BASE + 0x470)
    #define SCCTRL2_1C_SC_DRAM_W_WIDTH_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_1C_SC_DRAM_W_HEIGHT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_1D (IO_SCPIP_BASE + 0x474)
    #define SCCTRL2_1D_SC_DRAM_W_HSTR_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_1D_SC_DRAM_W_VSTR_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_1E (IO_SCPIP_BASE + 0x478)
    #define SCCTRL2_1E_SC_WR_DRAM_V_PITCH_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_1E_SC_RPT_GEN_VCNT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_1F (IO_SCPIP_BASE + 0x47C)
    #define SCCTRL2_1F_SC_WR_DRAM_H_PITCH_2 Fld(16,16,AC_FULLW32)//[31:16]
#define SCPIP_SCCTRL2_23 (IO_SCPIP_BASE + 0x48C)
    #define SCCTRL2_23_SC_STA_DRAM_WT_CRC_2 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_24 (IO_SCPIP_BASE + 0x490)
    #define SCCTRL2_24_SC_STA_DISPFIFO_W_CRC_2 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_25 (IO_SCPIP_BASE + 0x494)
    #define SCCTRL2_25_SC_HMSK_STR_OFST_2 Fld(17,0,AC_MSKDW)//[16:0]
#define SCPIP_SCCTRL2_26 (IO_SCPIP_BASE + 0x498)
    #define SCCTRL2_26_SC_TG_DLY_2 Fld(2,17,AC_MSKB2)//[18:17]
    #define SCCTRL2_26_SC_TG_DLY_IMPORT_ENABLE_2 Fld(1,16,AC_MSKB2)//[16:16]
    #define SCCTRL2_26_SC_DRAM_SRC_W_HEIGHT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_27 (IO_SCPIP_BASE + 0x49C)
    #define SCCTRL2_27_SC_VMSK_STR_OFST_2 Fld(17,15,AC_MSKDW)//[31:15]
    #define SCCTRL2_27_VDNS_BLANK_NUM_2 Fld(8,7,AC_MSKW10)//[14:7]
#define SCPIP_SCCTRL2_28 (IO_SCPIP_BASE + 0x4A0)
    #define SCCTRL2_28_SC_VMSK_END_CROP_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define SCCTRL2_28_SC_VMSK_STR_CROP_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define SCCTRL2_28_SC_HMSK_END_CROP_2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SCCTRL2_28_SC_HMSK_STR_CROP_2 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCCTRL2_29 (IO_SCPIP_BASE + 0x4A4)
    #define SCCTRL2_29_PART_DNSC_V_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_29_PART_DNSC_V_INIT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL2_30 (IO_SCPIP_BASE + 0x4C0)
    #define SCCTRL2_30_PART_DNSC_V_INIT_RIGHT_2 Fld(16,16,AC_FULLW32)//[31:16]
#define SCPIP_SCCTRL2_33 (IO_SCPIP_BASE + 0x4CC)
    #define SCCTRL2_33_SC_SCCTRL2_RESERVED0 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_34 (IO_SCPIP_BASE + 0x4D0)
    #define SCCTRL2_34_SC_SCCTRL2_RESERVED1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_35 (IO_SCPIP_BASE + 0x4D4)
    #define SCCTRL2_35_SC_SCCTRL2_RESERVED2 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_36 (IO_SCPIP_BASE + 0x4D8)
    #define SCCTRL2_36_SC_SCCTRL2_RESERVED3 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_37 (IO_SCPIP_BASE + 0x4DC)
    #define SCCTRL2_37_SC_SCCTRL2_RESERVED4 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_38 (IO_SCPIP_BASE + 0x4E0)
    #define SCCTRL2_38_SC_SCCTRL2_RESERVED5 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_39 (IO_SCPIP_BASE + 0x4E4)
    #define SCCTRL2_39_SC_SCCTRL2_RESERVED6 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_3A (IO_SCPIP_BASE + 0x4E8)
    #define SCCTRL2_3A_SC_SCCTRL2_RESERVED7 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL2_ATPG (IO_SCPIP_BASE + 0x4FC)
    #define SCCTRL2_ATPG_SCCTRL2_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL2_ATPG_SCCTRL2_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]   

//Page SCSYS  //mt5399
#define SCPIP_SCSYS_40 (IO_SCPIP_BASE + 0x100)
    #define SCSYS_40_SC_MIN_SELFGEN_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_40_SC_POUT_CRC_VSYNC_INV Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_40_SC_POUT_CRC_CLIP_V_START Fld(12,18,AC_MSKW32)//[29:18]
    #define SCSYS_40_SC_POUT_CRC_CLIP_H_START Fld(13,5,AC_MSKDW)//[17:5]
    #define SCSYS_40_SC_POUT_CRC_CHECK_SUM_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_40_SC_POUT_CRC_BOT_R_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_40_SC_POUT_CRC_TOP_R_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_40_SC_POUT_CRC_BOT_L_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_40_SC_POUT_CRC_TOP_L_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_41 (IO_SCPIP_BASE + 0x104)
    #define SCSYS_41_SC_POUT_CRC_CLIP_V_END Fld(12,13,AC_MSKDW)//[24:13]
    #define SCSYS_41_SC_POUT_CRC_CLIP_H_END Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_SCSYS_42 (IO_SCPIP_BASE + 0x108)
    #define SCSYS_42_SC_POUT_CRC_V_IN_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define SCSYS_42_SC_POUT_CRC_C_IN_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define SCSYS_42_SC_POUT_CRC_Y_IN_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_SCSYS_43 (IO_SCPIP_BASE + 0x10C)
    #define SCSYS_43_SC_POUT_STA_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_44 (IO_SCPIP_BASE + 0x110)
    #define SCSYS_44_SC_POUT_STA_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_45 (IO_SCPIP_BASE + 0x114)
    #define SCSYS_45_SC_MIN_SELF_HACT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_45_SC_MIN_SELF_HTOT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_46 (IO_SCPIP_BASE + 0x118)
    #define SCSYS_46_SC_MIN_SELF_VACT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_46_SC_MIN_SELF_VTOT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_47 (IO_SCPIP_BASE + 0x11C)
    #define SCSYS_47_SC_STA_H_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_47_SC_STA_V_CNT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_48 (IO_SCPIP_BASE + 0x120)
    #define SCSYS_48_SC_OUT_MEASURE_HACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_48_SC_OUT_MEASURE_VACTIVE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_49 (IO_SCPIP_BASE + 0x124)
    #define SCSYS_49_SC_SET_DATA_HTOT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_49_SC_SET_DATA_HACT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_50 (IO_SCPIP_BASE + 0x140)
    #define SCSYS_50_SC_SET_DATA_VTOT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_51 (IO_SCPIP_BASE + 0x144)
    #define SCSYS_51_SC_P_CTI_REG_H_BOUND_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_51_SC_P_CTI_MIRROR_422 Fld(2,7,AC_MSKW10)//[8:7]
    #define SCSYS_51_SC_P_CTI_UVINV Fld(1,6,AC_MSKB0)//[6:6]
    #define SCSYS_51_SC_P_CTI_SEL444 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_51_SC_M_CTI_REG_H_BOUND_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_51_SC_M_CTI_MIRROR_422 Fld(2,2,AC_MSKB0)//[3:2]
    #define SCSYS_51_SC_M_CTI_UVINV Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_51_SC_M_CTI_SEL444 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_52 (IO_SCPIP_BASE + 0x148)
    #define SCSYS_52_SC_M_REGEN_SET_RES_FINISHED_O Fld(1,29,AC_MSKB3)//[29:29]
    #define SCSYS_52_SC_M_REGEN_SET_RES_FINISHED_I Fld(1,28,AC_MSKB3)//[28:28]
    #define SCSYS_52_SC_M_REGEN_SET_RES_CLEAR_O Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_52_SC_M_REGEN_SET_RES_CLEAR_I Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_52_SC_M_REGEN_SET_RES_ACTIVE Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_52_SC_M_REGEN_IN_FORMAT Fld(4,12,AC_MSKB1)//[15:12]
    #define SCSYS_52_SC_M_REGEN_BYPASS_SET_RES_DB Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_52_SC_M_REGEN_IN_DISP_R_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_52_SC_M_REGEN_LR_PAIR_CTRL Fld(2,8,AC_MSKB1)//[9:8]
    #define SCSYS_52_SC_M_REGEN_SET_RES_MODE Fld(2,6,AC_MSKB0)//[7:6]
    #define SCSYS_52_SC_M_REGEN_SET_RES_IN_TRIG_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_52_SC_M_REGEN_EVEN_ONLY Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_52_SC_M_REGEN_ODD_ONLY Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_52_SC_M_REGEN_CLIP_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_52_SC_M_REGEN_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_52_SC_M_REGEN_BYPASS Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_53 (IO_SCPIP_BASE + 0x14C)
    #define SCSYS_53_SC_M_REGEN_IN_ACTIVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_53_SC_M_REGEN_IN_ACTIVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_54 (IO_SCPIP_BASE + 0x150)
    #define SCSYS_54_SC_M_REGEN_OUT_ACTIVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_54_SC_M_REGEN_OUT_ACTIVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_55 (IO_SCPIP_BASE + 0x154)
    #define SCSYS_55_SC_M_REGEN_V_START_L Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_55_SC_M_REGEN_H_START_L Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_56 (IO_SCPIP_BASE + 0x158)
    #define SCSYS_56_SC_M_REGEN_V_START_R Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_56_SC_M_REGEN_H_START_R Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_57 (IO_SCPIP_BASE + 0x15C)
    #define SCSYS_57_SC_M_REGEN_IN_HEIGHT_MEASURE Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_57_SC_M_REGEN_IN_WIDTH_MEASURE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_58 (IO_SCPIP_BASE + 0x160)
    #define SCSYS_58_SC_M_REGEN_OUT_HEIGHT_MEASURE Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_58_SC_M_REGEN_OUT_WIDTH_MEASURE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_59 (IO_SCPIP_BASE + 0x164)
    #define SCSYS_59_SC_P_REGEN_SET_RES_FINISHED_O Fld(1,29,AC_MSKB3)//[29:29]
    #define SCSYS_59_SC_P_REGEN_SET_RES_FINISHED_I Fld(1,28,AC_MSKB3)//[28:28]
    #define SCSYS_59_SC_P_REGEN_SET_RES_CLEAR_O Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_59_SC_P_REGEN_SET_RES_CLEAR_I Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_59_SC_P_REGEN_SET_RES_ACTIVE Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_59_SC_P_REGEN_IN_FORMAT Fld(4,12,AC_MSKB1)//[15:12]
    #define SCSYS_59_SC_P_REGEN_BYPASS_SET_RES_DB Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_59_SC_P_REGEN_IN_DISP_R_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_59_SC_P_REGEN_LR_PAIR_CTRL Fld(2,8,AC_MSKB1)//[9:8]
    #define SCSYS_59_SC_P_REGEN_SET_RES_MODE Fld(2,6,AC_MSKB0)//[7:6]
    #define SCSYS_59_SC_P_REGEN_SET_RES_IN_TRIG_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_59_SC_P_REGEN_EVEN_ONLY Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_59_SC_P_REGEN_ODD_ONLY Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_59_SC_P_REGEN_CLIP_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_59_SC_P_REGEN_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_59_SC_P_REGEN_BYPASS Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_5A (IO_SCPIP_BASE + 0x168)
    #define SCSYS_5A_SC_P_REGEN_IN_ACTIVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_5A_SC_P_REGEN_IN_ACTIVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_5B (IO_SCPIP_BASE + 0x16C)
    #define SCSYS_5B_SC_P_REGEN_OUT_ACTIVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_5B_SC_P_REGEN_OUT_ACTIVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_5C (IO_SCPIP_BASE + 0x170)
    #define SCSYS_5C_SC_P_REGEN_V_START_L Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_5C_SC_P_REGEN_H_START_L Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_5D (IO_SCPIP_BASE + 0x174)
    #define SCSYS_5D_SC_P_REGEN_V_START_R Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_5D_SC_P_REGEN_H_START_R Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_5E (IO_SCPIP_BASE + 0x178)
    #define SCSYS_5E_SC_P_REGEN_IN_HEIGHT_MEASURE Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_5E_SC_P_REGEN_IN_WIDTH_MEASURE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_5F (IO_SCPIP_BASE + 0x17C)
    #define SCSYS_5F_SC_P_REGEN_OUT_HEIGHT_MEASURE Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_5F_SC_P_REGEN_OUT_WIDTH_MEASURE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_60 (IO_SCPIP_BASE + 0x180)
    #define SCSYS_60_SC_TVE_MASK_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define SCSYS_60_SC_TVE_MASK_V Fld(8,16,AC_FULLB2)//[23:16]
    #define SCSYS_60_SC_TVE_MASK_U Fld(8,8,AC_FULLB1)//[15:8]
    #define SCSYS_60_SC_TVE_MASK_Y Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCSYS_77 (IO_SCPIP_BASE + 0x1DC)
    #define SCSYS_77_SC_SCSYS_RESERVED0 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_78 (IO_SCPIP_BASE + 0x1E0)
    #define SCSYS_78_SC_SCSYS_RESERVED1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_79 (IO_SCPIP_BASE + 0x1E4)
    #define SCSYS_79_SC_SCSYS_RESERVED2 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_7A (IO_SCPIP_BASE + 0x1E8)
    #define SCSYS_7A_SC_SCSYS_RESERVED3 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_7B (IO_SCPIP_BASE + 0x1EC)
    #define SCSYS_7B_SC_SCSYS_RESERVED4 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_7C (IO_SCPIP_BASE + 0x1F0)
    #define SCSYS_7C_SC_SCSYS_RESERVED5 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_7D (IO_SCPIP_BASE + 0x1F4)
    #define SCSYS_7D_SC_SCSYS_RESERVED6 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_7E (IO_SCPIP_BASE + 0x1F8)
    #define SCSYS_7E_SC_SCSYS_RESERVED7 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_ATPG (IO_SCPIP_BASE + 0x1FC)
    #define SCSYS_ATPG_SCSYS_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_ATPG_SCSYS_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_00 (IO_SCPIP_BASE + 0x300)
    #define SCSYS_00_C_GFX_CLK_CLKEN Fld(1,18,AC_MSKB2)//[18:18]
    #define SCSYS_00_SC_GFX_CLK_SWRST Fld(1,17,AC_MSKB2)//[17:17]
    #define SCSYS_00_SC_BYPASS_SCALER_2 Fld(1,16,AC_MSKB2)//[16:16]
    #define SCSYS_00_SC_BYPASS_SCALER_1 Fld(1,15,AC_MSKB1)//[15:15]
    #define SCSYS_00_C_PIP_OCLK_CLKEN Fld(1,14,AC_MSKB1)//[14:14]
    #define SCSYS_00_C_PIP_ICLK_CLKEN Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_00_C_PIP_MCLK_CLKEN Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_00_C_MAIN_OCLK_CLKEN Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_00_C_MAIN_ICLK_CLKEN Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_00_C_MAIN_MCLK_CLKEN Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_00_C_USE_INTL_CKEN Fld(1,8,AC_MSKB1)//[8:8]
    #define SCSYS_00_SC_MAIN_M_CLK_SWRST Fld(1,6,AC_MSKB0)//[6:6]
    #define SCSYS_00_SC_PIP_M_CLK_SWRST Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_00_SC_PIP_O_CLK_SWRST Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_00_SC_PIP_CLK_SWRST Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_00_SC_M_CLK_SWRST Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_00_SC_MAIN_O_CLK_SWRST Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_00_SC_MAIN_CLK_SWRST Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_01 (IO_SCPIP_BASE + 0x304)
    #define SCSYS_01_SC_TVE_SET_TOGGLE Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_01_SC_SET_DATA Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_01_SC_CLR_INPUT_MEASURE_2 Fld(1,29,AC_MSKB3)//[29:29]
    #define SCSYS_01_SC_CLR_INPUT_MEASURE_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define SCSYS_01_SC_STA_P_TG_UNDERFLOW Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_01_SC_CLR_PTG_UDFL_FLG Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_01_SC_LINESYNC_MODE Fld(1,23,AC_MSKB2)//[23:23]
    #define SCSYS_01_SC_SCALER_DISP_MODE Fld(1,22,AC_MSKB2)//[22:22]
    #define SCSYS_01_SC_CLR_MTG_UDFL_FLG Fld(1,21,AC_MSKB2)//[21:21]
    #define SCSYS_01_SC_STA_M_TG_UNDERFLOW Fld(1,20,AC_MSKB2)//[20:20]
    #define SCSYS_01_SC_STA_SET_DATA Fld(1,19,AC_MSKB2)//[19:19]
    #define SCSYS_01_SC_TG_DISP_R_FREERUN_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define SCSYS_01_SC_TG_TVE_IMPORT_ENABLE Fld(1,17,AC_MSKB2)//[17:17]
    #define SCSYS_01_SC_TG_IMPORT_ENABLE Fld(1,16,AC_MSKB2)//[16:16]
    #define SCSYS_01_SC_TG_IMPORT_VCNT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_02 (IO_SCPIP_BASE + 0x308)
    #define SCSYS_02_SC_PNL_WIDTH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_02_SC_PNL_HEIGHT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_03 (IO_SCPIP_BASE + 0x30C)
    #define SCSYS_03_SC_PNL_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_03_SC_PNL_VTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_04 (IO_SCPIP_BASE + 0x310)
    #define SCSYS_04_SC_PNL_BACK_HPORCH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_04_SC_PNL_FRNT_HPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_05 (IO_SCPIP_BASE + 0x314)
    #define SCSYS_05_SC_PNL_BACK_VPORCH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_05_SC_PNL_FRNT_VPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_06 (IO_SCPIP_BASE + 0x318)
    #define SCSYS_06_SC_DRAM_RDREQ_DLY_2 Fld(4,21,AC_MSKW32)//[24:21]
    #define SCSYS_06_SC_DRAM_RDREQ_DLY_1 Fld(4,17,AC_MSKB2)//[20:17]
#define SCPIP_SCSYS_07 (IO_SCPIP_BASE + 0x31C)
    #define SCSYS_07_SC_SCALER_DEBUG_SEL Fld(8,24,AC_FULLB3)//[31:24]
    #define SCSYS_07_SC_PIP_CEN_SEL Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_07_SC_PIP_HSYNC_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_07_SC_PIP_RST_VCNT Fld(4,8,AC_MSKB1)//[11:8]
    #define SCSYS_07_SC_PIP2TVE_ENABLE Fld(1,7,AC_MSKB0)//[7:7]
    #define SCSYS_07_SC_PIP2TVE_INTERLACED Fld(1,6,AC_MSKB0)//[6:6]
    #define SCSYS_07_SC_PIP2TVE_FLD_INV Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_07_SC_INV_TVE_VSYNC Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_07_SC_INV_TVE_HDE Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_07_SC_PIP_RGB2YUV_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_07_SC_PIP_SRC_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define SCPIP_SCSYS_08 (IO_SCPIP_BASE + 0x320)
    #define SCSYS_08_SC_FRM_MASK_O_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define SCSYS_08_SC_FRM_MASK_I_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define SCSYS_08_SC_INV_OUTPUT_DISP_R Fld(1,19,AC_MSKB2)//[19:19]
    #define SCSYS_08_SC_CLR_STA_INPUTV_ABNORMAL Fld(1,18,AC_MSKB2)//[18:18]
#define SCPIP_SCSYS_09 (IO_SCPIP_BASE + 0x324)
    #define SCSYS_09_INT_MON_DRAM_WR_END_CLR Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_09_INT_GFX_DRAM_WR_END_CLR Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_09_INT_P_DRAM_WR_END_CLR Fld(1,29,AC_MSKB3)//[29:29]
    #define SCSYS_09_INT_M_DRAM_WR_END_CLR Fld(1,28,AC_MSKB3)//[28:28]
    #define SCSYS_09_INT_PSC_OUT_VDE_F_CLR Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_09_INT_PSC_OUT_VDE_R_CLR Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_09_INT_PSC_OUT_VSYNC_F_CLR Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_09_INT_PSC_OUT_VSYNC_R_CLR Fld(1,24,AC_MSKB3)//[24:24]
    #define SCSYS_09_INT_FSC_OUT_VDE_F_CLR Fld(1,23,AC_MSKB2)//[23:23]
    #define SCSYS_09_INT_FSC_OUT_VDE_R_CLR Fld(1,22,AC_MSKB2)//[22:22]
    #define SCSYS_09_INT_FSC_OUT_VSYNC_F_CLR Fld(1,21,AC_MSKB2)//[21:21]
    #define SCSYS_09_INT_FSC_OUT_VSYNC_R_CLR Fld(1,20,AC_MSKB2)//[20:20]
    #define SCSYS_09_INT_P_IN_VSYNC_F_CLR Fld(1,19,AC_MSKB2)//[19:19]
    #define SCSYS_09_INT_P_IN_VSYNC_R_CLR Fld(1,18,AC_MSKB2)//[18:18]
    #define SCSYS_09_INT_M_IN_VSYNC_F_CLR Fld(1,17,AC_MSKB2)//[17:17]
    #define SCSYS_09_INT_M_IN_VSYNC_R_CLR Fld(1,16,AC_MSKB2)//[16:16]
    #define SCSYS_09_INT_MON_DRAM_WR_END_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define SCSYS_09_INT_GFX_DRAM_WR_END_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define SCSYS_09_INT_P_DRAM_WR_END_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_09_INT_M_DRAM_WR_END_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_09_INT_PSC_OUT_VDE_F_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_09_INT_PSC_OUT_VDE_R_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_09_INT_PSC_OUT_VSYNC_F_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_09_INT_PSC_OUT_VSYNC_R_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define SCSYS_09_INT_FSC_OUT_VDE_F_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define SCSYS_09_INT_FSC_OUT_VDE_R_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define SCSYS_09_INT_FSC_OUT_VSYNC_F_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_09_INT_FSC_OUT_VSYNC_R_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_09_INT_P_IN_VSYNC_F_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_09_INT_P_IN_VSYNC_R_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_09_INT_M_IN_VSYNC_F_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_09_INT_M_IN_VSYNC_R_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_0A (IO_SCPIP_BASE + 0x328)
    #define SCSYS_0A_INT_STA_MON_DRAM_WR_END Fld(1,15,AC_MSKB1)//[15:15]
    #define SCSYS_0A_INT_STA_GFX_DRAM_WR_END Fld(1,14,AC_MSKB1)//[14:14]
    #define SCSYS_0A_INT_STA_P_DRAM_WR_END Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_0A_INT_STA_M_DRAM_WR_END Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_0A_INT_STA_PSC_OUT_VDE_F Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_0A_INT_STA_PSC_OUT_VDE_R Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_0A_INT_STA_PSC_OUT_VSYNC_F Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_0A_INT_STA_PSC_OUT_VSYNC_R Fld(1,8,AC_MSKB1)//[8:8]
    #define SCSYS_0A_INT_STA_FSC_OUT_VDE_F Fld(1,7,AC_MSKB0)//[7:7]
    #define SCSYS_0A_INT_STA_FSC_OUT_VDE_R Fld(1,6,AC_MSKB0)//[6:6]
    #define SCSYS_0A_INT_STA_FSC_OUT_VSYNC_F Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_0A_INT_STA_FSC_OUT_VSYNC_R Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_0A_INT_STA_P_IN_VSYNC_F Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_0A_INT_STA_P_IN_VSYNC_R Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_0A_INT_STA_M_IN_VSYNC_F Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_0A_INT_STA_M_IN_VSYNC_R Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_0B (IO_SCPIP_BASE + 0x32C)
    #define SCSYS_0B_SC_GFX_WIDTH Fld(12,20,AC_MSKW32)//[31:20]
    #define SCSYS_0B_SC_GFX_HEIGHT Fld(12,8,AC_MSKW21)//[19:8]
    #define SCSYS_0B_SC_GFX_SET_RES_ENABLE Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_0B_SC_GFX_SRC_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_SCSYS_0C (IO_SCPIP_BASE + 0x330)
    #define SCSYS_0C_SC_GFX_CONTROL_SEL Fld(3,25,AC_MSKB3)//[27:25]
    #define SCSYS_0C_SC_GFX_IN_OFFSET_0P Fld(11,14,AC_MSKDW)//[24:14]
    #define SCSYS_0C_SC_GFX_OUT_S2P Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_0C_SC_GFX_OUT_S1P Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_0C_SC_GFX_OUT_S0P Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_0C_SC_GFX_CLAMP_S2P Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_0C_SC_GFX_CLAMP_S1P Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_0C_SC_GFX_CLAMP_S0P Fld(1,8,AC_MSKB1)//[8:8]
    #define SCSYS_0C_SC_GFX_IN_S2P Fld(1,7,AC_MSKB0)//[7:7]
    #define SCSYS_0C_SC_GFX_IN_S1P Fld(1,6,AC_MSKB0)//[6:6]
    #define SCSYS_0C_SC_GFX_IN_S0P Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_0C_SC_GFX_EASY_BP3X3 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_0C_SC_GFX_CHRM_DEL_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_0C_SC_GFX_YC2YUV_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_0C_SC_GFX_UVINV Fld(1,1,AC_MSKB0)//[1:1]
#define SCPIP_SCSYS_0D (IO_SCPIP_BASE + 0x334)
    #define SCSYS_0D_SC_GFX_ERR_DFN Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_0D_SC_GFX_REG_INDEX Fld(5,26,AC_MSKB3)//[30:26]
    #define SCSYS_0D_SC_GFX_INPUT_SELP Fld(4,22,AC_MSKW32)//[25:22]
    #define SCSYS_0D_SC_GFX_IN_OFFSET_2P Fld(11,11,AC_MSKW21)//[21:11]
    #define SCSYS_0D_SC_GFX_IN_OFFSET_1P Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCSYS_0E (IO_SCPIP_BASE + 0x338)
    #define SCSYS_0E_SC_GFX_OUT_OFFSET_1P Fld(11,11,AC_MSKW21)//[21:11]
    #define SCSYS_0E_SC_GFX_OUT_OFFSET_0P Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCSYS_0F (IO_SCPIP_BASE + 0x33C)
    #define SCSYS_0F_SC_STA_GFX_3X3 Fld(14,11,AC_MSKDW)//[24:11]
    #define SCSYS_0F_SC_GFX_OUT_OFFSET_2P Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCSYS_10 (IO_SCPIP_BASE + 0x340)
    #define SCSYS_10_SC_INV_OUTPUT_HSYNC Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_10_SC_INV_OUTPUT_VSYNC Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_10_SC_INV_OUTPUT_HDE Fld(1,29,AC_MSKB3)//[29:29]
    #define SCSYS_10_SC_INV_OUTPUT_VDE Fld(1,28,AC_MSKB3)//[28:28]
    #define SCSYS_10_SC_FRM_MASK_IN_LR_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_10_SC_FRM_MASK_IN_LR_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_10_SC_FRM_MASK_OUT_LR_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_10_SC_FRM_MASK_OUT_LR_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define SCSYS_10_SC_FRM_MASK_I Fld(4,20,AC_MSKB2)//[23:20]
    #define SCSYS_10_SC_FRM_MASK_O Fld(4,16,AC_MSKB2)//[19:16]
    #define SCSYS_10_SC_FRM_TRK_LINE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_11 (IO_SCPIP_BASE + 0x344)
    #define SCSYS_11_SC_DDDS_TRK_INV Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_11_SC_FRM_TRK_DDDS_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_11_SC_INV_OSD_TVE_VSYNC Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_11_SC_INV_INPUT_P_FIELD Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_11_SC_INV_INPUT_M_FIELD Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_11_SC_INV_MIB_DISP_R Fld(1,24,AC_MSKB3)//[24:24]
    #define SCSYS_11_SC_INV_INPUT_M_DISP_R Fld(1,23,AC_MSKB2)//[23:23]
    #define SCSYS_11_SC_INV_REF_DISP_R Fld(1,22,AC_MSKB2)//[22:22]
    #define SCSYS_11_SC_INV_REF_VSYNC_IN Fld(1,21,AC_MSKB2)//[21:21]
    #define SCSYS_11_SC_INV_INPUT_PVDE Fld(1,20,AC_MSKB2)//[20:20]
    #define SCSYS_11_SC_INV_INPUT_PHDE Fld(1,19,AC_MSKB2)//[19:19]
    #define SCSYS_11_SC_INV_INPUT_MVDE Fld(1,18,AC_MSKB2)//[18:18]
    #define SCSYS_11_SC_INV_INPUT_MHDE Fld(1,17,AC_MSKB2)//[17:17]
    #define SCSYS_11_SC_INV_HMSK Fld(1,16,AC_MSKB2)//[16:16]
    #define SCSYS_11_SC_INV_VMSK Fld(1,15,AC_MSKB1)//[15:15]
    #define SCSYS_11_SC_FRM_TRK_OUT_SEL Fld(2,13,AC_MSKB1)//[14:13]
    #define SCSYS_11_SC_FRM_TRK_IN_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_11_SC_FRM_TRK_ONCE_ONLY Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_11_SC_FRM_TRK_VIN_EDGE_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_11_SC_TG_DAT_RATIO Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_SCSYS_12 (IO_SCPIP_BASE + 0x348)
    #define SCSYS_12_SC_INOUT_VSYNC_DIST_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_12_SC_INOUT_VSYNC_DIST_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_13 (IO_SCPIP_BASE + 0x34C)
    #define SCSYS_13_SC_STA_FRM_VSYNC_OUT_EARLY Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_13_SC_P_HMSK_DLY Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_14 (IO_SCPIP_BASE + 0x350)
    #define SCSYS_14_SC_DDDS_TURBO_RGN Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_14_SC_OSD_HSYNC_SEL Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_14_SC_TVE_SRC_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_14_SC_OSD_SRC_SEL Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_14_SC_OSD_VSYNC_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_14_SC_OSD_TVE_VSYNC_SEL Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_14_SC_STA_FRM_TRK_LOCK_OK Fld(1,8,AC_MSKB1)//[8:8]
    #define SCSYS_14_SC_FRM_TRK_STEP Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCSYS_15 (IO_SCPIP_BASE + 0x354)
    #define SCSYS_15_SC_FRM_LOCK_TOR Fld(8,24,AC_FULLB3)//[31:24]
    #define SCSYS_15_SC_FRM_LOCK_WIN Fld(8,16,AC_FULLB2)//[23:16]
    #define SCSYS_15_SC_OSD_TVE_VSYNC_DLY Fld(8,8,AC_FULLB1)//[15:8]
    #define SCSYS_15_SC_OSD_VSYNC_DLY Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCSYS_16 (IO_SCPIP_BASE + 0x358)
    #define SCSYS_16_SC_TVE_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_16_SC_TVE_HTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_17 (IO_SCPIP_BASE + 0x35C)
    #define SCSYS_17_SC_TVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_17_SC_TVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_18 (IO_SCPIP_BASE + 0x360)
    #define SCSYS_18_SC_TVE_FRNT_HPORCH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_18_SC_TVE_BACK_HPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_19 (IO_SCPIP_BASE + 0x364)
    #define SCSYS_19_SC_TVE_FRNT_VPORCH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_19_SC_TVE_BACK_VPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_1A (IO_SCPIP_BASE + 0x368)
    #define SCSYS_1A_SC_GFX_COEFF_01P Fld(12,12,AC_MSKW21)//[23:12]
    #define SCSYS_1A_SC_GFX_COEFF_00P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1B (IO_SCPIP_BASE + 0x36C)
    #define SCSYS_1B_SC_GFX_COEFF_10P Fld(12,12,AC_MSKW21)//[23:12]
    #define SCSYS_1B_SC_GFX_COEFF_02P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1C (IO_SCPIP_BASE + 0x370)
    #define SCSYS_1C_SC_GFX_COEFF_12P Fld(12,12,AC_MSKW21)//[23:12]
    #define SCSYS_1C_SC_GFX_COEFF_11P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1D (IO_SCPIP_BASE + 0x374)
    #define SCSYS_1D_SC_GFX_COEFF_21P Fld(12,12,AC_MSKW21)//[23:12]
    #define SCSYS_1D_SC_GFX_COEFF_20P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1E (IO_SCPIP_BASE + 0x378)
    #define SCSYS_1E_SC_STA_PD_INFO_IN Fld(3,28,AC_MSKB3)//[30:28]
    #define SCSYS_1E_SC_STA_FIELD_OUT Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_1E_SC_STA_PIP_FIELD_IN Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_1E_SC_STA_MAIN_FIELD_IN Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_1E_SC_STA_OUTPUT_DISP_R Fld(1,24,AC_MSKB3)//[24:24]
    #define SCSYS_1E_SC_STA_INPUT_DISP_R Fld(1,23,AC_MSKB2)//[23:23]
    #define SCSYS_1E_SC_INV_MEASURE_H Fld(1,22,AC_MSKB2)//[22:22]
    #define SCSYS_1E_SC_INV_MEASURE_V Fld(1,21,AC_MSKB2)//[21:21]
    #define SCSYS_1E_SC_CLR_OUTPUT_MEASURE Fld(1,20,AC_MSKB2)//[20:20]
    #define SCSYS_1E_SC_OUTPUT_MEASURE_SEL Fld(2,18,AC_MSKB2)//[19:18]
    #define SCSYS_1E_SC_P_AFCTI_SEL444 Fld(1,17,AC_MSKB2)//[17:17]
    #define SCSYS_1E_SC_P_AFCTI_UVINV Fld(1,16,AC_MSKB2)//[16:16]
    #define SCSYS_1E_SC_P_AFCTI_YUV2YC_OPTION Fld(1,15,AC_MSKB1)//[15:15]
    #define SCSYS_1E_SC_M_AFCTI_SEL444 Fld(1,14,AC_MSKB1)//[14:14]
    #define SCSYS_1E_SC_M_AFCTI_UVINV Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_1E_SC_M_AFCTI_YUV2YC_OPTION Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_1E_SC_GFX_COEFF_22P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1F (IO_SCPIP_BASE + 0x37C)
    #define SCSYS_1F_SC_OUT_MEASURE_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_1F_SC_OUT_MEASURE_VTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_20 (IO_SCPIP_BASE + 0x380)
    #define SCSYS_20_SC_STA_PD_INFO_OUT Fld(3,29,AC_MSKB3)//[31:29]
    #define SCSYS_20_SC_FORCE_PIN_FIELD_VAL Fld(1,28,AC_MSKB3)//[28:28]
    #define SCSYS_20_SC_FORCE_PIN_FIELD Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_20_SC_FORCE_MIN_FIELD_VAL Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_20_SC_FORCE_MIN_FIELD Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_20_SC_FORCE_MIN_DISP_R_VAL Fld(1,24,AC_MSKB3)//[24:24]
    #define SCSYS_20_SC_FORCE_MIN_DISP_R Fld(1,23,AC_MSKB2)//[23:23]
    #define SCSYS_20_SC_OSD_TVE_HLEN Fld(10,13,AC_MSKW21)//[22:13]
    #define SCSYS_20_SC_OSD_TVE_VLEN Fld(10,3,AC_MSKW10)//[12:3]
#define SCPIP_SCSYS_21 (IO_SCPIP_BASE + 0x384)
    #define SCSYS_21_SC_INPTG_YPAT1_1 Fld(4,27,AC_MSKB3)//[30:27]
    #define SCSYS_21_SC_INPTG_LEFT_SIDE_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_21_SC_INPTG_PAT_DUTY_CYCLE_1 Fld(5,21,AC_MSKW32)//[25:21]
    #define SCSYS_21_SC_INPTG_ANGLE_SEL_1 Fld(3,18,AC_MSKB2)//[20:18]
    #define SCSYS_21_SC_INPTG_FREQ_SEL_1 Fld(3,15,AC_MSKW21)//[17:15]
    #define SCSYS_21_SC_INPTG_HOR_LINE_EN_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_21_SC_INPTG_COLOR_MODE_1 Fld(2,11,AC_MSKB1)//[12:11]
    #define SCSYS_21_SC_INPTG_PAT_MODE_1 Fld(2,9,AC_MSKB1)//[10:9]
    #define SCSYS_21_SC_INPTG_FREERUN_SPEED_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_21_SC_INPTG_FREERUN_EN_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_21_SC_INPTG_BND_EN_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_21_SC_INPTG_3D_MODE_1 Fld(2,1,AC_MSKB0)//[2:1]
    #define SCSYS_21_SC_INPTG_3D_INV_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_22 (IO_SCPIP_BASE + 0x388)
    #define SCSYS_22_SC_INPTG_BND_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_22_SC_INPTG_BND_W_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_23 (IO_SCPIP_BASE + 0x38C)
    #define SCSYS_23_SC_P_VMSK_DLY Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_23_SC_OUTPTG_ENABLE_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_23_SC_OUTPTG_YPAT2_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_23_SC_INPTG_ENABLE_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_23_SC_INPTG_YPAT2_1 Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_SCSYS_24 (IO_SCPIP_BASE + 0x390)
    #define SCSYS_24_SC_OUTPTG_YPAT1_1 Fld(4,27,AC_MSKB3)//[30:27]
    #define SCSYS_24_SC_OUTPTG_LEFT_SIDE_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_24_SC_OUTPTG_PAT_DUTY_CYCLE_1 Fld(5,21,AC_MSKW32)//[25:21]
    #define SCSYS_24_SC_OUTPTG_ANGLE_SEL_1 Fld(3,18,AC_MSKB2)//[20:18]
    #define SCSYS_24_SC_OUTPTG_FREQ_SEL_1 Fld(3,15,AC_MSKW21)//[17:15]
    #define SCSYS_24_SC_OUTPTG_HOR_LINE_EN_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_24_SC_OUTPTG_COLOR_MODE_1 Fld(2,11,AC_MSKB1)//[12:11]
    #define SCSYS_24_SC_OUTPTG_PAT_MODE_1 Fld(2,9,AC_MSKB1)//[10:9]
    #define SCSYS_24_SC_OUTPTG_FREERUN_SPEED_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_24_SC_OUTPTG_FREERUN_EN_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_24_SC_OUTPTG_BND_EN_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_24_SC_OUTPTG_3D_MODE_1 Fld(2,1,AC_MSKB0)//[2:1]
    #define SCSYS_24_SC_OUTPTG_3D_INV_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_25 (IO_SCPIP_BASE + 0x394)
    #define SCSYS_25_SC_OUTPTG_BND_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_25_SC_OUTPTG_BND_W_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_26 (IO_SCPIP_BASE + 0x398)
    #define SCSYS_26_SC_INPTG_BND_H_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_26_SC_INPTG_BND_W_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_27 (IO_SCPIP_BASE + 0x39C)
    #define SCSYS_27_SC_OUTPTG_ENABLE_2 Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_27_SC_OUTPTG_YPAT2_2 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_27_SC_INPTG_ENABLE_2 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_27_SC_INPTG_YPAT2_2 Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_SCSYS_28 (IO_SCPIP_BASE + 0x3A0)
    #define SCSYS_28_SC_OUTPTG_YPAT1_2 Fld(4,27,AC_MSKB3)//[30:27]
    #define SCSYS_28_SC_OUTPTG_LEFT_SIDE_2 Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_28_SC_OUTPTG_PAT_DUTY_CYCLE_2 Fld(5,21,AC_MSKW32)//[25:21]
    #define SCSYS_28_SC_OUTPTG_ANGLE_SEL_2 Fld(3,18,AC_MSKB2)//[20:18]
    #define SCSYS_28_SC_OUTPTG_FREQ_SEL_2 Fld(3,15,AC_MSKW21)//[17:15]
    #define SCSYS_28_SC_OUTPTG_HOR_LINE_EN_2 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_28_SC_OUTPTG_COLOR_MODE_2 Fld(2,11,AC_MSKB1)//[12:11]
    #define SCSYS_28_SC_OUTPTG_PAT_MODE_2 Fld(2,9,AC_MSKB1)//[10:9]
    #define SCSYS_28_SC_OUTPTG_FREERUN_SPEED_2 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_28_SC_OUTPTG_FREERUN_EN_2 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_28_SC_OUTPTG_BND_EN_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_28_SC_OUTPTG_3D_MODE_2 Fld(2,1,AC_MSKB0)//[2:1]
    #define SCSYS_28_SC_OUTPTG_3D_INV_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_29 (IO_SCPIP_BASE + 0x3A4)
    #define SCSYS_29_OUTPTG_BND_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_29_OUTPTG_BND_W_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_2A (IO_SCPIP_BASE + 0x3A8)
    #define SCSYS_2A_SC_INPTG_YPAT1_2 Fld(4,27,AC_MSKB3)//[30:27]
    #define SCSYS_2A_SC_INPTG_LEFT_SIDE_2 Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_2A_SC_INPTG_PAT_DUTY_CYCLE_2 Fld(5,21,AC_MSKW32)//[25:21]
    #define SCSYS_2A_SC_INPTG_ANGLE_SEL_2 Fld(3,18,AC_MSKB2)//[20:18]
    #define SCSYS_2A_SC_INPTG_FREQ_SEL_2 Fld(3,15,AC_MSKW21)//[17:15]
    #define SCSYS_2A_SC_INPTG_HOR_LINE_EN_2 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_2A_SC_INPTG_COLOR_MODE_2 Fld(2,11,AC_MSKB1)//[12:11]
    #define SCSYS_2A_SC_INPTG_PAT_MODE_2 Fld(2,9,AC_MSKB1)//[10:9]
    #define SCSYS_2A_SC_INPTG_FREERUN_SPEED_2 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_2A_SC_INPTG_FREERUN_EN_2 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_2A_SC_INPTG_BND_EN_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_2A_SC_INPTG_3D_MODE_2 Fld(2,1,AC_MSKB0)//[2:1]
    #define SCSYS_2A_SC_INPTG_3D_INV_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_2B (IO_SCPIP_BASE + 0x3AC)
    #define SCSYS_2B_STA_FRM_TRK_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_2B_STA_FRM_TRK_ABS_DIS Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_2C (IO_SCPIP_BASE + 0x3B0)
    #define SCSYS_2C_GFX_TRANS_CTRL Fld(30,0,AC_MSKDW)//[29:0]
#define SCPIP_SCSYS_2D (IO_SCPIP_BASE + 0x3B4)
    #define SCSYS_2D_SC_STA_HACTIVE_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_2D_SC_STA_HTOTAL_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_2E (IO_SCPIP_BASE + 0x3B8)
    #define SCSYS_2E_SC_STA_VACTIVE_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_2E_SC_STA_VTOTAL_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_2F (IO_SCPIP_BASE + 0x3BC)
    #define SCSYS_2F_SC_STA_HACTIVE_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_2F_SC_STA_HTOTAL_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_30 (IO_SCPIP_BASE + 0x3C0)
    #define SCSYS_30_SC_STA_VACTIVE_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_30_SC_STA_VTOTAL_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_31 (IO_SCPIP_BASE + 0x3C4)
    #define SCSYS_31_SC_MIN_CRC_CLIP_V_START Fld(12,18,AC_MSKW32)//[29:18]
    #define SCSYS_31_SC_MIN_CRC_CLIP_H_START Fld(13,5,AC_MSKDW)//[17:5]
    #define SCSYS_31_SC_MIN_CRC_CHECK_SUM_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_31_SC_MIN_CRC_BOT_R_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_31_SC_MIN_CRC_TOP_R_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_31_SC_MIN_CRC_BOT_L_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_31_SC_MIN_CRC_TOP_L_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_32 (IO_SCPIP_BASE + 0x3C8)
    #define SCSYS_32_SC_MIN_CRC_CLIP_V_END Fld(12,13,AC_MSKDW)//[24:13]
    #define SCSYS_32_SC_MIN_CRC_CLIP_H_END Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_SCSYS_33 (IO_SCPIP_BASE + 0x3CC)
    #define SCSYS_33_SC_MIN_CRC_V_IN_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define SCSYS_33_SC_MIN_CRC_C_IN_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define SCSYS_33_SC_MIN_CRC_Y_IN_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_SCSYS_34 (IO_SCPIP_BASE + 0x3D0)
    #define SCSYS_34_SC_MIN_STA_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_35 (IO_SCPIP_BASE + 0x3D4)
    #define SCSYS_35_SC_MIN_STA_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_36 (IO_SCPIP_BASE + 0x3D8)
    #define SCSYS_36_SC_PIN_CRC_CLIP_V_START Fld(12,18,AC_MSKW32)//[29:18]
    #define SCSYS_36_SC_PIN_CRC_CLIP_H_START Fld(13,5,AC_MSKDW)//[17:5]
    #define SCSYS_36_SC_PIN_CRC_CHECK_SUM_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_36_SC_PIN_CRC_BOT_R_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_36_SC_PIN_CRC_TOP_R_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_36_SC_PIN_CRC_BOT_L_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_36_SC_PIN_CRC_TOP_L_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_37 (IO_SCPIP_BASE + 0x3DC)
    #define SCSYS_37_SC_PIN_CRC_CLIP_V_END Fld(12,13,AC_MSKDW)//[24:13]
    #define SCSYS_37_SC_PIN_CRC_CLIP_H_END Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_SCSYS_38 (IO_SCPIP_BASE + 0x3E0)
    #define SCSYS_38_SC_PIN_CRC_V_IN_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define SCSYS_38_SC_PIN_CRC_C_IN_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define SCSYS_38_SC_PIN_CRC_Y_IN_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_SCSYS_39 (IO_SCPIP_BASE + 0x3E4)
    #define SCSYS_39_SC_PIN_STA_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_3A (IO_SCPIP_BASE + 0x3E8)
    #define SCSYS_3A_SC_PIN_STA_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_3B (IO_SCPIP_BASE + 0x3EC)
    #define SCSYS_3B_SC_MOUT_CRC_VSYNC_INV Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_3B_SC_MOUT_CRC_CLIP_V_START Fld(12,18,AC_MSKW32)//[29:18]
    #define SCSYS_3B_SC_MOUT_CRC_CLIP_H_START Fld(13,5,AC_MSKDW)//[17:5]
    #define SCSYS_3B_SC_MOUT_CRC_CHECK_SUM_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_3B_SC_MOUT_CRC_BOT_R_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_3B_SC_MOUT_CRC_TOP_R_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_3B_SC_MOUT_CRC_BOT_L_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_3B_SC_MOUT_CRC_TOP_L_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_3C (IO_SCPIP_BASE + 0x3F0)
    #define SCSYS_3C_SC_MOUT_CRC_CLIP_V_END Fld(12,13,AC_MSKDW)//[24:13]
    #define SCSYS_3C_SC_MOUT_CRC_CLIP_H_END Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_SCSYS_3D (IO_SCPIP_BASE + 0x3F4)
    #define SCSYS_3D_SC_MOUT_CRC_V_IN_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define SCSYS_3D_SC_MOUT_CRC_C_IN_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define SCSYS_3D_SC_MOUT_CRC_Y_IN_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_SCSYS_3E (IO_SCPIP_BASE + 0x3F8)
    #define SCSYS_3E_SC_MOUT_STA_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_3F (IO_SCPIP_BASE + 0x3FC)
    #define SCSYS_3F_SC_MOUT_STA_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
    

//Page DRAM1
#define SCPIP_DRAM_M_00 (IO_SCPIP_BASE + 0xB00)
    #define DRAM_M_00_DA_BANK_SWAP_PAGE_SIZE Fld(1,21,AC_MSKB2)//[21:21]
    #define DRAM_M_00_DA_BANK_SWAP Fld(1,20,AC_MSKB2)//[20:20]
    #define DRAM_M_00_DA_DRAM_LINE_PITCH Fld(4,16,AC_MSKB2)//[19:16]
    #define DRAM_M_00_DA_WR_SHORT_BURST_MODE Fld(1,14,AC_MSKB1)//[14:14]
    #define DRAM_M_00_DA_OVERFLOW_CLR Fld(1,13,AC_MSKB1)//[13:13]
    #define DRAM_M_00_DA_STATUS_CLR Fld(1,12,AC_MSKB1)//[12:12]
    #define DRAM_M_00_DA_FRAME_NUM Fld(4,8,AC_MSKB1)//[11:8]
    #define DRAM_M_00_DA_USE_GFX_FIFO Fld(1,7,AC_MSKB0)//[7:7]
    #define DRAM_M_00_DA_WRITE_EN_RELATCH Fld(1,6,AC_MSKB0)//[6:6]
    #define DRAM_M_00_DA_MIRROR_CHROMA_INV Fld(1,5,AC_MSKB0)//[5:5]
    #define DRAM_M_00_DA_SNAKE_MAPPING Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_00_DA_DISPLAY_MODE Fld(2,2,AC_MSKB0)//[3:2]
    #define DRAM_M_00_DA_READ_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_00_DA_WRITE_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_01 (IO_SCPIP_BASE + 0xB04)
    #define DRAM_M_01_DA_READ_REQ_SPACING Fld(8,24,AC_FULLB3)//[31:24]
    #define DRAM_M_01_DA_READ_LINE_START Fld(12,8,AC_MSKW21)//[19:8]
    #define DRAM_M_01_DA_READ_LINE_OFFSET Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_DRAM_M_02 (IO_SCPIP_BASE + 0xB08)
    #define DRAM_M_02_STA_RD_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
    #define DRAM_M_02_STA_GFX_WR_MAX_RESPONSE Fld(8,16,AC_FULLB2)//[23:16]
    #define DRAM_M_02_STA_WR_MAX_RESPONSE Fld(8,8,AC_FULLB1)//[15:8]
    #define DRAM_M_02_STA_GFX_WR_FRAME_CNT Fld(2,6,AC_MSKB0)//[7:6]
    #define DRAM_M_02_STA_GFX_WRITE_OOB Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_02_STA_WRITE_OOB Fld(1,3,AC_MSKB0)//[3:3]
    #define DRAM_M_02_STA_GFX_OVERFLOW Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_02_STA_OVERFLOW Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_03 (IO_SCPIP_BASE + 0xB0C)
    #define DRAM_M_03_DA_FRAME_DROP_N Fld(8,16,AC_FULLB2)//[23:16]
    #define DRAM_M_03_DA_FRAME_DROP_M Fld(8,8,AC_FULLB1)//[15:8]
    #define DRAM_M_03_DA_GFX_FRAME_DROP_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_03_DA_WRITE_FRAME_DROP_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_04 (IO_SCPIP_BASE + 0xB10)
    #define DRAM_M_04_DA_ADDR_BASE_MSB_Y Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_M_05 (IO_SCPIP_BASE + 0xB14)
    #define DRAM_M_05_DA_ADDR_BASE_LSB_Y Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_M_06 (IO_SCPIP_BASE + 0xB18)
    #define DRAM_M_06_DA_ADDR_BASE_MSB_U Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_M_07 (IO_SCPIP_BASE + 0xB1C)
    #define DRAM_M_07_DA_ADDR_BASE_LSB_U Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_M_08 (IO_SCPIP_BASE + 0xB20)
    #define DRAM_M_08_DA_ADDR_BASE_MSB_V Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_M_09 (IO_SCPIP_BASE + 0xB24)
    #define DRAM_M_09_DA_ADDR_BASE_LSB_V Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_M_0A (IO_SCPIP_BASE + 0xB28)
    #define DRAM_M_0A_DA_WADDR_HI_LIMIT Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_M_0B (IO_SCPIP_BASE + 0xB2C)
    #define DRAM_M_0B_DA_WADDR_LO_LIMIT Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_M_0C (IO_SCPIP_BASE + 0xB30)
    #define DRAM_M_0C_DA_GFX_ADDR_BASE_0 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_0D (IO_SCPIP_BASE + 0xB34)
    #define DRAM_M_0D_DA_GFX_ADDR_BASE_1 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_0E (IO_SCPIP_BASE + 0xB38)
    #define DRAM_M_0E_DA_GFX_ADDR_BASE_2 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_0F (IO_SCPIP_BASE + 0xB3C)
    #define DRAM_M_0F_DA_GFX_ADDR_BASE_3 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_10 (IO_SCPIP_BASE + 0xB40)
    #define DRAM_M_10_DA_GFX_WADDR_HI_LIMIT Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_11 (IO_SCPIP_BASE + 0xB44)
    #define DRAM_M_11_DA_GFX_WADDR_LO_LIMIT Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_12 (IO_SCPIP_BASE + 0xB48)
	#if 0
	#define DRAM_M_12_STA_WR_FLD Fld(1,31,AC_MSKB3)//[31:31]
	#define DRAM_M_12_DA_FLD_INT_EN Fld(1,18,AC_MSKB2)//[18:18]
	#define DRAM_M_12_DA_ADDR_FLD_INV Fld(1,17,AC_MSKB2)//[17:17]
	#define DRAM_M_12_DA_CTRL_FLD_INV Fld(1,16,AC_MSKB2)//[16:16]
	#endif
    #define DRAM_M_12_DA_3D_FP_LR_SWAP Fld(1,15,AC_MSKB1)//[15:15]
    #define DRAM_M_12_DA_GFX_3D_TO_2D_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define DRAM_M_12_DA_WRITE_3D_TO_2D_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define DRAM_M_12_DA_3D_TO_2D_LR Fld(1,12,AC_MSKB1)//[12:12]
    #define DRAM_M_12_DA_3D_FPR_MODE Fld(1,11,AC_MSKB1)//[11:11]
    #define DRAM_M_12_DA_3D_FPR_SWAP Fld(1,10,AC_MSKB1)//[10:10]
    #define DRAM_M_12_DA_3D_OUTPUT_LR Fld(1,9,AC_MSKB1)//[9:9]
    #define DRAM_M_12_DA_3D_OUTPUT_FIX_LR Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_M_12_DA_3D_OUTPUT_MODE Fld(2,6,AC_MSKB0)//[7:6]
    #define DRAM_M_12_DA_3D_OUTPUT_LR_SWAP Fld(1,5,AC_MSKB0)//[5:5]
    #define DRAM_M_12_DA_3D_OUTPUT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_12_DA_3D_INPUT_MODE Fld(2,2,AC_MSKB0)//[3:2]
    #define DRAM_M_12_DA_3D_INPUT_LR_SWAP Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_12_DA_3D_INPUT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_13 (IO_SCPIP_BASE + 0xB4C)
    #define DRAM_M_13_STA_GFX_WRITE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DRAM_M_13_DA_GFX_P2I_TB Fld(1,25,AC_MSKB3)//[25:25]
    #define DRAM_M_13_DA_GFX_P2I_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define DRAM_M_13_DA_GFX_ALPHA Fld(8,16,AC_FULLB2)//[23:16]
    #define DRAM_M_13_DA_GFX_MUX_B Fld(2,14,AC_MSKB1)//[15:14]
    #define DRAM_M_13_DA_GFX_MUX_G Fld(2,12,AC_MSKB1)//[13:12]
    #define DRAM_M_13_DA_GFX_MUX_R Fld(2,10,AC_MSKB1)//[11:10]
    #define DRAM_M_13_DA_GFX_ALPHA_POS_SEL Fld(1,9,AC_MSKB1)//[9:9]
    #define DRAM_M_13_DA_GFX_OVERFLOW_CLR Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_M_13_DA_GFX_ROUND_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define DRAM_M_13_DA_GFX_FRAME_NUM Fld(2,5,AC_MSKB0)//[6:5]
    #define DRAM_M_13_DA_GFX_MODE Fld(2,3,AC_MSKB0)//[4:3]
    #define DRAM_M_13_DA_GFX_WRITE_TRIGGER Fld(1,2,AC_MSKB0)//[2:2]
    #define DRAM_M_13_DA_GFX_WRITE_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_13_DA_GFX_WRITE_EN_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_14 (IO_SCPIP_BASE + 0xB50)
    #define DRAM_M_14_DA_ARB_DMMONSEL Fld(4,20,AC_MSKB2)//[23:20]
    #define DRAM_M_14_DA_ARB_DMAGBALL Fld(1,18,AC_MSKB2)//[18:18]
    #define DRAM_M_14_DA_ARB_DMARBRST Fld(1,17,AC_MSKB2)//[17:17]
    #define DRAM_M_14_DA_ARB_DMTIMARBEN Fld(1,16,AC_MSKB2)//[16:16]
    #define DRAM_M_14_DA_ARB_DMAGBMEND Fld(1,15,AC_MSKB1)//[15:15]
    #define DRAM_M_14_DA_ARB_DMAGBMAGID Fld(3,12,AC_MSKB1)//[14:12]
    #define DRAM_M_14_DA_ARB_DMAGBMBEG Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_M_14_DA_ARB_MONR_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define DRAM_M_14_DA_ARB_MONW_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define DRAM_M_14_DA_ARB_PIPR_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_14_DA_ARB_PIPW_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define DRAM_M_14_DA_ARB_GFXW_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define DRAM_M_14_DA_ARB_MAINR_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_14_DA_ARB_MAINW_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_15 (IO_SCPIP_BASE + 0xB54)
    #define DRAM_M_15_DA_ARB_MONR_TIM Fld(4,24,AC_MSKB3)//[27:24]
    #define DRAM_M_15_DA_ARB_MONW_TIM Fld(4,20,AC_MSKB2)//[23:20]
    #define DRAM_M_15_DA_ARB_PIPR_TIM Fld(4,16,AC_MSKB2)//[19:16]
    #define DRAM_M_15_DA_ARB_PIPW_TIM Fld(4,12,AC_MSKB1)//[15:12]
    #define DRAM_M_15_DA_ARB_GFXW_TIM Fld(4,8,AC_MSKB1)//[11:8]
    #define DRAM_M_15_DA_ARB_MAINR_TIM Fld(4,4,AC_MSKB0)//[7:4]
    #define DRAM_M_15_DA_ARB_MAINW_TIM Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_DRAM_M_16 (IO_SCPIP_BASE + 0xB58)
    #define DRAM_M_16_BM_LENGTH_COUNTER Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_17 (IO_SCPIP_BASE + 0xB5C)
    #define DRAM_M_17_BM_CYCLE_COUNTER Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_18 (IO_SCPIP_BASE + 0xB60)
    #define DRAM_M_18_STA_RD_CRC_READY Fld(1,24,AC_MSKB3)//[24:24]
    #define DRAM_M_18_DA_RD_CRC_CLR Fld(1,20,AC_MSKB2)//[20:20]
    #define DRAM_M_18_DA_RD_CRC_MODE Fld(1,16,AC_MSKB2)//[16:16]
    #define DRAM_M_18_STA_WR_CRC_READY Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_M_18_DA_WR_CRC_CLR Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_18_DA_WR_CRC_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_19 (IO_SCPIP_BASE + 0xB64)
    #define DRAM_M_19_DA_WR_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define DRAM_M_19_DA_WR_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define DRAM_M_19_DA_WR_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_M_1A (IO_SCPIP_BASE + 0xB68)
    #define DRAM_M_1A_DA_RD_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define DRAM_M_1A_DA_RD_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define DRAM_M_1A_DA_RD_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_M_1B (IO_SCPIP_BASE + 0xB6C)
    #define DRAM_M_1B_STA_WR_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_1C (IO_SCPIP_BASE + 0xB70)
    #define DRAM_M_1C_STA_RD_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_1D (IO_SCPIP_BASE + 0xB74)
    #define DRAM_M_1D_DA_GFX_OVERSCAN_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DRAM_M_1D_DA_WRITE_OVERSCAN_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define DRAM_M_1D_DA_OVERSCAN_DN Fld(11,16,AC_MSKW32)//[26:16]
    #define DRAM_M_1D_DA_OVERSCAN_UP Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DRAM_M_1E (IO_SCPIP_BASE + 0xB78)
    #define DRAM_M_1E_DA_OVERSCAN_RT Fld(11,16,AC_MSKW32)//[26:16]
    #define DRAM_M_1E_DA_OVERSCAN_LF Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DRAM_M_1F (IO_SCPIP_BASE + 0xB7C)
    #define DRAM_M_1F_DRAM1_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DRAM_M_1F_DRAM1_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define DRAM_M_1F_DRAM1_ATPG_CT Fld(1,29,AC_MSKB3)//[29:29]
    #define DRAM_M_1F_DA_WR_OFFSET_Y Fld(12,16,AC_MSKW32)//[27:16]
    #define DRAM_M_1F_DA_WR_OFFSET_X Fld(8,4,AC_MSKW10)//[11:4]
#define SCPIP_DRAM_M_20 (IO_SCPIP_BASE + 0xB80)
    #define DRAM_M_20_STA_FRM_WPTR Fld(4,28,AC_MSKB3)//[31:28]
    #define DRAM_M_20_STA_FRM_READY2 Fld(1,26,AC_MSKB3)//[26:26]
    #define DRAM_M_20_STA_FRM_READY1 Fld(1,25,AC_MSKB3)//[25:25]
    #define DRAM_M_20_STA_FRM_READY0 Fld(1,24,AC_MSKB3)//[24:24]
    #define DRAM_M_20_DA_WR_END_LINE_CNT Fld(12,8,AC_MSKW21)//[19:8]
    #define DRAM_M_20_DA_WR_END_LINE_CNT_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define DRAM_M_20_DA_CLR_FRM_READY2 Fld(1,6,AC_MSKB0)//[6:6]
    #define DRAM_M_20_DA_CLR_FRM_READY1 Fld(1,5,AC_MSKB0)//[5:5]
    #define DRAM_M_20_DA_CLR_FRM_READY0 Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_20_DA_WR_READY_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define DRAM_M_20_DA_420_SHIFT Fld(1,2,AC_MSKB0)//[2:2]
    #define DRAM_M_20_DA_H264_FRAME_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_20_DA_WRITE_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_21 (IO_SCPIP_BASE + 0xB84)
    #define DRAM_M_21_DA_H264_ADDR_BASE_Y0 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_22 (IO_SCPIP_BASE + 0xB88)
    #define DRAM_M_22_DA_H264_ADDR_BASE_C0 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_23 (IO_SCPIP_BASE + 0xB8C)
    #define DRAM_M_23_DA_H264_ADDR_BASE_Y1 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_24 (IO_SCPIP_BASE + 0xB90)
    #define DRAM_M_24_DA_H264_ADDR_BASE_C1 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_25 (IO_SCPIP_BASE + 0xB94)
    #define DRAM_M_25_DA_H264_ADDR_BASE_Y2 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_26 (IO_SCPIP_BASE + 0xB98)
    #define DRAM_M_26_DA_H264_ADDR_BASE_C2 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_M_27 (IO_SCPIP_BASE + 0xB9C)
    #define DRAM_M_27_DA_ECO_RSV Fld(32,0,AC_FULLDW)//[31:0]    

//Page DRAM2
#define SCPIP_DRAM_P_00 (IO_SCPIP_BASE + 0xC00)
    #define DRAM_P_00_DA_BANK_SWAP_PAGE_SIZE Fld(1,21,AC_MSKB2)//[21:21]
    #define DRAM_P_00_DA_BANK_SWAP Fld(1,20,AC_MSKB2)//[20:20]
    #define DRAM_P_00_DA_DRAM_LINE_PITCH Fld(4,16,AC_MSKB2)//[19:16]
    #define DRAM_P_00_DA_WR_SHORT_BURST_MODE Fld(1,14,AC_MSKB1)//[14:14]
    #define DRAM_P_00_DA_OVERFLOW_CLR Fld(1,13,AC_MSKB1)//[13:13]
    #define DRAM_P_00_DA_STATUS_CLR Fld(1,12,AC_MSKB1)//[12:12]
    #define DRAM_P_00_DA_FRAME_NUM Fld(4,8,AC_MSKB1)//[11:8]
    #define DRAM_P_00_DA_WRITE_EN_RELATCH Fld(1,6,AC_MSKB0)//[6:6]
    #define DRAM_P_00_DA_MIRROR_CHROMA_INV Fld(1,5,AC_MSKB0)//[5:5]
    #define DRAM_P_00_DA_SNAKE_MAPPING Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_P_00_DA_DISPLAY_MODE Fld(2,2,AC_MSKB0)//[3:2]
    #define DRAM_P_00_DA_READ_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_P_00_DA_WRITE_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_01 (IO_SCPIP_BASE + 0xC04)
    #define DRAM_P_01_DA_READ_REQ_SPACING Fld(8,24,AC_FULLB3)//[31:24]
    #define DRAM_P_01_DA_READ_LINE_START Fld(12,8,AC_MSKW21)//[19:8]
    #define DRAM_P_01_DA_READ_LINE_OFFSET Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_DRAM_P_02 (IO_SCPIP_BASE + 0xC08)
    #define DRAM_P_02_STA_RD_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
    #define DRAM_P_02_STA_WR_MAX_RESPONSE Fld(8,8,AC_FULLB1)//[15:8]
    #define DRAM_P_02_STA_WRITE_OOB Fld(1,3,AC_MSKB0)//[3:3]
    #define DRAM_P_02_STA_OVERFLOW Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_03 (IO_SCPIP_BASE + 0xC0C)
    #define DRAM_P_03_DA_FRAME_DROP_N Fld(8,16,AC_FULLB2)//[23:16]
    #define DRAM_P_03_DA_FRAME_DROP_M Fld(8,8,AC_FULLB1)//[15:8]
    #define DRAM_P_03_DA_GFX_FRAME_DROP_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_P_03_DA_WRITE_FRAME_DROP_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_04 (IO_SCPIP_BASE + 0xC10)
    #define DRAM_P_04_DA_ADDR_BASE_MSB_Y Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_P_05 (IO_SCPIP_BASE + 0xC14)
    #define DRAM_P_05_DA_ADDR_BASE_LSB_Y Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_P_06 (IO_SCPIP_BASE + 0xC18)
    #define DRAM_P_06_DA_ADDR_BASE_MSB_U Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_P_07 (IO_SCPIP_BASE + 0xC1C)
    #define DRAM_P_07_DA_ADDR_BASE_LSB_U Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_P_08 (IO_SCPIP_BASE + 0xC20)
    #define DRAM_P_08_DA_ADDR_BASE_MSB_V Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_P_09 (IO_SCPIP_BASE + 0xC24)
    #define DRAM_P_09_DA_ADDR_BASE_LSB_V Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_P_0A (IO_SCPIP_BASE + 0xC28)
    #define DRAM_P_0A_DA_WADDR_HI_LIMIT Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_P_0B (IO_SCPIP_BASE + 0xC2C)
    #define DRAM_P_0B_DA_WADDR_LO_LIMIT Fld(21,11,AC_MSKDW)//[31:11]
#define SCPIP_DRAM_P_12 (IO_SCPIP_BASE + 0xC48)
    #define DRAM_P_12_DA_3D_FP_LR_SWAP Fld(1,15,AC_MSKB1)//[15:15]
    #define DRAM_P_12_DA_GFX_3D_TO_2D_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define DRAM_P_12_DA_WRITE_3D_TO_2D_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define DRAM_P_12_DA_3D_TO_2D_LR Fld(1,12,AC_MSKB1)//[12:12]
    #define DRAM_P_12_DA_3D_FPR_MODE Fld(1,11,AC_MSKB1)//[11:11]
    #define DRAM_P_12_DA_3D_FPR_SWAP Fld(1,10,AC_MSKB1)//[10:10]
    #define DRAM_P_12_DA_3D_OUTPUT_LR Fld(1,9,AC_MSKB1)//[9:9]
    #define DRAM_P_12_DA_3D_OUTPUT_FIX_LR Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_P_12_DA_3D_OUTPUT_MODE Fld(2,6,AC_MSKB0)//[7:6]
    #define DRAM_P_12_DA_3D_OUTPUT_LR_SWAP Fld(1,5,AC_MSKB0)//[5:5]
    #define DRAM_P_12_DA_3D_OUTPUT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_P_12_DA_3D_INPUT_MODE Fld(2,2,AC_MSKB0)//[3:2]
    #define DRAM_P_12_DA_3D_INPUT_LR_SWAP Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_P_12_DA_3D_INPUT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_18 (IO_SCPIP_BASE + 0xC60)
    #define DRAM_P_18_STA_RD_CRC_READY Fld(1,24,AC_MSKB3)//[24:24]
    #define DRAM_P_18_DA_RD_CRC_CLR Fld(1,20,AC_MSKB2)//[20:20]
    #define DRAM_P_18_DA_RD_CRC_MODE Fld(1,16,AC_MSKB2)//[16:16]
    #define DRAM_P_18_STA_WR_CRC_READY Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_P_18_DA_WR_CRC_CLR Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_P_18_DA_WR_CRC_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_19 (IO_SCPIP_BASE + 0xC64)
    #define DRAM_P_19_DA_WR_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define DRAM_P_19_DA_WR_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define DRAM_P_19_DA_WR_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_P_1A (IO_SCPIP_BASE + 0xC68)
    #define DRAM_P_1A_DA_RD_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define DRAM_P_1A_DA_RD_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define DRAM_P_1A_DA_RD_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_P_1B (IO_SCPIP_BASE + 0xC6C)
    #define DRAM_P_1B_STA_WR_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_P_1C (IO_SCPIP_BASE + 0xC70)
    #define DRAM_P_1C_STA_RD_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_P_1D (IO_SCPIP_BASE + 0xC74)
    #define DRAM_P_1D_DA_WRITE_OVERSCAN_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define DRAM_P_1D_DA_OVERSCAN_DN Fld(11,16,AC_MSKW32)//[26:16]
    #define DRAM_P_1D_DA_OVERSCAN_UP Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DRAM_P_1E (IO_SCPIP_BASE + 0xC78)
    #define DRAM_P_1E_DA_OVERSCAN_RT Fld(11,16,AC_MSKW32)//[26:16]
    #define DRAM_P_1E_DA_OVERSCAN_LF Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DRAM_P_1F (IO_SCPIP_BASE + 0xC7C)
    #define DRAM_P_1F_DRAM2_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DRAM_P_1F_DRAM2_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define DRAM_P_1F_DRAM2_ATPG_CT Fld(1,29,AC_MSKB3)//[29:29]
    #define DRAM_P_1F_DA_WR_OFFSET_Y Fld(12,16,AC_MSKW32)//[27:16]
    #define DRAM_P_1F_DA_WR_OFFSET_X Fld(8,4,AC_MSKW10)//[11:4]
#define SCPIP_DRAM_P_20 (IO_SCPIP_BASE + 0xC80)
    #define DRAM_P_20_STA_FRM_WPTR Fld(4,28,AC_MSKB3)//[31:28]
    #define DRAM_P_20_STA_FRM_READY2 Fld(1,26,AC_MSKB3)//[26:26]
    #define DRAM_P_20_STA_FRM_READY1 Fld(1,25,AC_MSKB3)//[25:25]
    #define DRAM_P_20_STA_FRM_READY0 Fld(1,24,AC_MSKB3)//[24:24]
    #define DRAM_P_20_DA_WR_END_LINE_CNT Fld(12,8,AC_MSKW21)//[19:8]
    #define DRAM_P_20_DA_WR_END_LINE_CNT_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define DRAM_P_20_DA_CLR_FRM_READY2 Fld(1,6,AC_MSKB0)//[6:6]
    #define DRAM_P_20_DA_CLR_FRM_READY1 Fld(1,5,AC_MSKB0)//[5:5]
    #define DRAM_P_20_DA_CLR_FRM_READY0 Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_P_20_DA_WR_READY_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define DRAM_P_20_DA_420_SHIFT Fld(1,2,AC_MSKB0)//[2:2]
    #define DRAM_P_20_DA_H264_FRAME_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_P_20_DA_WRITE_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_21 (IO_SCPIP_BASE + 0xC84)
    #define DRAM_P_21_DA_H264_ADDR_BASE_Y0 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_P_22 (IO_SCPIP_BASE + 0xC88)
    #define DRAM_P_22_DA_H264_ADDR_BASE_C0 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_P_23 (IO_SCPIP_BASE + 0xC8C)
    #define DRAM_P_23_DA_H264_ADDR_BASE_Y1 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_P_24 (IO_SCPIP_BASE + 0xC90)
    #define DRAM_P_24_DA_H264_ADDR_BASE_C1 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_P_25 (IO_SCPIP_BASE + 0xC94)
    #define DRAM_P_25_DA_H264_ADDR_BASE_Y2 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_P_26 (IO_SCPIP_BASE + 0xC98)
    #define DRAM_P_26_DA_H264_ADDR_BASE_C2 Fld(28,4,AC_MSKDW)//[31:4]
#define SCPIP_DRAM_P_27 (IO_SCPIP_BASE + 0xC9C)
    #define DRAM_P_27_DA_ECO_RSV Fld(32,0,AC_FULLDW)//[31:0]    

#endif
