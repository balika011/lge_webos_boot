/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
//#include <stdlib.h>
//#include "sys_config.h"
//#include "drv_config.h"
#include "x_printf.h"
#include "x_debug.h"
#include "drv_common.h"
//#include "drv_thread.h"
#include "x_common.h"
#include "x_os.h"
//#include "x_bim.h"
#include "x_timer.h"
#include "x_drv_cli.h"


extern INT32 i4IrtDmaEvInit(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 i4IrtDmaEvReset(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 i4IrtDmaEvStop(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 i4IrtDmaEvArgRotateMirror(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 i4IrtDmaEvArgRotateMirrorFixSize(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 i4IrtDmaHwEVRotateMirror(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 i4IrtDmaSwEVRotateMirror(INT32 i4Argc, const CHAR ** szArgv);

static INT32 CMD_i4IrtDmaEvInit(INT32 i4Argc, const CHAR ** szArgv)
{
	i4IrtDmaEvInit(i4Argc, szArgv);
    return (0);
}
static INT32 CMD_i4IrtDmaEvReset(INT32 i4Argc, const CHAR ** szArgv)
{
	return i4IrtDmaEvReset(i4Argc, szArgv);
}

static INT32 CMD_i4IrtDmaEvStop(INT32 i4Argc, const CHAR ** szArgv)
{
	return i4IrtDmaEvStop(i4Argc, szArgv);
}

static INT32 CMD_i4IrtDmaEvArgRotateMirror(INT32 i4Argc, const CHAR ** szArgv)
{
	return i4IrtDmaEvArgRotateMirror(i4Argc, szArgv);
}

static INT32 CMD_i4IrtDmaEvArgRotateMirrorFixSize(INT32 i4Argc, const CHAR ** szArgv)
{
	return i4IrtDmaEvArgRotateMirrorFixSize(i4Argc, szArgv);
}

static INT32 CMD_i4IrtDmaHwEVRotateMirror(INT32 i4Argc, const CHAR ** szArgv)
{
	return i4IrtDmaHwEVRotateMirror(i4Argc, szArgv);
}

static INT32 CMD_i4IrtDmaSwEVRotateMirror(INT32 i4Argc, const CHAR ** szArgv)
{
	return i4IrtDmaSwEVRotateMirror(i4Argc, szArgv);
}


// cmd table
CLI_EXEC_T _arIrtdmaVrfCmdTbl[] =
{
    {"init",          "i",   CMD_i4IrtDmaEvInit,            NULL, "irt_dma module init", CLI_GUEST},
    {"reset",         "rst", CMD_i4IrtDmaEvReset,           NULL, "Reset irt_dma function", CLI_GUEST},
    {"stop",          "stop",CMD_i4IrtDmaEvStop,            NULL, "stop test thread", CLI_GUEST},
    {"RotateFlip",    "rf",  CMD_i4IrtDmaEvArgRotateMirror, NULL, "Rotate Mirror(Flip horizontal) output", CLI_GUEST},
    {"RotateFlipFixSize",  "rffs", CMD_i4IrtDmaEvArgRotateMirrorFixSize,   NULL, "Rotate Mirror(Flip horizontal) output",           CLI_GUEST},
    {"HwRotateFlip",  "hrf", CMD_i4IrtDmaHwEVRotateMirror,  NULL, "Only Hw Rotate Mirror(Flip horizontal) output", CLI_GUEST},
    {"SwRotateFlip",  "srf", CMD_i4IrtDmaSwEVRotateMirror,  NULL, "Only Sw Rotate Mirror(Flip horizontal) output", CLI_GUEST},

    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

//static CLI_EXEC_T _rIrtdmaModCmdTbl =
CLI_MAIN_COMMAND_ITEM(IRT)
{
    "irt_dma",
    NULL,
    NULL,
    _arIrtdmaVrfCmdTbl,
    "Irt_dma Verify command",
    CLI_GUEST
};
/*
CLI_EXEC_T* GetIrtDmaCmdTbl(void)
{
    return &_rIrtdmaModCmdTbl;
}*/

