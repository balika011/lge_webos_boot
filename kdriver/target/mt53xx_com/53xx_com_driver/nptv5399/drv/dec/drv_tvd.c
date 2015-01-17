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
*   $Workfile: drv_tvd.c $
*
* Project:
* --------
*   MT8205
*
* Description:
* ------------
*   TVD Driver
*
* Author:
* -------
*   Cindy Huang
*
* Last changed:
* -------------
* $Author: p4admin $
*
* $Modtime: 04/06/01 6:05p $
*
* $Revision: #2 $
****************************************************************************/
/**
* @file drv_tvd.c
*/
#define _DRV_TVD_C_
/**************************************************************************
 * Header Files
 *************************************************************************/
#ifndef CC_UP8032_ATV  //For 53xx
#include "x_bim.h"
#include "x_assert.h"
#ifdef CC_COPLAT_MT82
#include "general.h"
#endif
#else
#include "general_mt82.h" //For 8032
#include "x_hal_5381.h"
#include "general.h"
#endif

#ifndef CC_COPLAT_MT82
#include "x_os.h"
#endif

#include "hw_vdoin.h"
#include "hw_sys.h"
#include "drv_tvd.h"
#include "video_def.h"
#include "source_select.h"
#if SUPPORT_SCART
#include "drv_scart.h"
#endif
#include "drv_nr.h"
#include "drv_di.h"
#include "drv_tdc.h"
//#include "nptv_debug.h"
#include "util.h"
#include "hw_tdc.h"
#include "hw_scart.h"
#include "x_lint.h"
#include "api_notify.h"
#include "drv_video.h"
#include "nptv_drvif.h"
#include "eepdef.h"
#include "drvcust_if.h"
#include "x_util.h"
#include "api_eep.h"
#include "vdo_misc.h"
#if SUPPORT_VBI
#include "drv_vbi.h"
#endif
#include "tuner_if.h"
#ifdef SUPPORT_AV_COMP_AUTO
#include "drv_hdtv.h"
#endif

#include "tvd_debug.h"
#include "tve_if.h"


/**************************************************************************
 * Local Constant/Configure Definitions
 *************************************************************************/
//#if defined(CC_MT5363) || defined(CC_MT8223) || defined(CC_MT5365) || defined(CC_MT5395)
//#define CC_MT5363_TVD
//#endif
//#if defined(CC_MT8223) || defined(CC_MT5365) || defined(CC_MT5395)
//#define CC_MT8223_TVD
//#endif
//#if defined(CC_MT5365) || defined(CC_MT5395)
//#define CC_MT5365_TVD
//#endif

#ifdef CC_COPLAT_MT82
#define GetTargetCountry() COUNTRY_EU
#define eTVDLLockFreeRun 0
#endif
#define TVD_USE_NEW_DAGC    0
//#if TVD_USE_NEW_DAGC
#define TVD_DAGC_WA 	0 // 1: check sync level to change DAGC setting, 0: write correct DAGC for TG/CTG
//#endif
#define TVD_PALM50_SUPPORT 0
#define TVD_AntiTrs        1
#define TVD_PHALT_WA    1
#define TVD_PHALT_MN_WA2 	0
#define TVD_MODE_Deglitch 	1
#define TVD_MODESWITCH_PROTECT 0 //new add
#define TVD_MAX_CLAMP_TABLE	1
#define TVD_SERRTYPE_ONE	1
#define TVD_LLOCK_STATUS 1
#define TVD_ABS_DIFF_STDFV 1
#define TVD_FORCE_FAST_VALIGN 1
#define TVD_8222_NONSTANDARD 1
#define TVD_NO_BYPASS_TRICK 1
#define TVD_VCR_PALSW_FAST 0
#define TVD_VCR_SECAM_DARK 0
#define TVD_OLD_VCR 0
#define TVD_FAST_MODEGAIN_CHANNEL_CHANGE 0
#define SUPPORT_DFE_VPRES_DET 1
#define TVD_SECAM_DET 1 // 1: check burst lock
#define TVD_FORCE_UPD_MODE 1 // 1: always update mode
#define TVD_VCR_BV_STA     1
#define CAGC_PEAK_CNR_ADJ  0 //Adjust CAGC Peak paramater by CNR
#define TVD_HSHAKING_WA     1
#define TVD_HN_CHCHG_WA     1
#define TVD_AVG_SYNC_DET    0 //Avg Sync Switch
#define TVD_PHALT_SEL_PATCH 0

// ---------- Sony bug patchs ---------------------//
#define TVD_PICTURE_WHITE_FLASH	0
#define TVD_BURST_ONOFF_TEST_WA 0
#define TVD_RTM1_CM 0
// ---------- Sony bug patchs end -----------------//

// ----------  LG bug patchs ----------------------//
#define TVD_DETECT_UNSTABLE_VCRBV 1 //Eiden modulator
#define TVD_ADJUST_AGC_SPEED_FOR_LOW_SYNC 1
#define TVD_STRONG_VP_GAIN_FOR_US 0 //No Need
#define TVD_ADJUST_Y4H_FOR_FIELD 1
#define TVD_ADJUST_Y4V_FOR_FIELD 0 //No Need
#define TVD_NEW_HSYNCSLICER_SETTING 1
#define TVD_VPRES4_IMPROVEMENT_FOR_WEAKSGINAL 0
#define TVD_ADJSUT_PALSW_BY_VLOCK 1
#define TVD_LOW_SYNC_TRICKMODE_WA 1 //low sync, trick -> BOB mode
#define PDP_PANEL_NSTD_CHECK_WA 0
#ifdef CC_TVD_SUPPORT_DDI
#define TVD_FORCE_NO_CKILL 1
#define TVD_SUPPORT_704_DE 1
#define TVD_CHCHG_RESET_CORE_BY_AP 0
#define TVD_ATV_SNOWSCREEN_BY_DRV 1
#define TVD_FAST_CHCHG 1
#define TVD_NOBURST_SLOW_V625_WA 1
#define TVD_LIM_HERR_WA 1
#define TVD_BP_ATV_MODECHG 1
#define TVD_ADAP_VPRES_SETTING 1
#define TVD_RESET_MODE_CHCHG 1
#define TVD_VMASK_HEAD_SHAKING  1
#define TVD_SMALL_HWIDTH_SHAKING 1
#ifdef CC_LGE_PROTO_PCBA
#define TVD_CTRL_STABLE_MCNT 0
#else
#define TVD_CTRL_STABLE_MCNT 1
#endif
#define TVD_ANA_COPY_PROTECT_FILTER 1
#define TVD_MANUAL_VPRES_FOR_TUNING 1
#else
#define TVD_FORCE_NO_CKILL 0
#define TVD_SUPPORT_704_DE 0
#define TVD_CHCHG_RESET_CORE_BY_AP 0
#define TVD_ATV_SNOWSCREEN_BY_DRV 0
#define TVD_FAST_CHCHG 0
#define TVD_NOBURST_SLOW_V625_WA 0
#define TVD_LIM_HERR_WA 0
#define TVD_BP_ATV_MODECHG 0
#define TVD_ADAP_VPRES_SETTING 0
#define TVD_RESET_MODE_CHCHG 0
#define TVD_VMASK_HEAD_SHAKING  0
#define TVD_SMALL_HWIDTH_SHAKING 0
#ifdef CC_LGE_PROTO_PCBA
#define TVD_CTRL_STABLE_MCNT 0      // Modify TVD modechange>modechangedone 12v to 6v   zuyu.liao 2013.12.20
#else
#define TVD_CTRL_STABLE_MCNT 1
#endif
#define TVD_ANA_COPY_PROTECT_FILTER 0
#define TVD_MANUAL_VPRES_FOR_TUNING 0
#endif
// ----------  LG bug patchs end -----------------//

#ifdef CC_SCPOS_VPORCH_WA
#define TVD_VPORCH_TUNE (-2)	/* fix DISP mode & MJC,send mjc one line for MT5391 */
#else
#define TVD_VPORCH_TUNE 0
#endif
#define CCVSYNC_P_DEFAULT 0x19
#define CCVSYNC_N_DEFAULT 0x15
#define CCVSYNC_MIN -7
#define CCVSYNC_MAX 13

//----------Standard / Non-Standard Line number Define --------//
#define LINE_NON_STD_MIN  1695
#define LINE_NON_STD_MAX  1761
#define LINE_STD_MIN      1701
#define LINE_STD_MAX      1749

#define NA_SIZE 16 // window size

#define DET443CAGCThres 0x18
#define TVD_SCAN_CAGCThres 0x14 //20% chroma level

#if TVD_AUTO_CALIB
//target 16~235
#define TARGET_Y_LEVEL_N 0x230//560 NTSC_M, NTSC_J
#define TARGET_Y_LEVEL_N216 0x203//515 for 16~216
#define TARGET_Y_LEVEL_P 0x224 //548 PAL
#define TARGET_Y_LEVEL_P216 0x1f4 //500 PAL for 16~216
#define TVDAutoCalibDelayTime 0x10 //16 main_loop
#endif


#if TVD_SERRTYPE_ONE
#define AVGCOIIR_CNT 0x8
#define AVGCOIIR_TH 0x20
#endif

#define TVD_CAGC_LIM_GAIN 0x20

//For TvdCheckTVDLock
#define VLOCK_CHECK_TIME        0x20
#define TVDLOCK_RATIO           60      // 60%
#define TVDNRLevel_TH           0x13

#define MENU_AV_DELAY_H_OFFSET 0x80
#define CSCAN_DIFF_MIN_TH      0x1C0
#define CSCAN_NR_TH            0x5

#define IIR_LLOCK_EN_TH 39

#define TVD_SUPPORT_DRVCUST_DEFINE 0
/**************************************************************************
 * Local Type/Enum definitions
 *************************************************************************/
enum ehwVCRSetting
{
    VCR_NONE,
    VCR_MODE1
};

//--------- Noise Analysis -----------
enum eNASTATE
{
    NA_UNKNOWN,
    NA_LOWNOISE,
    NA_HIGHNOISE,
    NA_SUPERHIGHNOISE,
    NA_STOP
};

#if TVD_PALM50_SUPPORT
enum ePALM50_DET_STATE
{
    PALM50_DET_IDEL=0,
    PALM50_DET_START,   //PAL_N but burst is unlocked
    PALM50_DET_DONE_PALN,
    PALM50_DET_DONE_PALM50   //PAL_M50 detected  
};
#endif

/**************************************************************************
 * Imported functions (optional)
 *************************************************************************/
#ifdef CC_UP8032_ATV
#define fgApiEepromWriteByte(wAddr, bData) vApiEepromWriteByte(wAddr, bData)
//#else
//extern BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData);
#endif

/**************************************************************************
 * Imported variables (optional)
 *************************************************************************/
extern UINT16 _wTargetBlk;
extern UINT8  RF_level;
extern UINT32 _u4BypassNptvMloop;


/**************************************************************************
 * Static/Local variables
 *************************************************************************/

static TVD_PFN_CLI_NFY   _pfnTVDCLINotify     = NULL;
//static UINT8 _seSourceType; //Refer to enum Signal_Type
static struct  RTvdCtrl
{
    UINT8 en;
    UINT8 val;  /* should reserve 8bit MSB from effective bArg*/
} _srTvdCtrl[TCTL_MAX];
#if !TVD_HSHAKING_WA
static UINT8 _sbTVDFreeRun = 0; //Flag for HW FreeRun.
#endif

//{PAL_N, PAL, PAL_M, NTSC358, SECAM, PAL_60, SV_CS_UNSTABLE, NTSC443};
#if TVD_SUPPORT_704_DE
static const UINT8 _sTvd_MenuAvDelay_Tbl[] =
{0xC4,0xB8,0xBA,0xB9,0xA7,0xAE,0xA6,0xAD};
#else
static const UINT8 _sTvd_MenuAvDelay_Tbl[] =
{0xBC,0xB0,0xB2,0xB1,0xA0,0xA5,0xA6,0xA5};//MSPG-6100
#endif

#if !TVD_SUPPORT_DRVCUST_DEFINE
static UINT8* _spCurrAVDelayTbl = (UINT8 *)_sTvd_MenuAvDelay_Tbl;
#endif
#define DFT_H_PORCH 0xA0

static const INT8 _sTvd_ManualVPorch_Tbl[]=
     {0, 0, 0, -1, -1, -1, 0, -1};//PAL60 and NTSC443 are Demod_Curf not GameMode
static INT8* _spCurrVPorchTbl = (INT8 *)_sTvd_ManualVPorch_Tbl;
static INT8 _su1UIVPorchDelay=0;
#define DFT_V_PORCH 0x40


#if !TVD_HSHAKING_WA
static const UINT8 _sTvd_SHNA_MenuAvDelay_Tbl[] =
{0xC7, 0xB7, 0xB2, 0xB9, 0xA3, 0xAD, 0xB0, 0xA7};
//	{0xCB, 0xBD, 0xBF, 0xBF, 0xA9, 0xBD, 0xB0, 0xB0};
#endif
static INT8 _su1UIDelay = 0;

#if TVD_LLOCK_STATUS
static UINT16 _swLLStatus_avg;
#endif

#if TVD_ABS_DIFF_STDFV
static UINT16 _u2AbsDiffStdFV;
static UINT16 _u2Var8Field;
static UINT8  _u1LLockStableCnt = 200;
#endif

//Counter for VCR mode
static UINT8   _sbVCRCnt;

#if TVD_SECAM_DET
static UINT8 _sbBurstcnt;
#endif
#if TVD_PHALT_MN_WA2
static UINT8 _sbPhalt_Vpres=0;
static UINT8 _sbForcePalMN=FALSE;
#endif

//Mode Change Counter
static UINT8   _sbTvdModeCnt;
static UINT8   _sbTvd_McDone_cnt = 12;
#if TVD_CTRL_STABLE_MCNT
static UINT8   _sbTvd_Reduced_McDone_cnt = 6;
#endif

//Co-Channel deglitch Counter
static UINT8   _sbCOCHDeCnt;
//Co-Channel2 Counter
static UINT8   _sbCOCH2Cnt;

static UINT8 _sbHSyncPosDetCnt=0;

#if SUPPORT_SCART
//Scart counter
static UINT8 _sbSCnt;
//Counter for Scart SFisrt
static UINT8 _sbScart_SV;
#endif

//Target sync height
static UINT16  _swAGCTargetSyncHeight = 0x380;

//NR Avg
static UINT16 _swTVDNRavg1 = 3;

//Na detect
//static UINT8  _sna_HysterCnt; // For hystersis of NA calculation.
//static UINT8  _sna_HysterCntThres;
//static UINT8  _sna_cnt;
//static UINT16 _sna_sum;


#if TVD_SERRTYPE_ONE
static UINT16 _swCochIir_Avg;
static UINT8 _sbAvgCOCH_detect;
#endif

static UINT8 _sbHLOCKCnt;

#if TVD_OLD_VCR
//VCR
static UINT8 _sbOldVCRCnt;
static BOOL  _sfgOldVCR;  //maybe no useful ??
#endif

#if TVD_FRS_FOR_NONSTDSIGNAL
static UINT8 _sbNONSTDCnt;
static BOOL  _fgForceUpdNSTD;
#endif

static UINT8 _spre_na_state;

#if TVD_AUTO_CALIB
static UINT8 _sbTVDAutoCalibState1;
static UINT8 _sbTVDAutoCalibResult;
#endif

#if !TVD_NO_BYPASS_TRICK
static UINT8 _sbBypassTrick;
#endif

static UINT8 _sbDCLAMPCnt;

//for Fix AGC
static UINT8 _sbIsTVDFixAgc = 0;//default auto
static UINT8  _sbTvdMenuAcode = 0x1F;
static UINT16 _swTvdMenuDcode = 0x800;


//YCDelay control by AP
static UINT8 _sbIsSetYCDelaybyAP = 0;
//CAGC control by AP
static UINT8 _sbIsSetCAGCbyAP = 0;
//AAF control by AP
static UINT8 _sbIsSetAAFbyAP = 0;
//Offset control by AP
static UINT8 _sbIsTVDOffsetbyAP = 0;

static UINT8 _sbTVDSerrCnt;
static UINT8 _bTVDOverSampleEn = CC_SUPPORT_OVERSAMPLE_1440;

#if TVD_AUTO_CALIB
static UINT8 _sbTVDAutoCalibCount,_sbPre_Manual_ACode=0xff;
static UINT16 _swY_Avg_Lvl, _swY_Diff, _swTvd_Gain;
#endif

#ifdef  SUPPORT_AV_COMP_AUTO
static UINT8   _sbAvCompCnt;
static UINT8  _u1AVCompStatus=0xff;
#endif
#if SUPPORT_S_DETECT
static UINT8   _sbSFirstCnt;
static UINT8   _sbSVFNotify=SV_FALSE;
static UINT8   _sbSVFBypassNotify=SV_FALSE;
#endif
#if TVD_VPRES_STATE
static UINT8 _sbVpresState;
static UINT8 _sbVpresStateCnt1;
#endif
static UINT8 fgPreVPres_0;

#if TVD_VCR_BV
static UINT8 _sbVCRBVCnt;
#endif

#if TVD_VCR_BV_STA
static UINT8 _sbVCRBVStaCnt;
static UINT8 _fgVCRBV;
#endif
static UINT8 _sbVP1Cnt;
#if 0
static const UINT8 _sbTvdModeMapTable[8]=
{
    /* Is525,Is443,PHALT*/
    /* 0,0,0*/	SV_CS_PAL, /* SV_CS_UNSTABLE*/
    /* 0,0,1*/	SV_CS_PAL_N,
    /* 0,1,0*/	SV_CS_SECAM,
    /* 0,1,1*/	SV_CS_PAL,
    /* 1,0,0*/	SV_CS_NTSC358,
    /* 1,0,1*/	SV_CS_PAL_M,
    /* 1,1,0*/	SV_CS_NTSC443,
    /* 1,1,1*/	SV_CS_PAL_60
};
#endif

static UINT8 _su1VPresVFlagCnt=0;

#if TVD_NEW_HSYNCSLICER_SETTING
UINT32 _u4SyncLvSum = 0;
UINT16 _u2SyncLvAvg = 0;
UINT8 _u1SyncLvCnt = 0;
UINT32 _u4BYCLvSum  = 0;
UINT16 _u2BYCLvAvg  = 0;
#endif
static UINT8 _sbHsliceCnt;
// For TVD Vlock used
static UINT16 _swTVDNRLevel, _swTVDNRLevel_AVG; //New Add

static UINT8 _sbPreAutoSearch = 0 ;

#if TVD_HN_CHCHG_WA
static UINT8 _sbHNChChgMCntTh = 25;
static UINT8 _sbHNChChgNRTh = 30;
#endif

#if TVD_BP_ATV_MODECHG
static BOOL _sbBypassModeChg = FALSE;
static UINT8 _sbTvd3dModePreVPresOn = SV_CS_NONE;
#endif

#if TVD_DETECT_UNSTABLE_VCRBV
UINT8 VCRBV_AVG_COUNT = 0;
static BOOL _sbUnsbableVCRBV=FALSE;
static UINT16 _su2SumAbsDiff=0;
static UINT16 _su2Sum8FieldDiff=0;
static UINT16 _su2AvgAbsDiff=0;
static UINT16 _su2Avg8FieldDiff=0;
#endif

#if TVD_ADJUST_AGC_SPEED_FOR_LOW_SYNC
UINT8 _su1BYCClipCnt = 0;
#endif

#if TVD_ADJUST_Y4V_FOR_FIELD
UINT8 _su1SHNY4VBWCnt= 0;
#endif

#if TVD_VPRES4_IMPROVEMENT_FOR_WEAKSGINAL
UINT8 _su1VPresOffCnt = 0;
#endif

#if TVD_ADJSUT_PALSW_BY_VLOCK
UINT8 _su1VLockCntForPalsw = 0;
#endif

BOOL _sbSerrType0 = FALSE;
UINT8 TG_SERR_CNT_START = TG_SERR_CNT;
#if TVD_SERRCNT_WA
BOOL _sbSerrCntAdjEn = FALSE;
/*
UINT8 _u1SmallSerrWidthCntInChChg = 0;
UINT8 _u1SmallSerrWidthCntForce = 0;
UINT8 _u1SerrSetCnt=9;
UINT8 _u1LargeSerrWidthCnt = 0;
UINT8 _u1SmallSerrWidthCnt = 0;
UINT8 _u1SerrWidthChgCntTh = 0;
*/
//UINT8 _u1NTSCVPorch = 0x1;
//BOOL _sbSerrCntAdjBack = FALSE;
//UINT8 _sbSerrCntAdjBackCnt = 0;
#endif

#if TVD_LOW_SYNC_TRICKMODE_WA
#define SMALL_SYNC_DET_MAX_CNT 16
UINT8 _u1SmallSyncDet4Trick = 0;
#endif

#if PDP_PANEL_NSTD_CHECK_WA
BOOL _sbPDPModeChgDoneAgain = FALSE;
#endif

BOOL _sbManualVPresGainbyDDI = FALSE;
BOOL _sbManualLLockGainbyDDI = FALSE;
#if TVD_MANUAL_VPRES_FOR_TUNING
static BOOL _sbManualVPresForTuning = FALSE;
#endif

UINT8 _u1TvdVideoStableCnt = 4;
BOOL _bATVChannelChange = FALSE;
UINT8 _u1TvdRstCnt = 10;
UINT8 _u1IIRLLockEn = 0;
UINT8 _u1FixLLockGainTh =0x30;

#ifdef CC_SUPPORT_RECORD_AV
static BOOL _sbConnectedByMain = FALSE;
static BOOL _sbConnectedBySub = FALSE;
static BOOL _sbIssueModeChgDoneMain = FALSE;
static BOOL _sbIssueModeChgDoneSub = FALSE;
static BOOL _sbATVPVRBypassModeChg = FALSE;
static UINT8 _su1MainSourceType = SV_ST_MAX;
static UINT8 _su1SubSourceType = SV_ST_MAX;
#endif

#if TVD_PALM50_SUPPORT
UINT8 _su1PALM50DetState = PALM50_DET_IDEL;
UINT8 _su1PALM50BstLockCnt = 0;
UINT8 _su1PALM50BstUnLockCnt = 0;
UINT8 _su1PALM50VFreqChgCnt = 0;
#endif

static UINT8 _su1LineLockCondition = 0;
#if TVD_LIM_HERR_WA
static UINT8 _su1HerrTrickCnt = 0;
static UINT8 _su1HerrHSCnt = 0;
static BOOL _sbLimHerrEn1 = FALSE;
static BOOL _sbLimHerrEn2 = FALSE;
#define HERR_HS_CNT_TH1 100
#define HERR_HS_CNT_TH2 50
#endif

#if TVD_ADAP_VPRES_SETTING
static BOOL _sbTvdConnected = FALSE;
static BOOL _sbAdapVpresStart = FALSE;
static UINT16 _su2AdapVpresCounter = 0;
//BOOL _bAdapVpresUnmute = FALSE;
#endif

static UINT16 _su2ActiveWidth = 704;
static UINT16 _su2ActiveHeight = 480;

#ifdef CC_SUPPORT_STR
static UINT32 pm_save_cdet_regs[1+(CDET_0F-CDET_00)/4];
static UINT32 pm_save_dfe1_regs[1+(DFE_1F-DFE_00)/4];
static UINT32 pm_save_dfe2_regs[1+(DFE_23-DFE_20)/4];
static UINT32 pm_save_dfe3_regs[1+(DFE_SCART_02-DFE_SCART_02)/4];
static UINT32 pm_save_dfe4_regs[1+(DFE_24-DFE_24)/4];
static UINT32 pm_save_secam_regs[1+(SECAM_07-SECAM_00)/4];
static UINT32 pm_save_vsrc1_regs[1+(VSRC_01-VSRC_00)/4];
static UINT32 pm_save_vsrc2_regs[1+(VSRC_02-VSRC_02)/4];
static UINT32 pm_save_vsrc3_regs[1+(VSRC_08-VSRC_07)/4];
static UINT32 pm_save_vsrc4_regs[1+(VSRC_09-VSRC_09)/4];
static UINT32 pm_save_tg1_regs[1+(TG_17-TG_00)/4];
static UINT32 pm_save_tg2_regs[1+(TG_18-TG_18)/4];
static UINT32 pm_save_tg3_regs[1+(TG_19-TG_19)/4];
static UINT32 pm_save_ctg1_regs[1+(CTG_07-CTG_00)/4];
static UINT32 pm_save_ctg2_regs[1+(CTG_08-CTG_08)/4];
static UINT32 pm_save_svf_regs[1+(SVF_01-SVF_02)/4];
static UINT32 pm_save_ctg_dering_regs[1+(CTG_DERING-CTG_DERING)/4];
static UINT32 pm_save_tg_dering_regs[1+(TG_DERING-TG_DERING)/4];
static UINT32 pm_save_aaf_dering_regs[1+(AAF_DERING-AAF_DERING)/4];
static UINT32 pm_save_oth_dering_regs[1+(DERING_OTHER_SETTING-DERING_OTHER_SETTING)/4];
static UINT32 pm_save_omux_sel_regs [1+(OMUX_VDOIN_CLK_SEL-OMUX_VDOIN_CLK_SEL)/4];
#endif

/**************************************************************************
 * Global variables
 *************************************************************************/

RTvdAGCInfo     _rTvd3dAGCInfo;
RTvdCAGCInfo    _rTvd3dCAGCInfo;
RTvdStatus      _rTvd3dStatus;
RTvdNSTDStatus      _rTvd3dNSTDStatus;
UINT8           _bTvdHoldMode;
UINT8  _bTvdISRCnt;
UINT8 _bAgcPedEn;       //Pedestal Mode
UINT8 _bSetChannelCnt;  //Counter for tuner, set 60 when change channel number rock 20060324
#if TVD_AntiTrs
static UINT8 _sbCAGC1 =0 ;                        //  FIRST field :the value of cagc
static UINT8 _sfgCagc=0;
#endif
#if TVD_COCH_FLICKER
UINT8 bIsBlankVarLarge;
#endif

#if TVD_MODE_Deglitch
//--------- Counter for Pre-Mode-Change -----------
static UINT8   _sbTvdPreModeCnt=0xFF;
static BOOL    _sfgPreVPres_2=0;
static BOOL    _sfgPreIs525_2=0;
static UINT8   _sbPreTvdMode_2=0xFF;

#define vDrvTvdRstPreModeCnt()        (_sbTvdPreModeCnt = 0)
#define vDrvTvdIncPreModeCnt()        (_sbTvdPreModeCnt = (_sbTvdPreModeCnt == 0xff)?0xff:(_sbTvdPreModeCnt+1))
#define fgDrvTvdIsPreModeCnt(bCnt)    (_sbTvdPreModeCnt==(bCnt))

static UINT8 TVD_PreMCDONE_CNT_Vpres=5;
static UINT8 TVD_PreMCDONE_CNT_Mode=20;
#endif

UINT8 bhwVCRSetting;

//To solve PAL_SW problem of VCR. Benson060210.
UINT16 wTVDNRavg;  //CDET NR Avg.

BOOL _fgHLOCK;

#if TVD_FRS_FOR_NONSTDSIGNAL
UINT16 _wPreAvg_VLen;
UINT16 _wPreLCnt;
#endif

UINT8 _na_state; /* noise analysis state machine */

#if TVD_AUTO_CALIB
UINT8 _bTVDAutoCalibState;
#endif

#ifdef NEW_COUNTRY_TYPE
UINT32 u4AtvCountry;
#else
#ifdef COUNTRY_FROM_EEPROM
UINT8 u1TargetCountryInfo;
#endif
#endif
#if TVD_USE_NEW_DAGC
#if TVD_DAGC_WA
UINT8 _sbTvdDCodeCnt = 0;
#endif
#endif

#if TVD_PHALT_WA
BOOL _fgEnablePhaltWa = TRUE;
#endif

UINT32 _u4ChLockCnt=0;
UINT32 _u4ChUnLockCnt=0;
UINT32 _u4ChVPresOnCnt=0;
UINT32 _u4ChVPresOffCnt=0;
BOOL _bChPrevVPres = TRUE;

#if TVD_SET_ENABLED_CS
BOOL _bTriggerModeDetForEnabledCS = FALSE;
BOOL _bMModeForEnabledCS = FALSE;
RTvdEnabledCS_T _rTvEnabledCS;
RTvdEnabledCS_T _rAvEnabledCS;
RTvdEnabledCS_T _rPrevEnabledCS;
#endif

#if ENABLE_PRBS_BY_DRIVER
BOOL _bEnablePrbs = TRUE;
BOOL _bEnablePrbsByAPMute = FALSE;
#endif

#if TVD_MANUAL_TUNER_SCAN
static UINT8 _su1VpresGain = 0;
static UINT8 _su1VlockRatio =0;
static BOOL _sfgManualScanadj = FALSE;
#endif

#if TVD_ANA_COPY_PROTECT_FILTER
static UINT8 _su1AnaCopyProPSyncCnt =0;
static UINT8 _su1AnaCopyProCSTRIPECnt =0;
static UINT8 _su1AnaCopyProMvType2Cnt =0;
static UINT8 _sbAnaCopyProPSync =0, _sbPreAnaCopyProPSync =0;
static UINT8 _sbAnaCopyProCSTRIPE =0, _sbPreAnaCopyProCSTRIPE =0;
static UINT8 _sbAnaCopyProMvType2 =0, _sbPreAnaCopyProMvType2 =0;
#endif
/**************************************************************************
 * Local Macro definitions
 *************************************************************************/
#define IS_LINE_STD(line)      ((((line) < LINE_STD_MAX)&&((line) > LINE_STD_MIN)) ? TRUE : FALSE)
#define IS_LINE_NON_STD(line)  ((((line) > LINE_NON_STD_MAX)||((line) < LINE_NON_STD_MIN)) ? TRUE : FALSE)
#define IS_AUTOSEARCH()        ((_fgAutoSearch) ? TRUE : FALSE) //_fgAutoSearch extern golbal variable. 
#define IS_FH_NON_STD()        (fgHwTvdFHPos()||fgHwTvdFHNeg())
//#define IS_5387_NonECO()       (IS_IC_5387() && (BSP_GetIcVersion() <= IC_VER_5387_AA))
//#define IS_5387_ECO()          (IS_IC_5387() && (BSP_GetIcVersion() >= IC_VER_5387_AB))
//#define IS_5363_NonECO()       (IS_IC_5363() && (BSP_GetIcVersion() <= IC_VER_5363_AA))
//#define IS_5363_ECO()          (IS_IC_5363() && (BSP_GetIcVersion() >= IC_VER_5363_AB))


//--------- Counter for VCR Mode ------------------------------//
#define TVD_VCRON_CNT  0xFF
#define TVD_VCROFF_CNT 0x0
#define TVD_VCRReset   0xF2
#define vDrvTvdRstVCRCnt()      (_sbVCRCnt = TVD_VCRReset)
#define vDrvTvdIncVCRCnt()      (_sbVCRCnt = (_sbVCRCnt == TVD_VCRON_CNT)?TVD_VCRON_CNT:(_sbVCRCnt+1))
#define vDrvTvdDecVCRCnt()      (_sbVCRCnt = (_sbVCRCnt == TVD_VCROFF_CNT)?TVD_VCROFF_CNT:(_sbVCRCnt-1))
#define fgDrvTvdIsVCRCnt(bCnt)  ((_sbVCRCnt==(bCnt))? TRUE : FALSE)

//---------Counter for Burst unlock change BI_START/END -------//
#if TVD_SECAM_DET
#define TVD_BurstLock_CNT  0xA
#define TVD_BurstLock_CNT2 0x0
#define BurstcntReset    0x5

#define vDrvTvdRstBurstCnt()      (_sbBurstcnt = BurstcntReset)
#define vDrvTvdIncBurstCnt()      (_sbBurstcnt = (_sbBurstcnt == TVD_BurstLock_CNT)?TVD_BurstLock_CNT:(_sbBurstcnt+1))
#define vDrvTvdDecBurstCnt()      (_sbBurstcnt = (_sbBurstcnt == TVD_BurstLock_CNT2)?TVD_BurstLock_CNT2:(_sbBurstcnt-1))
#define fgDrvTvdIsBurstCnt(bCnt)  ((_sbBurstcnt == (bCnt))? TRUE : FALSE)
#endif

//--------- Counter for Mode Change ---------------------------//
#define vDrvTvdRstModeCnt()       (_sbTvdModeCnt = 0)
#define vDrvTvdIncModeCnt()       (_sbTvdModeCnt = (_sbTvdModeCnt == 0xff)?0xff:(_sbTvdModeCnt+1))
#define fgDrvTvdIsModeCnt(bCnt)   ((_sbTvdModeCnt==(bCnt))? TRUE : FALSE)
#define fgDrvTvdModeCntGt(bCnt)   ((_sbTvdModeCnt>=(bCnt))? TRUE : FALSE)

//--------- Counter for COCH deglitch-----------
#define TVD_COCHDe_CNT  0x20
#define TVD_COCHDe_CNT2 0x0
#define COCHDeReset     0x10
#define vDrvTvdRstCOCHDeCnt()       (_sbCOCHDeCnt = COCHDeReset)
#define vDrvTvdIncCOCHDeCnt()       (_sbCOCHDeCnt = (_sbCOCHDeCnt == TVD_COCHDe_CNT)?TVD_COCHDe_CNT:(_sbCOCHDeCnt+1))
#define vDrvTvdDecCOCHDeCnt()       (_sbCOCHDeCnt = (_sbCOCHDeCnt == TVD_COCHDe_CNT2)?TVD_COCHDe_CNT2:(_sbCOCHDeCnt-1))
#define fgDrvTvdIsCOCHDeCnt(bCnt)   ((_sbCOCHDeCnt == (bCnt))? TRUE : FALSE)
//--------- Counter for COCH2 Enable-----------
#define TVD_COCH2EN_CNT  0x40
#define TVD_COCH2EN_CNT2 0x0
#define COCH2ENReset     0x20
#define COIIR_TH_HIGH    0x36
#define COIIR_TH_LOW     0x30
#define vDrvTvdRstCOCH2ENCnt()       (_sbCOCH2Cnt = COCH2ENReset)
#define vDrvTvdIncCOCH2ENCnt()       (_sbCOCH2Cnt = (_sbCOCH2Cnt == TVD_COCH2EN_CNT)?TVD_COCH2EN_CNT:(_sbCOCH2Cnt+1))
#define vDrvTvdDecCOCH2ENCnt()       (_sbCOCH2Cnt = (_sbCOCH2Cnt == TVD_COCH2EN_CNT2)?TVD_COCH2EN_CNT2:(_sbCOCH2Cnt-1))
#define fgDrvTvdIsCOCH2ENCnt(bCnt)   ((_sbCOCH2Cnt == (bCnt))? TRUE : FALSE)

//HSync Position Detection
#define HSYNCPOS_DCNT_TH1 0x3A
#define HSYNCPOS_DCNT_TH2 0x80
#define TVD_HSYNCPOS_CNT  0x20
#define TVD_HSYNCPOS_CNT2 0x0
#define HSYNCPOSReset     0x10
#define vDrvTvdRstHsyncPosDetCnt()       (_sbHSyncPosDetCnt = HSYNCPOSReset)
#define vDrvTvdIncHsyncPosDetCnt()       (_sbHSyncPosDetCnt = (_sbHSyncPosDetCnt == TVD_HSYNCPOS_CNT)?TVD_HSYNCPOS_CNT:(_sbHSyncPosDetCnt+1))
#define vDrvTvdDecHsyncPosDetCnt()       (_sbHSyncPosDetCnt = (_sbHSyncPosDetCnt == TVD_HSYNCPOS_CNT2)?TVD_HSYNCPOS_CNT2:(_sbHSyncPosDetCnt-1))
#define fgDrvTvdIsHsyncPosDetCnt(bCnt)   ((_sbHSyncPosDetCnt == (bCnt))? TRUE : FALSE)


#if SUPPORT_SCART
//--------- Counter for Scart SFisrt -----------
#define TVD_S_CNT     9
#define TVD_S_CNT2    1

#define vDrvTvdRstSCnt()      (_sbSCnt = 5)
#define vDrvTvdIncSCnt()      (_sbSCnt = (_sbSCnt == 10) ?10:(_sbSCnt+1))
#define vDrvTvdDecSCnt()      (_sbSCnt = (_sbSCnt == 0x0)?0x0:(_sbSCnt-1))
#define fgDrvTvdIsSCnt(bCnt)  ((_sbSCnt==(bCnt))? TRUE : FALSE)
#endif

//--------- Counter for HLOCK for Manual Line-lock-----------
#define TVD_HLOCKDONE_CNT  0x50
#define TVD_HLOCKDONE_CNT2 0x0
#define TVD_HLOCK_RSTCNT   0x4a

#define vDrvTvdRstHLOCKCnt()      (_sbHLOCKCnt = TVD_HLOCK_RSTCNT)
#define vDrvTvdIncHLOCKCnt()      (_sbHLOCKCnt = (_sbHLOCKCnt == TVD_HLOCKDONE_CNT)?TVD_HLOCKDONE_CNT:(_sbHLOCKCnt+1))
#define vDrvTvdDecHLOCKCnt()      (_sbHLOCKCnt = (_sbHLOCKCnt == TVD_HLOCKDONE_CNT2)?TVD_HLOCKDONE_CNT2:(_sbHLOCKCnt-1))
#define fgDrvTvdIsHLOCKCnt(bCnt)  ((_sbHLOCKCnt == (bCnt))? TRUE : FALSE)

#if TVD_FRS_FOR_NONSTDSIGNAL
//--------- Counter for NonStandard  -----------
#define TVD_NONSTD_CNT      0xd0
#define TVD_NONSTD_CNT2     0x30
#define TVD_NONSTD_RSTCNT   0x80
#define vDrvTvdRstNONSTDCnt()      (_sbNONSTDCnt = TVD_NONSTD_RSTCNT)
#define vDrvTvdIncNONSTDCnt()      (_sbNONSTDCnt = (_sbNONSTDCnt == 0xFF)?0xFF:(_sbNONSTDCnt+1))
#define vDrvTvdDecNONSTDCnt()      (_sbNONSTDCnt = (_sbNONSTDCnt == 0)?0:(_sbNONSTDCnt-1))
#define fgDrvTvdIsNONSTDCnt(bCnt)  ((_sbNONSTDCnt == (bCnt))? TRUE : FALSE)
#define vDrvTvdForceUpdNSTD(fgForceNSTD) (_fgForceUpdNSTD = (fgForceNSTD))
#define vDrvTvdIsForceUpdNSTD()          (_fgForceUpdNSTD)
#endif


#if TVD_OLD_VCR
//--------- Counter for OldVCR for Manual Line-lock-----------
#define TVD_OldVCR_CNT  0x70
#define TVD_OldVCR_CNT2 0x0
#define _bOldVCRReset   0x68

#define vDrvTvdRstOldVCRCnt()      (_sbOldVCRCnt = _bOldVCRReset)
#define vDrvTvdIncOldVCRCnt()      (_sbOldVCRCnt = (_sbOldVCRCnt == TVD_OldVCR_CNT)?TVD_OldVCR_CNT:(_sbOldVCRCnt+1))
#define vDrvTvdDecOldVCRCnt()      (_sbOldVCRCnt = (_sbOldVCRCnt == TVD_OldVCR_CNT2)?TVD_OldVCR_CNT2:(_sbOldVCRCnt-1))
#define fgDrvTvdIsOldVCRCnt(bCnt)  (_sbOldVCRCnt == (bCnt))
#endif

#ifdef  SUPPORT_AV_COMP_AUTO
#define TVD_AVCOMP_CNT   10//original 20
#define vDrvTvdRstAVCompCnt()       (_sbAvCompCnt= 0)
#define vDrvTvdIncAVCompCnt()       (_sbAvCompCnt= (_sbAvCompCnt== 0xff)?0xff:(_sbAvCompCnt+1))
#define fgDrvTvdIsAVCompCnt(bCnt)   ((_sbAvCompCnt==bCnt)? TRUE : FALSE)
#define fgDrvTvdAvCompCntGt(bCnt)   ((_sbAvCompCnt>=bCnt)? TRUE : FALSE)
#endif


#if SUPPORT_S_DETECT
#define TVD_SFIRST_CNT   10//original 20
//--------- Counter for Scart SFisrt -----------
#define vDrvTvdRstSFirstCnt()       (_sbSFirstCnt = 0)
#define vDrvTvdIncSFirstCnt()       (_sbSFirstCnt = (_sbSFirstCnt == 0xff)?0xff:(_sbSFirstCnt+1))
#define fgDrvTvdIsSFirstCnt(bCnt)   ((_sbSFirstCnt==(bCnt))? TRUE : FALSE)
#define fgDrvTvdSFirstCntGt(bCnt)   ((_sbSFirstCnt>=(bCnt))? TRUE : FALSE)
#endif
#if TVD_VPRES_STATE
//--------- Counter for Vpres State  -----------
#define VPRES_STATE_0 0  // Vpres4 off, Vpres off
#define VPRES_STATE_1 1  // Vpres4 off, Vpres on
#define VPRES_STATE_2 2  // Vpres4 on, Vpres off
#define VPRES_STATE_3 3  // Vpres4 on, Vpres on
#define VPRES_STATE_4 4  // SVIDEO, Only Check Vpres3
#define VPRES_TH 2
#define vDrvTvdRstVpresStateCnt1()      (_sbVpresStateCnt1= 0)
#define vDrvTvdIncVpresStateCnt1()      (_sbVpresStateCnt1 = (_sbVpresStateCnt1 == 0xFF)?0xFF:(_sbVpresStateCnt1+1))
#define vDrvTvdDecVpresStateCnt1()      (_sbVpresStateCnt1 = (_sbVpresStateCnt1 == 0)?0:(_sbVpresStateCnt1-1))
#define fgDrvTvdIsVpresStateCnt1(bCnt)  ((_sbVpresStateCnt1 == (bCnt))? TRUE : FALSE)
#endif


//--------- Counter for Dclamp on/off ------------------------------//
#define TVD_DCLAMPON_CNT  0x20
#define TVD_DCLAMPOFF_CNT 0x0
#define TVD_DCLAMPReset   0x10
#define vDrvTvdRstDCLAMPCnt()      (_sbDCLAMPCnt = TVD_DCLAMPReset)
#define vDrvTvdIncDCLAMPCnt()      (_sbDCLAMPCnt = (_sbDCLAMPCnt == TVD_DCLAMPON_CNT)?TVD_DCLAMPON_CNT:(_sbDCLAMPCnt+1))
#define vDrvTvdDecDCLAMPCnt()      (_sbDCLAMPCnt = (_sbDCLAMPCnt == TVD_DCLAMPOFF_CNT)?TVD_DCLAMPOFF_CNT:(_sbDCLAMPCnt-1))
#define fgDrvTvdIsDCLAMPCnt(bCnt)  ((_sbDCLAMPCnt==(bCnt))? TRUE : FALSE)
#define TVD_DCLAMP_Y4H 0x3
#define TVD_STD_Y4H 0x2
#define TVD_FT_Y4H 0x1
//-----------Counter for channel Changed
#define CHANCHG_START  0x40
#define CHANCHG_STOP   0x0
#define CHANCHG_DONE   0x1

#define vDrvTvdSetChanchgCnt(value) (_bSetChannelCnt = (value))
#define vDrvTvdDecChanchgCnt()      (_bSetChannelCnt = (_bSetChannelCnt==0)?0:(_bSetChannelCnt-1))
#define fgDrvTvdIsChanchgCnt(bCnt)  ((_bSetChannelCnt == (bCnt))? TRUE : FALSE)

#if TVD_VCR_BV
#define TVD_VCR_BV_CNT  0x50

#define vDrvTvdRstVCRBVCnt()      (_sbVCRBVCnt = 0)
#define vDrvTvdIncVCRBVCnt()      (_sbVCRBVCnt = (_sbVCRBVCnt == TVD_VCR_BV_CNT)?TVD_VCR_BV_CNT:(_sbVCRBVCnt+1))

//#define vDrvTvdDecBurstCnt()      (_sbBurstcnt = (_sbBurstcnt == TVD_BurstLock_CNT2)?TVD_BurstLock_CNT2:(_sbBurstcnt-1))

#define fgDrvTvdIsVCRBVCnt(bCnt)  ((_sbVCRBVCnt == (bCnt))? TRUE : FALSE)
#define fgDrvTvdVCRBVCntGt(bCnt)  ((_sbVCRBVCnt >= (bCnt))? TRUE : FALSE)



//--------- Counter for VCR BV Status ------------------------------//
#define TVD_VCRBV_UP_GAIN   0x01
#define TVD_VCRBV_DOWN_GAIN 0x10
#define TVD_IS_VCRBVSTAON   0x50
#define TVD_IS_VCRBVSTAOFF  0x00
#define vDrvTvdRstVCRBVStaCnt()      (_sbVCRBVStaCnt = _fgVCRBV=0)
#define vDrvTvdIncVCRBVStaCnt()      (_sbVCRBVStaCnt = (_sbVCRBVStaCnt >= TVD_IS_VCRBVSTAON)?TVD_IS_VCRBVSTAON:(_sbVCRBVStaCnt+TVD_VCRBV_UP_GAIN))
#define vDrvTvdDecVCRBVStaCnt()      (_sbVCRBVStaCnt = (_sbVCRBVStaCnt <= TVD_VCRBV_DOWN_GAIN)?0x0:(_sbVCRBVStaCnt-TVD_VCRBV_DOWN_GAIN))
#define fgDrvTvdIsVCRStaCnt(bCnt)    ((_sbVCRBVStaCnt==(bCnt))? TRUE : FALSE)
#define fgGetTvdVCRBVSta()           ((_fgVCRBV)? TRUE : FALSE)
#endif

//--------- Counter for VP1 on/off gain when vpres off->on-----------//
#define TVD_VP1Reset   0x0
#define TVD_VP1OFFON_threshold   0x80
#define vDrvTvdinitVP1Cnt()      (_sbVP1Cnt = TVD_VP1OFFON_threshold)
#define vDrvTvdRstVP1Cnt()      (_sbVP1Cnt = TVD_VP1Reset)
#define vDrvTvdIncVP1Cnt()      (_sbVP1Cnt = (_sbVP1Cnt == 0xFF)?0xFF:(_sbVP1Cnt+1))
#define fgDrvTvdVP1CntGt(bCnt)  ((_sbVP1Cnt>=(bCnt))? TRUE : FALSE)
#define vDrvTvdGetTVDNRAvg()    (wTVDNRavg)

//--------- Hystersis for VPres_VFlag-----------//
#define TVD_VPresVFlag_dnth			0x01
#define TVD_VPresVFlag_rstth		0x1A
#define TVD_VPresVFlag_upth			0x2A
#define vDrvTvdRstVPresVFlagCnt()		(_su1VPresVFlagCnt= TVD_VPresVFlag_rstth)
#define vDrvTvdIncVPresVFlagCnt()		(_su1VPresVFlagCnt = (_su1VPresVFlagCnt > TVD_VPresVFlag_upth) ? (TVD_VPresVFlag_upth+1):(_su1VPresVFlagCnt+1))
#define vDrvTvdDecVPresVFlagCnt()		(_su1VPresVFlagCnt = (_su1VPresVFlagCnt < TVD_VPresVFlag_dnth) ? (TVD_VPresVFlag_dnth-1):(_su1VPresVFlagCnt-1))
#define fgDrvTvdVPresVFlagCnt(bCnt)		(_su1VPresVFlagCnt == (bCnt))

//--------- Counter for DCODE Status for small swing------------------------------//
#if TVD_USE_NEW_DAGC
#if TVD_DAGC_WA
#define TVD_IS_DCODESTAON   0x40
#define TVD_IS_DCODESTAOFF  0x00
#define TVD_DCODE_RST  0x20
#define TVD_DCODE_UPTH  0x700
#define TVD_DCODE_DNTH  0x600
#define vDrvTvdRstDCODEStaCnt()      (_sbTvdDCodeCnt = TVD_DCODE_RST)
#define vDrvTvdIncDCODEStaCnt()      (_sbTvdDCodeCnt = (_sbTvdDCodeCnt >= TVD_IS_DCODESTAON)?TVD_IS_DCODESTAON:(_sbTvdDCodeCnt+1))
#define vDrvTvdDecDCODEStaCnt()      (_sbTvdDCodeCnt = (_sbTvdDCodeCnt <= TVD_IS_DCODESTAOFF)?0x0:(_sbTvdDCodeCnt-1))
#define fgDrvTvdIsDCODEStaCnt(bCnt)    ((_sbTvdDCodeCnt==(bCnt))? TRUE : FALSE)
#endif
#endif
//--------- Counter for Hsync Slice Level adjust-----------//
#define TVD_HsliceReset   0x10
#define TVD_Hslice_threshold   0x20

#define vDrvTvdRstHsliceCnt()      (_sbHsliceCnt = TVD_HsliceReset)
#define vDrvTvdIncHsliceCnt()      (_sbHsliceCnt = (_sbHsliceCnt == TVD_Hslice_threshold)?TVD_Hslice_threshold:(_sbHsliceCnt+1))
#define vDrvTvdDecHsliceCnt()      (_sbHsliceCnt = (_sbHsliceCnt == 0)?0:(_sbHsliceCnt-1))
#define fgDrvTvdIsHsliceCnt(bCnt)    ((_sbHsliceCnt==(bCnt))? TRUE : FALSE)
#if TVD_ANA_COPY_PROTECT_FILTER
#define TVD_ANA_COPY_PRO_FILTER   8
#define TVD_ANA_COPY_PRO_MAX      255
#define fgDrvTvdIsPSync()         (_sbAnaCopyProPSync)
#define fgDrvTvdIsCSTRIPE()       (_sbAnaCopyProCSTRIPE)
#define fgDrvTvdIsMvType2()       (_sbAnaCopyProMvType2)
#define vDrvTvd3dAnaCopyProInit()  { \
    _su1AnaCopyProPSyncCnt    =0; \
    _su1AnaCopyProCSTRIPECnt  =0; \
    _su1AnaCopyProMvType2Cnt  =0; \
    _sbPreAnaCopyProPSync     =0; \
    _sbPreAnaCopyProCSTRIPE   =0; \
    _sbPreAnaCopyProMvType2   =0; \
    _sbAnaCopyProPSync        =0; \
    _sbAnaCopyProCSTRIPE      =0; \
    _sbAnaCopyProMvType2      =0; \
}
#else
#define fgDrvTvdIsPSync()         fgHwTvdIsPSync()
#define fgDrvTvdIsCSTRIPE()       fgHwTvdIsCSTRIPE()
#define fgDrvTvdIsMvType2()       fgHwTvdIsMvType2()
#endif
/**************************************************************************
 * Local/Static Functions
 *************************************************************************/
static void _svDrvTvdUpdateActiveWH(void)
{
    if(SV_VDO_STABLE == _rTvd3dStatus.bSigStatus)
    {
        _su2ActiveWidth = wTvdWidth();
        _su2ActiveHeight = wTvd3dHeight();
    }
}

static BOOL _svDrvTvdCheckSignalStatus(UINT8 bSigStatus)
{
    //static UINT8   _sbTvdModeCnt;
    //static UINT8   _sbTvd_McDone_cnt = 12;
#if TVD_CTRL_STABLE_MCNT
    if(_rTvd3dStatus.bSigStatus==bSigStatus && _sbTvdModeCnt>_sbTvd_McDone_cnt)
        return TRUE;
    else
        return FALSE;
#else
    if(_rTvd3dStatus.bSigStatus==bSigStatus)
        return TRUE;
    else
        return FALSE;
#endif
}

/**
 * @brief Set TVD Old Burst frequency detection Gain
 * @param  bUpGain: Up Gain
 * @param  bDnGain: Dn Gain
 * @retval void
 */
static void _svDrvTvdSetDET443Gain(UINT8 bUpGain, UINT8 bDnGain)
{
    vIO32WriteFldAlign(CDET_03, bUpGain, DET_443_GAIN_UP);// DET_443_GAIN_UP
    vIO32WriteFldAlign(CDET_03, bDnGain, DET_443_GAIN_DN);// DET_443_GAIN_DN
}


/**
 * @brief TVD SVF counter function
 *
 * This function only be called at isr
 *
 * @param None
 * @return None
 */
#if SUPPORT_S_DETECT
static void _svDrvTvdSVPresDetCount(void)
{
    UINT8 fgSVPres;
#ifdef  SUPPORT_AV_COMP_AUTO

    if((VSS_MAJOR(_bMonMainNew) != VSS_SV) && (VSS_MAJOR(_bMonSubNew) != VSS_SV))
    {
        return;
    }

#else

    if((_bMainMonICIn == P_FA) && (_bSubMonICIn == P_FA))
    {
        return;
    }

#endif
    fgSVPres = fgHwTvdVPresSVF();

    if(fgSVPres == _rTvd3dStatus.fgSVPres)
    {
        vDrvTvdIncSFirstCnt();

        if(fgDrvTvdIsSFirstCnt(TVD_SFIRST_CNT)&&(_sbSVFBypassNotify==SV_FALSE))
        {
            _sbSVFNotify=SV_TRUE;
        }
    }
    else
    {
        _rTvd3dStatus.fgSVPres = fgSVPres;
        vDrvTvdRstSFirstCnt();
    }
}
#endif
/**
 * @brief TVD load fix gain function
 *
 * This function only be called once when system start up
 *
 * @param None
 * @return None
 */
static void _svDrvTvdLoadFixGain(void)
{
    UINT32 _dwEFUSECVBS;
    UINT16 wTemp;

    if(DRVCUST_OptGet(eTVDSupportEFuseCVBS))
    {
        _dwEFUSECVBS = IO32ReadFldAlign(0xF000868c, AC_FULLDW);

        if((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_DONE)==0xaa))
        {
            _sbIsTVDFixAgc = SV_ON;
            _sbTvdMenuAcode = bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN0);
            _swTvdMenuDcode = (UINT16)(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN1)) |(((UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN2))<<8);
            vIO32WriteFldAlign(DFE_07, _sbTvdMenuAcode, AGC2_MANUAL_ACODE);
            vIO32WriteFldAlign(DFE_07, _swTvdMenuDcode, AGC2_MANUAL_DCODE);
        }
        else
        {
            _sbIsTVDFixAgc = SV_OFF;
        }

        if(_rTvd3dStatus.eSourceType!=SV_ST_TV)
        {
            if((bApiEepromReadByte(EEP_VIDEO_BYPASS_AV_EFUSE)!=0xaa) //CVBS EFUSE ADC
               &&((_dwEFUSECVBS>>29)==0x7)
               &&(((_dwEFUSECVBS&0xffff)<0x880)&&((_dwEFUSECVBS&0xffff)>0x780))
               &&((((_dwEFUSECVBS>>16) & 0xff)<=0x23)&&(((_dwEFUSECVBS>>16) & 0xff)>=0x1A))
              )
            {
                _sbIsTVDFixAgc = SV_ON;
                _sbTvdMenuAcode = (UINT8)(_dwEFUSECVBS>>16) & 0xff;
                _swTvdMenuDcode = (UINT16)_dwEFUSECVBS;

                if(DRVCUST_OptGet(eTVDSupportOutput216))
                    //				if(bApiEepromReadByte(EEP_VIDEO_SUPPORT_216)==0xaa)
                {
                    _swTvdMenuDcode = (_swTvdMenuDcode*939)/1024;
                }

                wTemp = DRVCUST_OptGet(eTVDSupportEFuseCVBSOffset);

                if(wTemp&0x8000)
                {
                    _swTvdMenuDcode = _swTvdMenuDcode - (wTemp&0x1ff);
                }
                else
                {
                    _swTvdMenuDcode = _swTvdMenuDcode + (wTemp&0x1ff);
                }

                vIO32WriteFldAlign(DFE_07, _sbTvdMenuAcode, AGC2_MANUAL_ACODE);
                vIO32WriteFldAlign(DFE_07, _swTvdMenuDcode, AGC2_MANUAL_DCODE);
            }
            else
            {
                if(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_DONE)==0xaa) //CVBS EEP ADC
                {
                    _sbIsTVDFixAgc = SV_ON;
                    _sbTvdMenuAcode = bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0);
                    _swTvdMenuDcode = (UINT16)(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN1)) |(((UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN2))<<8);
                    vIO32WriteFldAlign(DFE_07, _sbTvdMenuAcode, AGC2_MANUAL_ACODE);
                    vIO32WriteFldAlign(DFE_07, _swTvdMenuDcode, AGC2_MANUAL_DCODE);
                }
                else	//AGC
                {
                    _sbIsTVDFixAgc = SV_OFF;
                }
            }
        }
    }
    else
    {
        if(((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_DONE)==0xaa))
           ||(!(_rTvd3dStatus.eSourceType==SV_ST_TV) && (bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_DONE)==0xaa)))
        {
            _sbIsTVDFixAgc = SV_ON;

            //TVD_AUTO_CALIB_DONE = 1
            if(_rTvd3dStatus.eSourceType==SV_ST_TV)
            {
                _sbTvdMenuAcode = bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN0);
                _swTvdMenuDcode = (UINT16)(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN1)) |(((UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN2))<<8);
                vIO32WriteFldAlign(DFE_07, _sbTvdMenuAcode, AGC2_MANUAL_ACODE);
                vIO32WriteFldAlign(DFE_07, _swTvdMenuDcode, AGC2_MANUAL_DCODE);
            }
            else
            {
                _sbTvdMenuAcode = bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0);
                _swTvdMenuDcode = (UINT16)(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN1)) |(((UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN2))<<8);
                vIO32WriteFldAlign(DFE_07, _sbTvdMenuAcode, AGC2_MANUAL_ACODE);
                vIO32WriteFldAlign(DFE_07, _swTvdMenuDcode, AGC2_MANUAL_DCODE);
            }
        }
        else
        {
            _sbIsTVDFixAgc = SV_OFF;
        }
    }
}

#if TVD_NR_AVG
static UINT8 _svDrvTvdGetNRLevel(void)
{
    return RF_level;
}
#endif


#if TVD_USE_NEW_DAGC
#if TVD_DAGC_WA
/**
 * @brief Fine Tune for TVD FixGain Mode
 * @param bMcDone_Cnt IN: Mode Change Counter
 * @param bIsTvdFixAgc IN: TVD FixAgc Flag
 * @param bNaState IN: Noise State
 * @retval : NOON
 */
static void _svDrvTvdFixedGainFineTune(UINT8 bMcDone_Cnt, UINT8 bIsTvdFixAgc, UINT8 bNaState)
{
    if(fgDrvTvdModeCntGt(bMcDone_Cnt) && (bIsTvdFixAgc))
    {
        if(bNaState!=NA_SUPERHIGHNOISE)
        {
            if(IO32ReadFldAlign(DFE_STA_04, DGAIN_CODE)>(IO32ReadFldAlign(DFE_07, AGC2_MANUAL_DCODE)+TVD_DCODE_UPTH))
            {
                vDrvTvdIncDCODEStaCnt();

                if(fgDrvTvdIsDCODEStaCnt(TVD_IS_DCODESTAON))
                {
                    vIO32WriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL, FIXAGC_DAGCSEL);
                }
            }
            else if(IO32ReadFldAlign(DFE_STA_04, DGAIN_CODE)<(IO32ReadFldAlign(DFE_07, AGC2_MANUAL_DCODE)+TVD_DCODE_DNTH))
            {
                vDrvTvdDecDCODEStaCnt();

                if(fgDrvTvdIsDCODEStaCnt(TVD_IS_DCODESTAOFF))
                {
                    vIO32WriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_HighNA, FIXAGC_DAGCSEL);
                }
            }

            vTvdSetAGCMode(_sbIsTVDFixAgc?E_MENU_AGAIN:E_AUTO_GAIN);
        }
        else
        {
            vDrvTvdRstDCODEStaCnt();
            vIO32WriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_HighNA, FIXAGC_DAGCSEL);
            vTvdSetAGCMode(_sbIsTVDFixAgc?E_MENU_GAIN:E_AUTO_GAIN);
        }
    }
}
#else
static void _svDrvTvdFixedGainFineTune(UINT8 bMcDone_Cnt, UINT8 bIsTvdFixAgc)
{
    UINT32 u4DGain;

    if(bIsTvdFixAgc)
    {
        u4DGain = (0x800*IO32ReadFldAlign(DFE_STA_04, DGAIN_CODE))/IO32ReadFldAlign(DFE_07, AGC2_MANUAL_DCODE);
        vIO32WriteFldAlign(SVF_00, (UINT16)(u4DGain)>>6, TVDCTRL_MDAGC);
    }
}
#endif
#endif
static void _svDrvTvdQtestStaUpdate(void)
{
    if(_pfnTVDCLINotify!=NULL)
    {
        _rTvd3dStatus.fgHSYNC_LOCK = fgHwTvdHsyncLock();
        _rTvd3dStatus.wLine_err = wHwTvdLineErr();
        _rTvd3dStatus.fgVLOCK = fgHwTvdVLock();
        _rTvd3dStatus.fgHLOCK = fgHwTvdHLock();
        _rTvd3dStatus.LineInfo.wERR_VLEN=IO32ReadFldAlign(TG_STA_01, ERR_VLEN);
        _rTvd3dStatus.LineInfo.wLOCK_VLEN=IO32ReadFldAlign(TG_STA_01, LOCK_VLEN);
        _rTvd3dStatus.u4LLockStatus = IO32ReadFldAlign(TG_STA_03, LLOCK_STATUS);
        vIO32WriteFldAlign(SECAM_07, 8, YUV_CATCH_SEL);
        //vIO32WriteFldAlign(SECAM_07, u2X, YUV_CATCH_HPT);//four pixels
        vIO32WriteFldAlign(SECAM_07, 0x1B8, Fld(10, 10, AC_MSKW21));//one pixel
        vIO32WriteFldAlign(SECAM_07, 0x40, YUV_CATCH_VPT);
        _rTvd3dStatus.u4Y= IO32ReadFldAlign(STA_CTG_04, Fld(10, 0, AC_MSKW10));
        _rTvd3dStatus.u4U = IO32ReadFldAlign(STA_CTG_04, Fld(10, 12, AC_MSKW21));
        _rTvd3dStatus.u4V = IO32ReadFldAlign(STA_CTG_04, Fld(10, 22, AC_MSKW32));
    }
}

#if TVD_MODE_Deglitch

/**
 * @brief Mode Deglish Initial Function
 * @retval : NOON
 */
static void _svDrvTvdInitModeDeglish(void)
{
    if(bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)==1)
    {
        _sfgPreIs525_2= 0x1;
        _sbPreTvdMode_2= 0x3;
    }
    else
    {
#ifdef NEW_COUNTRY_TYPE

        if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_ATV_EU)
#else
        if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU)
#endif
        {
            _sfgPreIs525_2= 0x00;
        }
        else
        {
            _sfgPreIs525_2= 0x01;
        }

        _sbPreTvdMode_2= 0xff;
    }

    _sfgPreVPres_2= FALSE;
}

/**
 * @brief Mode Deglish Function
 * @param OUT:pointer to VPres status variable
 * @param OUT:pointer to pbTvdMode status variable
 * @param OUT:pointer to pfgIs525 status variable
 * @retval : NOON
 */
static UINT8 _sbDrvTvdModeDeglish(BOOL *pfgVPres , UINT8 *pbTvdMode, BOOL *pfgIs525)
{
    static  BOOL fgPreVPres_1=0;
    static  BOOL fgPreIs525_1=0;
    static  UINT8 bPreTvdMode_1=0xFF;

    if((fgPreVPres_1 != fgPreVPres_0)||
       (bPreTvdMode_1 != bHwTvdMode())||
       (fgPreIs525_1 != fgHwTvd525()))
    {
        vDrvTvdRstPreModeCnt();
        fgPreVPres_1 = fgPreVPres_0;
        bPreTvdMode_1 = bHwTvdMode();
        fgPreIs525_1 = fgHwTvd525();
        LOG(6,"[TVD_DBG_MSG] _sbDrvTvdModeDeglish fgPreVPres_1=%d, bPreTvdMode_1=%d, fgPreIs525_1=%d\n",
            fgPreVPres_1,bPreTvdMode_1,fgPreIs525_1);
    }

    vDrvTvdIncPreModeCnt();

    if((_swLLStatus_avg>0x10)&&(_rTvd3dStatus.eSourceType!=SV_ST_TV)
#if TVD_VCR_BV_STA
       &&fgGetTvdVCRBVSta()
#endif
      )
    {
        if(fgDrvTvdIsPreModeCnt(TVD_PreMCDONE_CNT_Vpres))
        {
            if((_sfgPreVPres_2 != fgPreVPres_1)||(_sfgPreIs525_2 != fgPreIs525_1))
            {
                _sfgPreVPres_2  = fgPreVPres_1;
                _sbPreTvdMode_2 = bPreTvdMode_1;
                _sfgPreIs525_2 = fgPreIs525_1;
                LOG(6,"[TVD_DBG_MSG] _sbDrvTvdModeDeglish Vpres modechange\n");
            }
        }

        if(fgDrvTvdIsPreModeCnt(TVD_PreMCDONE_CNT_Mode))
        {
            _sfgPreVPres_2  = fgPreVPres_1;
            _sbPreTvdMode_2 = bPreTvdMode_1;
            _sfgPreIs525_2 = fgPreIs525_1;
            LOG(6,"[TVD_DBG_MSG] _sbDrvTvdModeDeglish Mode modechange\n");
        }
    }
    else
    {
        _sfgPreVPres_2  = fgPreVPres_1;
        _sbPreTvdMode_2 = bPreTvdMode_1;
        _sfgPreIs525_2 = fgPreIs525_1;
    }

    *pfgVPres  = _sfgPreVPres_2;
    *pbTvdMode = _sbPreTvdMode_2;
    *pfgIs525 = _sfgPreIs525_2;
    return 0;
}
#endif


/**
 * @brief TVD ISR VCR Setting.
 * @param fgIsVCR IN: VCR Status.
 * @param bhwVCRSetting OUT: pointer to bhwVCRSetting variable.
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svDrvTvdVcrSet(BOOL fgIsVCR, UINT8 *pbhwVCRSetting, RTvdStatus *pTvd3dStatus)
{
    if(pTvd3dStatus->fgVPres && (fgDrvTvdModeCntGt(8)) && (pTvd3dStatus->bSigStatus != (UINT8)SV_VDO_NOSIGNAL))
    {
        // If VCR mode status is changed, reset VCR Counter
        if(fgIsVCR != pTvd3dStatus->fgIsVCR)
        {
            pTvd3dStatus->fgIsVCR = fgIsVCR;
            vDrvTvdRstVCRCnt();
        }

        if(IS_AUTOSEARCH())
        {
            // Reset VCR Counter after auto searching.  Benson 06.02.08
            vDrvTvdRstVCRCnt();
        }
        //************************************NonVCR Setting of TVD************************************
        else if(!pTvd3dStatus->fgIsVCR)
        {
            vDrvTvdDecVCRCnt();

            if(fgDrvTvdIsVCRCnt(TVD_VCROFF_CNT+1))  //check counter value is 1
            {
                vIO32WriteFldAlign(DFE_02, SV_ON, POS_DIFF_ONLY);
#if !TVD_VPRES_IMPROVE
                vDrvTvd3dVPresOnOff(0x12, DFE_NORMAL_ONOFF_THRD);
#endif
#if TVD_VCR_SECAM_DARK
                vIO32WriteFldMulti(CTG_07,
                                   P_Fld(1, FIXBLV)|
                                   P_Fld(1, SLOWBLC));
#endif
                *pbhwVCRSetting = (UINT8)VCR_NONE;
            }

            /*****************************TDC On/Off under NonVCR**************************/
#if TVD_VCR_PALSW_FAST

            if(fgDrvTvdIsVCRCnt(TVD_VCROFF_CNT+0xe3))  //check counter value is 0xe3
            {
                vIO32WriteFldAlign(CDET_05,  SV_OFF, PALSW_FAST);
                vIO32WriteFldAlign(CDET_05,  SV_OFF, PALSW_STEP_F);
            }

#endif
        }
        //**************************************VCR Setting of TVD************************************
        else if(pTvd3dStatus->fgIsVCR)
        {
            vDrvTvdIncVCRCnt();

            if(fgDrvTvdIsVCRCnt(TVD_VCRON_CNT-1))  //check VCR counter is 0xFE
            {
                vIO32WriteFldAlign(DFE_02, ((_na_state == NA_SUPERHIGHNOISE)?1:0), POS_DIFF_ONLY);
#if !TVD_VPRES_IMPROVE
                vDrvTvd3dVPresOnOff(0x21, DFE_NORMAL_ONOFF_THRD);
#endif
                *pbhwVCRSetting = (UINT8)VCR_MODE1;
#if TVD_VCR_SECAM_DARK

                if(pTvd3dStatus->bTvdMode==SV_CS_SECAM)
                {
                    vIO32WriteFldMulti(CTG_07,
                                       P_Fld(0, FIXBLV)|
                                       P_Fld(0, SLOWBLC));
                }
                else
                {
                    vIO32WriteFldMulti(CTG_07,
                                       P_Fld(1, FIXBLV)|
                                       P_Fld(1, SLOWBLC));
                }

#endif
                /*****************************PAL Switch Setting under VCR**************************/
                // Use a large counter(0x40)  for PALSW_FAST to on.
                //( To avoid fast Vdoswitch causing trick mode false alarm.)
#if TVD_VCR_PALSW_FAST

                if(vDrvTvdGetTVDNRAvg() < 0x15)
                {
                    vIO32WriteFldAlign(CDET_05,  SV_ON, PALSW_FAST);
                    vIO32WriteFldAlign(CDET_05,  SV_ON, PALSW_STEP_F);
                }
                else
                {
                    // Avoid false alarm for trick mode (pTvd3dStatus->fgIsVCR will on).
                    vIO32WriteFldAlign(CDET_05,  SV_OFF, PALSW_FAST);
                    vIO32WriteFldAlign(CDET_05,  SV_OFF, PALSW_STEP_F);
                }

#endif
            }
        }
    }

    //Improve VBI CC Performance
    if((_svDrvTvdGetNRLevel()>45) && (*pbhwVCRSetting==(UINT8)VCR_MODE1) && (fgHwTvdVCRBV()) && (!fgHwTvdIsMacroVision()) && (fgHwTvdVLock()) && fgDrvTvdModeCntGt(250))
    {
        vIO32WriteFldAlign(TG_04, SV_OFF, FAST_OFF);
    }
    else
    {
        vIO32WriteFldAlign(TG_04, SV_ON, FAST_OFF);
    }
}
/**
 * @brief TVD ISR adjust line threshold
 * @param IN:NOON
 * @retval : NOON
 */
static void _svDrvTvdAdjLineThr(void)
{
    if(IO32ReadFldAlign(TG_STA_05, LINE_STDFH_FLAG)) //AVG LINE In STD
    {
        #if TVD_VMASK_HEAD_SHAKING
        if ((_svDrvTvdGetNRLevel()> 60) && (_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
            && (_rTvd3dStatus.eSourceType == SV_ST_TV) && (_rTvd3dStatus.bTvdMode == SV_CS_PAL)
            && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
            && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
            && !fgHwTvdFHPos() && !fgHwTvdFHNeg() && !fgHwTvdCoChannel() && !fgHwTvdVCRBV()
            && (_rTvd3dStatus.fgTrick == 0) && (bhwVCRSetting == (UINT8)VCR_NONE)
            && (IO32ReadFldAlign(TG_STA_04, FRAME_DIFF_STDFV_VLD) && _u2AbsDiffStdFV > 0x1C)
            && ((_sbTvdModeCnt < 200 && _swLLStatus_avg > 0x6) ||  _swLLStatus_avg > 0xB)
            )
        {
            _u1LLockStableCnt = 0;
            vIO32WriteFldMulti(TG_17,
                               P_Fld(TG_LINE_STDFH_UP_VMASK, LINE_STDFH_UP)|
                               P_Fld(TG_LINE_STDFH_LO_VMASK, LINE_STDFH_LO));
        }
        else 
        #endif
        {
            vIO32WriteFldMulti(TG_17,
                               P_Fld(TG_LINE_STDFH_UP_STD, LINE_STDFH_UP)|
                               P_Fld(TG_LINE_STDFH_LO_STD, LINE_STDFH_LO));
        }
    }
    else
    {
        #if TVD_VMASK_HEAD_SHAKING
        if ((_svDrvTvdGetNRLevel()> 60) && (_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
            && (_rTvd3dStatus.eSourceType == SV_ST_TV) && (_rTvd3dStatus.bTvdMode == SV_CS_PAL)
            && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
            && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
            && !fgHwTvdFHPos() && !fgHwTvdFHNeg() && !fgHwTvdCoChannel() && !fgHwTvdVCRBV()
            && (_rTvd3dStatus.fgTrick == 0) && (bhwVCRSetting == (UINT8)VCR_NONE)
            && (IO32ReadFldAlign(TG_STA_04, FRAME_DIFF_STDFV_VLD) && _u2AbsDiffStdFV > 0x1C)
            && (_u1LLockStableCnt < 100 || _swLLStatus_avg < 8))
        {
            return ;
        }
        else
        #endif
        {
            vIO32WriteFldMulti(TG_17,
                               P_Fld(TG_LINE_STDFH_UP_NSTD, LINE_STDFH_UP)|
                               P_Fld(TG_LINE_STDFH_LO_NSTD, LINE_STDFH_LO));
        }
    }
}
/**
 * @brief TVD ISR adjust Again up/dn threshold
 * @param bAgainCode  IN:AgainCode value
 * @retval : NOON
 */
static void  _svDrvTvdAgainUpDnThr(UINT8 bAgainCode)
{
    if(bAgainCode < DFE_AGAIN_TH_SML)
    {
        vIO32WriteFldAlign(DFE_06,DFE_AGAIN_DN_RATIO_SML, AGC2_ACODE_DN_RATIO);
        vIO32WriteFldAlign(DFE_06,DFE_AGAIN_UP_RATIO_SML, AGC2_ACODE_UP_RATIO);
    }
    else
    {
        vIO32WriteFldAlign(DFE_06,DFE_AGAIN_DN_RATIO_NML, AGC2_ACODE_DN_RATIO);
        vIO32WriteFldAlign(DFE_06,DFE_AGAIN_UP_RATIO_NML, AGC2_ACODE_UP_RATIO);
    }
}

/**
 * @brief TVD ISR VCR_BV Setting
 * @param bNaState IN: Na State.
 * @param bMcDoneCnt IN:_sbTvd_McDone_cnt Value
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
#if TVD_VCR_BV
static void _svDrvTvdVcrBVSet(UINT16 wLLStatusAvg, UINT8 bNaState,  UINT8 bMcDoneCnt,RTvdStatus *pTvd3dStatus)
{
    UINT8 fgVCRBV;

#if TVD_DETECT_UNSTABLE_VCRBV
    UINT16 u2AbsDiff = IO32ReadFldAlign(TG_STA_04, ABS_DIFF_STDFV);
    UINT16 u28FieldDiff = IO32ReadFldAlign(TG_STA_04, STDFV_VAR_8FIELD);
    UINT8 u1UnstableTh = 0;
    UINT8 u1LLockStatusTh=0;

    if(_rTvd3dStatus.eSourceType == SV_ST_TV)
    {
        if(bNaState==NA_LOWNOISE && _svDrvTvdGetNRLevel()>=48)
        {
            u1UnstableTh = 0x20;
            u1LLockStatusTh = 0x10;
        }
        else if(bNaState==NA_LOWNOISE)
        {        
            u1UnstableTh = 0x20;
            u1LLockStatusTh = 0x10;
        }
        else if(bNaState==NA_HIGHNOISE)
        {
            u1UnstableTh = 0x80;
            u1LLockStatusTh = 0x18;
        }
        else if(bNaState==NA_SUPERHIGHNOISE)
        {
            u1UnstableTh = 0xA0;
            u1LLockStatusTh = 0x20;
        }

        
        if(fgDrvTvdIsModeCnt(0xFF))
        {
            _su2SumAbsDiff += u2AbsDiff;
            _su2Sum8FieldDiff += u28FieldDiff;
            VCRBV_AVG_COUNT++;
            
            if(VCRBV_AVG_COUNT==16)
            {
                _su2AvgAbsDiff = _su2SumAbsDiff/VCRBV_AVG_COUNT;
                _su2Avg8FieldDiff = _su2Sum8FieldDiff/VCRBV_AVG_COUNT;
                _su2SumAbsDiff = _su2Sum8FieldDiff = VCRBV_AVG_COUNT =0;
            }

            if(_su2AvgAbsDiff!=0 && _su2Sum8FieldDiff!=0)
            {
                if(((u2AbsDiff > _su2AvgAbsDiff+0x10) || (u28FieldDiff > _su2Avg8FieldDiff+0x10)))
                {
                    LOG(6, "[TVD_DBG_MSG] _su2AvgAbsDiff =0x%02X,  u2AbsDiff =0x%02X\n", _su2AvgAbsDiff, u2AbsDiff);
                    LOG(6, "[TVD_DBG_MSG] _su2Avg8FieldDiff =0x%02X,  u28FieldDiff =0x%02X\n", _su2Avg8FieldDiff, u28FieldDiff);
                    LOG(6, "[TVD_DBG_MSG] fgIsCoChannel =%d,  _swLLStatus_avg =0x%02X\n", pTvd3dStatus->fgIsCoChannel, _swLLStatus_avg);
                }
                if(((u2AbsDiff > _su2AvgAbsDiff+u1UnstableTh) || (u28FieldDiff > _su2Avg8FieldDiff+u1UnstableTh))
                && _su2AvgAbsDiff<0x10 && _su2Avg8FieldDiff<0x20
                && pTvd3dStatus->fgIsCoChannel==FALSE && _swLLStatus_avg>u1LLockStatusTh)
                {
                    LOG(6, "[TVD_DBG_MSG] UnstableVCRBV\n");
                    _sbUnsbableVCRBV = TRUE;
                }
            }
        }
        
        if(_sbUnsbableVCRBV==TRUE)
        {
            #if TVD_LIM_HERR_WA
            if(IO32ReadFldAlign(TG_04, LIM_HERR)==0 && _sbLimHerrEn1 && !_sbLimHerrEn2)
            {
                vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
                vIO32WriteFldAlign(TG_04, SV_ON, LF_AUTO);
                vIO32WriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
            }
            else
            #endif
            {
                vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
                vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
                vIO32WriteFldAlign(TG_04, TG_LF_KP0, LF_KP0);
                vIO32WriteFldAlign(TG_04, TG_LF_KI1, LF_KI1);
                if((bNaState==NA_HIGHNOISE || bNaState==NA_SUPERHIGHNOISE) && _svDrvTvdGetNRLevel()<IIR_LLOCK_EN_TH)
                {
                    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
                    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_PSEUDO_HSYNC);
                }
            }
        }
    }
#endif

    if(pTvd3dStatus->fgVPres&&(bNaState==NA_LOWNOISE))
    {
        fgVCRBV=fgHwTvdVCRBV();

        if(fgVCRBV!=pTvd3dStatus->fgIsVCRBV)
        {
            if((fgDrvTvdModeCntGt(bMcDoneCnt+10))&&fgVCRBV&&(!fgHwTvdVLock())&&(wLLStatusAvg>0x20))
            {
                vDrvTvdRstVCRBVCnt();
                vDrvTvdIncVCRBVCnt();
                vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
                vIO32WriteFldAlign(TG_17, SV_OFF, MHSS_EN);
                vIO32WriteFldAlign(TG_17, SV_OFF, MHSS);
                vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
                vIO32WriteFldAlign(TG_04, TG_LF_KP0, LF_KP0);
                vIO32WriteFldAlign(TG_04, TG_LF_KI1, LF_KI1);
                LOG(6,"[TVD_DBG_MSG] _svDrvTvdVcrBVSet LLOCK small gain !\n");
            }

            pTvd3dStatus->fgIsVCRBV=fgVCRBV;
        }
    }
}
#endif

/**
 * @brief TVD ISR Hsync slice leve adjust Setting
 * @param bNaState IN: Na State.
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svDrvTvdHSlicer(UINT8 bNaState, RTvdStatus *pTvd3dStatus)
{
#if TVD_NEW_HSYNCSLICER_SETTING
    if(!_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
    {
        vIO32WriteFldAlign(TG_00, 0x7, MIN_HWIDTH);
        vIO32WriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
		vDrvTvdRstHsliceCnt();
    }
    else
    {
        //UINT16 u2SyncLv = bHwTvdSYNCLV();
        //LOG(1, "_u2SyncLv=%d\n", u2SyncLv);
        _u4SyncLvSum += bHwTvdSYNCLV();
        _u4BYCLvSum  += IO32ReadFldAlign(DFE_STA_02, BYC_LV);
        LOG(9, "_u4SyncLvSum=%d , _u4BYCLvSum=%d\n", _u4SyncLvSum,_u4BYCLvSum);
        _u1SyncLvCnt++;
        if(_u1SyncLvCnt==16)
        {
            _u2SyncLvAvg = _u4SyncLvSum>>4;
            _u1SyncLvCnt = _u4SyncLvSum = 0; 
            _u2BYCLvAvg  = _u4BYCLvSum >> 4;
            _u4BYCLvSum  = 0;
            LOG(6, "==> _u2SyncLvAvg=%d, _u2BYCLvAvg=%d\n", _u2SyncLvAvg,_u2BYCLvAvg);
        }
        if(_na_state==NA_LOWNOISE && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
        && !fgHwTvdFHPos() && !fgHwTvdFHNeg() && !fgHwTvdCoChannel() && !fgHwTvdVCRBV()
        && _u2SyncLvAvg>0 && _u2SyncLvAvg<0x50 && IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)<0x38 && IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)>0x21
        && _rTvd3dStatus.bTvdMode == SV_CS_PAL_M
        )
        {
            vIO32WriteFldAlign(TG_0C, 0x48, HSLICE_ADJ);
        }
        else if(_na_state==NA_LOWNOISE && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
        && !fgHwTvdFHPos() && !fgHwTvdFHNeg() && !fgHwTvdCoChannel() && !fgHwTvdVCRBV()
        && _u2SyncLvAvg>0 && _u2SyncLvAvg<0x40 && IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)<0x20
        && _rTvd3dStatus.bTvdMode == SV_CS_NTSC358
        )
        {
            vIO32WriteFldAlign(TG_0C, 0x4, HSLICE_ADJ);
        }
        else
        {
             if(_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
		    {
		        if(bNaState!=NA_SUPERHIGHNOISE)
		        {
		            if(bHwTvdSYNCLV()< 0xA0)
		            {
		                vDrvTvdIncHsliceCnt();

		                if(fgDrvTvdIsHsliceCnt(TVD_Hslice_threshold-1))
		                {
		                    vIO32WriteFldAlign(TG_0C, TG_HSLICE_NORMAL, HSLICE_ADJ);
		                    LOG(6,"[TVD_DBG_MSG] _svDrvTvdHSlicer TG_HSLICE_NORMAL !\n");
		                }
		            }
		            else if(bHwTvdSYNCLV()> 0xA8)
		            {
		                vDrvTvdDecHsliceCnt();

		                if(fgDrvTvdIsHsliceCnt(1))
		                {
		                    vIO32WriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
		                    LOG(6,"[TVD_DBG_MSG] _svDrvTvdHSlicer TG_HSLICE_DEFAULT !\n");
		                }
		            }
		        }
		        else
		        {
		            vDrvTvdRstHsliceCnt();
		            vIO32WriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
		        }
		    }
		    else
		    {
		        vDrvTvdRstHsliceCnt();
		        vIO32WriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
		    }
        }

        //Sync Level 25%
        #if 0
        if((_na_state==NA_LOWNOISE && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
        && !fgHwTvdFHPos() && !fgHwTvdFHNeg()
        /*&& !fgHwTvdHeadSwitch() && !fgHwTvdTrick() && (bhwVCRSetting==(UINT8)VCR_NONE)*/
        && !fgHwTvdCoChannel() && !fgHwTvdVCRBV() /*&& !fgHwTvdCNRLow()*/
        /*&& IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)>0x50 && IO32ReadFldAlign(STA_CDET_01, CAGC_STA)<0x32*/)
        ||
        #endif
        if(_na_state==NA_LOWNOISE && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
        && !fgHwTvdFHPos() && !fgHwTvdFHNeg() && IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)>0x38)
        {
            if(_rTvd3dStatus.eSourceType==SV_ST_TV)
                vIO32WriteFldAlign(TG_00, 0xC, MIN_HWIDTH);
        }
        else if(_na_state==NA_LOWNOISE && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
        && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
        && !fgHwTvdFHPos() && !fgHwTvdFHNeg() && !fgHwTvdCoChannel() && !fgHwTvdVCRBV()
        && ((_u2SyncLvAvg>0 && _u2SyncLvAvg<0x40 && IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)<0x20
        && _rTvd3dStatus.bTvdMode == SV_CS_NTSC358)
        #if TVD_SMALL_HWIDTH_SHAKING
        || ((_u2SyncLvAvg > 0x5a) && (_u2BYCLvAvg < 0x20) && (_rTvd3dStatus.bTvdMode == SV_CS_PAL))
        #endif
        )
        )
        {            
            if(_rTvd3dStatus.eSourceType==SV_ST_TV)
                vIO32WriteFldAlign(TG_00, 0xA, MIN_HWIDTH);
        }           
        else
        {
            vIO32WriteFldAlign(TG_00, 0x7, MIN_HWIDTH);
        }
    }
#else
    if(_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
    {
        if(bNaState!=NA_SUPERHIGHNOISE)
        {
            if(bHwTvdSYNCLV()< 0xA0)
            {
                vDrvTvdIncHsliceCnt();

                if(fgDrvTvdIsHsliceCnt(TVD_Hslice_threshold-1))
                {
                    vIO32WriteFldAlign(TG_0C, TG_HSLICE_NORMAL, HSLICE_ADJ);
                    LOG(6,"[TVD_DBG_MSG] _svDrvTvdHSlicer TG_HSLICE_NORMAL !\n");
                }
            }
            else if(bHwTvdSYNCLV()> 0xA8)
            {
                vDrvTvdDecHsliceCnt();

                if(fgDrvTvdIsHsliceCnt(1))
                {
                    vIO32WriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
                    LOG(6,"[TVD_DBG_MSG] _svDrvTvdHSlicer TG_HSLICE_DEFAULT !\n");
                }
            }
        }
        else
        {
            vDrvTvdRstHsliceCnt();
            vIO32WriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
        }
    }
    else
    {
        vDrvTvdRstHsliceCnt();
        vIO32WriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
    }
#endif
}

/**
 * @brief TVD ISR Channel Change
 * @param IsTvdFixAgc IN: tvd fix AGC status
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void  _svDrvTvdChanChg(RTvdStatus *pTvd3dStatus,UINT8 bIsTVDFixAgc)

{
    if(pTvd3dStatus->eSourceType==SV_ST_TV)
    {
        if(fgDrvTvdIsChanchgCnt(CHANCHG_START))
        {
#if TVD_PHALT_MN_WA2
            _sbForcePalMN = FALSE;

            if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
            {
                vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE off (Channel Chg) \n");
            }

#endif
            LOG(6,"[TVD_DBG_MSG] _svDrvTvdChanChg CHANCHG_START\n");
            // Fast Chroma Lock.
#if TVD_FRS_FOR_NONSTDSIGNAL
            vDrvTvdRstNONSTDCnt();
#endif
            // Reset HLock Cnt, OldVCR Cnt and Apply Auto Line Lock.
            vDrvTvdRstHLOCKCnt();
            //Enable LF_AUTO
            if(_sbManualLLockGainbyDDI==FALSE)
                vIO32WriteFldAlign(TG_04, SV_ON, LF_AUTO); 
            // Fast VLock.
            if(!IS_AUTOSEARCH())
            {
                vIO32WriteFldAlign(TG_08, SV_ON, FAST_VALIGN);
            }
            else
            {
                vIO32WriteFldAlign(TG_08, SV_OFF, FAST_VALIGN);
            }

            // To avoid channel change AGC speed too slow, so set to defaule gain during mode change.   Mark 070711
            if(bIsTVDFixAgc != SV_ON)
            {
                vTvdSetAGCMode(E_HOLD_GAIN);
            }
        }
        else if(fgDrvTvdIsChanchgCnt(CHANCHG_DONE))
        {
            LOG(6,"[TVD_DBG_MSG] _svDrvTvdChanChg CHANCHG_DONE\n");
            vIO32WriteFldAlign(TG_08, SV_OFF, FAST_VALIGN);

            // To avoid channel change AGC speed too slow, so set to defaule gain during mode change.   Mark 070711
            if(bIsTVDFixAgc != SV_ON)
            {
                vTvdSetAGCMode(E_AUTO_GAIN);
            }
        }
    }

    vDrvTvdDecChanchgCnt();
}
/**
 * @brief TVD ISR AVG_SYNC_EN Enable/Disable Setting
 * @param fgvpres IN: vpres Status.
 * @param bNaState IN: Na State.
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
#if TVD_AVG_SYNC_DET
static void  _svDrvTvdAvgSyncEnableSet(UINT8 bNaState)
{
    vIO32WriteFldAlign(CTG_06, 0x3, HSYNC_WIDTH_STA_SEL);//Enable H_Sync Width Sta Sel

    if((bHwTvdNRLevel()<0x25) && !IS_FH_NON_STD() &&
       (IO32ReadFldAlign(TG_STA_02, HSYNCWIDTH)>0x16) && (IO32ReadFldAlign(TG_STA_02, HSYNCWIDTH)<0x60))
    {
        vIO32WriteFldAlign(DFE_24, SV_ON, AVG_SYNC_EN);//Enable Average Sync Mode
    }
    else
    {
        vIO32WriteFldAlign(DFE_24, SV_OFF, AVG_SYNC_EN);//Disable Average Sync Mode
    }
}
#endif

/**
 * @brief TVD ISR STD/NSTD Line Setting
 * @param fgvpres IN: vpres Status.
 * @param bNaState IN: Na State.
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void  _svDrvTvdStdNStdLineSet(UINT8 bNaState,RTvdStatus *pTvd3dStatus)

{
    (pTvd3dStatus->LineInfo).wLinecnt=IO32ReadFldAlign(TG_STA_05, STA_LCNT);

    if(!pTvd3dStatus->fgVPres)
    {
        (pTvd3dStatus->LineInfo).wLinecnt=1716;
    }

    if(IS_LINE_NON_STD((pTvd3dStatus->LineInfo).wLinecnt))
    {
        vIO32WriteFldAlign(DFE_01, DFE_BLANK_WIN_START_NSTD_L, BLANK_WIN_START);
        vIO32WriteFldAlign(TG_0D, SV_ON, HLEN_FHPOS_EN);
        vIO32WriteFldAlign(CDET_00, SV_ON, MDET_V525_SEL);
        //vIO32WriteFldAlign(TG_04, SV_ON, LF_OFFSET_EN);
    }
    else if(IS_LINE_STD((pTvd3dStatus->LineInfo).wLinecnt))
    {
        vIO32WriteFldAlign(DFE_01, DFE_BLANK_WIN_START_STD_L, BLANK_WIN_START);
        vIO32WriteFldAlign(TG_0D, SV_OFF, HLEN_FHPOS_EN);
        vIO32WriteFldAlign(CDET_00, SV_OFF, MDET_V525_SEL);
        //vIO32WriteFldAlign(TG_04, SV_OFF, LF_OFFSET_EN);
    }

    if(bNaState == (UINT8)NA_LOWNOISE)
    {
        if(IS_LINE_NON_STD((pTvd3dStatus->LineInfo).wLinecnt))
        {
            vIO32WriteFldAlign(CTG_05, SV_ON,BST_START_SEL);//?SHI
            vIO32WriteFldAlign(CTG_00, SV_ON, SOBVLD_MASK_EN); //auto  //?SHI
        }
        else if(IS_LINE_STD((pTvd3dStatus->LineInfo).wLinecnt))
        {
            vIO32WriteFldAlign(CTG_05, SV_OFF,BST_START_SEL);
            vIO32WriteFldAlign(CTG_00, SV_OFF, SOBVLD_MASK_EN); //force valid
        }

        if(fgHwTvdSVID()) //?SHI
        {
            vIO32WriteFldAlign(CTG_00, SV_ON, SOBVLD_MASK_EN); //auto
        }
    }
    else
    {
        vIO32WriteFldAlign(CTG_05, SV_OFF, BST_START_SEL);
        vIO32WriteFldAlign(CTG_00, SV_OFF, SOBVLD_MASK_EN); //force valid

        if(fgHwTvdSVID())
        {
            vIO32WriteFldAlign(CTG_00, SV_ON, SOBVLD_MASK_EN); //auto
        }
    }
}
/**
 * @brief TVD ISR Turn on/off Fast vlock
 * @param  IN:NOON
 * @retval : NOON
 */
#if TVD_FORCE_FAST_VALIGN
static void _svDrvTvdFastValign(void)
{
    UINT16 weer_vlen;
    UINT16 wlock_vlen;
    weer_vlen=IO32ReadFldAlign(TG_STA_01, ERR_VLEN);
    wlock_vlen=IO32ReadFldAlign(TG_STA_01, LOCK_VLEN);

    if(weer_vlen >= wlock_vlen)
    {
        weer_vlen= weer_vlen-wlock_vlen;
    }

    if(fgDrvTvdIsChanchgCnt(CHANCHG_STOP))
    {
        if(weer_vlen > 20)
        {
            vIO32WriteFldAlign(TG_08,SV_ON,FAST_VALIGN);
            //			LOG(6,"[TVD_DBG_MSG] _svDrvTvdFastValign FAST_VALIGN on \n");
        }
        else
        {
            vIO32WriteFldAlign(TG_08,SV_OFF,FAST_VALIGN);
        }
    }
}

#endif
/**
 * @brief TVD ISR CO IIR Check
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svDrvTvdCOIIRCHK(RTvdStatus *pTvd3dStatus, UINT8 bAvgCOCH_detect)
{
    if(!IS_AUTOSEARCH())
    {
        if((IO32ReadFldAlign(DFE_STA_08, COCH_IIR)>COIIR_TH_HIGH)&&(bHwTvdNRLevel()<0x40))  //Co-channel level
        {
            vDrvTvdIncCOCH2ENCnt();

            if(fgDrvTvdIsCOCH2ENCnt(TVD_COCH2EN_CNT-1))
            {
#if TVD_VPRES_IMPROVE
                vDrvTvd3dVPresOnOff(0x31, DFE_NORMAL_ONOFF_THRD);
#endif
#if TVD_SERRTYPE_ONE
                vIO32WriteFldAlign(TG_07, SV_OFF, SERR_TYPE);
#endif
                vIO32WriteFldAlign(DFE_18,SV_ON,PVSYNC_COCH_EN);//pseudo-Vsync minimum point generate Vsync
                vIO32WriteFldAlign(VSRC_08, SV_ON, COCH2_EN);//line avg compensate CCI(CoChannel Interference)
                LOG(6,"[TVD_DBG_MSG] _svDrvTvdCOIIRCHK COCH2 Enable !\n");
            }
        }
        else if((IO32ReadFldAlign(DFE_STA_08, COCH_IIR)<COIIR_TH_LOW)||(bHwTvdNRLevel()>0x40))
        {
            vDrvTvdDecCOCH2ENCnt();

            if(fgDrvTvdIsCOCH2ENCnt(TVD_COCH2EN_CNT2+1))
            {
#if TVD_SERRTYPE_ONE

                //Keep Original COCH setting.
                if(_sbSerrType0==FALSE)
                {
                    if((bAvgCOCH_detect)||(pTvd3dStatus->bTvdMode == SV_CS_PAL_M))
                    {
                        vIO32WriteFldAlign(TG_07, SV_OFF, SERR_TYPE);
                    }
                    else
                    {
                        vIO32WriteFldAlign(TG_07, SV_ON, SERR_TYPE);
                    }
                }
#endif
                vIO32WriteFldAlign(DFE_18,SV_OFF,PVSYNC_COCH_EN);
                vIO32WriteFldAlign(VSRC_08, SV_OFF, COCH2_EN);
                LOG(6,"[TVD_DBG_MSG] _svDrvTvdCOIIRCHK COCH2 Disable !\n");
            }

#if TVD_VPRES_IMPROVE
            else if(fgDrvTvdIsCOCH2ENCnt(TVD_COCH2EN_CNT2))
            {
                if(pTvd3dStatus->fgVPres)
                {
                    vDrvTvdIncVP1Cnt();

                    if(fgDrvTvdVP1CntGt(TVD_VP1OFFON_threshold))
                    {
                        if(IO32ReadFldAlign(DFE_STA_07, VPRES_VFLAG))
                        {
							vDrvTvdIncVPresVFlagCnt();
                            #if TVD_PICTURE_WHITE_FLASH
                            if (bHwTvdNRLevel()>0x40 || fgDrvTvdVPresVFlagCnt(TVD_VPresVFlag_upth))
                            #endif
                            {
                                vDrvTvd3dVPresOnOff(0x21, DFE_NORMAL_ONOFF_THRD);
                            }
                        }
                        else
                        {
                            vDrvTvdDecVPresVFlagCnt();
                            #if TVD_PICTURE_WHITE_FLASH
                            if (bHwTvdNRLevel()>0x40 || fgDrvTvdVPresVFlagCnt(TVD_VPresVFlag_dnth))
                            #endif
                            {
                                vDrvTvd3dVPresOnOff(0x33, DFE_NORMAL_ONOFF_THRD);
                            }
                        }
                    }
                    else
                    {
                        vDrvTvdRstVPresVFlagCnt();
                        vDrvTvd3dVPresOnOff(0x21, DFE_NORMAL_ONOFF_THRD);
                    }
                }
                else
                {
                    if(bHwTvdNRLevel()>0x10)
                    {
                        vDrvTvdRstVP1Cnt();
                        vDrvTvd3dVPresOnOff(0x35, DFE_NORMAL_ONOFF_THRD);
                    }
                    else
                    {
                        vDrvTvd3dVPresOnOff(0x12, DFE_NORMAL_ONOFF_THRD);
                    }
					vDrvTvdRstVPresVFlagCnt();
                }
            }

#endif
        }

        if(fgDrvTvdIsCOCHDeCnt(TVD_COCHDe_CNT))
        {
            if(fgDrvTvdIsCOCH2ENCnt(TVD_COCH2EN_CNT))
            {
                vIO32WriteFldAlign(TG_17, SV_OFF, MHSS_EN);
                vIO32WriteFldAlign(TG_17, SV_OFF, MHSS);
            }
            else if(fgDrvTvdIsCOCH2ENCnt(TVD_COCH2EN_CNT2))
            {
                vIO32WriteFldAlign(TG_17, SV_ON, MHSS_EN);
                vIO32WriteFldAlign(TG_17, SV_OFF, MHSS);
            }
        }
        else if(fgDrvTvdIsCOCHDeCnt(TVD_COCHDe_CNT2+1))
        {
            vIO32WriteFldAlign(TG_17, SV_OFF, MHSS_EN);
            vIO32WriteFldAlign(TG_17, SV_OFF, MHSS);
            LOG(6,"[TVD_DBG_MSG] _svDrvTvdCOIIRCHK HSS AUTO !\n");
        }
    }
    else //Auto Search : SERR_TYPE = 0, Disable COCH2 and PVSYNC_COCH
    {
        //vIO32WriteFldAlign(TG_07, SV_OFF, SERR_TYPE);  // mtk70117 marked for finetune V-shifting
        vIO32WriteFldAlign(DFE_18,SV_OFF,PVSYNC_COCH_EN);
        vIO32WriteFldAlign(VSRC_08, SV_OFF, COCH2_EN);
    }
}
/**
 * @brief TVD ISR NA State Check
 * @param bNaState IN: Na State.
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svDrvTvdNAStateCHK(UINT8 bNaState, RTvdStatus *pTvd3dStatus)
{
    if(bNaState == (UINT8)NA_LOWNOISE)
    {
        vIO32WriteFldAlign(CDET_06, CDET_KBURST_PAL, KBURST);

        if(pTvd3dStatus->bTvdMode==SV_CS_SECAM)
        {
            vIO32WriteFldAlign(CTG_03,SV_OFF, CTG_VUP_EN);
        }
        else
        {
            if(pTvd3dStatus->bCAGC < 0x08)
            {
                vIO32WriteFldAlign(CTG_03,SV_ON, CTG_VUP_EN);
            }
            else
            {
                vIO32WriteFldAlign(CTG_03,SV_OFF, CTG_VUP_EN);
            }
        }

        vIO32WriteFldAlign(CDET_00, SV_OFF, DET443_SEL);//DET443_SEL=1:det by f otherwise by energy

        if((pTvd3dStatus->bCAGC > 0xD0)&&(pTvd3dStatus->fgIsBurstLock == 0))
        {
            switch(pTvd3dStatus->bTvdMode)
            {
                case SV_CS_PAL:
                case SV_CS_SECAM:
                case SV_CS_PAL_60:
                case SV_CS_NTSC443:
                    _svDrvTvdSetDET443Gain(1, 0);
                    break;

                default:
                    _svDrvTvdSetDET443Gain(0, 1);
                    break;
            };
        }
        else
        {
            _svDrvTvdSetDET443Gain(0, 0);
        }
    }
    else // High and Super Hight NA
    {
        if(bNaState == (UINT8)NA_HIGHNOISE)
        {
            vIO32WriteFldAlign(CDET_06, CDET_KBURST_PAL, KBURST);
        }
        else
        {
            vIO32WriteFldAlign(CDET_06, 0xff, KBURST);
        }

        vIO32WriteFldAlign(CDET_00, SV_ON, DET443_SEL);
        vIO32WriteFldAlign(CTG_03,SV_OFF, CTG_VUP_EN);
    }
}


/**
 * @brief TVD ISR Mode switch protection
 * @param fgvpres IN: vpres Status.
 * @param bTvdMode IN: bTvdMode
  * @param bMcDoneCnt IN:_sbTvd_McDone_cnt Value
 * @retval : NOON
 */
#if TVD_MODESWITCH_PROTECT
static void _svDrvTvdModeSwPrtec(BOOL fgVPres,UINT8 bTvdMode, UINT8 bMcDoneCnt)
{
    if(fgDrvTvdIsModeCnt(bMcDoneCnt-9))
    {
        if(fgVPres)
        {
            if(IO32ReadFldAlign(STA_CDET_00, PHALT_TVD3D))
            {
                switch(bTvdMode)
                {
                    case SV_CS_NTSC358:
                    case SV_CS_NTSC443:
                    case SV_CS_SECAM:
                        //LOG(1, "TVD_MODESWITCH_PROTECT: Set PAL MODE=0 Cnt=%d\n", _bTvdModeCnt);
                        vIO32WriteFldAlign(CDET_08, 0x0, CDET_PAL_MODE);
                        break;

                    default:
                        break;
                };
            }
        }
    }
    else if(fgDrvTvdIsModeCnt(bMcDoneCnt))
    {
        //LOG(1,"TVD_MODESWITCH_PROTECT: Set PAL MODE=3 Cnt=%d\n", _bTvdModeCnt);
        vIO32WriteFldAlign(CDET_08, 0x3, CDET_PAL_MODE);
    }
}
#endif
/**
* @brief TVD ISR Non STD FH Setting
* @param  IN :NOON
* @retval : NOON
*/
static void  _svDrvTvdNonStdFhSet(void)
{
    if(IS_FH_NON_STD())  //Non STD FH  include FH_POS and FH_NEG
    {
        vIO32WriteFldMulti(CDET_04,
                           P_Fld(SV_OFF, CTG_NA_SEL)|
                           P_Fld(SV_OFF, CDET_NA_SEL));
        vIO32WriteFldAlign(CTG_00, 0x0, CTG_SWLBF); //free run for chroma
    }
    else
    {
        vIO32WriteFldMulti(CDET_04,
                           P_Fld(SV_ON, CTG_NA_SEL)|
                           P_Fld(SV_ON, CDET_NA_SEL));

        if(!fgHwTvdSVID())
        {
            vIO32WriteFldAlign(CTG_00, 0x1, CTG_SWLBF);
        }
    }
}
/**
* @brief TVD ISR HLOCK detection
* @param  PfgHLOCK OUT:pointer to fghlock status
* @retval : NOON
*/
static void  _svDrvTvdHLockDET(BOOL *PfgHLOCK)
{
    if(fgHwTvdHLock())
    {
        vDrvTvdIncHLOCKCnt();

        if(fgDrvTvdIsHLOCKCnt(TVD_HLOCKDONE_CNT-1))
        {
            *PfgHLOCK = 1;
        }
    }
    else
    {
        vDrvTvdDecHLOCKCnt();

        if(fgDrvTvdIsHLOCKCnt(TVD_HLOCKDONE_CNT2+1))
        {
            *PfgHLOCK = 0;
        }
    }
}

/**
* @brief TVD ISR Dclamp on/off condition check
* @param bHwVCRSetting IN: HW VCR Setting.
* @param PTvd3dNSTDStatus INOUT:pointer to RTvdNSTDStatus status
* @param pTvd3dStatus INOUT:pointer to RTvdStatus status
* @retval : NOON
*/
static void  _svDrvTvdDclampCHK(UINT8 bHwVCRSetting,RTvdNSTDStatus *PTvd3dNSTDStatus,RTvdStatus *pTvd3dStatus)
{
    if(((TVDAbsDiff(PTvd3dNSTDStatus->wHTotal,1716)<3)||(TVDAbsDiff(PTvd3dNSTDStatus->wHTotal,1728)<3))
       &&((TVDAbsDiff(PTvd3dNSTDStatus->wVTotal,525)<10) || (TVDAbsDiff(PTvd3dNSTDStatus->wVTotal,625)<10))
       &&(!pTvd3dStatus->fgIsCoChannel)
       &&((pTvd3dStatus->fgTrick == 0)&&(bHwVCRSetting==(UINT8)VCR_NONE))
      )
    {
        if(_svDrvTvdGetNRLevel()>75)
        {
            vDrvTvdIncDCLAMPCnt();

            if(fgDrvTvdIsDCLAMPCnt(TVD_DCLAMPON_CNT-1))
            {
                if(IO32ReadFldAlign(DFE_STA_05, CP_CUR)> 0x14)
                {
                    vIO32WriteFldAlign(DFE_08, SV_ON, DCLAMP_Y_EN);
                    vIO32WriteFldAlign(DFE_0E, SV_ON, DCLAMP_CHECK_LIM);	//new reg for 8223
                    vIO32WriteFldAlign(DFE_02, TVD_DCLAMP_Y4H, Y4H_BW);
					//vIO32WriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VSAG, CLAMP_TABLE_05);
                    LOG(6,"[TVD_DBG_MSG] _svDrvTvdDclampCHK Dclamp Enable !\n");
                }
                else
                {
                    vIO32WriteFldAlign(DFE_08, SV_OFF, DCLAMP_Y_EN);
                    vIO32WriteFldAlign(DFE_0E, SV_OFF, DCLAMP_CHECK_LIM);	//new reg for 8223
                    vIO32WriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW);
					//vIO32WriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
                    LOG(6,"[TVD_DBG_MSG] _svDrvTvdDclampCHK Dclamp CP_CUR Disable !\n");
                }

                vIO32WriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VSAG, CLAMP_TABLE_05);
            }
        }
        else if(_svDrvTvdGetNRLevel()<65)
        {
            vDrvTvdDecDCLAMPCnt();

            if(fgDrvTvdIsDCLAMPCnt(TVD_DCLAMPOFF_CNT+1))
            {
                vIO32WriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW);
                vIO32WriteFldAlign(DFE_08, SV_OFF, DCLAMP_Y_EN);
                vIO32WriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
                LOG(6,"[TVD_DBG_MSG] _svDrvTvdDclampCHK Dclamp NRLevel Disable !\n");
            }
        }
    }
    else
    {
        vDrvTvdDecDCLAMPCnt();

        if(fgDrvTvdIsDCLAMPCnt(TVD_DCLAMPOFF_CNT+1))
        {
            vIO32WriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW);
            vIO32WriteFldAlign(DFE_08, SV_OFF, DCLAMP_Y_EN);
            vIO32WriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
        }
    }
}
/* @brief Set Secam Burst start/end window.
* @param  bStart : start point
* @param  bEnd   : end point
* @retval void
*/
static void _svDrvTvdSetSCMBurstWin(UINT8 bStart, UINT8 bEnd) reentrant
{
    vIO32WriteFldMulti(CDET_02,P_Fld(bStart,BI_START)|P_Fld(bEnd,BI_END));
}

/**
 * @brief TVD ISR SECAM Detection
 * @param bMcDoneCnt IN:_sbTvd_McDone_cnt Value
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svDrvTvdSecamDET(RTvdStatus *pTvd3dStatus, UINT8 bMcDoneCnt)
{
#if TVD_SECAM_DET //when !BLOCK move windows to SECAM

    if(fgDrvTvdModeCntGt(bMcDoneCnt-2) && pTvd3dStatus->fgVPres)
    {
        if(pTvd3dStatus->fgIsBurstLock&&((pTvd3dStatus->bTvdMode==SV_CS_SECAM)||(pTvd3dStatus->bTvdMode==SV_CS_PAL)))
        {
            vDrvTvdIncBurstCnt();

            if(fgDrvTvdIsBurstCnt(TVD_BurstLock_CNT-1))
            {
                _svDrvTvdSetSCMBurstWin(0x40, 0x80);
                LOG(6,"[TVD_DBG_MSG] _svDrvTvdSecamDET BurstWin 0x4080\n");
            }
        }
        else
        {
            vDrvTvdDecBurstCnt();

            if(fgDrvTvdIsBurstCnt(TVD_BurstLock_CNT2+1))
            {
                _svDrvTvdSetSCMBurstWin(0x30, 0xb0);
                LOG(6,"[TVD_DBG_MSG] _svDrvTvdSecamDET BurstWin 0x30b0\n");
            }
        }
    }

#else

    if((pTvd3dStatus->bCAGC<DET443CAGCThres) && (!pTvd3dStatus->fgIsCoChannel) &&
       ((pTvd3dStatus->bTvdMode!=SV_CS_SECAM)||(!_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)))
      )
    {
        if(vDrvTvdGetTVDNRAvg() <= 0x09)
        {
            _svDrvTvdSetSCMBurstWin(0x40, 0x80);
        }
        else if(vDrvTvdGetTVDNRAvg() >= 0x0C)
        {
            _svDrvTvdSetSCMBurstWin(0x30, 0xa0);
        }
    }
    else
    {
        // Fix high noise SECAM sensitivity.
        _svDrvTvdSetSCMBurstWin(0x30, 0xb0);
    }

#endif
}
/**
 * @brief Set TVD Target Sync Height Value
 * @param :Noon
 */
static void _svDrvTvdSetTVDAGCTarget(UINT16 value)
{
    vIO32WriteFldAlign(DFE_03, value, AGC2_TARGET_SYNC_HEIGHT);
}

/**
 * @brief TVD ISR COCH Adjust
 * @param fgCoChannel IN: fgIsCoChannel Status.
 * @param wAGCTargetSyncHeight IN: _swAGCTargetSyncHeight value
 * @retval : NOON
 */
static void _svDrvTvdCochAdj(BOOL fgCoChannel,UINT16 wAGCTargetSyncHeight)
{
    if(fgCoChannel)
    {
        vDrvTvdIncCOCHDeCnt();
		vDrvTvdRstHsyncPosDetCnt();
        if(fgDrvTvdIsCOCHDeCnt(TVD_COCHDe_CNT-1))
        {
#ifdef TVD_RTM1_CM
            vIO32WriteFldAlign(DFE_0D, 0, CLAMP_ATD_GAIN);
            vIO32WriteFldAlign(DFE_0A, 0x6, CLAMP_FINE_RATIO);
#endif
            vIO32WriteFldAlign(DFE_02, SV_OFF, BLV_LIM_AUTO); //Avoid cochannel wrong measure.
            _svDrvTvdSetTVDAGCTarget(DFE_TGT_SYNC_HEIGHT_COCHANNEL);
            vIO32WriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_CCI, BLANK_BW);//test impluse noise
            LOG(6,"[TVD_DBG_MSG] _svDrvTvdCochAdj CoChannel detected!\n");
        }
    }
    else
    {
        vDrvTvdDecCOCHDeCnt();

        if(fgDrvTvdIsCOCHDeCnt(TVD_COCHDe_CNT2+1))
        {
#ifdef TVD_RTM1_CM
            vIO32WriteFldAlign(DFE_0D, DFE_CLAMP_ATD_GAIN, CLAMP_ATD_GAIN);
            vIO32WriteFldAlign(DFE_0A, 0xF, CLAMP_FINE_RATIO);
#endif
            vIO32WriteFldAlign(DFE_02,SV_ON, BLV_LIM_AUTO);
            _svDrvTvdSetTVDAGCTarget(wAGCTargetSyncHeight);
            vIO32WriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_NML, BLANK_BW);//test impluse noise
            LOG(6,"[TVD_DBG_MSG] _svDrvTvdCochAdj CoChannel non-detected!\n");
        }

        if(IO32ReadFldAlign(STA_CDET_01, DCNT) < HSYNCPOS_DCNT_TH1 || IO32ReadFldAlign(STA_CDET_01, DCNT) > HSYNCPOS_DCNT_TH2)
        {
            vDrvTvdIncHsyncPosDetCnt();
        }
        else
        {
            vDrvTvdDecHsyncPosDetCnt();
        }

        if(_na_state==NA_LOWNOISE && (!fgHwTvdCNRLow()) && (!fgHwTvdVCRBV()))
        {
            if(fgDrvTvdIsHsyncPosDetCnt(TVD_HSYNCPOS_CNT-1))
            {
                vIO32WriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_CCI, BLANK_BW);    //NSTD: hsync position
            }
            else if(fgDrvTvdIsHsyncPosDetCnt(TVD_HSYNCPOS_CNT2+1))
            {
                vIO32WriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_NML, BLANK_BW);    //test impluse noise
            }
        }
    }
}
/* @brief TVD ISR get NR average under non auto search
 * @param fgTrick IN: Trick Status.
 * @param bHwVCRSetting IN: HW VCR Setting.
 * @param bNaState IN: Na State.
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svDrvTvdGetNRAvg(UINT16 *PwTVDNRavg,UINT16 *PwTVDNRavg1,RTvdStatus *pTvd3dStatus)
{
#if TVD_MANUAL_VPRES_FOR_TUNING
    if(IS_AUTOSEARCH() && (FALSE == _sbManualVPresForTuning))
#else
    if(IS_AUTOSEARCH())
#endif

    {
        //Use old vpres method when searching channel.
        
		UINT8 bVpresGain = 0x21;
		vRegWriteFldMulti(DFE_0E,P_Fld(0, VPRES_SEL)|P_Fld(0x01, VPRES_MASK)); //Check VPRES 1
#if TVD_MANUAL_TUNER_SCAN
		if (_sfgManualScanadj)
		{
			bVpresGain = _su1VpresGain;
		}
		else
#endif
		{
#ifdef NEW_COUNTRY_TYPE
			if (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_ATV_EU)
#else
			if (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU)
#endif
			{
				if(_fgDescramblerOn) bVpresGain= 0x12;
				else bVpresGain = 0x12;
			}
			else
			{
				if(_fgDescramblerOn) bVpresGain= 0x3C;
				else bVpresGain = 0x3C;
			}
#if TVD_MANUAL_TUNER_SCAN
			_su1VpresGain = bVpresGain;
#endif
		}
		
		vDrvTvd3dVPresOnOff(bVpresGain, DFE_AUTOSRCH_ONOFF_THRD);
        
    }
    //-----------------Caulcate wTVDNRavg & _swTVDNRavg1 in non auto search mode------//
    else if(!fgHwTvdVLock() || !fgHwTvdVPres())
    {
        pTvd3dStatus->NAInfo.na_HysterCnt=0;
    }
    else if((pTvd3dStatus->NAInfo).na_HysterCnt>(pTvd3dStatus->NAInfo).na_HysterCntThres)  //Caculate the statistics only when vpres and vlock on.
    {
        (pTvd3dStatus->NAInfo).na_cnt++;
        (pTvd3dStatus->NAInfo).na_sum+= bHwTvdNRLevel();

        if((pTvd3dStatus->NAInfo).na_cnt == (UINT8)NA_SIZE)
        {
            *PwTVDNRavg =(pTvd3dStatus->NAInfo).na_sum/ NA_SIZE;
            (pTvd3dStatus->NAInfo).na_cnt = 0;
            (pTvd3dStatus->NAInfo).na_sum = 0;
            /* move to _svDrvTvdBHNAStateChg() */
            *PwTVDNRavg1 = ((*PwTVDNRavg1* 3) +*PwTVDNRavg) >> 2;  //SW IIR

            if(*PwTVDNRavg1 > 0x80)
            {
                *PwTVDNRavg1 = 0x80;
            }
        }
    }
}
/**
 * @brief TVD ISR Trick Mode detection
 * @param fgTrick IN: Trick Status.
 * @param bHwVCRSetting IN: HW VCR Setting.
 * @param bNaState IN: Na State.
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void  _svDrvTvdTrickModeDET(BOOL fgTrick, UINT8 bHwVCRSetting, UINT8 bNaState, RTvdStatus *pTvd3dStatus)
{
    #if TVD_LOW_SYNC_TRICKMODE_WA
    //For trick mode incorrectly detected in sync level reduce
    if(_na_state==NA_LOWNOISE && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
    && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
    && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
    && !fgHwTvdFHPos() && !fgHwTvdFHNeg() && !fgHwTvdCoChannel() && !fgHwTvdCNRLow()
    && _u2SyncLvAvg>0
    )
    {
        if(_u1SmallSyncDet4Trick<SMALL_SYNC_DET_MAX_CNT)
            _u1SmallSyncDet4Trick++;
    }
    else
    {
        /*
        if(_na_state!=NA_LOWNOISE || !_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
        || !((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
        || !((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
        || fgHwTvdFHPos() || fgHwTvdFHNeg() || fgHwTvdCoChannel() || fgHwTvdCNRLow()
        )
        {
            LOG(1, "_na_state=%d\n", _na_state==NA_LOWNOISE);
            LOG(1, "wHTotal=%d\n", (TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3));
            LOG(1, "wVTotal=%d\n", (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10));
            LOG(1, "FHPos=%d\n", !fgHwTvdFHPos());
            LOG(1, "FHNeg=%d\n", !fgHwTvdFHNeg());
            LOG(1, "CoCh=%d\n", !fgHwTvdCoChannel());
            LOG(1, "VCRBV=%d\n", !fgHwTvdVCRBV());
            LOG(1, "CNR=%d\n", !fgHwTvdCNRLow());
            
        }
        */
        _u1SmallSyncDet4Trick=0;
    }
    #endif
    
#if TVD_MANUAL_VPRES_FOR_TUNING
    if((!(pTvd3dStatus->eSourceType==SV_ST_TV)) || (FALSE == _sbManualVPresForTuning))
#endif
    {
        if((fgTrick && (!(pTvd3dStatus->eSourceType==SV_ST_TV))) ||
           (IS_AUTOSEARCH()&&(pTvd3dStatus->eSourceType==SV_ST_TV)))
        {
            //To fix trick mode brightness increasing.
            vIO32WriteFldMulti(DFE_0E,P_Fld(0, VPRES_SEL)|P_Fld(0x01, VPRES_MASK)); //Check VPRES 1
        }
        else
        {
            //To fix PICO Vpres toggle.//VPRES1 && VPRES2 for Special tuner
            vIO32WriteFldMulti(DFE_0E,P_Fld(1, VPRES_SEL)|P_Fld(0x04, VPRES_MASK)); //Check VPRES 1 & VPRES 2
        }
    }

    if(
#if !TVD_NO_BYPASS_TRICK
        (_sbBypassTrick == 0x00) &&
#endif
        (_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))) // CYJ 2006.01.07
    {
        if(pTvd3dStatus->fgTrick!=fgTrick)
        {
            BOOL bTrickOnOff = pTvd3dStatus->fgTrick=fgTrick;

            #if TVD_LOW_SYNC_TRICKMODE_WA
            if(_u1SmallSyncDet4Trick==SMALL_SYNC_DET_MAX_CNT && 
            (_u2SyncLvAvg>0x80 || (_u2SyncLvAvg>0x70 && IO32ReadFldAlign(DFE_STA_01, SYNC_LV)>0x80) || IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)>0x30))
            {
                bTrickOnOff = SV_OFF;
            }
            LOG(1, "bTrickOnOff = %d\n", bTrickOnOff);
            /*
            if(bTrickOnOff && pTvd3dStatus->fgTrick)
            {
                LOG(1, "_na_state=%d\n", _na_state==NA_LOWNOISE);
                LOG(1, "wHTotal=%d\n", (TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3));
                LOG(1, "wVTotal=%d\n", (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10));
                LOG(1, "FHPos=%d\n", !fgHwTvdFHPos());
                LOG(1, "FHNeg=%d\n", !fgHwTvdFHNeg());
                LOG(1, "CoCh=%d\n", !fgHwTvdCoChannel());
                LOG(1, "VCRBV=%d\n", !fgHwTvdVCRBV());
                LOG(1, "CNR=%d\n", !fgHwTvdCNRLow());
                LOG(1, "_u2SyncLvAvg=%d\n", _u2SyncLvAvg>0);
                LOG(1, "_u2SyncLvAvg=%d\n", _u2SyncLvAvg);
                LOG(1, "AGain=%d\n", IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE));
                LOG(1, "_u1SmallSyncDet4Trick=%d\n", _u1SmallSyncDet4Trick);
            }
            */
            #endif
            if(fgIsMainTvd3d())
            {
                vDrvDITrickModeOnOff(SV_VP_MAIN, bTrickOnOff);
            }
            else
            {
                vDrvDITrickModeOnOff(SV_VP_PIP, bTrickOnOff);
            }
        }

        // CYJ 2006.01.07 Trick mode and VCR mode
        if((pTvd3dStatus->fgTrick > 0)&&(bHwVCRSetting==(UINT8)VCR_MODE1))
        {
            //e77f4->e7ff4. Fix trick mode color flickering.  Benson.06.01.25
#if !TVD_8222_NONSTANDARD
            // shorten CAGC large step
            if(_sbIsSetCAGCbyAP != SV_ON)
            {
                vIO32WriteFldAlign(CDET_08, 0x03, CAGC_L_STEP);
            }

#endif

            switch(pTvd3dStatus->bTvdMode)
            {
                case SV_CS_PAL_N:
                case SV_CS_PAL:
                case SV_CS_PAL_M:
                case SV_CS_PAL_60:
                    //To avoid Trick mode no color.
                    vIO32WriteFldAlign(CTG_01, SV_ON, BST_DET);
                    break;

                default:
                    vIO32WriteFldAlign(CTG_01, SV_OFF, BST_DET);
                    break;
            }
        }
        else //non trick mode
        {
            if(bNaState==NA_LOWNOISE)   // Low noise
            {
#ifndef CC_COPLAT_MT82
                UNUSED(pTvd3dStatus->fgIsCkill);
#endif

                //Upper scean magenta flamesh. Benson. 060314
                if(pTvd3dStatus->bTvdMode!=SV_CS_SECAM)
                {
                    if(fgHwTvdIsMacroVision())
                    {
                        vIO32WriteFldAlign(CTG_01, SV_OFF, BST_DET);
                    }
                    else
                    {
                        vIO32WriteFldAlign(CTG_01, SV_ON, BST_DET);
                    }
                }
                else
                {
                    vIO32WriteFldAlign(CTG_01, SV_OFF, BST_DET);
                }
            }
            else
            {
                //Color Sensitivity. Benson.060213.
                if(pTvd3dStatus->bTvdMode==SV_CS_PAL)
                {
                    vIO32WriteFldAlign(CTG_01, SV_ON, BST_DET);
                }
                else
                {
                    vIO32WriteFldAlign(CTG_01, SV_OFF, BST_DET);
                }
            }

#if !TVD_8222_NONSTANDARD

            // CAGC large step
            if(_sbIsSetCAGCbyAP != SV_ON)
            {
                vIO32WriteFldAlign(CDET_08, 0x05, CAGC_L_STEP);
            }

#endif
        }
    }
}

/* @brief Adjust CAGC loop gain to shorten the transient time
* @param IN:bCAGC
* @retval : NOON
*/
#if   TVD_AntiTrs
static void _svDrvTvdAntiTrs(UINT8 bcagc,UINT8 *fgcagc,UINT8 *_sbCAGC1)
{
    if((*_sbCAGC1>bcagc+15)&&(*fgcagc==0))
    {
        *fgcagc=1;
        _bTvdISRCnt=0;
        vIO32WriteFldAlign(CDET_0B, 0x05, LARGE_LOOP_GAIN);
        //LOG(9, "\n bcagc1=%d,bcagc=%d \n",*_sbCAGC1,bcagc);
        LOG(6,"[TVD_DBG_MSG] _svDrvTvdAntiTrs bcagc1=%d, bcagc=%d \n",_sbCAGC1,bcagc);
    }

    if((_bTvdISRCnt>=0x20)&&(*fgcagc==1))
    {
        * fgcagc=0;
        // _bTvdISRCnt=0;
        //LOG(9, "\n the changed bcagc:bCAGC=%d \n",bHwTvdCAGC());
        LOG(6,"[TVD_DBG_MSG] _svDrvTvdAntiTrs the changed bcagc:bCAGC=%d \n",bHwTvdCAGC());
        vIO32WriteFldAlign(CDET_0B, 0x02,  LARGE_LOOP_GAIN);
    }

    * _sbCAGC1=bcagc;
}
#endif

#if CAGC_PEAK_CNR_ADJ
/**
 * @brief Adjust CAGC Peak paramater based on CNR.
 * @param IN:fgCNRLow : HW CNR value
 * @param IN:fgTrick  : HwTrick Mode Flag.
 * @retval : NOON
 */
static void _svDrvTvdAdjustCAGCPeak(BOOL fgCNRLow , BOOL fgTrick, UINT8 bMode)
{
    if(fgCNRLow && fgTrick)
    {
        vIO32WriteFldAlign(CDET_08, CDET_CAGC_PEAK_TC_CNR, CAGC_PEAK_TC);
        vIO32WriteFldAlign(CDET_09, CAGC_PEAK_U_CNR, CAGC_PEAK_U);
        vIO32WriteFldAlign(CDET_09, CAGC_PEAK_V_CNR, CAGC_PEAK_V);
    }
    else
    {
        vIO32WriteFldAlign(CDET_08, CDET_CAGC_PEAK_TC, CAGC_PEAK_TC);
        _svDrvTvdSetCAgcUVPeak(bMode);
    }
}
#endif

/**
 * @brief Get TVD VCR BV status after On/Off Gain.
 * @param IN:fgCurVCRBV : Current HW status
 * @param INOUT:pVCRBVSta : pointer VCR BV status for FW referenced.
 * @retval : NOON
 */
#if TVD_VCR_BV_STA
static void _sbDrvTvdSetVCRBVSta(UINT8 fgCurVCRBV, UINT8 *pVCRBVSta)
{
    if(fgCurVCRBV)
    {
        vDrvTvdIncVCRBVStaCnt();
    }
    else
    {
        vDrvTvdDecVCRBVStaCnt();
    }

    if(fgDrvTvdIsVCRStaCnt(TVD_IS_VCRBVSTAON))
    {
        *pVCRBVSta = 1;
    }
    else if(fgDrvTvdIsVCRStaCnt(TVD_IS_VCRBVSTAOFF))
    {
        *pVCRBVSta = 0;
    }
}
#endif

/**
 * @brief Reset TVD Non Standard status.
 * @param IN:fgIs525 : TRUE: 525 / FALSE: 625
 * @param OUT:pRTvdNSTDStatus : pointer to RTvdNSTDStatus structure variables
 * @retval : NOON.
 */
#if TVD_FRS_FOR_NONSTDSIGNAL
static void _svDrvTvdRstNSTStatus(BOOL fgIs525, RTvdNSTDStatus *pRTvdNSTDStatus)
{
    LOG(6,"[TVD_DBG_MSG] _svDrvTvdRstNSTStatus \n");

    if(fgIs525)
    {
        pRTvdNSTDStatus->wVTotal = 525;
        pRTvdNSTDStatus->wHTotal = 1716;
        pRTvdNSTDStatus->bRefreshRate = 60;
    }
    else
    {
        pRTvdNSTDStatus->wVTotal = 625;
        pRTvdNSTDStatus->wHTotal = 1728;
        pRTvdNSTDStatus->bRefreshRate = 50;
    }

    pRTvdNSTDStatus->fgIsNSTD = 0;
}
#endif

/**
 * @brief Vpres Status state machine.
 * @param :pVpresState IN/OUT: Pointer to Vpres State
 * @retval : VPRES status.
 */
#if TVD_VPRES_STATE
static UINT8 _sbDrvTvdVpresStateMachine(UINT8 *pVpresState)
{
    UINT8 bCurVpresState = *pVpresState;
    UINT8 fgRetVpres;

    switch(bCurVpresState)
    {
        case VPRES_STATE_0:  // VPRES4 OFF,VPRES3 OFF
            fgRetVpres = 0;
            vIO32WriteFldAlign(DFE_0E, SV_OFF, VPRES4TVD_MODE);

            if(IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D))
            {
                vDrvTvdIncVpresStateCnt1();

                if(fgDrvTvdIsVpresStateCnt1(VPRES_TH))
                {
                    *pVpresState = VPRES_STATE_1; //Next State
                    vDrvTvdRstVpresStateCnt1();
                    fgRetVpres=1;
                    LOG(6,"[TVD_DBG_MSG] _sbDrvTvdVpresStateMachine change to VPRES_STATE_1\n");
                }
            }
            else
            {
                vDrvTvdRstVpresStateCnt1();
            }

            break;

        case VPRES_STATE_1:  // Vpres4 Off,Vpres3 On
            fgRetVpres = 1;

            if(IO32ReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))
            {
                *pVpresState = VPRES_STATE_3; //Next State
                vDrvTvdRstVpresStateCnt1();
                LOG(6,"[TVD_DBG_MSG] _sbDrvTvdVpresStateMachine change to VPRES_STATE_3\n");
            }
            else
            {
                if(!IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D))
                {
                    vDrvTvdIncVpresStateCnt1();

                    if(fgDrvTvdIsVpresStateCnt1(VPRES_TH))
                    {
                        *pVpresState=VPRES_STATE_0; //Next State
                        vDrvTvdRstVpresStateCnt1();
                        fgRetVpres=0;
                        LOG(6,"[TVD_DBG_MSG] _sbDrvTvdVpresStateMachine change to VPRES_STATE_0\n");
                    }
                }
                else
                {
                    vDrvTvdRstVpresStateCnt1();
                }
            }

            break;

        case VPRES_STATE_2:  // Vpres4 on,Vpres3 off
            fgRetVpres = 1;

            if(!IO32ReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))
            {
                *pVpresState = VPRES_STATE_0; //Next State
                vDrvTvdRstVpresStateCnt1();
                fgRetVpres=0;
                LOG(6,"[TVD_DBG_MSG] _sbDrvTvdVpresStateMachine change to VPRES_STATE_0\n");
            }
            else
            {
                if(!IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D))
                {
                    vDrvTvdRstVpresStateCnt1();
                }
                else
                {
                    vDrvTvdIncVpresStateCnt1();

                    if(fgDrvTvdIsVpresStateCnt1(VPRES_TH))
                    {
                        *pVpresState = VPRES_STATE_3; //Next State
                        vDrvTvdRstVpresStateCnt1();
                        LOG(6,"[TVD_DBG_MSG] _sbDrvTvdVpresStateMachine change to VPRES_STATE_3\n");
                    }
                }
            }

            break;

        case VPRES_STATE_3:  // Vpres4 on,Vpres3 on
            fgRetVpres = 1;
            vIO32WriteFldAlign(DFE_0E, SV_ON, VPRES4TVD_MODE);

            if(!IO32ReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))
            {
                *pVpresState = VPRES_STATE_1; //Next State
                vDrvTvdRstVpresStateCnt1();
                LOG(6,"[TVD_DBG_MSG] _sbDrvTvdVpresStateMachine change to VPRES_STATE_1\n");
            }
            else
            {
                if(IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D))
                {
                    vDrvTvdRstVpresStateCnt1();
                }
                else
                {
                    vDrvTvdIncVpresStateCnt1();

                    if(fgDrvTvdIsVpresStateCnt1(VPRES_TH))
                    {
                        *pVpresState = VPRES_STATE_2; //Next State
                        vDrvTvdRstVpresStateCnt1();
                        LOG(6,"[TVD_DBG_MSG] _sbDrvTvdVpresStateMachine change to VPRES_STATE_2\n");
                    }
                }
            }

            break;

        case VPRES_STATE_4:  // Only Check Vpres3
            fgRetVpres = IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D);
            vIO32WriteFldAlign(DFE_0E, SV_OFF, VPRES4TVD_MODE);
            break;

        default:
            fgRetVpres = 0;
            break;
    }

    return fgRetVpres;
}
#endif

/**
 * @brief Reset VPRES State machine.
 * @param :Noon
 * @retval : Nood
 */
static void _svDrvTvdRstVpresState(void)
{
#if TVD_VPRES_STATE

    //    #ifdef CC_MT5363_TVD
    if(_rTvd3dStatus.eSourceType!=SV_ST_SV)
        //    #else
        //    if((_rTvd3dStatus.eSourceType!=SV_ST_SV)&&(IS_5363_NonECO()||IS_5363_ECO()||IS_5387_ECO()))
        //    #endif
    {
        _sbVpresState=VPRES_STATE_0;
        vDrvTvdRstVpresStateCnt1();
    }
    else
    {
        _sbVpresState=VPRES_STATE_4;
    }

    fgPreVPres_0=0;
#endif
}


/**
 * @brief Set Default TVD Menu Av Delay.
 * @param :bTvdMode: TVD Mode
 * @retval Default AvDelay Value
 */
static UINT16 _swDrvTvdGetDftMenuAvDelay(UINT8 bTvdMode)
{
    UINT16 wTvd_DftAvdelay = 0;

    if(bTvdMode >= sizeof(_sTvd_MenuAvDelay_Tbl))
    {
        goto EXIT;
    }

#if TVD_HSHAKING_WA
	#if TVD_SUPPORT_DRVCUST_DEFINE
		switch(bTvdMode)
		{
			case SV_CS_PAL_N:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_PAL_N);	
				break;
			case SV_CS_PAL:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_PAL);	
				break;
			case SV_CS_PAL_M:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_PAL_M);	
				break;
			case SV_CS_NTSC358:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_NTSC358);	
				break;
			case SV_CS_SECAM:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SECAM);	
				break;
			case SV_CS_PAL_60:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_PAL_60);	
				break;
			case SV_CS_UNSTABLE:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SV_CS_UNSTABLE);				
				break;
			case SV_CS_NTSC443:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_NTSC443);				
				break;
		}
	#else
    _spCurrAVDelayTbl = (UINT8 *)_sTvd_MenuAvDelay_Tbl;
	#endif
#else

    if((_na_state != (UINT8)NA_SUPERHIGHNOISE) || (_sbTVDFreeRun == 1)) //super high noise
    {
	#if TVD_SUPPORT_DRVCUST_DEFINE
		switch(bTvdMode)
		{
			case SV_CS_PAL_N:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_PAL_N);	
				break;
			case SV_CS_PAL:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_PAL);	
				break;
			case SV_CS_PAL_M:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_PAL_M);	
				break;
			case SV_CS_NTSC358:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_NTSC358);	
				break;
			case SV_CS_SECAM:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SECAM);	
				break;
			case SV_CS_PAL_60:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_PAL_60);	
				break;
			case SV_CS_UNSTABLE:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SV_CS_UNSTABLE);				
				break;
			case SV_CS_NTSC443:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_NTSC443);				
				break;
		}
	#else
		_spCurrAVDelayTbl = (UINT8 *)_sTvd_MenuAvDelay_Tbl;
	#endif
    }
    else
    {
    #if TVD_SUPPORT_DRVCUST_DEFINE
		switch(bTvdMode)
		{
			case SV_CS_PAL_N:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SHNA_PAL_N);	
				break;
			case SV_CS_PAL:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SHNA_PAL);	
				break;
			case SV_CS_PAL_M:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SHNA_PAL_M);	
				break;
			case SV_CS_NTSC358:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SHNA_NTSC358);	
				break;
			case SV_CS_SECAM:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SHNA_SECAM);	
				break;
			case SV_CS_PAL_60:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SHNA_PAL_60);	
				break;
			case SV_CS_UNSTABLE:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SHNA_SV_CS_UNSTABLE);				
				break;
			case SV_CS_NTSC443:
				wTvd_DftAvdelay = DRVCUST_OptGet(eTVDMenu_Av_Delay_SHNA_NTSC443);				
				break;
		}
	#else
        _spCurrAVDelayTbl = (UINT8 *)_sTvd_SHNA_MenuAvDelay_Tbl;
	#endif
    }

#endif
	#if !TVD_SUPPORT_DRVCUST_DEFINE
    	wTvd_DftAvdelay = _spCurrAVDelayTbl[bTvdMode];
	#endif
EXIT:
    return wTvd_DftAvdelay;
}


/**
 * @brief Set TVD Menu Av Delay.
 * @param :bTvdMode: TVD Mode
 * @retval AvDelay Value
 */
 #if 0
static UINT16 _swDrvTvdGetMenuAvDelay(UINT8 bTvdMode)
{
    UINT16 wRetAvDelay = 0;

    switch(bTvdMode)
    {
        case SV_CS_PAL_60:
        case SV_CS_PAL_M:
        case SV_CS_NTSC358:
        case SV_CS_NTSC443:
            wRetAvDelay = IO32ReadFldAlign(TG_07, AV_START_N);
            break;

        case SV_CS_SECAM:
        case SV_CS_PAL:
        case SV_CS_PAL_N:
            wRetAvDelay = IO32ReadFldAlign(TG_07, AV_START_P);
            break;

        default:
            assert(0);
            break;
    }

    return wRetAvDelay;
}
#endif

/**
 * @brief Set TVD Menu Av Delay.
 * @param :bTvdMode: TVD Mode.
 * @param :wAvDelay: Av Delay Value.
 * @retval VOID
 */
static void _svDrvTvdSetMenuAvDelay(UINT8 bTvdMode, UINT16 wAvDelay)
{
    if(IO32ReadFldAlign(TG_09, IIR_LLOCK_EN))
        wAvDelay = wAvDelay + 3;
    switch(bTvdMode)
    {
        case SV_CS_PAL_60:
        case SV_CS_PAL_M:
        case SV_CS_NTSC358:
        case SV_CS_NTSC443:
            vIO32WriteFldAlign(TG_07, wAvDelay, AV_START_N);
            break;

        case SV_CS_SECAM:
        case SV_CS_PAL:
        case SV_CS_PAL_N:
            vIO32WriteFldAlign(TG_07, wAvDelay, AV_START_P);
            break;

        default:
            assert(0);
            break;
    }
}
/**
 * @brief  Set Tvd3D AV Delay by Mode
 * @param  bTvdMode: TVD Modes
 * @retval void
 */
static void _svDrvTvdSetAVDelay(UINT8 bTvdMode)
{
    UINT16 wTVD_AVDelay;
    wTVD_AVDelay = _swDrvTvdGetDftMenuAvDelay(bTvdMode);
    _svDrvTvdSetMenuAvDelay(bTvdMode, _su1UIDelay + wTVD_AVDelay);
}
/**
 * @brief TVD ISR H Lock FineTune (KI/KP Gain, Menu/Auto Linelock, HSS Fine Tune).
 * @param bNaState IN: Na State.
 * @param pTvd3dStatus IN :pointer to RTvdStatus status
 * @retval : NOON
 */

static void _svDrvTvdSetVDelay(UINT8 bMode)
{
    UINT8 u1VDealy=0;
    INT8 i1Offset = 0;
    i1Offset = _su1UIVPorchDelay + _spCurrVPorchTbl[bMode];
	#if 1
	if(IO32ReadFldAlign(SCART_STA_00, FLD_FB_ON_CNT)==0xFFF)////For SCART-RGB 576i V porch 
    {
        i1Offset = i1Offset-2;
    }
	#endif

    #if TVD_PALM50_SUPPORT
    if(bMode == SV_CS_PAL_M && _su1PALM50DetState == PALM50_DET_DONE_PALM50)
    {
        i1Offset-= 4;
    }
    #endif
    
#if TVD_NEW_VPORCH
    if(i1Offset<0)
    {
        i1Offset = 0-i1Offset;
        if(i1Offset>0x3F)
            i1Offset = 0x3F;
        u1VDealy = 0x40 + i1Offset;
    }
    else
    {
        if(i1Offset>0x3F)
            i1Offset = 0x3F;
        u1VDealy = 0x0 + i1Offset;
    }
    
    #if 0
    //For SCART-RGB porch
    if(IO32ReadFldAlign(SCART_STA_00, FLD_FB_ON_CNT)==0xFFF)
    {
        u1VDealy = 0x43;
    }
    #endif
    LOG(1, "_svDrvTvdSetVDelay u1VDealy = 0x%02X\n", (u1VDealy));
    vIO32WriteFldAlign(TG_0A, u1VDealy, TG_SAF_ADJ);
#else
    if(i1Offset<CCVSYNC_MIN)
    {
        i1Offset = CCVSYNC_MIN;
    }
    else if(i1Offset>CCVSYNC_MAX)
    {
        i1Offset = CCVSYNC_MAX;
    }
    switch (_rTvd3dStatus.bTvdMode)
    {
    	case SV_CS_PAL_N:
	    case SV_CS_PAL:
	    case SV_CS_SECAM:
	        u1VDealy = CCVSYNC_P_DEFAULT + (i1Offset);
	        LOG(1, "_svDrvTvdSetVDelay u1VDealy = 0x%02X\n", (u1VDealy));
	        vIO32WriteFldAlign(TG_11,u1VDealy,CCVSYNC0_P);
            vIO32WriteFldAlign(TG_11,u1VDealy+1,CCVSYNC1_P);
	        break;
	    case SV_CS_PAL_M:
		case SV_CS_PAL_60:
		case SV_CS_NTSC358:
		case SV_CS_NTSC443:
		default:
		    u1VDealy = CCVSYNC_N_DEFAULT + (i1Offset);
		    LOG(1, "_svDrvTvdSetVDelay u1VDealy = 0x%02X\n", (u1VDealy));
		    vIO32WriteFldAlign(TG_11,u1VDealy,CCVSYNC0_N);
            vIO32WriteFldAlign(TG_11,u1VDealy,CCVSYNC1_N);
		    break;
    }
#endif
}



static void  _svDrvTvdHFineTune(UINT8 bNaState,  RTvdStatus *pTvd3dStatus)
{
#if TVD_HSHAKING_WA
    static UINT8 _sbHShakingCnt = 0;
#endif

    UNUSED(_su1LineLockCondition);
    
    if((bNaState==NA_SUPERHIGHNOISE)&& (pTvd3dStatus->fgVPres) && (!IS_AUTOSEARCH()))
    {
#if !TVD_HSHAKING_WA

        if(_svDrvTvdGetNRLevel() > 35)
        {
            vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
            vIO32WriteFldAlign(TG_09, SV_ON, IIR_LLOCK_EN);
        }
        else if(_svDrvTvdGetNRLevel() > 32)
        {
            vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
        }
        else
        {
#ifdef CC_COPLAT_MT82

            if(eTVDLLockFreeRun)
#else
            if(DRVCUST_OptGet(eTVDLLockFreeRun))
#endif
            {
                vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
            }
            else
            {
                vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
            }
        }

#else
#if TVD_HN_CHCHG_WA

        if(fgDrvTvdModeCntGt(_sbTvd_McDone_cnt-2) && _sbTvdModeCnt <_sbHNChChgMCntTh
           && pTvd3dStatus->eSourceType==SV_ST_TV)
        {
            LOG(3,"[TVD_DBG_MSG] _svDrvTvdHFineTune SuperHighNoise ChChg WA, mcnt=%d, NR_LV=%d, AVG=%d\n", _sbTvdModeCnt, bHwTvdNRLevel(), _swTVDNRavg1);
            vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
            vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
            vIO32WriteFldAlign(TG_04, TG_LF_KP0, LF_KP0);
            vIO32WriteFldAlign(TG_04, TG_LF_KI1, LF_KI1);
            vIO32WriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
            vIO32WriteFldAlign(TG_09, SV_OFF, IIR_PSEUDO_HSYNC);
        }
        else
        {
#endif
            #if TVD_DETECT_UNSTABLE_VCRBV
            if(_sbUnsbableVCRBV==FALSE)
            #endif
            {
                LOG(6,"[TVD_DBG_MSG] _svDrvTvdHFineTune SuperHighNoise1, mcnt=%d, NR_LV=%d, syncavg(0x%x)\n", 
                    _sbTvdModeCnt, _swTVDNRavg1, _u2SyncLvAvg);
                if(_svDrvTvdGetNRLevel()<IIR_LLOCK_EN_TH && _u2SyncLvAvg < 0x60)
                {
                    _su1LineLockCondition = 1;
                    LOG(6,">> go into line lock condition 1\n");
                    vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
                    vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
                    vIO32WriteFldAlign(TG_04, TG_LF_KP0, LF_KP0);
                    vIO32WriteFldAlign(TG_04, TG_LF_KI1, LF_KI1);
                    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
                    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_PSEUDO_HSYNC);
                }
                else
                {
                    _su1LineLockCondition = 2;
                    LOG(6,">> go into line lock condition 2\n");
                    vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
                    //vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
                    vIO32WriteFldAlign(TG_17, SV_ON, MHSS_EN);
                    vIO32WriteFldAlign(TG_17, SV_ON, MHSS);
                    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
                    vIO32WriteFldAlign(TG_04, SV_ON, LF_AUTO);

                    if(_svDrvTvdGetNRLevel() > 35)
                    {
                        _su1LineLockCondition = 3;
                        vIO32Write4B(TG_13, TG_KIKP_GAIN);
                        vIO32WriteFldAlign(TG_04, TG_FAST_KP, FAST_KP);
                        _sbHShakingCnt = 0;
                    }
                    else
                    {
                        _sbHShakingCnt ++;

                        if(_sbHShakingCnt == 0x4)
                        {
                            _su1LineLockCondition = 4;
                            vIO32WriteFldAlign(TG_04, TG_FAST_KP_TMP, FAST_KP);
                            vIO32Write4B(TG_13, TG_KIKP_GAIN_TMP);
                        }
                        else if(_sbHShakingCnt == 0x8)
                        {
                            _su1LineLockCondition = 5;
                            vIO32WriteFldAlign(TG_04, TG_FAST_KP_SHNA, FAST_KP);
                            vIO32Write4B(TG_13, TG_KIKP_GAIN_SHNA);
                        }
                    }
                }
            }
            #if TVD_DETECT_UNSTABLE_VCRBV
            else
            {
                if(_svDrvTvdGetNRLevel()>IIR_LLOCK_EN_TH+1)
                {
                    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
                }
            }
            #endif
#if TVD_HN_CHCHG_WA
        }

#endif
#endif
    }
    else
    {
#if TVD_HN_CHCHG_WA

        if((bNaState==NA_HIGHNOISE || bHwTvdNRLevel()>_sbHNChChgNRTh)
           && fgDrvTvdModeCntGt(_sbTvd_McDone_cnt-2) && _sbTvdModeCnt <_sbHNChChgMCntTh
           && pTvd3dStatus->eSourceType==SV_ST_TV)
        {
            LOG(3, "[TVD_DBG_MSG] _svDrvTvdHFineTune HighNoise ChChg WA, mcnt=%d, NR_LV=%d,  AVG=%d\n", _sbTvdModeCnt, bHwTvdNRLevel(), _swTVDNRavg1);
            vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
            vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
            vIO32WriteFldAlign(TG_04, TG_LF_KP0, LF_KP0);
            vIO32WriteFldAlign(TG_04, TG_LF_KI1, LF_KI1);
        }
        else
        {
#endif

            if(_sbTvdModeCnt<_sbHNChChgMCntTh+10)
            {
                LOG(3,"[TVD_DBG_MSG] _svDrvTvdHFineTune HighNoise, mcnt=%d, NR_LV=%d, AVG=%d\n", _sbTvdModeCnt, bHwTvdNRLevel(), _swTVDNRavg1);
            }

#if TVD_HSHAKING_WA
            vIO32Write4B(TG_13, TG_KIKP_GAIN);
            vIO32WriteFldAlign(TG_04, TG_FAST_KP, FAST_KP);
#endif
#if TVD_VCR_BV
            //if(fgDrvTvdVCRBVCntGt(1))
            #if TVD_DETECT_UNSTABLE_VCRBV
            if(_sbUnsbableVCRBV==FALSE)
            #endif
            {
                if(bNaState==NA_HIGHNOISE && _svDrvTvdGetNRLevel()<IIR_LLOCK_EN_TH)
                {
                    _su1LineLockCondition = 6;
                    vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
                    vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
                    vIO32WriteFldAlign(TG_04, TG_LF_KP0, LF_KP0);
                    vIO32WriteFldAlign(TG_04, TG_LF_KI1, LF_KI1);
                    vIO32WriteFldAlign(TG_09, SV_ON, IIR_LLOCK_EN);
                    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_PSEUDO_HSYNC);
                }
                else
                {
                    if(pTvd3dStatus->bTvdMode==SV_CS_PAL_M && bNaState==NA_LOWNOISE
                    && _svDrvTvdGetNRLevel()<=45 && _swLLStatus_avg>_u1FixLLockGainTh)
                    {
                        _su1LineLockCondition = 7;
                        vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
                        vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
                        vIO32WriteFldAlign(TG_04, TG_LF_KP0, LF_KP0);
                        vIO32WriteFldAlign(TG_04, TG_LF_KI1, LF_KI1);
                    }
                    else if (bNaState==NA_LOWNOISE && _swTVDNRavg1<=20)
                    {
                        if(pTvd3dStatus->eSourceType==SV_ST_TV && pTvd3dStatus->bTvdMode==SV_CS_NTSC358
                        && _svDrvTvdGetNRLevel()<=48 &&_swLLStatus_avg>0x70 && IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)<=5)
                        {
                            _su1LineLockCondition = 8;
                            vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
                            vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
                        }
                        else
                        {
                            _su1LineLockCondition = 9;
                            //LOG(1,"[TVD_DBG_MSG] _svDrvTvdHFineTune LowNoise2_ON, mcnt=%d, NR_LV=%d\n", _sbTvdModeCnt, _swTVDNRavg1);
                            vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
                            vIO32WriteFldAlign(TG_04, SV_ON, LF_AUTO);
                            #if TVD_HSHAKING_WA
                            vIO32WriteFldAlign(TG_17, SV_OFF, MHSS_EN);
                            vIO32WriteFldAlign(TG_17, SV_OFF, MHSS);
                            _sbHShakingCnt = 0;
                            #endif
                        }
                    }
                    else
                    {
                        vDrvTvdIncVCRBVCnt();
                        if(fgDrvTvdVCRBVCntGt(TVD_VCR_BV_CNT))
                        {
                            _su1LineLockCondition = 10;
                            vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
                            vIO32WriteFldAlign(TG_04, SV_ON, LF_AUTO);
                            #if TVD_HSHAKING_WA
                            vIO32WriteFldAlign(TG_17, SV_OFF, MHSS_EN);
                            vIO32WriteFldAlign(TG_17, SV_OFF, MHSS);
                            _sbHShakingCnt = 0;
                            #endif
                        }
                    }
                }
            }
            #if TVD_DETECT_UNSTABLE_VCRBV
            else
            {
                if(_svDrvTvdGetNRLevel()>IIR_LLOCK_EN_TH+1)
                {
                    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
                }
            }
            #endif
#else
            vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
#endif
#if TVD_HN_CHCHG_WA
        }

#endif
    }

#ifdef CC_COPLAT_MT82

    if((eTVDLLockFreeRun) && (pTvd3dStatus->fgVPres) && (!IS_AUTOSEARCH()))
#else
    if(DRVCUST_OptGet(eTVDLLockFreeRun) && (pTvd3dStatus->fgVPres) && (!IS_AUTOSEARCH()))
#endif
    {
        vIO32WriteFldAlign(TG_16, 0, LLOCK_ERRWIN_SEL);//Disable Free Run function

        if(_svDrvTvdGetNRLevel() < 32) //Low Signal
        {
            //vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
            vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
#if !TVD_HSHAKING_WA
            _sbTVDFreeRun = 1;
#endif

            if(fgHwTvdHsyncLock())
            {
                vIO32WriteFldAlign(TG_16, 1, LLOCK_ERRWIN_SEL);//Enable Free Run function
                vIO32WriteFldAlign(TG_00, TG_MAX_HWIDTH_FRUN, MAX_HWIDTH);//Let HW free run
            }
            else
            {
                vIO32WriteFldAlign(TG_00, TG_MAX_HWIDTH_NML, MAX_HWIDTH);
            }
        }
        else
        {
            //vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
            vIO32WriteFldAlign(TG_04, SV_ON, LF_AUTO);
            vIO32WriteFldAlign(TG_00, TG_MAX_HWIDTH_NML, MAX_HWIDTH);
#if !TVD_HSHAKING_WA
            _sbTVDFreeRun = 0;
#endif
        }

        _svDrvTvdSetAVDelay(pTvd3dStatus->bTvdMode);
    }
}
/**
 * @brief Set TVD CAGC Bound for Data Patch.
 * @param :bMidB: Lower Bound of target burst tolerance
 * @param :bMidA: Upper Bound of target burst tolerance
 * @param :bTgtB: Lower Bound of target burst level
 * @param :bTgtA: Upper Bound of target burst level
 */
static void _svDrvTvdSetCAGCBound4Data(UINT8 bMidB, UINT8 bMidA, UINT8 bTgtB, UINT8 bTgtA) reentrant
{

    if(_sbIsSetCAGCbyAP == SV_ON)
    {
        return;
    }

    vIO32WriteFldMulti(SECAM_03,P_Fld(bMidB,CAGC_IP_MID_B)|P_Fld(bMidA,CAGC_IP_MID_A));
    vIO32WriteFldMulti(SECAM_05, P_Fld(bTgtB, CAGC_IP_TARGET_B) | P_Fld(bTgtA, CAGC_IP_TARGET_A));
}


/**
 * @brief Set TVD CAGC Bound for Ctrl Patch.
 * @param :dwBound: MID_B[31:25]MID_A[23:16]TARGET_B[15:8]TARGET[7:0]
 */
static void _svDrvTvdSetCAGCBound4Ctrl(UINT32 dwBound)
{
    vIO32Write4B(CDET_07, dwBound);// CAGCBound
}


/**
 * @brief Set TVD Burst lock threshold
 * @param :wThr1: burst lock --> unlock threshold
 * @param :wThr2: burst unlock --> lock threshold
 */
static void _svDrvTvdSetBstLockTh(UINT16 wThr1, UINT16 wThr2) reentrant
{
    vIO32WriteFldMulti(CTG_01,P_Fld(wThr2,BSTLOCK_THR2)
    |P_Fld(wThr1,BSTLOCK_THR1));
}



/**
 * @brief Get TVD Target Sync Height Value
 * @param :Noon
 */
static UINT16 _swDrvTvdGetTVDAGCTarget(void)
{
    return IO32ReadFldAlign(DFE_03, AGC2_TARGET_SYNC_HEIGHT);
}

/**
 * @brief Set TVD Blank Level for YUV-->YCBCR
 * @param :bTVDMode: TVD Mode
 * @param :u2BlkLevel: YUV Blank Level
 */
static void _svDrvTvdSetTVDBlk(UINT8 bTVDMode, UINT16 u2BlkLevel)
{
    switch(bTVDMode) //(_rTvd3dStatus.bTvdMode)
    {
        case SV_CS_SECAM:
        case SV_CS_PAL:
        case SV_CS_PAL_60:
        case SV_CS_PAL_N:
	#if TVD_SUPPORT_DRVCUST_DEFINE
			_svDrvTvdSetTVDAGCTarget(DRVCUST_OptGet(eTVDTgt_Sync_Height_625)); //5391 Calib by TG35
	#else
            _svDrvTvdSetTVDAGCTarget(DFE_TGT_SYNC_HEIGHT_625); //5391 Calib by TG35
        #endif
            vIO32WriteFldAlign(DFE_04, ((_rTvd3dStatus.eSourceType==SV_ST_TV)?DFE_PEAKY_TV_625 : DFE_PEAKY_OTHERS_625)+(u2BlkLevel-DFE_BLANK_TARGET), AGC2_PY_LIM);
            vIO32WriteFldAlign(CTG_07, u2BlkLevel+DFE_BLANK_OFFSET_PAL, BLKLVL);//support ultra black
            break;

        case SV_CS_PAL_M:
        case SV_CS_NTSC358:
	#if TVD_SUPPORT_DRVCUST_DEFINE
	    _svDrvTvdSetTVDAGCTarget(DRVCUST_OptGet(eTVDTgt_Sync_Height_525)); //5391 Calib by TG35
	#else
            _svDrvTvdSetTVDAGCTarget(DFE_TGT_SYNC_HEIGHT_525); //5391 Calib by TG35
	#endif
            vIO32WriteFldAlign(DFE_04, ((_rTvd3dStatus.eSourceType==SV_ST_TV)?DFE_PEAKY_TV_525 : DFE_PEAKY_OTHERS_525)+(u2BlkLevel-DFE_BLANK_TARGET), AGC2_PY_LIM);

            if(_bAgcPedEn)
            {
                vIO32WriteFldAlign(CTG_07, u2BlkLevel+DFE_BLANK_OFFSET_NTSCJ, BLKLVL);
            }
            else
            {
                vIO32WriteFldAlign(CTG_07, u2BlkLevel+DFE_BLANK_OFFSET_NTSC, BLKLVL);// MT5381TVDPort CTG Port BLKLVL
            }

            break;

        case SV_CS_NTSC443:
	#if TVD_SUPPORT_DRVCUST_DEFINE
	    _svDrvTvdSetTVDAGCTarget(DRVCUST_OptGet(eTVDTgt_Sync_Height_525)); //5391 Calib by TG35
	#else
            _svDrvTvdSetTVDAGCTarget(DFE_TGT_SYNC_HEIGHT_525); //5391 Calib by TG35
	#endif
            vIO32WriteFldAlign(DFE_04, ((_rTvd3dStatus.eSourceType==SV_ST_TV)?DFE_PEAKY_TV_525 : DFE_PEAKY_OTHERS_525)+(u2BlkLevel-DFE_BLANK_TARGET), AGC2_PY_LIM);
            vIO32WriteFldAlign(CTG_07, u2BlkLevel+DFE_BLANK_OFFSET_NTSC, BLKLVL);// MT5381TVDPort CTG Port BLKLVL
            break;

        default:
            assert(0);
            break;
    }
}


#if TVD_LLOCK_STATUS
/**
 * @brief  Set Tvd3D AV Delay by Mode
 * @param  bTvdMode: TVD Modes
 * @retval Avg Line Error
 */
static UINT16 _svDrvTvdChkLineStatus(void)
{
    static UINT16 _wLLStatus_tmp;
    static UINT8  _bLLcnt;
    //	if (fgHwTvdHLock())
    {
        if(_bLLcnt < 15)
        {
            _wLLStatus_tmp+=IO32ReadFldAlign(TG_STA_03, LLOCK_STATUS);
            _bLLcnt++;
        }
        else
        {
            _swLLStatus_avg=_wLLStatus_tmp/16;
            _wLLStatus_tmp=IO32ReadFldAlign(TG_STA_03, LLOCK_STATUS);
            _bLLcnt=0;
        }
    }
    return _swLLStatus_avg;
}
#endif

#if TVD_ABS_DIFF_STDFV
static void _svDrvTvdChkAbsDiffStdFV(void)
{
    static UINT16 _su2AbsDiffStdFV_tmp =0, _su2Var8Field_tmp =0;
    static UINT8  _su1AbsDiffStdFV_Cnt =0;

    UNUSED(_u2AbsDiffStdFV);
    UNUSED(_u2Var8Field);

    if (_rTvd3dStatus.fgVPres) 
    {
        if(_su1AbsDiffStdFV_Cnt < 15)
        {
            _su2AbsDiffStdFV_tmp+=IO32ReadFldAlign(TG_STA_04, ABS_DIFF_STDFV);
            _su2Var8Field_tmp  += IO32ReadFldAlign(TG_STA_04, STDFV_VAR_8FIELD);
            _su1AbsDiffStdFV_Cnt++;
        }
        else
        {
            _u2AbsDiffStdFV=_su2AbsDiffStdFV_tmp/16;
            _u2Var8Field  = _su2Var8Field_tmp/16;
            _su2AbsDiffStdFV_tmp=IO32ReadFldAlign(TG_STA_04, ABS_DIFF_STDFV);
            _su2Var8Field_tmp  = IO32ReadFldAlign(TG_STA_04, STDFV_VAR_8FIELD);
            _su1AbsDiffStdFV_Cnt=0;
        }
    }
    else
    {
        _su2AbsDiffStdFV_tmp = _u2AbsDiffStdFV= IO32ReadFldAlign(TG_STA_04, ABS_DIFF_STDFV);
        _su2Var8Field_tmp =  _u2Var8Field= IO32ReadFldAlign(TG_STA_04, STDFV_VAR_8FIELD);
        _su1AbsDiffStdFV_Cnt =0;
    }

    if (_u1LLockStableCnt < 200)
        _u1LLockStableCnt ++;
}
#endif

/**
 * @brief  Noise Status Reset Function
 * @param  void
 * @retval void
 */
static void _svDrvTvdNAReset(void)
{
    vIO32WriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
#if !TVD_NO_BYPASS_TRICK
    _sbBypassTrick  = 0x00;
#endif

    //vIO32WriteFldAlign(DFE_18, SV_ON, VMAXMIN_EN);

    //Set CAGC bound & Burst unlock threshold according to TvdMode in Low Noise Sate.
    switch(_rTvd3dStatus.bTvdMode)
    {
        case SV_CS_SECAM:

            //Set normal burst unlock threshold under low noise.
        case SV_CS_PAL_N:
#if TVD_PHALT_MN_WA2
	#if TVD_SUPPORT_DRVCUST_DEFINE
	    _svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_PAL_N), DRVCUST_OptGet(eTVD_BstLock_Thr2_PAL_N));
	#else
            _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_PAL_N, CTG_BSTLOCK_THR2_PAL_N);
	#endif
            break;
#endif

        case SV_CS_PAL:
        case SV_CS_PAL_60:
            //Set normal burst unlock threshold under low noise.
        #if TVD_SUPPORT_DRVCUST_DEFINE
	    _svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_LN_625), DRVCUST_OptGet(eTVD_BstLock_Thr2_LN_625));
	#else
            _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_LN_625, CTG_BSTLOCK_THR2_LN_625);
	#endif
            break;

        case SV_CS_PAL_M:
        case SV_CS_NTSC358:
        case SV_CS_NTSC443:
            //Set normal burst unlock threshold under low noise.
        #if TVD_SUPPORT_DRVCUST_DEFINE
	    _svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_LN_525), DRVCUST_OptGet(eTVD_BstLock_Thr2_LN_525));
	#else
            _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_LN_525, CTG_BSTLOCK_THR2_LN_525);
	#endif
            break;

        default:
            assert(0);
            break;
    }

    switch(_rTvd3dStatus.bTvdMode)
    {
        case SV_CS_SECAM:
	#if TVD_SUPPORT_DRVCUST_DEFINE
	    _svDrvTvdSetCAGCBound4Data(SCM_IP_MID_B, SCM_IP_MID_A, DRVCUST_OptGet(eTVDSECAM_Ip_Tgt_B), DRVCUST_OptGet(eTVDSECAM_Ip_Tgt_A));
	#else
            _svDrvTvdSetCAGCBound4Data(SCM_IP_MID_B, SCM_IP_MID_A, SCM_IP_TGT_B, SCM_IP_TGT_A);
	#endif
            _svDrvTvdSetCAGCBound4Ctrl(SECAM_CAGC_BOUND);
            break;

        case SV_CS_PAL:
        case SV_CS_PAL_60:
        case SV_CS_PAL_N:
	#if TVD_SUPPORT_DRVCUST_DEFINE
	    _svDrvTvdSetCAGCBound4Data(PAL_IP_MID_B, PAL_IP_MID_A, DRVCUST_OptGet(eTVDPAL_Ip_Tgt_B), DRVCUST_OptGet(eTVDPAL_Ip_Tgt_A));
	#else
            _svDrvTvdSetCAGCBound4Data(PAL_IP_MID_B, PAL_IP_MID_A, PAL_IP_TGT_B, PAL_IP_TGT_A);
	#endif
            _svDrvTvdSetCAGCBound4Ctrl(PAL_CAGC_BOUND);
            break;

        case SV_CS_PAL_M:
        case SV_CS_NTSC358:
            if(_bAgcPedEn)
            {
        #if TVD_SUPPORT_DRVCUST_DEFINE
	        _svDrvTvdSetCAGCBound4Data(NTSC_IP_MID_B, NTSC_IP_MID_A, DRVCUST_OptGet(eTVDNTSC_Ip_Tgt_B), DRVCUST_OptGet(eTVDNTSC_Ip_Tgt_A));
	#else
                _svDrvTvdSetCAGCBound4Data(NTSC_IP_MID_B, NTSC_IP_MID_A, NTSC_IP_TGT_B, NTSC_IP_TGT_A);
	#endif
                _svDrvTvdSetCAGCBound4Ctrl(NTSC_CAGC_BOUND);
            }
            else
            {
        #if TVD_SUPPORT_DRVCUST_DEFINE
		_svDrvTvdSetCAGCBound4Data(NTSC_J_IP_MID_B, NTSC_J_IP_MID_A, DRVCUST_OptGet(eTVDNTSC_J_Ip_Tgt_B), DRVCUST_OptGet(eTVDNTSC_J_Ip_Tgt_A));
	#else
                _svDrvTvdSetCAGCBound4Data(NTSC_J_IP_MID_B, NTSC_J_IP_MID_A, NTSC_J_IP_TGT_B, NTSC_J_IP_TGT_A);
	#endif
                _svDrvTvdSetCAGCBound4Ctrl(NTSC_J_CAGC_BOUND);
            }

            break;

        case SV_CS_NTSC443:
	#if TVD_SUPPORT_DRVCUST_DEFINE
	    _svDrvTvdSetCAGCBound4Data(NTSC_J_IP_MID_B, NTSC_J_IP_MID_A, DRVCUST_OptGet(eTVDNTSC_J_Ip_Tgt_B), DRVCUST_OptGet(eTVDNTSC_J_Ip_Tgt_A));
	#else
            _svDrvTvdSetCAGCBound4Data(NTSC_J_IP_MID_B, NTSC_J_IP_MID_A, NTSC_J_IP_TGT_B, NTSC_J_IP_TGT_A);
	#endif
            _svDrvTvdSetCAGCBound4Ctrl(NTSC_J_CAGC_BOUND);
            break;

        default:
            assert(0);
            break;
    }
}

#if TVD_AUTO_DFT

/**
 * @brief Set TVD Default Mode
 * @param  bMode: TV mode enmu
 * @retval void
 */
static void _svDrvTvdSetDftMode(UINT8 bMode) reentrant
{
    switch(bMode)
    {
        case SV_CS_PAL_N:
            vIO32WriteFldMulti(CDET_00,  P_Fld(0,INI_IS443)|P_Fld(1,INI_PHALT)|P_Fld(0,INI_V525)|P_Fld(0,L525));
            break;

        case SV_CS_PAL:
            vIO32WriteFldMulti(CDET_00, P_Fld(1,INI_IS443)|P_Fld(1,INI_PHALT)|P_Fld(0,INI_V525)|P_Fld(0,L525));
            break;

        case SV_CS_PAL_M:
            vIO32WriteFldMulti(CDET_00, P_Fld(0,INI_IS443)|P_Fld(1,INI_PHALT)|P_Fld(1,INI_V525)|P_Fld(1,L525));
            break;

        case SV_CS_NTSC358:
            vIO32WriteFldMulti(CDET_00,  P_Fld(0,INI_IS443)|P_Fld(0,INI_PHALT)|P_Fld(1,INI_V525)|P_Fld(1,L525));
            break;

        case SV_CS_SECAM:
            vIO32WriteFldMulti(CDET_00,  P_Fld(1,INI_IS443)|P_Fld(0,INI_PHALT)|P_Fld(0,INI_V525)|P_Fld(0,L525));
            break;

        case SV_CS_PAL_60:
            vIO32WriteFldMulti(CDET_00,  P_Fld(1,INI_IS443)|P_Fld(1,INI_PHALT)|P_Fld(1,INI_V525)|P_Fld(1,L525));
            break;

        case SV_CS_NTSC443:
            vIO32WriteFldMulti(CDET_00,  P_Fld(1,INI_IS443)|P_Fld(0,INI_PHALT)|P_Fld(1,INI_V525)|P_Fld(1,L525));
            break;

        default:
            break;
    }
#if TVD_NOBURST_SLOW_V625_WA
    #if TVD_SET_ENABLED_CS
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && _rTvEnabledCS.u1TvdEnNTSC358 && _rTvEnabledCS.u1TvdEnPAL)
    #else
    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    #endif
    {
        if(bMode==SV_CS_NTSC358)
            vIO32WriteFldAlign(CDET_00, 0, INI_V525);
    }
#endif
}
#endif

/**
 * @brief Set TVD CAGC UV peak value
 * @param  bMode : TVD color system
 * @retval void
 */
static void _svDrvTvdSetCAgcUVPeak(UINT8 bMode)
{
    switch(bMode)
    {
        case SV_CS_PAL_N:
        case SV_CS_PAL:
        case SV_CS_SECAM:
            vIO32WriteFldAlign(CDET_09, CAGC_PEAK_U_PAL, CAGC_PEAK_U);
            vIO32WriteFldAlign(CDET_09, CAGC_PEAK_V_PAL, CAGC_PEAK_V);
            break;

        case SV_CS_PAL_M:
        case SV_CS_NTSC358:
            if(_bAgcPedEn)
            {
                vIO32WriteFldAlign(CDET_09, CAGC_PEAK_U_NTSC, CAGC_PEAK_U);
                vIO32WriteFldAlign(CDET_09, CAGC_PEAK_V_NTSC, CAGC_PEAK_V);
            }
            else
            {
                vIO32WriteFldAlign(CDET_09, CAGC_PEAK_U_NTSCJ, CAGC_PEAK_U);
                vIO32WriteFldAlign(CDET_09, CAGC_PEAK_V_NTSCJ, CAGC_PEAK_V);
            }

            break;

        case SV_CS_PAL_60:
        case SV_CS_NTSC443:
            vIO32WriteFldAlign(CDET_09, CAGC_PEAK_U_NTSCJ, CAGC_PEAK_U);
            vIO32WriteFldAlign(CDET_09, CAGC_PEAK_V_NTSCJ, CAGC_PEAK_V);
            break;

        default:
            break;
    }
}

/**
 * @brief TVD Mode Change Done Handle Function
 *
 * When Mode Change Done, signal status must be properly set.
 * TDC, NR and other mode related setting must be set according
 * to 525/625 of input signal and then turn on.
 *
 * @bug
 */
static void _svDrvTvdModeChgDone(void)
{
    LOG(1,"[TVD_DBG_MSG] _svDrvTvdModeChgDone fgVPres=%d mcnt(%d)\n",_rTvd3dStatus.fgVPres, _sbTvdModeCnt);

#ifdef CC_SUPPORT_RECORD_AV
    if(_sbATVPVRBypassModeChg==TRUE)
    {
        LOG(1,"======================================\n");
        LOG(1,"[TVD_DBG_MSG] _svDrvTvdModeChgDone bypass modechg \n");
        LOG(1,"======================================\n");

        if(_rTvd3dStatus.fgVPres == 0)
        {
            _rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_NOSIGNAL;
        }
        else
        {
            _rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_STABLE;
        }  
        
        if(fgIsMainTvd3d() && _sbIssueModeChgDoneMain)
        {
            LOG(1,"======================================\n");
            LOG(1,"[TVD_DBG_MSG] _svDrvTvdModeChgDone Main \n");
            LOG(1,"======================================\n");
            vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
        }

        if(fgIsPipTvd3d() && _sbIssueModeChgDoneSub)
        {
            LOG(1,"======================================\n");
            LOG(1,"[TVD_DBG_MSG] _svDrvTvdModeChgDone Sub \n");
            LOG(1,"======================================\n");
            vSetPipFlg(PIP_FLG_MODE_DET_DONE);
        }
        _sbIssueModeChgDoneMain = _sbIssueModeChgDoneSub = FALSE;
        return;
    }
#endif

#if PDP_PANEL_NSTD_CHECK_WA
    //For PDP sync 0->100%, because PDP can not apply NSTD check in mainloop, need to reset srcRegion again
    if(IS_PANEL_PDP && _rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        LOG(1,"[TVD_DBG_MSG] _svDrvTvdModeChgDone IS_PANEL_PDP fgIsNSTD=%d, vTotal=%d\n",_rTvd3dNSTDStatus.fgIsNSTD, _rTvd3dNSTDStatus.wVTotal);
        if(_rTvd3dNSTDStatus.fgIsNSTD)
        {
            LOG(1,"[TVD_DBG_MSG] _svDrvTvdModeChgDone will trigger modeChg again\n");            
            _sbPDPModeChgDoneAgain = TRUE;    
        }
        else
        {
            _sbPDPModeChgDoneAgain = FALSE;
        }
    }
#endif

#if TVD_AUTO_DFT
    //if (!IS_AUTOSEARCH())        //by xiaokuan
    if(_rTvd3dStatus.fgVPres)
    {
        _svDrvTvdSetDftMode(((_rTvd3dStatus.fgIs525)?SV_CS_NTSC358:SV_CS_PAL));
    }
#endif

#if TVD_SET_ENABLED_CS
    vDrvTvd3dCheckMModeForEnabledCS();
#endif

#if TVD_FRS_FOR_NONSTDSIGNAL
    _wPreAvg_VLen = 0xff;
#endif

    #if TVD_NEW_HSYNCSLICER_SETTING
    _u1SyncLvCnt = _u4SyncLvSum = _u2SyncLvAvg = 0;
    _u4BYCLvSum  = _u2BYCLvAvg  =0;
    #endif

    vIO32WriteFldAlign(DFE_18, SV_ON, VMAXMIN_EN);
    
    vDrvTvdRstDCLAMPCnt();
    vIO32WriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW);
    vIO32WriteFldAlign(DFE_08, SV_OFF, DCLAMP_Y_EN);
    vIO32WriteFldAlign(DFE_0E, SV_OFF, DCLAMP_CHECK_LIM);

    #if 0
    if(fgHwTvd525())
    {
        vIO32WriteFldAlign(CDET_00, 0, MODE000); //Setup Mode000 for NTSC
    }
    else
    {
#ifdef FORCE_MODE000_NTSC
        vIO32WriteFldAlign(CDET_00, 0, MODE000); //Setup Mode000 for PAL
#else
        vIO32WriteFldAlign(CDET_00, 1, MODE000); //Setup Mode000 for PAL
#endif
    }
    #endif

    //Restore 443 and PAL_UP PAL_DN gain. (Fast chennel is On in SetChNo() in srvext.c). Benson.06.02.27.
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && _rTvd3dStatus.fgVPres)
    {
        vTvd3dFastChannelChange(SV_OFF);
    }

    if(_rTvd3dStatus.fgVPres == 0)
    {
        _rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_NOSIGNAL;
#if TVD_MAX_CLAMP_TABLE
        vIO32WriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_OFF, CLAMP_TABLE_05);
#endif

        //no snowscreen when ATV without signal
        if(_rTvd3dStatus.fgIs525 == 0)
        {
            vIO32WriteFldAlign(TG_05, SV_ON, INV_FIELD);
        }
    }
    else
    {
        _rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_STABLE;
#if TVD_ANA_COPY_PROTECT_FILTER
        vDrvTvd3dAnaCopyProInit();
        _sbAnaCopyProPSync = _sbPreAnaCopyProPSync = fgHwTvdIsPSync();
        _sbAnaCopyProCSTRIPE = _sbPreAnaCopyProCSTRIPE = fgHwTvdIsCSTRIPE();
        _sbAnaCopyProMvType2 = _sbPreAnaCopyProMvType2 = fgHwTvdIsMvType2();
#endif
        _svDrvTvdSetVDelay(_rTvd3dStatus.bTvdMode);
        // To avoid channel change AGC speed too slow, so set to defaule gain during mode change. Mark070711
        if(_sbIsTVDFixAgc != SV_ON)
        {
            vTvdSetAGCMode(E_AUTO_GAIN);
        }

#if TVD_MAX_CLAMP_TABLE
        vIO32WriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
#endif
        vIO32WriteFldAlign(TG_05, SV_OFF, INV_FIELD);
        //vIO32WriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_NML, BLANK_BW);
        vIO32WriteFldAlign(TG_17, TG_FRAME_STDFV_TH_STD, FRAME_STDFV_TH);
        vIO32WriteFldAlign(TG_17, TG_FRAME_STDFH_TH_STD, FRAME_STDFH_TH);
    }

#if TVD_VCR_BV
    vDrvTvdRstVCRBVCnt();
    if(_sbManualLLockGainbyDDI==FALSE)
    {
	    vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
	    vIO32WriteFldAlign(TG_04, SV_ON, LF_AUTO);
    }
#endif
#if TVD_FRS_FOR_NONSTDSIGNAL
    vDrvTvdForceUpdNSTD(TRUE);
#endif

#if TVD_VCR_BV_STA
    vDrvTvdRstVCRBVStaCnt();
#endif
#ifdef CC_TVE_POWERDOWN_HALF

    if(_rTvd3dStatus.fgIs525)
    {
        vIO32WriteFldAlign(SECAM_07, 0x1B0, YUV_CATCH_VPT);
    }
    else
    {
        vIO32WriteFldAlign(SECAM_07, 0x201, YUV_CATCH_VPT);
    }

    vIO32WriteFldAlign(SECAM_07, 0x1a0, Fld(10, 10, AC_MSKW21));//one pixel
    vIO32WriteFldAlign(SECAM_07, 6, YUV_CATCH_SEL);
#endif

    if(bTvdCtrl(TCTL_BP_MODECHG,TC_GETEN,0))
    {
        return;
    }

#if TVD_PHALT_MN_WA2

    if(_fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
    {
        if(IO32ReadFldAlign(CDET_00, TVD_MMODE))
        {
            _sbForcePalMN = TRUE;
            LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 ModeChgDone Force MMODE ON\n");
        }
        else
        {
            _sbForcePalMN = FALSE;
            LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 ModeChgDone Force MMODE OFF\n");
        }
    }

#endif
    _svDrvTvdUpdateActiveWH();
    
#ifdef CC_SUPPORT_PIPELINE
	if(_fVSCConnectAVD)
	{
		if(fgIsMainTvd3d())
		{
			vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
		}
		
		if(fgIsPipTvd3d())
		{
			vSetPipFlg(PIP_FLG_MODE_DET_DONE);
		}
	}
	//VSC do not connect the AVD it need to something
    else
    {
       
	   vTvd3dBHModeDone();
	   if(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_NOSIGNAL)  //need to check DTV scart out case, need?
	   {
#if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
		vApiTVEVideoStatusNotify(SV_VP_MAIN,SV_TVE_NOTIFY_VIDEO_NO_SIGNAL);
#endif
	   }

	   if((VSS_MAJOR(_fVFEAVDSourceMainNew) == VSS_SCART))
	   {
		   vDrvEnableBlankLevelAdjust();
	   }

	   
#if SUPPORT_VBI
	   {
		   VBI_ResetSrc(0x0);
	   }
#endif
#ifndef CC_SUPPORT_RECORD_AV
		if((bDrvVideoSignalStatus(0x0) == SV_VDO_STABLE))
		{
#if SUPPORT_SCART
	
			if(VSS_MAJOR(_fVFEAVDSourceMainNew) == VSS_SCART)
			{
				vDrvScartRGBFreeRun(SV_OFF);
			}
	
#endif
		}
#endif

#ifdef CC_UP8032_ATV
#if SUPPORT_VBI
#if SUPPORT_MM_INPUT
		//if(!fgIsExtSrcMM())
#endif
		{
			ApiVBIServiceResetSrc(bGetVideoDecType(0x0),bDrvVideoGetType(0x0),fgIsOversampleTiming());
		}
#endif
#endif

	}
#else
	if(fgIsMainTvd3d())
	{
		vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
	}

	if(fgIsPipTvd3d())
	{
		vSetPipFlg(PIP_FLG_MODE_DET_DONE);
	}
#endif
}

/**
 * @brief TVD NA state change bottom half
 * @param  void
 * @retval void
 */
static void _svDrvTvdBHNAStateChg(void)
{
    if(_na_state != _spre_na_state)
    {
        _svDrvTvdSetAVDelay(_rTvd3dStatus.bTvdMode);
    }

    //Caculate the statistics only when vpres and vlock on.
    if((_rTvd3dStatus.NAInfo.na_HysterCnt >_rTvd3dStatus.NAInfo.na_HysterCntThres)&&(_na_state != _spre_na_state))
    {
        _spre_na_state = _na_state;
        LOG(3,"[TVD_DBG_MSG] _svDrvTvdBHNAStateChg _na_state=%d \n",_na_state);

        switch(_na_state)
        {
            case NA_LOWNOISE:
                _svDrvTvdNAReset();
                vIO32WriteFldAlign(VSRC_08, SV_ON, VPRES_COCH_EN);
#if !TVD_NO_BYPASS_TRICK
                _sbBypassTrick = 0x0;
#endif
                vIO32WriteFldAlign(TG_08, SV_OFF, FDET_SEL);
                _svDrvTvdSetAVDelay(_rTvd3dStatus.bTvdMode);
                vIO32WriteFldAlign(CDET_00, SV_ON, CTG_ADAP); //Low noise adap use high gain
                vIO32WriteFldAlign(CTG_06, SV_OFF, BLOCK4DETSEL);//MT5387 new TH
                vIO32WriteFldAlign(TG_17, TG_FRAME_STDFV_TH_STD, FRAME_STDFV_TH);
                vIO32WriteFldAlign(TG_17, TG_FRAME_STDFH_TH_STD, FRAME_STDFH_TH);
                break;

            case NA_HIGHNOISE:
#if !TVD_NO_BYPASS_TRICK
                _sbBypassTrick = 0x0;
#endif
                vIO32WriteFldAlign(TG_08, SV_OFF, FDET_SEL);
                //vIO32WriteFldAlign(DFE_18, SV_OFF, VMAXMIN_EN);
                vIO32WriteFldAlign(CDET_00, SV_OFF, CTG_ADAP); //hi_nosise adap use high gain,hard to relock back
                vIO32WriteFldAlign(VSRC_08, SV_OFF, VPRES_COCH_EN);
                vIO32WriteFldAlign(TG_17, TG_FRAME_STDFV_TH_HN, FRAME_STDFV_TH);
                vIO32WriteFldAlign(TG_17, TG_FRAME_STDFH_TH_HN, FRAME_STDFH_TH);
                vIO32WriteFldAlign(CTG_06, SV_OFF, BLOCK4DETSEL);//MT5387 new TH

                //Set burst unlock threshold under PAL.
                switch(_rTvd3dStatus.bTvdMode)
                {
                    case SV_CS_SECAM:
                        _svDrvTvdSetCAGCBound4Data(SCM_HN_IP_MID_B, SCM_HN_IP_MID_A, SCM_HN_IP_TGT_B, SCM_HN_IP_TGT_A);
                        break;

                    case SV_CS_PAL_N:
#if TVD_PHALT_MN_WA2
		#if TVD_SUPPORT_DRVCUST_DEFINE
			_svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_PAL_N), DRVCUST_OptGet(eTVD_BstLock_Thr2_PAL_N));
		#else
                        _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_PAL_N, CTG_BSTLOCK_THR2_PAL_N);
		#endif
                        _svDrvTvdSetCAGCBound4Data(PAL_HN_IP_MID_B, PAL_HN_IP_MID_A, PAL_HN_IP_TGT_B, PAL_HN_IP_TGT_A);
                        break;
#endif

                    case SV_CS_PAL:
                    case SV_CS_PAL_60:
		#if TVD_SUPPORT_DRVCUST_DEFINE
			_svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_HN_625), DRVCUST_OptGet(eTVD_BstLock_Thr2_HN_625));
		#else
                        _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_625, CTG_BSTLOCK_THR2_HN_625);
		#endif
                        _svDrvTvdSetCAGCBound4Data(PAL_HN_IP_MID_B, PAL_HN_IP_MID_A, PAL_HN_IP_TGT_B, PAL_HN_IP_TGT_A);
                        break;

                    case SV_CS_PAL_M:
                    case SV_CS_NTSC358:
                    case SV_CS_NTSC443:
		#if TVD_SUPPORT_DRVCUST_DEFINE
			_svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_HN_525), DRVCUST_OptGet(eTVD_BstLock_Thr2_HN_525));
		#else
                        _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_525, CTG_BSTLOCK_THR2_HN_525);
		#endif
                        _svDrvTvdSetCAGCBound4Data(NTSC_HN_IP_MID_B, NTSC_HN_IP_MID_A, NTSC_HN_IP_TGT_B, NTSC_HN_IP_TGT_A);
                        break;

                    default:
                        assert(0);
                        break;
                }

                break;

            case NA_SUPERHIGHNOISE:
#if !TVD_NO_BYPASS_TRICK
                _sbBypassTrick = 0x01;
#endif
                vIO32WriteFldAlign(TG_08, SV_ON, FDET_SEL);
                //vIO32WriteFldAlign(DFE_18, SV_OFF, VMAXMIN_EN);
                vIO32WriteFldAlign(CDET_00, SV_OFF, CTG_ADAP); //hi_nosise adap use high gain,hard to relock back
                vIO32WriteFldAlign(VSRC_08, SV_OFF, VPRES_COCH_EN);
                vIO32WriteFldAlign(TG_17, TG_FRAME_STDFV_TH_HN, FRAME_STDFV_TH);
                vIO32WriteFldAlign(TG_17, TG_FRAME_STDFH_TH_HN, FRAME_STDFH_TH);
                vIO32WriteFldAlign(CTG_06, SV_ON, BLOCK4DETSEL);//MT5387 new TH

                //Set burst unlock threshold under PAL.
                switch(_rTvd3dStatus.bTvdMode)
                {
                    case SV_CS_SECAM:
                        _svDrvTvdSetCAGCBound4Data(SCM_HN_IP_MID_B, SCM_HN_IP_MID_A, SCM_HN_IP_TGT_B, SCM_HN_IP_TGT_A);
                        break;

                    case SV_CS_PAL_N:
#if TVD_PHALT_MN_WA2
		#if TVD_SUPPORT_DRVCUST_DEFINE
			_svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_PAL_N), DRVCUST_OptGet(eTVD_BstLock_Thr2_PAL_N));
		#else
                        _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_PAL_N, CTG_BSTLOCK_THR2_PAL_N);
		#endif
                        _svDrvTvdSetCAGCBound4Data(PAL_HN_IP_MID_B, PAL_HN_IP_MID_A, PAL_HN_IP_TGT_B, PAL_HN_IP_TGT_A);
                        break;
#endif

                    case SV_CS_PAL:
                    case SV_CS_PAL_60:
		#if TVD_SUPPORT_DRVCUST_DEFINE
			_svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_HN_625), DRVCUST_OptGet(eTVD_BstLock_Thr2_HN_625));
		#else
                        _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_625, CTG_BSTLOCK_THR2_HN_625);
		#endif
                        _svDrvTvdSetCAGCBound4Data(PAL_HN_IP_MID_B, PAL_HN_IP_MID_A, PAL_HN_IP_TGT_B, PAL_HN_IP_TGT_A);
                        break;

                    case SV_CS_PAL_M:
                    case SV_CS_NTSC358:
                    case SV_CS_NTSC443:
		#if TVD_SUPPORT_DRVCUST_DEFINE
			_svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_HN_525), DRVCUST_OptGet(eTVD_BstLock_Thr2_HN_525));
		#else
                        _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_525, CTG_BSTLOCK_THR2_HN_525);
		#endif
                        _svDrvTvdSetCAGCBound4Data(NTSC_HN_IP_MID_B, NTSC_HN_IP_MID_A, NTSC_HN_IP_TGT_B, NTSC_HN_IP_TGT_A);
                        break;

                    default:
                        assert(0);
                        break;
                }

                break;
        }
    }
}

/**
 * @brief Set TVD 625/525 line system detection gain
 * @param  bV525Gain : Range 0~3
 * @param  bV625Gain : Range 0~3
 * @retval void
 */
static void _svDrvTvdSetDetV525V625Gain(UINT8 bV525Gain, UINT8 bV625Gain) reentrant
{
    vIO32WriteFldMulti(CDET_0D,
    P_Fld(bV625Gain,DET_V625_GAIN)|
    P_Fld(bV525Gain,DET_V525_GAIN));
}

/**
 * @brief Set TVD Phalt up/dn gain
 * @param  bUpGain : Range 0~7
 * @param  bDnGain : Range 0~7
 * @retval void
 */
static void _svDrvTvdSetPALTGain(UINT8 bUpGain, UINT8 bDnGain)
{
    UINT8 bGain;
    bGain = ((bUpGain << 3) | bDnGain);
    vIO32WriteFldAlign(CDET_03, bGain,DET_PHALT_GAIN);
}

/**
 * @brief  Start Noise State
 * @param  void
 * @retval void
 */
static void _svDrvTvdNAStart(void)
{
    _na_state = (UINT8)NA_LOWNOISE;
    _spre_na_state = (UINT8)NA_UNKNOWN;
    _rTvd3dStatus.NAInfo.na_HysterCnt=0;
    _svDrvTvdNAReset();
}

/**
 * @brief  Stop Noise State
 * @param  void
 * @retval void
 */
static void _svDrvTvdNAStop(void)
{
    _na_state = (UINT8)NA_STOP;
    _svDrvTvdNAReset();
}


/**
 * @brief TVD Mode Change Handle Function
 *
 * When TVD mode change, we need to turn off NR, TDC and other HW that would write Dram.
 * Main/Pip MODE_CHG flags are issued accordingly.
 *
 * @bug
 *
 */
static void _svDrvTvdModeChg(void)
{
    LOG(1, "[TVD_DBG_MSG] _svDrvTvdModeChg\n");

    #if TVD_BP_ATV_MODECHG
    LOG(1, "[TVD_DBG_MSG] BypassModeChg = %d\n", _sbBypassModeChg);
    #endif

    #if TVD_PALM50_SUPPORT
    _su1PALM50BstLockCnt = 0;
    _su1PALM50BstUnLockCnt = 0;
    _su1PALM50VFreqChgCnt = 0;
    #endif

    #if TVD_ADJUST_AGC_SPEED_FOR_LOW_SYNC
    _su1BYCClipCnt=0;
    vIO32WriteFldAlign(DFE_04, DFE_BYC_LIM, AGC2_BYC_LIM);
    vIO32WriteFldAlign(DFE_05, DFE_PEAK_TC, AGC2_PEAK_TC);
    #endif

    #if TVD_ADJUST_Y4V_FOR_FIELD
    _su1SHNY4VBWCnt=0;
    //vIO32WriteFldAlign(DFE_02, 0x1, Y4V_SHN_BW);
    #endif

    _sbSerrType0 = FALSE;
    vIO32WriteFldAlign(TG_07, SV_ON , SERR_TYPE);   

    #if TVD_SERRCNT_WA
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && _rTvd3dStatus.bTvdMode==SV_CS_NTSC358)
    {
        _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT_LNA;
        vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
        //_u1NTSCVPorch = 0x0;
        //vIO32WriteFldAlign(TG_0A, _u1NTSCVPorch, TG_SAF_ADJ);
        _sbSerrCntAdjEn = TRUE;
        #if SUPPORT_VBI
        #if TVD_SERRTYPE_ONE		
        VBI_SetAVFieldType(_rTvd3dStatus.fgIs525, (IO32ReadFldAlign(TG_07, SERR_TYPE)), _rTvd3dStatus.bTvdMode); //Serrtype 0: Field Inversed.
        #endif		
        #endif
    }
    else
    {
        _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT;
        vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
        //_u1NTSCVPorch = 0x1;
        //vIO32WriteFldAlign(TG_0A, _u1NTSCVPorch, TG_SAF_ADJ);
        _sbSerrCntAdjEn = FALSE;
        #if SUPPORT_VBI
        #if TVD_SERRTYPE_ONE		
        VBI_SetAVFieldType(_rTvd3dStatus.fgIs525, !(IO32ReadFldAlign(TG_07, SERR_TYPE)), _rTvd3dStatus.bTvdMode); //Serrtype 0: Field Inversed.
        #endif		
        #endif

    }
    //_u1SmallSerrWidthCntInChChg = _u1SmallSerrWidthCnt = _u1LargeSerrWidthCnt = 0;
    //_sbSerrCntAdjBack=FALSE;
    //_sbSerrCntAdjBackCnt=0;
    #else
    _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT;
    #endif

#if TVD_SET_ENABLED_CS
    {
        RTvdEnabledCS_T *pTvdEnabledCS = &_rAvEnabledCS;

        if(_rTvd3dStatus.eSourceType==SV_ST_TV)
        {
            pTvdEnabledCS = &_rTvEnabledCS;
        }

        if(_bMModeForEnabledCS==TRUE)
        {
            _bTriggerModeDetForEnabledCS=_bMModeForEnabledCS=FALSE;
            vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); //Enable Menu Mode
            LOG(3, "[TVD_DBG_MSG] Stop MMode\n");
        }

        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnPALN, PALN_EN);
        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnPALM, PALM_EN);
        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnSECAM, SECAM_EN);
        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnPAL60, PAL60_EN);
        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnNTSC443, NTSC443_EN);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem PALN=%d\n", pTvdEnabledCS->u1TvdEnPALN);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem PAL=%d\n", pTvdEnabledCS->u1TvdEnPAL);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem PALM=%d\n", pTvdEnabledCS->u1TvdEnPALM);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem NTSC358=%d\n", pTvdEnabledCS->u1TvdEnNTSC358);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem SECAM=%d\n", pTvdEnabledCS->u1TvdEnSECAM);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem PAL60=%d\n", pTvdEnabledCS->u1TvdEnPAL60);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem NTSC443=%d\n", pTvdEnabledCS->u1TvdEnNTSC443);
    }
#endif

#if TVD_LIM_HERR_WA
    _sbLimHerrEn1 = _sbLimHerrEn2 = FALSE;
    _su1HerrTrickCnt = _su1HerrHSCnt = 0;
    //vIO32WriteFldAlign(TG_04, 0, LIM_HERR);
#endif


    //Disable VMAXMIN to prevent V525 fault.
    vIO32WriteFldAlign(DFE_18, SV_OFF, VMAXMIN_EN);
    
    if(_rTvd3dStatus.fgVPres != 0)
    {
        vIO32WriteFldAlign(TG_05, SV_OFF, INV_FIELD);
    }

    _svDrvTvdNAStop();
#if TVD_PHALT_MN_WA2

    if(_fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
    {
        if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
        {
            if(_sbPhalt_Vpres==0)
            {
                if(!_sbForcePalMN)
                {
                    vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                    LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE off (modechg Vpres off) \n");
                }
            }
            else
            {
                //LOG(3, "_svDrvTvdModeChg\n");
                //LOG(3, "NPTV TVD BLOCK = %d ,TVD_MMODE=%d ,_sbTvdModeCnt=%d\n",fgHwTvdBLock(),fgHwTvdIsMMode(),_sbTvdModeCnt);
                //LOG(3, "mode status=%d ,NR_level=%d \n",(IO32ReadFldAlign(STA_CDET_00, Fld(3, 24, AC_MSKB3))),bHwTvdNRLevel());
            #if TVD_SUPPORT_DRVCUST_DEFINE
		_svDrvTvdSetBstLockTh(DRVCUST_OptGet(eTVD_BstLock_Thr1_HN_625), DRVCUST_OptGet(eTVD_BstLock_Thr2_HN_625));
	    #else
                _svDrvTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_625, CTG_BSTLOCK_THR2_HN_625);
	    #endif
            }
        }
    }
    else
    {
        _sbForcePalMN = FALSE;

        if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
        {
            vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
        }
    }

#endif

#if TVD_PHALT_WA
    #if TVD_SET_ENABLED_CS
    if(_bMModeForEnabledCS==FALSE && _fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
    #else
    if(_fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
    #endif
    {
        if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
        {
            vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); 
            LOG(9,"[TVD_DBG_MSG] TVD_PHALT_MN_WA TVD_MMODE off \n");
        }
    }
#endif

    if(bTvdCtrl(TCTL_BP_MODECHG,TC_GETEN,0))
    {
        return;
    }

#if TVD_BP_ATV_MODECHG
    if(_sbBypassModeChg == FALSE)
    {
#endif
        
		#ifdef CC_SUPPORT_PIPELINE
		if(_fVSCConnectAVD)
		{
			if(fgIsMainTvd3d())
			{
				vSetMainFlg(MAIN_FLG_MODE_CHG);
				vClrMainFlg(MAIN_FLG_MODE_DET_DONE);	// Clear this flag!
			}
			
			if(fgIsPipTvd3d())
			{
				vSetPipFlg(PIP_FLG_MODE_CHG);
				vClrPipFlg(PIP_FLG_MODE_DET_DONE);	// Clear this flag!
			}
		}
		//when VSC do not connect the AVD it need to do something  
		else
		{
               #ifndef CC_SUPPORT_RECORD_AV
               #if SUPPORT_SCART
	  		   if(fgIsMainScart())
	  			{
	  				vDrvScartRGBFreeRun(SV_ON);
	  			}
               #endif
               #endif

			   {
                vDrvVideoTrigModeDetect(SV_VP_MAIN);

                if(fgIsMainTvd3d())
                {
                    vTvd3dBHModeChg();
                }

                #if SUPPORT_VBI
                {
                    VBI_ResetSrc(SV_VP_MAIN);
                }
                #endif
                _bMainState = VDO_STATE_WAIT_MODE_DET;
                LOG(1, "Change to VDO_STATE_WAIT_MODE_DET\n");
                }
                
#if defined(CC_SUPPORT_TVE) || defined(CC_SUPPORT_TVE_82xx)
	            vApiTVEVideoStatusNotify(SV_VP_MAIN, SV_TVE_NOTIFY_VIDEO_MODE_CHANGE);
#endif
		}
		#else
        if(fgIsMainTvd3d())
        {
            vSetMainFlg(MAIN_FLG_MODE_CHG);
            vClrMainFlg(MAIN_FLG_MODE_DET_DONE);	// Clear this flag!
        }

        if(fgIsPipTvd3d())
        {
            vSetPipFlg(PIP_FLG_MODE_CHG);
            vClrPipFlg(PIP_FLG_MODE_DET_DONE);	// Clear this flag!
        }
		#endif
#if TVD_BP_ATV_MODECHG
    }
#endif

#if TVD_SERRTYPE_ONE
    _swCochIir_Avg=0;
#endif
#if SUPPORT_SCART
    vDrvTvdRstSCnt();
#endif
    _rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_UNKNOWN;
    vTvd3dTrigModeDet(); // Reset Mode Change Counter
    bhwVCRSetting = 0x0; // Initialize it to Prefer nonVCR.
    vDrvTvdRstVCRCnt();
    _rTvd3dStatus.fgIsVCR = 0;
    vIO32WriteFldAlign(DFE_02, 1, POS_DIFF_ONLY); // also check hi-low pattern
    vDrvTvdRstHLOCKCnt();
	vDrvTvdRstCOCHDeCnt();
	vDrvTvdRstHsyncPosDetCnt();
#if TVD_OLD_VCR
    vDrvTvdRstOldVCRCnt();
#endif
#if TVD_SECAM_DET
    vDrvTvdRstBurstCnt();
#endif

#if TVD_ANA_COPY_PROTECT_FILTER
    vDrvTvd3dAnaCopyProInit();
#endif
#if TVD_BP_ATV_MODECHG
    if(_sbBypassModeChg == FALSE)
    {
#endif
        // Turn off TDC
        vDrvTDCActive(SV_OFF);
#if TVD_BP_ATV_MODECHG
    }
#endif

    if(bTvdCtrl(TCTL_MDSTBPROT,TC_GETEN,0))
    {
        vIO32WriteFldAlign(CDET_05, SV_ON, FORCE_UPD_MODE);
    }

    vIO32WriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_CCI, BLANK_BW);
}

/**
 * @brief TVD ISR SCART Setting
 * @param bUIScartMode IN: SCART mode
 * @param PbSFisSV INOUT:pointer to SRisSV
 * @param PbTrigScartAuto INOUT:pointer to bTrigScartAuto
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
#if SUPPORT_SCART  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
static void _svDrvTvdScartSet(UINT8 bUIScartMode,UINT8 *PbSFisSV,UINT8 *PbTrigScartAuto,RTvdStatus *pTvd3dStatus)
{
    if(_bMainICIn == P_FB0 || _bMainICIn == P_FB1
#if SUPPORT_POP
       || _bSubICIn == P_FB0 || _bSubICIn == P_FB1
#endif
      )
    {
        if(bUIScartMode == SV_SCART_AUTO)
        {
            if(fgHwTvdSVFBstDet())
            {
                vDrvTvdIncSCnt();

                if((fgDrvTvdIsSCnt(TVD_S_CNT) && (*PbSFisSV == SV_FALSE))||(*PbTrigScartAuto==SV_TRUE))
                {
                    *PbSFisSV = SV_TRUE;
                    *PbTrigScartAuto = SV_FALSE;
                    vApiVideoSetEvtFlg(VDO_FLG_SCART_SF_CHG);
                    _svDrvTvdModeChg();
                    LOG(6,"[TVD_DBG_MSG] _svDrvTvdScartSet fgHwTvdSVFBstDet True\n");
                }
            }
            else
            {
                vDrvTvdDecSCnt();

                if((fgDrvTvdIsSCnt(TVD_S_CNT2) && (*PbSFisSV == SV_TRUE))||(*PbTrigScartAuto==SV_TRUE))
                {
                    *PbSFisSV = SV_FALSE;
                    *PbTrigScartAuto = SV_FALSE;
                    vApiVideoSetEvtFlg(VDO_FLG_SCART_SF_CHG);
                    _svDrvTvdModeChg();
                    LOG(6,"[TVD_DBG_MSG] _svDrvTvdScartSet fgHwTvdSVFBstDet False\n");
                }
            }

#if TVD_NEW_BST_DET_TH
            vIO32WriteFldAlign(SVF_03, 0x8+(pTvd3dStatus->bCAGC>>3), SVF_BSTDET_TH);
#endif
        }
        else
        {
            vDrvTvdRstSCnt();  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
        }
    }
}
#endif


#if TVD_AUTO_CALIB
#ifdef CC_TVE_POWERDOWN_HALF
/**
 * @brief bDrvGetTVDADCOffset(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT16 manual offset code
 */
static UINT16 _swDrvTvdGetTVDADCOffset(void)
{
    return (UINT16)IO32ReadFldAlign(DFE_0A, CLAMP_TARGET_BLANK_LV);
}

static UINT8 _sbDrvTvdGetTvdCatch(UINT8 u1Stage, UINT32 u2X, UINT32 u2Y, UINT32 *pu2Y, UINT32 *pu2U, UINT32 *pu2V)
{
    vIO32WriteFldAlign(SECAM_07, u1Stage, YUV_CATCH_SEL);
    //vIO32WriteFldAlign(SECAM_07, u2X, YUV_CATCH_HPT);//four pixels
    vIO32WriteFldAlign(SECAM_07, u2X, Fld(10, 10, AC_MSKW21));//one pixel
    vIO32WriteFldAlign(SECAM_07, u2Y, YUV_CATCH_VPT);
    *pu2Y = IO32ReadFldAlign(STA_CTG_04, Fld(10, 0, AC_MSKW10));
    *pu2U = IO32ReadFldAlign(STA_CTG_04, Fld(10, 12, AC_MSKW21));
    *pu2V = IO32ReadFldAlign(STA_CTG_04, Fld(10, 22, AC_MSKW32));
    return SV_TRUE;
}
/**
 * _sbDrvTvdGetTvdCatchAverage()
 */
static UINT8 _sbDrvTvdGetTvdCatchAverage(UINT8 u1Stage, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 *pu4Y, UINT32 *pu4U, UINT32 *pu4V)
{
    UINT32 u4TotalPixel;
    UINT32 u4XIndex, u4YIndex;
    UINT32 u4DataY, u4DataU, u4DataV;
    UINT32 u4TotalY=0, u4TotalU=0, u4TotalV=0;

    if((u4Width ==0) || (u4Height==0))
    {
        return SV_FALSE;
    }

    for(u4XIndex=0; u4XIndex<u4Width; u4XIndex++)
    {
        for(u4YIndex=0; u4YIndex<u4Height; u4YIndex++)
        {
            if(_sbDrvTvdGetTvdCatch(u1Stage, u4X+(u4XIndex*2), u4Y+(u4YIndex*2), &u4DataY, &u4DataU, &u4DataV)== SV_FALSE)
            {
                return SV_FALSE;
            }
            else
            {
                u4TotalY +=u4DataY;
                u4TotalU +=u4DataU;
                u4TotalV +=u4DataV;
            }
        }
    }

    u4TotalPixel= (UINT32)(u4Width * u4Height);
    *pu4Y = (UINT32)(u4TotalY/ u4TotalPixel);
    *pu4U = (UINT32)(u4TotalU/ u4TotalPixel);
    *pu4V = (UINT32)(u4TotalV/ u4TotalPixel);
    return SV_TRUE;
}
#endif //end of CC_TVE_POWERDOWN_HALF
#endif //end of TVD_AUTO_CALIB


static BOOL _sfgDrvTvdCheckVLock(UINT8 bTryLoopCnt)
{
    UINT8 bLockedTime = 0;
    UINT16 wLockTarget = 0;
    UINT16 wMinDiff=0, wMinDiffAvg=0;
    UINT32 dwTunerNR = 0, dwTunerNRAvg = 0;

    if(bTryLoopCnt)
    {
#if TVD_MANUAL_TUNER_SCAN
		wLockTarget = bTryLoopCnt * _su1VlockRatio / 100;
#else
		wLockTarget = bTryLoopCnt * TVDLOCK_RATIO / 100;
#endif
    }

    _swTVDNRLevel = 0;
    _swTVDNRLevel_AVG = 0xffff;

	LOG(3,"[TVD_DBG_MSG] _sfgDrvTvdCheckVLock targetloopcnt(%d), loopCnt(%d)\n",wLockTarget,bTryLoopCnt);
    do
    {
        if(TRUE == fgHwTvdVLock())
        {
            bLockedTime++;
            vIO32WriteFldAlign(DFE_18, 5, VMAXMIN_STASEL);
            wMinDiff = (IO32ReadFldAlign(DFE_STA_09, VMAXMIN_STATUS));
            dwTunerNR = (TunerGetNoiseLv() >> 11) &0xFF;
#if 0
#ifdef USE_ATD_IF_DEMOD
            dwTunerNR = (ATD_GetNoiseLevel() >> 11) &0xFF;
#else
            dwTunerNR = 0;
#endif
#endif
            //            LOG(3,"!!![DIFF_MIX,NR] = [%x][%x] \n", wMinDiff,dwTunerNR);
            wMinDiffAvg += wMinDiff;
            dwTunerNRAvg += dwTunerNR;
            _swTVDNRLevel += bHwTvdNRLevel();

            if(bLockedTime >= wLockTarget)
            {
                if(bLockedTime)
                {
                    _swTVDNRLevel_AVG = _swTVDNRLevel/bLockedTime;
                    wMinDiffAvg = wMinDiffAvg/bLockedTime;
                    dwTunerNRAvg = dwTunerNRAvg/bLockedTime;
                    //                    LOG(3,"!!![DIFF_MIX,AVGNR] = [%x][%x]\n", wMinDiffAvg,dwTunerNRAvg );
                    LOG(3,"[TVD_DBG_MSG] _sfgDrvTvdCheckVLock [DIFF_MIX,AVGNR] = [%x][%x]\n", wMinDiffAvg,dwTunerNRAvg);
                }

	#if TVD_SUPPORT_DRVCUST_DEFINE
		if((wMinDiffAvg > DRVCUST_OptGet(eTVDScan_Diff_Min_TH)) && (dwTunerNRAvg < CSCAN_NR_TH))
	#else		
                if((wMinDiffAvg > CSCAN_DIFF_MIN_TH) && (dwTunerNRAvg < CSCAN_NR_TH))
	#endif
                {
					LOG(3,"[TVD_DBG_MSG] _sfgDrvTvdCheckVLock FAIL [DIFF_MIX,AVGNR] = [%x][%x]\n", wMinDiffAvg,dwTunerNRAvg);
                    return FALSE;
                }
                else
                {
                    if((wMinDiffAvg > 0x20))
                    {
                        LOG(3,"[TVD_DBG_MSG] _sfgDrvTvdCheckVLock OK [DIFF_MIX,AVGNR] = [%x][%x]\n", wMinDiffAvg,dwTunerNRAvg);
                    }

                    return (TRUE);
                }
            }
        }

        vUtDelay1ms(15);
        bTryLoopCnt--;
    }
    while(bTryLoopCnt != 0);

    //    LOG(3, "TVD VLock fail (%d/%d)\n", bLockedTime,wLockTarget);
    LOG(3,"[TVD_DBG_MSG] _sfgDrvTvdCheckVLock VLock fail (%d/%d)\n", bLockedTime,wLockTarget);
    return (FALSE);
}

/**
 * @brief TVD ISR Mode Change  detection
 * @param fgvpres IN: vpres Status.
 * @param fgIs525 IN: 525 Status.
 * @param bTvdMode IN: bTvdMode
 * @param preTvd443nPHALT INOUT: preTvd443nPHALT Status
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svDrvTvdModeChgDet(BOOL fgIs525,UINT8 bTvdMode,RTvdStatus *pTvd3dStatus)
{
    static UINT8 sbPreTvd443nPHALT = 0;

    if(bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0) == 0)
    {
        if(pTvd3dStatus->fgVPres)  //Issue Mode change only when VPres is on.
        {
            if(pTvd3dStatus->bTvdMode != bTvdMode)
            {
                LOG(1,"[TVD_DBG_MSG] tvd mode change %d->%d\n",pTvd3dStatus->bTvdMode,bTvdMode);
                pTvd3dStatus->bTvdMode = bTvdMode;

                switch(bTvdMode)
                {
                    case SV_CS_NTSC358:
                    case SV_CS_NTSC443:
                    case SV_CS_PAL_M:
                    case SV_CS_PAL_60:
                        pTvd3dStatus->fgIs525 = 1;
                        break;

                    default:
                        pTvd3dStatus->fgIs525 = 0;
                        break;
                };

                _svDrvTvdModeChg();
            }

            if((fgHwTvdIsMMode())&&(pTvd3dStatus->fgIs525 != fgIs525))
            {
                LOG(1,"[TVD_DBG_MSG] tvd 525 mode change %d->%d\n",pTvd3dStatus->fgIs525,fgIs525);
                pTvd3dStatus->fgIs525 = fgIs525;
                _svDrvTvdModeChg();
            }

#if TVD_SET_ENABLED_CS

            if(_bTriggerModeDetForEnabledCS==TRUE && _bMModeForEnabledCS==TRUE)
            {
                LOG(1, "[TVD_DBG_MSG] MMode trigger mode change %d->%d\n",pTvd3dStatus->fgIs525,fgIs525);
                pTvd3dStatus->fgIs525 = fgIs525;
                _svDrvTvdModeChg();
            }

#endif
        }
    }
    else
    {
        if(pTvd3dStatus->fgVPres)  //Issue Mode change only when VPres is on.
        {
            if(sbPreTvd443nPHALT != pTvd3dStatus->fgIs443nPHALT)
            {
                sbPreTvd443nPHALT = pTvd3dStatus->fgIs443nPHALT;
                vTvdResetChromaLock();
            }

            if(pTvd3dStatus->fgIs525 != fgIs525)
            {
                LOG(1,"[TVD_DBG_MSG] tvd 525 mode change %d->%d\n",pTvd3dStatus->fgIs525,fgIs525);
                pTvd3dStatus->fgIs525 = fgIs525;
                _svDrvTvdModeChg();
            }

#if TVD_SET_ENABLED_CS

            if(_bTriggerModeDetForEnabledCS==TRUE && _bMModeForEnabledCS==TRUE)
            {
                LOG(1, "[TVD_DBG_MSG] MMode trigger mode change %d->%d\n",pTvd3dStatus->fgIs525,fgIs525);
                pTvd3dStatus->fgIs525 = fgIs525;
                _svDrvTvdModeChg();
            }

#endif
        }
    }
}

#if TVD_ANA_COPY_PROTECT_FILTER
void _svDrvTvd3dAnaCopyProFilter(void)
{      
    UINT8 bAnaCopyProPSync, bAnaCopyProCSTRIPE, bAnaCopyProMvType2;
    if (_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
    {
        bAnaCopyProPSync = fgHwTvdIsPSync();
        bAnaCopyProCSTRIPE = fgHwTvdIsCSTRIPE();
        bAnaCopyProMvType2 = fgHwTvdIsMvType2();

        if (bAnaCopyProPSync != _sbPreAnaCopyProPSync)
            _su1AnaCopyProPSyncCnt =0;
        else if (_su1AnaCopyProPSyncCnt < TVD_ANA_COPY_PRO_MAX)
            _su1AnaCopyProPSyncCnt ++;

        if (bAnaCopyProCSTRIPE != _sbPreAnaCopyProCSTRIPE)
            _su1AnaCopyProCSTRIPECnt=0;
        else if (_su1AnaCopyProCSTRIPECnt < TVD_ANA_COPY_PRO_MAX)
            _su1AnaCopyProCSTRIPECnt ++;

        if (bAnaCopyProMvType2 != _sbPreAnaCopyProMvType2)
            _su1AnaCopyProMvType2Cnt=0;
        else if (_su1AnaCopyProMvType2Cnt < TVD_ANA_COPY_PRO_MAX)
            _su1AnaCopyProMvType2Cnt ++;

        if (_su1AnaCopyProPSyncCnt >= TVD_ANA_COPY_PRO_FILTER)
            _sbAnaCopyProPSync = bAnaCopyProPSync;

        if (_su1AnaCopyProCSTRIPECnt >= TVD_ANA_COPY_PRO_FILTER)
            _sbAnaCopyProCSTRIPE = bAnaCopyProCSTRIPE;

        if (_su1AnaCopyProMvType2Cnt >= TVD_ANA_COPY_PRO_FILTER)
            _sbAnaCopyProMvType2 = bAnaCopyProMvType2;

        _sbPreAnaCopyProPSync = bAnaCopyProPSync;
        _sbPreAnaCopyProCSTRIPE = bAnaCopyProCSTRIPE;
        _sbPreAnaCopyProMvType2 = bAnaCopyProMvType2;
        
    }
}
#endif

#if ENABLE_PRBS_BY_DRIVER
static void _svDrvTvdEnablePrbs(void)
{
    if(_bEnablePrbs==TRUE && _rTvd3dStatus.eSourceType==SV_ST_TV)
    {    
        if(_sbTvdModeCnt<30)
            LOG(6, "MCnt=%d, bSigStatus=%d\n", _sbTvdModeCnt,_rTvd3dStatus.bSigStatus);
        if(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
        {
            _bEnablePrbs = FALSE;
            vIO32WriteFldAlign(DFE_24, SV_OFF, PRBS_EN);
        }
        else
            vIO32WriteFldAlign(DFE_24, SV_ON, PRBS_EN);
    }
    else
    {
        vIO32WriteFldAlign(DFE_24, SV_OFF, PRBS_EN);
    }

}
#endif

#if TVD_VPRES4_IMPROVEMENT_FOR_WEAKSGINAL
static void _svDrvTvdSetVPres4PIC(void)
{
    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        if(IS_AUTOSEARCH())
        {
            vIO32WriteFldAlign(DFE_0E, 0x0, VPRES4PIC_MODE); //DFE referenced VPRES4
        }
        else
        {   
            if(!fgHwTvdVPresTVD3D())
                _su1VPresOffCnt++;
            else
                _su1VPresOffCnt=0;
            //if(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
            if(_su1VPresOffCnt==4)
            {
                vIO32WriteFldAlign(DFE_0E, 0x1, VPRES4PIC_MODE); //DFE referenced VPRES3
            }
            else if(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
            {
                vIO32WriteFldAlign(DFE_0E, 0x0, VPRES4PIC_MODE);
            }
        }
    }
    else
    {   
        vIO32WriteFldAlign(DFE_0E, 0x0, VPRES4PIC_MODE);
    }
}
#endif

static void _svDrvTvdSetModeUnknown(void)
{
    /*
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && if(_sbTvdModeCnt<30))
    {
        LOG(2, "STD_V525 = %d\n", IO32ReadFldAlign(STA_CDET_00, STD_V525));
        LOG(2, "STD_V625 = %d\n", IO32ReadFldAlign(STA_CDET_00, STD_V625));
        LOG(2, "V525_TVD3D = %d\n", IO32ReadFldAlign(STA_CDET_00, V525_TVD3D));
        LOG(2, "LOCK_VLEN = %d\n", IO32ReadFldAlign(TG_STA_01, LOCK_VLEN));
    }
    */

    if (IO32ReadFldAlign(STA_CDET_00, STD_V525)==1 && IO32ReadFldAlign(STA_CDET_00, STD_V625)==0)
    {
        vIO32WriteFldAlign(CDET_00, 0, MODE000); //Setup Mode000 for NTSC
        vIO32WriteFldAlign(CTG_06, 0xE3, TG_SEED_OFFSET); 
    }
    else if(IO32ReadFldAlign(STA_CDET_00, STD_V525)==0 && IO32ReadFldAlign(STA_CDET_00, STD_V625)==1)
    {
    #ifdef FORCE_MODE000_NTSC
        vIO32WriteFldAlign(CDET_00, 0, MODE000); //Setup Mode000 for PAL
    #else
        vIO32WriteFldAlign(CDET_00, 1, MODE000); //Setup Mode000 for PAL
    #endif
        vIO32WriteFldAlign(CTG_06, 0x40, TG_SEED_OFFSET); //for decreasing ringing 
    }
}

#if TVD_ADJUST_AGC_SPEED_FOR_LOW_SYNC
static void _svDrvTvdAdjustAGCForLowSync(void)
{
    if(_na_state==NA_LOWNOISE && _svDrvTvdGetNRLevel()>=75
    && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
    && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
    && !fgHwTvdFHPos() && !fgHwTvdFHNeg()
    && !fgHwTvdHeadSwitch() && !fgHwTvdTrick() && (bhwVCRSetting==(UINT8)VCR_NONE)
    && !fgHwTvdCoChannel() && !fgHwTvdVCRBV() && !fgHwTvdCNRLow()
    && IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)>0x40 && IO32ReadFldAlign(STA_CDET_01, CAGC_STA)<0x35 
    )
    {
        if(_su1BYCClipCnt<10)
        {
            _su1BYCClipCnt++;
        }
        else if(_su1BYCClipCnt==10)
        {
            _su1BYCClipCnt++;
            vIO32WriteFldAlign(DFE_04, DFE_BYC_LIM_CLIP, AGC2_BYC_LIM);
            vIO32WriteFldAlign(DFE_05, DFE_PEAK_TC_CLIP, AGC2_PEAK_TC);
        }
    }    
    else
    {
        _su1BYCClipCnt=0;
    }
}
#endif

#if TVD_ADJUST_Y4V_FOR_FIELD
static void _svDrvTvdSetY4V(void)
{
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && _na_state==NA_SUPERHIGHNOISE && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
    && fgHwTvdCoChannel() && fgHwTvdVCRBV() && fgHwTvdHeadSwitch()
    )
    {
        if(_su1SHNY4VBWCnt<10)
        {
            _su1SHNY4VBWCnt++;
        }
        else if(_su1SHNY4VBWCnt==10)
        {
            _su1SHNY4VBWCnt++;
            //vIO32WriteFldAlign(DFE_02, 0x0, Y4V_SHN_BW);
        }
    }
    else
    {
        _su1SHNY4VBWCnt = 0;
    }
}
#endif

#if 0//TVD_SERRCNT_WA
static void _svDrvTvdAdjustSerrCntWA(void)
{
    #if 0
    {
        UINT8 u1SerrWidth = IO32ReadFldAlign(TG_STA_00, SERR_WIDTH);
        if(_sbTvdModeCnt<12)
            LOG(1, "u1SerrWidth=0x%02X\n", u1SerrWidth);
    }
    #endif
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && fgHwTvdVPresTVD3D() && _rTvd3dStatus.bTvdMode==SV_CS_NTSC358)
    {
        UINT8 u1SerrWidth = IO32ReadFldAlign(TG_STA_00, SERR_WIDTH);
        if(!_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
        {
            if(_sbTvdModeCnt<=_u1SerrSetCnt && u1SerrWidth<0x58)
            {
                _u1SmallSerrWidthCntInChChg++;
            }
            if(_sbTvdModeCnt==_u1SerrSetCnt)
            {
                if(_u1SmallSerrWidthCntForce==0x59)
                   _u1SmallSerrWidthCntInChChg=0;
                else if(_u1SmallSerrWidthCntForce==0x88)
                    _u1SmallSerrWidthCntInChChg=3;
                    
                if(_u1SmallSerrWidthCntInChChg>=2)
                {
                    LOG(1, "Set serr_cnt as 0x88 by SmallSerrWidth\n");
                    _sbSerrType0 = FALSE;
                    vIO32WriteFldAlign(TG_07, SV_ON , SERR_TYPE);
                    _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT_LNA;
                    vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
                    //_u1NTSCVPorch = 0x0;
                    //vIO32WriteFldAlign(TG_0A, _u1NTSCVPorch, TG_SAF_ADJ);
                    _sbSerrCntAdjEn = TRUE;
                }
                else
                {
                    LOG(1, "Set serr_cnt as 0x59 by NormalSerrWidth\n");
                    _sbSerrType0 = TRUE;
                    vIO32WriteFldAlign(TG_07, SV_OFF , SERR_TYPE);
                    _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT;
                    vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
                    //_u1NTSCVPorch = 0x1;
                    //vIO32WriteFldAlign(TG_0A, _u1NTSCVPorch, TG_SAF_ADJ);
                    _sbSerrCntAdjEn = FALSE;
                }
            }
        }
        else
        {
            if(u1SerrWidth<0x58)
            {
                if(_u1SmallSerrWidthCnt<20)
                    _u1SmallSerrWidthCnt++;
            }
            else
            {
                _u1SmallSerrWidthCnt=0;
            }

            if(u1SerrWidth>0x70)
            {
                if(_u1LargeSerrWidthCnt<20)
                    _u1LargeSerrWidthCnt++;
            }
            else
            {
                _u1LargeSerrWidthCnt=0;
            }

            if(_sbTvdModeCnt>35)
            {
                if(_sbSerrCntAdjEn==TRUE)
                {
                    //if(_na_state==NA_SUPERHIGHNOISE || (_na_state==NA_HIGHNOISE && _u1SmallSerrWidthCnt<20))
                    if(_na_state==NA_SUPERHIGHNOISE)
                    {
                        LOG(1, "Change serr_cnt to 0x59 by SuperHighNoise and UnstableSerrWidth _u1SmallSerrWidthCnt=%d\n", _u1SmallSerrWidthCnt);
                        _sbSerrType0 = TRUE;
                        vIO32WriteFldAlign(TG_07, SV_OFF , SERR_TYPE);                        
                        _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT;
                        vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
                        //_u1NTSCVPorch = 0x1;
                        //vIO32WriteFldAlign(TG_0A, _u1NTSCVPorch, TG_SAF_ADJ);
                        _sbSerrCntAdjEn = FALSE;
                    }
                    if(_u1LargeSerrWidthCnt==20)
                    {
                    #if 0
                        LOG(1, "Change serr_cnt to 0x59 by LargeSerrWidth\n");
                        _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT;
                        vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
                        //_u1NTSCVPorch = 0x1;
                        //vIO32WriteFldAlign(TG_0A, _u1NTSCVPorch, TG_SAF_ADJ);
                        _sbSerrCntAdjEn = FALSE;
                    #endif
                    }
                }
                else
                {
                    if(_u1SmallSerrWidthCnt==20)
                    {
                    #if 0
                        LOG(1, "Change serr_cnt to 0x88 by SmallSerrWidth\n");
                        _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT_LNA;
                        vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
                        //_u1NTSCVPorch = 0x0;
                        //vIO32WriteFldAlign(TG_0A, _u1NTSCVPorch, TG_SAF_ADJ);
                        _sbSerrCntAdjEn = TRUE;
                    #endif
                    }
                    if((_u1SmallSerrWidthCntInChChg>=2) && (_na_state==NA_LOWNOISE || _na_state==NA_HIGHNOISE))
                    {
                        LOG(1, "Change serr_cnt to 0x88 by SmallSerrWidth and Good signal\n");
                        _sbSerrType0 = FALSE;
                        vIO32WriteFldAlign(TG_07, SV_ON , SERR_TYPE);                        
                        _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT_LNA;
                        vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
                        //_u1NTSCVPorch = 0x0;
                        //vIO32WriteFldAlign(TG_0A, _u1NTSCVPorch, TG_SAF_ADJ);
                        _sbSerrCntAdjEn = TRUE;
                    }
                }
            }
        }
    }
#if 0
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && fgHwTvdVPresTVD3D() && !_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)
    && _na_state==NA_LOWNOISE && fgHwTvdVLock()
    && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
    && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
    && !fgHwTvdFHPos() && !fgHwTvdFHNeg() /*&& !fgHwTvdTrick()*/ && !fgHwTvdCoChannel()
    && _rTvd3dStatus.bTvdMode==SV_CS_NTSC358
    )
    {    
        if(_sbSerrCntAdjEn==FALSE)
        {
            _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT_LNA;
            vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
            _sbSerrCntAdjEn = TRUE;
        #if SUPPORT_VBI
        #if TVD_SERRTYPE_ONE		
            VBI_SetAVFieldType(_rTvd3dStatus.fgIs525, (IO32ReadFldAlign(TG_07, SERR_TYPE)), _rTvd3dStatus.bTvdMode); //Serrtype 0: Field Inversed.
        #endif		
        #endif
        }
    }
    else if(fgHwTvdVPresTVD3D() && !_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
    {
        LOG(3, "_na_state=%d\n", _na_state==NA_LOWNOISE);
        LOG(3, "fgHwTvdVLock=%d\n", fgHwTvdVLock());
        LOG(3, "wHTotal=%d\n", _rTvd3dNSTDStatus.wHTotal);
        LOG(3, "wVTotal=%d\n", _rTvd3dNSTDStatus.wVTotal);
        LOG(3, "fgHwTvdFHPos=%d\n", !fgHwTvdFHPos());
        LOG(3, "fgHwTvdFHNeg=%d\n", !fgHwTvdFHNeg());
        //LOG(3, "fgHwTvdTrick=%d\n", !fgHwTvdTrick());
        LOG(3, "fgHwTvdCoChannel=%d\n", !fgHwTvdCoChannel());
        //LOG(3, "fgHwTvdCNRLow=%d\n", fgHwTvdCNRLow());
    }
    
    if(_sbSerrCntAdjBack==FALSE && _rTvd3dStatus.eSourceType==SV_ST_TV && fgHwTvdVPresTVD3D() 
    && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE) && _sbTvdModeCnt>100
    )
    {
        if(_na_state!=NA_LOWNOISE || !fgHwTvdVLock() || !fgHwTvdHsyncLock()
        || !IO32ReadFldAlign(TG_STA_05,LINE_STDFH_FLAG)
        || fgHwTvdFHPos() || fgHwTvdFHNeg() || fgHwTvdTrick() || fgHwTvdHeadSwitch()
        || fgHwTvdCoChannel()
        )
        {
            _sbSerrCntAdjBackCnt++;            
            LOG(3, "Serr change back to original \n");
            LOG(3, "_na_state=%d\n", _na_state!=NA_LOWNOISE);
            LOG(3, "fgHwTvdVLock=%d\n", !fgHwTvdVLock());
            LOG(3, "fgHwTvdHsyncLock=%d\n", !fgHwTvdHsyncLock());
            LOG(3, "LINE_STDFH_FLAG=%d\n", !IO32ReadFldAlign(TG_STA_05,LINE_STDFH_FLAG));
            LOG(3, "FRAME_STDFH_FLAG=%d\n", !IO32ReadFldAlign(TG_STA_05,FRAME_STDFH_FLAG));
            LOG(3, "fgHwTvdFHPos=%d\n", fgHwTvdFHPos());
            LOG(3, "fgHwTvdFHNeg=%d\n", fgHwTvdFHNeg());
            LOG(3, "fgHwTvdTrick=%d\n", fgHwTvdTrick());
            LOG(3, "fgHwTvdCoChannel=%d\n", fgHwTvdCoChannel());
            //LOG(3, "fgHwTvdVCRBV=%d\n", fgHwTvdVCRBV());
            LOG(3, "fgHwTvdHeadSwitch=%d\n", fgHwTvdHeadSwitch());
            LOG(3, "fgHwTvdCNRLow=%d\n", !fgHwTvdCNRLow());
            if(_sbSerrCntAdjBackCnt>5)
            {
                 LOG(1, "Serr change back to original DONE\n");
                /*
                _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT;
                vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
                */
                _sbSerrCntAdjBack = TRUE;
            }
        }
        else
        {
            _sbSerrCntAdjBackCnt = 0;
        }
    }
#endif
}
#endif

#if TVD_ADJSUT_PALSW_BY_VLOCK
static void _svDrvTvdAdjustPALSW(void)
{
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE) && _na_state==(UINT8)NA_SUPERHIGHNOISE)
    {
        if(!fgHwTvdVLock())
        {
            vIO32WriteFldAlign(CDET_00, SV_OFF, PALSW_FAST_ADAP);
            vIO32WriteFldAlign(CDET_08, SV_OFF, PALSW_STATE_EN);
            _su1VLockCntForPalsw=0;
        }
    }
    if(_rTvd3dStatus.eSourceType==SV_ST_TV && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
    {
        if(fgHwTvdVLock())
        {
            if(_su1VLockCntForPalsw<20)
                _su1VLockCntForPalsw++;
            else
            {
                vIO32WriteFldAlign(CDET_00, SV_ON, PALSW_FAST_ADAP);
                vIO32WriteFldAlign(CDET_08, SV_ON, PALSW_STATE_EN);
            }
        }
        else
        {
            _su1VLockCntForPalsw=0;
        }
    }
}
#endif

#if TVD_PALM50_SUPPORT
static void _svDrvTvdPalM50DetectProc(void)
{
    if(_sbTvdModeCnt<_sbTvd_McDone_cnt)
        LOG(1,"[TVD_DBG_MSG] _su1PALM50DetState = %d\n", _su1PALM50DetState);

    if(_su1PALM50DetState==PALM50_DET_IDEL && fgDrvTvdIsModeCnt(_sbTvd_McDone_cnt-2))
    {
        if((_rTvd3dStatus.bColSys == SV_CS_AUTO)&&(!fgHwTvdBLock4DET()) && (bHwTvdCAGC()<TVD_SCAN_CAGCThres)
        && (fgHwTvd525()==0) && (fgHwTvd443()==0) && (fgHwTvdPHOLD()==1) && (_rTvd3dStatus.bTvdMode == SV_CS_PAL_N) 
        )
        {
            vIO32WriteFldAlign(CDET_00, SV_ON, TVD_MMODE);
            vIO32WriteFldAlign(CDET_00, SV_CS_PAL_M, TVD_MODE);
            _su1PALM50DetState = PALM50_DET_START;
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT Set Manual PALM50\n");
        }
        else
        {
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT fgHwTvdBLock4DET=%d\n", fgHwTvdBLock4DET());
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT bHwTvdCAGC=%d\n", bHwTvdCAGC());
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT fgHwTvd525=%d\n", fgHwTvd525());
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT fgHwTvd443=%d\n", fgHwTvd443());
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT fgHwTvdPHOLD=%d\n", fgHwTvdPHOLD());
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT _rTvd3dStatus.bTvdMode=%d\n", _rTvd3dStatus.bTvdMode);
        }
    }
    else if(_su1PALM50DetState==PALM50_DET_START)
    {
        if(_sbTvdModeCnt<_sbTvd_McDone_cnt-2)
        {
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT _sbTvdModeCnt=%d , fgHwTvdBLock4DET=%d\n", _sbTvdModeCnt, fgHwTvdBLock4DET());
            if(fgHwTvdBLock4DET())
            {
                _su1PALM50BstLockCnt++;
            }
        }
        else if(_sbTvdModeCnt==_sbTvd_McDone_cnt-2)
        {
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT _sbTvdModeCnt=%d , _su1PALM50BstLockCnt=%d\n", _sbTvdModeCnt, _su1PALM50BstLockCnt);    
            //PALM50
            if(_su1PALM50BstLockCnt>5)
            {
                _su1PALM50DetState = PALM50_DET_DONE_PALM50;
            }
            else
            {
                if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
                    vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                _su1PALM50DetState = PALM50_DET_DONE_PALN;
            }
        }
    }
    else if(_su1PALM50DetState==PALM50_DET_DONE_PALM50)
    {
        if(fgHwTvd525()==1)
        {
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT 525 detected\n");
            _su1PALM50VFreqChgCnt++;
            if(_su1PALM50VFreqChgCnt>=3 && _rTvd3dStatus.bColSys == SV_CS_AUTO)
            {
                LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT Set to Auto mode by 525\n");
                vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                _su1PALM50DetState = PALM50_DET_IDEL;
            }
        }
        if((!fgHwTvdBLock4DET()) && (bHwTvdCAGC()<TVD_SCAN_CAGCThres))
        {
            LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT Bst unlock detected\n");
            _su1PALM50BstUnLockCnt++;
            if(_su1PALM50BstUnLockCnt>=3 && _rTvd3dStatus.bColSys == SV_CS_AUTO)
            {
                LOG(1,"[TVD_DBG_MSG] TVD_PALM50_SUPPORT Set to Auto mode by BST_UNLOCK\n");
                vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                _su1PALM50DetState = PALM50_DET_IDEL;
            }
        }
    }
}
#endif

#if TVD_LIM_HERR_WA
static void _svDrvTvdLimHerrWA(void)
{
    static UINT8 cnt=0;
    if(_sbTvdModeCnt < 100)
    {
        if(fgHwTvdTrick())
            _su1HerrTrickCnt++;
        if(fgHwTvdHeadSwitch())
            _su1HerrHSCnt++;
    }
    else if(_sbTvdModeCnt == 100)
    {
        LOG(0,"[TVD_DBG_MSG] _svDrvTvdLimHerrWA() _su1HerrTrickCnt(%d) \n", _su1HerrTrickCnt);
        LOG(0,"[TVD_DBG_MSG] _svDrvTvdLimHerrWA() _su1HerrHSCnt(%d) \n", _su1HerrHSCnt);

        if(_su1HerrHSCnt > HERR_HS_CNT_TH1)
            _sbLimHerrEn1 = TRUE;
        else
            _sbLimHerrEn1 = FALSE;
        
        if(_su1HerrHSCnt > HERR_HS_CNT_TH2)
            _sbLimHerrEn2 = TRUE;
        else
            _sbLimHerrEn2 = FALSE;
        
    }
    else
    {
        if(_rTvd3dStatus.eSourceType==SV_ST_TV && IO32ReadFldAlign(VSRC_07,ATD_ENABLE)
        && _svDrvTvdGetNRLevel()<=42 && _svDrvTvdGetNRLevel()>=37 
        && _sbLimHerrEn1 && !fgHwTvdVCRBV()&& !fgHwTvdCoChannel())
        {
            vIO32WriteFldAlign(TG_04, 0, LIM_HERR);
        }
        else if(_rTvd3dStatus.eSourceType==SV_ST_TV && _na_state==NA_SUPERHIGHNOISE
        && fgHwTvdHeadSwitch() && fgHwTvdHsyncLock() && _rTvd3dStatus.bTvdMode == SV_CS_NTSC358
        && _sbLimHerrEn2 && !fgHwTvdVCRBV() && !fgHwTvdCoChannel())
        {
            vIO32WriteFldAlign(TG_04, 0, LIM_HERR);
        }
        else
        {
            cnt++;
            if(cnt==200)
            {
                cnt=0;
                LOG(1, "_na_state==NA_SUPERHIGHNOISE(%d)\n",_na_state==NA_SUPERHIGHNOISE);
                LOG(1, "fgHwTvdHeadSwitch(%d)\n",fgHwTvdHeadSwitch());
                LOG(1, "fgHwTvdHsyncLock(%d)\n",fgHwTvdHsyncLock());
                LOG(1, "_rTvd3dStatus.bTvdMode == SV_CS_NTSC358 (%d)\n",_rTvd3dStatus.bTvdMode == SV_CS_NTSC358);
                LOG(1, "_sbLimHerrEn2(%d)\n",_sbLimHerrEn2);
                LOG(1, "!fgHwTvdVCRBV(%d)\n",!fgHwTvdVCRBV());
                LOG(1, "!fgHwTvdCoChannel(%d)\n",!fgHwTvdCoChannel());
                LOG(1, "_swLLStatus_avg(0x%02X)\n",_swLLStatus_avg);
            }
            vIO32WriteFldAlign(TG_04, 2, LIM_HERR);
        }
    }
    //LOG(1, "_swLLStatus_avg=(0x%04X)\n", _swLLStatus_avg);
}
#endif

#if TVD_ADAP_VPRES_SETTING
static void _svDrvTvdAdapVpres(void)
{
    if(_sbAdapVpresStart==TRUE && _rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        _su2AdapVpresCounter++;
        if(!fgHwTvdVPresTVD3D())
        {
            if(_su2AdapVpresCounter==35)
            {
                vTvd3dResetOn();
                vIO32WriteFldAlign(VSRC_08, SV_OFF, VPRES_COCH_EN);
                vTvd3dResetOff();
            }
        }
        else
        {
            _sbAdapVpresStart = FALSE;
        }
    }
}
#endif

/*****************  ********************************************************
 * Global Functions
 *************************************************************************/
 /**
 * @brief TVD Snow Mode Enable/Disable Setting
 * @param fgSnowMode IN: Snow Mode Status.
 * @param bSnowIntension IN: Snow Intension.
 * @retval : NOON
 */
void  _svSnowModeSet(BOOL fgSnowMode, UINT8 bSnowIntensity)
{
    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    {
	    vIO32WriteFldAlign(CTG_08, bSnowIntensity, CVBS_PRBS_SEL);//Setting Snow Intension
	    vIO32WriteFldAlign(DFE_24, fgSnowMode, PRBS_EN);//Enable/Disable Snow Mode	
    }
    else
    {
	    vIO32WriteFldAlign(CTG_08, 0x10, CVBS_PRBS_SEL);//Setting Snow Intension
	    vIO32WriteFldAlign(DFE_24, 0, PRBS_EN);//Enable/Disable Snow Mode	                
    }
}

#if TVD_AUTO_CALIB
UINT16 wDrvGetTVDYLvl(void)
{
#ifdef CC_TVE_POWERDOWN_HALF
    UINT32 u4Y, u4U, u4V, u4Y_sum=0;
    UINT8 i;

    for(i = 0; i<32; i++)
    {
        if(fgHwTvd525())
        {
            _sbDrvTvdGetTvdCatchAverage(6, 0x1a0, 0x1B0, 4, 4, &u4Y, &u4U, &u4V);
        }
        else
        {
            _sbDrvTvdGetTvdCatchAverage(6, 0x1a0, 0x201, 4, 4, &u4Y, &u4U, &u4V);
        }

        u4Y_sum += u4Y;
    }

    return (UINT16)(u4Y_sum >> 5) - _swDrvTvdGetTVDADCOffset();
#else
    UINT16 YSum = 0;
    UINT8 i;

    for(i = 0; i<16; i++)
    {
        YSum += IO32ReadFldAlign(DFE_STA_03, PY_LV);
        vUtDelay2us(20);
    }

    return YSum>>4;
#endif
}
#endif

/**
 * @brief TVD SVF bypass function
 *
 * This function only be called at CLI
 *
 * @param None
 * @return None
 */
#if SUPPORT_S_DETECT
void vDrvSetSVFBypass(UINT8 bOnOff)
{
    _sbSVFBypassNotify=bOnOff;
}
#endif

/**
     * @brief  Apply special setting and flow  for descrambler box
     *
     * This function only be called AP
     *
     * @param On/Off
     * @return None
     */
#if TVD_MANUAL_TUNER_SCAN
#if 1//defined(ANDROID)

/*anroid desramble ui on->off or off->on,the flag fgDeScrambleOnOffswich will be true
when set from off to on,android will set all the para. But _su1VpresGain should be default value and should not be modifed at this time
linux ui set from off to on ,it will only set desramble on flag*/
BOOL fgDeScrambleOnOffswich=FALSE;
BOOL fgBeforeDescramblestate=FALSE;

#endif
#endif
void vApiSetForDescrambler(BOOL fgDescramOn)
{
#if TVD_MANUAL_TUNER_SCAN
#if 1//defined(ANDROID)
	if(fgBeforeDescramblestate != fgDescramOn)
	{
		fgDeScrambleOnOffswich=TRUE;
	}
	else
	{
		fgDeScrambleOnOffswich=FALSE;
	}
	LOG(0,"[TVD_DBG_MSG] vApiSetForDescrambler fgDeScrambleOnOffswich=%d\n",fgDeScrambleOnOffswich);
#endif
#endif
    if(fgDescramOn)
    {
		LOG(3,"[TVD_DBG_MSG] vApiSetForDescrambler set on\n");
        _fgDescramblerOn=1;
    }
    else
    {
		LOG(3,"[TVD_DBG_MSG] vApiSetForDescrambler set off\n");
        _fgDescramblerOn=0;
    }

#if TVD_MANUAL_TUNER_SCAN
	_sfgManualScanadj = FALSE;
	_su1VlockRatio= TVDLOCK_RATIO;
#ifdef NEW_COUNTRY_TYPE
	if ((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_ATV_EU))
#else
	if ((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU))
#endif
	{
		if(_fgDescramblerOn) _su1VpresGain= 0x12;
		else _su1VpresGain = 0x14;
	}
	else
	{
		if(_fgDescramblerOn) _su1VpresGain= 0x3C;
		else _su1VpresGain = 0x3C;
	}
	
	LOG(0,"[TVD_DBG_MSG] vApiSetForDescrambler _su1VpresGain=0x%02x,_su1VlockRatio=%d\n",_su1VpresGain,_su1VlockRatio);
#endif
}

 
UINT16 wTvdCCI(void)
{
    return IO32ReadFldAlign(DFE_STA_08, COCH_IIR);
}
BOOL fgHwTvdVPres(void)
{
    if(IS_AUTOSEARCH())
    {
        return (IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D));
    }
    else
    {
        if(DRVCUST_OptGet(eTVDUseVPres4))
        {
            return ((IO32ReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))||(IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D)));
        }
        else
        {
            return (IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D));
        }

        //#ifdef CC_MT5363_TVD
        //        return ((IO32ReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))||(IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D)));
        //#else
        //        if (IS_5387_ECO())
        //            return ((IO32ReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))||(IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D)));
        //        else
        //            return (IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D));
        //#endif
    }
}


/**
 * @brief Enable/Disable CAGC adaptive control
 * @param bOnOff SV_ON: Enable / SV_OFF: Disable  CAGC ADAP
 * @return None
 */
void vTvd3dSetCAGCADAP(UINT8 bOnOff)
{
    LOG(6,"[TVD_DBG_MSG] vTvd3dSetCAGCADAP bOnOff=%d \n", bOnOff);

    if(_sbIsSetCAGCbyAP == SV_ON)
    {
        return;
    }

    vIO32WriteFldAlign(CDET_00, bOnOff, CAGC_ADAP);
}

/**
 * @brief SVideo Init funciton
 * @param bOnOff SV_ON: Enable / SV_OFF: Disable  SV Init
 * @return None
 */
#if SUPPORT_SCART
void vTvd3dSVInit(UINT8 bSVOnOff)
{
    LOG(6,"[TVD_DBG_MSG] vTvd3dSVInit bSVOnOff=%d \n", bSVOnOff);

    if(bSVOnOff==SV_ON)
    {
        _sbScart_SV = 1;
        // Set Y/C Delay.
        //vTvd3dSetYCDelay(0);
        vDrvTDCOnOff(SV_OFF);
    }
    else // SV Off
    {
        _sbScart_SV = 0;
        //vTvd3dSetYCDelay(0);
        //vDrvTDCSet();
        vDrvTDCActive(SV_ON);
    }
}
#endif


/**
 * @brief TVD Set VPres1 On/Off Gain and Threshold
 * TVD Set On/Off Gain and Threshold
 * @param bPath bPath indicates Main or PIP path currently TVD is connect to or disconnect from
 * @param bOnOff bOnOff indicates Connect or Disconnect
 * @return None
 */
void vDrvTvd3dVPresOnOff(UINT8 bGain, UINT8 bThrs) reentrant
{
#if TVD_MANUAL_VPRES_FOR_TUNING
    if((TRUE == _sbManualVPresForTuning) && (SV_ST_TV == _rTvd3dStatus.eSourceType)) return;
#endif
    if(_sbManualVPresGainbyDDI==TRUE) return;
#if SUPPORT_DFE_VPRES_DET	//default
    // Set OnOff Gain to 1:1
    #if TVD_STRONG_VP_GAIN_FOR_US
    if(COUNTRY_US==GetTargetCountry())
    {
        vIO32WriteFldMulti(DFE_0F, P_Fld(2 ,VP1_ON_GAIN)|P_Fld(1 ,VP1_OFF_GAIN));
    }
    else
    #endif
    {
        vIO32WriteFldMulti(DFE_0F, P_Fld((bGain >> 4),VP1_ON_GAIN)|P_Fld((bGain & 0xF),VP1_OFF_GAIN));
    }
    // 2005/07/26 Set Threshold of OnOff Gain
    vIO32WriteFldAlign(DFE_0F, bThrs, VP1_THR);

#endif
}

void vDrvTvd3dSetVPresOnOffGain(UINT8 bOffGain, UINT8 bOnGain, UINT8 bThrs)
{
    if(bOffGain==0xFF && bOnGain==0xFF)
        _sbManualVPresGainbyDDI = FALSE;
    else
    {
        _sbManualVPresGainbyDDI = TRUE;
        #if SUPPORT_DFE_VPRES_DET   //default
    	vIO32WriteFldMulti(DFE_0F, P_Fld((bOnGain & 0xF),VP1_ON_GAIN)|P_Fld((bOffGain & 0xF),VP1_OFF_GAIN));
    	vIO32WriteFldAlign(DFE_0F, bThrs, VP1_THR);
        #endif
    }
}

void vDrvTvd3dSetSyncDetForTuning(BOOL fgLowSens)
{
#if TVD_MANUAL_VPRES_FOR_TUNING
    _sbManualVPresForTuning = TRUE;
    vIO32WriteFldMulti(DFE_0E,P_Fld(1, VPRES_SEL)|P_Fld(0x04, VPRES_MASK)); //Check VPRES 1 & VPRES 2
    vIO32WriteFldAlign(DFE_0F, DFE_NORMAL_ONOFF_THRD, VP1_THR);
    if(fgLowSens) //Lower sensitivity for auto tuning
    {
        vIO32WriteFldMulti(DFE_0F, P_Fld(1 ,VP1_ON_GAIN)|P_Fld(3 ,VP1_OFF_GAIN));
    }
    else //Higher sensitivity for normal
    {
        vIO32WriteFldMulti(DFE_0F, P_Fld(3 ,VP1_ON_GAIN)|P_Fld(5 ,VP1_OFF_GAIN));
    }
#else
    UNUSED(fgLowSens);
#endif 
}

void vDrvTvd3dSetHPLL(UINT8 u1KPGain, UINT8 u1KIGain, UINT32 u4ColSys)
{
    if(u1KPGain==0xFF && u1KIGain==0xFF)
    {
        _sbManualLLockGainbyDDI = FALSE;
        vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
        vIO32WriteFldAlign(TG_04, SV_ON, LF_AUTO);
    }
    else
    {
        _sbManualLLockGainbyDDI = TRUE;
        vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
        vIO32WriteFldAlign(TG_04, SV_OFF, LF_AUTO);
        vIO32WriteFldAlign(TG_04, u1KPGain, LF_KP0);
        vIO32WriteFldAlign(TG_04, u1KIGain, LF_KI1);
    }
}


void vTvdHoldMode(UINT8 bHoldOnOff)
{
    _bTvdHoldMode = bHoldOnOff;
    //	LOG(6,"[TVD_DBG_MSG] vTvdHoldMode bHoldOnOff=%d \n",bHoldOnOff);
}


/**
 * @brief The HTotal of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the HTotal of current input signal of TVDecoder.
 *
 * @param None
 * @return the HTotal of current video signal
 *
 */
UINT16 wTvdHTotal(void)
{
    if(_rTvd3dStatus.fgIs525)
    {
        return IO32ReadFldAlign(SYS_00,DATA_SYNC_DUAL)?1716:858;
    }
    else
    {
        return IO32ReadFldAlign(SYS_00,DATA_SYNC_DUAL)?1728:864;
    }
}

/**
 * @brief The VTotal of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the VTotal of current input signal of TVDecoder.
 *
 * @param None
 * @return the VTotal of current video signal
 *
 */
UINT16 wTvdVTotal(void)
{
    if(_rTvd3dStatus.fgIs525)
    {
        return 525;
    }
    else
    {
        return 625;
    }
}

/**
 * @brief measure if current input signal is Non-Standard Signal Status or not in V-sync ISR
 *
 * (Common Function of Video Decoders)
 * return none.
 *
 * @param VTotal and HTotal
 * @return none
 *
 */
#if TVD_FRS_FOR_NONSTDSIGNAL
void vTvd3dChkNSTD(UINT16 VTotal, UINT16 HTotal, RTvdNSTDStatus *pRTvdNSTDStatus)
{
    if(VTotal!=_wPreAvg_VLen)
    {
        _wPreAvg_VLen = VTotal;
        vDrvTvdRstNONSTDCnt();
    }

    if(TVDAbsDiff(HTotal, _wPreLCnt) > 3)
    {
        _wPreLCnt = HTotal;
        vDrvTvdRstNONSTDCnt();
    }

    if((((VTotal!=524)&&(VTotal!=624))||
        ((TVDAbsDiff(HTotal, 1715) > 3) && (TVDAbsDiff(HTotal, 1727) > 3)))&&
       (_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)))
    {
        vDrvTvdIncNONSTDCnt();

        if(fgDrvTvdIsNONSTDCnt(TVD_NONSTD_CNT) || vDrvTvdIsForceUpdNSTD())
        {
            pRTvdNSTDStatus->fgIsNSTD = 1;
            pRTvdNSTDStatus->wVTotal = VTotal+1;
            pRTvdNSTDStatus->wHTotal = HTotal+1;
            pRTvdNSTDStatus->bRefreshRate = (UINT8)((27000000 + (((UINT32)(pRTvdNSTDStatus->wHTotal)/2 * (UINT32)(pRTvdNSTDStatus->wVTotal))>>1))/((UINT32)(pRTvdNSTDStatus->wHTotal)/2 * (UINT32)(pRTvdNSTDStatus->wVTotal)));

            if(_rTvd3dStatus.fgIs525)
            {
                pRTvdNSTDStatus->wVTotalDiff = pRTvdNSTDStatus->wVTotal + 0x800 - 525;
            }
            else
            {
                pRTvdNSTDStatus->wVTotalDiff = pRTvdNSTDStatus->wVTotal + 0x800 - 625;
            }

            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD Non STD Detected !\n");
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD fgIsNSTD=%x\n",pRTvdNSTDStatus->fgIsNSTD);
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wVTotal=%d\n",pRTvdNSTDStatus->wVTotal);
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wHTotal=%d\n",pRTvdNSTDStatus->wHTotal);
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD bRefreshRate=%d\n",pRTvdNSTDStatus->bRefreshRate);
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wVTotalDiff=%d\n",pRTvdNSTDStatus->wVTotalDiff);
        }
    }
    else
    {
        vDrvTvdDecNONSTDCnt();

        if(fgDrvTvdIsNONSTDCnt(TVD_NONSTD_CNT2) || vDrvTvdIsForceUpdNSTD())
        {
            pRTvdNSTDStatus->fgIsNSTD = 0;
            pRTvdNSTDStatus->wVTotal = VTotal+1;
            pRTvdNSTDStatus->wHTotal = HTotal+1;
            pRTvdNSTDStatus->bRefreshRate = (UINT8)((27000000 + (((UINT32)(pRTvdNSTDStatus->wHTotal)/2 * (UINT32)(pRTvdNSTDStatus->wVTotal))>>1))/((UINT32)(pRTvdNSTDStatus->wHTotal)/2 * (UINT32)(pRTvdNSTDStatus->wVTotal)));

            if(_rTvd3dStatus.fgIs525)
            {
                pRTvdNSTDStatus->wVTotalDiff = pRTvdNSTDStatus->wVTotal + 0x800 - 525;
            }
            else
            {
                pRTvdNSTDStatus->wVTotalDiff = pRTvdNSTDStatus->wVTotal + 0x800 - 625;
            }

            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD STD Detected !\n");
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD fgIsNSTD=%x\n",pRTvdNSTDStatus->fgIsNSTD);
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wVTotal=%d\n",pRTvdNSTDStatus->wVTotal);
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wHTotal=%d\n",pRTvdNSTDStatus->wHTotal);
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD bRefreshRate=%d\n",pRTvdNSTDStatus->bRefreshRate);
            LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wVTotalDiff=%d\n",pRTvdNSTDStatus->wVTotalDiff);
        }
    }

    vDrvTvdForceUpdNSTD(FALSE);
}
#endif

/*
* pseudo sync and AGC pluses status
* 0: OFF,1: ON
*
*/
BOOL vTvd3dGetAnalogCopyProtectPSync()
{
	if(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
	{
		return fgDrvTvdIsPSync();
	}

	return FALSE;
}

/*
*  color stripe: 
*  00:OFF, 01:OFF, 11: 2-line cstirpe, 10 4-line cstripe
*
*/
UINT8 vTvd3dGetAnalogCopyProtectCStripe()
{
	UINT8 u1CStripe =0;

	if(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
	{
		UINT8 u1MV_CSTRIPE = fgDrvTvdIsCSTRIPE() ? 1:0;
		UINT8 u1MV_TYPE2   = fgDrvTvdIsMvType2() ? 1:0;

		u1CStripe = (u1MV_CSTRIPE<<1) + u1MV_TYPE2;
	}

	return u1CStripe;
}

/**
 * @brief The Non-Standard Signal Status of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the NSTD Signal Status of current input signal of TVDecoder.
 *
 * @param pTvdNSTDStatus  - A pointer to RTvdNSTDStatus to receive tvd status
 * @return the Signal Status of current video signal, return values include
 * fgIsNSTD, bRefreshRate, wVTotal and wVTotalDiff.
 *
 */
void vTvd3dGetNSTDStatus(RTvdNSTDStatus *pTvdNSTDStatus)
{
    assert(pTvdNSTDStatus != NULL);
    pTvdNSTDStatus->fgIsNSTD = _rTvd3dNSTDStatus.fgIsNSTD;
    pTvdNSTDStatus->bRefreshRate = _rTvd3dNSTDStatus.bRefreshRate;
    pTvdNSTDStatus->wVTotal = _rTvd3dNSTDStatus.wVTotal;
    pTvdNSTDStatus->wHTotal = _rTvd3dNSTDStatus.wHTotal;
    pTvdNSTDStatus->wVTotalDiff = _rTvd3dNSTDStatus.wVTotalDiff;
}


/**
 * @brief enable/disable Force TVD Manual VPRES
 * @param bMVPresOnOff: SV_ON / SV_OFF
 * @note: No used currently
 */
void vTvdForceTvdManualVPres(UINT8 bMVPresOnOff)
{
    if(bMVPresOnOff==SV_ON)
    {
        vIO32WriteFldAlign(DFE_0E, SV_ON, MVPRES_TVD_EN);
        vIO32WriteFldAlign(DFE_0E, SV_TRUE, MVPRES_TVD);
    }
    else if(bMVPresOnOff==SV_OFF)
    {
        vIO32WriteFldAlign(DFE_0E, SV_OFF, MVPRES_TVD_EN);
        vIO32WriteFldAlign(DFE_0E, SV_FALSE, MVPRES_TVD);
    }
}


/**
 * @brief enable/disable white peak limitator
 * @param eAgcType: reference to TVD_AGC_T Type
 */
void vTvdSetAGCMode(TVD_AGC_T eAgcType) reentrant
{
    if(IO32ReadFldAlign(VSRC_07,ATD_ENABLE) && (eAgcType == E_AUTO_GAIN))
    {
        eAgcType = E_MENU_AGAIN;
    }

    vIO32WriteFldAlign(DFE_03, eAgcType, AGC2_MODE);
}
/**
 * @brief enable/disable white peak limitator
 * @param bOnOff: SV_ON/SV_OFF
 */
void vDrvWhitePeakLimit(UINT8 bOnOff)
{
    //	if(!_sbIsTVDFixAgc)
    vIO32WriteFldAlign(DFE_03, ((bOnOff == SV_ON) ? 0xf : 0x7), AGC2_PK_MODE);// AGC_LIMIT_EN
    //	else
    //		vIO32WriteFldAlign(DFE_03, 0x7, AGC2_PK_MODE);// AGC_LIMIT_EN
}
/**
 * @brief bDrvGetTVDADCGain(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT8 manual analog gain code
 */
UINT8 bDrvGetTVDADCGain(void)
{
    return (UINT8)IO32ReadFldAlign(DFE_07, AGC2_MANUAL_ACODE);
}
/**
 * @brief bDrvSetTVDADCAGAIN(UINT8 value)
 * call by d_custom_vid_set
 * @param  value manual analog gain code
 * @retval void
 */
void vDrvSetTVDADCGain(UINT8 value)
{
    _sbTvdMenuAcode = value;
    vIO32WriteFldAlign(DFE_07, value, AGC2_MANUAL_ACODE);
}
/**
 * @brief bDrvGetTVDADCDGain(void)
 * call by
 * @param  void
 * @retval UINT8 manual analog gain code
 */
UINT16 bDrvGetTVDADCDGain(void)
{
    return (UINT16)IO32ReadFldAlign(DFE_07, AGC2_MANUAL_DCODE);
}
/**
 * @brief bDrvSetTVDADCDGAIN(UINT8 value)
 * call by
 * @param  value manual analog gain code
 * @retval void
 */
void vDrvSetTVDADCDGain(UINT16 value)
{
    _swTvdMenuDcode = value;
    vIO32WriteFldAlign(DFE_07, value, AGC2_MANUAL_DCODE);
}


/**
 * @brief Set TVD Anti-Aliasing Filter
 * call by
 * @param  bOnOff: SV_ON: Enable AAF / SV_OFF: Bypass AAF
 * @param  bValue 0:7.5HZ, 1:4.2MHZ, 2:5.0MHZ, 3:5.7MHZ, 4:6.0MHZ, 5:6.5MHZ
 * @retval void
 * @Note: If call this function, TVD internal driver won't set AAF again.
 */
void vTvd3dSetAAFbyAP(UINT8 bOnOff, UINT8 bValue)
{
    _sbIsSetAAFbyAP = SV_ON;

    if(bOnOff)
    {
        vIO32WriteFldAlign(VSRC_07, SV_OFF, BYPASS_AAF);
    }
    else
    {
        vIO32WriteFldAlign(VSRC_07, SV_ON, BYPASS_AAF);
    }

    vIO32WriteFldAlign(VSRC_07, bValue, AAF_SEL);
}


/**
 * @brief Get UI Delay
 * @param  void
 * @retval void
 */
void vTvd3dGetUIDelay(void)
{
#if 0
    UINT16 wTVD_AVDelay;
    wTVD_AVDelay = _swDrvTvdGetDftMenuAvDelay(_rTvd3dStatus.bTvdMode);
    _su1UIDelay = _swDrvTvdGetMenuAvDelay(_rTvd3dStatus.bTvdMode)- wTVD_AVDelay;
#endif
}

/**
 * @brief Set TVD Anti-Aliasing Filter
 * call by
 * @param  bValue: 0:7.5HZ, 1:4.2MHZ, 2:5.0MHZ, 3:5.7MHZ, 4:6.0MHZ, 5:6.5MHZ
 * @retval void
 */
void vTvd3dSetAAF(UINT8 bValue)
{
    if(_sbIsSetAAFbyAP == SV_ON)
    {
        return;
    }

    vIO32WriteFldAlign(VSRC_07, bValue, AAF_SEL);
}

#if TVD_MANUAL_TUNER_SCAN
void vDrvTvdGetScanadjInfo(UINT8 * u1VpresGain, UINT8 * u1VlockRatio)
{
	if (u1VpresGain && u1VlockRatio)
	{
		*u1VpresGain = _su1VpresGain;
		*u1VlockRatio = _su1VlockRatio;
		LOG(0,"[TVD_DBG_MSG] vDrvTvdGetScanadjInfo get on_gain(%d), off_gain(%d), vlock_ratio(%d)\n",
			(*u1VpresGain >>4), (0xF & *u1VpresGain), *u1VlockRatio);
	}
}

void vDrvTvdSetScanadjInfo(UINT8 u1VpresGain, UINT8 u1VlockRatio)
{
	if (!IS_AUTOSEARCH() &&  _fgDescramblerOn)
	{
	#if 1//defined(ANDROID)
		if(fgDeScrambleOnOffswich==TRUE)
		{
			
			LOG(0,"[TVD_DBG_MSG] vDrvTvdSetScanadjInfo set dummy\n");
			
		}
		else
	#endif
		{
			_sfgManualScanadj = TRUE;
			if (u1VlockRatio <= 100)
			{
				_su1VlockRatio = u1VlockRatio;
			}
			_su1VpresGain = u1VpresGain;
		}
	
		LOG(0,"[TVD_DBG_MSG] vDrvTvdSetScanadjInfo set on_gain(%d), off_gain(%d), vlock_ratio(%d)\n",
			(_su1VpresGain >>4), (0xF & _su1VpresGain), _su1VlockRatio);
	}
}
#endif


#if 0
BOOL _bGameModeCheckDone = FALSE;
BOOL _bGameModeApplyDone = FALSE;
UINT32 _u4GameModeCheckFlag = 0;
UINT16 _u2GameModeCheckAgain = 0x1F;
UINT16 _u2GameModeCheckDgain = 0x800;
UINT16 _u2GameModeCheckCAGC = 0x40;
void vTvd3dCheckCombNormalMode(void)
{
    //_sbTvdModeCnt
    if(fgDrvTvdIsModeCnt(10))
    {
        _bGameModeCheckDone = FALSE;
        _bGameModeApplyDone = FALSE;
        _u4GameModeCheckFlag = 0;
        _u2GameModeCheckAgain = IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE);
        _u2GameModeCheckDgain = IO32ReadFldAlign(DFE_STA_04, DGAIN_CODE);
        _u2GameModeCheckCAGC = IO32ReadFldAlign(STA_CDET_01, CAGC_STA);
    }

    //Check indicator
    if(fgDrvTvdModeCntGt(10) && _bGameModeCheckDone==FALSE)
        //if(fgDrvTvdModeCntGt(10) && _sbTvdModeCnt<25)
    {
        LOG(1, "vTvd3dCheckCombNormalMode-mcnt=%d\n", _sbTvdModeCnt);

        if((BSP_GetIcVersion()==IC_VER_5396_AA || BSP_GetIcVersion()==IC_VER_5368_AA) && _rTvd3dStatus.bTvdMode == SV_CS_PAL)
        {
            _bGameModeCheckDone = TRUE;
        }
        else if(_rTvd3dStatus.eSourceType==SV_ST_TV)
        {
            _bGameModeCheckDone = TRUE;
            _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<0));
        }
        else
        {
            UINT16 u2Blank = IO32ReadFldAlign(DFE_STA_01, BLANK_LV);
            UINT16 u2AGain = IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE);
            UINT16 u2DGain = IO32ReadFldAlign(DFE_STA_04, DGAIN_CODE);
            UINT16 u2CochIIR = IO32ReadFldAlign(DFE_STA_08, COCH_IIR);
            UINT16 u2AvgVLen = IO32ReadFldAlign(TG_STA_01, AVG_VLEN);
            UINT16 u2LLockStatus = IO32ReadFldAlign(TG_STA_03, LLOCK_STATUS);
            BOOL bLineStdFh = IO32ReadFldAlign(TG_STA_05, LINE_STDFH_FLAG);
            UINT16 u2LCnt = IO32ReadFldAlign(TG_STA_05, STA_LCNT);
            UINT16 u2NRLevel = IO32ReadFldAlign(STA_CDET_00, NR_LEVEL);
            UINT16 u2CAGC = IO32ReadFldAlign(STA_CDET_01, CAGC_STA);

            /*
            if(u2Blank == u2AGain == u2DGain == u2CochIIR == u2AvgVLen == u2LLockStatus == u2NRLevel == u2CAGC == 0)
                ;
            LOG(1, "vTvd3dCheckCombNormalMode-BLK=0x%X\n", u2Blank);
            LOG(1, "vTvd3dCheckCombNormalMode-AGain=0x%X\n", u2AGain);
            LOG(1, "vTvd3dCheckCombNormalMode-DGain=0x%X\n", u2DGain);
            LOG(1, "vTvd3dCheckCombNormalMode-COCH_IIR=0x%X\n", u2CochIIR);
            LOG(1, "vTvd3dCheckCombNormalMode-AvgVLen=%d\n", u2AvgVLen);
            LOG(1, "vTvd3dCheckCombNormalMode-LLockStatus=0x%X\n", u2LLockStatus);
            LOG(1, "vTvd3dCheckCombNormalMode-LCNT=%d\n", IO32ReadFldAlign(TG_STA_05, STA_LCNT));
            LOG(1, "vTvd3dCheckCombNormalMode-LLockStatus=0x%X\n", u2NRLevel);
            LOG(1, "vTvd3dCheckCombNormalMode-CAGC=0x%X\n", u2CAGC);
            */

            if(TVDAbsDiff(u2Blank, 0x100) > 10)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-BLK=0x%X\n", u2Blank);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<1));
            }

            if(_u2GameModeCheckAgain != u2AGain)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-DGain=0x%X\n", _u2GameModeCheckAgain);
                LOG(1, "vTvd3dCheckCombNormalMode-AGain=0x%X\n", u2AGain);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<2));
            }

            if(TVDAbsDiff(_u2GameModeCheckDgain, u2DGain) > 10)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-DGain=0x%X\n", _u2GameModeCheckDgain);
                LOG(1, "vTvd3dCheckCombNormalMode-DGain=0x%X\n", u2DGain);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<3));
            }

            if(u2AGain<0x1A || u2AGain>0x21)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-AGain=0x%X\n", u2AGain);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<4));
            }

            if(u2CochIIR>0x3)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-COCH_IIR=0x%X\n", u2CochIIR);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<5));
            }

            if(u2AvgVLen!=524 && u2AvgVLen!=624)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-AvgVLen=%d\n", u2AvgVLen);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<6));
            }

            if(u2LLockStatus>0x5)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-LLockStatus=0x%X\n", u2LLockStatus);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<7));
            }

            if(!bLineStdFh)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-LCNT=%d\n", u2LCnt);

                if(TVDAbsDiff(u2LCnt, 1716) < 3 || TVDAbsDiff(u2LCnt, 1728) < 3)
                {
                    LOG(1, "vTvd3dCheckCombNormalMode-LCNT=STD\n");
                }
                else
                {
                    _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<8));
                }
            }

            if(u2NRLevel>0x10)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-NRLevel=0x%X\n", u2NRLevel);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<9));
            }

            if(TVDAbsDiff(_u2GameModeCheckCAGC, u2CAGC) > 5)
            {
                LOG(1, "vTvd3dCheckCombNormalMode-CAGC=0x%X\n", u2CAGC);
                _u4GameModeCheckFlag = (_u4GameModeCheckFlag | (1<<10));
            }

            if(_u4GameModeCheckFlag>0)
            {
                _bGameModeCheckDone = TRUE;
            }
        }
    }

    if(fgDrvTvdIsModeCnt(_sbTvd_McDone_cnt-1))
    {
        _bGameModeCheckDone = TRUE;
    }

    if(_bGameModeCheckDone==TRUE && _bGameModeApplyDone==FALSE)
    {
        _bGameModeApplyDone = TRUE;

        if((BSP_GetIcVersion()==IC_VER_5396_AA || BSP_GetIcVersion()==IC_VER_5368_AA) && _rTvd3dStatus.bTvdMode == SV_CS_PAL)
        {
            LOG(1, "Force TDC Normal mode for PAL\n");
            //vDrvSetTDCGameMode(SV_OFF);
        }
        else if(_rTvd3dStatus.eSourceType==SV_ST_TV)
        {
            LOG(1, "Apply TDC Game mode in TV input\n");
            vDrvSetTDCGameMode(SV_ON);
        }
        else
        {
            if(_u4GameModeCheckFlag==0)
            {
                LOG(1, "Apply TDC Normal mode in CVBS input\n");
                //vDrvSetTDCGameMode(SV_OFF);
            }
            else
            {
                LOG(1, "Apply TDC Game mode in CVBS input -_u4GameModeCheckFlag=0x%08X\n", _u4GameModeCheckFlag);
                vDrvSetTDCGameMode(SV_ON);
            }
        }
    }
}
#endif

/**
 * @brief TVD Mode Change Done bottom half
 *
 * @bug
 */

void vTvd3dBHModeDone(void)
{
#ifdef CC_SUPPORT_RECORD_AV
    if(_sbATVPVRBypassModeChg==TRUE)
    {
        LOG(1,"======================================\n");
        LOG(1,"[TVD_DBG_MSG] vTvd3dBHModeDone bypass modechg \n");
        LOG(1,"======================================\n");
        _sbATVPVRBypassModeChg = FALSE;
        return;
    }
    #if SUPPORT_SCART	
    if(fgIsMainScart() || fgIsPipScart())
    {
        vDrvScartRGBFreeRun(SV_OFF);
    }
    #endif
#endif

    /* call by vVdoMainState(), vVdoPipState() */
    if(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
    {
        _svDrvTvdNAStart();

        if(bTvdCtrl(TCTL_SERRTYPE,TC_GETEN,0))
        {
            vIO32WriteFldAlign(TG_07,bTvdCtrl(TCTL_SERRTYPE,TC_GETVAL,0), SERR_TYPE);
        }
        else
        {
#if TVD_SERRTYPE_ONE
            if(_sbSerrType0 == FALSE)
            {
                //if(((_rTvd3dStatus.bTvdMode == SV_CS_PAL_M)||(_sbAvgCOCH_detect))&&(!IS_AUTOSEARCH()))
                if(_sbAvgCOCH_detect&&(!IS_AUTOSEARCH()))
                {
                    vIO32WriteFldAlign(TG_07, SV_OFF, SERR_TYPE);
                }
                else
                {
                    vIO32WriteFldAlign(TG_07, SV_ON, SERR_TYPE);
                }
            }
#else
            vIO32WriteFldAlign(TG_07, SV_OFF, SERR_TYPE);
#endif
        }

        vIO32WriteFldAlign(TG_06, 0x1, FDLY_SEL);//for VBI (1 line delay)
        vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);

        switch(_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL_N:
            case SV_CS_PAL:
            case SV_CS_PAL_M:
            case SV_CS_PAL_60:
                //vIO32WriteFldAlign(CDET_06, CDET_KBURST_PAL, KBURST);
                vIO32WriteFldAlign(CDET_06, CDRT_KHYST_625, KHYST);
                vIO32WriteFldAlign(CDET_01, CDET_NA_TH_HIGH_PAL, TH_HIGH);

                if(IO32ReadFldAlign(VSRC_07,ATD_ENABLE))
                {
                    vIO32WriteFldAlign(CDET_01, CDET_NA_TH_SUPER_PAL_ATD, TH_SUPER);
                }
                else
                {
                    vIO32WriteFldAlign(CDET_01, CDET_NA_TH_SUPER_PAL, TH_SUPER);
                }

                if(COUNTRY_US==GetTargetCountry() && _rTvd3dStatus.eSourceType==SV_ST_TV 
                && ((_na_state == (UINT8)NA_SUPERHIGHNOISE)||(_svDrvTvdGetNRLevel()<=42))
                && IO32ReadFldAlign(CDET_00, PALM_EN) && IO32ReadFldAlign(CDET_00, PALN_EN))
				{
    			    _svDrvTvdSetPALTGain(5, 2);
                }
    			else
    			{
    			    _svDrvTvdSetPALTGain(5, 3);
                }
                vIO32WriteFldAlign(CDET_06, SV_OFF, SCF_GAIN_SEL);//MT5387 YJ
                break;

            case SV_CS_SECAM:
                //vIO32WriteFldAlign(CDET_06, CDET_KBURST_SECAM, KBURST);
                vIO32WriteFldAlign(CDET_06, CDRT_KHYST_625, KHYST);
                vIO32WriteFldAlign(CDET_01, CDET_NA_TH_HIGH_SECAM, TH_HIGH);
                vIO32WriteFldAlign(CDET_01, CDET_NA_TH_SUPER_SECAM, TH_SUPER);
                _svDrvTvdSetPALTGain(3, 3);
                vIO32WriteFldAlign(CDET_06, SV_ON, SCF_GAIN_SEL);//MT5387 YJ
                break;

            default://NTSC358, NTSC443
                //vIO32WriteFldAlign(CDET_06, CDET_KBURST_NTSC, KBURST);
                vIO32WriteFldAlign(CDET_06, CDRT_KHYST_525, KHYST);
                vIO32WriteFldAlign(CDET_01, CDET_NA_TH_HIGH_NTSC, TH_HIGH);
                vIO32WriteFldAlign(CDET_01, CDET_NA_TH_SUPER_NTSC, TH_SUPER);
                if(COUNTRY_US==GetTargetCountry() && _rTvd3dStatus.eSourceType==SV_ST_TV 
                && ((_na_state == (UINT8)NA_SUPERHIGHNOISE)||(_svDrvTvdGetNRLevel()<=42))
                && IO32ReadFldAlign(CDET_00, PALM_EN) && IO32ReadFldAlign(CDET_00, PALN_EN))
                {
                    _svDrvTvdSetPALTGain(5, 2);
                }
                vIO32WriteFldAlign(CDET_06, SV_OFF, SCF_GAIN_SEL);//MT5387 YJ
                break;
        }

        //Setting High/Super High NA TH for Non Std FH
        if(IS_FH_NON_STD())
        {
            vIO32WriteFldAlign(CDET_01, CDET_NA_TH_HIGH_NSTD_FH, TH_HIGH);//for NTSC V Line fix FH -1500Hz H-unlock
            vIO32WriteFldAlign(CDET_01, CDET_NA_TH_SUPER_NSTD_FH, TH_SUPER);
        }

        //Only use IF compensation under NTSC TV.
        if((_rTvd3dStatus.eSourceType==SV_ST_TV)&&(_rTvd3dStatus.bTvdMode==SV_CS_NTSC358))
        {
            vIO32WriteFldAlign(CTG_05, CTG_NTSC_IFCOMP_TV, IFCOMP_SEL);
        }
        else
        {
            vIO32WriteFldAlign(CTG_05, CTG_NTSC_IFCOMP_DFT, IFCOMP_SEL);
        }

        if(_sbIsTVDOffsetbyAP == SV_ON)//for TVD offset
        {
            _wTargetBlk = DFE_BLANK_TARGET;
        }

        //------Set Blank Level & Related PeakY Limit
        _svDrvTvdSetTVDBlk(_rTvd3dStatus.bTvdMode, _wTargetBlk);
        _swAGCTargetSyncHeight = _swDrvTvdGetTVDAGCTarget();
        vDrvTDCSet();

        // Turn on TDC
        if((_rTvd3dStatus.eSourceType != SV_ST_SV)
#if SUPPORT_SCART
           &&(_sbScart_SV!=1)
#endif
          )
        {
            // Before Turn on TDC, Set the parameter of TDC
            vDrvTDCActive(SV_ON);
        }

        if(_rTvd3dStatus.fgIs525)
        {
            // Set DEF VSYNC MASK to avoid BAD VBI affecting Peak white limit.
            vIO32WriteFldMulti(TG_01, P_Fld(SV_ON,FRONT_VMASK_SEL)|
                               P_Fld(SV_ON,FRONT_VMASK_NEW)|
                               P_Fld(TG_FRONT_VMASK_START,FRONT_VMASK_START)|
                               P_Fld(TG_FRONT_VMASK_STOP_525,FRONT_VMASK_END));
            //vIO32Write1BMsk(TG_01_0, 0xc2, 0xf7);//	FRONT_VMASK_SEL|FRONT_VMASK_NEW|FRONT_VMASK_START|FRONT_VMASK_END
            vIO32WriteFldAlign(TG_02, TG_MASK_STOP_525, MASK_STOP);
            vIO32WriteFldAlign(TG_16, ATD_VMASK_START_NTSC, ATD_VMASK_START);
            //vIO32WriteFldAlign(PQCRC_01, ATD_VMASK_START_NTSC, ATD_VMASK_START_I2C);
        }
        else
        {
            // Set DEF VSYNC MASK to avoid BAD VBI affecting Peak white limit.
            vIO32WriteFldMulti(TG_01, P_Fld(SV_ON,FRONT_VMASK_SEL)|
                               P_Fld(SV_ON,FRONT_VMASK_NEW)|
                               P_Fld(TG_FRONT_VMASK_START,FRONT_VMASK_START)|
                               P_Fld(TG_FRONT_VMASK_STOP_625,FRONT_VMASK_END));
            //vIO32Write1BMsk(TG_01_0, 0xc0, 0xf7);// FRONT_VMASK_SEL|FRONT_VMASK_NEW|FRONT_VMASK_START|FRONT_VMASK_END
            vIO32WriteFldAlign(TG_02, TG_MASK_STOP_625, MASK_STOP);//for PAL macrovision
            vIO32WriteFldAlign(TG_16, ATD_VMASK_START_PAL, ATD_VMASK_START);
            //vIO32WriteFldAlign(PQCRC_01, ATD_VMASK_START_PAL, ATD_VMASK_START_I2C);
        }
    }
}

/**
* @brief TVD Mode Change bottom half
*
* @bug
*/

void vTvd3dBHModeChg(void)
{
#ifdef CC_SUPPORT_RECORD_AV
    if(_sbATVPVRBypassModeChg==TRUE)
    {
        LOG(1,"======================================\n");
        LOG(1,"[TVD_DBG_MSG] vTvd3dBHModeChg bypass modechg \n");
        LOG(1,"======================================\n");
        return;
    }
    #if SUPPORT_SCART
    if(fgIsMainScart() || fgIsPipScart())
    {
        vDrvScartRGBFreeRun(SV_ON);
    }
    #endif
#endif

    // Wrtie AV_Delay at once when TVDMode changes. Benson.060213
    _svDrvTvdSetAVDelay(_rTvd3dStatus.bTvdMode);
    _svDrvTvdSetCAgcUVPeak(_rTvd3dStatus.bTvdMode);
#if 0
    LOG(1, "Apply TDC Normal mode in ModeChg\n");
    vDrvSetTDCGameMode(SV_OFF);
#endif
#if TVD_USE_NEW_DAGC
#if TVD_DAGC_WA

    if(_sbIsTVDFixAgc)
    {
        vDrvTvdRstDCODEStaCnt();
        vIO32WriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL, FIXAGC_DAGCSEL);
    }

#endif
#endif
}


/**
* @brief TVD MainLoop routine for reduce ISR usage
*  in 539x improve TVD ISR max exec time from 57us to 34 us
* @bug
*/
void vTvd3dMainLoop(void)
{
    UINT8 fgIsMV;

    if(_rTvd3dStat.bIsMain||_rTvd3dStat.bIsPip)
    {
        _svDrvTvdQtestStaUpdate();
#if SUPPORT_VBI
#if TVD_SERRTYPE_ONE
        #if TVD_SERRCNT_WA
        if(_sbSerrCntAdjEn==FALSE)
        {
            VBI_SetAVFieldType(_rTvd3dStatus.fgIs525, 
                            !(IO32ReadFldAlign(TG_07, SERR_TYPE)), //Serrtype 0: Field Inversed.
                            _rTvd3dStatus.bTvdMode);
        }
        else
        {
            VBI_SetAVFieldType(_rTvd3dStatus.fgIs525, 
                            (IO32ReadFldAlign(TG_07, SERR_TYPE)), //Serrtype 0: Field Inversed.
                            _rTvd3dStatus.bTvdMode);
        }
        #else
        {
            VBI_SetAVFieldType(_rTvd3dStatus.fgIs525, 
                            !(IO32ReadFldAlign(TG_07, SERR_TYPE)), //Serrtype 0: Field Inversed.
                            _rTvd3dStatus.bTvdMode);
        }
        #endif
#endif
#endif
        //For Y/C signal at scart color abnormal(TDC need)	by wensheng
#if SUPPORT_SCART

        if(fgIsSrcScart(_rTvd3dStat.bIsPip))
        {
            if(fgDrvGetScartModeIsSv())
            {
                _rTvd3dStatus.eSourceType = SV_ST_SV;
            }
            else
            {
                _rTvd3dStatus.eSourceType = SV_ST_AV;
            }
        }

#endif

        if(IS_AUTOSEARCH())
        {
            if(DRVCUST_OptGet(eTVDPalNtscMixAutosearch))
            {
                if(_sbPreAutoSearch != _fgAutoSearch)
                {
                    if(!_rTvd3dStatus.fgVPres)
                    {
                        //						LOG(3,"auto search mode change\n");
                        LOG(2,"[TVD_DBG_MSG] vTvd3dMainLoop auto search mode change\n");
                        _svDrvTvdModeChg();
                    }

                    _sbPreAutoSearch = _fgAutoSearch ;
                }

                if(_rTvd3dStatus.bColSys != SV_CS_AUTO)
                {
                    _svDrvTvdSetDftMode(((_rTvd3dStatus.fgIs525)?SV_CS_NTSC358:SV_CS_PAL));
                }
                else
                {
                    _svDrvTvdSetDftMode(SV_CS_NTSC358);
                }

                if(!_rTvd3dStatus.fgVPres)
                {
                    _rTvd3dStatus.fgIs525 = 0x01;
                    _rTvd3dStatus.bTvdMode = 0xff;
                }
            }

            vTvd3dFastChannelChange(SV_ON);
            _svDrvTvdNAStart();
        }
        else
        {
            if(DRVCUST_OptGet(eTVDPalNtscMixAutosearch))
            {
                _sbPreAutoSearch = 0;
            }

            if(_rTvd3dStatus.bSigStatus != (UINT8)SV_VDO_NOSIGNAL)
            {
                _svDrvTvdBHNAStateChg();
            }
        }

        if(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
        {
            fgIsMV = fgHwTvdIsPSync();//fgHwTvdIsMacroVision

            if(_rTvd3dStatus.fgIsMV != fgIsMV)
            {
                vApiNotifyMVChg(_rTvd3dStat.bIsMain ? SV_VP_MAIN : SV_VP_PIP, fgIsMV);
                _rTvd3dStatus.fgIsMV = fgIsMV;
            }
        }
    }

    if(_rTvd3dStatus.eSourceType == SV_ST_TV)
    {
        if(_u1TvdRstCnt<=3)
        {
            LOG(1, "_u1TvdRstCnt=%d\n", _u1TvdRstCnt);
            #if TVD_CHCHG_RESET_CORE_BY_AP
            if(_u1TvdRstCnt==3)
                vTvd3dReset();
            #endif
           _u1TvdRstCnt++; 
        }
    }
}


/**
 * @brief TVD Interrupt Service Routine
 *
 * handles VDO On/Off, Mode Change, MacroVision On/Off and VSync Interrupt
 *
 * @param wStatus wStatus contains Interrupt Status Information
 * @return None
 * @warning Don't place too many code in ISR function to avoid blocking the execution of main flow
 *
 * @pre TVD is connected, and some interrupt happened
 * @post Video interrupt is properly handled
 */
void vTvd3dVSyncISR(void)
{
    BOOL fgVPres, fgIs525, fgIsVCR, fgTrick;
    UINT8 bTvdMode;

    //********************************** VSync Interrupt***************************************
    if(!((_rTvd3dStat.bIsMain && fgIsVdoIntMainDet()) || (_rTvd3dStat.bIsPip && fgIsVdoIntPipDet())))
    {
        return;
    }

    if(bTvdCtrl(TCTL_BP_ISR,TC_GETEN,0))
    {
        return;
    }

    _bTvdISRCnt++;
#if TVD_VCR_BV_STA
    _sbDrvTvdSetVCRBVSta(fgHwTvdVCRBV(), &_fgVCRBV);
#endif
    //-------------------------TVD Vpres State-------------------------------------------//
#if TVD_VPRES_STATE

    if(DRVCUST_OptGet(eTVDUseVPres4))
    {
        fgPreVPres_0 = _sbDrvTvdVpresStateMachine(&_sbVpresState);
    }
    else
    {
        fgPreVPres_0 = fgHwTvdVPres();
    }

#else
    fgPreVPres_0 = fgHwTvdVPres();
#endif
    fgVPres=fgPreVPres_0;
    bTvdMode = bHwTvdMode();
    fgIs525 = fgHwTvd525();

#if TVD_MODE_Deglitch
    _sbDrvTvdModeDeglish(&fgVPres , &bTvdMode, &fgIs525);
#else

    if(!fgHwTvdFHNeg())
    {
        fgIs525 = fgHwTvd525();
    }
    else
    {
        switch(bTvdMode)
        {
            case SV_CS_NTSC358:
            case SV_CS_NTSC443:
            case SV_CS_PAL_M:
            case SV_CS_PAL_60:
                fgIs525 = 1;
                break;

            default:
                fgIs525 = 0;
                break;
        };
    }

#endif
#if TVD_SET_ENABLED_CS
    {
        //UINT8 fgIs443nPHALT = (fgHwTvd443()<<1)|fgHwTvdPHALT();
        if(_sbTvdModeCnt<20)
        {
            LOG(6, "[TVD_DBG_MSG] mcnt=%d\n", _sbTvdModeCnt);
            LOG(6, "[TVD_DBG_MSG] bTvdMode=%d, bHwTvdMode=%d\n", bTvdMode, bHwTvdMode());
            LOG(6, "[TVD_DBG_MSG] Pre-BLOCK=%d, Cur-BLOCK=%d\n", _rTvd3dStatus.fgIsBurstLock, fgHwTvdBLock());
            LOG(6, "[TVD_DBG_MSG] Pre-CKill=%d, Cur-CKill=%d\n", _rTvd3dStatus.fgIsCkill, fgHwTvdCKill());
            LOG(6, "[TVD_DBG_MSG] Pre-525=%d, Cur-525=%d\n", _rTvd3dStatus.fgIs525, fgIs525);
            LOG(6, "[TVD_DBG_MSG] Pre-443&Phalt=%d, Cur-443&Phalt=%d\n", _rTvd3dStatus.fgIs443nPHALT, (fgHwTvd443()<<1)|fgHwTvdPHALT());
        }
    }

    if(_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE) && _bMModeForEnabledCS==TRUE)
    {
        UINT8 fgIs443nPHALT = (fgHwTvd443()<<1)|fgHwTvdPHALT();

        if(_rTvd3dStatus.fgIs525 != fgIs525
           || _rTvd3dStatus.fgIs443nPHALT != fgIs443nPHALT)
        {
            _bTriggerModeDetForEnabledCS = TRUE;
            LOG(3, "[TVD_DBG_MSG] Should Trigger Mode Change\n");
            LOG(3, "[TVD_DBG_MSG] Pre-BLOCK=%d, Cur-BLOCK=%d\n", _rTvd3dStatus.fgIsBurstLock, fgHwTvdBLock());
            LOG(3, "[TVD_DBG_MSG] Pre-CKill=%d, Cur-CKill=%d\n", _rTvd3dStatus.fgIsCkill, fgHwTvdCKill());
            LOG(3, "[TVD_DBG_MSG] Pre-525=%d, Cur-525=%d\n", _rTvd3dStatus.fgIs525, fgIs525);
            LOG(3, "[TVD_DBG_MSG] Pre-443&Phalt=%d, Cur-443&Phalt=%d\n", _rTvd3dStatus.fgIs443nPHALT, fgIs443nPHALT);
        }
    }

#endif

    //For DDI interface
    if(fgHwTvdVPresTVD3D())
    {
        //LOG(1,"[TVD_DBG_MSG] vTvd3dVSyncISR, mcnt=%d, Vlock=%d\n", _sbTvdModeCnt, fgHwTvdVLock());
        _u4ChVPresOnCnt++;

        if(fgHwTvdVLock())
        {
            _u4ChLockCnt++;
        }
        else
        {
            _u4ChUnLockCnt++;
        }
    }
    else
    {
        _u4ChVPresOffCnt++;
        _u4ChLockCnt = _u4ChUnLockCnt = 0;
    }

    _rTvd3dStatus.fgIsBurstLock = fgHwTvdBLock();
    _rTvd3dStatus.fgIsCkill = fgHwTvdCKill();
    _rTvd3dStatus.bCAGC = bHwTvdCAGC();
    fgTrick = fgHwTvdTrick() && fgVPres && (_na_state != (UINT8)NA_SUPERHIGHNOISE);
    fgIsVCR = bTvdCtrl(TCTL_VCR,TC_GETEN,0)?bTvdCtrl(TCTL_VCR,TC_GETVAL,0):(fgHwTvdHeadSwitch()||fgTrick); //Use head switch status to detect VCR. Benson.06.01.09
    _rTvd3dStatus.fgIsPSync = fgHwTvdIsPSync();
    _rTvd3dStatus.fgIsCoChannel =  bTvdCtrl(TCTL_COCH,TC_GETEN,0)?bTvdCtrl(TCTL_COCH,TC_GETVAL,0):fgHwTvdCoChannel()&&(_na_state != (UINT8)NA_SUPERHIGHNOISE);
    _rTvd3dStatus.fgIs443nPHALT = (fgHwTvd443()<<1)|fgHwTvdPHALT();
#if TVD_FRS_FOR_NONSTDSIGNAL
    _rTvd3dStatus.LineInfo.wAvg_VLen = wHwTvdAvgVlen();
#endif
#if CAGC_PEAK_CNR_ADJ
    _rTvd3dStatus.fgCNRLow = fgHwTvdCNRLow();
#endif
    //big UP/DN_RATIO for small Againcode
    _rTvd3dStatus.bAgainCode = IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE);

#if TVD_ANA_COPY_PROTECT_FILTER
    _svDrvTvd3dAnaCopyProFilter();
#endif
#if ENABLE_PRBS_BY_DRIVER
    _svDrvTvdEnablePrbs();
#endif

#if TVD_VPRES4_IMPROVEMENT_FOR_WEAKSGINAL
    _svDrvTvdSetVPres4PIC();
#endif

    _svDrvTvdSetModeUnknown();

#if TVD_ADJUST_AGC_SPEED_FOR_LOW_SYNC
    _svDrvTvdAdjustAGCForLowSync();
#endif

#if TVD_ADJUST_Y4V_FOR_FIELD
    _svDrvTvdSetY4V();
#endif

#if 0//TVD_SERRCNT_WA
    _svDrvTvdAdjustSerrCntWA();
#endif

#if TVD_ADJSUT_PALSW_BY_VLOCK
    _svDrvTvdAdjustPALSW();
#endif

#if TVD_LIM_HERR_WA
    _svDrvTvdLimHerrWA();
#endif

#if TVD_ADAP_VPRES_SETTING
    _svDrvTvdAdapVpres();
#endif

#if TVD_LLOCK_STATUS
    _svDrvTvdChkLineStatus();
#endif

#if TVD_ABS_DIFF_STDFV
    _svDrvTvdChkAbsDiffStdFV();
#endif

#if TVD_NR_AVG
    vTvdMeasureNR(SV_ON);
#endif
    //-------------------------over charoma adjust  when nocolor->color-----start------------------------//
#if   TVD_AntiTrs
    _svDrvTvdAntiTrs(_rTvd3dStatus.bCAGC,&_sfgCagc,&_sbCAGC1)  ; //Adjust CAGC loop gain to shorten the transient time
#endif
    /*
    if((bCAGC1>bCAGC+15)&&(fgcagc==0))
    {
        fgcagc=1;
        _bTvdISRCnt=0;
        vIO32WriteFldAlign(CDET_0B, 0x05, LARGE_LOOP_GAIN);
        LOG(9, "\n bcagc1=%d,bcagc=%d \n",bCAGC1,bCAGC)	;
    }
    if((_bTvdISRCnt>=0x20)&&(fgcagc==1))
    {
        fgcagc=0;
        _bTvdISRCnt=0;
        LOG(9, "\n the changed bcagc:bCAGC=%d \n",bHwTvdCAGC());
        vIO32WriteFldAlign(CDET_0B, 0x02,  LARGE_LOOP_GAIN);
    }
    bCAGC1=bCAGC;
    */

    //-------------------------over charoma adjust when nocolor->color----end-------------------------//
    //-------------------------SFirst detect counter ----------------------------//
#if SUPPORT_S_DETECT
    _svDrvTvdSVPresDetCount();
#endif
    //-------------------------Adj 2d/3d line avg threshold ----------------------------//
    _svDrvTvdAdjLineThr();
    _na_state =bTvdCtrl(TCTL_NOISELVL,TC_GETEN,0)?bTvdCtrl(TCTL_NOISELVL,TC_GETVAL,0):bHwTvdNAState();

    //-------------------Check Vpress Changed status----------------------
    if(_rTvd3dStatus.fgVPres != fgVPres)
    {
        LOG(1,"[TVD_DBG_MSG] tvd vpres mode change %d->%d\n",_rTvd3dStatus.fgVPres,fgVPres);
        #if ENABLE_PRBS_BY_DRIVER
        LOG(1,"[TVD_DBG_MSG] _bEnablePrbsByAPMute is %d\n", _bEnablePrbsByAPMute);
        if(_bEnablePrbsByAPMute==TRUE) //channel change 		    
            _bEnablePrbs = TRUE;
        #endif
        #if TVD_BP_ATV_MODECHG
        if(_rTvd3dStatus.fgVPres)
 		    _sbTvd3dModePreVPresOn = _rTvd3dStatus.bTvdMode;
 		#endif

        _rTvd3dStatus.fgVPres = fgVPres;

        #if TVD_PALM50_SUPPORT
        if(!_rTvd3dStatus.fgVPres)
        {
            if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
                vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
            _su1PALM50DetState = PALM50_DET_IDEL;
        }
        #endif
        
#if TVD_PHALT_MN_WA2

        if(_fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
        {
            if((_rTvd3dStatus.bColSys == SV_CS_AUTO)&&(_rTvd3dStatus.fgVPres))
            {
                _sbPhalt_Vpres=1;
                LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 vpres _sbPhalt_Vpres=1\n");
            }
            else
            {
                _sbPhalt_Vpres=0;
                LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 vpres _sbPhalt_Vpres=0\n");
            }
        }

#endif
        #if TVD_FAST_CHCHG
        if((_rTvd3dStatus.eSourceType==SV_ST_TV))
        {
            vTvd3dFastChannelChange(SV_ON);
            vDrvTvdSetChanchgCnt(CHANCHG_START);
        }
        #endif

        #if TVD_BP_ATV_MODECHG
        if(_rTvd3dStatus.eSourceType == SV_ST_TV && _bATVChannelChange==FALSE)
        _sbBypassModeChg = TRUE;
        #endif

        #if TVD_DETECT_UNSTABLE_VCRBV
        if(_rTvd3dStatus.eSourceType == SV_ST_TV)
        {
            LOG(9, "[TVD_DBG_MSG] StableVCRBV\n");
            _sbUnsbableVCRBV = FALSE;
            _su2AvgAbsDiff = _su2Avg8FieldDiff = 0;
            _su2SumAbsDiff = _su2Sum8FieldDiff = VCRBV_AVG_COUNT =0;
        }
        #endif
        _svDrvTvdModeChg();

        #if TVD_BP_ATV_MODECHG
        //if(_rTvd3dStatus.eSourceType == SV_ST_TV)
        //    _sbBypassModeChg = TRUE;
        #endif

        //rollback for SECAM_40db_CKILL Performance by xiaokuan
        if(_rTvd3dStatus.fgVPres)
        {
            vIO32WriteFldMulti(CDET_03,\
                               P_Fld(CDET_SCF_NR_UP_VPON, SCF_NR_UP)|\
                               P_Fld(CDET_SCF_NR_DN_VPON, SCF_NR_DN));
#if !TVD_VPRES_IMPROVE
            vIO32WriteFldAlign(DFE_11, DFE_VPRES2_H_OFF_GAIN, VP2_H_OFF_OFFSET);
#endif
        }
        else
        {
            vIO32WriteFldMulti(CDET_03,\
                               P_Fld(CDET_SCF_NR_UP_VPOFF, SCF_NR_UP)|\
                               P_Fld(CDET_SCF_NR_DN_VPOFF, SCF_NR_DN));
#if !TVD_VPRES_IMPROVE
            vIO32WriteFldAlign(DFE_11, DFE_VPRES2_H_OFF_GAIN_IN_VOFF, VP2_H_OFF_OFFSET);	// To add hsytersis of VPRES.
#endif
        }
    }

#if TVD_PHALT_MN_WA2

    if(_fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
    {
        if((fgIs525 == 0)&&(fgHwTvd443() == 0)&&(fgHwTvdPHOLD() == 0)
           &&(_sbPhalt_Vpres==1)&&(bHwTvdNRLevel()>0x1E)&&(!fgHwTvdBLock()))
        {
            vIO32WriteFldAlign(CDET_00, SV_ON, TVD_MMODE);
            vIO32WriteFldAlign(CDET_00, SV_CS_PAL_N, TVD_MODE);
            _sbPhalt_Vpres=2;
            LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE PAL_N (mode 000)\n");
            //LOG(9, "NPTV TVD vpres mode000\n");
            //LOG(9, "NPTV TVD BLOCK = %d ,TVD_MMODE=%d ,_sbTvdModeCnt=%d\n",fgHwTvdBLock(),fgHwTvdIsMMode(),_sbTvdModeCnt);
            //LOG(9, "mode status=%d ,NR_level=%d \n",(IO32ReadFldAlign(STA_CDET_00, Fld(3, 24, AC_MSKB3))),bHwTvdNRLevel());
        }

        if(!_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE))
        {
            LOG(3,"[TVD_DBG_MSG] Is525=%d, Hw525=%d\n", fgIs525, fgHwTvd525());
        }

        if(fgDrvTvdModeCntGt(8) && (fgIs525 == 1)&&(fgHwTvd443() == 0)&&(fgHwTvdPHOLD() == 0)
           &&(_sbPhalt_Vpres==1)&&(bHwTvdNRLevel()>0x1E)&&(!fgHwTvdBLock()))
        {
            vIO32WriteFldAlign(CDET_00, SV_ON, TVD_MMODE);
            vIO32WriteFldAlign(CDET_00, SV_CS_PAL_M, TVD_MODE);
            _sbPhalt_Vpres=2;
            LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE PAL_M (status100)\n");
        }
    }

#endif
    //-------------------Adjust Again Up/Dn Th------------------------
    _svDrvTvdAgainUpDnThr(_rTvd3dStatus.bAgainCode);
    //-------------------Adjust Non Std FH ------------------------
#if TVD_8222_NONSTANDARD

    if(IS_FH_NON_STD())
    {
        vIO32WriteFldAlign(DFE_0F, SV_ON, VP2_IGN_HSYNC_P);//ignore VP2 hsync period check
    }
    else
    {
        vIO32WriteFldAlign(DFE_0F, SV_OFF, VP2_IGN_HSYNC_P);
    }

#endif
    //------------------- Macro Vision --------------------------------
    vHwTvdSetMAV(_rTvd3dStatus.fgIsPSync);
    //------------------- SCart----------------------------------------
#if SUPPORT_SCART
    _svDrvTvdScartSet(_bUIScartMode,&_bSFisSV,&_bTrigScartAuto,&_rTvd3dStatus);
#endif
    //----Setting VCR Mode when VPres On-----
    _svDrvTvdVcrSet(fgIsVCR, &bhwVCRSetting, &_rTvd3dStatus);
    /*************MT8202B Change Channel************/
    _svDrvTvdChanChg(&_rTvd3dStatus,_sbIsTVDFixAgc);
    /*******************************Noise Analasis*****************************/
    // Measure noise only when vlock and vpres is on continuously after _sna_HysterCnt Vsync.
    _rTvd3dStatus.NAInfo.na_HysterCnt = (_rTvd3dStatus.NAInfo.na_HysterCnt>_rTvd3dStatus.NAInfo.na_HysterCntThres)?_rTvd3dStatus.NAInfo.na_HysterCnt : (_rTvd3dStatus.NAInfo.na_HysterCnt+1) ;
    //-------------------Check Auto Search state---------------------------//
    _svDrvTvdGetNRAvg(&wTVDNRavg,&_swTVDNRavg1,&_rTvd3dStatus);
    //-----------------Trick Mode Detection----------------------//
    _svDrvTvdTrickModeDET(fgTrick, bhwVCRSetting, _na_state, &_rTvd3dStatus);
    //-----------------Secam Detect-------------------------/
    _svDrvTvdSecamDET(&_rTvd3dStatus, _sbTvd_McDone_cnt);
    //-----------------Co Channel Adjustment-------------------/
    _svDrvTvdCochAdj(_rTvd3dStatus.fgIsCoChannel,_swAGCTargetSyncHeight);
    //-----------------STD/NONSTD Line Related Seeting-----------/
    _svDrvTvdStdNStdLineSet(_na_state,&_rTvd3dStatus);
    //Check Line Count
    //-----------------Non STD FH related Setting--------------/
    _svDrvTvdNonStdFhSet();
    //-----------------AVG_SYNC_EN Enable/Disable Setting-----------/
#if TVD_AVG_SYNC_DET
    _svDrvTvdAvgSyncEnableSet(_na_state);
#endif
    //-----------------HLOCK Detection-----------------/
    _svDrvTvdHLockDET(&_fgHLOCK);
    //-----------------Old VCR Detection*(check STA_COMB_0C)---------/
#if TVD_OLD_VCR

    if((fgHwTvdVCR()) &&
       (_rTvd3dStatus.bTvdMode <= SV_CS_NTSC358) &&
       (fgHwTvdNProg()))
    {
        vDrvTvdIncOldVCRCnt();

        if(fgDrvTvdIsOldVCRCnt(TVD_OldVCR_CNT-1))
        {
            _sfgOldVCR = 1;
        }
    }
    else
    {
        vDrvTvdDecOldVCRCnt();

        if(fgDrvTvdIsOldVCRCnt(TVD_OldVCR_CNT2+1))
        {
            _sfgOldVCR = 0;
        }
    }

#endif
    //-----------------Check STD/NONSTD VLEN-----------------/
#if TVD_FRS_FOR_NONSTDSIGNAL

    if((_rTvd3dStatus.LineInfo.wAvg_VLen != 0) && (_rTvd3dStatus.LineInfo.wLinecnt !=0) &&(_svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE)))
    {
        vTvd3dChkNSTD(_rTvd3dStatus.LineInfo.wAvg_VLen, _rTvd3dStatus.LineInfo.wLinecnt, &_rTvd3dNSTDStatus);
    }

#endif
    //-----------------DClamp on/off condition check-----------------/
    _svDrvTvdDclampCHK(bhwVCRSetting,&_rTvd3dNSTDStatus,&_rTvd3dStatus);

    #if TVD_ADJUST_Y4H_FOR_FIELD
    //Sync Level 25% condition is better
    if(_na_state==NA_LOWNOISE && bHwTvdNRLevel()<0xA
    && ((TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1716)<3)||(TVDAbsDiff(_rTvd3dNSTDStatus.wHTotal,1728)<3))
    && ((TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,525)<10) || (TVDAbsDiff(_rTvd3dNSTDStatus.wVTotal,625)<10))
    && !fgHwTvdFHPos() && !fgHwTvdFHNeg()
    /*&& !fgHwTvdHeadSwitch() && !fgHwTvdTrick() && (bhwVCRSetting==(UINT8)VCR_NONE) */
    && !fgHwTvdCoChannel() && !fgHwTvdVCRBV() && !fgHwTvdCNRLow()
    && IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE)>0x40
    )
    {
        vIO32WriteFldAlign(DFE_02, TVD_DCLAMP_Y4H, Y4H_BW);
    }
    else
    {
        if(_rTvd3dStatus.eSourceType==SV_ST_TV && _svDrvTvdCheckSignalStatus((UINT8)SV_VDO_STABLE) && _rTvd3dStatus.bTvdMode==SV_CS_PAL_M
        && bHwTvdCAGC()>0xE && fgHwTvdCNRLow() && IO32ReadFldAlign(STA_CDET_01,DCNT)>0x60 && _svDrvTvdGetNRLevel()<55)
        {
            vIO32WriteFldAlign(DFE_02, TVD_FT_Y4H, Y4H_BW); 
        }
        else
        {
            vIO32WriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW);
        }
    }
    #endif
    
    //-----------------Check Blank variation large-----------------/
#if TVD_COCH_FLICKER
    bIsBlankVarLarge = ((fgHwTvdBlkLargeVar()&&_rTvd3dStatus.fgIsCoChannel)?SV_TRUE:SV_FALSE); //?? no used curretly
#endif
    //-----------------Force Fast VALIGN-----------------/
#if TVD_FORCE_FAST_VALIGN
    _svDrvTvdFastValign();
#endif
    //-----------------CO IIR Check-----------------/
    _svDrvTvdCOIIRCHK(&_rTvd3dStatus, _sbAvgCOCH_detect);
    //-----------------NA State Check----------------------/
    _svDrvTvdNAStateCHK(_na_state,&_rTvd3dStatus);

    //-----------------Serr Cnt Check----------------------/
    if(fgHwTvdFieldDamp())  //Signal Is no good and field detect is toggling
    {
        _sbTVDSerrCnt = (_sbTVDSerrCnt == TG_SERR_CNT_START+0x12) ? TG_SERR_CNT_START: _sbTVDSerrCnt + 1;
        vIO32WriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
    }

    //-------------------VCR_BV setting------------------------------------------------------
#if TVD_VCR_BV
    if(_sbManualLLockGainbyDDI==FALSE)
    _svDrvTvdVcrBVSet(_swLLStatus_avg,_na_state,_sbTvd_McDone_cnt,&_rTvd3dStatus);
#endif
    _svDrvTvdHSlicer(_na_state,&_rTvd3dStatus);
    //-----------------TVD H Fine Tune----------------------/
    if(_sbManualLLockGainbyDDI==FALSE)
    _svDrvTvdHFineTune(_na_state, &_rTvd3dStatus);

    //-----------------Mode Change Counter--------------------/
    //Start mode detection only when Tvd doesn't hold the mode.
    if((_bTvdHoldMode==SV_TRUE) && (_rTvd3dStatus.eSourceType==SV_ST_TV))
    {
        if(!fgDrvTvdModeCntGt(_sbTvd_McDone_cnt))
        {
            vTvd3dTrigModeDet();
        }
    }

    vDrvTvdIncModeCnt();
#if 0
    vTvd3dCheckCombNormalMode();
#endif
    //-----------------AVG CoChannel Counter for Serrtype Switch--------------------/
#if TVD_SERRTYPE_ONE

    if(fgVPres)
    {
        if((!fgDrvTvdModeCntGt(_sbTvd_McDone_cnt))&&(fgDrvTvdModeCntGt(_sbTvd_McDone_cnt-AVGCOIIR_CNT)))
        {
            _swCochIir_Avg+=IO32ReadFldAlign(DFE_STA_08, COCH_IIR);
            //LOG(9, "\n COCH iir=0x%x\n",IO32ReadFldAlign(DFE_STA_08, COCH_IIR));
        }
    }
    if(_u1IIRLLockEn!=IO32ReadFldAlign(TG_09, IIR_LLOCK_EN))
    {
        _u1IIRLLockEn=IO32ReadFldAlign(TG_09, IIR_LLOCK_EN);
        #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
        _svTvd3dSetGameModeAVDelay(_bCombGameMode);
        #else
        _svDrvTvdSetAVDelay(bTvdMode);
        #endif
    }

#endif
    //-----------------Mode Switch Portection--------------------/
#if TVD_MODESWITCH_PROTECT
    _svDrvTvdModeSwPrtec(fgVPres,bTvdMode,_sbTvd_McDone_cnt);
#endif
    //-----------------Fixed gain issue--------------------/
#if TVD_USE_NEW_DAGC
#if TVD_DAGC_WA
    _svDrvTvdFixedGainFineTune(_sbTvd_McDone_cnt, _sbIsTVDFixAgc, _na_state);
#else
    _svDrvTvdFixedGainFineTune(_sbTvd_McDone_cnt, _sbIsTVDFixAgc);
#endif
#endif
    //-----------------Mode Change Detection--------------------/
    //-----------------phalt wa in PAL-N----------------------/
#if TVD_PHALT_WA
#if TVD_SET_ENABLED_CS

    if(_bMModeForEnabledCS==FALSE && _fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
#else
    if(_fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
#endif
    {
        if(fgDrvTvdModeCntGt(50))
        {
            if ((_na_state == (UINT8)NA_SUPERHIGHNOISE)||(_na_state == (UINT8)NA_HIGHNOISE)||(_svDrvTvdGetNRLevel()<50))
            {
                if((_rTvd3dStatus.bColSys == SV_CS_AUTO)&&(!fgHwTvdBLock4DET()))
                {
                    //PAL N
                    if(((fgIs525 == 0)&&(fgHwTvd443() == 0)&&(fgHwTvdPHOLD() == 0))
                       &&(_rTvd3dStatus.bTvdMode == SV_CS_PAL_N)
                      )
                    {
                        if(_rTvd3dStatus.bTvdMode != bTvdMode)
                        {
#if TVD_PHALT_MN_WA2
                            _sbForcePalMN = TRUE;
#endif
                            vIO32WriteFldAlign(CDET_00, SV_ON, TVD_MMODE);
                            //LOG(9, "\n do not run here ................\n");
                            //LOG(9, "\n do not run here ................%d \n",_na_state);
                            vIO32WriteFldAlign(CDET_00, SV_CS_PAL_N, TVD_MODE);
                            bTvdMode=SV_CS_PAL_N;
                            LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE PAL_N (SHN,HN)\n");
                        }
                    }
                    else
                    {
#if TVD_PHALT_MN_WA2

                        if(!_sbForcePalMN)
                        {
#endif
                            //vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                            //LOG(9,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE off \n");
#if TVD_PHALT_MN_WA2
                        }

#endif
                    }

                    //PAL M
                    if(((fgIs525 == 1)&&(fgHwTvd443() == 0)&&(fgHwTvdPHOLD() == 0))
                       &&(_rTvd3dStatus.bTvdMode == SV_CS_PAL_M)
                      )
                    {
                        if(_rTvd3dStatus.bTvdMode != bTvdMode)
                        {
#if TVD_PHALT_MN_WA2
                            _sbForcePalMN = TRUE;
#endif
                            vIO32WriteFldAlign(CDET_00, SV_ON, TVD_MMODE);
                            //LOG(9, "\n do not run here ................\n");
                            //LOG(9, "\n do not run here ................%d \n",_na_state);
                            vIO32WriteFldAlign(CDET_00, SV_CS_PAL_M, TVD_MODE);
                            bTvdMode=SV_CS_PAL_M;
                            LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE PAL_M (SHN,HN)\n");
                        }
                    }
                    else
                    {
#if TVD_PHALT_MN_WA2

                        if(!_sbForcePalMN)
                        {
#endif
                            //vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                            //LOG(9,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE off \n");
#if TVD_PHALT_MN_WA2
                        }

#endif
                    }
                }
            }
            else
            {
                if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
                {
#if TVD_PHALT_MN_WA2
                    _sbForcePalMN = FALSE;
#endif
                    vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                    LOG(9,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 TVD_MMODE off (Low Noise) \n");
                }
            }
        }
    }

#endif
    _svDrvTvdModeChgDet(fgIs525, bTvdMode, &_rTvd3dStatus);

    //-----------------Force Update Mode-----------------/
    if(bTvdCtrl(TCTL_MDSTBPROT,TC_GETEN,0)&&fgDrvTvdIsModeCnt(_sbTvd_McDone_cnt-2))
    {
        vIO32WriteFldAlign(CDET_05, TVD_FORCE_UPD_MODE?SV_ON:SV_OFF, FORCE_UPD_MODE);
    }

    //-----------------Mode Change Done-----------------/
#if TVD_PALM50_SUPPORT
    _svDrvTvdPalM50DetectProc();
#endif

    
#if TVD_PHALT_MN_WA2

    if(_fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
    {
        if(_sbPhalt_Vpres==2)
        {
            if(fgDrvTvdModeCntGt(_sbTvd_McDone_cnt+1))
            {
                if(!fgDrvTvdModeCntGt(100))
                {
                    if(fgHwTvdBLock())
                    {
                        _sbPhalt_Vpres=0;
                        _svDrvTvdModeChgDone();
                        LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 ModeChgDone BurstLock 1\n");
                    }
                }
                else if(fgDrvTvdIsModeCnt(100))
                {
                    if(fgHwTvdBLock())
                    {
                        _sbPhalt_Vpres=0;
                        _svDrvTvdModeChgDone();
                        LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 ModeChgDone BurstLock 2\n");
                    }
                    else
                    {
                        _sbPhalt_Vpres=0;

                        if(!_sbForcePalMN)
                        {
                            LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 ModeChg BurstUNLock\n");
                            vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE);
                            _svDrvTvdModeChg();
                        }
                        else
                        {
                            LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 ModeChgDone BurstUNLock-keep MMODE\n");
                            _rTvd3dStatus.bTvdMode=bTvdMode;
                            _svDrvTvdModeChgDone();
                            //LOG(9, "\n do not run here4 ................\n");
                            //LOG(9, "NPTV TVD BLOCK = %d ,TVD_MMODE=%d ,_sbTvdModeCnt=%d\n",fgHwTvdBLock(),fgHwTvdIsMMode(),_sbTvdModeCnt);
                            //LOG(9, "mode status=%d ,NR_level=%d \n",(IO32ReadFldAlign(STA_CDET_00, Fld(3, 24, AC_MSKB3))),bHwTvdNRLevel());
                        }
                    }
                }
            }
            else
            {
                if(fgHwTvdBLock())
                {
                    _sbPhalt_Vpres=0;
                }
            }
        }
    }

#endif

#if TVD_BP_ATV_MODECHG
    if(_rTvd3dStatus.eSourceType == SV_ST_TV && fgDrvTvdIsModeCnt(_sbTvd_McDone_cnt-2) && _sbBypassModeChg==TRUE)
    {
        _sbBypassModeChg = FALSE;
        if(_rTvd3dStatus.fgVPres && _sbTvd3dModePreVPresOn!=_rTvd3dStatus.bTvdMode)
        {
            LOG(1, "[TVD_DBG_MSG] TVD_BP_ATV_MODECHG ModeChg %d -> %d\n", _sbTvd3dModePreVPresOn, _rTvd3dStatus.bTvdMode);
            _svDrvTvdModeChg();
        }
    }
#endif

#if TVD_CTRL_STABLE_MCNT
    if(fgDrvTvdIsModeCnt(_sbTvd_Reduced_McDone_cnt))
#else
    if(fgDrvTvdIsModeCnt(_sbTvd_McDone_cnt))
#endif    
    {
#if TVD_SERRTYPE_ONE

        if(fgVPres)
        {
            _swCochIir_Avg=_swCochIir_Avg/AVGCOIIR_CNT;
            _sbAvgCOCH_detect=(_swCochIir_Avg>0x20)?1:0;
        }

#endif
        //if(bTvdCtrl(TCTL_MDSTBPROT,TC_GETEN,0)&&!fgHwTvdIsMMode()&&_rTvd3dStatus.fgIsBurstLock
        //&&(_rTvd3dStatus.bTvdMode!=_sbTvdModeMapTable[(UINT8)_rTvd3dStatus.fgIs443nPHALT |(UINT8)(fgIs525) << 2]))
        //{
        //      _svDrvTvdModeChg();
        //}
        //else
        //{
#if TVD_PHALT_MN_WA2

        if(_fgEnablePhaltWa && (_rTvd3dStatus.eSourceType==SV_ST_TV))
        {
            if(_sbPhalt_Vpres!=2)
            {
                _sbPhalt_Vpres=0;
                _svDrvTvdModeChgDone();
                LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 ModeChgDone 3\n");
            }
        }
        else
        {
            _svDrvTvdModeChgDone();
        }

#else
        _svDrvTvdModeChgDone();
#endif
        //}
    }

#if PDP_PANEL_NSTD_CHECK_WA
    //For PDP sync 0->100%, because PDP can not apply NSTD check in mainloop, need to reset srcRegion again
    if(IS_PANEL_PDP && _rTvd3dStatus.eSourceType == SV_ST_TV && _sbPDPModeChgDoneAgain==TRUE)
    {
        LOG(3, "TVD ISR - wait for STD signal\n");
        if(!_rTvd3dNSTDStatus.fgIsNSTD)
        {
            _sbPDPModeChgDoneAgain = FALSE;        
            if (fgIsMainTvd3d())
            {
                bVideoUpdateSrcRegion(SV_VP_MAIN);
            }
            if (fgIsPipTvd3d())
            {
                bVideoUpdateSrcRegion(SV_VP_PIP);
            }          
        }
    }
#endif

#if CAGC_PEAK_CNR_ADJ
    _svDrvTvdAdjustCAGCPeak(_rTvd3dStatus.fgCNRLow ,_rTvd3dStatus.fgTrick, _rTvd3dStatus.bTvdMode);
#endif

    //TVD CLI Nfy CallBack
    if(_pfnTVDCLINotify)
    {
        _pfnTVDCLINotify(&_rTvd3dStatus);
    }
}


/**
 * @brief TVD Connect/Dis-connect function
 *
 * To connect/dis-connect TVDecoder when input source is changed. Reset necessary registers
 * and firmware variables.
 *
 * @param bPath bPath indicates Main or PIP path currently TVD is connect to or disconnect from
 * @param bOnOff bOnOff indicates Connect or Disconnect
 * @return None
 */
void vTvd3dConnect(UINT8 bPath, UINT8 bOnOff)
{
    LOG(1,"======================================\n");
    LOG(1,"[TVD_DBG_MSG] vTvd3dConnect bPath=%d, bOnOff=%d \n", bPath, bOnOff);
    LOG(1,"======================================\n");	

    if(bGetSignalType(bPath)==SV_ST_TV)
    {
        vTvd3dFastChannelChange(SV_ON);
    }
#if TVD_ADAP_VPRES_SETTING
    if(bPath==SV_VP_MAIN && bGetSignalType(bPath)==SV_ST_TV && _sbTvdConnected==FALSE)
    {
        _sbTvdConnected = TRUE;
        _sbAdapVpresStart = TRUE;
        _su2AdapVpresCounter = 0;
    }
#endif

#ifdef CC_SUPPORT_RECORD_AV
	_sbATVPVRBypassModeChg = FALSE;
	_sbIssueModeChgDoneMain = FALSE;

    if(bPath==SV_VP_MAIN)
    {
        _rTvd3dStat.bIsMain = bOnOff;
        _sbConnectedByMain = bOnOff;
        if(bOnOff==SV_ON)
        {
            _su1MainSourceType = bGetSignalType(bPath);
        }
        else
        {
            _su1MainSourceType = SV_ST_MAX;
        }
    }
    if(bPath==SV_VP_PIP)
    {
        _rTvd3dStat.bIsPip = bOnOff;
        _sbConnectedBySub = bOnOff;    
        if(bOnOff==SV_ON)
        {
            _su1SubSourceType = bGetSignalType(bPath);
        }
        else
        {
            _su1SubSourceType = SV_ST_MAX;
        }
    }

    if(bOnOff==SV_ON)
    {
        if(bPath==SV_VP_MAIN && _sbConnectedBySub && _sbTvdModeCnt>_sbTvd_McDone_cnt && _su1MainSourceType==_su1SubSourceType)
        {
            _sbATVPVRBypassModeChg= TRUE;
            _sbIssueModeChgDoneMain = TRUE;
            vSetMainFlg(MAIN_FLG_MODE_CHG);
            vClrMainFlg(MAIN_FLG_MODE_DET_DONE);	// Clear this flag!
            LOG(1,"======================================\n");
            LOG(1,"[TVD_DBG_MSG] vTvd3dConnect Sub is connected before Main \n");
            LOG(1,"======================================\n");
            return;
        }
        if(bPath==SV_VP_PIP && _sbConnectedByMain && _sbTvdModeCnt>_sbTvd_McDone_cnt && _su1MainSourceType==_su1SubSourceType)
        {
            _sbATVPVRBypassModeChg= TRUE;
            _sbIssueModeChgDoneSub = TRUE;
            vSetPipFlg(PIP_FLG_MODE_CHG);
            vClrPipFlg(PIP_FLG_MODE_DET_DONE);	// Clear this flag!
            LOG(1,"======================================\n");
            LOG(1,"[TVD_DBG_MSG] vTvd3dConnect Main is connected before Sub \n");
            LOG(1,"======================================\n");     
            return;
        }
    }

    LOG(1,"======================================\n");
    LOG(1,"[TVD_DBG_MSG] vTvd3dConnect _sbConnectedByMain=%d _sbConnectedBySub=%d \n", _sbConnectedByMain, _sbConnectedBySub);
    LOG(1,"======================================\n");


    if((bOnOff==SV_OFF) && (_sbConnectedByMain || _sbConnectedBySub)) //dont disconnect if Main or Sub is still connected.
    {
        LOG(1,"======================================\n");
        LOG(1,"[TVD_DBG_MSG] vTvd3dConnect Main/Sub is connected -> dont disconnect \n");
        LOG(1,"======================================\n");
            
        return;
    }
#endif


    _u4ChVPresOnCnt = _u4ChVPresOffCnt = _u4ChLockCnt = _u4ChUnLockCnt = 0;
    wTVDNRavg = 0;
    _rTvd3dStatus.fgIsVCR = 0;
    bhwVCRSetting = 0x0;
#if TVD_FRS_FOR_NONSTDSIGNAL
    _wPreAvg_VLen = 0xff;
#endif
#if TVD_MAX_CLAMP_TABLE
    vIO32WriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
#endif
    vDrvTvdinitVP1Cnt();
    //Init Mode Deglish status.
#if TVD_MODE_Deglitch
    _svDrvTvdInitModeDeglish();
#endif

    //Init _rTvd3dStatus variable
    if(bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)==1)
    {
        _rTvd3dStatus.bTvdMode = 0x3;
    }
    else
    {
        _rTvd3dStatus.bTvdMode = 0xff;
    }

    _rTvd3dStatus.fgVPres = FALSE;
    _rTvd3dStatus.fgTrick = FALSE;
    _rTvd3dStatus.fgSVPres = 0xFF; // To force notify mw SV no signal. Benson.
    _rTvd3dStatus.fgIsMV = FALSE;

    if(bTvdCtrl(TCTL_AGCPEDEN,TC_GETVAL,0)==1)
    {
        _bAgcPedEn = 1;
    }
    else
    {
        _bAgcPedEn = 0;
    }

    _rTvd3dStatus.eSourceType = bTvdCtrl(TCTL_INPUTTYPE,TC_GETEN,0)?
                                bTvdCtrl(TCTL_INPUTTYPE,TC_GETVAL,0):bGetSignalType(bPath);
#if TVD_FRS_FOR_NONSTDSIGNAL
    _svDrvTvdRstNSTStatus(_rTvd3dStatus.fgIs525, &_rTvd3dNSTDStatus);
#endif
    //DFE initial
    vIO32WriteFldMulti(DFE_02,
                       P_Fld(SV_ON, BLV_LIM_AUTO)|
                       P_Fld(SV_OFF, BLV_LIM_EN));
    vIO32WriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW);
    vIO32WriteFldMulti(DFE_08,
                       P_Fld(SV_OFF, DCLAMP_Y_EN)|
                       P_Fld(SV_ON, DCLAMP_2LINE_AVG)|
                       P_Fld(DFE_DCLAMP_LOW_COR, DCLAMP_COR));
    vIO32WriteFldAlign(DFE_0E, SV_OFF, DCLAMP_CHECK_LIM);//new reg for 8223
    vDrvSetTVDADCGain(DFE_AGAIN_DFT);
    vDrvTvd3dVPresOnOff(0x12, DFE_NORMAL_ONOFF_THRD);
    vIO32WriteFldAlign(DFE_02, 1, POS_DIFF_ONLY);    //Consider invalid blank if blank region is MacroVision Like.
#if TVD_VPRES_IMPROVE
    vIO32WriteFldMulti(DFE_11,\
                       P_Fld(DFE_VPRES2_H_ON_GAIN, VP2_H_ON_OFFSET)|\
                       P_Fld(DFE_VPRES2_H_OFF_GAIN,VP2_H_OFF_OFFSET)|\
                       P_Fld(DFE_VPRES2_V_ON_GAIN, VP2_V_ON_OFFSET)|\
                       P_Fld(DFE_VPRES2_V_OFF_GAIN,VP2_V_OFF_OFFSET));
#else
    vIO32WriteFldMulti(DFE_11,\
                       P_Fld(DFE_VPRES2_H_ON_GAIN, VP2_H_ON_OFFSET)|\
                       P_Fld(DFE_VPRES2_H_OFF_GAIN,VP2_H_OFF_OFFSET)|\
                       P_Fld(DFE_VPRES2_V_ON_GAIN, VP2_V_ON_OFFSET)|\
                       P_Fld(DFE_VPRES2_V_OFF_GAIN,VP2_V_OFF_OFFSET));
#endif
#if TVD_8222_NONSTANDARD
    vIO32WriteFldAlign(DFE_0F, SV_OFF, VP2_IGN_HSYNC_P);
#endif
    vIO32WriteFldAlign(DFE_04, DFE_PEAKYC+(_wTargetBlk-DFE_BLANK_TARGET), AGC2_PYC_LIM);
    #if TVD_ADJUST_AGC_SPEED_FOR_LOW_SYNC
    _su1BYCClipCnt=0;
    #endif
	vIO32WriteFldAlign(DFE_04, DFE_BYC_LIM, AGC2_BYC_LIM);
    vIO32WriteFldAlign(DFE_05, DFE_PEAK_TC, AGC2_PEAK_TC);
    vIO32WriteFldAlign(DFE_18, SV_OFF, VMAXMIN_EN);
    vIO32WriteFldAlign(DFE_18, 5, VMAXMIN_STASEL); // Use Diffof Min level as status output.
    _svDrvTvdLoadFixGain();

    if(_sbIsTVDFixAgc == SV_ON)
    {
#if TVD_USE_NEW_DAGC
        vTvdSetAGCMode(E_MENU_AGAIN);
        vIO32WriteFldAlign(DFE_03, SV_ON, MANUAL_DAGC_EN);
        vIO32WriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL, FIXAGC_DAGCSEL);
#if !TVD_DAGC_WA
        vIO32WriteFldAlign(SVF_00, SV_ON, TVDCTRL_MDAGC_EN);
#endif
#else
        vTvdSetAGCMode(E_MENU_GAIN);
        vIO32WriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);//MT5387 FPGA
        vIO32WriteFldAlign(DFE_18, 0, FIXAGC_DAGCSEL);
#endif
    }
    else
    {
        vTvdSetAGCMode(E_AUTO_GAIN);
        vIO32WriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);
        vIO32WriteFldAlign(DFE_18, DFE_AUTOAGC_DAGCSEL, FIXAGC_DAGCSEL);

        if(_rTvd3dStatus.eSourceType==SV_ST_TV && IO32ReadFldAlign(VSRC_07, ATD_ENABLE))
        {
            vIO32WriteFldAlign(DFE_07, DFE_ATD_MANUAL_DCODE, AGC2_MANUAL_DCODE);//for ATD snow noise is too dark
        }
        else
        {
            vIO32WriteFldAlign(DFE_07, DFE_DFT_MANUAL_DCODE, AGC2_MANUAL_DCODE);
        }
    }

#if TVD_8222_NONSTANDARD
    vIO32WriteFldAlign(TG_06, SV_ON, FDLY_SEL); //for MDET_V525_SEL Fh+-2000Hz
#else
    vIO32WriteFldAlign(TG_06, 0x0, FDLY_SEL);
#endif
    vIO32WriteFldAlign(TG_06, TG_SERR_CNT, SERR_CNT);
    _sbTVDSerrCnt = TG_SERR_CNT_START = TG_SERR_CNT;
    #if TVD_SERRCNT_WA
    //_u1NTSCVPorch = 0x1;
    #endif
    //CDET initial
#if TVD_8222_NONSTANDARD
    //MT5387 FPGA
    //MY & YJ recommend default off, after finefine, then implement new threshold into hw
    vTvd3dSetCAGCADAP(SV_OFF);
    vIO32WriteFldAlign(CDET_00, SV_ON, PALSW_FAST_ADAP);
    vIO32WriteFldAlign(CDET_05, SV_OFF, PALSW_STEP_F);
#endif
    vIO32WriteFldAlign(CDET_00, SV_OFF, DET443_SEL);
    vIO32WriteFldAlign(CDET_00, SV_ON, CTG_ADAP);
#if !TVD_8222_NONSTANDARD

    if(_sbIsSetCAGCbyAP != SV_ON)
    {
        vIO32WriteFldAlign(CDET_08, CAGC_LARGE_STEP, CAGC_L_STEP);
    }

#endif
    vIO32WriteFldAlign(CDET_08, 0x00, CDET_PAL_MODE); //roll back to old PAL SECAM mode detect state machin
    vDrvTvdRstCOCH2ENCnt();
    vDrvTvdRstCOCHDeCnt();
    vDrvTvdRstHsyncPosDetCnt();
    /*FPGA script*/
    //TG Init
    vIO32WriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
    vIO32Write4B(TG_13, TG_KIKP_GAIN);
    //Disable LF_OFFSET
    vIO32WriteFldAlign(TG_04, SV_OFF, LF_OFFSET_EN);
    vIO32WriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
    //CTG Init
    //MT5387 FPGA
    //	vIO32WriteFldAlign(TG_08, 0x0, TGEN_DEBUG);//MT5387 FPGA
    vIO32WriteFldMulti(CTG_00,\
                       P_Fld(SV_OFF,FRUN_MSEED_EN)|\
                       P_Fld(1,CTG_SWLBF)|\
                       P_Fld(1,CTG_DEMOD_LPFSEL)|\
                       P_Fld(SV_ON,CTG_PHASE_ERR_SEL)|\
                       P_Fld(SV_OFF,BSYNC_AUTO)|\
                       P_Fld(CTG_CHROMA_PG,CHROMA_PG));
    _svDrvTvdSetDetV525V625Gain(0x3, 0x3);
    _svDrvTvdSetDET443Gain(0, 0);
    if(COUNTRY_US==GetTargetCountry() && _rTvd3dStatus.eSourceType==SV_ST_TV 
    && IO32ReadFldAlign(CDET_00, PALM_EN) && IO32ReadFldAlign(CDET_00, PALN_EN))
    {
        _svDrvTvdSetPALTGain(5, 2);
    }
    else
    {
        _svDrvTvdSetPALTGain(5, 3);
    }
#ifdef NEW_COUNTRY_TYPE

    if((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_ATV_US))
#else
    if((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_US))
#endif
    {
        vIO32WriteFldMulti(CDET_00,
                           P_Fld(SV_OFF,NTSC443_EN)|
                           P_Fld(SV_OFF,PAL60_EN));
    }
    else
    {
        vIO32WriteFldMulti(CDET_00,
                           P_Fld(SV_ON,NTSC443_EN)|
                           P_Fld(SV_ON,PAL60_EN));
    }

	
#if TVD_MANUAL_TUNER_SCAN
	if (!_sfgManualScanadj)
	{
#ifdef NEW_COUNTRY_TYPE
		if ((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_ATV_EU))
#else
		if ((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU))
#endif
		{
			if(_fgDescramblerOn) _su1VpresGain= 0x12;
			else _su1VpresGain = 0x14;
		}
		else
		{
			if(_fgDescramblerOn) _su1VpresGain= 0x3C;
			else _su1VpresGain = 0x3C;
		}
	}
#endif

#if !TVD_SECAM_DET

    if(!(_rTvd3dStatus.eSourceType==SV_ST_TV))
    {
        _svDrvTvdSetSCMBurstWin(0x30, 0xb0);
    }
    else
#endif
    {
        _svDrvTvdSetSCMBurstWin(0x40, 0x80);
    }

    //CTG initial
#if TVD_CLAMP_ON_BLANK
    vIO32WriteFldAlign(CTG_07, SV_ON, FIXBLV);    // Set Fix Blv. Target blank level will be set at adcset_int.c
#endif
#if SUPPORT_SCART

    if(!(_bMainICIn==(UINT8)P_FB0 || _bMainICIn==(UINT8)P_FB1
#if SUPPORT_POP
         || _bSubICIn==(UINT8)P_FB0 || _bSubICIn==(UINT8)P_FB1
#endif
        )
      )
#endif
    {
        //vTvd3dSetYCDelay(0);
#if SUPPORT_SCART
        _sbScart_SV = 0;
#endif
    }

    //TG initial
    vIO32WriteFldAlign(TG_00, TG_MIN_HWIDTH, MIN_HWIDTH);    //Set min H sync width to 0x7 for Braizal Field signal and HWIDTH Relock.
    vIO32WriteFldAlign(TG_02, TG_MASK_STOP_525, MASK_STOP);
#if TVD_FORCE_FAST_VALIGN
    vIO32WriteFldMulti(TG_08,P_Fld(SV_OFF,FAST_VALIGN)|P_Fld(0x1, VALIGN_SPEED));
#else
    vIO32WriteFldAlign(TG_08, 0x2, VALIGN_SPEED);
#endif
    vIO32WriteFldAlign(TG_17, TG_FRAME_STDFV_TH_STD, FRAME_STDFV_TH);

    if(_rTvd3dStatus.eSourceType==SV_ST_TV)    // hsync de-glitch
    {
        vIO32WriteFldAlign(TG_0D, TG_UNVLD_HSYNC_WIDTH_TV, UNVLD_HSYNC_WIDTH);
        vIO32WriteFldAlign(DFE_0F, SV_ON, VP2_CHK_VSYNC);
        _rTvd3dStatus.NAInfo.na_HysterCntThres= 0x0A;
    }
    else
    {
        vIO32WriteFldAlign(TG_0D, TG_UNVLD_HSYNC_WIDTH_SAV, UNVLD_HSYNC_WIDTH);
        vIO32WriteFldAlign(DFE_0F, SV_OFF, VP2_CHK_VSYNC);
        _rTvd3dStatus.NAInfo.na_HysterCntThres = 0x40;
    }

    //other
    vIO32WriteFldAlign(VSRC_08, SV_ON, VPRES_COCH_EN);
#ifdef TVD_RTM1_CM
    vIO32WriteFldAlign(VSRC_08, 0xA, COCH_VLD_TH);
#endif
    /*if (bTvdCtrl(TCTL_MCNT,TC_GETEN,0) == 0)
    {
    if (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_ATV_EU)
    {
    	if (_rTvd3dStatus.eSourceType==SV_ST_TV)
    	{
    		_sbTvd_McDone_cnt=15;
    	}
    	else
    	{
    		_sbTvd_McDone_cnt=25;
    	}
    }
    else
    {
        _sbTvd_McDone_cnt=12; //increase change channel time
    }
    }
    else
    {
    	_sbTvd_McDone_cnt = bTvdCtrl(TCTL_MCNT, TC_GETVAL, 0);
    }*/
    vTvdSetOverSampleEnable(_bTVDOverSampleEn);

    if(bTvdCtrl(TCTL_MDSTBPROT,TC_APPLY,0))//MT5387 FPGA
    {
        vIO32WriteFldAlign(CDET_05, SV_ON, FORCE_UPD_MODE);
    }

    // Reset Related Counters
    vTvd3dTrigModeDet();
#if TVD_SECAM_DET
    vDrvTvdRstBurstCnt();
#endif
#if TVD_VCR_BV_STA
    vDrvTvdRstVCRBVStaCnt();
#endif
#ifdef  SUPPORT_AV_COMP_AUTO
    _u1AVCompStatus = 0xff ;
#endif

#if TVD_ANA_COPY_PROTECT_FILTER
    vDrvTvd3dAnaCopyProInit();
#endif
    // Connect
    if(bOnOff == SV_ON)
    {
        if(bPath == SV_VP_MAIN)
        {
            _rTvd3dStat.bIsMain = TRUE;
            // Trun on Interrupt
            vVDOINIrqOn(MSK_MAIN_DET);
        }
        else
        {
            _rTvd3dStat.bIsPip = TRUE;
            // Trun on Interrupt
            vVDOINIrqOn(MSK_PIP_DET);
        }

        if(bTvdCtrl(TCTL_MCNT,TC_GETEN,0) == 0)
        {
            _sbTvd_McDone_cnt=12;
        }
        else
        {
            _sbTvd_McDone_cnt = bTvdCtrl(TCTL_MCNT, TC_GETVAL, 0);
        }

#if	TVD_AntiTrs
        _sbCAGC1=0;                                             //INIT THE VALUE
        _sfgCagc=0  ;   		             //INIT THE VALUE
#endif
        vDrvTvdSetChanchgCnt(CHANCHG_START);
        vTvd3dReset();
    }
    else
    {
        if(bPath == SV_VP_MAIN)
        {
            // Reset VCR mode for DI
            vDrvDITrickModeOnOff(SV_VP_MAIN, SV_OFF);
            _rTvd3dStat.bIsMain = FALSE;
            vVDOINIrqOff(MSK_MAIN_DET);
        }
        else
        {
            // Reset VCR mode for DI
            vDrvDITrickModeOnOff(SV_VP_PIP, SV_OFF);
            _rTvd3dStat.bIsPip = FALSE;
            vVDOINIrqOff(MSK_PIP_DET);
        }

#ifdef CC_SOURCE_AUTO_DETECT
        vIO32WriteFldAlign(VSRC_07, SV_OFF, ATD_ENABLE); //disable ATD path for TVD
#endif
        /* 3D Comb */
        vDrvTDCActive(SV_OFF);
        vDrvTDCOnOff(SV_OFF);
        _rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_NOSIGNAL;
        _rTvd3dStatus.bTvdMode = SV_CS_NONE;
        vDrvTvdSetChanchgCnt(CHANCHG_STOP);
    }
    /* Snow Mode Setting default Value */
	_svSnowModeSet(SV_OFF, 0x10);
}


/**
 * @brief The refresh rate of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the refresh rate of current input signal of TVDecoder.
 * The refresh rate of possible TVDecoder input are 50 or 60, depend on
 * the mode of input signal.
 *
 * @param None
 * @return the refresh rate of current video signal
 *
 */
UINT8 bTvd3dRefreshRate(void)
{
    return ((_rTvd3dStatus.fgIs525)?60:50);
}

/**
 * @brief The Signal Status of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the Signal Status of current input signal of TVDecoder.
 *
 * @param None
 * @return the Signal Status of current video signal, possible return values include
 * SV_VDO_NOSIGNAL, SV_VDO_NOSUPPORT, SV_VDO_UNKNOWN and SV_VDO_NOSTABLE.
 *
 */
UINT8 bTvd3dSignalStatus(void)
{
#if TVD_ATV_SNOWSCREEN_BY_DRV
//#if 0//SUPPORT_ATV_SNOWSCREEN

    if(fgIsSrcAtv(SV_VP_MAIN)|| fgIsSrcAtv(SV_VP_PIP))
    {
        return SV_VDO_STABLE;
    }
    else
    {
        return _rTvd3dStatus.bSigStatus;
    }

#else

    //if ((bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_ATV_EU) && IS_AUTOSEARCH())
    if(IS_AUTOSEARCH())
    {
        return SV_VDO_STABLE; // For EU system channel scan screen crash
    }

    return _rTvd3dStatus.bSigStatus;
#endif
}

/**
 * @brief Set 525, 443 gain to fasten the speed of changing channel.
 * @param bOnOff Set / Restore the register setting for fast changing channel ( Fast setting would cause false channel if
 *                       it is applied under autosearching).
 * @return None
 */

void vTvd3dFastChannelChange(UINT8 bOnOff)
{
    if(bOnOff==SV_ON)
    {
        //_sbFastChannelChange = SV_TRUE;
        _svDrvTvdSetDetV525V625Gain(0x3, 0x3);
#if TVD_FAST_MODEGAIN_CHANNEL_CHANGE
        _svDrvTvdSetDET443Gain(3,3);
#endif
    }
    else
    {
        //_sbFastChannelChange = SV_FALSE;
        _svDrvTvdSetDetV525V625Gain(0,0);
#if TVD_FAST_MODEGAIN_CHANNEL_CHANGE
        _svDrvTvdSetDET443Gain(0,0);
#endif
    }
}

void vTvd3dChannelChange(void)
{
    // Prevent blackmute when block TV in AV.
    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        vTvd3dReset();
        vDrvTvdSetChanchgCnt(CHANCHG_START);
    }
}
/**
 * @brief Triger TVDecoder firmware to re-confirm the format of input signal
 *
 * (Common Function of Video Decoders)
 * Triger TVDecoder firmware to re-confirm the format of input signal, current no hardware
 * action is taken.
 *
 * @param None
 * @return None
 *
 */
void vTvd3dTrigModeDet(void)
{
    vDrvTvdRstModeCnt();
}

/**
 * @brief Set the color system of TVDecoder
 *
 * return the width of current input signal of TVDecoder
 *
 * @param bPath bPath specifies whether SV_VP_MAIN or SV_VP_PIP is the target.
 * @param bColSys bColSys specifies the desired color system, it can be SV_CS_PAL_N,
 * SV_CS_PAL, SV_CS_PAL_M, SV_CS_NTSC358, SV_CS_SECAM, SV_CS_PAL_60, SV_CS_NTSC443, and
 * SV_CS_AUTO (for auto mode detection).
 * @return None
 */
void vDrvTvd3dSetColorSystem(UINT8 bColSys)
{
    LOG(1,"[TVD_DBG_MSG] vDrvTvd3dSetColorSystem bColSys=%d \n",bColSys);

    if(bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)== 1)
    {
        vIO32WriteFldMulti(CDET_00, P_Fld(SV_ON, TVD_MMODE)|P_Fld(SV_CS_NTSC358, TVD_MODE));//Menu NTSC Mode
        return;
    }

    if(bColSys != _rTvd3dStatus.bColSys)
    {
        vTvd3dReset();

        if(bColSys == SV_CS_AUTO)	// Switch to Hardware Auto Detect mode
        {
#if TVD_PHALT_MN_WA2

            if(!_sbForcePalMN)
            {
                vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); //Auto Mode
                LOG(3,"[TVD_DBG_MSG] TVD_PHALT_MN_WA2 Force TVD_MMODE off (AutoColorSys)\n");
            }

#else
            vIO32WriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); //Auto Mode
#endif
        }
        else
        {
            vIO32WriteFldAlign(CDET_00, bColSys, TVD_MODE);
            vIO32WriteFldAlign(CDET_00, SV_ON, TVD_MMODE); //Enable Menu Mode
        }

        _rTvd3dStatus.bColSys = bColSys;
    }
    else
    {
        return;
    }

    vTvd3dTrigModeDet();

    if(fgIsMainTvd3d())
    {
        vSetMainFlg(MAIN_FLG_MODE_CHG);
    }

    if(fgIsPipTvd3d())
    {
        vSetPipFlg(PIP_FLG_MODE_CHG);
    }
}

/**
 * @brief Set the color system of TVDecoder
 *
 * return the width of current input signal of TVDecoder
 *
 * @param bColSys bColSys specifies the desired color system, it can be SV_CS_PAL_N,
 * SV_CS_PAL, SV_CS_PAL_M, SV_CS_NTSC358, SV_CS_SECAM, SV_CS_PAL_60, SV_CS_NTSC443, and
 * SV_CS_AUTO (for auto mode detection).
 * @return None
 */
UINT8 bTvd3dGetColorSystem(void)
{
    if((fgIsMainTvd3d()||fgIsPipTvd3d()) && _rTvd3dStatus.bSigStatus==(UINT8)SV_VDO_STABLE)
    {
        if(fgHwTvdIsMMode())
        {
            return (bHwTvdMMode());
        }
        else
        {
            return (bHwTvdMode());
        }
    }
    else
    {
        return (SV_CS_UNSTABLE);
    }
}

#if TVD_SET_ENABLED_CS
BOOL bDrvTvd3dCheckModeChgForEnabledCS(void)
{
    RTvdEnabledCS_T *pTvdEnabledCS = &_rAvEnabledCS;
    
    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        pTvdEnabledCS = &_rTvEnabledCS;
    }
    
    LOG(1, "[TVD_DBG_MSG] vDrvTvd3dCheckModeChgForEnabledCS Mode=%d\n", _rTvd3dStatus.bTvdMode);
    if((_rTvd3dStatus.bTvdMode==SV_CS_PAL_N && pTvdEnabledCS->u1TvdEnPALN==FALSE)
    || (_rTvd3dStatus.bTvdMode==SV_CS_PAL && pTvdEnabledCS->u1TvdEnPAL==FALSE)
    || (_rTvd3dStatus.bTvdMode==SV_CS_PAL_M && pTvdEnabledCS->u1TvdEnPALM==FALSE)
    || (_rTvd3dStatus.bTvdMode==SV_CS_NTSC358 && pTvdEnabledCS->u1TvdEnNTSC358==FALSE)
    || (_rTvd3dStatus.bTvdMode==SV_CS_SECAM && pTvdEnabledCS->u1TvdEnSECAM==FALSE)
    || (_rTvd3dStatus.bTvdMode==SV_CS_PAL_60 && pTvdEnabledCS->u1TvdEnPAL60==FALSE)
    || (_rTvd3dStatus.bTvdMode==SV_CS_NTSC443 && pTvdEnabledCS->u1TvdEnNTSC443==FALSE))
    {
        LOG(1, "[TVD_DBG_MSG] vDrvTvd3dCheckModeChgForEnabledCS Need ModeChg\n");
        return TRUE;
    }
    else if((_rPrevEnabledCS.u1TvdEnPALN != pTvdEnabledCS->u1TvdEnPALN)
    || (_rPrevEnabledCS.u1TvdEnPAL != pTvdEnabledCS->u1TvdEnPAL)
    || (_rPrevEnabledCS.u1TvdEnPALM != pTvdEnabledCS->u1TvdEnPALM)
    || (_rPrevEnabledCS.u1TvdEnNTSC358 != pTvdEnabledCS->u1TvdEnNTSC358)
    || (_rPrevEnabledCS.u1TvdEnSECAM != pTvdEnabledCS->u1TvdEnSECAM)
    || (_rPrevEnabledCS.u1TvdEnPAL60 != pTvdEnabledCS->u1TvdEnPAL60)
    || (_rPrevEnabledCS.u1TvdEnNTSC443 != pTvdEnabledCS->u1TvdEnNTSC443))
    {
        LOG(1, "[TVD_DBG_MSG] vDrvTvd3dCheckModeChgForEnabledCS Need ModeChg(2)\n");
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

UINT8 vDrvTvd3dSetEnabledColorSystem(UINT32 u4ColSys)
{
    LOG(1, "[TVD_DBG_MSG] SetEnabledColorSystem mask=0x%X\n", u4ColSys);
    _rTvd3dStatus.bColSys = SV_CS_AUTO;
    if(fgIsMainTvd3d()||fgIsPipTvd3d())
    {
#if TVD_SET_ENABLED_CS
        RTvdEnabledCS_T *pTvdEnabledCS = &_rAvEnabledCS;

        if(_rTvd3dStatus.eSourceType==SV_ST_TV)
        {
            pTvdEnabledCS = &_rTvEnabledCS;
        }
        _rPrevEnabledCS = *pTvdEnabledCS;
        
        pTvdEnabledCS->u1TvdEnPALN = TRUE;
        pTvdEnabledCS->u1TvdEnPAL = TRUE;
        pTvdEnabledCS->u1TvdEnPALM = TRUE;
        pTvdEnabledCS->u1TvdEnNTSC358 = TRUE;
        pTvdEnabledCS->u1TvdEnSECAM = TRUE;
        pTvdEnabledCS->u1TvdEnPAL60 = TRUE;
        pTvdEnabledCS->u1TvdEnNTSC443 = TRUE;

        if((u4ColSys & (1<<SV_CS_PAL_N))==0)
        {
            pTvdEnabledCS->u1TvdEnPALN = FALSE;
        }

        if((u4ColSys & (1<<SV_CS_PAL))==0)
        {
            pTvdEnabledCS->u1TvdEnPAL = FALSE;
        }

        if((u4ColSys & (1<<SV_CS_PAL_M))==0)
        {
            pTvdEnabledCS->u1TvdEnPALM = FALSE;
        }

        if((u4ColSys & (1<<SV_CS_NTSC358))==0)
        {
            pTvdEnabledCS->u1TvdEnNTSC358= FALSE;
        }

        if((u4ColSys & (1<<SV_CS_SECAM))==0)
        {
            pTvdEnabledCS->u1TvdEnSECAM = FALSE;
        }

        if((u4ColSys & (1<<SV_CS_PAL_60))==0)
        {
            pTvdEnabledCS->u1TvdEnPAL60 = FALSE;
        }

        if((u4ColSys & (1<<SV_CS_NTSC443))==0)
        {
            pTvdEnabledCS->u1TvdEnNTSC443 = FALSE;
        }

#if 0//TVD_RESET_MODE_CHCHG
        if(bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)==1)
        {
            _rTvd3dStatus.bTvdMode = 0x3;
        }
        else
        {
            _rTvd3dStatus.bTvdMode = 0xff;
        }

        LOG(1,"[TVD_DBG_MSG] Reset tvd mode(%d)\n", _rTvd3dStatus.bTvdMode);
#endif

        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnPALN, PALN_EN);
        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnPALM, PALM_EN);
        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnSECAM, SECAM_EN);
        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnPAL60, PAL60_EN);
        vIO32WriteFldAlign(CDET_00, pTvdEnabledCS->u1TvdEnNTSC443, NTSC443_EN);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem PALN=%d\n", pTvdEnabledCS->u1TvdEnPALN);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem PAL=%d\n", pTvdEnabledCS->u1TvdEnPAL);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem PALM=%d\n", pTvdEnabledCS->u1TvdEnPALM);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem NTSC358=%d\n", pTvdEnabledCS->u1TvdEnNTSC358);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem SECAM=%d\n", pTvdEnabledCS->u1TvdEnSECAM);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem PAL60=%d\n", pTvdEnabledCS->u1TvdEnPAL60);
        LOG(3, "[TVD_DBG_MSG] SetEnabledColorSystem NTSC443=%d\n", pTvdEnabledCS->u1TvdEnNTSC443);
        if(_bATVChannelChange==FALSE && bDrvTvd3dCheckModeChgForEnabledCS())
        {
            _svDrvTvdModeChg();
        }

#endif
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if TVD_SET_ENABLED_CS
void vDrvTvd3dCheckMModeForEnabledCS(void)
{
    UINT8 bMModeCS = SV_CS_SECAM;
    RTvdEnabledCS_T *pTvdEnabledCS = &_rAvEnabledCS;

    if(!_rTvd3dStatus.fgVPres)
    {
        return;
    }

    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        pTvdEnabledCS = &_rTvEnabledCS;
    }

    if(_rTvd3dStatus.bTvdMode==SV_CS_NTSC358 && pTvdEnabledCS->u1TvdEnNTSC358==FALSE)
    {
        if(pTvdEnabledCS->u1TvdEnNTSC443)
        {
            bMModeCS = SV_CS_NTSC443;
        }
        else if(pTvdEnabledCS->u1TvdEnPALM)
        {
            bMModeCS = SV_CS_PAL_M;
        }
        else if(pTvdEnabledCS->u1TvdEnPAL60)
        {
            bMModeCS = SV_CS_PAL_60;
        }
        else if(pTvdEnabledCS->u1TvdEnPALN)
        {
            bMModeCS = SV_CS_PAL_N;
        }
        else if(pTvdEnabledCS->u1TvdEnPAL)
        {
            bMModeCS = SV_CS_PAL;
        }
        else
        {
            bMModeCS = SV_CS_SECAM;
        }

        _bMModeForEnabledCS = TRUE;
        vIO32WriteFldAlign(CDET_00, bMModeCS, TVD_MODE);
        vIO32WriteFldAlign(CDET_00, SV_ON, TVD_MMODE); //Enable Menu Mode
        _rTvd3dStatus.bTvdMode = bMModeCS;
        LOG(1, "[TVD_DBG_MSG] NTSC358 is disabled, trigger MMode = %d\n", bMModeCS);
    }

    if(_rTvd3dStatus.bTvdMode==SV_CS_PAL && pTvdEnabledCS->u1TvdEnPAL==FALSE)
    {
        if(pTvdEnabledCS->u1TvdEnPALN)
        {
            bMModeCS = SV_CS_PAL_N;
        }
        else if(pTvdEnabledCS->u1TvdEnSECAM)
        {
            bMModeCS = SV_CS_SECAM;
        }
        else if(pTvdEnabledCS->u1TvdEnNTSC358)
        {
            bMModeCS = SV_CS_NTSC358;
        }
        else if(pTvdEnabledCS->u1TvdEnNTSC443)
        {
            bMModeCS = SV_CS_NTSC443;
        }
        else if(pTvdEnabledCS->u1TvdEnPALM)
        {
            bMModeCS = SV_CS_PAL_M;
        }
        else
        {
            bMModeCS = SV_CS_PAL_60;
        }

        _bMModeForEnabledCS = TRUE;
        vIO32WriteFldAlign(CDET_00, bMModeCS, TVD_MODE);
        vIO32WriteFldAlign(CDET_00, SV_ON, TVD_MMODE); //Enable Menu Mode
        _rTvd3dStatus.bTvdMode = bMModeCS;
        LOG(1, "[TVD_DBG_MSG] PAL is disabled, trigger MMode = %d\n", bMModeCS);
    }

    return;
}
#endif

void vTvd3dReset(void)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    vDrvTDCActive(SV_OFF);
    vTvd3dResetOn();
    vUtDelay2us(2);
    vTvd3dResetOff();
    _svDrvTvdRstVpresState();
    x_crit_end(csState);
   // vUtDelay10ms(10);  remove dealy 100ms by zuyu for reduce boot time 2013.11.15
}


UINT8 bTvdNRLevel(void)
{
    return (bHwTvdNRLevel());
}

#if TVD_CHECK_BURST
/**
 * @brief   For tuner, check TVDLock Burst frequency.
 *
 * @param   none
 *
 * @retval  IS443/IS358/unknow
 */

UINT8 bDrvTvdCheckBurstFreq(void)
{
    UINT32 dwTVDBurstSeed;
    vIO32WriteFldAlign(CTG_00, SV_OFF, BSYNCSTA_SEL);
    vIO32WriteFldAlign(CTG_04, SV_OFF, STACTG00_SEL);
    dwTVDBurstSeed=IO32ReadFldAlign(STA_CTG_00,Fld(20, 12, AC_FULLDW));

    if((fgHwTvdVPres())&&fgHwTvdBLock4DET()&&(bHwTvdCAGC()<TVD_SCAN_CAGCThres))
    {
        if(fgHwTvd443() && (dwTVDBurstSeed < TVD_BURSTSEED_THR))
        {
            return TVD_BLOCK_IS443;
        }
        else if((!fgHwTvd443()) && (dwTVDBurstSeed > TVD_BURSTSEED_THR))
        {
            return TVD_BLOCK_IS358;
        }
        else
        {
            return TVD_BLOCK_UNKNOW;
        }
    }

    return TVD_BLOCK_UNKNOW;
}
#endif

/**
 * @brief   For tuner, check TVDLock.
 *
 * @param   bTryLoopCnt: loop count
 *
 * @retval  TRUE/FALSE
 */
BOOL fgDrvTvdCheckTVDLock(UINT16 bTryLoopCnt)
{
    // update interval * max value = 2048*128 = 2^18
    // 2^18 / 27 MHz = 0.0097 sec  = about 10 ms
    // tatal: 10Times x 2ms =20ms to do safe check
    if(_rTvd3dStatus.eSourceType!=SV_ST_TV)
    {
        return TRUE;    //for TV SCART bypass, it should not check TVD lock
    }

    if(IS_AUTOSEARCH())
    {
#ifdef NEW_COUNTRY_TYPE

        if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_ATV_US)
#else
        if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_US)
#endif
        {
            vTvd3dReset();//20110719: Only US Need to reset. The other will be reset in ATD
        }

        //wTryLoopCnt = wVpresCheckTime;
        // CN suggest to add 100ms after reset for scan loss, 081114
        //vUtDelay10ms(10);
    }

	LOG(3,"[TVD_DBG_MSG] fgDrvTvdCheckTVDLock current on_gain(%d), off_gain(%d), loopCnt(%d)\n",
		IO32ReadFldAlign(DFE_0F, VP1_ON_GAIN),IO32ReadFldAlign(DFE_0F, VP1_OFF_GAIN),bTryLoopCnt);
    while(bTryLoopCnt>0)
    {
        if(IO32ReadFldAlign(STA_CDET_00, VPRES_TVD3D))  //avoid vpres4 lock the sound carrier
        {
			LOG(3,"[TVD_DBG_MSG] fgDrvTvdCheckTVDLock vpres_tvd3d on, loopcnt(%d)\n",bTryLoopCnt);
			#if 0
            if(_fgDescramblerOn)
            {
                LOG(3, ".......for Descramber box issue,return TRUE directly");
                return TRUE;
            }
			#endif

#ifdef NEW_COUNTRY_TYPE

            if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_ATV_US)
#else
            if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_US)
#endif
            {
                LOG(3,"[TVD_DBG_MSG] fgDrvTvdCheckTVDLock Vpres lcoked\n");
                return TRUE;
            }
            else
            {
                //don't check Vlock when setting channel, Yulia,2005.10.24
                if(IS_AUTOSEARCH())
                {
                    if(_sfgDrvTvdCheckVLock(VLOCK_CHECK_TIME) != TRUE)
                    {
                        //check again when Vpres on but vunlock
                        return _sfgDrvTvdCheckVLock(VLOCK_CHECK_TIME);
                    }
                    else
                    {
                        return TRUE;
                    }
                }
                else
                {
                    return TRUE;
                }
            }
        }

        vUtDelay1ms(2);
        bTryLoopCnt --;
    }

    LOG(3,"[TVD_DBG_MSG] fgDrvTvdCheckTVDLock Vpres unlcoked\n");
    return FALSE;
}

BOOL fgDrvTvdCheckVPresOn(void)
{
    //UINT32 u4VLockTotalCnt = _u4ChLockCnt + _u4ChUnLockCnt;
    UINT32 u4VPresTotalCnt = _u4ChVPresOnCnt + _u4ChVPresOffCnt;
    BOOL u1Value = FALSE;

#ifndef CC_TVD_SUPPORT_DDI // LG AP call
    if(IS_AUTOSEARCH())
    {
        if(u4VPresTotalCnt!=0)
        {
            if(u4VPresTotalCnt>20)
            {
                if(_u4ChVPresOnCnt>0)
                    u1Value = TRUE;
            }
            else if((_u4ChVPresOnCnt*100/u4VPresTotalCnt)>60)
                u1Value = TRUE;
            else if(_u4ChVPresOnCnt>0)
            {
                LOG(3,"[TVD_DBG_MSG] VPresOn=%d, VPresOff=%d\n", _u4ChVPresOnCnt, _u4ChVPresOffCnt);
            }
        }
        else
        {
            LOG(3,"[TVD_DBG_MSG] u4VPresTotalCnt=%dn", u4VPresTotalCnt);
        }
        _u4ChVPresOnCnt = _u4ChVPresOffCnt = _u4ChLockCnt = _u4ChUnLockCnt = 0;
    }
    else
#endif
    {        
        if(u4VPresTotalCnt>=5)
        {
            if(_u4ChVPresOnCnt>2)
                u1Value = TRUE;
            else
            {
                LOG(3,"[TVD_DBG_MSG] VPresOn=%d, VPresOff=%d\n", _u4ChVPresOnCnt, _u4ChVPresOffCnt);
            }
            _u4ChVPresOnCnt = _u4ChVPresOffCnt = _u4ChLockCnt = _u4ChUnLockCnt = 0;
        }
        else
        {
            u1Value = _bChPrevVPres;
            if(u1Value==FALSE)
            {
                LOG(3,"[TVD_DBG_MSG] u4VPresTotalCnt=%dn", u4VPresTotalCnt);
            }
        }
        _bChPrevVPres = u1Value;
    }
    
    return u1Value;
}

void vDrvTvdSetChannelChange(UINT8 bOnOff)
{
    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        _bATVChannelChange = bOnOff;
        LOG(1, "_bATVChannelChange=%d\n", _bATVChannelChange);
#if TVD_BP_ATV_MODECHG
        if(_bATVChannelChange == TRUE)
            _sbBypassModeChg = FALSE;
#endif
        if(_bATVChannelChange == TRUE)
        {
            #ifdef CC_SUPPORT_RECORD_AV
            if(_sbConnectedBySub == FALSE)
            #endif
            {
                CRIT_STATE_T csState;
                csState = x_crit_start();
                vTvd3dResetOn();
                vUtDelay2us(2);
                vTvd3dResetOff();
                x_crit_end(csState);
            }
            if(bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)==1)
            {
                _rTvd3dStatus.bTvdMode = 0x3;
            }
            else
            {
                _rTvd3dStatus.bTvdMode = 0xff;
            }
            LOG(1,"[TVD_DBG_MSG] Reset tvd mode(%d)\n",_rTvd3dStatus.bTvdMode);
        }
    }
#if TVD_ANA_COPY_PROTECT_FILTER
    vDrvTvd3dAnaCopyProInit();
#endif
}

BOOL fgDrvTvdCheckVideoStable(void)
{
#if 0
    if(_rTvd3dStatus.bSigStatus==(UINT8)SV_VDO_NOSIGNAL || (fgHwTvdVPresTVD3D() && _sbTvdModeCnt>=_u1TvdVideoStableCnt))
        return TRUE;
    else
        return FALSE;
#endif
    if(_rTvd3dStatus.bSigStatus==(UINT8)SV_VDO_STABLE || _rTvd3dStatus.bSigStatus==(UINT8)SV_VDO_NOSIGNAL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * @brief   Get NA state.
 *
 * @param   None
 *
 * @retval  2:High Noise, 1: Low Noise
 */
UINT8 bTvdNAState4Step(void)
{
    if(_swTVDNRLevel_AVG > TVDNRLevel_TH)
    {
        return 2;//high noise
    }
    else
    {
        return 1;//low noise
    }
}

/**
 * @brief signal level for tuner
 * @param bMode
 *	0: 100~0
 *	1: dB value
 * @return the signal strength level
 */
UINT8 bTvdSignalStrengthLevel(UINT8 bMode)
{
    UINT8 bValue;
    bValue = 100 -((_swTVDNRavg1*100)>>7);

    if(fgHwTvdVPresTVD3D())
    {
        if(bMode == 0)
        {
            return bValue;
        }
        //get signal level interface in mw_if
        else if(bMode == 2)
        {
            bValue = ((_svDrvTvdGetNRLevel()-20)*100)/60;
            return bValue;
        }
        else
        {
            return _svDrvTvdGetNRLevel();
        }
    }
    else
    {
        return 0;
    }
}


/**
 * @brief The width of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the width of current input signal of TVDecoder.
 * The width of possible TVDecoder input is only 720.
 *
 * @param None
 * @return the width of current video signal
 *
 */
UINT16 wTvdWidth(void)
{
    #if TVD_SUPPORT_704_DE
    return IO32ReadFldAlign(SYS_00,DATA_SYNC_DUAL)?1408:704;
    #else
    return IO32ReadFldAlign(SYS_00,DATA_SYNC_DUAL)?1440:720;
    #endif
}

/**
 * @brief The height of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the height of current input signal of TVDecoder.
 * The height of possible TVDecoder input are 480 or 576, depend on
 * the mode of input signal.
 *
 * @warning When tuner is performing auto-search, we force the return height to be 480,
 * to prevent scale factor unstable during the process.
 * @param None
 * @return the height of current video signal
 *
 */
UINT16 wTvd3dHeight(void)
{
    return _rTvd3dStatus.fgIs525?480:576;
}

void vPreTvdWidthHeight(UINT16 *pu2Width,UINT16 *pu2Height)
{
    *pu2Width = _su2ActiveWidth;
    *pu2Height = _su2ActiveHeight;
}


#if TVD_AUTO_CALIB
/**
 * @brief vDrvIntAutoColorStart( void )
 * Internal auto color start
 * @param  void
 * @retval void
 * @example vDrvIntAutoColorStart()
 */
void vDrvTVDAutoCalibStart(void)
{
    if(!fgIsMainTvd3d() && !fgIsPipTvd3d())
    {
        LOG(1,"Calibration Fail !  Please use correct source to do calibration\n");
        return;
    }

    _bTVDAutoCalibState = VDO_AUTO_COLOR_START;
    _sbTVDAutoCalibState1 = VDO_AUTO_COLOR_1_START;
    _sbIsTVDFixAgc = SV_ON;
}

void vDrvTVDAutoCalibState(void)
{
    switch(_bTVDAutoCalibState)
    {
        case VDO_AUTO_COLOR_NOT_BEGIN:
            break;

        case VDO_AUTO_COLOR_START:
            if(bDrvTVDAdcCalib())
            {
                vApiVideoClrEvtFlg(VDO_FLG_TVDAUTO_DOING);
                _bTVDAutoCalibState = VDO_AUTO_COLOR_END;

                if(_sbTVDAutoCalibResult == SV_SUCCESS)
                {
                    vApiNotifyAutoColorDone((fgIsMainTvd3d()?0:1),TRUE);
                }
                else
                {
                    vApiNotifyAutoColorDone((fgIsMainTvd3d()?0:1),FALSE);
                }
            }

            break;

        case VDO_AUTO_COLOR_END:
            _bTVDAutoCalibState = VDO_AUTO_COLOR_NOT_BEGIN;
            LOG(1,"======> Auto Color Finish !!! <====== \n");
            break;

        default:
            break;
    }
}

void vDrvSetTVDOffset(UINT16 value)
{
    vIO32WriteFldAlign(CTG_07, value, BLKLVL);
}


/**
 * @brief bDrvGetTVDADCGain(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT8 manual analog gain code
 */
UINT8 wDrvGetTVDADCMAX(void)
{
    if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
    {
        return bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_MAX);
    }
    else
    {
        return bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_MAX);
    }
}


//Mike modify ADC setting 20080704
/**
 * @brief vCust_TVD_ADC_MAX_Update(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT8 manual analog gain code
 */
void  vCust_TVD_ADC_MAX_Update(UINT8 value)
{
    if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
    {
        UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_MAX,value));
    }
    else
    {
        UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_MAX,value));
    }
}

void vCust_TVD_Gain_Update(UINT8 value)
{
    if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
    {
        UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN0, value));
        //UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_DONE, 0xAA));
    }
    else
    {
        UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0, value));
        //UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_DONE, 0xAA));
    }
}

/**
 * @brief bDrvGetTVDADCOffset(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT16 manual offset code
 */
UINT16 bDrvGetTVDADCOffset(void)
{
    return (UINT16)IO32ReadFldAlign(DFE_0A, CLAMP_TARGET_BLANK_LV);
}
/**
 * @brief bDrvSetTVDADCOffset(UINT16 value)
 * call by d_custom_vid_set
 * @param  value manual offset code
 * @retval void
 */
void vDrvSetTVDADCOffset(UINT16 value)
{
    _sbIsTVDOffsetbyAP = 1;
    vIO32WriteFldAlign(DFE_0A, value, CLAMP_TARGET_BLANK_LV);
}

void vCust_TVD_Offset_Update(UINT16 value)
{
    value = value - 200;//only store the value - 200

    if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
    {
        UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_OFFSET, (UINT8)value));
    }
    else
    {
        UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_OFFSET, (UINT8)value));
    }
}

/**
 * @brief    AV and TV ADC Calibration target 16~235
 * @remarks
 * @return   TRUE/FALSE
 */
BOOL bDrvTVDAdcCalib(void)
{
    UINT8 MANUAL_ACODE, TUNE_STEP;
    UINT16 MANUAL_DCODE;
    UINT8 Tolerance = 0x2;
    UINT16 TARGET_Y_LEVEL;
#ifdef CC_TVE_POWERDOWN_HALF
    UINT16 TARGET_SYNC_ORI;
#endif

    switch(_rTvd3dStatus.bTvdMode)
    {
        case SV_CS_SECAM:
        case SV_CS_PAL:
        case SV_CS_PAL_N:
            if(DRVCUST_OptGet(eTVDSupportOutput216))
            {
                TARGET_Y_LEVEL = TARGET_Y_LEVEL_P216;
            }
            else
            {
                TARGET_Y_LEVEL = TARGET_Y_LEVEL_P;
            }

            break;

        default:
            if(DRVCUST_OptGet(eTVDSupportOutput216))
            {
                TARGET_Y_LEVEL = TARGET_Y_LEVEL_N216;
            }
            else
            {
                TARGET_Y_LEVEL = TARGET_Y_LEVEL_N;
            }

            break;
    }

#ifdef CC_TVE_POWERDOWN_HALF

    //amplifier gain=2.83, 1/(2.83/2)=0.707
    switch(_rTvd3dStatus.bTvdMode)
    {
        case SV_CS_SECAM:
        case SV_CS_PAL:
        case SV_CS_PAL_N:
	#if TVD_SUPPORT_DRVCUST_DEFINE
	    TARGET_SYNC_ORI = DRVCUST_OptGet(eTVDTgt_Sync_Height_625); 
	#else
            TARGET_SYNC_ORI = DFE_TGT_SYNC_HEIGHT_625;
	#endif
            break;

        default:
	#if TVD_SUPPORT_DRVCUST_DEFINE
	    TARGET_SYNC_ORI = DRVCUST_OptGet(eTVDTgt_Sync_Height_525);
	#else
            TARGET_SYNC_ORI = DFE_TGT_SYNC_HEIGHT_525;
	#endif
            break;
    }

    if(IO32ReadFldAlign(DFE_03, AGC2_MODE) == E_AUTO_GAIN)
    {
        TARGET_Y_LEVEL = TARGET_Y_LEVEL*0.76;
    }
    else
    {
        TARGET_Y_LEVEL = TARGET_Y_LEVEL;
    }

#endif

    //Enable AGC
    if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_1_START)
    {
#ifdef CC_TVE_POWERDOWN_HALF

        if(DRVCUST_OptGet(eTVDSupportOutput216))
        {
            _swTvd_Gain = TARGET_SYNC_ORI*0.69;//0.76*0.91;
        }
        else
        {
            _swTvd_Gain = TARGET_SYNC_ORI*0.76;
        }

        _svDrvTvdSetTVDAGCTarget(_swTvd_Gain);
#endif
        vDrvWhitePeakLimit(SV_OFF);
        vIO32WriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);
        vIO32WriteFldAlign(DFE_18, DFE_AUTOAGC_DAGCSEL, FIXAGC_DAGCSEL);
        vTvdSetAGCMode(E_AUTO_GAIN);
        _sbTVDAutoCalibState1 = VDO_AUTO_COLOR_2_START;
        _sbTVDAutoCalibCount = 0;
        _swY_Avg_Lvl = wDrvGetTVDYLvl();		//get Avg. PY_LV dummy read
        return SV_FAIL;
    }

    if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_2_START)
    {
        _sbTVDAutoCalibCount++;

        if(_sbTVDAutoCalibCount == TVDAutoCalibDelayTime)
        {
            _sbTVDAutoCalibState1 = VDO_AUTO_COLOR_3_START;
            _sbTVDAutoCalibCount = 0;
        }

        return SV_FAIL;
    }

    if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_3_START)
    {
        _swTvd_Gain = _swDrvTvdGetTVDAGCTarget() & 0x0fff;		//gain calibration
        _swY_Avg_Lvl = wDrvGetTVDYLvl();		//get Avg. PY_LV
        _swY_Diff = (_swY_Avg_Lvl>TARGET_Y_LEVEL)?(_swY_Avg_Lvl-TARGET_Y_LEVEL):(TARGET_Y_LEVEL-_swY_Avg_Lvl);

        if(_swY_Diff > 0xA0)
        {
            LOG(0, "_swY_Avg_Lvl= %d,TARGET_Y_LEVEL=%d,_swY_Diff=%d\n",_swY_Avg_Lvl,TARGET_Y_LEVEL,_swY_Diff);
            LOG(0, "Calibration Fail !  Please use correct pattern to do calibration\n");
            _bTVDAutoCalibState = VDO_AUTO_COLOR_END;
            _sbTVDAutoCalibResult = SV_FAIL;
            return SV_TRUE;
        }

        _sbTVDAutoCalibState1 = VDO_AUTO_COLOR_4_START;
        return SV_FAIL;
    }

    if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_4_START)
    {
        if(_swY_Diff>0x40)
        {
            TUNE_STEP = 0x2;
        }
        else
        {
            TUNE_STEP = 0x1;
        }

        if(_swY_Avg_Lvl > TARGET_Y_LEVEL)
        {
            _swTvd_Gain -= TUNE_STEP;
        }
        else
        {
            _swTvd_Gain += TUNE_STEP;
        }

        _svDrvTvdSetTVDAGCTarget(_swTvd_Gain);
        LOG(0, "_swTvd_Gain(Target_Sync_High)=%x\n",_swTvd_Gain);

        if((_swTvd_Gain > 0x400) || (_swTvd_Gain < 0x200))
        {
            _bTVDAutoCalibState = VDO_AUTO_COLOR_END;
            _sbTVDAutoCalibResult = SV_FAIL;
            return SV_TRUE;
        }

        _sbTVDAutoCalibState1 = VDO_AUTO_COLOR_5_START;
        return SV_FAIL;
    }

    if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_5_START)
    {
        _swY_Avg_Lvl = wDrvGetTVDYLvl();
        _swY_Diff = (_swY_Avg_Lvl>TARGET_Y_LEVEL)?(_swY_Avg_Lvl-TARGET_Y_LEVEL):(TARGET_Y_LEVEL-_swY_Avg_Lvl);
        LOG(0, "_swY_Avg_Lvl=%x , _swY_Diff=%x\n",_swY_Avg_Lvl, _swY_Diff);

        if(_swY_Diff > Tolerance)
        {
            _sbTVDAutoCalibState1 = VDO_AUTO_COLOR_4_START;
            return SV_FAIL;
        }

        //Fix AGC
        MANUAL_ACODE = IO32ReadFldAlign(DFE_STA_04, AGAIN_CODE);

        if(_sbPre_Manual_ACode != MANUAL_ACODE)
        {
            _sbPre_Manual_ACode = MANUAL_ACODE;
            _sbTVDAutoCalibState1 = VDO_AUTO_COLOR_5_START;
            return SV_FAIL;
        }

#ifdef CC_TVE_POWERDOWN_HALF

        if(IO32ReadFldAlign(DFE_03, AGC2_MODE) == E_AUTO_GAIN)
        {
            LOG(0, "Calibration Again ok, start calibrate Dgain\n");

            if(DRVCUST_OptGet(eTVDSupportOutput216))
            {
                _swTvd_Gain = TARGET_SYNC_ORI*0.91;
            }
            else
            {
                _swTvd_Gain = TARGET_SYNC_ORI;
            }

            _svDrvTvdSetTVDAGCTarget(_swTvd_Gain);
            vDrvSetTVDADCGain(MANUAL_ACODE);
            vTvdSetAGCMode(E_MENU_AGAIN);
            _sbTVDAutoCalibState1 = VDO_AUTO_COLOR_5_START;
            return SV_FAIL;
        }

        //UINT8 i;
        //LOG(1,"DCODE start\n");
        //for(i = 0; i<50; i++)
        //{
        //	MANUAL_DCODE = IO32ReadFldAlign(DFE_STA_04, DGAIN_CODE);
        //	LOG(1,"DCODE=%x\n",MANUAL_DCODE);
        //	vUtDelay1ms(1);
        //}
        //LOG(1,"DCODE END\n");
#endif
        vTvdSetAGCMode(E_HOLD_GAIN);
        MANUAL_DCODE = IO32ReadFldAlign(DFE_STA_04, DGAIN_CODE);
        LOG(0, "MANUAL_ACODE=%x\n",MANUAL_ACODE);
        LOG(0, "MANUAL_DCODE=%x\n",MANUAL_DCODE);
        vDrvSetTVDADCGain(MANUAL_ACODE);
        vDrvSetTVDADCDGain(MANUAL_DCODE);
#if TVD_USE_NEW_DAGC
        vTvdSetAGCMode(E_MENU_AGAIN);
        vIO32WriteFldAlign(DFE_03, SV_ON, MANUAL_DAGC_EN);
        vIO32WriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL, FIXAGC_DAGCSEL);
#else
        vTvdSetAGCMode(E_MENU_GAIN);
        vIO32WriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);//MT5387 FPGA
        vIO32WriteFldAlign(DFE_18, 0, FIXAGC_DAGCSEL);
#endif
        vDrvWhitePeakLimit(SV_ON);

        //save data in EEPROM after calibartion
        switch(_rTvd3dStatus.bTvdMode) //YUV2YCbCr
        {
            case SV_CS_SECAM:
            case SV_CS_PAL:
            case SV_CS_PAL_60:
            case SV_CS_PAL_N:
                //wYlevel = ((wYlevel+40)*1.6)/4;
                _swY_Avg_Lvl = ((_swY_Avg_Lvl+40)*2)/5;
                break;

            default:
                //wYlevel = ((wYlevel+41)*1.56)/4;
                _swY_Avg_Lvl = ((_swY_Avg_Lvl+41)*39)/100;
                break;
        }

        LOG(0, "TVD MAX=%d\n",_swY_Avg_Lvl);

        if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
        {
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN0, MANUAL_ACODE));
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN1, (UINT8)MANUAL_DCODE));
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN2, (UINT8)(MANUAL_DCODE>>8)));
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_MAX, _swY_Avg_Lvl));
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_DONE, 0xAA));
        }
        else
        {
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0, MANUAL_ACODE));
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN1, (UINT8)MANUAL_DCODE));
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN2, (UINT8)(MANUAL_DCODE>>8)));
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_MAX, _swY_Avg_Lvl));
            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_DONE, 0xAA));
        }

        _sbTVDAutoCalibResult = SV_SUCCESS;
        LOG(0, "Calibration SUCCESS!\n");
        return SV_SUCCESS;
    }

    return SV_FAIL;
}

BOOL bDrvTVDAdcCalib4Auto(void)
{
    UINT16 wY_Avg_Lvl;
    UINT16 wTvd_Gain;
    UINT16 TARGET_Y_LEVEL;

    if(!fgIsMainTvd3d() && !fgIsPipTvd3d())
    {
        return SV_FAIL;
    }

    _sbIsTVDFixAgc = SV_OFF;
    //Disable white peak limiter
    vDrvWhitePeakLimit(SV_OFF);
    //Enable AGC
    vTvdSetAGCMode(E_AUTO_GAIN);
    vUtDelay10ms(10);
    wTvd_Gain = _swDrvTvdGetTVDAGCTarget();
    wY_Avg_Lvl = wDrvGetTVDYLvl();

    switch(_rTvd3dStatus.bTvdMode)
    {
        case SV_CS_SECAM:
        case SV_CS_PAL:
        case SV_CS_PAL_60:
        case SV_CS_PAL_N:
            TARGET_Y_LEVEL = TARGET_Y_LEVEL_P;
            break;

        default:
            TARGET_Y_LEVEL = TARGET_Y_LEVEL_N;
            break;
    }

    while(wY_Avg_Lvl != TARGET_Y_LEVEL)
    {
        if((wY_Avg_Lvl > TARGET_Y_LEVEL) && wTvd_Gain >0)
        {
            wTvd_Gain--;
        }
        else if((wY_Avg_Lvl < TARGET_Y_LEVEL) && wTvd_Gain < 0xFFF)
        {
            wTvd_Gain++;
        }

        _svDrvTvdSetTVDAGCTarget(wTvd_Gain);
        LOG(5, "wTvd_Gain=%x\n",wTvd_Gain);
        vUtDelay10ms(10);
        wY_Avg_Lvl = wDrvGetTVDYLvl();
        LOG(5, "wY_Avg_Lvl=%x\n",wY_Avg_Lvl);
    }

    //Enable white peak limiter
    vDrvWhitePeakLimit(SV_ON);
    return SV_SUCCESS;
}
#endif

void vTvd3dSetYCDelay(UINT8 bYCDelay)
{
    if(_sbIsSetYCDelaybyAP)
    {
        return;
    }

    vIO32WriteFldAlign(CTG_07, (bYCDelay&0x3) ,Y_DELAY); //Y_DELAY
    vIO32WriteFldAlign(CTG_07, (bYCDelay>>2) ,UV_DELAY); //UV_DELAY
}

void vTvd3dSetYCDelaybyAP(UINT8 bYDelay, UINT8 bCDelay)
{
    _sbIsSetYCDelaybyAP = SV_ON;
    vIO32WriteFldAlign(CTG_07, bYDelay ,Y_DELAY); //Y_DELAY
    vIO32WriteFldAlign(CTG_07, bCDelay ,UV_DELAY); //UV_DELAY
}

//void vTvd3dSetCAGCBound(UINT32 dwBound)
//{
//	if(_sbIsSetCAGCbyAP == SV_ON) return;
//	vIO32Write4B(CDET_07, dwBound);// CAGCBound ??
//}


void vTvd3dSetCAGCbyAP(void)
{
    _sbIsSetCAGCbyAP = SV_ON;
    vIO32WriteFldAlign(CDET_05, _rTvd3dCAGCInfo.MCAGC_EN, MACC_EN);
    vIO32WriteFldAlign(CDET_05, _rTvd3dCAGCInfo.MCAGC, MACC);
    /*
    _svDrvTvdSetCAGCBound4Data((_rTvd3dCAGCInfo.Bound>>24),
                        (_rTvd3dCAGCInfo.Bound>>16)&0xFF,
                        (_rTvd3dCAGCInfo.Bound>>8)&0xFF,
                        _rTvd3dCAGCInfo.Bound&0xFF);
    */
    //_svDrvTvdSetCAGCBound4Data
    vIO32WriteFldMulti(SECAM_03,P_Fld((_rTvd3dCAGCInfo.Bound>>24),CAGC_IP_MID_B)|P_Fld((_rTvd3dCAGCInfo.Bound>>16)&0xFF,CAGC_IP_MID_A));
    vIO32WriteFldMulti(SECAM_05, P_Fld((_rTvd3dCAGCInfo.Bound>>8)&0xFF, CAGC_IP_TARGET_B) | P_Fld(_rTvd3dCAGCInfo.Bound&0xFF, CAGC_IP_TARGET_A));
    vIO32WriteFldAlign(CDET_08, _rTvd3dCAGCInfo.L_Step, CAGC_L_STEP);
    vIO32WriteFldAlign(CDET_08, _rTvd3dCAGCInfo.S_Step, CAGC_S_STEP);
}

void vTvd3dATDInit(UINT8 bATDOnOff)
{
    if(bATDOnOff==SV_ON)
    {
        // Set Noise Measure Source.
        vIO32WriteFldAlign(CDET_0C, SV_ON, NR_VSRC_SEL);
        // ATD Flag
        //_sbIsATDOnOff = SV_ON;
    }
    else // ATD Off
    {
        // Set Noise Measure Source.
        vIO32WriteFldAlign(CDET_0C, SV_OFF, NR_VSRC_SEL);
        // ATD Flag
        //_sbIsATDOnOff = SV_OFF;
    }

    vIO32WriteFldAlign(DFE_0D, DFE_CLAMP_ATD_GAIN, CLAMP_ATD_GAIN);
}

UINT8 bTvdLumaSignalRefreshRate(void)
{
    return fgHwTvd525()?60:50;
}

/**
 * @brief TDC Set Dram Base Routine
 *
 * Set TDC Dram Base
 *
 * @param None
 * @return None
 * @warning
 * @todo
 *
 * @pre System Initial
 * @post TDC can start
 */

#if SUPPORT_S_DETECT
void vSVPresDet(void)
{
    UINT8 path;

    if(_sbSVFNotify==SV_TRUE)
    {
        LOG(3,"[TVD_DBG_MSG] vSVPresDet fgSVPres=%d \n",_rTvd3dStatus.fgSVPres);
        path=(fgIsMainTvd3d()?0:1);

        if(_rTvd3dStatus.fgSVPres)
        {
            vApiNotifySFirst(path,SV_VDO_STABLE);
        }
        else
        {
            vApiNotifySFirst(path,SV_VDO_NOSIGNAL);
        }

        _sbSVFNotify=SV_FALSE;
    }
}
#endif

#ifdef  SUPPORT_AV_COMP_AUTO
void vAVCompAutoDet(void)
{
    if((VSS_MAJOR(_bMonMainNew) != VSS_YPBPR) && (VSS_MAJOR(_bMonSubNew) != VSS_YPBPR))
    {
        return;
    }

    UINT8 u1Status ;
    UINT8 path=(fgIsMainTvd3d()?0:1);

    if(fgHwTvdBLock()&&(bHwTvdCAGC()<0x40)) //10% burst level
    {
        u1Status = SV_VDO_NOSIGNAL;
    }
    else
    {
        u1Status = u1DrvGetCompStatus() ;
    }

    if(u1Status == _u1AVCompStatus)
    {
        vDrvTvdIncAVCompCnt();

        if(fgDrvTvdIsAVCompCnt(TVD_AVCOMP_CNT))
        {
            UINT8 u1Status ;
            UINT8 path=(fgIsMainTvd3d()?0:1);

            if(fgHwTvdBLock()&&(bHwTvdCAGC()<0x40)) //10% burst level
            {
                u1Status = SV_VDO_NOSIGNAL;
            }
            else
            {
                u1Status = u1DrvGetCompStatus() ;
            }

            LOG(3, "TVD AVCompo Notify(%d)\n", u1Status);
            vApiNotifyAVCOMPAuto(path, u1Status);
        }
    }
    else
    {
        _u1AVCompStatus = u1Status ;
        vDrvTvdRstAVCompCnt();
    }
}
#endif

void vTvdResetChromaLock(void)
{
    #if !TVD_FORCE_NO_CKILL
    vIO32WriteFldMulti(CDET_05,
                       P_Fld(SV_ON,MCKILL_EN)|
                       P_Fld(SV_ON,MCKILL));
    vUtDelay2us(2);
    vIO32WriteFldMulti(CDET_05,
                       P_Fld(0x0,MCKILL_EN)|
                       P_Fld(0x0,MCKILL));
    #endif
}

UINT16 wTvdGetDefaultPorch(UINT8 bPath,UINT8 bPorchType)
{
    switch(bPorchType)
    {
        case SV_HPORCH_DEFAULT:
            switch(_rTvd3dStatus.bTvdMode)
            {
                case SV_CS_PAL_N:
                case SV_CS_PAL:
                case SV_CS_SECAM:
                    return 68;

                case SV_CS_PAL_M:
                case SV_CS_PAL_60:
                case SV_CS_NTSC358:
                case SV_CS_NTSC443:
                    return 58;
            }

            break;

        case SV_VPORCH_DEFAULT:
            switch(_rTvd3dStatus.bTvdMode)
            {
                case SV_CS_PAL_N:
                case SV_CS_PAL:
                case SV_CS_SECAM:
                    return 40;

                case SV_CS_PAL_M:
                case SV_CS_PAL_60:
                case SV_CS_NTSC358:
                case SV_CS_NTSC443:
                    return 32;
            }

            break;

        default:
            return 0;
    }

    return 0;
}

UINT16 wTvdGetPorch(UINT8 bPath,UINT8 bPorchType)
{
    switch(bPorchType)
    {
        case SV_HPORCH_CURRENT:
            LOG(3, "wTvdGetPorch H_Current = 0x%02X\n", (DFT_H_PORCH+_su1UIDelay));
            return (DFT_H_PORCH+_su1UIDelay);
            
        case SV_HPORCH_DEFAULT:
            LOG(3, "wTvdGetPorch H_Default = 0x%02X\n", (DFT_H_PORCH));
            return DFT_H_PORCH;

        case SV_VPORCH_CURRENT:
            LOG(3, "wTvdGetPorch V_Current = 0x%02X\n", (DFT_V_PORCH+_su1UIVPorchDelay));
            return (DFT_V_PORCH+_su1UIVPorchDelay);

        case SV_VPORCH_DEFAULT:
            LOG(3, "wTvdGetPorch V_Default = 0x%02X\n", (DFT_V_PORCH));
            return DFT_V_PORCH;

        case SV_VPORCH_MIN:
            //return  6;
            #if TVD_NEW_VPORCH
            {
                return 0;
            }

            #else
            {
                return DFT_V_PORCH + CCVSYNC_MIN;
            }
            #endif

        case SV_VPORCH_MAX:
            #if TVD_NEW_VPORCH
            {
                return 0x7f;
            }
            #else
            {
                return  DFT_V_PORCH+CCVSYNC_MAX;
            }
            #endif

        case SV_HPORCH_MAX:
        case SV_HPORCH_MIN:
            return wDrvVideoPorchStd(bPath,bPorchType) +  MENU_AV_DELAY_H_OFFSET;

        default:
            return wDrvVideoPorchStd(bPath,bPorchType);
    }
}


void vTvdSetPorch(UINT8 bPorchType, UINT16 wValue)
{
    if(bPorchType==SV_HPORCH_CURRENT)
    {
        CRIT_STATE_T csState;
        csState = x_crit_start();
        LOG(3, "vTvdSetPorch H_Current = 0x%02X\n", (wValue));
        _su1UIDelay = wValue - DFT_H_PORCH;
        LOG(3, "vTvdSetPorch _su1UIDelay = %d\n", (_su1UIDelay));
        _svDrvTvdSetAVDelay(_rTvd3dStatus.bTvdMode);
        x_crit_end(csState);
    }
    else
    {
        CRIT_STATE_T csState;
        csState = x_crit_start();
        LOG(3, "vTvdSetPorch V_Current = 0x%02X\n", (wValue));
        _su1UIVPorchDelay = wValue - DFT_V_PORCH;
        LOG(3, "vTvdSetPorch _su1UIVPorchDelay = %d\n", (_su1UIVPorchDelay));
        _svDrvTvdSetVDelay(_rTvd3dStatus.bTvdMode);
        x_crit_end(csState);
#if 0
        switch(_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL_N:
            case SV_CS_PAL:
            case SV_CS_SECAM:
#if TVD_NEW_VPORCH

                //                     #if defined(CC_MT5387) || defined(CC_MT5363)
                //                     if(IS_5387_ECO()||IS_5363_ECO())
                //                     #else
                if(1)
                    //			#endif
                {
                    if(wValue>=wTvdGetPorch(0,SV_VPORCH_DEFAULT))
                    {
                        wValue-=wTvdGetPorch(0,SV_VPORCH_DEFAULT);
                    }
                    else
                    {
                        wValue=wTvdGetPorch(0,SV_VPORCH_DEFAULT)-wValue;
                        wValue+=0x40;
                    }

                    //LOG(9, "\n...............wValue is ..........%x \n",wValue);
                    vIO32WriteFldAlign(TG_0A,(UINT8)wValue,TG_SAF_ADJ);
                }
                else
#endif
                {
                    vIO32WriteFldAlign(TG_11,wValue,CCVSYNC0_P);
                    vIO32WriteFldAlign(TG_11,wValue+1,CCVSYNC1_P);
                }

                break;

            case SV_CS_PAL_M:
            case SV_CS_PAL_60:
            case SV_CS_NTSC358:
            case SV_CS_NTSC443:
#if TVD_NEW_VPORCH

                //                    #if defined(CC_MT5387) || defined(CC_MT5363)
                //			if(IS_5387_ECO()||IS_5363_ECO())
                //                     #else
                if(1)
                    //			#endif
                {
                    if(wValue>=wTvdGetPorch(0,SV_VPORCH_DEFAULT))
                    {
                        wValue-=wTvdGetPorch(0,SV_VPORCH_DEFAULT);
                    }
                    else
                    {
                        wValue=wTvdGetPorch(0,SV_VPORCH_DEFAULT)-wValue;
                        wValue+=0x40;
                    }

                    //LOG(9, "\n...............wValue is ..........%x \n",wValue);
                    vIO32WriteFldAlign(TG_0A,(UINT8)wValue,TG_SAF_ADJ);
                }
                else
#endif
                {
                    vIO32WriteFldAlign(TG_11,wValue,CCVSYNC0_N);
                    vIO32WriteFldAlign(TG_11,wValue,CCVSYNC1_N);
                }

                break;

            default:
                break;
        }
#endif
    }
}

/**
 * TVD query function, show status in CLI with debug level 3.
 *
 * @param void
 * @return void
 */
void vDrvTvdQueryStatus(void)
{
    LOG(0, "VPRES(%d) Mode(%ld) 525(%ld) 443(%ld) PHA(%ld) MMode(%d) NA(%ld) NA_Lvl(%ld) RF_Lvl(%d) \n",
           fgHwTvdVPres(),
           bHwTvdMode(),
           fgHwTvd525(),
           fgHwTvd443(),
           fgHwTvdPHALT(),
           fgHwTvdIsMMode(),
           bHwTvdNAState(),
           bHwTvdNRLevel(),
           _svDrvTvdGetNRLevel()
          );
    LOG(0, "MV(%d,%ld,%ld) CoCh(%ld) Scan(%d) VLock(%ld) VLen(%ld) HLock(%d) HsyncLock(%d) VCR_BV(%d)\n",
           fgHwTvdIsMacroVision(),fgHwTvdIsPSync(),fgHwTvdIsCSTRIPE(),
           fgHwTvdCoChannel(),
           IS_AUTOSEARCH(),
           fgHwTvdVLock(),
           wHwTvdAvgVlen()+1,
           fgHwTvdHLock(),
           fgHwTvdHsyncLock(),
           fgHwTvdVCRBV()
          );
    LOG(0, "HS(%ld) Trick(%ld) NONVCRTG(%d) VCR(%d) CNR(%d) CAGC(%ld) CKill(%ld) BLock(%ld) BLock4det(%d)\n",
           fgHwTvdHeadSwitch(),
           fgHwTvdTrick(),
           fgHwTvdVCR(),
           bhwVCRSetting,
           fgHwTvdCNRLow(),
           bHwTvdCAGC(),
           fgHwTvdCKill(),
           fgHwTvdBLock(),
           fgHwTvdBLock4DET()
          );
    LOG(0, "var_8field(%d) diff_stdFV(%d) line_cnt(%d) line_err(0x%X)\n",
        IO32ReadFldAlign(TG_STA_04, STDFV_VAR_8FIELD),
        IO32ReadFldAlign(TG_STA_04, ABS_DIFF_STDFV),
        IO32ReadFldAlign(TG_STA_05, STA_LCNT),
        wHwTvdLineErr()
        );
    LOG(0, "_sbUnsbableVCRBV = %d\n", _sbUnsbableVCRBV);
    LOG(0, "_su1LineLockCondition = %d\n", _su1LineLockCondition);
    LOG(0, "_swLLStatus_avg=(0x%04X)\n", _swLLStatus_avg);
#ifdef CC_SUPPORT_RECORD_AV
    LOG(0, "_su1MainSourceType = %d\n", _su1MainSourceType);
    LOG(0, "_su1SubSourceType = %d\n", _su1SubSourceType);
    LOG(0, "_rTvd3dStatus.eSourceType = %d\n", _rTvd3dStatus.eSourceType);
#endif
    #if TVD_NEW_HSYNCSLICER_SETTING
    LOG(0, "SyncLvAvg=0x%04X\n", _u2SyncLvAvg);
    #endif
}

//?? didn't used functions?
void SetTVDAGCstatus(void)
{
    //vIO32WriteFldAlign(DFE_03, _rTvd3dAGCInfo.bmanualAGC, AGC2_MAGC);
    vIO32WriteFldAlign(TG_00, _rTvd3dAGCInfo.minHWidth, MIN_HWIDTH);
    vIO32WriteFldAlign(TG_00, _rTvd3dAGCInfo.maxHWidth, MAX_HWIDTH);
    vIO32WriteFldAlign(CDET_01, _rTvd3dAGCInfo.thHigh, TH_HIGH);
    vIO32WriteFldAlign(CDET_01, _rTvd3dAGCInfo.thSuper, TH_SUPER);
}

//?? didn't used functions?
void GetTVDAGCstatus(void)
{
    _rTvd3dAGCInfo.minHWidth=IO32ReadFldAlign(TG_00, MIN_HWIDTH);
    _rTvd3dAGCInfo.maxHWidth=IO32ReadFldAlign(TG_00, MAX_HWIDTH);
    _rTvd3dAGCInfo.thHigh=IO32ReadFldAlign(CDET_01, TH_HIGH);
    _rTvd3dAGCInfo.thSuper=IO32ReadFldAlign(CDET_01, TH_SUPER);
}
void vTvdSetOverSampleEnable(UINT8 bmode)
{
    LOG(3,"[TVD_DBG_MSG] vTvdSetOverSampleEnable bonoff=%d \n",bmode);
#if 0

    if(bmode)
    {
        vIO32WriteFldAlign(VSRC_07, 0, TVD_27M_SEL);
        vIO32WriteFldAlign(SYS_00, 1, DATA_SYNC_DUAL);
    }
    else
    {
        vIO32WriteFldAlign(VSRC_07, 1, TVD_27M_SEL);
        vIO32WriteFldAlign(SYS_00, 0, DATA_SYNC_DUAL);
    }

#endif
}

UINT8 vTvdGetOverSampleEnable(void)
{
    return IO32ReadFldAlign(SYS_00,DATA_SYNC_DUAL);
}

/**
 * @brief Set Tvd3D mode detect counter
 * @param  bMDCnt: TVD Modes
 * @retval void
 */
void vTvdSetModeDetTime(UINT8 bMDCnt)
{
    bTvdCtrl(TCTL_MCNT, TC_ENABLE, bMDCnt);
}

/**
 * @brief Get Tvd3D mode detect counter
 * @param  void
 * @retval UINT8 TVD mode detect counter
 */
UINT8 bTvdGetModeDetTime(void)
{
    return bTvdCtrl(TCTL_MCNT, TC_GETVAL, 0);
}

UINT8 bTvdCtrl(UINT8 bItem,UINT8 bCmd,UINT32 bArg) reentrant
{
    UINT8 ret= SV_SUCCESS;

    if((bItem>=TCTL_MAX)||((bCmd&TC_INIT) && _srTvdCtrl[TCTL_BP_TC_INIT].en))
    {
        return SV_FAIL;    //bypass TC_INIT
    }

    bCmd&=(UINT8)~TC_INIT;

    switch(bCmd)
    {
        case TC_DISABLE:
            _srTvdCtrl[bItem].en=0;
            _srTvdCtrl[bItem].val=0;
            break;

        case TC_ENABLE:
            _srTvdCtrl[bItem].en=1;
            _srTvdCtrl[bItem].val=(UINT8) bArg;
            break;

        case TC_APPLY:
            bCmd=(_srTvdCtrl[bItem].en)?TC_ENABLE:TC_DISABLE;
            ret=(_srTvdCtrl[bItem].en)?SV_SUCCESS:SV_FAIL;
            break;

        case TC_GETEN:
            return _srTvdCtrl[bItem].en;

        case TC_GETVAL:
            return _srTvdCtrl[bItem].en?_srTvdCtrl[bItem].val:0;

        default:
            return SV_FAIL;
    };

    /* dedicate control for ENABLE/DISABLE command */
    switch(bItem)
    {
        case TCTL_TVD_RESET:
            _srTvdCtrl[bItem].en=0;
            vTvd3dReset();
            break;
#if 0

        case TCTL_RESET_DEFAULT:
            _srTvdCtrl[bItem].en=0;
            /* TODO */
            break;

        case TCTL_RECONNECT:
            _srTvdCtrl[bItem].en=0;
#if 0 //Need to do ??

            if(_rTvd3dStat.bIsMain)
            {
                vTvd3dConnect(SV_VP_MAIN,SV_ON);
            }
            else if(_rTvd3dStat.bIsPip)
            {
                vTvd3dConnect(SV_VP_PIP,SV_ON);
            }

            break;
#endif
#endif

        case TCTL_INPUTTYPE:
            if(bCmd==TC_ENABLE)
            {
                _srTvdCtrl[bItem].val=_rTvd3dStatus.eSourceType=(bArg<=SV_ST_SV)?bArg:bGetSignalType(_rTvd3dStat.bIsPip);
            }
            else
            {
                _rTvd3dStatus.eSourceType=bGetSignalType(_rTvd3dStat.bIsPip);
            }

            break;

        case TCTL_COUNTRY:
            if(bCmd==TC_ENABLE)
#ifdef NEW_COUNTRY_TYPE
                _srTvdCtrl[bItem].val=(bArg==COUNTRY_ATV_EU)?COUNTRY_ATV_EU:COUNTRY_ATV_US;

#else
                _srTvdCtrl[bItem].val=(bArg==COUNTRY_EU)?COUNTRY_EU:COUNTRY_US;
#endif
            else
#ifdef NEW_COUNTRY_TYPE
                u4AtvCountry = GetTargetCountryMask(COUNTRY_ATV_MASK);

            _srTvdCtrl[bItem].val=(UINT8)u4AtvCountry;
#else
#ifdef COUNTRY_FROM_EEPROM
                u1TargetCountryInfo = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;;

            if((u1TargetCountryInfo == COUNTRY_US)||
               (u1TargetCountryInfo == COUNTRY_JP)||
               (u1TargetCountryInfo == COUNTRY_TW))
            {
                _srTvdCtrl[bItem].val=COUNTRY_US;
            }
            else if(u1TargetCountryInfo == COUNTRY_EU)
            {
                _srTvdCtrl[bItem].val=COUNTRY_EU;
            }

#else
                _srTvdCtrl[bItem].val=GetTargetCountry();
#endif
#endif
            _srTvdCtrl[bItem].en=1; //always need enable country info
            break;

        case TCTL_DFTMODE:
            if(bCmd==TC_ENABLE)
            {
                if((_srTvdCtrl[bItem].val>=SV_CS_AUTO)||(_srTvdCtrl[bItem].val==SV_CS_UNSTABLE))
                {
                    //turn off
                    _srTvdCtrl[bItem].en=0;
                    _srTvdCtrl[bItem].val=0;
                }
                else
                {
                    _svDrvTvdSetDftMode(_srTvdCtrl[bItem].val);
                }
            }
            else	//reference country
#ifdef NEW_COUNTRY_TYPE
                _svDrvTvdSetDftMode((_srTvdCtrl[TCTL_COUNTRY].val==COUNTRY_ATV_EU)?SV_CS_PAL:SV_CS_NTSC358);

#else
                _svDrvTvdSetDftMode((_srTvdCtrl[TCTL_COUNTRY].val==COUNTRY_EU)?SV_CS_PAL:SV_CS_NTSC358);
#endif
            break;

        case TCTL_MDSTBPROT:
            if(bCmd!=TC_ENABLE)
            {
                vIO32WriteFldAlign(CDET_05, TVD_FORCE_UPD_MODE?SV_ON:SV_OFF, FORCE_UPD_MODE);
            }

            break;

        case TCTL_AGCPEDEN:
            if(bCmd==TC_ENABLE)
            {
                _bAgcPedEn = 1;
            }
            else
            {
                _bAgcPedEn = 0;
            }

            break;

        case TCTL_BP_TC_INIT:
        case TCTL_NTSCONLY:
        case TCTL_BP_ISR:
        case TCTL_BP_MODECHG:
            break;

        case TCTL_BP_MLOOP:
            if(bCmd==TC_ENABLE)
            {
                _u4BypassNptvMloop |= BYPASS_NPTV_TVD;
            }
            else
            {
                _u4BypassNptvMloop &= ~BYPASS_NPTV_TVD;
            }

            break;

        case TCTL_CKILL:
            if(bCmd==TC_ENABLE)
            {
                vIO32WriteFldAlign(CDET_05,1,MCKILL_EN);
                vIO32WriteFldAlign(CDET_05,_srTvdCtrl[bItem].val,MCKILL);
            }
            else
            {
                vIO32WriteFldMulti(CDET_05,P_Fld(0,MCKILL_EN)|P_Fld(0,MCKILL));
            }

            break;

        case TCTL_COLORSYSTEM:
#if 0 //Need to do ??
            if(bCmd==TC_ENABLE)
            {
                fgApiVideoColorSystem(_srTvdCtrl[bItem].val);
            }
            else
            {
                fgApiVideoColorSystem(SV_CS_AUTO);
            }

#endif
            break;

        case TCTL_SERRTYPE:
            if(bCmd==TC_ENABLE)
            {
                _srTvdCtrl[bItem].val=(bArg==SV_ON)?SV_ON:SV_OFF;
            }

            break;

        case TCTL_SERRCNT:
            break;

        case TCTL_ACODE:
            break;

        case TCTL_DCODE:
            break;

        case TCTL_CAGC:
            break;

        case TCTL_REG_SAVE_RESTORE:
            break;

        case TCTL_MCNT:
            break;

        default:
        case TCTL_NOISELVL:
        case TCTL_VCR:
        case TCTL_COCH:
            break;
    }

    return ret;
}

/**
 * @brief TVD Init function
 *
 * This function only be called once when system start up
 *
 * @param None
 * @return None
 */
void vTvd3dInit(void)
{
//MT5881 CVBS 108 Clock Setting
#if defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5882)
    vIO32WriteFldAlign(OMUX_VDOIN_CLK_SEL, 0x1, FLD_VDOIN_AD_CVBSADC_CKOUTA_SEL_108);
    vIO32WriteFldAlign(OMUX_VDOIN_CLK_SEL, 0x1, FLD_VDOIN_AD_CGA_SEL_108);
    vIO32WriteFldAlign(OMUX_VDOIN_CLK_SEL, 0x1, FLD_VDOIN_AD_CVBSADC_CKOUTB_SEL_108);
    vIO32WriteFldAlign(OMUX_VDOIN_CLK_SEL, 0x1, FLD_VDOIN_AD_CGB_SEL_108);
    vIO32WriteFldAlign(VSRC_02, 0x1, CVBS_ADC_IDX_SEL);
    vIO32WriteFldAlign(VSRC_09, 0x1, CVBS_ADC_IDX_INV);
#endif

    //KI gain
    vIO32WriteFldMulti(CTG_03,
                       P_Fld(CTG_KI_GAIN0, CTG_KI0) |
                       P_Fld(CTG_KI_GAIN1, CTG_KI1) |
                       P_Fld(CTG_KI_GAIN2, CTG_KI2));
    vIO32WriteFldMulti(CTG_03,
                       P_Fld(CTG_KI_GAIN3, CTG_KI3) |
                       P_Fld(CTG_KI_GAIN4, CTG_KI4) |
                       P_Fld(CTG_KI_GAIN5, CTG_KI5));
    //KP gain
    vIO32WriteFldMulti(CTG_04,
                       P_Fld(CTG_KP_GAIN0,CTG_KP0)|
                       P_Fld(CTG_KP_GAIN1,CTG_KP1)|
                       P_Fld(CTG_KP_GAIN2,CTG_KP2));
    vIO32WriteFldMulti(CTG_04,
                       P_Fld(CTG_KP_GAIN3,CTG_KP3)|
                       P_Fld(CTG_KP_GAIN4,CTG_KP4)|
                       P_Fld(CTG_KP_GAIN5,CTG_KP5));
#ifdef NEW_COUNTRY_TYPE
    u4AtvCountry = GetTargetCountryMask(COUNTRY_ATV_MASK);

    if(u4AtvCountry == COUNTRY_ATV_US)
    {
        bTvdCtrl(TCTL_COUNTRY,TC_INIT|TC_ENABLE,COUNTRY_ATV_US);    /* inital country*/
    }
    else
    {
        bTvdCtrl(TCTL_COUNTRY,TC_INIT|TC_ENABLE,COUNTRY_ATV_EU);    /* inital country*/
    }

#else
#ifdef COUNTRY_FROM_EEPROM
    u1TargetCountryInfo = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;;

    if((u1TargetCountryInfo == COUNTRY_US)||
       (u1TargetCountryInfo == COUNTRY_JP)||
       (u1TargetCountryInfo == COUNTRY_TW))
    {
        bTvdCtrl(TCTL_COUNTRY,TC_INIT|TC_ENABLE,COUNTRY_US);/* inital country*/
    }
    else if(u1TargetCountryInfo == COUNTRY_EU)
    {
        bTvdCtrl(TCTL_COUNTRY,TC_INIT|TC_ENABLE,COUNTRY_EU);/* inital country*/
    }

#else
    bTvdCtrl(TCTL_COUNTRY,TC_INIT|TC_ENABLE,GetTargetCountry());/* inital country*/
#endif
#endif
#if TVD_NEW_VPORCH
    {
        vIO32WriteFldAlign(TG_0A,SV_ON,TG_CCVADJ_EN);
#if TVD_SUPPORT_DRVCUST_DEFINE
		vIO32WriteFldAlign(TG_0A, DRVCUST_OptGet(eTVDMenu_Av_V_Delay), TG_SAF_ADJ);  //adjust default picture position
#else
        vIO32WriteFldAlign(TG_0A,0x0,TG_SAF_ADJ); //adjust default picture position
#endif
        vIO32WriteFldAlign(TG_0A,SV_ON,CCV_F_LATCH_AUTO);
    }
#else
    vIO32WriteFldAlign(TG_0A,SV_OFF,TG_CCVADJ_EN);
#endif

    vIO32WriteFldMulti(TG_11,\
                       P_Fld(CCVSYNC_P_DEFAULT+1+TVD_VPORCH_TUNE,CCVSYNC1_P)|P_Pad0(2, 30)|\
                       P_Fld(CCVSYNC_P_DEFAULT+TVD_VPORCH_TUNE,CCVSYNC0_P)|P_Pad0(2, 22)|\
                       P_Fld(CCVSYNC_N_DEFAULT+TVD_VPORCH_TUNE,CCVSYNC1_N)|P_Pad0(2, 14)|\
                       P_Fld(CCVSYNC_N_DEFAULT+TVD_VPORCH_TUNE,CCVSYNC0_N)|P_Pad0(2, 6));
    bTvdCtrl(TCTL_NTSCONLY,TC_INIT|(DRVCUST_OptGet(eTVDSupportNTSCOnly)?TC_ENABLE:TC_DISABLE),1);
    bTvdCtrl(TCTL_MDSTBPROT,TC_INIT|TC_ENABLE,1); //Enable Force Updated
    bTvdCtrl(TCTL_AGCPEDEN,TC_INIT|(DRVCUST_OptGet(eTVDSupportPedEn)?TC_ENABLE:TC_DISABLE),1);

    if(bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)==1)
    {
        _rTvd3dStatus.fgIs525 = 0x1;
    }
    else
    {
#ifdef NEW_COUNTRY_TYPE

        if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_ATV_EU)
#else
        if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU)
#endif
        {
            _rTvd3dStatus.fgIs525 = 0x00;
        }
        else
        {
            _rTvd3dStatus.fgIs525 = 0x01;
        }
    }

#if TVD_FRS_FOR_NONSTDSIGNAL
    _svDrvTvdRstNSTStatus(_rTvd3dStatus.fgIs525, &_rTvd3dNSTDStatus);
#endif

    if(bTvdCtrl(TCTL_DFTMODE,TC_GETEN,0))
    {
        _svDrvTvdSetDftMode(bTvdCtrl(TCTL_DFTMODE,TC_GETVAL,0));
    }
    else
    {
#ifdef NEW_COUNTRY_TYPE

        if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_ATV_EU)
#else
        if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU)
#endif
        {
            _svDrvTvdSetDftMode(SV_CS_PAL);
        }
        else
        {
            _svDrvTvdSetDftMode(SV_CS_NTSC358);
        }
    }

	
#if TVD_MANUAL_TUNER_SCAN
	_sfgManualScanadj = FALSE;
	_su1VlockRatio= TVDLOCK_RATIO;
	_su1VpresGain = 0xFF;
#endif

    //moved from connect
    //DFE initial.
    vIO32WriteFldAlign(DFE_02, 0x1, Y4V_SHN_BW);
    vIO32WriteFldAlign(DFE_01, 0x78, ACTIVE_START);	//adjust peak detection active start point
    vIO32WriteFldAlign(DFE_00, DFE_CLAMP_START, CLAMP_START);
    vIO32WriteFldAlign(DFE_0E, 0x1, NR_DET_VPRES_SEL);
    vIO32WriteFldAlign(DFE_0E, 0x0, VPRES4PIC_MODE); //DFE referenced VPRES3
    if(!DRVCUST_OptGet(eTVDUseVPres4))
    {
        vIO32WriteFldAlign(DFE_0E, 0x0, VPRES4TVD_MODE);
    }
    vIO32WriteFldAlign(DFE_02, 0x1, BLV_LIM_SEL);// Original Back-porch blank level
#if TVD_COCH_FLICKER
    vIO32WriteFldAlign(DFE_13, DFE_CLAMP_ERROR_SUM_TH, CLAMP_CURRENT_SUM_THR);
#endif
    vIO32WriteFldAlign(DFE_0E, 0x30,DCLAMP_UP_LIM);//new reg for 8223
    vIO32WriteFldMulti(DFE_09,
                       P_Fld(SV_ON, CLAMP_HEADSWITCH_ADAP)|
                       P_Fld(SV_OFF, CLAMP_VBI_FORBID)|
                       P_Fld(SV_ON, CLAMP_VBI_NOISE_EN));
    vIO32WriteFldAlign(DFE_10, DFE_VPRES2_HSYNC_W_LLIM, VP2_HSYNC_W_LLIM);// VP2_VSYNC_W_ULIM,VP2_VSYNC_W_LLIM,VP2_HSYNC_W_ULIM,VP2_HSYNC_W_LLIM
    vIO32WriteFldAlign(DFE_21, SV_OFF, AGC_PED_EN);
    vIO32WriteFldAlign(DFE_03, DFE_NORMAL_LOOP_GAIN, AGC2_NORMAL_LOOP_GAIN);// Apply normal loop gain to avoid flag flickering.
    vIO32WriteFldAlign(DFE_05, DFE_PEAK_STEP, AGC2_PEAK_STEP);
    vIO32WriteFldAlign(DFE_05, DFE_AGAIN_STEP, AGC2_ACODE_STEP);//AGC2 gain steps
    vIO32WriteFldAlign(DFE_16, DFE_ABV_UP_TH, ABV_UP_TH);
    //5387 Add
    vIO32WriteFldAlign(DFE_19, DFE_VDET_MBLK, VDET_MBLK);//MT5387 TODO:IC setting is different from FPGA 0xE0
    vIO32WriteFldAlign(DFE_18, SV_ON, MULTIV_DET_EN);//MT5387
    vIO32WriteFldAlign(DFE_23, DFE_MULTIV_DET_CNTH, MULTIV_DET_CNTH);//MT5387
    vIO32WriteFldAlign(DFE_23, DFE_RAWV_WDTH, RAWV_WDTH);//MT5387 TODO 0x20 or 0x16
    vIO32WriteFldAlign(DFE_12, DFE_VCNT_TH, VCNT_TH);//MT5387
    vIO32WriteFldAlign(DFE_17, DFE_MAXMINMIN_TH, MAXMINMIN_TH);//MT5387: Default:0x40(Qlab airplane flutter 0dB will v_unlock)
    //5396 Add New Feature
    vIO32WriteFldAlign(DFE_24, SV_OFF, AVG_SYNC_EN);//Enable Average Sync Mode
    vIO32WriteFldAlign(DFE_24, SV_OFF, SYNC_MASK_EN);//Disable SYNC MASK Enable
    vIO32WriteFldAlign(DFE_24, SV_OFF, SYNC_MASK_INV);//Disable Sync Mask Inv
    vIO32WriteFldAlign(DFE_24, SV_OFF, PRBS_EN);//Disable PRBS Enable
    vIO32WriteFldAlign(DFE_24, 0x0, SYNC_MASK_WIN_ST);//Setting Default MASK Windows Start Value
    vIO32WriteFldAlign(DFE_24, 0x0, SYNC_MASK_WIN_END);//Setting Default MASK Windows End Value
    //CDET initial
    #if TVD_FORCE_NO_CKILL
    vIO32WriteFldAlign(CDET_05, SV_ON, MCKILL_EN);
    vIO32WriteFldAlign(CDET_05, SV_OFF, MCKILL);
    #endif
    
    vIO32WriteFldAlign(CDET_09, TVD_CAGC_LIM_GAIN, CAGC_LIM_GAIN);
#if TVD_8222_NONSTANDARD
    vIO32WriteFldAlign(CDET_00, SV_OFF, MODE_CKILL_EN);
    vIO32WriteFldMulti(CDET_0C,
                       P_Fld(SV_OFF,BSWTGT_IGNR)| //for chroma flicker
                       P_Fld(SV_ON,DET443_NB_EN)|
                       P_Fld(SV_ON,PHALT_NB_EN));
#else
    vIO32WriteFldAlign(CDET_00, SV_ON, MODE_CKILL_EN);
    vIO32WriteFldMulti(CDET_0C,
                       P_Fld(SV_ON,DET443_NB_EN)|
                       P_Fld(SV_ON,PHALT_NB_EN));
#endif
    vIO32WriteFldAlign(CDET_01, CDET_LOW_NA_TH, LOW_NA_TH);
    vIO32WriteFldMulti(CDET_03,
                       P_Fld(CDET_DET443_UP,DET443_UP)|
                       P_Fld(CDET_DET443_DN,DET443_DN));
    vIO32WriteFldMulti(CDET_03,
                       P_Fld(CDET_SCF_STD_UP,SCF_STD_UP)|
                       P_Fld(CDET_SCF_STD_DOWN,SCF_STD_DN));
    vIO32WriteFldAlign(CDET_04, CDET_SCF_TH, SCF_TH);
    vIO32WriteFldAlign(CDET_04, CDET_DET443_TH, DET443_TH);
    vIO32WriteFldAlign(CDET_04, 0x23, SCF_NR_TH);//SCF_NR_TH,HW default=0x18
    vIO32WriteFldAlign(CDET_05, CDET_BURST_THRESHOLD_SEL, BURST_THRESHOLD_SEL);
#if TVD_SUPPORT_DRVCUST_DEFINE
	vIO32WriteFldMulti(CDET_06,
                       P_Fld(DRVCUST_OptGet(eTVD_SCkill_UP),SCKILL_UP)|
                       P_Fld(DRVCUST_OptGet(eTVD_SCkill_DN),SCKILL_DN));
#else
    vIO32WriteFldMulti(CDET_06,
                       P_Fld(CDET_SCKILL_UP,SCKILL_UP)|
                       P_Fld(CDET_SCKILL_DN,SCKILL_DN));
#endif
    vIO32WriteFldAlign(CDET_08, CDET_PAL_SW_GAIN, PAL_SW_GAIN);
    #if TVD_PHALT_SEL_PATCH
    vIO32WriteFldAlign(CDET_08, SV_ON, PHALT_SEL);//MT5387 4lines
    #else
    vIO32WriteFldAlign(CDET_08, SV_OFF, PHALT_SEL);//MT5387 4lines
    #endif
    vIO32WriteFldAlign(CDET_05, SV_ON, CKILL_PHALT); //reset 443/358 up/down counter
    vIO32WriteFldAlign(CDET_0F, CDET_SB4CTG_TH, SB4CTG_TH);//for VCR issue
    vIO32WriteFldAlign(CDET_08, 0x1, MDET_ALL_RDY);//MT5387 FPGA
    vIO32WriteFldAlign(CDET_05, SV_ON, CAGC_LIM_EN);
    vIO32WriteFldAlign(CDET_0B, CDET_CAGC_PEAK_GAIN, CAGC_PEAK_GAIN);
    vIO32WriteFldAlign(CDET_0B, CDET_CAGC_LOOP_GAIN, CAGC_LOOP_GAIN);
    vIO32WriteFldAlign(CDET_0C, CDET_CAGC_MAX, CAGC_MAX);
    vIO32WriteFldAlign(CDET_0D, CDET_SMALL_SYNC_TH, SMALL_SYNC_TH);
    vIO32WriteFldAlign(CDET_0E, CDET_V625_LIMIT, V625_LIMIT); // set same value as 8226.
    //CAGC status change to old cagc, YJ will change hw to old cagc CAGC_STA_SEL
    vIO32WriteFldAlign(CDET_08, 1, CAGC_STA_SEL); //CAGC_STA_SEL
    // Set CAGC bound for ctrl path.
    _svDrvTvdSetCAGCBound4Ctrl(PAL_CAGC_BOUND);
    vIO32WriteFldAlign(CDET_0B, 0, CAGC_DATA_PATH_SEL);
    vIO32WriteFldAlign(CDET_0B, 0, CAGC_CTL_PATH_SEL);
    vIO32WriteFldAlign(CDET_06, CDET_KBURST_PAL, KBURST);
    vIO32WriteFldAlign(CDET_08, CDET_CAGC_PEAK_TC, CAGC_PEAK_TC);
    vIO32WriteFldAlign(CDET_08, CDET_CAGC_PEAK_STEP, CAGC_PEAK_STEP);
#if TVD_BURST_ONOFF_TEST_WA
    vRegWriteFldAlign(CDET_0D, CDET_CAGC_PEAK_ERR_GAIN, CAGC_PEAK_ERR_GAIN);
#endif
    //TG initial
    vIO32WriteFldAlign(TG_0C, SV_OFF, LLOCK_FINE_LIM_EN);
    vIO32WriteFldAlign(TG_12, SV_ON, TOPFIELD_POLARITY); //same as MT5360A
    //MT5387 FPGA 0xbf  for bpres off->on,bsync auto counter
    vIO32Write4B(TG_18, 0xbf488488);   //SCART_03, for KIP_FORCE Gain. china#35 ???
    vIO32WriteFldAlign(TG_05, TG_VHYST_EN, VHYST_EN);
    vIO32WriteFldAlign(TG_0B, SV_OFF, AUTO_AVDELAY);
    vIO32WriteFldAlign(TG_08, SV_OFF, VFIR_EN);
    vIO32WriteFldAlign(TG_08, 0, VFIR_SEL);       // 2 fields
 #if TVD_SUPPORT_DRVCUST_DEFINE
 	vIO32WriteFldAlign(TG_0A, DRVCUST_OptGet(eTVDMenu_Av_V_Delay), TG_SAF_ADJ); // adjust default picture position
 #else
    vIO32WriteFldAlign(TG_0A, 0x0, TG_SAF_ADJ); // adjust default picture position
 #endif
    vIO32WriteFldAlign(TG_14, 0x0, LLOCK_S0_THR);//cntseng ask MY lin 0x34
    vIO32WriteFldAlign(TG_17, TG_FRAME_STDFH_TH_STD, FRAME_STDFH_TH);
    vIO32WriteFldAlign(TG_0F, 1, TKMODE_SW);
    vIO32WriteFldAlign(TG_15, SV_ON, NHLOCK_EN);
    //5396 Add New Feature
    vIO32WriteFldAlign(TG_19, SV_OFF, IIR_SOL_POS_SEL);//Disable IIR SOL Select
    vIO32WriteFldAlign(TG_19, 0x31, IIR_SOL_POS_LLIM);//Setting Position Limit Value
    vIO32WriteFldAlign(TG_19, 0x3D, IIR_SOL_POS_ULIM);//Setting Position Upper Value
#if defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5882)
	vIO32WriteFldAlign(TG_04, 1, TGLIN2_SEL); // Remy Video will not shift during adjust Fh Offset 
#endif
    //CTG initial
    vIO32WriteFldAlign(CTG_03, SV_ON,CNRDET_EN);
    vIO32WriteFldAlign(CTG_03, SV_ON, CTG_BSTART_FORCE); //??
#if TVD_8222_NONSTANDARD
    vIO32WriteFldAlign(CTG_00, SV_OFF, BUKRSTSD_EN); /// for FSC burst Ckill improve
#endif
    vIO32WriteFldAlign(CTG_00, CTG_BSYNC_TYPE, BSYNC_TYPE);
#if defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860) || defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5882)
	vIO32WriteFldMulti(CTG_05,
                       P_Fld(SV_ON, BUNLOCK_CKILL)|
                       P_Fld(2,DEMOD_LPFSEL));// 0: 1.44 Mhz /1: 0.97Mhz  Bandwidth
    vIO32WriteFldAlign(CTG_05, 0x1, DEMOD_NEW_LPFSEL_EN);//Open Chroma Filter LPF6         
#else
    vIO32WriteFldMulti(CTG_05,
                       P_Fld(SV_ON, BUNLOCK_CKILL)|
                       P_Fld(0,DEMOD_LPFSEL));// 0: 1.44 Mhz /1: 0.97Mhz  Bandwidth
    vIO32WriteFldAlign(CTG_05, 0x0, DEMOD_NEW_LPFSEL_EN);//Open Chroma Filter LPF6 
#endif
    vIO32WriteFldMulti(CTG_05,
                       P_Fld(SV_OFF,DETEN_SEL)|//DETEN_FORCE = 1; set 8 pixels cause of MV
                       P_Fld(CTG_BP_OFFSET,BP_OFFSET));
    vIO32WriteFldMulti(CTG_05,
                       P_Fld(SV_OFF,CTG_SV_SECAM_EN)|
                       P_Fld(SV_ON,DETEN_FORCE));//MT5387 MY
    vIO32WriteFldAlign(CTG_07, 0x1, CTG_SECAM_PHASE_SEL);//MT5387
    //YJ for PAL-N signal lock in PAL mode, will set by HW by def.
    vIO32WriteFldAlign(CTG_07, 1, CTG_4DET_SUM_SEL); //SUM8SEL
#if CAGC_PEAK_CNR_ADJ
    vIO32WriteFldAlign(CTG_05, SV_OFF, CNR_BSTADJ_EN);
    vIO32WriteFldAlign(CTG_03, 1, SUM_DIV_SEL);
#endif
    //SECAM
    vIO32WriteFldAlign(SECAM_02, SCM_SEED_SEL, SEED_SEL);// MT5381TVDPort SECAM Port SEED_SEL
    vIO32WriteFldMulti(SECAM_01,
                       P_Fld(SV_ON,BPRES_CKILL_EN)|
                       P_Fld(SV_ON,VMASK_CKILL_EN)|
                       P_Fld(SV_ON,TVDCKILL_CKILL_EN)|
                       P_Fld(SV_ON,BLENDING_SEL)|
                       P_Fld(SCM_PDIFF_THR,PDIFF_THR)|
                       P_Fld(SCM_MSTART_SEL,MSTART_SEL)|
                       P_Fld(SCM_CSEL,TVD_CSEL)|
                       P_Fld(SCM_YSEL,TVD_YSEL)
                      );
#ifdef CC_MT5396

    if(_rTvd3dStatus.eSourceType==SV_ST_SV)
    {
        vIO32WriteFldAlign(SECAM_02, SCM_MATCH_PAL_DELAY_SV_FOR_C, MATCH_PAL_DELAY_FOR_C);
        vIO32WriteFldAlign(SECAM_02, SCM_MATCH_PAL_DELAY_SV_FOR_Y, MATCH_PAL_DELAY_FOR_Y);
    }
    else
#endif
    {
        vIO32WriteFldAlign(SECAM_02, SCM_MATCH_PAL_DELAY_FOR_C, MATCH_PAL_DELAY_FOR_C);
        vIO32WriteFldAlign(SECAM_02, SCM_MATCH_PAL_DELAY_FOR_Y, MATCH_PAL_DELAY_FOR_Y);
    }

    //SECAM Correct U/V Scale.
    vIO32WriteFldMulti(SECAM_00,
                       P_Fld(SCM_VSCALE,VSCALE)|
                       P_Fld(SCM_USCALE,USCALE));
    //change SVF VPRES2 to VPRES1
    vIO32WriteFldAlign(SVF_00, 0x0, SVF_VPRES_SEL);
    vIO32WriteFldMulti(SECAM_06,
                       P_Fld(SCM_STDVSTB_TH, STDVSTB_TH)|
                       P_Fld(SCM_HLOCKSTB_TH, HLOCKSTB_TH)|
                       P_Fld(SCM_VLOCKSTB_TH, VLOCKSTB_TH)|
                       P_Fld(SCM_SCFSTB_TH, SCFSTB_TH));
    //VSRC
    vIO32WriteFldAlign(VSRC_08, VSRC_COCH_SLV, COCH_SLV); //for COCH field signal upper screen Hjitter
#if TVD_SET_ENABLED_CS
    _rTvEnabledCS.u1TvdEnPALN = TRUE;
    _rTvEnabledCS.u1TvdEnPAL = TRUE;
    _rTvEnabledCS.u1TvdEnPALM = TRUE;
    _rTvEnabledCS.u1TvdEnNTSC358 = TRUE;
    _rTvEnabledCS.u1TvdEnSECAM = TRUE;
    _rTvEnabledCS.u1TvdEnPAL60 = TRUE;
    _rTvEnabledCS.u1TvdEnNTSC443 = TRUE;
    _rAvEnabledCS.u1TvdEnPALN = TRUE;
    _rAvEnabledCS.u1TvdEnPAL = TRUE;
    _rAvEnabledCS.u1TvdEnPALM = TRUE;
    _rAvEnabledCS.u1TvdEnNTSC358 = TRUE;
    _rAvEnabledCS.u1TvdEnSECAM = TRUE;
    _rAvEnabledCS.u1TvdEnPAL60 = TRUE;
    _rAvEnabledCS.u1TvdEnNTSC443 = TRUE;
#endif

    #if TVD_SUPPORT_704_DE
    vIO32WriteFldAlign(TG_0C, 0xf0, HDATA_ADJ);//send 704 DE
    #endif
}

#ifdef CC_SUPPORT_RECORD_AV
BOOL bTvd3dIsConnected(void)
{
    if(_sbConnectedByMain || _sbConnectedBySub)
        return TRUE;
    else
        return FALSE;
}
#endif

#if TVD_CLI_SUPPORT
void vTvdSetCLINfy(TVD_PFN_CLI_NFY pfnCliNfy)
{
    _pfnTVDCLINotify = pfnCliNfy;
}

void vTVDAVDelay(UINT8 bDelay)        // For CLI
{
    _svDrvTvdSetMenuAvDelay(_rTvd3dStatus.bTvdMode, bDelay);
}

void vTvd3dSetFixAgc(UINT8 bOnOff)
{
    _sbIsTVDFixAgc = bOnOff;

    if(bOnOff)
    {
        //MT5387 FPGA
        //TODO
        /*
        9 bit
        [31]INTERP_DAGC_SEL
        [30]TGYI_DAGC_SEL
        [29]TDC_DAGC_SEL
        [28]SVF_DAGC_SEL
        [27]CDET_DAGC_SEL
        [26]MDET_DAGC_SEL
        [25]VBI_DAGC_SEL
        [24]TG_DAGC_SEL 0: Fix-gained;1: Digital-gained
        [23]CTG_DAGC_SEL 0: Fix-gained;1: Digital-gained
        */
#if TVD_USE_NEW_DAGC
        vTvdSetAGCMode(E_MENU_AGAIN);
        vIO32WriteFldAlign(DFE_03, SV_ON, MANUAL_DAGC_EN);//MT5387 FPGA
        vIO32WriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL, FIXAGC_DAGCSEL);
#else
        vTvdSetAGCMode(E_MENU_GAIN);
        vIO32WriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);//MT5387 FPGA
        vIO32WriteFldAlign(DFE_18, 0, FIXAGC_DAGCSEL);
#endif
    }
    else
    {
        vTvdSetAGCMode(E_AUTO_GAIN);
        vIO32WriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);//MT5387 FPGA
        vIO32WriteFldAlign(DFE_18, DFE_AUTOAGC_DAGCSEL, FIXAGC_DAGCSEL);
    }
}
void vTVD3dAGCPedEn(UINT8 bOnOff)
{
    if(bOnOff)
    {
        _bAgcPedEn = 1;

        switch(_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL_M:
            case SV_CS_NTSC358:
                vIO32WriteFldAlign(CTG_07, _wTargetBlk+DFE_BLANK_OFFSET_NTSCJ, BLKLVL);
	#if TVD_SUPPORT_DRVCUST_DEFINE
		_svDrvTvdSetCAGCBound4Data(NTSC_IP_MID_B, NTSC_IP_MID_A, DRVCUST_OptGet(eTVDNTSC_Ip_Tgt_B), DRVCUST_OptGet(eTVDNTSC_Ip_Tgt_A));
	#else
                _svDrvTvdSetCAGCBound4Data(NTSC_IP_MID_B, NTSC_IP_MID_A, NTSC_IP_TGT_B, NTSC_IP_TGT_A);
	#endif
                break;

            default:
                break;
        }
    }
    else
    {
        _bAgcPedEn = 0;

        switch(_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL_M:
            case SV_CS_NTSC358:
                vIO32WriteFldAlign(CTG_07, _wTargetBlk+DFE_BLANK_OFFSET_NTSC, BLKLVL);
	#if TVD_SUPPORT_DRVCUST_DEFINE
		_svDrvTvdSetCAGCBound4Data(NTSC_J_IP_MID_B, NTSC_J_IP_MID_A, DRVCUST_OptGet(eTVDNTSC_J_Ip_Tgt_B), DRVCUST_OptGet(eTVDNTSC_J_Ip_Tgt_A));
	#else
                _svDrvTvdSetCAGCBound4Data(NTSC_J_IP_MID_B, NTSC_J_IP_MID_A, NTSC_J_IP_TGT_B, NTSC_J_IP_TGT_A);
	#endif
                break;

            default:
                break;
        }
    }
}

/**
 * @brief TVDecoder pattern generator
 *
 * @param bOnOff  0: disable; 1: enable
 * @return void.
 *
 */
void vTvd3dSetPatGen(UINT8 bOnOff)
{
    if(bOnOff)
    {
        vIO32WriteFldAlign(TG_0B, SV_ON, TVD_PATGEN_EN);
    }
    else
    {
        vIO32WriteFldAlign(TG_0B, SV_OFF, TVD_PATGEN_EN);
    }
}

void vTvd3dSetHNChChgMCntTh(UINT8 bTh)
{
#if TVD_HN_CHCHG_WA
    //_sbHNChChgMCntTh = bTh;
    _sbHNChChgNRTh = bTh;
#endif
    return ;
}

void vTvd3dSetPhaltWaOnOff(UINT8 bEnable)
{
#if TVD_PHALT_WA
    _fgEnablePhaltWa = bEnable;
#endif
}

void vTvdSetSFSetting(UINT32 u4value)
{
    //vIO32Write4B(SECAM_01, u4value  & 0x07e7ffff);
    vIO32WriteFldAlign(SECAM_01, u4value&0x07FFFFFF, Fld(27, 0, AC_MSKDW));
    return;
}

#ifdef CC_SUPPORT_STR
void TVD_pm_suspend(void)
{
    LOG(3,"vTvd3d_pm_suspend()\n");
    /*
    LOG(0, "_rTvd3dStatus.wLine_err=%d\n", _rTvd3dStatus.wLine_err);
    LOG(0, "_rTvd3dStatus.bCAGC=%d\n", _rTvd3dStatus.bCAGC);
    LOG(0, "_rTvd3dStatus.bTvdMode=%d\n", _rTvd3dStatus.bTvdMode);
    LOG(0, "_rTvd3dNSTDStatus.wHTotal=%d\n", _rTvd3dNSTDStatus.wHTotal);
    LOG(0, "_rTvd3dNSTDStatus.wVTotal=%d\n", _rTvd3dNSTDStatus.wVTotal);
    LOG(0, "_rTvd3dNSTDStatus.wVTotalDiff=%d\n", _rTvd3dNSTDStatus.wVTotalDiff);
    */    
    x_memcpy((void *)pm_save_cdet_regs,(void *)CDET_00,sizeof(pm_save_cdet_regs));
    x_memcpy((void *)pm_save_dfe1_regs,(void *)DFE_00,sizeof(pm_save_dfe1_regs));
    x_memcpy((void *)pm_save_dfe2_regs,(void *)DFE_20,sizeof(pm_save_dfe2_regs));
    x_memcpy((void *)pm_save_dfe3_regs,(void *)DFE_SCART_02, sizeof(pm_save_dfe3_regs));
    x_memcpy((void *)pm_save_dfe4_regs,(void *)DFE_24, sizeof(pm_save_dfe4_regs));
    x_memcpy((void *)pm_save_secam_regs,(void *)SECAM_00,sizeof(pm_save_secam_regs));
    x_memcpy((void *)pm_save_vsrc1_regs,(void *)VSRC_00,sizeof(pm_save_vsrc1_regs));
    x_memcpy((void *)pm_save_vsrc2_regs,(void *)VSRC_02,sizeof(pm_save_vsrc2_regs));
	x_memcpy((void *)pm_save_vsrc3_regs,(void *)VSRC_07,sizeof(pm_save_vsrc3_regs));
	x_memcpy((void *)pm_save_vsrc4_regs,(void *)VSRC_09,sizeof(pm_save_vsrc4_regs));
    x_memcpy((void *)pm_save_tg1_regs,(void *)TG_00,sizeof(pm_save_tg1_regs));
    x_memcpy((void *)pm_save_tg2_regs,(void *)TG_18,sizeof(pm_save_tg2_regs));
    x_memcpy((void *)pm_save_tg3_regs,(void *)TG_19,sizeof(pm_save_tg3_regs));
    x_memcpy((void *)pm_save_ctg1_regs,(void *)CTG_00,sizeof(pm_save_ctg1_regs));
    x_memcpy((void *)pm_save_ctg2_regs,(void *)CTG_08,sizeof(pm_save_ctg2_regs));
    x_memcpy((void *)pm_save_svf_regs,(void *)SVF_02,sizeof(pm_save_svf_regs));
	x_memcpy((void *)pm_save_ctg_dering_regs,(void *)CTG_DERING,sizeof(pm_save_ctg_dering_regs));
	x_memcpy((void *)pm_save_tg_dering_regs,(void *)TG_DERING,sizeof(pm_save_tg_dering_regs));
	x_memcpy((void *)pm_save_aaf_dering_regs,(void *)AAF_DERING,sizeof(pm_save_aaf_dering_regs));
	x_memcpy((void *)pm_save_oth_dering_regs,(void *)DERING_OTHER_SETTING,sizeof(pm_save_oth_dering_regs));
	x_memcpy((void *)pm_save_omux_sel_regs,(void *)OMUX_VDOIN_CLK_SEL,sizeof(pm_save_omux_sel_regs));
}

void TVD_pm_resume(void)
{
    //restore osd reg
    LOG(3, "vTvd3d_pm_resume()\n");
    /*
    LOG(0, "_rTvd3dStatus.wLine_err=%d\n", _rTvd3dStatus.wLine_err);
    LOG(0, "_rTvd3dStatus.bCAGC=%d\n", _rTvd3dStatus.bCAGC);
    LOG(0, "_rTvd3dStatus.bTvdMode=%d\n", _rTvd3dStatus.bTvdMode);
    LOG(0, "_rTvd3dNSTDStatus.wHTotal=%d\n", _rTvd3dNSTDStatus.wHTotal);
    LOG(0, "_rTvd3dNSTDStatus.wVTotal=%d\n", _rTvd3dNSTDStatus.wVTotal);
    LOG(0, "_rTvd3dNSTDStatus.wVTotalDiff=%d\n", _rTvd3dNSTDStatus.wVTotalDiff);
    */   
    x_memcpy((void *)CDET_00,(void *)(pm_save_cdet_regs),sizeof(pm_save_cdet_regs));
    x_memcpy((void *)DFE_00,(void *)(pm_save_dfe1_regs),sizeof(pm_save_dfe1_regs));
    x_memcpy((void *)DFE_20,(void *)(pm_save_dfe2_regs),sizeof(pm_save_dfe2_regs));
    x_memcpy((void *)DFE_SCART_02,(void *)(pm_save_dfe3_regs),sizeof(pm_save_dfe3_regs));
    x_memcpy((void *)DFE_24,(void *)(pm_save_dfe4_regs),sizeof(pm_save_dfe4_regs));
    x_memcpy((void *)SECAM_00,(void *)(pm_save_secam_regs),sizeof(pm_save_secam_regs));
    x_memcpy((void *)VSRC_00,(void *)(pm_save_vsrc1_regs),sizeof(pm_save_vsrc1_regs));
    x_memcpy((void *)VSRC_02,(void *)(pm_save_vsrc2_regs),sizeof(pm_save_vsrc2_regs));
	x_memcpy((void *)VSRC_07,(void *)(pm_save_vsrc3_regs),sizeof(pm_save_vsrc3_regs));
	x_memcpy((void *)VSRC_09,(void *)(pm_save_vsrc4_regs),sizeof(pm_save_vsrc4_regs));
    x_memcpy((void *)TG_00,(void *)(pm_save_tg1_regs),sizeof(pm_save_tg1_regs));
    x_memcpy((void *)TG_18,(void *)(pm_save_tg2_regs),sizeof(pm_save_tg2_regs));
    x_memcpy((void *)TG_19,(void *)(pm_save_tg3_regs),sizeof(pm_save_tg3_regs));
    x_memcpy((void *)CTG_00,(void *)(pm_save_ctg1_regs),sizeof(pm_save_ctg1_regs));
    x_memcpy((void *)CTG_08,(void *)(pm_save_ctg2_regs),sizeof(pm_save_ctg2_regs));
    x_memcpy((void *)SVF_02,(void *)(pm_save_svf_regs),sizeof(pm_save_svf_regs));
	x_memcpy((void *)CTG_DERING,(void *)pm_save_ctg_dering_regs,sizeof(pm_save_ctg_dering_regs));
	x_memcpy((void *)TG_DERING,(void *)pm_save_tg_dering_regs,sizeof(pm_save_tg_dering_regs));
	x_memcpy((void *)AAF_DERING,(void *)pm_save_aaf_dering_regs,sizeof(pm_save_aaf_dering_regs));
	x_memcpy((void *)DERING_OTHER_SETTING,(void *)pm_save_oth_dering_regs,sizeof(pm_save_oth_dering_regs));
	x_memcpy((void *)OMUX_VDOIN_CLK_SEL,(void *)(pm_save_omux_sel_regs),sizeof(pm_save_omux_sel_regs));
}
#endif

#endif
