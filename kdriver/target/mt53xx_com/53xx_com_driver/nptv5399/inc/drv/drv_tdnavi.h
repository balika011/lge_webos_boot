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
 * $RCSfile: drv_tdnavi.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_TD_NAVI_H_
#define _DRV_TD_NAVI_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "typedef.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"
#include "video_timing.h"

//#define NAVI_LINE_WIDTH     32
#define NAVI_LINE_WIDTH     30
#define NAVI_LINE_HEIGHT    16

EXTERN HANDLE_T _hTDNAVIDumpLock;
EXTERN HANDLE_T _hTDNAVIDumpStart;
EXTERN HANDLE_T _hTDNAVIVCheck;
EXTERN HANDLE_T _hTDNAVIVLoopCheck;


EXTERN UINT32 eUINaviPatternDetect;

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
 
typedef enum
{
    TD_NAVI_DETECT_2D = 0,
    TD_NAVI_DETECT_SBS,
    TD_NAVI_DETECT_TAB,
    TD_NAVI_DETECT_LI,
    TD_NAVI_DETECT_CI,
    TD_NAVI_DETECT_CB,
    TD_NAVI_DETECT_QC,
    TD_NAVI_DETECT_FS,
    TD_NAVI_DETECT_UNKNOWN,
#if 0 // For Interface , Should not return unkown status to other module
    TD_NAVI_DETECT_RESERVED,
    TD_NAVI_DETECT_TOO_FEW,
    TD_NAVI_DETECT_AMBIGUO,
    TD_NAVI_DETECT_NEIGHBO,
#endif
    TD_NAVI_DETECT_MAX
}TD_NAVI_DETECT;

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/

// 3D NAVI
void vDrvTdNaviSetPicSize(void);
UINT8 u1DrvTdNaviModeChange(void);
void vDrvTdNaviInit(void);
void vDrvTdNaviMainloop(void);
UINT8 vDrvTdNaviGetSbsCnt(void);
UINT8 vDrvTdNaviGetTabCnt(void);
UINT8 vDrvTdNaviGetSbsVldBlk(void);
UINT8 vDrvTdNaviGetTabVldBlk(void);
UINT8 vDrvTdNaviGetNewDetType(void);
UINT8 vDrvTdNaviGetCurrDetType(void);
void vDrvTdNaviSetSimilarityRatio(UINT8 u1Ratio);
void vDrvTdNaviSetSimilarityThr(UINT8 u1Thr);
void vDrvTdNaviSetDecisionType(UINT8 u1Type);
void vDrvTdNaviSetDbgType(UINT8 u1Type);
void vDrvTDTVTDNAVIStressTest(UINT32 count);
void vDrvTdNaviAtInputISR(void);

//Extra Format detection

typedef enum
{
	TD_NAVI_V2_DETECT_LI=0,    
	TD_NAVI_V2_DETECT_CI,
    TD_NAVI_V2_DETECT_CB,
    TD_NAVI_V2_DETECT_QC,
    TD_NAVI_V2_DETECT_2D,
    TD_NAVI_V2_DETECT_UNKNOWN,
    TD_NAVI_V2_DETECT_MAX
}TD_NAVI_V2_DETECT;

void vDrvTdNaviV2Det(void);
void vDrvTdNaviV2ModeChange(void);
void vDrvTdNaviV2Init(void);

extern UINT8 g_u1TdNaviV2DetCurr;
extern UINT8 g_u1TdNaviV2DetNew;


// REALD

enum
{
    REALD_DETECT_2D = 0,
    REALD_DETECT_RD,
    REALD_DETECT_MAX
};

void vDrvRealDInit(void);
void vDrvRealDMainloop(UINT8 u1VdpId);
void u1DrvRealDMaskOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
void u2DrvRealDQuery(void);

UINT8 u1DrvRealDModeChange(UINT8 u1VdpId);
UINT8 u1DrvRealDGetCurrDetType(UINT8 u1VdpId);


//SW TDNAVI ,TDNAVI PLUS
TD_NAVI_DETECT eDrvSwNaviDetectedFmtQuery(void);
void vDrvTdNaviFWDetectionInit(void);
void vDrvTdNaviFWDetectionProc(void);

void vDrvTDTVTDNAVIDumpInit(void);
void vDrvTDTVTDNAVIDumpBuf(void *pvArgs);
void vDrvTDTVTDNAVIFillBuf(void);

//3D TO 2D
typedef struct
{
    UINT16 u2Width;
    UINT16 u2Height;
    UINT8 u1TD22DEn;
    UINT8 u1MaskSel;
    UINT8 u1ModeSel;
    UINT8 u1IsIntr;
}TD22D_INFO;

UINT32 vDrvTTTIsFldInv(UINT32 u1VdpId);

EXTERN TD22D_INFO gMain3DTo2DInfo, gPip3DTo2DInfo;

#endif //#ifndef _DRV_TD_NAVI_H_
