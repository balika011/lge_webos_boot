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

// === INCLUDE =============================================================================
#include "pe_if.h"
#include "drv_meter.h"
#include "drv_contrast.h"
#include "vdo_misc.h"
#include "vdp_if.h"
#include "nptv_debug.h"
#include "x_assert.h"
#include "hw_sw.h"
#include "hw_ycproc.h"

// === HW INCLUDE =========================================================================

// === DEFINE =============================================================================
#define CLIP(val, min, max) ((val>=max) ? max : ((val<=min) ? min : val))

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================
void vDrvADLProc_int(const UINT16 waHist[LUMA_HIST_NUM_WEIGHT], UINT32 u4APL);
void vDrvADLProcMainLoop_int(void);

// === EXTERN VARIABLE ====================================================================
extern UINT8 bForceScenceChange;
extern UINT8 bSkinLumaOnOff;
extern UINT8 bALUpdateCount;
extern UINT8 bADLSceneChange;
extern UINT8 bIsSrcLRflg;
extern UINT8 bIIRStrength;

// === GLOBAL VARIABLE ====================================================================
UINT16 waLumaArray[LUMA_CURVE_NUM]; //luma curve
UINT16 waSecondLumaArray[LUMA_CURVE_SKIN_NUM];	//Second luma curve

UINT8 bManualLumaCurve = SV_ON;

static void vDrvADLSetLumaCurveHw(const UINT16 * waLArray)
{
	UINT8 bi;

	ASSERT(waLArray!=NULL);

	for (bi = 0; bi < 16; bi++)
	{
        vIO32Write4B(Y_FTN_1_0_MAIN + 4 * bi,
            (UINT32)waLArray[2 * bi] + (((UINT32)waLArray[2 * bi + 1]) << 16));
	}
	vIO32Write4B(Y_FTN_32_33_MAIN, waLArray[32]);
}

void vDrvADLSetSecondLumaCurveHw(const UINT16 * waSLArray)
{
	UINT8 bi;

	ASSERT(waSLArray!=NULL);

	for (bi = 0; bi < 16; bi++)
	{
        vIO32Write4B(Y_FTN2_1_0_MAIN + 4 * bi,
            (UINT32)waSLArray[2 * bi] + (((UINT32)waSLArray[2 * bi + 1]) << 16));
	}
	vIO32Write4B(Y_FTN2_33_32_MAIN, waSLArray[32]);
}

void vDrvADLSetYGamma(UINT8 bIndex)
{
    vIO32WriteFldAlign(ADAP_LUMA_09, bIndex, ADL_UI_YGAMMA);
}

void vDrvADLSetLinearCurve(void)
{
	UINT32 i;

	for (i = 0; i < LUMA_CURVE_NUM-1; i++)
	{
		waLumaArray[i] = (UINT16)(i << 5);
	}

	waLumaArray[LUMA_CURVE_NUM-1] = 1023;
    vDrvADLSetLumaCurveHw(waLumaArray);
}

void vDrvADLSetLinearSecondCurve(void)
{
	UINT32 i;

	for (i = 0; i < LUMA_CURVE_NUM-1; i++)
	{
		waSecondLumaArray[i] = (UINT16)(i << 5);
	}

	waSecondLumaArray[LUMA_CURVE_NUM-1] = 1023;
    vDrvADLSetSecondLumaCurveHw(waSecondLumaArray);
}

void vDrvADLGetLumaCurve(UINT16* waLArray)
{
    if (waLArray == NULL)
    {
        return;
    }

	x_memcpy(waLArray, waLumaArray, LUMA_CURVE_NUM*sizeof(UINT16));
}

void vDrvADLSetLumaCurve(const UINT16* waLArray)
{
    if (waLArray == NULL)
    {
        return;
    }

    x_memcpy(waLumaArray, waLArray, LUMA_CURVE_NUM*sizeof(UINT16));    
    vApiRegisterVideoEvent(PE_EVENT_LUMA_CURVE, VDP_1, SV_ON);    
}

void vDrvADLSetLumaSecCurve(const UINT16* waLArray)
{
    if (waLArray == NULL)
    {
        return;
    }

    x_memcpy(waSecondLumaArray, waLArray, LUMA_CURVE_SKIN_NUM*sizeof(UINT16));
}


void vDrvAutoContrastFreeze(UINT8 bFreeze)
{
	vIO32WriteFldAlign(ADAPTIVE_REG, (!bFreeze), ADAPTIVE_GETINFO);
}

void vDrvADLOnOff(UINT8 bOnOff)
{

#ifdef __MODEL_slt__
    bOnOff = 0;
    LOG(0, "AdaptiveLuma: Disable Luma Histogram for SLT\n");
#endif

    vIO32WriteFldAlign(ADAPTIVE_REG, bOnOff, ADAPTIVE_LUMA);
}

void vDrvSetSecondLumaCurveWindow(UINT16* YWindow, UINT16* SWindow, UINT16* HWindow)
{
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_1, YWindow[0], Y_FTN2_YSLOPE);
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_1, YWindow[1], Y_FTN2_YMAX);
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_1, YWindow[2], Y_FTN2_YMIN);
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_2, SWindow[0], Y_FTN2_SSLOPE);
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_2, SWindow[1], Y_FTN2_SMAX);
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_2, SWindow[2], Y_FTN2_SMIN);
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_3, HWindow[0], Y_FTN2_HSLOPE);
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_3, HWindow[1], Y_FTN2_HMAX);
	vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_3, HWindow[2], Y_FTN2_HMIN);
}

void vDrvSetSecondLumaCurveWinInit(void)
{
    UINT8 u1SSlope =(wReadQualityTable(QUALITY_SKIN_SSLOPE)) ? (wReadQualityTable(QUALITY_SKIN_SSLOPE)): 4;
    UINT8 u1HSlope =(wReadQualityTable(QUALITY_SKIN_HSLOPE)) ? (wReadQualityTable(QUALITY_SKIN_HSLOPE)): 2;

	UINT16 wSecondCurveWindowY[3] = {wReadQualityTable(QUALITY_SKIN_YSLOPE), wReadQualityTable(QUALITY_SKIN_YMAX)<<2,
                                        wReadQualityTable(QUALITY_SKIN_YMIN)<<2};   //Y_Slope, Y_Max, Y_Min
	UINT16 wSecondCurveWindowS[3] = {wReadQualityTable(QUALITY_SKIN_SSLOPE), wReadQualityTable(QUALITY_SKIN_SMAX)<<2,
                                        wReadQualityTable(QUALITY_SKIN_SMIN)<<2};  //S_Slope, S_Max, S_Min
	UINT16 wSecondCurveWindowH[3] = {wReadQualityTable(QUALITY_SKIN_HSLOPE), wReadQualityTable(QUALITY_SKIN_HMAX)<<2,
                                        wReadQualityTable(QUALITY_SKIN_HMIN)<<2};  //H_Slope, H_Max, H_Min
    UINT8 bWin[4] = {   40,  5,  100,   78};

	vDrvSetSecondLumaCurveWindow(wSecondCurveWindowY, wSecondCurveWindowS, wSecondCurveWindowH);

//Use 2D window 0 for skin stability detection
    bWin[0] = CLIP(((wReadQualityTable(QUALITY_SKIN_SMAX)<<2) + 192/u1SSlope)>>2, 40, 255);
    bWin[1] = CLIP(((wReadQualityTable(QUALITY_SKIN_SMIN)<<2) - 192/u1SSlope)>>2, 1, 255);
    bWin[2] = CLIP(((wReadQualityTable(QUALITY_SKIN_HMAX)<<2) + 192/u1HSlope)>>2, 100, 255);
    bWin[3] = CLIP(((wReadQualityTable(QUALITY_SKIN_HMIN)<<2) - 192/u1HSlope)>>2, 50, 255);

    vDrvSetSatHue2DWindowBound(0, bWin[0], bWin[1], bWin[2], bWin[3]);
}

void vDrvADLUpdateParam(void)
{
	vIO32WriteFldAlign(SCE_CFG_MAIN, wReadQualityTable(QUALITY_AL2_HISTOGRAM_MODE), COLOR_YHIST_WGT_EN);
// Update Adaptive luma version 1 parameter
    vIO32WriteFldAlign(ADAP_LUMA_04, wReadQualityTable(QUALITY_AL_ON_1_OFF_0), SWREG_AL_ON_1_OFF_0);
    vIO32WriteFldAlign(ADAP_LUMA2_00, wReadQualityTable(QUALITY_AL2_AL_VERSION), ADL_VERSION);

    vIO32WriteFldAlign(ADAP_LUMA_04, wReadQualityTable(QUALITY_AL_BWS_BLACK_ON_1_OFF_0), SWREG_BWS_BLACK_ON_1_OFF_0);
    vIO32WriteFldAlign(ADAP_LUMA_04, wReadQualityTable(QUALITY_AL_BWS_WHITE_ON_1_OFF_0), SWREG_BWS_WHITE_ON_1_OFF_0);

    vIO32WriteFldAlign(ADAP_LUMA_03, wReadQualityTable(QUALITY_AL_GAIN), SWREG_AL_GAIN);
    vIO32WriteFldAlign(ADAP_LUMA_03, wReadQualityTable(QUALITY_AL_OFFSET), SWREG_AL_OFFSET);
    vIO32WriteFldAlign(ADAP_LUMA_03, wReadQualityTable(QUALITY_AL_LIMIT), SWREG_AL_LIMIT);

    vIO32WriteFldAlign(ADAP_LUMA_04, wReadQualityTable(QUALITY_AL_BWS_BLACK_LEVEL), SWREG_BWS_BLACK_LEVEL);
    vIO32WriteFldAlign(ADAP_LUMA_00, wReadQualityTable(QUALITY_AL_BWS_BLACK_GAIN), SWREG_BWS_BLACK_GAIN);
    vIO32WriteFldAlign(ADAP_LUMA_00, wReadQualityTable(QUALITY_AL_BWS_BLACK_OFFSET), SWREG_BWS_BLACK_OFFSET);
    vIO32WriteFldAlign(ADAP_LUMA_00, wReadQualityTable(QUALITY_AL_BWS_BLACK_RATIO), SWREG_BWS_BLACK_RATIO);
    vIO32WriteFldAlign(ADAP_LUMA_00, wReadQualityTable(QUALITY_AL_BWS_BLACK_LIMIT), SWREG_BWS_BLACK_LIMIT);

    vIO32WriteFldAlign(ADAP_LUMA_04, wReadQualityTable(QUALITY_AL_BWS_WHITE_LEVEL), SWREG_BWS_WHITE_LEVEL);
    vIO32WriteFldAlign(ADAP_LUMA_01, wReadQualityTable(QUALITY_AL_BWS_WHITE_GAIN), SWREG_BWS_WHITE_GAIN);
    vIO32WriteFldAlign(ADAP_LUMA_01, wReadQualityTable(QUALITY_AL_BWS_WHITE_OFFSET), SWREG_BWS_WHITE_OFFSET);
    vIO32WriteFldAlign(ADAP_LUMA_01, wReadQualityTable(QUALITY_AL_BWS_WHITE_RATIO), SWREG_BWS_WHITE_RATIO);
    vIO32WriteFldAlign(ADAP_LUMA_01, wReadQualityTable(QUALITY_AL_BWS_WHITE_LIMIT), SWREG_BWS_WHITE_LIMIT);

    vIO32WriteFldAlign(ADAP_LUMA_02, wReadQualityTable(QUALITY_AL_DYNBS_PXL_THD1), SWREG_DYNBS_PXL_THD1);
    vIO32WriteFldAlign(ADAP_LUMA_02, wReadQualityTable(QUALITY_AL_DYNBS_PXL_THD2), SWREG_DYNBS_PXL_THD2);
    vIO32WriteFldAlign(ADAP_LUMA_02, wReadQualityTable(QUALITY_AL_DYNBS_DIST_THDL), SWREG_DYNBS_DIST_THDL);
    vIO32WriteFldAlign(ADAP_LUMA_02, wReadQualityTable(QUALITY_AL_DYNBS_DIST_THDH), SWREG_DYNBS_DIST_THDH);

    vIO32WriteFldAlign(ADAP_LUMA_04, wReadQualityTable(QUALITY_AL_DYNBS_EN), SWREG_DYNBS_ONOFF);
    vIO32WriteFldAlign(ADAP_LUMA_04, wReadQualityTable(QUALITY_AL_DYNBS_RANGE), SWREG_DYNBS_RANGE);
    vIO32WriteFldAlign(ADAP_LUMA_05, wReadQualityTable(QUALITY_AL_DYNBS_LMT_BTM), SWREG_DYNBS_LMT_BTM);
    vIO32WriteFldAlign(ADAP_LUMA_05, wReadQualityTable(QUALITY_AL_DYNBS_LMT_TOP), SWREG_DYNBS_LMT_TOP);
    vIO32WriteFldAlign(ADAP_LUMA_05, wReadQualityTable(QUALITY_AL_DYNBS_GAIN_BTM), SWREG_DYNBS_GAIN_BTM);
    vIO32WriteFldAlign(ADAP_LUMA_05, wReadQualityTable(QUALITY_AL_DYNBS_GAIN_TOP), SWREG_DYNBS_GAIN_TOP);
    vIO32WriteFldAlign(ADAP_LUMA_06, wReadQualityTable(QUALITY_AL_DYNBS_OFST_BTM), SWREG_DYNBS_OFST_BTM);
    vIO32WriteFldAlign(ADAP_LUMA_06, wReadQualityTable(QUALITY_AL_DYNBS_OFST_TOP), SWREG_DYNBS_OFST_TOP);

    vIO32WriteFldAlign(ADAP_LUMA_0D, wReadQualityTable(QUALITY_AL_BWS_KEEP_MID_ONOFF), SWREG_BWS_KEEP_MID_ON_OFF);
    vIO32WriteFldAlign(ADAP_LUMA_0D, wReadQualityTable(QUALITY_AL_BWS_KEEP_MID_X_LVL), SWREG_BWS_KEEP_MID_X);
    vIO32WriteFldAlign(ADAP_LUMA_0D, wReadQualityTable(QUALITY_AL_BWS_KEEP_MID_Y_POS), SWREG_BWS_KEEP_MID_Y);

// Update Adaptive luma version 2 parameter
	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL_ON_1_OFF_0), ADL_ON_1_OFF_0);

	vIO32WriteFldAlign(ADAP_LUMA2_00, wReadQualityTable(QUALITY_AL2_MID_AC_GAIN), ADL_MID_AC_GAIN);
	vIO32WriteFldAlign(ADAP_LUMA2_00, wReadQualityTable(QUALITY_AL2_MID_DC_GAIN), ADL_MID_DC_GAIN);

	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL2_BWS_BK_EN), ADL_BWS_BLACK_ON_1_OFF_0);
	vIO32WriteFldAlign(ADAP_LUMA2_01, wReadQualityTable(QUALITY_AL2_BWS_BK_LEVEL), ADL_BWS_BLACK_LEVEL);
	vIO32WriteFldAlign(ADAP_LUMA2_01, wReadQualityTable(QUALITY_AL2_BWS_BK_AC_GAIN_RATIO), ADL_BWS_BLACK_AC_GAIN_RATIO);
	vIO32WriteFldAlign(ADAP_LUMA2_01, wReadQualityTable(QUALITY_AL2_BWS_BK_DC_GAIN_RATIO), ADL_BWS_BLACK_DC_GAIN_RATIO);
	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL2_BWS_BK_ADPTLMT_EN), ADL_BWS_BLACK_ADPT_LMT_EN);
	vIO32WriteFldAlign(ADAP_LUMA2_01, wReadQualityTable(QUALITY_AL2_BWS_BK_RATIO), ADL_BWS_BLACK_RATIO);
	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL2_BWS_BK_STROFT_EN), ADL_BWS_BLACK_STRONG_OFFSET_EN);
	vIO32WriteFldAlign(ADAP_LUMA2_02, wReadQualityTable(QUALITY_AL2_BWS_BK_OFST_L), ADL_BWS_BLACK_OFFSET_L);
	vIO32WriteFldAlign(ADAP_LUMA2_02, wReadQualityTable(QUALITY_AL2_BWS_BK_OFST_H), ADL_BWS_BLACK_OFFSET_H);
	vIO32WriteFldAlign(ADAP_LUMA2_02, wReadQualityTable(QUALITY_AL2_BWS_BK_OFST_TH_L), ADL_BWS_BLACK_OFT_TH_L);
	vIO32WriteFldAlign(ADAP_LUMA2_02, wReadQualityTable(QUALITY_AL2_BWS_BK_OFST_TH_H), ADL_BWS_BLACK_OFT_TH_H);

	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL2_BWS_WT_EN), ADL_BWS_WHITE_ON_1_OFF_0);
	vIO32WriteFldAlign(ADAP_LUMA2_03, wReadQualityTable(QUALITY_AL2_BWS_WT_LEVEL), ADL_BWS_WHITE_LEVEL);
	vIO32WriteFldAlign(ADAP_LUMA2_03, wReadQualityTable(QUALITY_AL2_BWS_WT_AC_GAIN_RATIO), ADL_BWS_WHITE_AC_GAIN_RATIO);
	vIO32WriteFldAlign(ADAP_LUMA2_03, wReadQualityTable(QUALITY_AL2_BWS_WT_DC_GAIN_RATIO), ADL_BWS_WHITE_DC_GAIN_RATIO);
	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL2_BWS_WT_ADPTLMT_EN), ADL_BWS_WHITE_ADPT_LMT_EN);
	vIO32WriteFldAlign(ADAP_LUMA2_03, wReadQualityTable(QUALITY_AL2_BWS_WT_RATIO), ADL_BWS_WHITE_RATIO);
	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL2_BWS_WT_STROFT_EN), ADL_BWS_WHITE_STRONG_OFFSET_EN);
	vIO32WriteFldAlign(ADAP_LUMA2_04, wReadQualityTable(QUALITY_AL2_BWS_WT_OFST_L), ADL_BWS_WHITE_OFFSET_L);
	vIO32WriteFldAlign(ADAP_LUMA2_04, wReadQualityTable(QUALITY_AL2_BWS_WT_OFST_H), ADL_BWS_WHITE_OFFSET_H);
	vIO32WriteFldAlign(ADAP_LUMA2_04, wReadQualityTable(QUALITY_AL2_BWS_WT_OFST_TH_L), ADL_BWS_WHITE_OFT_TH_L);
	vIO32WriteFldAlign(ADAP_LUMA2_04, wReadQualityTable(QUALITY_AL2_BWS_WT_OFST_TH_H), ADL_BWS_WHITE_OFT_TH_H);

	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL2_WEIGHT1), ADL_WEIGHT1);
	vIO32WriteFldAlign(ADAP_LUMA2_05, wReadQualityTable(QUALITY_AL2_WEIGHT2), ADL_WEIGHT2);

    vIO32WriteFldAlign(ADAP_LUMA_0C, wReadQualityTable(QUALITY_ALCOM_YGAMMA_SEL), Y_GAMMA_SEL);
    vIO32WriteFldAlign(ADAP_LUMA_0C, wReadQualityTable(QUALITY_ALCOM_YGAMMA_DECAY_HI), Y_GAMMA_DECAY_H);
    vIO32WriteFldAlign(ADAP_LUMA_0C, wReadQualityTable(QUALITY_ALCOM_YGAMMA_DECAY_LO), Y_GAMMA_DECAY_L);
    vIO32WriteFldAlign(ADAP_LUMA_0C, wReadQualityTable(QUALITY_ALCOM_YGAMMA_DECAY_BRIGHT), Y_BRIGHT);
	vIO32WriteFldAlign(ADAP_LUMA_08, wReadQualityTable(QUALITY_ALCOM_YGAMMA_MID_SEL), Y_GAMMA_MID_SEL);

	vIO32WriteFldAlign(ADAP_LUMA_0A, wReadQualityTable(QUALITY_ALCOM_SKIN_LUMA_EN), ADL_SECOND_LUMA_EN);
	vIO32WriteFldAlign(ADAP_LUMA_0A, wReadQualityTable(QUALITY_ALCOM_CURVE_WEIGHT), ADL_SECOND_CURVE_WEIGHT);
	vIO32WriteFldAlign(ADAP_LUMA_0A, wReadQualityTable(QUALITY_ALCOM_BLEND_METHOD), ADL_SECOND_LUMA_BLEND_METHOD);
	vIO32WriteFldAlign(ADAP_LUMA_08, wReadQualityTable(QUALITY_ALCOM_YGAMMA_SKIN_MID_SEL), Y_GAMMA_MID_SEL_SKIN);

    // Depend on customer spec
    bForceScenceChange = 1;

    vDrvSetSecondLumaCurveWinInit();
}

void vDrvSetSecondLumaCurveOnOff(void)
{
	if(bSkinLumaOnOff)
	{
		vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_1, SV_OFF, Y_FTN2_BYPASS);
	}
	else
	{
		vIO32WriteFldAlign(Y_FTN2_SKIN_RANGE_1, SV_ON, Y_FTN2_BYPASS);
	}
}

void vDrvADLInit(void)
{
    if (fgIsSWRegInit()==SV_FALSE)
    {
        ASSERT(0);
    }

    vIO32WriteFldAlign(ADAP_LUMA_08, 0x00, ADL_DEBUG_INDEX);
    vIO32WriteFldAlign(ADAP_LUMA_08, 0x40, ADL_DEBUG_COUNT);
    vIO32WriteFldAlign(ADAP_LUMA_09, 0x00, ADL_UI_YGAMMA);
    vIO32WriteFldAlign(ADAP_LUMA_09, 0x00, ADL_UPDATE_COUNT);
//    vIO32WriteFldAlign(SCE_CFG_MAIN, 0x00, COLOR_YHIST_TYPE);  //select 33 bin luma histogram
	vDrvSetSecondLumaCurveWinInit();
}

void vDrvADLManualLumaCurveOnOff(UINT8 bOnOff)
{
	bManualLumaCurve = bOnOff;
}

void vDrvADLUpdateLumaCurve(void)
{
    vDrvADLSetLumaCurveHw(waLumaArray);
}

void vDrvADLUpdateSecondLumaCurve(void)
{
    vDrvADLSetSecondLumaCurveHw(waSecondLumaArray);
}

extern UINT8 bHistogramMode;

void vDrvADLProc(void)
{
	static UINT8 bEnable = SV_OFF;
	UINT16 wHistCur[LUMA_HIST_NUM_WEIGHT];
	static UINT8 bCurCount = 0;

    SYS_MEASURE_TIME_START(SV_ADAP_LUMA_TIMER);

	// Bypass Adaptive Luma
	if (IO32ReadFldAlign(ADAPTIVE_REG, ADAPTIVE_LUMA) == 0)
	{
		// From On to Off
		if(bEnable == SV_ON)
		{
			LOG(0, "Disable AdaptiveLuma\n");
			vDrvADLSetLinearCurve();
			vDrvSetSecondLumaCurveOnOff();
			vDrvADLSetLinearSecondCurve();
		}
		bEnable = SV_OFF;
		return;
	}
    else
    {
    	if (bEnable == SV_OFF)
    	{
    		LOG(0, "Enable AdaptiveLuma\n");
    	}
    	bEnable = SV_ON;
    }

    if(bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE)
    {
        vDrvADLSetLinearCurve();
        vDrvADLSetLinearSecondCurve();
		return;
    }

	u1DrvGetLumaHistWeight(wHistCur, LUMA_HIST_NORM_BASE);

    vDrvGetBlackWhite2BinSum(wHistCur, bHistogramMode);   // MJC moving box patch of mt5396 changelsit:903459 ,checked to cd lee ,it also do in Cobra
    
    if (bManualLumaCurve == SV_OFF)
    {      
        vDrvADLProc_int(wHistCur, (UINT32)bDrvGetAPL());

        if ((++bCurCount > bALUpdateCount) || bForceScenceChange || bADLSceneChange || bIsSrcLRflg)
        {
		    vDrvADLUpdateLumaCurve();
			vDrvSetSecondLumaCurveOnOff();
			if(bSkinLumaOnOff)
			{
				vDrvADLUpdateSecondLumaCurve();
			}
			else
			{
				vDrvADLSetLinearSecondCurve();
			}
            bCurCount = 0;
        }     
	}
	
    SYS_MEASURE_TIME_END(SV_ADAP_LUMA_TIMER);
    SYS_STORE_TIME_INFO_TO_REG(SV_ADAP_LUMA_TIMER);    
}

void vDrvADLProcMainLoop(void)
{
    vDrvADLProcMainLoop_int();
}

