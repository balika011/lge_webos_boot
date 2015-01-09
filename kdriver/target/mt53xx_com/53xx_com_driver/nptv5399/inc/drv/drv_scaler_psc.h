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
 * $RCSfile: drv_scaler.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_SCALER_PSC_H_
#define _DRV_SCALER_PSC_H_

#include "hw_scpos.h"
#include "panel.h"
#include "drv_display.h"
#include "frametrack_drvif.h"

#define PSC_WRITE_FLD(reg,value,fld) vIO32WriteFldAlign(reg, value, fld)
#define PSC_READ_FLD(reg,fld) IO32ReadFldAlign(reg, fld)

//*****************************************************************************
//  drv_scaler_psc.c
//*****************************************************************************
void vPscSwInit(void);
void vPscHwInit(void);
void vPscPreInit(void);
void vPscFireModeChanged(void);
void vPscClearModeChanged(void);
UINT8 fgPscIsModeChanged(void);
void vPscSetImportOff(UINT8 bOnOff);
void vPSCDumpResInfo(void);
void vPscPrintCurrentScale(void);

BOOL bPscIsNeedSetData(void);
void vPscSetNeedSetDataFlag(UINT8 bFlag);
void vPscUpdateReg_SETDATA(void);

/* API for PSC frame track */
void vPscConfigFrameTrack(UINT32 u4Step);
void vPscSetFrameTrackOnOff(UINT32 u4OnOff);

void vPscNormProc(void);

//*****************************************************************************
//  drv_scaler_psc_hw.c
//*****************************************************************************

// functions for time gen.
void vPscSetPanelWidth(UINT32 u4Width);
void vPscSetPanelHeight(UINT32 u4Height);
void vPscSetPanelHTotal(UINT32 u4HTotal);
void vPscSetPanelVTotal(UINT32 u4VTotal);
UINT32 u4PscGetPanelWidth(void);
UINT32 u4PscGetPanelHeight(void);
UINT32 u4PscGetPanelHTotal(void);
UINT32 u4PscGetPanelVTotal(void);
UINT32 u4PscGetOutputHActiveStatus(void);
UINT32 u4PscGetOutputVActiveStatus(void);
UINT32 u4PscGetOutputHTotalStatus(void);
UINT32 u4PscGetOutputVTotalStatus(void);
UINT32 vPscGetInputHactive(void);
UINT32 vPscGetInputHtotal(void);
UINT32 vPscGetInputVactive(void);
UINT32 vPscGetInputVtotal(void);
void vPscSetPanelHSyncFp(UINT32 u4FrontPorch);
void vPscSetPanelVSyncFp(UINT32 u4FrontPorch);
void vPscSetPanelHSyncBp(UINT32 u4BackPorch);
void vPscSetPanelVSyncBp(UINT32 u4BackPorch);
UINT32 u4PscGetPanelHSyncFp(void);
UINT32 u4PscGetPanelVSyncFp(void);
UINT32 u4PscGetPanelHSyncBp(void);
UINT32 u4PscGetPanelVSyncBp(void);
UINT8 u1PscGetInDispR(void);
UINT8 u1PscGetOutDispR(void);
void vPscSetData(UINT8 bOnOff);
void vPscTimegenImportEnable(UINT8 bOnOff);

//functions for register update
void vPscSetResImportEnable(UINT8 bOnOff);
void vPscSetResToggle(UINT8 bOnOff);
void vPscSetInputWidthHeight(UINT32 u4Width, UINT32 u4Height);
void vPscSetMjcDispInterfaceHtotal(UINT32 u4Htotal);
void vPscSetDispFifoWaterLevel(UINT32 u4WaterLevel);
void vPscSetHInitPhase(UINT32 u4InitSign, UINT32 u4InitIntg, UINT32 u4InitFrac);
void vPscSetHDeltaPhase(UINT32 u4DeltaIntg, UINT32 u4DeltaFrac);
void vPscSetHmskStartOfst(UINT32 u4HStart);
void vPscSetVmskStartOfst(UINT32 u4VStart);
void vPscSetOutputWidthHeight(UINT32 u4Width, UINT32 u4Height);

//functions for in/ out pattern gen.
void vPscSetInputPatternGenEnable(UINT8 bOnOff);
void vPscSetInputPatternGenBoundaryEnable(UINT8 bOnOff);
void vPscSetInputPatternGenBoundary(UINT32 u4Hboundary,UINT32 u4Vboundary);
void vPscSetOutputPatternGenEnable(UINT8 bOnOff);
void vPscSetOutputPatternGenBoundaryEnable(UINT8 bOnOff);
void vPscSetOutputPatternGenBoundary(UINT32 u4Hboundary,UINT32 u4Vboundary);

//functions for frame track
void vPscSelectFrameTrackVsyncInEdge(UINT8 bOnOff);
void vPscSetDddsTrackFrameTurboSpeed(UINT32 u4Speed);
void vPscSetFrameTrackStep(UINT32 u4Step);
void vPscSetFrameTrackLockWin(UINT32 u4LockWin);
void vPscSetFrameTrackLockTolerateLineNum(UINT32 u4LineNum);
void vPscSetFrameTrackTurboRegion(UINT32 u4Region);
void vPscSetFrameTrackTargetDistance(UINT32 u4Distance);
void vPscSetDddsFrameTrackEnable(UINT8 bOnOff);
void vPscMaskInputFrame(UINT32 u4Frame);
void vPscSelectOsdVsync(UINT8 bOnOff);
void vPscSetInvOsdHVSync(UINT8 bInvHSync, UINT8 bInvVSync);
#endif
