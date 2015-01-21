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
 * $Author: p4admin $
 * $Date: 2015/01/21 $
 * $RCSfile: drv_ycproc.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_YCPROC_H_
#define _DRV_YCPROC_H_

#include "typedef.h"
#include "x_typedef.h"

#define EXTMJC_SW_TO_FRONT (IO32ReadFldAlign(PEUI_00, PEUI_EXTMJC_SWITCH_TO_FRONT) == 1)
#define GAMMA_IN_DRAM (IO32ReadFldAlign(PEUI_00, PEUI_INIT_GAMMA_TBL_IN_DRAM) == 1)
#define SET_GAMMA_IN_DRAM(u1OnOff) (vIO32WriteFldAlign(PEUI_00, (u1OnOff)?1:0, PEUI_INIT_GAMMA_TBL_IN_DRAM))
#define ISSCALER444(u1Path) ((u1Path) ? (IO32ReadFldAlign(MATRIX_00, SCALER444_PIP)) : (IO32ReadFldAlign(MATRIX_00, SCALER444_MAIN)))
#define ISSCALERRGB(u1Path) ((u1Path) ? (IO32ReadFldAlign(MATRIX_00, SCALERRGB_PIP)) : (IO32ReadFldAlign(MATRIX_00, SCALERRGB_MAIN)))
#define SET_FORCE_LOAD_MATRIX(u1OnOff) (vIO32WriteFldAlign(MATRIX_00, u1OnOff, FORCE_LOAD_MATRIX))
#define GET_FORCE_LOAD_MATRIX (IO32ReadFldAlign(MATRIX_00, FORCE_LOAD_MATRIX))
#define SET_MATRIX_PED(u1OnOff) (vIO32WriteFldAlign(MATRIX_00, u1OnOff, MATRIX_PED_EN))
#define GET_MATRIX_PED (IO32ReadFldAlign(MATRIX_00, MATRIX_PED_EN))
#define GET_BLK_LVL(u1Path) ((u1Path) ? (IO32ReadFldAlign(MATRIX_00, IN_CSC_BLACK_LVL_P)) : (IO32ReadFldAlign(MATRIX_00, IN_CSC_BLACK_LVL_M)))
#define GET_MATRIX_HD_709 (IO32ReadFldAlign(MATRIX_00, MATRIX_HD_709))

typedef enum
{
	BASE_COLOR_M = 0,
	BASE_COLOR_R,
	BASE_COLOR_Y,
	BASE_COLOR_G,
	BASE_COLOR_C,
	BASE_COLOR_B,
	BASE_COLOR_MAX
} BASE_COLOR_INDEX_T;

enum
{
    TDS_CURVE_SW_REG_GAIN=0,
    TDS_CURVE_SW_REG_COR_ZERO,
    TDS_CURVE_SW_REG_COR_RATIO,
    TDS_CURVE_SW_REG_COR_TH,
    TDS_CURVE_SW_REG_LIMIT,
    TDS_CURVE_SW_REG_LIMIT_RATIO,
    TDS_CURVE_SW_REG_LIMIT_BOUND,
    TDS_CURVE_SW_REG_END
};

enum
{
    ENUM_Y_SLOPE = 0,

    ENUM_S_GAIN1 = 1,
    ENUM_S_GAIN2 = 2,
    ENUM_S_GAIN3 = 3,
    ENUM_S_P1 = 4,
    ENUM_S_P2 = 5,

    ENUM_S_Y000 = 6,
    ENUM_S_Y064 = 7,
    ENUM_S_Y128 = 8,
    ENUM_S_Y192 = 9,
    ENUM_S_Y256 = 10,

    ENUM_H_FTN = 11,

    ENUM_H_Y000 = 12,
    ENUM_H_Y064 = 13,
    ENUM_H_Y128 = 14,
    ENUM_H_Y192 = 15,
    ENUM_H_Y256 = 16,

    ENUM_H_S000 = 17,
    ENUM_H_S032 = 18,
    ENUM_H_S064 = 19,
    ENUM_H_S096 = 20,
    ENUM_H_S128 = 21,

    ENUM_LCMP_M11M = 22,
    ENUM_LCMP_M11L = 23,
    ENUM_LCMP_M12M = 24,
    ENUM_LCMP_M12L = 25,
    ENUM_LCMP_M13M = 26,
    ENUM_LCMP_M13L = 27,
    ENUM_LCMP_M21M = 28,
    ENUM_LCMP_M21L = 29,
    ENUM_LCMP_M22M = 30,
    ENUM_LCMP_M22L = 31,
    ENUM_LCMP_M23M = 32,
    ENUM_LCMP_M23L = 33,

	ENUM_3DMAP_W1H = 34,
	ENUM_3DMAP_W1L = 35,
	ENUM_3DMAP_W1S = 36,
	ENUM_3DMAP_W2H = 37,
	ENUM_3DMAP_W2L = 38,
	ENUM_3DMAP_W2S = 39,
	ENUM_3DMAP_W3H = 40,
	ENUM_3DMAP_W3L = 41,
	ENUM_3DMAP_W3S = 42,

    ENUM_END = 43
};

typedef struct structAmbilightWindow {
    UINT16 wX_Start;
    UINT16 wX_End;
    UINT16 wY_Start;
    UINT16 wY_End;
    UINT8 bAttenuate;
} structAmbilightWindow;

enum
{
    AmbilightRSel,
    AmbilightGSel,
    AmbilightBSel
};

typedef struct {
    UINT16 wHStart;
    UINT16 wHEnd;
    UINT16 wVStart;
    UINT16 wVEnd;
    UINT8 u1OnOff;
}Region;

typedef struct {
    UINT16 u1CMSBoundL;
    UINT16 u1CMSP0;
    UINT16 u1CMSP1;
    UINT16 u1CMSP2;
    UINT16 u1CMSP3;
    UINT16 u1CMSP4;    
    UINT16 u1CMSBoundU;
} CMSCtrlPt;

typedef struct {
    UINT16 u1CMSG0;
    UINT16 u1CMSG1;
    UINT16 u1CMSG2;
    UINT16 u1CMSG3;
    UINT16 u1CMSG4;    
    UINT16 u1CMSG5;    
} CMSGain;

typedef struct {
    UINT32 bX;
    UINT32 bY;
} TDSPoint;

enum
{
	TDS_H1=0,
	TDS_H2,
	TDS_H3,
	TDS_V1,
	TDS_V2,
	TDS_V3,
	TDS_D1,
	TDS_D2,
	TDS_D3,
	TDS_LTI_HL,
	TDS_LTI_HH,
	TDS_LTI_V,
	CDS1_H1,
	CDS1_H2,
	CDS1_H3,
	CDS1_V1,
	CDS1_V2,
	CDS1_V3,
	CDS1_D1,
	CDS1_D2,
	CDS1_D3,
	CDS2_H1,
	CDS2_H2,
	CDS2_H3,
	CDS2_V1,
	CDS2_V2,
	CDS2_V3,
	CDS2_D1,
	CDS2_D2,
	CDS2_D3,
	CDS3_H1,
	CDS3_H2,
	CDS3_H3,
	CDS3_V1,
	CDS3_V2,
	CDS3_V3,
	CDS3_D1,
	CDS3_D2,
	CDS3_D3,
	TDS_ITEM_MAX
};
#define TDS_BAND_NUM TDS_ITEM_MAX
#define TSD_CURVE_POINTSNUM 2
#define TSD_CURVE_SLOPESNUM 2

typedef struct {
    UINT8 u1AppType;
    UINT16 u2SrcWidth;
    UINT16 u2SrcHeight;
    UINT16 u2OutWidth;
    UINT16 u2OutHeight;
    UINT32 u4BitRate;
}MMAppInfo;

enum
{
    R_DITHER,
    E_DITHER,
    E_R_DITHER,
    LFSR_DITHER,
    ROUNDING,
    DITHER_OFF    
};

typedef enum
{
    DUMP_ENG_NONE   = 0x00,
    DUMP_ENG_OD     = 0x01,
    DUMP_ENG_PSCAN  = 0x02,
    DUMP_ENG_MJC    = 0x04,
    DUMP_ENG_GRAPHIC      = 0x08, 
    DUMP_ENG_Y      = 0x10,     // not use now
    DUMP_ENG_Z      = 0x20,     // not use now
    DUMP_ENG_MAX
} DUMP_ENG_SEL_T;

typedef enum
{
	DUMP_STATE_IDLE = 0,
	DUMP_STATE_INIT,
	DUMP_STATE_START,
	DUMP_STATE_END,
} DUMP_STATE_T;

typedef enum
{
    DUMP_FMT_NONE            = 0x00,
    DUMP_FMT_RGB_444_10BIT   = 0x01,
    DUMP_FMT_Y_8BIT          = 0x02,
    DUMP_FMT_YCBCR_444_10BIT = 0x04,
    DUMP_FMT_MAX
} DUMP_FMT_T;

#ifdef CC_SUPPORT_PREPROC_TIMING
typedef enum
{
    T_480P,
    T_525I,
    T_576P,
    T_625I,
    T_720P_60,
    T_1080P_60,
    T_1080I,
    T_1024_768_60,
    T_4096_2160_24,
}PRE_PROC_TIMING;
#endif
/*****************************************************************************************/
/**************************  General Register Access   ***********************************/
/*****************************************************************************************/
UINT8 bDrvCheckPqReg(UINT32* pu4Table, UINT32 u4Size);
void vDrvSetPqReg(UINT32* pu4Table, UINT32 u4Size);
void vDrvGetPqReg(UINT32* pu4Table, UINT32 u4Size);
void vDrvWritePostRegBuffer(UINT32 u4Addr, UINT32 u4Value, UINT32 u4Mask);
void vDrvFlushPostRegBuffer(void);

/*****************************************************************************************/
/*************************************  General   ****************************************/
/*****************************************************************************************/
void vDrvYcprocInit(void);
void vDrvYcprocProc(void);
void vDrvMMAPPorc(void);
void vDrvDBLInit(void);
void vDrvOPCProc(void);  
void vDrvDBLSWInit(void);
void vDrvDBLHWInit(void);
void vDrvPreprocResume(void);
void vDrvPostprocSuspend(void);
void vDrvPostprocResume(void);

/*****************************************************************************************/
/*********************************   3x3 Matrix   ****************************************/
/*****************************************************************************************/
void vVdoSetColorTransform(UINT8 bPath);
void vDrvColorTransform(UINT8 bPath);
void vLoadColorMatrix(UINT8 bPath, const UINT16 * pwTable);
void vLoadOSColorMatrix(UINT8 bPath, const INT32 * pwTable);
void vDrvMatrixOnOff(UINT8 bPath, UINT8 bOnOff);
void vDrvOSMatrixOnOff(UINT8 bPath, UINT8 bOnOff);
void vDrvGlobalPQAdj(void);
UINT8 vDrvGetColorSystem(UINT8 bPath);

/*****************************************************************************************/
/************************************   CTI   ********************************************/
/*****************************************************************************************/
void vDrvCTIFOnOff(UINT8 bOnOff);
void vDrvCTIFInit(void);
void vDrvCTIFParam(void);
void vDrvCTIROnOff(UINT8 bOnOff);
void vDrvCTIRInit(void);
void vDrvCTIRParam(void);

/*****************************************************************************************/
/*************************************   CBE   ******************************************/
/*****************************************************************************************/
#if defined (CC_MT5399)
void vDrvCBEOnOff(UINT8 bOnOff);
void vDrvCBEInit(void);
void vDrvCBELoadParam(void);
#endif
UINT8 u1DrvCBEIsBypass(void);

/*****************************************************************************************/
/************************************   LTI   ********************************************/
/*****************************************************************************************/
void vDrvLTIOnOff(UINT8 bOnOff);
void vDrvLTIParam(void);

/*****************************************************************************************/
/*********************************  HSHARP FRONT *****************************************/
/*****************************************************************************************/
void vDrvPreSharpOnOff(UINT8 bPath, UINT8 bOnOff);
void vDrvPreSharpInit(void);
void vDrvPreSharpParam(UINT8 bPath);

/*****************************************************************************************/
/************************************  2DSHARP *******************************************/
/*****************************************************************************************/
void vDrvPostSharpOnOff(UINT8 bOnOff);
void vDrvLIPostSharpOnOff(UINT8 bOnOff);
void vDrvPostSharpHorizontalOnOff(UINT8 bOnOff);
void vDrvPostSharpVerticalOnOff(UINT8 bOnOff);
void vDrvPostSharpDiagonalOnOff(UINT8 bOnOff);
void vDrvPostSharpInit(void);
void vDrvPostSharpParam(void);
void vDrvPostSharpParamHorizontal(void);
void vDrvPostSharpParamVertical(void);
void vDrvPostSharpParamDiagonal(void);
void vDrvYlevSharpInit(void);
void vDrvLoadYLevTable(void);
void vDrvSetYLevTable(UINT8* pYLev);
void vDrvCalYLevTable(void);
void vDrvCalYLevAdlTable(void);
void vDrvCalYLevAplTable(void);
void vDrvAdapYLev(void);
void vDrvPostSharpParamPBC(void);
void vDrvMainSharpPatGen(UINT8 bOnOff);
void vDrvGetSharp3DWeighting(void);
UINT16 wDrvGetTDShp3DGain(UINT16 wInputGain, UINT16 wWeighting);
UINT8 vDrvTDSharpOffset(UINT8 bBand, UINT8 bItem, UINT8 bValue);
void vDrvLoadTDSCurve(void);
void vDrvSharpnessBypass(UINT8 bOnOff);
void vDrvSetGlobalCoring(UINT8 bValue);
void vDrvTDSharpOffsetInit(void);
void vDrvPostSharpGainHOffset(UINT8 u1GainH1, UINT8 u1GainH2, UINT8 u1GainH3);
void vDrvPostSharpGainVOffset(UINT8 u1GainV1, UINT8 u1GainV2, UINT8 u1GainV3);
void vDrvPostSharpGainDOffset(UINT8 u1GainD1, UINT8 u1GainD2, UINT8 u1GainD3);
void vDrvPostSharpCorzHOffset(UINT8 u1CorzH1, UINT8 u1CorzH2, UINT8 u1CorzH3);
void vDrvPostSharpCorzVOffset(UINT8 u1CorzV1, UINT8 u1CorzV2, UINT8 u1CorzV3);
void vDrvPostSharpCorzDOffset(UINT8 u1CorzD1, UINT8 u1CorzD2, UINT8 u1CorzD3);

/*****************************************************************************************/
/*****************************   GLOBAL PQ ADJUSTMENT   **********************************/
/*****************************************************************************************/
void vDrvSetContrast(UINT8 bPath, UINT16 u2Value);
void vDrvSetBrightness(UINT8 bPath, UINT16 wValue);
void vDrvSetSaturation(UINT8 bPath, UINT16 u2Value);
void vDrvSetHue(UINT8 bPath, UINT16 wValue);
void vDrvVideoColorMatrix(UINT8 bPath);
void vDrvCalVideoColorMatrix(UINT8 bPath, UINT16 wBriValue, UINT16 wContValue, UINT16 wSatValue, UINT16 wHueValue);

/*****************************************************************************************/
/*****************************   External PQ ADJUSTMENT   **********************************/
/*****************************************************************************************/
void vDrvSetExtContrast(UINT8 bPath, UINT16 wValue);
void vDrvSetExtBrightness(UINT8 bPath, UINT16 wValue);
void vDrvSetExtSaturation(UINT8 bPath, UINT16 wValue);
void vDrvSetExtHue(UINT8 bPath, UINT16 wValue);

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/
void vDrvSCEInit(void);
void vDrvSCEHWInit(void);
void vDrvSCESWInit(void);
void vDrvSCEParam(void);
void vDrvSCELoadTable(void);
void vDrvSCECalculate(void);
void vDrvSCEFindColor(void);
void vDrvSCEAngularReset(void);
void vDrvSCEAllOnOff(UINT8 bOnOff);
void vDrvSetSCELevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain);
void vDrvSetSCEContrast(UINT16 u2Value);
void vDrvSetSCEBrightness(UINT16 u2Value);
void vDrvSetSCESaturation(UINT16 u2Value);
void vDrvSetSCEHue(UINT16 u2Value);
void vDrvSetSkinLevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain);
void vDrvSCEPatGen(UINT8 bMode, UINT16 Ydata);
void vDrvSCEAngularSetVal(UINT8 bmodule, UINT8 bColor, UINT8 bValue);
UINT8 bDrvSCEVerifyTable(void);
UINT32 vDrvSCELoadIdx(UINT32 u4FunIdx, UINT32 u4HueIdx, UINT32 u4Value);
void vDrvBitPlusProc(void);
void vDrvBitPlusSetYRange(UINT8 u1Value);
void vDrvBitPlusSetSRange(UINT8 u1Value);
void vDrvBitPlusSetYShift(UINT8 u1Value);
void vDrvBitPlusSetSShift(UINT8 u1Value);
void vDrvSetSCEContrast(UINT16 u2Value);
void vDrvSetSCEBrightness(UINT16 u2Value);
void vDrvSetSCESaturation(UINT16 u2Value);
void vDrvSetSCEHue(UINT16 u2Value);
UINT8 u1DrvBitPlusSIsBypass(void);
UINT8 u1DrvBitPlusYIsBypass(void);
UINT8 u1DrvSharpnessIsBypass(void);
void vDrvSCECalculateOnOff(UINT8 bOnOff);


/*****************************************************************************************/
/***********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vDrvColorDemo(UINT8 bOnOff, UINT8 bInside);
void vDrvSetColorDemoRange(Region rRegion);
void vDrvSetPostSharpDemoRange(Region rRegion);
void vDrv2DSharpDemo(UINT8 bOnOff, UINT8 bInside);
void vDrvECTIDemo(UINT8 bOnOff, UINT8 bInside);
void vDrvHSharpDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);


/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void vDrvGammaOnOff(UINT8 bOnOff);
void vDrvGammaDecodeTable(void);
void vDrvGammaWriteTable(void);
void vDrvGammaReadTable(UINT16 u2GammaTable[3][257]);
UINT8 bDrvGammaVerifyTable(void);
void vDrvGammaMute(UINT16 wGammaMuteR, UINT16 wGammaMuteG, UINT16 wGammaMuteB);
void vDrvGammaUnmute(void);
void  vDrvGammaKickPingPong(void);
void vDrvSetGammaWritePingPongFlag(BOOL bfgOn);
BOOL bDrvGetGammaPingPongFlag(void);
void vDrvGammaInterpolation(UINT32 u4Count, INT32* iCtlPos, INT32* iCtlValue, INT32* OutputLut);
void vDrvGammaProc(void);
void vDrvGammaInitSRAM(void);
/*****************************************************************************************/
/************************************  AMBILIGHT  ****************************************/
/*****************************************************************************************/
void vDrvAmbilightDemoMode(UINT8 bOnOff);
void vDrvAmbilightSetWindow(UINT8 bWindowNum, const structAmbilightWindow *pAmbiWindow);

/*****************************************************************************************/
/**********************************   AMBILIGHT V2 ***************************************/
/*****************************************************************************************/
void vDrvAmbiLitInit(void);
void vDrvAmbiLitSetMaskMode(UINT8 u1MaskMode);
void vDrvAmbiLitUpdateLine(UINT16 * u2AmbiLitArray, UINT8 * u1AmbilBlk, UINT8 u1Line);
UINT8 u1DrvAmbiLitGetStrapID(void);

/*****************************************************************************************/
/**************************************  xvYCC  *******************************************/
/*****************************************************************************************/
void vDrvxvYCCOnOff(UINT8 bOnOff);
void vDrvxvYCCGammaOnOff(UINT8 bOnOff);
void vDrvxvYCCAntiCGammaOnOff(UINT8 bOnOff);
void vDrvxvYCC3x3OnOff(UINT8 bOnOff);
void vDrvxvYCCSetPath(UINT8 bPath);
void vDrvxvYCCLoad3x3Matrix(const UINT16 * pwTable);
UINT8 bDrvGetxvYCCOnOff(void);
void vDrvxvYCCSetClip(UINT8 bOnOff);
void vDrvSetxvYCCPatGenOnOff(UINT8 bOnOff);
void vDrvSetxvYCCPatGenRGB(UINT16 wMuteR, UINT16 wMuteG, UINT16 wMuteB);

/*****************************************************************************************/
/************************************ BLUE STRETCH ** **************************************/
/*****************************************************************************************/
void vDrvSetBlueStretchOnOff(UINT8 bPath, UINT8 bOnOff);
void vDrvSetBlueStretchParam(void);
UINT8 bDrvGetBlueStretchOnOff(void);
void vDrvSetBlueStretchBase(void);
void vDrvSetBlueStretchBaseExt(UINT8 u1Idx, UINT8 u1BSBase);
void vDrvSetBlueStretchParamExt(UINT8 u1Idx, UINT8 u1BSGainR, UINT8 u1BSGainG, UINT8 u1BSGainB);

/*****************************************************************************************/
/************************************ RGB GAIN Offset **************************************/
/*****************************************************************************************/
void vDrvSetRGain1(UINT8 bPath, UINT16 wValue);
void vDrvSetGGain1(UINT8 bPath, UINT16 wValue);
void vDrvSetBGain1(UINT8 bPath, UINT16 wValue);
void vDrvSetROffset11(UINT8 bPath, UINT16 wValue);
void vDrvSetGOffset11(UINT8 bPath, UINT16 wValue);
void vDrvSetBOffset11(UINT8 bPath, UINT16 wValue);
void vDrvSetROffset12(UINT8 bPath, UINT16 wValue);
void vDrvSetGOffset12(UINT8 bPath, UINT16 wValue);
void vDrvSetBOffset12(UINT8 bPath, UINT16 wValue);
UINT16 wDrvGetRGain1(UINT8 bPath);
UINT16 wDrvGetGGain1(UINT8 bPath);
UINT16 wDrvGetBGain1(UINT8 bPath);
UINT32 dwDrvGetROffset11(UINT8 bPath);
UINT32 dwDrvGetGOffset11(UINT8 bPath);
UINT32 dwDrvGetBOffset11(UINT8 bPath);
UINT32 dwDrvGetROffset12(UINT8 bPath);
UINT32 dwDrvGetGOffset12(UINT8 bPath);
UINT32 dwDrvGetBOffset12(UINT8 bPath);

/*****************************************************************************************/
/***********************************  3D CMS *********************************************/
/*****************************************************************************************/
void vDrv3DCMSInit(void);
void vDrv3DCMSModuleBypass(UINT8 u1Bypass);
void vDrv3DCMSWindowOnOff(UINT8 u1OnOff, UINT8 u1WindowID);
void vDrv3DCMSSetInk(UINT8 u1OnOff, UINT8 u1InkMode);
void vDrv3DCMSSetWeight(UINT8 u1WindowID, UINT8 u1ColorAxis, UINT16 u2WeightLSlope, UINT16 u2WeightUSlope);
void vDrv3DCMSSetCurve(UINT8 u1WindowID, UINT8 u1ColorAxis, CMSCtrlPt rCMSCtrlPt, CMSGain rCMSGain);
void vDrv3DCMSGetBound(UINT8 u1WindowID, UINT8 u1ColorAxis, UINT8 *u1CMSBoundL, UINT8 *u1CMSBoundU);
UINT8 u1Drv3DCMSIsBypass(void);

/*****************************************************************************************/
/********************************  Internal Driver *********************************************/
/*****************************************************************************************/
void vDrvSceDbgInfo(UINT16 u2PosX, UINT16 u2PosY, UINT16 u2YCbCrIn[3], UINT16 u2YCbCrOut[3]);
void vDrvOsDbgInfo(UINT16 u2PosX, UINT16 u2PosY, UINT16 u2RGB[3]);

/*****************************************************************************************/
/***************************************  Misc  *********************************************/
/*****************************************************************************************/
void vDrvSet422(UINT8 bPath, UINT8 bOnOff);
UINT8 bDrvGetPeUiDebugMode(void);
void vDrvSetPeUiDebugMode(UINT8 bOnOff);
void vDrvPreprocEnablePattern(UINT8 bPath,UINT8 bPattern);
void vDrvPreprocSetFieldResync(UINT8 bPath);
#ifdef CC_SUPPORT_PREPROC_TIMING
void vDrvSetPreProcTiming(UINT8 bPath,PRE_PROC_TIMING mTiming);
UINT8 bPreProcRefreshRate(void);
UINT16 u2PreProcHTotal(UINT8 bPath);
UINT16 u2PreProcVTotal(UINT8 bPath);
UINT16 u2PreProcInputWidth(UINT8 bPath);
UINT16 u2PreProcInputHeight(UINT8 bPath);
void vDrvEnablePreprocSelfGen(UINT8 bPath,UINT8 bOnOff);
void vDrvSetOmuxTimToPreProc(UINT8 bPath,UINT8 bOnOff);
#endif
void vDrvSetOstgOptPos(UINT8 bOpt);
void vDrvSetDefaultMatrix(UINT8 bPath, BOOL OnOff);

void vDrvDumpProc(void);
void vDrvDumpSetSize(UINT16 u2DumpWidth, UINT16 u2DumpHeight);
void vDrvDumpSetShrinkSize(UINT16 u2ShrinkWidth, UINT16 u2ShrinkHeight);
void vDrvDumpSetContinuousMode(UINT8 u1OnOff);
void vDrvDumpStart(UINT8 u1Module);
void vDrvDumpForceDRAMAddress(UINT32 u4DumpDRAMAddr);
void vDrvGetDumpInfo(void);

/*****************************************************************************************/
/********************************** CUSTOMIZE DRIVER *************************************/
/*****************************************************************************************/
void DRVCUST_MMAPProc(void);
void DRVCUST_SetBlackLvlCtrl(UINT8 bPath);
UINT8 DRVCUST_MMAPPGetTiming(UINT8 bOutputTiming);
UINT16 DRVCUST_Y2RMap(void);
UINT32 DRVCUST_HDMIRange(UINT8 bPath);
UINT32 DRVCUST_GetOSMatrix709(UINT8 bPath);
void DRVCUST_SetGamutOnOSMatrix(void);

void vDrvAmbilightISRInit(void);  
void vDrvAmbiLPrintContent(UINT8 u1Color);

#endif //_DRV_YCPROC_H_
