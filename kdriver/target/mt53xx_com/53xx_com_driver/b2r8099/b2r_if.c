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
 * $Date: 2015/03/02 $
 * $RCSfile: b2r_if.c,v $
 * $Revision: #20 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_if.c
 *  Video Plane driver - public interface
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "b2r_drvif.h"
#include "b2r_if.h"
#include "fbm_drvif.h"
#include "vdp_if.h"
#include "vdp_drvif.h"
#include "drv_scaler.h"
#include "vdp_vsync.h"
#include "vdp_frc.h"
#include "vdp_hal.h"
#include "vdp_image.h"
#include "vdp_debug.h"
#include "drv_video.h"
#include "nptv_if.h"
#include "vdp_display.h"
#include "drv_mpeg.h"
#include "mute_if.h"
#include "video_def.h"
#include "sv_const.h"
#include "vdec_drvif.h"
#include "srm_drvif.h"
#include "drv_display.h"
#include "hw_vdoin.h"


#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

#ifdef CC_FAST_INIT
#include "x_bim.h"
#endif

#include "x_lint.h"
#include "x_assert.h"
//LINT_EXT_HEADER_END
#ifdef CC_SUPPORT_PIPELINE
BOOL fgLGPipLine =TRUE;
BOOL fgVdpModeChg[B2R_NS] ={FALSE,FALSE};
static VDP_CFG_T    _arVdpPipLineCfg;
#endif
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifndef CC_CLI
#define CC_CLI
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef void (*VDP_GET_PIC_TYPE_CB_FUNC)(VDEC_PICTYPE_CB_T* prPicType);

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

extern VDP_CB_FUNC_T _rVdpCbFunc; /* To include VDP callback function */
B2R_CB_FUNC_T _rB2rCbFunc;

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static B2R_PRM_T*    _prB2rPrm[B2R_NS];
static VDP_CFG_T*    _prVdpCfg[VDP_NS];
static UCHAR _aucImageConnected[VDP_NS];

static UCHAR _aucThumbnailMode[VDP_NS]; // Thumbnail mode in MM mode

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define VERIFY_VDP_ID(id, ret)                        \
        do {                                          \
            if ((id) >= VDP_NS) { return (ret); }     \
        } while (0)
#define VERIFY_VDP_ID_RET_VOID(id)                    \
        do {                                          \
            if ((id) >= VDP_NS) { return ; }          \
        } while (0) 

#define VERIFY_B2R_ID(id, ret)                        \
        do {                                          \
            if ((id) >= B2R_NS) { return (ret); }     \
        } while (0)

#define VERIFY_B2R_ID_RET_VOID(id)                    \
        do {                                          \
            if ((id) >= B2R_NS) { return ; }          \
        } while (0) 

#define VERIFY_NULL_POINT(ptr,ret)                     \
        do {                                           \
            if (ptr == NULL) { return ret; }           \
        } while (0)


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/**
 * VDP transfer ES id to VDP id
 *
 * @param ucEsId specify the video decoder id.
 * @return VDP id
 */
UINT32 VDP_Es2Vdp(UCHAR ucEsId)
{
    UINT32 u4VdpIdx;
    UINT32 u4PortIdx;
    UINT32 u4Return = VDP_1;
    CRIT_STATE_T rCritState;

    ASSERT(ucEsId < MAX_ES_NS);

    rCritState = x_crit_start();

    for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
    {
        for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
        {
            if (_prVdpCfg[u4VdpIdx]->ucInputPort[u4PortIdx] == ucEsId)
            {
                u4Return = u4VdpIdx;
                goto end;
            }
        }
    }

    end:    

    x_crit_end(rCritState);

    return u4Return;
}



/**
 * VDP DTV frame buffer group ready check
 *
 * @param void
 * @return void
 */
void _VdpCheckFbgReady(UCHAR ucFbgId, UCHAR ucEsId)
{
    UCHAR  ucGetFbgId;
    UINT32 u4EsId = (UINT32)ucEsId;
    UINT32 u4FbgNs;
    //UINT32 u4PortIdx;
    UINT32 u4FbgIdx;
    UINT32 u4VdpId;
    UINT32 u4DisplayReady =0;

    UNUSED(u4VdpId);

    u4FbgNs = FBM_GetFbgNs();
    for (u4FbgIdx = 0; u4FbgIdx < u4FbgNs; u4FbgIdx++)
    {
        u4EsId = (UINT32)ucEsId;
        u4DisplayReady = 0;
        if(u4EsId >= 0xFF)
        {
            for(u4EsId =0 ; u4EsId< MAX_ES_NS ; u4EsId++)
            {
                ucGetFbgId = FBM_GetFbgByEs(u4EsId);
                if (ucGetFbgId == (UCHAR)u4FbgIdx)
                {
                    u4DisplayReady = 1;
                    break;
                }
            }
        }
        else
        {
            if(u4EsId < MAX_ES_NS)
            {
                ucGetFbgId = FBM_GetFbgByEs(u4EsId);
                if (ucGetFbgId == (UCHAR)u4FbgIdx)
                {
                    u4DisplayReady = 1;
                    break;
                }
            }
            else
            {
                ASSERT((u4EsId < MAX_ES_NS));
            }
        }
        if(u4DisplayReady == 1)
        {
            break;
        }
    }

    if(u4FbgIdx < u4FbgNs)
    {
        if (u4DisplayReady)
        {
            FBM_SetFrameBufferFlag((UCHAR)u4FbgIdx, FBM_FLAG_DISP_READY);
        }
        else
        {
            FBM_ClrFrameBufferFlag((UCHAR)u4FbgIdx, FBM_FLAG_DISP_READY);
            FBM_SetFrameBufferFlag((UCHAR)u4FbgIdx, FBM_FLAG_RESET);
        }
    }

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        u4EsId = _prVdpCfg[u4VdpId]->ucInputPort[0];

        if (u4EsId >= MAX_ES_NS)
        {
            return;
        }

        ucGetFbgId = FBM_GetFbgByEs(u4EsId);
        if ((ucGetFbgId == FBM_FBG_ID_UNKNOWN) &&
                (_aucImageConnected[u4VdpId] == 0))
        {
            _VDP_StatusNotify(u4VdpId, VDP_B2R_NO_SIGNAL);
        }
    }
    UNUSED(ucFbgId);
}
/**
 * VDP DTV frame buffer group change notify
 *
 * @param ucFbgId specify the frame buffer group id.
 * @param ucEsId specify the video decoder id.
 * @return void
 */
#ifndef CC_SUPPORT_PIPELINE

static void _VdpSetNewFbg(UCHAR ucFbgId, UCHAR ucEsId)
{
    UINT32 u4VdpIdx;
    UINT32 u4PortIdx;
    UCHAR  ucB2rId = 0;
    UCHAR  ucInputPort;
    ASSERT(ucEsId < MAX_ES_NS);
    
    for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
    {
        ucB2rId = VDP2B2RID(u4VdpIdx);
        for (u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
        {
            B2R_MUTEX_LOCK(ucB2rId);
            ucInputPort = _prVdpCfg[u4VdpIdx]->ucInputPort[u4PortIdx];
            B2R_MUTEX_UNLOCK(ucB2rId);
            if (ucInputPort == ucEsId)
            {
                UINT8 bType;
                bType = bGetVideoDecType(u4VdpIdx);
                if((SV_VD_MPEGHD == bType) || (SV_VD_MPEGSD == bType))
                {
                    _vDrvVideoSetMute(MUTE_MODULE_B2R, u4VdpIdx, 10, TRUE);
                    _B2R_VsyncSendCmd(VDP2B2RID(u4VdpIdx), VDP_CMD_SET_INPUT);
                }
                u4PortIdx = VDP_MAX_INPORT_NS;
            }
        }
    }
    
    _VdpCheckFbgReady(ucFbgId, ucEsId);

    return;
}
#endif

static void _VdpFbgChgNotify(UCHAR ucFbgId, UCHAR ucEsId)
{
#ifdef CC_SUPPORT_PIPELINE
	VDP_PipeConnectFromVdec(ucEsId,ucFbgId);
#else
    _VdpSetNewFbg(ucFbgId,ucEsId);
#endif
}

#ifdef CC_SUPPORT_TVE
extern UCHAR _ucSameB2rVdp;
#endif

/**
 * VDP DTV video frontend enable/disable
 *
 * @param ucVdpId specify the video plane id.
 * @return void
 */
void VDP_SetB2rEnable(UCHAR ucVdpId)
{
    UCHAR ucB2rId;
    UCHAR ucEnable;
    VDP_CFG_T* ptFirstDP;
    B2R_OBJECT_T *this = NULL;
    VDP_CFG_T* ptDP;
    
    VERIFY_VDP_ID_RET_VOID(ucVdpId);

    ptDP = _prVdpCfg[ucVdpId];
    if(!ptDP)
    {
        return;
    }
    
    ucEnable = ptDP->ucEnable;
    ucB2rId = ptDP->ucB2rId;
    if(ucB2rId >= B2R_NS)
    {
        ptDP->ucEnable = 0;
        return;
    }
    
    if (!(this = _B2R_GetObj(ucB2rId)))
    {
        return;
    }
    
    B2R_MUTEX_LOCK(ucB2rId);

    ptDP->ucEnable = ptDP->ucVdpEnable;
    ptFirstDP = this->avVideoPanel[0];
    
    if (ptDP->ucEnable != ucEnable)
    {
        if (ptDP->ucEnable)
        {
            LOG(0, "VDP(%d) Enable B2R(%d)\n", ptDP->ucVdpId, ptDP->ucB2rId);


            //just add one DP
            if (ptFirstDP != NULL && ptFirstDP != ptDP)
            {
                ptDP->ucStatus = ptFirstDP->ucStatus;
                if (ptFirstDP->ucStatus == VDP_STATUS_STABLE && ptFirstDP->ucEnable)
                {
                    vMpegModeChg(ptDP->ucVdpId);
                    vMpegModeDetDone(ptDP->ucVdpId);
                }
                else
                {
                    //clone mute status
                    #ifdef VDP_MUTE_EARLY_DISPLAY
                    if (ptFirstDP->ucEarlyDisp== 1)
                    {
                        LOG(3, "DTV Mute Early Display -3 VDP(%d)\n", ptDP->ucVdpId);
                    }
#endif
                }
            }
            //just only one DP
            else if (ptFirstDP == ptDP)
            {
                ptDP->ucStatus = VDP_STATUS_NOSIGNAL;
                B2R_MUTEX_UNLOCK(ucB2rId);
                _VdpCheckFbgReady(0xFF, 0xFF);
                _B2R_VsyncSendCmd(ptDP->ucB2rId, VDP_CMD_SET_ENABLE);
            }

        }
        else
        {
            //remove this one, but still exsit another DP
            if (ptFirstDP != NULL && ptFirstDP != ptDP)
            {
                B2R_MUTEX_UNLOCK(ucB2rId);
                return;
            }
            else
            {
               if(( _prVdpCfg[ucVdpId]->ucB2rId==_prVdpCfg[1-ucVdpId]->ucB2rId)&&(_prVdpCfg[ucVdpId]->ucVdpEnable||_prVdpCfg[1-ucVdpId]->ucVdpEnable))
                {
                
                   B2R_MUTEX_UNLOCK(ucB2rId);
                   return;
                }
               else
               {
                    ptDP->ucStatus = VDP_STATUS_NOSIGNAL;
                    B2R_MUTEX_UNLOCK(ucB2rId);
                   _VdpCheckFbgReady(0xFF, 0xFF);
                   _B2R_VsyncSendCmd(ptDP->ucB2rId, VDP_CMD_SET_ENABLE);
                    {
                        VDP_CFG_T *ptVdpCfg = _B2R_GetVdpConf(VDP_1);
                        
                        if (ptVdpCfg)
                        {
                            ptVdpCfg->fgScartOut = FALSE;
                        }
                    }
                    LOG(0, "clear fgScartOut!!!!!!!!!!!!\n");
#ifdef CC_B2R_RM_SUPPORT
              //      b2r_resource_release(ptDP->ucB2rId);
#endif
                }
            }
            LOG(0, "VDP(%d) disable B2R(%d)\n", ptDP->ucVdpId, ptDP->ucB2rId);
        }
    }
    B2R_MUTEX_UNLOCK(ucB2rId);
}

/**
 * DTV status notify
 *
 * @param ucVdpId specify the video plane id.
 * @param u4Status specify DTV status.
 * @return void
 */
void _VDP_StatusNotify(UCHAR ucVdpId, UINT32 u4Status)
{
    UCHAR ucB2rId;

     B2R_OBJECT_T *this;
    
    
    LOG(3, "DTV(%d) Event(%d)\n", ucVdpId, u4Status);
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
     ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(ucB2rId);
   
    this = _B2R_GetObj(ucB2rId);
    B2R_MUTEX_LOCK(ucB2rId);

#ifdef __MODEL_slt__
    if (VDP2B2RID(ucVdpId) < B2R_NS)
    {
        if (u4Status == VDP_B2R_START_PLAY)
        {
            _prVdpCfg[ucVdpId]->ucStatus = VDP_STATUS_STABLE;
        }
        vMpegModeChg(ucVdpId);
        vMpegModeDetDone(ucVdpId);
    }

#else

#ifdef CC_SCPOS_EN
    if (VDP2B2RID(ucVdpId) < B2R_NS)
    {
        UINT32 u4IssueModeChange = 0;

        // When DTV signal not ready, mute SCPOS
        if ((u4Status == VDP_B2R_NO_SIGNAL))
        {
            _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, FOREVER_MUTE, FALSE);
            LOG(3, "DTV Mute B2R No Signal VDP(%d)\n", ucVdpId);
        }
        else if (u4Status == VDP_B2R_START_PLAY)
        {
#ifdef CC_SUPPORT_TVE
            BOOL bIsScartOut =  (VDP2B2RID(VDP_1) == B2R_1) &&
                                (VDP2B2RID(VDP_2) == B2R_1)&&
                                (_prVdpCfg[VDP_1]->ucEnable == 1) &&
                                (_prVdpCfg[VDP_2]->ucEnable == 1);
#endif
            if(_prB2rPrm[VDP2B2RID(ucVdpId)]->ucReady == 1)
            {
                if(_prVdpCfg[ucVdpId]->ucEarlyDisp == 1)
                {
                    LOG(3, "Original seq chg + video frame sync ready(%d) VDP(%d) earlydisp(%d)\n",_prB2rPrm[VDP2B2RID(ucVdpId)]->ucReady, ucVdpId, _prVdpCfg[ucVdpId]->ucEarlyDisp);
                    _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, FOREVER_MUTE, FALSE);
#ifdef CC_SUPPORT_TVE
                    if (bIsScartOut)
                    {
                        LOG(3, "Original seq chg + video frame sync ready(%d) VDP(%d) earlydisp(%d)\n",_prB2rPrm[VDP2B2RID(1-ucVdpId)]->ucReady, (1-ucVdpId), _prVdpCfg[1-ucVdpId]->ucEarlyDisp);
                        _vDrvVideoSetMute(MUTE_MODULE_DTV, (1 - ucVdpId), FOREVER_MUTE, FALSE);
                    }
#endif
                }
                else
                {
                    #ifdef TIME_MEASUREMENT
                    TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "B2R Set Unmute");
                    #endif
                    LOG(3, "Original seq chg + video frame sync ready(%d) VDP(%d) unearlydisp(%d)\n",_prB2rPrm[VDP2B2RID(ucVdpId)]->ucReady, ucVdpId, _prVdpCfg[ucVdpId]->ucEarlyDisp);
                    _vDrvVideoSetMute(MUTE_MODULE_B2R, ucVdpId, 10, FALSE);
                    _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, 0, FALSE);
#ifdef CC_SUPPORT_TVE
                    if (bIsScartOut)
                    {
                        LOG(3, "Original seq chg + video frame sync ready(%d) VDP(%d) unearlydisp(%d)\n",_prB2rPrm[VDP2B2RID(1-ucVdpId)]->ucReady, (1-ucVdpId), _prVdpCfg[1-ucVdpId]->ucEarlyDisp);
                        _vDrvVideoSetMute(MUTE_MODULE_B2R, (1-ucVdpId), 10, FALSE);
                        _vDrvVideoSetMute(MUTE_MODULE_DTV, (1-ucVdpId), 0, FALSE);
                    }
#endif
                }
            }
            else
            {   
                LOG(3, "seq chg speedup + video frame sync unready(%d) VDP(%d)\n",_prB2rPrm[VDP2B2RID(ucVdpId)]->ucReady, ucVdpId);
                _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, FOREVER_MUTE, FALSE);
#ifdef CC_SUPPORT_TVE
                if (bIsScartOut)
                {
                    LOG(3, "seq chg speedup + video frame sync unready(%d) VDP(%d)\n",_prB2rPrm[VDP2B2RID(1-ucVdpId)]->ucReady, (1-ucVdpId));
                   	_vDrvVideoSetMute(MUTE_MODULE_DTV, (1-ucVdpId), FOREVER_MUTE, FALSE);
                    if(((1-ucVdpId >= 0) && (1 - ucVdpId <= 1)) && (_prB2rPrm[VDP2B2RID(1-ucVdpId)]->ucReady == 1))
                       {
                         LOG(3, "Set ucEarlyDisp =%d\n",_prVdpCfg[1-ucVdpId]->ucEarlyDisp);
                        _prVdpCfg[1-ucVdpId]->ucEarlyDisp = 1;
                       }
                }
#endif
            }
        }

        if (u4Status == VDP_B2R_NO_SIGNAL)
        {
            if (_prVdpCfg[ucVdpId]->ucStatus != VDP_STATUS_NOSIGNAL)
            {
                u4IssueModeChange = 1;
            }

            _prVdpCfg[ucVdpId]->ucStatus = VDP_STATUS_NOSIGNAL;

#ifdef CC_SRM_ON
            _prVdpCfg[ucVdpId]->u4MpegHeight = 0;
            _prVdpCfg[ucVdpId]->u4MpegWidth = 0;

#endif
        }
        else if (u4Status == VDP_B2R_START_PLAY)
        {
            if (_prVdpCfg[ucVdpId]->ucStatus != VDP_STATUS_STABLE)
            {
                u4IssueModeChange = 1;
            }

            _prVdpCfg[ucVdpId]->ucStatus = VDP_STATUS_STABLE;
        }
        else
//        if ((u4Status == VDP_B2R_OUTPUT_CHG) || (u4Status == VDP_B2R_RESOLUTION_NFY))
        {
            // RM interface change
            static UCHAR _aucLastPmxMode[VDP_NS];

            if (_prVdpCfg[ucVdpId]->rOutInfo.ucPmxMode != _aucLastPmxMode[ucVdpId])
            {
                _aucLastPmxMode[ucVdpId] = _prVdpCfg[ucVdpId]->rOutInfo.ucPmxMode;

                if (_prVdpCfg[ucVdpId]->ucStatus == VDP_STATUS_STABLE)
                {
                    u4IssueModeChange = 1;
                }
            }
            else if (_prVdpCfg[ucVdpId]->ucStatus == VDP_STATUS_STABLE)
            {
                /* Code Review to use mode change instead of calling vApiVideoInputChangeNotify */
                u4IssueModeChange = 1;
            }
        }

        if (_aucThumbnailMode[ucVdpId] != 0)
        {
            if (u4IssueModeChange != 0)
            {
                u4IssueModeChange = 0;
                LOG(5, "Thumbnail mode, no mode change\n");
            }
        }

        if (u4IssueModeChange != 0)
        {
        #ifdef CC_SUPPORT_PIPELINE
            LOG(0, "ucVdpId(%d) DTV Mode Change,->fgVdpModeChg=%d,ucB2rId=%d\n\n", ucVdpId,VDP_PipeModeChangeing(ucVdpId,this->ucB2rId),this->ucB2rId);
		#else
		     LOG(0, "ucVdpId(%d) DTV Mode Change,ucB2rId=%d\n\n", ucVdpId,this->ucB2rId);
		#endif
#ifdef TIME_MEASUREMENT
            TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "B2R Mode Change");
#endif
            _vDrvVideoSetMute(MUTE_MODULE_B2R, ucVdpId, 0, FALSE);
            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, ucVdpId, 10, FALSE);
			#ifdef CC_SUPPORT_PIPELINE
             if(fgLGPipLine==FALSE || VDP_PipeModeChangeing(ucVdpId,this->ucB2rId))
			#endif
            {
                vMpegModeChg(ucVdpId);
                vMpegModeDetDone(ucVdpId);
				#ifdef CC_SUPPORT_PIPELINE
                VDP_PipeModeChangeDone(ucVdpId,this->ucB2rId);
				#endif
            }
            //#ifdef CC_B2R_RM_SUPPORT
            if((u4Status == VDP_B2R_START_PLAY)&&(_prVdpCfg[ucVdpId]->u4SrcWidth>1920||_prVdpCfg[ucVdpId]->u4SrcHeight>1088)&&_prVdpCfg[ucVdpId]->fgBypssEnabe)
            {
            
                LOG(1,"====================VB1 Timing Gen=======================\n");
                B2R_VB1_Setting();
            }
           //#endif
#ifndef CC_SUPPORT_NPTV_SEAMLESS

            if(this)
            {
                _B2R_NPTVSeamlessStatus(this, VDP_SMLS_PREPARE_WHILE_START_PLAY);
            }
#endif
        }
    }
    else
    {
        _prVdpCfg[ucVdpId]->u4MpegHeight = 0;
        _prVdpCfg[ucVdpId]->u4MpegWidth = 0;

    }
#endif

#endif // __MODEL_slt__

    B2R_MUTEX_UNLOCK(ucB2rId);
}

/**
 * DTV event notify
 *
 * @param ucVdpId specify the video plane id.
 * @param u4Arg1 specify arguement #1.
 * @param u4Arg1 specify arguement #2.
 * @param u4Arg1 specify arguement #3.
 * @return void
 */
void _VDP_EventNofify(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3)
{
    if (u4Arg1 >= (UINT32)VDP_CB_FUNC_NS)
    {
        ASSERT(u4Arg1 < (UINT32)VDP_CB_FUNC_NS);
        return;
    }

    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    if(!_prVdpCfg[ucVdpId])
    {
        return;
    }

    LOG(10, "EVENT NFY (%d %d %d %d)\n", ucVdpId, u4Arg1, u4Arg2, u4Arg3);

    if (VDP_CHECK_CB_FUNC_VERIFY(_rVdpCbFunc.au4CbFunc[u4Arg1], _rVdpCbFunc.au4CbFuncCRC[u4Arg1]))
    {
        if (u4Arg1 == (UINT32)VDP_CB_FUNC_PLAY_DONE_IND)
        {
            ((PLAYDONE_CFG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_PLAY_DONE_IND])(ucVdpId, u4Arg1, u4Arg2);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_AFD_IND)
        {
            ((AFD_CFG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_AFD_IND])(ucVdpId, u4Arg2, (FBM_ASPECT_RATIO_T*)u4Arg3);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_AFD_IND_2)
        {
            ((AFD_CFG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_AFD_IND_2])(ucVdpId, u4Arg2, (FBM_ASPECT_RATIO_T*)u4Arg3);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_PTS_IND)
        {
            ((PTS_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_PTS_IND])(ucVdpId, u4Arg2, u4Arg3);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_UNMUTE_IND)
        {
            ((UNMUTE_CFG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_UNMUTE_IND])(ucVdpId, (BOOL)u4Arg2);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_MM_COND_IND)
        {
            VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
            ((VDP_MM_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_COND_IND])
            (ucVdpId, (VDP_COND_T)u4Arg2,(BOOL)(_prB2rPrm[VDP2B2RID(ucVdpId)]->fgFastForward),
             (UINT32)(u4Arg3));
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_LIPSYNC_IND)
        {
            ((VDP_LIPSYNC_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_LIPSYNC_IND])();
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_Get_PIC_TYPE)
        {
            VDEC_PICTYPE_CB_T rPicType;
            x_memset(&rPicType, 0, sizeof(VDEC_PICTYPE_CB_T));
            rPicType.u1EsId = (UINT8)u4Arg2;
            rPicType.u1PicType = (UINT8)u4Arg3;
            ((VDP_GET_PIC_TYPE_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_Get_PIC_TYPE])(&rPicType);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_MM_STEP_FIN_IND)
        {
            VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
            ((VDP_STEP_FIN_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_STEP_FIN_IND])
            (ucVdpId, u4Arg2, u4Arg3,
             _prB2rPrm[VDP2B2RID(ucVdpId)]->u4AttachedSrcId);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_MM_SEEK_FIN_IND)
        {
            UCHAR ucB2rId;

            ucB2rId = VDP2B2RID(ucVdpId);
            VERIFY_B2R_ID_RET_VOID(ucB2rId);

            ((VDP_SEEK_FIN_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_MM_SEEK_FIN_IND])(_prB2rPrm[ucB2rId]->u4AttachedSrcId, ucVdpId, u4Arg2, u4Arg3);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_REPEAT_DROP_IND)
        {
            UCHAR ucEsId = VDP_Vdp2Es(ucVdpId);
            ((VDP_REPEAT_DROP_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_REPEAT_DROP_IND])(ucVdpId, u4Arg2, ucEsId);
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_LIPSYNC_OK_TO_NG_IND)
        {
            ((VDP_LIPSYNC_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_LIPSYNC_OK_TO_NG_IND])();
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_LIPSYNC_NG_TO_OK_IND)
        {
            ((VDP_LIPSYNC_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_LIPSYNC_NG_TO_OK_IND])();
        }
        else if (u4Arg1 == (UINT32)VDP_CB_FUNC_RES_CHG_IND)
        {
            ((VDP_RES_CHG_CB_FUNC)_rVdpCbFunc.au4CbFunc[VDP_CB_FUNC_RES_CHG_IND])(ucVdpId);
        }
        else
        {
            ASSERT(0);
        }
    }
    else
    {
        ASSERT(_rVdpCbFunc.au4CbFunc[u4Arg1] == 0);
    }
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

UINT32 VDP_SetPicInfo(UCHAR ucVdpId, VDP_SET_PIC_INFO_T rPicInfo)
{
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucEsId;
    UCHAR ucB2rId;
    UINT32 u4Ret;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);


    do 
    {
        u4Ret = VDP_SET_OK;

        B2R_MUTEX_LOCK(ucB2rId);
    
        ucEsId = VDP_Vdp2Es(ucVdpId);

        B2R_MUTEX_UNLOCK(ucB2rId);

        if (ucEsId >= MAX_ES_NS)
        {
            u4Ret =  VDP_SET_ERROR;
            break;
        }
        
        ucFbgId = FBM_GetFbgByEs(ucEsId);
        ucFbId = VDP_FrcGetLockFrameBuffer(ucVdpId);

        if (ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);

            if ((prFbmSeqHdr == NULL) || (prFbmPicHdr == NULL))
            {
                u4Ret =  VDP_SET_ERROR;
                break;
            }
            else
            {
                prFbmPicHdr->u4TickNum = rPicInfo.u4TickNum;
                B2R_MUTEX_LOCK(ucB2rId);
                _B2R_SetCurrentTick(VDP2B2RID(ucVdpId), rPicInfo.u4TickNum);
                B2R_MUTEX_UNLOCK(ucB2rId);
            }
        }
        else
        {
            u4Ret =  VDP_SET_ERROR;
            break;
        }
    
    } while (0);
    
    return u4Ret;
}

UINT32 VDP_GetPicInfo(UINT8 ucVdpId, VDP_PIC_INFO_T* prPicInfo)
{
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucEsId;
    UCHAR ucB2rId;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;
    UINT32 u4Pts;
    UINT32 u4Ret;

    VERIFY_NULL(prPicInfo);
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    do
    {
        u4Ret = VDP_SET_OK;
        
        B2R_MUTEX_LOCK(ucB2rId);

        ucEsId = VDP_Vdp2Es(ucVdpId);
        
        B2R_MUTEX_UNLOCK(ucB2rId);

        if (ucEsId >= MAX_ES_NS)
        {
            u4Ret =  VDP_SET_ERROR;
            break;
        }
        ucFbgId = FBM_GetFbgByEs(ucEsId);
        ucFbId = VDP_FrcGetLockFrameBuffer(ucVdpId);
        
        if (ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);
        
            if ((prFbmSeqHdr == NULL) || (prFbmPicHdr == NULL))
            {
                u4Ret =  VDP_SET_ERROR;
                break;
            }
            else
            {
                prPicInfo->ucHours = prFbmSeqHdr->ucHours;
                prPicInfo->ucMinutes = prFbmSeqHdr->ucMinutes;
                prPicInfo->ucSeconds = prFbmSeqHdr->ucSeconds;
                prPicInfo->ucPictures = prFbmSeqHdr->ucPictures;
                prPicInfo->u4TotlaTimeOffset = prFbmPicHdr->u4TotlaTimeOffset;
                prPicInfo->u4TickNum = prFbmPicHdr->u4TickNum;
        
                prPicInfo->u8Offset = prFbmPicHdr->u8Offset;
                prPicInfo->u8OffsetDisp = prFbmPicHdr->u8OffsetDisp;
                //prPicInfo->u4Pts = prFbmPicHdr->u4PTS;
                u4Pts = _B2R_GetCurrentPts(VDP2B2RID(ucVdpId));
        
                prPicInfo->u4Pts = u4Pts;
        
                if (prFbmPicHdr->ucTrueZeroPTS == FBM_USE_LAST_PTS_IF_ZERO)
                {
                    if (u4Pts == 0)
                    {
                        B2R_MUTEX_LOCK(ucB2rId);
                        prPicInfo->u4Pts = _B2R_GetLastPts(VDP2B2RID(ucVdpId));
                        B2R_MUTEX_UNLOCK(ucB2rId);
                    }
                }
            }
        }
        else
        {
            u4Ret =  VDP_SET_ERROR;
            break;
        }

    } while (0);

    return u4Ret;
}

/**
 * Get mute status of DTV playback
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE for mute, FALSE for not mute
 */
BOOL VDP_GetUnMuteStatus(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, FALSE);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FALSE);
    return _B2R_FrcGetUnMuteStatus(VDP2B2RID(ucVdpId));
}

UINT32 VDP_SetInput(UCHAR ucVdpId, UCHAR ucEsId, UCHAR ucPort)
{

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);

    if (ucPort >= VDP_MAX_INPORT_NS)
    {
        return VDP_SET_ERROR;
    }

    LOG(0,"Set input with vdpid(%d) esid = %d,ucPort=%d\n",ucVdpId,ucEsId,ucPort);
    if (_prVdpCfg[ucVdpId]->ucInputPort[ucPort] == ucEsId)
    {
        LOG(1,"Equal return\n");
        return VDP_SET_OK;
    }
    

    B2R_MUTEX_LOCK(ucVdpId);

    //workaround for 3D
    _prVdpCfg[ucVdpId]->ucInputPort[ucPort] = ucEsId;

    B2R_MUTEX_UNLOCK(ucVdpId);

#ifdef CC_B2R_RES_SUPPORT
    /*handle scart out case*/
   #ifndef CC_SUPPORT_PIPELINE
    _B2R_HandleScartOut(ucVdpId,ucEsId,ucPort);
   #endif
#endif

    VDP_VsyncSendCmd(ucVdpId, VDP_CMD_SET_INPUT);
    _VdpCheckFbgReady(0xFF, 0xFF);

    return VDP_SET_OK;
}

VOID _B2R_AddDP(B2R_OBJECT_T *this, VOID *pvDP)
{
    UINT32 i;

    if (!this || !pvDP)
    {
        return;
    }

    for (i = 0; i < this->u4VideoPanelNs; i++)
    {
        if (this->avVideoPanel[i] == pvDP)
        {
            return;
        }
    }

    for (i = 0; i < this->u4VideoPanelNs; i++)
    {
        if (this->avVideoPanel[i] == NULL)
        {
            this->avVideoPanel[i] = pvDP;
            break;
        }
    }
    
}

VOID _B2R_RemoveDP(B2R_OBJECT_T *this, VOID *pvDP)
{
    UINT32 i,j;

    if (!this || !pvDP)
    {
        return;
    }

    for (i = 0; i < this->u4VideoPanelNs; i++)
    {
        if (this->avVideoPanel[i] == pvDP)
        {
            this->avVideoPanel[i] = NULL;
        }
    }

    //re-order
    for (i = 0; i < this->u4VideoPanelNs; i++)
    {
        if (this->avVideoPanel[i] == NULL)
        {
            for (j = i+1; j < this->u4VideoPanelNs; j++)
            {
                if (this->avVideoPanel[j] != NULL)
                {
                    this->avVideoPanel[i] = this->avVideoPanel[j];
                    this->avVideoPanel[j] = NULL;
                    break;
                }
            }
        }
    }
    
}

B2R_OBJECT_T *_B2R_RegDP(UCHAR ucB2rId, VDP_CFG_T* ptDP)
{
    UCHAR ucOldB2rId;
    B2R_OBJECT_T *this = NULL;

    if (!ptDP)
    {
        return this;
    }

    ucOldB2rId = ptDP->ucB2rId;
    ptDP->ucB2rId = ucB2rId;

#ifdef CC_SUPPORT_TVE
    //old style, todo 
    if ( (ucB2rId == B2R_1) &&
         (VDP2B2RID(VDP_1) == VDP2B2RID(VDP_2)))
    {
        _ucSameB2rVdp = VDP_1;
    }
    else
    {
        _ucSameB2rVdp = VDP_NS;
    }
#endif

    B2R_MUTEX_UNLOCK(ucB2rId);
    
    //Remove DP from b2r 
    if (ucB2rId >= B2R_NS)
    {
        if ((this = _B2R_GetObj(ucOldB2rId)) != NULL)
        {
            _B2R_RemoveDP(this, (VOID *)ptDP);
        }
    }
    else
    {
        if ((this = _B2R_GetObj(ucB2rId)) != NULL)
        {
            _B2R_AddDP(this, (VOID *)ptDP);
        }
    }

    return this;
}


VOID _B2R_ProcDPs(B2R_OBJECT_T *this, VDP_CFG_T* ptDP)
{
    VDP_CFG_T* ptFirstDP;
    UCHAR ucVdpId;
    UCHAR ucB2rId;

    if (!ptDP || !this)
    {
        return; 
    }

    ucVdpId = ptDP->ucVdpId;
    //mute first
    if (ptDP->ucB2rId >= B2R_NS)
    {
#ifdef CC_SCPOS_EN
        UCHAR ucCnt,ucIdx;
        UCHAR aucVdpIds[VDP_NS];

        ucCnt = _B2R_GetVdpIds(ptDP->ucB2rId, aucVdpIds);
        LOG(1,"ucB2rId : %d , ucCnt : %d!\n",ptDP->ucB2rId,ucCnt);
        for (ucIdx = 0; ucIdx < ucCnt; ucIdx++)
        {
            ucVdpId = aucVdpIds[ucIdx];
            // When video plane connect to non-DTV, unmute DTV mask
            _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, 0, FALSE);
            LOG(0, "DTV Unmute B2R Switch VDP(%d)\n",ucVdpId);
        }
#endif
        ptDP->u4Afd = ACT_FMT_DEFAULT;
    }

    ucB2rId = ptDP->ucB2rId;

    B2R_MUTEX_LOCK(ucB2rId);

    ptFirstDP = this->avVideoPanel[0];

    if (ptFirstDP != NULL && ptFirstDP != ptDP) 
    {
        // Copy VdpConf here.
        ptDP->u4MpegHeight  = ptFirstDP->u4MpegHeight;
        ptDP->u4MpegWidth   = ptFirstDP->u4MpegWidth;
        x_memcpy(&ptDP->rOutInfo, &ptFirstDP->rOutInfo, sizeof(B2R_OUT_INFO_T));
        ptDP->u4SrcLineSize = ptFirstDP->u4SrcLineSize;
        ptDP->u4SrcHeight   = ptFirstDP->u4SrcHeight;
        ptDP->u4SrcWidth    = ptFirstDP->u4SrcWidth;
        ptDP->u4FrameRate   = ptFirstDP->u4FrameRate;
    
        /* To protect ucEarlyDisp is set between "connect sub" and "connect main" */
        ptDP->ucEarlyDisp   = ptFirstDP->ucEarlyDisp;
        ptDP->u4AspectRatio = ptFirstDP->u4AspectRatio;
        ptDP->u4Afd         = ptFirstDP->u4Afd;
    }

    B2R_MUTEX_UNLOCK(ucB2rId);

    //part 2
    VDP_SetB2rEnable(ucVdpId);
}

/**
 * VDP switch B2R to corresponding video plane
 *
 * @param ucVdpId specify the video plane id.
 * @param ucB2rId specify B2R id (0 only)
 * @return void
 */
#ifdef CC_SUPPORT_PIPELINE

#define CONNECTION_ADAPTOR_NUM 3
#define DISCONNECT_ES_ID 0xFF

#define CONN_ES_CNT 3
#define CONN_VDP_CNT 2
#define PIPE_LOCK(mutex) VERIFY(x_sema_lock(mutex, X_SEMA_OPTION_WAIT) == OSR_OK)
#define PIPE_UNLOCK(mutex) VERIFY(x_sema_unlock(mutex) == OSR_OK)


typedef struct
{
   BOOL fgVdecReady;
   BOOL fgVdpReady;
   BOOL fgConnected;
   BOOL fgModeChanging;
   UCHAR ucVdpId;
   UCHAR ucEsId;
   UCHAR ucFbgId;
   UCHAR ucB2rId;
   //VDP_CFG_T rConfigInfor;
}CONNECTION_ADAPTOR;

typedef enum
{
   E_CONNECT_SRC_VDP,
   E_CONNECT_SRC_VDEC
}E_CONNECT_SRC;

static CONNECTION_ADAPTOR rConnAdaptor[CONN_VDP_CNT][CONN_ES_CNT];
static HANDLE_T _rPipeMutex;
static PFN_VDEC_CALLSTACK_CB pfnStackInfor =NULL;
static VOID _Vdp_PipInit(VOID)
{
    UCHAR ucVdecId,ucVdpId;
    CONNECTION_ADAPTOR *prConnAdaptor;
    LOG(0,"[Pipe] Start init\n");
    x_memset((void *)rConnAdaptor, 0, sizeof(rConnAdaptor));

    for(ucVdpId = 0; ucVdpId < CONN_VDP_CNT; ucVdpId++)
    {
        for(ucVdecId = 0; ucVdecId < CONN_ES_CNT; ucVdecId++)
        {
            prConnAdaptor = &rConnAdaptor[ucVdpId][ucVdecId];
            prConnAdaptor->ucVdpId = ucVdpId;
            prConnAdaptor->ucEsId = ucVdecId;
            prConnAdaptor->ucFbgId = FBM_FBG_ID_UNKNOWN;
            prConnAdaptor->ucB2rId = B2R_HW_MAX_ID;
        }
    }
    VERIFY(x_sema_create(&_rPipeMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
    return;
}


static VOID _VDP_PipeLineSwitch(UCHAR ucVdpId, UCHAR ucB2rId)
{
		B2R_OBJECT_T *this;
		VDP_CFG_T* ptDP;
	
#ifdef CC_B2R_RES_SUPPORT
		UCHAR  ucRetB2rid = B2R_NS;    
#endif
		
		// if ucB2rId = B2R_NS, it means that video plane switch to non-DTV video input
		LOG(0, "Switch VdpId(%d) with B2rId(%d)!\n",ucVdpId, ucB2rId);
		VERIFY_VDP_ID_RET_VOID(ucVdpId);
		
#ifdef CC_B2R_RES_SUPPORT
		

		ucRetB2rid = _B2R_ChkHwConflict(ucVdpId,ucB2rId);
		if(ucRetB2rid != B2R_NS)
		{
			ucB2rId = ucRetB2rid;
		}
		else
		{
			LOG(1,"_B2R_ChkHwConflict return invalid b2rid(%d)!\n",ucRetB2rid);
		}
		
#endif
	
		ptDP = _prVdpCfg[ucVdpId];
	
		this = _B2R_RegDP(ucB2rId, ptDP);
	
		LOG(0, "VdpId(%d) switch to B2rId(%d),vdpId%d,B2Rid=%d\n",ucVdpId, ucB2rId,_prVdpCfg[ucVdpId]->ucVdpId,_prVdpCfg[ucVdpId]->ucB2rId);
#ifdef CC_SCPOS_EN
		vMpegInitB2rConf();
#endif
      
		_B2R_ProcDPs(this, ptDP);
	
}

static VOID _Vdp_PipeConnect(CONNECTION_ADAPTOR *prAdaptor,E_CONNECT_SRC eSrcType)
{
    B2R_HAL_OMUX_T rOmux;
    UCHAR ucPlayMode = FBM_FBG_DTV_MODE;
    if(prAdaptor->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
	    B2R_OBJECT_T *this;
        if(prAdaptor->ucB2rId != B2R_HW_MAX_ID)
        {
            LOG(1,"[Pipe]_Vdp_PipConnect(VdpId=%d,EsId=%d,FbgId=%d,B2rId=%d SrcType=%d)\n",\
                prAdaptor->ucVdpId,prAdaptor->ucEsId,prAdaptor->ucFbgId,prAdaptor->ucB2rId,eSrcType);
            prAdaptor->fgModeChanging = FALSE;

		   
		   if(prAdaptor->ucVdpId == VDP_2 && bApiQuearyScartOutStatus() && B2R_GetVdpConf(VDP_1))
		   {
		       UCHAR ucEsId,ucFbgId;

			   ucEsId = VDP_Vdp2Es(VDP_1);
			   ucFbgId = FBM_GetFbgByEs(ucEsId);
			   
			   FBM_GetPlayMode(ucFbgId ,&ucPlayMode);
		       if(ucPlayMode == FBM_FBG_MM_MODE)
		       {
		           LOG(0,"[Pipe]_Vdp_PipConnect scant can't connect to MM play\n");
                   return ;
		       }
			   else
			   {
				   x_memcpy(B2R_GetVdpConf(prAdaptor->ucVdpId),B2R_GetVdpConf(VDP_1),sizeof(VDP_CFG_T)); 
			   }
		   }
		   else
		   {			   
			   _VDP_PipeLineSwitch(prAdaptor->ucVdpId,prAdaptor->ucB2rId); 
			   
			   LG_PipLine_VDP_SetEnable(prAdaptor->ucVdpId,TRUE); 
			   
			   VDP_SetInput(prAdaptor->ucVdpId,prAdaptor->ucEsId,0);
			   
			   _vDrvVideoSetMute(MUTE_MODULE_B2R, prAdaptor->ucVdpId, 10, TRUE);
			   
			   _B2R_VsyncSendCmd(prAdaptor->ucB2rId, VDP_CMD_SET_INPUT);
			   
			   _VdpCheckFbgReady(prAdaptor->ucFbgId, prAdaptor->ucEsId);
			   
			   
			   vMpegHdConnect(prAdaptor->ucVdpId,SV_ON);

		   }

            x_memset(&rOmux,0,sizeof(B2R_HAL_OMUX_T));
            rOmux.ucPath = prAdaptor->ucVdpId;
            rOmux.fgScartOut =FALSE;

            this = _B2R_GetObj(prAdaptor->ucB2rId);
            if(this)
            {
                B2R_HAL_Set(this->hB2r, B2R_HAL_OMUX, &rOmux);
            }
            else
            {
                LOG(1,"[Pipe][Error]_Vdp_PipConnect _B2R_GetObj error\n");
            }
            
            prAdaptor->fgModeChanging = TRUE;
            vMpegModeChg(prAdaptor->ucVdpId);
            vMpegModeDetDone(prAdaptor->ucVdpId);
        }
        else 
        {
            LOG(0,"[Pipe][Error]_Vdp_PipConnect(VdpId=%d,EsId=%d,FbgId=%d,B2rId=%d,SrcType=%d) B2r Error\n",\
                prAdaptor->ucVdpId,prAdaptor->ucEsId,prAdaptor->ucFbgId,prAdaptor->ucB2rId,eSrcType);
        }
    }
    else
    {
        LOG(0,"[Pipe][Error]_Vdp_PipConnect(VdpId=%d,EsId=%d,ucFbgId=%d,SrcType=%d) Fbg Error\n",\
            prAdaptor->ucVdpId,prAdaptor->ucEsId,prAdaptor->ucFbgId,eSrcType);
    }
    
    return;
}

static VOID _Vdp_PipeDisConnect(CONNECTION_ADAPTOR *prAdaptor,E_CONNECT_SRC eSrcType)
{   
    LOG(1,"[Pipe]_Vdp_PipDisConnect(VdpId=%d,EsId=%d,FbgId=%d,B2rId=%d SrcType=%d)\n",\
                prAdaptor->ucVdpId,prAdaptor->ucEsId,prAdaptor->ucFbgId,prAdaptor->ucB2rId,eSrcType);
    B2R_MUTEX_UNLOCK(prAdaptor->ucB2rId);
    LG_PipLine_VDP_SetEnable(prAdaptor->ucVdpId,FALSE);
    vMpegHdConnect(prAdaptor->ucVdpId,SV_OFF);
    _VDP_PipeLineSwitch(prAdaptor->ucVdpId,B2R_NS); 
    VDP_SetInput(prAdaptor->ucVdpId,0xff,0);    
    prAdaptor->fgModeChanging = FALSE;
    return;
}

VOID VDP_PipeRegPrintStackCb(PFN_VDEC_CALLSTACK_CB cb)
{
   pfnStackInfor = cb;
   return;
}

BOOL VDP_PipeModeChangeing(UCHAR ucVdpId,UCHAR ucB2rId)
{ 
    CONNECTION_ADAPTOR *prConnAdaptor;
    UCHAR ucVdecId,ucConnectedCnt=0;
    if(ucB2rId >= B2R_HW_MAX_ID || ucVdpId >=CONN_VDP_CNT)
    {
        LOG(0,"[Pipe][Error]VDP_PipeInModeChangeing(%d,%d) Param Error\n",ucVdpId,ucB2rId);
        return FALSE;
    }

	for(ucVdecId=0; ucVdecId < CONN_ES_CNT; ucVdecId++)
	{
	   prConnAdaptor = &rConnAdaptor[ucVdpId][ucVdecId];
	   if(/*prConnAdaptor->ucB2rId ==ucB2rId &&*/prConnAdaptor->fgModeChanging)
	   {
		  LOG(1,"[Pipe]Doing Mode change conn=%d,vdp=%d,Es=%d,fbg=%d,b2r=%d\n",
			prConnAdaptor->fgConnected,ucVdpId,ucVdecId,prConnAdaptor->ucFbgId,prConnAdaptor->ucB2rId);
		  ucConnectedCnt++;
	   }
	}
    
    LOG(1,"[Pipe]VDP_PipeModeChangeing(%d,%d) cnt=%d\n",ucVdpId,ucB2rId,ucConnectedCnt);
    if(ucConnectedCnt > 0)
    {
       if(ucConnectedCnt > 1)
       {
           LOG(0,"[Pipe][Warning]VDP_PipeInModeChangeing(Vdp=%d,cnt=%d) \n",ucVdpId,ucConnectedCnt);
       }
       return TRUE;
    }

    return FALSE;
}

VOID VDP_PipeModeChangeDone(UCHAR ucVdpId,UCHAR ucB2rId)
{
    CONNECTION_ADAPTOR *prConnAdaptor;
    UCHAR ucVdecId,ucConnectedCnt=0;
    if(ucB2rId >= B2R_HW_MAX_ID || ucVdpId >=CONN_VDP_CNT)
    {
        LOG(0,"[Pipe][Error]VDP_PipeModeChangeDone(%d,%d) Param Error\n",ucVdpId,ucB2rId);
        return;
    }

	for(ucVdecId=0; ucVdecId < CONN_ES_CNT; ucVdecId++)
	{
	   prConnAdaptor = &rConnAdaptor[ucVdpId][ucVdecId];
	   
	   if(/*prConnAdaptor->ucB2rId ==ucB2rId && */prConnAdaptor->fgModeChanging)
	   {
		  prConnAdaptor->fgModeChanging = FALSE;
		  LOG(1,"[Pipe]Doing Mode change done, conn=%d,vdp=%d,es=%d,fbg=%d,b2r=%d\n",
			prConnAdaptor->fgConnected,ucVdpId,ucVdecId,prConnAdaptor->ucFbgId,prConnAdaptor->ucB2rId);
		  ucConnectedCnt++;
	   }
	}
    
    LOG(1,"[Pipe]VDP_PipeModeChangeDone(%d,%d) cnt=%d\n",ucVdpId,ucB2rId,ucConnectedCnt);
    if(ucConnectedCnt > 0)
    {
       if(ucConnectedCnt > 1)
       {
           LOG(0,"[Pipe][Warning]VDP_PipeModeChangeDone(Vdp=%d,cnt=%d) \n",ucVdpId,ucConnectedCnt);
       }
       return;
    }

    return;
    
}

BOOL VDP_PipeIsConnected(UCHAR ucVdpId,UCHAR ucEsId)
{
    if(ucVdpId >= CONN_VDP_CNT || ucEsId >= CONN_ES_CNT)
    {
        LOG(0,"_Vdp_PipConnted(%d,%d) ParamErro\n",ucVdpId,ucEsId);
        return FALSE;
    }
    return rConnAdaptor[ucVdpId][ucEsId].fgConnected;
}

VOID VDP_PipeGetInifor(VOID)
{
    CONNECTION_ADAPTOR *prConnAdaptor;
    UCHAR ucVdecId,ucVdpId;

    LOG(0,"\n");
    PIPE_LOCK(_rPipeMutex);
    for(ucVdpId = 0; ucVdpId < CONN_VDP_CNT; ucVdpId++)
    {
        for(ucVdecId = 0; ucVdecId < CONN_ES_CNT; ucVdecId++)
        {
            prConnAdaptor = &rConnAdaptor[ucVdpId][ucVdecId];
            LOG(0,"Pipe[%d][%d]:C=%d,V=%d,VR=%d,E=%d,ER=%d,F=%d,B=%d,M=%d\n",ucVdpId,ucVdecId,\
                prConnAdaptor->fgConnected,prConnAdaptor->ucVdpId,prConnAdaptor->fgVdpReady,prConnAdaptor->ucEsId,\
                prConnAdaptor->fgVdecReady,prConnAdaptor->ucFbgId,prConnAdaptor->ucB2rId,prConnAdaptor->fgModeChanging);
        }
    }
    
    PIPE_UNLOCK(_rPipeMutex);
    return;
}

VOID VDP_PipeConnectFromVdp(UCHAR ucVdpId,UCHAR ucEsId)
{
    CONNECTION_ADAPTOR *prConnAdaptor;
    UCHAR ucVdecId;
    
    if(ucVdpId >= VDP_MAX)
    {
        LOG(0,"[Pipe][Error]VDP_PipeConnectFromVdp(%d,%d) Param Error\n",ucVdpId,ucEsId);
        return;
    }
    
    LOG(1,"[Pipe]VDP_PipeConnectFromVdp(%d,%d)\n",ucVdpId,ucEsId);
    if(pfnStackInfor) pfnStackInfor(VDEC_DEBUG_CALLSTACK_T_VDEC_PIPE,"VDP_PipeConnectFromVdp",ucEsId);
    PIPE_LOCK(_rPipeMutex);

    if(ucEsId == DISCONNECT_ES_ID)
    {
        for(ucVdecId=0; ucVdecId < CONN_ES_CNT; ucVdecId++)
        {
            prConnAdaptor = &rConnAdaptor[ucVdpId][ucVdecId];
            prConnAdaptor->fgVdpReady = FALSE;
            if(prConnAdaptor->fgConnected)
            {
                _Vdp_PipeDisConnect(prConnAdaptor,E_CONNECT_SRC_VDP);
                prConnAdaptor->fgConnected = FALSE;
            }
        }
    }
    else if( ucEsId < CONN_ES_CNT)
    {
        for(ucVdecId=0; ucVdecId < CONN_ES_CNT; ucVdecId++)
        {
            prConnAdaptor = &rConnAdaptor[ucVdpId][ucVdecId];
            if(ucVdecId != ucEsId)
            {
                if(prConnAdaptor->fgConnected)
                {
                    _Vdp_PipeDisConnect(prConnAdaptor,E_CONNECT_SRC_VDP);
                    prConnAdaptor->fgConnected = FALSE;
                }
                
                prConnAdaptor->fgVdpReady = FALSE;
            }
        }
        
        prConnAdaptor = &rConnAdaptor[ucVdpId][ucEsId];
        prConnAdaptor->fgVdpReady = TRUE;
        if(prConnAdaptor->fgConnected == FALSE && prConnAdaptor->fgVdecReady)
        {
            _Vdp_PipeConnect(prConnAdaptor,E_CONNECT_SRC_VDP);
            prConnAdaptor->fgConnected = TRUE;
        }
    }
    else 
    {
        LOG(0,"[Pipe][Error]VDP_PipeConnectFromVdp(%d,%d) EsId Error\n",ucVdpId,ucEsId);
    }
    
    PIPE_UNLOCK(_rPipeMutex);
    LOG(1,"[Pipe]VDP_PipeConnectFromVdp(%d,%d) done\n",ucVdpId,ucEsId);
    return;
}


void VDP_PipeConnectFromVdec(UCHAR ucEsId,UCHAR ucFbgId)
{
    CONNECTION_ADAPTOR *prConnAdaptor;
    UINT8 ucVdpId;
    //UINT8 ucFbmPlayMode;
    BOOL fgRenderQFlushed = FALSE;

    if(ucEsId >= CONN_ES_CNT)
    {
        LOG(0,"[Pipe][Error]VDP_PipeConnectFromVdec(%d,%d) Param Error\n",ucEsId,ucFbgId);
        return;
    }
    
    LOG(1,"[Pipe]VDP_PipeConnectFromVdec(%d,%d)\n",ucEsId,ucFbgId);
    if(pfnStackInfor) pfnStackInfor(VDEC_DEBUG_CALLSTACK_T_VDEC_PIPE,"VDP_PipeConnectFromVdec",ucEsId);
    PIPE_LOCK(_rPipeMutex);
	if(ucFbgId == FBM_FBG_ID_UNKNOWN) //Disconnect
    {
        for(ucVdpId = 0; ucVdpId < VDP_MAX; ucVdpId++)
        {
            prConnAdaptor = &rConnAdaptor[ucVdpId][ucEsId];

            prConnAdaptor->fgVdecReady = FALSE;
            if(prConnAdaptor->fgConnected)
            {
                _Vdp_PipeDisConnect(prConnAdaptor,E_CONNECT_SRC_VDEC);
                prConnAdaptor->fgConnected =  FALSE;
            }
            
            if(prConnAdaptor->ucB2rId != B2R_HW_MAX_ID)
            {
                _B2R_FlushB2RChgFrameMsg(prConnAdaptor->ucB2rId);
            }
            
            prConnAdaptor->ucFbgId = FBM_FBG_ID_UNKNOWN;
            prConnAdaptor->ucB2rId = B2R_HW_MAX_ID;
        }
    }
	else
    {        
        for(ucVdpId = 0; ucVdpId < VDP_MAX; ucVdpId++)
        {
            prConnAdaptor = &rConnAdaptor[ucVdpId][ucEsId];
            prConnAdaptor->fgVdecReady = TRUE;            

            if(prConnAdaptor->ucFbgId !=ucFbgId && fgRenderQFlushed == FALSE)
            {
               _B2R_FlushB2RChgFrameMsg(FBM_B2rResIdAccess(ucFbgId, RES_R, NULL));
               fgRenderQFlushed = TRUE;
            }

            if(prConnAdaptor->fgVdpReady && prConnAdaptor->fgConnected == FALSE)
            {
                prConnAdaptor->ucFbgId = ucFbgId;
                prConnAdaptor->ucB2rId = FBM_B2rResIdAccess(ucFbgId, RES_R, NULL);
                _Vdp_PipeConnect(prConnAdaptor,E_CONNECT_SRC_VDEC);
                prConnAdaptor->fgConnected = TRUE;
            }
            else if(prConnAdaptor->fgConnected && prConnAdaptor->ucFbgId != ucFbgId)
            {
               LOG(1,"[Pipe]VDP_PipeConnectFromVdec,Es%d,Vdp%d do Fbg change %d->%d\n",\
                  ucEsId,ucVdpId,prConnAdaptor->ucFbgId,ucFbgId);
                //_Vdp_PipeDisConnect(prConnAdaptor,E_CONNECT_SRC_VDEC);
                prConnAdaptor->ucFbgId = ucFbgId;
                prConnAdaptor->ucB2rId = FBM_B2rResIdAccess(ucFbgId, RES_R, NULL);
                _Vdp_PipeConnect(prConnAdaptor,E_CONNECT_SRC_VDEC);
           }
           else
           {
               prConnAdaptor->ucB2rId = FBM_B2rResIdAccess(ucFbgId, RES_R, NULL);
               prConnAdaptor->ucFbgId = ucFbgId;
           }
        }
        
    }
    
    PIPE_UNLOCK(_rPipeMutex);
    LOG(1,"[Pipe]VDP_PipeConnectFromVdec(%d,%d) done\n",ucEsId,ucFbgId);
    return;
}

void LG_pipLineScartConnect(UCHAR ucVdpId,UCHAR ucEsId)
{
    UCHAR ucOrgVdpId;
    B2R_HAL_OMUX_T tOmux = {0};
	B2R_OBJECT_T *this;
    //UCHAR i;//for test
    UCHAR ucB2rId;
	ucOrgVdpId=VDP_Es2Vdp(ucEsId);
	{
       x_memset(&_arVdpPipLineCfg,0x0,sizeof(_arVdpPipLineCfg));
	   x_memcpy(&_arVdpPipLineCfg,B2R_GetVdpConf(ucOrgVdpId),sizeof(_arVdpPipLineCfg));
	   _arVdpPipLineCfg.ucVdpId=ucVdpId;
	   if(B2R_GetVdpConf(ucVdpId) == NULL)
	   	{
	   	  LOG(0,"LG_pipLineScartConnect failed 1\n");
	   	  return;
	   	}
	   x_memcpy(B2R_GetVdpConf(ucVdpId),&_arVdpPipLineCfg,sizeof(_arVdpPipLineCfg));
	   tOmux.ucPath = ucVdpId;
	   tOmux.fgScartOut =FALSE;
	   ucB2rId=VDP_Vdp2B2rId(ucOrgVdpId);
	  
	 
	   this = _B2R_GetObj(ucOrgVdpId);
	   LOG(0,"Scart out VdpConnect(ucVdpId=%d,ucEsId=%d),ucOrgVdpId=%d,ucB2rId=%d\n",ucVdpId,ucEsId,ucOrgVdpId,ucB2rId);
	   if(this==NULL)
	   {
	       LOG(0,"LG_pipLineScartConnect failed 2\n");
	   	   return;
	   }
	   B2R_HAL_Set(this->hB2r, B2R_HAL_OMUX, &tOmux);
	   fgVdpModeChg[ucB2rId]= TRUE;
	   vMpegModeChg(ucVdpId);
       vMpegModeDetDone(ucVdpId);
   }
}

void  LG_PipLineVdpConnect(UCHAR ucVdpId,UCHAR ucEsId)
{
   UCHAR ucOrgVdpId;
    B2R_HAL_OMUX_T tOmux = {0};
	B2R_OBJECT_T *this;
    UCHAR i;//for test
    UCHAR ucB2rId;

	ucOrgVdpId=VDP_Es2Vdp(ucEsId);
    if((ucEsId != 0xff)&&(_prVdpCfg[ucVdpId]->ucInputPort[0]==6))    
	{        
		UCHAR ucFbgId;       
		ucFbgId=FBM_GetFbgByEs(ucEsId);    
		LOG(0,"Video path connect to Vdec  get Fbg id=%d\n",ucFbgId);
		if(ucFbgId != FBM_FBG_ID_UNKNOWN)
		{        
		   LOG(2,"LG_PipLineVdpConnect(%d,%d) connected to a runing vdec \n");           
		   ucB2rId= FBM_B2rResIdAccess(ucFbgId, RES_R, NULL); 
		   _VDP_PipeLineSwitch(ucVdpId,ucB2rId);  
		   LG_PipLine_VDP_SetEnable(ucVdpId,TRUE);
		   VDP_SetInput(ucVdpId,ucEsId,0);
           
		   FBM_SetFrameBufferFlag(ucFbgId, FBM_FLAG_SEQ_CHG);      
		   FBM_FbgChgNotify(ucFbgId, ucEsId);   
		   //fgLGPipeConnect= TRUE;
		}        
		else       
		{            
		   LOG(2,"LG_PipLineVdpConnect(%d,%d) connected to a stoped vdec \n");       
		}   
	}    
	vMpegHdConnect(ucVdpId,SV_ON);
	
	//if(ucVdpId!=ucOrgVdpId)
   {
       x_memset(&_arVdpPipLineCfg,0x0,sizeof(_arVdpPipLineCfg));
	   x_memcpy(&_arVdpPipLineCfg,B2R_GetVdpConf(ucOrgVdpId),sizeof(_arVdpPipLineCfg));
	   _arVdpPipLineCfg.ucVdpId=ucVdpId;
	   if(B2R_GetVdpConf(ucVdpId) == NULL)
	   {
	   	  LOG(0,"LG_PipLineVdpConnect failed 1\n");
	   	  return;
	   }
	   x_memcpy(B2R_GetVdpConf(ucVdpId),&_arVdpPipLineCfg,sizeof(_arVdpPipLineCfg));
	   tOmux.ucPath = ucVdpId;
	   tOmux.fgScartOut =FALSE;
	   ucB2rId=VDP_Vdp2B2rId(ucOrgVdpId);
	  
	 
	   this = _B2R_GetObj(ucB2rId);
	   LOG(0,"LG_PipLineVdpConnect(ucVdpId=%d,ucEsId=%d),ucOrgVdpId=%d,ucB2rId=%d\n",ucVdpId,ucEsId,ucOrgVdpId,ucB2rId);
	    if(this==NULL)
	   {
	       LOG(0,"LG_PipLineVdpConnect failed 2\n");
	   	   return;
	   }
	   B2R_HAL_Set(this->hB2r, B2R_HAL_OMUX, &tOmux);
	   fgVdpModeChg[ucB2rId]= TRUE;
	   vMpegModeChg(ucVdpId);
       vMpegModeDetDone(ucVdpId);
   }
	
	 for(i=0;i<VDP_NS;i++)
	 {
	    if(B2R_GetVdpConf(i)!=NULL)
           LOG(0,"total(%d,%d),vdpId=%d,framerate=%d\n",B2R_GetVdpConf(i)->rOutInfo.u4TotalHeight,B2R_GetVdpConf(i)->rOutInfo.u4TotalWidth,B2R_GetVdpConf(i)->ucVdpId,B2R_GetVdpConf(i)->u4FrameRate);
	 }
}

 void LG_PipLineTest(UCHAR ucVdpId,UCHAR ucEsId)
 {
    LG_PipLineVdpConnect(ucVdpId,ucEsId);
    LOG(0,"LG_PipLineTest(ucVdpId=%d,ucEsId=%d)\n",ucVdpId,ucEsId);
 }

UCHAR  LG_PipLineConnect(UCHAR ucVdpId, UCHAR ucB2rId)
{
   UCHAR i;

   if(!fgLGPipLine||ucB2rId>=B2R_NS)
   {
	   return B2R_NS;
   }
      LOG(0,"LG_PipLineConnect [%d][%d]\n", ucVdpId, ucB2rId);
	  fgVdpModeChg[ucB2rId]=FALSE;
	  LG_PipLine_VDP_SetEnable(ucVdpId,FALSE); 
	  _VDP_PipeLineSwitch(ucVdpId,B2R_NS);
	  LOG(0, "[%s][%d]--VS path0[%d],path1[%d]\n", __FUNCTION__, __LINE__,
		  bApiQuearyVSCConnectStatus(0), bApiQuearyVSCConnectStatus(1));

	  for(i=0;i<VDP_NS;i++)
	  {
	      #ifdef CC_SUPPORT_PIPELINE
		   LOG(0,"bApiQuearyVSCConnectStatus=%d\n",bApiQuearyVSCConnectStatus(i));
	       if(bApiQuearyVSCConnectStatus(i)==SV_VD_MPEGHD)
		   	{
				ucVdpId = i;
				fgVdpModeChg[ucB2rId]= TRUE;
				if(i == SV_VP_MAIN)
				{	
					bApiVFESetMainSubSrc(SV_VS_DTV1,SV_VS_NO_CHANGE);
				}
				else if (i == SV_VP_PIP)
				{
					bApiVFESetMainSubSrc(SV_VS_NO_CHANGE,SV_VS_DTV1);
				}
				break;
			}
		   #endif
	  }
      
	  LOG(0,"LG_PipLineSwitch(%d,%d)\n",ucVdpId,ucB2rId);
	  _VDP_PipeLineSwitch(ucVdpId,ucB2rId); 
      LG_PipLine_VDP_SetEnable(ucVdpId,TRUE); 
	  return ucVdpId;

}

void LG_PipLineDisconnect(UCHAR ucVdpId)
 {
      
     LOG(0,"LG_PipLineDisconnect\n");
	 B2R_MUTEX_UNLOCK(ucVdpId);
     LG_PipLine_VDP_SetEnable(ucVdpId,FALSE);
	 vMpegHdConnect(ucVdpId,SV_OFF);
	 _VDP_PipeLineSwitch(ucVdpId,B2R_NS); 
	 VDP_SetInput(ucVdpId,6,0);
	 LOG(0,"LG_PipLineDisconnect end.\n");
 }
#endif
void VDP_B2rSwitch(UCHAR ucVdpId, UCHAR ucB2rId)
{
    B2R_OBJECT_T *this;
    VDP_CFG_T* ptDP;

#ifdef CC_B2R_RES_SUPPORT
    UCHAR  ucRetB2rid = B2R_NS;    
#endif
#ifdef CC_SUPPORT_PIPELINE

    if(fgLGPipLine)
    {
       return;
    }
#endif
    // if ucB2rId = B2R_NS, it means that video plane switch to non-DTV video input
    #ifdef CC_B2R_RM_SUPPORT
    if(ucB2rId==B2R_NS)
     {
        LOG(0,"b2r_resource_release=%d\n",VDP_Vdp2B2rId(ucVdpId));
         b2r_resource_release(VDP_Vdp2B2rId(ucVdpId));
     }
    #endif
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    
#ifdef CC_B2R_RES_SUPPORT
    LOG(1, "Switch VdpId(%d) with B2rId(%d)!\n",ucVdpId, ucB2rId);
    
    /*check the id is used by the other vdp ,if used,find a free one reassigned and will correct in create fbg
          except scart out case, in scart out case, the b2rid will be the same!*/

    ucRetB2rid = _B2R_ChkHwConflict(ucVdpId,ucB2rId);
    if(ucRetB2rid != B2R_NS)
    {
        ucB2rId = ucRetB2rid;
    }
    else
    {
        LOG(1,"_B2R_ChkHwConflict return invalid b2rid(%d)!\n",ucRetB2rid);
    }
    
#endif

    ptDP = _prVdpCfg[ucVdpId];

    this = _B2R_RegDP(ucB2rId, ptDP);

    LOG(1, "VdpId(%d) switch to B2rId(%d)!\n",ucVdpId, ucB2rId);
#ifdef CC_SCPOS_EN
    vMpegInitB2rConf();
#endif
  
    _B2R_ProcDPs(this, ptDP);

}
UINT32 VDP_Vdp2B2rId(UCHAR ucVdpId)
{
    if(ucVdpId >= VDP_NS)
    {
        return B2R_NS;
    }
    
    if(_prVdpCfg[ucVdpId]!=0)
       return VDP2B2RID(ucVdpId);
    else
       return B2R_NS;
}

/**
 * VDP transfer VDP id to Es id
 *
 * @param specify the VDP id .
 * @return ucEsId (video decoder id)
 */
UINT32 VDP_Vdp2Es(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, MAX_ES_NS);
    return _prVdpCfg[ucVdpId]->ucInputPort[0];
}


VOID VDP_SetEnableLog(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    _B2R_SetEnableLog(VDP2B2RID(ucVdpId),(BOOL)u4Arg1,(BOOL)u4Arg2);
}

BOOL VDP_GstEnablGstLog(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, FALSE);
    return _B2R_GetEnableGstLog(VDP2B2RID(ucVdpId));
}

/**
 * VDP send test pattern (for module test)
 *
 * @param void
 * @return void
 */
void VDP_Pattern(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3)
{
    UNUSED(u4Arg2);
    UNUSED(u4Arg3);

    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));

    /* Change to B2R pattern directly */
    if (u4Arg1 == 99)
    {
        _B2R_StatusTest(VDP2B2RID(ucVdpId));
        return;
    }
    else if (u4Arg1 == 98)
    {
        _B2R_DisStatusTest(VDP2B2RID(ucVdpId));
        return;
    }

    //todo new pattern flow

}

/**
 * VDP set video plane with Pause MM feature.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucPause value 0:disable, 1:Pause MM feature enable.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetPauseMm(UINT8 ucVdpId, BOOL fgPause)
{
    UCHAR ucB2rId;

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);

    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);
    
    B2R_MUTEX_LOCK(ucB2rId);

    _B2R_SetPauseMM(VDP2B2RID(ucVdpId),fgPause);
    
    B2R_MUTEX_UNLOCK(ucB2rId);

    return VDP_SET_OK;
}
/**
 * VDP set b2r lock FB or not.
 *
 * @param ucVdpId specify the video plane id.
 * @param fgNoLock value 0:lock, 1: no lock
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 VDP_SetB2rNoLock(UINT8 ucVdpId, BOOL fgNoLock)
{
    UCHAR ucB2rId;
    
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);
    
    B2R_MUTEX_LOCK(ucB2rId);
    
    _B2R_SetB2rNoLock(VDP2B2RID(ucVdpId),fgNoLock);
    
    B2R_MUTEX_UNLOCK(ucB2rId);

    return VDP_SET_OK;
}

UINT32 VDP_SetSpeed(UINT8 ucVdpId, STC_SPEED_TYPE_T eSpeed)
{
    UCHAR ucB2rId;
    UINT32 u4Speed;
    BOOL fgDiv10 = 0;
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    switch (eSpeed)
    {
    case STC_SPEED_TYPE_FORWARD_2X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_2X;
        break;
    case STC_SPEED_TYPE_FORWARD_3X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_3X;
        break;
    case STC_SPEED_TYPE_FORWARD_4X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_4X;
        break;
    case STC_SPEED_TYPE_FORWARD_8X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_8X;
        break;
    case STC_SPEED_TYPE_FORWARD_16X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_16X;
        break;
    case STC_SPEED_TYPE_FORWARD_32X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_32X;
        break;
    case STC_SPEED_TYPE_FORWARD_64X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_64X;
        break;
    case STC_SPEED_TYPE_FORWARD_128X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_128X;
        break;
    case STC_SPEED_TYPE_FORWARD_256X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_256X;
        break;
    case STC_SPEED_TYPE_FORWARD_512X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_512X;
        break;
    case STC_SPEED_TYPE_FORWARD_1024X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1024X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_2X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_2X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_3X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_3X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_4X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_4X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_8X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_8X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_16X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_16X;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DIV_32X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DIV_32X;
        break;
    case STC_SPEED_TYPE_FORWARD_0_DOT_6X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_0_DOT_6X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_0_DOT_7X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_0_DOT_7X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_0_DOT_8X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_0_DOT_8X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_0_DOT_9X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_0_DOT_9X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_1X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_1X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_2X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_2X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_3X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_3X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_4X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_4X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1_DOT_5X:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1_DOT_5X;
        fgDiv10 = TRUE;
        break;
    case STC_SPEED_TYPE_FORWARD_1X:
    default:
        u4Speed =  (UINT32)B2R_SPEED_TYPE_FORWARD_1X;
        break;
    }

    
    B2R_MUTEX_LOCK(ucB2rId);

    LOG(0,"set speed(%d)!\n",u4Speed);
    _B2R_StcSpeed(VDP2B2RID(ucVdpId),u4Speed, fgDiv10);
    _B2R_FrcStcSpeed(VDP2B2RID(ucVdpId),eSpeed);

    B2R_MUTEX_UNLOCK(ucB2rId);

    return VDP_SET_OK;

}

void VDP_SetB2rStc(UINT8 ucVdpId,UINT32 u4B2rStc)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _B2R_SetB2rStc(VDP2B2RID(ucVdpId),u4B2rStc);
}
void B2R_EnalbleSmartPcr(UCHAR ucB2rId, BOOL fgEn)
{
    B2R_HalEnableSmartPcr(ucB2rId, fgEn);
}

UINT32 VDP_GetB2rStc(UINT8 ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), VDP_SET_ERROR);
    return _B2R_GetB2rStc(VDP2B2RID(ucVdpId));
}

UINT32 VDP_SetFrmCrop(UCHAR ucVdpId, UINT32 u4FrmCropTop, UINT32 u4FrmCropBtm)
{
    UCHAR ucB2rId;
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    _prVdpCfg[ucVdpId]->u4FrmCropTop = u4FrmCropTop;
    _prVdpCfg[ucVdpId]->u4FrmCropBtm = u4FrmCropBtm;

    B2R_MUTEX_UNLOCK(ucB2rId);

    return VDP_SET_OK;
}

UINT32 VDP_GetPts(UCHAR ucVdpId)
{
    UINT32 u4Pts;

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);

    u4Pts = _B2R_GetCurrentPts(VDP2B2RID(ucVdpId));

    return u4Pts;
}


UINT32 VDP_GetLockFrameBufferAddr(UCHAR ucVdpId, VDP_CAPTURE_INTO_T* prCapInfo)
{

    UCHAR ucEsId;
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucB2rId;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);
    
    ucEsId = VDP_Vdp2Es(ucVdpId);

    B2R_MUTEX_UNLOCK(ucB2rId);

    if (ucEsId >= MAX_ES_NS)
    {
        return VDP_SET_ERROR;
    }
    ucFbgId = FBM_GetFbgByEs(ucEsId);

    ucFbId = VDP_FrcGetLockFrameBuffer(ucVdpId);

    FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &(prCapInfo->u4AddrY), &(prCapInfo->u4AddrC));

    FBM_GetFrameBufferSize(ucFbgId, &(prCapInfo->u4MemHSize), &(prCapInfo->u4MemVSize));

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);

    if (prFbmSeqHdr == NULL)
    {
        return VDP_SET_ERROR;
    }
    else
    {
        /* Thumbnail usage: Image resizer will use this as pitch. */
        if (prFbmPicHdr && prFbmSeqHdr->fgResizeSmallPic)
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmPicHdr->u4PicWidthPitch;

            prCapInfo->u4HSize = (UINT32)prFbmPicHdr->u4PicWidth;
            prCapInfo->u4VSize = (UINT32)prFbmPicHdr->u4PicHeight;
        }
        else
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmSeqHdr->u2LineSize;

            prCapInfo->u4HSize = (UINT32)prFbmSeqHdr->u2OrgHSize;
            prCapInfo->u4VSize = (UINT32)prFbmSeqHdr->u2OrgVSize;
        }

        prCapInfo->u4RRMode = (UINT32)prFbmSeqHdr->fgRRYEn;
    }

    return VDP_SET_OK;

}


UINT32 VDP_GetDisplayFrameBufferAddr(UCHAR ucVdpId, VDP_CAPTURE_INTO_T* prCapInfo)
{
    UINT32 u4FbNum;
    UINT32 u4Idx;
    UCHAR ucEsId;
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucB2rId;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_PIC_HDR_T* prFbmPicHdr;

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    ucEsId = VDP_Vdp2Es(ucVdpId);

    B2R_MUTEX_UNLOCK(ucB2rId);

    if (ucEsId >= MAX_ES_NS)
    {
        return VDP_SET_ERROR;
    }
    ucFbgId = FBM_GetFbgByEs(ucEsId);
    ucFbId = 0xFF;
    u4FbNum = (UINT32)FBM_GetFrameBufferNs(ucFbgId);
    for(u4Idx = 0; u4Idx < u4FbNum; u4Idx++)
    {
        if(FBM_GetFrameBufferStatus(ucFbgId, u4Idx)
            == FBM_FB_STATUS_DISPLAYQ)  //already freeze at mpv_mw.c
        {
            ucFbId = (UCHAR)u4Idx;
            break;
        }
    }

    if (ucFbId == 0xFF)
    {
        return VDP_SET_ERROR;
    }

    FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &(prCapInfo->u4AddrY), &(prCapInfo->u4AddrC));

    FBM_GetFrameBufferSize(ucFbgId, &(prCapInfo->u4MemHSize), &(prCapInfo->u4MemVSize));

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);

    if (prFbmSeqHdr == NULL)
    {
        return VDP_SET_ERROR;
    }
    else
    {
        /* Thumbnail usage: Image resizer will use this as pitch. */
        if (prFbmPicHdr && prFbmSeqHdr->fgResizeSmallPic)
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmPicHdr->u4PicWidthPitch;

            prCapInfo->u4HSize = (UINT32)prFbmPicHdr->u4PicWidth;
            prCapInfo->u4VSize = (UINT32)prFbmPicHdr->u4PicHeight;
        }
        else
        {
            prCapInfo->u4MemHSize = (UINT32)prFbmSeqHdr->u2LineSize;

            prCapInfo->u4HSize = (UINT32)prFbmSeqHdr->u2OrgHSize;
            prCapInfo->u4VSize = (UINT32)prFbmSeqHdr->u2OrgVSize;
        }

        prCapInfo->u4RRMode = (UINT32)prFbmSeqHdr->fgRRYEn;
    }

    return VDP_SET_OK;

}


BOOL VDP_GetFilePosition(UCHAR ucVdpId,
                         INT32 *pi4TempRef, UINT16 *pu2DecodingOrder,
                         UINT64 *pu8Offset, UINT64 *pu8OffsetI)
{
    UINT32 u4Ret;
    VERIFY_VDP_ID(ucVdpId, FALSE);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FALSE);

    u4Ret = _B2R_GetFilePosition(VDP2B2RID(ucVdpId), pi4TempRef, pu2DecodingOrder,
                                 pu8Offset, pu8OffsetI);

    return (u4Ret == 0) ? FALSE : TRUE;
}

BOOL VDP_GetPosInfo(UINT8 ucVdpId, VDP_POS_INTO_T *prPosInfo)
{
    UINT32 u4Ret;
    VERIFY_VDP_ID(ucVdpId, FALSE);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FALSE);

    u4Ret = _B2R_GetPosInfo(VDP2B2RID(ucVdpId), prPosInfo);

    return (u4Ret == 0) ? FALSE : TRUE;
}

BOOL VDP_SetABInfo(UCHAR ucVdpId, VDP_AB_INTO_T *prAB)
{
    UCHAR ucB2rId;
    UINT32 u4Ret;
    VERIFY_VDP_ID(ucVdpId, FALSE);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    u4Ret = _B2R_SetABInfo(VDP2B2RID(ucVdpId), prAB);

    B2R_MUTEX_UNLOCK(ucB2rId);

    return (u4Ret == 0) ? FALSE : TRUE;
}

UCHAR VDP_GetPendingFB(UCHAR ucVdpId, UCHAR ucFbgId)
{
    BOOL fgSubFbg = FALSE;

    VERIFY_VDP_ID(ucVdpId, FBM_FB_ID_UNKNOWN);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FBM_FB_ID_UNKNOWN);

    if (_prB2rPrm[VDP2B2RID(ucVdpId)]->ucFbgId == ucFbgId)
    {
        fgSubFbg = FALSE;
    }
#ifdef CC_3D_MM_DS_SUPPORT
    else if (_prB2rPrm[VDP2B2RID(ucVdpId)]->ucSubFbgId == ucFbgId)
    {
        fgSubFbg = TRUE;
    }
#endif
    else
    {
        return FBM_FB_ID_UNKNOWN;
    }

    return _B2R_GetPendingFB(VDP2B2RID(ucVdpId), fgSubFbg);
}

void VDP_CleanPendingFB(UCHAR ucVdpId, UCHAR ucFbgId)
{
    BOOL fgSubFbg = FALSE;

    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));

    if (_prB2rPrm[VDP2B2RID(ucVdpId)]->ucFbgId == ucFbgId)
    {
        fgSubFbg = FALSE;
    }
#ifdef CC_3D_MM_DS_SUPPORT
    else if (_prB2rPrm[VDP2B2RID(ucVdpId)]->ucSubFbgId == ucFbgId)
    {
        fgSubFbg = TRUE;
    }
#endif
    else
    {
        return;
    }

    _B2R_CleanPendingFB(VDP2B2RID(ucVdpId), fgSubFbg);
}

void VDP_SetReleaseDispQ(UCHAR ucVdpId, UCHAR ucFbgId)
{
    BOOL fgSubFbg = FALSE;

    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    
    if (_prB2rPrm[VDP2B2RID(ucVdpId)]->ucFbgId == ucFbgId)
    {
        fgSubFbg = FALSE;
    }
#ifdef CC_3D_MM_DS_SUPPORT
    else if (_prB2rPrm[VDP2B2RID(ucVdpId)]->ucSubFbgId == ucFbgId)
    {
        fgSubFbg = TRUE;
    }
#endif
    else
    {
        return;
    }

    _B2R_SetReleaseDispQ(VDP2B2RID(ucVdpId), fgSubFbg);
}


void VDP_VsyncMutexLock(UCHAR ucVdpId)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _B2R_VsyncMutexLock(VDP2B2RID(ucVdpId));
}

void VDP_VsyncMutexUnlock(UCHAR ucVdpId)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _B2R_VsyncMutexUnlock(VDP2B2RID(ucVdpId));
}

/* Trick Play at 20081001 */
UINT32 VDP_SetTrickMode(UINT8 ucVdpId,UCHAR ucTrick)
{
    UCHAR ucB2rId;
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);
    
    B2R_MUTEX_LOCK(ucB2rId);

    _B2R_SetTrickMode(ucB2rId,ucTrick);

    B2R_MUTEX_UNLOCK(ucB2rId);

    return VDP_SET_OK;
}

/* Step Forward at 20081030 */
UINT32 VDP_SetStepForward(UINT8 ucVdpId, UCHAR ucSet)
{
    UCHAR ucB2rId;
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    _B2R_SetStepForward(VDP2B2RID(ucVdpId),ucSet);

    B2R_MUTEX_UNLOCK(ucB2rId);

    return VDP_SET_OK;
}

/* Step Forward at 20081030 */
UCHAR VDP_GetStepForward(UINT8 ucVdpId)
{
    UCHAR ucResult;
    UCHAR ucB2rId;
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    ucResult = _B2R_GetStepForward(ucB2rId);

    B2R_MUTEX_UNLOCK(ucB2rId);

    return ucResult;
}

/* Slow Forward at 20090506 */
UINT32 VDP_SetSlowForward(UINT8 ucVdpId, UCHAR ucSet)
{
    UCHAR ucB2rId;
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    _B2R_SetSlowForward(ucB2rId,ucSet);

    B2R_MUTEX_UNLOCK(ucB2rId);

    return VDP_SET_OK;
}

/* Slow Forward at 20090506 */
UCHAR VDP_GetSlowForward(UINT8 ucVdpId)
{
    UCHAR ucResult;
    UCHAR ucB2rId;

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    ucResult = _B2R_GetSlowForward(ucB2rId);

    B2R_MUTEX_UNLOCK(ucB2rId);

    return ucResult;
}
UINT32 VDP_SetFBNoTimeout(UINT8 ucVdpId, UCHAR ucSet)
{
    UCHAR ucEsId;
    UCHAR ucFbgId;
    UCHAR ucB2rId;

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    ucEsId = VDP_Vdp2Es(ucVdpId);
    
    B2R_MUTEX_UNLOCK(ucB2rId);

    if (ucEsId >= MAX_ES_NS)
    {
        return VDP_SET_ERROR;
    }
    ucFbgId = FBM_GetFbgByEs(ucEsId);

    if (ucSet != 0)
    {
        FBM_SetFrameBufferGlobalFlag(ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);
    }
    else
    {
        FBM_ClrFrameBufferGlobalFlag(ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);
    }

    return VDP_SET_OK;

}


/* VDP trigger mode + audio master to do trigger mode */
void VDP_TriggerAudReceive(UINT8 ucVdpId, UINT32 u4Pts)
{
    _B2R_TriggerAudReceive(ucVdpId, u4Pts);
}

/** Disable VDP Trigger action */
BOOL VDP_TriggerAudReceiveEx(UINT8 ucVdpId, BOOL fgEnable, UINT32 u4Pts)
{
    VERIFY_VDP_ID(ucVdpId, FALSE);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FALSE);
    return _B2R_TriggerAudReceiveEx(VDP2B2RID(ucVdpId), fgEnable, u4Pts);
}

/* whether Update STC when VDP start output or not */
BOOL VDP_SetPendingForStc(UINT8 ucVdpId, BOOL fgPending)
{
    VERIFY_VDP_ID(ucVdpId, FALSE);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FALSE);
    return _B2R_SetPendingForStc(VDP2B2RID(ucVdpId), fgPending);
}

/* VDP Seek done notification */
void VDP_SetSeek(UCHAR ucVdpId, UCHAR ucSet, UINT32 u4Pts)
{
    UCHAR ucB2rId;
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(ucB2rId);
    
    B2R_MUTEX_LOCK(ucB2rId);

    LOG(0,"Set seek!\n");
    _B2R_SetSeek(ucB2rId, ucSet, u4Pts);

    B2R_MUTEX_UNLOCK(ucB2rId);
}
void VDP_SetSeekMode(UCHAR ucVdpId, UCHAR ucSet, UINT32 u4Pts)
{
    UCHAR ucB2rId;
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(ucB2rId);
    
    B2R_MUTEX_LOCK(ucB2rId);

    LOG(0,"Set seek ucB2rId=%d,u4Pts=%x\n",ucB2rId,u4Pts);
    _B2R_SetSeekMode(ucB2rId, ucSet, u4Pts);

    B2R_MUTEX_UNLOCK(ucB2rId);
}

/* VDP AB repeat notification */
void VDP_SetABRepeat(UCHAR ucVdpId, UCHAR ucSet)
{
    UCHAR ucB2rId;
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(ucB2rId);
    
    B2R_MUTEX_LOCK(ucB2rId);

    _B2R_SetABRepeat(ucB2rId, ucSet);

    B2R_MUTEX_UNLOCK(ucB2rId);
}

void VDP_GetYCStart(UCHAR ucVdpId,UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    _B2R_GetYCStart(VDP2B2RID(ucVdpId),pu4YStart, pu4CStart, pu4LineSize, pu4VActive);
}

void VDP_SetB2RMirror(UCHAR ucVdpId, BOOL fgOn)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    _B2R_SetMirrorStatus(VDP2B2RID(ucVdpId), fgOn);
}

UCHAR VDP_GetPlayMode(UCHAR ucVdpId)
{
    UCHAR ucPlayMode;
    UCHAR ucEsId;
    UCHAR ucFbgId;
    UCHAR ucB2rId;

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    ucEsId = VDP_Vdp2Es(ucVdpId);

    B2R_MUTEX_UNLOCK(ucB2rId);

    if (ucEsId >= MAX_ES_NS)
    {
        return VDP_SET_ERROR;
    }
    ucFbgId = FBM_GetFbgByEs(ucEsId);

    FBM_GetPlayMode(ucFbgId, &ucPlayMode);

    return ucPlayMode;
}

UCHAR VDP_SetThumbnailMode(UCHAR ucVdpId, UCHAR ucSet)
{
    UCHAR ucB2rId;
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);

    B2R_MUTEX_LOCK(ucB2rId);

    if (_aucThumbnailMode[ucVdpId] != ucSet)
    {
        _aucThumbnailMode[ucVdpId] = ucSet;
        LOG(5, "Thumbnail Mode (%d)\n", _aucThumbnailMode[ucVdpId]);
    }

    B2R_MUTEX_UNLOCK(ucB2rId);

    return VDP_SET_OK;
}

void B2R_SetImgConnect(UCHAR ucVdpId, UCHAR ucConnect)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _aucImageConnected[ucVdpId] = ucConnect;
}

UCHAR B2R_GetImgConnect(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    return _aucImageConnected[ucVdpId];
}

void VDP_VsyncSendCmd(UCHAR ucVdpId, UCHAR ucCmdId)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    _B2R_VsyncSendCmd(VDP2B2RID(ucVdpId), ucCmdId);
}

void VDP_VsyncReset(UCHAR ucVdpId)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _B2R_VsyncReset(ucVdpId);
    _B2R_VdpCfgReset(ucVdpId);
}

void VDP_FrcSetPtsCb(UINT32 u4CbArg, UINT32 u4CbPts)
{
    _B2R_FrcSetPtsCb(u4CbArg, u4CbPts);
}

void VDP_FrcSetLipSyncCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4Thrsd, UINT32 u4FrmCnt)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    _B2R_FrcSetLipSyncCb(VDP2B2RID(ucVdpId), ucPort, u4Thrsd, u4FrmCnt);
}

void VDP_FrcSetLipSyncNGOKCb(UCHAR ucNgOk, UCHAR ucRemove, UINT32 u4Thrsd, UINT32 u4FrmCnt)
{
    _B2R_FrcSetLipSyncNGOKCb(ucNgOk, ucRemove, u4Thrsd, u4FrmCnt);
}

UINT32 VDP_TrickModeSkipFrameNo(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), VDP_SET_ERROR);
    return _B2R_TrickModeSkipFrameNo(VDP2B2RID(ucVdpId));
}

UINT32 VDP_GetFrameRate(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, 30);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), 30);

    if(_prB2rPrm[VDP2B2RID(ucVdpId)])
    {
        return _prB2rPrm[VDP2B2RID(ucVdpId)]->ucInFrameRate;
    }
    return 30;
}

//void VDP_SetCb
void VDP_SetCbAttachedSrcId(UCHAR ucVdpId, UINT32 u4AttachId)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    if(_prB2rPrm[VDP2B2RID(ucVdpId)])
    {
        _prB2rPrm[VDP2B2RID(ucVdpId)]->u4AttachedSrcId = u4AttachId;
    }
}

UINT32 VDP_Set3DInfo(UCHAR ucVdpId,UINT8 u13DType,BOOL fg3DEnable)
{
    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), VDP_SET_ERROR);
    _B2R_Set3DInfo(VDP2B2RID(ucVdpId),u13DType,fg3DEnable);

    return VDP_SET_OK;
}

BOOL VDP_IsStartToPlay(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, FALSE);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FALSE);
    return _B2R_IsStartToPlay(VDP2B2RID(ucVdpId));
}

BOOL VDP_SendB2RAysncRenderFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg)
{
    if(prMsg)
    {
        return _B2R_SendB2RAysncRenderFrameMsg(prMsg);
    }
    return FALSE;
}

BOOL VDP_FlushB2RAysyncRenderFrameMsg(UCHAR ucFbgId)
{  
    UCHAR ucB2rId = 0;
    ucB2rId=_FBM_Fbg2B2r(ucFbgId);

    if(ucB2rId >= B2R_NS)
    {
        LOG(0,"VDP_FlushB2RAysyncRenderFrameMsg fbg %d not match a b2r id\n",ucFbgId);
        return FALSE;
    }
    return _B2R_FlushB2RChgFrameMsg(ucB2rId);
}

VOID VDP_DropB2RAysyncRenderFrame(UCHAR ucFbgId)
{  
    UINT32 u1DispFbId;
    u1DispFbId = FBM_GetFrameBufferFromDispQ(ucFbgId);
    if(u1DispFbId != 0xff )
    {
        FBM_SetFrameBufferStatus(ucFbgId, u1DispFbId, FBM_FB_STATUS_LOCK);
        FBM_SetFrameBufferStatus(ucFbgId, u1DispFbId, FBM_FB_STATUS_EMPTY);
        LOG(1,"VDP_DropB2RAysyncRenderFrame,fbg=%d,fb=%d\n",ucFbgId,u1DispFbId);
    }
    else
    {
        LOG(1,"VDP_DropB2RAysyncRenderFrame,fbg=%d no frame drop\n",ucFbgId);
    }
    
    return;
}

BOOL VDP_SendB2RChgFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg)
{
    if(prMsg == NULL)
    {
        return FALSE;
    }
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    return _B2R_SendB2RChgFrameMsg(prMsg);
#else
    return TRUE;
#endif
}

BOOL VDP_FlushB2RChgFrameMsg(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, FALSE);
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    return _B2R_FlushB2RChgFrameMsg(VDP2B2RID(ucVdpId));
#else
    return TRUE;
#endif
}

BOOL VDP_GetB2R2DSupport(UINT32 u4Width,UINT32 u4Height)
{
    if (u4Width > B2R_MAX_WIDTH)
    {
        return FALSE;
    }
    if (u4Height > B2R_MAX_HEIGHT)
    {
        return FALSE;   // Current IC, the VTotal/VActive is 11bits(at most 2048). Will be improved after 5860/5880.
    }
    return TRUE;
}

// Get target resize size of seamless mode
// Return TRUE: it is seamless play, and resize width/height is available.
// Return FALSE: it is not seamless play.
BOOL VDP_GetSeamlessResizeSize(UCHAR ucVdpId, UINT32 *pu4ResizeWidth, UINT32 *pu4ResizeHeight)
{
    FBM_SEQ_HDR_T *prFbmSeqHdr = NULL;

    VERIFY_VDP_ID(ucVdpId, FALSE);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FALSE);

    if (pu4ResizeWidth == NULL || pu4ResizeHeight == NULL)
    {
        return FALSE;
    }
    
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_prB2rPrm[VDP2B2RID(ucVdpId)]->ucFbgId);
    if (prFbmSeqHdr != NULL)
    {
        if (prFbmSeqHdr->fgSeamlessPlay && (prFbmSeqHdr->u4ResizeWidth != 0) && (prFbmSeqHdr->u4ResizeHeight != 0))
        {
            *pu4ResizeWidth = prFbmSeqHdr->u4ResizeWidth;
            *pu4ResizeHeight = prFbmSeqHdr->u4ResizeHeight;
            return TRUE;
        }
    }    

    return FALSE;
}

UCHAR VDP_GetSeamlessInfo(UCHAR ucVdpId, VDP_SEAMLESS_INFO_T* prSeamlessInfo)
{
#ifdef ENABLE_MULTIMEDIA
    UCHAR ucB2rId;
    UCHAR ucRet = VDP_SET_OK;
    UINT32 u4SeqWidth;
    UINT32 u4SeqHeight;
    VDP_CFG_T* prVdpConf = NULL;
    B2R_PRM_T* ptVdpPrm;
    B2R_OBJECT_T *this;
    FBM_SEQ_HDR_T* prSeqHdr;

    do
    {
        if(ucVdpId >= VDP_NS || !prSeamlessInfo)
        {
            ucRet = VDP_SET_ERROR;
            break;
        }

        prVdpConf = _B2R_GetVdpConf(ucVdpId);
        if(!prVdpConf)
        {
            ucRet = VDP_SET_ERROR;
            break;
        }
        ucB2rId = prVdpConf->ucB2rId;
        if(ucB2rId >= B2R_NS)
        {
            ucRet = VDP_SET_ERROR;
            break;
        }

        this = _B2R_GetObj(ucB2rId);
        if(!this)
        {
            ucRet = VDP_SET_ERROR;
            break;
        }

        ptVdpPrm = this->ptB2rPrm;
        if(!ptVdpPrm)
        {
            ucRet = VDP_SET_ERROR;
            break;
        }
        
        prSeqHdr = FBM_GetFrameBufferSeqHdr(ptVdpPrm->ucFbgId);
        if (!prSeqHdr)
        {
            ucRet = VDP_SET_ERROR;
            break;
        }

        if (prSeqHdr->eSeamlessMode & SEAMLESS_BY_NPTV)
        {
            u4SeqWidth  = prSeqHdr->u2HSize;
            u4SeqHeight = prSeqHdr->u2VSize;
        }
        else
        {
            ucRet = VDP_SET_ERROR;
            break;
        }

        if (prSeqHdr->fgProgressiveSeq != 0)
        {
            u4SeqHeight &= 0xFFFFFFFE;
        }
        else
        {
            u4SeqHeight &= 0xFFFFFFFC;
        }
    
#ifdef CC_B2R_3D_SUPPROT
        if (prSeqHdr->fgB2R3DEnable)
        {
            LOG(1, "[%s-%d]3D Not support NPTV SeamlessPlay Yet.\n", __func__, __LINE__);
            ucRet = VDP_SET_ERROR;
            break;
        }
#endif
        prSeamlessInfo->u4SrcWidth = ((u4SeqWidth > B2R_MAX_WIDTH) ? B2R_MAX_WIDTH : u4SeqWidth);
        prSeamlessInfo->u4SrcHeight = ((u4SeqHeight > B2R_MAX_HEIGHT) ? B2R_MAX_HEIGHT : u4SeqHeight);

        _B2R_Height_Align(&prSeamlessInfo->u4SrcHeight);

        LOG(2, "[%s-%d]W,H=(%d, %d).\n", __func__, __LINE__, prSeamlessInfo->u4SrcWidth, prSeamlessInfo->u4SrcHeight);
    }
    while(0);

    return ucRet;
#endif
    return VDP_SET_ERROR;
}

void VDP_NfySetResolutionReady(UCHAR ucVdpId, BOOL fgReady)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);

    // TBD
}

UINT32 VDP_GetDispBufLocked(UCHAR ucVdpId, UINT32 u4BufAddr, BOOL *pfgLocked)
{
    UCHAR ucB2rId;
    VDP_CFG_T* prVdpConf;
    
    prVdpConf = _B2R_GetVdpConf(ucVdpId);
    if(!prVdpConf)
    {
        return B2R_INV_ARG;
    }
    
    ucB2rId = prVdpConf->ucB2rId;
    return _B2R_GetDispBufLocked(ucB2rId, u4BufAddr, pfgLocked);
}

void VDP_ResetPts(UCHAR ucVdpId)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    _B2R_ResetPts(VDP2B2RID(ucVdpId));
}

void VDP_FrcResetPort(UCHAR ucVdpId, UCHAR ucPort)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    _B2R_FrcResetPort(VDP2B2RID(ucVdpId), ucPort);
}

void VDP_FrcSetFreeze(UCHAR ucVdpId, UINT32 u4OnOff)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    VERIFY_B2R_ID_RET_VOID(VDP2B2RID(ucVdpId));
    _B2R_FrcSetFreeze(VDP2B2RID(ucVdpId), u4OnOff);
}

UCHAR VDP_FrcGetLockFrameBuffer(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, FBM_FB_ID_UNKNOWN);
    return _B2R_FrcGetLockFrameBuffer(VDP2B2RID(ucVdpId));
}


UCHAR VDP_GetOutFrameRate(UCHAR ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, FBM_FRAME_RATE_UNKNOWN);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FBM_FRAME_RATE_UNKNOWN);
    return _B2R_GetOutFrameRate(VDP2B2RID(ucVdpId));
}

VOID VDP_SetDropFrame(UCHAR ucVdpId)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _B2R_SetDropFrame(VDP2B2RID(ucVdpId));
}

VOID VDP_SetRepeatFrame(UCHAR ucVdpId)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _B2R_SetRepeatFrame(VDP2B2RID(ucVdpId));
}

//functions with b2r ID
UINT32 VDP_GetCurrentPts(UCHAR ucB2rId)
{
    return _B2R_GetCurrentPts(ucB2rId);
}

INT32 VDP_GetPtsStcDelta(UCHAR ucB2rId)
{
    return _B2R_GetPtsStcDelta(ucB2rId);
}

VOID VDP_SetTimeShiftSpeed(UINT8 u1B2rId,UINT32 u4SpeedValue)
{
    _B2R_SetTimeShiftSpeed(u1B2rId, u4SpeedValue);
}

VOID VDP_SetTimeShiftMode(UINT8 u1B2rId,BOOL fgEnable)
{
    _B2R_SetTimeShiftMode(u1B2rId, fgEnable);
}

BOOL VDP_GetTimeShfitStartPlay(UCHAR ucB2rId)
{
    return _B2R_GetTimeShfitStartPlay(ucB2rId);
}

UCHAR VDP_GetFbg(UCHAR ucB2rId)
{
    return _B2R_GetFbg(ucB2rId);
}

B2R_PRM_T* VDP_GetVdpPrm(UINT8 u1B2rId)
{
    return _B2R_GetB2rPrm(u1B2rId);
}
void VDP_PrintDtvInfo(void)
{
    UINT32 u4VdpId;

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        if (_prVdpCfg[u4VdpId]->ucB2rId < B2R_NS)
        {
            LOG(3, "DTV INFO RESOLUTION(%d, %d) Aspect Ratio(%d) AFD(%d)\n",
                _prVdpCfg[u4VdpId]->u4SrcWidth,
                _prVdpCfg[u4VdpId]->u4SrcHeight,
                _prVdpCfg[u4VdpId]->u4AspectRatio,
                _prVdpCfg[u4VdpId]->u4Afd);
        }
    }
}

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
void B2R_SetChapterEnable(VDP_CHAPTER_T * prChapter)
{
    _B2R_FrcSetChapterEnable(prChapter);
}
#endif
#endif

#ifdef CC_FAST_INIT
void VDP_pm_suspend(void)
{
}


void VDP_pm_resume(void)
{
    BIM_EnableIrq(VECTOR_DISPLAY);
}
#endif

#ifdef CC_SUPPORT_STR

void B2R_pm_str_suspend(void)
{
	B2R_CkgenOnOff(FALSE);
}

void B2R_pm_str_resume(void)
{
	B2R_CkgenOnOff(TRUE);
}

#endif


/* B2R CRC check on PTS value */
void VDP_SetChkPTS(UINT32 u4Pts)
{
    UNUSED(u4Pts);
#ifdef VDP_B2R_PSCAN_CRC_FLOW
    _VDPSetChkPTS(u4Pts);
#endif
}

void VDP_SetEncTrigger(BOOL fgEnable)
{
    UNUSED(fgEnable);
#ifdef VDP_B2R_PSCAN_CRC_FLOW
    _VDP_SetEncTrigger(fgEnable);
#endif
}

/**
 * VDP transfer Stc id to B2R id
 *
 * @param ucStcId specify the STC id.
 * @return B2R id that reference to the STC
 */
UCHAR VDP_Stc2B2r(UCHAR ucStcId)
{
    UCHAR ucB2rId;
    UCHAR ucFbgSyncMode;
    UCHAR ucFbgStcId;
    UCHAR ucFbgId;

    for (ucB2rId = B2R_1; ucB2rId < B2R_NS; ucB2rId++)
    {
        B2R_MUTEX_LOCK(ucB2rId);
        ucFbgId = _prB2rPrm[ucB2rId]->ucFbgId;
        B2R_MUTEX_UNLOCK(ucB2rId);
        if (ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            FBM_GetSyncStc(ucFbgId, &ucFbgSyncMode, &ucFbgStcId);
            if (ucFbgStcId != STC_SRC_NS && ucFbgStcId == ucStcId)
            {
                return ucB2rId;
            }
        }
    }

    return B2R_NS;
}



BOOL B2R_CkgenOnOff(BOOL fgEnable)
{
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389) 
#define BLOCK_RST_CFG0 0x270
#define B2R_RST ((UINT32)1 << 1)
#define BLOCK_CKEN_CFG0 0x278
#define B2R_CKEN ((UINT32)1 << 1)
#else
#define BLOCK_RST_CFG0 0x1C0
#define B2R_RST ((UINT32)1 << 7)
#define BLOCK_CKEN_CFG0 0x1C8
#define B2R_CKEN ((UINT32)1 << 7)
#endif

    if (fgEnable)
    {
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~B2R_RST);
        IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) | B2R_CKEN);
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) | B2R_RST);
        //VDP_ForceResetClk(fgEnable);
    }
    else
    {
        IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~B2R_RST);
        IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) & ~B2R_CKEN);
    }

    UNUSED(fgEnable);
    return TRUE;
}

//vdp image interface, will be removed

/**
 * Video Image Module : Get information
 *
 * @param void
 * @return pointer of Video Image Module Capability Infomation
 */
VDP_IMAGE_INFO_T* VDP_Image_GetInfo(void)
{
    return _VDP_Image_GetInfo();
}

/**
 * Video Image Module : Connect
 *
 * @param u4VdpId: Video plane connected to
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_Connect(UINT32 u4VdpId)
{
    UINT32 u4ReturnValue;
    UINT32 u4Idx;
    UCHAR ucVdpId;
    UCHAR ucB2rId;

    ucVdpId = (UCHAR)u4VdpId;

    VERIFY_VDP_ID(ucVdpId, VDP_SET_ERROR);
    ucB2rId = VDP2B2RID(ucVdpId);
    VERIFY_B2R_ID(ucB2rId, VDP_SET_ERROR);
    
    B2R_MUTEX_LOCK(ucB2rId);

    for (u4Idx = 0; u4Idx < VDP_NS; u4Idx++)
    {
        _aucImageConnected[u4Idx] = 0;
    }

    B2R_MUTEX_UNLOCK(ucB2rId);
    
    u4ReturnValue = _VDP_Image_Connect(ucVdpId);

    B2R_MUTEX_LOCK(ucB2rId);
    
    if (u4ReturnValue == VDP_SET_OK)
    {
        _aucImageConnected[ucVdpId] = 1;
    }

    B2R_MUTEX_UNLOCK(ucB2rId);

    return u4ReturnValue;
}

/**
 * Video Image Module : Disconnect
 *
 * @param void
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_Disconnect(void)
{
    UINT32 u4VdpId;

    for (u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        _aucImageConnected[u4VdpId] = 0;
    }

    return _VDP_Image_Disconnect();
}

/**
 * Video Image Module : Mute on/off
 *
 * @param u4OnOff: Mute on/off
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_Mute(UINT32 u4OnOff)
{
    return _VDP_Image_Mute(u4OnOff);
}

/**
 * Video Image Module : Show Frame Buffer
 *
 * @param prInfo: Frame Buffer Info
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_Show(VDP_IMAGE_ARG_T* prInfo)
{
    return _VDP_Image_Show(prInfo);
}

/**
 * Video Image Module : Get Frame Buffer Address
 *
 * @param u4FrameBufferId: Frame Buffer id
 * @param u4YAddr: pointer to store Y address
 * @param u4CAddr: pointer to store C address
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
UINT32 VDP_Image_GetFrameBuffer(UINT32 u4FrameBufferId, UINT32* u4YAddr, UINT32* u4CAddr)
{
    return _VDP_Image_GetFrameBuffer(u4FrameBufferId, u4YAddr, u4CAddr);
}

/**
 * VDP connect to video decoder.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucEsId specify video decoder id.
 * @param ucPort always 0.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */


void B2R_SetB2rUpdateStc(UINT8 ucVdpId,BOOL fgEnable)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _B2R_EnB2rUpdateStc(VDP2B2RID(ucVdpId),fgEnable);
}

BOOL B2R_GetB2rUpdateStc(UINT8 ucVdpId)
{
    VERIFY_VDP_ID(ucVdpId, FALSE);
    VERIFY_B2R_ID(VDP2B2RID(ucVdpId), FALSE);
    return _B2R_GetUpdateStc(VDP2B2RID(ucVdpId));
}

VOID B2R_Init(VOID)
{
    UCHAR i;
    
    for (i=0; i<B2R_NS; i++)
    {
       
        _prB2rPrm[i] = _B2R_GetB2rPrm(i);
    }

    for (i=0; i<VDP_NS; i++)
    {   
        _aucThumbnailMode[i] = 0;
        _prVdpCfg[i] = _B2R_GetVdpConf(i);
        _prVdpCfg[i]->ucVdpId = i;
        x_memset(_prVdpCfg[i]->ucInputPort, VDEC_MAX_ES, sizeof(_prVdpCfg[i]->ucInputPort));
    }
	#ifdef CC_SUPPORT_PIPELINE
    fgLGPipLine=TRUE;
    _Vdp_PipInit();
	#endif
    FBM_RegCbFunc(FBM_CB_FUNC_FBG_CHG_IND, (UINT32)_VdpFbgChgNotify);

    //register the B2R timegen
#ifdef CC_MT5890
    fpDrvRegisterDisplayTimingUpdateCB(E_TIMEGEN_PATH_BYPASS_MM, E_DISPLAY_TIMING_CB_H_ACTIVE, VDP_SetBypassHactive);
    fpDrvRegisterDisplayTimingUpdateCB(E_TIMEGEN_PATH_BYPASS_MM, E_DISPLAY_TIMING_CB_H_TOTAL, VDP_SetBypassHTotal);
    fpDrvRegisterDisplayTimingUpdateCB(E_TIMEGEN_PATH_BYPASS_MM, E_DISPLAY_TIMING_CB_H_FP, VDP_SetBypassHFph);
    fpDrvRegisterDisplayTimingUpdateCB(E_TIMEGEN_PATH_BYPASS_MM, E_DISPLAY_TIMING_CB_H_BP, VDP_SetBypassHBph);
    fpDrvRegisterDisplayTimingUpdateCB(E_TIMEGEN_PATH_BYPASS_MM, E_DISPLAY_TIMING_CB_V_ACTIVE, VDP_SetBypassVactive);
    fpDrvRegisterDisplayTimingUpdateCB(E_TIMEGEN_PATH_BYPASS_MM, E_DISPLAY_TIMING_CB_V_TOTAL, VDP_SetBypassVTotal);
    fpDrvRegisterDisplayTimingUpdateCB(E_TIMEGEN_PATH_BYPASS_MM, E_DISPLAY_TIMING_CB_V_FP, VDP_SetBypassVFph);
    fpDrvRegisterDisplayTimingUpdateCB(E_TIMEGEN_PATH_BYPASS_MM, E_DISPLAY_TIMING_CB_V_BP, VDP_SetBypassVBph);
#endif
    
    _B2R_ObjectInit();
    _B2R_VsyncInit();
   #ifdef CC_B2R_RM_SUPPORT
     B2R_status_rest();
   #endif
    _VDP_Image_Init();
}

#ifdef CC_MAPLE_CUST_DRV
INT32 B2R_Get_Cust_info (UCHAR                 ucB2rId,
                              B2R_GET_CUST_TYPE_T   e_set_type,
                              VOID*                 pv_get_info,
                              SIZE_T                z_set_info_size)
{
    INT32   i4Ret = B2R_OK;

    do
    {
        if(ucB2rId >= B2R_NS || 
            !pv_get_info)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        switch(e_set_type)
        {
            case B2R_GET_TYPE_CUST_INFO:
            {
                B2R_CUST_INFO_T*    pt_cust_info = (B2R_CUST_INFO_T*)pv_get_info;
 
                if(sizeof(B2R_CUST_INFO_T) != z_set_info_size)
                {
                    i4Ret = B2R_INV_ARG;
                    break;
                }
                i4Ret = _B2R_Get_Cust_info(ucB2rId,pt_cust_info);
                break;
            }

            case B2R_GET_TYPE_ASPINFO:
            {
                UINT8*    pu1_asp_info = (UINT8*)pv_get_info;
                if(sizeof(UINT8) != z_set_info_size)
                {
                    i4Ret = B2R_INV_ARG;
                    break;
                }
                i4Ret = _B2R_GetAspInfo(ucB2rId,pu1_asp_info);
                break;
            }

            default:
                i4Ret = B2R_NOT_IMPL;
                break;
        }
    }while(0);

    if(i4Ret != B2R_OK)
    {
            LOG(0,"%s Failed , i4Ret : %d!\n",__FUNCTION__,i4Ret);
    }

    return i4Ret;
}

#endif
#ifdef CC_B2R_RM_SUPPORT
VOID B2R_RegRmCb(PFN_B2R_RM_CB pfnRmCb)
{
    pfB2R_RM_Cb=pfnRmCb;
}
VOID B2R_RmSetKickDone(UCHAR ucvdpId, BOOL fgSuccess)
{
    if((B2R_RM.u2ConflictVdpid==ucvdpId)&&fgSuccess)
    {
        B2R_RM.fgconflict=fgSuccess;
    }
    #ifdef CC_VDEC_RM_SUPPORT
    _VDEC_RMKickRet(_prVdpCfg[ucvdpId]->ucInputPort[0], VDEC_RM_BLK_B2R_DONE);
    #endif
}

void B2R_Send_Error_Notify(B2R_RM_COND_T eCond)
{
  UINT16 u2ConflictVdp;
  UINT16 i;


  for(i=0;i<B2R_NS;i++)
  {
      if(i<MaxB2RNum)
      {
          if( B2R_RM.OldB2R_RM[i].u2CapabilityLvl==No_Capability)
          {
              u2ConflictVdp = _B2R_GetVdpId(i);
              if(pfB2R_RM_Cb)
              {
                  pfB2R_RM_Cb(u2ConflictVdp,eCond,1-u2ConflictVdp);
                  break;
              }
          }
      }
      else
      {
          i=i-MaxB2RNum;
          if( B2R_RM.NB2R_RM[i].u2CapabilityLvl==No_Capability)
          {
              
              u2ConflictVdp = _B2R_GetVdpId(i);
              if(pfB2R_RM_Cb)
              {
                  pfB2R_RM_Cb(u2ConflictVdp,eCond,1-u2ConflictVdp);
                  break;
              }
          }
      }
  }
  
  }


B2R_Table  B2R_Select_OldB2R(B2R_RM_T input_Src,UCHAR ucVdpId)
{
    B2R_Table eB2RID=B2R_NULL;
    LOG(1,"Old [B2RHD].u2CapabilityLvl=%d,[B2RSD].u2CapabilityLvl=%d\n",B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl);
    if(ucVdpId==VDP_1)
    {
      
        if(B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability)
        {
            if(B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl==Max_Capability)//Old B2R HD/SD both free
            {
                 if(B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)
                 {
                      eB2RID = Old_B2R0;
                      B2R_RM.OldB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.OldB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                      B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =Limit_Capability;
                      B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                      return eB2RID;
                 }
                 else if((B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability))
                 {
                      eB2RID = Old_B2R1;
                      B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                      B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =Limit_Capability;
                      B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                      return eB2RID;
                 }
            }
            else if((B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl==Limit_Capability))
            {
                 if((B2R_RM.OldB2R_RM[B2RSD].u2HActive*B2R_RM.OldB2R_RM[B2RSD].u2VActive>=FHD_INPUT)&&(B2R_RM.OldB2R_RM[B2RSD].u2Fps>30))
                 {
                      eB2RID = New_B2R1;
                      B2R_RM.NB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =Limit_Capability;
                      B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                      return eB2RID;
                 }
                 else
                 {
                      eB2RID = Old_B2R0;
                      B2R_RM.OldB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.OldB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                      B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                      return eB2RID;
                 }
            }
        }
        else if (B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)
        {
            if((B2R_RM.OldB2R_RM[B2RHD].u2HActive*B2R_RM.OldB2R_RM[B2RHD].u2VActive>=FHD_INPUT)&&(B2R_RM.OldB2R_RM[B2RHD].u2Fps>30))
            {
                  eB2RID = New_B2R0;
                  B2R_RM.NB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                  B2R_RM.NB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                  B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =Limit_Capability;
                  B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                  return eB2RID;
            }
            else
            {
                eB2RID = Old_B2R1;
                B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                return eB2RID;
            }
               
        }
    }
    else if(ucVdpId==VDP_2)
    {
        if(B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)
        {
            if(B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl==Max_Capability)//Old B2R HD/SD both free
            {
                 if((B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability))
                 {
                      eB2RID = Old_B2R1;
                      B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                      B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =Limit_Capability;
                      B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                      return eB2RID;
                 }
                 else if((B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability))
                 {
                      eB2RID = Old_B2R0;
                      B2R_RM.OldB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.OldB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                      B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =Limit_Capability;
                      B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                      return eB2RID;
                 }
            }
            else if((B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl==Limit_Capability))
            {
                 if((B2R_RM.OldB2R_RM[B2RHD].u2HActive*B2R_RM.OldB2R_RM[B2RHD].u2VActive>=FHD_INPUT)&&(B2R_RM.OldB2R_RM[B2RHD].u2Fps>30))
                 {
                      eB2RID = New_B2R0;
                      B2R_RM.NB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =Limit_Capability;
                      B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                      return eB2RID;
                 }
                 else
                 {
                      eB2RID = Old_B2R1;
                      B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                      B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                      return eB2RID;
                 }
            }
        }
        else if (B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability)
        {
            LOG(1,"u2HActive=%d,u2VActive=%d,u2Fps=%d,New B2R HD cap=%d,SD=%d\n,",B2R_RM.OldB2R_RM[B2RSD].u2HActive,B2R_RM.OldB2R_RM[B2RSD].u2VActive,B2R_RM.OldB2R_RM[B2RSD].u2Fps,B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
            if((B2R_RM.OldB2R_RM[B2RSD].u2HActive*B2R_RM.OldB2R_RM[B2RSD].u2VActive>=FHD_INPUT)&&(B2R_RM.OldB2R_RM[B2RSD].u2Fps>30))
            {
                eB2RID = New_B2R1;
                B2R_RM.NB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                B2R_RM.NB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =Limit_Capability;
                B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                return eB2RID;
            }
            else
            {
                eB2RID = Old_B2R0;
                B2R_RM.OldB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                B2R_RM.OldB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                return eB2RID;
            }
        }
    
    }
    return eB2RID;
}

B2R_Table  B2R_Select_NewB2R(B2R_RM_T input_Src,UCHAR ucVdpId)
{
    B2R_Table eB2RID=B2R_NULL;
   LOG(1,"[B2RHD].Lvl=%d,[B2RSD].Lvl=%d,ucVdpId=%d\n", B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl,ucVdpId);
   LOG(1,"Old [B2RHD].Lvl=%d,Old [B2RSD].Lvl=%d,ucVdpId=%d\n", B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl,ucVdpId);
   if(ucVdpId==VDP_1)
    {
        if(B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability)
        {
             if(B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl==Max_Capability)//New B2R HD/SD both free
             {
                 if((B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability))
                 {
                      eB2RID = New_B2R0;
                      B2R_RM.NB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =Limit_Capability;
                      B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                 }
                 else if((B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability))
                 {
                      eB2RID = New_B2R1;
                      B2R_RM.NB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                      B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =Limit_Capability;
                 }
             }
            else if ((B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl==Limit_Capability))//New B2R SD  is used
            {
                 if((input_Src.fg8Bit&&B2R_RM.NB2R_RM[B2RSD].fg8Bit)||(input_Src.eVdecType!=B2R_RM.NB2R_RM[B2RSD].eVdecType))
                 {
                      eB2RID = New_B2R0;
                      B2R_RM.NB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                     
                 }
            }
        }
       else if(B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)
        {
             if((input_Src.fg8Bit&&B2R_RM.NB2R_RM[B2RHD].fg8Bit)||(input_Src.eVdecType!=B2R_RM.NB2R_RM[B2RHD].eVdecType))
              {
                      eB2RID = New_B2R1;
                      B2R_RM.NB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                     
              }
        
        }
       
    }
   else if(ucVdpId==VDP_2)
    {
    
        if(B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)
        {
             if(B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl==Max_Capability)//New B2R HD/SD both free
             {
                 if((B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability))
                 {
                      eB2RID = New_B2R1;
                      B2R_RM.NB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =Limit_Capability;
                      B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                 }
                 else if((B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability))
                 {
                      eB2RID = New_B2R0;
                      B2R_RM.NB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                      B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =Limit_Capability;
                 }
             
             }
             else if ((B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl==Limit_Capability))//New B2R SD  is used
             {
                 if((input_Src.fg8Bit&&B2R_RM.NB2R_RM[B2RHD].fg8Bit)||(input_Src.eVdecType!=B2R_RM.NB2R_RM[B2RHD].eVdecType))
                 {
                      eB2RID = New_B2R1;
                      B2R_RM.NB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                     
                 }
             }
        }
        else if(B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability)
        {
             if((input_Src.fg8Bit&&B2R_RM.NB2R_RM[B2RSD].fg8Bit)||(input_Src.eVdecType!=B2R_RM.NB2R_RM[B2RSD].eVdecType))
              {
                      eB2RID = New_B2R0;
                      B2R_RM.NB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                      B2R_RM.NB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                      B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                     
              }
        
        }
    }
   return eB2RID;
}

B2R_Table  B2R_resource_alloc(B2R_RM_T input_Src,UCHAR ucVdpId)
{
    B2R_Table eB2RID =B2R_NULL;
    if(input_Src.u2BlockMode)
    {
         LOG(1,"u2BlockMode=%d,Max input=%x\n",input_Src.u2BlockMode,input_Src.u2HActive*input_Src.u2VActive);
         if(input_Src.u2HActive*input_Src.u2VActive>FHD_INPUT)//4k2k byapss 
         {
            LOG(1,"Bypass mode.NB2R_RM[B2RHD].u2CapabilityLvl=%d,.NB2R_RM[B2RHD].u2CapabilityLvl=%d\n",B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
           
            if(( B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl==Max_Capability)&&(( B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)))
            {
                eB2RID = New_B2R0;
                B2R_RM.NB2R_RM[B2RSD].fg4K2k=TRUE;
                B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                return eB2RID;
            }    
        }
        else////FHD case
        {
            LOG(1,"eVdecType=%d\n",input_Src.eVdecType);
            if(input_Src.eVdecType==VDEC_H265)//use New B2R t0 process
            {
                eB2RID = B2R_Select_NewB2R(input_Src,ucVdpId); 
            }
            else
            {
                LOG(1,"input_Src.fg8Bit=%d,input_Src.fgUFOEnable=%d\n",input_Src.fg8Bit,input_Src.fgUFOEnable);
                if(!(input_Src.fg8Bit)||input_Src.fgUFOEnable)
                {
                    eB2RID = B2R_Select_NewB2R(input_Src,ucVdpId); 
                }
                else
                {
                    eB2RID = B2R_Select_OldB2R(input_Src,ucVdpId);
                }
            }
        
        }
    }
    else//raster mode input
    {
          if(input_Src.u2HActive*input_Src.u2VActive>FHD_INPUT)
          {
            LOG(1,"Raster 4K  input (B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl=%d,B2R_RM.NB2R_RM[B2RHSD].u2CapabilityLvl=%d \n",B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
            LOG(1,"B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl =%d \n",B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
              if((( B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl==Max_Capability))&&( B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability))
              {
                  eB2RID = Old_B2R0;
                  B2R_RM.OldB2R_RM[B2RSD].fg4K2k=TRUE;
                  B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                  B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                
              }
          }
          else
          {
               if(ucVdpId==VDP_1)
               {
                    LOG(1,"Main input (B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl=%d,B2R_RM.NB2R_RM[B2RHSD].u2CapabilityLvl=%d\n",B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
                
                    if(B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability)
                    {
                        if(B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl==Max_Capability)//Old B2R HD/SD both free
                        {
                             if((B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability))
                             {
                                  eB2RID = Old_B2R0;
                                  B2R_RM.OldB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                                  B2R_RM.OldB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                                  B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =Limit_Capability;
                                  B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                                
                             }
                             else if((B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability))
                             {
                                  eB2RID = Old_B2R1;
                                  B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                                  B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                                  B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =Limit_Capability;
                                  B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                                 
                             }
                        }
                        else if(B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl==Limit_Capability)
                        {
                            
                            
                              eB2RID = Old_B2R0;
                              B2R_RM.OldB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                              B2R_RM.OldB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                              B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                             
                             
                        }
                    }
                    else if (B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)
                    {
                       
                       
                        eB2RID = Old_B2R1;
                        B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                        B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                        B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                        
                        
                           
                    }
               }
               else if(ucVdpId==VDP_2)
               {
                   LOG(1,"Sub input (B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl=%d,B2R_RM.NB2R_RM[B2RHSD].u2CapabilityLvl=%d,B2R HW HD=%d,SD=%d",B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
                   
                    if(B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)
                    {
                        if(B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl==Max_Capability)//Old B2R HD/SD both free
                        {
                             if((B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl!=No_Capability))
                             {
                                  eB2RID = Old_B2R1;
                                  B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                                  B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                                  B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =Limit_Capability;
                                  B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                                 
                             }
                             else if((B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability))
                             {
                                  eB2RID = Old_B2R0;
                                  B2R_RM.OldB2R_RM[B2RHD].fg8Bit=input_Src.fg8Bit;
                                  B2R_RM.OldB2R_RM[B2RHD].eVdecType=input_Src.eVdecType;
                                  B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =Limit_Capability;
                                  B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl =No_Capability;
                                  
                             }
                        }
                        else if(B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl==Limit_Capability)
                        {
                            
                              eB2RID = Old_B2R1;
                              B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                              B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                              B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                             
                             
                        }
                    }
                    else if (B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl!=No_Capability)
                    {
                       
                       
                        eB2RID = Old_B2R1;
                        B2R_RM.OldB2R_RM[B2RSD].fg8Bit=input_Src.fg8Bit;
                        B2R_RM.OldB2R_RM[B2RSD].eVdecType=input_Src.eVdecType;
                        B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl =No_Capability;
                          
                         
                    }
               }
          }
    }
    if(eB2RID!=B2R_NULL)
    {
        return eB2RID;
    }
    else
    {
    
        B2R_Send_Error_Notify(B2R_RM_ALLOC_CFT);
        return eB2RID;
    }
}

void b2r_resource_release(UCHAR ucB2rId)
{
    
 
    if(ucB2rId>New_B2R1)
    {
        return ;
    }
  
   
   LOG(3,"====b2r_resource_release Old HD=%d,Old SD=%d,New HD=%d,New SD=%d,Current B2R =%d\n",B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl, B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl,ucB2rId);
    if(ucB2rId<New_B2R0)//release old B2R id
    {
        x_memset(&B2R_RM.OldB2R_RM[ucB2rId], 0, sizeof(B2R_RM_T));
        B2R_RM.OldB2R_RM[ucB2rId].u2CapabilityLvl=Limit_Capability;
        if(( B2R_RM.OldB2R_RM[1-ucB2rId].u2CapabilityLvl>=Limit_Capability)||B2R_RM.OldB2R_RM[B2RSD].fg4K2k)
        {
            x_memset(&B2R_RM.OldB2R_RM[ucB2rId], 0, sizeof(B2R_RM_T));
            x_memset(&B2R_RM.OldB2R_RM[1-ucB2rId], 0, sizeof(B2R_RM_T));
            B2R_RM.OldB2R_RM[ucB2rId].u2CapabilityLvl=Max_Capability;
            B2R_RM.OldB2R_RM[1-ucB2rId].u2CapabilityLvl=Max_Capability;
        }
    }
    else//release New  B2R 
    {   
        ucB2rId=ucB2rId-2;
        x_memset(&B2R_RM.NB2R_RM[ucB2rId], 0, sizeof(B2R_RM_T));
        B2R_RM.NB2R_RM[ucB2rId].u2CapabilityLvl=Limit_Capability;
        if( B2R_RM.NB2R_RM[1-ucB2rId].u2CapabilityLvl>=Limit_Capability||B2R_RM.NB2R_RM[B2RSD].fg4K2k)//if(3-ucB2rId)
        {
            x_memset(&B2R_RM.NB2R_RM[ucB2rId], 0, sizeof(B2R_RM_T));
            x_memset(&B2R_RM.NB2R_RM[1-ucB2rId], 0, sizeof(B2R_RM_T));
            B2R_RM.NB2R_RM[ucB2rId].u2CapabilityLvl=Max_Capability;
            B2R_RM.NB2R_RM[1-ucB2rId].u2CapabilityLvl=Max_Capability;
        }
    }
    
}

static void B2R_status_rest(void)
{

    x_memset(&B2R_RM, 0, sizeof(B2R_Capability_Map));
    B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl=Max_Capability;
    B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl=Max_Capability;
    B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl=Max_Capability;
    B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl=Max_Capability;


}

void VDP_SetVideSrc(UCHAR ucSrc, UCHAR ucB2rId)
{
        VDP_CFG_T*  ptVdpCfg = NULL;
        
        LOG(1,"VDP set src(%d),B2rId(%d)!\n",ucSrc, ucB2rId);
    switch(ucSrc)
    {
        case SV_VP_MAIN:
        {
            ucMainSnkId = ucB2rId;
            if(ucB2rId >= B2R_NS)
            {
                VDP_B2rSwitch(VDP_1, B2R_NS);
                break;
            }
    
            if(ucMainSnkId != ucSubSnkId)
            {
                break;
            }
                
            ptVdpCfg = _B2R_GetVdpConf(VDP_2);
            if(!ptVdpCfg)
            {
                break;
            }
    
            if(ptVdpCfg->ucB2rId < B2R_NS)
            {
                B2R_OBJECT_T * this = NULL;
                
                VDP_B2rSwitch(VDP_1, ptVdpCfg->ucB2rId);
                this =_B2R_GetObj(ptVdpCfg->ucB2rId);
                {
                    B2R_HAL_OMUX_T tOmux = {0};
                    VDP_CFG_T *ptVdpCfg = _B2R_GetVdpConf(VDP_1);
                    
                    tOmux.ucPath = VDP_1;
                    if (ptVdpCfg)
                    {
                        tOmux.fgScartOut = ptVdpCfg->fgScartOut;
                    }
                    B2R_HAL_Set(this->hB2r, B2R_HAL_OMUX, &tOmux);
                }
            }
            break;
        }

        case SV_VP_PIP:
        {
                ucSubSnkId = ucB2rId;
                if(ucB2rId >= B2R_NS)
                {
                    VDP_B2rSwitch(VDP_2, B2R_NS);
                    break;
                }
    
                if(ucMainSnkId != ucSubSnkId)
                {
                    break;
                }
                
                ptVdpCfg = _B2R_GetVdpConf(VDP_1);
                if(!ptVdpCfg)
                {
                    break;
                }
    
                if(ptVdpCfg->ucB2rId < B2R_NS)
            {
                    VDP_B2rSwitch(VDP_2, ptVdpCfg->ucB2rId);
            }
            break;
        }

        default:
            break;
    }
}

void VDP_SetB2RId( UCHAR ucB2RId,UCHAR ucVdpId, UCHAR ucVdecEsId)
{
    VDP_CFG_T*  ptVdpCfg = NULL;
    
    ucCurB2RId=ucB2RId;
    if (ucVdpId == VDP_1)
    {
        ucMainSnkId = ucVdpId;
        ptVdpCfg = _B2R_GetVdpConf(VDP_1);
        _prVdpCfg[ucVdpId]->ucInputPort[0] = ucVdecEsId;
        if(ptVdpCfg &&
            ptVdpCfg->ucB2rId != B2R_NS)
        {
            VDP_B2rSwitch(VDP_1, B2R_NS);
        }
    }
    else if (ucVdpId == VDP_2)
    {
        ucSubSnkId = ucVdpId;
        ptVdpCfg = _B2R_GetVdpConf(VDP_2);
        _prVdpCfg[ucVdpId]->ucInputPort[0] = ucVdecEsId;
        if(ptVdpCfg &&
            ptVdpCfg->ucB2rId != B2R_NS)
        {
            VDP_B2rSwitch(VDP_2, B2R_NS);
        }
    }
}

UCHAR  _B2R_TableConv(B2R_Table eB2rId)
{
    UCHAR ucB2rId = B2R_NS;
    
    switch (eB2rId)
    {
        case Old_B2R0:
            ucB2rId = B2R_1;
            break;
            
        case Old_B2R1:
            ucB2rId = B2R_2;
            break;

        case New_B2R0:
            ucB2rId = B2R_3;
            break;

        case New_B2R1:
            ucB2rId = B2R_4;
            break;
            
        default:
            ucB2rId = B2R_NS;
            break;
    }

    return ucB2rId;
}

INT32  _B2R_MainConn(B2R_RM_T* ptSrcInfo,
                            UCHAR*    puB2rId)
{
    INT32 i4Ret   = B2R_OK;
    B2R_Table eB2rId;
    UCHAR ucB2rId = B2R_NS;
    VDP_CFG_T*  ptVdpCfg = NULL;
    
    do
    {
        LOG(1,"ptSrcInfo=%d,puB2rId=%d\n",!ptSrcInfo,!puB2rId);
        if(!ptSrcInfo || !puB2rId)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        ptVdpCfg = _B2R_GetVdpConf(VDP_1);
        LOG(1,"_B2R_MainConn ptVdpCfg=%d\n",!ptVdpCfg);
        if(!ptVdpCfg)
        {
            i4Ret = B2R_FAIL;
            break;
        }
        LOG(1,"_B2R_MainConn  ptVdpCfg->ucB2rId=%d\n",ptVdpCfg->ucB2rId);
        if(ptVdpCfg->ucB2rId < B2R_NS)
        {
        #ifndef CC_VDEC_RM_SUPPORT
         #ifdef CC_B2R_RM_SUPPORT
            *puB2rId = ptVdpCfg->ucB2rId;
         #endif
        #endif
            i4Ret = B2R_FAIL;
            break;
        }

        LOG(1,"====u2CapabilityLvl Old HD=%d,Old SD=%d,New HD=%d,New SD=%d\n",B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl, B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
        eB2rId = B2R_resource_alloc(*ptSrcInfo, VDP_1);
      
        ucB2rId = _B2R_TableConv(eB2rId);
        if(ucB2rId >= B2R_NS)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        *puB2rId = ucB2rId;
  
        LOG(3,"MainConn,VDP(%d) switch to B2R(%d)!\n",VDP_1, ucB2rId);
        VDP_B2rSwitch(VDP_1, ucB2rId);
    }while(0);

    return i4Ret;
}

INT32  _B2R_SubConn(B2R_RM_T* ptSrcInfo,
                           UCHAR*    puB2rId)
{
    INT32 i4Ret   = B2R_OK;
    B2R_Table eB2rId;
    UCHAR ucB2rId = B2R_NS;
    VDP_CFG_T*  ptVdpCfg = NULL;
    
    do
    {
        if(!ptSrcInfo || !puB2rId)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        ptVdpCfg = _B2R_GetVdpConf(VDP_2);
        if(!ptVdpCfg)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        if(ptVdpCfg->ucB2rId < B2R_NS)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        LOG(3,"====u2CapabilityLvl Old HD=%d,Old SD=%d,New HD=%d,New SD=%d\n",B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl, B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
        eB2rId = B2R_resource_alloc(*ptSrcInfo, VDP_2);
        ucB2rId = _B2R_TableConv(eB2rId);
        if(ucB2rId >= B2R_NS)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        *puB2rId = ucB2rId;
        
        LOG(3,"MainConn,VDP(%d) switch to B2R(%d)!\n",VDP_2, ucB2rId);
        VDP_B2rSwitch(VDP_2, ucB2rId);
    }while(0);

    return i4Ret;
}

INT32  _B2R_ScartOutConn(B2R_RM_T* ptSrcInfo,
                            UCHAR*    puB2rId)
{
    INT32 i4Ret   = B2R_OK;
    B2R_Table eB2rId;
    UCHAR ucB2rId = B2R_NS;
    VDP_CFG_T*  ptMVdpCfg = NULL;
    VDP_CFG_T*  ptSVdpCfg = NULL;
    
    do
    {
        if(!ptSrcInfo || !puB2rId)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        ptMVdpCfg = _B2R_GetVdpConf(VDP_1);
        ptSVdpCfg = _B2R_GetVdpConf(VDP_2);
        if(!ptMVdpCfg || !ptSVdpCfg)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        if(ptMVdpCfg->ucB2rId != B2R_NS &&
            ptSVdpCfg->ucB2rId != B2R_NS)
        {
            i4Ret = B2R_FAIL;
            break;
        }
        
        LOG(3,"====u2CapabilityLvl Old HD=%d,Old SD=%d,New HD=%d,New SD=%d\n",B2R_RM.OldB2R_RM[B2RHD].u2CapabilityLvl, B2R_RM.OldB2R_RM[B2RSD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RHD].u2CapabilityLvl,B2R_RM.NB2R_RM[B2RSD].u2CapabilityLvl);
        eB2rId = B2R_resource_alloc(*ptSrcInfo, VDP_1);
        ucB2rId = _B2R_TableConv(eB2rId);
        if(ucB2rId >= B2R_NS)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        *puB2rId = ucB2rId;

        LOG(3,"ScartOut,VDP switch B2R!\n");
        VDP_B2rSwitch(VDP_1, ucB2rId);
        VDP_B2rSwitch(VDP_2, ucB2rId);
    }while(0);

    return i4Ret;
}

VOID  _B2R_CliConn(UCHAR ucB2rId)
{

}

UCHAR  B2R_Source_Connect(B2R_RM_T input_Src)
{
    UCHAR ucB2rId = B2R_NS;

    if(ucCurB2RId<B2R_NS)
    {
        _B2R_CliConn(ucCurB2RId);
    }
    else
    {
        LOG(3,"Prepare to Map VDP and B2R,MainSnk:%d,SubSnk:%d!\n",ucMainSnkId, ucSubSnkId);
        if((ucMainSnkId == ucSubSnkId)&&(ucMainSnkId!=B2R_NS))
        {
            if (_B2R_ScartOutConn(&input_Src, &ucB2rId) == B2R_OK)
            {
                VDP_CFG_T *ptVdpCfg = _B2R_GetVdpConf(VDP_1);

                if (ptVdpCfg)
                {
                    ptVdpCfg->fgScartOut = TRUE;
                }
            }
        }
        else
        {
            if ((ucMainSnkId < VDP_NS) && (_prVdpCfg[VDP_1]->ucInputPort[0] == input_Src.ucEsId))
            {
                if(B2R_OK == _B2R_MainConn(&input_Src, &ucB2rId))
                {
                    return ucB2rId;
                }
            }

            if ((ucSubSnkId < VDP_NS) && (_prVdpCfg[VDP_2]->ucInputPort[0] == input_Src.ucEsId))
            {
                if(B2R_OK == _B2R_SubConn(&input_Src, &ucB2rId))
                {
                    return ucB2rId;
                }
            }
        }     
    }
    
    return ucB2rId;
}
#if 0
void b2r_resource_test(UINT16 u2TestItem)// for CLI test 
{
    B2R_Table sourceid;
 //   UINT16 i;
  //  for(i=0;i<=265;i++)
        {
    B2R_status_rest();
    Printf("Frist input u2HActive=%d,u2VActive=%d,fps=%d£¬is8bit=%d,eVdecType=%d,u2BlockMode=%d\n",B2R_TIMING_TEST[u2TestItem][0].u2HActive,B2R_TIMING_TEST[u2TestItem][0].u2VActive,B2R_TIMING_TEST[u2TestItem][0].u2Fps,B2R_TIMING_TEST[u2TestItem][0].fg8Bit,B2R_TIMING_TEST[u2TestItem][0].eVdecType,B2R_TIMING_TEST[u2TestItem][0].u2BlockMode);
   sourceid= B2R_resource_alloc(B2R_TIMING_TEST[u2TestItem][0]);
   Printf("Frist input connect to %d\n ",sourceid);

   Printf("second input u2HActive=%d,u2VActive=%d,fps=%d£¬is8bit=%d,eVdecType=%d,u2BlockMode=%d\n",B2R_TIMING_TEST[u2TestItem][1].u2HActive,B2R_TIMING_TEST[u2TestItem][1].u2VActive,B2R_TIMING_TEST[u2TestItem][1].u2Fps,B2R_TIMING_TEST[u2TestItem][1].fg8Bit,B2R_TIMING_TEST[u2TestItem][1].eVdecType,B2R_TIMING_TEST[u2TestItem][1].u2BlockMode);
   sourceid= B2R_resource_alloc(B2R_TIMING_TEST[u2TestItem][1]);
    Printf("Second input connect to %d\n",sourceid);

   B2R_status_rest();

        }
}
#endif
#endif

#if defined(CC_MT5890)

UINT16 VDP_GetBypassHTotal(VOID)
{
    UINT16 u2Htotal;
    u2Htotal= VDP_GetNB2R_Htotal();
    return u2Htotal;
}
UINT16 VDP_GetBypassVTotal(VOID)
{
    UINT16 u2Vtotal;
    u2Vtotal= VDP_GetNB2R_Vtotal();
    return u2Vtotal;
}
UINT16 VDP_GetBypassHActive(VOID)
{
    UINT16 u2Htotal;
    u2Htotal= VDP_GetNB2R_HActive();
    return u2Htotal;
}
UINT16 VDP_GetBypassVActive(VOID)
{
    UINT16 u2Vtotal;
    u2Vtotal= VDP_GetNB2R_VActive();
    return u2Vtotal;
}

VOID VDP_SetBypassHactive(UINT16 u2HActive)
{
    VDP_SetNB2R_Bypass_H_ACTIVE(u2HActive);
}
VOID VDP_SetBypassVactive(UINT16 u2VActive)
{
    VDP_SetNB2R_Bypass_V_ACTIVE(u2VActive);
}
VOID VDP_SetBypassHTotal(UINT16 u2HTotal)
{
    VDP_SetNB2R_Bypass_H_TOTAL(u2HTotal);
}
VOID VDP_SetBypassVTotal(UINT16 u2VTotal)
{
    VDP_SetNB2R_Bypass_V_TOTAL(u2VTotal);
}
VOID VDP_SetBypassHFph(UINT16 porch)
{
    VDP_SetNB2R_Bypass_H_FrontPorch(porch);   
}
VOID VDP_SetBypassHBph(UINT16 porch)
{
    VDP_SetNB2R_Bypass_H_BackPorch(porch);      
}
VOID VDP_SetBypassVFph(UINT16 porch)
{
   VDP_SetNB2R_Bypass_V_FrontPorch(porch);   
}
VOID VDP_SetBypassVBph(UINT16 porch)
{
  
    VDP_SetNB2R_Bypass_V_BackPorch(porch);
       
}
VOID VDP_SetBypassHVInvert(BOOL HVInvert,BOOL enable)
{
    VDP_SetNB2R_Bypass_conf(HVInvert,enable);
}
VOID VDP_SetBypassUFOModeEnable(BOOL UFOEnable,BOOL Bittype)
{
    VDP_SetNB2R_Bypass_enable(UFOEnable,Bittype);
}
VOID VDP_SetBypassHwEnable(BOOL fgEnable)
{
    B2R_HW_Enable(fgEnable);
}

#endif

INT32 B2R_RTSetInfo (UCHAR               ucB2rId,
                     B2R_RT_TYPE_T       e_set_type,
                     VOID*               pv_set_info,
                     SIZE_T              z_set_info_size)
{
    if(ucB2rId >= B2R_NS
        || !pv_set_info)
    {
        return VDP_SET_ERROR;
    }
    
    switch(e_set_type)
    {
        case B2R_RT_TYPE_CLI_ACTIVE:
        {
            UCHAR    ucEnable;

            if(z_set_info_size != sizeof(UCHAR))
            {
                return VDP_SET_ERROR;
            }
            ucEnable = *((UCHAR *)pv_set_info);
            _B2R_RTCliActive(ucB2rId,ucEnable);
            break;
        }

        case B2R_RT_TYPE_LARENCY_NS:
        {
            UINT32 u4_latency;

            if(z_set_info_size != sizeof(UINT32))
            {
                return VDP_SET_ERROR;
            }
            u4_latency = *((UINT32 *)pv_set_info);
            _B2R_SetLatency(ucB2rId,u4_latency);
            break;
        }
        
        case B2R_RT_TYPE_LARENCY_QY:
        {
            if(z_set_info_size != sizeof(UCHAR))
            {
                return VDP_SET_ERROR;
            }
            _B2R_QueryLatency(ucB2rId);
            break;
        }
        
        default:
            LOG(0,"%s,  Unknow set type : %d!\n",__FUNCTION__,e_set_type);
            break;
    }

    return VDP_SET_OK;
}

VDP_CFG_T* B2R_GetVdpConf(UCHAR ucVdpId)
{
    return _B2R_GetVdpConf(ucVdpId);
}

INT32 VDP_B2R_GetInfo (UCHAR            ucVdpId,
                           B2R_GET_TYPE_T   e_get_type,
                           VOID*            pv_get_info,
                           SIZE_T           z_get_info_size)
{
    INT32   i4Ret = B2R_OK;
    UCHAR   ucB2rId;
    VERIFY_VDP_ID(ucVdpId,B2R_INV_ARG);
    VERIFY_NULL_POINT(pv_get_info,B2R_INV_ARG);

    ucB2rId = VDP2B2RID(ucVdpId);

    B2R_MUTEX_LOCK(ucB2rId);
    
    switch (e_get_type)
    {
        case B2R_GET_TYPE_B2R_STATUS:
        {
            VDP_CFG_T*     prVdpConf;
            B2R_STATUS_T*  pt_b2r_sts = (B2R_STATUS_T*)pv_get_info;
    
            prVdpConf = _B2R_GetVdpConf(ucVdpId);
            if(!prVdpConf)
            {
                i4Ret = B2R_INV_ARG;
                break;
            }
            
            if(sizeof(B2R_STATUS_T) != z_get_info_size)
            {
                LOG(0,_ERROR_HEADER"_B2R_GetCfg Fail i4Ret : %d!\n",i4Ret);
                i4Ret = B2R_INV_ARG;
                break;
            }
            pt_b2r_sts->ucEnable = prVdpConf->ucEnable;
            pt_b2r_sts->ucStatus = prVdpConf->ucStatus;
            pt_b2r_sts->ucB2rId  = prVdpConf->ucB2rId;
            pt_b2r_sts->fgMMPlayback  = prVdpConf->fgMMPlayback;
            break;
        }
        
        case B2R_GET_TYPE_ASP_INFO:
        {
            VDP_CFG_T*       prVdpConf;
            B2R_ASP_INFO_T*  pt_asp_info = (B2R_ASP_INFO_T*)pv_get_info;
    
            prVdpConf = _B2R_GetVdpConf(ucVdpId);
            if(!prVdpConf)
            {
                i4Ret = B2R_INV_ARG;
                break;
            }
            
            if(sizeof(B2R_ASP_INFO_T) != z_get_info_size)
            {
                i4Ret = B2R_INV_ARG;
                break;
            }
            pt_asp_info->u4Afd               = prVdpConf->u4Afd;
            pt_asp_info->u4AspectRatio       = prVdpConf->u4AspectRatio;
            pt_asp_info->u4AspectRatioIdc    = prVdpConf->u4AspectRatioIdc;
            pt_asp_info->u4AspectRatioWidth  = prVdpConf->u4AspectRatioWidth;
            pt_asp_info->u4AspectRatioHeight = prVdpConf->u4AspectRatioHeight;
            break;
        }
        
        default:
            LOG(0,_ERROR_HEADER"VDP_B2R_GetInfo Unknow get type : %d!\n",e_get_type);
            break;
    }
    B2R_MUTEX_UNLOCK(ucB2rId);

    return i4Ret;
}

INT32 VDP_B2R_SetCfgParam (UCHAR               ucVdpId,
                                  B2R_SET_TYPE_CFG_T   e_set_type,
                                  VOID*                pv_set_info,
                                  SIZE_T               z_set_info_size)
{
    INT32   i4Ret = B2R_OK;
    UCHAR   ucB2rId;
    VERIFY_VDP_ID(ucVdpId,B2R_INV_ARG);
    VERIFY_NULL_POINT(pv_set_info,B2R_INV_ARG);

    ucB2rId = VDP2B2RID(ucVdpId);

    B2R_MUTEX_LOCK(ucB2rId);

    switch(e_set_type)
    {
        case B2R_CFG_MODE:
        {
            if(sizeof(UCHAR) != z_set_info_size)
            {
                LOG(0,_ERROR_HEADER"VDP_B2R_SetCfgParam Fail i4Ret : %d!\n",i4Ret);
                i4Ret = B2R_INV_ARG;
                break;
            }
            _prVdpCfg[ucVdpId]->ucMode = *((UCHAR*)pv_set_info);
            break;
        }

        case B2R_CFG_VDP_ENABLE:
        {
            if(sizeof(UCHAR) != z_set_info_size)
            {
                LOG(0,_ERROR_HEADER"VDP_B2R_SetCfgParam Fail i4Ret : %d!\n",i4Ret);
                i4Ret = B2R_INV_ARG;
                break;
            }
            _prVdpCfg[ucVdpId]->ucVdpEnable = *((UCHAR*)pv_set_info);
            break;
        }

        default:
            LOG(0,_ERROR_HEADER"VDP_B2R_SetCfgParam Unknow get type : %d!\n",pv_set_info);
            break;
    }

    B2R_MUTEX_UNLOCK(ucB2rId);

    return i4Ret;
}

void VDP_SetRenderRegion(UINT8 ucVdpId, VDP_REGION_T *prRendRegion)
{
    VERIFY_VDP_ID_RET_VOID(ucVdpId);
    _VDP_HalSetRenderRegion(VDP2B2RID(ucVdpId), prRendRegion);
}

/**
 * B2R call back function register
 *
 * @param eFuncType specify the call back function Info.
 * @param u4FuncPtr specify the call back function pointer.
 * @return INT32
 */
INT32 B2R_RegCbFunc(B2R_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr, UINT32 u4Arg1)
{
    INT32 i4Ret = B2R_OK;

    do
    {
        if(eFuncType >= B2R_CB_FUNC_NS)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        _rB2rCbFunc.au4CbFunc[eFuncType]    = u4FuncPtr;
        _rB2rCbFunc.au4CbFuncCRC[eFuncType] = u4FuncPtr ?
            ~u4FuncPtr : u4FuncPtr;
        if(u4Arg1)
        {
            x_memcpy(&_rB2rCbFunc.pu4CbObj[eFuncType],(VOID*)u4Arg1,sizeof(B2R_REG_CB_OBJ_T));
        }

    }while(0);

    return i4Ret;
}

void _B2R_EventNofify(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3)
{
    if (u4Arg1 >= (UINT32)B2R_CB_FUNC_NS)
    {
        ASSERT(u4Arg1 < (UINT32)B2R_CB_FUNC_NS);
        return;
    }

    VERIFY_VDP_ID_RET_VOID(ucVdpId);

    if(B2R_CHECK_CB_FUNC_VERIFY(_rB2rCbFunc.au4CbFunc[u4Arg1], _rB2rCbFunc.au4CbFuncCRC[u4Arg1]))
    {
        if(u4Arg1 == B2R_CB_FUNC_TRIG_AUD_TO_PLAY)
        {
            LOG(0,"%s send aud to play!\n",__FUNCTION__);
            ((B2R_CB_COMM_FUNC)_rB2rCbFunc.au4CbFunc[B2R_CB_FUNC_TRIG_AUD_TO_PLAY])
                (ucVdpId,u4Arg2,(UINT32)_rB2rCbFunc.pu4CbObj[B2R_CB_FUNC_TRIG_AUD_TO_PLAY].pfCb,
                (UINT32)_rB2rCbFunc.pu4CbObj[B2R_CB_FUNC_TRIG_AUD_TO_PLAY].rCbInfo.u.t_comm.pv_tag);
        }
        else if(u4Arg1 == B2R_CB_FUNC_EOS)
        {
            ((B2R_CB_COMM_FUNC)_rB2rCbFunc.au4CbFunc[B2R_CB_FUNC_EOS])
                    (ucVdpId,u4Arg2,(UINT32)_rB2rCbFunc.pu4CbObj[B2R_CB_FUNC_EOS].pfCb,
                    (UINT32)_rB2rCbFunc.pu4CbObj[B2R_CB_FUNC_EOS].rCbInfo.u.t_comm.pv_tag);
        }
    }
    else
    {
        ASSERT(_rB2rCbFunc.au4CbFunc[u4Arg1] == 0);
    }
}

/** Disable VDP Trigger action */
INT32 B2R_AudTrigger(UINT8 ucVdpId, VDP_TRIG_AUD_CFG_T* pr_trig_cfg)
{
    return _B2R_AudTrigger(ucVdpId, pr_trig_cfg);
}

INT32 B2R_FilterLog(UCHAR ucVdpId, UCHAR ucEnable, UINT32 u4Mask)
{
    INT32 i4Ret = B2R_OK;
    //UINT32 u4Filter = (1 << u4Mask);
    VDP_CFG_T* ptVdpConf = NULL;

    do
    {
        if(ucVdpId >= VDP_NS)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        ptVdpConf = _B2R_GetVdpConf(ucVdpId);
        if(!ptVdpConf)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        if(ucEnable)
        {
            //ptVdpConf->u4LogFilter |= u4Filter;
        }
        else
        {
            //ptVdpConf->u4LogFilter ^= u4Filter;
        }
        //LOG(0,"LogFilter:0x%x!\n", ptVdpConf->u4LogFilter);
    }while(0);

    return i4Ret;
}

#ifdef CC_B2R_RES_SUPPORT
INT32 B2R_GetVdpIdFromFbg(UCHAR ucFbgId,
                                    UCHAR* puVdpId)
{
    UINT8 u1VdpId = VDP_NS;
    INT32 i4Ret = B2R_OK;
    UINT32 u4Idx;
    B2R_PRM_T*  ptVdpPrm = NULL;
    B2R_OBJECT_T* this   = NULL;
    
    do
    {
        if(ucFbgId >= FBG_MAX_NS || !puVdpId)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        for(u4Idx = 0; u4Idx < B2R_NS; u4Idx ++)
        {
            this = _B2R_GetObj(u4Idx);
            if(!this)
            {
                continue;
            }

            ptVdpPrm = this->ptB2rPrm;
            if(!ptVdpPrm)
            {
                continue;
            }

            if(ptVdpPrm->ucFbgId == ucFbgId)
            {
                u1VdpId = _B2R_GetVdpId(u4Idx);
                break;
            }
        }

        *puVdpId = u1VdpId;
    }while(0);

    return i4Ret;
}

INT32 B2R_MainSubStatus(UCHAR* pucMVdpId,
                                UCHAR* pucSVdpId)
{
    INT32 i4Ret = B2R_OK;
    UCHAR ucMVdpId = VDP_NS;
    UCHAR ucSVdpId = VDP_NS;
    VDP_CFG_T* ptVdpConf = NULL;

    do
    {
        if(!pucMVdpId || !pucSVdpId)
        {
            LOG(0,"pucMVdpId = NULL or pucSVdpId = NULL");
            i4Ret = B2R_INV_ARG;
            break;
        }

        ptVdpConf = _B2R_GetVdpConf(VDP_1);
        if(ptVdpConf &&
            ptVdpConf->ucB2rId != B2R_NS)
        {
            ucMVdpId = VDP_1;
        }

        ptVdpConf = _B2R_GetVdpConf(VDP_2);
        if(ptVdpConf &&
            ptVdpConf->ucB2rId != B2R_NS)
        {
            ucSVdpId = VDP_2;
        }

        *pucMVdpId = ucMVdpId;
        *pucSVdpId = ucSVdpId;
    }while(0);

    return i4Ret;
}

BOOL B2R_ChkHwSwitch(UCHAR ucVdpId,
                              UCHAR ucB2rId,
                              UCHAR* pucRetId)
{
    return _B2R_ChkHwSwitch(ucVdpId, ucB2rId, pucRetId);
}

UCHAR Get_VDP_2_B2R_ID(UINT8 ucVdpId)
{
    return VDP2B2RID((UCHAR)ucVdpId); 
}


#endif

