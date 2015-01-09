/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: hw_ttd.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _HW_TD_NAVI_H_
#define _HW_TD_NAVI_H_

#define IO_PRE_PROC_BASE (VDOIN_BASE + 0x3000)

#define TDNVAI_00 (IO_PRE_PROC_BASE + 0x400)
    #define SIMILAR_THR Fld(5,26,AC_MSKB3)//[30:26]
    #define VAR_THR Fld(5,21,AC_MSKW32)//[25:21]
    #define BLK_X_MUL_BLK_Y Fld(13,8,AC_MSKW21)//[20:8]
    #define PXL_DIFF_THR Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_01 (IO_PRE_PROC_BASE + 0x404)
    #define BLK_SIZE_X Fld(8,24,AC_FULLB3)//[31:24]
    #define AMBIG_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define D_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define VALID_BLK_THR Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_02 (IO_PRE_PROC_BASE + 0x408)
    #define ADAP_THR_SHIFT Fld(3,24,AC_MSKB3)//[26:24]
    #define BLK_SIZE_Y Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_03 (IO_PRE_PROC_BASE + 0x40C)
    #define SSTB_INK_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define DECISION_TYPE Fld(2,29,AC_MSKB3)//[30:29]
    #define FW_DBG Fld(1,28,AC_MSKB3)//[28:28]
    #define TDNAVI_DBG Fld(3,25,AC_MSKB3)//[27:25]
    #define ADAP_THR_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define BLK_Y_ST Fld(12,12,AC_MSKW21)//[23:12]
    #define BLK_X_ST Fld(12,0,AC_MSKW10)//[11:0]
#define TDNVAI_04 (IO_PRE_PROC_BASE + 0x410)
    #define NEIGHBOR_CNT_THR Fld(6,24,AC_MSKB3)//[29:24]
    #define NEIGHBOR_DIFF_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define FW_DBG_INK_C Fld(8,8,AC_FULLB1)//[15:8]
    #define FW_DBG_INK_Y Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_05 (IO_PRE_PROC_BASE + 0x414)
    #define TDNAVI_DET_HEIGHT Fld(12,20,AC_MSKW32)//[31:20]
    #define TDNAVI_DET_WIDTH Fld(12,8,AC_MSKW21)//[19:8]
    #define GDV_BLK_THR Fld(6,2,AC_MSKB0)//[7:2]
    #define GDV_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define GDV2_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TDNVAI_06 (IO_PRE_PROC_BASE + 0x418)
    #define DECISION_TYPE_EX Fld(3,22,AC_MSKW32)//[24:22]
    #define GDV_SBS_MAX Fld(3,19,AC_MSKB2)//[21:19]
    #define GDV_TAB_MAX Fld(4,15,AC_MSKW21)//[18:15]
    #define LK_VLD_DIFF_THR Fld(4,11,AC_MSKB1)//[14:11]
    #define LK_GAIN Fld(5,6,AC_MSKW10)//[10:6]
    #define LK_ADAP_DIFF_THR Fld(3,3,AC_MSKB0)//[5:3]
    #define LK_ADAP_VLD_THR Fld(3,0,AC_MSKB0)//[2:0]
#define TDNVAI_07 (IO_PRE_PROC_BASE + 0x41C)
    #define H_MID_BLK Fld(2,27,AC_MSKB3)//[28:27]
    #define W_MID_BLK Fld(3,24,AC_MSKB3)//[26:24]
    #define HEIGHT_ST Fld(12,12,AC_MSKW21)//[23:12]
    #define WIDTH_ST Fld(12,0,AC_MSKW10)//[11:0]
#define TDNVAI_08 (IO_PRE_PROC_BASE + 0x420)
    #define WIN_HEIGHT Fld(9,22,AC_MSKW32)//[30:22]
    #define WIN_WIDTH Fld(10,12,AC_MSKW21)//[21:12]
    #define TD_LB_THR Fld(4,8,AC_MSKB1)//[11:8]
    #define LI_VER_THR1 Fld(4,4,AC_MSKB0)//[7:4]
    #define LI_VER_THR2 Fld(4,0,AC_MSKB0)//[3:0]
#define TDNVAI_09 (IO_PRE_PROC_BASE + 0x424)
    #define CI_HOR_THR Fld(4,20,AC_MSKB2)//[23:20]
    #define CI_DIFF_GAP_THR Fld(4,16,AC_MSKB2)//[19:16]
    #define LI_CNT_THR Fld(4,12,AC_MSKB1)//[15:12]
    #define CI_CNT_THR Fld(4,8,AC_MSKB1)//[11:8]
    #define LI_STA_MODE Fld(4,4,AC_MSKB0)//[7:4]
    #define CI_STA_MODE Fld(4,0,AC_MSKB0)//[3:0]
#define TDNVAI_0A (IO_PRE_PROC_BASE + 0x428)
    #define LI_FCR_HI Fld(5,26,AC_MSKB3)//[30:26]
    #define LI_FCR_LO Fld(5,21,AC_MSKW32)//[25:21]
    #define CI_FCR_HI Fld(5,16,AC_MSKB2)//[20:16]
    #define CI_FCR_LO Fld(5,11,AC_MSKB1)//[15:11]
    #define TD_NAVI_INK Fld(3,8,AC_MSKB1)//[10:8]
    #define FW_DBG_INK_CR Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_0B (IO_PRE_PROC_BASE + 0x42C)
    #define TDNAVI_RESERVED Fld(22,10,AC_MSKDW)//[31:10]
    #define FW_ADDRESS Fld(9,1,AC_MSKW10)//[9:1]
    #define FW_READ_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TDNVAI_0C (IO_PRE_PROC_BASE + 0x430)
    #define CB_CNT_THR Fld(4,26,AC_MSKB3)//[29:26]
    #define QC_CNT_THR Fld(4,22,AC_MSKW32)//[25:22]
    #define CB_CORI_THR Fld(4,18,AC_MSKB2)//[21:18]
    #define CB_DIFF_GAP_THR Fld(4,14,AC_MSKW21)//[17:14]
    #define QC_VDIFF_MIN_THR Fld(5,9,AC_MSKB1)//[13:9]
    #define QC_VDIFF_GAIN_THR Fld(4,5,AC_MSKW10)//[8:5]
    #define QC_DIFF_GAP_THR Fld(4,1,AC_MSKB0)//[4:1]
    #define QC_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define TDNVAI_0D (IO_PRE_PROC_BASE + 0x434)
    #define CB_FCR_HI Fld(5,21,AC_MSKW32)//[25:21]
    #define CB_FCR_LO Fld(5,16,AC_MSKB2)//[20:16]
    #define QC_FCR_HI Fld(5,11,AC_MSKB1)//[15:11]
    #define QC_FCR_LO Fld(5,6,AC_MSKW10)//[10:6]
    #define CB_STA_MODE Fld(4,2,AC_MSKB0)//[5:2]
    #define QC_STA_MODE Fld(2,0,AC_MSKB0)//[1:0]
#define TDNVAI_STA0 (IO_PRE_PROC_BASE + 0x43C)
    #define STA_TAB_VLD_BLK Fld(8,22,AC_MSKW32)//[29:22]
    #define STA_SBS_VLD_BLK Fld(8,14,AC_MSKW21)//[21:14]
    #define STA_BLK_APL Fld(8,0,AC_FULLB0)//[7:0]

    
#define TDNVAI_STA1 (IO_PRE_PROC_BASE + 0x440)
    #define STA_TAB_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_SBS_CNT Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_INVLD_TAB Fld(8,8,AC_FULLB1)//[15:8]
    #define STA_INVLD_SBS Fld(8,0,AC_FULLB0)//[7:0]

    #define TDNVAI_FPGA_02_BLK_Y Fld(8,8,AC_FULLB1)//[15:8]    
    #define TDNVAI_FPGA_02_BLK_PIXEL_DIFF Fld(8,0,AC_FULLB0)//[7:0]
    
#define TDNVAI_STA2 (IO_PRE_PROC_BASE + 0x444)
    #define STA_GDV_S1 Fld(4,25,AC_MSKB3)//[28:25]
    #define STA_GDV_S0 Fld(4,21,AC_MSKW32)//[24:21]
    #define STA_TAB_NEIGHBOR_DIFF_CNT Fld(9,12,AC_MSKW21)//[20:12]
    #define STA_SBS_NEIGHBOR_DIFF_CNT Fld(9,3,AC_MSKW10)//[11:3]
    #define STA_DET_TYPE Fld(3,0,AC_MSKB0)//[2:0]
#define TDNVAI_STA3 (IO_PRE_PROC_BASE + 0x448)
    #define STA_GDV2_T1 Fld(5,23,AC_MSKW32)//[27:23]
    #define STA_GDV2_T0 Fld(5,18,AC_MSKB2)//[22:18]
    #define STA_GDV_T1 Fld(5,13,AC_MSKW21)//[17:13]
    #define STA_GDV_T0 Fld(5,8,AC_MSKB1)//[12:8]
    #define STA_GDV2_S1 Fld(4,4,AC_MSKB0)//[7:4]
    #define STA_GDV2_S0 Fld(4,0,AC_MSKB0)//[3:0]
#define TDNVAI_STA4 (IO_PRE_PROC_BASE + 0x44C)
    #define STA_LI_CNT_MEAN Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_CI_CNT_MEAN Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_LI_CNT_MAX Fld(8,8,AC_FULLB1)//[15:8]
    #define STA_CI_CNT_MAX Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_STA5 (IO_PRE_PROC_BASE + 0x450)
    #define STA_CB_IDX Fld(4,27,AC_MSKB3)//[30:27]
    #define STA_QC_IDX Fld(4,23,AC_MSKW32)//[26:23]
    #define STA_CB_FCR_MAX Fld(5,18,AC_MSKB2)//[22:18]
    #define STA_QC_FCR Fld(5,13,AC_MSKW21)//[17:13]
    #define STA_LI_FCR_MAX Fld(5,8,AC_MSKB1)//[12:8]
    #define STA_CI_FCR_MAX Fld(5,3,AC_MSKB0)//[7:3]
    #define STA_FMT Fld(3,0,AC_MSKB0)//[2:0]
#define TDNVAI_STA6 (IO_PRE_PROC_BASE + 0x454)
    #define STA_QC_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_CB_CNT_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_CB_CNT_MEAN Fld(8,8,AC_FULLB1)//[15:8]
    #define STA_CB_FCR_MEAN Fld(5,0,AC_MSKB0)//[4:0]
#define TDNVAI_STA7 (IO_PRE_PROC_BASE + 0x458)
    #define STA_LI_IDX Fld(4,14,AC_MSKW21)//[17:14]
    #define STA_CI_IDX Fld(4,10,AC_MSKB1)//[13:10]
    #define STA_LI_FCR_MEAN Fld(5,5,AC_MSKW10)//[9:5]
    #define STA_CI_FCR_MEAN Fld(5,0,AC_MSKB0)//[4:0]
#define TDNAVI_ATPG (IO_PRE_PROC_BASE + 0x45C)
    #define PRE_TDNAV_MAIN_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_TDNAV_MAIN_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]
   
//Page PRE_3D22D_MAIN
#define PRE_3D22D_MAIN_00 (IO_PRE_PROC_BASE + 0x110)
    #define R_UIIMGWIDTH_M Fld(12,20,AC_MSKW32)//[31:20]
    #define R_UIIMGHEIGHT_M Fld(12,8,AC_MSKW21)//[19:8]
    #define R_B3D22D_EN_M Fld(1,7,AC_MSKB0)//[7:7]
    #define R_BMASKSEL_M Fld(1,4,AC_MSKB0)//[4:4]
    #define PRE_3D22D_DBUF_M Fld(1,2,AC_MSKB0)//[2:2]
    #define R_UI3DMODESEL_M Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_3D22D_MAIN_01 (IO_PRE_PROC_BASE + 0x114)
    #define R_UIINKWIDTH_M Fld(12,20,AC_MSKW32)//[31:20]
    #define R_UIINKHEIGHT_M Fld(12,8,AC_MSKW21)//[19:8]
    #define PRE_3D22D_MAIN_ATPG_CT Fld(1,6,AC_MSKB0)//[6:6]
    #define PRE_3D22D_MAIN_ATPG_OB Fld(1,5,AC_MSKB0)//[5:5]
    #define R_INKEN_M Fld(1,4,AC_MSKB0)//[4:4]
    #define R_3D22D_SET_RES_CLEAR_O_M Fld(1,3,AC_MSKB0)//[3:3]
    #define R_3D22D_SET_RES_FINISHED_O_M Fld(1,2,AC_MSKB0)//[2:2]
    #define R_3D22D_SET_RES_CLEAR_I_M Fld(1,1,AC_MSKB0)//[1:1]
    #define R_3D22D_SET_RES_FINISHED_I_M Fld(1,0,AC_MSKB0)//[0:0]

//Page PRE_3D22D_PIP
#define PRE_3D22D_PIP_00 (IO_PRE_PROC_BASE + 0x310)
    #define R_UIIMGWIDTH_P Fld(12,20,AC_MSKW32)//[31:20]
    #define R_UIIMGHEIGHT_P Fld(12,8,AC_MSKW21)//[19:8]
    #define R_B3D22D_EN_P Fld(1,7,AC_MSKB0)//[7:7]
    #define R_BMASKSEL_P Fld(1,4,AC_MSKB0)//[4:4]
    #define PRE_3D22D_DBUF_P Fld(1,2,AC_MSKB0)//[2:2]
    #define R_UI3DMODESEL_P Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_3D22D_PIP_01 (IO_PRE_PROC_BASE + 0x314)
    #define R_UIINKWIDTH_P Fld(12,20,AC_MSKW32)//[31:20]
    #define R_UIINKHEIGHT_P Fld(12,8,AC_MSKW21)//[19:8]
    #define PRE_3D22D_PIP_ATPG_CT Fld(1,6,AC_MSKB0)//[6:6]
    #define PRE_3D22D_PIP_ATPG_OB Fld(1,5,AC_MSKB0)//[5:5]
    #define R_INKEN_P Fld(1,4,AC_MSKB0)//[4:4]
    #define R_3D22D_SET_RES_CLEAR_O_P Fld(1,3,AC_MSKB0)//[3:3]
    #define R_3D22D_SET_RES_FINISHED_O_P Fld(1,2,AC_MSKB0)//[2:2]
    #define R_3D22D_SET_RES_CLEAR_I_P Fld(1,1,AC_MSKB0)//[1:1]
    #define R_3D22D_SET_RES_FINISHED_I_P Fld(1,0,AC_MSKB0)//[0:0]

//Page PRE_REALD_MAIN
#define PRE_REALD_MAIN_00 (IO_PRE_PROC_BASE + 0x0F0)
    #define R_UIRGBTYPE_00_M Fld(2,30,AC_MSKB3)//[31:30]
    #define R_UIRGBTYPE_01_M Fld(2,28,AC_MSKB3)//[29:28]
    #define R_UIRGBTYPE_02_M Fld(2,26,AC_MSKB3)//[27:26]
    #define R_UIRGBTYPE_03_M Fld(2,24,AC_MSKB3)//[25:24]
    #define R_UIRGBTYPE_04_M Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIRGBTYPE_05_M Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIRGBTYPE_06_M Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIRGBTYPE_07_M Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIRGBTYPE_08_M Fld(2,14,AC_MSKB1)//[15:14]
    #define R_UIRGBTYPE_09_M Fld(2,12,AC_MSKB1)//[13:12]
    #define R_UIRGBTYPE_10_M Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIRGBTYPE_11_M Fld(2,8,AC_MSKB1)//[9:8]
    #define R_UIRGBTYPE_12_M Fld(2,6,AC_MSKB0)//[7:6]
    #define R_UIRGBTYPE_13_M Fld(2,4,AC_MSKB0)//[5:4]
    #define R_UIRGBTYPE_14_M Fld(2,2,AC_MSKB0)//[3:2]
    #define R_UIRGBTYPE_15_M Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_MAIN_01 (IO_PRE_PROC_BASE + 0x0F4)
    #define R_UIRGBTYPE_16_M Fld(2,30,AC_MSKB3)//[31:30]
    #define R_UIRGBTYPE_17_M Fld(2,28,AC_MSKB3)//[29:28]
    #define R_UIRGBTYPE_18_M Fld(2,26,AC_MSKB3)//[27:26]
    #define R_UIRGBTYPE_19_M Fld(2,24,AC_MSKB3)//[25:24]
    #define R_UIRGBTYPE_20_M Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIRGBTYPE_21_M Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIRGBTYPE_22_M Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIRGBTYPE_23_M Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIRGBTYPE_24_M Fld(2,14,AC_MSKB1)//[15:14]
    #define R_UIRGBTYPE_25_M Fld(2,12,AC_MSKB1)//[13:12]
    #define R_UIRGBTYPE_26_M Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIRGBTYPE_27_M Fld(2,8,AC_MSKB1)//[9:8]
    #define R_UIRGBTYPE_28_M Fld(2,6,AC_MSKB0)//[7:6]
    #define R_UIRGBTYPE_29_M Fld(2,4,AC_MSKB0)//[5:4]
    #define R_UIRGBTYPE_30_M Fld(2,2,AC_MSKB0)//[3:2]
    #define R_UIRGBTYPE_31_M Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_MAIN_02 (IO_PRE_PROC_BASE + 0x0F8)
    #define R_UIRGBTYPE_32_M Fld(2,30,AC_MSKB3)//[31:30]
    #define R_UIRGBTYPE_33_M Fld(2,28,AC_MSKB3)//[29:28]
    #define R_UIRGBTYPE_34_M Fld(2,26,AC_MSKB3)//[27:26]
    #define R_UIRGBTYPE_35_M Fld(2,24,AC_MSKB3)//[25:24]
    #define R_UIRGBTYPE_36_M Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIRGBTYPE_37_M Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIRGBTYPE_38_M Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIRGBTYPE_39_M Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIRGBTYPE_40_M Fld(2,14,AC_MSKB1)//[15:14]
    #define R_UIRGBTYPE_41_M Fld(2,12,AC_MSKB1)//[13:12]
    #define R_UIRGBTYPE_42_M Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIRGBTYPE_43_M Fld(2,8,AC_MSKB1)//[9:8]
    #define R_UIRGBTYPE_44_M Fld(2,6,AC_MSKB0)//[7:6]
    #define R_UIRGBTYPE_45_M Fld(2,4,AC_MSKB0)//[5:4]
    #define R_UIRGBTYPE_46_M Fld(2,2,AC_MSKB0)//[3:2]
    #define R_UIRGBTYPE_47_M Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_MAIN_03 (IO_PRE_PROC_BASE + 0x0FC)
    #define R_UIRGBTYPE_48_M Fld(2,30,AC_MSKB3)//[31:30]
    #define R_UIRGBTYPE_49_M Fld(2,28,AC_MSKB3)//[29:28]
    #define R_UIRGBTYPE_50_M Fld(2,26,AC_MSKB3)//[27:26]
    #define R_UIRGBTYPE_51_M Fld(2,24,AC_MSKB3)//[25:24]
    #define R_UIRGBTYPE_52_M Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIRGBTYPE_53_M Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIRGBTYPE_54_M Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIRGBTYPE_55_M Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIRGBTYPE_56_M Fld(2,14,AC_MSKB1)//[15:14]
    #define R_UIRGBTYPE_57_M Fld(2,12,AC_MSKB1)//[13:12]
    #define R_UIRGBTYPE_58_M Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIRGBTYPE_59_M Fld(2,8,AC_MSKB1)//[9:8]
    #define R_UIINSRCMODE_M Fld(2,6,AC_MSKB0)//[7:6]
#define PRE_REALD_MAIN_04 (IO_PRE_PROC_BASE + 0x100)
    #define R_UIDEQUITHL_M Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIQUITHL_M Fld(8,16,AC_FULLB2)//[23:16]
    #define PRE_REALD_MAIN_ATPG_CT Fld(1,15,AC_MSKB1)//[15:15]
    #define PRE_REALD_MAIN_ATPG_OB Fld(1,14,AC_MSKB1)//[14:14]
    #define R_BIN2D3DMODE_M Fld(1,8,AC_MSKB1)//[8:8]
    #define R_UIMASKLINE_M Fld(3,4,AC_MSKB0)//[6:4]
    #define R_UIMASKENABLE_M Fld(1,0,AC_MSKB0)//[0:0]
#define STA_REALD_MAIN_00 (IO_PRE_PROC_BASE + 0x104)
    #define FRAME_QUA_STA_M Fld(12,0,AC_MSKW10)//[11:0]

//Page PRE_REALD_PIP
#define PRE_REALD_PIP_00 (IO_PRE_PROC_BASE + 0x2F0)
    #define R_UIRGBTYPE_00_P Fld(2,30,AC_MSKB3)//[31:30]
    #define R_UIRGBTYPE_01_P Fld(2,28,AC_MSKB3)//[29:28]
    #define R_UIRGBTYPE_02_P Fld(2,26,AC_MSKB3)//[27:26]
    #define R_UIRGBTYPE_03_P Fld(2,24,AC_MSKB3)//[25:24]
    #define R_UIRGBTYPE_04_P Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIRGBTYPE_05_P Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIRGBTYPE_06_P Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIRGBTYPE_07_P Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIRGBTYPE_08_P Fld(2,14,AC_MSKB1)//[15:14]
    #define R_UIRGBTYPE_09_P Fld(2,12,AC_MSKB1)//[13:12]
    #define R_UIRGBTYPE_10_P Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIRGBTYPE_11_P Fld(2,8,AC_MSKB1)//[9:8]
    #define R_UIRGBTYPE_12_P Fld(2,6,AC_MSKB0)//[7:6]
    #define R_UIRGBTYPE_13_P Fld(2,4,AC_MSKB0)//[5:4]
    #define R_UIRGBTYPE_14_P Fld(2,2,AC_MSKB0)//[3:2]
    #define R_UIRGBTYPE_15_P Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_PIP_01 (IO_PRE_PROC_BASE + 0x2F4)
    #define R_UIRGBTYPE_16_P Fld(2,30,AC_MSKB3)//[31:30]
    #define R_UIRGBTYPE_17_P Fld(2,28,AC_MSKB3)//[29:28]
    #define R_UIRGBTYPE_18_P Fld(2,26,AC_MSKB3)//[27:26]
    #define R_UIRGBTYPE_19_P Fld(2,24,AC_MSKB3)//[25:24]
    #define R_UIRGBTYPE_20_P Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIRGBTYPE_21_P Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIRGBTYPE_22_P Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIRGBTYPE_23_P Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIRGBTYPE_24_P Fld(2,14,AC_MSKB1)//[15:14]
    #define R_UIRGBTYPE_25_P Fld(2,12,AC_MSKB1)//[13:12]
    #define R_UIRGBTYPE_26_P Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIRGBTYPE_27_P Fld(2,8,AC_MSKB1)//[9:8]
    #define R_UIRGBTYPE_28_P Fld(2,6,AC_MSKB0)//[7:6]
    #define R_UIRGBTYPE_29_P Fld(2,4,AC_MSKB0)//[5:4]
    #define R_UIRGBTYPE_30_P Fld(2,2,AC_MSKB0)//[3:2]
    #define R_UIRGBTYPE_31_P Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_PIP_02 (IO_PRE_PROC_BASE + 0x2F8)
    #define R_UIRGBTYPE_32_P Fld(2,30,AC_MSKB3)//[31:30]
    #define R_UIRGBTYPE_33_P Fld(2,28,AC_MSKB3)//[29:28]
    #define R_UIRGBTYPE_34_P Fld(2,26,AC_MSKB3)//[27:26]
    #define R_UIRGBTYPE_35_P Fld(2,24,AC_MSKB3)//[25:24]
    #define R_UIRGBTYPE_36_P Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIRGBTYPE_37_P Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIRGBTYPE_38_P Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIRGBTYPE_39_P Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIRGBTYPE_40_P Fld(2,14,AC_MSKB1)//[15:14]
    #define R_UIRGBTYPE_41_P Fld(2,12,AC_MSKB1)//[13:12]
    #define R_UIRGBTYPE_42_P Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIRGBTYPE_43_P Fld(2,8,AC_MSKB1)//[9:8]
    #define R_UIRGBTYPE_44_P Fld(2,6,AC_MSKB0)//[7:6]
    #define R_UIRGBTYPE_45_P Fld(2,4,AC_MSKB0)//[5:4]
    #define R_UIRGBTYPE_46_P Fld(2,2,AC_MSKB0)//[3:2]
    #define R_UIRGBTYPE_47_P Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_PIP_03 (IO_PRE_PROC_BASE + 0x2FC)
    #define R_UIRGBTYPE_48_P Fld(2,30,AC_MSKB3)//[31:30]
    #define R_UIRGBTYPE_49_P Fld(2,28,AC_MSKB3)//[29:28]
    #define R_UIRGBTYPE_50_P Fld(2,26,AC_MSKB3)//[27:26]
    #define R_UIRGBTYPE_51_P Fld(2,24,AC_MSKB3)//[25:24]
    #define R_UIRGBTYPE_52_P Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIRGBTYPE_53_P Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIRGBTYPE_54_P Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIRGBTYPE_55_P Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIRGBTYPE_56_P Fld(2,14,AC_MSKB1)//[15:14]
    #define R_UIRGBTYPE_57_P Fld(2,12,AC_MSKB1)//[13:12]
    #define R_UIRGBTYPE_58_P Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIRGBTYPE_59_P Fld(2,8,AC_MSKB1)//[9:8]
    #define R_UIINSRCMODE_P Fld(2,6,AC_MSKB0)//[7:6]
#define PRE_REALD_PIP_04 (IO_PRE_PROC_BASE + 0x300)
    #define R_UIDEQUITHL_P Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIQUITHL_P Fld(8,16,AC_FULLB2)//[23:16]
    #define PRE_REALD_PIP_ATPG_CT Fld(1,15,AC_MSKB1)//[15:15]
    #define PRE_REALD_PIP_ATPG_OB Fld(1,14,AC_MSKB1)//[14:14]
    #define R_BIN2D3DMODE_P Fld(1,8,AC_MSKB1)//[8:8]
    #define R_UIMASKLINE_P Fld(3,4,AC_MSKB0)//[6:4]
    #define R_UIMASKENABLE_P Fld(1,0,AC_MSKB0)//[0:0]
#define STA_REALD_PIP_00 (IO_PRE_PROC_BASE + 0x304)
    #define FRAME_QUA_STA_P Fld(12,0,AC_MSKW10)//[11:0]

//Page PRE_DQ_MAIN
#define DQ_00 (IO_PRE_PROC_BASE + 0x4A0)
    #define DQ_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define DQ_LUMA_X2_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define DQ_CHROMA_X2_MODE Fld(1,29,AC_MSKB3)//[29:29]
    #define DQ_444_422_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define FS_SS_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define SRAM_PSCAN_FULL Fld(1,25,AC_MSKB3)//[25:25]
    #define SRAM_PSCAN_HALF Fld(1,24,AC_MSKB3)//[24:24]
    #define SRAM_NR_FULL Fld(1,23,AC_MSKB2)//[23:23]
    #define SETRES_DQ_SET Fld(1,5,AC_MSKB0)//[5:5]
    #define SETRES_FINISH_OUT Fld(1,4,AC_MSKB0)//[4:4]
    #define SETRES_FINISH_IN Fld(1,3,AC_MSKB0)//[3:3]
    #define SETRES_CLEAN_OUT Fld(1,2,AC_MSKB0)//[2:2]
    #define SETRES_CLEAN_IN Fld(1,1,AC_MSKB0)//[1:1]
    #define DQ_DBUF Fld(1,0,AC_MSKB0)//[0:0]
#define DQ_01 (IO_PRE_PROC_BASE + 0x4A4)
    #define DQ_X2_EVENPHASE_COEF0 Fld(9,23,AC_MSKW32)//[31:23]
    #define DQ_X2_EVENPHASE_COEF1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DQ_X2_EVENPHASE_COEF2 Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_02 (IO_PRE_PROC_BASE + 0x4A8)
    #define DQ_X2_EVENPHASE_COEF3 Fld(12,20,AC_MSKW32)//[31:20]
    #define DQ_X2_EVENPHASE_COEF4 Fld(11,9,AC_MSKW21)//[19:9]
    #define DQ_X2_EVENPHASE_COEF5 Fld(9,0,AC_MSKW10)//[8:0]
#define DQ_03 (IO_PRE_PROC_BASE + 0x4AC)
    #define DQ_X2_ODDPHASE_COEF0 Fld(9,23,AC_MSKW32)//[31:23]
    #define DQ_X2_ODDPHASE_COEF1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DQ_X2_ODDPHASE_COEF2 Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_04 (IO_PRE_PROC_BASE + 0x4B0)
    #define DQ_X2_ODDPHASE_COEF3 Fld(12,20,AC_MSKW32)//[31:20]
    #define DQ_X2_ODDPHASE_COEF4 Fld(11,9,AC_MSKW21)//[19:9]
    #define DQ_X2_ODDPHASE_COEF5 Fld(9,0,AC_MSKW10)//[8:0]
#define DQ_05 (IO_PRE_PROC_BASE + 0x4B4)
    #define DQ_X1_EVENPHASE_COEF0 Fld(9,23,AC_MSKW32)//[31:23]
    #define DQ_X1_EVENPHASE_COEF1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DQ_X1_EVENPHASE_COEF2 Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_06 (IO_PRE_PROC_BASE + 0x4B8)
    #define DQ_X1_EVENPHASE_COEF3 Fld(12,20,AC_MSKW32)//[31:20]
    #define DQ_X1_EVENPHASE_COEF4 Fld(11,9,AC_MSKW21)//[19:9]
    #define DQ_X1_EVENPHASE_COEF5 Fld(9,0,AC_MSKW10)//[8:0]
#define DQ_07 (IO_PRE_PROC_BASE + 0x4BC)
    #define DQ_X1_ODDPHASE_COEF0 Fld(9,23,AC_MSKW32)//[31:23]
    #define DQ_X1_ODDPHASE_COEF1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DQ_X1_ODDPHASE_COEF2 Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_08 (IO_PRE_PROC_BASE + 0x4C0)
    #define DQ_X1_ODDPHASE_COEF3 Fld(12,20,AC_MSKW32)//[31:20]
    #define DQ_X1_ODDPHASE_COEF4 Fld(11,9,AC_MSKW21)//[19:9]
    #define DQ_X1_ODDPHASE_COEF5 Fld(9,0,AC_MSKW10)//[8:0]
#define DQ_09 (IO_PRE_PROC_BASE + 0x4C4)
    #define SRCIMG_HEIGHT Fld(12,12,AC_MSKW21)//[23:12]
    #define SRCIMG_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_10 (IO_PRE_PROC_BASE + 0x4C8)
    #define PRE_DQ_MAIN_ATPG_CT Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_DQ_MAIN_ATPG_OB Fld(1,0,AC_MSKB0)//[0:0]

// CB
#define CB_00 (IO_PRE_PROC_BASE + 0x480)
    #define CB_PACK Fld(1,14,AC_MSKB1)//[13:13]
    #define CB_VI Fld(1,13,AC_MSKB1)//[13:13]
    #define CB_BYPASS Fld(1,12,AC_MSKB1)//[12:12]
    #define CB_SRCIMG_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define CB_01 (IO_PRE_PROC_BASE + 0x484)
    #define CB_SETRES_CLEAN_IN Fld(1,6,AC_MSKB0)//[6:6]
    #define CB_SETRES_CLEAN_OUT Fld(1,5,AC_MSKB0)//[5:5]
    #define CB_SETRES_FINISH_IN Fld(1,4,AC_MSKB0)//[4:4]
    #define CB_SETRES_FINISH_OUT Fld(1,3,AC_MSKB0)//[3:3]
    #define CB_DBUF Fld(1,2,AC_MSKB0)//[2:2]
    #define PRE_CB_MAIN_ATPG_CT Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_CB_MAIN_ATPG_OB Fld(1,0,AC_MSKB0)//[0:0]


#endif // #ifndef _HW_TD_NAVI_H_     
