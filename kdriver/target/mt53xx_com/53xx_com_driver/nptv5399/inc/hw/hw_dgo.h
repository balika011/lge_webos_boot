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
 *   $Workfile: hw_dgo.h $ 
 *
 * Project:
 * --------
 *   MT5399
 *
 * Description:
 * ------------
 *   DGO Module miscellaneous register definition
 *   
 * Author:
 * -------
 *   Jiewen Zhou
 *
 * Last changed:
 * ------------- 
 * $Author: 
 *
 * $Modtime:  
 *
 * $Revision: 
****************************************************************************/
#ifndef _HW_DGO_
#define _HW_DGO_
#define INPUT_CTRL (IO_VIRT + 0x5F00C)
    #define VSYNC_EDGE_SEL_3D Fld(1,31,AC_MSKB3)//[31:31]
#define DGO_CRC_CTRL (IO_VIRT + 0x5F014)
    #define CRC_CLR_OUT Fld(1,6,AC_MSKB0)//[6:6]
    #define CRC_CLR_IN Fld(1,5,AC_MSKB0)//[5:5]
    #define CRC_START_OUT Fld(1,3,AC_MSKB0)//[3:3]
    #define CRC_START_IN Fld(1,2,AC_MSKB0)//[2:2]
    #define CRC_OUT_CLOCK_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define CRC_IN_CLOCK_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define DGO_CRC_VAL (IO_VIRT + 0x5F018)
    #define DGO_CRC_OUT Fld(16,16,AC_FULLW32)//[31:16]
    #define DGO_CRC_IN Fld(16,0,AC_FULLW10)//[15:0]
#define MIX_CTRL4 (IO_VIRT + 0x5F024)
    #define MON_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define DEBUG1 (IO_VIRT + 0x5F028)
    #define HVSYN_IN_ Fld(2,14,AC_MSKB1)//[15:14]
    #define DISPLAY_R_IN Fld(1,13,AC_MSKB1)//[13:13]
    #define ODD_FIELD_DIGITAL Fld(1,12,AC_MSKB1)//[12:12]
    #define TV_LINE Fld(12,0,AC_MSKW10)//[11:0]
#define DEBUG3 (IO_VIRT + 0x5F03C)
    #define H_COUNTER Fld(13,16,AC_MSKW32)//[28:16]
    #define V_COUNTER Fld(12,0,AC_MSKW10)//[11:0]
#define DGO_ANAIF_CTRL0 (IO_VIRT + 0x5F04C)
    #define DGO_DELAY_CHAIN_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DGO_CLK_DELAY_SEL1 Fld(5,12,AC_MSKW21)//[16:12]
    #define DGO_CLK_DELAY_SEL0 Fld(5,4,AC_MSKW10)//[8:4]
    #define DGO_CLK_INV_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DGO_CLK_EN Fld(1,0,AC_MSKB0)//[0:0]
#define DGO_MODE (IO_VIRT + 0x5F050)
    #define DE_POL Fld(1,29,AC_MSKB3)//[29:29]
    #define VSYNC_POL Fld(1,28,AC_MSKB3)//[28:28]
    #define HSYNC_POL Fld(1,27,AC_MSKB3)//[27:27]
    #define C_DEL Fld(1,26,AC_MSKB3)//[26:26]
    #define SYN_DEL Fld(2,24,AC_MSKB3)//[25:24]
    #define C_ESAV_D3 Fld(1,23,AC_MSKB2)//[23:23]
    #define HW_OPT Fld(1,22,AC_MSKB2)//[22:22]
    #define EAAV0123 Fld(1,21,AC_MSKB2)//[21:21]
    #define ADJ_SYN_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define SYNC_LENGTH_SEL Fld(1,19,AC_MSKB2)//[19:19]
    #define ADJ_SYN_FWD Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_INVERSE Fld(1,17,AC_MSKB2)//[17:17]
    #define USE_FLD Fld(1,16,AC_MSKB2)//[16:16]
    #define DGO_REAPEAT_MODE Fld(1,12,AC_MSKB1)//[12:12]
    #define ESAVI Fld(1,11,AC_MSKB1)//[11:11]
    #define DGO_NO_MIX Fld(1,10,AC_MSKB1)//[10:10]
    #define CCIR_MODIFY Fld(1,9,AC_MSKB1)//[9:9]
    #define SMPTE_AUTOFLD Fld(1,8,AC_MSKB1)//[8:8]
    #define ODD_FLD_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define P2I_FLD Fld(1,3,AC_MSKB0)//[3:3]
    #define PRGS_INVFLD Fld(1,2,AC_MSKB0)//[2:2]
    #define PRGS_AUTOFLD Fld(1,1,AC_MSKB0)//[1:1]
    #define DGO_656_EN Fld(1,0,AC_MSKB0)//[0:0]
#define DGO_CTRL (IO_VIRT + 0x5F054)
    #define FRAME_LO_USE_DE Fld(1,31,AC_MSKB3)//[31:31]
    #define FRAME_LO_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define RESET_V_SYNC_CNT Fld(1,29,AC_MSKB3)//[29:29]
    #define DGO_444 Fld(1,28,AC_MSKB3)//[28:28]
    #define SMPTE_C_YUV_SEL Fld(2,26,AC_MSKB3)//[27:26]
    #define SD_YC_16BIT Fld(1,25,AC_MSKB3)//[25:25]
    #define DIS_ESAV_CODE Fld(1,24,AC_MSKB3)//[24:24]
    #define DGO_CR_DEL Fld(2,22,AC_MSKB2)//[23:22]
    #define DGO_CB_DEL Fld(2,20,AC_MSKB2)//[21:20]
    #define DGO_Y_DEL Fld(2,18,AC_MSKB2)//[19:18]
    #define C_DGO_LMT_FF00 Fld(1,17,AC_MSKB2)//[17:17]
    #define C_ITLC_TOPLONG Fld(1,16,AC_MSKB2)//[16:16]
    #define C_INV_TVEFLD Fld(1,15,AC_MSKB1)//[15:15]
    #define C_UV_SYNC_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define DGO_YUV_SEL_ADJ Fld(2,12,AC_MSKB1)//[13:12]
    #define C_DGO_C_INV Fld(1,11,AC_MSKB1)//[11:11]
    #define C_DGO_Y_INV Fld(1,10,AC_MSKB1)//[10:10]
    #define C_8_10_RND Fld(1,9,AC_MSKB1)//[9:9]
    #define C_10B_DGO Fld(1,8,AC_MSKB1)//[8:8]
    #define C_DGO_LMT_CBCR Fld(1,7,AC_MSKB0)//[7:7]
    #define C_DGO_LMT_656C Fld(1,6,AC_MSKB0)//[6:6]
    #define C_DGO_LMT_BOT Fld(1,5,AC_MSKB0)//[5:5]
    #define C_DGO_LMT_TOP Fld(1,4,AC_MSKB0)//[4:4]
    #define DGO_LPF_11_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define DGO_LPF_TYPE Fld(1,2,AC_MSKB0)//[2:2]
    #define LPF_ROUND Fld(1,1,AC_MSKB0)//[1:1]
    #define DGO_LPF_EN Fld(1,0,AC_MSKB0)//[0:0]
#define DGO_TST_PAT_CTRL (IO_VIRT + 0x5F058)
    #define RG_PTGEN_COLOR_BAR_TH Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_PTGEN_TYPE Fld(8,4,AC_MSKW10)//[11:4]
    #define FRAME_RST Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_TST_PAT_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_TST_PAT_CLK_EN Fld(1,0,AC_MSKB0)//[0:0]
#define RG_PTGEN (IO_VIRT + 0x5F05C)
    #define RG_PTGEN_BD_CR Fld(8,16,AC_FULLB2)//[23:16]
    #define RG_PTGEN_BD_CB Fld(8,8,AC_FULLB1)//[15:8]
    #define RG_PTGEN_BD_Y Fld(8,0,AC_FULLB0)//[7:0]
#define SMPTE (IO_VIRT + 0x5F080)
    #define SMPTE_SHIFT_12_11_ Fld(2,30,AC_MSKB3)//[31:30]
    #define SMPTE_ON Fld(1,29,AC_MSKB3)//[29:29]
    #define SMPTE_V_POL Fld(1,28,AC_MSKB3)//[28:28]
    #define SMPTE_F_POL Fld(1,27,AC_MSKB3)//[27:27]
    #define SMPTE_SHIFT_10_0_ Fld(11,16,AC_MSKW32)//[26:16]
    #define SMPTE_TOTAL_USE_REG Fld(1,0,AC_MSKB0)//[0:0]
#define MAST_CTRL (IO_VIRT + 0x5F084)
    #define MST_PRGS Fld(1,3,AC_MSKB0)//[3:3]
#define DGO_SYNC1 (IO_VIRT + 0x5F090)
    #define ADJ_VSYNC_WIDTH Fld(10,16,AC_MSKW32)//[25:16]
    #define ADJ_HSYNC_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define FMT_MODE (IO_VIRT + 0x5F094)
    #define TV_TYPE Fld(2,30,AC_MSKB3)//[31:30]
    #define PRGS Fld(1,15,AC_MSKB1)//[15:15]
    #define HD_ON Fld(1,14,AC_MSKB1)//[14:14]
    #define HD_TP Fld(1,13,AC_MSKB1)//[13:13]
    #define VSYNWIDTH Fld(5,8,AC_MSKB1)//[12:8]
    #define HSYNWIDTH Fld(8,0,AC_FULLB0)//[7:0]
#define VSYN_OFFSET (IO_VIRT + 0x5F098)
    #define SMPTE_ACT_BGN_LINE Fld(7,24,AC_MSKB3)//[30:24]
    #define REG_1098 Fld(12,0,AC_MSKW10)//[11:0]
#define FMT_CTRL (IO_VIRT + 0x5F0AC)
    #define HW_OPTION Fld(4,28,AC_MSKB3)//[31:28]
    #define SYNC_DEL Fld(2,14,AC_MSKB1)//[15:14]
    #define FTRST Fld(1,10,AC_MSKB1)//[10:10]
    #define PXLSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define C_RST_SEL Fld(1,7,AC_MSKB0)//[7:7]
    #define VPOR Fld(1,4,AC_MSKB0)//[4:4]
    #define HPOR Fld(1,3,AC_MSKB0)//[3:3]
    #define FMTM Fld(1,1,AC_MSKB0)//[1:1]
    #define FMT_ON Fld(1,0,AC_MSKB0)//[0:0]
#define BG_COLOR (IO_VIRT + 0x5F0B8)
    #define BGCR Fld(8,16,AC_FULLB2)//[23:16]
    #define BGCB Fld(8,8,AC_FULLB1)//[15:8]
    #define BGY Fld(8,0,AC_FULLB0)//[7:0]
#define DGO_CTRL2 (IO_VIRT + 0x5F0BC)
    #define DGO_3D_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FRAME_CNT_INV Fld(1,29,AC_MSKB3)//[29:29]
    #define DGO_SELF_PRGS_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define DGO_SELF_PRGS Fld(1,27,AC_MSKB3)//[27:27]
    #define DGO_C2_CHANNEL_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define DGO_C_CHANNEL_SEL Fld(2,18,AC_MSKB2)//[19:18]
    #define DGO_Y_CHANNEL_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define DGO_VSYNC_CNT Fld(2,14,AC_MSKB1)//[15:14]
#define Y_LMT (IO_VIRT + 0x5F0C4)
    #define Y_LMT_BOT Fld(8,8,AC_FULLB1)//[15:8]
    #define Y_LMT_TOP Fld(8,0,AC_FULLB0)//[7:0]
#define DGO_DE_H (IO_VIRT + 0x5F0D0)
    #define CCIR Fld(1,31,AC_MSKB3)//[31:31]
    #define CCIR_HBGN Fld(13,16,AC_MSKW32)//[28:16]
    #define CCIR_HEND Fld(13,0,AC_MSKW10)//[12:0]
#define FMT_HV_TOTAL (IO_VIRT + 0x5F0D4)
    #define ADJ_T Fld(1,31,AC_MSKB3)//[31:31]
    #define H_TOTAL Fld(13,16,AC_MSKW32)//[28:16]
    #define V_TOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define DGO_DE_VO (IO_VIRT + 0x5F0E0)
    #define PFLD Fld(1,31,AC_MSKB3)//[31:31]
    #define CCIR_VOBGN Fld(12,16,AC_MSKW32)//[27:16]
    #define CCIR_VOEND Fld(12,0,AC_MSKW10)//[11:0]
#define MULTI_CTRL (IO_VIRT + 0x5F0E4)
    #define DGO_CLK_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define SET_DATA_POLARITY Fld(1,30,AC_MSKB3)//[30:30]
    #define SET_DATA_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define SMPTE_625L Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_TST_DATA_PAT_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define RST_PHASE Fld(1,14,AC_MSKB1)//[14:14]
#define DGO_SYNC_DELAY (IO_VIRT + 0x5F0E8)
    #define VSYN_DELAY Fld(13,16,AC_MSKW32)//[28:16]
    #define HSYN_DELAY Fld(13,0,AC_MSKW10)//[12:0]
#define DGO_DE_VE (IO_VIRT + 0x5F0F0)
    #define CCIR_VEBGN Fld(12,16,AC_MSKW32)//[27:16]
    #define CCIR_VEEND Fld(12,0,AC_MSKW10)//[11:0]
#define MIX_CTRL3 (IO_VIRT + 0x5F0F8)
    #define VSYNC_120HZ Fld(1,31,AC_MSKB3)//[31:31]
    #define H_MAX_2FS Fld(1,29,AC_MSKB3)//[29:29]
    #define DGO_120HZ_FLD_INV Fld(1,16,AC_MSKB2)//[16:16]
    #define RST_VALUE_120HZ Fld(2,14,AC_MSKB1)//[15:14]
    #define ORIG_VDO_ACTIVE Fld(1,12,AC_MSKB1)//[12:12]

#endif
