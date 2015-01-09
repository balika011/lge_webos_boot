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

/**
 * @file Local Dimming driver interface
 *
 * @author dragon_chen@mtk.com.tw
 */

#ifndef _DRV_LCDIM_V2_H_
#define _DRV_LCDIM_V2_H_

// *-----------------------------------------------------------------------------
// * structure / global variables
// *-----------------------------------------------------------------------------
#define LDM_LP_TOTAL_PHASE_NUM 21
#define LCDIM_LIGHTPROF_NUM	        21*128
#define LCDIM_DIMMING_NUM           33
#define LCDIM_GAIN_NUM              41
#define LCDIM_GAIN_LIMIT_NUM        33
#define LCDIM_DEMO_ROUND            3
#define LCDIM_MAX_BLK_NUM           200
#define LCDIM_MAX_HIST_NUM          16
#define LCDIM_MAX_BLKINFO_NUM       32
#define LCDIM_SW_MODE_MAX_BLK_NUM   24

#define LCDIM_SPCTRL_VER_0          0
#define LCDIM_SPCTRL_VER_1          1


#define LIGHTPROFILE(phase, sampleidx) u1LightProfile[(sampleidx)*LDM_LP_TOTAL_PHASE_NUM + (phase)]

extern UINT8 u1LightProfileTable[LCDIM_LIGHTPROF_NUM];
extern UINT16 u2LcdimGainTable[LCDIM_GAIN_NUM];


typedef struct
{
    UINT8 u1DimWeight0;
    UINT8 u1DimWeight1;
    UINT8 u1DimWeight2;
    UINT8 u1DimWeight3;
    UINT8 u1DimWeight4;
} LCDIM_SPATIAL_CFG_T;

typedef struct
{
    UINT8 u1UpDimMax;
    UINT8 u1DnDimMax;
    UINT8 u1DimSlope;
    UINT8 u1DimStepSft;
} LCDIM_TEMPORAL_CFG_T;

typedef struct
{
    UINT8 u1LcdimDebug_SCG;
    UINT8 u1LcdimDebug_SpCtrl;
    UINT8 u1LcdimDebug_PreDetect;
    UINT8 u1LcdimDebug_GainLimit;
    UINT8 u1LcdimDebug_Downtrend;
    UINT8 u1LcdimDebug_PostInfo;
} LCDIM_DEBUG_LOG_EN_T;

typedef struct
{
    UINT8 u1AdapCurveCtrlEn;
    UINT8 u1SCGSpeedCtrlEn;
    UINT8 u1SCGSpeedCtrlVer;    
    UINT8 u1SCGAdapMinDimCtrlEn;
    UINT8 u1SCGAGLCtrlEn;
    UINT8 u1SCGAdapSpiIIRCtrlEn;
    UINT8 u1SCGSupportSCG;
    UINT8 u1LcdimLightDebugEn;
    UINT8 u1ForceScenceChange;
    UINT8 u1SCGAGLForceEn;
    UINT8 u1AdvancePowerSaveEn;
    UINT8 u1HueDisregardEn;
    UINT8 u1CGBoostEn;
    UINT8 u1AGLatCGBoostEn;
    UINT8 u1OSDDetectEn;
    UINT8 u1RFChgDetectEn;
    UINT8 u13DDetectEn;
} LCDIM_FUNCTION_EN_T;

typedef struct
{
    UINT8 u1MotionThd;
    UINT8 u1MotionPenaltyStep;
    UINT8 u1GammaPenaltyStep;
    UINT8 u1MaxRGBConfStep[4];
    UINT8 u1StaticStep;
    UINT8 u1FadeInStep;

    UINT8 u1ConfidenceTh[4];
    UINT8 u1ConfidenceThDft;

    UINT8 u1MaxUpDimStep[4];
    UINT8 u1MaxDnDimStep[4];
    UINT8 u1DyDimSlope[4];
    UINT8 u1DyBoostStep[3];

    UINT8 bCGBoostFrameCnt;
    UINT8 u1DimDowntrendTh;
    UINT8 u1SCGStableCnt;
    UINT8 u1DistThreshold;

    UINT8 u1PreDetectAPLThLow;
    UINT8 u1PreDetectAPLThHigh;
    UINT8 u1PreDetectAPLThOstg;
    UINT8 u1PreDetectMotionTh;
    UINT8 u1PreDetectCntFrmTh;
    UINT8 u1PredetectStep;

    UINT8 u1FadeInDarkCnt;
    UINT8 u1FadeInIncCnt;

    UINT8 u13DDetectAPLThH;
    UINT8 u13DDetectAPLThL;

} LCDIM_SPCTRL_CFG_T;

typedef struct
{
    UINT16 u2SCGRGBMaxDiffThd;
    UINT16 u2SCGRGBTotalDiffThd;
    UINT16 u2SCGRGBAvgDiffThd;
    UINT16 u2SCGRGBMaxDiffThd2;
    UINT16 u2SCGRGBTotalDiffThd2;
    UINT16 u2SCGRGBAvgDiffThd2;
} LCDIM_SCENECHG_CFG_T;      
      
typedef struct
{
    UINT8 u1AGLSCGCor;
    UINT8 u1AGLAPLThL;
    UINT8 u1AGLAPLThH;
    UINT8 u1AGLStep;
    UINT8 u1AGLCorCnt;
} LCDIM_AGL_CFG_T;  

typedef struct 
{
	UINT16  u2max_pxl_th; 
    UINT8   u1dim_wet_0; 
    UINT8   u1dim_wet_1; 
    UINT8   u1dim_wet_2; 
    UINT8   u1dim_wet_3; 
    UINT8   u1dim_wet_4; 
    UINT8   u1rgb_weight; 
    UINT8   u1rgb_weight2; 
    UINT8   u1lpf_weight; 
    UINT8   u1blkinfo_weight; 
    UINT8   u1maxinfo_lpf_mode; 
    UINT16  u2max_dim; 
    UINT16  u2min_dim; 
    UINT8   u1dy_dim_slope; 
    UINT8   u1max_up_dim_step; 
    UINT8   u1max_dn_dim_step;    
	UINT16	u2max_diff_thd; 
	UINT16	u2total_diff_thd; 
	UINT16	u2avg_diff_thd; 
} LCDIM_HW_PARAM_T;

typedef struct 
{
	UINT8   u1lcdim_speed_ctrl_en; 
    UINT8   u1osd_detect; 
    UINT8   u1motion_th; 
    UINT16  u2max_diff_thd2; 
    UINT16  u2total_diff_thd2; 
    UINT16  u2avg_diff_thd2; 
    UINT8   u1scg_wait_th; 
    UINT8   u1motion_penalty; 
    UINT8   u1gamma_penalty; 
    UINT8   u1static_step; 
    UINT8   u1maxrgb_step_low; 
    UINT8   u1maxrgb_step_mid_dn; 
    UINT8   u1maxrgb_step_mid_up; 
    UINT8   u1maxrgb_step_high; 
    UINT8   u1predet_apl_th_low; 
    UINT8   u1predet_apl_th_high; 
    UINT8   u1predet_apl_th_ostg; 
    UINT8   u1predet_step; 
    UINT8   u1predet_frame_th; 
    UINT8   u1fadein_det_dark_th; 
    UINT8   u1fadein_det_inc_th; 
    UINT8   u1fadein_step; 
    UINT8   u1confidence_th0; 
    UINT8   u1confidence_th1; 
    UINT8   u1confidence_th2; 
    UINT8   u1confidence_th3; 
    UINT8   u1confidence_thdft; 
    UINT8   u1lcdim_max_up_dim_step_low; 
    UINT8   u1lcdim_max_up_dim_step_mid_dn; 
    UINT8   u1lcdim_max_up_dim_step_mid_up; 
    UINT8   u1lcdim_max_up_dim_step_high; 
    UINT8   u1lcdim_max_dn_dim_step_low; 
    UINT8   u1lcdim_max_dn_dim_step_mid_dn; 
    UINT8   u1lcdim_max_dn_dim_step_mid_up; 
    UINT8   u1lcdim_max_dn_dim_step_high; 
    UINT8   u1lcdim_dy_dim_slope_low; 
    UINT8   u1lcdim_dy_dim_slope_mid_dn; 
    UINT8   u1lcdim_dy_dim_slope_mid_up; 
    UINT8   u1lcdim_dy_dim_slope_high; 
    UINT8   u1lcdim_adap_mindim; 
    UINT8   u1dist_th; 
    UINT8   u1apl_th; 
    UINT8   u1adap_weight_0_l; 
    UINT8   u1adap_weight_1_l; 
    UINT8   u1adap_weight_2_l; 
    UINT8   u1adap_weight_3_l; 
    UINT8   u1adap_weight_4_l; 
    UINT8   u1adap_weight_0_h; 
    UINT8   u1adap_weight_1_h; 
    UINT8   u1adap_weight_2_h; 
    UINT8   u1adap_weight_3_h; 
    UINT8   u1adap_weight_4_h; 
    UINT8   u1adv_ps_en; 
    UINT8   u1adv_ps_ratio; 
    UINT8   u1scg_iir_en; 
    UINT8   u1iir_sel; 
    UINT8   u1downtrend_dim_th; 
    UINT8   u1scg_enable; 
    UINT8   u1boost_0; 
    UINT8   u1boost_1; 
    UINT8   u1boost_2;
} LCDIM_SW_PARAM_T;


void vDrvLcdimWriteLightProfile(UINT8 *u1LightProfile);
void vDrvLcdimReadLightProfile(UINT8 *u1LightProfile);
void vDrvLcdimWriteDimTable(UINT16 *u2DimTable);
void vDrvLcdimReadDimTable(UINT16 *u2DimTable);
void vDrvLcdimWriteGainTable(UINT16 *u2GainTable);
void vDrvLcdimWriteGainLimitTable(UINT16 *u2GainLimitTable);
void vDrvLcdimReadGainTable(UINT16 *u2GainTable);
void vDrvLcdimQualitySWInit(void);
void vDrvLcdimQualityHWInit(void);
void vDrvLcDimQualityInit(void);
BOOL bDrvLcDimPanelInit(void);
BOOL bDrvLcdimDetectSceneChange(void);
void vDrvLcdimResetTemporalFilter(UINT8 bOnOff);
void vDrvLcdimSCGProc(void);
void vDrvLcdimSPIProc(void);
void vDrvLcdimBlkSCGProc(void);
void vDrvLcdimProc(void);
void vDrvLcdimProcMainLoop(void);
void vDrvLcdimSetPhase(UINT8 *u1LcdimPhase);
void vDrvSetLcDimOnOff(UINT8 bOnOff);
UINT8 vDrvGetLcDimOnOffStatus(void);
void vDrvLcdimForceBlockDimOnOff(UINT8 bOnOff);
void vDrvLcdimForceBlockDimCtrl(UINT8 u1PosH, UINT8 u1PosV, UINT8 u1BlockOnOff, UINT8 u1ForceLevel);
void vDrvGetLcDimDimmingResult(UINT8 *u1DimmingResult); 
void vDrvGetLcDimDimmingResultHW(UINT16 *u2DimmingResult);
BOOL bDrvLcDimSetPanelInfo(UINT32 u4BlockNumH, UINT32 u4BlockNumV, UINT8 u1LightDirection);
void vDrvLcdimSetInk(UINT8 u1InkType);
void vDrvLcdimSetInkColor(UINT16 u2ColorR, UINT16 u2ColorG, UINT16 u2ColorB);
UINT32 u4DrvLcdimReadBlockInfo(UINT8 u1H, UINT8 u1V, UINT8 u1InfoType);
UINT32 u4DrvLcdimReadBlkInfoV3(UINT8 u1InfoType,UINT8 * pu1BLBlockDataTbl,UINT8 u1BlkNum);
BOOL vDrvVerifyLcDimLightProfile(UINT8 *bLcDimLightProfile);
BOOL vDrvVerifyLcDimDimTBL(UINT16 *u2LcDimDimTbl);
BOOL vDrvVerifyLcDimGainTBL(UINT16 *bu2LcDimGainTbl);
void vDrvSetLcDSOnOff(BOOL bOnOff);
void vDrvLcdimSetDSDemo(UINT8 u1DemoType, BOOL bOnOff);
void vDrvLcdimSetDemoEffect(UINT8 *bDimmingResult);
void vDrvLcdimSetDemoParam(UINT8 bSel);
UINT32 u4QueAverage(UINT32 u4Value, UINT8 bItem, UINT32 u4QueSize);
void vDrvClearDataQue(void);
void vDrvAdaptiveLcDimUpdateParam(void);
void vDrvLcDimCreateAdaptiveCurve(UINT16 *u2LcdimCurveDft, UINT16 *u2LcdimCurveStr, UINT32 u4Confidence);
void vDrvAdaptiveCurveRemapping(UINT16 *u2LcdimCurveDft, UINT16 *u2LcdimCurveStr);
void vDrvAdaptiveLcDim(UINT16 *u2LcdimCurveDft, UINT16 *u2LcdimCurveStr);
void vDrvLcdimWriteSpatialCfg(LCDIM_SPATIAL_CFG_T* prLDSptlCfg);
void vDrvLcdimWriteTemporalCfg(LCDIM_TEMPORAL_CFG_T* prLDTplCfg);
UINT8 u1DrvLcDimCalAvgDivider(void);
void vDrvLcdimSetDimSWMode(UINT16 *u2DimmingResult);
void vDrvLcdimGetBlkinfo(UINT8 *u1BlkInfo);
void vDrvLcDimSetSWTable(UINT16 *u2LDGainLimitTable_LOW_BND, UINT16 *u2LDGainLimitTable_UP_BND);
void vDrvLcDimSetHWParam(LCDIM_HW_PARAM_T* pLCDIM_HWParam);
void vDrvLcDimSetSWParam(LCDIM_SW_PARAM_T* pLCDIM_SWParam);



void vDrvLcDimPQSuspend(void);
void vDrvLcDimPQResume(void);

#endif //_DRV_LCDIM_H_

