
#ifndef _HW_PSC_H_
#define _HW_PSC_H_

#include "x_hal_5381.h"

#define IO_POSTSC_BASE  (IO_VIRT + 0x5E000)

//Page PSCCTRL
#define PSCCTRL_05 (IO_POSTSC_BASE + 0x214)
    #define PSC_3D_MODE Fld(4,0,AC_MSKB0)//[3:0]
#define PSCCTRL_09 (IO_POSTSC_BASE + 0x224)
    #define PSC_INPUT_WIDTH Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_INPUT_HEIGHT Fld(16,0,AC_FULLW10)//[15:0]
#define PSCCTRL_0A (IO_POSTSC_BASE + 0x228)
    #define PSC_LR_PAIR_CTRL Fld(2,6,AC_MSKB0)//[7:6]
    #define PSC_CLR_STA_SET_RES_O Fld(1,5,AC_MSKB0)//[5:5]
    #define PSC_STA_SET_RES_O Fld(1,4,AC_MSKB0)//[4:4]
    #define PSC_CLR_STA_SET_RES_I Fld(1,3,AC_MSKB0)//[3:3]
    #define PSC_STA_SET_RES_I Fld(1,2,AC_MSKB0)//[2:2]
    #define PSC_SET_RES_TOGGLE Fld(1,1,AC_MSKB0)//[1:1]
    #define PSC_SET_RES_IMPORT_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define PSCCTRL_0B (IO_POSTSC_BASE + 0x22C)
    #define PSC_STA_DISPFF_OVFL Fld(1,28,AC_MSKB3)//[28:28]
    #define PSC_CLR_DISPFF_FLG Fld(1,27,AC_MSKB3)//[27:27]
    #define PSC_DISP_FF_WTLV Fld(11,0,AC_MSKW10)//[10:0]
#define PSCCTRL_0C (IO_POSTSC_BASE + 0x230)
    #define PSC_MJC_INIT_DAT_RATIO Fld(10,0,AC_MSKW10)//[9:0]
#define PSCCTRL_0D (IO_POSTSC_BASE + 0x234)
    #define PSC_HMSK_STR_OFST_1 Fld(17,0,AC_MSKDW)//[16:0]
#define PSCCTRL_0E (IO_POSTSC_BASE + 0x238)
    #define PSC_VMSK_STR_OFST_1 Fld(17,0,AC_MSKDW)//[16:0]
#define PSCCTRL_11 (IO_POSTSC_BASE + 0x244)
    #define PSC_OUTPUT_WIDTH Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_OUTPUT_HEIGHT Fld(16,0,AC_FULLW10)//[15:0]
#define PSCCTRL_17 (IO_POSTSC_BASE + 0x25C)
    #define PSC_MIRROR_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define PSC_FLIP_EN Fld(1,2,AC_MSKB0)//[2:2]
#define PSCCTRL_18 (IO_POSTSC_BASE + 0x260)
    #define PSC_MJC_DISP_INF_HTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define PSCCTRL_19 (IO_POSTSC_BASE + 0x264)
    #define PSC_VMSK_END_CROP_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define PSC_VMSK_STR_CROP_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define PSC_HMSK_END_CROP_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define PSC_HMSK_STR_CROP_1 Fld(8,0,AC_FULLB0)//[7:0]
#define PSCCTRL_33 (IO_POSTSC_BASE + 0x2CC)
    #define PSC_PSCCTRL_RESERVED0 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCCTRL_34 (IO_POSTSC_BASE + 0x2D0)
    #define PSC_PSCCTRL_RESERVED1 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCCTRL_35 (IO_POSTSC_BASE + 0x2D4)
    #define PSC_PSCCTRL_RESERVED2 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCCTRL_36 (IO_POSTSC_BASE + 0x2D8)
    #define PSC_PSCCTRL_RESERVED3 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCCTRL_37 (IO_POSTSC_BASE + 0x2DC)
    #define PSC_PSCCTRL_RESERVED4 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCCTRL_38 (IO_POSTSC_BASE + 0x2F0)
    #define PSC_PSCCTRL_RESERVED5 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCCTRL_39 (IO_POSTSC_BASE + 0x2F4)
    #define PSC_PSCCTRL_RESERVED6 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCCTRL_3A (IO_POSTSC_BASE + 0x2F8)
    #define PSC_PSCCTRL_RESERVED7 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCCTRL_ATPG (IO_POSTSC_BASE + 0x2FC)
    #define PSCCTRL_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define PSCCTRL_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]
    
//Page PSCFIR
#define PSCFIR_00 (IO_POSTSC_BASE + 0x500)
    #define PSC_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define PSC_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define PSC_RDATA_BYTE_EN_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define PSC_WDATA_BYTE_EN_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define PSC_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define PSC_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define PSC_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define PSC_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define PSC_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define PSCFIR_01 (IO_POSTSC_BASE + 0x504)
    #define PSC_CPU_SEL_SRAM_0 Fld(1,7,AC_MSKB0)//[7:7]
    #define PSC_CPURW_ACTIVE Fld(1,6,AC_MSKB0)//[6:6]
    #define PSC_TABLE_WAIT_VSYNC Fld(1,5,AC_MSKB0)//[5:5]
    #define PSC_FILL_TABLE_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define PSC_READ_FINISH Fld(1,3,AC_MSKB0)//[3:3]
    #define PSC_READ Fld(1,2,AC_MSKB0)//[2:2]
    #define PSC_WRITE_FINISH Fld(1,1,AC_MSKB0)//[1:1]
    #define PSC_WRITE Fld(1,0,AC_MSKB0)//[0:0]
#define PSCFIR_02 (IO_POSTSC_BASE + 0x508)
    #define PSC_COE_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define PSC_CPU_SRAM_ADDR Fld(8,16,AC_FULLB2)//[23:16]
    #define PSC_R_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define PSCFIR_03 (IO_POSTSC_BASE + 0x50C)
    #define PSC_WDATA_00 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCFIR_04 (IO_POSTSC_BASE + 0x510)
    #define PSC_WDATA_01 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCFIR_05 (IO_POSTSC_BASE + 0x514)
    #define PSC_WDATA_02 Fld(30,0,AC_MSKDW)//[29:0]
#define PSCFIR_06 (IO_POSTSC_BASE + 0x518)
    #define PSC_RDATA_00 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCFIR_07 (IO_POSTSC_BASE + 0x51C)
    #define PSC_RDATA_01 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCFIR_08 (IO_POSTSC_BASE + 0x520)
    #define PSC_RDATA_02 Fld(30,0,AC_MSKDW)//[29:0]
#define PSCFIR_09 (IO_POSTSC_BASE + 0x524)
    #define PSC_DE_RING_EN_V Fld(1,31,AC_MSKB3)//[31:31]
    #define PSC_DE_RING_SEL_V Fld(2,28,AC_MSKB3)//[29:28]
    #define PSC_DE_RING_SLOPE_V Fld(4,20,AC_MSKB2)//[23:20]
    #define PSC_DE_RING_MAX_THRESHOLD_V Fld(8,12,AC_MSKW21)//[19:12]
#define PSCFIR_0A (IO_POSTSC_BASE + 0x528)
    #define PSC_DE_RING_EN_H_YUV Fld(1,31,AC_MSKB3)//[31:31]
    #define PSC_DE_RING_SEL_H_YUV Fld(2,28,AC_MSKB3)//[29:28]
    #define PSC_DE_RING_SLOPE_H_Y Fld(4,20,AC_MSKB2)//[23:20]
    #define PSC_DE_RING_MAX_THRESHOLD_H_Y Fld(8,12,AC_MSKW21)//[19:12]
    #define PSC_DE_RING_FIRST_SLOPE_H_Y Fld(4,8,AC_MSKB1)//[11:8]
    #define PSC_DE_RING_FIRST_THRESHOLD_H_Y Fld(8,0,AC_FULLB0)//[7:0]
#define PSCFIR_0B (IO_POSTSC_BASE + 0x52C)
    #define PSC_RING_THR_H_UV Fld(8,20,AC_MSKW32)//[27:20]
    #define PSC_SLOPE_SEL_H_UV Fld(4,16,AC_MSKB2)//[19:16]
    #define PSC_RING_1ST_THR_H_UV Fld(8,8,AC_FULLB1)//[15:8]
    #define PSC_SLOPE_1ST_SEL_H_UV Fld(4,4,AC_MSKB0)//[7:4]
#define PSCFIR_0C (IO_POSTSC_BASE + 0x530)
    #define PSC_H_CV422_COEFF Fld(1,2,AC_MSKB0)//[2:2]
    #define PSC_H_SRAM_COEFF_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define PSC_V_SRAM_COEFF_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PSCFIR_ATPG (IO_POSTSC_BASE + 0x5FC)
    #define PSC_PSCFIR_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define PSC_PSCFIR_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]
    
//Page PSCSYS
#define PSCSYS_00 (IO_POSTSC_BASE + 0x300)
    #define PSC_OCLK_CLKEN Fld(1,3,AC_MSKB0)//[3:3]
    #define PSC_USE_INTL_CKEN Fld(1,2,AC_MSKB0)//[2:2]
    #define PSC_PSC_CLK_SWRST Fld(1,0,AC_MSKB0)//[0:0]
#define PSCSYS_01 (IO_POSTSC_BASE + 0x304)
    #define PSC_SET_DATA Fld(1,30,AC_MSKB3)//[30:30]
    #define PSC_CLR_INPUT_MEASURE Fld(1,28,AC_MSKB3)//[28:28]
    #define PSC_CLR_PTG_UDFL_FLG Fld(1,26,AC_MSKB3)//[26:26]
    #define PSC_CLR_MTG_UDFL_FLG Fld(1,21,AC_MSKB2)//[21:21]
    #define PSC_STA_M_TG_UNDERFLOW Fld(1,20,AC_MSKB2)//[20:20]
    #define PSC_STA_SET_DATA Fld(1,19,AC_MSKB2)//[19:19]
    #define PSC_TG_DISP_R_FREERUN_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define PSC_TG_IMPORT_ENABLE Fld(1,16,AC_MSKB2)//[16:16]
    #define PSC_TG_IMPORT_VCNT Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_02 (IO_POSTSC_BASE + 0x308)
    #define PSC_PNL_WIDTH Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_PNL_HEIGHT Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_03 (IO_POSTSC_BASE + 0x30C)
    #define PSC_PNL_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_PNL_VTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_04 (IO_POSTSC_BASE + 0x310)
    #define PSC_PNL_BACK_HPORCH Fld(16,16,AC_FULLW32)//[31:16] 
    #define PSC_PNL_FRNT_HPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_05 (IO_POSTSC_BASE + 0x314)
    #define PSC_PNL_BACK_VPORCH Fld(16,16,AC_FULLW32)//[31:16] 
    #define PSC_PNL_FRNT_VPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_07 (IO_POSTSC_BASE + 0x31C)
    #define PSC_SCALER_DEBUG_SEL Fld(8,24,AC_FULLB3)//[31:24]
#define PSCSYS_09 (IO_POSTSC_BASE + 0x324)
    #define PSC_STA_OUT_DISP_R Fld(1,31,AC_MSKB3)//[31:31]
    #define PSC_STA_IN_DISP_R Fld(1,30,AC_MSKB3)//[30:30]
    #define PSC_FORCE_IN_DISP_R_VAL Fld(1,29,AC_MSKB3)//[29:29]
    #define PSC_FORCE_IN_DISP_R Fld(1,28,AC_MSKB3)//[28:28]
    #define PSC_FRM_MASK_O_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define PSC_FRM_MASK_I_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define PSC_INV_OUTPUT_DISP_R Fld(1,19,AC_MSKB2)//[19:19]
    #define PSC_CLR_STA_INPUTV_ABNORMAL Fld(1,18,AC_MSKB2)//[18:18]
//#define PSCSYS_0B (IO_POSTSC_BASE + 0x32C)
//    #define PSC_CLR_DISPFF_FLG Fld(1,27,AC_MSKB3)//[27:27]
#define PSCSYS_10 (IO_POSTSC_BASE + 0x340)
    #define PSC_INV_OUTPUT_HSYNC Fld(1,31,AC_MSKB3)//[31:31]
    #define PSC_INV_OUTPUT_VSYNC Fld(1,30,AC_MSKB3)//[30:30]
    #define PSC_INV_OUTPUT_DE Fld(1,29,AC_MSKB3)//[29:29]
    #define PSC_FRM_MASK_IN_LR_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define PSC_FRM_MASK_IN_LR_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define PSC_FRM_MASK_OUT_LR_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define PSC_FRM_MASK_OUT_LR_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define PSC_FRM_MASK_I Fld(4,20,AC_MSKB2)//[23:20]
    #define PSC_FRM_MASK_O Fld(4,16,AC_MSKB2)//[19:16]
    #define PSC_FRM_TRK_LINE Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_11 (IO_POSTSC_BASE + 0x344)
    #define PSC_DDDS_TRK_INV Fld(1,31,AC_MSKB3)//[31:31]
    #define PSC_FRM_TRK_DDDS_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define PSC_INV_OSD_DISP_R Fld(1,26,AC_MSKB3)//[26:26]
    #define PSC_INV_OSD_HSYNC Fld(1,25,AC_MSKB3)//[25:25]
    #define PSC_INV_OSD_VSYNC Fld(1,24,AC_MSKB3)//[24:24]
    #define PSC_INV_INPUT_M_DISP_R Fld(1,23,AC_MSKB2)//[23:23]
    #define PSC_INV_REF_DISP_R Fld(1,22,AC_MSKB2)//[22:22]
    #define PSC_INV_REF_VSYNC_IN Fld(1,21,AC_MSKB2)//[21:21]
    #define PSC_INV_INPUT_PVDE Fld(1,20,AC_MSKB2)//[20:20]
    #define PSC_INV_INPUT_PHDE Fld(1,19,AC_MSKB2)//[19:19]
    #define PSC_INV_INPUT_MVDE Fld(1,18,AC_MSKB2)//[18:18]
    #define PSC_INV_INPUT_MHDE Fld(1,17,AC_MSKB2)//[17:17]
    #define PSC_INV_MSK Fld(1,15,AC_MSKB1)//[15:15]
    #define PSC_FRM_TRK_OUT_SEL Fld(2,13,AC_MSKB1)//[14:13]
    #define PSC_FRM_TRK_ONCE_ONLY Fld(1,11,AC_MSKB1)//[11:11]
    #define PSC_FRM_TRK_VIN_EDGE_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define PSC_TG_DAT_RATIO Fld(10,0,AC_MSKW10)//[9:0]
#define PSCSYS_12 (IO_POSTSC_BASE + 0x348)
    #define PSC_INOUT_VSYNC_DIST_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_INOUT_VSYNC_DIST_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_13 (IO_POSTSC_BASE + 0x34C)
    #define PSC_STA_FRM_VSYNC_OUT_EARLY Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_P_HMSK_DLY Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_14 (IO_POSTSC_BASE + 0x350)
    #define PSC_DDDS_TURBO_RGN Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_OSD_HSYNC_SEL Fld(1,11,AC_MSKB1)//[11:11]
    #define PSC_OSD_VSYNC_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define PSC_STA_FRM_TRK_LOCK_OK Fld(1,8,AC_MSKB1)//[8:8]
    #define PSC_FRM_TRK_STEP Fld(8,0,AC_FULLB0)//[7:0]
#define PSCSYS_15 (IO_POSTSC_BASE + 0x354)
    #define PSC_FRM_LOCK_TOR Fld(8,24,AC_FULLB3)//[31:24]
    #define PSC_FRM_LOCK_WIN Fld(8,16,AC_FULLB2)//[23:16]
    #define PSC_OSD_VSYNC_DLY Fld(8,8,AC_FULLB1)//[15:8]
#define PSCSYS_16 (IO_POSTSC_BASE + 0x358)
    #define PSC_MIN_CRC_CLIP_V_START Fld(12,18,AC_MSKW32)//[29:18]
    #define PSC_MIN_CRC_CLIP_H_START Fld(13,5,AC_MSKDW)//[17:5]
    #define PSC_MIN_CRC_CHECK_SUM_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define PSC_MIN_CRC_BOT_R_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define PSC_MIN_CRC_TOP_R_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define PSC_MIN_CRC_BOT_L_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define PSC_MIN_CRC_TOP_L_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PSCSYS_17 (IO_POSTSC_BASE + 0x35C)
    #define PSC_INV_MEASURE_H Fld(1,29,AC_MSKB3)//[29:29]
    #define PSC_INV_MEASURE_V Fld(1,28,AC_MSKB3)//[28:28]
    #define PSC_CLR_OUTPUT_MEASURE Fld(1,27,AC_MSKB3)//[27:27]
    #define PSC_OUTPUT_MEASURE_SEL Fld(2,25,AC_MSKB3)//[26:25]
    #define PSC_MIN_CRC_CLIP_V_END Fld(12,13,AC_MSKDW)//[24:13]
    #define PSC_MIN_CRC_CLIP_H_END Fld(13,0,AC_MSKW10)//[12:0]
#define PSCSYS_18 (IO_POSTSC_BASE + 0x360)
    #define PSC_MIN_CRC_V_IN_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define PSC_MIN_CRC_C_IN_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define PSC_MIN_CRC_Y_IN_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define PSCSYS_19 (IO_POSTSC_BASE + 0x364)
    #define PSC_MIN_STA_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_1A (IO_POSTSC_BASE + 0x368)
    #define PSC_MIN_STA_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_1B (IO_POSTSC_BASE + 0x36C)
    #define PSC_MOUT_CRC_VSYNC_INV Fld(1,30,AC_MSKB3)//[30:30]
    #define PSC_MOUT_CRC_CLIP_V_START Fld(12,18,AC_MSKW32)//[29:18]
    #define PSC_MOUT_CRC_CLIP_H_START Fld(13,5,AC_MSKDW)//[17:5]
    #define PSC_MOUT_CRC_CHECK_SUM_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define PSC_MOUT_CRC_BOT_R_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define PSC_MOUT_CRC_TOP_R_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define PSC_MOUT_CRC_BOT_L_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define PSC_MOUT_CRC_TOP_L_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PSCSYS_1C (IO_POSTSC_BASE + 0x370)
    #define PSC_MOUT_CRC_CLIP_V_END Fld(12,13,AC_MSKDW)//[24:13]
    #define PSC_MOUT_CRC_CLIP_H_END Fld(13,0,AC_MSKW10)//[12:0]
#define PSCSYS_1D (IO_POSTSC_BASE + 0x374)
    #define PSC_MOUT_CRC_V_IN_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define PSC_MOUT_CRC_C_IN_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define PSC_MOUT_CRC_Y_IN_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define PSCSYS_1E (IO_POSTSC_BASE + 0x378)
    #define PSC_MOUT_STA_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_1F (IO_POSTSC_BASE + 0x37C)
    #define PSC_MOUT_STA_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_20 (IO_POSTSC_BASE + 0x380)
    #define PSC_STA_H_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_STA_V_CNT Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_21 (IO_POSTSC_BASE + 0x384)
    #define PSC_INPTG_YPAT1_1 Fld(4,27,AC_MSKB3)//[30:27]
    #define PSC_INPTG_LEFT_SIDE_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PSC_INPTG_PAT_DUTY_CYCLE_1 Fld(5,21,AC_MSKW32)//[25:21]
    #define PSC_INPTG_ANGLE_SEL_1 Fld(3,18,AC_MSKB2)//[20:18]
    #define PSC_INPTG_FREQ_SEL_1 Fld(3,15,AC_MSKW21)//[17:15]
    #define PSC_INPTG_HOR_LINE_EN_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define PSC_INPTG_COLOR_MODE_1 Fld(2,11,AC_MSKB1)//[12:11]
    #define PSC_INPTG_PAT_MODE_1 Fld(2,9,AC_MSKB1)//[10:9]
    #define PSC_INPTG_FREERUN_SPEED_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define PSC_INPTG_FREERUN_EN_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define PSC_INPTG_BND_EN_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define PSC_INPTG_3D_MODE_1 Fld(2,1,AC_MSKB0)//[2:1]
    #define PSC_INPTG_3D_INV_1 Fld(1,0,AC_MSKB0)//[0:0]
#define PSCSYS_22 (IO_POSTSC_BASE + 0x388)
    #define PSC_INPTG_BND_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_INPTG_BND_W_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_23 (IO_POSTSC_BASE + 0x38C)
    #define PSC_P_VMSK_DLY Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_OUTPTG_ENABLE_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define PSC_OUTPTG_YPAT2_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define PSC_INPTG_ENABLE_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define PSC_INPTG_YPAT2_1 Fld(4,0,AC_MSKB0)//[3:0]
#define PSCSYS_24 (IO_POSTSC_BASE + 0x390)
    #define PSC_OUTPTG_YPAT1_1 Fld(4,27,AC_MSKB3)//[30:27]
    #define PSC_OUTPTG_LEFT_SIDE_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PSC_OUTPTG_PAT_DUTY_CYCLE_1 Fld(5,21,AC_MSKW32)//[25:21]
    #define PSC_OUTPTG_ANGLE_SEL_1 Fld(3,18,AC_MSKB2)//[20:18]
    #define PSC_OUTPTG_FREQ_SEL_1 Fld(3,15,AC_MSKW21)//[17:15]
    #define PSC_OUTPTG_HOR_LINE_EN_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define PSC_OUTPTG_COLOR_MODE_1 Fld(2,11,AC_MSKB1)//[12:11]
    #define PSC_OUTPTG_PAT_MODE_1 Fld(2,9,AC_MSKB1)//[10:9]
    #define PSC_OUTPTG_FREERUN_SPEED_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define PSC_OUTPTG_FREERUN_EN_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define PSC_OUTPTG_BND_EN_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define PSC_OUTPTG_3D_MODE_1 Fld(2,1,AC_MSKB0)//[2:1]
    #define PSC_OUTPTG_3D_INV_1 Fld(1,0,AC_MSKB0)//[0:0]
#define PSCSYS_25 (IO_POSTSC_BASE + 0x394)
    #define PSC_OUTPTG_BND_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_OUTPTG_BND_W_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_26 (IO_POSTSC_BASE + 0x398)
    #define PSC_STA_FRM_TRK_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_STA_FRM_TRK_ABS_DIS Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_27 (IO_POSTSC_BASE + 0x39C)
    #define PSC_FRM_TRK_TURBO_RGN Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_28 (IO_POSTSC_BASE + 0x3A0)
    #define PSC_STA_HACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_STA_HTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_29 (IO_POSTSC_BASE + 0x3A4)
    #define PSC_STA_VACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_STA_VTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_2A (IO_POSTSC_BASE + 0x3A8)
    #define PSC_STA_OUT_MEASURE_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_STA_OUT_MEASURE_VTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_2B (IO_POSTSC_BASE + 0x3AC)
    #define PSC_STA_OUT_MEASURE_HACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_STA_OUT_MEASURE_VACTIVE Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_2C (IO_POSTSC_BASE + 0x3B0)
    #define PSC_SET_DATA_HTOT Fld(16,16,AC_FULLW32)//[31:16]
    #define PSC_SET_DATA_HACT Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_2D (IO_POSTSC_BASE + 0x3B4)
    #define PSC_SET_DATA_VTOT Fld(16,0,AC_FULLW10)//[15:0]
#define PSCSYS_2E (IO_POSTSC_BASE + 0x3B8)
    #define PSC_STALL_DLY Fld(2,0,AC_MSKB0)//[1:0]
#define PSCSYS_33 (IO_POSTSC_BASE + 0x3CC)
    #define PSC_PSCSYS_RESERVED0 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_34 (IO_POSTSC_BASE + 0x3D0)
    #define PSC_PSCSYS_RESERVED1 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_35 (IO_POSTSC_BASE + 0x3D4)
    #define PSC_PSCSYS_RESERVED2 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_36 (IO_POSTSC_BASE + 0x3D8)
    #define PSC_PSCSYS_RESERVED3 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_37 (IO_POSTSC_BASE + 0x3DC)
    #define PSC_PSCSYS_RESERVED4 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_38 (IO_POSTSC_BASE + 0x3E0)
    #define PSC_PSCSYS_RESERVED5 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_39 (IO_POSTSC_BASE + 0x3E4)
    #define PSC_PSCSYS_RESERVED6 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_3A (IO_POSTSC_BASE + 0x3E8)
    #define PSC_PSCSYS_RESERVED7 Fld(32,0,AC_FULLDW)//[31:0]
#define PSCSYS_ATPG (IO_POSTSC_BASE + 0x3FC)
    #define PSCSYS_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define PSCSYS_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]
#endif
