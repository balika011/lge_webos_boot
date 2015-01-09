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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: nptv_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "nptv_debug.h"
#include "x_debug.h"

#include "source_table.h"
#include "sv_const.h"
#include "video_def.h"
#include "nptv_cmd.h"
#include "drv_scpos.h"
#include "drv_vdoclk.h"
#include "drv_video.h"
#include "vdo_if.h"
#include "vdo_misc.h"
#include "vdo_drvif.h"

#include "drv_scpos.h"
#include "drv_ycproc.h"
#include "mute_if.h"
#include "api_eep.h"
#include "eepdef.h"
#include "musb_if.h"
#include "drv_scaler_drvif.h"
#include "hw_sw.h"
#ifdef CC_CDC_SUPPORT
#include "mu_cdc_if.h"
#include "gdma_if.h"
#endif

//#ifdef CC_BOOT_MUTE_LOG
#include "feature.h"
//#endif
#ifdef CC_MT5398
#include "drv_mjc.h"
#include "drv_css.h"
#endif


#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
#endif


#ifndef CC_CLI
#define CC_CLI
#endif
#ifdef CC_CLI
static INT32 _NptvInitCmd (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMainSrcSelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvPipSrcSelCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvFixColorSpace(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvSetScanModeCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvVideoQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvVideoDelay(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvVideoModChg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvVideoSWRegRead(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvVideoSWRegWrite(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_MT5398
static INT32 _NptvCSSInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvCSSCompensate(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvCSSOnOff(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _NptvMuteOn(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteColor(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteEnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteDisable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvMuteForceOnOff(INT32 i4Argc, const CHAR **szArgv);
#if CC_BOOT_MUTE_LOG
static INT32 _NptvMuteBootLog(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _NptvDemoModeCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvFwOnOff (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvStatusCmd(INT32 i4Argc, const CHAR **szArgv);

#ifndef NDEBUG 
#ifndef __KERNEL__
//static INT32 _NptvMwDebug(INT32 i4Argc, const CHAR **szArgv);
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
static INT32 _NptvSetFlipMirror(INT32 i4Argc, const CHAR **szArgv);
#endif
#endif //NDEBUG


#if CC_MEASURE_SYS_TIME
static INT32 _NptvTimeMeasureEn(INT32 i4Argc, const CHAR **szArgv);
#endif

#ifdef CC_CDC_SUPPORT
static INT32 _NptvDevDumpCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvDramStatusChk(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvDevVideoDumpCmd(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _NptvFWUpgradeCmd(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_SUPPORT_4K2K
static INT32 _Nptv4k2kQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _Nptv4k2kSetAddr(INT32 i4Argc, const CHAR **szArgv);
static INT32 _Nptv4k2kSetMuteTime(INT32 i4Argc, const CHAR **szArgv);
static INT32 _Nptv4k2kSetDelay(INT32 i4Argc, const CHAR **szArgv);
#endif
#endif /*#ifdef CC_CLI*/

EXTERN void vSetUSBDump(BOOL bOnOff);
EXTERN void NptvScalerVideoDump(INT32 i4Argc, const CHAR **szArgv);

// ==========================================================
EXTERN UINT32 FlashGetBlkSize(UINT8 u1FlashType);
EXTERN INT8 FlashWrite(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size, UINT8 fgBoost);
EXTERN INT8 FlashRead(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size);
EXTERN INT8 FlashErase(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4Size);
EXTERN UINT32 CalcCheckSum(UINT32 u4MemPtr, UINT32 u4Size);
// ==========================================================




/******************************************************************************
* CLI Command Table
******************************************************************************/

CLIMOD_DEBUG_FUNCTIONS(NPTV)
CLIMOD_DEBUG_FUNCTIONS(MUTE)

CLI_EXEC_T arVdoCmdTbl[] = {
    #ifdef CC_CLI
    {"VideoQuery",      "q",     _NptvVideoQuery,     NULL, "Video Query",                 CLI_GUEST},
    {"VideoDelay",      "dly",   _NptvVideoDelay,     NULL, "Video Delay",                 CLI_GUEST},
    {"VideoModeChg",    "modchg",_NptvVideoModChg,    NULL, "Video Mode Change Done",      CLI_SUPERVISOR},
    {"VideoSWRegRead",  "swr",   _NptvVideoSWRegRead, NULL, "Read Software Register",      CLI_SUPERVISOR},
    {"VideoSWRegWrite", "sww",   _NptvVideoSWRegWrite,NULL, "Write Software Register",     CLI_SUPERVISOR},
    #endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_SUPPORT_4K2K
CLI_EXEC_T ar4k2kCmdTbl[] = {
    #ifdef CC_CLI
    {"4k2kQuery",      "q",     _Nptv4k2kQuery,     NULL, "4k2k Query",                 CLI_GUEST},
    {"4k2kSetAddr",      "addr",     _Nptv4k2kSetAddr,     NULL, "4k2k i2c addr",                 CLI_GUEST},    
    {"4k2kSetCmdDly",      "dly",     _Nptv4k2kSetDelay,     NULL, "4k2k cmd delay",                 CLI_GUEST},      
    {"4k2kSetMuteTime",      "mt",     _Nptv4k2kSetMuteTime,     NULL, "4k2k mute time",                 CLI_GUEST},          
    #endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_MT5398
CLI_EXEC_T arCssCmdTbl[] = {
    #ifdef CC_CLI
    {"CSSInitial",      "i",     _NptvCSSInit,        NULL, "CSS Initial",                 CLI_SUPERVISOR},
    {"CSSCompensate",   "comp",  _NptvCSSCompensate,  NULL, "CSS Compensate",              CLI_SUPERVISOR},
    {"CSSOnOff",        "onoff", _NptvCSSOnOff,       NULL, "CSS On/Off",                  CLI_SUPERVISOR},
    #endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

CLI_EXEC_T arMuteCmdTbl[] = {
    #ifdef CC_CLI
    {"MuteQurey",       "q",     _NptvMuteQuery,      NULL, "Main/PIP mute query",         CLI_GUEST},
    {"MuteOn",          "on",    _NptvMuteOn,         NULL, "Main/PIP mute",               CLI_GUEST},
    {"MuteOff",         "off",   _NptvMuteOff,        NULL, "Main/PIP unmute",             CLI_GUEST},
    {"MuteColor",       "color", _NptvMuteColor,      NULL, "Main/PIP mute color",         CLI_GUEST},
    {"ModuleEnable",    "ena",   _NptvMuteEnable,     NULL, "Main/PIP module enable",      CLI_GUEST},
    {"ModuleDisable",   "dis",   _NptvMuteDisable,    NULL, "Main/PIP module disable",     CLI_GUEST},
    {"ModuleForceAll",  "all",   _NptvMuteForceOnOff, NULL, "Main/PIP mute validate",      CLI_GUEST},
    #if CC_BOOT_MUTE_LOG
    {"MuteBootLog",     "boot",  _NptvMuteBootLog,    NULL, "Boot-up mute log",            CLI_GUEST},
    CLIMOD_DEBUG_CLIENTRY(MUTE),
    #endif
    #endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for Nptv command entry
static CLI_EXEC_T arNptvCmdTbl[] = {
#ifdef CC_CLI
    {"Init",            "i",     _NptvInitCmd,        NULL, "Initialize Nptv",             CLI_SUPERVISOR},
    {"MainSrc",         "ms",    _NptvMainSrcSelCmd,  NULL, "Main Source Select",          CLI_SUPERVISOR},
    {"PipSrc",          "ps",    _NptvPipSrcSelCmd,   NULL, "Pip Source Select",           CLI_SUPERVISOR},
    {"FwOnoff",         "fw",    _NptvFwOnOff,        NULL, "Nptv fimware on/off",         CLI_GUEST},
    {"FixColorSpace",   "fixcs", _NptvFixColorSpace,  NULL, "Fix color space",             CLI_SUPERVISOR},
    {"SetScanMode",     "scan",  _NptvSetScanModeCmd, NULL, "Set Scan Mode",               CLI_SUPERVISOR},
    {"SetDemoMode",     "demo",  _NptvDemoModeCmd,    NULL, "Set Demo Mode",               CLI_GUEST},
    #ifdef CC_DRV_SUPPORT_SMICRO
    {"SMicro",          "sm",    NULL, arSMicroCmdTbl,      "SMicro",                      CLI_GUEST},
    #endif
    {"Video",           "vdo",   NULL, arVdoCmdTbl,         "Vidoe Command",               CLI_GUEST},
    #ifdef CC_MT5398
    {"CSS",             "css",   NULL, arCssCmdTbl,         "CSS Command",                 CLI_SUPERVISOR},
    #endif
    {"TDTV",            "3d",    NULL, arTDTVCmdTbl,        "TDTV",                        CLI_GUEST},
    {"vrm",             "vrm",   NULL, arVrmCmdTbl,         "VRM Command",                 CLI_GUEST},        
    {"Scpos",           "scl",   NULL, arScposCmdTbl,       "Scpos command",               CLI_GUEST},
    #ifdef SCPIP_SUPPORT_GFX_DUMP
    {"ScpipGfx",        "gfx",   NULL, arScpipGfxDumpCmdTbl,"Scpip graphic dump command",  CLI_GUEST},
    #endif
    {"Decoder",         "dec",   NULL, arDecCmdTbl,         "Decoder Command",             CLI_GUEST},
    {"HDMI",            "hdmi",  NULL, arHDMICmdTbl,        "HDMI Command",                CLI_GUEST},
    {"HVPos",           "hv",    NULL, arHVCmdTbl,          "H/V position Command",        CLI_SUPERVISOR},
    {"DVI",             "dvi",   NULL, arDVICmdTbl,         "DVI Command",                 CLI_GUEST},
    {"Ycproc",          "ycproc",NULL, arYcprocCmdTbl,      "YCPROC Command",              CLI_GUEST},
    {"MDDi",            "di",    NULL, arYcprocDiTbl,       "MDDi Command",                CLI_GUEST},
    #ifdef CC_MT5398
    {"MJC",             "mjc",   NULL, arMjcCmdTbl,         "MJC Command",                 CLI_GUEST},
    #endif
    {"PreProc",         "pre3d", NULL, arPre3DCmdTbl,       "PreProc 3D Command",          CLI_GUEST},
    {"vga",             "vga",   NULL, arVgaCmdTbl,         "VGA Command",                 CLI_GUEST},
    {"Teletext",        "ttx",   NULL, arTTXCmdTbl,         "Teletext Command",            CLI_SUPERVISOR},
    {"VBI",             "vbi",   NULL, arVbiCmdTbl,         "VBI Command",                 CLI_GUEST},
    {"TVD",             "tvd",   NULL, arTvdCmdTbl,         "TVD Command",                 CLI_GUEST},
    {"TDC",             "tdc",   NULL, arTdcCmdTbl,         "TDC Command",                 CLI_GUEST},
    {"NR",              "nr",    NULL, arNRCmdTbl,          "NR Command",                  CLI_GUEST},
    {"SCART",           "scart", NULL, arScartCmdTbl,       "SCART Command",               CLI_GUEST},
    {"Mute",            "mute",  NULL, arMuteCmdTbl,        "Mute Command",                CLI_GUEST},
    {"PatternGen",      "pt",    NULL, arPatgenCmdTbl,      "Video pattern Command",       CLI_GUEST},
    {"Bypass",          "byp",   NULL, arBypassCmdTbl,      "Video module bypass Command", CLI_GUEST},
    {"srm",             "srm",   NULL, arSrmCmdTbl,         "Video srm Command",           CLI_GUEST},
    CLIMOD_DEBUG_CLIENTRY(NPTV),
#endif
    {"VFE",             "vfe",   NULL, arVfeCmdTbl,         "VFE Command",                 CLI_SUPERVISOR},
    
	{"DDCCI", 		  "ddcci",	 NULL, arDdcCiCmdTbl, 		"DDCCI Command",			   CLI_SUPERVISOR},
	
    #ifdef CC_CDC_SUPPORT
    {"DramStatus",      "ds",    _NptvDramStatusChk,  NULL, "Dram Status",                 CLI_GUEST},
    {"dram_dump",       "dd",    _NptvDevDumpCmd,     NULL, "NPTV dram dump",              CLI_GUEST},
    {"scaler_dd",   "scaler_dd", _NptvDevVideoDumpCmd,NULL, "NPTV scaler video dram dump", CLI_GUEST},
    #endif
    {"fw_upgrade", "fw_upgrade", _NptvFWUpgradeCmd,   NULL, "Firmware Upgrade",            CLI_SUPERVISOR},  
    {"status",          "ss",    _NptvStatusCmd,      NULL, "NPTV Status",                 CLI_SUPERVISOR}, 
#ifdef CC_SUPPORT_4K2K    
    {"4k2k", 		  "4k2k",	 NULL, ar4k2kCmdTbl, 		"4k2k Command",			   CLI_GUEST},
#endif    
    #ifndef NDEBUG
        #ifdef CC_FLIP_MIRROR_SUPPORT
    {"flip_mirror",     "fm",    _NptvSetFlipMirror,  NULL, "Set Flip Mirror",             CLI_SUPERVISOR},
    #endif
    #ifndef __KERNEL__
    //{"mw_dbg",         "mw_dbg", _NptvMwDebug,        NULL, "Set Mw if debug",             CLI_SUPERVISOR},
        #endif
    #endif // NDEBUG

    #if CC_MEASURE_SYS_TIME
    {"time_measure_en",         "tm_en", _NptvTimeMeasureEn,        NULL, "Enable Time measure",             CLI_GUEST},
    #endif    
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for top level nptv cli interface
CLI_MAIN_COMMAND_ITEM(Nptv)
{
	"nptv",
	"n",
	NULL,
	arNptvCmdTbl,
	"Nptv command",
	CLI_GUEST
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

extern BOOL VdoMLInit(void);

static INT32 _NptvInitCmd (INT32 i4Argc, const CHAR **szArgv)
{
  UNUSED(i4Argc);
  UNUSED(szArgv);

  VdoMLInit();

  Printf("Nptv Initialized!");

  return 0;
}


struct VideoInputNameMap {
	char * shortname;
	UINT8 vss_id;
};
static  struct VideoInputNameMap  cli_vdoinputmap[]=
{
	{ "tv",SV_VS_TUNER1 },
	{ "atv1",SV_VS_ATV1},
	{ "atd1",SV_VS_ATD1},
	{ "av1",SV_VS_CVBS1 },
	{ "av2",SV_VS_CVBS2 },
	{ "av3",SV_VS_CVBS3 },
	{ "av4",SV_VS_CVBS4 },
	{ "sv1",SV_VS_S1 },
	{ "sv2",SV_VS_S2 },
	{ "sv3",SV_VS_S3},
	{ "dvd",SV_VS_YPbPr3}, /* alias to SV_VS_YPbPr3 */
	{ "ypbpr1",SV_VS_YPbPr1},
	{ "ypbpr2",SV_VS_YPbPr2},
	{ "ypbpr3",SV_VS_YPbPr3},	/*DVD */
	{ "ypbpr4",SV_VS_YPbPr4},	/*vga componet */
	{ "vgacomp",SV_VS_VGACOMP1},
	{ "vga",SV_VS_VGA},
	{ "dtd1",SV_VS_DTD1},
	{ "dtv1",SV_VS_DT1},
	{ "dtv2",SV_VS_DT2},
	{ "hdmi1",SV_VS_HDMI1},
	{ "hdmi2",SV_VS_HDMI2},
	{ "hdmi3",SV_VS_HDMI3},
	{ "hdmi4",SV_VS_HDMI4},
	{ "scart0",SV_VS_SCART},
	{ "scart1",SV_VS_SCART1},
	{ "scart2",SV_VS_SCART2},
#ifdef CC_OSD_ENCODE
	{ "osdencode1",SV_VS_OSDENCODE1},	
	{ "ostgenc"   ,SV_VS_OSTGENC},	
#endif
	{ "off",SV_VS_MAX}
};

#define VDOSHORTNAME_NR (sizeof(cli_vdoinputmap)/sizeof(struct VideoInputNameMap))


static INT32 _NptvSrcSel(UINT8 path, INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 i;
	UINT8 u1Ret = 0;
	char * pathname;
	UINT8 srcnew;

	if(!path)
	{
		pathname="main";
		srcnew=_bSrcMainNew;
	}
	else
	{
		pathname="pip";
		srcnew=_bSrcSubNew;
		path=1;
	}

	//Printf("%d Arguments", i4Argc);
	if(i4Argc <= 2)
	{
		if(i4Argc==1)
		{
			for (i=0;i<VDOSHORTNAME_NR;i++)
			{
				if( srcnew == cli_vdoinputmap[i].vss_id)
					break;
			}
			if(i >= VDOSHORTNAME_NR)
            {
				i = VDOSHORTNAME_NR - 1;
            }
			Printf("Current %s input: [%s]\n", pathname, cli_vdoinputmap[i].shortname);
			return 0;
		}

		for (i=0;i<VDOSHORTNAME_NR;i++)
		{
			if(x_strcmp(szArgv[1], cli_vdoinputmap[i].shortname) == 0)
			{
				u1Ret = bApiVideoSetVideoSrc(path, cli_vdoinputmap[i].vss_id);
				break;
			}
		}
		if(u1Ret)
		{
			Printf("Successful to select %s to [%s]\n", pathname, szArgv[1]);
			return 0;
		}
		else
		{
			Printf("Fail to select %s to [%s]\n", pathname, szArgv[1]);
		}
	}

	Printf("Usage: %s <", szArgv[0]);
	for (i=0;i<VDOSHORTNAME_NR;i++)
	{
		Printf("%s ",cli_vdoinputmap[i].shortname);
		if((i&0xf)==0xf) Printf("\n\t");
	}
		Printf(">\n");
	return 0;

}


static INT32 _NptvMainSrcSelCmd(INT32 i4Argc, const CHAR **szArgv)
{
	return _NptvSrcSel(0,i4Argc,szArgv);
}

static INT32 _NptvPipSrcSelCmd(INT32 i4Argc, const CHAR **szArgv)
{
	return _NptvSrcSel(1,i4Argc,szArgv);
}

static INT32 _NptvVideoQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;

    if (i4Argc != 2)
    {
        Printf("Arg: VdpId\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        return 1;
    }

    u4VdpId = StrToInt(szArgv[1]) ? SV_VP_PIP : SV_VP_MAIN;

    vDrvVideoQueryInputTimingInfo(u4VdpId);    

    return 0;
}

static INT32 _NptvVideoDelay(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("bDrvVideoGetFrameDelay: %4d(ms)\n", bDrvVideoGetFrameDelay(SV_VP_MAIN));
    Printf("VDP_GetDelayTime:       %4d(90000 = 1s)\n", VDP_GetDelayTime());

    return 0;
}

static INT32 _NptvVideoModChg(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;

    if (i4Argc != 3)
    {
        Printf("Arg: VdpId ModuleId\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        Printf("<ModuleId>\n");
        Printf("0: All\n");
        Printf("1: PE\n");
        #ifdef CC_MT5398
        Printf("10: MJC\n");
        #endif
        return 1;
    }

    u4VdpId = StrToInt(szArgv[1]) ? SV_VP_PIP : SV_VP_MAIN;

    switch (StrToInt(szArgv[2]))
    {
        case 0:
            vVdoModeChgDone(u4VdpId);
            break;
        case 1:
            vVdoPEModeChgDone(u4VdpId);
            break;
        #ifdef CC_MT5398
        case 10:
            vDrvMJCModeChangeDone(u4VdpId);
        #endif
        default:
            break;
    }

    return 0;
}

static INT32 _NptvVideoSWRegRead(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Len;
    UINT32 u4Idx;

    if (i4Argc < 2)
    {
        Printf("Arg: Addr Len\n");
        Printf("<Addr>\n");
        Printf("0 ~ 0x10000\n");
        Printf("<Len>\n");
        Printf("0 ~ 0x1000\n");
        return 1;
    }

    u4Addr = StrToInt(szArgv[1]);
    u4Len  = StrToInt(szArgv[2]);

    if (u4Len > 0x1000)
    {
        u4Len = 0x1000;
    }

    // DW alignment
    u4Addr &= 0xFFFFFFFC;

    for (u4Idx = 0; u4Idx < u4Len; u4Idx += 16)
    {
        if ((u4Addr + u4Idx + 16) >= 0x10000)
        {
            break;
        }
        Printf("0x%04X | %08X %08X %08X %08X\n",
            (u4Addr + u4Idx),
            u4IO32Read4B(u4SWReg(u4Addr + u4Idx +  0)),
            u4IO32Read4B(u4SWReg(u4Addr + u4Idx +  4)),
            u4IO32Read4B(u4SWReg(u4Addr + u4Idx +  8)),
            u4IO32Read4B(u4SWReg(u4Addr + u4Idx + 12)));
    }

    return 0;
}

static INT32 _NptvVideoSWRegWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Value;

    if (i4Argc < 2)
    {
        Printf("Arg: Addr Value\n");
        Printf("<Addr>\n");
        Printf("0 ~ 0x10000\n");
        Printf("<Value>\n");
        Printf("0 ~ 0xFFFFFFFF\n");
        return 0;
    }

    u4Addr  = StrToInt(szArgv[1]);
    u4Value = StrToInt(szArgv[2]);

    // DW alignment
    u4Addr &= 0xFFFFFFFC;

    vIO32Write4B(u4SWReg(u4Addr), u4Value);
    Printf("0x%04X | %08X\n", u4Addr, u4Value);

    return 0;
}

#ifdef CC_MT5398
static INT32 _NptvCSSInit(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvCSSInitialize();

    return 0;
}

static INT32 _NptvCSSCompensate(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4XOffset, i4YOffset;

    if (szArgv[1][0] == '-')
    {
        i4XOffset = 0 - StrToInt(&szArgv[1][1]);
    }
    else
    {
        i4XOffset = StrToInt(&szArgv[1][0]);
    }

    if (szArgv[2][0] == '-')
    {
        i4YOffset = 0 - StrToInt(&szArgv[2][1]);
    }
    else
    {
        i4YOffset = StrToInt(&szArgv[2][0]);
    }

    vDrvCSSCompensate(i4XOffset, i4YOffset);

    return 0;
}

static INT32 _NptvCSSOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvCSSOnOff((StrToInt(szArgv[1])) ? SV_ON : SV_OFF);

    return 0;
}
#endif

static INT32 _NptvMuteOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;

    if (i4Argc != 2)
    {
        Printf("Arg: VdpId\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        return 1;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);

    vDrvVideoMuteOnOff(u4VdpId, SV_ON);
	return 0;
}

static INT32 _NptvMuteOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;

    if (i4Argc != 2)
    {
        Printf("Arg: VdpId\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        return 1;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);

    vDrvVideoMuteOnOff(u4VdpId, SV_OFF);
	return 0;
}

static INT32 _NptvMuteQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: VdpId\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        return 1;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);

	if ((u4VdpId != SV_VP_MAIN) && (u4VdpId != SV_VP_PIP))
	{
		Printf("Invalid VdpId\n");
		return 1;
	}

    vDrvDumpVideoMuteModule(u4VdpId, TRUE);
    return 0;
}

static INT32 _NptvMuteColor(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Red, u4Green, u4Blue;

    if ((i4Argc < 5) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        Printf("Arg: VdpId Red Green Blue\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        Printf("<Red><Green><Blue>\n");
        Printf("0 ~ 255: Level\n");
        return 1;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4Red   = (UINT32)StrToInt(szArgv[2]);
    u4Green = (UINT32)StrToInt(szArgv[3]);
    u4Blue  = (UINT32)StrToInt(szArgv[4]);

    if ((u4VdpId != SV_VP_MAIN) && (u4VdpId != SV_VP_PIP))
    {
        Printf("Invalid VdpId\n");
        return 1;
    }

    if ((u4Red > 255) || (u4Green > 255) || (u4Blue > 255))
    {
        Printf("Invalid Color Level\n");
        return 1;
    }

    vDrvVideoMuteColor(u4VdpId, (u4Red << 16) | (u4Green << 8) | u4Blue);
    return 0;
}

static INT32 _NptvMuteEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Module;

    if (i4Argc != 3)
    {
        Printf("Arg: VdpId Module\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        Printf("<Module>\n");
        vDrvDumpVideoMuteModule(SV_VP_MAIN, TRUE);
        vDrvDumpVideoMuteModule(SV_VP_PIP, TRUE);
        return 1;
    }
    
    u4VdpId  = (UCHAR) StrToInt(szArgv[1]);
    u4Module = (UCHAR) StrToInt(szArgv[2]);

    vDrvVideoMuteValid(u4VdpId, u4Module, SV_ON);
	return 0;
}

static INT32 _NptvMuteDisable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Module;

    if (i4Argc != 3)
    {
        Printf("Arg: VdpId Module\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        Printf("<Module>\n");
        Printf("0 ~ %d\n", MUTE_MODULE_MAX);
        return 1;
    }
    
    u4VdpId  = (UCHAR) StrToInt(szArgv[1]);
    u4Module = (UCHAR) StrToInt(szArgv[2]);

    vDrvVideoMuteValid(u4VdpId, u4Module, SV_OFF);
	return 0;
}

static INT32 _NptvMuteForceOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT8  bUnMute;

    if (i4Argc != 3)
    {
        Printf("Arg: VdpId OnOff\n");
        Printf("<VdpId>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        Printf("<OnOff>\n");
        Printf("0: Off\n");
        Printf("1: On\n");
        return 1;
    }

    u4VdpId = StrToInt(szArgv[1]);
    bUnMute = (StrToInt(szArgv[2]))? SV_FALSE : SV_TRUE;

    _vDrvVideoForceUnMute(u4VdpId, bUnMute);
    return 0;
}

#if CC_BOOT_MUTE_LOG
static INT32 _NptvMuteBootLog(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4En;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )
	{
		Printf("Arg: u4Enable\n");
		return 0;
	}
	u4En = (UINT32) StrToInt(szArgv[1]);
	u4En = (u4En>0) ? 1 : 0;

	if(u4En)
	{
	    fgApiEepromWriteByte(EEP_BOOT_MUTE_LOG, 0xaa);  // magic word : 0xaa
	    Printf("Success : Enable mute log when system boot up\n");
	}
	else
	{
	    fgApiEepromWriteByte(EEP_BOOT_MUTE_LOG, 0);
	    Printf("Success : Disable mute log when system boot up\n");
	}
	return 0;
}
#endif


#define SV_LEFT     1
#define SV_RIGHT    0
static INT32 _NptvDemoModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    EXTERN void vApiVideoDemoMode(UINT8 bmode, Region DemoRec, UINT8 bDEMOModules);
    Region Test;
    UINT8 bModules;   
    if(i4Argc != 6)
	{
        Printf("Usage: %s <mode> <region HStart> <region HEnd> <region VStart> <region VEnd> <Demo Modules>\n", szArgv[0]);
		return (0);
	}
    Test.u1OnOff = SV_ON;
    Test.wHEnd = (UINT16) StrToInt(szArgv[3]);
    Test.wHStart = (UINT16) StrToInt(szArgv[2]);
    Test.wVEnd = (UINT16) StrToInt(szArgv[5]);
    Test.wVStart = (UINT16) StrToInt(szArgv[4]);    
    bModules = (UINT8) StrToInt(szArgv[6]);  
    vApiVideoDemoMode((UINT16)StrToInt(szArgv[1]), Test, bModules);
	return 0;
}

static INT32 _NptvFixColorSpace(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc != 3)
	{
	  Printf("Usage: %s <main/pip 0/1> <color mode >\n", szArgv[0]);
	  Printf("mode: (0)auto (1)RGB_444 (2)RGB_422(3)YCbCr(4)YCBCRtoRGB(5)RGBtoYCBCR\n"
	           "     (17)RGB 444 once (18)RGB 422 once (19)YCbCr(20)YCBCRtoRGB once(21)RGBtoYCBCRonce\n");
	}
	else
	{
		vApiVideoSetFixColorSpaceMode((SV_FIXCOLORSPACE_MODE)StrToInt(szArgv[1]),(SV_FIXCOLORSPACE_MODE)StrToInt(szArgv[2]));
		Printf("fix(force) %s to colorspace mode to %d\n", (UINT16)StrToInt(szArgv[1]),(UINT16)StrToInt(szArgv[2]));
	}
	Printf("Main fix color mode (0x%02x) Sub fix color mode (0x%02x)\n",bApiVideoGetFixColorSpaceMode(SV_VP_MAIN),bApiVideoGetFixColorSpaceMode(SV_VP_PIP));	
	return (0);
}

static INT32 _NptvSetScanModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path = 0;
    UINT32 u4Mode = 0;
    
    if (i4Argc < 3)
    {
        Printf("Usage: %s <main/pip 0/1> <scan mode >\n", szArgv[0]);    
        if (i4Argc > 1)
        {
            Printf("Current Mode: %d\n", u4ApiVideoGetScanMode((UINT32)StrToInt(szArgv[1])));            
        }
        return 0;
    }
    u4Path = (UINT32)StrToInt(szArgv[1]);
    u4Mode = (UINT32)StrToInt(szArgv[2]);

    vApiVideoSetScanMode(u4Path, u4Mode);    

    return 0;
}

extern UINT32 _u4BypassNptvMloop ;
static INT32 _NptvFwOnOff (INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc == 2)
	{
		_u4BypassNptvMloop = StrToInt(szArgv[1]);
	}
    else
    {
        Printf("BYPASS_NPTV_All         (1 << 0)  0x1\n");
        Printf("BYPASS_NPTV_NR          (1 << 1)  0x2\n");
        Printf("BYPASS_NPTV_HDMI        (1 << 2)  0x4\n");
        Printf("BYPASS_NPTV_SCPOS       (1 << 3)  0x8\n");
        Printf("BYPASS_NPTV_PSCAN       (1 << 4)  0x10\n");
        Printf("BYPASS_NPTV_TVD         (1 << 5)  0x20\n");
        Printf("BYPASS_NPTV_DDDS        (1 << 6)  0x40\n");
        Printf("BYPASS_NPTV_CEC         (1 << 7)  0x80\n");
        Printf("BYPASS_NPTV_TVE         (1 << 8)  0x100\n");
        Printf("BYPASS_NPTV_SCART       (1 << 9)  0x200\n");
        Printf("BYPASS_OUTPUT_ISR       (1 << 10) 0x400\n");
        Printf("BYPASS_INPUT_ISR        (1 << 11) 0x800\n");
        Printf("BYPASS_COLOR_PROC       (1 << 12) 0x1000\n");
        Printf("BYPASS_NSD_PROC	        (1 << 13) 0x2000\n");
        Printf("BYPASS_NPTV_MJC         (1 << 14) 0x4000\n");
        Printf("BYPASS_NPTV_TDTV        (1 << 15) 0x8000\n");
        Printf("BYPASS_VDO_MAIN_STATE   (1 << 16) 0x10000\n");
        Printf("BYPASS_VDO_PIP_STATE    (1 << 17) 0x20000\n");
        Printf("BYPASS_SCAN_CHK         (1 << 18) 0x40000\n");
        Printf("BYPASS_NPTV_YCPROC      (1 << 19) 0x80000\n");
        Printf("BYPASS_NPTV_FRAME_TRACK (1 << 20) 0x100000\n");
        Printf("BYPASS_NPTV_LCM         (1 << 21) 0x200000\n");
        Printf("BYPASS_SRC_DETECT       (1 << 22) 0x400000\n");    
        Printf("BYPASS_NPTV_OD          (1 << 23) 0x800000\n");
        Printf("BYPASS_NPTV_LCDIM       (1 << 24) 0x1000000\n");
        Printf("BYPASS_NPTV_PDS         (1 << 25) 0x2000000\n");
        Printf("BYPASS_NPTV_REGEN       (1 << 26) 0x4000000\n");
        Printf("BYPASS_NPTV_VRM         (1 << 27) 0x8000000\n");        
        Printf("BYPASS_NPTV_4K2K        (1 << 28) 0x10000000\n");            
        Printf("\nBypassNptvMloop: 0x%X\n", _u4BypassNptvMloop);
    }

    return 0;
}

static INT32 _NptvStatusCmd(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef COUNTRY_FROM_EEPROM 
	Printf("EEPROM_COUNTRY_OFFSET1 0x%x\n", DRVCUST_OptGet(eCountryEEPROMOffset1));
	Printf("EEPROM_COUNTRY_OFFSET2 0x%x\n", DRVCUST_OptGet(eCountryEEPROMOffset2));
	Printf("EEPROM_COUNTRY_OFFSET3 0x%x\n", DRVCUST_OptGet(eCountryEEPROMOffset3));
#endif
	Printf("nptv size =%d\n", DRVCUST_OptGet(eEepromNptvDriverSize));
	Printf("nptv start =%d\n", DRVCUST_OptGet(eEepromNptvDriverOffset));
	Printf("EEP_ADC_START=%d\n", EEP_ADC_START);
	Printf("EEP_VIDEO_AUTO_COLOR_START=%d\n", EEP_VIDEO_AUTO_COLOR_START);
	Printf("EEP_VGA_USR_START=%d\n", EEP_VGA_USR_START);
	Printf("EEP_CEC_LOG_ADDR=%d\n", EEP_CEC_LOG_ADDR);
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
	Printf("EEP_VGA_AMBIGUOUS_DEFAULT_START=%d\n", EEP_VGA_AMBIGUOUS_DEFAULT_START);
#endif
	Printf("EEP_GEN_SET_END=%d\n", EEP_GEN_SET_END);
#if 1
	Printf("cec menu lang=%d\n", DRVCUST_InitGet(eEepromCECMenuLanguageOffset));
	Printf("cec enable =%d\n", DRVCUST_InitGet(eEepromCECEnableOffset));
	Printf("cec touch =%d\n", DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset));
#endif
	return 0;
}

#ifndef NDEBUG 
#ifdef CC_FLIP_MIRROR_SUPPORT
static INT32 _NptvSetFlipMirror(INT32 i4Argc, const CHAR **szArgv)
{
		DTVCFG_T rDtvCfg;

		if(i4Argc==3)
		{
			if(EEPDTV_GetCfg(&rDtvCfg))
			{
				Printf(" Read Dtv config fail\n");
				return 0 ;
			}

			if(StrToInt(szArgv[1]) == 0)
			{
				if(StrToInt(szArgv[2]))
				{
					rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_FLIP_ON);	
				}
				else
				{
					rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_FLIP_ON);	
				}
				if(EEPDTV_SetCfg(&rDtvCfg))
				{
					Printf("set Dtv config fail\n");
				}
			}
			else if(StrToInt(szArgv[1]) == 1)
			{
				if(StrToInt(szArgv[2]))
				{
					rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_MIRROR_ON);	
				}
				else
				{
					rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_MIRROR_ON);	
				}

				if(EEPDTV_SetCfg(&rDtvCfg))
				{
					Printf("set Dtv config fail\n");
				}
			}
			else
			{
				Printf("flip_mirror flip/mirror(0/1)  enable/disable(1/0)\n");
			}
		}
		else
		{
			Printf("flip_mirror flip/mirror(0/1)  enable/disable(1/0)\n");
		}

    return 0;
}
#endif
#ifndef __KERNEL__
#if 0
extern UINT32 _u4MwIfDbgFlags ;
static INT32 _NptvMwDebug(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc==2)
    {
       _u4MwIfDbgFlags = StrToInt(szArgv[1]) ;
        
    }
    else
    {
		Printf("Current Val:0x%x\n", _u4MwIfDbgFlags);
        Printf("VDEC : 0x8\n");
		Printf("VDP : 0x10\n");
		Printf("SRCR : 0x20\n");
		Printf("SRCR : 0x40\n");
		Printf("vdp set: 0x80\n");
		Printf("overscan : 0x100\n");
    }
    
    return 0;
}
#endif
#endif //#ifndef CC_DRIVER_PROGRAM
#endif


#if CC_MEASURE_SYS_TIME
#include "util.h"
static INT32 _NptvTimeMeasureEn(INT32 i4Argc, const CHAR **szArgv)
{

    if(i4Argc==2)
    {
       _u4SysMeasueTimeEn = StrToInt(szArgv[1]) ;
        
    }
    else
    {
         UINT32 i;
        Printf("Current Val:0x%x\n", _u4SysMeasueTimeEn);
        for(i=0 ; i < SV_MAX_SYS_MODULE ; i++)
        {
            Printf("[%s]: 0x%x\n", _aszTimeModuleStr[i], (1<< i));            
        }
    }
    return 0 ;
}

#endif


//============================================================================
// NPTV Tool Command
//============================================================================
#define USB_EEPROM_MAGIC 0x73

#ifdef CC_CDC_SUPPORT
static INT32 _NptvDramStatusChk(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {     	
    	Printf("Usage: %s OnOff\n", szArgv[0]);
		Printf("<OnOff>\n");
		Printf("0: Disable\n");
		Printf("1: Enable\n");
        return 0;
    }

    if (StrToInt(szArgv[1]))
    {
        vDrvVideoDramStatusRst();
        Printf("\n");
    }

    fgDramStatusChkOnOff = StrToInt(szArgv[1]);
    return 0;
}

static INT32 _NptvDevDumpCmd(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc < 2)
    {     	
    	Printf("n.dd [on/off]\n");
    	if(bApiEepromReadByte(EEP_MUSB_ENABLE) == USB_EEPROM_MAGIC)
    	{
    		Printf("USB is in device mode\n");
    	}else
    	{
    		Printf("USB is in host mode\n");
    	}    	
		return 0;
    }else
    {
		if(x_strcmp(szArgv[1], "off") == 0)
		{

			vSetUSBDump(SV_FALSE);
			fgApiEepromWriteByte(EEP_MUSB_ENABLE, 0xFF);	
		}else
		{
			fgApiEepromWriteByte(EEP_MUSB_ENABLE, USB_EEPROM_MAGIC);			
			vSetUSBDump(SV_TRUE);				
		}	
    } 
    return 0;
}

static INT32 _NptvDevVideoDumpCmd(INT32 i4Argc, const CHAR **szArgv)
{	
    if(i4Argc < 2)
    {
    	Printf("scaler_dd on|off|start [#number] [half=0/1]\n");
    	Printf("scaler_dd add address size\n");    	
    }else
    {
		NptvScalerVideoDump(i4Argc, szArgv);  
    }

    return 0;
}
#endif

static INT32 _NptvFWUpgradeCmd(INT32 i4Argc, const CHAR **szArgv)
{
	static UINT8 *pbData;
	static UINT8 *pbDataAlign;	
	static UINT8 u1FlashType=0;
	static UINT32 u4PartID=0xFF;  // 0xFF mean auto-detect
	static UINT32 u4Offset=0;	
	static UINT32 u4Size=0;	
	static UINT32 u4BlkSize;
	UINT8 fgBoost=0;	
	INT8 iRetVal;
	
    if(i4Argc < 2)
    {
    	Printf("fw_upgrade [alloc/free/erase/upgrade/backup/checksum]\n");
		return 0;
    }else
    {
		if(x_strcmp(szArgv[1], "alloc") == 0)
		{

			if(i4Argc < 6)
			{
				Printf("fw_upgrade alloc [auto=0|nor=1|nand=2] [NAND_Part=0xFF(Auto)] [offset] [size]");
			}
			else
			{
				UINT32 AllocAddr;
			
				if(pbData)
				{			
					x_mem_free(pbData);
					pbData = NULL;
				}
				
				u1FlashType = StrToInt(szArgv[2]);
				u4PartID = StrToInt(szArgv[3]);
				u4Offset =  StrToInt(szArgv[4]);				
				u4Size =  StrToInt(szArgv[5]);							
	    		pbData = x_mem_alloc(u4Size+0x10);

				if(u1FlashType==0)
				{
					#if defined(CC_NAND_ENABLE) 		
						u1FlashType = 2;					
					#else
						u1FlashType = 1;
					#endif
				}	

				if(u1FlashType==2 && u4PartID==0xFF)
				{
					// Get NAND partition ID	
					DRVCUST_OptQuery(eNANDFlashPartIdSysImgA, (UINT32*)&u4PartID);		
							
				}
				u4BlkSize = FlashGetBlkSize(u1FlashType);	
				
				// 16 bytes alignment.
				AllocAddr = (UINT32)pbData;				
				pbDataAlign = (UINT8*)((AllocAddr % 16 != 0) ? (AllocAddr/16+1)*16 : AllocAddr);
				
	
				// disable ISR
				vIO32Write4B((VDOIN_BASE+0x240c), 0xFFFFFFFF);
				// disable watchdog
				vIO32Write4B((VDOIN_BASE+0x8000), 0x0);
			
    			Printf("Allocate Memory Address=0x%X, Size=0x%08X, BlkSize=0x%08X\n", pbDataAlign, u4Size, u4BlkSize);				
			}
		}
		else if(x_strcmp(szArgv[1], "free") == 0)
		{	
			if(pbData)
			{
    			Printf("Free Memory Address=0x%X, Size=0x%08X\n", pbData, u4Size);				
				x_mem_free(pbData);
				
				pbData = NULL;
				pbDataAlign = NULL;	
				u1FlashType=0;
				u4PartID=0xFF;  
				fgBoost=0;	
				u4Offset=0;	
				u4Size=0;
				u4BlkSize=0;				
			}
		}
		else if(x_strcmp(szArgv[1], "erase") == 0)		
		{
			iRetVal = FlashErase(u1FlashType, u4PartID, u4Offset, u4Size);
			
			if(iRetVal != 0)
			{				
				Printf("Flash Erase Failed\n");
				return -1;
			}		
		}
		else if(x_strcmp(szArgv[1], "upgrade") == 0)
    	{
			if(i4Argc < 3)
			{
				Printf("fw_upgrade upgrade [boost=0/1]");
			}
			else
			{    	
				fgBoost = StrToInt(szArgv[2]);

				// Get CheckSum
				CalcCheckSum((UINT32)pbDataAlign, u4Size);

				iRetVal = FlashWrite(u1FlashType, u4PartID, u4Offset, (UINT32)pbDataAlign, u4Size, fgBoost);
				if(iRetVal != 0)
				{				
					Printf("Flash Write Failed\n");
					return -1;
				}			
			}
    	}  
		else if(x_strcmp(szArgv[1], "backup") == 0)
    	{	
			iRetVal = FlashRead(u1FlashType, u4PartID, u4Offset, (UINT32)pbDataAlign, u4Size);

			// Get CheckSum
			CalcCheckSum((UINT32)pbDataAlign, u4Size);
			
			if(iRetVal != 0)
			{				
				Printf("Flash Read Failed\n");
				return -1;
			}			
    	}   	
		else if(x_strcmp(szArgv[1], "checksum") == 0)
    	{	
			u4Size =  StrToInt(szArgv[2]);			
			CalcCheckSum((UINT32)pbDataAlign, u4Size);
    	}  		
    }				
	return 0;
}

#ifdef CC_SUPPORT_4K2K
#include "drv_display.h"
extern r4k2kHandler *psCur4k2kHandler;
static INT32 _Nptv4k2kQuery(INT32 i4Argc, const CHAR **szArgv)
{
	if(!psCur4k2kHandler)
	{
		Printf("[4k2k]none handler Init\n");
		return 0 ;
	}
	switch(psCur4k2kHandler->u1Tag)
	{
	case E_4K2K_CMI_RTK:
		Printf("[4k2k]CMI+RTK 4k2k FRC handler Init\n");
		break;	
	case E_4K2K_CMI_MST:
		if(psCur4k2kHandler->u13D2DTimingChg)
		{
			Printf("[4k2k]CMI+MST 4k2k FRC 120 handler Init\n");		
		}
		else
		{
			Printf("[4k2k]CMI+MST 4k2k FRC 120 handler Init\n");					
		}	
		break;
	case E_4K2K_CSOT_NTK:
		Printf("[4k2k]CSOT+NTK 4k2k FRC handler Init\n");
		break;
	default:
		Printf("[4k2k]none handler Init\n");
		break;
	}	
	Printf("i2d addr 0x%x\n", psCur4k2kHandler->u1I2cAddr);
	Printf("cmd len  %d\n", psCur4k2kHandler->u1CmdLen);
	Printf("u13D2DTimingChg  %d\n", psCur4k2kHandler->u13D2DTimingChg);
	Printf("mute time %d\n", psCur4k2kHandler->u2MuteTime);
    Printf("cmd delay %d\n", psCur4k2kHandler->u2CmdInterval);	
	return 0;	

}
static INT32 _Nptv4k2kSetAddr(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {     	
    	Printf("Usage: %s addr \n", szArgv[0]);
        return 0;
    }
	if(!psCur4k2kHandler)
	{
		Printf("[4k2k]none handler Init\n");
		return 0 ;
	}
    //if (StrToInt(szArgv[1]))
    {
        psCur4k2kHandler->u1I2cAddr = StrToInt(szArgv[1]) ;
        Printf("i2d addr 0x%x\n", psCur4k2kHandler->u1I2cAddr);
    }
	return 0;
}
static INT32 _Nptv4k2kSetDelay(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {     	
    	Printf("Usage: %s delay ms \n", szArgv[0]);
        return 0;
    }
	if(!psCur4k2kHandler)
	{
		Printf("[4k2k]none handler Init\n");
		return 0 ;
	}
    //if (StrToInt(szArgv[1]))
    {
        psCur4k2kHandler->u2CmdInterval = StrToInt(szArgv[1]) ;
        Printf("cmd delay %d\n", psCur4k2kHandler->u2CmdInterval);
    }
		return 0;
}

static INT32 _Nptv4k2kSetMuteTime(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {     	
    	Printf("Usage: %s mutetime ms\n", szArgv[0]);
        return 0;
    }
	if(!psCur4k2kHandler)
	{
		Printf("[4k2k]none handler Init\n");
		return 0 ;
	}
    //if (StrToInt(szArgv[1]))
    {
        psCur4k2kHandler->u2MuteTime= StrToInt(szArgv[1]) ;
        Printf("mute time %d\n", psCur4k2kHandler->u2MuteTime);
    }
	return 0;
}

#endif

//============================================================================
// End of NPTV Tool Command
//============================================================================


#endif /* CC_CLI */

