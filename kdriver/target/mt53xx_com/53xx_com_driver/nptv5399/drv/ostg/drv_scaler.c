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
 * $Date: 2015/01/23 $
 * $RCSfile: drv_scaler.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.c
 *  Brief of file drv_scaler.c.
 *  Details of file drv_scaler.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#ifndef CC_COPLAT_MT82
#include "osd_drvif.h"
#include "drvcust_if.h"
#include "tve_if.h"
#endif
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
// Scaler related header files
#include "hw_ckgen.h"
#include "hw_scpos.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_scaler_psc.h"
#include "drv_scaler_gfx.h"
#include "drv_upscaler.h"
#include "drv_predown.h"
#include "drv_regen.h"
#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "scpos_debug.h"
#include "vdo_rm.h"
#include "drv_mon_out.h"
#include "vdp_venc_ctrl.h"



// nptv header files
#include "general.h"       
#include "sv_const.h"       
#include "video_def.h"      
#include "vdo_misc.h"       
#include "vdp_display.h"
#include "vdp_if.h"
#include "drv_video.h"   
#include "source_select.h"
#include "drv_display.h"
#include "panel.h"
#include "mute_if.h"
#ifdef CC_MT5399
#include "drv_mjc.h"
#endif
#include "hw_predown.h"
#include "hw_regen.h"
#include "drv_tdtv_drvif.h"

// drvif header files
#include "fbm_drvif.h"
#include "srm_drvif.h"
#include "vdp_drvif.h"
#include "nptv_drvif.h"

// Other header files
#include "c_model.h"
#include "x_bim.h"
#include "x_mid.h"
#include "x_assert.h"
#include "x_util.h"

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#include "ir_if.h"
#endif
#if SUPPORT_DOT_TO_DOT_PATTERN
#include "hw_tdc.h"
#endif
#ifdef CC_SCALER_LR_SHIFT_WA
#include "hw_ycproc.h"
#endif

#include "hw_di.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations : all WA
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DFT_SCPOS_MAIN_444_FIFO_LENGTH        1920
#define DFT_SCPOS_MAIN_422_FIFO_LENGTH        2100
#define DFT_SCPOS_SUB_444_FIFO_LENGTH        960
#define DFT_SCPOS_SUB_422_FIFO_LENGTH        960
//static const UINT32 _u4SCPOS_FIFO_LENGTH[2][2] = {{DFT_SCPOS_MAIN_422_FIFO_LENGTH, DFT_SCPOS_MAIN_444_FIFO_LENGTH},//main
//    {DFT_SCPOS_SUB_422_FIFO_LENGTH, DFT_SCPOS_SUB_444_FIFO_LENGTH}};    //sub
#define FRAME_TRACK_DEFAULT  ((1<<16)-1)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#ifdef CC_SUPPORT_STR
#define  vDrvScposReset( x )     ( x==ON ? (vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG1 ,  0x7 , FLD_SCALER_RST)):(vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG1, 0x00 , FLD_SCALER_RST)) )
#define  vDrvScposSetClock(x)    ( x==ON ? (vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG1  , 0x7 , FLD_SCALER_CKEN)):(vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG1, 0x00 , FLD_SCALER_CKEN)) )
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
SCALER_OUTTIMING_PRM_T _arOutTimingPrm = {2200,1920,1125,1080,0,0,0,0,0x8000,0x8000,60,SV_FALSE,SV_FALSE};
#if defined(CC_MT5399) || defined (CC_MT5882)
extern UINT8 _bSuperResolutionStats;
#endif

//only for test
UINT8 bLOGPath = 0;
UINT8 bLOGModule = 0;
UINT8 bLOGCnt = 0;
UINT8 bLOGDelay = 0;
//auto check
UINT8  _XdataAutoScan = 0;
UINT8  _XdataAutoScanMode = 0;
UINT8  _XdataAutoScanVDP = 0;
UINT16 _XdataAutoScanW = 0;
UINT16 _XdataAutoScanH = 0;
UINT16 _XdataAutoScanHstep = 0;
UINT16 _XdataAutoScanVstep = 0;

// nptv global variable
VDP_SCALER_PRM_T _arScalePrm[VDP_NS] =
{
    {                                                      //SV_VP_MAIN
        0,                                                 //u4Enable;
        #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
        0,                                                 //u4Bypass;
        #endif
        DFT_DS_FACTOR,                                     //u4HpsFactor;
        0,                                                 //u4Forced10BitMode;
        0,                                                 //u410BitMode;
        0,                                                 //u4Forced444;
        0,                                                 //u4Is444;
        0,                                                 //u4Forced422Scaling
        0,                                                 //u4Is422Mode    
        
        0,                                                 //u4DramInputFmt;        
        0,                                                 //u4ForcedDramWriteFmt;
        0,                                                 //u4DramWriteFmt;
        0,                                                 //u4Dram444to422En
            
        SCALER_FORCE_IMPORT_UNKNOWN,                       //force import enable to this value
        0,                                                 //u4121MapEn;
        0,                                                 //u4121Map;
        SCPIP_121MAP_H_POS_MID,                            //u2121MapHPos
        VDP_SCPOS_DISPMODE_UNKNOWN,                        //u4DispMode;
        SV_FALSE,                                          //u4IsGameMode;
        SV_FALSE,                                          //u4IsSuperGameMode;
        0,                                                 //u4MirrorEnable;
        0,                                                 //u4FlipEnable
        0,                                                 //u4Is422UVMirrorMode
        SV_FALSE,                                          //u4IsScalerBobMode
        SV_FALSE,                                          //u4IsLRDataSwap        
        0,                                                 //u4NonlinearEn;
        0,                                                 //u4NonlinearCtrl;
        0,                                                 //u4LastNonlinearCtrl;
        SV_TRUE,                                           //u4NonlinearAuto;
        {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR},    //rAutoNonlinearPrm;
        {0, 0, 0, 0},                                      //rNonlinearPrm;
        {0, 0, 0, 0},                                      //rLastNonlinearPrm;
        SCALER_DRAM_INIT_STATE_NOT_INIT,                   //eDramInitState;
        0,                                                 //u4CntToRstDramPrm;
        0,                                                 //u4MddiFifoMin;
        0,                                                 //u4MddiFifoMax;
        {0, 0, 0, 0, 0, 0, 0, 0, 0},                       //rFbmInfo;
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},             //rDramAddrInfo;
        0,                                                 //u4NewNonlinear;
        {0, 0, 0, 0, 0},                                   //_NONLINEAR_REG_T rNewNonlinearPrm;
        {0 ,0, 0, 0, 0},                                   //VDP_COBRA_NEW_NONLINEAR_REG_T rCobraNewNonlinearPrm
        {SCALER_TDTV_IN_TYPE_2D,
         SCALER_TDTV_OUT_TYPE_2D,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         SV_FALSE},//rTdtvInfo
         // For Cobra
         {VRM_MODULE_FSC,0,0,0,0,0,0,0},                   //VRM info (FSC in)
         {VRM_MODULE_MIXER,0,0,0,0,0,0,0},                 //VRM info (FSC out)
         {VRM_MODULE_MJC,0,0,0,0,0,0,0},                   //VRM info (FSC time gen.)
         0,                                                //H Scaling factor (integer part)
         0,                                                //H Scaling factor (fraction part)
         0,                                                //V Scaling factor (integer part)
         0,                                                //V Scaling factor (fraction part)
         0,                                                //H Scaling phase  (sign)
         0,                                                //H Scaling phase  (integer)
         0,                                                //H Scaling phase  (fraction)
         0,                                                //V Scaling phase  (sign)
         0,                                                //V Scaling phase  (integer)
         0,                                                //V Scaling phase  (fraction)
         0,                                                //H UV Scaling phase for 422 mode (sign)
         0,                                                //H UV Scaling phase for 422 mode (integer)
         0,                                                //H UV Scaling phase for 422 mode (fraction)
         0,                                                //V Scaling phase Top Right (sign)
         0,                                                //V Scaling phase Top Right  (integer)
         0,                                                //V Scaling phase Top Right  (fraction)
         0,                                                //V Scaling phase Bottom Left (sign)
         0,                                                //V Scaling phase Bottom Left (integer)
         0,                                                //V Scaling phase Bottom Left (fraction)
         0,                                                //V Scaling phase Bottom Right (sign)
         0,                                                //V Scaling phase Bottom Right (integer)
         0,                                                //V Scaling phase Bottom Right (fraction)
         0,                                                //ForcedDramMode 0: auto 1:force to front mode 2: force to back mode
         0,                                                //IsDramFrontMode 0: back mode 1:front mode
         0,                                                //DramSRCWidth
         0,                                                //DramWidth
         0,                                                //DramHPitch
         0,                                                //DramLinePitch
         0,                                                //DramSRCHeight
         0,                                                //DramHeight
         0,                                                //VPitch
         0,                                                //Read pointer gen position
         0,                                                //Dram Frame Dly for import
         0,                                                //for Time gen Import Enable
         0,                                                //input width(pixel based)
         0,                                                //input height(pixel based)
         0,                                                //output X(pixel based)
         0,                                                //output Y(pixel based)
         0,                                                //output Width(pixel based)
         0,                                                //output Height(pixel based)
         SV_FALSE,                                         //bUpdateOutXYWHLater   
         #if defined(DZONE_DETECTION_EN)
         SV_FALSE,                                         //bIsInDZone;
         0,                                                //u4DZoneCenter;
         #endif
    },
    {                                                      //SV_VP_PIP
        0,												   //u4Enable
        #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)         
        0,                                                 //u4Bypass;        
        #endif
        DFT_DS_FACTOR,                                     //u4HpsFactor
        0,                                                 //u4Forced10BitMode
        0,                                                 //u410BitMode
        0,                                                 //u4Forced444;
        0,                                                 //u4Is444
        0,                                                 //u4Forced422Scaling
        0,                                                 //u4Is422Mode
        
        0,                                                 //u4DramInputFmt;        
        0,                                                 //u4ForcedDramWriteFmt;
        0,                                                 //u4DramWriteFmt;
        0,                                                 //u4Dram444to422En
        
        SCALER_FORCE_IMPORT_UNKNOWN,                       //force import enable to this value        
        0,                                                 //u4121MapEn
        0,                                                 //u4121Map
        SCPIP_121MAP_H_POS_MID,                            //u2121MapHPos
        VDP_SCPOS_DISPMODE_UNKNOWN,                        //u4DispMode
        SV_FALSE,                                          //u4IsGameMode;
        SV_FALSE,                                          //u4IsSuperGameMode;
        0,                                                 //u4MirrorEnable;
        0,                                                 //u4FlipEnable
        0,                                                 //u4Is422UVMirrorMode
        SV_FALSE,                                          //u4IsScalerBobMode
        SV_FALSE,                                          //u4IsLRDataSwap        
        0,                                                 //u4NonlinearEn
        0,                                                 //u4NonlinearCtrl
        0,                                                 //u4LastNonlinearCtrl
        FALSE,                                             //u4NonlinearAuto
        {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR},    //rAutoNonlinearPrm
        {0, 0, 0, 0},                                      //rNonlinearPrm
        {0, 0, 0, 0},                                      //rLastNonlinearPrm
        SCALER_DRAM_INIT_STATE_NOT_INIT,                   //eDramInitState;        
        0,                                                 //u4CntToRstDramPrm;
        0,                                                 //u4MddiFifoMin
        0,                                                 //u4MddiFifoMax
        {0, 0, 0, 0, 0, 0, 0, 0, 0},                       //rFbmInfo;
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},             //rDramAddrInfo;        
        0,                                                 //u4NewNonlinear
        {0, 0, 0, 0, 0},                                   //rNewNonlinearPrm
        {0 ,0, 0, 0, 0},                                   //VDP_COBRA_NEW_NONLINEAR_REG_T rCobraNewNonlinearPrm        
        {SCALER_TDTV_IN_TYPE_2D,
         SCALER_TDTV_OUT_TYPE_2D,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         SV_FALSE},   //rTdtvInfo
         // For cobra
         {VRM_MODULE_FSC,0,0,0,0,0,0,0},                   //VRM info (FSC in)
         {VRM_MODULE_MIXER,0,0,0,0,0,0,0},                 //VRM info (FSC out)
         {VRM_MODULE_MJC,0,0,0,0,0,0,0},                   //VRM info (FSC time gen.)
         0,                                                //H Scaling factor (integer part)
         0,                                                //H Scaling factor (fraction part)
         0,                                                //V Scaling factor (integer part)
         0,                                                //V Scaling factor (fraction part)        
         0,                                                //H Scaling phase  (sign)
         0,                                                //H Scaling phase  (integer)
         0,                                                //H Scaling phase  (fraction)
         0,                                                //V Scaling phase  (sign)
         0,                                                //V Scaling phase  (integer)
         0,                                                //V Scaling phase  (fraction)
         0,                                                //H UV Scaling phase for 422 mode (sign)
         0,                                                //H UV Scaling phase for 422 mode (integer)
         0,                                                //H UV Scaling phase for 422 mode (fraction)
         0,                                                //V Scaling phase Top Right (sign)
         0,                                                //V Scaling phase Top Right  (integer)
         0,                                                //V Scaling phase Top Right  (fraction)
         0,                                                //V Scaling phase Bottom Left (sign)
         0,                                                //V Scaling phase Bottom Left (integer)
         0,                                                //V Scaling phase Bottom Left (fraction)
         0,                                                //V Scaling phase Bottom Right (sign)
         0,                                                //V Scaling phase Bottom Right (integer)
         0,                                                //V Scaling phase Bottom Right (fraction)         
         0,                                                //ForcedDramMode 0: auto 1:force to front mode 2: force to back mode
         0,                                                //IsDramFrontMode 0: back mode 1:front mode
         0,                                                //DramSRCWidth
         0,                                                //DramWidth
         0,                                                //DramHPitch
         0,                                                //DramLinePitch         
         0,                                                //DramSRCHeight         
         0,                                                //DramHeight
         0,                                                //VPitch         
         0,                                                //Read pointer gen position         
         0,                                                //Dram Frame Dly for import
         0,                                                //for Time gen Import Enable
         0,                                                //input width(pixel based)
         0,                                                //input height(pixel based)
         0,                                                //output X(pixel based)
         0,                                                //output Y(pixel based)
         0,                                                //output Width(pixel based)
         0,                                                //output Height(pixel based)    
         SV_FALSE,                                         //bUpdateOutXYWHLater   
         #if defined(DZONE_DETECTION_EN)
         SV_FALSE,                                         //bIsInDZone;
         0,                                                //u4DZoneCenter;
         #endif         
    }
};

extern UINT32 _u4VSyncCountSD;
RDispInfo _rTveDispInfo = {0,0,720,576,{0,0,5000,5000},0};

#if defined(SCALER_WA1_GFX)
UINT8 _VSS_SW_Trigger;
#endif

static UINT8 u1ScalerVencModule = E_VENC_MODULE_MAX;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Functions defined in drv_scaler_dram.c
//-----------------------------------------------------------------------------
EXTERN void vScpipCheckWriteCtrl(UINT8 bPath);
EXTERN void vScpipResetDramCountDown(UINT8 bPath);
//-----------------------------------------------------------------------------
// Functions defined in drv_scaler_dispmode.c
//-----------------------------------------------------------------------------
EXTERN void vScpipModeSwitch(UINT8 bPath);
EXTERN void vScpipSetDispModeVar(UINT8 bPath, UINT8 u1Dispmode);


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
#ifdef CC_FLIP_MIRROR_SUPPORT
EXTERN void vScpipConfigFlip(UINT8 bPath);
#endif

static void _vScpipNormProcByPath(const UINT32 u4VdpId); 
static void _VDP_ScposEnable(UCHAR ucVdpId, UCHAR ucEnable);

//static function for update display parameter

//Cobra
static void _vScpipUpdateDramMNRatio(UINT8 bPath);
static void _vScpipUpdateDispPrm(UINT32 u4VdpId) ;
static void _vScpipSetDispReg(UINT32 u4VdpId);
static void _vScpipUpdateInOutPrm(UINT8 bPath);
#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
static void _vScpipUpdateBypassFlg(UINT8 bPath, UINT32 inW, UINT32 inH);
#endif
static void _vScpipUpdateDramFrnBckModePrm(UINT8 bPath, UINT32 inW, UINT32 inH, UINT32 outW, UINT32 outH);
static void _vScpipUpdateDramWidthHeightPrm(UINT8 bPath, UINT32 inW, UINT32 inH, UINT32 outW, UINT32 outH);
static void _vScpipUpdateScaliingFactorPrm(UINT8 bPath, UINT32 inW, UINT32 inH, UINT32 outW, UINT32 outH);
static void _vScpipUpdateDataFormatPrm(UINT8 bPath);
static void _vScpipUpdateRptGenPrm(UINT8 bPath, UINT32 u4DramHeight);
static void _vScpipUpdateDramFrameDelay(UINT8 bPath);
static void _vScpipSetDramLinePitchReg(UINT32 u4VdpId);
UINT8 u1ScpipIsDramFrontMode(UINT8 bPath);

//static function for update regiter
//static void _vScpipSetUSReg(UINT32 u4VdpId);
static void _vScpipSetMirrorFlipReg(UINT32 u4VdpId);
static void vScpipSetColorTranInCTI(UINT8 bPath, UINT32 u4Is444, UINT32 u4Is422Mode);
static UINT8 _vScpicIsOneToOneAble(UINT32 u4VdpId);

//MT5396 Legacy......static function for calculate scaling factor
//static void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 in, UINT32 out, UINT32 u4LimitLength, UINT8 bDircection);
//static void _vScpipCalNewScale(UINT32 u4VdpId);

//static function for frame track
static void _vScpipSetFrameTrackOnOff(UINT32 u4OnOff);
static void _vScpipConfigFrameTrack(UINT32 u4Step);

#if defined(DZONE_DETECTION_EN)
static void _vScpipDoDZoneDetection(UINT8 bPath);
#endif

#if defined(CC_MT5399)
EXTERN void vScpipConfigPOCLK(UINT8 mode);
#endif
#if defined(SCPIP_SUPPORT_POST_SCALER)
void vScpipCheckSetDataTrigger(void);
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

UINT32 _au4OriFrameNum[VDP_NS] = {3, 3};
// When TVE on, force dispOff
UINT32 _u4ForceDispOff[2]={SV_FALSE, SV_FALSE};
static UINT32 _u4ScposImportNotReady[VDP_NS] = {0, 0};

//#define CC_SCPOS_LOG_DRAM_PARAM (50)
#ifdef CC_SCPOS_LOG_DRAM_PARAM
static UINT32 _au4TimeLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _au4AddressLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _au4ValueLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _u4LogIdx = 0;
#endif

static UINT32 _u4ScpipFrameTrackTarget = FRAME_TRACK_DEFAULT;

UINT8 u1SetScalingCoef = 1;
void vScpipSetScalingCoef(UINT8 u1En)
{
    u1SetScalingCoef = u1En;
}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
UINT32 u4ScpipGetFrameTrackTargetVar(void)
{
    return _u4ScpipFrameTrackTarget;
}

void vScpipSetFrameTrackTargetVar(UINT32 target)
{
    _u4ScpipFrameTrackTarget = target;
}

/**
 * @brief Re-calculate delay counter by input/output frame rate ratio
 * @param u4VdpId  
 * @param u4Delay delay counter for input frame rate = output frame rate
 * @return the delay counter for output domain
 */
// TODO: change back to static
#ifdef CC_SCPOS_3DTV_SUPPORT
UINT32 _u4ScpipReCalDelayCounter(UINT32 u4VdpId, UINT32 u4Delay)
{
    UINT8  bPath;
    UINT32 u4InputFrameRate;
    UINT32 u4OutputFrameRate;

    bPath = getScalerPath(u4VdpId);
    u4InputFrameRate = bDrvVideoGetRefreshRate(bPath);
    u4OutputFrameRate = vDrvGetLCDFreq();
    
    if(u4InputFrameRate == 0)
    {
        return u4Delay;
    }

    if(PANEL_IsSupport120Hz())
    {
        u4OutputFrameRate >>= 1;
    }

    u4Delay = ((u4OutputFrameRate * u4Delay) + (u4InputFrameRate - 1))/u4InputFrameRate;
    return u4Delay;
}
#endif
#ifdef CC_COPLAT_MT82
UINT8  _bBlankMuteCnt;

UINT8 bDrvIsScalerFlagCleared(UINT8 bPath)
{
    if(bPath == SV_VP_MAIN){
        if(fgIsScposFlgSet(MAIN_DISP_PRM_CHG | SCALER_MAIN_MODE_CHG))
        {
            return SV_FALSE;
        }
        else
        {
            return SV_TRUE;
        }
    }
    else
    {
        if(fgIsScposFlgSet(PIP_DISP_PRM_CHG | SCALER_PIP_MODE_CHG))
        {
            return SV_FALSE;
        }
        else
        {
            return SV_TRUE;
        }        
    }

}

void vDrvSetFastBlank(UINT8 bOnOff, UINT8 bCount)
{
    if(bOnOff == SV_ON && bCount > 0)
    {
        if(bCount > _bBlankMuteCnt)
        {
            _bBlankMuteCnt = bCount;
        }
        vIO32WriteFldAlign(MUTE_00, 1,  R_MUTE_POST_EN);
    }
    else
    {
        _bBlankMuteCnt = 0;
        vIO32WriteFldAlign(MUTE_00, 0,  R_MUTE_POST_EN);
    }
}

/**
 * @brief  to know if video can be freezed
 * @param  bPath path info.(SV_VP_MAIN/SV_VP_PIP)
 * @retval void
 */
UINT8 bIsVideoFreezable(UINT8 bPath)
{
    UINT8 mode;
    mode = u1ScpipGetDispMode(bPath);
    
    if(mode != VDP_SCPOS_DISPMODE_OFF)  //can only freeze under dram mode
    {
        return SV_FALSE;
    }
    else
    {
        return SV_TRUE;
    }
}
#endif

/**
 * @brief Set scpos delay enable time
 * @param ucVdpId VDP_1/VDP_2
 * @param ucEnable Enable/Disable
 * @param u4DelayCount Delay time
 */
void vScpipDelayEnable(UCHAR ucVdpId, UCHAR ucEnable, UINT32 u4DelayCount)
{
    LOG(3, "vScpipDelayEnable(%d) %d\n", ucVdpId, ucEnable);

    if (ucVdpId >= VDP_NS)
    {
        return;
    }
       
    if (ucEnable)
    {
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PLA_EN, ucVdpId, u4DelayCount, FALSE);
        _VDP_ScposEnable(ucVdpId, TRUE);
    }
    else
    {
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PLA_EN, ucVdpId, FOREVER_MUTE, FALSE);
        _VDP_ScposEnable(ucVdpId, FALSE);
    }
}

/**
 * @brief Set scpos enable/disable
 * @param ucVdpId VDP_1/VDP_2
 * @param ucEnable Enable/Disable
 */
static void _VDP_ScposEnable(UCHAR ucVdpId, UCHAR ucEnable)
{
    _arScalePrm[ucVdpId].u4Enable = ucEnable;    

    if (ucEnable)
    {
        //do...
    }
    else
    {
        vScpipWriteCtrlOnOff(ucVdpId, SV_OFF);
    }
} 

#ifdef CC_CLI
/**
 * @brief Print current scale paramters
 * @param ucVdpId VDP_1/VDP_2
 */
void vScpipPrintCurrentScale(UCHAR ucVdpId)
{
    UINT8 bPath;
    RVChannel* channel;
    RDispInfo* dispInfo;
    RPicInfo* picInfo;
    UINT8 isChannelOn;
    PREDOWN_INFO_T* pdsInfo;
    VDP_SCALER_PRM_T* scalerInfo;

    bPath = getScalerPath(ucVdpId);
    channel = getChannel(bPath);
    isChannelOn = channel->bIsChannelOn;        
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    pdsInfo = getPDSInfo(bPath);
    scalerInfo = getScalerInfo(bPath);

    if (isChannelOn != SV_OFF)
    {
        Printf("SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
                ucVdpId,  
                dispInfo->wXOffset,
                dispInfo->wYOffset,
                dispInfo->wWidth,
                dispInfo->wHeight,
                getMixedHPorch(ucVdpId),
                getMixedVPorch(ucVdpId),
                picInfo->wTargetW,
                picInfo->wTargetH);

        Printf("H(0x%x, 0x%x-0x%x) V(0x%x, 0x%x-0x%x)\n",
                pdsInfo->u4HPDSFactor, scalerInfo->u4HDeltaPhaseIntg, scalerInfo->u4HDeltaPhaseFrac,
                pdsInfo->u4VPDSFactor, scalerInfo->u4VDeltaPhaseIntg, scalerInfo->u4VDeltaPhaseFrac);
    }
}
#endif

void vScpipDumpResInfo(UINT8 bPath)
{
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    VDP_SCALER_PRM_T* scalerInfo;

    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    scalerInfo = getScalerInfo(bPath);
    
    Printf("[%s] info:\n", bPath==SV_VP_MAIN?"MAIN":"SUB");
    Printf("=============================================\n");
    Printf("[picInfo]\n");
    Printf("input htotal.............%5d\n",picInfo->wHTotal);
    Printf("input vtotal.............%5d\n",picInfo->wVTotal);
    Printf("input width..............%5d\n",picInfo->wSrcW);
    Printf("input height.............%5d\n",picInfo->wSrcH);    
    Printf("input xOffset............%5d\n",picInfo->wXOffset);
    Printf("input yOffset............%5d\n",picInfo->wYOffset);
    Printf("input TargetW............%5d\n",picInfo->wTargetW);
    Printf("input TargetH............%5d\n",picInfo->wTargetH);
    
    Printf("UI H porch...............%5d\n",picInfo->wUIHPorch);
    Printf("UI V porch...............%5d\n",picInfo->wUIVPorch);
    Printf("Mixed H porch............%5d\n",getMixedHPorch(bPath));
    Printf("Mixed V porch............%5d\n",getMixedVPorch(bPath));    
       
    Printf("[dispInfo]\n");
    Printf("output xOffset...........%5d\n",dispInfo->wXOffset);
    Printf("output yOffset...........%5d\n",dispInfo->wYOffset);
    Printf("output width.............%5d\n",dispInfo->wWidth);
    Printf("output height............%5d\n",dispInfo->wHeight);    

    Printf("[FSC window mask]\n");
    Printf("X........................%5d\n",scalerInfo->u4OutputX);
    Printf("Y........................%5d\n",scalerInfo->u4OutputY);
    Printf("Width....................%5d\n",scalerInfo->u4OutputWidth);
    Printf("Height...................%5d\n",scalerInfo->u4OutputHeight);
    
    Printf("[Scaling Factor]\n");
    Printf("H Intg...................0x%5x\n",scalerInfo->u4HDeltaPhaseIntg);
    Printf("H Frac...................0x%5x\n",scalerInfo->u4HDeltaPhaseFrac);
    Printf("V Intg...................0x%5x\n",scalerInfo->u4VDeltaPhaseIntg);
    Printf("V Frac...................0x%5x\n",scalerInfo->u4VDeltaPhaseFrac);    

    #if 0 //defined(CC_MT5399)
    vPSCDumpResInfo();
    #endif
}

/**
 * @brief Get scpos porch
 * @param bPath VDP_1/VDP_2
 * @param bPorchType porch type
 * @return scpos porch
 */
UINT16 _VDP_ScposGetPorch(UINT8 bPath, UINT8 bPorchType)
{
    RPicInfo* picInfo;
    picInfo = getPicInfo(bPath);

    switch(bPorchType)
    {
        case SV_HPORCH_CURRENT:
            return picInfo->wUIHPorch;
        case SV_HPORCH_DEFAULT:
            return 1000;
        case SV_VPORCH_CURRENT:
            return picInfo->wUIVPorch;
        case SV_VPORCH_DEFAULT:
            return 1000;
        case SV_HPORCH_MAX:
            return 1000 + picInfo->wSrcW - picInfo->wTargetW - picInfo->wXOffset;
        case SV_VPORCH_MAX:
            return 1000 + picInfo->wSrcH - picInfo->wTargetH - picInfo->wYOffset;
        case SV_HPORCH_MIN:
            return  1000 - picInfo->wXOffset;
        case SV_VPORCH_MIN:
            return  1000 - picInfo->wYOffset;
        default: /*MinMax*/
            return 1000;
    }
}


/**
 * @brief Set scpos porch
 * @param bPath VDP_1/VDP_2
 * @param bPorchType porch type
 * @param wValue porch value
 */
void _VDP_ScposSetPorch(UINT8 bPath,UINT8 bPorchType,UINT16 wValue)
{
    /* trigger from UI*/
    RPicInfo* picInfo;
    UINT32 u4Update;

    picInfo = getPicInfo(bPath);
    u4Update = 0;

	if(bPath == SV_VP_PIP)
	{
		return ;
	}

    if (bPorchType==SV_HPORCH_CURRENT)
    {
        if (wValue != picInfo->wUIHPorch) 
        {
            picInfo->wUIHPorch = wValue;
            u4Update = 1;
        }
    }
    else
    {
        if (wValue != picInfo->wUIVPorch) 
        {
            picInfo->wUIVPorch = wValue;
            u4Update = 1;
        }
    }

    if (u4Update)
    {
        vScpipReconfig((UINT32)bPath);
    }

}

/**
 *  @Cobra porting done
 */
void vScpipSetMirror(UINT8 bPath, UINT8 bOnOff)
{
    if(_arScalePrm[bPath].u4MirrorEnable != bOnOff)
    {
        _arScalePrm[bPath].u4MirrorEnable = bOnOff;
    }
}

/**
 *  @Cobra porting done
 */
void vScpipSetFlip(UINT8 bPath, UINT8 bOnOff)
{
    if(_arScalePrm[bPath].u4FlipEnable != bOnOff)
    {
        _arScalePrm[bPath].u4FlipEnable = bOnOff;
    }
}

/**
 *  @Cobra porting done
 */
void vScpipSetMirrorFlipAll(UINT8 bOnOff)
{
    vScpipSetMirror(SV_VP_MAIN, bOnOff);
    vScpipSetMirror(SV_VP_PIP, bOnOff);

    vScpipSetFlip(SV_VP_MAIN, bOnOff);
    vScpipSetFlip(SV_VP_PIP, bOnOff);
}

#ifdef CC_FLIP_MIRROR_SUPPORT
/**
 * @brief Set mirror flip mode
 * @param bPath VDP_1/VDP_2
 * @param bDramMode SV_TRUE/SV_FALSE 
 */
void vScpipConfigFlip(UINT8 bPath)
{
    // if(bDramMode ==SV_TRUE) // do flip only when dram mode
    if(u4GetFlipMirrorModule(bPath)==FLIP_BY_SCALER)
    {
        UINT8 u1Config;

        u1Config = u1GetFlipMirrorConfig();

        if((u1Config & SYS_MIRROR_CONFIG_ON))
        {
            vScpipSetMirror(bPath, SV_ON);
            LOG(4, "vScalerConfigFlip(%d): MIRROR ON\n", bPath);
        }    
        else 
        {
            vScpipSetMirror(bPath, SV_OFF);
            LOG(4, "vScalerConfigFlip(%d): MIRROR OFF\n", bPath);
        }

        if((u1Config & SYS_FLIP_CONFIG_ON) /*&& !(ucEnable != SV_FALSE && bPath == SV_VP_PIP)*/)
        {
            vScpipSetFlip(bPath, SV_ON);
            LOG(4, "vScalerConfigFlip(%d): FLIP ON\n", bPath);
        }
        else
        {
            vScpipSetFlip(bPath, SV_OFF);
            LOG(4, "vScalerConfigFlip(%d): FLIP OFF\n", bPath);
        }
    }
    else
    {
        vScpipSetMirror(bPath, SV_OFF);
        vScpipSetFlip(bPath, SV_OFF);
        LOG(4, "vScalerConfigFlip(%d): Both FLIP and MIRROR OFF\n", bPath);
    }

    vScpipReconfig(getScalerVDP(bPath));
}
#endif

/*******************
 * Frame track API *
 *******************/
void vScpipFrameTrackConfig(UINT32 u4Step) 
{
    _vScpipConfigFrameTrack(u4Step);
    #if defined(SCPIP_SUPPORT_POST_SCALER)
        //To avoid panel sync keep moving. Enlarge PSC frame track step size. 
        vPscConfigFrameTrack(u4Step<<1);    
    #endif    
}

extern UINT32 GCD(UINT32 a, UINT32 b);


/**
 *  @cobra need review
 */
static void _vScpipConfigFrameTrack(UINT32 u4Step)
{
    UINT8 u1InFR;
    UINT8 u1OutFR;
    UINT8 u1TrackVNum;
    UINT32 u4TrackTargetVal;
    UINT8 bMode;
    UINT32 u4GCD;
    UINT8 fgLRTrack;
    UINT8 bLockWin;
    VRM_INFO_T* inRes;

    /**************************************
     ** Determine Frame track parameter  **
     *************************************/

    bLockWin = 2;
    fgLRTrack = SV_OFF;
    bMode = getScalerMode(SV_VP_MAIN);

    //1. Get input Frame rate
    inRes = VRMGetVRMInfo(SV_VP_MAIN, (bMode == VDP_SCPOS_PSCAN_DISPMODE?VRM_MODULE_MIB:VRM_MODULE_FSC));
    u1InFR= inRes->u2FrameRate;    
    //2. Get output Frame rate
    u1OutFR = u2DrvGetInternalFrameRate();
    //3. calculate GCD
    u4GCD = GCD(u1InFR, u1OutFR);
        
    //error handling
    if ((bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE) || (u1InFR==0) || (u1OutFR==0) || (u4GCD ==0))
    {
    	return;
    }
    
    //4. calculate track interval        
    u1TrackVNum = u1InFR / u4GCD;

    //5. Determine frame track line
    if (u4ScpipGetFrameTrackTargetVar() != FRAME_TRACK_DEFAULT)
    {
        u4TrackTargetVal = u4ScpipGetFrameTrackTargetVar();
    }
    else
    {
        #if defined(CC_MT5399) 
        //Reduce frame delay in super game mode
        if(VDP_RTNeedSuperGameMode(VDP_1) && !u1BypassWFDEnhance)
        {
        	u4TrackTargetVal = _arOutTimingPrm.u2PanelVTotal /4; 
        }
        else
        {
        	u4TrackTargetVal = _arOutTimingPrm.u2PanelVTotal /8; 
        }
        #else
		#if defined(CC_MT5882)
        //Reduce frame delay in super game mode
        if(VDP_RTNeedSuperGameMode(VDP_1) && !u1BypassWFDEnhance)
        {
        	u4TrackTargetVal = wDISPLAY_HEIGHT / 4;
        }
        else
        {
        	u4TrackTargetVal = wDISPLAY_HEIGHT / 8;
        }
		#endif 
        #endif    
    }

    //6. Determine track based on L/R
    if((bMode == VDP_SCPOS_PSCAN_DISPMODE)
        &&((E_TD_IN_FS_I==TD_MIB_IN(u4DrvTDTVDIModeQuery()))
            ||(E_TD_IN_FS_P==TD_MIB_IN(u4DrvTDTVDIModeQuery()))))    
    {
        fgLRTrack = SV_ON;

    }

    //7. Calculate Lock/Tor window size
    bLockWin *= u1TrackVNum;

    if(fgLRTrack==SV_ON)
    {
        bLockWin = bLockWin<<1;
    }

    /**************************************
     ** Set Frame track register         **
     *************************************/    
    
    //1. set frame track interval
    vScpipSetFrmTrkFrames(u1TrackVNum);  

    //2. set frame track target.
	vScpipSetFrmTrkLines(u4TrackTargetVal);
    
    //3. set reference vsync
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        //in Pscan dispmode, reference vsync before Pscan(Input Vsync)
        vScpipSelFrmTrkRefVsync(0);
    }
    else
    {
        //in other mode, reference vsync of scaler input
        vScpipSelFrmTrkRefVsync(1);
    }
	//5. set frame track based on L/R
    vScpipFrmTrkInLREnable(fgLRTrack);
	
	//6. set frame track lock/tor window size.
    vScpipSetFrmTrkLockWin(bLockWin);
    vScpipSetFrmTrkLockTor(bLockWin);
}


/**
 *  @cobra review done
 */
void vScpipSwitchFrameTrackOnOff(UINT32 u4FscOnOff, UINT32 u4PscOnOff) 
{
    // can be enable when dram mode and scaler display mode
    static UINT32 u4FscStatus = 0xFF;
    static UINT32 u4PscStatus = 0xFF;

    if (u4FscStatus != u4FscOnOff || u4PscStatus != u4PscOnOff)
    {
        u4FscStatus = u4FscOnOff;
        u4PscStatus = u4PscOnOff;
        LOG(5, "vScpipSwitchFrameTrackOnOff %d %d\n\n", u4FscOnOff, u4PscOnOff);
    }
    else
    {
        return; // no change
    }
    //1. set FSC frame track on/off
    _vScpipSetFrameTrackOnOff(u4FscOnOff);

    //2. set PSC frame track on/off
    #if defined(SCPIP_SUPPORT_POST_SCALER)
        vPscSetFrameTrackOnOff(u4PscOnOff);
    #endif
}

/**
  * @cobra review done
  */
static void _vScpipSetFrameTrackOnOff(UINT32 u4OnOff)
{
    #if defined(__MODEL_slt__) || defined(DRV_SUPPORT_EXTMJC) // Frame tracking always off for SLT and external MJC
    vScpipSetFrmTrkStep(0);
    vScpipFrmTrkDddsEnable(SV_OFF);
    #else
    if (u4OnOff)  // notice the order
    { 
        if(fgFrameTrackIsFlgSet(FRAME_TRACK_MODE_DDDS_BY_FNTSCALER)) // DDDS frame track mode
        {
            //set to DDDS Adjust mode        
            vScpipFrmTrkDddsEnable(SV_ON);
            vScpipSetFrmTrkStep(0);
        }
        else 
        {
            //set to V Total Adjust mode         
            vScpipFrmTrkDddsEnable(SV_OFF);
            vScpipSetFrmTrkStep(5);
        }    
    }
    else
    {
        vScpipFrmTrkDddsEnable(SV_OFF);
        vScpipSetFrmTrkStep(0);
    }
    #endif    
}

void vScpipOnInputVSync(UINT8 bPath)
{
    if (bPath >= SV_VP_NA)
    {
        return;
    }
    vScpipResetDramCountDown(bPath);
}

#ifdef CC_SCPOS_3DTV_SUPPORT
void vScpipOnTVEInputVSync(void)
{
}
#endif

void vScpipOnVdoOutputVSync(void)
{  
}
#if defined(SCPIP_SUPPORT_POST_SCALER)

void vScpipOnMJCInputVSync(void)
{
   	// #if defined(CC_MT5399)
    //vScpipCheckSetDataTrigger();
    //#endif
}

UINT32 _u4MjcVSyncCountSD=0;
void vScpipCheckSetDataTrigger(void)
{
    UINT32 u4InputFrameRate = u2DrvGetInternalFrameRate();
    UINT32 u4Cnt = 1;
    _u4MjcVSyncCountSD++;
    if(u4InputFrameRate == 0)
    {
        u4Cnt = 1;
    }
    else
    {
        u4Cnt = (vDrvGetLCDFreq() + u4InputFrameRate -1 )/u4InputFrameRate;
    }
    if(bScpipIsNeedSetData() == SV_TRUE)
    {
        if(_u4MjcVSyncCountSD%(u4Cnt*3) == 0)
        {
            vScpipUpdateReg_SETDATA();                              //set FSC reg
            vScpipClearNeedSetDataFlag();                           //clear FSC flag
            LOG(3,"FSC Set data trigger. _u4MjcVSyncCountSD=%d\n", _u4MjcVSyncCountSD);
            vScpipSetData(1);       			                    //trigger FSC set data
        }        
    }
}
#endif

BOOL bScpipIsNeedSetData(void)
{
    BOOL ret = SV_FALSE;
    //VDP_SCALER_PRM_T* scalerInfo;
    //UINT8 bIndex;

    #if defined(CC_MT5399)
    if(_arOutTimingPrm.bUpdateTGLater == SV_TRUE)
    {
        ret = SV_TRUE;
    }
    #endif  

    return ret;
}

void vScpipClearNeedSetDataFlag(void)
{
    //VDP_SCALER_PRM_T* scalerInfo;
    //UINT8 bIndex;

    #if defined(CC_MT5399)
    _arOutTimingPrm.bUpdateTGLater = SV_FALSE;
    #endif    
}

void vScpipUpdateReg_SETDATA(void)
{
    //VDP_SCALER_PRM_T* scalerInfo;
    //UINT8 bIndex;
    #if defined(CC_MT5399)
    LOG(3,"vScpipUpdateReg_SETDATA _u4MjcVSyncCountSD(%d)\n",_u4MjcVSyncCountSD);
    #else
    LOG(3,"vScpipUpdateReg_SETDATA _u4VSyncCountSD(%d)\n",_u4VSyncCountSD);
    #endif
        
    #if defined(CC_MT5399)

    
    if(_arOutTimingPrm.bUpdateTGLater == SV_TRUE)
    {
        LOG(1, "Set FSC TG IN OUT ISR.\n");
        //update Front scaler Time gen
        vScpipSetSCTimeGen(_arOutTimingPrm.u2PanelHTotal,
                            _arOutTimingPrm.u2PanelHActive,
                            _arOutTimingPrm.u2PanelVTotal,
                            _arOutTimingPrm.u2PanelVActive);
        vScpipSetSCTimeGenPorch(_arOutTimingPrm.u2PanelHFPorch,
                                _arOutTimingPrm.u2PanelHBPorch,
                                _arOutTimingPrm.u2PanelVFPorch,
                                _arOutTimingPrm.u2PanelVBPorch);
    }
    #endif
}

void vScpipOnDIOutputISR(void)
{
}


/**
 * @brief Sw Initialize 
 * @6896 wait review
 */
void vScpipSwInit(void)
{
    getScalerInfo(SV_VP_MAIN)->bUpdateOutXYWHLater = SV_FALSE; 
    getScalerInfo(SV_VP_PIP)->bUpdateOutXYWHLater = SV_FALSE; 
}

/**
 * @brief Set dynamic scaler
 * @param ucVdpId VDP_1/VDP_2
 * @param ucOnOff On/Off
 * @param u4ResponseTime response time
 * @6896 remove this function
 */
void vScpipSetDynamicScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime)
{
    //removed
}
/******************************************************************
 **                  Freeze-related API                          **
 *****************************************************************/

/**
 * @brief Check if video is freezable
 * @param bPath VDP_1/VDP_2
 */
UINT8 u1ScpipIsVideoFreezable(UINT8 bPath)
{
    UINT8 scalerMode;
    UINT8 ret;

    ret = SV_TRUE;
    scalerMode = u1ScpipGetDispMode(bPath);

    // If scaler is in scaler dispmode or pscan dispmode, can't support freeze function.
    if(scalerMode != VDP_SCPOS_DISPMODE_OFF)
    {       
        ret = SV_FALSE;
    }

    return ret;
}
 
/**
 * @brief vScpipSetFreeze
 * Freeze Video in scalar
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         bOnOff: SV_ON/SV_OFF
 * @retval void
 * @Cobra review done.
 */
void vScpipSetFreeze(UINT8 bPath, UINT8 bOnOff) 
{
    CRIT_STATE_T csState;
    UINT8 freezeDelay=0;
    RVChannel* channel;

    csState = x_crit_start();

    channel = getChannel(bPath);

    if (bPath == SV_VP_MAIN) 
    {
#if (MAIN_FREEZE_DELAY)
		if(FBM_GetFixedScalerDram() == SV_TRUE) //atv pvr delay 3 v to freeze will freeze garbage
		{
			freezeDelay = MAIN_FREEZE_DELAY - 1;
		}
		else
		{
			freezeDelay = MAIN_FREEZE_DELAY;
		}
#else
        freezeDelay = 0;
#endif
    }
    else if(bPath == SV_VP_PIP)
    {
#if (PIP_FREEZE_DELAY)
        freezeDelay = PIP_FREEZE_DELAY;
#else
        freezeDelay = 0;
#endif
    }        

    if(bOnOff == SV_ON)
    {
        channel->bIsGoingToFreeze = freezeDelay;
    }
    else
    {
        channel->bIsGoingToFreeze = 0;
        channel->bIsFreeze = 0;
    }

    x_crit_end(csState);
}

/**
 * @brief Handle channel freeze
 * @param u4VdpId VDP_1/VDP_2
 */
static void _vScpipHandleChannelFreeze(UINT32 u4VdpId)
{  
    UINT8 bPath;   
    RVChannel* channel;
    UINT8 isChannelOn;

    bPath = getScalerPath(u4VdpId);
    channel = getChannel(bPath);
    isChannelOn = channel->bIsChannelOn;

    if(isChannelOn == SV_ON && channel->bIsGoingToFreeze!=0 && _arScalePrm[u4VdpId].u4Enable)
    {    
        // wait for some VSync to fill the whole scaler frame buffers before freezing
        UINT8 bMute;
        LOG(3, "Channel is going to freeze: %d\n", channel->bIsGoingToFreeze);
        bMute =  ((u4VdpId == VDP_1) ? fgMainMuteOn() : fgPIPMuteOn());

        if (bMute)
        {
            LOG(3, "Channel is still in mute mode\n");
        }
        else
        {
            channel->bIsGoingToFreeze --;
            if (channel->bIsGoingToFreeze == 0)
            {
                LOG(3, "Channel freeze on\n");
                channel->bIsFreeze = SV_ON;
            }
        }
    }    
}

/**
 * @brief Set pre-scaling down factor
 * @param u4VdpId VDP_1/VDP_2
 * @param u4Factor pre-scaling down factor
 * @Cobra porting done
 */
void vScpipSetForcedHpsFactor(UINT32 u4VdpId,UINT32 u4Factor) 
{
    UINT8 bPath;
    PREDOWN_INFO_T* pdsInfo;

    bPath = getScalerPath(u4VdpId);
    pdsInfo = getPDSInfo(bPath);

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    pdsInfo->u4ForcedHPDS = u4Factor;

    vVRMSetEventFlg(bPath, VRM_EVENT_BY_FORCE_PDS);
}

/**
 * @brief Set pre-scaling down factor
 * @param u4VdpId VDP_1/VDP_2
 * @param u4Factor pre-scaling down factor
 * @Cobra porting done
 */
void vScpipSetForcedVpsFactor(UINT32 u4VdpId,UINT32 u4Factor) 
{
    UINT8 bPath;
    PREDOWN_INFO_T* pdsInfo;

    bPath = getScalerPath(u4VdpId);
    pdsInfo = getPDSInfo(bPath);

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    pdsInfo->u4ForcedVPDS = u4Factor;

    vVRMSetEventFlg(bPath, VRM_EVENT_BY_FORCE_PDS);
}

void vScpipSetVpsPhaseBottomEn(UINT32 u4VdpId, UINT32 u4En) 
{
    UINT8 bPath;
    PREDOWN_INFO_T* pdsInfo;

    bPath = getScalerPath(u4VdpId);
    pdsInfo = getPDSInfo(bPath);

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    pdsInfo->u4VPDSPhaseBottomEn = u4En;

    vPDSSetModeChanged(bPath);
    vDrvSetImportWaitFlag(SV_VP_MAIN, VRM_IMPORT_WAIT_FLAG_PDS);
}

/**
 *  @Cobra review done
 */
UINT16 u2ScpipGetDispWidth(UINT8 bPath)
{    
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath);
    return (UINT16)scalerInfo->u4OutputWidth;
}
/**
 *  @Cobra review done
 */
UINT16 u2ScpipGetDispHeight(UINT8 bPath)
{            
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath);
    return (UINT16)scalerInfo->u4OutputHeight;
}

/**
 *  @Cobra review done
 */
void vScpipForceDispOff(UINT32 u4Path, UINT32 u4OnOff)
{
    if (_u4ForceDispOff[u4Path] != u4OnOff)
    {
        _u4ForceDispOff[u4Path] = u4OnOff;
    }
}
/**
 *  @Cobra review done
 */
void vScpipForceDispOff_Ext(UINT32 u4Path,UINT32 u4DispOffModule, UINT32 u4OnOff)
{
    static UINT32 u4ForceDispOff[2] ={0,0};

    if(u4DispOffModule > DRV_DISP_OFF_MODULE_MAX)
    {
        return;
    }
    
    if(u4OnOff==TRUE)
    {
        u4ForceDispOff[u4Path] |= (1<<u4DispOffModule);
    }
    else
    {
        u4ForceDispOff[u4Path] &=~(1<<u4DispOffModule);
    }

    if(u4ForceDispOff[u4Path] > 0)
    {
        _u4ForceDispOff[u4Path] = SV_TRUE;
    }
    else
    {
        _u4ForceDispOff[u4Path] = SV_FALSE;
    }
    
    return; 
}

/**
 * @brief Set mode change flag
 * @param ucVdpId VDP_1/VDP_2
 * @6896 review done
 */
void vScpipReconfigScl(UINT32 u4VdpId)
{
    if (u4VdpId == VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else if (u4VdpId == VDP_2)
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }

    #if defined(SCPIP_SUPPORT_POST_SCALER)
        vPscFireModeChanged();
    #endif

}


/**
 * @brief Set mode change flag
 * @param ucVdpId VDP_1/VDP_2
 * @6896 review done
 */
void vScpipReconfig(UINT32 u4VdpId)
{
    vScpipReconfigScl(u4VdpId);

    //fire VRM mode change,too.
    fireVRMModeChange(getScalerPath(u4VdpId), VRM_EVENT_BY_SCPOS);
}

UINT8 u1DrvScpipGetFrmDelay10x(UINT32 u4VdpId)
{
    UINT8 u1Delay = 0;
    UINT8 u1Mode, u1Path;

    u1Path = getScalerPath(u4VdpId);
    u1Mode = u1ScpipGetDispMode(u1Path);

    if(u1Mode != VDP_SCPOS_DISPMODE_OFF)        // display mode on, there is no frame delay
    {
        u1Delay= 0;
    }
    else                                        // scaler dram mode
    {
        if (_arScalePrm[u1Path].u4FlipEnable)   // scaler flip
        {
            u1Delay= 20;  //delay 2 frame
        }
        else
        {
            u1Delay= 10; // delay 1 frame
        }
    }

    return u1Delay;
}

/**
 * @brief Get pre-scaling down width
 * @param bPath VDP_1/VDP_2
 * @return pre-scaling down width
 * @6896 porting done
 */
UINT32 u4ScpipGetPreScaleWidth(UINT8 bPath)
{
    UINT16 u2Width;
    UINT32 u4Factor;
    RPicInfo* picInfo;
    
    picInfo = getPicInfo(bPath);
    u4Factor = u4PDSGetHPSFactorReg(bPath);
    
    if (u4Factor == DFT_HPD_FACTOR)
    {
        u2Width = picInfo->wSrcW;
    }
    else
    {
        u2Width = (((UINT32)(picInfo->wSrcW+1) * u4Factor)/ DFT_HPD_FACTOR);
    }
    return u2Width;
}

/**
 * @brief Set dot by dot mode
 * @param ucVdpId VDP_1/VDP_2
 * @param ucOnOff On/Off
 * @6896 Review done
 */
void vScpipSet121Map(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
    LOG(2, "Set 121 Map to (%d)\n",ucOnOff);
    if (_arScalePrm[ucVdpId].u4121Map != (UINT32) ucOnOff) 
    {
        // 121-map & nonlinear scaling can NOT enable together
        if (ucOnOff)
        {
            _arScalePrm[ucVdpId].u4NonlinearEn = 0;
        }

        _arScalePrm[ucVdpId].u4121Map = (UINT32) ucOnOff;

        vScpipReconfig((UINT32)ucVdpId);
    }
}

/**
 * @6896 Review done
 */
UINT32 vScpipGet121Map(UCHAR ucVdpId)
{
    return _arScalePrm[ucVdpId].u4121Map;
}


void vScpipSet121MapHPos(UCHAR ucVdpId, UINT16 u2HPos)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if(_arScalePrm[ucVdpId].u2121MapHPos != u2HPos)
    {
    	_arScalePrm[ucVdpId].u2121MapHPos = u2HPos;
   	vScpipReconfig((UINT32)ucVdpId);
    }
}

static UINT8 _vScpicIsOneToOneAble(UINT32 u4VdpId)
{
    UINT8 ret = SV_TRUE;
    UINT8 bPath;
    UINT32 u4ScalerMode;
    
    UINT32 u4InputWidth;
    
    bPath = getScalerPath(u4VdpId);
    u4ScalerMode = u1ScpipGetDispMode(bPath);    

    u4InputWidth = (UINT32)wDrvVideoInputWidth(bPath); 
    if (bDrvVideoGetWidthOverSample(bPath))    
    {         
        u4InputWidth = u4InputWidth >> 1;
    }
    
    if((u4ScalerMode == VDP_SCPOS_LINESYNC_DISPMODE) && vScpipIs3DDoneByMJC()==SV_FALSE)    // display mode
    {
        ret = SV_FALSE;
    }   

    if((u4InputWidth > wDISPLAY_WIDTH) || (wDrvVideoInputHeight(bPath) > wDISPLAY_HEIGHT) || (u4InputWidth == 0 ) || (wDrvVideoInputHeight(bPath) == 0 )  )
    {
       ret= SV_FALSE;
    }    
   
    return ret;
}


void vScpipUpdate121MapEn(UINT32 u4VdpId)
{   
    // handle 121-mapping
    _arScalePrm[u4VdpId].u4121MapEn = SV_OFF;
    if(_arScalePrm[u4VdpId].u4121Map == SV_ON)
    {
        if(_vScpicIsOneToOneAble(u4VdpId) == SV_TRUE)
        {
            _arScalePrm[u4VdpId].u4121MapEn = SV_ON;
            LOG(3,"121 is True and can be enabled.\n");
        }
        else
        {
            LOG(3,"Unable to do 121\n");
        }        
    }
}

/**
 * @brief Set 10/8 bit mode
 * @param u4VdpId VDP_1/VDP_2
 * @param u4Mode 10/8 bit, or 0 means not forced.
 * @6896 porting done
 */
void _VDP_Set10bitMode(UINT32 u4VdpId, UINT32 u4Mode)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[u4VdpId].u4Forced10BitMode != u4Mode)
    {
        _arScalePrm[u4VdpId].u4Forced10BitMode = u4Mode;

        vScpipReconfig(u4VdpId);
    }
}

/**
 *  @6896 porting done
 */
UINT32 u4ScpipGetForced10BitMode(UINT32 u4VdpId)
{
    return _arScalePrm[u4VdpId].u4Forced10BitMode;
}

/**
 *  @Cobra porting done
 */
UINT32 u4ScpipGet10BitMode(UINT32 u4VdpId)
{
    UINT32 u410BitMode = 0xffffffff;
    UINT32 u4Forced10BitMode;

    u4Forced10BitMode = u4ScpipGetForced10BitMode(u4VdpId);

    if(u4Forced10BitMode == 0) // FW auto
    {
        u410BitMode = _arScalePrm[u4VdpId].rFbmInfo.fg10bit;
    }
    else if (u4Forced10BitMode == VDP_SCPOS_8BitMode) // force 8 bit mode
    {
        u410BitMode = 0;
    }
    else if (u4Forced10BitMode == VDP_SCPOS_10BitMode) // force 10 bit mode
    {
        u410BitMode = 1;
    }            

    ASSERT(u410BitMode < 2);

    return u410BitMode;
}

UINT32 u4ScpipGetIsInput444Mode(UINT8 bPath)
{
    UINT32 ret;
    VDP_SCALER_PRM_T* scalerInfo;

    scalerInfo = getScalerInfo(bPath);
    ret = 0;
    
    if(scalerInfo->u4Forced444 == 0)
    {
        ret = (bIsScalerInput444(bPath) == SV_TRUE)? 1 :0;            
    }
    else
    {
        ret = (scalerInfo->u4Forced444==1)?1:0;
    }
    
    return ret;
}

void vScpipSetForced444Mode(UINT32 u4VdpId,UINT32 u4Mode)
{
    UINT8 bPath;
    VDP_SCALER_PRM_T* scalerInfo;

    if(u4VdpId>= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    scalerInfo = getScalerInfo(bPath);

    scalerInfo->u4Forced444 = u4Mode;
    vScpipReconfig(u4VdpId);    
}

void vScpipSetForced422Scaling(UINT8 bPath, UINT32 bIsForced422Scaling)
{
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath);

    //sub don't support 422 scaling
    if(bPath == SV_VP_PIP && bIsForced422Scaling==1)
    {
        return;
    }
    
    //0:auto, 1: force 422 scaing, 2: force 444 scaling
    scalerInfo->u4Forced422Scaling = bIsForced422Scaling;
    vScpipReconfig(bPath);
}

UINT32 u4ScpipGetIs422ScalingMode(UINT8 bPath)
{
    UINT32 ret;
    VDP_SCALER_PRM_T* scalerInfo;
    ret = 0;
    scalerInfo = getScalerInfo(bPath);

    //only main path support 422 scaling mode
    if(bPath == SV_VP_MAIN)
    {
        //if u4Forced422Scaling=Auto, determine by scalerInfo->u4Is444
        if(scalerInfo->u4Forced422Scaling == 0)
        {
            ret = (scalerInfo->u4Is444==0)?1:0;
        }
        else    //else use forced value;
        {
            ret = (scalerInfo->u4Forced422Scaling==1)?1:0;
        }        
    }

    return ret;
}

//0:422 1:444
UINT32 u4ScpipUpdateDramInputFmt(UINT8 bPath)
{
    UINT32 ret;
    VDP_SCALER_PRM_T* scalerInfo;

    ret = 0;    //0:422
    scalerInfo = getScalerInfo(bPath);

    if(getScalerMode(bPath) == VDP_SCPOS_DISPMODE_OFF)
    {
        //1. front mode & 444 scaling.
        if((scalerInfo->u4IsDramFrontMode==SV_TRUE && scalerInfo->u4Is422Mode==0) ||
            (scalerInfo->u4IsDramFrontMode==SV_FALSE))
        {
            ret = 1;    //1:444
        }
    }

    return ret;
}

void vScpipSetForcedDramWriteFmt(UINT8 bPath, UINT32 u4ForcedDramWriteFmt)
{
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath);

    scalerInfo->u4ForcedDramWriteFmt = u4ForcedDramWriteFmt;    
    LOG(3,"Set forced dram write fmt to %d\n", scalerInfo->u4ForcedDramWriteFmt);
    vScpipReconfig(bPath);    
}

//0:422 1:444
UINT32 u4ScpipGetDramWriteFmt(UINT8 bPath)
{
    UINT32 ret;
    VDP_SCALER_PRM_T* scalerInfo;

    ret = 0;    //0:422
    scalerInfo = getScalerInfo(bPath);

    if(getScalerMode(bPath) == VDP_SCPOS_DISPMODE_OFF)
    {
        if(scalerInfo->u4ForcedDramWriteFmt!=0)
        {
            if(scalerInfo->u4ForcedDramWriteFmt==1)     //force 444 mode
            {
                if((scalerInfo->u4IsDramFrontMode==SV_FALSE) ||
                   (scalerInfo->u4IsDramFrontMode==SV_TRUE && scalerInfo->u4Is422Mode==0))
                {
                    ret = 1;                            // **if front mode + 422 scaling, can't force to write 444
                }
            }
            else                                        //force 422 mode
            {
                ret = 0;                                //no matter input is 444 or 422, we can always use 422 to write dram.
            }
        }
        else if(u1IsMainOrSubVenc(bPath))
        {
            ret = 0;                                   //venc alway use 422
        }
        else                                           //auto
        {
            if(scalerInfo->u4IsDramFrontMode==SV_TRUE)  //front mode
            {
                if(scalerInfo->u4Is444==1 && scalerInfo->u4Is422Mode==0)    //input is 444 & do 444 scaling
                {
                    ret = 1;    //1:444
                }
            }
            else                                        //back mode
            {
                if(scalerInfo->u4Is444==1)              //input is 444
                {
                    ret = 1;    //1:444
                }
            }
        }
    }

    return ret;
}

//0:disable, 1:enable
UINT32 u4ScpipGetDram444to422En(UINT8 bPath)
{
    UINT32 ret;
    VDP_SCALER_PRM_T* scalerInfo;

    ret = 0;    //0:disable
    scalerInfo = getScalerInfo(bPath);

    //only enable when dram input fmt is 444 and dram write fmt is 422
    if(scalerInfo->u4DramInputFmt == 1 && scalerInfo->u4DramWriteFmt==0)
    {
        ret = 1;
    }

    return ret;
}

static void _vScpipSetDramLinePitchReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    VDP_SCALER_PRM_T* scalerInfo;
    
    bPath = getScalerPath(u4VdpId);
    scalerInfo = getScalerInfo(bPath);

    //1. Dram Line picth
    if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
    {
        SCALER_TDTV_IN_TYPE in = eScpipGetTdtvInType(u4VdpId);
        SCALER_TDTV_OUT_TYPE out = eScpipGetTdtvOutType(u4VdpId);
        
        if(in == SCALER_TDTV_IN_TYPE_SBS)
        {
            vScpipSetDramLinePitch(u4VdpId, (scalerInfo->u4DramLinePitch + 1)/2);
        }
        else if(out == SCALER_TDTV_OUT_TYPE_SBS)
        {
            // when SBS output, the max value of line pitch is 960/16=60
            vScpipSetDramLinePitch(u4VdpId, MIN(60,scalerInfo->u4DramLinePitch));        
        }
        else
        {
            vScpipSetDramLinePitch(u4VdpId, scalerInfo->u4DramLinePitch); 
        }
    }
    else
    {
        vScpipSetDramLinePitch(u4VdpId, scalerInfo->u4DramLinePitch); 
    }          
}

static void _vScpipSetMirrorFlipReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 dramOffset; 
    UINT8 bMirrorFlipMode;
    
    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);
    bMirrorFlipMode = ((_arScalePrm[u4VdpId].u4FlipEnable << 1) | (_arScalePrm[u4VdpId].u4MirrorEnable));

    //1. set display mode (bit1: Mirror bit0: Flip)
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_00+dramOffset, bMirrorFlipMode, DRAM_M_00_DA_DISPLAY_MODE);
}

static void vScpipSetColorTranInCTI(UINT8 bPath, UINT32 u4Is444, UINT32 u4Is422Mode)
{
    //Rule: 1. CTI need 444 data. Need to make sure CTI receive 444 data.
    //      2. After CTI, 444 to 422 depends on 422 scaling mode is enabled.
    vScpipEnable422To444BeforeCTI(bPath,(u4Is444==1)?1:0);     //0: enable 422 to 444, 1: disable 422 to 444
    vScpipEnable444To422AfterCTI(bPath,(u4Is422Mode==1)?0:1);  //0: enable 444 to 422, 1: disable 444 to 422    
}

static void _vScpipUpdateInOutPrm(UINT8 bPath)
{
    VRM_INFO_T dummy;
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath);

    //get fsc in & fsc out
    VRMGetModuleVRMInfo(bPath, VRM_MODULE_FSC, &(scalerInfo->fscIn), &(scalerInfo->fscOut));
    //get time gen info
    VRMGetModuleVRMInfo(bPath, VRM_MODULE_MIXER, &(dummy), &(scalerInfo->fscTimeGen));
}

static void _vScpipUpdateScaliingFactorPrm(UINT8 bPath, UINT32 inW, UINT32 inH, UINT32 outW, UINT32 outH)
{
    VDP_SCALER_PRM_T* scalerInfo;
    UINT32 u4HPhaseInc;
 	UINT32 u4VPhaseInc;

    scalerInfo = getScalerInfo(bPath);
    
	if(outW ==0 || outH == 0)
	{
	    Printf("AP set Output Window Width or Height is 0\n");
	    return;
	}

	u4HPhaseInc = (inW * DFT_UNIT_PHASE(bPath)) / outW;
	u4VPhaseInc = (inH * DFT_UNIT_PHASE(bPath)) / outH;

	LOG(3, "scaling factor in(%d %d) out(%d %d)\n", inW, inH, outW, outH);
		
	scalerInfo->u4HDeltaPhaseIntg = u4HPhaseInc >> DFT_FRAC_PHASE_BITS(bPath);	
	scalerInfo->u4HDeltaPhaseFrac = u4HPhaseInc & DFT_FRAC_PHASE_MASK(bPath);
	scalerInfo->u4VDeltaPhaseIntg = u4VPhaseInc >> DFT_FRAC_PHASE_BITS(bPath);
	scalerInfo->u4VDeltaPhaseFrac = u4VPhaseInc & DFT_FRAC_PHASE_MASK(bPath);

	LOG(3, "scaling factor H(0x%x 0x%x) V(0x%x 0x%x)\n", scalerInfo->u4HDeltaPhaseIntg, 
                                        	             scalerInfo->u4HDeltaPhaseFrac, 
                                        	             scalerInfo->u4VDeltaPhaseIntg, 
                                        	             scalerInfo->u4VDeltaPhaseFrac);

	return;
}

/**
 * @Cobra porting done
 */
static void _vScpipUpdateDataFormatPrm(UINT8 bPath)
{
    VDP_SCALER_PRM_T* scalerInfo;
    UINT8   u1IsMirrorMode = 0;
    
    scalerInfo = getScalerInfo(bPath);

    //1. update 8/10 bits parameter
    scalerInfo->u410BitMode = u4ScpipGet10BitMode(getScalerVDP(bPath));

    //2. update 422/444 parameter
    scalerInfo->u4Is444 = u4ScpipGetIsInput444Mode(bPath);

    //3. update 422 up parameter, only main support 422 mode.
    scalerInfo->u4Is422Mode = u4ScpipGetIs422ScalingMode(bPath);
    
    //4. 422 up + mirror
   #ifdef MIB_FLIP_MIRROR_COLOR_SHIFT_WA
   #ifdef CC_FLIP_MIRROR_SUPPORT
   if((bPath == SV_VP_MAIN) && (scalerInfo->u4Is444 == 0) && (u4GetFlipMirrorModule(bPath)==FLIP_BY_PSCAN))
   {
	   if(u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON)
	   {
		   u1IsMirrorMode = 1;
	   }
   }
   #endif
   #endif
   
    if((scalerInfo->u4Is422Mode== 1 && scalerInfo->u4MirrorEnable == 1 && u1ScpipIsDramFrontMode(bPath)==SV_TRUE) || u1IsMirrorMode)
    {
        scalerInfo->u4Is422UVMirrorMode = 1;
    } 
	else
	{
	    scalerInfo->u4Is422UVMirrorMode = 0;
	}  

    //5. update dram input format. 0:422 1:444
    scalerInfo->u4DramInputFmt = u4ScpipUpdateDramInputFmt(bPath);

    //6. update dram write format. 0:422 1:444
    scalerInfo->u4DramWriteFmt = u4ScpipGetDramWriteFmt(bPath);

    //7. update dram 444to422 & 422to444 enable
    scalerInfo->u4Dram444to422En = u4ScpipGetDram444to422En(bPath);
}

void _vScpipUpdateScaliingPhasePrm(UINT8 bPath, UINT32 inW,UINT32 inH, UINT32 outW,UINT32 outH)
{
	UINT32 u4HPhaseInc;
	UINT32 u4VPhaseInc;
	UINT32 u4PhaseExceeded;
    VDP_SCALER_PRM_T* scalerInfo;    
	UINT8   u1IsMirrorMode = 0;
	
	if(outW ==0 || outH ==0)
	{
		LOG(3,"!!Output size is 0. outW(%d) outH(%d)\n", outW, outH);
		return;
	}

    scalerInfo = getScalerInfo(bPath);
	
	u4HPhaseInc = (inW * DFT_UNIT_PHASE(bPath)) / outW;
	u4VPhaseInc = (inH * DFT_UNIT_PHASE(bPath)) / outH;

	// H Initial Phase
    if(inW < outW)    /* h up scaling */
    {
	    scalerInfo->u4HInitPhaseIntg = 0;
	    if(scalerInfo->u4HDeltaPhaseFrac == 0)
    	{
			scalerInfo->u4HInitPhaseFrac = 0;
			scalerInfo->u4HInitPhaseSign = 0;    	
    	}
	    else
    	{
			scalerInfo->u4HInitPhaseFrac = DFT_UNIT_PHASE(bPath) + ( ((inW-1)*DFT_UNIT_PHASE(bPath))>>1 ) - (((outW-1)*scalerInfo->u4HDeltaPhaseFrac)>>1);
			scalerInfo->u4HInitPhaseSign = 1;            
    	}	    
    }
    else    /* h down scaling */
    {
    	u4PhaseExceeded = (inW -1)* DFT_UNIT_PHASE(bPath) - (outW-1)*u4HPhaseInc;
    	
		scalerInfo->u4HInitPhaseIntg = (u4PhaseExceeded/2)>>DFT_FRAC_PHASE_BITS(bPath);
		scalerInfo->u4HInitPhaseFrac = (u4PhaseExceeded/2) & DFT_FRAC_PHASE_MASK(bPath);
		scalerInfo->u4HInitPhaseSign = 0;		
    }
    if((bPath == SV_VP_PIP) && (scalerInfo->u4HInitPhaseFrac > 0x7fff))
    {
        LOG(3,"Sub path :Current H-initial phase is (%d) and fix to 0x7fff\n",scalerInfo->u4HInitPhaseFrac);
        scalerInfo->u4HInitPhaseFrac = 0x7fff;
    }
    else if((bPath == SV_VP_MAIN) && (scalerInfo->u4HInitPhaseFrac > 0xFFFFF))
    {
        LOG(3,"Main path :Current H-initial phase is (%d) and fix to 0xfffff\n",scalerInfo->u4HInitPhaseFrac);
        scalerInfo->u4HInitPhaseFrac = 0xFFFFF;
    }

	// V Initial Phase
    if(inH < outH)    /* v up scaling */
    {
    	scalerInfo->u4VInitPhaseIntg = 0;
    	
		if(scalerInfo->u4VDeltaPhaseFrac == 0)
		{
			scalerInfo->u4VInitPhaseFrac = 0;
			scalerInfo->u4VInitPhaseSign = 0;
		}
		else
		{
            //scalerInfo->u4VInitPhaseFrac =(INT32)( DFT_UNIT_PHASE(bPath) + (((INT32)((inH-1)*DFT_UNIT_PHASE(bPath))) - ((INT32)((outH-1)*scalerInfo->u4VDeltaPhaseFrac)) )/2 );
            scalerInfo->u4VInitPhaseFrac =( DFT_UNIT_PHASE(bPath) + (((inH-1)*DFT_UNIT_PHASE(bPath))>>1) - (((outH-1)*scalerInfo->u4VDeltaPhaseFrac)>>1)  );

			scalerInfo->u4VInitPhaseSign = 1;
		}
    }
    else    		/* v down scaling */
    {
    	u4PhaseExceeded = (inH -1)* DFT_UNIT_PHASE(bPath) - (outH-1)*u4VPhaseInc;
		scalerInfo->u4VInitPhaseIntg = (u4PhaseExceeded/2)>>DFT_FRAC_PHASE_BITS(bPath);
		scalerInfo->u4VInitPhaseFrac = (u4PhaseExceeded/2) & DFT_FRAC_PHASE_MASK(bPath);
		scalerInfo->u4VInitPhaseSign = 0;
    }
    if((bPath == SV_VP_PIP) && (scalerInfo->u4VInitPhaseFrac > 0x7fff))
    {
        LOG(3,"Sub path :Current V-initial phase is (%d) and fix to 0x7fff\n",scalerInfo->u4HInitPhaseFrac);
        scalerInfo->u4VInitPhaseFrac = 0x7fff;
    }
    else if((bPath == SV_VP_MAIN) && (scalerInfo->u4VInitPhaseFrac > 0xFFFFF))
    {
        LOG(3,"Main path :Current V-initial phase is (%d) and fix to 0xfffff\n",scalerInfo->u4HInitPhaseFrac);
        scalerInfo->u4VInitPhaseFrac = 0xFFFFF;
    }

    if(scalerInfo->u4Is444)
    {
        scalerInfo->u4HUVInitPhaseSign = scalerInfo->u4HInitPhaseSign;
        scalerInfo->u4HUVInitPhaseIntg = scalerInfo->u4HInitPhaseIntg;
        scalerInfo->u4HUVInitPhaseFrac = scalerInfo->u4HInitPhaseFrac;
    }
    else // 422
    {
        scalerInfo->u4HUVInitPhaseSign = scalerInfo->u4HInitPhaseSign;
        scalerInfo->u4HUVInitPhaseIntg = scalerInfo->u4HInitPhaseIntg;
        
        #ifdef MIB_FLIP_MIRROR_COLOR_SHIFT_WA
        #ifdef CC_FLIP_MIRROR_SUPPORT
        if(u4GetFlipMirrorModule(bPath)==FLIP_BY_PSCAN)
        {
            if (u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON)
            {
                u1IsMirrorMode = 1;
            }
        }
        #endif
        #endif

        if(scalerInfo->u4HInitPhaseSign == 0) // downscaling or NO scaling
        {
            scalerInfo->u4HUVInitPhaseFrac = scalerInfo->u4HInitPhaseFrac;
            if((scalerInfo->u4MirrorEnable != 0 && u1ScpipIsDramFrontMode(bPath)==SV_TRUE)||u1IsMirrorMode==1)
            {
                scalerInfo->u4HUVInitPhaseFrac = scalerInfo->u4HInitPhaseFrac + DFT_UNIT_PHASE(bPath);
            }
        }
        else // upscaling
        {
            scalerInfo->u4HUVInitPhaseFrac = scalerInfo->u4HInitPhaseFrac + DFT_UNIT_PHASE(bPath);
            if((scalerInfo->u4MirrorEnable != 0 && u1ScpipIsDramFrontMode(bPath)==SV_TRUE)||u1IsMirrorMode==1)
            {
                scalerInfo->u4HUVInitPhaseFrac = scalerInfo->u4HInitPhaseFrac;
            }
        }
    }

	LOG(3, "Scaling phase H(0x%x 0x%x 0x%x)\n", scalerInfo->u4HInitPhaseSign,
										  scalerInfo->u4HInitPhaseIntg,
										  scalerInfo->u4HInitPhaseFrac);
	LOG(3, "Scaling phase V(0x%x 0x%x 0x%x)\n", scalerInfo->u4VInitPhaseSign,
										  scalerInfo->u4VInitPhaseIntg,
										  scalerInfo->u4VInitPhaseFrac);
	LOG(3, "Scaling phase HUV(0x%x 0x%x 0x%x)\n", scalerInfo->u4HUVInitPhaseSign,
										  scalerInfo->u4HUVInitPhaseIntg,
										  scalerInfo->u4HUVInitPhaseFrac);
	
	return;
}

void vScpipUpdateScalerBobModePhasePrm(UINT32 u4VdpId)
{
    VDP_SCALER_PRM_T* scalerInfo;
    UINT8 bPath;
    UINT32 u4UnitPhase;
    
    bPath = getScalerPath(u4VdpId);
    scalerInfo = getScalerInfo(bPath);
    u4UnitPhase = DFT_UNIT_PHASE(bPath);
    
    if(scalerInfo->u4IsScalerBobMode == SV_TRUE)
    {
        //1. fraction
        //pScctrl1Csr->sc_v_phase_init_frac_btm_left_1 = pScctrl1Csr->sc_v_phase_init_frac_btm_right_1 = pScctrl1Csr->sc_v_phase_init_frac_1 - ( iUnitPhase>>2 );
        scalerInfo->u4VInitPhaseFracBtmLeft = scalerInfo->u4VInitPhaseFrac - (u4UnitPhase>>2);
        scalerInfo->u4VInitPhaseFracBtmRight = scalerInfo->u4VInitPhaseFrac - (u4UnitPhase>>2);
        
        //2. integer
        //pScctrl1Csr->sc_v_phase_init_intg_btm_left_1 = pScctrl1Csr->sc_v_phase_init_intg_btm_right_1 = pScctrl1Csr->sc_v_phase_init_intg_1;
        scalerInfo->u4VInitPhaseIntgBtmLeft = scalerInfo->u4VInitPhaseIntg;
        scalerInfo->u4VInitPhaseIntgBtmRight = scalerInfo->u4VInitPhaseIntg;

        //3. Sign
        //pScctrl1Csr->sc_v_phase_init_sign_btm_left_1 = pScctrl1Csr->sc_v_phase_init_sign_btm_right_1 = pScctrl1Csr->sc_v_phase_init_sign_1;
        scalerInfo->u4VInitPhaseSignBtmLeft = scalerInfo->u4VInitPhaseSign;
        scalerInfo->u4VInitPhaseSignBtmRight = scalerInfo->u4VInitPhaseSign;        

        //4. 
        //pScctrl1Csr->sc_v_phase_init_frac_1 += ( iUnitPhase>>2 );
        scalerInfo->u4VInitPhaseFrac = scalerInfo->u4VInitPhaseFrac + (u4UnitPhase>>2);

        //if( pScctrl1Csr->sc_v_phase_init_frac_1 >= iUnitPhase )
        if(scalerInfo->u4VInitPhaseFrac >= u4UnitPhase)
        {
            //pScctrl1Csr->sc_v_phase_init_frac_1 -= iUnitPhase;
            scalerInfo->u4VInitPhaseFrac = scalerInfo->u4VInitPhaseFrac - u4UnitPhase;
          
            //if( pScctrl1Csr->sc_v_phase_init_sign_1 == 1 )
            if(scalerInfo->u4VInitPhaseSign == 1)
            {
                //pScctrl1Csr->sc_v_phase_init_sign_1 = 0;
                scalerInfo->u4VInitPhaseSign = 0;
            }
            else
            {
                //pScctrl1Csr->sc_v_phase_init_intg_1 += 1;
                scalerInfo->u4VInitPhaseIntg = scalerInfo->u4VInitPhaseIntg + 1;
            }
        }
        
        //pScctrl1Csr->sc_v_phase_init_frac_top_right_1 = pScctrl1Csr->sc_v_phase_init_frac_1;
        //pScctrl1Csr->sc_v_phase_init_intg_top_right_1 = pScctrl1Csr->sc_v_phase_init_intg_1;
        //pScctrl1Csr->sc_v_phase_init_sign_top_right_1 = pScctrl1Csr->sc_v_phase_init_sign_1;
        scalerInfo->u4VInitPhaseFracTopRight = scalerInfo->u4VInitPhaseFrac;
        scalerInfo->u4VInitPhaseIntgTopRight = scalerInfo->u4VInitPhaseIntg;
        scalerInfo->u4VInitPhaseSignTopRight = scalerInfo->u4VInitPhaseSign;        
    }
    else
    {
        scalerInfo->u4VInitPhaseFracTopRight = scalerInfo->u4VInitPhaseFrac;
        scalerInfo->u4VInitPhaseIntgTopRight = scalerInfo->u4VInitPhaseIntg;
        scalerInfo->u4VInitPhaseSignTopRight = scalerInfo->u4VInitPhaseSign;    
        scalerInfo->u4VInitPhaseFracBtmLeft = scalerInfo->u4VInitPhaseFrac;
        scalerInfo->u4VInitPhaseIntgBtmLeft = scalerInfo->u4VInitPhaseIntg;
        scalerInfo->u4VInitPhaseSignBtmLeft = scalerInfo->u4VInitPhaseSign;    
        scalerInfo->u4VInitPhaseFracBtmRight = scalerInfo->u4VInitPhaseFrac;
        scalerInfo->u4VInitPhaseIntgBtmRight = scalerInfo->u4VInitPhaseIntg;
        scalerInfo->u4VInitPhaseSignBtmRight = scalerInfo->u4VInitPhaseSign;    
    }
}

static BOOL _bScpipIsFrontDramMode(UINT8 bPath, UINT32 inW, UINT32 inH)
{
	BOOL ret;
	SCALER_FBM_POOL_T* fbmInfo;

	ret= SV_FALSE;

#if defined(CC_SRM_ON)	
    fbmInfo = getFbmPoolInfo(bPath);
	
	if( ALIGN_256_CEIL(inW)  <= ALIGN_256_CEIL(fbmInfo->u4Width)&&
		ALIGN_16_CEIL(inH) <= ALIGN_16_CEIL(fbmInfo->u4Height))
	{
		ret = SV_TRUE;
	}
	LOG(3,"FBM Dram whether enought Ret (%d)\n",ret);
#endif

	return ret;
}

/********************* 
  * 1 -- frn_dram_mode
  * 0 -- bck_dram_mode
  *********************/
static void _vScpipUpdateDramFrnBckModePrm(UINT8 bPath, UINT32 inW, UINT32 inH, UINT32 outW, UINT32 outH)
{
	UINT32 inres, outres;
	VDP_SCALER_PRM_T* scalerInfo;
    UINT32 u4IsDramFrontModePre;
    
	scalerInfo = getScalerInfo(bPath);   
    inres  = ALIGN_256_CEIL(inW)  * ALIGN_16_CEIL(inH);
    outres = ALIGN_256_CEIL(outW) * ALIGN_16_CEIL(outH);

    LOG(3, "bPath(%d)  In Area(%d(%d)x%d(%d)) = %d\n", bPath,  inW, ALIGN_256_CEIL(inW),   inH, ALIGN_16_CEIL(inH),   inres);
    LOG(3, "bPath(%d) Out Area(%d(%d)x%d(%d)) = %d\n", bPath, outW, ALIGN_256_CEIL(outW), outH, ALIGN_16_CEIL(outH), outres);
    
    u4IsDramFrontModePre=scalerInfo->u4IsDramFrontMode;
    if(scalerInfo->u4ForcedDramMode == 0)           //auto
    {
        #ifdef CC_DOWN_SCALER_BETTER_Q
        if(_bScpipIsFrontDramMode(bPath,outW,outH)) // BACK dram mode have higher priority if there is no pre-down
        {
            scalerInfo->u4IsDramFrontMode = SV_FALSE;
        }
        else
        {
            scalerInfo->u4IsDramFrontMode = SV_TRUE;
        }
        #else
        if(inres <= outres || _bScpipIsFrontDramMode(bPath,inW,inH))
        {
            scalerInfo->u4IsDramFrontMode = SV_TRUE;
        }
        else
        {
            scalerInfo->u4IsDramFrontMode = SV_FALSE;
        }
        #endif

        #ifdef CC_SUPPORT_VENC
        #ifdef VENC_SUB_SCALER_SCALING
        if((u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER) && (bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC)) && (bPath == VDP_2))
        {
            if(inW == outW && inH == outH)
            {
               scalerInfo->u4IsDramFrontMode = SV_TRUE; 
            }
            else
            {
               scalerInfo->u4IsDramFrontMode = SV_FALSE; 
            }      
        }
        #endif
        #endif
    }
    else if (scalerInfo->u4ForcedDramMode == 1)
    {
        scalerInfo->u4IsDramFrontMode = SV_TRUE;
    }
    else
    {
        scalerInfo->u4IsDramFrontMode = SV_FALSE;
    }

    //dram mode and front<->back mode changed
    if(u4IsDramFrontModePre!=scalerInfo->u4IsDramFrontMode && scalerInfo->u4DispMode==VDP_SCPOS_DISPMODE_OFF)
    {
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, bPath, 20, TRUE);
        LOG(1,"bPath(%d) Front dram mode(%d)->(%d) mute\n",bPath,u4IsDramFrontModePre,scalerInfo->u4IsDramFrontMode);
    }
   
    LOG(3, "bPath(%d) Is Front Dram mode:%d Force Dram mode: %d\n", bPath, scalerInfo->u4IsDramFrontMode, scalerInfo->u4ForcedDramMode);
}

void vScpipSetForcedFrontBackDramMode(UINT32 u4VdpId,UINT32 u4Mode)
{
    UINT8 bPath;
    VDP_SCALER_PRM_T* scalerInfo;

    if(u4VdpId>= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    scalerInfo = getScalerInfo(bPath);

    scalerInfo->u4ForcedDramMode = u4Mode;
    vScpipReconfig(u4VdpId);
}

/********************* 
  * 1 -- frn_dram_mode
  * 0 -- bck_dram_mode
  *********************/
UINT8 u1ScpipIsDramFrontMode(UINT8 bPath)
{
    VDP_SCALER_PRM_T* scalerInfo;

    scalerInfo = getScalerInfo(bPath);

    return scalerInfo->u4IsDramFrontMode;
}

UINT8 u1ScpipIsTGUnderflow(UINT32 u4VdpId)
{
    UINT8 bIsUnder;

    if(u4VdpId >= VDP_NS)
    {
        return 0;
    }
    
    if(u4VdpId == VDP_1)
    {
        SCPIP_WRITE_FLD(SCPIP_SCSYS_01, 1, SCSYS_01_SC_CLR_MTG_UDFL_FLG);
        //x_thread_delay(1);
        SCPIP_WRITE_FLD(SCPIP_SCSYS_01, 0, SCSYS_01_SC_CLR_MTG_UDFL_FLG);

        bIsUnder = SCPIP_READ_FLD(SCPIP_SCSYS_01, SCSYS_01_SC_STA_M_TG_UNDERFLOW);
    }
    else
    {
        SCPIP_WRITE_FLD(SCPIP_SCSYS_01, 1, SCSYS_01_SC_CLR_PTG_UDFL_FLG);
        //x_thread_delay(1);
        SCPIP_WRITE_FLD(SCPIP_SCSYS_01, 0, SCSYS_01_SC_CLR_PTG_UDFL_FLG);

        bIsUnder = SCPIP_READ_FLD(SCPIP_SCSYS_01, SCSYS_01_SC_STA_P_TG_UNDERFLOW);
    
    }

    LOG(1, "%s Scaler underflow: %s\n", ((u4VdpId == VDP_1)?"Main":"Sub"), ((bIsUnder==1)?"TRUE":"FALSE"));       
    return bIsUnder;
}

void _vScpipUpdateDramWidthHeightPrm(UINT8 bPath, UINT32 inW, UINT32 inH, UINT32 outW, UINT32 outH)
{
    VDP_SCALER_PRM_T* scalerInfo;
	UINT32 u4IsDramFrontMode;
	u4IsDramFrontMode = u1ScpipIsDramFrontMode(bPath);
	scalerInfo = getScalerInfo(bPath);
	
	if(u4IsDramFrontMode==SV_TRUE)
	{
	    //width-related
		scalerInfo->u4DramSRCWidth = inW;	
		scalerInfo->u4DramWidth =  inW;
		scalerInfo->u4DramHPitch = inW;
		//Height-related		
		scalerInfo->u4DramSRCHeight = inH;
		scalerInfo->u4DramHeight = inH;
		scalerInfo->u4DramVPitch = inH;

		if(bIsScalerInput444(bPath)==SV_TRUE && (inW%2)!=0)
		{
    		scalerInfo->u4DramSRCWidth += 1;		
    		scalerInfo->u4DramWidth += 1;
    		scalerInfo->u4DramHPitch += 1;		    
		}

		LOG(3, "Path(%d) = DramFrnMode, dram HV(%d %d), pitch(%d %d)\n", bPath, inW, inH, inW, inH);

		//for scaler 3D test
        if( (eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_SBS || 
             eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_TAB) && 
             eScpipGetTdtvOutType(bPath) == SCALER_TDTV_OUT_TYPE_PR)
        {
    		scalerInfo->u4DramHeight = inH >> 1;
        }

        if( eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_FS && 
            eScpipGetTdtvOutType(bPath) == SCALER_TDTV_OUT_TYPE_PR)
        {
    		scalerInfo->u4DramVPitch = inH << 1;
        }
        
        //SBS to SG
        if(eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_SBS && 
            eScpipGetTdtvOutType(bPath) == SCALER_TDTV_OUT_TYPE_SG)
        {
            scalerInfo->u4DramSRCWidth = scalerInfo->u4DramSRCWidth<<1;
        }
        //TAB/LI to SG
        if((eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_TAB ||
            eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_LI) && 
            eScpipGetTdtvOutType(bPath) == SCALER_TDTV_OUT_TYPE_SG)
        {
            scalerInfo->u4DramSRCHeight = scalerInfo->u4DramSRCHeight<<1;
        }        
	}
	else
	{
	    //width-related
		scalerInfo->u4DramSRCWidth = outW;		
		scalerInfo->u4DramWidth =  outW;
		scalerInfo->u4DramHPitch = outW;
		//Height-related
		scalerInfo->u4DramSRCHeight = outH;
		scalerInfo->u4DramHeight = outH;
		scalerInfo->u4DramVPitch = outH;

		LOG(3, "Path(%d) = DramBckMode, dram HV(%d %d), pitch(%d %d)\n", bPath, outW, outH, outW, outH);	
	}

	return;
}

/**
 *  Update read pointer generate position
 */
UINT32 u4VHeightn[2]={0,0};

static void _vScpipUpdateRptGenPrm(UINT8 bPath, UINT32 u4DramHeight)
{
	VDP_SCALER_PRM_T* scalerInfo;

	scalerInfo = getScalerInfo(bPath);
	
	if(u4DramHeight >= 2)
	{
		if (_arScalePrm[SV_VP_MAIN].u4IsSuperGameMode)
		{
			scalerInfo->u4RptGenCount = 2;
		}
		else
		{
			scalerInfo->u4RptGenCount = u4DramHeight - 2;	//original code flow 
			if ( (u4VHeightn[bPath] > 0) && ( scalerInfo->u4RptGenCount == u4VHeightn[bPath] ) )
			{
				scalerInfo->u4RptGenCount = u4VHeightn[bPath] - 1;
			}
			
			u4VHeightn[bPath] = u4DramHeight;
			LOG(3,"path %d, htotal = %d, height = %d,rpt_vcnt = %d\n",bPath, scalerInfo->fscIn.u2HTotal , scalerInfo->fscIn.u2Height, scalerInfo->u4RptGenCount );
		}
	}
	else
	{
		scalerInfo->u4RptGenCount = 0;
	}
	
	LOG(3, "Path(%d) Update RptGen: %d\n", bPath, scalerInfo->u4RptGenCount);
	
	return;
}

extern UINT8 _bSRMStable[VDP_NS];
static void _vScpipUpdateDramFrameDelay(UINT8 bPath)
{
    VDP_SCALER_PRM_T* scalerInfo;
    UINT8 bScalerMode;
    
    scalerInfo = getScalerInfo(bPath);
    bScalerMode = getScalerMode(bPath);
    if(bDrvVideoSignalStatus(bPath)==(UINT8)SV_VDO_STABLE && 
       bScalerMode == VDP_SCPOS_DISPMODE_OFF &&
       _bSRMStable[bPath]==1 &&
       bScpipGetDramRead(bPath) == SV_ON)
    {
        scalerInfo->u4DramFrmDly = 1;
    }
    else
    {
        scalerInfo->u4DramFrmDly = 0;
    }

    LOG(3, "bPath(%d) Update Dram frame delay to (%d)\n",bPath, scalerInfo->u4DramFrmDly);
}

static void _vScpipUpdateInputResolution(UINT8 bPath, UINT32 inW, UINT32 inH)
{
    VDP_SCALER_PRM_T* scalerInfo;

    scalerInfo = getScalerInfo(bPath);

    scalerInfo->u4InputWidth = inW;
    scalerInfo->u4InputHeight = inH;

    //SBS to PR
    if(eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_SBS && eScpipGetTdtvOutType(bPath) == SCALER_TDTV_OUT_TYPE_PR)
    {
        scalerInfo->u4InputWidth /= 2;
        scalerInfo->u4InputHeight *= 2;        
    }

    //SBS to SG
    if(eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_SBS && eScpipGetTdtvOutType(bPath) == SCALER_TDTV_OUT_TYPE_SG)
    {
        scalerInfo->u4InputWidth /= 2;    
    }
    
    //TAB/LI to SG
    if((eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_TAB ||
        eScpipGetTdtvInType(bPath) == SCALER_TDTV_IN_TYPE_LI) 
        && eScpipGetTdtvOutType(bPath) == SCALER_TDTV_OUT_TYPE_SG)
    {
        scalerInfo->u4InputHeight /= 2;    
    }    
}

#if defined(DZONE_DETECTION_EN)
static void _vScpipDoDZoneDetection(UINT8 bPath)
{
    UINT16 u2InVTotal;
    UINT16 u2InVActive;
    UINT16 u2InVBlank;
    UINT32 u4SclInVTotal;
    UINT16 u2FtkLine;
    UINT8 bFtkStep;
    //VRM_INFO_T* RegenOut;
    VDP_SCALER_PRM_T* scalerInfo;

    if(bPath == SV_VP_PIP || 
       getScalerMode(bPath)!= VDP_SCPOS_DISPMODE_OFF || 
       bDrvVideoSignalStatus(bPath)!=(UINT8)SV_VDO_STABLE)
    {
        return;
    }

    scalerInfo = getScalerInfo(bPath);
    //RegenOut = VRMGetVRMInfo(bPath, VRM_MODULE_MIB);

    u2InVTotal = wDrvVideoGetVTotal(bPath);
    u2InVActive = wDrvVideoInputHeight(bPath);
    u2InVBlank = u2InVTotal - u2InVActive;
    u4SclInVTotal = ((u2InVActive * u4PDSGetVPDSFactor(bPath)) / 0x8000) + u2InVBlank;
    u2FtkLine = vScpipGetFrmTrkLines();
    bFtkStep = bScpipGetFrmTrkStep();

    //calculate the center pos
    scalerInfo->u4DZoneCenter = 
        ((u4SclInVTotal) * (_arOutTimingPrm.u2PanelVTotal - u2FtkLine))/(_arOutTimingPrm.u2PanelVTotal);
    
    //if center pos is around rpt ren.
    if( DIFF(scalerInfo->u4DZoneCenter, scalerInfo->u4RptGenCount)<=bFtkStep ) 
    {
        scalerInfo->bIsInDZone = SV_TRUE;
        scalerInfo->u4RptGenCount =  scalerInfo->u4DZoneCenter - MAX(bFtkStep+1, FRC_LINE_ADJ);
    }
    else
    {
        scalerInfo->bIsInDZone = SV_FALSE;
    }

    LOG(2,"IsInDZone(%d) DZoneCenter(%d) RptGenCnt(%d)\n", scalerInfo->bIsInDZone, scalerInfo->u4DZoneCenter, scalerInfo->u4RptGenCount);
    LOG(2,"InVTotal(%d) InVActive(%d) InVBlank(%d) \n", u2InVTotal, u2InVActive, u2InVBlank);
    LOG(2,"SclInVTotal(%d) u2FtkLine(%d) bFtkStep(%d) u2PanelVTotal(%d)\n",
            u4SclInVTotal, u2FtkLine, bFtkStep, _arOutTimingPrm.u2PanelVTotal);
}
#endif

static void _vScpipUpdateOutputResolution(UINT8 bPath, UINT32 outX, UINT32 outY, UINT32 outW, UINT32 outH)
{
    VDP_SCALER_PRM_T* scalerInfo;

    CRIT_STATE_T csState;

    scalerInfo = getScalerInfo(bPath);

 
	if(bDrvVideoSignalStatus(bPath)!=(UINT8)SV_VDO_STABLE)  //unstable
    {
        scalerInfo->u4TGImportEnable = SV_OFF;
    }
    else
    {
        scalerInfo->u4TGImportEnable = SV_ON;
    }  

    if(bDrvVideoSignalStatus(bPath) == SV_VDO_NOSIGNAL &&
        (scalerInfo->u4OutputX != outX || scalerInfo->u4OutputY != outY || scalerInfo->u4OutputWidth != outW || scalerInfo->u4OutputHeight != outH))
	{
        scalerInfo->bUpdateOutXYWHLater = SV_TRUE;
	}
    else
    {
        scalerInfo->bUpdateOutXYWHLater = SV_FALSE;       
    }

    
    csState = x_crit_start();
    scalerInfo->u4OutputX = outX;
    scalerInfo->u4OutputY = outY;
    scalerInfo->u4OutputWidth = outW;
    scalerInfo->u4OutputHeight = outH;   
    x_crit_end(csState);

    //@todo: review if cobra need to do this??
    if(_u4ForceDispOff[bPath])// for TVE use, not to turn on sub window.
    {
        //scalerInfo->u4OutputHeight = 0; 
        //scalerInfo->u4OutputWidth = 0; 
    } 
}

BOOL bScpipIsNeedSetUpdate(UINT8 bPath)
{
    BOOL ret = SV_FALSE;
    VDP_SCALER_PRM_T* scalerInfo;
    
    scalerInfo = getScalerInfo(bPath);
    if(scalerInfo->bUpdateOutXYWHLater == SV_TRUE )
    {
        ret = SV_TRUE;
    }
    return ret;
}

void vScpipClearNeedUpdateFlag(void)
{
    VDP_SCALER_PRM_T* scalerInfo;
    UINT8 bIndex;

    for(bIndex=0;bIndex<=SV_VP_PIP;bIndex++)
    {
        scalerInfo = getScalerInfo(bIndex);
        scalerInfo->bUpdateOutXYWHLater = SV_FALSE;
    }   
}


#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
static void _vScpipUpdateBypassFlg(UINT8 bPath, UINT32 inW, UINT32 inH)
{
    VDP_SCALER_PRM_T* scalerInfo;
    UINT8 bFscBypass;
    
    scalerInfo = getScalerInfo(bPath);
    bFscBypass = SV_FALSE;

    //MIB is bypassed and Scaler's input > 1920 => need to bypass.
    if(inW > 1920 && getFbmPoolInfo(bPath)->u4MddiDramWidth == SCPOS_BYPASS_PSCANNR)
    {
        bFscBypass = SV_TRUE;
    }

    scalerInfo->u4Bypass = bFscBypass;
}
#endif

void vScpipSetForcedVScalingMode(UINT8 bPath, UINT8 mode)
{
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath);    

    scalerInfo->bForcedVScaingMode = mode;
    vScpipReconfig(bPath);
}

UINT8 vScpipGetForcedVScalingMode(UINT8 bPath)
{
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath);    

    return scalerInfo->bForcedVScaingMode;
    
}

static void _vScpipUpdateVScalingModePrm(UINT8 bPath,UINT32 u4InHeight, UINT32 u4OutHeight)
{
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath);    

    if(u4InHeight < u4OutHeight)    //up-scaling
    {
        scalerInfo->bVScalingMode = 1;  //FIR;
    }
    else                            //down-scaling or no-scaling
    {
        if(scalerInfo->bForcedVScaingMode==0)       //auto
        {
            if ((getScalerMode(bPath) == VDP_SCPOS_DISPMODE_OFF) && !u1ScpipIsDramFrontMode(bPath) &&(bPath == SV_VP_PIP && u4OutHeight < u4InHeight>>1))
            {
                scalerInfo->bVScalingMode = 0;  //use Partial sum when back dram mode && down scaling > 2
            }
            else
            {
                scalerInfo->bVScalingMode = 1;  //Currently, use FIR as default
            }
        }
        else if(scalerInfo->bForcedVScaingMode==1)
        {
            scalerInfo->bVScalingMode = 1;  //Force to FIR
        }
        else
        {
            scalerInfo->bVScalingMode = 0;  //Force to Partial sum
        }
    }
}

static void _vScpipUpdateVPartialSumFactomPrm(UINT8 bPath,UINT32 u4InHeight, UINT32 u4OutHeight)
{
    VDP_SCALER_PRM_T* scalerInfo;
    scalerInfo = getScalerInfo(bPath); 

    if((u4InHeight < u4OutHeight) || (u4InHeight == 0))    //up-scaling
    {
        scalerInfo->u4VPartialSumFactor = DFT_DS_FACTOR;
    }
    else
    {
        scalerInfo->u4VPartialSumFactor = ((DFT_PDS_FACTOR * u4OutHeight) / u4InHeight);
    }

    scalerInfo->u4VPartialSumInitL = DFT_DS_FACTOR - scalerInfo->u4VPartialSumFactor;
    scalerInfo->u4VPartialSumInitR = DFT_DS_FACTOR - scalerInfo->u4VPartialSumFactor;
}

static void _vScpipUpdateDispPrm(UINT32 u4VdpId) 
{
    UINT8 bPath;
    VDP_SCALER_PRM_T* scalerInfo;
    bPath = getScalerPath(u4VdpId);
    scalerInfo = getScalerInfo(bPath);    
    
    //1. Get in and out size from VRM
    _vScpipUpdateInOutPrm(bPath);

    #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
    //1.5 update scaler bypass
    _vScpipUpdateBypassFlg(bPath,scalerInfo->fscIn.u2Width,scalerInfo->fscIn.u2Height);
    #endif
    
    //2. Update input resolution
    _vScpipUpdateInputResolution(bPath,scalerInfo->fscIn.u2Width,scalerInfo->fscIn.u2Height);
    //3. Update output resolution
    _vScpipUpdateOutputResolution(bPath,scalerInfo->fscOut.u2X,scalerInfo->fscOut.u2Y,
                                        scalerInfo->fscOut.u2Width,scalerInfo->fscOut.u2Height);
    //4. Update dram front or back mode
    _vScpipUpdateDramFrnBckModePrm(bPath,scalerInfo->u4InputWidth,scalerInfo->u4InputHeight,
                                         scalerInfo->u4OutputWidth,scalerInfo->u4OutputHeight);
    //5. Update data format
    _vScpipUpdateDataFormatPrm(bPath);

    //6. Update Dram width/height prm
    _vScpipUpdateDramWidthHeightPrm(bPath,scalerInfo->u4InputWidth,scalerInfo->u4InputHeight,
                                             scalerInfo->u4OutputWidth,scalerInfo->u4OutputHeight);

    if(scalerInfo->u4IsDramFrontMode == SV_TRUE)
    {    
        //7. Update scaling factor
        _vScpipUpdateScaliingFactorPrm(bPath,scalerInfo->u4DramHPitch,scalerInfo->u4DramVPitch,
                                             scalerInfo->u4OutputWidth,scalerInfo->u4OutputHeight);
        //8. Update scaling phase
        _vScpipUpdateScaliingPhasePrm(bPath,scalerInfo->u4DramHPitch,scalerInfo->u4DramVPitch,
                                         scalerInfo->u4OutputWidth,scalerInfo->u4OutputHeight);  
        //update v scaling mode
        _vScpipUpdateVScalingModePrm(bPath,scalerInfo->u4DramVPitch,scalerInfo->u4OutputHeight);
        //9. Update V scaling factor of partial sum
        _vScpipUpdateVPartialSumFactomPrm(bPath, scalerInfo->u4DramVPitch,scalerInfo->u4OutputHeight);
    }
    else
    {
         //7. Update scaling factor
        _vScpipUpdateScaliingFactorPrm(bPath,scalerInfo->u4InputWidth,scalerInfo->u4InputHeight,
                                             scalerInfo->u4DramWidth,scalerInfo->u4DramHeight);
        //8. Update scaling phase
        _vScpipUpdateScaliingPhasePrm(bPath,scalerInfo->u4InputWidth,scalerInfo->u4InputHeight,
                                         scalerInfo->u4DramWidth,scalerInfo->u4DramHeight);    
        //update v scaling mode
        _vScpipUpdateVScalingModePrm(bPath,scalerInfo->u4InputHeight,scalerInfo->u4DramHeight);
        //9. Update V scaling factor of partial sum
        _vScpipUpdateVPartialSumFactomPrm(bPath, scalerInfo->u4InputHeight,scalerInfo->u4DramHeight);
    }
    //9. Update if it's scaler bob mode
    vScpipUpdateScalerBobModeFlag(u4VdpId);

    //10. Update scaler bob mode v init phase
    vScpipUpdateScalerBobModePhasePrm(bPath);
    
    //11. Update Read pointer generate position (will depends on _vScpipUpdateDramWidthHeightPrm())
    _vScpipUpdateRptGenPrm(bPath, scalerInfo->u4DramHeight);
    #if defined(DZONE_DETECTION_EN)
    _vScpipDoDZoneDetection(bPath);
    #endif

    //12. Update dram frame delay parameter
    _vScpipUpdateDramFrameDelay(bPath);

    //13. Update Nonlinear
    vScpipUpdateUsNonlinear(u4VdpId,scalerInfo->u4InputWidth,scalerInfo->u4OutputWidth);    

    //14. update out dispr freerun
    vScpipUpdateDisprFreerunPrm(u4VdpId);    
}

#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
static void _vScpipSetBypass(UINT8 bPath, UINT32 isBypass)
{
    //1. set bypass bit
    vScpipBypass(bPath,isBypass);
    //2. inverse input VDE 
    vScpipInverseInputVDE(bPath,isBypass);    
    //3. inverse output VDE&HDE
    if(bPath == SV_VP_MAIN)
    {
        vScpipInverseOutputVSync(isBypass==SV_TRUE?SV_FALSE:SV_TRUE);
        vScpipInverseOutputHSync(isBypass==SV_TRUE?SV_FALSE:SV_TRUE);        
    }
}

static void _vScpipSet4K2KDramMode(UINT32 u2Width)
{
    LOG(2, "_vScpipSet4K2KDramMode: %d\n", u2Width);
    if (u2Width > 1920)
    {
        vScpipSetSC3DMode(8);
        vScpipSet422Mode(SV_VP_MAIN, SV_ON);
        vScpipSetDram444Fmt(SV_VP_MAIN, SV_OFF);
        vScpipEnable444To422AfterCTI(SV_VP_MAIN, 0);
    }
}
#endif

static void _vScpipSetDispReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    VDP_SCALER_PRM_T* scalerInfo;

    bPath = getScalerPath(u4VdpId);
    scalerInfo = getScalerInfo(bPath);

    //0. set Dram frame delay
    vScpipSetDramFrmDely(bPath, scalerInfo->u4DramFrmDly);        
    vScpipDramImportOnOff(bPath,((scalerInfo->u4DramFrmDly!=0)?SV_ON:SV_OFF));
     
    vScpipSetTimeGenDelay(bPath,scalerInfo->u4DramFrmDly);
    vScpipSetTimeGenDlyImportEnable(bPath,((scalerInfo->u4DramFrmDly != 0)? scalerInfo->u4TGImportEnable : SV_OFF)); 
        
    //1. set output resolution
    if(bScpipIsNeedSetUpdate(getScalerPath(u4VdpId)) == SV_FALSE)
    {
       vScpipSetOutputResolution(bPath, scalerInfo->u4OutputWidth, scalerInfo->u4OutputHeight);
    }

    if(bScpipIsNeedSetUpdate(getScalerPath(u4VdpId)) == SV_FALSE)
    {
       vScpipSetMaskStart(bPath, scalerInfo->fscOut.u2X, scalerInfo->fscOut.u2Y);    
    }
 
    //11. Set Read Pointer gen.
    // Disable Read Pointer gen AUTO.
    vScpipSetReadPosGenAuto(bPath, 0);
    vScpipSetReadPosGen(bPath, scalerInfo->u4RptGenCount);   

    #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
    //Update bypass setting
    _vScpipSetBypass(bPath, scalerInfo->u4Bypass);
    #endif

    /************************************************************************************
     **    following register won't be updated if there's no input or no output        **
     ************************************************************************************/
    if(getScalerInfo(bPath)->u4InputWidth == 0 || getScalerInfo(bPath)->u4InputHeight== 0||
	   getScalerInfo(bPath)->u4OutputWidth == 0 || getScalerInfo(bPath)->u4OutputHeight== 0) 
    {
        return;
    }    
    
    //2. set input resolution
    vScpipSetInputResolution(bPath, scalerInfo->u4InputWidth, scalerInfo->u4InputHeight);        

    //3. Set V scaling Mode
    vScpipSetVScalingMode(bPath, scalerInfo->bVScalingMode);
    //4. Set Partical Sum factor
    vScpipSetVPartialSumFactor(bPath, scalerInfo->u4VPartialSumFactor, scalerInfo->u4VPartialSumInitL, scalerInfo->u4VPartialSumInitR);
    
    //3. set H Delta Phase
    vScpipSetHDeltaPhase(bPath, scalerInfo->u4HDeltaPhaseIntg, scalerInfo->u4HDeltaPhaseFrac);
    //4. Set H Initial Phase
    vScpipSetHInitPhase(bPath, scalerInfo->u4HInitPhaseSign, scalerInfo->u4HInitPhaseIntg, scalerInfo->u4HInitPhaseFrac);
    //5. Set H Chroma Initial Phase
    vScpipSetHUVInitPhase(bPath, scalerInfo->u4HUVInitPhaseSign, scalerInfo->u4HUVInitPhaseIntg, scalerInfo->u4HUVInitPhaseFrac);
    //6. Set V Delta Phase
    vScpipSetVDeltaPhase(bPath, scalerInfo->u4VDeltaPhaseIntg, scalerInfo->u4VDeltaPhaseFrac);
    //7. Set V Initial Phase
    vScpipSetVInitPhase(bPath, scalerInfo->u4VInitPhaseSign, scalerInfo->u4VInitPhaseIntg, scalerInfo->u4VInitPhaseFrac);
    if(bPath == SV_VP_MAIN)
    {
        vScpipBobVScalingOnOff(0, scalerInfo->u4IsScalerBobMode, 0);
        vScpipSetBobBtmLeftInitPhase(scalerInfo->u4VInitPhaseSignBtmLeft, scalerInfo->u4VInitPhaseIntgBtmLeft, scalerInfo->u4VInitPhaseFracBtmLeft);
        vScpipSetBobTopRightInitPhase(scalerInfo->u4VInitPhaseSignTopRight, scalerInfo->u4VInitPhaseIntgTopRight, scalerInfo->u4VInitPhaseFracTopRight);
        vScpipSetBobBtmRightInitPhase(scalerInfo->u4VInitPhaseSignBtmRight, scalerInfo->u4VInitPhaseIntgBtmRight, scalerInfo->u4VInitPhaseFracBtmRight);
    }

    if(u1SetScalingCoef == 1&& bPath==SV_VP_MAIN)
    {
    	//Set Scaler Coeff in VSync
		vIO32Write4B(SCALER_COEF_MAIN_H_IN,scalerInfo->fscIn.u2Width);
		vIO32Write4B(SCALER_COEF_MAIN_H_OUT,scalerInfo->fscOut.u2Width);
		vIO32Write4B(SCALER_COEF_MAIN_V_IN,scalerInfo->fscIn.u2Height);
		vIO32Write4B(SCALER_COEF_MAIN_V_OUT,scalerInfo->fscOut.u2Height); 
		vApiRegisterVideoEvent(PE_EVENT_SCALER_H, SV_VP_MAIN, SV_ON);
		vApiRegisterVideoEvent(PE_EVENT_SCALER_V, SV_VP_MAIN, SV_ON);
    }
	
    //8. Set Dram Write Width/ Height
    vScpipSetDramWidthHeight(bPath, scalerInfo->u4DramWidth, scalerInfo->u4DramHeight);
    //9. Set Dram H/V Pitch
    vScpipSetDramHVPitch(bPath, scalerInfo->u4DramHPitch, scalerInfo->u4DramVPitch);
    //10. Set Dram Src Width
    vScpipSetDramSrcWidth(bPath, scalerInfo->u4DramSRCWidth);
    vScpipSetDramSrcHeight( bPath, scalerInfo->u4DramSRCHeight);
    //13. Set 422 Mode
    vScpipSet422Mode(bPath,scalerInfo->u4Is422Mode);
    //14. Set 422 Mirror Enable
    vScpipSet422MirrorEnable(bPath, scalerInfo->u4Is422UVMirrorMode);
    //15. Set 8/10 bit
    if(getScalerMode(bPath) == VDP_SCPOS_DISPMODE_OFF)
    {
        vScpipSetDram10bFmt(bPath,scalerInfo->u410BitMode);
    }
    
    //16. Set 422 to 444 enable before CTI
    vScpipSetColorTranInCTI(bPath, scalerInfo->u4Is444, scalerInfo->u4Is422Mode);
        
    //17. Set 422/444 mode
    vScpipSetDram444Fmt(bPath, scalerInfo->u4DramWriteFmt);

    //18. Set front/ back mode
    if(getScalerInfo(bPath)->u4DispMode == VDP_SCPOS_DISPMODE_OFF)
    {
        if(scalerInfo->u4IsDramFrontMode==SV_TRUE)
        {
            vScpipSetFrontMode(bPath,SV_ON);
            vScpipSetBackMode(bPath,SV_OFF);
            #if 0
            if(bPath == SV_VP_MAIN)
            {
                if(scalerInfo->u4Is444 && scalerInfo->u4Forced444 == 2)
                {
                    vScpipEnable444To422BeforeDram(bPath,SV_ON); // Is444, but forced dram mode to 422, so 444->422->444 transform is needed
                    vScpipEnable422To444AfterDram(bPath,SV_ON);
                }
                else
                {
                    vScpipEnable444To422BeforeDram(bPath,SV_OFF);
                    vScpipEnable422To444AfterDram(bPath,SV_OFF);
                }
            }
            //sub path 422 to 444 before CTI is enable when 422 input ,and 444 to 422 after CTI always bypass
            // sub path + 422 input + front mode, need do 444 to 422 before enter dram and do 422 to 444 after dram
            //main path + 422 input + front mode, no need do 444 to 422 before enter dram in sample IC(because 422 to 444 before CTI is disable in sample IC)
            if(bPath == SV_VP_PIP)
            {
                vScpipEnable444To422BeforeDram(bPath,(!scalerInfo->u4Is444)? SV_ON : SV_OFF);
                vScpipEnable422To444AfterDram(bPath,(!scalerInfo->u4Is444)? SV_ON : SV_OFF);
            }
            #endif
        }
        else
        {
            vScpipSetFrontMode(bPath,SV_OFF);
            vScpipSetBackMode(bPath,SV_ON);
            #if 0
            if(!scalerInfo->u4Is444)
            {
                vScpipEnable444To422BeforeDram(bPath,SV_ON);
                vScpipEnable422To444AfterDram(bPath,SV_ON);
            }
            else
            {
                if(scalerInfo->u4Forced444 == 2)
                {
                    vScpipEnable444To422BeforeDram(bPath,SV_ON); // Is444, but forced dram mode to 422, so 444->422->444 transform is needed
                    vScpipEnable422To444AfterDram(bPath,SV_ON);
                }
                else
                {
                    vScpipEnable444To422BeforeDram(bPath,SV_OFF);
                    vScpipEnable422To444AfterDram(bPath,SV_OFF);
                }
            }
            #endif
        }

        vScpipEnable444To422BeforeDram(bPath,scalerInfo->u4Dram444to422En);
        vScpipEnable422To444AfterDram(bPath,scalerInfo->u4Dram444to422En);        
    }

    #if !defined(CC_FPGA_SUPPORT)
    //19. Set Dram Line Pitch
    _vScpipSetDramLinePitchReg(u4VdpId);
    #endif
    //20. Set Filp/ Mirror register
    _vScpipSetMirrorFlipReg(u4VdpId);
    //21. Set 3D Input/ Output Convert register
    vScpipSet3DReg(u4VdpId);  
    //22. Set Nonlinear
    vScpipSetUsNonlinearReg(u4VdpId);
    //23. Set 3D mode register
    vScpipSet3DModeReg(u4VdpId, scalerInfo->rTdtvInfo.eTdtvInType);
    //24. Set 3D FS freerun register in pscan dispmode
    vScpipSet3DLRFreerunReg(u4VdpId,scalerInfo->rTdtvInfo.eTdtvInType);	

    #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
    _vScpipSet4K2KDramMode(scalerInfo->fscIn.u2Width);
    #endif    
}

static void _vScpipUpdateDramMNRatio(UINT8 bPath)
{
    VDP_SCALER_PRM_T* scalerInfo;    
    UINT16 u2SubInFR;
    UINT16 u2SubOutFR;
    UINT16 u2GCD;
    UINT8 m;
    UINT8 n;
    UINT8 mnEnable;

    scalerInfo = getScalerInfo(bPath);
    m = 0;
    n = 0;
    mnEnable = SV_FALSE;

    u2SubInFR = scalerInfo->fscIn.u2FrameRate;
    u2SubOutFR = scalerInfo->fscOut.u2FrameRate; 

    LOG(3,"SubInFR(%d) SubOutFR(%d)\n", u2SubInFR,u2SubOutFR);

    if((u2SubInFR !=0) && (u2SubOutFR !=0) && (u2SubInFR > u2SubOutFR) && _u4ForceDispOff[bPath]==0)
    {
        u2GCD = GCD(u2SubInFR,u2SubOutFR);
        if(u2GCD!=0)
        {
            m = u2SubInFR / u2GCD;
            n = u2SubOutFR / u2GCD;
            mnEnable = SV_TRUE;
            LOG(3, "m(%d) n(%d) mnEnable(%d)\n", m, n,mnEnable);
        }
    }
    
    vScpipDramFrameDropEn(bPath, mnEnable);
    vScpipSetFrameDropRatio(bPath,n,m);    
}

static void _vScpipUpdateDramPrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    VDP_SCALER_PRM_T* scalerInfo;
    SCALER_DRAM_ADDR_T* dramAddrInfo;
    SCALER_FBM_POOL_T* fbmInfo;
    //UINT32 u4FbmWidth;
    //UINT32 u4FbmHeight;
    UINT32 u4FbmTotalSize;
    UINT8 bIs10Bit;
    UINT8 bIs422;
    UINT32 u4FrameNum;
    UINT32 u4ScaledMSB;
    UINT32 u4ScaledLSB;
    UINT32 u4ScaledPage;
    UINT32 u4Padding;

    bPath = getScalerPath(u4VdpId);
    scalerInfo = getScalerInfo(bPath);
    dramAddrInfo = getDramAddrInfo(u4VdpId);
    fbmInfo = getFbmPoolInfo(u4VdpId);    
    u4FrameNum = dramAddrInfo->FrameNum;    
    //u4FbmWidth = fbmInfo->u4Width;
    //u4FbmHeight = fbmInfo->u4Height;
    u4FbmTotalSize = (fbmInfo->u4FbmSize) >> 11;  //total page number from FBM
    if (u4FbmTotalSize == 0)
    {
        return;
    }
    
    bIs10Bit = fbmInfo->fg10bit;
    bIs422 = fbmInfo->fg422;

    u4ScaledMSB = ((scalerInfo->u4DramHPitch + 255) / 256) * (((scalerInfo->u4DramVPitch * u4FrameNum) + 7) / 8);
    if(bIs10Bit == SV_TRUE)
    {
        u4ScaledLSB = ((scalerInfo->u4DramHPitch + 255) / 256) * (((scalerInfo->u4DramVPitch * u4FrameNum) + 255) / 256) * 8;    
    }
    else
    {
        u4ScaledLSB = 0;
    }

    if(bIs10Bit == SV_TRUE && bIs422 == SV_TRUE)          //422 10 bit
    {
        u4ScaledPage = (u4ScaledMSB * 2) + (u4ScaledLSB * 2);
        u4Padding = 8;
    }
    else if(bIs10Bit == SV_FALSE && bIs422 == SV_TRUE)    //422 8 bit
    {
        u4ScaledPage = u4ScaledMSB * 2;
        u4Padding = 8;
    }
    else if(bIs10Bit == SV_TRUE && bIs422 == SV_FALSE)    //444 10 bit
    {
        u4ScaledPage = (u4ScaledMSB * 3) + (u4ScaledLSB * 3);
        u4Padding = 12;
    }
    else if(bIs10Bit == SV_FALSE && bIs422 == SV_FALSE)   //444 8 bit
    {
        u4ScaledPage = (u4ScaledMSB * 3);
        u4Padding = 12;
    }
    else
    {
        u4ScaledPage = 0;
        u4Padding = 0;
    }

    LOG(1, "Scaled width (%d) height (%d)\n", scalerInfo->u4DramHPitch, scalerInfo->u4DramVPitch);
    LOG(1, "MSB Page Num(%d) x %d\n", u4ScaledMSB, (bIs422==1)?2:3);
    if(bIs10Bit == SV_TRUE)
    {
        LOG(1, "LSB Page Num(%d) x %d\n", u4ScaledLSB, (bIs422==1)?2:3);
    }
    if(u4FbmTotalSize < u4ScaledPage + u4Padding)
    {
        LOG(0, "After scaling FBM buffer to small!!! %d < %d + %d\n", u4FbmTotalSize, u4ScaledPage, u4Padding);
    }
    LOG(1, "Scaler DRAM debug size FBM %d Scaler %d Padding %d\n", u4FbmTotalSize, u4ScaledPage, u4Padding);
}

void vScpipOnVdoModeChange(UINT8 bPath)
{
    if(bPath == SV_VP_MAIN)
    {
        //turn off frame track
        vScpipSwitchFrameTrackOnOff(SV_OFF, SV_OFF);
        //turn off scaler dispmode
        vSwitchDispMode(SV_OFF);
        //set to dram mode
        if (u1ScpipGetDispMode(SV_VP_MAIN) != VDP_SCPOS_PSCAN_DISPMODE)
        {
            _VDP_DispModeEnable((UCHAR)SV_VP_MAIN, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
        }
    }
}

void vScpipOnVdoModeChangeDone(UINT8 bPath)
{    
}

void vScpipSetImportEnable(UINT8 bPath, UINT32 u4OnOff)
{   
    UINT8 bOnOff;
    
    //1. force import
    if(getScalerInfo(bPath)->eForceImport != SCALER_FORCE_IMPORT_UNKNOWN)
    {
        bOnOff = getScalerInfo(bPath)->eForceImport;
    }
    else
    {
        bOnOff = u4OnOff;
    }

    //2. If it's not under set resolution mode, don't enable import.
    if(vDrvGetImportTriggerModule(bPath) == IMPORT_MODULE_NONE)
    {
        bOnOff = SV_OFF;
    }

    vScpipSetResImportEnable(bPath, bOnOff);
}

void vScpipSetTimeGenDlyImportEnable(UINT8 bPath, UINT32 u4OnOff)
{
    UINT32 u4OldOnOff=0;
    
    u4OldOnOff=vScpipGetTimeGenDlyImportOnOff(bPath);   
    if(u4OldOnOff!=u4OnOff)
    {
         vScpipSetTimeGenDlyImportOnOff(bPath,((u4OnOff!=0)?SV_ON:SV_OFF));
    }
}

void vScpipQueryISR(void)
{
    LOG(0, "ISR enable 0x%X\n", u4ScpipGetISREn(0xFF));     // default: read all
    LOG(0, "ISR status 0x%X\n", u4ScpipGetISRstatus(0xFF)); // default: read all
}

/**
 * @brief Set Scaler interrupt
 * @param u2Vector
 */

#ifdef CC_SUPPORT_VENC
EXTERN E_SCALER_VENC_RECORD_MODE eScalerVencFldMode;
EXTERN E_SCALER_VENC_INPUT_FLE_INFO eScalerVencFldInfo;
extern UINT8 bVss420;
#endif

void (*_pfVSS420Cb)(void) = NULL;
void (*_pfVdoVSSCb)(void) = NULL;

EXTERN UINT8 VssWriteFreeze ;
EXTERN UINT8 VssWriteUnFreeze[GFX_SRC_SEL_MAX];

static void _vDrvScpipISR(UINT16 u2Vector)
{
    static UINT32 CRC_Min, CRC_Mout;
    static UINT8  rptr, wptr;

    ASSERT(u2Vector == VECTOR_SCALER);

    if (u4ScpipGetISRstatus(0xFF))
    {
        LOG(9, "_vDrvScpipISR\n");
    }

    if(u4ScpipGetISRstatus(ISR_FSC_MAIN_VSYNC_IN_RISING)) // 0
    {
        LOG(7, "ISR_FSC_MAIN_VSYNC_IN_RISING %d\n", u4ScpipGetOutVCnt());
		if(VssWriteUnFreeze[GFX_MAIN_SCL] == SV_TRUE)
		{
			u1Scpip_GFX_Write_Enable(SV_TRUE);
			VssWriteUnFreeze[GFX_MAIN_SCL] = SV_FALSE;
		}
        vScpipClearISRstatus(ISR_FSC_MAIN_VSYNC_IN_RISING);
    }

    if(u4ScpipGetISRstatus(ISR_FSC_MAIN_VSYNC_IN_FALLING)) // 1
    {
        LOG(7, "ISR_FSC_MAIN_VSYNC_IN_FALLING %d\n", u4ScpipGetOutVCnt());
        vScpipClearISRstatus(ISR_FSC_MAIN_VSYNC_IN_FALLING);
    }

    if(u4ScpipGetISRstatus(ISR_FSC_SUB_VSYNC_IN_RISING)) // 2
    {
        LOG(7, "ISR_FSC_SUB_VSYNC_IN_RISING %d\n", u4ScpipGetOutVCnt());
        vScpipClearISRstatus(ISR_FSC_SUB_VSYNC_IN_RISING);
    }

    if(u4ScpipGetISRstatus(ISR_FSC_SUB_VSYNC_IN_FALLING)) // 3
    {
        LOG(6, "ISR_FSC_SUB_VSYNC_IN_FALLING %d\n", u4ScpipGetOutVCnt());
        
        #ifdef CC_SUPPORT_VENC
        if(u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER && eScalerVencFldMode != E_SCALER_VENC_PROGRESSIVE)
        { 
            eScalerVencFldInfo = (E_SCALER_VENC_INPUT_FLE_INFO)vScpipGetPipFldInfo();
        }
        #endif
        vScpipClearISRstatus(ISR_FSC_SUB_VSYNC_IN_FALLING);

    }

    if(u4ScpipGetISRstatus(ISR_FSC_VSYNC_OUT_RISING)) // 4
    {
        LOG(7, "ISR_FSC_VSYNC_OUT_RISING %d\n", u4ScpipGetOutVCnt());
		#if defined(CC_MT5399)
	    vIO32WriteFldAlign(MJC_PD_INFO_01, IO32ReadFldAlign(MJC_PD_INFO_01, MJC_PD_INFO_MIB_IN), MJC_PD_INFO_SCL_IN);
	    vIO32WriteFldAlign(MJC_PD_INFO_03, IO32ReadFldAlign(MJC_PD_INFO_03, MJC_FW_FRM_SCH_SEQ_SCL_IN_DL1), MJC_FW_FRM_SCH_SEQ_SCL_IN_DL2);
	    vIO32WriteFldAlign(MJC_PD_INFO_03, IO32ReadFldAlign(MJC_PD_INFO_02, MJC_FW_FRM_SCH_SEQ_SCL_IN), MJC_FW_FRM_SCH_SEQ_SCL_IN_DL1);
	    vIO32WriteFldAlign(MJC_PD_INFO_02, IO32ReadFldAlign(MJC_PD_INFO_02, MJC_FW_FRM_SCH_SEQ_MIB_IN), MJC_FW_FRM_SCH_SEQ_SCL_IN);
	    vIO32WriteFldAlign(MJC_PD_INFO_05, IO32ReadFldAlign(MJC_PD_INFO_05, MJC_FW_FRM_SCH_SEQ_SCL_IN_DL1_R), MJC_FW_FRM_SCH_SEQ_SCL_IN_DL2_R);
	    vIO32WriteFldAlign(MJC_PD_INFO_05, IO32ReadFldAlign(MJC_PD_INFO_04, MJC_FW_FRM_SCH_SEQ_SCL_IN_R), MJC_FW_FRM_SCH_SEQ_SCL_IN_DL1_R);
	    vIO32WriteFldAlign(MJC_PD_INFO_04, IO32ReadFldAlign(MJC_PD_INFO_04, MJC_FW_FRM_SCH_SEQ_MIB_IN_R), MJC_FW_FRM_SCH_SEQ_SCL_IN_R);
	    #endif

        #if 0//defined(SCALER_WA1_GFX)
        if (_VSS_SW_Trigger)
        {
            LOG(1, "DEBUG GFX WEN %d\n", u1ScpipGFXGetWriteStatus());
            if(u1ScpipGFXGetWriteStatus() == 0)
            {
                vScpipGfxWriteTrigger(1);
                LOG(1, "DEBUG trigger again\n");
            }
            else
            {                
                _VSS_SW_Trigger = 0;
                LOG(1, "DEBUG clear SW trigger\n");
            }
        }
        #endif
        
        vScpipClearISRstatus(ISR_FSC_VSYNC_OUT_RISING);
    }

    if(u4ScpipGetISRstatus(ISR_FSC_VSYNC_OUT_FALLING)) // 5
    {
        UINT8 u1Mchg = 0;

        LOG(6, "DEBUG R old %d new %d, W old %d new %d\n", rptr, u4ScpipGetDramRStatus(SV_VP_MAIN), wptr, u4ScpipGetDramWStatus(SV_VP_MAIN));
        wptr = u4ScpipGetDramWStatus(SV_VP_MAIN);
        if (rptr != u4ScpipGetDramRStatus(SV_VP_MAIN))
        {
            rptr  = u4ScpipGetDramRStatus(SV_VP_MAIN);
        }
        else
        {
            LOG(5, "DEBUG %d %d frame drop/repeat\n", rptr, u4ScpipGetDramRStatus(SV_VP_MAIN));
        }
        if (rptr == wptr)
        {
            LOG(5, "DEBUG %d %d R/W the same frame\n", rptr, wptr);
        }

        if (CRC_Min != u4ScpipCRCsysVstatus(SV_VP_MAIN, 0))
        {
            u1Mchg = 1;
            CRC_Min  = u4ScpipCRCsysVstatus(SV_VP_MAIN, 0);
        }

        if (CRC_Mout != u4ScpipCRCsysVstatus(SV_VP_MAIN, 1))
        {
            u1Mchg = 1;
            CRC_Mout  = u4ScpipCRCsysVstatus(SV_VP_MAIN, 1);
        }

        if( u1Mchg == 1)
        {
            LOG(5, "CRC @ ISR %8X %8X changed\n", CRC_Min, CRC_Mout);    
        }
        
        LOG(7, "ISR_FSC_VSYNC_OUT_FALLING %d\n", u4ScpipGetOutVCnt());
        
        if(bScpipIsNeedSetUpdate(getScalerPath(SV_VP_MAIN)) == SV_TRUE)
        {
           vScpipSetOutputResolution(getScalerPath(SV_VP_MAIN), getScalerInfo(getScalerPath(SV_VP_MAIN))->u4OutputWidth, getScalerInfo(getScalerPath(SV_VP_MAIN))->u4OutputHeight);
           vScpipSetMaskStart(getScalerPath(SV_VP_MAIN), getScalerInfo(getScalerPath(SV_VP_MAIN))->fscOut.u2X, getScalerInfo(getScalerPath(SV_VP_MAIN))->fscOut.u2Y); 
        }

        if(bScpipIsNeedSetUpdate(getScalerPath(SV_VP_PIP)) == SV_TRUE)
        {
           vScpipSetOutputResolution(getScalerPath(SV_VP_PIP), getScalerInfo(getScalerPath(SV_VP_PIP))->u4OutputWidth, getScalerInfo(getScalerPath(SV_VP_PIP))->u4OutputHeight);
           vScpipSetMaskStart(getScalerPath(SV_VP_PIP), getScalerInfo(getScalerPath(SV_VP_PIP))->fscOut.u2X, getScalerInfo(getScalerPath(SV_VP_PIP))->fscOut.u2Y); 
        }
		vScpipClearNeedUpdateFlag();
        
        vScpipClearISRstatus(ISR_FSC_VSYNC_OUT_FALLING);
    }

    if(u4ScpipGetISRstatus(ISR_FSC_VDE_OUT_RISING)) // 6
    {
        LOG(7, "ISR_FSC_VDE_OUT_RISING %d\n", u4ScpipGetOutVCnt());
        vScpipClearISRstatus(ISR_FSC_VDE_OUT_RISING);
    }

    if(u4ScpipGetISRstatus(ISR_FSC_VDE_OUT_FALLING)) // 7
    {
        LOG(7, "ISR_FSC_VDE_OUT_FALLING %d\n", u4ScpipGetOutVCnt());
        DRVCUST_PostDEEndISR();
        vScpipClearISRstatus(ISR_FSC_VDE_OUT_FALLING);
    }

    if(u4ScpipGetISRstatus(ISR_PSC_VSYNC_OUT_RISING)) // 8
    {
        LOG(7, "ISR_PSC_VSYNC_OUT_RISING %d\n", u4ScpipGetOutVCnt());
        vScpipClearISRstatus(ISR_PSC_VSYNC_OUT_RISING);
    }

    if(u4ScpipGetISRstatus(ISR_PSC_VSYNC_OUT_FALLING)) // 9
    {
        LOG(7, "ISR_PSC_VSYNC_OUT_FALLING %d\n", u4ScpipGetOutVCnt());
        vScpipClearISRstatus(ISR_PSC_VSYNC_OUT_FALLING);
    }

    if(u4ScpipGetISRstatus(ISR_PSC_VDE_OUT_RISING)) // 10
    {
        LOG(7, "ISR_PSC_VDE_OUT_RISING %d\n", u4ScpipGetOutVCnt());
        vScpipClearISRstatus(ISR_PSC_VDE_OUT_RISING);
    }

    if(u4ScpipGetISRstatus(ISR_PSC_VDE_OUT_FALLING)) // 11
    {
        LOG(7, "ISR_PSC_VDE_OUT_FALLING %d\n", u4ScpipGetOutVCnt());
        vScpipClearISRstatus(ISR_PSC_VDE_OUT_FALLING);
    }

    if(u4ScpipGetISRstatus(ISR_DRAM_MAIN_WRITE)) // 12
    {
        LOG(7, "ISR_DRAM_MAIN_WRITE %d\n", u4ScpipGetOutVCnt());
        #ifdef CC_SUPPORT_VENC
        vDrvScpipVEncISR();
        #endif
        vScpipClearISRstatus(ISR_DRAM_MAIN_WRITE);
    }

    if(u4ScpipGetISRstatus(ISR_DRAM_SUB_WRITE)) // 13
    {
        LOG(6, "ISR_DRAM_SUB_WRITE %d\n", u4ScpipGetOutVCnt());
        #ifdef CC_SUPPORT_VENC
        vDrvScpipVEncISR();
        #endif
        vScpipClearISRstatus(ISR_DRAM_SUB_WRITE);
    }

    if(u4ScpipGetISRstatus(ISR_DRAM_GFX_WRITE)) // 14
    {
        LOG(3, "ISR_DRAM_GFX_WRITE %d\n", u4ScpipGetOutVCnt());
        LOG(3, "GFX interrupt\n");
        if (_pfVdoVSSCb != NULL)
        {
            _pfVdoVSSCb(); // trigger callback
        }

		if(VssWriteFreeze == SV_TRUE)
		{
			u1Scpip_GFX_Write_Enable(SV_FALSE);
			VssWriteFreeze = SV_FALSE;
		}
        vScpipClearISRstatus(ISR_DRAM_GFX_WRITE);
    }

    if(u4ScpipGetISRstatus(ISR_DRAM_MON_WRITE)) // 15
    {
        LOG(6, "ISR_DRAM_MON_WRITE %d\n", u4ScpipGetOutVCnt());
        #ifdef CC_SUPPORT_VENC
        if(u1DrvScpipGetVencModule() == E_VENC_MODULE_MON_SCALER && eScalerVencFldMode != E_SCALER_VENC_PROGRESSIVE)
        {
           eScalerVencFldInfo = u1MonOutGetFldInfo();
        }
		if ( bVss420 == ON )
		{	
		   vDrvScpipVss420ISR(); 
	    }
		else	
	    {
		   LOG(3,"ISR_DRAM_MON_WRITE---Venc write done\n");
           vDrvScpipVEncISR();
	    }   
        #endif
        vScpipClearISRstatus(ISR_DRAM_MON_WRITE);
    }

    VERIFY(BIM_ClearIrq(VECTOR_SCALER));
}

/**
 * @brief Set Scaler init interrupt
 */
static void _vDrvScpipInitISR(void)
{
    static UINT8 u1Initialized = SV_FALSE;
    x_os_isr_fct pfnOldIsr;

    if (!u1Initialized)
    {
        if (x_reg_isr(VECTOR_SCALER, _vDrvScpipISR, &pfnOldIsr) != OSR_OK) // todo
        {
            LOG(0, "Error: fail to register SCPIP ISR!\n");
        }
        else
        {
            LOG(0, "Register SCPIP ISR OK!\n");
        }

        u1Initialized = SV_TRUE;
        LOG(2, "SCPIP ISR registered!\n");
    }
}

void vScpipSetISR()
{
    _vDrvScpipInitISR();
    // ISR enable
    #if defined(CC_MT5399) || defined (CC_MT5882)
    vScpipSetISREn(ISR_FSC_VSYNC_OUT_RISING, SV_TRUE);
    vScpipSetISREn(ISR_FSC_VSYNC_OUT_FALLING,SV_TRUE);
	vScpipSetISREn(ISR_FSC_MAIN_VSYNC_IN_RISING,SV_TRUE);
	vScpipSetISREn(ISR_DRAM_GFX_WRITE,SV_TRUE);
	#endif 
}

void u1DrvScpipSetVencModule(UINT8 u1VencModule)
{
    if (u1VencModule > E_VENC_MODULE_MAX)
    {
        LOG(0, "Invalid Venc Module %d!!\n", u1VencModule);
        return;
    }

    if (u1ScalerVencModule != u1VencModule)
    {
        LOG(2, "%s Change Scaler VENC Module [%d]=>[%d]\n", __FUNCTION__, u1ScalerVencModule, u1VencModule);
        u1ScalerVencModule  = u1VencModule;
    }    
}

UINT8 u1DrvScpipGetVencModule(void)
{
    return u1ScalerVencModule;
}

/**
 * @brief Scpos main path normal procedure
 */
static void _vScpipNormProcByPath(const UINT32 u4VdpId) 
{
    UINT32 u4Update;
    UINT8 bPath;
    BOOL fgIsMain;
    RVChannel* channel;
    BOOL fgChannelIsFreeze;
    #ifdef __MODEL_slt__
    UINT8 bIsChannelOn;
    #endif

    //Variable init
    u4Update = 0;
    bPath = getScalerPath(u4VdpId);
    fgIsMain = (u4VdpId == VDP_1);
    channel = getChannel(bPath);
    fgChannelIsFreeze = channel->bIsFreeze;
    #ifdef __MODEL_slt__
    bIsChannelOn = channel->bIsChannelOn;
    #endif    

    if(bLOGModule != 0 && bLOGPath == bPath)
    {
        if (bLOGCnt != 0)
        {
            bLOGCnt--;
        }
        else
        {        
        //LOG(0, " %d %d\n", bLOGModule, bLOGDelay);
        if (bLOGModule & PD_LOG_STATUS)
        {
            LOG(1, "PD In(%4dx%4d %4dx%4d) Out(%4dx%4d %4dx%4d)\n", u4PDSGetHActive(bPath, 0), u4PDSGetVActive(bPath, 0), u4PDSGetHTotal(bPath, 0), u4PDSGetVTotal(bPath, 0), u4PDSGetHActive(bPath, 1), u4PDSGetVActive(bPath, 1), u4PDSGetHTotal(bPath, 1), u4PDSGetVTotal(bPath, 1));
        }       
        if (bLOGModule & RG_LOG_STATUS)
        {
            LOG(1, "RG In(%4dx%4d) Out(%4dx%4d)\n", u4RegenGetInWidth(bPath,REGEN_MODULE_PRE_REGEN), u4RegenGetInHeight(bPath,REGEN_MODULE_PRE_REGEN), u4RegenGetOutWidth(bPath,REGEN_MODULE_PRE_REGEN), u4RegenGetOutHeight(bPath,REGEN_MODULE_PRE_REGEN));
        }
        if (bLOGModule & SC_LOG_STATUS)
        {
            LOG(1, "SC In(%4dx%4d %4dx%4d) Out(%4dx%4d %4dx%4d)\n", u4ScpipGetInputHActiveStatus(bPath), u4ScpipGetInputVActiveStatus(bPath), u4ScpipGetInputHTotalStatus(bPath), u4ScpipGetInputVTotalStatus(bPath), u4ScpipGetOutputHActiveStatus(bPath), u4ScpipGetOutputVActiveStatus(bPath), u4ScpipGetOutputHTotalStatus(bPath), u4ScpipGetOutputVTotalStatus(bPath));
        }
        if (bLOGModule & PS_LOG_STATUS)
        {
            LOG(1, "PS In(%4dx%4d %4dx%4d) Out(%4dx%4d %4dx%4d)\n", vPscGetInputHactive(), vPscGetInputVactive(), vPscGetInputHtotal(), vPscGetInputVtotal(), u4PscGetOutputHActiveStatus(), u4PscGetOutputVActiveStatus(), u4PscGetOutputHTotalStatus(), u4PscGetOutputVTotalStatus());
        }
        if (bLOGModule & FT_LOG_STATUS)
        {
            LOG(1, "FT L(%d) V(%d-%d) VT(%d)\n", u4ScpipGetFrametrackLockStatus(), u4ScpipGetInOutVDistance(), u4ScpipGetFrametrackABSDistance(), u4ScpipGetFrametrackVTotalStatus());
        }
            bLOGCnt = bLOGDelay;
        }
    }

    vScpipCheckWriteCtrl(u4VdpId);

    //@Cobra import not ready, just return from this function. Do we need to do this in Cobra??
    if((_u4ScposImportNotReady[u4VdpId] != 0) && bScpipGetDramRead(u4VdpId) == SV_ON)
    {
        _u4ScposImportNotReady[u4VdpId]--;
        return;  // import not ready, return from this function without any display parameter change          
    }
    else
    {
        _u4ScposImportNotReady[u4VdpId]=0;
    }
    
    //handle freeze count down
    _vScpipHandleChannelFreeze(u4VdpId);
   
    if ((fgIsScposFlgSet((fgIsMain) ? (MAIN_DISP_PRM_CHG|MAIN_HV_CHG) : (PIP_DISP_PRM_CHG|PIP_HV_CHG))) && 
        (fgChannelIsFreeze==SV_FALSE)) // if still in freeze, don't apply scaler mode change
    {        
        #ifdef __MODEL_slt__
        if (bIsChannelOn && (!fgChannelIsFreeze))   //channel is on & not freezed
        #endif
        {  
            //update display parameter
            _vScpipUpdateDispPrm(u4VdpId);
            u4Update = 1;
            _vScpipUpdateDramPrm(u4VdpId); // for Cobra dram page usage check
        }
        //clear scaler mode change flag
        vClrScposFlg((fgIsMain) ? MAIN_DISP_PRM_CHG : PIP_DISP_PRM_CHG);
        vClrScposFlg((fgIsMain) ? MAIN_HV_CHG : PIP_HV_CHG);
    }            

    if (u4Update)
    {
        UINT8 bInputRefreshRate = bDrvVideoGetRefreshRate(u4VdpId);
        //@6896 review this. need to do this??
        if((bInputRefreshRate <=30) && (bInputRefreshRate>0)) 
        {
            _u4ScposImportNotReady[u4VdpId] = (VDP_SCPOS_IMPORT_PROTECT * vDrvGetLCDFreq() + (bInputRefreshRate -1))/bInputRefreshRate;
        }
        else
        {
            _u4ScposImportNotReady[u4VdpId] = VDP_SCPOS_IMPORT_PROTECT;
        }

        //2. Update register
        _vScpipSetDispReg(u4VdpId);
        //3. Update Poclk & frame track setting
        if(fgIsMain)
        {
            #if defined(CC_MT5399)
            if (fgIsMJCToOSTG())
            {   
                //1. Set POCLK           
               vScpipConfigPOCLK(getScalerMode(bPath));
            }
            #endif        
            //2. reconfig Scaler frame track (both FSC & PSC)
            vFrameTrackConfig(1);
        }        
        else
        {
            if(!((bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC)) && (u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER)))
            {
                _vScpipUpdateDramMNRatio(bPath);
            }
        }
        
        //4. trigger import
        vDrvScpipClrSetRes(bPath, SET_RES_BOTH);

        //5.Resolution Reg update done
        vDrvClrImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_FSC);
        
    }

    // dispmode mode change handling
    if (fgIsMain && fgIsScposFlgSet(SCALER_MAIN_MODE_CHG))
    {
        LOG(1, "dispmode change %d to M(%d)\n", bPath, getScalerMode(bPath));
        
        vScpipModeSwitch(SV_VP_MAIN); 
        vClrScposFlg(SCALER_MAIN_MODE_CHG);        
    }
    else if (bPath == SV_VP_PIP && fgIsScposFlgSet(SCALER_PIP_MODE_CHG))
    {
        LOG(1, "dispmode change %d to M(%d)\n", bPath, getScalerMode(bPath));
        
        vScpipModeSwitch(SV_VP_PIP); 
        vClrScposFlg(SCALER_PIP_MODE_CHG); 
    }

    #if defined(SCPIP_SUPPORT_POST_SCALER)
    if(fgIsMain)
    {
        vScpipCheckSetDataTrigger();
    }
    #endif
}

/**
 * @brief Scpos normal procedure
 */
void vScpipNormProc(void) 
{
    #if !defined(CC_FPGA_SUPPORT)
    //@6896 review this.
    vOstgNormProc();

    //@6896 review this
    #ifndef CC_COPLAT_MT82
    #ifndef __MODEL_slt__
    OSD_PLA_Unmute();
    #endif //__MODEL_slt__
    #endif //CC_COPLAT_MT82

    //@6896 review this
    #ifdef DRV_SUPPORT_EXTMJC
    if(IS_COMPANION_CHIP_ON())
    {
        OSD_DMA_OnOutputVsync();
    }
    #endif

    #endif

    //Main Path Normal procedure
    _vScpipNormProcByPath(VDP_1);    

    //Sub Path Normal procedure
    #if SUPPORT_POP
    _vScpipNormProcByPath(VDP_2);
    #endif

    //Post scaler Normal procedure
    #if defined(SCPIP_SUPPORT_POST_SCALER)
    vPscNormProc();
    #endif
}

void vScpipXdataAutoCheck(UINT32 u4VdpId, UINT32 u4Width, UINT32 u4Height, UINT32 u4Hstep, UINT32 u4Vstep)
{
    _XdataAutoScanVDP = getScalerPath(u4VdpId);
    _XdataAutoScanW = u4Width;
    _XdataAutoScanH = u4Height;
    _XdataAutoScanHstep = u4Hstep;
    _XdataAutoScanVstep = u4Vstep;
}

void vScpipLOGStatus(UINT32 u4VdpId, UINT32 u4Module, UINT32 u4Delay)
{
    bLOGPath   = getScalerPath(u4VdpId);
    bLOGModule = u4Module;
    bLOGDelay  = u4Delay;
}

UINT8 u1IsMainOrSubVenc(UINT8 bPath)
{
#ifdef CC_SUPPORT_VENC
	if(bPath == VDP_2 && (bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC)) && (u1DrvScpipH264GetSource() == E_VENC_MODULE_SUB_SCALER))
	{
		return TRUE;
	}
#endif
	return FALSE;
}

SCALER_SCREEN_CAPTURE_MODE_T u4ScpipGetCaptureMode(void)
{
	IC_VERSION_T u4IcVersioin;

	u4IcVersioin = BSP_GetIcVersion();

	if((u4IcVersioin == IC_VER_5399_AA) ||
		(u4IcVersioin == IC_VER_5399_AB) ||
		(u4IcVersioin == IC_VER_5399_AC))
	{
		return SCALER_SCREEN_CAPTURE_FROM_OSTG;
	}
	else
	{
		return SCALER_SCREEN_CAPTURE_FROM_OMUX;
	}
}

#ifdef CC_SUPPORT_STR

extern void DisableVsyncInt(void);
extern void EnableVsyncInt(void);
extern void vSetPowerDown( void );
extern void vClrPowerDown( void );


//-----------------------------------------------------
//
// scaler
// added for scpos suspend/resume  function
//-----------------------------------------------------
void vDrvScposSuspend(void)
{

    DisableVsyncInt();
    vSetPowerDown();
    vDrvScposSetClock(OFF);

}

void vDrvScposResume(void)
{
	_vDrvVideoSetMute(MUTE_MODULE_MODECHG,0,FOREVER_MUTE,FALSE);
	_vDrvVideoSetMute(MUTE_MODULE_MODECHG,1,FOREVER_MUTE,FALSE);
    vDrvScposSetClock( SV_ON ); 

    vClrPowerDown();
    EnableVsyncInt();
    vDrvSCPQInit(); 

    #if defined(CC_MT5399)
    vScpipSwInit();
    vScpipSetISR();//enable ISR
    vDrvMonOutInit();
    #endif

	#if defined(CC_MT5882) 
    vScpipSwInit();
    vScpipSetISR();//enable ISR
    #endif

    vPDSHwInit(SV_VP_MAIN);
    vRegenHwInit(SV_VP_MAIN,REGEN_MODULE_PRE_REGEN);
    vRegenHwInit(SV_VP_MAIN,REGEN_MODULE_FSC_REGEN);
    vScpipHwInit(SV_VP_MAIN);
        
#if SUPPORT_POP
    vPDSHwInit(SV_VP_PIP);
    vRegenHwInit(SV_VP_PIP,REGEN_MODULE_PRE_REGEN);
    vRegenHwInit(SV_VP_PIP,REGEN_MODULE_FSC_REGEN);
    vScpipHwInit(SV_VP_PIP);
#endif
    _arOutTimingPrm.bUpdateTGLater = SV_TRUE; // force update scaler timegen.
    vDrvVRMResume();//pre down and regen need vrm resume init to 0  for [DTV00581516]
}
#endif



