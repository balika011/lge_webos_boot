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
 * $RCSfile: b2r_reg_v1.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file b2r_reg_v1.h
 *  This header file declares register map of b2r version 1.
 */


#ifndef _B2R_REG_V2_H_
#define _B2R_REG_V2_H_

/*
hd and sd share the register defines from 0 to 0x60
*/

#define B2R_HD_BASE 0x0
#define B2R_SD_BASE 0x100

#define B2R_YS (0x0000)
    #define ADDR_START Fld(27,4,AC_MSKDW)//[30:4]
#define B2R_CS (0x0004)
    //#define CS Fld(27,4,AC_MSKDW)//[30:4]
#define B2R_R_MAP (0x0008)
    #define START_REQ_MODE Fld(2,28,AC_MSKB3)//[29:28]
    #define NEW_EVEN_REPEAT Fld(1,26,AC_MSKB3)//[26:26]
    #define REPEAT_H_SHIFT Fld(1,25,AC_MSKB3)//[25:25]
    #define REPEAT_FOREVER Fld(1,24,AC_MSKB3)//[24:24]
    #define REPEAT_CNT Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAP_C Fld(5,8,AC_MSKB1)//[12:8]
    #define R_MAP_Y Fld(5,0,AC_MSKB0)//[4:0]
#define B2R_HV_PORCH (0x000C)
    #define TAG_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define TAG_LINE_NUM Fld(1,29,AC_MSKB3)//[29:29]
    #define TAG_POSITION Fld(1,28,AC_MSKB3)//[28:28]
    #define UNEQUAL_HDE Fld(1,19,AC_MSKB2)//[19:19]
    #define FIELD_DISP_R_CHANGE Fld(1,18,AC_MSKB2)//[18:18]
    #define VSYNC_RISING Fld(1,17,AC_MSKB2)//[17:17]
    #define VSYNC_FALLING Fld(1,16,AC_MSKB2)//[16:16]
    #define H_PORCH_MODE Fld(1,12,AC_MSKB1)//[12:12]
    #define H_PORCH Fld(12,0,AC_MSKW10)//[11:0]
#define B2R_TP_GRID (0x0010)
    #define GRID_Y_TGL Fld(1,31,AC_MSKB3)//[31:31]
    #define GRID_Y_ON_E Fld(1,30,AC_MSKB3)//[30:30]
    #define GRID_Y_ON_M Fld(1,29,AC_MSKB3)//[29:29]
    #define GRID_Y_ON_B Fld(1,28,AC_MSKB3)//[28:28]
    #define GRID_Y_TIME_INC Fld(4,24,AC_MSKB3)//[27:24]
    #define GRID_Y_TIME_MODE Fld(2,22,AC_MSKB2)//[23:22]
    #define GRID_Y_SIZE Fld(6,16,AC_MSKB2)//[21:16]
    #define GRID_X_TGL Fld(1,15,AC_MSKB1)//[15:15]
    #define GRID_X_ON_E Fld(1,14,AC_MSKB1)//[14:14]
    #define GRID_X_ON_M Fld(1,13,AC_MSKB1)//[13:13]
    #define GRID_X_ON_B Fld(1,12,AC_MSKB1)//[12:12]
    #define GRID_X_TIME_INC Fld(4,8,AC_MSKB1)//[11:8]
    #define GRID_X_TIME_MODE Fld(2,6,AC_MSKB0)//[7:6]
    #define GRID_X_SIZE Fld(6,0,AC_MSKB0)//[5:0]
#define B2R_TP_COLOR (0x0014)
    #define V_PORCH_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define V_UP_PORCH Fld(11,20,AC_MSKW32)//[30:20]
    #define COLOR_TIME_MODE Fld(2,18,AC_MSKB2)//[19:18]
    #define COLOR_TIME_STEP Fld(2,16,AC_MSKB2)//[17:16]
    #define COLOR_XY_MODE Fld(2,14,AC_MSKB1)//[15:14]
    #define COLOR_XY_STEP Fld(2,12,AC_MSKB1)//[13:12]
    #define COLOR_Y_INC Fld(4,8,AC_MSKB1)//[11:8]
    #define COLOR_CB_INC Fld(4,4,AC_MSKB0)//[7:4]
    #define COLOR_CR_INC Fld(4,0,AC_MSKB0)//[3:0]
#define B2R_TP_ON (0x0018)
    #define TP_3D Fld(1,28,AC_MSKB3)//[28:28]
    #define TP_GRID_OP Fld(3,25,AC_MSKB3)//[27:25]
    #define TP_ON Fld(1,24,AC_MSKB3)//[24:24]
    #define TP_Y Fld(8,16,AC_FULLB2)//[23:16]
    #define TP_CB Fld(8,8,AC_FULLB1)//[15:8]
    #define TP_CR Fld(8,0,AC_FULLB0)//[7:0]
    #define TP_YCBCR Fld(24,0,AC_MSKDW)//[23:0]
#define B2R_ACTIVE (0x001C)
    #define VDEW Fld(12,20,AC_MSKW32)//[31:20]  //CC_MT5399
    #define SET_RESOLUTION Fld(1,16,AC_MSKB2)//[16:16]
    #define HDEW Fld(12,0,AC_MSKW10)//[11:0]     //CC_MT5399
#define B2R_HV_TOTAL (0x0020)
    #define FLD_F_DLY Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_R_DLY Fld(2,28,AC_MSKB3)//[29:28]
    #define V_TOTAL Fld(12,16,AC_MSKW32)//[27:16]   //CC_MT5399
    #define H_TOTAL Fld(13,0,AC_MSKW10)//[12:0]
#define B2R_HDE_RATIO (0x0024)
    #define INT_ADJ8 Fld(1,24,AC_MSKB3)//[24:24]
    #define HDE_RATIO Fld(10,8,AC_MSKW21)//[17:8]
    #define INT_ADJ Fld(8,0,AC_FULLB0)//[7:0]
#define B2R_LINE_OFFSET (0x0028)
    #define LINE_OFFSET_H Fld(1,28,AC_MSKB3)//[28:28]   //CC_MT5399
    #define REQ_WAIT_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define REQ_WAIT_CYCLE Fld(8,16,AC_FULLB2)//[23:16]
    #define FIFO_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define LINE_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define B2R_MODE_CTRL (0x002C)
    #define AUTO_HV_OFFSET Fld(1,31,AC_MSKB3)//[31:31]
    #define DISP_R_VIEW_MODE Fld(2,29,AC_MSKB3)//[30:29]
    #define DISP_R_VIEW Fld(1,28,AC_MSKB3)//[28:28]
    #define DISP_R_VIEW_INV Fld(1,27,AC_MSKB3)//[27:27]
    #define TWO_ADDR_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define HD_VS_INV Fld(1,25,AC_MSKB3)//[25:25]
    #define HD_HS_INV Fld(1,24,AC_MSKB3)//[24:24]
    #define BURST_READ_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define ADDR_SWAP Fld(3,20,AC_MSKB2)//[22:20]
    #define INTERLACE_3D_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define FIELD_INV Fld(1,18,AC_MSKB2)//[18:18]
    #define DIS_PROTECT_BURST Fld(1,17,AC_MSKB2)//[17:17]
    #define DIS_MIR_FLP_BURST Fld(1,16,AC_MSKB2)//[16:16]
    #define FIFO_MON_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define C_420I Fld(1,14,AC_MSKB1)//[14:14]
    #define TOP_VT_DEC Fld(1,13,AC_MSKB1)//[13:13]
    #define OUT_I Fld(1,12,AC_MSKB1)//[12:12]
    #define M422 Fld(1,11,AC_MSKB1)//[11:11]
    #define TOP_VT_DEC_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define MIRROR_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLIP_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define FIELD_SEL Fld(1,7,AC_MSKB0)//[7:7]
    #define LEFT_VT_DEC_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define LEFT_VT_DEC Fld(1,5,AC_MSKB0)//[5:5]
    #define BTM_FST Fld(1,4,AC_MSKB0)//[4:4]
    #define REQR_TH Fld(2,0,AC_MSKB0)//[1:0]
#define B2R_TRIGGER (0x0030)
    #define SET_REPEAT_CNT Fld(1,3,AC_MSKB0)//[3:3]
    #define TGEN_RST Fld(1,2,AC_MSKB0)//[2:2]
    #define SET_FIELD Fld(1,0,AC_MSKB0)//[0:0]
#define B2R_CTRL (0x0034)
    #define LINE_PULSE Fld(1,23,AC_MSKB2)//[23:23]
    #define LAST_ROW Fld(7,16,AC_MSKB2)//[22:16]
    #define C_COMPACT Fld(1,15,AC_MSKB1)//[15:15]
    #define Y_COMPACT Fld(1,14,AC_MSKB1)//[14:14]
    #define LAST_P_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FIRST_BR_INT_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define BR_VX2 Fld(1,9,AC_MSKB1)//[9:9]
    #define CC_BR_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define BR_MAX_V Fld(7,0,AC_MSKB0)//[6:0]
#define B2R_BREN (0x0038)
    #define B4K_MODE Fld(1,31,AC_MSKB3) //[31:31]
    #define PACK_MODE Fld(3,28,AC_MSKB3)//[30:28]
    #define HD_VB1_MODE Fld(1,27,AC_MSKB3)//[30:28]
    #define PACK_EN   Fld(1,26,AC_MSKB3)//[26:26]
    #define BR_WRAP_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define BR_PULSE_EN Fld(1,0,AC_MSKB0)//[0:0]
#define B2R_STA (0x003C)
    #define VCNT Fld(11,16,AC_MSKW32)//[26:16]
    #define REPEAT_NOW Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_ADJI Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_VEI Fld(1,13,AC_MSKB1)//[13:13]
    #define DISP_R_ADJI Fld(1,12,AC_MSKB1)//[12:12]
    #define LR_ADJI Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_NOW Fld(1,10,AC_MSKB1)//[10:10]
    #define BR_INT Fld(1,9,AC_MSKB1)//[9:9]
    #define ADJ_INT Fld(1,8,AC_MSKB1)//[8:8]
    #define VE_INT Fld(1,7,AC_MSKB0)//[7:7]
    #define TB_ADJI Fld(1,6,AC_MSKB0)//[6:6]
    #define TB_VEI Fld(1,5,AC_MSKB0)//[5:5]
    #define UDFL Fld(1,3,AC_MSKB0)//[3:3]
    #define TB_NOW Fld(1,2,AC_MSKB0)//[2:2]
    #define VS_NOW Fld(1,1,AC_MSKB0)//[1:1]
    #define HS_NOW Fld(1,0,AC_MSKB0)//[0:0]
#define B2R_EN (0x0040)
    #define VEI_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define ADJI_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DBF_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define PATH_EN Fld(1,0,AC_MSKB0)//[0:0]
#define B2R_OFFSET (0x0048)
    #define V_OFFSET Fld(10,17,AC_MSKW32)//[26:17]
    #define H_OFFSET Fld(10,2,AC_MSKW10)//[11:2]
#define B2R_YS_R_VIEW (0x0050)
    //#define YS_R_VIEW Fld(27,4,AC_MSKDW)//[30:4] //refer to ADDR_START
#define B2R_CS_R_VIEW (0x0054)
    //#define CS_R_VIEW Fld(27,4,AC_MSKDW)//[30:4]
#define B2R_DE_CTRL (0x0058)
    #define DE_START Fld(13,16,AC_MSKW32)//[27:16]
    #define DE_END Fld(13,0,AC_MSKW10)//[0:0]
#define B2R_TRACK_MB_LINE (0x005C)
    #define TRACK_MBL Fld(8,0,AC_FULLB0)//[7:0]
#define B2R_UFO_CFG (0x0060)
    #define UFO_MODE Fld(2,26,AC_MSKB3)//[27:26]
#define B2R_YS_LEN (0x0064)
    #define ADDR_START_LEN Fld(28,4,AC_MSKDW)//[31:4]
#define B2R_CS_LEN (0x0068)
#define B2R_YS_R_LEN (0x006C)
#define B2R_CS_R_LEN (0x0070)

#define B2R_MISC (0x0300)
    #define SD_C_RR_CACHE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define SD_C_RR_FB_FMT Fld(1,30,AC_MSKB3)//[30:30]
    #define SD_Y_RR_CACHE_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define SD_Y_RR_FB_FMT Fld(1,28,AC_MSKB3)//[28:28]
    #define HD_C_RR_CACHE_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define HD_C_RR_FB_FMT Fld(1,26,AC_MSKB3)//[26:26]
    #define HD_Y_RR_CACHE_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define HD_Y_RR_FB_FMT Fld(1,24,AC_MSKB3)//[24:24]
    #define RR_BYPASS Fld(1,16,AC_MSKB2)//[16:16]
    #define SD_C_RR_TYPE Fld(1,15,AC_MSKB1)//[15:15]
    #define SD_C_RR_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define SD_Y_RR_TYPE Fld(1,13,AC_MSKB1)//[13:13]
    #define SD_Y_RR_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define HD_C_RR_TYPE Fld(1,11,AC_MSKB1)//[11:11]
    #define HD_C_RR_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define SD_DRAM_RST Fld(1,9,AC_MSKB1)//[9:9]
    #define HD_DRAM_RST Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_CK_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define SRAM_ALWAYS_ON Fld(1,6,AC_MSKB0)//[6:6]
    //#define DRAM_RST Fld(1,5,AC_MSKB0)//[5:5]
    #define VCXO_RST Fld(1,4,AC_MSKB0)//[4:4]
    #define SD_SM_PCR_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SD_RST Fld(1,2,AC_MSKB0)//[2:2]
    #define HD_RST Fld(1,1,AC_MSKB0)//[1:1]
    #define HD_SM_PCR_EN Fld(1,0,AC_MSKB0)//[0:0]
#define B2R_HD_PCR_ERR (0x0304)
    #define PCR_ERR_SIGN Fld(1,8,AC_MSKB1)//[8:8]
    #define PCR_ERR_ABS Fld(8,0,AC_FULLB0)//[7:0]
#define B2R_HD_IDEAL_CK (0x0308)
    #define PLL_N Fld(10,16,AC_MSKW32)//[25:16]
    #define PLL_M Fld(10,0,AC_MSKW10)//[9:0]
#define B2R_INT_CLR (0x030C)
    #define SD_PCR_RSTR Fld(1,16,AC_MSKB2)//[16:16]
    #define FBIST_SET_RANDOM_7 Fld(1,15,AC_MSKB1)//[15:15]
    #define FBIST_SET_RANDOM_6 Fld(1,14,AC_MSKB1)//[14:14]
    #define FBIST_SET_RANDOM_5 Fld(1,13,AC_MSKB1)//[13:13]
    #define FBIST_SET_RANDOM_4 Fld(1,12,AC_MSKB1)//[12:12]
    #define FBIST_SET_RANDOM_3 Fld(1,11,AC_MSKB1)//[11:11]
    #define FBIST_SET_RANDOM_2 Fld(1,10,AC_MSKB1)//[10:10]
    #define FBIST_SET_RANDOM_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define FBIST_SET_RANDOM_0 Fld(1,8,AC_MSKB1)//[8:8]
    #define SD_SET_RES_FINISHED_CLR Fld(1,7,AC_MSKB0)//[7:7]
    #define HD_SET_RES_FINISHED_CLR Fld(1,6,AC_MSKB0)//[6:6]
    #define SD_INT_CLR Fld(1,5,AC_MSKB0)//[5:5]
    #define HD_INT_CLR Fld(1,4,AC_MSKB0)//[4:4]
    #define FBIST_TRG_STA Fld(1,3,AC_MSKB0)//[3:3]
    #define YC_CRC_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define DRAM_CHKSUM_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define HD_PCR_RSTR Fld(1,0,AC_MSKB0)//[0:0]
#define B2R_HD_PIXEL_CRC (0x0310)
    #define PXL_CRC Fld(32,0,AC_FULLDW)//[31:0]
#define B2R_DRAM_CHKSUM (0x0314)
    #define DRAM_CHKSUM Fld(32,0,AC_FULLDW)//[31:0]
#define B2R_SD_PIXEL_CRC (0x0318)
    //#define SD_PXL_CRC Fld(32,0,AC_FULLDW)//[31:0]    //refer to PXL_CRC
#define B2R_WAVELET_PIXEL_CRC (0x0320)
    #define HD_WAVELET_PXL_CRC Fld(32,0,AC_FULLDW)//[31:0]
#define B2R_PCR_PATH_SEL (0x0324)
    #define MAIN_USE_SD_PATH Fld(1,2,AC_MSKB0)//[2:2]
    #define HD_NO_PCR_TGEN Fld(1,1,AC_MSKB0)//[1:1]
    #define SD_NO_PCR_TGEN Fld(1,0,AC_MSKB0)//[0:0]
#define B2R_SD_PCR_ERR (0x0328)
    //#define SD_PCR_ERR_SIGN Fld(1,8,AC_MSKB1)//[8:8] //refer to PCR_ERR_SIGN
    //#define SD_PCR_ERR_ABS Fld(8,0,AC_FULLB0)//[7:0]
#define B2R_SD_IDEAL_CK (0x032C)
    //#define SD_PLL_N Fld(10,16,AC_MSKW32)//[25:16] //refer to PLL_N
    //#define SD_PLL_M Fld(10,0,AC_MSKW10)//[9:0]
#define B2R_SRC_SEL (0x0064)
    #define BYPS_PATH_UV_SWAP Fld(1,13,AC_MSKB1)//[13:13]
    #define SMART_PCR1_SEL Fld(3,9,AC_MSKB1)//[11:9]
    #define VB1_PATH_SRC_SEL Fld(1,8,AC_MSKB1)//[8:8]
    #define SMART_PCR0_SEL Fld(3,5,AC_MSKB0)//[7:5]
    #define DDDS_B2R_SRC_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define B2R_SUB_PATH_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define B2R_MAIN_PATH_SEL Fld(2,0,AC_MSKB0)//[1:0]

#define B2R_DATA_MUX (IO_VIRT + 0x26840)
    #define NSD_CLK_SEL Fld(1,13,AC_MSKB1)//[0:0]
    #define NHD_CLK_SEL Fld(1,12,AC_MSKB1)//[0:0]
    #define SUB_PATH_SEL Fld(2,10,AC_MSKB1)//[1:0]
    #define MAIN_PATH_SEL Fld(2,8,AC_MSKB1)//[3:2]
    #define PATH_RST Fld(1,7,AC_MSKB0)//[3:2]

#endif /*_B2R_REG_V2_H_*/

