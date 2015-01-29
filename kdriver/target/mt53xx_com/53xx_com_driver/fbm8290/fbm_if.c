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
 * $Date: 2015/01/29 $
 * $RCSfile: fbm_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_if.c
 *  This file contains implementation of exported APIs of FBM.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "fbm_drvif.h"
#include "fbm_inst_if.h"
#include "fbm_inst_drvif.h"
#include "fbm_fb.h"
#include "fbm_pool.h"
#include "fbm_pool_config.h"
#include "fbm_cs.h"
#include "fbm_debug.h"
#include "vdp_prescale_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_syslog.h"
#include "x_timer.h"
#include "x_bim.h"
#include "drvcust_if.h"
#include "vdec_drvif.h"
#include "b2r_drvif.h"
#include "gfx_drvif.h"
#include "gdma_if.h"
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

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    BOOL fgInUse;
} FBM_SEAMLESS_T;

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    BOOL fgInUse;
} FBM_IMGRZ_TMP_BUF_T;


#if defined(CC_FBM_TWO_4K2K)||defined(CC_FBM_FOUR_INST_SUPPORT)
#define MAX_SEAMLESS_BUFF_COUNT 4
#elif defined(CC_FBM_TWO_FBP)||defined(CC_FBM_SUPPORT_4K2K)
#define MAX_SEAMLESS_BUFF_COUNT 2
#else
#define MAX_SEAMLESS_BUFF_COUNT 1
#endif
static BOOL fgPipLine =TRUE;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define VERIFY_FBG(id)                                  \
    ((id >= FBG_MAX_NS) ||                              \
    (_arFbg[id].ucFbgType == FBM_FBG_TYPE_UNKNOWN) ||   \
    (_arFbg[id].ucFbgId != id))

#define VERIFY_FB(gid, id)                              \
    (!(((_arFbg[gid].ucFbNsBase <= id) && (id < _arFbg[gid].ucFbNs))   ||  \
    ((_arFbg[gid].ucFbNsOldBase <= id) && (id < _arFbg[gid].ucFbNsOld)))   || \
    (id >= FBM_MAX_FB_NS_PER_GROUP))

#define VERIFY_FB_NS(gid)                               \
    (_arFbg[gid].ucFbNs - _arFbg[gid].ucFbNsBase > FBM_MAX_FB_NS_PER_GROUP/2)

#define IS_REFERENCE_FB(gid, id)                        \
    ((id == _arFbg[gid].ucFbFRef) || (id == _arFbg[gid].ucFbBRef))

#define NOT_REFERENCE_FB(gid, id)                        \
    ((id != _arFbg[gid].ucFbFRef) && (id != _arFbg[gid].ucFbBRef))

#ifdef CC_VDP_FULL_ISR

#define FBM_MUTEX_LOCK(id)                              \
    (*_arFbg[id].prState = x_crit_start())

#define FBM_MUTEX_UNLOCK(id)                            \
    (x_crit_end(*_arFbg[id].prState))

#else

#define FBM_MUTEX_LOCK(id)                              \
    (VERIFY(x_sema_lock(_arFbg[id].hMutex, X_SEMA_OPTION_WAIT) == OSR_OK))

#define FBM_MUTEX_UNLOCK(id)                            \
    (VERIFY(x_sema_unlock(_arFbg[id].hMutex) == OSR_OK))

#endif


#define FBM_MEMSET(addr, value, len)                    \
    (VERIFY(x_memset((addr), value, len) == (addr)))

#define FBM_BYTE(value)                                 \
    ((UCHAR)(value & 0xFF))

#define FBM_GFX_MEMSET(u4Addr, u4Width, u4Height, u4Color)    \
    if (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_GFX_MEMSET])) \
    {\
        ((FBM_GFX_MEMSET_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET])((UINT8*)u4Addr, u4Width, u4Height, u4Color);\
    }

#define FBM_GFX_MEMSET_ASYNC(u4Addr, u4Width, u4Height, u4Color)    \
    ((FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_GFX_TASK_MEMSET])) ?\
    (((FBM_GFX_TASK_MEMSET_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET])((UINT8*)u4Addr, u4Width, u4Height, u4Color)) : 0)
    
#define FBM_GDMA_MEMSET(s,c,n)    \
	if((_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GDMA_MEMSET])) \
	{\
        ((FBM_GDMA_MEMSET_FUNC)(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GDMA_MEMSET]))(s,c,n);\
	}

#define FBM_GDMA_MEMSET_ASYNC(s,c,n)    \
    ((FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_GDMA_TASK_MEMSET])) ?\
    (((FBM_GDMA_TASK_MEMSET_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET])(s,c,n)) : 0)
	
#define FBM_Check_AddrIsOverLap(u4Addr,u4Size,u4StartAddr,u4EndAddr)    \
    if((u4Addr >= u4StartAddr) && \
       (u4Addr <= u4EndAddr) && \
       (u4Addr+u4Size >= u4StartAddr) && \
       (u4Addr+u4Size <= u4EndAddr)) \
    {\
     \
    }\
    else \
    {\
        LOG(1,"Addr(0x%x) u4Size(0x%x) layout overlap!! \n",u4Addr,u4Size);\
    }
    
//If ns >= 10, return 10; else if ns >= 5, return 5; else return ns;
#define FBM_FLOOR_FIX(ns, a, b) (((ns)>=(a)) ? (a) : (((ns)>=(b)) ? (b) : (ns)))
#define FBM_MIN(a, b) ((a) < (b) ? (a) : (b))

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

extern UINT32 _FBM_QueryBufNum(UINT32 u4VDecFmt, UINT32 u4Width,
    UINT32 u4Height, UCHAR ucColorMode, UINT32 *pu4BufNum, UINT32 *pu4Stride,
    UINT32 *pu4SliceHeight, UINT32 *pu4UVAddrOffset, UINT32 u4ExtOption);
#ifdef FBM_CS_LOG
extern void _FbmCsLog(UINT32 u4Log, INT32 i4Count, UINT32 u4handle);
#endif

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _FbgStatus(UCHAR ucFbgId);
static UINT32 FBM_PreConfigColorMode(UCHAR ucFbgId, UCHAR ucFbgCm, UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar);
static void _DetectNewSeamless(UINT32 u4FbgId, UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar);
static void _InitSeamlessBuffer(void);
static void _FreeSeamlessBuffer(UINT32 u4BufferAddr);
static void _GetSeamlessBuffer(UINT32 *pu4BufferAddr, UINT32 *pu4BufferSize);
static void _QueryFixedBuffer(UINT32 *pu4BufferAddr, UINT32 *pu4BufferSize);
static void _ActiveFHDPool(UINT32 u4FbgId,UCHAR ucFbgType,UINT32 u4FixedSize);

#ifdef FBM_4K2K_SUPPORT
static void _Active4k2kPool(UINT32 u4FbgId,UCHAR ucFbgType,UINT32 u4FixedSize);
#endif

void _EnableNewSeamless(UINT32 u4FbgId);
#ifdef CC_B2R_RES_SUPPORT
void _FBM_B2rResInit(void);
void _FBM_B2rResGetSrcInfo(UCHAR ucFbgId,
                                     B2R_IPT_INFO_T* pt_src,
                                     FBM_CREATE_FBG_PAR_T* ptPar);
UINT8 FBM_B2rResIdRelease(UCHAR ucFbgId);
#endif



//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

// Initiated Protection
static UCHAR _FbmInitiated = 0;

#ifndef FBM_VIRTUAL_MPEG
// frame buffer group
static FBM_FBG_T _arFbg[FBG_MAX_NS];
#else
FBM_FBG_T _arFbg[FBG_MAX_NS];
#endif

static FBM_SEAMLESS_T _arSeamlessInfo[MAX_SEAMLESS_BUFF_COUNT];

static FBM_IMGRZ_TMP_BUF_T _arImgRzTmpBufInfo[MAX_SEAMLESS_BUFF_COUNT];

static UINT32 _u4NextFbgId = 0;
static UINT8 _u1FbgColorMode = FBM_CM_420;

static BOOL _fgSetDecoderFb = FALSE;
static UINT8 _u1DecoderType = FBM_VDEC_UNKNOWN;
static UINT8 _u1NumOfFb = 4;

static FBM_POOL_T *_prPool = NULL;

static FBM_CB_FUNC_T _arFbmCbFunc;

#ifdef CC_B2R_RES_SUPPORT
static B2R_HW_RES_T _arB2rHwRes[B2R_HW_MAX_ID];
#endif

#ifdef CC_VDP_FULL_ISR
static CRIT_STATE_T _rState;
#endif

#ifdef FBM_CS_LOG
UINT32 _u4EQHandle = 0;
#endif

static HANDLE_T _hFbgMutex = (HANDLE_T)(NULL);

EXTERN void _VdpCheckFbgReady(UCHAR ucFbgId, UCHAR ucEsId);

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _FbgStatus
 *  Print FBM status
 *  @return NONE.
 */
//-------------------------------------------------------------------------
static void _FbgStatus(UCHAR ucFbgId)
{
    UCHAR ucIdx;
    UCHAR ucDispTag;

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

      LOG(0, "FBG(%d) Type(%d) CM(%d) Ns(%d) D(%d) F(%d) B(%d) Pool(0x%x, 0x%x) \n",
        _arFbg[ucFbgId].ucFbgId,
        _arFbg[ucFbgId].ucFbgType,
        _arFbg[ucFbgId].ucFbgCm,
        _arFbg[ucFbgId].ucFbNs,
        _arFbg[ucFbgId].ucFbDecode,
        _arFbg[ucFbgId].ucFbFRef,
        _arFbg[ucFbgId].ucFbBRef,
        _arFbg[ucFbgId].u4FbMemoryPool,
        _arFbg[ucFbgId].u4FbMemoryPoolSize);
      LOG(0,"Resolution(0x%x,0x%x),Pitch(0x%x),Raster(%d),422Mode(%d)\n",
        _arFbg[ucFbgId].rSeqHdr.u2HSize,
        _arFbg[ucFbgId].rSeqHdr.u2VSize,
        _arFbg[ucFbgId].rSeqHdr.u2LineSize,
        _arFbg[ucFbgId].rSeqHdr.fgRasterOrder,
        _arFbg[ucFbgId].rSeqHdr.fg422Mode);

    for(ucIdx = _arFbg[ucFbgId].ucFbNsBase; ucIdx < _arFbg[ucFbgId].ucFbNs; ucIdx++)
    {
    
        if ( _arFbg[ucFbgId].u4DispTag[ucIdx] == INVALID_DISPTAG)
        {
            ucDispTag = 'R';
        }
        else if ( _arFbg[ucFbgId].u4DispTag[ucIdx] == OMX_PENDING_DISPTAG)
        {
            ucDispTag = 'P';
        }
        else 
        {
            ucDispTag = 'D';
        }        
        
        if(ucIdx < FBM_MAX_FB_NS_PER_GROUP)
        {
            if (_arFbg[ucFbgId].fgUFO)
            {
                  LOG(0, "\tFB(%d) S(%d,%d,%c) Y(0x%x) Y_Ext(0x%x) | C(0x%x) C_Ext(0x%x) PTS(0x%8x)\n",
                      ucIdx,
                      _arFbg[ucFbgId].aucFbStatus[ucIdx],
                      _arFbg[ucFbgId].afgRefList[ucIdx],
                      ucDispTag,
                      _arFbg[ucFbgId].au4AddrY[ucIdx],
                      _arFbg[ucFbgId].au4AddrY_Ext[ucIdx],
                      _arFbg[ucFbgId].au4AddrC[ucIdx],
                      _arFbg[ucFbgId].au4AddrC_Ext[ucIdx],
                      _arFbg[ucFbgId].prPicHdr ? _arFbg[ucFbgId].prPicHdr[ucIdx].u4PTS : 0);    
            }    
            else
            {
                LOG(0, "\tFB(%2d) S(%d,%d,%c) Y(0x%x) C(0x%x) PTS(0x%8x)\n",
                    ucIdx,
                    _arFbg[ucFbgId].aucFbStatus[ucIdx],
                    _arFbg[ucFbgId].afgRefList[ucIdx],
                    ucDispTag,
                    _arFbg[ucFbgId].au4AddrY[ucIdx],
                    _arFbg[ucFbgId].au4AddrC[ucIdx],
                    _arFbg[ucFbgId].prPicHdr ? _arFbg[ucFbgId].prPicHdr[ucIdx].u4PTS : 0);                
            }
        }
    }
    for(ucIdx = _arFbg[ucFbgId].ucFbNsOldBase; ucIdx < _arFbg[ucFbgId].ucFbNsOld; ucIdx++)
    {
        if ( _arFbg[ucFbgId].u4DispTag[ucIdx] == INVALID_DISPTAG)
        {
            ucDispTag = 'R';
        }
        else if ( _arFbg[ucFbgId].u4DispTag[ucIdx] == OMX_PENDING_DISPTAG)
        {
            ucDispTag = 'P';
        }
        else 
        {
            ucDispTag = 'D';
        }   
        
        if(ucIdx < FBM_MAX_FB_NS_PER_GROUP)
        {
            if (_arFbg[ucFbgId].fgUFO)
            {
                  LOG(0, "\tFB(%d) S(%d,%d,%c) Y(0x%x) Y_Ext(0x%x) | C(0x%x) C_Ext(0x%x) PTS(0x%8x)\n",
                      ucIdx,
                      _arFbg[ucFbgId].aucFbStatus[ucIdx],
                      _arFbg[ucFbgId].afgRefList[ucIdx],
                      ucDispTag,
                      _arFbg[ucFbgId].au4AddrY[ucIdx],
                      _arFbg[ucFbgId].au4AddrY_Ext[ucIdx],
                      _arFbg[ucFbgId].au4AddrC[ucIdx],
                      _arFbg[ucFbgId].au4AddrC_Ext[ucIdx],
                      _arFbg[ucFbgId].prPicHdr ? _arFbg[ucFbgId].prPicHdr[ucIdx].u4PTS : 0);    
            }
            else
            {
                  LOG(0, "\tFB(%2d) S(%d,%d,%c) Y(0x%x) C(0x%x) PTS(0x%8x)\n",
                        ucIdx,
                        _arFbg[ucFbgId].aucFbStatus[ucIdx],
                        _arFbg[ucFbgId].afgRefList[ucIdx],
                        ucDispTag,
                        _arFbg[ucFbgId].au4AddrY[ucIdx],
                        _arFbg[ucFbgId].au4AddrC[ucIdx],
                        _arFbg[ucFbgId].prPicHdr ? _arFbg[ucFbgId].prPicHdr[ucIdx].u4PTS : 0);                
            }        
        }
    }

    if(_arFbg[ucFbgId].fgEnableNewSeamless)
    {
        for (ucIdx = 0; ucIdx < _arFbg[ucFbgId].ucSeamlessFbNs; ucIdx++)
        {
            if(ucIdx < FBM_MAX_FB_NS_PER_GROUP)
            {
                LOG(0, "\tFB(%d) S(%d) O(%d) Y(0x%x) C(0x%x)\n",
                    ucIdx,
                    _arFbg[ucFbgId].aucResizeFbStatus[ucIdx],
                    _arFbg[ucFbgId].aucResizeSrcFbId[ucIdx],
                    _arFbg[ucFbgId].au4ResizeAddrY[ucIdx],
                    _arFbg[ucFbgId].au4ResizeAddrC[ucIdx]);
            }
        }
        
        for (ucIdx = 0; ucIdx < _arFbg[ucFbgId].ucSeamless2FbNs; ucIdx++)
        {
            if(ucIdx < FBM_MAX_FB_NS_PER_GROUP)
            {
                LOG(0, "\tFB(%d) S(%d) O(%d) Y(0x%x) C(0x%x)\n",
                    ucIdx,
                    _arFbg[ucFbgId].aucResize2FbStatus[ucIdx],
                    _arFbg[ucFbgId].aucResize2SrcFbId[ucIdx],
                    _arFbg[ucFbgId].au4Resize2AddrY[ucIdx],
                    _arFbg[ucFbgId].au4Resize2AddrC[ucIdx]);
            }
        }        
        
		
	    for (ucIdx=0; ucIdx < MAX_SEAMLESS_BUFF_COUNT; ucIdx++)
	    {			
			LOG(0,"\tImgRz[%d] Addr 0x%x Size 0x%x\n",
				ucIdx,
				_arImgRzTmpBufInfo[ucIdx].u4Addr,
				_arImgRzTmpBufInfo[ucIdx].u4Size);
	    }		
		
    }
	
	if ((_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
		(_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
		(_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP9))
	{
        for (ucIdx = 0; ucIdx < _arFbg[ucFbgId].ucMvBufNs; ucIdx++)
    	{
            if(ucIdx < FBM_MAX_FB_NS_PER_GROUP - 1)
            {
                LOG(1, "\tFB(%d) AddrMv(0x%x)\n",ucIdx,_arFbg[ucFbgId].au4AddrMv[ucIdx]);
            }
    	}
	}
	
}

UINT32 FBM_PreConfigColorMode(UCHAR ucFbgId, UCHAR ucFbgCm, UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar)
{
    //UINT32 u4YAddr, u4CAddr, u4FbgIdx, u4FbId;
    UINT32 u4FbId ,u4BufNum, u4Stride, u4SliceHeight, u4UVAddrOffset;

    // init Q
    _arFbg[ucFbgId].rEmptyQ.ucCount = 0;
    _arFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;


    _FBM_QueryBufNum(_arFbg[ucFbgId].u4VDecFmt, u4HSize, u4VSize, ucFbgCm,
        &u4BufNum, &u4Stride, &u4SliceHeight, &u4UVAddrOffset, 0);
    _arFbg[ucFbgId].ucFbNs = u4BufNum;

    if (_arFbg[ucFbgId].ucFbNs > FBM_MAX_FB_NS_PER_GROUP)
    {
        _arFbg[ucFbgId].ucFbNs = FBM_MAX_FB_NS_PER_GROUP;
    }

    for (u4FbId = 0; u4FbId < _arFbg[ucFbgId].ucFbNs; u4FbId++)
    {
        _arFbg[ucFbgId].au4AddrY[u4FbId] = 0;
        _arFbg[ucFbgId].au4AddrC[u4FbId] = 0;
    }

    _arFbg[ucFbgId].u1FbgAppMode= prPar->u1AppMode;
    _arFbg[ucFbgId].u4FbWidth = u4Stride;
    _arFbg[ucFbgId].u4FbHeight = u4SliceHeight;
    _arFbg[ucFbgId].ucFbgCm = ucFbgCm;
    _arFbg[ucFbgId].ucMvBufNs = 0;

    _arFbg[ucFbgId].u4FbMemoryPoolSize = 0;
    _arFbg[ucFbgId].u4FbMemoryPool = 0;
    
    // init working buf
    if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_MPEG2)
    {
        _arFbg[ucFbgId].au4AddrMv[0] = 0;

        if ((_arFbg[ucFbgId].ucFbNs >= FBM_DBK_FB_NS) && (_arFbg[ucFbgId].ucFbgType != FBM_FBG_TYPE_1080HD_RR))
        {
            _arFbg[ucFbgId].au4AddrY[_arFbg[ucFbgId].ucFbNs] = 0;
            _arFbg[ucFbgId].au4AddrC[_arFbg[ucFbgId].ucFbNs] = 0;
        }
    }
    else if ((_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265))
    {
        for (u4FbId = 0; u4FbId < _arFbg[ucFbgId].ucMvBufNs && u4FbId < (FBM_MAX_FB_NS_PER_GROUP-1) ; u4FbId++)
        {
            _arFbg[ucFbgId].au4AddrMv[u4FbId] = 0;
        }
    }
    else if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RV)
    {
        _arFbg[ucFbgId].u4ExtraYBuffer = 0;
        _arFbg[ucFbgId].u4ExtraCBuffer = 0;
        _arFbg[ucFbgId].au4AddrMv[0] = 0;
    }
    else
    {
        _arFbg[ucFbgId].au4AddrMv[0] = 0;
    }

    _arFbg[ucFbgId].u4WorkBufSize = 0;

    if ((_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
        (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8)||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP6))

    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, 0) == OSR_OK);
    }
    else
    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, 0) == OSR_OK);

        // create semaphore, Empty BQ
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, 0) == OSR_OK);

#ifdef CC_VDP_FULL_ISR
        _arFbg[ucFbgId].hEmptyQSemaphore.prState = &_rState;
        _arFbg[ucFbgId].hEmptyBQSemaphore.prState = &_rState;
#endif

    }

    _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;

    // set default status to unknown
    for (u4FbId = 0; u4FbId < _arFbg[ucFbgId].ucFbNs; u4FbId++)
    {
        _arFbg[ucFbgId].aucFbStatus[u4FbId] = FBM_FB_STATUS_UNKNOWN;
    }

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
//    ASSERT(_arFbg[ucFbgId].hEmptyRQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    return 0;
}


void _DetectNewSeamless(UINT32 u4FbgId, UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar)
{
#ifdef CC_FBM_ENABLE_NEW_SEAMLESS_DETECTION
    if (prPar->u1AppMode == FBM_FBG_APP_Frame_Pack_3D)
    {
        // auto detect if need enable new seamless.
        UINT32 u43DFbgId = FBM_FBG_ID_UNKNOWN;
        UINT32 u4ResizeFbgId = FBM_FBG_ID_UNKNOWN;
        UINT32 u4Addr, u4FbgIdx, u4FbIdx, u4KR3DFbNs = 0;
        UINT32 u4ReusePoolId = FBM_FBG_ID_UNKNOWN;
        FBM_POOL_T* prFbmPool = NULL;

        UINT32 u4SeamlessYSize = FBM_FBG_TYPE_1080HD_Y_SIZE;
        UINT32 u4SeamlessCSize = FBM_FBG_TYPE_1080HD_C_SIZE;
        UCHAR ucRzId = 0;
        
        _arFbg[u4FbgId].rSeqHdr.u2HSize = u4HSize;
        _arFbg[u4FbgId].rSeqHdr.u2VSize = u4VSize;
        
        for (u4FbgIdx = 0; u4FbgIdx < FBG_MAX_NS; u4FbgIdx++)
        {
            if (_arFbg[u4FbgIdx].ucFbgId != FBM_FBG_ID_UNKNOWN && _arFbg[u4FbgIdx].u1FbgAppMode == FBM_FBG_APP_Frame_Pack_3D)
            {
                if (u4FbgIdx == u4FbgId)
                {
                    // skip self.
                    continue;
                }
                u43DFbgId = u4FbgIdx;

                if (u4HSize > _arFbg[u43DFbgId].rSeqHdr.u2HSize || 
                    (u4VSize > _arFbg[u43DFbgId].rSeqHdr.u2VSize  && _arFbg[u43DFbgId].rSeqHdr.u2VSize < 1080))
                {
                    LOG(1, "Resize FbgId %d  (%d %d) to (%d %d)\n", u43DFbgId, _arFbg[u43DFbgId].rSeqHdr.u2HSize, _arFbg[u43DFbgId].rSeqHdr.u2VSize, u4HSize, u4VSize);
                    u4ResizeFbgId = u43DFbgId;
                    _arFbg[u43DFbgId].rSeqHdr.u4ResizeWidth = u4HSize;
                    _arFbg[u43DFbgId].rSeqHdr.u4ResizeHeight = u4VSize;
                }
                else if (u4HSize < _arFbg[u43DFbgId].rSeqHdr.u2HSize || 
                    (u4VSize < _arFbg[u43DFbgId].rSeqHdr.u2VSize && u4VSize < 1080))
                {
                    LOG(1, "Resize FbgId %d  (%d %d) to (%d %d)\n", u4FbgId, u4HSize, u4VSize, _arFbg[u43DFbgId].rSeqHdr.u2HSize, _arFbg[u43DFbgId].rSeqHdr.u2VSize);
                    u4ResizeFbgId = u4FbgId;
                    _arFbg[u4ResizeFbgId].rSeqHdr.u4ResizeWidth = _arFbg[u43DFbgId].rSeqHdr.u2HSize;
                    _arFbg[u4ResizeFbgId].rSeqHdr.u4ResizeHeight = _arFbg[u43DFbgId].rSeqHdr.u2VSize;
                }
                else
                {
                    break;
                }

                if (_arFbg[u4FbgId].ucFbgCm == FBM_CM_422)
                {
                    u4SeamlessCSize *= 2;
                }
                else if (_arFbg[u4FbgId].ucFbgCm == FBM_CM_444)
                {
                    u4SeamlessCSize *= 4;
                }

                ASSERT(_arFbg[u4FbgId].ucFbgCm == _arFbg[u43DFbgId].ucFbgCm);
                
                //get extra buffer for 1 frame.
                if (_arFbg[u4FbgId].u4FbMemoryPoolSize > FBM_GetRunningVdecMemSize(u4FbgId))
                {
                    UINT32 u4FreeSize = 0;
                    u4FreeSize = _arFbg[u4FbgId].u4FbMemoryPoolSize - FBM_GetRunningVdecMemSize(u4FbgId);

                    if (u4FreeSize > u4SeamlessYSize + u4SeamlessCSize)
                    {
                        u4ReusePoolId = u4FbgId;
                        LOG(1, "Share pool FbgId %d, free size %d\n",u4ReusePoolId, u4FreeSize);
                    }
                    else if (_arFbg[u43DFbgId].u4FbMemoryPoolSize > FBM_GetRunningVdecMemSize(u43DFbgId))
                    {
                        u4FreeSize = _arFbg[u43DFbgId].u4FbMemoryPoolSize - FBM_GetRunningVdecMemSize(u43DFbgId);

                        if (u4FreeSize > u4SeamlessYSize + u4SeamlessCSize)
                        {
                            u4ReusePoolId = u43DFbgId;
                            LOG(1, "Share pool FbgId %d, free size %d\n",u4ReusePoolId, u4FreeSize);
                        }
                    }
                }
                else if (_arFbg[u43DFbgId].u4FbMemoryPoolSize > FBM_GetRunningVdecMemSize(u43DFbgId))
                {
                    UINT32 u4FreeSize = 0;
                    u4FreeSize = _arFbg[u43DFbgId].u4FbMemoryPoolSize - FBM_GetRunningVdecMemSize(u43DFbgId);

                    if (u4FreeSize > u4SeamlessYSize + u4SeamlessCSize)
                    {
                        u4ReusePoolId = u43DFbgId;
                        LOG(1, "Share pool FbgId %d, free size %d\n",u4ReusePoolId, u4FreeSize);
                    }
                }

                LOG(1, "Reuse Pool ID: %d\n", u4ReusePoolId);
                ASSERT(u4ReusePoolId != FBM_FBG_ID_UNKNOWN);

                ucRzId = _VDP_Prescale_Init(0);
                LOG(1, "Partitioning new seamless buffer, RzId %d.\n", ucRzId);
                ASSERT(ucRzId != VDP_PRESCALE_RZ_ID_UNKNOWN);
                _arFbg[u4ResizeFbgId].ucSeamlessRzId = (ucRzId != VDP_PRESCALE_RZ_ID_UNKNOWN) ? ucRzId : VDP_PRESCALE_RZ_ID_DEFAULT;
                    
                _arFbg[u4ResizeFbgId].u4FbResizeMemoryPoolSize = 1920*1088*3; // 1 from share pool, 2 from KR3D.
                _arFbg[u4ResizeFbgId].u4FbResizeMemoryPool = 0;

                // partition seamless buffer here.
                _arFbg[u4ResizeFbgId].ucSeamlessFbNs = 1;

                u4Addr = _arFbg[u4ReusePoolId].u4FbMemoryPool + FBM_GetRunningVdecMemSize(u4ReusePoolId);
                u4Addr = (((u4Addr) + 0x7FF) & (~0x7FF));
                for (u4FbIdx = 0; ((u4FbIdx < _arFbg[u4ResizeFbgId].ucSeamlessFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
                {
                    _arFbg[u4ResizeFbgId].au4ResizeAddrY[u4FbIdx] = u4Addr;
                    FBM_GFX_MEMSET((UINT8*)u4Addr, FBM_FBG_1080HD_H, FBM_FBG_1080HD_V, 0x0);
                    u4Addr += u4SeamlessYSize;

                    _arFbg[u4ResizeFbgId].au4ResizeAddrC[u4FbIdx] = u4Addr;
                    FBM_GFX_MEMSET((UINT8*)u4Addr, FBM_FBG_1080HD_H, (FBM_FBG_1080HD_V >> 1), 0x80);
                    u4Addr += u4SeamlessCSize;
                }

                // Reuse KR3D buffer.
                prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_IMGRZ_3D_KR);
                u4Addr = (((prFbmPool->u4Addr) + 0x7FF) & (~0x7FF));
                
                u4KR3DFbNs = (FBM_IMGRZ_3D_KR_POOL_SIZE / (u4SeamlessYSize + u4SeamlessCSize));
                
                for (u4FbIdx = _arFbg[u4ResizeFbgId].ucSeamlessFbNs ; ((u4FbIdx < _arFbg[u4ResizeFbgId].ucSeamlessFbNs + u4KR3DFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
                {
                    _arFbg[u4ResizeFbgId].au4ResizeAddrY[u4FbIdx] = u4Addr;
                    FBM_GFX_MEMSET((UINT8*)u4Addr, FBM_FBG_1080HD_H, FBM_FBG_1080HD_V, 0x0);
                    u4Addr += u4SeamlessYSize;

                    _arFbg[u4ResizeFbgId].au4ResizeAddrC[u4FbIdx] = u4Addr;
                    FBM_GFX_MEMSET((UINT8*)u4Addr, FBM_FBG_1080HD_H, (FBM_FBG_1080HD_V >> 1), 0x80);
                    u4Addr += u4SeamlessCSize;
                }
                
                _arFbg[u4ResizeFbgId].ucSeamlessFbNs += u4KR3DFbNs;

                _arFbg[u4ResizeFbgId].rResizeEmptyQ.ucCount = 0;
                _arFbg[u4ResizeFbgId].rResizeEmptyQ.ucReadIdx = 0;

                // create semaphore, Empty Q
                VERIFY(_FBM_csema_reset(&_arFbg[u4ResizeFbgId].hEmptyResizeQSemaphore, _arFbg[u4ResizeFbgId].hMutex, (INT32)_arFbg[u4ResizeFbgId].ucSeamlessFbNs) == OSR_OK);

                // put frames into empty Q
                for (u4FbIdx = 0; u4FbIdx < _arFbg[u4ResizeFbgId].ucSeamlessFbNs; u4FbIdx++)
                {
                    _FBM_PutFrameBufferToEmptyResizeQ(u4ResizeFbgId, (UCHAR)(u4FbIdx));
                }

                if (u4ResizeFbgId == u43DFbgId)
                {
                    FBM_MUTEX_LOCK(u4ResizeFbgId);

                    _arFbg[u4ResizeFbgId].fgEnableNewSeamless = TRUE;
                    if (_arFbg[u4ResizeFbgId].rDisplayQ.ucCount > 0)
                    {
                        UCHAR ucCount = _arFbg[u4ResizeFbgId].rDisplayQ.ucCount;
                        UCHAR ucResizeFbId, ucReadIdx;
                        ucReadIdx = _arFbg[u4ResizeFbgId].rDisplayQ.ucReadIdx;

                        do
                        {
                            ucResizeFbId = _arFbg[u4ResizeFbgId].rDisplayQ.aucQueue[ucReadIdx];
                            LOG(1, "Peek and resize (%d, %d)\n",u4ResizeFbgId,ucResizeFbId);
                            _arFbg[u4ResizeFbgId].ucCurSeamlessTaskId++;
                            if (_arFbg[u4ResizeFbgId].ucCurSeamlessTaskId == 0xFF)
                            {
                                _arFbg[u4ResizeFbgId].ucCurSeamlessTaskId = 0;
                            }
                            _arFbg[u4ResizeFbgId].prPicHdr[ucResizeFbId].ucSeamlessTaskId = _arFbg[u4ResizeFbgId].ucCurSeamlessTaskId;
                            _VDP_Prescale_Send(_arFbg[u4ResizeFbgId].ucSeamlessRzId, u4ResizeFbgId, ucResizeFbId, _arFbg[u4ResizeFbgId].ucCurSeamlessTaskId);
                            ucCount--;
                            ucReadIdx = (UCHAR) (ucReadIdx+1) % FBM_MAX_FB_NS_PER_GROUP;
                        }
                        while(ucCount > 0);
                    }

                    FBM_MUTEX_UNLOCK(u4ResizeFbgId);
                }
               
                _arFbg[u4ResizeFbgId].fgEnableNewSeamless = TRUE;
                _arFbg[u4ResizeFbgId].fgAutoSeamless = TRUE;
                break;
            }
        }
    }
#endif
    return;
}

void _EnableNewSeamless(UINT32 u4FbgId)
{
    if (!_arFbg[u4FbgId].fgEnableNewSeamless)
    {
        UINT32 u4Addr, u4FbIdx;
#ifdef CC_FBM_SUPPORT_4K2K_SEAMLESS
        UINT32 u4SeamlessYSize = FBM_FBG_TYPE_4K2K_Y_SIZE;
        UINT32 u4SeamlessCSize = FBM_FBG_TYPE_4K2K_C_SIZE;
#else
        UINT32 u4SeamlessYSize = FBM_FBG_TYPE_1080HD_Y_SIZE;
        UINT32 u4SeamlessCSize = FBM_FBG_TYPE_1080HD_C_SIZE;
#endif
		FBM_CODEC_RESOLUTION_T rResolution;
		x_memset((VOID*)&rResolution,0x0,sizeof(FBM_CODEC_RESOLUTION_T));	
		
        _arFbg[u4FbgId].rResizeEmptyQ.ucCount = 0;
        _arFbg[u4FbgId].rResizeEmptyQ.ucReadIdx = 0;
		
        _arFbg[u4FbgId].rResize2EmptyQ.ucCount = 0;
        _arFbg[u4FbgId].rResize2EmptyQ.ucReadIdx = 0;		
		
		_arFbg[u4FbgId].ucSeamlessFbNs = 0;
		_arFbg[u4FbgId].ucSeamless2FbNs = 0;
		
        if (_arFbg[u4FbgId].ucFbgCm == FBM_CM_422)
        {
            u4SeamlessCSize *= 2;
        }
        else if (_arFbg[u4FbgId].ucFbgCm == FBM_CM_444)
        {
            u4SeamlessCSize *= 4;
        }

        _arFbg[u4FbgId].u4FbResizeMemoryPool = 0;
        _arFbg[u4FbgId].u4FbResizeMemoryPoolSize = 0;

        // Allocate seamless buffer for RV RPR.
        if (_arFbg[u4FbgId].u4VDecFmt == FBM_VDEC_RV && _arFbg[u4FbgId].fgRPRMode)
        {
            LOG(1, "Partitioning RV RPR seamless buffer.\n");
            _arFbg[u4FbgId].ucSeamlessFbNs = 2;
            _arFbg[u4FbgId].au4ResizeAddrY[0] = _arFbg[u4FbgId].u4RPRSeamlessBuffer;
            _arFbg[u4FbgId].au4ResizeAddrC[0] = _arFbg[u4FbgId].au4ResizeAddrY[0] + FBM_FBG_TYPE_PAL_Y_SIZE;
            _arFbg[u4FbgId].au4ResizeAddrY[1] = _arFbg[u4FbgId].au4ResizeAddrC[0] + FBM_FBG_TYPE_PAL_C_SIZE;
            _arFbg[u4FbgId].au4ResizeAddrC[1] = _arFbg[u4FbgId].au4ResizeAddrY[1] + FBM_FBG_TYPE_PAL_Y_SIZE;

            // create semaphore, Empty Q
            VERIFY(_FBM_csema_reset(&_arFbg[u4FbgId].hEmptyResizeQSemaphore, _arFbg[u4FbgId].hMutex, (INT32)_arFbg[u4FbgId].ucSeamlessFbNs) == OSR_OK);

            // put frames into empty Q
            for (u4FbIdx = 0; u4FbIdx < _arFbg[u4FbgId].ucSeamlessFbNs; u4FbIdx++)
            {
                _FBM_PutFrameBufferToEmptyResizeQ(u4FbgId, (UCHAR)(u4FbIdx));
            }

            return;
        }

        // Allocate seamless buffer for netflix, etc.

        LOG(1, "Partitioning seamless buffer.\n");
        
	
		FBM_GetCodecResolution(u4FbgId,&rResolution);
        

        _GetSeamlessBuffer(&_arFbg[u4FbgId].u4FbResizeMemoryPool, &_arFbg[u4FbgId].u4FbResizeMemoryPoolSize);
        if (_arFbg[u4FbgId].u4FbResizeMemoryPool == 0)
        {
            LOG(0, "Error! Could not allocate seamless buffer!\n");
            ASSERT(_arFbg[u4FbgId].u4FbResizeMemoryPool != 0 && _arFbg[u4FbgId].u4FbResizeMemoryPoolSize != 0);
        }

        _arFbg[u4FbgId].ucSeamlessFbNs = (_arFbg[u4FbgId].u4FbResizeMemoryPoolSize / (u4SeamlessYSize + u4SeamlessCSize));

        LOG(1, "Seamless Config(2K): FbNs(%d) Y,C(0x%8x, 0x%8x).\n", _arFbg[u4FbgId].ucSeamlessFbNs, u4SeamlessYSize, u4SeamlessCSize);
        u4Addr = _arFbg[u4FbgId].u4FbResizeMemoryPool;

		if (((rResolution.u4Width <= FBM_FBG_4K2K_H) && (rResolution.u4Width > FBM_FBG_1080HD_H)) &&
		    ((rResolution.u4Height <= FBM_FBG_4K2K_V) && (rResolution.u4Height > FBM_FBG_1080HD_V)))
	    {
	        //now,4K2K seamleas 2FB,if enlarge FB,then using 2FB of MPEG2 to save buff 

            for (u4FbIdx = 0; ((u4FbIdx < _arFbg[u4FbgId].ucSeamlessFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
            {
                _arFbg[u4FbgId].au4ResizeAddrY[u4FbIdx] = u4Addr;
        		FBM_GDMA_MEMSET((VOID*)u4Addr,0x0,(FBM_FBG_4K2K_H * FBM_FBG_4K2K_V));
                u4Addr += u4SeamlessYSize;

                _arFbg[u4FbgId].au4ResizeAddrC[u4FbIdx] = u4Addr;
        		FBM_GDMA_MEMSET((VOID*)u4Addr,0x80808080,(FBM_FBG_4K2K_H * FBM_FBG_4K2K_V >> 1));
                u4Addr += u4SeamlessCSize;                
            }	 
            
            _GetSeamlessBuffer(&_arFbg[u4FbgId].u4FbResizeMemoryPool2, &_arFbg[u4FbgId].u4FbResizeMemoryPool2Size);
            if (_arFbg[u4FbgId].u4FbResizeMemoryPool2 == 0)
            {
                LOG(0, "Error! Could not allocate seamless buffer!\n");
                ASSERT(_arFbg[u4FbgId].u4FbResizeMemoryPool2 != 0 && _arFbg[u4FbgId].u4FbResizeMemoryPool2Size != 0);
            }		 
            u4Addr = _arFbg[u4FbgId].u4FbResizeMemoryPool2;
            //start FbNs      
			
			_arFbg[u4FbgId].ucSeamless2FbNs = (_arFbg[u4FbgId].u4FbResizeMemoryPool2Size / (u4SeamlessYSize + u4SeamlessCSize));
			            
            for (u4FbIdx = 0; ((u4FbIdx < _arFbg[u4FbgId].ucSeamless2FbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
            {
                _arFbg[u4FbgId].au4Resize2AddrY[u4FbIdx] = u4Addr;
        		FBM_GDMA_MEMSET((VOID*)u4Addr,0x0,(FBM_FBG_4K2K_H * FBM_FBG_4K2K_V));
                u4Addr += u4SeamlessYSize;

                _arFbg[u4FbgId].au4Resize2AddrC[u4FbIdx] = u4Addr;
        		FBM_GDMA_MEMSET((VOID*)u4Addr,0x80808080,(FBM_FBG_4K2K_H * FBM_FBG_4K2K_V >> 1));
                u4Addr += u4SeamlessCSize;
            }            
	    }
	    else
	    {
            for (u4FbIdx = 0; ((u4FbIdx < _arFbg[u4FbgId].ucSeamlessFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
            {
                _arFbg[u4FbgId].au4ResizeAddrY[u4FbIdx] = u4Addr;
                //FBM_GFX_MEMSET((UINT8*)u4Addr, FBM_FBG_1080HD_H, FBM_FBG_1080HD_V, 0x0);
				FBM_GDMA_MEMSET((VOID*)u4Addr,0x0,(FBM_FBG_1080HD_H * FBM_FBG_1080HD_V));
                u4Addr += u4SeamlessYSize;

                _arFbg[u4FbgId].au4ResizeAddrC[u4FbIdx] = u4Addr;
                //FBM_GFX_MEMSET((UINT8*)u4Addr, FBM_FBG_1080HD_H, (FBM_FBG_1080HD_V >> 1), 0x80);
                FBM_GDMA_MEMSET((VOID*)u4Addr,0x80808080,(FBM_FBG_1080HD_H * FBM_FBG_1080HD_V >> 1));
                u4Addr += u4SeamlessCSize;
            }
	    }	    		
        // create semaphore for  Resize Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[u4FbgId].hEmptyResizeQSemaphore, _arFbg[u4FbgId].hMutex, (INT32)_arFbg[u4FbgId].ucSeamlessFbNs) == OSR_OK);

        // create semaphore, Resize2 Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[u4FbgId].hEmptyResize2QSemaphore, _arFbg[u4FbgId].hMutex, (INT32)_arFbg[u4FbgId].ucSeamless2FbNs) == OSR_OK);

        // put frames into for Resize empty Q
        for (u4FbIdx = 0; u4FbIdx < _arFbg[u4FbgId].ucSeamlessFbNs; u4FbIdx++)
        {
            _FBM_PutFrameBufferToEmptyResizeQ(u4FbgId, (UCHAR)(u4FbIdx));
        }
		
        // put frames into Resize2 empty Q
        for (u4FbIdx = 0; u4FbIdx < _arFbg[u4FbgId].ucSeamless2FbNs; u4FbIdx++)
        {
            _FBM_PutFrameBufferToEmptyResize2Q(u4FbgId, (UCHAR)(u4FbIdx));
        }

        //_arFbg[u4FbgId].fgEnableNewSeamless = TRUE;
    }
    return;
}

static void _InitSeamlessBuffer(void)
{
#ifdef CC_ENABLE_SEAMLESS_FOR_2D
    FBM_POOL_T* prFbmPool = NULL;
    UCHAR ucCount = 0;
    UINT32 u4Addr = 0;
	UINT32 u4ResizePoolSize = 0;
	UINT32 u4ImgrzTmpPoolSize = 0;
	
	#if defined(CC_MT5890) || defined(CC_MT5861)
		u4ImgrzTmpPoolSize = ((FBM_FBG_TYPE_1080HD_Y_SIZE * 3) >> 1); //(6M ,2FB) 2 ImgRz need this temp buffer,MPEG & MPEG2 use one 
	#endif
	
#if defined(CC_FBM_SUPPORT_4K2K_SEAMLESS)
    u4ResizePoolSize = ((FBM_FBG_TYPE_4K2K_Y_SIZE * 3) >> 1) * 2; // (27M ,2FB )
#else
    u4ResizePoolSize = ((FBM_FBG_TYPE_1080HD_Y_SIZE * 3) >> 1) * 2; //(6M ,2FB)
#endif

    x_memset(_arSeamlessInfo,0x0,sizeof(_arSeamlessInfo));
	x_memset(_arImgRzTmpBufInfo,0x0,sizeof(_arImgRzTmpBufInfo));

//4K2K + 4WAY FHD using 4k2k buff 
#if defined(CC_FBM_SUPPORT_4K2K)

    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG);
    if (prFbmPool)
    {
        u4Addr = prFbmPool->u4Addr + FBM_MPEG_Y_SIZE - u4ResizePoolSize;
		_arSeamlessInfo[ucCount].u4Addr = u4Addr;
		_arSeamlessInfo[ucCount].u4Size = u4ResizePoolSize;
		_arSeamlessInfo[ucCount].fgInUse = FALSE;
		
		LOG(0,"Seamless[%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
			ucCount,
			_arSeamlessInfo[ucCount].u4Addr,
			_arSeamlessInfo[ucCount].u4Size,
			_arSeamlessInfo[ucCount].fgInUse);

		_arImgRzTmpBufInfo[ucCount].u4Addr = _arSeamlessInfo[ucCount].u4Addr - u4ImgrzTmpPoolSize;
		_arImgRzTmpBufInfo[ucCount].u4Size = u4ImgrzTmpPoolSize;
		_arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;
		
		LOG(0,"ImgRz [%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
			ucCount,
			_arImgRzTmpBufInfo[ucCount].u4Addr,
			_arImgRzTmpBufInfo[ucCount].u4Size,
			_arImgRzTmpBufInfo[ucCount].fgInUse);
		
		ucCount++;		
    }
	
	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG2);
	if (prFbmPool)
	{
		u4Addr = prFbmPool->u4Addr + FBM_MPEG_Y_SIZE - u4ResizePoolSize;
		_arSeamlessInfo[ucCount].u4Addr = u4Addr;
		_arSeamlessInfo[ucCount].u4Size = u4ResizePoolSize;
		_arSeamlessInfo[ucCount].fgInUse = FALSE;
		
		LOG(0,"Seamless[%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
			ucCount,
			_arSeamlessInfo[ucCount].u4Addr,
			_arSeamlessInfo[ucCount].u4Size,
			_arSeamlessInfo[ucCount].fgInUse);	

		_arImgRzTmpBufInfo[ucCount].u4Addr = _arSeamlessInfo[ucCount].u4Addr - u4ImgrzTmpPoolSize;
		_arImgRzTmpBufInfo[ucCount].u4Size = u4ImgrzTmpPoolSize;
		_arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;
		
		LOG(0,"ImgRz [%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
			ucCount,
			_arImgRzTmpBufInfo[ucCount].u4Addr,
			_arImgRzTmpBufInfo[ucCount].u4Size,
			_arImgRzTmpBufInfo[ucCount].fgInUse);		
		
		ucCount++;
	}
	#if defined(CC_FBM_TWO_4K2K)

		prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG3);
		if (prFbmPool)
		{
			u4Addr = prFbmPool->u4Addr + FBM_MPEG_Y_SIZE - u4ResizePoolSize;
			_arSeamlessInfo[ucCount].u4Addr = u4Addr;
			_arSeamlessInfo[ucCount].u4Size = u4ResizePoolSize;
			_arSeamlessInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"Seamless[%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arSeamlessInfo[ucCount].u4Addr,
				_arSeamlessInfo[ucCount].u4Size,
				_arSeamlessInfo[ucCount].fgInUse);
			
			_arImgRzTmpBufInfo[ucCount].u4Addr = _arSeamlessInfo[ucCount].u4Addr - u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].u4Size = u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"ImgRz [%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arImgRzTmpBufInfo[ucCount].u4Addr,
				_arImgRzTmpBufInfo[ucCount].u4Size,
				_arImgRzTmpBufInfo[ucCount].fgInUse);
			
			ucCount++;		
		}

		prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG4);
		if (prFbmPool)
		{
			u4Addr = prFbmPool->u4Addr + FBM_MPEG_Y_SIZE - u4ResizePoolSize;
			_arSeamlessInfo[ucCount].u4Addr = u4Addr;
			_arSeamlessInfo[ucCount].u4Size = u4ResizePoolSize;
			_arSeamlessInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"Seamless[%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arSeamlessInfo[ucCount].u4Addr,
				_arSeamlessInfo[ucCount].u4Size,
				_arSeamlessInfo[ucCount].fgInUse);		
			
			_arImgRzTmpBufInfo[ucCount].u4Addr = _arSeamlessInfo[ucCount].u4Addr - u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].u4Size = u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"ImgRz [%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arImgRzTmpBufInfo[ucCount].u4Addr,
				_arImgRzTmpBufInfo[ucCount].u4Size,
				_arImgRzTmpBufInfo[ucCount].fgInUse);					
			
			ucCount++;
		}
	#endif
	
//FHD 
#elif !defined(CC_FBM_SUPPORT_4K2K) && !defined(CC_FBM_TWO_4K2K)
	
	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG);
	if (prFbmPool)
	{
		u4Addr = prFbmPool->u4Addr + FBM_MPEG_Y_SIZE - u4ResizePoolSize;
		_arSeamlessInfo[ucCount].u4Addr = u4Addr;
		_arSeamlessInfo[ucCount].u4Size = u4ResizePoolSize;
		_arSeamlessInfo[ucCount].fgInUse = FALSE;
		
		LOG(0,"Seamless[%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
			ucCount,
			_arSeamlessInfo[ucCount].u4Addr,
			_arSeamlessInfo[ucCount].u4Size,
			_arSeamlessInfo[ucCount].fgInUse);		
		
		_arImgRzTmpBufInfo[ucCount].u4Addr = _arSeamlessInfo[ucCount].u4Addr - u4ImgrzTmpPoolSize;
		_arImgRzTmpBufInfo[ucCount].u4Size = u4ImgrzTmpPoolSize;
		_arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;
		
		LOG(0,"ImgRz [%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
			ucCount,
			_arImgRzTmpBufInfo[ucCount].u4Addr,
			_arImgRzTmpBufInfo[ucCount].u4Size,
			_arImgRzTmpBufInfo[ucCount].fgInUse);		
		
		ucCount++;
	}
	#if defined(CC_FBM_TWO_FBP)
		prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG2);
		if (prFbmPool)
		{
			u4Addr = prFbmPool->u4Addr + FBM_MPEG_Y_SIZE - u4ResizePoolSize;  // YY MV  SM CC 
			_arSeamlessInfo[ucCount].u4Addr = u4Addr;
			_arSeamlessInfo[ucCount].u4Size = u4ResizePoolSize;
			_arSeamlessInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"Seamless[%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arSeamlessInfo[ucCount].u4Addr,
				_arSeamlessInfo[ucCount].u4Size,
				_arSeamlessInfo[ucCount].fgInUse);			
			
			_arImgRzTmpBufInfo[ucCount].u4Addr = _arSeamlessInfo[ucCount].u4Addr - u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].u4Size = u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"ImgRz [%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arImgRzTmpBufInfo[ucCount].u4Addr,
				_arImgRzTmpBufInfo[ucCount].u4Size,
				_arImgRzTmpBufInfo[ucCount].fgInUse);			
		
			ucCount++;
		}		
	#elif defined(CC_FBM_TWO_FBP) && defined(CC_FBM_FOUR_INST_SUPPORT)
		prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG3);
		if (prFbmPool)
		{
			u4Addr = prFbmPool->u4Addr + FBM_MPEG_Y_SIZE - u4ResizePoolSize;
			_arSeamlessInfo[ucCount].u4Addr = u4Addr;
			_arSeamlessInfo[ucCount].u4Size = u4ResizePoolSize;
			_arSeamlessInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"Seamless[%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arSeamlessInfo[ucCount].u4Addr,
				_arSeamlessInfo[ucCount].u4Size,
				_arSeamlessInfo[ucCount].fgInUse);
			
			_arImgRzTmpBufInfo[ucCount].u4Addr = _arSeamlessInfo[ucCount].u4Addr - u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].u4Size = u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"ImgRz [%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arImgRzTmpBufInfo[ucCount].u4Addr,
				_arImgRzTmpBufInfo[ucCount].u4Size,
				_arImgRzTmpBufInfo[ucCount].fgInUse);			
		
			ucCount++;
		}
		
		prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG4);
		if (prFbmPool)
		{
			u4Addr = prFbmPool->u4Addr + FBM_MPEG_Y_SIZE - u4ResizePoolSize;
			_arSeamlessInfo[ucCount].u4Addr = u4Addr;
			_arSeamlessInfo[ucCount].u4Size = u4ResizePoolSize;
			_arSeamlessInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"Seamless[%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arSeamlessInfo[ucCount].u4Addr,
				_arSeamlessInfo[ucCount].u4Size,
				_arSeamlessInfo[ucCount].fgInUse);
			
			_arImgRzTmpBufInfo[ucCount].u4Addr = _arSeamlessInfo[ucCount].u4Addr - u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].u4Size = u4ImgrzTmpPoolSize;
			_arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;
			
			LOG(0,"ImgRz [%d] Addr 0x%x u4Size 0x%x fgInUse %d\n",
				ucCount,
				_arImgRzTmpBufInfo[ucCount].u4Addr,
				_arImgRzTmpBufInfo[ucCount].u4Size,
				_arImgRzTmpBufInfo[ucCount].fgInUse);			
		
			ucCount++;
		}
		
	#endif	
#endif
#endif
}
static void _FreeSeamlessBuffer(UINT32 u4BufferAddr)
{
    UCHAR ucCount;
    for (ucCount=0; ucCount < MAX_SEAMLESS_BUFF_COUNT; ucCount++)
    {
        if (_arSeamlessInfo[ucCount].u4Addr == u4BufferAddr)
        {
            ASSERT(_arSeamlessInfo[ucCount].fgInUse == TRUE);
            _arSeamlessInfo[ucCount].fgInUse = FALSE;

			LOG(1,"Free Seamless[%d] Addr 0x%x\n",
				ucCount,
				_arSeamlessInfo[ucCount].u4Addr);
			
            break;
        }
    }
}

static void _GetSeamlessBuffer(UINT32 *pu4BufferAddr, UINT32 *pu4BufferSize)
{
    UCHAR ucCount;
    
    if(pu4BufferAddr == NULL || pu4BufferSize == NULL)
    {
        ASSERT(0);
        return;
    }

    for (ucCount=0; ucCount < MAX_SEAMLESS_BUFF_COUNT; ucCount++)
    {
        if (_arSeamlessInfo[ucCount].fgInUse == FALSE)
        {
            *pu4BufferAddr = _arSeamlessInfo[ucCount].u4Addr;
            *pu4BufferSize = _arSeamlessInfo[ucCount].u4Size;
            _arSeamlessInfo[ucCount].fgInUse = TRUE;

			LOG(1,"Get Seamless[%d] Addr 0x%x Size 0x%x\n",
				ucCount,
				_arSeamlessInfo[ucCount].u4Addr,
				_arSeamlessInfo[ucCount].u4Size);
			
            break;
        }
    }
}

static void _QueryFixedBuffer(UINT32 *pu4BufferAddr, UINT32 *pu4BufferSize)
{
    UCHAR ucCount;
    UINT32 u4BufferSize = 0;
	
    if(pu4BufferAddr == NULL || pu4BufferSize == NULL)
    {
        ASSERT(0);
        return;
    }
	
    for (ucCount=0; ucCount < MAX_SEAMLESS_BUFF_COUNT; ucCount++)
    {
        //if (_arSeamlessInfo[ucCount].fgInUse == FALSE)
        {
            u4BufferSize = _arSeamlessInfo[ucCount].u4Size;

			LOG(1,"Query Seamless[%d] Addr 0x%x Size 0x%x\n",
				ucCount,
				_arSeamlessInfo[ucCount].u4Addr,
				_arSeamlessInfo[ucCount].u4Size);
			
            break;
        }
    }
	
    for (ucCount=0; ucCount < MAX_SEAMLESS_BUFF_COUNT; ucCount++)
    {
        //if (_arImgRzTmpBufInfo[ucCount].fgInUse == FALSE)
        {
            u4BufferSize += _arImgRzTmpBufInfo[ucCount].u4Size;
			
			LOG(1,"Query ImgRz [%d] Addr 0x%x Size 0x%x\n",
				ucCount,
				_arImgRzTmpBufInfo[ucCount].u4Addr,
				_arImgRzTmpBufInfo[ucCount].u4Size);
			
            break;
        }
    }
    
    LOG(2,"Fixed Size 0x%x\n",u4BufferSize);
		
	*pu4BufferSize = u4BufferSize;
	
	UNUSED(pu4BufferAddr);
		
}
static void _ActiveFHDPool(UINT32 u4FbgId,UCHAR ucFbgType,UINT32 u4FixedSize)
{
    UINT32 u4RequestPoolSize = 0;
    
    _arFbg[u4FbgId].u4FbMemoryPool =  _arFbg[u4FbgId].u4FbActiveFHDPool;
    _arFbg[u4FbgId].u4FbMemoryPoolC = _arFbg[u4FbgId].u4FbActiveFHDPoolC;
    
	if (_FBM_QueryCurPoolSize(ucFbgType,&u4RequestPoolSize))
	{
		_arFbg[u4FbgId].u4FbMemoryPoolSize = u4RequestPoolSize;
	}
	else
	{
	    LOG(0,"ucFbgType(%d) Query Pool Size fail\n",ucFbgType);
	}
	/* -------------check fbm layout overlap use it-------------------------*/

	_arFbg[u4FbgId].u4FbStartPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool;				
	_arFbg[u4FbgId].u4FbEndPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - u4FixedSize;				
	_arFbg[u4FbgId].u4FbStartPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;
	_arFbg[u4FbgId].u4FbEndPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC + FBM_MPEG_C_SIZE;	
	
	#if 0 //YC_MERGE_IN_CHB 
	_arFbg[u4FbgId].u4FbMemoryPoolC = 	_arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - FBM_MPEG_C_SIZE_IN_CHB - u4FixedSize;

	_arFbg[u4FbgId].u4FbStartPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool;				
	_arFbg[u4FbgId].u4FbEndPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;				
	_arFbg[u4FbgId].u4FbStartPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;				
	_arFbg[u4FbgId].u4FbEndPoolCAddr	= _arFbg[u4FbgId].u4FbStartPoolCAddr + FBM_MPEG_C_SIZE_IN_CHB;				
	#endif			
	/*-----------------------------------------------------------------------*/
    
    LOG(2, "ActiveFHDFBM = Y:(0x%8x) C:(0x%x) Pool Size:(0x%x).\n", 
        _arFbg[u4FbgId].u4FbMemoryPool,
        _arFbg[u4FbgId].u4FbMemoryPoolC,
        _arFbg[u4FbgId].u4FbMemoryPoolSize);
    
}
#ifdef FBM_4K2K_SUPPORT
static void _Active4k2kPool(UINT32 u4FbgId,UCHAR ucFbgType,UINT32 u4FixedSize)
{   
    _arFbg[u4FbgId].u4FbMemoryPool = _arFbg[u4FbgId].u4FbActive4K2KPool;
    _arFbg[u4FbgId].u4FbMemoryPoolC = _arFbg[u4FbgId].u4FbActive4K2KPoolC;
    _arFbg[u4FbgId].u4FbMemoryPool2 = _arFbg[u4FbgId].u4FbActive4K2KPool2;
    _arFbg[u4FbgId].u4FbMemoryPool2C = _arFbg[u4FbgId].u4FbActive4K2KPool2C;
    _arFbg[u4FbgId].u4FbMemoryPoolSize = FBM_FBG_TYPE_4K2K_7FB_SIZE_PART1 + FBM_FBG_TYPE_4K2K_7FB_SIZE_PART2;


    /* -------------check fbm layout overlap use it-------------------------*/            
    _arFbg[u4FbgId].u4FbStartPoolYAddr  = _arFbg[u4FbgId].u4FbMemoryPool;    		
    _arFbg[u4FbgId].u4FbEndPoolYAddr    = _arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE ;    		
    _arFbg[u4FbgId].u4FbStartPoolCAddr  = _arFbg[u4FbgId].u4FbMemoryPoolC;
    _arFbg[u4FbgId].u4FbEndPoolCAddr    = _arFbg[u4FbgId].u4FbMemoryPoolC + FBM_MPEG_C_SIZE;

    _arFbg[u4FbgId].u4FbStartPool2YAddr  = _arFbg[u4FbgId].u4FbMemoryPool2;    		
    _arFbg[u4FbgId].u4FbEndPool2YAddr    = _arFbg[u4FbgId].u4FbMemoryPool2 + FBM_MPEG_Y_SIZE;    		
    _arFbg[u4FbgId].u4FbStartPool2CAddr  = _arFbg[u4FbgId].u4FbMemoryPool2C;
    _arFbg[u4FbgId].u4FbEndPool2CAddr    = _arFbg[u4FbgId].u4FbMemoryPool2C + FBM_MPEG_C_SIZE;    	
    /*-----------------------------------------------------------------------*/    	  
    LOG(2, "Active4k2kFBM = Y:(0x%8x) C:(0x%x) Pool Size:(0x%x).\n", 
        _arFbg[u4FbgId].u4FbMemoryPool,
        _arFbg[u4FbgId].u4FbMemoryPoolC,
        _arFbg[u4FbgId].u4FbMemoryPoolSize);            
}
#endif

BOOL FBM_GetFixedImgRzBuffer(UINT32 *pu4AddrY, UINT32 *pu4AddrC)
{
    UCHAR ucCount;
	UINT32 u4AddrY = 0;
	UINT32 u4AddrC = 0;
		
    for (ucCount=0; ucCount < MAX_SEAMLESS_BUFF_COUNT; ucCount++)
    {
        if (_arImgRzTmpBufInfo[ucCount].fgInUse == FALSE)
        {
        	u4AddrY = _arImgRzTmpBufInfo[ucCount].u4Addr;
			u4AddrC = u4AddrY + FBM_FBG_TYPE_1080HD_Y_SIZE;		
			_arImgRzTmpBufInfo[ucCount].fgInUse = TRUE;			
            break;
        }
    }
	if (u4AddrY != 0)
	{
		*pu4AddrY = u4AddrY;
		*pu4AddrC = u4AddrC;
		return TRUE;
	}
	
	return FALSE;
	
}

BOOL FBM_FreeFixedImgRzBuffer(UINT32 u4AddrY, UINT32 u4AddrC)
{
    UCHAR ucCount;
    for (ucCount=0; ucCount < MAX_SEAMLESS_BUFF_COUNT; ucCount++)
    {
        if (_arImgRzTmpBufInfo[ucCount].u4Addr == u4AddrY)
        {
            ASSERT(_arImgRzTmpBufInfo[ucCount].fgInUse == TRUE);
            _arImgRzTmpBufInfo[ucCount].fgInUse = FALSE;

			LOG(1,"Free Fixed[%d] Addr 0x%x\n",
				ucCount,
				_arImgRzTmpBufInfo[ucCount].u4Addr);
			
        	return TRUE;
        }
    }	
	return FALSE;
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** FBM_Init
 *  FBM initialize
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_Init(void)
{
    if (_FbmInitiated == 0)
    {
        UINT32 u4Idx;

        _FbmInitiated = 1;

        FBM_MEMSET((void*)_arFbg, 0, (sizeof(FBM_FBG_T) * FBG_MAX_NS));

        FBM_MEMSET((void*)(&_arFbmCbFunc), 0, sizeof(FBM_CB_FUNC_T));

        // register GFX_Memset() into FBM module
#if !defined(CC_B2R_EMULATION) && !defined(MPV_SEMI_HOST)
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_MEMSET, (UINT32)GFX_Memset);
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_TASK_MEMSET, (UINT32)GFX_Task_Memset);
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_TASK_MEMSET_DONE, (UINT32)GFX_Task_Check_Done);
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_MEMSET, (UINT32)GDMA_MemSet);
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_TASK_MEMSET, (UINT32)GDMA_MemSetAnsync);        
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE, (UINT32)IsMemOptionDone); 
#endif

        // create FBG mutex
        VERIFY(x_sema_create(&_hFbgMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

#ifdef CC_B2R_RES_SUPPORT
        _FBM_B2rResInit();
#endif

        // init frame buffer group
        for (u4Idx = 0; u4Idx < FBG_MAX_NS; u4Idx++)
        {
            _arFbg[u4Idx].ucFbgType = FBM_FBG_TYPE_UNKNOWN;
            _arFbg[u4Idx].ucFbgId = FBM_FBG_ID_UNKNOWN;
            _arFbg[u4Idx].ucFbgCm = FBM_CM_UNKNOWN;

            _arFbg[u4Idx].ucStcSrc = STC_SRC_NS;
            _arFbg[u4Idx].ucAvSyncMode = VID_SYNC_MODE_NS;
            _arFbg[u4Idx].ucPlayMode = FBM_FBG_DTV_MODE;
            _arFbg[u4Idx].u4AvSyncStatus = FBM_FBG_AVSYNC_DISABLE;
            #ifdef CC_B2R_RES_SUPPORT
            _arFbg[u4Idx].pt_res = _arB2rHwRes;
            #endif

            // create semaphore
            VERIFY(x_sema_create(&_arFbg[u4Idx].hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

#ifdef CC_VDP_FULL_ISR
            _arFbg[u4Idx].prState = &_rState;
#endif

            // create semaphore
            VERIFY(_FBM_csema_create(&_arFbg[u4Idx].hEmptyQSemaphore, _arFbg[u4Idx].hMutex, 0) == OSR_OK);

            // create semaphore
            VERIFY(_FBM_csema_create(&_arFbg[u4Idx].hEmptyBQSemaphore, _arFbg[u4Idx].hMutex, 0) == OSR_OK);

            // create semaphore
            VERIFY(_FBM_csema_create(&_arFbg[u4Idx].hEmptyResizeQSemaphore, _arFbg[u4Idx].hMutex, 0) == OSR_OK);

            VERIFY(_FBM_csema_create(&_arFbg[u4Idx].hEmptyResize2QSemaphore, _arFbg[u4Idx].hMutex, 0) == OSR_OK);
            

#ifdef CC_VDP_FULL_ISR
            _arFbg[u4Idx].hEmptyQSemaphore.prState = &_rState;
            _arFbg[u4Idx].hEmptyBQSemaphore.prState = &_rState;
            _arFbg[u4Idx].hEmptyResizeQSemaphore.prState = &_rState;
#endif
        }

        // initialize frame buffer pool
        _prPool = _FBM_PoolInit();
        ASSERT(_prPool != NULL);

        // initialize frame buffer
        _FBM_FbInit(_arFbg);
        FBM_Inst_Init();
        _InitSeamlessBuffer();

        _u4NextFbgId = 0;
#ifndef NDEBUG
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
         FBM_QueryStatus(0xff); 
#endif
#endif
    }
}


//-------------------------------------------------------------------------
/** FBM_GetFbgNs
 *  Get maximum FBG number
 *  @return NONE.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFbgNs(void)
{
    return FBG_MAX_NS;
}

//-------------------------------------------------------------------------
/** FBM_CalcRefNum 
 *  Calc the MV buffer Ns for H264 only
 *  @return Ns.
 */
//-------------------------------------------------------------------------

static UINT32 FBM_CalcRefNum(UCHAR ucFbgType, UINT32 u4VDecFmt, UINT32 u4Width, UINT32 u4Height, UINT8 u1AppMode)
{
    UINT32 u4ReferenceNs = 0;
    
    if ((u4VDecFmt == FBM_VDEC_H264) && (u1AppMode != FBM_FBG_APP_SKYPE))
    {
        // Calculate Reference Frame Number
        switch(ucFbgType)
        {
            case FBM_FBG_TYPE_4K2K:
                u4ReferenceNs = (((FBM_FBG_4K2K_H * FBM_FBG_4K2K_V) * FBM_FBG_H265_4K2K_REFERNCE_NS) / (u4Width * u4Height));  // 4 ref max!!!
                
                LOG(1,"4K2K FBM_FBG_H264_4K2K_REFERNCE_NS %d\n",u4ReferenceNs);
                
                break;
            case FBM_FBG_TYPE_SBSHD: 
            case FBM_FBG_TYPE_TNBHD:
            case FBM_FBG_TYPE_1080HD:
                u4ReferenceNs = (((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V) * FBM_FBG_H264_FHD_REFERNCE_NS) / (u4Width * u4Height));                      
                break;
            case FBM_FBG_TYPE_720HD:
                u4ReferenceNs = (((FBM_FBG_720HD_H * FBM_FBG_720HD_V) * FBM_FBG_TYPE_720HD_H264_R_NS) / (u4Width * u4Height));                                              
                break;
            case FBM_FBG_TYPE_PAL:
                u4ReferenceNs =  (((FBM_FBG_PAL_H * FBM_FBG_PAL_V) * FBM_FBG_TYPE_PAL_H264_R_NS) / (u4Width * u4Height));                                       
                break;
            default:
                u4ReferenceNs = (((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V) * FBM_FBG_H264_FHD_REFERNCE_NS) / (u4Width * u4Height));
                LOG(0, "Warning! No matched fbg type!\n");
                break;
        }
    }
    else if ((u4VDecFmt == FBM_VDEC_H265) && (u1AppMode != FBM_FBG_APP_SKYPE))
    {
        // Calculate Reference Frame Number
        switch(ucFbgType)
        {
            case FBM_FBG_TYPE_4K2K:
                u4ReferenceNs = (((FBM_FBG_4K2K_H * FBM_FBG_4K2K_V) * FBM_FBG_H265_4K2K_REFERNCE_NS) / (u4Width * u4Height));  // 4 ref max!!!
                LOG(1,"4K2K FBM_FBG_H265_4K2K_REFERNCE_NS %d\n",u4ReferenceNs);                
                break;
            case FBM_FBG_TYPE_SBSHD: 
            case FBM_FBG_TYPE_TNBHD:
            case FBM_FBG_TYPE_1080HD:
                u4ReferenceNs = (((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V) * FBM_FBG_H264_FHD_REFERNCE_NS) / (u4Width * u4Height));                      
                break;
            case FBM_FBG_TYPE_720HD:
                u4ReferenceNs = (((FBM_FBG_720HD_H * FBM_FBG_720HD_V) * FBM_FBG_TYPE_720HD_H264_R_NS) / (u4Width * u4Height));                                              
                break;
            case FBM_FBG_TYPE_PAL:
                u4ReferenceNs = (((FBM_FBG_PAL_H * FBM_FBG_PAL_V) * FBM_FBG_TYPE_PAL_H264_R_NS) / (u4Width * u4Height));                                       
                break;
            default:
                u4ReferenceNs = (((FBM_FBG_1080HD_H * FBM_FBG_1080HD_V) * FBM_FBG_H265_FHD_REFERNCE_NS) / (u4Width * u4Height));
                LOG(0, "Warning! No matched fbg type!\n");
                break;
        }    
        
    }
    else if((u4VDecFmt == FBM_VDEC_H264) && (u1AppMode == FBM_FBG_APP_SKYPE))
    {
        u4ReferenceNs = 4;
    }
    else if((u4VDecFmt == FBM_VDEC_H265) && (u1AppMode == FBM_FBG_APP_SKYPE))
    {
        u4ReferenceNs = 6;
    }
    
    if(u4ReferenceNs > FBM_FBG_H264_REFERNCE_MAX_NS)
    {
        u4ReferenceNs = FBM_FBG_H264_REFERNCE_MAX_NS;
    }

    return u4ReferenceNs;
}

//-------------------------------------------------------------------------
/** FBM_VMallocMaping
 *  Calc the buffer Ns for w,h
 *  @return Ns.
 */
//-------------------------------------------------------------------------
#ifdef CC_VDEC_FBM_DYNAMIC_MAPPING

VOID * FBM_WorkBufPartMap(UCHAR ucFbgId, UINT32 u4Addr, UINT32 u4Size)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return NULL;
    }

    LOG(4," FbgId(%d) MAP Addr 0x%x Size 0x%x\n",ucFbgId,u4Addr,u4Size);
    
    if (_arFbg[ucFbgId].pWorkBufVirAddr)
    {
        BSP_UnMapReservedMem(_arFbg[ucFbgId].pWorkBufVirAddr);
    }

    //VC1=12.75K;  VP8=16K;  MPEG=4K;  RV=75K; MJPEG=300K
    //ASSERT(u4Size <= 300*1024);

    _arFbg[ucFbgId].pWorkBufVirAddr = BSP_MapReservedMem((VOID *)u4Addr, u4Size);
    
    if (!_arFbg[ucFbgId].pWorkBufVirAddr)
    {
        LOG(0, "WorkBuf(0x%08x) Map(%d) Virtual Failed!\n", u4Addr, u4Size);
        ASSERT(0);
    }

    return _arFbg[ucFbgId].pWorkBufVirAddr;
}
#endif

//-------------------------------------------------------------------------
/** FBM_CalcBufNum
 *  Calc the buffer Ns for w,h
 *  @return Ns.
 */
//-------------------------------------------------------------------------

UINT32 FBM_CalcBufNum(UCHAR ucFbgId, UCHAR ucFbgType, UINT32 u4VDecFmt, UINT32 u4Width, UINT32 u4Height,
    UINT32 u4PoolSize, UINT32 u4YSize, UINT32 u4CSize, UINT8 u1AppMode, UINT32* pu4ExtFbNs)
{
    BOOL fgEnableH264V5Support = FALSE;
    UINT32 u4FbCount = 0;
    UINT32 u4FbSize = 0;
    UINT32 u4ExtraFbNs = 0;
    FBM_POOL_T* prFbmPool = NULL;
    
    if ((ucFbgId != FBM_FBG_ID_UNKNOWN) && (ucFbgId < FBG_MAX_NS))
    {
        fgEnableH264V5Support = _arFbg[ucFbgId].fgEnableH264V5Support;
    }
    UNUSED(fgEnableH264V5Support);
    UNUSED(prFbmPool);
    UNUSED(u4ExtraFbNs);

    u4FbSize = u4YSize + u4CSize;
    u4FbCount = u4PoolSize / u4FbSize;

    if(u1AppMode == FBM_FBG_APP_MHP)
    {
        u4FbCount = 2;
        return u4FbCount;
    }

    switch (u4VDecFmt)
    {
        case FBM_VDEC_H264:
        case FBM_VDEC_H265:        
        {
            UCHAR ucMvBufNs;

            // MV Frame Buffer Number = Reference Frame Number + 1;
            // MV Frame Buffer Size = (MV Frame Buffer Number * Y Size) / 4
            ucMvBufNs = FBM_CalcRefNum(ucFbgType, u4VDecFmt, u4Width, u4Height, u1AppMode) + 1; //one decoder frame

            // Calculate Total Frame Number = (Total Size - MV Size) / (Y Size + C Size)
            u4FbCount = ((u4PoolSize - ((ucMvBufNs * u4YSize) >> 2)) / u4FbSize);
            
            LOG(1,"ucMvBufNs %d u4FbCount %d u4PoolSize 0x%x u4FbSize 0x%x\n",ucMvBufNs,u4FbCount,u4PoolSize,u4FbSize);
            
#if defined(CC_H264_LV5_SUPPORT_SHARE_POOL) || defined(CC_H264_LV5_SUPPORT_SINGLE_POOL)
            if (fgEnableH264V5Support)
            {
                UINT32 u4ExtraReferenceNs = 0;
#if defined(CC_H264_LV5_SUPPORT_SHARE_POOL)
                prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG2);
        
                ASSERT(FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL > FBM_FBG_H264_FHD_REFERNCE_NS);
                u4ExtraReferenceNs = FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL - FBM_FBG_H264_FHD_REFERNCE_NS;
                u4ExtraFbNs = prFbmPool ? ((prFbmPool->u4Size - ((u4ExtraReferenceNs * u4YSize) >> 2))/u4FbSize) : 0;
#elif defined(CC_H264_LV5_SUPPORT_SINGLE_POOL)
                ASSERT(FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL > FBM_FBG_H264_FHD_REFERNCE_NS);
                u4ExtraReferenceNs = FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL - FBM_FBG_H264_FHD_REFERNCE_NS;
                u4ExtraFbNs = ((FMB_EXTRA_FOR_SINGLE_H264_LV5 - ((u4ExtraReferenceNs * u4YSize) >> 2))/u4FbSize);
#endif
                ASSERT(u4ExtraFbNs >= u4ExtraReferenceNs);
                if (u4ExtraFbNs > u4ExtraReferenceNs)
                {
                    u4ExtraFbNs = u4ExtraReferenceNs;
                }
            }
#endif

            if (u1AppMode == FBM_FBG_APP_SKYPE)
            {
                u4FbCount = 7;
                u4ExtraFbNs = 0;
            }
                        
            if ((ucFbgId != FBM_FBG_ID_UNKNOWN) && (ucFbgId < FBG_MAX_NS))
            {
                _arFbg[ucFbgId].ucMvBufNs = ucMvBufNs;
            }
        }
        break;
        case FBM_VDEC_JPEG:
        {
            if ((u4YSize == u4CSize) && (u1AppMode != FBM_FBG_APP_MTIMAGE))
            {   //FBM_CM_422
                u4FbCount = 2;
            }
            else
            {
                u4FbCount = 4;
            }

#if defined(FBM_4K2K_SUPPORT) || defined(FBM_4K2K_JPG_SUPPORT)// 4k2k need 4 fb! 
            u4FbCount = 4;
#endif      
            LOG(1,"4k2k JPG u4FbCount %d\n",u4FbCount);
        }
        break;
        case FBM_VDEC_MPEG2:
        {
            u4FbCount = 6;           
        }
        break;
        case FBM_VDEC_VC1:
        {
            u4FbCount = 5;
        }
        break;
        case FBM_VDEC_RV:
        {
            // reserve three frame for RPR mode
            // one is for reference frame, the other two frame is for seamless display
            if(u1AppMode == FBM_FBG_APP_OMX_DISP)
            {
                LOG(0, "[%s-%d]OMX not support RV yet....\n", __FUNCTION__, __LINE__);
            }
            if (u4FbCount >= 10)
            {
                u4FbCount -= 2; // usine two frame buffer for rpr mode
            }
#ifdef CC_USE_DDI
            u4FbCount = FBM_MIN(u4FbCount, 5);
#endif
        }
        break;
        case FBM_VDEC_VP6:
        {
            u4FbCount = 5;
        }
        break;
        case FBM_VDEC_VP8:
        {
            u4FbCount = 6;
        }
        break;
        case FBM_VDEC_MPEG4:
        {
            u4FbCount = 6;
        }
        break;
        case FBM_VDEC_VP9:
        {
            u4FbCount = 11;
            if ((ucFbgId != FBM_FBG_ID_UNKNOWN) && (ucFbgId < FBG_MAX_NS))
            {
                _arFbg[ucFbgId].ucMvBufNs = 2;
            }			
        }
        break;
        case FBM_VDEC_AVS:
        {
            u4FbCount = 5;
        }
        break;		
        default:
        {
            //do nothing
        }
        break;
    }
	
    if (u4FbCount >= FBM_MAX_FB_NS_PER_GROUP/2) //FB enough, not need extra FB.
    {
        u4ExtraFbNs = 0;
        u4FbCount = FBM_MAX_FB_NS_PER_GROUP/2-1;
    }
    else if (u4FbCount + u4ExtraFbNs >= FBM_MAX_FB_NS_PER_GROUP/2) //FB not enough, but not need all extra FB.
    {
        u4ExtraFbNs = FBM_MAX_FB_NS_PER_GROUP/2 - u4FbCount - 1;
        u4FbCount = FBM_MAX_FB_NS_PER_GROUP/2-1;
    }
    else
    {
        u4FbCount += u4ExtraFbNs;
    }

    ASSERT(u4FbCount <= FBM_MAX_FB_NS_PER_GROUP/2);
    
    if ((ucFbgId != FBM_FBG_ID_UNKNOWN) && (ucFbgId < FBG_MAX_NS))
    {
        *pu4ExtFbNs = u4ExtraFbNs;
    }

    if ((u4VDecFmt == FBM_VDEC_H264) || (u4VDecFmt == FBM_VDEC_H265) || (u4VDecFmt == FBM_VDEC_VP9))
    {
        if (u1AppMode == FBM_FBG_APP_OMX_DISP)
        {
            //B2r render need 4 lock frame in stage fright: 
            //User space set render will get by b2r(set register) at next vsync. 
            //B2R register will enable render at next vsync. (double buffer)
            //After render, fb will be release at next vysnc. 
            //ASSERT(u4FbCount >= 4 + 1 + 4); // 1decoder + 4 B2R render
        }
        LOG(2, "FbgType(%d) H/V(%d, %d) Ns(%d) Mv(%d) ExtNs(%d) TotalSize(%d)\n",
            ucFbgType, u4Width, u4Height, u4FbCount, ((ucFbgId < FBG_MAX_NS) ? _arFbg[ucFbgId].ucMvBufNs : 0), u4ExtraFbNs, u4PoolSize);
    }
    else
    {
        LOG(2, "FbgType(%d) Codec(%d) W/H(%d, %d) APP(%d) Ns(%d)\n", ucFbgType, u4VDecFmt, u4Width, u4Height, u1AppMode, u4FbCount);
    }
    return u4FbCount;
}


//-------------------------------------------------------------------------
/** FBM_CreateGroupExt
 *  Create FBM Group
 *  @param  ucFbgType   FBG type.
 *  @return FBG index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_CreateGroupExt(UCHAR ucFbgType, UINT32 u4VDecFmt,
                         UINT32 u4HSize, UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar)
{
    UCHAR ucFbpListNs;

    UINT32 u4FbgTypeValid;
    UINT32 u4MallocSize;

    UINT32 u4FbgId;
    UINT32 u4FbgIdx;
    UINT32 u4TargetFbgIdx;
    UINT32 u4ListIdx;
    UINT32 u4FbIdx;
	UINT32 u4FixedAddr = 0;
	UINT32 u4FixedSize = 0;
    CRIT_STATE_T _rEmptyQState;    
#ifdef CC_B2R_RES_SUPPORT
    B2R_IPT_INFO_T t_b2r_res;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
#endif
    UCHAR ucB2rId     = B2R_NS;
    FBP_LIST_T* prFbpList;
    
    if(prPar && (prPar->ucCftFbgId == 0))
    {
        prPar->ucCftFbgId = FBM_FBG_ID_UNKNOWN;
    }
    else if (prPar)
    {
        prPar->ucCftFbgId--;
    }

    VERIFY(x_sema_lock(_hFbgMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    if ((ucFbgType == FBM_FBG_TYPE_4K2K ) && (u4VDecFmt == FBM_VDEC_JPEG))        
    {
        ucFbgType = FBM_FBG_TYPE_4K2K_JPEG;
        LOG(0,"This case is JPEG and revert jpeg from 4k2k video\n");
        
    }

    LOG(0, "FBM_CreateGroupExt: FbgType(%d) Codec(%d) HV(%d, %d) Thumbnail(%d) 10Bit(%d) UFO(%d) CreateFromInst(%d) SeamlessMode(%d) .\n",ucFbgType,u4VDecFmt,u4HSize,u4VSize, 
			prPar?prPar->fgThumbnailMode:0,
			prPar?prPar->fg10Bit:0,
			prPar?prPar->fgUFO:0,
			prPar?prPar->fgCreateFromInst:0,
			prPar ? (prPar->eSeamlessMode):0);
	
    _FBM_GetPoolList(&prFbpList, &ucFbpListNs);

    // Check frame buffer group type
    u4FbgTypeValid = 0;
    for (u4ListIdx = 0; u4ListIdx < ucFbpListNs; u4ListIdx++)
    {
        if (ucFbgType == prFbpList[u4ListIdx].ucType)
        {
            u4FbgTypeValid = 1;
        }
    }
    if ((u4FbgTypeValid == 0) || (u4VDecFmt == FBM_VDEC_UNKNOWN))
    {
        VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
        return FBM_FBG_ID_UNKNOWN;
    }
    if(prPar && (prPar->ucCftFbgId != FBM_FBG_ID_UNKNOWN) && (prPar->ucCftFbgId < FBG_MAX_NS))
    {
        VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
        if (_arFbg[prPar->ucCftFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE)
        {
            LOG(3, "Block as: Previous Dynamic swith not done yet!.\n");
        }
        while (_arFbg[prPar->ucCftFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE)
        {
            x_thread_delay(2);
        }
        VERIFY(x_sema_lock(_hFbgMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
        
        for (u4FbIdx = _arFbg[prPar->ucCftFbgId].ucFbNsBase;
                ((u4FbIdx < _arFbg[prPar->ucCftFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
        {
            if (_arFbg[prPar->ucCftFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_DECODE || 
                _arFbg[prPar->ucCftFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_READY ||
                _arFbg[prPar->ucCftFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_FIELD_READY)
            {
                if (_arFbg[prPar->ucCftFbgId].afgRefList[u4FbIdx] == 1)
                {
                    _arFbg[prPar->ucCftFbgId].afgRefList[u4FbIdx] = 0;
                }
                _arFbg[prPar->ucCftFbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_EMPTY;
                _FBM_PutFrameBufferToEmptyQ(prPar->ucCftFbgId, (UCHAR)(u4FbIdx));
                LOG(3, "Flush DECODE/READY FB(%d) to emptyQ.\n", u4FbIdx);
            }
        }
        u4FbgId = prPar->ucCftFbgId;
        _arFbg[u4FbgId].u4FbMemoryPool2Size = _arFbg[u4FbgId].u4FbMemoryPoolSize;
        _arFbg[u4FbgId].u4FbMemoryPool2 = _arFbg[u4FbgId].u4FbMemoryPool;
    }
    else
    {
		// Search for empty frame buffer group
		u4FbgId = FBM_FBG_ID_UNKNOWN;
		u4TargetFbgIdx = _u4NextFbgId;
		for (u4FbgIdx = 0; u4FbgIdx < FBG_MAX_NS; u4FbgIdx++)
		{
		    if (_arFbg[u4TargetFbgIdx].ucFbgId == FBM_FBG_ID_UNKNOWN)
		    {
		        u4FbgId = u4TargetFbgIdx;

		        u4FbgIdx = FBG_MAX_NS;

		        if (++u4TargetFbgIdx >= FBG_MAX_NS)
		        {
		            u4TargetFbgIdx = 0;
		        }
		        _u4NextFbgId = u4TargetFbgIdx;
		    }

		    if (++u4TargetFbgIdx >= FBG_MAX_NS)
		    {
		        u4TargetFbgIdx = 0;
		    }
		}
		if (u4FbgId == FBM_FBG_ID_UNKNOWN)
		{
		    VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
		    return FBM_FBG_ID_UNKNOWN;
		}

		LOG(1,"FBM_CreateGroupExt u4FbgId %d\n",u4FbgId);

#if defined(CC_H264_LV5_SUPPORT_SHARE_POOL)
		if (u4VDecFmt == FBM_VDEC_H264 && prPar && (prPar->u1VdecProfile & FBM_H264_LV5_PROFILE))
		{
		    for (u4FbgIdx = 0; u4FbgIdx < FBG_MAX_NS; u4FbgIdx++)
		    {
		        if (_arFbg[u4FbgIdx].ucFbgId != FBM_FBG_ID_UNKNOWN)
		        {
		            LOG(0, "Create FBG error: Could not find enough MPEG pools to support H264 LV5 profile!\n");
		            VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
		            return FBM_FBG_ID_UNKNOWN;
		        }
		    }
		    // enable share mpeg pool
		    _arFbg[u4FbgId].fgEnableH264V5Support = TRUE;
		}
#elif defined(CC_H264_LV5_SUPPORT_SINGLE_POOL)
		if (u4VDecFmt == FBM_VDEC_H264 && prPar && (prPar->u1VdecProfile & FBM_H264_LV5_PROFILE))
		{
		    for (u4FbgIdx = 0; u4FbgIdx < FBG_MAX_NS; u4FbgIdx++)
		    {
		        if (_arFbg[u4FbgIdx].ucFbgId != FBM_FBG_ID_UNKNOWN && _arFbg[u4FbgIdx].fgEnableH264V5Support == TRUE)
		        {
		            LOG(0, "Create FBG error: Extra MPEG pool is used by H264 for LV5 profile!\n");
		            VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
		            return FBM_FBG_ID_UNKNOWN;
		        }
		    }

		    // enable share mpeg pool
		    _arFbg[u4FbgId].fgEnableH264V5Support = TRUE;
		}
#endif

#ifdef FBM_CS_LOG
		_u4EQHandle = (UINT32)_arFbg[u4FbgId].hEmptyQSemaphore.hBinary;
#endif

		_arFbg[u4FbgId].u4VDecFmt = u4VDecFmt;
		_arFbg[u4FbgId].fg10Bit = prPar?prPar->fg10Bit:0;
		_arFbg[u4FbgId].fgUFO   = prPar?prPar->fgUFO:0;
			
		// Clear Flag
		_arFbg[u4FbgId].u4FbgFlag = 0;

		// Determine number of frame buffer, later
		_arFbg[u4FbgId].ucFbNs = FBM_MAX_FB_NS_PER_GROUP;

		// Allocalte memory for picture header
		u4MallocSize = _arFbg[u4FbgId].ucFbNs * (sizeof(FBM_PIC_HDR_T));
		_arFbg[u4FbgId].pMallocAddr = x_mem_alloc(u4MallocSize);
		if (_arFbg[u4FbgId].pMallocAddr == NULL)
		{
		    VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
		    return FBM_FBG_ID_UNKNOWN;
		}
		FBM_MEMSET(_arFbg[u4FbgId].pMallocAddr, 0, u4MallocSize);
		_arFbg[u4FbgId].prPicHdr = (FBM_PIC_HDR_T *)_arFbg[u4FbgId].pMallocAddr;

		// Init frame buffer status
		for (u4FbIdx = 0; u4FbIdx < _arFbg[u4FbgId].ucFbNs; u4FbIdx++)
		{
		    _arFbg[u4FbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_UNKNOWN;
            _arFbg[u4FbgId].u4DispTag[u4FbIdx] = INVALID_DISPTAG;			
		    _arFbg[u4FbgId].prPicHdr[u4FbIdx].ucSeamlessResizeFbid = FBM_FB_ID_UNKNOWN;
		    _arFbg[u4FbgId].prPicHdr[u4FbIdx].ucSeamlessResize2Fbid = FBM_FB_ID_UNKNOWN;		    
		}
	}
	
	_QueryFixedBuffer(&u4FixedAddr,&u4FixedSize);

    if (prPar && (( prPar->u4TargetW > FBM_FBG_1080HD_H) || (prPar->u4TargetH > FBM_FBG_1080HD_V)))
    {
		_arFbg[u4FbgId].eResolutionChangeMode = FBM_RESOLUTION_CHANGE_MODE_4K2K;
    }
	else
	{
		_arFbg[u4FbgId].eResolutionChangeMode = FBM_RESOLUTION_CHANGE_MODE_FHD;
	}
	
    if (prPar && prPar->fgExternalAddr)
    {
        //Not alloc
        LOG(1, "Not alloc FB Pool As FB addr will set external.\n");
    }
    else if (prPar && prPar->fgCreateFromInst)
    {
        _arFbg[u4FbgId].fgCreateFromInst = TRUE;
    }
#if (defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST)) && defined(CC_FBM_FULL_3D_SUPPORT) || defined (FBM_4K1K_VIDEO_SUPPORT)
    else if(ucFbgType == FBM_FBG_TYPE_SBSHD || ucFbgType == FBM_FBG_TYPE_TNBHD)
    {
        UINT32 u4PoolSize = 0;
        _arFbg[u4FbgId].u4FbMemoryPool = 0;
        _arFbg[u4FbgId].u4FbMemoryPool2 = 0;
		_arFbg[u4FbgId].u4FbMemoryPool2C = 0;
        _arFbg[u4FbgId].u4FbMemoryPoolC = 0;
		
        _arFbg[u4FbgId].u4FbUsingPoolYSize = 0;
        _arFbg[u4FbgId].u4FbUsingPool2YSize = 0;		
        _arFbg[u4FbgId].u4FbUsingPoolCSize = 0;
        _arFbg[u4FbgId].u4FbUsingPool2CSize = 0;
        
        u4PoolSize = _FBM_PoolAllocate(FBM_FBG_TYPE_1080HD, &_arFbg[u4FbgId].u4FbMemoryPool,
                                        &_arFbg[u4FbgId].u4FbMemoryPoolC);
		
	    _arFbg[u4FbgId].u4FbStartPoolYAddr  = _arFbg[u4FbgId].u4FbMemoryPool;
		
	    _arFbg[u4FbgId].u4FbEndPoolYAddr    = _arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - u4FixedSize;
		
	    _arFbg[u4FbgId].u4FbStartPoolCAddr  = _arFbg[u4FbgId].u4FbMemoryPoolC;
	    _arFbg[u4FbgId].u4FbEndPoolCAddr    = _arFbg[u4FbgId].u4FbMemoryPoolC + FBM_MPEG_C_SIZE;		
		
        #ifdef YC_MERGE_IN_CHB 
		
		_arFbg[u4FbgId].u4FbMemoryPoolC     =	_arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - FBM_MPEG_C_SIZE_IN_CHB - u4FixedSize ;

	    _arFbg[u4FbgId].u4FbStartPoolYAddr  = _arFbg[u4FbgId].u4FbMemoryPool;

		_arFbg[u4FbgId].u4FbEndPoolYAddr    = _arFbg[u4FbgId].u4FbMemoryPoolC;
		
		_arFbg[u4FbgId].u4FbStartPoolCAddr  = _arFbg[u4FbgId].u4FbMemoryPoolC;
		
		_arFbg[u4FbgId].u4FbEndPoolCAddr    = _arFbg[u4FbgId].u4FbStartPoolCAddr + FBM_MPEG_C_SIZE_IN_CHB;
		 
		#endif
                                        
        if (u4PoolSize == 0 || (!_arFbg[u4FbgId].u4FbMemoryPool) || (!_arFbg[u4FbgId].u4FbMemoryPoolC))
        {
            if (_arFbg[u4FbgId].pMallocAddr != NULL)
            {
                x_mem_free(_arFbg[u4FbgId].pMallocAddr);
                _arFbg[u4FbgId].pMallocAddr = NULL;
            }
            VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
            return FBM_FBG_ID_UNKNOWN;
        }
        ASSERT(u4PoolSize == FBM_FBG_TYPE_1080HD_H264_SIZE);

        u4PoolSize = _FBM_PoolAllocate(FBM_FBG_TYPE_1080HD, &_arFbg[u4FbgId].u4FbMemoryPool2,
                                        &_arFbg[u4FbgId].u4FbMemoryPool2C);
		
	    _arFbg[u4FbgId].u4FbStartPool2YAddr  = _arFbg[u4FbgId].u4FbMemoryPool2;
		
	    _arFbg[u4FbgId].u4FbEndPool2YAddr    = _arFbg[u4FbgId].u4FbMemoryPool2 + FBM_MPEG_Y_SIZE - u4FixedSize;
		
	    _arFbg[u4FbgId].u4FbStartPool2CAddr  = _arFbg[u4FbgId].u4FbMemoryPool2C;
	    _arFbg[u4FbgId].u4FbEndPool2CAddr    = _arFbg[u4FbgId].u4FbMemoryPool2C + FBM_MPEG_C_SIZE;
		
		
		 #ifdef YC_MERGE_IN_CHB 
		_arFbg[u4FbgId].u4FbMemoryPool2C =	_arFbg[u4FbgId].u4FbMemoryPool2 + FBM_MPEG_Y_SIZE - FBM_MPEG_C_SIZE_IN_CHB - u4FixedSize;

	    _arFbg[u4FbgId].u4FbStartPool2YAddr  = _arFbg[u4FbgId].u4FbMemoryPool2;

		_arFbg[u4FbgId].u4FbEndPool2YAddr    = _arFbg[u4FbgId].u4FbMemoryPool2C;
		
		_arFbg[u4FbgId].u4FbStartPool2CAddr  = _arFbg[u4FbgId].u4FbMemoryPool2C;
		
		_arFbg[u4FbgId].u4FbEndPool2CAddr    = _arFbg[u4FbgId].u4FbStartPool2CAddr + FBM_MPEG_C_SIZE_IN_CHB;
		 
		#endif
																
        if (u4PoolSize == 0 || (!_arFbg[u4FbgId].u4FbMemoryPool2) || (!_arFbg[u4FbgId].u4FbMemoryPool2C))
        {
            _FBM_PoolRelease(_arFbg[u4FbgId].u4FbMemoryPool);
            if (_arFbg[u4FbgId].pMallocAddr != NULL)
            {
                x_mem_free(_arFbg[u4FbgId].pMallocAddr);
                _arFbg[u4FbgId].pMallocAddr = NULL;
            }
            
            VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
            return FBM_FBG_ID_UNKNOWN;
        }
        ASSERT(u4PoolSize == FBM_FBG_TYPE_1080HD_H264_SIZE);
        _arFbg[u4FbgId].u4FbMemoryPoolSize = FBM_FBG_TYPE_1080HD_H264_SIZE*2;
    }
#endif
#ifdef FBM_4K2K_JPG_SUPPORT  // FBM_4K2K_SUPPORT
    else if (ucFbgType == FBM_FBG_TYPE_4K2K_JPEG)
    {
        UINT32 u4PoolSize = 0;
        _arFbg[u4FbgId].u4FbMemoryPool = 0;
        _arFbg[u4FbgId].u4FbMemoryPool2 = 0;
        _arFbg[u4FbgId].u4FbMemoryPoolC = 0;
        _arFbg[u4FbgId].u4FbMemoryPool2C = 0;
        _arFbg[u4FbgId].u4FbMemoryPoolSize = 0;

        _arFbg[u4FbgId].u4FbUsingPoolYSize = 0;
        _arFbg[u4FbgId].u4FbUsingPool2YSize = 0;		
        _arFbg[u4FbgId].u4FbUsingPoolCSize = 0;
        _arFbg[u4FbgId].u4FbUsingPool2CSize = 0;
		        
        u4PoolSize = _FBM_PoolAllocate(FBM_FBG_TYPE_4K2K_JPEG, &_arFbg[u4FbgId].u4FbMemoryPool,
                                        &_arFbg[u4FbgId].u4FbMemoryPoolC);
		

	    _arFbg[u4FbgId].u4FbStartPoolYAddr  = _arFbg[u4FbgId].u4FbMemoryPool;		
	    _arFbg[u4FbgId].u4FbEndPoolYAddr    = _arFbg[u4FbgId].u4FbMemoryPool + FBM_FBG_TYPE_4K2K_JPEG_SIZE;		
	    _arFbg[u4FbgId].u4FbStartPoolCAddr  = _arFbg[u4FbgId].u4FbMemoryPoolC;
	    _arFbg[u4FbgId].u4FbEndPoolCAddr    = _arFbg[u4FbgId].u4FbMemoryPoolC + FBM_FBG_TYPE_4K2K_JPEG_C_SIZE;
		
		#ifdef YC_MERGE_IN_CHB 
		_arFbg[u4FbgId].u4FbMemoryPoolC =	_arFbg[u4FbgId].u4FbMemoryPool + FBM_FBG_TYPE_4K2K_JPEG_SIZE - FBM_FBG_TYPE_4K2K_JPEG_C_SIZE;


		_arFbg[u4FbgId].u4FbStartPoolYAddr  = _arFbg[u4FbgId].u4FbMemoryPool;

		_arFbg[u4FbgId].u4FbEndPoolYAddr    = _arFbg[u4FbgId].u4FbMemoryPoolC;
		
		_arFbg[u4FbgId].u4FbStartPoolCAddr  = _arFbg[u4FbgId].u4FbMemoryPoolC;
		
		_arFbg[u4FbgId].u4FbEndPoolCAddr    = _arFbg[u4FbgId].u4FbStartPoolCAddr + FBM_FBG_TYPE_4K2K_JPEG_C_SIZE;
		
		#endif
        if (u4PoolSize == 0 || (!_arFbg[u4FbgId].u4FbMemoryPool) || (!_arFbg[u4FbgId].u4FbMemoryPoolC))
        {
            if (_arFbg[u4FbgId].pMallocAddr != NULL)
            {
                x_mem_free(_arFbg[u4FbgId].pMallocAddr);
                _arFbg[u4FbgId].pMallocAddr = NULL;
            }
            VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
            return FBM_FBG_ID_UNKNOWN;
        }
        LOG(1,"4K2K_JPEG_POOL_SIZE 0x%0x u4PoolSize 0x%x\n",FBM_FBG_TYPE_4K2K_JPEG_SIZE,u4PoolSize);
        
        _arFbg[u4FbgId].u4FbMemoryPoolSize += u4PoolSize;
        
        //in order to not distinguish linux (4 FB) and android (2 FB) ,now using max (5 FB) case
        
        u4PoolSize = _FBM_PoolAllocate(FBM_FBG_TYPE_4K2K_JPEG, &_arFbg[u4FbgId].u4FbMemoryPool2,
                                        &_arFbg[u4FbgId].u4FbMemoryPool2C);
		
	    _arFbg[u4FbgId].u4FbStartPool2YAddr  = _arFbg[u4FbgId].u4FbMemoryPool2;		
	    _arFbg[u4FbgId].u4FbEndPool2YAddr    = _arFbg[u4FbgId].u4FbMemoryPool2 + FBM_FBG_TYPE_4K2K_JPEG_SIZE;		
	    _arFbg[u4FbgId].u4FbStartPool2CAddr  = _arFbg[u4FbgId].u4FbMemoryPool2C;
	    _arFbg[u4FbgId].u4FbEndPool2CAddr    = _arFbg[u4FbgId].u4FbMemoryPool2C + FBM_FBG_TYPE_4K2K_JPEG_C_SIZE;		
		
		#ifdef YC_MERGE_IN_CHB 		
		_arFbg[u4FbgId].u4FbMemoryPool2C =	_arFbg[u4FbgId].u4FbMemoryPool2 + FBM_FBG_TYPE_4K2K_JPEG_SIZE - FBM_FBG_TYPE_4K2K_JPEG_C_SIZE;

		_arFbg[u4FbgId].u4FbStartPool2YAddr  = _arFbg[u4FbgId].u4FbMemoryPool2;
		_arFbg[u4FbgId].u4FbEndPool2YAddr    = _arFbg[u4FbgId].u4FbMemoryPool2C;		
		_arFbg[u4FbgId].u4FbStartPool2CAddr  = _arFbg[u4FbgId].u4FbMemoryPool2C;		
		_arFbg[u4FbgId].u4FbEndPool2CAddr    = _arFbg[u4FbgId].u4FbStartPool2CAddr + FBM_FBG_TYPE_4K2K_JPEG_C_SIZE;		
		#endif
        if (u4PoolSize == 0 || (!_arFbg[u4FbgId].u4FbMemoryPool2) || (!_arFbg[u4FbgId].u4FbMemoryPool2C))
        {
            _FBM_PoolRelease(_arFbg[u4FbgId].u4FbMemoryPool);
            if (_arFbg[u4FbgId].pMallocAddr != NULL)
            {
                x_mem_free(_arFbg[u4FbgId].pMallocAddr);
                _arFbg[u4FbgId].pMallocAddr = NULL;
            }

            VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
            return FBM_FBG_ID_UNKNOWN;
        }
        
        LOG(1,"4K2K_JPEG_POOL2_SIZE 0x%0x u4PoolSize 0x%x\n",FBM_FBG_TYPE_4K2K_JPEG_SIZE,u4PoolSize);
        
        _arFbg[u4FbgId].u4FbMemoryPoolSize += u4PoolSize;
        
    }
#endif
#ifdef FBM_4K2K_SUPPORT
    else if ((ucFbgType == FBM_FBG_TYPE_4K2K) ||
			 ((_arFbg[u4FbgId].eResolutionChangeMode == FBM_RESOLUTION_CHANGE_MODE_4K2K) &&
			  (_arFbg[u4FbgId].fg4k2KPoolAlocate == FALSE)))
    {
        UINT32 u4PoolSize = 0;       
        
    	if (_arFbg[u4FbgId].fg4k2KPoolAlocate == FALSE)
    	{
            _arFbg[u4FbgId].u4FbMemoryPool = 0;
            _arFbg[u4FbgId].u4FbMemoryPool2 = 0;
            _arFbg[u4FbgId].u4FbMemoryPoolC = 0;
            _arFbg[u4FbgId].u4FbMemoryPool2C = 0;

            _arFbg[u4FbgId].u4FbUsingPoolYSize = 0;
            _arFbg[u4FbgId].u4FbUsingPool2YSize = 0;		
            _arFbg[u4FbgId].u4FbUsingPoolCSize = 0;
            _arFbg[u4FbgId].u4FbUsingPool2CSize = 0;
                    
    		
            u4PoolSize = _FBM_PoolAllocate(FBM_FBG_TYPE_4K2K, &_arFbg[u4FbgId].u4FbMemoryPool,
                                            &_arFbg[u4FbgId].u4FbMemoryPoolC);
    		
    	    _arFbg[u4FbgId].u4FbStartPoolYAddr  = _arFbg[u4FbgId].u4FbMemoryPool;
    		
    	    _arFbg[u4FbgId].u4FbEndPoolYAddr    = _arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE ;
    		
    	    _arFbg[u4FbgId].u4FbStartPoolCAddr  = _arFbg[u4FbgId].u4FbMemoryPoolC;
    	    _arFbg[u4FbgId].u4FbEndPoolCAddr    = _arFbg[u4FbgId].u4FbMemoryPoolC + FBM_MPEG_C_SIZE;
    	                    		
            if (u4PoolSize == 0 || (!_arFbg[u4FbgId].u4FbMemoryPool) || (!_arFbg[u4FbgId].u4FbMemoryPoolC))
            {
                LOG(0,"4K2K Can not allocate pool\n");      
                
                if (_arFbg[u4FbgId].pMallocAddr != NULL)
                {
                    x_mem_free(_arFbg[u4FbgId].pMallocAddr);
                    _arFbg[u4FbgId].pMallocAddr = NULL;
                }
                VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
                return FBM_FBG_ID_UNKNOWN;
            }
            ASSERT(u4PoolSize == FBM_FBG_TYPE_4K2K_7FB_SIZE_PART1);

            u4PoolSize = _FBM_PoolAllocate(FBM_FBG_TYPE_4K2K_EXTRA, &_arFbg[u4FbgId].u4FbMemoryPool2,&_arFbg[u4FbgId].u4FbMemoryPool2C);

    	    _arFbg[u4FbgId].u4FbStartPool2YAddr  = _arFbg[u4FbgId].u4FbMemoryPool2;
    		
    	    _arFbg[u4FbgId].u4FbEndPool2YAddr    = _arFbg[u4FbgId].u4FbMemoryPool2 + FBM_MPEG_Y_SIZE;
    		
    	    _arFbg[u4FbgId].u4FbStartPool2CAddr  = _arFbg[u4FbgId].u4FbMemoryPool2C;
    	    _arFbg[u4FbgId].u4FbEndPool2CAddr    = _arFbg[u4FbgId].u4FbMemoryPool2C + FBM_MPEG_C_SIZE;	

            _arFbg[u4FbgId].u4FbActiveFHDPool  =  _arFbg[u4FbgId].u4FbMemoryPool;
            _arFbg[u4FbgId].u4FbActiveFHDPool2  =  _arFbg[u4FbgId].u4FbMemoryPool2;    
			#if 0           
            _arFbg[u4FbgId].u4FbActiveFHDPoolC =  _arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - FBM_MPEG_C_SIZE_IN_CHB - u4FixedSize;
            _arFbg[u4FbgId].u4FbActiveFHDPool2C =  _arFbg[u4FbgId].u4FbMemoryPool2 + FBM_MPEG_Y_SIZE - FBM_MPEG_C_SIZE_IN_CHB - u4FixedSize;
            #else            
            _arFbg[u4FbgId].u4FbActiveFHDPoolC =  _arFbg[u4FbgId].u4FbMemoryPoolC;
            _arFbg[u4FbgId].u4FbActiveFHDPool2C =  _arFbg[u4FbgId].u4FbMemoryPool2C;
            #endif
            
            _arFbg[u4FbgId].u4FbActive4K2KPool = _arFbg[u4FbgId].u4FbMemoryPool;
            _arFbg[u4FbgId].u4FbActive4K2KPoolC = _arFbg[u4FbgId].u4FbMemoryPoolC;
            _arFbg[u4FbgId].u4FbActive4K2KPool2 = _arFbg[u4FbgId].u4FbMemoryPool2;
            _arFbg[u4FbgId].u4FbActive4K2KPool2C = _arFbg[u4FbgId].u4FbMemoryPool2C;                
            
            if (u4PoolSize == 0 || (!_arFbg[u4FbgId].u4FbMemoryPool2) || (!_arFbg[u4FbgId].u4FbMemoryPool2C))
            {
                LOG(0,"4K2K_EXTRA Can not allocate pool\n");
                _FBM_PoolRelease(_arFbg[u4FbgId].u4FbMemoryPool);
                if (_arFbg[u4FbgId].pMallocAddr != NULL)
                {
                    x_mem_free(_arFbg[u4FbgId].pMallocAddr);
                    _arFbg[u4FbgId].pMallocAddr = NULL;
                }

                VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
                return FBM_FBG_ID_UNKNOWN;
            }
            ASSERT(u4PoolSize == FBM_FBG_TYPE_4K2K_7FB_SIZE_PART2);
            _arFbg[u4FbgId].u4FbMemoryPoolSize = FBM_FBG_TYPE_4K2K_7FB_SIZE_PART1 + FBM_FBG_TYPE_4K2K_7FB_SIZE_PART2;
            
            if (_arFbg[u4FbgId].u4VDecFmt == FBM_VDEC_VP9 )
            {
                _arFbg[u4FbgId].u4FbMemoryPoolSize = FBM_FBG_TYPE_4K2K_SIZE_PART1 + FBM_FBG_TYPE_4K2K_SIZE_PART2;
            }
    	    _arFbg[u4FbgId].fg4k2KPoolAlocate = TRUE;
    	}
    	
    	if ((ucFbgType != FBM_FBG_TYPE_4K2K) && (_arFbg[u4FbgId].fg4k2KPoolAlocate))
    	{
            _ActiveFHDPool(u4FbgId,ucFbgType,u4FixedSize);
    	}
    	else if((ucFbgType == FBM_FBG_TYPE_4K2K) && (_arFbg[u4FbgId].fg4k2KPoolAlocate))
    	{
            _Active4k2kPool(u4FbgId,ucFbgType,u4FixedSize);
    	
            if(prPar && (prPar->ucCftFbgId != FBM_FBG_ID_UNKNOWN) && (prPar->ucCftFbgId < FBG_MAX_NS))
            {
				_rEmptyQState = x_crit_start();            
                _arFbg[u4FbgId].ucFbOldEmptyCnt = _arFbg[u4FbgId].rEmptyQ.ucCount;
                for (u4FbIdx = _arFbg[u4FbgId].ucFbNsBase; u4FbIdx < _arFbg[u4FbgId].ucFbNs; u4FbIdx++)
                {
                    if (_arFbg[u4FbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_UNKNOWN)
                    {
                        _arFbg[u4FbgId].ucFbOldEmptyCnt++;
                    }
                }

                FBM_MEMSET((VOID *)&_arFbg[u4FbgId].rEmptyQ, 0, sizeof(_arFbg[u4FbgId].rEmptyQ));
                
                _arFbg[u4FbgId].ucFbNsOld = _arFbg[u4FbgId].ucFbNs;
                _arFbg[u4FbgId].ucFbNsOldBase = _arFbg[u4FbgId].ucFbNsBase;
                _arFbg[u4FbgId].ucFbNsBase = (_arFbg[u4FbgId].ucFbNsOldBase ? 0 : FBM_MAX_FB_NS_PER_GROUP/2);
                FBM_MEMSET((VOID *)(_arFbg[u4FbgId].aucFbStatus + _arFbg[u4FbgId].ucFbNsBase), FBM_FB_STATUS_UNKNOWN, FBM_MAX_FB_NS_PER_GROUP/2);                
                _arFbg[u4FbgId].eDynamicMode = FBM_DYNAMIC_SAME;
				x_crit_end(_rEmptyQState);				                
            }
    	}    				
    }
#endif
    else
    {
        // Allocate frame buffer pool
        _arFbg[u4FbgId].u4FbMemoryPoolSize = 0;
        _arFbg[u4FbgId].u4FbMemoryPool = 0;
		
        _arFbg[u4FbgId].u4FbUsingPoolYSize = 0;
        _arFbg[u4FbgId].u4FbUsingPool2YSize = 0;		
        _arFbg[u4FbgId].u4FbUsingPoolCSize = 0;
        _arFbg[u4FbgId].u4FbUsingPool2CSize = 0;
		
        //_arFbg[u4FbgId].ucFbNsBase = 0;
        if(prPar && (prPar->ucCftFbgId != FBM_FBG_ID_UNKNOWN) && (prPar->ucCftFbgId < FBG_MAX_NS))
        {
			_rEmptyQState = x_crit_start();    
        	
            _arFbg[u4FbgId].ucFbOldEmptyCnt = _arFbg[u4FbgId].rEmptyQ.ucCount;
            
            for (u4FbIdx = _arFbg[u4FbgId].ucFbNsBase; u4FbIdx < _arFbg[u4FbgId].ucFbNs; u4FbIdx++)
            {
                if (_arFbg[u4FbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_UNKNOWN)
                {
                    _arFbg[u4FbgId].ucFbOldEmptyCnt++;
                    
                }
            }

            FBM_MEMSET((VOID *)&_arFbg[u4FbgId].rEmptyQ, 0, sizeof(_arFbg[u4FbgId].rEmptyQ));
            
            _arFbg[u4FbgId].ucFbNsOld = _arFbg[u4FbgId].ucFbNs;
            _arFbg[u4FbgId].ucFbNsOldBase = _arFbg[u4FbgId].ucFbNsBase;
            _arFbg[u4FbgId].ucFbNsBase = (_arFbg[u4FbgId].ucFbNsOldBase ? 0 : FBM_MAX_FB_NS_PER_GROUP/2);
            FBM_MEMSET((VOID *)(_arFbg[u4FbgId].aucFbStatus + _arFbg[u4FbgId].ucFbNsBase), FBM_FB_STATUS_UNKNOWN, FBM_MAX_FB_NS_PER_GROUP/2);

            if(_arFbg[u4FbgId].fg4k2KPoolAlocate == FALSE)
            {
                if ((prPar->eSeamlessMode & SEAMLESS_2FBS_2POOL) == SEAMLESS_2FBS_2POOL)
                {
                    _arFbg[u4FbgId].u4FbMemoryPoolSize = _FBM_PoolAllocate(ucFbgType, &_arFbg[u4FbgId].u4FbMemoryPool,&_arFbg[u4FbgId].u4FbMemoryPoolC);

    				_arFbg[u4FbgId].u4FbStartPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool;				
    				_arFbg[u4FbgId].u4FbEndPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - u4FixedSize;				
    				_arFbg[u4FbgId].u4FbStartPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;
    				_arFbg[u4FbgId].u4FbEndPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC + FBM_MPEG_C_SIZE;	
    				
    				#ifdef YC_MERGE_IN_CHB 
    				_arFbg[u4FbgId].u4FbMemoryPoolC = 	_arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - FBM_MPEG_C_SIZE_IN_CHB - u4FixedSize;

    				_arFbg[u4FbgId].u4FbStartPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool;				
    				_arFbg[u4FbgId].u4FbEndPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;				
    				_arFbg[u4FbgId].u4FbStartPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;				
    				_arFbg[u4FbgId].u4FbEndPoolCAddr	= _arFbg[u4FbgId].u4FbStartPoolCAddr + FBM_MPEG_C_SIZE_IN_CHB;				
    				#endif
                }
                if (_arFbg[u4FbgId].u4FbMemoryPoolSize && _arFbg[u4FbgId].u4FbMemoryPool)//alloc success
                {
                    _arFbg[u4FbgId].eDynamicMode = FBM_DYNAMIC_DIFF;
                }
                else
                {
    			    UINT32 u4RequestPoolSize =0;
                    _arFbg[u4FbgId].u4FbMemoryPool = _arFbg[u4FbgId].u4FbMemoryPool2;
    				if (_FBM_QueryCurPoolSize(ucFbgType,&u4RequestPoolSize))
    				{
    					_arFbg[u4FbgId].u4FbMemoryPoolSize = u4RequestPoolSize;
    				}
    				else
    				{
    					_arFbg[u4FbgId].u4FbMemoryPoolSize = _arFbg[u4FbgId].u4FbMemoryPool2Size;
    				}
                    _arFbg[u4FbgId].eDynamicMode = FBM_DYNAMIC_SAME;
                }            
                
            }
            else
            {
                _ActiveFHDPool(u4FbgId,ucFbgType,u4FixedSize); 
                _arFbg[u4FbgId].eDynamicMode = FBM_DYNAMIC_SAME;
            }
			
			x_crit_end(_rEmptyQState);			

        }
        else
        {
            _arFbg[u4FbgId].u4FbMemoryPoolSize = _FBM_PoolAllocate(ucFbgType, &_arFbg[u4FbgId].u4FbMemoryPool,&_arFbg[u4FbgId].u4FbMemoryPoolC);

			_arFbg[u4FbgId].u4FbStartPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool;				
			_arFbg[u4FbgId].u4FbEndPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - u4FixedSize;				
			_arFbg[u4FbgId].u4FbStartPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;
			_arFbg[u4FbgId].u4FbEndPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC + FBM_MPEG_C_SIZE;
			
			#ifdef YC_MERGE_IN_CHB 
			_arFbg[u4FbgId].u4FbMemoryPoolC =	_arFbg[u4FbgId].u4FbMemoryPool + FBM_MPEG_Y_SIZE - FBM_MPEG_C_SIZE_IN_CHB - u4FixedSize;
			
			_arFbg[u4FbgId].u4FbStartPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPool;				
			_arFbg[u4FbgId].u4FbEndPoolYAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;				
			_arFbg[u4FbgId].u4FbStartPoolCAddr	= _arFbg[u4FbgId].u4FbMemoryPoolC;				
			_arFbg[u4FbgId].u4FbEndPoolCAddr	= _arFbg[u4FbgId].u4FbStartPoolCAddr + FBM_MPEG_C_SIZE_IN_CHB;			
			#endif
			
        }
        LOG(2, "AllocFBM = Y:(0x%8x) C:(0x%x) Pool Size:(0x%x).\n", _arFbg[u4FbgId].u4FbMemoryPool,_arFbg[u4FbgId].u4FbMemoryPoolC,_arFbg[u4FbgId].u4FbMemoryPoolSize);
            
        if ((_arFbg[u4FbgId].u4FbMemoryPoolSize == 0) ||
                (!_arFbg[u4FbgId].u4FbMemoryPool) || (!_arFbg[u4FbgId].u4FbMemoryPoolC))
        {
            if (_arFbg[u4FbgId].pMallocAddr != NULL)
            {
                x_mem_free(_arFbg[u4FbgId].pMallocAddr);
                _arFbg[u4FbgId].pMallocAddr = NULL;
            }
            VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
            return FBM_FBG_ID_UNKNOWN;
        }
    }

    // Settings
    _arFbg[u4FbgId].eSeamlessMode = prPar ? prPar->eSeamlessMode : SEAMLESS_NONE;
    if (((_arFbg[u4FbgId].eSeamlessMode & SEAMLESS_2FBS_2POOL) == SEAMLESS_2FBS_2POOL) && 
        (_arFbg[u4FbgId].eDynamicMode != FBM_DYNAMIC_DIFF))
    {
        _arFbg[u4FbgId].eSeamlessMode &= ~(SEAMLESS_2FBS_2POOL);
        _arFbg[u4FbgId].eSeamlessMode |= (SEAMLESS_ONE_FBS);
    }

    _arFbg[u4FbgId].ucFbgType = ucFbgType;
    _arFbg[u4FbgId].ucFbgId = (UCHAR)u4FbgId;

    if(prPar && prPar->fgSwHEVC)
        _arFbg[u4FbgId].fgCreateFromInst = TRUE;
	
   // Config color mode		
	LOG(1, "Config FBG(%d): SeamlessMode(%x) FBNs(%d-%d %d-%d)",
			u4FbgId,_arFbg[u4FbgId].eSeamlessMode, _arFbg[u4FbgId].ucFbNsOldBase, _arFbg[u4FbgId].ucFbNsOld, _arFbg[u4FbgId].ucFbNsBase, _arFbg[u4FbgId].ucFbNs);

 
    LOG(1, " Pool [Y addr 0x%x] and  [C addr 0x%x] [RealSize(Y+C) 0x%x] EmptyCnt(%d).\n", 
            _arFbg[u4FbgId].u4FbMemoryPool,
            _arFbg[u4FbgId].u4FbMemoryPoolC,            
            _arFbg[u4FbgId].u4FbMemoryPoolSize, _arFbg[u4FbgId].ucFbOldEmptyCnt);
	
    if (prPar && prPar->fgCreateFromInst)
    {
        FBM_PreConfigColorMode((UCHAR)(u4FbgId), _u1FbgColorMode, u4HSize, u4VSize, prPar);
    }
    else
    {
        FBM_ConfigColorMode((UCHAR)(u4FbgId), _u1FbgColorMode, u4HSize, u4VSize, prPar);
    }
    // Detect new seamless for 3D
    _DetectNewSeamless(u4FbgId, u4HSize, u4VSize, prPar);

    _u1FbgColorMode = FBM_CM_420;

#ifdef CC_B2R_RES_SUPPORT
    if(prPar &&
        !prPar->fgThumbnailMode)
    {
        LOG(1,"[B2R_RES]FBM create fbg b2r_Res enter,u4FbgId(%d)!\n", u4FbgId); 

        prVdecEsInfo = _VDEC_GetEsInfo(prPar->u1VdecId);
        if(prVdecEsInfo == NULL)
        {
            LOG(1,"!!!!!!!!!!!Can not get ESinfo with vdecid(%d)\n",prPar->u1VdecId);
        }
        
        switch(u4VDecFmt)
        {
            case FBM_VDEC_H265:
                t_b2r_res.t_vd.eCodec = VDEC_H265;
                break;
            case FBM_VDEC_JPEG:
                t_b2r_res.t_vd.eCodec = VDEC_JPEG;
                break;
            default:
                t_b2r_res.t_vd.eCodec = VDEC_OTHER;
                break;
        }

        t_b2r_res.t_vd.fgUfoEn = prPar->fgUFO;
        t_b2r_res.t_vd.u1PixDepth = prPar->fg10Bit ? 10 : 8;
        t_b2r_res.t_vd.u4Resolution = u4HSize * u4VSize;

        if(prVdecEsInfo != NULL)
        {
            t_b2r_res.u1VdpId = prVdecEsInfo->u4RenderVDPId;
        }

        

        LOG(1,"[B2R_RES]FBM create fbg b2r_Res info:codec(%d),fgUfoEn(%d),PixelDepth(%d),Resolution(%d) with VDPID(%d)!\n", 
            t_b2r_res.t_vd.eCodec,t_b2r_res.t_vd.fgUfoEn,t_b2r_res.t_vd.u1PixDepth,t_b2r_res.t_vd.u4Resolution,t_b2r_res.u1VdpId);
        
        FBM_B2rResIdAccess(u4FbgId, RES_RW, &t_b2r_res);
    }
#endif

    // [LOG] FBM Create
    SYSLOG(FBM_PREFIX + 99, u4FbgId, ucFbgType, 0);

    // Apply global value
    if(FBM_ChkFrameBufferGlobalFlagIsr(0xFF, FBM_FLAG_FB_DECODE_ONLY))
    {
        _arFbg[u4FbgId].u4FbgFlag |= FBM_FLAG_DISP_READY;
    }
    if (prPar && prPar->fgThumbnailMode)
    {
        _arFbg[u4FbgId].u4FbgFlag |= FBM_FLAG_THUMBNAIL_MODE;
        _arFbg[u4FbgId].u4FbgFlag |= FBM_FLAG_DISP_READY;
    }

    ASSERT(_arFbg[u4FbgId].hEmptyQSemaphore.hMutex == _arFbg[u4FbgId].hMutex);
    ASSERT(_arFbg[u4FbgId].hEmptyBQSemaphore.hMutex == _arFbg[u4FbgId].hMutex);

    VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
	ucB2rId= FBM_B2rResIdAccess(u4FbgId, RES_R, NULL);
    LG_PipLineConnect(VDP_1,ucB2rId);
    return FBM_BYTE(u4FbgId);
}

//-------------------------------------------------------------------------
/** FBM_CreateGroup
 *  Create FBM Group
 *  @param  ucFbgType   FBG type.
 *  @return FBG index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_CreateGroup(UCHAR ucFbgType, UINT32 u4VDecFmt, UINT32 u4HSize, UINT32 u4VSize)
{
    UCHAR ucFbpListNs;

    UINT32 u4FbgTypeValid;
    UINT32 u4MallocSize;

    UINT32 u4FbgId;
    UINT32 u4FbgIdx;
    UINT32 u4TargetFbgIdx;
    UINT32 u4ListIdx;
    UINT32 u4FbIdx;

    FBM_CREATE_FBG_PAR_T rPar;
    FBP_LIST_T* prFbpList;

    VERIFY(x_sema_lock(_hFbgMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    _FBM_GetPoolList(&prFbpList, &ucFbpListNs);

    // Check frame buffer group type
    u4FbgTypeValid = 0;
    for (u4ListIdx = 0; u4ListIdx < ucFbpListNs; u4ListIdx++)
    {
        if (ucFbgType == prFbpList[u4ListIdx].ucType)
        {
            u4FbgTypeValid = 1;
        }
    }
    if ((u4FbgTypeValid == 0) || (u4VDecFmt == FBM_VDEC_UNKNOWN))
    {
        VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
        return FBM_FBG_ID_UNKNOWN;
    }

    // Search for empty frame buffer group
    u4FbgId = FBM_FBG_ID_UNKNOWN;
    u4TargetFbgIdx = _u4NextFbgId;
    for (u4FbgIdx = 0; u4FbgIdx < FBG_MAX_NS; u4FbgIdx++)
    {
        if (_arFbg[u4TargetFbgIdx].ucFbgId == FBM_FBG_ID_UNKNOWN)
        {
            u4FbgId = u4TargetFbgIdx;

            u4FbgIdx = FBG_MAX_NS;

            if (++u4TargetFbgIdx >= FBG_MAX_NS)
            {
                u4TargetFbgIdx = 0;
            }
            _u4NextFbgId = u4TargetFbgIdx;
        }

        if (++u4TargetFbgIdx >= FBG_MAX_NS)
        {
            u4TargetFbgIdx = 0;
        }
    }
    if (u4FbgId == FBM_FBG_ID_UNKNOWN)
    {
        VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
        return FBM_FBG_ID_UNKNOWN;
    }

#ifdef FBM_CS_LOG
    _u4EQHandle = (UINT32)_arFbg[u4FbgId].hEmptyQSemaphore.hBinary;
#endif

    _arFbg[u4FbgId].u4VDecFmt = u4VDecFmt;


    // Clear Flag
    _arFbg[u4FbgId].u4FbgFlag = 0;

    // Determine number of frame buffer, later
    _arFbg[u4FbgId].ucFbNs = FBM_MAX_FB_NS_PER_GROUP;

    // Allocalte memory for picture header
    u4MallocSize = _arFbg[u4FbgId].ucFbNs * (sizeof(FBM_PIC_HDR_T));
    _arFbg[u4FbgId].pMallocAddr = x_mem_alloc(u4MallocSize);
    if (_arFbg[u4FbgId].pMallocAddr == NULL)
    {
        VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
        return FBM_FBG_ID_UNKNOWN;
    }
    FBM_MEMSET(_arFbg[u4FbgId].pMallocAddr, 0, u4MallocSize);
    _arFbg[u4FbgId].prPicHdr = (FBM_PIC_HDR_T *)_arFbg[u4FbgId].pMallocAddr;

    // Init frame buffer status
    for (u4FbIdx = 0; u4FbIdx < _arFbg[u4FbIdx].ucFbNs; u4FbIdx++)
    {
        _arFbg[u4FbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_EMPTY;
    }

    // Allocate frame buffer pool
    _arFbg[u4FbgId].u4FbMemoryPoolSize = _FBM_PoolAllocate(ucFbgType, &_arFbg[u4FbgId].u4FbMemoryPool,&_arFbg[u4FbgId].u4FbMemoryPoolC);

    if ((_arFbg[u4FbgId].u4FbMemoryPoolSize == 0) ||
            (_arFbg[u4FbgId].u4FbMemoryPool == 0))
    {
        if (_arFbg[u4FbgId].pMallocAddr)
        {
            x_mem_free(_arFbg[u4FbgId].pMallocAddr);
            _arFbg[u4FbgId].pMallocAddr = NULL;
        }
        VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
        return FBM_FBG_ID_UNKNOWN;
    }

    // Settings
    _arFbg[u4FbgId].ucFbgType = ucFbgType;
    _arFbg[u4FbgId].ucFbgId = (UCHAR)u4FbgId;

    // Config color mode

    x_memset(&rPar, 0, sizeof(FBM_CREATE_FBG_PAR_T));
    rPar.u1AppMode = FBM_FBG_APP_NORMAL;
    FBM_ConfigColorMode((UCHAR)(u4FbgId), _u1FbgColorMode, u4HSize, u4VSize, &rPar);

    _u1FbgColorMode = FBM_CM_420;

    // [LOG] FBM Create
    SYSLOG(FBM_PREFIX + 99, u4FbgId, ucFbgType, 0);

    // Apply global value
    if(FBM_ChkFrameBufferGlobalFlagIsr(0xFF, FBM_FLAG_FB_DECODE_ONLY))
    {
        _arFbg[u4FbgId].u4FbgFlag |= FBM_FLAG_DISP_READY;
    }

    ASSERT(_arFbg[u4FbgId].hEmptyQSemaphore.hMutex == _arFbg[u4FbgId].hMutex);
    ASSERT(_arFbg[u4FbgId].hEmptyBQSemaphore.hMutex == _arFbg[u4FbgId].hMutex);

    VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);

    return FBM_BYTE(u4FbgId);
}


//-------------------------------------------------------------------------
/** FBM_ReleaseGroup
 *  Release FBM Group
 *  @param  ucFbgType   FBG type.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ReleaseGroup(UCHAR ucFbgId)
{
    // Notes about mutex semaphore
    // We will not destroy mutex semaphore at release
    // Because OSAI itself is NOT threadsafe
    // Semaphore release function is dangerous

    void* pMallocAddr;
    void* pWorkBufVirAddr = NULL;
    UINT32 u4FbMemoryPool;
    UINT32 u4FbMemoryPool2;    
    UINT32 u4FbActive4K2KPool;
    UINT32 u4FbActive4K2KPool2;  	
    HANDLE_T hMutex;
    UCHAR    ucFbgType;
    BOOL   fg4k2KPoolAlocate;
    UCHAR    ucIdx = 0;
    BOOL     bIsWaiting = FALSE;
    static  UINT32 u4WaitingCnt = 0;

    COUNTING_SEMAPHORE_T         hEmptyQSemaphore;    // Empty Queue Counting Semaphore
    COUNTING_SEMAPHORE_T         hEmptyBQSemaphore;    // Empty B Queue Counting Semaphore
    COUNTING_SEMAPHORE_T         hEmptyResizeQSemaphore;
    COUNTING_SEMAPHORE_T         hEmptyResize2QSemaphore;
    UNUSED(u4FbMemoryPool2);
    UNUSED(pWorkBufVirAddr);
    UNUSED(ucFbgType);
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    
    if ((_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_MPEG4) || (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8))
    {
        do 
        {
            // query emptyQ status
           FBM_MUTEX_LOCK(ucFbgId);             
            bIsWaiting = FALSE;
            for(ucIdx = _arFbg[ucFbgId].ucFbNsBase; ucIdx < _arFbg[ucFbgId].ucFbNs; ucIdx++)
            {
                if (_arFbg[ucFbgId].aucFbStatus[ucIdx] != FBM_FB_STATUS_EMPTY && _arFbg[ucFbgId].u4DispTag[ucIdx] != INVALID_DISPTAG)
                {
                    LOG(0,"Need waiting cnt(%d) Release all empty Que Fb(%d) FbStatus(%d)\n",u4WaitingCnt,ucIdx,_arFbg[ucFbgId].aucFbStatus[ucIdx]);
                    u4WaitingCnt++;
                    bIsWaiting = TRUE;  
                    
                    if (u4WaitingCnt > 100)
                    {
                         bIsWaiting = FALSE;
                         u4WaitingCnt = 0;
                         LOG(0,"Force stop Waiting Fb Empty\n");
                    }                    
                    break;
                }
                
            }

            if (bIsWaiting)
            {
                LOG(0,"Waiting Release all empty Que\n");
                FBM_MUTEX_UNLOCK(ucFbgId);
                x_thread_delay(10);
                continue;
            }
            LOG(0,"Waited Release all empty Que\n");
            FBM_MUTEX_UNLOCK(ucFbgId);       
            break;            
        }while(1);
        
    }

    LOG(2, "[%s-%d]Fbg(%d) release.\n", __func__, __LINE__, ucFbgId);
    VERIFY(x_sema_lock(_hFbgMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    FBM_MUTEX_LOCK(ucFbgId);

#ifdef CC_B2R_RES_SUPPORT
    FBM_B2rResIdRelease(ucFbgId);
#endif
    LG_PipLineDisconnect(VDP_1);
    // [LOG] FBM Release
    SYSLOG(FBM_PREFIX + 98, ucFbgId, _arFbg[ucFbgId].ucFbgType, 0);

    u4FbMemoryPool = _arFbg[ucFbgId].u4FbMemoryPool;
    u4FbMemoryPool2 = _arFbg[ucFbgId].u4FbMemoryPool2;
	u4FbActive4K2KPool = _arFbg[ucFbgId].u4FbActive4K2KPool;
	u4FbActive4K2KPool2 = _arFbg[ucFbgId].u4FbActive4K2KPool2;
    pMallocAddr = _arFbg[ucFbgId].pMallocAddr;
    ucFbgType = _arFbg[ucFbgId].ucFbgType;
    fg4k2KPoolAlocate = _arFbg[ucFbgId].fg4k2KPoolAlocate;
    pWorkBufVirAddr = _arFbg[ucFbgId].pWorkBufVirAddr;
	
    // backup semaphore
    hMutex = _arFbg[ucFbgId].hMutex;

    hEmptyQSemaphore = _arFbg[ucFbgId].hEmptyQSemaphore;
    hEmptyBQSemaphore = _arFbg[ucFbgId].hEmptyBQSemaphore;
    hEmptyResizeQSemaphore = _arFbg[ucFbgId].hEmptyResizeQSemaphore;
    hEmptyResize2QSemaphore = _arFbg[ucFbgId].hEmptyResize2QSemaphore;
    
    if (_arFbg[ucFbgId].fgEnableNewSeamless)
    {
        FBM_MUTEX_UNLOCK(ucFbgId); //unlock first for VDP Prescale thread exit normally.
        _VDP_Prescale_Close(_arFbg[ucFbgId].ucSeamlessRzId);
        if( ((_arFbg[ucFbgId].rSeqHdr.eSeamlessMode & SEAMLESS_TWO_RESIZER) == SEAMLESS_TWO_RESIZER) 
          && (_arFbg[ucFbgId].ucSeamlessRzId2 != VDP_PRESCALE_RZ_ID_UNKNOWN))
        {
            _VDP_Prescale_Close(_arFbg[ucFbgId].ucSeamlessRzId2);
        }
        FBM_MUTEX_LOCK(ucFbgId);
        _FreeSeamlessBuffer(_arFbg[ucFbgId].u4FbResizeMemoryPool);
		if (fg4k2KPoolAlocate)
		{
			_FreeSeamlessBuffer(_arFbg[ucFbgId].u4FbResizeMemoryPool2);
		}
    }
    // Clear frame buffer group
    FBM_MEMSET(((void*)&_arFbg[ucFbgId]), 0, sizeof(FBM_FBG_T));

    // Init Settings, refer to FBM_Init
    _arFbg[ucFbgId].ucFbgType = FBM_FBG_TYPE_UNKNOWN;
    _arFbg[ucFbgId].ucFbgId = FBM_FBG_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbgCm = FBM_CM_UNKNOWN;
    _arFbg[ucFbgId].ucStcSrc = STC_SRC_NS;
    _arFbg[ucFbgId].ucAvSyncMode = VID_SYNC_MODE_NS;
    _arFbg[ucFbgId].ucPlayMode = FBM_FBG_DTV_MODE;
    _arFbg[ucFbgId].u4AvSyncStatus = FBM_FBG_AVSYNC_DISABLE;

    _arFbmCbFunc.aau4CbFunc[ucFbgId][FBM_CB_FUNC_FB_READY_EX_IND] = 0;
    _arFbmCbFunc.aau4CbFunc[ucFbgId][FBM_CB_FUNC_FBG_CHG_EX_IND] = 0;
    for (ucIdx = 0; ucIdx < _arFbg[ucIdx].ucFbNs; ucIdx++)
    {
        _arFbg[ucFbgId].u4DispTag[ucIdx] = INVALID_DISPTAG;
    }
    
    _arFbg[ucFbgId].fg4k2KPoolAlocate = FALSE;  
    // restore semaphore
    _arFbg[ucFbgId].hMutex = hMutex;

#ifdef CC_VDP_FULL_ISR
    _arFbg[ucFbgId].prState = &_rState;
#endif

    _arFbg[ucFbgId].hEmptyQSemaphore = hEmptyQSemaphore;
    _arFbg[ucFbgId].hEmptyBQSemaphore = hEmptyBQSemaphore;
    _arFbg[ucFbgId].hEmptyResizeQSemaphore = hEmptyResizeQSemaphore;
    _arFbg[ucFbgId].hEmptyResize2QSemaphore = hEmptyResize2QSemaphore;
    
    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyResizeQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyResize2QSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    
	if (fg4k2KPoolAlocate)
	{
        _FBM_PoolRelease(u4FbActive4K2KPool);
        _FBM_PoolRelease(u4FbActive4K2KPool2);		
	}	
    // Release frame buffer pool
    else if (!_arFbg[ucFbgId].fgCreateFromInst)
    {
#if ((defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)  || defined(CC_VOMX_TV_COEXIST)) && defined(CC_FBM_FULL_3D_SUPPORT)) || defined(FBM_4K2K_JPG_SUPPORT) ||defined(FBM_4K2K_SUPPORT) ||defined(FBM_4K1K_VIDEO_SUPPORT)
    if (ucFbgType == FBM_FBG_TYPE_SBSHD || ucFbgType == FBM_FBG_TYPE_TNBHD ||ucFbgType == FBM_FBG_TYPE_4K2K_JPEG  || ucFbgType == FBM_FBG_TYPE_4K2K || fg4k2KPoolAlocate)
    {         
        _FBM_PoolRelease(u4FbMemoryPool);
        _FBM_PoolRelease(u4FbMemoryPool2);        
    }
    else
#endif
    {
        _FBM_PoolRelease(u4FbMemoryPool);
    }
    }
    // Release memory
    if (pMallocAddr != NULL)
    {
        x_mem_free(pMallocAddr);
    }
#ifdef CC_VDEC_FBM_DYNAMIC_MAPPING
    if (pWorkBufVirAddr)
    {
        BSP_UnMapReservedMem(pWorkBufVirAddr);
    }
#endif

    FBM_MUTEX_UNLOCK(ucFbgId);
    VERIFY(x_sema_unlock(_hFbgMutex) == OSR_OK);
    
    // TODO, to have vdec es id
#ifdef CC_53XX_SWDMX_V2
    VDEC_FbgReleaseHdlr(_arFbg[ucFbgId].u1DecoderSrcId);
#else
    VDEC_FbgReleaseHdlr();
#endif
}


//-------------------------------------------------------------------------
/** FBM_ResetGroup
 *  Reset FBM Group
 *  @param  ucFbgType   FBG type.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ResetGroup(UCHAR ucFbgId)
{
    // Note about usage
    // When FBM_FLAG_RESET flag was set by vdp
    // 5381 Â²¤Æ, 20070403, Refer to FBM_ConfigColorMode

    UINT32 u4FbIdx;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    if (VERIFY_FB_NS(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if (_arFbg[ucFbgId].fgCreateFromInst)
    {
        ASSERT(0);
#if 0        
        ASSERT(_arFbg[ucFbgId].rEmptyQ.ucCount != 0);
        for (u4FbIdx = 0; ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
        {
            if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] != FBM_FB_STATUS_EMPTY)
            {
                _arFbg[ucFbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_RECYCLE;
            }
        }

        // clear display queue
        _arFbg[ucFbgId].rDisplayQ.ucReadIdx = 0;
        _arFbg[ucFbgId].rDisplayQ.ucCount = 0;
        
        // Init Settings
        _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbFRefValid = 0;
        _arFbg[ucFbgId].ucFbBRefValid = 0;
        
        // default flag
        _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_NOT_READY;
#endif        
        FBM_MUTEX_UNLOCK(ucFbgId);
        return;
    }
    // put all frame buffer into empty queue
    _arFbg[ucFbgId].rEmptyQ.ucCount = 0;
    for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase;
            ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
    {
        _FBM_PutFrameBufferToEmptyQ(ucFbgId, (UCHAR)(u4FbIdx));
        _arFbg[ucFbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_EMPTY;
    }

    // put all frame buffer into empty queue
    if (_arFbg[ucFbgId].fgEnableNewSeamless)
    {
        _arFbg[ucFbgId].rResizeEmptyQ.ucCount = 0;
        _arFbg[ucFbgId].rResize2EmptyQ.ucCount = 0;   
        
        for (u4FbIdx = 0; ((u4FbIdx < _arFbg[ucFbgId].ucSeamlessFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
        {
            _FBM_PutFrameBufferToEmptyResizeQ(ucFbgId, (UCHAR)(u4FbIdx));
        }
        
        for (u4FbIdx = 0; ((u4FbIdx < _arFbg[ucFbgId].ucSeamless2FbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
        {
            _FBM_PutFrameBufferToEmptyResize2Q(ucFbgId, (UCHAR)(u4FbIdx));
        }        
    }

    if ((_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
        (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
            (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG))
    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucFbNs) == OSR_OK);
    }
    else
    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucFbNs) == OSR_OK);

        // create semaphore, Empty BQ
        VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)(_arFbg[ucFbgId].ucFbNs - FBM_REF_FB_NS_PER_GROUP)) == OSR_OK);
    }

    VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyResizeQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucSeamlessFbNs) == OSR_OK);

    VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyResize2QSemaphore, _arFbg[ucFbgId].hMutex, (INT32)_arFbg[ucFbgId].ucSeamless2FbNs) == OSR_OK);

    // clear display queue
    _arFbg[ucFbgId].rDisplayQ.ucReadIdx = 0;
    _arFbg[ucFbgId].rDisplayQ.ucCount = 0;

    // Init Settings
    _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbFRefValid = 0;
    _arFbg[ucFbgId].ucFbBRefValid = 0;

    // default flag
    _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_NOT_READY;

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyResizeQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyResize2QSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    
    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_InvalidateRefFrameBuffer
 *  Invalidate FBM Group reference frame buffer
 *  @param  ucFbgType   FBG type.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_InvalidateRefFrameBuffer(UCHAR ucFbgId)
{
    // Note about usage
    // When mpv mode change, invalidate refernce
    // Start again at next I picture

    UINT32 u4FbIdx;
    UINT32 u4LockNs;
    UINT32 u4EmptyNs;
    UINT32 u4EmptyRefNs;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    if (VERIFY_FB_NS(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);
    if (_arFbg[ucFbgId].fgCreateFromInst)
    {
        for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase; ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
        {
            if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_DECODE || 
                _arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_READY ||
                _arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_FIELD_READY)
            {
                FBM_SetFrameBufferStatus(ucFbgId, u4FbIdx, FBM_FB_STATUS_EMPTY); //flush to displayQ
            }
        }

        _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbFRefValid = 0;
        _arFbg[ucFbgId].ucFbBRefValid = 0;
    
        FBM_MUTEX_UNLOCK(ucFbgId);
        return;
    }

    // display can lock 1~2 B picture
    u4LockNs = 0;
    for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase;
            ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
    {
        if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
        {
            u4LockNs++;
        }
    }

#if 1
    // reset counting Semaphore
    _arFbg[ucFbgId].hEmptyBQSemaphore.i4Count = 0;
    _arFbg[ucFbgId].hEmptyQSemaphore.i4Count = 0;
#endif

    // clear empty queue
    _arFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;
    _arFbg[ucFbgId].rEmptyQ.ucCount = 0;

    // clear display queue
    _arFbg[ucFbgId].rDisplayQ.ucReadIdx = 0;
    _arFbg[ucFbgId].rDisplayQ.ucCount = 0;
    _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
//    _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
//    _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;
    _arFbg[ucFbgId].ucFbFRefValid = 0;
    _arFbg[ucFbgId].ucFbBRefValid = 0;

    u4EmptyNs = _arFbg[ucFbgId].ucFbNs - _arFbg[ucFbgId].ucFbNsBase - u4LockNs;

    u4EmptyRefNs = 0;
    if ((_arFbg[ucFbgId].ucFbFRef != FBM_FB_ID_UNKNOWN) &&
            (_arFbg[ucFbgId].aucFbStatus[_arFbg[ucFbgId].ucFbFRef] == FBM_FB_STATUS_LOCK))
    {
    }
    else
    {
        u4EmptyRefNs++;
    }
    if ((_arFbg[ucFbgId].ucFbBRef != FBM_FB_ID_UNKNOWN) &&
            (_arFbg[ucFbgId].aucFbStatus[_arFbg[ucFbgId].ucFbBRef] == FBM_FB_STATUS_LOCK))
    {
    }
    else
    {
        u4EmptyRefNs++;
    }

    // set counting Semaphore
    // Bug fixed 20050708, reference may locked by display
    // _arFbg[ucFbgId].hEmptyBQSemaphore.i4Count = (INT32)(u4EmptyNs - FBM_REF_FB_NS_PER_GROUP) is NOT correct!

    // create semaphore, Empty Q
    VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)0) == OSR_OK);

    // create semaphore, Empty BQ
    VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)(u4EmptyNs - u4EmptyRefNs)) == OSR_OK);

    for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase;
            ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
    {
        if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] != FBM_FB_STATUS_LOCK)
        {
            if (_arFbg[ucFbgId].fgEnableNewSeamless && _arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_DISPLAYQ)
            {
                if (_arFbg[ucFbgId].prPicHdr[u4FbIdx].ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN &&
                    _arFbg[ucFbgId].aucResizeFbStatus[_arFbg[ucFbgId].prPicHdr[u4FbIdx].ucSeamlessResizeFbid] != FBM_FB_STATUS_EMPTY)
                {
                    _FBM_PutFrameBufferToEmptyResizeQ(ucFbgId, _arFbg[ucFbgId].prPicHdr[u4FbIdx].ucSeamlessResizeFbid);
                    VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyResizeQSemaphore) == OSR_OK);
                }
                if (_arFbg[ucFbgId].prPicHdr[u4FbIdx].ucSeamlessResize2Fbid != FBM_FB_ID_UNKNOWN &&
                    _arFbg[ucFbgId].aucResize2FbStatus[_arFbg[ucFbgId].prPicHdr[u4FbIdx].ucSeamlessResize2Fbid] != FBM_FB_STATUS_EMPTY)
                {
                    _FBM_PutFrameBufferToEmptyResize2Q(ucFbgId, _arFbg[ucFbgId].prPicHdr[u4FbIdx].ucSeamlessResize2Fbid);
                    VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyResize2QSemaphore) == OSR_OK);
                }                
            }
            
            _arFbg[ucFbgId].aucFbStatus[u4FbIdx] = FBM_FB_STATUS_EMPTY;
            _FBM_PutFrameBufferToEmptyQ(ucFbgId, (UCHAR)(u4FbIdx));
        }
    }

    LOG(7, "Invalidate G(%d) S(%d %d %d %d) R(%02x %02x) Q(%d %d %d %d)\n",
        ucFbgId,
        _arFbg[ucFbgId].aucFbStatus[0],
        _arFbg[ucFbgId].aucFbStatus[1],
        _arFbg[ucFbgId].aucFbStatus[2],
        _arFbg[ucFbgId].aucFbStatus[3],
        _arFbg[ucFbgId].ucFbFRef,
        _arFbg[ucFbgId].ucFbBRef,
        _arFbg[ucFbgId].rDisplayQ.ucCount,
        _arFbg[ucFbgId].rEmptyQ.ucCount,
        _arFbg[ucFbgId].hEmptyQSemaphore.i4Count,
        _arFbg[ucFbgId].hEmptyBQSemaphore.i4Count);

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.i4Count >= 0);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.i4Count >= 0);

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_WaitUnlockFrameBuffer
 *  Wait FBM Group unlock frame buffer
 *  @param  ucFbgType   FBG type.
 *  @param  u4Timeout   time to wait.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_WaitUnlockFrameBuffer(UCHAR ucFbgId, UINT32 u4Timeout)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    if (VERIFY_FB_NS(ucFbgId))
    {
        return;
    }

    if (_arFbg[ucFbgId].fgCreateFromInst)
    {
        return;
    }
    FBM_MUTEX_LOCK(ucFbgId);

    {
        UINT32 u4FbIdx;
        UINT32 u4LockFbNs;
        UINT32 u4EmptyFbNs;

        // unlock frame buffer Ns
        u4LockFbNs = 0;
        u4EmptyFbNs = 0;
        for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase;
                ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
        {
            if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
            {
                u4LockFbNs++;
            }
            else if (_arFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_EMPTY)
            {
                u4EmptyFbNs++;
            }
        }

        // wait for lock frame buffer
        if (u4LockFbNs != 0)
        {
            u4FbIdx = 0;
            while (u4FbIdx < (u4LockFbNs + u4EmptyFbNs))
            {
                if (_FBM_csema_lock(&_arFbg[ucFbgId].hEmptyQSemaphore, X_SEMA_OPTION_WAIT, u4Timeout) == OSR_OK)
                {
                    u4FbIdx++;
                }
                else
                {
                    u4FbIdx = _arFbg[ucFbgId].ucFbNs - _arFbg[ucFbgId].ucFbNsBase;
                    LOG(1, "Wait unlock timeout! (%d, %d)\n",u4LockFbNs, u4EmptyFbNs);
                    //FBM_QueryStatus(ucFbgId);
                }
            }
        }
    }

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_ReleaseDispQ
 *  Release fb in DispQ to EmptyQ.
 *  @param  ucFbgId   FBG index.
 *  @return release fb number.
 */
//-------------------------------------------------------------------------
void FBM_ReleaseDispQ(UCHAR ucFbgId)
{
    UCHAR ucCnt;
    UCHAR ucFbId = FBM_FB_ID_UNKNOWN;
    UCHAR ucIdx;
	UCHAR ucVdpId = VDP_NS;
	UCHAR ucEsId = ES0;
    UCHAR ucThumbnail = 0;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    if (_arFbg[ucFbgId].fgCreateFromInst)
    {
        return;
    }

	FBM_MUTEX_LOCK(ucFbgId);
    ucThumbnail = FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE);

    if (!ucThumbnail)
    {
        FBM_MUTEX_UNLOCK(ucFbgId);
        ucEsId = _arFbg[ucFbgId].u1DecoderSrcId;
        ucVdpId = VDP_Es2Vdp(ucEsId);
        #if defined(CC_MT5890)
        VDP_VsyncMutexLock(ucVdpId);
        #else
        #endif
        ucFbId = VDP_GetPendingFB(ucVdpId, ucFbgId);
        VDP_SetReleaseDispQ(ucVdpId, ucFbgId);
        FBM_MUTEX_LOCK(ucFbgId);
    }

    if (_arFbg[ucFbgId].fgEnableNewSeamless)
    {
        _VDP_Prescale_Flush(_arFbg[ucFbgId].ucSeamlessRzId, FALSE);
        if( ((_arFbg[ucFbgId].rSeqHdr.eSeamlessMode & SEAMLESS_TWO_RESIZER) == SEAMLESS_TWO_RESIZER) 
          && (_arFbg[ucFbgId].ucSeamlessRzId2 != VDP_PRESCALE_RZ_ID_UNKNOWN))
        {
            _VDP_Prescale_Flush(_arFbg[ucFbgId].ucSeamlessRzId2,FALSE);
        }
        ucCnt = _arFbg[ucFbgId].rDisplayQ.ucCount;
    }
    else
    {
        ucCnt = FBM_CheckFrameBufferDispQ(ucFbgId);
    }

    for (ucIdx = 0; ucIdx < ucCnt; ucIdx++)
    {
        UCHAR ucDispFbId;

        if (_arFbg[ucFbgId].fgEnableNewSeamless)
        {
            if (_arFbg[ucFbgId].rDisplayQ.ucCount > 0)
            {
                ucDispFbId = _arFbg[ucFbgId].rDisplayQ.aucQueue[_arFbg[ucFbgId].rDisplayQ.ucReadIdx];
                
                _arFbg[ucFbgId].rDisplayQ.ucReadIdx = (UCHAR)(_arFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP;
                _arFbg[ucFbgId].rDisplayQ.ucCount--;
                
                ASSERT(_arFbg[ucFbgId].aucFbStatus[ucDispFbId] == FBM_FB_STATUS_DISPLAYQ);
                
                if ((_arFbg[ucFbgId].ucSeqChg) && (_arFbg[ucFbgId].ucNewFbId == ucDispFbId))
                {
                    _arFbg[ucFbgId].u4FbgFlag |= _arFbg[ucFbgId].u4SeqChgFlag;
                    _arFbg[ucFbgId].ucSeqChg = 0;
                }
            }
            else
            {
                ucDispFbId = FBM_FB_ID_UNKNOWN;
            }
        }
        else
        {
            ucDispFbId = FBM_GetFrameBufferFromDispQ(ucFbgId);
        }
        LOG(3, "Flush display fbid (%d ,%d)\n", ucDispFbId, _arFbg[ucFbgId].aucFbStatus[ucDispFbId]);
        FBM_SetFrameBufferStatus(ucFbgId, ucDispFbId, FBM_FB_STATUS_EMPTY);
    }

    if ((ucFbId != FBM_FB_ID_UNKNOWN) && (ucFbId < FBM_MAX_FB_NS_PER_GROUP))
    {
        if (_arFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK)
        {
            LOG(3, "Release Display Q with 1 more lock frame buffer!\n");

            if ((_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
                (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
                    (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
                    (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
                    (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG))

            {
                //VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
            }
            else
            {
                // release semaphore (Empty)
               // VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);

                if (IS_REFERENCE_FB(ucFbgId, ucFbId))
                {   // release reference frame buffer
                }
                else
                {   // release B frame buffer

                    // release semaphore (B & Empty)
                    VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyBQSemaphore) == OSR_OK);
                }
            }

            _FBM_PutFrameBufferToEmptyQ(ucFbgId, ucFbId);
            if (_arFbg[ucFbgId].afgRefList[ucFbId] ==1)
            {
                _arFbg[ucFbgId].afgRefList[ucFbId]=0;
            }

            _arFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_EMPTY;
#if 1
            if (_arFbg[ucFbgId].fgEnableNewSeamless)
            {
                if (_arFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN &&
                    _arFbg[ucFbgId].aucResizeFbStatus[_arFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid] != FBM_FB_STATUS_EMPTY)
                {
                    LOG(3, "Release resize fbid %d from pending fb %d!\n", _arFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid, ucFbId);
                    _FBM_PutFrameBufferToEmptyResizeQ(ucFbgId, _arFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid);
                    VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyResizeQSemaphore) == OSR_OK);
                }
                if (_arFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid != FBM_FB_ID_UNKNOWN &&
                    _arFbg[ucFbgId].aucResize2FbStatus[_arFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid] != FBM_FB_STATUS_EMPTY)
                {
                    LOG(3, "Release resize fbid %d from pending fb %d!\n", _arFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid, ucFbId);
                    _FBM_PutFrameBufferToEmptyResize2Q(ucFbgId, _arFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid);
                    VERIFY(_FBM_csema_unlock(&_arFbg[ucFbgId].hEmptyResize2QSemaphore) == OSR_OK);
                }                
            }
#endif
            if (!ucThumbnail)
            {
                VDP_CleanPendingFB(ucVdpId, ucFbgId);
            }
        }
    }
    FBM_MUTEX_UNLOCK(ucFbgId);
    if (!ucThumbnail)
    {
        #if defined(CC_MT5890)
        VDP_VsyncMutexUnlock(ucVdpId);
        #else
        #endif
    }
}


//-------------------------------------------------------------------------
/** FBM_ConfigColorMode
 *  Check FBG color mode
 *  @param  ucFbgType   FBG type.
 *  @param  ucFbgCm   color mode.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ConfigColorMode(UCHAR ucFbgId, UCHAR ucFbgCm, UINT32 u4HSize,
                         UINT32 u4VSize, FBM_CREATE_FBG_PAR_T *prPar)
{
    // Note about color
    // Support 420/422/444/420Digest

    UCHAR ucFbpListNs;

    UINT32 u4YSize;
    UINT32 u4CSize;
    UINT32 u4MvSize;
    UINT32 u4FbIdx;
    UINT32 au4CbFunc[FBM_CB_FUNC_NS] = {0};

    UINT32 u4FbNs;
    UINT32 u4ListIdx;
    FBM_POOL_T* prFbmPool = NULL;
    
    UINT32 u4PoolSize = 0;

    FBP_LIST_T* prFbpList;
    
    UNUSED(prFbmPool);
    UNUSED(u4PoolSize);
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    if (!prPar)
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    _arFbg[ucFbgId].rEmptyQ.ucCount = 0;
    _arFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;

    _FBM_GetPoolList(&prFbpList, &ucFbpListNs);

    _arFbg[ucFbgId].u1DecoderSrcId = prPar->u1VdecId;
    u4YSize = 0;
    u4CSize = 0;
    for (u4ListIdx = 0; u4ListIdx < ucFbpListNs; u4ListIdx++)
    {
        if (_arFbg[ucFbgId].ucFbgType == prFbpList[u4ListIdx].ucType)
        {
            u4YSize = prFbpList[u4ListIdx].u4YSize;
            u4CSize = prFbpList[u4ListIdx].u4CSize;

            _arFbg[ucFbgId].u4FbWidth = prFbpList[u4ListIdx].u4Width;
            _arFbg[ucFbgId].u4FbHeight = prFbpList[u4ListIdx].u4Height;

			LOG(1,"ucFbgType %d u4YSize 0x%x u4CSize 0x%x u4FbWidth 0x%x u4FbHeight 0x%x\n",
				 prFbpList[u4ListIdx].ucType,
				 u4YSize,
				 u4CSize,
				 _arFbg[ucFbgId].u4FbWidth,
				 _arFbg[ucFbgId].u4FbHeight);
			
            u4ListIdx = ucFbpListNs;
        }
    }
    if (prPar->fg10Bit)
    {
        u4YSize = (u4YSize * 5) >> 2;
        u4CSize = (u4CSize * 5) >> 2;
        _arFbg[ucFbgId].u4FbMemoryPoolSize = (_arFbg[ucFbgId].u4FbMemoryPoolSize * 5 ) >> 2;
                
        LOG(1,"Compact Mode 10 bit [u4YSize 0x%x u4CSize 0x%x Pool Size 0x%x]\n",u4YSize,u4CSize,_arFbg[ucFbgId].u4FbMemoryPoolSize);
    }	
    if (_arFbg[ucFbgId].eDynamicMode == FBM_DYNAMIC_SAME)
    {    //save the memset func ptr
        au4CbFunc[FBM_CB_FUNC_GFX_MEMSET] = _arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_MEMSET];
        au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET] = _arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET];
        au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE] = _arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE];
		au4CbFunc[FBM_CB_FUNC_GDMA_MEMSET] = _arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GDMA_MEMSET];
		au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET] = _arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET];
		au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE] = _arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE];
		
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_MEMSET, (UINT32)NULL);
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_TASK_MEMSET, (UINT32)NULL);
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_TASK_MEMSET_DONE, (UINT32)NULL);
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_MEMSET , (UINT32)NULL);
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_TASK_MEMSET, (UINT32)NULL);
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE , (UINT32)NULL);        
    }


    if (prPar && prPar->fgExternalAddr)
    {
        _arFbg[ucFbgId].u1FbgAppMode= prPar->u1AppMode;
        _arFbg[ucFbgId].ucFbgCm = ucFbgCm;
        //_arFbg[ucFbgId].ucFbNsBase = 0;
        
        _arFbg[ucFbgId].ucFbNs = prPar->u1RequestFbNs;// + _arFbg[ucFbgId].ucFbNsBase;
    }
    else if (ucFbgCm == FBM_CM_420 || ucFbgCm == FBM_CM_422 || ucFbgCm == FBM_CM_444)
    {
        UINT32 u4Addr = 0;
        UINT32 u4AddrC = 0;
        UINT32 u4WidthDiv = 1;
        UINT32 u4ExtraFbNs = 0;
        
        UINT32 u4FbStartPoolYAddr = 0;
        UINT32 u4FbEndPoolYAddr = 0;        
        UINT32 u4FbStartPoolCAddr = 0;
        UINT32 u4FbEndPoolCAddr = 0;
        
        _arFbg[ucFbgId].fgRPRMode = FALSE;
        _arFbg[ucFbgId].u1FbgAppMode= prPar->u1AppMode;
        _arFbg[ucFbgId].ucFbgCm = ucFbgCm;
#ifdef ENABLE_MULTIMEDIA
    //for not-standard timing such as 1440*900 to support much more reference.
    {
        VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(_arFbg[ucFbgId].u1DecoderSrcId);
        if(prVdecEsInfo && (prVdecEsInfo->eMMSrcType != SWDMX_SRC_TYPE_NETWORK_NETFLIX) && (_arFbg[ucFbgId].ucFbgType != FBM_FBG_TYPE_4K2K_JPEG)) 
        {            
            if ((FBM_FBG_PAL_H > u4HSize) && (FBM_FBG_PAL_V > u4VSize))
            {
                u4HSize = FBM_FBG_PAL_H;
                u4VSize = FBM_FBG_PAL_V;                
            }
            _arFbg[ucFbgId].u4FbWidth = FBM_ALIGN_MASK(u4HSize, FBM_B2R_H_PITCH);
            _arFbg[ucFbgId].u4FbHeight = FBM_ALIGN_MASK(u4VSize, FBM_B2R_H_PITCH);;
            u4YSize = FBM_ALIGN_MASK((_arFbg[ucFbgId].u4FbWidth * _arFbg[ucFbgId].u4FbHeight), FBM_FMG_Y_ALIGMENT);
            u4CSize = FBM_ALIGN_MASK((u4YSize >> 1), FBM_FMG_Y_ALIGMENT);
            if (prPar->fg10Bit)
            {
                u4YSize = (u4YSize * 5) >> 2;
                u4CSize = (u4CSize * 5) >> 2;
                        
                LOG(1,"Compact Mode 10 bit [u4YSize 0x%x u4CSize 0x%x Pool Size 0x%x]\n",u4YSize,u4CSize,_arFbg[ucFbgId].u4FbMemoryPoolSize);
            }
            LOG(1,"Conver real w/h by 64 align W/H(%d %d) YSize/CSize(0x%x 0x%x)\n",
                _arFbg[ucFbgId].u4FbWidth,_arFbg[ucFbgId].u4FbHeight,u4YSize,u4CSize);
        }
    }
#endif        
        
        if (ucFbgCm == FBM_CM_422)
        {
            u4CSize *= 2;    // 420 to 422
        }
        else if(ucFbgCm == FBM_CM_444)
        {
            u4CSize *= 4;    // 420 to 444
        }

        // Calculate Frame Buffer Number
        u4FbNs = FBM_CalcBufNum(ucFbgId, _arFbg[ucFbgId].ucFbgType, _arFbg[ucFbgId].u4VDecFmt, 
                _arFbg[ucFbgId].u4FbWidth, _arFbg[ucFbgId].u4FbHeight, 
                _arFbg[ucFbgId].u4FbMemoryPoolSize, u4YSize, u4CSize, 
                _arFbg[ucFbgId].u1FbgAppMode, &u4ExtraFbNs);
        
        u4FbNs -= u4ExtraFbNs; // Remove Extra Fb first, as the extra partion maybe not continuous.

        if(_fgSetDecoderFb && (_u1DecoderType == _arFbg[ucFbgId].u4VDecFmt))
        {
            u4FbNs = _u1NumOfFb;
            _fgSetDecoderFb = FALSE;
        }

        if (u4FbNs >= FBM_MAX_FB_NS_PER_GROUP/2)
        {
            u4FbNs = FBM_MAX_FB_NS_PER_GROUP/2 - 1;
        }
        
        _arFbg[ucFbgId].ucFbNs = (UCHAR)(u4FbNs) + _arFbg[ucFbgId].ucFbNsBase;

        
        u4Addr = _arFbg[ucFbgId].u4FbMemoryPool;
        u4AddrC = _arFbg[ucFbgId].u4FbMemoryPoolC;
        u4PoolSize = _arFbg[ucFbgId].u4FbMemoryPoolSize;

        u4FbStartPoolYAddr = _arFbg[ucFbgId].u4FbStartPoolYAddr;
        u4FbEndPoolYAddr   = _arFbg[ucFbgId].u4FbEndPoolYAddr;
        u4FbStartPoolCAddr = _arFbg[ucFbgId].u4FbStartPoolCAddr;
        u4FbEndPoolCAddr   = _arFbg[ucFbgId].u4FbEndPoolCAddr;
        
        if(_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG && ucFbgCm == FBM_CM_422 && prPar->u1AppMode != FBM_FBG_APP_MTIMAGE)
        {
            u4Addr = _arFbg[ucFbgId].u4FbMemoryPool;
#ifdef CC_FBM_3D_OSD
#if !defined(FBM_4K2K_JPG_SUPPORT) && !defined(FBM_4K2K_SUPPORT)
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPG_VDP);
            if (prFbmPool != NULL)
            {
                if (_arFbg[ucFbgId].u4FbMemoryPool < prFbmPool->u4Addr + prFbmPool->u4Size && 
                    prFbmPool->u4Addr + prFbmPool->u4Size < _arFbg[ucFbgId].u4FbMemoryPool + _arFbg[ucFbgId].u4FbMemoryPoolSize)
                {
                    LOG(0, "Free size after overlapped with JPG_VDP %d\n", _arFbg[ucFbgId].u4FbMemoryPool + _arFbg[ucFbgId].u4FbMemoryPoolSize - (prFbmPool->u4Addr + prFbmPool->u4Size));
                    //ASSERT(_arFbg[ucFbgId].u4FbMemoryPool + _arFbg[ucFbgId].u4FbMemoryPoolSize - (prFbmPool->u4Addr + prFbmPool->u4Size) >= 0x880000);
                    u4Addr += (prFbmPool->u4Addr + prFbmPool->u4Size - _arFbg[ucFbgId].u4FbMemoryPool);

                    //1024 alignment
                    u4Addr = ((u4Addr+1023) & ~1023);
                }
            }
#endif
#endif
        }
        else
        {
            // Pool Size
            if(_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_SBSHD  || _arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_TNBHD)
            {
                u4PoolSize = FBM_FBG_TYPE_1080HD_H264_SIZE;
	            if (prPar && prPar->fg10Bit)
	            {
	                u4PoolSize = (u4PoolSize * 5 ) >> 2;                
	                LOG(0,"enlarge Pool size(10Bit) 0x%x\n",u4PoolSize);
	            }
	            if (prPar && (prPar->fgUFO || prPar->fgThumbnailMode))
	            {
	                u4PoolSize = u4PoolSize + ((u4PoolSize * 3) >> 9);                
	                LOG(0,"enlarge Pool size(UFO) 0x%x\n",u4PoolSize);
	            }     				
            }
#ifdef FBM_4K2K_SUPPORT
            else if (_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_4K2K)
            {
                u4PoolSize = FBM_FBG_TYPE_4K2K_7FB_SIZE_PART1 ;
                
                if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP9 )
                {
                    u4PoolSize = FBM_FBG_TYPE_4K2K_SIZE_PART1 ;
                }                
	            if (prPar && prPar->fg10Bit)
	            {
	                u4PoolSize = (u4PoolSize * 5 ) >> 2;                
	                LOG(0,"enlarge Pool size(10Bit) 0x%x\n",u4PoolSize);
	            }
	            if (prPar && (prPar->fgUFO || prPar->fgThumbnailMode))
	            {
	                u4PoolSize = u4PoolSize + ((u4PoolSize * 3) >> 9);                
	                LOG(0,"enlarge Pool size(UFO) 0x%x\n",u4PoolSize);
	            }     				
            }
#endif
#ifdef FBM_4K2K_JPG_SUPPORT
            else if (_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_4K2K_JPEG)
            {
                u4PoolSize = FBM_FBG_TYPE_4K2K_JPEG_SIZE ;
	            if (prPar && prPar->fg10Bit)
	            {
	                u4PoolSize = (u4PoolSize * 5 ) >> 2;                
	                LOG(0,"enlarge Pool size(10Bit) 0x%x\n",u4PoolSize);
	            }
	            if (prPar && (prPar->fgUFO || prPar->fgThumbnailMode))
	            {
	                u4PoolSize = u4PoolSize + ((u4PoolSize * 3) >> 9);                
	                LOG(0,"enlarge Pool size(UFO) 0x%x\n",u4PoolSize);
	            }	
            }
#endif        
                               
        }

#if (defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)  || defined(CC_VOMX_TV_COEXIST)) && defined(CC_FBM_FULL_3D_SUPPORT) || defined (FBM_4K1K_VIDEO_SUPPORT)
        if(_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_SBSHD  || _arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_TNBHD)
        {
            for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase; ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
            {
                if (prPar && (prPar->fgUFO || prPar->fgThumbnailMode))
                {
                    if (u4PoolSize < u4YSize + u4CSize +((u4YSize*3) >> 9)) //u4YSize/256 +  u4YSize/512
                    {
                        u4Addr = _arFbg[ucFbgId].u4FbMemoryPool2;
                        u4AddrC = _arFbg[ucFbgId].u4FbMemoryPool2C;                                
                        u4PoolSize = FBM_FBG_TYPE_1080HD_H264_SIZE + ((FBM_FBG_TYPE_1080HD_H264_SIZE *3) >> 9); 
                        
                        u4FbStartPoolYAddr = _arFbg[ucFbgId].u4FbStartPool2YAddr;
                        u4FbEndPoolYAddr   = _arFbg[ucFbgId].u4FbEndPool2YAddr;
                        u4FbStartPoolCAddr = _arFbg[ucFbgId].u4FbStartPool2CAddr;
                        u4FbEndPoolCAddr   = _arFbg[ucFbgId].u4FbEndPool2CAddr;   
                        
                        if (prPar && prPar->fg10Bit)
                        {
                            u4PoolSize = (u4PoolSize * 5) >> 2;
                        }  
                        LOG(1,"ucFbgId %d u4FbIdx %d start allocate from MPEG2 pool\n",ucFbgId,u4FbIdx);                            
                    }
                }           
                else if (u4PoolSize < (u4YSize+u4CSize))
                {
                    u4Addr = _arFbg[ucFbgId].u4FbMemoryPool2;
                    u4AddrC = _arFbg[ucFbgId].u4FbMemoryPool2C;
                    u4PoolSize = FBM_FBG_TYPE_1080HD_H264_SIZE;
                    
                    u4FbStartPoolYAddr = _arFbg[ucFbgId].u4FbStartPool2YAddr;
                    u4FbEndPoolYAddr   = _arFbg[ucFbgId].u4FbEndPool2YAddr;
                    u4FbStartPoolCAddr = _arFbg[ucFbgId].u4FbStartPool2CAddr;
                    u4FbEndPoolCAddr   = _arFbg[ucFbgId].u4FbEndPool2CAddr; 
                    
                    if (prPar && prPar->fg10Bit)
                    {
                        u4PoolSize = (u4PoolSize * 5) >> 2;
                        LOG(1,"ucFbgId %d u4FbIdx %d start allocate from MPEG2 pool\n",ucFbgId,u4FbIdx);
                    }                     
                }
                
                if (prPar->u1AppMode == FBM_FBG_APP_OMX || prPar->u1AppMode == FBM_FBG_APP_OMX_DISP)
                {
                    u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_GFX_Y_ALIGMENT);
                    u4AddrC = FBM_ALIGN_MASK(u4AddrC, FBM_GFX_C_ALIGMENT);                
                }
                
                // Y
                _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr;
                u4Addr += u4YSize;
                
                _arFbg[ucFbgId].au4GfxMemsetId[u4FbIdx] = FBM_GFX_MEMSET_ASYNC(_arFbg[ucFbgId].au4AddrY[u4FbIdx], (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), _arFbg[ucFbgId].u4FbHeight, 0x0);

                FBM_Check_AddrIsOverLap(_arFbg[ucFbgId].au4AddrY[u4FbIdx],u4YSize,u4FbStartPoolYAddr,u4FbEndPoolYAddr)             
                
                _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4AddrC;
                 u4AddrC += u4CSize;
                 
                _arFbg[ucFbgId].au4GfxMemsetId[u4FbIdx] = FBM_GFX_MEMSET_ASYNC(_arFbg[ucFbgId].au4AddrC[u4FbIdx], (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), (_arFbg[ucFbgId].u4FbHeight >> 1), 0x80);

                FBM_Check_AddrIsOverLap(_arFbg[ucFbgId].au4AddrC[u4FbIdx],u4CSize,u4FbStartPoolCAddr,u4FbEndPoolCAddr)             
                
                //Y_LEN
                if (prPar && (prPar->fgUFO || prPar->fgThumbnailMode))
                {
                    _arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx] = u4Addr;
                    u4Addr += u4YSize/256;
                    
                    _arFbg[ucFbgId].au4GfxMemsetId[u4FbIdx] = FBM_GFX_MEMSET_ASYNC(_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx],(_arFbg[ucFbgId].u4FbWidth / u4WidthDiv),(_arFbg[ucFbgId].u4FbHeight>>8),0xFF);    

                    FBM_Check_AddrIsOverLap(_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx],(u4YSize/256),u4FbStartPoolYAddr,u4FbEndPoolYAddr)

                    _arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx] = u4AddrC;
                    u4AddrC += u4CSize/256;    
                    
                    _arFbg[ucFbgId].au4GfxMemsetId[u4FbIdx] = FBM_GFX_MEMSET_ASYNC(_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx],(_arFbg[ucFbgId].u4FbWidth / u4WidthDiv),(_arFbg[ucFbgId].u4FbHeight>>9),0xFF);

                    FBM_Check_AddrIsOverLap(_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx],(u4CSize/256),u4FbStartPoolCAddr,u4FbEndPoolCAddr)
                    
                    if (u4PoolSize >= u4YSize + u4CSize +((u4YSize*3) >> 9)) //u4YSize/256 +  u4YSize/512
                    {
                        u4PoolSize -= (u4YSize + u4CSize + ((u4YSize*3) >> 9));
                    }
                    else
                    {
                        u4PoolSize = 0;
                    }                      
                }
                else 
                {
                    if (u4PoolSize >= u4YSize + u4CSize)
                    {
                        u4PoolSize -= (u4YSize + u4CSize);
                    }
                    else
                    {
                        u4PoolSize = 0;
                    }                   
                }
             
            }
        }
        else
#endif
#ifdef FBM_4K2K_JPG_SUPPORT  //FBM_4K2K_SUPPORT
            if (_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_4K2K_JPEG)
            {
                for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase; ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
                {
                    if (u4PoolSize < (u4YSize + u4CSize))
                    {
                        u4Addr = _arFbg[ucFbgId].u4FbMemoryPool2;
                        u4AddrC = _arFbg[ucFbgId].u4FbMemoryPool2C;                        
                        u4PoolSize = FBM_FBG_TYPE_4K2K_JPEG_SIZE;
                        
                        u4FbStartPoolYAddr = _arFbg[ucFbgId].u4FbStartPool2YAddr;
                        u4FbEndPoolYAddr   = _arFbg[ucFbgId].u4FbEndPool2YAddr;
                        u4FbStartPoolCAddr = _arFbg[ucFbgId].u4FbStartPool2CAddr;
                        u4FbEndPoolCAddr   = _arFbg[ucFbgId].u4FbEndPool2CAddr;                         
                        
                    }
                    u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_GFX_Y_ALIGMENT);
                    // Y
                    _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr;
                    _arFbg[ucFbgId].au4GfxMemsetId[u4FbIdx] = FBM_GFX_MEMSET_ASYNC(u4Addr, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), _arFbg[ucFbgId].u4FbHeight, 0x0);

                    FBM_Check_AddrIsOverLap(u4Addr,u4YSize,u4FbStartPoolYAddr,u4FbEndPoolYAddr)   

                    u4Addr += u4YSize;

                    // C
                    u4AddrC = FBM_ALIGN_MASK(u4AddrC, FBM_GFX_C_ALIGMENT);
                    _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4AddrC;
                    _arFbg[ucFbgId].au4GfxMemsetId[u4FbIdx] = FBM_GFX_MEMSET_ASYNC(u4AddrC, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), (_arFbg[ucFbgId].u4FbHeight >> 1), 0x80);
                    FBM_Check_AddrIsOverLap(u4AddrC,u4CSize,u4FbStartPoolCAddr,u4FbEndPoolCAddr)
                    
                    u4AddrC += u4CSize;
                    
                    LOG(2, "FBG_TYPE_4K2K(%d): AddrY/C(0x%x/0x%x) SizeY/C(0x%x/0x%x) Pool/Pool2(%x/%x) Size(0x%x).\n",
                        u4FbIdx, _arFbg[ucFbgId].au4AddrY[u4FbIdx], _arFbg[ucFbgId].au4AddrC[u4FbIdx],
                        u4YSize, u4CSize, _arFbg[ucFbgId].u4FbMemoryPool, _arFbg[ucFbgId].u4FbMemoryPool2, u4PoolSize);

                    if (u4PoolSize >= u4YSize + u4CSize)
                    {
                        u4PoolSize -= (u4YSize + u4CSize);
                    }
                    else
                    {
                        u4PoolSize = 0;
                    }
                }
            }
            else
#endif
#ifdef FBM_4K2K_SUPPORT
            if (_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_4K2K)
            {
                LOG(0, "4K2K FB:AddrY/C(0x%x/0x%x) Y Size(0x%x) C Size(0x%x) FbNs(%d) FbHeight(%d) u4FbWidth(%d).\n", 
						u4Addr,u4AddrC, u4YSize, u4CSize, _arFbg[ucFbgId].ucFbNs,
						_arFbg[ucFbgId].u4FbHeight,_arFbg[ucFbgId].u4FbWidth);
				
                for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase; ((u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
                {
                    if (prPar && (prPar->fgUFO || prPar->fgThumbnailMode))
                    {
                        if (u4PoolSize < u4YSize + u4CSize +((u4YSize*3) >> 9)) //u4YSize/256 +  u4YSize/512
                        {
                            u4Addr = _arFbg[ucFbgId].u4FbMemoryPool2;
                            u4AddrC = _arFbg[ucFbgId].u4FbMemoryPool2C;
                            u4PoolSize = FBM_FBG_TYPE_4K2K_7FB_SIZE_PART2 + ((FBM_FBG_TYPE_4K2K_7FB_SIZE_PART2 * 3) >> 9);
                            if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP9 )
                            {
                                u4PoolSize = FBM_FBG_TYPE_4K2K_SIZE_PART2 + ((FBM_FBG_TYPE_4K2K_SIZE_PART2 * 3) >> 9);
                            }                               
                            u4FbStartPoolYAddr = _arFbg[ucFbgId].u4FbStartPool2YAddr;
                            u4FbEndPoolYAddr   = _arFbg[ucFbgId].u4FbEndPool2YAddr;
                            u4FbStartPoolCAddr = _arFbg[ucFbgId].u4FbStartPool2CAddr;
                            u4FbEndPoolCAddr   = _arFbg[ucFbgId].u4FbEndPool2CAddr;                                 
                            if (prPar && prPar->fg10Bit)
                            {
                                u4PoolSize = (u4PoolSize * 5) >> 2;
                            }  
                            
                            LOG(1,"ucFbgId %d u4FbIdx %d start allocate from MPEG2 pool\n",ucFbgId,u4FbIdx);                            
                        }
                    }
                    else if (u4PoolSize < (u4YSize + u4CSize))
                    {
                        u4Addr = _arFbg[ucFbgId].u4FbMemoryPool2;
                        u4AddrC = _arFbg[ucFbgId].u4FbMemoryPool2C;
                        u4PoolSize = FBM_FBG_TYPE_4K2K_7FB_SIZE_PART2;
                        
                        if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP9 )
                        {
                            u4PoolSize = FBM_FBG_TYPE_4K2K_SIZE_PART2;
                        }                         
                        u4FbStartPoolYAddr = _arFbg[ucFbgId].u4FbStartPool2YAddr;
                        u4FbEndPoolYAddr   = _arFbg[ucFbgId].u4FbEndPool2YAddr;
                        u4FbStartPoolCAddr = _arFbg[ucFbgId].u4FbStartPool2CAddr;
                        u4FbEndPoolCAddr   = _arFbg[ucFbgId].u4FbEndPool2CAddr;   
                        
                        if (prPar && prPar->fg10Bit)
                        {
                            u4PoolSize = (u4PoolSize * 5) >> 2;
                            LOG(1,"ucFbgId %d u4FbIdx %d start allocate from MPEG2 pool\n",ucFbgId,u4FbIdx);
                        }                        
                    }
                    u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_GFX_Y_ALIGMENT);

                    // Y
                    _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr;
                    
                    _arFbg[ucFbgId].au4GdmaMemsetYId[u4FbIdx] = FBM_GDMA_MEMSET_ASYNC((VOID*)u4Addr,0x0,(_arFbg[ucFbgId].u4FbHeight * _arFbg[ucFbgId].u4FbWidth));
                                        
                    FBM_Check_AddrIsOverLap(u4Addr,u4YSize,u4FbStartPoolYAddr,u4FbEndPoolYAddr)
                    
                    //GDMA_MemSet((VOID*)u4Addr,0x0,(_arFbg[ucFbgId].u4FbHeight * _arFbg[ucFbgId].u4FbWidth));

                    u4Addr += u4YSize;

                    // C
                    u4AddrC = FBM_ALIGN_MASK(u4AddrC, FBM_GFX_C_ALIGMENT);

                    _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4AddrC;
                    
          
                  _arFbg[ucFbgId].au4GdmaMemsetCId[u4FbIdx] = FBM_GDMA_MEMSET_ASYNC((VOID*)u4AddrC,0x80808080,(_arFbg[ucFbgId].u4FbHeight * _arFbg[ucFbgId].u4FbWidth / 2));

                   FBM_Check_AddrIsOverLap(u4AddrC,u4CSize,u4FbStartPoolCAddr,u4FbEndPoolCAddr)
                   
                   //GDMA_MemSet((VOID*)u4AddrC,0x80,(_arFbg[ucFbgId].u4FbHeight * _arFbg[ucFbgId].u4FbWidth / 2));          
           
                    u4AddrC += u4CSize;
					
					LOG(1,"ucFbgId(%d) au4AddrY[%d] 0x%x au4AddrC[%d] 0x%x\n",
						ucFbgId,
						u4FbIdx,
						_arFbg[ucFbgId].au4AddrY[u4FbIdx],
						u4FbIdx,
						_arFbg[ucFbgId].au4AddrC[u4FbIdx]);					

                    //Y_LEN & C_LEN
                    if (prPar && (prPar->fgUFO || prPar->fgThumbnailMode))
                    {
                    	u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_GFX_Y_ALIGMENT);

						_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx] = u4Addr;
						u4Addr += u4YSize/256;
						//  _arFbg[ucFbgId].au4GfxMemsetId[u4FbIdx] = FBM_GFX_MEMSET_ASYNC(_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx],(_arFbg[ucFbgId].u4FbWidth / u4WidthDiv),(_arFbg[ucFbgId].u4FbHeight>>8),0x0);             
						//GDMA_MemSet((VOID*)_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx],0xFF,(_arFbg[ucFbgId].u4FbHeight>>8));

						FBM_GDMA_MEMSET((VOID*)_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx],0xFFFFFFFF,(_arFbg[ucFbgId].u4FbHeight>>8));
						
                        FBM_Check_AddrIsOverLap(_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx],(u4YSize/256),u4FbStartPoolYAddr,u4FbEndPoolYAddr)

						u4AddrC = FBM_ALIGN_MASK(u4AddrC, FBM_GFX_C_ALIGMENT);

						_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx] = u4AddrC;
						u4AddrC += u4CSize/256;

						//GDMA_MemSet((VOID*)_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx],0xFF,(_arFbg[ucFbgId].u4FbHeight>>9));
						FBM_GDMA_MEMSET((VOID*)_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx],0xFFFFFFFF,(_arFbg[ucFbgId].u4FbHeight>>9));
                        FBM_Check_AddrIsOverLap(_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx],(u4CSize/256),u4FbStartPoolCAddr,u4FbEndPoolCAddr)
                        
						//  _arFbg[ucFbgId].au4GfxMemsetId[u4FbIdx] = FBM_GFX_MEMSET_ASYNC(_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx],(_arFbg[ucFbgId].u4FbWidth / u4WidthDiv),(_arFbg[ucFbgId].u4FbHeight>>9),0x80);                        

						LOG(1,"ucFbgId(%d) au4AddrY_Ext[%d] 0x%x au4AddrC_Ext[%d] 0x%x\n",
							ucFbgId,
							u4FbIdx,
							_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx],
							u4FbIdx,
							_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx]);									

                        if (u4PoolSize >= u4YSize + u4CSize +((u4YSize*3) >> 9)) //u4YSize/256 +  u4YSize/512
                        {
                            u4PoolSize -= (u4YSize + u4CSize + ((u4YSize*3) >> 9));
                        }
                        else
                        {
                            u4PoolSize = 0;
                        }                      
                    }
                    else
                    {
                        if (u4PoolSize >= u4YSize + u4CSize)
                        {
                            u4PoolSize -= (u4YSize + u4CSize);
                        }
                        else
                        {
                            u4PoolSize = 0;
                        }                        
                    }
                }
            }
            else
#endif
        {
            for (u4FbIdx= _arFbg[ucFbgId].ucFbNsBase;(u4FbIdx < _arFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP); u4FbIdx++)
            {
       
                u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_GFX_Y_ALIGMENT);

                // Y
                _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr;
                FBM_GFX_MEMSET(u4Addr, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), _arFbg[ucFbgId].u4FbHeight, 0x0);

                FBM_Check_AddrIsOverLap(u4Addr,u4YSize,u4FbStartPoolYAddr,u4FbEndPoolYAddr)

                u4Addr += u4YSize;

                u4AddrC= FBM_ALIGN_MASK(u4AddrC,FBM_GFX_C_ALIGMENT);
                
                _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4AddrC;
                FBM_GFX_MEMSET(u4AddrC, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), (_arFbg[ucFbgId].u4FbHeight >> 1), 0x80);
                FBM_Check_AddrIsOverLap(u4AddrC,u4CSize,u4FbStartPoolCAddr,u4FbEndPoolCAddr)

                u4AddrC+= u4CSize;
                
				LOG(1,"ucFbgId(%d) au4AddrY[%d] 0x%x au4AddrC[%d] 0x%x\n",
					ucFbgId,
					u4FbIdx,
					_arFbg[ucFbgId].au4AddrY[u4FbIdx],
					u4FbIdx,
					_arFbg[ucFbgId].au4AddrC[u4FbIdx]);
					
                if (prPar && (prPar->fgUFO || prPar->fgThumbnailMode))
                {
                	   u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_VDEC_WORKING_ALIGNMENT);
                	
                    _arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx] = u4Addr;
                    
                   // FBM_GFX_MEMSET(u4Addr, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), (_arFbg[ucFbgId].u4FbHeight>>8), 0xFF);
                    FBM_GDMA_MEMSET((VOID*)u4Addr,0xFFFFFFFF,((_arFbg[ucFbgId].u4FbHeight * _arFbg[ucFbgId].u4FbWidth/ u4WidthDiv )>>8));
                    FBM_Check_AddrIsOverLap(u4Addr,(u4YSize/256),u4FbStartPoolYAddr,u4FbEndPoolYAddr)
                    
                    u4Addr += u4YSize/256;
             
                    _arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx] = u4AddrC;
                    
                    //FBM_GFX_MEMSET(u4AddrC, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), (_arFbg[ucFbgId].u4FbHeight >> 9), 0xFF);                                        
					FBM_GDMA_MEMSET((VOID*)u4AddrC,0xFFFFFFFF,((_arFbg[ucFbgId].u4FbHeight * _arFbg[ucFbgId].u4FbWidth/u4WidthDiv )>>9));                    
                    FBM_Check_AddrIsOverLap(u4AddrC,(u4CSize/256),u4FbStartPoolCAddr,u4FbEndPoolCAddr)					
                    u4AddrC += u4CSize/256;
					
					LOG(1,"ucFbgId(%d) au4AddrY_Ext[%d] 0x%x au4AddrC_Ext[%d] 0x%x\n",
						ucFbgId,
						u4FbIdx,
						_arFbg[ucFbgId].au4AddrY_Ext[u4FbIdx],
						u4FbIdx,
						_arFbg[ucFbgId].au4AddrC_Ext[u4FbIdx]);											
                }
					
            }
        }

        //MV buffer config
        if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_MPEG2)
        {
   		    u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_B2R_H_PITCH); 
   		   
            _arFbg[ucFbgId].au4AddrMv[0] = u4Addr;

            if ((u4FbIdx >= FBM_DBK_FB_NS) && (_arFbg[ucFbgId].ucFbgType != FBM_FBG_TYPE_1080HD_RR))
            {
                if (u4FbIdx >= FBM_MAX_FB_NS_PER_GROUP)
                {
                    ASSERT(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP);
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return;
                }
                /* Y/C separation,in CH B & CH C respectively */ 
                _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr;
 
                _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4AddrC;
            }
        }
        else if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264)
        {
            if (_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_1080HD_RR)
            {
                u4MvSize = u4YSize >> 1;
            }
            else
            {
                u4MvSize = u4YSize >> 2;
            }

            if (_arFbg[ucFbgId].ucMvBufNs > FBM_MAX_FB_NS_PER_GROUP-1)
            {
                ASSERT(_arFbg[ucFbgId].ucMvBufNs <= FBM_MAX_FB_NS_PER_GROUP-1);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return;
            }
            if((prPar->ucCftFbgId != FBM_FBG_ID_UNKNOWN) && (prPar->ucCftFbgId < FBG_MAX_NS))
            {
                if(_arFbg[ucFbgId].au4AddrMv[0] > u4Addr)
                {
                    u4Addr = _arFbg[ucFbgId].au4AddrMv[0];
                }
            }
            for (u4FbIdx = 0;
                    ((u4FbIdx < _arFbg[ucFbgId].ucMvBufNs)&&(u4FbIdx < (FBM_MAX_FB_NS_PER_GROUP-1))); u4FbIdx++)
            {
            
			   u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_B2R_H_PITCH); // H264 mv start addr need 64 align
            
                _arFbg[ucFbgId].au4AddrMv[u4FbIdx] = u4Addr;
                
                FBM_Check_AddrIsOverLap(u4Addr,u4MvSize,u4FbStartPoolYAddr,u4FbEndPoolYAddr)
                
                u4Addr += u4MvSize;
                LOG(1,"H264:AddrMv[%d] 0x%x\n",u4FbIdx,_arFbg[ucFbgId].au4AddrMv[u4FbIdx]);
            }
            for (u4FbIdx = 0;u4FbIdx < FBM_MAX_CABAC_BUF_NS_PER_GROUP;u4FbIdx++)
            {
                u4Addr = FBM_ALIGN_MASK(u4Addr,FBM_FMG_Y_ALIGMENT);
                
                _arFbg[ucFbgId].au4AddrCabac[u4FbIdx] = u4Addr; //MV End Storge CABAC buffer
                FBM_Check_AddrIsOverLap(u4Addr,FBM_FBG_TYPE_CABAC_SIZE,u4FbStartPoolYAddr,u4FbEndPoolYAddr)
                
                u4Addr += FBM_FBG_TYPE_CABAC_SIZE;
                
                LOG(1,"H264: Addr Cabc[%d] 0x%x\n",u4FbIdx,_arFbg[ucFbgId].au4AddrCabac[u4FbIdx]);          
            }             
        }
        else if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265)
        {
            u4MvSize = u4YSize >> 4;            
            if (_arFbg[ucFbgId].ucMvBufNs>FBM_MAX_FB_NS_PER_GROUP-1)
            {
                ASSERT(_arFbg[ucFbgId].ucMvBufNs<=FBM_MAX_FB_NS_PER_GROUP-1);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return;
            }
            for (u4FbIdx = 0;
                    ((u4FbIdx < _arFbg[ucFbgId].ucMvBufNs)&&(u4FbIdx < (FBM_MAX_FB_NS_PER_GROUP-1))); u4FbIdx++)
            {
                u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_VDEC_WORKING_ALIGNMENT);
                _arFbg[ucFbgId].au4AddrMv[u4FbIdx] = u4Addr; //YAddr End Storge MV buffer 
                
                FBM_Check_AddrIsOverLap(u4Addr,u4MvSize,u4FbStartPoolYAddr,u4FbEndPoolYAddr)
                
                u4Addr += u4MvSize;
              
                LOG(1,"H265:AddrMv[%d] 0x%x\n",u4FbIdx,_arFbg[ucFbgId].au4AddrMv[u4FbIdx]);
            }
            for (u4FbIdx = 0;u4FbIdx < FBM_MAX_CABAC_BUF_NS_PER_GROUP;u4FbIdx++)
            {
              u4Addr = FBM_ALIGN_MASK(u4Addr,FBM_FMG_Y_ALIGMENT);
              
                _arFbg[ucFbgId].au4AddrCabac[u4FbIdx] = u4Addr; //MV End Storge CABAC buffer
                
                FBM_Check_AddrIsOverLap(u4Addr,FBM_FBG_TYPE_CABAC_SIZE,u4FbStartPoolYAddr,u4FbEndPoolYAddr)
                
                u4Addr += FBM_FBG_TYPE_CABAC_SIZE;
                
              LOG(1,"H265: Addr Cabc[%d] 0x%x\n",u4FbIdx,_arFbg[ucFbgId].au4AddrCabac[u4FbIdx]);          
            }
        }
        else if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RV)
        {
	        LOG(1,"RM Before working Buffer 64 Align u4Addr 0x%x\n",u4Addr);        
   		    u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_B2R_H_PITCH); 
   		    
   		    LOG(1,"RM After working Buffer 64 Align u4Addr 0x%x\n",u4Addr);
        
            if (_arFbg[ucFbgId].fgRPRMode)
            {
                _arFbg[ucFbgId].u4ExtraYBuffer = u4Addr;
                //u4Addr += u4YSize;
                u4Addr += FBM_FBG_TYPE_PAL_Y_SIZE;
                _arFbg[ucFbgId].u4ExtraCBuffer = u4Addr;
                //u4Addr += u4CSize;
                u4Addr += FBM_FBG_TYPE_PAL_C_SIZE;

                _arFbg[ucFbgId].u4RPRSeamlessBuffer = u4Addr;
                //for another two frame
                u4Addr += ((FBM_FBG_TYPE_PAL_Y_SIZE+FBM_FBG_TYPE_PAL_C_SIZE)<<1);
                //u4Addr += (u4YSize<<1);
                //u4Addr += (u4CSize<<1);
            }
            _arFbg[ucFbgId].au4AddrMv[0] = u4Addr;
        }
        else if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_MPEG4)
        {
   		    u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_B2R_H_PITCH); 
        
#if (defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)  || defined(CC_VOMX_TV_COEXIST)) && defined(CC_FBM_FULL_3D_SUPPORT) || defined(FBM_4K1K_VIDEO_SUPPORT)
            if(_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_SBSHD  || _arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_TNBHD)
            {
                if (u4PoolSize < FBM_WORK_BUF_SIZE)
                {
                    u4Addr = _arFbg[ucFbgId].u4FbMemoryPool2;
                    u4PoolSize = FBM_FBG_TYPE_1080HD_H264_SIZE;
                }
            }
            _arFbg[ucFbgId].au4AddrMv[0] = u4Addr;
#else
            {
                _arFbg[ucFbgId].au4AddrMv[0] = u4Addr;
            }
#endif
        }
        else if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP9)
        {
            if (_arFbg[ucFbgId].ucMvBufNs>FBM_MAX_FB_NS_PER_GROUP-1)
            {
                ASSERT(_arFbg[ucFbgId].ucMvBufNs<=FBM_MAX_FB_NS_PER_GROUP-1);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return;
            }
            for (u4FbIdx = 0;
                    ((u4FbIdx < _arFbg[ucFbgId].ucMvBufNs)&&(u4FbIdx < (FBM_MAX_FB_NS_PER_GROUP-1))); u4FbIdx++)
            {
                u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_VDEC_WORKING_ALIGNMENT);
                _arFbg[ucFbgId].au4AddrMv[u4FbIdx] = u4Addr; //YAddr End Storge MV buffer 
                
                if(_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_4K2K)
                {               
                    FBM_Check_AddrIsOverLap(u4Addr,FBM_WORK_BUF_VP9_4K2K_SIZE,u4FbStartPoolYAddr,u4FbEndPoolYAddr)
                    u4Addr += FBM_WORK_BUF_VP9_4K2K_SIZE;
                        
                }
                else
                {
                    FBM_Check_AddrIsOverLap(u4Addr,FBM_WORK_BUF_VP9_FHD_SIZE,u4FbStartPoolYAddr,u4FbEndPoolYAddr)
                    u4Addr += FBM_WORK_BUF_VP9_FHD_SIZE;                        
                }
                
                LOG(1,"VP9:AddrMv[%d] 0x%x\n",u4FbIdx,_arFbg[ucFbgId].au4AddrMv[u4FbIdx]);
            }       
                    
            for (u4FbIdx = 0;u4FbIdx < FBM_MAX_CABAC_BUF_NS_PER_GROUP;u4FbIdx++)
            {
                u4Addr = FBM_ALIGN_MASK(u4Addr,FBM_FMG_Y_ALIGMENT);

                _arFbg[ucFbgId].au4AddrCabac[u4FbIdx] = u4Addr; //MV End Storge CABAC buffer

                FBM_Check_AddrIsOverLap(u4Addr,(FBM_FBG_TYPE_CABAC_SIZE*2),u4FbStartPoolYAddr,u4FbEndPoolYAddr)

                u4Addr += (FBM_FBG_TYPE_CABAC_SIZE*2);

                LOG(1,"VP9: Addr Cabc[%d] 0x%x\n",u4FbIdx,_arFbg[ucFbgId].au4AddrCabac[u4FbIdx]);          
            }
            
            // segld buf size + tife buf size + count tbl size + prob tbl buf size
            u4Addr = FBM_ALIGN_MASK(u4Addr,FBM_FMG_Y_ALIGMENT);
            _arFbg[ucFbgId].au4MiscTblStartAddr = u4Addr;
            
            
        }
        else if (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)
        {
   		    u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_B2R_H_PITCH);         
            _arFbg[ucFbgId].au4AddrMv[0] = u4Addr;
            
            LOG(1,"MJPEG Mv Addr 0x%x\n",u4Addr);
            
        }
        else    //VC-1 and RM
        {
            ASSERT(_arFbg[ucFbgId].u4VDecFmt != FBM_VDEC_UNKNOWN);
            u4Addr = FBM_ALIGN_MASK(u4Addr, FBM_B2R_H_PITCH);     
            _arFbg[ucFbgId].au4AddrMv[0] = u4Addr;
        }
        if (!_arFbg[ucFbgId].fgRPRMode)
        {
            _arFbg[ucFbgId].u4ExtraYBuffer =0;
            _arFbg[ucFbgId].u4ExtraCBuffer =0;
        }

#ifdef CC_FBM_SPLIT_MPEG_FREE_PART
        _arFbg[ucFbgId].u4WorkBufSize = FBM_WORK_BUF_SIZE;
#else

#if (defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)  || defined(CC_VOMX_TV_COEXIST)) && defined(CC_FBM_FULL_3D_SUPPORT) ||defined(FBM_4K1K_VIDEO_SUPPORT)
        if(_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_SBSHD  || _arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_TNBHD)
        {
            _arFbg[ucFbgId].u4WorkBufSize = u4PoolSize;
        }
#ifdef FBM_4K2K_SUPPORT
        else if (_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_4K2K)
        {
            _arFbg[ucFbgId].u4WorkBufSize = FBM_FBG_TYPE_4K2K_7FB_SIZE_PART2 - (3 * ( u4YSize + u4CSize));
        }
#endif
        else
        {
            _arFbg[ucFbgId].u4WorkBufSize = _arFbg[ucFbgId].u4FbMemoryPoolSize - ((_arFbg[ucFbgId].ucFbNs - _arFbg[ucFbgId].ucFbNsBase) * ( u4YSize + u4CSize));
        }
#else
        _arFbg[ucFbgId].u4WorkBufSize = _arFbg[ucFbgId].u4FbMemoryPoolSize - ((_arFbg[ucFbgId].ucFbNs - _arFbg[ucFbgId].ucFbNsBase) * ( u4YSize + u4CSize));
#endif
        LOG(1, "u4WorkBufSize(0x%x)=PoolSize(0x%x)-(Y(%d)+C(%d))*Ns(%d - %d).\n", _arFbg[ucFbgId].u4WorkBufSize, 
                _arFbg[ucFbgId].u4FbMemoryPoolSize, u4YSize, u4CSize, _arFbg[ucFbgId].ucFbNs, _arFbg[ucFbgId].ucFbNsBase);
#endif

    //Handle extra reference frame, update FbNs & Fb partition here.
    #if 0
     /* temp solution ,marked it ,as multi-way playback confilict with this case . */
#if defined(CC_H264_LV5_SUPPORT_SHARE_POOL) || defined(CC_H264_LV5_SUPPORT_SINGLE_POOL)

        if (u4ExtraFbNs && _arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264 && _arFbg[ucFbgId].fgEnableH264V5Support)
        {
            UINT32 u4ExtraReferenceNs;
            UINT32 u4MVSize;

#if defined(CC_H264_LV5_SUPPORT_SHARE_POOL)

            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG2);
            u4Addr = FBM_ALIGN_MASK(prFbmPool->u4Addr , FBM_FMG_Y_ALIGMENT);
            u4PoolSize = prFbmPool->u4Size;

            ASSERT(FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL > FBM_FBG_H264_FHD_REFERNCE_NS);
            u4ExtraReferenceNs = FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL - FBM_FBG_H264_FHD_REFERNCE_NS;
            u4MVSize = ((u4ExtraReferenceNs * u4YSize) >> 2);

            u4ExtraReferenceNs = ((u4PoolSize - u4MVSize)/(u4YSize + u4CSize));
            ASSERT(u4ExtraReferenceNs >= FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL - FBM_FBG_H264_FHD_REFERNCE_NS);

            if (u4ExtraReferenceNs > FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL - FBM_FBG_H264_FHD_REFERNCE_NS)
            {
                u4ExtraReferenceNs = FBM_FBG_H264_FHD_REFERNCE_NS_TWO_POOL - FBM_FBG_H264_FHD_REFERNCE_NS;
            }

#elif defined(CC_H264_LV5_SUPPORT_SINGLE_POOL)
           
            ASSERT(FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL > FBM_FBG_H264_FHD_REFERNCE_NS);
            u4ExtraReferenceNs = FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL - FBM_FBG_H264_FHD_REFERNCE_NS;
            u4MVSize = ((u4ExtraReferenceNs * u4YSize) >> 2);

            u4ExtraReferenceNs = ((FMB_EXTRA_FOR_SINGLE_H264_LV5 - u4MVSize)/(u4YSize + u4CSize));
            ASSERT(u4ExtraReferenceNs >= FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL - FBM_FBG_H264_FHD_REFERNCE_NS);

            if (u4ExtraReferenceNs > FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL - FBM_FBG_H264_FHD_REFERNCE_NS)
            {
                u4ExtraReferenceNs = FBM_FBG_H264_FHD_REFERNCE_NS_ONE_POOL - FBM_FBG_H264_FHD_REFERNCE_NS;
            }
            
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG);
            u4Addr = FBM_ALIGN_MASK(prFbmPool->u4Addr + prFbmPool->u4Size - FMB_EXTRA_FOR_SINGLE_H264_LV5, FBM_FMG_Y_ALIGMENT);
            u4PoolSize = FMB_EXTRA_FOR_SINGLE_H264_LV5;

#endif
            u4ExtraReferenceNs = FBM_MIN(u4ExtraReferenceNs, u4ExtraFbNs);

            for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase; ((u4FbIdx < _arFbg[ucFbgId].ucFbNs + u4ExtraReferenceNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
            {
                // Y
                _arFbg[ucFbgId].au4AddrY[u4FbIdx] = u4Addr;
                FBM_GFX_MEMSET((UINT8*)u4Addr, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), _arFbg[ucFbgId].u4FbHeight, 0x0);
                u4Addr += u4YSize;

                _arFbg[ucFbgId].au4AddrC[u4FbIdx] = u4Addr;
                FBM_GFX_MEMSET((UINT8*)u4Addr, (_arFbg[ucFbgId].u4FbWidth / u4WidthDiv), (_arFbg[ucFbgId].u4FbHeight >> 1), 0x80);
                u4Addr += u4CSize;
            }

            _arFbg[ucFbgId].ucFbNs += u4ExtraReferenceNs;

            if (_arFbg[ucFbgId].ucMvBufNs + u4ExtraReferenceNs > FBM_MAX_FB_NS_PER_GROUP-1)
            {
                ASSERT(_arFbg[ucFbgId].ucMvBufNs + u4ExtraReferenceNs <= FBM_MAX_FB_NS_PER_GROUP-1);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return;
            }

            if (_arFbg[ucFbgId].ucFbgType == FBM_FBG_TYPE_1080HD_RR)
            {
                u4MvSize = u4YSize >> 1;
            }
            else
            {
                u4MvSize = u4YSize >> 2;
            }
            
            for (u4FbIdx = _arFbg[ucFbgId].ucMvBufNs; ((u4FbIdx < _arFbg[ucFbgId].ucMvBufNs + u4ExtraReferenceNs)&&(u4FbIdx < (FBM_MAX_FB_NS_PER_GROUP-1))); u4FbIdx++)
            {
                _arFbg[ucFbgId].au4AddrMv[u4FbIdx] = u4Addr;
                u4Addr += u4MvSize;
            }

            _arFbg[ucFbgId].ucMvBufNs += u4ExtraReferenceNs;

            // TODO: Need update _arFbg[ucFbgId].u4WorkBufSize for H.264?             
            //_arFbg[ucFbgId].u4WorkBufSize = ?
            
            if (u4ExtraFbNs >= u4ExtraReferenceNs)
            {
                u4ExtraFbNs -= u4ExtraReferenceNs;
            }
            
            ASSERT(u4ExtraFbNs == 0);
            
        }                
#endif
#endif

    }
    
    if (_arFbg[ucFbgId].eDynamicMode == FBM_DYNAMIC_SAME)
    {    //recover the memset func ptr
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_MEMSET, (UINT32)au4CbFunc[FBM_CB_FUNC_GFX_MEMSET]);
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_TASK_MEMSET, (UINT32)au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET]);
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_TASK_MEMSET_DONE, (UINT32)au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE]);
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_MEMSET, (UINT32)au4CbFunc[FBM_CB_FUNC_GDMA_MEMSET]);
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_TASK_MEMSET, (UINT32)au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET]);
        FBM_RegCbFunc(FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE, (UINT32)au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE]);
        
    }
	#if 0
    //fbm memory error check
    {
        FBM_POOL_T* prFbmPool;
#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST) || defined(CC_FBM_SUPPORT_4K2K)
        FBM_POOL_T* prFbmPool2;
        prFbmPool2 = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG2);
#endif
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG);

#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST)  || defined(CC_FBM_SUPPORT_4K2K)
        if(((_arFbg[ucFbgId].au4AddrMv[0] + _arFbg[ucFbgId].ucMvBufNs * u4MvSize) >= (prFbmPool2->u4Addr + prFbmPool2->u4Size)))
#else
        if((_arFbg[ucFbgId].au4AddrMv[0] + _arFbg[ucFbgId].ucMvBufNs * u4MvSize) > (prFbmPool->u4Addr + prFbmPool->u4Size))
#endif
        {
            LOG(1,"warning ! frame buffer allocation error\n");
            //ASSERT(0);
        }
    }
	#endif
    {
        UINT32 u4FbIdx;

        if ((_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
                (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
                (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
                (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
                (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)||
                (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8)||
                (_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP6))

        {
            // create semaphore, Empty Q
            VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)0) == OSR_OK);
        }
        else
        {
            // create semaphore, Empty Q
            VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)0) == OSR_OK);

            // create semaphore, Empty BQ
            VERIFY(_FBM_csema_reset(&_arFbg[ucFbgId].hEmptyBQSemaphore, _arFbg[ucFbgId].hMutex, (INT32)(_arFbg[ucFbgId].ucFbNs - FBM_REF_FB_NS_PER_GROUP)) == OSR_OK);

#ifdef CC_VDP_FULL_ISR
            _arFbg[ucFbgId].hEmptyQSemaphore.prState = &_rState;
            _arFbg[ucFbgId].hEmptyBQSemaphore.prState = &_rState;
#endif
        }

        // Nivia and William Co-design
        _arFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
        _arFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;
        

        if((prPar->ucCftFbgId != FBM_FBG_ID_UNKNOWN) && (prPar->ucCftFbgId < FBG_MAX_NS))
        {
            LOG(1,"[%s %d] eDynamicMode %d \n",__FUNCTION__,__LINE__,_arFbg[ucFbgId].eDynamicMode);

            if (_FBM_2FBsChkDynamicDone(ucFbgId))            
            {
                //Do nothing            
            }
            
            if (_arFbg[ucFbgId].eDynamicMode == FBM_DYNAMIC_SAME)
            {
                _FBM_2FBsConflictList(ucFbgId);
            }
        }
        for (u4FbIdx = _arFbg[ucFbgId].ucFbNsBase; u4FbIdx < _arFbg[ucFbgId].ucFbNs; u4FbIdx++)
        {
            //LOG(2, "FB[%d-%d] Try put into emptyQ.\n", ucFbgId, u4FbIdx);
            _FBM_PutFrameBufferToEmptyQ(ucFbgId, (UCHAR)(u4FbIdx));
        }
    }

    ASSERT(_arFbg[ucFbgId].hEmptyQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);
    ASSERT(_arFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _arFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}

//-------------------------------------------------------------------------
/** FBM_SetFrameBufferLayout
 *  Fill frame buffer with constant color
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  u4Color   color.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferLayout(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Addr, BOOL fgIsY)
{
    
    if (u4Addr == 0)
    {
        return;
    }

    if (VERIFY_FBG(ucFbgId) || VERIFY_FB(ucFbgId, ucFbId))
    {
        return;
    }

    if (fgIsY)
    {
        _arFbg[ucFbgId].au4AddrY[ucFbId] = u4Addr;
    }
    else
    {
        _arFbg[ucFbgId].au4AddrC[ucFbId] = u4Addr;
    }

}

//-------------------------------------------------------------------------
/** FBM_SetFrameBufferColor
 *  Fill frame buffer with constant color
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  u4Color   color.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferColor(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Color)
{
    UCHAR* pucYAddr;
    UCHAR* pucCAddr;
    UINT32 u4YSize;
    UINT32 u4CSize;
    UINT32 u4Idx;
    UCHAR ucY;
    UCHAR ucCb;
    UCHAR ucCr;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    switch (_arFbg[ucFbgId].ucFbgType)
    {
    case FBM_FBG_TYPE_1080HD:
        u4YSize = FBM_FBG_TYPE_1080HD_Y_SIZE;
        u4CSize = FBM_FBG_TYPE_1080HD_C_SIZE;
        break;

    case FBM_FBG_TYPE_720HD:
        u4YSize = FBM_FBG_TYPE_720HD_Y_SIZE;
        u4CSize = FBM_FBG_TYPE_720HD_C_SIZE;
        break;

    case FBM_FBG_TYPE_PAL:
        u4YSize = FBM_FBG_TYPE_PAL_Y_SIZE;
        u4CSize = FBM_FBG_TYPE_PAL_C_SIZE;
        break;

    case FBM_FBG_TYPE_NTSC:
        u4YSize = FBM_FBG_TYPE_NTSC_Y_SIZE;
        u4CSize = FBM_FBG_TYPE_NTSC_C_SIZE;
        break;

    default:
        u4YSize = 0;
        u4CSize = 0;
        ASSERT(0);
        break;
    }

    if (_arFbg[ucFbgId].ucFbgCm == FBM_CM_422)
    {
        u4CSize *= 2;
    }
    else if (_arFbg[ucFbgId].ucFbgCm == FBM_CM_444)
    {
        u4CSize *= 4;
    }
    else
    {
    }

    ucY = (UCHAR)((u4Color >> 16) & 0xFF);
    ucCb = (UCHAR)((u4Color >> 8) & 0xFF);
    ucCr = (UCHAR)((u4Color) & 0xFF);

    pucYAddr = (UCHAR*) _arFbg[ucFbgId].au4AddrY[ucFbId];
    pucCAddr = (UCHAR*)_arFbg[ucFbgId].au4AddrC[ucFbId];

    // use Gfx Engine in the future
    FBM_MEMSET(pucYAddr, ucY, u4YSize);

    for (u4Idx = 0; u4Idx < u4CSize; u4Idx += 2)
    {
        pucCAddr[u4Idx + 0] = ucCb;
        pucCAddr[u4Idx + 1] = ucCr;
    }
}


//-------------------------------------------------------------------------
/** FBM_SetFrameBufferZebraColor
 *  Fill frame buffer with constant color
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferZebraColor(UCHAR ucFbgId, UCHAR ucFbId)
{
    UCHAR* pucYAddr;
    UCHAR* pucCAddr;
    UCHAR ucYColor;
    UINT32 u4YSize;
    UINT32 u4CSize;
    UINT32 u4Idx;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    switch (_arFbg[ucFbgId].ucFbgType)
    {
    case FBM_FBG_TYPE_1080HD:
        u4YSize = FBM_FBG_TYPE_1080HD_Y_SIZE;
        u4CSize = FBM_FBG_TYPE_1080HD_C_SIZE;
        break;

    case FBM_FBG_TYPE_720HD:
        u4YSize = FBM_FBG_TYPE_720HD_Y_SIZE;
        u4CSize = FBM_FBG_TYPE_720HD_C_SIZE;
        break;

    case FBM_FBG_TYPE_PAL:
        u4YSize = FBM_FBG_TYPE_PAL_Y_SIZE;
        u4CSize = FBM_FBG_TYPE_PAL_C_SIZE;
        break;

    case FBM_FBG_TYPE_NTSC:
        u4YSize = FBM_FBG_TYPE_NTSC_Y_SIZE;
        u4CSize = FBM_FBG_TYPE_NTSC_C_SIZE;
        break;

    default:
        u4YSize = 0;
        u4CSize = 0;
        ASSERT(0);
        break;
    }

    if (_arFbg[ucFbgId].ucFbgCm == FBM_CM_422)
    {
        u4CSize *= 2;
    }
    else if (_arFbg[ucFbgId].ucFbgCm == FBM_CM_444)
    {
        u4CSize *= 4;
    }
    else
    {
    }

    pucYAddr = (UCHAR*) _arFbg[ucFbgId].au4AddrY[ucFbId];
    pucCAddr = (UCHAR*)_arFbg[ucFbgId].au4AddrC[ucFbId];

    ucYColor = 0;
    for (u4Idx = 0; u4Idx < u4YSize; u4Idx++)
    {
        pucYAddr[u4Idx] = ucYColor;

        if ((u4Idx & 0x3F) == 0x3F)
        {
            ucYColor = ~ucYColor;
        }
    }

    // use Gfx Engine in the future
    FBM_MEMSET(pucCAddr, 0x80, u4CSize);
}


//-------------------------------------------------------------------------
/** FBM_SetFrameBufferPlayBackNs
 *  Set frame buffer playback number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  ucPlayBackNs   playback number.
 *  @param  ucChangeFieldNs   change field number.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs, UCHAR ucChangeFieldNs)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    _arFbg[ucFbgId].aucPlayBackNs[ucFbId] = ucPlayBackNs;
    _arFbg[ucFbgId].aucChangeFieldNs[ucFbId] = ucChangeFieldNs;
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferPlayBackNs
 *  Get frame buffer playback number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return playback number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFrameBufferPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    return _arFbg[ucFbgId].aucPlayBackNs[ucFbId];
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferChangeFieldNs
 *  Get frame buffer change field number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return change field number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFrameBufferChangeFieldNs(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    return _arFbg[ucFbgId].aucChangeFieldNs[ucFbId];
}


//-------------------------------------------------------------------------
/** FBM_ChkFrameBufferLastPlayBack
 *  Check frame buffer last playback
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return 1/0 for last playback or NOT.
 */
//-------------------------------------------------------------------------
UCHAR FBM_ChkFrameBufferLastPlayBack(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 1;
    }

    if (_arFbg[ucFbgId].aucPlayBackNs[ucFbId] > 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


//-------------------------------------------------------------------------
/** FBM_QueryStatus
 *  Query FBM status
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_QueryStatus(UCHAR ucFbgId)
{
    UCHAR ucIdx;

    if ((ucFbgId < FBG_MAX_NS) &&
        (_arFbg[ucFbgId].ucFbgType != FBM_FBG_TYPE_UNKNOWN) &&
        (_arFbg[ucFbgId].ucFbgId != FBM_FBG_ID_UNKNOWN))
    {
        _FbgStatus(_arFbg[ucFbgId].ucFbgId);
        return;
    }

    for (ucIdx = 0; ucIdx < FBG_MAX_NS; ucIdx++)
    {
        if ((_arFbg[ucIdx].ucFbgType != FBM_FBG_TYPE_UNKNOWN) &&
                (_arFbg[ucIdx].ucFbgId != FBM_FBG_ID_UNKNOWN))
        {
            _FbgStatus(_arFbg[ucIdx].ucFbgId);
        }
    }

    _FBM_PoolStatus();
}

//-------------------------------------------------------------------------
/** FBM_RegCbFunc
 */
//-------------------------------------------------------------------------
void FBM_RegCbFunc(FBM_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr)
{
    if (eFuncType < FBM_CB_FUNC_NS)
    {
        if (u4FuncPtr == 0)
        {
            _arFbmCbFunc.au4CbFunc[eFuncType] = u4FuncPtr;
            _arFbmCbFunc.au4CbFuncCRC[eFuncType] = u4FuncPtr;
        }
        else
        {
            _arFbmCbFunc.au4CbFunc[eFuncType] = u4FuncPtr;
            _arFbmCbFunc.au4CbFuncCRC[eFuncType] = ~u4FuncPtr;
        }
    }
    else
    {
        ASSERT(0);
    }
}

//-------------------------------------------------------------------------
/** FBM_RegFbgCbFunc
 */
//-------------------------------------------------------------------------
void FBM_RegFbgCbFunc(UCHAR ucFbgId, FBM_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr, UINT32 u4Tag)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    if (eFuncType < FBM_CB_FUNC_NS)
    {
        if (u4FuncPtr == 0)
        {
            _arFbmCbFunc.aau4CbFunc[ucFbgId][eFuncType] = u4FuncPtr;
            _arFbmCbFunc.aau4CbFuncCRC[ucFbgId][eFuncType] = u4FuncPtr;
            _arFbmCbFunc.aau4CbFuncTag[ucFbgId][eFuncType] = u4Tag;
        }
        else
        {
            _arFbmCbFunc.aau4CbFunc[ucFbgId][eFuncType] = u4FuncPtr;
            _arFbmCbFunc.aau4CbFuncCRC[ucFbgId][eFuncType] = ~u4FuncPtr;
            _arFbmCbFunc.aau4CbFuncTag[ucFbgId][eFuncType] = u4Tag;
        }
    }
    else
    {
        ASSERT(0);
    }
}

//-------------------------------------------------------------------------
/** FBM_GetCbFuncTbl
 */
//-------------------------------------------------------------------------
FBM_CB_FUNC_T* FBM_GetCbFuncTbl(void)
{
    return (&_arFbmCbFunc);
}

//-------------------------------------------------------------------------
/** FBM_FbgChgNotify
 *  FBM notify (MPEG to VDP)
 *  @param  ucFbgId   FBG index.
 *  @param  ucEsId   ESindex.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_FbgChgNotify(UCHAR ucFbgId, UCHAR ucEsId)
{
    UCHAR ucPlayMode = FBM_FBG_MODE_NS;
    UCHAR ucPreFbgId = FBM_FBG_ID_UNKNOWN;
    
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;
    
    // Note about usage
    // This function is called back by mpeg decode to indicate that
    // Frame buffer group ID is change.
    // It will by pass this notify to video plane driver

    if(ucFbgId != FBM_FBG_ID_UNKNOWN && (ucFbgId<FBG_MAX_NS))
    {
        _arFbg[ucFbgId].u1DecoderSrcId = ucEsId;
        if(_arFbg[ucFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE)
        {
            return ;
        }
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    
    if (prVdecEsInfoKeep && prVdecEsInfoKeep->fgFrameToUser)
    {
        LOG(0, "czg fgFrameToUser is true\n");
        if(ucFbgId != FBM_FBG_ID_UNKNOWN && (ucFbgId<FBG_MAX_NS))
        {
            FBM_SetFrameBufferGlobalFlag(ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);
            _arFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_DISP_READY;
        }
        return ;
    }
    if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE))
    {
        return;
    }

    ucPreFbgId = FBM_GetFbgByEs(ucEsId);
    
    FBM_GetPlayMode(ucFbgId,&ucPlayMode);
    if (ucPlayMode == FBM_FBG_NOTDISPLAY_MODE)
    {
        LOG(5, "FBM_FBG_NOTDISPLAY_MODE\n");
        _VdpCheckFbgReady(ucFbgId, ucEsId);
        return;
    }

    if (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_FBG_CHG_IND], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_FBG_CHG_IND]))
    {
#if defined(CC_53XX_SWDMX_V2) && defined(CC_MT5365)
        VDP_CONF_T* prVdpConf = VDP_GetConf(VDP_1); // get which decoder is connect to vdp1
#endif

        ((FBM_FBG_CHG_IND_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_FBG_CHG_IND])(ucFbgId, ucEsId);

#if defined(CC_53XX_SWDMX_V2) && defined(CC_MT5365)
        if(prVdpConf != NULL)
        {
            if(ucFbgId == FBM_FBG_ID_UNKNOWN && prVdpConf->ucInputPort[0] == ucEsId)
            {
                UINT8 u1Idx;
                UINT8 u1TargetEsId = MAX_ES_NS;
                UINT8 u1FbgId = FBM_FBG_ID_UNKNOWN;
                UCHAR ucEs2FbgId = FBM_FBG_ID_UNKNOWN;

                for(u1Idx=0 ; u1Idx < MAX_ES_NS ; u1Idx++)
                {
                    ucEs2FbgId = FBM_GetFbgByEs(u1Idx);
                    if((ucEs2FbgId != FBM_FBG_ID_UNKNOWN) && (u1Idx != ucEsId))
                    {
                        u1FbgId = ucEs2FbgId;
                        u1TargetEsId = u1Idx;
                    }
                }

                if(u1FbgId != FBM_FBG_ID_UNKNOWN)
                {
                    ((FBM_FBG_CHG_IND_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_FBG_CHG_IND])(FBM_FBG_ID_UNKNOWN, u1TargetEsId);
                    VDP_SetInput(VDP_1,u1TargetEsId,0);
                    ((FBM_FBG_CHG_IND_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_FBG_CHG_IND])(u1FbgId, u1TargetEsId);
                }
            }
        }
#endif
    }
    else
    {
        ASSERT(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_FBG_CHG_IND] == 0);
    }

    if  ((ucFbgId != FBM_FBG_ID_UNKNOWN && (ucFbgId<FBG_MAX_NS)) && (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.aau4CbFunc[ucFbgId][FBM_CB_FUNC_FBG_CHG_EX_IND],
                                 _arFbmCbFunc.aau4CbFuncCRC[ucFbgId][FBM_CB_FUNC_FBG_CHG_EX_IND])))
    {
        ((FBM_FB_READY_IND_FUNC_EX)_arFbmCbFunc.aau4CbFunc[ucFbgId][FBM_CB_FUNC_FBG_CHG_EX_IND])(
            _arFbmCbFunc.aau4CbFuncTag[ucFbgId][FBM_CB_FUNC_FBG_CHG_EX_IND],
            0);
    }
    if (ucFbgId == FBM_FBG_ID_UNKNOWN  && ucPreFbgId != FBM_FBG_ID_UNKNOWN)
    {
        if (_arFbg[ucPreFbgId].fgEnableNewSeamless)
        {
            _VDP_Prescale_Flush(_arFbg[ucPreFbgId].ucSeamlessRzId, TRUE);
            if(((_arFbg[ucPreFbgId].rSeqHdr.eSeamlessMode & SEAMLESS_TWO_RESIZER) == SEAMLESS_TWO_RESIZER) 
              && (_arFbg[ucPreFbgId].ucSeamlessRzId2 != VDP_PRESCALE_RZ_ID_UNKNOWN))
            {
                _VDP_Prescale_Flush(_arFbg[ucPreFbgId].ucSeamlessRzId2,TRUE);
            }
        }
    }

    if (ucFbgId < FBG_MAX_NS)
    {
        VDEC_FbgChgHdlr(ucEsId,ucFbgId, _arFbg[ucFbgId].u4FbMemoryPool,
                        (_arFbg[ucFbgId].u4FbMemoryPool + _arFbg[ucFbgId].u4FbMemoryPoolSize));
    }
}

//-------------------------------------------------------------------------
/** FBM_PicStructChgNotify
 *  FBM notify (MPEG to VDP)
 *  @param  ucEsId   ESindex.
 *  @param  ucPicStruct   Frame/Field.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_PicStructChgNotify(UCHAR ucEsId, UCHAR ucPicStruct)
{
    // Note about usage
    // This function is called back by mpeg decode to indicate that
    // Picture structure is change. (frame/field)
    // It will by pass this notify to video plane driver

    if (FBM_CHECK_CB_FUNC_VERIFY(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_PICSTRUCT_CHG_IND], _arFbmCbFunc.au4CbFuncCRC[FBM_CB_FUNC_PICSTRUCT_CHG_IND]))
    {
        ((FBM_PICSTRUCT_CHG_IND_FUNC)_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_PICSTRUCT_CHG_IND])(ucEsId, ucPicStruct);
    }
    else
    {
        ASSERT(_arFbmCbFunc.au4CbFunc[FBM_CB_FUNC_PICSTRUCT_CHG_IND] == 0);
    }
}

//-------------------------------------------------------------------------
/** FBM_SetSyncStc
 *  Set AV Sync Info
 *  @param  ucFbgId   FBG index
 *  @param  ucMode   AV Sync Mode
 *  @param  ucStcId   AV Sync Source
  *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetSyncStc(UCHAR ucFbgId, UCHAR ucMode, UCHAR ucStcId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    ASSERT(ucMode < VID_SYNC_MODE_NS);
    ASSERT(ucStcId < STC_SRC_NS);


    _arFbg[ucFbgId].ucStcSrc = ucStcId;
#if defined(CC_B2R_EMULATION)
    _arFbg[ucFbgId].ucAvSyncMode = VID_SYNC_MODE_NONE;
#else
    _arFbg[ucFbgId].ucAvSyncMode = ucMode;
#endif
}


//-------------------------------------------------------------------------
/** FBM_GetSyncStc
 *  GetV Sync Info
 *  @param  ucFbgId   FBG index
 *  @param  pucMode   AV Sync Mode pointer
 *  @param  pucStcId   AV Sync Source pointer
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetSyncStc(UCHAR ucFbgId, UCHAR* pucMode, UCHAR* pucStcId)
{
    if ((pucMode == NULL) || (pucStcId == NULL))
    {
        return;
    }

    if (VERIFY_FBG(ucFbgId))
    {
        *pucMode = VID_SYNC_MODE_NS;
        *pucStcId = STC_SRC_NS;
        return;
    }

    *pucMode = _arFbg[ucFbgId].ucAvSyncMode;
    *pucStcId = _arFbg[ucFbgId].ucStcSrc;
}

//-------------------------------------------------------------------------
/** FBM_SetAVSyncStatus
 *  Set AV Sync status (Repeat or Drop or ...)
 *  @param  ucFbgId   FBG index
 *  @param  u4AVSyncStatus   A/V sync mode
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetAVSyncStatus(UCHAR ucFbgId, UINT32 u4AVSyncStatus)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    _arFbg[ucFbgId].u4AvSyncStatus = u4AVSyncStatus;
}

//-------------------------------------------------------------------------
/** FBM_GetAVSyncStatus
 *  Get AV Sync status (Repeat or Drop or ...)
 *  @param  ucFbgId   FBG index
 *  @param  pu4AVSyncStatus   A/V sync mode
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetAVSyncStatus(UCHAR ucFbgId, UINT32* pu4AVSyncStatus)
{
    if (pu4AVSyncStatus == NULL)
    {
        return;
    }
    
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    *pu4AVSyncStatus = _arFbg[ucFbgId].u4AvSyncStatus;
}

//-------------------------------------------------------------------------
/** FBM_GetEmptyQCount
 *  Get Empty Queue Count
 *  @param  ucFbgId   FBG index
 *  @param  pucCount   Pointer of return results
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetEmptyQCount(UCHAR ucFbgId, UCHAR* pucCount)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    *pucCount = _arFbg[ucFbgId].rEmptyQ.ucCount;
}

UINT8 FBM_GetDecoderSrcId(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return 0xFF;
    }

    return _arFbg[ucFbgId].u1DecoderSrcId;
}

UINT8 FBM_GetFbgAppMode(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return 0xFF;
    }

    return _arFbg[ucFbgId].u1FbgAppMode;
}

//-------------------------------------------------------------------------
/** FBM_SetPlayMode
 *  Set Play Mode (DTV or MM or ...)
 *  @param  ucFbgId   FBG index
 *  @param  ucPlayMode   Play Mode
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetPlayMode(UCHAR ucFbgId, UCHAR ucPlayMode)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    _arFbg[ucFbgId].ucPlayMode = ucPlayMode;
}

void FBM_SetDecoderFB(UINT8 u1DecoderType,UINT8 u1NumOfFB)
{
    _fgSetDecoderFb = TRUE;
    _u1DecoderType = u1DecoderType;
    _u1NumOfFb = u1NumOfFB;
}

void FBM_SetColorMode(UINT8 u1ColorMode)
{
    if(u1ColorMode == FBM_CM_420 || u1ColorMode ==FBM_CM_422 )
    {
        LOG(1, "Color mode %d is selected.\n", u1ColorMode);
        _u1FbgColorMode = u1ColorMode;
    }
}

//-------------------------------------------------------------------------
/** FBM_GetPlayMode
 *  Get Play Mode (DTV or MM or ...)
 *  @param  ucFbgId   FBG index
 *  @param  pucPlayMode   Play Mode pointer
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetPlayMode(UCHAR ucFbgId, UCHAR* pucPlayMode)
{
    if ((pucPlayMode == NULL))
    {
        return;
    }

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    *pucPlayMode = _arFbg[ucFbgId].ucPlayMode;
}

//-------------------------------------------------------------------------
/** FBM_CheckPadding
 *  Check if pool memory corruption
 *  @param NONE
 *  @return NONE.
 */
//-------------------------------------------------------------------------

void FBM_CheckPadding(void)
{
#ifndef __KERNEL__
    UINT32 u4PoolIdx;
    UINT32 u4Idx;
    UINT32 u4PadAddr;
    UINT32 u4PadLen;
    UINT32 u4Addr;
    UINT32 u4Valid;

    u4Valid = 1;
    for (u4PoolIdx = 1 ; u4PoolIdx < (UINT32)FBM_POOL_TYPE_NS; u4PoolIdx++)
    {
        if ((_prPool[u4PoolIdx].u4Inherit == 0) &&
                (_prPool[u4PoolIdx].u4Addr > DRVCUST_OptGet(eFbmMemAddr)))
        {
            u4PadAddr = _prPool[u4PoolIdx].u4Addr - FBM_POOL_PADDING_SIZE;
            u4PadLen = FBM_POOL_PADDING_SIZE;

            for (u4Idx = 0; u4Idx < u4PadLen; u4Idx++)
            {
                u4Addr = u4PadAddr + u4Idx;

                if (*(UCHAR*)u4Addr != FBM_POOL_PADDING)
                {
                    LOG(1, "Pool(%s) memory corruption! addr=0x%x, val=0x%x\n", FBM_GetPoolName((UCHAR)u4PoolIdx), u4Addr, *(UCHAR*)u4Addr);

                    u4Valid = 0;

                    u4Idx = u4PadLen;
                }
            }
        }
    }

    if (u4Valid)
    {
        LOG(1, "No memory corruption\n");
    }
#else
    LOG(1, "NOT Support!\n");
#endif
}


//-------------------------------------------------------------------------
/** FBM_ResetFbChangeField
 *  Wait Frame buffer change field
 *  @param  ucFbgType   FBG type.
 *  @return lock status.
 */
//-------------------------------------------------------------------------
INT32 FBM_ResetFbChangeField(UCHAR ucFbgId) {

    if (VERIFY_FBG(ucFbgId))
    {
        return OSR_INV_ARG;
    }

    if (x_sema_lock(_arFbg[ucFbgId].hChgFieldSema, X_SEMA_OPTION_NOWAIT) == OSR_OK)
    {
    }
    else
    {
    }

    return OSR_OK;
}


//-------------------------------------------------------------------------
/** FBM_WaitFbChangeField
 *  Wait Frame buffer change field
 *  @param  ucFbgType   FBG type.
 *  @param  u4Timeout   time to wait.
 *  @return lock status.
 */
//-------------------------------------------------------------------------
INT32 FBM_WaitFbChangeField(UCHAR ucFbgId, UINT32 u4Timeout)
{
    INT32 i4Ret;

    if (VERIFY_FBG(ucFbgId))
    {
        return OSR_INV_ARG;
    }

    i4Ret = x_sema_lock_timeout(_arFbg[ucFbgId].hChgFieldSema, u4Timeout);

    return i4Ret;
}


//-------------------------------------------------------------------------
/** FBM_FbChangeField
 *  Frame buffer change field
 *  @param  ucFbgType   FBG type.
 *  @return unlock status.
 */
//-------------------------------------------------------------------------
void FBM_FbChangeField(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    if ((ucFbgId == FBM_FBG_ID_UNKNOWN) ||
            (_arFbg[ucFbgId].hChgFieldSema == 0))
    {
        return;
    }

    if (_arFbg[ucFbgId].hChgFieldSema)
    {
        VERIFY(x_sema_unlock(_arFbg[ucFbgId].hChgFieldSema) == OSR_OK);
    }
}


//-------------------------------------------------------------------------
/** FBM_SetFbNextFieldPlayBackNs
 *  Set frame buffer next field playback number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  ucPlayBackNs   playback number.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucPlayBackNs)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    _arFbg[ucFbgId].aucChgFldPlayBackNs[ucFbId] = ucPlayBackNs;

}


//-------------------------------------------------------------------------
/** FBM_GetFbNextFieldPlayBackNs
 *  Get frame buffer next field playback number
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return playback number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFbNextFieldPlayBackNs(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    return _arFbg[ucFbgId].aucChgFldPlayBackNs[ucFbId];
}

UINT32 FBM_GetVDecMemSize(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }
    return _arFbg[ucFbgId].u4FbMemoryPoolSize;
}
UINT32 FBM_GetCabacMemAddr(UCHAR ucFbgId,UCHAR ucCabNs)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }
    
    if (ucCabNs >= FBM_MAX_CABAC_BUF_NS_PER_GROUP )
    {
        return 0;
    }
    
    return _arFbg[ucFbgId].au4AddrCabac[ucCabNs];
}

UINT32  FBM_GetMiscMemAddr(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }
	
	return _arFbg[ucFbgId].au4MiscTblStartAddr;
	
	// default size FBM_WORK_BUF_VP9_MISC_TBL_SIZE
	
}

UINT32 FBM_GetRunningVdecMemSize(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }
    if(_arFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264)
    {
        return _arFbg[ucFbgId].u4FbMemoryPoolSize;
    }
    else
    {
        return _arFbg[ucFbgId].ucFbNs * ( (_arFbg[ucFbgId].u4FbWidth * _arFbg[ucFbgId].u4FbHeight *3) >>1) + FBM_WORK_BUF_SIZE;
    }
}

BOOL FBM_ChkFbgCreateFromInst(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return FALSE;
    }

#ifdef CC_HWCOMP_UT
    return TRUE;
#else
    return _arFbg[ucFbgId].fgCreateFromInst;
#endif
}
#ifdef FBM_CS_LOG
void FBM_FlushFbCsLog(void)
{
    _FbmCsLog(24, 0, _u4EQHandle);

}
#endif

#if defined(CC_MT5890) && defined(CC_FBM_FOUR_INST_SUPPORT)
//For Video Texture mode(backgroud capture) dst addr
UINT32 FBM_GetCaptureDstAddr(void)
{
    FBM_POOL_T* prFbmPool = NULL;
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG4_C);

    if (prFbmPool->u4Addr && (prFbmPool->u4Size >= FBM_FBG_1080HD_H * FBM_FBG_1080HD_V * 4 * 2))
    {
        if (prFbmPool->u4Size < FBM_FBG_1080HD_H * FBM_FBG_1080HD_V * 4 * 2) //*4 ARGB8888 *2 TWO instance
        {
            LOG(1, "MPEG4_C size not enough");
        }
        return prFbmPool->u4Addr;  //ARGB8888
    }

    return (UINT32)NULL;
}
#else
UINT32 FBM_GetCaptureDstAddr(void)
{
    LOG(1, "Not implement while NOT defined(CC_MT5890) && defined(CC_FBM_FOUR_INST_SUPPORT).\n");
    return (UINT32)NULL;
}

#endif

#if 1
void FBM_SetCodecInfo(UCHAR ucFbgId, ENUM_CODEC_INFO_T eCodecInfo)
{
    _arFbg[ucFbgId].eCodecType = eCodecInfo;
}

ENUM_CODEC_INFO_T FBM_GetCodecInfo(UCHAR ucFbgId)
{
    ENUM_CODEC_INFO_T eCodecInfo;
    if (VERIFY_FBG(ucFbgId))
    {
       return E_VDEC_CODEC_UNKNOWN;
    }
    eCodecInfo=_arFbg[ucFbgId].eCodecType;
    return  eCodecInfo;
}
BOOL FBM_GetCodecResolution(UCHAR ucFbgId,FBM_CODEC_RESOLUTION_T* rResolution)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return FALSE;
    }
    rResolution->u4Width = _arFbg[ucFbgId].rSeqHdr.u2HSize;
    rResolution->u4Height = _arFbg[ucFbgId].rSeqHdr.u2VSize;
    rResolution->u4OrgWidth = _arFbg[ucFbgId].rSeqHdr.u2OrgWidth;
    rResolution->u4OrgHeight = _arFbg[ucFbgId].rSeqHdr.u2OrgHeight;
    return TRUE;
}

#endif

UCHAR FBM_GetFbgByEs(UCHAR ucEsId)
{
    UCHAR ucFbgId = FBM_FBG_ID_UNKNOWN;
    UCHAR ucFbgIdx;

    do
    {
        if(ucEsId >= MAX_ES_NS)
        {
            break;
        }

        for(ucFbgIdx = 0; ucFbgIdx < FBG_MAX_NS; ucFbgIdx ++)
        {
            if(VERIFY_FBG(ucFbgIdx))
            {
                continue;
            }
            
            if(_arFbg[ucFbgIdx].u1DecoderSrcId == ucEsId)
            {
                ucFbgId = ucFbgIdx;
                break;
            }
        }
    }while(0);

    return ucFbgId;
}

#ifdef CC_B2R_RES_SUPPORT
B2R_HW_RES_T* _FBM_B2rResGetObj(UINT8 u1HwId)
{
    UINT32 i;
    B2R_HW_RES_T *ptRet = NULL;

    for (i = 0; i < B2R_HW_MAX_ID; i++)
    {
        if (_arB2rHwRes[i].t_cfg.u1HwId == u1HwId)
        {
            ptRet = &_arB2rHwRes[i];
            break;
        }
    }

    return ptRet;
}

void _FBM_B2rResPrmInit(B2R_HW_RES_T* pt_this)
{
    do
    {
        if(!pt_this)
        {
            break;
        }

        pt_this->t_prm.b_used  = FALSE;
        pt_this->t_prm.u1FbgId = FBG_MAX_NS;
        pt_this->t_prm.u1VdpId = VDP_MAX;
    }while(0);
}

void _FBM_B2rResInit(void)
{
    UINT32 i;
    B2R_HW_RES_T* pt_this = NULL;
    
    for(i = 0; i < B2R_HW_MAX_ID; i ++)
    {
        pt_this = &_arB2rHwRes[i];
        if(pt_this)
        {
            pt_this->t_cfg.u1HwId   = i;
            pt_this->t_cfg.u1GrpId  = i / 2;
            pt_this->t_cfg.u1AsscId = B2R_HW_MAX_ID - (i + 1);
            pt_this->t_cfg.u4MaxRes = FHD_TIMING;
            
            _FBM_B2rResPrmInit(pt_this);
        }
    }

    _arB2rHwRes[2].t_cfg.u4MaxRes = UHD_TIMING;
}

BOOL _FBM_B2rResInUsing(UCHAR ucFbgId,
                                UINT8* pu1B2rId)
{
    BOOL b_used = FALSE;
    UINT32 i;
    B2R_HW_RES_T* pt_this = NULL;
    
    do
    {
        if(ucFbgId >= FBG_MAX_NS || !pu1B2rId)
        {
            break;
        }

        if(VERIFY_FBG(ucFbgId))
        {
            break;
        }
        

        for(i = 0; i < B2R_HW_MAX_ID; i ++)
        {
            pt_this = _FBM_B2rResGetObj(i);
            if(pt_this &&
                pt_this->t_prm.u1FbgId == ucFbgId &&
                pt_this->t_prm.b_used)
            {
                *pu1B2rId = pt_this->t_cfg.u1HwId;
                b_used = TRUE;
                break;
            }
        }

        LOG(1,"FbgId(%d) B2rId(%d) use status(%d)!\n", ucFbgId,*pu1B2rId, b_used);
    }while(0);

    return b_used;
}

UINT8 FBM_B2rResGetPeerId(UINT8 u1HwId)
{
    UINT8 u1GrpId;
    UINT8 u1RetId = B2R_HW_MAX_ID;
    UINT32 i;
    B2R_HW_RES_T* pt_this = NULL;
    
    do
    {
        if(u1HwId >= B2R_HW_MAX_ID)
        {
            break;
        }

        pt_this = _FBM_B2rResGetObj(u1HwId);
        if(!pt_this)
        {
            break;
        }
        
        u1GrpId = pt_this->t_cfg.u1GrpId;
        for(i = 0; i < B2R_HW_MAX_ID; i++)
        {
            pt_this = _FBM_B2rResGetObj(i);
            if(pt_this &&
                pt_this->t_cfg.u1GrpId == u1GrpId &&
                pt_this->t_cfg.u1HwId != u1HwId)
            {
                u1RetId = i;
                break;
            }
        }
    }while(0);

    return u1RetId;
}

UINT8 FBM_B2rResGrpSelect(B2R_IPT_INFO_T* pt_src)
{
    UINT8 u1GrpId = GRP_MAX;
    
    do
    {
        if(!pt_src)
        {
            break;
        }

        if(pt_src->t_vd.eCodec == VDEC_JPEG)
        {
            u1GrpId = GRP_1;
            break;
        }
        else if(pt_src->t_vd.fgUfoEn ||
            pt_src->t_vd.eCodec == VDEC_H265 ||
            pt_src->t_vd.u1PixDepth == 10 ||
            pt_src->t_vd.u4Resolution > FHD_TIMING)
        {
            u1GrpId = GRP_2;
            break;
        }

        u1GrpId = GRP_1;
    }while(0);

    return u1GrpId;
}

BOOL FBM_B2rResHwIsReady(B2R_IPT_INFO_T* pt_src,
                                    UINT8 u1HwId)
{
    BOOL b_chosen = FALSE;
    UINT8 u1AsscId;
    UINT8 u1PeerId;
    B2R_HW_RES_T* pt_this = NULL;
    B2R_HW_RES_T* pt_assc = NULL;
    B2R_HW_RES_T* pt_peer = NULL;

    do
    {
        if(u1HwId >= B2R_HW_MAX_ID || !pt_src)
        {
            LOG(0,"Invalid hwid(%d) or pt_src == NULL\n",u1HwId);
            break;
        }

        pt_this = _FBM_B2rResGetObj(u1HwId);
        if(!pt_this)
        {
            LOG(0,"Can not get object with hwid(%d)\n",u1HwId);
            break;
        }
        
        if(pt_src->t_vd.u4Resolution > pt_this->t_cfg.u4MaxRes)
        {
            LOG(0,"Capbility exceed: now(%d) > cap(%d)\n",pt_src->t_vd.u4Resolution,pt_this->t_cfg.u4MaxRes);
            break;
        }

        if(pt_this->t_prm.b_used)
        {
            LOG(1,"The hwid(%d) is used!!!\n",u1HwId);
            break;
        }

        u1AsscId = pt_this->t_cfg.u1AsscId;
        pt_assc = _FBM_B2rResGetObj(u1AsscId);
        if(pt_assc &&
            pt_assc->t_prm.b_used)
        {
            LOG(1,"Conflict hwid(%d) with now(%d) is used!!!\n",u1AsscId,u1HwId);
            break;
        }

        u1PeerId = FBM_B2rResGetPeerId(u1HwId);
        pt_peer = _FBM_B2rResGetObj(u1PeerId);
        if(pt_peer &&
            pt_peer->t_prm.b_used &&
            (pt_peer->t_src.eCodec == pt_src->t_vd.eCodec &&
            (pt_peer->t_src.u1PixDepth == 10 ||
            pt_src->t_vd.u1PixDepth == 10)))
        {
            LOG(1,"The same group(%d %d) has conflict!!!\n",u1PeerId,u1HwId);
            break;
        }

        b_chosen = TRUE;
    }while(0);

    return b_chosen;
}

UINT8 FBM_B2rResAlloc(B2R_IPT_INFO_T* pt_src)
{
    UINT8 u1HwId;
    UINT8 u1B2rHwId = B2R_HW_MAX_ID;
    UINT8 u1GrpId;
    UINT8 i;
    
    do
    {
        if(!pt_src)
        {
            LOG(0,"The pt_src == NULL\n");
            break;
        }

        u1GrpId = FBM_B2rResGrpSelect(pt_src);

        LOG(1,"B2r Res GroupId(%d) with Vdpid(%d)!\n", u1GrpId,pt_src->u1VdpId);
        
        switch(pt_src->u1VdpId)
        {
            case VDP_MAIN:
            {
                for(i = 0; i < 2 ; i++)
                {
                    u1HwId = u1GrpId*2 + i;
                    if(FBM_B2rResHwIsReady(pt_src, u1HwId))
                    {
                        u1B2rHwId = u1HwId;
                        break;
                    }
                }
                break;
            }

            case VDP_SUB:
            {
                for(i = 1; i >= 0 ; i--)
                {
                    u1HwId = u1GrpId*2 + i;
                    if(FBM_B2rResHwIsReady(pt_src, u1HwId))
                    {
                        u1B2rHwId = u1HwId;
                        break;
                    }
                }
                break;
            }

            default:
                LOG(0,"Invalid VDPID(%d)\n",pt_src->u1VdpId);
                break;
        }
    }while(0);

    return u1B2rHwId;
}

void FBM_B2rResHdrVdpId(UCHAR ucFbgId,
                                  B2R_IPT_INFO_T* pt_src)
{
    UINT32 i = 0;
    UCHAR ucMVdpId = VDP_MAX;
    UCHAR ucSVdpId = VDP_MAX;
    BOOL  b_MainUsed = FALSE;
    BOOL  b_SubUsed  = FALSE;
    B2R_HW_RES_T* pt_this = NULL;
    INT32         i4Ret     = B2R_OK;
    
    do
    {
        if(ucFbgId >= FBG_MAX_NS || !pt_src)
        {
            LOG(1,"The fbgid(%d) is invalid or pt_src = NULL!\n",ucFbgId);
            break;
        }

        i4Ret = B2R_MainSubStatus(&ucMVdpId, &ucSVdpId);
        if(i4Ret != B2R_OK)
        {
            LOG(1,"Call B2R_MainSubStatus returns(%d)!\n",i4Ret);
            return;
        }
        
        if(ucMVdpId != VDP_MAX)
        {
            for(i = 0; i < B2R_HW_MAX_ID; i ++)
            {
                pt_this = _FBM_B2rResGetObj(i);
                if(pt_this->t_prm.u1VdpId == ucMVdpId)
                {
                    b_MainUsed = TRUE;
                    break;
                }
            }
            
            if(!b_MainUsed)
            {
                pt_src->u1VdpId = (UINT8)ucMVdpId;
                LOG(1,"Main use VDP_MAIN!\n");
                break;
            }
        }

        if(ucSVdpId != VDP_MAX)
        {
            for(i = 0; i < B2R_HW_MAX_ID; i ++)
            {
                pt_this = _FBM_B2rResGetObj(i);
                if(pt_this->t_prm.u1VdpId == ucSVdpId)
                {
                    b_SubUsed = TRUE;
                    break;
                }
            }
            
            if(!b_SubUsed)
            {
                pt_src->u1VdpId = (UINT8)ucSVdpId;
                LOG(1,"Sub use VDP_SUB!\n");
                break;
            }
        }

        if(pt_src->u1VdpId == 0xFF ||
            (pt_src->u1VdpId != VDP_MAIN &&
            pt_src->u1VdpId != VDP_SUB))
        {
            pt_src->u1VdpId = VDP_MAIN;

            LOG(1,"Default use VDP_MAIN!\n");
            break;
        }

    }while(0);
}

void _FBM_B2rResGetSrcInfo(UCHAR ucFbgId,
                                     B2R_IPT_INFO_T* pt_src,
                                     FBM_CREATE_FBG_PAR_T* ptPar)
{
    VDEC_ES_INFO_T*      ptVdecEsInfo = NULL;
    VDEC_ES_INFO_KEEP_T* ptVdecEsInfoKeep = NULL;
    
    do
    {
        if(VERIFY_FBG(ucFbgId))
        {
            break;
        }

        if(!pt_src || !ptPar)
        {
            break;
        }
        
        ptVdecEsInfo = _VDEC_GetEsInfo(_arFbg[ucFbgId].u1DecoderSrcId);
        if(!ptVdecEsInfo)
        {
            break;
        }
        
        ptVdecEsInfoKeep = _VDEC_GetEsInfoKeep(_arFbg[ucFbgId].u1DecoderSrcId);
        if(!ptVdecEsInfoKeep)
        {
            break;
        }

        pt_src->t_vd.eCodec = ptVdecEsInfoKeep->eCurFMT;
        pt_src->t_vd.fgUfoEn = ptVdecEsInfo->fgEnableUFO;
        pt_src->t_vd.u1PixDepth = ptVdecEsInfo->u4BitDepth;
        pt_src->t_vd.u4Resolution = ptVdecEsInfo->u2OrgHSize * ptVdecEsInfo->u2OrgVSize;

        LOG(1,"eCodec(%d), UfoEn(%d), Depth(%d), Res(%d)!\n", pt_src->t_vd.eCodec,
            pt_src->t_vd.fgUfoEn, pt_src->t_vd.u1PixDepth, pt_src->t_vd.u4Resolution);
    }while(0);
}

void FBM_B2rResNotifyClient(UCHAR ucFbgId,
                                   B2R_CHG_INFO_T*  pt_change)
{

    FBM_CB_FUNC_T* pt_cb_func = NULL;
    VDEC_ES_INFO_KEEP_T* ptVdecEsInfoKeep;

    do
    {
        if(VERIFY_FBG(ucFbgId) || pt_change == NULL)
        {
            LOG(0,"Invalid argument!\n");
            break;
        }
        
        pt_cb_func = FBM_GetCbFuncTbl();
        if(!pt_cb_func)
        {
            LOG(1,"pt_cb_func = NULL!\n");
            break;
        }
        
        if (FBM_CHECK_CB_FUNC_VERIFY(pt_cb_func->au4CbFunc[FBM_CB_FUNC_B2R_HW_CHG_CB],
                                 pt_cb_func->au4CbFuncCRC[FBM_CB_FUNC_B2R_HW_CHG_CB]))
        {
            LOG(1,"Send Hw change Notify with vdp(%d) and change(%d)!\n",
                pt_change->u1VdpId,pt_change->b_change);
            ((FBM_B2R_RES_CHG_FUNC)pt_cb_func->au4CbFunc[FBM_CB_FUNC_B2R_HW_CHG_CB])(ucFbgId, pt_change);
        }

        ptVdecEsInfoKeep = _VDEC_GetEsInfoKeep(_arFbg[ucFbgId].u1DecoderSrcId);
        if(!ptVdecEsInfoKeep)
        {
            LOG(1,"Es info Null!\n");
            break;
        }

        if (ptVdecEsInfoKeep->pfnVdecSetParam && 
            ((pt_change->u1B2rId == 2) || 
            (pt_change->u1B2rId == 3)))
        {
            ptVdecEsInfoKeep->pfnVdecSetParam(_arFbg[ucFbgId].u1DecoderSrcId, (UINT32)VDEC_BLK_MODE, 0, 0, 0);
        }

    }while(0);
}

UINT8 FBM_B2rResIdAccess(UCHAR ucFbgId, 
                                   UINT32 u4Mask,
                                   B2R_IPT_INFO_T* pt_src)
{
    UINT8 u1B2rId = B2R_HW_MAX_ID;
    B2R_HW_RES_T* pt_this = NULL;
    B2R_CHG_INFO_T  t_change;
    
        if(VERIFY_FBG(ucFbgId))
        {
        LOG(0,"Unvalid fbgid(%d)!\n",ucFbgId);
        return u1B2rId;
        }

    FBM_MUTEX_LOCK(ucFbgId);
    
    do
    {
        
        if(!pt_src &&
            u4Mask & RES_RW)
        {
            LOG(0,"It is RW mode but the src == NULL!\n");
            break;
        }
        
        /*use fbgid to check b2r resource is in use or not*/
        if(_FBM_B2rResInUsing(ucFbgId, &u1B2rId))
        {
            LOG(1,"The fbgid(%d) with b2rid(%d) is in using now!",ucFbgId,u1B2rId);
            break;
        }

        if(u4Mask & RES_RW)
        {
            if(fgPipLine)
				pt_src->u1VdpId= 0;    // for LG case
		   else
                FBM_B2rResHdrVdpId(ucFbgId, pt_src);

            LOG(0,"B2r Res get VdpId(%d)!,fgPipLine=%d\n", pt_src->u1VdpId,fgPipLine);
            
            u1B2rId = FBM_B2rResAlloc(pt_src);
            
            LOG(0,"Alloc B2r(%d) hardware resource!\n", u1B2rId);
            
            pt_this = _FBM_B2rResGetObj(u1B2rId);
            if(pt_this)
            {
                pt_this->t_prm.u1FbgId = (UINT8)ucFbgId;
                pt_this->t_prm.b_used  = TRUE;
                pt_this->t_prm.u1VdpId = pt_src->u1VdpId;

                x_memcpy(&pt_this->t_src, &pt_src->t_vd, sizeof(B2R_VD_INFO_T));

                x_memset(&t_change,0,sizeof(B2R_CHG_INFO_T));

                t_change.u1VdpId = pt_this->t_prm.u1VdpId;
                t_change.u1B2rId = u1B2rId;
                t_change.b_change = TRUE;

                LOG(0,"Alloc B2r(%d) OK with fbgid(%d) and vdpid(%d)!\n", u1B2rId,ucFbgId,pt_src->u1VdpId);

                FBM_B2rResNotifyClient(ucFbgId, &t_change);
            }

        }

        
    }while(0);

    FBM_MUTEX_UNLOCK(ucFbgId);

    return u1B2rId;
}

UINT8 FBM_B2rResIdRelease(UCHAR ucFbgId)
{
    UINT8 u1B2rId = B2R_HW_MAX_ID;
    B2R_HW_RES_T* pt_this = NULL;
    B2R_CHG_INFO_T  t_change;

    do
    {
        if(ucFbgId >= FBG_MAX_NS)
        {
            break;
        }

        if(VERIFY_FBG(ucFbgId))
        {
            break;
        }

        if(_FBM_B2rResInUsing(ucFbgId, &u1B2rId))
        {
            pt_this = _FBM_B2rResGetObj(u1B2rId);

            x_memset(&t_change,0,sizeof(B2R_CHG_INFO_T));

            t_change.u1VdpId = pt_this->t_prm.u1VdpId;
            t_change.b_change = TRUE;
                
            _FBM_B2rResPrmInit(pt_this);

            FBM_B2rResNotifyClient(ucFbgId, &t_change);
            LOG(1,"Release B2r(%d) hardware resource!\n", u1B2rId);
        }

    }while(0);

    return u1B2rId;
}

#endif

