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
 * $Date: 2015/02/22 $
 * $RCSfile: b2r_drvif.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file b2r_drvif.c
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "b2r_drvif.h"
#include "vdp_frc.h"
#include "vdp_debug.h"
#include "b2r_hal.h"
#include "drv_video.h"
#include "drv_di.h"
#include "drv_scaler.h"
#include "vdec_drvif.h"
#include "tve_if.h"
#include "mute_if.h"
#include "vdp_drvif.h"
#include "../tve/tve_hal.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VDP_VSYNC_CB_THD                (20)                        // 20 VSync ~= 330 ms

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static B2R_OBJECT_T _arB2rObj[B2R_NS];
static VDP_CFG_T*  _prVdpCfg[VDP_NS];
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static B2R_VAR_T* _B2R_DrvGetB2rVar(UCHAR ucB2rId)
{
    return _B2R_GetB2rVar(ucB2rId);
}

static void _B2R_VsyncNotify(UCHAR ucB2rId, UINT32 u4Type, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3)
{
    B2R_OBJECT_T *this;
    VDP_CB_MSG_T rMsg;
    this = _B2R_GetObj(ucB2rId);
    rMsg.u4Type = u4Type;
    rMsg.u4Arg1 = u4Arg1;
    rMsg.u4Arg2 = u4Arg2;
    rMsg.u4Arg3 = u4Arg3;

    if (u4Arg1 >= VDP_NS || !this)
    {
        return;
    }

    if (_prVdpCfg[u4Arg1]->ucEnable == 0)
    {
        if ((u4Type == VDP_MSG_UNMUTE_CB) ||
                (u4Type == VDP_MSG_CC_CB) ||
                (u4Type == VDP_MSG_AFD_CB) ||
                (u4Type == VDP_MSG_LIPSYNC_CB) ||
                (u4Type == VDP_MSG_REPEAT_DROP_CB))
        {
            // skip redundant notify if video plane not enable
            return;
        }
    }
	if(u4Type == VDP_MSG_UNMUTE_CB)return;
    if (x_msg_q_send(this->hNotifyMsgQ, (void *)(&rMsg), sizeof(VDP_CB_MSG_T), 0) != OSR_OK)
    {
        LOG(5, "DTV NOTIFY-Q FULL\n");
    }
}
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
VOID _B2R_ObjectInit(VOID)
{
    UINT32 i;

    for (i = 0; i < B2R_NS; i++)
    {
        _arB2rObj[i].ucB2rId = i;
        _arB2rObj[i].ptB2rVar = _B2R_GetB2rVar(i);
        _arB2rObj[i].ptB2rPrm = _B2R_GetB2rPrm(i);
    }
    for (i = 0; i < VDP_NS; i++)
    {
        _prVdpCfg[i] = _B2R_GetVdpConf(i);
    }
}


B2R_OBJECT_T * _B2R_GetObj(UCHAR ucB2rId)
{
    UINT32 i;
    B2R_OBJECT_T *ptRet = NULL;

    for (i = 0; i < B2R_NS; i++)
    {
        if (_arB2rObj[i].ucB2rId == ucB2rId)
        {
            ptRet = &_arB2rObj[i];
            break;
        }
    }

    return ptRet;
}

VOID _B2R_SetTimeShiftMode(UCHAR ucB2rId,BOOL fgEnable)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (prB2rVar)
    {
        prB2rVar->fgTimeShift = fgEnable;
    }
}

VOID _B2R_SetPauseMM(UCHAR ucB2rId,BOOL fgPauseMM)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (prB2rVar)
    {
        prB2rVar->fgPauseMM = fgPauseMM;
    }
}

STC_SPEED_TYPE_T _B2R_GetFrcStcSpeed(UCHAR ucB2rId)
{
	B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    return prB2rVar ? prB2rVar->eSpeed : STC_SPEED_TYPE_FORWARD_1X;
}

UCHAR _B2R_GetWaitSTC(UCHAR ucB2rId)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (prB2rVar)
    {
        if (prB2rVar->fgEnableWaitSTC)
        {
            return 1;
        }
    }
    return 0;
}

VOID _B2R_SetABRepeat(UCHAR ucB2rId, UCHAR ucSet)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (ucB2rId < B2R_NS)
    {
        if ((ucSet) && (prB2rVar->rAB.fgValid))
        {
            prB2rVar->rVdpSeek.ucABNotifyTrigger = 1;
            prB2rVar->rVdpSeek.fgABReach = FALSE;
            LOG(3, "ucABNotifyTrigger ENABLE\n");
        }
        else
        {
            prB2rVar->rVdpSeek.ucABNotifyTrigger = 0;
            prB2rVar->rVdpSeek.fgABReach = FALSE;
        }
    }
    else
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
}

BOOL _B2R_TriggerAudReceiveEx(UCHAR ucB2rId, BOOL fgEnable, UINT32 u4Pts)
{
   B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (prB2rVar)
    {
        prB2rVar->rVdpTrigAud.ucTriggerReceived = fgEnable?1:0;
        prB2rVar->rVdpTrigAud.u4TriggerPts = u4Pts;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

VOID _B2R_SetEnableLog(UCHAR ucB2rId,BOOL fgEnableGstLog, BOOL fgEnableVideoLog)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    UNUSED(ucB2rId);
    UNUSED(fgEnableGstLog);
    UNUSED(fgEnableVideoLog);
    if (prB2rVar)
    {
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
        prB2rVar->fgEnableGstLog = fgEnableGstLog;
#endif
        prB2rVar->fgEnableVideoLog = fgEnableVideoLog;
    }
}

BOOL _B2R_GetEnableGstLog(UCHAR ucB2rId)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    if (prB2rVar)
    {
        return prB2rVar->fgEnableGstLog;
    }
    else
    {
        return FALSE;
    }
#else
    return FALSE;
#endif
}

VOID _B2R_StatusTest (UCHAR ucB2rId)
{
    B2R_OBJECT_T *this;
    B2R_HAL_TP_T tPat;

    this = _B2R_GetObj(ucB2rId);

    if (!this)
    {
        return;
    }
    
    tPat.fgEnable  = TRUE;
    tPat.eMode     = B2R_TP_H_XOR_V;
    tPat.u4Color   = 0x000FFFFF;
    tPat.u4Reserve = 0x9FFF9FFF;

    B2R_HAL_Set(this->hB2r, B2R_HAL_PATTERN, &tPat);

}


//-----------------------------------------------------------------------------
/** Brief of _B2R_DisStatusTest.
 */
//-----------------------------------------------------------------------------
VOID _B2R_DisStatusTest (UCHAR ucB2rId)
{
    B2R_OBJECT_T *this;
    B2R_HAL_TP_T tPat;

    this = _B2R_GetObj(ucB2rId);

    if (!this)
    {
        return;
    }

    tPat.fgEnable  = FALSE;
    tPat.eMode     = B2R_TP_NO_GRID;
    tPat.u4Color   = 0;
    tPat.u4Reserve = 0;

    B2R_HAL_Set(this->hB2r, B2R_HAL_PATTERN, &tPat);

}

//-----------------------------------------------------------------------------
/** Brief of _VDP_HalDisablePattern.
 */
//-----------------------------------------------------------------------------
VOID _B2R_GetYCStart(UCHAR ucB2rId,UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive)
{
    B2R_OBJECT_T *this;
    B2R_HAL_DISP_INFO_T tDispInfo;
    this = _B2R_GetObj(ucB2rId);

    if (!this)
    {
        return;
    }

    B2R_HAL_Get(this->hB2r, B2R_HAL_DISP_INFO, &tDispInfo);

    *pu4YStart   = tDispInfo.u4YAddr;
    *pu4CStart   = tDispInfo.u4CAddr;
    *pu4LineSize = tDispInfo.u4Pitch;
    *pu4VActive  = tDispInfo.u4Height;
}

VOID _B2R_SetBlack(UCHAR ucB2rId, BOOL fgBlack)
{
    UNUSED(ucB2rId);
    UNUSED(fgBlack);
}

VOID _B2R_ForceBg(UCHAR ucB2rId, UCHAR ucForce)
{
    UINT8 bType1;
    UINT8 i, bDPNs;
    B2R_OBJECT_T *this;
  //  B2R_HAL_TP_T tPat;
    this = _B2R_GetObj(ucB2rId);

    if (!this)
    {
        return;
    }

    for (bDPNs = 0, i = 0; i < this->u4VideoPanelNs; i++)
    {
        if (this->avVideoPanel[i])
        {
            bDPNs++;
        }
    }

    bType1 = bGetVideoDecType(VDP_1);

    LOG(5, "ForceBg : B2R(%d) En(%d)\n", ucB2rId, ucForce);
/*
    if (bDPNs <= 1 || (bType1 == SV_VD_MPEGHD))
    {
        if (ucForce != 0)
        {
            tPat.fgEnable  = TRUE;
            tPat.eMode     = B2R_TP_NO_GRID;
            tPat.u4Color   = 0x00108080;
            tPat.u4Reserve = 0;
            B2R_HAL_Set(this->hB2r, B2R_HAL_PATTERN, &tPat);
        }
        else
        {
            tPat.fgEnable  = FALSE;
            tPat.eMode     = B2R_TP_NO_GRID;
            tPat.u4Color   = 0x00108080;
            tPat.u4Reserve = 0;
            B2R_HAL_Set(this->hB2r, B2R_HAL_PATTERN, &tPat);
        }
    }
    else
    {
        if (ucForce == 0)
        {
            tPat.fgEnable  = FALSE;
            tPat.eMode     = B2R_TP_NO_GRID;
            tPat.u4Color   = 0x00108080;
            tPat.u4Reserve = 0;
            B2R_HAL_Set(this->hB2r, B2R_HAL_PATTERN, &tPat);
        }
        else
        {
            LOG(5, "DTV Scart out, can't set B2R to BG mode\n");
        }
    }
*/
}


VOID _B2R_FrcSetLipSyncCb(UCHAR ucB2rId, UCHAR ucPort, UINT32 u4Thrsd, UINT32 u4FrmCnt)
{
    UNUSED(ucPort);
   
    if (ucB2rId < B2R_NS)
    {
        B2R_VAR_T *ptB2rVar;
        ptB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
        ptB2rVar->u4CbSyncEn = 1;
        ptB2rVar->u4CbSyncThrsd = (u4Thrsd * 90000);
        ptB2rVar->u4CbSyncChkFrmCnt = u4FrmCnt;
        LOG(9, "(Thrsd: %d, FrmCnt:%d\n", u4Thrsd, u4FrmCnt);
    }
    else
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
    }
}
UINT32 _B2R_SetABInfo(UINT8 u1B2rId, VDP_AB_INTO_T *prAB)
{
     B2R_VAR_T *ptB2rVar;
    if (!prAB)
    {
        return VDP_SET_ERROR;
    }
    if (u1B2rId >= B2R_NS)
    {
        return VDP_SET_ERROR;
    }
     ptB2rVar = _B2R_DrvGetB2rVar(u1B2rId);
    x_memcpy(&(ptB2rVar->rAB), prAB, sizeof(VDP_AB_INTO_T));
    _B2R_SetABRepeat(u1B2rId, prAB->fgValid);
    return VDP_SET_OK;
}
VOID _B2R_SetMirrorStatus(UCHAR ucB2rId, BOOL fgOn)
{
    B2R_VAR_T *ptB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (ptB2rVar)
    {
       ptB2rVar->ucMirrorStatus = (fgOn ? MIRROR_ON : MIRROR_OFF);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_EnB2rUpdateStc
 */
//-----------------------------------------------------------------------------
VOID _B2R_EnB2rUpdateStc(UCHAR ucB2rId, BOOL fgEnable)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if(prB2rVar)
    {
        prB2rVar->rB2rStc.fgEnable = fgEnable;
    }
}


//-----------------------------------------------------------------------------
/** Brief of _B2R_SetB2rStc
 */
//-----------------------------------------------------------------------------
VOID _B2R_SetB2rStc(UCHAR ucB2rId,UINT32 u4B2rStc)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if(prB2rVar)
    {
        prB2rVar->rB2rStc.u4B2rStcVal = u4B2rStc;
    }
}

UINT32 _B2R_GetB2rStc(UCHAR ucB2rId)
{
    B2R_VAR_T *ptB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    return ptB2rVar ? ptB2rVar->rB2rStc.u4B2rStcVal : 0;
}

BOOL _B2R_GetUpdateStc(UCHAR ucB2rId)
{
    B2R_VAR_T *ptB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    return ptB2rVar ? ptB2rVar->rB2rStc.fgEnable : FALSE;
}

VOID _B2R_StcPeriod(UCHAR ucB2rId, UINT32 u4VSyncPeriod)
{
    B2R_VAR_T *ptB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (ptB2rVar)
    {
        ptB2rVar->rB2rStc.u4VSyncPeriod = u4VSyncPeriod;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_SetStepForward
 */
//-----------------------------------------------------------------------------
VOID _B2R_SetStepForward(UCHAR ucB2rId, UCHAR ucSet)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);

    if(prB2rVar)
    {
        prB2rVar->rB2rData.ucStepForward = ucSet;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_SetSlowForward
 */
//-----------------------------------------------------------------------------
VOID _B2R_SetSlowForward(UCHAR ucB2rId, UCHAR ucSet)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if(prB2rVar)
    {
        prB2rVar->rB2rData.ucSlowForward = ucSet;
    }
}

UCHAR _B2R_GetSlowForward(UCHAR ucB2rId)
{
    B2R_VAR_T *ptB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    return ptB2rVar ? ptB2rVar->rB2rData.ucSlowForward : 0;
}

UCHAR _B2R_GetStepForward(UCHAR ucB2rId)
{
    B2R_VAR_T *ptB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    return ptB2rVar ? ptB2rVar->rB2rData.ucStepForward : 0;
}

/* whether Update STC when VDP start output or not */
BOOL _B2R_SetPendingForStc(UCHAR ucB2rId, BOOL fgPending)
{
	B2R_VAR_T* prB2rVar ;

    if (ucB2rId < B2R_NS)
    {
        prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
        prB2rVar->fgPendingForSTC = fgPending;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}
/* VDP Seek done notification */
VOID _B2R_SetSeek(UCHAR ucB2rId, UCHAR ucSet, UINT32 u4Pts)
{
    if (ucB2rId < B2R_NS)
    {
        B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
        prB2rVar->rVdpSeek.u4SeekPts = u4Pts;
        prB2rVar->rVdpSeek.ucNotifyTrigger = 1;
        prB2rVar->rVdpSeek.ucSeekMode = 1;
    }
    else
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
}
VOID _B2R_SetSeekMode(UCHAR ucB2rId, UCHAR ucSet, UINT32 u4Pts)
{
    if (ucB2rId < B2R_NS)
    {
        B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
        prB2rVar->rVdpSeek.u4SeekPts = u4Pts;
        prB2rVar->rVdpSeek.ucNotifyTrigger = 1;
        prB2rVar->rVdpSeek.ucSeekMode = 2;
    }
    else
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
}

/* VDP Set Wait STC while starts */
VOID _B2R_SetWaitSTC(UCHAR ucB2rId, UCHAR ucSet)
{
    if (ucB2rId < B2R_NS)
    {
        B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
        if (ucSet != 0)
        {
            prB2rVar->fgEnableWaitSTC = TRUE;
        }
        else
        {
            prB2rVar->fgEnableWaitSTC = FALSE;
        }
    }
    else
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
    }
}

VOID _B2R_StcSpeed(UCHAR ucB2rId,UINT32 u4Speed, BOOL fgDiv10)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (prB2rVar)
    {
        prB2rVar->rB2rStc.u4Speed = u4Speed;
        prB2rVar->rB2rStc.fgDiv10 = fgDiv10;
    }
}

VOID _B2R_SetTimeShiftSpeed(UCHAR ucB2rId, UINT32 u4SpeedValue)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);

    if (prB2rVar)
    {
        prB2rVar->i4TimeShiftSpeed = u4SpeedValue;
    }
}


BOOL _B2R_GetTimeShfitStartPlay(UCHAR ucB2rId)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);

    if ( prB2rVar &&
        prB2rVar->fgTimeShift && ucB2rId < B2R_NS)
    {
        return prB2rVar->fgTimeShfitThumbStartPlay;
    }
    else
    {
        return TRUE;
    }
}

VOID _B2R_SetRepeatFrame(UCHAR ucB2rId)
{
    UCHAR ucVdpId;
    VDP_CFG_T* prVdpConf;
    B2R_VAR_T* prB2rVar;
    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
    prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);    
    ucVdpId = _B2R_GetVdpId(ucB2rId);
    if (ucVdpId >= VDP_NS)
    {
        LOG(0, "ucVdpId = %d [%s-%d]\n", ucVdpId, __FUNCTION__, __LINE__);
        return;
    }
    prVdpConf = _B2R_GetVdpConf(ucVdpId);
    // Because of wrong frame repeat problem:
    // 1. Do not apply for timeshift.
    // 2. Do not enable during early display.
    if ((prB2rVar->fgTimeShift == FALSE) &&
            ((prVdpConf != NULL) && (prVdpConf->ucEarlyDisp != 1)))
    {
        prB2rVar->fgRepeatFrame = TRUE;
        LOG(9, "_B2R_SetRepeatFrame !! %d\n", prB2rVar->fgRepeatFrame);
    }
}

VOID _B2R_SetDropFrame(UCHAR ucB2rId)
{
    UCHAR ucVdpId;
    VDP_CFG_T* prVdpConf;
    B2R_VAR_T* prB2rVar;
    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
    prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);    
    ucVdpId  = _B2R_GetVdpId(ucB2rId);
    if (ucVdpId >= VDP_NS)
    {
        LOG(0, "ucVdpId = %d [%s-%d]\n", ucVdpId, __FUNCTION__, __LINE__);
        return;
    }
    prVdpConf = _B2R_GetVdpConf(ucVdpId);
    // Because of wrong frame repeat problem:
    // 1. Do not apply for timeshift.
    // 2. Do not enable during early display.
    if ((prB2rVar->fgTimeShift == FALSE) &&
            ((prVdpConf != NULL) && (prVdpConf->ucEarlyDisp != 1)))
    {
        prB2rVar->fgDropFrame = TRUE;
        LOG(9, "_B2R_SetDropFrame !! %d\n", prB2rVar->fgDropFrame);
    }
}

//move from vdp_hal.c
//-----------------------------------------------------------------------------
/** Brief of _B2R_SetTrickMode
 */
//-----------------------------------------------------------------------------
VOID _B2R_SetTrickMode(UCHAR ucB2rId, UCHAR ucTrick)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if(prB2rVar)
    {
        prB2rVar->rB2rData.ucTrickPlay = ucTrick;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_GetTrickMode
 */
//-----------------------------------------------------------------------------
UCHAR _B2R_GetTrickMode(UCHAR ucB2rId)
{
    B2R_VAR_T* prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);

    return prB2rVar ? prB2rVar->rB2rData.ucTrickPlay : 0; 
}



//-----------------------------------------------------------------------------
/** Brief of _B2R_FrcSetFreeze.
 */
//-----------------------------------------------------------------------------
VOID _B2R_FrcSetFreeze(UCHAR ucB2rId, UINT32 u4OnOff)
{
    B2R_VAR_T* prB2rVar;
    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
    prB2rVar = _B2R_DrvGetB2rVar(ucB2rId);
    if (u4OnOff != prB2rVar->rVdpDtvFreeze.u4OnOff)
    {
        UCHAR ucFbgId;
        B2R_PRM_T* prB2rPrm;
        
        prB2rPrm = _B2R_GetB2rPrm(ucB2rId);
        ucFbgId = prB2rPrm->ucFbgId;

        if (ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            if (u4OnOff != 0)
            {
                FBM_SetFrameBufferFlag(ucFbgId, FBM_FLAG_FREEZE);
    //                FBM_ReleaseDispQ(ucFbgId);
            }
            else
            {
                FBM_ClrFrameBufferFlag(ucFbgId, FBM_FLAG_FREEZE);
            }
        }

        if (u4OnOff == 0)
        {
            prB2rVar->rVdpDtvFreeze.u4Recovery = 1;
            prB2rVar->rVdpDtvFreeze.u4UnFreeze= 1;
        }
        else
        {
            prB2rVar->rVdpDtvFreeze.u4Recovery = 0;
            prB2rVar->rVdpDtvFreeze.u4UnFreeze= 0;
        }

        prB2rVar->rVdpDtvFreeze.u4CleanDispQ = 0;
        prB2rVar->rVdpDtvFreeze.u4OnOff = u4OnOff;
    }
}

UINT32 _B2R_PtsCbProc(B2R_OBJECT_T * this, UINT32 u4Status)
{
    B2R_PRM_T *prFrcPrm;
    B2R_VAR_T *prB2rVar;

    if (!this)
    {
        return B2R_INV_ARG;
    }

    prFrcPrm = this->ptB2rPrm;
    prB2rVar = this->ptB2rVar;

    if (!prB2rVar->u4CbPtsEn)
    {
        return B2R_OK;
    }

    // 0 comes from avsync proc
    if (u4Status == 0)
    {
        if ((prFrcPrm->u4Pts >= prB2rVar->u4CbPts) ||
                ((prB2rVar->u4CbPts - prFrcPrm->u4Pts) > 0x80000000))    // normal & warp case
        {
            _B2R_PostNotify(this, VDP_MSG_CC_CB, prB2rVar->u4CbPts, prB2rVar->u4CbArg);
            prB2rVar->u4CbPtsEn = 0;
        }
        else
        {
            // call back protection
            if (prFrcPrm->ucReady)
            {
                // invalid PTS detection
                if ((prB2rVar->u4CbLastPts != 0) &&
                        (prFrcPrm->u4Pts < prB2rVar->u4CbLastPts) &&
                        ((prB2rVar->u4CbLastPts - prFrcPrm->u4Pts) < 0x80000000))
                {
                    _B2R_PostNotify(this, VDP_MSG_CC_CB, prB2rVar->u4CbPts, (prB2rVar->u4CbArg | VDP_CB_ERR));
                    prB2rVar->u4CbPtsEn = 0;
    
                    LOG(5, "P2(%x)\n", prB2rVar->u4CbPts);
                }
                else
                {
                    prB2rVar->u4CbLastPts = prFrcPrm->u4Pts;
                }
            }
        }

    }
    else if (u4Status == 1)
    {
        if (prFrcPrm->ucReady)
        {
            if (++(prB2rVar->u4CbCntDown) >= VDP_VSYNC_CB_THD)
            {
                //UCHAR ucVdpId = VDP_GetVdpId(ucB2rId);
                //_VDP_VsyncNotify(VDP_MSG_CC_CB, (UINT32)ucVdpId, prB2rVar->u4CbPts, (prB2rVar->u4CbArg | VDP_CB_ERR));
                _B2R_PostNotify(this, VDP_MSG_CC_CB, prB2rVar->u4CbPts, (prB2rVar->u4CbArg | VDP_CB_ERR));
        
                prB2rVar->u4CbPtsEn = 0;
        
                LOG(5, "P1(%x)\n", prB2rVar->u4CbPts);
            }
        }
    }

    return B2R_OK;
}

void _B2R_PostNotify(B2R_OBJECT_T *this, UINT32 u4Type, UINT32 u4Arg2, UINT32 u4Arg3)
{
    UCHAR ucVdpId;
    if (!this ||(this->ucB2rId >= B2R_NS))
    {
        LOG(2, "Warning: %s-%d fail: ucB2rId(%d)!!!\n", __func__, __LINE__, this ? this->ucB2rId : 0xb2);
        return;
    }
    ucVdpId = _B2R_GetVdpId(this->ucB2rId);
    if (ucVdpId >= VDP_NS)
    {
        LOG(2, "Warning: %s-%d fail: ucVdpId(%d)!!!\n", __func__, __LINE__, ucVdpId);
        return;
    }   
   
    _B2R_VsyncNotify(this->ucB2rId, u4Type, ucVdpId, u4Arg2, u4Arg3);
}

UINT32 _B2R_GetContainerType(B2R_OBJECT_T *this)
{
    UINT32 u4Type = 0; //SWDMX_FMT_UNKNOWN;
    UCHAR ucB2rId;
    do 
    {
        if (!this)
        {
            break;
        }

        ucB2rId = this->ucB2rId;

        #ifdef ENABLE_MULTIMEDIA
        {
        VDEC_ES_INFO_T* prVdecEsInfo;
        VDP_CFG_T* prVdpConf;
        UCHAR ucEsId;
        prVdpConf = _B2R_GetDP(this);
        if (!prVdpConf)
        {
             break;
        }   
        ucEsId = prVdpConf->ucInputPort[0];
        prVdecEsInfo = (ucEsId < VDEC_MAX_ES)? _VDEC_GetEsInfo(ucEsId) : NULL;
        if (prVdecEsInfo)
        {
            u4Type = (UINT32)prVdecEsInfo->eContainerType;
        }
        }        
        #endif
    }while(0);

    return u4Type;
    
}

UINT32 _B2R_GetContentSourceType(B2R_OBJECT_T *this)
{
    UINT32 u4Type = 0; //SWDMX_SRC_TYPE_UNKNOWN;
    UCHAR ucB2rId;
    do 
    {
        if (!this)
        {
            break;
        }

        ucB2rId = this->ucB2rId;

        #ifdef ENABLE_MULTIMEDIA
        {
            VDEC_ES_INFO_T* prVdecEsInfo;
            VDP_CFG_T* prVdpConf;
            UCHAR ucEsId;
            prVdpConf = _B2R_GetDP(this);
            if (!prVdpConf)
            {
                 break;
            }   
            ucEsId = prVdpConf->ucInputPort[0];
            prVdecEsInfo = (ucEsId < VDEC_MAX_ES) ? _VDEC_GetEsInfo(ucEsId) : NULL;
            if (prVdecEsInfo)
            {
                u4Type = (UINT32)prVdecEsInfo->eMMSrcType;
            }
        }        
        #endif
    }while(0);

    return u4Type;
    
}

VOID _B2R_PreparePendingForNextVsync(B2R_OBJECT_T* this)
{
    B2R_PRM_T * prFrcPrm;
    if (!this)
    {
        return;
    }
    prFrcPrm = this->ptB2rPrm;
    if (!prFrcPrm)
    {
        return;
    }

    prFrcPrm->u2TargetNs = 0;
    prFrcPrm->u2ChangeFieldNs = 0;
    prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
    prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
    prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
    prFrcPrm->u4InFrameStc = 0;
    prFrcPrm->u4OutFrameStc = 0;
#ifdef CC_3D_MM_DS_SUPPORT
    // If L view is pending, R view should be pending, too.
    ASSERT(prFrcPrm->ucSubPendingFbId == FBM_FB_ID_UNKNOWN);
    prFrcPrm->ucSubPendingFbId = prFrcPrm->ucSubFbId;
    prFrcPrm->ucSubFbId = FBM_FB_ID_UNKNOWN;
#endif
}

UINT32 _B2R_SetPathMute(UCHAR ucB2rId, MUTE_MODULE_ID eModule, UINT32 u4UnMuteDelay, BOOL bReplaceByLongerDelay)
{
    BOOL u4Ret;
    UINT32 i;
    B2R_OBJECT_T* this;
    UCHAR ucVdpId;
    do 
    {
        if (ucB2rId >= B2R_NS)
        {
            u4Ret = B2R_INV_ARG;
            break;
        }

        this = _B2R_GetObj(ucB2rId);
        if(!this)
        {
            u4Ret = B2R_INTERNAL_ERR;
            break;
        }
        
        //for each DP
        for (i = 0; i < this->u4VideoPanelNs; i++)
        {
            VDP_CFG_T *ptDP;
            if( this->avVideoPanel[i]!=NULL)
            {
               ptDP = (VDP_CFG_T *)this->avVideoPanel[i];
               ucVdpId = _B2R_GetVdpId(ptDP->ucB2rId);
               LOG(3,"_B2R_SetPathMute----> ucVdpId=%d,ptDP->ucB2rId=%d\n",ucVdpId,ptDP->ucB2rId);
               _vDrvVideoSetMute(eModule, ucVdpId, u4UnMuteDelay, bReplaceByLongerDelay);
            }
        }

        u4Ret = B2R_OK;

    }while (0);

    return u4Ret;
}

void _B2R_GetSeek(B2R_OBJECT_T* this, UCHAR* pucSeekMode, UINT32* pu4Pts, UCHAR* pucNotifyTrigger)
{
    if (this)
    {
        B2R_VAR_T* prB2rVar = this->ptB2rVar;
        *pu4Pts = prB2rVar->rVdpSeek.u4SeekPts;
        *pucNotifyTrigger = prB2rVar->rVdpSeek.ucNotifyTrigger;
        *pucSeekMode = prB2rVar->rVdpSeek.ucSeekMode;
    }
    else
    {
        LOG(0, "null ptr [%s-%d]\n", __FUNCTION__, __LINE__);
    }
}

UINT32 _B2R_GetDispingPts(B2R_OBJECT_T* this)
{
    B2R_PRM_T * prFrcPrm;
    if (!this)
    {
        return 0;
    }
    prFrcPrm = this->ptB2rPrm;
    if (!prFrcPrm)
    {
        return 0;
    }
    return prFrcPrm->u4Pts;
}

UINT32 _B2R_GetEsmQCnt()
{
    return _B2R_GetFrcEsmQCnt();
}

UINT8 _B2R_GetVdpIds(UINT8 u1B2rId, UINT8* pu1VdpIds)
{
	UINT8 u1Idx;
	UINT8 u1Cnt = 0;
	if (!pu1VdpIds)
	{
		return 0;
	}
	for (u1Idx = 0; u1Idx < VDP_NS; u1Idx++)
	{
		if (VDP2B2RID(u1Idx) == u1B2rId)
		{
			pu1VdpIds[u1Cnt] = u1Idx;
			u1Cnt++;
		}
	}
	return u1Cnt;
}

UINT8 _B2R_GetVdpId(UCHAR ucB2rId)
{
    UINT8   u1_idx;
    UCHAR   ucVdpId = VDP_NS;

    do
    {
        if(ucB2rId >= B2R_NS)
        {
            break;
        }

        for(u1_idx = 0; u1_idx < VDP_NS; u1_idx ++)
        {
            if(VDP2B2RID(u1_idx) == ucB2rId)
            {
                ucVdpId = u1_idx;
                break;
            }
        }
    }while(0);
    
    return ucVdpId;
}

