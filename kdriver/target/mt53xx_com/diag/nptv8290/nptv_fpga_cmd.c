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
 * $RCSfile: nptv_scpos_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "x_debug.h"
#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "vdo_fpga.h"
 
#if defined(CC_FPGA_SUPPORT)
CLIMOD_DEBUG_FUNCTIONS(SCPOS)

static INT32 _FpgaMLoopInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _FpgaMLoopStart(INT32 i4Argc, const CHAR **szArgv);
static INT32 _FpgaMLoopStop(INT32 i4Argc, const CHAR **szArgv);    
static INT32 _FpgaSetInputTiming(INT32 i4Argc, const CHAR **szArgv);

// Table for scpos command entry
CLI_EXEC_T arFpgaCmdTbl[] = 
{
    {"init loop",  "i",  _FpgaMLoopInit,  NULL,  "Init FPGA mainloop.",  CLI_SUPERVISOR},      
    {"start loop",  "start",  _FpgaMLoopStart,  NULL,  "Init FPGA mainloop.",  CLI_SUPERVISOR},      
    {"stop loop",  "stop",  _FpgaMLoopStop,  NULL,  "Init FPGA mainloop.",  CLI_SUPERVISOR},
    {"set input timing", "st", _FpgaSetInputTiming, NULL, "Set FPGA input timing", CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(SCPOS),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static INT32 _FpgaMLoopInit(INT32 i4Argc, const CHAR **szArgv)
{
    vVdoFpgaMloopInit();
    CLI_Parser("n.mute.all 0 0");
    return 0;
}

static INT32 _FpgaMLoopStart(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2Delay;
    if (i4Argc < 2)
    {
        Printf("Usage: timerDelay\n");
        Printf("Current delay : %d\n", u2VdoFpgaGetTimerDelay());
        return 0;
    }       

    u2Delay = (UINT16)StrToInt(szArgv[1]);

    vVdoFpgaStartTimer(u2Delay);

    return 0;
}

static INT32 _FpgaMLoopStop(INT32 i4Argc, const CHAR **szArgv)    
{
    vVdoFpgaStopTimer();
    return 0;
}

static INT32 _FpgaSetInputTiming(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8  bPath;
    UINT8  bTimingId;
    UINT16 u2HTotal;
    UINT16 u2VTotal;
    UINT16 u2HActive;
    UINT16 u2VActive;
    UINT16 u2FrameRate;
    UINT16 u2IsInterlace;

    if(i4Argc != 8 && i4Argc!=3)
    {
        Printf("Usage[1]: Path HTotal VTotal HActive VActive Framerate IsInterlace\n");
        Printf("Usage[2]: Path timingId\n");
        Printf("timingId:\n");
        Printf("0:480i\n");
        Printf("1:480p\n");
        Printf("2:576i\n");
        Printf("3:576p\n");
        Printf("4:720p\n");
        Printf("5:1080i\n");        
        Printf("6:1080p\n");
        return 0;
    }

    if(i4Argc==8)
    {
        bPath = (UINT16)StrToInt(szArgv[1]);
        u2HTotal = (UINT16)StrToInt(szArgv[2]);
        u2VTotal = (UINT16)StrToInt(szArgv[3]);
        u2HActive = (UINT16)StrToInt(szArgv[4]);
        u2VActive = (UINT16)StrToInt(szArgv[5]);
        u2FrameRate = (UINT16)StrToInt(szArgv[6]);
        u2IsInterlace = (UINT16)StrToInt(szArgv[7]);    

        vVdoFpgaSetInputTiming(bPath, u2HTotal, u2VTotal, u2HActive, u2VActive, u2FrameRate, u2IsInterlace);
    }
    else if(i4Argc==3)
    {
        bPath = (UINT16)StrToInt(szArgv[1]);
        bTimingId = (UINT16)StrToInt(szArgv[2]);
        vVdoFpgaSetInputTimingByIdx(bPath, bTimingId);
    }

    CLI_Parser("vdp.i");
    CLI_Parser("vdp.e 0");
    CLI_Parser("vdp.s.srcr 0 0 0 10000 10000");
    CLI_Parser("vdp.s.outr 0 0 0 10000 10000");
    CLI_Parser("vdp.s.dispmode 0 0");    

    return 0;
}

#endif

