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

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: vdo_misc.h $
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 * Video Misc Function
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: p4admin $
 *
 * $Modtime: 04/05/31 8:25p $
 *
 * $Revision: #4 $
*************************************************************************/

#ifndef _VDO_MISC_H_
#define _VDO_MISC_H_

#include "hw_sw.h"
#include "debug.h"


#define MT8280 0
#define MT8280_E1 1
//extern UINT8 bGetICVersion(void);
//extern UINT8 _bExtmjcICVersion;

typedef struct
{
    UINT32  u4Addr;
    UINT32  u4Fld;
    char    szName[32];
} VDO_DRAM_STATUS_T;

typedef struct
{
    UINT32  u4Addr;
    UINT32  u4Fld;
} VDO_DRAM_STATUS_CLR_T;

extern void vDrvVideoSwInit(void) ;
extern void vDrvVideoHwInit(void) ;
extern UINT8 bIsScalerInput444(UINT8 bPath) reentrant;
extern UINT8 bIsScalerInputRGB(UINT8 bPath) reentrant;


extern UINT8 bDrvVideoGetType(UINT8 bPath) ;
#ifdef CC_SUPPORT_PIPELINE
extern UINT8 bDrvVideoGetTypeAVD(UINT8 bPath) ;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
void vPreProcConnect(UINT8 u1Path, UINT8 fgIsOn);
#endif
#define VDOTYPE_NTSC 0
#define VDOTYPE_PAL 1
#define VDOTYPE_720P 2
#define VDOTYPE_1080I 3
#define VDOTYPE_OTHER 4

#ifndef CC_UP8032_ATV
extern void vDrvVideoAllocateDram(void) ;
#else
void vDrvVideoSRMChk(UINT8 bPath);
#endif
void vDrvNptvResume(void);
void vDrvVideoSysClockInit(void);
// video call-back function
void vDrvVideoConnect(UINT8 bPath, UINT8 bOnOff) ;
void vDrvVideoTrigModeDetect(UINT8 bPath) ;

UINT16 wDrvVideoInputWidth(UINT8 bPath) ;
UINT16 wDrvVideoInputWidthByDec(UINT8 bDecType) ;

UINT16 wDrvVideoInputHeight(UINT8 bPath) ;
UINT16 wDrvVideoInputHeightByDec(UINT8 bDecType) ;

UINT8 bDrvVideoGetRefreshRate(UINT8 bPath)   ;
UINT8 bDrvVideoGetRefreshRateByDec(UINT8 bDecType)   ;

UINT8 bDrvVideoIsSrcInterlace(UINT8 bPath)   ;
UINT8 bDrvVideoIsSrcInterlaceByDec(UINT8 bDecType)   ;

UINT16 wDrvMDDIGetVPorch(UINT8 bPath) ;
UINT16 wDrvVideoGetVPorch(UINT8 bPath) ;
UINT16 wDrvVideoGetHPorch(UINT8 bPath) ;
UINT8 bDrvVideoSignalStatus_DBL(UINT8 bPath);
UINT8 bDrvVideoSignalStatus(UINT8 bPath)   ;
UINT8 bDrvVideoSignalStatusByDec(UINT8 bDecType)   ;

UINT16 wDrvVideoGetHTotalByDec(UINT8 bDecType) ;
UINT16 wDrvVideoGetHTotal(UINT8 bPath) ;
UINT16 wDrvVideoGetVTotalByDec(UINT8 bDecType) ;
UINT16 wDrvVideoGetVTotal(UINT8 bPath) ;
UINT8 bDrvVideoGetTiming(UINT8 bPath) ;
const CHAR* strDrvVideoGetTimingString(UINT8 bTiming);
UINT8 bDrvVideoGetWidthOverSample(UINT8 bPath) ;
UINT8 bDrvVideoGetWidthOverSampleByDec(UINT8 bDecType) ;

extern UINT8 bDrvVideoIsMainSrcTV(void);
extern UINT8 bDrvVideoGetSourceTypeTiming(UINT8 bPath) reentrant;
extern UINT16 wDrvVideoInputDEWidth(UINT8 bPath);
extern UINT16 bDrvVideoGetFrameDelay(UINT8 bPath);

extern UINT8 bDrvPorchTune[2]; //porch tune mode
extern UINT8 bDrvGetPorchTune(UINT8 bPath);
UINT16 wDrvVideoGetPorchByDec(UINT8 bDecType, UINT8 bPorchType);

extern void vDrvSetPorchTune(UINT8 bPath, UINT8 bModemsk);

//#define bDrvGetPorchTune(bPath)			bDrvPorchTune[bPath]
//#define vDrvSetPorchTune(bPath,bModemsk)	(bDrvPorchTune[bPath]=(bModemsk))

extern UINT16 wDrvVideoPorchStd(UINT8 bPath, UINT8 bPorchType);
extern UINT16 wDrvVideoGetPorch(UINT8 bPath, UINT8 bPorchType);
extern UINT16 wDrvVideoSetPorch(UINT8 bPath, UINT8 bPorchType, UINT16 wValue);

extern UINT16 wTvdGetPorch(UINT8 bPath, UINT8 bPorchType); /* bPath is for wDrvVideoPorchStd()*/
extern UINT16 wVgaGetPorch(UINT8 bPath, UINT8 bPorchType);
extern UINT16 wHdtvGetPorch(UINT8 bPath, UINT8 bPorchType);
extern UINT16 wDviGetPorch(UINT8 bPath, UINT8 bPorchType);
extern UINT8 u1DrvDIGetBob (UINT8 u1VdpId);
//extern void vVDOIN_MPCLK_Setting(void);
//extern void vTvdSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue);
extern void vVgaSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue);
extern void vHdtvSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue);
extern void vDviSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue);
extern void vDrvMainSubFifoReset(UINT8 bPath);
//if current signal is non-standard signal
extern UINT8 bDrvVideoIsNonStandardSignal(UINT8 bPath);

#if 0
// DXX_HV_POS
UINT16 wTVDecGetHPos(UINT8 bPath);
UINT16 wTVDecAdjHPos(UINT8 bPath, UINT8 dir, UINT16 value);
UINT16 wTVDecGetVPos(UINT8 bPath);
UINT16 wTVDecAdjVPos(UINT8 bPath, UINT8 dir, UINT16 value);
INT16 wDrvAdjHPos(UINT8 bPath, UINT8 dir, UINT16 value);
INT16 wDrvAdjVPos(UINT8 bPath, UINT8 dir, UINT16 value);
#endif
//extern void vApiRedoTwoChipConnection(BOOL fgEnable, UINT8 u1Path);
extern UINT8 bDrvVideoGetWidthOverSample(UINT8 bPath);
extern UINT32 u4DrvVideoGetNSDOffset(UINT8 bPath);

 typedef struct 
{
	UINT32 u4Reg;
	UINT32 u4Value;
	UINT32 u4Mask;
} REGTBL_T;

#define REGTBL_END	0xFFFFFFFF

void vDrvLoadRegTbl(REGTBL_T const * pRegTbl) reentrant;

#ifdef  CC_SCPOS_PATTERN_GENERATOR
void vSetScposPatternGen(UINT8 u1Enable);
UINT8 u1GetScposPtGenEnable(void); 
#endif

UINT32 u43DGetOutputLR(void);
void vDrvVideoQueryInputTimingInfo(UINT32 u4Path);
UINT8 u1DrvVideoGetIs44PullDownInfo(void);
void vDrvVideoDramStatusRst(void);
void vDrvVideoDramStatusChk(void);
extern UINT32 u4VdoDramStatusMonCount;
extern void vDrvForceFrameRate(UINT8 u1FrameRate);
#if  CC_CHECK_DEC_STABLE
extern void vCheckDeocerStable(UINT8 bState) ;
#else
#define vCheckDeocerStable(x)   
#endif
void vSetHDMIColorMode(UINT8 bPath, UINT8 bMode);
UINT8 u1GetHDMIColorMode(void);
void vSetVGAColorMode(UINT8 bMode);
UINT8 u1GetVGAColorMode(void);

extern    void vShowOmuxStatus(UINT8 u1Path);
BOOL fgIsVideoDone(UINT8 bPath);

//PQ dump
extern void CopyScalerDRAM(void);
extern void vTriggerScalerDumpCopyDram(void);

//4k2k
BOOL fgIs4k2kNativeTimingOut(UINT8 bPath);
BOOL fgIs4k2kScalerBypass(UINT8 bPath);
BOOL fgIs4k2kPhoto3DSupport(UINT8 bPath);

UINT8 bDrvVideoIsDecDualMode(void);
void vDrvVideoForceDualMode(UINT8 u1OnOff);
BOOL fgIsVideoDone(UINT8 bPath);

#endif
