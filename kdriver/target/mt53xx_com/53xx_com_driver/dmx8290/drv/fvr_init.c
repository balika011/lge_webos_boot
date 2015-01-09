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
 * $RCSfile: fvr_init.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_init.c
 *  FVR driver - FVR
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_util.h"

LINT_EXT_HEADER_BEGIN
#include "x_ckgen.h"
#include "drv_common.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DMX_FVR_QUEUE_NAME                  "FVRQ"
#define DMX_FVR_QUEUE_SIZE                  1024


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _afgFVRIsRecording[FVR_GBL_COUNT];
static HANDLE_T _hFvrQueue = NULL_HANDLE;
static HANDLE_T _hFvrThread = NULL_HANDLE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _SetFVRTimer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _SetFVRTimer(UINT8 u1TsIdx)
{
    FVR_GBL_STRUCT_T* prGBL;
    UINT32 u4Clk, u4Compare, u4Div, u4Reg;

    prGBL = _FVR_GetGBLStruct(u1TsIdx);
    if (prGBL == NULL)
    {
        return FALSE;
    }

    if (prGBL->u4TimeToNotify == 0)
    {
        FVR_GBL_TIMEINFO = 0x0;
        u4Reg = FVR_GBL_TIMEINFO_UPDATE & 0xFFFFFF00;
        u4Reg |= 0x1;
        FVR_GBL_TIMEINFO_UPDATE = u4Reg;
        return TRUE;
    }

#ifndef CC_DMX_FPGA_TEST
    u4Clk = FVR_CLOCK;
#else
    u4Clk = 27000000;
#endif

    u4Compare = 0xFFFF;
    u4Div = ((u4Clk / u4Compare) * prGBL->u4TimeToNotify) / 1000;
    FVR_GBL_TIMEINFO = ((u4Div & 0xFFFF) << 16) |
                          (u4Compare & 0xFFFF) ;

    u4Reg = FVR_GBL_TIMEINFO_UPDATE & 0xFFFFFF00;
    u4Reg |= 0x1;
    FVR_GBL_TIMEINFO_UPDATE = u4Reg;

    LOG(0, "Time dividor: 0x%x, compare val: 0x%x\n", u4Div, u4Compare);

    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _FVR_SoftInit
 *  Soft-initialize demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _FVR_SoftInit(void)
{
    UINT32 u4Ctrl, i;
    FVR_GBL_STRUCT_T* prGblPidStruct;

    _FVR_Lock();

    // Clear global 0 table and struct
    for (i = 0; i < FVR_GBL_COUNT ; i++)
    {
        x_memset((void*)&(FVR_GBL_ARY_W(i, 0)), 0, FVR_GBL_SIZE * 4);
        prGblPidStruct = _FVR_GetGBLStruct(i);
        x_memset((void*)prGblPidStruct, 0, sizeof(FVR_GBL_STRUCT_T));
    }

    // Clear PID index table and DMEM
    for (i = 0; i < FVR_NUM_PID_INDEX; i++)
    {
        FVR_PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(FVR_PER_PID_S(i)), 0, FVR_PID_SIZE * 4);
    }

    // Clear Per-Pid struct
    for (i = 0; i < FVR_NUM_PID_INDEX; i++)
    {
        FVR_PID_STRUCT_T* prPidStruct;
        UINT8 u1SerialNum;

        prPidStruct = _FVR_GetPidStruct(i);
        u1SerialNum = prPidStruct->u1SerialNumber;
        x_memset((void*)prPidStruct, 0, sizeof (FVR_PID_STRUCT_T));
        prPidStruct->u1SerialNumber = u1SerialNum;
    }

    _FVR_Unlock();

    //Clear KEy
    x_memset((void*)FVR_KEY_BASE, 0, (FVR_KEY_COUNT * 96));

    // Initialize common routines
    _FVR_InitAPI();

    _FVR_Lock();

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF;
    u4Ctrl |= (FVR_PER_PID_OFFSET) << 16;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFD00FEFF;
    u4Ctrl |= (1 << 25);                    // Disable record path
    u4Ctrl |= ((FVR_PID_SIZE * 4) << 16);
    u4Ctrl |= (1 << 8);                     // Direct map
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Disable insert timestamp
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2) & 0xDFFFFFFF;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);

    for (i = 0; i < FVR_GBL_COUNT ; i++)
    {

        FVR_GBL_ARY_W(i, 2) = 0x100;    // Disable record region 0
    }

    _FVR_Unlock();

    return TRUE;
}

static VOID _DmxFvrThread(VOID* pvArg)
{
   UNUSED(pvArg);

    while (1)
    {
        INT32 i4Ret;
        UINT32 u4Count;
        SIZE_T zMsgSize;
        UINT16 u2QueueIndex;
        FVR_MSG_T rMsg;
    
        zMsgSize = sizeof (rMsg);
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hFvrQueue, 1, X_MSGQ_OPTION_WAIT);

        if (i4Ret != OSR_OK)
        {
            LOG(1, "Error on receiving fvr message!\n");
            continue;
        }

        ASSERT(zMsgSize == sizeof (rMsg));
        ASSERT(u2QueueIndex == 0);
        
        switch (rMsg.eMsgType)
        {
        case FVR_MSG_SITCOMPLETE:
            {
                FVR_SIT_PKTREPLACE_INFO_T *prSitInfo;

                prSitInfo = &rMsg.uMsgData.rSitInfo;
                _FVR_SitReplacePacket(prSitInfo->u1TsIndex, prSitInfo->u1SitPktCnt, TRUE);
                
                LOG(3, "Receive FVR_MSG_SITCOMPLETE\n");
            }
            break;

        case FVR_MSG_PICSEARCH:
            {
                FVR_PID_STRUCT_T *prPid;
                FVR_PID_PICNOTIFY_INFO_T *prPicInfo;

                prPicInfo = &rMsg.uMsgData.rPicInfo;
                prPid = _FVR_GetPidStruct(prPicInfo->u1Pidx);
                u4Count = _FVR_SitQueryInsertedPacketCount(prPid->u1TsIndex);
                if (u4Count)
                {
                    LOG(0, "PIC: SIT count=%u, addr=0x%08x\n", 
                        u4Count, FVR_SIT_ARY_W(prPid->u1TsIndex, u4Count-1));
                    
                    _FVR_SitReplacePacket(prPid->u1TsIndex, u4Count, FALSE);
                }
        
                if (0xFF == prPicInfo->u2PicType)
                {
                    // insert DIT
                    _FVR_ReplaceDITPacket(prPicInfo->u1Pidx, prPicInfo->u4PktCount);
                }

                if (prPid->pfnPicSearchNotify != NULL)
                {
                    prPid->pfnPicSearchNotify(prPicInfo->u1Pidx, prPicInfo->u2PicType, 
                                            prPicInfo->u4PktCount, prPicInfo->u8Pts33, 
                                            prPid->pvSwapNotifyTag);
                }
                
                LOG(3, "Receive FVR_MSG_PICSEARCH - 0x%08x\n", prPid->pfnPicSearchNotify);
            }
            break;

        case FVR_MSG_TIMERPKT:
            {
                FVR_GBL_STRUCT_T *prGBL;
                FVR_GBL_TIMENOTIFY_INFO_T *prTimeInfo;

                prTimeInfo = &rMsg.uMsgData.rTimeInfo;
                u4Count = _FVR_SitQueryInsertedPacketCount(prTimeInfo->u1TsIndex);
                if (u4Count)
                {
                    LOG(0, "TIMER: SIT count=%u, addr=0x%08x\n", 
                        u4Count, FVR_SIT_ARY_W(prTimeInfo->u1TsIndex, u4Count-1));
                    
                    _FVR_SitReplacePacket(prTimeInfo->u1TsIndex, u4Count, FALSE);
                }
                prGBL = _FVR_GetGBLStruct(prTimeInfo->u1TsIndex);
                if (prGBL->pfnNotifyFunc != NULL)
                {
                    prGBL->pfnNotifyFunc(FVR_GBL_NFY_TIME, (UINT32)prTimeInfo, prGBL->pvNotifyTag);
                }
                
                LOG(3, "Receive FVR_MSG_TIMERPKT - 0x%08x\n", prGBL->pfnNotifyFunc);
            }
            break;

        default:
            // Unknown message type
            ASSERT(FALSE);
            break;
        }
    }
}
 
//-----------------------------------------------------------------------------
/** _FVR_Init
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Init(void)
{
    UINT8 i;
    static BOOL _fgInit = FALSE;

    if (!_FVR_InitIsr())
    {
        return FALSE;
    }

    // Soft init
    if (!_FVR_SoftInit())
    {
        return FALSE;
    }

    if (!_DMX_GetIsPanicReset())
    {
        if (!_FVR_SwapInit())
        {
            return FALSE;
        }
    }

    _FVR_Lock();
    for (i=0; i<FVR_GBL_COUNT; i++)
    {
        _afgFVRIsRecording[i] = FALSE;
    }
    _FVR_Unlock();

    if (!_fgInit)
    {
        // Create purge queues
        if (x_msg_q_create(&_hFvrQueue, DMX_FVR_QUEUE_NAME,
            sizeof (FVR_MSG_T), DMX_FVR_QUEUE_SIZE) != OSR_OK)
        {
            LOG(1, "Fail to create fvr queue!\n");
            return FALSE;
        }

        // Create picture thread
        if (x_thread_create(&_hFvrThread, DMX_FVR_THREAD_NAME,
            DMX_FVR_THREAD_STACK_SIZE, DMX_FVR_THREAD_PRIORITY, _DmxFvrThread, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create fvr thread!\n");
            return FALSE;
        }
        
        _fgInit = TRUE;
    }

    _FVR_SitInit();

    // Init PCR index
    _FVR_Lock();
    for (i = 0; i < FVR_SINGLE_STREAM_GBL_COUNT; i++)
    {
        FVR_GBL_ARY_W(0, 8) |= (0xFFu << 8);
        FVR_GBL_ARY_W(0, 8) &= ~(0xFFu << 16);
    }
    _FVR_Unlock();
        
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Start
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Start(UINT8 u1Idx)
{
    UINT32 u4Ctrl;

    if (u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    if (_FVR_IsRecording(u1Idx))
    {
        return TRUE;
    }

    _FVR_SetIsToggleKey(u1Idx);

    // Enable DMEM global control
    FVR_GBL_ARY_W(u1Idx, 2) = 0x0;

    if (!_afgFVRIsRecording[0] && !_afgFVRIsRecording[1] && !_afgFVRIsRecording[2] 
		&& !_afgFVRIsRecording[3]&&!_afgFVRIsRecording[4])
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFDFFFEFF;
        u4Ctrl |= (1 << 8);     // Direct map
        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

        if (!_SetFVRTimer(u1Idx))
        {
            return FALSE;
        }
    }

    _afgFVRIsRecording[u1Idx] = TRUE;

    LOG(3, "FVR start, %d\n", u1Idx);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Stop
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Stop(UINT8 u1Idx)
{
    UINT32 u4Ctrl;

    LOG(5, "%s\n", __FUNCTION__);

    if (u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    // Disable DMEM global control
    _FVR_Lock();
    FVR_GBL_ARY_W(u1Idx, 2) = 0x100;    // Disable record region 0
    _FVR_Unlock();

    _afgFVRIsRecording[u1Idx] = FALSE;

    if (!_afgFVRIsRecording[0] && !_afgFVRIsRecording[1] && !_afgFVRIsRecording[2] 
		&& !_afgFVRIsRecording[3]&& !_afgFVRIsRecording[4])
    {
        _FVR_Lock();
        FVR_GBL_TIMEINFO = 0x0;                     // Disable timer interrupt
        u4Ctrl = FVR_GBL_TIMEINFO_UPDATE& 0xFFFFFF00;
        u4Ctrl |= 0x1;
        FVR_GBL_TIMEINFO_UPDATE = u4Ctrl;

        u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFDFFFFFF;
        u4Ctrl |= (1 << 25);
        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);
        _FVR_Unlock();
    }

    // Avoid race condition, but may call in HISR
    //x_thread_delay(5);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_IsRecording
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_IsRecording(UINT8 u1Idx)
{
    BOOL fgIsRecord;

    if (u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    _FVR_Lock();
    fgIsRecord = _afgFVRIsRecording[u1Idx];
    _FVR_Unlock();

    return fgIsRecord;
}


//-----------------------------------------------------------------------------
/** _FVR_GetResidual
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetResidual(UINT8 u1Idx, UINT32 *pu4Addr, UINT32 *pu4Size, UINT32 *pu4Index)
{
    FVR_GBL_STRUCT_T *prGBL;
    UINT32 u4Wp, u4PktIndex, u4PktRp, u4BufSize, u4DataLen;

    if ((pu4Addr == NULL) || (pu4Size == NULL) || (pu4Index == NULL))
    {
        return FALSE;
    }

    if (u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    if (_afgFVRIsRecording[u1Idx])
    {
        LOG(1, "Still recording\n");
        return FALSE;
    }

    prGBL = _FVR_GetGBLStruct(u1Idx);

    _FVR_Lock();
    u4Wp = VIRTUAL(FVR_GBL_ARY_W(u1Idx, 3));              // write pointer
    u4PktIndex = FVR_GBL_ARY_W(u1Idx, 6);
    u4PktRp = VIRTUAL(prGBL->u4PktRp);
    u4BufSize = prGBL->u4BufSize;
    _FVR_Unlock();

    u4DataLen = DATASIZE(u4PktRp, u4Wp, u4BufSize);

    *pu4Addr = VIRTUAL(u4Wp);
    *pu4Size = u4DataLen;
    *pu4Index = u4PktIndex;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetScrambleScheme
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_SetScrambleScheme(BOOL fgOrg)
{
    UINT32 u4Reg;

    //FVR_GBL_TIMEINFO_UPDATE
    //byte 2 Scrambling detection
    _FVR_Lock();
    u4Reg = FVR_GBL_TIMEINFO_UPDATE & 0xFFFF00FF;
    if (fgOrg)
    {
        u4Reg |= 0x0100;
        FVR_GBL_TIMEINFO_UPDATE = u4Reg;
    }
    _FVR_Unlock();
}

//-----------------------------------------------------------------------------
/** _FVR_SendTimerPktEvent
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SendTimerPktEvent(FVR_GBL_TIMENOTIFY_INFO_T *prTimeInfo)
{
    FVR_MSG_T rMsg;

    ASSERT(prTimeInfo != NULL);

    rMsg.eMsgType = FVR_MSG_TIMERPKT;
    rMsg.uMsgData.rTimeInfo = *prTimeInfo;
    if (x_msg_q_send(_hFvrQueue, (VOID*)&rMsg, sizeof (FVR_MSG_T),
        DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(2, "Failed to send TIMERPKT due to fvr msgQ full!\n");
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _FVR_SendPictureEvent
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SendPictureEvent(FVR_PID_PICNOTIFY_INFO_T *prPicInfo)
{
    FVR_MSG_T rMsg;

    ASSERT(prPicInfo != NULL);

    rMsg.eMsgType = FVR_MSG_PICSEARCH;
    rMsg.uMsgData.rPicInfo = *prPicInfo;
    if (x_msg_q_send(_hFvrQueue, (VOID*)&rMsg, sizeof (FVR_MSG_T),
        DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(2, "Failed to send PICSEARCH due to fvr msgQ full!\n");
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _FVR_SendSitCompleteEvent
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SendSitCompleteEvent(FVR_SIT_PKTREPLACE_INFO_T *prSitInfo)
{
    FVR_MSG_T rMsg;

    ASSERT(prSitInfo != NULL);

    rMsg.eMsgType = FVR_MSG_SITCOMPLETE;
    rMsg.uMsgData.rSitInfo = *prSitInfo;
    if (x_msg_q_send(_hFvrQueue, (VOID*)&rMsg, sizeof (FVR_MSG_T),
        DMX_MSG_PRIORITY_NORMAL) != OSR_OK)
    {
        LOG(2, "Failed to send SITCOMPLETE due to fvr msgQ full!\n");
        return FALSE;
    }

    return TRUE;
}

