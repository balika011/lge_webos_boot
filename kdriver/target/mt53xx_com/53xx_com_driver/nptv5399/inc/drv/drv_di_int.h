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
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: drv_di_int.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file drv_di_int.h
 *  Brief of file drv_di_int.h.
 *  Details of file drv_di_int.h (optional).
 */

#ifndef DRV_DI_INT_H
#define DRV_DI_INT_H

#include "drv_di.h"


#define DI_BE_HIST_BIN              12
#define DI_CS_SEARCH_RANGE_X        15
#define DI_CS_SEARCH_RANGE_Y        5
#define DI_CS_MV_HIST_BIN           ((DI_CS_SEARCH_RANGE_X*2+1)*(DI_CS_SEARCH_RANGE_Y*2+1)+1)
#define DI_CHROMA_JAGGY_EN          1
#define DI_DTV_TEAR_EN              1

typedef enum
{
    E_DI_FILM_NONE,
    E_DI_FILM_32,
    E_DI_FILM_22, 
    E_DI_FILM_2224,
    E_DI_FILM_2332,
    E_DI_FILM_32322,
    E_DI_FILM_44,
    E_DI_FILM_55,
    E_DI_FILM_64,
    E_DI_FILM_66,
    E_DI_FILM_87,
    E_DI_FILM_KALEIDOSCOPE,
} E_DI_FILM_MODE;

typedef struct
{
    UINT32 u4HeterUp;
    UINT32 u4HeterDn;
    UINT32 u4Heter;
    UINT32 u4HeterDiff;
    UINT32 u4HomoUp;
    UINT32 u4HomoDn;
    UINT32 u4Homo;
    UINT32 u4HomoDiff;
    UINT32 u4HomoAvg;
    UINT32 u4SawInv;
    UINT32 u4SawNom;
    UINT32 u4MultibusrtCnt;
    UINT32 u4HEdgeCnt;
    UINT32 u4VEdgeCnt;
    UINT32 u4EdgeSum; 
    UINT32 u4AvgCurY;
    UINT32 u4AvgPreY;
    UINT32 u4AvgCurCb;
    UINT32 u4AvgPreCb;
    UINT32 u4AvgCurCr;
    UINT32 u4AvgPreCr;
} DI_IF_STA;

typedef struct
{
    UINT32 u4AliasCnt;
    UINT32 u4EdgeSum;
    UINT32 u4EdgeHis[DI_BE_HIST_BIN];
} DI_PE_STA;

typedef struct
{
    UINT8 u1GmvX; 
    UINT8 u1GmvY;     
    UINT8 u1GmvValid;    
    UINT8 u1MvReadValid;
    UINT16 u2HistMax;
    UINT16 u2HistCnt;  
    UINT16 u2EdgeCnt;    
    UINT16 u2NonZeroCnt;
    UINT32 u4FieldSawRank;
    UINT32 u4HistDiffAcc;
    UINT32 u4MultibusrtCnt;
    UINT8 bAllBurst;     
    UINT8 u1SmtDeconf;
    UINT8 u1MvDeconf;
    UINT16 u2SmtCnt;
    UINT16 u2SmtCntROI;
    UINT32 u4SawAB;
    UINT32 u4SawBC;    
    UINT32 u4MVHist[DI_CS_MV_HIST_BIN/2];       // 1 Index Contain 2 Hist To Save ISR Run Time
} DI_CS_STA;

typedef struct
{
    UINT8 bBlkMotion;
    UINT8 bCurFilmMode;     //Non-Film, 32_Film, 22_Film
    UINT32 u4FrmMotion;
    UINT32 u4EdgMotion;
    UINT32 u4FldMotion;
} DI_PD_STA;

typedef struct
{
     DI_IF_STA IfSta;
     DI_PE_STA PeSta;
     DI_CS_STA CsSta;
     DI_PD_STA PdSta;
     UINT32 u4IsrIndex;
} DI_PQ_STA;

EXTERN DI_PQ_STA   DiSta;

#define MV_HIST_IND(x)              (((x)%2) ? (DiSta.CsSta.u4MVHist[(x)>>1]>>16) : (DiSta.CsSta.u4MVHist[(x)>>1]&0xFFFF))
#define MV_HIST(x, y)               (MV_HIST_IND(31*(DI_CS_SEARCH_RANGE_Y-(y))+(DI_CS_SEARCH_RANGE_X-(x)))) 

typedef void (*PThreadFunc)(void *pvArgv);

typedef struct{
    PThreadFunc pThreadFunc;
    CHAR* szThreadDescription;
} DI_PQ_THREAD;

typedef enum
{
    E_OSD_DBG_NONE = 0,
    E_OSD_DBG_PSCAN,     
    E_OSD_DBG_NR,     
    E_OSD_DBG_NUM,
} E_OSD_DEBUG_IDX;


#ifdef PSCAN_AUTO_VERIFY

typedef enum{
    E_DI_CHECKING,
    E_DI_OK,
    E_DI_NG
} E_CHECKING_STATE;

EXTERN UINT8 u1InkOnOff[VDP_NS];
EXTERN UINT8 u1CrcOnOff[VDP_NS];
EXTERN UINT8 u1GetLength[VDP_NS];
EXTERN UINT16 u2FactorIdx;
EXTERN UINT16 u2LumaAvgPre;
EXTERN UINT16 u2LumaAvgCur;
#endif
typedef struct
{
    PThreadFunc* ptrPfnVSyncFunc;
    CHAR*     szVsyncDesc;
} DI_SP_VSYNC_T;

typedef struct
{
    PThreadFunc pThreadFunc;
    CHAR* szThreadDescription;
} DI_SP_THREAD;

typedef struct
{
    UINT32 u4Addr;
    UINT8  u1Msb;
    UINT8  u1Lsb;
} DI_SP_REG_T;

typedef enum{
    E_INPUT_VSYNC_RISING      = 0x0,
    E_INPUT_VSYNC_FALLING     = 0x1,
    E_INPUT_VSYNC_ADJ         = 0x2,
    E_OUTPUT_VSYNC_RISING     = 0x3,
    E_OUTPUT_VSYNC_FALLING    = 0x4,
    E_OUTPUT_VSYNC_ADJ        = 0x5,
    E_VSYNC_STATUS_MAX        = 0x6
}E_VSYNC_STATUS;

EXTERN DI_SP_VSYNC_T DISPVsyncIsr[];
EXTERN UINT32 u4DIStatusPollingCount;
EXTERN DI_SP_THREAD DISPThread[];

EXTERN UINT8 fgDIStatusPolling;
EXTERN DI_PQ_THREAD DIPQThread[];

UINT32 u4DrvDISetSPReg(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Msb, UINT32 u4Lsb);
void vDrvDIGetSPReg(void);
void _vDrvDISpVsyncIsrStatus(UINT8 u1VsyncStatus);

void vDrvDIModePQSetting(UINT8 u1VdpId,E_MIB_DI_DMODE eDIMode);
void vDrvDIInitQuality(UINT8 bPath);
void vDrvDISetQuality(UINT8 bPath);
void vDrvDIQtyMainLoop(UINT8 bPath);
void vDrvDISetQualityISR(void);
void vDrvDISetQualityISR_Ex(void);
void vDrvDIChkStatus(UINT8 u1VdpId);
void vDrvDISetInterpMode(UINT8 u1VdpId,E_MIB_DI_DMODE eDataMode);
void vDrvDISetDebugOSDModule(E_OSD_DEBUG_IDX u4Module);
void vDrvDISetDebugOSD(E_OSD_DEBUG_IDX u4Module, UINT32 u4Value);
void vDrvDISetDebugOSDMask(E_OSD_DEBUG_IDX u4Module, UINT32 Fld, UINT32 u4Value);
void vDrvDIGetCSStatusISR(void);
void vDrvDIEnableISR(UINT8 u1VdpId, UINT8 u1Enable);
void vDrvDISetDoubleBuffer(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetClkReset(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetFrcLine(UINT8 u1VdpId);
void vDrvDINonStdVMaskCfg(UINT8 u1VdpId);
void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetRegionMotionOnOff(UINT8 u1VdpId);
void _vDrvDIFieldInverse(void);

#ifdef PSCAN_AUTO_VERIFY
void vDrvSetRelatedParam(UINT8 u1VdpId, UINT32 u4Factor, UINT32 u4Width);
void vDrvDIInitInkRange(UINT16 u2Factor, UINT16 u2Width, UINT16 u2Height);
void vDrvDISetInkBlock(UINT8 u1VdpId, UINT8 u2Block);
void vDrvDIDumpAllHistogram(void);
void vDrvDISetDataSize(UINT16 u2Factor, UINT16 u2Frame);
void vDrvDIQueryInkStatus(void);
void vDrvDIQueryInkLevel(UINT16 u2Factor, UINT8 u1Block, UINT16 u2Frame);
void vDrvDIInkHistogramPolling(UINT8 u1VdpId);
void vDrvDIGetClipLength(UINT8 u1VdpId);
void vDrvDIDumpAllCrc(UINT8 u1Mode);
void vDrvDIGetCRCResult(UINT8 u1VdpId);
void vDrvDISetCrcRange(UINT8 u1VdpId, UINT16 u2Factor, UINT16 u2Width);
void vDrvDISetMotionLevel(UINT8 u1VdpId, UINT8 u1YOnOff, UINT8 u1COnOff, UINT8 u1YLevel, UINT8 u1CLevel);
void vDrvDISetDetectCRC(UINT32 u4Value);
void vDrvDISetCurrentState(void);
void vDrvDIAutoVerifyInputISR(UINT8 u1VdpId);
void vDrvDIAutoVerifyISR(UINT8 u1VdpId);
UINT8 u1DrvDICheckResult(UINT32 u4RatioIdx);
UINT8 u1DrvDIOutOfBound(void);
UINT32 u4DrvDIGetVdoinCRC(void);

UINT32 u4DrvDIGetVdoinCRC(void);
void vDrvDIISRAutoVerifyChk(void);
void vDrvDISetTestParam(UINT32 u4PrlnSetThrshld, UINT32 u1SetChkCnt, UINT8 u1Chk);	
void vDrvDIShowWidthHeight(void);
void vDrvDIRstChkSta(void);
void vDrvDISetIPTG(UINT8 fgOnOff);
E_CHECKING_STATE eGetChkSta(void);
UINT32 u4GetChkCnt(void);

void vDrvDICRCStart(UINT8 u1VdpId, UINT16 u2Option);
void vDrvDICRCRecord(UINT8 u1VdpId);
void vDrvDICRCCompare(UINT8 u1One, UINT8 u1Two);
void vDrvDICRCClear(void);
void _vDiSetSceneChg(UINT8 fgCSScene, UINT8 fgFuScene);
void vDrvDICRCISR(UINT8 u1VdpId);
void vDrvDICRCDump(UINT8 u1Mode);

#endif
#endif    // DRV_DI_INT_H
