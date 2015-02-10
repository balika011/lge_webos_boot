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
 * $Date: 2015/02/10 $
 * $RCSfile: panel.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file panel.h
 *  This file includes panel definitions.
 */

#ifndef PANEL_H
#define PANEL_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END

#include "feature.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// this file was place on each customer's directory
#include "panel_cust.h"

#define PANEL_DEFAULT 0
#define PANEL_INVALID 0xff

#ifndef PANEL_SELECT    /* NO CHANGE */
#define PANEL_SELECT PANEL_DEFAULT /* DO NOT CHANGE THIS SETTING!!! */
#endif /* PANEL_SELECT */


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define LVDS_A_EVEN 0
#define LVDS_A_ODD 1
#define LVDS_B_EVEN 2
#define LVDS_B_ODD 3

// display mode
#define DISP_MODE_OFF 0
#define DISP_MODE_ON 1
    #define DISP_MODE_USE DISP_MODE_OFF

// no interlace mode in 537x
#define PROGRESSIVE_MODE 0
#define INTERLACE_MODE 1
    #define INTERLACE_ENABLE PROGRESSIVE_MODE

// scpos
#define SRGB_EN_OFF 0
#define SRGB_EN_ON 1
    #define SRGB_FUNCTION_EN SRGB_EN_OFF
#define DEN_NO_ACIVE 0
#define DEN_ACIVE 1
    #define DEN_TYPE DEN_NO_ACIVE
#define DEN_HIGH 0
#define DEN_LOW 1
    #define DEN_POLARITY DEN_HIGH
#define FIELD_HIGH 0
#define FIELD_LOW 1
    #define FIELD_POLARITY FIELD_HIGH
#define HSYNC_HIGH 0
#define HSYNC_LOW 1
#ifdef __MODEL_slt__
    #define HSYNC_POLARITY HSYNC_HIGH
#else
    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    #define HSYNC_POLARITY PANEL_GetHSyncPolarity()
    #else    
    #define HSYNC_POLARITY HSYNC_LOW
    #endif
#endif
#define VSYNC_HIGH 0
#define VSYNC_LOW 1
#ifdef __MODEL_slt__
    #define VSYNC_POLARITY VSYNC_HIGH
#else
    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    #define VSYNC_POLARITY PANEL_GetVSyncPolarity()
    #else    
    #define VSYNC_POLARITY VSYNC_LOW
    #endif
#endif
#define RB_CHANNEL_INVERSE_OFF 0
#define RB_CHANNEL_INVERSE_ON 1
    #define RB_CHANNEL_INVERSE RB_CHANNEL_INVERSE_OFF
#define BIT_ORDER_INVERSE_OFF 0
#define BIT_ORDER_INVERSE_ON 1
    #define BIT_ORDER_INVERSE BIT_ORDER_INVERSE_OFF

// dump flag
#define PANEL_DUMP_CURRENT 0
#define PANEL_DUMP_ALL 1
#define PANEL_DUMP_TITLE 2

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#ifdef CC_ENABLE_MTK_MODEL_INDEX
extern void WriteModelIndex(UINT32 u4ModelIndex);
extern UINT32 GetModelIndexFromEEPROM(void);
#endif
extern void LoadPanelIndex(void);
extern void WritePanelIndexToEeprom(UINT32 u4PanelIndex);
extern void SelectPanel(UINT32 u4Index);
extern CHAR* GetCurrentPanelName(void);
extern UINT32 GetCurrentPanelIndex(void);
extern CHAR* GetPanelName(UINT32 u4Index);
extern UINT32 GetFlagPanelIdInEeprom(void);
extern void DumpPanelAttribute(UINT32 u4Flag);
extern UINT32 IsLvdsMsbSwOn(void);
extern UINT8 fgIsSupport120HzPanel(void);
extern UINT32 IsPwmLowPanelBright(void);
extern UINT32 PANEL_GetBacklightHigh(void);
extern UINT32 PANEL_GetBacklightMiddle(void);
extern UINT32 PANEL_GetBacklightLow(void);
extern UINT32 PANEL_TranslateEepromToFlashIndex(UINT32 u4EepromIndex);
extern UINT32 PANEL_GetEepromPanelIndex(void);
extern void PANEL_SetPixelShift(BOOL fgEnable, INT32 i4HPixelShift, INT32 i4VPixelShift);
extern BOOL PANEL_IsPixelShift(void);
extern INT32 PANEL_GetPixelShift_H(void);
extern INT32 PANEL_GetPixelShift_V(void);

extern UINT32 PANEL_GetPanelPhysicalWidth(void);
extern UINT32 PANEL_GetPanelPhysicalHeight(void);

extern UINT32 PANEL_GetPanelWidth(void);
extern UINT32 PANEL_GetPanelHeight(void);
extern UINT32 PANEL_GetPixelClkMax(void);
extern UINT32 PANEL_GetPixelClk60Hz(void);
extern UINT32 PANEL_GetPixelClk50Hz(void);
extern UINT32 PANEL_GetPixelClkMin(void);
extern UINT32 PANEL_GetHTotalMax(void);
extern UINT32 PANEL_GetHTotal60Hz(void);
extern UINT32 PANEL_GetHTotal50Hz(void);
extern UINT32 PANEL_GetHTotalMin(void);
extern UINT32 PANEL_GetHSyncLen60Hz(void);
extern UINT32 PANEL_GetHSyncLen50Hz(void);

extern UINT32 PANEL_GetPixelClk48Hz(void);
extern UINT32 PANEL_GetHTotal48Hz(void);
extern UINT32 PANEL_GetVTotal48Hz(void);
extern UINT32 PANEL_GetHSyncLen48Hz(void);
extern UINT32 PANEL_GetHPosition48Hz(void);
extern UINT32 PANEL_GetDimmingFrequency48Hz(void);

extern UINT32 PANEL_GetVTotalMax(void);
extern UINT32 PANEL_GetVTotal60Hz(void);
extern UINT32 PANEL_GetVTotal50Hz(void);
extern UINT32 PANEL_GetVTotalMin(void);
extern UINT32 PANEL_GetVClkMax(void);
extern UINT32 PANEL_GetVClkMin(void);

extern UINT32 PANEL_GetHSyncWidth(void);
extern UINT32 PANEL_GetVSyncWidth(void);
extern UINT32 PANEL_GetHPosition60Hz(void);
extern UINT32 PANEL_GetHPosition50Hz(void);
extern UINT32 PANEL_GetVPosition(void);
extern UINT32 PANEL_GetControlWord(void);
extern UINT32 PANEL_GetControlWord2(void);
extern UINT32 PANEL_GetBacklightHighHwPwm(void);
extern UINT32 PANEL_GetBacklightMiddleHwPwm(void);
extern UINT32 PANEL_GetBacklightLowHwPwm(void);
extern UINT32 PANEL_GetDimmingFrequency60Hz(void);
extern UINT32 PANEL_GetDimmingFrequency50Hz(void);
extern UINT32 PANEL_GetLvdsOnDalay(void);
extern UINT32 PANEL_GetBacklightOnDelay(void);
extern UINT32 PANEL_GetBacklightOffDelay(void);
extern UINT32 PANEL_GetLvdsOffDalay(void);
extern UINT32 PANEL_GetMaxOverscan(void);
extern UINT32 PANEL_GetPixelShiftHMax(void);
extern UINT32 PANEL_GetPixelShiftVMax(void);

extern void PANEL_SetPanelWidth(UINT32 u4Value);
extern void PANEL_SetPanelHeight(UINT32 u4Value);
extern void PANEL_SetPixelClkMax(UINT32 u4Value);
extern void PANEL_SetPixelClk60Hz(UINT32 u4Value);
extern void PANEL_SetPixelClk50Hz(UINT32 u4Value);
extern void PANEL_SetPixelClk48Hz(UINT32 u4Value);
extern void PANEL_SetPixelClkMin(UINT32 u4Value);
extern void PANEL_SetHTotalMax(UINT32 u4Value);
extern void PANEL_SetHTotal60Hz(UINT32 u4Value);
extern void PANEL_SetHTotal50Hz(UINT32 u4Value);
extern void PANEL_SetHTotal48Hz(UINT32 u4Value);
extern void PANEL_SetHTotalMin(UINT32 u4Value);
extern void PANEL_SetVTotalMax(UINT32 u4Value);
extern void PANEL_SetVTotal60Hz(UINT32 u4Value);
extern void PANEL_SetVTotal50Hz(UINT32 u4Value);
extern void PANEL_SetVTotal48Hz(UINT32 u4Value);
extern void PANEL_SetVTotalMin(UINT32 u4Value);
extern void PANEL_SetVClkMax(UINT32 u4Value);
extern void PANEL_SetVClkMin(UINT32 u4Value);
extern void PANEL_SetHSyncWidth(UINT32 u4Value);
extern void PANEL_SetVSyncWidth(UINT32 u4Value);
extern void PANEL_SetHPosition60Hz(UINT32 u4Value);
extern void PANEL_SetVPosition(UINT32 u4Value);

#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
extern INT32 PANEL_GetHPositionByTiming(UINT8 bDclkType);
extern INT32 PANEL_GetVPositionByTiming(UINT8 bDclkType);
extern UINT32 PANEL_GetHSyncWidthByTiming(UINT8 bDclkType);
extern UINT32 PANEL_GetVSyncWidthByTiming(UINT8 bDclkType);
extern void PANEL_SetVSyncWidthByTiming(UINT8 bDclkType, UINT16 u2VSyncWd);
extern void PANEL_SetHSyncWidthByTiming(UINT8 bDclkType, UINT16 u2HSyncWd);
extern UINT32 PANEL_GetVSyncBpByTiming(UINT8 bDclkType);
extern void PANEL_SetVSyncBpByTiming(UINT8 bDclkType, UINT16 u2VSyncFP);
extern void PANEL_SetHSyncFpByTiming(UINT8 bDclkType, UINT16 u2HSyncFP);
extern UINT32 PANEL_GetHSyncPolarity(void);
extern UINT32 PANEL_GetVSyncPolarity(void);
extern void PANEL_SetHSyncPolarity(UINT8 u1HsyncPol);
extern void PANEL_SetVSyncPolarity(UINT8 u1VsyncPol);
#endif //SUPPORT_PANEL_CUSTOMER_SPEC

#ifdef SUPPORT_PANEL_SCAN_PWM
extern UINT32 PANEL_GetScanPWMStart(void);
extern UINT32 PANEL_GetScanPWMDuty(void);
#endif

extern void PANEL_SetControlWord(UINT32 u4Value);
extern void PANEL_SetControlWord2(UINT32 u4Value);
extern void PANEL_SetBacklightHighHwPwm(UINT32 u4Value);
extern void PANEL_SetBacklightMiddleHwPwm(UINT32 u4Value);
extern void PANEL_SetBacklightLowHwPwm(UINT32 u4Value);
extern void PANEL_SetDimmingFrequency60Hz(UINT32 u4Value);
extern void PANEL_SetDimmingFrequency50Hz(UINT32 u4Value);
extern void PANEL_SetLvdsOnDalay(UINT32 u4Value);
extern void PANEL_SetBacklightOnDelay(UINT32 u4Value);
extern void PANEL_SetBacklightOffDelay(UINT32 u4Value);
extern void PANEL_SetLvdsOffDalay(UINT32 u4Value);
extern void PANEL_SetMaxOverscan(UINT32 u4Value);
extern INT32 DRVCUST_PanelQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data);

CHAR * PANEL_GetVersion(void);
UINT32 PANEL_GetSpreadSpectrum(UINT32 *pu4Freq, UINT32 *pu4Permillage);
UINT32 PANEL_GetDither(void);
UINT32 PANEL_GetDrivingCurrent(void);
UINT16 PANEL_Get3DPixelSize(void);
UINT32 Panel_GetAQIndex(UINT32 u4Idx);
UINT32 Panel_GetPQIndex(UINT32 u4Idx);

#if defined(SUPPORT_PANEL_CUSTOMER_SPEC) || defined(SUPPORT_PANEL_SS) || defined(SUPPORT_PANEL_DITHER) || defined(SUPPORT_PANEL_SCAN_PWM)
extern UINT32 PANEL_IsSupportExtAttribute(void);
#endif

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

// replace these legacy global variable with function call

#define TV_WIDTH PANEL_GetPanelWidth()
#define TV_HEIGHT PANEL_GetPanelHeight()
#define wDISPLAY_WIDTH PANEL_GetPanelWidth()
#define wDISPLAY_HEIGHT PANEL_GetPanelHeight()

#define PIXEL_SHIFT_H_MAX 0
#define PIXEL_SHIFT_V_MAX 0

#define HLEN_TOTAL_TYPI PANEL_GetHTotal60Hz()
#define VLEN_TOTAL_TYPI PANEL_GetVTotal60Hz()
#define HSYNCLEN_TYPI PANEL_GetHSyncLen60Hz()
#define LEFT_MASK_TYPI 0
#define TOP_MASK_TYPI 0
#define RIGHT_MASK_TYPI (UINT16)(((HLEN_TOTAL_TYPI-HSYNCLEN_TYPI)-PANEL_GetPanelWidth())-LEFT_MASK_TYPI)
#define BOTTOM_MASK_TYPI (UINT16)((VLEN_TOTAL_TYPI-PANEL_GetPanelHeight())-TOP_MASK_TYPI)

#define HLEN_TOTAL_50HZ PANEL_GetHTotal50Hz()
#define VLEN_TOTAL_50HZ PANEL_GetVTotal50Hz()
#define HSYNCLEN_50HZ PANEL_GetHSyncLen50Hz()
#define LEFT_MASK_50HZ 0
#define TOP_MASK_50HZ 0
#define RIGHT_MASK_50HZ (UINT16)(((HLEN_TOTAL_50HZ-HSYNCLEN_50HZ)-PANEL_GetPanelWidth())-LEFT_MASK_50HZ)
#define BOTTOM_MASK_50HZ (UINT16)((VLEN_TOTAL_50HZ-PANEL_GetPanelHeight())-TOP_MASK_50HZ)

#define HLEN_TOTAL_48HZ PANEL_GetHTotal48Hz()
#define VLEN_TOTAL_48HZ PANEL_GetVTotal48Hz()
#define HSYNCLEN_48HZ PANEL_GetHSyncLen48Hz()
#define LEFT_MASK_48HZ 0
#define TOP_MASK_48HZ 0
#define RIGHT_MASK_48HZ (UINT16)(((HLEN_TOTAL_48HZ-HSYNCLEN_48HZ)-PANEL_GetPanelWidth())-LEFT_MASK_48HZ)
#define BOTTOM_MASK_48HZ (UINT16)((VLEN_TOTAL_48HZ-PANEL_GetPanelHeight())-TOP_MASK_48HZ)

#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
#define H_POS_60HZ PANEL_GetHPositionByTiming(SV_DCLK_60HZ)
#define H_POS_50HZ PANEL_GetHPositionByTiming(SV_DCLK_50HZ)
#define H_POS_48HZ PANEL_GetHPositionByTiming(SV_DCLK_48HZ)
#else
#define H_POS_60HZ PANEL_GetHPosition60Hz()
#define H_POS_50HZ PANEL_GetHPosition50Hz()
#define H_POS_48HZ PANEL_GetHPosition48Hz()
#endif
#define HS_WIDTH PANEL_GetHSyncWidth()
#define VS_WIDTH PANEL_GetVSyncWidth()
#define V_POS PANEL_GetVPosition()

#ifdef SUPPORT_PANEL_SCAN_PWM
#define DEFAULT_SCAN_PWM_START 0   // permillage
#define DEFAULT_SCAN_PWM_DUTY 500  // permillage
#define SCANPWM_START PANEL_GetScanPWMStart()
#define SCANPWM_DUTY  PANEL_GetScanPWMDuty()
#endif

// panel limitation, for display mode use
#define DCLK_MIN PANEL_GetPixelClkMin()
#define DCLK_MAX PANEL_GetPixelClkMax()
#define DHS_TOTAL_MIN PANEL_GetHTotalMin()
#define DHS_TOTAL_MAX PANEL_GetHTotalMax()
#define DVS_TOTAL_MIN PANEL_GetVTotalMin()
#define DVS_TOTAL_MAX PANEL_GetVTotalMax()
#define DVS_FREQ_MIN PANEL_GetVClkMin()
#define DVS_FREQ_MAX PANEL_GetVClkMax()

#define PANEL_IsSupport240Hz() ((PANEL_GetVClkMax() > 200)? SV_TRUE: SV_FALSE)
#define PANEL_IsSupport120Hz() ((PANEL_GetVClkMax() > 100)? SV_TRUE: SV_FALSE)


// dither structure :
// 1. lsb on/off [1] (OSTG)
// 2. round on/off [2] (OSTG)
// 3. rdither on/off [3] (OSTG)
// 4. edither on/off[4] (OSTG)
// 5. subpixel on/off [5] (OSTG)
// 6. lfsr dither [6] (OSTG)
// 7. fphase [11:8] (OSTG)

// 1. lsb on/off [17] (TCON/OD)
// 2. round on/off [18] (TCON/OD)
// 3. rdither on/off [19] (TCON/OD)
// 4. edither on/off[20] (TCON/OD)
// 5. subpixel on/off [21] (TCON/OD)
// 6. lfsr dither [22] (TCON/OD)
// 7. fphase [27:24] (TCON/OD)

// Output Stage Dither
#define OSTG_LSB_OFF (1<<1)
#define OSTG_LSB_ON (0)
    #define OSTG_LSBOFF_FUNCTION    (1<<1)
    
#define OSTG_ROUND_OFF (0)
#define OSTG_ROUND_ON (1<<2)
    #define OSTG_ROUND_FUNCTION    (1<<2)

#define OSTG_RDITHER_OFF (0)
#define OSTG_RDITHER_ON (1<<3)
    #define OSTG_RDITHER_FUNCTION    (1<<3)

#define OSTG_ERROR_DITHER_OFF (0)
#define OSTG_ERROR_DITHER_ON (1<<4)
        #define OSTG_ERROR_DITHER_FUNCTION   (1<<4)

#define OSTG_SUB_PIXEL_OFF (0)
#define OSTG_SUB_PIXEL_ON (1<<5)
    #define OSTG_SUB_PIXEL_FUNCTION    (1<<5)

#define OSTG_LFSR_OFF (0)
#define OSTG_LFSR_ON (1<<6)
    #define OSTG_LFSR_FUNCTION    (1<<6)

#define OSTG_FPHASE_VALUE ((PANEL_GetDither() & 0x0f00) >> 8)

// OD / TCon Dither

#define TCON_LSB_OFF (1<<17)
#define TCON_LSB_ON (0)
    #define TCON_LSBOFF_FUNCTION    (1<<17)
    
#define TCON_ROUND_OFF (0)
#define TCON_ROUND_ON (1<<18)
    #define TCON_ROUND_FUNCTION    (1<<18)

#define TCON_RDITHER_OFF (0)
#define TCON_RDITHER_ON (1<<19)
    #define TCON_RDITHER_FUNCTION    (1<<19)

#define TCON_ERROR_DITHER_OFF (0)
#define TCON_ERROR_DITHER_ON (1<<20)
        #define TCON_ERROR_DITHER_FUNCTION   (1<<20)

#define TCON_SUB_PIXEL_OFF (0)
#define TCON_SUB_PIXEL_ON (1<<21)
    #define TCON_SUB_PIXEL_FUNCTION    (1<<21)

#define TCON_LFSR_OFF (0)
#define TCON_LFSR_ON (1<<22)
    #define TCON_LFSR_FUNCTION    (1<<22)

#define TCON_FPHASE_VALUE ((PANEL_GetDither() & 0x0f000000) >> 8)



// lvds
// msb switch
#define LVDS_MSB_SW_OFF 0 // {MSB 7,6,5,4,3,2,1,0 LSB}
#define LVDS_MSB_SW_ON (1<<0) // {MSB 5,4,3,2,1,0,7,6 LSB}
#define LVDS_SPECIAL_NS (1<<1) 
#define LVDS_JEIDA LVDS_MSB_SW_OFF
#define LVDS_NS LVDS_MSB_SW_ON
    #define LVDS_DISP_FORMAT IsLvdsMsbSwOn()

// odd/even pixel stitch
#define LVDS_ODD_SW_OFF 0 // even in A0~A3, odd in A4~A7
#define LVDS_ODD_SW_ON (1 << 2)
    #define LVDS_DISP_ODD_SW (PANEL_GetControlWord() & LVDS_ODD_SW_ON)

// lvds port
#define SINGLE_PORT 0
#define DUAL_PORT (1 << 3)
#define FOUR_PORT (2 << 3)
#define OCTA_PORT (3 << 3)
    #define LVDS_OUTPUT_PORT (PANEL_GetControlWord() & (DUAL_PORT|FOUR_PORT|OCTA_PORT))

// display bit
#define DISP_18BIT 0 // 18-bit RGB output
#define DISP_24BIT (1 << 5) // 24-bit RGB output
#define DISP_30BIT (2 << 5) // 30-bit RGB output
#define DISP_36BIT (3 << 5) // 36-bit RGB output
#define DISP_BIT_MSK (3 << 5)
    #define DISP_BIT (PANEL_GetControlWord() & DISP_BIT_MSK)

// PWM_HIGH_PANEL_BRIGHT : panel will be brighter when duty cycle is higher
#define PWM_HIGH_PANEL_BRIGHT 0
#define PWM_LOW_PANEL_BRIGHT (1 << 7)
    #define PANEL_BRIGHT_SETTING IsPwmLowPanelBright()

// reset panel when switch frame rate
#define WFB_PANEL_RESET_OFF 0
#define WFB_PANEL_RESET_ON (1 << 8)
    #define WFB_PANEL_RESET (PANEL_GetControlWord() & WFB_PANEL_RESET_ON)

//LVDS ABCD control mapping
#define LVDS_A_FROM_AE (0 << 10)
#define LVDS_A_FROM_AO (1 << 10)
#define LVDS_A_FROM_BE (2 << 10)
#define LVDS_A_FROM_BO (3 << 10)

#define LVDS_B_FROM_AE (0 << 12)
#define LVDS_B_FROM_AO (1 << 12)
#define LVDS_B_FROM_BE (2 << 12)
#define LVDS_B_FROM_BO (3 << 12)

#define LVDS_C_FROM_AE (0 << 14)
#define LVDS_C_FROM_AO (1 << 14)
#define LVDS_C_FROM_BE (2 << 14)
#define LVDS_C_FROM_BO (3 << 14)

#define LVDS_D_FROM_AE (0 << 16)
#define LVDS_D_FROM_AO (1 << 16)
#define LVDS_D_FROM_BE (2 << 16)
#define LVDS_D_FROM_BO (3 << 16)
    #define LVDS_USE_INDEPENDENT_SETTING (PANEL_GetControlWord() & 0x3Fc00)
    #define LVDS_A_INDEPENDENT_SETTING ((PANEL_GetControlWord() >> 10) & 0x3)
    #define LVDS_B_INDEPENDENT_SETTING ((PANEL_GetControlWord() >> 12) & 0x3)
    #define LVDS_C_INDEPENDENT_SETTING ((PANEL_GetControlWord() >> 14) & 0x3)
    #define LVDS_D_INDEPENDENT_SETTING ((PANEL_GetControlWord() >> 16) & 0x3)
	
// 3D support
#ifdef SUPPORT_DISP_50TO120
#define DISP_50TO120_2D             (1 << 0)
#define DISP_50TO120_3D             (1 << 1)
#define IS_SUPPORT_DISP_50TO120     (SUPPORT_DISP_50TO120 != 0)
#define IS_SUPPORT_DISP_50TO120_2D  (SUPPORT_DISP_50TO120 & DISP_50TO120_2D)
#define IS_SUPPORT_DISP_50TO120_3D  (SUPPORT_DISP_50TO120 & DISP_50TO120_3D)
#endif
#ifdef SUPPORT_DISP_60TO100
#define DISP_60TO100_2D             (1 << 0)
#define DISP_60TO100_3D             (1 << 1)
#define IS_SUPPORT_DISP_60TO100     (SUPPORT_DISP_60TO100 != 0)
#define IS_SUPPORT_DISP_60TO100_2D  (SUPPORT_DISP_60TO100 & DISP_60TO100_2D)
#define IS_SUPPORT_DISP_60TO100_3D  (SUPPORT_DISP_60TO100 & DISP_60TO100_3D)
#endif
#ifdef SUPPORT_DISP_WEB_VIDEO_SMOOTHER
#define IS_SUPPORT_DISP_6X3X_WEB 1
#endif

#define LVDS_DISP_3D_NA  (0)
#define LVDS_DISP_3D_POLARIZED_LRLR  (1<<18)
#define LVDS_DISP_3D_POLARIZED_RLRL  (2<<18)
#define LVDS_DISP_3D_SHUTTER  (3<<18)
#define LVDS_DISP_3D_MASK    (3<<18)
    #define LVDS_DISP_3D (PANEL_GetControlWord() & LVDS_DISP_3D_MASK)
#define IS_LVDS_DISP_3D_POLARIZED ((LVDS_DISP_3D==LVDS_DISP_3D_POLARIZED_LRLR) || (LVDS_DISP_3D==LVDS_DISP_3D_POLARIZED_RLRL))
#define IS_LVDS_DISP_3D_SHUTTER (LVDS_DISP_3D==LVDS_DISP_3D_SHUTTER)

// left/right picture split 
#define LVDS_DISP_SPLIT_OFF 0 
#define LVDS_DISP_SPLIT_ON (1 << 20)
    #define LVDS_DISP_SPLIT (PANEL_GetControlWord() & LVDS_DISP_SPLIT_ON)

#define DISP_TYPE_MASK (7<< 21)
#define DISP_TYPE_VB1 (1<< 21)
#define DISP_TYPE_MLVDS (2<< 21)
#define DISP_TYPE_EPI (3<< 21)
	#define IS_VBYONE	((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_VB1)
	#define IS_MLVDS	((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_MLVDS)
	#define IS_EPI		((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_EPI)

// High Speed support
#define LVDS_HIGHSPEED (1<< 24)
#define IS_LVDS_HIGHSPEED (PANEL_GetControlWord() & LVDS_HIGHSPEED)

// support L2R2 or L1R1
#define PANEL_L12R12 (1<< 26)
#define IS_PANEL_L12R12 (PANEL_GetControlWord() & PANEL_L12R12)

// support Aspect Ratio 21:9
#define PANEL_ASPECT_RATIO_21_9 (1<< 27)
#define IS_PANEL_ASPECT_RATIO_21_9 (PANEL_GetControlWord() & PANEL_ASPECT_RATIO_21_9)

#define DISP_3D_LBRB    (1 << 28)
#define IS_DISP_3D_LBRB (PANEL_GetControlWord() & DISP_3D_LBRB)

//Support OD
#define OD_ON (1<<29)
#define IS_OD_ENABLE (PANEL_GetControlWord() & OD_ON)

#define FB_ON (1<<30)
#define IS_FB_ENABLE (PANEL_GetControlWord() & FB_ON)

//Support LBRB_3D_ON
#define LBRB_3D_ON (1U <<31)
#define IS_LBRB_3D_ON (PANEL_GetControlWord() & LBRB_3D_ON)



// ========= Panel Control Word 2 ==========

// support DISP_R Encode on Link4 
#define DISPR_ENCODEL4 (1<< 0)
#define IS_DISPR_ENCODEL4 (PANEL_GetControlWord2() & DISPR_ENCODEL4)

// disable Hsync and Vsync output (Output low)
#define DISABLE_PANEL_HVSYNC (1<< 1)
#define IS_DISABLE_PANEL_HVSYNC (PANEL_GetControlWord2() & DISABLE_PANEL_HVSYNC)

#define SUPPORT_VB1_GRAPH_MODE (1<< 2)
#define IS_SUPPORT_VB1_GRAPH_MODE (PANEL_GetControlWord2() & SUPPORT_VB1_GRAPH_MODE)

#define FORCE_LVDS_ANA_2CH_ON (1<< 3)
#define IS_FORCE_LVDS_ANA_2CH_ON (PANEL_GetControlWord2() & FORCE_LVDS_ANA_2CH_ON)

#define FORCE_LVDS_ANA_4CH_ON (1<< 4)
#define IS_FORCE_LVDS_ANA_4CH_ON (PANEL_GetControlWord2() & FORCE_LVDS_ANA_4CH_ON)

#define SEPARATE_OSD_AND_DATA (1<< 5)
#define IS_SEPARATE_OSD_AND_DATA (PANEL_GetControlWord2() & SEPARATE_OSD_AND_DATA)

#define DISPLAY_FREERUN (1<< 6)
#define IS_DISPLAY_FREERUN (PANEL_GetControlWord2() & DISPLAY_FREERUN)

#define FORCE_LVDS_DIG_4CH_ON (1<< 7)
#define IS_FORCE_LVDS_DIG_4CH_ON (PANEL_GetControlWord2() & FORCE_LVDS_DIG_4CH_ON)

// 2D: Normal display 3D: L12R12 display
#define PANEL_2D_N_3D_L12R12 (1<< 8)
#define IS_PANEL_2D_N_3D_L12R12 (PANEL_GetControlWord2() & PANEL_2D_N_3D_L12R12)

#define DISABLE_LVDS_LINEBUFFER (1<< 9)
#define IS_DISABLE_LVDS_LINEBUFFER (PANEL_GetControlWord2() & DISABLE_LVDS_LINEBUFFER)

#define SUPPORT_4K2K_TYPE_MASK (3 << 10)
	#define DISP_4K2K_TYPE_1 (1<< 10)  // FHD120/FHD60/4K2K30
	#define DISP_4K2K_TYPE_2 (2<< 10)  // FHD60/4K2K30	
	#define DISP_4K2K_TYPE_3 (3<< 10)  // reserved		
		#define IS_SUPPORT_4K2K_DISPLAY (PANEL_GetControlWord2() & SUPPORT_4K2K_TYPE_MASK)
		#define IS_DISP_4K2K_TYPE1 ((PANEL_GetControlWord2() & SUPPORT_4K2K_TYPE_MASK) == DISP_4K2K_TYPE_1)
		#define IS_DISP_4K2K_TYPE2 ((PANEL_GetControlWord2() & SUPPORT_4K2K_TYPE_MASK) == DISP_4K2K_TYPE_2)
		#define IS_DISP_4K2K_TYPE3 ((PANEL_GetControlWord2() & SUPPORT_4K2K_TYPE_MASK) == DISP_4K2K_TYPE_3)		

#define SUPPORT_OSD_INDEX (1<< 12)
#define IS_SUPPORT_OSD_INDEX (PANEL_GetControlWord2() & SUPPORT_OSD_INDEX)

#define EPI_LANE_MASK (3<< 13)
 #define EPI_LANE_6  (0 << 13) 
 #define EPI_LANE_4  (1 << 13)  
 #define EPI_LANE_8  (2 << 13)  
	#define IS_EPI_4LANE ((PANEL_GetControlWord2() & EPI_LANE_MASK) == EPI_LANE_4)
	#define IS_EPI_8LANE ((PANEL_GetControlWord2() & EPI_LANE_MASK) == EPI_LANE_8)	

#define STEP_CLOCK_CHG (1<< 15)
#define IS_STEP_CLOCK_CHG (PANEL_GetControlWord2() & STEP_CLOCK_CHG)

#define DISPR_INVERSE (1<< 16)
#define IS_DISPR_INVERSE (PANEL_GetControlWord2() & DISPR_INVERSE)

#define VB1_ALL_LANE_ON (1<< 17)
#define IS_VB1_ALL_LANE_ON (PANEL_GetControlWord2() & VB1_ALL_LANE_ON)

// =========Graph Mode Control Word ==========
#define GM_FORMAT_CASE0_SMODE1 (0)
#define GM_FORMAT_CASE1_SMODE0 (1 << 0)
#define GM_FORMAT_CASE2_MMODE0 (2 << 0)
#define GM_FORMAT_CASE3_INHOUSE (3 << 0)
	#define GraphMode_FORMAT (GraphMode_GetControlWord() & (GM_FORMAT_CASE1_SMODE0|GM_FORMAT_CASE2_MMODE0|GM_FORMAT_CASE3_INHOUSE))

#define GM_LANE_MASK (3<< 2)
 #define GM_LANE_1  (0 << 2) 
 #define GM_LANE_2  (1 << 2)  
 #define GM_LANE_4  (2 << 2)  
	#define GM_LANE_NUM (GraphMode_GetControlWord() & (GM_LANE_2|GM_LANE_4))


typedef struct
{
    UINT8 u4PanelIndex ;   

	//0:    SV_DCLK_48HZ, 1: SV_DCLK_50HZ, 2:SV_DCLK_60HZ
    UINT32 au4PixelClk[3];
    UINT16 au2HTotal[3];
    UINT16 u2GraphModeWidth;	
    UINT16 au2HFrontPorch[3];
    UINT8 au1HSyncWidth[3];
    UINT16 au2HBackPorch[3];

    UINT16 au2VTotal[3];
    UINT16 u2GraphModeHight;
    UINT16 au2VFrontPorch[3];	
    UINT8 au1VSyncWidth[3];	
    UINT16 au2VBackPorch[3];
    
    UINT32 u4GControlWord;

    UINT8 u1FrameRateMax;
    UINT8 u1FrameRateMin;	
} GRAPHMODE_ATTRIBUTE_T;	

#if defined(SUPPORT_PANEL_GRAPH_MODE)
extern UINT32 GraphMode_GetControlWord(void);
extern void GraphMode_SetControlWord(UINT32 u4ControlWord);
extern void DumpGraphModeAttribute(void);
extern UINT32 GRAPHMODE_GetPixelClkByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetPixelClkByTiming(UINT8 bDclkType, UINT32 u4PixelClk);
extern void GRAPHMODE_SetPixelClk(UINT32 u4PixelClk48Hz, UINT16 u4PixelClk50Hz, UINT16 u4PixelClk60Hz);
extern UINT16 GRAPHMODE_GetHtotalByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetHtotalByTiming(UINT8 bDclkType, UINT16 u2Htotal);
extern void GRAPHMODE_SetHtotal(UINT16 u2Htotal48Hz, UINT16 u2Htotal50Hz, UINT16 u2Htotal60Hz);
extern UINT32 GRAPHMODE_GetPanelWidth(void);
extern void GRAPHMODE_SetPanelWidth(UINT16 u2PanelWidth);
extern UINT16 GRAPHMODE_GetHSyncFpByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetHSyncFpByTiming(UINT8 bDclkType, UINT16 u2HFp);
extern void GRAPHMODE_SetHSyncFp(UINT16 u2HFp48Hz, UINT16 u2HFp50Hz, UINT16 u2HFp60Hz);
extern UINT8 GRAPHMODE_GetHSyncWidthByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetHSyncWidthByTiming(UINT8 bDclkType, UINT16 u2HWidth);
extern void GRAPHMODE_SetHSyncWidth(UINT16 u2HWidth48Hz, UINT16 u2HWidth50Hz, UINT16 u2HWidth60Hz);
extern UINT16 GRAPHMODE_GetHSyncBpByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetHSyncBpByTiming(UINT8 bDclkType, UINT16 u2HBp);
extern void GRAPHMODE_SetHSyncBp(UINT16 u2HBp48Hz, UINT16 u2HBp50Hz, UINT16 u2HBp60Hz);
extern UINT16 GRAPHMODE_GetVtotalByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetVtotalByTiming(UINT8 bDclkType, UINT16 u2Vtotal);
extern void GRAPHMODE_SetVtotal(UINT16 u2Vtotal48Hz, UINT16 u2Vtotal50Hz, UINT16 u2Vtotal60Hz);
extern UINT32 GRAPHMODE_GetPanelHeight(void);
extern void GRAPHMODE_SetPanelHeight(UINT16 u2PanelHeight);
extern UINT16 GRAPHMODE_GetVSyncFpByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetVSyncFpByTiming(UINT8 bDclkType, UINT16 u2VFp);
extern void GRAPHMODE_SetVSyncFp(UINT16 u2VFp48Hz, UINT16 u2VFp50Hz, UINT16 u2VFp60Hz);
extern UINT8 GRAPHMODE_GetVSyncWidthByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetVSyncWidthByTiming(UINT8 bDclkType, UINT16 u2VWidth);
extern void GRAPHMODE_SetVSyncWidth(UINT8 u1VWidth48Hz, UINT8 u1VWidth50Hz, UINT8 u1VWidth60Hz);
extern UINT16 GRAPHMODE_GetVSyncBpByTiming(UINT8 bDclkType);
extern void GRAPHMODE_SetVSyncBpByTiming(UINT8 bDclkType, UINT16 u2VBp);
extern void GRAPHMODE_SetVSyncBp(UINT16 u2VBp48Hz, UINT16 u2VBp50Hz, UINT16 u2VBp60Hz);
#endif

// ========= Local Dimming ==========
//SPI enhancement , mtk70763 , 2010-6-15

//2A0[0] CPHA , clock phase
#define CPHA_OFF      0
#define CPHA_ON       1

//2A0[0] CPOL, clock polarity
#define CPOL_OFF      0
#define CPOL_ON       1

//2AC[1] MDATA_SINGLE_EN , horizontal 
#define HDATA_SINGLE_DISABLE (0)
#define HDATA_SINGLE_EN      (1)

//2AC[5] NDATA_SINGLE_EN , vertical
#define VDATA_SINGLE_DISABLE (0)
#define VDATA_SINGLE_EN      (1)

//2AC[15:12]=2 Tclg , not set by costomer
//2AC[31:28]=2 Tcld , not set by costomer
//#define LDM_TCLG             (2)
//#define LDM_TCLD             (2)

// 2B4[0] LSB Mode
#define LD_FMT_ORDER_MSB (0)
#define LD_FMT_ORDER_LSB (1)
    #define LD_FMT_ORDER_MASK (1)

// 2B4[1] RGB Mode
#define LD_FMT_WHITE   0
#define LD_FMT_RGB    (1)

// 2B4[4:5] Bit mode
#define LD_FMT_DEPTH_8BIT        0
#define LD_FMT_DEPTH_10BIT      (1)
#define LD_FMT_DEPTH_12BIT      (2)
#define LD_FMT_DEPTH_7BIT       (3)

// 2B4[10:8] SEQ_FMT , SW mode set to 0
#define LD_FMT_SEQ_NONE   0

// 2B4[13:12] LED type
#define LD_FMT_EDGE_LIT_DIRECT         0
#define LD_FMT_EDGE_LIT_TYPE1         (1)
#define LD_FMT_EDGE_LIT_TYPE2         (2)
#define LD_FMT_EDGE_LIT_SPLIT_TYPE    (3)

//2B8[0]=0 SW trigger bit , initial set to 0
#define LDM_SW_TIRG_NO      (0)

//2B8[1]=1 enable trigger by SW instead of Vsync input
#define LDM_SW_TIRG_EN_FLAG   (1)

//2B8[2]=0 select SRAM SW mode 
#define LDM_SRAM_SW_MODE    (0)

//2B8[3]  SRAM fold mode ,refer to u1BitMode
#define LDM_SRAM_DEPTH_8BIT  (0)  //u1BitMode =0&3 
#define LDM_SRAM_DEPTH_16BIT (1)  //u1BitMode=1&2

//2B8[9]=1 SW set data 
#define LDM_SW_SET_DATA_FLAG  (1)

typedef struct
{
    UINT32 u4PanelIndex ;
	UINT8 u8Inch;
	UINT8 u8Led_type;        
    UINT8  u1ProtocolIndex;     // include special format (LGD / Sharp) settomg
    UINT8  u1SWModeEn;    
    UINT8  u1ScanningOnoff;
	UINT8  u1ScanningTblIdx;
	
    // clock configuration
    UINT32 u4OutputClock;      // SPI output clock , unit is Hz
    UINT8  u1CPOL;             // clock polarity 
    UINT8  u1CPHA;             // clock phase

    // frame rate & Vsync latency calculate from panel_table
    // frame_rate_mode , when output frenquency >=input frenquency is multiply mode 

    // Vsync access timing control
    UINT16 u1VSyncHighTime;      // Vsync high time ,unit is us
    UINT16 u1WaitTime;           // wait time from Vsync low to CS low or data , unit is us

    // CS access timing control
    UINT16 u2CSHighTime;         // output CS hold time, unit is ns
    UINT8  u1Tcld;               // setup time from CS low to SCLK edge , unit is ns
    UINT8  u1Tclg;               // hold time from SCLK edge to CS high, unit is ns    
    UINT8  u1HheadSingleEn;      // CS setting
    UINT8  u1HdataSingleEn;   
    UINT8  u1HfootSingleEn;   
    UINT8  u1VheadSingleEn;   
    UINT8  u1VdataSingleEn;   
    UINT8  u1VfootSingleEn;   

    // LED output format
    UINT8 u1BitMode;       // bits in dimming data, 0:8bit , 1:10bit , 2:12bit ,  3:7bit

    UINT16 u2DimmingLowBound;
    UINT16 u2HistMaxThd;
    UINT8 u1BlockNumM;
    UINT8 u1BlockNumN;
#if defined (CC_MT5399) ||  defined (CC_MT5882)
/*
    PANEL_LIGHT_DIR_DIRECT=0,
    PANEL_LIGHT_DIR_LR=1,
    PANEL_LIGHT_DIR_TB=2,
    PANEL_LIGHT_DIR_L=3,
    PANEL_LIGHT_DIR_R=4,    
    PANEL_LIGHT_DIR_T=5,
    PANEL_LIGHT_DIR_B=6,  
*/
    UINT8 u1LightDirection;
    UINT8 u1ScanningEnable;         // bit1     0:no scanning 1:scanning
    UINT8 u1CurrentBoostEnable;     //bit2      0:Disable       1:Enable(pq current up)
    UINT8 u1H1BSelectEnable;            // bit6     0:V2B       1:H1B
    UINT8 u1ReverseEnable;          // bit7     0:Non reverse   1:Reverse
    
#endif
} PANEL_LOCAL_DIMMING_ATTRIBUTE_T;


enum 
{
  TCLK_PCLK_RATION_1,
  TCLK_PCLK_RATION_1P5,
  TCLK_PCLK_RATION_2,
  TCLK_PCLK_RATION_3,
  TCLK_PCLK_RATION_4,
  TCLK_PCLK_RATION_0P75
};
enum 
{
  TCON_TCLK_RATION_1,  
  TCON_TCLK_RATION_2,
};

typedef struct
{
    UINT32 u4PanelIndex ;   
    UINT8  u1Port;    
    UINT8  u1Ratio;
    UINT8  u1TconRatio;
} PANEL_MINILVDS_ATTRIBUTE_T;

UINT32 PANEL_IsSupportLocalDimming(void);
UINT32 PANEL_GetLocalDimmingParam(PANEL_LOCAL_DIMMING_ATTRIBUTE_T **pParam);

UINT32 PANEL_GetErrorLimit(void);

UINT32 PANEL_IsSupportMlvds(void);
UINT32 PANEL_GetMlvdsParam(PANEL_MINILVDS_ATTRIBUTE_T **pParam);


// for multiple panel interface (MLVDS, VB1, EPI) support
#define PANEL_DISPLAY_TYPE_LVDS  (0)
#define PANEL_DISPLAY_TYPE_VB1   (1)
#define PANEL_DISPLAY_TYPE_MLVDS (2)
#define PANEL_DISPLAY_TYPE_EPI   (3)
#define PANEL_DISPLAY_TYPE_iDP   (4)
#define PANEL_DISPLAY_TYPE_MAX   (5)

UINT32 PANEL_SetProtDisplayTypeMsk(UINT32 *u4ProtDisplayTypeMsk);
UINT32 PANEL_GetDisplayType(void);
UINT8 PANEL_IsSupportVb1(void);
void PANEL_SetColorDepth(UINT8 u1BitRes);
void PANEL_SetLvdsSelect(UINT8 u1Format);
void PANEL_SetLvdsSwap(UINT8 u1Inverse);
void PANEL_SetLvdsPCBSwap(UINT8 u1Inverse);
void PANEL_SetLvdsPixel(UINT8 u1Mode);
void PANEL_SetResolution(UINT16 u2Width, UINT16 u2Height);
void PANEL_SetHTotalTyp(UINT16 u2HTotal);
void PANEL_SetVTotal(UINT16 u2VTotal48Hz, UINT16 u2VTotal50Hz, UINT16 u2VTotal60Hz);
void PANEL_SetVertSyncWidth(UINT16 u2Vsync48Hz, UINT16 u2Vsync50Hz, UINT16 u2Vsync60Hz);
void PANEL_SetVertBackPorch(UINT16 u2VbackPorch48Hz, UINT16 u2VbackPorch50Hz, UINT16 u2VbackPorch60Hz);
void PANEL_SetHorizSyncWidth(UINT16 u2Hsync48Hz, UINT16 u2Hsync50Hz, UINT16 u2Hsync60Hz);
void PANEL_SetHorizBackPorch(UINT16 u2HbackPorch48Hz, UINT16 u2HbackPorch50Hz, UINT16 u2HbackPorch60Hz);

#endif /* PANEL_H */
