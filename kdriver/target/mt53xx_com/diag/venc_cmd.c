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
 * $RCSfile: venc_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_cmd.c
 *  This file contains implementation of CLI CMD for Video Encoder
 *
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#define DEFINE_IS_LOG    VENC_IsLog
#include "x_debug.h"

#include "venc_if.h"
#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_os.h"

#include "fbm_drvif.h"
#include "venc_util_drvif.h"
#include "venc_drvif.h"

#include "drv_di.h"
#include "drv_video.h"

#ifdef CC_SUPPORT_MUXER
#define VENC_TO_MUXER_IF
#endif
#ifdef VENC_TO_MUXER_IF
#include "muxer_if.h"
#endif

#include "vdec_drvif.h"
#ifdef USB_SUPPORT
#include "x_fm.h"
#endif


#include "vdo_misc.h"
#include "drv_vga.h"
#include "video_def.h"
#include "drv_hdmi.h"
#include "vdo_if.h"
#include "srm_drvif.h"
//#include "../5396_driver/venc/h264/venc_h264api.h"

#define VENC_LOCAL_DISC_ID_ENABLE   0

#if VENC_LOCAL_DISC_ID_ENABLE
#define VENC_LOCAL_DISC_ID  0x51
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#ifdef VENC_EMU

typedef struct CfgFile
{
    char InputFileName[512];
    char GoldenFileName[512];
    char MMRFileName[512];
    char CRCFileName[512];
    char OutputFileName[512]; // target bitrate/framerate
    char KeyFrameFileName[512];
    BOOL fgSaveOutputFile;
}CFGFILE;
typedef void (*PFNVEncCmdEmuItem)(UINT8 *, CHAR *);
typedef void (*PFNVEncPscan)(UINT32, UINT32);

#endif

typedef struct
{
  UINT32 u4Cmd;
  UINT32 u4CmdParam[12];
}ENCODER_TESTCMD,*PENCODER_TESTCMD;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

#ifdef VENC_EMU
extern VOID _VEncH264LoadEmu(VOID);
extern VOID *_VEncH264GetEmuItem(void);
#endif

extern void vDrvDISetVEncHandle(void *handle, void *ptFrmBuffMgr, void *ptResizeFrmBuffMgr, void *ptFilterPts);
#ifdef LINUX_TURNKEY_SOLUTION
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
extern INT32 _EncodeTestCmd(INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef INPUT_BUF_FROM_SCALER
extern void u1DrvScpipH264SetDramAddress(UINT32 u4BaseAddr, UINT32 u4YFrameSize);
#endif
//---------------------------------------------------------------------------
// Emulation code
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
#ifdef __MODEL_slt__
static INT32 _VEncCmdSlt(INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef INPUT_BUF_FROM_SCALER
static INT32 _VEncCmdTriggerScaler(INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32 _VEncCmdSetResolution(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdSetFrameRate(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdSetBitRate(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdQuery(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VEncCmdMultiScreenDemoRun(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdMultiScreenDemoStop(INT32 i4Argc, const CHAR ** szArgv);

#ifdef CC_ATV_PVR_SUPPORT
static INT32 _VEncCmdAtvPvrDemoRun(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdAtvPvrDemoStop(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _VEncCmdDumpBs(INT32 i4Argc, const CHAR ** szArgv);
#ifdef VDEC_VENC_PATH
static INT32 _VEncCmdVdecPathDemoRun(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdVdecPathDemoStop(INT32 i4Argc, const CHAR ** szArgv);
#endif

static VOID venc_demo_cb(VOID *pvArg, VENC_IN_PIC_T * ptIn, VENC_OUT_PIC_T * ptOut, UINT32 type);

#ifdef VENC_EMU
static INT32 _VEncCmdLoadEmu(INT32 i4Argc, const CHAR ** szArgv);
#endif

#if defined(LINUX_TURNKEY_SOLUTION) && !defined(CC_DRIVER_DEMO)
static INT32 _VEncCmdToggleMtalLog(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _VEncCmdGetThumbnail(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdToggleRc(INT32 i4Argc, const CHAR ** szArgv);


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(VENC)

// cmd table
CLI_EXEC_T _arVEncCmdMsTbl[] =
{
    {"query",          "q",    _VEncCmdQuery,                   NULL, "Query the info.of video encoder",  CLI_SUPERVISOR},
    {"run",            "r",    _VEncCmdMultiScreenDemoRun,      NULL, "Multiscreen interaction demo: Run",   CLI_SUPERVISOR},
    {"stop",           "s",    _VEncCmdMultiScreenDemoStop,     NULL, "Multiscreen interaction demo: Stop",  CLI_SUPERVISOR},
    {"dump",           "bs",   _VEncCmdDumpBs,                  NULL, "Dump Bitstream",                      CLI_SUPERVISOR},
    {"thumbnail",      "tn",   _VEncCmdGetThumbnail,            NULL, "Get thumbnail test",                  CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(VENC),     // for LOG use
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_ATV_PVR_SUPPORT
CLI_EXEC_T _arVEncCmdAtvTbl[] =
{
    {"query",          "q",    _VEncCmdQuery,                   NULL, "Query the info.of video encoder",  CLI_SUPERVISOR},
    {"run",            "r",    _VEncCmdAtvPvrDemoRun,           NULL, "ATV PVR demo: Run",                   CLI_SUPERVISOR},
    {"stop",           "s",    _VEncCmdAtvPvrDemoStop,          NULL, "ATV PVR demo: Stop",                  CLI_SUPERVISOR},
    {"dump",           "bs",   _VEncCmdDumpBs,                  NULL, "Dump Bitstream",                      CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(VENC),     // for LOG use
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef VDEC_VENC_PATH
CLI_EXEC_T _arVEncCmdVdecTbl[] =
{
    {"query",          "q",    _VEncCmdQuery,                   NULL, "Query the info.of video encoder",  CLI_SUPERVISOR},
    {"run",            "r",    _VEncCmdVdecPathDemoRun,         NULL, "Vdec Path demo: Run",     CLI_SUPERVISOR},
    {"stop",           "s",    _VEncCmdVdecPathDemoStop,        NULL, "Vdec Path demo: Stop",    CLI_SUPERVISOR},
    {"setresolution",  "sl",   _VEncCmdSetResolution,           NULL, "Set video resolution",    CLI_SUPERVISOR},
    {"dump",           "bs",   _VEncCmdDumpBs,                  NULL, "Dump Bitstream",          CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(VENC),     // for LOG use
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

CLI_EXEC_T _arVEncCmdTbl[] =
{
    {"query",          "q",    _VEncCmdQuery,                   NULL, "Query the info.of video encoder",  CLI_SUPERVISOR},
    {"setresolution",  "sl",   _VEncCmdSetResolution,           NULL, "Set video resolution",             CLI_SUPERVISOR},
    {"setframerate",   "sfr",  _VEncCmdSetFrameRate,            NULL, "Set video frame rate",             CLI_SUPERVISOR},
    {"setbitrate",     "sbr",  _VEncCmdSetBitRate,              NULL, "Set video bit rate",               CLI_SUPERVISOR},
    {"dump",           "bs",   _VEncCmdDumpBs,                  NULL, "Dump Bitstream",                   CLI_SUPERVISOR},
#if defined(LINUX_TURNKEY_SOLUTION) && !defined(CC_DRIVER_DEMO)
    {"mtal",           NULL,   _VEncCmdToggleMtalLog,           NULL, "Toggle MTAL log",                  CLI_SUPERVISOR},
#endif
    {"rc",             NULL,   _VEncCmdToggleRc,                NULL, "Toggle Rate control",              CLI_SUPERVISOR},
    {"multiscreen",    "ms",   NULL,                 _arVEncCmdMsTbl, "Multiscreen interaction demo",     CLI_SUPERVISOR},
#ifdef LINUX_TURNKEY_SOLUTION
	{"encode",		   NULL,   _EncodeTestCmd, NULL, "Encode test command", 				CLI_SUPERVISOR},
#endif	
#ifdef CC_ATV_PVR_SUPPORT
    {"atvpvr",         "atv",  NULL,                _arVEncCmdAtvTbl, "ATV PVR demo",                     CLI_SUPERVISOR},
#endif
#ifdef VDEC_VENC_PATH
    {"vdec",           NULL,   NULL,               _arVEncCmdVdecTbl, "Vdec Path demo",                   CLI_SUPERVISOR},
#endif
#ifdef VENC_EMU
    {"emu load",       "el",   _VEncCmdLoadEmu,                 NULL, "Load Emu items",                   CLI_SUPERVISOR},
    {"emu",            "emu",  NULL,                            NULL, "Emu items",                        CLI_SUPERVISOR},
#endif
    
#ifdef INPUT_BUF_FROM_SCALER
    {"trigscaler",     "ts",   _VEncCmdTriggerScaler,           NULL, "Trigger Scaler",                CLI_SUPERVISOR},
#endif

#ifdef __MODEL_slt__
    {"slt",            NULL,   _VEncCmdSlt,                     NULL, "Video encoder SLT",                CLI_SUPERVISOR},
#endif
    CLIMOD_DEBUG_CLIENTRY(VENC),     // for LOG use
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

//LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(VEnc)
{
    "venc",
    NULL,
    NULL,
    _arVEncCmdTbl,
    "Video Encoder command",
    CLI_SUPERVISOR
};
//LINT_RESTORE


VENC_BUFMGR_DECLARE(VENC_RZ_FRM, VENC_FRM_T, 10);
VENC_BUFMGR_BASE_T *      _ptResizeFrmBuffMgr = NULL;


VENC_BUFMGR_DECLARE(VENC_FRM, VENC_FRM_T, 10);
VENC_BUFMGR_BASE_T *      _ptFrmBuffMgr = NULL;
VENC_BUFMGR_T(VENC_FRM) * _ptFrmBuffMgrDbg = NULL;
static VENC_HANDLE _hVenc = NULL;
#ifdef VENC_FRAME_CTRL_BY_DRV
VENC_FILTER_T _tEncFilter;
VENC_FILTER_T *_ptEncFilter = NULL;
#endif

#ifdef VENC_EMU
CHAR _acVEncEmuFListFileName[128];
PFNVEncCmdEmuItem pfnVEncCmdEmuItem = NULL;
PFNVEncPscan      pfnVEncPscan = NULL;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


extern UINT32 _u4BsTmp;

#ifdef VENC_RAVEN_DRIVER //Venc Raven driver, 53xx_com_driver/venc_v2
extern INT32 i4Venc_Hw_VENC_BITSTREAM_BUF_DRAM_ADDR_GET(VOID);
#else //Venc Eagle driver, 53xx_com_driver/Venc
extern UINT32 _VENC_H264_HalGetOutputCurAddr(VOID);
#endif

static INT32 _VEncCmdDumpBs(INT32 i4Argc, const CHAR ** szArgv)
{
    static UINT32 u4Cnt = 0;
    
#ifdef VENC_RAVEN_DRIVER
    UINT32 u4BsEnd = VIRTUAL(i4Venc_Hw_VENC_BITSTREAM_BUF_DRAM_ADDR_GET() - 1);
#else
    UINT32 u4BsEnd = VIRTUAL(_VENC_H264_HalGetOutputCurAddr() - 1);
#endif
    
    UINT32 u4BsSa = VIRTUAL(_u4BsTmp);

    if (i4Argc > 1)
    {
        u4Cnt = StrToInt(szArgv[1]);
    }

    HalInvalidateDCacheMultipleLine(u4BsSa, u4BsEnd - u4BsSa + 1);
    LOG(0, "d.save.b G:\\test%d.264 0x%08x--0x%08x\n", u4Cnt++, u4BsSa, u4BsEnd);

    return 0;
}


#ifdef __MODEL_slt__
extern UINT32 _VEncH264Slt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdSlt(INT32 i4Argc, const CHAR ** szArgv)
{
    return (INT32)_VEncH264Slt(i4Argc, szArgv);
}
#endif

#ifdef INPUT_BUF_FROM_SCALER
extern UINT32 _VEncH264TriggerScaler(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VEncCmdTriggerScaler(INT32 i4Argc, const CHAR ** szArgv)
{
    return (INT32)_VEncH264TriggerScaler(i4Argc, szArgv);
}
#endif


#if 1

//--input source settings
//#define TMP_DRIVER_SET_SUB
/// need to remove setting sub source at driver, disable this define to do this

#ifdef TMP_DRIVER_SET_SUB

extern void vScpipSetForcedHpsFactor(UINT32 u4VdpId, UINT32 u4Factor);
extern void vScpipSetForcedVpsFactor(UINT32 u4VdpId, UINT32 u4Factor);
EXTERN UINT32 _u4DrvVideoGetMute(UINT32 u4Path);

//// issue
// main is hdmi ycbcr   sub is vga (hide) error
//extern UINT8 u1ForceHPSDByCli;
static void _VENCPIP(UINT8 u1OnOff)
{
    VDP_REGION_T rRegion;
    VDP_OVERSCAN_REGION_T rOvRegion;
    UINT8 u1MainSrc;
    static UINT8 u1OldSubSrc = SV_VS_ATV1;
    static UCHAR ucEnable;
    UINT32 u4WFactor, u4HFactor,u4Width,u4Height;
    rRegion.u4X = 0;
    rRegion.u4Y = 0;

    if (u1OnOff)
    {
        u4Width = wDrvVideoInputWidth(VDP_1);//640;
        u4Height = wDrvVideoInputHeight(VDP_1);//480;
        
        u1OldSubSrc= bApiVideoGetSrcType(VDP_2);
        u1MainSrc = bApiVideoGetSrcType(VDP_1);
        Printf("Set VDP2 Enable FFFFF\n");

        rRegion.u4Width = VDP_MAX_REGION_WIDTH; // >> 1);
        rRegion.u4Height = VDP_MAX_REGION_HEIGHT; // >> 1);
        VDP_SetOutRegion(VDP_2, 0, rRegion);
        /*LOG(0, "width %d  \n", wDrvVideoInputWidth(VDP_1));*/
        /*if (wDrvVideoInputWidth(VDP_1) >= 1280)
        {
            u1ForceHPSDByCli = 1;
            vScpipSetForcedHpsFactor(VDP_2, 0x4000);
            LOG(0, "Predown 1/2 \n");
        }*/
        //_vApiTveSetSclPara(TRUE, 720, 576);
        VDP_GetEnable(VDP_2, &ucEnable);
        VDP_SetEnable(VDP_2, TRUE);
        VDP_SetDisplayOff_Ext(VDP_2, DRV_DISP_OFF_MODULE_VENC,TRUE);
        rOvRegion.u4Top = 0;
        rOvRegion.u4Bottom = 0;
        rOvRegion.u4Left = 0;
        rOvRegion.u4Right = 0;
        VDP_SetOverScan(VDP_2, rOvRegion);

        /*if( VSS_MAJOR(u1MainSrc) == VSS_DTV || VSS_MAJOR(u1MainSrc) == VSS_DTD)
        {
            VDP_SetInput(VDP_2, 0, 0);
        }*/
        bApiVideoSetVideoSrc(SV_VP_PIP, u1MainSrc);
        rRegion.u4Width = VDP_MAX_REGION_WIDTH;
        rRegion.u4Height = VDP_MAX_REGION_HEIGHT;
        VDP_SetSrcRegion(VDP_2,0, rRegion);
        //vClrPipFlg(PIP_FLG_MODE_DET_DONE);
        //vSetPipFlg(PIP_FLG_MODE_CHG);
        //u1ForceHPSDByCli = 1;

        u4WFactor = 0x8000;
        u4HFactor = 0x8000;
        if( u4Width > 1280 || u4Height > 720)
        {
            u4WFactor = 0x8000 * 1280 / u4Width;
            u4HFactor = 0x8000 * 720 / u4Height;            
        }

        vScpipSetForcedHpsFactor(VDP_2, u4WFactor);
        vScpipSetForcedVpsFactor(VDP_2, u4HFactor);
#if 1
        x_thread_delay(400);
#else
        do{
            x_thread_delay(100);
        }while( bDrvVideoSignalStatus(VDP_1) != SV_VDO_STABLE ||
                bDrvVideoSignalStatus(VDP_2) != SV_VDO_STABLE ||
                _u4DrvVideoGetMute(VDP_2) == TRUE );
#endif
    }
    else
    {
#if 1
        //SRM_SetTvMode(rOldSrmMode);
        vScpipSetForcedHpsFactor(VDP_2, 0);
        vScpipSetForcedVpsFactor(VDP_2, 0);
        //u1ForceHPSDByCli = 0;
        VDP_SetDisplayOff_Ext(VDP_2, DRV_DISP_OFF_MODULE_VENC,FALSE);
        bApiVideoSetVideoSrc(SV_VP_PIP, u1OldSubSrc);
        VDP_SetEnable(VDP_2, ucEnable);
        if (!ucEnable)
        {
            rRegion.u4Width = 0;
            rRegion.u4Height = 0;
            VDP_SetOutRegion(VDP_2, 0, rRegion);
        }
        //remove these to test
        //vClrPipFlg(PIP_FLG_MODE_DET_DONE);
        //vSetPipFlg(PIP_FLG_MODE_CHG);
        Printf("  UC enable %d gggg \n", ucEnable);
        do{
            x_thread_delay(100);
        }while(bDrvVideoSignalStatus(VDP_1) != SV_VDO_STABLE ||
               _u4DrvVideoGetMute(VDP_1) == TRUE);
#endif        
    }

}

extern UINT8 u1HdmiColorMode;
static void _VENCVDPInit(UINT8 u1OnOff)
{
//    static UINT8 fgIsHDMIChg = 0, u1OldHdmiMode = SV_HDMI_MODE_AUTO;
//    static UINT8 fgIsVGAChg = 0;

    _VENCPIP(u1OnOff);
#if 0
    if (u1OnOff)
    {
        if (SV_VD_DVI == bGetVideoDecType(VDP_2) && bHDMIInputType() == 1 && bIsScalerInput444(VDP_2))
        {
            u1OldHdmiMode = u1HdmiColorMode;
            u1HdmiColorMode = SV_HDMI_MODE_VIDEO ;
            vApiHdmiColorModeChg(VDP_2);
            fgIsHDMIChg = 1;
        }
        else if (SV_VD_VGA == bGetVideoDecType(VDP_2))
        {
            if(u1GetVGA422En() != SV_TRUE)
            {
                vVgaTriggerModeChange();
                vSetVGA422En(SV_TRUE);
                fgIsVGAChg = 1;
            }
        }

        do{
            x_thread_delay(100);
              /*if(bDrvVideoSignalStatus(VDP_2) != SV_VDO_STABLE )
               LOG(0, "\n\n bDrvVideoSignalStatus(VDP_2) != SV_VDO_STABLE \n\n");
           if(bDrvVideoSignalStatus(VDP_1) != SV_VDO_STABLE )
               LOG(0, "\n\n bDrvVideoSignalStatus(VDP_1) != SV_VDO_STABLE \n\n");
           if (_u4DrvVideoGetMute(VDP_1) == TRUE)
               LOG(0,"\n\n _u4DrvVideoGetMute(VDP_1) == TRUE \n\n");       */
        }while( bDrvVideoSignalStatus(VDP_1) != SV_VDO_STABLE ||
                bDrvVideoSignalStatus(VDP_2) != SV_VDO_STABLE ||
                _u4DrvVideoGetMute(VDP_1) == TRUE);
    }
    else
    {
        if (fgIsHDMIChg && SV_VD_DVI == bGetVideoDecType(VDP_1))
        {
            fgIsHDMIChg = 0;
            u1HdmiColorMode = u1OldHdmiMode;
            vApiHdmiColorModeChg(VDP_1);
        }
        else if (SV_VD_VGA == bGetVideoDecType(VDP_1))
        {
            if (fgIsVGAChg)
            {
                fgIsVGAChg = 0;
                vVgaTriggerModeChange();
                vSetVGA422En(SV_FALSE);
            }
        }

        do{
            x_thread_delay(100);
        }while( bDrvVideoSignalStatus(VDP_1) != SV_VDO_STABLE ||
                _u4DrvVideoGetMute(VDP_1) == TRUE);
    }
#endif
}
#endif // TMP_DRIVER_SET_SUB

static INT32 _VEncCmdMultiScreenDemoRun(INT32 i4Argc, const CHAR ** szArgv)
{
    VENC_HANDLE hVenc = NULL;
    UINT32 u4Value;
    UINT32 u4Ret;
    UINT32 u4Fps;
    UINT32 u4SrcPitch;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4TgtPitch;
    UINT32 u4TgtWidth;
    UINT32 u4TgtHeight;
    VENC_CTRL_OUTCB_T rCb;
    VENC_PRAM_RES_T res;
    VENC_CTRL_PIXFMT_T pix;
    VENC_CTRL_RZ_T rRz;
    BOOL fgResize = FALSE;
    BOOL fgInterlace = IS_INTERLACE(VDP_2)? TRUE : FALSE;

#ifdef TMP_DRIVER_SET_SUB
    _VENCVDPInit(SV_ON);
#endif

    if (VENC_NULL_HANDLE != _hVenc)
    {
        LOG(0, "%s(%d): handle of venc is not release yet!\n", __FUNCTION__, __LINE__);
        ASSERT(VENC_NULL_HANDLE == _hVenc);
        return VENC_DEV_BUSY;
    }

    hVenc = VENC_Open(VENC_H264, VENC_PROFILE_MS, fgInterlace);
    if (VENC_NULL_HANDLE == hVenc)
    {
        LOG(0, "%s(%d): Open venc fail!\n", __FUNCTION__, __LINE__);
        return VENC_INV_HANDLE;
    }
    _hVenc = hVenc;

    u4TgtWidth = wDrvVideoInputWidth(VDP_2);//640;
    u4TgtHeight = wDrvVideoInputHeight(VDP_2);//480;
    if (u4TgtWidth > 1280 || u4TgtHeight > 720)
    {
        u4TgtWidth = 1280;
        u4TgtHeight = 720;
    }
    u4TgtPitch = VENC_ALIGN_MASK(u4TgtWidth, 15);
    u4SrcWidth = wDrvVideoInputWidth(VDP_2);//u2DrvDIGetVdpWidth(1);
    u4SrcHeight = wDrvVideoInputHeight(VDP_2); //u2DrvDIGetVdpHeight(1);
#ifdef TMP_DRIVER_SET_SUB
    // scaler do the down scale
    if (u4SrcWidth > 1280 || u4SrcHeight > 720)
    {
        u4SrcWidth = 1280;
        u4SrcHeight = 720;
    }
#endif
    u4SrcPitch= VENC_ALIGN_MASK(u4SrcWidth, 15);
    u4Fps = bDrvVideoGetRefreshRate(VDP_2);
    if (fgInterlace)
    {
        u4Fps >>= 1;
    }
    else if (u4TgtWidth * u4TgtHeight * u4Fps > 1280 * 720 * 30)
    {
        u4Fps >>= 1;
        vDrvDIVencDropFrame(1);
    }

    if ((u4SrcWidth != u4TgtWidth) || (u4SrcHeight != u4TgtHeight))
    {
        fgResize = TRUE;
        LOG(0, "%s: %dx%d resize to %dx%d @ %d fps\n", __FUNCTION__, u4SrcWidth, u4SrcHeight, u4TgtWidth, u4TgtHeight, u4Fps);
    }
    else
    {
        LOG(0, "%s: %dx%d @ %d fps\n", __FUNCTION__, u4SrcWidth, u4SrcHeight, u4Fps);
    }

    u4Value = u4Fps;
    LOG(2, "%s: set %d fps\n", __FUNCTION__, u4Value);
    u4Ret = VENC_SetParam(hVenc, VENC_PRAM_FRAMERATE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set framerate failed(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //##bitrate.
    u4Value = 1000;
    LOG(2, "%s: set %d kbps\n", __FUNCTION__, u4Value);
    u4Ret = VENC_SetParam(hVenc, VENC_PRAM_BITRATE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(1, "%s(%d): Set bitrate failed(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //##resolution
    res.u2Width  = (UINT16)u4TgtWidth;
    res.u2Height = (UINT16)u4TgtHeight;
    LOG(2, "%s: set %dx%d\n", __FUNCTION__, res.u2Width, res.u2Height);
    u4Ret = VENC_SetParam(hVenc, VENC_PRAM_RESOLUTION, (VOID*)&res);
    if (VENC_OK != u4Ret)
    {
        LOG(1, "%s(%d): Set resolution failed(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //##pixel format
    pix.ePixFmt  = VENC_PIX_FMT_NV12;
    pix.u2Pitch  = (UINT16)u4TgtPitch;
    u4Ret = VENC_SetCtrl(hVenc, VENC_CTRL_PIX_FMT, (VOID*)&pix);
    if (VENC_OK != u4Ret)
    {
       LOG(1, "%s(%d): Set pixfmt failed(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //SYNC MODE
    u4Value = (UINT32)VENC_SMODE_ASYNC;
    u4Ret = VENC_SetCtrl(hVenc, VENC_CTRL_SYNC_MODE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set sync mode error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //Output Callback
    rCb.pvData = NULL;
    rCb.cb     = venc_demo_cb;
    u4Ret = VENC_SetCtrl(hVenc, VENC_CTRL_OUT_CB, (VOID*)&rCb);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set callback error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

//set buffer
{
    UINT32 u4Size;
    VENC_FRM_T rFrm;
    UINT32 u4Idx, u4TmpAddr;
    FBM_POOL_T *prFbmPool;
    VENC_CTRL_BUFF_T buff;

    prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_VENC);

    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);
    ASSERT(prFbmPool->u4Size != 0);

    //raw data buffer
    ASSERT(NULL == _ptFrmBuffMgr);
    _ptFrmBuffMgr = (VENC_BUFMGR_BASE_T *)VENC_BUFMGR_OBJ_CREATE(VENC_FRM);
    ASSERT(_ptFrmBuffMgr);

    _BUFMGR_OBJ_Reset(_ptFrmBuffMgr);
    u4Size = VENC_ALIGN_MASK(u4TgtWidth, 15) * VENC_ALIGN_MASK(u4TgtHeight, 31);

    u4TmpAddr = VENC_ALIGN_MASK(prFbmPool->u4Addr, 63);
      
#ifndef INPUT_BUF_FROM_SCALER // alloc input buffer for DI
    for (u4Idx = 2; u4Idx < 4; u4Idx++)
    {
        rFrm.u4YAddr  = u4TmpAddr;
        u4TmpAddr    += u4Size;
        u4TmpAddr     = VENC_ALIGN_MASK(u4TmpAddr, 63);
        rFrm.u4UVAddr = u4TmpAddr;
        u4TmpAddr    += (u4Size >> 1);
        u4TmpAddr     = VENC_ALIGN_MASK(u4TmpAddr, 63);
        _BUFMGR_OBJ_Add(_ptFrmBuffMgr, &rFrm);
    }
#else // alloc input buffer for scaler
    u1DrvScpipH264SetDramAddress(u4TmpAddr, (1920*1088)); 
    u4TmpAddr += 0x900000; //3 input buffer
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, 63);
#endif

    if (fgResize)
    {
        ASSERT(NULL == _ptResizeFrmBuffMgr);
        _ptResizeFrmBuffMgr = (VENC_BUFMGR_BASE_T *)VENC_BUFMGR_OBJ_CREATE(VENC_RZ_FRM);
        ASSERT(_ptResizeFrmBuffMgr);

        _BUFMGR_OBJ_Reset(_ptResizeFrmBuffMgr);
        u4Size = 1920 * 1088;

        for (u4Idx = 0; u4Idx < VENC_RESIZE_BUF_N; u4Idx++)
        {
            rFrm.u4YAddr  = u4TmpAddr;
            u4TmpAddr    += u4Size;
            u4TmpAddr     = VENC_ALIGN_MASK(u4TmpAddr, 63);
            rFrm.u4UVAddr = u4TmpAddr;
            u4TmpAddr    += (u4Size >> 1);
            u4TmpAddr     = VENC_ALIGN_MASK(u4TmpAddr, 63);
            _BUFMGR_OBJ_Add(_ptResizeFrmBuffMgr, &rFrm);
        }
    }

    //Working Buffer
    buff.u4Addr  = VENC_ALIGN_MASK(u4TmpAddr, 63);
    buff.u4Size  = prFbmPool->u4Size - (u4TmpAddr - prFbmPool->u4Addr);
    ASSERT(prFbmPool->u4Size > (buff.u4Addr - prFbmPool->u4Addr));

    u4Ret = VENC_SetCtrl(hVenc, VENC_CTRL_BUFF, (VOID*)&buff);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set buffer error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }
}

    if (fgResize)
    {
        rRz.u4SrcPitch = u4SrcPitch;
        rRz.u4SrcWidth = u4SrcWidth;
        rRz.u4SrcHeight = u4SrcHeight;
        rRz.pvIn = (void *)_ptResizeFrmBuffMgr;
        rRz.pvOut = (void *)_ptFrmBuffMgr;
        u4Ret = VENC_SetCtrl(hVenc, VENC_CTRL_RZ, (VOID*)&rRz);
        if (VENC_OK != u4Ret)
        {
            LOG(0, "%s(%d): Set resize error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
            return -1;
        }
    }

    //Init
    u4Ret = VENC_Init(hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Init fail(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //Run
    u4Ret = VENC_Run(hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Run fail(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    // trigger DI write mode
    vDrvDISupportH264Init();
    vDrvDISetVEncHandle((void *)hVenc, (void *)_ptFrmBuffMgr, (void *)_ptResizeFrmBuffMgr, NULL);

    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 0;
}


static INT32 _VEncCmdMultiScreenDemoStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Ret;

    // stop pscan
    vDrvDIStopSendBufToVENC(TRUE);

    //Stop
    u4Ret = VENC_Stop(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Stop fail(%d)!\n", u4Ret);
        return u4Ret;
    }

    //Close
    u4Ret = VENC_Close(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Close fail(%d)!\n", u4Ret);
        return u4Ret;
    }

    _hVenc = VENC_NULL_HANDLE;

    if (_ptFrmBuffMgr)
    {
        VENC_BUFMGR_OBJ_DELETE(VENC_FRM,(VENC_BUFMGR_T(VENC_FRM) *)_ptFrmBuffMgr);
        _ptFrmBuffMgr = NULL;
    }

    if (_ptResizeFrmBuffMgr)
    {
        VENC_BUFMGR_OBJ_DELETE(VENC_RZ_FRM,(VENC_BUFMGR_T(VENC_RZ_FRM) *)_ptResizeFrmBuffMgr);
        _ptResizeFrmBuffMgr = NULL;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef TMP_DRIVER_SET_SUB
    _VENCVDPInit(SV_OFF);
#endif

    return 0;
}
#endif

#ifdef CC_ATV_PVR_SUPPORT
//#define TIMING_HARD_CODE
#define DI_DEMO_CB
static INT32 _VEncCmdAtvPvrDemoRun(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value;
    UINT32 u4Ret;
    UINT32 u4Fps;
    UINT32 u4SrcPitch;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4TgtPitch;
    UINT32 u4TgtWidth;
    UINT32 u4TgtHeight;
    VENC_CTRL_OUTCB_T rCb;
    VENC_PRAM_RES_T res;
    VENC_CTRL_PIXFMT_T pix;
    BOOL fgInterlace = IS_INTERLACE(VDP_2)? TRUE : FALSE;

    _hVenc = VENC_Open(VENC_H264, VENC_PROFILE_PVR, fgInterlace);
    if (VENC_NULL_HANDLE == _hVenc)
    {
        LOG(0, "%s(%d): Open venc fail!\n", __FUNCTION__, __LINE__);
        return VENC_INV_HANDLE;
    }

#ifdef TIMING_HARD_CODE
    u4SrcWidth = 1280;
    u4SrcHeight = 720;
    u4SrcPitch= 1280;
    u4Fps = 30;
#else
    u4SrcWidth = u2DrvDIGetVdpWidth(1);
    u4SrcHeight = u2DrvDIGetVdpHeight(1);
    u4SrcPitch= (u4SrcWidth + 15) & ~15;
    u4Fps = bDrvVideoGetRefreshRate(1);
    if (fgInterlace || u4Fps > 30)
    {
        u4Fps >>= 1;
    }
#endif


    u4TgtPitch = u4SrcPitch;
    u4TgtWidth = u4SrcWidth;
    u4TgtHeight = u4SrcHeight;
    Printf("ATV PVR demo %dx%d @ %d fps\n", u4SrcWidth, u4SrcHeight, u4Fps);

    u4Value = u4Fps;
    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_FRAMERATE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set parameter error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    u4Value = 1000;
    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_BITRATE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set parameter error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    res.u2Width = u4TgtWidth;
    res.u2Height= u4TgtHeight;
    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_RESOLUTION, (VOID*)&res);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set parameter error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //PIX FORMATE
    pix.ePixFmt  = VENC_PIX_FMT_NV12;
    pix.u2Pitch   = u4TgtPitch;
    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_PIX_FMT, (VOID*)&pix);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set ctrl error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //SYNC MODE
    u4Value = (UINT32)VENC_SMODE_ASYNC;
    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_SYNC_MODE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set ctrl error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //Output Callback
    rCb.pvData = NULL;
    rCb.cb     = venc_demo_cb;
    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_OUT_CB, (VOID*)&rCb);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set ctrl error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return -1;
    }

//set buffer
#if 1
{
    UINT32 u4Size;
    VENC_FRM_T rFrm;
    UINT32 u4Idx, u4TmpAddr;
    FBM_POOL_T *prFbmPool;
    VENC_CTRL_BUFF_T buff;

    prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_VENC);

    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);
    ASSERT(prFbmPool->u4Size != 0);

    //raw data buffer
    _ptFrmBuffMgr = (VENC_BUFMGR_BASE_T *)VENC_BUFMGR_OBJ_CREATE(VENC_FRM);
    ASSERT(_ptFrmBuffMgr);

    _BUFMGR_OBJ_Reset(_ptFrmBuffMgr);
    u4Size = 1280*736; //VENC_ALIGN_MASK(res.u2Width, 31) * VENC_ALIGN_MASK(res.u2Height, 31); //

    u4TmpAddr = VENC_ALIGN_MASK(prFbmPool->u4Addr, 63);
    for (u4Idx = 2; u4Idx < 8; u4Idx++)
    {
        rFrm.u4YAddr  = u4TmpAddr;
        u4TmpAddr    += u4Size;
        u4TmpAddr     = VENC_ALIGN_MASK(u4TmpAddr, 63);
        rFrm.u4UVAddr = u4TmpAddr;
        u4TmpAddr    += (u4Size >> 1);
        u4TmpAddr     = VENC_ALIGN_MASK(u4TmpAddr, 63);
        _BUFMGR_OBJ_Add(_ptFrmBuffMgr, &rFrm);
    }

    //Working Buffer
    buff.u4Addr  = VENC_ALIGN_MASK(u4TmpAddr, 63);
    buff.u4Size  = prFbmPool->u4Size - (u4TmpAddr - prFbmPool->u4Addr);

    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_BUFF, (VOID*)&buff);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Set ctrl error(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }
}
#endif

    //Init
    u4Ret = VENC_Init(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Init fail(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    //Run
    u4Ret = VENC_Run(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "%s(%d): Run fail(%d)!\n", __FUNCTION__, __LINE__, u4Ret);
        return u4Ret;
    }

    // trigger DI write mode
    vDrvDISupportH264Init();
    vDrvDISetVEncHandle((void *)_hVenc, (void *)_ptFrmBuffMgr, NULL, NULL);

#ifdef CC_ATV_PVR_SUPPORT
    // trigger VBI
    VBI_PVRStartNotifyCCOnOff(SV_VP_MAIN, TRUE);
#endif

    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 0;
}


static INT32 _VEncCmdAtvPvrDemoStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Ret;

#ifdef CC_ATV_PVR_SUPPORT
    // stop VBI
    VBI_PVRStartNotifyCCOnOff(SV_VP_MAIN, FALSE);
#endif

    // stop pscan
    vDrvDIStopSendBufToVENC(TRUE);

    //Stop
    u4Ret = VENC_Stop(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Stop fail(%d)!\n", u4Ret);
        return u4Ret;
    }

    //Close
    u4Ret = VENC_Close(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Close fail(%d)!\n", u4Ret);
        return u4Ret;
    }

    _hVenc = VENC_NULL_HANDLE;

    if (_ptFrmBuffMgr)
    {
        VENC_BUFMGR_OBJ_DELETE(VENC_FRM,(VENC_BUFMGR_T(VENC_FRM) *)_ptFrmBuffMgr);
        _ptFrmBuffMgr = NULL;
    }

    if (_ptResizeFrmBuffMgr)
    {
        VENC_BUFMGR_OBJ_DELETE(VENC_RZ_FRM,(VENC_BUFMGR_T(VENC_RZ_FRM) *)_ptResizeFrmBuffMgr);
        _ptResizeFrmBuffMgr = NULL;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 0;
}
#endif


#ifdef VDEC_VENC_PATH
static INT32 _VEncCmdVdecPathDemoRun(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value;
    UINT32 u4Ret;
    UINT32 u4Width, u4Height, u4Pitch, u4Fps;
    VENC_PRAM_RES_T res;
    VENC_CTRL_PIXFMT_T pix;
    VENC_CTRL_OUTCB_T rCb;

    _hVenc = VENC_Open(VENC_H264, VENC_PROFILE_VPHONE, FALSE);

    if (i4Argc > 2)
    {
        u4Width  = StrToInt(szArgv[1]);
        u4Height = StrToInt(szArgv[2]);
    }
    else
    {
        u4Width  = 640;
        u4Height = 480;
    }

    u4Pitch= (u4Width + 15) & ~15;
    u4Fps    = 30;

    LOG(5, "VDEC path to VENC demo %dx%d @ %d fps\n", u4Width, u4Height, u4Fps);

    if (VENC_NULL_HANDLE == _hVenc)
    {
        LOG(0, "Open venc fail!\n");
        return VENC_INV_HANDLE;
    }

    u4Value = u4Fps;
    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_FRAMERATE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Set parameter error!\n");
        return u4Ret;
    }

    u4Value = 1000;
    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_BITRATE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Set parameter error!\n");
        return u4Ret;
    }

    res.u2Width = u4Width;
    res.u2Height= u4Height;
    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_RESOLUTION, (VOID*)&res);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Set parameter error!\n");
        return u4Ret;
    }

    //PIX FORMATE
    pix.ePixFmt  = VENC_PIX_FMT_NV12;
    pix.u2Pitch  = u4Pitch;
    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_PIX_FMT, (VOID*)&pix);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Set ctrl error!\n");
        return u4Ret;
    }

    //SYNC MODE
    u4Value = (UINT32)VENC_SMODE_ASYNC;
    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_SYNC_MODE, (VOID*)&u4Value);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Set ctrl error!\n");
        return u4Ret;
    }

    //Output Callback
    rCb.pvData = NULL;
    rCb.cb     = venc_demo_cb;
    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_OUT_CB, (VOID*)&rCb);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Set ctrl error(%d)!\n", u4Ret);
        return -1;
    }


    //set buffer
    //Working Buffer
    {
    UINT32 u4Size;
    VENC_FRM_T rFrm;
    UINT32 u4Idx, u4TmpAddr;
    FBM_POOL_T *prFbmPool;
    VENC_CTRL_BUFF_T buff;

    prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_VENC);

    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);
    ASSERT(prFbmPool->u4Size != 0);

    //raw data buffer
    _ptFrmBuffMgr = (VENC_BUFMGR_BASE_T *)VENC_BUFMGR_OBJ_CREATE(VENC_FRM);
    ASSERT(_ptFrmBuffMgr);

    _BUFMGR_OBJ_Reset(_ptFrmBuffMgr);
    u4Size = 1280*736; //VENC_ALIGN_MASK(res.u2Width, 31) * VENC_ALIGN_MASK(res.u2Height, 31); //

    u4TmpAddr = VENC_ALIGN_MASK(prFbmPool->u4Addr, 63);
    for (u4Idx = 2; u4Idx < 4; u4Idx++)
    {
        rFrm.u4YAddr  = u4TmpAddr;
        u4TmpAddr    += u4Size;
        u4TmpAddr     = VENC_ALIGN_MASK(u4TmpAddr, 63);
        rFrm.u4UVAddr = u4TmpAddr;
        u4TmpAddr    += (u4Size >> 1);
        u4TmpAddr     = VENC_ALIGN_MASK(u4TmpAddr, 63);
        _BUFMGR_OBJ_Add(_ptFrmBuffMgr, &rFrm);
    }

    buff.u4Addr  = VENC_ALIGN_MASK(u4TmpAddr, 63);
    buff.u4Size  = prFbmPool->u4Size - (u4TmpAddr - prFbmPool->u4Addr);

    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_BUFF, (VOID*)&buff);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Set ctrl buffer error!\n");
        return u4Ret;
    }
    }

    //Init
    u4Ret = VENC_Init(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Init fail!\n");
        return u4Ret;
    }

    //Run
    u4Ret = VENC_Run(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Run fail!\n");
        return u4Ret;
    }

#ifdef VENC_FRAME_CTRL_BY_DRV
    _ptEncFilter = &_tEncFilter;
    //delta = u4RefFps * u4RefFpsUnit/framerate => 90000/framerate
    //TEST file 30fps ->  15fps
    _VENC_Filter_Init(_ptEncFilter, 6000);
#endif

    //vdec path settings
    LOG(0, "Connect to vdec path\n");
    _VDEC_SetVencHandle(0, (VOID *)_hVenc, (VOID *)_ptFrmBuffMgr, (VOID *)_ptEncFilter);
    _VDEC_SetVencFrmSz(0, u4Width, u4Height, u4Pitch);
    _VDEC_Set2VencFlow(0, TRUE);

    // trigger VDEC
    LOG(0, "Please play a mjpeg file\n");

    UNUSED(i4Argc);
    UNUSED(szArgv);
    return 0;
}


static INT32 _VEncCmdVdecPathDemoStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Ret;

    // stop VDEC
    LOG(0, "Disconnect from vdec path\n");
    _VDEC_Set2VencFlow(0, FALSE);
    _VDEC_SetVencHandle(0, NULL, NULL, NULL);
    _VDEC_SetVencFrmSz(0, 0, 0, 0);

    //Stop
    u4Ret = VENC_Stop(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Stop fail!\n");
        return u4Ret;
    }

    //Close
    u4Ret = VENC_Close(_hVenc);
    if (VENC_OK != u4Ret)
    {
        LOG(0, "Close fail!\n");
        return u4Ret;
    }

    _hVenc = VENC_NULL_HANDLE;

    if (_ptFrmBuffMgr)
    {
        VENC_BUFMGR_OBJ_DELETE(VENC_FRM,(VENC_BUFMGR_T(VENC_FRM) *)_ptFrmBuffMgr);
        _ptFrmBuffMgr = NULL;
    }

    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 0;
}
#endif /*VDEC_VENC_PATH*/


static INT32 _VEncCmdSetResolution(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height, u4Ret;

    if (i4Argc < 3)
    {
        LOG(0, "Usage: %s <width> <height>\n", szArgv[0]);
        return -1;
    }

    ASSERT(i4Argc >= 3);

    u4Width = StrToInt(szArgv[1]);
    u4Height = StrToInt(szArgv[2]);

#if 0
{
    extern VOID MW_VENC_SetRes(UINT32 u4Width, UINT32 u4Height);
    MW_VENC_SetRes(u4Width, u4Height);
}
#else
    if (!_hVenc)
    {
        LOG(0, "Video encoder handle is null!\n");
        return -2;
    }

#if 1
{
    VENC_RT_PARM_T rRTParm;
    //RESOLUTION
    rRTParm.u4Flag   = VENC_RT_PARM_FLAG_RES;
    rRTParm.u2Width  = u4Width;
    rRTParm.u2Height = u4Height;
    rRTParm.u2Pitch  = VENC_ALIGN_MASK(u4Width, 15);
    u4Ret = VENC_SetRtParamAsync(_hVenc, &rRTParm);
}
#else
{
    VENC_PRAM_RES_T rRes;
    rRes.u2Height = (UINT16)u4Height;
    rRes.u2Width = (UINT16)u4Width;

    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_RESOLUTION, (VOID*)&rRes);
}
#endif

    if (u4Ret != VENC_OK)
    {
        LOG(0, "Set Video encoder resolution fail(%d)!\n", u4Ret);
        return (INT32)u4Ret;
    }
#endif

    return 0;
}


static INT32 _VEncCmdSetFrameRate(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4fps, u4Ret;

    if (i4Argc < 2)
    {
        LOG(0, "Usage: %s <fps>\n", szArgv[0]);
        return -1;
    }

    ASSERT(i4Argc >= 2);
    u4fps = StrToInt(szArgv[1]);

#if 0
{
    extern VOID MW_VENC_SetFps(UINT32 u4Fps);
    MW_VENC_SetFps(u4fps);
}
#else
    if (!_hVenc)
    {
        LOG(0, "Video encoder handle is null!\n");
        return -2;
    }

    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_FRAMERATE, (VOID*)&u4fps);
    if (u4Ret != VENC_OK)
    {
        LOG(0, "Set Video encoder frame rate fail(%d)!\n", u4Ret);
        return (INT32)u4Ret;
    }
#endif

    return 0;
}


static INT32 _VEncCmdSetBitRate(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Bps, u4Ret;

    if (i4Argc < 2)
    {
        LOG(0, "Usage: %s <bps>\n", szArgv[0]);
        return -1;
    }

    ASSERT(i4Argc >= 2);
    u4Bps = StrToInt(szArgv[1]);

#if 0
{
    extern VOID MW_VENC_SetBps(UINT32 u4Bps);
    MW_VENC_SetBps(u4Bps);
}
#else
    if (!_hVenc)
    {
        LOG(0, "Video encoder handle is null!\n");
        return -2;
    }

    u4Ret = VENC_SetParam(_hVenc, VENC_PRAM_BITRATE, (VOID*)&u4Bps);
    if (u4Ret != VENC_OK)
    {
        LOG(0, "Set Video encoder bitrate fail(%d)!\n", u4Ret);
        return (INT32)u4Ret;
    }
#endif

    return 0;
}


#ifdef LINUX_TURNKEY_SOLUTION
INT32 _EncodeTestCmd(INT32 i4Argc, const CHAR ** szArgv)
{
   ENCODER_TESTCMD rCmd;
   if(i4Argc<=1)
   {
      return 0;
   }
   rCmd.u4Cmd= StrToInt(szArgv[1]);
   if(i4Argc>2)
   {
	   rCmd.u4CmdParam[0]= StrToInt(szArgv[2]);
   }

   if(i4Argc>3)
   {
	   rCmd.u4CmdParam[1]= StrToInt(szArgv[3]);
   }

   if(i4Argc>4)
   {
	   rCmd.u4CmdParam[2]= StrToInt(szArgv[4]);
   }

   if(i4Argc>5)
   {
	   rCmd.u4CmdParam[3]= StrToInt(szArgv[5]);
   }

   if(i4Argc>6)
   {
	   rCmd.u4CmdParam[4]= StrToInt(szArgv[6]);
   }

   if(i4Argc>7)
   {
	   rCmd.u4CmdParam[5]= StrToInt(szArgv[7]);
   }

   if(i4Argc>8)
   {
	   rCmd.u4CmdParam[6]= StrToInt(szArgv[8]);
   }

   if(i4Argc>9)
   {
	   rCmd.u4CmdParam[7]= StrToInt(szArgv[9]);
   }

   if(i4Argc>10)
   {
	   rCmd.u4CmdParam[8]= StrToInt(szArgv[10]);
   }

   _CB_PutEvent(256/*CB_MTAL_ENC_TESTCMD_NFY*/, sizeof(ENCODER_TESTCMD), &rCmd); 
   return 0;
}
#endif   
static INT32 _VEncCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Idx = 0;
    VENC_HANDLE handle = VENC_NULL_HANDLE;

    VENC_FOR_EACH_HANDLE(handle)
    {
        UINT32 u4Value;

        LOG(0, "\n");
        LOG(0, "Instance %d:\n", u4Idx);

        {
        UINT8  u1aName[8];
        if (VENC_OK == VENC_GetCtrl(handle, VENC_CTRL_INT_GET_NAME, (VOID*)u1aName))
        {
            LOG(0, "\tName:%s\n", u1aName);
        }
        }

        {
        VENC_PRAM_RES_T rRes;
        rRes.u2Width  = 0;
        rRes.u2Height = 0;
        if (VENC_OK == VENC_GetParam(handle, VENC_PRAM_RESOLUTION, (VOID*)&rRes))
        {
            LOG(0, "\tResolution:%dx%d\n", rRes.u2Width, rRes.u2Height);
        }
        }

        if (VENC_OK == VENC_GetParam(handle, VENC_PRAM_BITRATE, (VOID*)&u4Value))
        {
            LOG(0, "\tBitrate:%dkbps\n", u4Value);
        }

        if (VENC_OK == VENC_GetParam(handle, VENC_PRAM_FRAMERATE, (VOID*)&u4Value))
        {
            LOG(0, "\tFramerate:%dfps\n", u4Value);
        }

        {
        VENC_CTRL_PIXFMT_T rPixFmt;
        rPixFmt.ePixFmt = VENC_PIX_FMT_PSCAN;
        if (VENC_OK == VENC_GetCtrl(handle, VENC_CTRL_PIX_FMT,(VOID*)&rPixFmt))
        {
            LOG(0, "\tPixel Formate:%s\n", (rPixFmt.ePixFmt  == VENC_PIX_FMT_PSCAN)?"PSCAN":
                                                 ((rPixFmt.ePixFmt == VENC_PIX_FMT_NV12)? "NV12":
                                                   (rPixFmt.ePixFmt == VENC_PIX_FMT_B16X32? "B16X32": "B64X32")));
        }
        }

        {
        VENC_CTRL_BUFF_T rBuff;
        rBuff.u4Addr = 0;
        rBuff.u4Size = 0;
        if (VENC_OK == VENC_GetCtrl(handle, VENC_CTRL_BUFF,   (VOID*)&rBuff))
        {
            LOG(0, "\tWorking Buff: from 0x%08x to 0x%08x\n", rBuff.u4Addr, rBuff.u4Addr + rBuff.u4Size);
        }
        }

        {
        VENC_CTRL_STATE rState;
        rState.eState = VENC_STATE_STOP;
        if (VENC_OK == VENC_GetCtrl(handle, VENC_CTRL_INT_GET_STATE, (VOID*)&rState))
        {
            LOG(0, "\tState: %s, Engine State:%s \n", rState.eState == VENC_STATE_STOP ?"stop":"run",
                                                        rState.eEncState == VENC_ENC_STATE_IDLE? "idle":"encoding");
        }
        }

        {
        VENC_CTRL_BS_INFO rBsInfo;
        rBsInfo.u4Addr = 0;
        rBsInfo.u4Size = 0;
        rBsInfo.u4Wp   = 0;
        rBsInfo.u4Rp   = 0;
        if (VENC_OK == VENC_GetCtrl(handle, VENC_CTRL_INT_GET_BS_INFO, (VOID*)&rBsInfo))
        {
            LOG(0, "\tBs info::addr:0x%08x, size:0x%08x, wp:0x%08x, rp:0x%08x\n",
                rBsInfo.u4Addr, rBsInfo.u4Size, rBsInfo.u4Wp, rBsInfo.u4Rp);
        }
        }

        VENC_GetCtrl(handle, VENC_CTRL_INT_GET_TYPE, (VOID*)&u4Value);
        switch(u4Value)
        {
        case VENC_H264:
        {
            VENC_H264_PRAM_PL_T rPL;
            rPL.u2Profile = 0;
            rPL.u2Level = 0;
            if (VENC_OK == VENC_GetParam(handle, VENC_H264_PRAM_PROFILE_LEVEL, (VOID*)&rPL))
            {
                LOG(0, "\tProfile:%s, Level:%d.%d\n", rPL.u2Profile? "main":"baseline" , rPL.u2Level/10, rPL.u2Level%10);
            }
        }
            break;
        }

        u4Idx++;
    }

    if (!u4Idx)
    {
        LOG(0, "Venc Instance :None\n", u4Idx);
    }
    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 0;
}


static VOID venc_demo_cb(VOID *pvArg, VENC_IN_PIC_T * ptIn, VENC_OUT_PIC_T * ptOut, UINT32 type)
{
    if (ptIn)
    {
        VENC_FRM_T rFrm;
        rFrm.u4YAddr  = ptIn->u4YAddr;
        rFrm.u4UVAddr = ptIn->u4CAddr;
        _BUFMGR_OBJ_Free(_ptFrmBuffMgr, (VOID *)&rFrm);
        LOG(7, "%s free: src(Y,UV) => (0x%08x, 0x%08x)\n", __FUNCTION__, ptIn->u4YAddr, ptIn->u4CAddr);
    }

    if (ptOut)
    {
        LOG(7, "%s out: (type, addr, size) => (%d, 0x%08x, 0x%08x)\n", __FUNCTION__, type, ptOut->u4Addr, ptOut->u4Size);
#ifndef VENC_TO_MUXER_IF
        VENC_UnlockBuf(_hVenc, ptOut->u4Addr, ptOut->u4Size);
#endif
    }

#ifdef VENC_TO_MUXER_IF
    if (ptOut)
    {
        VENC_PES_INFO_T rVEncPes;
        x_memset(&rVEncPes, 0, sizeof(rVEncPes));
        rVEncPes.u4FifoStart = ptOut->u4RStart;
        rVEncPes.u4FifoEnd = ptOut->u4REnd;
        rVEncPes.u4StartAddr = ptOut->u4Addr;
        rVEncPes.u4FrameSize = ptOut->u4Size;
        rVEncPes.u4VideoType = type;
        rVEncPes.u4Pts = ptOut->u4Pts;
        rVEncPes.pvTag       = _hVenc;
        MUXER_SendVideoPes(0, &rVEncPes);
    }
#endif
 
    UNUSED(pvArg);
    UNUSED(type);
}


#ifdef VENC_EMU

enum
{
    E_T32_NOP,
    E_T32_EXEC,
    E_T32_RESTART,
    E_T32_EXIT,
    E_T32_EXEC_RELOAD_SYM,
    E_T32_DETECT_HOST,
    E_T32_END
};


INT32 _t32_HostBreakPT(INT32 i4TargetFlag, CHAR *szHostCommand)
{
    /*
    __asm
    {
        nop
    }
    */
    UNUSED(szHostCommand);
    return i4TargetFlag;
}

INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...)
{
    CHAR szBuffer[256];
    VA_LIST t_ap;

    VA_START(t_ap, szFormat);
    UNUSED(x_vsprintf(szBuffer, szFormat, t_ap));
    VA_END(t_ap);

    UNUSED(_t32_HostBreakPT( i4TargetFlag, szBuffer ));

    return 0;
}


//-----------------------------------------------------------------------------
/** _VEncLoadFile
 *  Load file to dram
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _VEncLoadFile(CHAR *pcFileName, UINT32 u4Addr)
{
    //Printf("d.load.b \"%s\" 0x%08x /ny\n", pcFileName, u4Addr);
    LOG(7, "d.load.b \"%s\" 0x%08x /ny\n", pcFileName, u4Addr);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.b \"%s\" 0x%08x /ny",
                pcFileName, u4Addr));

    return TRUE;
}


#ifdef USB_SUPPORT
BOOL _VEncLoadUsbFile(CHAR *pcFileName, UINT32 u4Addr)
{
    UINT64 u8FileSize, u8Tmp;
    UINT32 u4Request, u4Read;
    HANDLE_T  h_file;

    VERIFY(FMR_OK == x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777, FALSE, &h_file));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_END, &u8FileSize));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_BGN, &u8Tmp));

    while (u8FileSize > 0)
    {
        if (u8FileSize >> 32)
        {
            u4Request = 0xffffffff;
        }
        else
        {
            u4Request = (UINT32)u8FileSize;
        }
        x_fm_read(h_file, (VOID *)u4Addr, u4Request, &u4Read);
        ASSERT(u4Request == u4Read);
        u4Addr += u4Read;
        u8FileSize -= u4Read;
    }

    VERIFY(FMR_OK == x_fm_close(h_file));

    return TRUE;
}
#endif

//-----------------------------------------------------------------------------
void getCFGPathfile( UCHAR *pBuf, CFGFILE *ptCfgFile)
{
    UINT32 offset;
    UINT32 offset_Item;
    char ucCnt;
#ifdef USB_SUPPORT
    char TmpInputFileName[512];
#endif

    offset = 0;

    for(ucCnt=0; ucCnt< 7/*5*/; ucCnt++)
    {
        offset_Item = 0;
        while(!(0x0D == *(char *)(pBuf+ offset) && 0x0A == *(char *)(pBuf+ offset+1)) && (!(0x0A == *(char *)(pBuf+ offset))) )
        {
            switch(ucCnt)
            {
            case 0:
#ifdef USB_SUPPORT
                TmpInputFileName[offset_Item]= *(char *)(pBuf+ offset);
#else
                ptCfgFile->InputFileName[offset_Item]= *(char *)(pBuf+ offset);
#endif
                break;
            case 1:
                ptCfgFile->GoldenFileName[offset_Item]= *(char *)(pBuf+ offset);
                break;
            case 2:
                ptCfgFile->MMRFileName[offset_Item]= *(char *)(pBuf+ offset);
                break;
            case 3:
                ptCfgFile->OutputFileName[offset_Item]= *(char *)(pBuf+ offset);
                break;
            case 4:
                ptCfgFile->fgSaveOutputFile = 0;
                ptCfgFile->CRCFileName[offset_Item]= *(char *)(pBuf+ offset);
                break;
            case 5:
                //ptCfgFile->fgSaveOutputFile = *(char *)(pBuf+ offset);
                ptCfgFile->fgSaveOutputFile = 0;
                ptCfgFile->KeyFrameFileName[offset_Item]= *(char *)(pBuf+ offset);
                break;
            case 6:
                ptCfgFile->fgSaveOutputFile = 0;
            default:
                break;
            }

            if (!pBuf[offset])
            {
                break;
            }
            offset ++;
            offset_Item ++;

        }
        if (0x0 == *(char *)(pBuf+offset))
        {

        }
        else if(0x0A == *(char *)(pBuf+offset))
        {
            offset ++;//*addr = *addr + 1;
        }
        else
        {
            offset +=2;//*addr= *addr + 2;
        }

        switch(ucCnt)
        {
        case 0:
#ifndef USB_SUPPORT
            ptCfgFile->InputFileName[offset_Item]= '\0';
#else
            {
                UINT32 i;
#ifdef SINGLE_FILE_SUPPORT
                x_strncpy(&TmpInputFileName[offset_Item - 1], ".yuv", 4);
#else
                TmpInputFileName[offset_Item]= '\0';
#endif
                for (i = 3; TmpInputFileName[i] != '\0'; i++)
                {
                    if (TmpInputFileName[i] == '\\')
                    {
                        TmpInputFileName[i] = '/';
                    }
                }
                x_sprintf(ptCfgFile->InputFileName, "/mnt/usb_0/%s", &TmpInputFileName[3]);
            }
#endif
          #if VENC_LOCAL_DISC_ID_ENABLE
            #ifndef USB_SUPPORT
              ptCfgFile->InputFileName[0] = VENC_LOCAL_DISC_ID;
            #endif
          #endif
            break;
        case 1:
            ptCfgFile->GoldenFileName[offset_Item]= '\0';
          #if VENC_LOCAL_DISC_ID_ENABLE
            ptCfgFile->GoldenFileName[0] = VENC_LOCAL_DISC_ID;
          #endif
            break;
        case 2:
            ptCfgFile->MMRFileName[offset_Item]= '\0';
        #if VENC_LOCAL_DISC_ID_ENABLE
            ptCfgFile->MMRFileName[0] = VENC_LOCAL_DISC_ID;
        #endif
            break;
        case 3:
            ptCfgFile->OutputFileName[offset_Item]= '\0';
          #if VENC_LOCAL_DISC_ID_ENABLE
            ptCfgFile->OutputFileName[0] = VENC_LOCAL_DISC_ID;
          #endif
            break;
        case 4:
            ptCfgFile->CRCFileName[offset_Item]= '\0';
          #if VENC_LOCAL_DISC_ID_ENABLE
            ptCfgFile->CRCFileName[0] = VENC_LOCAL_DISC_ID;
          #endif
            break;
        case 5:
            ptCfgFile->KeyFrameFileName[offset_Item]= '\0';
          #if VENC_LOCAL_DISC_ID_ENABLE
            ptCfgFile->KeyFrameFileName[0] = VENC_LOCAL_DISC_ID;
          #endif
        case 6:
        default:
            break;
        }

    }

}

//-----------------------------------------------------------------------------
char getCFGFileList( UCHAR *pBuf, UINT32 *addr, char *u4Val)
{
    UINT32 offset;
    UINT32 offset_Path;
    offset = *addr;
    offset_Path = 0;
    //sscanf(pBuf+offset, "%s", u4Val);
    if((0x2A == *(char *)(pBuf+ offset) && 0x2A == *(char *)(pBuf+ offset+1)))
    {
        return 0;
    }

    while(!(0x0D == *(char *)(pBuf+ offset) && 0x0A == *(char *)(pBuf+ offset+1)) && (!(0x0A == *(char *)(pBuf+ offset))) )
    {
        *(char *)(u4Val + offset_Path)=*(char *)(pBuf+ offset);
        offset ++;
        offset_Path ++;
    }
    if(0x0A == *(char *)(pBuf+offset))
    {
        offset ++;//*addr = *addr + 1;
    }
    else
    {
        offset +=2;//*addr= *addr + 2;
    }
    *(u4Val + offset_Path) = '\0';
    *addr = offset;
    //return offset;
    return 1;
}

//-----------------------------------------------------------------------------
//ks add for 8555 proj
void getMMRfile(CHAR *pBuf, UINT32 *addr, UINT32 *u4Val)
{
    UINT32 offset;
    offset = *addr;
    while(!(0x0D == *(char *)(pBuf+ offset) && 0x0A == *(char *)(pBuf+ offset+1)) && (!(0x0A == *(char *)(pBuf+ offset))) )
    {
        offset ++;
    }
    if(0x0A == *(char *)(pBuf+offset))
    {
        offset ++;//*addr = *addr + 1;
    }
    else
    {
        offset +=2;//*addr= *addr + 2;
    }
    x_sscanf(pBuf+offset, "%x", u4Val);
    //sscanf(pBuf+offset, "%x", u4Val);

    *addr = offset;
    //return offset;
}


static INT32 _VEncCmdLoadEmu(INT32 i4Argc, const CHAR ** szArgv)
{
    //h264
    _VEncH264LoadEmu();

    pfnVEncCmdEmuItem = (PFNVEncCmdEmuItem)(_VEncH264GetEmuItem());

    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 0;
}

UINT8 *_pu1FListTempBuf;
UINT8 *_pu1FPathTempBuf;
CHAR *_pcFListPathName;

VOID _VEncEmuLoop(VOID *pvArg)
{
    UINT32 u4FListIdx;
    UINT8 *pu1FListTempBuf;
    UINT8 *pu1FPathTempBuf;
    CHAR *pcFListPathName;

    Printf("Start to Verify Video Encoder...\n");

    if (_pu1FListTempBuf == NULL)
    {
        _pu1FListTempBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(0x32000, 32);
    }
    pu1FListTempBuf = _pu1FListTempBuf;
    if (_pu1FPathTempBuf == NULL)
    {
        _pu1FPathTempBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(0x2800, 32);
    }
    pu1FPathTempBuf = _pu1FPathTempBuf;
    if (_pcFListPathName == NULL)
    {
        _pcFListPathName = (CHAR *)BSP_AllocAlignedDmaMemory(512, 32);
    }
    pcFListPathName = _pcFListPathName;

    ASSERT(pu1FListTempBuf);
    ASSERT(pu1FPathTempBuf);
    ASSERT(pcFListPathName);

    Printf("loading FileList file ...\n");
    _VEncLoadFile(_acVEncEmuFListFileName, (UINT32)pu1FListTempBuf);
    u4FListIdx = 0;
    ASSERT(pfnVEncCmdEmuItem);

    while (getCFGFileList(pu1FListTempBuf, &u4FListIdx, pcFListPathName))
    {
    #if VENC_LOCAL_DISC_ID_ENABLE
        *pcFListPathName = VENC_LOCAL_DISC_ID;
        Printf("pcFListPathName %s\n", __LINE__, pcFListPathName);
    #endif
        pfnVEncCmdEmuItem(pu1FPathTempBuf, pcFListPathName);
    }

    //BSP_FreeAlignedDmaMemory(pu1FListTempBuf);
    //BSP_FreeAlignedDmaMemory(pu1FPathTempBuf);
    //BSP_FreeAlignedDmaMemory(pcFListPathName);

    Printf("Verification done!\n");

    UNUSED(pvArg);
    x_thread_exit();
}

VOID _VEncCmdEmuList(const CHAR *pcFileList)
{
    HANDLE_T hEmuThrd;

    HalFlushInvalidateDCache();

    x_strcpy(_acVEncEmuFListFileName, pcFileList);

    // create emulation thread
    VERIFY(x_thread_create(&hEmuThrd,
        "Emulation Thread", VENC_STACK_SIZE, VENC_THREAD_PRIORITY,
        (x_os_thread_main_fct)_VEncEmuLoop, 0, NULL) == OSR_OK);
}

extern VOID _VEncCmdEmuImgrz(UINT8 *pu1FPathTempBuf, CHAR *pcFListPathName);
VOID _VEncEmuImgrzLoop(VOID *pvArg)
{
    UINT32 u4FListIdx;
    UINT8 *pu1FListTempBuf;
    UINT8 *pu1FPathTempBuf;
    CHAR *pcFListPathName;

    Printf("Start to Verify image resizer for Video Encoder...\n");

    if (_pu1FListTempBuf == NULL)
    {
        _pu1FListTempBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(0x32000, 32);
    }
    pu1FListTempBuf = _pu1FListTempBuf;
    if (_pu1FPathTempBuf == NULL)
    {
        _pu1FPathTempBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(0x2800, 32);
    }
    pu1FPathTempBuf = _pu1FPathTempBuf;
    if (_pcFListPathName == NULL)
    {
        _pcFListPathName = (CHAR *)BSP_AllocAlignedDmaMemory(512, 32);
    }
    pcFListPathName = _pcFListPathName;

    ASSERT(pu1FListTempBuf);
    ASSERT(pu1FPathTempBuf);
    ASSERT(pcFListPathName);

    Printf("loading FileList file ...\n");
    _VEncLoadFile(_acVEncEmuFListFileName, (UINT32)pu1FListTempBuf);
    u4FListIdx = 0;
    while (getCFGFileList(pu1FListTempBuf, &u4FListIdx, pcFListPathName))
    {
    #if VENC_LOCAL_DISC_ID_ENABLE
        *pcFListPathName = VENC_LOCAL_DISC_ID;
    #endif
        _VEncCmdEmuImgrz(pu1FPathTempBuf, pcFListPathName);
    }

    Printf("Verification done!\n");

    UNUSED(pvArg);
    x_thread_exit();
}


VOID _VEncCmdEmuImgrzList(const CHAR *pcFileList)
{
    HANDLE_T hEmuThrd;

    x_strcpy(_acVEncEmuFListFileName, pcFileList);

    // create emulation thread
    VERIFY(x_thread_create(&hEmuThrd,
        "Emulation Imgrz Thread", VENC_STACK_SIZE, VENC_THREAD_PRIORITY,
        (x_os_thread_main_fct)_VEncEmuImgrzLoop, 0, NULL) == OSR_OK);
}

#endif


#if defined(LINUX_TURNKEY_SOLUTION) && !defined(CC_DRIVER_DEMO)
static INT32 _VEncCmdToggleMtalLog(INT32 i4Argc, const CHAR ** szArgv)
{
#define MTVENC_LOG_MASK (1 << 7) // should align with macro in mttype.h
    extern unsigned long u4MtalLogMask2;
    UINT32 u4LogMaskBits = (i4Argc < 2)
        ? MTVENC_LOG_MASK
        : StrToInt(szArgv[1]);
    u4MtalLogMask2 ^= u4LogMaskBits;
    LOG(0, "u4MtalLogMask2=0x%x\n", u4MtalLogMask2);

    return 0;
}
#endif


VENC_CTRL_THUMBNAIL_T _rThumbnail;
static VOID _VEncCmdGetThumbnailCb(VOID *pvTag, UINT32 u4PhyAddr)
{
    UINT32 u4VirAddr = VIRTUAL(u4PhyAddr);
    UINT32 u4Size = _rThumbnail.u4Width * _rThumbnail.u4Height * 4;
    ASSERT(pvTag == _hVenc);
    LOG(0, "%p is map to %p\n", u4PhyAddr, u4VirAddr);
    LOG(0, "d.save.b G:\\ARGB.raw 0x%08x--0x%08x\n", u4VirAddr, u4VirAddr + u4Size - 1);
}


static INT32 _VEncCmdGetThumbnail(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Ret;

    _rThumbnail.u4Width = 160;
    _rThumbnail.u4Height= 120;
    _rThumbnail.pvTag = _hVenc;
    _rThumbnail.cb = _VEncCmdGetThumbnailCb;
    u4Ret = VENC_SetCtrl(_hVenc, VENC_CTRL_GET_THUMBNAIL, (VOID*)&_rThumbnail);
    LOG(0, "VENC_CTRL_GET_THUMBNAIL return %d\n", u4Ret);

    return 0;
}


static INT32 _VEncCmdToggleRc(INT32 i4Argc, const CHAR ** szArgv)
{
    extern BOOL _fgFixQp;
    _fgFixQp = _fgFixQp? FALSE : TRUE;
    LOG(0, "rc is %s\n", _fgFixQp? "off" : "on");

    return 0;
}


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
CLI_EXEC_T * getVEncCmdTbl(void)
{
    return _arVEncCmdTbl;
}


