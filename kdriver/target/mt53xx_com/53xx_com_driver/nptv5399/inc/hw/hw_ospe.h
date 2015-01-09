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
 * $RCSfile: hw_ospe.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

// *********************************************************************
// Down Scaler related Register and Macro
// *********************************************************************
#ifndef _HW_OSPE_H_
#define _HW_OSPE_H_

#include "hw_vdoin.h"

#define IO_POSTPROC_BASE  (VDOIN_BASE+0x7000)

#define OSTG_00 (IO_POSTPROC_BASE + 0x800)
    #define R_OSTG2MON_SEL Fld(2,26,AC_MSKB3)//[27:26]
    #define C_ITG_RES Fld(2,24,AC_MSKB3)//[25:24]
    #define C_ITG_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define R_OSTG2VDOIN_SEL Fld(1,22,AC_MSKB2)//[22:22]
    #define C_DBUF_POCLK_ON Fld(1,21,AC_MSKB2)//[21:21]
    #define C_DBUF_OCLK_ON Fld(1,20,AC_MSKB2)//[20:20]
    #define C_BYPASS_TTD Fld(1,19,AC_MSKB2)//[19:19]
    #define R_POST_PWMCLK_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define R_POST_OCLK_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define R_POST_POCLK_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define REG_PTGEN_FOR_XVYCC Fld(1,7,AC_MSKB0)//[7:7]
    #define REG_XVYCC_CLIP_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define C_SET_DATA_VS_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define REG_SET_DATA_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define CLK2_RST_CTRL Fld(1,2,AC_MSKB0)//[2:2]
    #define CLK_RST_CTRL Fld(1,1,AC_MSKB0)//[1:1]
    #define REG_RST_CTRL Fld(1,0,AC_MSKB0)//[0:0]
#define OSTG_01 (IO_POSTPROC_BASE + 0x804)
    #define C_R2Y_FORMAT Fld(2,21,AC_MSKB2)//[22:21]
    #define R_BYPASS_OD Fld(1,20,AC_MSKB2)//[20:20]
    #define REG_3DTV_OUT_SEL Fld(2,17,AC_MSKB2)//[18:17]
    #define REG_C_LINE_EXT Fld(1,16,AC_MSKB2)//[16:16]
    #define REG_LPF_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define REG_UVINV Fld(1,14,AC_MSKB1)//[14:14]
    #define REG_YUV2YC_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define REG_RGB2YUV_BYPASS Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_MAIN_TOP_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define REG_OP_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define C_MJC_MASK_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define REG_MJC_IN_EN Fld(1,0,AC_MSKB0)//[0:0]
#define YCV_OFFSET_00 (IO_POSTPROC_BASE + 0x808)
    #define R_YCV_PTGEN_CBAR Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_PTGEN_PIP_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_PTGEN_MAIN_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define CIN_OFFSET_MAIN Fld(12,16,AC_MSKW32)//[27:16]
    #define YIN_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define YCV_OFFSET_01 (IO_POSTPROC_BASE + 0x80C)
    #define YIN_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define VIN_OFFSET_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define YCV_OFFSET_02 (IO_POSTPROC_BASE + 0x810)
    #define VIN_OFFSET_PIP Fld(12,16,AC_MSKW32)//[27:16]
    #define CIN_OFFSET_PIP Fld(12,0,AC_MSKW10)//[11:0]
#define YCV_OFFSET_03 (IO_POSTPROC_BASE + 0x814)
    #define M_YSUM Fld(32,0,AC_FULLDW)//[31:0]
#define YCV_OFFSET_04 (IO_POSTPROC_BASE + 0x818)
    #define P_YSUM Fld(32,0,AC_FULLDW)//[31:0]
#define YCV_OFFSET_05 (IO_POSTPROC_BASE + 0x81C)
    #define M_YMAX Fld(8,24,AC_FULLB3)//[31:24]
    #define P_YMAX Fld(8,16,AC_FULLB2)//[23:16]
    #define M_YMIN Fld(8,8,AC_FULLB1)//[15:8]
    #define P_YMIN Fld(8,0,AC_FULLB0)//[7:0]
#define YCBCR2RGB_00 (IO_POSTPROC_BASE + 0x820)
    #define YUV2RGB_BYPASS_MAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define YUV2RGB_BYPASS_PIP Fld(1,30,AC_MSKB3)//[30:30]
    #define Y2R_MATRIX00 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX01 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_01 (IO_POSTPROC_BASE + 0x824)
    #define Y2R_MATRIX10 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX02 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_02 (IO_POSTPROC_BASE + 0x828)
    #define Y2R_MATRIX12 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX11 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_03 (IO_POSTPROC_BASE + 0x82C)
    #define Y2R_MATRIX21 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX20 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_04 (IO_POSTPROC_BASE + 0x830)
    #define Y2R_MATRIX00_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX22 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_05 (IO_POSTPROC_BASE + 0x834)
    #define Y2R_MATRIX02_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX01_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_06 (IO_POSTPROC_BASE + 0x838)
    #define Y2R_MATRIX11_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX10_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_07 (IO_POSTPROC_BASE + 0x83C)
    #define Y2R_MATRIX12_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX20_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_08 (IO_POSTPROC_BASE + 0x840)
    #define Y2R_MATRIX22_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX21_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define RGBOFFSET_00 (IO_POSTPROC_BASE + 0x844)
    #define R_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define R_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define RGBOFFSET_01 (IO_POSTPROC_BASE + 0x848)
    #define G_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define G_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define RGBOFFSET_02 (IO_POSTPROC_BASE + 0x84C)
    #define B_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define B_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define MUTE_00 (IO_POSTPROC_BASE + 0x858)
    #define B_BACKGROUND Fld(8,24,AC_FULLB3)//[31:24]
    #define G_BACKGROUND Fld(8,16,AC_FULLB2)//[23:16]
    #define R_BACKGROUND Fld(8,8,AC_FULLB1)//[15:8]
    #define BGR_BACKGROUND Fld(24,8,AC_MSKDW)//[31:8]    
    #define R_MUTE_POST_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define R_P_MUTE_FRONT_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define R_M_MUTE_FRONT_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define R_MUTE_VS_LATCH_OFF Fld(1,3,AC_MSKB0)//[3:3]
    #define R_MUTE_POST_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define R_P_MUTE_FRONT_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define R_M_MUTE_FRONT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define MUTE_01 (IO_POSTPROC_BASE + 0x85C)
    #define C_MUTE_FRAME_CNT Fld(3,28,AC_MSKB3)//[30:28]
    #define G_MUTE_MAIN Fld(12,16,AC_MSKW32)//[27:16]
    #define B_MUTE_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_02 (IO_POSTPROC_BASE + 0x860)
    #define B_MUTE_PIP Fld(12,16,AC_MSKW32)//[27:16]
    #define R_MUTE_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_03 (IO_POSTPROC_BASE + 0x864)
    #define R_MUTE_PIP Fld(12,16,AC_MSKW32)//[27:16]
    #define G_MUTE_PIP Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_04 (IO_POSTPROC_BASE + 0x868)
    #define B_BACKGROUND_MJC Fld(8,24,AC_FULLB3)//[31:24]
    #define G_BACKGROUND_MJC Fld(8,16,AC_FULLB2)//[23:16]
    #define R_BACKGROUND_MJC Fld(8,8,AC_FULLB1)//[15:8]
    #define BGR_BACKGROUND_MJC Fld(24,8,AC_MSKDW)//[31:8]
    #define R_MUTE_MJC_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define R_MUTE_MJC_EN Fld(1,0,AC_MSKB0)//[0:0]
#define MUTE_05 (IO_POSTPROC_BASE + 0x86C)
    #define G_MUTE_MAIN_MJC Fld(12,16,AC_MSKW32)//[27:16]
    #define B_MUTE_MAIN_MJC Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_06 (IO_POSTPROC_BASE + 0x870)
    #define R_MUTE_MAIN_MJC Fld(12,0,AC_MSKW10)//[11:0]
#define WINDOW_00 (IO_POSTPROC_BASE + 0x874)
    #define REG_WINDOW_2_ON Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_WINDOW_1_ON Fld(1,30,AC_MSKB3)//[30:30]
    #define WINDOW_COLOR1 Fld(30,0,AC_MSKDW)//[29:0]
#define WINDOW_01 (IO_POSTPROC_BASE + 0x878)
    #define WINDOW_COLOR2 Fld(30,0,AC_MSKDW)//[29:0]
#define WINDOW_02 (IO_POSTPROC_BASE + 0x87C)
    #define WINDOW_1_HPOSLR Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_1_HPOSLL Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_03 (IO_POSTPROC_BASE + 0x880)
    #define WINDOW_1_HPOSRR Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_1_HPOSRL Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_04 (IO_POSTPROC_BASE + 0x884)
    #define WINDOW_1_VPOSUD Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_1_VPOSUU Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_05 (IO_POSTPROC_BASE + 0x888)
    #define WINDOW_1_VPOSDD Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_1_VPOSDU Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_06 (IO_POSTPROC_BASE + 0x88C)
    #define WINDOW_2_HPOSLR Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_2_HPOSLL Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_07 (IO_POSTPROC_BASE + 0x890)
    #define WINDOW_2_HPOSRR Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_2_HPOSRL Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_08 (IO_POSTPROC_BASE + 0x894)
    #define WINDOW_2_VPOSUD Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_2_VPOSUU Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_09 (IO_POSTPROC_BASE + 0x898)
    #define WINDOW_2_VPOSDD Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_2_VPOSDU Fld(16,0,AC_FULLW10)//[15:0]
#define GAINOSET_00 (IO_POSTPROC_BASE + 0x89C)
    #define RGB_GAIN_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define R_GAIN_MAIN Fld(10,20,AC_MSKW32)//[29:20]
    #define G_GAIN_MAIN Fld(10,10,AC_MSKW21)//[19:10]
    #define B_GAIN_MAIN Fld(10,0,AC_MSKW10)//[9:0]
#define GAINOSET_01 (IO_POSTPROC_BASE + 0x8A0)
    #define R_GAIN_PIP Fld(10,20,AC_MSKW32)//[29:20]
    #define G_GAIN_PIP Fld(10,10,AC_MSKW21)//[19:10]
    #define B_GAIN_PIP Fld(10,0,AC_MSKW10)//[9:0]
#define GAINOSET_02 (IO_POSTPROC_BASE + 0x8A4)
    #define R_OFST1_MAIN Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST1_MAIN Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST1_MAIN Fld(10,2,AC_MSKW10)//[11:2]
#define GAINOSET_03 (IO_POSTPROC_BASE + 0x8A8)
    #define R_OFST2_MAIN Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST2_MAIN Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST2_MAIN Fld(10,2,AC_MSKW10)//[11:2]
#define GAINOSET_04 (IO_POSTPROC_BASE + 0x8AC)
    #define R_OFST1_PIP Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST1_PIP Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST1_PIP Fld(10,2,AC_MSKW10)//[11:2]
    #define REG_BG_GAIN_OFS Fld(2,0,AC_MSKB0)//[1:0]
#define GAINOSET_05 (IO_POSTPROC_BASE + 0x8B0)
    #define R_OFST2_PIP Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST2_PIP Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST2_PIP Fld(10,2,AC_MSKW10)//[11:2]
#define MAXMETER_00 (IO_POSTPROC_BASE + 0x8B4)
    #define C_MAX_METER_X_END Fld(13,13,AC_MSKDW)//[25:13]
    #define C_MAX_METER_X_START Fld(13,0,AC_MSKW10)//[12:0]
#define MAXMETER_01 (IO_POSTPROC_BASE + 0x8B8)
    #define C_MAX_METER_SRC_FMT Fld(1,26,AC_MSKB3)//[26:26]
    #define C_MAX_METER_VS_POL Fld(1,25,AC_MSKB3)//[25:25]
    #define C_MAX_METER_HS_POL Fld(1,24,AC_MSKB3)//[24:24]
    #define C_MAX_METER_Y_END Fld(12,12,AC_MSKW21)//[23:12]
    #define C_MAX_METER_Y_START Fld(12,0,AC_MSKW10)//[11:0]
#define MAXMETER_02 (IO_POSTPROC_BASE + 0x8BC)
    #define CNT00 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_03 (IO_POSTPROC_BASE + 0x8C0)
    #define CNT01 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_04 (IO_POSTPROC_BASE + 0x8C4)
    #define CNT02 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_05 (IO_POSTPROC_BASE + 0x8C8)
    #define CNT03 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_06 (IO_POSTPROC_BASE + 0x8CC)
    #define CNT04 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_07 (IO_POSTPROC_BASE + 0x8D0)
    #define CNT05 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_08 (IO_POSTPROC_BASE + 0x8D4)
    #define CNT06 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_09 (IO_POSTPROC_BASE + 0x8D8)
    #define CNT07 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_10 (IO_POSTPROC_BASE + 0x8DC)
    #define CNT08 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_11 (IO_POSTPROC_BASE + 0x8E0)
    #define CNT09 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_12 (IO_POSTPROC_BASE + 0x8E4)
    #define CNT10 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_13 (IO_POSTPROC_BASE + 0x8E8)
    #define CNT11 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_14 (IO_POSTPROC_BASE + 0x8EC)
    #define CNT12 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_15 (IO_POSTPROC_BASE + 0x8F0)
    #define CNT13 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_16 (IO_POSTPROC_BASE + 0x8F4)
    #define CNT14 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_17 (IO_POSTPROC_BASE + 0x8F8)
    #define CNT15 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_18 (IO_POSTPROC_BASE + 0x8FC)
    #define CNT16 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_19 (IO_POSTPROC_BASE + 0x900)
    #define CNT17 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_20 (IO_POSTPROC_BASE + 0x904)
    #define CNT18 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_21 (IO_POSTPROC_BASE + 0x908)
    #define CNT19 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_22 (IO_POSTPROC_BASE + 0x90C)
    #define CNT20 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_23 (IO_POSTPROC_BASE + 0x910)
    #define CNT21 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_24 (IO_POSTPROC_BASE + 0x914)
    #define CNT22 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_25 (IO_POSTPROC_BASE + 0x918)
    #define CNT23 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_26 (IO_POSTPROC_BASE + 0x91C)
    #define CNT24 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_27 (IO_POSTPROC_BASE + 0x920)
    #define CNT25 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_28 (IO_POSTPROC_BASE + 0x924)
    #define CNT26 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_29 (IO_POSTPROC_BASE + 0x928)
    #define CNT27 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_30 (IO_POSTPROC_BASE + 0x92C)
    #define CNT28 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_31 (IO_POSTPROC_BASE + 0x930)
    #define CNT29 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_32 (IO_POSTPROC_BASE + 0x934)
    #define CNT30 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_33 (IO_POSTPROC_BASE + 0x938)
    #define CNT31 Fld(23,0,AC_MSKDW)//[22:0]

//Page OSTG_2
#define MAXMETER_34 (IO_POSTPROC_BASE + 0x93C)
    #define CNT32 Fld(23,0,AC_MSKDW)//[22:0]
#define DBL_00 (IO_POSTPROC_BASE + 0x940)
    #define DBL_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define DBL_G_SLOPE Fld(11,0,AC_MSKW10)//[10:0]
#define DBL_01 (IO_POSTPROC_BASE + 0x944)
    #define DBL_G2 Fld(8,24,AC_FULLB3)//[31:24]
    #define DBL_G1 Fld(8,16,AC_FULLB2)//[23:16]
    #define DBL_P2 Fld(8,8,AC_FULLB1)//[15:8]
    #define DBL_P1 Fld(8,0,AC_FULLB0)//[7:0]
#define GAMMA_00 (IO_POSTPROC_BASE + 0x948)
    #define C_GM_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_GM_SWAP_POST Fld(1,28,AC_MSKB3)//[28:28]
    #define R_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define R_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define R_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define R_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define R_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define R_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define R_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define GAMMA_01 (IO_POSTPROC_BASE + 0x94C)
    #define CPU_SRAM_ADDR Fld(8,16,AC_FULLB2)//[23:16]
    #define R_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define GAMMA_02 (IO_POSTPROC_BASE + 0x950)
    #define R_WDATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define R_WDATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define GAMMA_03 (IO_POSTPROC_BASE + 0x954)
    #define R_WDATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define GAMMA_04 (IO_POSTPROC_BASE + 0x958)
    #define R_RDATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define R_RDATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define GAMMA_05 (IO_POSTPROC_BASE + 0x95C)
    #define R_RDATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define GAMMA_06 (IO_POSTPROC_BASE + 0x960)
    #define CPU_SEL_SRAM_0 Fld(1,7,AC_MSKB0)//[7:7]
    #define R_CPURW_ACTIVE Fld(1,6,AC_MSKB0)//[6:6]
    #define TABLE_WAIT_VSYNC Fld(1,5,AC_MSKB0)//[5:5]
    #define R_FILL_TABLE_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define READ_FINISH Fld(1,3,AC_MSKB0)//[3:3]
    #define R_READ Fld(1,2,AC_MSKB0)//[2:2]
    #define WRITE_FINISH Fld(1,1,AC_MSKB0)//[1:1]
    #define R_WRITE Fld(1,0,AC_MSKB0)//[0:0]
#define GAMMA_09 (IO_POSTPROC_BASE + 0x96C)
    #define C_GM_256_R Fld(13,0,AC_MSKW10)//[12:0]
#define GAMMA_10 (IO_POSTPROC_BASE + 0x970)
    #define C_GM_256_B Fld(13,16,AC_MSKW32)//[28:16]
    #define C_GM_256_G Fld(13,0,AC_MSKW10)//[12:0]
#define OS_DITHER_00 (IO_POSTPROC_BASE + 0x98C)
    #define REG_FPHASE_BIT Fld(3,29,AC_MSKB3)//[31:29]
    #define REG_FPHASE_SEL Fld(2,27,AC_MSKB3)//[28:27]
    #define REG_FPHASE_CTRL Fld(2,25,AC_MSKB3)//[26:25]
    #define DITHER_FPHASE Fld(6,19,AC_MSKW32)//[24:19]
    #define REG_FPHASE_R Fld(1,18,AC_MSKB2)//[18:18]
    #define REG_FPHASE_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define REG_SUB_B Fld(2,15,AC_MSKW21)//[16:15]
    #define REG_SUB_G Fld(2,13,AC_MSKB1)//[14:13]
    #define REG_SUB_R Fld(2,11,AC_MSKB1)//[12:11]
    #define REG_SUBPIX_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_TABLE_EN Fld(2,8,AC_MSKB1)//[9:8]
    #define DITHER_LFSR_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define RDITHER_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define EDITHER_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DITHER_ROUND_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define REG_LEFT_EN Fld(1,2,AC_MSKB0)//[2:2]
#define OS_DITHER_01 (IO_POSTPROC_BASE + 0x990)
    #define REG_INK_DATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define REG_INK_DATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define OS_DITHER_02 (IO_POSTPROC_BASE + 0x994)
    #define REG_INK Fld(1,31,AC_MSKB3)//[31:31]
    #define DITHER_LSB_OFF Fld(1,30,AC_MSKB3)//[30:30]
    #define DRMOD_B Fld(2,16,AC_MSKB2)//[17:16]
    #define DRMOD_G Fld(2,14,AC_MSKB1)//[15:14]
    #define DRMOD_R Fld(2,12,AC_MSKB1)//[13:12]
    #define REG_INK_DATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define OS_DITHER_03 (IO_POSTPROC_BASE + 0x998)
    #define REG_FPHASE_BIT_POST Fld(3,29,AC_MSKB3)//[31:29]
    #define REG_FPHASE_SEL_POST Fld(2,27,AC_MSKB3)//[28:27]
    #define REG_FPHASE_CTRL_POST Fld(2,25,AC_MSKB3)//[26:25]
    #define REG_FPHASE_POST Fld(6,19,AC_MSKW32)//[24:19]
    #define REG_FPHASE_R_POST Fld(1,18,AC_MSKB2)//[18:18]
    #define REG_FPHASE_EN_POST Fld(1,17,AC_MSKB2)//[17:17]
    #define REG_SUB_B_POST Fld(2,15,AC_MSKW21)//[16:15]
    #define REG_SUB_G_POST Fld(2,13,AC_MSKB1)//[14:13]
    #define REG_SUB_R_POST Fld(2,11,AC_MSKB1)//[12:11]
    #define REG_SUBPIX_EN_POST Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_TABLE_EN_POST Fld(2,8,AC_MSKB1)//[9:8]
    #define REG_LFSR_EN_POST Fld(1,6,AC_MSKB0)//[6:6]
    #define REG_RDITHER_EN_POST Fld(1,5,AC_MSKB0)//[5:5]
    #define REG_EDITHER_EN_POST Fld(1,4,AC_MSKB0)//[4:4]
    #define REG_ROUND_EN_POST Fld(1,3,AC_MSKB0)//[3:3]
    #define REG_LEFT_EN_POST Fld(1,2,AC_MSKB0)//[2:2]
#define OS_DITHER_04 (IO_POSTPROC_BASE + 0x99C)
    #define REG_INK_DATA_G_POST Fld(12,16,AC_MSKW32)//[27:16]
    #define REG_INK_DATA_R_POST Fld(12,0,AC_MSKW10)//[11:0]
#define OS_DITHER_05 (IO_POSTPROC_BASE + 0x9A0)
    #define REG_INK_POST Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_LSB_OFF_POST Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_DRMOD_B_POST Fld(2,16,AC_MSKB2)//[17:16]
    #define REG_DRMOD_G_POST Fld(2,14,AC_MSKB1)//[15:14]
    #define REG_DRMOD_R_POST Fld(2,12,AC_MSKB1)//[13:12]
    #define REG_INK_DATA_B_POST Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_00 (IO_POSTPROC_BASE + 0x9A4)
    #define BSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define GSEL Fld(2,6,AC_MSKB0)//[7:6]
    #define RSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define RB_MSBINV Fld(1,3,AC_MSKB0)//[3:3]
    #define MLSB_INV Fld(1,2,AC_MSKB0)//[2:2]
    #define RB_INV Fld(1,1,AC_MSKB0)//[1:1]
    #define DUMP_EN Fld(1,0,AC_MSKB0)//[0:0]
#define DUMP_01 (IO_POSTPROC_BASE + 0x9A8)
    #define POS_Y Fld(16,16,AC_FULLW32)//[31:16]
    #define POS_X Fld(16,0,AC_FULLW10)//[15:0]
#define DUMP_02 (IO_POSTPROC_BASE + 0x9AC)
    #define POS_G Fld(12,16,AC_MSKW32)//[27:16]
    #define POS_R Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_03 (IO_POSTPROC_BASE + 0x9B0)
    #define POS_B Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_04 (IO_POSTPROC_BASE + 0x9B4)
    #define POS_OUT_G Fld(12,16,AC_MSKW32)//[27:16]
    #define POS_OUT_R Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_05 (IO_POSTPROC_BASE + 0x9B8)
    #define POS_OUT_B Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_06 (IO_POSTPROC_BASE + 0x9BC)
    #define R_CMI_3DTV_1ST_G Fld(12,16,AC_MSKW32)//[27:16]
    #define R_CMI_3DTV_1ST_R Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_07 (IO_POSTPROC_BASE + 0x9C0)
    #define R_CMI_3DTV_2ND_R Fld(12,16,AC_MSKW32)//[27:16]
    #define R_CMI_3DTV_1ST_B Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_08 (IO_POSTPROC_BASE + 0x9C4)
    #define R_CMI_3DTV_FMT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define R_CMI_3DTV_AUTO_DIS Fld(1,30,AC_MSKB3)//[30:30]
    #define R_CMI_3DTV_2ND_B Fld(12,16,AC_MSKW32)//[27:16]
    #define R_CMI_3DTV_2ND_G Fld(12,0,AC_MSKW10)//[11:0]

#define CRC32_00 (IO_POSTPROC_BASE + 0x9D8)
    #define R_CRC_STILL_CHECK_DONE Fld(1,20,AC_MSKB2)//[20:20]
    #define R_CRC_NON_STILL_CNT Fld(4,16,AC_MSKB2)//[19:16]
    #define R_CRC_STILL_CHECK_MAX Fld(8,8,AC_FULLB1)//[15:8]
    #define R_CRC_STILL_CHECK_TRIG Fld(1,7,AC_MSKB0)//[7:7]
    #define R_CRC_3D_L_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define R_CRC_3D_R_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define R_CRC_MUX_SEL Fld(5,0,AC_MSKB0)//[4:0]
#define CRC32_01 (IO_POSTPROC_BASE + 0x9DC)
    #define R_CRC_H_END Fld(13,16,AC_MSKW32)//[28:16]
    #define R_CRC_H_START Fld(13,0,AC_MSKW10)//[12:0]
#define CRC32_02 (IO_POSTPROC_BASE + 0x9E0)
    #define R_CRC_V_END Fld(12,16,AC_MSKW32)//[27:16]
    #define R_CRC_V_START Fld(12,0,AC_MSKW10)//[11:0]
#define CRC32_03 (IO_POSTPROC_BASE + 0x9E4)
    #define R_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define R_CRC_C_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define R_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define CRC32_04 (IO_POSTPROC_BASE + 0x9E8)
    #define R_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define DEBUG_00 (IO_POSTPROC_BASE + 0x9EC)
    #define DEBUG_00_INPUT_HACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define DEBUG_00_REG_OUT_MON_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define DEBUG_00_REG_IN_MON_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define DEBUG_00_R_DEBUG_OSTG_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define DEBUG_00_REG_OUT_MON_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define DEBUG_00_REG_IN_MON_EN Fld(1,4,AC_MSKB0)//[4:4]
#define DEBUG_01 (IO_POSTPROC_BASE + 0x9F0)
    #define DEBUG_01_PANEL_DISP_R Fld(1,24,AC_MSKB3)//[24:24]
    #define R_OUT_SEL_OTHER Fld(2,2,AC_MSKB0)//[3:2]
    #define DEBUG_01_R_HVM_INPUT_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define DEBUG_02 (IO_POSTPROC_BASE + 0x9F4)
    #define DEBUG_02_INPUT_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define DEBUG_02_INPUT_HTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define DEBUG_03 (IO_POSTPROC_BASE + 0x9F8)
    #define DEBUG_03_HVM_VTOTAL_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define DEBUG_03_HVM_HTOTAL_CNT Fld(16,0,AC_FULLW10)//[15:0]
#define DEBUG_04 (IO_POSTPROC_BASE + 0x9FC)
    #define DEBUG_04_HVM_VDE_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define DEBUG_04_HVM_HDE_CNT Fld(16,0,AC_FULLW10)//[15:0]
#define SPARE_00 (IO_POSTPROC_BASE + 0xA04)
    #define OSTG_SPARE_REG0 Fld(32,0,AC_FULLDW)//[31:0]
#define SPARE_01 (IO_POSTPROC_BASE + 0xA08)
    #define OSTG_SPARE_REG1 Fld(32,0,AC_FULLDW)//[31:0]
#define OSD_TIMING_00 (IO_POSTPROC_BASE + 0xA14)
    #define OSD_VS_END Fld(3,29,AC_MSKB3)//[31:29]
    #define OSD_VS_START Fld(3,26,AC_MSKB3)//[28:26]
    #define OSD_HS_END Fld(10,16,AC_MSKW32)//[25:16]
    #define OSD_HS_START Fld(8,8,AC_FULLB1)//[15:8]
    #define OSD_HS_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define OSD_VS_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define OSD_HSYNCP Fld(1,2,AC_MSKB0)//[2:2]
    #define OSD_VSYNCP Fld(1,1,AC_MSKB0)//[1:1]
    #define VSOSDA1 Fld(1,0,AC_MSKB0)//[0:0]
#define OSD_TIMING_01 (IO_POSTPROC_BASE + 0xA18)
    #define TOP_MASK Fld(10,16,AC_MSKW32)//[25:16]
    #define BOTTOM_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define OSD_TIMING_02 (IO_POSTPROC_BASE + 0xA1C)
    #define LEFT_MASK Fld(10,16,AC_MSKW32)//[25:16]
    #define RIGHT_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define OSD_TIMING_03 (IO_POSTPROC_BASE + 0xA20)
    #define REG_VSYNC_SEL_OSD Fld(2,24,AC_MSKB3)//[25:24]
    #define REG_VSYNC_OUT_SEL_OSD Fld(2,22,AC_MSKB2)//[23:22]
    #define REG_VCOUNT_ENABLE_OSD Fld(1,21,AC_MSKB2)//[21:21]
    #define REG_HCOUNT_ENABLE_OSD Fld(1,20,AC_MSKB2)//[20:20]
    #define BOTTOM_MASK_OSD Fld(10,10,AC_MSKW21)//[19:10]
    #define TOP_MASK_OSD Fld(10,0,AC_MSKW10)//[9:0]
#define OSD_TIMING_04 (IO_POSTPROC_BASE + 0xA24)
    #define LEFT_MASK_OSD Fld(10,10,AC_MSKW21)//[19:10]
    #define RIGHT_MASK_OSD Fld(10,0,AC_MSKW10)//[9:0]
#define OSD_TIMING_05 (IO_POSTPROC_BASE + 0xA28)
    #define REG_V_OUT_H_OSD Fld(16,0,AC_FULLW10)//[15:0]
#define OSD_MIX_2 (IO_POSTPROC_BASE + 0xA30)
    #define REG_OSD_INDEX_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define REG_OSD_INDEX_THD Fld(8,0,AC_FULLB0)//[7:0]

#define BLUESTCH_00 (IO_POSTPROC_BASE + 0x850)
    #define C_BS_SAT_THD Fld(7,24,AC_MSKB3)//[30:24]
    #define C_BS_Y_THD Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BS_BASE_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BS_ONE_GAIN_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define C_BS_EN_PIP Fld(1,2,AC_MSKB0)//[2:2]
    #define C_BS_INK_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define C_BS_EN_MAIN Fld(1,0,AC_MSKB0)//[0:0]
#define BLUESTCH_01 (IO_POSTPROC_BASE + 0x854)
    #define C_BS_BASE Fld(8,24,AC_FULLB3)//[31:24]
    #define C_BS_GAIN_B Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BS_GAIN_G Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BS_GAIN_R Fld(8,0,AC_FULLB0)//[7:0]
#define BLUESTCH_02 (IO_POSTPROC_BASE + 0xA34)
    #define C_BS_GAIN_B_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BS_GAIN_G_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BS_GAIN_R_1 Fld(8,0,AC_FULLB0)//[7:0]
#define BLUESTCH_03 (IO_POSTPROC_BASE + 0xA3C)
    #define C_BS_BASE_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define C_BS_GAIN_B_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BS_GAIN_G_2 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BS_GAIN_R_2 Fld(8,0,AC_FULLB0)//[7:0]
#define BLUESTCH_04 (IO_POSTPROC_BASE + 0xA40)
    #define C_BS_BASE_3 Fld(8,24,AC_FULLB3)//[31:24]
    #define C_BS_GAIN_B_3 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BS_GAIN_G_3 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BS_GAIN_R_3 Fld(8,0,AC_FULLB0)//[7:0]
#define BLUESTCH_05 (IO_POSTPROC_BASE + 0xA44)
    #define C_BS_BASE_4 Fld(8,24,AC_FULLB3)//[31:24]
    #define C_BS_GAIN_B_4 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BS_GAIN_G_4 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BS_GAIN_R_4 Fld(8,0,AC_FULLB0)//[7:0]
    
#define ANAG_CTRL (IO_POSTPROC_BASE + 0xA38)
    #define ANAG_DIS_VALUE Fld(8,8,AC_FULLB1)//[15:8]
    #define ANAG_REYE_B_DIS Fld(1,7,AC_MSKB0)//[7:7]
    #define ANAG_REYE_G_DIS Fld(1,6,AC_MSKB0)//[6:6]
    #define ANAG_REYE_R_DIS Fld(1,5,AC_MSKB0)//[5:5]
    #define ANAG_LEYE_B_DIS Fld(1,4,AC_MSKB0)//[4:4]
    #define ANAG_LEYE_G_DIS Fld(1,3,AC_MSKB0)//[3:3]
    #define ANAG_LEYE_R_DIS Fld(1,2,AC_MSKB0)//[2:2]
    #define ANAG_REF_ODDEVEN Fld(1,1,AC_MSKB0)//[1:1]
    #define ANAG_3D_EN Fld(1,0,AC_MSKB0)//[0:0]



//=> Copy from Cobra : Not Verified by R2
#define OSD_MIX_0 (IO_POSTPROC_BASE + 0x984)
    #define REG_MIX_SIGN_AUTO_OFF Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_MIX1_SIGN_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_MIX2_SIGN_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define ALPHA_MIX_GRP2_5 Fld(1,21,AC_MSKB2)//[21:21]
    #define ALPHA_MIX_GRP2_1 Fld(1,17,AC_MSKB2)//[17:17]
    #define ALPHA_MIX_GRP2_0 Fld(1,16,AC_MSKB2)//[16:16]
    #define ALPHA_MIX_GRP12_5 Fld(1,7,AC_MSKB0)//[7:7]
    #define ALPHA_MIX_GRP12_1 Fld(1,6,AC_MSKB0)//[6:6]
    #define ALPHA_MIX_GRP12_0 Fld(1,5,AC_MSKB0)//[5:5]
    #define ALPHA_MIX_GRP1_5 Fld(1,4,AC_MSKB0)//[4:4]
    #define ALPHA_MIX_GRP1_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define ALPHA_MIX_GRP1_0 Fld(1,2,AC_MSKB0)//[2:2]
    #define ALPHA_MIX_GRP0_1 Fld(1,1,AC_MSKB0)//[1:1]
#define OSD_MIX_1 (IO_POSTPROC_BASE + 0x988)
    #define REG_MIX2_BOT_ALPHA Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_MIX12_BOT_ALPHA Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_MIX11_BOT_ALPHA Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_MIX2_SEL_BOT_ALPHA Fld(1,2,AC_MSKB0)//[2:2]
    #define REG_MIX12_SEL_BOT_ALPHA Fld(1,1,AC_MSKB0)//[1:1]
    #define REG_MIX11_SEL_BOT_ALPHA Fld(1,0,AC_MSKB0)//[0:0]



#define XVYCC_00 (IO_POSTPROC_BASE + 0xC00)
    #define REG_3X3_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_BYPASS_AG Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_BYPASS_G Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_BYPASS_XVYCC_MAIN Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_BYPASS_MATRIX Fld(1,27,AC_MSKB3)//[27:27]
    #define REG_XVYCC_IN1 Fld(1,25,AC_MSKB3)//[25:25]
    #define REG_XVYCC_M_P_SW Fld(1,24,AC_MSKB3)//[24:24]
    #define REG_BYPASS_XV_IN2OUT Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_BYPASS_XVYCC_PIP Fld(1,11,AC_MSKB1)//[11:11]
#define XVYCC_05 (IO_POSTPROC_BASE + 0xC14)
    #define REG_XV_MATRIX00 Fld(15,16,AC_MSKW32)//[30:16]
#define XVYCC_06 (IO_POSTPROC_BASE + 0xC18)
    #define REG_XV_MATRIX02 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX01 Fld(15,0,AC_MSKW10)//[14:0]
#define XVYCC_07 (IO_POSTPROC_BASE + 0xC1C)
    #define REG_XV_MATRIX11 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX10 Fld(15,0,AC_MSKW10)//[14:0]
#define XVYCC_08 (IO_POSTPROC_BASE + 0xC20)
    #define REG_XV_MATRIX20 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX12 Fld(15,0,AC_MSKW10)//[14:0]
#define XVYCC_09 (IO_POSTPROC_BASE + 0xC24)
    #define REG_XV_MATRIX22 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX21 Fld(15,0,AC_MSKW10)//[14:0]
#define XVYCC_10 (IO_POSTPROC_BASE + 0xC28)
    #define REG_GAMMA_G_SRAM_4096 Fld(12,16,AC_MSKW32)//[27:16]
    #define REG_GAMMA_R_SRAM_4096 Fld(12,0,AC_MSKW10)//[11:0]
#define XVYCC_11 (IO_POSTPROC_BASE + 0xC2C)
    #define REG_GAMMA_B_SRAM_4096 Fld(12,0,AC_MSKW10)//[11:0]

#define ARM_INTR1 (0xF0036008)
    #define MJC_INTR_EN_1 Fld(4, 24, AC_MSKB3) //27:24
#define ARM_INTR2 (0xF0008034)
    #define ARM_INTR_EN_1 Fld(1,10,AC_MSKB1)//[10:10]
    #define ARM_INTR_EN_2 Fld(1,9,AC_MSKB1)//[9:9]

#define AMBL_00_ES2 (IO_POSTPROC_BASE + 0xB00)
    #define R_AMBI_INTR_MASK_MODE Fld(2,12,AC_MSKB1)//[13:12]
    #define R_AMBI_SHIFT_N Fld(4,8,AC_MSKB1)//[11:8]
    #define R_AMBI_START_BLKNUM Fld(5,3,AC_MSKB0)//[7:3]
    #define AMBI_SW_EN Fld(1,0,AC_MSKB0)//[0:0]    
#define AMBL_01_ES2 (IO_POSTPROC_BASE + 0xB04)
    #define R_AMBI_BLK_H_LAST Fld(8,24,AC_FULLB3)//[31:24]
    #define R_AMBI_BLK_W_LAST Fld(8,16,AC_FULLB2)//[23:16]
    #define R_AMBI_BLK_H Fld(8,8,AC_FULLB1)//[15:8]
    #define R_AMBI_BLK_W Fld(8,0,AC_FULLB0)//[7:0]
#define AMBL_02_ES2 (IO_POSTPROC_BASE + 0xB08)
    #define AMBI_RESULT_R Fld(16,16,AC_FULLW32)//[31:16]
    #define AMBI_INTR_DEBUG Fld(1,10,AC_MSKB1)//[10:10]
    #define AMBI_BLKNUM Fld(5,5,AC_MSKW10)//[9:5]
    #define AMBI_STRAP_ID Fld(5,0,AC_MSKB0)//[4:0]
#define AMBL_03_ES2 (IO_POSTPROC_BASE + 0xB0C)
    #define AMBI_RESULT_B Fld(16,16,AC_FULLW32)//[31:16]
    #define AMBI_RESULT_G Fld(16,0,AC_FULLW10)//[15:0]
    


#define OSTG_PTGEN_00 (IO_POSTPROC_BASE + 0x9C8)
    #define FLD_CSSEL Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_VSYNC_DELSEL2 Fld(3,23,AC_MSKW32)//[25:23]
    #define FLD_VSYNC_DELSEL Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_DEN_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_VS_SEL Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_REG_VSYNC_OUT_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_REG_VSYNC_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_REG_PTGEN_HV_MODE Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_VCOUNT_ENABLE Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HCOUNT_ENABLE Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_VSYNCP Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HSYNCP Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_DENP Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DENTPY Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PATGEN_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_PATGEN_RSEL Fld(3,1,AC_MSKB0)//[3:1]
    #define FLD_PATGEN_EN Fld(1,0,AC_MSKB0)//[0:0]
#define OSTG_PTGEN_01 (IO_POSTPROC_BASE + 0x9CC)
    #define REG_VSYNC_OUT_H Fld(16,16,AC_FULLW32)//[31:16]
    #define OSTG_PTGEN_HS_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define OSTG_PTGEN_02 (IO_POSTPROC_BASE + 0x9D0)
    #define OSTG_PTGEN_V_POS Fld(16,16,AC_FULLW32)//[31:16]
    #define OSTG_PTGEN_H_POS Fld(16,0,AC_FULLW10)//[15:0]
#define OSTG_PTGEN_03 (IO_POSTPROC_BASE + 0x9D4)
    #define OSTG_PTGEN_VS_WIDTH Fld(4,0,AC_MSKB0)//[3:0]
//<= Copy from Cobra : Not Verified by R2
#endif

