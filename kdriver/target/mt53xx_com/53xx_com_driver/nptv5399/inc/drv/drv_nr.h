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
 * $RCSfile: drv_nr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _DRV_NR_H_
#define _DRV_NR_H_

#include "typedef.h"

#define NR_R(u4Addr, u4Fld) IO32ReadFldAlign((u4Addr), (u4Fld))

#define NR_R4B(u4Addr) u4IO32Read4B((u4Addr))

#define NR_W4B(u4Addr, u4Value) vIO32Write4B((u4Addr), (u4Value))

#define NR_W(u4Addr, u4Value, u4Fld) \
    vIO32WriteFldAlign((u4Addr), (u4Value), (u4Fld))

#define NR_WM(u4Addr, list) \
    vIO32WriteFldMulti((u4Addr), (list))

#define NR_WMSK(u4Addr, u4Value, u4Msk) \
    vIO32Write4BMsk((u4Addr), (u4Value), (u4Msk))

#define _SWR_4B(addr) u4IO32Read4B(addr)
#define _SWR(addr, fld) IO32ReadFldAlign((addr), (fld))
#define _SWW_4B(addr, val) vIO32Write4B((addr), (val))
#define _SWW(addr,val,fld) vIO32WriteFldAlign((addr), (val), (fld))
#define _SWWM(addr,list) vIO32WriteFldMulti((addr),(list))

#define NR_DEMO_MASK        (4)
#define NR_NM_SAD_HIST      (1)
#define NR_NM_TDF_BASE      (2)
#define PQL_DB_ON   0
enum
{
    NM_LEVEL1_0,
    NM_LEVEL1_1,
    NM_LEVEL1_2,
    NM_LEVEL1_3,
    NM_LEVEL2,
    NM_LEVEL3,
    NM_LEVEL4,
    NM_LEVEL5,
    NM_LEVEL6,
    NM_LEVEL7,
    NM_LEVEL8,
    NM_INVALIDLEVEL
};

typedef enum
{
    E_NR_FB_MANR = 0,
    E_NR_FB_MCNR = 1
} E_NR_FB_TYPE;

typedef enum
{
    MCNR_MODE_OFF = 0, 
    MCNR_MODE_MC  = 1,
    MCNR_MODE_MV  = 2,
    MCNR_MODE_NM  = 3,
    MCNR_MODE_MA  = 4
} E_3DNR_MODE;

typedef enum
{
    NXNR_MODE_OFF = 0,
    NXNR_MODE_ON = 1
} E_2DNR_MODE;

typedef enum
{
    E_BNM_DFT = 0,
    E_BNM_CVBS,
    E_BNM_COMP_I,
    E_BNM_COMP_P,
    E_BNM_HDMI_I,
    E_BNM_HDMI_P
} E_BNM_MODE;

typedef enum
{
    MCNR_IIR_TBL_MA = 0,
    MCNR_IIR_TBL_MC = 1,
    MCNR_IIR_TBL_C  = 2
} E_3DNR_IIR_TBL;

typedef struct
{    
    E_3DNR_MODE eMCNRMode;
    E_2DNR_MODE eNXNRMode;
    UINT8 u1Ver;
    UINT8 u1Timing;
    UINT8 u1ParamUpdate;
    UINT8 u1NoiseMeterEn;
    UINT8 u1Interlace;
    UINT8 u1IsFhdP;
    UINT8 u1MCNROnOff;
    UINT8 u1BNRSNROnOff;
    UINT8 u1SADWindowH;
    UINT8 u1SADWindowV;
    UINT8 u1NormFact;       /* Normalized factor based on FHD resolution, fractional 2bit*/
    UINT16 u2Width;
    UINT16 u2Height;
    UINT32 u4ConflictCnt;
    UINT32 u4TolBlks;
} NR_PARAM_T;

EXTERN void vDrvNRPQInit(void);
EXTERN void vDrvNRISR(void);
EXTERN void vDrvNRVBlankProc(void);
EXTERN void vDrvNXNRWritePos(BOOL u1IsH, UINT32 u4Pos, UINT8 u1OnOff);
EXTERN void vDrvNXNRWritePosLR(BOOL u1IsR, UINT32 u4Pos, UINT8 u1OnOff);
EXTERN UINT8 u1DrvNXNRReadPos(BOOL u1IsH, UINT32 u4Pos);
EXTERN void vDrvNXNRResetPos(void);
EXTERN void vDrvNXNRResetPosH(void);
EXTERN void vDrvNXNRResetPosV(void);
EXTERN UINT16 u2DrvNXNRGetVBlkLineSum(BOOL fgR, UINT32 u4Pos);
EXTERN UINT16 u2DrvNXNRGetHBlkLineSum(BOOL fgR, UINT32 u4Pos);
EXTERN UINT16 u2DrvNXNRGetVBlkPxCnt(BOOL fgR);
EXTERN UINT16 u2DrvNXNRGetHBlkPxCnt(BOOL fgR);
EXTERN void vDrvMCNRSetDftDcnrAlpha(void);
EXTERN void vDrvMCNRSetWeiTbl(E_3DNR_IIR_TBL eIIRTbl, const UINT8 arTbl[8]);
EXTERN void vDrvMCNRSetTblThd(const UINT8 arTbl[8]);
EXTERN void vDrvNRSetNMDebugInk(UINT32 u4Enable);

E_3DNR_MODE eDrvNRGet3DNRMode(void);
void vDrvNRSet3DNRMode(E_3DNR_MODE e3DMode);
void vDrvNRSet2DNRMode(E_2DNR_MODE e2DMode);
void vDrvNRSetChromaUpSample(BOOL fgOnOff);
void vDrvNRSetQuality(UINT8 u1Interlace, UINT16 u2Width, UINT16 u2Height);
void vDrvNRSetWindow(UINT16 u2Width, UINT16 u2Height);
void vDrvNRSetMoANRParamByTiming(UINT32 u4VdoTiming,E_BNM_MODE eBnmMode);

void vDrvNRSetBNRFWMode(UINT8 u1FWMode);

void vDrvNRSet3DNRParam(void);
void vDrvNRSetSNRParam(void);
void vDrvNRSetMpegNRParam(void);
void vDrvNRSet2DNRBNROnOff(UINT8 u1OnOff);
void vDrvNRSet2DNRMNROnOff(UINT8 u1OnOff);
void vDrvNRSet2DNRSNROnOff(UINT8 u1OnOff);
void vDrvNRSet2DNRAuto(UINT8 u1OnOff);
void vDrvNRSet3DNROnOff(UINT8 u1OnOff);
void vDrvNRSetDotCrawlNROnOff(UINT8 u1OnOff);
void vDrvNRSet3DNRStrength(UINT8 u1Strength);
void vDrvNRSet3DNRMaxNoiseLvl(UINT8 u1MaxLvl);
void vDrvNRSet3DNRAuto(UINT8 u1OnOff);
void vDrvNRSet3DNRAutoStrength(INT8 bStrength);

void vDrvNRSet3DNRChromaPath(UINT8 u1OnOff);
void vDrvNRSetBitRes(UINT8 u18bit);

void vDrvNRSet2DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
void vDrvNRSet3DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);

void vDrvNRSetForceBypass(UINT8 u1OnOff);
void vDrvNRGetNRStatus(void);
UINT8 u1DrvNRGetBlockMeterSupport(void);
UINT8 u1DrvNRBNMISR(void);

UINT8 u1DrvNRGetNoiseLevel(void);
UINT8 u1DrvNXNRGetBlkConf(void);

void vDrvNRInit(void);
void vDrvNRProc(void);

//=================== To be removed===============================

typedef enum 
{
	NR_CHROMA_LINE_AVERAGE_MODE_OFF,
	NR_CHROMA_LINE_AVERAGE_MODE_DYN,
	NR_CHROMA_LINE_AVERAGE_MODE_121,
}NR_CHROMA_LINE_AVERAGE_MODE;

void vDrvNRSetChromaLineAvgMode(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode);
void vDrvNRSetInk(UINT8 u1Mode, UINT8 u1Type);
void vDrvNRSetChromaLineAvgModeReg(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode);
void vDrvNRChromaLineAvgCfg(UINT8 bPath);

#endif //_DRV_NR_H_

