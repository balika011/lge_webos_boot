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
 * $RCSfile: muxer_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file muxer_cmd.c
 *  This file contains implementation of CLI CMD for Muxer
 *
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e534 -e715 -e786 -e818 -e826 -e830 -e950 -e957 -e960 -e641
             -e613 -e836 */

#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_os.h"

#define DEFINE_IS_LOG    MUXER_IsLog
#include "x_debug.h"

#include "muxer_if.h"
#include "muxer_drvif.h"
#include "fbm_drvif.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//#define USE_FBM_MUXER


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#ifdef USE_FBM_MUXER
#define MX_CLI_BUF_SZ (prFbmPool->u4Size)
#else
#define MX_CLI_BUF_SZ (2 * 1024 * 1024)
#endif

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static INT32 _MuxerCmdInit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MuxerCmdSetFmt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MuxerCmdGo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MuxerCmdStop(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MuxerCmdProd(INT32 i4Argc, const CHAR ** szArgv);
#ifdef CC_ATV_PVR_SUPPORT
static INT32 _MuxerCmdStcRead(INT32 i4Argc, const CHAR **szArgv);
static INT32 _MuxerCmdStcWrite(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _MuxerCmdDemo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _MuxerCmdQuery(INT32 i4Argc, const CHAR ** szArgv);
#if defined(LINUX_TURNKEY_SOLUTION) && !defined(CC_DRIVER_DEMO)
static INT32 _MuxerCmdToggleMtalLog(INT32 i4Argc, const CHAR ** szArgv);
#endif

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(MUXER)

// cmd table
static CLI_EXEC_T _arMuxerCmdTbl[] =
{
    {"init",    "i",    _MuxerCmdInit,            NULL, "Muxer test init",                  CLI_GUEST},
    {"setFmt",  "sf",   _MuxerCmdSetFmt,          NULL, "Muxer test set format",            CLI_GUEST},
    {"go",      "g",    _MuxerCmdGo,              NULL, "Muxer test start",                 CLI_GUEST},
    {"stop",    "s",    _MuxerCmdStop,            NULL, "Muxer test stop",                  CLI_GUEST},
    {"prod",    "p",    _MuxerCmdProd,            NULL, "Muxer test produce output file",   CLI_SUPERVISOR},
#ifdef CC_ATV_PVR_SUPPORT                         
    {"stcr",    "sr",   _MuxerCmdStcRead,         NULL, "Muxer test STC read",              CLI_SUPERVISOR},
    {"stcw",    "sw",   _MuxerCmdStcWrite,        NULL, "Muxer test STC write",             CLI_SUPERVISOR},
#endif                                            
    {"demo",    "d",    _MuxerCmdDemo,            NULL, "Muxer demo",                       CLI_GUEST},
    {"query",   "q",    _MuxerCmdQuery,           NULL, "Muxer query status",               CLI_SUPERVISOR},
#if defined(LINUX_TURNKEY_SOLUTION) && !defined(CC_DRIVER_DEMO)
    {"mtal",   NULL,    _MuxerCmdToggleMtalLog,   NULL, "Toggle MTAL log",                  CLI_SUPERVISOR},
#endif
    CLIMOD_DEBUG_CLIENTRY(MUXER),     // for LOG use

    {NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static UCHAR _ucMxId = 0;
static UINT32 _u4VStrmNo = 0xFFFFFFFF;
static UINT32 _u4AStrmNo = 0xFFFFFFFF;
static UINT32 _u4BufSa;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


static VOID _muxer_cli_data_nfy(UCHAR ucMxId, MX_CB_DATA_T *prCbData)
{
#if 0
    static BOOL fgChkAvc = FALSE;
    UINT8 *pu1Buf = (UINT8 *)prCbData->u4Addr;

    //LOG(2, "mx_cb(id, addr, len, pts): %d, %08x, %d, 0x%08x\n", ucMxId, prCbData->u4Addr, prCbData->u4Len, prCbData->u4Pts);

    if (fgChkAvc)
    {
        ASSERT(0 == pu1Buf[0]);
        ASSERT(0 == pu1Buf[1]);
        ASSERT(0 == pu1Buf[2]);
        ASSERT(1 == pu1Buf[3]);
    }

    {
        UINT32 i, u4DumpSz;

        u4DumpSz = (u4Len > 16)? 16 : prCbData->u4Len;
        for (i = 0; i < u4DumpSz; i++)
            LOG(0, "%02x ", pu1Buf[i]);
        LOG(0, "\n");
    }
#endif

    MUXER_UpdateRp(ucMxId, prCbData->u4Addr);
}


static VOID _muxer_cli_nfy(UCHAR ucMxId, MX_NFY_COND_T eType, VOID *pvData)
{
    switch (eType)
    {
    default:
        LOG(1, "%s: unknown type %d\n", __FUNCTION__, eType);
        break;

    case MX_COND_DATA:
        _muxer_cli_data_nfy(ucMxId, (MX_CB_DATA_T *)pvData);
        break;
#if 0
    case MX_COND_OVERFLOW:
        _muxer_cli_overflow_nfy(ucMxId, (MX_OVERFLOW_REASON_T)pvData);
        break;

    case MX_COND_ERROR:
        _muxer_cli_error_nfy(ucMxId, (MX_ERROR_REASON_T)pvData);
        break;
#endif
    }
}


static INT32 _MuxerCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    MUXER_Init();

    return 0;
}


static INT32 _MuxerCmdGo(INT32 i4Argc, const CHAR ** szArgv)
{
    MUXER_Start(_ucMxId);

    return 0;
}


static INT32 _MuxerCmdStop(INT32 i4Argc, const CHAR ** szArgv)
{
    MUXER_Stop(_ucMxId);

    if (0xFFFFFFFF != _u4VStrmNo)
    {
        VERIFY(MUXER_DelStrm(_ucMxId, _u4VStrmNo));
        _u4VStrmNo = 0xFFFFFFFF;
    }

    if (0xFFFFFFFF != _u4AStrmNo)
    {
        VERIFY(MUXER_DelStrm(_ucMxId, _u4AStrmNo));
        _u4AStrmNo = 0xFFFFFFFF;
    }

    return 0;
}


static INT32 _MuxerCmdSetFmt(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucMxId;
    ENUM_MUXER_FMT_T eMuxFmt;
#ifdef USE_FBM_MUXER
    FBM_POOL_T *prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_MUXER);
#endif

    if (i4Argc < 3)
    {
        LOG(0, "Usage: %s <id> <fmt: 4-ts, 5-192ts, 7-cmpb, 8-raw>\n", szArgv[0]);
        return -1;
    }

    ucMxId = StrToInt(szArgv[1]);
    MUXER_VERIFY_ID_RET(ucMxId, -2);
    _ucMxId = ucMxId;

    eMuxFmt = (ENUM_MUXER_FMT_T)StrToInt(szArgv[2]);
    switch (eMuxFmt)
    {
    default:
        LOG(0, "%s(%d): wrong format(%d)!!!\n", __FUNCTION__, __LINE__, eMuxFmt);
        return -3;

    // support list
    case MUXER_FMT_MPEG2_TS:
        LOG(0, "Set 188 TS\n");
        break;

    case MUXER_FMT_MPEG2_TS_192:
        LOG(0, "Set 192 TS\n");
        break;

    case MUXER_FMT_CMPB:
        LOG(0, "Set private format\n");
        break;

    case MUXER_FMT_RAW:
        LOG(0, "Set Raw\n");
        break;
    }

    if (!_u4BufSa)
    {
#ifdef USE_FBM_MUXER
        _u4BufSa = prFbmPool->u4Addr;
#else
        _u4BufSa = BSP_AllocAlignedDmaMemory(MX_CLI_BUF_SZ, 1024);
#endif
        if (!_u4BufSa)
        {
            LOG(0, "%s(%d): allocate buf fail!!!\n", __FUNCTION__, __LINE__);
            return -4;
        }
    }
    VERIFY(MUXER_SetBuffer(ucMxId, VIRTUAL(_u4BufSa), MX_CLI_BUF_SZ, _muxer_cli_nfy, 0));
    VERIFY(MUXER_SetMuxFmt(ucMxId, eMuxFmt));

    return 0;
}


static INT32 _MuxerCmdProd(INT32 i4Argc, const CHAR ** szArgv)
{
    MUXER_INFO_T *prMuxInfo = _MUXER_QueryInst(_ucMxId);

    LOG(0, "d.save.b \"d:\\mx.264\" 0x%08x--0x%08x\n",
        prMuxInfo->pucMxBuf, prMuxInfo->pucMxBuf + prMuxInfo->u4MxBufWp - 1);

    return 0;
}


#ifdef CC_ATV_PVR_SUPPORT
extern UINT32 _AUD_ReadPsrStc1(void);
extern void _AUD_SetPsrStc1(UINT32 u4Val);
static INT32 _MuxerCmdStcRead(INT32 i4Argc, const CHAR **szArgv)
{
    LOG(0, "Muxer test STC read: %d\n", _AUD_ReadPsrStc1());

    return 0;
}

static INT32 _MuxerCmdStcWrite(INT32 i4Argc, const char **szArgv)
{
    UINT32 u4Val;

    if (i4Argc == 1)
    {
        u4Val = 0;
    }
    else
    {
        u4Val = StrToInt(szArgv[1]);
    }

    LOG(0, "Muxer test write STC: %d\n", u4Val);
    _AUD_SetPsrStc1(u4Val);

    return 0;
}
#endif


static INT32 _MuxerCmdDemo(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucMxId = 0;
    UINT32 u4Type = 1; //2 1: v, 2: a, 3: a/v
#ifdef USE_FBM_MUXER
    FBM_POOL_T *prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_MUXER);
#endif

    if (i4Argc > 1)
    {
        u4Type = StrToInt(szArgv[1]);
        if (u4Type > 3)
        {
            LOG(0, "%s(%d): arg1 should be 1: v, 2: a, 3: a/v\n", __FUNCTION__, __LINE__);
            u4Type = 1;
        }
    }

    MUXER_Init();

    if (!_u4BufSa)
    {
#ifdef USE_FBM_MUXER
        _u4BufSa = prFbmPool->u4Addr;
#else
        _u4BufSa = BSP_AllocAlignedDmaMemory(MX_CLI_BUF_SZ, 1024);
#endif
        if (!_u4BufSa)
        {
            LOG(0, "%s(%d): allocate buf fail!!!\n", __FUNCTION__, __LINE__);
            return -4;
        }
    }
    VERIFY(MUXER_SetBuffer(ucMxId, VIRTUAL(_u4BufSa), MX_CLI_BUF_SZ, _muxer_cli_nfy, 0));
#if 1
    VERIFY(MUXER_SetMuxFmt(ucMxId, MUXER_FMT_MPEG2_TS));
#else
    VERIFY(MUXER_SetMuxFmt(ucMxId, MUXER_FMT_RAW));
#endif

    if (u4Type == 1)
    {
        VERIFY(MUXER_AddStrm(ucMxId, eMUXER_STRMTYPE_VIDEO, &_u4VStrmNo));
        LOG(0, "muxer demo video only\n");
    }
    else if (u4Type == 2)
    {
        VERIFY(MUXER_AddStrm(ucMxId, eMUXER_STRMTYPE_AUDIO, &_u4AStrmNo));
        LOG(0, "muxer demo audio only\n");
    }
    else
    {
        VERIFY(MUXER_AddStrm(ucMxId, eMUXER_STRMTYPE_VIDEO, &_u4VStrmNo));
        VERIFY(MUXER_AddStrm(ucMxId, eMUXER_STRMTYPE_AUDIO, &_u4AStrmNo));
    }

    MUXER_Start(ucMxId);
    _ucMxId = ucMxId;

    return 0;
}


static INT32 _MuxerCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucMxId = 0;

    if (i4Argc < 2)
    {
        LOG(0, "Usage: %s <id>\n", szArgv[0]);
    }
    else
    {
        ucMxId = StrToInt(szArgv[1]);
    }

    MUXER_Query(ucMxId);

    return 0;
}


#if defined(LINUX_TURNKEY_SOLUTION) && !defined(CC_DRIVER_DEMO)
static INT32 _MuxerCmdToggleMtalLog(INT32 i4Argc, const CHAR ** szArgv)
{
#define MTMX_LOG_MASK               (1 << 8) // should align with macro in mttype.h

    extern unsigned long u4MtalLogMask2;
    UINT32 u4LogMaskBits = (i4Argc < 2)
        ? MTMX_LOG_MASK
        : StrToInt(szArgv[1]);
    u4MtalLogMask2 ^= u4LogMaskBits;
    LOG(0, "u4MtalLogMask2=0x%x\n", u4MtalLogMask2);

    return 0;
}
#endif


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Muxer)
{
    "muxer",
    "mx",
    NULL,
    _arMuxerCmdTbl,
    "MUXER command",
    CLI_SUPERVISOR
};
LINT_RESTORE

/*lint -restore */



