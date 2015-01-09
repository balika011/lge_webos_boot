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
 * $Date  $
 * $RCSfile: drv_pwm.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef DRV_PWM_H
#define DRV_PWM_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_bim.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SrcPWM0 0
#define SrcPWM1 1
#define SrcPWM2 2
#define SrcPWM3 3
#define SrcPWM4 4
#define SrcPWM5 5
#define SrcPWM6 6
#define SrcPWM7 7
#define SrcPWM8 8

#define MINILVDS_IS_HSYNC 0
#define MINILVDS_IS_VSYNC 1

//OPWM0
#define PAD_PWM0_PWM0 	0	//FUNC2 

//OPWM1
#define PAD_PWM1_PWM1 	0	//FUNC2 (FUNC3 SCANPWM)
#define PAD_PWM1_GPIO48 1	//FUNC1

//OPWM2
#define PAD_PWM2_PWM2 	0	//FUNC2 

#define SCAN_PWM_LATCH_MODE_SETDATA_OFF     0
#define SCAN_PWM_LATCH_MODE_SETDATA         1
#define SCAN_PWM_LATCH_MODE_OFF             0
#define SCAN_PWM_LATCH_MODE_ON              1
#define SCAN_PWM_ALIGN_MODE_ONCE            0
#define SCAN_PWM_ALIGN_MODE_VSYNC           1

#define vDrvSetScan1Phase(u4Val) vIO32WriteFldAlign(PWM_SCAN_29, u4Val, REG_PWM_SCAN1_START_PHASE)
#define vDrvSetScan1Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_00, u4Val, REG_PWM_SCAN1_START)
#define vDrvSetScan1High(u4Val) vIO32WriteFldAlign(PWM_SCAN_01, u4Val, REG_PWM_SCAN1_HIGH)
#define vDrvSetScan1Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_02, u4Val, REG_PWM_SCAN1_LOW)

#define vDrvSetScan2Phase(u4Val) vIO32WriteFldAlign(PWM_SCAN_2A, u4Val, REG_PWM_SCAN2_START_PHASE)
#define vDrvSetScan2Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_03, u4Val, REG_PWM_SCAN2_START)
#define vDrvSetScan2High(u4Val) vIO32WriteFldAlign(PWM_SCAN_04, u4Val, REG_PWM_SCAN2_HIGH)
#define vDrvSetScan2Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_05, u4Val, REG_PWM_SCAN2_LOW)
// MT5396/68 New feature for ECO
#define vDrvSetScan3Phase(u4Val) vIO32WriteFldAlign(PWM_SCAN_2B, u4Val, REG_PWM_SCAN3_START_PHASE)
#define vDrvSetScan3Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_07, u4Val, REG_PWM_SCAN3_START)
#define vDrvSetScan3High(u4Val) vIO32WriteFldAlign(PWM_SCAN_08, u4Val, REG_PWM_SCAN3_HIGH)
#define vDrvSetScan3Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_09, u4Val, REG_PWM_SCAN3_LOW)

#define vDrvSetScan4Phase(u4Val) vIO32WriteFldAlign(PWM_SCAN_2C, u4Val, REG_PWM_SCAN4_START_PHASE)
#define vDrvSetScan4Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_0A, u4Val, REG_PWM_SCAN4_START)
#define vDrvSetScan4High(u4Val) vIO32WriteFldAlign(PWM_SCAN_0B, u4Val, REG_PWM_SCAN4_HIGH)
#define vDrvSetScan4Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_0C, u4Val, REG_PWM_SCAN4_LOW)

#define vDrvSetScan5Phase(u4Val) vIO32WriteFldAlign(PWM_SCAN_2D, u4Val, REG_PWM_SCAN5_START_PHASE)
#define vDrvSetScan5Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_0D, u4Val, REG_PWM_SCAN5_START)
#define vDrvSetScan5High(u4Val) vIO32WriteFldAlign(PWM_SCAN_0E, u4Val, REG_PWM_SCAN5_HIGH)
#define vDrvSetScan5Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_0F, u4Val, REG_PWM_SCAN5_LOW)

// MT5398 new feature
#define vDrvSetScan6Phase(u4Val) vIO32WriteFldAlign(PWM_SCAN_2E, u4Val, REG_PWM_SCAN6_START_PHASE)
#define vDrvSetScan6Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_23, u4Val, REG_PWM_SCAN6_START)
#define vDrvSetScan6High(u4Val) vIO32WriteFldAlign(PWM_SCAN_24, u4Val, REG_PWM_SCAN6_HIGH)
#define vDrvSetScan6Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_25, u4Val, REG_PWM_SCAN6_LOW)

//MT5861 and MT5882 new feature
#define vDrvSetScan7Phase(u4Val) vIO32WriteFldAlign(PWM_SCAN_2F, u4Val, REG_PWM_SCAN7_START_PHASE)
#define vDrvSetScan7Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_30, u4Val, REG_PWM_SCAN7_START)
#define vDrvSetScan7High(u4Val) vIO32WriteFldAlign(PWM_SCAN_31, u4Val, REG_PWM_SCAN7_HIGH)
#define vDrvSetScan7Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_32, u4Val, REG_PWM_SCAN7_LOW)

#define vDrvSetScan8Phase(u4Val) vIO32WriteFldAlign(PWM_SCAN_36, u4Val, REG_PWM_SCAN8_START_PHASE)
#define vDrvSetScan8Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_37, u4Val, REG_PWM_SCAN8_START)
#define vDrvSetScan8High(u4Val) vIO32WriteFldAlign(PWM_SCAN_38, u4Val, REG_PWM_SCAN8_HIGH)
#define vDrvSetScan8Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_38, u4Val, REG_PWM_SCAN8_LOW)

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
extern BOOL _fgScanPWMAuto;
extern UINT8 _u1ScanPWMTestSrc;
extern UINT8 _u1ScanPWMTestChg;
extern UINT32 _u4ScanPWMTesFreq;
extern INT32 _i4ScanPWMTestStart;
extern UINT32 _u4ScanPWMTestTop;
extern UINT32 _u4ScanPWMTestBottom;
extern UINT32 _u4ScanPWMTestHigh;
extern UINT32 _u4ScanPWMTestLow;

extern void vDrvSetPWM(UINT32 u4Src, UINT32 u4TimeBase, UINT32 u4DutyON,
                       UINT32 u4DutyAll);
void vDrvSetLock(UINT32 u4Src, UINT32 u4LockON);
UINT8 vDrvGetPWMDutyCycleRatio(void);
void vDrvInitSCANPWM(UINT8 u1Src);
extern void vDrvSetScanPWMSW(UINT8 u1Src, UINT32 u4Start, UINT32 u4High, UINT32 u4Low);
extern void vDrvSetScanPWM(UINT8 u1Src, UINT32 u4Start, UINT32 u4High, UINT32 u4Low);
extern void vDrvSetScanPWMSW_SETDATA(void);
extern void vDrvSetScanPWMSW_SETDATA_Status(void);
extern UINT8 u1DrvGetScanPWMStruct_Ready(void);
extern void vDrvSetScanPWMSW_StepControl(void);
extern void vDrvSetScanPWMStepControlEn(UINT8 fgEnable);
extern void vDrvSetScanPWMStepControl(UINT8 fgEnable, UINT16 u2Step);
extern void vDrvGetScanPWMStepControlStatus(void);
void vDrvSetScanPWMStartPhase(UINT32 u4Src, UINT32 u4Start_Phase);
void vDrvSetScanBLOnOff(UINT8 u1OnOff);
extern void vDrvSetScanPWMStepTarget(UINT32 u4High, UINT32 u4Low);
extern void vDrvSetCrozzUpLimit(UINT16 u2Value);
extern UINT16 u2DrvGetCrozzUpLimit(void);

void vDrvSetScanPWMPolarity(UINT32 u4Src, UINT8 u1blPol);
void vDrvSetScanPWMOnOff(UINT32 u4Src, UINT8 u1OnOff);
BOOL fgIsScanPWMSetDataLatch(void);
void vDrvSetScanPWMLatchMode(UINT8 u1SetDataMode,UINT8 u1Mode, UINT8 u1AlignMode);
void vDrvScanPWMDataFire(void);
void vDrvSetScanPWMOutBL(UINT32 u4Src, UINT8 u1OutBL);
void vDrvSetScanStartModeOnOff(UINT32 u4Src, UINT8 u1OnOff);
void vDrvSetScanBLPolarity(UINT8 u1blPol);
void vDrvSetScanBLOnOff(UINT8 u1OnOff);
void vDrvSetScanBLLatchMode(UINT8 u1Mode);
extern void vDrvSetScanFixHighLow(UINT8 channel, UINT8 mode);
extern UINT8 u1DrvGetScanFixHighLow(UINT8 channel);
extern void vDrvScanPWMAutoTest(void);

void vDrvSetISOnOff(UINT8 u1Src, UINT8 u1OnOff);
void vDrvSetISMaskNum(UINT8 u1Src, UINT8 u1MaskNum);
void vDrvSetISInputInverse(UINT8 u1Src, UINT8 u1Enable);
void vDrvSetISDelayOnOff(UINT8 u1Src, UINT8 u1OnOff);
void vDrvSetISDelayNum(UINT8 u1Src, UINT16 u2DelayNum);
void vDrvSetISHoldOnOff(UINT8 u1Src, UINT8 u1OnOff);
void vDrvSetISRelation(UINT8 u1Src, UINT8 u1SyncCnt, UINT8 u1ISSyncCnt);
extern void PWM_pm_str_suspend(void);
extern void PWM_pm_str_resume(void);


#endif
