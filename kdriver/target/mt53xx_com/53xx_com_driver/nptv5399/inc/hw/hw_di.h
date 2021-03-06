#ifndef _HW_DI_H_
#define _HW_DI_H_

#include "hw_vdoin.h"

#define IO_MDDI_BASE (VDOIN_BASE + 0x4000)

//Page GC
#define MCVP_GC_00 (IO_MDDI_BASE + 0x000)
    #define GC_RSV Fld(16,16,AC_FULLW32)//[31:16]
    #define TEST_IN_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define P_SYNC_FULL_SPEED Fld(1,13,AC_MSKB1)//[13:13]
    #define M_SYNC_FULL_SPEED Fld(1,12,AC_MSKB1)//[12:12]
    #define WR_NO_QUEUE Fld(1,11,AC_MSKB1)//[11:11]
    #define SRAM_INIT_REQ Fld(1,10,AC_MSKB1)//[10:10]
    #define CLK_METER_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define MON_SEL Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_GC_01 (IO_MDDI_BASE + 0x004)
    #define GC_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define GC_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define DRAM_CK_ACTIVE Fld(1,27,AC_MSKB3)//[27:27]
    #define KERNEL_CK_ACTIVE Fld(1,26,AC_MSKB3)//[26:26]
    #define CLK_ACTIVE Fld(1,25,AC_MSKB3)//[25:25]
    #define CLK_PERIOD_READY Fld(1,24,AC_MSKB3)//[24:24]
    #define CLK_PERIOD Fld(24,0,AC_MSKDW)//[23:0]
#define MCVP_GC_02 (IO_MDDI_BASE + 0x008)
    #define FPGA_VERSION Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_GC_03 (IO_MDDI_BASE + 0x00C)
    #define FPGA_CONFIG Fld(32,0,AC_FULLDW)//[31:0]

//Page BC
#define MCVP_BC_00 (IO_MDDI_BASE + 0x010)
    #define PIP_RD_LOCAL_VP Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_WR_LOCAL_VP Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_RD_SET_RES_VP Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_WR_SET_RES_VP Fld(1,28,AC_MSKB3)//[28:28]
    #define MAIN_RD_LOCAL_VP Fld(1,27,AC_MSKB3)//[27:27]
    #define MAIN_WR_LOCAL_VP Fld(1,26,AC_MSKB3)//[26:26]
    #define MAIN_RD_SET_RES_VP Fld(1,25,AC_MSKB3)//[25:25]
    #define MAIN_WR_SET_RES_VP Fld(1,24,AC_MSKB3)//[24:24]
    #define BC_RSV_2 Fld(3,21,AC_MSKB2)//[23:21]
    #define EXTRA_PIP_BUF_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define BC_RSV_1 Fld(2,18,AC_MSKB2)//[19:18]
    #define MJC_DBG_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define PR_MODE_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define FRC_CK_DIV_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define DRAM_P_CK_RST Fld(1,13,AC_MSKB1)//[13:13]
    #define DRAM_M_CK_RST Fld(1,12,AC_MSKB1)//[12:12]
    #define MCVP_K_P_CK_RST Fld(1,11,AC_MSKB1)//[11:11]
    #define MCVP_K_M_CK_RST Fld(1,10,AC_MSKB1)//[10:10]
    #define MIB_IN_P_CK_RST Fld(1,9,AC_MSKB1)//[9:9]
    #define MIB_IN_M_CK_RST Fld(1,8,AC_MSKB1)//[8:8]
    #define BIM_CK_RST Fld(1,7,AC_MSKB0)//[7:7]
    #define BC_RSV_0 Fld(1,6,AC_MSKB0)//[6:6]
    #define DRAM_CK_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define MCVP_K_M_PQ_CK_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define MCVP_K_P_CK_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define MCVP_K_M_CK_EN Fld(1,2,AC_MSKB0)//[2:2]
#define MCVP_BC_01 (IO_MDDI_BASE + 0x014)
    #define INT_PO_ADJ_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define INT_POVS_F_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define INT_POVS_R_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define INT_MOVS_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define INT_PI_ADJ_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define INT_PIVS_F_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define INT_PIVS_R_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define INT_MO_ADJ_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define INT_MOVS_F_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define INT_MOVS_R_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define INT_MI_ADJ_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define INT_MIVS_F_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define INT_MIVS_R_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define INT_PO_ADJ_CLR Fld(1,14,AC_MSKB1)//[14:14]
    #define INT_POVS_F_CLR Fld(1,13,AC_MSKB1)//[13:13]
    #define INT_POVS_R_CLR Fld(1,12,AC_MSKB1)//[12:12]
    #define INT_PI_ADJ_CLR Fld(1,10,AC_MSKB1)//[10:10]
    #define INT_PIVS_F_CLR Fld(1,9,AC_MSKB1)//[9:9]
    #define INT_PIVS_R_CLR Fld(1,8,AC_MSKB1)//[8:8]
    #define INT_MO_ADJ_CLR Fld(1,6,AC_MSKB0)//[6:6]
    #define INT_MOVS_F_CLR Fld(1,5,AC_MSKB0)//[5:5]
    #define INT_MOVS_R_CLR Fld(1,4,AC_MSKB0)//[4:4]
    #define INT_MI_ADJ_CLR Fld(1,2,AC_MSKB0)//[2:2]
    #define INT_MIVS_F_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define INT_MIVS_R_CLR Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_BC_02 (IO_MDDI_BASE + 0x018)
    #define INT_PO_ADJ_STA Fld(1,14,AC_MSKB1)//[14:14]
    #define INT_POVS_F_STA Fld(1,13,AC_MSKB1)//[13:13]
    #define INT_POVS_R_STA Fld(1,12,AC_MSKB1)//[12:12]
    #define INT_PI_ADJ_STA Fld(1,10,AC_MSKB1)//[10:10]
    #define INT_PIVS_F_STA Fld(1,9,AC_MSKB1)//[9:9]
    #define INT_PIVS_R_STA Fld(1,8,AC_MSKB1)//[8:8]
    #define INT_MO_ADJ_STA Fld(1,6,AC_MSKB0)//[6:6]
    #define INT_MOVS_F_STA Fld(1,5,AC_MSKB0)//[5:5]
    #define INT_MOVS_R_STA Fld(1,4,AC_MSKB0)//[4:4]
    #define INT_MI_ADJ_STA Fld(1,2,AC_MSKB0)//[2:2]
    #define INT_MIVS_F_STA Fld(1,1,AC_MSKB0)//[1:1]
    #define INT_MIVS_R_STA Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_BC_03 (IO_MDDI_BASE + 0x01C)
    #define BC_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define BC_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define SET_RANDOM_SEED_DRAM_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define SET_RANDOM_SEED_C_P Fld(1,27,AC_MSKB3)//[27:27]
    #define SET_RANDOM_SEED_Y_P Fld(1,26,AC_MSKB3)//[26:26]
    #define SET_RANDOM_SEED_C_M Fld(1,25,AC_MSKB3)//[25:25]
    #define SET_RANDOM_SEED_Y_M Fld(1,24,AC_MSKB3)//[24:24]
    #define SET_RANDOM_SEED_M7 Fld(1,23,AC_MSKB2)//[23:23]
    #define SET_RANDOM_SEED_M6 Fld(1,22,AC_MSKB2)//[22:22]
    #define SET_RANDOM_SEED_M5 Fld(1,21,AC_MSKB2)//[21:21]
    #define SET_RANDOM_SEED_M4 Fld(1,20,AC_MSKB2)//[20:20]
    #define SET_RANDOM_SEED_M3 Fld(1,19,AC_MSKB2)//[19:19]
    #define SET_RANDOM_SEED_M2 Fld(1,18,AC_MSKB2)//[18:18]
    #define SET_RANDOM_SEED_M1 Fld(1,17,AC_MSKB2)//[17:17]
    #define SET_RANDOM_SEED_M0 Fld(1,16,AC_MSKB2)//[16:16]
    #define FBIST_RANDOM_SEED Fld(16,0,AC_FULLW10)//[15:0]
#define MCVP_BC_04 (IO_MDDI_BASE + 0x020)
    #define FRAME_CNT_P_OUT Fld(8,24,AC_FULLB3)//[31:24]
    #define FRAME_CNT_P_IN Fld(8,16,AC_FULLB2)//[23:16]
    #define FRAME_CNT_M_OUT Fld(8,8,AC_FULLB1)//[15:8]
    #define FRAME_CNT_M_IN Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_BC_05 (IO_MDDI_BASE + 0x024)
    #define FINAL_PRB_Y_POS Fld(12,16,AC_MSKW32)//[27:16]
    #define FINAL_PRB_X_POS Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_BC_06 (IO_MDDI_BASE + 0x028)
    #define OUT_FORMAT Fld(4,24,AC_MSKB3)//[27:24]
    #define IN_FORMAT Fld(4,20,AC_MSKB2)//[23:20]
    #define FINAL_BORDER_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define FINAL_CURSOR_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define FINAL_OSD_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FINAL_OSD_SIZE Fld(1,16,AC_MSKB2)//[16:16]
    #define FINAL_MP_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define FINAL_TIME_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define FINAL_BORDER_LUMA Fld(4,8,AC_MSKB1)//[11:8]
    #define FINAL_CURSOR_LUMA Fld(4,4,AC_MSKB0)//[7:4]
    #define FINAL_OSD_LUMA Fld(4,0,AC_MSKB0)//[3:0]
#define MCVP_BC_07 (IO_MDDI_BASE + 0x02C)
    #define FINAL_PROBE_DISP_R Fld(1,31,AC_MSKB3)//[31:31]
    #define FINAL_PROBE_FLD Fld(1,30,AC_MSKB3)//[30:30]
    #define FINAL_PROBE_V Fld(10,20,AC_MSKW32)//[29:20]
    #define FINAL_PROBE_C Fld(10,10,AC_MSKW21)//[19:10]
    #define FINAL_PROBE_Y Fld(10,0,AC_MSKW10)//[9:0]

//Page MULTI
#define MCVP_MULTI_00 (IO_MDDI_BASE + 0x040)
    #define FBIST_ON Fld(1,15,AC_MSKB1)//[15:15]
    #define OUTBUF_SYNC Fld(1,13,AC_MSKB1)//[13:13]
    #define KERNEL_SYNC Fld(1,12,AC_MSKB1)//[12:12]
    #define PROG_MODE Fld(1,11,AC_MSKB1)//[11:11]
    #define EN_DRAM_RW Fld(9, 0, AC_MSKW10) //[8:0]  
    #define EN_NRW Fld(1,8,AC_MSKB1)//[8:8]
    #define EN_RNRR Fld(1,7,AC_MSKB0)//[7:7]
    #define EN_RNRW Fld(1,6,AC_MSKB0)//[6:6]
    #define EN_MVR Fld(1,5,AC_MSKB0)//[5:5]
    #define EN_MVW Fld(1,4,AC_MSKB0)//[4:4]
    #define EN_LMR Fld(1,3,AC_MSKB0)//[3:3]
    #define EN_LMW Fld(1,2,AC_MSKB0)//[2:2]
    #define EN_SRCR Fld(1,1,AC_MSKB0)//[1:1]
    #define EN_SRCW Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_MULTI_01 (IO_MDDI_BASE + 0x044)
    #define MULTI_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define MULTI_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define MULTI_RSV Fld(16,0,AC_FULLW10)//[15:0]

//Page KC_1
#define MCVP_KC_00 (IO_MDDI_BASE + 0x100)
    #define DISP_R_IN_P Fld(1,31,AC_MSKB3)//[31:31]
    #define FD_INFO_PROG_OS_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define PAL_C_AVG_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OUT_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define BYPASS_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define FRAME420 Fld(1,26,AC_MSKB3)//[26:26]
    #define SRCW_AUTO_STOP Fld(1,25,AC_MSKB3)//[25:25]
    #define I2P_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define ROUND_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define MJCFD_DEBUG_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define ERR_DFU_RAND_INI_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define ERR_DFU_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define SRCW_V_BGN Fld(4,16,AC_MSKB2)//[19:16]
    #define V_START_WAIT Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_IN_P Fld(1,14,AC_MSKB1)//[14:14]
    #define SRCW_CRC_SEL Fld(1,13,AC_MSKB1)//[13:13]
    #define FRC_LINE Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_01 (IO_MDDI_BASE + 0x104)
    #define IPTG_SELFGEN_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define IPTG_SELF_DISP_R_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define IPTG_SELF_FLD_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define IPTG_SELF_VT Fld(13,16,AC_MSKW32)//[28:16]
    #define IPTG_TIME_MODE Fld(2,14,AC_MSKB1)//[15:14]
    #define IPTG_SELF_HT Fld(14,0,AC_MSKW10)//[13:0]
#define MCVP_KC_02 (IO_MDDI_BASE + 0x108)
    #define IPTG_TIME_EN Fld(4,28,AC_MSKB3)//[31:28]
    #define IPTG_V_ACT Fld(12,16,AC_MSKW32)//[27:16]
    #define IPTG_TIME_STEP Fld(3,13,AC_MSKB1)//[15:13]
    #define IPTG_H_ACT Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_03 (IO_MDDI_BASE + 0x10C)
    #define IPTG_DE_N Fld(5,27,AC_MSKB3)//[31:27]
    #define IPTG_NOISE_EN Fld(3,24,AC_MSKB3)//[26:24]
    #define IPTG_V_OFFSET Fld(8,16,AC_FULLB2)//[23:16]
    #define IPTG_PAT_TYPE Fld(4,12,AC_MSKB1)//[15:12]
    #define IPTG_NOISE_LEVEL Fld(4,8,AC_MSKB1)//[11:8]
    #define IPTG_H_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_04 (IO_MDDI_BASE + 0x110)
    #define IPTG_DE_M Fld(5,27,AC_MSKB3)//[31:27]
    #define IPTG_GRID_WIDTH Fld(1,24,AC_MSKB3)//[24:24]
    #define IPTG_V_GRID Fld(8,16,AC_FULLB2)//[23:16]
    #define IPTG_BORDER_EN Fld(4,12,AC_MSKB1)//[15:12]
    #define IPTG_MAX_F Fld(4,8,AC_MSKB1)//[11:8]
    #define IPTG_H_GRID Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_05 (IO_MDDI_BASE + 0x114)
    #define IPTG_HV_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define IPTG_FC_CR Fld(10,20,AC_MSKW32)//[29:20]
    #define IPTG_FC_CB Fld(10,10,AC_MSKW21)//[19:10]
    #define IPTG_FC_Y Fld(10,0,AC_MSKW10)//[9:0]
#define MCVP_KC_06 (IO_MDDI_BASE + 0x118)
    #define IPTG_COLOR_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define IPTG_BC_CR Fld(10,20,AC_MSKW32)//[29:20]
    #define IPTG_BC_CB Fld(10,10,AC_MSKW21)//[19:10]
    #define IPTG_BC_Y Fld(10,0,AC_MSKW10)//[9:0]
#define MCVP_KC_07 (IO_MDDI_BASE + 0x11C)
    #define IPTG_PRB_EN Fld(4,28,AC_MSKB3)//[31:28]
    #define IPTG_PRB_Y_POS Fld(12,16,AC_MSKW32)//[27:16]
    #define IPTG_PRB_CURSOR_ON Fld(1,15,AC_MSKB1)//[15:15]
    #define IPTG_PRB_OSD_ON Fld(1,14,AC_MSKB1)//[14:14]
    #define IPTG_HV_NO_ALIGN Fld(1,13,AC_MSKB1)//[13:13]
    #define IPTG_PRB_X_POS Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_08 (IO_MDDI_BASE + 0x120)
    #define IPTG_PRB_OUT_DISP_R Fld(1,31,AC_MSKB3)//[31:31]
    #define IPTG_PRB_OUT_FLD Fld(1,30,AC_MSKB3)//[30:30]
    #define IPTG_PRB_OUT_CR Fld(10,20,AC_MSKW32)//[29:20]
    #define IPTG_PRB_OUT_CB Fld(10,10,AC_MSKW21)//[19:10]
    #define IPTG_PRB_OUT_Y Fld(10,0,AC_MSKW10)//[9:0]
#define MCVP_KC_09 (IO_MDDI_BASE + 0x124)
    #define SC2MIB_DISP_R_P Fld(1,31,AC_MSKB3)//[31:31]
    #define SC2MIB_DISP_R_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define HBLANK_EXT_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define O_VDEW_X2_FORCE Fld(2,6,AC_MSKB0)//[7:6]
    #define K_VDEW_X2_FORCE Fld(2,4,AC_MSKB0)//[5:4]
    #define O_HDEW_X2_FORCE Fld(2,2,AC_MSKB0)//[3:2]
    #define K_HDEW_X2_FORCE Fld(2,0,AC_MSKB0)//[1:0]
#define MCVP_KC_0A (IO_MDDI_BASE + 0x128)
    #define FREEZE_ON Fld(1,31,AC_MSKB3)//[31:31]
    #define FREEZE_FLD_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define FREEZE_LR_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define VDEW Fld(12,16,AC_MSKW32)//[27:16]
    #define FRC_LR_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define MUTE_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define CF_BOT_INV Fld(1,13,AC_MSKB1)//[13:13]
    #define HDEW Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_0B (IO_MDDI_BASE + 0x12C)
    #define SIZE_UPDATE Fld(1,31,AC_MSKB3)//[31:31]
    #define PXLW_CRC_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLAGW_CRC_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define IN_ADJ_INT_LINE Fld(13,16,AC_MSKW32)//[28:16]
    #define IN_HS_REGULAR Fld(1,15,AC_MSKB1)//[15:15]
    #define OUT_ADJ_INT_LINE Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_0C (IO_MDDI_BASE + 0x130)
    #define IN_VT2 Fld(14,16,AC_MSKW32)//[29:16]
    #define IN_HT Fld(16,0,AC_FULLW10)//[15:0]
#define MCVP_KC_0D (IO_MDDI_BASE + 0x134)
    #define TRICK_TVD3D Fld(1,31,AC_MSKB3)//[31:31]
    #define IN_DISP_R Fld(1,30,AC_MSKB3)//[30:30]
    #define IN_FLD Fld(1,29,AC_MSKB3)//[29:29]
    #define IN_VT_CNT Fld(13,16,AC_MSKW32)//[28:16]
    #define REPEAT_FIELD_FOUND Fld(1,15,AC_MSKB1)//[15:15]
    #define DROP_FIELD_FOUND Fld(1,14,AC_MSKB1)//[14:14]
    #define CLEAR_FRC_STATUS Fld(1,13,AC_MSKB1)//[13:13]
    #define IN_V_ACT Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_0E (IO_MDDI_BASE + 0x138)
    #define KC_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define KC_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define IN_DE_H_ACT Fld(14,16,AC_MSKW32)//[29:16]
    #define IN_DE_HT Fld(14,0,AC_MSKW10)//[13:0]
#define MCVP_KC_0F (IO_MDDI_BASE + 0x13C)
    #define FRC_IN_VT Fld(16,16,AC_FULLW32)//[31:16]
    #define FRC_IN_V_ACT Fld(16,0,AC_FULLW10)//[15:0]
#define MCVP_KC_10 (IO_MDDI_BASE + 0x140)
    #define FRC_OUT_VT Fld(16,16,AC_FULLW32)//[31:16]
    #define FRC_OUT_V_ACT Fld(16,0,AC_FULLW10)//[15:0]
#define MCVP_KC_11 (IO_MDDI_BASE + 0x144)
    #define SRCW_CRC_READY_CLR Fld(1,31,AC_MSKB3)//[31:31]
    #define SRCW_CRC_READY Fld(1,30,AC_MSKB3)//[30:30]
    #define SRCW_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define SRCW_CRC_C_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define SRCW_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define MCVP_KC_12 (IO_MDDI_BASE + 0x148)
    #define SRCW_STILL_CHECK_MAX Fld(8,24,AC_FULLB3)//[31:24]
    #define SRCW_CRC_CLIP_V_END Fld(12,12,AC_MSKW21)//[23:12]
    #define SRCW_CRC_CLIP_V_START Fld(12,0,AC_MSKW10)//[11:0]
#define MCVP_KC_13 (IO_MDDI_BASE + 0x14C)
    #define SRCW_STILL_CHECK_TRIG Fld(1,31,AC_MSKB3)//[31:31]
    #define SRCW_CRC_CLIP_H_END Fld(13,16,AC_MSKW32)//[28:16]
    #define SRCW_CRC_CHECK_SUM_MODE Fld(1,13,AC_MSKB1)//[13:13]
    #define SRCW_CRC_CLIP_H_START Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_14 (IO_MDDI_BASE + 0x150)
    #define SRCW_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_KC_15 (IO_MDDI_BASE + 0x154)
    #define SRCW_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_KC_16 (IO_MDDI_BASE + 0x158)
    #define SPAT_COLOR_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define SPAT_PAT_MODE Fld(2,28,AC_MSKB3)//[29:28]
    #define SPAT_BND_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define SPAT_FREERUN_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define SPAT_HOR_LINE_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define SPAT_PAT_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define SPAT_FREERUN_SPEED Fld(4,20,AC_MSKB2)//[23:20]
    #define SPAT_LEFT_SIDE Fld(1,19,AC_MSKB2)//[19:19]
    #define SPAT_ANGLE_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define SPAT_FREQ_SEL Fld(3,13,AC_MSKB1)//[15:13]
    #define SPAT_PAT_DUTY_CYCLE Fld(5,8,AC_MSKB1)//[12:8]
    #define SPAT_YPAT2 Fld(4,4,AC_MSKB0)//[7:4]
    #define SPAT_YPAT1 Fld(4,0,AC_MSKB0)//[3:0]
#define MCVP_KC_17 (IO_MDDI_BASE + 0x15C)
    #define SRAM_CS_ON Fld(1,31,AC_MSKB3)//[31:31]
    #define MCVP_SUB_OUT_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define MCVP_MAIN_OUT_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define MCVP_SUB_IN_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define MCVP_MAIN_IN_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define K_DE_LOW_MIN Fld(4,20,AC_MSKB2)//[23:20]
    #define AUTO_TNR_OFF_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define AUTO_BOB_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define AUTO_TRICK_ON Fld(1,12,AC_MSKB1)//[12:12]
    #define BF_TO_CF Fld(1,10,AC_MSKB1)//[10:10]
    #define AF_TO_CF Fld(1,9,AC_MSKB1)//[9:9]
    #define AF_FROM_SRC Fld(1,8,AC_MSKB1)//[8:8]
    #define ABCD_YC_EN Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_18 (IO_MDDI_BASE + 0x160)
    #define MCVP_SET_RES_FINISH_I Fld(1,31,AC_MSKB3)//[31:31]
    #define MCVP_SET_RES_FINISH_O Fld(1,30,AC_MSKB3)//[30:30]
    #define MCVP_SET_RES_CLEAR_I Fld(1,29,AC_MSKB3)//[29:29]
    #define MCVP_SET_RES_CLEAR_O Fld(1,28,AC_MSKB3)//[28:28]
    #define MCVP_SET_RESOLUTION Fld(1,27,AC_MSKB3)//[27:27]
    #define OUT_STILL_CHECK_DONE Fld(1,26,AC_MSKB3)//[26:26]
    #define SRCR_STILL_CHECK_DONE Fld(1,25,AC_MSKB3)//[25:25]
    #define SRCW_STILL_CHECK_DONE Fld(1,24,AC_MSKB3)//[24:24]
    #define OUT_NON_STILL_CNT Fld(4,20,AC_MSKB2)//[23:20]
    #define SRCR_NON_STILL_CNT Fld(4,16,AC_MSKB2)//[19:16]
    #define SRCW_NON_STILL_CNT Fld(4,12,AC_MSKB1)//[15:12]
    #define FIXED_DA_IP_RP_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FIXED_DA_PROG_RP_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FIXED_DA_RP_DISP_R Fld(1,9,AC_MSKB1)//[9:9]
    #define FIXED_DA_RP_FLD Fld(1,8,AC_MSKB1)//[8:8]
    #define FIXED_DA_RP_C Fld(3,4,AC_MSKB0)//[6:4]
    #define FIXED_DA_RP_Y Fld(3,0,AC_MSKB0)//[2:0]
#define MCVP_KC_19 (IO_MDDI_BASE + 0x164)
    #define FRC_REPEAT_FIELD Fld(1,31,AC_MSKB3)//[31:31]
    #define CF_BOTTOM Fld(1,30,AC_MSKB3)//[30:30]
    #define W2R_DELAY Fld(3,20,AC_MSKB2)//[22:20]
    #define PXL_CRC_W2R_DELAY Fld(3,16,AC_MSKB2)//[18:16]
    #define SRAM_MODE Fld(4,0,AC_MSKB0)//[3:0]
#define MCVP_KC_1A (IO_MDDI_BASE + 0x168)
    #define SRCRW_CRC_BOT_L_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define SRCRW_CRC_TOP_L_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define SRCRW_CRC_BOT_R_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define SRCRW_CRC_TOP_R_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define SRCR_CRC_READY Fld(1,27,AC_MSKB3)//[27:27]
    #define SRCR_CRC_READY_CLR Fld(1,26,AC_MSKB3)//[26:26]
    #define SRCR_CRC_ERR_STATUS Fld(1,25,AC_MSKB3)//[25:25]
    #define SRCR_CRC_ERR_CLR Fld(1,24,AC_MSKB3)//[24:24]
    #define SRCR_CRC_AUTO_FIELD_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define SRCR_CRC_SEL_DET Fld(1,22,AC_MSKB2)//[22:22]
    #define SRCR_CRC_SEL_FIELD Fld(2,20,AC_MSKB2)//[21:20]
    #define SRCR_CRC_C_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define SRCR_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define MCVP_KC_1B (IO_MDDI_BASE + 0x16C)
    #define SRCR_STILL_CHECK_MAX Fld(8,24,AC_FULLB3)//[31:24]
    #define SRCR_CRC_CLIP_V_END Fld(12,12,AC_MSKW21)//[23:12]
    #define SRCR_CRC_CLIP_V_START Fld(12,0,AC_MSKW10)//[11:0]
#define MCVP_KC_1C (IO_MDDI_BASE + 0x170)
    #define SRCR_STILL_CHECK_TRIG Fld(1,31,AC_MSKB3)//[31:31]
    #define SRCR_CRC_CLIP_H_END Fld(13,16,AC_MSKW32)//[28:16]
    #define SRCR_CRC_CHECK_SUM_MODE Fld(1,13,AC_MSKB1)//[13:13]
    #define SRCR_CRC_CLIP_H_START Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_1D (IO_MDDI_BASE + 0x174)
    #define SRCR_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_KC_1E (IO_MDDI_BASE + 0x178)
    #define PAT3D_SIZE_RGT Fld(4,28,AC_MSKB3)//[31:28]
    #define PAT3D_SIZE_LFT Fld(4,24,AC_MSKB3)//[27:24]
    #define DISP_R_OUT_P Fld(1,18,AC_MSKB2)//[18:18]
    #define M444 Fld(1,17,AC_MSKB2)//[17:17]
    #define M422 Fld(1,16,AC_MSKB2)//[16:16]
    #define PAT3D_EN_RGT Fld(1,15,AC_MSKB1)//[15:15]
    #define PAT3D_EN_LFT Fld(1,14,AC_MSKB1)//[14:14]
    #define NON_STD_VMASK_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define BORDER_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define PAT_GEN_MODE_CR Fld(3,8,AC_MSKB1)//[10:8]
    #define PAT_GEN_MODE_CB Fld(3,4,AC_MSKB0)//[6:4]
    #define PAT_GEN_MODE_Y Fld(3,0,AC_MSKB0)//[2:0]
#define MCVP_KC_1F (IO_MDDI_BASE + 0x17C)
    #define OUT_TG_V_BACK_PORCH Fld(1,31,AC_MSKB3)//[31:31]
    #define OUT_TG_RESTART Fld(1,29,AC_MSKB3)//[29:29]
    #define OUT_TG_VT Fld(13,16,AC_MSKW32)//[28:16]
    #define MAIN_PIP_OUT_SYNC Fld(1,15,AC_MSKB1)//[14:14]
    #define OUT_TG_MODE Fld(1,14,AC_MSKB1)//[14:14]
    #define OUT_TG_H_BLANK Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_20 (IO_MDDI_BASE + 0x180)
    #define PRB_TIME_MODE Fld(4,28,AC_MSKB3)//[31:28]
    #define PRB_Y_POS Fld(12,16,AC_MSKW32)//[27:16]
    #define PRB_CURSOR_ON Fld(1,15,AC_MSKB1)//[15:15]
    #define PRB_CURSOR_BLINK Fld(1,14,AC_MSKB1)//[14:14]
    #define PRB_X_POS Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_21 (IO_MDDI_BASE + 0x184)
    #define OSD_MODE Fld(4,16,AC_MSKB2)//[19:16]
    #define OSD_Y_POS Fld(8,8,AC_FULLB1)//[15:8]
    #define OSD_X_POS Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_22 (IO_MDDI_BASE + 0x188)
    #define OUT_CRC_READY Fld(1,31,AC_MSKB3)//[31:31]
    #define PQ_SRC_DISP_R Fld(1,30,AC_MSKB3)//[30:30]
#define MCVP_KC_23 (IO_MDDI_BASE + 0x18C)
    #define PRB_BOTTOM Fld(1,31,AC_MSKB3)//[31:31]
    #define PRB_DISP_R Fld(1,30,AC_MSKB3)//[30:30]
    #define PRB_V_OUT Fld(10,20,AC_MSKW32)//[29:20]
    #define PRB_C_OUT Fld(10,10,AC_MSKW21)//[19:10]
    #define PRB_Y_OUT Fld(10,0,AC_MSKW10)//[9:0]
#define MCVP_KC_24 (IO_MDDI_BASE + 0x190)
    #define OUT_CRC_SRC_ONLY Fld(1,31,AC_MSKB3)//[31:31]
    #define OUT_CRC_READY_CLR Fld(1,30,AC_MSKB3)//[30:30]
    #define OUT_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define OUT_CRC_C_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define OUT_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define MCVP_KC_25 (IO_MDDI_BASE + 0x194)
    #define OUT_STILL_CHECK_MAX Fld(8,24,AC_FULLB3)//[31:24]
    #define OUT_CRC_CLIP_V_END Fld(12,12,AC_MSKW21)//[23:12]
    #define OUT_CRC_CLIP_V_START Fld(12,0,AC_MSKW10)//[11:0]

//Page KC_2
#define MCVP_KC_26 (IO_MDDI_BASE + 0x198)
    #define OUT_STILL_CHECK_TRIG Fld(1,31,AC_MSKB3)//[31:31]
    #define OUT_CRC_CLIP_H_END Fld(13,16,AC_MSKW32)//[28:16]
    #define OUT_CRC_R_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define OUT_CRC_L_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define OUT_CRC_CHECK_SUM_MODE Fld(1,13,AC_MSKB1)//[13:13]
    #define OUT_CRC_CLIP_H_START Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_27 (IO_MDDI_BASE + 0x19C)
    #define OUT_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_KC_28 (IO_MDDI_BASE + 0x1A0)
    #define OUT_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_KC_29 (IO_MDDI_BASE + 0x1A4)
    #define NUM_OSD_ON Fld(1,31,AC_MSKB3)//[31:31]
    #define NUM_PRB_POS_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define NUM_OSD_MODE Fld(4,26,AC_MSKB3)//[29:26]
    #define NUM_SHIFT Fld(3,23,AC_MSKW32)//[25:23]
    #define NUM_OSD_X_POS Fld(8,8,AC_FULLB1)//[15:8]
    #define NUM_OSD_Y_POS Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_2A (IO_MDDI_BASE + 0x1A8)
    #define FW_OSD_FLAG Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_KC_2B (IO_MDDI_BASE + 0x1AC)
    #define DISPMODE_H_BLANK Fld(8,16,AC_FULLB2)//[23:16]
    #define DIS_BLK_DE_LIMIT Fld(1,8,AC_MSKB1)//[8:8]
    #define I2P_H_BLANK Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_2C (IO_MDDI_BASE + 0x1B0)
    #define STA_MASKED_DE_DN Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_MASKED_DE_UP Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_IN_DE_LOW Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_2D (IO_MDDI_BASE + 0x1B4)
    #define PAL_C_AVG_INK_UV_SWAP Fld(1,24,AC_MSKB3)//[24:24]
    #define PAL_C_AVG_T_W_8_INK_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define PAL_C_AVG_T_W_7_INK_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define PAL_C_AVG_T_W_6_INK_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define PAL_C_AVG_T_W_5_INK_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define PAL_C_AVG_T_W_4_INK_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define PAL_C_AVG_T_W_3_INK_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define PAL_C_AVG_T_W_2_INK_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define PAL_C_AVG_T_W_1_INK_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define PAL_C_AVG_T_W_0_INK_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define OUT_VDE_CNT Fld(13,0,AC_MSKW10)//[12:0]
#define MCVP_KC_2E (IO_MDDI_BASE + 0x1B8)
    #define OUT_TG_DE_N Fld(12,12,AC_MSKW21)//[23:12]
    #define OUT_TG_DE_M Fld(12,0,AC_MSKW10)//[11:0]
#define MCVP_KC_2F (IO_MDDI_BASE + 0x1BC)
    #define PAL_C_AVG_HALF Fld(1,31,AC_MSKB3)//[31:31]
    #define PAL_C_AVG_3_LINE_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define PAL_C_AVG_BLEND_MIN Fld(8,16,AC_FULLB2)//[23:16]
    #define PAL_C_AVG_BLEND_RATIO Fld(6,8,AC_MSKB1)//[13:8]
    #define PAL_C_AVG_MAX_DIFF_VALUE Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_30 (IO_MDDI_BASE + 0x1C0)
    #define MEM_PAT_TYPE_Y Fld(3,12,AC_MSKB1)//[14:12]
    #define MEM_PAT_CHROMA_OFF Fld(1,11,AC_MSKB1)//[11:11]
    #define MEM_PAT_TYPE_C Fld(3,8,AC_MSKB1)//[10:8]
    #define MEM_PAT_ABCD_EN Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_KC_33 (IO_MDDI_BASE + 0x1CC)
    #define DIS_ISYNC_AUTO_BOB_NRW_SEL Fld(1,31,AC_MSKB3)//[31:31]    
    #define FAST_PRE_FETCH           Fld(1,30,AC_MSKB3) //[30:30]
    #define KC_RSV1                  Fld(2,28,AC_MSKB3)//[29:28]
    #define FAST_K_DE_LOW_MIN        Fld(4,24,AC_MSKB3) //[27:24]
    #define KC_RSV                   Fld(13,11,AC_MSKW21)//[23:11]
    #define OSYNC_CTRL_BY_SCL        Fld(1,13,AC_MSKB1) //[13:13]
    #define DUALDI_OUTBUF_OPTION     Fld(2,11,AC_MSKB1) //[12:11]
    #define FRAME_START_SLAVE_MODE   Fld(1,10,AC_MSKB1) //[10:10]
    #define LINE_START_SLAVE_MODE    Fld(1, 9,AC_MSKB1) //[9:9]
    #define PROG_LAST_PIXEL_OPT      Fld(1, 8,AC_MSKB1) //[8:8]
    #define HDEW_ALIGN8_INC_EN       Fld(1, 7,AC_MSKB0) //[7:7]
    #define DIS_REPEAT_SET_RES       Fld(1, 6,AC_MSKB0) //[6:6]
    #define I2P_TOP_VS_FALL_EN       Fld(1, 5,AC_MSKB0) //[5:5]
    #define OS_RST_NO_DELAY          Fld(1, 4,AC_MSKB0) //[4:4]
    #define AUTO_ISYNC_FREEZE_CS_OFF Fld(1, 3,AC_MSKB0) //[3:3]
    #define I2P_HT_DIV2_SEL          Fld(1, 2,AC_MSKB0) //[2:2]
    #define I2P_RST_SEL              Fld(2, 0,AC_MSKB0) //[1:0]
#define MCVP_KC_3E (IO_MDDI_BASE + 0x1F8)
    #define MCNR_DITHER_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_DITHER_BYPASS Fld(1,30,AC_MSKB3)//[30:30]
    #define MCNR_DITHER_FPHASE_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define MCNR_DITHER_FPHASE Fld(6,23,AC_MSKW32)//[28:23]
    #define MCNR_DITHER_FPHASE_CTRL Fld(2,21,AC_MSKB2)//[22:21]
    #define MCNR_DITHER_FPHASE_SEL Fld(2,19,AC_MSKB2)//[20:19]
    #define MCNR_DITHER_FPHASE_BIT Fld(3,16,AC_MSKB2)//[18:16]
    #define MCNR_DITHER_TABLE_EN Fld(2,14,AC_MSKB1)//[15:14]
    #define MCNR_DITHER_SUBSAMPLE_MODE Fld(2,12,AC_MSKB1)//[13:12]
    #define MCNR_ROUND_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define INK_Y_COLOR Fld(3,8,AC_MSKB1)//[10:8]
    #define INK_CB_COLOR Fld(3,5,AC_MSKB0)//[7:5]
    #define INK_CR_COLOR Fld(3,2,AC_MSKB0)//[4:2]
    #define MCNR_DITHER_INK_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define MCNR_DITHER_FPHASE_R Fld(1,0,AC_MSKB0)//[0:0]
    
//Page DRAM
#define MCVP_DRAM_00 (IO_MDDI_BASE + 0x060)
    #define DA_ECO_RSV Fld(16,16,AC_FULLW32)//[31:16]
    #define DA_PIP_RD_CK_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define DA_MAIN_RD_CK_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define DA_MAIN_SC_CK_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DA_MAIN_NR_CK_EN Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_DRAM_01 (IO_MDDI_BASE + 0x064)
    #define DA_FLAG_OVERFLOW_CLR Fld(1,18,AC_MSKB2)//[18:18]
    #define DA_FLAG_UNDERFLOW_CLR Fld(1,17,AC_MSKB2)//[17:17]
    #define DA_FLAG_STATUS_CLR Fld(1,16,AC_MSKB2)//[16:16]
    #define DA_FLAG_MV_HALF_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define DA_FLAG_FORCE_FRAME_IDX Fld(2,4,AC_MSKB0)//[5:4]
    #define DA_FLAG_RNR_MODE Fld(2,2,AC_MSKB0)//[3:2]
    #define DA_FLAG_LINE_FETCH Fld(1,1,AC_MSKB0)//[1:1]
    #define DA_FLAG_FIX_FRAME_IDX Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_DRAM_02 (IO_MDDI_BASE + 0x068)
    #define DA_FLAG_MV_ADDR Fld(27,4,AC_MSKDW)//[30:4]
#define MCVP_DRAM_03 (IO_MDDI_BASE + 0x06C)
    #define DA_FLAG_LM_ADDR Fld(27,4,AC_MSKDW)//[30:4]
#define MCVP_DRAM_04 (IO_MDDI_BASE + 0x070)
    #define DA_FLAG_RNR_ADDR Fld(27,4,AC_MSKDW)//[30:4]
#define MCVP_DRAM_05 (IO_MDDI_BASE + 0x074)
    #define DA_FLAG_WADDR_HI_LIMIT Fld(27,4,AC_MSKDW)//[30:4]
#define MCVP_DRAM_06 (IO_MDDI_BASE + 0x078)
    #define DA_FLAG_WADDR_LO_LIMIT Fld(27,4,AC_MSKDW)//[30:4]
#define MCVP_DRAM_07 (IO_MDDI_BASE + 0x07C)
    #define STA_FLAG_WR_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_FLAG_RD_MAX_RESPONSE Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_FLAG_WRITE_OOB Fld(1,3,AC_MSKB0)//[3:3]
    #define STA_FLAG_OVERFLOW Fld(1,2,AC_MSKB0)//[2:2]
    #define STA_FLAG_UNDERFLOW Fld(1,1,AC_MSKB0)//[1:1]
    #define STA_FLAG_REQ_ERR Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_DRAM_08 (IO_MDDI_BASE + 0x080)
    #define DA_FRAME_NUM_V Fld(3,28,AC_MSKB3)//[30:28]
    #define DA_FRAME_NUM_U Fld(3,24,AC_MSKB3)//[26:24]
    #define DA_FRAME_NUM_Y Fld(3,20,AC_MSKB2)//[22:20]
    #define DA_RD_SHORT_BURST_MODE Fld(1,19,AC_MSKB2)//[19:19]
    #define DA_WR_SHORT_BURST_MODE Fld(1,18,AC_MSKB2)//[18:18]
    #define DA_MODE_YUV Fld(2,16,AC_MSKB2)//[17:16]
    #define DA_DISPLAY_MODE Fld(2,14,AC_MSKB1)//[15:14]
    #define DA_PIP_POP Fld(1,13,AC_MSKB1)//[13:13]
    #define DA_MAIN_ONLY Fld(1,12,AC_MSKB1)//[12:12]
    #define STA_OVERFLOW Fld(1,10,AC_MSKB1)//[10:10]
    #define STA_UNDERFLOW Fld(1,9,AC_MSKB1)//[9:9]
    #define STA_WRITE_OOB Fld(1,8,AC_MSKB1)//[8:8]
    #define DA_OVERFLOW_CLR Fld(1,7,AC_MSKB0)//[7:7]
    #define DA_UNDERFLOW_CLR Fld(1,6,AC_MSKB0)//[6:6]
    #define DA_STATUS_CLR Fld(1,5,AC_MSKB0)//[5:5]
    #define DA_MIRROR_CHROMA_INV Fld(1,4,AC_MSKB0)//[4:4]
    #define DA_SNAKE_MAPPING Fld(1,3,AC_MSKB0)//[3:3]
    #define DA_BIT_SEL_WV Fld(1,2,AC_MSKB0)//[2:2]
    #define DA_BIT_SEL_WU Fld(1,1,AC_MSKB0)//[1:1]
    #define DA_BIT_SEL_WY Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_DRAM_09 (IO_MDDI_BASE + 0x084)
    #define STA_RD_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_WR_MAX_RESPONSE Fld(8,16,AC_FULLB2)//[23:16]
    #define DA_READ_REQ_SPACING Fld(8,8,AC_FULLB1)//[15:8]
    #define DA_BANK_SWAP_PAGE_SIZE Fld(1,5,AC_MSKB0)//[5:5]
    #define DA_BANK_SWAP Fld(1,4,AC_MSKB0)//[4:4]
    #define DA_DRAM_LINE_LENGTH Fld(4,0,AC_MSKB0)//[3:0]
#define MCVP_DRAM_0A (IO_MDDI_BASE + 0x088)
    #define DA_ADDR_BASE_MSB_Y Fld(20,11,AC_MSKDW)//[30:11]
#define MCVP_DRAM_0B (IO_MDDI_BASE + 0x08C)
    #define DA_ADDR_BASE_LSB_Y Fld(20,11,AC_MSKDW)//[30:11]
#define MCVP_DRAM_0C (IO_MDDI_BASE + 0x090)
    #define DA_ADDR_BASE_MSB_U Fld(20,11,AC_MSKDW)//[30:11]
#define MCVP_DRAM_0D (IO_MDDI_BASE + 0x094)
    #define DA_ADDR_BASE_LSB_U Fld(20,11,AC_MSKDW)//[30:11]
#define MCVP_DRAM_0E (IO_MDDI_BASE + 0x098)
    #define DA_ADDR_BASE_MSB_V Fld(20,11,AC_MSKDW)//[30:11]
#define MCVP_DRAM_0F (IO_MDDI_BASE + 0x09C)
    #define DA_ADDR_BASE_LSB_V Fld(20,11,AC_MSKDW)//[30:11]
#define MCVP_DRAM_10 (IO_MDDI_BASE + 0x0A0)
    #define DA_WADDR_HI_LIMIT Fld(20,11,AC_MSKDW)//[30:11]
#define MCVP_DRAM_11 (IO_MDDI_BASE + 0x0A4)
    #define DA_WADDR_LO_LIMIT Fld(20,11,AC_MSKDW)//[30:11]
#define MCVP_DRAM_12 (IO_MDDI_BASE + 0x0A8)
    #define DA_DEBUG Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_DRAM_13 (IO_MDDI_BASE + 0x0AC)
    #define STA_DEBUG Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_DRAM_14 (IO_MDDI_BASE + 0x0B0)
    #define STA_NR_WR_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_NR_OVERFLOW Fld(1,17,AC_MSKB2)//[17:17]
    #define STA_NR_WRITE_OOB Fld(1,16,AC_MSKB2)//[16:16]
    #define DA_NR_3D_TO_2D_LR Fld(1,13,AC_MSKB1)//[13:13]
    #define DA_NR_3D_TO_2D_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define DA_NR_3D_INPUT_LR_SWAP Fld(1,11,AC_MSKB1)//[11:11]
    #define DA_NR_3D_INPUT_MODE Fld(2,9,AC_MSKB1)//[10:9]
    #define DA_NR_3D_INPUT_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define DA_NR_SHORT_BURST_MODE Fld(1,7,AC_MSKB0)//[7:7]
    #define DA_NR_420 Fld(1,6,AC_MSKB0)//[6:6]
    #define DA_NR_YUV_MODE Fld(2,4,AC_MSKB0)//[5:4]
    #define DA_NR_420_SHIFT Fld(1,3,AC_MSKB0)//[3:3]
    #define DA_NR_OVERFLOW_CLR Fld(1,2,AC_MSKB0)//[2:2]
    #define DA_NR_STATUS_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define DA_NR_SNAKE_MAPPING Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_DRAM_15 (IO_MDDI_BASE + 0x0B4)
    #define DRAM_ATPG_OB Fld(1,31,AC_MSKB3)//[31:31]
    #define DRAM_ATPG_CT Fld(1,30,AC_MSKB3)//[30:30]
    #define DA_3D_OUTPUT_LR Fld(1,21,AC_MSKB2)//[21:21]
    #define DA_3D_OUTPUT_FIX_LR Fld(1,20,AC_MSKB2)//[20:20]
    #define DA_3D_OUTPUT_LR_SWAP Fld(1,19,AC_MSKB2)//[19:19]
    #define DA_3D_OUTPUT_MODE Fld(2,17,AC_MSKB2)//[18:17]
    #define DA_3D_OUTPUT_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define DA_3D_FP_LR_SWAP Fld(1,14,AC_MSKB1)//[14:14]
    #define DA_3D_TO_2D_LR Fld(1,13,AC_MSKB1)//[13:13]
    #define DA_3D_TO_2D_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define DA_3D_INPUT_LR_SWAP Fld(1,11,AC_MSKB1)//[11:11]
    #define DA_3D_INPUT_MODE Fld(2,9,AC_MSKB1)//[10:9]
    #define DA_3D_INPUT_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define DA_RD_AGGRESSIVE Fld(1,7,AC_MSKB0)//[7:7]
    #define DA_420_SHIFT Fld(1,6,AC_MSKB0)//[6:6]
    #define DA_PROG_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define DA_SYNC_MODE Fld(1,4,AC_MSKB0)//[4:4]
#define MCVP_DRAM_18 (IO_MDDI_BASE + 0x0C0)
    #define STA_SC_RD_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
    #define DA_SC_READ_REQ_SPACING Fld(8,16,AC_FULLB2)//[23:16]
    #define DA_SC_TST_V Fld(2,14,AC_MSKB1)//[15:14]
    #define DA_SC_TST_U Fld(2,12,AC_MSKB1)//[13:12]
    #define DA_SC_TST_Y Fld(2,10,AC_MSKB1)//[11:10]
    #define DA_SC_DISPLAY_MODE Fld(2,8,AC_MSKB1)//[9:8]
    #define DA_SC_MIRROR_CHROMA_INV Fld(1,7,AC_MSKB0)//[7:7]
    #define DA_SC_BIT_SEL_RV Fld(1,6,AC_MSKB0)//[6:6]
    #define DA_SC_BIT_SEL_RU Fld(1,5,AC_MSKB0)//[5:5]
    #define DA_SC_BIT_SEL_RY Fld(1,4,AC_MSKB0)//[4:4]
    #define DA_SC_SNAKE_MAPPING Fld(1,3,AC_MSKB0)//[3:3]
    #define DA_SC_UNDERFLOW_CLR Fld(1,2,AC_MSKB0)//[2:2]
    #define DA_SC_STATUS_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define DA_SC_READ_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_DRAM_19 (IO_MDDI_BASE + 0x0C4)
    #define DA_SC_3D_OUTPUT_LR Fld(1,5,AC_MSKB0)//[5:5]
    #define DA_SC_3D_OUTPUT_FIX_LR Fld(1,4,AC_MSKB0)//[4:4]
    #define DA_SC_3D_OUTPUT_LR_SWAP Fld(1,3,AC_MSKB0)//[3:3]
    #define DA_SC_3D_OUTPUT_MODE Fld(2,1,AC_MSKB0)//[2:1]
    #define DA_SC_3D_OUTPUT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_DRAM_1A (IO_MDDI_BASE + 0x0C8)
    #define STA_SC_UNDERFLOW Fld(1,31,AC_MSKB3)//[31:31]
    #define DA_SC_AGGRESSIVE Fld(1,29,AC_MSKB3)//[29:29]
    #define DA_SC_SHORT_BURST_MODE Fld(1,28,AC_MSKB3)//[28:28]
    #define DA_SC_MODE_YUV Fld(2,24,AC_MSKB3)//[25:24]
    #define DA_SC_FORCE_V Fld(8,16,AC_FULLB2)//[23:16]
    #define DA_SC_FORCE_U Fld(8,8,AC_FULLB1)//[15:8]
    #define DA_SC_FORCE_Y Fld(8,0,AC_FULLB0)//[7:0]
#define MCVP_DRAM_1B (IO_MDDI_BASE + 0x0CC)
    #define DA_CHSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define DA_ARB_DMMONSEL Fld(4,20,AC_MSKB2)//[23:20]
    #define DA_ARB_DMARBRST Fld(1,17,AC_MSKB2)//[17:17]
    #define DA_ARB_DMTIMARBEN Fld(1,16,AC_MSKB2)//[16:16]
    #define DA_ARB_DMAGBMEND Fld(1,15,AC_MSKB1)//[15:15]
    #define DA_ARB_DMAGBMAGID Fld(3,12,AC_MSKB1)//[14:12]
    #define DA_ARB_DMAGBMALL Fld(1,9,AC_MSKB1)//[9:9]
    #define DA_ARB_DMAGBMBEG Fld(1,8,AC_MSKB1)//[8:8]
    #define DA_ARB_PROG_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define DA_ARB_PIPR_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define DA_ARB_MAINR_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define DA_ARB_PIPW_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DA_ARB_NRW_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define DA_ARB_MAINW_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define DA_ARB_FLAGR_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DA_ARB_FLAGW_EN Fld(1,0,AC_MSKB0)//[0:0]
#define MCVP_DRAM_1C (IO_MDDI_BASE + 0x0D0)
    #define DA_ARB_PROG_TIM Fld(4,28,AC_MSKB3)//[31:28]
    #define DA_ARB_PIPR_TIM Fld(4,24,AC_MSKB3)//[27:24]
    #define DA_ARB_MAINR_TIM Fld(4,20,AC_MSKB2)//[23:20]
    #define DA_ARB_PIPW_TIM Fld(4,16,AC_MSKB2)//[19:16]
    #define DA_ARB_NRW_TIM Fld(4,12,AC_MSKB1)//[15:12]
    #define DA_ARB_MAINW_TIM Fld(4,8,AC_MSKB1)//[11:8]
    #define DA_ARB_FLAGR_TIM Fld(4,4,AC_MSKB0)//[7:4]
    #define DA_ARB_FLAGW_TIM Fld(4,0,AC_MSKB0)//[3:0]
#define MCVP_DRAM_1D (IO_MDDI_BASE + 0x0D4)
    #define BM_LENGTH_COUNTER Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_DRAM_1E (IO_MDDI_BASE + 0x0D8)
    #define BM_CYCLE_COUNTER Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_DRAM_1F (IO_MDDI_BASE + 0x0DC)
    #define MEM_CRC0 Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_DRAM_20 (IO_MDDI_BASE + 0x0E0)
    #define MEM_CRC1 Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_DRAM_21 (IO_MDDI_BASE + 0x0E4)
    #define MEM_CRC2 Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_DRAM_22 (IO_MDDI_BASE + 0x0E8)
    #define MEM_CRC3 Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_DRAM_23 (IO_MDDI_BASE + 0x0F0)
    #define REQ_CRC Fld(32,0,AC_FULLDW)//[31:0]
#define MCVP_DRAM_24 (IO_MDDI_BASE + 0x0F4)
    #define REQ_CHKSUM Fld(32,0,AC_FULLDW)//[31:0]

    /* H.264 */
#define MCVP_H264_00 (IO_MDDI_BASE + 0x8B4)
    #define DA_H264_FRAME_MODE Fld(1, 1, AC_MSKB0) //1
    #define DA_WRITE_MODE Fld(1, 0, AC_MSKB0) //0
#define MCVP_H264_01 (IO_MDDI_BASE + 0x8B8)
    #define DA_H264_ADDR_BASE_Y Fld(27, 4, AC_MSKDW) //4:30
#define MCVP_H264_02 (IO_MDDI_BASE + 0x8BC)
    #define DA_H264_ADDR_BASE_C Fld(27, 4, AC_MSKDW) //4:30

#endif
