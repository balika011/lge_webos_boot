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
/*****************************************************************************
*  Video Plane: Diagnostic command
*****************************************************************************/

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "fbm_drvif.h"
#include "srm_drvif.h"

#ifdef __KERNEL__
#include "source_table.h"  // nptv

#include <linux/fb.h>
#endif

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_mid.h"

#define DEFINE_IS_LOG   CLI_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END

LINT_SUPPRESS_BRACE(534)            // Warning 534: Ignoring return value of function
LINT_SUPPRESS_BRACE(818)            // Info 818: Pointer parameter 'szArgv'


/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _FbmInitCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmCreateCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmReleaseCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmResetCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmQueryCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmCStatusCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmMidCmd (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _FbmColorCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmCheckCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmChangeCmd (INT32 i4Argc, const CHAR ** szArgv);

#ifdef FBM_FB_LOG
static INT32 _FbmEnFbDebugLog (INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32 _FbmQueryQueInfoCmd (INT32 i4Argc, const CHAR ** szArgv);


#ifdef __KERNEL__
static INT32 _FbmShowFrameBufferCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmSaveDramBwModeCmd (INT32 i4Argc, const CHAR ** szArgv);
#endif

#ifdef FBM_VIRTUAL_DISPLAY
static INT32 _FbmVirtualDispStart (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmVirtualDispStop (INT32 i4Argc, const CHAR ** szArgv);
#endif
#ifdef FBM_VIRTUAL_MPEG
static INT32 _FbmVirtualMpegStart (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FbmVirtualMpegStop (INT32 i4Argc, const CHAR ** szArgv);
#endif

/******************************************************************************
* CLI Command Table
******************************************************************************/
LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(FBM)
LINT_RESTORE

#ifdef FBM_VIRTUAL_DISPLAY
static CLI_EXEC_T _arFbmVirtualDisplayCmdTbl[] =
{
    {"start",       NULL,   _FbmVirtualDispStart, NULL, "start", CLI_SUPERVISOR},
    {"stop",        NULL,   _FbmVirtualDispStop, NULL,  "stop", CLI_SUPERVISOR},
    DECLARE_END_ITEM(),
};
#endif

#ifdef FBM_VIRTUAL_MPEG
static CLI_EXEC_T _arFbmVirtualMpegCmdTbl[] =
{
    {"start",       NULL,   _FbmVirtualMpegStart, NULL, "start", CLI_SUPERVISOR},
    {"stop",        NULL,   _FbmVirtualMpegStop, NULL,  "stop", CLI_SUPERVISOR},
    DECLARE_END_ITEM(),
};
#endif

static CLI_EXEC_T _arFbmCmdTbl[] =
{
    {"init",        "i",    _FbmInitCmd, NULL,          "Fbm init", CLI_SUPERVISOR},
    {"query",       "q",    _FbmQueryCmd, NULL,         "Fbm status query", CLI_GUEST},
    {"mid",         "mid",  _FbmMidCmd, NULL,           "Fbm mid setting", CLI_SUPERVISOR},
    {"status",      "s",    _FbmCStatusCmd, NULL,       "Fbm status",       CLI_SUPERVISOR},

    {"create",      "c",    _FbmCreateCmd, NULL,        "Fbm create group", CLI_GUEST},
    {"release",     "r",    _FbmReleaseCmd, NULL,       "Fbm release group", CLI_GUEST},
    {"reset",       "rst",  _FbmResetCmd, NULL,         "Fbm reset group", CLI_SUPERVISOR},
    {"color",       NULL,   _FbmColorCmd, NULL,         "Fbm set color", CLI_SUPERVISOR},
    {"check",       "ck",   _FbmCheckCmd, NULL,         "Fbm check pool", CLI_SUPERVISOR},
	{"change",		"cg",	_FbmChangeCmd, NULL,		"Fbm change pool", CLI_SUPERVISOR},
    {"queryQue",    "que",  _FbmQueryQueInfoCmd, NULL,  "Fbm Que Empty/Display que info", CLI_SUPERVISOR},        
#ifdef FBM_FB_LOG
	{"enable",		"en",	_FbmEnFbDebugLog, NULL,		"Enable debug fb status log", CLI_GUEST},
#endif

#ifdef __KERNEL__
    {"fb",          "fb",   _FbmShowFrameBufferCmd, NULL,   "Fbm Show Frame Buffer Info", CLI_SUPERVISOR},
    {"savebw",      "save bw mode",   _FbmSaveDramBwModeCmd, NULL,   "Fbm save dram bw mode", CLI_SUPERVISOR},
#endif

#ifdef FBM_VIRTUAL_DISPLAY
    {"vd",          NULL,   NULL, _arFbmVirtualDisplayCmdTbl,   "Virtual display", CLI_SUPERVISOR},
#endif

#ifdef FBM_VIRTUAL_MPEG
    {"vm",          NULL,   NULL, _arFbmVirtualMpegCmdTbl,      "Virtual MPEG decoder", CLI_SUPERVISOR},
#endif

LINT_SAVE_AND_DISABLE
    CLIMOD_DEBUG_CLIENTRY(FBM),
LINT_RESTORE

    DECLARE_END_ITEM(),
};

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Fbm)
{
    "fbm",
    NULL,
    NULL,
    _arFbmCmdTbl,
    "Frame buffer manager command",
    CLI_GUEST
};
LINT_RESTORE


/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _FbmInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    FBM_Init();
    SRM_Init();

    return 0;
}

static INT32 _FbmCreateCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR  ucFbgType = 0;
    UINT32 u4Fmt = 0;
    UINT32 u4Hsize, u4Vsize;
    UCHAR  ucFbgId = 0;
	BOOL   bIs10Bit = FALSE;
	BOOL   bIsUFO = FALSE;
    FBM_CREATE_FBG_PAR_T rPar;
    
    x_memset(&rPar, 0, sizeof(FBM_CREATE_FBG_PAR_T));
    rPar.u1VdecId = (UINT8)0;
    rPar.u1AppMode = FBM_FBG_APP_NORMAL;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: Fmt(1-H264 2-MPEG2 3-MPEG4 5-JPEG 6-RV 7-AVS 8-RAW 9-VP6 10-VP8 11-H265) W() H() 10BIT(0/1) UFO(0/1) .\n");
        return 0;
    }

    u4Fmt       = (i4Argc > 1) ? (UINT32)StrToInt(szArgv[1]) : FBM_VDEC_H264;
    u4Hsize     = (i4Argc > 2) ? (UINT32)StrToInt(szArgv[2]) : 1920;
    u4Vsize     = (i4Argc > 3) ? (UINT32)StrToInt(szArgv[3]) : 1080;
    //rPar.fgCreateFromInst = (i4Argc > 4) ? (BOOL)StrToInt(szArgv[4]) : FALSE;
	bIs10Bit   =  (i4Argc > 4) ? (BOOL)StrToInt(szArgv[4]) : FALSE;
	bIsUFO     =  (i4Argc > 5) ? (BOOL)StrToInt(szArgv[5]) : FALSE;

    ucFbgType = FBM_SelectGroupType(u4Hsize, u4Vsize);

    if((ucFbgType == 25 /*FBM_FBG_TYPE_4K2K_JPEG*/) || (u4Fmt == 5 /*FBM_VDEC_JPEG */))
    {
        FBM_SetColorMode(FBM_CM_422);
    }
	rPar.fg10Bit = bIs10Bit;
	rPar.fgUFO = bIsUFO;
    ucFbgId = FBM_CreateGroupExt(ucFbgType, u4Fmt, u4Hsize, u4Vsize, &rPar);

    return ucFbgId;
}


static INT32 _FbmReleaseCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    // arg: (UCHAR ucFbgId)
    UCHAR ucFbgId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: ucFbgId\n");
        return 0;
    }

    ucFbgId = (UCHAR) StrToInt(szArgv[1]);

    FBM_ReleaseGroup(ucFbgId);
    return 0;
}

static INT32 _FbmResetCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    // arg: (UCHAR ucFbgId)
    UCHAR ucFbgId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: ucFbgId\n");
        return 0;
    }

    ucFbgId = (UCHAR) StrToInt(szArgv[1]);

    FBM_ResetGroup(ucFbgId);
    return 0;
}

static FBM_POOL_TYPE_T _MidFbmMapping(MID_AGENT_ID_T aeAgents)
{
    switch(aeAgents)
    {
        case MID_AGENT_PSCAN:
            return FBM_POOL_TYPE_MDDI_MAIN;
        case MID_AGENT_SCPOS:
            return FBM_POOL_TYPE_SCPOS_MAIN;
#if (defined(CC_MT5396) || defined(CC_MT5398) || defined(CC_MT5399)||defined(CC_MT5890))
        case MID_AGENT_MJCIN:
            return FBM_POOL_TYPE_MJC_1;
        case MID_AGENT_VENC:
            return FBM_POOL_TYPE_VENC;
#endif
        default:
            return FBM_POOL_TYPE_TOTAL;
    }
}

static INT32 _FbmMidCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Module;
    UINT8 idx = 0;
    const CHAR* szAgent;
    FBM_POOL_T *prPool, *prPool2;
    UINT32 u4LowAddr = 0, u4HiAddr = 0, u4ChA = 1;

    MID_AGENT_ID_T aeAgents[] =
    {
        MID_AGENT_PSCAN, // = 4,
        MID_AGENT_SCPOS, // = 7,
#if (defined(CC_MT5396) || defined(CC_MT5398) || defined(CC_MT5399)||defined(CC_MT5890))       
        MID_AGENT_VENC,  // = 12,
        MID_AGENT_MJCIN  // = 13,
#endif
    };

    if (i4Argc != 2)
    {
        for (idx = 0; idx < sizeof(aeAgents)/sizeof(MID_AGENT_ID_T); idx++)
        {
            szAgent = MID_AgentIdToString(aeAgents[idx]);
            Printf("  %2u: %s - ", idx, szAgent);

            prPool = FBM_GetPoolInfoAuto((UCHAR)_MidFbmMapping(aeAgents[idx]), NULL);
            if(prPool != NULL)
            {
                u4LowAddr = prPool->u4Addr;
                u4HiAddr  = prPool->u4Size + u4LowAddr;
            }
            Printf("Addr %X to %X\n", u4LowAddr, u4HiAddr);
        }
        return 0;
    }

    u1Module = (UINT8) StrToInt(szArgv[1]);
    if(u1Module >= sizeof(aeAgents)/sizeof(MID_AGENT_ID_T))
    {
        Printf("Invalid Module!!\n");
        return 0;
    }

    prPool  = FBM_GetPoolInfoAuto((UCHAR)_MidFbmMapping(aeAgents[u1Module]), NULL);
    prPool2 = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_TOTAL2, NULL);
    if(prPool != NULL)
    {
        u4LowAddr = prPool->u4Addr;
        u4HiAddr  = prPool->u4Size + u4LowAddr;
    }
    Printf("%s addr %X to %X\n", MID_AgentIdToString(aeAgents[u1Module]), u4LowAddr, u4HiAddr);

    if(prPool2 != NULL) // MJC
    {
        u4ChA = (u4LowAddr <= prPool2->u4Addr);
        if (!u4ChA)
        {
            u4LowAddr -= prPool2->u4Addr;
            u4HiAddr  -= prPool2->u4Addr;
            Printf("Channel B addr %X to %X\n", u4LowAddr, u4HiAddr);
        }
    }
    
    MID_SetDRAMChannel(u4ChA); // 1 for Channel A, 0 for Channel B

    //mid.d 4
    MID_DisableRegionProtect(4);
    //mid.rr 4
    MID_ResetRegion(4);

    MID_SetRegionEx2(4, aeAgents[u1Module], u4LowAddr, u4HiAddr, 0, 0);  
    //mid.e 4
    MID_EnableRegionProtect(4);
    //mid.ie 1
    //BIM_EnableIrq(VECTOR_DRAMC);

    MID_SetDRAMChannel(TRUE); // channel A

    return 0;
}

static INT32 _FbmQueryCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucFbgId = 0xff;
    UNUSED(i4Argc);
    UNUSED(szArgv);
    ucFbgId = (i4Argc > 1) ? (UCHAR) StrToInt(szArgv[1]) : 0xff;

    FBM_QueryStatus(ucFbgId);
    return 0;
}
static INT32 _FbmQueryQueInfoCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucQueType = 0;
    UCHAR ucFbgId = 0xff;
    
    if ((i4Argc < 0) || (szArgv == NULL))
    {
        LOG(0,"Arg error\n");
        return 0;
    }    
    if (i4Argc == 1)
    {
        LOG(0,"QueType 1: emptyQue;2:Display Que\n");
        return 0;
    }
    if (i4Argc > 1)
    {
        ucQueType = (UCHAR) StrToInt(szArgv[1]);
        ucFbgId   = (UCHAR) StrToInt(szArgv[2]);
    
        if (ucQueType == 1)
        {
        	#if (defined(CC_MT5882) || defined(CC_MT5861)||defined(CC_MT5890))
            _FbmQueryFromEmptyQ(ucFbgId);
          #endif
            return 0;
        }
        else if (ucQueType == 2)
        {
        	#if (defined(CC_MT5882) || defined(CC_MT5861)||defined(CC_MT5890))        	
            _FbmQueryFromDisplayQ(ucFbgId);
          #endif
            return 0;       
        }
        else if (ucQueType == 3)
        {
#ifdef FBM_FB_LOG        
            FBM_FlushFbLog(0xFC, 0xFF);
#endif            

            return 0;       
        }        
        else
        {
            LOG(0,"ucQueType %d other case\n");
            return 0;
        }
    }
    return 0;
    
}

static INT32 _FbmCStatusCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    SRM_CStatus();
    return 0;
}


static INT32 _FbmColorCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    // arg: (UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Color)
    UCHAR ucFbgType;
    UCHAR ucFbId;
    UINT32 u4Color;

    if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        Printf("Arg: ucFbgType ucFbId u4Color\n");
        return 0;
    }

    ucFbgType = (UCHAR) StrToInt(szArgv[1]);
    ucFbId = (UCHAR) StrToInt(szArgv[2]);
    u4Color = StrToInt(szArgv[3]);

    FBM_SetFrameBufferColor(ucFbgType, ucFbId, u4Color);

    return 0;
}


static INT32 _FbmCheckCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    FBM_CheckPadding();
	#if defined(CC_MT5399)||defined(CC_MT5890)
	FBM_CheckBufferdefine();
    #endif
    return 0;
}
extern UINT32 volatile _u1FBMmode;

static INT32 _FbmChangeCmd (INT32 i4Argc, const CHAR ** szArgv)
{

	 if ((i4Argc < 1) || (szArgv == NULL))
    {
        Printf("current :_u1FBMmode %d \n",_u1FBMmode);
        return 0;
    }
    _u1FBMmode = StrToInt(szArgv[1]);
    return 0;
}


#ifdef FBM_FB_LOG

extern BOOL  g_bEnableDebugLog;

static INT32 _FbmEnFbDebugLog (INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc >= 1)
    {
        g_bEnableDebugLog = (BOOL) StrToInt(szArgv[1]);
        Printf("[Enable/Disable(%d)] debug Fb status \n",g_bEnableDebugLog);
    }
    else
    {
        Printf("Enable/Disable(1/0) debug Fb status \n");
    }
    return 0;
}

#endif



#ifdef __KERNEL__
static INT32 _FbmShowFrameBufferCmd (INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
LINT_SAVE_AND_DISABLE
    SRM_VDP_INFO_T* prVdpInfo;
    FBM_POOL_T* prFbmPool;
    UCHAR ucPool;

    struct fb_info* pFb;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    // Main Video Plane
    prVdpInfo = SRM_GetVdpInfo(0);

    if (prVdpInfo == NULL)
    {
        return 0;
    }

    if (prVdpInfo->u4Enable != 0)
    {
        Printf("Main Video Frame Buffer\n");

        if (prVdpInfo->u4Source == (UINT32) VSS_DTV)
        {
            prFbmPool = FBM_GetPoolMpeg();

            if (prFbmPool == NULL)
            {
                return 0;
            }

            Printf("MPEG Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);

            prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DMX);

            if (prFbmPool == NULL)
            {
                return 0;
            }

            Printf("Video-ES Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);
        }
        else
        if (prVdpInfo->u4Source <= (UINT32) VSS_CVBS)
        {
            prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC);

            if (prFbmPool == NULL)
            {
                return 0;
            }

            Printf("3D Comb Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);
        }

        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_NR);

        if (prFbmPool == NULL)
        {
            return 0;
        }

        Printf("NR Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);

        if (prVdpInfo->u4Interlace != 0)
        {
            ucPool = FBM_GetPoolCurrent(FBM_POOL_TYPE_MDDI_MAIN);
            prFbmPool = FBM_GetPoolInfo(ucPool);

            if (prFbmPool == NULL)
            {
                return 0;
            }

            Printf("Deinterlacer Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);
        }

        ucPool = FBM_GetPoolCurrent(FBM_POOL_TYPE_SCPOS_MAIN);
        prFbmPool = FBM_GetPoolInfo(ucPool);

        if (prFbmPool == NULL)
        {
            return 0;
        }

        Printf("Scaler Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);

        Printf("\n");
    }

    // Pip Video Plane
    prVdpInfo = SRM_GetVdpInfo(1);

    if (prVdpInfo == NULL)
    {
        return 0;
    }

    if (prVdpInfo->u4Enable != 0)
    {
        Printf("Pip Video Frame Buffer\n");

        if (prVdpInfo->u4Source == (UINT32) VSS_DTV)
        {
            prFbmPool = FBM_GetPoolMpeg();

            if (prFbmPool == NULL)
            {
                return 0;
            }

            Printf("MPEG Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);

            prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DMX);

            if (prFbmPool == NULL)
            {
                return 0;
            }

            Printf("Video-ES Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);
        }
        else
        if (prVdpInfo->u4Source <= (UINT32) VSS_CVBS)
        {
            prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC);

            if (prFbmPool == NULL)
            {
                return 0;
            }

            Printf("3D Comb Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);
        }

        prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_PIP);

        if (prFbmPool == NULL)
        {
            return 0;
        }

        Printf("Scaler Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);

        Printf("\n");
    }

    prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);

    if (prFbmPool == NULL)
    {
        return 0;
    }

    Printf("Audio DSP Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);

    pFb = mt53fb_get_fb_info();

    if (pFb == NULL)
    {
        return 0;
    }

    Printf("OSD/GFX/Image Addr=(0x%x), Size=(0x%x)\n", pFb->fix.smem_start, pFb->fix.smem_len);

#if 0
    prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG_OSD1);

    if (prFbmPool == NULL)
    {
        return 0;
    }

    Printf("GFX/OSD #1 Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);

    prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG_OSD2);

    if (prFbmPool == NULL)
    {
        return 0;
    }

    Printf("GFX/OSD #2 Addr=(0x%x), Size=(0x%x)\n", prFbmPool->u4Addr, prFbmPool->u4Size);
#endif

    return 0;
LINT_RESTORE
#else

    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    return 0;
#endif    
}
#endif

#ifdef __KERNEL__
static INT32 _FbmSaveDramBwModeCmd (INT32 i4Argc, const CHAR ** szArgv)
{
LINT_SAVE_AND_DISABLE
#ifdef CC_DOWN_BW_WITH_MENU 
    extern UINT32 VDP_SetDownBWMode(UINT8 u1OnOff); 
    UINT32 u4SaveBwOn;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: SaveBwOnOff\n");
        return 0;
    }

    u4SaveBwOn = StrToInt(szArgv[1]);

    UNUSED(VDP_SetDownBWMode((UINT8) u4SaveBwOn));
#endif // 
LINT_RESTORE
   
    return 0;
}
#endif


#ifdef FBM_VIRTUAL_DISPLAY
static INT32 _FbmVirtualDispStart (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Delay;
    UINT32 u4VirtualDispPath = 0;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: u4Delay Path(None/0:All, 1:Main, 2:Sub)\n");
        return 0;
    }

    u4Delay = StrToInt(szArgv[1]);
    if(i4Argc < 4)
    {
        u4VirtualDispPath = StrToInt(szArgv[2]);
    }

    FBM_VirtualDispStart(u4Delay, u4VirtualDispPath);
    return 0;
}

static INT32 _FbmVirtualDispStop (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    FBM_VirtualDispStop();
    return 0;
}
#endif

#ifdef FBM_VIRTUAL_MPEG
static INT32 _FbmVirtualMpegStart (INT32 i4Argc, const CHAR ** szArgv)
{
   
   UINT32 u4Delay;
   const CHAR* pcFile;
   
   if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1]  == NULL) || (szArgv[2] == NULL))
   {
	   Printf("Arg: (1)u4Delay (2)pcFile \n");
	   return 0;
   }
   
   u4Delay = StrToInt(szArgv[1]);
   pcFile = szArgv[2];
   
   FBM_VirtualMpegStart(u4Delay, pcFile);
   return 0;
   
}

static INT32 _FbmVirtualMpegStop (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    FBM_VirtualMpegStop();
    return 0;
}
#endif

#if 0
/******************************************************************************
* Global Function
******************************************************************************/
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetFbmMwCmdTbl(void)
#else
CLI_EXEC_T* GetFbmCmdTbl(void)
#endif
{
    return &_rFbmModCmdTbl;
}
#endif

