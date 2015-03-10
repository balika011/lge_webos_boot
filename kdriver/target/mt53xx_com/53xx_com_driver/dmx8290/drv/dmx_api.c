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
 * $Date: 2015/03/10 $
 * $RCSfile: dmx_api.c,v $
 * $Revision: #6 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_api.c
 *  Demux driver - main implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_mm.h"
#include "dmx_debug.h"
#include "dmx_if.h"
#include "fvr.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "fbm_drvif.h"
#include "mpv_drvif.h"
#include "aud_drvif.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_hal_arm.h"
#include "x_mid.h"
#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#include "nim_if.h"
#endif

LINT_EXT_HEADER_END


#ifdef TIME_MEASUREMENT
LINT_EXT_HEADER_BEGIN
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
LINT_EXT_HEADER_END
BOOL _fgWaitForFirstI = FALSE;  // Measure the time from "Enabling PID"
// to the time "the first I frame is received".
#endif  // TIME_MEASUREMENT

#include "dlmalloc.h"
extern UINT32 _u1FBMmode;

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// Define DEBUG_MEMORY_INTRUSION to protect video FIFO from writing by other
/// agents
//
//#define DEBUG_MEMORY_INTRUSION
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#ifdef CC_DMX_PES_EXT_BUFFER
// 64 KB for the PES Header Extention buffer
#define PES_EXT_BUF_SIZE                    0x10000
#endif  // CC_DMX_PES_EXT_BUFFER

/// Default interrupt threshold of video PID (in unit of 188-byte)
#define DEF_INT_THRESHOLD_VIDEO             0

/// Default interrupt threshold of audio PID (in unit of 188-byte)
#define DEF_INT_THRESHOLD_AUDIO             3

/// Test lock times
#define TS_LOCK_TEST_TIMES                  3

/// Maximum waiting loop of TS locking
#define TS_LOCK_MAX_LOOP                    3000

/// Minimum packet count of TS locking
#define TS_LOCK_MIN_PACKET                  5

#ifdef CC_DMX_TRACK_ERROR_INTS
#define STATUS_REG_BUF_SIZE                 10
#define FTUP_PC_REG_BUF_SIZE                100
#define ERR_RECORD_BUF_SIZE                 60
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#ifdef CC_DMX_TRACK_ERROR_INTS
typedef struct {
    UINT8 u1TsIndex;
    UINT16 u2Pid;
    DMX_PID_TYPE_T ePidType;
    UINT32 u4ErrorInt;
    UINT32 u4ErrorStatus;
    UINT32 u4ErrorCount;
    UINT32 u4TimeStamp;
} DMX_ERROR_RECORD_T;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// The PID structures
static PID_STRUCT_T _arPidStruct[DMX_NUM_PID_INDEX];

/// The filter structures
static FILTER_STRUCT_T _arFilterStruct[DMX_NUM_FILTER_INDEX];

/// DMX certain states
static DMX_STATES_T _rDmxStates;

#ifdef CC_EIT_SECTION_FILTER
//for sw filter section 
static CHANNEL_EIT_S_SECTION_T _arEITFilterStruct[CHANNEL_MAX_NUM];
// how many CHANNEL_EIT_S_SECTION_T is used
BOOL fg_enable_eit_sw_filter = TRUE;
BOOL fg_eit_filter_auto_insert = FALSE;
#endif

#ifdef CC_DMX_EMULATION
static BOOL _fgToDecoder = FALSE;
#endif


#ifdef CC_DEFINE_DMX_LOCK
CRIT_STATE_T _rGDmxLock;
#else /* CC_DEFINE_DMX_LOCK */
/// State word for locking interrupt
static CRIT_STATE_T _rDmxLock;
#endif /* CC_DEFINE_DMX_LOCK */

/// Is interrupt locking? For making sure Lock()/Unlock() are not nested
static BOOL _fgDmxLocking = FALSE;

/// Interrupt threshold of video PID (in unit of 188-byte)
static UINT16 _u2IntThresholdVideo = DEF_INT_THRESHOLD_VIDEO;

/// Interrupt threshold of audio PID (in unit of 188-byte)
static UINT16 _u2IntThresholdAudio = DEF_INT_THRESHOLD_AUDIO;

#ifdef DEBUG_MEMORY_INTRUSION

/// Region ID of protected buffer
static UINT32 _u4ProtectedRegion = MID_MAX_REGIONS;

#endif  // DEBUG_MEMORY_INTRUSION

#ifdef DMX_CHECK_SECTION_CRC
static BOOL _afgFilterCRC[32][50];
static UINT32 _au4FilterCRCIdx[32];
#endif  // DMX_CHECK_SECTION_CRC

static FILTER_TYPE_T _eFilterType = FILTER_TYPE_LEGACY;

static DMX_AUDIO_TYPE_T _aeDmxAideoType[DMX_NUM_PID_INDEX];
static DMX_TRACE_LOGFILE_T _arDmxTraces[DMX_NUM_TRACES];

#ifndef NDEBUG
UINT32 u4LogLvlMask = 0;
UINT8 u1PidxMask = 0xff;
UINT8 u1SecIdxMask = 0xff;
#endif
static UINT32 _u4PsiMaxBufSize = 0;
static UINT32 _u4PesMaxBufSize = 0;

#ifdef CC_DMX_TRACK_ERROR_INTS
static UINT32 _arDmxRegFtupFull[STATUS_REG_BUF_SIZE];
static UINT32 _arDmxRegFtupPktFifo[STATUS_REG_BUF_SIZE];
static UINT32 _arDmxRegDbmStatus3[STATUS_REG_BUF_SIZE];
static UINT32 _arDmxRegFtupPC[FTUP_PC_REG_BUF_SIZE];

static UINT32 _u4DmxErrTime = 0;
static UINT32 _u4DmxErrIdx = 0;
static UINT32 _u4DmxLastErrIdx = ERR_RECORD_BUF_SIZE;
static DMX_ERROR_RECORD_T _rDmxErrorRecords[ERR_RECORD_BUF_SIZE];
#endif

//teletex not sync with audio & video in some pvr playback, add to notify 
//audio pts to mw, the carrier is pes int nofify func
DMX_NOTIFY_INFO_PTS_T _arDMXNotifyInfoPts = {FALSE,0,0,0};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef DEBUG_MEMORY_INTRUSION

//-----------------------------------------------------------------------------
/** _DmxIntrudeHandler
 *  Memory intrusion handler
 *
 *  @param  u4Region        The intruded memory region
 *  @param  eAgentId        The agent who intrudes the protected memory
 *  @param  u4Addr          The intruded memory address
 *
 *  @retval -
 */
//-----------------------------------------------------------------------------
static void _DmxIntrudeHandler(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
                               UINT32 u4Addr)
{
    // Flush log data
    UTIL_LogFlush();

    // Show intrusion info
    LOG(0,"MEMORY VIOLATION - agent: %s, address: 0x%08x\n",
        MID_AgentIdToString(eAgentId), u4Addr);
    LOG(0,"System halted\n");

    // Reset MID
    VERIFY(MID_Reset());

    // Halt system
    BSP_HaltSystem();

    UNUSED(u4Region);
}


//-----------------------------------------------------------------------------
/** _DmxInitMemoryProtection
 *  Init memory protection
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitMemoryProtection(void)
{
    PFN_MID_CALLBACK pfnOrgHandler;

    // Init MID
    if (!MID_Init())
    {
        return FALSE;
    }

    // Register handler
    pfnOrgHandler = MID_RegisterHandler(_DmxIntrudeHandler);
    UNUSED(pfnOrgHandler);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxIsProtectedMemory
 *  If the buffer of a given PID should be protected or not
 *
 *  @param  prPidStruct     PID structure
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
static BOOL _DmxIsProtectedMemory(const PID_STRUCT_T* prPidStruct)
{
    BOOL fgRet;

    ASSERT(prPidStruct != NULL);

    // Currently, only video ES buffer is protected
    fgRet = (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxSetProtectedMemory
 *  Setup a memory region to be protected
 *
 *  @param  u4BufStart      Start address of the protected memory
 *  @param  u4BufEnd        End address of the protected memory
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSetProtectedMemory(UINT32 u4BufStart, UINT32 u4BufEnd)
{
    ASSERT(_DMX_IsAligned(u4BufStart, MID_ADDR_ALIGNMENT));
    ASSERT(_DMX_IsAligned(u4BufEnd, MID_ADDR_ALIGNMENT));

    // Allocate a read-only region
    if (!MID_AllocateFreeRegion(FALSE, &_u4ProtectedRegion))
    {
        return FALSE;
    }

    // Set RO section to read-only
    if (!MID_SetRegion(_u4ProtectedRegion, MID_AGENT_DEMUX, u4BufStart,
                       u4BufEnd))
    {
        return FALSE;
    }

    // Enable the region detection
    if (!MID_EnableRegionProtect(_u4ProtectedRegion))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxResetProtectedMemory
 *  Reset memory protection
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxResetProtectedMemory(void)
{
    if (_u4ProtectedRegion < MID_MAX_REGIONS)
    {
        VERIFY(MID_ResetRegion(_u4ProtectedRegion));
        _u4ProtectedRegion = MID_MAX_REGIONS;
    }

    return TRUE;
}
#endif  // DEBUG_MEMORY_INTRUSION


BOOL _DMX_LogOperation(UINT8 u1Flag, UINT32 u4SetInfo, UINT32 *pu4GetInfo)
{
#ifndef NDEBUG
    if ((u1Flag == DMX_LOG_OP_GETMASK) || (u1Flag == DMX_LOG_OP_GETPIDX) ||
            (u1Flag == DMX_LOG_OP_GETSEC))
    {
        if (pu4GetInfo == NULL)
        {
            LOG(3, "pu4GetInfo is NULL!\n");
            return FALSE;
        }

        switch (u1Flag)
        {
        case DMX_LOG_OP_GETMASK:
            *pu4GetInfo = u4LogLvlMask;
            break;
        case DMX_LOG_OP_GETPIDX:
            *pu4GetInfo = (UINT32)u1PidxMask;
            break;
        case DMX_LOG_OP_GETSEC:
            *pu4GetInfo = (UINT32)u1SecIdxMask;
            break;
        }
    }
    else if (u1Flag == DMX_LOG_OP_SETMASK)
    {
        if ((u4SetInfo & 0xFFFF) != 0)
        {
            // This is not a valid mask
            LOG(3, "This is not a valid mask!\n");
            return FALSE;
        }

        u4LogLvlMask = u4SetInfo;
    }
    else if (u1Flag == DMX_LOG_OP_SETPIDX)
    {
        UINT8 u1Pidx;
        u1Pidx = (UINT8)u4SetInfo;

        if ((u1Pidx >= DMX_NUM_PID_INDEX) && (u1Pidx != 0xFF))
        {
            LOG(3, "This is not a valid Pidx!\n");
            return FALSE;
        }

        u1PidxMask = u1Pidx;
    }
    else if (u1Flag == DMX_LOG_OP_SETSEC)
    {
        UINT8 u1SecIdx;
        u1SecIdx = (UINT8)u4SetInfo;

        if ((u1SecIdx >= DMX_NUM_FILTER_INDEX) && (u1SecIdx != 0xFF))
        {
            return FALSE;
        }

        u1SecIdxMask = u1SecIdx;
    }
    else
    {
        LOG(3, "Wrong DMX_LOG_OP_FLAG.\n");
        return FALSE;
    }
#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxAllocateVideoChannel
 *  Allocate a video channel for a given pid index
 *
 *  @param  u1Pidx          PID index
 *  @param  pu1Channel [output]
 *                          The allocated video channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxAllocateVideoChannel(UINT8 u1Pidx, UINT8 u1ChannelId)
{
    UINT8 i;
    BOOL fgFoundOld = FALSE;
    if (u1ChannelId >= MPV_MAX_ES)
    {
        LOG(2, "%s, channel id is over MPV_MAX_ES.\n", __FUNCTION__);
        return FALSE;
    }

    _DMX_Lock();
    if (_rDmxStates.au1ChannelId[u1Pidx] != DMX_NULL_CHANNEL_ID)
    {
        _DMX_Unlock();
        LOG(2, "%s, channel used. ch:%d, pid:0x%x\n", __FUNCTION__, u1ChannelId, u1Pidx);
        return FALSE;
    }

    _rDmxStates.au1ChannelId[u1Pidx] = u1ChannelId;
    _rDmxStates.afgNewPidx[u1Pidx] = TRUE;

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if ((i != u1Pidx) && (_rDmxStates.au1ChannelId[i] == u1ChannelId))
        {
            _rDmxStates.afgNewPidx[i] = FALSE;
            _rDmxStates.afgUseNew[u1ChannelId] = FALSE;
            fgFoundOld = TRUE;
        }
    }

    if (!fgFoundOld)
    {
        _rDmxStates.afgUseNew[u1ChannelId] = TRUE;
    }

    _DMX_Unlock();

    return TRUE;
}


#ifdef CC_FBM_SUPPORT_DMXPID
void* _Dmx_AllocPsiBuffer(UINT32 u4Size)
{
    void *pBuf;

    pBuf = dlmalloc(u4Size);
    LOG(7, "%s: 0x%x, line=%d\n", __FUNCTION__, (UINT32)pBuf, __LINE__);

    pBuf = (void *)PHYSICAL((UINT32)pBuf);

    return pBuf;
}

BOOL _Dmx_FreePsiBuffer(void *pBuf)
{
    pBuf = (void *)VIRTUAL((UINT32)pBuf);

    //LOG(0, "%s: 0x%x\n", __FUNCTION__, (UINT32)pBuf);

    if (dlfree(pBuf) != 1)
    {
        return FALSE;
    }

    return TRUE;
}
#endif


//-----------------------------------------------------------------------------
/** _DmxSetPidBuffer
 *  Setup PID buffer(s)
 *
 *  @param  u1Pidx          PID index
 *  @param  prPid           PID structure
 *  @param  prPidStruct     Local PID structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSetPidBuffer(UINT8 u1Pidx, const DMX_PID_T* prPid,
                             PID_STRUCT_T* prPidStruct)
{
    UINT32 u4VirtBufStart, u4PhyBufStart, u4W0 = 0, u4BufSize;

    ASSERT(prPid != NULL);
    ASSERT(prPidStruct != NULL);

    if (prPid->ePidType == DMX_PID_TYPE_NONE)
    {
        // No PID type, can't setup buffer
        return TRUE;
    }

#ifdef CC_DMX_TS130
    prPidStruct->fgTS130 = FALSE;
    /*allocate the instand id for this pid*/
    if (_DMX_MUL_GetTSMode(_DMX_MUL_GetPidxInstId(u1Pidx)) == DMX_TSFMT_130 ||
		_DMX_MUL_GetTSMode(_DMX_MUL_GetPidxInstId(u1Pidx)) == DMX_TSFMT_134)
    {
        prPidStruct->fgTS130 = TRUE;
    }
#endif // CC_DMX_TS130

    // Check if buffer already allocated, to avoid memory leakage
    if ((prPidStruct->u4BufStart != 0) || (prPidStruct->u4HeaderBufAddr != 0))
    {
        LOG(2, "%s, header and ES buffer are allocated.\n", __FUNCTION__);
        return FALSE;
    }

    if (prPid->ePidType == DMX_PID_TYPE_PSI)
    {
        u4W0 = (UINT32)
               ((1 << 0) |         // PSI
                (1 << 1) |          // Section filter on
                (23 << 16));        // Max bytes sending to section filter
    }
    else if (prPid->ePidType == DMX_PID_TYPE_PES)
    {
        // PES to memory
        // Turn on filter, no need to setup header buffer
        u4W0 = (UINT32)(1 << 1);
    }
    else if (prPid->ePidType == DMX_PID_TYPE_AV_PES)
    {
        // AV PES to memory
        // Turn on filter, no need to setup header buffer
        u4W0 = (UINT32)(1 << 1);
        u4W0 |= (UINT32)(0x1 << 24); // identify av pes
    }
    else
    {
        if (prPid->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
#ifdef CC_DMX_PES_EXT_BUFFER
            UINT32 u4ExtPhyBufStart, u4ExtVirtBufStart;

            // Allocate the PES Extention buffer.
            u4ExtPhyBufStart = (UINT32)BSP_AllocAlignedDmaMemory(PES_EXT_BUF_SIZE,
                               DMX_HEADER_FIFO_ALIGNMENT);
            if (u4ExtPhyBufStart == 0)
            {
                LOG(2, "%s:%d: Can't allocate memory!\n", __FILE__, __LINE__);
                return FALSE;
            }
            u4ExtVirtBufStart = VIRTUAL(u4ExtPhyBufStart);
            DMXPIDLOG(DMX_LOG_PID DMX_LOG_AV 9, u1Pidx, "PES Ext buffer is allocated! (pidx: %d)\n", u1Pidx);
            _DMX_Lock();
            prPidStruct->u4ExtBufStart = u4ExtVirtBufStart;
            prPidStruct->u4ExtBufEnd = (u4ExtVirtBufStart + PES_EXT_BUF_SIZE) - 1;  // !!!
            prPidStruct->u4ExtBufRp = u4ExtVirtBufStart;
            prPidStruct->u4ExtBufWp = u4ExtVirtBufStart;
            _DMX_Unlock();
#endif  // CC_DMX_PES_EXT_BUFFER
        }

        // Section filter flags
        // bit 0: 1: PSI, 0: PES
        // bit 1: Section filter on
        // bit 2: Output mode
        // bit 4: PID on/off toggle (combined with bit 8 of PID index entry)
        // bit 5: PID disable
        // bit 6: To CD-FIFO
        // bit 7: Enable picture finder
        u4W0 = (UINT32)((1 << 1) | (1 << 6) |
                        (((prPid->ePidType == DMX_PID_TYPE_ES_VIDEO) ? 1 : 0) << 7));

#ifdef CC_DMX_TS130
        if (prPidStruct->fgTS130)
        {
            u4W0 = (UINT32)((1 << 2) | (1 << 6) |
                            (((prPid->ePidType == DMX_PID_TYPE_ES_VIDEO) ? 1 : 0) << 7));
        }
#endif // CC_DMX_TS130

        if (prPid->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            //set byte2 to 0xff for default picture search by TS_IDX
            u4W0 |= 0x00FF0000;
        }

        if (prPid->ePidType == DMX_PID_TYPE_ES_VIDEOCLIP)
        {
            u4W0 |= (1 << 2);       // Output entire data
            u4W0 |= (1 << 7);       // Enable picture finder
            u4W0 |= 0x00FF0000;
        }

        if (prPid->ePidType == DMX_PID_TYPE_TS_RAW)
        {
            u4W0 |= (1 << 0);       // TS flag
            u4W0 |= (1 << 2);       // Output entire data
        }

        if (prPid->ePidType == DMX_PID_TYPE_PES_AUDIO)
        {
            u4W0 &= ~(1 << 6);       //  Clear "To CD-FIFO" for non-AV PES
            u4W0 &= ~(0xFF000000); // PES type
            u4W0 |= 0x01000000; // PES type
        }

        // Setup header buffer start address
        u4PhyBufStart = (UINT32)BSP_AllocAlignedDmaMemory(PES_HEADER_BUF_SIZE,
                        DMX_HEADER_FIFO_ALIGNMENT);
        if (u4PhyBufStart == 0)
        {
#ifdef CC_DMX_PES_EXT_BUFFER
            VERIFY(BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4ExtBufStart)));
            DMXPIDLOG(DMX_LOG_PID DMX_LOG_AV 9, u1Pidx, "PES Ext buffer is freed! (pidx: %d)\n", u1Pidx);
#endif  // CC_DMX_PES_EXT_BUFFER
            LOG(2, "%s:%d: Can't allocate memory!\n", __FILE__, __LINE__);
            return FALSE;
        }

        _DMX_Lock();

        u4VirtBufStart = VIRTUAL(u4PhyBufStart);
        prPidStruct->u4HeaderBufAddr = u4VirtBufStart;
        PID_S_W(u1Pidx, 10) = u4PhyBufStart;    // FIFO start
        PID_S_W(u1Pidx, 12) = u4PhyBufStart;    // Start
        PID_S_W(u1Pidx, 13) = u4PhyBufStart;    // Write pointer
        PID_S_W(u1Pidx, 14) = u4PhyBufStart;    // Read pointer
        prPidStruct->u4NextPic = 0;

        // Header buffer end address
        PID_S_W(u1Pidx, 11) = (u4PhyBufStart + PES_HEADER_BUF_SIZE) - 1;
        
        _DMX_Unlock();
    }

    _DMX_Lock();
    prPidStruct->u4BufLen = prPid->u4BufSize;
    if (prPid->fgAllocateBuffer && (prPid->ePidType == DMX_PID_TYPE_PSI))
    {
        prPidStruct->u4BufLen = prPid->u4BufSize * DMX_PSI_BUF_TIMES;
    }
    u4BufSize = prPidStruct->u4BufLen;
    _DMX_Unlock();

    // FIFO start address
    if (prPid->fgAllocateBuffer)
    {
#ifdef CC_FBM_SUPPORT_DMXPID
        if (prPid->ePidType != DMX_PID_TYPE_ES_VIDEO && prPid->ePidType != DMX_PID_TYPE_ES_AUDIO)
        {
            prPidStruct->u4BufStartPoint = (UINT32)_Dmx_AllocPsiBuffer(u4BufSize + 64);
            u4PhyBufStart = DMX_Align(prPidStruct->u4BufStartPoint, DMX_ES_FIFO_ALIGNMENT);
            if (u4PhyBufStart==0)
            {
                LOG(0, "%s, line=%u, u4PhyBufStart=0x%x, u4BufSize=%d, DMX_ES_FIFO_ALIGNMENT=%d\n", __FUNCTION__, __LINE__, u4PhyBufStart, u4BufSize, DMX_ES_FIFO_ALIGNMENT);
                LOG(0, "%s, line=%u, prPidStruct->u4BufStartPoint=0x%x\n", __FUNCTION__, __LINE__, prPidStruct->u4BufStartPoint);
            }
        }
        else
#endif
        {
            u4PhyBufStart = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSize,
                            DMX_ES_FIFO_ALIGNMENT);
        }
        if (u4PhyBufStart == 0)
        {
#ifdef CC_DMX_PES_EXT_BUFFER
            VERIFY(BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4ExtBufStart)));
            DMXPIDLOG(DMX_LOG_PID DMX_LOG_AV 9, u1Pidx, "PES Ext buffer is freed! (pidx: %d)\n", u1Pidx);
#endif  // CC_DMX_PES_EXT_BUFFER
            VERIFY(BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4HeaderBufAddr)));

            _DMX_Lock();
            prPidStruct->u4HeaderBufAddr = 0;
            _DMX_Unlock();

            LOG(0, "Can't allocate memory! %s, line=%u\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }
    else
    {
        ASSERT(prPid->u4BufAddr != 0);
        //Printf("%s - Pidx %d sets buffer addr 0x%08lx\n", __FUNCTION__, u1Pidx, prPid->u4BufAddr);
        /* Add PHYSICAL for safety check, remove it in the future */
        //enable IOMMU  use virture address
        if (prPid->fgIommuEnable)
        {
            u4PhyBufStart = prPid->u4BufAddr;
        }
        else
        {
            u4PhyBufStart = PHYSICAL(prPid->u4BufAddr);
        }
    }

    if (prPid->ePidType == DMX_PID_TYPE_PSI )
    {
        _u4PsiMaxBufSize += u4BufSize;   //get psi max memeory size information use
    }
    else if ((prPidStruct->ePidType == DMX_PID_TYPE_PES ) ||(prPidStruct->ePidType == DMX_PID_TYPE_PES_TIME))
    {
        _u4PesMaxBufSize += u4BufSize;  //get pes max memeory size information use
    }

    _DMX_Lock();

    //IOMMU enable
    if (prPid->fgIommuEnable)
    {
        //Enable Payload  buffer IOMMU
        u4W0 |= (UINT32)(1 << 9);
    }

    // Preserve bit 4: PID on/off toggle (combined with bit 8 of PID index)
    PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) & (1 << 4)) | u4W0;

    PID_S_W(u1Pidx, 4) = 0xffffffff;

    u4VirtBufStart = VIRTUAL(u4PhyBufStart);
    prPidStruct->u4BufStart = u4VirtBufStart;
    prPidStruct->u4Wp = u4VirtBufStart;
    prPidStruct->u4Rp = u4VirtBufStart;
    prPidStruct->u4SectionRp = u4VirtBufStart;
    prPidStruct->u4PesRp = u4VirtBufStart;
    prPidStruct->u4BufEnd = (u4VirtBufStart + u4BufSize) - 1;

    //if enable iommu, set virture addr to hw,
    //else set phy addr to HW
    if (prPid->fgIommuEnable)
    {
        PID_S_W(u1Pidx, 5) = u4VirtBufStart;     // FIFO start
        PID_S_W(u1Pidx, 7) = u4VirtBufStart;     // PES_start
        PID_S_W(u1Pidx, 8) = u4VirtBufStart;     // Write pointer
        PID_S_W(u1Pidx, 9) = u4VirtBufStart;     // Read pointer
        PID_S_W(u1Pidx, 6) = (u4VirtBufStart + u4BufSize) - 1; //End Addr
    }
    else
    {
        PID_S_W(u1Pidx, 5) = u4PhyBufStart;     // FIFO start
        PID_S_W(u1Pidx, 7) = u4PhyBufStart;     // PES_start
        PID_S_W(u1Pidx, 8) = u4PhyBufStart;     // Write pointer
        PID_S_W(u1Pidx, 9) = u4PhyBufStart;     // Read pointer
        PID_S_W(u1Pidx, 6) = (u4PhyBufStart + u4BufSize) - 1; //End Addr
    }


    // FIFO size
    //prPidStruct->u4BufLen = prPid->u4BufSize;

    prPidStruct->fgAllocateBuffer = prPid->fgAllocateBuffer;
    prPidStruct->ePidType = prPid->ePidType;
    prPidStruct->fgIommuEnable= prPid->fgIommuEnable;
    prPidStruct->u1ContainAD = prPid->u1ContainAD;
    prPidStruct->u1ChannelId = prPid->u1ChannelId;

    _DMX_Unlock();

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if (!_DmxAllocateVideoChannel(u1Pidx, prPidStruct->u1ChannelId))
        {
            if (prPidStruct->u1ChannelId >= MPV_MAX_ES)
            {
                LOG(2, "%s: failed to allocate video channel.\n", __FUNCTION__);
#ifndef CC_DMX_EMULATION
                return FALSE;
#endif
            }
            else
            {
                /* re-use existing u1Pidx and disable video fifo */
                _DMX_Lock();
                prPidStruct->fgDisableFifoOutput = TRUE;
                PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) | (1 << 5));
                _DMX_Unlock();
            }
        }
    }

#ifdef CC_DMX_FLUSH_ALL
    HalFlushInvalidateDCache();
#else 
    /* We need to flush cache to avoid CPU overwriting the buffers 
     * handled by ftup when cache replacement happens
     */
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO || prPidStruct->ePidType == DMX_PID_TYPE_PES)
    {/***L5L***/
        //The size of these buffers is large enough. 
        //Using HalFlushInvalidateDCacheMultipleLine can't improve the perfermence
        HalFlushInvalidateDCache();
    }
    else
    {
        UINT32 u4BufAddr, u4BufLen;
        UINT32 u4HeaderBufAddr;

        _DMX_Lock();
        u4HeaderBufAddr = prPidStruct->u4HeaderBufAddr;
        u4BufAddr = prPidStruct->u4BufStart;
        u4BufLen = prPidStruct->u4BufLen;
        _DMX_Unlock();

        if (u4HeaderBufAddr)
        {
            HalFlushInvalidateDCacheMultipleLine(u4HeaderBufAddr, PES_HEADER_BUF_SIZE);
        }

        HalFlushInvalidateDCacheMultipleLine(u4BufAddr, u4BufLen);
    }
#endif

#ifdef DEBUG_MEMORY_INTRUSION
    if (_DmxIsProtectedMemory(prPidStruct))
    {
        VERIFY(_DmxSetProtectedMemory(u4PhyBufStart, PID_S_W(u1Pidx, 6) + 1));
    }
#endif

    DMXPIDLOG(DMX_LOG_PID 6, u1Pidx, "Set pixd %u buffer: 0x%08x - 0x%08x\n", u1Pidx,
              prPidStruct->u4BufStart, prPidStruct->u4BufEnd);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxFreePidBuffer
 *  Free PID buffer(s)
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxFreePidBuffer(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    prPidStruct = &_arPidStruct[u1Pidx];

    DMXPIDLOG(DMX_LOG_PID 6, u1Pidx, "Free pidx %u buffer\n", u1Pidx);

#ifdef DEBUG_MEMORY_INTRUSION

    if (_DmxIsProtectedMemory(prPidStruct))
    {
        VERIFY(_DmxResetProtectedMemory());
    }

#endif  // DEBUG_MEMORY_INTRUSION
    // Make sure the final DMA transaction terminates before freeing PID buffer.
    x_thread_delay(1);

    // Free buffer
    if (prPidStruct->fgAllocateBuffer)
    {
#ifdef CC_FBM_SUPPORT_DMXPID
        if (prPidStruct->ePidType != DMX_PID_TYPE_ES_VIDEO && prPidStruct->ePidType != DMX_PID_TYPE_ES_AUDIO)
        {
            if(!_Dmx_FreePsiBuffer((void *)prPidStruct->u4BufStartPoint))
            {
                LOG(0, "Fail to free PID buffer! pidx: %u, addr: 0x%08x\n",
                    u1Pidx, prPidStruct->u4BufStart);
            }
        }
        else
#endif
        {
            if (!BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4BufStart)))
            {
                LOG(3, "Fail to free PID buffer! pidx: %u, addr: 0x%08x\n",
                    u1Pidx, prPidStruct->u4BufStart);
                return FALSE;
            }
        }
    }

    if (prPidStruct->ePidType == DMX_PID_TYPE_PSI )
    {
        if (_u4PsiMaxBufSize >= prPidStruct->u4BufLen)
        {
            _u4PsiMaxBufSize -= prPidStruct->u4BufLen;
        }
        else
        {
            _u4PsiMaxBufSize = 0;
        }

    }
    else if ((prPidStruct->ePidType == DMX_PID_TYPE_PES )||(prPidStruct->ePidType == DMX_PID_TYPE_PES_TIME))
    {
        if (_u4PesMaxBufSize >= prPidStruct->u4BufLen)
        {
            _u4PesMaxBufSize -= prPidStruct->u4BufLen;
        }
        else
        {
            _u4PesMaxBufSize = 0;
        }

    }

    // Free header buffer
    if (!BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4HeaderBufAddr)))
    {
        LOG(3, "Fail to free PID header buffer! pidx: %u, addr: 0x%08x\n",
            u1Pidx, prPidStruct->u4HeaderBufAddr);
        return FALSE;
    }

#ifdef CC_DMX_PES_EXT_BUFFER
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if (!BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4ExtBufStart)))
        {
            LOG(3, "Failed to free PES ext buffer! pidx: %u, addr: 0x%08x\n",
                u1Pidx, prPidStruct->u4ExtBufStart);
            return FALSE;
        }
        DMXPIDLOG(DMX_LOG_PID DMX_LOG_AV 9, u1Pidx, "PES Ext buffer is freed! (pidx: %d)\n", u1Pidx);
    }
#endif  // CC_DMX_PES_EXT_BUFFER

    _DMX_Lock();

    // Update PID state
    prPidStruct->fgAllocateBuffer = FALSE;
    prPidStruct->u4BufStart = 0;
    prPidStruct->u4BufEnd = 0;
    prPidStruct->u4BufLen = 0;
    prPidStruct->u4Rp = 0;
    prPidStruct->u4Wp = 0;
    prPidStruct->u4HeaderBufAddr = 0;
#ifdef CC_DMX_MULTIPLE_SECTION
    //prPidStruct->u4SecMask = 0;
    x_memset((VOID*)prPidStruct->au4SecMask , 0 ,SEC_CNT * sizeof(UINT32));
#endif // CC_DMX_MULTIPLE_SECTION

    PID_S_W(u1Pidx, 10) = 0x0;
    PID_S_W(u1Pidx, 11) = 0x0;

#ifdef CC_DMX_PES_EXT_BUFFER
    prPidStruct->u4ExtBufStart = 0;
    prPidStruct->u4ExtBufEnd = 0;
    prPidStruct->u4ExtBufRp = 0;
    prPidStruct->u4ExtBufWp = 0;
#endif  // CC_DMX_PES_EXT_BUFFER

    _DMX_Unlock();

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        // Reset pidx-to-channel mapping
        _DMX_FreeVideoChannel(u1Pidx);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxEnablePid
 *  Enable or disable a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  fgEnable        Enable or disable
 *  @param  pu4PidEntry     Pointer to current value of PID index entry
 *  @param  pu4PidWord0     Pointer to the word 0 of PID memory
 *  @param  prPidStruct     Internal PID data structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxEnablePid(UINT8 u1Pidx, BOOL fgEnable, UINT32* pu4PidEntry,
                          UINT32* pu4PidWord0, PID_STRUCT_T* prPidStruct, BOOL fgKeepScramble)
{
    UINT32 u4ActiveFlag, u4PidIndexTable;
    BOOL fgRet = TRUE;
    UINT8 i;

    ASSERT(prPidStruct != NULL);
    ASSERT(pu4PidEntry != NULL);
    ASSERT(pu4PidWord0 != NULL);

    DMXPIDLOG(DMX_LOG_PID 6, u1Pidx, "%s pidx %u, type: %d\n", fgEnable ? "Enable" : "Disable", u1Pidx, prPidStruct->ePidType);

    if (fgEnable)
    {
#ifdef TIME_MEASUREMENT
        if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "DMX_EnablePid");
			TMS_DIFF_EX(TMS_FLAG_BOOT, TMS_COOL_BOOT_TIME, "DMX_EnablePid");
            _fgWaitForFirstI = TRUE;
        }
#endif  // TIME_MEASUREMENT

#ifdef CHANNEL_CHANGE_LOG
        HAL_TIME_T dt;
        if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            SIGNAL rSignal;
            NIM_GetCableSignal(&rSignal);
            HAL_GetTime(&dt);
            LOG(0, " %u.%06u s [AV SYNC] 2 1 DMX Enable V-PID ( 0x%x ), Freq %u\n", dt.u4Seconds, dt.u4Micros, prPidStruct->u2Pid, (UINT32)rSignal.Frequency);
        }
        else if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
        {
            HAL_GetTime(&dt);
            LOG(0, " %u.%06u s [AV SYNC] 2 2 DMX Enable A-PID ( 0x%x )\n", dt.u4Seconds, dt.u4Micros, prPidStruct->u2Pid);
        }
#endif  // CHANNEL_CHANGE_LOG

        //In some pvr play, need alloc audio pts for mw
        if ((prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO) &&
            (prPidStruct->eInputType == DMX_IN_PLAYBACK_TS))
        {
            _arDMXNotifyInfoPts.fgGetPtsEn = TRUE;
            _arDMXNotifyInfoPts.u4Pts = 0;

        }

        if ((prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO) &&
            (prPidStruct->eInputType == DMX_IN_BROADCAST_TS))
        {
            HAL_TIME_T rCurrTime;

            HAL_GetTime(&rCurrTime);

            _DMX_Lock();
            prPidStruct->u4FrameCntPs = 0;
            prPidStruct->rPrevFrameTime = rCurrTime;
            _DMX_Unlock();
        }

        if (prPidStruct->ePidType == DMX_PID_TYPE_TS_RAW)
        {
            _DMX_RawTsEnable(u1Pidx, fgEnable);
        }

        if (!fgKeepScramble && (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO))
        {
            VERIFY(DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_VIDEO, 0));
        }

        if (!_DMX_ResetPidBuffer(u1Pidx, prPidStruct, fgKeepScramble))
        {
            return FALSE;
        }

        _DMX_Lock();

        // sync audio ES and audio PES value
        if ((prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO) || (prPidStruct->ePidType == DMX_PID_TYPE_PES_AUDIO))
        {
            // check if the audio PES PID is enabled
            for (i = 0; i < DMX_NUM_PID_INDEX; i++)
            {
                u4PidIndexTable = PID_INDEX_TABLE(i);
                if ((i != u1Pidx) && (u4PidIndexTable & 0xFFFF0000U) == (((UINT32)1U << 31) | (*pu4PidEntry & 0x7FFF0000)))
                {
                    if ((_arPidStruct[i].ePidType == DMX_PID_TYPE_ES_AUDIO) ||
                            (_arPidStruct[i].ePidType == DMX_PID_TYPE_PES_AUDIO))
                    {
                        // already set PES PID, Read bit 8 of PID index table from PES PID
                        u4ActiveFlag = (UINT32)((((u4PidIndexTable) & (1 << 8)) != 0) ? 1 : 0);

                        // set to ES PID DMEM bit 4 of word 0
                        *pu4PidWord0 = ((*pu4PidWord0) & ~(1 << 4)) | (u4ActiveFlag << 4);
                    }
                }
            }
        }

        if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
        {
            // check if the audio PES PID is enabled
            for (i = 0; i < DMX_NUM_PID_INDEX; i++)
            {
                u4PidIndexTable = PID_INDEX_TABLE(i);
                if ((i != u1Pidx) && (u4PidIndexTable & 0xFFFF0000U) == (((UINT32)1U << 31) | (*pu4PidEntry & 0x7FFF0000)))
                {
                    if (_arPidStruct[i].ePidType == DMX_PID_TYPE_PES_AUDIO)
                    {
                        // already set PES PID, set bit[9] =1 and second PID index
                        PID_S_W(u1Pidx, 22) |= (i << 24);
                        *pu4PidWord0 |= (1 << 11);
                    }
                }
            }
        }

		if(prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEOCLIP)
		{/*use this type as mm data,so set 0.*/
		  *pu4PidWord0 = ((*pu4PidWord0) & ~(1<<4));
		  *pu4PidEntry = ((*pu4PidEntry) & ~(1 << 8));
		}
		else
		{

          // Read bit 4 of the first word in PID memory
          u4ActiveFlag = (UINT32)((((*pu4PidWord0) & (1 << 4)) != 0) ? 1 : 0);

          // Copy to bit 8 of PID index table
          *pu4PidEntry = ((*pu4PidEntry) & ~(1 << 8)) | (u4ActiveFlag << 8);
	    }

#ifdef CC_DMX_TS130
        if (prPidStruct->fgTS130)
        {
            *pu4PidEntry &= 0x7FFFFFFF;
        }
        else
#endif  // CC_DMX_TS130
        {
            *pu4PidEntry |= 0x80000000;
        }

        prPidStruct->fgEnable = fgEnable;

        _DMX_Unlock();
    }
    else
    {
        _DMX_Lock();

        // Read bit 8 of PID index table
        u4ActiveFlag = (UINT32)(((*pu4PidEntry & (1 << 8)) == 0) ? 1 : 0);

        // Copy inverse to bit 4 of the first word in PID memory
        *pu4PidWord0 = (UINT32)(((*pu4PidWord0) & ~(1 << 4)) | (u4ActiveFlag << 4));

        *pu4PidEntry &= 0x7FFFFFFF;
        /*
                // Disable PID index table first
                PID_INDEX_TABLE(u1Pidx) &= 0x7fffffff;
        */
        prPidStruct->fgEnable = fgEnable;

        if ((prPidStruct->fgFifoFull) && (prPidStruct->ePidType == DMX_PID_TYPE_PSI))
        {
            DMXPIDLOG(DMX_LOG_PID DMX_LOG_PSI 6, u1Pidx, "No need update serial number(Fifo full and PSI).\n");
        }
        else
        {
            // Update serial number
            prPidStruct->u1SerialNumber++;
            if (prPidStruct->u1SerialNumber > DMX_MAX_SERIAL_NUM)
            {
                prPidStruct->u1SerialNumber = 0;
            }
        }

        if (prPidStruct->ePidType == DMX_PID_TYPE_TS_RAW)
        {
            _DMX_RawTsEnable(u1Pidx, fgEnable);
        }

        // special handling for free PES PID
        if (prPidStruct->ePidType == DMX_PID_TYPE_PES_AUDIO)
        {
            // check if the audio ES PID is enabled
            for (i = 0; i < DMX_NUM_PID_INDEX; i++)
            {
                u4PidIndexTable = PID_INDEX_TABLE(i);
                if ((i != u1Pidx) && (u4PidIndexTable & 0xFFFF0000U) == (((UINT32)1U << 31) | (*pu4PidEntry & 0x7FFF0000)))
                {
                    if (_arPidStruct[i].ePidType == DMX_PID_TYPE_ES_AUDIO)
                    {
                        // already ES PID, set bit[11] =1 and second PID index
                        PID_S_W(i, 0) &= ~((UINT32)1 << 11);
                        PID_S_W(i, 22) &= ~((UINT32)0xFFU << 24);
                    }
                }
            }
        }

        _DMX_Unlock();
    }

    // Notify handler
    _DMX_NotifyEnablePid(u1Pidx, fgEnable);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxEnableAudioPesPid
 *  Enable or disable a audio PES PID
 *
 *  @param  u1Pidx          PID index
 *  @param  fgEnable        Enable or disable
 *  @param  pu4PidEntry     Pointer to current value of PID index entry
 *  @param  prPidStruct     Internal PID data structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxEnableAudioPesPid(UINT8 u1Pidx, BOOL fgEnable, UINT32* pu4PidEntry,
                                  PID_STRUCT_T* prPidStruct)
{
    UINT8 i;
    UINT32 u4PidIndexTable;

    if (fgEnable)
    {
        if (prPidStruct->ePidType == DMX_PID_TYPE_PES_AUDIO)
        {
            // check if the audio ES PID is enabled
            for (i = 0; i < DMX_NUM_PID_INDEX; i++)
            {
                u4PidIndexTable = PID_INDEX_TABLE(i);
                if ((i != u1Pidx) && (u4PidIndexTable & 0xFFFF0000U) == (((UINT32)1U << 31) | (*pu4PidEntry & 0x7FFF0000)))
                {
                    if (_arPidStruct[i].ePidType == DMX_PID_TYPE_ES_AUDIO)
                    {
                        // already set ES PID, set bit[9] =1 and second PID index
                        PID_S_W(i, 22) |= (u1Pidx << 24);
                        PID_S_W(i, 0) |= (1 << 11);
                    }
                }
            }
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxIsPidEnabled
 *  Is a PID enabled or disabled
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Disabled
 */
//-----------------------------------------------------------------------------
static BOOL _DmxIsPidEnabled(UINT8 u1Pidx)
{
    BOOL fgActive1, fgActive2;

#ifdef ENABLE_MULTIMEDIA
    PID_STRUCT_T* prPidStruct;
    DMX_INPUT_TYPE_T eInputType;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    eInputType = _DMX_GetPidInputType(u1Pidx);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    if ((eInputType == DMX_IN_PLAYBACK_PS) || (eInputType == DMX_IN_PLAYBACK_MM) ||
            (eInputType == DMX_IN_PLAYBACK_ES))
    {
        return prPidStruct->fgEnable;
    }

#ifdef CC_DMX_TS130
    if (prPidStruct->fgTS130)
    {
        return prPidStruct->fgEnable;
    }
#endif // CC_DMX_TS130

#endif  // ENABLE_MULTIMEDIA

    {
        _DMX_Lock();

        // Bit 4 (PID on/off toggle) of the first word in PID memory
        fgActive1 = ((PID_S_W(u1Pidx, 0) & (1 << 4)) != 0);

        // Bit 8 of PID index table
        fgActive2 = ((PID_INDEX_TABLE(u1Pidx) & (1 << 8)) != 0);

        _DMX_Unlock();
    }

    return ((fgActive1 && fgActive2) || (!fgActive1 && !fgActive2));
}


//-----------------------------------------------------------------------------
/** _DmxDTVEnableCheck
 */
//-----------------------------------------------------------------------------
static void _DmxDTVEnableCheck(void)
{
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
#ifndef CC_DEFINE_DMX_LOCK
//-----------------------------------------------------------------------------
/** _DMX_Lock
 *  Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_Lock(void)
{
    _rDmxLock = x_crit_start();

    ASSERT(!_fgDmxLocking);
    _fgDmxLocking = TRUE;

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _DMX_Unlock
 *  Leave demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_Unlock(void)
{
    ASSERT(_fgDmxLocking);
    _fgDmxLocking = FALSE;

    x_crit_end(_rDmxLock);

    UNUSED(_fgDmxLocking);          // Make Lint happy
}

#endif /* CC_DEFINE_DMX_LOCK */


#ifdef CC_FBM_SUPPORT_DMXPID
BOOL _DMX_PSIBufferInit(void)
{
    FBM_POOL_T* prFbmPool;
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMXPID);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);

    if (dlmem_init((void *)VIRTUAL(prFbmPool->u4Addr), prFbmPool->u4Size) != 1)
    {
        LOG(0, "%s failed", __FUNCTION__);
        return FALSE;
    }

    return TRUE;
}


void _DMX_PSIBufferStatus(void)
{
    mempool_status status;
    dlmem_get_status(MEMPOOL_STAT_ALL, &status);
}
#endif

//-----------------------------------------------------------------------------
/** _DMX_AllocateVideoChannel
 *  Allocate a video channel for a given pid index
 *
 *  @param  u1Pidx          PID index
 *  @param  pu1Channel[output]
 *                          The allocated channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_AllocateVideoChannel(UINT8 u1Pidx, UINT8 u1Channel)
{
    return _DmxAllocateVideoChannel(u1Pidx, u1Channel);
}


//-----------------------------------------------------------------------------
/** _DMX_FreeVideoChannel
 *  Release mapping of a given pidx and its corresponding video channel
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_FreeVideoChannel(UINT8 u1Pidx)
{
    _DMX_Lock();

    _rDmxStates.au1ChannelId[u1Pidx] = DMX_NULL_CHANNEL_ID;

    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_StopVideoChannel
 *  Release mapping of a given pidx and its corresponding video channel
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_StopOldVideoChannel(UINT8 u1Pidx)
{
    UINT8 u1ChannelId;

    u1ChannelId = _rDmxStates.au1ChannelId[u1Pidx];
    _rDmxStates.afgUseNew[u1ChannelId] = TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetVideoChannel
 *  Get corresponding video channel of a given pid index
 *
 *  @param  u1Pid           PID index
 *  @param  pu1Channel [output]
 *                          The channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel)
{
    ASSERT(pu1Channel != NULL);

    if (_rDmxStates.au1ChannelId[u1Pidx] != DMX_NULL_CHANNEL_ID)
    {
        *pu1Channel = _rDmxStates.au1ChannelId[u1Pidx];
        return TRUE;
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetVideoPidx
 *  Get corresponding pid index of a given video channel
 *
 *  @param  u1Channel       Channel index
 *  @param  pu1Pidx [output]
 *                          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetVideoPidx(UINT8 u1Channel, UINT8* pu1Pidx)
{
    UINT8 i;

    ASSERT(pu1Pidx != NULL);

    if (u1Channel >= MPV_MAX_ES)
    {
        return FALSE;
    }

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (_rDmxStates.au1ChannelId[i] == u1Channel)
        {
            if ((_rDmxStates.afgNewPidx[i] && _rDmxStates.afgUseNew[u1Channel]) ||
                    (!_rDmxStates.afgNewPidx[i] && !_rDmxStates.afgUseNew[u1Channel]))
            {
                *pu1Pidx = i;
                return TRUE;
            }
        }
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
/** _DMX_VCTSoftFilterEnable
 *  fgEnable - filter enable flags,enable/disable
 *                true means use old method to check duplicate packet
 *  TableId -  pidval 0x1ffb's tableid that be filtered.for 0x1ffb psi 
 *             need Micro-code soft filter(tableid 0xc8 0xc9)
 */
//-----------------------------------------------------------------------------
VOID _DMX_VCTSoftFilterEnable(BOOL fgEnable, UINT8 TableId)
{
#ifndef CC_MT5890

    UINT32 u4Ctrl;
    UINT8 u1bitoffset;

    _DMX_Lock();
  
    u4Ctrl = VCT_SOFTFILTER_SETTING;

    //filter table id Micro-code just support table 0xc8,0xc9
    //if((TableId!=0xc7)&&(TableId!=0xc8)&&(TableId!=0xc9)&&(TableId!=0xcd))
    if((TableId!=0xc8)&&(TableId!=0xc9))
    {
           _DMX_Unlock();
           LOG(5,"[%S],VCT softfilter error,table id not meet.\n",__FUNCTION__);
           return;
    }
    switch(TableId)
    {
        case 0xc8:
            u1bitoffset = 0;
            break;
        case 0xc9:
            u1bitoffset = 1;
            break;

        default:
            break;
    }
    
    if (fgEnable)
    {
        // enable vct softfilter
        u4Ctrl |= (0x1<<(u1bitoffset+8));
    }
    else
    {
        // disable vct softfilter
        u4Ctrl &= (~(0x1<<(u1bitoffset+8)));
    }
    
    VCT_SOFTFILTER_SETTING = u4Ctrl;   
    _DMX_Unlock();
    
#endif
}

//-----------------------------------------------------------------------------
/** _DMX_GetPidStruct
 *  Get the PID structure of a given PID index
 *
 *  @param  u4PidIndex      PID index
 *
 *  @retval The pointer of the PID structure
 */
//-----------------------------------------------------------------------------
PID_STRUCT_T* _DMX_GetPidStruct(UINT32 u4PidIndex)
{
    ASSERT(u4PidIndex < DMX_NUM_PID_INDEX);

    if(u4PidIndex >= DMX_NUM_PID_INDEX)
    {
        u4PidIndex = 0;
    }

    return &_arPidStruct[u4PidIndex];
}


//-----------------------------------------------------------------------------
/** _DMX_SetFilterType
 */
//-----------------------------------------------------------------------------
void _DMX_SetFilterType(FILTER_TYPE_T eType)
{
    _eFilterType = eType;
}


//-----------------------------------------------------------------------------
/** _DMX_GetFilterType
 */
//-----------------------------------------------------------------------------
FILTER_TYPE_T _DMX_GetFilterType(void)
{
    return _eFilterType;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFilter
 *  Setup a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
                    const DMX_FILTER_T* prFilter)
{
    UINT32 u4FilterWord;
    UINT32 au4Pattern[2], au4Mask[2];
    FILTER_STRUCT_T *prFilterStruct = NULL;
#ifdef CC_DMX_MULTIPLE_SECTION
    PID_STRUCT_T *prPidStruct = NULL;
    FVR_PID_STRUCT_T *prFvrStruct = NULL;
    UINT8 u1SecCount = 0, u1SecIdx = 0;
    UINT32 au4SecMask[SEC_CNT] = {0};
#endif // CC_DMX_MULTIPLE_SECTION

    ASSERT(prFilter != NULL);

    _DMX_SetFilterType(FILTER_TYPE_LEGACY);

    // Check parameters
    if ((u1FilterIndex >= DMX_NUM_FILTER_INDEX) ||
            (u1Bank >= DMX_NUM_FILTER_BANK))
    {
        return FALSE;
    }

    if (u4Flags == DMX_FILTER_FLAG_NONE)
    {
        return TRUE;
    }

	if (prFilter->u1Tsidx >= DMX_FRAMER_COUNT)
    {
        LOG(0, "Tsidx is out of Framer counter!\n");
        return FALSE;
    }

    _DMX_Lock();

    u4FilterWord = SECTION_FILTER_CTRL(u1FilterIndex);

    prFilterStruct = &_arFilterStruct[u1FilterIndex];

    // PID index
    if ((u4Flags & DMX_FILTER_FLAG_PIDX) != 0)
    {
        UINT8 u1Pidx;

        //ASSERT(!prFilter->fgIsFvr);

        u1Pidx = prFilter->u1Pidx;
        if (prFilter->fgIsFvr)
        {
            u1Pidx += DMX_FVR_START_PID;
        }
        u4FilterWord &= ~0xFF;
        u4FilterWord |= ((UINT32)(u1Pidx & 0xFF));
        prFilterStruct->arBank[u1Bank].u1Pidx = prFilter->u1Pidx;
        prFilterStruct->fgIsFvr = prFilter->fgIsFvr;
    }

    // Offset
    if ((u4Flags & DMX_FILTER_FLAG_OFFSET) != 0)
    {
        ASSERT(prFilter->u1Offset < 16);

        if (u1Bank == 0)
        {
            u4FilterWord &= ~0xF0000;
            u4FilterWord |= ((prFilter->u1Offset & 0xF) << 16);
        }

        prFilterStruct->arBank[u1Bank].u1Offset = prFilter->u1Offset;
    }

    // Check CRC
    if ((u4Flags & DMX_FILTER_FLAG_CRC) != 0)
    {
        u4FilterWord &= ~0x1000000;
        u4FilterWord |= ((prFilter->fgCheckCrc ? 1 : 0) << 24);

        prFilterStruct->arBank[u1Bank].fgCheckCrc = prFilter->fgCheckCrc;

    }

    // Valid
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        if (u1Bank == 0)
        {
            u4FilterWord &= ~0x80000000;
            u4FilterWord |= ((prFilter->fgEnable ? 1 : 0) << 31);

#ifdef CC_DMX_MULTIPLE_SECTION
            if (!prFilterStruct->fgIsFvr)
            {
                prPidStruct = &_arPidStruct[prFilterStruct->arBank[u1Bank].u1Pidx];
                if (prPidStruct != NULL)
                {
                    u1SecCount = prPidStruct->u1SecCount;
                    u1SecIdx = prPidStruct->u1SecIdx;
                    x_memcpy((VOID*)au4SecMask, (VOID*)prPidStruct->au4SecMask, SEC_CNT * sizeof(UINT32));
                }
            }
            else
            {
                prFvrStruct = _FVR_GetPidStruct(prFilterStruct->arBank[u1Bank].u1Pidx);
                if (prFvrStruct != NULL)
                {
                    u1SecCount = prFvrStruct->u1SecCount;
                    u1SecIdx = prFvrStruct->u1SecIdx;
                    x_memcpy((VOID*)au4SecMask, (VOID*)prFvrStruct->au4SecMask,SEC_CNT * sizeof(UINT32));
                }
            }

            if (prFilter->fgEnable)
            {
                u1SecCount++;

                if (u1SecCount == 1)
                {
                    u1SecIdx = u1FilterIndex;
                }
                au4SecMask[SEC_NUM(u1FilterIndex)] |=(1 << (FILTERINDEX(u1FilterIndex)));
            }
            else
            {
                if (u1SecCount > 0)
                {
                    u1SecCount--;
                }
                au4SecMask[SEC_NUM(u1FilterIndex)] &= ~(1 << (FILTERINDEX(u1FilterIndex)));
            }

            if (!prFilterStruct->fgIsFvr)
            {
                if (prPidStruct != NULL)
                {
                    prPidStruct->u1SecCount = u1SecCount;
                    prPidStruct->u1SecIdx = u1SecIdx;
                    x_memcpy((VOID*)prPidStruct->au4SecMask ,(VOID*)au4SecMask, SEC_CNT * sizeof(UINT32));
                }
            }
            else
            {
                if (prFvrStruct != NULL)
                {
                    prFvrStruct->u1SecCount = u1SecCount;
                    prFvrStruct->u1SecIdx = u1SecIdx;
                    x_memcpy((VOID*)prFvrStruct->au4SecMask ,(VOID*)au4SecMask, SEC_CNT * sizeof(UINT32));
                }
            }
#endif  // CC_DMX_MULTIPLE_SECTION
        }

        prFilterStruct->arBank[u1Bank].fgEnable = prFilter->fgEnable;
    }

    // Mode
    if ((u4Flags & DMX_FILTER_FLAG_MODE) != 0)
    {
        prFilterStruct->eMode = prFilter->eMode;
    }

    // Pattern
    if ((u4Flags & DMX_FILTER_FLAG_PATTERN) != 0)
    {
        // Disable filter first
        SECTION_FILTER_CTRL(u1FilterIndex) &= ~(1U << 31);

        au4Pattern[0] = SECTION_FILTER_PAT(u1FilterIndex, 0);
        au4Pattern[1] = SECTION_FILTER_PAT(u1FilterIndex, 1);

        au4Mask[0] = SECTION_FILTER_MASK(u1FilterIndex, 0);
        au4Mask[1] = SECTION_FILTER_MASK(u1FilterIndex, 1);

        if (prFilterStruct->eMode == DMX_FILTER_MODE_SHORT)
        {
            au4Pattern[0] = prFilter->au4Data[0];
            au4Pattern[1] = prFilter->au4Data[1];

            au4Mask[0] = prFilter->au4Mask[0];
            au4Mask[1] = prFilter->au4Mask[1];

            SECTION_FILTER_POSNEG(u1FilterIndex, 0) = 0x0;
            SECTION_FILTER_POSNEG(u1FilterIndex, 1) = 0x0;
        }
        else if (prFilterStruct->eMode == DMX_FILTER_MODE_LONG)
        {
            // Not support long filter
            ASSERT(0);
        }
        else if (prFilterStruct->eMode == DMX_FILTER_MODE_14_2)
        {
            // Not support 14_2 filter
            ASSERT(0);
        }
        else if (prFilterStruct->eMode == DMX_FILTER_MODE_POS_NEG)
        {
            if (u1Bank == 0)
            {
                au4Pattern[0] = prFilter->au4Data[0];
                au4Pattern[1] = prFilter->au4Data[1];
                au4Mask[0] = prFilter->au4Mask[0];
                au4Mask[1] = prFilter->au4Mask[1];

                SECTION_FILTER_POSNEG(u1FilterIndex, 0) = 0x0;
                SECTION_FILTER_POSNEG(u1FilterIndex, 1) = 0x0;
            }
            else if (u1Bank == 1)
            {
                au4Pattern[0] = (au4Mask[0] & au4Pattern[0]) |
                                (prFilter->au4Mask[0] & prFilter->au4Data[0]);
                au4Pattern[1] = (au4Mask[1] & au4Pattern[1]) |
                                (prFilter->au4Mask[1] & prFilter->au4Data[1]);
                au4Mask[0] = (prFilter->au4Mask[0] | au4Mask[0]);
                au4Mask[1] = (prFilter->au4Mask[1] | au4Mask[1]);

                SECTION_FILTER_POSNEG(u1FilterIndex, 0) = prFilter->au4Mask[0];
                SECTION_FILTER_POSNEG(u1FilterIndex, 1) = prFilter->au4Mask[1];
            }
        }
        else
        {
            _DMX_Unlock();
            return FALSE;
        }

        SECTION_FILTER_PAT(u1FilterIndex, 0) = au4Pattern[0];
        SECTION_FILTER_PAT(u1FilterIndex, 1) = au4Pattern[1];

        SECTION_FILTER_MASK(u1FilterIndex, 0) = au4Mask[0];
        SECTION_FILTER_MASK(u1FilterIndex, 1) = au4Mask[1];

        // Set pattern and mask
        prFilterStruct->arBank[u1Bank].au1Mask[0] = GET_BYTE(prFilter->au4Mask[0], 3);
        prFilterStruct->arBank[u1Bank].au1Mask[1] = GET_BYTE(prFilter->au4Mask[0], 2);
        prFilterStruct->arBank[u1Bank].au1Mask[2] = GET_BYTE(prFilter->au4Mask[0], 1);
        prFilterStruct->arBank[u1Bank].au1Mask[3] = GET_BYTE(prFilter->au4Mask[0], 0);
        prFilterStruct->arBank[u1Bank].au1Mask[4] = GET_BYTE(prFilter->au4Mask[1], 3);
        prFilterStruct->arBank[u1Bank].au1Mask[5] = GET_BYTE(prFilter->au4Mask[1], 2);
        prFilterStruct->arBank[u1Bank].au1Mask[6] = GET_BYTE(prFilter->au4Mask[1], 1);
        prFilterStruct->arBank[u1Bank].au1Mask[7] = GET_BYTE(prFilter->au4Mask[1], 0);

        prFilterStruct->arBank[u1Bank].au1Data[0] = GET_BYTE(prFilter->au4Data[0], 3);
        prFilterStruct->arBank[u1Bank].au1Data[1] = GET_BYTE(prFilter->au4Data[0], 2);
        prFilterStruct->arBank[u1Bank].au1Data[2] = GET_BYTE(prFilter->au4Data[0], 1);
        prFilterStruct->arBank[u1Bank].au1Data[3] = GET_BYTE(prFilter->au4Data[0], 0);
        prFilterStruct->arBank[u1Bank].au1Data[4] = GET_BYTE(prFilter->au4Data[1], 3);
        prFilterStruct->arBank[u1Bank].au1Data[5] = GET_BYTE(prFilter->au4Data[1], 2);
        prFilterStruct->arBank[u1Bank].au1Data[6] = GET_BYTE(prFilter->au4Data[1], 1);
        prFilterStruct->arBank[u1Bank].au1Data[7] = GET_BYTE(prFilter->au4Data[1], 0);

    }

    SECTION_FILTER_CTRL(u1FilterIndex) = u4FilterWord;

    _DMX_Unlock();

    // Debug
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        DMXSECLOG(DMX_LOG_PSI 6, u1FilterIndex, "%s filter %u bank %u, Tsindex : %d.\n", prFilter->fgEnable ? "Enable" : "Disable",
                  u1FilterIndex, u1Bank, prFilter->u1Tsidx);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetFilter
 *  Get a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
                    DMX_FILTER_T* prFilter)
{
    FILTER_STRUCT_T *prFilterStruct = NULL;

    ASSERT(prFilter != NULL);

    // Check parameters
    if ((u1FilterIndex >= DMX_NUM_FILTER_INDEX) ||
            (u1Bank >= DMX_NUM_FILTER_BANK))
    {
        return FALSE;
    }

    if (u4Flags == DMX_FILTER_FLAG_NONE)
    {
        return TRUE;
    }

	if (prFilter->u1Tsidx >= DMX_FRAMER_COUNT)
    {
        LOG(0, "Tsidx is out of Framer counter!\n");
        return FALSE;
    }

    _DMX_Lock();

    prFilterStruct = &_arFilterStruct[u1FilterIndex];

    // PID index
    if ((u4Flags & DMX_FILTER_FLAG_PIDX) != 0)
    {
        prFilter->fgIsFvr = prFilterStruct->fgIsFvr;
        prFilter->u1Pidx = prFilterStruct->arBank[u1Bank].u1Pidx;
    }

    // Offset
    if ((u4Flags & DMX_FILTER_FLAG_OFFSET) != 0)
    {
        prFilter->u1Offset= prFilterStruct->arBank[u1Bank].u1Offset;
    }

    // Check CRC
    if ((u4Flags & DMX_FILTER_FLAG_CRC) != 0)
    {
        prFilter->fgCheckCrc = prFilterStruct->arBank[u1Bank].fgCheckCrc;
    }

    // Valid
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        prFilter->fgEnable= prFilterStruct->arBank[u1Bank].fgEnable;
    }

    // Filter mode
    if ((u4Flags & DMX_FILTER_FLAG_MODE) != 0)
    {
        prFilter->eMode = prFilterStruct->eMode;
    }

    // Pattern
    if ((u4Flags & DMX_FILTER_FLAG_PATTERN) != 0)
    {
        prFilter->au4Data[0] = ((UINT32)prFilterStruct->arBank[u1Bank].au1Data[0] << 24) |
                               ((UINT32)prFilterStruct->arBank[u1Bank].au1Data[1] << 16) |
                               ((UINT32)prFilterStruct->arBank[u1Bank].au1Data[2] << 8) |
                               (UINT32)prFilterStruct->arBank[u1Bank].au1Data[3];
        prFilter->au4Data[1] = ((UINT32)prFilterStruct->arBank[u1Bank].au1Data[4] << 24) |
                               ((UINT32)prFilterStruct->arBank[u1Bank].au1Data[5] << 16) |
                               ((UINT32)prFilterStruct->arBank[u1Bank].au1Data[6] << 8) |
                               (UINT32)prFilterStruct->arBank[u1Bank].au1Data[7];

        prFilter->au4Mask[0] = ((UINT32)prFilterStruct->arBank[u1Bank].au1Mask[0] << 24) |
                               ((UINT32)prFilterStruct->arBank[u1Bank].au1Mask[1] << 16) |
                               ((UINT32)prFilterStruct->arBank[u1Bank].au1Mask[2] << 8) |
                               (UINT32)prFilterStruct->arBank[u1Bank].au1Mask[3];
        prFilter->au4Mask[1] = ((UINT32)prFilterStruct->arBank[u1Bank].au1Mask[4] << 24) |
                               ((UINT32)prFilterStruct->arBank[u1Bank].au1Mask[5] << 16) |
                               ((UINT32)prFilterStruct->arBank[u1Bank].au1Mask[6] << 8) |
                               (UINT32)prFilterStruct->arBank[u1Bank].au1Mask[7];
    }

    _DMX_Unlock();

    // Debug
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        DMXSECLOG(DMX_LOG_PSI 6, u1FilterIndex, "%s filter %u bank %u\n", prFilter->fgEnable ? "Enable" : "Disable",
                  u1FilterIndex, u1Bank);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetGenFilter
 *  Setup a generic section filter
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                       const DMX_FILTER_GENERIC_T* prFilter)
{
    UINT32 u4FilterWord;
    UINT32 i;
    FILTER_STRUCT_T *prFilterStruct = NULL;
#ifdef CC_DMX_MULTIPLE_SECTION
    PID_STRUCT_T *prPidStruct = NULL;
    FVR_PID_STRUCT_T *prFvrStruct = NULL;
    UINT8 u1SecCount = 0, u1SecIdx = 0;
    UINT32 au4SecMask[SEC_CNT] = {0};
    UINT32 j;
#endif // CC_DMX_MULTIPLE_SECTION

    ASSERT(prFilter != NULL);

    _DMX_SetFilterType(FILTER_TYPE_GENERIC);

    // Check parameters
    if ((u1FilterIndex >= DMX_NUM_FILTER_INDEX))
    {
        return FALSE;
    }

    if (u4Flags == DMX_FILTER_FLAG_NONE)
    {
        return TRUE;
    }

	if (prFilter->u1Tsidx >= DMX_FRAMER_COUNT)
    {
        LOG(0, "Tsidx is out of Framer counter!\n");
        return FALSE;
    }

    _DMX_Lock();

    prFilterStruct = &_arFilterStruct[u1FilterIndex];

    u4FilterWord = SECTION_FILTER_CTRL(u1FilterIndex);

    // PID index
    if ((u4Flags & DMX_FILTER_FLAG_PIDX) != 0)
    {
        UINT8 u1Pidx;

        u1Pidx = prFilter->u1Pidx;
        if (prFilter->fgIsFvr)
        {
            u1Pidx += DMX_FVR_START_PID;
        }

        u4FilterWord &= ~0xFF;
        u4FilterWord |= ((UINT32)(u1Pidx & 0xFF));
        prFilterStruct->rGeneric.u1Pidx = prFilter->u1Pidx;
        prFilterStruct->fgIsFvr = prFilter->fgIsFvr;
    }

    // Offset
    if ((u4Flags & DMX_FILTER_FLAG_OFFSET) != 0)
    {
        ASSERT(prFilter->u1Offset < 16);

        u4FilterWord &= ~0xF0000;
        u4FilterWord |= ((prFilter->u1Offset & 0xF) << 16);

        prFilterStruct->rGeneric.u1Offset = prFilter->u1Offset;
    }

    // Check CRC
    if ((u4Flags & DMX_FILTER_FLAG_CRC) != 0)
    {
        u4FilterWord &= ~0x1000000;
        u4FilterWord |= ((prFilter->fgCheckCrc ? 1 : 0) << 24);

        prFilterStruct->rGeneric.fgCheckCrc = prFilter->fgCheckCrc;
    }

    if ((u4Flags & DMX_FILTER_FLAG_MODE) != 0)
    {
        prFilterStruct->rGeneric.eMode = prFilter->eMode;
    }

    // Valid
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        u4FilterWord &= ~0x80000000;
        u4FilterWord |= ((prFilter->fgEnable ? 1 : 0) << 31);
        DMXSECLOG(DMX_LOG_PSI 6, u1FilterIndex, "[PSI PARSER]the Section Filter %d,Ts index: %d, Pid index is %d\n",u1FilterIndex, prFilter->u1Tsidx ,prFilterStruct->rGeneric.u1Pidx);

#ifdef CC_DMX_MULTIPLE_SECTION
        if (!prFilterStruct->fgIsFvr)
        {
            prPidStruct = &_arPidStruct[prFilterStruct->rGeneric.u1Pidx];
            if (prPidStruct != NULL)
            {
                u1SecCount = prPidStruct->u1SecCount;
                u1SecIdx = prPidStruct->u1SecIdx;
                x_memcpy((VOID*)au4SecMask, (VOID*)prPidStruct->au4SecMask, SEC_CNT * sizeof(UINT32));
            }
        }
        else
        {
            prFvrStruct = _FVR_GetPidStruct(prFilterStruct->rGeneric.u1Pidx);
            if (prFvrStruct != NULL)
            {
                u1SecCount = prFvrStruct->u1SecCount;
                u1SecIdx = prFvrStruct->u1SecIdx;
                x_memcpy((VOID*)au4SecMask, (VOID*)prFvrStruct->au4SecMask,SEC_CNT * sizeof(UINT32));
            }
        }

        if (prFilter->fgEnable)
        {
            if (!prFilterStruct->rGeneric.fgEnable)
            {
                u1SecCount++;
            }
            if (u1SecCount == 1)
            {
                u1SecIdx = u1FilterIndex;
            }
            au4SecMask[SEC_NUM(u1FilterIndex)] |=(1 << (FILTERINDEX(u1FilterIndex)));
        }
        else
        {
            if ((u1SecCount > 0)&&(prFilterStruct->rGeneric.fgEnable))
            {
                u1SecCount--;
            }
            au4SecMask[SEC_NUM(u1FilterIndex)] &= ~(1 << (FILTERINDEX(u1FilterIndex)));

            if (u1SecCount == 1)
            {
                for (j = 0 ; j < SEC_CNT ; j++)
                {
                    for (i=0; i< 32; i++)
                    {
                        if ((au4SecMask[j] >> i) & 0x1)
                        {
                            u1SecIdx = i + j * 32;
                            break;
                        }
                    }
                }
            }
        }

        if (!prFilterStruct->fgIsFvr)
        {
            if (prPidStruct != NULL)
            {
                prPidStruct->u1SecCount = u1SecCount;
                prPidStruct->u1SecIdx = u1SecIdx;
                x_memcpy((VOID*)prPidStruct->au4SecMask ,(VOID*)au4SecMask, SEC_CNT * sizeof(UINT32));
            }
        }
        else
        {
            if (prFvrStruct != NULL)
            {
                prFvrStruct->u1SecCount = u1SecCount;
                prFvrStruct->u1SecIdx = u1SecIdx;
                x_memcpy((VOID*)prFvrStruct->au4SecMask ,(VOID*)au4SecMask, SEC_CNT * sizeof(UINT32));
            }
        }
#endif  // CC_DMX_MULTIPLE_SECTION
        prFilterStruct->rGeneric.fgEnable = prFilter->fgEnable;
    }

    // Mode
    if ((u4Flags & DMX_FILTER_FLAG_MODE) != 0)
    {
        prFilterStruct->eMode = DMX_FILTER_MODE_GENERIC;
    }

    // Pattern
    if ((u4Flags & DMX_FILTER_FLAG_PATTERN) != 0)
    {
        // Disable filter first
        SECTION_FILTER_CTRL(u1FilterIndex) &= ~(1U << 31);

        for (i=0; i<16; i++)
        {
            prFilterStruct->rGeneric.au1Mask[i] = prFilter->au1Mask[i];
            prFilterStruct->rGeneric.au1Data[i] = prFilter->au1Data[i];
            prFilterStruct->rGeneric.au1PosNeg[i] = prFilter->au1PosNeg[i];
        }

#ifdef CC_DMX_MULTIPLE_SECTION
        prFilterStruct->rGeneric.fgCompareSW = FALSE;
        for (i=8; i<16; i++)
        {
            if (prFilterStruct->rGeneric.au1Mask[i] != 0)
            {
                prFilterStruct->rGeneric.fgCompareSW = TRUE;
                break;
            }
        }
#endif // CC_DMX_MULTIPLE_SECTION

        prFilterStruct->rGeneric.fgNegSpread = FALSE;
        for (i=8; i<16; i++)
        {
            if (prFilter->au1PosNeg[i] != 0)
            {
                prFilterStruct->rGeneric.fgNegSpread = TRUE;
                prFilterStruct->rGeneric.fgCompareSW = TRUE;
                break;
            }
        }

        SECTION_FILTER_PAT(u1FilterIndex, 0) = (prFilter->au1Data[0] << 24) |
                (prFilter->au1Data[1] << 16) |
                (prFilter->au1Data[2] << 8)  |
                (prFilter->au1Data[3]) ;
        SECTION_FILTER_PAT(u1FilterIndex, 1) = (prFilter->au1Data[4] << 24) |
                (prFilter->au1Data[5] << 16) |
                (prFilter->au1Data[6] << 8)  |
                (prFilter->au1Data[7]);

        if (!prFilterStruct->rGeneric.fgNegSpread)
        {
            SECTION_FILTER_MASK(u1FilterIndex, 0) = (prFilter->au1Mask[0] << 24) |
                    (prFilter->au1Mask[1] << 16) |
                    (prFilter->au1Mask[2] << 8)  |
                    (prFilter->au1Mask[3]);
            SECTION_FILTER_MASK(u1FilterIndex, 1) =  (prFilter->au1Mask[4] << 24) |
                    (prFilter->au1Mask[5] << 16) |
                    (prFilter->au1Mask[6] << 8)  |
                    (prFilter->au1Mask[7]);
        }
        else
        {
            //Only Table ID
            SECTION_FILTER_MASK(u1FilterIndex, 0) = (UINT32)(prFilter->au1Mask[0] << 24);
            SECTION_FILTER_MASK(u1FilterIndex, 1) =  0;
        }

        SECTION_FILTER_POSNEG(u1FilterIndex, 0) = (prFilter->au1PosNeg[0] << 24) |
                (prFilter->au1PosNeg[1] << 16) |
                (prFilter->au1PosNeg[2] << 8)  |
                (prFilter->au1PosNeg[3]);
        SECTION_FILTER_POSNEG(u1FilterIndex, 1) = (prFilter->au1PosNeg[4] << 24) |
                (prFilter->au1PosNeg[5] << 16) |
                (prFilter->au1PosNeg[6] << 8)  |
                (prFilter->au1PosNeg[7]);
    }
    //for VCT PSI,use _DMX_VCTSoftFilterEnable() must be called
    prPidStruct = &_arPidStruct[prFilterStruct->rGeneric.u1Pidx];
    if(prPidStruct->u2Pid==0x1FFB)
    {
        _DMX_VCTSoftFilterEnable(prFilter->fgEnable,prFilterStruct->rGeneric.au1Data[0]);
        LOG(5,"_DMX_VCTSoftFilterEnable, enable %d,table id 0x%x.\n",prFilter->fgEnable,prFilterStruct->rGeneric.au1Data[0]);
    }
    SECTION_FILTER_CTRL(u1FilterIndex) = u4FilterWord;

    _DMX_Unlock();

    // Debug
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        DMXSECLOG(DMX_LOG_PSI 6, u1FilterIndex, "%s filter %u, Tsindex :%d.\n", prFilter->fgEnable ? "Enable" : "Disable",
                  u1FilterIndex, prFilter->u1Tsidx);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetGenFilter
 *  Get a section filter
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                       DMX_FILTER_GENERIC_T* prFilter)
{
    UINT32 i;
    FILTER_STRUCT_T *prFilterStruct = NULL;

    ASSERT(prFilter != NULL);

    // Check parameters
    if (u1FilterIndex >= DMX_NUM_FILTER_INDEX)
    {
        return FALSE;
    }

    if (u4Flags == DMX_FILTER_FLAG_NONE)
    {
        return TRUE;
    }

	if (prFilter->u1Tsidx >= DMX_FRAMER_COUNT)
    {
        LOG(0, "Tsidx is out of Framer counter!\n");
        return FALSE;
    }

    _DMX_Lock();

    prFilterStruct = &_arFilterStruct[u1FilterIndex];

    // PID index
    if ((u4Flags & DMX_FILTER_FLAG_PIDX) != 0)
    {
        prFilter->u1Pidx = prFilterStruct->rGeneric.u1Pidx;
        prFilter->fgIsFvr = prFilterStruct->fgIsFvr;
    }

    // Offset
    if ((u4Flags & DMX_FILTER_FLAG_OFFSET) != 0)
    {
        prFilter->u1Offset= prFilterStruct->rGeneric.u1Offset;
    }

    // Check CRC
    if ((u4Flags & DMX_FILTER_FLAG_CRC) != 0)
    {
        prFilter->fgCheckCrc = prFilterStruct->rGeneric.fgCheckCrc;
    }

    // Valid
    if ((u4Flags & DMX_FILTER_FLAG_VALID) != 0)
    {
        prFilter->fgEnable = prFilterStruct->rGeneric.fgEnable;
    }

    // Filter mode
    if ((u4Flags & DMX_FILTER_FLAG_MODE) != 0)
    {
        prFilter->eMode = prFilterStruct->rGeneric.eMode;
    }

    // Pattern
    if ((u4Flags & DMX_FILTER_FLAG_PATTERN) != 0)
    {
        for (i=0; i<16; i++)
        {
            prFilter->au1Mask[i] = prFilterStruct->rGeneric.au1Mask[i];
            prFilter->au1Data[i] = prFilterStruct->rGeneric.au1Data[i];
            prFilter->au1PosNeg[i] = prFilterStruct->rGeneric.au1PosNeg[i];
        }
    }

    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_SetOneByteFilter
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetOneByteFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                           const DMX_FILTER_GENERIC_T* prFilter)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetOneByteFilter
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetOneByteFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                           DMX_FILTER_GENERIC_T* prFilter)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetFilterStruct
 *  Get the Filter structure of a given filter index
 *
 *  @param  u4FilterIndex      filter index
 *  @retval The pointer of the filter structure
 */
//-----------------------------------------------------------------------------
FILTER_STRUCT_T* _DMX_GetFilterStruct(UINT8 u1Idx, UINT32 u4FilterIndex)
{
    ASSERT(u1Idx < DMX_FRAMER_COUNT);
    ASSERT(u4FilterIndex < DMX_NUM_FILTER_INDEX);

    return &_arFilterStruct[u4FilterIndex];
}


//-----------------------------------------------------------------------------
/** _DMX_SetPid
 *  Set a PID
 *  Note: The given buffer address should be a physical address if
 *  fgAllocateBuffer of DMX_PID_T is FALSE.
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid, BOOL fgKeepScrmble)
{
    PID_STRUCT_T rPidStruct;
    UINT32 u4PidEntry, u4PidWord0 = 0;
    BOOL fgPidWord0Modified = FALSE;

    ASSERT(prPid != NULL);

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();

    rPidStruct = _arPidStruct[u1Pidx];

#if 0
    // Check if it's already active
    if (rPidStruct.fgEnable)
    {
        // What can be done to an active pid is disabling it only
        if ((u4Flags & DMX_PID_FLAG_VALID) == 0)
        {
            _DMX_Unlock();
            LOG(3, "Error: try to change attributes of the active pidx %u\n",
                u1Pidx);
            return FALSE;
        }
    }
#endif

    // Get current PID settings
    u4PidEntry = PID_INDEX_TABLE(u1Pidx);

    // Ask uP to stop first
    // TBD...

    // Disable PID first
    PID_INDEX_TABLE(u1Pidx) &= 0x7fffffff;

    _DMX_Unlock();

    if ((u4Flags & DMX_PID_FLAG_INPUTTYPE) != 0)
    {
        rPidStruct.eInputType = prPid->eInputType;
    }

    // TS index, NOT use now.
    if ((u4Flags & DMX_PID_FLAG_TS_INDEX) != 0)
    {
        // Check parameter
        if (prPid->u1TsIndex > DMX_MAX_TUNER_INDEX)
        {
            LOG(2, "Invalid TS index: %u\n", prPid->u1TsIndex);
            return FALSE;
        }
		if(prPid->u1TsIndex <4)
		{
         u4PidEntry &= 0x9ffffffe;
         u4PidEntry |= (prPid->u1TsIndex & 0x3) << 29;
         rPidStruct.u1TsIndex = prPid->u1TsIndex & 0x3;
		}
		else if(prPid->u1TsIndex == 4)
		{
		  u4PidEntry &= 0x9ffffffe;
		  u4PidEntry |= 0x1;
		  rPidStruct.u1TsIndex = 0x4;
		  
		}
    }

	if(u4Flags &DMX_PID_FLAG_LOCAL_TSINDEX)
	{
	  rPidStruct.u1LocalTsIndex = prPid->u1LocalTsIndex;
	  u4PidEntry &= ~(0x6);
	  u4PidEntry |= (prPid->u1LocalTsIndex & 0x3) << 1;
	  
	}

    if ((u4Flags & DMX_PID_FLAG_INSTANCE_TAG) != 0)
    {
        rPidStruct.pvInstanceTag = prPid->pvInstanceTag;
    }

    // PID
    if ((u4Flags & DMX_PID_FLAG_PID) != 0)
    {
        // Check parameter
        /*
        if (prPid->u2Pid > 0x1fff)
        {
            LOG(3, "Invalid PID value: 0x%x\n", prPid->u2Pid);
            return FALSE;
        }
        */

        if (!_DMX_CTSetPid(u1Pidx, DMX_PID_FLAG_PID, &rPidStruct, prPid, &u4PidEntry))
        {
            u4PidEntry &= 0xe000ffff;
            u4PidEntry |= (prPid->u2Pid & 0x1fff) << 16;
            rPidStruct.u2Pid = prPid->u2Pid & 0x1fff;
        }
    }

    // PCR
    if ((u4Flags & DMX_PID_FLAG_PCR) != 0)
    {
        BOOL fgHasPcr = FALSE;
        DMX_PCR_MODE_T ePcrMode = DMX_PCR_MODE_NONE;

        switch (prPid->ePcrMode)
        {
        case DMX_PCR_MODE_OLD:
        case DMX_PCR_MODE_NEW:
            fgHasPcr = TRUE;
            ePcrMode = DMX_PCR_MODE_NEW;
            break;

        case DMX_PCR_MODE_NONE:
        default:
            break;
        }

        u4PidEntry &= 0xffff7fff;
        u4PidEntry |= ((fgHasPcr ? 1 : 0) << 15);
        rPidStruct.ePcrMode = ePcrMode;
        if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
        {
            u4PidEntry &= 0xfffffdff;
            u4PidEntry |= ((prPid->u1DeviceId & 0x1) << 9);
        }
    }

    // Buffer
    if ((u4Flags & DMX_PID_FLAG_BUFFER) != 0)
    {
        if (!_DmxSetPidBuffer(u1Pidx, prPid, &rPidStruct))
        {
            LOG(0, "%s: line=%u\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }

    //Disable Output Buffer
    if ((u4Flags & DMX_PID_FLAG_DISABLE_OUTPUT) != 0)
    {
        rPidStruct.fgDisableFifoOutput = prPid->fgDisableFifoOutput;
        if(rPidStruct.fgDisableFifoOutput)
        {
            PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) | (1 << 5));
        }
        else
        {
            PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) & ~(1 << 5));
        }
    }
	
    // Steering
    if ((u4Flags & DMX_PID_FLAG_STEER) != 0)
    {
        UINT32 u4Steering;
        BOOL fgTo1394, fgToPvr, fgToFTuP;

        fgTo1394 = ((prPid->u1SteerMode & DMX_STEER_TO_1394) != 0);
        fgToPvr = ((prPid->u1SteerMode & DMX_STEER_TO_PVR) != 0);
        fgToFTuP = ((prPid->u1SteerMode & DMX_STEER_TO_FTUP) != 0);

        u4Steering = (UINT32)(((fgTo1394 ? 1 : 0) << 7) | ((fgToPvr ? 1 : 0) << 6) |
                              ((fgToFTuP ? 1 : 0) << 5));

        u4PidEntry = (u4PidEntry & 0xffffff0f) | u4Steering;
        rPidStruct.u1SteerMode = prPid->u1SteerMode;
    }

    // Key index
    if ((u4Flags & DMX_PID_FLAG_KEY_INDEX) != 0)
    {
        UINT32 u4Tmp;

        //word 23 byte 0 is key index
        //move key index  to dmem.
        u4Tmp = PID_S_W(u1Pidx, 23) & 0xffffff00;
        u4Tmp |= prPid->u1KeyIndex;
        PID_S_W(u1Pidx, 23) = u4Tmp;
        rPidStruct.u1KeyIndex = prPid->u1KeyIndex;
    }

    // Descramble mode
    if ((u4Flags & DMX_PID_FLAG_DESC_MODE) != 0)
    {
        // For backward compatiable, we assume that key index will be set before descramble mode
        // If not, we may change wrong CA mode of the key index
        if (!_DMX_SetDescMode(rPidStruct.u1KeyIndex, prPid->eDescMode))
        {
            LOG(5, "fail to set descramble mode.\n");
        }

        if (prPid->eDescMode == DMX_DESC_MODE_NONE)
        {
            u4PidEntry &= (~0x3C00);    // clear CA descramble
        }
        else
        {
            u4PidEntry |= 0x400;    // Set CA do descramble
        }

        rPidStruct.eDescMode = prPid->eDescMode;
    }

    // Device ID
    if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
    {
        rPidStruct.u1DeviceId = prPid->u1DeviceId;
    }

    // Notification callback function
    if ((u4Flags & DMX_PID_FLAG_CALLBACK) != 0)
    {
        rPidStruct.pfnNotify = prPid->pfnNotify;
        rPidStruct.pvNotifyTag = prPid->pvNotifyTag;
    }

    // Scramble state callback function
    if ((u4Flags & DMX_PID_FLAG_SCRAMBLE_STATE) != 0)
    {
        rPidStruct.pfnScramble = prPid->pfnScramble;
        rPidStruct.pvScrambleTag = prPid->pvScrambleTag;
    }

    // Primary PID
    if ((u4Flags & DMX_PID_FLAG_PRIMARY) != 0)
    {
        if (prPid->fgPrimary)
        {
            u4PidEntry |= (1 << 14);
        }
        else
        {
            u4PidEntry &= ~(1 << 14);
        }

        rPidStruct.fgPrimary = prPid->fgPrimary;
    }

    // Pid buffer full
    if ((u4Flags & DMX_PID_FLAG_FIFO_FULL) != 0)
    {
        rPidStruct.fgFifoFull = prPid->fgFifoFull;
    }

    // Valid bit
    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        if ((rPidStruct.fgEnable && !prPid->fgEnable) ||
                (!rPidStruct.fgEnable && prPid->fgEnable))
        {
            _DmxDTVEnableCheck();

            u4PidEntry &= 0x7fffffff;
            if (!_DMX_CTSetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPidStruct, prPid, &u4PidEntry))
            {
                u4PidEntry |= ((prPid->fgEnable ? 1 : 0) << 31);
            }
            u4PidWord0 = PID_S_W(u1Pidx, 0);
            fgPidWord0Modified = TRUE;

            if (!_DmxEnablePid(u1Pidx, prPid->fgEnable, &u4PidEntry,
                               &u4PidWord0, &rPidStruct, fgKeepScrmble))
            {
                return FALSE;
            }

            if (!_DMX_CTConfig(u1Pidx, &rPidStruct, &u4PidEntry, prPid->fgEnable))
            {
                return FALSE;
            }
        }
    }

    // Update PID index table
    _DMX_Lock();
    if (fgPidWord0Modified)
    {
        PID_S_W(u1Pidx, 0) = u4PidWord0;
    }
    PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    _arPidStruct[u1Pidx] = rPidStruct;
    _DMX_Unlock();

#ifdef CC_DMX_TS130
    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        if (rPidStruct.fgTS130)
        {
            if (!_DMX_TS130_SetPidArray())
            {
                return FALSE;
            }
        }
		//set the instant id 
		//DMX_MUL_SetPidxInstance
    }
#endif  // CC_DMX_TS130

    // special handling for enabled PES PID
    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        if (!_DmxEnableAudioPesPid(u1Pidx, prPid->fgEnable, &u4PidEntry,
                                   &rPidStruct))
        {
            return FALSE;
        }
    }


    DMXPIDLOG(DMX_LOG_PID 7, u1Pidx, "PidTable[%u] = 0x%08x\n", u1Pidx, u4PidEntry);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetScrambleCheckPid
 *  Set a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetScrambleCheckPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid)
{
    PID_STRUCT_T rPidStruct;

    ASSERT(prPid != NULL);

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    _DMX_Lock();

    rPidStruct = _arPidStruct[u1Pidx];
    _DMX_Unlock();

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    if ((u4Flags & DMX_PID_FLAG_DISABLE_OUTPUT) != 0)
    {
        rPidStruct.fgDisableFifoOutput = prPid->fgDisableFifoOutput;
        if (rPidStruct.fgDisableFifoOutput)
        {
            PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) | (1 << 5));
        }
        else
        {
            PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) & ~(1 << 5));
        }
    }

    _DMX_Lock();
    _arPidStruct[u1Pidx] = rPidStruct;
    _DMX_Unlock();

    return _DMX_SetPid(u1Pidx, u4Flags, prPid, FALSE);
}


//-----------------------------------------------------------------------------
/** Set a seamless PID switch
 *
 *  @param  u1Pidx          The new PID index
 *  @param  u4Flags         The old PID index
 *  @param  prPid           The PID value
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetSeamlessPID(UINT8 u1NewPidx, UINT8 u1OldPidx, UINT16 u2Pid)
{
    PID_STRUCT_T rPidStruct, rOldPidStruct;
    UINT32 u4PidEntry, i = 0;
    UINT32 u4OrgScrambleState;

    _DMX_Lock();
    rPidStruct = _arPidStruct[u1NewPidx];
    rOldPidStruct = _arPidStruct[u1OldPidx];
    _DMX_Unlock();

    if ((u1NewPidx >= DMX_NUM_PID_INDEX) || (u1OldPidx >= DMX_NUM_PID_INDEX))
    {
        LOG(3, "Invalid PID index: %u or %u\n", u1NewPidx, u1OldPidx);
        return FALSE;
    }

    // check if u1OldPidx is enabled
    if (!rOldPidStruct.fgEnable)
    {
        LOG(3, "PID index is not enabled: %u\n", u1OldPidx);
        return FALSE;
    }

    _DMX_Lock();

    // Disable PID first
    PID_INDEX_TABLE(u1NewPidx) &= 0x7fffffff;

    // Get old PID settings
    u4PidEntry = PID_INDEX_TABLE(u1OldPidx);

    // Copy PID struct
    x_memcpy(&rPidStruct, &rOldPidStruct, sizeof(rPidStruct));
    rPidStruct.u4NextPic = 0;
    // Copy PID DMEM
    for (i = 0; i < DMX_DMEM_ENTRY_LEN; i++)
    {
        PID_S_W(u1NewPidx, i) = PID_S_W(u1OldPidx, i);
    }

    PID_S_W(u1NewPidx, 1) = 0;
    u4OrgScrambleState = PID_S_W(u1NewPidx, 2);
    PID_S_W(u1NewPidx, 2) = (u4OrgScrambleState & 0x0000FF00);
    PID_S_W(u1NewPidx, 3) = 0xffff0001;        // Default for PES
    PID_S_W(u1NewPidx, 4) = 0xffffffff;     // PES previous start code pattern
    PID_S_W(u1NewPidx, 18) = 0xffffffff;        // PES previous start code pattern

    // Update to new PID value
    u4PidEntry &= 0xe000ffff;
    u4PidEntry |= (u2Pid & 0x1fff) << 16;
    rPidStruct.u2Pid = u2Pid & 0x1fff;

    // fill switch information
    PID_S_W(u1NewPidx, 0) = PID_S_W(u1NewPidx, 0) | (1 << 10);
    PID_S_W(u1NewPidx, 22) = (PID_S_W(u1NewPidx, 22) & 0xFF00FFFF) | (u1OldPidx << 16);

    _DMX_Unlock();

    // allocate video channel
    if (rPidStruct.ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if (!_DMX_AllocateVideoChannel(u1NewPidx, rPidStruct.u1ChannelId))
        {
            return FALSE;
        }
    }

    _DMX_Lock();

    _arPidStruct[u1NewPidx] = rPidStruct;
    PID_INDEX_TABLE(u1NewPidx) = u4PidEntry;

    _DMX_Unlock();

    return TRUE;
}


BOOL _DMX_SetIgnoreCCCheck(UINT8 u1Pidx, BOOL fgEnable)
{
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    if (fgEnable)
    {
        PID_S_W(u1Pidx, 0) = PID_S_W(u1Pidx, 0) | (0x1 << 12);
    }
    else
    {
        PID_S_W(u1Pidx, 0) = PID_S_W(u1Pidx, 0) & ~(0x1 << 12);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_TS_SetAudioType
 *
 *  If the input type is DMX_IN_PLAYBACK_TS, then users need to specify the
 *  format of the audio input data.  The micro-code will filter the number
 *  of bytes specified in "PID word 0, byte 3" from the beginning of the PES
 *  payload if "PID word 0, bit 6" is set.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TS_SetAudioType(UINT8 u1Pidx, DMX_AUDIO_TYPE_T eAudioType)
{
    BOOL fgRet;
    PID_STRUCT_T *prPidStruct;
#ifdef ENABLE_MULTIMEDIA
    UINT8 u1Inst = 0;
#endif

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    // _DMX_Lock() and _DMX_Unlock() are not needed because a pointer is used.
    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    if (prPidStruct->ePidType != DMX_PID_TYPE_ES_AUDIO)
    {
        return FALSE;
    }

#ifdef ENABLE_MULTIMEDIA
    u1Inst = _DMX_MUL_GetPidxInstId(u1Pidx);
    if (u1Inst >= DMX_MAX_INSTANCE)
    {
        return FALSE;
    }
#endif  // ENABLE_MULTIMEDIA

    switch (eAudioType)
    {
    case DMX_AUDIO_MPEG:
    case DMX_AUDIO_AC3:
    case DMX_AUDIO_DTS:
        fgRet = TRUE;
        break;
    case DMX_AUDIO_LPCM:
        _DMX_Lock();
        PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) & 0x00FFFFFF) | 0x4000000;
        _aeDmxAideoType[u1Pidx] = eAudioType;
        _DMX_Unlock();
        fgRet = TRUE;
        break;
    default:
        LOG(5, "L%d: Unknown TS audio type %d!\n", __LINE__, eAudioType);
        fgRet = FALSE;
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_TS_GetAudioType
 *
 *  Called in HISR.
 */
//-----------------------------------------------------------------------------
DMX_AUDIO_TYPE_T _DMX_TS_GetAudioType(UINT8 u1Pidx)
{
#ifdef ENABLE_MULTIMEDIA
    UINT8 u1Inst = 0;

    u1Inst = _DMX_MUL_GetPidxInstId(u1Pidx);
    if (u1Inst >= DMX_MAX_INSTANCE)
    {
        return DMX_AUDIO_UNKNOWN;
    }
#endif  // ENABLE_MULTIMEDIA

    return _aeDmxAideoType[u1Pidx];
}

//-----------------------------------------------------------------------------
/** _DMX_HDCP_EnableDistCtrCheck
 *  Enable streamCtr value checking. This API is added for HDCP2.2 errata 
 *
 *  @param  fgEnable        Enable or disable check
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_HDCP_EnableDistCtrCheck(BOOL fgEnable)
{
    UINT32 u4Reg;

    _DMX_Lock();

    u4Reg = (B_FRAME_DROP_ENABLE & ~0xFF00u);

    if (fgEnable)
    {
        u4Reg |= 0x100;
    }

    B_FRAME_DROP_ENABLE = u4Reg;
        
    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_HDCP_IsInvalidCtr
 *  Check if streamCtr value is invalid. This API is added for HDCP2.2 errata 
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_HDCP_IsInvalidCtr(UINT8 u1Pidx)
{
    BOOL fgRet = FALSE;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    _DMX_Lock();

    fgRet = (BOOL)((PID_S_W(u1Pidx, 24) >> 24) & 0xFF);
    
    _DMX_Unlock();
    
    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_GetPid
 *  Get a PID
 *  Note: the returned buffer address (e.g. start, write pointer, and read pointer)
 *  are kernel logical address.
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid)
{
    PID_STRUCT_T* prPidStruct;
    ASSERT(prPid != NULL);

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid PID index: %u\n", u1Pidx);
        return FALSE;
    }

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    prPidStruct = &_arPidStruct[u1Pidx];

    if ((u4Flags & DMX_PID_FLAG_INPUTTYPE) != 0)
    {
        prPid->eInputType = prPidStruct->eInputType;
    }

    // TS index
    if ((u4Flags & DMX_PID_FLAG_TS_INDEX) != 0)
    {
        //In broadcast ts case,there will be more than one ts index mapping the same Instance,
        //so in this case we use tsidx from prPidStruct,The other case, we use tsidx from multi instance.
        if (prPidStruct->eInputType == DMX_IN_BROADCAST_TS)
        {
            prPid->u1TsIndex = prPidStruct->u1TsIndex;
        }
        else
        {
            prPid->u1TsIndex = DMX_MUL_GetTsIdx(DMX_MUL_GetPidxInstId(u1Pidx));
        }
    }

    // PID
    if ((u4Flags & DMX_PID_FLAG_PID) != 0)
    {
        prPid->u2Pid = prPidStruct->u2Pid;
    }

    // PCR
    if ((u4Flags & DMX_PID_FLAG_PCR) != 0)
    {
        prPid->ePcrMode = prPidStruct->ePcrMode;
    }

    // Buffer
    if ((u4Flags & DMX_PID_FLAG_BUFFER) != 0)
    {
        prPid->fgAllocateBuffer = prPidStruct->fgAllocateBuffer;
        prPid->u4BufAddr = prPidStruct->u4BufStart;
        prPid->u4BufSize = prPidStruct->u4BufLen;
        prPid->ePidType = prPidStruct->ePidType;
        prPid->u1ChannelId = prPidStruct->u1ChannelId;
    }

    // Steering
    if ((u4Flags & DMX_PID_FLAG_STEER) != 0)
    {
        prPid->u1SteerMode = prPidStruct->u1SteerMode;
    }

    // Descramble mode
    if ((u4Flags & DMX_PID_FLAG_DESC_MODE) != 0)
    {
        prPid->eDescMode = prPidStruct->eDescMode;
    }

    // Key index
    if ((u4Flags & DMX_PID_FLAG_KEY_INDEX) != 0)
    {
        prPid->u1KeyIndex = prPidStruct->u1KeyIndex;
    }

    // Device ID
    if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
    {
        prPid->u1DeviceId = prPidStruct->u1DeviceId;
    }

    // Notification callback function
    if ((u4Flags & DMX_PID_FLAG_CALLBACK) != 0)
    {
        prPid->pfnNotify = prPidStruct->pfnNotify;
        prPid->pvNotifyTag = prPidStruct->pvNotifyTag;
    }

    // Data pointers
    if ((u4Flags & DMX_PID_FLAG_DATA_POINTERS) != 0)
    {
        prPid->u4Rp = prPidStruct->u4Rp;
        prPid->u4Wp = prPidStruct->u4Wp;
        prPid->u4PeakBufFull = prPidStruct->u4PeakBufFull;
    }

    // Primary PID
    if ((u4Flags & DMX_PID_FLAG_PRIMARY) != 0)
    {
        prPid->fgPrimary = prPidStruct->fgPrimary;
    }

    if ((u4Flags & DMX_PID_FLAG_INSTANCE_TAG) != 0)
    {
        prPid->pvInstanceTag = prPidStruct->pvInstanceTag;
    }

    if ((u4Flags & DMX_PID_FLAG_DISABLE_OUTPUT) != 0)
    {
        prPid->fgDisableFifoOutput = prPidStruct->fgDisableFifoOutput;
    }

    // Valid
    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        prPid->fgEnable = prPidStruct->fgEnable;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_GetPsiMaxSize
 *
 *  @retval  psi pes max memory size
 */
//-----------------------------------------------------------------------------
UINT32  _DMX_GetPsiMaxSize(void)
{
    return _u4PsiMaxBufSize;

}

//-----------------------------------------------------------------------------
/** _DMX_GetPesMaxSize
 *
 *  @retval  psi pes max memory size
 */
//-----------------------------------------------------------------------------
UINT32  _DMX_GetPesMaxSize(void)
{
    return _u4PesMaxBufSize;

}





//-----------------------------------------------------------------------------
/** _DMX_FreePid
 *  Free a PID
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_FreePid(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;
    UINT8 u1SerialNumber;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    prPidStruct = &_arPidStruct[u1Pidx];
    if (prPidStruct->fgEnable)
    {
        UINT32 u4PidEntry, u4PidWord0;

        LOG(3, "Try to free an active PID! (pidx: %u)\n", u1Pidx);

        // Stop PID
        u4PidEntry = PID_INDEX_TABLE(u1Pidx);
        u4PidWord0 = PID_S_W(u1Pidx, 0);
        if (!_DmxEnablePid(u1Pidx, FALSE, &u4PidEntry, &u4PidWord0, prPidStruct, FALSE))
        {
            return FALSE;
        }

        // Actual stop PID
        _DMX_Lock();
        PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
        PID_S_W(u1Pidx, 0) = u4PidWord0;
        if (!_DMX_CTConfig(u1Pidx, prPidStruct, &u4PidEntry, FALSE))
        {
            return FALSE;
        }
        _DMX_Unlock();
    }

    // Stop and clear PID index table except for the Toggle bit.
    // Clear DMEM offset 0 except toggle bit
    _DMX_Lock();
    PID_INDEX_TABLE(u1Pidx) = PID_INDEX_TABLE(u1Pidx) & (1 << 8);
    PID_S_W(u1Pidx, 0) = PID_S_W(u1Pidx, 0) & (1 << 4);
    _DMX_Unlock();

    // Flush MPV
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        // Sync with MPV
#ifndef CC_DMX_EMULATION
        UINT8 u1Channel = 0;

        VERIFY(_DMX_GetVideoChannel(u1Pidx, &u1Channel));
        _DMX_TraceLog((UINT32)_DMX_FreePid, "_DMX_FreePid",
                      (UINT32)MPV_FlushEsmQ, "MPV_FlushEsmQ",
                      DMX_TRACE_USE_LOCK | DMX_TRACE_FUNCTION_CALL);

        MPV_FlushEsmQ(u1Channel, TRUE);

        _DMX_TraceLog((UINT32)_DMX_FreePid, "_DMX_FreePid",
                      (UINT32)MPV_FlushEsmQ, "MPV_FlushEsmQ",
                      DMX_TRACE_USE_LOCK | DMX_TRACE_FUNCTION_RETURN);
#endif

        // No need to send purge message if serail number is applied
        // Discard intermediate pictures, clean FIFO
//        VERIFY(_DMX_SendPurgeMessage(u1Pidx, FALSE));

        // free pidx video type
        if (!_DMX_FreeVideoTypeByPidx(u1Pidx))
        {
            return FALSE;
        }

        if (!_DMX_ClearPicFlag(u1Pidx))
        {
            return FALSE;
        }
    }
    else if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        // FIXME
        if (!_DMX_GetIsPanicReset())
        {
#ifndef CC_DMX_A1
            _DMX_TraceLog((UINT32)_DMX_FreePid, "_DMX_FreePid",
                          (UINT32)AUD_WaitDspFlush, "AUD_WaitDspFlush",
                          DMX_TRACE_USE_LOCK | DMX_TRACE_FUNCTION_CALL);

            AUD_WaitDspFlush(prPidStruct->u1DeviceId);

            _DMX_TraceLog((UINT32)_DMX_FreePid, "_DMX_FreePid",
                          (UINT32)AUD_WaitDspFlush, "AUD_WaitDspFlush",
                          DMX_TRACE_USE_LOCK | DMX_TRACE_FUNCTION_RETURN);
#endif

            _DMX_TraceLog((UINT32)_DMX_FreePid, "_DMX_FreePid",
                          (UINT32)AUD_MMClearEOSFlag, "AUD_MMClearEOSFlag",
                          DMX_TRACE_USE_LOCK | DMX_TRACE_FUNCTION_CALL);

            AUD_MMClearEOSFlag(prPidStruct->u1DeviceId);

            _DMX_TraceLog((UINT32)_DMX_FreePid, "_DMX_FreePid",
                          (UINT32)AUD_MMClearEOSFlag, "AUD_MMClearEOSFlag",
                          DMX_TRACE_USE_LOCK | DMX_TRACE_FUNCTION_RETURN);

            _DMX_TraceLog((UINT32)_DMX_FreePid, "_DMX_FreePid",
                          (UINT32)AUD_MMClearPesCnt, "AUD_MMClearPesCnt",
                          DMX_TRACE_USE_LOCK | DMX_TRACE_FUNCTION_CALL);

            AUD_MMClearPesCnt(prPidStruct->u1DeviceId);

            _DMX_TraceLog((UINT32)_DMX_FreePid, "_DMX_FreePid",
                          (UINT32)AUD_MMClearPesCnt, "AUD_MMClearPesCnt",
                          DMX_TRACE_USE_LOCK | DMX_TRACE_FUNCTION_RETURN);

        }

        // No need to send purge message if serial number is applied
        // Discard intermediate frames, clean FIFO
//        VERIFY(_DMX_SendPurgeMessage(u1Pidx, FALSE));
    }

    //In some pvr play, need alloc audio pts for mw, here free it
    if ((prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)&&
        (prPidStruct->eInputType == DMX_IN_PLAYBACK_TS))
    {
        _arDMXNotifyInfoPts.fgGetPtsEn = FALSE;       
        _arDMXNotifyInfoPts.u4Pts = 0;

    }

    // Free buffer
    if (!_DmxFreePidBuffer(u1Pidx))
    {
        return FALSE;
    }

    _DMX_Lock();

    // Clear PID structures and PID memory
    // Note that serial number must be preserved across freeing PID
    // Also, peak buffer fullness is stored while channel changed
    u1SerialNumber = prPidStruct->u1SerialNumber;
    x_memset((void*)prPidStruct, 0, sizeof (PID_STRUCT_T));
    prPidStruct->u1SerialNumber = u1SerialNumber;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_FreeSwitchPid
 *  Free a PID
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_FreeSwitchPid(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;
    UINT8 u1SerialNumber;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    prPidStruct = &_arPidStruct[u1Pidx];
    if (prPidStruct->fgEnable)
    {
        UINT32 u4PidEntry, u4PidWord0;

        LOG(3, "Try to free an active PID! (pidx: %u)\n", u1Pidx);

        // Stop PID
        u4PidEntry = PID_INDEX_TABLE(u1Pidx);
        u4PidWord0 = PID_S_W(u1Pidx, 0);
        if (!_DmxEnablePid(u1Pidx, FALSE, &u4PidEntry, &u4PidWord0, prPidStruct, FALSE))
        {
            return FALSE;
        }

        // Actual stop PID
        _DMX_Lock();
        PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
        PID_S_W(u1Pidx, 0) = u4PidWord0;
        if (!_DMX_CTConfig(u1Pidx, prPidStruct, &u4PidEntry, FALSE))
        {
            return FALSE;
        }
        _DMX_Unlock();
    }

    // Stop and clear PID index table except for the Toggle bit.
    // Clear DMEM offset 0 except toggle bit
    _DMX_Lock();
    PID_INDEX_TABLE(u1Pidx) = PID_INDEX_TABLE(u1Pidx) & (1 << 8);
    PID_S_W(u1Pidx, 0) = PID_S_W(u1Pidx, 0) & (1 << 4);
    _DMX_Unlock();

    // Make sure the final DMA transaction terminates before freeing PID buffer.
    x_thread_delay(1);

    _DMX_Lock();

    PID_S_W(u1Pidx, 10) = 0x0;
    PID_S_W(u1Pidx, 11) = 0x0;

    _DMX_Unlock();

    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        // Reset pidx-to-channel mapping
        _DMX_FreeVideoChannel(u1Pidx);
    }

    _DMX_Lock();

    // Clear PID structures and PID memory
    // Note that serial number must be preserved across freeing PID
    // Also, peak buffer fullness is stored while channel changed
    u1SerialNumber = prPidStruct->u1SerialNumber;
    x_memset((void*)prPidStruct, 0, sizeof (PID_STRUCT_T));
    prPidStruct->u1SerialNumber = u1SerialNumber;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_FreePidBuffer(UINT8 u1Pidx)
 *  Free PID buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_FreePidBuffer(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    prPidStruct = &_arPidStruct[u1Pidx];
    if (prPidStruct->fgEnable)
    {
        LOG(3, "Try to free buffer of active PID! (pidx: %u)\n", u1Pidx);
        return FALSE;
    }

    return _DmxFreePidBuffer(u1Pidx);
}


//-----------------------------------------------------------------------------
/** _DMX_ResetPidBuffer
 *  Reset PID buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  prPidStruct     Internal PID data structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ResetPidBuffer(UINT8 u1Pidx, PID_STRUCT_T* prPidStruct, BOOL fgKeepScramble)
{
    UINT32 u4BufStart, u4OrgScrambleState;
    BOOL fgRet = TRUE;

    FUNC_ENTRY;

    ASSERT(prPidStruct != NULL);

    _DMX_Lock();

    // Check if PID is inactive
    if ((prPidStruct->fgEnable) ||
            ((PID_INDEX_TABLE(u1Pidx) & 0x80000000) != 0))
    {
        _DMX_Unlock();
        LOG(2, "Reset buffer of an active pid\n");
        return FALSE;
    }

    // Setup PID structures
    PID_S_W(u1Pidx, 1) = 0;

    if (fgKeepScramble)
    {
        u4OrgScrambleState = PID_S_W(u1Pidx, 2);
        PID_S_W(u1Pidx, 2) = (u4OrgScrambleState & 0x0000FF00);
        
        u4OrgScrambleState = PID_S_W(u1Pidx, 24);
        PID_S_W(u1Pidx, 24) = (u4OrgScrambleState & 0x000000FF);
    }
    else
    {
        PID_S_W(u1Pidx, 2) = 0x0000ff00;        // Scramble state = unknown
        PID_S_W(u1Pidx, 24) = 0x000000ff;       // Raw scramble state = unknown
    }

    PID_S_W(u1Pidx, 3) = 0xffff0001;        // Default for PES
    PID_S_W(u1Pidx, 4) = 0xffffffff;        // PES previous start code pattern
    PID_S_W(u1Pidx, 18) = 0xffffffff;       // PES previous start code pattern

    // Reset payload FIFO
    u4BufStart = prPidStruct->u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4Wp = u4BufStart;
	if(prPidStruct->fgIommuEnable)
    {
     u4BufStart = VIRTUAL(u4BufStart);
	}
	else
	{
	  u4BufStart = PHYSICAL(u4BufStart);
	}

    PID_S_W(u1Pidx, 5) = u4BufStart;     // FIFO start
    PID_S_W(u1Pidx, 6) = (u4BufStart + prPidStruct->u4BufLen) - 1; //End Addr

    PID_S_W(u1Pidx, 7) = u4BufStart;        // PES_start
    PID_S_W(u1Pidx, 8) = u4BufStart;        // Write pointer
    PID_S_W(u1Pidx, 9) = u4BufStart;        // Read pointer

    // Set a small threshold for audio type, added from v2.16
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        // uCode doesn't handle the case that partial PES threshold is
        // reached but PES header still incomplete. So the minimum
        // threshold is 2 (PES_header_length field is 8-bit in PES header)
//        ASSERT(DMX_INT_THRESHOLD_AUDIO > 1);

        PID_S_W(u1Pidx, 15) = _u2IntThresholdAudio;
    }
    else if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        PID_S_W(u1Pidx, 15) = _u2IntThresholdVideo;
    }
    else
    {
        PID_S_W(u1Pidx, 15) = 0;            // 0: No partial PES interrupt
    }

    // Extra checking/setting for different PID types
    //
    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_PSI:
        PID_S_W(u1Pidx, 3) = 0xffff0002;
        prPidStruct->u4SectionRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_ES_VIDEO:
    case DMX_PID_TYPE_ES_AUDIO:
    case DMX_PID_TYPE_ES_VIDEOCLIP:
    case DMX_PID_TYPE_ES_OTHER:
    case DMX_PID_TYPE_PES_AUDIO:
        // Reset header FIFO
        u4BufStart = PHYSICAL(prPidStruct->u4HeaderBufAddr);
        PID_S_W(u1Pidx, 12) = u4BufStart;   // Start
        PID_S_W(u1Pidx, 13) = u4BufStart;   // Write pointer
        PID_S_W(u1Pidx, 14) = u4BufStart;   // Read pointer
        prPidStruct->u4NextPic = 0;
        prPidStruct->u4PesRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_PES:
    case DMX_PID_TYPE_TS_RAW:
    case DMX_PID_TYPE_AV_PES:
        prPidStruct->u4PesRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_ES_DTCP:
    case DMX_PID_TYPE_NONE:
        break;

    default:
        fgRet = FALSE;
        break;
    }

    _DMX_Unlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_FlushPidBuffer(UINT8 u1Pidx)
 *  Flush PID buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_FlushPidBuffer(UINT8 u1Pidx)
{
    PID_STRUCT_T rPidStruct;
    UINT32 u4PidEntry, u4PidWord0, u4BufStart;
    BOOL fgEnable;
    BOOL fgFifofull;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    rPidStruct = _arPidStruct[u1Pidx];
    fgEnable = rPidStruct.fgEnable;
    fgFifofull = rPidStruct.fgFifoFull;

    DMXPIDLOG(DMX_LOG_PID 6, u1Pidx, "Flush pidx %u,fgEnable: %s.\n", u1Pidx,fgEnable ? "YES" : "NO");
    DMXPIDLOG(DMX_LOG_PID 6, u1Pidx, "Flush pidx %u buffer\n", u1Pidx);

    // Stop PID and clear fifo full flag
    _DMX_Lock();
    rPidStruct.fgFifoFull = FALSE;
    _DMX_Unlock();

    u4PidEntry = PID_INDEX_TABLE(u1Pidx);
    u4PidWord0 = PID_S_W(u1Pidx, 0);
    if (!_DmxEnablePid(u1Pidx, FALSE, &u4PidEntry, &u4PidWord0, &rPidStruct, FALSE))
    {
        return FALSE;
    }

    _DMX_Lock();

    // Actual stop PID
    PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    PID_S_W(u1Pidx, 0) = u4PidWord0;

    if (!_DMX_CTConfig(u1Pidx, &rPidStruct, &u4PidEntry, FALSE))
    {
        return FALSE;
    }
    _DMX_Unlock();

    HAL_Delay_us(1);

    // Move read/write pointers to buffer start
    _DMX_Lock();
    u4BufStart = rPidStruct.u4BufStart;
    rPidStruct.u4Rp = u4BufStart;
    rPidStruct.u4Wp = u4BufStart;
	if(rPidStruct.fgIommuEnable)
	{
	  u4BufStart = VIRTUAL(u4BufStart);
	}
	else
	{
	 u4BufStart = PHYSICAL(u4BufStart);
	}

    PID_S_W(u1Pidx, 7) = u4BufStart;        // PES_start
    PID_S_W(u1Pidx, 8) = u4BufStart;        // Write pointer
    PID_S_W(u1Pidx, 9) = u4BufStart;        // Read pointer
    _arPidStruct[u1Pidx] = rPidStruct;

    _DMX_Unlock();

    //if the pid buffer full and pid type is psi type , re-enable pid again
    // If the PID was not enabled, just return TRUE.
    if (fgFifofull && (rPidStruct.ePidType == DMX_PID_TYPE_PSI))
    {
        LOG(5, "Flush pidx %u,PidType: PSI ,Psi pid buffer full,Need re- enable pid.\n", u1Pidx);
    }
    else
    {
        if (!fgEnable)
        {
            LOG(5, "Flush pidx %u,fgEnable: %s, disable just return true.\n", u1Pidx,fgEnable ? "YES" : "NO");
            return TRUE;
        }
    }

    // Re-enable PID only if it was enabled.
    if (!_DmxEnablePid(u1Pidx, TRUE, &u4PidEntry, &u4PidWord0, &rPidStruct, FALSE))
    {
        return FALSE;
    }

    // special handling for enabled PES PID
    if (!_DmxEnableAudioPesPid(u1Pidx, TRUE, &u4PidEntry, &rPidStruct))
    {
        return FALSE;
    }

    _DMX_Lock();
    if (!_DMX_CTConfig(u1Pidx, &rPidStruct, &u4PidEntry, TRUE))
    {
        return FALSE;
    }
    PID_INDEX_TABLE(u1Pidx) = u4PidEntry;
    PID_S_W(u1Pidx, 0) = u4PidWord0;
    _arPidStruct[u1Pidx] = rPidStruct;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetPidIndex
 *  Find PID index for a given PID value
 *
 *  @param  u2Pid           PID
 *  @param  pu1PidIndex [out]
 *                          The PID index if found
 *
 *  @retval TRUE            Found
 *  @retval FALSE           Not found
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetPidIndex(UINT16 u2Pid, UINT8* pu1PidIndex)
{
    UINT32 i;
    BOOL fgRet = FALSE;

    ASSERT(pu1PidIndex != NULL);

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (_arPidStruct[i].u2Pid == u2Pid)
        {
            *pu1PidIndex = (UINT8)i;
            fgRet = TRUE;
            break;
        }
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_SetPcr
 *  Set a PCR
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  fgEnable        Enable PCR of the PID or not
 *  @param  fgNewMode       Use the new PCR mode or not
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetPcr(UINT8 u1PidIndex, BOOL fgEnable, BOOL fgNewMode)
{
    UINT32 u4PidInfo;
    PID_STRUCT_T* prPidStruct;
    DMX_PCR_MODE_T ePcrMode = DMX_PCR_MODE_NONE;

    if (u1PidIndex >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    if (fgEnable)
    {
        ePcrMode = DMX_PCR_MODE_NEW;
    }

    _DMX_Lock();

    prPidStruct = &_arPidStruct[u1PidIndex];
    prPidStruct->ePcrMode = ePcrMode;

    u4PidInfo = PID_INDEX_TABLE(u1PidIndex);
    u4PidInfo &= ~(1 << 15);
    u4PidInfo |= ((fgEnable ? 1 : 0) << 15);
    PID_INDEX_TABLE(u1PidIndex) = u4PidInfo;

    _DMX_Unlock();

    UNUSED(fgNewMode);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_IsVideoPid
 *  Check if a given PID index is for video data
 *
 *  @param  u1PidIndex      PID index
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsVideoPid(UINT8 u1PidIndex)
{
    BOOL fgIsVideoPid;

    ASSERT(u1PidIndex < DMX_NUM_PID_INDEX);

    fgIsVideoPid = (PID_S_W(u1PidIndex, 0) & (1 << 7)) != 0;

    return fgIsVideoPid;
}



#ifdef CC_DMX_EMULATION

//-----------------------------------------------------------------------------
/** _DMX_SetToDecoder
 *  Set data sink to real decoders
 *
 *  @param  fgToDecoder     TRUE: to decoders, FALSE: not to decoders
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetToDecoder(BOOL fgToDecoder)
{
    _DMX_Lock();
    _fgToDecoder = fgToDecoder;
    _DMX_Unlock();
}

BOOL _DMX_GetToDecoderStatus(void)
{
    return _fgToDecoder;
}

#endif

//-----------------------------------------------------------------------------
/** _DMX_IsToDecoder
 *  Query if data sink is set to real decoders
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsToDecoder(void)
{
#ifdef CC_DMX_EMULATION

    return _fgToDecoder;

#else

    return TRUE;

#endif
}


//-----------------------------------------------------------------------------
/** _DMX_GetPidCounters
 *  Get the counter structure of a given pid index
 *
 *  @param  u1Pidx          PID index
 *  @param  prCounters [output]
 *                          Pointer to the PID counter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetPidCounters(UINT8 u1Pidx, DMX_PID_COUNTERS_T* const prCounters)
{
    if ((u1Pidx >= DMX_NUM_PID_INDEX) || (prCounters == NULL))
    {
        return FALSE;
    }

    // Copy structure here to avoid race condition
    *prCounters = _arPidStruct[u1Pidx].rCounters;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ResetPidCounters
 *  Reset counters of a given pid index
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ResetPidCounters(UINT8 u1Pidx)
{
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    // Zero entire counter structure
    x_memset(&(_arPidStruct[u1Pidx].rCounters), 0,
             sizeof (DMX_PID_COUNTERS_T));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetStates
 *  Get demux states
 *
 *  @retval The pointer of the structure of demux states
 */
//-----------------------------------------------------------------------------
DMX_STATES_T* _DMX_GetStates(void)
{
    return &_rDmxStates;
}


//-----------------------------------------------------------------------------
/** _DMX_SetStartCodePattern_Ex
 *  Set search start code pattern
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetStartCodePattern_Ex(UINT8 u1NoIntrCount, const DMX_STARTCODE_T *prNoIntrStartCode,
                                 UINT8 u1IntrCount, const DMX_STARTCODE_T *prIntrStartCode)
{
    UINT16 u2Control;
    UINT8 i;

    if ((u1NoIntrCount > DMX_STARTCODE_NONINTR_NUM) ||
            (u1IntrCount > DMX_STARTCODE_INTR_NUM))
    {
        return FALSE;
    }

    u2Control = 0x0;

    _DMX_Lock();

    if ((u1NoIntrCount > 0) && (prNoIntrStartCode != NULL))
    {
        for (i=0; i<u1NoIntrCount; i++)
        {
            DMX_VCODE_S_W(i, 0) = prNoIntrStartCode[i].u4Pattern0;
            DMX_VCODE_S_W(i, 1) = prNoIntrStartCode[i].u4Pattern1;
            DMX_VCODE_S_W(i, 2) = prNoIntrStartCode[i].u4Mask0;
            DMX_VCODE_S_W(i, 3) = prNoIntrStartCode[i].u4Mask1;
            u2Control |= (1 << i);
        }
    }

    if ((u1IntrCount > 0) && (prIntrStartCode != NULL))
    {
        for (i=0; i<u1IntrCount; i++)
        {
            DMX_VCODE_S_W(i + DMX_STARTCODE_NONINTR_NUM, 0) = prIntrStartCode[i].u4Pattern0;
            DMX_VCODE_S_W(i + DMX_STARTCODE_NONINTR_NUM, 1) = prIntrStartCode[i].u4Pattern1;
            DMX_VCODE_S_W(i + DMX_STARTCODE_NONINTR_NUM, 2) = prIntrStartCode[i].u4Mask0;
            DMX_VCODE_S_W(i + DMX_STARTCODE_NONINTR_NUM, 3) = prIntrStartCode[i].u4Mask1;
            u2Control |= (1 << (i + DMX_STARTCODE_NONINTR_NUM));
        }
    }

    // Write 0x1 to the "pattern changed" byte.
    DMX_VCODE_W(1) = ((UINT32)u2Control << 16) | 0xFFFF;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFramerErrorHandlingTable
 *  Set error handling table of framers
 *
 *  @param  u4Value         New value of error handling table
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetFramerErrorHandlingTable(UINT32 u4Value)
{
    DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, u4Value);
}


//-----------------------------------------------------------------------------
/** _DMX_GetFramerErrorHandlingTable
 *  Get error handling table of framers
 *
 *  @retval The current value of error handling table
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetFramerErrorHandlingTable(void)
{
    return DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE);
}

//-----------------------------------------------------------------------------
/** _DMX_SetFramerPacketErrorHandling
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetFramerPacketErrorHandling(UINT8 u1TsIndex, BOOL fgEnable, UINT32 u4Value)
{
    UINT32 u4Reg = 0;

    if (u1TsIndex >= DMX_FRAMER_COUNT)
    {
        ASSERT(0);
    }

#if defined(CC_MT5882)
    if (u1TsIndex == 2)
    {
        ASSERT(0);
    }
#endif

    if (u1TsIndex == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE) & 0xFFFF0000;
        if (fgEnable)
        {
            u4Reg |= u4Value;
        }
        DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, u4Reg);
    }
    else if (u1TsIndex == 1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE) & 0x0000FFFF;
        if (fgEnable)
        {
            u4Reg |= (u4Value << 16);
        }
        DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, u4Reg);
    }
    else if (u1TsIndex == 2)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL) & (0xFFFF); //Framer 2
        if (fgEnable)
        {
            u4Reg |= (u4Value << 16);
        }
        DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Reg);
    }
    else if (u1TsIndex == 3)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_BUF_CTRL) & (0xFFFF); //Framer 3
        if (fgEnable)
        {
            u4Reg |= (u4Value << 16);
        }
        DMXCMD_WRITE32(DMX_REG_DBM_BUF_CTRL, u4Reg);
    }
    else if(u1TsIndex == 4)
    {
      return TRUE;
    }
	else
    {
        ASSERT(0);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_GetFramerPacketErrorHandling
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetFramerPacketErrorHandling(UINT8 u1TsIndex)
{
    UINT32 u4Reg = 0;

    if (u1TsIndex >= DMX_FRAMER_COUNT)
    {
        ASSERT(0);
    }

    if (u1TsIndex == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE) & 0x0000FFFF;
    }
    else if (u1TsIndex == 1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_ERROR_HANDLE) >> 16;
        u4Reg &= 0xFFFF;
    }
    else if (u1TsIndex == 2)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL) >> 16; //Framer 2
        u4Reg &= 0xFFFF;
    }
    else if (u1TsIndex == 3)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_BUF_CTRL) >> 16; //Framer 3
        u4Reg &= 0xFFFF;
    }
    else
    {
        ASSERT(0);
    }

    return u4Reg;
}

//-----------------------------------------------------------------------------
/** _DMX_SetInterruptThreshold
 *  Set interrupt threshold for partial PES
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param  u4Bytes         Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32 u4Bytes)
{
    UINT32 u4Packets;
    BOOL fgRet;

    u4Packets = (u4Bytes + 187) / 188;
    if (u4Packets > 0x10000)
    {
        // Too big to set to demux
        return FALSE;
    }

    fgRet = FALSE;

    _DMX_Lock();

    switch (ePidType)
    {
    case DMX_PID_TYPE_ES_VIDEO:
        _u2IntThresholdVideo = (UINT16)u4Packets;
        fgRet = TRUE;
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        _u2IntThresholdAudio = (UINT16)u4Packets;
        fgRet = TRUE;
        break;

    default:
        break;
    }

    if (fgRet)
    {
        UINT32 i;

        // Also set the new threshold to all running PIDs of the same PID type
        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
#ifdef ENABLE_MULTIMEDIA
            // DRM use the same register value as threshold, so do not set threshold in MM case
            if (_arPidStruct[i].eInputType == DMX_IN_PLAYBACK_MM)
            {
                continue;
            }
#endif  // ENABLE_MULTIMEDIA
            if (_arPidStruct[i].fgEnable && (_arPidStruct[i].ePidType == ePidType))
            {
                PID_S_W(i, 15) = (UINT16)u4Packets;
            }
        }
    }

    _DMX_Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_GetInterruptThreshold
 *  Get interrupt threshold for partial PES
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param  pu4Bytes[output]
 *                          Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32* pu4Bytes)
{
    BOOL fgRet;

    if (pu4Bytes == NULL)
    {
        return FALSE;
    }

    fgRet = FALSE;

    switch (ePidType)
    {
    case DMX_PID_TYPE_ES_VIDEO:
        *pu4Bytes = (UINT32)(_u2IntThresholdVideo * 188);
        fgRet = TRUE;
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        *pu4Bytes = (UINT32)(_u2IntThresholdAudio * 188);
        fgRet = TRUE;
        break;

    default:
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_IsPidEnabled
 *  Is a PID enabled or disabled
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Disabled
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsPidEnabled(UINT8 u1Pidx)
{
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    return _DmxIsPidEnabled(u1Pidx);
}


//-----------------------------------------------------------------------------
/** _DMX_GetScrambleState
 *  Get scramble state
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval The scramble state
 */
//-----------------------------------------------------------------------------
DMX_SCRAMBLE_STATE_T _DMX_GetScrambleState(UINT8 u1Pidx)
{
    UINT32 u4ScrambleFlag;
    DMX_SCRAMBLE_STATE_T eState;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_SCRAMBLE_STATE_UNKNOWN;
    }

    u4ScrambleFlag = (PID_S_W(u1Pidx, 2) >> 8) & 0xff;
    switch (u4ScrambleFlag)
    {
    case 0:
        eState = DMX_SCRAMBLE_STATE_CLEAR;
        break;

    case 1:
        eState = DMX_SCRAMBLE_STATE_SCRAMBLED;
        break;

    default:
        eState = DMX_SCRAMBLE_STATE_UNKNOWN;
        break;
    }

    return eState;
}

//-----------------------------------------------------------------------------
/** _DMX_GetRawScrambleState
 *  Get scramble state
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval The scramble state
 */
//-----------------------------------------------------------------------------
DMX_SCRAMBLE_STATE_T _DMX_GetRawScrambleState(UINT8 u1Pidx)
{
    UINT32 u4ScrambleFlag;
    DMX_SCRAMBLE_STATE_T eState;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_SCRAMBLE_STATE_UNKNOWN;
    }

    u4ScrambleFlag = PID_S_W(u1Pidx, 24) & 0xff;
    switch (u4ScrambleFlag)
    {
    case 0:
        eState = DMX_SCRAMBLE_STATE_CLEAR;
        break;

    case 1:
        eState = DMX_SCRAMBLE_STATE_SCRAMBLED;
        break;

    default:
        eState = DMX_SCRAMBLE_STATE_UNKNOWN;
        break;
    }

    return eState;
}


//-----------------------------------------------------------------------------
/** _DMX_InitAPI
 *  Initialize stuffs in this file
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_InitAPI(void)
{
    UINT32 i;
#ifdef DMX_CHECK_SECTION_CRC
    UINT32 j;
#endif  // DMX_CHECK_SECTION_CRC

    // Reset demux states
    //
    _rDmxStates.rCaptureSettings.pfnHandler = NULL;
    _rDmxStates.rCaptureSettings.pvTag = NULL;
    _rDmxStates.rCaptureSettings.u4BufStart = 0;
    _rDmxStates.rCaptureSettings.u4BufEnd = 0;

    _eFilterType = FILTER_TYPE_LEGACY;

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        _rDmxStates.au1ChannelId[i] = DMX_NULL_CHANNEL_ID;
    }

#ifdef DEBUG_MEMORY_INTRUSION

    VERIFY(_DmxInitMemoryProtection());

#endif  // DEBUG_MEMORY_INTRUSION

#ifdef CC_EIT_SECTION_FILTER
    for(i=0; i<CHANNEL_MAX_NUM; i++)
    {
        _arEITFilterStruct[i].b_used = 0;
        x_memset(_arEITFilterStruct[i].aui1_cur_version,-1,EIT_TABLE_NUM);
        x_memset(_arEITFilterStruct[i].aui1_sec_mask,0,EIT_TABLE_NUM * EIT_SECTION_NUM);
    }
#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetDBMFramerStatus
 *  Get framer and DBM Status
 *
 *  @param  u4Value
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_GetDBMFramerStatus(UINT8 u1Tsidx, UINT32 *pu4DBMStatus, UINT32 *pu4FramerStatus)
{
    UINT32 u4Reg = 0;
    ASSERT(u1Tsidx <= 3);
    ASSERT((pu4DBMStatus != NULL) || (pu4FramerStatus != NULL));

    //klocwork warning
    if ((pu4DBMStatus == NULL) || (pu4FramerStatus == NULL))
    {
        LOG(0, "Input Error!\n");
        return;
    }

    if (u1Tsidx == 0)
    {
        u4Reg = DMX_REG_FRAMER0_FSM;
    }
    else if (u1Tsidx == 1)
    {
        u4Reg = DMX_REG_FRAMER1_FSM;
    }
    else if (u1Tsidx == 2)
    {
        u4Reg = DMX_REG_FRAMER2_FSM;
    }
    else if (u1Tsidx == 3)
    {
        u4Reg = DMX_REG_FRAMER3_FSM;
    }
    else
    {
        LOG(0, "Unknow Tsidx!\n");
        ASSERT(0);
    }

    *pu4DBMStatus = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
    *pu4FramerStatus = DMXCMD_READ32(u4Reg);

    return;

}

//-----------------------------------------------------------------------------
/** _DMX_GetPacketStatus
 *  Get Recirved and Dropped pkt nimber of each framer
 *
 */
//-----------------------------------------------------------------------------
VOID _DMX_GetPacketStatus(UINT8 u1Tsidx, UINT32 u4MsTime, UINT32 *pu4RcvCount, UINT32 *pu4DropCount)
{
    UINT32 u4Reg = 0, u4RcvCount, u4DropCount, u4Period, i;
    UINT32 u4RcvPkt0, u4DropPkt0;
    UINT32 u4RcvPkt1, u4DropPkt1;

    ASSERT(u1Tsidx <= 3);
    ASSERT((pu4RcvCount != NULL) || (pu4DropCount != NULL));

    //klocwork warning
    if ((pu4RcvCount == NULL) || (pu4DropCount == NULL))
    {
        LOG(0, "Input Error!\n");
        return;
    }

    if (u1Tsidx == 0)
    {
        u4Reg = DMX_REG_FRAMER0_STATUS;
    }
    else if (u1Tsidx == 1)
    {
        u4Reg = DMX_REG_FRAMER1_STATUS;
    }
    else if (u1Tsidx == 2)
    {
        u4Reg = DMX_REG_FRAMER2_STATUS;
    }
    else if (u1Tsidx == 3)
    {
        u4Reg = DMX_REG_FRAMER3_STATUS;
    }
    else
    {
        LOG(0, "Unknow Tsidx!\n");
        ASSERT(0);
    }

    u4RcvCount = 0;
    u4DropCount = 0;
    u4Period = 10; //fix 10ms per time

    u4RcvPkt0 = DMXCMD_READ32(u4Reg) & 0xFFFF;
    u4DropPkt0 = (DMXCMD_READ32(u4Reg) >> 16) & 0xFFFF;

    for (i = 0; i < u4MsTime; i+= u4Period)
    {
        x_thread_delay(u4Period);

        u4RcvPkt1 = DMXCMD_READ32(u4Reg) & 0xFFFF;
        u4DropPkt1 = (DMXCMD_READ32(u4Reg) >> 16) & 0xFFFF;

        if (u4RcvPkt1 < u4RcvPkt0)
        {
            u4RcvCount += ((0x10000 + u4RcvPkt1) - u4RcvPkt0);
        }
        else
        {
            u4RcvCount += (u4RcvPkt1 - u4RcvPkt0);
        }

        if (u4DropPkt1 < u4DropPkt0)
        {
            u4DropCount += ((0x10000 + u4DropPkt1) - u4DropPkt0);
        }
        else
        {
            u4DropCount += (u4DropPkt1 - u4DropPkt0);
        }

        u4RcvPkt0 = u4RcvPkt1;
        u4DropPkt0 = u4DropPkt1;
    }

    *pu4RcvCount = u4RcvCount;
    *pu4DropCount = u4DropCount;

    return;
}


//-----------------------------------------------------------------------------
/** _DMX_GetLockState
 *  Get lock state of framer
 *
 *  @retval TRUE            TS locked
 *  @retval FALSE           TS not lock
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetLockState(UINT8 u1FramerIdx)
{
    UINT32 u4Packets0, u4Packets1, u4Diff, i, u4Reg;
    UINT32 u4LockTimes = 0;

    ASSERT(u1FramerIdx <= 3);

    if (u1FramerIdx == 0)
    {
        u4Reg = DMX_REG_FRAMER0_STATUS;
    }
    else if (u1FramerIdx == 1)
    {
        u4Reg = DMX_REG_FRAMER1_STATUS;
    }
    else if (u1FramerIdx == 2)
    {
        u4Reg = DMX_REG_FRAMER2_STATUS;
    }
    else if (u1FramerIdx == 3)
    {
        u4Reg = DMX_REG_FRAMER3_STATUS;
    }
    else
    {
        LOG(0, "Unknow framer\n");
        return FALSE;
    }

    for (i=0; i<TS_LOCK_TEST_TIMES; i++)
    {
        u4Packets0 = DMXCMD_READ32(u4Reg) & 0xffff;

        x_thread_delay(1);

        u4Packets1 = DMXCMD_READ32(u4Reg) & 0xffff;
        if (u4Packets1 >= u4Packets0)
        {
            u4Diff = u4Packets1 - u4Packets0;
        }
        else
        {
            u4Diff = (u4Packets0 + 0x10000) - u4Packets1;
        }

        if (u4Diff >= TS_LOCK_MIN_PACKET)
        {
            u4LockTimes++;
        }
    }

    if (u4LockTimes == TS_LOCK_TEST_TIMES)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//-----------------------------------------------------------------------------
/** DMX_GetReceivedPacketNumber
 *  Get received TS packet number
 *
 *  @retval The received packet number
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetReceivedPacketNumber(void)
{
    UINT32 u4ReceivedPackets;

    u4ReceivedPackets = DMXCMD_READ32(DMX_REG_FRAMER0_STATUS) & 0xffff;

    return u4ReceivedPackets;
}


//-----------------------------------------------------------------------------
/** DMX_GetDroppedPacketNumber
 *  Get dropped TS packet number
 *
 *  @retval The dropped packet number
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetDroppedPacketNumber(void)
{
    UINT32 u4DroppedPackets;

    u4DroppedPackets = (DMXCMD_READ32(DMX_REG_FRAMER0_STATUS) & 0xffff0000)
                       >> 16;

    return u4DroppedPackets;
}


//-----------------------------------------------------------------------------
/** _DmxGetMinFreeBufferSize
 *
 *  Suppose the minimal size of the Audio/Video buffer is X, and this function
 *  is provided to calculate the value X.  This function must also take the
 *  Demux internal buffer into account by substracting the internal buffer size
 *  from X.  It is important that users must query the size of data (suppose Y)
 *  in DDI before calling this function.  Then, the amount of data to be sent
 *  to DDI must be less than (X - Y).  Otherwise, excess data will be
 *  lost/dropped in the Half-Push mode.
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxGetMinFreeBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T ePidType)
{
    UINT32 u4BufSize, u4FreeLen, u4MinFreeSize;
    UINT32 u4BufStart, u4BufEnd, u4Rp, u4Wp;
    UINT8 i;
    BOOL fgEnabled;
    PID_STRUCT_T *prPidStruct;
    BOOL fgAudioPrebufferEnalbe = FALSE;
    UINT8 u1HardAudioBufferIdx = 0xFF;

    if ((ePidType != DMX_PID_TYPE_ES_AUDIO) &&
            (ePidType != DMX_PID_TYPE_ES_VIDEO) &&
            (ePidType != DMX_PID_TYPE_PSI))
    {
        DMXLOG(DMX_LOG_BUFFER 9, "Cannot query buffer size other than A/V.\n");
        return 0;
    }

    u4MinFreeSize = 0xFFFFFFFF;

#ifdef CC_DMX_AUDIO_PREBUF
		fgAudioPrebufferEnalbe = _DMX_AudHandler_GetEnable();
		u1HardAudioBufferIdx =	_DMX_AudHandler_GetHardAudioBufferIdx();
#endif

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
#ifdef ENABLE_MULTIMEDIA
        if (_DMX_MUL_GetPidxInstId(i) != u1Inst)
        {
            continue;
        }
#else
        UNUSED(u1Inst);
#endif  // ENABLE_MULTIMEDIA

        prPidStruct = _DMX_GetPidStruct(i);

        fgEnabled = (_u1FBMmode) ? _DmxIsPidEnabled(i) : (prPidStruct->fgEnable);

	    //if enalbe audio prebuffer, and input type is playback TS, get audio buffer empty size is queue hard audio
        //buffer size, prebuffer audio buffer size is not include!
        if (ePidType == DMX_PID_TYPE_ES_AUDIO)
        {
            if(fgEnabled && fgAudioPrebufferEnalbe && (u1HardAudioBufferIdx != i))
            {
                continue;
            }
        }

        _DMX_Lock();

        if (fgEnabled && (prPidStruct->ePidType == ePidType))
        {
            UINT16 u2Loop;

            if (ePidType == DMX_PID_TYPE_PSI)
            {
                // Read the hardware buffer.
                // There's no need to convert hardware addresses to virtual ones.
                u4BufStart = PID_S_W(i, 5);
                u4BufEnd = PID_S_W(i, 6);
                u4Rp = PID_S_W(i, 9);
                // Repeatedly read the WP until it stabilizes.
                // The caller of this function must stop data transfer before
                // calling this function in the blocking DDI mode.  In such a
                // case, the following lines are redundant.
                u2Loop = 0;
                do
                {
                    u4Wp = PID_S_W(i, 8);
                    if ((++u2Loop) >= 1000)
                    {
                        LOG(3, "(1) Write pointer is not stabilized!\n");
                        break;
                    }
                }
                while (u4Wp != PID_S_W(i, 8));
            }
            else
            {
                // Read the hardware buffer.
                // There's no need to convert hardware addresses to virtual ones.
                u4BufStart = PID_S_W(i, 5);
                u4BufEnd = PID_S_W(i, 6);
                u4Rp = PID_S_W(i, 9);
                // Repeatedly read the WP until it stabilizes.
                // The caller of this function must stop data transfer before
                // calling this function in the blocking DDI mode.  In such a
                // case, the following lines are redundant.
                u2Loop = 0;
                do
                {
                    u4Wp = PID_S_W(i, 8);
                    if ((++u2Loop) >= 1000)
                    {
                        LOG(3, "(2) Write pointer is not stabilized!\n");
                        break;
                    }
                }
                while (u4Wp != PID_S_W(i, 8));
            }

            u4BufSize = (u4BufEnd - u4BufStart) + 1;
            u4FreeLen = EMPTYSIZE(u4Rp, u4Wp, u4BufSize);
            if (u4FreeLen < u4MinFreeSize)
            {
                u4MinFreeSize = u4FreeLen;
            }
        }
        _DMX_Unlock();
    }

    if (u4MinFreeSize == (UINT32)0xFFFFFFFF)
    {
        return 0;
    }

    return u4MinFreeSize;
}


//-----------------------------------------------------------------------------
/** _DmxGetMinFreeBufferSize by TS index
 *
 *  Suppose the minimal size of the Audio/Video buffer is X, and this function
 *  is provided to calculate the value X.  This function must also take the
 *  Demux internal buffer into account by substracting the internal buffer size
 *  from X.  It is important that users must query the size of data (suppose Y)
 *  in DDI before calling this function.  Then, the amount of data to be sent
 *  to DDI must be less than (X - Y).  Otherwise, excess data will be
 *  lost/dropped in the Half-Push mode.
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxGetMinFreeBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType)
{
    UINT32 u4BufSize, u4FreeLen, u4MinFreeSize;
    UINT32 u4BufStart, u4BufEnd, u4Rp, u4Wp;
    UINT8 i;
    BOOL fgEnabled;
    PID_STRUCT_T *prPidStruct;

    if ((ePidType != DMX_PID_TYPE_ES_AUDIO) &&
            (ePidType != DMX_PID_TYPE_ES_VIDEO) &&
            (ePidType != DMX_PID_TYPE_PSI))
    {
        DMXLOG(DMX_LOG_BUFFER 9, "Cannot query buffer size other than A/V.\n");
        return 0;
    }

    u4MinFreeSize = 0xFFFFFFFF;

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        prPidStruct = _DMX_GetPidStruct(i);

        fgEnabled = (_u1FBMmode) ? _DmxIsPidEnabled(i) : (prPidStruct->fgEnable);

        _DMX_Lock();

        if (fgEnabled && (prPidStruct->ePidType == ePidType) && (prPidStruct->u1TsIndex == u1TsIdx))
        {
            UINT16 u2Loop;

            if (ePidType == DMX_PID_TYPE_PSI)
            {
                // Read the hardware buffer.
                // There's no need to convert hardware addresses to virtual ones.
                u4BufStart = PID_S_W(i, 5);
                u4BufEnd = PID_S_W(i, 6);
                u4Rp = PID_S_W(i, 9);
                // Repeatedly read the WP until it stabilizes.
                // The caller of this function must stop data transfer before
                // calling this function in the blocking DDI mode.  In such a
                // case, the following lines are redundant.
                u2Loop = 0;
                do
                {
                    u4Wp = PID_S_W(i, 8);
                    if ((++u2Loop) >= 1000)
                    {
                        LOG(3, "(1) Write pointer is not stabilized!\n");
                        break;
                    }
                }
                while (u4Wp != PID_S_W(i, 8));
            }
            else
            {
                // Read the hardware buffer.
                // There's no need to convert hardware addresses to virtual ones.
                u4BufStart = PID_S_W(i, 5);
                u4BufEnd = PID_S_W(i, 6);
                u4Rp = PID_S_W(i, 9);
                // Repeatedly read the WP until it stabilizes.
                // The caller of this function must stop data transfer before
                // calling this function in the blocking DDI mode.  In such a
                // case, the following lines are redundant.
                u2Loop = 0;
                do
                {
                    u4Wp = PID_S_W(i, 8);
                    if ((++u2Loop) >= 1000)
                    {
                        LOG(3, "(2) Write pointer is not stabilized!\n");
                        break;
                    }
                }
                while (u4Wp != PID_S_W(i, 8));
            }

            u4BufSize = (u4BufEnd - u4BufStart) + 1;
            u4FreeLen = EMPTYSIZE(u4Rp, u4Wp, u4BufSize);
            if (u4FreeLen < u4MinFreeSize)
            {
                u4MinFreeSize = u4FreeLen;
            }
        }
        _DMX_Unlock();
    }

    if (u4MinFreeSize == (UINT32)0xFFFFFFFF)
    {
        return 0;
    }

    return u4MinFreeSize;
}


//-----------------------------------------------------------------------------
/** _DMX_GetMinFreeBufferSize
 *  Get free buffer: dram buffer + 10 TS packets inside DMX
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetMinFreeBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T ePidType)
{
    UINT32 u4MinFreeSize;

    u4MinFreeSize = _DmxGetMinFreeBufferSize(u1Inst, ePidType);

    // The internal buffer of the 5391 Demux can hold up to 10 TS packets
    // (188 * 10).  For each packet, at most 184 bytes will be sent to the
    // output buffer.  Thus, we shall be able to use 184 instead of 188 (not
    // tested yet!).
    u4MinFreeSize = (UINT32)((u4MinFreeSize < 1880) ? 0 : (u4MinFreeSize - 1880));

    return u4MinFreeSize;
}


//-----------------------------------------------------------------------------
/** _DMX_GetMinFreeDRAMBufferSize
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetMinFreeDRAMBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T ePidType)
{
    return _DmxGetMinFreeBufferSize(u1Inst, ePidType);
}


//-----------------------------------------------------------------------------
/** _DMX_GetMinFreeBufferSizeEx
 *  Get free buffer: dram buffer + 10 TS packets inside DMX
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetMinFreeBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType)
{
    UINT32 u4MinFreeSize;

    u4MinFreeSize = _DmxGetMinFreeBufferSizeEx(u1TsIdx, ePidType);

    // The internal buffer of the 5391 Demux can hold up to 10 TS packets
    // (188 * 10).  For each packet, at most 184 bytes will be sent to the
    // output buffer.  Thus, we shall be able to use 184 instead of 188 (not
    // tested yet!).
    u4MinFreeSize = (UINT32)((u4MinFreeSize < 1880) ? 0 : (u4MinFreeSize - 1880));

    return u4MinFreeSize;
}


//-----------------------------------------------------------------------------
/** _DMX_GetMinFreeDRAMBufferSizeEx
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_GetMinFreeDRAMBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType)
{
    return _DmxGetMinFreeBufferSizeEx(u1TsIdx, ePidType);
}


//-----------------------------------------------------------------------------
/** _DMX_SetTsIndex
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetTsIndex(UINT8 u1TunerIdx)
{
    DMX_PID_T rPid;
    UINT8 i;
#ifdef TIME_SHIFT_SUPPORT
    FVR_PID_T rFVRPid;
#endif // TIME_SHIFT_SUPPORT

    //-------------------------------------------
    // Play PID
    //-------------------------------------------
    for (i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if (!_DMX_GetPid(i, DMX_PID_FLAG_PID | DMX_PID_FLAG_VALID |DMX_PID_FLAG_TS_INDEX , &rPid))
        {
            return FALSE;
        }

        _DMX_CTSetIndex(i, u1TunerIdx, &rPid);

        if (rPid.fgEnable && (rPid.u1TsIndex != u1TunerIdx))
        {
            rPid.u1TsIndex = u1TunerIdx;
            if (!_DMX_SetPid(i, DMX_PID_FLAG_TS_INDEX, &rPid, FALSE))
            {
                return FALSE;
            }
        }
    }

#ifdef TIME_SHIFT_SUPPORT
    //-------------------------------------------
    // Record PID
    //-------------------------------------------
    x_memset((void*)&rFVRPid, 0, sizeof(rFVRPid));

    for (i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if (!_FVR_GetPid(i, FVR_PID_FLAG_ENABLE | FVR_PID_FLAG_TSINDEX, &rFVRPid))
        {
            return FALSE;
        }

        if (rFVRPid.fgEnable && (rFVRPid.u1TsIndex != u1TunerIdx))
        {
            rFVRPid.u1TsIndex = u1TunerIdx;
            if (!_FVR_SetPid(i, DMX_PID_FLAG_TS_INDEX, &rFVRPid))
            {
                return FALSE;
            }
        }
    }
#endif  //TIME_SHIFT_SUPPORT

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetBufPointer
*   This function used to set DMX buffer pointer for single move.
*   The given addresses should be physical addresses.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_SetBufPointer(UINT8 u1Idx, UINT32 u4BufStart, UINT32 u4BufEnd,
                        UINT32 u4Addr)
{
    PID_STRUCT_T* prPidStruct;

    //ASSERT(u4BufStart < u4BufEnd);
    //ASSERT((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd));
    if (!((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd)))
    {
        return FALSE;
    }

    if (u1Idx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    LOG(2, "%s - Start 0x%08lx End 0x%08lx Addr 0x%08lx\n",
        __FUNCTION__, u4BufStart, u4BufEnd, u4Addr);

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    _DMX_Lock();
    prPidStruct->u4BufStart = VIRTUAL(u4BufStart);
    prPidStruct->u4BufEnd = VIRTUAL(u4BufEnd);
    prPidStruct->u4Wp = VIRTUAL(u4Addr);
    prPidStruct->u4Rp = VIRTUAL(u4Addr);
    prPidStruct->u4SectionRp = VIRTUAL(u4Addr);
    prPidStruct->u4PesRp = VIRTUAL(u4Addr);
    prPidStruct->u4BufLen = u4BufEnd - u4BufStart + 1;

    PID_S_W(u1Idx, 5) = u4BufStart;     // FIFO start
    PID_S_W(u1Idx, 6) = u4BufEnd;       // FIFO end
    PID_S_W(u1Idx, 7) = u4Addr;         // PES_start
    PID_S_W(u1Idx, 8) = u4Addr;         // Write pointer
    PID_S_W(u1Idx, 9) = u4Addr;         // Read pointer
    _DMX_Unlock();

    return TRUE;
}



#ifdef CC_DMX_EMULATION
//-----------------------------------------------------------------------------
/** _DMX_EnableLocalArbitor
 */
//-----------------------------------------------------------------------------
BOOL _DMX_EnableLocalArbitor(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, BOOL fgEnable)
{
    UINT32 u4Reg;

    u4Reg = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_CTRL2);

    switch (eDevice)
    {
    case DMX_LOCAL_ARBITOR_DMX:
        if (fgEnable)
        {
            u4Reg |= 0x1;
        }
        else
        {
            u4Reg &= ~0x1;
        }
        break;

    case DMX_LOCAL_ARBITOR_DDI:
        if (fgEnable)
        {
            u4Reg |= 0x2;
        }
        else
        {
            u4Reg &= ~0x2;
        }
        break;

    case DMX_LOCAL_ARBITOR_PARSER:
        if (fgEnable)
        {
            u4Reg |= 0x4;
        }
        else
        {
            u4Reg &= ~0x4;
        }
        break;

    default:
        return FALSE;
    }

    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_DRAMMeasure
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DRAMMeasure(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, UINT32 u4TimeMs)
{
    UINT32 u4Reg, u4BandWidth, u4Cycles;

    if (eDevice >= DMX_LOCAL_ARBITOR_NUM)
    {
        return FALSE;
    }

    // Reset
    u4Reg = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_CTRL2) & 0xFF0FFFFF;
    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    u4Reg = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_CTRL2) & 0x0F0FFFFF;

    // Set device
    u4Reg |= ((eDevice & 0xF) << 28);

    // Enable
    u4Reg |= 0x100000;
    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    x_thread_delay(u4TimeMs);

    // Stop
    u4Reg |= 0x200000;
    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    // Read value
    DMXCMD_WRITE32(DMX_REG_REG_FILE_ADDR_REG, 0x3000);
    u4Cycles = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_MONITOR);
    DMXCMD_WRITE32(DMX_REG_REG_FILE_ADDR_REG, 0x2000);
    // bandwdith is in the unit of 8 bytes
    u4BandWidth = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_MONITOR);

    // Reset
    u4Reg = DMXCMD_READ32(DMX_REG_LOCAL_ARBITOR_CTRL2) & 0x0F0FFFFF;
    DMXCMD_WRITE32(DMX_REG_LOCAL_ARBITOR_CTRL2, u4Reg);

    LOG(1, "Bandwidth: 0x%x, cycles: 0x%x\n", u4BandWidth, u4Cycles);

    return TRUE;
}
#endif // CC_DMX_EMULATION


//-----------------------------------------------------------------------------
/** _DMX_GetPidInputType
 */
//-----------------------------------------------------------------------------
DMX_INPUT_TYPE_T _DMX_GetPidInputType(UINT8 u1Pidx)
{
    DMX_INPUT_TYPE_T eInputType;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_IN_NONE;
    }

    _DMX_Lock();
    eInputType = _arPidStruct[u1Pidx].eInputType;
    _DMX_Unlock();

    return eInputType;
}


//-----------------------------------------------------------------------------
/** _DMX_GetPidTsIndex
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_GetPidTsIndex(UINT8 u1Pidx)
{
    UINT8 u1TsIndex;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return 0xFF;
    }

    _DMX_Lock();
    u1TsIndex = _arPidStruct[u1Pidx].u1TsIndex;
    _DMX_Unlock();

    return u1TsIndex;
}


//-----------------------------------------------------------------------------
/** _DMX_GetIsTSEnabled
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetIsTSEnabled(UINT8 u1TsIdx)
{
    UINT8 i;

    _DMX_Lock();
    for (i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if ((_arPidStruct[i].u1TsIndex == u1TsIdx) &&
                _arPidStruct[i].fgEnable &&
                ((_arPidStruct[i].eInputType == DMX_IN_BROADCAST_TS) ||
                 (_arPidStruct[i].eInputType == DMX_IN_PLAYBACK_TS)))
        {
            _DMX_Unlock();
            return TRUE;
        }
    }
    _DMX_Unlock();

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetStcToDspUpdate
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetStcToDspUpdate(UINT8 u1Id, BOOL fgEnable)
{
#if defined(CC_MT5890) || defined(CC_MT5882)

    UINT32 u4Reg;

    if (u1Id >= DMX_NUM_STC_NUM)
    {
        return FALSE;
    }

    // disable
    u4Reg = DMXCMD_READ32(DMX_REG_PCR_NUM_CONTROL) & 0xFFFFFFFC;

    if (fgEnable)
    {
        u4Reg |= ((1 << 1) | (u1Id << 0));
    }

    DMXCMD_WRITE32(DMX_REG_PCR_NUM_CONTROL, u4Reg);
#endif
    return TRUE;
}

#ifdef CC_EIT_SECTION_FILTER
//-----------------------------------------------------------------------------
/** _DMX_Find_EIT_Hash_idx
 *  use this function to find hash index
 */
//-----------------------------------------------------------------------------
static UINT32  _DMX_Find_EIT_Hash_idx(CHANNEL_EIT_KEY_T *ptr_key, BOOL fgOnlyGetExist)
{
    UINT32 i4_i = 0;
    UINT32 ui4_code = 0;
    UINT8* pi1_key = (UINT8*)ptr_key;
	UINT32 ui4_key_idx = 0;
    UINT8 fgCheckHashIdx = TRUE;
	
    if(ptr_key == NULL)
    {
        LOG(2,"%s:Key is NULL.\r\n",__FUNCTION__);
        return CHANNEL_MAX_NUM;
    }

    /* FNV hash algorithm */
    for( i4_i = 0; i4_i < sizeof(CHANNEL_EIT_KEY_T); ++i4_i )
    {
        ui4_code ^= (UINT32)pi1_key[i4_i];

        ui4_code += ( (ui4_code<<1) + (ui4_code<<4) + (ui4_code<<7)
                      + (ui4_code<<8) + (ui4_code<<24) );
    }

    ui4_key_idx = ui4_code % CHANNEL_MAX_NUM;
    i4_i = ui4_key_idx;
    fgCheckHashIdx = TRUE;

    while(i4_i<CHANNEL_MAX_NUM)
    {  
        if(_arEITFilterStruct[i4_i].b_used == 0) //Break if meet a unused item
        {
            if(fgOnlyGetExist) //If only get exist item, a unused item indicate not found
            {   
                i4_i = CHANNEL_MAX_NUM;
            }
            break;
        }
        else
        { 
            //x_memcmp(ptr_key,&_arEITFilterStruct[i].t_channel,sizeof(CHANNEL_EIT_KEY_T))
            if((ptr_key->ui2_onid ==_arEITFilterStruct[i4_i].t_channel.ui2_onid)&&
            (ptr_key->ui2_tsid ==_arEITFilterStruct[i4_i].t_channel.ui2_tsid)&&
            (ptr_key->ui2_svcid ==_arEITFilterStruct[i4_i].t_channel.ui2_svcid))
            {// used and match ,return this idx , or i++.
                break;
            }
        }
        if(fgCheckHashIdx) //Not found at hash index, start search from 0
        {
            fgCheckHashIdx = FALSE;
            i4_i = 0; 
        }
        else
        {
            i4_i++;
        }
    }

    if(i4_i == CHANNEL_MAX_NUM)
    {
        LOG(7,"%s:cant find idx for this item.\r\n",__FUNCTION__);
    }

	//LOG(1,"svc_id=0x%02x,ts_id=0x%02x,onid=0x%02x,key_idx=%u.\r\n",ptr_key->ui2_svcid,ptr_key->ui2_tsid,ptr_key->ui2_onid,i4_i);
	return i4_i;
}

//-----------------------------------------------------------------------------
/** _DMX_Enable_EIT_FILTER
 *  provide this interface for to enable/disable eit filter(for debug).
 */
//-----------------------------------------------------------------------------
VOID _DMX_Enable_EIT_FILTER(BOOL fgEnable)
{
    fg_enable_eit_sw_filter =  fgEnable;
    return ;
}

VOID _DMX_ENABLE_EIT_Filter_Auto_Insert(BOOL fgEnable)
{
    fg_eit_filter_auto_insert = fgEnable;
    return;
}

VOID _DMX_Print_EIT_Filter(VOID) //For cli command debug print
{
    UINT32 i,j,k;
    for(i=0; i<CHANNEL_MAX_NUM; i++)
    {
        if(1 == _arEITFilterStruct[i].b_used)
        {
            Printf("Channel Idx%d(onid:%d,svcid:%d,tsid:%d):\n",i,\
                _arEITFilterStruct[i].t_channel.ui2_onid,_arEITFilterStruct[i].t_channel.ui2_svcid,_arEITFilterStruct[i].t_channel.ui2_tsid);
            for(j=0; j<EIT_TABLE_NUM; j++)
            {
                if(EIT_INVALID_TABLE_VERSION != _arEITFilterStruct[i].aui1_cur_version[j])
                {
                    Printf("**Table 0x%02x(Version %d) EIT Filters:\n",j+0x50,_arEITFilterStruct[i].aui1_cur_version[j]);
                    for(k=0; k<EIT_SECTION_NUM; k++)
                    {
                        Printf("0x%02x",_arEITFilterStruct[i].aui1_sec_mask[j*EIT_SECTION_NUM + k]);
                        if(k != (EIT_SECTION_NUM-1))
                        {
                            Printf("-");
                        }
                        else
                        {
                            Printf("\n");
                        }
                    }
                }
            }
        }
    }

    return;
}

EIT_FILTER_CHECK_RESULT _DMX_CheckEITFilter(SECTION_KEY_T *prSection, UINT32* pu4FltIndex)
{
    UINT32 u4FltIndex;
    CHANNEL_EIT_KEY_T tChannelKey;
    UINT8 u1TableId, u1SecNum, u1Version, u1SecMask, u1CurrentVersion;

    if(NULL == prSection)
    {
        return EIT_FILTER_INVALID;
    }
    u1TableId = prSection->ui1_tableid;
    u1SecNum = prSection->ui1_secnum;
    u1Version = prSection->ui1_version;
    tChannelKey = prSection->e_key;
    
    u4FltIndex = _DMX_Find_EIT_Hash_idx(&tChannelKey,TRUE);
    *pu4FltIndex = u4FltIndex;
    if((u4FltIndex >= CHANNEL_MAX_NUM) || (u1TableId > 0x6f) || (u1TableId < 0x50))
    {
        return EIT_FILTER_INDEX_NOT_FOUND;
    }

    u1CurrentVersion = _arEITFilterStruct[u4FltIndex].aui1_cur_version[EIT_FILTER_BY_TABLEID(u1TableId)];
    if(EIT_INVALID_TABLE_VERSION == u1CurrentVersion)
    {
        return EIT_FILTER_INDEX_NOT_FOUND;
    }
    u1SecMask =_arEITFilterStruct[u4FltIndex].aui1_sec_mask[EIT_SECTION_IDX(u1TableId,u1SecNum)];
    if(u1Version != u1CurrentVersion)
    {
        return EIT_FILTER_VERSION_NOT_SAME;
    }
    else if(EIT_GET_SECTION_MASK(u1SecNum,u1SecMask))
    {
        return EIT_FILTER_MATCH;
    }
    return EIT_FILTER_NOT_MATCH;
}

BOOL _DMX_EITFilter_VersionChange(UINT32 u4FltIndex, UINT8 u1TableId, UINT8 u1Version)
{
    UINT8 u1TableIndex;
    if((u4FltIndex >= CHANNEL_MAX_NUM) || (u1TableId > 0x6f) || (u1TableId < 0x50))
    {
        return FALSE;
    }
    u1TableIndex = EIT_FILTER_BY_TABLEID(u1TableId);
    LOG(5,"%s:Version change from %d to %d\n",__FUNCTION__,_arEITFilterStruct[u4FltIndex].aui1_cur_version[u1TableIndex],u1Version);
    _arEITFilterStruct[u4FltIndex].aui1_cur_version[u1TableIndex] = u1Version;
    x_memset(&(_arEITFilterStruct[u4FltIndex].aui1_sec_mask[u1TableIndex * EIT_SECTION_NUM]),0,EIT_SECTION_NUM);
    
    return TRUE;
}

BOOL _DMX_SetEITSection(SECTION_KEY_SET_T *prSetSection)
{
    UINT32 i,u4FltIndex;
    BOOL fgSet;
    SECTION_KEY_T tSecKey;
    UINT8 u1TableId,u1TableIndex,u1SecNum, u1Version;
    UINT16 u2SecIndex;
    
    if(NULL == prSetSection)
    {
        return FALSE;
    }
    fgSet = prSetSection->fgSet;
    tSecKey = prSetSection->tSecKey;
    u1TableId = tSecKey.ui1_tableid;
    u1SecNum = tSecKey.ui1_secnum;
    u1Version = tSecKey.ui1_version;
    
    switch(prSetSection->eSetType)
    {
        case EIT_SET_SECTION:
            {
                u4FltIndex = _DMX_Find_EIT_Hash_idx(&(tSecKey.e_key),!fgSet); //If clear(fgSet=FALSE), only get exist index
                if((u4FltIndex >= CHANNEL_MAX_NUM) || (u1TableId > 0x6f) || (u1TableId < 0x50))
                {
                    if(!fgSet)
                    {
                        return TRUE;
                    }
                    else
                    {
                        return FALSE;
                    }
                }
                u2SecIndex =  EIT_SECTION_IDX(u1TableId,u1SecNum);
                if(fgSet)
                {
                    if(0 == _arEITFilterStruct[u4FltIndex].b_used)
                    {
                        _arEITFilterStruct[u4FltIndex].b_used = 1;
                        _arEITFilterStruct[u4FltIndex].t_channel.ui2_onid = tSecKey.e_key.ui2_onid;
                        _arEITFilterStruct[u4FltIndex].t_channel.ui2_svcid = tSecKey.e_key.ui2_svcid;
                        _arEITFilterStruct[u4FltIndex].t_channel.ui2_tsid = tSecKey.e_key.ui2_tsid;
                    }
                    _arEITFilterStruct[u4FltIndex].aui1_sec_mask[u2SecIndex] |= (1 << (u1SecNum%8));
                    u1TableIndex = EIT_FILTER_BY_TABLEID(u1TableId);
                    _arEITFilterStruct[u4FltIndex].aui1_cur_version[u1TableIndex] = u1Version;
                }
                else
                {
                    _arEITFilterStruct[u4FltIndex].aui1_sec_mask[u2SecIndex] &= ~(1 << (u1SecNum%8));
                }
            }
            break;
        case EIT_SET_TABLE: //Clear table, set table is not implemented
            {
                u4FltIndex = _DMX_Find_EIT_Hash_idx(&(tSecKey.e_key),TRUE); //If clear, only get exist index
                if((u4FltIndex >= CHANNEL_MAX_NUM) || (u1TableId > 0x6f) || (u1TableId < 0x50))
                {
                    return TRUE;
                }
                u1TableIndex = EIT_FILTER_BY_TABLEID(u1TableId);
                _arEITFilterStruct[u4FltIndex].aui1_cur_version[u1TableIndex] = EIT_INVALID_TABLE_VERSION;
                x_memset(&(_arEITFilterStruct[u4FltIndex].aui1_sec_mask[u1TableIndex * EIT_SECTION_NUM]),0,EIT_SECTION_NUM);
            }
            break;
        case EIT_SET_CHANNEL: //Clear channel, set channel is not implemented
            {
                u4FltIndex = _DMX_Find_EIT_Hash_idx(&(tSecKey.e_key),TRUE); //If clear, only get exist index
                if(u4FltIndex >= CHANNEL_MAX_NUM)
                {
                    return TRUE;
                }
                _arEITFilterStruct[u4FltIndex].b_used = 0;
                x_memset(_arEITFilterStruct[u4FltIndex].aui1_cur_version,-1,EIT_TABLE_NUM); //Invalid table version is 0xff
                x_memset(_arEITFilterStruct[u4FltIndex].aui1_sec_mask,0,EIT_TABLE_NUM * EIT_SECTION_NUM);
            }
            break;
        case EIT_SET_ALL_CHANNEL: //Clear all, set all is not implemented
            {
                for(i=0; i<CHANNEL_MAX_NUM; i++)
                {
                    _arEITFilterStruct[i].b_used = 0;
                    x_memset(_arEITFilterStruct[i].aui1_cur_version,-1,EIT_TABLE_NUM);
                    x_memset(_arEITFilterStruct[i].aui1_sec_mask,0,EIT_TABLE_NUM * EIT_SECTION_NUM);
                }
            }
            break;
        default:
            return FALSE;
            break;
    }

    return TRUE;
}
#endif

//-----------------------------------------------------------------------------
/** _DMX_TraceLog
 *  Log a trace record for function call and function return.
 *
 *  @param  u4CallerAddr                The address of caller function
 *  @param  pszCaller                   The name of caller function
 *  @param  u4CalleeAddr                The address of callee function
 *  @param  pszCaller                   The name of callee function
 *  @param  u4Flags                     Function flags
 *            DMX_TRACE_USE_LOCK          Use lock before critical section
 *            DMX_TRACE_FUNCTION_CALL     Indicate a trace of function call
 *            DMX_TRACE_FUNCTION_RETURN   Indicate a trace of function return
 *
 *  @retval TRUE                        Succeed
 *  @retval FALSE                       Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TraceLog(UINT32 u4CallerAddr, CHAR *pszCaller,
                   UINT32 u4CalleeAddr, CHAR *pszCallee, UINT32 u4Flags)
{
    UINT32 i;
    DMX_TRACE_LOGFILE_T *prTrace;

    if ((0 == u4CallerAddr) || (0 == u4CalleeAddr) ||
            (NULL == pszCaller) || (NULL == pszCallee))
    {
        return FALSE;
    }

    if (u4Flags & DMX_TRACE_USE_LOCK)
    {
        _DMX_Lock();
    }

    for (i = 0; i < DMX_NUM_TRACES; i++)
    {
        if ((0 == _arDmxTraces[i].u4CallerAddr) ||
                (0 == _arDmxTraces[i].u4CalleeAddr))
            break;

        if ((u4CallerAddr == _arDmxTraces[i].u4CallerAddr) &&
                (u4CalleeAddr == _arDmxTraces[i].u4CalleeAddr))
            break;
    }

    if (i > DMX_NUM_TRACES)
    {
        if (u4Flags & DMX_TRACE_USE_LOCK)
        {
            _DMX_Unlock();
        }
        return FALSE;
    }

    prTrace = &_arDmxTraces[i];
    prTrace->u4CallerAddr = u4CallerAddr;
    prTrace->u4CalleeAddr = u4CalleeAddr;
    prTrace->pszCaller = pszCaller;
    prTrace->pszCallee = pszCallee;

    if (u4Flags & DMX_TRACE_FUNCTION_CALL)
    {
        prTrace->u4CallCount++;
    }
    else if (u4Flags & DMX_TRACE_FUNCTION_RETURN)
    {
        prTrace->u4ReturnCount++;
    }
    else
    {
        /* do nothing */
    }

    if (u4Flags & DMX_TRACE_USE_LOCK)
    {
        _DMX_Unlock();
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_GetTraceLogFiles
 *  Get a trace record by index
 *
 *  @param  u4CTracex               Trace index
 *  @param  prReturnTrace[output]   The returned pointer which points to the trace
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 *
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetTraceLogFiles(UINT32 u4Tracex, DMX_TRACE_LOGFILE_T *prReturnTrace)
{
    if ((u4Tracex >= DMX_NUM_TRACES) || (NULL == prReturnTrace))
    {
        return FALSE;
    }

    // Since interrupts are enabled during this procedure, a return count is a close estimate.
    *prReturnTrace =  _arDmxTraces[u4Tracex];

    return TRUE;
}

#ifdef CC_DMX_TRACK_ERROR_INTS
//-----------------------------------------------------------------------------
/** _DMX_RecordErrorInt
 *  Record error interrupts
 *  Note: This API could be called by ISR
 *
 *  @param  u1TsIndex               TS index
 *  @param  u2Pid                   PID
 *  @param  ePidType                PID type
 *  @param  u4ErrorInt              The error interrupt
 *  @param  u4ErrorStatus           The error status
 *
 *  @retval TRUE            Succeed
 *
 */
//-----------------------------------------------------------------------------
BOOL _DMX_RecordErrorInt(UINT8 u1TsIndex, UINT16 u2Pid, DMX_PID_TYPE_T ePidType, UINT32 u4ErrorInt, UINT32 u4ErrorStatus)
{
    UINT32 i;
    BOOL fgFound = FALSE;

    if (_u4DmxErrIdx >= ERR_RECORD_BUF_SIZE)
    {
        goto lbExit1;
    }

    if ((_u4DmxLastErrIdx < ERR_RECORD_BUF_SIZE) &&
        (_rDmxErrorRecords[_u4DmxLastErrIdx].u1TsIndex == u1TsIndex) &&
        (_rDmxErrorRecords[_u4DmxLastErrIdx].ePidType == ePidType) &&
        (_rDmxErrorRecords[_u4DmxLastErrIdx].u4ErrorInt == u4ErrorInt) &&
        (_rDmxErrorRecords[_u4DmxLastErrIdx].u4ErrorStatus == u4ErrorStatus))
    {
        _rDmxErrorRecords[_u4DmxLastErrIdx].u4ErrorCount++;
        goto lbExit1;
    }

    _u4DmxErrTime++;

    for (i = 0; i < _u4DmxErrIdx; i++)
    {
        if ((_rDmxErrorRecords[i].u1TsIndex == u1TsIndex) &&
            (_rDmxErrorRecords[i].ePidType == ePidType) &&
            (_rDmxErrorRecords[i].u4ErrorInt == u4ErrorInt) &&
            (_rDmxErrorRecords[i].u4ErrorStatus == u4ErrorStatus))
        {
            fgFound = TRUE;
            _rDmxErrorRecords[i].u4TimeStamp = _u4DmxErrTime;
            _rDmxErrorRecords[i].u4ErrorCount++;
            _u4DmxLastErrIdx = i;
            break;
        }
    }

    if (!fgFound)
    {
        _rDmxErrorRecords[_u4DmxErrIdx].u1TsIndex = u1TsIndex;
        _rDmxErrorRecords[_u4DmxErrIdx].ePidType = ePidType;
        _rDmxErrorRecords[_u4DmxErrIdx].u4ErrorInt = u4ErrorInt;
        _rDmxErrorRecords[_u4DmxErrIdx].u4ErrorStatus = u4ErrorStatus;
        _rDmxErrorRecords[_u4DmxErrIdx].u4TimeStamp = _u4DmxErrTime;
        _rDmxErrorRecords[_u4DmxErrIdx].u2Pid = u2Pid;
        _rDmxErrorRecords[_u4DmxErrIdx].u4ErrorCount++;

        _u4DmxLastErrIdx = _u4DmxErrIdx;
        _u4DmxErrIdx++;
    }

lbExit1:
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_DumpErrIntRecords
 *  Dump error interrupt records
 *
 *  @retval TRUE            Succeed
 *
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DumpErrIntRecords(void)
{
    UINT32 i;
    CHAR *pszIntType, *pszPidType;

    for (i = 0; i < _u4DmxErrIdx; i++)
    {
        switch (_rDmxErrorRecords[i].u4ErrorInt)
        {
        case DMX_INT_ERR_DBM:
            pszIntType = "DBM";
            break;
        case DMX_INT_ERR_FTuP:
            pszIntType = "FTuP";
            break;
        default:
            pszIntType = "UNKNOWN";
            break;
        }

        switch (_rDmxErrorRecords[i].ePidType)
        {
        case DMX_PID_TYPE_NONE:
            pszPidType = "NONE";
            break;
        case DMX_PID_TYPE_PSI:
            pszPidType = "PSI";
            break;
        case DMX_PID_TYPE_PES:
            pszPidType = "PES";
            break;
        case DMX_PID_TYPE_PES_TIME:
            pszPidType = "PES_TIME";
            break;
        case DMX_PID_TYPE_ES_VIDEO:
            pszPidType = "ES_VIDEO";
            break;
        case DMX_PID_TYPE_ES_AUDIO:
            pszPidType = "ES_AUDIO";
            break;
        case DMX_PID_TYPE_ES_VIDEOCLIP:
            pszPidType = "ES_VIDEOCLIP";
            break;
        case DMX_PID_TYPE_ES_OTHER:
            pszPidType = "ES_OTHER";
            break;
        case DMX_PID_TYPE_ES_DTCP:
            pszPidType = "ES_DTCP";
            break;
        case DMX_PID_TYPE_TS_RAW:
            pszPidType = "TS_RAW";
            break;
        default:
            pszPidType = "UNKNOWN";
            break;
        }

        LOG(0, "%s: (Idx=%u, Pid=0x%x, PidT=%s, Err=%u, Count=%u, TS=%u)\n",
            pszIntType, _rDmxErrorRecords[i].u1TsIndex,
            _rDmxErrorRecords[i].u2Pid, pszPidType,
            _rDmxErrorRecords[i].u4ErrorStatus,
            _rDmxErrorRecords[i].u4ErrorCount,
            _rDmxErrorRecords[i].u4TimeStamp);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_ResetErrIntRecords
 *  Reset error interrupt records
 *
 *  @retval TRUE            Succeed
 *
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ResetErrIntRecords(void)
{
    _DMX_Lock();
    x_memset((VOID *)_rDmxErrorRecords, 0, sizeof(_rDmxErrorRecords));
    _u4DmxErrTime = 0;
    _u4DmxErrIdx = 0;
    _u4DmxLastErrIdx = ERR_RECORD_BUF_SIZE;
    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_SaveDBMStatus
 *  Save the related registers to debug DBM.
 *  Note: This API is called by ISR
 *
 *  @param  u4ErrorStatus   The error status
 *
 *  @retval TRUE            Succeed
 *
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SaveDBMStatus(UINT32 u4ErrorStatus)
{
    UINT32 i;

    if (u4ErrorStatus & 0x1)
    {
        // DBM error due to internal buffer full
        for (i = 0; i < STATUS_REG_BUF_SIZE; i++)
        {
            _arDmxRegFtupFull[i] = DMXCMD_READ32(DMX_REG_FTuP_FULL_STATUS);
            _arDmxRegFtupPktFifo[i] = DMXCMD_READ32(DMX_REG_FTuP_TS_PACKET_FIFO_STATUS);
            _arDmxRegDbmStatus3[i] = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
        }

        for (i = 0; i < FTUP_PC_REG_BUF_SIZE; i++)
        {
            _arDmxRegFtupPC[i] = DMXCMD_READ32(DMX_REG_FTuP_PROGRAM_COUNTER);
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_DumpDBMStatus
 *  Dump the related registers to debug DBM
 *
 *  @retval TRUE            Succeed
 *
 */
//-----------------------------------------------------------------------------
BOOL _DMX_DumpDBMStatus(void)
{
    UINT32 i;
    UINT32 u4FtupFull, u4FtupPktFifo, u4DbmStatus3, u4FtupPC;

    // DBM error due to internal buffer full
    for (i = 0; i < STATUS_REG_BUF_SIZE; i++)
    {
        u4FtupFull = _arDmxRegFtupFull[i];
        u4FtupPktFifo = _arDmxRegFtupPktFifo[i];
        u4DbmStatus3 = _arDmxRegDbmStatus3[i];
        LOG(0, "Full=0x%08x Fifo=0x%08x DbmStat3=0x%08x\n", u4FtupFull, u4FtupPktFifo, u4DbmStatus3);
    }

    for (i = 0; i < FTUP_PC_REG_BUF_SIZE; i++)
    {
        u4FtupPC = _arDmxRegFtupPC[i];
        LOG(0, "PC = 0x%08x\n", u4FtupPC);
    }

    return TRUE;
}
#endif //CC_DMX_TRACK_ERROR_INTS
