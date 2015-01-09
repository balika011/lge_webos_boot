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
 * $RCSfile: fbm_inst_drvif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_inst_drvif.c
 *  This file contains implementation of exported APIs of FBM.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "fbm_inst_if.h"
#include "fbm_inst_drvif.h"
#include "fbm_pool.h"
#include "fbm_cs.h"
#include "fbm_debug.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_syslog.h"
#include "x_timer.h"
#include "x_bim.h"
#include "drvcust_if.h"
#include "vdec_drvif.h"
#include "gfx_drvif.h"
LINT_EXT_HEADER_END


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
UINT32 _FBM_QueryBufNum(UINT32 u4VDecFmt, UINT32 u4Width, UINT32 u4Height,
    UCHAR ucColorMode, UINT32 *pu4BufNum, UINT32 *pu4Stride,
    UINT32 *pu4SliceHeight, UINT32 *pu4UVAddrOffset, UINT32 u4ExtOption);


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
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static FBM_INST_T _arFBMInst[FBM_INST_MAX_NS];
static HANDLE_T _hInstMutex = (HANDLE_T)(NULL);
static UINT32 _u4NextInstId = 0;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static UINT32 _GetFBMVecFormat(FBM_VDEC_FORMAT format)
{
    switch(format)
    {
        case FBM_H264:
            return FBM_VDEC_H264;
        case FBM_MPEG2:
            return FBM_VDEC_MPEG2;
        case FBM_MPEG4:
            return FBM_VDEC_MPEG4;
        case FBM_VC1:
            return FBM_VDEC_VC1;
        case FBM_JPEG:
            return FBM_VDEC_JPEG;
        case FBM_RV:
            return FBM_VDEC_RV;
        case FBM_AVS:
            return FBM_VDEC_AVS;
        case FBM_RAW:
            return FBM_VDEC_RAW;
        case FBM_VP6:
            return FBM_VDEC_VP6;
        case FBM_VP8:
            return FBM_VDEC_VP8;
        case FBM_H265:
            return FBM_VDEC_H265;            
        default:
            LOG(0, "Unexpected video format!\n");
            ASSERT(0);
            return FBM_VDEC_UNKNOWN;
    }
}

static UCHAR _GetFBMColorMode(FBM_ColorFMT_T cm)
{
    switch(cm)
    {
        case FBM_CFMT_420:
            return FBM_CM_420;
        case FBM_CFMT_422:
            return FBM_CM_422;
        case FBM_CFMT_444:
            return FBM_CM_444;
        case FBM_CFMT_ARGB:
            return FBM_CM_RGB32;
        default:
            LOG(0, "Unexpected Color Mode!\n");
            ASSERT(0);
            return FBM_CM_420;
    }
}

#if 0 // liuqu mark to aovid build error!20121203
//-------------------------------------------------------------------------
/** _FBMInstStatus
 *  Print FBM Inst status
 *  @return NONE.
 */
//-------------------------------------------------------------------------
static void _FBMInstStatus(UCHAR ucInstId)
{
/*
    UCHAR ucIdx;

    if (ucFbgId >= FBG_MAX_NS)
    {
        return;
    }

    if ((_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_UNKNOWN) ||
            (_arFbg[ucFbgId].ucFbgId != ucFbgId))
    {
        LOG(1, "FBG(%d) is NULL!\n", ucFbgId);
        return;
    }

    LOG(1, "FBG(%d) Type(%d) CM(%d) Ns(%d) D(%d) F(%d) B(%d) Pool(0x%x, 0x%x)\n",
        _arFbg[ucFbgId].ucFbgId,
        _arFbg[ucFbgId].ucFbgType,
        _arFbg[ucFbgId].ucFbgCm,
        _arFbg[ucFbgId].ucFbNs,
        _arFbg[ucFbgId].ucFbDecode,
        _arFbg[ucFbgId].ucFbFRef,
        _arFbg[ucFbgId].ucFbBRef,
        _arFbg[ucFbgId].u4FbMemoryPool,
        _arFbg[ucFbgId].u4FbMemoryPoolSize);

    for (ucIdx = 0; ucIdx < _arFbg[ucFbgId].ucFbNs; ucIdx++)
    {
        if(ucIdx < FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(1, "\tFB(%d) S(%d) Y(0x%x) C(0x%x)\n",
                ucIdx,
                _arFbg[ucFbgId].aucFbStatus[ucIdx],
                _arFbg[ucFbgId].au4AddrY[ucIdx],
                _arFbg[ucFbgId].au4AddrC[ucIdx]);
        }
    }
*/    
}
#endif


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

void FBM_Inst_Init(void)
{
    UINT32 u4Idx;
    
    x_memset((void*)_arFBMInst, 0, (sizeof(FBM_INST_T) * FBM_INST_MAX_NS));
    VERIFY(x_sema_create(&_hInstMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

    for (u4Idx = 0; u4Idx < FBM_INST_MAX_NS; u4Idx++)
    {
        VERIFY(x_sema_create(&_arFBMInst[u4Idx].hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
        
        _arFBMInst[u4Idx].ucInstCm = FBM_CM_420;
        _arFBMInst[u4Idx].ucInstMode = FBM_INST_TYPE_ALLOC;
        _arFBMInst[u4Idx].ucFbgId = FBM_FBG_ID_UNKNOWN;
        _arFBMInst[u4Idx].u4VDecFmt = FBM_VDEC_UNKNOWN;
        _arFBMInst[u4Idx].ucInstId = FBM_INST_ID_UNKNOWN;
    }
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

UCHAR_T FBM_GetInstance(FBM_VDEC_FORMAT eVDecFormat, UINT32_T u4Width, UINT32_T u4Height, FBM_ColorFMT_T eColorMode, UINT32_T u4FrameNum, BOOL_T fgAlloc, UINT32_T u4ExtOption)
{
    UINT32 u4InstId = FBM_INST_ID_UNKNOWN;
    UINT32 u4InstIdx;
    UCHAR ucPoolType;
    FBM_SEQ_HDR_T* prFbmSeqHdr =NULL;

    VERIFY(x_sema_lock(_hInstMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    for (u4InstIdx = 0; u4InstIdx < FBM_INST_MAX_NS; u4InstIdx++)
    {
        if (_arFBMInst[_u4NextInstId].ucInstId == FBM_INST_ID_UNKNOWN)
        {
            u4InstId = _u4NextInstId;

            if (++_u4NextInstId >= FBM_INST_MAX_NS)
            {
                _u4NextInstId = 0;
            }
            break;
        }

        if (++_u4NextInstId >= FBM_INST_MAX_NS)
        {
            _u4NextInstId = 0;
        }
    }
    if (u4InstId == FBM_INST_ID_UNKNOWN)
    {
        VERIFY(x_sema_unlock(_hInstMutex) == OSR_OK);
        return FBM_INST_ID_UNKNOWN;
    }

    _arFBMInst[u4InstId].ucInstCm = _GetFBMColorMode(eColorMode);
    _arFBMInst[u4InstId].ucInstMode = (fgAlloc ? FBM_INST_TYPE_ALLOC : FBM_INST_TYPE_SET);
    _arFBMInst[u4InstId].u4FbWidth = u4Width;
    _arFBMInst[u4InstId].u4FbHeight = u4Height;
    _arFBMInst[u4InstId].u4VDecFmt = _GetFBMVecFormat(eVDecFormat);

    if (fgAlloc)
    {
        UINT32 i = 0;
        FBM_CREATE_FBG_PAR_T rPar;
        
        ASSERT(u4FrameNum <= FBM_MAX_FB_NS_PER_GROUP);
        _arFBMInst[u4InstId].ucFbNs = u4FrameNum;

        FBM_SetColorMode(_arFBMInst[u4InstId].ucInstCm);
        ucPoolType = FBM_SelectGroupType(u4Width, u4Height);

        
        x_memset(&rPar, 0, sizeof(FBM_CREATE_FBG_PAR_T));
        rPar.u1VdecId = (UINT8)ES0;  // TODO: fix it.
        rPar.u1AppMode = FBM_FBG_APP_OMX_DISP;
        if(265 == u4ExtOption)
            rPar.fgSwHEVC = TRUE;
        
        _arFBMInst[u4InstId].ucFbgId = FBM_CreateGroupExt(ucPoolType, _arFBMInst[u4InstId].u4VDecFmt, u4Width, u4Height, &rPar);

        if (_arFBMInst[u4InstId].ucFbgId == FBM_FBG_ID_UNKNOWN)
        {
            VERIFY(x_sema_unlock(_hInstMutex) == OSR_OK);
            return FBM_INST_ID_UNKNOWN;
        }

        if(265 == u4ExtOption)
        {            
			//send sequence change event
            FBM_SetSyncStc(_arFBMInst[u4InstId].ucFbgId,AV_SYNC_MODE_NONE,0);
		    FBM_FbgChgNotify(_arFBMInst[u4InstId].ucFbgId,0);
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_arFBMInst[u4InstId].ucFbgId);
            if(prFbmSeqHdr == NULL)
            {
                ASSERT(prFbmSeqHdr != NULL);
                return FBM_INST_ID_UNKNOWN;
            }
            prFbmSeqHdr->u2HSize = u4Width;
            prFbmSeqHdr->u2VSize = u4Height;
            prFbmSeqHdr->u2OrgHSize = prFbmSeqHdr->u2HSize;
            prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
            //prFbmSeqHdr->u2LineSize = MAX_WIDTH;
            prFbmSeqHdr->u2LineSize = prFbmSeqHdr->u2HSize;
            prFbmSeqHdr->fgProgressiveSeq = TRUE;
            prFbmSeqHdr->fgRasterOrder = TRUE;

            FBM_SetFrameBufferFlag(_arFBMInst[u4InstId].ucFbgId, FBM_FLAG_SEQ_CHG_SPEEDUP);

        }

        ASSERT(u4FrameNum == FBM_GetFrameBufferNs(_arFBMInst[u4InstId].ucFbgId));

        //reset
        for (i = 0; i < FBM_MAX_FB_NS_PER_GROUP; i++)
        {
            _arFBMInst[u4InstId].aucFbStatus[i] = FBM_FB_STATUS_UNKNOWN;
        }
        //update fb array.
        for (i = 0; i < u4FrameNum; i++)
        {
            _arFBMInst[u4InstId].aucFbStatus[i] = FBM_FB_STATUS_EMPTY;
            FBM_GetFrameBufferAddr(_arFBMInst[u4InstId].ucFbgId, i, &(_arFBMInst[u4InstId].au4AddrY[i]), &(_arFBMInst[u4InstId].au4AddrC[i]));
        }
    }
    else
    {
        _arFBMInst[u4InstId].ucFbNs = 0;
        _arFBMInst[u4InstId].fgOccupied = FALSE;
        for (u4InstIdx = 0; u4InstIdx < FBM_MAX_FB_NS_PER_GROUP; u4InstIdx++)
        {
            _arFBMInst[u4InstId].aucFbStatus[u4InstIdx] = FBM_FB_STATUS_UNKNOWN;
        }
    }

    _arFBMInst[u4InstId].ucInstId = u4InstId;
    VERIFY(x_sema_unlock(_hInstMutex) == OSR_OK);
    return u4InstId;
}

UINT32_T FBM_FreeInstance(UCHAR_T ucInstId)
{
    VERIFY(x_sema_lock(_hInstMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    if (VERIFY_INST(ucInstId))
    {
        VERIFY(x_sema_unlock(_hInstMutex) == OSR_OK);
        return FBM_INVALID_ARGUMENT;
    }
    FBM_INST_MUTEX_LOCK(ucInstId);

    if (_arFBMInst[ucInstId].ucInstMode == FBM_INST_TYPE_ALLOC)
    {
        UINT32 i;
        for (i = 0; i < FBM_MAX_FB_NS_PER_GROUP; i++)
        {
            if(_arFBMInst[ucInstId].aucFbStatus[i] == FBM_FB_STATUS_LOCK)
            {
                LOG(1, "Warning: inst(%d) type FBM_INST_TYPE_ALLOC doesn't free all buf before free inst.\n", ucInstId);
            }
        }

        FBM_ReleaseGroup(_arFBMInst[ucInstId].ucFbgId);
    }
    else
    {
        if (_arFBMInst[ucInstId].ucFbNs != 0)
        {
            LOG(1, "Warning: inst(%d) type FBM_INST_TYPE_SET doesn't unset all buf before free.\n", ucInstId);
        }

        if (_arFBMInst[ucInstId].fgOccupied == TRUE)
        {
            LOG(1, "Warning: inst(%d) type FBM_INST_TYPE_SET is still occupied!\n", ucInstId);
        }
    }

    _arFBMInst[ucInstId].ucInstId = FBM_INST_ID_UNKNOWN;

    FBM_INST_MUTEX_UNLOCK(ucInstId);
    VERIFY(x_sema_unlock(_hInstMutex) == OSR_OK);
    return FBM_STATUS_OK;
}

UINT32_T FBM_FreeBuf(UCHAR_T ucInstId, UINT32_T u4Addr)
{
    UINT32 i, u4Ret = FBM_STATUS_OK;

    if (VERIFY_INST(ucInstId))
    {
        return (ucInstId < FBM_INST_MAX_NS) ? FBM_STATUS_OK : FBM_INVALID_ARGUMENT;
    }

    FBM_INST_MUTEX_LOCK(ucInstId);

    if (_arFBMInst[ucInstId].ucFbNs == 0)
    {
        u4Ret = FBM_UNEXPECTED_ERROR;
    }
    else
    {
        for (i = 0; i < FBM_MAX_FB_NS_PER_GROUP; i++)
        {
            if(_arFBMInst[ucInstId].aucFbStatus[i] == FBM_FB_STATUS_LOCK && u4Addr == _arFBMInst[ucInstId].au4AddrY[i])
            {
                _arFBMInst[ucInstId].aucFbStatus[i] = FBM_FB_STATUS_EMPTY;
                break;
            }
        }
    }

    FBM_INST_MUTEX_UNLOCK(ucInstId);
    return u4Ret;
}

UINT32_T FBM_AllocBuf(UCHAR_T ucInstId, UINT32_T *pu4Addr)
{
    UINT32 i, u4Ret = FBM_STATUS_OK;
    
    if (VERIFY_INST(ucInstId) || pu4Addr == NULL)
    {
        return FBM_INVALID_ARGUMENT;
    }

    FBM_INST_MUTEX_LOCK(ucInstId);

    if (_arFBMInst[ucInstId].ucFbNs == 0)
    {
        u4Ret = FBM_UNEXPECTED_ERROR;
    }
    else
    {
        for (i = 0; i < FBM_MAX_FB_NS_PER_GROUP; i++)
        {
            if(_arFBMInst[ucInstId].aucFbStatus[i] == FBM_FB_STATUS_EMPTY)
            {
                _arFBMInst[ucInstId].aucFbStatus[i] = FBM_FB_STATUS_LOCK;
                *pu4Addr = _arFBMInst[ucInstId].au4AddrY[i];
                break;
            }
        }
    }

    FBM_INST_MUTEX_UNLOCK(ucInstId);
    return u4Ret;
}

UINT32_T FBM_Alloc_YCBuf(UCHAR_T ucInstId, UINT32_T *pu4AddrY,UINT32_T *pu4AddrC)
{
    UINT32 i, u4Ret = FBM_STATUS_OK;
    
    if (VERIFY_INST(ucInstId) || pu4AddrY == NULL || pu4AddrC == NULL)
    {
        return FBM_INVALID_ARGUMENT;
    }

    FBM_INST_MUTEX_LOCK(ucInstId);

    if (_arFBMInst[ucInstId].ucFbNs == 0)
    {
        u4Ret = FBM_UNEXPECTED_ERROR;
    }
    else
    {
        for (i = 0; i < FBM_MAX_FB_NS_PER_GROUP; i++)
        {
            if(_arFBMInst[ucInstId].aucFbStatus[i] == FBM_FB_STATUS_EMPTY)
            {
                _arFBMInst[ucInstId].aucFbStatus[i] = FBM_FB_STATUS_LOCK;
                *pu4AddrY = _arFBMInst[ucInstId].au4AddrY[i];
                *pu4AddrC = _arFBMInst[ucInstId].au4AddrC[i];
                break;
            }
        }
    }

    FBM_INST_MUTEX_UNLOCK(ucInstId);
    return u4Ret;
}


UINT32_T FBM_QueryBufNum(FBM_VDEC_FORMAT eVDecFormat, UINT32_T u4Width,
    UINT32_T u4Height, FBM_ColorFMT_T eColorMode, UINT32_T *pu4BufNum,
    UINT32_T *pu4Stride, UINT32_T *pu4SliceHeight,
    UINT32_T *pu4UVAddrOffset, UINT32_T u4ExtOption)
{
    return _FBM_QueryBufNum(_GetFBMVecFormat(eVDecFormat), u4Width, u4Height,
        _GetFBMColorMode(eColorMode), (UINT32 *)pu4BufNum, (UINT32 *)pu4Stride, (UINT32 *)pu4SliceHeight,
        (UINT32 *)pu4UVAddrOffset, u4ExtOption);
}

UINT32 _FBM_QueryBufNum(UINT32 u4VDecFmt, UINT32 u4Width, UINT32 u4Height,
    UCHAR ucColorMode, UINT32 *pu4BufNum, UINT32 *pu4Stride,
    UINT32 *pu4SliceHeight, UINT32 *pu4UVAddrOffset, UINT32 u4ExtOption)
{
    UCHAR ucPoolType = 0;
    UCHAR ucFbpListNs = 0;
    UINT32 u4PoolSize = 0;
    UINT32 u4ListIdx = 0;
    FBP_LIST_T* prFbpList = NULL;
    UINT32 u4YSize = 0, u4CSize = 0;
    UCHAR ucFBMCm = ucColorMode;

    if (pu4BufNum == NULL || pu4Stride == NULL ||
        pu4SliceHeight == NULL || pu4UVAddrOffset == NULL)
    {
        return FBM_INVALID_ARGUMENT;
    }

    _FBM_GetPoolList(&prFbpList, &ucFbpListNs);

    ucPoolType = FBM_SelectGroupType(u4Width, u4Height);

    // Check frame buffer group type
#ifdef CC_SUPPORT_TV_STAGEFRIGHT_4K2K 
    if(ucPoolType == FBM_FBG_TYPE_4K2K)  // 4K2K PoolSize should add PART1+PART2
    {
        for (u4ListIdx = 0; u4ListIdx < ucFbpListNs; u4ListIdx++)
        {
            if ((FBM_FBG_TYPE_4K2K == prFbpList[u4ListIdx].ucType) || (FBM_FBG_TYPE_4K2K_EXTRA == prFbpList[u4ListIdx].ucType))
            {
                u4PoolSize += prFbpList[u4ListIdx].u4Size;
                u4YSize = prFbpList[u4ListIdx].u4YSize;
                u4CSize = prFbpList[u4ListIdx].u4CSize;
                //break; //no break here
            }
        }
    }
    else
#endif
    {
        for (u4ListIdx = 0; u4ListIdx < ucFbpListNs; u4ListIdx++)
        {
            if (ucPoolType == prFbpList[u4ListIdx].ucType)
            {
                u4PoolSize = prFbpList[u4ListIdx].u4Size;
                u4YSize = prFbpList[u4ListIdx].u4YSize;
                u4CSize = prFbpList[u4ListIdx].u4CSize;
                break;
            }
        }
    }

    if (u4PoolSize == 0)
    {
        return FBM_UNEXPECTED_ERROR;
    }

    if (ucFBMCm == FBM_CM_422)
    {
        u4CSize *= 2;
    }
    else if (ucFBMCm == FBM_CM_444)
    {
        u4CSize *= 4;
    }
    else if (ucFBMCm == FBM_CM_RGB32)
    {
        u4CSize = 0;
        u4YSize *= 4;
    }
    *pu4UVAddrOffset = u4YSize;

    *pu4Stride = FBM_ALIGN_MASK(u4Width, FBM_B2R_H_PITCH);
    *pu4SliceHeight = FBM_ALIGN_MASK(u4Height, FBM_MPV_V_ALIGMENT);

    *pu4BufNum = FBM_CalcBufNum(FBM_FBG_ID_UNKNOWN, ucPoolType, u4VDecFmt, u4Width, u4Height, u4PoolSize, u4YSize, u4CSize, FBM_FBG_APP_OMX_DISP, NULL);
    
    return FBM_STATUS_OK;
}

void FBM_GetInstBufAddr(UCHAR ucInstId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC)
{
    if ((pu4AddrY == NULL) || (pu4AddrC == NULL))
    {
        return;
    }

    *pu4AddrY = (UINT32)NULL;
    *pu4AddrC = (UINT32)NULL;

    if (VERIFY_INST(ucInstId))
    {
        return;
    }

    FBM_INST_MUTEX_LOCK(ucInstId);

    if (ucFbId>=FBM_MAX_FB_NS_PER_GROUP || ucFbId >= _arFBMInst[ucInstId].ucFbNs)
    {
        ASSERT(0);
        return;
    }

    *pu4AddrY = _arFBMInst[ucInstId].au4AddrY[ucFbId];
    *pu4AddrC = _arFBMInst[ucInstId].au4AddrC[ucFbId];

    FBM_INST_MUTEX_UNLOCK(ucInstId);
}

UINT32_T FBM_GetInstInfo(UCHAR_T ucInstId, FBM_INST_INFO_TYPE eInfoType, UINT32_T *pInfoResult)
{
	UINT32 u4YSize = 0;
	UINT32 u4CSize = 0;
	
    if (VERIFY_INST(ucInstId) || pInfoResult == NULL)
    {
        return FBM_INVALID_ARGUMENT;
    }

    FBM_INST_MUTEX_LOCK(ucInstId);

    switch(eInfoType)
    {
        case FBM_FRAME_SIZE:
            if (_arFBMInst[ucInstId].ucInstMode == FBM_INST_TYPE_ALLOC)
            {
            	u4YSize = _arFBMInst[ucInstId].au4AddrY[1] - _arFBMInst[ucInstId].au4AddrY[0];
				u4CSize = _arFBMInst[ucInstId].au4AddrC[1] - _arFBMInst[ucInstId].au4AddrC[0];
				
                *pInfoResult = u4YSize + u4CSize;
            }
            else
            {
                ASSERT(0);
                *pInfoResult = 0; // TODO: Fix it.
            }
            break;
        case FBM_FRAME_COUNT:
            *pInfoResult = _arFBMInst[ucInstId].ucFbNs;
            break;
        default:
            break;
    };

    FBM_INST_MUTEX_UNLOCK(ucInstId);
    return FBM_STATUS_OK;
}

