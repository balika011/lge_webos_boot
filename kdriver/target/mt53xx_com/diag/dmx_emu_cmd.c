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
 * $RCSfile: dmx_emu_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_emu_cmd.c
 *  Demux Emulation CLI commands
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm_if.h"
#include "dmx_emu_if.h"
#include "dmx_drm_drvif.h"
#include "dmx_drm_if.h"
#include "fvr_if.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN


#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_rand.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "x_bim.h"
#include <math.h>

//---------------------------------------------------------------------
// LOG macro related

// #define DEFINE_IS_LOG    CLI_IsLog
#define DEFINE_IS_LOG    DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//LINT_SAVE_AND_DISABLE
//CLIMOD_DEBUG_FUNCTIONS(DMX)
//LINT_RESTORE

LINT_SUPPRESS_SYMBOL(818, aszArgv)


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
EXTERN CLI_EXEC_T _arCiEmuCmdTbl[];
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// TS index
UINT8 _u1TsIndex = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//=====================================================================
// CLI command table
//=====================================================================

static INT32 _CmdDdiTransfer0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size,  u4Mode = 0x2;
    UINT8 u1Type,u1Tsidx;
    BOOL fgRet;
    PUL_MODE_T eMode;
    BOOL fgBypassFramer = FALSE;

    if (i4Argc == 1)
    {
        Printf("Usage: %s , Tsindex , type[0-1] , [loop(0=infinite)] bypassframer, [mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("Notice:mode type is optional,default is full pull mode,\n"
               "The other two type is just for test!\n");
        return -1;
    }

    //txindex
    if (i4Argc > 1)
    {
        u1Tsidx = StrToInt(aszArgv[1]);
    }

    //type
    if (i4Argc > 2)
    {
        u1Type = StrToInt(aszArgv[2]);
    }

    //loop
    if (i4Argc > 3)
    {
        u4MaxLoop = StrToInt(aszArgv[3]);
    }

    //bypassframer
    if (i4Argc > 4)
    {
        if (StrToInt(aszArgv[4]) == 0)
        {
            fgBypassFramer = FALSE;
        }
        else
        {
            fgBypassFramer = TRUE;
        }
    }

    //mode
    if (i4Argc > 5)
    {
        u4Mode = StrToInt(aszArgv[5]);
    }

    //maxsize
    if (i4Argc > 6)
    {
        u4MaxSize = StrToInt(aszArgv[6]);
    }

    //minsize
    if (i4Argc > 7)
    {
        u4MinSize = StrToInt(aszArgv[7]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart DDI transfer 0 test\n");
    Printf("Type: %u, TS index: %u, round: %u, bypass framer : %s, max size: 0x%x, min size: 0x%x\n\n",
           u1Type, u1Tsidx, u4MaxLoop,fgBypassFramer ? "TRUE" : "FALSE", u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,188);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_DDI_Transfer0(u1Type, u1Tsidx, u4Size, eMode, fgBypassFramer))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdDdi_To_PidFilter_Transfer0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 1;
    UINT32 u4Loop;
    UINT8 u1Tsidx;
    BOOL fgRet;
    BOOL b_match = TRUE;

    if (i4Argc == 1)
    {
        Printf("Usage: %s , Tsindex(2,3,4) ,b_match(0:1),maxLoop ", aszArgv[0]);
        return -1;
    }

    //txindex
    if (i4Argc > 1)
    {
        u1Tsidx = StrToInt(aszArgv[1]);
    }

    //type
    if (i4Argc > 2)
    {
        b_match = StrToInt(aszArgv[2]);
    }

    //loop
    if (i4Argc > 3)
    {
        u4MaxLoop = StrToInt(aszArgv[3]);
    }
    Printf("\nStart DDI transfer pass pid filter 0 test\n");
    Printf("Match: %u, TS index: %u, round: %u\n\n",b_match, u1Tsidx, u4MaxLoop);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        Printf("Round %u - size: %u\n", u4Loop);

        if (!DMX_EMU_DDI_To_PID_FILTER_Transfer( u1Tsidx, b_match))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdDdiTransfer1(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size, u4Mode = 0x2;;
    BOOL fgRet;
    PUL_MODE_T eMode;

    if (i4Argc == 1)
    {
        Printf("Usage: %s [loop(0=infinite)][mode(0 full fush,1 half, 2 full pull)][max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("Notice:mode type is optional,default is full pull mode ,The other two type is just for test!");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4Mode = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart DDI transfer 1 test\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    if (!DMX_EMU_DDI_Init(_u1TsIndex))
    {
        Printf("Fail to initialize DDI!\n");
        return -1;
    }

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
                           188);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_DDI_Transfer1(0, u4Size, eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    if (!DMX_EMU_DDI_Exit())
    {
        Printf("Fail to release DDI!\n");
        return -1;
    }

    return (fgRet ? 0 : -1);
}

extern BOOL DMX_EMU_DDI_Transfer2(UINT8 u1Type, UINT8 u1TsIndex, UINT32 u4Size);
static INT32 _CmdDdiTransfer2(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    UINT8 u1Type;
    BOOL fgRet;

    if (i4Argc == 1)
    {
        Printf("Usage: %s type[0-2] [loop(0=infinite)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u1Type = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart DDI transfer 1 test\n");
    Printf("Type: %u, TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           u1Type, _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
                           188);

        //u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
        //    188);

        //u4Size = random(u4MaxSize - u4MinSize) + u4MinSize;

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_DDI_Transfer2(u1Type, _u1TsIndex, u4Size))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullTsTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet, fgBypassFramer;
    UINT32 u4Mode = 0x2;
    PUL_MODE_T eMode;
    UINT8 u1Tsidx;

    if (i4Argc == 1)
    {
        Printf("Usage: %s , Tsindex ,loop(0=infinite) bypassframer, [mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("Notice:mode type is optional,default is full pull mode,\n"
               "The other two type is just for test!\n");
        return -1;
    }

    //txindex
    if (i4Argc > 1)
    {
        u1Tsidx = (UINT8)StrToInt(aszArgv[1]);
    }

    //MaxLoop
    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    //bypassframer
    if (i4Argc > 3)
    {
        if (StrToInt(aszArgv[3]) == 0)
        {
            fgBypassFramer = FALSE;
        }
        else
        {
            fgBypassFramer = TRUE;
        }
    }

    //Mode
    if (i4Argc > 4)
    {
        u4Mode = StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u4MaxSize = StrToInt(aszArgv[5]);
    }

    if (i4Argc > 6)
    {
        u4MinSize = StrToInt(aszArgv[6]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart Pull TS test 0\n");
    Printf("TS index: %u, round: %u, bypass framer : %s, max size: 0x%x, min size: 0x%x\n\n",
           u1Tsidx, u4MaxLoop, fgBypassFramer ? "TRUE" : "FALSE", u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullTsTransfer0(u1Tsidx, u4Size, u2Pid, eMode, fgBypassFramer))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullTsTest1(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT32 u4Mode = 0x2;
    PUL_MODE_T eMode;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop(0=infinite)[mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("Notice:mode type is optional,default is full pull mode ,The other two type is just for test!");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4Mode = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart Pull TS test 1\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
                           188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullTsTransfer1(_u1TsIndex, u4Size, u2Pid,eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullTsTest2(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT32 u4Mode = 0x2;
    PUL_MODE_T eMode;


    if (i4Argc == 1)
    {
        Printf("Usage: %s loop(0=infinite)[mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("Notice:mode type is optional,default is full pull mode ,The other two type is just for test!");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4Mode = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart pull TS test 2\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    if (!DMX_EMU_InitPullTsBuffers(_u1TsIndex))
    {
        Printf("Fail to initialize buffers!\n");
        return -1;
    }

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
                           188);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullTsTransfer2(u4Size,eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    if (!DMX_EMU_ReleasePullTsBuffers())
    {
        Printf("Fail to release buffers!\n");
        return -1;
    }

    return (fgRet ? 0 : -1);
}

// generate ts packet ,bypass pid filter output ca output buffer

static INT32 _CmdPullTsTest3(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT32 u4Mode = 0x2;
    PUL_MODE_T eMode;

    if (i4Argc == 1)
    {
        Printf("Usage: %s [loop(0=infinite)][mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("Notice:mode type is optional,default is full pull mode ,The other two type is just for test!");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4Mode = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart Pull TS test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           3, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,188);

        u2Pid = random(0x2000); //random(0x2000)

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullTsTransfer3(3, u4Size, u2Pid,eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}

static INT32 _CmdPullEsTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Ts_idx=0,u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT32 u4Mode = 0x2;
    PUL_MODE_T eMode;

    if (i4Argc == 1)
    {
#if defined(CC_MT5890)
        Printf("Usage: %s [TS_IDX=0,1,2,3,4][loop(0=infinite)][mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
#else  //MT5882
        Printf("Usage: %s [TS_IDX=0,1,2,3][loop(0=infinite)][mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
#endif
        Printf("Notice:mode type is optional,default is full pull mode ,The other two type is just for test!");
        return -1;
    }

	if (i4Argc > 1)
	{
	  u4Ts_idx = StrToInt(aszArgv[1]);
	}

    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4Mode = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MaxSize = StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u4MinSize = StrToInt(aszArgv[5]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart Pull ES test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           u4Ts_idx, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
                           188);
        u2Pid = random(0x2000);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullEsTransfer0(u4Ts_idx, u4Size, u2Pid,eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdPullEsTest1(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Ts_idx=0,u4MaxLoop = 0, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT32 u4Mode = 0x2;
    PUL_MODE_T eMode;

    if (i4Argc == 1)
    {
#if defined(CC_MT5890)
        Printf("Usage: %s [u4Ts_idx=0-4][loop(0=infinite)][mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
#else  //MT5882
        Printf("Usage: %s [u4Ts_idx=0-3][loop(0=infinite)][mode(0 full fush,1 half, 2 full pull)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
#endif
        Printf("Notice:mode type is optional,default is full pull mode ,The other two type is just for test!");
        return -1;
    }

	if (i4Argc > 1)
	{
	  u4Ts_idx = StrToInt(aszArgv[1]);
	}

    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4Mode = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MaxSize = StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u4MinSize = StrToInt(aszArgv[5]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart Pull ES test 1\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           u4Ts_idx, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
                           188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_PullEsTransfer1(u4Ts_idx, u4Size, u2Pid,eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    if (!DMX_Init())
    {
        return -1;
    }
    if (!DMX_DDI_Init())
    {
        return -1;
    }

    return (fgRet ? 0 : -1);
}


static INT32 _CmdMulti2CBC(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop = 1;
    UINT8 u1Type = 0, u1SetType;
    UINT8 u1TsIdx;

    if (i4Argc < 3)
    {
        Printf("%s loop type tsidx\n", aszArgv[0]);
        Printf("   type, 0:play, 1:rec, 2:rec_enc, 3:mix\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4Loop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u1Type = (UINT8)StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u1TsIdx = (UINT8)StrToInt(aszArgv[3]);
    }

    for (i=0; i<u4Loop; i++)
    {
        Printf("----- Loop: %d, Tsidx: %d.\n", i,u1TsIdx);
        if (u1Type == 3)
        {
            u1SetType = (UINT8)i % 3;
        }
        else
        {
            u1SetType = u1Type;
        }

        switch (u1SetType)
        {
        case 0:
            Printf("   Play test\n");
            break;
        case 1:
            Printf("   Rec Level1 test\n");
            break;
        case 2:
            Printf("   Rec Level2 test\n");
            break;
        }

        if (!DMX_EMU_Multi2Test(u1TsIdx, u1SetType))
        {
            Printf("Fail\n");
            return -1;
        }

        Printf("----- Loop: %d, result: pass!.\n", i);
    }

    return 0;
}


static INT32 _CmdDesTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT8 u1Type = 0;
    UINT8 u1SetType = 0;
    UINT8 u1TsIdx = 0;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop type tsidx [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("    type, 0:play, 1:rec_l1, 2:rec_l2, 3:combine\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u1Type = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u1TsIdx = (UINT8)StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MaxSize = StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u4MinSize = StrToInt(aszArgv[5]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart DES test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           u1TsIdx, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
                           188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u, pid: %u\n", u4Loop, u4Size, u2Pid);

        if (u1Type == 3)
        {
            u1SetType = u4Loop % 3;
        }
        else
        {
            u1SetType = u1Type;
        }

        switch (u1SetType)
        {
        case 0:
            Printf("   play\n");
            break;
        case 1:
            Printf("   Rec Level1\n");
            break;
        case 2:
            Printf("   REc Level2\n");
            break;
        }

        if (!DMX_EMU_Des0(u1TsIdx, u4Size, u2Pid, u1SetType))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdAesTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    UINT8 u1Type = 0;
    UINT8 u1SetType = 0;
    BOOL fgRet;
    UINT8 u1TsIdx = 0;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop type u1tsidx [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("   type, 0:play, 1:rec_L1, 2:rec_L2, 3:L1__pattern_match__L2, 4:L1__pattern_match 5:random\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u1Type = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u1TsIdx = (UINT8)StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MaxSize = StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u4MinSize = StrToInt(aszArgv[5]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart AES test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           u1TsIdx, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size =  DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,188); //0x500c;//

        u2Pid = random(0x2000); // 0x100

        Printf("Round %u - size: %u, pid: %u\n", u4Loop, u4Size, u2Pid);

        if (u1Type == 5)
        {
            u1SetType = (UINT8)u4Loop % 5;
        }
        else
        {
            u1SetType = u1Type;
        }

        switch (u1SetType)
        {
        case 0:
            Printf("   Play test\n");
            break;
        case 1:
            Printf("   Rec Level1 test\n");
            break;
        case 2:
            Printf("   Rec Level2 test\n");
            break;
        case 3:
            Printf("   Rec descramble-- pattern match -- encramble\n");
            break;
        case 4:
            Printf("   Rec descramble-- pattern match \n");
            break;
        }

        if (!DMX_EMU_Aes0(u1TsIdx, u4Size, u2Pid, u1SetType))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdSMS4Test(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    UINT8 u1Type;
    UINT8 u1TsIdx = 0;

    if (i4Argc == 1)
    {
        Printf("Usage: %s loop type tsidx [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("    type, 0:play, 1:rec, 2:mix\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u1Type = (UINT8)StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u1TsIdx = (UINT8)StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MaxSize = StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u4MinSize = StrToInt(aszArgv[5]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart SMS4 test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           u1TsIdx, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;

    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188,
                           188);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u, pid: %u\n", u4Loop, u4Size, u2Pid);

        if (!DMX_EMU_SMS4(u1TsIdx, u4Size, u2Pid, u1Type))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


#ifdef TEST_MODE
static INT32 _CmdVuduAesTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Size = 188 * 16 * 16;
    UINT16 u2Pid = 0;
    BOOL fgRet, fgSCTE = TRUE;

    Printf("\nStart AES test 0\n");

    fgRet = DMX_EMU_VuduAes0(_u1TsIndex, u4Size, u2Pid, fgSCTE);

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdHDCP2Test(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet;

    fgRet = DMX_EMU_HDCP();

    return (fgRet ? 0 : -1);
}
#endif

#if 0  // Not use in MT5396
static void _ShowDrmTest0Usage(const CHAR* szCmd, UINT32 u4Max, UINT32 u4Min)
{
    Printf("Usage: %s mode[bypass|aes|aesscte52|nd|random] [loop(1, 0=infinite)]"
           " [max_size=0x%x] [min_size=0x%x]\n",
           szCmd, u4Max, u4Min);
}

static INT32 _CmdDrmTest0(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 1, u4MaxSize = 0x1000, u4MinSize = 0x100;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    DMX_DRM_MODE_T eMode;

    if (i4Argc == 1)
    {
        _ShowDrmTest0Usage(aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (x_strcmp(aszArgv[1], "bypass") == 0)
    {
        eMode = DMX_DRM_MODE_BYPASS;
    }
    else if (x_strcmp(aszArgv[1], "aes") == 0)
    {
        eMode = DMX_DRM_MODE_AES;
    }
    else if (x_strcmp(aszArgv[1], "aese") == 0)
    {
        eMode = DMX_DRM_MODE_AES_ENCRYPTION;
    }
    else if (x_strcmp(aszArgv[1], "aesscte52") == 0)
    {
        eMode = DMX_DRM_MODE_AES_SCTE52;
    }
    else if (x_strcmp(aszArgv[1], "nd") == 0)
    {
        eMode = DMX_DRM_MODE_ND;
    }
    else if (x_strcmp(aszArgv[1], "random") == 0)
    {
        eMode = DMX_DRM_MODE_UNKNOWN;
    }
    else
    {
        _ShowDrmTest0Usage(aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MaxSize = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MinSize = StrToInt(aszArgv[4]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart DRM test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        UINT16 u2Pid;

        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 1,
                           1);

        u2Pid = random(0x2000);

        Printf("Round %u - size: %u, pid: %u\n", u4Loop, u4Size, u2Pid);

        if (!DMX_EMU_Drm0(_u1TsIndex, u4Size, u2Pid, eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdDrmPerfTest(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet;
    BOOL fgSw = FALSE;
    UINT32 u4Size = 1 * 1024 * 1024;
    DMX_DRM_MODE_T eMode = DMX_DRM_MODE_AES;

    if (i4Argc > 4 || i4Argc == 1)
    {
        Printf("Usage: %s [aes|aese|nd|pd] [size=0x%x] [SW(0|1)]\n", aszArgv[0], u4Size);
        return -1;
    }

    if (x_strcmp(aszArgv[1], "aes") == 0)
    {
        eMode = DMX_DRM_MODE_AES;
    }
    else if (x_strcmp(aszArgv[1], "aese") == 0)
    {
        eMode = DMX_DRM_MODE_AES_ENCRYPTION;
    }
    else if (x_strcmp(aszArgv[1], "nd") == 0)
    {
        eMode = DMX_DRM_MODE_ND;
    }
    else if (x_strcmp(aszArgv[1], "pd") == 0)
    {
        eMode = DMX_DRM_MODE_PD;
    }
    else
    {
        Printf("Usage: %s [aes|aese|nd|pd] [size=0x%x] [SW(0|1)]\n", aszArgv[0], u4Size);
        return -1;
    }

    if (i4Argc == 3)
    {
        u4Size = StrToInt(aszArgv[2]);
    }

    if (i4Argc == 4)
    {
        fgSw = StrToInt(aszArgv[3]);
    }

    {
        UINT16 u2Pid;

        u2Pid = random(0x2000);

        fgRet = DMX_EMU_DRM_Perf(_u1TsIndex, u4Size, u2Pid, eMode, fgSw);
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}
#endif

#ifdef TEST_MODE
static INT32 _CmdDrmNdSelfTest(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet;

    Printf("DRM-ND self test ...\n");

    fgRet = DMX_EMU_DrmNdSelfTest();
    if (!fgRet)
    {
        Printf("Fail!\n");
        return -1;
    }

    Printf("Pass!\n");

    return 0;
}
#endif  // DMX_SUPPORT_NDDRM

static INT32 _CmdPatternMatch(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0, u4MaxSize = 0x10000, u4MinSize = 0x2000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet;
    HAL_RAW_TIME_T rawTime;

    if (i4Argc == 1)
    {
        Printf("Usage: %s [loop(0=infinite)] [max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u4MaxSize = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4MinSize = StrToInt(aszArgv[3]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart Pattern Match test 0\n");
    Printf("TS index: %u, round: %u, max size: 0x%x, min size: 0x%x\n\n",
           _u1TsIndex, u4MaxLoop, u4MaxSize, u4MinSize);

    HAL_GetRawTime(&rawTime);
    srand(rawTime.u4Cycles);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188, 188);
        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        // FIXME!!!
#if 0
        if (!DMX_EMU_PM_Transfer(_u1TsIndex, u4Size))
#endif
            if (!DMX_EMU_PM_Transfer(_u1TsIndex))
            {
                break;
            }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


EXTERN BOOL _DMX_EMU_PSI_Test(UINT8 u1TsIdx, BOOL fgGenPat, DMX_FILTER_MODE_T eMode, BOOL fgBypassFramer,UINT32 u4_offset);
static INT32 _CmdPSITest(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_MODE_T eMode;
    UINT32 i, u4Loop, u4Mode;
    UINT8 u1TsIdx = 0;
    BOOL fgBypassFramer =FALSE;
    UINT32 ui4_offset = 0;

    if (i4Argc != 6)
    {
        Printf("%s [loop] [tsindex][bypassFramer][mode][filter idx offset]\n", aszArgv[0]);
        Printf("1: BypassFramer, 0 : passFramer\n");
        Printf("    0: short mode\n");
        Printf("    1: pos_neg mode\n");
        Printf("    2: generic mode\n");
		Printf("filter idx offset:[0,96,48]");
        return -1;
    }

    //Loop
    u4Loop = StrToInt(aszArgv[1]);

    //Tsindex
    u1TsIdx = StrToInt(aszArgv[2]);

    //bypassframer
    if (StrToInt(aszArgv[3]) == 0)
    {
        fgBypassFramer = FALSE;
    }
    else
    {
        fgBypassFramer = TRUE;
    }

    //Mode
    u4Mode = StrToInt(aszArgv[4]);

	//section filter index offset
	ui4_offset=StrToInt(aszArgv[5]);

    if (u4Mode == 0)
    {
        eMode = DMX_FILTER_MODE_SHORT;
    }
    else if (u4Mode == 1)
    {
        eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if (u4Mode == 2)
    {
        eMode = DMX_FILTER_MODE_GENERIC;
    }
    else
    {
        Printf("Error Filter Mode!\n");
        return -1;
    }

    Printf("Loop :%d, Tsidx : %d, BypassFramer : %s, Mode: %d,filter index offset=%d.\n",
            u4Loop, u1TsIdx , fgBypassFramer ? "TRUE" : "FALSE",u4Mode,ui4_offset);

    for (i=0; i<u4Loop; i++)
    {
        Printf("------ Round %u ---------\n", i+1);
        if (!_DMX_EMU_PSI_Test(u1TsIdx, TRUE, eMode, fgBypassFramer,ui4_offset))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("------ %d time OK -------\n", i+1);
    }

    return 0;
}


EXTERN BOOL _DMX_EMU_MulMatch_Test(UINT8 u1TsIdx);
static INT32 _CmdMultipleMatchTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;

    if (i4Argc != 2)
    {
        Printf("%s loop\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    for (i=0; i<u4Loop; i++)
    {
        if (!_DMX_EMU_MulMatch_Test(_u1TsIndex))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i);
    }

    return 0;
}


EXTERN BOOL _DMX_EMU_PSICombo_Test(UINT8 u1TsIdx, BOOL fgGenPat, DMX_FILTER_MODE_T eMode, UINT32 u4Hw , BOOL fgBypassFramer);
static INT32 _CmdPSIComboTest(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_MODE_T eMode;
    UINT32 i, u4Loop, u4Mode, u4Hw;
    UINT8 u1TsIdx;
    BOOL fgBypassFramer;

    if (i4Argc != 6)
    {
        Printf("%s [loop][tsindex][bypassFramer][mode][hw]\n", aszArgv[0]);
        Printf("   mode for playback:  \n");
        Printf("    0: short mode\n");
        Printf("    1: pos_neg mode\n");
        Printf("    2: generic mode\n");
	    Printf("   mode for record:  \n");
        Printf("    0: pid filter [0-127]\n");
        Printf("   hw:  \n");
        Printf("       0: play only\n");
        Printf("       1: record only\n");
        Printf("       2: play and record\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);

    //Tsindex
    u1TsIdx = StrToInt(aszArgv[2]);

    //bypassframer
    if (StrToInt(aszArgv[3]) == 0)
    {
        fgBypassFramer = FALSE;
    }
    else
    {
        fgBypassFramer = TRUE;
    }

    u4Mode = StrToInt(aszArgv[4]);

    u4Hw = StrToInt(aszArgv[5]);

    if (u4Mode == 0)
    {
        eMode = DMX_FILTER_MODE_SHORT;
    }
    else if (u4Mode == 1)
    {
        eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if (u4Mode == 2)
    {
        eMode = DMX_FILTER_MODE_GENERIC;
    }
    else
    {
        return -1;
    }

    Printf("Loop :%d, Tsidx : %d, BypassFramer : %s, Mode: %d, HW: %d.\n",
            u4Loop, u1TsIdx , fgBypassFramer ? "TRUE" : "FALSE",u4Mode, u4Hw);

    for (i=0; i<u4Loop; i++)
    {
        Printf("------ Round %u ---------\n", i+1);
        if (!_DMX_EMU_PSICombo_Test(u1TsIdx, TRUE, eMode, u4Hw, fgBypassFramer))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("-----%d time OK --------\n", i+1);
    }

    return 0;
}

static INT32 _CmdFVRPesTest(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet;
    UINT8 u1TsIdx;

    if (i4Argc != 2)
    {
        Printf("%s tsindex.\n", aszArgv[0]);
        return -1;
    }

    //Tsindex
    u1TsIdx = (UINT8)StrToInt(aszArgv[1]);

    fgRet = DMX_EMU_FVR_PES_Test(u1TsIdx);
    if (fgRet)
    {
        Printf("OK\n");
    }
    else
    {
        Printf("Fail\n");
    }

    return 0;
}


#if 0
static INT32 _CmdStartTSOutput(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet = FALSE;
    UINT32 u4MaxLoop, u4Loop = 0;
    UINT8 u1TsIndex;
    UINT32 u4Size = 0;

    if (i4Argc != 3 && i4Argc != 4)
    {
        Printf("Usage: %s ts_index data_size [loop(0=infinite)]\n", aszArgv[0]);
        return -1;
    }

    u1TsIndex = (UINT8) StrToInt(aszArgv[1]);
    u4Size = StrToInt(aszArgv[2]);
    u4MaxLoop = StrToInt(aszArgv[3]);

    u4Size -= (u4Size % 188);

    while (1)
    {
        Printf("Round: %u\n", u4Loop);

        if (!DMX_EMU_TSOut_Init(u1TsIndex, u4Size))
        {
            return -1;
        }

        if (!DMX_EMU_TSOut_StressTest())
        {
            break;
        }

        if ((++u4Loop >= u4MaxLoop) && (u4MaxLoop != 0))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS!" : "FAIL!");

    return (fgRet ? 0 : -1);
}
#endif

static INT32 _CmdFVRTimestampTest(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgTimestampOn = FALSE;
    UINT32 i, u4Loop;
    UINT8 u1TsOutIdx = 0;
	UINT32 u4_tsidx = 0,u4_ptsidx=2;

    if (i4Argc != 6)
    {
        Printf("%s loop TsoutIdx on|off,record ts_idx[0,1,2,3],playback ts_idx[2,3,4]\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    u1TsOutIdx = (UINT8)StrToInt(aszArgv[2]);

    if (x_strcmp(aszArgv[3], "on") == 0)
    {
        fgTimestampOn = TRUE;
    }
    else if (x_strcmp(aszArgv[3], "off") == 0)
    {
        fgTimestampOn = FALSE;
    }
    else
    {
        Printf("%s loop on|off\n", aszArgv[0]);
        return -1;
    }
    u4_tsidx =  StrToInt(aszArgv[4]);
	u4_ptsidx = StrToInt(aszArgv[5]);
    for (i=0; i<u4Loop; i++)
    {
        Printf("----- Test %d , TsoutIdx: %d, fgTimestampOn : %s,record_idx=%d,playback_idx=%d---\n", i+1, u1TsOutIdx, fgTimestampOn ? "TRUE" : "FALSE",u4_tsidx,u4_ptsidx);
        if (DMX_EMU_FVR_TimestampOnOffTest(u4_tsidx, fgTimestampOn, u1TsOutIdx,u4_ptsidx))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}

static INT32 _CmdFVRTimestampPauseTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsOutIdx = 0;
    UINT32 u4_tsidx = 0,u4_ptsidx=2;
    if (i4Argc != 5)
    {
        Printf("%s loop, tsoutidx,record_idx[0-3],playback_idx[2-4]\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    u1TsOutIdx = (UINT8)StrToInt(aszArgv[2]);
    
    u4_tsidx =  StrToInt(aszArgv[3]);
	u4_ptsidx = StrToInt(aszArgv[4]);
    for (i=0; i<u4Loop; i++)
    {
        Printf("----- Test %d , Tsoutidx :%d.-----\n", i+1, u1TsOutIdx);

        if (DMX_EMU_FVR_TimestampPauseTest(u4_tsidx, u1TsOutIdx,u4_ptsidx))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}

static INT32 _CmdFVRTimestampErrTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsOutIdx = 0;
    UINT32 u4_tsidx = 0,u4_ptsidx=2;
    if (i4Argc != 5)
    {
        Printf("%s loop, tsoutidx,record_ts_idx[0,1,2,3],playback_ts[2,3,4]\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    u1TsOutIdx = (UINT8)StrToInt(aszArgv[2]);
    	
    u4_tsidx =  StrToInt(aszArgv[3]);
	u4_ptsidx = StrToInt(aszArgv[4]);
    for (i=0; i<u4Loop; i++)
    {
        Printf("----- Test %d , Tsoutidx :%d.-----\n", i+1, u1TsOutIdx);
        if (DMX_EMU_FVR_TimestampErrTest(u4_tsidx, u1TsOutIdx,u4_ptsidx))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}

static INT32 _CmdFVRTimestampResetTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsOutIdx = 0;
    UINT32 u4_tsidx = 0,u4_ptsidx=2;
	
    if (i4Argc != 5)
    {
        Printf("%s loop, tsoutidx,record_ts_idx[0,1,2,3],playback_ts[2,3,4]\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    u1TsOutIdx = (UINT8)StrToInt(aszArgv[2]);
	
    u4_tsidx =  StrToInt(aszArgv[3]);
	u4_ptsidx = StrToInt(aszArgv[4]);

    for (i=0; i<u4Loop; i++)
    {
        Printf("----- Test %d , Tsoutidx :%d.-----\n", i+1, u1TsOutIdx);

        if (DMX_EMU_FVR_TimestampResetTest(u4_tsidx, u1TsOutIdx,u4_ptsidx))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
        }
    }

    return 0;
}


static INT32 _CmdBypassTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop, u4Rec;
    BOOL fgRec;
    UINT8 u1Tsidx;

    if (i4Argc != 4)
    {
        Printf("%s loop mode tsidx.\n", aszArgv[0]);
        Printf("   play:0, rec:1, mix:2\n");
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Rec = StrToInt(aszArgv[2]);
    u1Tsidx = (UINT8)StrToInt(aszArgv[3]);

    for (i=0; i<u4Loop; i++)
    {
        if (u4Rec == 0)
        {
            fgRec = FALSE;
        }
        else if (u4Rec == 1)
        {
            fgRec = TRUE;
        }
        else
        {
            fgRec = (random(2) == 1) ? TRUE : FALSE;
        }

        Printf("----- Test %d -----\n", i+1);
        if (fgRec)
        {
            Printf("Rec bypass, tsidx: %d, :\n",u1Tsidx);
        }
        else
        {
            Printf("Play bypass, tsidx: %d, :\n", u1Tsidx);
        }
        if (DMX_EMU_DDI_Bypass_Test(u1Tsidx, fgRec))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}


extern BOOL DMX_EMU_PreByte_Test(UINT8 u1TsIndex, BOOL fgExtSync, UINT32 u4Size, UINT16 u2PktSize);
static INT32 _CmdPreByteTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop, u4Size, u4Sync;
    UINT16 u2PktSize;
    BOOL fgRandom = FALSE;
    UINT8 u1TsIdx = 0;

    if ((i4Argc != 2) && (i4Argc != 6))
    {
        Printf("%s loop tsidx sync size pkt_size\n", aszArgv[0]);
        Printf("    sync, 0: internal, 1: external\n");
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    if (i4Argc >= 5)
    {
        u1TsIdx = (UINT8)StrToInt(aszArgv[2]);
        u4Sync = StrToInt(aszArgv[3]);
        u4Size = StrToInt(aszArgv[4]);
        u2PktSize = StrToInt(aszArgv[5]);
    }
    else
    {
        fgRandom = TRUE;
    }

    for (i=0; i<u4Loop; i++)
    {
        if (fgRandom)
        {
            u4Sync = random(2);
            u4Size = random(0xF0000) + 0x10000;
            u2PktSize = 188 + random(10) * 4;
            u1TsIdx = random(4);
        }

        Printf("Sync: %s, size: 0x%x, pkt:%d, TsIdx : %d.\n", (u4Sync == 1) ? "Ext" : "Int", u4Size, u2PktSize, u1TsIdx);

        Printf("----- Test %d -----\n", i+1);
        if (DMX_EMU_PreByte_Test(u1TsIdx, (u4Sync == 1), u4Size, u2PktSize))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_TsOutput
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TsOutput(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_TSOUT_ROUTE_T eRoute;
    DMX_TSOUT_ENDIAN_T eEndian;
    DMX_TSOUT_SYNC_CYCLE_T eSyncCycle;
    DMX_TSOUT_LATCH_EDGE_T eLatchEdge;
    DMX_TSOUT_DATAWIDTH_T eDataWidth;
    DMX_TSOUT_DATAWIDTH_T eTsInDataWidth;
    UINT32 u4Size, u4ValidSpace;
    UINT8 u1Tsindex = 2;
    BOOL fgInvert = FALSE;
    BOOL fgGating = FALSE;
    UINT8 u1TsOutindex = 0;

    if ((i4Argc < 3) || (i4Argc > 15))
    {
        Printf("%s size tsindex tsoutindex [route] [clk_invert] [clk_gating] TSout:[1|2|8bit] TSIn:[1|2|8bit] [msb|lsb] [sync] [valid] [pos|neg]\n", aszArgv[0]);
        Printf("    route: dbmloop |framerloop | extloop | ext(ex CI) | int(int CI) -> ckgen\n");
        Printf("     sync: 8 | 1\n");
        Printf("    valid: 0-3\n");
        Printf("  default: loopback 0 0 1 1 msb 8 0 neg 2\n");
        Printf("    fpga use ts index 1\n");
        return -1;
    }

    // Get size
    if (i4Argc >= 2)
    {
        u4Size = (UINT32)StrToInt(aszArgv[1]);
        Printf("Ts out Size: 0x%x, ",u4Size);
    }

    if (i4Argc >= 3)
    {
        u1Tsindex = (UINT8)StrToInt(aszArgv[2]);
        Printf("Tsindex: %d, ",u1Tsindex);
    }

    if (i4Argc >= 4)
    {
        u1TsOutindex = (UINT8)StrToInt(aszArgv[3]);
        Printf("TsOutindex: %d, ",u1TsOutindex);
    }

    // Set route.
    if (i4Argc >= 5)
    {
        if (x_strncmp(aszArgv[4], "dbmloop", sizeof("dbmloop"))==0)
        {
            eRoute = DMX_TSOUT_ROUTE_DBM_LOOPBACK;
        }
        else if (x_strncmp(aszArgv[4], "framerloop", sizeof("framerloop"))==0)
        {
            eRoute = DMX_TSOUT_ROUTE_FRAMER_LOOPBACK;
        }
        else if (x_strncmp(aszArgv[4], "extloop", sizeof("extloop"))==0)
        {
            eRoute = DMX_TSOUT_ROUTE_EXT_LOOPBACK;
        }
        else if (x_strncmp(aszArgv[4], "ext", sizeof("ext"))==0)
        {
            eRoute = DMX_TSOUT_ROUTE_EXTERNAL_CI;
        }
        else if (x_strncmp(aszArgv[4], "int", sizeof("int"))==0)
        {
            eRoute = DMX_TSOUT_ROUTE_INTERNAL_CI;
        }
        else
        {
            Printf("Unknown route!\n");
            return -1;
        }
    }
    else
    {
        eRoute = DMX_TSOUT_ROUTE_DBM_LOOPBACK;
    }
    Printf("eRounte: %d, ",eRoute);

    //fgInvert
    if (i4Argc >= 6)
    {
        if (x_strncmp(aszArgv[5], "1", sizeof("1"))==0)
        {
            fgInvert = TRUE;
        }
        else
        {
            fgInvert = FALSE;
        }
    }
    else
    {
        fgInvert = FALSE;
    }
    Printf("fgInvert: %s, ",fgInvert ? "TRUE" : "FALSE");

    //fggating
    if (i4Argc >= 7)
    {
        if (x_strncmp(aszArgv[6], "1", sizeof("1"))==0)
        {
            fgGating = TRUE;
        }
        else
        {
            fgGating = FALSE;
        }
    }
    else
    {
        fgGating = FALSE;
    }
    Printf("fgGating: %s, ",fgGating ? "TRUE" : "FALSE");

    if (i4Argc >= 8)
    {
        if (x_strncmp(aszArgv[7], "1", sizeof("1"))==0)
        {
            eDataWidth = DMX_TSOUT_DATAWIDTH_1;
        }
        else if (x_strncmp(aszArgv[7], "2", sizeof("2"))==0)
        {
            eDataWidth = DMX_TSOUT_DATAWIDTH_2;
        }
        else if (x_strncmp(aszArgv[7], "8", sizeof("8"))==0)
        {
            eDataWidth = DMX_TSOUT_DATAWIDTH_8;
        }
        else
        {
            Printf("Unknown data width!\n");
            return -1;
        }
    }
    else
    {
        eDataWidth = DMX_TSOUT_DATAWIDTH_1;
    }
    Printf("eDataWidth: %d, ",eDataWidth);

    if (i4Argc >= 9)
    {
        if (x_strncmp(aszArgv[8], "1", sizeof("1"))==0)
        {
            eTsInDataWidth = DMX_TSOUT_DATAWIDTH_1;
        }
        else if (x_strncmp(aszArgv[8], "2", sizeof("2"))==0)
        {
            eTsInDataWidth = DMX_TSOUT_DATAWIDTH_2;
        }
        else if (x_strncmp(aszArgv[8], "8", sizeof("8"))==0)
        {
            eTsInDataWidth = DMX_TSOUT_DATAWIDTH_8;
        }
        else
        {
            Printf("Unknown data width!\n");
            return -1;
        }
    }
    else
    {
        eTsInDataWidth = DMX_TSOUT_DATAWIDTH_1;
    }
    Printf("eTsInDataWidth: %d, ",eTsInDataWidth);

    // Set endian mode.
    if (i4Argc >= 10)
    {
        if (x_strncmp(aszArgv[9], "msb", sizeof("msb"))==0)
        {
            eEndian = DMX_TSOUT_ENDIAN_MSB;
        }
        else if (x_strncmp(aszArgv[9], "lsb", sizeof("lsb"))==0)
        {
            eEndian = DMX_TSOUT_ENDIAN_LSB;
        }
        else
        {
            Printf("Unknown endian mode!\n");
            return -1;
        }
    }
    else
    {
        eEndian = DMX_TSOUT_ENDIAN_MSB;
    }
    Printf("eEndian: %d, ",eEndian);

    // Set sync cycle.
    if (i4Argc >= 11)
    {
        if (x_strncmp(aszArgv[10], "8", sizeof("8"))==0)
        {
            eSyncCycle = DMX_TSOUT_SYNC_CYCLE_8;
        }
        else if (x_strncmp(aszArgv[10], "1", sizeof("1"))==0)
        {
            eSyncCycle = DMX_TSOUT_SYNC_CYCLE_1;
        }
        else
        {
            Printf("Unknown Sync cycle!\n");
            return -1;
        }
    }
    else
    {
        eSyncCycle = DMX_TSOUT_SYNC_CYCLE_8;
    }
    Printf("eSyncCycle: %d, ",eSyncCycle);

    // Set valid space.
    if (i4Argc >= 12)
    {
        u4ValidSpace = (UINT32)StrToInt(aszArgv[11]);
        if (u4ValidSpace >= 4)
        {
            Printf("Spacing %u is out of range!\n", u4ValidSpace);
            return -1;
        }
    }
    else
    {
        u4ValidSpace = 0;
    }
    Printf("u4ValidSpace: %d, ",u4ValidSpace);

    // Set latch edge.
    if (i4Argc >= 13)
    {
        if (x_strncmp(aszArgv[12], "pos", sizeof("pos"))==0)
        {
            eLatchEdge = DMX_TSOUT_LATCH_EDGE_POSITIVE;
        }
        else if (x_strncmp(aszArgv[12], "neg", sizeof("neg"))==0)
        {
            eLatchEdge = DMX_TSOUT_LATCH_EDGE_NEGATIVE;
        }
        else
        {
            Printf("Unknown latch edge!\n");
            return -1;
        }
    }
    else
    {
        eLatchEdge = DMX_TSOUT_LATCH_EDGE_NEGATIVE;
    }
    Printf("eLatchEdge: %d.\n",eLatchEdge);

    if (!_DMX_TsOut_SetRoute(u1Tsindex, u1TsOutindex, eRoute))
    {
        Printf("Failed to set route!\n");
        return -1;
    }

	if(eRoute != DMX_TSOUT_ROUTE_DBM_LOOPBACK)
   {
      if (!DMX_TsOut_SetDataWidthEx(u1TsOutindex, eDataWidth))
     {
        Printf("Failed to set data width!\n");
        return -1;
     }
     if (!DMX_TsIn_SetDataWidth(u1Tsindex, eTsInDataWidth))
     {
        Printf("Failed to set data width!\n");
        return -1;
     }
     if (!DMX_TsOut_SetEndianessEx(u1TsOutindex, eEndian))
     {
        Printf("Failed to set endian mode!\n");
        return -1;
     }
     if (!DMX_TsOut_SetSyncCycleEx(u1TsOutindex, eSyncCycle))
     {
        Printf("Failed to set sync cycle!\n");
        return -1;
     }
     if (!DMX_TsOut_SetValidCycleEx(u1TsOutindex, u4ValidSpace))
     {
        Printf("Failed to set valid space!\n");
        return -1;
     }
     if (!DMX_TsOut_SetLatchEdgeEx(u1TsOutindex, eLatchEdge))
     {
        Printf("Failed to set Latch edge!\n");
        return -1;
     }

     if (!_DMX_TsOut_SetClockInvertGating(u1TsOutindex, fgInvert, fgGating))
     {
        Printf("Failed to set Latch edge!\n");
        return -1;
     }
   }
    if (!_DMX_TsOut_TransmitData(u1Tsindex, u1TsOutindex, u4Size))
    {
        Printf("Failed\n");
    }
    else
    {
        Printf("OK\n");
    }

    return 0;
}

static INT32 _CmdDvbTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop = 1;
    UINT8 u1Type = 0, u1TsIdx;
    BOOL fgBypassFramer = FALSE;

    if (i4Argc < 4)
    {
        Printf("%s loop type Tsidx bypassframer\n", aszArgv[0]);
        Printf("   type, 0:play, 1:rec, 2:rec_enc\n");
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        return -1;
    }

    if (i4Argc > 1)
    {
        u4Loop = StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u1Type = (UINT8)StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u1TsIdx = (UINT8)StrToInt(aszArgv[3]);
    }


    //bypassframer
    if (i4Argc > 4)
    {
        if (StrToInt(aszArgv[4]) == 0)
        {
            fgBypassFramer = FALSE;
        }
        else
        {
            fgBypassFramer = TRUE;
        }
    }

    for (i=0; i<u4Loop; i++)
    {
        Printf("Loop: %d\n", i);
        if (!DMX_EMU_DvbMain(u1TsIdx, u1Type, fgBypassFramer))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i + 1);
    }
    return 0;
}


static INT32 _CmdPSILoopbackTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_FILTER_MODE_T eMode;
    UINT32 i, u4Loop, u4Mode;
    UINT8 u1TsIdx, u1TsOutIdx;

    if (i4Argc != 5)
    {
        Printf("%s [loop] [mode] [tsidx] [tsoutidx]\n", aszArgv[0]);
        Printf("   tsidx: 0, 1 . tsidx 2 , 3 is use for pvr playback! \n");
        Printf("   mode:  \n");
        Printf("    0: short mode\n");
        Printf("    1: pos_neg mode\n");
        Printf("    2: generic mode\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Mode = StrToInt(aszArgv[2]);

    if (u4Mode == 0)
    {
        eMode = DMX_FILTER_MODE_SHORT;
    }
    else if (u4Mode == 1)
    {
        eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if (u4Mode == 2)
    {
        eMode = DMX_FILTER_MODE_GENERIC;
    }
    else
    {
        return -1;
    }

    u1TsIdx = (UINT8)StrToInt(aszArgv[3]);
    u1TsOutIdx = (UINT8)StrToInt(aszArgv[4]);

    Printf("Loop :%d, Mode: %d, Tsidx :%d, Tsoutidx :%d.\n",
            u4Loop, u4Mode, u1TsIdx, u1TsOutIdx);

    for (i=0; i<u4Loop; i++)
    {
        if (!DMX_EMU_PSILoopback_Test(u1TsIdx, TRUE, eMode, u1TsOutIdx))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i + 1);
    }

    return 0;
}


static INT32 _CmdPSICALoopbackTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_FILTER_MODE_T eMode;
    UINT32 i, u4Loop, u4Mode;
    UINT8 u1TsIdx, u1TsOutIdx;

    if (i4Argc != 5)
    {
        Printf("%s [loop][mode][tsidx] [tsoutidx]\n", aszArgv[0]);
        Printf("   tsidx: 0, 1 . tsidx 2 , 3 is use for pvr playback! \n");
        Printf("   mode:  \n");
        Printf("    0: short mode\n");
        Printf("    1: pos_neg mode\n");
        Printf("    2: generic mode\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Mode = StrToInt(aszArgv[2]);

    if (u4Mode == 0)
    {
        eMode = DMX_FILTER_MODE_SHORT;
    }
    else if (u4Mode == 1)
    {
        eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if (u4Mode == 2)
    {
        eMode = DMX_FILTER_MODE_GENERIC;
    }
    else
    {
        return -1;
    }

    u1TsIdx = (UINT8)StrToInt(aszArgv[3]);
    u1TsOutIdx = (UINT8)StrToInt(aszArgv[4]);

    Printf("Loop :%d, Mode: %d, Tsidx :%d, Tsoutidx :%d.\n",
            u4Loop, u4Mode, u1TsIdx, u1TsOutIdx);

    for (i=0; i<u4Loop; i++)
    {
        if (!DMX_EMU_PSICALoopback_Test(u1TsIdx, TRUE, eMode, u1TsOutIdx))
        {
            Printf("Fail\n");
            return -1;
        }
        Printf("%d time OK\n", i + 1);
    }

    return 0;
}

//for pb_to_DBM,PB2_toDBM,PB3_toDBM bypass test .
static INT32 _CmdPVR_PB_DBM_TEST(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 1, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    BOOL fgRet, fgIntr = FALSE;
	BOOL enable_170c8_9=FALSE,enable_pid_filter=FALSE; 
    UINT8 u1TsIdx = 0, u1TsOutIdx = 0;

    if (i4Argc == 1)
    {
#if defined(CC_MT5890)
        Printf("Usage: %s Tsidx[2,3,4],enable_170c8_9[0,1]TsOutIdx[0,1,2],interrupt[0,1],enable_pid_filter[0,1][max_size=0x%x] [min_size=0x%x]\n",
#else  //MT5882
        Printf("Usage: %s Tsidx[2,3],enable_170c8_9[0,1]TsOutIdx[0,1],interrupt[0,1],enable_pid_filter[0,1][max_size=0x%x] [min_size=0x%x]\n",
#endif
		aszArgv[0], u4MaxSize, u4MinSize);
        return -1;
    }

    //Ts index
    if (i4Argc > 1)
    {
        u1TsIdx = (UINT8)StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        enable_170c8_9 = StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
         u1TsOutIdx = (UINT8)StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        fgIntr = ((UINT32)StrToInt(aszArgv[4]) == 1);
    }

	 if (i4Argc > 5)
    {
         enable_pid_filter = StrToInt(aszArgv[5]);
    }

	if(u1TsIdx == 1 || u1TsIdx==0)
	{
#if defined(CC_MT5890)
        Printf("Usage: %s Tsidx[2,3,4],enable_170c8_9[0,1]TsOutIdx[0,1,2],interrupt[0,1],enable_pid_filter[0,1][max_size=0x%x] [min_size=0x%x]\n",
		aszArgv[0], u4MaxSize, u4MinSize);
#else  //MT5882
        Printf("Usage: %s Tsidx[2,3],enable_170c8_9[0,1]TsOutIdx[0,1],interrupt[0,1],enable_pid_filter[0,1][max_size=0x%x] [min_size=0x%x]\n",
		aszArgv[0], u4MaxSize, u4MinSize);
#endif
        return -1;

	}
    Printf("\nStart PVR-PB-DBM transfer 0 test\n");

    u4Loop = 0;
    fgRet = FALSE;
#if defined(CC_MT5890)
    if(u1TsOutIdx == 2)
    {
      u1TsIdx = 4;
    }
#endif
    while (1)
    {
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188, 188);
       
        Printf("Round %u, size: %u\n", u4Loop,u4Size);

        if (enable_pid_filter == FALSE)
        {

            if (!DMX_EMU_PVR_Reset_Bypass_Transfer(u1TsIdx, enable_170c8_9,
					u1TsOutIdx,fgIntr,u4Size) )
            {
                break;
            }
        }
        else
        { 

            if (!DMX_EMU_PVR_Reset_PID_FILTER_Transfer(u1TsIdx, enable_170c8_9,
					u1TsOutIdx,FALSE,u4Size) )
            {
                break;
            }
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


// Test RP and interrupt FIFO already empty
static INT32 _CmdDMAFIFOTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 1, u4MaxSize = 0x100000, u4MinSize = 0x1000;
    UINT32 u4Loop, u4Size;
    UINT8 u1Type;
    BOOL fgRet, fgIntr = FALSE;
    BOOL fgDDI;
    UINT8 u1TsIdx = 0, u1TsOutIdx = 0;
    BOOL fgBypassFramer;

    if (i4Argc == 1)
    {
        Printf("Usage: %s Tsidx, BypassFramer, type loop intr [TsOutIdx][max_size=0x%x]"
               " [min_size=0x%x]\n", aszArgv[0], u4MaxSize, u4MinSize);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("    type, 0:ddi, 1:pvr, 2:auto\n");
        Printf("    intr, 0:polling, 1:intr\n");
        return -1;
    }

    //Ts index
    if (i4Argc > 1)
    {
        u1TsIdx = (UINT8)StrToInt(aszArgv[1]);
    }

    //bypassframer
    if (i4Argc > 2)
    {
        if (StrToInt(aszArgv[2]) == 0)
        {
            fgBypassFramer = FALSE;
        }
        else
        {
            fgBypassFramer = TRUE;
        }
    }

    if (i4Argc > 3)
    {
        u1Type = StrToInt(aszArgv[3]);
    }

    if (i4Argc > 4)
    {
        u4MaxLoop = StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        fgIntr = ((UINT32)StrToInt(aszArgv[5]) == 1);
    }

    if (i4Argc > 6)
    {
        u1TsOutIdx = (UINT8)StrToInt(aszArgv[6]);
    }

    if (i4Argc > 7)
    {
        u4MaxSize = StrToInt(aszArgv[7]);
    }

    if (i4Argc > 8)
    {
        u4MinSize = StrToInt(aszArgv[8]);
    }

    if (u4MaxSize < u4MinSize)
    {
        Printf("Incorrect size range - max: 0x%x, min: 0x%x\n", u4MaxSize,
               u4MinSize);
        return -1;
    }

    Printf("\nStart DDI transfer 0 test\n");
    Printf("Type: %s, Tsindex : %d, BypassFramer: %s, round: %u, max size: 0x%x, min size: 0x%x, u1TsOutIdx: %d.\n\n",
           (u1Type == 0) ? "DDI" : "PVR",u1TsIdx,  fgBypassFramer ? "TRUE" : "FALSE", u4MaxLoop, u4MaxSize, u4MinSize, u1TsOutIdx);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        #if 0
		u4Size = 1880; //10packet
		#else
        u4Size = DMX_Align(random(u4MaxSize - u4MinSize) + u4MinSize - 188, 188);
		#endif
        //u4Size = 1880; //10 packet

        if (u1Type == 0)
        {
            fgDDI = TRUE;
        }
        else if (u1Type == 1)
        {
            fgDDI = FALSE;
        }
        else if (u1Type == 2)
        {
            UINT32 u4Sel = u4Loop % 4;
            switch (u4Sel)
            {
            case 0:
                fgDDI = TRUE;
                fgIntr = FALSE;
                break;
            case 1:
                fgDDI = TRUE;
                fgIntr = TRUE;
                break;
            case 2:
                fgDDI = FALSE;
                fgIntr = FALSE;
                break;
            case 3:
                fgDDI = FALSE;
                fgIntr = TRUE;
                break;
            }
        }
        else
        {
            return -1;
        }

        Printf("Round %u, %s, %s - size: %u\n", u4Loop, fgDDI ? "DDI" : "PVR",
               fgIntr ? "Intr" : "Polling", u4Size);

        if (fgDDI)
        {
            if (!DMX_EMU_DDI_ResetTransfer(u1TsIdx, fgBypassFramer, u4Size, fgIntr))
            {
                break;
            }
        }
        else
        {
            if (!DMX_EMU_PVR_ResetTransfer(u1TsIdx, fgBypassFramer,u1TsOutIdx,u4Size, fgIntr))  // FPGA 1 is TSOut
            {
                break;
            }
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------

extern BOOL _DMX_EMU_RecTimerTest(UINT32 u4BufSize);
static INT32 _CmdPVRTimer(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4BufSize = 0x80000;

    if (i4Argc > 1)
    {
        u4BufSize = StrToInt(aszArgv[1]);
    }

    if (!_DMX_EMU_RecTimerTest(u4BufSize))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMX_EMU_STCCount(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4RcvPkt0,  u4RcvPkt1;
    UINT32 i, u4MsTime, u4Reg, u4Reg_Ctrl ,u4Temp ,j;
    UINT8 u1Stcidx , u1Speed;
    UINT32 u4RcvCount , u4RcvCount0 ,u4RcvSum;
    BOOL fgFirst,fgUpdown, fgRet = FALSE;
    INT32 i4Gap;
    UINT32 u4Gap , u4130Byte;

    u4MsTime = 100;
    u1Stcidx = 0;
    u4130Byte = 0x0;

    if (i4Argc < 2)
    {
        Printf("%s [ms_sec][STC_idx] [130byte_enable].\n", aszArgv[0]);
        Printf("STC_idx : 0 , 1 .\n");
        Printf("130byte_enable : 0 ,FALSE, 1 , TRUE.\n");
    }

    if (i4Argc >= 2)
    {
        u4MsTime = StrToInt(aszArgv[1]);
    }

    if (i4Argc >= 3)
    {
        u1Stcidx = StrToInt(aszArgv[2]);
    }

    if (i4Argc >= 4)
    {
        u4130Byte = StrToInt(aszArgv[3]);
    }

    if (u1Stcidx >= 2)
    {
        Printf("STC Index exceed\n");
        return -1;
    }

    if (u4130Byte >= 2)
    {
        Printf("130byte_enable : 0 ,FALSE, 1 , TRUE.\n");
        return -1;
    }

    u4Reg = (u1Stcidx == 0) ? 0x23C : 0x28C;
    u4Reg_Ctrl = (u1Stcidx == 0) ? 0x918 : 0x91C;

    // set 130 byte stc counter enable
    if (u1Stcidx == 0) //stc index 0
    {
        if (u4130Byte == 0) //normal ts packet 188 byte
        {
            u4Temp = IO_READ32(DEMUX0_BASE, u4Reg_Ctrl);
            u4Temp &= ~(0x1 << 4);
            IO_WRITE32(DEMUX0_BASE, u4Reg_Ctrl, u4Temp);
        }
        else  //130 byte ts packet
        {
            u4Temp = IO_READ32(DEMUX0_BASE, u4Reg_Ctrl);
            u4Temp |= (0x1 << 4);
            IO_WRITE32(DEMUX0_BASE, u4Reg_Ctrl, u4Temp);
        }
    }
    else  //stc index 1
    {
        if (u4130Byte == 0) //normal ts packet 188 byte
        {
            u4Temp = IO_READ32(DEMUX0_BASE, u4Reg_Ctrl);
            u4Temp &= ~(0x1 << 4);
            IO_WRITE32(DEMUX0_BASE, u4Reg_Ctrl, u4Temp);
        }
        else  //130 byte ts packet
        {
            u4Temp = IO_READ32(DEMUX0_BASE, u4Reg_Ctrl);
            u4Temp |= (0x1 << 4);
            IO_WRITE32(DEMUX0_BASE, u4Reg_Ctrl, u4Temp);
        }
    }

    u4RcvCount = 0;

    for (i=0; i<2; i++)
    {
        //set up or down mode
        u4Temp = IO_READ32(DEMUX0_BASE, u4Reg_Ctrl);
        u4Temp &= ~ (0x1 << 3);
        u4Temp |= ((UINT32)i << 3);
        IO_WRITE32(DEMUX0_BASE, u4Reg_Ctrl, u4Temp);
        fgFirst = TRUE;
        if (i == 0)
            fgUpdown = TRUE;
        else
            fgUpdown = FALSE;

        for (u1Speed = 0; u1Speed < 6 ; u1Speed++)
        {
            //set speed
            u4Temp = IO_READ32(DEMUX0_BASE, u4Reg_Ctrl);
            u4Temp &= ~ 0x7;
            u4Temp |= (UINT32)u1Speed;
            IO_WRITE32(DEMUX0_BASE, u4Reg_Ctrl, u4Temp);

            //wait for a moment
            x_thread_delay(u4MsTime);

            //get average value;
            u4RcvSum = 0x0;
            for (j = 0 ; j < 3 ; j++)
            {
                //reset counter
                u4Temp = IO_READ32(DEMUX0_BASE, u4Reg);
                u4Temp &= ~ 0xFFFFFFFF;
                IO_WRITE32(DEMUX0_BASE, u4Reg, u4Temp);

                //wait for a moment
                x_thread_delay(10);

                //read first time
                u4RcvPkt0 = IO_READ32(DEMUX0_BASE, u4Reg) ;

                x_thread_delay(u4MsTime);

                //read after 100 ms
                u4RcvPkt1 = IO_READ32(DEMUX0_BASE, u4Reg) ;

#if 1
                u4RcvSum += (u4RcvPkt1 - u4RcvPkt0);
#else
                u4RcvCount = u4RcvPkt1 - u4RcvPkt0;
                Printf("The %d Time,First: %d, Second: %d, counter : %u.\n",j , u4RcvPkt0 , u4RcvPkt1 ,u4RcvCount);
                u4RcvSum += u4RcvCount;
#endif
            }
            u4RcvCount = u4RcvSum / 3;

            if (!fgFirst)
            {
                if (fgUpdown)
                {
                    i4Gap = 2 * u4RcvCount0 - u4RcvCount;
                    u4Gap = abs(i4Gap);
                    Printf("The Gap : %u.\n",u4Gap);
                    if (u4Gap > 1500)
                    {
                        //break;
                    }
                }
                else
                {
                    i4Gap = 2 * u4RcvCount - u4RcvCount0;
                    u4Gap = abs(i4Gap);
                    Printf("The Gap : %u.\n",u4Gap);
                    if (u4Gap > 500)
                    {
                        //break;
                    }
                }
            }
            u4RcvCount0 = u4RcvCount;
            fgFirst = FALSE;

            Printf("STC index: %u,up/down : %u, Speed : %u, Period: %u ms, First time: %u , Second time :%u, Difference: %u.\n",
                   u1Stcidx,i, u1Speed,u4MsTime, u4RcvPkt0, u4RcvPkt1 ,u4RcvCount);

        }
    }
    if ( (i == 0x2) && (u1Speed == 0x6))
        fgRet = TRUE;
    else
        fgRet = FALSE;

    Printf("%s\n", fgRet ? "PASS" : "FAIL");
    return (fgRet ? 0 : -1);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------

static INT32 _CmdDBM_Auto_SwitchTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    BOOL fgEnable_AutoSwitch = FALSE;
    UINT8 u1TsOutIdx = 0;

    if (i4Argc != 4)
    {
        Printf("%s loop u1TsOutIdx enable/disable\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);

    u1TsOutIdx = (UINT8)StrToInt(aszArgv[2]);

    if (x_strncmp(aszArgv[3], "enable", sizeof("enable"))==0)
    {
        fgEnable_AutoSwitch = TRUE;
    }
    else if (x_strncmp(aszArgv[3], "disable", sizeof("disable"))==0)
    {
        fgEnable_AutoSwitch = FALSE;
    }
    else
    {
        Printf("%s loop u1TsOutIdx enable/disable\n", aszArgv[0]);
        return -1;
    }

    for (i=0; i<u4Loop; i++)
    {
        Printf(" -----Test %d, u1TsOutIdx : %d.  -----\n", i+1, u1TsOutIdx);
        if (DMX_EMU_DBM_Auto_SwitchTest(u1TsOutIdx, fgEnable_AutoSwitch))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _Cmd130byte_TsTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop , u4Sync;
    UINT8 u1Tsindex;

    if (i4Argc != 4)
    {
        Printf("%s loop sync tsindex\n", aszArgv[0]);
        Printf("    sync, 0: internal, 1: external\n");
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Sync = StrToInt(aszArgv[2]);
    u1Tsindex =StrToInt(aszArgv[3]);

    Printf(" ---Sync : %s -- Tsindex: %d.\n",(u4Sync == 1) ? "Ext" : "Int" , u1Tsindex);

    for (i=0; i<u4Loop; i++)
    {
        Printf(" -----Test %d -----\n", i+1);
        if (DMX_Emu_130TsTest((u4Sync == 1),u1Tsindex))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _Cmd130byte_TsoutTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop ,u4Sync;
    UINT8 u1TsIndex;
    UINT8 u1TsOutIndex;

    if (i4Argc != 5)
    {
        Printf("%s loop fgExtSync tsindex tsoutidx\n", aszArgv[0]);
        Printf("    sync, 0: internal, 1: external\n");
        Printf("    tsindex: 0 , 1.\n");
        Printf("    tsoutindex: 0 , 1.\n");
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Sync = StrToInt(aszArgv[2]);
    u1TsIndex = StrToInt(aszArgv[3]);
    u1TsOutIndex = StrToInt(aszArgv[4]);

    Printf(" ---Sync : %s ,tsidx: %d, tsoutidx: %d.--- .\n",(u4Sync == 1) ? "Ext" : "Int",u1TsIndex,u1TsOutIndex);

    for (i=0; i<u4Loop; i++)
    {
        Printf(" -----Test %d -----\n", i+1);
        if (DMX_Emu_Tsout130_TransmitData((u4Sync == 1), u1TsIndex, u1TsOutIndex))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdMulti_PacketSizeTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT32 u4Framer0Reset, u4Framer1Reset;
    UINT8 utTsOutIdx;

    if (i4Argc != 5)
    {
        Printf("%s loop tsoutidx, fgFramer0Reset fgFramer1Reset\n", aszArgv[0]);
        Printf("    tsoutidx : 0 , 1.\n");
        Printf("    fgFramer0Reset, 0: FALSE, 1: TRUE\n");
        Printf("    fgFramer1Reset, 0: FALSE, 1: TRUE\n");
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    utTsOutIdx = StrToInt(aszArgv[2]);
    u4Framer0Reset = StrToInt(aszArgv[3]);
    u4Framer1Reset = StrToInt(aszArgv[4]);

    Printf(" ---Framer0_Reset : %s --- .\n",(u4Framer0Reset == 1) ? "TRUE" : "FALSE");
    Printf(" ---Framer1_Reset : %s --- .\n",(u4Framer1Reset == 1) ? "TRUE" : "FALSE");
    Printf(" ---Tsoutidx : %d --- .\n", utTsOutIdx);

    for (i=0; i<u4Loop; i++)
    {
        Printf(" -----Test %d -----\n", i+1);
        if (DMX_EMU_Multi_PacketSize_Test((u4Framer0Reset == 1),(u4Framer1Reset == 1), utTsOutIdx))
        {
            Printf("OK\n");
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}

EXTERN BOOL _DMX_GetToDecoderStatus(void);
EXTERN void _DMX_SetToDecoder(BOOL fgToDecoder);
static INT32 _CmdSetToDecoder(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Type;

    if (i4Argc != 2)
    {
        Printf("%s type!\n", aszArgv[0]);
        Printf("type, 0: FALSE, 1: TRUE\n");
        Printf("Current ToDecoder Status: %s.\n", _DMX_GetToDecoderStatus() ? "TRUE" : "FALSE");
        return 0;
    }

    u4Type = StrToInt(aszArgv[1]);

    Printf(" ---DMX_SetToDecoder : %s --- .\n",(u4Type == 1) ? "TRUE" : "FALSE");

    _DMX_SetToDecoder(u4Type == 1);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdDDI_IOMMU_Test(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsIndex;
    BOOL fgBypassFramer;
    UINT8 u1Mode;

    if (i4Argc != 5)
    {
        Printf("%s ,loop, TsIdx, BypassFramer, Mode.\n", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("Mode(1, 2, 3, 4, 5).\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u1TsIndex = (UINT8)StrToInt(aszArgv[2]);

    if (StrToInt(aszArgv[3]) == 0)
    {
        fgBypassFramer = FALSE;
    }
    else
    {
        fgBypassFramer = TRUE;
    }

    u1Mode = (UINT8)StrToInt(aszArgv[4]);

    Printf("--Loop: %d, TsIdx: %d, BypassFramer: %s, Mode: %d.\n",
           u4Loop, u1TsIndex, fgBypassFramer ? "TRUE" : "FALSE",u1Mode);

    for (i=0; i<u4Loop; i++)
    {
        Printf(" -------------- Test %d -------------\n", i+1);
        if (DMX_EMU_DDI_IOMMU(u1TsIndex, fgBypassFramer, u1Mode))
        {
            Printf(">>>>> Test :%d OK <<<<<<\n",i+1);
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdDMX_IOMMU_Test(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsIndex;
    BOOL fgBypassFramer;
    UINT8 u1Mode;

    if (i4Argc != 5)
    {
        Printf("%s ,loop, TsIdx, BypassFramer, Mode.\n", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("Mode(1, 2, 3, 4).\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u1TsIndex = (UINT8)StrToInt(aszArgv[2]);

    if (StrToInt(aszArgv[3]) == 0)
    {
        fgBypassFramer = FALSE;
    }
    else
    {
        fgBypassFramer = TRUE;
    }

    u1Mode = (UINT8)StrToInt(aszArgv[4]);

    Printf("--Loop: %d, TsIdx: %d, BypassFramer: %s, Mode: %d.\n",
           u4Loop, u1TsIndex, fgBypassFramer ? "TRUE" : "FALSE",u1Mode);

    for (i=0; i<u4Loop; i++)
    {
        Printf(" -------------- Test %d -------------\n", i+1);
        if (DMX_EMU_DMX_IOMMU(u1TsIndex, fgBypassFramer, u1Mode))
        {
            Printf(">>>>> Test :%d OK <<<<<<\n",i+1);
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdDMXDDI_IOMMU_Test(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsIndex;
    BOOL fgBypassFramer;
    UINT8 u1Mode;

    if (i4Argc != 5)
    {
        Printf("%s ,loop, TsIdx, BypassFramer, Mode.\n", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("Mode(1, 2, 3).\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u1TsIndex = (UINT8)StrToInt(aszArgv[2]);

    if (StrToInt(aszArgv[3]) == 0)
    {
        fgBypassFramer = FALSE;
    }
    else
    {
        fgBypassFramer = TRUE;
    }

    u1Mode = (UINT8)StrToInt(aszArgv[4]);

    Printf("--Loop: %d, TsIdx: %d, BypassFramer: %s, Mode: %d.\n",
           u4Loop, u1TsIndex, fgBypassFramer ? "TRUE" : "FALSE",u1Mode);

    for (i=0; i<u4Loop; i++)
    {
        Printf(" -------------- Test %d -------------\n", i+1);
        if (DMX_EMU_DMXDDI_IOMMU(u1TsIndex, fgBypassFramer, u1Mode))
        {
            Printf(">>>>> Test :%d OK <<<<<<\n\n",i+1);
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdIOMMU_Performance_Test(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsIndex;
    BOOL fgBypassFramer;
    BOOL fgIommu1,fgIommu2,fgIommu3;
    UINT8 u1Mode;

    if (i4Argc != 7)
    {
        Printf("%s ,loop, TsIdx, BypassFramer, fgIommu1, fgIommu2, fgIommu3.\n", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("fgIommu1: 0(false), 1(true).\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u1TsIndex = (UINT8)StrToInt(aszArgv[2]);

    if (StrToInt(aszArgv[3]) == 0)
    {
        fgBypassFramer = FALSE;
    }
    else
    {
        fgBypassFramer = TRUE;
    }

    if (StrToInt(aszArgv[4]) == 0)
    {
        fgIommu1 = FALSE;
    }
    else
    {
        fgIommu1 = TRUE;
    }

    if (StrToInt(aszArgv[5]) == 0)
    {
        fgIommu2 = FALSE;
    }
    else
    {
        fgIommu2 = TRUE;
    }

    if (StrToInt(aszArgv[6]) == 0)
    {
        fgIommu3 = FALSE;
    }
    else
    {
        fgIommu3 = TRUE;
    }

    Printf("--Loop: %d, TsIdx: %d, BypassFramer: %s, fgIommu1: %s,fgIommu2: %s, fgIommu3: %s--\n",
           u4Loop, u1TsIndex, fgBypassFramer ? "TRUE" : "FALSE",
           fgIommu1 ? "TRUE" : "FALSE", fgIommu2 ? "TRUE" : "FALSE",
           fgIommu3 ? "TRUE" : "FALSE");


    for (i=0; i<u4Loop; i++)
    {
        Printf(" -------------- Test %d -------------\n", i+1);
        if (DMX_EMU_IOMMU_PerformanceTest(u1TsIndex, fgBypassFramer, fgIommu1, fgIommu2, fgIommu3))
        {
            Printf(">>>>> Test :%d OK <<<<<<\n\n",i+1);
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}


static INT32 _CmdFrmaerUnlockTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0;
    UINT32 u4Loop, u4Size,  u4Mode = 0x2;
    UINT8 u1Tsidx;
    BOOL fgRet;
    PUL_MODE_T eMode;

    if (i4Argc == 1)
    {
        Printf("Usage: %s , Tsindex , [loop(0=infinite)]  [mode(0 full fush,1 half, 2 full pull)].\n", aszArgv[0]);
        Printf("Notice:mode type is optional,default is full pull mode,\n"
               "The other two type is just for test!\n");
        return -1;
    }

    //txindex
    if (i4Argc > 1)
    {
        u1Tsidx = StrToInt(aszArgv[1]);
    }

    //loop
    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    //mode
    if (i4Argc > 3)
    {
        u4Mode = StrToInt(aszArgv[3]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }


    Printf("\nStart Frmaer Unlock test\n");
    Printf("TS index: %u, round: %u, \n\n",u1Tsidx, u4MaxLoop);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        u4Size = 1100 *188; //1100 Ts packet

        Printf("Round %u - size: %u\n", u4Loop, u4Size);

        if (!DMX_EMU_FramerUnlock_Test(u1Tsidx, u4Size, eMode))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdDMXVideoESTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0;
    UINT32 u4Loop;
    BOOL fgRet, fgBypassFramer;
    UINT32 u4Mode = 0x2;
    PUL_MODE_T eMode;
    UINT8 u1Tsidx;

    if (i4Argc == 1)
    {
        Printf("Usage: %s , Tsindex ,loop(0=infinite) bypassframer, [mode(0 full fush,1 half, 2 full pull)] \n", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("Notice:mode type is optional,default is full pull mode,\n"
               "The other two type is just for test!\n");
        return -1;
    }

    //txindex
    if (i4Argc > 1)
    {
        u1Tsidx = (UINT8)StrToInt(aszArgv[1]);
    }

    //MaxLoop
    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    //bypassframer
    if (i4Argc > 3)
    {
        if (StrToInt(aszArgv[3]) == 0)
        {
            fgBypassFramer = FALSE;
        }
        else
        {
            fgBypassFramer = TRUE;
        }
    }

    //Mode
    if (i4Argc > 4)
    {
        u4Mode = StrToInt(aszArgv[4]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }


    Printf("\nStart Video Es test 0\n");
    Printf("TS index: %u, round: %u, bypass framer : %s\n\n",
           u1Tsidx, u4MaxLoop, fgBypassFramer ? "TRUE" : "FALSE");

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        Printf("------------Round %u ------------------\n", u4Loop);

        if (!DMX_VideoEstest(u1Tsidx, eMode, fgBypassFramer))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}

static INT32 _CmdSecureBufferTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0;
    UINT32 u4Loop;
    BOOL fgRet, fgBypassFramer;
    UINT32 u4Mode = 0x2;
    PUL_MODE_T eMode;
    UINT8 u1Tsidx;

    if (i4Argc == 1)
    {
        Printf("Usage: %s , Tsindex ,loop(0=infinite) bypassframer, [mode(0 full fush,1 half, 2 full pull)] ", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("Notice:mode type is optional,default is full pull mode,\n"
               "The other two type is just for test!\n");
        return -1;
    }

    //txindex
    if (i4Argc > 1)
    {
        u1Tsidx = (UINT8)StrToInt(aszArgv[1]);
    }

    //MaxLoop
    if (i4Argc > 2)
    {
        u4MaxLoop = StrToInt(aszArgv[2]);
    }

    //bypassframer
    if (i4Argc > 3)
    {
        if (StrToInt(aszArgv[3]) == 0)
        {
            fgBypassFramer = FALSE;
        }
        else
        {
            fgBypassFramer = TRUE;
        }
    }

    //Mode
    if (i4Argc > 4)
    {
        u4Mode = StrToInt(aszArgv[4]);
    }

    if (u4Mode == 0)
    {
        eMode = FULL_PUSH_MODE;
        Printf("Current mode: FULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 1)
    {
        eMode = HALF_PULL_PUSH_MODE;
        Printf("Current mode: HALF_PULL_PUSH_MODE.\n");
    }
    else if (u4Mode == 2)
    {
        eMode = FULL_PULL_MODE;
        Printf("Current mode: FULL_PULL_MODE.\n");
    }
    else
    {
        Printf("Please input correct mode (0,1,2)\n");
        return -1;
    }


    Printf("\nStart SecureBuffer test 0\n");
    Printf("TS index: %u, round: %u, bypass framer : %s\n\n",
           u1Tsidx, u4MaxLoop, fgBypassFramer ? "TRUE" : "FALSE");

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        Printf("------------Round %u ------------------\n", u4Loop);

        if (!DMX_EMU_SecureBuffer_Test(u1Tsidx, eMode, fgBypassFramer))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdTsoutToCAoutputBufferTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0;
    UINT32 u4Loop;
    BOOL fgRet, fgBypassFramer;
    UINT8 u1Tsidx, u1TsOutIdx = 0;

    if (i4Argc == 1)
    {
        Printf("Usage: %s , loop(0=infinite), Tsindex , bypassframer, TsOutIdx\n", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        return -1;
    }

    //MaxLoop
    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    //txindex
    if (i4Argc > 2)
    {
        u1Tsidx = (UINT8)StrToInt(aszArgv[2]);
    }

    //bypassframer
    if (i4Argc > 3)
    {
        if (StrToInt(aszArgv[3]) == 0)
        {
            fgBypassFramer = FALSE;
        }
        else
        {
            fgBypassFramer = TRUE;
        }
    }

    //Mode
    if (i4Argc > 4)
    {
        u1TsOutIdx = (UINT8)StrToInt(aszArgv[4]);
    }

    Printf("\nStart Tsout toCAoutputBuffer.\n");
    Printf("TS index: %u, round: %u, bypass framer : %s, Ts Out Idx: %u.\n\n",
           u1Tsidx, u4MaxLoop, fgBypassFramer ? "TRUE" : "FALSE",u1TsOutIdx);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        Printf("------------Round %u ------------------\n", u4Loop);

        if (!DMX_TsOut_ToCaOutput(u1Tsidx, fgBypassFramer, u1TsOutIdx))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}


static INT32 _CmdTsoutToFTUPTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MaxLoop = 0;
    UINT32 u4Loop;
    BOOL fgRet, fgBypassFramer;
    UINT8 u1Tsidx, u1TsOutIdx = 0;

    if (i4Argc == 1)
    {
        Printf("Usage: %s , loop(0=infinite), Tsindex , bypassframer, TsOutIdx\n", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        return -1;
    }

    //MaxLoop
    if (i4Argc > 1)
    {
        u4MaxLoop = StrToInt(aszArgv[1]);
    }

    //txindex
    if (i4Argc > 2)
    {
        u1Tsidx = (UINT8)StrToInt(aszArgv[2]);
    }

    //bypassframer
    if (i4Argc > 3)
    {
        if (StrToInt(aszArgv[3]) == 0)
        {
            fgBypassFramer = FALSE;
        }
        else
        {
            fgBypassFramer = TRUE;
        }
    }

    //Mode
    if (i4Argc > 4)
    {
        u1TsOutIdx = (UINT8)StrToInt(aszArgv[4]);
    }

    Printf("\nStart Tsout to FTUP Buffer.\n");
    Printf("TS index: %u, round: %u, bypass framer : %s, Ts Out Idx: %u.\n\n",
           u1Tsidx, u4MaxLoop, fgBypassFramer ? "TRUE" : "FALSE",u1TsOutIdx);

    u4Loop = 0;
    fgRet = FALSE;
    while (1)
    {
        Printf("------------Round %u ------------------\n", u4Loop);

        if (!DMX_TsOut_ToFtup(u1Tsidx, fgBypassFramer, u1TsOutIdx))
        {
            break;
        }

        u4Loop++;
        if ((u4MaxLoop != 0) && (u4Loop >= u4MaxLoop))
        {
            fgRet = TRUE;
            break;
        }
    }

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdDDI_Transfer_Test(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsIndex;
    BOOL fgBypassFramer;
    UINT8 u1Mode;

    if (i4Argc != 5)
    {
        Printf("%s ,loop, TsIdx, BypassFramer, Mode.\n", aszArgv[0]);
        Printf("bypassframer(0:not bypass, 1:bypass), only for framer 2 and framer 3.\n");
        Printf("Mode(1, 2, 3).\n");
        return -1;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u1TsIndex = (UINT8)StrToInt(aszArgv[2]);

    if (StrToInt(aszArgv[3]) == 0)
    {
        fgBypassFramer = FALSE;
    }
    else
    {
        fgBypassFramer = TRUE;
    }

    u1Mode = (UINT8)StrToInt(aszArgv[4]);

    Printf("--Loop: %d, TsIdx: %d, BypassFramer: %s, Mode: %d.\n",
           u4Loop, u1TsIndex, fgBypassFramer ? "TRUE" : "FALSE",u1Mode);

    for (i=0; i<u4Loop; i++)
    {
        Printf(" -------------- Test %d -------------\n", i+1);
        if (DMX_EMU_DDI_TransferTest(u1Mode, u1TsIndex, fgBypassFramer))
        {
            Printf(">>>>> Test :%d OK <<<<<<\n",i+1);
        }
        else
        {
            Printf("Fail\n");
            return -1;
        }
    }
    return 0;
}

EXTERN BOOL _DMX_EMU_VCTTest(u1TsIndex, u1Pidx, u2Pid);
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdVCT_Test(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4Loop;
    UINT8 u1TsIndex;
    UINT8 u1Pidx;
    UINT16 u2Pid;
    
    if (i4Argc != 4)
    {
        Printf("%s ,TsIdx, PidIdx, Pid.\n", aszArgv[0]);
        return -1;
    }

    u1TsIndex = (UINT8)StrToInt(aszArgv[1]);
    u1Pidx = (UINT8)StrToInt(aszArgv[2]);
    u2Pid = (UINT16)StrToInt(aszArgv[3]);
    
    if (_DMX_EMU_VCTTest(u1TsIndex, u1Pidx, u2Pid))
    {
        Printf(">>>>> VCT Test OK <<<<<<\n");
    }
    else
    {
        Printf("VCT Test Fail\n");
        return -1;
    }
 
    return 0;
}

#ifdef TEST_MODE
INT32 _DMXCLI_Ndo (INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_EMU_ND_Offset();

    return 0;
}

//////////////////////////////////////////////////////
//  4 Framer test
//////////////////////////////////////////////////////
UINT8 u1Inst0, u1Inst1, u1Inst2, u1Inst3;
UINT8 u1Pidx0, u1Pidx1, u1Pidx2, u1Pidx3;
UINT32 u4RecordBuffer;
UINT32 u4RecordSize;
static HANDLE_T _hFvrQueue = NULL_HANDLE;
static HANDLE_T _hFvrThread = NULL_HANDLE;


typedef struct
{
    UINT8 u1TsIdx;
    UINT32 u4Addr;
    UINT32 u4Size;
} FVR_MSG_T;

static VOID _FvrThread(VOID* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        static FVR_MSG_T rMsg;
        INT32 i4Ret;
        SIZE_T zMsgSize;
        UINT16 u2QueueIndex;
        DMX_MM_DATA_T rData;

        zMsgSize = sizeof (rMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hFvrQueue, 1, X_MSGQ_OPTION_WAIT);

        //move for pvr
        _DMX_PVRPlay_UpdateWp(0, rMsg.u4Addr, rMsg.u4Size);
        _DMX_PVRPlay_Start(0);

        //move for ddi
        #if 1
        x_memset(&rData, 0, sizeof(rData));
        rData.u1Idx = u1Pidx3;
        rData.u4BufStart = u4RecordBuffer;
        rData.u4BufEnd = u4RecordBuffer + u4RecordSize;
        rData.u4StartAddr = rMsg.u4Addr;
        rData.u4FrameSize = rMsg.u4Size;
        DMX_MUL_MoveData(u1Inst3, &rData);
        #endif

        // unlock record buffer
        FVR_UnlockBuffer(rMsg.u1TsIdx, rMsg.u4Addr, rMsg.u4Size);

    }   // while (1)
}

static BOOL DMX_VideoPesHandler(const DMX_PES_MSG_T* prPes)
{
    return DMX_UpdateReadPointer(prPes->u1Pidx, prPes->u4Wp, prPes->u4FrameAddr);
}

static BOOL DMX_AudioPesHandler(const DMX_AUDIO_PES_T* prPes)
{
    return DMX_UpdateReadPointer(prPes->u1PidIndex, prPes->u4Wp, prPes->u4PesRp);
}

static void DMX_UpdateWp(UINT8 u1PidIndex, UINT32 u4Wp)
{

}

static void FVR_GBLCallback(FVR_GBL_NOTIFY_CODE_T eCode,
                                       UINT32 u4Data, const void* pvTag)
{
    FVR_GBL_TIMENOTIFY_INFO_T *prInfo;
    FVR_MSG_T rPes;

    switch(eCode)
    {
    case FVR_GBL_NFY_TIME:
        prInfo = (FVR_GBL_TIMENOTIFY_INFO_T*)u4Data;
        LOG(3, "recorded addr: 0x%x size: 0x%x\n", prInfo->u4Addr, prInfo->u4Size);
        rPes.u4Addr = prInfo->u4Addr;
        rPes.u4Size = prInfo->u4Size;
        rPes.u1TsIdx = prInfo->u1TsIndex;
        if (x_msg_q_send(_hFvrQueue, (VOID*)&rPes, sizeof (FVR_MSG_T),
            128) != OSR_OK)
        {
            LOG(3, "Demux queue full!\n");
        }
        break;

    case FVR_GBL_NFY_PKTCOUNT:
        // Don't use
        ASSERT(0);
        break;

    case FVR_GBL_NFY_OVERFLOW:
        LOG(0, "overflow...\n");
        break;

    default:
        // Unexpected
        ASSERT(0);
        break;
    }
}


// 1. need to enable nim
// 2. need to set MPV_MAX_ES as 2
static UINT16 TEST_PID = 65;

static INT32 _DMXCLI_Test4Framer(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_PID_T rPid;
    DMX_DECODER_CALLBACKS_T rCallback;
    DMX_MM_T rMM;

    FVR_PID_T rFVRPid;
    FVR_GBL_T rGblPid;
    DMX_PVR_PLAY_T rPlay;
    UINT32 u4Flags;

    FVR_TIMESTAMP_T rTimeStamp;

    rCallback.pfnSendVideoPes = DMX_VideoPesHandler;
    rCallback.pfnSendAudioPes = DMX_AudioPesHandler;
    rCallback.pfnUpdateVideoWp = DMX_UpdateWp;

    CLI_Parser("nim.go atsc");
    CLI_Parser("nim.freq 551000");
    CLI_Parser("fbm.i");
    CLI_Parser("vdec.i");
    CLI_Parser("d.i");
    CLI_Parser("d.sfe 0 0");
    CLI_Parser("d.sfe 0 1");
    //CLI_Parser("d.sfe 0 3");

    // Create picture queues
    if (x_msg_q_create(&_hFvrQueue, "FVR",
        sizeof (FVR_MSG_T), 1024) != OSR_OK)
    {
        LOG(1, "Fail to create picture queue!\n");
        return FALSE;
    }

    // Create picture thread
    if (x_thread_create(&_hFvrThread, "FvrThread",
        DMX_THREAD_STACK_SIZE, DMX_THREAD_PRIORITY, _FvrThread, 0,
        NULL) != OSR_OK)
    {
        LOG(1, "Fail to create DMX thread!\n");
        return FALSE;
    }

    // set the framer 0 pid for tuner 0
    u1Inst0 = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_TUNER_0);
    DMX_MUL_SetInstType(u1Inst0, DMX_IN_BROADCAST_TS);
    DMX_MUL_SetTSIdx(u1Inst0);
    u1Pidx0 = DMX_MUL_GetAvailablePidx(u1Inst0);
    DMX_MUL_SetDecoderCallbacks(u1Inst0, &rCallback);

    x_memset(&rPid, 0, sizeof(rPid));
    rPid.u1TsIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u2Pid = TEST_PID;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rPid.fgAllocateBuffer = FALSE;
    rPid.u4BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(0x100000,
                64);
    rPid.u4BufSize = 0x100000;
    rPid.fgPrimary = TRUE;
    rPid.u1ChannelId = 0;

    if(!DMX_SetPid(u1Pidx0, DMX_PID_FLAG_ALL, &rPid))
    {
        Printf("fail to set pid\n");
    }


    // set the framer 1 pid for ddi
    u1Inst1 = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_TUNER_1);
    DMX_MUL_SetInstType(u1Inst1, DMX_IN_BROADCAST_TS);
    DMX_MUL_SetTSIdx(u1Inst1);
    u1Pidx1 = DMX_MUL_GetAvailablePidx(u1Inst1);
    DMX_MUL_SetDecoderCallbacks(u1Inst1, &rCallback);

    x_memset(&rPid, 0, sizeof(rPid));
    rPid.u1TsIndex = 1;
    rPid.fgEnable = TRUE;
    rPid.u2Pid = TEST_PID;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rPid.fgAllocateBuffer = FALSE;
    rPid.u4BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(0x100000,
                64);
    rPid.u4BufSize = 0x100000;
    rPid.fgPrimary = TRUE;
    rPid.u1ChannelId = 1;

    if(!DMX_SetPid(u1Pidx1, DMX_PID_FLAG_ALL, &rPid))
    {
        Printf("fail to set pid\n");
    }

    // set the pvr playback data
    u1Inst2 = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_BUFAGENT_0);
    DMX_MUL_SetInstType(u1Inst2, DMX_IN_PLAYBACK_TS);
    u1Pidx2 = DMX_MUL_GetAvailablePidx(u1Inst2);
    DMX_MUL_SetDecoderCallbacks(u1Inst2, &rCallback);
    DMX_SetTSMode(u1Inst2, DMX_TSFMT_TIMESHIFT);

    x_memset(&rPid, 0, sizeof(rPid));
    rPid.u1TsIndex = 2;
    rPid.fgEnable = TRUE;
    rPid.u2Pid = TEST_PID;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rPid.fgAllocateBuffer = FALSE;
    rPid.u4BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(0x100000,
                64);
    rPid.u4BufSize = 0x100000;
    rPid.fgPrimary = TRUE;
    rPid.u1ChannelId = 2;

    if(!DMX_SetPid(u1Pidx2, DMX_PID_FLAG_ALL, &rPid))
    {
        Printf("fail to set pid\n");
    }

    u4RecordBuffer = (UINT32)BSP_AllocAlignedDmaMemory(0x100000, 192);
    u4RecordSize = 0x100000 - (0x100000 % 192);

    VERIFY(FVR_Init());
    x_memset((VOID*)&rGblPid, 0x0, sizeof(FVR_GBL_T));
    rGblPid.fgAllocateBuffer = FALSE;
    rGblPid.u4BufStart = u4RecordBuffer;
    rGblPid.u4BufSize = u4RecordSize;
    rGblPid.u4TimeToNotify = 500;
    rGblPid.pfnNotifyFunc = FVR_GBLCallback;
    if(!FVR_SetGBL(0, FVR_GBL_FLAG_ALL, &rGblPid))
    {
        return FALSE;
    }

    if(!FVR_GetGBL(0, FVR_GBL_FLAG_ALL, &rGblPid))
    {
        return FALSE;
    }

    // get the real record buffer size
    u4RecordBuffer = rGblPid.u4BufStart;
    u4RecordSize = rGblPid.u4BufSize;

    // Enabe Record timestamp
    rTimeStamp.fgEnable = TRUE;
    rTimeStamp.u2ClkDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    rTimeStamp.u4Mask = 0xFFFFFFFF;
    rTimeStamp.u4Counter= 0x0;
    FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);

    x_memset((void*)&rFVRPid, 0, sizeof(rFVRPid));
    rFVRPid.u1TsIndex = 0;
    rFVRPid.fgEnable = TRUE;
	rFVRPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rFVRPid.u2Pid = TEST_PID;

    if(!FVR_SetPid(0, FVR_PID_FLAG_ALL & ~FVR_PID_FLAG_KEY_LEN, &rFVRPid))
    {
        return FALSE;
    }

    // Init PVR
    if (!_DMX_PVRPlay_Init())
    {
        Printf("Failed to initialize PVR!\n");
        return FALSE;
    }

    // Set PVR playback
    x_memset(&rPlay, 0, sizeof(rPlay));
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = u4RecordBuffer;
    rPlay.u4BufSize = u4RecordSize;
    rPlay.pfnPVRNotify = NULL;
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = TRUE;
    rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    rPlay.u4ThresholdSize = 192*10;
    rPlay.eMode = DMX_PVR_PLAY_STREAM;
    u4Flags = PVRPLAY_FLAGS_THRESHOLD | PVRPLAY_FLAGS_BUFFER | PVRPLAY_FLAGS_CALLBACK | PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP;
    if (!_DMX_PVRPlay_Set(0, u4Flags, &rPlay))
    {
        Printf("Failed to set PVR properties!\n");
        //fgRet = FALSE;
    }

    VERIFY(_DMX_PVRPlay_FlushBuf(0));

    // set framer3 for DDI
    u1Inst3 = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_BUFAGENT_1);
    DMX_MUL_SetInstType(u1Inst3, DMX_IN_PLAYBACK_MM);
    u1Pidx3 = DMX_MUL_GetAvailablePidx(u1Inst3);
    DMX_MUL_SetDecoderCallbacks(u1Inst3, &rCallback);

    x_memset(&rMM, 0, sizeof(rMM));
    rMM.fgEnable = TRUE;
    rMM.fgSearchStartCode = TRUE;
    rMM.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rMM.fgAllocBuf = FALSE;
    rMM.u4BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(0x100000,
                64);
    rMM.u4BufSize = 0x100000;
    rMM.u1ChannelId = 3;

    if(!DMX_MM_Set(u1Pidx3, DMX_MM_FLAG_ALL, &rMM))
    {
        Printf("fail to set pid\n");
    }

    // start to record...
    VERIFY(_FVR_Start(0));

    return 0;
}


//////////////////////////////////////////////////////
//  Audio PES
//////////////////////////////////////////////////////

static HANDLE_T _hAPESQueue = NULL_HANDLE;
static HANDLE_T _hAPESThread = NULL_HANDLE;

static VOID _APESThread(VOID* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        static DMX_NOTIFY_INFO_PES_T rMsg;
        INT32 i4Ret;
        SIZE_T zMsgSize;
        UINT16 u2QueueIndex;
        DMX_MM_DATA_T rData;

        zMsgSize = sizeof (rMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hAPESQueue, 1, X_MSGQ_OPTION_WAIT);

        _DMX_PES_UnlockBuffer(u1Pidx1, rMsg.u1SerialNumber, rMsg.u4DataSize, rMsg.u4DataAddr);

    }   // while (1)
}


static BOOL _DmxCallback(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag)
{
    DMX_NOTIFY_INFO_PES_T* prPESInfo;

    // Check if PID is really active
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    // Check notification code
    switch (eCode)
    {
    case DMX_NOTIFY_CODE_OVERFLOW:
        break;

    case DMX_NOTIFY_CODE_SCRAMBLE_STATE:
        break;

    case DMX_NOTIFY_CODE_PES:
        if (x_msg_q_send(_hAPESQueue, (VOID*)u4Data, sizeof (DMX_NOTIFY_INFO_PES_T),
            128) != OSR_OK)
        {
            LOG(3, "APES queue full!\n");
        }
        break;

    default:
        // Unexpected notification
        ASSERT(0);
        break;
    }

    return TRUE;
}


static INT32 _DMXCLI_AudioPes(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_PID_T rPid;
    DMX_DECODER_CALLBACKS_T rCallback;
    UINT32 u4Action;
    UINT32 u4Value;
    rCallback.pfnSendVideoPes = DMX_VideoPesHandler;
    rCallback.pfnSendAudioPes = DMX_AudioPesHandler;
    rCallback.pfnUpdateVideoWp = DMX_UpdateWp;

    u4Action = (UINT8)StrToInt(aszArgv[1]);
    u4Value = (UINT8)StrToInt(aszArgv[2]);

    if(u4Action == 0) // initial dtv
    {
        CLI_Parser("nim.go atsc");
        CLI_Parser("nim.freq 689000");
        CLI_Parser("fbm.i");
        CLI_Parser("vdec.i");
        CLI_Parser("d.i");
        CLI_Parser("d.sfe 0 0");
        CLI_Parser("aud.i");

        // set the framer 0 pid for tuner 0
        u1Inst0 = DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_TUNER_0);
        DMX_MUL_SetInstType(u1Inst0, DMX_IN_BROADCAST_TS);
        DMX_MUL_SetTSIdx(u1Inst0);
        DMX_MUL_SetDecoderCallbacks(u1Inst0, &rCallback);

        // Create picture queues
        if (x_msg_q_create(&_hAPESQueue, "FVR",
            sizeof (DMX_NOTIFY_INFO_PES_T), 1024) != OSR_OK)
        {
            LOG(1, "Fail to create picture queue!\n");
            return FALSE;
        }

        // Create picture thread
        if (x_thread_create(&_hAPESThread, "FvrThread",
            DMX_THREAD_STACK_SIZE, DMX_THREAD_PRIORITY, _APESThread, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX thread!\n");
            return FALSE;
        }
    }
    else if(u4Action == 1)  // set audio ES
    {
        u1Pidx0 = DMX_MUL_GetAvailablePidx(u1Inst0);

        x_memset(&rPid, 0, sizeof(rPid));
        rPid.u1TsIndex = 0;
        rPid.fgEnable = TRUE;
        rPid.u2Pid = (UINT16)u4Value;
        rPid.u1SteerMode = DMX_STEER_TO_FTUP;
        rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
        rPid.fgAllocateBuffer = FALSE;
        rPid.u4BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(0x100000,
                    64);
        rPid.u4BufSize = 0x100000;
        rPid.fgPrimary = TRUE;
        rPid.u1ChannelId = 0;

        if(!DMX_SetPid(u1Pidx0, DMX_PID_FLAG_ALL, &rPid))
        {
            Printf("fail to set pid\n");
        }
    }
    else if(u4Action == 2)  // set audio PES
    {
        u1Pidx1 = DMX_MUL_GetAvailablePidx(u1Inst0);

        x_memset(&rPid, 0, sizeof(rPid));
        rPid.u1TsIndex = 0;
        rPid.fgEnable = TRUE;
        rPid.u2Pid = (UINT16)u4Value;
        rPid.u1SteerMode = DMX_STEER_TO_FTUP;
        rPid.ePidType = DMX_PID_TYPE_PES_AUDIO;
        rPid.fgAllocateBuffer = FALSE;
        rPid.u4BufAddr = (UINT32)BSP_AllocAlignedDmaMemory(0x100000,
                    64);
        rPid.u4BufSize = 0x100000;
        rPid.fgPrimary = FALSE;
        rPid.u1ChannelId = 0;
        rPid.pfnNotify = _DmxCallback;

        if(!DMX_SetPid(u1Pidx1, DMX_PID_FLAG_ALL, &rPid))
        {
            Printf("fail to set pid\n");
        }
    }
    else if(u4Action == 3)  // free audio ES
    {
        x_memset(&rPid, 0, sizeof(rPid));
        rPid.fgEnable = FALSE;

        if(!DMX_SetPid(u1Pidx0, DMX_PID_FLAG_VALID, &rPid))
        {
            Printf("fail to set pid\n");
        }

        if(!DMX_FreePid(u1Pidx0))
        {
            Printf("fail to free pid\n");
        }

        if(!DMX_MUL_FreePidx(u1Pidx0))
        {
            Printf("fail to free mul pid\n");
        }
    }
    else if(u4Action == 4)  // free audio PES
    {
        x_memset(&rPid, 0, sizeof(rPid));
        rPid.fgEnable = FALSE;

        if(!DMX_SetPid(u1Pidx1, DMX_PID_FLAG_VALID, &rPid))
        {
            Printf("fail to set pid\n");
        }

        if(!DMX_FreePid(u1Pidx1))
        {
            Printf("fail to free pid\n");
        }

        if(!DMX_MUL_FreePidx(u1Pidx1))
        {
            Printf("fail to free mul pid\n");
        }
    }
}
#endif

#ifdef TEST_MODE
// this function verify ddi update wp method
static INT32 _CmdDdiUpdateWp(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgRet;

    fgRet = DMX_EMU_DDI_UpdateWp();

    Printf("%s\n", fgRet ? "PASS" : "FAIL");

    return (fgRet ? 0 : -1);
}
#endif

static INT32 _DMXCLI_TsOutExt(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_TSOUT_ROUTE_T eRoute;
    DMX_TSOUT_ENDIAN_T eEndian;
    DMX_TSOUT_SYNC_CYCLE_T eSyncCycle;
    DMX_TSOUT_LATCH_EDGE_T eLatchEdge;
    DMX_TSOUT_DATAWIDTH_T eDataWidth;
    DMX_TSOUT_DATAWIDTH_T eTsInDataWidth;
    UINT32 u4Size, u4ValidSpace;
    UINT8 u1Tsindex = 2;
    BOOL fgInvert = FALSE;
    BOOL fgGating = FALSE;
    UINT8 u1TsOutindex = 0;
    UINT8 u1Clock;
    UINT8 u1TsOutSel; 
    
    if ((i4Argc < 3) || (i4Argc > 6))
    {
        Printf("%s Tsindex TsOutindex u1Clock u1TsOutSel u4Size\n", aszArgv[0]);
        Printf("DMX_PVR_OUTPUT_CLOCK_27M = 0\n");
        Printf("DMX_PVR_OUTPUT_CLOCK_108M = 1\n");
        Printf("DMX_PVR_OUTPUT_CLOCK_72M = 2\n");
        Printf("DMX_PVR_OUTPUT_CLOCK_54M = 3\n");
        Printf("DMX_PVR_OUTPUT_CLOCK_48M = 4\n");
        Printf("DMX_PVR_OUTPUT_CLOCK_96M = 5\n");
        Printf("DMX_PVR_OUTPUT_CLOCK_MAX = 6\n");
        return -1;
    }
    // Get size
    if (i4Argc >= 2)
    {
        u1Tsindex = (UINT8)StrToInt(aszArgv[1]);
        Printf("Tsindex: %d, ",u1Tsindex);
    }

    if (i4Argc >= 3)
    {
        u1TsOutindex = (UINT8)StrToInt(aszArgv[2]);
        Printf("TsOutindex: %d, ",u1TsOutindex);
    }

    if (i4Argc >= 4)
    {
        u1Clock = (UINT32)StrToInt(aszArgv[3]);
        if(u1Clock > 6)
        {
           Printf("DMX_PVR_OUTPUT_CLOCK_27M = 0\n");
           Printf("DMX_PVR_OUTPUT_CLOCK_108M = 1\n");
           Printf("DMX_PVR_OUTPUT_CLOCK_72M = 2\n");
           Printf("DMX_PVR_OUTPUT_CLOCK_54M = 3\n");
           Printf("DMX_PVR_OUTPUT_CLOCK_48M = 4\n");
           Printf("DMX_PVR_OUTPUT_CLOCK_96M = 5\n");
           Printf("DMX_PVR_OUTPUT_CLOCK_MAX = 6\n");
           return -1;
        }
        Printf("u1Clock: 0x%x, ",u1Clock);
    }
    else
    {
        u1Clock = 1;
    }

    if (i4Argc >= 5)
    {
        u1TsOutSel = (UINT32)StrToInt(aszArgv[4]);
        if(u1TsOutSel > 4)
        {
            Printf("u1TsOutSel should be [0-4]\n");
            return -1;
        }
        Printf("u1TsOutSel: 0x%x, ",u1TsOutSel);
    }
    else
    {
        u1TsOutSel = 1;
    }
    
    if (i4Argc >= 6)
    {
        u4Size = (UINT32)StrToInt(aszArgv[5]);
        Printf("Ts out Size: 0x%x, ",u4Size);
    }
    else
    {
        u4Size = 0x200000;
    }

    eRoute = DMX_TSOUT_ROUTE_EXT_LOOPBACK;
    fgInvert = FALSE;
    fgGating = FALSE;
    eDataWidth = DMX_TSOUT_DATAWIDTH_1;
    eTsInDataWidth = DMX_TSOUT_DATAWIDTH_1;
    eEndian = DMX_TSOUT_ENDIAN_MSB;
    eSyncCycle = DMX_TSOUT_SYNC_CYCLE_8;
    u4ValidSpace = 0;
    eLatchEdge = DMX_TSOUT_LATCH_EDGE_NEGATIVE;

    ///-----------------------------------
    //  0x304 --> REG_TSOUT_CKCFG
    //   0x158 --> CI_TSIF_CFG
    //   0X160 --> EXTTS_CKCKG

    if (!_DMX_SetBypassMode(u1Tsindex, 188, FALSE, TRUE))
    {
        Printf("Failed to set bypass mode\n");
        return FALSE;
    }

    //Set ts output enalbe
    _DMX_PVRPlay_SetPort(u1TsOutindex,DMX_PVRPLAY_PORT_TSOUT);

    //Set output ts clock 108M(default)
    _DMX_TsOut_SetClock(u1TsOutindex, u1Clock);
    if(u1Clock == 0)
    {
        Printf("DMX_PVR_OUTPUT_CLOCK_27M = 0\n");           
    }
    else if(u1Clock == 1)
    {
        Printf("DMX_PVR_OUTPUT_CLOCK_108M = 1\n");
    }
    else if(u1Clock == 2)
    {
        Printf("DMX_PVR_OUTPUT_CLOCK_72M = 2\n");
    }
    else if(u1Clock == 3)
    {
        Printf("DMX_PVR_OUTPUT_CLOCK_54M = 3\n");
    }
    else if(u1Clock == 4)
    {
        Printf("DMX_PVR_OUTPUT_CLOCK_48M = 4\n");
    }
    else if(u1Clock == 5)
    {
        Printf("DMX_PVR_OUTPUT_CLOCK_96M = 5\n");
    }
    else if(u1Clock == 6)
    {
        Printf("DMX_PVR_OUTPUT_CLOCK_MAX = 6\n");
    }

    _DMX_TsOut_Config(u1TsOutSel, u1TsOutindex);

    if (!DMX_TsOut_SetDataWidthEx(u1TsOutindex, eDataWidth))
    {
        Printf("Failed to set data width!\n");
        return -1;
    }
    if (!DMX_TsIn_SetDataWidth(u1Tsindex, eTsInDataWidth))
    {
        Printf("Failed to set data width!\n");
        return -1;
    }
    if (!DMX_TsOut_SetEndianessEx(u1TsOutindex, eEndian))
    {
        Printf("Failed to set endian mode!\n");
        return -1;
    }
    if (!DMX_TsOut_SetSyncCycleEx(u1TsOutindex, eSyncCycle))
    {
        Printf("Failed to set sync cycle!\n");
        return -1;
    }
    if (!DMX_TsOut_SetValidCycleEx(u1TsOutindex, u4ValidSpace))
    {
        Printf("Failed to set valid space!\n");
        return -1;
    }
    if (!DMX_TsOut_SetLatchEdgeEx(u1TsOutindex, eLatchEdge))
    {
        Printf("Failed to set Latch edge!\n");
        return -1;
    }

    if (!_DMX_TsOut_SetClockInvertGating(u1TsOutindex, fgInvert, fgGating))
    {
        Printf("Failed to set Latch edge!\n");
        return -1;
    }

    if (!_DMX_TsOut_TransmitData(u1Tsindex, u1TsOutindex, u4Size))
    {
        Printf("Failed\n");
    }
    else
    {
        Printf("OK\n");
    }

    return 0;
}


static CLI_EXEC_T _arDmxEmuCmdTbl[] =
{
    {
        "dditransfer0", "ddit0", _CmdDdiTransfer0, NULL, "", CLI_SUPERVISOR
    },

    {
        "dditransfer1", "ddit1", _CmdDdiTransfer1, NULL, "", CLI_SUPERVISOR
    },

    {
        "dditransfer2", "ddit2", _CmdDdiTransfer2, NULL, "", CLI_SUPERVISOR
    },

    {
        "pullts0", "pt0", _CmdPullTsTest0, NULL, "", CLI_SUPERVISOR
    },
    {
        "ddi_pid_filter","ddip",_CmdDdi_To_PidFilter_Transfer0,NULL, "", CLI_SUPERVISOR
	},

    {
        "pullts1", "pt1", _CmdPullTsTest1, NULL, "", CLI_SUPERVISOR
    },

    {
        "pullts2", "pt2", _CmdPullTsTest2, NULL, "", CLI_SUPERVISOR
    },

    {
        "pullts3", "pt3", _CmdPullTsTest3, NULL, "DDI-TS-CAOUTPUT", CLI_SUPERVISOR
    },
    {
        "pulles0", "pe0", _CmdPullEsTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "pulles1", "pe1", _CmdPullEsTest1, NULL, "", CLI_SUPERVISOR
    },

    {
        "multi2cbc", "m2cbc", _CmdMulti2CBC, NULL, "", CLI_SUPERVISOR
    },

    {
        "des0", "d0", _CmdDesTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "aes0", "a0", _CmdAesTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "sms4", "sms4", _CmdSMS4Test, NULL, "", CLI_SUPERVISOR
    },
#ifdef TEST_MODE
    {
        "vuduaes0", "va0", _CmdVuduAesTest0, NULL, "", CLI_SUPERVISOR
    },

    {
        "hdcp2test", "h2", _CmdHDCP2Test, NULL, "", CLI_SUPERVISOR
    },
#endif
    /*
        {
            "drm0", "dr0", _CmdDrmTest0, NULL, "", CLI_SUPERVISOR
        },

        {
            "drmperf", "drmp", _CmdDrmPerfTest, NULL, "", CLI_SUPERVISOR
        },
    */
#ifdef TEST_MODE
#ifdef DMX_SUPPORT_NDDRM
    {
        "drmndselftest", "dnst", _CmdDrmNdSelfTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "drmndmultipasstest", "dnpt", _CmdDrmNdMultipassTest, NULL, "", CLI_SUPERVISOR
    },
#endif
#endif
#ifdef TEST_MODE
    {
        "drmndselftest", "dnst", _CmdDrmNdSelfTest, NULL, "", CLI_SUPERVISOR
    },
#endif
    {
        "patternmatch", "pm", _CmdPatternMatch, NULL, "", CLI_SUPERVISOR
    },

    {
        "psitest", "psitest", _CmdPSITest, NULL, "", CLI_SUPERVISOR
    },

    {
        "mulmatch", "mmatch", _CmdMultipleMatchTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "psicombotest", "psictest", _CmdPSIComboTest, NULL, "", CLI_SUPERVISOR
    },
    /*
        {
            "starttsoutput", "stso", _CmdStartTSOutput, NULL, "", CLI_SUPERVISOR
        },
    */
    {
        "tsout", "tsout", _DMXCLI_TsOutput, NULL, "Diagnose CI with TS-output", CLI_SUPERVISOR
    },
    {
        "tsoutext", "tsoutext", _DMXCLI_TsOutExt, NULL, "Diagnose CI with TS-output extloop", CLI_SUPERVISOR
    },  
    {
        "dvbtest", "dvb", _CmdDvbTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrpestest", "ppt", _CmdFVRPesTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrpsitest", "pvrpsi", _CmdPSILoopbackTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrtimestamp", "pvrts", _CmdFVRTimestampTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrtspause", "pvrtsp", _CmdFVRTimestampPauseTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrtserr", "pvrtserr", _CmdFVRTimestampErrTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrtsreset", "pvrtsr", _CmdFVRTimestampResetTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "pvrpsicatest", "pvrpsica", _CmdPSICALoopbackTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "bypass", "bypass", _CmdBypassTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "prebyte", "prebyte", _CmdPreByteTest    , NULL, "", CLI_SUPERVISOR
    },

    {
        "dmafifotest", "dftest", _CmdDMAFIFOTest    , NULL, "", CLI_SUPERVISOR
    },
   {
        "pvr_pb_dbm_bypass", "pftest", _CmdPVR_PB_DBM_TEST, NULL, "", CLI_SUPERVISOR
    },
    {
        "pvrtimer", "ptimer", _CmdPVRTimer    , NULL, "", CLI_SUPERVISOR
    },
    {
        "STCCounter", "stccount", _DMX_EMU_STCCount    , NULL, "STCCounter", CLI_SUPERVISOR
    },
    {
        "DBM_Auto_Switch", "autoswitch", _CmdDBM_Auto_SwitchTest   , NULL, "DBM_Auto_Switch", CLI_SUPERVISOR
    },
    {
        "130byte_test", "130test", _Cmd130byte_TsTest   , NULL, "130byte_test", CLI_SUPERVISOR
    },
    {
        "130byte_tsouttest", "130pvrtest", _Cmd130byte_TsoutTest   , NULL, "130byte_tsouttest", CLI_SUPERVISOR
    },
    {
        "130_188 Multisize", "multisize", _CmdMulti_PacketSizeTest   , NULL, "130_188 Multisize Test", CLI_SUPERVISOR
    },
    {
        "Dmx_SetDecoder", "stdecoder", _CmdSetToDecoder   , NULL, "Dmx_SetDecoder", CLI_SUPERVISOR
    },
    {
        "DDI_IOMMU_Test", "ddiiommu", _CmdDDI_IOMMU_Test   , NULL, "DMX_IOMMU_Test", CLI_SUPERVISOR
    },
    {
        "DMX_IOMMU_Test", "dmxiommu", _CmdDMX_IOMMU_Test   , NULL, "DMX_IOMMU_Test", CLI_SUPERVISOR
    },
    {
        "DMXDDI_IOMMU_Test", "ddiommu", _CmdDMXDDI_IOMMU_Test   , NULL, "DMXDDI_IOMMU_Test", CLI_SUPERVISOR
    },
    {
        "IOMMU_Performance_Test", "iommu", _CmdIOMMU_Performance_Test   , NULL, "IOMMU_Performance_Test", CLI_SUPERVISOR
    },
    {
        "FramerUnlockTest", "unlocktest", _CmdFrmaerUnlockTest   , NULL, "FramerUnlockTest", CLI_SUPERVISOR
    },
    {
        "SecureBufferTest", "securetest", _CmdSecureBufferTest   , NULL, "SecureBufferTest", CLI_SUPERVISOR
    },
    {
        "VideoEstest", "estest", _CmdDMXVideoESTest   , NULL, "VideoEstest", CLI_SUPERVISOR
    },
    {
        "TsOut_ToCaOutPutBuffer", "tsout_to_caout", _CmdTsoutToCAoutputBufferTest  , NULL, "TsOut_ToCaOutPutBuffer", CLI_SUPERVISOR
    },
    {
        "TsOut_ToftupBuffer", "tsout_to_ftup", _CmdTsoutToFTUPTest  , NULL, "TsOut_ToftupBuffer", CLI_SUPERVISOR
    },
    {
        "DDi_transfer", "dditest", _CmdDDI_Transfer_Test  , NULL, "DDi_transfer", CLI_SUPERVISOR
    },
    {
      "VCT test","vct", _CmdVCT_Test, NULL, "VCT test", CLI_SUPERVISOR
    }, 

#ifdef TEST_MODE
    {
        "ndoffset", "ndo", _DMXCLI_Ndo, NULL, "", CLI_SUPERVISOR
    },

    {
        "apes", "apes", _DMXCLI_AudioPes, NULL, "", CLI_SUPERVISOR
    },

    {
        "test4framer", "t4", _DMXCLI_Test4Framer, NULL, "", CLI_SUPERVISOR
    },

    {
        "ddiupdatewp", "ddiuw", _CmdDdiUpdateWp, NULL, "", CLI_SUPERVISOR
    },
#endif
    {
      "ci+1.4test","ci",NULL,_arCiEmuCmdTbl,"CI Plus1.4 Command Directory",CLI_SUPERVISOR
    },
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};


LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(DmxEmu)
{
    "dmxemu", "demu", NULL, _arDmxEmuCmdTbl, "Demux emulation commands", CLI_GUEST

};
LINT_RESTORE


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _DMX_EMU_SetTsIndex(UINT8 u1TsIndex)
{
    UINT32 u4Reg = 0, u4CkGenReg = 0x260;

    _u1TsIndex = u1TsIndex;

    u4CkGenReg = 0x24c;

    switch (u1TsIndex)
    {
    case 0:
        u4Reg = 0x00020002;
        break;

    case 1:
        u4Reg = 0x00000000;
        break;

    case 2:
        LOG(3, "TS2 is not for normal input!\n");
        return;

    case 3:
        u4Reg = 0x00020002;
        break;

    default:
        LOG(3, "Unknown TS index number!\n");
        return;
    }

    CKGEN_WRITE32(u4CkGenReg, u4Reg);   // Select DMX clock as TS0 clock source
}


#if 0
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetDmxEmuMwCmdTbl(void)
#else
CLI_EXEC_T* GetDmxEmuCmdTbl(void)
#endif
{
    return _arDmxEmuCmd;
}

#endif
