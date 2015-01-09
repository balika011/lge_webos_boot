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
 * $RCSfile: drv_ttd.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_TTD_H_
#define _DRV_TTD_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "typedef.h"
#include "video_def.h"

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
#define SUPPORT_TTD_FULL_SCREEN_MODE 0
#define SUPPORT_TDDC_FULL_SCREEN_MODE 0
#define SUPPORT_BLACK_SINGLE_MASK 1
#define TTD_CHECK_INV_VER TTD_CHECK_INV_V3

#define DISPARITY_SCALAR_TDDC 0x200
#define CONVERGENCE_DEPTH_3D 192
#define DEPTH_RANGE_RATIO_3D 0
#define DISPARITY_GAIN_3D 0
#define MAX_TTD_RST_COUNTER 3 
#define STEREO_COMFORT_FACTOR 40 // Set this value < parallax (for children = 50 mm)
#define TTD_BLACKBAR_BUF_SIZE 8

#define UI_DSPT_SCALAR       0x1
#define UI_GLOBAL_SFT        0x2
#define UI_SW_GLOBAL         0x4
#define UI_SW_LOCAL          0x8
#define UI_SW_GLOBAL_SFT     0x10
#define UI_BLACK_BAR         0x20

#define DRAM_DUMP_DBG   0
#define LSV_DUMP_MAX_SIZE       (1920*16)
#define LSV_DES_WIDTH       (704)   // user define : must 64 byte align
#define LSV_DES_HEIGHT      (16)    // user define
#define SCL_BLK_BIT             (11)
#define SCL_H_PXL_BIT           (8)
#define SCL_H_PXL_MASK          ((1<<SCL_H_PXL_BIT) - 1)
#define SCL_V_PXL_BIT           (SCL_BLK_BIT-SCL_H_PXL_BIT)
#define SCL_V_PXL_MASK          ((1<<SCL_V_PXL_BIT) - 1)

#define TDDC_WIDTH_LIMIT     (512+32)

#if defined(CC_MT5882)
#define SMALL_OUTWINDOW_TTD_NEWSETTING 
#endif


typedef enum{
    SRC_3D_TYPE_FS  = 0,
    SRC_3D_TYPE_SBS = 1,
    SRC_3D_TYPE_TAB = 2,
    SRC_3D_TYPE_LI  = 3,
}E_SRC_3D_TYPE;

typedef enum{
    TTD_CHECK_INV_V1 = 1,
    TTD_CHECK_INV_V2,
    TTD_CHECK_INV_V3,
    TTD_CHECK_INV_MAX
}E_TTD_CHECK_INV_VER;

typedef struct
{
    UINT16 ttdInputWidth;
    UINT16 ttdInputHeight;
    UINT8 ttdFwStatus;
    UINT8 tddcFwStatus;
    UINT8 ttdConvergenceDepth;
    UINT8 ttdGlobalWeight;
    UINT8 ttdLocalWeight;
    UINT8 ttdQtyUpdate;
    UINT8 ttdFgLocalInverse;
    UINT32 ttdPreAPL;
    UINT32 ttdCurAPL;
    UINT32 ttdPreDiver;
    UINT32 ttdCurDiver;
    UINT32 ttdSceneChangeThr;
    UINT16 ttdHist[32];
    UINT16 ttdHueHist[8];
    INT32 max_index, hist_max, max_index_sec, hist_max_sec;
}TTD_INFO;

// LSV definition
typedef unsigned char PEL;

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
EXTERN UINT8 g_u1SupportDepthCtrl;
EXTERN UINT8 g_u1Input3D;
EXTERN UINT8 g_u1DepthOfField;
EXTERN UINT8 g_u1AutoConvEnable;
EXTERN UINT8 g_u1AutoConvBlkBarUpdate;
EXTERN UINT8 g_u1AutoConvBlkBarPxl;
EXTERN UINT8 g_u1ImageSafetyLevel;
EXTERN UINT8 g_u1DistanceToTV;
EXTERN INT32 g_i4dLimMax;
EXTERN INT32 g_i4dLimMin;
EXTERN UINT32 g_u4ProtrudeFromScreen;
EXTERN UINT32 g_u4AutoConvOutPxl;
EXTERN TTD_INFO gTTDInfo;

EXTERN INT32 cur_convg;
EXTERN UINT8 u1Lsvinit;
EXTERN UINT32 u4CalLoop;
EXTERN INT32 iSclRatio;
EXTERN INT32 m_depth_width, m_depth_height;

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
void vDrvTTDInit(void);
void vDrvTTDSetDepthControl(UINT8 u1OnOff);
void vDrvTTDSet3DInput(void);
void vDrvTTDSet2DInput(void);
void vDrvTTDSet3DDepthCtrl(UINT8 u1OnOff);
void vDrvTTDSetEnable(UINT8 bEnable);
void vDrvTTDSetBlackSingleInv(UINT8 u1OnOff);
void vDrvTTDSetQualityISR(void);
void vDrvTTDProc(void);
void vDrvTTDSetFWOnOff(UINT8 bOnOff);
void vDrvTDDCSetFWOnOff(UINT8 bOnOff);
void vDrvTTDCheckRstCtrl(void);
void vDrvTTDSetStereoComfortRange(UINT8 u1Factor);
void vDrvTTDSetImageSafetyLevel(UINT8 u1Level);
void vDrvTTDSetDepthOfField(UINT8 u1Depth);
void vDrvTTDSetProtrudeFromScreen(UINT8 u1Protrude);
void vDrvTTDSetDistanceToTV(UINT8 u1Distance);
void vDrvTTDSetLREyeInverse(UINT8 u1OnOff);
void vDrvTTDSetShutterLREyeInverse(UINT8 u1OnOff);
void vDrvTTDCalculateSafetyScale(void);
void vDrvTTDUiValueUpdate(void);
void vDrvTTDSetUiProc(void);
void vDrvTTDSetLRSyncSwitch(UINT8 bEnable);
void vDrvTTDSetLRDataSwitch(UINT8 bEnable);
void vDrvTTDSetGlobalShift(UINT32 u4Shift);
void vDrvTTDSetV3GlobalShift(INT8 i1Shift);
void vDrvTTDSetNewGlobalShift(UINT8 u1Shift);
void vDrvTTDSetPrInfo(UINT8 u1OnOff);
void vDrvTTDSetDepthBias(UINT8 u1Value);
void vDrvTTDQualityInit(void);
void vDrvTTDSetBlackBar(UINT32 u4BlackFromTTD, UINT32 u4BlackFromGbSft, UINT32 u4BlackSingleInv);
void vDrvTTDSetBlackEn(UINT8 u1OnOff);
void vDrvTTDSetBlackNum(UINT32 u4Value);
void vDrvTTDSetBlackSingleEn(UINT8 u1OnOff);
void vDrvTTDSetBlackSingleNum(UINT8 u1Value);
void vDrvTTDLSVInit(void);
void vDrvTTDLSVProc(void);
void vDrvTTDSet3DDepthCtrl(UINT8 u1OnOff);
void vDrvTTDSuspend(void);
void vDrvTTDResume(void);
void vDrvTTDSetDisptScalar(UINT32 u4Scalar);
void vDrvTTDSetGlobalWeight(UINT8 u1Value);
void vDrvTTDSetLocalWeight(UINT8 u1Value);
void vDrvTTDSetTfuWeight(UINT8 u1Value);
void vDrvTTDSetTfuBase(UINT32 u4Value);
void vDrvTTDSetRgnHistogram(UINT16 u2Width,UINT16 u2Height);
void vDrvTTDSetBlackBarMainLoop(void);
void vDrvTTDSetGlobalModel(void);
void vDrvTTDCheckQtyUpdate(void);
void vDrvTTDSetPQWindow(UINT16 u2Width,UINT16 u2Height);
void vDrvTTDQualityProc(void);
void vDrvTTDCheckLocalInverseMode(void);
void vDrvTTDSetFullScreenMode(UINT32 u4OnOff);
void vDrvTTDAdaptiveWeight(void);
void vDrvTTDCalImgChar(void);
UINT8 u1DrvTTDGet3DDepthCtrl(void);
UINT32 u4DrvTTDGetPrInfo(void);
UINT32 u4DrvTTDIsSceneChanged(void);
UINT32 u4DrvTTDCheckCIDInverseV1(void);
UINT32 u4DrvTTDCheckCIDInverseV2(void);
UINT32 u4DrvTTDCheckCIDInverseV3(void);
UINT32 u4DrvTTDGetFullScreenMode(void);
UINT32 u4DrvGetArrayMaxValue(UINT32 u4Array[], UINT32 u4Size);

void vDrvTddcInit(UINT32 img_width, UINT32 img_height);
void vDrvTddcFwTrigger(void);
void vDrvTddcProc(void);
void vDrvTddcSetPicSize(UINT16 u2Width, UINT16 u2Height);
void vDrvTTDLumaSum(UINT32 size, PEL *SRYL, PEL *SRYR);
void vDrvTTDLSVCalculate(PEL *SRYL, PEL *SRYR);
void vDrvLSVModeChange(INT32 iWidth, INT32 iHeight, INT32 iStep_h);

extern void HalFlushInvalidateDCache(void);
extern void vDrvTddcSetCtrl(UINT8 u1OnOff);

#ifdef CC_SUPPORT_STR
void vDrvTddcResume(void);
#endif

 
#endif //#ifndef _DRV_TTD_H_
