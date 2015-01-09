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
 * $RCSfile: hw_tdc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _HW_TDC_H_
#define _HW_TDC_H_

#include "hw_vdoin.h"


//#define IO_COMB_BASE (VDOIN_BASE+0x2000)
#define IO_COMB_BASE (0xF0022000)

	
//Page COMB_1
#define STA_COMB_00 (IO_COMB_BASE + 0x0C0)
    #define BIN12PIXCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_01 (IO_COMB_BASE + 0x0C4)
    #define BIN34PIXCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_02 (IO_COMB_BASE + 0x0C8)
    #define CZPCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_03 (IO_COMB_BASE + 0x0CC)
    #define NOISTIPISUM Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_04 (IO_COMB_BASE + 0x0D0)
	#define CHROMAFLICKERFIELDCNT Fld(4,28,AC_MSKB3)//[31:28]
    #define PERFIELDCHROMAFLICKERCNT Fld(8,20,AC_MSKW32)//[27:20]
    #define INPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_05 (IO_COMB_BASE + 0x0D4)
    #define PERMOTIONPIXCOUNT_FIELD Fld(1,31,AC_MSKB3)//[31:31]
    #define PERMOTIONPIXCOUNT Fld(8,20,AC_MSKW32)//[27:20]
    #define MOPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_06 (IO_COMB_BASE + 0x0D8)
    #define PERFIELDCHROMAVARSUM Fld(8,20,AC_MSKW32)//[27:20]
    #define MBPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_07 (IO_COMB_BASE + 0x0DC)
    #define LUMASUMSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_08 (IO_COMB_BASE + 0x0E0)
    #define COLORSUMSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_09 (IO_COMB_BASE + 0x0E4)
    #define LUMAEDGESTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0A (IO_COMB_BASE + 0x0E8)
    #define LUMAEDPISTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0B (IO_COMB_BASE + 0x0EC)
    #define COLOREDGESTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0C (IO_COMB_BASE + 0x0F0)
    #define FIFO_EVERFULL Fld(1,22,AC_MSKB2)//[22:22]
    #define FIFOEMPTY Fld(1,21,AC_MSKB2)//[21:21]
    #define FIFOEMPTY_A Fld(1,20,AC_MSKB2)//[20:20]
    #define FIFOEMPTY_AA Fld(1,19,AC_MSKB2)//[19:19]
    #define BIG_MOTION Fld(1,18,AC_MSKB2)//[18:18]
    #define NONVCR3D Fld(1,17,AC_MSKB2)//[17:17]
    #define NONVCRTG Fld(1,16,AC_MSKB2)//[16:16]
    #define SMALL_MOTION Fld(1,15,AC_MSKB1)//[15:15]
    #define STILL_FRAME Fld(1,14,AC_MSKB1)//[14:14]
    #define WHOLE_FRAME_C3D_SMALL Fld(1,13,AC_MSKB1)//[13:13]
    #define NOISEFG Fld(1,4,AC_MSKB0)//[4:4]
    #define CZP_DETECT Fld(1,3,AC_MSKB0)//[3:3]
    #define SWEEP_DETECTION Fld(1,2,AC_MSKB0)//[2:2]
    #define WHOLE_FRAME_VERTICAL_SMOOTH Fld(1,1,AC_MSKB0)//[1:1]
    #define NOISY_SMALL Fld(1,0,AC_MSKB0)//[0:0]
#define COMB_CTRL_00 (IO_COMB_BASE + 0x640)
    #define CLRFULL Fld(1,28,AC_MSKB3)//[28:28]
    #define CLREMPTY Fld(1,27,AC_MSKB3)//[27:27]
    #define FULLSTA Fld(1,26,AC_MSKB3)//[26:26]
    #define EMPTYSTA Fld(1,25,AC_MSKB3)//[25:25]
    #define EN_BLOCK_BY_SEED Fld(1,24,AC_MSKB3)//[24:24]
    #define ADYSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define SETZERO Fld(1,22,AC_MSKB2)//[22:22]
    #define FIFORY Fld(6,16,AC_MSKB2)//[21:16]
    #define DML_METHOD Fld(1,15,AC_MSKB1)//[15:15]
    #define DML_FSEL Fld(1,14,AC_MSKB1)//[14:14]
    #define FIFORX Fld(6,8,AC_MSKB1)//[13:8]
    #define DMEGSEL Fld(1,7,AC_MSKB0)//[7:7]
    #define DMTPSEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FIFORM1 Fld(6,0,AC_MSKB0)//[5:0]
#define COMB_CTRL_01 (IO_COMB_BASE + 0x644)
    #define WVSYNCTH Fld(7,25,AC_MSKB3)//[31:25]
    #define DRAMBASEADR Fld(25,0,AC_MSKDW)//[24:0]
#define COMB_CTRL_02 (IO_COMB_BASE + 0x648)
    #define EN3D Fld(1,31,AC_MSKB3)//[31:31]
    #define ENFORCE3D Fld(1,30,AC_MSKB3)//[30:30]
    #define COMB_OPTION Fld(2,28,AC_MSKB3)//[29:28]
    #define B10MODE Fld(1,27,AC_MSKB3)//[27:27]
    #define FORCEDRAM Fld(1,26,AC_MSKB3)//[26:26]
    #define GAMEMODE Fld(1,25,AC_MSKB3)//[25:25]
    #define ENPGMODE Fld(1,24,AC_MSKB3)//[24:24]
    #define WITHCLRTH_0 Fld(1,23,AC_MSKB2)//[23:23]
    #define ENVCRFSIG2 Fld(1,22,AC_MSKB2)//[22:22]
    #define ENVCRFSIG1 Fld(1,21,AC_MSKB2)//[21:21]
    #define HCNT3D Fld(10,11,AC_MSKW21)//[20:11]
    #define HLEN3D Fld(11,0,AC_MSKW10)//[10:0]
#define COMB_CTRL_03 (IO_COMB_BASE + 0x64C)
    #define VLINEST Fld(7,25,AC_MSKB3)//[31:25]
    #define VLINECNT Fld(9,16,AC_MSKW32)//[24:16]
    #define DUMPSTAL Fld(1,15,AC_MSKB1)//[15:15]
    #define ENCKILLWFIFO Fld(1,14,AC_MSKB1)//[14:14]
    #define LBUFSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define DUMPSEL Fld(4,8,AC_MSKB1)//[11:8]
    #define ICTST Fld(1,7,AC_MSKB0)//[7:7]
    #define ICTSTLOCK Fld(1,6,AC_MSKB0)//[6:6]
    #define HOLD Fld(1,5,AC_MSKB0)//[5:5]
    #define FSKBACK Fld(1,4,AC_MSKB0)//[4:4]
    #define ENSHORT Fld(1,3,AC_MSKB0)//[3:3]
    #define CLRSRAM Fld(1,2,AC_MSKB0)//[2:2]
    #define ENYCCKILL Fld(1,1,AC_MSKB0)//[1:1]
    #define ENCKILL Fld(1,0,AC_MSKB0)//[0:0]
#define COMB_CTRL_04 (IO_COMB_BASE + 0x650)
    #define CRCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define DUMPNOW Fld(1,27,AC_MSKB3)//[27:27]
    #define HLEN3D_M Fld(11,16,AC_MSKW32)//[26:16]
    #define WVSYNCTH_0 Fld(1,15,AC_MSKB1)//[15:15]
    #define ENDUMPENDADR Fld(1,14,AC_MSKB1)//[14:14]
    #define ADB4SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define AD10BITMODE Fld(1,11,AC_MSKB1)//[11:11]
    #define AD8BITMODE Fld(1,10,AC_MSKB1)//[10:10]
    #define AD4BITMODE Fld(1,9,AC_MSKB1)//[9:9]
    #define DUMP_LCNTSEL Fld(9,0,AC_MSKW10)//[8:0]
#define COMB_CTRL_05 (IO_COMB_BASE + 0x654)
    #define VCNT3D_M Fld(7,25,AC_MSKB3)//[31:25]
    #define VLEN3D_M Fld(9,16,AC_MSKW32)//[24:16]
    #define HSYNCST Fld(5,11,AC_MSKB1)//[15:11]
    #define WINSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define HCNT3D_M Fld(10,0,AC_MSKW10)//[9:0]
#define COMB_CTRL_06 (IO_COMB_BASE + 0x658)
    #define C_START_DISTANCE Fld(16,16,AC_FULLW32)//[31:16]
    #define C_PALMODE Fld(1,12,AC_MSKB1)//[12:12]
    #define C_ENGEN Fld(1,11,AC_MSKB1)//[11:11]
    #define C_FIELDI_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define C_LCNT_TOTAL Fld(10,0,AC_MSKW10)//[9:0]
#define COMB_CTRL_07 (IO_COMB_BASE + 0x65C)
    #define C_COMB_VCOUNT_REACH Fld(10,16,AC_MSKW32)//[25:16]
    #define C_VS_LCNT_FALL_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_VS_LCNT_RISE_1 Fld(8,0,AC_FULLB0)//[7:0]
#define COMB_CTRL_08 (IO_COMB_BASE + 0x660)
    #define TIMEGEN_WIN_OR_ENABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define DIS_AUTO_ENGEN Fld(1,29,AC_MSKB3)//[29:29]
    #define CB_VFSEL Fld(2,27,AC_MSKB3)//[28:27]
    #define WIN_DISABLE Fld(1,26,AC_MSKB3)//[26:26]
    #define C_X_Y_DISABLE Fld(1,25,AC_MSKB3)//[25:25]
    #define C_ENGEN_V_LOCK_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define C_VS_LCNT_FALL_2 Fld(9,12,AC_MSKW21)//[20:12]
    #define C_VS_LCNT_RISE_2 Fld(9,0,AC_MSKW10)//[8:0]
#define COMB_CTRL_09 (IO_COMB_BASE + 0x664)
    #define C_LINE_LENGTH Fld(11,20,AC_MSKW32)//[30:20]
    #define C_LINE_ACTIVE Fld(10,8,AC_MSKW21)//[17:8]
    #define C_SWITCH_FIELD_LCNT Fld(8,0,AC_FULLB0)//[7:0]
#define COMB_CTRL_0A (IO_COMB_BASE + 0x668)
    #define PCNTH2_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define PCNTH2_1 Fld(24,0,AC_MSKDW)//[23:0]
#define COMB_CTRL_0B (IO_COMB_BASE + 0x66C)
    #define PCNTH1_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define PCNTH1_1 Fld(24,0,AC_MSKDW)//[23:0]
#define COMB_CTRL_0C (IO_COMB_BASE + 0x670)
    #define REG_DRAMENDADR Fld(25,0,AC_MSKDW)//[24:0]
#define COMB_CTRL_0D (IO_COMB_BASE + 0x674)
    #define REG_F2BLEN Fld(4,28,AC_MSKB3)//[31:28]
    #define REG_F1BLEN Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_F0BLEN Fld(4,20,AC_MSKB2)//[23:20]
    #define CTRL_RESERVE1 Fld(15,5,AC_MSKDW)//[19:5]
    #define REG_GOHONLY Fld(1,4,AC_MSKB0)//[4:4]
    #define REG_DISGOHONLY Fld(1,3,AC_MSKB0)//[3:3]
    #define REG_WITHCLRTH_5 Fld(1,2,AC_MSKB0)//[2:2]
    #define REG_WITHCLRTH_4 Fld(1,1,AC_MSKB0)//[1:1]
    #define REG_DIRINSEL Fld(1,0,AC_MSKB0)//[0:0]
#define COMB_CTRL_0E (IO_COMB_BASE + 0x678)
    #define PERFIELDCVBSPHASESUM3 Fld(8,24,AC_FULLB3)//[31:24]
    #define PERFIELDCVBSPHASESUM2 Fld(8,16,AC_FULLB2)//[23:16]
    #define PERFIELDCVBSPHASESUM1 Fld(8,8,AC_FULLB1)//[15:8]
    #define PERFIELDCVBSPHASESUM0 Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_00 (IO_COMB_BASE + 0x67C)
    #define REG_CVSMTHS Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_YSMTH Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_OFFSETTH Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_CVSMTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_01 (IO_COMB_BASE + 0x680)
    #define REG_DIS_HOR_SM Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_EN_TMB_4PV Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_DISMO4PV Fld(1,29,AC_MSKB3)//[29:29]
    #define DIS_BACKGROUND_SAME Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_V_EDGE_TH Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_C_DIFF_TH_4PV Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_MBTH Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_HFTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_02 (IO_COMB_BASE + 0x684)
    #define REG_ENPALDRAM Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_ENSWDET Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_MBTHSEL Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_ENTMB Fld(1,27,AC_MSKB3)//[27:27]
    #define REG_ONE_LINE_MOTION_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define REG_CVBS_MOTION_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define REG_GRAYVERSMSEL Fld(1,24,AC_MSKB3)//[24:24]
    #define REG_DISSWEEPDET Fld(1,23,AC_MSKB2)//[23:23]
    #define REG_DISSTILLHS Fld(1,22,AC_MSKB2)//[22:22]
    #define REG_ENHSTO3D Fld(1,21,AC_MSKB2)//[21:21]
    #define REG_EN2DPUREVER1 Fld(1,20,AC_MSKB2)//[20:20]
    #define REG_ENCVMO Fld(1,19,AC_MSKB2)//[19:19]
    #define REG_ENMBEXIST Fld(1,18,AC_MSKB2)//[18:18]
    #define REG_ENCVBSVERSMOOTH Fld(1,17,AC_MSKB2)//[17:17]
    #define REG_ENCVBSHORSMOOTH Fld(1,16,AC_MSKB2)//[16:16]
    #define REG_HORMBTH Fld(5,8,AC_MSKB1)//[12:8]
    #define REG_VERMBTH Fld(5,0,AC_MSKB0)//[4:0]
#define COMB3D_03 (IO_COMB_BASE + 0x688)
    #define REG_CVBS_DIFF_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_LUMA_DIFF_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define ONE_LINE_VER_DIFF Fld(8,8,AC_FULLB1)//[15:8]
    #define ONE_LINE_HOR_TH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_04 (IO_COMB_BASE + 0x68C)
    #define REG_PATCH_TMB_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_PATCH_COST_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_PATCH_LUMATH Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_PATCH_COST_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_05 (IO_COMB_BASE + 0x690)
    #define RESERVE3D_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_EN_PATCH_STAIR Fld(1,16,AC_MSKB2)//[16:16]
    #define REG_MATRIX_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define REG_RAMP1_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define REG_RAMP_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define REG_EN_ANT_PATCH Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_LINEAR_VAR_2D_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define REG_LINEAR_VAR_3D_TH Fld(4,4,AC_MSKB0)//[7:4]
    #define REG_LINEAR_VAR_2D_TH2 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_06 (IO_COMB_BASE + 0x694)
    #define REG_UNIFORMTH Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_UNIAMPSM Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_LOSTI_LUMATH Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_LOSTI_CVBSTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_07 (IO_COMB_BASE + 0x698)
    #define REG_MOTION9X3TH Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_MOTION9X3TH_B Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_D3GAIN_Y_LS Fld(4,4,AC_MSKB0)//[7:4]
    #define REG_D3GAIN_CV_LS Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_08 (IO_COMB_BASE + 0x69C)
	#define REG_WHIVPHTH Fld(4,28,AC_MSKB3)//[31:28]
	#define REG_HORCVBS_MAX_EN Fld(1,27,AC_MSKB3)//[27:27]
	#define REG_PENALTY2D_HV_MAX_EN Fld(1,26,AC_MSKB3)//[26:26]
	#define REG_SMALLONSEL Fld(1,25,AC_MSKB3)//[25:25]
    #define REG_ENPALCCS_NEW Fld(1,24,AC_MSKB3)//[24:24]
    #define REG_UNIFORMTH4CCS Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_BETTERTH Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_D2SMALLTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_09 (IO_COMB_BASE + 0x6A0)
    #define REG_3D_FW_COST Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_2D_FW_COST Fld(8,16,AC_FULLB2)//[23:16]
    #define DIS_WRONG_EDGE Fld(1,15,AC_MSKB1)//[15:15]
    #define WRONG_EDGE_TH Fld(3,12,AC_MSKB1)//[14:12]
    #define REG_D2VERCVBSSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_PAL2DVERCVBSSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define REG_PENALTY2D_MAX Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_0A (IO_COMB_BASE + 0x6A4)
    #define EN_TWO_LINE_MAX Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_ENLOCALSTILLG_B Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_ENLOCALSTILLG Fld(1,27,AC_MSKB3)//[27:27]
    #define REG_EN_SCENECHANGE Fld(1,26,AC_MSKB3)//[26:26]
    #define REG_WEIGHTLPFSEL Fld(2,24,AC_MSKB3)//[25:24]
    #define REG_D3BIGTH1 Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_MOTION9X3TH_LS_NTSC Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_LINEAR_VAR_OFFSET Fld(4,4,AC_MSKB0)//[7:4]
    #define REG_LINEAR_VAR_MAX Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_0B (IO_COMB_BASE + 0x6A8)
    #define REG_D3BIGTH_SW Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_D2D3SMALLTH Fld(7,16,AC_MSKB2)//[22:16]
    #define REG_INPHASE_NTSC_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define REG_ENSWEEPGDET Fld(1,14,AC_MSKB1)//[14:14]
    #define REG_D3BIGTH_MIN Fld(2,12,AC_MSKB1)//[13:12]
    #define REG_D3BIGTH Fld(10,0,AC_MSKW10)//[9:0]
#define COMB3D_0C (IO_COMB_BASE + 0x6AC)
    #define CHROMA3D_OFFSET Fld(8,24,AC_FULLB3)//[31:24]
    #define CHROMA3D_SLOP Fld(8,16,AC_FULLB2)//[23:16]
    #define LUMAHOR_OFFSET Fld(8,8,AC_FULLB1)//[15:8]
    #define LUMAHOR_SLOP Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_0D (IO_COMB_BASE + 0x6B0)
    #define REG_MOTH4MB Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_PERIOD_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define DRAM_FRAME_CNT1 Fld(3,8,AC_MSKB1)//[10:8]
    #define DRAM_FRAME_CNT2 Fld(3,4,AC_MSKB0)//[6:4]
    #define DRAM_FRAME_CNT3 Fld(3,0,AC_MSKB0)//[2:0]
#define COMB3D_0E (IO_COMB_BASE + 0x6B4)
    #define REG_PERIOD_STEP Fld(8,24,AC_FULLB3)//[31:24]
    #define SP_VAR_MAX_RANGE1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SP_VAR_MAX_RANGE2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SP_VAR_TH Fld(8,0,AC_FULLB0)//[7:0]

//Page COMB_2
#define COMB3D_0F (IO_COMB_BASE + 0x6B8)
    #define REG_D2GAINCV_MB Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_D3GAINY Fld(4,20,AC_MSKB2)//[23:20]
    #define REG_D3GAINC Fld(4,16,AC_MSKB2)//[19:16]
    #define REG_D3GAINCV Fld(4,12,AC_MSKB1)//[15:12]
    #define REG_D2GAINY Fld(4,8,AC_MSKB1)//[11:8]
    #define REG_D2GAINC Fld(4,4,AC_MSKB0)//[7:4]
    #define REG_D2GAINCV Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_10 (IO_COMB_BASE + 0x6BC)
    #define REG_WHSWEEPEDGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_D3GAIN_Y_SW Fld(4,20,AC_MSKB2)//[23:20]
    #define REG_D3GAIN_C_SW Fld(4,16,AC_MSKB2)//[19:16]
    #define REG_D3GAIN_CV_SW Fld(4,12,AC_MSKB1)//[15:12]
    #define REG_D2GAIN_Y_SW Fld(4,8,AC_MSKB1)//[11:8]
    #define REG_D2GAIN_C_SW Fld(4,4,AC_MSKB0)//[7:4]
    #define REG_D2GAIN_CV_SW Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_11 (IO_COMB_BASE + 0x6C0)
	#define REG_YDIFTH Fld(8,20,AC_MSKW32)//[27:20]
    #define REG_D3GAIN_Y_SC Fld(4,16,AC_MSKB2)//[19:16]
    #define REG_D3GAIN_C_SC Fld(4,12,AC_MSKB1)//[15:12]
    #define REG_D3GAIN_CV_SC Fld(4,8,AC_MSKB1)//[11:8]
    #define FLASH_FRM_MAX Fld(4,4,AC_MSKB0)//[7:4]
    #define FLASH_FRM Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_12 (IO_COMB_BASE + 0x6C4)
    #define FLASH_NEI_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define FLASH_MAX_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define FLASH_AVG_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define FLASH_PIX_TH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_13 (IO_COMB_BASE + 0x6C8)
	#define REG_DZSM2DTH Fld(8,20,AC_MSKW32)//[27:20]
    #define D2D3SMALL_PRIORITY Fld(1,19,AC_MSKB2)//[19:19]
    #define REG_WDATASEL Fld(2,16,AC_MSKB2)//[17:16]
    #define REG_MOTIONTH Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_CHECKFTH Fld(4,4,AC_MSKB0)//[7:4]
    #define REG_CHECKMAXF Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_14 (IO_COMB_BASE + 0x6CC)
    #define REG_DARKG3 Fld(4,28,AC_MSKB3)//[31:28]
    #define REG_DARKG2 Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_DARKTH Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_ENYDIF Fld(1,15,AC_MSKB1)//[15:15]
    #define REG_ENGAINSTL Fld(1,14,AC_MSKB1)//[14:14]
    #define REG_ENDARKG Fld(1,13,AC_MSKB1)//[13:13]
    #define REG_ENGAINSML Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_ENSWC3DSM Fld(1,11,AC_MSKB1)//[11:11]
    #define REG_CCDTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_15 (IO_COMB_BASE + 0x6D0)
    #define REG_GAIN3DST Fld(4,28,AC_MSKB3)//[31:28]
    #define REG_GAIN2DST Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_GAIN3D_C3DSM Fld(4,20,AC_MSKB2)//[23:20]
    #define REG_GAIN2D_C3DSM Fld(4,16,AC_MSKB2)//[19:16]
    #define REG_GAIN3DL Fld(4,12,AC_MSKB1)//[15:12]
    #define REG_GAIN2DL Fld(4,8,AC_MSKB1)//[11:8]
    #define REG_GAIN3DN Fld(4,4,AC_MSKB0)//[7:4]
    #define REG_GAIN2DN Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_16 (IO_COMB_BASE + 0x6D4)
    #define REG_WHLUMASUMTH Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_ENWHDCDIF Fld(1,19,AC_MSKB2)//[19:19]
    #define REG_ENWHLUMA Fld(1,15,AC_MSKB1)//[15:15]
    #define REG_ENWHMBPIX Fld(1,14,AC_MSKB1)//[14:14]
    #define REG_ENWHCEDGE Fld(1,13,AC_MSKB1)//[13:13]
    #define REG_ENWHYDEPI Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_ENWHEDGE Fld(1,11,AC_MSKB1)//[11:11]
    #define REG_ENWHCOLOR Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_WHYDETH Fld(10,0,AC_MSKW10)//[9:0]
#define COMB3D_17 (IO_COMB_BASE + 0x6D8)
    #define REG_EN_FLASH_STATUS Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_METRIC_INK Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_START_M_INK_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define REG_WHITEINK Fld(1,24,AC_MSKB3)//[24:24]
    #define REG_SCENE_CHANGE_SEQ_INK_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define REG_INK_GREY_LEVEL Fld(1,22,AC_MSKB2)//[22:22]
    #define REG_SWININK Fld(1,21,AC_MSKB2)//[21:21]
    #define REG_WININK Fld(1,20,AC_MSKB2)//[20:20]
    #define REG_DRAM_REQ_INK Fld(1,19,AC_MSKB2)//[19:19]
    #define REG_PALSWEEPINK Fld(1,18,AC_MSKB2)//[18:18]
    #define REG_TOTALINK Fld(6,12,AC_MSKW21)//[17:12]
    #define REG_INKON Fld(1,11,AC_MSKB1)//[11:11]
    #define REG_WITHCLRTH_1 Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_INKTH Fld(10,0,AC_MSKW10)//[9:0]
#define COMB3D_18 (IO_COMB_BASE + 0x6DC)
    #define REG_C3D_FIR_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_EN_C3D_FIR Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_ENFORCEC3D Fld(1,23,AC_MSKB2)//[23:23]
    #define REG_ENFORCEY3D Fld(1,22,AC_MSKB2)//[22:22]
    #define REG_ENFORCEY2D Fld(1,21,AC_MSKB2)//[21:21]
    #define REG_DISFIRINCBG Fld(1,20,AC_MSKB2)//[20:20]
    #define REG_ENADAPTCFIR Fld(1,19,AC_MSKB2)//[19:19]
    #define REG_ENFORCECFIR Fld(1,18,AC_MSKB2)//[18:18]
    #define REG_CFIRSEL Fld(1,17,AC_MSKB2)//[17:17]
    #define REG_DISCLUMAEDGE Fld(1,16,AC_MSKB2)//[16:16]
    #define REG_FIRMOTIONTH Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_CHROMASUMBGTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_19 (IO_COMB_BASE + 0x6E0)
    #define REG_SENCECHANGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_WHSWEEPSUMTH Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_MOTION_THBIG Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_SMLMOTHC Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_1A (IO_COMB_BASE + 0x6E4)
    #define REG_NOISETH Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_CZPTH Fld(7,16,AC_MSKB2)//[22:16]
    #define MANUAL_NOISE Fld(1,13,AC_MSKB1)//[13:13]
    #define MANUAL_NOISE_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_SMLCNTH Fld(12,0,AC_MSKW10)//[11:0]
#define COMB3D_1B (IO_COMB_BASE + 0x6E8)
    #define REG_DISSWEETDET Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_ENSTFI Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_NCSEL Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_PALC5X3SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_PALSW2 Fld(2,26,AC_MSKB3)//[27:26]
    #define REG_PALSW1 Fld(2,24,AC_MSKB3)//[25:24]
    #define REG_SAMECSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define REG_ENSAMEC Fld(1,22,AC_MSKB2)//[22:22]
    #define REG_DELAYSEL2 Fld(2,20,AC_MSKB2)//[21:20]
    #define REG_BLDCSEL Fld(2,18,AC_MSKB2)//[19:18]
    #define REG_UVSMSEL Fld(2,16,AC_MSKB2)//[17:16]
    #define REG_ENWHOLEVSMOOTH Fld(1,15,AC_MSKB1)//[15:15]
    #define REG_ENBSWDETPALHIGH Fld(1,14,AC_MSKB1)//[14:14]
    #define REG_ENBSWDETPAL Fld(1,13,AC_MSKB1)//[13:13]
    #define REG_ENSWDETPAL Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_BSWDETPIXPAL Fld(4,8,AC_MSKB1)//[11:8]
    #define REG_BSWDETTHPAL Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_1C (IO_COMB_BASE + 0x6EC)
    #define RESERVE3D_2 Fld(24,8,AC_MSKDW)//[31:8]			// cheng
	//#define REG_DZSM2DTH Fld(8,16,AC_FULLB1)//[23:16]
	//#define REG_YDIFTH Fld(8,8,AC_FULLB0)//[15:8]	
    #define REG_DELAYSEL Fld(2,6,AC_MSKB0)//[7:6]
    #define CB_3D_MOTION_TH Fld(6,0,AC_MSKB0)//[5:0]
#define COMB3D_1D (IO_COMB_BASE + 0x6F0)
    #define CB_3D_RA_MOTION_LEVEL_INI Fld(4,28,AC_MSKB3)//[31:28]
    #define CB_3D_RA_LOW_CONTRAST_TH Fld(3,24,AC_MSKB3)//[26:24]
    #define CB_3D_RA_IIR_FACTOR_DN Fld(4,20,AC_MSKB2)//[23:20]
    #define CB_3D_RA_IIR_FACTOR_UP Fld(4,16,AC_MSKB2)//[19:16]
    #define CB_3D_SP_VAR_MIN Fld(4,12,AC_MSKB1)//[15:12]
    #define CB_3D_SP_VAR_STEP Fld(4,8,AC_MSKB1)//[11:8]
    #define CB_3D_SP_VAR_MAX Fld(7,0,AC_MSKB0)//[6:0]
#define COMB3D_1E (IO_COMB_BASE + 0x6F4)
    #define CB_3D_SP_VAR_W_MIN Fld(4,24,AC_MSKB3)//[27:24]
    #define CB_3D_SP_VAR_TH Fld(4,20,AC_MSKB2)//[23:20]
    #define CB_3D_UV_VAR_TH Fld(4,16,AC_MSKB2)//[19:16]
    #define CB_3D_SMALL_OBJECT_TH Fld(4,12,AC_MSKB1)//[15:12]
    #define CB_3D_MOTION_W_MIN Fld(4,8,AC_MSKB1)//[11:8]
    #define CB_3D_FIELD_MOTION_PIXEL_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_1F (IO_COMB_BASE + 0x6F8)
    #define CB_3D_CUBE_FILTER_Y_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define CB_3D_CUBE_FILTER_C_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define CB_3D_FIX_CUBE_WEIGHT_Y Fld(1,29,AC_MSKB3)//[29:29]
    #define CB_3D_FIX_CUBE_WEIGHT_C Fld(1,28,AC_MSKB3)//[28:28]
    #define CB_3D_FORCE_CUBE_WEIGHT_Y Fld(4,24,AC_MSKB3)//[27:24]
    #define CB_3D_FORCE_CUBE_WEIGHT_C Fld(4,20,AC_MSKB2)//[23:20]
    #define CB_3D_REGION_MOTION_TH Fld(6,8,AC_MSKB1)//[13:8]
    #define CB_3D_MM_MIN Fld(3,5,AC_MSKB0)//[7:5]
    #define CB_3D_MM_TH Fld(5,0,AC_MSKB0)//[4:0]
#define COMB3D_20 (IO_COMB_BASE + 0x6FC)
    #define CB_3D_MOVE_CONF_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define CB_3D_AB_BC_BLEND_RATIO Fld(6,24,AC_MSKB3)//[29:24]
    #define CB_3D_AB_BC_BLEND_MIN_TH Fld(7,16,AC_MSKB2)//[22:16]
    #define CB_3D_MOVE_CONF_RATIO Fld(6,8,AC_MSKB1)//[13:8]
    #define CB_3D_MOVE_CONF_MIN_TH Fld(7,0,AC_MSKB0)//[6:0]
#define COMB3D_21 (IO_COMB_BASE + 0x700)
    #define FW_RESERVE Fld(32,0,AC_FULLDW)//[31:0]
#define COMB3D_22 (IO_COMB_BASE + 0x704)
    #define CB_3D_C_FLICKER_FIELD_BASED_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define CB_3D_C_FLICKER_FIELD_CNT_HW_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define CB_3D_C_FLICKER_DIS_AB_PHASE Fld(1,22,AC_MSKB2)//[22:22]
    #define CB_3D_C_FLICKER_FIELD_CNT_FW Fld(4,16,AC_MSKB2)//[19:16]
    #define CB_3D_C_FLICKER_FIELD_CNT_TH Fld(4,12,AC_MSKB1)//[15:12]
    #define CB_3D_C_FLICKER_CNT_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define CB_3D_C_FLICKER_COST_2D Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_23 (IO_COMB_BASE + 0x708)
    #define CB_3D_SCENE_CHANGE_FOUND Fld(1,31,AC_MSKB3)//[31:31]
    #define CB_3D_SCENE_CHANGE_HW_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define CB_3D_SCENE_CHANGE_INI Fld(3,24,AC_MSKB3)//[26:24]
    #define CB_3D_HW_SCENE_CHANGE_TH Fld(6,16,AC_MSKB2)//[21:16]
    #define CB_3D_C_FLICKER_C_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define CB_3D_C_FLICKER_STATUS_C_TH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB3D_24 (IO_COMB_BASE + 0x70C)
    #define CB_3D_START_M_HW_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define CB_3D_START_M_FW Fld(1,15,AC_MSKB1)//[15:15]
    #define CB_3D_START_M_W_MIN Fld(3,12,AC_MSKB1)//[14:12]
    #define CB_3D_START_M_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define CB_3D_START_M_CNT_TH2 Fld(4,4,AC_MSKB0)//[7:4]
    #define CB_3D_START_M_CNT_TH1 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB3D_25 (IO_COMB_BASE + 0x710)
    #define RESERVE3D_3 Fld(32,0,AC_FULLDW)//[31:0]
#define COMB2D_00 (IO_COMB_BASE + 0x714)
    #define REG_ENMHFY Fld(1,31,AC_MSKB3)//[31:31]
    #define ENSVNO Fld(1,30,AC_MSKB3)//[30:30]
    #define VEXP_C Fld(1,29,AC_MSKB3)//[29:29]
    #define VEXP Fld(1,28,AC_MSKB3)//[28:28]
    #define ENFVY Fld(1,27,AC_MSKB3)//[27:27]
    #define ENFSVNO Fld(1,26,AC_MSKB3)//[26:26]
    #define EN_Y5X3OUT Fld(1,25,AC_MSKB3)//[25:25]
    #define Y2D_CSHAP_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define ENMK4 Fld(1,23,AC_MSKB2)//[23:23]
    #define ENMK3 Fld(1,22,AC_MSKB2)//[22:22]
    #define ENMK2 Fld(1,21,AC_MSKB2)//[21:21]
    #define ENMK1 Fld(1,20,AC_MSKB2)//[20:20]
    #define ENFWEAKC Fld(1,19,AC_MSKB2)//[19:19]
    #define FORCEWEAKC Fld(1,18,AC_MSKB2)//[18:18]
    #define REG_PURE1D Fld(1,17,AC_MSKB2)//[17:17]
    #define CUSE5X3 Fld(1,16,AC_MSKB2)//[16:16]
    #define ENMBNEW_C Fld(1,15,AC_MSKB1)//[15:15]
    #define ENMBNEW_Y Fld(1,14,AC_MSKB1)//[14:14]
    #define ENSHARP Fld(1,13,AC_MSKB1)//[13:13]
    #define ENPUREHORMB Fld(1,12,AC_MSKB1)//[12:12]
    #define ENPUREHORXX Fld(1,11,AC_MSKB1)//[11:11]
    #define ENPALVCD Fld(1,10,AC_MSKB1)//[10:10]
    #define ENPALVCD2 Fld(1,9,AC_MSKB1)//[9:9]
    #define ENFIXVCD Fld(1,8,AC_MSKB1)//[8:8]
    #define ENFIXVCD2 Fld(1,7,AC_MSKB0)//[7:7]
    #define ENUNIFORM Fld(1,6,AC_MSKB0)//[6:6]
    #define ENSGDIRIN Fld(1,5,AC_MSKB0)//[5:5]
    #define INVSGDIRIN Fld(1,4,AC_MSKB0)//[4:4]
    #define ENINVDIADIR Fld(1,3,AC_MSKB0)//[3:3]
    #define ENFIROK Fld(1,2,AC_MSKB0)//[2:2]
    #define FORCEFIR Fld(1,1,AC_MSKB0)//[1:1]
#define COMB2D_01 (IO_COMB_BASE + 0x718)
    #define ENGOH Fld(1,31,AC_MSKB3)//[31:31]
    #define ENVPERS Fld(1,30,AC_MSKB3)//[30:30]
    #define COMB_ATPG_CT Fld(1,29,AC_MSKB3)//[29:29]
    #define COMB_ATPG_OB Fld(1,28,AC_MSKB3)//[28:28]
    #define MBBOUNDSEL Fld(1,27,AC_MSKB3)//[27:27]
    #define DISCFIRINCBG Fld(1,26,AC_MSKB3)//[26:26]
    #define FIRINSEL Fld(1,25,AC_MSKB3)//[25:25]
    #define DISCVBS01234SM Fld(1,24,AC_MSKB3)//[24:24]
    #define ENDIRIN Fld(1,23,AC_MSKB2)//[23:23]
    #define MBLSEL Fld(1,22,AC_MSKB2)//[22:22]
    #define MB51 Fld(1,21,AC_MSKB2)//[21:21]
    #define HFYTBSEL Fld(1,20,AC_MSKB2)//[20:20]
    #define HFYTBSEL2 Fld(1,19,AC_MSKB2)//[19:19]
    #define MBBOUNDARY_Y Fld(1,18,AC_MSKB2)//[18:18]
    #define MBBOUNDARY_C Fld(1,17,AC_MSKB2)//[17:17]
    #define PUREVERPAL Fld(1,16,AC_MSKB2)//[16:16]
    #define EN_VERYLP Fld(1,15,AC_MSKB1)//[15:15]
    #define DHYSEL Fld(1,14,AC_MSKB1)//[14:14]
    #define REG_ENSPC Fld(1,13,AC_MSKB1)//[13:13]
    #define REG_ENSPCPT Fld(1,12,AC_MSKB1)//[12:12]
    #define RESERVE2D_1 Fld(12,0,AC_MSKW10)//[11:0]
#define COMB2D_02 (IO_COMB_BASE + 0x71C)
    #define REG_MHFYK Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_VDGYHORSMOOTHTH Fld(8,16,AC_FULLB2)//[23:16]
    #define DYLOMIN_THR Fld(8,8,AC_FULLB1)//[15:8]
    #define SVNOCGTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_03 (IO_COMB_BASE + 0x720)
    #define MK4 Fld(4,28,AC_MSKB3)//[31:28]
    #define MK3 Fld(4,24,AC_MSKB3)//[27:24]
    #define MK2 Fld(4,20,AC_MSKB2)//[23:20]
    #define MK1 Fld(4,16,AC_MSKB2)//[19:16]
    #define MATRIX_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define SHARP_TBTH Fld(7,0,AC_MSKB0)//[6:0]
#define COMB2D_04 (IO_COMB_BASE + 0x724)
    #define CAMPGTH Fld(8,24,AC_FULLB3)//[31:24]
    #define FIRVERDIFTH Fld(8,16,AC_FULLB2)//[23:16]
    #define PALSW Fld(2,8,AC_MSKB1)//[9:8]
    #define GOHTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_05 (IO_COMB_BASE + 0x728)
    #define INKTH2D Fld(8,24,AC_FULLB3)//[31:24]
    #define VCD_TBTH Fld(7,17,AC_MSKB2)//[23:17]
    #define TBTH_CVAR Fld(7,10,AC_MSKW21)//[16:10]
    #define HORMBTH2D Fld(5,5,AC_MSKW10)//[9:5]
    #define VERMBTH2D Fld(5,0,AC_MSKB0)//[4:0]

//Page COMB_3
#define COMB2D_06 (IO_COMB_BASE + 0x72C)
    #define MBTH Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_LSMOOTH Fld(8,16,AC_FULLB2)//[23:16]
    #define PVCVBSVERTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HDVDTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_07 (IO_COMB_BASE + 0x730)
    #define HFY_YTH2 Fld(7,24,AC_MSKB3)//[30:24]
    #define VWY2CHROMATH Fld(8,16,AC_FULLB2)//[23:16]
    #define VWY2CHROMA_STEP Fld(4,12,AC_MSKB1)//[15:12]
    #define REG_SPC_HALFTH Fld(4,8,AC_MSKB1)//[11:8]
    #define REG_SPC_SUMTH Fld(4,4,AC_MSKB0)//[7:4]
    #define REG_ENUNIFORM_SPCLR Fld(1,3,AC_MSKB0)//[3:3]
    #define REG_ENBOUND Fld(1,2,AC_MSKB0)//[2:2]
    #define REG_BOUNDLEVELSEL Fld(2,0,AC_MSKB0)//[1:0]
#define COMB2D_08 (IO_COMB_BASE + 0x734)
    #define REG_HFY_VYD_GAIN Fld(4,28,AC_MSKB3)//[31:28]
    #define REG_HFY_VCD_GAIN Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_HFY_HYD_GAIN Fld(4,20,AC_MSKB2)//[23:20]
    #define REG_HFY_HCD_GAIN Fld(4,16,AC_MSKB2)//[19:16]
    #define REG_HFY_MBVTH Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_HFY_HDGAIN Fld(4,4,AC_MSKB0)//[7:4]
    #define CTPSEL Fld(2,2,AC_MSKB0)//[3:2]
    #define PVSEL Fld(2,0,AC_MSKB0)//[1:0]
#define COMB2D_09 (IO_COMB_BASE + 0x738)
    #define VYD_GAIN_HFY2 Fld(4,28,AC_MSKB3)//[31:28]
    #define VCD_GAIN_HFY2 Fld(4,24,AC_MSKB3)//[27:24]
    #define HYD_GAIN_HFY2 Fld(4,20,AC_MSKB2)//[23:20]
    #define HCD_GAIN_HFY2 Fld(4,16,AC_MSKB2)//[19:16]
    #define MBVTH_HFY2 Fld(8,8,AC_FULLB1)//[15:8]
    #define HDGAIN_HFY2 Fld(4,4,AC_MSKB0)//[7:4]
    #define CSEL Fld(2,2,AC_MSKB0)//[3:2]
    #define YSEL Fld(2,0,AC_MSKB0)//[1:0]
#define COMB2D_0A (IO_COMB_BASE + 0x73C)
    #define K2H4MB Fld(4,28,AC_MSKB3)//[31:28]
    #define K2HSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define TBTH_K2B Fld(7,16,AC_MSKB2)//[22:16]
    #define K2HVERTH Fld(8,8,AC_FULLB1)//[15:8]
    #define VIPTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_0B (IO_COMB_BASE + 0x740)
    #define K2_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define K2_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define K2_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define K2_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define K2_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define K2_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define K2_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define K2_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_0C (IO_COMB_BASE + 0x744)
    #define VCD_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define VCD_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define VCD_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define VCD_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define VCD_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define VCD_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define VCD_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define VCD_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_0D (IO_COMB_BASE + 0x748)
    //#define U1_MBTH Fld(8,24,AC_FULLB3)//[31:24]
    #define CHANNEL_B_SEL Fld(1,29,AC_MSKB3)//[29:29]
	#define DRAMBASEADR_MSB Fld(1,28,AC_MSKB3)//[28:28]
    #define CON4_B4 Fld(1,27,AC_MSKB3)//[27:27]
    #define CON4_B1 Fld(1,26,AC_MSKB3)//[26:26]
    #define CON4_B2 Fld(1,25,AC_MSKB3)//[25:25]
    #define CON4_B3 Fld(1,24,AC_MSKB3)//[24:24]
    #define U1_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_SPC_UVTH Fld(8,8,AC_FULLB1)//[15:8]
    #define CLGTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_0E (IO_COMB_BASE + 0x74C)
    #define INKSEL Fld(1,30,AC_MSKB3)//[30:30]
    #define COMB2D_REG_INK Fld(6,24,AC_MSKB3)//[29:24]
    #define TBTH_K2 Fld(7,16,AC_MSKB2)//[22:16]
    #define TBTH_K1V Fld(7,8,AC_MSKB1)//[14:8]
    #define TBTH_K1H Fld(7,0,AC_MSKB0)//[6:0]
#define COMB2D_0F (IO_COMB_BASE + 0x750)
    #define UV_DELAYSEL Fld(2,30,AC_MSKB3)//[31:30]
    #define Y2D_COLOR_HSLOPE Fld(5,25,AC_MSKB3)//[29:25]
    #define Y2D_COLOR_HUE_RANGE Fld(6,19,AC_MSKW32)//[24:19]
    #define Y2D_COLOR_SSLOPE Fld(3,16,AC_MSKB2)//[18:16]
    #define Y2D_COLOR_SAT_RANGE Fld(8,8,AC_FULLB1)//[15:8]
    #define Y2D_COLOR_SLOPE Fld(2,5,AC_MSKB0)//[6:5]
    #define Y2D_COLOR_SIMILAR_TH Fld(5,0,AC_MSKB0)//[4:0]
#define COMB2D_10 (IO_COMB_BASE + 0x754)
    #define Y2D_BAND23FILTER_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define Y2D_COLOR_UVSLOPE Fld(3,21,AC_MSKB2)//[23:21]
    #define Y2D_COLOR_UVVAR_TH Fld(6,15,AC_MSKW21)//[20:15]
    #define Y2D_COLOR_GSLOPE Fld(3,6,AC_MSKW10)//[8:6]
    #define Y2D_COLOR_GRAY_RANGE Fld(6,0,AC_MSKB0)//[5:0]
#define COMB2D_11 (IO_COMB_BASE + 0x758)
    #define Y2D_CSHAP_K4_OFFSET Fld(3,28,AC_MSKB3)//[30:28]
    #define Y2D_CSHAP_K3_OFFSET Fld(3,24,AC_MSKB3)//[26:24]
    #define Y2D_CSHAP_K2_OFFSET Fld(3,20,AC_MSKB2)//[22:20]
    #define Y2D_CSHAP_K1_OFFSET Fld(3,16,AC_MSKB2)//[18:16]
    #define Y2D_CSHAP_K4_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define Y2D_CSHAP_K3_GAIN Fld(4,8,AC_MSKB1)//[11:8]
    #define Y2D_CSHAP_K2_GAIN Fld(4,4,AC_MSKB0)//[7:4]
    #define Y2D_CSHAP_K1_GAIN Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_12 (IO_COMB_BASE + 0x75C)
    #define RESERVE2D_2 Fld(12,20,AC_MSKW32)//[31:20]
    #define Y2D_CSHAP_EDGE_TH Fld(6,14,AC_MSKW21)//[19:14]
    #define Y2D_CSHAP_CORING Fld(6,8,AC_MSKB1)//[13:8]
    #define Y2D_CSHAP_CLIP_TH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_13 (IO_COMB_BASE + 0x760)
    #define Y2D_CSHAP_DY_CLIP_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define Y2D_CSHAP_DY_CLIP_GAIN Fld(3,28,AC_MSKB3)//[30:28]
    #define REG_SHARP_2D_CVAR_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define REG_CUBE_CVAR_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define REG_LEGACY_CVAR_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define DATA_POSITION_Y Fld(12,12,AC_MSKW21)//[23:12]
    #define DATA_POSITION_X Fld(12,0,AC_MSKW10)//[11:0]
#define COMB2D_14 (IO_COMB_BASE + 0x764)
    #define DUMP_DATA_SEL Fld(2,22,AC_MSKB2)//[23:22]
    #define DUMP_DATA_B Fld(10,12,AC_MSKW21)//[21:12]
    #define DUMP_DATA_A Fld(10,0,AC_MSKW10)//[9:0]
#define COMB2D_15 (IO_COMB_BASE + 0x768)
    #define C2D_COLOR_HUE_RANGE Fld(6,24,AC_MSKB3)//[29:24]
    #define C2D_COLOR_SAT_RANGE Fld(8,16,AC_FULLB2)//[23:16]
    #define C2D_COLOR_GSIMILAR_TH Fld(5,8,AC_MSKB1)//[12:8]
    #define C2D_COLOR_SIMILAR_TH Fld(5,0,AC_MSKB0)//[4:0]
#define COMB2D_16 (IO_COMB_BASE + 0x76C)
    #define C2D_COLOR_FORCE_GRAYWGTC Fld(2,30,AC_MSKB3)//[31:30]
	#define C2D_COLOR_FORCE_COLORWGTC Fld(2,28,AC_MSKB3)//[29:28]
	#define C2D_COLOR_SMIN_YDIFF Fld(10,14,AC_MSKW21)//[23:14]
    #define C2D_COLOR_UVVAR_TH Fld(6,8,AC_MSKB1)//[13:8]
    #define C2D_COLOR_GRAY_RANGE Fld(6,0,AC_MSKB0)//[5:0]
#define COMB2D_17 (IO_COMB_BASE + 0x770)
    #define C2D_COLOR_CBANDWGT_BSUMTH Fld(8,24,AC_FULLB3)//[31:24]
    #define C2D_COLOR_GBANDWGT_BSUMTH Fld(8,16,AC_FULLB2)//[23:16]
    #define C2D_COLOR_CBANDWGT_YRNGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define C2D_COLOR_GBANDWGT_YRNGTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_18 (IO_COMB_BASE + 0x774)
    #define C2D_COLOR_GRNG_YRNG Fld(8,24,AC_FULLB3)//[31:24]
    #define C2D_COLOR_GRNG_BSUM Fld(8,16,AC_FULLB2)//[23:16]
    #define C2D_COLOR_MAXGRNG_YRNG Fld(8,8,AC_FULLB1)//[15:8]
    #define C2D_COLOR_MAXGRNG_BSUM Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_19 (IO_COMB_BASE + 0x778)
	#define Y2D_CBAND_CSEL Fld(2,30,AC_MSKB3)//[31:30]
	#define C2D_COLOR_CLINE_HRANGE Fld(6,24,AC_MSKB3)//[29:24]
    #define C2D_COLOR_CLINE_GRANGE Fld(6,18,AC_MSKB2)//[23:18]
    #define C2D_COLOR_GWGT_SATLIMIT_TYPE Fld(2,16,AC_MSKB2)//[17:16]
    #define C2D_COLOR_GWGT_YRNG Fld(8,8,AC_FULLB1)//[15:8]
    #define C2D_COLOR_GWGT_BSUM Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_1A (IO_COMB_BASE + 0x77C)
    #define C2D_CBAND_GRAY_RATIO Fld(3,29,AC_MSKB3)//[31:29]
	#define C2D_COLOR_CLINE_GDIFF Fld(5,24,AC_MSKB3)//[28:24]
    #define C2D_COLOR_CLINE_HSIMILAR Fld(5,16,AC_MSKB2)//[20:16]
    #define C2D_COLOR_CLINE_YRNG Fld(8,8,AC_FULLB1)//[15:8]
    #define C2D_COLOR_CLINE_BSUM Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_1B (IO_COMB_BASE + 0x780)
    #define C2D_CBAND_FORCE_CBWGT Fld(2,30,AC_MSKB3)//[31:30]
    #define C2D_CBAND_RVRATIO Fld(5,24,AC_MSKB3)//[28:24]
    #define C2D_CBAND_YRNG Fld(8,16,AC_FULLB2)//[23:16]
    #define C2D_CBAND_HRNG Fld(8,8,AC_FULLB1)//[15:8]
    #define C2D_CBAND_CDIFF Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_1C (IO_COMB_BASE + 0x784)
    #define Y2D_CBAND_BLENDWGT Fld(4,28,AC_MSKB3)//[31:28]
    #define Y2D_CBAND_MODE Fld(2,24,AC_MSKB3)//[25:24]
    #define C2D_CBAND_CVRATIO Fld(5,16,AC_MSKB2)//[20:16]
    #define C2D_CBAND_CVDIFF Fld(8,8,AC_FULLB1)//[15:8]
    #define C2D_CBAND_MAXCV Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_1D (IO_COMB_BASE + 0x788)
    #define FRM_SAT_SUM Fld(8,24,AC_FULLB3)//[31:24]
    #define FRM_UVVAR_SUM Fld(8,16,AC_FULLB2)//[23:16]
    #define FRM_YBAND_VSHIFT Fld(4,12,AC_MSKB1)//[15:12]
    #define FRM_YBAND_HSHIFT Fld(4,8,AC_MSKB1)//[11:8]
    #define FRM_SAT_SUM_VSHIFT Fld(4,4,AC_MSKB0)//[7:4]
    #define FRM_SAT_SUM_HSHIFT Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_1E (IO_COMB_BASE + 0x78C)
    #define FRM_YBAND1_SUM Fld(8,24,AC_FULLB3)//[31:24]
    #define FRM_YBAND2_SUM Fld(8,16,AC_FULLB2)//[23:16]
    #define FRM_YBAND3_SUM Fld(8,8,AC_FULLB1)//[15:8]
    #define FRM_YBAND4_SUM Fld(8,0,AC_FULLB0)//[7:0]







	

#endif // #ifndef _HW_TDC_H_

