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
 * $Date: 2015/02/28 $
 * $RCSfile: vdp_prescale.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/*****************************************************************************
*  Notes: 
*****************************************************************************/


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fbm_drvif.h"
#include "imgrz_if.h"
//#include "imgrz_hw.h"
#include "imgrz_drvif.h"
#include "vdp_prescale_drvif.h"
#include "gdma_if.h"
#include "b2r_drvif.h"
#include "x_timer.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#define DEFINE_IS_LOG    VDP_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CC_INTERLACE_RESIZING

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Brief of VDP_PRESCALE_FRM_T
 */
typedef struct _VDP_PRESCALE_INFO_T
{
    UCHAR ucRzHwId;
    HANDLE_T hRzMsgQ;
    HANDLE_T hRzCmdQ;
    HANDLE_T hFlushSema;
    HANDLE_T hFlushMutex;
    HANDLE_T hThdSyncCloseSema;
    BOOL fgRetry;
    BOOL fgIsWaitFlush;
    UCHAR ucCmdId;
    UINT32 u4UncompressYaddr;
    UINT32 u4UncompressCaddr;
    PFN_PRESCALE_GETTGBUF_CB pfnGetExTgBuf;
    PFN_PRESCALE_RZFIN_CB pfnRzFinCb;
    VOID *pvRzFinCbTag;
    VDP_PRESCALE_TG_INFO_T rTgBufInfo;
    VOID *pvTgBufInfoTag;
    PFN_GET_GETRZBUF pfnGetRzBuf;
    PFN_GET_GETRZBUFADDR pfnGetRzBufAddr;
    PFN_SET_SETRZBUFSTATUS pfnSetRzBufStatus;
} VDP_PRESCALE_INFO_T;


/** Brief of VDP_PRESCALE_FRM_T
 */
typedef struct _VDP_PRESCALE_FRM_T
{
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UCHAR ucRzFbId;
    UCHAR ucCmdId;
    UCHAR ucTaskId;
    HAL_TIME_T rTimePutDispQ;
    HAL_TIME_T rTimeRzWaitFB;
    HAL_TIME_T rTimeRzStart;
    HAL_TIME_T rTimeRzEnd;
} VDP_PRESCALE_FRM_T;


/** Brief of VDP_PRESCALE_CMD_T
 */
typedef struct _VDP_PRESCALE_CMD_T
{
    UCHAR ucCmd;
    UCHAR ucID;
    BOOL fgIsWait;
} VDP_PRESCALE_CMD_T;


typedef enum
{
    PRESCALE_CMD_UNKNOWN,
    PRESCALE_CMD_FLUSH,
    PRESCALE_CMD_MAX,
}   VPUSH_CMD_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define PRESCALE_CMD_Q_SIZE 16
#define PRESCALE_MSG_Q_SIZE 64
#define VDP_PRESCALE_STACK_SIZE 2048

#ifdef CC_DTV_SUPPORT_LG
#if defined(CC_HIGH_B2R_THREAD_PRIORITY) //customer kr 3d case
#define VDP_PRESCALE_THREAD_PRIORITY 4
#else
#define VDP_PRESCALE_THREAD_PRIORITY 60
#endif

#define CC_INTERLACE_RESIZING
#define CC_PRESCALE_PROFILE
#else
#define VDP_PRESCALE_THREAD_PRIORITY 60
//#define CC_INTERLACE_RESIZING
//#define CC_PRESCALE_PROFILE
#endif

#define VDP_PRESCALE_SKIP 1
#define VDP_PRESCALE_RETRY 0

extern UINT32 ImgrzBase[IMGRZ_NUM];
#define IMGRZ_READ32(u4Which,offset)         IO_READ32(ImgrzBase[u4Which], (offset))
#define IMGRZ_WRITE32(u4Which,offset,value)   IO_WRITE32(ImgrzBase[u4Which], (offset), (value))


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
static void _VDP_Prescale_MainLoop(void* pvArg);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgPreScaleInitiated[IMGRZ4VDO_NUM] = {0};
static BOOL _fgPreScaleAlloc[IMGRZ4VDO_NUM] = {0};

VDP_PRESCALE_INFO_T _arPreScaleInfo[IMGRZ4VDO_NUM];
UINT32 u4ProfileEn = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

void _VDPPrescaleReceiveCmd(UCHAR ucRzId)
{
    INT32 i4Ret;
    SIZE_T zMsgSize, zCmdSize;
    UINT16 u2MsgQIdx, u2Count;
    VDP_PRESCALE_INFO_T* prPreScaleInfo;
    VDP_PRESCALE_CMD_T rPreScaleCmd;
    VDP_PRESCALE_FRM_T rPreScaleFrm;

    prPreScaleInfo = &_arPreScaleInfo[ucRzId];
    u2MsgQIdx = 0;
    zCmdSize = sizeof(VDP_PRESCALE_CMD_T);
    zMsgSize = sizeof(VDP_PRESCALE_FRM_T);
        
    if(x_msg_q_receive(&u2MsgQIdx, &rPreScaleCmd, &zCmdSize,
        &(prPreScaleInfo->hRzCmdQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
        VERIFY(x_msg_q_num_msgs(prPreScaleInfo->hRzCmdQ, &u2Count) == OSR_OK);
        LOG(1, "[Prescale]RcvCmd (%d, wait %d, id %d, remain %d)\n", 
            rPreScaleCmd.ucCmd, rPreScaleCmd.fgIsWait, rPreScaleCmd.ucID, u2Count);
        switch(rPreScaleCmd.ucCmd)
        {
            case PRESCALE_CMD_FLUSH:
                //flush Q
                if(prPreScaleInfo->ucCmdId != rPreScaleCmd.ucID) //not reach flush end
                {
                    while (x_msg_q_receive(&u2MsgQIdx, &rPreScaleFrm, &zMsgSize,
                        &(prPreScaleInfo->hRzMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
                    {
                        //prPreScaleInfo->ucCmdId = rPreScaleFrm.ucCmdId;
                        if(rPreScaleFrm.ucFbgId == 0xFF)
                        {
                            if(rPreScaleFrm.ucCmdId != rPreScaleCmd.ucID)
                            {
                                if((i4Ret = x_msg_q_send(prPreScaleInfo->hRzMsgQ, (void *)&rPreScaleFrm,
                                    sizeof(VDP_PRESCALE_FRM_T), 0)) != OSR_OK)
                                {
                                    LOG(1, "VDP Prescale Frm Overflow(%d)\n",i4Ret);
                                }
                                LOG(1, "Prescale Cmd %d/Msg %d(%d) mismatch\n", 
                                    rPreScaleCmd.ucID, rPreScaleFrm.ucCmdId, prPreScaleInfo->ucCmdId);
                            }
                            break;
                        }
                        else
                        {
                            LOG(1, "[Prescale]Flush(Fbg,Fb)=(%d,%d)\n", rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);
                        }
                    }
                }
                // cancel retry mechanism after flush
                prPreScaleInfo->fgRetry = FALSE;
                
                // handle synchronous flush command
                if(rPreScaleCmd.fgIsWait)
                {
                    prPreScaleInfo->fgIsWaitFlush = FALSE;
                    VERIFY(x_sema_unlock(prPreScaleInfo->hFlushSema) == OSR_OK);
                }            
                break;
            default:
                break;
        }
    }
    else
    {
        //no message
    }

}

//-----------------------------------------------------------------------------
/** Brief of _VDP_Prescale_Init.
 */
//-----------------------------------------------------------------------------
UCHAR _VDP_Prescale_Init(UCHAR ucRzIdMask)
{
    INT32 i4Idx = IMGRZ4VDO_NUM;
    UCHAR ucRzId;
    CHAR szBuf[16];
    HANDLE_T hThread;
    VDP_PRESCALE_INFO_T* prPreScaleInfo;

    if (ucRzIdMask)  //Set by Player
    {
        for (i4Idx = 0; i4Idx < IMGRZ4VDO_NUM; i4Idx++)
        {
            {
                if ((ucRzIdMask & (0x01<<i4Idx)) && !_fgPreScaleAlloc[i4Idx])
                {
                    _fgPreScaleAlloc[i4Idx] = TRUE;
                    break;
                }
            }
        }
    }

    if (i4Idx == IMGRZ4VDO_NUM)//not set by Player, use default flow
    {
        for (i4Idx = 0; i4Idx < IMGRZ4VDO_NUM; i4Idx++)
        {
            if(!_fgPreScaleAlloc[i4Idx])
            {
                _fgPreScaleAlloc[i4Idx] = TRUE;
                break;
            }
        }
    }


    if(i4Idx >= IMGRZ4VDO_NUM)
    {
        LOG(1, "IMGRZ ID unavailable\n");
        return VDP_PRESCALE_RZ_ID_UNKNOWN;
    }
    else
    {
        ucRzId = (UCHAR)i4Idx;
    }

    prPreScaleInfo = &_arPreScaleInfo[ucRzId];

    if(!_fgPreScaleInitiated[ucRzId])
    {   
    	LOG(1, "_VDP_Prescale_Init %d\n", VDP_PRESCALE_THREAD_PRIORITY);
        x_memset((void*)prPreScaleInfo, 0, sizeof(VDP_PRESCALE_INFO_T));
        _fgPreScaleInitiated[ucRzId] = TRUE;
        prPreScaleInfo->ucRzHwId = ucRzId;
        prPreScaleInfo->fgRetry = FALSE;
        prPreScaleInfo->fgIsWaitFlush = FALSE;
        FBM_GetFixedImgRzBuffer(&prPreScaleInfo->u4UncompressYaddr,&prPreScaleInfo->u4UncompressCaddr);
        
        x_snprintf(szBuf, sizeof(szBuf),  "PrescaleQ%d\n", ucRzId);
        VERIFY(x_msg_q_create(&prPreScaleInfo->hRzMsgQ, szBuf,
            sizeof(VDP_PRESCALE_FRM_T), PRESCALE_MSG_Q_SIZE) == OSR_OK);
        
        x_snprintf(szBuf, sizeof(szBuf),  "PrescaleCmdQ%d\n", ucRzId);
        VERIFY(x_msg_q_create(&prPreScaleInfo->hRzCmdQ, szBuf,
            sizeof(VDP_PRESCALE_FRM_T), PRESCALE_CMD_Q_SIZE) == OSR_OK);

        VERIFY(x_sema_create(&prPreScaleInfo->hFlushSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&prPreScaleInfo->hThdSyncCloseSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&prPreScaleInfo->hFlushMutex, X_SEMA_TYPE_MUTEX,
            X_SEMA_STATE_UNLOCK) == OSR_OK);

        x_snprintf(szBuf, sizeof(szBuf), "VDP_Prescale%2d", ucRzId);
        VERIFY(x_thread_create(&hThread, szBuf, VDP_PRESCALE_STACK_SIZE,
            VDP_PRESCALE_THREAD_PRIORITY,  _VDP_Prescale_MainLoop, sizeof(ucRzId), (void*)&ucRzId) == OSR_OK);
    }

    LOG(0, "u4ProfileEn 0x%x\n", (UINT32)(&u4ProfileEn));
    return ucRzId;
}


void _VDP_Prescale_Send(UCHAR ucRzId, UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucTaskId)
{
    VDP_PRESCALE_INFO_T* prPreScaleInfo;
    VDP_PRESCALE_FRM_T rPreScaleFrm;
    INT32 i4Ret;

    if(ucRzId >= IMGRZ4VDO_NUM)
    {
        LOG(1, "IMGRZ ID %d unavailable\n", ucRzId);
        return;
    }
    prPreScaleInfo = &_arPreScaleInfo[ucRzId];

    x_memset((void*)&rPreScaleFrm, 0, sizeof(VDP_PRESCALE_FRM_T));
    rPreScaleFrm.ucFbgId = ucFbgId;
    rPreScaleFrm.ucFbId = ucFbId;
    rPreScaleFrm.ucTaskId = ucTaskId;
    LOG(3,"Send fb[%d-%d] taskid %d rzid %d\n",ucFbgId,ucFbId,ucTaskId,ucRzId);
    HAL_GetTime(&rPreScaleFrm.rTimePutDispQ);
    if((i4Ret = x_msg_q_send(prPreScaleInfo->hRzMsgQ, (void *)&rPreScaleFrm,
        sizeof(VDP_PRESCALE_FRM_T), 255)) != OSR_OK)
    {
        LOG(1, "VDP Prescale Overflow(%d)\n",i4Ret);
    }

}


void _VDP_Prescale_Flush(UCHAR ucRzId, BOOL bIsWait)
{
    static UCHAR _ucCmdID = 1;
    INT32 i4Ret;
    UINT16 u2MsgQIdx = 0, u2ReSendMsgCnt = 0;
    SIZE_T zMsgSize = sizeof(VDP_PRESCALE_FRM_T);

    VDP_PRESCALE_INFO_T* prPreScaleInfo;
    VDP_PRESCALE_CMD_T rPreScaleCmd;
    VDP_PRESCALE_FRM_T rPreScaleFrm, rPreScaleFrmRsv;
	if (!_fgPreScaleInitiated[ucRzId] || !_fgPreScaleAlloc[ucRzId])  
    {
    	return;
	}
    if(ucRzId >= IMGRZ4VDO_NUM)
    {
        LOG(1, "IMGRZ ID %d unavailable\n", ucRzId);
        return;
    }
    prPreScaleInfo = &_arPreScaleInfo[ucRzId];

    VERIFY(x_sema_lock(prPreScaleInfo->hFlushMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(prPreScaleInfo->fgIsWaitFlush)
    {
        if(bIsWait)
        {
            //binary semaphore, can't handle multiple flush sync cmd
            LOG(0, "[PreScale]Warning, IgnoreSyncCmd\n");
        }
        else
        {
            LOG(1, "[PreScale]IgnoreCmd, already flush\n");
        }
        VERIFY(x_sema_unlock(prPreScaleInfo->hFlushMutex) == OSR_OK);
        return;
    }
    else
    {
        LOG(1, "[Prescale]FlushCmd (%d, %d, ID %d)\n", ucRzId, bIsWait, _ucCmdID);
    }
    
    if(bIsWait)
    {
        prPreScaleInfo->fgIsWaitFlush = TRUE;
    }
	
    x_memset((void*)&rPreScaleCmd, 0, sizeof(VDP_PRESCALE_CMD_T));
    rPreScaleCmd.ucCmd = PRESCALE_CMD_FLUSH;
    rPreScaleCmd.fgIsWait = bIsWait;
    rPreScaleCmd.ucID = _ucCmdID;
    _ucCmdID =  (_ucCmdID==0xFF) ? 1 : (_ucCmdID + 1);  //0 as default value, set between 1-0xFF
        
    if((i4Ret = x_msg_q_send(prPreScaleInfo->hRzCmdQ, (void *)&rPreScaleCmd,
        sizeof(VDP_PRESCALE_CMD_T), 255)) != OSR_OK)
    {
        LOG(1, "VDP Prescale Cmd Overflow(%d)\n",i4Ret);
    }
    
    //dummy message
    x_memset((void*)&rPreScaleFrm, 0, sizeof(VDP_PRESCALE_FRM_T));
    rPreScaleFrm.ucFbgId = 0xFF;
    rPreScaleFrm.ucFbId = 0xFF;
    rPreScaleFrm.ucCmdId = rPreScaleCmd.ucID;
    if((i4Ret = x_msg_q_send(prPreScaleInfo->hRzMsgQ, (void *)&rPreScaleFrm,
        sizeof(VDP_PRESCALE_FRM_T), 255)) != OSR_OK)
    {
        LOG(1, "VDP Prescale Flush Overflow(%d)\n",i4Ret);
        u2ReSendMsgCnt++;
        //error handling
        while(u2ReSendMsgCnt > 0)
        {
            if(x_msg_q_receive(&u2MsgQIdx, &rPreScaleFrmRsv, &zMsgSize,
                &(prPreScaleInfo->hRzMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
            {
                if(rPreScaleFrmRsv.ucFbgId == 0xFF)    //keep dummy message for cmd
                {
                    if((i4Ret = x_msg_q_send(prPreScaleInfo->hRzMsgQ, (void *)&rPreScaleFrmRsv,
                        sizeof(VDP_PRESCALE_FRM_T), 255)) != OSR_OK)
                    {
                        LOG(1, "VDP Prescale lost dummy msg for cmd\n");
                        u2ReSendMsgCnt++;
                    }
                }
                else
                {
                    if((i4Ret = x_msg_q_send(prPreScaleInfo->hRzMsgQ, (void *)&rPreScaleFrm,
                        sizeof(VDP_PRESCALE_FRM_T), 255)) != OSR_OK)
                    {
                        LOG(1, "VDP Prescale lost dummy msg for cmd\n");
                        u2ReSendMsgCnt++;
                    }
                    else
                    {
                        u2ReSendMsgCnt--;
                        if(u2ReSendMsgCnt == 0)
                        {
                            break;  //normal case
                        }
                    }
                }
            }
            else
            {
                LOG(1, "VDP Prescale lost dummy msg for cmd, cnt %d\n", u2ReSendMsgCnt);
                break;
            }
        }
    }
    
    if(bIsWait)
    {
        VERIFY(x_sema_unlock(prPreScaleInfo->hFlushMutex) == OSR_OK);
        VERIFY(x_sema_lock(prPreScaleInfo->hFlushSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
    else
    {
        VERIFY(x_sema_unlock(prPreScaleInfo->hFlushMutex) == OSR_OK);
    }
}


BOOL _VDP_Prescale_RegGetTgBufCb(UCHAR ucRzId, 
    PFN_PRESCALE_GETTGBUF_CB pfnGetTgBufCb, VDP_PRESCALE_TG_INFO_T *prTgBufInfo, VOID *pvTag)
{
    VDP_PRESCALE_INFO_T* prPreScaleInfo;

    if(ucRzId >= IMGRZ4VDO_NUM)
    {
        LOG(1, "IMGRZ ID %d unavailable\n", ucRzId);
        return FALSE;
    }
    prPreScaleInfo = &_arPreScaleInfo[ucRzId];

    prPreScaleInfo->pfnGetExTgBuf = pfnGetTgBufCb;
    prPreScaleInfo->pvTgBufInfoTag = pvTag;
    x_memcpy((void*)&prPreScaleInfo->rTgBufInfo, (void*)prTgBufInfo, sizeof(VDP_PRESCALE_TG_INFO_T));

    return TRUE;
}


BOOL _VDP_Prescale_RegRzFinCb(UCHAR ucRzId, PFN_PRESCALE_RZFIN_CB pfnRzFinCb, VOID *pvTag)
{
    VDP_PRESCALE_INFO_T* prPreScaleInfo;

    if(ucRzId >= IMGRZ4VDO_NUM)
    {
        LOG(1, "IMGRZ ID %d unavailable\n", ucRzId);
        return FALSE;
    }
    prPreScaleInfo = &_arPreScaleInfo[ucRzId];

    prPreScaleInfo->pfnRzFinCb = pfnRzFinCb;
    prPreScaleInfo->pvRzFinCbTag = pvTag;
    return TRUE;
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_Prescale_MainLoop.
 */
//-----------------------------------------------------------------------------

static void _VDP_Prescale_MainLoop(void* pvArg)
{
    UCHAR ucRzId, ucRetValue;
    SIZE_T zMsgSize;
    UINT16 u2MsgQIdx;
    VDP_PRESCALE_INFO_T* prPreScaleInfo;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    VDP_PRESCALE_FRM_T rPreScaleFrm;
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
 #if defined (CC_MT5890) || defined (CC_MT5861)
    VDP_CAPTURE_INTO_T rCapSrcInfo;
 #endif
#ifdef CC_PRESCALE_PROFILE
    HAL_TIME_T rTimeDt1, rTimeDt2, rTimeDt3;
    STC_CLOCK rStcClk;
    UINT32 u4TimeDiff=0;
#endif
    VDP_PRESCALE_TG_BUF_T rTgBuf;
    VDP_PRESCALE_RZFIN_CB_T rRzFinCBInfo;
    
    if(pvArg == NULL)
    {
        ASSERT(!pvArg);
        ucRzId = 0;
        LOG(1, "_VDP_Prescale_MainLoop(): (pvArg == NULL)\n");
    }
    else
    {
        ucRzId = *(UCHAR*)pvArg;
        if(ucRzId >= IMGRZ4VDO_NUM)
        {
            LOG(1, "IMGRZ ID %d unavailable\n", ucRzId);
            return;
        }
    }

    prPreScaleInfo = &_arPreScaleInfo[ucRzId];
    u2MsgQIdx = 0;
    zMsgSize = sizeof(VDP_PRESCALE_FRM_T);
    x_memset((void*)&rPreScaleFrm, 0, sizeof(VDP_PRESCALE_FRM_T));
    x_memset((void*)&rTgBuf, 0, sizeof(VDP_PRESCALE_TG_BUF_T));
    rPreScaleFrm.ucFbgId = 0xFF;
    rPreScaleFrm.ucFbId = 0xFF;
    
    while (_fgPreScaleInitiated[ucRzId])
    {
    	if (FALSE == _fgPreScaleAlloc[ucRzId])
    	{
    		break;
    	}
//--------------  get command
        _VDPPrescaleReceiveCmd(ucRzId);

//--------------  get resize source
        if(!prPreScaleInfo->fgRetry)
        {
            VERIFY(x_msg_q_receive(&u2MsgQIdx, &rPreScaleFrm, &zMsgSize,
                &(prPreScaleInfo->hRzMsgQ), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
            prPreScaleInfo->ucCmdId = rPreScaleFrm.ucCmdId;
        }        

        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(rPreScaleFrm.ucFbgId);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);

        if(prFbmSeqHdr && prFbmPicHdr)
        {
	        if(rPreScaleFrm.ucTaskId != prFbmPicHdr->ucSeamlessTaskId)
	        {
	            prPreScaleInfo->fgRetry = FALSE;
	            LOG(1, "Skip Rz not match taskid (%d, %d)\n", rPreScaleFrm.ucTaskId, prFbmPicHdr->ucSeamlessTaskId);
	            continue;
	        }
	        
            x_memset(&rScaleParamV, 0, sizeof(RZ_VDO_SCL_PARAM_SET_T));
            if (10 == prFbmSeqHdr->u1Depth)
            {
                rScaleParamV.u4UfoEn = TRUE;
                rScaleParamV.eUfoType = RZ_UFO_TYPE_10BIT_COMPACT;           
            }
            else if(8 == prFbmSeqHdr->u1Depth)
            {
                rScaleParamV.u4UfoEn = prFbmSeqHdr->fgUfoEn;
                rScaleParamV.eUfoType = RZ_UFO_TYPE_8BIT;
            }

            if(rScaleParamV.u4UfoEn)
            {
                FBM_GetFrameBufferAddrExt(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId,&(rScaleParamV.u4UfoYBufSZSA),&(rScaleParamV.u4UfoCBufSZSA));
                if((10 == prFbmSeqHdr->u1Depth) && (!prFbmSeqHdr->fgUfoEn))
                {                 
                    GDMA_MemSet((VOID*)rScaleParamV.u4UfoYBufSZSA,0xFFFFFFFF,((prFbmPicHdr->u4PicWidthPitch * D_RZ_ALIGN32(prFbmPicHdr->u4PicHeight) *5)>> 10));                    
                    GDMA_MemSet((VOID*)rScaleParamV.u4UfoCBufSZSA,0xFFFFFFFF,((prFbmPicHdr->u4PicWidthPitch * D_RZ_ALIGN32(prFbmPicHdr->u4PicHeight) *5) >> 11));                    
                }
            }

            FBM_GetFrameBufferAddr(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId, &rScaleParamV.u4YSrcBase, &rScaleParamV.u4CSrcBase);
            //uncompress if needed. 10bit & 8bit + ufo
            //8bit + ufo need step if fgCropping == true, TBD
            #if defined (CC_MT5890)		
			if(!(IS_IC_5861_ES2()|| IS_IC_5890_ES3()))				
			{
            	if(rScaleParamV.u4UfoEn && (prFbmPicHdr->u4PicWidth != prFbmPicHdr->u4PicWidthPitch))
	            {
	                rCapSrcInfo.fgUFOOn = TRUE;
	                rCapSrcInfo.u4IsRsIn = 0;//16x32
	                rCapSrcInfo.u4SrcSwap = 0;
	                rCapSrcInfo.u4UFOType = (prFbmSeqHdr->u1Depth == 8) ? RZ_UFO_TYPE_8BIT : RZ_UFO_TYPE_10BIT_COMPACT;
	                rCapSrcInfo.u4MemHSize = prFbmPicHdr->u4PicWidthPitch;
	                rCapSrcInfo.u4MemVSize = ((prFbmPicHdr->u4PicHeight + 31) >> 5) << 5;
	                rCapSrcInfo.u4HSize = prFbmPicHdr->u4PicWidthPitch;
	                rCapSrcInfo.u4VSize = prFbmPicHdr->u4PicHeight;
	                
	                rCapSrcInfo.u4AddrY = rScaleParamV.u4YSrcBase;
	                rCapSrcInfo.u4AddrC = rScaleParamV.u4CSrcBase;
	                rCapSrcInfo.u4DstY = prPreScaleInfo->u4UncompressYaddr;
	                rCapSrcInfo.u4DstC = prPreScaleInfo->u4UncompressCaddr;
	                rCapSrcInfo.u4AddrYLen = rScaleParamV.u4UfoYBufSZSA;
	                rCapSrcInfo.u4AddrCLen = rScaleParamV.u4UfoCBufSZSA;
					
	                IMGRZ_DecompressUFO((void*)&rCapSrcInfo);

	                rScaleParamV.u4YSrcBase = rCapSrcInfo.u4DstY;
	                rScaleParamV.u4CSrcBase = rCapSrcInfo.u4DstC;
	                rScaleParamV.u4UfoEn = FALSE;
	            }	
			}
            #endif

            rScaleParamV.u4VdoCbCrSwap =0;  //todo2, set center position by aspect ratio?
            rScaleParamV.u4IsRsIn = (prFbmSeqHdr->fgRasterOrder) ? 1 : 0;
            rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_420;
            rScaleParamV.u4YSrcBufLen = prFbmPicHdr->u4PicWidthPitch;
            rScaleParamV.u4YSrcHOffset = prFbmPicHdr->fgCropping ? prFbmPicHdr->u4CropX : 0;
            rScaleParamV.u4YSrcVOffset = prFbmPicHdr->fgCropping ? prFbmPicHdr->u4CropY : 0;
            rScaleParamV.u4YSrcW = prFbmPicHdr->fgCropping ? prFbmPicHdr->u4CropWidth : prFbmPicHdr->u4PicWidth;
            rScaleParamV.u4YSrcH = prFbmPicHdr->fgCropping ? prFbmPicHdr->u4CropHeight : prFbmPicHdr->u4PicHeight;
            rScaleParamV.u4CSrcHOffset = rScaleParamV.u4YSrcHOffset >> 1;
            rScaleParamV.u4CSrcVOffset = rScaleParamV.u4YSrcVOffset >> 1;
            rScaleParamV.u4CSrcW = rScaleParamV.u4YSrcW >> 1;
            rScaleParamV.u4CSrcH = rScaleParamV.u4YSrcH >> 1;
			
			
                    
//--------------  get resize target
            HAL_GetTime(&rPreScaleFrm.rTimeRzWaitFB);

            if(prPreScaleInfo->pfnGetExTgBuf)
            {
                if(!prPreScaleInfo->pfnGetExTgBuf(prPreScaleInfo->pvTgBufInfoTag, &rTgBuf))
                {
                    rPreScaleFrm.ucRzFbId = FBM_FB_ID_UNKNOWN;
                    ucRetValue = VDP_PRESCALE_SKIP;
                }
                else
                {
                    rPreScaleFrm.ucRzFbId = 0;
                    prFbmPicHdr->ucSeamlessResizeFbid = 0;
                }
            }
            else
            {
                if(prPreScaleInfo->pfnGetRzBuf != NULL)
                {
                    rPreScaleFrm.ucRzFbId = prPreScaleInfo->pfnGetRzBuf(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId, rPreScaleFrm.ucTaskId, 1000, &ucRetValue);
                }
                rScaleParamV.u4IsRsOut = (prFbmSeqHdr->fgRasterOrder) ? 1 : 0;
//                if (ucRzId == 0)
//                {
//                    rPreScaleFrm.ucRzFbId = FBM_GetEmptyResizeFrameBuffer(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId, rPreScaleFrm.ucTaskId, 1000, &ucRetValue);
//                    rScaleParamV.u4IsRsOut = (prFbmSeqHdr->fgRasterOrder) ? 1 : 0;
//                }
//                else if (ucRzId == 1)
//                {
//                    rPreScaleFrm.ucRzFbId = FBM_GetEmptyResize2FrameBuffer(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId, rPreScaleFrm.ucTaskId, 1000, &ucRetValue);
//                    rScaleParamV.u4IsRsOut = (prFbmSeqHdr->fgRasterOrder) ? 1 : 0;
//                }
            }

            //retry: vdp pause or repeat
            if(rPreScaleFrm.ucRzFbId == FBM_FB_ID_UNKNOWN)
            {
                if(ucRetValue == VDP_PRESCALE_RETRY)
                {
                prPreScaleInfo->fgRetry = TRUE;
                LOG(2, "GetRzFb fail, Fbg %d, Fb %d, retry..\n", rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);
                }
                else    //VDP_PRESCALE_SKIP
                {
                    LOG(2, "GetRzFb fail, Fbg %d, Fb %d, skip..\n", rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);
                }
            }
            else
            {
                prPreScaleInfo->fgRetry = FALSE;
                if(prPreScaleInfo->pfnGetExTgBuf)
                {
                    rScaleParamV.u4YTgBase = rTgBuf.u4YTgBase;
                    rScaleParamV.u4CTgBase = rTgBuf.u4CTgBase;
                    rScaleParamV.u4YTgW = prPreScaleInfo->rTgBufInfo.u4YTgW;
                    rScaleParamV.u4YTgH = prPreScaleInfo->rTgBufInfo.u4YTgH;
                    rScaleParamV.u4YTgBufLen= prPreScaleInfo->rTgBufInfo.u4YTgBufLen;
                    rScaleParamV.u4IsRsOut = prPreScaleInfo->rTgBufInfo.fgRasterOrder;
                }
                else
                {
                    if(prPreScaleInfo->pfnGetRzBufAddr != NULL)
                    {
                        prPreScaleInfo->pfnGetRzBufAddr(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, &rScaleParamV.u4YTgBase, &rScaleParamV.u4CTgBase);
                    }
//                    if (ucRzId == 0)
//                    {
//                        FBM_GetResizeFrameBufferAddr(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, &rScaleParamV.u4YTgBase, &rScaleParamV.u4CTgBase);
//                    }
//                    else if (ucRzId == 1)
//                    {
//                        FBM_GetResize2FrameBufferAddr(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, &rScaleParamV.u4YTgBase, &rScaleParamV.u4CTgBase);
//                    }
                    
                    rScaleParamV.u4YTgW = prFbmSeqHdr->u4ResizeWidth;
                    rScaleParamV.u4YTgH = prFbmSeqHdr->u4ResizeHeight;
                    //use resize width as pitch when auto seamless
                    rScaleParamV.u4YTgBufLen= 
                        (prFbmPicHdr->fgSeamlessDisp) ? prFbmSeqHdr->u2LineSize : prFbmSeqHdr->u4ResizeWidth;
                    rScaleParamV.u4IsRsOut = (prFbmSeqHdr->fgRasterOrder) ? 1 : 0;
                }
                rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_420;
                rScaleParamV.u4IsVdo2Osd = 0;
                rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_420;

                if(rScaleParamV.u4YTgW&0x1)
                {
                    rScaleParamV.u4YTgW--;
                }

                if(rScaleParamV.u4YTgH&0x1)
                {
                    rScaleParamV.u4YTgH--;
                }
                rScaleParamV.u4CTgW=rScaleParamV.u4YTgW>>1;
                rScaleParamV.u4CTgH=rScaleParamV.u4YTgH>>1;

                //don't need to handle centralization, APP will set source region after vdp callback. 
                rScaleParamV.u4YTgHOffset = 0;//(prFbmSeqHdr->u2HSize - prFbmSeqHdr->u4ResizeWidth) >> 1;
                rScaleParamV.u4YTgVOffset = 0;//(prFbmSeqHdr->u2VSize - prFbmSeqHdr->u4ResizeHeight) >> 1;

                rScaleParamV.u4OutSwap = (rScaleParamV.u4IsRsOut || (prFbmSeqHdr->ucBlockType == 1)) ? 0 : 6;  //0: 16*32 , 6:64*32
                rScaleParamV.u4SrcSwap = (prFbmSeqHdr->fgRasterOrder || (prFbmSeqHdr->ucBlockType == 1)) ? 0 : 6;  //0: 16*32,  6:64*32

    //--------------  start resizing
                HAL_GetTime(&rPreScaleFrm.rTimeRzStart);
#ifdef CC_INTERLACE_RESIZING
                if(!prFbmSeqHdr->fgProgressiveSeq)
                {
                    rScaleParamV.u4FieldTypeEnable = 1;
                    rScaleParamV.u4FieldType = E_RZ_VDO_TOP_FILED;
#if 1
                    rScaleParamV.u4YSrcH >>= 1;
                    rScaleParamV.u4CSrcH >>= 1;
                    rScaleParamV.u4YTgH >>= 1;
                    rScaleParamV.u4CTgH >>= 1;
#endif
                }
#endif
                // error handle, maybe fgb has released by vdec.
                if(FBM_GetFrameBufferPicHdr(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId) == NULL)
                {   
                    if(prPreScaleInfo->pfnSetRzBufStatus != NULL)
                    {
                        prPreScaleInfo->pfnSetRzBufStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_EMPTY);
                        LOG(3, "[%s-%d] FB(%d, %d) PicHdr is Null\n", __func__, __LINE__, rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);
                    }
//                    if (ucRzId == 0)  
//                    {
//                        FBM_SetResizeFrameBufferStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_EMPTY);
//                        LOG(3, "[%s-%d] FB(%d, %d) PicHdr is Null\n", __func__, __LINE__, rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);
//                    }
//                    else if (ucRzId == 1)
//                    {
//                        FBM_SetResize2FrameBufferStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_EMPTY);
//                        LOG(3, "[%s-%d] FB(%d, %d) PicHdr is Null\n", __func__, __LINE__, rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);
//                    }
                    continue;
                }
				
                //frame or top field
                IMGRZ_Lock_Ex((UINT32)ucRzId);
                IMGRZ_ReInit_Ex((UINT32)ucRzId);
                IMGRZ_SetScaleOpt_Ex((UINT32)ucRzId, E_RZ_INOUT_TYPE_VDOMD);
                IMGRZ_Scale_Ex((UINT32)ucRzId, (void *)(&rScaleParamV));
                IMGRZ_Flush_Ex((UINT32)ucRzId);
                IMGRZ_Wait_Ex((UINT32)ucRzId);
                IMGRZ_Unlock_Ex((UINT32)ucRzId);

#ifdef CC_INTERLACE_RESIZING
                //bottom field
                if(!prFbmSeqHdr->fgProgressiveSeq)
                {
                    rScaleParamV.u4FieldTypeEnable = 1;
                    rScaleParamV.u4FieldType = E_RZ_VDO_BOTTOM_FILED;
                    
                    IMGRZ_Lock_Ex((UINT32)ucRzId);
                    IMGRZ_ReInit_Ex((UINT32)ucRzId);
                    IMGRZ_SetScaleOpt_Ex((UINT32)ucRzId, E_RZ_INOUT_TYPE_VDOMD);
                    IMGRZ_Scale_Ex((UINT32)ucRzId, (void *)(&rScaleParamV));
                    IMGRZ_Flush_Ex((UINT32)ucRzId);
                    IMGRZ_Wait_Ex((UINT32)ucRzId);
                    IMGRZ_Unlock_Ex((UINT32)ucRzId);
                }
#endif

                //--------------  update resize status
                if(FBM_GetFrameBufferPicHdr(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId) == NULL)
                {

                    if(prPreScaleInfo->pfnSetRzBufStatus != NULL)
                    {
                        prPreScaleInfo->pfnSetRzBufStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_EMPTY);
                        LOG(3, "[%s-%d] FB(%d, %d) PicHdr is Null\n", __func__, __LINE__, rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);
                    }
//                    if (ucRzId == 0)  
//                    {
//                        FBM_SetResizeFrameBufferStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_EMPTY);
//                        LOG(3, "[%s-%d] FB(%d, %d) PicHdr is Null\n", __func__, __LINE__, rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);                        
//                    }
//                    else if (ucRzId == 1)
//                    {
//                        FBM_SetResize2FrameBufferStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_EMPTY);
//                        LOG(3, "[%s-%d] FB(%d, %d) PicHdr is Null\n", __func__, __LINE__, rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);                        
//                    }                
                    continue;
                }

                if(prPreScaleInfo->pfnSetRzBufStatus != NULL)
                {
                    prPreScaleInfo->pfnSetRzBufStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_RESIZE_DONE);
                    LOG(3, "[%s-%d] FB(%d, %d) PicHdr is Null\n", __func__, __LINE__, rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId);
                }
                
//                if (ucRzId == 0)  
//                {
//                    FBM_SetResizeFrameBufferStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_RESIZE_DONE);                
//                }
//                else if (ucRzId == 1)
//                {
//                    FBM_SetResize2FrameBufferStatus(rPreScaleFrm.ucFbgId, rPreScaleFrm.ucRzFbId, FBM_FB_STATUS_RESIZE_DONE);                
//                }			
                if(prPreScaleInfo->pfnRzFinCb)
                {
                    rRzFinCBInfo.u4YTgBase = rScaleParamV.u4YTgBase;
                    rRzFinCBInfo.u4CTgBase = rScaleParamV.u4CTgBase;
                    rRzFinCBInfo.u4PTS = prFbmPicHdr->u4PTS;
                    prPreScaleInfo->pfnRzFinCb(prPreScaleInfo->pvRzFinCbTag, &rRzFinCBInfo);
                    // if not display, set lock/empty
                }

                HAL_GetTime(&rPreScaleFrm.rTimeRzEnd);

                x_memcpy((void*)&prFbmPicHdr->rTimePutDispQ, (void*)&rPreScaleFrm.rTimePutDispQ, sizeof(HAL_TIME_T));
                x_memcpy((void*)&prFbmPicHdr->rTimeRzGetFB, (void*)&rPreScaleFrm.rTimeRzWaitFB, sizeof(HAL_TIME_T));
                x_memcpy((void*)&prFbmPicHdr->rTimeGotFBRzStart, (void*)&rPreScaleFrm.rTimeRzStart, sizeof(HAL_TIME_T));
                x_memcpy((void*)&prFbmPicHdr->rTimeRzEnd, (void*)&rPreScaleFrm.rTimeRzEnd, sizeof(HAL_TIME_T));

                LOG(2, "FBG%d, FB%d/Rz%d with Rzidx %d PTS 0x%x\n", rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId, rPreScaleFrm.ucRzFbId,ucRzId,prFbmPicHdr->u4PTS);				
#ifdef CC_PRESCALE_PROFILE
                if(u4ProfileEn > 0)
                {
                    HAL_GetDeltaTime(&rTimeDt1, &rPreScaleFrm.rTimePutDispQ, &rPreScaleFrm.rTimeRzWaitFB);
                    HAL_GetDeltaTime(&rTimeDt2, &rPreScaleFrm.rTimeRzWaitFB, &rPreScaleFrm.rTimeRzStart);
                    HAL_GetDeltaTime(&rTimeDt3, &rPreScaleFrm.rTimeRzStart, &rPreScaleFrm.rTimeRzEnd);
                    if (STC_GetSrc(0, &rStcClk) != STC_VALID)
                    {
                        rStcClk.u4Base = 0;
                    }
                    u4TimeDiff = (rStcClk.u4Base > prFbmPicHdr->u4PTS) ? (((rStcClk.u4Base - prFbmPicHdr->u4PTS))/90) : 0;

                    LOG(2, "FBG%d, FB%d/Rz%d, PutQ-RcvRzQ(%dms), WaitTgFb(%dms), Rz(%dms,p=%d), PTS/STC(0x%x/0x%x/late%dms)\n", 
                        rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId, rPreScaleFrm.ucRzFbId,
                        rTimeDt1.u4Micros/1000, rTimeDt2.u4Micros/1000, rTimeDt3.u4Micros/1000, 
                        (UINT32)prFbmSeqHdr->fgProgressiveSeq,
                        prFbmPicHdr->u4PTS, rStcClk.u4Base, u4TimeDiff);
                }

                LOG(2, "FBG%d, FB%d/Rz%d, PutQ-RcvRzQ(%dms), WaitTgFb(%dms), Rz(%dms,p=%d), PTS/STC(0x%x/0x%x/late%dms)\n", 
                    rPreScaleFrm.ucFbgId, rPreScaleFrm.ucFbId, rPreScaleFrm.ucRzFbId,
                    rTimeDt1.u4Micros/1000, rTimeDt2.u4Micros/1000, rTimeDt3.u4Micros/1000, 
                    (UINT32)prFbmSeqHdr->fgProgressiveSeq,
                    prFbmPicHdr->u4PTS, rStcClk.u4Base, u4TimeDiff);

#endif
            }
        }
		else
		{
			prPreScaleInfo->fgRetry = FALSE;
		}
    }
    VERIFY(x_sema_unlock(prPreScaleInfo->hThdSyncCloseSema) == OSR_OK);
    VERIFY(x_msg_q_delete(prPreScaleInfo->hRzMsgQ) == OSR_OK);
    VERIFY(x_msg_q_delete(prPreScaleInfo->hRzCmdQ) == OSR_OK);
    VERIFY(x_sema_delete(prPreScaleInfo->hFlushSema) == OSR_OK);
    VERIFY(x_sema_delete(prPreScaleInfo->hFlushMutex) == OSR_OK);
    _fgPreScaleInitiated[ucRzId] = FALSE;
    x_thread_exit();
}

void _VDP_Prescale_Close(UCHAR ucRzId)
{
    VDP_PRESCALE_INFO_T* prPreScaleInfo = NULL;
    prPreScaleInfo = &_arPreScaleInfo[ucRzId];
    if((ucRzId < IMGRZ4VDO_NUM) && _fgPreScaleAlloc[ucRzId])
    {
        _fgPreScaleAlloc[ucRzId] = FALSE;
        //Sync wait thread exit.
        if(prPreScaleInfo->hThdSyncCloseSema != NULL_HANDLE)
        {
        	_VDP_Prescale_Send(ucRzId, FBM_FBG_ID_UNKNOWN, FBM_FB_ID_UNKNOWN, 0xFF);//send msg first. as the sema lock will wait
            VERIFY(x_sema_lock(prPreScaleInfo->hThdSyncCloseSema, X_SEMA_OPTION_WAIT) == OSR_OK);
            VERIFY(x_sema_delete(prPreScaleInfo->hThdSyncCloseSema) == OSR_OK);
            prPreScaleInfo->hThdSyncCloseSema = NULL_HANDLE;
        }
        FBM_FreeFixedImgRzBuffer(prPreScaleInfo->u4UncompressYaddr,prPreScaleInfo->u4UncompressCaddr);
        LOG(1, "_VDP_Prescale_Close sync done .\n");
    }
    else
    {
        LOG(1, "_VDP_Prescale_Close wrong ID %d\n", ucRzId);
    }
}

UCHAR _VDP_GetRzId(UCHAR ucRzIdMask,UCHAR ucTaskId)
{
    INT32 i4Idx,i4RzCnt = 0;
    UCHAR ucRzIdx[IMGRZ4VDO_NUM];
    
    for (i4Idx = 0; i4Idx < IMGRZ4VDO_NUM; i4Idx++)
    {
        if(ucRzIdMask & (0x01<<i4Idx))
        {
            ucRzIdx[i4RzCnt] = i4Idx;
            i4RzCnt ++;
        }
    }

    return (ucRzIdx[ucTaskId%i4RzCnt]);
}

void _VDP_SetPfn_RzGetFB(UCHAR ucRzId,PFN_GET_GETRZBUF pfnGetRzFb)
{
    VDP_PRESCALE_INFO_T* prPreScaleInfo;
    if(ucRzId >= IMGRZ4VDO_NUM)
    {
        LOG(1, "IMGRZ ID %d unavailable\n", ucRzId);
        return ;
    }
    prPreScaleInfo = &_arPreScaleInfo[ucRzId];
    prPreScaleInfo->pfnGetRzBuf = pfnGetRzFb;
}

void _VDP_SetPfn_RzGetFBAddr(UCHAR ucRzId,PFN_GET_GETRZBUFADDR pfnGetRzFbAddr)
{
    VDP_PRESCALE_INFO_T* prPreScaleInfo;
    if(ucRzId >= IMGRZ4VDO_NUM)
    {
        LOG(1, "IMGRZ ID %d unavailable\n", ucRzId);
        return ;
    }
    prPreScaleInfo = &_arPreScaleInfo[ucRzId];
    prPreScaleInfo->pfnGetRzBufAddr = pfnGetRzFbAddr;
}


void _VDP_SetPfn_RzSetFBStatus(UCHAR ucRzId,PFN_SET_SETRZBUFSTATUS pfnSetRzFbStatus)
{
    VDP_PRESCALE_INFO_T* prPreScaleInfo;
    if(ucRzId >= IMGRZ4VDO_NUM)
    {
        LOG(1, "IMGRZ ID %d unavailable\n", ucRzId);
        return ;
    }
    prPreScaleInfo = &_arPreScaleInfo[ucRzId];
    prPreScaleInfo->pfnSetRzBufStatus = pfnSetRzFbStatus;
}


