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
 * $RCSfile: pmx_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_cmd.c
 *  This header file includes CLI function definitions of PMX driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pmx_drvif.h"
#include "osd_drvif.h"
#include "panel.h"
#include "drv_video.h"
#include "drv_vdoclk.h"
#include "drv_vbyone.h"
#include "drv_display.h"
#include "sv_const.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_lvds.h"
#include "drv_tcon.h"
#include "drv_pwm.h"
#include "ostg_if.h"
#include "sif_if.h"
#include "drv_ycproc.h"
#include "vdp_if.h"
#include "video_def.h"
#include "pe_if.h"

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_ckgen.h"
#include "x_hal_arm.h"
#include "x_gpio.h"
#include "frametrack_drvif.h"
#include "drv_od.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#ifdef CC_5391_LOADER
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#else /* CC_5391_LOADER */
#define DEFINE_IS_LOG PMX_IsLog
#endif /* CC_5391_LOADER */
#include "x_debug.h"


//extern void vApiHwInit(void);

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define CHK_NUM_MIN_PARAM(NUN, USAGE) \
    do \
    { \
        if (i4Argc < ((NUN) + 1)) \
        { \
            Printf("Args: %s\n", USAGE); \
            return -1; \
        } \
    } while (0)

#define PARAM(IDX) (UINT32)StrToInt(szArgv[IDX+1])


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

EXTERN UINT8 bSI_DISPLAY_DCLK_TYPE;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN UINT8 SerPollGetChar(void);
EXTERN void vDrvOutputStageInit(void);
EXTERN INT32 EEPROM_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);



//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
static INT32 _PmxInitCmd(INT32 i4Argc, const CHAR **szArgv);

static INT32 _PmxODInitCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODDMACmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODSetLocalODTblCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODOnOffCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxPCIDOnOffCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODIdxCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODBypassCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODQueryCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODOverflowResetCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODOverflowGetCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODCheckCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODStressTestCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODResetCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxODTableSelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxVOPLLStatusCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxVOPLLSetVtrl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxVOPLLSourceCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxVOPLLSetFreqCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxVOPLLCalErrorLimitCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxVOPLLTestModeCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxDVOPLLLockCheck(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_MT5882
static INT32 _PmxPLLStatusCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxPLLTestModeCmd(INT32 i4Argc, const CHAR **szArgv);
#endif

//automation
static INT32 _PmxLVDS_ATERM_ATVO_Setting(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_ATERM_ATVO_Restore(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_ATVO_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_APSRC_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_ANSRC_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_PADPD_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxLVDS_RESET_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxMLVDS_LS_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxMLVDS_TestMODE_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxDrvVCOPhase_SEL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSET_DDDSFreeRun(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSET_DDDSErrorLimit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneScript (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneCRCChk (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneImpCal (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneTestMode (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneStatus (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneLockChk (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneLockCnt (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneOn (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneOff (INT32 i4Argc, const CHAR **szArgv);
static INT32 _VByOneINXI2CCmd(INT32 i4Argc, const CHAR **szArgv);

static INT32 _PmxEnableCmd(INT32 i4Argc, const CHAR ** szArgv);
INT32 _PmxPatternCmd(INT32 i4Argc, const CHAR ** szArgv);
INT32 _PmxOutPatternChgCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxQueryCmd(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _PmxSetCmdBacklt(INT32 i4Argc, const CHAR ** szArgv);
#ifdef LVDS_ESD_PROTECT
#ifdef CC_MT5882
static INT32 _PmxSetCmdESDProtectFg(INT32 i4Argc, const CHAR ** szArgv);
#endif
#endif
static INT32 _PmxSetCmdBg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdPlaneOrder(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPlaneOrderArray(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPanel(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPanelDriverIni(INT32 i4Argc, const CHAR ** szArgv);
INT32 _PmxSetCmdOSTGPt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdPowerSequence(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxDiagCmdDumpScpos(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdDrivingCurrent(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdVCM(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdSpreadSpectrum(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _PmxSetCmdXtal(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdNs(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdJeida(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdSpecialNs(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxListPanelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdDnie(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmd10bit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdSwap(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdLvdsMod(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdQueryLvdsMod(INT32 i4Argc, const CHAR **szArgv);
#ifdef SUPPORT_CW_CHANGE_BY_STEP
static INT32 _NptvSetCtrlWordStep(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _NptvOutputStageDumpEnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvForceFrameRate(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvForceFreeRun(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetBorder(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetBorderColor(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetBorderParam(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetPanelDisplayTypeMsk(INT32 i4Argc, const CHAR **szArgv);
#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
static INT32 _PmxSetCmdVb1(INT32 i4Argc, const CHAR **szArgv);
#endif // 5395
static INT32 _PmxSetCmdCtlWord(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxSetCmdCtlWord2(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_SCPOS_3DTV_SUPPORT
static INT32 _PmxSetScanningPWM(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxGetScanningStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxGetScanningStepCtl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxGetScanningTarget(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _PmxTXCRCStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFDigitalPadSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFAnalogPadSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFGroupSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFGroupAuto(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFPairChk(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFLineBufferEn(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXHVSYNCEncodeQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXHVSYNCEncodeEnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFDISP_RQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFDISP_REnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFGlasses_Control(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFGlasses_RSTControl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFGlasses_DutyControl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFGlasses_Enable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFIS_Control(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFISDelay_Control(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFPWM_Init(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFPWM_Enable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFPWM_Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxTXIFPWM_AutoTest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxPSCmdQuery(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxPSCmdHsyncTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxPSCmdVsyncTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxPSCmdSetDataTrigger(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxINTRStatus(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxINTRSetting(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCmdMixAlpha(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxSetCustFRC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _PmxUHDStatus(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PmxUHDSetting(INT32 i4Argc, const CHAR ** szArgv);




#ifndef CC_5391_LOADER
CLIMOD_DEBUG_FUNCTIONS(PMX)
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static CLI_EXEC_T _arPmxSetCmdTbl[] =
{
    {"bg", NULL, _PmxSetCmdBg, NULL, "background color", CLI_SUPERVISOR},
    {"po", NULL, _PmxSetCmdPlaneOrder, NULL, "plane order", CLI_SUPERVISOR},
    {"poa", NULL, _PmxSetCmdPlaneOrderArray, NULL, "plane order array",
     CLI_SUPERVISOR},
    {"pma", NULL, _PmxSetCmdMixAlpha, NULL, "plane mix alpha", CLI_SUPERVISOR},
    {"backlt", "bl", _PmxSetCmdBacklt, NULL, "backlight", CLI_GUEST},
    {"driving", "d", _PmxSetCmdDrivingCurrent, NULL, "lvds driving current",
     CLI_GUEST},
    {"vcm", "vcm", _PmxSetCmdVCM, NULL, "lvds common voltage",
     CLI_GUEST},     
    {"spread", "s", _PmxSetCmdSpreadSpectrum, NULL, "lvds spread spectrum",
     CLI_GUEST},

    {"panel", "p", _PmxSetCmdPanel, NULL, "panel resolution", CLI_GUEST },
    {"driverini", "di", _PmxSetCmdPanelDriverIni, NULL, "panel driver init", CLI_GUEST },    
    {"powersequence", "ps", _PmxSetCmdPowerSequence, NULL,
     "adjust power sequence", CLI_GUEST },
    //{"xtal", "xtal", _PmxSetCmdXtal, NULL, "panel resolution", CLI_GUEST },
    {"ns", "ns", _PmxSetCmdNs, NULL, "NS lvds format", CLI_GUEST },
    {"jeida", "jeida", _PmxSetCmdJeida, NULL, "JEIDA lvds format", CLI_GUEST },
    {"SpecialNs", "sns", _PmxSetCmdSpecialNs, NULL, "Special NS lvds format", CLI_GUEST },
    {"dnie", "dnie", _PmxSetCmdDnie, NULL, "ByPass DNIE", CLI_GUEST },
    {"10bit", "10bit", _PmxSetCmd10bit, NULL, "switch LVDS to 10bit", CLI_GUEST },
    {"swap", "swap", _PmxSetCmdSwap, NULL, "even odd channel swap", CLI_GUEST },
    {"lvdsmod", NULL, _PmxSetCmdLvdsMod, NULL, "set LVDS Tx mode", CLI_GUEST},
    {"qlvdsmod", NULL, _PmxSetCmdQueryLvdsMod, NULL, "query LVDS Tx mode", CLI_GUEST},
#ifdef SUPPORT_CW_CHANGE_BY_STEP
    {"SetCtrlWordStep",  "cws",	_NptvSetCtrlWordStep,  NULL,	"Nptv Set Control Word Step",  CLI_SUPERVISOR},
#endif
    {"OutputStageDumpEnable",  "osdumpen",   _NptvOutputStageDumpEnable,   NULL,           "Enable OutputStage Dump",    CLI_SUPERVISOR},     
    {"ForceFrameRate",	"ffr",	_NptvForceFrameRate,  NULL,  "Nptv force frame rate",  CLI_SUPERVISOR},    
    {"ForceFreeRun",	"fr",	_NptvForceFreeRun,  NULL,  "Nptv force free run",  CLI_SUPERVISOR},
    {"borderonoff",	"boo",	_PmxSetBorder,  NULL,  "OSTG border",  CLI_GUEST},
    {"bordercolor",	"bc", 	_PmxSetBorderColor,  NULL,  "OSTG border color",  CLI_GUEST},
    {"borderparameter",	"bparam",	_PmxSetBorderParam,  NULL,  "OSTG border position",  CLI_GUEST},
    {"panelifmask", "pim", _PmxSetPanelDisplayTypeMsk, NULL, "Set Panel interface allowed mask", CLI_SUPERVISOR},

    //automation
    {"ATERM_ATVO", "ATERM_ATVO", _PmxLVDS_ATERM_ATVO_Setting, NULL, "PmxLVDS_ATERM_ATVO_Setting", CLI_SUPERVISOR},
    {"ATERM_ATVO_Restore", "ATERM_ATVO_Restore", _PmxLVDS_ATERM_ATVO_Restore, NULL, "PmxLVDS_ATERM_ATVO_Restore", CLI_SUPERVISOR},
    {"ATVO_Set", "ATVO_Set", _PmxLVDS_ATVO_Set, NULL, "PmxLVDS_ATVO_Set", CLI_SUPERVISOR},
    {"APSRC_Set", "APSRC_Set", _PmxLVDS_APSRC_Set, NULL, "PmxLVDS_APSRC_Set", CLI_SUPERVISOR},
    {"ANSRC_Set", "ANSRC_Set", _PmxLVDS_ANSRC_Set, NULL, "PmxLVDS_ANSRC_Set", CLI_SUPERVISOR},
    {"PADPD_Set", "PADPD_Set", _PmxLVDS_PADPD_Set, NULL, "PmxLVDS_PADPD_Set", CLI_SUPERVISOR},
    {"RESET_Set", "RESET_Set", _PmxLVDS_RESET_Set, NULL, "PmxLVDS_RESET_Set", CLI_SUPERVISOR},
    {"LS_Set", "LS_Set", _PmxMLVDS_LS_Set, NULL, "PmxMLVDS_LS_Set", CLI_SUPERVISOR},
    {"TestMODE_Set", "TestMODE_Set", _PmxMLVDS_TestMODE_Set, NULL, "PmxMLVDS_TestMODE_Set", CLI_SUPERVISOR},
    {"VCOPhase_SEL", "VCOPhase_SEL", _PmxDrvVCOPhase_SEL, NULL, "PmxDrvVCOPhase_SEL", CLI_SUPERVISOR},
    {"DDDSFreeRun", "DDDSFreeRun", _PmxSET_DDDSFreeRun, NULL, "PmxSET_DDDSFreeRun", CLI_SUPERVISOR},
    {"DDDSErrorLimit", "dddsel", _PmxSET_DDDSErrorLimit, NULL, "PmxSET_DDDSErrorLimit", CLI_SUPERVISOR},
    {"controlword", "ctlw",  _PmxSetCmdCtlWord, NULL, "Set control word", CLI_SUPERVISOR},
    {"controlword2", "ctlw2",  _PmxSetCmdCtlWord2, NULL, "Set control word2", CLI_SUPERVISOR},    
#ifdef CC_SCPOS_3DTV_SUPPORT
    {"scanningpwm", "scanpwm",  _PmxSetScanningPWM, NULL, "Set scanning pwm", CLI_SUPERVISOR},
    {"scanstatus", "scanpwms",  _PmxGetScanningStatus, NULL, "Get scanning status", CLI_SUPERVISOR},    
    {"scanstep", "scanstep",  _PmxGetScanningStepCtl, NULL, "Set scanning step control", CLI_SUPERVISOR},    
    {"scantarget", "scantarget",  _PmxGetScanningTarget, NULL, "Set scanning step target", CLI_SUPERVISOR},        
#endif
#ifdef LVDS_ESD_PROTECT
#ifdef CC_MT5882
	{"ESDfg", NULL, _PmxSetCmdESDProtectFg, NULL, "ESDProtectflag", CLI_SUPERVISOR},
#endif
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxDiagCmdTbl[] =
{
    {"dump", NULL, _PmxDiagCmdDumpScpos, NULL, "dump scpos", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxPSCmdTbl[] =
{
    {"query status", "q", _PmxPSCmdQuery, NULL, "query post scaler status", CLI_GUEST},
    {"ps hsynctest", "ht", _PmxPSCmdHsyncTest, NULL, "hsync scaler timing test", CLI_SUPERVISOR},		
    {"ps vsynctest", "vt", _PmxPSCmdVsyncTest, NULL, "vsync scaler timing test", CLI_SUPERVISOR},		    
    {"ps set_data trigger", "st", _PmxPSCmdSetDataTrigger, NULL, "set data trigger", CLI_GUEST},		        
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};	

static CLI_EXEC_T _arPmxODCmdTbl[] =
{
    {"init", "i", _PmxODInitCmd, NULL, "OD initial", CLI_SUPERVISOR},
    {"onoff", "on", _PmxODOnOffCmd, NULL, "OD enable", CLI_SUPERVISOR},
    {"pcid onoff", "pcidon", _PmxPCIDOnOffCmd, NULL, "PCID enable", CLI_SUPERVISOR},
    {"odidx", "idx", _PmxODIdxCmd, NULL, "OD index", CLI_SUPERVISOR},
    {"bypass", "bypass", _PmxODBypassCmd, NULL, "OD bypass", CLI_SUPERVISOR},
    {"query", "q", _PmxODQueryCmd, NULL, "OD query status", CLI_SUPERVISOR},
    {"overflow reset", "oreset", _PmxODOverflowResetCmd, NULL, "OD overflow reset", CLI_SUPERVISOR},
    {"overflow get", "oget", _PmxODOverflowGetCmd, NULL, "OD overflow get", CLI_SUPERVISOR},
    {"Check", "chk", _PmxODCheckCmd, NULL, "OD check", CLI_SUPERVISOR},
    {"stress test", "st", _PmxODStressTestCmd, NULL, "OD stress test", CLI_SUPERVISOR},
     {"od reset", "reset", _PmxODResetCmd, NULL, "OD reset test", CLI_SUPERVISOR},
     {"od table select", "sel", _PmxODTableSelCmd, NULL, "OD table select", CLI_SUPERVISOR},
     {"od dma", "dma", _PmxODDMACmd, NULL, "OD DMA", CLI_SUPERVISOR},
     {"set local od", "lcod", _PmxODSetLocalODTblCmd, NULL, "Set Local OD", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxVOPLLCmdTbl[] =
{
    {"status", "s", _PmxVOPLLStatusCmd, NULL, "vopll status", CLI_SUPERVISOR},
    {"xtal", "xtal", _PmxVOPLLSourceCmd, NULL, "vopll source from xtal", CLI_SUPERVISOR},    
    {"freq", "freq", _PmxVOPLLSetFreqCmd, NULL, "set vopll frequency", CLI_SUPERVISOR},
    {"calerrorlimit", "calel", _PmxVOPLLCalErrorLimitCmd, NULL, "calculate error limit", CLI_SUPERVISOR},      
    {"Vtrl", "vtrl", _PmxVOPLLSetVtrl, NULL, "VOPLL VCO monitor out", CLI_SUPERVISOR},      
    {"Test Mode", "tm", _PmxVOPLLTestModeCmd, NULL, "VOPLL Test Mode with SS", CLI_SUPERVISOR},          
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxDVOPLLCmdTbl[] =
{
    {"status", "s", _PmxVOPLLStatusCmd, NULL, "dvopll status", CLI_SUPERVISOR},
    {"chk", "chk", _PmxDVOPLLLockCheck, NULL, "dvopll check", CLI_SUPERVISOR},    
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_MT5882
static CLI_EXEC_T _arPmxPLLCmdTbl[] =
{
    {"status", "s", _PmxPLLStatusCmd, NULL, "PLL freqence status", CLI_SUPERVISOR},    
	{"Test Mode", "tm", _PmxPLLTestModeCmd, NULL, "PLL VCO control voltage", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arPmxIFCmdTbl[] =
{
    {"LVDS tx crc","crc", _PmxTXCRCStatus,       NULL,           "LVDS TX crc",  CLI_SUPERVISOR},      
	{"digital pad on/off","dpen", _PmxTXIFDigitalPadSet,       NULL,           "LVDS digital pad on/off",  CLI_GUEST},
	{"analog pad on/off","apen", _PmxTXIFAnalogPadSet,       NULL,           "LVDS analog pad on/off",  CLI_GUEST},
    {"set group","sgroup", _PmxTXIFGroupSet,       NULL,           "LVDS set group",  CLI_GUEST},      				
    {"auto switch","agroup", _PmxTXIFGroupAuto,       NULL,           "LVDS switch group auto",  CLI_GUEST},      		
    {"cable chk","cc", _PmxTXIFPairChk,       NULL,           "Check DE and Clcok with LVDS cable ",  CLI_GUEST},      		    
    {"6 line buffer","lineb", _PmxTXIFLineBufferEn,       NULL,           "6 line buffer bypass or enable",  CLI_SUPERVISOR},         
    {"h/vsync query","hvq", _PmxTXHVSYNCEncodeQuery,       NULL,           "h/vsync encode query",  CLI_GUEST},             
    {"h/vsync set","hvset", _PmxTXHVSYNCEncodeEnable,       NULL,           "h/vsync encode value set",  CLI_GUEST},                     
    {"disp_r query","disprq", _PmxTXIFDISP_RQuery,       NULL,           "DISP_R status query",  CLI_GUEST},             
    {"disp_r enable","dispren", _PmxTXIFDISP_REnable,       NULL,           "DISP_R encode enable",  CLI_GUEST},                 
    {"Glasses control","glactr", _PmxTXIFGlasses_Control,       NULL,           "glasses control",  CLI_GUEST},                     
    {"Glasses RST Mode","glarst", _PmxTXIFGlasses_RSTControl,       NULL,           "glasses reset mode control",  CLI_GUEST},                         
    {"Glasses duty","gladuty", _PmxTXIFGlasses_DutyControl,       NULL,           "glasses duty control",  CLI_GUEST},                             
    {"Glasses enable","glaen", _PmxTXIFGlasses_Enable,       NULL,           "glasses enable control",  CLI_GUEST},                                 
    {"IS control","isc", _PmxTXIFIS_Control,       NULL,           "Inverter sync control",  CLI_GUEST},    
    {"IS delay","isd", _PmxTXIFISDelay_Control,       NULL,           "Inverter sync delay control",  CLI_GUEST},        
    {"Scan_PWN Initial","pwmi", _PmxTXIFPWM_Init, 	  NULL, 		  "Scan_PWN Initail",  CLI_GUEST},		 
    {"Scan_PWN Enable","pwmen", _PmxTXIFPWM_Enable,       NULL,           "Scan_PWN Enable",  CLI_GUEST},        
    {"Scan_PWN Set", "pwmset",_PmxTXIFPWM_Set,       NULL,           "Scan_PWN Set",  CLI_GUEST},            
    {"Scan_PWN Auto Test", "pwmat",_PmxTXIFPWM_AutoTest,       NULL,           "Scan_PWN Auto Test",  CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxINTRCmdTbl[] =
{
    {"INTR status","q", _PmxINTRStatus,       NULL,   "interrupt status query",  CLI_GUEST},      
    {"INTR setting","s", _PmxINTRSetting,       NULL,   "interrupt setting",  CLI_GUEST},      		
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxVB1CmdTbl[] =
{
#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)    
    {"VByOne Script","script", _VByOneScript,       NULL,           "V By One Script",  CLI_SUPERVISOR},
    {"VByOne_CRC_chk","crc", _VByOneCRCChk,       NULL,           "V By One CRC Check",  CLI_SUPERVISOR},       
    {"VByOne_imp_cal","cal", _VByOneImpCal,       NULL,           "V By One Impendance Calibration",  CLI_SUPERVISOR},   
    {"vbone_panle", "p", _PmxSetCmdVb1, NULL, "Set Panel vb1 flag", CLI_SUPERVISOR},
    {"vbone_TestMode", "tm", _VByOneTestMode, NULL, "Set Test Mode", CLI_SUPERVISOR},    
    {"vbone_status", "s", _VByOneStatus, NULL, "Get VB1 status", CLI_SUPERVISOR},        
    {"lock_chk", "chk", _VByOneLockChk, NULL, "Get VB1 status", CLI_SUPERVISOR},        
    {"lock_cnt", "cnt", _VByOneLockCnt, NULL, "Get VB1 status", CLI_SUPERVISOR},            
    {"VByOne_On", "on", _VByOneOn, NULL, "Set VB1 on", CLI_SUPERVISOR},      
    {"VByOne_Off", "off", _VByOneOff, NULL, "Set VB1 on", CLI_SUPERVISOR},          
    {"INX VB1 I2C cmd","inx", _VByOneINXI2CCmd,       NULL,   "INX VB1 I2C cmd",  CLI_GUEST},
#endif    
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxUHDCmdTbl[] =
{
    {"uhd status","s", _PmxUHDStatus,       NULL,   "uhd status query",  CLI_GUEST},      
    {"und setting","p", _PmxUHDSetting,       NULL,   "uhd panel setting",  CLI_GUEST},      		
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPmxCmdTbl[] =
{
    {"init", "i", _PmxInitCmd, NULL, "init", CLI_SUPERVISOR},
    {"enable", "e", _PmxEnableCmd, NULL, "enable/disable LVDS", CLI_GUEST},
    {"pattern", "p", _PmxPatternCmd, NULL, "enable/disable test pattern", CLI_GUEST},
    {"pat chg", "pc", _PmxOutPatternChgCmd, NULL, "enable/disable test multi pattern", CLI_GUEST},       
    {"SetCustFRC","scf",   _PmxSetCustFRC, NULL, "Set Out Frame Rate",     CLI_GUEST},
    {"OSTG in pattern", "ostgpt", _PmxSetCmdOSTGPt, NULL, "ostg paten", CLI_GUEST },
    {"list", "l", _PmxListPanelCmd, NULL, "show panel list", CLI_GUEST},
    {"query", "q", _PmxQueryCmd, NULL, "dump pmx(scpos) info", CLI_GUEST},
    {"set", "s", NULL, _arPmxSetCmdTbl, "set parameter", CLI_GUEST},
    {"post scaler", "ps", NULL, _arPmxPSCmdTbl, "set parameter", CLI_GUEST},    
    {"diag", "d", NULL, _arPmxDiagCmdTbl, "verify hardware", CLI_SUPERVISOR},
    {"od", "od",NULL, _arPmxODCmdTbl, "od parameters", CLI_SUPERVISOR},
    {"vopll", "vopll",NULL, _arPmxVOPLLCmdTbl, "vopll parameters", CLI_SUPERVISOR},    
    {"dvopll", "dvopll",NULL, _arPmxDVOPLLCmdTbl, "dvopll parameters", CLI_SUPERVISOR},        
    {"if", "if",NULL, _arPmxIFCmdTbl, "lvds interface parameters", CLI_GUEST},            
	{"intr", "intr",NULL, _arPmxINTRCmdTbl, "interrupt", CLI_GUEST},			  
	{"vb1", "vb1",NULL, _arPmxVB1CmdTbl, "v by one panel", CLI_GUEST},		
	{"uhd", "u",NULL, _arPmxUHDCmdTbl, "uhd", CLI_GUEST},			  	
	#ifdef CC_MT5882
	{"pll", "pll",NULL, _arPmxPLLCmdTbl, "pll check", CLI_GUEST},	
	#endif

#ifndef CC_5391_LOADER
    CLIMOD_DEBUG_CLIENTRY(PMX),
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Pmx)
{
    "pmx",
    NULL,
    NULL,
    _arPmxCmdTbl,
    "pmx command",
    CLI_GUEST
};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxInitCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    PMX_Init();

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxEnableCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "fgEnable");

    vApiPanelPowerSequence(PARAM(0));
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxQueryCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    PMX_DIAG_DumpScpos();
    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
#ifdef LVDS_ESD_PROTECT
#ifdef CC_MT5882
extern BOOL _fgESDProtect;
static INT32 _PmxSetCmdESDProtectFg(INT32 i4Argc, const CHAR **szArgv)
{	
	_fgESDProtect =  PARAM(0);

	return 0;
}
#endif
#endif

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdBg(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "u4BgColor(RGB)");
    PMX_SetBg(PARAM(0), FALSE);

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdPlaneOrder(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2, "ucVdpId(0:main,2:osd1,3:osd2)\n"
                      "ucPlaneOrder(0:top~2:bottom)");

    if (PMX_SetPlaneOrder(PARAM(0), PARAM(1)) == PMX_SET_OK)
    {
        return 0;
    }
    return -1;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdPlaneOrderArray(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 au4PlaneOrder[4];

    CHK_NUM_MIN_PARAM(4, "order1(top), order2(bot)\n"
                      "(0:main,3:osd2)");

    au4PlaneOrder[0] = PARAM(0);
    au4PlaneOrder[1] = PARAM(1);
    au4PlaneOrder[2] = PARAM(2);
    au4PlaneOrder[3] = PARAM(3);
    if (PMX_SetPlaneOrderArray(au4PlaneOrder) == PMX_SET_OK)
    {
        return 0;
    }
    return -1;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdMixAlpha(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR   ucType    =0x0;
    UINT32  pu4Alpha[3]={0x0,0x0,0x0};

    CHK_NUM_MIN_PARAM(4, "para(1) osd&video layout(value:0~3)\n"
                      "alpha value array[3]");

    ucType      = PARAM(0);
    
    pu4Alpha[0] = PARAM(1);
    pu4Alpha[1] = PARAM(2);
    pu4Alpha[2] = PARAM(3);

    if (PMX_SetMixAlpha(ucType,pu4Alpha) == PMX_SET_OK)
    {
        return 0;
    }
    
    return -1;
}

static INT32 _PmxSetCustFRC(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4CustFRCMask = 0;

    if (i4Argc != 2)
    {
		Printf("Usage: %s <CustFRCMask>\n", szArgv[0]);
		Printf("Set customized FRC\n");
		Printf("0x0: Normal\n");
		Printf("0x1: SUPPORT_50TO60_2D\n");
		Printf("0x2: SUPPORT_50TO60_3D\n");
		Printf("0x4: SUPPORT_60TO50_2D\n");
		Printf("0x8: SUPPORT_60TO50_3D\n");

        return 1;
    }

    u4CustFRCMask = StrToInt(szArgv[1]);
    u4DrvSetCustFRC(u4CustFRCMask);
    
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdBacklt(INT32 i4Argc, const CHAR ** szArgv)
{
#ifndef SUPPORT_PANEL_SCAN_PWM
    UINT32 u4Bright = PARAM(0);
    UINT32 u4Freq = (i4Argc == 3)?PARAM(1):BACKLT_GetDimmingFrequency();

    CHK_NUM_MIN_PARAM(1, "backlight(0~255) [freq(Hz)]");

    if (IsPwmLowPanelBright() == PWM_LOW_PANEL_BRIGHT)
    {
        u4Bright = 255 - u4Bright;
    }
    BACKLT_SetDimming(u4Freq, u4Bright);
#else
    UINT32 u4Devider=1;
    UINT32 u4High=0;
	UINT32 u4Low=0;
	
    UINT8  u1Src = PARAM(0);
    UINT32 u4Start = PARAM(1);
    UINT32 u4Bright = PARAM(2);
    UINT32 u4Freq = (i4Argc == 5)?PARAM(3):BACKLT_GetDimmingFrequency();

    CHK_NUM_MIN_PARAM(4, "scanPWMSrc(1~6) start(permillage) duty(permillage) freq(Hz)");

    if (IsPwmLowPanelBright() == PWM_LOW_PANEL_BRIGHT)
    {
    #ifndef SUPPORT_PANEL_SCAN_PWM
        u4Bright = 255 - u4Bright;
    #else
	    u4Bright = 1000 - u4Bright;
    #endif
    }

    if(u4Freq == 0)
	{
		u4Freq = BACKLT_GetDimmingFrequency();
	}
	
	if((vDrvGetLCDFreq() == 25) || (vDrvGetLCDFreq() == 50))
    {
        u4Devider = u4Freq/50;
    }
    else if(SUPPORT_PANEL_48HZ && ((vDrvGetLCDFreq() == 24) || (vDrvGetLCDFreq() == 48)))
    {
        u4Devider = u4Freq/48;
    }
    else if ((vDrvGetLCDFreq() == 30) || (vDrvGetLCDFreq() == 60))
    {
        u4Devider = u4Freq/60;
    }
    else if (vDrvGetLCDFreq() == 100)
    {
	    u4Devider = u4Freq/100;
    }
    else if (vDrvGetLCDFreq() == 120)
    {
	    u4Devider = u4Freq/120;
    }

    if ((u4Bright>0)&&(u4Bright<1000))
    {
		u4High = u4Bright/u4Devider;
		u4Low = (1000-u4Bright)/u4Devider;
    }
    else if (u4Bright>=1000)
    {
		u4High = 1000;
    }
    vDrvInitSCANPWM(u1Src);
    vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Aligned Vsync
    vDrvSetScanPWM(u1Src,u4Start,u4High,u4Low);
    vDrvScanPWMDataFire();
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdSpreadSpectrum(INT32 i4Argc, const CHAR ** szArgv)
{

#if 1
    UINT32 u4Permillage = DDDS_GetSpreadSpectrum();
    UINT32 u4Freq = DDDS_GetSpreadSpectrumFreq();
    if (i4Argc == 1)
    {
        //Printf("current value=%d\n", u4Permillage);
        if (u4Permillage == 0)
        {   
               Printf("current range = 0\n");
               Printf("current modulation rate = 0\n");
        }
        else
        {
               Printf("current range = %d\n", u4Permillage);
               Printf("current modulation rate = %d\n", u4Freq);
        }
    }

    //CHK_NUM_MIN_PARAM(1, "value(0~1000) | {period(us), range(0.1%)}");
    CHK_NUM_MIN_PARAM(1, "{value(20K~100K), value(0~60)} | {rate(Hz), range(0.1%)}");

    if (i4Argc == 2)
    {
        vDrvSetSSWithCLICmd(1);
        vDrvSetVOSpread(PARAM(0));
    }

    if (i4Argc == 3)
    {
        vDrvSetSSWithCLICmd(1);
        DDDS_SetSpreadSpectrum(PARAM(0), PARAM(1));
    }
#else
Printf("CLI not ready\n");
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdPanel(INT32 i4Argc, const CHAR ** szArgv)
{
    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    CHK_NUM_MIN_PARAM(1, "u4PanelIndex | \n"
                      "{u4DispClk u4PanelWidth u4PanelHeight\n"
                      "u4HTotal u4VTotal\n"
                      "u4HSyncWidth u4VSyncWidth u4HPos u4VPos target}\n");

    if (i4Argc == 2 || i4Argc == 3)
    {
#ifndef CC_PANEL_FROM_FLASH
        if (PARAM(0) > 0xff)
        {
            LoadPanelIndex();
            DumpPanelAttribute(PANEL_DUMP_CURRENT);
            return 0;
        }
        SelectPanel(PARAM(0));
        if (i4Argc == 3)
        {
            return 0;
        }
        WritePanelIndexToEeprom(PARAM(0));        
#else
    
    WritePanelIndexToEeprom(PARAM(0));

    LoadPanelIndex();
    if (PARAM(0) > 0xff)
    {
        DumpPanelAttribute(PANEL_DUMP_CURRENT);
        return 0;
    }
        SelectPanel(0);
        if (i4Argc == 3)
        {
            return 0;
        }
#endif        

    }
    else
    {
        UINT32 u4DispClk = PARAM(0);
        UINT32 u4DispWidth = PARAM(1);
        UINT32 u4DispHeight = PARAM(2);
        UINT32 u4HTotal = PARAM(3);
        UINT32 u4VTotal = PARAM(4);
        UINT32 u4HSyncWidth = PARAM(5);
        UINT32 u4VSyncWidth = PARAM(6);
        UINT32 u4HPos = PARAM(7);
        UINT32 u4VPos = PARAM(8);
        UINT32 u4Target = PARAM(9);

        if ((u4Target != 50) && (u4Target != 60))
        {
            u4Target = 0;
        }

        if (u4DispClk > 0)
        {
            if ((u4Target == 0) || (u4Target == 60))
            {
                PANEL_SetPixelClk60Hz(u4DispClk);
            }
            if ((u4Target == 0) || (u4Target == 50))
            {
                PANEL_SetPixelClk50Hz(u4DispClk);
            }
        }

        if (u4DispWidth > 0)
        {
            PANEL_SetPanelWidth(u4DispWidth);
        }

        if (u4DispHeight > 0)
        {
            PANEL_SetPanelHeight(u4DispHeight);
        }

        if (u4HTotal > 0)
        {
            if ((u4Target == 0) || (u4Target == 60))
            {
                PANEL_SetHTotal60Hz(u4HTotal);
            }
            if ((u4Target == 0) || (u4Target == 50))
            {
                PANEL_SetHTotal50Hz(u4HTotal);
            }
        }
        if (u4VTotal > 0)
        {
            if ((u4Target == 0) || (u4Target == 60))
            {
                PANEL_SetVTotal60Hz(u4VTotal);
            }
            if ((u4Target == 0) || (u4Target == 50))
            {
                PANEL_SetVTotal50Hz(u4VTotal);
            }
        }

        if (u4HSyncWidth > 0)
        {
            PANEL_SetHSyncWidth(u4HSyncWidth);
        }

        if (u4VSyncWidth > 0)
        {
            PANEL_SetVSyncWidth(u4VSyncWidth);
        }

        if (u4HPos > 0)
        {
            PANEL_SetHPosition60Hz(u4HPos);
        }

        if (u4VPos > 0)
        {
            PANEL_SetVPosition(u4VPos);
        }
    }

    // turn off panel
    vApiPanelPowerSequence(FALSE);

    //vApiHwInit();
    //Printf("[SA7] Pmx_Cmd \n");
    //vDDDSInit();    
#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
    if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_VB1)
#endif
    {
        vDrvLVDSInit();
    }
	//vDrvDisplayInit();
    vDrvOutputStageInit();
    // vDrvLVDSConfig();
    vDrvVOFreqSet(
        (vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
            (PANEL_GetPixelClk50Hz()):
            (SUPPORT_PANEL_48HZ && (vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48))?
            (PANEL_GetPixelClk48Hz()):
            (PANEL_GetPixelClk60Hz()));
    vDrvSetPanelTiming();
    vErrorHandleSetByPanel();
    OSTG_OnOutputVSync();
    IGNORE_RET(OSD_UpdateTiming());

    vApiPanelPowerSequence(TRUE);

    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    
#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
    if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_VB1)
#endif
    {
        //LVDS 7->4 FIFO enable 
        vDrvLVDS7To4FifoEnable();
    }
    //od init after change panel
    vDrvODInit();

    return 0;
}

#ifdef CC_MT5399
// extern void vDrvMJCModeChangeDone(UINT32 u4Path);
#endif

static INT32 _PmxSetCmdPanelDriverIni(INT32 i4Argc, const CHAR ** szArgv)
{
    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    SelectPanel(PARAM(0));

    // turn off panel
    vApiPanelPowerSequence(FALSE);

    //vApiHwInit();
    //Printf("[SA7] Pmx_Cmd \n");
    vDDDSInit();    
#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
    if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_VB1)
#endif
    {
        vDrvLVDSInit();
    }
	//vDrvDisplayInit();
    vDrvOutputStageInit();
    vDrvVOFreqSet(
        (vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
            (PANEL_GetPixelClk50Hz()):
            (SUPPORT_PANEL_48HZ && (vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48))?
            (PANEL_GetPixelClk48Hz()):
            (PANEL_GetPixelClk60Hz()));
    vDrvSetPanelTiming();

	
    vErrorHandleInit();
    OSTG_OnOutputVSync();
    IGNORE_RET(OSD_UpdateTiming());

    vApiPanelPowerSequence(TRUE);

    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    
#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
    if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_VB1)
#endif
    {
        //LVDS 7->4 FIFO enable 
        vDrvLVDS7To4FifoEnable();
    }
    //od init after change panel
     //vDrvODInit();
    #ifdef CC_MT5399
	//vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
	// vSetMainFlg(MAIN_FLG_MODE_CHG);	
	// vDrvMJCModeChangeDone(0);
	#endif
	
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdPowerSequence(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(4, "LvdsOn, BacklightOn, BacklightOff, LvdsOff (unit:10ms)");
    PANEL_SetLvdsOnDalay(PARAM(0));
    PANEL_SetBacklightOnDelay(PARAM(1));
    PANEL_SetBacklightOffDelay(PARAM(2));
    PANEL_SetLvdsOffDalay(PARAM(3));
    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdDrivingCurrent(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Current =(UINT32)vDrvLVDSGetDriving();
    if (i4Argc == 1)
    {
        Printf("current value=%d\n", u4Current);
    }

    CHK_NUM_MIN_PARAM(1, "value(0~15)");
    u4Current = PARAM(0) & 0xf;
    //vWriteSCPOSMsk(0x4, u4Current, 0xf);
    vDrvLVDSSetDriving(u4Current);

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdVCM(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VCM =(UINT32)vDrvLVDSGetVCM();
    if (i4Argc == 1)
    {
        Printf("current VCM=%d\n", u4VCM);
    }

    CHK_NUM_MIN_PARAM(1, "value(0~15)");
    u4VCM = PARAM(0) & 0xf;
    vDrvLVDSSetVCM(u4VCM);

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _PmxPatternCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 cmd;
    CHK_NUM_MIN_PARAM(1, "fgEnable(0:off, 1:on)");

    cmd = PARAM(0);

    vDrvSetOSTGOutPattern(cmd);
    return 0;
}

INT32 _PmxOutPatternChgCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 cmd, u4Type, u4Spd;
    CHK_NUM_MIN_PARAM(3, "fgEnable(0:off, 1:on), Type (0:RGB 1:VLine 2:Ramp), Speed (0~7)");

    cmd = PARAM(0);
	u4Type = PARAM(1);
	u4Spd = PARAM(2);

    vDrvSetOSTGOutPatternChg(cmd,u4Type,u4Spd);
    return 0;
}

INT32 _PmxSetCmdOSTGPt(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 id, cmd;
    CHK_NUM_MIN_PARAM(2, "VdpId(0:main, 1:pip) fgEnable(0:off, 1:on)");

    id = PARAM(0);
    cmd = PARAM(1);

    vDrvSetOSTGInPattern(id, cmd);
 
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxDiagCmdDumpScpos(INT32 i4Argc, const CHAR ** szArgv)
{
    PMX_DIAG_DumpScpos();
    if (i4Argc == 1)
    {
        DumpPanelAttribute(PANEL_DUMP_CURRENT);
    }
    if (i4Argc == 2)
    {
        DumpPanelAttribute(PARAM(0));
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxPSCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vGetPostScalerStatus();

    return 0;
}

//extern LCDTIMING_INFO_T lcdtimingInfo;
//extern UINT8 fgApplyScalerSetData;
static INT32 _PmxPSCmdHsyncTest(INT32 i4Argc, const CHAR ** szArgv)
{
    LINT_SAVE_AND_DISABLE 
    UINT8 u4Arg1,u4Arg2; 
    UINT8 u4Char;
    
    u4Arg1 = (UINT8) StrToInt(szArgv[1]);	   
    u4Arg2 = (UINT8) StrToInt(szArgv[2]);
	
    if(((i4Argc == 3) && (u4Arg1 == 1))||((i4Argc == 2) && (u4Arg1 == 1)))
	{ 
	    UINT32 u4HsTotal, u4HsTotal_old, u4HsActive, u4HsActive_old;	
		UINT32 u4HsTotalChg;
        UINT32 u4HsFp,u4HsFp_old,u4HsBp,u4HsBp_old;
		UINT32 u4HsFpChg,u4HsBpChg;		
        UINT32 u4HsWid,u4HsWid_old;		
        UINT8  u1ChangeStep;

        u4HsTotal = wDrvGetOutputHTotal();
		u4HsTotal_old = u4HsTotal;
		u4HsTotalChg = u4HsTotal;
        u4HsActive = wDrvGetOutputHActive();
		u4HsActive_old = u4HsActive;
		u4HsFp = wDrvGetHsyncFp();
		u4HsFp_old = u4HsFp;
		u4HsFpChg = u4HsFp;		
		u4HsBp = wDrvGetHsyncBp();
		u4HsBp_old = u4HsBp;
		u4HsBpChg = u4HsBp;		
		u4HsWid = u4HsTotal - u4HsActive - u4HsFp - u4HsBp;
        u4HsWid_old = u4HsWid;

        if (LVDS_OUTPUT_PORT == SINGLE_PORT)
        {
         u1ChangeStep = 1;
        }
		else if (LVDS_OUTPUT_PORT == DUAL_PORT)
		{
         u1ChangeStep = 2;		
		}
        else if (LVDS_OUTPUT_PORT == FOUR_PORT)			
        {
		 u1ChangeStep = 4;	  
        }
		else
		{
		 u1ChangeStep = 1;	 
		}
			
        Printf("Begin to panel Hsync test \n");
		Printf("Htotal=%d, Hfp=%d,Hwid=%d HBp=%d Hactive=%d\n",u4HsTotal_old,u4HsFp_old,u4HsWid_old,u4HsBp_old,u4HsActive_old);		
        Printf("Please enter 'u' when do up test \n");
        Printf("Please enter 'd' when do down test \n");
        while(1)
        {
            u4Char = SerPollGetChar();
			
            vDrvSwitchImportProtection(1);
			
            if((u4Char== 'u') || (u4Char== 'U'))
            {
                if(u4Char== 'u')
                {
                    u4Char = u4Char-87;
                }
                else
                {
                    u4Char = u4Char-55;
                }
               
				if (u4Arg2 ==0)
				{
                 u4HsTotalChg += u1ChangeStep;				
                 Printf("Current H_Total is /************ (%d) ***********/ \n",u4HsTotalChg);								
                 wDrvSetOutputHTotal(u4HsTotalChg);
                 //lcdtimingInfo.u4HTotal = u4HsTotalChg;
				} 
				else  if(u4Arg2 ==1) 
				{
				 u4HsFpChg += u1ChangeStep;
				 u4HsTotalChg += u1ChangeStep;	
                 Printf("Current H_Fp is /************ (%d) ***********/ \n",u4HsFpChg);												 
				 wDrvSetOutputHTotal(u4HsTotalChg);
                 vDrvSetHsyncFp(u4HsFpChg);
                 //lcdtimingInfo.u4HTotal = u4HsTotalChg;
                 //lcdtimingInfo.u4HFP = u4HsFpChg;
				} 
				else  if(u4Arg2 ==2) 
				{
				 u4HsBpChg += u1ChangeStep;
				 u4HsTotalChg += u1ChangeStep;	
                 Printf("Current H_Bp is /************ (%d) ***********/ \n",u4HsBpChg);												 				 
				 wDrvSetOutputHTotal(u4HsTotalChg);
				 vDrvSetHsyncBp(u4HsBpChg);
				 //lcdtimingInfo.u4HTotal = u4HsTotalChg;
                 //lcdtimingInfo.u4HBP= u4HsBpChg;
				} 
				else
				{
				 u4HsTotalChg += u1ChangeStep; 			
				 Printf("Current H_Total is /************ (%d) ***********/ \n",u4HsTotalChg);								
				 wDrvSetOutputHTotal(u4HsTotalChg);
				 //lcdtimingInfo.u4HTotal = u4HsTotalChg;
				} 
            }
            else if((u4Char== 'd') || (u4Char== 'D'))
            {
                if(u4Char== 'd')
                {
                    u4Char = u4Char-87;
                }
                else
                {
                    u4Char = u4Char-55;
                }
				if (u4Arg2 ==0)
				{
                 u4HsTotalChg -= u1ChangeStep;				
                 Printf("Current H_Total is /************ (%d) ***********/ \n",u4HsTotalChg);								
                 wDrvSetOutputHTotal(u4HsTotalChg);
                 //lcdtimingInfo.u4HTotal = u4HsTotalChg;
				} 
				else  if(u4Arg2 ==1) 
				{
				 u4HsTotalChg -= u1ChangeStep;
				 u4HsFpChg -= u1ChangeStep;
                 Printf("Current H_Fp is /************ (%d) ***********/ \n",u4HsFpChg);												 
				 wDrvSetOutputHTotal(u4HsTotalChg);
                 vDrvSetHsyncFp(u4HsFpChg);
                 //lcdtimingInfo.u4HTotal = u4HsTotalChg;
                 //lcdtimingInfo.u4HFP = u4HsFpChg;
				} 
				else  if(u4Arg2 ==2) 
				{
				 u4HsTotalChg -= u1ChangeStep;
				 u4HsBpChg -= u1ChangeStep;
                 Printf("Current H_Bp is /************ (%d) ***********/ \n",u4HsBpChg);												 				 
				 wDrvSetOutputHTotal(u4HsTotalChg);				 
				 vDrvSetHsyncBp(u4HsBpChg);
				 //lcdtimingInfo.u4HTotal = u4HsTotalChg;
                 //lcdtimingInfo.u4HBP= u4HsBpChg;
				} 
				else
				{
				 u4HsTotalChg -= u1ChangeStep; 			
				 Printf("Current H_Total is /************ (%d) ***********/ \n",u4HsTotalChg);								
				 wDrvSetOutputHTotal(u4HsTotalChg);
				 //lcdtimingInfo.u4HTotal = u4HsTotalChg;
				} 
            }
            else if((u4Char==13) || (u4Char==10)) // ncstool, enter
            {
               continue;
            }
            else if(u4Char==27) // escape, end of auto pattern
	        {
	          Printf("Quit Panel Hsync Test \n");
              wDrvSetOutputHTotal(u4HsTotal_old);
              vDrvSetHsyncFp(u4HsFp_old);
              vDrvSetHsyncBp(u4HsBp_old);				
			  vDrvFireImportPortection();
	     	  return 0;
	        }	 
		 vScEhSta();
	     vDrvFireImportPortection();			
	     //fgApplyScalerSetData = 1;
	     //x_thread_delay(40);
        }		 
	}
    else if((i4Argc == 3) && (u4Arg1 == 0))
    {
        Printf("Turn off Panel Hsync Test\n");	
        return 0;
    }
    else
    {
	     Printf("Arg: ucOnOff  type (Panel Hsync Test)\n");	
	     Printf("type [0] Sync Width change \n");			 
	     Printf("type [1] Sync Front porch change \n");			 
	     Printf("type [2] Sync Back porch change \n");			 		 
	     return 0;
    }     		
    
    LINT_RESTORE

}

extern void vDrvMJCSetVsyncDelay(UINT32 u4FrontPorch, UINT32 u4VTotal);
static INT32 _PmxPSCmdVsyncTest(INT32 i4Argc, const CHAR ** szArgv)
{
    LINT_SAVE_AND_DISABLE 
    UINT8 u4Arg1,u4Arg2; 
    UINT8 u4Char;
    
    u4Arg1 = (UINT8) StrToInt(szArgv[1]);	   
    u4Arg2 = (UINT8) StrToInt(szArgv[2]);
    if(((i4Argc == 3) && (u4Arg1 == 1))||((i4Argc == 2) && (u4Arg1 == 1)))
	{ 
	    UINT32 u4VsTotal, u4VsTotal_old, u4VsActive, u4VsActive_old;	
		UINT32 u4VsTotalChg;
        UINT32 u4VsFp,u4VsFp_old,u4VsBp,u4VsBp_old;
		UINT32 u4VsFpChg,u4VsBpChg;		
        UINT32 u4VsWid,u4VsWid_old;		

        u4VsTotal = wDrvGetOutputVTotal();
		u4VsTotal_old = u4VsTotal;
		u4VsTotalChg = u4VsTotal;
        u4VsActive = wDrvGetOutputVActive();
		u4VsActive_old = u4VsActive;
		u4VsFp = wDrvGetVsyncFp();
		u4VsFp_old = u4VsFp;
		u4VsFpChg = u4VsFp;		
		u4VsBp = wDrvGetVsyncBp();
		u4VsBp_old = u4VsBp;
		u4VsBpChg = u4VsBp;		
		u4VsWid = u4VsTotal - u4VsActive - u4VsFp - u4VsBp;
        u4VsWid_old = u4VsWid;
			
        Printf("Begin to panel Vsync test \n");
		Printf("Vtotal=%d, Vfp=%d,Vwid=%d VBp=%d Vactive=%d\n",u4VsTotal_old,u4VsFp_old,u4VsWid_old,u4VsBp_old,u4VsActive_old);		
        Printf("Please enter 'u' when do up test \n");
        Printf("Please enter 'd' when do down test \n");
        while(1)
        {
            u4Char = SerPollGetChar();
			
            vDrvSwitchImportProtection(1);
			
            if((u4Char== 'u') || (u4Char== 'U'))
            {
                if(u4Char== 'u')
                {
                    u4Char = u4Char-87;
                }
                else
                {
                    u4Char = u4Char-55;
                }
               
				if (u4Arg2 ==0)
				{
                 u4VsTotalChg += 1;				
                 Printf("Current V_Total is /************ (%d) ***********/ \n",u4VsTotalChg);								
                 wDrvSetOutputVTotal(u4VsTotalChg);
                 vDrvMJCSetVsyncDelay(u4VsFp, u4VsTotalChg);
				} 
				else  if(u4Arg2 ==1) 
				{
				 u4VsFpChg += 1;
				 u4VsTotalChg += 1;	
                 Printf("Current V_Fp is /************ (%d) ***********/ \n",u4VsFpChg);												 
				 wDrvSetOutputVTotal(u4VsTotalChg);
                 vDrvSetVsyncFp(u4VsFpChg);
                 vDrvMJCSetVsyncDelay(u4VsFpChg, u4VsTotalChg);
				} 
				else  if(u4Arg2 ==2) 
				{
				 u4VsBpChg += 1;
				 u4VsTotalChg += 1;	
                 Printf("Current V_Bp is /************ (%d) ***********/ \n",u4VsBpChg);												 				 
				 wDrvSetOutputVTotal(u4VsTotalChg);
				 vDrvSetVsyncBp(u4VsBpChg);
				 vDrvMJCSetVsyncDelay(u4VsFp, u4VsTotalChg);
				} 
				else
				{
				 u4VsTotalChg += 1; 			
				 Printf("Current V_Total is /************ (%d) ***********/ \n",u4VsTotalChg);								
				 wDrvSetOutputVTotal(u4VsTotalChg);
				 vDrvMJCSetVsyncDelay(u4VsFp, u4VsTotalChg);
				} 
            }
            else if((u4Char== 'd') || (u4Char== 'D'))
            {
                if(u4Char== 'd')
                {
                    u4Char = u4Char-87;
                }
                else
                {
                    u4Char = u4Char-55;
                }
				if (u4Arg2 ==0)
				{
                 u4VsTotalChg -= 1;				
                 Printf("Current V_Total is /************ (%d) ***********/ \n",u4VsTotalChg);								
				 wDrvSetOutputVTotal(u4VsTotalChg);
				 vDrvMJCSetVsyncDelay(u4VsFp, u4VsTotalChg);
				} 
				else  if(u4Arg2 ==1) 
				{
				 u4VsTotalChg -= 1;
				 u4VsFpChg -= 1;
                 Printf("Current V_Fp is /************ (%d) ***********/ \n",u4VsFpChg);												 
				 wDrvSetOutputVTotal(u4VsTotalChg);
                 vDrvSetVsyncFp(u4VsFpChg);
                 vDrvMJCSetVsyncDelay(u4VsFpChg, u4VsTotalChg);
				} 
				else  if(u4Arg2 ==2) 
				{
				 u4VsTotalChg -= 1;
				 u4VsBpChg -= 1;
                 Printf("Current V_Bp is /************ (%d) ***********/ \n",u4VsBpChg);												 				 
				 wDrvSetOutputVTotal(u4VsTotalChg);				 
				 vDrvSetVsyncBp(u4VsBpChg);
				 vDrvMJCSetVsyncDelay(u4VsFp, u4VsTotalChg);
				} 
				else
				{
				 u4VsTotalChg -= 1; 			
				 Printf("Current V_Total is /************ (%d) ***********/ \n",u4VsTotalChg);								
				 wDrvSetOutputVTotal(u4VsTotalChg);
				 vDrvMJCSetVsyncDelay(u4VsFp, u4VsTotalChg);
				} 
            }
            else if((u4Char==13) || (u4Char==10)) // ncstool, enter
            {
               continue;
            }
            else if(u4Char==27) // escape, end of auto pattern
	        {
	          Printf("Quit Panel Vsync Test \n");
              wDrvSetOutputVTotal(u4VsTotal_old);
              vDrvSetVsyncFp(u4VsFp_old);
              vDrvSetVsyncBp(u4VsBp_old);				
			  vDrvFireImportPortection();
	     	  return 0;
	        }	 
		vScEhSta();
	     vDrvFireImportPortection();			
         //x_thread_delay(40);
        }		 
	}
    else if((i4Argc == 2) && (u4Arg1 == 0))
    {
        Printf("Turn off Panel Vsync Test\n");	
        return 0;
    }
    else
    {
	     Printf("Arg: ucOnOff  (Panel Vsync Test)\n");	
	     Printf("type [0] Sync Width change \n");			 
	     Printf("type [1] Sync Front porch change \n");			 
	     Printf("type [2] Sync Back porch change \n");			 		 
	     return 0;
    }     		
    
    LINT_RESTORE

}

static INT32 _PmxPSCmdSetDataTrigger(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	vDrvFireImportPortection();
	return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxODInitCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvODInit();

    return 0;
}

extern UINT8 OD_Table_test[FB_TBL_SIZE];
static INT32 _PmxODDMACmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvODSetDMA(0, OD_Table_test,sizeof(OD_Table_test));

    return 0;
}

static INT32 _PmxODSetLocalODTblCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Region;
    UINT8 u1TableIdx;
    UINT32 i;
    
    CHK_NUM_MIN_PARAM(2, "Region (0~19 ; 20:set all)  TableIdx(0 ~max)");
    u1Region = (UINT8)PARAM(0);
    u1TableIdx =(UINT8)PARAM(1);

    if (u1Region < 20)
    {
        vDrvSetLocalODTbl(u1Region , u1TableIdx);
    }
    else if (u1Region == 20)
    {
        for ( i=0 ; i<20 ;i++)
        {
            vDrvSetLocalODTbl(i,u1TableIdx);
        }    
    }
    else
    {
        Printf("region range is 0~20 \n");
    }
    
    return 0;
}


static INT32 _PmxODOnOffCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "1: On / 0:Off)");

    vDrvODMode(PARAM(0));

    return 0;
}

static INT32 _PmxPCIDOnOffCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "1: On / 0:Off)");

    vDrvPCIDOnOff(PARAM(0));

    return 0;
}

static INT32 _PmxODIdxCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "1: On / 0:Off)");

    vApiFlashPqSetOdId(PARAM(0));
    DRVCUST_SetODTable();

    return 0;
}


static INT32 _PmxODBypassCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "1: bypass / 0:not bypass)");

    vDrvODBypass(PARAM(0));

    return 0;
}

static INT32 _PmxODQueryCmd(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvODQueryStatus();
    return 0;
}


static INT32 _PmxODOverflowResetCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    vDrvODDramOverflowReset();

    return 0;
}
static INT32 _PmxODOverflowGetCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ODDramOverflowStatus = u4DrvODDramOverflow();
    UNUSED(i4Argc);
    UNUSED(szArgv);


    if (u4ODDramOverflowStatus == 1)
    {
        Printf("OD dram read overflow\n");
    }
    else if (u4ODDramOverflowStatus == 2)
    {
        Printf("OD dram write overflow\n");
    }
    else
    {
        Printf("OD dram No overflow\n");
    }
 
    return 0;
}


static INT32 _PmxODStressTestCmd(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(5, "st mode(0:normal 1:dma) st number  OD_table  Log_cnt  Delay_time ");

    vDrvODStressTest(PARAM(0),PARAM(1),PARAM(2),PARAM(3),PARAM(4));

    return 0;
}
static INT32 _PmxODCheckCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvODCheckEnable();

    return 0;
}

static INT32 _PmxODResetCmd(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvODReset();
    return 0;
}

static INT32 _PmxODTableSelCmd(INT32 i4Argc, const CHAR **szArgv)
{
    //DRVCUST_ODTableSel(PARAM(0));
    return 0;
}

static INT32 _PmxVOPLLStatusCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvVOPLLStatus();

    return 0;
}

static INT32 _PmxVOPLLSetVtrl(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	if (i4Argc != 2)
	{
		Printf("VOPLL VCO: 0 / PPLL VCO: 1\n",szArgv[0]);		   
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

    vDrvSetVopllVcoMonitor(u1arg1);
    
    return 0;
}

static INT32 _PmxVOPLLSourceCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	if (i4Argc != 2)
	{
		Printf("On: 1 / Off: 0\n",szArgv[0]);		   
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

    u1VOPLLSource(u1arg1);

    return 0;
}

static INT32 _PmxVOPLLTestModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	if (i4Argc != 2)
	{
		Printf("Mode: 1~4  Off: 0\n",szArgv[0]);		   
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

    vDrvVOPLLDDDS_SS_TestMode(u1arg1);

    return 0;
}


static INT32 _PmxVOPLLSetFreqCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4arg1;
	if (i4Argc != 2)
	{
		Printf("[frequency]",szArgv[0]);		   
		return 0;
	}
	u4arg1 = (UINT32)StrToInt(szArgv[1]);

    vDrvVOPLLFreqSet(u4arg1);

    return 0;
}

static INT32 _PmxVOPLLCalErrorLimitCmd(INT32 i4Argc, const CHAR **szArgv)
{

    UINT32 u4arg1;
	UINT16 u2ErrorLimit;
    CHK_NUM_MIN_PARAM(1, "Permillage = upeer + lower Range");

    if (i4Argc == 2)
    {
      u4arg1 = (UINT8)StrToInt(szArgv[1]);
      u2ErrorLimit = u2DDDS_CalErrorLimit(u4arg1);
	  Printf("Error Limit = 0x%3x \n",u2ErrorLimit);
    }

    return 0;
}

static INT32 _PmxDVOPLLLockCheck(INT32 i4Argc, const CHAR **szArgv)
{

	UINT8 u4Char;

	Printf("Begin to check dvopll lock ststus \n");
	Printf("Please enter 'c' or 'C' to do check loop\n");
	Printf("Please enter ESC key to quit \n");

	while(1)
	{
		u4Char = SerPollGetChar();
		
		if((u4Char=='c') || (u4Char=='C')) // ncstool, enter
		{
		  vDrvDDDSLockStatus(100000);
		}			
		else if((u4Char==13) || (u4Char==10)) // ncstool, enter
		{
		   continue;
		}
		else if(u4Char==27) // escape, end of auto pattern
		{
		  vDrvDDDSLockStatus(1);
		  return 0;
		}				
	}	
}

#ifdef CC_MT5882
static INT32 _PmxPLLStatusCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvChkPLLFreq();
    return 0;
}

static INT32 _PmxPLLTestModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	if (i4Argc != 2)
	{
		Printf("Mode: 1~7  Off: 0\n",szArgv[0]);
		Printf("[1]\n");
		Printf("[2]\n");
		Printf("[3]G3DPLL VCO Control voltage\n");
		Printf("[4]syspll VCO Control voltage\n");
		Printf("[5]\n");
		Printf("[6]ARMCPLL VCO Control voltage\n");
		Printf("[7]HPCPLL VCO Control voltage\n");
		Printf("[8]TAPLL VCO Control voltage\n");
		Printf("[9]SAWLESSPLL VCO Control voltage\n");		
		return 0;
	}
	
	u1arg1 = (UINT8)StrToInt(szArgv[1]);
    vDrvPLLVCO_TestMode(u1arg1);
    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
#if 0
static INT32 _PmxSetCmdXtal(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(1, "fgEnable(0:off, 1:on)");

    if (PARAM(0) == 0)
    {
        CKGEN_SetVOPLLInputAsDDDS(TRUE);
        // power down sdds
//        IO_WRITE32MSK(VIDEO_IN0_BASE, 0x490, 0, 1 << 30);
    }
    else
    {
        CKGEN_SetVOPLLInputAsDDDS(FALSE);
        // power on sdds
//        IO_WRITE32MSK(VIDEO_IN0_BASE, 0x490, 1 << 30, 1 << 30);
    }
    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdNs(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord = (u4ControlWord & 0xFFFFFFFC) | LVDS_NS;

    PANEL_SetControlWord(u4ControlWord);


#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
	if(PANEL_IsSupportVb1()==TRUE)
	{
		vVByOneFormatConfig();
	}
	else
#endif	
	{
	    vDrvLVDSConfig();
	}
    
    return 0;
}

static INT32 _PmxSetCmdSpecialNs(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord = (u4ControlWord & 0xFFFFFFFC) | LVDS_SPECIAL_NS;

    PANEL_SetControlWord(u4ControlWord);
    vDrvLVDSConfig();
    
    return 0;
}
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxSetCmdJeida(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();
        u4ControlWord = (u4ControlWord & 0xFFFFFFFC) | LVDS_JEIDA;

    PANEL_SetControlWord(u4ControlWord);
#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
	if(PANEL_IsSupportVb1()==TRUE)
	{
		vVByOneFormatConfig();
	}
	else
#endif	
	{
	    vDrvLVDSConfig();
	} 
    
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _PmxListPanelCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    DumpPanelAttribute(3);
    return 0;
}


static INT32 _PmxSetCmdDnie(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 au1Buf[2] = {0x00, 0x20};

    LOG(1, "Bypassing DNIE SDP64 ...\n");

    GPIO_SetOut(GPIO(8), 0);
    x_thread_delay(100);
    GPIO_SetOut(GPIO(8), 1);

    if (SIF_WriteMultipleSubAddr(0x100, 0xC8, 2, 0x01FA, au1Buf, 2) == 0)
    {
        LOG(1, "Write DNIE SDP64 Fail!\n");
    }
    else
    {
        LOG(1, "Write DNIE SDP64 Success (%d, %d)!\n", au1Buf[0], au1Buf[1]);
    }

    return 0;
}


static INT32 _PmxSetCmd10bit(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ControlWord;

    CHK_NUM_MIN_PARAM(1, "2:12bit, 1:10bit, 0:8bit)");

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord = u4ControlWord & (~DISP_BIT_MSK);

    if (PARAM(0) == 0)
    {
        u4ControlWord |= DISP_24BIT;
    }
    else if (PARAM(0) == 1)
    {
        u4ControlWord |= DISP_30BIT;
    }
    else
    {
        u4ControlWord |= DISP_36BIT;
    }	

    PANEL_SetControlWord(u4ControlWord);
	#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
	if(PANEL_IsSupportVb1()==TRUE)
	{
		vDrvVByOneOff();	
		vDrvVByOneOn();		
	    vDrvVOFreqSet(
        (vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
            (PANEL_GetPixelClk50Hz()):
            (SUPPORT_PANEL_48HZ && (vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48))?
            (PANEL_GetPixelClk48Hz()):
            (PANEL_GetPixelClk60Hz()));		
	}
	else
	#endif	
	{
		vDrvLVDSOff();
		vDrvLVDSOn();
	}

    return 0;
}

static INT32 _PmxSetCmdSwap(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ControlWord;

    CHK_NUM_MIN_PARAM(1, "1: LVDS_ODD_SW_ON, 0: LVDS_ODD_SW_OFF)");

    u4ControlWord = PANEL_GetControlWord();
    u4ControlWord = u4ControlWord & (~LVDS_ODD_SW_ON);

    if (PARAM(0) == 0)
    {
        u4ControlWord |= LVDS_ODD_SW_OFF;
    }
    else
    {
        u4ControlWord |= LVDS_ODD_SW_ON;
    }

    PANEL_SetControlWord(u4ControlWord);
    vDrvLVDSConfig(); 

    return 0;
}

#ifdef SUPPORT_CW_CHANGE_BY_STEP
extern UINT32 _u4CWStep;
static INT32 _NptvSetCtrlWordStep(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc==2)
	{
		_u4CWStep = StrToInt(szArgv[1]);
	}
	else
	{
		Printf("Arg: _u4CWStep, 0xfffffff:disable, Now=0x%x\n", _u4CWStep);
	}

	return 0;	
}
#endif
static INT32 _NptvOutputStageDumpEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UCHAR ucOnOff;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: 1(On)/0(Off)\n");
		return 0;
	}

	ucOnOff = (UCHAR) StrToInt(szArgv[1]);
	ucOnOff = ((ucOnOff>0)?SV_TRUE : SV_FALSE);
	vScpipSetDumpCtrl(ucOnOff);
    
    return 0;    
}

extern UINT8 _bForceFrameRate;
static INT32 _NptvForceFrameRate(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
	{
		_bForceFrameRate = StrToInt(szArgv[1]);
		vSetMainFlg(MAIN_FLG_MODE_CHG);
	}
	else
	{
		Printf("Arg: ubFrameRage, 0:disable\n");
	}

	return 0;	

}

static INT32 _NptvForceFreeRun(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
	{
		vDrvSetDisplayFreeRun(StrToInt(szArgv[1]));
		vDrvSetDDDSCloseloop(1);
	}
	else
	{
		Printf("Arg: 1: on, 0:disable\n");
	}

	return 0;	
}


static INT32 _PmxSetCmdLvdsMod(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef DRV_SUPPORT_EXTMJC
		UINT8   u1ColorDomain;
    UINT8   u1LvdsMode;
    
    CHK_NUM_MIN_PARAM(1, "\n0: Normal\n1: Yuv422 one port(LVDS 10-bit)\n2: Rgb444 one port(LVDS 10-bit)\n3: Yuv422 two port(LVDS 10-bit)\n4: Rgb444 two port(LVDS 10-bit)\n5: Yuv422 one port(LVDS 8-bit)\n6: Rgb444 one port(LVDS 8-bit)\n7: Yuv422 two port(LVDS 8-bit)");

    u1LvdsMode = (UINT8)PARAM(0);

    if (u1LvdsMode > 7)
    {
        LOG(0, "invalid parameters\n");
        return 0;
    }

    if ((u1LvdsMode == eLvdsVidYuv422OnePort8Bit) || (u1LvdsMode == eLvdsVidYuv422TwoPort8Bit) || (u1LvdsMode == eLvdsVidYuv422TwoPort) || (u1LvdsMode == eLvdsVidYuv422OnePort))
    {
        u1ColorDomain = eColorDomainYUV;
    }
    else
    {
        u1ColorDomain = eColorDomainRGB;
    }
    
    // set current LVDS mode
    vDrvSetCurLvdsMode(u1LvdsMode);    

    vDrvSetUniColorDomain(u1ColorDomain, u1LvdsMode);
    vDrvSetLvdsColorDomain(u1ColorDomain);
#else
    LOG(0, "Not Support\n");
#endif
    return 0;
}

static INT32 _PmxSetCmdQueryLvdsMod(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef DRV_SUPPORT_EXTMJC
    // get current LVDS mode
    LOG(1, "Current LVDS mode = %u\n", vDrvGetCurLvdsMode());
#else
    LOG(0, "Not Support\n");
#endif
    
    return 0;
}

static INT32 _PmxSetBorder(INT32 i4Argc, const CHAR **szArgv)
{
    CHK_NUM_MIN_PARAM(2, "VdpId(0:main, 1:pip) fgEnable(0:off, 1:on)");

    vOSTGSetBorderOnOff(PARAM(0), PARAM(1));
    
    return 0;
}

static INT32 _PmxSetBorderColor(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 id = PARAM(0);

    CHK_NUM_MIN_PARAM(2, "VdpId(0:main, 1:pip) u4RGB(0xRRGGBB)");
    
    vOSTGSetBorderOnOff(id, 0);
    vOSTGSetBorderColor(id, PARAM(1));
    vOSTGSetBorderOnOff(id, 1);

    return 0;
}

static INT32 _PmxSetBorderParam(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 id = PARAM(0);

    CHK_NUM_MIN_PARAM(6, "VdpId(0:main, 1:pip) u4X, u4Y, u4Width, u4Height, u4Thickness");
   
    vOSTGSetBorderOnOff(id, 0);
    vOSTGSetBorderParam(id, PARAM(1), PARAM(2), PARAM(3), PARAM(4), PARAM(5));
    vOSTGSetBorderOnOff(id, 1);

    return 0;
}

static INT32 _PmxSetPanelDisplayTypeMsk(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PanelProtTypeMsk;

    if(i4Argc < 2)
    {
        Printf("Usage: %s <display type mask> ([0]:LVDS, [2]:MLVDS"
#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
", [1]:VBI, [3]:EPI [4]:iDP"
#endif
")\ncurr allowed mask = %d\n", szArgv[0], PANEL_SetProtDisplayTypeMsk(NULL));
        return 0;
    }

    u4PanelProtTypeMsk = PARAM(0);

    PANEL_SetProtDisplayTypeMsk(&u4PanelProtTypeMsk);
    Printf("curr allowed mask = %d\n", PANEL_SetProtDisplayTypeMsk(NULL));

    return 0;
}

#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5882)
static INT32 _PmxSetCmdVb1(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4ControlWord;

    u4ControlWord = PANEL_GetControlWord();

    if (PARAM(0) == 0)
    {
        u4ControlWord &= ~DISP_TYPE_MASK;
    }
    else
    {
        u4ControlWord &= ~DISP_TYPE_MASK;
        u4ControlWord |= DISP_TYPE_VB1;
    }

    PANEL_SetControlWord(u4ControlWord);

    return 0;
}
#endif

//automation
static INT32 _PmxLVDS_ATERM_ATVO_Setting(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;
  
  if(x_strcmp(szArgv[1], "1") ==0 )
  { 
    mode = 1;
  }
  else if(x_strcmp(szArgv[1], "2") ==0 )
  {
    mode = 2;
  }
  else if(x_strcmp(szArgv[1], "3") ==0 )
  {
    mode = 3;
  }


  LVDS_ATERM_ATVO_Setting(mode);
    
     
  return 0;
}

static INT32 _PmxLVDS_ATERM_ATVO_Restore(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 ATERN=0,ATVO=0;

  if(i4Argc==3)
  { 
    ATERN = StrToInt(szArgv[1]); 
    ATVO  = StrToInt(szArgv[2]); 
  }

  LVDS_ATERM_ATVO_Restore(ATERN,ATVO);
  Printf("ATERN=%d, ATVO=%d\n",ATERN,ATVO);     
     
  return 0;
}

static INT32 _PmxLVDS_ATVO_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_ATVO_Set(mode);
  Printf("LVDS_ATVO_Set=%d\n",mode);     
     
  return 0;
}

static INT32 _PmxLVDS_APSRC_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_APSRC_Set(mode);
  Printf("LVDS_APSRC_Set=%d\n",mode);     
     
  return 0;
}

static INT32 _PmxLVDS_ANSRC_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_ANSRC_Set(mode);
  Printf("LVDS_ANSRC_Set=%d\n",mode);     
     
  return 0;
}

static INT32 _PmxLVDS_PADPD_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_PADPD_Set(mode);
  Printf("LVDS_PADPD_Set=%d\n",mode);     
     
  return 0;
}

static INT32 _PmxLVDS_RESET_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  LVDS_RESET_Set(mode);
  Printf("LVDS_RESET_Set=%d\n",mode);     
     
  return 0;
}

static INT32 _PmxMLVDS_LS_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  MLVDS_LS_Set(mode);
  Printf("MLVDS_LS_Set=%d\n",mode);     
     
  return 0;
}

static INT32 _PmxMLVDS_TestMODE_Set(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 mode=0;

  if(i4Argc==2)
  { 
    mode = StrToInt(szArgv[1]); 
   
  }

  MLVDS_TestMODE_Set(mode);
  Printf("MLVDS_TestMODE_Set=%d\n",mode);     
     
  return 0;
}

static INT32 _PmxDrvVCOPhase_SEL(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 phase=0;

  if(i4Argc==2)
  { 
    phase = StrToInt(szArgv[1]); 
   
  }

  vDrvVCOPhase_SEL(phase);
  Printf("vDrvVCOPhase_SEL=%d\n",phase);     
     
  return 0;
}

static INT32 _PmxSET_DDDSFreeRun(INT32 i4Argc, const CHAR **szArgv)
{
//  SET_DDDSFreeRun();  
//  Printf("SET_DDDSFreeRun"); 
  if (i4Argc < 7)
  {
      Printf("Usage: bKP0_START bKP0_END bKI0_START bKI0_END ERROR_LIM Range\n");
      return 1;
  }

  vDrvEnable_DDDS_Calibration(StrToInt(szArgv[1]),StrToInt(szArgv[2]), StrToInt(szArgv[3]), StrToInt(szArgv[4]), StrToInt(szArgv[5]),StrToInt(szArgv[6]));
     
  return 0;
}

static INT32 _PmxSET_DDDSErrorLimit(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc < 2)
  	{
    	Printf("Usage: dddslm <DDDS_ERROR_LIMIT (0x00~0x3f)> \n");
    	Printf("Current DDDS_ERROR_LIMIT = 0x%x\n", IO32ReadFldAlign(DDDS_03,DDDS_03_DDDS_ERR_LIM));
        return 1;
  	}
  	
  	vDrvSetErrorLimit((UINT32)StrToInt(szArgv[1]));
  	
	return 0;
}

static INT32 _VByOneScript (INT32 i4Argc, const CHAR **szArgv)
{
  if(x_strcmp(szArgv[1], "0") ==0 )
  { 
    vVByOneLane4_8bit_script(); 
    Printf("load 8 bits 4 lanes done\n");
  }
  else if(x_strcmp(szArgv[1], "1") ==0 )
  {
    vVByOneLane4_10bit_script();
    Printf("load 10 bits 4 lanes done\n");
  }
  else
  {
    vVByOneLane4_12bit_script();
    Printf("load 12 bits 4 lanes done\n");
  }
  
  return 0;
}

static INT32 _VByOneCRCChk (INT32 i4Argc, const CHAR **szArgv)
{
	UNUSED(i4Argc);
	UNUSED(szArgv);

	vDrvVByOneCRCCheck();
	return 0;
}

static INT32 _VByOneImpCal (INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vVByOne_Impedance_Cal();
    return 0;
}

static INT32 _VByOneTestMode (INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vVByOneTestMode();
    return 0;
}

static INT32 _VByOneStatus (INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vGetVByOneStatus();
    return 0;
}

static INT32 _VByOneLockChk (INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Status = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);
	if(i4Argc == 2)
	{
		u1Status = StrToInt(szArgv[1]);
	
		if (u1Status == 1)
			Printf("vb1 unlock check start\n");
		else
			Printf("vb1 unlock check end\n");

	
		vSetVByOneUnLockChk(u1Status);
	}

    return 0;
}

static INT32 _VByOneLockCnt (INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vGetVByOneUnLockCnt();
    return 0;
}

static INT32 _VByOneOn (INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvVByOneOn();
    return 0;
}

static INT32 _VByOneOff (INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvVByOneOff();
    return 0;
}

#ifdef CC_MT5882
#define I2C_CHANNEL	1
#else
#define I2C_CHANNEL	2
#endif
static UINT8 _au1cmiMst4K2K60VB1Buf[] = {
                                            0x20, 0x01, 0x02, 0x00, 0x40, 0xFF, 0x00,
                                            0x80, 0x02, 0x00, 0x40, 0x62, 0x51, 0x73,                                            
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,

											0x23, 0x1b, 0x10, 0x27, 0xc2, 0xee, 0xd0,  // add delay for CDR training
                                            0x33, 0x1b, 0x10, 0x27, 0x40, 0x71, 0xd0,                                            
                                            0x33, 0x1b, 0x10, 0x27, 0xc3, 0xee, 0xd0, 
                                            
                                            
                                            0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // video mode
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
                                            0x33, 0x1c, 0x13, 0x00, 0x04, 0x40, 0x40,  //reset
                                            0x33, 0x1c, 0x13, 0x00, 0x04, 0x00, 0x40};
static UINT8 _au1cmiMstFHD120VB1Buf[] = {	//120Hz 
											0x80, 0x01, 0x02, 0x00, 0x00, 0x55, 0x00,
											0x80, 0x02, 0x00, 0x00, 0x02, 0x01, 0x03,											 
											0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
											
											0x23, 0x1b, 0x10, 0x27, 0xc2, 0xee, 0xd0,  // add delay for CDR training
											0x33, 0x1b, 0x10, 0x27, 0x40, 0x71, 0xd0,											 
											0x33, 0x1b, 0x10, 0x27, 0xc3, 0xee, 0xd0, 
											
											0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
											0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // video mode
											0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,  //color engine bypass 
											0x33, 0x1c, 0x13, 0x00, 0x04, 0x40, 0x40,  //reset
											0x33, 0x1c, 0x13, 0x00, 0x04, 0x00, 0x40};
static UINT8 _au1cmiMstFHD60VB1Buf[] = {	//60Hz 
											0x40, 0x01, 0x02, 0x00, 0x00, 0x11, 0x00,
											0x30, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
											0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
											
											0x23, 0x1b, 0x10, 0x27, 0xc2, 0xee, 0xd0,  // add delay for CDR training
											0x33, 0x1b, 0x10, 0x27, 0x40, 0x71, 0xd0,											 
											0x33, 0x1b, 0x10, 0x27, 0xc3, 0xee, 0xd0, 
											
											0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,											 
											0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // video mode
											0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,  //color engine bypass  
											0x33, 0x1c, 0x13, 0x00, 0x04, 0x40, 0x40,  //reset
											0x33, 0x1c, 0x13, 0x00, 0x04, 0x00, 0x40};
static UINT8 _au1cmiMst4K2K30VB1Buf[] = {
                                            0x80, 0x01, 0x02, 0x00, 0x00, 0x55, 0x00,
                                            0x80, 0x02, 0x00, 0x00, 0x02, 0x01, 0x03,                                            
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            
                                            0x23, 0x1b, 0x10, 0x27, 0xc2, 0xee, 0xd0,  // add delay for CDR training
                                            0x33, 0x1b, 0x10, 0x27, 0x40, 0x71, 0xd0,                                            
                                            0x33, 0x1b, 0x10, 0x27, 0xc3, 0xee, 0xd0, 
                                            
                                            0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // video mode
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,  //color engine bypass
                                            0x33, 0x1c, 0x13, 0x00, 0x04, 0x40, 0x40,  //reset
                                            0x33, 0x1c, 0x13, 0x00, 0x04, 0x00, 0x40};											

static INT32 _VByOneINXI2CCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1CmdType = 0;
	if (i4Argc < 2)
    {
		vDrvGetScanPWMStepControlStatus();
		Printf("Usage: %s <cmd type>\n", szArgv[0]);
		Printf("<cmd type> 1:FHD60; 2:FHD120; 3:4K2K30; 4:4K2K60\n", szArgv[0]);
        return 1;
    }
	else
	{
		UINT8 *pTmp, u1CmdLen, k, i, u1Checksum = 0;
		UINT16 u2Len;
		UINT32 iret;
		u1CmdLen = 7;
		u1CmdType = PARAM(0);
	    switch(u1CmdType)
	    {
			case 1:  // FHD60
				Printf("[INX VB1] FHD60 cmd\n");
				pTmp = _au1cmiMstFHD60VB1Buf;
				u2Len = sizeof(_au1cmiMstFHD60VB1Buf);
				break;
			case 3:  // 4K2K30
				Printf("[INX VB1] 4K2K30 cmd\n");
				pTmp = _au1cmiMst4K2K30VB1Buf;
				u2Len = sizeof(_au1cmiMst4K2K30VB1Buf);
				break;
			case 4:  // 4K2K60
				Printf("[INX VB1] 4K2K60 cmd\n");
				pTmp = _au1cmiMst4K2K60VB1Buf;
				u2Len = sizeof(_au1cmiMst4K2K60VB1Buf);
				break;
			case 2:  // FHD120
			case 0:
			default:
				Printf("[INX VB1] FHD120 cmd\n");
				pTmp = _au1cmiMstFHD120VB1Buf;
				u2Len = sizeof(_au1cmiMstFHD120VB1Buf);
				break;
		}

		for(k=0; k < u2Len ; k=k+u1CmdLen)
		{
			u1Checksum = 0;
			{
			    for(  i=1 ; i< u1CmdLen ; i++)
			    {
					//skip data 0
						u1Checksum += pTmp[i] ;
			    }
			}    
			Printf("\n[INX VB1] I2c Data :");
			{
		    	pTmp[0] = (pTmp[0] &0xF) + ((u1Checksum & 0xF) << 4) ; //upper 4 bit ;
		    }	
			 for(i=0 ; i < u1CmdLen ; i++)
			 {
				Printf("0x%x ", pTmp[i]);
			 }
			 Printf("\n");
		     iret = SIF_X_Write(I2C_CHANNEL, 0x100 , 0x38, 0 ,0, pTmp, u1CmdLen);
		     if(iret <=0)
		     {
				Printf("[INX VB1] i2c send error\n");

		     }
		     pTmp = pTmp + u1CmdLen ;
		     //command delay
		     x_thread_delay(50);
	     }

		return 0;
	}
}

static INT32 _PmxSetCmdCtlWord(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4CtlWord;

    u4CtlWord = PANEL_GetControlWord();
    Printf("Curr panel controlword = %08x\n", u4CtlWord);
    if(i4Argc == 2)
    {
        u4CtlWord = StrToInt(szArgv[1]);
        Printf("Set controlword to %08x\n", u4CtlWord);
        PANEL_SetControlWord(u4CtlWord);
    }
    return 0;
}

static INT32 _PmxSetCmdCtlWord2(INT32 i4Argc, const CHAR **szArgv)
{
	#ifdef EnablePANELCW2
    UINT32 u4CtlWord;

    u4CtlWord = PANEL_GetControlWord2();
    Printf("Curr panel controlword2 = %08x\n", u4CtlWord);
    if(i4Argc == 2)
    {
        u4CtlWord = StrToInt(szArgv[1]);
        Printf("Set controlword 2 to %08x\n", u4CtlWord);
        PANEL_SetControlWord2(u4CtlWord);
    }
	#endif
	return 0;
}

#ifdef CC_SCPOS_3DTV_SUPPORT
static INT32 _PmxSetScanningPWM(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Devider=1;
    UINT32 u4High=0;
	UINT32 u4Low=0;
	
    UINT8  u1Src = PARAM(0);
    UINT32 u4Start = PARAM(1);
    UINT32 u4Bright = PARAM(2);
    UINT32 u4Freq = (i4Argc == 5)?PARAM(3):BACKLT_GetDimmingFrequency();

    CHK_NUM_MIN_PARAM(4, "scanPWMSrc(1~6) start(0%~100%) duty(0~100%) freq(Hz)");

    if (IsPwmLowPanelBright() == PWM_LOW_PANEL_BRIGHT)
    {
	    u4Bright = 100 - u4Bright;
    }

    if(u4Freq == 0)
	{
		u4Freq = BACKLT_GetDimmingFrequency();
	}
	
	if((vDrvGetLCDFreq() == 25) || (vDrvGetLCDFreq() == 50))
    {
        u4Devider = u4Freq/50;
    }
    else if(SUPPORT_PANEL_48HZ && ((vDrvGetLCDFreq() == 24) || (vDrvGetLCDFreq() == 48)))
    {
        u4Devider = u4Freq/48;
    }
    else if ((vDrvGetLCDFreq() == 30) || (vDrvGetLCDFreq() == 60))
    {
        u4Devider = u4Freq/60;
    }
    else if (vDrvGetLCDFreq() == 100)
    {
	    u4Devider = u4Freq/100;
    }
    else if (vDrvGetLCDFreq() == 120)
    {
	    u4Devider = u4Freq/120;
    }

    if ((u4Bright>0)&&(u4Bright<1000))
    {
		u4High = u4Bright/u4Devider;
		u4Low = (1000-u4Bright)/u4Devider;
    }
    else if (u4Bright>=1000)
    {
		u4High = 1000;
    }
    vDrvInitSCANPWM(u1Src);
    vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Aligned Vsync
    vDrvSetScanPWM(u1Src,u4Start,u4High,u4Low);
    vDrvScanPWMDataFire();

    return 0;
}

static INT32 _PmxGetScanningStatus(INT32 i4Argc, const CHAR **szArgv)
{
	vDrvSetScanPWMSW_SETDATA_Status();
    return 0;	
}

static INT32 _PmxGetScanningStepCtl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 fgStepCtl;
    UINT16 u2step;	

    if (i4Argc < 3)
    {
		vDrvGetScanPWMStepControlStatus();
		Printf("Usage: %s <enable><step>\n", szArgv[0]);
        return 1;
    }
	
    fgStepCtl = StrToInt(szArgv[1]);
    u2step = StrToInt(szArgv[2]);	
	vDrvSetScanPWMStepControl(fgStepCtl, u2step);
    return 0;
}

static INT32 _PmxGetScanningTarget(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4HighTarget,u4LowTarget;	

    if (i4Argc < 3)
    {
		vDrvGetScanPWMStepControlStatus();
		Printf("Usage: %s <High Target><Low Target>\n", szArgv[0]);
        return 1;
    }
	
    u4HighTarget = StrToInt(szArgv[1]);
    u4LowTarget = StrToInt(szArgv[2]);	
	vDrvSetScanPWMStepTarget(u4HighTarget, u4LowTarget);
    return 0;
}
#endif

static INT32 _PmxTXCRCStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vTXCRCStatus();

    return 0;
}

static INT32 _PmxTXIFDigitalPadSet(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc < 2)
    {
    	Printf("Usage: %s\n1. <digital pad PD>([0]:power on, [1]:power down)\n2. <mode>([2]:user set) <value>(0x******)\n", szArgv[0]);
		u4ReadDPadPD();
    }
	else if (i4Argc < 3)
	{
		UINT8 u1PDOnOff = StrToInt(szArgv[1]);
		if (u1PDOnOff < 2)
		{
			vSetLVDSDPadPD(u1PDOnOff,0);
		}
		else
		{
			Printf("If want to set the RG_PD, please input: pmx.if.dpen 2 0x******\n");
		}
	}
	else if (i4Argc < 4)
	{
		UINT8 u1Mode = StrToInt(szArgv[1]);
		if (u1Mode==2)
		{
			UINT32 u4Value = StrToInt(szArgv[2]);
			vSetLVDSDPadPD(u1Mode,u4Value);
		}
		else
		{
			Printf("If want to set the RG_PD, please input: pmx.if.dpen 2 0x******\n");
		}
	}
	return 0;
}

static INT32 _PmxTXIFAnalogPadSet(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc < 2)
    {
	#ifdef CC_MT5399
		Printf("Usage: %s\n1. <analog pad PD>([0]:power on, [1]:power down)\n2. <mode>([2]:user set) <LVDSB_EXT_EN> <LVDSB_DRV_PWD> <LVDSA_EXT_EN> <LVDSA_DRV_PWD>\n", szArgv[0]);
		
	#else
		Printf("Usage: %s\n1. <analog pad PD>([0]:power on, [1]:power down)\n2. <mode>([2]:user set) <LVDSB_EXT_EN> <LVDSB_DRV_PWD>\n", szArgv[0]);
	#endif
		u4ReadAPad();
    }
	else if (i4Argc < 3)
	{
		UINT8 u1PDOnOff = StrToInt(szArgv[1]);
		if (u1PDOnOff < 2)
		{
			vSetLVDSAPad(u1PDOnOff,0, 0, 0, 0);
		}
		else
		{
		#ifdef CC_MT5399
			Printf("If want to set the reg, please input: pmx.if.apen 2 0x****, 0x****, 0x****, 0x****\n");
		#else
			Printf("If want to set the reg, please input: pmx.if.apen 2 0x****, 0x****\n");
		#endif
		}
	}
#ifndef CC_MT5399
	else if (i4Argc < 5)
	{
		UINT8 u1Mode = StrToInt(szArgv[1]);
		if (u1Mode==2)
		{
			UINT32 u4Value1 = StrToInt(szArgv[2]);
			UINT32 u4Value2 = StrToInt(szArgv[3]);
			vSetLVDSAPad(u1Mode,u4Value1, u4Value2, 0, 0);
		}
		else
		{
			Printf("If want to set the reg, please input: pmx.if.apen 2 0x****, 0x****\n");
		}
	}
#else
	else if (i4Argc < 7)
	{
		UINT8 u1Mode = StrToInt(szArgv[1]);
		if (u1Mode==2)
		{
			UINT32 u4BExtEn = StrToInt(szArgv[2]);
			UINT32 u4BDrvPwd = StrToInt(szArgv[3]);
			UINT32 u4AExtEn = StrToInt(szArgv[4]);
			UINT32 u4ADrvPwd = StrToInt(szArgv[5]);
			vSetLVDSAPad(u1Mode,u4BExtEn, u4BDrvPwd, u4AExtEn, u4ADrvPwd);
		}
		else
		{
			Printf("If want to set the reg, please input: pmx.if.apen 2 0x****, 0x****, 0x****, 0x****\n");
		}
	}
#endif
	return 0;
}

static INT32 _PmxTXIFGroupSet(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4AGROUP,u4BGROUP,u4CGROUP,u4DGROUP;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc < 5)
    {
     	u4AGROUP = vGetPI_GROUP(0);
		u4BGROUP = vGetPI_GROUP(1);
		u4CGROUP = vGetPI_GROUP(2);
		u4DGROUP = vGetPI_GROUP(3);
	
        Printf("Usage: <A_Group><B_Group><C_Group><D_Group>\n", szArgv[0]);
		Printf("[Current setting] A=%d, B=%d, C=%d, D=%d \n",u4AGROUP,u4BGROUP,u4CGROUP,u4DGROUP);		
        Printf("[Example] pmx.if.sgroup 0 1 2 3\n");		

        return 1;
    }
    u4AGROUP = StrToInt(szArgv[1]);
    u4BGROUP = StrToInt(szArgv[2]);
    u4CGROUP = StrToInt(szArgv[3]);
    u4DGROUP = StrToInt(szArgv[4]);    
    
    vSetPI_GROUP((UINT8)u4AGROUP,(UINT8)u4BGROUP,(UINT8)u4CGROUP,(UINT8)u4DGROUP);

    return 0;
}

static void PmxIFGroupPrint(UINT8 u1Link)
{	
	if (u1Link == 0)
	Printf("AE|");
	else if (u1Link == 1)
	Printf("AO|");		
	else if (u1Link == 2)
	Printf("BE|");
	else if (u1Link == 3)
	Printf("BO|");		
}


static INT32 _PmxTXIFGroupAuto(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Char;
	UINT8 A_Cnt, B_Cnt, C_Cnt, D_Cnt;
	UINT8 A_Cnt_tmp, B_Cnt_tmp, C_Cnt_tmp, D_Cnt_tmp;	
    UINT8 u1LoopCnt =0;	

    UNUSED(i4Argc);
    UNUSED(szArgv);

	A_Cnt_tmp = vGetPI_GROUP(0);
	B_Cnt_tmp = vGetPI_GROUP(1);
	C_Cnt_tmp = vGetPI_GROUP(2);
	D_Cnt_tmp = vGetPI_GROUP(3);

    Printf("Press [ESC] to quit\n");
    Printf("Press [N] or [n] to Next setting\n");	
    Printf("A_Cnt_tmp=%d, B_Cnt_tmp=%d, C_Cnt_tmp=%d, D_Cnt_tmp=%d \n",A_Cnt_tmp,B_Cnt_tmp,C_Cnt_tmp,D_Cnt_tmp);
	 
	for (A_Cnt = 0; A_Cnt<4; A_Cnt++)
	{
	for (B_Cnt = 0; B_Cnt<4; B_Cnt++)
	{
	for (C_Cnt = 0; C_Cnt<4; C_Cnt++)
	{	 
	for (D_Cnt = 0; D_Cnt<4; D_Cnt++)
	{  
	 if ((A_Cnt!=B_Cnt) && (A_Cnt!=C_Cnt)&& (A_Cnt!=D_Cnt)
		 && (B_Cnt!=C_Cnt) && (B_Cnt!=D_Cnt)
		 && (C_Cnt!=D_Cnt))
	 {
	 Printf("[%2d]A_Cnt=%d, B_Cnt=%d, C_Cnt=%d, D_Cnt=%d \n", u1LoopCnt,A_Cnt,B_Cnt,C_Cnt,D_Cnt);
 	 u1LoopCnt ++;
     vSetPI_GROUP(A_Cnt,B_Cnt,C_Cnt,D_Cnt);
	 
   	 u1Char = SerPollGetChar();
     if (u1Char == 27) // escape, end of auto pattern
     {
       Printf("Quit Auto Group Switch\n");
	   Printf("Please add the below define on your panel table\n");
	   Printf("LVDS_A_FROM_");
	   PmxIFGroupPrint(A_Cnt);
	   Printf("LVDS_B_FROM_");
	   PmxIFGroupPrint(B_Cnt);
	   Printf("LVDS_C_FROM_");
	   PmxIFGroupPrint(C_Cnt);
	   Printf("LVDS_D_FROM_");
	   PmxIFGroupPrint(D_Cnt);	   
       return 0;
     }	  
     else if ((u1Char >= 'N') && (u1Char <= 'n'))
     {
       continue;
     }	  
	 }					  
	}		  
	}		  
	}		  
	} 
	
	vSetPI_GROUP(A_Cnt_tmp,B_Cnt_tmp,C_Cnt_tmp,D_Cnt_tmp);
	
    return 0;
}

static void DEB_MESG_PMX(void)
{
    UINT8 u1Port, u1Pair;

    for (u1Port =0; u1Port <2; u1Port++)
    {
		for (u1Pair =0; u1Pair <12; u1Pair++)
		{
			Printf("%x" ,u1Get_DataPair(u1Port,u1Pair));
		}
    }
	Printf("\n");	
}


static INT32 _PmxTXIFPairChk(INT32 i4Argc, const CHAR **szArgv)
{

    UINT8 u1Char, u1SearchDE, u1LoopCnt=0;
  	UINT8 u1PairMatrix_tmp[2][12];
    UINT8 u1Port, u1Pair, u1Porttmp=0, u1Pairtmp=0;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    for (u1Port =0; u1Port <2; u1Port++)
    {
		for (u1Pair =0; u1Pair <12; u1Pair++)
		{
			u1PairMatrix_tmp[u1Port][u1Pair]= u1Get_DataPair(u1Port,u1Pair);
		}
    }
	DEB_MESG_PMX();
	vSetLVDSDAPadPDForceOn();

    Printf("Press [ESC] to quit\n");
    Printf("Press [N] or [n] to Next setting\n");	

	for (u1SearchDE = 0; u1SearchDE<2; u1SearchDE++)
	{
	 u1LoopCnt = 0;		
	 u1Porttmp = 0;
	 u1Pairtmp = 0;
	 
	    for (u1Port =0; u1Port <2; u1Port++)
	    {
			for (u1Pair =0; u1Pair <12; u1Pair++)
			{
			    if(u1SearchDE)
				vSet_DataPair(u1Port,u1Pair,0x0A);					
				else
				vSet_DataPair(u1Port,u1Pair,0x02);
			}
	    }	
		
	if(u1SearchDE)
	Printf("==== Search DE Pair Start \n");
	else
	Printf("==== Search Clock Pair Start \n");

	
		for (u1Port = 0; u1Port<2; u1Port++)
		{			
			for (u1Pair = 0; u1Pair<12; u1Pair++)
			{
			 if(u1SearchDE)
			 Printf("[%2d] DE Pair [Port:%2d, Pair: %2d]\n",u1LoopCnt,u1Port, u1Pair);
			 else
			 Printf("[%2d] CK Pair [Port:%2d, Pair: %2d]\n",u1LoopCnt,u1Port, u1Pair);
			 
		     if (u1LoopCnt)
		     {
		      if(u1SearchDE)
			  vSet_DataPair(u1Porttmp,u1Pairtmp,0x0A);	  	
			  else
			  vSet_DataPair(u1Porttmp,u1Pairtmp,0x02);
			  DEB_MESG_PMX();
		     }
		 	 u1LoopCnt ++;
			 if(u1SearchDE)
		     vSet_DataPair(u1Port,u1Pair,0x02);	 	
			 else	
		     vSet_DataPair(u1Port,u1Pair,0x0A);
			 DEB_MESG_PMX();	 
			 u1Porttmp = u1Port;
			 u1Pairtmp = u1Pair;
	 
		   	 u1Char = SerPollGetChar();
	 
	    	 if (u1Char == 27) // escape, end of auto pattern
    	 	 {
		       Printf("Quit LVDS Cable Check\n");
			   if(u1SearchDE)
			   Printf("[%2d] DE Pair [Port:%2d, Pair: %2d]\n",u1LoopCnt,u1Port, u1Pair);
			   else
			   Printf("[%2d] CK Pair [Port:%2d, Pair: %2d]\n",u1LoopCnt,u1Port, u1Pair);			   
			   DEB_MESG_PMX();	   
		       return 0;
	    	 }	  
		     else if ((u1Char >= 'N') && (u1Char <= 'n'))
		     {
	    	   continue;
		     }	  
				  	  
			}		  
		} 
	}

    for (u1Port =0; u1Port <2; u1Port++)
    {
		for (u1Pair =0; u1Pair <12; u1Pair++)
		{
			vSet_DataPair(u1Port,u1Pair,u1PairMatrix_tmp[u1Port][u1Pair]);
		}
    }
	DEB_MESG_PMX();
			
    return 0;
}


static INT32 _PmxTXIFLineBufferEn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Status;

    UNUSED(i4Argc);
    UNUSED(szArgv);
	
	u1Status= u1TCONIFLineBufferGet();
	if (u1Status == 1)
		Printf("6 line buffer bypass\n");
	else
		Printf("6 line buffer Enable\n");

    if(i4Argc == 2)
    {
        u1Status = StrToInt(szArgv[1]);

		if (u1Status == 1)
			Printf("Set 6 line buffer bypass\n");
		else
			Printf("Set 6 line buffer Enable\n");

		u1TCONIFLineBufferSet(u1Status);
    }
	
	
	
    return 0;
}

static INT32 _PmxTXHVSYNCEncodeQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
	
	vGetLVDS_HVEncodeStatus();
	
    return 0;
}

static INT32 _PmxTXHVSYNCEncodeEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 mode=0;
	
    if(i4Argc==2)
    { 
     mode = StrToInt(szArgv[1]); 
	 u1SetLVDS_HVEncodeValue(mode); 	 
    }
    else
    {
     Printf("Arg: encode setting \n");	
	 Printf("bit [0] VSYNC encode enable\n");			 
	 Printf("bit [1] VSYNC encode value\n");			 
	 Printf("bit [2] HSYNC encode enable\n");	
	 Printf("bit [3] HSYNC encode value\n");	
    }
  
    return 0;
}

static INT32 _PmxTXIFDISP_RQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
	
	vGetDISP_RStatus();
	
    return 0;
}

static INT32 _PmxTXIFDISP_REnable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 mode=0;
	
    if(i4Argc==2)
    { 
     mode = StrToInt(szArgv[1]); 
	 u1SetDISP_REncodeEn(mode); 	 
    }
    else
    {
     Printf("Arg: Enable encode position\n");	
	 Printf("bit [0] A channel\n");			 
	 Printf("bit [1] B channel\n");			 
	 Printf("bit [2] C channel\n");	
	 Printf("bit [3] D channel\n");	
	 Printf("bit [4] OSD Hsync channel\n");	
	 Printf("bit [5] OSD VSync channel\n");		  
    }
  
    return 0;
}

static INT32 _PmxTXIFGlasses_Control(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Pin = PARAM(0);
	UINT8 u1Type = PARAM(1);
	BOOL fgPol = PARAM(2);
	BOOL fgPos = PARAM(3);
	UINT8 u1DelayV = PARAM(4);
	UINT16 u2DelayH = PARAM(5);

	if(i4Argc == 7)
	{
     Panel_3DGlassesCTL(u1Pin,u1Type,fgPol,fgPos,u1DelayV,u2DelayH);
	} 
	else
	{
	 Printf("Arg: pin_name pin_function Pin_polarity pin_position delay_v delay_h\n");   
	 Printf("pin_name: [0] 3DR [1] 3DL [2]3DBL\n");			
	 Printf("Pin_function: [0] 3DRL [1] 3DR [2] 3DL [3]3DBL [4]DISP_R [5]E/O Idx [6]vsync [7] hsync\n");			
	 Printf("Pin_polarity: [0] normal [1] reverse\n"); 
	 Printf("pin_position: [0] 1st [1] 2nd\n"); 
	 Printf("delay_v: [0~3] frame delay\n"); 
	 Printf("delay_h: [0~(htotal-1)] line delay\n"); 
	 Printf("[Example] pmx.if.glactr 1 1 0 0 0 600 \n");
	 Printf("pin:3DL source: 3DR delay h: 600 line\n");	 
	}
    return 0;	
}

static INT32 _PmxTXIFGlasses_RSTControl(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Type = PARAM(0);
	BOOL fgOnOff = PARAM(1);
	UINT16 u2RstLine = PARAM(2);

	if(i4Argc == 4)
	{
     Panel_3DGlassesRSTCTL(u1Type,fgOnOff,u2RstLine);
	} 
	else
	{
	 Printf("Arg: pin_function fgOnOff u2RstLine \n");   	
	 Printf("Pin_function: [0] 3DRL [1] 3DR [2] 3DL [3]3DBL [4]DISP_R [5]E/O Idx [6]vsync [7] hsync\n");			
	 Printf("fgOnOff [0] normal [1] reset mode\n"); 
	 Printf("u2RstLine: [0~(htotal-1)]\n"); 
	 Printf("[Example] pmx.if.glarst 3 1 562 \n");
	 Printf("source: (3DBL) Mode: (reset mode) width: (562 line) \n");	 
	}
    return 0;	
}

static INT32 _PmxTXIFGlasses_DutyControl(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Type = PARAM(0);
	UINT8 u2DutyH = PARAM(1);

	if(i4Argc == 3)
	{
     Panel_3DGlassesDutyCTL(u1Type,u2DutyH);
	} 
	else
	{
	 Printf("Arg: pin_function u2DutyH \n");   	
	 Printf("Pin_function: [0] 3DRL [1] 3DR [2] 3DL [3]3DBL [4]DISP_R [5]E/O Idx [6]vsync [7] hsync\n");			
	 Printf("u2DutyH: percent \n"); 
	 Printf("[Example] pmx.if.gladuty 3 50\n");
	 Printf("source: (3DBL) duty: 50 percent \n");	 
	}
    return 0;	
}

static INT32 _PmxTXIFGlasses_Enable(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Type = PARAM(0);
	BOOL fgOnOff = PARAM(1);

	if(i4Argc == 3)
	{
     Panel_3DGlassesCTLEn(u1Type,fgOnOff);
	} 
	else
	{
	 Printf("Arg: pin_name fgonoff \n");   	
	 Printf("pin_name: [0] 3DR [1] 3DL [2]3DBL\n");	
	 Printf("fgonoff: [0] off [1] on \n"); 
	}
    return 0;	
}

static INT32 _PmxTXIFIS_Control(INT32 i4Argc, const CHAR **szArgv)
{

 UINT8 u1En = PARAM(0);
 UINT8 u1Source= PARAM(1);
 UINT8 u1Inv= PARAM(2); 
 UINT8 u1SyncCnt= PARAM(3); 
 UINT8 u1ISSyncCnt= PARAM(4);  
 UINT8 u1MaskNum= PARAM(5);   
 
 if(i4Argc == 7)
 {
 	vDrvSetISOnOff(u1Source,u1En);
	vDrvSetISInputInverse(u1Source,u1Inv);	
 	vDrvSetISRelation(u1Source,u1SyncCnt,u1ISSyncCnt);
	vDrvSetISMaskNum(u1Source,u1MaskNum);
 }
 else
 {
    Printf("Arg: enable sync_type snc_inverse sync_cnt isync_cnt\n"); 
	Printf("[enable] 0: disable 1: enable\n"); 
	Printf("[sync_type] 0: HSYNC 1: VSYNC\n"); 	
	Printf("[sync_inverse] 0: normal 1: inverse\n");	
	Printf("[sync_cnt] 1~15\n"); 	
	Printf("[isync_cnt] 1~15\n"); 		
	Printf("[mask_cnt] 1~15\n"); 	
 }
    return 0;	 
}

static INT32 _PmxTXIFISDelay_Control(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1En = PARAM(0);
	UINT8 u1Source= PARAM(1);
	UINT16 u2DelayCnt= PARAM(2);

	if(i4Argc == 4)
	{
	vDrvSetISDelayOnOff(u1Source,u1En);
	vDrvSetISDelayNum(u1Source,u2DelayCnt);
	}
	else
	{
		Printf("Arg: enable sync_type sync_cnt isync_cnt\n"); 
		Printf("[enable] 0: disable 1: enable\n"); 
		Printf("[sync_type] 0: HSYNC 1: VSYNC\n");	
		Printf("[delay_cnt] \n");	
	}
    return 0;		
}

static INT32 _PmxTXIFPWM_Init(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PwmSrc=0;
	
	if(i4Argc==2)
    { 
    	u1PwmSrc = StrToInt(szArgv[1]);
    	if ((u1PwmSrc>=1)&&(u1PwmSrc<=8))
    	{
			vDrvInitSCANPWM(u1PwmSrc); 
		}
		else
		{
			Printf("scanPWM_src (1: SCAN1, 2: SCAN2, 3: SCAN3, 4: SCAN4, 5: SCAN5, 6:SCAN6, 7:SCAN7, 8:SCAN8)\n");
		}
    }
    else
    {
    	Printf("usage: pwmi <scanPWM_src>\n");
    	Printf("scanPWM_src (1: SCAN1, 2: SCAN2, 3: SCAN3, 4: SCAN4, 5: SCAN5, 6:SCAN6 7:SCAN7, 8:SCAN8)\n");
    }
	
    return 0;

}

static INT32 _PmxTXIFPWM_Enable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PwmSrc=0;
    UINT8 u1Enable=0;
	
    if(i4Argc==3)
    { 
    	u1PwmSrc = StrToInt(szArgv[1]);
    	u1Enable = StrToInt(szArgv[2]);
    	if ((u1PwmSrc>=1)&&(u1PwmSrc<=8))
    	{
			vDrvSetScanPWMOnOff(u1PwmSrc, u1Enable); 
		}
		else
		{
			Printf("scanPWM_src (1: SCAN1, 2: SCAN2, 3: SCAN3, 4: SCAN4, 5:SCAN5, 6:SCAN6 7:SCAN7, 8:SCAN8)\n");
		}
    }
    else
    {
    	Printf("usage: pwmen <scanPWM_src> <fgEnable>\n");
    	Printf("scanPWM_src (1: SCAN1, 2: SCAN2, 3: SCAN3, 4: SCAN4, 5: SCAN5, 6:SCAN6 7:SCAN7, 8:SCAN8)\n");
    	Printf("fgEnable(0:off, 1:on) \n");	
    }
	
    return 0;
}

static INT32 _PmxTXIFPWM_Set(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Source = 0;
	UINT32 u4Start= 0;
	UINT32 u4Hight= 0;
	UINT32 u4Low= 0;

	if(i4Argc == 5)
	{
		u1Source = PARAM(0);
		u4Start = PARAM(1);
		u4Hight = PARAM(2);
		u4Low = PARAM(3);

		if (u4Start>1000)
			u4Start = 1000;
		if (u4Hight>1000)
			u4Hight = 1000;
		if (u4Low>1000)
			u4Low = 1000;
		
    	if ((u1Source>=1)&&(u1Source<=8))
    	{
    		vDrvInitSCANPWM(u1Source);
		    vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Aligned Vsync
		    vDrvSetScanPWM(u1Source,u4Start,u4Hight,u4Low);
		    vDrvScanPWMDataFire();
		}
		else
		{
			Printf("scanPWM_src (1: SCAN1, 2: SCAN2, 3: SCAN3, 4: SCAN4, 5: SCAN5, 6: SCAN6 7:SCAN7, 8:SCAN8)\n");
		}
	}
    else
    {
 	 Printf("usage: pwmset <scanPWM_src> <sart_period> <high_period> <low_period>\n"); 
 	 Printf("scanPWM_src (1: SCAN1, 2: SCAN2, 3: SCAN3, 4: SCAN4, 5: SCAN5, 6: SCAN6 7:SCAN7, 8:SCAN8)\n");
	 Printf("start_period (0~1000, ex: 1 --> 0.001)\n"); 
	 Printf("high_period (0~1000, ex: 400 --> 0.4)\n");	
	 Printf("low_period (0~100, ex: 600 --> 0.6)\n");	
    }	
	return 0;
}

static INT32 _PmxTXIFPWM_AutoTest(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1mode = 0;
	UINT8 u1Source= 0;
	UINT32 u4Start= 0;
	UINT32 u4Int = 1;
	UINT8 u2Cnt=0;
	UINT32 u4High = 0;
	UINT32 u4Low = 0;
	//UINT8 u1Char;
	
	if (i4Argc == 5)
	{
		u1mode = PARAM(0);
		u1Source = PARAM(1);
		u4Start = PARAM(2);
		u4Int = PARAM(3);
		
		if (u4Start>100)
			u4Start = 100;

		if ((u1Source>=1)&&(u1Source<=8))
    	{
			vDrvInitSCANPWM(u1Source);
			vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Aligned Vsync
			if (!u1mode) // 100%-->0%
			{
				for (u2Cnt=100; u2Cnt>0; u2Cnt--)
				{
					u4High = (100-u4Start)*u2Cnt/100;
					u4High = u4High/u4Int;
					u4Low = (100-u4Start)-(u4High*u4Int);
					u4Low = u4Low/u4Int;
					Printf("%d", u2Cnt);
					vDrvSetScanPWM(u1Source,u4Start*10,u4High*10,u4Low*10);
					vDrvScanPWMDataFire();
					vUtDelay10ms(5);
			    #if 0
					u1Char = SerPollGetChar();
				    if (u1Char == 27) // escape, end of auto pattern
				    {
				    	Printf("Quit Auto scanPWM Test!!!\n");
				    	vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_OFF);
				        return 0;
     				}
     			#endif
				}
			}
			else // 0%-->100%
			{
				for (u2Cnt=0; u2Cnt<100; u2Cnt++)
				{
					u4High = (100-u4Start)*u2Cnt/100;
					u4High = u4High/u4Int;
					u4Low = (100-u4Start)-(u4High*u4Int);
					u4Low = u4Low/u4Int;
					Printf("%d", u2Cnt);
					vDrvSetScanPWM(u1Source,u4Start*10,u4High*10,u4Low*10);
					vDrvScanPWMDataFire();
					vUtDelay10ms(5);
				#if 0				
					u1Char = SerPollGetChar();
				    if (u1Char == 27) // escape, end of auto pattern
				    {
				    	Printf("Quit Auto scanPWM Test!!!\n");
				    	vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_OFF);
				        return 0;
     				}
     			#endif
				}
			}
			//vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Set data
		}
		else
		{
			Printf("scanPWM_src (1: SCAN1, 2: SCAN2, 3: SCAN3, 4: SCAN4, 5: SCAN5, 6: SCAN6 7:SCAN7, 8:SCAN8)\n");
		}
	}
	else
	{
		Printf("Usage: pwmat <Up/Down> <scanPWM_src> <Start_Period> <Integer>\n");
    	Printf("Up/Down (0: down, 1: up)\n");
    	Printf("scanPWM_src (1: SCAN1, 2: SCAN2, 3: SCAN3, 4: SCAN4, 5: SCAN5, 6:SCAN6 7:SCAN7, 8:SCAN8)\n");
    	Printf("start_period (0~100, ex: 0 --> 0%)\n");
    	Printf("interger (1,2,3...)\n");
	}
return 0;
}

static INT32 _PmxINTRStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
	
	vDrvVsyncISRStatus();
	
    return 0;
}

static INT32 _PmxINTRSetting(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2Line=0;
    UINT16 u2Mask=0;	

    if(i4Argc==3)
    { 
     u2Line = StrToInt(szArgv[1]); 
	 u2Mask = StrToInt(szArgv[2]); 
	 vDrvVsyncISRSetting(u2Line, u2Mask); 	 
	 vDrvVsyncISRStatus();	 
    }	
    else
    {
	 vDrvVsyncISRStatus();    
     Printf("Arg: [INTR postion Line] [Mask] \n");
     Printf("ex: 1121 0x3F \n");
    }
  
    return 0;
}


static INT32 _PmxUHDStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
	
    #if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)	
	vPanelAttribute_Status_4k2k();
    #endif	
    return 0;
}

static INT32 _PmxUHDSetting(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode=0;

    if(i4Argc==2)
    { 
     u1Mode = StrToInt(szArgv[1]); 	 
    #if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)	
	 SetPanel_4k2k(u1Mode);	 
	 #endif
    }	
    else
    {
    #if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)	
     Panel4K2KStatus();
 	 #endif
     Printf("Arg: E_4K2K_NORMAL = 0 \n");
     Printf("E_4K2K_NATIVE = 1 \n");
     Printf("E_4K2K_SPLIT_VDO = 2 \n");
     Printf("E_4K2K_SPLIT_PHOTO = 3  \n");
     Printf("E_4K2K_NORMAL_3D = 4  \n");
    }
  
    return 0;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

