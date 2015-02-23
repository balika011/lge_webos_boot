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
 * $Date: 2015/02/23 $
 * $RCSfile: vdp_vsync.c,v $
 * $Revision: #7 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_vsync.c
 *  Brief of file vdp_vsync.c.
 *  Details of file vdp_vsync.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdp_vsync.h"
#include "vdp_hal.h"
#include "vdp_frc.h"
#include "fbm_drvif.h"
#include "vdp_debug.h"
#include "b2r_drvif.h"
#include "video_def.h"
#include "sv_const.h"
#include "b2r_hal.h"
#include "../tve/tve_hal.h"
#include "tve_if.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "drv_common.h"
#include "x_os.h"
#include "x_assert.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef UINT32 (*VDP_CMD_FUNC)(UCHAR ucB2rId);


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define VDP_MSG_Q_NUM           8

#define VDP_NOTIFY_Q_NUM        32

#define VDP_VSYNC_Q_FULL_ERR    8


/** Brief of VSYNC_RECORD_T
 */
typedef struct
{
    UINT32 u4B2rId;
    UINT32 u4Bottom;
    UINT32 u4IsrType;
    UINT32 u4RightView;
} VSYNC_RECORD_T;

typedef enum
{
    B2R_VMSG_TYPE_CMD,
    B2R_VMSG_TYPE_INT,
    B2R_VMSG_TYPE_SINT,
    B2R_VMSG_TYPE_MAX
} B2R_VMSG_TYPE_T;

typedef enum
{
    B2R_VCMD_RUN,
    B2R_VCMD_STOP,
    B2R_VCMD_CONNECT,     //connect to hw
    B2R_VCMD_DISCONNECT,  //disconnect to hw
    B2R_VCMD_MAX
} B2R_VCMD_TYPE_T;


typedef struct
{
    B2R_VCMD_TYPE_T eCmd;
} B2R_VCMD_T;



typedef struct
{
    UINT32 u4B2rId;
    UINT32 u4Bottom;
    UINT32 u4IsrType;
    UINT32 u4RightView;
} B2R_VINT_T;

typedef struct
{
    B2R_VMSG_TYPE_T eType;
    union{
        B2R_VCMD_T tCmd;
        B2R_VINT_T tInt;        
    }u;
} B2R_VMSG_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static UINT32 _B2R_CmdRst(UCHAR ucB2rId);
static UINT32 _B2R_CmdEnable(UCHAR ucB2rId);
static UINT32 _B2R_CmdMode(UCHAR ucB2rId);
static UINT32 _B2R_CmdInput(UCHAR ucB2rId);
static UINT32 _B2R_CmdOutput(UCHAR ucB2rId);
static UINT32 _B2R_CmdBg(UCHAR ucB2rId);
static UINT32 _B2R_CmdSrcOutRegion(UCHAR u4B2rId);
static INT32 _B2R_SendCmdMsg(B2R_OBJECT_T *this, B2R_VCMD_TYPE_T eType, UINT32 u4Arg);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static const VDP_CMD_FUNC _apfCmdHandle[] =
{
    _B2R_CmdRst,             //VDP_CMD_RST
    _B2R_CmdEnable,          //VDP_CMD_SET_ENABLE
    _B2R_CmdMode,            //VDP_CMD_SET_MODE
    _B2R_CmdInput,           //VDP_CMD_SET_INPUT
    _B2R_CmdOutput,          //VDP_CMD_SET_OUTPUT
    _B2R_CmdSrcOutRegion,    //VDP_CMD_SET_SRCR
    _B2R_CmdSrcOutRegion,    //VDP_CMD_SET_OUTR
    _B2R_CmdBg,              //VDP_CMD_SET_BG
    _B2R_CmdSrcOutRegion     //VDP_CMD_SET_SRCSIZE
};

static UCHAR _ucVdpVsyncInitiated = 0;

static VDP_CFG_T*  _prVdpCfg[VDP_NS];

static HANDLE_T _ahDtvMutex[B2R_NS];

static UINT32 _u4VdpVsyncOnOff[B2R_NS];     // For Video Image Module

//static UINT32 _u4VdpVsyncQFullCount = 0;

static VDP_VSYNC_CB_FUNC _vdp_vsync_cb[VDP_NS];
static VOID*             _vdp_vsync_cb_tag[VDP_NS];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static inline HANDLE_T _B2R_GetLock(UCHAR ucB2rId)
{
    UNUSED(ucB2rId);
    return _ahDtvMutex[ucB2rId];
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_CmdRst.
 */
//-----------------------------------------------------------------------------
static UINT32 _B2R_CmdRst(UCHAR ucB2rId)
{
    B2R_OBJECT_T *this;
    this = _B2R_GetObj(ucB2rId);

    if (this)
    {
        B2R_HAL_Reset(this->hB2r);
    }

    return VDP_EVENT_NONE;
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_CmdEnable.
 */
//-----------------------------------------------------------------------------
static UINT32 _B2R_CmdEnable(UCHAR ucB2rId)
{
    B2R_OBJECT_T *this;
    UCHAR ucVdpId = _B2R_GetVdpId(ucB2rId);
    if(ucVdpId >= VDP_NS)
    {
        return VDP_EVENT_NONE;
    }

    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return VDP_EVENT_NONE;
    }
    
    if (_prVdpCfg[ucVdpId]->ucEnable)
    {
        LOG(0,"_B2R_CmdEnable(%d)->B2R_VCMD_RUN\n",this->ucB2rId);
        _B2R_SendCmdMsg(this, B2R_VCMD_RUN, 0);
    }
    else
    {
       LOG(0,"B2R_HAL_Disable(%d)->B2R_VCMD_STOP\n",this->ucB2rId);
        B2R_HAL_Disable(this->hB2r);
        _B2R_SendCmdMsg(this, B2R_VCMD_STOP, 0);
    }

    if (!_prVdpCfg[ucVdpId]->ucEnable)
    {
        //_VDP_HalForceBg(ucB2rId, 1);
        _B2R_ForceBg(ucB2rId, 1);
    }

    return (VDP_EVENT_RECONFIG | VDP_EVENT_ENABLE_CHG);
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_.
 */
//-----------------------------------------------------------------------------
static UINT32 _B2R_CmdMode(UCHAR ucB2rId)
{
    UCHAR ucVdpId = _B2R_GetVdpId(ucB2rId);
    if(ucVdpId >= VDP_NS)
    {
        return VDP_EVENT_NONE;
    }
    if (_prVdpCfg[ucVdpId]->ucMode == VDP_MODE_BG)
    {
        return VDP_EVENT_NONE;
    }
    else
    {
        //_VDP_HalForceBg(ucB2rId, 0);
        _B2R_ForceBg(ucB2rId, 0);
        return (VDP_EVENT_RECONFIG | VDP_EVENT_MODE_CHG);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_CmdInput.
 */
//-----------------------------------------------------------------------------
static UINT32 _B2R_CmdInput(UCHAR ucB2rId)
{
    UNUSED(ucB2rId);
    return VDP_EVENT_INPUT_CHG;
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_CmdOutput.
 */
//-----------------------------------------------------------------------------
static UINT32 _B2R_CmdOutput(UCHAR ucB2rId)
{
    UNUSED(ucB2rId);

#ifdef CC_SCPOS_EN
    // do nothing
    return VDP_EVENT_NONE;
#else
    return (VDP_EVENT_RECONFIG| VDP_EVENT_OUTPUT_CHG);
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_CmdBg.
 */
//-----------------------------------------------------------------------------
static UINT32 _B2R_CmdBg(UCHAR ucB2rId)
{
    UINT32 u4Y, u4Cb, u4Cr;
    UINT32 u4A, u4B, u4C;
    UINT32 u4YCbCr;
    B2R_OBJECT_T *this;
    B2R_HAL_TP_T tPat;

    
    UCHAR ucVdpId = _B2R_GetVdpId(ucB2rId);
    if (ucVdpId >= VDP_NS)
    {
        return VDP_EVENT_NONE;
    }
    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return VDP_EVENT_NONE;
    }

    u4A = (_prVdpCfg[ucVdpId]->u4BgColor >> 16) & 0xFF;
    u4B= (_prVdpCfg[ucVdpId]->u4BgColor >> 8) & 0xFF;
    u4C = (_prVdpCfg[ucVdpId]->u4BgColor) & 0xFF;

    u4Y = (16 + (((u4A * 66) + (u4B * 129) + (u4C * 25)) / 255)) & 0xFF;
    u4Cb = ((((128 * 255) + (u4C * 112)) - ((u4A * 38) + (u4B * 74)) ) / 255) & 0xFF;
    u4Cr = ((((128 * 255) + (u4A * 112)) - ((u4B * 94) + (u4C * 18))) / 255) & 0xFF;

    u4YCbCr = ((u4Y << 16) + (u4Cb << 8) + (u4Cr));

    //_VDP_HalSetBg(ucB2rId, u4YCbCr);
    tPat.fgEnable = FALSE;
    tPat.eMode    = B2R_TP_NO_GRID;
    tPat.u4Color  = u4YCbCr;
    B2R_HAL_Set(this->hB2r, B2R_HAL_PATTERN, &tPat); 
    
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_CmdSrcOutRegion.
 */
//-----------------------------------------------------------------------------
static UINT32 _B2R_CmdSrcOutRegion(UCHAR ucB2rId)
{
    B2R_OBJECT_T *this;
    UCHAR ucVdpId = _B2R_GetVdpId(ucB2rId);
    ASSERT(ucB2rId < B2R_NS && ucVdpId < VDP_NS);
	
	LOG(0,"_B2R_CmdSrcOutRegion (%d,%d)\n",ucB2rId,ucVdpId);
	LOG(0,"main status (%d,%d)\n",_prVdpCfg[0]->ucVdpId,_prVdpCfg[0]->ucB2rId);
	LOG(0,"sub sratus(%d,%d)\n",_prVdpCfg[1]->ucVdpId,_prVdpCfg[1]->ucB2rId);
    if(ucB2rId >= B2R_NS || ucVdpId >= VDP_NS)
    {
        return VDP_EVENT_NONE;    
    }
    
    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return VDP_EVENT_NONE;
    }

    // B2R width issue
    if(_prVdpCfg[ucVdpId]->rOutInfo.u4Width < _prVdpCfg[ucVdpId]->u4SrcWidth)
    {
        LOG(0, "[%s-%d]ASSERT: rOutInfo.u4Width(%d) >=  u4SrcWidth(%d).\n", __func__, __LINE__, _prVdpCfg[ucVdpId]->rOutInfo.u4Width, _prVdpCfg[ucVdpId]->u4SrcWidth);
        ASSERT(_prVdpCfg[ucVdpId]->rOutInfo.u4Width >= _prVdpCfg[ucVdpId]->u4SrcWidth);
    }

    // Work-Around # 1, B2R/PSW Width 64 byte alignment
    /* 20081127 Pibben: Why we need to use rOutInfo.u4Width/u4Height to set H/V active? */
    B2R_HAL_Config(this->hB2r);
#ifdef CC_SUPPORT_PIPELINE
   if( VDP_PipeModeChangeing(ucVdpId,ucB2rId))
#endif
    {
        B2R_HAL_OMUX_T tOmux = {0};
        VDP_CFG_T *ptVdpCfg = _B2R_GetVdpConf(VDP_1);
        
        tOmux.ucPath = ucVdpId;
        if (ptVdpCfg)
        {
            tOmux.fgScartOut = ptVdpCfg->fgScartOut;
        }
        B2R_HAL_Set(this->hB2r, B2R_HAL_OMUX, &tOmux);
    }
   
    _B2R_PostNotify(this, VDP_MSG_NOTIFY, VDP_B2R_SOURCE_CHG, 0);
    LOG(0, "Return VDP_EVENT_RECONFIG \n");
    return VDP_EVENT_RECONFIG;
}


static UINT32 _B2R_ReConfig(UCHAR ucB2rId)
{
    B2R_OBJECT_T *this;

    this = _B2R_GetObj(ucB2rId);

    if (this)
    {
        _B2R_SendCmdMsg(this, B2R_VCMD_CONNECT, 0);
        B2R_HAL_Enable(this->hB2r);
    }

    return VDP_EVENT_NONE;
}

static UINT32 _B2R_HandleEvent(B2R_OBJECT_T *this, UINT32 u4Event)
{
    UCHAR ucB2rId;
 
    if (!this)
    {
        return B2R_INV_ARG;
    }

    ucB2rId = this->ucB2rId;


    // Seqeunce Change
    if (u4Event & VDP_EVENT_SEQ_CHG)
    {
        LOG(0, "VDP_EVENT_SEQ_CHG \n");
        u4Event |= _B2R_CmdSrcOutRegion(ucB2rId);
    }
    
    // Optional Seqeunce Change
    if (u4Event & VDP_EVENT_OPT_SEQ_CHG)
    {
        //// TODO, notify middleware (aspect ratio, afd, ...)
    }
    
    // Re-config Hardware
    if (u4Event & VDP_EVENT_RECONFIG)
    {
        LOG(0, "VDP_EVENT_RECONFIG \n");
        // todo 
        _B2R_ReConfig(ucB2rId);
    }
    
    return B2R_OK;
}


static UINT32 _B2R_HandleNotify(B2R_OBJECT_T *this)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    VDP_CB_MSG_T rNotifyMsg;
    UINT32 u4Cnt;

    if (!this)
    {
        return B2R_INV_ARG;
    }
   
    zMsgSize = sizeof(VDP_CB_MSG_T);
    u4Cnt    = 0;
    
    while (x_msg_q_receive(&u2MsgQIdx, &rNotifyMsg, &zMsgSize, &this->hNotifyMsgQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
        LOG(11, "Notify(%d) (%d, %d, %d, %d)\n", u4Cnt, rNotifyMsg.u4Type, rNotifyMsg.u4Arg1, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        u4Cnt++;
    
        B2R_MUTEX_LOCK(this->ucB2rId);
    
        if (rNotifyMsg.u4Type == VDP_MSG_NOTIFY)
        {
            if (VDP2B2RID(VDP_1) == VDP2B2RID(VDP_2))
            {
                B2R_MUTEX_UNLOCK(this->ucB2rId);
                _VDP_StatusNotify((UCHAR)VDP_1, rNotifyMsg.u4Arg2);
                _VDP_StatusNotify((UCHAR)VDP_2, rNotifyMsg.u4Arg2);
            }
            else
            {
                B2R_MUTEX_UNLOCK(this->ucB2rId);
                _VDP_StatusNotify((UCHAR)rNotifyMsg.u4Arg1, rNotifyMsg.u4Arg2);
            }
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_CC_CB)
        {
            if (VDP2B2RID(VDP_1) == VDP2B2RID(VDP_2))
            {
                _VDP_EventNofify((UCHAR)VDP_1, (UINT32)VDP_CB_FUNC_PTS_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                _VDP_EventNofify((UCHAR)VDP_2, (UINT32)VDP_CB_FUNC_PTS_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
            }
            else
            {
                _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_PTS_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
            }
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_AFD_CB)
        {
            if (VDP2B2RID(VDP_1) == VDP2B2RID(VDP_2))
            {
                UCHAR ucIdx;
                UCHAR ucSndIdx;

                ucIdx = (UCHAR)rNotifyMsg.u4Arg1;
                ucSndIdx = 1 -  ucIdx;
                if(ucIdx < VDP_NS && ucSndIdx < VDP_NS)
                {
                    _prVdpCfg[ucSndIdx]->u4AspectRatio = _prVdpCfg[ucIdx]->u4AspectRatio;
                    _prVdpCfg[ucSndIdx]->u4Afd = _prVdpCfg[ucIdx]->u4Afd;
                }
    
                _VDP_EventNofify((UCHAR)VDP_1, (UINT32)VDP_CB_FUNC_AFD_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                _VDP_EventNofify((UCHAR)VDP_2, (UINT32)VDP_CB_FUNC_AFD_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                _VDP_EventNofify((UCHAR)VDP_1, (UINT32)VDP_CB_FUNC_AFD_IND_2, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                _VDP_EventNofify((UCHAR)VDP_2, (UINT32)VDP_CB_FUNC_AFD_IND_2, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
            }
            else
            {
                _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_AFD_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
                _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_AFD_IND_2, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
            }
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_PLAY_DONE_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_PLAY_DONE_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_RES_CHG_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_RES_CHG_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_UNMUTE_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_UNMUTE_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_MM_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_MM_COND_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_LIPSYNC_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_LIPSYNC_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_GET_PIC_TYPE_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_Get_PIC_TYPE, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_MM_STEP_FIN_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_MM_STEP_FIN_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_MM_SEEK_FIN_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_MM_SEEK_FIN_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if (rNotifyMsg.u4Type == VDP_MSG_REPEAT_DROP_CB)
        {
            _VDP_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)VDP_CB_FUNC_REPEAT_DROP_IND, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if(rNotifyMsg.u4Type == VDP_MSG_TRIG_AUD_TO_PLAY)
        {
            LOG(0,"%s receieve aud to play!\n",__FUNCTION__);
            _B2R_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)B2R_CB_FUNC_TRIG_AUD_TO_PLAY, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        else if(rNotifyMsg.u4Type == VDP_MSG_MM_COMM_CB)
        {
            LOG(0,"%s receieve eos!\n",__FUNCTION__);
            _B2R_EventNofify((UCHAR)rNotifyMsg.u4Arg1, (UINT32)B2R_CB_FUNC_EOS, rNotifyMsg.u4Arg2, rNotifyMsg.u4Arg3);
        }
        
        B2R_MUTEX_UNLOCK(this->ucB2rId);
        
    }

    return B2R_OK;

}


#define _B2R_IsRunWithoutHW(o)  (((o)->u4State == 1) && ((o)->u4HWSt == 0))

INT32 _B2R_SendIntMsg(B2R_OBJECT_T *this, UCHAR ucB2rId, UINT32 u4Bottom, UINT32 u4RightView,UINT32 u4IsrType)
{
    B2R_VMSG_T rMsg;
    INT32 i4Ret = B2R_OK;

    do
    {
        if (!this)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }
        
        rMsg.eType              = B2R_VMSG_TYPE_INT;
        rMsg.u.tInt.u4B2rId     = (UINT32) ucB2rId;
        rMsg.u.tInt.u4Bottom    = u4Bottom;
        rMsg.u.tInt.u4IsrType   = u4IsrType;
        rMsg.u.tInt.u4RightView = u4RightView;
        
        i4Ret = x_msg_q_send(this->hMsgQ, (void *)(&rMsg), sizeof(B2R_VMSG_T), 0);
    } while(FALSE);

    return i4Ret;
    
}


INT32 _B2R_SendCmdMsg(B2R_OBJECT_T *this, B2R_VCMD_TYPE_T eType, UINT32 u4Arg)
{
    B2R_VMSG_T rMsg;
    INT32 i4Ret = B2R_OK;

    UNUSED(u4Arg);

    do
    {
        if (!this)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        rMsg.eType = B2R_VMSG_TYPE_CMD;
        rMsg.u.tCmd.eCmd = eType;
        
        i4Ret = x_msg_q_send(this->hMsgQ, (void *)(&rMsg), sizeof(B2R_VMSG_T), 0);
    } while(FALSE);

    return i4Ret;
    
}


static UINT32 _B2R_GetMsg(B2R_OBJECT_T *this, B2R_VMSG_T *ptMsg)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    if (!this || !ptMsg)
    {
        return B2R_INV_ARG;
    }

    zMsgSize = sizeof(B2R_VMSG_T);

    if (_B2R_IsRunWithoutHW(this))
    {
        i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, ptMsg, &zMsgSize, &this->hMsgQ, 1, 20);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, ptMsg, &zMsgSize, &this->hMsgQ, 1, X_MSGQ_OPTION_WAIT);
    }

    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

    if (i4Ret == OSR_TIMEOUT)  // sw interrupte
    {
        ptMsg->eType = B2R_VMSG_TYPE_SINT;
        ptMsg->u.tInt.u4IsrType   = VDP_ISR_ADI;
        ptMsg->u.tInt.u4Bottom    = 0;
        ptMsg->u.tInt.u4RightView = 0;
        i4Ret = OSR_OK;
    }


    if (i4Ret != OSR_OK)
    {
        return B2R_FAIL;
    }


    return B2R_OK;
}

static UINT32 _B2R_ProcCmdMsg(B2R_OBJECT_T *this, B2R_VCMD_T *ptCmd)
{
    if (!this || !ptCmd)
    {
        return B2R_INV_ARG;
    }

    switch(ptCmd->eCmd)
    {
    default:
        break;
    case B2R_VCMD_RUN:
    {
        UINT32 u4Arg;
        this->u4State = 1;

        //if b2r handle is not null and it is enabled, in this case, we do not change hwSt
        if (this->hB2r != B2R_NULL_HANDLE && B2R_HAL_Is(this->hB2r, B2R_HAL_ENABLE_ST, &u4Arg))
        {
            //todo
        }
        else
        {
            this->u4HWSt  = 0;
        }
    }
        break;
    case B2R_VCMD_STOP:
        this->u4State = 0;
        this->u4HWSt  = 0;
        break;
    case B2R_VCMD_CONNECT:
        this->u4HWSt  = 1;
        break;
    case B2R_VCMD_DISCONNECT:
        this->u4HWSt  = 0;
        break;
    }

    LOG(6, "%s @ %d, arg(%x)\n", __FUNCTION__, __LINE__, ptCmd->eCmd);

    return B2R_OK;
}


static UINT32 _B2R_ProcIsrMsg(B2R_OBJECT_T *this, B2R_VINT_T *ptInt)
{
    UCHAR ucB2rId;
    UCHAR ucVdpId;
    UCHAR u1Idx;
    CRIT_STATE_T rState;
    
    if (!this || !ptInt)
    {
        return B2R_INV_ARG;
    }

    ucB2rId = this->ucB2rId;

    ucVdpId = _B2R_GetVdpId(ucB2rId);
    if (ucVdpId >= VDP_NS)
    {
        return B2R_FAIL;
    }


    if (ptInt->u4IsrType & (VDP_ISR_ADI))
    {
        // Normal ISR
        UINT32 u4Event;

        B2R_MUTEX_LOCK(ucB2rId);

        // Frame Rate Control Process
        u4Event = _B2R_FrcProc(this, (UCHAR)(ptInt->u4Bottom),(UCHAR)(ptInt->u4RightView));

        _B2R_HandleEvent(this, u4Event);

        _B2R_IsrFin(this);

        B2R_MUTEX_UNLOCK(ucB2rId);

        // Notify (Upper Layer, and Main Loop) at non-critical session
        _B2R_HandleNotify(this);

        // Callback relevant module
        for (u1Idx=VDP_1; u1Idx<VDP_NS; u1Idx++)
        {
            if (_vdp_vsync_cb[u1Idx] && _prVdpCfg[u1Idx] &&
                (_prVdpCfg[u1Idx]->ucB2rId == ucB2rId))
            {
                rState = x_crit_start();
                _vdp_vsync_cb[u1Idx](u1Idx, _vdp_vsync_cb_tag[u1Idx]);
                x_crit_end(rState);
            }
        }
    }
    else if (ptInt->u4IsrType & (VDP_ISR_VEI))
    {
        // Racing
        B2R_MUTEX_LOCK(ucB2rId);
        _B2R_FrcProcEarly(this, (UCHAR)(ptInt->u4Bottom));
        B2R_MUTEX_UNLOCK(ucB2rId);
    }
    else if (ptInt->u4IsrType & (VDP_ISR_FI))
    {
        ASSERT(0);
    }


    LOG(11, "%s @ %d, arg(%x)\n", __FUNCTION__, __LINE__, ptInt->u4IsrType);

    return B2R_OK;

}

//new b2r thread
void _B2rRoutine(void* pvArg)
{
    B2R_OBJECT_T *this;
    B2R_VMSG_T tMsg;

    this = (B2R_OBJECT_T *)(*(UINT32 *)pvArg);

    if (!this)
    {
        LOG(0, "b2r object null, please check it \n");
        return;
    }

    while (TRUE)
    {
        if (B2R_OK != _B2R_GetMsg(this, &tMsg))
        {
            //todo 
            x_thread_delay(5);
            continue;
        }

        switch(tMsg.eType)
        {
        default:
            ASSERT(0);
            break;
        case B2R_VMSG_TYPE_CMD:
            _B2R_ProcCmdMsg(this, &tMsg.u.tCmd);
            break;
        case B2R_VMSG_TYPE_SINT:
        case B2R_VMSG_TYPE_INT:
            _B2R_ProcIsrMsg(this, &tMsg.u.tInt);
            break;
        }

    }

    UNUSED(pvArg);
}

static void _B2R_IsrCb(VOID *pvData, VOID *pvArg)
{
    B2R_OBJECT_T *this;
    B2R_HAL_STATUS_T *ptStatus;
    UCHAR ucB2rId;

    if (!pvData || !pvArg)
    {
        return;
    }

    this     = (B2R_OBJECT_T *)pvData;
    ptStatus = (B2R_HAL_STATUS_T *)pvArg;
    ucB2rId  = this->ucB2rId;

#ifdef CC_SUPPORT_TVE
    {
    UCHAR bEnable;
    //HAR bEnable;
    // call tve isr, for VBI CC and TTX
    TVE_GetCcEnable(TVE_1, &bEnable);
    if (bEnable)
    {
        TVE_HalSendCcIsr(TVE_1, TVE_GetFieldInfo(TVE_1), 0);
    }
    TVE_GetTTXEnable(TVE_1, &bEnable);
    if (bEnable)
    {
        TVE_HalSendTTXIsr(TVE_1, TVE_GetFieldInfo(TVE_1), 0);
    }
    }
#endif

    _B2R_UpdateStc(this);

    //    LOG(4, "ISR %d 0x%x 0x%x\n", _u4CurrentLineCount, _u4CurrentField, _u4CurrentIsrStatus);

    // Note: Hardware latch status, Top Field = 0, Bottom Field = 1,
    // Note: Current Field = Top, Next Field = Bottom
    // Note: Current Field = Bottom, Next Field = Top
    // Note: Since we are using double buffer, need to prepare frame buffer for next field
    _B2R_SendIntMsg(this, ucB2rId, (ptStatus->u4CurrentField == 0), (ptStatus->u4CurrentLR == 0), ptStatus->u4Status);
}

void _B2R_VSyncInitEx(void)
{
    B2R_OBJECT_T *this;
    B2R_ICfG_T tCfg;
    UINT32 i;
    UINT32 Idx;

    B2R_HAL_Init();


    for (Idx = 0; Idx < B2R_NS; Idx++)
    {
        UCHAR acName[16];
        HANDLE_T hThread;
        UINT32 arg;

        this = _B2R_GetObj(Idx);
        if(!this)
        {
            return;
        }
        
        this->pvData  = (VOID *)this;
        this->IsrCb   = _B2R_IsrCb;

        // create Message Queue (VSYNC)
        x_snprintf(acName, 16, "VSYNCQ%2x", Idx);
        VERIFY(x_msg_q_create(&this->hMsgQ, acName, sizeof(B2R_VMSG_T), VDP_MSG_Q_NUM) == OSR_OK);
        x_snprintf(acName, 16, "V-NOTIFY%2x", Idx);
        VERIFY(x_msg_q_create(&this->hNotifyMsgQ, acName, sizeof(VDP_CB_MSG_T), VDP_NOTIFY_Q_NUM) == OSR_OK);

        this->u4State = 0;
        this->u4HWSt  = 0;
#ifndef NEW_B2R_SINGLE_TEST
        tCfg.u4Data = 0xFFFFFFFF - Idx;
#else
        tCfg.u4Data = 0xFFFFFFFD - Idx;
#endif
        tCfg.pvData = (VOID *)this;
        this->hB2r  = B2R_HAL_Open(&tCfg);
        this->u4VideoPanelNs = VDP_NS;
        for (i = 0; i < VDP_NS; i++)
        {
            this->avVideoPanel[i] = NULL;
        }

        // create thread
        x_snprintf(acName, 16, "%s%2x", VSYNC_THREAD_NAME, Idx);
        arg = (UINT32)this;
        #if defined(CC_HIGH_B2R_THREAD_PRIORITY) //customer kr 3d case
        if (x_thread_create(&hThread, acName, VSYNC_STACK_SIZE, 5, _B2rRoutine, sizeof(UINT32), (VOID*)&arg) != OSR_OK)
        {
            ASSERT(0);
        }
        #else
        if (x_thread_create(&hThread, acName, VSYNC_STACK_SIZE, VSYNC_THREAD_PRIORITY, _B2rRoutine, sizeof(UINT32), (VOID*)&arg) != OSR_OK)
        {
            ASSERT(0);
        }
        #endif
        
    }

}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _B2R_VsyncInit.
 */
//-----------------------------------------------------------------------------
void _B2R_VsyncInit(void)
{
    if (_ucVdpVsyncInitiated == 0)
    {
        UINT32 u4VdpIdx;
        UINT32 u4B2rIdx;

        _ucVdpVsyncInitiated = 1;

        for (u4VdpIdx = 0; u4VdpIdx < VDP_NS; u4VdpIdx++)
        {
            _prVdpCfg[u4VdpIdx] = _B2R_GetVdpConf((UCHAR)u4VdpIdx);
            _u4VdpVsyncOnOff[u4VdpIdx] = 1;
        }

        // create semaphore
        for(u4B2rIdx = 0; u4B2rIdx < B2R_NS; u4B2rIdx ++)
        {
            VERIFY(x_sema_create(&_ahDtvMutex[u4B2rIdx], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        }

        _B2R_FrcInit();
        _B2R_VSyncInitEx();

    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_VsyncReset.
 */
//-----------------------------------------------------------------------------
void _B2R_VsyncReset(UCHAR ucB2rId)
{
    B2R_OBJECT_T *this;
    this = _B2R_GetObj(ucB2rId);

    if (this)
    {
        B2R_HAL_Reset(this->hB2r);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_VsyncOnOff.
 */
//-----------------------------------------------------------------------------
void _B2R_VsyncOnOff(UCHAR ucB2rId, UINT32 u4OnOff)
{
    if (ucB2rId < B2R_NS)
    {
        _u4VdpVsyncOnOff[ucB2rId] = u4OnOff;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_VsyncSendCmd.
 */
//-----------------------------------------------------------------------------
void _B2R_VsyncSendCmd(UCHAR ucB2rId, UCHAR ucCmdId)
{
    // handle API command immediately
    UINT32 u4Event;

    if ((ucCmdId < VDP_CMD_MAX) && (ucB2rId < B2R_NS))
    {
        B2R_MUTEX_LOCK(ucB2rId);

        u4Event = _apfCmdHandle[ucCmdId](ucB2rId);

        if (u4Event)
        {
            u4Event |= _B2R_FrcHandlerEvent(ucB2rId, u4Event);
        }

        B2R_MUTEX_UNLOCK(ucB2rId);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_VsyncMutexLock.
 */
//-----------------------------------------------------------------------------
void _B2R_VsyncMutexLock(UCHAR ucB2rId)
{
    if (ucB2rId < B2R_NS)
    {
        VERIFY(x_sema_lock(_B2R_GetLock(ucB2rId), X_SEMA_OPTION_WAIT) == OSR_OK);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_VsyncMutexUnlock.
 */
//-----------------------------------------------------------------------------
void _B2R_VsyncMutexUnlock(UCHAR ucB2rId)
{
    if (ucB2rId < B2R_NS)
    {
        VERIFY(x_sema_unlock(_B2R_GetLock(ucB2rId)) == OSR_OK);
    }
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
BOOL VDP_VsyncCBRegister(UCHAR ucVdpId, VDP_VSYNC_CB_FUNC VDP_VsyncCB, VOID* pv_tag)
{
    if (ucVdpId >= VDP_NS)
    {
        return FALSE;
    }

    _vdp_vsync_cb[ucVdpId] = VDP_VsyncCB;
    _vdp_vsync_cb_tag[ucVdpId] = pv_tag;

    return TRUE;
}
