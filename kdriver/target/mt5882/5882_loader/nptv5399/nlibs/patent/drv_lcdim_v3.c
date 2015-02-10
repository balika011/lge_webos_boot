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
 * $RCSfile: drv_lcdim.c,v $
 *
 *---------------------------------------------------------------------------*/

/*
    1. design new local dimming panel table, add phase information, add light direction
    2. add SUPPORT_10BIT_DIMMING, to keep consistence between V1 / V2
    3. phase information : (1) phase sampling points (2) phase x/y (3) phase ratio
    4. check if temporal filter reset would reset itself (no need to toggle)
*/

// === INCLUDE =========================================================================
#include "general.h"
#include "vdo_misc.h"
#include "drv_lcdim_v2.h"
#include "hw_lcdim_v2.h"
#include "panel.h"
#include "drv_meter.h"
#include "nptv_debug.h"
#include "hw_sw.h"
#include "hw_ospe.h"
#include "drv_ldmspi.h"
#include "drv_di.h"


// === DEFINE ===========================================================================

#define MAX_DIM_BLOCK 200
#define RGB_MAX_DIFF_THD        1000
#define RGB_TOTAL_DIFF_THD      2000
#define RGB_AVG_DIFF_THD        40
#define RGB_HIST_NORM_BASE      4096
#define HUE_HIST_NORM_BASE      4096

#define SUPPORT_10BIT_DIMMING   SV_FALSE

#define AbsDiff(a, b)   (((a)>(b))?((a)-(b)):((b)-(a)))
#define CLIP(val, min, max) ((val>=max) ? max : ((val<=min) ? min : val)) 

#define SW_CTRL_PT_NUM 33
#define SW_LPF_SUM 128
#define SW_LPF_SUM_SFT 7  // 8 = 2^3

#define POST_INFO_QUE 8

typedef enum
{
    NO_OSD=0,
    OSD_APPEAR=1,
	OSD_DISAPPEAR=2,
	OSD_SHOW=3,
} LCDIM_OSD_ITEM;

typedef enum
{
    E_LCDIM_BLOCK_CONF=0,
    E_LCDIM_NM_MOTION_CONF=1,
	E_LCDIM_ITEM_MAX=2,
} LCDIM_QUE_ITEM;

typedef enum
{
    E_RGB_SCG_LEVEL_LOW=0,
    E_RGB_SCG_LEVEL_MID_DN=1,
    E_RGB_SCG_LEVEL_MID_UP=2,    
    E_RGB_SCG_LEVEL_HIGH=3,
} LCDIM_SCG_LEVEL;

typedef enum
{
    E_MOTION_LEVEL_ZERO=0,
    E_MOTION_LEVEL_LOW=1,
    E_MOTION_LEVEL_MID=2,
    E_MOTION_LEVEL_HIGH=3,
    E_MOTION_LEVEL_EXT=4,    
} LCDIM_MOTION_LEVEL;

typedef enum
{
    E_POST_HUE_CNT=0,
    E_POST_ASL=1,
    E_POST_APL=2,
} LCDIM_POST_INFO;

// === GLOBAL VARIABLE ====================================================================
UINT32 u4DataQue[2][256];
UINT32 u4QueCounter[2] = {0, 0};

UINT8 bSCGAdapGainLimitFlg = SV_FALSE;
UINT8 bSCGAdapGainLimitUpdate = SV_FALSE;
UINT8 bSCGAdapGainLimitSpeedUp = SV_FALSE;
UINT8 bSCGAdapSpiIIRWeight = 0;

UINT8 u1QueSizeForBlock = 0x0A;
UINT8 u1QueSizeForAve = 0x8;

UINT8 u1LcdimOutputDimming[MAX_DIM_BLOCK] = {0};
UINT8 u1BlkDimResult[LCDIM_MAX_BLK_NUM];    
UINT16 u2LcdimDimResult[LCDIM_MAX_BLK_NUM];    


UINT8 u1ForceBlockDimOnOff = SV_OFF;
UINT16 u2BlockH = 0; 
UINT16 u2BlockV = 0;
UINT32 u4CurrentDemoBlock = 0;
UINT32 u4DemoFrameCount = 0;
UINT32 u4DemoBlockWidth = 0;

UINT32 u4MotionConf;
UINT32 u4MotionConfFiltered;

INT16 LcdimSCGConf = 0;
UINT8 u1LcdimSCGStrength = SV_FALSE;
UINT8 u1ASLVal = 0;
UINT32 u4HuePxlCnt = 0;

UINT16 u2AdvPSRatio = 0;

UINT16 u2RGBAverage;
UINT8 u1RGBCGFlat;
UINT8 u1RGBCGDist;
UINT8 u1OSDSCG = 0;
UINT8 bDetectNotCG = 0;

UINT8 u1SCGflg = 0;
UINT8 u1CGSpeedUpflg = 0;

UINT32 u4DimResultAvg = 0;

UINT8 u1Lcdim3DStatus = 0;
UINT8 u1Lcdim3DStatusCtrl = 0;

UINT32 u4POSTDataQue[3][POST_INFO_QUE];

UINT8 u1LCDIMOnOffStatus = 0;

UINT8 u1LcdimDftSwCurve[SW_CTRL_PT_NUM] = {0x40, 0x40, 0x40, 0x46, 0x4C, 0x58, 0x64, 0x72, 0x81, 0x8F, 0x9D, 0xA8, 0xB4, 0xBE, 0xC9, 0xD3, 0xDE,
                                           0xE5, 0xEC, 0xF1, 0xF7, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};    // default dimming curve
UINT8 u1LcdimStrSwCurve[SW_CTRL_PT_NUM] = {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x46, 0x4D, 0x57, 0x62, 0x6C, 0x77, 0x82, 0x8E, 0x9A, 0xA7,
                                           0xB3, 0xC0, 0xCA, 0xD5, 0xDD, 0xE6, 0xEC, 0xF3, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};    // default dimming curve
UINT8 u1LcdimSWCtrlPtPre[2][SW_CTRL_PT_NUM];  // recording dimming curve control point of previous frame


LCDIM_SPATIAL_CFG_T rLCDIMSpCfg;
LCDIM_TEMPORAL_CFG_T rLCDIMTplCfg;
LCDIM_DEBUG_LOG_EN_T rLCDIMDbgEn;
LCDIM_FUNCTION_EN_T rLCDIMFncEn;
LCDIM_SPCTRL_CFG_T rLCDIMSpCtrlCfg;
LCDIM_SCENECHG_CFG_T rLCDIMSCGCfg;
LCDIM_AGL_CFG_T rLCDIMAGLCfg;

// === EXTERN FUNCTION ====================================================================
EXTERN UINT8 bSupportLocalDimming;
EXTERN PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

REGTBL_T const REGTBL_LCDIM_V2[] =
{
    {     LD2_MODE_00, 0xC05803A0, 0xFC0107FF},
//    {     LD2_MODE_01, 0x0C8700A0, 0xFFFFFFFF},
//    {     LD2_MODE_02, 0x20000050, 0xE7FF1FFF},
    {     LD2_MODE_04, 0x1024383F, 0x3F3F3F3F},
    {     LD2_MODE_05, 0x017FF080, 0x0FFFF3FF},
    {     LD2_MODE_06, 0x00100104, 0x7FFFFFFF},
    {     LD2_MODE_07, 0x00FE37A4, 0x0FFC0000},
    {     LD2_MODE_08, 0x00041CE6, 0x0FFFBFFF},
    {     LD2_MODE_09, 0x3FF32244, 0x000F73FF},
    {     LD2_MODE_17, 0xFFFF0000, 0xFFFFFFFF},        
    {     LD2_MODE_33, 0x80000000, 0xFFFFFFFF},        
    {     REGTBL_END,  0x00000000, 0x00000000}
};

UINT8 u1LcdimPhase[LDM_LP_TOTAL_PHASE_NUM * 2 + 4] = 
{
// (x, y)    
    0, 0,     // phase 0    
    1, 0,     // phase 1
    0xF, 0,     // phase 2
    2, 0,     // phase 3
    0xE, 0,     // phase 4
    3, 0,     // phase 5
    0xD, 0,     // phase 6
    4, 0,     // phase 7
    0xC, 8,     // phase 8
    8, 8,     // phase 9
    8, 8,     // phase 10
    8, 8,     // phase 11
    8, 8,     // phase 12
    8, 8,     // phase 13
    8, 8,     // phase 14
    8, 8,     // phase 15
    8, 8,     // phase 16
    8, 8,     // phase 17
    8, 8,     // phase 18
    8, 8,     // phase 19
    8, 8,     // phase 20
// phase config    
    3,        // vertical phase ratio           
    0x12,        // vertical phase sampling width
    2,        // horizontal phase ratio
    0x4         // horizontal phase sampling width    
};

UINT8 u1LightProfileTable[LCDIM_LIGHTPROF_NUM]=
{
    122,79,79,16,16,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    123,95,58,21,11,8,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    114,97,42,34,13,7,10,10,1,0,0,0,0,0,0,0,0,0,0,0,0,
    97,114,34,42,7,13,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,
    122,79,79,16,16,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    123,95,58,21,11,8,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    114,97,42,34,13,7,10,10,1,0,0,0,0,0,0,0,0,0,0,0,0,
    97,114,34,42,7,13,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,
    122,79,79,16,16,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    123,95,58,21,11,8,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    114,97,42,34,13,7,10,10,1,0,0,0,0,0,0,0,0,0,0,0,0,
    97,114,34,42,7,13,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,
    122,79,79,16,16,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    123,95,58,21,11,8,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    114,97,42,34,13,7,10,10,1,0,0,0,0,0,0,0,0,0,0,0,0,
    97,114,34,42,7,13,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,
    122,79,79,16,16,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    123,95,58,21,11,8,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,
    114,97,42,34,13,7,10,10,1,0,0,0,0,0,0,0,0,0,0,0,0,
    97,114,34,42,7,13,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,
    131,81,81,14,14,4,4,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
    130,90,64,21,11,4,7,4,0,0,0,0,0,0,0,0,0,0,0,0,0,
    121,101,44,34,10,7,7,7,1,0,0,0,0,0,0,0,0,0,0,0,0,
    101,121,34,44,7,10,7,7,1,0,0,0,0,0,0,0,0,0,0,0,0,
    130,74,74,19,19,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    132,89,58,24,16,4,4,4,1,0,0,0,0,0,0,0,0,0,0,0,0,
    127,104,41,39,11,8,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
    104,127,39,41,8,11,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    139,76,76,20,20,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    144,92,54,29,8,2,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
    128,117,46,37,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
    117,128,37,46,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
    148,73,73,15,15,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
    165,94,56,10,3,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
    142,102,43,31,12,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    102,141,31,43,1,12,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,
    158,73,73,13,13,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    161,89,52,25,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    136,112,42,31,1,9,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    112,136,31,42,9,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    169,72,72,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    157,95,52,21,3,0,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
    148,117,40,25,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    117,148,25,40,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    180,68,68,6,6,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    167,99,54,10,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    153,118,31,27,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
    118,153,27,31,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
    161,68,68,17,17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    171,102,43,14,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    150,133,22,22,1,0,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    133,150,22,22,0,1,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,
    183,70,70,3,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    181,93,41,6,10,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    160,121,28,18,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    121,160,18,28,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    212,59,59,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    188,102,34,3,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    170,129,15,11,2,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    129,170,11,15,6,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    216,56,56,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    210,85,30,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    181,125,15,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    125,181,11,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    249,41,41,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    222,70,27,7,3,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,
    187,126,7,7,3,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,
    127,188,7,7,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    249,41,41,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    222,70,27,7,3,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,
    187,126,7,7,3,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,
    127,188,7,7,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0    
};

UINT16 u2LcdimDimTable[LCDIM_DIMMING_NUM]=
{
    0, 0, 4, 8, 12, 24, 36, 48, 64, 84, 
    104, 128, 156, 188, 220, 256, 296, 336, 384, 432, 
    480, 536, 596, 656, 720, 788, 860, 932, 1012, 1023, 
    1023, 1023, 1023
};

UINT16 u2LcdimDimTable_OFF[LCDIM_DIMMING_NUM]=
{
    1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 
    1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023
};

// Low / MID / Strong for Demo
UINT16 u2LcdimDimTable_LOW[LCDIM_DIMMING_NUM]=
{
    0, 1, 3, 8, 14, 23, 35, 49, 65, 84, 106, 131, 159, 189, 223, 259, 299, 342, 387, 436, 488, 544, 602, 664, 729, 798, 870, 945, 1023, 1023, 1023, 1023, 1023
};

UINT16 u2LcdimDimTable_MID[LCDIM_DIMMING_NUM]=
{
    0, 1, 3, 7, 13, 21, 32, 45, 60, 78, 98, 121, 147, 175, 206, 240, 277, 316, 359, 404, 452, 503, 558, 615, 675, 739, 805, 875, 948, 1023, 1023, 1023, 1023
};

UINT16 u2LcdimDimTable_STRONG[LCDIM_DIMMING_NUM]=
{
    0 ,1 ,3 ,6 ,12 ,20 ,30 ,42 ,56 ,72 ,91 ,113 ,136 ,163 ,191 ,223 ,257 ,294 ,333 ,375 ,420 ,467 ,518 ,571 ,627 ,686 ,747 ,812 ,880 ,950 ,1023 ,1023 ,1023
};

// Dft / Str for curve adaptive
UINT16 u2LcdimDimTable_DFT[LCDIM_DIMMING_NUM]=
{
    0 ,1 ,3 ,6 ,12 ,20 ,30 ,42 ,56 ,72 ,91 ,113 ,136 ,163 ,191 ,223 ,257 ,294 ,333 ,375 ,420 ,467 ,518 ,571 ,627 ,686 ,747 ,812 ,880 ,950 ,1023 ,1023 ,1023
};

UINT16 u2LcdimDimTable_STR[LCDIM_DIMMING_NUM]=
{
    0 ,1 ,3 ,6 ,12 ,20 ,30 ,42 ,56 ,72 ,91 ,113 ,136 ,163 ,191 ,223 ,257 ,294 ,333 ,375 ,420 ,467 ,518 ,571 ,627 ,686 ,747 ,812 ,880 ,950 ,1023 ,1023 ,1023
};

UINT16 u2LcdimGainTable[LCDIM_GAIN_NUM]=
{
    1024, 1232, 1404, 1552, 1688, 1808, 1924, 2028, 2128, 2224, 
    2312, 2396, 2480, 2560, 2636, 3168, 3612, 3996, 4342, 4657,
    4948, 5220, 5477, 5719, 5950, 6170, 6382, 6585, 6781, 7505,
    8153, 8745, 9292, 9803,10284,10740,11173,11587,11984,12366,
    12734,  
};

UINT16 u2LcdimGainTable_LOW[LCDIM_GAIN_NUM]=
{
    1024, 1232, 1404, 1552, 1688, 1808, 1924, 2028, 2128, 2224, 
    2312, 2396, 2480, 2560, 2636, 3168, 3612, 3996, 4342, 4657,
    4948, 5220, 5477, 5719, 5950, 6170, 6382, 6585, 6781, 7505,
    8153, 8745, 9292, 9803,10284,10740,11173,11587,11984,12366,
    12734,  
};

UINT16 u2LcdimGainTable_MID[LCDIM_GAIN_NUM]=
{
    1024, 1232, 1404, 1552, 1688, 1808, 1924, 2028, 2128, 2224, 
    2312, 2396, 2480, 2560, 2636, 3168, 3612, 3996, 4342, 4657,
    4948, 5220, 5477, 5719, 5950, 6170, 6382, 6585, 6781, 7505,
    8153, 8745, 9292, 9803,10284,10740,11173,11587,11984,12366,
    12734,  
};

UINT16 u2LcdimGainTable_STRONG[LCDIM_GAIN_NUM]=
{
    1024, 1232, 1404, 1552, 1688, 1808, 1924, 2028, 2128, 2224, 
    2312, 2396, 2480, 2560, 2636, 3168, 3612, 3996, 4342, 4657,
    4948, 5220, 5477, 5719, 5950, 6170, 6382, 6585, 6781, 7505,
    8153, 8745, 9292, 9803,10284,10740,11173,11587,11984,12366,
    12734,  
};

UINT16 u2LcdimGainLimitTable[LCDIM_GAIN_LIMIT_NUM]=
{
    4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 3724, 3324, 
    3004, 2742, 2524, 2339, 2181, 2044, 1924, 1818, 1724, 1640, 
    1564, 1495, 1433, 1376, 1324, 1276, 1232, 1191, 1153, 1117, 
    1084, 1053, 1024
};

UINT16 u2LcdimGainLimitTable_LOW[LCDIM_GAIN_LIMIT_NUM]=
{
    4095, 4095, 4095, 4095, 4095, 4095, 4095, 4075, 3584, 3190, 2870, 2606, 2387, 2206, 2047, 1912, 1791, 1685, 1593, 1509, 1434, 1364, 1303, 1246, 1194, 1146, 1102, 1067, 1052, 1040, 1024, 1024, 1024
};

UINT16 u2LcdimGainLimitTable_MID[LCDIM_GAIN_LIMIT_NUM]=
{
    4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 3717, 3299, 2974, 2702, 2473, 2285, 2122, 1979, 1854, 1747, 1648, 1562, 1484, 1414, 1349, 1290, 1237, 1187, 1142, 1137, 1097, 1051, 1040, 1024, 1024
};

UINT16 u2LcdimGainLimitTable_STRONG[LCDIM_GAIN_LIMIT_NUM]=
{
    4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 3835, 3421, 3076, 2787, 2562, 2360, 2196, 2047, 1919, 1805, 1706, 1616, 1535, 1463, 1395, 1335, 1279, 1228, 1181, 1137, 1124, 1081, 1051, 1038, 1024
};

UINT16 u2LcdimGainLimitTable_UP_BND[LCDIM_GAIN_LIMIT_NUM]=
{
    3500, 2900, 2610, 2380, 2230, 2120, 2040, 1960, 1880, 1840, 1800, 1780, 1760, 1740, 1720, 1700,  
    1670, 1640, 1596, 1468, 1392, 1360, 1340, 1320, 1280, 1226, 1181, 1176, 1125, 1088, 1039, 1035, 1024 

};

UINT16 u2LcdimGainLimitTable_LOW_BND[LCDIM_GAIN_LIMIT_NUM]=
{
    3300, 2900, 2600, 2300, 2100, 2000, 1930, 1860, 1790, 1720, 1650, 1580, 1510, 1440, 1370, 1300,  
    1250, 1200, 1150, 1100, 1080, 1060, 1040, 1033, 1032, 1031, 1030, 1029, 1028, 1027, 1026, 1025, 1024
};

UINT16 u2LcdimGainLimitTable_Adap[LCDIM_GAIN_LIMIT_NUM]=
{
    3500, 2900, 2610, 2380, 2230, 2120, 2040, 1960, 1880, 1840, 1800, 1780, 1760, 1700, 1600, 1500,  
    1400, 1300, 1200, 1100, 1072, 1068, 1064, 1060, 1056, 1052, 1048, 1044, 1040, 1036, 1032, 1028, 1024 
};

void vDrvLcdimQualitySWInit(void)
{
    UINT8 i;
    
    // SW Register / Variable Initialization
    vIO32WriteFldAlign(ADAP_LCDIM_00, 1, ADAP_LCDIM_FORCE_EN);
    vIO32WriteFldAlign(ADAP_LCDIM_00, 0, ADAP_LD_CUR_CTRL);    
    
    // Init SW Dimming Curve
    for(i = 0; i < SW_CTRL_PT_NUM; i++)
    {
        vIO32Write1B(LCDIM_SW_CTRL_PT_DFT_0+i, u1LcdimDftSwCurve[i]);
        vIO32Write1B(LCDIM_SW_CTRL_PT_STR_0+i, u1LcdimStrSwCurve[i]);        
//        vIO32Write1B(LCDIM_SW_DS_CTRL_PT_0_3+i, (UINT8)(u2LcdimGainTable_MID[i]>>4));       
    }
    // Init SW Gain limit Curve
    for(i = 0; i < LCDIM_GAIN_LIMIT_NUM; i++)
    {
        vIO32Write2B(ADAP_LCDIM_ADAP_GAIN_LMT_H_0_1+2*i, u2LcdimGainLimitTable_UP_BND[i]);
        vIO32Write2B(ADAP_LCDIM_ADAP_GAIN_LMT_L_0_1+2*i, u2LcdimGainLimitTable_LOW_BND[i]);
    }

    //Scene change threshold
    vIO32WriteFldAlign(LCDIM_SCENE_CHANGE0, 260, LCDIM_SCG_RGB_MAX_DIFF_THD);
    vIO32WriteFldAlign(LCDIM_SCENE_CHANGE0, 300, LCDIM_SCG_RGB_TOTAL_DIFF_THD);
    vIO32WriteFldAlign(LCDIM_SCENE_CHANGE1, 10, LCDIM_SCG_RGB_AVG_DIFF_THD);   

    //SCG speed control setting
    vIO32WriteFldAlign(LCDIM_SCENE_CHANGE2, 100, LCDIM_SCG_RGB_MAX_DIFF_THD2);
    vIO32WriteFldAlign(LCDIM_SCENE_CHANGE2, 50, LCDIM_SCG_RGB_TOTAL_DIFF_THD2); 
    vIO32WriteFldAlign(LCDIM_SCENE_CHANGE1, 2, LCDIM_SCG_RGB_AVG_DIFF_THD2);

    vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 1, LCDIM_SPEED_CTRL_SCG_EN);    
    vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 1, LCDIM_ADAP_MINDIM_EN);    
    vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 1, LCDIM_ADAP_GAINLIMT_EN);        
    vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 1, LCDIM_CG_BOOST_EN);            
    vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 0, LCDIM_OSD_DETECT_EN);     
    vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 1, LCDIM_RFCHG_DETECT_EN);
 
    vIO32WriteFldAlign(ADAP_LCDIM_02, 0x36, LCDIM_NR_MOTION_THD);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL3, 2, LCDIM_SPCTRL_MOTION_PENALTY);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL3, 0, LCDIM_SPCTRL_GAMMA_PENALTY);    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL2, 0xA8, LCDIM_SPCTRL_MAXRGB_STEP_HIGH);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL2, 0x58, LCDIM_SPCTRL_MAXRGB_STEP_MID_UP);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL2, 0x28, LCDIM_SPCTRL_MAXRGB_STEP_MID_DN);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL2, 0x00, LCDIM_SPCTRL_MAXRGB_STEP_LOW);    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL3, 1, LCDIM_SPCTRL_STATIC_STEP);
    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL4, 0x20, LCDIM_SPCTRL_CONF_TH0);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL4, 0x38, LCDIM_SPCTRL_CONF_TH1);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL4, 0x50, LCDIM_SPCTRL_CONF_TH2);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL4, 0xA0, LCDIM_SPCTRL_CONF_TH3);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL5, 0x1E, LCDIM_SPCTRL_CONF_TH_DFT);    
    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL6, 0x04, LCDIM_MAX_UP_DIM_STEP_HIGH);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL6, 0x1A, LCDIM_MAX_UP_DIM_STEP_MID_UP);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL6, 0x2E, LCDIM_MAX_UP_DIM_STEP_MID_DN);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL6, 0x3F, LCDIM_MAX_UP_DIM_STEP_LOW);

    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL7, 0x03, LCDIM_MAX_DN_DIM_STEP_HIGH);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL7, 0x07, LCDIM_MAX_DN_DIM_STEP_MID_UP);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL7, 0x17, LCDIM_MAX_DN_DIM_STEP_MID_DN);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL7, 0x1E, LCDIM_MAX_DN_DIM_STEP_LOW);

    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL8, 0x08, LCDIM_DY_DIM_SLOPE_HIGH);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL8, 0x15, LCDIM_DY_DIM_SLOPE_MID_UP);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL8, 0x2E, LCDIM_DY_DIM_SLOPE_MID_DN);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL8, 0x3F, LCDIM_DY_DIM_SLOPE_LOW);

    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, 0, LCDIM_CG_BOOST_0);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, 2, LCDIM_CG_BOOST_1);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, 2, LCDIM_CG_BOOST_2);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, 6, LCDIM_CG_BOOST_FRAME_CNT);

    vIO32WriteFldAlign(LCDIM_MAXINFO_SCG, 0, LCDIM_SCG_MAXHIST_LOW_APL_TH);    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL5, 0, LCDIM_SPCTRL_SCG_STABLE_TH);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL5, 2, LCDIM_SPCTRL_SCG_WAIT_CNT);    
    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL9, 16, LCDIM_PREDETECT_APL_TH_LOW);    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL9, 43, LCDIM_PREDETECT_APL_TH_HIGH);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, 26, LCDIM_PREDETECT_APL_TH_OSTG);    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL9, 0x00, LCDIM_SPCTRL_PREDETECT_STEP);    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL9, 0x3, LCDIM_SPCTRL_PREDETECT_FRAME_TH);        
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, 0, LCDIM_PREDETECT_MOTION_TH);     
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL11, 3, LCDIM_FADEIN_DARK_TH);
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL11, 3, LCDIM_FADEIN_INC_TH);    
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL11, 0x00, LCDIM_FADEIN_STEP);          
    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL11, 0x00, LCDIM_DOWNTREND_DIM_TH);    

//Adap min Dim
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL1, 13, LCDIM_ADAP_DIM_MIN_LOW);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL1, 192, LCDIM_ADAP_DIM_MIN_HIGH);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL0, 40, LCDIM_ADAP_DIM_DIST_TH);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL0, 64, LCDIM_ADAP_DIM_APL_TH);    
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, 0x3F, LCDIM_ADAP_DIM_WEIGHT0_L);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, 0x2C, LCDIM_ADAP_DIM_WEIGHT1_L);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, 0x13, LCDIM_ADAP_DIM_WEIGHT2_L);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, 0x0C, LCDIM_ADAP_DIM_WEIGHT3_L);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL3, 0x05, LCDIM_ADAP_DIM_WEIGHT4_L);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, 0x3F, LCDIM_ADAP_DIM_WEIGHT0_H);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, 0x3F, LCDIM_ADAP_DIM_WEIGHT1_H);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, 0x3F, LCDIM_ADAP_DIM_WEIGHT2_H);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, 0x30, LCDIM_ADAP_DIM_WEIGHT3_H);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL5, 0x28, LCDIM_ADAP_DIM_WEIGHT4_H);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL1, 0x80, LCDIM_ADAP_DIM_CONF_COR);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL1, 0xA0, LCDIM_ADAP_DIM_SPCONF_TH);    
    
//Adaptive Gain Limit
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_GAIN_LMT_0, 0x40, LCDIM_ADAP_GAIN_LMT_COR);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_GAIN_LMT_0, 0xE0, LCDIM_ADAP_GAIN_LMT_APLTH_L);    
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_GAIN_LMT_0, 0xF0, LCDIM_ADAP_GAIN_LMT_APLTH_H);
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_GAIN_LMT_0, 0x01, LCDIM_ADAP_GAIN_LMT_CONF_STEP);    
//Dimming Result IIR    
    vIO32WriteFldAlign(LCDIM_00, 0x01, LCDIM_ADAP_SPI_IIR_EN);    
    vIO32WriteFldAlign(LCDIM_01, 0x01, LCDIM_FILTER_TYPE);    
//Advanced Power Saving
    vIO32WriteFldAlign(ADAP_LCDIM_ADV_PS, 205, LCDIM_ADV_PS_RATIO);   //80%

//3D detection
    vIO32WriteFldAlign(LCDIM_3D_DETECT, 32, ADAP_LCDIM_SBS_LOW_TH);           
    vIO32WriteFldAlign(LCDIM_3D_DETECT, 116, ADAP_LCDIM_SBS_HIGH_TH);           
    vIO32WriteFldAlign(LCDIM_3D_DETECT, 1, ADAP_LCDIM_SBS_DETECT_EN);     

//Temporal IIR
    vIO32WriteFldAlign(LCDIM_TEMP_IIR_0, 2, ADAP_LCDIM_IIR_CUR_W_MIN);
    vIO32WriteFldAlign(LCDIM_TEMP_IIR_0, 0x40, ADAP_LCDIM_IIR_CUR_W);     
    vIO32WriteFldAlign(LCDIM_TEMP_IIR_0, 0, ADAP_LCDIM_IIR_FIX_W_EN);     
    vIO32WriteFldAlign(LCDIM_TEMP_IIR_1, 0x40, ADAP_LCDIM_IIR_DIFF_GAIN_MAX);
    vIO32WriteFldAlign(LCDIM_TEMP_IIR_1, 0x04, ADAP_LCDIM_IIR_DIFF_GAIN_MIN);    
    vIO32WriteFldAlign(LCDIM_ADAP_CLIP_0, 0x80, ADAP_LCDIM_ADAP_CLIP_RATIO);
    vIO32WriteFldAlign(LCDIM_ADAP_CLIP_1, 0x20, ADAP_LCDIM_ADAP_CLIP_DIM_MIN);
    vIO32WriteFldAlign(LCDIM_ADAP_CLIP_1, 0x80, ADAP_LCDIM_ADAP_CLIP_DIM_MAX);
    vIO32WriteFldAlign(LCDIM_ADAP_CLIP_1, 0x20, ADAP_LCDIM_ADAP_CLIP_TH_MIN);
    vIO32WriteFldAlign(LCDIM_ADAP_CLIP_1, 0x60, ADAP_LCDIM_ADAP_CLIP_TH_MAX);
}

void vDrvLcdimQualityHWInit(void)
{
    vDrvLoadRegTbl(REGTBL_LCDIM_V2);

    // LcDim Tables
    vDrvLcdimWriteLightProfile(u1LightProfileTable);
    vDrvLcdimWriteDimTable(u2LcdimDimTable_MID);
    vDrvLcdimWriteGainTable(u2LcdimGainTable_MID);
    vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_MID);    
    vDrvLcdimSetPhase(u1LcdimPhase);
    if(u1DrvLcDimCalAvgDivider())
    {
        vIO32WriteFldAlign(LD2_MODE_16, 16, C_LD_BLKINFO_WEIGHT);        
        Printf("[LDM] Panel Block Avg Initialization Pass\b");
    }

#ifndef SUPPORT_LCDIM_AVG_DEMO
    //In SW mode, set small C_LD_SPI_LIMIT_LNUM to prevent garbage
    if((pParam->u1SWModeEn == SV_TRUE))
    {
        vIO32WriteFldAlign(LD2_MODE_08, 2, C_LD_SPI_LIMIT_LNUM);          
    }    
#endif
}

void vDrvLcDimQualityInit(void)
{
    vDrvLcdimQualitySWInit();
    vDrvLcdimQualityHWInit();
}

BOOL bDrvLcDimPanelInit(void)
{
    // Panel Initialization
    if(bDrvLcDimSetPanelInfo(pParam->u1BlockNumM, pParam->u1BlockNumN, pParam->u1LightDirection) == SV_FAIL)        
    {
        Printf("[LDM] Panel Block Initialization Fail\b");
        return SV_FAIL;
    }
    return SV_SUCCESS;

}

UINT8 u1CheckCGDist(const UINT16 * waHistCur)
{
	UINT8 bi;
    UINT32 dwLCDIMDist;
    
    for (bi = 0, dwLCDIMDist = 0; bi < LUMA_HIST_NUM; bi++)
    {
        dwLCDIMDist += AbsDiff(bi * 8, u2RGBAverage) * waHistCur[bi];
    }
    
    // shift 12 bits to make distribution a 8 bits value
    dwLCDIMDist = dwLCDIMDist >> 12;
    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL0, (UINT8)dwLCDIMDist, LCDIM_ADAP_DIM_DIST);
    return (UINT8)dwLCDIMDist;

}
UINT8 u1CheckCGFlat(const UINT16 * waHistCur)
{
	UINT8 bi;
    UINT32 u4MaxHist1st = 0, u4MaxHist2nd = 0;
    
	for (bi = 0; bi < LUMA_HIST_NUM; bi++)
	{
        if(waHistCur[bi] >= u4MaxHist1st)   //95%
        {
            u4MaxHist2nd = u4MaxHist1st;
            u4MaxHist1st = waHistCur[bi];
        }
        else if(waHistCur[bi] >= u4MaxHist2nd)
        {
            u4MaxHist2nd = waHistCur[bi];
        }
	}
    

    if((u4MaxHist1st + u4MaxHist2nd) > 3890)    // 3890/4096
    {
        vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL3, 1, LCDIM_ADAP_DIM_FLAT_FLG);
        return SV_TRUE;    
    }
    else
    {
        vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL3, 0, LCDIM_ADAP_DIM_FLAT_FLG);
        return SV_FALSE;
    }

}

#define OSD_EN Fld(1, 0, AC_MSKB0) //0
UINT8 u1DrvLcdimGetOSDStatus(void)
{
    UINT8 u1Enable = 0;
    
    if(IO32ReadFldAlign(0xF0021300, OSD_EN) == SV_TRUE)
    {
        u1Enable = SV_TRUE;
    }
    else
    {
        u1Enable = SV_FALSE;
    }
    return u1Enable;

}

UINT8 u1DrvLcdimDetectOSDSCG(void)
{
    static UINT8 u1PreOSDflg = 0;    
    UINT8 u1CurOSDflg = 0;    
	UINT8 bRet;  

    if(rLCDIMFncEn.u1OSDDetectEn == SV_TRUE)
    {
        u1CurOSDflg = u1DrvLcdimGetOSDStatus();//(UINT8)OSD_BASE_IsDisplaying();

        if((u1PreOSDflg == SV_FALSE) && (u1CurOSDflg == SV_TRUE))
        {
            bRet = OSD_APPEAR;
        }
        else if((u1PreOSDflg == SV_TRUE) && (u1CurOSDflg == SV_FALSE))
        {
            bRet = OSD_DISAPPEAR;
        }        
        else if((u1PreOSDflg == SV_TRUE) && (u1CurOSDflg == SV_TRUE))
        {
            bRet = OSD_SHOW;
        }            
        else    
        {
            bRet = NO_OSD;
        }
    }
    else
    {
        bRet = NO_OSD;
    }

//    bRet = ((u1PreOSDflg == SV_FALSE) && (u1CurOSDflg == SV_TRUE)) ? SV_TRUE : SV_FALSE;
    u1PreOSDflg = u1CurOSDflg;    

    return bRet;


}

UINT32 u4DrvLcdimPostInfoQue(UINT8 u1Item, UINT32 u4Value)
{
    UINT8 bi = 0;
    for(bi = 0; bi < (POST_INFO_QUE - 1); bi++)
    {
        u4POSTDataQue[u1Item][bi] = u4POSTDataQue[u1Item][bi + 1];
    }
    u4POSTDataQue[u1Item][POST_INFO_QUE - 1] = u4Value;

    return u4POSTDataQue[u1Item][0];

}


void u1DrvLcdimGetPostInfo(void)
{
//    UINT8 u1ASLPostPre = IO32ReadFldAlign(METER_INFO_00, METER_ASL_VAL);
//    UINT8 u1APLPostPre = IO32ReadFldAlign(METER_INFO_00, METER_APL_VAL);
    UINT32 u4HueCntPostPre = 0;
    UINT16 u2HueHist[HUE_HIST_NUM];

    u1DrvGetHueHist(u2HueHist, &u4HueCntPostPre, HUE_HIST_NORM_BASE);    
    
    u4HuePxlCnt = u4DrvLcdimPostInfoQue(E_POST_HUE_CNT, u4HueCntPostPre);
//    u1ASLPost = (UINT8)u4DrvLcdimPostInfoQue(E_POST_ASL, u1ASLPostPre);
//    u1APLPost = (UINT8)u4DrvLcdimPostInfoQue(E_POST_APL, u1APLPostPre);

    if(rLCDIMDbgEn.u1LcdimDebug_PostInfo == SV_TRUE)
    {
        LOG(4, "Hue Cnt: Pre = %d, Cur = %d \n", u4HueCntPostPre, u4HuePxlCnt);
        //LOG(4, "ASL: Pre = %d, Cur = %d \n", u1ASLPostPre, u1ASLPost);
        //LOG(4, "APL: Pre = %d, Cur = %d \n", u1APLPostPre, u1APLPost);
    }
}


#define DIM_RESULT_QUE 4
UINT8 u1DrvLcdimDetectDimTrend(void)
{
    static UINT32 u4DimResultAvgRec[DIM_RESULT_QUE] = {0, 0, 0, 0};
    UINT8 bi = 0;
    UINT8 bRet = SV_FALSE;
    UINT8 bDecCnt = 0;

    if((rLCDIMSpCtrlCfg.u1DimDowntrendTh == 0))
    return SV_FALSE;

//update
    for(bi = 0; bi < (DIM_RESULT_QUE - 1); bi++)
    {
        u4DimResultAvgRec[bi] = u4DimResultAvgRec[bi + 1];
    }
    u4DimResultAvgRec[DIM_RESULT_QUE - 1] = u4DimResultAvg;
    
//detect trend
    for(bi = 0; bi < (DIM_RESULT_QUE - 1); bi++)
    {
        if(u4DimResultAvgRec[bi] > u4DimResultAvgRec[bi + 1])
        {
            bDecCnt ++;
        }
    }

    if((bDecCnt == (DIM_RESULT_QUE - 1)) && (u4DimResultAvg >= (rLCDIMSpCtrlCfg.u1DimDowntrendTh * 4) ))
    {
        bRet = SV_TRUE;     //Dim result is down trend
        if(rLCDIMDbgEn.u1LcdimDebug_Downtrend == SV_TRUE)
        {
            LOG(4,"u1DrvLcdimDetectDimTrend = TRUE, u4DimResultAvg = %d", u4DimResultAvg);
        }
    }

    if(rLCDIMDbgEn.u1LcdimDebug_Downtrend == SV_TRUE)
    {
        Printf("u4DimResultAvgRec =\n");
        
        for (bi = 0; bi < DIM_RESULT_QUE; bi++)
        {
            Printf("%d ", u4DimResultAvgRec[bi]);
        }
        Printf("bDecCnt = %d\n", bDecCnt);
    }

    return bRet;
}

UINT8 u1DrvLcdimDetectSCGLevel(void)
{
	static UINT16 PreRGBAverage = 0;	
	static UINT8 bPreSc = SV_FALSE;
    static UINT16 u2HistCur[RGB_MAX_HIST_NUM];		
	static UINT16 u2HistRec[RGB_MAX_HIST_NUM];
    static UINT32 u4HuePxlCntPre = 0;
    static UINT8 bSCGCnt = 0;
    static UINT8 bSCGQue = 0;
    static UINT8 bCGBoostCnt = 0;        
	UINT8 bi, bCurSc;
	UINT8 bRet;
	UINT16 wMaxDiff = 0, wTotalDiff = 0, wDiff = 0, wAPLDiff = 0;    


    UINT8 bAPLUpFlg = 0;
    UINT8 bDimTrend = u1DrvLcdimDetectDimTrend();
    
    bDetectNotCG = ((u4HuePxlCntPre > (UINT32)(HUE_HIST_NORM_BASE>>2))  || rLCDIMFncEn.u1HueDisregardEn);
    u1OSDSCG = u1DrvLcdimDetectOSDSCG();
    
    u1DrvGetRGBHist(u2HistCur, RGB_HIST_NORM_BASE);
    u2RGBAverage = u2GetRGBAverage(u2HistCur, RGB_HIST_NORM_BASE);
    u1RGBCGFlat = u1CheckCGFlat(u2HistCur);
    u1RGBCGDist = u1CheckCGDist(u2HistCur);
    
    wAPLDiff = AbsDiff(u2RGBAverage, PreRGBAverage);
    bAPLUpFlg = (u2RGBAverage > PreRGBAverage)? SV_TRUE : SV_FALSE;
    vIO32WriteFldAlign(LCDIM_MAXINFO_SCG, u2RGBAverage,  LCDIM_SCG_MAXHIST_APL);
       
	for (bi = 0; bi < RGB_MAX_HIST_NUM; bi++)
	{
		wDiff = AbsDiff(u2HistCur[bi], u2HistRec[bi]);		
		wMaxDiff = (wMaxDiff < wDiff) ? wDiff : wMaxDiff;		
		wTotalDiff += wDiff;	
	}	   

    if(u1OSDSCG == OSD_APPEAR)
    {
        bCurSc = E_RGB_SCG_LEVEL_MID_DN;
        LOG(4, "Scene Change case OSD : PreSc %d  CurSc %d  Max %d  Total %d  APL diff %d \n", 
            bPreSc, bCurSc, wMaxDiff, wTotalDiff, wAPLDiff);
    }   
    else if(bAPLUpFlg && (bSCGCnt == 0) && (bSCGQue == 1))
    {
        bCurSc = E_RGB_SCG_LEVEL_HIGH;  
        LOG(4, "Scene Change case 1.2: PreSc %d  CurSc %d  Max %d  Total %d  APL diff %d \n", 
            bPreSc, bCurSc, wMaxDiff, wTotalDiff, wAPLDiff);   

        u1SCGflg = rLCDIMAGLCfg.u1AGLSCGCor;

        bSCGCnt = rLCDIMSpCtrlCfg.u1SCGStableCnt;   //keep n frame to prevent flicker

        bSCGQue = SV_FALSE;

    }
	else if ((wMaxDiff > rLCDIMSCGCfg.u2SCGRGBMaxDiffThd) && ((wTotalDiff >> 2) > rLCDIMSCGCfg.u2SCGRGBTotalDiffThd) && 
	    (wAPLDiff > rLCDIMSCGCfg.u2SCGRGBAvgDiffThd)
//	    && bAPLUpFlg && (u1SCGStableCnt == 0))
        /*&& bAPLUpFlg*/ && (u1OSDSCG == NO_OSD)
//        && bDetectNotCG //if CG pattern, don't SCG
        )

	{		
//        if(bAPLUpFlg && (bSCGCnt == 0) && ((bDimTrend == SV_FALSE) || (bSCGQue == SV_TRUE)))
        if(bAPLUpFlg && (bSCGCnt == 0) && ((bDimTrend == SV_FALSE) && (bSCGQue == 0)))
        {
    		bCurSc = E_RGB_SCG_LEVEL_HIGH;	
    		LOG(4, "Scene Change case 1.0: PreSc %d  CurSc %d  Max %d  Total %d  APL diff %d  PxlCntPre %d PxlCnt %d\n", 
    	        bPreSc, bCurSc, wMaxDiff, wTotalDiff, wAPLDiff, u4HuePxlCntPre, u4HuePxlCnt);   
            u1SCGflg = rLCDIMAGLCfg.u1AGLSCGCor;        
            bSCGCnt = rLCDIMSpCtrlCfg.u1SCGStableCnt;   //keep 10 frame to prevent flicker

            if(bSCGQue>0)
                bSCGQue --;
        }
        else
        {
            bCurSc = E_RGB_SCG_LEVEL_MID_DN;   
            LOG(4, "Scene Change case 1.1 : PreSc %d  CurSc %d  Max %d  Total %d  APL diff %d PxlCntPre %d PxlCnt %d\n", 
                bPreSc, bCurSc, wMaxDiff, wTotalDiff, wAPLDiff, u4HuePxlCntPre, u4HuePxlCnt); 
            u1SCGflg = rLCDIMAGLCfg.u1AGLSCGCor;        

            if(bDimTrend == SV_TRUE)
            {
                bSCGQue = 4;
            }
            else
            {
                if(bSCGQue>0)
                    bSCGQue --;
            }
        }
	}
    else if((wMaxDiff < rLCDIMSCGCfg.u2SCGRGBMaxDiffThd2) || ((wTotalDiff >> 2) < rLCDIMSCGCfg.u2SCGRGBTotalDiffThd2) 
            || (wAPLDiff < rLCDIMSCGCfg.u2SCGRGBAvgDiffThd2))    
    {
        bCurSc = E_RGB_SCG_LEVEL_LOW;

        if(bSCGQue>0)
            bSCGQue --;
    }
    else
	{
        if(bAPLUpFlg)
        {
            bCurSc = E_RGB_SCG_LEVEL_MID_UP;   
            LOG(4, "Scene Change case 3 : PreSc %d  CurSc %d  Max %d  Total %d  APL diff %d \n", 
                bPreSc, bCurSc, wMaxDiff, wTotalDiff, wAPLDiff);
            
            if(bSCGQue>0)
                bSCGQue --;
        }
        else
        {
            bCurSc = E_RGB_SCG_LEVEL_MID_DN;   
            LOG(4, "Scene Change case 4 : PreSc %d  CurSc %d  Max %d  Total %d  APL diff %d \n", 
                bPreSc, bCurSc, wMaxDiff, wTotalDiff, wAPLDiff);

            if(bSCGQue>0)
                bSCGQue --;
        }
	}	
    if(u1SCGflg > 0)    //for adaptive gain limit
        u1SCGflg --;

    if(bSCGCnt > 0)
        bSCGCnt --;

//    if(((bDetectNotCG == SV_FALSE) && ((bCurSc == E_RGB_SCG_LEVEL_MID_UP) || (bCurSc == E_RGB_SCG_LEVEL_MID_DN)))
        if(//(bDetectNotCG == SV_FALSE) && 
          (((bCurSc == E_RGB_SCG_LEVEL_HIGH) && (rLCDIMFncEn.u1SCGSupportSCG == SV_FALSE)) 
          ||((bCurSc == E_RGB_SCG_LEVEL_MID_UP) && (rLCDIMFncEn.u1SCGSupportSCG == SV_TRUE)))
        && bAPLUpFlg && (u1OSDSCG == NO_OSD)
        && (rLCDIMFncEn.u1CGBoostEn == SV_TRUE))
    {
        u1CGSpeedUpflg = SV_TRUE;
        bCGBoostCnt = rLCDIMSpCtrlCfg.bCGBoostFrameCnt;
        LOG(4, "CG Boost triggerd\n");
        
    }
    else if((bCGBoostCnt > 0))
    {
        bCGBoostCnt--;
    }
    else
    {
        u1CGSpeedUpflg = SV_FALSE;
        bCGBoostCnt = 0;
    }

    
    if(rLCDIMDbgEn.u1LcdimDebug_SCG == SV_TRUE)
    {
		LOG(4, "Scene Change : PreSc %d  CurSc %d  Max %d  Total %d  APL diff %d \n", 
	        bPreSc, bCurSc, wMaxDiff, wTotalDiff, wAPLDiff);
    }

    bRet = (bCurSc == bPreSc) ? E_RGB_SCG_LEVEL_LOW : bCurSc;

	PreRGBAverage = u2RGBAverage;	
    u4HuePxlCntPre = u4HuePxlCnt;
	bPreSc = bRet;//bCurSc;
    x_memcpy(u2HistRec, u2HistCur, sizeof(UINT16)*RGB_MAX_HIST_NUM);

	return bRet;//bCurSc;
}

UINT8 u1DrvLcdimDetectFadeIn(void)
{
    static UINT8 u1DarkCnt = 0;
    static UINT8 u1IncCnt = 0;
    static UINT8 u1PreRGBAverage = 0;
    static UINT8 u1ResetCnt = 0;
    static UINT8 u1Coring = 0;
    if(u2RGBAverage < 5)
    {
        u1DarkCnt ++;
    }
    else
    {
        u1DarkCnt = 0;
    }
    u1DarkCnt = CLIP(u1DarkCnt , 0, 0x80);

    if(u1Coring > 0)
        u1Coring--;

    if((u1DarkCnt >= rLCDIMSpCtrlCfg.u1FadeInDarkCnt) && (u1Coring == 0))
    {
        if((u2RGBAverage > u1PreRGBAverage) && (u1LcdimSCGStrength == E_RGB_SCG_LEVEL_LOW))
        {
            u1IncCnt ++;
        }
        else
        {
            u1ResetCnt ++;
        }
    }
    else
    {
        u1ResetCnt ++;
    }    
    
    if(u1ResetCnt >= 10)
    {
        u1DarkCnt = 0;        
        u1IncCnt = 0;
        u1ResetCnt = 0;
    }

    if(u1IncCnt >= rLCDIMSpCtrlCfg.u1FadeInIncCnt)
    {
        u1DarkCnt = 0;
        u1IncCnt = 0;
        u1Coring = rLCDIMAGLCfg.u1AGLSCGCor;
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
        
}

UINT8 u1DrvLcdimPreDetectRunIn(void)
{
    UINT8 u1APL = IO32ReadFldAlign(METER_INFO_00, METER_APL_VAL);


    UINT8 u1PredetectFlg = 0;
    static UINT8 u1PreDetectCnt = 0;
	static UINT8 bPreSc = SV_FALSE;
    static UINT8 u1PreAPL = 0;
    static UINT8 u1PreRGBAverage = 0;
    static UINT8 u1ResetCnt = 0;
    
    if((u1APL >= (rLCDIMSpCtrlCfg.u1PreDetectAPLThLow)) && (u2RGBAverage < rLCDIMSpCtrlCfg.u1PreDetectAPLThOstg)
        && (u1APL < (rLCDIMSpCtrlCfg.u1PreDetectAPLThHigh)) && (u4MotionConf >= rLCDIMSpCtrlCfg.u1PreDetectMotionTh)
        && (u1ASLVal == 0))
    {
        if((u1APL != u1PreAPL) || (u2RGBAverage != u1PreRGBAverage))   //prevent gray pattern
        {
            u1PreDetectCnt ++;
        }
        else
        {
            u1ResetCnt++;
        }

        if(u1ResetCnt >= 10)
        {
            u1PreDetectCnt = 0;
            u1ResetCnt = 0;
        }
        
        if(rLCDIMDbgEn.u1LcdimDebug_PreDetect == SV_TRUE)
        {
            LOG(4, "Pre Detect u1PreDetectCnt = %d, SCE APL = %d, OSTG APL =%d, Motion = %d\n", 
                u1PreDetectCnt, u1APL, u2RGBAverage, u4MotionConf);
        }
    }
    else
    {
        if(rLCDIMDbgEn.u1LcdimDebug_PreDetect == SV_TRUE)
        {
            LOG(4, "Pre Detect Fail, SCE APL = %d, OSTG APL =%d, Motion = %d\n", u1APL, u2RGBAverage, u4MotionConf);
        }
        u1PreDetectCnt = 0;
        u1ResetCnt = 0;        
    }

    if(u1PreDetectCnt>=rLCDIMSpCtrlCfg.u1PreDetectCntFrmTh)
    {
        u1PredetectFlg = 1;
        u1PreDetectCnt = 0;
    }

    u1PredetectFlg = ((bPreSc == SV_FALSE) && (u1PredetectFlg == SV_TRUE)) ? SV_TRUE : SV_FALSE;


    bPreSc = u1PredetectFlg;
    u1PreAPL = u1APL;
    u1PreRGBAverage= u2RGBAverage;

    return u1PredetectFlg;
}


BOOL bDrvLcdimDetectSceneChange(void)
{
	static UINT16 PreRGBAverage = 0;	
	static UINT8 bPreSc = SV_FALSE;
    static UINT16 u2HistCur[RGB_MAX_HIST_NUM];		
	static UINT16 u2HistRec[RGB_MAX_HIST_NUM];

	UINT8 bi, bCurSc, bRet;     
	UINT16 wMaxDiff = 0, wTotalDiff = 0, wDiff = 0;    

    u1DrvGetRGBHist(u2HistCur, RGB_HIST_NORM_BASE);
    u2RGBAverage = u2GetRGBAverage(u2HistCur, RGB_HIST_NORM_BASE);
    
	for (bi = 0; bi < RGB_MAX_HIST_NUM; bi++)
	{
		wDiff = AbsDiff(u2HistCur[bi], u2HistRec[bi]);		
		wMaxDiff = (wMaxDiff < wDiff) ? wDiff : wMaxDiff;		
		wTotalDiff += wDiff;	
	}	

	if ((wMaxDiff > rLCDIMSCGCfg.u2SCGRGBMaxDiffThd) && 
	   ((wTotalDiff >> 2) > rLCDIMSCGCfg.u2SCGRGBTotalDiffThd) && 
	    (AbsDiff(u2RGBAverage, PreRGBAverage) > rLCDIMSCGCfg.u2SCGRGBAvgDiffThd)
	    && (u2RGBAverage >= PreRGBAverage))	
	{		
		bCurSc = SV_TRUE;	
		LOG(4, "Scene Change case 1: PreSc %d  Max %d  Total %d  APL diff %d \n", 
	        bPreSc, wMaxDiff, wTotalDiff, AbsDiff(u2RGBAverage, PreRGBAverage));
	}
    else if((wMaxDiff > (rLCDIMSCGCfg.u2SCGRGBMaxDiffThd * 2)) && 
	   ((wTotalDiff >> 2) > (rLCDIMSCGCfg.u2SCGRGBTotalDiffThd * 2))
	   && (u2RGBAverage >= PreRGBAverage))	
    {       
        bCurSc = SV_TRUE;   
        LOG(4, "Scene Change case 2 : PreSc %d  Max %d  Total %d  APL diff %d \n", 
            bPreSc, wMaxDiff, wTotalDiff, AbsDiff(u2RGBAverage, PreRGBAverage));
    }
    else
	{
		bCurSc = SV_FALSE;	
	}	
    
    if(rLCDIMDbgEn.u1LcdimDebug_SCG == SV_TRUE)
    {
		LOG(4, "Scene Change : PreSc %d  Max %d  Total %d  APL diff %d \n", 
	        bPreSc, wMaxDiff, wTotalDiff, AbsDiff(u2RGBAverage, PreRGBAverage));
    }

    bRet = ((bPreSc == SV_FALSE) && (bCurSc == SV_TRUE)) ? SV_TRUE : SV_FALSE;

	PreRGBAverage = u2RGBAverage;	
	bPreSc = bCurSc;
    x_memcpy(u2HistRec, u2HistCur, sizeof(UINT16)*RGB_MAX_HIST_NUM);

	return bRet;
}

void vDrvLcdimForceBlockDimOnOff(UINT8 bOnOff)
{
    // force set specific block dimming
    u1ForceBlockDimOnOff = bOnOff;
}

void vDrvLcdimForceBlockDimCtrl(UINT8 u1PosH, UINT8 u1PosV, UINT8 u1BlockOnOff, UINT8 u1ForceLevel)
{
    // this function used for debugging / measurement usage
    u1LcdimOutputDimming[u1PosV*u2BlockH + u1PosH] = (u1BlockOnOff == SV_ON) ? u1ForceLevel : 0;
}

void vDrvGet3DStatus(void)
{
    UINT8 bAPLL, bAPLR;
    UINT8 bASLL, bASLR;    

    if(rLCDIMFncEn.u13DDetectEn == SV_TRUE)
    {
        if((u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS) || (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TB)
            || (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_SBS))
        {
            bAPLR = IO32ReadFldAlign(METER_INFO_00+0x80, METER_APL_VAL);
            bAPLL = IO32ReadFldAlign(METER_INFO_00, METER_APL_VAL);
            bASLR = IO32ReadFldAlign(METER_INFO_00+0x80, METER_ASL_VAL);
            bASLL = u1ASLVal;            
            u1Lcdim3DStatusCtrl = 1;

            if( (((bAPLL>=(rLCDIMSpCtrlCfg.u13DDetectAPLThH-12)) && (bAPLL<(rLCDIMSpCtrlCfg.u13DDetectAPLThH+12)) && (bAPLR<=rLCDIMSpCtrlCfg.u13DDetectAPLThL)) 
                || ((bAPLL<=rLCDIMSpCtrlCfg.u13DDetectAPLThL) && (bAPLR>=(rLCDIMSpCtrlCfg.u13DDetectAPLThH-12))&& (bAPLR<(rLCDIMSpCtrlCfg.u13DDetectAPLThH+12)))) 
                && (u1RGBCGDist >= rLCDIMSpCtrlCfg.u1DistThreshold) 
                && (u4HuePxlCnt < (UINT32)(HUE_HIST_NORM_BASE>>8)))
                u1Lcdim3DStatus = 1;
            else if (((bASLL>=0x34) && (bASLL<0x40) && (bAPLR<=rLCDIMSpCtrlCfg.u13DDetectAPLThL)) 
                || ((bAPLL<=rLCDIMSpCtrlCfg.u13DDetectAPLThL) && (bASLR>=0x34)&& (bASLR<0x40)))
                u1Lcdim3DStatus = 1;                
            else
                u1Lcdim3DStatus = 0;
            
        }
        else
        {
            u1Lcdim3DStatus = 0;
            u1Lcdim3DStatusCtrl = 0;
        }
    }
    else
    {
        u1Lcdim3DStatus = 0;
        u1Lcdim3DStatusCtrl = 0;
    }
        vIO32WriteFldAlign(LCDIM_3D_DETECT, u1Lcdim3DStatus, ADAP_LCDIM_SBS_DETECT_STATUS);    
}

void vDrvGetLcDimDimmingResult(UINT8 *u1DimmingResult) 
{
    UINT16 u2TotalBlock = u2BlockV * u2BlockH;
    UINT32 i = 0;
    UINT32 u4DimResultSum = 0;
    UINT8 u1PrintCnt = 8;
    UINT32 u4FilterEnable = IO32ReadFldAlign(LCDIM_02, LCDIM_FILTER_EN);
    UINT32 u4FilterType = IO32ReadFldAlign(LCDIM_01, LCDIM_FILTER_TYPE);    
    UINT16 u2Buffer[LCDIM_MAX_BLK_NUM];
    

    static UINT16 u2DimmingResultRec[LCDIM_MAX_BLK_NUM];		
    static UINT32 u4LcDimCnt = 0;
    UINT8 u1DimmingResultInv[LCDIM_MAX_BLK_NUM];

    // Cobra local dimming V2 supports only up to 200 blocks
    if(u2TotalBlock > 200)
    {
        u2TotalBlock = 200;
    }
    
    if(u1ForceBlockDimOnOff)
    {
        x_memcpy(u1DimmingResult, u1LcdimOutputDimming, sizeof(UINT16) * u2BlockH * u2BlockV);   
    }
    else
    {
        if(rLCDIMFncEn.u1LcdimLightDebugEn == SV_TRUE)
        {
            for(i = 0; i < u2TotalBlock; i++)
            {
                u1DimmingResult[i] = (u1IO32Read1B(LCDIM_DEBUG_DIM_0_3 + i));
            }
        }
        else
        {
            if(IO32ReadFldAlign(LD2_MODE_09, C_LD_FORCE_DIM_EN) == SV_TRUE)
            {
                for(i = 0; i < u2TotalBlock; i++)
                {
                    u1DimmingResult[i] = (IO32ReadFldAlign(LD2_MODE_09, C_LD_FORCE_DIM_VALUE)>>2);
                    x_memcpy(u1BlkDimResult, u1DimmingResult, sizeof(UINT8)*LCDIM_MAX_BLK_NUM);                    
                }
            
            }
            else
            {
                if(IO32ReadFldAlign(LD2_DIM_RESULT_00, LD_DIM_READY) != SV_TRUE)
                {
                    x_memcpy(u1DimmingResult, u1BlkDimResult, sizeof(UINT8)*LCDIM_MAX_BLK_NUM); //update previous info
                    //LOG(0, "Local Dimming Retrieve Data Error.\n");
                }
                else
                {

                    if(IO32ReadFldAlign(LD2_MODE_33, C_LD_FW_DIM_EN) == SV_ON)
                    {
                        for (i = 0; i < u2TotalBlock; i++)
                        {
                            u1DimmingResult[i] = (UINT8)(u2LcdimDimResult[i]>>2);
                            u4DimResultSum += u1DimmingResult[i];                        
                        }
                    }
                    else
                    {
                        vDrvGetLcDimDimmingResultHW(u2Buffer);
                    
                        for (i = 0; i < u2TotalBlock; i++)
                        {
                            u1DimmingResult[i] = (UINT8)(u2Buffer[i]>>2);
                            u4DimResultSum += u1DimmingResult[i];
                        }
                        
                        u4DimResultAvg = u4DimResultSum / u2TotalBlock;

                    }


                    if(rLCDIMFncEn.u1AdvancePowerSaveEn == SV_TRUE)
                    {
                        for (i = 0; i < u2TotalBlock; i++)
                        {
                            u1DimmingResult[i] = (u1DimmingResult[i] * u2AdvPSRatio + 128 ) >> 8;
                        }
                    }

                    //Apply temporal SPI IIR
                    if (u4FilterEnable || bSCGAdapSpiIIRWeight)
                    {
                        for (i = 0; i < u2TotalBlock; i++)
                        {
                            switch(u4FilterType)
                            {
                                case 1:
                                    u2DimmingResultRec[i] = ((u2DimmingResultRec[i] * 1 + 1) >> 1) + u1DimmingResult[i];
                                    u1DimmingResult[i] = (u2DimmingResultRec[i] + 1) >> 1;
                                break;
                                case 2:
                                    u2DimmingResultRec[i] = ((u2DimmingResultRec[i] * 3 + 2) >> 2) + u1DimmingResult[i];
                                    u1DimmingResult[i] = (u2DimmingResultRec[i] + 2) >> 2;
                                break;
                                case 3:
                                    u2DimmingResultRec[i] = ((u2DimmingResultRec[i] * 7 + 4) >> 3) + u1DimmingResult[i];
                                    u1DimmingResult[i] = (u2DimmingResultRec[i] + 4) >> 3;
                                break;
                                case 4:
                                    u2DimmingResultRec[i] = ((u2DimmingResultRec[i] * 15 + 8) >> 4) + u1DimmingResult[i];
                                    u1DimmingResult[i] = (u2DimmingResultRec[i] + 8) >> 4;
                                break;
                                case 5:
                                    u2DimmingResultRec[i] = ((u2DimmingResultRec[i] * 31 + 16) >> 5) + u1DimmingResult[i];
                                    u1DimmingResult[i] = (u2DimmingResultRec[i] + 16) >> 5;
                                break;
                                case 6:
                                    u2DimmingResultRec[i] = ((u2DimmingResultRec[i] * 63 + 32) >> 6) + u1DimmingResult[i];
                                    u1DimmingResult[i] = (u2DimmingResultRec[i] + 32) >> 6;
                                break;
                                case 7: //for scene change
                                    if(rLCDIMFncEn.u1SCGAdapSpiIIRCtrlEn)
                                    {
                                        u2DimmingResultRec[i] = ((u2DimmingResultRec[i]*bSCGAdapSpiIIRWeight + 32) >> 6) + u1DimmingResult[i]*(64 - bSCGAdapSpiIIRWeight);
                                        u1DimmingResult[i] = (u2DimmingResultRec[i] + 32) >> 6;
                                    }
                                break;

                                default:
                                    u2DimmingResultRec[i] = ((u2DimmingResultRec[i] * 7 + 4) >> 3) + u1DimmingResult[i];
                                    u1DimmingResult[i] = (u2DimmingResultRec[i] + 4) >> 3;
                                break;      
                            }
                        }
                        
                        if(rLCDIMFncEn.u1SCGAdapSpiIIRCtrlEn)
                        {
                            bSCGAdapSpiIIRWeight = CLIP(bSCGAdapSpiIIRWeight, 1, 64);
                            bSCGAdapSpiIIRWeight --;                            
                        }                        
//                            x_memcpy(u1DimmingResult, u2DimmingResultRec, sizeof(UINT8)*LCDIM_MAX_BLK_NUM);                        
                    }

                    x_memcpy(u1BlkDimResult, u1DimmingResult, sizeof(UINT8)*LCDIM_MAX_BLK_NUM);                    

                    if (IO32ReadFldAlign(LCDIM_02, LCDIM_SPI_INV))
                    {
                        for (i = 0; i < u2TotalBlock; i++)
                        {
                            u1DimmingResultInv[i] = u1DimmingResult[(u2TotalBlock -1) - i];
                        }
                        for (i = 0; i < u2TotalBlock; i++)
                        {
                            u1DimmingResult[i] = u1DimmingResultInv[i];
                        }                        
                    }

                    if (IO32ReadFldAlign(LCDIM_02, LCDIM_FILTER_LOG))
                    {                          
                        if (PANEL_IsSupport120Hz() == SV_FALSE)    
                            u1PrintCnt = 8;
                        else
                            u1PrintCnt = 16;
                        
                        if (u4LcDimCnt % u1PrintCnt == 0)
                            {
                            Printf("SPI DATA = ");
                                for (i = 0; i < u2TotalBlock; i++)
                                {
                                    Printf("%d ", u1DimmingResult[i]);
                                }
                                Printf("\n");
                                
                            if (u4FilterEnable || bSCGAdapSpiIIRWeight)
                            {
                                Printf("Record = ");
                                for (i = 0; i < u2TotalBlock; i++)
                                {
                                    Printf("%d ", u2DimmingResultRec[i]);
                                }
                                Printf("\n");
                                    Printf("IIRWeight = %d \n", bSCGAdapSpiIIRWeight);
                                }   
                            }
                        
                            u4LcDimCnt ++;
                    }                        
                }
            }
        }
    }
}

UINT8 bDrvLcDimSetSWCurve(UINT16 *u2InterpolatedCurve, UINT32 u4SWAddrBase, UINT8 u1Index)
{
    UINT8 u1LcdimSWCtrlPt[SW_CTRL_PT_NUM];
    UINT8 CtrlPt = 0;
    UINT32 i;


    // 1. read out control point from sw register
    for(CtrlPt = 0; CtrlPt < SW_CTRL_PT_NUM; CtrlPt++)
    {
        u1LcdimSWCtrlPt[CtrlPt] = u1IO32Read1B(u4SWAddrBase+CtrlPt);
    }

    // 2. compare read-out with exist one, if the same, do nothing, else re-interpolate
    // 3. Mapping 17 to 33 point
    for(i = 0; i < SW_CTRL_PT_NUM; i++)
    {
        u2InterpolatedCurve[i] = ((UINT16)u1LcdimSWCtrlPt[i])<<2; //8 to 10 bits
    }

    x_memcpy(&u1LcdimSWCtrlPtPre[u1Index][0], u1LcdimSWCtrlPt, SW_CTRL_PT_NUM); 
    return SV_TRUE;    
}

UINT8 bDrvLcDimSetSWDSCurve(UINT32 u4SWAddrBase)
{
    UINT8 u1LcdimSWCtrlPt[SW_CTRL_PT_NUM];
    UINT8 CtrlPt = 0;
    UINT32 i;

    // 1. read out control point from sw register
    for(CtrlPt = 0; CtrlPt < SW_CTRL_PT_NUM; CtrlPt++)
    {
        u1LcdimSWCtrlPt[CtrlPt] = u1IO32Read1B(u4SWAddrBase+CtrlPt);
    }

    // 2. compare read-out with exist one, if the same, do nothing, else re-interpolate
    // 3. Mapping 17 to 33 point
    for(i = 0; i < SW_CTRL_PT_NUM; i++)
    {
        u2LcdimGainTable[i] = ((UINT16)u1LcdimSWCtrlPt[i])<<4; //8 to 12bits
    }

    vDrvLcdimWriteGainTable(u2LcdimGainTable);

    return SV_TRUE;    
}

UINT8 bDrvLcDimGetSWDSCurve(UINT32 u4SWAddrBase)
{
    UINT16 u2LcdimGainTblTemp[LCDIM_GAIN_NUM];
	UINT16 i = 0;
    
    vDrvLcdimReadGainTable(u2LcdimGainTblTemp);

    // Init SW Dimming Curve
    for(i = 0; i < SW_CTRL_PT_NUM; i++)
    {       
        vIO32Write1B(u4SWAddrBase+i, (UINT8)(u2LcdimGainTblTemp[i]>>4));       
    }
    

    return SV_TRUE;    
}


UINT32 u4QueAverage(UINT32 u4Value, UINT8 bItem, UINT32 u4QueSize)
{
	UINT32 bi;
	UINT32 u4Average=0;

    if(u4QueSize == 0)
    {
        u4QueSize = 1;
    }

	if (u4QueCounter[bItem] == 0)
	{
		u4QueCounter[bItem]++;
		u4DataQue[bItem][0] = u4Value;
		return u4Value;
	}

	if (u4QueCounter[bItem] < u4QueSize)
	{
		u4QueCounter[bItem]++;
	}
	else
	{
        u4QueCounter[bItem] = u4QueSize;
	}

	for (bi = u4QueCounter[bItem] - 1; bi > 0 ; bi--)
	{
		u4DataQue[bItem][bi] = u4DataQue[bItem][bi - 1];
	}

	// Insert New Item
	u4DataQue[bItem][0] = u4Value;

	// Average Running Que
	for (bi = 0; bi < u4QueCounter[bItem]; bi++)
	{
		u4Average += u4DataQue[bItem][bi];
	}

	u4Average = u4Average / u4QueCounter[bItem];

	return u4Average;
}

void vDrvClearDataQue(void)
{
    UINT8 i;

    for(i = 0; i < E_LCDIM_ITEM_MAX; i++)
    {
    	u4QueCounter[i] = 0;
    }
}

void vDrvAdaptiveLcDimUpdateParam(void)
{    
    UINT8 bi = 0;
    UINT16 u2TotalBlock = u2BlockV * u2BlockH;

//Algorithm function enable
    rLCDIMFncEn.u1AdapCurveCtrlEn       = IO32ReadFldAlign(ADAP_LCDIM_00, ADAP_LD_CUR_CTRL);
    rLCDIMFncEn.u1SCGSpeedCtrlEn        = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_SPEED_CTRL_SCG_EN);
    rLCDIMFncEn.u1SCGAdapMinDimCtrlEn   = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_ADAP_MINDIM_EN);
    rLCDIMFncEn.u1SCGAGLCtrlEn          = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_ADAP_GAINLIMT_EN);
    rLCDIMFncEn.u1SCGAdapSpiIIRCtrlEn   = IO32ReadFldAlign(LCDIM_00, LCDIM_ADAP_SPI_IIR_EN);    
    rLCDIMFncEn.u1SCGSupportSCG         = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_SCG_EN);
    rLCDIMFncEn.u1LcdimLightDebugEn     = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_LIGHT_DEBUG_EN);
    rLCDIMFncEn.u1ForceScenceChange     = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_FORCE_SCG);
    rLCDIMFncEn.u1SCGAGLForceEn         = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_ADAP_GAINLIMT_FORCE);
    rLCDIMFncEn.u1AdvancePowerSaveEn    = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_ADVANCE_PS_EN);
    rLCDIMFncEn.u1HueDisregardEn        = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_SCG_HUE_DISREGARD);
    rLCDIMFncEn.u1CGBoostEn             = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_CG_BOOST_EN);
    rLCDIMFncEn.u1AGLatCGBoostEn        = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_CG_BOOST_AGL_EN);
    rLCDIMFncEn.u1OSDDetectEn           = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_OSD_DETECT_EN);
    rLCDIMFncEn.u1RFChgDetectEn         = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_RFCHG_DETECT_EN);
    rLCDIMFncEn.u13DDetectEn            = IO32ReadFldAlign(LCDIM_3D_DETECT, ADAP_LCDIM_SBS_DETECT_EN);
    rLCDIMFncEn.u1SCGSpeedCtrlVer       = (u2TotalBlock > LCDIM_SW_MODE_MAX_BLK_NUM)? LCDIM_SPCTRL_VER_0 : IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_SPCTRL_VER);
    
        

//Speed control setting
    rLCDIMSpCtrlCfg.u1MotionThd         = IO32ReadFldAlign(ADAP_LCDIM_02, LCDIM_NR_MOTION_THD);
    rLCDIMSpCtrlCfg.u1MotionPenaltyStep = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL3, LCDIM_SPCTRL_MOTION_PENALTY);
    rLCDIMSpCtrlCfg.u1GammaPenaltyStep  = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL3, LCDIM_SPCTRL_GAMMA_PENALTY);    
    rLCDIMSpCtrlCfg.u1MaxRGBConfStep[0] = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL2, LCDIM_SPCTRL_MAXRGB_STEP_LOW);
    rLCDIMSpCtrlCfg.u1MaxRGBConfStep[1] = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL2, LCDIM_SPCTRL_MAXRGB_STEP_MID_DN);
    rLCDIMSpCtrlCfg.u1MaxRGBConfStep[2] = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL2, LCDIM_SPCTRL_MAXRGB_STEP_MID_UP);
    rLCDIMSpCtrlCfg.u1MaxRGBConfStep[3] = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL2, LCDIM_SPCTRL_MAXRGB_STEP_HIGH);
    rLCDIMSpCtrlCfg.u1StaticStep        = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL3, LCDIM_SPCTRL_STATIC_STEP);
    rLCDIMSpCtrlCfg.u1FadeInStep        = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL11, LCDIM_FADEIN_STEP);

    rLCDIMSpCtrlCfg.u1ConfidenceTh[0]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL4, LCDIM_SPCTRL_CONF_TH0);
    rLCDIMSpCtrlCfg.u1ConfidenceTh[1]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL4, LCDIM_SPCTRL_CONF_TH1);
    rLCDIMSpCtrlCfg.u1ConfidenceTh[2]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL4, LCDIM_SPCTRL_CONF_TH2);
    rLCDIMSpCtrlCfg.u1ConfidenceTh[3]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL4, LCDIM_SPCTRL_CONF_TH3);
    rLCDIMSpCtrlCfg.u1ConfidenceThDft   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL5, LCDIM_SPCTRL_CONF_TH_DFT);    

    rLCDIMSpCtrlCfg.u1MaxUpDimStep[0]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL6, LCDIM_MAX_UP_DIM_STEP_HIGH);
    rLCDIMSpCtrlCfg.u1MaxUpDimStep[1]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL6, LCDIM_MAX_UP_DIM_STEP_MID_UP);
    rLCDIMSpCtrlCfg.u1MaxUpDimStep[2]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL6, LCDIM_MAX_UP_DIM_STEP_MID_DN);
    rLCDIMSpCtrlCfg.u1MaxUpDimStep[3]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL6, LCDIM_MAX_UP_DIM_STEP_LOW);    

    rLCDIMSpCtrlCfg.u1MaxDnDimStep[0]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL7, LCDIM_MAX_DN_DIM_STEP_HIGH);
    rLCDIMSpCtrlCfg.u1MaxDnDimStep[1]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL7, LCDIM_MAX_DN_DIM_STEP_MID_UP);
    rLCDIMSpCtrlCfg.u1MaxDnDimStep[2]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL7, LCDIM_MAX_DN_DIM_STEP_MID_DN);
    rLCDIMSpCtrlCfg.u1MaxDnDimStep[3]   = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL7, LCDIM_MAX_DN_DIM_STEP_LOW);
    
    rLCDIMSpCtrlCfg.u1DyDimSlope[0]     = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL8, LCDIM_DY_DIM_SLOPE_HIGH);
    rLCDIMSpCtrlCfg.u1DyDimSlope[1]     = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL8, LCDIM_DY_DIM_SLOPE_MID_UP);
    rLCDIMSpCtrlCfg.u1DyDimSlope[2]     = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL8, LCDIM_DY_DIM_SLOPE_MID_DN);
    rLCDIMSpCtrlCfg.u1DyDimSlope[3]     = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL8, LCDIM_DY_DIM_SLOPE_LOW);

    rLCDIMSpCtrlCfg.u1DyBoostStep[0]    = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL10, LCDIM_CG_BOOST_0);
    rLCDIMSpCtrlCfg.u1DyBoostStep[1]    = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL10, LCDIM_CG_BOOST_1);        
    rLCDIMSpCtrlCfg.u1DyBoostStep[2]    = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL10, LCDIM_CG_BOOST_2);
    rLCDIMSpCtrlCfg.bCGBoostFrameCnt    = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL10, LCDIM_CG_BOOST_FRAME_CNT);

    rLCDIMSpCtrlCfg.u1DimDowntrendTh    = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL11, LCDIM_DOWNTREND_DIM_TH);
    rLCDIMSpCtrlCfg.u1SCGStableCnt      = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL5, LCDIM_SPCTRL_SCG_WAIT_CNT);
    rLCDIMSpCtrlCfg.u1DistThreshold     = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL0, LCDIM_ADAP_DIM_DIST_TH);

    rLCDIMSpCtrlCfg.u1PreDetectAPLThLow = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL9, LCDIM_PREDETECT_APL_TH_LOW);
    rLCDIMSpCtrlCfg.u1PreDetectAPLThHigh= IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL9, LCDIM_PREDETECT_APL_TH_HIGH);
    rLCDIMSpCtrlCfg.u1PreDetectAPLThOstg= IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL10, LCDIM_PREDETECT_APL_TH_OSTG);    
    rLCDIMSpCtrlCfg.u1PreDetectMotionTh = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL10, LCDIM_PREDETECT_MOTION_TH);    
    rLCDIMSpCtrlCfg.u1PreDetectCntFrmTh = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL9, LCDIM_SPCTRL_PREDETECT_FRAME_TH);
    rLCDIMSpCtrlCfg.u1PredetectStep     = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL9, LCDIM_SPCTRL_PREDETECT_STEP);

    rLCDIMSpCtrlCfg.u1FadeInDarkCnt     = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL11, LCDIM_FADEIN_DARK_TH);
    rLCDIMSpCtrlCfg.u1FadeInIncCnt      = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL11, LCDIM_FADEIN_INC_TH);

    rLCDIMSpCtrlCfg.u13DDetectAPLThH    = IO32ReadFldAlign(LCDIM_3D_DETECT, ADAP_LCDIM_SBS_HIGH_TH);
    rLCDIMSpCtrlCfg.u13DDetectAPLThL    = IO32ReadFldAlign(LCDIM_3D_DETECT, ADAP_LCDIM_SBS_LOW_TH);

//Scene change threshold
    rLCDIMSCGCfg.u2SCGRGBMaxDiffThd     = IO32ReadFldAlign(LCDIM_SCENE_CHANGE0, LCDIM_SCG_RGB_MAX_DIFF_THD);
    rLCDIMSCGCfg.u2SCGRGBTotalDiffThd   = IO32ReadFldAlign(LCDIM_SCENE_CHANGE0, LCDIM_SCG_RGB_TOTAL_DIFF_THD);
    rLCDIMSCGCfg.u2SCGRGBAvgDiffThd     = IO32ReadFldAlign(LCDIM_SCENE_CHANGE1, LCDIM_SCG_RGB_AVG_DIFF_THD);    
    rLCDIMSCGCfg.u2SCGRGBMaxDiffThd2    = IO32ReadFldAlign(LCDIM_SCENE_CHANGE2, LCDIM_SCG_RGB_MAX_DIFF_THD2);
    rLCDIMSCGCfg.u2SCGRGBTotalDiffThd2  = IO32ReadFldAlign(LCDIM_SCENE_CHANGE2, LCDIM_SCG_RGB_TOTAL_DIFF_THD2);
    rLCDIMSCGCfg.u2SCGRGBAvgDiffThd2    = IO32ReadFldAlign(LCDIM_SCENE_CHANGE1, LCDIM_SCG_RGB_AVG_DIFF_THD2);

//Adaptive gain limit
    rLCDIMAGLCfg.u1AGLSCGCor            = IO32ReadFldAlign(ADAP_LCDIM_SPEED_CTRL5, LCDIM_SPCTRL_SCG_STABLE_TH);
    rLCDIMAGLCfg.u1AGLAPLThL            = IO32ReadFldAlign(ADAP_LCDIM_ADAP_GAIN_LMT_0, LCDIM_ADAP_GAIN_LMT_APLTH_L);
    rLCDIMAGLCfg.u1AGLAPLThH            = IO32ReadFldAlign(ADAP_LCDIM_ADAP_GAIN_LMT_0, LCDIM_ADAP_GAIN_LMT_APLTH_H);
    rLCDIMAGLCfg.u1AGLStep              = IO32ReadFldAlign(ADAP_LCDIM_ADAP_GAIN_LMT_0, LCDIM_ADAP_GAIN_LMT_CONF_STEP);
    rLCDIMAGLCfg.u1AGLCorCnt            = IO32ReadFldAlign(ADAP_LCDIM_ADAP_GAIN_LMT_0, LCDIM_ADAP_GAIN_LMT_COR);

//Debug Log
    rLCDIMDbgEn.u1LcdimDebug_SCG        = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_SCG_DEBUG_EN);
    rLCDIMDbgEn.u1LcdimDebug_SpCtrl     = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_SPEED_CTRL_DEBUG_EN);

    rLCDIMDbgEn.u1LcdimDebug_PreDetect  = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_PREDETECT_DEBUG);
    rLCDIMDbgEn.u1LcdimDebug_GainLimit  = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_ADAP_GAINLIMT_DEBUG);
    rLCDIMDbgEn.u1LcdimDebug_Downtrend  = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_DOWNTREND_DEBUG);
    rLCDIMDbgEn.u1LcdimDebug_PostInfo   = IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_POSTINFO_DEBUG);

//Adaptive dimming curve
    bDrvLcDimSetSWCurve(u2LcdimDimTable_DFT, LCDIM_SW_CTRL_PT_DFT_0, 0);
    bDrvLcDimSetSWCurve(u2LcdimDimTable_STR, LCDIM_SW_CTRL_PT_STR_0, 1);         

//Data stretch
#if 0
    if(IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_UPDATE_DS_CURVE) == SV_TRUE)
    {
        bDrvLcDimSetSWDSCurve(LCDIM_SW_DS_CTRL_PT_0_3);
        vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 0, LCDIM_UPDATE_DS_CURVE);
    }
    
    if(IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_GET_DS_CURVE) == SV_TRUE)
    {
        bDrvLcDimGetSWDSCurve(LCDIM_SW_DS_CTRL_PT_0_3);
        vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 0, LCDIM_GET_DS_CURVE);
    }
#endif
//Gain limit curve
    if(IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_UPDATE_GL_CURVE) == SV_TRUE)
    {
        for (bi = 0; bi < LCDIM_GAIN_LIMIT_NUM; bi++)
        {
            u2LcdimGainLimitTable_UP_BND[bi] = u2IO32Read2B(ADAP_LCDIM_ADAP_GAIN_LMT_H_0_1+2*bi);
            u2LcdimGainLimitTable_LOW_BND[bi] = u2IO32Read2B(ADAP_LCDIM_ADAP_GAIN_LMT_L_0_1+2*bi);            
        }   
        bSCGAdapGainLimitUpdate = SV_TRUE;
        vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, 0, LCDIM_UPDATE_GL_CURVE);        
    } 
}

void vDrvLcDimCreateAdaptiveCurve(UINT16 *u2LcdimCurveDft, UINT16 *u2LcdimCurveStr, UINT32 u4Confidence)
{
    UINT32 i;

    for(i = 0; i < LCDIM_DIMMING_NUM; i++)
    {
        u2LcdimCurveDft[i] = (UINT16)((((0x80 - u4Confidence) * (UINT32)u2LcdimCurveStr[i]) + (u4Confidence * (UINT32)u2LcdimCurveDft[i])) >> 7);
    }
}

void vDrvAdaptiveCurveRemapping(UINT16 *u2LcdimCurveDft, UINT16 *u2LcdimCurveStr)
{
    UINT32 u4BlockConfidence = 0;

    static UINT8 bAdapCurveCtrlStatus = SV_OFF;    
        
    if(rLCDIMFncEn.u1AdapCurveCtrlEn == SV_TRUE)
    {
        u4BlockConfidence = (UINT32)(u2RGBAverage + 1)>>1; //normalize to 0x80
        u4BlockConfidence = u4QueAverage(u4BlockConfidence, 
                                         E_LCDIM_BLOCK_CONF,
                                         u1QueSizeForBlock);

        vDrvLcDimCreateAdaptiveCurve(u2LcdimCurveDft, u2LcdimCurveStr, u4BlockConfidence);

        vIO32WriteFldAlign(ADAP_LCDIM_04, u4BlockConfidence, LCDIM_BLACK_CONFIDENCE);
        bAdapCurveCtrlStatus = SV_ON;
    }
    else
    {
        // On --> Off
        if(bAdapCurveCtrlStatus == SV_ON)        
        {
            // TODO : Recover Job Here
            bAdapCurveCtrlStatus = SV_OFF;  
        }
    }
}    

void vDrvAdaptiveLcDim(UINT16 *u2LcdimCurveDft, UINT16 *u2LcdimCurveStr)
{ 
    static UINT8 bAdapCurveCtrlStatus = SV_OFF;    

    if(rLCDIMFncEn.u1AdapCurveCtrlEn == SV_TRUE)
    {        
        // Adaptive Curve Remapping
        vDrvAdaptiveCurveRemapping(u2LcdimCurveDft, u2LcdimCurveStr);
        vDrvLcdimWriteDimTable(u2LcdimCurveDft);    
        bAdapCurveCtrlStatus = SV_ON;        
    }
    else
    {
        // On --> Off
        if(bAdapCurveCtrlStatus == SV_ON)        
        {
            // TODO : Recover Job Here
            bAdapCurveCtrlStatus = SV_OFF;  
            vDrvLcdimWriteDimTable(u2LcdimCurveDft);
        }
    }  
}

UINT8 bDrvGetAdaptiveLcDimMotionCtrl(void)
{
    UINT8 bMotionStrength = 0;
        
    u4MotionConf = u4DrvDIGetMotionLevel();

    u4MotionConfFiltered = u4QueAverage(u4MotionConf, 
                            E_LCDIM_NM_MOTION_CONF,
                            u1QueSizeForBlock);    
    vIO32Write4B(ADAP_LCDIM_SPEED_CTRL1, u4MotionConfFiltered);

    if(u4MotionConfFiltered > (rLCDIMSpCtrlCfg.u1MotionThd * 4))
        bMotionStrength = E_MOTION_LEVEL_EXT;
    else if(u4MotionConfFiltered > (rLCDIMSpCtrlCfg.u1MotionThd * 3))
        bMotionStrength = E_MOTION_LEVEL_HIGH;
    else if(u4MotionConfFiltered > (rLCDIMSpCtrlCfg.u1MotionThd * 2))
        bMotionStrength = E_MOTION_LEVEL_MID;
    else if(u4MotionConfFiltered > rLCDIMSpCtrlCfg.u1MotionThd)
        bMotionStrength = E_MOTION_LEVEL_LOW;
    else    //static
        bMotionStrength = E_MOTION_LEVEL_ZERO;

    return bMotionStrength;
}

void vDrvGetAdaptiveLcDimSCGConf(void)
{
    UINT8 u1LcdimMotionStrength = 0;
    UINT8 u1LcdimGammaStrength = 0;
    UINT8 u1LcdimDarkPenalty = 0;
    UINT8 u1LcdimPredetectPenalty = 0;    
    UINT8 u1LcdimFadeInPenalty = 0;    
    
    u1LcdimSCGStrength = u1DrvLcdimDetectSCGLevel();

    u1LcdimMotionStrength = bDrvGetAdaptiveLcDimMotionCtrl();

    if(u1DrvLcdimPreDetectRunIn() == SV_TRUE)
        u1LcdimPredetectPenalty = rLCDIMSpCtrlCfg.u1PredetectStep;
    else
        u1LcdimPredetectPenalty = 0;
    

    if(u1DrvLcdimDetectFadeIn() == SV_TRUE)
        u1LcdimFadeInPenalty = rLCDIMSpCtrlCfg.u1FadeInStep;
    else
        u1LcdimFadeInPenalty = 0;


    if((u1ASLVal == 0) && (u1RGBCGDist >= rLCDIMSpCtrlCfg.u1DistThreshold))
        u1LcdimGammaStrength = rLCDIMSpCtrlCfg.u1GammaPenaltyStep;
    else
        u1LcdimGammaStrength = 0;
    
    if((u1LcdimMotionStrength == 0) || (u1LcdimSCGStrength != E_RGB_SCG_LEVEL_LOW))
    {
//        LcdimSCGConf = LcdimSCGConf + u1MaxRGBConfStep[u1LcdimSCGStrength] + u1LcdimGammaStrength - u1LcdimDarkPenalty;
        LcdimSCGConf = LcdimSCGConf + rLCDIMSpCtrlCfg.u1MaxRGBConfStep[u1LcdimSCGStrength] - u1LcdimDarkPenalty + u1LcdimPredetectPenalty
                       + u1LcdimFadeInPenalty; 
    }
    else
    {
        LcdimSCGConf = LcdimSCGConf + rLCDIMSpCtrlCfg.u1MaxRGBConfStep[u1LcdimSCGStrength] - u1LcdimMotionStrength * rLCDIMSpCtrlCfg.u1MotionPenaltyStep + u1LcdimGammaStrength
                    - u1LcdimDarkPenalty + u1LcdimPredetectPenalty + u1LcdimFadeInPenalty;
    }
        
    if(u1LcdimSCGStrength != E_RGB_SCG_LEVEL_HIGH)
    {
        LcdimSCGConf = CLIP(LcdimSCGConf, 0x00, rLCDIMSpCtrlCfg.u1ConfidenceTh[3]);
    }     
    
    if(LcdimSCGConf >= (rLCDIMSpCtrlCfg.u1ConfidenceThDft+rLCDIMSpCtrlCfg.u1StaticStep))
        LcdimSCGConf = LcdimSCGConf - rLCDIMSpCtrlCfg.u1StaticStep;
    else if(LcdimSCGConf < (rLCDIMSpCtrlCfg.u1ConfidenceThDft-rLCDIMSpCtrlCfg.u1StaticStep))
        LcdimSCGConf = LcdimSCGConf + rLCDIMSpCtrlCfg.u1StaticStep;            
    
    if(rLCDIMDbgEn.u1LcdimDebug_SpCtrl == SV_TRUE)
    {
		LOG(4, "SCGConf = %d  Motion = %d  MaxRGB = %d  Gamma(CGDist) = %d(%d)  Predetect = %d FadeIn = %d\n", 
            LcdimSCGConf, u1LcdimMotionStrength, rLCDIMSpCtrlCfg.u1MaxRGBConfStep[u1LcdimSCGStrength], u1LcdimGammaStrength, 
            u1RGBCGDist, u1LcdimPredetectPenalty, u1LcdimPredetectPenalty);           
    }

    LcdimSCGConf = CLIP(LcdimSCGConf, 0x00, 0xFF);

    vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL3, LcdimSCGConf, LCDIM_SPCTRL_CONFIDENCE);
}
        
void vDrvLcdimUnstableSignalProt(void)
{
    UINT16 u2LcDimMinDimLow = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL1, LCDIM_ADAP_DIM_MIN_LOW);
    UINT16 u2LcDimMinDimHigh = 0xFF;
    UINT32 u4LcDimMinDim = 0;
    static UINT8 u1RFChgDimConf;

    if(rLCDIMFncEn.u1RFChgDetectEn != SV_TRUE)
    {
        return;
    }

    if(bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE)
    {
        vIO32WriteFldAlign(LCDIM_3D_DETECT, 1, ADAP_LCDIM_RF_DETECT);  
        u1RFChgDimConf  = 255;        
    }
    else
    {
        vIO32WriteFldAlign(LCDIM_3D_DETECT, 0, ADAP_LCDIM_RF_DETECT);    
        if(u1RFChgDimConf>0)
        u1RFChgDimConf = u1RFChgDimConf-1;
    }        

    u4LcDimMinDim = CLIP((((u2LcDimMinDimHigh - u2LcDimMinDimLow) * u1RFChgDimConf + 128) >> 8) + u2LcDimMinDimLow, 0x00, 0xFF);        
    vIO32WriteFldAlign(LD2_MODE_05, (UINT16)u4LcDimMinDim<<2, C_LD_MIN_DIM);
}


void vDrvLcdimAdapMinDimCtrl(void)
{
    static UINT8 u1AdapDimConf;
    static UINT8 u1AdapDimConfCoring = 0;    
    static UINT16 u2AdapWeightConf = 0;   
    UINT8 u1APLThdLow = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL0, LCDIM_ADAP_DIM_APL_TH);
    UINT8 u1APLThdHigh = CLIP(u1APLThdLow * 2, 0, 0xFF);

    UINT8 u1DimWeight0_L = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, LCDIM_ADAP_DIM_WEIGHT0_L);
    UINT8 u1DimWeight1_L = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, LCDIM_ADAP_DIM_WEIGHT1_L);
    UINT8 u1DimWeight2_L = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, LCDIM_ADAP_DIM_WEIGHT2_L);
    UINT8 u1DimWeight3_L = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, LCDIM_ADAP_DIM_WEIGHT3_L);
    UINT8 u1DimWeight4_L = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL3, LCDIM_ADAP_DIM_WEIGHT4_L);
    UINT8 u1DimWeight0_H = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, LCDIM_ADAP_DIM_WEIGHT0_H);
    UINT8 u1DimWeight1_H = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, LCDIM_ADAP_DIM_WEIGHT1_H);
    UINT8 u1DimWeight2_H = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, LCDIM_ADAP_DIM_WEIGHT2_H);
    UINT8 u1DimWeight3_H = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, LCDIM_ADAP_DIM_WEIGHT3_H);
    UINT8 u1DimWeight4_H = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL5, LCDIM_ADAP_DIM_WEIGHT4_H);

    u1ASLVal = IO32ReadFldAlign(METER_INFO_00, METER_ASL_VAL);

    if((rLCDIMFncEn.u1SCGAdapMinDimCtrlEn == SV_FALSE))  //if scene change, disable adap spatial filter
    {
        rLCDIMSpCfg.u1DimWeight0 = u1DimWeight0_L;
        rLCDIMSpCfg.u1DimWeight1 = u1DimWeight1_L;
        rLCDIMSpCfg.u1DimWeight2 = u1DimWeight2_L;
        rLCDIMSpCfg.u1DimWeight3 = u1DimWeight3_L;
        rLCDIMSpCfg.u1DimWeight4 = u1DimWeight4_L;        

        vDrvLcdimWriteSpatialCfg(&rLCDIMSpCfg);
        u1AdapDimConf = 1;

        return;
    }

    if((u1ASLVal == 0) && (u4MotionConfFiltered <= rLCDIMSpCtrlCfg.u1MotionThd)
        && (u1RGBCGFlat == 1) && (u2RGBAverage < u1APLThdHigh)    
        && (u1AdapDimConfCoring == 0 ))
    {
        u1AdapDimConf ++;
    }
    else
    {
        u1AdapDimConfCoring --;
        u1AdapDimConf --;
    }
        u1AdapDimConf = CLIP(u1AdapDimConf, 0x01, 0x7E);

    if((u4MotionConfFiltered >=rLCDIMSpCtrlCfg.u1MotionThd*3) && (u1ASLVal == 0))
    {
        u2AdapWeightConf ++;        
    }
    else if((LcdimSCGConf > IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL1, LCDIM_ADAP_DIM_SPCONF_TH)) || (u1OSDSCG == OSD_APPEAR))
    {   
        u2AdapWeightConf = 0;
 
        u1AdapDimConf = 1;
        u1AdapDimConfCoring = IO32ReadFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL1, LCDIM_ADAP_DIM_CONF_COR);
    }
    else if(u1AdapDimConf > 0x20)
    {
        u2AdapWeightConf ++;       
    }
    else
    {
        if(u2AdapWeightConf > 0)
        u2AdapWeightConf --;        
 
    }

    u2AdapWeightConf = CLIP(u2AdapWeightConf, 0, 0x3FF);
    rLCDIMSpCfg.u1DimWeight0 = (UINT8)CLIP((((UINT32)u2AdapWeightConf * (u1DimWeight0_H - u1DimWeight0_L) + (1<<9)) >>10 ) + u1DimWeight0_L, 
                               u1DimWeight0_L, u1DimWeight0_H);
    rLCDIMSpCfg.u1DimWeight1 = (UINT8)CLIP((((UINT32)u2AdapWeightConf * (u1DimWeight1_H - u1DimWeight1_L) + (1<<9)) >>10 ) + u1DimWeight1_L, 
                               u1DimWeight1_L, u1DimWeight1_H);
    rLCDIMSpCfg.u1DimWeight2 = (UINT8)CLIP((((UINT32)u2AdapWeightConf * (u1DimWeight2_H - u1DimWeight2_L) + (1<<9)) >>10 ) + u1DimWeight2_L, 
                               u1DimWeight2_L, u1DimWeight2_H);
    rLCDIMSpCfg.u1DimWeight3 = (UINT8)CLIP((((UINT32)u2AdapWeightConf * (u1DimWeight3_H - u1DimWeight3_L) + (1<<9)) >>10 ) + u1DimWeight3_L, 
                               u1DimWeight3_L, u1DimWeight3_H);
    rLCDIMSpCfg.u1DimWeight4 = (UINT8)CLIP((((UINT32)u2AdapWeightConf * (u1DimWeight4_H - u1DimWeight4_L) + (1<<9)) >>10 ) + u1DimWeight4_L, 
                               u1DimWeight4_L, u1DimWeight4_H);


    vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL0, u1AdapDimConf, LCDIM_ADAP_DIM_CONF);
    
    vDrvLcdimWriteSpatialCfg(&rLCDIMSpCfg);
}

void vDrvLcdimAdapGainLimit(void)
{
    static UINT16 u2AdapGainLimitConf;
    static UINT8 u2AdapGainLimitCor;    
    UINT8 bi;
	UINT8 bBlending = 0;
	static UINT8 bBlendingPre = 0;    
	static UINT8 bSCGAdapGainLimitFlgPre = 0;        



    UINT8 bAPLFactor = 0;
	UINT32 u4Diff;

    if(rLCDIMFncEn.u1SCGAGLCtrlEn == SV_FALSE)
    {
        return;
    }

    if(u2RGBAverage < rLCDIMAGLCfg.u1AGLAPLThL)
        bAPLFactor = 0;
    else if(u2RGBAverage > rLCDIMAGLCfg.u1AGLAPLThH)
        bAPLFactor = 0xFF;
    else
        bAPLFactor = (u2RGBAverage - rLCDIMAGLCfg.u1AGLAPLThL) * 0xFF / (rLCDIMAGLCfg.u1AGLAPLThH - rLCDIMAGLCfg.u1AGLAPLThL);

    if(u1CGSpeedUpflg == SV_TRUE)
    {
        u1SCGflg = 0;
        u2AdapGainLimitConf = 0;
        bSCGAdapGainLimitSpeedUp = SV_TRUE;
    }

//detection
    if((u1LcdimSCGStrength == E_RGB_SCG_LEVEL_HIGH) && (rLCDIMFncEn.u1SCGSupportSCG == SV_TRUE)) //reset status if scene change
	{
        bSCGAdapGainLimitFlg = SV_FALSE;
        u2AdapGainLimitConf = 0;
        u2AdapGainLimitCor = 0;
    }
    else if(( (u1OSDSCG == OSD_APPEAR) 
        || ((u1CGSpeedUpflg == SV_TRUE) && (bDetectNotCG == SV_FALSE) && (rLCDIMFncEn.u1AGLatCGBoostEn == SV_TRUE))
        || u1Lcdim3DStatus
        || rLCDIMFncEn.u1SCGAGLForceEn)
        && (u1SCGflg == 0))
/*        
    else if((u1LcdimSCGStrength >= E_RGB_SCG_LEVEL_MID_DN) && 
    ((u4HuePxlCnt < (UINT32)(HUE_HIST_NORM_BASE>>2))  || IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_ADAP_GAINLIMT_HUE_DET)) &&
        (u1SCGflg == 0)    )
*/        
    {
        bSCGAdapGainLimitFlg = SV_TRUE;
        u2AdapGainLimitConf = 0x3FF;
        u2AdapGainLimitCor = rLCDIMAGLCfg.u1AGLCorCnt * 4;
        LOG(4, "Adaptive GL start\n");
    }
    else if(u2AdapGainLimitConf >= 1 && (u1SCGflg == 0))
    {
        if(u2AdapGainLimitCor == 0 )
        {
            if(u2AdapGainLimitConf > rLCDIMAGLCfg.u1AGLStep)
                u2AdapGainLimitConf = u2AdapGainLimitConf - rLCDIMAGLCfg.u1AGLStep;
            else
                u2AdapGainLimitConf = 0;
		}
		else
		{
            u2AdapGainLimitCor --;
        }
    }
    else
	{
        bSCGAdapGainLimitFlg = SV_FALSE;
        u2AdapGainLimitConf = 0;
	}

    bBlending = (UINT8)CLIP((   ( (UINT32)u2AdapGainLimitConf * (0xFF - bAPLFactor)) >>10 ), 0, 255);

//interpolation

    if ((bBlending != bBlendingPre) || (bSCGAdapGainLimitFlg != bSCGAdapGainLimitFlgPre))
        bSCGAdapGainLimitUpdate = SV_TRUE;


    
    if(rLCDIMDbgEn.u1LcdimDebug_GainLimit == SV_TRUE)
    {
        LOG(4, "u2AdapGainLimitConf = %d  bBlending = %d, bAPLFactor = %d , u4HuePxlCnt= %d\n", 
            u2AdapGainLimitConf, bBlending, bAPLFactor, u4HuePxlCnt);           
	}        

    
    if(bSCGAdapGainLimitFlg)
    {
        for (bi = 0; bi < LCDIM_GAIN_LIMIT_NUM; bi++)
        {
            u4Diff = AbsDiff(u2LcdimGainLimitTable_UP_BND[bi],u2LcdimGainLimitTable_LOW_BND[bi]);
            u2LcdimGainLimitTable_Adap[bi] = u2LcdimGainLimitTable_UP_BND[bi] - (UINT16)((u4Diff * bBlending + (1<<7)) >>8);
        }
    }
    else
    {
        for (bi = 0; bi < LCDIM_GAIN_LIMIT_NUM; bi++)
        {
            u2LcdimGainLimitTable_Adap[bi] = u2LcdimGainLimitTable_UP_BND[bi];
        }
    }

    bBlendingPre = bBlending;
    bSCGAdapGainLimitFlgPre = bSCGAdapGainLimitFlg;

}

void vDrvLcdimSCGSpeedCtrlMainLoop(void)
{
    vDrvLcdimAdapMinDimCtrl();
    vDrvGet3DStatus();    
    vDrvLcdimAdapGainLimit();
}


void vDrvLcdimSCGSpeedCtrl(void)
{
    vDrvGetAdaptiveLcDimSCGConf();
    vDrvLcdimUnstableSignalProt();   
/*
    vDrvLcdimAdapMinDimCtrl();
    vDrvGet3DStatus();    
    vDrvLcdimAdapGainLimit();
    */
    if(LcdimSCGConf > rLCDIMSpCtrlCfg.u1ConfidenceTh[3])
    {
        if(rLCDIMFncEn.u1SCGSupportSCG == SV_TRUE)  
        {
            vDrvLcdimResetTemporalFilter(SV_ON);
            vDrvClearDataQue();
            LcdimSCGConf = LcdimSCGConf - rLCDIMSpCtrlCfg.u1MaxRGBConfStep[3];
            LOG(4, "LCDIM scene change2\n");

            if(rLCDIMFncEn.u1SCGAGLCtrlEn) 
            {
                if(bSCGAdapGainLimitUpdate)
                {
                    vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_UP_BND);
                    bSCGAdapGainLimitUpdate = 0;       
                }
     
            }        
            if(rLCDIMFncEn.u1SCGAdapSpiIIRCtrlEn)
                bSCGAdapSpiIIRWeight = 63;
            else
                bSCGAdapSpiIIRWeight = 0;
        }
        else
        {
            LcdimSCGConf = rLCDIMSpCtrlCfg.u1ConfidenceTh[3] - 1;

            vDrvLcdimResetTemporalFilter(SV_OFF);
            vDrvClearDataQue();            
            if(rLCDIMFncEn.u1SCGAGLCtrlEn && bSCGAdapGainLimitFlg) 
            {
                if(bSCGAdapGainLimitUpdate)
                {
                    vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_Adap);
                    bSCGAdapGainLimitUpdate = 0;        
                }
                if(bSCGAdapGainLimitSpeedUp)
                {
                    rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[3];
                    rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[3];
                    rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[3];

                    if(u1CGSpeedUpflg == SV_TRUE)
                    {
                        rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[2];
                    }
                    else
                    {
                        rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];
                    }
                    vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg);


                    bSCGAdapGainLimitSpeedUp = 0;
                }            
            }
            else
            {
                rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[3];
                rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[3];
                rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[3];                
                if(u1CGSpeedUpflg == SV_TRUE)
                {
                    rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[2];                    
                }
                else
                {
                    rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];                    
                }              
                vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg);

                if(bSCGAdapGainLimitUpdate)
                {
                    vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_UP_BND);
                    bSCGAdapGainLimitUpdate = 0;  
                }            
            }
        }
    }
    else if(LcdimSCGConf > rLCDIMSpCtrlCfg.u1ConfidenceTh[2])
    {
        vDrvLcdimResetTemporalFilter(SV_OFF);
        vDrvClearDataQue();

        if(rLCDIMFncEn.u1SCGAGLCtrlEn && bSCGAdapGainLimitFlg) 
        {
            if(bSCGAdapGainLimitUpdate)
            {
                vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_Adap);
                bSCGAdapGainLimitUpdate = 0;        
            }
            if(bSCGAdapGainLimitSpeedUp)
            {
                rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[3];
                rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[3];
                rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[3];   

                if((u1CGSpeedUpflg == SV_TRUE) ||(LcdimSCGConf > ((rLCDIMSpCtrlCfg.u1ConfidenceTh[2] +rLCDIMSpCtrlCfg.u1ConfidenceTh[3])/2)))
                {
                    rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[1];                        
                }
                else
                {
                    rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];                        
                }
                vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg);

                bSCGAdapGainLimitSpeedUp = 0;
            }            
        }
        else
        {
            rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[3];
            rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[3];
            rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[3];           
            
            if((u1CGSpeedUpflg == SV_TRUE) ||(LcdimSCGConf > ((rLCDIMSpCtrlCfg.u1ConfidenceTh[2] +rLCDIMSpCtrlCfg.u1ConfidenceTh[3])/2)))
            {
                rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[1];                
            }
            else
            {
                rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];                        
            }
            vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg); 
            
            if(bSCGAdapGainLimitUpdate)
            {
                vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_UP_BND);
                bSCGAdapGainLimitUpdate = 0;  
            }
        }

    }
    else if(LcdimSCGConf > rLCDIMSpCtrlCfg.u1ConfidenceTh[1])
    {
        vDrvLcdimResetTemporalFilter(SV_OFF);

        if(rLCDIMFncEn.u1SCGAGLCtrlEn && bSCGAdapGainLimitFlg) 
        {
            if(bSCGAdapGainLimitUpdate)
            {
                vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_Adap);
                bSCGAdapGainLimitUpdate = 0;                
            }
            if(bSCGAdapGainLimitSpeedUp)
            {
                rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[2];
                rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[2];
                rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[2];                   
                
                if(u1CGSpeedUpflg == SV_TRUE)
                {
                    rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[1];
                }
                else
                {
                    rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];
                }
                vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg); 
                
                bSCGAdapGainLimitSpeedUp = 0;
            }
        }      
        else
        {
            rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[2];
            rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[2];
            rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[2];     

            if(u1CGSpeedUpflg == SV_TRUE)
            {
                rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[1];
            }
            else
            {
                rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];
            }              
            vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg); 
            
            if(bSCGAdapGainLimitUpdate)
            {
                vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_UP_BND);
                bSCGAdapGainLimitUpdate = 0;               
            }            
        }
    }    
    else if(LcdimSCGConf > rLCDIMSpCtrlCfg.u1ConfidenceTh[0])
    {
        vDrvLcdimResetTemporalFilter(SV_OFF);
    
        if(rLCDIMFncEn.u1SCGAGLCtrlEn && bSCGAdapGainLimitFlg) 
        {
            if(bSCGAdapGainLimitUpdate)
            {
                vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_Adap);
                bSCGAdapGainLimitUpdate = 0;        
            }
            if(bSCGAdapGainLimitSpeedUp)
            {
                rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[1];
                rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[1];
                rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[1];                   
                rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];
                vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg);                 
                
                bSCGAdapGainLimitSpeedUp = 0;                
            }
        }
        else
        {
            rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[1];
            rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[1];
            rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[1];                   
            rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];
            vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg);                
            
            if(bSCGAdapGainLimitUpdate)
            {
                vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_UP_BND);
                bSCGAdapGainLimitUpdate = 0;               
            }
        }
    } 
    else
    {
        vDrvLcdimResetTemporalFilter(SV_OFF);

        if(rLCDIMFncEn.u1SCGAGLCtrlEn && bSCGAdapGainLimitFlg) 
        {
            if(bSCGAdapGainLimitUpdate)
            {    
                vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_Adap);
                bSCGAdapGainLimitUpdate = 0;                
            }
        }    
        
        else if(rLCDIMFncEn.u1SCGAGLCtrlEn && (bSCGAdapGainLimitFlg == SV_FALSE)) 
        {
            if(bSCGAdapGainLimitUpdate)
            {                       
                vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_UP_BND);
                bSCGAdapGainLimitUpdate = 0;                
            }
        }    
        rLCDIMTplCfg.u1UpDimMax = rLCDIMSpCtrlCfg.u1MaxUpDimStep[0];
        rLCDIMTplCfg.u1DnDimMax = rLCDIMSpCtrlCfg.u1MaxDnDimStep[0];
        rLCDIMTplCfg.u1DimSlope = rLCDIMSpCtrlCfg.u1DyDimSlope[0];                   
        rLCDIMTplCfg.u1DimStepSft = rLCDIMSpCtrlCfg.u1DyBoostStep[0];
        vDrvLcdimWriteTemporalCfg(&rLCDIMTplCfg);    

    }
}

void vDrvLcDimAdapiveIIR(UINT16 *u2DimmingResult)
{
    UINT8 i;
    UINT16 u2TotalBlock = u2BlockV * u2BlockH;
    UINT16 u2LcDimPreWeight = 0;    
    UINT16 u2LcDimCurWeight = IO32ReadFldAlign(LCDIM_TEMP_IIR_0, ADAP_LCDIM_IIR_CUR_W);
    UINT16 u2LcDimDiffGainMax  = IO32ReadFldAlign(LCDIM_TEMP_IIR_1, ADAP_LCDIM_IIR_DIFF_GAIN_MAX);
    UINT16 u2LcDimDiffGainMin  = IO32ReadFldAlign(LCDIM_TEMP_IIR_1, ADAP_LCDIM_IIR_DIFF_GAIN_MIN);
    UINT16 u2LcDimCurWMin   = IO32ReadFldAlign(LCDIM_TEMP_IIR_0, ADAP_LCDIM_IIR_CUR_W_MIN);

    static UINT32 u4DimBufferPre[LCDIM_SW_MODE_MAX_BLK_NUM];
    static UINT16 u2DimBufferResult[LCDIM_SW_MODE_MAX_BLK_NUM];
    static UINT16 _u2LcDimDiffGain  = 0;

    u4MotionConf = u4DrvDIGetMotionLevel();

    u4MotionConfFiltered = u4QueAverage(u4MotionConf, 
                            E_LCDIM_NM_MOTION_CONF,
                            u1QueSizeForBlock);    

//1. check block number

    if(u2TotalBlock > LCDIM_SW_MODE_MAX_BLK_NUM)
    {
        LOG(2,"Not support block size more than 24\n");
        return;
    }


//2. change IIR filter adaptively
    u2LcDimCurWeight = CLIP(u2LcDimCurWeight, 0, 0x80);
    u2LcDimPreWeight = 0x80 - u2LcDimCurWeight;

    if(u4MotionConfFiltered > rLCDIMSpCtrlCfg.u1MotionThd)
    {
        _u2LcDimDiffGain = MAX(MIN((_u2LcDimDiffGain - 1), u2LcDimDiffGainMax),u2LcDimDiffGainMin);
    }
    else
    {
        _u2LcDimDiffGain = MAX(MIN((_u2LcDimDiffGain + 1), u2LcDimDiffGainMax),u2LcDimDiffGainMin);
    }
    vIO32WriteFldAlign(LCDIM_TEMP_IIR_1, _u2LcDimDiffGain, ADAP_LCDIM_IIR_DIFF_GAIN);

//3. apply IIR
    if(IO32ReadFldAlign(LCDIM_TEMP_IIR_0, ADAP_LCDIM_IIR_FIX_W_EN) == SV_TRUE)
    {
    for(i = 0; i < u2TotalBlock; i++)
    {
            u4DimBufferPre[i] = ((u4DimBufferPre[i] + 64)>>7) * u2LcDimPreWeight + (UINT32)u2DimmingResult[i] * u2LcDimCurWeight;
            u2DimBufferResult[i] = (UINT16)((u4DimBufferPre[i] + 64)>>7);
        }
    }
    else
    {
        for(i = 0; i < u2TotalBlock; i++)
        {
            u2LcDimCurWeight = ((AbsDiff((UINT16)((u4DimBufferPre[i] + 64)>>7),u2DimmingResult[i])*_u2LcDimDiffGain + 128)>>8) + u2LcDimCurWMin;

            u2LcDimCurWeight = CLIP(u2LcDimCurWeight, 0, 0x80);
            u2LcDimPreWeight = 0x80 - u2LcDimCurWeight;
            
            u4DimBufferPre[i] = ((u4DimBufferPre[i] + 64)>>7) * u2LcDimPreWeight + (UINT32)u2DimmingResult[i] * u2LcDimCurWeight;
            u2DimBufferResult[i] = (UINT16)((u4DimBufferPre[i] + 64)>>7);
        }

    }


//4. backup result

//    x_memcpy(u2DimBufferPre, u2DimBufferResult, sizeof(UINT16)*LCDIM_SW_MODE_MAX_BLK_NUM);
    x_memcpy(u2DimmingResult, u2DimBufferResult, sizeof(UINT16)*LCDIM_SW_MODE_MAX_BLK_NUM); 

}

void vDrvLcDimAdapDimClip(UINT16 *u2DimmingResult)
{
    UINT8 i;
    UINT16 u2TotalBlock = u2BlockV * u2BlockH;
    UINT16 u2Max = 0;
    UINT16 u2Min = 1023;
    UINT16 u2NeiDiffMax = 0;
    UINT16 u2AbsDiffMax = 0;
    UINT8 u1DiffFactor = 0;
    UINT8 u1DiffRatio = IO32ReadFldAlign(LCDIM_ADAP_CLIP_0, ADAP_LCDIM_ADAP_CLIP_RATIO);
    UINT8 u1DimMin = IO32ReadFldAlign(LCDIM_ADAP_CLIP_1, ADAP_LCDIM_ADAP_CLIP_DIM_MIN);
    UINT8 u1DimMax = IO32ReadFldAlign(LCDIM_ADAP_CLIP_1, ADAP_LCDIM_ADAP_CLIP_DIM_MAX);
    UINT8 u1Dim;    
    UINT8 u1ThMin = IO32ReadFldAlign(LCDIM_ADAP_CLIP_1, ADAP_LCDIM_ADAP_CLIP_TH_MIN);
    UINT8 u1ThMax = IO32ReadFldAlign(LCDIM_ADAP_CLIP_1, ADAP_LCDIM_ADAP_CLIP_TH_MAX);


    
    if(IO32ReadFldAlign(LCDIM_TEMP_IIR_0, ADAP_LCDIM_ADAP_CLIP_EN)== SV_FALSE)
        return;

    for(i = 0; i < u2TotalBlock; i++)
    {
        u2Max = MAX(u2DimmingResult[i], u2Max);
        u2Min = MIN(u2DimmingResult[i], u2Min);            
    }
    u2AbsDiffMax = u2Max - u2Min;
    
    for(i = 0; i < (u2TotalBlock - 1); i++)
    {
        u2NeiDiffMax = MAX(ABS(u2DimmingResult[i] - u2DimmingResult[i+1]), u2NeiDiffMax);
    }

    vIO32WriteFldAlign(LCDIM_ADAP_CLIP_0, ((u2AbsDiffMax+2)>>2), ADAP_LCDIM_ADAP_CLIP_ABSDIFF);
    vIO32WriteFldAlign(LCDIM_ADAP_CLIP_0, ((u2NeiDiffMax+2)>>2), ADAP_LCDIM_ADAP_CLIP_NEIDIFF);

    u1DiffFactor = (UINT8)(((UINT32)((u2AbsDiffMax+2)>>2)*u1DiffRatio + ((u2NeiDiffMax+2)>>2)*(256 - u1DiffRatio) + 128 )>>8);

    vIO32WriteFldAlign(LCDIM_ADAP_CLIP_0, u1DiffFactor, ADAP_LCDIM_ADAP_CLIP_DIFF_FAC);

    if(u1DiffFactor < u1ThMin)
        u1Dim = u1DimMin;
    else if(u1DiffFactor > u1ThMax)
        u1Dim = u1DimMax;
    else
        u1Dim = (UINT8)(((UINT32)(u1DiffFactor - u1ThMin)*(UINT32)(u1DimMax - u1DimMin))/(u1ThMax - u1ThMin)) + u1DimMin;
    
//Don't use HW mindim
//    vIO32WriteFldAlign(LD2_MODE_05, (UINT16)u1Dim<<2, C_LD_MIN_DIM);

    for(i = 0; i < u2TotalBlock; i++)
    {
        u2DimmingResult[i] = MAX(u2DimmingResult[i], (UINT16)u1Dim<<2);     
    }
}



void vDrvLcdimSCGSpeedCtrlV2(void)
{

//Get Target Dimming Result
    if(IO32ReadFldAlign(LD2_DIM_RESULT_00, LD_DIM_READY) == SV_TRUE)
    {
        vDrvGetLcDimDimmingResultHW(u2LcdimDimResult); //move local dimming data to buffer

    //Adaptive Clip
        vDrvLcDimAdapDimClip(u2LcdimDimResult);        
    }
    else
    {
        LOG(2, "Local Dimming Retrieve Data Error.\n");
    }
   
//Apply IIR
    vDrvLcDimAdapiveIIR(u2LcdimDimResult);

//FW dimming (cur_W)
    vDrvLcdimSetDimSWMode(u2LcdimDimResult);

}

void vDrvLcdimSCGProc(void)
{
    UINT8 u1LcdimSceneChanged = SV_FALSE; 

    if(rLCDIMFncEn.u1ForceScenceChange)
    {
        vDrvLcdimResetTemporalFilter(SV_ON);
        vDrvClearDataQue();
        LOG(4, "LCDIM force scene change\n");
        LcdimSCGConf = 0;        
    }
    else if(rLCDIMFncEn.u1SCGSpeedCtrlEn)
    {
        if(rLCDIMFncEn.u1SCGSpeedCtrlVer == LCDIM_SPCTRL_VER_0)
            vDrvLcdimSCGSpeedCtrl();
        else
            vDrvLcdimSCGSpeedCtrlV2();
    }
    else
    {          
        u1LcdimSceneChanged = bDrvLcdimDetectSceneChange();

        if(u1LcdimSceneChanged)
        {
            vDrvLcdimResetTemporalFilter(SV_ON);
            vDrvClearDataQue();
            LOG(4, "LCDIM scene change\n");
        }    
        else
        {
            vDrvLcdimResetTemporalFilter(SV_OFF);
        }    
        LcdimSCGConf = 0;
    }
  
}

void vDrvLcdimSPIProc(void)
{
    if( IO32ReadFldAlign(ADAP_LCDIM_SPI, SPI_AUTO_VSYNC_EN)== SV_TRUE)
    {
        vDrvLcdimSpiCommandGen(E_SPI_PAGE_WRITE, 0);
    }
}

#define LCDIM_ADVPS_START_TIME      300
#define LCDIM_ADVPS_FULL_RATIO      256
#define LCDIM_ADVPS_RECOVER_STEP    100

void vDrvLcdimAdvPowerSaving(void)
{
    static UINT32 u4AdvPSConf = 0;
    UINT16 u2AdvPSRatio_L = IO32ReadFldAlign(ADAP_LCDIM_ADV_PS, LCDIM_ADV_PS_RATIO);

    if(rLCDIMFncEn.u1AdvancePowerSaveEn == SV_FALSE)
    {
        u2AdvPSRatio = LCDIM_ADVPS_FULL_RATIO;
        u4AdvPSConf = 0;
    }
    else
    {
    //Detection
        if(u4HuePxlCnt > (UINT32)(HUE_HIST_NORM_BASE>>1))
        {
            u4AdvPSConf ++;
        }
        else
        {
            if(u4AdvPSConf > LCDIM_ADVPS_RECOVER_STEP)
                u4AdvPSConf  = u4AdvPSConf - LCDIM_ADVPS_RECOVER_STEP;
            else
                u4AdvPSConf = 0;
        }
        u4AdvPSConf = CLIP(u4AdvPSConf, 1, 4094);
    //Remap to power saving ratio
        u2AdvPSRatio = LCDIM_ADVPS_FULL_RATIO - 
        (((u4AdvPSConf - LCDIM_ADVPS_START_TIME)*(LCDIM_ADVPS_FULL_RATIO - u2AdvPSRatio_L) + 1024 ) >> 11);
    
        u2AdvPSRatio = CLIP(u2AdvPSRatio, u2AdvPSRatio_L, LCDIM_ADVPS_FULL_RATIO);

    }
    
    vIO32WriteFldAlign(ADAP_LCDIM_ADV_PS, u2AdvPSRatio, LCDIM_ADV_PS_RATIO_RESULT);
}

void vDrvLcdimGetBlkinfo(UINT8 *u1BlkInfo)
{
    UINT8 u1TotalBlock = u2BlockV * u2BlockH;
    UINT16 u2CurPos = 5;
    UINT16 u2CurAddrOfst = 0;
    UINT16 u2Remainder = u1TotalBlock % 5;    
    UINT32 u4Read;

    
    while(u2CurPos <= u1TotalBlock)
    {
        u4Read = u4IO32Read4B(LD2_MODE_25 + u2CurAddrOfst);
    
        u1BlkInfo[u2CurPos-1] = (UINT8)(((u4Read >> 24) & 0x3F));
        u1BlkInfo[u2CurPos-2] = (UINT8)(((u4Read >> 18) & 0x3F));        
        u1BlkInfo[u2CurPos-3] = (UINT8)(((u4Read >> 12) & 0x3F));
        u1BlkInfo[u2CurPos-4] = (UINT8)(((u4Read >> 6) & 0x3F));
        u1BlkInfo[u2CurPos-5] = (UINT8)((u4Read & 0x3F));
    
        u2CurPos      += 5;
        u2CurAddrOfst += 4;
    }
    
    if(u2Remainder == 1)
    {
        u4Read = u4IO32Read4B(LD2_MODE_25 + u2CurAddrOfst);
        
        u1BlkInfo[u2CurPos-5] = (UINT8)((u4Read & 0x3F));
    }
    else if(u2Remainder == 2)
    {
        u4Read = u4IO32Read4B(LD2_MODE_25 + u2CurAddrOfst);
    
        u1BlkInfo[u2CurPos-4] = (UINT8)(((u4Read >> 6) & 0x3F));
        u1BlkInfo[u2CurPos-5] = (UINT8)((u4Read & 0x3F));
    }
    else if(u2Remainder == 3)
    {
        u4Read = u4IO32Read4B(LD2_MODE_25 + u2CurAddrOfst);
    
        u1BlkInfo[u2CurPos-3] = (UINT8)(((u4Read >> 12) & 0x3F));
        u1BlkInfo[u2CurPos-4] = (UINT8)(((u4Read >> 6) & 0x3F));
        u1BlkInfo[u2CurPos-5] = (UINT8)((u4Read & 0x3F));
    }
    else if(u2Remainder == 4)
    {
        u4Read = u4IO32Read4B(LD2_MODE_25 + u2CurAddrOfst);
    
        u1BlkInfo[u2CurPos-2] = (UINT8)(((u4Read >> 18) & 0x3F));
        u1BlkInfo[u2CurPos-3] = (UINT8)(((u4Read >> 12) & 0x3F));
        u1BlkInfo[u2CurPos-4] = (UINT8)(((u4Read >> 6) & 0x3F));
        u1BlkInfo[u2CurPos-5] = (UINT8)((u4Read & 0x3F));
    }

}

#define LCDIM_BLKSCG_TH     0x20
void vDrvLcdimBlkSCGProc(void)
{
    UINT8 u1BlkSCGMode = IO32ReadFldAlign(LD2_MODE_23, C_LD_BLK_SCENE_CHG_MODE);
    UINT8 u1TotalBlock = u2BlockV * u2BlockH;
    UINT8 i;
    UINT32 u4BlkSCGTrigger = 0;
    
    UINT8 u1BlkInfo[LCDIM_MAX_BLKINFO_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //fix warning
    static UINT8 u1BlkInfoRec[LCDIM_MAX_BLKINFO_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //fix warning

    if(u1TotalBlock > LCDIM_MAX_BLKINFO_NUM)
    {
        //not support BLK SCG
        vIO32WriteFldAlign(LD2_MODE_23, 0, C_LD_BLK_SCENE_CHG_MODE);        
    }

    if(u1BlkSCGMode == 0)    //BLK SCG Off
    {
        //Do nothing
    }
    else if(u1BlkSCGMode == 2)  //HW mode
    {
        //Do nothing
    }
    else if(u1BlkSCGMode == 1)  //FW mode
    {
        vDrvLcdimGetBlkinfo(u1BlkInfo);
    }

    for(i = 0; i < u1TotalBlock; i++)
    {
        if( (u1BlkInfo[i] > u1BlkInfoRec[i]) && (ABS(u1BlkInfo[i] - u1BlkInfoRec[i]) > LCDIM_BLKSCG_TH)   )
        {
            u4BlkSCGTrigger = u4BlkSCGTrigger | (1 << i);
        }
    }

    vIO32WriteFldAlign(LD2_MODE_17, (u4BlkSCGTrigger & 0x00FF), C_LD_BLK_SCENE_CHG_0_15);
    vIO32WriteFldAlign(LD2_MODE_18, ((u4BlkSCGTrigger>>16) & 0x00FF), C_LD_BLK_SCENE_CHG_16_47);

    x_memcpy(u1BlkInfoRec, u1BlkInfo, sizeof(UINT8)*LCDIM_MAX_BLKINFO_NUM);    

}

void vDrvLcdimProc(void)
{
    if(bSupportLocalDimming)
    {
        if(u1LCDIMOnOffStatus)
        {
//            vDrvAdaptiveLcDimUpdateParam();
//            u1DrvLcdimGetPostInfo();
            vDrvAdaptiveLcDim(u2LcdimDimTable_DFT, u2LcdimDimTable_STR);
            vDrvLcdimSCGProc();

//            vDrvLcdimAdvPowerSaving();
        }
        vDrvLcdimSPIProc();
    }
}

void vDrvLcdimProcMainLoop(void)
{
    if(bSupportLocalDimming)
    {
        if(u1LCDIMOnOffStatus)
        {
            vDrvAdaptiveLcDimUpdateParam();
            u1DrvLcdimGetPostInfo();    

            if(rLCDIMFncEn.u1SCGSpeedCtrlEn)
            {
                vDrvLcdimSCGSpeedCtrlMainLoop();
            }
        }
    }
}


void vDrvLcdimSetDemoEffect(UINT8 *bDimmingResult)
{ 
    // note : this function need to be called by SPI write command
    static UINT32 u4DemoRound = 0;
    UINT32 i;

    if (u4DemoFrameCount <= 60)
    {
        // Set SPI dimming command
        for(i = 0; i < u2BlockH  * u2BlockV; i ++)
        {
            bDimmingResult[i] = 0xFF;
        }
        u4DemoFrameCount ++;
    }
    else
    {
        if((++u4DemoFrameCount)%30 == 0)
        {
            if (u4CurrentDemoBlock == (u2BlockH  * u2BlockV))
            {
                u4DemoRound ++;
                u4CurrentDemoBlock = 0;
            }
            else
            {
                u4CurrentDemoBlock ++;
            }
        }

        // Check demo rounds
        //Remove check demo cnt, demo mode will lasted until off cmd
/*
        if (u4DemoRound == LCDIM_DEMO_ROUND)
        {
            u4DemoRound = 0;
            vDrvLcdimSetDSDemo(1, SV_OFF);
            return;
        }
*/
        // Set SPI dimming command

        if(IO32ReadFldAlign(LCDIM_02, LCDIM_SPI_INV) == SV_OFF)
        {
            for(i = 0; i < u2BlockH * u2BlockV; i ++)
            {
                bDimmingResult[(u2BlockH * u2BlockV - 1) - i] = (i == u4CurrentDemoBlock) ? 0xFF : 0x0;
            }
        }
        else
        {
            for(i = 0; i < u2BlockH * u2BlockV; i ++)
            {
                bDimmingResult[(u2BlockH * u2BlockV - 1) - i] = (i == (u2BlockH * u2BlockV - u4CurrentDemoBlock)) ? 0xFF : 0x0;
            }            
        }
    }
}

void vDrvLcdimSetDemoParam(UINT8 bSel)
{ 
    switch(bSel)
    {
        case 1:     //low
            vDrvLcdimWriteDimTable(u2LcdimDimTable_LOW);
            vDrvLcdimWriteGainTable(u2LcdimGainTable_LOW);
            vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_LOW);       
            x_memcpy(u2LcdimDimTable, u2LcdimDimTable_LOW, sizeof(UINT16)*LCDIM_DIMMING_NUM);

            rLCDIMSpCfg.u1DimWeight0 = 0x3F;
            rLCDIMSpCfg.u1DimWeight1 = 0x3F;
            rLCDIMSpCfg.u1DimWeight2 = 0x28;
            rLCDIMSpCfg.u1DimWeight3 = 0x14;
            rLCDIMSpCfg.u1DimWeight4 = 0x09;        
            vDrvLcdimWriteSpatialCfg(&rLCDIMSpCfg);

            vIO32WriteFldAlign(LD2_MODE_17, 0x04, C_LD_MAX_UP_DIM_STEP);
            vIO32WriteFldAlign(LD2_MODE_17, 0x04, C_LD_MAX_DN_DIM_STEP);
            vIO32WriteFldAlign(LD2_MODE_08, 0x08, C_LD_RGB_WEIGHT);
            vIO32WriteFldAlign(LD2_MODE_08, 0x07, C_LD_LPF_WEIGHT);    
            vIO32WriteFldAlign(LD2_MODE_00, 896, C_LD_MAX_PXL_TH);            
        break;

        case 2:     //MID
            vDrvLcdimWriteDimTable(u2LcdimDimTable_MID);
            vDrvLcdimWriteGainTable(u2LcdimGainTable_MID);
            vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_MID);
            x_memcpy(u2LcdimDimTable, u2LcdimDimTable_MID, sizeof(UINT16)*LCDIM_DIMMING_NUM);

            rLCDIMSpCfg.u1DimWeight0 = 0x3F;
            rLCDIMSpCfg.u1DimWeight1 = 0x38;
            rLCDIMSpCfg.u1DimWeight2 = 0x24;
            rLCDIMSpCfg.u1DimWeight3 = 0x10;
            rLCDIMSpCfg.u1DimWeight4 = 0x05;                    
            vDrvLcdimWriteSpatialCfg(&rLCDIMSpCfg);

            vIO32WriteFldAlign(LD2_MODE_17, 0x04, C_LD_MAX_UP_DIM_STEP);
            vIO32WriteFldAlign(LD2_MODE_17, 0x04, C_LD_MAX_DN_DIM_STEP);
            vIO32WriteFldAlign(LD2_MODE_08, 0x07, C_LD_RGB_WEIGHT);
            vIO32WriteFldAlign(LD2_MODE_08, 0x07, C_LD_LPF_WEIGHT);            
            vIO32WriteFldAlign(LD2_MODE_00, 928, C_LD_MAX_PXL_TH);
        break;

        case 3:     //STRONG
            vDrvLcdimWriteDimTable(u2LcdimDimTable_STRONG);
            vDrvLcdimWriteGainTable(u2LcdimGainTable_STRONG);
            vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable_STRONG);
            x_memcpy(u2LcdimDimTable, u2LcdimDimTable_STRONG, sizeof(UINT16)*LCDIM_DIMMING_NUM);

            rLCDIMSpCfg.u1DimWeight0 = 0x3F;
            rLCDIMSpCfg.u1DimWeight1 = 0x2C;
            rLCDIMSpCfg.u1DimWeight2 = 0x1D;
            rLCDIMSpCfg.u1DimWeight3 = 0x0E;
            rLCDIMSpCfg.u1DimWeight4 = 0x00;        
            vDrvLcdimWriteSpatialCfg(&rLCDIMSpCfg);
            
            vIO32WriteFldAlign(LD2_MODE_17, 0x04, C_LD_MAX_UP_DIM_STEP);
            vIO32WriteFldAlign(LD2_MODE_17, 0x04, C_LD_MAX_DN_DIM_STEP);
            vIO32WriteFldAlign(LD2_MODE_08, 0x06, C_LD_RGB_WEIGHT);
            vIO32WriteFldAlign(LD2_MODE_08, 0x07, C_LD_LPF_WEIGHT);                
            vIO32WriteFldAlign(LD2_MODE_00, 960, C_LD_MAX_PXL_TH);
        break;

        default:
            vDrvLcdimWriteDimTable(u2LcdimDimTable);
            vDrvLcdimWriteGainTable(u2LcdimGainTable);
            vDrvLcdimWriteGainLimitTable(u2LcdimGainLimitTable);       

            rLCDIMSpCfg.u1DimWeight0 = 0x3F;
            rLCDIMSpCfg.u1DimWeight1 = 0x3F;
            rLCDIMSpCfg.u1DimWeight2 = 0x28;
            rLCDIMSpCfg.u1DimWeight3 = 0x14;
            rLCDIMSpCfg.u1DimWeight4 = 0x09;        
            vDrvLcdimWriteSpatialCfg(&rLCDIMSpCfg);

            vIO32WriteFldAlign(LD2_MODE_17, 0x05, C_LD_MAX_UP_DIM_STEP);
            vIO32WriteFldAlign(LD2_MODE_17, 0x14, C_LD_MAX_DN_DIM_STEP);
            vIO32WriteFldAlign(LD2_MODE_08, 0x06, C_LD_RGB_WEIGHT);
            vIO32WriteFldAlign(LD2_MODE_08, 0x07, C_LD_LPF_WEIGHT);
            vIO32WriteFldAlign(LD2_MODE_00, 900, C_LD_MAX_PXL_TH);
        break;        
    }
}

void vDrvLcDimPQSuspend(void)
{
    vIO32WriteFldAlign(LD2_MODE_00, 0,  C_LD_EN);   //always enable LCD to update dimming result
    vIO32WriteFldAlign(LD2_MODE_09, 0, C_LD_FORCE_DIM_EN);
	vIO32WriteFldAlign(LD2_MODE_00, 1, C_LD_BYPASS);
	vIO32WriteFldAlign(LD2_MODE_00, 0, C_LD_DIS_DS);                
    vIO32WriteFldAlign(LD2_MODE_09, 0x3FF,   C_LD_FORCE_DIM_VALUE);    
}

void vDrvLcDimPQResume(void)
{
    if(bSupportLocalDimming)   
    {
        if(bDrvLcDimPanelInit() == SV_SUCCESS)
        {        
            vDrvLcdimQualityHWInit();
        }
    }
}

void vDrvLcDimSetSWTable(UINT16 *u2LDGainLimitTable_LOW_BND, UINT16 *u2LDGainLimitTable_UP_BND)
{
    UINT32 i;

	for(i = 0; i < LCDIM_GAIN_LIMIT_NUM; i++)
    {
        vIO32Write2B(ADAP_LCDIM_ADAP_GAIN_LMT_H_0_1+2*i, u2LDGainLimitTable_UP_BND[i]);
        vIO32Write2B(ADAP_LCDIM_ADAP_GAIN_LMT_L_0_1+2*i, u2LDGainLimitTable_LOW_BND[i]);
    }
}

void vDrvLcDimSetHWParam(LCDIM_HW_PARAM_T* pLCDIM_HWParam)
{

	vIO32WriteFldAlign(LD2_MODE_00, pLCDIM_HWParam->u2max_pxl_th, C_LD_MAX_PXL_TH);
	vIO32WriteFldAlign(LD2_MODE_04, pLCDIM_HWParam->u1dim_wet_0, C_LD_DIM_WET_0);
	vIO32WriteFldAlign(LD2_MODE_04, pLCDIM_HWParam->u1dim_wet_1, C_LD_DIM_WET_1);
	vIO32WriteFldAlign(LD2_MODE_04, pLCDIM_HWParam->u1dim_wet_2, C_LD_DIM_WET_2);
	vIO32WriteFldAlign(LD2_MODE_04, pLCDIM_HWParam->u1dim_wet_3, C_LD_DIM_WET_3);
	vIO32WriteFldAlign(LD2_MODE_05, pLCDIM_HWParam->u1dim_wet_4, C_LD_DIM_WET_4);
	vIO32WriteFldAlign(LD2_MODE_08, pLCDIM_HWParam->u1rgb_weight, C_LD_RGB_WEIGHT);
	vIO32WriteFldAlign(LD2_MODE_24, pLCDIM_HWParam->u1rgb_weight, C_LD_RGB_WEIGHT2);
	vIO32WriteFldAlign(LD2_MODE_08, pLCDIM_HWParam->u1dim_wet_4, C_LD_LPF_WEIGHT);
	vIO32WriteFldAlign(LD2_MODE_16, pLCDIM_HWParam->u1dim_wet_4, C_LD_BLKINFO_WEIGHT);
	vIO32WriteFldAlign(LD2_MODE_16, pLCDIM_HWParam->u1dim_wet_4, C_LD_MAXINFO_LPF_MODE);
	vIO32WriteFldAlign(LD2_MODE_05, pLCDIM_HWParam->u2max_dim, C_LD_MAX_DIM);
	vIO32WriteFldAlign(LD2_MODE_05, pLCDIM_HWParam->u2min_dim, C_LD_MIN_DIM);
	vIO32WriteFldAlign(LD2_MODE_06, pLCDIM_HWParam->u1dy_dim_slope, C_LD_DY_DIM_SLOPE);
	//vIO32WriteFldAlign(LD2_MODE_06, pLCDIM_HWParam->u1max_up_dim_step, C_LD_DY_DIM_SLOPE);
	//vIO32WriteFldAlign(LD2_MODE_06, pLCDIM_HWParam->u1max_dn_dim_step, C_LD_DY_DIM_SLOPE);
	vIO32WriteFldAlign(LCDIM_SCENE_CHANGE0, pLCDIM_HWParam->u2max_diff_thd, LCDIM_SCG_RGB_MAX_DIFF_THD);
	vIO32WriteFldAlign(LCDIM_SCENE_CHANGE0, pLCDIM_HWParam->u2total_diff_thd, LCDIM_SCG_RGB_TOTAL_DIFF_THD);
	vIO32WriteFldAlign(LCDIM_SCENE_CHANGE1, pLCDIM_HWParam->u2avg_diff_thd, LCDIM_SCG_RGB_AVG_DIFF_THD);
	
}

void vDrvLcDimSetSWParam(LCDIM_SW_PARAM_T* pLCDIM_SWParam)
{
	vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, pLCDIM_SWParam->u1lcdim_speed_ctrl_en, LCDIM_SPEED_CTRL_SCG_EN);
	vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, pLCDIM_SWParam->u1osd_detect, LCDIM_OSD_DETECT_EN);
	vIO32WriteFldAlign(ADAP_LCDIM_02, pLCDIM_SWParam->u1motion_th, LCDIM_NR_MOTION_THD);
	vIO32WriteFldAlign(LCDIM_SCENE_CHANGE2, pLCDIM_SWParam->u2max_diff_thd2, LCDIM_SCG_RGB_MAX_DIFF_THD2);
	vIO32WriteFldAlign(LCDIM_SCENE_CHANGE2, pLCDIM_SWParam->u2total_diff_thd2, LCDIM_SCG_RGB_TOTAL_DIFF_THD2);
	vIO32WriteFldAlign(LCDIM_SCENE_CHANGE1, pLCDIM_SWParam->u2avg_diff_thd2, LCDIM_SCG_RGB_AVG_DIFF_THD2);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL5, pLCDIM_SWParam->u1scg_wait_th, LCDIM_SPCTRL_SCG_WAIT_CNT);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL3, pLCDIM_SWParam->u1motion_penalty, LCDIM_SPCTRL_MOTION_PENALTY);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL3, pLCDIM_SWParam->u1gamma_penalty, LCDIM_SPCTRL_GAMMA_PENALTY);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL3, pLCDIM_SWParam->u1static_step, LCDIM_SPCTRL_STATIC_STEP);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL2, pLCDIM_SWParam->u1maxrgb_step_low, LCDIM_SPCTRL_MAXRGB_STEP_LOW);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL2, pLCDIM_SWParam->u1maxrgb_step_mid_dn, LCDIM_SPCTRL_MAXRGB_STEP_MID_DN);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL2, pLCDIM_SWParam->u1maxrgb_step_mid_up, LCDIM_SPCTRL_MAXRGB_STEP_MID_UP);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL2, pLCDIM_SWParam->u1maxrgb_step_high, LCDIM_SPCTRL_MAXRGB_STEP_HIGH);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL9, pLCDIM_SWParam->u1predet_apl_th_low, LCDIM_PREDETECT_APL_TH_LOW);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL9, pLCDIM_SWParam->u1predet_apl_th_high, LCDIM_PREDETECT_APL_TH_HIGH);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, pLCDIM_SWParam->u1predet_apl_th_ostg, LCDIM_PREDETECT_APL_TH_OSTG);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL9, pLCDIM_SWParam->u1predet_step, LCDIM_SPCTRL_PREDETECT_STEP);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL9, pLCDIM_SWParam->u1predet_frame_th, LCDIM_SPCTRL_PREDETECT_FRAME_TH);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL11, pLCDIM_SWParam->u1fadein_det_dark_th, LCDIM_FADEIN_DARK_TH);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL11, pLCDIM_SWParam->u1fadein_det_inc_th, LCDIM_FADEIN_INC_TH);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL11, pLCDIM_SWParam->u1fadein_step, LCDIM_FADEIN_STEP);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL4, pLCDIM_SWParam->u1confidence_th0, LCDIM_SPCTRL_CONF_TH0);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL4, pLCDIM_SWParam->u1confidence_th1, LCDIM_SPCTRL_CONF_TH1);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL4, pLCDIM_SWParam->u1confidence_th2, LCDIM_SPCTRL_CONF_TH2);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL4, pLCDIM_SWParam->u1confidence_th3, LCDIM_SPCTRL_CONF_TH3);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL5, pLCDIM_SWParam->u1confidence_thdft, LCDIM_SPCTRL_CONF_TH_DFT);	
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL6, pLCDIM_SWParam->u1lcdim_max_up_dim_step_low, LCDIM_MAX_UP_DIM_STEP_LOW);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL6, pLCDIM_SWParam->u1lcdim_max_up_dim_step_mid_dn, LCDIM_MAX_UP_DIM_STEP_MID_DN);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL6, pLCDIM_SWParam->u1lcdim_max_up_dim_step_mid_up, LCDIM_MAX_UP_DIM_STEP_MID_UP);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL6, pLCDIM_SWParam->u1lcdim_max_up_dim_step_high, LCDIM_MAX_UP_DIM_STEP_HIGH);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL7, pLCDIM_SWParam->u1lcdim_max_dn_dim_step_low, LCDIM_MAX_DN_DIM_STEP_LOW);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL7, pLCDIM_SWParam->u1lcdim_max_dn_dim_step_mid_dn, LCDIM_MAX_DN_DIM_STEP_MID_DN);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL7, pLCDIM_SWParam->u1lcdim_max_dn_dim_step_mid_up, LCDIM_MAX_DN_DIM_STEP_MID_UP);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL7, pLCDIM_SWParam->u1lcdim_max_dn_dim_step_high, LCDIM_MAX_DN_DIM_STEP_HIGH);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL8, pLCDIM_SWParam->u1lcdim_dy_dim_slope_low, LCDIM_DY_DIM_SLOPE_LOW);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL8, pLCDIM_SWParam->u1lcdim_dy_dim_slope_mid_dn, LCDIM_DY_DIM_SLOPE_MID_DN);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL8, pLCDIM_SWParam->u1lcdim_dy_dim_slope_mid_up, LCDIM_DY_DIM_SLOPE_MID_UP);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL8, pLCDIM_SWParam->u1lcdim_dy_dim_slope_high, LCDIM_DY_DIM_SLOPE_HIGH);
	vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, pLCDIM_SWParam->u1lcdim_adap_mindim, LCDIM_ADAP_MINDIM_EN);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL0, pLCDIM_SWParam->u1dist_th, LCDIM_ADAP_DIM_DIST_TH);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL0, pLCDIM_SWParam->u1apl_th, LCDIM_ADAP_DIM_APL_TH);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, pLCDIM_SWParam->u1adap_weight_0_l, LCDIM_ADAP_DIM_WEIGHT0_L);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, pLCDIM_SWParam->u1adap_weight_1_l, LCDIM_ADAP_DIM_WEIGHT1_L);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, pLCDIM_SWParam->u1adap_weight_2_l, LCDIM_ADAP_DIM_WEIGHT2_L);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL2, pLCDIM_SWParam->u1adap_weight_3_l, LCDIM_ADAP_DIM_WEIGHT3_L);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL3, pLCDIM_SWParam->u1adap_weight_4_l, LCDIM_ADAP_DIM_WEIGHT4_L);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, pLCDIM_SWParam->u1adap_weight_0_h, LCDIM_ADAP_DIM_WEIGHT0_H);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, pLCDIM_SWParam->u1adap_weight_1_h, LCDIM_ADAP_DIM_WEIGHT1_H);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, pLCDIM_SWParam->u1adap_weight_2_h, LCDIM_ADAP_DIM_WEIGHT2_H);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL4, pLCDIM_SWParam->u1adap_weight_3_h, LCDIM_ADAP_DIM_WEIGHT3_H);
	vIO32WriteFldAlign(ADAP_LCDIM_ADAP_DIM_CTRL5, pLCDIM_SWParam->u1adap_weight_4_h, LCDIM_ADAP_DIM_WEIGHT4_H);
	vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, pLCDIM_SWParam->u1adv_ps_en, LCDIM_ADVANCE_PS_EN);
	vIO32WriteFldAlign(ADAP_LCDIM_ADV_PS, pLCDIM_SWParam->u1adv_ps_ratio, LCDIM_ADV_PS_RATIO);
	vIO32WriteFldAlign(LCDIM_00, pLCDIM_SWParam->u1scg_iir_en, LCDIM_ADAP_SPI_IIR_EN);
	vIO32WriteFldAlign(LCDIM_01, pLCDIM_SWParam->u1iir_sel, LCDIM_FILTER_TYPE);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL11, pLCDIM_SWParam->u1downtrend_dim_th, LCDIM_DOWNTREND_DIM_TH);
	vIO32WriteFldAlign(LCDIM_DEMO_SETTING_0, pLCDIM_SWParam->u1scg_enable, LCDIM_SCG_EN);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, pLCDIM_SWParam->u1boost_0, LCDIM_CG_BOOST_0);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, pLCDIM_SWParam->u1boost_1, LCDIM_CG_BOOST_1);
	vIO32WriteFldAlign(ADAP_LCDIM_SPEED_CTRL10, pLCDIM_SWParam->u1boost_2, LCDIM_CG_BOOST_2);


	 
}


