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

#include "vdo_fpga.h"
#include "nptv_debug.h"
#include "drv_video.h"
#include "drv_regen.h"

#ifdef CC_FPGA_SUPPORT

FPGA_TIMING_INFO_T fpgaTiming[VDP_MAX_NS];

FPGA_TIMING_INFO_T fpgaTimingList[] = 
{
    { 858, 525, 720, 480,60,1,SV_VDO_STABLE}, //480i
    { 858, 525, 720, 480,60,0,SV_VDO_STABLE}, //480p    
    { 858, 525, 720, 576,60,1,SV_VDO_STABLE}, //576i
    { 858, 525, 720, 576,60,0,SV_VDO_STABLE}, //576p    
    {1650, 750,1280, 720,60,0,SV_VDO_STABLE}, //720p
    {2200,1125,1920,1080,60,1,SV_VDO_STABLE}, //1080i
    {2200,1125,1920,1080,60,0,SV_VDO_STABLE}, //1080p    
};

HANDLE_T  hFpgaTimerThread;
UINT16 u2TimerDelay;
static VOID _OnFpgaTimer(HANDLE_T  pt_tm_handle, VOID *pv_tag);

void vVdoFpgaMloopInit(void)
{
    static UINT8 bFpgaMloopInited = SV_FALSE;

    if(bFpgaMloopInited == SV_FALSE)
    {
        bFpgaMloopInited = SV_TRUE;
        VERIFY(x_timer_create(&hFpgaTimerThread) == OSR_OK);
        u2TimerDelay = 0;
        vDrvVideoSwInit();
        getChannel(SV_VP_MAIN)->bIsChannelOn = SV_ON;
        getChannel(SV_VP_PIP)->bIsChannelOn = SV_ON;        
        getChannel(SV_VP_MAIN)->bDecType = 1;
        getChannel(SV_VP_PIP)->bDecType = 1;
        bFpgaMloopInited = TRUE;
    }
    else
    {
        Printf("Fpga mloop already inited!\n");
    }
}

void vVdoFpgaStartTimer(UINT16 timerDelay)
{
    u2TimerDelay = timerDelay;    
    VERIFY(x_timer_stop(hFpgaTimerThread) == OSR_OK);
    VERIFY(x_timer_start(hFpgaTimerThread, u2TimerDelay, X_TIMER_FLAG_REPEAT, _OnFpgaTimer, NULL) == OSR_OK);    
}

void vVdoFpgaStopTimer(void)
{
    VERIFY(x_timer_stop(hFpgaTimerThread) == OSR_OK);
}

UINT16 u2VdoFpgaGetTimerDelay(void)
{
    return u2TimerDelay;
}

void vVdoFpgaSetInputTimingByIdx(UINT8 bPath, UINT8 idx)
{
    UINT8 listSize;

    listSize = sizeof(fpgaTimingList) / sizeof(FPGA_TIMING_INFO_T);

    if(idx >= listSize)
    {
        return;
    }

    vVdoFpgaSetInputTiming(bPath, 
                           fpgaTimingList[idx].u2HTotal,
                           fpgaTimingList[idx].u2VTotal,
                           fpgaTimingList[idx].u2Width,
                           fpgaTimingList[idx].u2Height,
                           fpgaTimingList[idx].u2FrameRate,
                           fpgaTimingList[idx].u2ScanMode);
}

void vVdoFpgaSetInputTiming(UINT8 bPath,UINT16 u2HTotal,UINT16 u2VTotal,UINT16 u2HActive,UINT16 u2VActive,UINT16 u2FrameRate,UINT16 u2IsInterlace)
{
    fpgaTiming[bPath].u2HTotal = u2HTotal;
    fpgaTiming[bPath].u2VTotal = u2VTotal;    
    fpgaTiming[bPath].u2Width = u2HActive;    
    fpgaTiming[bPath].u2Height = u2VActive;
    fpgaTiming[bPath].u2FrameRate = u2FrameRate;
    fpgaTiming[bPath].u2ScanMode = u2IsInterlace;
    fpgaTiming[bPath].u2SignalStatus = SV_VDO_STABLE;
}

static VOID _OnFpgaTimer(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    static UINT32 u4Cnt = 0;

    u4Cnt++;
    LOG(3,"_OnFpgaTimer:: u4Cnt(%d)\n",u4Cnt);

    //VRM main loop
    if(fgIsBypassFlgClr(BYPASS_NPTV_VRM))
    {
        vVRMLoop(SV_VP_MAIN);
        vVRMLoop(SV_VP_PIP);
    }
#if 1 
    if(fgIsBypassFlgClr(BYPASS_NPTV_PDS))
    {
        vPDSLoop(SV_VP_MAIN);
        vPDSLoop(SV_VP_PIP);
    }
    
    if(fgIsBypassFlgClr(BYPASS_NPTV_REGEN))
    {
        vRegenLoop(SV_VP_MAIN,REGEN_MODULE_PRE_REGEN);
        vRegenLoop(SV_VP_PIP,REGEN_MODULE_PRE_REGEN);        
    }

    if(fgIsBypassFlgClr(BYPASS_NPTV_PSCAN))
    {
        vDrvMIBProc();
    }
#endif
    if(fgIsBypassFlgClr(BYPASS_NPTV_SCPOS))
    {
        vRegenLoop(SV_VP_MAIN,REGEN_MODULE_FSC_REGEN);
        vRegenLoop(SV_VP_PIP,REGEN_MODULE_FSC_REGEN);        
        vScpipNormProc();
    }
}

UINT16 wDrvVideoInputWidth(UINT8 bPath)
{
    return fpgaTiming[bPath].u2Width;
}

UINT16 wDrvVideoInputHeight(UINT8 bPath)
{
    return fpgaTiming[bPath].u2Height;
}

UINT8 bDrvVideoGetRefreshRate(UINT8 bPath)
{
    return fpgaTiming[bPath].u2FrameRate;
}

UINT8 bDrvVideoIsSrcInterlace(UINT8 bPath)
{
    return fpgaTiming[bPath].u2ScanMode;
}

UINT8 bDrvVideoSignalStatus(UINT8 bPath)
{
    return fpgaTiming[bPath].u2SignalStatus;
}

UINT16 wDrvVideoGetHTotal(UINT8 bPath)
{
    return fpgaTiming[bPath].u2HTotal;
}

UINT16 wDrvVideoGetVTotal(UINT8 bPath)
{
    return fpgaTiming[bPath].u2VTotal;
}

#endif

