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

#ifndef _HW_TTD_H_
#define _HW_TTD_H_

#if defined(CC_MT5882)
#define IO_TTD_BASE (IO_VIRT +0x5D000)
#else
#define IO_TTD_BASE (IO_VIRT +0x36000)  //MT5399 base address
#endif

#define TTD_00 (IO_TTD_BASE + 0xC00)
    #define TTD_FSAMPLE_ENABLE Fld(1,8,AC_MSKB1)//[8:8]
    #define TTD_CONVERGENCE_DEPTH Fld(8,0,AC_FULLB0)//[7:0]
#define TTD_01 (IO_TTD_BASE + 0xC04)
    #define TTD_OUT_LR_DLY Fld(2,15,AC_MSKW21)//[16:15]
    #define TTD_DIBR_LR_INV Fld(1,8,AC_MSKB1)//[8:8]
    #define TTD_OUT_LR_INV Fld(1,6,AC_MSKB0)//[6:6]
    #define TTD_LLRR_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_MIRROW Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_02 (IO_TTD_BASE + 0xC08)
    #define TTD_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define MJC_PRE_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define LR_FREERUN_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define YUV_444_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define SRC_3D_TYPE Fld(2,26,AC_MSKB3)//[27:26]
    #define TTD_SRC_2D_NRPT Fld(1,25,AC_MSKB3)//[25:25]
    #define TTD_SRC_3D_MODE Fld(1,24,AC_MSKB3)//[24:24]
    #define TTD_IMG_HEIGHT Fld(12,12,AC_MSKW21)//[23:12]
    #define TTD_IMG_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define TTD_03 (IO_TTD_BASE + 0xC0C)
    #define TTD_DBG_FORCE_LR Fld(1,29,AC_MSKB3)//[29:29]
    #define TTD_DBG_FORCE_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define WIDTH_8X_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SRC_2D_RPT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_04 (IO_TTD_BASE + 0xC10)
    #define TTD_LM_UV_BASE Fld(8,12,AC_MSKW21)//[19:12]
    #define TTD_LM_V_WEIGHT Fld(4,8,AC_MSKB1)//[11:8]
    #define TTD_LM_U_WEIGHT Fld(4,4,AC_MSKB0)//[7:4]
    #define TTD_3DC_DECREASE_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define TTD_DS4_V_MODE Fld(1,2,AC_MSKB0)//[2:2]
    #define TTD_DS4_H_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define TTD_3DC_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_05 (IO_TTD_BASE + 0xC14)
    #define STA_DMAP_ERR_OVER Fld(1,5,AC_MSKB0)//[5:5]
    #define DMAP_ERR_CLEAN Fld(1,4,AC_MSKB0)//[4:4]
    #define TTD_DBG_SEL Fld(4,0,AC_MSKB0)//[3:0]

#define TTDPO_00 (IO_TTD_BASE + 0xC18)
    #define STA_IN_DISPR Fld(1,17,AC_MSKB2)//[17:17]
    #define POST_IN_LR_INV Fld(1,16,AC_MSKB2)//[16:16]
    #define POST_OUT_LR_INV Fld(1,15,AC_MSKB1)//[15:15]
    #define POST_DEBUG_SEL Fld(4,10,AC_MSKB1)//[13:10]
    #define POST_HS_NUM Fld(8,2,AC_MSKW10)//[9:2]
    #define POST_TTD_BYPASS Fld(1,1,AC_MSKB0)//[1:1]
    #define POST_TTD_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTDPO_01 (IO_TTD_BASE + 0xC1C)
    #define SYNC_DELAY_CYCLES Fld(9,22,AC_MSKW32)//[30:22]
    #define POST_VSYNC_MJC Fld(4,18,AC_MSKB2)//[21:18]
    #define POST_HS_BNK_WIDTH Fld(8,10,AC_MSKW21)//[17:10]
    #define POST_HS_TOTAL_WIDTH Fld(10,0,AC_MSKW10)//[9:0]
#define TTD_RESERVED1 (IO_TTD_BASE + 0xC3C)
    #define TTD_RESERVED Fld(32,0,AC_FULLDW)//[31:0]
#define TTDDM_00 (IO_TTD_BASE + 0xDB0)
    #define CID_FRM_SIZE Fld(16,16,AC_FULLW32)//[31:16]
    #define DSO_FRM_SIZE Fld(16,0,AC_FULLW10)//[15:0]
#define TTDDM_01 (IO_TTD_BASE + 0xDB4)
    #define TTD_DSO_STR_ADR_L Fld(24,8,AC_MSKDW)//[31:8]
    #define CID_FRM_WT_NUM Fld(4,4,AC_MSKB0)//[7:4]
    #define DSO_FRM_WT_NUM Fld(4,0,AC_MSKB0)//[3:0]
#define TTDDM_02 (IO_TTD_BASE + 0xDB8)
    #define STA_DS_FRM_PTR Fld(4,27,AC_MSKB3)//[30:27]
    #define DRAM_DS_ERR_OVERFLOW_CLR Fld(1,26,AC_MSKB3)//[26:26]
    #define STA_DRAM_DS_ERR_OVERFLOW Fld(1,25,AC_MSKB3)//[25:25]
    #define TTD_DRAM_WT_DSO_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TTD_DSO_STR_ADR_R Fld(24,0,AC_MSKDW)//[23:0]
#define TTDDM_03 (IO_TTD_BASE + 0xDBC)
    #define STA_CID_FRM_PTR Fld(4,27,AC_MSKB3)//[30:27]
    #define DRAM_CID_ERR_OVERFLOW_CLR Fld(1,26,AC_MSKB3)//[26:26]
    #define STA_DRAM_CID_ERR_OVERFLOW Fld(1,25,AC_MSKB3)//[25:25]
    #define TTD_DRAM_WT_CID_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TTD_CID_STR_ADR_L Fld(24,0,AC_MSKDW)//[23:0]
#define TTDDM_04 (IO_TTD_BASE + 0xDC0)
    #define TTD_CID_STR_ADR_R Fld(24,0,AC_MSKDW)//[23:0]
#define TTDDM_05 (IO_TTD_BASE + 0xDC4)
    #define TTD_DRAM_ADR_LOW Fld(24,0,AC_MSKDW)//[23:0]
#define TTDDM_06 (IO_TTD_BASE + 0xDC8)
    #define TTD_DRAM_ADR_HIGH Fld(24,0,AC_MSKDW)//[23:0]
#define TTDDM_07 (IO_TTD_BASE + 0xDCC)
    #define FW_DMA_READ_TRIG Fld(1,26,AC_MSKB3)//[26:26]
    #define HW_AUTO_DMA_READ_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define TTD_DRAM_RD_MV_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TTD_MV_RD_STR_ADR Fld(24,0,AC_MSKDW)//[23:0]
#define TTDDM_08 (IO_TTD_BASE + 0xDD0)
    #define MV_HW_MODE Fld(1,25,AC_MSKB3)//[25:25]
    #define TTD_DRAM_RD_CID_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TTD_CID_RD_STR_ADR_L Fld(24,0,AC_MSKDW)//[23:0]
#define TTDDM_09 (IO_TTD_BASE + 0xDD4)
    #define TTD_DRAM_ADDR_MSB Fld(1,26,AC_MSKB3)//[26:26]
    #define TTD_CID_RD_STR_ADR_R Fld(24,0,AC_MSKDW)//[23:0]

//Page TTDWRAP
//#if defined(CC_MT5399)
#define TTDWRAP_00 (IO_TTD_BASE + 0x7D0)
    #define C_RST_CTRL Fld(4,28,AC_MSKB3)//[31:28]
    #define C_CLK_TDDC_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define C_CLK_TTD_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define STA_TTD_INT Fld(4,16,AC_MSKB2)//[19:16]
    #define C_TTD_INT_POL_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define C_TTD_INT_CLEAR Fld(4,8,AC_MSKB1)//[11:8]
    #define C_TTD_INT_MASK Fld(4,4,AC_MSKB0)//[7:4]
    #define C_TTD_CLR_MODE Fld(1,3,AC_MSKB0)//[3:3]
#define TTDWRAP_LARB0 (IO_TTD_BASE + 0x7D4)
    #define C_TDDC_ADDR_MSB Fld(4,28,AC_MSKB3)//[31:28]
    #define C_LARB_TDDC_DMAGBMBEG Fld(1,15,AC_MSKB1)//[15:15]
    #define C_LARB_TDDC_DMAGBMEND Fld(1,14,AC_MSKB1)//[14:14]
    #define C_LARB_TDDC_DMAGBMAGID Fld(3,11,AC_MSKB1)//[13:11]
    #define C_LARB_TDDC_DMAGBMALL Fld(1,10,AC_MSKB1)//[10:10]
    #define C_LARB_TDDC_DMTIMARBEN Fld(1,9,AC_MSKB1)//[9:9]
    #define C_LARB_TDDC_DMARBRST Fld(1,8,AC_MSKB1)//[8:8]
#define TTDWRAP_LARB1 (IO_TTD_BASE + 0x7D8)
    #define C_LARB_TDDC_DMAG6EN Fld(1,6,AC_MSKB0)//[6:6]
    #define C_LARB_TDDC_DMAG5EN Fld(1,5,AC_MSKB0)//[5:5]
    #define C_LARB_TDDC_DMAG4EN Fld(1,4,AC_MSKB0)//[4:4]
    #define C_LARB_TDDC_DMAG3EN Fld(1,3,AC_MSKB0)//[3:3]
    #define C_LARB_TDDC_DMAG2EN Fld(1,2,AC_MSKB0)//[2:2]
    #define C_LARB_TDDC_DMAG1EN Fld(1,1,AC_MSKB0)//[1:1]
    #define C_LARB_TDDC_DMAG0EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTDWRAP_LARB2 (IO_TTD_BASE + 0x7DC)
    #define C_LARB_TDDC_DMAG6TIM Fld(4,24,AC_MSKB3)//[27:24]
    #define C_LARB_TDDC_DMAG5TIM Fld(4,20,AC_MSKB2)//[23:20]
    #define C_LARB_TDDC_DMAG4TIM Fld(4,16,AC_MSKB2)//[19:16]
    #define C_LARB_TDDC_DMAG3TIM Fld(4,12,AC_MSKB1)//[15:12]
    #define C_LARB_TDDC_DMAG2TIM Fld(4,8,AC_MSKB1)//[11:8]
    #define C_LARB_TDDC_DMAG1TIM Fld(4,4,AC_MSKB0)//[7:4]
    #define C_LARB_TDDC_DMAG0TIM Fld(4,0,AC_MSKB0)//[3:0]
#define TTDWRAP_LARB3 (IO_TTD_BASE + 0x7E0)
    #define STA_LARB_TDDC_REQUEST_QUEUE_EMPTY Fld(1,0,AC_MSKB0)//[0:0]
#define TTDWRAP_LARB4 (IO_TTD_BASE + 0x7E4)
    #define STA_LARB_TDDC_BM_LENGTH_COUNTER_CHA Fld(32,0,AC_FULLDW)//[31:0]
#define TTDWRAP_LARB5 (IO_TTD_BASE + 0x7E8)
    #define STA_LARB_TDDC_BM_LENGTH_COUNTER_CHB Fld(32,0,AC_FULLDW)//[31:0]
#define TTDWRAP_LARB6 (IO_TTD_BASE + 0x7EC)
    #define STA_LARB_TDDC_BM_CYCLE_COUNTER_CHA Fld(32,0,AC_FULLDW)//[31:0]
#define TTDWRAP_LARB7 (IO_TTD_BASE + 0x7F0)
    #define STA_LARB_TDDC_BM_CYCLE_COUNTER_CHB Fld(32,0,AC_FULLDW)//[31:0]
#define TTDWRAP_RESERVED (IO_TTD_BASE + 0x7F4)
    #define C_TTD_RESERVED Fld(32,0,AC_FULLDW)//[31:0]
#define TTDWRAP_ATPG (IO_TTD_BASE + 0x7FC)
    #define TTDWRAP_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define TTDWRAP_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]
//#endif    

#define TTDVP_00 (IO_TTD_BASE + 0xC40)
    #define CID_TEXTURE_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define CID_CHROMA_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define CID_Y_WEIGHT Fld(8,8,AC_FULLB1)//[15:8]
    #define CID_C_WEIGHT Fld(8,0,AC_FULLB0)//[7:0]
#define TTDVP_01 (IO_TTD_BASE + 0xC44)
    #define CID_TEXTURE_THR Fld(8,24,AC_FULLB3)//[31:24]
    #define CID_EDGE_BASE Fld(8,16,AC_FULLB2)//[23:16]
    #define CID_UV_BASE Fld(8,8,AC_FULLB1)//[15:8]
    #define CID_Y_BASE Fld(8,0,AC_FULLB0)//[7:0]
#define TTDVP_02 (IO_TTD_BASE + 0xC48)
    #define CID_TEXTURE_SAT Fld(8,24,AC_FULLB3)//[31:24]
    #define CID_TEXTURE_RATIO Fld(3,21,AC_MSKB2)//[23:21]
    #define CID_OFFSET Fld(8,3,AC_MSKW10)//[10:3]
    #define CID_UV_INV_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define CID_NONLINEAR Fld(1,1,AC_MSKB0)//[1:1]
    #define CID_TDDC_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTDVP_03 (IO_TTD_BASE + 0xC4C)
    #define VP_TRANSITION_STAT Fld(6,1,AC_MSKB0)//[6:1]
    #define VP_TRANSITION_CTRL_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTDVP_04 (IO_TTD_BASE + 0xC50)
    #define VP_TRANSIENT_STEP Fld(6,24,AC_MSKB3)//[29:24]
#define TTDVP_05 (IO_TTD_BASE + 0xC54)
    #define CID_INVERSE_FLAG Fld(1,24,AC_MSKB3)//[24:24]
#define TTDVP_06 (IO_TTD_BASE + 0xC58)
    #define VP_VFLIP_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define VP_EDGE_THR Fld(11,10,AC_MSKW21)//[20:10]
#define TTDVP_07 (IO_TTD_BASE + 0xC5C)
    #define VP_LAST_TILE_PXL_WEIGHT Fld(4,26,AC_MSKB3)//[29:26]
    #define LOCAL_INV_SUCC_COUNT_THR Fld(4,22,AC_MSKW32)//[25:22]
#define TTDVP_08 (IO_TTD_BASE + 0xC60)
    #define GM_HORI_SMOOTH_WEIGHT Fld(6,0,AC_MSKB0)//[5:0]
#define TTDVP_09 (IO_TTD_BASE + 0xC64)
    #define GM_VH_SMOOTH_WEIGHT Fld(11,21,AC_MSKW32)//[31:21]
    #define GM_VERT_MODEL_GAIN Fld(9,12,AC_MSKW21)//[20:12]
    #define GM_VERT_MAX_GAIN Fld(6,6,AC_MSKW10)//[11:6]
    #define GM_MINUS_VERT_MIN_GAIN Fld(6,0,AC_MSKB0)//[5:0]
#define TTDVP_10 (IO_TTD_BASE + 0xC68)
    #define GM_VERT_SMOOTH_WEIGHT Fld(6,26,AC_MSKB3)//[31:26]
    #define GM_VERT_CENTER Fld(11,4,AC_MSKW10)//[14:4]
    #define GM_VERT_GAIN_SIGN Fld(1,3,AC_MSKB0)//[3:3]
    #define GM_VERT_CTRL_EN Fld(1,2,AC_MSKB0)//[2:2]
#define TTDVP_12 (IO_TTD_BASE + 0xC70)
    #define GM_VERT_GAIN Fld(9,23,AC_MSKW32)//[31:23]
    #define GM_VERT_GAIN2 Fld(9,14,AC_MSKW21)//[22:14]
    #define GM_VERT_SLOPE Fld(6,8,AC_MSKB1)//[13:8]
    #define GM_VERT_SLOPE_SIGN Fld(1,7,AC_MSKB0)//[7:7]
#define TTDVP_13 (IO_TTD_BASE + 0xC74)
    #define GM_VERT_OFFSET Fld(9,23,AC_MSKW32)//[31:23]
    #define GM_VERT_GAIN2_SIGN Fld(1,12,AC_MSKB1)//[12:12]
    #define GM_WEIGHT_CTRL_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define GM_HORI_WEIGHT Fld(11,0,AC_MSKW10)//[10:0]
#define TTDVP_14 (IO_TTD_BASE + 0xC78)
    #define GM_VERT_WEIGHT Fld(11,21,AC_MSKW32)//[31:21]
    #define GM_BLOCK_HEIGHT Fld(4,16,AC_MSKB2)//[19:16]
    #define GM_BLOCK_WIDTH Fld(5,11,AC_MSKB1)//[15:11]
    #define GM_STD_SCALAR Fld(5,6,AC_MSKW10)//[10:6]
    #define GM_STD_SHIFTER Fld(2,4,AC_MSKB0)//[5:4]
#define TTDVP_16 (IO_TTD_BASE + 0xC80)
    #define GM_MAX_HIST_DIST Fld(9,23,AC_MSKW32)//[31:23]
#define TTDVP_17 (IO_TTD_BASE + 0xC84)
    #define GM_HORI_CTRL_PT_3 Fld(8,24,AC_FULLB3)//[31:24]
    #define GM_HORI_CTRL_PT_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define GM_HORI_CTRL_PT_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define GM_HORI_CTRL_PT_0 Fld(8,0,AC_FULLB0)//[7:0]
#define TTDVP_18 (IO_TTD_BASE + 0xC88)
    #define GM_HORI_CTRL_PT_7 Fld(8,24,AC_FULLB3)//[31:24]
    #define GM_HORI_CTRL_PT_6 Fld(8,16,AC_FULLB2)//[23:16]
    #define GM_HORI_CTRL_PT_5 Fld(8,8,AC_FULLB1)//[15:8]
    #define GM_HORI_CTRL_PT_4 Fld(8,0,AC_FULLB0)//[7:0]
#define TTDVP_19 (IO_TTD_BASE + 0xC8C)
    #define GM_HORI_CTRL_PT_8 Fld(8,24,AC_FULLB3)//[31:24]
    #define GM_HORI_CTRL_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define GM_HORI_CTRL_PT_TOP Fld(8,15,AC_MSKW21)//[22:15]
    #define GM_HORI_CTRL_PT_BTM Fld(8,7,AC_MSKW10)//[14:7]
#define TTDVP_20 (IO_TTD_BASE + 0xC90)
    #define CID_V_DOWN2 Fld(1,1,AC_MSKB0)//[1:1]
    #define CID_H_DOWN2 Fld(1,0,AC_MSKB0)//[0:0]
#define STA_TTDVP_16 (IO_TTD_BASE + 0xCD0)
    #define STA_GM_VERT_GAIN Fld(10,22,AC_MSKW32)//[31:22]
#define STA_TTDVP_17 (IO_TTD_BASE + 0xCD4)
    #define STA_GM_VERT_GAIN2 Fld(10,22,AC_MSKW32)//[31:22]
#define STA_TTDVP_18 (IO_TTD_BASE + 0xCD8)
    #define STA_GM_HIST_DIST_SUM Fld(13,16,AC_MSKW32)//[28:16]
    #define STA_GM_MAX_HIST_DIST Fld(13,0,AC_MSKW10)//[12:0]
#define STA_TTDVP_19 (IO_TTD_BASE + 0xCDC)
    #define STA_FINAL_INVERSE_FLAG Fld(1,31,AC_MSKB3)//[31:31]
#define STA_TTDVP_20 (IO_TTD_BASE + 0xCE0)
    #define STA_GM_HORI_CTRL_PT_0 Fld(8,0,AC_FULLB0)//[7:0]
#define STA_TTDVP_21 (IO_TTD_BASE + 0xCE4)
    #define STA_GM_HORI_CTRL_PT_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_GM_HORI_CTRL_PT_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_GM_HORI_CTRL_PT_3 Fld(8,8,AC_FULLB1)//[15:8]
    #define STA_GM_HORI_CTRL_PT_4 Fld(8,0,AC_FULLB0)//[7:0]
#define STA_TTDVP_22 (IO_TTD_BASE + 0xCE8)
    #define STA_GM_HORI_CTRL_PT_5 Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_GM_HORI_CTRL_PT_6 Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_GM_HORI_CTRL_PT_7 Fld(8,8,AC_FULLB1)//[15:8]
    #define STA_GM_HORI_CTRL_PT_8 Fld(8,0,AC_FULLB0)//[7:0]
#define STA_TTDVP_23 (IO_TTD_BASE + 0xCF0)
    #define STA_GM_HORI_EDGE_GRAD_SUM Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_GM_VERT_EDGE_GRAD_SUM Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_24 (IO_TTD_BASE + 0xCF4)
    #define STA_GM_VERT_EDGE_CNT_0 Fld(15,17,AC_MSKW32)//[31:17]
    #define STA_GM_VERT_EDGE_CNT_1 Fld(15,2,AC_MSKDW)//[16:2]
#define STA_TTDVP_25 (IO_TTD_BASE + 0xCF8)
    #define STA_GM_VERT_EDGE_CNT_2 Fld(15,17,AC_MSKW32)//[31:17]
#define TTDVP_RESERVED (IO_TTD_BASE + 0xCFC)

#define TTDDG_00 (IO_TTD_BASE + 0xD00)
    #define DG_ROI_BASE Fld(8,23,AC_MSKW32)//[30:23]
    #define DG_INTP_V_MODE Fld(2,21,AC_MSKB2)//[22:21]
    #define DG_INTP_H_MODE Fld(2,19,AC_MSKB2)//[20:19]
    #define BND_PADDING_MODE Fld(1,18,AC_MSKB2)//[18:18]
    #define DG_TFU_INV_STAT Fld(5,13,AC_MSKW21)//[17:13]
    #define DG_HIST_WINDOW_SIZE Fld(4,9,AC_MSKB1)//[12:9]
    #define DG_TRANSIENT_STEP Fld(6,3,AC_MSKW10)//[8:3]
    #define ADA_TFU_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define ADA_LOCAL_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DG_FUSION_TYPE Fld(1,0,AC_MSKB0)//[0:0]
#define TTDDG_01 (IO_TTD_BASE + 0xD04)
    #define DG_LOCAL_BASE Fld(8,24,AC_FULLB3)//[31:24]
    #define DG_GLOBAL_BASE Fld(8,16,AC_FULLB2)//[23:16]
    #define DG_LOCAL_WEIGHT Fld(8,8,AC_FULLB1)//[15:8]
    #define DG_GLOBAL_WEIGHT Fld(8,0,AC_FULLB0)//[7:0]
#define TTDDG_02 (IO_TTD_BASE + 0xD08)
    #define GAINV2 Fld(5,25,AC_MSKB3)//[29:25]
    #define GAINV1 Fld(5,20,AC_MSKW32)//[24:20]
    #define GAINU2 Fld(5,15,AC_MSKW21)//[19:15]
    #define GAINU1 Fld(5,10,AC_MSKB1)//[14:10]
    #define GAINY2 Fld(5,5,AC_MSKW10)//[9:5]
    #define GAINY1 Fld(5,0,AC_MSKB0)//[4:0]
#define TTDDG_03 (IO_TTD_BASE + 0xD0C)
    #define DG_DSPRT_MIN Fld(8,24,AC_FULLB3)//[31:24]
    #define DG_DSPRT_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define DG_DSPRT_BIAS Fld(8,8,AC_FULLB1)//[15:8]
#define TTDDG_04 (IO_TTD_BASE + 0xD10)
    #define DG_TFU_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define DG_TFU_BASE Fld(8,16,AC_FULLB2)//[23:16]
    #define DG_TEXTURE_WEIGHT Fld(4,12,AC_MSKB1)//[15:12]
    #define DG_TEXTURE_V Fld(4,8,AC_MSKB1)//[11:8]
    #define DG_TEXTURE_U Fld(4,4,AC_MSKB0)//[7:4]
    #define DG_TEXTURE_Y Fld(4,0,AC_MSKB0)//[3:0]
#define TTDDG_05 (IO_TTD_BASE + 0xD14)
    #define TFU_NBR_CLIP_Y Fld(6,22,AC_MSKW32)//[27:22]
    #define MAX_TUNE_THR Fld(6,16,AC_MSKB2)//[21:16]
    #define TFU_THR_HIGH Fld(16,0,AC_FULLW10)//[15:0]
#define TTDDG_08 (IO_TTD_BASE + 0xD24)
    #define DG_ROI_X_STR Fld(12,20,AC_MSKW32)//[31:20]
    #define DG_ROI_Y_STR Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_09 (IO_TTD_BASE + 0xD28)
    #define DG_ROI_X_END Fld(12,20,AC_MSKW32)//[31:20]
    #define DG_ROI_Y_END Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_11 (IO_TTD_BASE + 0xD30)
    #define DG_1ST_V_GLOBAL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DMAP_FILTER_STRNGTH Fld(4,27,AC_MSKB3)//[30:27]
    #define DMAP_DILATE_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define DMAP_FILTER_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define DMAP_INTPL_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define LOCAL_GAIN_CONTROL_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define TFU_GAIN_CONTROL_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FW_LOCAL_GAIN Fld(4,18,AC_MSKB2)//[21:18]
    #define FW_TFU_GAIN Fld(4,14,AC_MSKW21)//[17:14]
    #define DMAP_INTP_STRNGTH_SEL Fld(3,0,AC_MSKB0)//[2:0]

#define TTDIR_00 (IO_TTD_BASE + 0xD80)
    #define IR_FSCRN_BND_R Fld(10,19,AC_MSKW32)//[28:19]
    #define IR_BLACK_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define IR_PR_INFO Fld(1,17,AC_MSKB2)//[17:17]
    #define IR_PR_MODE Fld(1,16,AC_MSKB2)//[16:16]
    #define IR_FSCRN_BND_L Fld(10,1,AC_MSKW10)//[10:1]
    #define IR_INTP_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_01 (IO_TTD_BASE + 0xD84)
    #define IR_FSCRN_RANGE Fld(12,13,AC_MSKDW)//[24:13]
    #define IR_FSCRN_STEP Fld(12,1,AC_MSKW10)//[12:1]
    #define IR_FSCRN_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_02 (IO_TTD_BASE + 0xD88)
    #define IR_FIFO_LENGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define IR_ERR_CLEAN Fld(1,4,AC_MSKB0)//[4:4]
    #define IR_LEFT_VIEW_OFF Fld(1,1,AC_MSKB0)//[1:1]
    #define IR_RIGHT_VIEW_OFF Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_03 (IO_TTD_BASE + 0xD8C)
    #define IR_BLACK_ASYM Fld(1,31,AC_MSKB3)//[31:31]
    #define IR_BLACK_SINGLE_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define IR_BLACK_SINGLE_INV Fld(1,29,AC_MSKB3)//[29:29]
    #define IR_BLACK_SINGLE_WIDTH_R Fld(8,8,AC_FULLB1)//[15:8]
    #define IR_BLACK_SINGLE_WIDTH_L Fld(8,0,AC_FULLB0)//[7:0]
#define TTDIR_04 (IO_TTD_BASE + 0xD94)
    #define TTD_3DC_HW_MODE Fld(1,26,AC_MSKB3)//[26:26]
    #define IR_GLOBAL_SHIFT Fld(10,16,AC_MSKW32)//[25:16]
    #define IR_DISPT_MIN Fld(8,8,AC_FULLB1)//[15:8]
    #define IR_DISPT_MAX Fld(8,0,AC_FULLB0)//[7:0]
#define TTDIR_05 (IO_TTD_BASE + 0xD98)
    #define IR_DISPT_POSTOFFSET Fld(10,18,AC_MSKW32)//[27:18]
    #define IR_DISPT_PREOFFSET Fld(8,10,AC_MSKW21)//[17:10]
    #define IR_DISPT_SCALAR Fld(10,0,AC_MSKW10)//[9:0]
#define TTDIR_07 (IO_TTD_BASE + 0xDA0)
    #define IR_BLACK_R Fld(11,11,AC_MSKW21)//[21:11]
    #define IR_BLACK_L Fld(11,0,AC_MSKW10)//[10:0]
#define TTDIR_08 (IO_TTD_BASE + 0xDA4)
    #define IR_DEBUG_Y_POS Fld(12,12,AC_MSKW21)//[23:12]
    #define IR_DEBUG_X_POS Fld(12,0,AC_MSKW10)//[11:0]
#define TTDIR_09 (IO_TTD_BASE + 0xD90)
    #define IR_BLACK_BAR_LEVEL Fld(8,0,AC_FULLB0)//[7:0]
#define STA_TTDIR_00 (IO_TTD_BASE + 0xDA8)
    #define STA_IR_XY_POS_DEPTH Fld(8,8,AC_FULLB1)//[15:8]
    #define STA_IR_ERR_OVER Fld(1,1,AC_MSKB0)//[1:1]
    #define STA_IR_ERR_UNDER Fld(1,0,AC_MSKB0)//[0:0]

#endif // #ifndef _HW_TTD_H_

