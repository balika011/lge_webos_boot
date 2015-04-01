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
 * $Date: 2015/04/01 $
 * $RCSfile: fbm_fb.c,v $
 * $Revision: #20 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_fb.c
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "fbm_fb.h"
#include "fbm_drvif.h"
#include "fbm_pool.h"
#include "fbm_debug.h"
#include "x_assert.h"
#include "x_syslog.h"
#include "vdec_drvif.h"
#include "vdp_prescale_drvif.h"
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"

#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

// FBM Frame Buffer Status Log
#define NEW_SEQ_CHG_NOTIFY

//#define CC_GET_AFTER_RESIZE_DONE
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

/* wait FB for 10000 ms (VDP_INVALID_DRIFT) when the first pic is not displayed yet. */
#define FBM_WAIT_FB_TIMEOUT 10000

#define FBM_PTS_FRM_RATE_12	7500
#define FBM_PTS_FRM_RATE_15 6000
#define FBM_PTS_FRM_RATE_20 4500
#define FBM_PTS_FRM_RATE_24 3750
#define FBM_PTS_FRM_RATE_25 3600
#define FBM_PTS_FRM_RATE_30 3000
#define FBM_PTS_FRM_RATE_50 1800
#define FBM_PTS_FRM_RATE_60 1500

#define FBM_PTS_FRM_RATE_TORRENCE 90

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define VERIFY_FBG(id)                                  \
    ((id >= FBG_MAX_NS) ||                              \
    (_prFbg[id].ucFbgType == FBM_FBG_TYPE_UNKNOWN) ||   \
    (_prFbg[id].ucFbgId != id))

#define VERIFY_FB(gid, id)                              \
    (!(((_prFbg[gid].ucFbNsBase <= id) && (id < _prFbg[gid].ucFbNs))   ||  \
       ((_prFbg[gid].ucFbNsOldBase <= id) && (id < _prFbg[gid].ucFbNsOld)))   || \
    (id >= FBM_MAX_FB_NS_PER_GROUP) || \
    (_prFbg[gid].aucFbRotationStatus[id] == FB_ROTATION_UNUSE) || (_prFbg[gid].aucFbRotationStatus[id] == FB_ROTATION_WAIT_USE))

#define VERIFY_RESIZE_FB(gid, id)                       \
    ((id >= _prFbg[gid].ucSeamlessFbNs) ||              \
    (id >= FBM_MAX_FB_NS_PER_GROUP))

#define VERIFY_FB_NS(gid)                               \
    (_prFbg[gid].ucFbNs - _prFbg[gid].ucFbNsBase > FBM_MAX_FB_NS_PER_GROUP)

#define IS_REFERENCE_FB(gid, id)                        \
    ((id == _prFbg[gid].ucFbFRef) || (id == _prFbg[gid].ucFbBRef))

#define NOT_REFERENCE_FB(gid, id)                        \
    ((id != _prFbg[gid].ucFbFRef) && (id != _prFbg[gid].ucFbBRef))

#define Fb_Using(gid,id) (_prFbg[gid].aucFbRotationStatus[id] == FB_ROTATION_USE || _prFbg[gid].aucFbRotationStatus[id] == FB_ROTATION_WAIT_UNUSE)

#define IS_NEW_FBS(gid, id)  ((_prFbg[gid].ucFbNsBase <= id) && (id < _prFbg[gid].ucFbNs))
#define IS_OLD_FBS(gid, id)  (FALSE)  //( (_prFbg[gid].ucFbNsOldBase <= id) && (id < _prFbg[gid].ucFbNsOld))

#ifdef CC_VDP_FULL_ISR

#define FBM_MUTEX_LOCK(id)                              \
    (*_prFbg[id].prState = x_crit_start())

#define FBM_MUTEX_UNLOCK(id)                            \
    (x_crit_end(*_prFbg[id].prState))

#else

#define FBM_MUTEX_LOCK(id)                              \
    (VERIFY(x_sema_lock(_prFbg[id].hMutex, X_SEMA_OPTION_WAIT) == OSR_OK))

#define FBM_MUTEX_UNLOCK(id)                            \
    (VERIFY(x_sema_unlock(_prFbg[id].hMutex) == OSR_OK))

#endif


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void _EnableNewSeamless(UINT32 u4FbgId);
extern VOID i4VDOOmxRenderFrame(UINT8 ucType);
extern VOID FBM_FbMemReset(UCHAR ucFbgId,UCHAR ucFbId);
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _FbmPutFrameBufferToDispQ(UCHAR ucFbgId, UCHAR ucFbId);

static UCHAR _FbmGetRefFrameBufferFromEmptyQ(UCHAR ucFbgId);

static UCHAR _FbmGetBFrameBufferFromEmptyQ(UCHAR ucFbgId);

static void _FbmResetPicHdrFields(UCHAR ucFbgId, UCHAR ucFbId);

static UCHAR _FbmSyncWaitUseFb(UCHAR ucFbgId);

#ifdef FBM_FB_LOG
static void _FbmFbLog(UCHAR u1FbgIdx,UCHAR u1FbIdx, UCHAR ucFbStatus, const CHAR* sFunc, const UINT32 u4Line);
#define  ASSERT_FBM(x) \
{\
    if(!(x))\
    {\
        FBM_FlushFbLog(0xFC, 0xFF);\
        FBM_QueryStatus(0xff);\
        LOG(0, "FBM is Going to Assert. Sleep 60s.\n");\
        x_thread_delay(60000);\
        ASSERT(x);\
    }\
}
#else
#define _FbmFbLog(xx,x, y, z, zz)     ((void)0)
#define  ASSERT_FBM(x)             (ASSERT(x))
#endif

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static FBM_FBG_T* _prFbg = NULL;

static FBM_CB_FUNC_T* _prFbmCbFunc = NULL;

#ifdef NEW_SEQ_CHG_NOTIFY
static UINT32 u4FirstFBStored = 1;
static FBM_FBQ_T rSeqChgEmptyQ; /* Empty Q while sequence change */
static UCHAR _ucSeqChgFbgId = 0xFF;     /* FBG id, specified by decoder if need apply seq chg on it */
static UCHAR _ucSeqChgFbId = 0xFF;      /* FB id, specified by decoder if need apply seq chg on it */
#endif

/* FBM global flag, not related to FBG index */
static UINT8 _ucFbmGlobalFlag = 0;
static BOOL _fgCheckFBStatus = TRUE;
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _FbmPutFrameBufferToDispQ
 *  Put FB to display queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
static void _FbmPutFrameBufferToDispQ(UCHAR ucFbgId, UCHAR ucFbId)
{
    UCHAR ucNextWriteIdx;

#ifdef NEW_SEQ_CHG_NOTIFY
    UCHAR i;
    UCHAR ucGetFbId;
#endif

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }
    if (_prFbg[ucFbgId].rDisplayQ.ucCount >= FBM_MAX_FB_NS_PER_GROUP)
    {
        LOG(0,"FbgId(%d) DisplayQ.Count(%d)\n",ucFbgId,_prFbg[ucFbgId].rDisplayQ.ucCount);
    }
    
    ASSERT_FBM(_prFbg[ucFbgId].rDisplayQ.ucCount < FBM_MAX_FB_NS_PER_GROUP);

#ifdef NEW_SEQ_CHG_NOTIFY
    if ((_prFbg[ucFbgId].ucSeqChg == 1) && (u4FirstFBStored == 0))
    {
        UINT8 u1ReadIdx = rSeqChgEmptyQ.ucReadIdx;

        LOG(5,"[LEO]PutFBToDispQ %d ,Type :%d\n",ucFbId,_prFbg[ucFbgId].prPicHdr[ucFbId].ucPicCdTp);


        for (i=0; i<rSeqChgEmptyQ.ucCount; i++)
        {
            if (rSeqChgEmptyQ.ucReadIdx >= FBM_MAX_FB_NS_PER_GROUP)
            {
                if (rSeqChgEmptyQ.ucReadIdx >= FBM_MAX_FB_NS_PER_GROUP)
                {
                    LOG(0,"ucReadIdx %d\n",rSeqChgEmptyQ.ucReadIdx);
                }
                ASSERT_FBM(rSeqChgEmptyQ.ucReadIdx < FBM_MAX_FB_NS_PER_GROUP);
                return;
            }

            ucGetFbId = rSeqChgEmptyQ.aucQueue[u1ReadIdx];

            if (++(u1ReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
            {
                u1ReadIdx = 0;
            }
            if (ucGetFbId == FBM_FB_ID_UNKNOWN)
            {
                LOG(0,"ucGetFbId %d\n",ucGetFbId);
            }
            ASSERT_FBM(ucGetFbId != FBM_FB_ID_UNKNOWN);
            if (ucGetFbId == ucFbId)
            {
                _prFbg[ucFbgId].ucNewFbId = ucFbId;
                LOG(5,"[LEO] tag7,ucNewFbId = %d\n",ucFbId);
                u4FirstFBStored = 1;
            }
        }

        /* Special scenario for H.264 decoder.
         * H.264 may request an empty buffer before seq chg, it caused 1 frame mismatch
         * FBM provide an API, H.264 decoder can specify the SeqChg should applied on which FB.
         */
        if (u4FirstFBStored == 0)
        {
            if ((ucFbgId == _ucSeqChgFbgId) && (ucFbId == _ucSeqChgFbId))
            {
                _prFbg[ucFbgId].ucNewFbId = ucFbId;
                u4FirstFBStored = 1;
            }
        }
    }
#endif

    ucNextWriteIdx = (_prFbg[ucFbgId].rDisplayQ.ucReadIdx + _prFbg[ucFbgId].rDisplayQ.ucCount) % FBM_MAX_FB_NS_PER_GROUP;
    _prFbg[ucFbgId].rDisplayQ.aucQueue[ucNextWriteIdx] = ucFbId;
    _prFbg[ucFbgId].rDisplayQ.ucCount++;
    LOG(4,"DispInQ fb(%d,%d) Cnt(%d,%d)\n",ucFbgId,ucFbId,_prFbg[ucFbgId].rDisplayQ.ucCount);
    
}


//-------------------------------------------------------------------------
/** _FbmGetRefFrameBufferFromEmptyQ
 *  Get reference FB from empty queue
 *  @param  ucFbgId   FBG index.
 *  @return FB index.
 */
//-------------------------------------------------------------------------
static UCHAR _FbmGetRefFrameBufferFromEmptyQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    while (_prFbg[ucFbgId].rEmptyQ.ucCount > 0)
    {
        ucFbId = _prFbg[ucFbgId].rEmptyQ.aucQueue[_prFbg[ucFbgId].rEmptyQ.ucReadIdx];

        if (++(_prFbg[ucFbgId].rEmptyQ.ucReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
        {
            _prFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;
        }
        _prFbg[ucFbgId].rEmptyQ.ucCount--;
        
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
        }
        
        ASSERT_FBM(ucFbId != FBM_FB_ID_UNKNOWN);
		
		if (_prFbg[ucFbgId].au4GfxMemsetId[ucFbId] &&
			FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE]))
		{
			 while(0 == ((FBM_GFX_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GfxMemsetId[ucFbId]))
			{
				x_thread_delay(1);
				//wait
			};
			_prFbg[ucFbgId].au4GfxMemsetId[ucFbId] = 0;
		}		
		if (_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId] &&
		    _prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId] &&
			FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE]))
		{
			 while(0 == ((FBM_GDMA_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId]))
			{
				x_thread_delay(1);
				//wait
			};
			while(0 == ((FBM_GDMA_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId]))
			{
				x_thread_delay(1);
				//wait
			};			
			
			_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId] = 0;
			_prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId] = 0;			
		}					
 		
        return ucFbId;
    }

    return FBM_FB_ID_UNKNOWN;
}


//-------------------------------------------------------------------------
/** _FbmGetBFrameBufferFromEmptyQ
 *  Get B FB from empty queue
 *  @param  ucFbgId   FBG index.
 *  @return FB index.
 */
//-------------------------------------------------------------------------
static UCHAR _FbmGetBFrameBufferFromEmptyQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;
    UINT32 u4Count;
    UINT32 u4ReadIdx;
    UINT32 u4TargetIdx;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    ucFbId = FBM_FB_ID_UNKNOWN;

    // search for B frame from Empty Queue
    u4Count = _prFbg[ucFbgId].rEmptyQ.ucCount;
    u4ReadIdx = _prFbg[ucFbgId].rEmptyQ.ucReadIdx;
    u4TargetIdx = FBM_MAX_FB_NS_PER_GROUP;
    while (u4Count > 0)
    {
        if (u4ReadIdx >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"u4ReadIdx %d\n",u4ReadIdx);
            
            ASSERT_FBM(u4ReadIdx < FBM_MAX_FB_NS_PER_GROUP);
            return FBM_FB_ID_UNKNOWN;
        }
        ucFbId = _prFbg[ucFbgId].rEmptyQ.aucQueue[u4ReadIdx];

        if (NOT_REFERENCE_FB(ucFbgId, ucFbId))
        {   // found B frame
            u4TargetIdx = u4ReadIdx;
            u4Count = 0;
        }
        else
        {   // skip Reference frame
            ucFbId = FBM_FB_ID_UNKNOWN;
            if (++u4ReadIdx >= FBM_MAX_FB_NS_PER_GROUP)
            {
                u4ReadIdx = 0;
            }
            u4Count--;
        }
    }

    if (u4TargetIdx != FBM_MAX_FB_NS_PER_GROUP)
    {
        // move back reference frame
        while (u4TargetIdx != _prFbg[ucFbgId].rEmptyQ.ucReadIdx)
        {
            if (u4TargetIdx > 0)
            {
                u4ReadIdx = u4TargetIdx - 1;
            }
            else
            {
                u4ReadIdx = FBM_MAX_FB_NS_PER_GROUP - 1;
            }

            _prFbg[ucFbgId].rEmptyQ.aucQueue[u4TargetIdx] = _prFbg[ucFbgId].rEmptyQ.aucQueue[u4ReadIdx];

            u4TargetIdx = u4ReadIdx;
        }
    }
    else
    {
        if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
        {
            return FBM_FB_ID_UNKNOWN;
        }
        else
        {
            ASSERT_FBM(0);
        }
    }

    _prFbg[ucFbgId].rEmptyQ.ucReadIdx =
        (UCHAR)((_prFbg[ucFbgId].rEmptyQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP);
    _prFbg[ucFbgId].rEmptyQ.ucCount--;
	
	if (_prFbg[ucFbgId].au4GfxMemsetId[ucFbId] &&
		FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE]))
	{
		 while(0 == ((FBM_GFX_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GfxMemsetId[ucFbId]))
		{
			x_thread_delay(1);
			//wait
		};
		 _prFbg[ucFbgId].au4GfxMemsetId[ucFbId] = 0;
	}
		if (_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId] &&
		    _prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId] &&
			FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE]))
		{
			 while(0 == ((FBM_GDMA_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId]))
			{
				x_thread_delay(1);
				//wait
			};
			while(0 == ((FBM_GDMA_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId]))
			{
				x_thread_delay(1);
				//wait
			};			
			
			_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId] = 0;
			_prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId] = 0;			
		}		
	

	
    return ucFbId;
}

//-------------------------------------------------------------------------
/** _FbmResetPicHdrFields
 *  Reset Picture header fields
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId    FB index.
 *  @return
 */
//-------------------------------------------------------------------------
static void _FbmResetPicHdrFields(UCHAR ucFbgId, UCHAR ucFbId)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }
    _prFbg[ucFbgId].prPicHdr[ucFbId].u2DecodingOrder = 0;
    _prFbg[ucFbgId].prPicHdr[ucFbId].u4TotlaTimeOffset = 0;
    _prFbg[ucFbgId].prPicHdr[ucFbId].u4MmFlag = 0;
    _prFbg[ucFbgId].prPicHdr[ucFbId].u8Offset = 0;
    _prFbg[ucFbgId].prPicHdr[ucFbId].u8OffsetI = 0;
    _prFbg[ucFbgId].prPicHdr[ucFbId].fgNotDisplay = FALSE;
    _prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessResized = FALSE;
    _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid = FBM_FB_ID_UNKNOWN;
    _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid = FBM_FB_ID_UNKNOWN;   
    _prFbg[ucFbgId].u4DispTag[ucFbId] = INVALID_DISPTAG;
    _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessTaskId = 0xFF;
    x_memset(&_prFbg[ucFbgId].prPicHdr[ucFbId].rExtra, 0, sizeof(FBM_PIC_EXTRA_INFO_T));
}

//-------------------------------------------------------------------------
/** _FbmGetLatestEmptyFrameBufferFromEmptyQ
 *  Get LATEST empty FB from empty queue for MM STEP BACKWARD application
 *  @param  ucFbgId   FBG index.
 *  @return FB index.
 */
//-------------------------------------------------------------------------
static UCHAR _FbmGetLatestEmptyFrameBufferFromEmptyQ(UCHAR ucFbgId)
{
    UCHAR ucIndex;
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    while (_prFbg[ucFbgId].rEmptyQ.ucCount > 0)
    {
        ucIndex = (_prFbg[ucFbgId].rEmptyQ.ucReadIdx+_prFbg[ucFbgId].rEmptyQ.ucCount-1)% FBM_MAX_FB_NS_PER_GROUP;
        ucFbId = _prFbg[ucFbgId].rEmptyQ.aucQueue[ucIndex];

        _prFbg[ucFbgId].rEmptyQ.ucCount--;
        
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
        }
        ASSERT_FBM(ucFbId != FBM_FB_ID_UNKNOWN);
        return ucFbId;
    }

    return FBM_FB_ID_UNKNOWN;
}

BOOL _FbmQueryFromEmptyQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;
    UCHAR ucReadIdx = 0;
    UCHAR ucEmptyCnt = 0;

    if (VERIFY_FBG(ucFbgId))
    {
        return FALSE;
    }
    
    ucReadIdx = _prFbg[ucFbgId].rEmptyQ.ucReadIdx;
    
    for (ucEmptyCnt = 0; ucEmptyCnt < _prFbg[ucFbgId].rEmptyQ.ucCount; ucEmptyCnt++)
    {            
        ucFbId = _prFbg[ucFbgId].rEmptyQ.aucQueue[ucReadIdx];

        if (++(ucReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
        {
            ucReadIdx = 0;
        }
        
        LOG(0,"EmpQue(%d) FbId(%d) PTS(0x%8x)\n",ucEmptyCnt,ucFbId,_prFbg[ucFbgId].prPicHdr ? _prFbg[ucFbgId].prPicHdr[ucFbId].u4PTS : 0); 
    }
    
    return TRUE;

    
}

BOOL _FbmQueryFromDisplayQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;
    UCHAR ucReadIdx = 0;
    UCHAR ucDispCnt = 0;

    if (VERIFY_FBG(ucFbgId))
    {
        return FALSE;
    }
    
    ucReadIdx = _prFbg[ucFbgId].rDisplayQ.ucReadIdx;
    
    for (ucDispCnt = 0; ucDispCnt < _prFbg[ucFbgId].rDisplayQ.ucCount; ucDispCnt++)
    {            
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[ucReadIdx];

        if (++(ucReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
        {
            ucReadIdx = 0;
        }
        
        LOG(0,"DisplayQue(%d) FbId(%d) PTS(0x%8x)\n",ucDispCnt,ucFbId,_prFbg[ucFbgId].prPicHdr ? _prFbg[ucFbgId].prPicHdr[ucFbId].u4PTS : 0); 
    }
    
    return TRUE;

}


//-------------------------------------------------------------------------
/** _FbmGetEmptyFrameBufferFromEmptyQ
 *  Get empty FB from empty queue. H264 ONLY.
 *  @param  ucFbgId   FBG index.
 *  @return FB index.
 */
//-------------------------------------------------------------------------
static UCHAR _FbmGetEmptyFrameBufferFromEmptyQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }


    while (_prFbg[ucFbgId].rEmptyQ.ucCount > 0)
    {
        ucFbId = _prFbg[ucFbgId].rEmptyQ.aucQueue[_prFbg[ucFbgId].rEmptyQ.ucReadIdx];

        if (++(_prFbg[ucFbgId].rEmptyQ.ucReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
        {
            _prFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;
        }
        _prFbg[ucFbgId].rEmptyQ.ucCount--;
        
        LOG(4,"EmpytDeQ fb(%d,%d) Cnt(%d,%d)\n",ucFbgId,ucFbId,
          _prFbg[ucFbgId].rEmptyQ.ucCount,_prFbg[ucFbgId].hEmptyQSemaphore.i4Count);
        
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
        }
        ASSERT_FBM(ucFbId != FBM_FB_ID_UNKNOWN);
        
        if (_prFbg[ucFbgId].au4GfxMemsetId[ucFbId] &&
            FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE]))
        {
             while(0 == ((FBM_GFX_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GFX_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GfxMemsetId[ucFbId]))
            {
                x_thread_delay(1);
                //wait
            };
             _prFbg[ucFbgId].au4GfxMemsetId[ucFbId] = 0;
        }
		if (_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId] &&
		    _prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId] &&
			FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE]))
		{
			 while(0 == ((FBM_GDMA_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId]))
			{
				x_thread_delay(1);
				//wait
			};
			while(0 == ((FBM_GDMA_TASK_MEMSET_DONE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_GDMA_TASK_MEMSET_DONE])(_prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId]))
			{
				x_thread_delay(1);
				//wait
			};			
			
			_prFbg[ucFbgId].au4GdmaMemsetYId[ucFbId] = 0;
			_prFbg[ucFbgId].au4GdmaMemsetCId[ucFbId] = 0;			
		}	        

        return ucFbId;
    }
    
    return FBM_FB_ID_UNKNOWN;
}

//-------------------------------------------------------------------------
/** _FbmGetEmptyFrameBufferFromEmptyResizeQ
 *  Get empty FB from empty resize queue.
 *  @param  ucFbgId   FBG index.
 *  @return FB index.
 */
//-------------------------------------------------------------------------
static UCHAR _FbmGetEmptyFrameBufferFromEmptyResizeQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }
    
    
    while (_prFbg[ucFbgId].rResizeEmptyQ.ucCount > 0)
    {
        ucFbId = _prFbg[ucFbgId].rResizeEmptyQ.aucQueue[_prFbg[ucFbgId].rResizeEmptyQ.ucReadIdx];

        if (++(_prFbg[ucFbgId].rResizeEmptyQ.ucReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
        {
            _prFbg[ucFbgId].rResizeEmptyQ.ucReadIdx = 0;
        }
        _prFbg[ucFbgId].rResizeEmptyQ.ucCount--;
        
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
        }
        ASSERT_FBM(ucFbId != FBM_FB_ID_UNKNOWN);
        return ucFbId;
    }

    return FBM_FB_ID_UNKNOWN;
}

//-------------------------------------------------------------------------
/** _FbmGetEmptyFrameBufferFromEmptyResize2Q
 *  Get empty FB from empty resize queue.
 *  @param  ucFbgId   FBG index.
 *  @return FB index.
 */
//-------------------------------------------------------------------------
static UCHAR _FbmGetEmptyFrameBufferFromEmptyResize2Q(UCHAR ucFbgId)
{
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }


    while (_prFbg[ucFbgId].rResize2EmptyQ.ucCount > 0)
    {
        ucFbId = _prFbg[ucFbgId].rResize2EmptyQ.aucQueue[_prFbg[ucFbgId].rResize2EmptyQ.ucReadIdx];

        if (++(_prFbg[ucFbgId].rResize2EmptyQ.ucReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
        {
            _prFbg[ucFbgId].rResize2EmptyQ.ucReadIdx = 0;
        }
        _prFbg[ucFbgId].rResize2EmptyQ.ucCount--;
        
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
        }
        ASSERT_FBM(ucFbId != FBM_FB_ID_UNKNOWN);
        return ucFbId;
    }

    return FBM_FB_ID_UNKNOWN;
}


#ifdef __KERNEL__
//-------------------------------------------------------------------------
/** _FbmGetFrameBufferFromDispQ
 *  Get FB from empty queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId    FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
static void _FbmGetFrameBufferFromDispQ(UCHAR ucFbgId, UCHAR ucFbId)
{
    UINT32 u4Count;
    UINT32 u4ReadIdx;
    UINT32 u4TargetIdx;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }
    if (_prFbg[ucFbgId].rDisplayQ.ucCount >= FBM_MAX_FB_NS_PER_GROUP)
    {
        LOG(0,"ucFbgId %d rDisplayQ.ucCount(%d)\n",ucFbgId,_prFbg[ucFbgId].rDisplayQ.ucCount);
    }

    ASSERT_FBM(_prFbg[ucFbgId].rDisplayQ.ucCount < FBM_MAX_FB_NS_PER_GROUP);

    // search FB from Display Queue
    u4Count = _prFbg[ucFbgId].rDisplayQ.ucCount;
    u4ReadIdx = _prFbg[ucFbgId].rDisplayQ.ucReadIdx;
    u4TargetIdx = FBM_MAX_FB_NS_PER_GROUP;

    if (u4ReadIdx >= FBM_MAX_FB_NS_PER_GROUP)
    {
        ASSERT_FBM(0);
        u4ReadIdx = 0;
    }

    while (u4Count > 0)
    {
        if (ucFbId == _prFbg[ucFbgId].rDisplayQ.aucQueue[u4ReadIdx])
        {
            u4TargetIdx = u4ReadIdx;
            u4Count = 0;
        }
        else
        {
            if (++u4ReadIdx >= FBM_MAX_FB_NS_PER_GROUP)
            {
                u4ReadIdx = 0;
            }
            u4Count--;
        }
    }

    if (u4TargetIdx != FBM_MAX_FB_NS_PER_GROUP)
    {
        // move back other frame buffer
        while (u4TargetIdx != _prFbg[ucFbgId].rDisplayQ.ucReadIdx)
        {
            if (u4TargetIdx > 0)
            {
                u4ReadIdx = u4TargetIdx - 1;
            }
            else
            {
                u4ReadIdx = FBM_MAX_FB_NS_PER_GROUP - 1;
            }
            _prFbg[ucFbgId].rDisplayQ.aucQueue[u4TargetIdx] = _prFbg[ucFbgId].rDisplayQ.aucQueue[u4ReadIdx];

            u4TargetIdx = u4ReadIdx;
        }
    }
    else
    {
        if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
        {
            return;
        }
        else
        {
            ASSERT_FBM(0);
        }
    }

    _prFbg[ucFbgId].rDisplayQ.ucReadIdx = (_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP;
    _prFbg[ucFbgId].rDisplayQ.ucCount--;

}
#endif

BOOL  g_bEnableDebugLog = TRUE;

#ifdef FBM_FB_LOG
#define FBM_FB_LOG_NS 64
typedef struct{
	UCHAR   ucFbgId;
    UCHAR   ucFBId;
    CHAR    ucStatus;
    CHAR    sCallFunc[64];
    UINT32  u4CallLine;
    HAL_TIME_T tCallTime;
}FBM_FB_LOG_T;

static FBM_FB_LOG_T tDebugLog[FBM_FB_LOG_NS];
static UINT32 u4DebugIdx = 0;
static HANDLE_T hFbLogMutex = NULL_HANDLE;

//-----------------------------------------------------------------------------
/** Brief of _FbmFbLog.
 */
//-----------------------------------------------------------------------------
static void _FbmFbLog(UCHAR u1FbgIdx,UCHAR u1FbIdx, UCHAR ucFbStatus, const CHAR* sFunc, const UINT32 u4Line)
{
    UINT32 u4_idx = 0;
    
    if (!g_bEnableDebugLog)
    {
        return;
    }    
    
    if (hFbLogMutex == NULL_HANDLE)
    {
        // create semaphore
        VERIFY(x_sema_create(&hFbLogMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
        x_memset(tDebugLog, 0, FBM_FB_LOG_NS*sizeof(FBM_FB_LOG_T));
        #if 0
        for (u4DebugIdx = 0; u4DebugIdx < FBM_FB_LOG_NS; u4DebugIdx++)
        {
            szDebug1[u4DebugIdx] = 0;
            szDebug2[u4DebugIdx] = 0;
        }
        #endif

        u4DebugIdx = 0;
    }

    VERIFY(x_sema_lock(hFbLogMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    tDebugLog[u4DebugIdx].ucFbgId = u1FbgIdx;
    tDebugLog[u4DebugIdx].ucFBId = u1FbIdx;
    tDebugLog[u4DebugIdx].ucStatus = ucFbStatus;
    tDebugLog[u4DebugIdx].u4CallLine = u4Line;
    x_memcpy(tDebugLog[u4DebugIdx].sCallFunc, sFunc, x_strlen(sFunc));
    tDebugLog[u4DebugIdx].sCallFunc[x_strlen(sFunc)] = '\0';
    HAL_GetTime(&(tDebugLog[u4DebugIdx].tCallTime));
    
    if ((++u4DebugIdx >= FBM_FB_LOG_NS) && (u1FbIdx != 0xFC))
    {
        for(u4_idx = 0; u4_idx < FBM_FB_LOG_NS; u4_idx++)
        {
            LOG(6, "%d: [%d.%06d][%-32s-%4d] FBG(%d) FB(%d) Status(%d).\n", u4_idx,
                tDebugLog[u4_idx].tCallTime.u4Seconds, tDebugLog[u4_idx].tCallTime.u4Micros,
                tDebugLog[u4_idx].sCallFunc, tDebugLog[u4_idx].u4CallLine, 
                tDebugLog[u4_idx].ucFbgId,
                tDebugLog[u4_idx].ucFBId, tDebugLog[u4_idx].ucStatus);
        }
        LOG(6, "u4DebugIdx = %d\n", u4DebugIdx);
        u4DebugIdx = 0;
    }
    
    if(u1FbIdx == 0xFC)
    {
        for(u4_idx = u4DebugIdx; u4_idx < FBM_FB_LOG_NS; u4_idx++)
        {
            LOG(0, "%d: [%d.%06d][%-32s-%4d] FBG(%d) FB(%d) Status(%d).\n", u4_idx,
                tDebugLog[u4_idx].tCallTime.u4Seconds, tDebugLog[u4_idx].tCallTime.u4Micros,
                tDebugLog[u4_idx].sCallFunc, tDebugLog[u4_idx].u4CallLine, 
                tDebugLog[u4_idx].ucFbgId,
                tDebugLog[u4_idx].ucFBId, tDebugLog[u4_idx].ucStatus);

        }
        LOG(0, "u4DebugIdx = %d\n", u4DebugIdx);
        for(u4_idx = 0; u4_idx < u4DebugIdx; u4_idx++)
        {
            LOG(0, "%d: [%d.%06d][%-32s-%4d] FBG(%d) FB(%d) Status(%d).\n", u4_idx,
                tDebugLog[u4_idx].tCallTime.u4Seconds, tDebugLog[u4_idx].tCallTime.u4Micros,
                tDebugLog[u4_idx].sCallFunc, tDebugLog[u4_idx].u4CallLine, 
                tDebugLog[u4_idx].ucFbgId,
                tDebugLog[u4_idx].ucFBId, tDebugLog[u4_idx].ucStatus);

        }
        u4DebugIdx = 0;
    }


    VERIFY(x_sema_unlock(hFbLogMutex) == OSR_OK);
}

#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _FBM_FbInit
 *  Set FBG structure pointer
 *  @param  prFbg   FBG structure pointer.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void _FBM_FbInit(FBM_FBG_T* prFbg)
{
    _prFbg = prFbg;

    _prFbmCbFunc = FBM_GetCbFuncTbl();
}

UCHAR FBM_CheckFbg(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return TRUE;
    }

    return FALSE;
}


//-------------------------------------------------------------------------
/** FBM_TriggerSeamless
 *  Trigger seamless 
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

VOID FBM_TriggerSmlsByResizer(UCHAR ucFbgId, UCHAR ucFbId)
{
    UCHAR ucRezId = 0;
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }
    if ((_prFbg[ucFbgId].prPicHdr[ucFbId].u4PicWidthPitch <= FBM_FBG_4K2K_H) &&
        (_prFbg[ucFbgId].prPicHdr[ucFbId].u4PicWidthPitch > FBM_FBG_1080HD_H) && 
        _prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessDisp)
    {
        _prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessDisp = FALSE;
        
        LOG(6,"FbgId(%d) ucFbId(%d) resolution achieve to 4K ,don't need imgrz\n",ucFbgId,ucFbId);        
    }
    
    
    FBM_MUTEX_LOCK(ucFbgId);

    if (_prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessDisp || _prFbg[ucFbgId].fgAutoSeamless || _prFbg[ucFbgId].prPicHdr[ucFbId].fgResizeDispFrm)
    {
        if(_prFbg[ucFbgId].prPicHdr[ucFbId].fgResizeDispFrm)
        {
            _prFbg[ucFbgId].ucSeamlessRzId = _prFbg[ucFbgId].prPicHdr[ucFbId].ucRzId;
        }
        else if (!_prFbg[ucFbgId].fgEnableNewSeamless)
        {
            UCHAR ucRzId;
            UCHAR ucRzIdMask = 0;

#if defined(ENABLE_MULTIMEDIA) && defined(CC_VDEC_RM_SUPPORT)
            VDEC_ES_INFO_T *prVdecEsInfo = NULL;
            prVdecEsInfo = _prFbg[ucFbgId].u1DecoderSrcId < VDEC_MAX_ES ? _VDEC_GetEsInfo(_prFbg[ucFbgId].u1DecoderSrcId) : NULL;
            ucRzIdMask = prVdecEsInfo ? prVdecEsInfo->ucImgrzId : 0;
#endif
            _prFbg[ucFbgId].ucSeamlessRzId = VDP_PRESCALE_RZ_ID_UNKNOWN;
            _prFbg[ucFbgId].ucSeamlessRzId2 = VDP_PRESCALE_RZ_ID_UNKNOWN;
            _prFbg[ucFbgId].ucSeamlessRzMask = 0x0;
            
            _EnableNewSeamless(ucFbgId);
            _prFbg[ucFbgId].fgEnableNewSeamless = TRUE;
            ucRzId = _VDP_Prescale_Init(ucRzIdMask);
            ASSERT_FBM(ucRzId != VDP_PRESCALE_RZ_ID_UNKNOWN);
            _prFbg[ucFbgId].ucSeamlessRzMask |= (1 << ucRzId);
            _prFbg[ucFbgId].ucSeamlessRzId = ucRzId;
            _VDP_SetPfn_RzGetFB(ucRzId,FBM_GetEmptyResizeFrameBuffer);
            _VDP_SetPfn_RzGetFBAddr(ucRzId,FBM_GetResizeFrameBufferAddr);
            _VDP_SetPfn_RzSetFBStatus(ucRzId,FBM_SetResizeFrameBufferStatus);
            //1 video with 2 imgrz
            if((_prFbg[ucFbgId].rSeqHdr.eSeamlessMode & SEAMLESS_TWO_RESIZER) == SEAMLESS_TWO_RESIZER)
            {
                ucRzId = _VDP_Prescale_Init(ucRzIdMask);
                ASSERT_FBM(ucRzId != VDP_PRESCALE_RZ_ID_UNKNOWN);
                _prFbg[ucFbgId].ucSeamlessRzMask |= (1 << ucRzId);
                _prFbg[ucFbgId].ucSeamlessRzId2 = ucRzId;
                _VDP_SetPfn_RzGetFB(ucRzId,FBM_GetEmptyResize2FrameBuffer);
                _VDP_SetPfn_RzGetFBAddr(ucRzId,FBM_GetResize2FrameBufferAddr);
                _VDP_SetPfn_RzSetFBStatus(ucRzId,FBM_SetResize2FrameBufferStatus);
            }
            LOG(1, "Enable new seamless, ucRzIdMask %d Rzid0 %d Rzid1 %d\n", ucRzIdMask,_prFbg[ucFbgId].ucSeamlessRzId,_prFbg[ucFbgId].ucSeamlessRzId2);
            //_prFbg[ucFbgId].ucSeamlessRzId = (ucRzId != VDP_PRESCALE_RZ_ID_UNKNOWN) ? ucRzIdMask : VDP_PRESCALE_RZ_ID_DEFAULT;
        }
        LOG(6, "Send resize fbid (%d, %d)\n",ucFbId, _prFbg[ucFbgId].aucFbStatus[ucFbId]);
        
        _prFbg[ucFbgId].ucCurSeamlessTaskId++;
        
        if (_prFbg[ucFbgId].ucCurSeamlessTaskId == 0xFF)
        {
            _prFbg[ucFbgId].ucCurSeamlessTaskId = 0;
        }
        ucRezId = _VDP_GetRzId(_prFbg[ucFbgId].ucSeamlessRzMask,_prFbg[ucFbgId].ucCurSeamlessTaskId);
        
        _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessTaskId = _prFbg[ucFbgId].ucCurSeamlessTaskId;
        _VDP_Prescale_Send(ucRezId, ucFbgId, ucFbId, _prFbg[ucFbgId].ucCurSeamlessTaskId);
    }
    FBM_MUTEX_UNLOCK(ucFbgId);

    return;
}

//-------------------------------------------------------------------------
/** _FBM_PutFrameBufferToEmptyQ
 *  Put FB to empty queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
BOOL _FBM_2FBsChkDynamicDone(UCHAR ucFbgId)
{
    UINT32 u4FbIdx = FBM_FB_ID_UNKNOWN;
    if(_prFbg[ucFbgId].ucFbOldEmptyCnt < _prFbg[ucFbgId].ucFbNsOld - _prFbg[ucFbgId].ucFbNsOldBase)
    {
        return FALSE;
    }
    
    LOG(0, "FBG(%d) 2FBs Dynamic Mode(%d) NsOldBase(%d) ucFbOldEmptyCnt(%d) End.\n", 
             ucFbgId,
            _prFbg[ucFbgId].eDynamicMode, 
            _prFbg[ucFbgId].ucFbNsOldBase,
            _prFbg[ucFbgId].ucFbOldEmptyCnt);

    for (u4FbIdx = _prFbg[ucFbgId].ucFbNsBase; u4FbIdx < _prFbg[ucFbgId].ucFbNs; u4FbIdx++)
    {
        if (_prFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_UNKNOWN)
        {
            _FBM_PutFrameBufferToEmptyQ(ucFbgId, u4FbIdx);
            LOG(1, "Warn: FBG(%d, %d) UNKNOW->EMPTY.\n", ucFbgId, u4FbIdx);
        }
    }
    
    if(_prFbg[ucFbgId].eDynamicMode == FBM_DYNAMIC_DIFF)
    {
        _FBM_PoolRelease(_prFbg[ucFbgId].u4FbMemoryPool2);
    }
    //_prFbg[ucFbgId].u4FbMemoryPool2 = 0;
    _prFbg[ucFbgId].eDynamicMode = FBM_DYNAMIC_MODE_NONE;

    // clear conlifct list & cnt
    x_memset(_prFbg[ucFbgId].auc2FBsConflictListCnt, 0, sizeof(_prFbg[ucFbgId].auc2FBsConflictListCnt));
    x_memset(_prFbg[ucFbgId].auc2FBsConflictList, 0xFF, sizeof(_prFbg[ucFbgId].auc2FBsConflictList));

    //Clear FB addr?
    //Clear FBNsOld ?
    
    //x_memset((UCHAR *)_prFbg[ucFbgId].aucFbStatus + _prFbg[ucFbgId].ucFbNsOldBase, FBM_FB_STATUS_UNKNOWN, FBM_MAX_FB_NS_PER_GROUP/2);
    _prFbg[ucFbgId].ucFbNsOld = 0xFB;
    _prFbg[ucFbgId].ucFbNsOldBase = 0xFB;
    //release pool.

    return TRUE;
}
//-------------------------------------------------------------------------
/** _FBM_PutFrameBufferToEmptyQ
 *  Put FB to empty queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

VOID _FBM_Print2FBsConflictList(UCHAR ucFbgId)
{
    UCHAR  ucFbId = 0;
    UCHAR  *paucCftLst = NULL;
    
    for(ucFbId = 0; ucFbId < FBM_MAX_FB_NS_PER_GROUP; ucFbId++)
    {
        paucCftLst = _prFbg[ucFbgId].auc2FBsConflictList[ucFbId];
        LOG(2, "FB[%d] Cnt(%d): %d %d %d %d.\n", ucFbId, _prFbg[ucFbgId].auc2FBsConflictListCnt[ucFbId], 
            paucCftLst[0], paucCftLst[1], paucCftLst[2], paucCftLst[3]);
    }
}
//-------------------------------------------------------------------------
/** _FBM_PutFrameBufferToEmptyQ
 *  Put FB to empty queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

VOID _FBM_2FBsConflictList(UCHAR ucFbgId)
{
    UCHAR   ucNewFbId = _prFbg[ucFbgId].ucFbNsBase;
    UCHAR   ucOldFbId = _prFbg[ucFbgId].ucFbNsOldBase;
    // Size = Y(N+1) -Y(N)..  Fix me. 
    UINT32  u4NewFBSize = _prFbg[ucFbgId].au4AddrY[ucNewFbId+1] - _prFbg[ucFbgId].au4AddrY[ucNewFbId];
    UINT32  u4OldFBSize = _prFbg[ucFbgId].au4AddrY[ucOldFbId+1] - _prFbg[ucFbgId].au4AddrY[ucOldFbId];
    UCHAR  *pau4CftCnt = _prFbg[ucFbgId].auc2FBsConflictListCnt;
    
    #define au4NewFbStart(FbIdx)   (_prFbg[ucFbgId].au4AddrY[FbIdx])
    #define au4OldFbStart(FbIdx)   (_prFbg[ucFbgId].au4AddrY[FbIdx])
    #define au4NewFbEnd(FbIdx)     (au4NewFbStart(FbIdx) + u4NewFBSize)
    #define au4OldFbEnd(FbIdx)     (au4OldFbStart(FbIdx) + u4OldFBSize)
    
    x_memset(_prFbg[ucFbgId].auc2FBsConflictListCnt, 0, sizeof(_prFbg[ucFbgId].auc2FBsConflictListCnt));
    x_memset(_prFbg[ucFbgId].auc2FBsConflictList, 0xFF, sizeof(_prFbg[ucFbgId].auc2FBsConflictList));

    while(IS_NEW_FBS(ucFbgId, ucNewFbId) && IS_OLD_FBS(ucFbgId, ucOldFbId))
    {
        if((au4NewFbStart(ucNewFbId) <= au4OldFbStart(ucOldFbId)) && (au4NewFbEnd(ucNewFbId) > au4OldFbStart(ucOldFbId)))
        {
            _prFbg[ucFbgId].auc2FBsConflictList[ucNewFbId][pau4CftCnt[ucNewFbId]] = ucOldFbId;
            pau4CftCnt[ucNewFbId]++;
            _prFbg[ucFbgId].auc2FBsConflictList[ucOldFbId][pau4CftCnt[ucOldFbId]] = ucNewFbId;
            pau4CftCnt[ucOldFbId]++;
        }
        else
        if((au4NewFbStart(ucNewFbId) >= au4OldFbStart(ucOldFbId)) && (au4NewFbStart(ucNewFbId) < au4OldFbEnd(ucOldFbId)))
        {
            _prFbg[ucFbgId].auc2FBsConflictList[ucNewFbId][pau4CftCnt[ucNewFbId]] = ucOldFbId;
            pau4CftCnt[ucNewFbId]++;
            _prFbg[ucFbgId].auc2FBsConflictList[ucOldFbId][pau4CftCnt[ucOldFbId]] = ucNewFbId;
            pau4CftCnt[ucOldFbId]++;
        }

        if(au4NewFbEnd(ucNewFbId) > au4OldFbEnd(ucOldFbId))
        {
            ucOldFbId++;
        }
        else 
        {
            ucNewFbId++;
        }
    }
    _FBM_Print2FBsConflictList(ucFbgId);
}


//-------------------------------------------------------------------------
/** _FBM_PutFrameBufferToEmptyQ
 *  Put FB to empty queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 
 */
//-------------------------------------------------------------------------
       

static BOOL _FBM_PrePutFBToEmptyQ(UCHAR ucFbgId, UCHAR ucFbId)
{
    UCHAR  ucListIdx = 0;
    BOOL   bFBCfting = FALSE;
    UCHAR  *paucCftLst = NULL;
    paucCftLst = _prFbg[ucFbgId].auc2FBsConflictList[ucFbId];
    
    if (_prFbg[ucFbgId].eDynamicMode == FBM_DYNAMIC_SAME)
    {
        if(IS_NEW_FBS(ucFbgId, ucFbId))// New FBs put empty Q should check Old conflict FBs is still using?
        {
            bFBCfting = FALSE;
            for(ucListIdx = 0; ucListIdx < _prFbg[ucFbgId].auc2FBsConflictListCnt[ucFbId]; ucListIdx++)
            {
                //While FB is EMPTY or LOCK, the memory is can be use now, as displaying data is get from Resize FB.
                if(!(FBM_CheckFrameBufferStatus(ucFbgId, paucCftLst[ucListIdx], FBM_FB_STATUS_EMPTY)
                    /*|| FBM_CheckFrameBufferStatus(ucFbgId, paucCftLst[ucListIdx], FBM_FB_STATUS_LOCK)*/))
                {
                    bFBCfting = TRUE;
                    break;
                }
            }
        }
        else if(IS_OLD_FBS(ucFbgId, ucFbId))// old FBs set empty should try put new conflict FBs into empty Q
        {
            _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_EMPTY;
            for(ucListIdx = 0; ucListIdx < _prFbg[ucFbgId].auc2FBsConflictListCnt[ucFbId]; ucListIdx++)
            {
                if(FBM_CheckFrameBufferStatus(ucFbgId, paucCftLst[ucListIdx], FBM_FB_STATUS_UNKNOWN))
                {
                    _FBM_PutFrameBufferToEmptyQ(ucFbgId, paucCftLst[ucListIdx]);
                }
            }
            bFBCfting = FALSE; // MASTER FB shoule always can be put empty.
        }
        else
        {
            ASSERT(IS_NEW_FBS(ucFbgId, ucFbId) || IS_OLD_FBS(ucFbgId, ucFbId));
        }
    }
    else if (_prFbg[ucFbgId].eDynamicMode == FBM_DYNAMIC_DIFF)
    {
        bFBCfting = FALSE;
    }
    else if(_prFbg[ucFbgId].eDynamicMode == FBM_DYNAMIC_MODE_NONE)
    {
        bFBCfting = FALSE;
    }
    
    return bFBCfting;
}

//-------------------------------------------------------------------------
/** _FBM_PutFrameBufferToEmptyQ
 *  Put FB to empty queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void _FBM_PutFrameBufferToEmptyQ(UCHAR ucFbgId, UCHAR ucFbId)
{
    UCHAR ucNextWriteIdx;
    CRIT_STATE_T _rEmptyQState;    
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }
    
    if(_FBM_PrePutFBToEmptyQ(ucFbgId, ucFbId))
    {
        LOG(1, "FB(%d, %d) Put to EmptyQ Fail, as conflict.\n", ucFbgId, ucFbId);
        return;
    }
    
    _rEmptyQState = x_crit_start();    
    
    if(IS_OLD_FBS(ucFbgId, ucFbId))
    {
        if ((_prFbg[ucFbgId].ucFbOldEmptyCnt + 1) > (_prFbg[ucFbgId].ucFbNsOld - _prFbg[ucFbgId].ucFbNsOldBase))
        {
            LOG(0,"ucFbgId(%d) ucFbOldEmptyCnt(%d) ucFbNsOld(%d) ucFbNsOldBase(%d)\n",
                ucFbgId,
                _prFbg[ucFbgId].ucFbOldEmptyCnt,
                _prFbg[ucFbgId].ucFbNsOld,
                _prFbg[ucFbgId].ucFbNsOldBase);
        }
        ASSERT_FBM(_prFbg[ucFbgId].ucFbOldEmptyCnt + 1 <= _prFbg[ucFbgId].ucFbNsOld - _prFbg[ucFbgId].ucFbNsOldBase);
        //dump_stack();
    }
    if((_prFbg[ucFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE) && IS_OLD_FBS(ucFbgId, ucFbId))
    {
        //not add the FB to emptyQ
        _prFbg[ucFbgId].ucFbOldEmptyCnt++;
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_EMPTY;
        
        LOG(3, "[%s-%d]Fb(%d) EmptyCnt(%d) < Total(%d) ignore.\n", __func__, __LINE__, ucFbId, _prFbg[ucFbgId].ucFbOldEmptyCnt,
            _prFbg[ucFbgId].ucFbNsOld - _prFbg[ucFbgId].ucFbNsOldBase);
        _FBM_2FBsChkDynamicDone(ucFbgId);
        x_crit_end(_rEmptyQState); 
        return;
    }

    if (_prFbg[ucFbgId].rEmptyQ.ucCount >= (_prFbg[ucFbgId].ucFbNs - _prFbg[ucFbgId].ucFbNsBase))
    {
        if (_prFbg[ucFbgId].u4FbgFlag & (FBM_FLAG_RESET | FBM_FLAG_CLEAR_FRC_PRM))
        {
            x_crit_end(_rEmptyQState);        
            return;
        }
        else if (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY)
        {
            LOG(1, "[Warning] Put to EmptyQ Again (%d, %d)\n", ucFbgId, ucFbId);
            x_crit_end(_rEmptyQState);                    
            return;
        }
        else if (_prFbg[ucFbgId].ucFbNs > _prFbg[ucFbgId].ucFbNsBase) //Multi thread, maybe vdec is re-create fbg now.
        {
            LOG(1, "Assert: rEmptyQCnt(%d) > FBNs(%d-%d).\n", 
                _prFbg[ucFbgId].rEmptyQ.ucCount, 
                _prFbg[ucFbgId].ucFbNs, _prFbg[ucFbgId].ucFbNsBase);
                
            if (_prFbg[ucFbgId].rEmptyQ.ucCount >= (_prFbg[ucFbgId].ucFbNs - _prFbg[ucFbgId].ucFbNsBase))
            {
                LOG(0,"ucFbgId(%d) EmptyQCnt(%d) ucFbNs(%d) ucFbNsBase(%d)\n",
                    ucFbgId,
                    _prFbg[ucFbgId].rEmptyQ.ucCount,
                    _prFbg[ucFbgId].ucFbNs,
                    _prFbg[ucFbgId].ucFbNsBase);
            }
            ASSERT_FBM(_prFbg[ucFbgId].rEmptyQ.ucCount < (_prFbg[ucFbgId].ucFbNs - _prFbg[ucFbgId].ucFbNsBase));
        }
        else
        {
            x_crit_end(_rEmptyQState);        
            return;
        }
    }
    
    _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_EMPTY;
    
    if(_prFbg[ucFbgId].aucFbRotationStatus[ucFbId] == FB_ROTATION_USE 
        || _prFbg[ucFbgId].aucFbRotationStatus[ucFbId] == FB_ROTATION_WAIT_USE)
    {
        ucNextWriteIdx = (_prFbg[ucFbgId].rEmptyQ.ucReadIdx + _prFbg[ucFbgId].rEmptyQ.ucCount) % FBM_MAX_FB_NS_PER_GROUP;
        _prFbg[ucFbgId].rEmptyQ.aucQueue[ucNextWriteIdx] = ucFbId;
        _prFbg[ucFbgId].rEmptyQ.ucCount++;
        // add by liuqu,20121127, for seq chg flow, a new fb may be put inot emptyq!
#ifdef NEW_SEQ_CHG_NOTIFY
            if ((ucFbId !=FBM_FB_ID_UNKNOWN) &&
                    ((_prFbg[ucFbgId].ucSeqChg == 1) && (u4FirstFBStored == 0))) // new condition!
            {
                UINT8 u1ReadIdx  = rSeqChgEmptyQ.ucReadIdx;
                UINT8 i =0;
                UINT8 ucGetFbId = 0xff;
        
                for (i=0; i<rSeqChgEmptyQ.ucCount; i++)
                {
                    if (rSeqChgEmptyQ.ucReadIdx >= FBM_MAX_FB_NS_PER_GROUP)
                    {
                        LOG(0,"ucReadIdx %d\n",rSeqChgEmptyQ.ucReadIdx);
                        
                        ASSERT_FBM(rSeqChgEmptyQ.ucReadIdx < FBM_MAX_FB_NS_PER_GROUP);
                    }
        
                    ucGetFbId = rSeqChgEmptyQ.aucQueue[u1ReadIdx];
        
                    if (ucGetFbId == ucFbId )
                        break ; // the FbId is already in the rSeqChgEmptyQ!
        
                    if (++(u1ReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
                    {
                        u1ReadIdx = 0;
                    }
                }
        
                if (i == rSeqChgEmptyQ.ucCount)  // the ucFbId is not in the rSeqChgEmptyQ,add into it!
                {     
                    u1ReadIdx  = rSeqChgEmptyQ.ucReadIdx;
                    for (i = 0; i< rSeqChgEmptyQ.ucCount;i++)
                    {
                        if (++(u1ReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
                        {
                            u1ReadIdx = 0;
                        }
                    }
        
                     rSeqChgEmptyQ.ucCount++;  // add 1 more !
                    rSeqChgEmptyQ.aucQueue[u1ReadIdx] = ucFbId ;
                   LOG(5,"[LEO]Tag 13, rSeqChgEmptyQ changed!FB %d is added!\n",ucFbId);
                }      
            }
#endif
           LOG(4,"EmpytInQ fb(%d,%d) Cnt(%d,%d)\n",ucFbgId,ucFbId,
           _prFbg[ucFbgId].rEmptyQ.ucCount,_prFbg[ucFbgId].hEmptyQSemaphore.i4Count+1);

           VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);

    }
    
    x_crit_end(_rEmptyQState);
    
    if(_prFbg[ucFbgId].aucFbRotationStatus[ucFbId] == FB_ROTATION_WAIT_UNUSE)
    {
        LOG(2,"_FBM_PutFrameBufferToEmptyQ Fb(%d,%d) change to unused \n",ucFbgId,ucFbId);
        _prFbg[ucFbgId].aucFbRotationStatus[ucFbId] = FB_ROTATION_UNUSE;
        _FbmSyncWaitUseFb(ucFbgId);
    }

}


//-------------------------------------------------------------------------
/** _FBM_PutFrameBufferToEmptyResizeQ
 *  Put FB to empty queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void _FBM_PutFrameBufferToEmptyResizeQ(UCHAR ucFbgId, UCHAR ucRzFbId)
{
    UCHAR ucNextWriteIdx;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_RESIZE_FB(ucFbgId, ucRzFbId)))
    {
        return;
    }
    if (_prFbg[ucFbgId].rResizeEmptyQ.ucCount >= _prFbg[ucFbgId].ucSeamlessFbNs)  //done 0/0/1
    {
        if (_prFbg[ucFbgId].aucResizeFbStatus[ucRzFbId] == FBM_FB_STATUS_EMPTY)
        {
            LOG(1, "[Warning] Put to EmptyResizeQ Again (%d, %d)\n", ucFbgId, ucRzFbId);
            return;
        }
        else
        {
            if (_prFbg[ucFbgId].rResizeEmptyQ.ucCount >= _prFbg[ucFbgId].ucSeamlessFbNs)
            {
                LOG(0,"ucFbgId(%d) RzEmptyQCnt(%d) ucSeamlessFbNs(%d)\n",
                    ucFbgId,
                    _prFbg[ucFbgId].rResizeEmptyQ.ucCount,
                    _prFbg[ucFbgId].ucSeamlessFbNs);
            }
            ASSERT_FBM(_prFbg[ucFbgId].rResizeEmptyQ.ucCount < _prFbg[ucFbgId].ucSeamlessFbNs);
        }
    }
    if (_prFbg[ucFbgId].aucResizeBlockFb != FBM_FB_ID_UNKNOWN)
    {
        //Direct match the seamless RZ FB and resize block FB, for b2r change frame successful.
        //Or B2R need change frame by fb ready callback again.
        _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResizeBlockFb].ucSeamlessResizeFbid = ucRzFbId;
        _prFbg[ucFbgId].aucResizeBlockFb = FBM_FB_ID_UNKNOWN;      
    }

    _prFbg[ucFbgId].aucResizeFbStatus[ucRzFbId] = FBM_FB_STATUS_EMPTY;
    _prFbg[ucFbgId].aucResizeSrcFbId[ucRzFbId] = FBM_FB_ID_UNKNOWN;
    _prFbg[ucFbgId].aucResizeTaskId[ucRzFbId] = 0xFF;

    ucNextWriteIdx = (_prFbg[ucFbgId].rResizeEmptyQ.ucReadIdx + _prFbg[ucFbgId].rResizeEmptyQ.ucCount) % FBM_MAX_FB_NS_PER_GROUP;
    _prFbg[ucFbgId].rResizeEmptyQ.aucQueue[ucNextWriteIdx] = ucRzFbId;
    _prFbg[ucFbgId].rResizeEmptyQ.ucCount++;
}
//-------------------------------------------------------------------------
/** _FBM_PutFrameBufferToEmptyResize2Q
 *  Put FB to empty queue
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

void _FBM_PutFrameBufferToEmptyResize2Q(UCHAR ucFbgId, UCHAR ucRzFbId)
{
    UCHAR ucNextWriteIdx;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_RESIZE_FB(ucFbgId, ucRzFbId)))
    {
        return;
    }

    if (_prFbg[ucFbgId].rResize2EmptyQ.ucCount >= _prFbg[ucFbgId].ucSeamless2FbNs)
    {
        if (_prFbg[ucFbgId].aucResize2FbStatus[ucRzFbId] == FBM_FB_STATUS_EMPTY)
        {
            LOG(1, "[Warning] Put to EmptyResizeQ Again (%d, %d)\n", ucFbgId, ucRzFbId);
            return;
        }
        else
        {
            if (_prFbg[ucFbgId].rResize2EmptyQ.ucCount >= _prFbg[ucFbgId].ucSeamless2FbNs)
            {
                LOG(0,"ucFbgId(%d) Rz2EmptyQCnt(%d) Seamless2FbNs(%d)\n",
                    ucFbgId,
                    _prFbg[ucFbgId].rResize2EmptyQ.ucCount,
                    _prFbg[ucFbgId].ucSeamless2FbNs);
                    
            }
            ASSERT_FBM(_prFbg[ucFbgId].rResize2EmptyQ.ucCount < _prFbg[ucFbgId].ucSeamless2FbNs);
        }
    }
    if (_prFbg[ucFbgId].aucResize2BlockFb != FBM_FB_ID_UNKNOWN)
    {
        //Direct match the seamless RZ FB and resize block FB, for b2r change frame successful.
        //Or B2R need change frame by fb ready callback again.
        _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResize2BlockFb].ucSeamlessResize2Fbid = ucRzFbId;
        _prFbg[ucFbgId].aucResize2BlockFb = FBM_FB_ID_UNKNOWN;
    }

    _prFbg[ucFbgId].aucResize2FbStatus[ucRzFbId] = FBM_FB_STATUS_EMPTY;
    _prFbg[ucFbgId].aucResize2SrcFbId[ucRzFbId] = FBM_FB_ID_UNKNOWN;
    _prFbg[ucFbgId].aucResize2TaskId[ucRzFbId] = 0xFF;

    ucNextWriteIdx = (_prFbg[ucFbgId].rResize2EmptyQ.ucReadIdx + _prFbg[ucFbgId].rResize2EmptyQ.ucCount) % FBM_MAX_FB_NS_PER_GROUP;
    _prFbg[ucFbgId].rResize2EmptyQ.aucQueue[ucNextWriteIdx] = ucRzFbId;
    _prFbg[ucFbgId].rResize2EmptyQ.ucCount++;
	
}


//-------------------------------------------------------------------------
/** FBM_GetEmptyDelayTime
 *  If FBG without B2R mode, auto get empty from dispQ.
 *  @param  ucFbgId   FBG index.
 *  @param  pu4Delay   time to wait.
 *  @return 1/0 OK/Fail.
 */
//-------------------------------------------------------------------------

static UINT32 FBM_GetEmptyDelayTime(UCHAR ucFbgId, UINT32 *pu4Delay)
{
#ifdef ENABLE_MULTIMEDIA
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    prVdecEsInfo = _prFbg[ucFbgId].u1DecoderSrcId < VDEC_MAX_ES ? _VDEC_GetEsInfo(_prFbg[ucFbgId].u1DecoderSrcId) : NULL;
#endif
    
    if (VERIFY_FBG(ucFbgId) || !pu4Delay)
    {
        return 0;
    }
    
    if ((_prFbg[ucFbgId].ucPlayMode == FBM_FBG_DTV_MODE)
#ifdef ENABLE_MULTIMEDIA
            ||
            ((_prFbg[ucFbgId].ucPlayMode == FBM_FBG_MM_MODE) && prVdecEsInfo &&
             (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            )
#endif
       )
    {
        if (!FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_DISPLAYING))
        {
            *pu4Delay = FBM_WAIT_FB_TIMEOUT;
        }
        else if (_prFbg[ucFbgId].u4AvSyncStatus == FBM_FBG_AVSYNC_REPEAT)
        {
            /* During Frame repeat, wait FB for a longer time */
            *pu4Delay = FBM_WAIT_FB_TIMEOUT;
            LOG(3, "Start to play but frame repeat\n");
        }
    }

    if (FBM_ChkFrameBufferGlobalFlagIsr(ucFbgId, FBM_FLAG_FB_NO_TIMEOUT))
    {
        *pu4Delay = 0xfffff;
    }
    
    if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE))
    {
        *pu4Delay = 0;
    }
    else if(_prFbg[ucFbgId].ucPlayMode == FBM_FBG_MM_MODE)
    {
        *pu4Delay = 0xfffff;
    }
    else if(_prFbg[ucFbgId].ucPlayMode == FBM_FBG_DTV_MODE)
    {
        if(*pu4Delay < 50) *pu4Delay = 50;
    }

    if(FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_GETEMPTY_NOWAIT))
    {
        *pu4Delay = 1;
    }
    
    if(FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_AUTO_RENDER))
    {
        switch (_prFbg[ucFbgId].rSeqHdr.ucFrmRatCod)
        {
            case MPEG_FRAME_RATE_12:
                *pu4Delay = 90000/FBM_FRAME_RATE_12;
                break;
            case MPEG_FRAME_RATE_15:
                *pu4Delay = 90000/FBM_FRAME_RATE_15;
                break;
            case MPEG_FRAME_RATE_20:
                *pu4Delay = 90000/FBM_FRAME_RATE_20;
                break;
            case MPEG_FRAME_RATE_24_:
            case MPEG_FRAME_RATE_24:
                *pu4Delay = 90000/FBM_FRAME_RATE_24;
                break;
            case MPEG_FRAME_RATE_25:
                *pu4Delay = 90000/FBM_FRAME_RATE_25;
                break;
            case MPEG_FRAME_RATE_30_:
            case MPEG_FRAME_RATE_30:
                *pu4Delay = 90000/FBM_FRAME_RATE_30;
                break;
            case MPEG_FRAME_RATE_50:
                *pu4Delay = 90000/FBM_FRAME_RATE_50;
                break;
            case MPEG_FRAME_RATE_60_:
            case MPEG_FRAME_RATE_60:
                *pu4Delay = 90000/FBM_FRAME_RATE_60;
                break;
            case MPEG_FRAME_RATE_UNKNOWN:
            default:
                LOG(1, "Unknown Frame Rate %d\n");
                *pu4Delay = 90000/FBM_FRAME_RATE_60;
                break;
        }
    }
    
    return 0; //ms
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** FBM_SetFrameBufferFlag
 *  Set FBG flag
 *  @param  ucFbgId   FBG index.
 *  @param  u4Flag   flag.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

void FBM_SetFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag)
{

    BOOL fgGotFirstSeqChg;
    
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
   
   fgGotFirstSeqChg = _prFbg[ucFbgId].fgGotFirstSeqChg;

    FBM_MUTEX_LOCK(ucFbgId);
    if (u4Flag == FBM_FLAG_SEQ_CHG || u4Flag == FBM_FLAG_SEQ_CHG_SPEEDUP)
    {
             u4Flag = FBM_FLAG_SEQ_CHG_SPEEDUP;//for LG only
            _prFbg[ucFbgId].fgGotFirstSeqChg = TRUE;
#ifdef TIME_MEASUREMENT
              TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "Vdec_SEQ_CHG");
              TMS_DIFF_EX(TMS_FLAG_BOOT, TMS_COOL_BOOT_TIME, "Vdec_SEQ_CHG");
#endif
        if (u4Flag == FBM_FLAG_SEQ_CHG_SPEEDUP && fgGotFirstSeqChg == FALSE)
        {
            _prFbg[ucFbgId].u4FbgFlag |= u4Flag;
        }
        else
        {            
            _prFbg[ucFbgId].ucRecordNextFbId = 1;
            _prFbg[ucFbgId].u4SeqChgFlag = u4Flag;
            LOG(5,"[LEO]Seq chg received!\n");
        }

		VDP_PipeStartSeqChange(ucFbgId);
#ifndef CC_VDEC_RM_SUPPORT
#ifdef CC_B2R_RM_SUPPORT

    if(!fgGotFirstSeqChg)
    {
            UCHAR ucB2rId = B2R_NULL;
             B2R_RM_T input_Src;
            VDEC_ES_INFO_T *prVdecEsInfo = NULL;
            VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;
             
            if(_prFbg[ucFbgId].u1FbgAppMode != FBM_FBG_APP_MTIMAGE)
            {
                prVdecEsInfo= _VDEC_GetEsInfo(_prFbg[ucFbgId].u1DecoderSrcId);
                prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(_prFbg[ucFbgId].u1DecoderSrcId);
                input_Src.eVdecType = prVdecEsInfoKeep->eCurFMT;// prVdecEsInfoKeep->eCurFMT;
            }
            else
            {
                input_Src.eVdecType = VDEC_FMT_JPG; // prVdecEsInfoKeep->eCurFMT;
            }

            input_Src.u2HActive = _prFbg[ucFbgId].rSeqHdr.u2HSize;//prVdecEsInfo->u2OrgHSize;
            input_Src.u2VActive =_prFbg[ucFbgId].rSeqHdr.u2VSize; //prVdecEsInfo->u2OrgVSize;
            input_Src.u2Fps = 30;//prVdecEsInfo->u4FrameRate/100;
            input_Src.fgUFOEnable =_prFbg[ucFbgId].rSeqHdr.fgUfoEn;//prVdecEsInfo->fgEnableUFO;
            input_Src.fg8Bit =!(_prFbg[ucFbgId].rSeqHdr.u1Depth==10); //(prVdecEsInfo->u4BitDepth == 8);
            input_Src.u2BlockMode = !(_prFbg[ucFbgId].rSeqHdr.fgRasterOrder);
            input_Src.ucEsId = _prFbg[ucFbgId].u1DecoderSrcId;

            FBM_MUTEX_UNLOCK(ucFbgId);

            if(_prFbg[ucFbgId].u1FbgAppMode == FBM_FBG_APP_MTIMAGE)
            {
                VDP_SetB2RId(B2R_NULL, VDP_1, _prFbg[ucFbgId].u1DecoderSrcId);
                ucB2rId = B2R_Source_Connect(input_Src);
            }
            else if(!FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE))
            {
                if (prVdecEsInfo && prVdecEsInfo->u4RenderVDPId != 0xFF)
                {
                    VDP_SetB2RId(B2R_NULL, prVdecEsInfo->u4RenderVDPId, _prFbg[ucFbgId].u1DecoderSrcId);
                }
               
                ucB2rId = B2R_Source_Connect(input_Src);
                LOG(1,"cB2rId =%d,prVdecEsInfoKeep=%d,u1DecoderSrcId=%d,rSeqHdr.u1Depth=%d\n",ucB2rId,prVdecEsInfoKeep,_prFbg[ucFbgId].u1DecoderSrcId,_prFbg[ucFbgId].rSeqHdr.u1Depth);
                if (prVdecEsInfoKeep && prVdecEsInfoKeep->pfnVdecSetParam && (ucB2rId >= B2R_3) && (ucB2rId < B2R_NULL))
                {
                    prVdecEsInfoKeep->pfnVdecSetParam(_prFbg[ucFbgId].u1DecoderSrcId, (UINT32)VDEC_BLK_MODE, 0, 0, 0);
                }
            }
            FBM_MUTEX_LOCK(ucFbgId);
    }
#endif
#endif
#ifndef CC_SUPPORT_NPTV_SEAMLESS 
        if (FBM_ChkSeamlessMode(ucFbgId, SEAMLESS_BY_NPTV) && fgGotFirstSeqChg)
        {
            if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_SEQ_CHG_PRE_CB],
                                         _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_SEQ_CHG_PRE_CB]))
            {
                ((FBM_FB_SEQ_CHG_PRE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_SEQ_CHG_PRE_CB])(ucFbgId);
            }
        }
#endif
        
        FBM_MUTEX_UNLOCK(ucFbgId);

        return;
    }
    _prFbg[ucFbgId].u4FbgFlag |= u4Flag;

    FBM_MUTEX_UNLOCK(ucFbgId);
}

//-------------------------------------------------------------------------
/** FBM_ChkFrameBufferFlag
 *  Check FBG flag
 *  @param  ucFbgId   FBG index.
 *  @param  u4Flag   flag.
 *  @return 1/0 for valid/invalid.
 */
//-------------------------------------------------------------------------
UCHAR FBM_ChkSeamlessMode(UCHAR ucFbgId, UINT32 u4Mode)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    return (_prFbg[ucFbgId].eSeamlessMode & u4Mode) ? 1 : 0;
}

//-------------------------------------------------------------------------
/** FBM_ChkFrameBufferFlag
 *  Check FBG flag
 *  @param  ucFbgId   FBG index.
 *  @param  u4Flag   flag.
 *  @return 1/0 for valid/invalid.
 */
//-------------------------------------------------------------------------
UCHAR FBM_ChkFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag)
{
    UCHAR ucChk;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if (_prFbg[ucFbgId].u4FbgFlag & u4Flag)
    {
        ucChk = 1;
    }
    else
    {
        ucChk = 0;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucChk;
}


//-------------------------------------------------------------------------
/** FBM_SetFrameBufferPicFlag
 *  Set FB Picture flag
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  u4Flag   flag.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    _prFbg[ucFbgId].prPicHdr[ucFbId].u4MmFlag |= u4Flag;
    if ((FBM_MM_EOS_FLAG == u4Flag) || (FBM_MM_PSEUDO_EOS_FLAG == u4Flag))
    {
        _prFbg[ucFbgId].fgSendEOS = TRUE;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_ChkFrameBufferPicFlag
 *  Check FBG flag
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  u4Flag   flag.
 *  @return 1/0 for valid/invalid.
 */
//-------------------------------------------------------------------------
UCHAR FBM_ChkFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag)
{
    UCHAR ucChk;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);


    if (_prFbg[ucFbgId].prPicHdr &&
        _prFbg[ucFbgId].prPicHdr[ucFbId].u4MmFlag & u4Flag)
    {
        ucChk = 1;
    }
    else
    {
        ucChk = 0;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucChk;
}


//-------------------------------------------------------------------------
/** FBM_ClrFrameBufferPicFlag
 *  Clear FBG flag
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  u4Flag   flag.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ClrFrameBufferPicFlag(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4Flag)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    _prFbg[ucFbgId].prPicHdr[ucFbId].u4MmFlag &= ~u4Flag;

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_ChkFrameBufferFlagIsr
 *  Check FBG flag at ISR
 *  @param  ucFbgId   FBG index.
 *  @param  u4Flag   flag.
 *  @return 1/0 for valid/invalid.
 */
//-------------------------------------------------------------------------
UCHAR FBM_ChkFrameBufferFlagIsr(UCHAR ucFbgId, UINT32 u4Flag)
{
    UCHAR ucChk;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    if (_prFbg[ucFbgId].u4FbgFlag & u4Flag)
    {
        ucChk = 1;
    }
    else
    {
        ucChk = 0;
    }

    return ucChk;
}


//-------------------------------------------------------------------------
/** FBM_ClrFrameBufferFlag
 *  Clear FBG flag
 *  @param  ucFbgId   FBG index.
 *  @param  u4Flag   flag.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ClrFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    _prFbg[ucFbgId].u4FbgFlag &= ~u4Flag;

    if ((u4Flag == FBM_FLAG_DEINTERLACE) &&
            (_prFbg[ucFbgId].u4ResidualFb == 1))
    {
        _prFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_RESET;
        _prFbg[ucFbgId].u4ResidualFree = 1;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferNs
 *  Get FBG frame buffer number
 *  @param  ucFbgId   FBG index.
 *  @return frame buffer number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFrameBufferNs(UCHAR ucFbgId)
{
    UCHAR ucFbNs;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);
    ucFbNs = _prFbg[ucFbgId].u4FbCnt;
    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbNs;
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferSize
 *  Get FBG frame buffer size
 *  @param  ucFbgId   FBG index.
 *  @param  pu4Width   width pointer.
 *  @param  pu4Height   height pointer.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetFrameBufferSize(UCHAR ucFbgId, UINT32 *pu4Width, UINT32 *pu4Height)
{
    if ((pu4Width == NULL) || (pu4Height == NULL))
    {
        return;
    }

    *pu4Width = (UINT32)NULL;
    *pu4Height = (UINT32)NULL;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    *pu4Width = _prFbg[ucFbgId].u4FbWidth;
    *pu4Height = _prFbg[ucFbgId].u4FbHeight;
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferStatus
 *  Get FBG frame buffer status
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return frame buffer status.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId)
{
    UCHAR ucFbStatus;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return FBM_FB_STATUS_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucFbStatus = _prFbg[ucFbgId].aucFbStatus[ucFbId];

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbStatus;
}


//-------------------------------------------------------------------------
/** FBM_CheckFrameBufferStatus
 *  Check FBG frame buffer status
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  ucFbStatus   FB status.
 *  @return 1/0 for valid/invalid.
 */
//-------------------------------------------------------------------------
UCHAR FBM_CheckFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus)
{
    UCHAR ucValid;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucValid = (ucFbStatus == _prFbg[ucFbgId].aucFbStatus[ucFbId]);

    // VDP protection
    if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_NOT_READY)
    {
        ucValid = 1;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucValid;
}

//-------------------------------------------------------------------------
/** FBM_SetFrameBufferStatus
 *  Set FBG frame buffer status
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  ucFbStatus   frame buffer status.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

#ifdef FBM_FB_LOG

#if 0
#ifdef FBM_SetFrameBufferStatus
//#undef FBM_SetFrameBufferStatus
#endif
void FBM_SetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus)
{
   FBM_SetFrameBufferStatus_dbg(ucFbgId, ucFbId, ucFbStatus, __func__, __LINE__); 
}
#endif

void FBM_SetFrameBufferStatus_dbg(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus, const CHAR* sFunc, const UINT32 u4Line)
#else
void FBM_SetFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus)
#endif
{

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    // [LOG] FBM change FB status
    SYSLOG(FBM_PREFIX + ucFbgId, ucFbId, _prFbg[ucFbgId].aucFbStatus[ucFbId], ucFbStatus);
    _FbmFbLog(ucFbgId,ucFbId, ucFbStatus, sFunc, u4Line);

//    LOG(7, "Fbg(%d) Fb(%d) %d >> %d\n", ucFbgId, ucFbId, _prFbg[ucFbgId].aucFbStatus[ucFbId], ucFbStatus);
//    LOG(7, "Fb(%d, %x) %d >> %d\n", ucFbId, _prFbg[ucFbgId].au4AddrY[ucFbId], _prFbg[ucFbgId].aucFbStatus[ucFbId], ucFbStatus);

    // All FB go to EMPTY status at NOT READY mode
    if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_NOT_READY)
    {
        ucFbStatus = FBM_FB_STATUS_EMPTY;
    }

    if (ucFbId == _prFbg[ucFbgId].ucFbDecode)
    {
        // check current decode frame buffer status change
        if ((ucFbStatus != FBM_FB_STATUS_DECODE) &&
                (ucFbStatus != FBM_FB_STATUS_FIELD_READY))
        {
            _prFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
        }

        // check back reference frame buffer status change
        if (ucFbId == _prFbg[ucFbgId].ucFbBRef)
        {
            if ((ucFbStatus == FBM_FB_STATUS_READY) ||
                    (ucFbStatus == FBM_FB_STATUS_FIELD_READY))
            {
                _prFbg[ucFbgId].ucFbBRefValid = 1;
            }
            else if (ucFbStatus == FBM_FB_STATUS_EMPTY) //if FBM_FB_STATUS_FIELD_READY->FBM_FB_STATUS_EMPTY, set BRef invalid.
            {
                _prFbg[ucFbgId].ucFbBRefValid = 0;
            }
            else
            {
            }
        }
    }

    // If Display NOT ready, don't put FB in DisplayQ (Display Q >> Empty)
    if ((ucFbStatus == FBM_FB_STATUS_DISPLAYQ) &&
            ((_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_DISP_READY) == 0))
    {
        ucFbStatus = FBM_FB_STATUS_EMPTY;
    }

    if(ucFbStatus == FBM_FB_STATUS_DISPLAYQ && _prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK)
    {
       LOG(0,"FBM_SetFrameBufferStatus(Fbg:%d,Fb%d) Status Error ->xxxxxxxxxxxxxxxxx\n",ucFbgId,ucFbId);
    }

#if 1
    //need more test.
    // for new OMX, dropped frame should go back to OMX by display.
    if (_prFbg[ucFbgId].fgCreateFromInst && ucFbStatus == FBM_FB_STATUS_EMPTY && _prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
    {
        ucFbStatus = FBM_FB_STATUS_DISPLAYQ;
        _prFbg[ucFbgId].prPicHdr[ucFbId].fgNotDisplay = TRUE;
        _FbmFbLog(ucFbgId,ucFbId, ucFbStatus, __func__, __LINE__);
    }
#endif
    switch (ucFbStatus)
    {
    case FBM_FB_STATUS_EMPTY:
        if (!FBM_ChkFrameBufferFlag(ucFbgId , FBM_FLAG_FREEZE|FBM_FLAG_RELEASE_FBG|FBM_FLAG_SEEK_MODE))
        {
#ifndef __MODEL_slt__
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ)
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ);
#endif
        }

        _prFbg[ucFbgId].afgWaitDisplay[ucFbId] =  FALSE;
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            _prFbg[ucFbgId].u4DispTag[ucFbId]=INVALID_DISPTAG;
            if (_prFbg[ucFbgId].fgCreateFromInst)
            {
                if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP6) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP9))
                {
                    if(_prFbg[ucFbgId].afgRefList[ucFbId])
                    {
                        break;
                    }
                }
                ucFbStatus = FBM_FB_STATUS_RECYCLE;
                break;
            }
            if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
                (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP6) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP9))
            {
                if (_prFbg[ucFbgId].afgRefList[ucFbId])
                {
                    // the resized frame should be useless now.
                    if (_prFbg[ucFbgId].fgEnableNewSeamless)
                    {
                            
                        if (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN &&
                            _prFbg[ucFbgId].aucResizeFbStatus[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid] != FBM_FB_STATUS_EMPTY)
                        {
                            HAL_GetTime(&_prFbg[ucFbgId].arRlResizeFb[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid]);
                            _FBM_PutFrameBufferToEmptyResizeQ(ucFbgId, _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid);
                            VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResizeQSemaphore) == OSR_OK);
                        }
                        
                        if (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid != FBM_FB_ID_UNKNOWN &&
                            _prFbg[ucFbgId].aucResize2FbStatus[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid] != FBM_FB_STATUS_EMPTY)
                        {
                            //HAL_GetTime(&_prFbg[ucFbgId].arRlResizeFb[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid]);
                            _FBM_PutFrameBufferToEmptyResize2Q(ucFbgId, _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid);
                            VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResize2QSemaphore) == OSR_OK);
                        }                        
						            _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessTaskId = 0xFF;
                    }

                    //don't release FB when it is still in the reference list.
                    break;
                }
                else
                {
                    // release semaphore (Empty)
                    //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
                }
            }
            else
            {
                if (IS_REFERENCE_FB(ucFbgId, ucFbId))
                {   // release reference frame buffer
                }
                else if(_prFbg[ucFbgId].aucFbRotationStatus[ucFbId] == FB_ROTATION_USE ||
                    _prFbg[ucFbgId].aucFbRotationStatus[ucFbId] == FB_ROTATION_WAIT_USE)
                {   // release B frame buffer
                
                    // release semaphore (B & Empty)
                    VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyBQSemaphore) == OSR_OK);
                }

                // release semaphore (Empty)
                //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
            }
            _FBM_PutFrameBufferToEmptyQ(ucFbgId, ucFbId);

            if (_prFbg[ucFbgId].fgEnableNewSeamless)
            {
                if (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN &&
                    _prFbg[ucFbgId].aucResizeFbStatus[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid] != FBM_FB_STATUS_EMPTY)
                {
                    HAL_GetTime(&_prFbg[ucFbgId].arRlResizeFb[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid]);
                    
                    if (_prFbg[ucFbgId].aucResizeFbStatus[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid] == FBM_FB_STATUS_EMPTY)
                    {
                        LOG(0, "Warning! rz fb already cleaned! \n");
                    }
                        
                    _FBM_PutFrameBufferToEmptyResizeQ(ucFbgId, _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid);
                    VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResizeQSemaphore) == OSR_OK);
                }
                else if (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid != FBM_FB_ID_UNKNOWN &&
                    _prFbg[ucFbgId].aucResize2FbStatus[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid] != FBM_FB_STATUS_EMPTY)
                {
                    //HAL_GetTime(&_prFbg[ucFbgId].arRlResizeFb[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid]);
                    
                    if (_prFbg[ucFbgId].aucResize2FbStatus[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid] == FBM_FB_STATUS_EMPTY)
                    {
                        LOG(0, "Warning! rz fb already cleaned! \n");
                    }
                        
                    _FBM_PutFrameBufferToEmptyResize2Q(ucFbgId, _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid);
                    VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResize2QSemaphore) == OSR_OK);
                }                
				_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessTaskId = 0xFF;
            }
        }
        break;

    case FBM_FB_STATUS_DECODE:
        _prFbg[ucFbgId].ucFbDecode = ucFbId;
        break;

    case FBM_FB_STATUS_DISPLAYQ:

        if ((_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DECODE) ||
                (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_FIELD_READY)||
                (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_READY))
        {
            if(_prFbg[ucFbgId].u1FbgAppMode != FBM_FBG_APP_MTIMAGE)
            {
#ifdef CC_53XX_SWDMX_V2
                VDEC_GenPTS(_prFbg[ucFbgId].u1DecoderSrcId, ucFbgId, ucFbId);
#else
                VDEC_GenPTS(ucFbgId, ucFbId);
#endif
            }

            _prFbg[ucFbgId].afgWaitDisplay[ucFbId] =  TRUE;
            _FbmPutFrameBufferToDispQ(ucFbgId, ucFbId);

            if(_prFbg[ucFbgId].ucPlayMode == FBM_FBG_MM_MODE)
            {
                i4VDOOmxRenderFrame(3);
            }
            
            if (_prFbg[ucFbgId].u1FbgAppMode == FBM_FBG_APP_OMX_DISP)
            {
                #ifdef CC_DTV_SUPPORT_LG
                //FBM_TriggerSmlsByResizer(ucFbgId, ucFbId);
                #else
                //stage fright case, do seamless in B2R send render msg.
                //Otherwise, the stage fright drop frame will not update seamless FB to empty in time.
                #endif
            }
            else
            {
                //FBM_TriggerSmlsByResizer(ucFbgId, ucFbId);
            }

        }
        else
        {
            if (_prFbg[ucFbgId].fgCreateFromInst)
            {
                ASSERT_FBM(0);
            }
            if ((_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DECODE) &&
                   (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_FIELD_READY)&&
                   (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_READY))
            {
               LOG(0,"ucFbgId(%d)ucFbId(%d)FbStatus(%d) \n",
                    ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]); 
            }
            ASSERT_FBM((_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DECODE) ||
                   (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_FIELD_READY)||
                   (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_READY));

            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
            {
                if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
                        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
                        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
                        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)||
                        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP6) ||
                        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8) ||
                        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP9))
                {
                    if (_prFbg[ucFbgId].afgRefList[ucFbId])
                    {
                        //don't release FB when it is still in the reference list.
                        break;
                    }
                    else
                    {
                        // release semaphore (Empty)
                        //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
                    }
                }
                else
                {
                    if (IS_REFERENCE_FB(ucFbgId, ucFbId))
                    {   // release reference frame buffer
                    }
                    else
                    {   // release B frame buffer

                        // release semaphore (B & Empty)
                        VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyBQSemaphore) == OSR_OK);
                    }

                    // release semaphore (Empty)
                    //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
                }

                _FBM_PutFrameBufferToEmptyQ(ucFbgId, ucFbId);
            }
        }
        break;

    case FBM_FB_STATUS_READY:
        break;
    case FBM_FB_STATUS_LOCK:
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY)
        {
            ucFbStatus = FBM_FB_STATUS_EMPTY;
        }
        break;
    default:
        break;
    }

    _prFbg[ucFbgId].aucFbStatus[ucFbId] = ucFbStatus;
    
    FBM_MUTEX_UNLOCK(ucFbgId);

    if (ucFbStatus == FBM_FB_STATUS_DISPLAYQ 
        && !(_prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessDisp || _prFbg[ucFbgId].fgAutoSeamless)
        && !FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE) /*Thumbnail mode not call fb ready to b2r.*/)
    {
        FB_CHG_INFO_T t_fb_info;

        t_fb_info.ucFbgId = ucFbgId;
        t_fb_info.ucFbid  = ucFbId;
        if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_FB_READY_IND]))
        {
            ((FBM_FB_READY_IND_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND])(&t_fb_info);
        }
        else
        {
            ASSERT_FBM(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND] == 0);
        }
    }

    if (ucFbStatus == FBM_FB_STATUS_DISPLAYQ )
    {
        if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->aau4CbFunc[ucFbgId][FBM_CB_FUNC_FB_READY_EX_IND],
                                     _prFbmCbFunc->aau4CbFuncCRC[ucFbgId][FBM_CB_FUNC_FB_READY_EX_IND]))
        {
            FBM_PIC_NTF_INFO_T rPicNfyInfo;
            x_memset(&rPicNfyInfo, 0, sizeof(FBM_PIC_NTF_INFO_T));
            rPicNfyInfo.ucFbgId = ucFbgId;
            rPicNfyInfo.ucFbId = ucFbId;
            if(FBM_ChkFrameBufferPicFlag(ucFbgId, ucFbId, (FBM_MM_EOS_FLAG | FBM_MM_PSEUDO_EOS_FLAG)))
            {
                rPicNfyInfo.fgEos = TRUE;
            }
            
            ((FBM_FB_READY_IND_FUNC_EX)_prFbmCbFunc->aau4CbFunc[ucFbgId][FBM_CB_FUNC_FB_READY_EX_IND])(
                _prFbmCbFunc->aau4CbFuncTag[ucFbgId][FBM_CB_FUNC_FB_READY_EX_IND],
                &rPicNfyInfo);
        }
        
		if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->aau4CbFunc[ucFbgId][FBM_CB_FUNC_FB_FRAME_TYPEINFOR_CB],
                                     _prFbmCbFunc->aau4CbFuncCRC[ucFbgId][FBM_CB_FUNC_FB_FRAME_TYPEINFOR_CB]))
        {

            ((FBM_FB_FAME_TYPEINFO_FUNC)_prFbmCbFunc->aau4CbFunc[ucFbgId][FBM_CB_FUNC_FB_FRAME_TYPEINFOR_CB])(
                _prFbg[ucFbgId].u1DecoderSrcId, ucFbgId, ucFbId);
        }
    }
    
}


BOOL FBM_ForceReleaseFrameBuffer(UCHAR ucFbgId,UCHAR ucFbId)
{
    FBM_FBQ_T *prDisplayQ = NULL;
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }
    
    FBM_MUTEX_LOCK(ucFbgId);

    prDisplayQ = &_prFbg[ucFbgId].rDisplayQ;
    if(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ &&  prDisplayQ->ucCount > 0)
    {
        BOOL fgInDispQ = FALSE;
        UCHAR ucFbCnt= 0,ucFbIndex=0;

        while(ucFbCnt < prDisplayQ->ucCount)
        {
            ucFbIndex = (UCHAR)(prDisplayQ->ucReadIdx + ucFbCnt) % FBM_MAX_FB_NS_PER_GROUP;
            if(ucFbId == prDisplayQ->aucQueue[ucFbIndex])
            {
                fgInDispQ = TRUE;
                break;
            }
            ucFbCnt++;
        }

        if(fgInDispQ)
        {
            UCHAR ucToIndex = 0,ucFromIndex = 0;
            ucFbCnt = 0;
            while(ucFbIndex + ucFbCnt + 1< prDisplayQ->ucReadIdx + prDisplayQ->ucCount)
            {
               ucToIndex =  (ucFbIndex + ucFbCnt) % FBM_MAX_FB_NS_PER_GROUP;
               ucFromIndex = (ucFbIndex + ucFbCnt + 1) % FBM_MAX_FB_NS_PER_GROUP;
               prDisplayQ->aucQueue[ucToIndex] = prDisplayQ->aucQueue[ucFromIndex];
               ucFbCnt++;
            }
            prDisplayQ->ucCount--;
        }
        
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_READY;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);
    
    if(FBM_GetFrameBufferStatus(ucFbgId,ucFbId) != FBM_FB_STATUS_LOCK)
    {
        FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
    }
    return TRUE;
}

//-------------------------------------------------------------------------
/** FBM_CheckFrameBufferDispQ
 *  Get FBG display queue size
 *  @param  ucFbgId   FBG index.
 *  @return display queue size.
 */
//-------------------------------------------------------------------------
UCHAR FBM_CheckFrameBufferDispQ(UCHAR ucFbgId)
{
    UCHAR ucCount, ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);
#if 0
    if (_prFbg[ucFbgId].fgEnableNewSeamless)
    {
        if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
        {
            UCHAR ucFbId;
            ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];

            if (_prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessDisp || _prFbg[ucFbgId].fgAutoSeamless)
            {
#ifdef CC_GET_AFTER_RESIZE_DONE
                if (_prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessResized /* && have resized status */)
#else
                if (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid !=  FBM_FB_ID_UNKNOWN/* && have resized status */)
#endif
                {
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return 1;
                }
                else
                {
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return 0;
                }
            }
            else
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return 1;
            }
        }
        else
        {
            FBM_MUTEX_UNLOCK(ucFbgId);
            return 0;
        }
    }
    else
#endif
    ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
    if (_prFbg[ucFbgId].prPicHdr[ucFbId].fgResizeDispFrm)
    {
        if (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid == FBM_FB_ID_UNKNOWN)
        {
            FBM_MUTEX_UNLOCK(ucFbgId);
            return 0;
        }
        else
        {
            FBM_MUTEX_UNLOCK(ucFbgId);
            return 1;
        }
    }
    else
    {
        ucCount = _prFbg[ucFbgId].rDisplayQ.ucCount;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucCount;
}

//-------------------------------------------------------------------------
/** FBM_GetFrameBufferFromDispQ
 *  Get frame buffer from display queue
 *  @param  ucFbgId   FBG index.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFrameBufferFromDispQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;
    UCHAR ucKeepFbNs = 0;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucFbId = FBM_FB_ID_UNKNOWN;
    #if 0  // For LG seamless, B2R used renderQ number to keep frame
    if (FBM_ChkSeamlessMode(ucFbgId, SEAMLESS_BY_NPTV))
    {
        ucKeepFbNs = _prFbg[ucFbgId].fgSendEOS ? 0 : 3;
    }
    #endif
    
    if (_prFbg[ucFbgId].rDisplayQ.ucCount > ucKeepFbNs)
    {
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];

        if (_prFbg[ucFbgId].fgEnableNewSeamless)
        {
            if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
            {
                LOG(0,"ucFbId(%d)\n",ucFbId);                
                ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }

            if (_prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessDisp || _prFbg[ucFbgId].fgAutoSeamless)
            {
#ifdef CC_GET_AFTER_RESIZE_DONE
                if( _prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessResized == FALSE)
#else
                if ((_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid == FBM_FB_ID_UNKNOWN) &&
                    (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid == FBM_FB_ID_UNKNOWN))
#endif
                {
                    LOG(3, "Not Resized yet!! FbgId(%d) FbId(%d)\n",ucFbgId,ucFbId);
#if 1
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return FBM_FB_ID_UNKNOWN;
#endif
                }
            }
        }

        if (_prFbg[ucFbgId].prPicHdr[ucFbId].fgResizeDispFrm &&
                (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid == FBM_FB_ID_UNKNOWN) &&
                (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid == FBM_FB_ID_UNKNOWN))
        {
            LOG(3, "Not Resized yet! FbgId(%d) FbId(%d)\n",ucFbgId,ucFbId);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }

        _prFbg[ucFbgId].rDisplayQ.ucReadIdx =
            (UCHAR)(_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP;
        _prFbg[ucFbgId].rDisplayQ.ucCount--;
		//LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",ucFbgId,ucFbId, _prFbg[ucFbgId].rDisplayQ.ucCount);
		
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId(%d)\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",ucFbgId,ucFbId, _prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }
        
        LOG(4,"DispDeQ fb(%d,%d) Cnt(%d)\n",ucFbgId,ucFbId,_prFbg[ucFbgId].rDisplayQ.ucCount);
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
    }

    if ((_prFbg[ucFbgId].ucSeqChg) && (_prFbg[ucFbgId].ucNewFbId == ucFbId))
    {
        _prFbg[ucFbgId].u4FbgFlag |= _prFbg[ucFbgId].u4SeqChgFlag;
        _prFbg[ucFbgId].ucSeqChg = 0;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}

//-------------------------------------------------------------------------
/** FBM_PreLook2ndFrameBufferFromDispQ
 *  Pre-look frame buffer from display queue
 *  @param  ucFbgId   FBG index.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------

UCHAR FBM_PreLook2ndFrameBufferFromDispQ(UCHAR ucFbgId)
{
    UCHAR ucFbId = FBM_FB_ID_UNKNOWN;
    UCHAR ucKeepFbNs = 0;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);
    if (FBM_ChkSeamlessMode(ucFbgId, SEAMLESS_BY_NPTV))
    {
        ucKeepFbNs = (_prFbg[ucFbgId].fgSendEOS ? 0 : 3) + 1;
    }

    if (_prFbg[ucFbgId].rDisplayQ.ucCount > ucKeepFbNs)
    {
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[(_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP];
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId(%d)\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
        }
        else
        {
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
            {
                LOG(0,"FbgId(%d) ucFbId(%d) FbStatus(%d)\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
        }
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}

UCHAR FBM_PreLookBFromDispQ(UCHAR ucFbgId, UCHAR ucTheNth)
{
    UCHAR ucFbId = FBM_FB_ID_UNKNOWN;
   

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);
   
    if (_prFbg[ucFbgId].rDisplayQ.ucCount > ucTheNth)
    {
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[(_prFbg[ucFbgId].rDisplayQ.ucReadIdx + ucTheNth) % FBM_MAX_FB_NS_PER_GROUP];
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId(%d)\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
        }
        else
        {
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",
                    ucFbgId,ucFbId,
                    _prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
        }
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}

//-------------------------------------------------------------------------
/** FBM_PreLookNthFBFromDispQ
 *  Pre-look frame buffer from display queue
 *  @param  ucFbgId   FBG index.  @ucTheNth start from index 0. n means the N+1 th
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------

UCHAR FBM_PreLookNthFBFromDispQ(UCHAR ucFbgId, UCHAR ucTheNth)
{
    UCHAR ucFbId = FBM_FB_ID_UNKNOWN;
    UCHAR ucKeepFbNs = 0;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);
    if (FBM_ChkSeamlessMode(ucFbgId, SEAMLESS_BY_NPTV))
    {
        ucKeepFbNs = (_prFbg[ucFbgId].fgSendEOS ? 0 : 3) + ucTheNth;
    }

    if (_prFbg[ucFbgId].rDisplayQ.ucCount > ucKeepFbNs)
    {
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[(_prFbg[ucFbgId].rDisplayQ.ucReadIdx + ucTheNth) % FBM_MAX_FB_NS_PER_GROUP];
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId(%d)\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
        }
        else
        {
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",
                    ucFbgId,ucFbId,
                    _prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
        }
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}

//-------------------------------------------------------------------------
/** FBM_PreLookFrameBufferFromDispQ
 *  Pre-look frame buffer from display queue
 *  @param  ucFbgId   FBG index.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_PreLookFrameBufferFromDispQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;
    UCHAR ucKeepFbNs = 0;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucFbId = FBM_FB_ID_UNKNOWN;
    if (FBM_ChkSeamlessMode(ucFbgId, SEAMLESS_BY_NPTV))
    {
        ucKeepFbNs = _prFbg[ucFbgId].fgSendEOS ? 0 : 3;
    }
    if (_prFbg[ucFbgId].rDisplayQ.ucCount > ucKeepFbNs)
    {
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId(%d)\n",ucFbId);            
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",
                ucFbgId,ucFbId,
                _prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }        
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}

//-------------------------------------------------------------------------
/** FBM_GetNextDispQPts
 *  Get next frame buffer's PTS from display queue
 *  @param  ucFbgId   FBG index.
 *  @return PTS value.
 */
//-------------------------------------------------------------------------
UINT32 FBM_GetNextDispQPts(UCHAR ucFbgId)
{
    UINT32 u4Pts;
    UCHAR ucKeepFbNs = 0;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    u4Pts = 0;
    if (FBM_ChkSeamlessMode(ucFbgId, SEAMLESS_BY_NPTV))
    {
        ucKeepFbNs = _prFbg[ucFbgId].fgSendEOS ? 0 : 3;
    }
    if (_prFbg[ucFbgId].rDisplayQ.ucCount > ucKeepFbNs)
    {
        UCHAR ucFbId;

        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId(%d)\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return 0;
        }
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",ucFbgId,ucFbId,
                _prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);

        u4Pts = _prFbg[ucFbgId].prPicHdr[ucFbId].u4PTS;
    }
    else
    {
        // return last display q item
        UCHAR ucFbId;
        UCHAR ucLastQ;

        if (_prFbg[ucFbgId].rDisplayQ.ucReadIdx == 0)
        {
            ucLastQ = FBM_MAX_FB_NS_PER_GROUP - 1;
        }
        else
        {
            ucLastQ = (UCHAR)(_prFbg[ucFbgId].rDisplayQ.ucReadIdx - 1);
        }

        if (ucLastQ >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"LastQ(%d)\n",ucLastQ);
            FBM_MUTEX_UNLOCK(ucFbgId);
            ASSERT_FBM(ucLastQ < FBM_MAX_FB_NS_PER_GROUP);
            return 0;
        }

        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[ucLastQ];

        if (ucFbId < FBG_MAX_NS)
        {
            u4Pts = _prFbg[ucFbgId].prPicHdr[ucFbId].u4PTS;
        }
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return u4Pts;
}

//-------------------------------------------------------------------------
/** FBM_CalFrmRate
 *  Use next frame buffer's PTS from display queue to calculate frame rate
 *  @param  ucFbgId   FBG index.
 *  @return Frame rate type.
 */
//-------------------------------------------------------------------------
UCHAR FBM_CalFrmRate(UCHAR ucFbgId)
{
    UINT32 u4NextPts;
    UINT32 u4CurPts;
    UINT32 u4PtsDiff;
    UINT32 u4PtsInterval;
    INT32 i4NextTmpRef;
    INT32 i4CurTmpRef;
    UINT16 u2Interval;
    UCHAR ucFrmRate;
    UCHAR ucLastQ;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    u4NextPts = 0;
    ucFrmRate = MPEG_FRAME_RATE_UNKNOWN;

    if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
    {
        UCHAR ucFbId;

        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"FbId(%d)\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return 0;
        }
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",ucFbgId,ucFbId,
                _prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }        
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);

        u4NextPts = _prFbg[ucFbgId].prPicHdr[ucFbId].u4PTS;
        i4NextTmpRef = _prFbg[ucFbgId].prPicHdr[ucFbId].i4TemporalRef;

        if (_prFbg[ucFbgId].rDisplayQ.ucReadIdx == 0)
        {
            ucLastQ = FBM_MAX_FB_NS_PER_GROUP - 1;
        }
        else
        {
            ucLastQ = (UCHAR)(_prFbg[ucFbgId].rDisplayQ.ucReadIdx - 1);
        }

        if (ucLastQ >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"LastQ %d\n",ucLastQ);
            FBM_MUTEX_UNLOCK(ucFbgId);
            ASSERT_FBM(ucLastQ < FBM_MAX_FB_NS_PER_GROUP);
            return 0;
        }

        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[ucLastQ];

        u4CurPts = _prFbg[ucFbgId].prPicHdr[ucFbId].u4PTS;
        i4CurTmpRef = _prFbg[ucFbgId].prPicHdr[ucFbId].i4TemporalRef;

        if (u4NextPts >= u4CurPts)
        {
            u4PtsDiff = u4NextPts - u4CurPts;
        }
        else
        {
            u4PtsDiff = (0xFFFFFFFF - u4CurPts) + u4NextPts;
        }

        if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265))
        {
            /* H.264 temporal reference can not be used as units */
            u2Interval = 1;
        }
        else
        {
            if (i4NextTmpRef > i4CurTmpRef)
            {
                u2Interval = (UINT16)(i4NextTmpRef - i4CurTmpRef);
            }
            else
            {
                u2Interval = 1;
            }
        }

        u4PtsInterval = (u4PtsDiff /u2Interval);
        if(u4PtsInterval > FBM_PTS_FRM_RATE_15-FBM_PTS_FRM_RATE_TORRENCE)
        {
            ucFrmRate = MPEG_FRAME_RATE_30;
        }
        else if (u4PtsInterval == FBM_PTS_FRM_RATE_24)
        {
            ucFrmRate = MPEG_FRAME_RATE_24;
        }
        else if (u4PtsInterval > FBM_PTS_FRM_RATE_24-FBM_PTS_FRM_RATE_TORRENCE)
        {
            ucFrmRate = MPEG_FRAME_RATE_24_;
        }
        else if (u4PtsInterval >= FBM_PTS_FRM_RATE_25-FBM_PTS_FRM_RATE_TORRENCE)
        {
            ucFrmRate = MPEG_FRAME_RATE_25;
        }
        else if (u4PtsInterval >= FBM_PTS_FRM_RATE_30-1 && u4PtsInterval <= FBM_PTS_FRM_RATE_30+1)
        {
            ucFrmRate = MPEG_FRAME_RATE_30;
        }
        else if (u4PtsInterval > FBM_PTS_FRM_RATE_30-FBM_PTS_FRM_RATE_TORRENCE)
        {
            ucFrmRate = MPEG_FRAME_RATE_30_;
        }
        else if (u4PtsInterval >= FBM_PTS_FRM_RATE_50-FBM_PTS_FRM_RATE_TORRENCE)
        {
            ucFrmRate = MPEG_FRAME_RATE_50;
        }
        else if (u4PtsInterval == FBM_PTS_FRM_RATE_60)
        {
            ucFrmRate = MPEG_FRAME_RATE_60;
        }
        else if (u4PtsInterval > FBM_PTS_FRM_RATE_60-FBM_PTS_FRM_RATE_TORRENCE)
        {
            ucFrmRate = MPEG_FRAME_RATE_60_;
        }
        else
        {
            ucFrmRate = MPEG_FRAME_RATE_60;
        }
        LOG(1,"#### FBM calculate frame rate : %d %d\n",u4PtsInterval,ucFrmRate);
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFrmRate;
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferAtIsr
 *  Get frame buffer from display queue at ISR
 *  @param  ucFbgId   FBG index.
 *  @param  pu4AddrY   Y Address.
 *  @param  pu4AddrC   C Address.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetFrameBufferAtIsr(UCHAR ucFbgId, UINT32* pu4AddrY, UINT32* pu4AddrC)
{
    if ((pu4AddrY == NULL) || (pu4AddrC == NULL))
    {
        return;
    }

    if (VERIFY_FBG(ucFbgId))
    {
        *pu4AddrY = (UINT32)NULL;
        *pu4AddrC = (UINT32)NULL;
        return;
    }

    if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
    {
        UCHAR ucFbId;
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];

        if (VERIFY_FB(ucFbgId, ucFbId))
        {
            *pu4AddrY = (UINT32)NULL;
            *pu4AddrC = (UINT32)NULL;
            return;
        }

        *pu4AddrY = _prFbg[ucFbgId].au4AddrY[ucFbId];
        *pu4AddrC = _prFbg[ucFbgId].au4AddrC[ucFbId];

        // [LOG] FBM get fb at ISR
        SYSLOG(FBM_PREFIX + 30 + ucFbgId, ucFbId, _prFbg[ucFbgId].au4AddrY[ucFbId], _prFbg[ucFbgId].au4AddrC[ucFbId]);
    }
    else
    {
        *pu4AddrY = (UINT32)NULL;
        *pu4AddrC = (UINT32)NULL;

        // [LOG] FBM get fb at ISR
        SYSLOG(FBM_PREFIX + 30 + ucFbgId, FBM_FB_ID_UNKNOWN, 0, 0);

        LOG(7, "FBAI-F\n");
    }
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferPicHdr
 *  Get frame buffer picture header
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return picture header.
 */
//-------------------------------------------------------------------------
FBM_PIC_HDR_T *FBM_GetFrameBufferPicHdr(UCHAR ucFbgId, UCHAR ucFbId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        LOG(7,"FBG = %d , is invalid to get picture header.\n", ucFbgId);
        return NULL;
    }

    if (ucFbId != _prFbg[ucFbgId].ucFbNs)
    {
        if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
        {
            return NULL;
        }
    }
    else if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_DEBLOCKING))
    {
        if (VERIFY_FBG(ucFbgId))
        {
            return NULL;
        }
    }

	if(_prFbg[ucFbgId].prPicHdr==NULL)
	{
	    LOG(0,"FBM_GetFrameBufferPicHdr FBG[%d] PicHdr is Null\n",ucFbgId);
        return NULL;
	}
	else
	{
        return &(_prFbg[ucFbgId].prPicHdr[ucFbId]);
	}
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferSeqHdr
 *  Get frame buffer sequence header
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return sequence header.
 */
//-------------------------------------------------------------------------
FBM_SEQ_HDR_T *FBM_GetFrameBufferSeqHdr(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return NULL;
    }

    return &(_prFbg[ucFbgId].rSeqHdr);
}


//-------------------------------------------------------------------------
/** FBM_GetFrameBufferAddr
 *  Get frame buffer address
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  pu4AddrY   Y Address.
 *  @param  pu4AddrC   C Address.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetFrameBufferAddr(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC)
{
    if ((pu4AddrY == NULL) || (pu4AddrC == NULL))
    {
        return;
    }

    *pu4AddrY = (UINT32)NULL;
    *pu4AddrC = (UINT32)NULL;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    if (VERIFY_FB(ucFbgId, ucFbId))
    {
        if (ucFbId != _prFbg[ucFbgId].ucFbNs)
        {
            return;
        }
    }
    if (ucFbId>=FBM_MAX_FB_NS_PER_GROUP)
    {
        LOG(0,"FbId(%d)\n",ucFbId);
        ASSERT_FBM(ucFbId<FBM_MAX_FB_NS_PER_GROUP);
        return;
    }

    *pu4AddrY = _prFbg[ucFbgId].au4AddrY[ucFbId];
    *pu4AddrC = _prFbg[ucFbgId].au4AddrC[ucFbId];

}

//-------------------------------------------------------------------------
/** FBM_GetFrameBufferExtAddr
 *  Get frame buffer address
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @param  pu4AddrExtY   Y_LEN Address.
 *  @param  pu4AddrExtC   C_EN Address.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

VOID FBM_GetFrameBufferAddrExt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrYExt, UINT32 *pu4AddrCExt)
{
    if ((pu4AddrYExt == NULL) || (pu4AddrCExt == NULL))
    {
        return;
    }

    *pu4AddrYExt = (UINT32)NULL;
    *pu4AddrCExt = (UINT32)NULL;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    if (VERIFY_FB(ucFbgId, ucFbId))
    {
        if (ucFbId != _prFbg[ucFbgId].ucFbNs)
        {
            return;
        }
    }
    if (ucFbId>=FBM_MAX_FB_NS_PER_GROUP)
    {
        LOG(0,"FbId(%d)\n",ucFbId);
        ASSERT_FBM(ucFbId<FBM_MAX_FB_NS_PER_GROUP);
        return;
    }

    *pu4AddrYExt = _prFbg[ucFbgId].au4AddrY_Ext[ucFbId];
    *pu4AddrCExt = _prFbg[ucFbgId].au4AddrC_Ext[ucFbId];

	
}

//-------------------------------------------------------------------------
/** FBM_GetFrameBufferAddrMv
 *  Get MV buffer address. H264 ONLY.
 *  @param  ucFbgId   FBG index.
 *  @param  ucMvBufId   MV buffer index.
 *  @param  pu4AddrMv   MV buffer address.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

void FBM_GetFrameBufferAddrMv(UCHAR ucFbgId, UCHAR ucMvBufId, UINT32 *pu4AddrMv)
{
    if (pu4AddrMv == NULL)
    {
        return;
    }

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    if ((ucMvBufId>=_prFbg[ucFbgId].ucMvBufNs) || (ucMvBufId>=FBM_MAX_FB_NS_PER_GROUP-1))
    {
        return;
    }

    *pu4AddrMv = _prFbg[ucFbgId].au4AddrMv[ucMvBufId];

}

//-------------------------------------------------------------------------
/** FBM_GetFrameBufferMvNs
 *  Get MV buffer Ns. H264 ONLY.
 *  @param  ucFbgId   FBG index.
 *  @param  ucMvBufId   MV buffer index.
 *  @param  pu4AddrMv   MV buffer address.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetFrameBufferMvNs(UCHAR ucFbgId)
{
    UCHAR ucFbNs;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucFbNs = _prFbg[ucFbgId].ucMvBufNs;

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbNs;
}

//-------------------------------------------------------------------------
/** FBM_GetWorkingBuffer
 *  Get MV buffer address.
 *  @param  ucFbgId   FBG index.
 *  @param  pu4Addr   Working buffer address.
 *  @param  pu4Size    Working buffer size.
 *  @return NONE.
 */
//-------------------------------------------------------------------------


void FBM_GetWorkingBuffer(UCHAR ucFbgId, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    if ((pu4Addr == NULL) || (pu4Size == NULL))
    {
        return;
    }

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    *pu4Addr = _prFbg[ucFbgId].u4Workbuffer;
    *pu4Size = _prFbg[ucFbgId].u4WorkBufSize;

}

void FBM_GetExtraFrameBuffer(UCHAR ucFbgId, UINT32* pu4YAddr, UINT32* pu4CAddr)
{
    if ( (pu4YAddr == NULL) || (pu4CAddr == NULL) )
    {
        return;
    }
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    *pu4YAddr = _prFbg[ucFbgId].u4ExtraYBuffer;
    *pu4CAddr = _prFbg[ucFbgId].u4ExtraCBuffer;
}


//-------------------------------------------------------------------------
/** FBM_GetDecFrameBuffer
 *  Get decoding frame buffer
 *  @param  ucFbgId   FBG index.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetDecFrameBuffer(UCHAR ucFbgId)
{
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucFbId = _prFbg[ucFbgId].ucFbDecode;

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}


//-------------------------------------------------------------------------
/** FBM_GetReadyFrameBuffer
 *  Get ready frame buffer
 *  @param  ucFbgId   FBG index.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetReadyFrameBuffer(UCHAR ucFbgId)
{
    // Notes about usage
    // Error Concealment background for I picture

    UCHAR ucFbId;
    UINT32 u4FbIdx;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }
    if (VERIFY_FB_NS(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucFbId = FBM_FB_ID_UNKNOWN;

    if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
    {
        // get the last FB at display Q

        u4FbIdx = (UINT32)(((_prFbg[ucFbgId].rDisplayQ.ucReadIdx + _prFbg[ucFbgId].rDisplayQ.ucCount) - 1) % FBM_MAX_FB_NS_PER_GROUP);
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[u4FbIdx];

        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) FbStatus(%d)\n",ucFbgId,ucFbId,
                _prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
    }
    else
    {
        // get locked frame buffer
        for (u4FbIdx = _prFbg[ucFbgId].ucFbNsBase;
                ((u4FbIdx < _prFbg[ucFbgId].ucFbNs)&& (u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
        {
            if (_prFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
            {
                ucFbId = (UCHAR)u4FbIdx;
                u4FbIdx = FBM_MAX_FB_NS_PER_GROUP;
            }
        }

        // get any frame buffer which is NOT decoding
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            for (u4FbIdx = _prFbg[ucFbgId].ucFbNsBase; u4FbIdx < _prFbg[ucFbgId].ucFbNs; u4FbIdx++)
            {
                if (_prFbg[ucFbgId].ucFbDecode != (UCHAR)(u4FbIdx) && Fb_Using(ucFbgId,u4FbIdx))
                {
                    ucFbId = (UCHAR)u4FbIdx;
                    u4FbIdx = FBM_MAX_FB_NS_PER_GROUP;
                }
            }
        }
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}


//-------------------------------------------------------------------------
/** FBM_GetRefFrameBuffer
 *  Get reference frame buffer
 *  @param  ucFbgId   FBG index.
 *  @param  ucFRefFbId   forward reference FB index pointer.
 *  @param  ucBRefFbId   backward reference FB index pointer.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_GetRefFrameBuffer(UCHAR ucFbgId, UCHAR* ucFRefFbId, UCHAR* ucBRefFbId)
{
    if ((ucFRefFbId == NULL) ||
            (ucBRefFbId == NULL))
    {
        return;
    }

    if (VERIFY_FBG(ucFbgId))
    {
        *ucFRefFbId = FBM_FB_ID_UNKNOWN;
        *ucBRefFbId = FBM_FB_ID_UNKNOWN;

        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if (_prFbg[ucFbgId].ucFbFRefValid)
    {
        *ucFRefFbId = _prFbg[ucFbgId].ucFbFRef;
    }
    else
    {
        *ucFRefFbId = FBM_FB_ID_UNKNOWN;
    }

    if (_prFbg[ucFbgId].ucFbBRefValid)
    {
        *ucBRefFbId = _prFbg[ucFbgId].ucFbBRef;
    }
    else
    {
        *ucBRefFbId = FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);
}

void FBM_PrintFBStatus(UCHAR ucFbgId)
{
    UINT32 u4FbIdx;
    CHAR szDebug1[FBM_MAX_FB_NS_PER_GROUP+1];

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    if (VERIFY_FB_NS(ucFbgId))
    {
        return;
    }

    for (u4FbIdx = _prFbg[ucFbgId].ucFbNsBase;
            ((u4FbIdx < _prFbg[ucFbgId].ucFbNs)&& (u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
    {
        if(Fb_Using(ucFbgId,u4FbIdx))
        {
            szDebug1[u4FbIdx] = '0' +  _prFbg[ucFbgId].aucFbStatus[u4FbIdx];
        }
    }
    szDebug1[u4FbIdx] = 0;

    LOG(1, "%s\n", szDebug1);
}


//-------------------------------------------------------------------------
/** FBM_GetEmptyRefFrameBuffer
 *  Get empty reference frame buffer
 *  @param  ucFbgId   FBG index.
 *  @param  u4Delay   time to wait.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetEmptyRefFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay)
{
#ifdef CC_MT5351_SLT
    HAL_TIME_T rStart;
    HAL_TIME_T rStop;
    HAL_TIME_T rDelta;
#endif

    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

#ifdef CC_MT5351_SLT
    HAL_GetTime(&rStart);
#endif

    FBM_MUTEX_LOCK(ucFbgId);

    FBM_GetEmptyDelayTime(ucFbgId, &u4Delay);

    // We should not change status at NOT READY mode
    if (_prFbg[ucFbgId].u4FbgFlag & (FBM_FLAG_NOT_READY | FBM_FLAG_RESET | FBM_FLAG_CLEAR_FRC_PRM))
    {
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }
    //LOG(1, "REF: ");
    //FBM_PrintFBStatus(ucFbgId);

    ucFbId = FBM_FB_ID_UNKNOWN;
    // wait semaphore (Empty)
    if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyQSemaphore, X_SEMA_OPTION_WAIT, u4Delay) == OSR_OK)
    {
        // wait semaphore success

#ifdef NEW_SEQ_CHG_NOTIFY
        if (_prFbg[ucFbgId].ucRecordNextFbId)
        {
            x_memcpy(&rSeqChgEmptyQ, &_prFbg[ucFbgId].rEmptyQ, sizeof(FBM_FBQ_T));
        }
#endif

        ucFbId = _FbmGetRefFrameBufferFromEmptyQ(ucFbgId);

        // Work around counting semaphore issue
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }

#ifdef NEW_SEQ_CHG_NOTIFY
        if (_prFbg[ucFbgId].ucRecordNextFbId)
        {
            _prFbg[ucFbgId].ucRecordNextFbId = 0;
            _prFbg[ucFbgId].ucSeqChg = 1;
            _prFbg[ucFbgId].ucNewFbId = FBM_FB_ID_UNKNOWN;
            LOG(5,"[LEO] tag 1,ucFbId = %d\n",ucFbId);
            //x_memcpy(&rSeqChgEmptyQ, &_prFbg[ucFbgId].rEmptyQ, sizeof(FBM_FBQ_T));
            u4FirstFBStored = 0;
        }
#else
        if (!FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_DEBLOCKING))
        {

            if (_prFbg[ucFbgId].ucRecordNextFbId)
            {
                _prFbg[ucFbgId].ucNewFbId = ucFbId;
                _prFbg[ucFbgId].ucRecordNextFbId = 0;
                _prFbg[ucFbgId].ucSeqChg = 1;
            }
        }
#endif
        if (IS_REFERENCE_FB(ucFbgId, ucFbId) ||
                (FBM_FB_ID_UNKNOWN == _prFbg[ucFbgId].ucFbFRef) ||
                (FBM_FB_ID_UNKNOWN == _prFbg[ucFbgId].ucFbBRef))
        {   // we get an empty reference frame buffer
        }
        else
        {   // we get an empty B fram buffer

            // In this case, old forward reference become B, Bug Fixed (20041110)
            if (_prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].ucFbFRef] != FBM_FB_STATUS_EMPTY)
            {
                // wait semaphore (B & Empty)
                //                VERIFY(_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyBQSemaphore, X_SEMA_OPTION_NOWAIT, 0) == OSR_OK);
                if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyBQSemaphore, X_SEMA_OPTION_NOWAIT, 0) != OSR_OK)
                {
                    if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
                    {
                        // reset, handle abnormal case

                        FBM_MUTEX_UNLOCK(ucFbgId);
                        return FBM_FB_ID_UNKNOWN;
                    }
                    else
                    {
                        ASSERT_FBM(0);
                    }
                }

                // London Bug
                // Put old forward reference to Empty if it is still in ready status
                if (_prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].ucFbFRef] == FBM_FB_STATUS_READY)
                {
                        if (_prFbg[ucFbgId].fgCreateFromInst)
                        {
                            ASSERT_FBM(0);
                        }
                    LOG(1, "[ERR] Reference FB leak!\n");

                    // release semaphore (B & Empty)
                    VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyBQSemaphore) == OSR_OK);

                    // release semaphore (Empty)
                    //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);

                    _FBM_PutFrameBufferToEmptyQ(ucFbgId, _prFbg[ucFbgId].ucFbFRef);

                    _prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].ucFbFRef] = FBM_FB_STATUS_EMPTY;
                }
            }
        }

        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"FbId %d\n",ucFbId);
            
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
#ifdef __KERNEL__
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            else
            {
                    if (_prFbg[ucFbgId].fgCreateFromInst)
                    {
                        ASSERT_FBM(0);
                    }
                switch (_prFbg[ucFbgId].aucFbStatus[ucFbId])
                {
                case FBM_FB_STATUS_DISPLAYQ:
                    _FbmGetFrameBufferFromDispQ(ucFbgId, ucFbId);
                    //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
                    _FBM_PutFrameBufferToEmptyQ(ucFbgId, ucFbId);
                    _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_EMPTY;
                    break;
                case FBM_FB_STATUS_DECODE:
                case FBM_FB_STATUS_READY:
                case FBM_FB_STATUS_LOCK:
                default:
                   // VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
                    _FBM_PutFrameBufferToEmptyQ(ucFbgId, ucFbId);
                    _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_EMPTY;
                    break;
                }
            }
        }
#else
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",
                ucFbgId,ucFbId,
                _prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY);
#endif
    }
    else
    {
        // wait semaphore fail
        if (_prFbg[ucFbgId].hEmptyQSemaphore.i4Count > 0)
        {
            LOG(0,"ucFbgId %d i4Count %d\n",
                ucFbgId,
                _prFbg[ucFbgId].hEmptyQSemaphore.i4Count);
        }
        ASSERT_FBM(_prFbg[ucFbgId].hEmptyQSemaphore.i4Count == 0);

        if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
        {
                if (_prFbg[ucFbgId].fgCreateFromInst)
                {
                    ASSERT_FBM(0);
                }
            // get frame buffer from display Q

            ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
            _prFbg[ucFbgId].rDisplayQ.ucReadIdx =
                (UCHAR)((_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP);
            _prFbg[ucFbgId].rDisplayQ.ucCount--;

            LOG(6, "[LINE%d]Get FB(%d-%d) From DispQ Cnt%d.\n", __LINE__, ucFbgId, ucFbId, _prFbg[ucFbgId].rDisplayQ.ucCount);
            if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
            {
                LOG(0,"ucFbId %d\n",ucFbId);
                ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",
                    ucFbgId,ucFbId,
                    _prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
        }
        else
        {
            UINT32 u4FbIdx;
                if (_prFbg[ucFbgId].fgCreateFromInst)
                {
                    ASSERT_FBM(0);
                }                

            // it is possible. Both Ref is lock by vdp
            // 2005/02/18 William, I found this case when change channel, fbm reset is pending

            if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
            {
                // get locked frame buffer
                for (u4FbIdx = 0; u4FbIdx < FBM_MAX_FB_NS_PER_GROUP; u4FbIdx++)
                {
                    if (_prFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
                    {
                        LOG(3, "{DL}\n");

                        ucFbId = (UCHAR)u4FbIdx;
                        u4FbIdx = FBM_MAX_FB_NS_PER_GROUP;

                        // ask vdp to reset parameter, otherwise, assert at vdp set status to empty
                        _prFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_CLEAR_FRC_PRM;
                    }
                }
            }

            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
        }
    }

    if (ucFbId != FBM_FB_ID_UNKNOWN)
    {
        _prFbg[ucFbgId].ucLastDecodeReference = 1;
    }

    if ((ucFbId != FBM_FB_ID_UNKNOWN) &&
            (ucFbId != _prFbg[ucFbgId].ucFbBRef))
    {
        _prFbg[ucFbgId].ucFbFRef = _prFbg[ucFbgId].ucFbBRef;
        _prFbg[ucFbgId].ucFbBRef = ucFbId;

        _prFbg[ucFbgId].ucFbFRefValid = _prFbg[ucFbgId].ucFbBRefValid;
        _prFbg[ucFbgId].ucFbBRefValid = 0;
    }

    // Auto-Change Empty Status to Decode Status
    // Bug Fixed (20041206)
    _FbmFbLog(ucFbgId,ucFbId, FBM_FB_STATUS_DECODE, __func__, __LINE__);    
    if (_prFbg[ucFbgId].ucFbDecode != FBM_FB_ID_UNKNOWN)
    {
        LOG(0,"ucFbgId(%d) ucFbDecode(%d)\n", _prFbg[ucFbgId].ucFbDecode);
    }
    ASSERT_FBM(_prFbg[ucFbgId].ucFbDecode == FBM_FB_ID_UNKNOWN);
    if (ucFbId < FBM_MAX_FB_NS_PER_GROUP)
    {
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_DECODE;
        _prFbg[ucFbgId].ucFbDecode = ucFbId;
        _FbmResetPicHdrFields(ucFbgId, ucFbId);
    }
    else
    {
        ucFbId = FBM_FB_ID_UNKNOWN;
        LOG(1, "[Warning] Get Empty Ref Fail (%d, %d)\n", ucFbgId, ucFbId);
//        ASSERT_FBM(0);
    }

    // [LOG] FBM get empty Ref FB
    SYSLOG(FBM_PREFIX + 10 + ucFbgId, ucFbId, _prFbg[ucFbgId].rEmptyQ.ucCount, 0);

    FBM_MUTEX_UNLOCK(ucFbgId);

#ifdef CC_MT5351_SLT
    HAL_GetTime(&rStop);

    HAL_GetDeltaTime(&rDelta, &rStart, &rStop);

    if (rDelta.u4Micros > 100000)
    {
        LOG(1, "GetRefTime %u.%6u\n", rDelta.u4Seconds, rDelta.u4Micros);
    }
#endif

    return ucFbId;
}

//-------------------------------------------------------------------------
/** FBM_GetEmptyBFrameBuffer
 *  Get empty B frame buffer
 *  @param  ucFbgId   FBG index.
 *  @param  u4Delay   time to wait.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetEmptyBFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay)
{
#ifdef CC_MT5351_SLT
    HAL_TIME_T rStart;
    HAL_TIME_T rStop;
    HAL_TIME_T rDelta;
#endif

    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

#ifdef CC_MT5351_SLT
    HAL_GetTime(&rStart);
#endif

    FBM_MUTEX_LOCK(ucFbgId);

    FBM_GetEmptyDelayTime(ucFbgId, &u4Delay);

    // We should not change status at NOT READY mode
    if (_prFbg[ucFbgId].u4FbgFlag & (FBM_FLAG_NOT_READY | FBM_FLAG_RESET | FBM_FLAG_CLEAR_FRC_PRM))
    {
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }
    //LOG(1, "B: ");
    //FBM_PrintFBStatus(ucFbgId);

    ucFbId = FBM_FB_ID_UNKNOWN;
    // wait semaphore (B & Empty)
    if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyBQSemaphore, X_SEMA_OPTION_WAIT, u4Delay) == OSR_OK)
    {
        // wait semaphore success

        // wait semaphore (Empty)
        if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyQSemaphore, X_SEMA_OPTION_NOWAIT, 0) != OSR_OK)
        {
            if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
            {
                // reset, handle abnormal case

                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            else
            {
                ASSERT_FBM(0);
            }
        }

        ucFbId = _FbmGetBFrameBufferFromEmptyQ(ucFbgId);

        if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
        {
            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                LOG(1, "[Warning] Get Empty B Fail (%d, %d)\n", ucFbgId, ucFbId);

                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
        }
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d) \n",
                ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY);
    }
    else
    {
        // wait semaphore fail
        if (_prFbg[ucFbgId].hEmptyBQSemaphore.i4Count > 0)
        {
            LOG(0,"ucFbgId(%d) i4Count(%d)\n",ucFbgId,_prFbg[ucFbgId].hEmptyBQSemaphore.i4Count);
        }
        ASSERT_FBM(_prFbg[ucFbgId].hEmptyBQSemaphore.i4Count == 0);

        if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
        {
            if (_prFbg[ucFbgId].fgCreateFromInst)
            {
                ASSERT_FBM(0);
            }
            ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
            _prFbg[ucFbgId].rDisplayQ.ucReadIdx =
                (UCHAR)((_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP);
            _prFbg[ucFbgId].rDisplayQ.ucCount--;

            LOG(3,"{DQ}");
            if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
            {
                LOG(0,"ucFbId %d\n",ucFbId);
                ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",
                    ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
        }
        else
        {
            UINT32 u4FbIdx;
            if (_prFbg[ucFbgId].fgCreateFromInst)
            {
                ASSERT_FBM(0);
            }

            // it is possible. Both Ref is lock by vdp
            // 2005/02/18 William, I found this case when change channel, fbm reset is pending

            if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
            {
                // get locked frame buffer
                for (u4FbIdx = 0; u4FbIdx < FBM_MAX_FB_NS_PER_GROUP; u4FbIdx++)
                {
                    if (_prFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
                    {
                        LOG(3, "{DL}\n");

                        ucFbId = (UCHAR)u4FbIdx;
                        u4FbIdx = FBM_MAX_FB_NS_PER_GROUP;

                        // ask vdp to reset parameter, otherwise, assert at vdp set status to empty
                        _prFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_CLEAR_FRC_PRM;
                    }
                }
            }

            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
        }
    }

    if (ucFbId != FBM_FB_ID_UNKNOWN)
    {
        _prFbg[ucFbgId].ucLastDecodeReference = 0;
    }

    // Auto-Change Empty Status to Decode Status
    // Bug Fixed (20041206)
    _FbmFbLog(ucFbgId,ucFbId, FBM_FB_STATUS_DECODE, __func__, __LINE__);    
    if (_prFbg[ucFbgId].ucFbDecode != FBM_FB_ID_UNKNOWN)
    {
        LOG(0,"ucFbgId(%d) ucFbDecode(%d)\n",ucFbgId,_prFbg[ucFbgId].ucFbDecode);
    }
    ASSERT_FBM(_prFbg[ucFbgId].ucFbDecode == FBM_FB_ID_UNKNOWN);
    if (ucFbId < FBM_MAX_FB_NS_PER_GROUP)
    {
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_DECODE;
        _prFbg[ucFbgId].ucFbDecode = ucFbId;
        _FbmResetPicHdrFields(ucFbgId, ucFbId);
    }
    else
    {
        ucFbId = FBM_FB_ID_UNKNOWN;
        LOG(1, "[Warning] Get Empty B Fail (%d, %d)\n", ucFbgId, ucFbId);
//        ASSERT_FBM(0);
    }

    // [LOG] FBM get empty B FB
    SYSLOG(FBM_PREFIX + 20 + ucFbgId, ucFbId, _prFbg[ucFbgId].rEmptyQ.ucCount, 0);

    FBM_MUTEX_UNLOCK(ucFbgId);

#ifdef CC_MT5351_SLT
    HAL_GetTime(&rStop);

    HAL_GetDeltaTime(&rDelta, &rStart, &rStop);

    if (rDelta.u4Micros > 100000)
    {
        LOG(1, "GetBTime %u.%6u\n", rDelta.u4Seconds, rDelta.u4Micros);
    }
#endif

    return ucFbId;
}


//-------------------------------------------------------------------------
/** FBM_GetEmptyFrameBuffer
 *  Get empty frame buffer. H264 ONLY.
 *  @param  ucFbgId   FBG index.
 *  @param  u4Delay   time to wait.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetEmptyFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay)
{
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    FBM_GetEmptyDelayTime(ucFbgId, &u4Delay);

    // We should not change status at NOT READY mode
    if (_prFbg[ucFbgId].u4FbgFlag & (FBM_FLAG_NOT_READY | FBM_FLAG_RESET | FBM_FLAG_CLEAR_FRC_PRM))
    {
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }

    ucFbId = FBM_FB_ID_UNKNOWN;
    // wait semaphore (Empty)
    if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyQSemaphore, X_SEMA_OPTION_WAIT, u4Delay) == OSR_OK)
    {
        // wait semaphore success

#ifdef NEW_SEQ_CHG_NOTIFY
        if (_prFbg[ucFbgId].ucRecordNextFbId)
        {
            x_memcpy(&rSeqChgEmptyQ, &_prFbg[ucFbgId].rEmptyQ, sizeof(FBM_FBQ_T));
        }
#endif

        ucFbId = _FbmGetEmptyFrameBufferFromEmptyQ(ucFbgId);

        // Work around counting semaphore issue
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        
#ifdef NEW_SEQ_CHG_NOTIFY
        if (_prFbg[ucFbgId].ucRecordNextFbId)
        {
            _prFbg[ucFbgId].ucRecordNextFbId = 0;
            _prFbg[ucFbgId].ucSeqChg = 1;
            _prFbg[ucFbgId].ucNewFbId = FBM_FB_ID_UNKNOWN;
            //x_memcpy(&rSeqChgEmptyQ, &_prFbg[ucFbgId].rEmptyQ, sizeof(FBM_FBQ_T));
            u4FirstFBStored = 0;
        }
#else
        if (!FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_DEBLOCKING))
        {

            if (_prFbg[ucFbgId].ucRecordNextFbId)
            {
                _prFbg[ucFbgId].ucNewFbId = ucFbId;
                _prFbg[ucFbgId].ucRecordNextFbId = 0;
                _prFbg[ucFbgId].ucSeqChg = 1;
            }
        }
#endif
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId(%d)\n",ucFbId);
            
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",ucFbgId,ucFbId, _prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY);
    }
    else
    {
        // wait semaphore fail

        if (_prFbg[ucFbgId].fgCreateFromInst)
        {
            ASSERT_FBM(0);
        }
        if (_prFbg[ucFbgId].hEmptyQSemaphore.i4Count > 0)
        {
            LOG(0,"ucFbgId(%d) i4Count(%d)\n",ucFbgId,_prFbg[ucFbgId].hEmptyQSemaphore.i4Count);
        }
        ASSERT_FBM(_prFbg[ucFbgId].hEmptyQSemaphore.i4Count == 0);

        if ((_prFbg[ucFbgId].rDisplayQ.ucCount > 0 && FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE)))
        {
            UCHAR ucTmpId = 0;
            UCHAR ucTmpFbId = FBM_FB_ID_UNKNOWN;
            
            ucFbId = FBM_FB_ID_UNKNOWN;            

            // get frame buffer from display Q. Ref FB only set Empty
            for (ucTmpId = 0; ucTmpId < _prFbg[ucFbgId].rDisplayQ.ucCount; ucTmpId++)
            {
                ucTmpFbId = FBM_GetFrameBufferFromDispQ(ucFbgId);
                                
                if ((ucTmpFbId < FBM_MAX_FB_NS_PER_GROUP) && _prFbg[ucFbgId].afgRefList[ucTmpFbId])
                {
                    FBM_SetFrameBufferStatus(ucFbgId, ucTmpFbId, FBM_FB_STATUS_LOCK);
                    FBM_SetFrameBufferStatus(ucFbgId, ucTmpFbId, FBM_FB_STATUS_EMPTY);
                }
                else if (ucTmpFbId < FBM_MAX_FB_NS_PER_GROUP)
                {
                    ucFbId = ucTmpFbId;
                    break;
                }
                else
                {
                    LOG(3, "[LINE%d]Get FB(%d-%d) From DispQ Error!.\n", __LINE__, ucFbgId, ucFbId);
                    if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
                    {
                        LOG(0,"ucFbId(%d)\n",ucFbId);
                    }
                    ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
                }
            }
            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                LOG(0,"All displayQ are reference frame!!\n");
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }

            LOG(6, "[LINE%d]Get FB(%d-%d) From DispQ Cnt%d.\n", __LINE__, ucFbgId, ucFbId, _prFbg[ucFbgId].rDisplayQ.ucCount);

            if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
            {
                LOG(0,"ucFbId(%d)\n",ucFbId);                
                ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
#if 1 //FixMe
            if (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN &&
                _prFbg[ucFbgId].aucResizeFbStatus[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid] != FBM_FB_STATUS_EMPTY)
            {
                _FBM_PutFrameBufferToEmptyResizeQ(ucFbgId, _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResizeFbid);
                VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResizeQSemaphore) == OSR_OK);
            }
            
            if (_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid != FBM_FB_ID_UNKNOWN &&
                _prFbg[ucFbgId].aucResize2FbStatus[_prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid] != FBM_FB_STATUS_EMPTY)
            {
                _FBM_PutFrameBufferToEmptyResize2Q(ucFbgId, _prFbg[ucFbgId].prPicHdr[ucFbId].ucSeamlessResize2Fbid);
                VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResize2QSemaphore) == OSR_OK);
            }            
#endif
        }
        else
        {
            UINT32 u4FbIdx;

            // it is possible. Both Ref is lock by vdp
            // 2005/02/18 William, I found this case when change channel, fbm reset is pending

            if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
            {
                // get locked frame buffer
                for (u4FbIdx = 0; u4FbIdx < FBM_MAX_FB_NS_PER_GROUP; u4FbIdx++)
                {
                    if (_prFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
                    {
                        LOG(3, "{DL}\n");

                        ucFbId = (UCHAR)u4FbIdx;
                        u4FbIdx = FBM_MAX_FB_NS_PER_GROUP;

                        // ask vdp to reset parameter, otherwise, assert at vdp set status to empty
                        _prFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_CLEAR_FRC_PRM;
                    }
                }
            }

            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
        }
    }

    // Auto-Change Empty Status to Decode Status
    // Bug Fixed (20041206)
    _FbmFbLog(ucFbgId,ucFbId, FBM_FB_STATUS_DECODE, __func__, __LINE__);
    if (_prFbg[ucFbgId].ucFbDecode != FBM_FB_ID_UNKNOWN)
    {
        LOG(4,"ucFbgId(%d) ucFbDecode(%d)\n",ucFbgId,_prFbg[ucFbgId].ucFbDecode);
    }
    ASSERT_FBM(_prFbg[ucFbgId].ucFbDecode == FBM_FB_ID_UNKNOWN);
    if (ucFbId < FBM_MAX_FB_NS_PER_GROUP)
    {
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_DECODE;
        _prFbg[ucFbgId].ucFbDecode = ucFbId;
        _FbmResetPicHdrFields(ucFbgId, ucFbId);
    }
    else
    {
        ucFbId = FBM_FB_ID_UNKNOWN;
        LOG(1, "[Warning] Get Empty Ref Fail (%d, %d)\n", ucFbgId, ucFbId);
//        ASSERT_FBM(0);
    }

    // [LOG] FBM get empty Ref FB
    SYSLOG(FBM_PREFIX + 10 + ucFbgId, ucFbId, _prFbg[ucFbgId].rEmptyQ.ucCount, 0);

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}


//-------------------------------------------------------------------------
/** FBM_CanGetEmptyRefFrameBuffer
 *  Check empty reference frame buffer
 *  @param  ucFbgId   FBG index.
 *  @return frame buffer number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_CanGetEmptyRefFrameBuffer(UCHAR ucFbgId)
{
    UCHAR rValue;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    rValue = (UCHAR) _prFbg[ucFbgId].hEmptyQSemaphore.i4Count;

    FBM_MUTEX_UNLOCK(ucFbgId);

    return rValue;
}


//-------------------------------------------------------------------------
/** FBM_CanGetEmptyBFrameBuffer
 *  Check empty B frame buffer
 *  @param  ucFbgId   FBG index.
 *  @return frame buffer number.
 */
//-------------------------------------------------------------------------
UCHAR FBM_CanGetEmptyBFrameBuffer(UCHAR ucFbgId)
{
    UCHAR rValue;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    rValue = (UCHAR) _prFbg[ucFbgId].hEmptyBQSemaphore.i4Count;

    FBM_MUTEX_UNLOCK(ucFbgId);

    return rValue;
}


//-------------------------------------------------------------------------
/** FBM_GetDbkEmptyRefFrameBuffer
 *  Get empty Ref frame buffer for DBK
 *  @param  ucFbgId   FBG index.
 *  @param  u4Delay   time to wait.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------

UCHAR FBM_GetDbkEmptyRefFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay)
{

#ifdef CC_MT5351_SLT
    HAL_TIME_T rStart;
    HAL_TIME_T rStop;
    HAL_TIME_T rDelta;
#endif

    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

#ifdef CC_MT5351_SLT
    HAL_GetTime(&rStart);
#endif

    FBM_MUTEX_LOCK(ucFbgId);

    FBM_GetEmptyDelayTime(ucFbgId, &u4Delay);

    // We should not change status at NOT READY mode
    if (_prFbg[ucFbgId].u4FbgFlag & (FBM_FLAG_NOT_READY | FBM_FLAG_RESET | FBM_FLAG_CLEAR_FRC_PRM))
    {
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }

    ucFbId = FBM_FB_ID_UNKNOWN;
    // wait semaphore (B & Empty)
    if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyBQSemaphore, X_SEMA_OPTION_WAIT, u4Delay) == OSR_OK)
    {
        // wait semaphore success

        // wait semaphore (Empty)
        if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyQSemaphore, X_SEMA_OPTION_NOWAIT, 0) != OSR_OK)
        {
            if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
            {
                // reset, handle abnormal case

                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            else
            {
                ASSERT_FBM(0);
            }
        }

        ucFbId = _FbmGetBFrameBufferFromEmptyQ(ucFbgId);

        if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
        {
            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                LOG(1, "[Warning] Get Empty B Fail (%d, %d)\n", ucFbgId, ucFbId);

                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
        }
        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            LOG(0,"ucFbgId %d ucFbId %d aucFbStatus(%d)\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY);
    }
    else
    {
        // wait semaphore fail

        ASSERT_FBM(_prFbg[ucFbgId].hEmptyBQSemaphore.i4Count == 0);

        if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
        {
            ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
            _prFbg[ucFbgId].rDisplayQ.ucReadIdx =
                (UCHAR)((_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP);
            _prFbg[ucFbgId].rDisplayQ.ucCount--;

            LOG(3,"{DQ}");

            if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
            {
                LOG(0,"ucFbId %d\n",ucFbId);
                ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
            {
                LOG(0,"ucFbgId %d ucFbId %d aucFbStatus %d\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
        }
        else
        {
            UINT32 u4FbIdx;

            // it is possible. Both Ref is lock by vdp
            // 2005/02/18 William, I found this case when change channel, fbm reset is pending

            if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
            {
                // get locked frame buffer
                for (u4FbIdx = 0; u4FbIdx < FBM_MAX_FB_NS_PER_GROUP; u4FbIdx++)
                {
                    if (_prFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
                    {
                        LOG(3, "{DL}\n");

                        ucFbId = (UCHAR)u4FbIdx;
                        u4FbIdx = FBM_MAX_FB_NS_PER_GROUP;

                        // ask vdp to reset parameter, otherwise, assert at vdp set status to empty
                        _prFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_CLEAR_FRC_PRM;
                    }
                }
            }

            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
        }
    }

    if (VERIFY_FB(ucFbgId, ucFbId))
    {
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }

    // Auto-Change Empty Status to Decode Status
    // Bug Fixed (20041206)
    _FbmFbLog(ucFbgId,ucFbId, FBM_FB_STATUS_DECODE, __func__, __LINE__);
    _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_DECODE;
    _FbmResetPicHdrFields(ucFbgId, ucFbId);

#ifndef NEW_SEQ_CHG_NOTIFY
    if (_prFbg[ucFbgId].ucRecordNextFbId)
    {
        _prFbg[ucFbgId].ucNewFbId = ucFbId;
        _prFbg[ucFbgId].ucRecordNextFbId = 0;
        _prFbg[ucFbgId].ucSeqChg = 1;
    }
#endif

    // [LOG] FBM get empty B FB
    SYSLOG(FBM_PREFIX + 20 + ucFbgId, ucFbId, _prFbg[ucFbgId].rEmptyQ.ucCount, 0);

    FBM_MUTEX_UNLOCK(ucFbgId);

#ifdef CC_MT5351_SLT
    HAL_GetTime(&rStop);

    HAL_GetDeltaTime(&rDelta, &rStart, &rStop);

    if (rDelta.u4Micros > 100000)
    {
        LOG(1, "GetBTime %u.%6u\n", rDelta.u4Seconds, rDelta.u4Micros);
    }
#endif

    return ucFbId;
}


//-------------------------------------------------------------------------
/** FBM_UpdateReferenceList
 *  Update Reference FB List. H264 ONLY.
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId   FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------

void FBM_UpdateReferenceList(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgReference)
{
    if (VERIFY_FBG(ucFbgId) || VERIFY_FB(ucFbgId, ucFbId))
    {
        return;
    }

    if ((_prFbg[ucFbgId].u4VDecFmt != FBM_VDEC_H264) &&
        (_prFbg[ucFbgId].u4VDecFmt != FBM_VDEC_H265) &&
            (_prFbg[ucFbgId].u4VDecFmt != FBM_VDEC_AVS) &&
            (_prFbg[ucFbgId].u4VDecFmt != FBM_VDEC_RAW) &&
            (_prFbg[ucFbgId].u4VDecFmt != FBM_VDEC_JPEG)&&
            (_prFbg[ucFbgId].u4VDecFmt != FBM_VDEC_VP6) &&
            (_prFbg[ucFbgId].u4VDecFmt != FBM_VDEC_VP8) &&
            (_prFbg[ucFbgId].u4VDecFmt != FBM_VDEC_VP9))
    {
        return;
    }
    
    if (fgReference && (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY))
    {
        //Empty status only can be set reference(0), or
        //FBM_UpdateReferenceList(1) -> FBM_UpdateReferenceList(0)
        //will make this FB put into empty Q twice.
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if (_prFbg[ucFbgId].afgRefList[ucFbId] && (!fgReference))
    {
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY)
        {
            if (0)//_prFbg[ucFbgId].fgCreateFromInst)
            {
                ASSERT_FBM(0);
            }
            else
            {
            //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
            _FBM_PutFrameBufferToEmptyQ(ucFbgId, ucFbId);
            }
        }
    }
    _prFbg[ucFbgId].afgRefList[ucFbId] = fgReference;

    FBM_MUTEX_UNLOCK(ucFbgId);

}


#if 1
UCHAR FBM_GetDbkSmallBFrameBuffer(UCHAR ucFbgId)
{
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FBG_ID_UNKNOWN;
    }

    ucFbId = _prFbg[ucFbgId].ucFbNs;

    return ucFbId;
}
#endif


#if 1
void FBM_SetFrameBufferFtrCnt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 u4DbkOfStCnt, UINT32 u4Dbk4pCnt)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    _prFbg[ucFbgId].au4DbkOfstCnt[ucFbId] = u4DbkOfStCnt;
    _prFbg[ucFbgId].au4Dbk4pCnt[ucFbId] = u4Dbk4pCnt;

}


void FBM_GetFrameBufferFtrCnt(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4DbkOfStCnt, UINT32 *pu4Dbk4pCnt)
{
    if ((pu4DbkOfStCnt == NULL) || (pu4Dbk4pCnt == NULL))
    {
        return;
    }

    *pu4DbkOfStCnt = (UINT32)NULL;
    *pu4Dbk4pCnt = (UINT32)NULL;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    *pu4DbkOfStCnt = _prFbg[ucFbgId].au4DbkOfstCnt[ucFbId];
    *pu4Dbk4pCnt = _prFbg[ucFbgId].au4Dbk4pCnt[ucFbId];
}
#endif

//-------------------------------------------------------------------------
/** FBM_SetFrameBufferGlobalFlag
 *  Set FBG global flag
 *  @param  ucFbgId   FBG index.
 *  @param  ucFlag   flag.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_SetFrameBufferGlobalFlag(UCHAR ucFbgId, UINT8 ucFlag)
{
    UNUSED(ucFbgId);
    _ucFbmGlobalFlag |= ucFlag;
}


//-------------------------------------------------------------------------
/** FBM_ChkFrameBufferGlobalFlagIsr
 *  Check FBG global flag
 *  @param  ucFbgId   FBG index.
 *  @param  ucFlag   flag.
 *  @return 1/0 for valid/invalid.
 */
//-------------------------------------------------------------------------
UCHAR FBM_ChkFrameBufferGlobalFlagIsr(UCHAR ucFbgId, UINT8 ucFlag)
{
    UCHAR ucChk;

    UNUSED(ucFbgId);
    if (_ucFbmGlobalFlag & ucFlag)
    {
        ucChk = 1;
    }
    else
    {
        ucChk = 0;
    }

    return ucChk;
}

//-------------------------------------------------------------------------
/** FBM_ClrFrameBufferGlobalFlag
 *  Clear FBG global flag
 *  @param  ucFbgId   FBG index.
 *  @param  ucFlag   flag.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_ClrFrameBufferGlobalFlag(UCHAR ucFbgId, UINT8 ucFlag)
{
    UNUSED(ucFbgId);
    _ucFbmGlobalFlag &= ~ucFlag;
}

#ifdef FBM_FB_LOG
void FBM_FlushFbLog(UINT8 ucFbId, UINT8 u4FbStatus)
{
    _FbmFbLog(0xFF,0xFC, u4FbStatus, __func__, __LINE__);
}

#endif


//-------------------------------------------------------------------------
/** FBM_GetLatestEmptyFrameBuffer
 *  Get LATEST empty frame for MM STEP BACKWARD application
 *  @param  ucFbgId   FBG index.
 *  @param  u4Delay   time to wait.
 *  @return frame buffer index.
 */
//-------------------------------------------------------------------------
UCHAR FBM_GetLatestEmptyFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay)
{
    UCHAR ucFbId;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);
    if (_prFbg[ucFbgId].ucPlayMode != FBM_FBG_MM_MODE)
    {
        LOG(0,"ucFbgId %d ucPlayMode %d\n",ucFbgId,_prFbg[ucFbgId].ucPlayMode);
    }

    ASSERT_FBM(_prFbg[ucFbgId].ucPlayMode == FBM_FBG_MM_MODE);

    if (FBM_ChkFrameBufferFlagIsr(ucFbgId, FBM_FLAG_FB_NO_TIMEOUT))
    {
        u4Delay = 0xfffff;
    }
    
    if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE))
    {
        u4Delay = 0;
    }

    // We should not change status at NOT READY mode
    if (_prFbg[ucFbgId].u4FbgFlag & (FBM_FLAG_NOT_READY | FBM_FLAG_RESET | FBM_FLAG_CLEAR_FRC_PRM))
    {
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }

    ucFbId = FBM_FB_ID_UNKNOWN;
    if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG))
    {
        // wait semaphore (Empty)
        if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyQSemaphore, X_SEMA_OPTION_WAIT, u4Delay) == OSR_OK)
        {
            // wait semaphore success

#ifdef NEW_SEQ_CHG_NOTIFY
            if (_prFbg[ucFbgId].ucRecordNextFbId)
            {
                x_memcpy(&rSeqChgEmptyQ, &_prFbg[ucFbgId].rEmptyQ, sizeof(FBM_FBQ_T));
            }
#endif

            ucFbId = _FbmGetLatestEmptyFrameBufferFromEmptyQ(ucFbgId);

            // Work around counting semaphore issue
            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }

            if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",
                    ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }

            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY);
        }
        else
        {
            // wait semaphore fail

            ASSERT_FBM(_prFbg[ucFbgId].hEmptyQSemaphore.i4Count == 0);

            if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
            {
                // get frame buffer from display Q

                ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
                _prFbg[ucFbgId].rDisplayQ.ucReadIdx = (_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP;
                _prFbg[ucFbgId].rDisplayQ.ucCount--;

                LOG(6, "[LINE%d]Get FB(%d-%d) From DispQ Cnt%d.\n", __LINE__, ucFbgId, ucFbId, _prFbg[ucFbgId].rDisplayQ.ucCount);

                if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
                {
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return FBM_FB_ID_UNKNOWN;
                }
                if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
                {
                    LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",
                        ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
                    
                }
                ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
            }
            else
            {
                UINT32 u4FbIdx;

                // it is possible. Both Ref is lock by vdp
                // 2005/02/18 William, I found this case when change channel, fbm reset is pending

                if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_RESET)
                {
                    // get locked frame buffer
                    for (u4FbIdx = 0; u4FbIdx < FBM_MAX_FB_NS_PER_GROUP; u4FbIdx++)
                    {
                        if (_prFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK)
                        {
                            LOG(3, "{DL}\n");

                            ucFbId = u4FbIdx;
                            u4FbIdx = FBM_MAX_FB_NS_PER_GROUP;

                            // ask vdp to reset parameter, otherwise, assert at vdp set status to empty
                            _prFbg[ucFbgId].u4FbgFlag |= FBM_FLAG_CLEAR_FRC_PRM;
                        }
                    }
                }

                if (ucFbId == FBM_FB_ID_UNKNOWN)
                {
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return FBM_FB_ID_UNKNOWN;
                }
            }
        }
    }
    else
    {
        // wait semaphore (Empty)
        if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyQSemaphore, X_SEMA_OPTION_WAIT, u4Delay) == OSR_OK)
        {
            // wait semaphore success

            ucFbId = _FbmGetLatestEmptyFrameBufferFromEmptyQ(ucFbgId);

            // Work around counting semaphore issue
            if (ucFbId == FBM_FB_ID_UNKNOWN)
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }

            if ( ((ucFbId == _prFbg[ucFbgId].ucFbFRef) && (_prFbg[ucFbgId].ucFbFRefValid))
                    || ((ucFbId == _prFbg[ucFbgId].ucFbBRef) && (_prFbg[ucFbgId].ucFbBRefValid))
               )
            {   // we get an empty reference frame buffer
            }
            else
            {
                //Get empty B
                VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyBQSemaphore) == OSR_OK);
            }

            if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
            {
                FBM_MUTEX_UNLOCK(ucFbgId);
                return FBM_FB_ID_UNKNOWN;
            }
            if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",
                    ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);                
            }
            
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY);
        }
        else
        {
            // wait semaphore fail

            ASSERT_FBM(_prFbg[ucFbgId].hEmptyQSemaphore.i4Count == 0);

            if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
            {
                // get frame buffer from display Q

                ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
                _prFbg[ucFbgId].rDisplayQ.ucReadIdx = (_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP;
                _prFbg[ucFbgId].rDisplayQ.ucCount--;

                LOG(6, "[LINE%d]Get FB(%d-%d) From DispQ Cnt%d.\n", __LINE__, ucFbgId, ucFbId, _prFbg[ucFbgId].rDisplayQ.ucCount);

                if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
                {
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return FBM_FB_ID_UNKNOWN;
                }
                if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
                {
                    LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
                }
                ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);
            }
            else
            {
                if (ucFbId == FBM_FB_ID_UNKNOWN)
                {
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return FBM_FB_ID_UNKNOWN;
                }
            }
        }
    }

    if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG))
    {
        if (IS_REFERENCE_FB(ucFbgId, ucFbId) ||
                (FBM_FB_ID_UNKNOWN == _prFbg[ucFbgId].ucFbFRef) ||
                (FBM_FB_ID_UNKNOWN == _prFbg[ucFbgId].ucFbBRef))
        {
            // we get an empty reference frame buffer
            if (ucFbId != FBM_FB_ID_UNKNOWN)
            {
                _prFbg[ucFbgId].ucLastDecodeReference = 1;
            }

            if ((ucFbId != FBM_FB_ID_UNKNOWN) &&
                    (ucFbId != _prFbg[ucFbgId].ucFbBRef))
            {
                _prFbg[ucFbgId].ucFbFRef = _prFbg[ucFbgId].ucFbBRef;
                _prFbg[ucFbgId].ucFbBRef = ucFbId;

                _prFbg[ucFbgId].ucFbFRefValid = _prFbg[ucFbgId].ucFbBRefValid;
                _prFbg[ucFbgId].ucFbBRefValid = 0;
            }
        }
        else
        {
            // we get an empty B fram buffer
            if (ucFbId != FBM_FB_ID_UNKNOWN)
            {
                _prFbg[ucFbgId].ucLastDecodeReference = 0;
            }
        }

    }



    // Auto-Change Empty Status to Decode Status
    // Bug Fixed (20041206)
    // There might be FB in decode state in STEP BACKWARD case
    //ASSERT_FBM(_prFbg[ucFbgId].ucFbDecode == FBM_FB_ID_UNKNOWN);
    if (ucFbId < FBM_MAX_FB_NS_PER_GROUP)
    {
        _FbmFbLog(ucFbgId,ucFbId, FBM_FB_STATUS_DECODE, __func__, __LINE__);
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_DECODE;
        // ilii
        //_prFbg[ucFbgId].ucFbDecode = ucFbId;
        //_FbmResetPicHdrFields(ucFbgId, ucFbId);
    }
    else
    {
        ucFbId = FBM_FB_ID_UNKNOWN;
        LOG(1, "[Warning] Get Latest Empty Fail (%d, %d)\n", ucFbgId, ucFbId);
//        ASSERT_FBM(0);
    }

    // [LOG] FBM get empty Ref FB
    SYSLOG(FBM_PREFIX + 10 + ucFbgId, ucFbId, _prFbg[ucFbgId].rEmptyQ.ucCount, 0);

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}

//-------------------------------------------------------------------------
/** FBM_SetSeqChgFbId
 *  Setup the FB id to which the sequence change will be applied
 *  @param  ucFbgId   FBG index.
 *  @param  ucFbId    FB index.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
VOID FBM_SetSeqChgFbId(UCHAR ucFbgId, UCHAR ucFbId)
{
#ifdef NEW_SEQ_CHG_NOTIFY
    _ucSeqChgFbgId = ucFbgId;
    _ucSeqChgFbId = ucFbId;
#endif
}

UINT32 FBM_ConfigWorkingBuffer(UCHAR ucFbgId, UINT32 u4BufAddr)
{
    UCHAR ucSrcFbgId = FBM_FBG_ID_UNKNOWN;
    UINT32 u4FbgIdx, u4FbId;

    // init Q
    _prFbg[ucFbgId].rEmptyQ.ucCount = 0;
    _prFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;

    for (u4FbgIdx = 0; u4FbgIdx < FBG_MAX_NS; u4FbgIdx++)
    {
        if (_prFbg[u4FbgIdx].ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            for (u4FbId = _prFbg[ucFbgId].ucFbNsBase; u4FbId < _prFbg[u4FbgIdx].ucFbNs; u4FbId++)
            {
                if (_prFbg[u4FbgIdx].au4AddrY[u4FbId] == u4BufAddr)
                {
                    ucSrcFbgId = u4FbgIdx;
                    break;
                }
            }
        }

        if (ucSrcFbgId != FBM_FBG_ID_UNKNOWN)
        {
            break;
        }
    }

    if (ucSrcFbgId == FBM_FBG_ID_UNKNOWN)
    {
        LOG(0, "Error: could not find proper FBG from FBM instance!\n");
        return (UINT32)-1;
    }
    if (_prFbg[ucFbgId].ucFbNs != _prFbg[ucSrcFbgId].ucFbNs)
    {
        LOG(0,"ucFbgId(%d) ucSrcFbgId(%d) FbNs(%d) SrcFbNs(%d)\n",ucFbgId,ucSrcFbgId,_prFbg[ucFbgId].ucFbNs,_prFbg[ucSrcFbgId].ucFbNs);
    }
    ASSERT_FBM(_prFbg[ucFbgId].ucFbNs == _prFbg[ucSrcFbgId].ucFbNs);
    _prFbg[ucFbgId].ucFbNs = _prFbg[ucSrcFbgId].ucFbNs;
    _prFbg[ucFbgId].ucFbNsBase = _prFbg[ucSrcFbgId].ucFbNsBase;
    for (u4FbId = 0; u4FbId < _prFbg[ucSrcFbgId].ucFbNs; u4FbId++)
    {
        _prFbg[ucFbgId].au4AddrY[u4FbId] = _prFbg[ucSrcFbgId].au4AddrY[u4FbId];
        _prFbg[ucFbgId].au4AddrC[u4FbId] = _prFbg[ucSrcFbgId].au4AddrC[u4FbId];
    }

    _prFbg[ucFbgId].u4FbWidth = _prFbg[ucSrcFbgId].u4FbWidth;
    _prFbg[ucFbgId].u4FbHeight = _prFbg[ucSrcFbgId].u4FbHeight;
    _prFbg[ucFbgId].ucFbgCm = _prFbg[ucSrcFbgId].ucFbgCm;
    _prFbg[ucFbgId].ucMvBufNs = _prFbg[ucSrcFbgId].ucMvBufNs;

    _prFbg[ucFbgId].u4FbMemoryPoolSize = _prFbg[ucSrcFbgId].u4FbMemoryPoolSize;
    _prFbg[ucFbgId].u4FbMemoryPool = _prFbg[ucSrcFbgId].u4FbMemoryPool;
    
    // init working buf
    if (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_MPEG2)
    {
        _prFbg[ucFbgId].au4AddrMv[0] = _prFbg[ucSrcFbgId].au4AddrMv[0];

        if ((_prFbg[ucFbgId].ucFbNs - _prFbg[ucFbgId].ucFbNsBase >= FBM_DBK_FB_NS) && (_prFbg[ucFbgId].ucFbgType != FBM_FBG_TYPE_1080HD_RR))
        {
            _prFbg[ucFbgId].au4AddrY[_prFbg[ucFbgId].ucFbNs] = _prFbg[ucSrcFbgId].au4AddrY[_prFbg[ucFbgId].ucFbNs];
            _prFbg[ucFbgId].au4AddrC[_prFbg[ucFbgId].ucFbNs] = _prFbg[ucSrcFbgId].au4AddrC[_prFbg[ucFbgId].ucFbNs];
        }
    }
    else if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265))
    {
        for (u4FbId = 0; u4FbId < _prFbg[ucFbgId].ucMvBufNs && u4FbId < (FBM_MAX_FB_NS_PER_GROUP-1) ; u4FbId++)
        {
            _prFbg[ucFbgId].au4AddrMv[u4FbId] = _prFbg[ucSrcFbgId].au4AddrMv[u4FbId];
        }
    }
    else if (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RV)
    {
        _prFbg[ucFbgId].u4ExtraYBuffer = _prFbg[ucSrcFbgId].u4ExtraYBuffer;
        _prFbg[ucFbgId].u4ExtraCBuffer = _prFbg[ucSrcFbgId].u4ExtraCBuffer;
        _prFbg[ucFbgId].au4AddrMv[0] = _prFbg[ucSrcFbgId].au4AddrMv[0];
    }
    else
    {
        _prFbg[ucFbgId].au4AddrMv[0] = _prFbg[ucSrcFbgId].au4AddrMv[0];
    }

    _prFbg[ucFbgId].u4WorkBufSize = _prFbg[ucSrcFbgId].u4WorkBufSize;

    if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8)||
            (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP6))

    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_prFbg[ucFbgId].hEmptyQSemaphore, _prFbg[ucFbgId].hMutex, 0) == OSR_OK);
    }
    else
    {
        // create semaphore, Empty Q
        VERIFY(_FBM_csema_reset(&_prFbg[ucFbgId].hEmptyQSemaphore, _prFbg[ucFbgId].hMutex, 0) == OSR_OK);

        // create semaphore, Empty BQ
        VERIFY(_FBM_csema_reset(&_prFbg[ucFbgId].hEmptyBQSemaphore, _prFbg[ucFbgId].hMutex, 0) == OSR_OK);

#ifdef CC_VDP_FULL_ISR
        _prFbg[ucFbgId].hEmptyQSemaphore.prState = &_rState;
        _prFbg[ucFbgId].hEmptyBQSemaphore.prState = &_rState;
#endif

    }

    x_memset(_prFbg[ucFbgId].afgIsOwner, 0, sizeof(_prFbg[ucFbgId].afgIsOwner));

    _prFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
    _prFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
    _prFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;

    // put all frame buffer into empty queue
#if 0    
    for (u4FbId = 0; u4FbId < _prFbg[ucFbgId].ucFbNs; u4FbId++)
    {
        _FBM_PutFrameBufferToEmptyQ(ucFbgId, (UCHAR)(u4FbId));
    }
#endif

    ASSERT_FBM(_prFbg[ucFbgId].hEmptyQSemaphore.hMutex == _prFbg[ucFbgId].hMutex);
//    ASSERT_FBM(_prFbg[ucFbgId].hEmptyRQSemaphore.hMutex == _prFbg[ucFbgId].hMutex);
    ASSERT_FBM(_prFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _prFbg[ucFbgId].hMutex);

    return 0;
}

VOID FBM_GetFbByAddr(UINT32 u4Addr, UCHAR *pucFbgId, UCHAR *pucFbId)
{	
	UCHAR ucFbgId;
    UCHAR ucFbId;

    if (pucFbgId == NULL || pucFbId == NULL)
    {
        return;
    }

    *pucFbgId = FBM_FBG_ID_UNKNOWN;
    *pucFbId = FBM_FB_ID_UNKNOWN;

    for (ucFbgId = 0; ucFbgId < FBG_MAX_NS; ucFbgId++)
    {
        if(VERIFY_FBG(ucFbgId))
        {
            continue;
        }

        for (ucFbId=_prFbg[ucFbgId].ucFbNsBase; ucFbId<_prFbg[ucFbgId].ucFbNs; ucFbId++)
        {
            if (_prFbg[ucFbgId].au4AddrY[ucFbId] == u4Addr)
            {
                *pucFbgId = ucFbgId;
                *pucFbId = ucFbId;
                return;
            }
        }
        
        if (_prFbg[ucFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE)
        {
            for (ucFbId=_prFbg[ucFbgId].ucFbNsOldBase; ucFbId<_prFbg[ucFbgId].ucFbNsOld; ucFbId++)
            {
                if (_prFbg[ucFbgId].au4AddrY[ucFbId] == u4Addr)
                {
                    *pucFbgId = ucFbgId;
                    *pucFbId = ucFbId;
                    return;
                }
            }
        }
    }    
}

UCHAR _FBM_GetFbIdByAddr(UCHAR ucFbgId, UINT32 u4Addr)
{
    UCHAR i;
    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    for (i=_prFbg[ucFbgId].ucFbNsBase; i<_prFbg[ucFbgId].ucFbNs; i++)
    {
        if (_prFbg[ucFbgId].au4AddrY[i] == u4Addr)
        {
            return i;
        }
    }
    
    if (_prFbg[ucFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE)
    {
        for (i=_prFbg[ucFbgId].ucFbNsOldBase; i<_prFbg[ucFbgId].ucFbNsOld; i++)
        {
            if (_prFbg[ucFbgId].au4AddrY[i] == u4Addr)
            {
                return i;
            }
        }
    }
    return FBM_FB_ID_UNKNOWN;
}

BOOL FBM_FlushEmptyBufToDispQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;

    if ((ucFbgId == FBM_FBG_ID_UNKNOWN) || (_prFbg[ucFbgId].hMutex == (HANDLE_T)NULL))
    {
        return FALSE;
    }

    FBM_MUTEX_LOCK(ucFbgId);
    while (_prFbg[ucFbgId].rEmptyQ.ucCount > 0)
    {
        ucFbId = _prFbg[ucFbgId].rEmptyQ.aucQueue[_prFbg[ucFbgId].rEmptyQ.ucReadIdx];
    
        if (++(_prFbg[ucFbgId].rEmptyQ.ucReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
        {
            _prFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;
        }
        _prFbg[ucFbgId].rEmptyQ.ucCount--;
        
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            LOG(0,"ucFbId %d\n",ucFbId);
        }
        ASSERT_FBM(ucFbId != FBM_FB_ID_UNKNOWN);

        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_DISPLAYQ;
        _FbmFbLog(ucFbgId,ucFbId, _prFbg[ucFbgId].aucFbStatus[ucFbId], __func__, __LINE__);
        _prFbg[ucFbgId].prPicHdr[ucFbId].fgNotDisplay = TRUE;
        _FbmPutFrameBufferToDispQ(ucFbgId, ucFbId);

        LOG(3, "FBM_FlushEmptyBufToDispQ Empty -> DispQ : (%d)\n", ucFbId);
    }
    VERIFY(_FBM_csema_reset(&_prFbg[ucFbgId].hEmptyQSemaphore, _prFbg[ucFbgId].hMutex, 0) == OSR_OK);
    VERIFY(_FBM_csema_reset(&_prFbg[ucFbgId].hEmptyBQSemaphore, _prFbg[ucFbgId].hMutex, 0) == OSR_OK);

    FBM_MUTEX_UNLOCK(ucFbgId);

    LOG(3, "FBM_FlushEmptyBufToDispQ EmptyQ Cnt : (%d)\n", _prFbg[ucFbgId].rEmptyQ.ucCount);
    
    return TRUE;
}



BOOL FBM_ReleaseFBFlush(UCHAR ucFbgId)
{
    UCHAR i;
    UCHAR ucFbId;
    FBM_MUTEX_LOCK(ucFbgId);
    LOG(3, "FBM_ReleaseFBFlush() Enter.\n");
    while (_prFbg[ucFbgId].rEmptyQ.ucCount > 0)
    {
        ucFbId = _prFbg[ucFbgId].rEmptyQ.aucQueue[_prFbg[ucFbgId].rEmptyQ.ucReadIdx];
    
        if (++(_prFbg[ucFbgId].rEmptyQ.ucReadIdx) >= FBM_MAX_FB_NS_PER_GROUP)
        {
            _prFbg[ucFbgId].rEmptyQ.ucReadIdx = 0;
        }
        _prFbg[ucFbgId].rEmptyQ.ucCount--;
    
        ASSERT_FBM(ucFbId != FBM_FB_ID_UNKNOWN);

#if 1
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_RECYCLE;
#else
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_DISPLAYQ;
        _prFbg[ucFbgId].prPicHdr[ucFbId].fgNotDisplay = TRUE;
        _FbmPutFrameBufferToDispQ(ucFbgId, ucFbId);
#endif 
        _FbmFbLog(ucFbgId,ucFbId, _prFbg[ucFbgId].aucFbStatus[ucFbId], __func__, __LINE__);
    }
    VERIFY(_FBM_csema_reset(&_prFbg[ucFbgId].hEmptyQSemaphore, _prFbg[ucFbgId].hMutex, 0) == OSR_OK);
    VERIFY(_FBM_csema_reset(&_prFbg[ucFbgId].hEmptyBQSemaphore, _prFbg[ucFbgId].hMutex, 0) == OSR_OK);

    _prFbg[ucFbgId].ucFbDecode = FBM_FB_ID_UNKNOWN;
    _prFbg[ucFbgId].ucFbFRef = FBM_FB_ID_UNKNOWN;
    _prFbg[ucFbgId].ucFbBRef = FBM_FB_ID_UNKNOWN;
    _prFbg[ucFbgId].ucFbFRefValid = 0;
    _prFbg[ucFbgId].ucFbBRefValid = 0;
    
    for (i=_prFbg[ucFbgId].ucFbNsBase; i < _prFbg[ucFbgId].ucFbNs; i++)
    {
        _prFbg[ucFbgId].afgRefList[i] = FALSE;
    }

    LOG(3, "FBM_ReleaseFBFlush() Leave.\n");
    FBM_MUTEX_UNLOCK(ucFbgId);
    return TRUE;
}

BOOL FBM_InsertFB(UCHAR ucFbgId, UINT32 u4Addr)
{
    UCHAR ucFbId;
    UCHAR ucCount;
    if (VERIFY_FBG(ucFbgId) || u4Addr == 0)
    {
        return FALSE;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if (_prFbg[ucFbgId].au4AddrY[0] == 0)
    {
        FBM_ConfigWorkingBuffer(ucFbgId, u4Addr);
    }

    ucFbId = _FBM_GetFbIdByAddr(ucFbgId, u4Addr);

    if ((ucFbId == FBM_FB_ID_UNKNOWN) || (ucFbId >= FBM_MAX_FB_NS_PER_GROUP))
    {
        LOG(0, "Can't find target FB by the given addr!\n");
        ASSERT_FBM(0);
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FALSE;
    }

    ASSERT_FBM(_prFbg[ucFbgId].fgCreateFromInst == TRUE);

    if (_fgCheckFBStatus)
    {
        if (!(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_UNKNOWN ||
                   _prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK ||
                   _prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_RECYCLE))
        {
            LOG(0, "Insert fbid %d status NG %d !\n", ucFbId, _prFbg[ucFbgId].aucFbStatus[ucFbId]);
            if ((_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_UNKNOWN) &&
                (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_LOCK) &&
                (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_RECYCLE))
            {
                LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
            }
            ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_UNKNOWN ||
                   _prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK ||
                   _prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_RECYCLE);
        }
        ASSERT_FBM(_prFbg[ucFbgId].afgIsOwner[ucFbId] == FALSE);
    }

    ucCount = 1;
    if(FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_DEBLOCKING))
    {
        ucCount = 2;
    }
    if (_prFbg[ucFbgId].rEmptyQ.ucCount >= ucCount)
    {
        _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_DISPLAYQ;
        _prFbg[ucFbgId].prPicHdr[ucFbId].fgNotDisplay= TRUE;
        _prFbg[ucFbgId].afgIsOwner[ucFbId] = TRUE;
        _FbmFbLog(ucFbgId,ucFbId, FBM_FB_STATUS_DISPLAYQ, __func__, __LINE__);
        _FbmPutFrameBufferToDispQ(ucFbgId, ucFbId);
        FBM_MUTEX_UNLOCK(ucFbgId);
        return TRUE;
    }
    
    _FbmFbLog(ucFbgId,ucFbId, FBM_FB_STATUS_EMPTY, __func__, __LINE__);
    _prFbg[ucFbgId].aucFbStatus[ucFbId] = FBM_FB_STATUS_EMPTY;
    _prFbg[ucFbgId].afgIsOwner[ucFbId] = TRUE;

    _FBM_PutFrameBufferToEmptyQ(ucFbgId, ucFbId);

    if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP6) ||
        (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8))
    {
        //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
    }
    else
    {
        UCHAR ucRefCount = 0;

        if (_prFbg[ucFbgId].ucFbFRef != FBM_FB_ID_UNKNOWN)
        {
            ucRefCount++;
        }
        
        if (_prFbg[ucFbgId].ucFbBRef != FBM_FB_ID_UNKNOWN)
        {
            ucRefCount++;
        }
        
        //if (_prFbg[ucFbgId].hEmptyQSemaphore.i4Count + ucRefCount >= 2)
        {
            VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyBQSemaphore) == OSR_OK);
        }
        //VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyQSemaphore) == OSR_OK);
    }

    FBM_MUTEX_UNLOCK(ucFbgId);
    return TRUE;
}

UCHAR FBM_GetFreeFB(UCHAR ucFbgId)
{
    UCHAR i, ucTargetId = FBM_FB_ID_UNKNOWN;
    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    ASSERT_FBM(_prFbg[ucFbgId].fgCreateFromInst == TRUE);
    FBM_MUTEX_LOCK(ucFbgId);

    for (i=_prFbg[ucFbgId].ucFbNsBase; i < _prFbg[ucFbgId].ucFbNs; i++)
    {
        if (_prFbg[ucFbgId].afgIsOwner[i] == FALSE)
        {
            continue;
        }
        
        if (_prFbg[ucFbgId].aucFbStatus[i] == FBM_FB_STATUS_RECYCLE)
        {
            ucTargetId = i;
            break;
        }

        if (_prFbg[ucFbgId].aucFbStatus[i] == FBM_FB_STATUS_LOCK)
        {
            if ((_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H264) ||
                (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_H265) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_AVS) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_RAW) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_JPEG)||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP6) ||
                    (_prFbg[ucFbgId].u4VDecFmt == FBM_VDEC_VP8))
            {
                if (!_prFbg[ucFbgId].afgRefList[i])
                {
                    ucTargetId = i;
                    break;
                }
            }
            else
            {
                if (!IS_REFERENCE_FB(ucFbgId, i))
                {   
                    ucTargetId = i;
                    break;
                }
            }
        }
    }

    if (ucTargetId != FBM_FB_ID_UNKNOWN)
    {
        _prFbg[ucFbgId].afgIsOwner[ucTargetId] = FALSE;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucTargetId;
}


BOOL FBM_GetInsertFBCount(UCHAR ucFbgId, UINT32 *pu4Count)
{
    if (VERIFY_FBG(ucFbgId) || pu4Count == 0)
    {
        return FALSE;
    }

    FBM_MUTEX_LOCK(ucFbgId);    
    *pu4Count = _prFbg[ucFbgId].rEmptyQ.ucCount;
    FBM_MUTEX_UNLOCK(ucFbgId);
    return TRUE;
}
UCHAR FBM_GetResizeFrameBufferNs(UCHAR ucFbgId)
{
    UCHAR ucFbNs;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucFbNs = _prFbg[ucFbgId].ucSeamlessFbNs;

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbNs;
}

VOID FBM_GetResizeFrameBufferSize(UCHAR ucFbgId, UINT32 *pu4Width, UINT32 *pu4Height)
{
    if ((pu4Width == NULL) || (pu4Height == NULL))
    {
        return;
    }

    *pu4Width = (UINT32)NULL;
    *pu4Height = (UINT32)NULL;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    *pu4Width = FBM_FBG_1080HD_H;
    *pu4Height = FBM_FBG_1080HD_V;
}

UCHAR FBM_GetResizeFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId)
{
    UCHAR ucFbStatus;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_RESIZE_FB(ucFbgId, ucFbId)))
    {
        return FBM_FB_STATUS_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucFbStatus = _prFbg[ucFbgId].aucResizeFbStatus[ucFbId];

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbStatus;
}

UCHAR FBM_CheckResizeFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus)
{
    UCHAR ucValid;

    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_RESIZE_FB(ucFbgId, ucFbId)))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    ucValid = (ucFbStatus == _prFbg[ucFbgId].aucResizeFbStatus[ucFbId]);

    /*
    // VDP protection
    if (_prFbg[ucFbgId].u4FbgFlag & FBM_FLAG_NOT_READY)
    {
        ucValid = 1;
    }
    */

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucValid;
}

VOID FBM_GetResizeFrameBufferAddr(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC)
{
    if ((pu4AddrY == NULL) || (pu4AddrC == NULL))
    {
        return;
    }

    *pu4AddrY = (UINT32)NULL;
    *pu4AddrC = (UINT32)NULL;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    if (VERIFY_RESIZE_FB(ucFbgId, ucFbId))
    {
        if (ucFbId != _prFbg[ucFbgId].ucSeamlessFbNs)
        {
            return;
        }
    }

    if (ucFbId>=FBM_MAX_FB_NS_PER_GROUP)
    {
        LOG(0,"ucFbId %d\n",ucFbId);
        ASSERT_FBM(ucFbId<FBM_MAX_FB_NS_PER_GROUP);
        return;
    }

    *pu4AddrY = _prFbg[ucFbgId].au4ResizeAddrY[ucFbId];
    *pu4AddrC = _prFbg[ucFbgId].au4ResizeAddrC[ucFbId];
}

VOID FBM_GetResize2FrameBufferAddr(UCHAR ucFbgId, UCHAR ucFbId, UINT32 *pu4AddrY, UINT32 *pu4AddrC)
{
    if ((pu4AddrY == NULL) || (pu4AddrC == NULL))
    {
        return;
    }

    *pu4AddrY = (UINT32)NULL;
    *pu4AddrC = (UINT32)NULL;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    if (VERIFY_RESIZE_FB(ucFbgId, ucFbId))
    {
        if (ucFbId != _prFbg[ucFbgId].ucSeamless2FbNs)
        {
            return;
        }
    }

    if (ucFbId>=FBM_MAX_FB_NS_PER_GROUP)
    {
        LOG(0,"ucFbId %d\n",ucFbId);
        ASSERT_FBM(ucFbId<FBM_MAX_FB_NS_PER_GROUP);
        return;
    }

    *pu4AddrY = _prFbg[ucFbgId].au4Resize2AddrY[ucFbId];
    *pu4AddrC = _prFbg[ucFbgId].au4Resize2AddrC[ucFbId];
}

VOID FBM_SetResizeFrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_RESIZE_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    // [LOG] FBM change FB status
    SYSLOG(FBM_PREFIX + ucFbgId, ucFbId, _prFbg[ucFbgId].aucResizeFbStatus[ucFbId], ucFbStatus);
    _FbmFbLog(ucFbgId,ucFbId, ucFbStatus, __func__, __LINE__);

    // check current resize frame buffer status change
    if (ucFbId == _prFbg[ucFbgId].ucFbResize && ucFbStatus != FBM_FB_STATUS_RESIZE)
    {
        _prFbg[ucFbgId].ucFbResize = FBM_FB_ID_UNKNOWN;
    }

    switch (ucFbStatus)
    {
    case FBM_FB_STATUS_EMPTY:

        if (_prFbg[ucFbgId].aucResizeFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            _FBM_PutFrameBufferToEmptyResizeQ(ucFbgId, ucFbId);
            VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResizeQSemaphore) == OSR_OK);
        }
        break;

    case FBM_FB_STATUS_RESIZE:

        _prFbg[ucFbgId].ucFbResize = ucFbId;
        break;
    case FBM_FB_STATUS_RESIZE_DONE:

        if (_prFbg[ucFbgId].aucResizeFbStatus[ucFbId] != FBM_FB_STATUS_RESIZE)
        {
            LOG(6, "[Warning] Set ResizeDone to non-Resizing frame (%d, %d)!\n", ucFbId, _prFbg[ucFbgId].aucResizeFbStatus[ucFbId]);
            ucFbStatus = FBM_FB_STATUS_EMPTY;
        }

#ifdef CC_GET_AFTER_RESIZE_DONE
        if (_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId] != FBM_FB_ID_UNKNOWN &&
                _prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]] == FBM_FB_STATUS_DISPLAYQ &&
                _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]].ucSeamlessResizeFbid == ucFbId &&
                _prFbg[ucFbgId].aucResizeTaskId[ucFbId] == _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]].ucSeamlessTaskId)
#else
        if (_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId] != FBM_FB_ID_UNKNOWN &&
                (_prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]] == FBM_FB_STATUS_DISPLAYQ ||
                 _prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]] == FBM_FB_STATUS_LOCK) &&
                _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]].ucSeamlessResizeFbid == ucFbId &&
                _prFbg[ucFbgId].aucResizeTaskId[ucFbId] == _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]].ucSeamlessTaskId)
#endif
        {
            if (ucFbStatus != FBM_FB_STATUS_RESIZE_DONE)
            {
                LOG(0,"ucFbStatus %d\n",ucFbStatus);
            }
            ASSERT_FBM(ucFbStatus == FBM_FB_STATUS_RESIZE_DONE);
            _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]].fgSeamlessResized = TRUE;
        }
        else
        {
            LOG(6, "[Warning] Set ResizeDone with src (%d:%d) not in dispQ, id %d/%d\n",
                _prFbg[ucFbgId].aucResizeSrcFbId[ucFbId],
                _prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]],
                _prFbg[ucFbgId].aucResizeTaskId[ucFbId],
                _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResizeSrcFbId[ucFbId]].ucSeamlessTaskId
               );
            ucFbStatus = FBM_FB_STATUS_EMPTY;
#if 1
            if (_prFbg[ucFbgId].aucResizeFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
            {
                _FBM_PutFrameBufferToEmptyResizeQ(ucFbgId, ucFbId);
                VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResizeQSemaphore) == OSR_OK);
            }
#endif
        }

        break;

    default:
        ASSERT_FBM(0);
        break;
    }
	
    _prFbg[ucFbgId].aucResizeFbStatus[ucFbId] = ucFbStatus;

    FBM_MUTEX_UNLOCK(ucFbgId);

#if 1
    if ((ucFbStatus == FBM_FB_STATUS_RESIZE_DONE) && !FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE))
    {
        FB_CHG_INFO_T t_fb_info;

        t_fb_info.ucFbgId = ucFbgId;
        t_fb_info.ucFbid  = ucFbId;
        
        if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_FB_READY_IND]))
        {
            ((FBM_FB_READY_IND_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND])(&t_fb_info);
        }
        else
        {
            ASSERT_FBM(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND] == 0);
        }
    }
#endif
}

VOID FBM_SetResize2FrameBufferStatus(UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus)
{
    if ((VERIFY_FBG(ucFbgId)) || (VERIFY_RESIZE_FB(ucFbgId, ucFbId)))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    // [LOG] FBM change FB status
    SYSLOG(FBM_PREFIX + ucFbgId, ucFbId, _prFbg[ucFbgId].aucResize2FbStatus[ucFbId], ucFbStatus);
    _FbmFbLog(ucFbgId,ucFbId, ucFbStatus, __func__, __LINE__);

    // check current resize frame buffer status change
    if (ucFbId == _prFbg[ucFbgId].ucFbResize2 && ucFbStatus != FBM_FB_STATUS_RESIZE)
    {
        _prFbg[ucFbgId].ucFbResize2 = FBM_FB_ID_UNKNOWN;
    }

    switch (ucFbStatus)
    {
    case FBM_FB_STATUS_EMPTY:
        if (_prFbg[ucFbgId].aucResize2FbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            _FBM_PutFrameBufferToEmptyResize2Q(ucFbgId, ucFbId);
            VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResize2QSemaphore) == OSR_OK);
        }
        break;

    case FBM_FB_STATUS_RESIZE:

        _prFbg[ucFbgId].ucFbResize2 = ucFbId;
		
		LOG(1,"FBM_FB_STATUS_RESIZE2 ucFbgId %d ucFbResize %d\n",ucFbgId,ucFbId);
		
        break;
    case FBM_FB_STATUS_RESIZE_DONE:

        if (_prFbg[ucFbgId].aucResize2FbStatus[ucFbId] != FBM_FB_STATUS_RESIZE)
        {
            LOG(6, "[Warning] Set ResizeDone to non-Resizing frame (%d, %d)!\n", ucFbId, _prFbg[ucFbgId].aucResize2FbStatus[ucFbId]);
            ucFbStatus = FBM_FB_STATUS_EMPTY;
        }

#ifdef CC_GET_AFTER_RESIZE_DONE
        if (_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId] != FBM_FB_ID_UNKNOWN &&
                _prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]] == FBM_FB_STATUS_DISPLAYQ &&
                _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]].ucSeamlessResize2Fbid == ucFbId &&
                _prFbg[ucFbgId].aucResize2TaskId[ucFbId] == _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]].ucSeamlessTaskId)
#else
        if (_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId] != FBM_FB_ID_UNKNOWN &&
                (_prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]] == FBM_FB_STATUS_DISPLAYQ ||
                 _prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]] == FBM_FB_STATUS_LOCK) &&
                _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]].ucSeamlessResize2Fbid == ucFbId &&
                _prFbg[ucFbgId].aucResize2TaskId[ucFbId] == _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]].ucSeamlessTaskId)
#endif
        {
            if (ucFbStatus != FBM_FB_STATUS_RESIZE_DONE)
            {
                LOG(0,"ucFbStatus %d\n",ucFbStatus);
            }
            ASSERT_FBM(ucFbStatus == FBM_FB_STATUS_RESIZE_DONE);
            _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]].fgSeamlessResized = TRUE;
        }
        else
        {
            LOG(6, "[Warning] Set ResizeDone with src (%d:%d) not in dispQ, id %d/%d\n",
                _prFbg[ucFbgId].aucResize2SrcFbId[ucFbId],
                _prFbg[ucFbgId].aucFbStatus[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]],
                _prFbg[ucFbgId].aucResize2TaskId[ucFbId],
                _prFbg[ucFbgId].prPicHdr[_prFbg[ucFbgId].aucResize2SrcFbId[ucFbId]].ucSeamlessTaskId
               );
            ucFbStatus = FBM_FB_STATUS_EMPTY;
#if 1
            if (_prFbg[ucFbgId].aucResize2FbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
            {
                _FBM_PutFrameBufferToEmptyResize2Q(ucFbgId, ucFbId);
                VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResize2QSemaphore) == OSR_OK);
            }
#endif
        }

        break;

    default:
        ASSERT_FBM(0);
        break;
    }
    
    _prFbg[ucFbgId].aucResize2FbStatus[ucFbId] = ucFbStatus;

    FBM_MUTEX_UNLOCK(ucFbgId);

#if 1
    if ((ucFbStatus == FBM_FB_STATUS_RESIZE_DONE) && !FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE))
    {
        FB_CHG_INFO_T t_fb_info;

        t_fb_info.ucFbgId = ucFbgId;
        t_fb_info.ucFbid  = ucFbId;
        
        if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_FB_READY_IND]))
        {
            ((FBM_FB_READY_IND_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND])(&t_fb_info);
        }
        else
        {
            ASSERT_FBM(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND] == 0);
        }
    }
#endif
}

UCHAR FBM_GetEmptyResizeFrameBuffer(UCHAR ucFbgId, UCHAR ucSrcFbId, UCHAR ucTaskId, UINT32 u4Delay, UCHAR* pucRetValue)
{
    UCHAR ucFbId;

    if (!pucRetValue)
    {
        return FBM_FB_ID_UNKNOWN;
    }
    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    ASSERT_FBM(ucTaskId != 0xFF);

    FBM_MUTEX_LOCK(ucFbgId);
    *pucRetValue = 0;

    if (_prFbg[ucFbgId].prPicHdr[ucSrcFbId].ucSeamlessTaskId != ucTaskId)
    {

        *pucRetValue = 1;
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }
    ucFbId = FBM_FB_ID_UNKNOWN;
    // wait semaphore (Empty)
    _prFbg[ucFbgId].aucResizeBlockFb = ucSrcFbId;
    if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyResizeQSemaphore, X_SEMA_OPTION_WAIT, u4Delay) == OSR_OK)
    {
        // wait semaphore success
        _prFbg[ucFbgId].aucResizeBlockFb = FBM_FB_ID_UNKNOWN;

        if(_prFbg[ucFbgId].prPicHdr[ucSrcFbId].ucSeamlessTaskId != ucTaskId)
        {

            *pucRetValue = 1;
            VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResizeQSemaphore) == OSR_OK);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }

        ucFbId = _FbmGetEmptyFrameBufferFromEmptyResizeQ(ucFbgId);

        // Work around counting semaphore issue
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            LOG(0,"Unexpected error when get empty resize buffer!!!\n");
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }

        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucFbId %d\n",ucFbId);            
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        if (_prFbg[ucFbgId].aucResizeFbStatus[ucFbId] != FBM_FB_STATUS_EMPTY)
        {
            LOG(0,"ucFbgId(%d) ucFbId(%d) ResizeFbStatus(%d)\n",
                    ucFbgId,ucFbId,_prFbg[ucFbgId].aucResizeFbStatus[ucFbId]);
        }
        ASSERT_FBM(_prFbg[ucFbgId].aucResizeFbStatus[ucFbId] == FBM_FB_STATUS_EMPTY);
    }
    else
    {
        // wait semaphore fail
        _prFbg[ucFbgId].aucResizeBlockFb = FBM_FB_ID_UNKNOWN;

        ASSERT_FBM(_prFbg[ucFbgId].hEmptyResizeQSemaphore.i4Count == 0);
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }

    if (ucFbId < FBM_MAX_FB_NS_PER_GROUP)
    {
        _FbmFbLog(ucFbgId,ucFbId, FBM_FB_STATUS_RESIZE, __func__, __LINE__);
        _prFbg[ucFbgId].aucResizeFbStatus[ucFbId] = FBM_FB_STATUS_RESIZE;
        _prFbg[ucFbgId].ucFbResize = ucFbId;
        _prFbg[ucFbgId].aucResizeSrcFbId[ucFbId] = ucSrcFbId;
        _prFbg[ucFbgId].aucResizeTaskId[ucFbId] = ucTaskId;
        _prFbg[ucFbgId].prPicHdr[ucSrcFbId].ucSeamlessResizeFbid = ucFbId;
        _prFbg[ucFbgId].prPicHdr[ucSrcFbId].ucSeamlessResize2Fbid = FBM_FB_ID_UNKNOWN;
		
    }
    else
    {
        ucFbId = FBM_FB_ID_UNKNOWN;
        LOG(1, "[Warning] Get Empty Ref Fail (%d, %d)\n", ucFbgId, ucFbId);
    }

    // [LOG] FBM get empty Ref FB
    SYSLOG(FBM_PREFIX + 10 + ucFbgId, ucFbId, _prFbg[ucFbgId].rResizeEmptyQ.ucCount, 0);

    FBM_MUTEX_UNLOCK(ucFbgId);

    //callback to b2r to update buffer within ready threshold
    if (!FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE))
    {
        FB_CHG_INFO_T t_fb_info;

        t_fb_info.ucFbgId = ucFbgId;
        t_fb_info.ucFbid  = ucFbId;
        
        if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_FB_READY_IND]))
        {
            ((FBM_FB_READY_IND_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND])(&t_fb_info);
        }
        else
        {
            ASSERT_FBM(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND] == 0);
        }
    }

    x_memcpy((void*)&_prFbg[ucFbgId].prPicHdr[ucSrcFbId].rTimeB2rReleaseRzFB, (void*)(&_prFbg[ucFbgId].arRlResizeFb[ucFbId]), sizeof(HAL_TIME_T));
    return ucFbId;

}

UCHAR FBM_GetEmptyResize2FrameBuffer(UCHAR ucFbgId, UCHAR ucSrcFbId, UCHAR ucTaskId, UINT32 u4Delay, UCHAR* pucRetValue)
{
    UCHAR ucRzFbId;

    if (!pucRetValue)
    {
        return FBM_FB_ID_UNKNOWN;
    }
    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    ASSERT_FBM(ucTaskId != 0xFF);

    FBM_MUTEX_LOCK(ucFbgId);
    *pucRetValue = 0;

    if (_prFbg[ucFbgId].prPicHdr[ucSrcFbId].ucSeamlessTaskId != ucTaskId)
    {

        *pucRetValue = 1;
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }
    ucRzFbId = FBM_FB_ID_UNKNOWN;
    // wait semaphore (Empty)
    _prFbg[ucFbgId].aucResize2BlockFb = ucSrcFbId;
    if (_FBM_csema_lock(&_prFbg[ucFbgId].hEmptyResize2QSemaphore, X_SEMA_OPTION_WAIT, u4Delay) == OSR_OK)
    {
        // wait semaphore success
        _prFbg[ucFbgId].aucResize2BlockFb = FBM_FB_ID_UNKNOWN;

        if(_prFbg[ucFbgId].prPicHdr[ucSrcFbId].ucSeamlessTaskId != ucTaskId)
        {

            *pucRetValue = 1;
            VERIFY(_FBM_csema_unlock(&_prFbg[ucFbgId].hEmptyResize2QSemaphore) == OSR_OK);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }

        ucRzFbId = _FbmGetEmptyFrameBufferFromEmptyResize2Q(ucFbgId);

        // Work around counting semaphore issue
        if (ucRzFbId == FBM_FB_ID_UNKNOWN)
        {
            LOG(0,"Unexpected error when get empty resize2 buffer!!!\n");
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }

        if (ucRzFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"ucRzFbId %d\n",ucRzFbId);            
            ASSERT_FBM(ucRzFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        if (_prFbg[ucFbgId].aucResize2FbStatus[ucRzFbId] != FBM_FB_STATUS_EMPTY)
        {
            LOG(0,"ucFbgId(%d) ucRzFbId(%d) Resize2FbStatus(%d)\n",
                    ucFbgId,ucRzFbId,_prFbg[ucFbgId].aucResize2FbStatus[ucRzFbId]);            
        }

        ASSERT_FBM(_prFbg[ucFbgId].aucResize2FbStatus[ucRzFbId] == FBM_FB_STATUS_EMPTY);
    }
    else
    {
        // wait semaphore fail
        _prFbg[ucFbgId].aucResize2BlockFb = FBM_FB_ID_UNKNOWN;
		
		if (_prFbg[ucFbgId].hEmptyResize2QSemaphore.i4Count != 0)
		{
			LOG(0,"ucFbgId %d ResizeQSemaphore.i4Count %d\n",
				ucFbgId,_prFbg[ucFbgId].hEmptyResize2QSemaphore.i4Count);
		}

        ASSERT_FBM(_prFbg[ucFbgId].hEmptyResize2QSemaphore.i4Count == 0);
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FBM_FB_ID_UNKNOWN;
    }

    if (ucRzFbId < FBM_MAX_FB_NS_PER_GROUP)
    {
        _FbmFbLog(ucFbgId,ucRzFbId, FBM_FB_STATUS_RESIZE, __func__, __LINE__);
        _prFbg[ucFbgId].aucResize2FbStatus[ucRzFbId] = FBM_FB_STATUS_RESIZE;
        _prFbg[ucFbgId].ucFbResize2 = ucRzFbId;
        _prFbg[ucFbgId].aucResize2SrcFbId[ucRzFbId] = ucSrcFbId;
        _prFbg[ucFbgId].aucResize2TaskId[ucRzFbId] = ucTaskId;
        _prFbg[ucFbgId].prPicHdr[ucSrcFbId].ucSeamlessResize2Fbid = ucRzFbId;
        _prFbg[ucFbgId].prPicHdr[ucSrcFbId].ucSeamlessResizeFbid = FBM_FB_ID_UNKNOWN;
                		
    }
    else
    {
        ucRzFbId = FBM_FB_ID_UNKNOWN;
        LOG(1, "[Warning] Get Empty Ref Fail (%d, %d)\n", ucFbgId, ucRzFbId);
    }

    // [LOG] FBM get empty Ref FB
    SYSLOG(FBM_PREFIX + 10 + ucFbgId, ucRzFbId, _prFbg[ucFbgId].rResize2EmptyQ.ucCount, 0);

    FBM_MUTEX_UNLOCK(ucFbgId);
#if 0
    //callback to b2r to update buffer within ready threshold
    if (!FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_THUMBNAIL_MODE))
    {
        if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_FB_READY_IND]))
        {
            ((FBM_FB_READY_IND_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND])(ucFbgId);
        }
        else
        {
            ASSERT_FBM(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_FB_READY_IND] == 0);
        }
    }

    x_memcpy((void*)&_prFbg[ucFbgId].prPicHdr[ucSrcFbId].rTimeB2rReleaseRzFB, (void*)(&_prFbg[ucFbgId].arRlResizeFb[ucFbId]), sizeof(HAL_TIME_T));
#endif

    return ucRzFbId;

}



void FBM_GetResizeEmptyQCount(UCHAR ucFbgId, UCHAR* pucCount)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    if (pucCount == NULL)
    {
        return;
    }

    *pucCount = _prFbg[ucFbgId].rResizeEmptyQ.ucCount;
}

BOOL FBM_IsSeamlessFB(UCHAR ucFbgId, UCHAR ucFbId)
{
    BOOL bNeedResize = FALSE;

    if (VERIFY_FBG(ucFbgId))
    {
        return bNeedResize;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if (_prFbg[ucFbgId].fgAutoSeamless || _prFbg[ucFbgId].prPicHdr[ucFbId].fgSeamlessDisp)
    {
        bNeedResize = TRUE;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);
    return bNeedResize;
}

void FBM_SetWfdFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    _prFbg[ucFbgId].u4WfdFlag |= u4Flag;

    _prFbg[ucFbgId].u4_es_cnt = 1;

    FBM_MUTEX_UNLOCK(ucFbgId);
}

INT32 FBM_ChkWfdFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag)
{
    UINT32 ucChk;

    if (VERIFY_FBG(ucFbgId))
    {
        return 0;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if (_prFbg[ucFbgId].u4WfdFlag & u4Flag)
    {
        ucChk = 1;
    }
    else
    {
        ucChk = 0;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucChk;
}

void FBM_ClrWfdFrameBufferFlag(UCHAR ucFbgId, UINT32 u4Flag)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    _prFbg[ucFbgId].u4WfdFlag &= ~u4Flag;

    FBM_MUTEX_UNLOCK(ucFbgId);
}

void FBM_AccumulateWfdFrameCnt(UCHAR ucFbgId)
{
    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    _prFbg[ucFbgId].u4_es_cnt ++;

    FBM_MUTEX_UNLOCK(ucFbgId);
}

UINT32 FBM_WfdFrameStatistics(UCHAR ucFbgId)
{
    UINT32 u4_es_cnt = 0;
    if (VERIFY_FBG(ucFbgId))
    {
        return u4_es_cnt;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    u4_es_cnt = _prFbg[ucFbgId].u4_es_cnt ;

    FBM_MUTEX_UNLOCK(ucFbgId);

    return u4_es_cnt;
}

UCHAR FBM_GetDisplayFB(UCHAR ucFbgId)
{
    UCHAR ucFbId = FBM_FB_ID_UNKNOWN;

    if (VERIFY_FBG(ucFbgId))
    {
        return FBM_FB_ID_UNKNOWN;
    }

    FBM_MUTEX_LOCK(ucFbgId);

    if (_prFbg[ucFbgId].rDisplayQ.ucCount > 0)
    {
        ucFbId = _prFbg[ucFbgId].rDisplayQ.aucQueue[_prFbg[ucFbgId].rDisplayQ.ucReadIdx];
        _prFbg[ucFbgId].rDisplayQ.ucReadIdx =
            (UCHAR)(_prFbg[ucFbgId].rDisplayQ.ucReadIdx + 1) % FBM_MAX_FB_NS_PER_GROUP;
        _prFbg[ucFbgId].rDisplayQ.ucCount--;

        if (ucFbId >= FBM_MAX_FB_NS_PER_GROUP)
        {
            ASSERT_FBM(ucFbId < FBM_MAX_FB_NS_PER_GROUP);
            FBM_MUTEX_UNLOCK(ucFbgId);
            return FBM_FB_ID_UNKNOWN;
        }
        if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_DISPLAYQ)
        {
            LOG(0,"ucFbgId %d ucFbId %d Status %d\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
        }        
        
        ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_DISPLAYQ);

       FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_LOCK); 
       
        _prFbg[ucFbgId].u4DispTag[ucFbId] = OMX_PENDING_DISPTAG;
		
    }

    if ((_prFbg[ucFbgId].ucSeqChg) && (_prFbg[ucFbgId].ucNewFbId == ucFbId))
    {
        _prFbg[ucFbgId].u4FbgFlag |= _prFbg[ucFbgId].u4SeqChgFlag;
        _prFbg[ucFbgId].ucSeqChg = 0;
    }

    FBM_MUTEX_UNLOCK(ucFbgId);

    return ucFbId;
}
BOOL FBM_SetDispTag(UCHAR ucFbgId, UCHAR ucFbId,UINT32 u4Tag)
{
    if((VERIFY_FBG(ucFbgId))||(VERIFY_FB(ucFbgId, ucFbId)))
    {
        return FALSE;
    }

    LOG(2,"omx set tag fbg=%d,fb=%d,Tag=0x%x\n",ucFbgId,ucFbId,u4Tag);
    FBM_MUTEX_LOCK(ucFbgId);
    _prFbg[ucFbgId].u4DispTag[ucFbId]=u4Tag;
    if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_LOCK)
    {
        LOG(0,"ucFbgId %d ucFbId %d Status %d\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
    }
    
    ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK);
    FBM_MUTEX_UNLOCK(ucFbgId);
    return TRUE;
}

BOOL FBM_GetFbByDispTag(UINT32 u4Tag, UCHAR *pucFbgId, UCHAR *pucFbId)
{
    UCHAR ucFbgId;
    UCHAR ucFbId;

    if (pucFbgId == NULL || pucFbId == NULL)
    {
        return FALSE;
    }

    *pucFbgId = FBM_FBG_ID_UNKNOWN;
    *pucFbId = FBM_FB_ID_UNKNOWN;

    for (ucFbgId = 0; ucFbgId < FBG_MAX_NS; ucFbgId++)
    {
        if(VERIFY_FBG(ucFbgId))
        {
            continue;
        }
        
        FBM_MUTEX_LOCK(ucFbgId);
        for (ucFbId=_prFbg[ucFbgId].ucFbNsBase; ucFbId<_prFbg[ucFbgId].ucFbNs; ucFbId++)
        {
            if(_prFbg[ucFbgId].u4DispTag[ucFbId] == u4Tag)
            {
                *pucFbgId = ucFbgId;
                *pucFbId = ucFbId;
                _prFbg[ucFbgId].u4DispTag[ucFbId]=INVALID_DISPTAG;
                
                if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_LOCK)
                {
                    LOG(0,"ucFbgId %d ucFbId %d Status %d\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
                }
                
                ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK);                
                LOG(5,"HWC got fbg=%d,fb=%d,Tag=0x%x\n",ucFbgId,ucFbId,u4Tag);
                 FBM_MUTEX_UNLOCK(ucFbgId);
                 return TRUE;
            }
        }
        if (_prFbg[ucFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE)
        {
            for (ucFbId=_prFbg[ucFbgId].ucFbNsOldBase; ucFbId<_prFbg[ucFbgId].ucFbNsOld; ucFbId++)
            {
                if(_prFbg[ucFbgId].u4DispTag[ucFbId] == u4Tag)
                {
                    *pucFbgId = ucFbgId;
                    *pucFbId = ucFbId;
                    _prFbg[ucFbgId].u4DispTag[ucFbId]=INVALID_DISPTAG;
                    
                    if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_LOCK)
                    {
                        LOG(0,"ucFbgId %d ucFbId %d Status %d\n",ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
                    }
                    
                    ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK);
                    LOG(1,"HWC got fbg=%d,fb=%d,Tag=0x%x\n",ucFbgId,ucFbId,u4Tag);
                     FBM_MUTEX_UNLOCK(ucFbgId);
                     return TRUE;
                }
            }
        }
        FBM_MUTEX_UNLOCK(ucFbgId);
    } 
    
    LOG(1,"HWC get fb failed!!!!  Tag=0x%x\n",u4Tag);
    return FALSE;
}

VOID FBM_ReleaseBufferByDispTag(UINT32 u4Tag)
{
    UCHAR ucFbgId;
    UCHAR ucFbId;

    for (ucFbgId = 0; ucFbgId < FBG_MAX_NS; ucFbgId++)
    {
        if(VERIFY_FBG(ucFbgId)==FALSE)
        {
           FBM_MUTEX_LOCK(ucFbgId);
           for (ucFbId=_prFbg[ucFbgId].ucFbNsBase; ucFbId<_prFbg[ucFbgId].ucFbNs; ucFbId++)
           {
                if(_prFbg[ucFbgId].u4DispTag[ucFbId]==u4Tag)
                {
                   _prFbg[ucFbgId].u4DispTag[ucFbId]=INVALID_DISPTAG;
                   LOG(1,"release fbg=%d,fb=%d,Tag=0x%x\n",ucFbgId,ucFbId,u4Tag);
                   if((_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_LOCK) && !FBM_ChkFrameBufferPicFlag(ucFbgId, ucFbId, FBM_MM_EOS_FLAG | FBM_MM_NOT_DISPLAY_FLAG))
                   {
                       LOG(0,"FBM_ReleaseBufferByDispTag fbg=%d,fb=%d,Tag=0x%x, status:%d\n",ucFbgId,ucFbId,u4Tag,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
                       ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK);
                   }
                   FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
                   FBM_MUTEX_UNLOCK(ucFbgId);
                   return;
                }
           }
           if (_prFbg[ucFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE)
           {
               for (ucFbId=_prFbg[ucFbgId].ucFbNsOldBase; ucFbId<_prFbg[ucFbgId].ucFbNsOld; ucFbId++)
               {
                    if(_prFbg[ucFbgId].u4DispTag[ucFbId]==u4Tag)
                    {
                       _prFbg[ucFbgId].u4DispTag[ucFbId]=INVALID_DISPTAG;
                       LOG(1,"release fbg=%d,fb=%d,Tag=0x%x\n",ucFbgId,ucFbId,u4Tag);
                       if((_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_LOCK) && !FBM_ChkFrameBufferPicFlag(ucFbgId, ucFbId, FBM_MM_EOS_FLAG | FBM_MM_NOT_DISPLAY_FLAG))
                       {
                           LOG(0,"FBM_ReleaseBufferByDispTag fbg=%d,fb=%d,Tag=0x%x, status:%d\n",ucFbgId,ucFbId,u4Tag,_prFbg[ucFbgId].aucFbStatus[ucFbId]);
                           ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK);
                       }
                       FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);                       
                       FBM_MUTEX_UNLOCK(ucFbgId);
                       return;
                    }
               }
           }
           FBM_MUTEX_UNLOCK(ucFbgId);
        }
    }

    LOG(5,"FBM_ReleaseBufferByDispTag don't need Tag=0x%x\n",u4Tag);
}

VOID FBM_ReleaseBufferByIndex(UCHAR ucFbgId, UCHAR ucFbId)
{
    if((VERIFY_FBG(ucFbgId)))
    {
        return;
    }
    FBM_MUTEX_LOCK(ucFbgId);
    
    if (_prFbg[ucFbgId].aucFbStatus[ucFbId] != FBM_FB_STATUS_LOCK)
    {
        LOG(0,"ucFbgId(%d) ucFbId(%d) aucFbStatus(%d)\n",
            ucFbgId,ucFbId,_prFbg[ucFbgId].aucFbStatus[ucFbId]);    
    }
    ASSERT_FBM(_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK);
    FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
    FBM_MUTEX_UNLOCK(ucFbgId);
    return;
}

VOID FBM_FlushLockToEmptyQ(UCHAR ucFbgId)
{
    UCHAR ucFbId;
    if(VERIFY_FBG(ucFbgId)==FALSE)
    {
       FBM_MUTEX_LOCK(ucFbgId);
       for (ucFbId=_prFbg[ucFbgId].ucFbNsBase; ucFbId<_prFbg[ucFbgId].ucFbNs; ucFbId++)
       {
            if (Fb_Using(ucFbgId,ucFbId) && (_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK) &&
                ((_prFbg[ucFbgId].u4DispTag[ucFbId] != INVALID_DISPTAG) || (_prFbg[ucFbgId].u4DispTag[ucFbId] == OMX_PENDING_DISPTAG)))
            {
            	LOG(1,"omx flush lock, fbg:%d, fbid:%d, tag:0x%x\n", ucFbgId, ucFbId, _prFbg[ucFbgId].u4DispTag[ucFbId]);
               _prFbg[ucFbgId].u4DispTag[ucFbId]=INVALID_DISPTAG;
               FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
            }
       }
       if (_prFbg[ucFbgId].eDynamicMode != FBM_DYNAMIC_MODE_NONE)
       {
           for (ucFbId=_prFbg[ucFbgId].ucFbNsOldBase; ucFbId<_prFbg[ucFbgId].ucFbNsOld; ucFbId++)
           {
               if ((_prFbg[ucFbgId].aucFbStatus[ucFbId] == FBM_FB_STATUS_LOCK) &&
                   ((_prFbg[ucFbgId].u4DispTag[ucFbId] != INVALID_DISPTAG) || (_prFbg[ucFbgId].u4DispTag[ucFbId] == OMX_PENDING_DISPTAG)))
               {
                   LOG(1,"omx flush lock, fbg:%d, fbid:%d, tag:0x%x\n", ucFbgId, ucFbId, _prFbg[ucFbgId].u4DispTag[ucFbId]);
                  _prFbg[ucFbgId].u4DispTag[ucFbId]=INVALID_DISPTAG;
                  FBM_SetFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
               }
           }
       }
       FBM_MUTEX_UNLOCK(ucFbgId);
    }

    return;
}

//-------------------------------------------------------------------------
/** FBM_FlushLockFrameBuffer
 *  Flush FBM Group lock frame buffer
 *  @param  ucFbgType   FBG type.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
void FBM_FlushLockFrameBuffer(UCHAR ucFbgId)
{
    // Note about usage
    // When vdp mode change, flush lock frame buffer

    UINT32 u4FbIdx;

    if (VERIFY_FBG(ucFbgId))
    {
        return;
    }
    if (VERIFY_FB_NS(ucFbgId))
    {
        return;
    }

    if (_prFbg[ucFbgId].fgCreateFromInst)
    {
        return;
    }
    FBM_MUTEX_LOCK(ucFbgId);

    // put all lock frame buffer into empty queue
    for (u4FbIdx = _prFbg[ucFbgId].ucFbNsBase;
            ((u4FbIdx < _prFbg[ucFbgId].ucFbNs)&&(u4FbIdx < FBM_MAX_FB_NS_PER_GROUP)); u4FbIdx++)
    {
        // DispTag == invalid disptag B2r flush or,waiting vdec omx flush
      
        if (Fb_Using(ucFbgId,u4FbIdx)&&(_prFbg[ucFbgId].aucFbStatus[u4FbIdx] == FBM_FB_STATUS_LOCK) &&
            ((_prFbg[ucFbgId].u1FbgAppMode != FBM_FBG_APP_OMX_DISP) ||
            ((_prFbg[ucFbgId].u1FbgAppMode == FBM_FBG_APP_OMX_DISP) && (_prFbg[ucFbgId].u4DispTag[u4FbIdx] == INVALID_DISPTAG))))
        {
        
            FBM_SetFrameBufferStatus(ucFbgId, u4FbIdx, FBM_FB_STATUS_EMPTY);
        }
    }

    ASSERT(_prFbg[ucFbgId].hEmptyQSemaphore.hMutex == _prFbg[ucFbgId].hMutex);
    ASSERT(_prFbg[ucFbgId].hEmptyBQSemaphore.hMutex == _prFbg[ucFbgId].hMutex);

    FBM_MUTEX_UNLOCK(ucFbgId);
}

VOID FBM_FrameDisplayStart(UCHAR ucFbgId,UCHAR ucFbId)
{
    if(VERIFY_FBG(ucFbgId) || VERIFY_FB(ucFbgId, ucFbId))
    {
        return;
    }
    if (VERIFY_FB_NS(ucFbgId))
    {
        return;
    }
    
    FBM_MUTEX_LOCK(ucFbgId);
    if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->aau4CbFunc[ucFbgId][FBM_CB_FUNC_FB_DISPLAY_START],
                                 _prFbmCbFunc->aau4CbFuncCRC[ucFbgId][FBM_CB_FUNC_FB_DISPLAY_START]))
    {
         LOG(7,"FBM_FrameDisplayStart(%d,%d)\n",ucFbgId,ucFbId);
		 if((_prFbg[ucFbgId].afgWaitDisplay[ucFbId] == TRUE) && (_prFbg[ucFbgId].ucPlayMode == FBM_FBG_DTV_MODE))
         {
             ((FBM_FB_DISP_START_FUNC)_prFbmCbFunc->aau4CbFunc[ucFbgId][FBM_CB_FUNC_FB_DISPLAY_START])(
                 _prFbg[ucFbgId].u1DecoderSrcId, ucFbgId, ucFbId);
             _prFbg[ucFbgId].afgWaitDisplay[ucFbId] = FALSE;
         }
    }
    FBM_MUTEX_UNLOCK(ucFbgId);
    return;
}

void FBM_SetPtsSync(UCHAR ucFbgId,BOOL fgAvsync)
{
    if((VERIFY_FBG(ucFbgId)))
    {
           return;
    }
    
    _prFbg[ucFbgId].fgPtsSync = fgAvsync;
    return;
}

BOOL FBM_GetPtsSync(UCHAR ucFbgId)
{
    if((VERIFY_FBG(ucFbgId)))
    {
           return FALSE;
    }

    return _prFbg[ucFbgId].fgPtsSync;
}

UCHAR FBM_GetFbRotationStatus(UCHAR ucFbgId,UCHAR ucFbId)
{
    UCHAR ucStatus;
    if(VERIFY_FBG(ucFbgId) || VERIFY_FB(ucFbgId, ucFbId))
     {
         return FB_ROTATION_UNUSE;
     }
    
     if (VERIFY_FB_NS(ucFbgId))
     {
         return FB_ROTATION_UNUSE;
     }
     
     FBM_MUTEX_LOCK(ucFbgId);
     ucStatus = _prFbg[ucFbgId].aucFbRotationStatus[ucFbId];
     FBM_MUTEX_UNLOCK(ucFbgId);
     return ucStatus;
}


inline static UINT32 _FbmGetMVParam(FBM_FBG_T *pFbg, UINT32 *pu4Size, UINT32 *pu4Cnt)
{
    UINT32 u4MvSize = 0,u4MvCnt =0;
    if(pFbg->u4VDecFmt == FBM_VDEC_H264)
    {
        if (pFbg->ucFbgType == FBM_FBG_TYPE_1080HD_RR)
        {
            u4MvSize = pFbg->u4YSize >> 1;
        }
        else
        {
            u4MvSize = pFbg->u4YSize >> 2;
        }
        u4MvCnt = pFbg->u4FbCnt > 4 ? (pFbg->u4FbCnt -4) : pFbg->u4FbCnt;
    }
    else if(pFbg->u4VDecFmt == FBM_VDEC_H265)
    {
        u4MvSize = pFbg->u4YSize >> 4;
        
        u4MvCnt = pFbg->u4FbCnt > 4 ? (pFbg->u4FbCnt -4) : pFbg->u4FbCnt;
    }
    else if(pFbg->u4VDecFmt == FBM_VDEC_VP9)
    {
        u4MvSize = FBM_WORK_BUF_VP9_4K2K_SIZE;
        u4MvCnt = 2;
    }

    if(pu4Size) *pu4Size = u4MvSize;
    if(pu4Cnt)  *pu4Cnt  = u4MvCnt;
    
    return u4MvSize;
}

inline static UINT32 _FbmGetCabacParam(FBM_FBG_T *pFbg,UINT32 *pu4Size, UINT32 *pu4Cnt)
{
    UINT32 u4CabacSize = 0,u4CabacCnt = 0;
    if(pFbg->u4VDecFmt == FBM_VDEC_H264 || pFbg->u4VDecFmt == FBM_VDEC_H265)
    {
        u4CabacSize = FBM_FBG_TYPE_CABAC_SIZE;
        u4CabacCnt = FBM_MAX_CABAC_BUF_NS_PER_GROUP;
    }
    else if(pFbg->u4VDecFmt == FBM_VDEC_VP9)
    {
        u4CabacSize = FBM_FBG_TYPE_CABAC_SIZE * 2;
        u4CabacCnt = FBM_MAX_CABAC_BUF_NS_PER_GROUP;
    }
    
    if(pu4Size) *pu4Size = u4CabacSize;
    if(pu4Cnt)  *pu4Cnt  = u4CabacCnt;

    return u4CabacSize;
}

static void _FbmCalculateYCSize(FBM_FBG_T *pFbg,UINT32 u4Width, UINT32 u4Height)
{
    UINT32 u4YSize,u4CSize;
    UINT32 u4MaxYSize,u4MaxCSize,uMax4Pitch,u4MaxWidth, u4MaxHeight;

    u4MaxWidth = FBM_MEMUNIT_FRAME_MAX_WIDTH;
    u4MaxHeight = FBM_MEMUNIT_FRAME_MAX_HEIGHT;
    u4MaxYSize = u4MaxWidth * u4MaxHeight;    
    u4MaxCSize = u4MaxYSize >> 1;

    u4YSize = FBM_ALIGN_MASK(u4Width, FBM_B2R_H_PITCH) * FBM_ALIGN_MASK(u4Height, FBM_B2R_H_PITCH);
    u4CSize = u4YSize >> 1;
    
    if(pFbg->fg10Bit)
    {
        u4MaxYSize = (u4MaxYSize *5) >> 2;
        u4MaxCSize = (u4MaxCSize *5) >> 2;
        u4YSize = (u4YSize * 5) >> 2;
        u4CSize = (u4CSize * 5) >> 2;
        
    }

    if (pFbg->ucFbgCm == FBM_CM_422)
    {
        u4CSize *= 2;    // 420 to 422
        u4MaxCSize *= 2; 
    }
    else if(pFbg->ucFbgCm == FBM_CM_444)
    {
        u4CSize *= 4;    // 420 to 444
        u4MaxCSize *= 4;
    }

    uMax4Pitch = FBM_FMG_Y_ALIGMENT + 1 + FBM_FMG_Y_ALIGMENT;
    u4MaxYSize =  FBM_ALIGN_MASK(u4MaxYSize,uMax4Pitch);
    u4MaxCSize =  FBM_ALIGN_MASK(u4MaxCSize,uMax4Pitch);

    if(u4YSize> (u4MaxYSize>>1)) // level 1
    {
        pFbg->u4YSize = u4MaxYSize;
        pFbg->u4CSize = u4MaxCSize;
        pFbg->u4FbWidth = FBM_ALIGN_MASK(u4MaxWidth, FBM_B2R_H_PITCH);
        pFbg->u4FbHeight = FBM_ALIGN_MASK(u4MaxHeight, FBM_B2R_H_PITCH);
    }
    else  //levle 2
    {
        pFbg->u4YSize = (u4MaxYSize>>1);
        pFbg->u4CSize = (u4MaxCSize>>1);
    }
    
    pFbg->u4FbWidth = FBM_ALIGN_MASK(u4Width, FBM_B2R_H_PITCH);
    pFbg->u4FbHeight = FBM_ALIGN_MASK(u4Height, FBM_B2R_H_PITCH);
    
    LOG(2,"_FbmCalculateYCSize(Width:%d, Height:%d, FbWidht:%d, FbHeight:%d, CM:%d, 10bit:%d, YSize:%d, CSize:%d)\n",
        u4Width,u4Height,pFbg->u4FbWidth,pFbg->u4FbHeight,pFbg->ucFbgCm,pFbg->fg10Bit,pFbg->u4YSize,pFbg->u4CSize);
    return;
}


static UINT32 u4DeFaultWorkFlag = 0;
static FBM_FBG_T *pDefaultFbg = NULL;
static FBM_MEMUNIT arDefaultMemPoolList[FBM_MEMUNIT_LIST_MAX];
static UINT32 _FbmMemUnitExtenBufFlag(FBM_FBG_T *pFbg)
{
    UINT32 u4Flag;

     u4Flag = FBM_MEMUNIT_USETYPE_WORK;
     if(pFbg->u4VDecFmt == FBM_VDEC_H265 || pFbg->u4VDecFmt == FBM_VDEC_H264 || pFbg->u4VDecFmt ==FBM_VDEC_VP9)
     {
        u4Flag |= FBM_MEMUNIT_USETYPE_MV;
        u4Flag |= FBM_MEMUNIT_USETYPE_CABAC;
     }
     
     if(pFbg->u4VDecFmt == FBM_VDEC_RV && pFbg->fgRPRMode)
     {
        u4Flag |= FBM_MEMUNIT_USETYPE_RPR;
     }
    
     if(pFbg->fgUFO || (pFbg->u4FbgFlag & FBM_FLAG_THUMBNAIL_MODE))
     {
        u4Flag |= FBM_MEMUNIT_USETYPE_EXT ;
     }
     
     if(u4DeFaultWorkFlag != 0)
     {
        u4Flag = u4DeFaultWorkFlag;
     }
     return u4Flag;
}

static UINT32 _FbmMemUnitMemParam(FBM_FBG_T *pFbg, 
    UINT32 *pu4Size, UINT32*pu4Count, UINT32 u4Flag,BOOL fgTotal)
{
    UINT32 u4MemSize = 0, u4TempSize=0, u4MemCnt=0;
    UINT32 u4WorkFlag = _FbmMemUnitExtenBufFlag(pFbg);
    
    if(u4Flag & FBM_MEMUNIT_USETYPE_WORK)
    {
        if(pFbg->u4VDecFmt == FBM_VDEC_H264 ||
            pFbg->u4VDecFmt == FBM_VDEC_H265)
        {
            u4MemSize += FBM_FMG_Y_ALIGMENT + 1;
        }
        else
        {
            u4MemSize += FMB_MEMUNIT_MIN_WORKBUF_SIZE;
        }
        u4MemCnt += 1;
    }

    if(u4Flag & FBM_MEMUNIT_USETYPE_Y)
    {
        if(fgTotal)
        {
            u4MemSize += pFbg->u4YSize * pFbg->u4FbCnt;
        }
        else
        {
            u4MemSize += pFbg->u4YSize;
        }
        
        u4MemCnt += pFbg->u4FbCnt;
    }
    
    if(u4Flag & FBM_MEMUNIT_USETYPE_C)
    {
        if(fgTotal)
        {
            u4MemSize += pFbg->u4CSize * pFbg->u4FbCnt;
        }
        else
        {
            u4MemSize += pFbg->u4CSize;
        }
        u4MemCnt += pFbg->u4FbCnt;
    }
    
    if((u4Flag & FBM_MEMUNIT_USETYPE_EXT) && (u4WorkFlag & FBM_MEMUNIT_USETYPE_EXT))
    {
        u4TempSize = FBM_ALIGN_MASK((pFbg->u4YSize>>8),FBM_FMG_Y_ALIGMENT) + FBM_ALIGN_MASK((pFbg->u4CSize>>8),FBM_FMG_Y_ALIGMENT);
        if(fgTotal)
        {
            u4MemSize += pFbg->u4FbCnt * u4TempSize;
        }
        else
        {
            u4MemSize += u4TempSize;
        }
        u4MemCnt += pFbg->u4FbCnt;
    }
    
    if((u4Flag & FBM_MEMUNIT_USETYPE_MV) && (u4WorkFlag & FBM_MEMUNIT_USETYPE_MV)) 
    {
        UINT32 u4MvCnt = 0;
        _FbmGetMVParam(pFbg,&u4TempSize,&u4MvCnt);
        u4TempSize =  FBM_ALIGN_MASK(u4TempSize,FBM_FMG_Y_ALIGMENT);
        if(fgTotal)
        {
            u4MemSize += u4MvCnt*u4TempSize;
        }
        else
        {
            u4MemSize += u4TempSize;
        }
        
        u4MemCnt += u4MvCnt;
    }
    
    if((u4Flag & FBM_MEMUNIT_USETYPE_CABAC) && (u4WorkFlag & FBM_MEMUNIT_USETYPE_CABAC))
    {
        UINT32 u4CabacCnt = 0;
        _FbmGetCabacParam(pFbg,&u4TempSize,&u4CabacCnt);
        u4TempSize =  FBM_ALIGN_MASK(u4TempSize,FBM_FMG_Y_ALIGMENT);
        if(fgTotal)
        {
            u4MemSize += u4CabacCnt * u4TempSize;
        }
        else
        {
            u4MemSize += u4TempSize;
        }
        u4MemCnt += u4CabacCnt;
    }

    if((u4Flag & FBM_MEMUNIT_USETYPE_RPR) && (u4WorkFlag & FBM_MEMUNIT_USETYPE_RPR))
    {
        u4TempSize = FBM_ALIGN_MASK(FBM_FBG_TYPE_PAL_Y_SIZE,FBM_FMG_Y_ALIGMENT) + FBM_ALIGN_MASK(FBM_FBG_TYPE_PAL_C_SIZE,FBM_FMG_Y_ALIGMENT);
        u4MemSize += u4TempSize * 3;
        u4MemCnt += 1;
        //u4MemSize += ((FBM_FBG_TYPE_PAL_Y_SIZE+FBM_FBG_TYPE_PAL_C_SIZE)<<1);
    }

    if(pu4Size) *pu4Size = u4MemSize;
    if(pu4Count) *pu4Count = u4MemCnt;
    
    return u4MemSize;

}


static BOOL _FBMMemUnitMatch(UINT32 u4UnitFlag,UINT32 u4MatchFlag,FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    BOOL fgMatch =FALSE;
    if(eSearchType == MEMUNIT_SEARCH_TYPE_OR)
    {
        fgMatch =  (u4UnitFlag & u4MatchFlag) != 0;
    }
    else if(eSearchType == MEMUNIT_SEARCH_TYPE_AND)
    {
        fgMatch =  (u4UnitFlag & u4MatchFlag) == u4MatchFlag;
    }
    else if(eSearchType == MEMUNIT_SEARCH_TYPE_EQ)
    {
        fgMatch = u4UnitFlag == u4MatchFlag;
    }
    else if(eSearchType == MEMUNIT_SEARCH_TYPE_NOR)
    {
        fgMatch = (u4UnitFlag&u4MatchFlag) == 0;
    }

    return fgMatch;
    
}

static VOID _FbmMemUnitPrint(FBM_MEMUNIT *prUnitList,UCHAR *pzPrex)
{
    FBM_MEMUNIT *pRetUnit = prUnitList;
    UINT8 UnitIndex = 0;//,uIdx;
    /*
    UCHAR *ucUnitTypeStr[FBM_MEMUNIT_USETYPE_MAX -FBM_MEMUNIT_USETYPE_START +1] =
        {
            "Y","C","Ext","MV","CABAC","RPR","WORK"
        };
    */
    LOG(2,"%s:\n",pzPrex);
    while(pRetUnit->u4StartAddr != 0)
    {  
       LOG(2,"MemUnit[%d] Start:0x%x, User:0x%x, End:0x%x, Usage:0x%x/0x%x, Flag=0x%x\n",\
        UnitIndex,pRetUnit->u4StartAddr,pRetUnit->u4UsedAddr,pRetUnit->u4EndAddr,\
        pRetUnit->u4UsedAddr - pRetUnit->u4StartAddr,pRetUnit->u4EndAddr - pRetUnit->u4StartAddr,pRetUnit->u4Flag);
       #if 0
       for(uIdx = FBM_MEMUNIT_USETYPE_START; uIdx <= FBM_MEMUNIT_USETYPE_MAX; uIdx++)
       {
          if(pRetUnit->u4Flag&(1<<uIdx))
          {
              Printf("[%s]",ucUnitTypeStr[uIdx-FBM_MEMUNIT_USETYPE_START]);
          }
       }
       Printf("\n");
       #endif
       pRetUnit++;
       UnitIndex++;
    }
}

static FBM_MEMUNIT * _FBMMemUnitSearch(FBM_MEMUNIT *prUnitList,UINT32 u4SearcFlag,FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    FBM_MEMUNIT *pRetUnit = NULL;
    UINT8 UnitIndex = 0;
    while(prUnitList[UnitIndex].u4StartAddr != 0)
    {
        if(_FBMMemUnitMatch(prUnitList[UnitIndex].u4Flag,u4SearcFlag,eSearchType))
        {
            pRetUnit =&prUnitList[UnitIndex];
            break;
        }
        UnitIndex++;
    }
    
    return pRetUnit;
}

static FBM_MEMUNIT * _FBMMemUnitMemSearch(FBM_MEMUNIT *prUnitList,UINT32 u4ReqMemSize,UINT32 u4SearcFlag,FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    FBM_MEMUNIT *pRetUnit = prUnitList;
    
    do
    {
        pRetUnit = _FBMMemUnitSearch(pRetUnit,u4SearcFlag,eSearchType);
        if(pRetUnit)
        {
           if(pRetUnit->u4EndAddr - pRetUnit->u4UsedAddr >= u4ReqMemSize)
           {
               break;
           }
           else
           {
               pRetUnit++;
               continue;
           }
        }
    }while(pRetUnit);
    
    return pRetUnit;
}

static UINT32 _FBMMemUnitFreeSize(FBM_MEMUNIT *prUnitList,UINT32 u4SearcFlag, FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    FBM_MEMUNIT *pRetUnit = prUnitList;
    UINT32 u4FreeSize = 0;
    
    do
    {
        pRetUnit = _FBMMemUnitSearch(pRetUnit,u4SearcFlag,eSearchType);
        if(pRetUnit)
        {
            u4FreeSize += pRetUnit->u4EndAddr - pRetUnit->u4UsedAddr;
            pRetUnit++;
        }
    }while(pRetUnit);

    return u4FreeSize;
}

//un used mem unit function
#if 0
static FBM_MEMUNIT * _FBMMemUnitMemMatch(FBM_MEMUNIT *prUnitList,UINT32 u4MatchSize,UINT32 u4SearcFlag,FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    FBM_MEMUNIT *pRetUnit = prUnitList;
    UINT32 u4Diff= (UINT32)-1,u4UnUsedSize=0;
    FBM_MEMUNIT *pMatchUnit =NULL;
    do
    {
        pRetUnit = _FBMMemUnitSearch(pRetUnit,u4SearcFlag,eSearchType);
        if(pRetUnit)
        {
           u4UnUsedSize = pRetUnit->u4EndAddr - pRetUnit->u4UsedAddr;
           if(u4UnUsedSize >= u4MatchSize && (u4UnUsedSize-u4MatchSize)<u4Diff)
           {
               pMatchUnit = pRetUnit;
               u4Diff = u4UnUsedSize-u4MatchSize;
           }
           pRetUnit++;
        }
    }while(pRetUnit);
    
    return pMatchUnit;
}

static BOOL _FBMMemUnitOverlap(FBM_MEMUNIT *prUnitList, UINT32 u4FlagA, UINT32 u4FlagB,FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    FBM_MEMUNIT *pRetUnit = prUnitList;
    BOOL  fgOverlap = FALSE;

    do
    {
        pRetUnit = _FBMMemUnitSearch(pRetUnit,u4FlagA,eSearchType);
        if(pRetUnit)
        {
            if(_FBMMemUnitMatch(pRetUnit->u4Flag,u4FlagB,eSearchType))
            {
                fgOverlap = TRUE;
                break;
            }
            else
            {
                pRetUnit ++;
            }
        }
        
    }while(pRetUnit);

    return fgOverlap;
}

static void _FBMMemUnitReset(FBM_MEMUNIT *prUnitList,UINT32 u4SearcFlag,FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    FBM_MEMUNIT *pRetUnit = prUnitList;
    do
    {
        pRetUnit = _FBMMemUnitSearch(pRetUnit,u4SearcFlag,eSearchType);
        if(pRetUnit)
        {
            pRetUnit->u4UsedAddr = pRetUnit->u4StartAddr;
            pRetUnit++;
            continue;
        }
    }while(pRetUnit);

    return;
}
#endif

static VOID _FBMMemUnitAppend(FBM_MEMUNIT *prUnitList,FBM_MEMUNIT *pAppendUnit, BOOL fgTotal)
{
    
    while(prUnitList->u4StartAddr != 0) prUnitList++;

    if(fgTotal == FALSE)
    {
       *prUnitList = *pAppendUnit;
    }
    else
    {
       while(pAppendUnit->u4StartAddr != 0)
       {
           *prUnitList++ = *pAppendUnit++;
       }
    }
    return;
}

static UINT32 _FBMMemUnitCount(FBM_MEMUNIT *prUnitList,UINT32 u4SearcFlag,FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    FBM_MEMUNIT *pRetUnit = prUnitList;
    UINT32 u4UnitCnt =  0;
    do
    {
        pRetUnit = _FBMMemUnitSearch(pRetUnit,u4SearcFlag,eSearchType);
        if(pRetUnit)
        {
            u4UnitCnt ++;
            pRetUnit ++;
            continue;
        }
    }while(pRetUnit);

    return u4UnitCnt;
}

static UINT32 _FBMMemUnitMemOccupy(FBM_MEMUNIT *prFromList,FBM_MEMUNIT *prToList, UINT32 u4OccupySize,
    UINT32 u4SearcFlag, FBM_MEMUNIT_SEARCH_TYPE eSearchType)
{
    FBM_MEMUNIT *pRetUnit = prFromList;
    UINT32 u4CopyCnt = 0, u4OccupedSize = 0, u4UnitDataSize=0 , u4OccupeUnitSize=0;
    do
    {
        pRetUnit = _FBMMemUnitSearch(pRetUnit,u4SearcFlag,eSearchType);
        if(pRetUnit)
        {
            u4UnitDataSize = pRetUnit->u4EndAddr - pRetUnit->u4UsedAddr;
            u4OccupeUnitSize =  u4UnitDataSize > u4OccupySize ? u4OccupySize : u4UnitDataSize;
            if(u4OccupeUnitSize > 0)
            {
                prToList[u4CopyCnt].u4StartAddr = pRetUnit->u4UsedAddr;
                prToList[u4CopyCnt].u4EndAddr = pRetUnit->u4UsedAddr + u4OccupeUnitSize;
                prToList[u4CopyCnt].u4UsedAddr = pRetUnit->u4UsedAddr;
                prToList[u4CopyCnt].u4Flag = u4SearcFlag;
                pRetUnit->u4UsedAddr += u4OccupeUnitSize;
                u4OccupedSize += u4OccupeUnitSize;
                u4OccupySize -= u4OccupeUnitSize;
                u4CopyCnt ++;
            }
            pRetUnit ++;
        }
    }while(u4OccupySize > 0 && pRetUnit);
    return u4OccupedSize;
}

static BOOL _FbmMemUnitCreateUnitList(FBM_FBG_T *pFbg, FBM_MEMUNIT *prMemPoolList, FBM_MEMUNIT *prMemUnitList, BOOL fgTest)
{
   UINT32 u4Idx,u4ReqSize,u4HaveSize,u4UnitCnt;
   FBM_MEMUNIT *pUnitList = prMemUnitList;
   for(u4Idx = FBM_MEMUNIT_USETYPE_START; u4Idx <= FBM_MEMUNIT_USETYPE_MAX; u4Idx++)
   {
       u4ReqSize = _FbmMemUnitMemParam(pFbg, NULL, NULL, (1 << u4Idx),TRUE);

       if(((1 << u4Idx) == FBM_MEMUNIT_USETYPE_WORK) && pFbg->fgAdjustWorkBuf == TRUE)
       {
           u4ReqSize = FBM_MEMUNIT_MAX_WORKBUF_SIZE;
       }
       
       if(u4ReqSize >0)
       {
           if(_FBMMemUnitFreeSize(prMemPoolList,(1 << u4Idx),MEMUNIT_SEARCH_TYPE_EQ) > 0) // default use special mem firstly
           {
               u4HaveSize = _FBMMemUnitMemOccupy(prMemPoolList,pUnitList,u4ReqSize,(1 << u4Idx),MEMUNIT_SEARCH_TYPE_EQ);
               u4UnitCnt = _FBMMemUnitCount(pUnitList, 0, MEMUNIT_SEARCH_TYPE_AND);
               pUnitList += u4UnitCnt;
               u4ReqSize -= u4HaveSize;
           }
       }
       
       if(u4ReqSize > 0)
       {
           if(_FBMMemUnitFreeSize(prMemPoolList,(1 << u4Idx),MEMUNIT_SEARCH_TYPE_OR) > 0)//  use common mem secondly
           {
               u4HaveSize = _FBMMemUnitMemOccupy(prMemPoolList,pUnitList,u4ReqSize,(1 << u4Idx),MEMUNIT_SEARCH_TYPE_OR);
               u4UnitCnt = _FBMMemUnitCount(pUnitList, 0, MEMUNIT_SEARCH_TYPE_AND);
               pUnitList += u4UnitCnt;
               u4ReqSize -= u4HaveSize;
           }
       }
       
       if(u4ReqSize > 0)  // both special  and common memory not enough.
       {
           if(((1 << u4Idx) != FBM_MEMUNIT_USETYPE_WORK) || pFbg->fgAdjustWorkBuf == FALSE)
           {
               u4ReqSize =  _FbmMemUnitMemParam(pFbg, NULL, NULL, (1 << u4Idx), TRUE);
               u4HaveSize = _FBMMemUnitFreeSize(prMemPoolList,(1 << u4Idx),MEMUNIT_SEARCH_TYPE_OR);
               u4HaveSize += _FBMMemUnitFreeSize(prMemUnitList,(1 << u4Idx),MEMUNIT_SEARCH_TYPE_OR);
               if(fgTest == FALSE)
               {
                   LOG(0,"_FbmMemUnitCreateUnitList(Fbg:%d,FbCnt:%d,YSize:0x%x,CSize:0x%x) 0x%x Memory alloc fail(0x%x/0x%x)\n",
                       pFbg->ucFbgId, pFbg->u4FbCnt, pFbg->u4YSize, pFbg->u4CSize,(1 << u4Idx),u4HaveSize,u4ReqSize);
               }
               else
               {
                   LOG(2,"_FbmMemUnitCreateUnitList(Fbg:%d,FbCnt:%d,YSize:0x%x,CSize:0x%x) 0x%x Memory alloc fail(0x%x/0x%x)\n",
                        pFbg->ucFbgId, pFbg->u4FbCnt, pFbg->u4YSize, pFbg->u4CSize,(1 << u4Idx),u4HaveSize,u4ReqSize);
               }
               return FALSE;
           }
           else 
           {
               u4ReqSize =  _FbmMemUnitMemParam(pFbg, NULL, NULL, (1 << u4Idx), TRUE);
               u4HaveSize = _FBMMemUnitFreeSize(prMemUnitList,(1 << u4Idx),MEMUNIT_SEARCH_TYPE_OR);
               if(u4ReqSize > u4HaveSize)
               {
                   if(fgTest == FALSE)
                   {
                       LOG(0,"_FbmMemUnitCreateUnitList(Fbg:%d,FbCnt:%d,YSize:0x%x,CSize:0x%x) workbuf maybe not enough (0x%x/0x%x)\n",
                                     pFbg->ucFbgId, pFbg->u4FbCnt, pFbg->u4YSize, pFbg->u4CSize, u4HaveSize,u4ReqSize);
                   }
                   else
                   {
                       LOG(2,"_FbmMemUnitCreateUnitList(Fbg:%d,FbCnt:%d,YSize:0x%x,CSize:0x%x) workbuf maybe not enough (0x%x/0x%x)\n",
                                      pFbg->ucFbgId, pFbg->u4FbCnt, pFbg->u4YSize, pFbg->u4CSize, u4HaveSize,u4ReqSize);
                   }
               }
           }
       }
   }

   return TRUE;
}

static UINT32 _FBMMemUnitFbMaxCnt(FBM_FBG_T *pFbg)
{
    UINT32 u4FbMaxCnt= 0;

    switch (pFbg->u4VDecFmt)
    {  
        case FBM_VDEC_MPEG2:
        {
            u4FbMaxCnt = 6;      
            if(pFbg->u1FbgAppMode == FBM_FBG_APP_MHP)
            {
                u4FbMaxCnt = 2;
            }
        }
        break;
        
        case FBM_VDEC_H264:
        case FBM_VDEC_H265:
        {
            if (pFbg->u1FbgAppMode == FBM_FBG_APP_SKYPE)
            {
                u4FbMaxCnt = 7;
            }
        }
        break;
        case FBM_VDEC_JPEG:
        {
            u4FbMaxCnt = 4; 
        }
        break;
        case FBM_VDEC_VC1:
        {
            u4FbMaxCnt = 5;
        }
        break;
        case FBM_VDEC_RV:
        {
            u4FbMaxCnt = 6;
        }
        break;
        case FBM_VDEC_VP6:
        {
            u4FbMaxCnt = 5;
        }
        break;
        case FBM_VDEC_VP8:
        {
            u4FbMaxCnt = 6;
        }
        break;
        case FBM_VDEC_MPEG4:
        {
            u4FbMaxCnt = 6;
        }
        break;
        case FBM_VDEC_VP9:
        {
            u4FbMaxCnt = 11;
        }
        break;
        case FBM_VDEC_AVS:
        {
            u4FbMaxCnt = 5;
        }
        break;		
        default:
        {
            u4FbMaxCnt = 0;
        }
        break;
    }
    return u4FbMaxCnt;
}

static UINT32 _FBMMemUnitCalculateFbCount(FBM_FBG_T *pFbg,FBM_MEMUNIT *prMemPoolList)
{
    UINT32 u4YCnt,u4CCnt,u4FbCnt=0,u4MvCnt=0;
    UINT32 u4YMemSize,u4CMemSize,u4TotalYCSize,u4TotalSize;
    UINT32 u4NeedSize,u4ExtBufFlag;

    u4ExtBufFlag = _FbmMemUnitExtenBufFlag(pFbg);
    u4TotalSize = _FBMMemUnitFreeSize(prMemPoolList, 0, MEMUNIT_SEARCH_TYPE_AND);
    u4YMemSize = _FBMMemUnitFreeSize(prMemPoolList,FBM_MEMUNIT_USETYPE_Y,MEMUNIT_SEARCH_TYPE_EQ);
    u4CMemSize = _FBMMemUnitFreeSize(prMemPoolList,FBM_MEMUNIT_USETYPE_C,MEMUNIT_SEARCH_TYPE_EQ);
    u4TotalYCSize = _FBMMemUnitFreeSize(prMemPoolList,FBM_MEMUNIT_USETYPE_YC,MEMUNIT_SEARCH_TYPE_OR);

    u4TotalYCSize -= (u4YMemSize + u4CMemSize);
    u4YCnt = u4YMemSize/pFbg->u4YSize;
    u4CCnt = u4CMemSize/pFbg->u4CSize;
    u4FbCnt = u4YCnt > u4CCnt ? u4CCnt : u4YCnt;
    u4FbCnt += u4TotalYCSize/(pFbg->u4YSize + pFbg->u4CSize);
    pFbg->u4FbCnt = u4FbCnt;
    pFbg->fgAdjustWorkBuf = FALSE;
    u4NeedSize = _FbmMemUnitMemParam(pFbg, &u4NeedSize, NULL, FBM_MEMUNIT_USETYPE_ALL, TRUE);
    LOG(1,"CalculateFbCount Fbg=%d, TotalMemory=0x%x,NeedMemory=0x%x, FbCnt=%d Before Adjust\n",\
        pFbg->ucFbgId, u4TotalSize, u4NeedSize, pFbg->u4FbCnt);

#if 0
    while(u4NeedSize > u4TotalSize && pFbg->u4FbCnt > 0)
    {
        pFbg->u4FbCnt--;
        u4NeedSize = _FbmMemUnitMemParam(pFbg, &u4NeedSize, NULL, FBM_MEMUNIT_USETYPE_ALL, TRUE);
    }
#else
    {
        FBM_MEMUNIT *pPoolList, *pUnitList;
        pPoolList = (FBM_MEMUNIT *) x_mem_alloc_virtual(sizeof(FBM_MEMUNIT)*FBM_MEMUNIT_LIST_MAX);
        pUnitList = (FBM_MEMUNIT *) x_mem_alloc_virtual(sizeof(FBM_MEMUNIT)*FBM_MEMUNIT_LIST_MAX);
        if(pPoolList == NULL || pUnitList == NULL)
        {
            pFbg->u4FbCnt = 0;
            LOG(0,"_FBMMemUnitCalculateFbCount(fbg:%d) alloc FBM_MEMUNIT memory fail\n", pFbg->ucFbgId);
            return 0;
        }
        
        while(pFbg->u4FbCnt > 0)
        {
            x_memset(pPoolList,0,sizeof(FBM_MEMUNIT)*FBM_MEMUNIT_LIST_MAX);
            x_memset(pUnitList,0,sizeof(FBM_MEMUNIT)*FBM_MEMUNIT_LIST_MAX);
            _FBMMemUnitAppend(pPoolList,prMemPoolList,TRUE);
            if(_FbmMemUnitCreateUnitList(pFbg,pPoolList,pUnitList,TRUE)) // only check memory enought or not
            {
               break;
            }
            else
            {
                pFbg->u4FbCnt --;
            }
        }
        
        x_mem_free(pPoolList);
        x_mem_free(pUnitList);
    }
#endif

    LOG(1,"CalculateFbCount Fbg=%d, TotalMemory=0x%x,NeedMemory=%d, FbCnt=%d After Adjust\n",\
        pFbg->ucFbgId, u4TotalSize, u4NeedSize, pFbg->u4FbCnt);

    u4FbCnt = _FBMMemUnitFbMaxCnt(pFbg);

    if(u4FbCnt != 0)
    {
        if(pFbg->u4FbCnt > u4FbCnt)
        {
            LOG(1,"CalculateFbCount Fbg=%d, FbCnt %d change to %d for codec %d\n",
                pFbg->ucFbgId, pFbg->u4FbCnt, u4FbCnt, pFbg->u4VDecFmt);
        }
        else if(pFbg->u4FbCnt < u4FbCnt)
        {
           LOG(0,"CalculateFbCount Fbg=%d,EsId=%d,Codec=%d,Width=%d,Height=%d,Fb(%d<%d) Fail !!!!!\n",
            pFbg->ucFbgId,pFbg->u1DecoderSrcId,pFbg->u4VDecFmt,pFbg->u4FbWidth, pFbg->u4FbHeight,pFbg->u4FbCnt,u4FbCnt);
           pFbg->u4FbCnt = 0;
        }
    }
    
    if(pFbg->u4FbCnt > (FBM_MAX_FB_NS_PER_GROUP/2)-1)
    {
        pFbg->u4FbCnt = (FBM_MAX_FB_NS_PER_GROUP/2)-1;
        LOG(1,"CalculateFbCount Fbg=%d, FbCnt Cnage to %d by group limit\n", pFbg->ucFbgId, pFbg->u4FbCnt);
    }

    _FbmMemUnitMemParam(pFbg, NULL, &u4MvCnt, FBM_MEMUNIT_USETYPE_MV, FALSE);
    pFbg->ucMvBufNs = u4MvCnt;
    LOG(1,"CalculateFbCount Fbg %d, MvCnt=%d\n", pFbg->ucFbgId,pFbg->ucMvBufNs);
    return pFbg->u4FbCnt;
}

static UINT32 _FBMMemUnitAllocMemory(FBM_MEMUNIT *prMemUnitList, UINT32 u4AllocSize,
    UINT32 *pu4BaseAddr, UINT32 u4AllocCnt, UINT32 u4MemAlignment, UINT32 u4Flag)
{
    FBM_MEMUNIT *pMemUnit = prMemUnitList;
    UINT32 u4Addr, u4SearchFlag = MEMUNIT_SEARCH_TYPE_EQ; // default use special memory.
    UINT32 u4Index = 0;

    while(u4Index < u4AllocCnt)
    {
        pMemUnit = _FBMMemUnitMemSearch(pMemUnit,u4AllocSize,u4Flag,u4SearchFlag);
        if(pMemUnit)
        {
            u4Addr = FBM_ALIGN_MASK(pMemUnit->u4UsedAddr,u4MemAlignment);
            if(u4Addr + u4AllocSize > pMemUnit->u4EndAddr)
            {
                LOG(0,"_FBMMemUnitAllocMemory(%d/%d,0x%x) 0x%x out range (0x%x--0x%x) try next unit\n",
                    u4Index,u4AllocCnt,u4Flag,u4Addr + u4AllocSize,pMemUnit->u4StartAddr,pMemUnit->u4EndAddr);
                pMemUnit ++; // try next
            }
            else
            {
                pu4BaseAddr[u4Index] = u4Addr;
                pMemUnit->u4UsedAddr = u4Addr + u4AllocSize;
                u4Index ++;
            }
        }
        else
        {
            if(u4SearchFlag == MEMUNIT_SEARCH_TYPE_EQ)
            {
                u4SearchFlag = MEMUNIT_SEARCH_TYPE_OR; // if special memory not enough, use common memory.
                pMemUnit = prMemUnitList;
            }
            else  // common memory also enough, return
            {
                if(u4Index == 0)
                {
                    LOG(1,"_FBMAllocFromMemUnit No Memory Unit for 0x%x,Size\n",u4Flag);
                }
                else
                {
                    LOG(1,"_FBMAllocFromMemUnit(%d,0x%x) no enough\n",u4Index,u4Flag);
                }
                break;
            }
        }
    }
    
    return u4Index;
}

static BOOL _FbmMemUnitAssignFbgMemory(FBM_FBG_T *pFbg,FBM_MEMUNIT *prMemUnitList,
    UINT8 ucBaseIdx, UINT32 u4AssignCnt,UINT32 u4AssignFlag)
{
    UINT32 u4AllocCnt=0,u4MemSize =0,u4MemCnt= 0,u4FbIdx=0;
    
    // Assign Y memory;    
    if(u4AssignFlag & FBM_MEMUNIT_USETYPE_Y)
    {
        u4MemSize = pFbg->u4YSize;
        if(u4AssignCnt > pFbg->u4FbCnt || (ucBaseIdx + u4AssignCnt) > FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc Y Count (%d,%d,%d) Error\n",pFbg->ucFbgId,ucBaseIdx,u4AssignCnt,pFbg->u4FbCnt);
            return FALSE;
        }
        else if(u4AssignCnt != 0)
        {
            u4MemCnt = u4AssignCnt;
        }
        else  // u4AssignCnt is 0
        {
            u4MemCnt = pFbg->u4FbCnt;
        }
        
        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList,u4MemSize,pFbg->au4AddrY + ucBaseIdx,
            u4MemCnt,FBM_FMG_Y_ALIGMENT,FBM_MEMUNIT_USETYPE_Y);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc Y error (%d/%d)\n",pFbg->ucFbgId,u4AllocCnt,u4MemCnt);
            return FALSE;
        }

        for(u4FbIdx = ucBaseIdx; u4FbIdx < ucBaseIdx + u4MemCnt; u4FbIdx++)
        {
            pFbg->au4SizeY[u4FbIdx] = u4MemSize;
        }
    }
    
    // Assign C memory;
    if(u4AssignFlag & FBM_MEMUNIT_USETYPE_C)
    {
        u4MemSize = pFbg->u4CSize;
        
        if(u4AssignCnt > pFbg->u4FbCnt || (ucBaseIdx + u4AssignCnt) > FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc C Count (%d,%d,%d) Error\n",pFbg->ucFbgId,ucBaseIdx,u4AssignCnt,pFbg->u4FbCnt);
            return FALSE;
        }
        else if(u4AssignCnt != 0)
        {
            u4MemCnt = u4AssignCnt;
        }
        else  // u4AssignCnt is 0
        {
            u4MemCnt = pFbg->u4FbCnt;
        }

        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList,u4MemSize,pFbg->au4AddrC + ucBaseIdx,
            u4MemCnt,FBM_FMG_Y_ALIGMENT,FBM_MEMUNIT_USETYPE_C);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc Cbcr error (%d/%d)\n",pFbg->ucFbgId,u4AllocCnt,u4MemCnt);
            return FALSE;
        }

        for(u4FbIdx = ucBaseIdx; u4FbIdx < ucBaseIdx + u4MemCnt; u4FbIdx++)
        {
            pFbg->au4SizeC[u4FbIdx] = u4MemSize;
        }
        
    }
    
    // Assign Ext memory;
    if(u4AssignFlag & FBM_MEMUNIT_USETYPE_EXT)
    {
        u4MemSize = pFbg->u4YSize>>8;
        
        if(u4AssignCnt > pFbg->u4FbCnt || (ucBaseIdx + u4AssignCnt) > FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc Y Count (%d,%d,%d) Error\n",pFbg->ucFbgId,ucBaseIdx,u4AssignCnt,pFbg->u4FbCnt);
            return FALSE;
        }
        else if(u4AssignCnt != 0)
        {
            u4MemCnt = u4AssignCnt;
        }
        else  // u4AssignCnt is 0
        {
            u4MemCnt = pFbg->u4FbCnt;
        }

        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList, u4MemSize, pFbg->au4AddrY_Ext + ucBaseIdx,
            u4MemCnt,FBM_FMG_Y_ALIGMENT,FBM_MEMUNIT_USETYPE_EXT);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc ExtY error (%d/%d)\n",pFbg->ucFbgId,u4AllocCnt,u4MemCnt);
            return FALSE;
        }
        
        u4MemSize = pFbg->u4CSize>>8;
        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList, u4MemSize, pFbg->au4AddrC_Ext + ucBaseIdx,
            u4MemCnt,FBM_FMG_Y_ALIGMENT,FBM_MEMUNIT_USETYPE_EXT);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc ExtC error (%d/%d)\n",pFbg->ucFbgId,u4AllocCnt,u4MemCnt);
            return FALSE;
        }
    }

    // Assign MV memory;
    if(u4AssignFlag & FBM_MEMUNIT_USETYPE_MV)
    {

        _FbmMemUnitMemParam(pFbg,&u4MemSize,&u4MemCnt,FBM_MEMUNIT_USETYPE_MV,FALSE);
        if(u4AssignCnt != 0 && u4AssignCnt<u4MemCnt)
        {
            u4MemCnt = u4AssignCnt;
        }
        
        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList, u4MemSize, pFbg->au4AddrMv, u4MemCnt,FBM_B2R_H_PITCH, FBM_MEMUNIT_USETYPE_MV);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc MV error (%d/%d)\n",pFbg->ucFbgId,u4AllocCnt,u4MemCnt);
            return FALSE;
        }
    }

    // Assign MV memory;
    if(u4AssignFlag & FBM_MEMUNIT_USETYPE_CABAC)
    {        
        _FbmMemUnitMemParam(pFbg,&u4MemSize,&u4MemCnt,FBM_MEMUNIT_USETYPE_CABAC,FALSE);
        if(u4AssignCnt != 0 && u4AssignCnt<u4MemCnt)
        {
            u4MemCnt = u4AssignCnt;
        }
        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList, u4MemSize, pFbg->au4AddrCabac,u4MemCnt, FBM_B2R_H_PITCH, FBM_MEMUNIT_USETYPE_CABAC);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc cabac error (%d/%d)\n",pFbg->ucFbgId,u4AllocCnt,u4MemCnt);
            return FALSE;
        }
    }

    if(u4AssignFlag & FBM_MEMUNIT_USETYPE_RPR)
    {
        u4MemCnt = 1;
        
        u4MemSize = FBM_FBG_TYPE_PAL_Y_SIZE;
        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList, u4MemSize, &pFbg->u4ExtraYBuffer, u4MemCnt, FBM_B2R_H_PITCH, FBM_MEMUNIT_USETYPE_RPR);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc RPR_Y error \n",pFbg->ucFbgId);
            return FALSE;
        }

        u4MemSize = FBM_FBG_TYPE_PAL_C_SIZE;
        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList, u4MemSize, &pFbg->u4ExtraCBuffer, u4MemCnt, FBM_B2R_H_PITCH, FBM_MEMUNIT_USETYPE_RPR);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc RPR_C error\n",pFbg->ucFbgId);
            return FALSE;
        }

        u4MemSize = ((FBM_FBG_TYPE_PAL_Y_SIZE+FBM_FBG_TYPE_PAL_C_SIZE)<<1);
        u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList, u4MemSize, &pFbg->u4RPRSeamlessBuffer, u4MemCnt, FBM_B2R_H_PITCH, FBM_MEMUNIT_USETYPE_RPR);
        if(u4AllocCnt != u4MemCnt)
        {
            LOG(0,"_FbmAssignFbMemory Fbg%d Alloc RPR_Seamless error\n",pFbg->ucFbgId);
            return FALSE;
        }
    }

    if(u4AssignFlag & FBM_MEMUNIT_USETYPE_WORK)
    {
         UINT32 u4RemainSize;
         //u4MemSize = _FbmMemUnitMemParam(pFbg, NULL, NULL, FBM_MEMUNIT_USETYPE_WORK, TRUE);
         u4RemainSize = _FBMMemUnitFreeSize(prMemUnitList,FBM_MEMUNIT_USETYPE_WORK,MEMUNIT_SEARCH_TYPE_OR);
         u4MemSize =   u4RemainSize - (FBM_FMG_Y_ALIGMENT+1);
         u4MemCnt = 1;
         u4AllocCnt = _FBMMemUnitAllocMemory(prMemUnitList, u4MemSize, &pFbg->u4Workbuffer, u4MemCnt, FBM_FMG_Y_ALIGMENT, FBM_MEMUNIT_USETYPE_WORK);
         if(u4AllocCnt != u4MemCnt)
         {
             LOG(0,"_FbmAssignFbMemory Fbg%d Alloc Workbuffer error\n",pFbg->ucFbgId);
             return FALSE;
         }
         pFbg->u4WorkBufSize = u4MemSize;
    }
    
    return TRUE;
}

static void _FbmMemUnitSpecialSetting(FBM_FBG_T *pFbg)
{
    pFbg->au4MiscTblStartAddr =  pFbg->u4Workbuffer;
    if(pFbg->au4AddrMv[0] == 0)
    {
        pFbg->au4AddrMv[0] = pFbg->u4Workbuffer;
    }
    
    return;
}

static BOOL _FbmFbMemOverlap(UCHAR ucFbgId,UCHAR ucFbIdA, UCHAR ucFbIdB)
{
    FBM_FBG_T *pFbg = &_prFbg[ucFbgId];
    UINT32 u4AddrA,u4AddrB,u4SizeA,u4SizeB;
    BOOL fgOverlap = FALSE;

    // Y-Y overlap check
    u4AddrA = pFbg->au4AddrY[ucFbIdA];
    u4SizeA = pFbg->au4SizeY[ucFbIdA];
    u4AddrB = pFbg->au4AddrY[ucFbIdB];
    u4SizeB = pFbg->au4SizeY[ucFbIdB];
    
    if(((u4AddrA >= u4AddrB) && (u4AddrA < u4AddrB + u4SizeB))
        || ((u4AddrB >= u4AddrA) && (u4AddrB < u4AddrA + u4SizeA)))
    {
        fgOverlap = TRUE;
    }

    // C-C overlap check
    u4AddrA = pFbg->au4AddrC[ucFbIdA];
    u4SizeA = pFbg->au4SizeC[ucFbIdA];
    u4AddrB = pFbg->au4AddrC[ucFbIdB];
    u4SizeB = pFbg->au4SizeC[ucFbIdB];
    
    if(((u4AddrA >= u4AddrB) && (u4AddrA < u4AddrB + u4SizeB))
        || ((u4AddrB >= u4AddrA) && (u4AddrB < u4AddrA + u4SizeA)))
    {
        fgOverlap = TRUE;
    }

    // Y-C overlap check
    u4AddrA = pFbg->au4AddrY[ucFbIdA];
    u4SizeA = pFbg->au4SizeY[ucFbIdA];
    u4AddrB = pFbg->au4AddrC[ucFbIdB];
    u4SizeB = pFbg->au4SizeC[ucFbIdB];

    if(((u4AddrA >= u4AddrB) && (u4AddrA < u4AddrB + u4SizeB))
        || ((u4AddrB >= u4AddrA) && (u4AddrB < u4AddrA + u4SizeA)))
    {
        fgOverlap = TRUE;
    }

    // C-Y overlap check
    u4AddrA = pFbg->au4AddrC[ucFbIdA];
    u4SizeA = pFbg->au4SizeC[ucFbIdA];
    u4AddrB = pFbg->au4AddrY[ucFbIdB];
    u4SizeB = pFbg->au4SizeY[ucFbIdB];
    
    if(((u4AddrA >= u4AddrB) && (u4AddrA < u4AddrB + u4SizeB))
        || ((u4AddrB >= u4AddrA) && (u4AddrB < u4AddrA + u4SizeA)))
    {
        fgOverlap = TRUE;
    }

    return fgOverlap;
}

static UCHAR _FbmSyncWaitUseFb(UCHAR ucFbgId)
{
   FBM_FBG_T *pFbg = &_prFbg[ucFbgId];
   UCHAR ucSyncCnt = 0, ucWaitSyncCnt=0, ucFbIdx=0,ucWaitFbId=0;
   BOOL fgOverLap= FALSE;
   while(ucWaitFbId < FBM_MAX_FB_NS_PER_GROUP)
   {
       if(pFbg->aucFbRotationStatus[ucWaitFbId] == FB_ROTATION_WAIT_USE)
       {
          fgOverLap = FALSE;
          ucWaitSyncCnt++;

          for(ucFbIdx = 0; ucFbIdx < FBM_MAX_FB_NS_PER_GROUP; ucFbIdx++)
          {
              if(pFbg->aucFbRotationStatus[ucFbIdx] == FB_ROTATION_WAIT_UNUSE)
              {
                   if(_FbmFbMemOverlap(ucFbgId,ucWaitFbId,ucFbIdx))
                   {
                      fgOverLap = TRUE;
                      break;
                   }
              }
          }

          if(fgOverLap == FALSE)
          {
              if(pFbg->u4VDecFmt == FBM_VDEC_MPEG2 || pFbg->u4VDecFmt == FBM_VDEC_MPEG4
                   || pFbg->u4VDecFmt == FBM_VDEC_VC1 || pFbg->u4VDecFmt == FBM_VDEC_RV);
              {
                   VERIFY(_FBM_csema_unlock(&pFbg->hEmptyBQSemaphore) == OSR_OK);
              }
              
              pFbg->afgRefList[ucWaitFbId] = FALSE;
              pFbg->afgWaitDisplay[ucWaitFbId] =FALSE;
              pFbg->aucFbStatus[ucWaitFbId] = FBM_FB_STATUS_EMPTY;
              pFbg->aucFbRotationStatus[ucWaitFbId] = FB_ROTATION_USE;
              //FBM_FbMemReset(pFbg->ucFbgId,ucWaitFbId);
              _FBM_PutFrameBufferToEmptyQ(pFbg->ucFbgId,ucWaitFbId);
              ucSyncCnt++;
          }
          
       }
       ucWaitFbId ++;
   }

   if(ucSyncCnt > 0)
   {
       LOG(2,"_FbmSyncWaitUseFb(Fb=%d,Synced=%d,UnSynced=%d, Total=%d)\n",
        pFbg->ucFbgId,ucSyncCnt,ucWaitSyncCnt - ucSyncCnt, pFbg->u4FbCnt);
   }
   
   return ucSyncCnt;
}

static UINT32 _FBM_GetDefaultExternDataSize(UCHAR ucFbgId)
{
    FBM_FBG_T *pFbg;
    UINT32 u4YSize, u4CSize, u4FbWidth,u4FbHeight, u4FbCnt, u4MvCnt,ExternSize;
    pFbg = &_prFbg[ucFbgId];

    u4YSize = pFbg->u4YSize;
    u4CSize = pFbg->u4CSize;
    u4FbWidth = pFbg->u4FbWidth;
    u4FbHeight = pFbg->u4FbHeight;
    u4FbCnt = pFbg->u4FbCnt;

    u4MvCnt = pFbg->ucMvBufNs;

    pFbg->u4FbCnt = FBM_MEMUNIT_DEFAULT_EXTERNDATA_FBCNT;
    _FbmCalculateYCSize(pFbg,FBM_MEMUNIT_FRAME_MAX_WIDTH,FBM_MEMUNIT_FRAME_MAX_HEIGHT);
    ExternSize = _FbmMemUnitMemParam(pFbg, NULL, NULL, _FbmMemUnitExtenBufFlag(pFbg), TRUE);
    
    pFbg->u4YSize = u4YSize;
    pFbg->u4CSize = u4CSize;
    pFbg->u4FbWidth = u4FbWidth;
    pFbg->u4FbHeight = u4FbHeight;
    pFbg->u4FbCnt = u4FbCnt;
    pFbg->ucMvBufNs= (UCHAR)u4MvCnt;
    
    return ExternSize;
}

VOID _FbmMemUnitAllocMemPool(UCHAR ucFbgId ,FBM_MEMUNIT *prMemPoolList)
{   
    FBM_FBG_T *pFbg;
    pFbg = &_prFbg[ucFbgId];
    
#ifdef FBM_MEMUNIT_MEM_SEPARATE
    prMemPoolList[0].u4StartAddr = pFbg->u4FbYPool;
    prMemPoolList[0].u4UsedAddr  = pFbg->u4FbYPool;
    prMemPoolList[0].u4EndAddr   = pFbg->u4FbYPool + pFbg->u4FbYPoolSize;
    prMemPoolList[0].u4Flag      = FBM_MEMUNIT_USETYPE_Y;

    prMemPoolList[1].u4StartAddr = pFbg->u4FbCPool;
    prMemPoolList[1].u4UsedAddr  = pFbg->u4FbCPool;
    prMemPoolList[1].u4EndAddr   = pFbg->u4FbCPool + pFbg->u4FbCPoolSize;
    prMemPoolList[1].u4Flag      = FBM_MEMUNIT_USETYPE_C;
    
    prMemPoolList[2].u4StartAddr = pFbg->u4FbExternDataPool;
    prMemPoolList[2].u4UsedAddr  = pFbg->u4FbExternDataPool;
    prMemPoolList[2].u4EndAddr   = pFbg->u4FbExternDataPool + pFbg->u4FbExternDataPoolSize;
    prMemPoolList[2].u4Flag      = FBM_MEMUNIT_USETYPE_ALLEXTEN;
#else
    prMemPoolList[0].u4StartAddr = pFbg->u4FbMemoryPool;
    prMemPoolList[0].u4UsedAddr  = pFbg->u4FbMemoryPool;
    prMemPoolList[0].u4EndAddr   = pFbg->u4FbMemoryPool + pFbg->u4FbMemoryPoolSize;
    prMemPoolList[0].u4Flag      = FBM_MEMUNIT_USETYPE_ALL;

    prMemPoolList[1].u4Flag = 0;
    prMemPoolList[1].u4StartAddr = 0;
#endif

    return;
}

VOID  _FBM_FbgSetDefaultDataPartion(UCHAR ucFbgId, UINT32 u4Width, UINT32 u4Height)
{
    FBM_FBG_T *pFbg;
    UINT32 u4ExternDataSize = 0,u4PoolSize,u4PoolEnd,u4MvCnt;
    pFbg = &_prFbg[ucFbgId];
    u4ExternDataSize = _FBM_GetDefaultExternDataSize(ucFbgId);
    _FbmCalculateYCSize(pFbg, u4Width, u4Height);

    u4PoolSize = pFbg->u4FbMemoryPoolSize;
    u4PoolEnd = pFbg->u4FbMemoryPool + u4PoolSize;
    
    LOG(1,"Fbg:%d _FBM_FbgSetDefaultDataPartion size 0x%x, poolsize =0x%x\n",ucFbgId,u4ExternDataSize,pFbg->u4FbMemoryPoolSize);
    
    if(u4PoolSize < u4ExternDataSize)
    {
        u4ExternDataSize = 0;
        LOG(0,"_FBM_FbgSetDefaultDataPartion no enough data !!!!");
    }
    else
    {
        u4PoolSize -= u4ExternDataSize;
    }

    pFbg->u4FbCnt = FBM_MAX_FB_NS_PER_GROUP;//u4PoolSize/(pFbg->u4YSize + pFbg->u4CSize);
    
    do
    {
       if(_FbmMemUnitMemParam(pFbg,NULL, NULL, _FbmMemUnitExtenBufFlag(pFbg),TRUE) > u4ExternDataSize)
       {
           pFbg->u4FbCnt --;
       }
       else
       {
           break;
       }
       
    } while(pFbg->u4FbCnt > 0);

    
    _FbmMemUnitMemParam(pFbg, NULL, &u4MvCnt, FBM_MEMUNIT_USETYPE_MV, FALSE);
    pFbg->ucMvBufNs  = u4MvCnt;
    pFbg->u4FbCnt = u4PoolSize/(pFbg->u4YSize + pFbg->u4CSize);
    
    pFbg->u4FbYPool = pFbg->u4FbMemoryPool;
    pFbg->u4FbYPoolSize = pFbg->u4FbCnt * pFbg->u4YSize;
    pFbg->u4FbCPool = pFbg->u4FbYPool + pFbg->u4FbYPoolSize;
    pFbg->u4FbCPoolSize = pFbg->u4FbCnt * pFbg->u4CSize;
    pFbg->u4FbExternDataPool = pFbg->u4FbCPool + pFbg->u4FbCPoolSize;

    if(pFbg->u4FbExternDataPool + u4ExternDataSize > u4PoolEnd)
    {
        LOG(0,"_FBM_FbgSetDefaultDataPartion(fbg:%d,FbCnt:%d,YSize:0x%x,YSize:0x%x) (0x%x+0x%x) larger thern (0x%x+0x%x)!!!\n",
            pFbg->ucFbgId,pFbg->u4FbCnt, pFbg->u4YSize, pFbg->u4CSize, pFbg->u4FbExternDataPool,u4ExternDataSize,pFbg->u4FbMemoryPool,pFbg->u4FbMemoryPoolSize);
    }

    pFbg->u4FbExternDataPoolSize = u4PoolEnd - pFbg->u4FbExternDataPool;

    LOG(1,"_FBM_FbgSetDefaultDataPartion(Fbg:%d, FbCnt:%d, MvCnt:%d,YSize:0x%x,CSize:0x%x)\n",
        pFbg->ucFbgId, pFbg->u4FbCnt,pFbg->ucMvBufNs,pFbg->u4YSize, pFbg->u4CSize);

    LOG(1,"_FBM_FbgSetDefaultDataPartion(Fbg:%d,TotalPool(0x%x-0x%x),YPool(0x%x-0x%x),CPool(0x%x-0x%x))\n",
        pFbg->ucFbgId, pFbg->u4FbMemoryPool,u4PoolEnd,pFbg->u4FbYPool,pFbg->u4FbYPool + pFbg->u4FbYPoolSize,pFbg->u4FbCPool,pFbg->u4FbCPool+pFbg->u4FbCPoolSize);  
    LOG(1,"_FBM_FbgSetDefaultDataPartion((Fbg:%d,ExternPool(0x%x-0x%x,Need:0x%x))\n",
        pFbg->ucFbgId,pFbg->u4FbExternDataPool,pFbg->u4FbExternDataPool + pFbg->u4FbExternDataPoolSize,u4ExternDataSize);
    return ;
}

BOOL _FBM_FbgRemap(UCHAR ucFbgId, UINT32 u4Width, UINT32 u4Height)
{
    FBM_FBG_T *pFbg;
    UINT8  ucIdx = 0,ucFbCnt = 0,ucFbId=0,ucLoopCnt=0;
    FBM_MEMUNIT arMemPoolList[FBM_MEMUNIT_LIST_MAX];
    FBM_MEMUNIT arMemUnitList[FBM_MEMUNIT_LIST_MAX];
    if(VERIFY_FBG(ucFbgId))
    {
        return FALSE;
    }
    
    pFbg = &_prFbg[ucFbgId];
    x_memset(arMemPoolList,0,sizeof(arMemPoolList));
    x_memset(arMemUnitList,0,sizeof(arMemUnitList));
    FBM_MUTEX_LOCK(ucFbgId);

    LOG(0,"FBM_FbgRemap(Fbg:%d,Pool(0x%x+0x%x),Fmt:%d,10Bit:%d,Ufo:%d,App:%d) ->(%d x %d) Start\n",
    pFbg->ucFbgId,pFbg->u4FbMemoryPool,pFbg->u4FbMemoryPoolSize,pFbg->u4VDecFmt,
    pFbg->fg10Bit,pFbg->fgUFO,pFbg->u1FbgAppMode, u4Width,u4Height);
    
    _FbmMemUnitAllocMemPool(pFbg->ucFbgId,arMemPoolList);
    _FbmMemUnitPrint(arMemPoolList,"MemPool Infor");
    _FbmCalculateYCSize(pFbg,u4Width,u4Height);
    _FBMMemUnitCalculateFbCount(pFbg,arMemPoolList);
    
    LOG(0,"FBM_FbgRemap(Fbg:%d,Width:%d,Height:%d,FbWidht:%d,FbHeight:%d,YSize:0x%x,CSize:0x%x,FbCnt:%d MvCnt:%d\n",
        pFbg->ucFbgId, u4Width,u4Height,pFbg->u4FbWidth,pFbg->u4FbHeight,pFbg->u4YSize,
        pFbg->u4CSize,pFbg->u4FbCnt,pFbg->ucMvBufNs);
    
    if(pFbg->u4FbCnt == 0)
    {
         LOG(0,"_FBM_FbgRemap(Fbg:%d) Frame buffer calculate fbg cnt error fbg adjust fail\n",pFbg->ucFbgId);
         FBM_MUTEX_UNLOCK(ucFbgId);
         return FALSE;
    }
    
    pFbg->fgAdjustWorkBuf = TRUE;
    _FbmMemUnitCreateUnitList(pFbg,arMemPoolList,arMemUnitList,FALSE);
    
    _FbmMemUnitPrint(arMemPoolList,"MemPool Infor After Occupy");
    _FbmMemUnitPrint(arMemUnitList,"MemList Infor");
    
    for(ucIdx=0; ucIdx<FBM_MAX_FB_NS_PER_GROUP; ucIdx++)
    {
        if(pFbg->aucFbRotationStatus[ucIdx] == FB_ROTATION_USE)
        {
            pFbg->aucFbRotationStatus[ucIdx] = FB_ROTATION_WAIT_UNUSE;
        }

        if(pFbg->aucFbRotationStatus[ucIdx] == FB_ROTATION_WAIT_USE)
        {
            pFbg->aucFbRotationStatus[ucIdx] = FB_ROTATION_UNUSE;
        }
    }

    pFbg->ucFbNs = FBM_MAX_FB_NS_PER_GROUP;
    pFbg->ucFbNsBase = 0;

    ucFbCnt = 0;
    ucLoopCnt =0;
    FBM_SetFrameBufferFlag(ucFbgId,FBM_FLAG_GETEMPTY_NOWAIT);
    
    do
    {
        ucFbId = FBM_GetEmptyFrameBuffer(ucFbgId,0);
        if(ucFbId != FBM_FB_ID_UNKNOWN)
        {
            pFbg->aucFbRotationStatus[ucFbId] = FB_ROTATION_UNUSE;
            pFbg->aucFbStatus[ucFbId] = FBM_FB_STATUS_EMPTY;
            LOG(2,"_FBM_FbgRemap(fbg:%d) flush empyt fb:%d to UNUSE\n",ucFbgId,ucFbId);
            ucFbCnt++;
        }
        else
        { 
            LOG(1,"_FBM_FbgRemap(fbg:%d) flush %d empyt Fbs to UNUSE\n",ucFbgId,ucFbCnt);
            break;
        }
        
        if(ucLoopCnt >=FBM_MAX_FB_NS_PER_GROUP)
        {
            LOG(0,"_FBM_FbgRemap(fbg:%d) flush emptyQ abnormal\n",ucFbgId);
            break;
        }
        ucLoopCnt ++;
    }while(1);
    
    FBM_ClrFrameBufferFlag(ucFbgId,FBM_FLAG_GETEMPTY_NOWAIT);

    ucFbCnt = 0;
    for(ucIdx=0; ucIdx < FBM_MAX_FB_NS_PER_GROUP ; ucIdx++)
    {
        if(pFbg->aucFbRotationStatus[ucIdx] == FB_ROTATION_UNUSE)
        {
            if(ucFbCnt < pFbg->u4FbCnt)
            {
                if(_FbmMemUnitAssignFbgMemory(pFbg,arMemUnitList,ucIdx,1,FBM_MEMUNIT_USETYPE_YC))
                {
                    pFbg->aucFbRotationStatus[ucIdx] = FB_ROTATION_WAIT_USE;
                }
                else
                {
                    LOG(0,"_FbmMemUnitAssignFbgMemory(fbg:%d,fbCnt:(%d/%d) )YC FAIL \n",pFbg->ucFbgId,ucFbCnt,pFbg->u4FbCnt);
                    FBM_MUTEX_UNLOCK(ucFbgId);
                    return FALSE;
                }
                ucFbCnt++;
            }
            else
            {
                 break;
            }
        }
    }

    if(ucFbCnt < pFbg->u4FbCnt && ucIdx == FBM_MAX_FB_NS_PER_GROUP)
    {
        LOG(0,"_FBM_FbgRemap(fbg:%d) abnormal,remap fail\n",ucFbgId);
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FALSE;
    }

    if(_FbmMemUnitAssignFbgMemory(pFbg,arMemUnitList,0,0,_FbmMemUnitExtenBufFlag(pFbg)) == FALSE)
    {
        LOG(0,"_FbmMemUnitAssignFbgMemory(fbg:%d) Exten Data FAIL \n",pFbg->ucFbgId);
        FBM_MUTEX_UNLOCK(ucFbgId);
        return FALSE;
    }
    
    _FbmMemUnitPrint(arMemUnitList,"MemList Infor After Occupy");
    _FbmMemUnitSpecialSetting(pFbg);
    ucFbCnt = _FbmSyncWaitUseFb(ucFbgId);
    FBM_MUTEX_UNLOCK(ucFbgId);

    LOG(0,"_FBM_FbgRemap(Fbg:%d,Use %d Fbs, Total Fb %d) Remap OK!!\n",ucFbgId, ucFbCnt, pFbg->u4FbCnt);
    return TRUE;
}

static VOID _FbmMemUnitVerifyStart(UCHAR u4FbgId,UINT32 u4Widht,UINT32 u4Height,UINT32 u4FbCnt)
{
    static FBM_FBG_T *pFbg = NULL;
    FBM_MEMUNIT arMemPoolList[FBM_MEMUNIT_LIST_MAX];
    FBM_MEMUNIT arMemUnitList[FBM_MEMUNIT_LIST_MAX];
    UINT8 ucIdx=0;
    BOOL fgUseDefaultPool = FALSE;
    if(pDefaultFbg == NULL)
    {
        pDefaultFbg = (FBM_FBG_T *)x_mem_alloc_virtual(sizeof(FBM_FBG_T));
        if(pDefaultFbg == NULL)
        {
            Printf("FBG memalloc fail \n");
            return;
        }
    }

    pFbg = pDefaultFbg;
    x_memcpy(pFbg,&_prFbg[u4FbgId],sizeof(FBM_FBG_T));
    x_memset(arMemPoolList,0,sizeof(arMemPoolList));
    x_memset(arMemUnitList,0,sizeof(arMemUnitList));

    Printf("Fbg:%d,Pool(0x%x+0x%x),Fmt:%d,10Bit:%d,Ufo:%d,App:%d\n",
        pFbg->ucFbgId,pFbg->u4FbMemoryPool,pFbg->u4FbMemoryPoolSize,pFbg->u4VDecFmt,
        pFbg->fg10Bit,pFbg->fgUFO,pFbg->u1FbgAppMode);

    for(ucIdx=0; ucIdx<FBM_MEMUNIT_LIST_MAX;ucIdx++)
    {
        if(arDefaultMemPoolList[ucIdx].u4StartAddr != 0)
        {
            _FBMMemUnitAppend(arMemPoolList,arDefaultMemPoolList+ucIdx, FALSE);
            fgUseDefaultPool = TRUE;
        }
    }
    
    if(fgUseDefaultPool == FALSE)
    {
        _FbmMemUnitAllocMemPool(u4FbgId,arMemPoolList);
    }
    _FbmMemUnitPrint(arMemPoolList,"MemPoolInfo Before Occupy");

    _FbmCalculateYCSize(pFbg,u4Widht,u4Height);

    if(u4FbCnt != 0)
    {
        pFbg->u4FbCnt = u4FbCnt;
    }
    else
    {
        _FBMMemUnitCalculateFbCount(pFbg,arMemPoolList);
        /*
               pFbg->u4FbCnt = FBM_CalcBufNum(pFbg->ucFbgId,pFbg->ucFbgType, pFbg->u4VDecFmt,
               u4Widht,u4Height,pFbg->u4FbMemoryPoolSize,pFbg->u4YSize,pFbg->u4CSize,pFbg->u1FbgAppMode,NULL);
             */
    }
    
    Printf("Width=0x%x,Height=0x%x,YSize=0x%x,CSize=0x%x,FbCnt=%d MvCnt=%d\n",
        u4Widht,u4Height,pFbg->u4YSize,pFbg->u4CSize,pFbg->u4FbCnt,pFbg->ucMvBufNs);

     if(pFbg->u4FbCnt == 0)
     {
         Printf("Frame buffer calculate fbg cnt error fbg adjust fail\n");
         return ;
     }
     
    _FbmMemUnitCreateUnitList(pFbg,arMemPoolList,arMemUnitList,FALSE);
    _FbmMemUnitPrint(arMemPoolList,"MemPoolInfo After Occupy");
    _FbmMemUnitPrint(arMemUnitList,"MemunitInfo Before Occupy");
    _FbmMemUnitAssignFbgMemory(pFbg,arMemUnitList,0,0,_FbmMemUnitExtenBufFlag(pFbg)|(FBM_MEMUNIT_USETYPE_YC));
    _FbmMemUnitPrint(arMemUnitList,"MemunitInfo After Occupy");
    Printf("_FbmMemUnitTest done \n");
}

VOID _FbmMemUnitVerify(UCHAR ucType,UINT32 u4Param1, 
    UINT32 u4Param2, UINT32 u4Param3, UINT32 u4Param4,UINT32 u4Param5,UINT32 u4Param6)
{
    UINT32 u4Flag,u4Idx;

    Printf("_FbmMemUnitVerify(%d,0x%x,%0x%x,0x%x,%0x%x,0x%x,0x%x)\n",ucType,u4Param1,u4Param2,u4Param3,
        u4Param4, u4Param5, u4Param6);
    
    if(ucType == 0)
    {
        _FbmMemUnitVerifyStart((UCHAR)u4Param1,u4Param2,u4Param3,u4Param4);
    }
    else if(ucType == 1)
    {
        u4DeFaultWorkFlag = u4Param1;
        if(pDefaultFbg)
        {
            Printf("Currnet Flag=0x%x\n",_FbmMemUnitExtenBufFlag(pDefaultFbg));
        }
    }
    else if(ucType == 2)
    {
        UCHAR ucSetType =(UCHAR )u4Param1;
        UCHAR uPoolIdx = (UCHAR )u4Param2;
        if(ucSetType == 0)
        {
            x_memset(arDefaultMemPoolList,0,sizeof(arDefaultMemPoolList));
        }
        else if(ucSetType == 1)
        {
            arDefaultMemPoolList[uPoolIdx].u4StartAddr = u4Param3;
            arDefaultMemPoolList[uPoolIdx].u4EndAddr = u4Param4;
            arDefaultMemPoolList[uPoolIdx].u4UsedAddr = u4Param5;
            arDefaultMemPoolList[uPoolIdx].u4Flag = u4Param6;
        }
        else if(ucSetType == 100)
        {
            for(u4Idx=0; u4Idx<FBM_MEMUNIT_LIST_MAX;u4Idx++)
            {
                Printf("Defaul[%d] Start:0x%x, End:0x%x, Used:0x%x, u4Flag:0x%x\n",u4Idx,
                 arDefaultMemPoolList[uPoolIdx].u4StartAddr,
                 arDefaultMemPoolList[uPoolIdx].u4EndAddr,
                 arDefaultMemPoolList[uPoolIdx].u4UsedAddr,
                 arDefaultMemPoolList[uPoolIdx].u4Flag);
            }
        }
    }
    else if(ucType == 100)
    {
        if(pDefaultFbg == NULL)
        {
            return ;
        }
        
        //_FbgStatus(pDefaultFbg);
        u4Flag = _FbmMemUnitExtenBufFlag(pDefaultFbg);
        if(u4Flag & FBM_MEMUNIT_USETYPE_EXT)
        {
            for(u4Idx=0;u4Idx < pDefaultFbg->u4FbCnt; u4Idx++)
            {
                Printf("Exter[%d]:Y=0x%x, C=0x%x\n",u4Idx, pDefaultFbg->u4ExtraYBuffer,pDefaultFbg->u4ExtraCBuffer);
            }
        }

        if(u4Flag & FBM_MEMUNIT_USETYPE_MV)
        {
            for(u4Idx=0;u4Idx<pDefaultFbg->ucMvBufNs;u4Idx++)
            {
                Printf("MV[%d]=0x%x\n",u4Idx,pDefaultFbg->au4AddrMv[u4Idx]);
            }
        }

        if(u4Flag & FBM_MEMUNIT_USETYPE_CABAC)
        {
            for(u4Idx=0;u4Idx<FBM_MAX_CABAC_BUF_NS_PER_GROUP;u4Idx++)
            {
                Printf("Cabac[%d]=0x%x\n",u4Idx,pDefaultFbg->au4AddrCabac[u4Idx]);
            }
        }
        
        if(u4Flag & FBM_MEMUNIT_USETYPE_RPR)
        {
            Printf("RPR: ExtY=0x%x, ExtC=0x%x,Seamless:0x%x\n",\
                pDefaultFbg->u4ExtraYBuffer,pDefaultFbg->u4ExtraCBuffer,pDefaultFbg->u4RPRSeamlessBuffer);
        }

        if(u4Flag & FBM_MEMUNIT_USETYPE_WORK)
        {
            Printf("WorkBuffer: 0x%x, Size=0x%x\n",pDefaultFbg->u4Workbuffer,pDefaultFbg->u4WorkBufSize);
        }

    }
}

