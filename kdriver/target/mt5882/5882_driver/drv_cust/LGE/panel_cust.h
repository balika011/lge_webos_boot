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
/*----------------------------------------------------------------------------
 * PANEL LIST, PLEASE FOLLOW NAMING FORMAT WHEN ADDING NEW PANEL:
 * 1.add model suffix to avoid the different resolution in same panel size
 * 2.add resolution info for user friendly
 * 3.DO NOT change panel index, use PANEL_TOTAL_NUMBER as new panel's index
 *   and increment PANEL_TOTAL_NUMBER
 *---------------------------------------------------------------------------*/

#define PANEL_XGA 0 // 1024x768
#define PANEL_SXGA 1 // 1280x1024

#define PANEL_CMO_154_C1_P1 53 // 1440x900
#define PANEL_CMO_154_I2_L2 54 // 1280x800
#define PANEL_CMO_154_I2_P2 80 // 1280x800
#define PANEL_CMO_154_I2_P1 83// 1280x800
#define PANEL_CMO_154_I5_P2 59 // 1280x800
#define PANEL_CMO_19_A1 2 // 1440x900
#define PANEL_CMO_19_A1_P2 81 // 1440x900
#define PANEL_CMO_22_Z1 22 // 1680x1050
#define PANEL_CMO_22_Z1_P1 82 // 1680x1050
#define PANEL_CMO_26_B1 24 // 1366x768 (LVDS=5-V, BL=24-V)
#define PANEL_CMO_26_B1_L04 74 //1366x768
#define PANEL_CMO_27_W1 3 // 1280x720
#define PANEL_CMO_27_B1 4 // 1366x768
#define PANEL_CMO_315_B1 28 // 1366x768
#define PANEL_CMO_32_B1 5 // 1366x768
#define PANEL_CMO_37_H1 6 // 1920x1080
#define PANEL_CMO_42_H1 7 // 1920x1080
#define PANEL_CMO_42_H1_L5 52 // 1920x1080
#define PANEL_CMO_42_H1_L8 86 // 1920x1080
#define PANEL_CMO_42_H1_L5_DELTA 58 // 1920x1080

#define PANEL_AUO_20_XW2 8 // 1366x768
#define PANEL_AUO_26_XW3 30 // 1366x768 (LVDS=5-V, BL=24-V)
#define PANEL_AUO_315_XW1_VH 62// 1366x768
#define PANEL_AUO_315_XW1_VG 64// 1366x768
#define PANEL_AUO_315_XW2_V0 31 // 1366x768 (LVDS=5-V, BL=24-V)
#define PANEL_AUO_315_XW2_V1 32 // 1366x768 (LVDS=5-V, BL=24-V)
#define PANEL_AUO_315_XW2_V3 48 // 1366x768
#define PANEL_AUO_315_XW2_V6 35 // 1366x768 (LVDS=5-V, BL=24-V) 120Hz
#define PANEL_AUO_32_XW1 9 // 1366x768
#define PANEL_AUO_32_XW2 27 // 1366x768
#define PANEL_AUO_37_XW1_V1 45 // 1366x768
#define PANEL_AUO_37_XW1_V2 46 // 1366x768
#define PANEL_AUO_37_XW2 20 // 1366x768
#define PANEL_AUO_37_XW2_V5 42 // 1366x768
#define PANEL_AUO_37_HW1 10 // 1920x1080
#define PANEL_AUO_40_XW1_V0 60 // 1366x768
#define PANEL_AUO_42_HW1_V1 76 // 1920x1080
#define PANEL_AUO_42_XW1_V3 43 // 1366x768
#define PANEL_AUO_42_HW1_V0 50 // 1920x1080
#define PANEL_AUO_42_HW1_V3 93 // 1920x1080

#define PANEL_LG_26_W1 11 // 1280x768
#define PANEL_LG_26_WX2 19 // 1366x768
#define PANEL_LG_32_W1 12 // 1366x768
#define PANEL_LG_32_WX3_SLB1 36 // 1366x768 (LVDS=12-V, BL=24-V)
#define PANEL_LG_37_WX1 13 // 1366x768
#define PANEL_LG_37_WX1_SL2 41 // 1366x768
#define PANEL_LG_37_WX1_SLA1 37 // 1366x768 (LVDS=12-V, BL=24-V)
#define PANEL_LG_37_WU1 14 // 1920x1080
#define PANEL_LG_42_W2 15 // 1366x768
#define PANEL_LG_42_W2_SLA2 47 // 1366x768
#define PANEL_LG_42_WU2 25 // 1920x1080 (LVDS=12-V, BL=24-V)
#define PANEL_LG_42_WU3_SLA1 51 // 1920x1080
#define PANEL_LG_42_WU5_SLB1 89 // 1920x1080 (120 Hz)
#define PANEL_LG_42_WX4_SLB1 87 // 1366x768 (120 Hz)
#define PANEL_LG_47_WU4 21 // 1920x1080
#define PANEL_LG_50_X4_PDP 29 // 1366x768
#define PANEL_LG_42_WUD_SAC1_10BIT 122 // 1920x1080 (120Hz Panel)

#define PANEL_SS_19_M2 23 // 1440x900 (LVDS=5-V, BL=12-V)
#define PANEL_SS_23_W2_L1 34 // 1366x768
#define PANEL_SS_32_WT_L5 56 // 1366x768
#define PANEL_SS_40_WT 16 // 1366x768
#define PANEL_SS_40_FHD 55 // 1920x1080
#define PANEL_SS_40_FHD_10BIT 88 // 1920x1080
#define PANEL_SS_40_WT_L17 61 // 1366x768
#define PANEL_SS_46_HS_L03 57 // 1920x1080
#define PANEL_SS_46_HS_L03_BESTBUY 77 // 1920x1080
#define PANEL_SS_40_HH_LH2 94 // 1920x1080 (120 HZ)
#define PANEL_SS_46_HH_L01 95 // 1920x1080

#define PANEL_CLAA_15_XP3 17 // 1024x768
#define PANEL_CLAA_32_WB2 38 // 1366x768

#define PANEL_QD_32_HL1 18 // 1366x768
#define PANEL_QD_32_HL1_W 44 // 1366x768
#define PANEL_HSD_19_MGW1 26 // 1440x900 (LVDS=5-V, BL=12-V)

#define PANEL_PV_154_LCM_C01 33 // 1440x900
#define PANEL_PV_154_LCM_C03 79 // 1280x800
#define PANEL_PV_154_LCM_C04 39 // 1280x800
#define PANEL_PV_154_LCM_C05 78 // 1280x800
#define PANEL_PV_201_TVM_C01 40 // 1400x1050
#define PANEL_PV_260_TVM_A01H 72// 1366x768
#define PANEL_PV_260_TVM_A04H 75// 1366x768
#define PANEL_PV_260_TVM_A11H 73// 1366x768
#define PANEL_PV_260_TVM_C01H 70 // 1366x768
#define PANEL_PV_260_TVM_C11H 71// 1366x768
#define PANEL_PV_320_TVM_A01H 49 // 1366x768
#define PANEL_PV_320_TVM_A02H 65 // 1366x768
#define PANEL_PV_320_TVM_A03H 67 // 1366x768
#define PANEL_PV_320_TVM_A12H 66 // 1366x768
#define PANEL_PV_320_TVM_A21H 68 // 1366x768
#define PANEL_PV_320_TVM_C01 85 // 1366x768
#define PANEL_PV_320_TVM_C02H 69 // 1366x768
#define PANEL_PV_320_TVM_C12H 63 // 1366x768

#define PANEL_INNOLUX_19_AW01 84  // 1440x900

#define PANEL_AUO_315_XW2_V6_8280 90 // 1366x768 (LVDS=5-V, BL=24-V) 120Hz
#define PANEL_LG_42_WU5_SLB1_8280 91 // 1920x1080 (120 Hz)
#define PANEL_LG_42_WX4_SLB1_8280 92 // 1366x768 (120 Hz)

#define PANEL_FHD60_LVDS_CAP 	  96 // 1920x1080 (60 Hz)
#define PANEL_FHD120_LVDS_CAP 	  97 // 1920x1080 (120 Hz)
#define PANEL_WXGA60_LVDS_CAP 	  98 // 1366x768 (60 Hz)
#define PANEL_WXGA120_LVDS_CAP    99 // 1366x768 (120 Hz)
#define PANEL_SLT_FHD_60Hz_10BIT       100 // 1920x1080(10bit, 60Hz) for SLT
#define PANEL_SLT_FHD_120Hz_10BIT      101 // 1920x1080(10bit, 120Hz) for SLT

//For SBTVD
#define PANEL_INNOLUX_19_AW02_LIPS_LOW	102  // 1440x900 (LVDS=5-V)
#define PANEL_CMO_216_B1_L01_LIPS_LOW 103 // 1366x768 (LVDS=5-V)
#define PANEL_CMO_260_B1_L11_LIPS_HIGH 104 // 1366x768 (LVDS=12-V)
#define PANEL_AUO_19_PW1 105 //1440x900	
#define PANEL_CMO_216_B1_L1 106 //1366X768
#define PANEL_AUO_37_XW2_VC 107    //WXGA1,  sammi modified 080912
#define PANEL_SS_32_WT_1 108 //WXGA1, add by stanley 080908
#define PANEL_AUO_26_WT_1 109 //WXGA1, add by stanley 080908
#define PANEL_CMO_22_B1_L01 110     //WXGA1,  sammi modified 080912
#define PANEL_AUO_19_XW1 111          //WXGA3,  sammi modified 080912
#define PANEL_SS_52_FHD 112 // 1920x1080
#define PANEL_SS_32_WT_1_WXGA2 113  //WXGA2
#define PANEL_AUO_315_XW2_VS_WXGA2 114  //WXGA2

#define PANEL_SS_32NC_LBA_SESL_WXGA1 115 // WXGA1
#define PANEL_SS_32NC_LBA_SESL_WXGA2 116 // WXGA2
#define PANEL_AUO_T315XW02_VS_WXGA1  117 // WXGA1
#define PANEL_AUO_T315XW02_VS_WXGA2  118 // WXGA2
#define PANEL_SS_32NC_LBA_SESL_FHD   119 // FHD   
#define PANEL_AUO_46_HW03_V4 120 // 1920x1080 (240Hz for MT5395+2xMT8282)
#define PANEL_AUO_65_HW02_V1 121 // 1920x1080 (120Hz PR Panel)

#define PANEL_LG_42_WU5_SLB1_VB1 123 // VB1 1920x1080 (120 Hz)

#define PANEL_MLVDS_LGDV4FHD60     124
#define PANEL_MLVDS_LGDV4FHD120    125
#define PANEL_MLVDS_LGDV4GIPFHD60  126
#define PANEL_MLVDS_LGDV4GIPFHD120 127
#define PANEL_ChangHong_PDP 128
#define PANEL_MLVDS_LGDV5GIPFHD60  129
#define PANEL_MLVDS_LGDV5GIPFHD120 130
#define PANEL_MLVDS_LGDV6GIPFHD60  131
#define PANEL_MLVDS_LGDV6GIPFHD120 132
#define PANEL_MLVDS_AUOFHD60   133
#define PANEL_MLVDS_AUOFHD120   134
#define PANEL_MLVDS_SHAFHD60   135
#define PANEL_MLVDS_SHAFHD120   136
#define PANEL_MLVDS_FUNAWXGA  137
#define PANEL_MLVDS_FUNAFHD60 138
#define PANEL_MLVDS_HIWXGA09 139
#define PANEL_MLVDS_HIWXGA10 140
#define PANEL_MLVDS_WXGA        141
#define PANEL_MLVDS_CMO2011FHD60 142
#define PANEL_MLVDS_PANASONIC32WXGA1P6P 143

#define PANEL_LG_47_EUH_PR 144 // 1920x1080 (120Hz PR Panel)
#define PANEL_CMO_M236H5_L0A 145 // 1920x1080 (120Hz PR Panel)
#define PANEL_LG_42_WUD_SAC1_10BIT_NS 146
#define PANEL_LG_42_WU5_SLB1_PR 147 // 1920x1080 (120Hz PR Panel)
#define PANEL_LG_42_WU5_SLB1_SG 148 // 1920x1080 (120Hz SG Panel)
#define PANEL_CMO_M236H3_LHH3 149 // 1920x1080 (120Hz PR Panel)
#define PANEL_LG_42_WU5_SLB1_AS3693 150 // 1920x1080 (120Hz Panel)
#define PANEL_LG_42_WU5_SLB1_IWATT  151 // 1920x1080 (120Hz Panel)
#define PANEL_LG_42_WU5_SLB1_SG_BEX 152 // [152] 1920x1080 (120Hz SG Panel + Blank Extension)
#define PANEL_AUO_46_HW03_V4_FHD240 153 // [153] 1920x1080 (240Hz for MT5395+2xMT8282)
#define PANEL_AUO_46_HW03_V3        154 // [154] 1920x1080 (120Hz Panel)
#define PANEL_LG_50_CINEMA          155 // [New] 2560x1080 (120Hz Panel)
#define PANEL_FOR_8283      156
#define PANEL_AUO_T576DC01_V1   157 // [New] 2560x1080 (CSHD-120Hz PR Panel)
#define PANEL_CMI_V420H2_LS1   158 // [New] 1920x1080 (FHD-120Hz SG Panel)
#define PANEL_LC420EUD_SDF1_PR   159 // [New] 1920x1080 (FHD-120Hz PR Panel)
#define PANEL_AUO_T645HW05_V0   160 // [New] 1920x1080 (FHD-120Hz PR Panel)
#define PANEL_SS_42_PDP   161 // [New] 1024x768
#define PANEL_LVDS_TO_8282 162
#define PANEL_SS_58_PDP    163 //[New] 1920x1080
#define PANEL_EPI_LGDV12GIPFHD60    164 //[New] 1920x1080 (60Hz Panel)
#define PANEL_AUO_T315HB01_V1    165 //[New] 1920x1080 (60Hz PR Panel)
#define PANEL_CMI_42_HW03_V3        166 // 1920x1080 (120Hz shutter  Panel)
#define PANEL_EPI_LGDV12GIPFHD120    167 //[New] 1920x1080 (120Hz Panel)
#define PANEL_LG_32_PDP 168  // 1366x768 simulation pdp
#define PANEL_SS_L40V8200_120SG 169 // 1920x1080 (120 HZ)
#define PANEL_PDP_PD5L_XGA60 170
#define PANEL_LG_42_WU3_SLA1_HISPD 171
#define PANEL_MLVDS_CMO2011FHD120 172
#define PANEL_MLVDS_SS_MB4 173
#define PANEL_MLVDS_SS_MB7 174
#define PANEL_MLVDS_PANASONIC32WXGA2P3P 175
#define PANEL_VIDEO_1080P 176
#define PANEL_VIDEO_720P 177
#define PANEL_VIDEO_576P 178
#define PANEL_VIDEO_480P 179
#define PANEL_LG_42_WU3_HISPD_2CH  180
#define PANEL_AUO_37_HW2 181 // 1920x1080
#define PANEL_MLVDS_PANASONIC42FHD120 182
#define PANEL_MLVDS_LGD32WXGAV9      183
#define PANEL_SLT_FHD_60Hz_10BIT_AUTO    184 // 1920x1080(10bit, 60Hz) for verification automation
#define PANEL_MLVDS_HISENSE_HE420FF 185 //42' 1920*1080 minilvds
#define PANEL_MLVDS_HISENSE32WXGA2P3P 186 //32' 1366*768(6bit) minilvds
#define PANEL_MLVDS_HISENSE32WXGA1P6P 187 //32' 1366*768(8bit) minilvds
#define PANEL_MLVDS_CHINASTARWXGA8BIT 188  // 1366*768 1port6pairs TRD
#define PANEL_MLVDS_LGD32WXGAV12 189    //1366*768 DRD emmdedded mini-lvds
#define PANEL_MLVDS_LGD32WXGAV13 190    //1366*768 DRD emmdedded mini-lvds
#define PANEL_EPI_LGDV13GIPFHD60    191  //[New] 1920x1080 (60Hz Panel)
#define PANEL_EPI_LGDV13GIPFHD120    192 //[New] 1920x1080 (120Hz Panel)
#define PANEL_EPI_LGDV14GIPFHD60_8LANE    193  //[New] 1920x1080 (60Hz Panel)
#define PANEL_EPI_LGDV14GIPFHD120_8LANE    194 //[New] 1920x1080 (120Hz Panel)
#define PANEL_EPI_LGDV14GIPFHD60_4LANE    195  //[New] 1920x1080 (60Hz Panel) DRD
#define PANEL_THINE_THAV226_FHD60_VB1 196 // VB1 1920x1080(10bit, 60Hz)
#define PANEL_4K2K30_VB1  197 // VB1 3840x2160(30Hz)

#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#define PANEL_AUO_4K2K 198
#define PANEL_AUO_4K2K_FHD60 199
#define PANEL_CMI_4K2K 200
#define PANEL_CMI_4K2K_FHD120 201
#define PANEL_CMO_V580DK1_LS1_INX 202 // 4k2k panel 120Hz SG 3D panel
#define PANEL_CSOT_4K2K_FHD60 203 // 4k2k 55 csot ntk
#define PANEL_CMO_V580DK1_LS1_INX_VB1 204 // 4k2k panel 120Hz SG 3D panel
#define PANEL_CMO_V580DK1_LS1_INX_60_VB1 205 // 4k2k panel 60Hz SG 3D panel
#define PANEL_TOTAL_NUMBER 206
#else
#define PANEL_TOTAL_NUMBER 198
#endif

#if (defined (CC_FRC_SUPPORT_FORCE120_WEB) || defined (CC_FRC_SUPPORT_FORCE100) || defined (CC_FRC_SUPPORT_F48)) //ctrl with build option
    #if defined (CC_FRC_SUPPORT_FORCE120_WEB)
    #define SUPPORT_DISP_50TO120    3 
    #define SUPPORT_DISP_WEB_VIDEO_SMOOTHER 
    #elif defined (CC_FRC_SUPPORT_FORCE100)
    #define SUPPORT_DISP_60TO100    2 
    #endif
#else
    //#define SUPPORT_FORCE_OUTPUT_FRAMERATE_BY_USER 
    #define SUPPORT_DISP_50TO120   0 //(IS_LVDS_DISP_3D_SHUTTER ? 2 : 0)  
    //#define SUPPORT_DISP_60TO100    2
//#define SUPPORT_DISP_WEB_VIDEO_SMOOTHER
#endif

#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
	//#define SUPPORT_NTK_DEMO_BOARD
#endif

#define SUPPORT_PANEL_48HZ           PANEL_IsSupportExtAttribute()
#define SUPPORT_PANEL_CUSTOMER_SPEC
//#define SUPPORT_DISP_48TO60         //48 to 60
#define SUPPORT_CUSTOMER_FRAME_RATE_SPEC

//#define SUPPORT_PANEL_SS
//#define SUPPORT_PANEL_DITHER
//#define SUPPORT_PANEL_DRIVING
//#define SUPPORT_PANEL_ERRORLIMIT_TABLE
//#define SUPPORT_PANEL_3D_PIXEL_SIZE
//#define SUPPORT_AS3693_SPI_SPEC    //mtk70763 , 20100810 , SPI driver porting
#ifdef CC_LGE_PROTO_PCBA
#define SUPPORT_PANEL_SCAN_PWM
#else
//#define SUPPORT_PANEL_SCAN_PWM
#endif
//#define SUPPORT_DGO
//#define SUPPORT_PANEL_GRAPH_MODE

#if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#define PANEL_4K2K_FHD120 0
#define PANEL_4K2K_FHD60  1
#define PANEL_4K2K_30     2
#endif

#ifdef SUPPORT_PANEL_GRAPH_MODE
#define GRAPH_TYPE_4K2K 0
#define GRAPH_TYPE_4K1K 1
#endif

#ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
#define LOAD_PANEL_FROM_FLASH_BY_ID     0
#define LOAD_PANEL_FROM_FLASH_BY_NAME     1

BOOL MTK_PanelLoadTableFromFlash(UINT32 u4Type, void *pData);

CHAR * FlashPanel_GetVersion(void);
CHAR * FlashPanel_GetIDString(void);
UINT32 FlashPanel_GetDither(void);
UINT32 FlashPanel_GetAQIndex(UINT32 u4Idx);
UINT32 FlashPanel_GetPQIndex(UINT32 u4Idx);
#endif

#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
extern UINT32 PANEL_GetFrameDelay(void);
#endif //SUPPORT_PANEL_CUSTOMER_SPEC


