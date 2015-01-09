
#ifndef _HW_TDSHARP_H_
#define _HW_TDSHARP_H_

#include "hw_vdoin.h"

#define IO_POSTPROC_BASE  (VDOIN_BASE+0x7000)



//Page SHARPNESS_1
#define SHARP_00 (IO_POSTPROC_BASE + 0x000)
    #define TDS_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define TDS_BLUR Fld(1,29,AC_MSKB3)//[29:29]
    #define TDS_FLT_LTI_V Fld(2,24,AC_MSKB3)//[25:24]
    #define TDS_FLT_LTI_H2 Fld(2,22,AC_MSKB2)//[23:22]
    #define TDS_FLT_LTI_H1 Fld(2,20,AC_MSKB2)//[21:20]
    #define TDS_DISABLE_LTI_V Fld(1,19,AC_MSKB2)//[19:19]
    #define TDS_DISABLE_LTI_H2 Fld(1,18,AC_MSKB2)//[18:18]
    #define TDS_DISABLE_LTI_H1 Fld(1,17,AC_MSKB2)//[17:17]
    #define TDS_DISABLE_V3 Fld(1,16,AC_MSKB2)//[16:16]
    #define TDS_DISABLE_V2 Fld(1,15,AC_MSKB1)//[15:15]
    #define TDS_DISABLE_V1 Fld(1,14,AC_MSKB1)//[14:14]
    #define TDS_DISABLE_D3 Fld(1,13,AC_MSKB1)//[13:13]
    #define TDS_DISABLE_D2 Fld(1,12,AC_MSKB1)//[12:12]
    #define TDS_DISABLE_D1 Fld(1,11,AC_MSKB1)//[11:11]
    #define TDS_DISABLE_H3 Fld(1,10,AC_MSKB1)//[10:10]
    #define TDS_DISABLE_H2 Fld(1,9,AC_MSKB1)//[9:9]
    #define TDS_DISABLE_H1 Fld(1,8,AC_MSKB1)//[8:8]
    #define SHP_COMBINE_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define TDS_FLT_V_3D Fld(2,4,AC_MSKB0)//[5:4]
    #define TDS_INK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define SHARP_03 (IO_POSTPROC_BASE + 0x00C)
    #define SHP_H1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_H1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_H1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_H1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_04 (IO_POSTPROC_BASE + 0x010)
    #define SHP_H1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_H1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_H1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_H1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_05 (IO_POSTPROC_BASE + 0x014)
    #define SHP_H1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_H1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_H1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_06 (IO_POSTPROC_BASE + 0x018)
    #define SHP_H1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_H1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_07 (IO_POSTPROC_BASE + 0x01C)
    #define SHP_H2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_H2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_H2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_H2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_08 (IO_POSTPROC_BASE + 0x020)
    #define SHP_H2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_H2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_H2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_H2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_09 (IO_POSTPROC_BASE + 0x024)
    #define SHP_H2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_H2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_H2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_0A (IO_POSTPROC_BASE + 0x028)
    #define SHP_H2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_H2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_0B (IO_POSTPROC_BASE + 0x02C)
    #define SHP_H3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_H3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_H3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_H3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_0C (IO_POSTPROC_BASE + 0x030)
    #define SHP_H3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_H3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_H3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_H3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_0D (IO_POSTPROC_BASE + 0x034)
    #define SHP_H3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_H3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_H3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_0E (IO_POSTPROC_BASE + 0x038)
    #define SHP_H3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_H3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_0F (IO_POSTPROC_BASE + 0x03C)
    #define SHP_V1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_V1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_V1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_V1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_10 (IO_POSTPROC_BASE + 0x040)
    #define SHP_V1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_V1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_V1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_V1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_11 (IO_POSTPROC_BASE + 0x044)
    #define SHP_V1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_V1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_V1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_12 (IO_POSTPROC_BASE + 0x048)
    #define SHP_V1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_V1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_13 (IO_POSTPROC_BASE + 0x04C)
    #define SHP_V2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_V2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_V2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_V2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_14 (IO_POSTPROC_BASE + 0x050)
    #define SHP_V2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_V2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_V2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_V2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_15 (IO_POSTPROC_BASE + 0x054)
    #define SHP_V2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_V2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_V2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_16 (IO_POSTPROC_BASE + 0x058)
    #define SHP_V2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_V2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_17 (IO_POSTPROC_BASE + 0x05C)
    #define SHP_V3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_V3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_V3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_V3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_18 (IO_POSTPROC_BASE + 0x060)
    #define SHP_V3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_V3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_V3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_V3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_19 (IO_POSTPROC_BASE + 0x064)
    #define SHP_V3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_V3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_V3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_1A (IO_POSTPROC_BASE + 0x068)
    #define SHP_V3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_V3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_1B (IO_POSTPROC_BASE + 0x06C)
    #define SHP_D1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_D1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_D1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_D1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_1C (IO_POSTPROC_BASE + 0x070)
    #define SHP_D1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_D1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_D1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_D1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_1D (IO_POSTPROC_BASE + 0x074)
    #define SHP_D1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_D1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_D1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_1E (IO_POSTPROC_BASE + 0x078)
    #define SHP_D1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_D1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_1F (IO_POSTPROC_BASE + 0x07C)
    #define SHP_D2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_D2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_D2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_D2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_20 (IO_POSTPROC_BASE + 0x080)
    #define SHP_D2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_D2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_D2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_D2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_21 (IO_POSTPROC_BASE + 0x084)
    #define SHP_D2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_D2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_D2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_22 (IO_POSTPROC_BASE + 0x088)
    #define SHP_D2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_D2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_23 (IO_POSTPROC_BASE + 0x08C)
    #define SHP_D3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_D3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_D3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_D3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_24 (IO_POSTPROC_BASE + 0x090)
    #define SHP_D3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_D3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_D3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_D3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_25 (IO_POSTPROC_BASE + 0x094)
    #define SHP_D3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_D3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_D3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_26 (IO_POSTPROC_BASE + 0x098)
    #define SHP_D3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_D3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_27 (IO_POSTPROC_BASE + 0x09C)
    #define SHP_COMBINED_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_30 (IO_POSTPROC_BASE + 0x0C0)
    #define SHP_DETAIL_GAIN_MB Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_EDGE_GAIN_MB Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_DETAIL_GAIN_LB Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_EDGE_GAIN_LB Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_31 (IO_POSTPROC_BASE + 0x0C4)
    #define SHP_EDGE_RANK_SEL Fld(2,18,AC_MSKB2)//[19:18]
    #define SHP_DETAIL_RANK_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define SHP_DETAIL_GAIN_HB Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_EDGE_GAIN_HB Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_32 (IO_POSTPROC_BASE + 0x0C8)
    #define SHP_FLAT_STEP Fld(4,28,AC_MSKB3)//[31:28]
    #define SHP_FLAT_TH Fld(6,22,AC_MSKW32)//[27:22]
    #define SHP_DETAIL_STEP Fld(4,18,AC_MSKB2)//[21:18]
    #define SHP_DETAIL_TH Fld(6,12,AC_MSKW21)//[17:12]
    #define SHP_EDGE_STEP Fld(4,8,AC_MSKB1)//[11:8]
    #define SHP_EDGE_TH Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_33 (IO_POSTPROC_BASE + 0x0CC)
    #define SHP_MIN_MAX_HF Fld(2,30,AC_MSKB3)//[31:30]
    #define SHP_MIN_MAX_XS Fld(11,16,AC_MSKW32)//[26:16]
    #define SHP_MIN_MAX_XE Fld(11,0,AC_MSKW10)//[10:0]
#define SHARP_34 (IO_POSTPROC_BASE + 0x0D0)
    #define SHP_MIN_MAX_VF Fld(2,30,AC_MSKB3)//[31:30]
    #define SHP_MIN_MAX_YS Fld(11,16,AC_MSKW32)//[26:16]
    #define SHP_MIN_MAX_YE Fld(11,0,AC_MSKW10)//[10:0]
#define SHARP_35 (IO_POSTPROC_BASE + 0x0D4)
    #define SHP_REGIONAL_MAX Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_REGIONAL_MIN Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_36 (IO_POSTPROC_BASE + 0x0D8)
    #define TDS_SAT_PROC Fld(6,20,AC_MSKW32)//[25:20]
    #define TDS_SAT_CONSERVATIVE Fld(1,17,AC_MSKB2)//[17:17]
    #define TDS_SAT_121 Fld(1,16,AC_MSKB2)//[16:16]
    #define TDS_SAT_WHITE Fld(5,8,AC_MSKB1)//[12:8]
    #define TDS_SAT_BLACK Fld(5,0,AC_MSKB0)//[4:0]
#define SHARP_40 (IO_POSTPROC_BASE + 0x100)
    #define TDS_HV_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define TDS_HV_SLOPE Fld(3,24,AC_MSKB3)//[26:24]
    #define TDS_HV_MIN Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_HV_THR Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_HV_COR Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_41 (IO_POSTPROC_BASE + 0x104)
    #define TDS_AC_LPF_AUTO Fld(1,24,AC_MSKB3)//[24:24]
    #define TDS_AC_LPF_COE_AUTO_STEP Fld(4,20,AC_MSKB2)//[23:20]
    #define TDS_AC_LPF_COE Fld(4,16,AC_MSKB2)//[19:16]
    #define TDS_AC_LPF_HIGH Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_AC_LPF_LOW Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_42 (IO_POSTPROC_BASE + 0x108)
    #define TDS_TAP_MODE Fld(1,22,AC_MSKB2)//[22:22]
    #define TDS_TAP_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define TDS_TAP_CLIP_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define TDS_TAP_LIMIT Fld(6,12,AC_MSKW21)//[17:12]
    #define TDS_TAP_RATIO Fld(5,4,AC_MSKW10)//[8:4]
    #define TDS_TAP_SRATIO Fld(4,0,AC_MSKB0)//[3:0]
#define SHARP_43 (IO_POSTPROC_BASE + 0x10C)
    #define TDS_NEG_EN_H Fld(1,31,AC_MSKB3)//[31:31]
    #define TDS_NEG_RATIO_H Fld(5,24,AC_MSKB3)//[28:24]
    #define TDS_NEG_LIMIT_H Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_NEG_EN_V Fld(1,15,AC_MSKB1)//[15:15]
    #define TDS_NEG_RATIO_V Fld(5,8,AC_MSKB1)//[12:8]
    #define TDS_NEG_LIMIT_V Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_44 (IO_POSTPROC_BASE + 0x110)
    #define TDS_NEG_EN_D Fld(1,15,AC_MSKB1)//[15:15]
    #define TDS_NEG_RATIO_D Fld(5,8,AC_MSKB1)//[12:8]
    #define TDS_NEG_LIMIT_D Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_45 (IO_POSTPROC_BASE + 0x114)
    #define TDS_CLIP_EN_MD Fld(1,31,AC_MSKB3)//[31:31]
    #define TDS_CLIP_TAP_MD Fld(1,30,AC_MSKB3)//[30:30]
    #define TDS_CLIP_RATIO_MD Fld(5,24,AC_MSKB3)//[28:24]
    #define TDS_CLIP_THR_MD Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_CLIP_EN_LH Fld(1,15,AC_MSKB1)//[15:15]
    #define TDS_CLIP_TAP_LH Fld(1,14,AC_MSKB1)//[14:14]
    #define TDS_CLIP_RATIO_LH Fld(5,8,AC_MSKB1)//[12:8]
    #define TDS_CLIP_THR_LH Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_46 (IO_POSTPROC_BASE + 0x118)
    #define TDS_CLIP_EN_LTI_V Fld(1,31,AC_MSKB3)//[31:31]
    #define TDS_CLIP_TAP_LTI_V Fld(1,30,AC_MSKB3)//[30:30]
    #define TDS_CLIP_RATIO_LTI_V Fld(5,24,AC_MSKB3)//[28:24]
    #define TDS_CLIP_THR_LTI_V Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_CLIP_EN_HV Fld(1,15,AC_MSKB1)//[15:15]
    #define TDS_CLIP_TAP_HV Fld(1,14,AC_MSKB1)//[14:14]
    #define TDS_CLIP_RATIO_HV Fld(5,8,AC_MSKB1)//[12:8]
    #define TDS_CLIP_THR_HV Fld(8,0,AC_FULLB0)//[7:0]

//Page SHARPNESS_2
#define SHARP_47 (IO_POSTPROC_BASE + 0x11C)
    #define TDS_YLEV_P000 Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_YLEV_P064 Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_YLEV_P128 Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_YLEV_P192 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_48 (IO_POSTPROC_BASE + 0x120)
    #define TDS_YLEV_P256 Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_YLEV_P320 Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_YLEV_P384 Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_YLEV_P448 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_49 (IO_POSTPROC_BASE + 0x124)
    #define TDS_YLEV_P512 Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_YLEV_P576 Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_YLEV_P640 Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_YLEV_P704 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_4A (IO_POSTPROC_BASE + 0x128)
    #define TDS_YLEV_P768 Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_YLEV_P832 Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_YLEV_P896 Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_YLEV_P960 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_4B (IO_POSTPROC_BASE + 0x12C)
    #define TDS_YLEV_P1024 Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_YLEV_ALPHA Fld(6,16,AC_MSKB2)//[21:16]
    #define TDS_YLEV_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define TDS_YLEV_EN Fld(1,14,AC_MSKB1)//[14:14]
#define SHARP_4C (IO_POSTPROC_BASE + 0x130)
    #define TDS_VERT_TAP_APT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TDS_VERT_TAP_APT_SLOPE Fld(6,24,AC_MSKB3)//[29:24]
    #define TDS_VERT_TAP_APT_EDGE Fld(6,18,AC_MSKB2)//[23:18]
    #define TDS_VERT_TAP_APT_DIFF Fld(6,12,AC_MSKW21)//[17:12]
    #define TDS_VERT_TAP_APT_BACK Fld(6,6,AC_MSKW10)//[11:6]
#define SHARP_4D (IO_POSTPROC_BASE + 0x134)
	#define TDS_HORIZ_TAP_APT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TDS_HORIZ_TAP_APT_SLOPE Fld(6,24,AC_MSKB3)//[29:24]
    #define TDS_HORIZ_TAP_APT_EDGE Fld(6,18,AC_MSKB2)//[23:18]
    #define TDS_HORIZ_TAP_APT_DIFF Fld(6,12,AC_MSKW21)//[17:12]
    #define TDS_HORIZ_TAP_APT_BACK_0 Fld(6,6,AC_MSKW10)//[11:6]
    #define TDS_HORIZ_TAP_APT_BACK_1 Fld(6,0,AC_MSKB0)//[5:0]
#define SHARP_4E (IO_POSTPROC_BASE + 0x138)
    #define TDS_HORIZ_TAP_APT_BACK_2 Fld(6,26,AC_MSKB3)//[31:26]
    #define TDS_HORIZ_TAP_APT_BACK_3 Fld(6,20,AC_MSKW32)//[25:20]
    #define TDS_HORIZ_TAP_APT_BACK_H2 Fld(6,14,AC_MSKW21)//[19:14]
    #define TDS_HORIZ_TAP_APT_EDGE_H2 Fld(6,8,AC_MSKB1)//[13:8]
#define SHARP_50 (IO_POSTPROC_BASE + 0x140)
    #define SHP_LTI_HL_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_LTI_HL_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_LTI_HL_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_LTI_HL_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_51 (IO_POSTPROC_BASE + 0x144)
    #define SHP_LTI_HL_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_LTI_HL_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_LTI_HL_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_LTI_HL_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_52 (IO_POSTPROC_BASE + 0x148)
    #define SHP_LTI_HL_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_LTI_HL_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_LTI_HL_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_53 (IO_POSTPROC_BASE + 0x14C)
    #define SHP_LTI_HL_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_LTI_HL_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_54 (IO_POSTPROC_BASE + 0x150)
    #define SHP_LTI_HH_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_LTI_HH_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_LTI_HH_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_LTI_HH_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_55 (IO_POSTPROC_BASE + 0x154)
    #define SHP_LTI_HH_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_LTI_HH_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_LTI_HH_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_LTI_HH_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_56 (IO_POSTPROC_BASE + 0x158)
    #define SHP_LTI_HH_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_LTI_HH_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_LTI_HH_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_57 (IO_POSTPROC_BASE + 0x15C)
    #define SHP_LTI_HH_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_LTI_HH_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_58 (IO_POSTPROC_BASE + 0x160)
    #define SHP_LTI_V_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_LTI_V_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_LTI_V_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_LTI_V_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_59 (IO_POSTPROC_BASE + 0x164)
    #define SHP_LTI_V_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_LTI_V_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_LTI_V_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_LTI_V_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_5A (IO_POSTPROC_BASE + 0x168)
    #define SHP_LTI_V_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define SHP_LTI_V_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_LTI_V_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_5B (IO_POSTPROC_BASE + 0x16C)
    #define SHP_LTI_V_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_LTI_V_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define SHARP_5C (IO_POSTPROC_BASE + 0x170)
    #define TDS_CLIP_EN_LTI_H2 Fld(1,31,AC_MSKB3)//[31:31]
    #define TDS_CLIP_TAP_LTI_H2 Fld(1,30,AC_MSKB3)//[30:30]
    #define TDS_CLIP_RATIO_LTI_H2 Fld(5,24,AC_MSKB3)//[28:24]
    #define TDS_CLIP_THR_LTI_H2 Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_CLIP_EN_LTI_H1 Fld(1,15,AC_MSKB1)//[15:15]
    #define TDS_CLIP_TAP_LTI_H1 Fld(1,14,AC_MSKB1)//[14:14]
    #define TDS_CLIP_RATIO_LTI_H1 Fld(5,8,AC_MSKB1)//[12:8]
    #define TDS_CLIP_THR_LTI_H1 Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_5D (IO_POSTPROC_BASE + 0x174)
    #define TDS_CLIP_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define TDS_CLIP_TAP Fld(1,14,AC_MSKB1)//[14:14]
    #define TDS_CLIP_RATIO Fld(5,8,AC_MSKB1)//[12:8]
    #define TDS_CLIP_THR Fld(8,0,AC_FULLB0)//[7:0]
#define SHARP_5E (IO_POSTPROC_BASE + 0x178)
    #define SHP_RESERVED Fld(32,0,AC_FULLDW)//[31:0]

//Page CDS_1
#define CDS_00 (IO_POSTPROC_BASE + 0x600)
    #define CDS1_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_MODE_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define CDS1_ALPHA_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define CDS1_D_THETA Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_D_RADIUS Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_01 (IO_POSTPROC_BASE + 0x604)
    #define CDS1_THETA_C1 Fld(10,22,AC_MSKW32)//[31:22]
    #define CDS1_RADIUS_C1 Fld(10,12,AC_MSKW21)//[21:12]
    #define CDS1_BETA Fld(12,0,AC_MSKW10)//[11:0]
#define CDS_02 (IO_POSTPROC_BASE + 0x608)
    #define CDS1_THETA_C2 Fld(10,22,AC_MSKW32)//[31:22]
    #define CDS1_RADIUS_C2 Fld(10,12,AC_MSKW21)//[21:12]
    #define CDS1_MAX_RADIUS_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_03 (IO_POSTPROC_BASE + 0x60C)
    #define CDS1_ALPHA Fld(12,20,AC_MSKW32)//[31:20]
    #define CDS1_EDGE_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define CDS1_EDGE_THRESHOLD Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_EDGE_SLOPE Fld(6,0,AC_MSKB0)//[5:0]
#define CDS_04 (IO_POSTPROC_BASE + 0x610)
    #define CDS1_CONF_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_LPF_STRENGTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_LPF_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define CDS1_MAX_THETA_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_06 (IO_POSTPROC_BASE + 0x618)
    #define CDS1_H1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_H1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_H1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_H1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_07 (IO_POSTPROC_BASE + 0x61C)
    #define CDS1_H1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_H1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_H1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_H1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_08 (IO_POSTPROC_BASE + 0x620)
    #define CDS1_H1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_H1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_H1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_09 (IO_POSTPROC_BASE + 0x624)
    #define CDS1_H1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_H1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_0A (IO_POSTPROC_BASE + 0x628)
    #define CDS1_H2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_H2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_H2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_H2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_0B (IO_POSTPROC_BASE + 0x62C)
    #define CDS1_H2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_H2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_H2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_H2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_0C (IO_POSTPROC_BASE + 0x630)
    #define CDS1_H2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_H2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_H2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_0D (IO_POSTPROC_BASE + 0x634)
    #define CDS1_H2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_H2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_0E (IO_POSTPROC_BASE + 0x638)
    #define CDS1_H3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_H3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_H3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_H3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_0F (IO_POSTPROC_BASE + 0x63C)
    #define CDS1_H3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_H3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_H3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_H3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_10 (IO_POSTPROC_BASE + 0x640)
    #define CDS1_H3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_H3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_H3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_11 (IO_POSTPROC_BASE + 0x644)
    #define CDS1_H3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_H3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_12 (IO_POSTPROC_BASE + 0x648)
    #define CDS1_V1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_V1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_V1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_V1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_13 (IO_POSTPROC_BASE + 0x64C)
    #define CDS1_V1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_V1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_V1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_V1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_14 (IO_POSTPROC_BASE + 0x650)
    #define CDS1_V1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_V1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_V1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_15 (IO_POSTPROC_BASE + 0x654)
    #define CDS1_V1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_V1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_16 (IO_POSTPROC_BASE + 0x658)
    #define CDS1_V2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_V2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_V2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_V2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_17 (IO_POSTPROC_BASE + 0x65C)
    #define CDS1_V2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_V2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_V2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_V2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_18 (IO_POSTPROC_BASE + 0x660)
    #define CDS1_V2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_V2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_V2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_19 (IO_POSTPROC_BASE + 0x664)
    #define CDS1_V2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_V2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_1A (IO_POSTPROC_BASE + 0x668)
    #define CDS1_V3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_V3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_V3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_V3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_1B (IO_POSTPROC_BASE + 0x66C)
    #define CDS1_V3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_V3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_V3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_V3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_1C (IO_POSTPROC_BASE + 0x670)
    #define CDS1_V3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_V3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_V3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_1D (IO_POSTPROC_BASE + 0x674)
    #define CDS1_V3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_V3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_1E (IO_POSTPROC_BASE + 0x678)
    #define CDS1_D1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_D1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_D1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_D1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_1F (IO_POSTPROC_BASE + 0x67C)
    #define CDS1_D1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_D1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_D1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_D1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_20 (IO_POSTPROC_BASE + 0x680)
    #define CDS1_D1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_D1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_D1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_21 (IO_POSTPROC_BASE + 0x684)
    #define CDS1_D1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_D1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_22 (IO_POSTPROC_BASE + 0x688)
    #define CDS1_D2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_D2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_D2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_D2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_23 (IO_POSTPROC_BASE + 0x68C)
    #define CDS1_D2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_D2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_D2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_D2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_24 (IO_POSTPROC_BASE + 0x690)
    #define CDS1_D2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_D2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_D2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_25 (IO_POSTPROC_BASE + 0x694)
    #define CDS1_D2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_D2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_26 (IO_POSTPROC_BASE + 0x698)
    #define CDS1_D3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_D3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_D3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_D3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_27 (IO_POSTPROC_BASE + 0x69C)
    #define CDS1_D3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS1_D3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS1_D3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS1_D3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_28 (IO_POSTPROC_BASE + 0x6A0)
    #define CDS1_D3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS1_D3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_D3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_29 (IO_POSTPROC_BASE + 0x6A4)
    #define CDS1_D3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS1_D3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_2A (IO_POSTPROC_BASE + 0x6A8)
    #define CDS2_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_MODE_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define CDS2_ALPHA_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define CDS2_D_THETA Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_D_RADIUS Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_2B (IO_POSTPROC_BASE + 0x6AC)
    #define CDS2_THETA_C1 Fld(10,22,AC_MSKW32)//[31:22]
    #define CDS2_RADIUS_C1 Fld(10,12,AC_MSKW21)//[21:12]
    #define CDS2_BETA Fld(12,0,AC_MSKW10)//[11:0]
#define CDS_2C (IO_POSTPROC_BASE + 0x6B0)
    #define CDS2_THETA_C2 Fld(10,22,AC_MSKW32)//[31:22]
    #define CDS2_RADIUS_C2 Fld(10,12,AC_MSKW21)//[21:12]
    #define CDS2_MAX_RADIUS_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_2D (IO_POSTPROC_BASE + 0x6B4)
    #define CDS2_ALPHA Fld(12,20,AC_MSKW32)//[31:20]
    #define CDS2_EDGE_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define CDS2_EDGE_THRESHOLD Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_EDGE_SLOPE Fld(6,0,AC_MSKB0)//[5:0]
#define CDS_2E (IO_POSTPROC_BASE + 0x6B8)
    #define CDS2_CONF_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_LPF_STRENGTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_LPF_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define CDS2_MAX_THETA_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_30 (IO_POSTPROC_BASE + 0x6C0)
    #define CDS2_H1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_H1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_H1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_H1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_31 (IO_POSTPROC_BASE + 0x6C4)
    #define CDS2_H1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_H1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_H1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_H1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_32 (IO_POSTPROC_BASE + 0x6C8)
    #define CDS2_H1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_H1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_H1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_33 (IO_POSTPROC_BASE + 0x6CC)
    #define CDS2_H1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_H1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_34 (IO_POSTPROC_BASE + 0x6D0)
    #define CDS2_H2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_H2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_H2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_H2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_35 (IO_POSTPROC_BASE + 0x6D4)
    #define CDS2_H2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_H2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_H2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_H2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_36 (IO_POSTPROC_BASE + 0x6D8)
    #define CDS2_H2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_H2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_H2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_37 (IO_POSTPROC_BASE + 0x6DC)
    #define CDS2_H2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_H2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_38 (IO_POSTPROC_BASE + 0x6E0)
    #define CDS2_H3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_H3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_H3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_H3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_39 (IO_POSTPROC_BASE + 0x6E4)
    #define CDS2_H3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_H3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_H3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_H3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_3A (IO_POSTPROC_BASE + 0x6E8)
    #define CDS2_H3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_H3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_H3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_3B (IO_POSTPROC_BASE + 0x6EC)
    #define CDS2_H3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_H3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_3C (IO_POSTPROC_BASE + 0x6F0)
    #define CDS2_V1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_V1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_V1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_V1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_3D (IO_POSTPROC_BASE + 0x6F4)
    #define CDS2_V1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_V1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_V1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_V1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_3E (IO_POSTPROC_BASE + 0x6F8)
    #define CDS2_V1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_V1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_V1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_3F (IO_POSTPROC_BASE + 0x6FC)
    #define CDS2_V1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_V1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_40 (IO_POSTPROC_BASE + 0x700)
    #define CDS2_V2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_V2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_V2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_V2_Y2 Fld(8,0,AC_FULLB0)//[7:0]

//Page CDS_2
#define CDS_41 (IO_POSTPROC_BASE + 0x704)
    #define CDS2_V2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_V2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_V2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_V2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_42 (IO_POSTPROC_BASE + 0x708)
    #define CDS2_V2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_V2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_V2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_43 (IO_POSTPROC_BASE + 0x70C)
    #define CDS2_V2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_V2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_44 (IO_POSTPROC_BASE + 0x710)
    #define CDS2_V3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_V3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_V3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_V3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_45 (IO_POSTPROC_BASE + 0x714)
    #define CDS2_V3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_V3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_V3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_V3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_46 (IO_POSTPROC_BASE + 0x718)
    #define CDS2_V3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_V3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_V3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_47 (IO_POSTPROC_BASE + 0x71C)
    #define CDS2_V3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_V3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_48 (IO_POSTPROC_BASE + 0x720)
    #define CDS2_D1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_D1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_D1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_D1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_49 (IO_POSTPROC_BASE + 0x724)
    #define CDS2_D1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_D1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_D1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_D1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_4A (IO_POSTPROC_BASE + 0x728)
    #define CDS2_D1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_D1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_D1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_4B (IO_POSTPROC_BASE + 0x72C)
    #define CDS2_D1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_D1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_4C (IO_POSTPROC_BASE + 0x730)
    #define CDS2_D2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_D2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_D2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_D2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_4D (IO_POSTPROC_BASE + 0x734)
    #define CDS2_D2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_D2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_D2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_D2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_4E (IO_POSTPROC_BASE + 0x738)
    #define CDS2_D2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_D2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_D2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_4F (IO_POSTPROC_BASE + 0x73C)
    #define CDS2_D2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_D2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_50 (IO_POSTPROC_BASE + 0x740)
    #define CDS2_D3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_D3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_D3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_D3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_51 (IO_POSTPROC_BASE + 0x744)
    #define CDS2_D3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS2_D3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS2_D3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS2_D3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_52 (IO_POSTPROC_BASE + 0x748)
    #define CDS2_D3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS2_D3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_D3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_53 (IO_POSTPROC_BASE + 0x74C)
    #define CDS2_D3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS2_D3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_54 (IO_POSTPROC_BASE + 0x750)
    #define CDS3_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_MODE_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define CDS3_ALPHA_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define CDS3_D_THETA Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_D_RADIUS Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_55 (IO_POSTPROC_BASE + 0x754)
    #define CDS3_THETA_C1 Fld(10,22,AC_MSKW32)//[31:22]
    #define CDS3_RADIUS_C1 Fld(10,12,AC_MSKW21)//[21:12]
    #define CDS3_BETA Fld(12,0,AC_MSKW10)//[11:0]
#define CDS_56 (IO_POSTPROC_BASE + 0x758)
    #define CDS3_THETA_C2 Fld(10,22,AC_MSKW32)//[31:22]
    #define CDS3_RADIUS_C2 Fld(10,12,AC_MSKW21)//[21:12]
    #define CDS3_MAX_RADIUS_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_57 (IO_POSTPROC_BASE + 0x75C)
    #define CDS3_ALPHA Fld(12,20,AC_MSKW32)//[31:20]
    #define CDS3_EDGE_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define CDS3_EDGE_THRESHOLD Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_EDGE_SLOPE Fld(6,0,AC_MSKB0)//[5:0]
#define CDS_58 (IO_POSTPROC_BASE + 0x760)
    #define CDS3_CONF_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_LPF_STRENGTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_LPF_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define CDS3_MAX_THETA_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_59 (IO_POSTPROC_BASE + 0x764)
    #define CDS3_H1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_H1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_H1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_H1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_5A (IO_POSTPROC_BASE + 0x768)
    #define CDS3_H1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_H1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_H1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_H1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_5B (IO_POSTPROC_BASE + 0x76C)
    #define CDS3_H1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_H1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_H1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_5C (IO_POSTPROC_BASE + 0x770)
    #define CDS3_H1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_H1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_5D (IO_POSTPROC_BASE + 0x774)
    #define CDS3_H2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_H2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_H2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_H2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_5E (IO_POSTPROC_BASE + 0x778)
    #define CDS3_H2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_H2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_H2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_H2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_5F (IO_POSTPROC_BASE + 0x77C)
    #define CDS3_H2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_H2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_H2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_60 (IO_POSTPROC_BASE + 0x780)
    #define CDS3_H2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_H2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_61 (IO_POSTPROC_BASE + 0x784)
    #define CDS3_H3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_H3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_H3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_H3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_62 (IO_POSTPROC_BASE + 0x788)
    #define CDS3_H3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_H3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_H3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_H3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_63 (IO_POSTPROC_BASE + 0x78C)
    #define CDS3_H3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_H3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_H3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_64 (IO_POSTPROC_BASE + 0x790)
    #define CDS3_H3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_H3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_65 (IO_POSTPROC_BASE + 0x794)
    #define CDS3_V1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_V1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_V1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_V1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_66 (IO_POSTPROC_BASE + 0x798)
    #define CDS3_V1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_V1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_V1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_V1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_67 (IO_POSTPROC_BASE + 0x79C)
    #define CDS3_V1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_V1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_V1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_68 (IO_POSTPROC_BASE + 0x7A0)
    #define CDS3_V1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_V1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_69 (IO_POSTPROC_BASE + 0x7A4)
    #define CDS3_V2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_V2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_V2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_V2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_6A (IO_POSTPROC_BASE + 0x7A8)
    #define CDS3_V2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_V2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_V2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_V2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_6B (IO_POSTPROC_BASE + 0x7AC)
    #define CDS3_V2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_V2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_V2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_6C (IO_POSTPROC_BASE + 0x7B0)
    #define CDS3_V2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_V2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_6D (IO_POSTPROC_BASE + 0x7B4)
    #define CDS3_V3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_V3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_V3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_V3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_6E (IO_POSTPROC_BASE + 0x7B8)
    #define CDS3_V3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_V3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_V3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_V3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_6F (IO_POSTPROC_BASE + 0x7BC)
    #define CDS3_V3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_V3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_V3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_70 (IO_POSTPROC_BASE + 0x7C0)
    #define CDS3_V3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_V3_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_71 (IO_POSTPROC_BASE + 0x7C4)
    #define CDS3_D1_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_D1_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_D1_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_D1_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_72 (IO_POSTPROC_BASE + 0x7C8)
    #define CDS3_D1_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_D1_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_D1_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_D1_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_73 (IO_POSTPROC_BASE + 0x7CC)
    #define CDS3_D1_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_D1_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_D1_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_74 (IO_POSTPROC_BASE + 0x7D0)
    #define CDS3_D1_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_D1_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_75 (IO_POSTPROC_BASE + 0x7D4)
    #define CDS3_D2_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_D2_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_D2_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_D2_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_76 (IO_POSTPROC_BASE + 0x7D8)
    #define CDS3_D2_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_D2_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_D2_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_D2_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_77 (IO_POSTPROC_BASE + 0x7DC)
    #define CDS3_D2_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_D2_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_D2_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_78 (IO_POSTPROC_BASE + 0x7E0)
    #define CDS3_D2_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_D2_S34 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_79 (IO_POSTPROC_BASE + 0x7E4)
    #define CDS3_D3_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_D3_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_D3_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_D3_Y2 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_7A (IO_POSTPROC_BASE + 0x7E8)
    #define CDS3_D3_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define CDS3_D3_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define CDS3_D3_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define CDS3_D3_Y4 Fld(8,0,AC_FULLB0)//[7:0]
#define CDS_7B (IO_POSTPROC_BASE + 0x7EC)
    #define CDS3_D3_BYPASS_CP4 Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS3_D3_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_D3_S12 Fld(14,0,AC_MSKW10)//[13:0]
#define CDS_7C (IO_POSTPROC_BASE + 0x7F0)
    #define CDS_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define CDS_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define CDS3_D3_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define CDS3_D3_S34 Fld(14,0,AC_MSKW10)//[13:0]

	// Define AC Curve Fld
	#define SHP_X1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_X2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_Y2 Fld(8,0,AC_FULLB0)//[7:0]
    #define SHP_X3 Fld(8,24,AC_FULLB3)//[31:24]
    #define SHP_Y3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SHP_X4 Fld(8,8,AC_FULLB1)//[15:8]
    #define SHP_Y4 Fld(8,0,AC_FULLB0)//[7:0]
    #define SHP_S01 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_S12 Fld(14,0,AC_MSKW10)//[13:0]
    #define SHP_S23 Fld(14,16,AC_MSKW32)//[29:16]
    #define SHP_S34 Fld(14,0,AC_MSKW10)//[13:0]

//Page CTIR
#define CTIR_00 (IO_POSTPROC_BASE + 0x280)
    #define CTIR_HUE_TIE Fld(1,28,AC_MSKB3)//[28:28]
    #define CTIR_FLPF Fld(1,12,AC_MSKB1)//[12:12]
    #define CTIR_FLPF_SEL Fld(1,11,AC_MSKB1)//[11:11]
    #define CTIR_DX_SGN Fld(1,9,AC_MSKB1)//[9:9]
    #define CTIR_PRT_ENA Fld(1,6,AC_MSKB0)//[6:6]
    #define CTIR_SGN_PRT Fld(1,5,AC_MSKB0)//[5:5]
    #define CTIR_HD Fld(1,4,AC_MSKB0)//[4:4]
    #define CTIR_INK Fld(1,3,AC_MSKB0)//[3:3]
    #define CTIR_ENA Fld(1,0,AC_MSKB0)//[0:0]
#define CTIR_01 (IO_POSTPROC_BASE + 0x284)
    #define CTIR_LPF3 Fld(1,26,AC_MSKB3)//[26:26]
    #define CTIR_LPF3_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define CTIR_LPF2 Fld(1,22,AC_MSKB2)//[22:22]
    #define CTIR_LPF2_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define CTIR_LPF1 Fld(1,18,AC_MSKB2)//[18:18]
    #define CTIR_LPF1_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define CTIR_FIX_SZ Fld(3,12,AC_MSKB1)//[14:12]
    #define CTIR_FIX Fld(1,11,AC_MSKB1)//[11:11]
    #define CTIR_SEXT_LARGE Fld(1,10,AC_MSKB1)//[10:10]
    #define CTIR_SEXT Fld(1,9,AC_MSKB1)//[9:9]
    #define CTIR_SGN Fld(1,8,AC_MSKB1)//[8:8]
    #define CTIR_SGN_TH Fld(8,0,AC_FULLB0)//[7:0]
#define CTIR_02 (IO_POSTPROC_BASE + 0x288)
    #define CTIR_U_STB_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define CTIR_U_STB_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define CTIR_U_STB_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define CTIR_U_WND_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define CTIR_03 (IO_POSTPROC_BASE + 0x28C)
    #define CTIR_V_STB_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define CTIR_V_STB_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define CTIR_V_STB_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define CTIR_V_WND_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define CTIR_04 (IO_POSTPROC_BASE + 0x290)
    #define CTIR_FLAT_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define CTIR_FLAT_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define CTIR_FLAT_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define CTIR_FLAT_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define CTIR_FLAT_TIE Fld(1,5,AC_MSKB0)//[5:5]
    #define CTIR_FLAT_BYPASS Fld(1,4,AC_MSKB0)//[4:4]
    #define CTIR_FLAT_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define CTIR_05 (IO_POSTPROC_BASE + 0x294)
    #define CTIR_COR Fld(8,0,AC_FULLB0)//[7:0]
#define CTIR_07 (IO_POSTPROC_BASE + 0x29C)
    #define CTIR_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define CTIR_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]

//Page MISC
#define SP_MASK_00 (IO_POSTPROC_BASE + 0x340)
    #define CH1_H_DMWINDOW_END Fld(16,16,AC_FULLW32)//[31:16]
    #define CH1_H_DMWINDOW_START Fld(16,0,AC_FULLW10)//[15:0]
#define SP_MASK_01 (IO_POSTPROC_BASE + 0x344)
    #define CH1_V_DMWINDOW_END Fld(16,16,AC_FULLW32)//[31:16]
    #define CH1_V_DMWINDOW_START Fld(16,0,AC_FULLW10)//[15:0]
#define TDPROC_MISC_00 (IO_POSTPROC_BASE + 0x350)
    #define PPCM_SUB_PATGEN_VSE Fld(1,31,AC_MSKB3)//[31:31]
    #define ECTI_DEMO_SWAP Fld(1,26,AC_MSKB3)//[26:26]
    #define ECTI_DEMO_ENA Fld(1,25,AC_MSKB3)//[25:25]
    #define TDS_DEMO_SWAP Fld(1,24,AC_MSKB3)//[24:24]
    #define TDS_DEMO_ENA Fld(1,23,AC_MSKB2)//[23:23]
    #define TDPROC_BYPASS_ALL Fld(1,22,AC_MSKB2)//[22:22]
    #define NO_VERTICAL_SHARP Fld(1,20,AC_MSKB2)//[20:20]
    #define TDPROC_PATGEN_VSEL Fld(1,19,AC_MSKB2)//[19:19]
    #define TDPROC_PATGEN_USEL Fld(1,18,AC_MSKB2)//[18:18]
    #define TDPROC_PATGEN_YSEL Fld(1,17,AC_MSKB2)//[17:17]
    #define PATGEN_CTRL_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define PATGEN_HVSEL Fld(1,12,AC_MSKB1)//[12:12]
    #define TDPROC_PATGEN_DATA_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define TDPROC_PATGEN_MODE Fld(3,8,AC_MSKB1)//[10:8]
    #define TDPROC_DGB Fld(3,0,AC_MSKB0)//[2:0]
#define TDPROC_MISC_01 (IO_POSTPROC_BASE + 0x354)
    #define TDPROC_RND_LD Fld(1,30,AC_MSKB3)//[30:30]
    #define TDPRCO_RND_SEED Fld(30,0,AC_MSKDW)//[29:0]
#define TDPROC_MISC_02 (IO_POSTPROC_BASE + 0x358)
    #define TDPROC_VSW Fld(16,16,AC_FULLW32)//[31:16]
    #define TDPROC_VPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define TDPROC_MISC_03 (IO_POSTPROC_BASE + 0x35C)
    #define TDPROC_HSW Fld(16,16,AC_FULLW32)//[31:16]
    #define TDPROC_HPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define TDPROC_MISC_04 (IO_POSTPROC_BASE + 0x360)
    #define TDPROC_PICH Fld(16,16,AC_FULLW32)//[31:16]
    #define TDPROC_PICW Fld(16,0,AC_FULLW10)//[15:0]
#define TDPROC_MISC_05 (IO_POSTPROC_BASE + 0x364)
    #define TDPROC_INK_YPOS Fld(16,16,AC_FULLW32)//[31:16]
    #define TDPROC_INK_XPOS Fld(16,0,AC_FULLW10)//[15:0]
#define TDPROC_MISC_06 (IO_POSTPROC_BASE + 0x368)
    #define CH1_LCRC_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define CH1_RCRC_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define CH1_Y_CRC_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define CH1_CB_CRC_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define CH1_CR_CRC_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define TDPROC_MISC_07 (IO_POSTPROC_BASE + 0x36C)
    #define CH1_CRC_CLIP_H_END Fld(13,16,AC_MSKW32)//[28:16]
    #define CH1_CRC_CLIP_H_START Fld(13,0,AC_MSKW10)//[12:0]
#define TDPROC_MISC_08 (IO_POSTPROC_BASE + 0x370)
    #define CH1_CRC_CLIP_V_END Fld(12,16,AC_MSKW32)//[27:16]
    #define CH1_CRC_CLIP_V_START Fld(12,0,AC_MSKW10)//[11:0]
#define TDPROC_MISC_09 (IO_POSTPROC_BASE + 0x374)
    #define CH1_CRC_RESULT Fld(32,0,AC_FULLDW)//[31:0]
#define TDPROC_MISC_0D (IO_POSTPROC_BASE + 0x384)
    #define CH2_CRC_RESULT Fld(32,0,AC_FULLDW)//[31:0]
#define TDPROC_MISC_0E (IO_POSTPROC_BASE + 0x388)
    #define CH2_CRC_STILL_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define CH1_CRC_STILL_CNT Fld(16,0,AC_FULLW10)//[15:0]
#define TDPROC_MISC_0F (IO_POSTPROC_BASE + 0x38C)
    #define CORE_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define DISPRCNTEN Fld(1,29,AC_MSKB3)//[29:29]
    #define CRCIN_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define CH1_CRC_STILL_MAX Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BYPASS_DISP_R Fld(1,5,AC_MSKB0)//[5:5]
    #define C_CRC_STILL_CNT_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define CH1_CRC_STILL_TRG Fld(1,2,AC_MSKB0)//[2:2]

//Page CBE
#define CBE_00 (IO_POSTPROC_BASE + 0x2C0)
    #define CBEENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define CBEINKMODE Fld(4,24,AC_MSKB3)//[27:24]
    #define CBE_THR_K7X3_IT3 Fld(7,16,AC_MSKB2)//[22:16]
    #define CBE_W_IT3 Fld(7,8,AC_MSKB1)//[14:8]
    #define CBE_W_IT5 Fld(7,0,AC_MSKB0)//[6:0]
#define CBE_01 (IO_POSTPROC_BASE + 0x2C4)
    #define CBE_THR_K7X3_IT5 Fld(7,8,AC_MSKB1)//[14:8]
    #define CBE_THR_K5X3_IT5 Fld(7,0,AC_MSKB0)//[6:0]
#define CBE_02 (IO_POSTPROC_BASE + 0x2C8)
    #define CBE_THR_K5X3_IT3 Fld(7,8,AC_MSKB1)//[14:8]
    #define CBE_THR_K3X3_IT3 Fld(7,0,AC_MSKB0)//[6:0]
#define CBE_05 (IO_POSTPROC_BASE + 0x2D4)
    #define BYPASS_CBE Fld(1,2,AC_MSKB0)//[2:2]
#endif

