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
 * $RCSfile: dmx_pvr_play.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_pvr_play.c
 *  dmx pvr driver - DMX PVR play function
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"

#if defined(CC_TRUSTZONE_SUPPORT)
#include "tz_if.h"
#endif
LINT_EXT_HEADER_BEGIN
#include "x_ckgen.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#define DMX_PVRPLAY_MEASURE_TIME
#define CC_DMX_SET_TIMESTAMP_RELOAD_THRESHOLD

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DMX_PVR_BUF_ALIGNMENT               16
#define DMX_PVR_POINTER_ALIGNMENT           4

#define DMX_PVRP_STOP_PKT_THRESHOLD         (20)

#define MPEG_TS_HEADER_SIZE                 (4)
#define PVR_TS_PACKET_SIZE                  (192)
#define PVR_PACKET_SIZE                     (188)

#define PVR_PROLOG_PACKETS                  (3)
#if 0
#define PVR_MAX_PACKET_SIZE                 (255)
#define PVR_PROLOG_BUF_SIZE                 (PVR_MAX_PACKET_SIZE * PVR_PROLOG_PACKETS + (2 * DMX_PVR_BUF_ALIGNMENT))
#else
#define PVR_MAX_PACKET_SIZE                 (208)   /* must be a multiple of DMX_PVR_BUF_ALIGNMENT and greater than PVR_TS_PACKET_SIZE */
#define PVR_PROLOG_BUF_SIZE                 (PVR_MAX_PACKET_SIZE * PVR_PROLOG_PACKETS)

#define MAX_SYNC_BYTE_OFFSET                (PVR_MAX_PACKET_SIZE - MPEG_TS_HEADER_SIZE)
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    BOOL fgValid;
    UINT16 u2PidNum;
} DMX_PVR_INFO_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define IS_KERN_LOGICAL_ADDRESS(addr) \
    (((UINT32)(addr) >= 0xBF000000) ? TRUE : FALSE)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static DMX_PVR_PLAY_T _arPVRPlay[DMX_MAX_TSOUT];

static HANDLE_T _hDMXPVRPlaySema[DMX_MAX_TSOUT] = {NULL_HANDLE};

static BOOL _fgTSOut[DMX_MAX_TSOUT] = {FALSE};

static DMX_PVRPLAY_COUNTER_T _rPVRPlayCounter[DMX_MAX_TSOUT];

static DMX_PVRPLAY_PORT_T _ePVRPort[DMX_MAX_TSOUT];

static DMX_TSFMT_T _eDmxPVRTSFmt[DMX_MAX_TSOUT];

static BOOL _fgDmxTimeshiftOpen[DMX_MAX_TSOUT] = {FALSE};

static UINT16 _au2PVRPlayFramerPktSize[DMX_MAX_TSOUT];


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_SetFramerPktSize
 */
//-----------------------------------------------------------------------------
static VOID _DmxPVRPlay_SetFramerMode(UINT8 u1TsOutIdx, BOOL fgExtSync, BOOL fgEnable)
{
    UINT32 u4Ctrl, u4CtrlNew;

    // Reset framer -
    if (u1TsOutIdx == 0)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
        u4CtrlNew = (u4Ctrl & 0xFFFFFFFE);

        if (fgExtSync)
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFD) | (0x1 << 1);
        }
        else
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFD);
        }

        if (fgEnable)
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFE) | 0x1 ;
        }
        else
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFE);
        }
        
        if (u4CtrlNew != u4Ctrl)
        {
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, (u4Ctrl & 0xFFFFFFFE));   // disable framer
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4CtrlNew);
        }

    }
    else if (u1TsOutIdx == 1)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1);
        u4CtrlNew = (u4Ctrl & 0xFFFFFFFE);

        if (fgExtSync)
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFD) | (0x1 << 1);
        }
        else
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFD);
        }

        if (fgEnable)
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFE) | 0x1 ;
        }
        else
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFE);
        }

        if (u4CtrlNew != u4Ctrl)
        {
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, (u4Ctrl & 0xFFFFFFFE));   // disable framer
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4CtrlNew);
        }
    }
    else if(u1TsOutIdx == 2)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1);
        u4CtrlNew = (u4Ctrl & 0xFFFFFFFE);

        if (fgExtSync)
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFD) | (0x1 << 1);
        }
        else
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFD);
        }

        if (fgEnable)
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFE) | 0x1 ;
        }
        else
        {
            u4CtrlNew = (u4CtrlNew & 0xFFFFFFFE);
        }

        if (u4CtrlNew != u4Ctrl)
        {
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, (u4Ctrl & 0xFFFFFFFE));   // disable framer
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4CtrlNew);
        }
    }
	else
    {
        ASSERT(0);
    }
}


//-----------------------------------------------------------------------------
/** _DMX_SetFramerPktSize
 */
//-----------------------------------------------------------------------------
static VOID _DmxPVRPlay_ResetFramer(UINT8 u1TsOutIdx)
{
    UINT32 u4Ctrl;
    UINT8 i;

    // Reset framer -
    if (u1TsOutIdx == 0)
    {
        // disable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1) & (~0x1);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Ctrl);

        // wait for framer state to idle
        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
            if (((u4Ctrl >> 16) & 0xFF) == 0x01)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1) & (~0x10);
        u4Ctrl |= 0x1 << 4;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Ctrl);
        //x_thread_delay(1);
        HAL_Delay_us(1);
        u4Ctrl &= ~(0x1 << 4);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Ctrl);

        // enable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1) | (0x1);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Ctrl);
    }
    else if (u1TsOutIdx == 1)
    {
        // disable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1) & (~0x1);
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Ctrl);

        // wait for framer state to idle
        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1);
            if (((u4Ctrl >> 16) & 0xFF) == 0x01)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1) & (~0x10);
        u4Ctrl |= 0x1 << 4;
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Ctrl);
        //x_thread_delay(1);
        HAL_Delay_us(1);
        u4Ctrl &= ~(0x1 << 4);
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Ctrl);

        // enable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1) | (0x1);
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Ctrl);
    }
	else if(u1TsOutIdx == 2)
	{
	           // disable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1) & (~0x1);
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4Ctrl);

        // wait for framer state to idle
        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1);
            if (((u4Ctrl >> 16) & 0xFF) == 0x01)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1) & (~0x10);
        u4Ctrl |= 0x1 << 4;
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4Ctrl);
        //x_thread_delay(1);
        HAL_Delay_us(1);
        u4Ctrl &= ~(0x1 << 4);
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4Ctrl);

        // enable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1) | (0x1);
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4Ctrl);
	  
	}
    else
    {
        ASSERT(0);
    }
}

//-----------------------------------------------------------------------------
/** _DmxPVRPlay_SetFramerPktSize
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPVRPlay_SetFramerPktSize(UINT8 u1TsOutIdx, UINT16 u2PktSize)
{
    UINT32 u4Reg;

    if (u1TsOutIdx >= DMX_MAX_TSOUT)
    {
        return FALSE;
    }

    if (_au2PVRPlayFramerPktSize[u1TsOutIdx] == u2PktSize)
    {
        return TRUE;
    }

    if (u1TsOutIdx == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL2) & ~0xFF000000;
        u4Reg |= ((UINT32)(u2PktSize & 0xFF) << 24);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL2, u4Reg);

        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
        u4Reg |= 0x1 << 2;        // enable pre-byte scheme
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Reg);
    }
    else if (u1TsOutIdx == 1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL2) & ~0xFF000000;
        u4Reg |= ((UINT32)(u2PktSize & 0xFF) << 24);
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL2, u4Reg);

        u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1);
        u4Reg |= 0x1 << 2;        // enable pre-byte scheme
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Reg);
    }
	else if(u1TsOutIdx == 2)
	{
	    u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL2) & ~0xFF000000;
        u4Reg |= ((UINT32)(u2PktSize & 0xFF) << 24);
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL2, u4Reg);

        u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1);
        u4Reg |= 0x1 << 2;        // enable pre-byte scheme
        DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL2, u4Reg);
	}
    else
    {
        ASSERT(0);
    }

    //Reset Framer
    _DmxPVRPlay_ResetFramer(u1TsOutIdx);

    _au2PVRPlayFramerPktSize[u1TsOutIdx] = u2PktSize;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxPVRPlay_UpdateAddr
 */
//-----------------------------------------------------------------------------
static UINT32 _DmxPVRPlay_UpdateAddr(UINT32 u4Addr, UINT32 u4Size, UINT32 u4BufStart,
                                     UINT32 u4BufEnd)
{
    UINT32 u4NewAddr;

    u4NewAddr = u4Addr + u4Size;
    if (u4NewAddr >= u4BufEnd)
    {
        u4NewAddr -= (u4BufEnd - u4BufStart);
    }

    ASSERT((u4BufStart<=u4NewAddr)&&(u4NewAddr<u4BufEnd));

    return u4NewAddr;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRSetupProlog
 *
 *  Set up three TS null packets for Framer's Internal Sync Lock.
 */
//-----------------------------------------------------------------------------
static BOOL _DMX_PVRSetupProlog(UINT8 u1TsOutIdx, UINT32 *pu4BufStart, UINT32 *pu4BufEnd)
{
    UINT32 u4SyncWord, i;

    ASSERT(pu4BufStart != NULL);
    ASSERT(pu4BufEnd   != NULL);
    ASSERT(_arPVRPlay[u1TsOutIdx].u1SyncOffset < MAX_SYNC_BYTE_OFFSET);

    // Setup two TS null packets for Internal Sync Lock at the stream head.
    // Null Packet (payload only): 0x471FFF10 + 0xFF...0xFF
    *pu4BufStart = (UINT32)_arPVRPlay[u1TsOutIdx].pu1Prolog;
    *pu4BufEnd   = (UINT32)_arPVRPlay[u1TsOutIdx].pu1Prolog + PVR_PROLOG_BUF_SIZE;

    if (_arPVRPlay[u1TsOutIdx].u1PrevSyncOffset != _arPVRPlay[u1TsOutIdx].u1SyncOffset)
    {
        x_memset((void*)_arPVRPlay[u1TsOutIdx].pu1Prolog, 0xFF, PVR_PROLOG_BUF_SIZE);

        u4SyncWord = 0x10FF1F47;
        for (i = 0; i < PVR_PROLOG_PACKETS; i++)
        {
            x_memcpy((void*)((*pu4BufStart) +
                             (i * _arPVRPlay[u1TsOutIdx].u1PacketSize) + _arPVRPlay[u1TsOutIdx].u1SyncOffset),
                     &u4SyncWord, sizeof(u4SyncWord));
        }
    #ifdef CC_DMX_FLUSH_ALL
        HalFlushInvalidateDCache();
    #else
        HalFlushInvalidateDCacheMultipleLine(*pu4BufStart, *pu4BufEnd - *pu4BufStart);
    #endif

    }
    _arPVRPlay[u1TsOutIdx].u1PrevSyncOffset = _arPVRPlay[u1TsOutIdx].u1SyncOffset;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SendNullPackets
 *
 *  Send three TS null packets for Framer's Internal Sync Lock.
 *  Note: current play buffer addresses (e.g. start, end, read-pointer, write-pointer)
 *  will be modified after invoking this function.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SendNullPackets(UINT8 u1TsOutIdx)
{
    BOOL fgRet = TRUE;
    DMX_MM_DATA_T rData;
    DMX_PVR_PLAY_T rOrigPlay, rPlay;
    UINT32 u4PrologBufStart = 0, u4PrologBufEnd = 0;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (_arPVRPlay[u1TsOutIdx].eState != DMX_PVRPLAY_STATE_STOP)
    {
        fgRet = FALSE;
        goto EXITA;
    }

    /* save original mode and timestamp setting */
    if (!_DMX_PVRPlay_Get(u1TsOutIdx, (UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rOrigPlay))
    {
        fgRet = FALSE;
        goto EXITA;
    }

    /* set new mode and timestamp */
    rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = TRUE;
    rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    if (!_DMX_PVRPlay_Set(u1TsOutIdx, (UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rPlay))
    {
        fgRet = FALSE;
        goto EXITB;
    }

    VERIFY(_DMX_PVRSetupProlog(u1TsOutIdx, &u4PrologBufStart, &u4PrologBufEnd));

    x_memset((void*)&rData, 0, sizeof(rData));
    rData.u4BufStart = u4PrologBufStart;
    rData.u4BufEnd = u4PrologBufEnd;
    rData.u4StartAddr = u4PrologBufStart;
    rData.u4FrameSize = _arPVRPlay[u1TsOutIdx].u4PrologSize;

    if (!_DMX_PVRPlay_SingleMove(u1TsOutIdx, &rData))
    {
        fgRet = FALSE;
    }

EXITB:
    /* restore original move and timestamp */
    if (!_DMX_PVRPlay_Set(u1TsOutIdx, (UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rOrigPlay))
    {
        fgRet = FALSE;
    }

EXITA:
    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DmxPVRPlay_DoStop
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPVRPlay_DoStop(UINT8 u1TsOutIdx, DMX_PVRPLAY_PTR_T *prPtr)
{
    UINT32 u4Wp, u4Rp, u4BufStart, u4BufSize, u4BufEnd, u4DataSize;
    UINT32 u4NewWp, u4NewRp, u4Align;
    UINT32 i, u4Reg;
    DMX_PVR_PLAY_T* prPvrPlayStruct;

    u4Wp = 0;
    u4Rp = 0;
    u4NewRp = 0;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    _DMX_Lock();
    prPvrPlayStruct = &_arPVRPlay[u1TsOutIdx];
    _DMX_Unlock();

    if (prPvrPlayStruct->eState != DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "PVR play already stop!\n");
        return TRUE;
    }

    if (u1TsOutIdx == 0)
    {
        if (prPvrPlayStruct->fgIgnoreTimeStamp)
        {
            // Disable PVR play
            _DMX_Lock();
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
            DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0);
            _DMX_Unlock();
        }
        else
        {
            _DMX_Lock();
            u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
            // Set DBM output spacing
            u4Reg = (u4Reg & 0xff00ffff) | (1 << 16);
            // Set DBM max playback TS packets
            u4Reg = (u4Reg & 0xffff00ff) | (3 << 8);
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
            _DMX_Unlock();

            //--------------------------------------------------
            // Special case that this operation must be atomic
            //--------------------------------------------------
            _DMX_Lock();
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
            u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
            u4BufStart = prPvrPlayStruct->u4BufStart;
            u4BufEnd = prPvrPlayStruct->u4BufEnd;
            u4BufSize = prPvrPlayStruct->u4BufSize;

            u4Wp = VIRTUAL(u4Wp);
            u4Rp = VIRTUAL(u4Rp);

            if (u4Wp != u4Rp)
            {
                u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);
                u4NewWp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
                u4NewWp = VIRTUAL(u4NewWp);
                if (u4DataSize > (DMX_PVRP_STOP_PKT_THRESHOLD * PVR_TS_PACKET_SIZE))
                {
                    u4Align = (u4Rp - u4BufStart) % PVR_TS_PACKET_SIZE;
                    // Make sure u4NewRp is 192 bytes alignment
                    u4NewRp = _DmxPVRPlay_UpdateAddr(u4Rp, (PVR_TS_PACKET_SIZE - u4Align),
                                                     u4BufStart, u4BufEnd);
                    u4NewWp = _DmxPVRPlay_UpdateAddr(u4NewRp,
                                                     (DMX_PVRP_STOP_PKT_THRESHOLD * PVR_TS_PACKET_SIZE),
                                                     u4BufStart, u4BufEnd);
                    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, PHYSICAL(u4NewWp));
                }
                _DMX_Unlock();

                i = 0;
                while (1)
                {
                    u4Rp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP));
                    if (u4Rp == u4NewWp)
                    {
                        break;
                    }
                    x_thread_delay(1);
                    if (i++ > 50)
                    {
                        LOG(1,"%s timeout\n", __FUNCTION__);
                        break;
                    }
                }
            }
            else
            {
                _DMX_Unlock();
            }

            // Disable PVR play
            DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0);

            _DMX_Lock();
            u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
            // Set DBM output spacing
            u4Reg = (u4Reg & 0xff00ffff) | (8 << 16);
            // Set DBM max playback TS packets
            u4Reg = (u4Reg & 0xffff00ff) | (1 << 8);
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
            _DMX_Unlock();

            DMXLOG(DMX_LOG_PVRPLAY 6, "dostop, u4Rp:0x%x, u4NewRp:0x%x, u4Wp:0x%x\n", u4Rp, u4NewRp, u4Wp);

        }
    }
    else if(u1TsOutIdx == 1)
    {
        if (prPvrPlayStruct->fgIgnoreTimeStamp)
        {
            // Disable PVR play
            _DMX_Lock();
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP2);
            DMXCMD_WRITE32(DMX_REG_PVR_CONTROL2, 0);
            _DMX_Unlock();
        }
        else
        {
            _DMX_Lock();
            u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
            // Set DBM output spacing
            u4Reg = (u4Reg & 0xff00ffff) | (1 << 16);
            // Set DBM max playback TS packets
            u4Reg = (u4Reg & 0xffff00ff) | (3 << 8);
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
            _DMX_Unlock();

            //--------------------------------------------------
            // Special case that this operation must be atomic
            //--------------------------------------------------
            _DMX_Lock();
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP2);
            u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP2);
            u4BufStart = prPvrPlayStruct->u4BufStart;
            u4BufEnd = prPvrPlayStruct->u4BufEnd;
            u4BufSize = prPvrPlayStruct->u4BufSize;

            u4Wp = VIRTUAL(u4Wp);
            u4Rp = VIRTUAL(u4Rp);

            if (u4Wp != u4Rp)
            {
                u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);
                u4NewWp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP2);
                u4NewWp = VIRTUAL(u4NewWp);
                if (u4DataSize > (DMX_PVRP_STOP_PKT_THRESHOLD * PVR_TS_PACKET_SIZE))
                {
                    u4Align = (u4Rp - u4BufStart) % PVR_TS_PACKET_SIZE;
                    // Make sure u4NewRp is 192 bytes alignment
                    u4NewRp = _DmxPVRPlay_UpdateAddr(u4Rp, (PVR_TS_PACKET_SIZE - u4Align),
                                                     u4BufStart, u4BufEnd);
                    u4NewWp = _DmxPVRPlay_UpdateAddr(u4NewRp,
                                                     (DMX_PVRP_STOP_PKT_THRESHOLD * PVR_TS_PACKET_SIZE),
                                                     u4BufStart, u4BufEnd);
                    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP2, PHYSICAL(u4NewWp));
                }
                _DMX_Unlock();

                i = 0;
                while (1)
                {
                    u4Rp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP2));
                    if (u4Rp == u4NewWp)
                    {
                        break;
                    }
                    x_thread_delay(1);
                    if (i++ > 50)
                    {
                        LOG(1,"%s timeout\n", __FUNCTION__);
                        break;
                    }
                }
            }
            else
            {
                _DMX_Unlock();
            }

            // Disable PVR play
            DMXCMD_WRITE32(DMX_REG_PVR_CONTROL2, 0);

            _DMX_Lock();
            u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
            // Set DBM output spacing
            u4Reg = (u4Reg & 0xff00ffff) | (8 << 16);
            // Set DBM max playback TS packets
            u4Reg = (u4Reg & 0xffff00ff) | (1 << 8);
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
            _DMX_Unlock();

            DMXLOG(DMX_LOG_PVRPLAY 6, "dostop, u4Rp:0x%x, u4NewRp:0x%x, u4Wp:0x%x\n", u4Rp, u4NewRp, u4Wp);

        }
    }
	else if(u1TsOutIdx == 2)
	{
        if (prPvrPlayStruct->fgIgnoreTimeStamp)
        {
            // Disable PVR play
            _DMX_Lock();
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP3);                
            DMXCMD_WRITE32(DMX_REG_PVR_CONTROL3, 0);
            _DMX_Unlock();
        }
        else
        {
            _DMX_Lock();
            u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
            // Set DBM output spacing
            u4Reg = (u4Reg & 0xff00ffff) | (1 << 16);
            // Set DBM max playback TS packets
            u4Reg = (u4Reg & 0xffff00ff) | (3 << 8);
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
            _DMX_Unlock();

            //--------------------------------------------------
            // Special case that this operation must be atomic
            //--------------------------------------------------
            _DMX_Lock();
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP3);
            u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP3);
            u4BufStart = prPvrPlayStruct->u4BufStart;
            u4BufEnd = prPvrPlayStruct->u4BufEnd;
            u4BufSize = prPvrPlayStruct->u4BufSize;

            u4Wp = VIRTUAL(u4Wp);
            u4Rp = VIRTUAL(u4Rp);

            if (u4Wp != u4Rp)
            {
                u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufSize);
                u4NewWp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP3);
                u4NewWp = VIRTUAL(u4NewWp);
                if (u4DataSize > (DMX_PVRP_STOP_PKT_THRESHOLD * PVR_TS_PACKET_SIZE))
                {
                    u4Align = (u4Rp - u4BufStart) % PVR_TS_PACKET_SIZE;
                    // Make sure u4NewRp is 192 bytes alignment
                    u4NewRp = _DmxPVRPlay_UpdateAddr(u4Rp, (PVR_TS_PACKET_SIZE - u4Align),
                                                     u4BufStart, u4BufEnd);
                    u4NewWp = _DmxPVRPlay_UpdateAddr(u4NewRp,
                                                     (DMX_PVRP_STOP_PKT_THRESHOLD * PVR_TS_PACKET_SIZE),
                                                     u4BufStart, u4BufEnd);
                    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP3, PHYSICAL(u4NewWp));
                }
                _DMX_Unlock();

                i = 0;
                while (1)
                {
                    u4Rp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP3));
                    if (u4Rp == u4NewWp)
                    {
                        break;
                    }
                    x_thread_delay(1);
                    if (i++ > 50)
                    {
                        LOG(1,"%s timeout\n", __FUNCTION__);
                        break;
                    }
                }
            }
            else
            {
                _DMX_Unlock();
            }

            // Disable PVR play
            DMXCMD_WRITE32(DMX_REG_PVR_CONTROL3, 0);

            _DMX_Lock();
            u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
            // Set DBM output spacing
            u4Reg = (u4Reg & 0xff00ffff) | (8 << 16);
            // Set DBM max playback TS packets
            u4Reg = (u4Reg & 0xffff00ff) | (1 << 8);
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
            _DMX_Unlock();

            DMXLOG(DMX_LOG_PVRPLAY 6, "dostop, u4Rp:0x%x, u4NewRp:0x%x, u4Wp:0x%x\n", u4Rp, u4NewRp, u4Wp);

        }
    }
    if (prPtr != NULL)
    {
        _DMX_Lock();
        if (u1TsOutIdx == 0)
        {
            u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
        }
        else if(u1TsOutIdx == 1)
        {
            u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP2);
        }
		else if(u1TsOutIdx == 2)
		{
		   u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP3);
    	}
        prPtr->u4Rp = VIRTUAL(u4Rp);
        prPtr->u4Wp = VIRTUAL(u4Wp);
        _DMX_Unlock();
    }

    prPvrPlayStruct->eState = DMX_PVRPLAY_STATE_STOP;

    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DmxPVRPlay_SendEOS
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPVRPlay_SendEOS(UINT8 u1TsIdx)
{
    PID_STRUCT_T *prPidStruct;
    UINT8 u1Pidx;

    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if (_DMX_IsPidEnabled(u1Pidx))
        {
            prPidStruct = _DMX_GetPidStruct(u1Pidx);
            if((prPidStruct->eInputType != DMX_IN_BROADCAST_TS) &&
               (prPidStruct->eInputType != DMX_IN_PLAYBACK_TS))
            {
                continue;
            }

            if(_DMX_GetPidTsIndex(u1Pidx) != u1TsIdx)
            {
                continue;
            }

            if(prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
            {
                if (!_DMX_SendEOSMessage(u1Pidx))
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}
//-----------------------------------------------------------------------------
/** _DmxPVRPlay_SyncVideoWp
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPVRPlay_SyncVideoWp(UINT8 u1TsIdx)
{
    PID_STRUCT_T *prPidStruct;
    UINT8 u1Pidx;
    UINT32 u4Wp1, u4Wp2, i;
    UINT8 u1Channel;

    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if (_DMX_IsPidEnabled(u1Pidx))
        {
            prPidStruct = _DMX_GetPidStruct(u1Pidx);
            if((prPidStruct->eInputType != DMX_IN_BROADCAST_TS) &&
               (prPidStruct->eInputType != DMX_IN_PLAYBACK_TS))
            {
                continue;
            }

            if(_DMX_GetPidTsIndex(u1Pidx) != u1TsIdx)
            {
                continue;
            }

            if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
            {
                for(i=0; i<100; i++)
                {
                    u4Wp1 = PID_S_W(u1Pidx, 8);
                    x_thread_delay(1);
                    u4Wp2 = PID_S_W(u1Pidx, 8);
                    if(u4Wp1 == u4Wp2)
                    {
                        break;
                    }
                }

                if(i == 100)
                {
                    LOG(3, "Out of sync WP\n");
                    return TRUE;
                }

                // Update write pointer to decoder
                if (_DMX_IsToDecoder())
                {
                    VERIFY(_DMX_UpdateWritePointer(u1Pidx, PHYSICAL(u4Wp1)));
                    VERIFY(_DMX_GetVideoChannel(u1Pidx, &u1Channel));
                    VERIFY(_DMX_UpdateVideoWritePointer(u1Pidx, u1Channel, PHYSICAL(u4Wp1)));
                }
            }
        }
    }

    return TRUE;

}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Free
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Free(UINT8 u1TsOutIdx)
{
    if (!_DMX_PVRPlay_Stop(u1TsOutIdx,TRUE))
    {
        return FALSE;
    }

    if (!_DMX_PVRPlay_FreeBuf(u1TsOutIdx))
    {
        return FALSE;
    }

    _DMX_Lock();

    x_memset((void*)&_arPVRPlay[u1TsOutIdx], 0x0, sizeof(DMX_PVR_PLAY_T));
    _arPVRPlay[u1TsOutIdx].fgContainTimeStamp = FALSE;
    _arPVRPlay[u1TsOutIdx].fgIgnoreTimeStamp = TRUE;
    _arPVRPlay[u1TsOutIdx].eState = DMX_PVRPLAY_STATE_STOP;
    _arPVRPlay[u1TsOutIdx].eMode = DMX_PVR_PLAY_STREAM;

    _fgTSOut[u1TsOutIdx] = FALSE;

    if (u1TsOutIdx == 0)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0x0);
    }
    else if(u1TsOutIdx == 1)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL2, 0x0);
    }
	else if(u1TsOutIdx == 2)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL3, 0x0);
    }

    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Init(void)
{
    static BOOL _fgInit = FALSE;
    UINT8 *pu1Prolog[DMX_MAX_TSOUT];
    UINT8 i;

    // When error reset, do not init following variables
    if(!_fgInit)//(!_DMX_GetIsPanicReset())
    {
        _DMX_Lock();
        for (i = 0; i < DMX_MAX_TSOUT; i++)
        {
            _fgDmxTimeshiftOpen[i] = FALSE;
            x_memset((void*)&_arPVRPlay[i], 0x0, sizeof(DMX_PVR_PLAY_T));
            _arPVRPlay[i].fgAllocBuf = FALSE;
            _arPVRPlay[i].fgContainTimeStamp = FALSE;
            _arPVRPlay[i].fgIgnoreTimeStamp = TRUE;
            _arPVRPlay[i].eState = DMX_PVRPLAY_STATE_STOP;
            _arPVRPlay[i].eMode = DMX_PVR_PLAY_STREAM;
            _arPVRPlay[i].u1PrevSyncOffset = PVR_MAX_PACKET_SIZE;
            _fgTSOut[i] = FALSE;
            x_memset((void*)&_rPVRPlayCounter[i], 0, sizeof(DMX_PVRPLAY_COUNTER_T));
        }
        _DMX_Unlock();
    }

    for (i = 0; i < DMX_MAX_TSOUT; i++)
    {
        _DMX_PVRPlay_Reset(i);
    }

    //clear PVR Control Reg,enable  pvr Timestamp clock to 27MHZ=(432/16)
    _DMX_Lock();
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL2, DMX_PVRPLAY_TIMESTAMP_DIV_BASE << 16);
    DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, DMX_PVRPLAY_TIMESTAMP_DIV_BASE << 16);
	DMXCMD_WRITE32(DMX_REG_PVR_CONTROL3, DMX_PVRPLAY_TIMESTAMP_DIV_BASE << 16);
    _DMX_Unlock();

    //Set PVRPaly port
    _DMX_PVRPlay_SetPort(0,DMX_PVRPLAY_PORT_FRAMER2_BYPASS);
    _DMX_PVRPlay_SetPort(1,DMX_PVRPLAY_PORT_FRAMER3_BYPASS);
    
#if defined(CC_MT5890)
	_DMX_PVRPlay_SetPort(2,DMX_PVRPLAY_PORT_DBM); //set ts out .
#endif

    for (i = 0; i < DMX_MAX_TSOUT; i++)
    {
        //Set Framer packet Size
        if (!_DmxPVRPlay_SetFramerPktSize(i, 188))
        {
            return FALSE;
        }

#ifndef CC_DMX_EMULATION
        _DmxPVRPlay_SetFramerMode(i, FALSE, TRUE);
#else
        _DmxPVRPlay_SetFramerMode(i, TRUE, TRUE);
#endif

        if (!_DMX_PVRPlay_SetMoveMode(i, DMX_TSFMT_188, TRUE))
        {
            return FALSE;
        }
    }

    if (!_fgInit)
    {
        for (i = 0; i < DMX_MAX_TSOUT; i++)
        {
            VERIFY(x_sema_create(&_hDMXPVRPlaySema[i], X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_LOCK) == OSR_OK);

            pu1Prolog[i] = (UINT8*)BSP_AllocAlignedDmaMemory(PVR_PROLOG_BUF_SIZE,
                        DMX_PVR_BUF_ALIGNMENT);
            pu1Prolog[i] = (UINT8*)VIRTUAL((UINT32)pu1Prolog[i]);

            if (pu1Prolog[i] == NULL)
            {
                return FALSE;
            }
            _arPVRPlay[i].pu1Prolog = pu1Prolog[i];
        }
        _fgInit = TRUE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Start
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Start(UINT8 u1TsOutIdx)
{
    UINT32 u4RegControl;
    DMX_PVR_PLAY_T* prPvrPlayStruct;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    _DMX_Lock();
    prPvrPlayStruct = &_arPVRPlay[u1TsOutIdx];
    _DMX_Unlock();

    if (prPvrPlayStruct->eState != DMX_PVRPLAY_STATE_STOP)
    {
        LOG(3, "PVR play already enable!\n");
        return FALSE;
    }

    if (prPvrPlayStruct->u4BufSize == 0)
    {
        LOG(1, "No buffer allocated!\n");
        return FALSE;
    }

    ///////// Set framer ///////////

    if (u1TsOutIdx == 0)
    {
        //Set TimeStamp Threshold
#ifdef CC_DMX_EMULATION
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD, 0x100000);
#else
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD, 0xFFFFFFFF);
#endif

        //Buffer Threshold
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD, prPvrPlayStruct->u4ThresholdSize);

        //Output Steering Logic
        u4RegControl = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF;
        if (_fgTSOut[0])
        {
            u4RegControl |= (1 << 16);
        }
        else
        {
            u4RegControl |= (1 << 17);
        }

        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4RegControl);

        //Mini PVR Playback config
        u4RegControl = 0x0;
        u4RegControl |= (((UINT32)prPvrPlayStruct->u2TimeStampFreqDiv) << 16) |
                        (1 << 14) |                                     // Enable interrupt
                        (1 << 6) |                                      // Enable bit
                        ((prPvrPlayStruct->fgIgnoreTimeStamp ? 1: 0) << 3) |   // Ignore timestamp
                        ((prPvrPlayStruct->fgContainTimeStamp ? 1 : 0) << 0);  // Contain timestamp

        u4RegControl |= 0x200;          // Reset timestamp
        u4RegControl |= 0x20;
#ifdef CC_DMX_SET_TIMESTAMP_RELOAD_THRESHOLD
        u4RegControl |= 0x10;
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD, 0x5265C0);  // 0.2 second
#endif
       //set record timestamp:27MHZ:
        u4RegControl |= (DMX_PVRPLAY_TIMESTAMP_DIV_BASE << 16);

        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, u4RegControl);
    }
    else if(u1TsOutIdx == 1)
    {
        //Set TimeStamp Threshold
#ifdef CC_DMX_EMULATION
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD2, 0x100000);
#else
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD2, 0xFFFFFFFF);
#endif

        //Buffer Threshold
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD2, prPvrPlayStruct->u4ThresholdSize);

        //Output Steering Logic
        u4RegControl = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL) & 0xFFFCFFFF;
        if (_fgTSOut[1])
        {
            u4RegControl |= (1 << 16);
        }
        else
        {
            u4RegControl |= (1 << 17);
        }
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4RegControl);

        //Mini PVR Playback config
        u4RegControl = 0x0;
        u4RegControl |= (((UINT32)prPvrPlayStruct->u2TimeStampFreqDiv) << 16) |
                        (1 << 14) |                                     // Enable interrupt
                        (1 << 6) |                                      // Enable bit
                        ((prPvrPlayStruct->fgIgnoreTimeStamp ? 1: 0) << 3) |   // Ignore timestamp
                        ((prPvrPlayStruct->fgContainTimeStamp ? 1 : 0) << 0);  // Contain timestamp

        u4RegControl |= 0x200;          // Reset timestamp
        u4RegControl |= 0x20;
#ifdef CC_DMX_SET_TIMESTAMP_RELOAD_THRESHOLD
        u4RegControl |= 0x10;
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD2, 0x5265C0);  // 0.2 second
#endif
        //set record timestamp:27MHZ:
        u4RegControl |= (DMX_PVRPLAY_TIMESTAMP_DIV_BASE << 16);

        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL2, u4RegControl);
    }
#if defined(CC_MT5890)
	else if(u1TsOutIdx == 2)
	{
        //Set TimeStamp Threshold
#ifdef CC_DMX_EMULATION
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD3, 0x100000);
#else
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD3, 0xFFFFFFFF);
#endif

        //Buffer Threshold
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD3, prPvrPlayStruct->u4ThresholdSize);

        //Output Steering Logic
        u4RegControl = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL) & 0xFFFCFFFF;
        if (_fgTSOut[2])
        {
            u4RegControl |= (1 << 16);
        }
        else
        {
            u4RegControl |= (1 << 17);
        }
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4RegControl);

        //Mini PVR Playback config
        u4RegControl = 0x0;
        u4RegControl |= (((UINT32)prPvrPlayStruct->u2TimeStampFreqDiv) << 16) |
                        (1 << 14) |                                     // Enable interrupt
                        (1 << 6) |                                      // Enable bit
                        ((prPvrPlayStruct->fgIgnoreTimeStamp ? 1: 0) << 3) |   // Ignore timestamp
                        ((prPvrPlayStruct->fgContainTimeStamp ? 1 : 0) << 0);  // Contain timestamp

        u4RegControl |= 0x200;          // Reset timestamp
        u4RegControl |= 0x20;
#ifdef CC_DMX_SET_TIMESTAMP_RELOAD_THRESHOLD
        u4RegControl |= 0x10;
        DMXCMD_WRITE32(DMX_REG_PVR_TIMESTAMP_THRESHOLD3, 0x5265C0);  // 0.2 second
#endif
        //set record timestamp:27MHZ:
        u4RegControl |= (DMX_PVRPLAY_TIMESTAMP_DIV_BASE << 16);

        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL3, u4RegControl);
    }
#endif
    prPvrPlayStruct->eState = DMX_PVRPLAY_STATE_PLAYING;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Stop
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Stop(UINT8 u1TsOutIdx, BOOL fgForce)
{
    //LOG(3, "%s, force:%d\n", __FUNCTION__, (INT32)fgForce);

    if (!_DmxPVRPlay_DoStop(u1TsOutIdx,NULL))
    {
        return FALSE;
    }

    if (fgForce)
    {
        _DMX_PVRPlay_Reset(u1TsOutIdx);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Pause
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Pause(UINT8 u1TsOutIdx, DMX_PVRPLAY_PTR_T *prPtr)
{
    if (!_DmxPVRPlay_DoStop(u1TsOutIdx, prPtr))
    {
        return FALSE;
    }

    _DMX_PVRPlay_Reset(u1TsOutIdx);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Resume
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Resume(UINT8 u1TsOutIdx, DMX_PVRPLAY_PTR_T *prPtr)
{

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (prPtr == NULL)
    {
        return FALSE;
    }

    if (!_DMX_PVRPlay_SetBuffer(u1TsOutIdx, _arPVRPlay[u1TsOutIdx].u4BufStart, _arPVRPlay[u1TsOutIdx].u4BufEnd - 1,
                                prPtr->u4Wp, prPtr->u4Rp))
    {
        return FALSE;
    }

    if (!_DMX_PVRPlay_Start(u1TsOutIdx))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Reset
 */
//-----------------------------------------------------------------------------
VOID _DMX_PVRPlay_Reset(UINT8 u1TsOutIdx)
{
    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (_arPVRPlay[u1TsOutIdx].eState == DMX_PVRPLAY_STATE_PLAYING)
    {
        ASSERT(0);
    }

    if (u1TsOutIdx == 0)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0x100);
        HAL_Delay_us(1);
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL, 0x0);
    }
    else if (u1TsOutIdx == 1)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL2, 0x100);
        HAL_Delay_us(1);
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL2, 0x0);
    }
    else 
    {
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL3, 0x100);
        HAL_Delay_us(1);
        DMXCMD_WRITE32(DMX_REG_PVR_CONTROL3, 0x0);
    }
    return;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetTimeStamp
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_GetTimeStamp(UINT8 u1TsOutIdx, UINT32 *pu4RetLocalStamp)
{
    BOOL fgRet = TRUE;

    if (!pu4RetLocalStamp)
    {
        fgRet = FALSE;
        goto lbExit1;
    }

    if (u1TsOutIdx == 0)
    {
        *pu4RetLocalStamp = DMXCMD_READ32(DMX_REG_PVR_LOCAL_TIMESTAMP);
    }
    else if (u1TsOutIdx == 1)
    {
        *pu4RetLocalStamp = DMXCMD_READ32(DMX_REG_PVR_LOCAL_TIMESTAMP2);
    }
    else if (u1TsOutIdx == 2)
    {
        *pu4RetLocalStamp = DMXCMD_READ32(DMX_REG_PVR_LOCAL_TIMESTAMP3);
    }
    else
    {
        fgRet = FALSE;
    }

lbExit1:
    
    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetState
 */
//-----------------------------------------------------------------------------
DMX_PVR_PLAYSTATE_T _DMX_PVRPlay_GetState(UINT8 u1TsOutIdx)
{
    return _arPVRPlay[u1TsOutIdx].eState;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Set
    Push or pull mode
    Timestamp
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Set(UINT8 u1TsoutIdx, UINT32 u4Flags, const DMX_PVR_PLAY_T* prPVRPlay)
{
    UINT32 u4BufStart, u4BufEnd;
    UINT32 u4VirBufStart, u4PhyBufStart, u4PhyBufEnd;
    DMX_PVR_PLAY_T rPvrPlayStruct;

    ASSERT(prPVRPlay != NULL);

    if (u1TsoutIdx >= DMX_MAX_TSOUT)
    {
        LOG(3, "Invalid Ts Out Index: %u\n", u1TsoutIdx);
        return FALSE;
    }

    if (u4Flags == PVRPLAY_FLAGS_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();
    rPvrPlayStruct = _arPVRPlay[u1TsoutIdx];
    _DMX_Unlock();


    if (rPvrPlayStruct.eState == DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "Now playing, stop first!\n");
        return FALSE;
    }


    _DMX_Lock();

    if (u4Flags & PVRPLAY_FLAGS_TIMESTAMP)
    {
        rPvrPlayStruct.fgContainTimeStamp = prPVRPlay->fgContainTimeStamp;
        rPvrPlayStruct.fgIgnoreTimeStamp = prPVRPlay->fgIgnoreTimeStamp;
        rPvrPlayStruct.u2TimeStampFreqDiv = prPVRPlay->u2TimeStampFreqDiv;

        _rPVRPlayCounter[u1TsoutIdx].fgUseTimestamp = !prPVRPlay->fgIgnoreTimeStamp;

        if (rPvrPlayStruct.fgContainTimeStamp)
        {
            rPvrPlayStruct.u1PacketSize = PVR_TS_PACKET_SIZE;
            rPvrPlayStruct.u1SyncOffset = 4;
            rPvrPlayStruct.u4PrologSize = PVR_TS_PACKET_SIZE * PVR_PROLOG_PACKETS;
        }
        else
        {
            rPvrPlayStruct.u1PacketSize = PVR_PACKET_SIZE;
            rPvrPlayStruct.u1SyncOffset = 0;
            rPvrPlayStruct.u4PrologSize = PVR_PACKET_SIZE * PVR_PROLOG_PACKETS;
        }
    }

    if (u4Flags & PVRPLAY_FLAGS_CALLBACK)
    {
        rPvrPlayStruct.pfnPVRNotify = prPVRPlay->pfnPVRNotify;
    }

    if (u4Flags & PVRPLAY_FLAGS_THRESHOLD)
    {
        rPvrPlayStruct.u4ThresholdSize = prPVRPlay->u4ThresholdSize;
        if (u1TsoutIdx == 0)
        {
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD, rPvrPlayStruct.u4ThresholdSize);
        }
        else if (u1TsoutIdx == 1)
        {
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD2, rPvrPlayStruct.u4ThresholdSize);
        }
		else
		{
		   DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD3, rPvrPlayStruct.u4ThresholdSize);
		}
    }

    if (u4Flags & PVRPLAY_FLAGS_MODE)
    {
        rPvrPlayStruct.eMode = prPVRPlay->eMode;
    }

    if (u4Flags & PVRPLAY_FLAGS_BUFFER)
    {
        if ((rPvrPlayStruct.u4BufStart != 0) || (rPvrPlayStruct.u4BufEnd != 0))
        {
            _DMX_Unlock();
            LOG(3, "Buffer already allocated\n");
            return FALSE;
        }

        if (prPVRPlay->fgAllocBuf)
        {
            u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(prPVRPlay->u4BufSize, 32);
            if (u4BufStart == 0)
            {
                _DMX_Unlock();
                LOG(1, "Out of memory!\n");
                return FALSE;
            }
            u4BufEnd = u4BufStart + prPVRPlay->u4BufSize;
            //LOG(5, "PVR play buffer: 0x%08x ~ 0x%08x\n", u4BufStart, u4BufEnd);
            // Can not print here
            //Printf("PVR play buffer: 0x%08x ~ 0x%08x\n", u4BufStart, u4BufEnd);
            HalInvalidateDCacheMultipleLine(VIRTUAL(u4BufStart), prPVRPlay->u4BufSize);
        }
        else
        {
            ASSERT(prPVRPlay->u4BufStart != 0);
            u4BufStart = prPVRPlay->u4BufStart;
            u4BufEnd = prPVRPlay->u4BufStart + prPVRPlay->u4BufSize;
        }

        u4VirBufStart = VIRTUAL(u4BufStart);

        rPvrPlayStruct.fgAllocBuf = prPVRPlay->fgAllocBuf;
        rPvrPlayStruct.u4BufStart = u4BufStart;
        rPvrPlayStruct.u4BufEnd = u4BufStart + prPVRPlay->u4BufSize;
        rPvrPlayStruct.u4BufSize = prPVRPlay->u4BufSize;
        rPvrPlayStruct.u4Wp = u4VirBufStart;
        rPvrPlayStruct.u4Rp = u4VirBufStart;

        u4PhyBufStart = PHYSICAL(u4BufStart);
        u4PhyBufEnd = PHYSICAL(u4BufEnd);

        // Set playback buffer
        if (u1TsoutIdx == 0)
        {
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, u4PhyBufStart);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, u4PhyBufEnd - 1);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, u4PhyBufStart);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, u4PhyBufStart);
        }
        else if (u1TsoutIdx == 1)
        {
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START2, u4PhyBufStart);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END2, u4PhyBufEnd - 1);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP2, u4PhyBufStart);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP2, u4PhyBufStart);
        }
		else
		{
		    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START3, u4PhyBufStart);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END3, u4PhyBufEnd - 1);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP3, u4PhyBufStart);
            DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP3, u4PhyBufStart);
		}

    }

    _arPVRPlay[u1TsoutIdx] = rPvrPlayStruct;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Get
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_Get(UINT8 u1TsOutIdx, UINT32 u4Flags, DMX_PVR_PLAY_T* prPVRPlay)
{
    UINT32 u4Wp, u4Rp;
    DMX_PVR_PLAY_T* prPvrPlayStruct;

      ASSERT(prPVRPlay != NULL);

    if (u1TsOutIdx >= DMX_MAX_TSOUT)
    {
        LOG(3, "Invalid Ts Out Index: %u\n", u1TsOutIdx);
        return FALSE;
    }

    if (u4Flags == PVRPLAY_FLAGS_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();
    prPvrPlayStruct = &_arPVRPlay[u1TsOutIdx];
    _DMX_Unlock();


    if (u4Flags & PVRPLAY_FLAGS_TIMESTAMP)
    {
        prPVRPlay->fgContainTimeStamp = prPvrPlayStruct->fgContainTimeStamp;
        prPVRPlay->fgIgnoreTimeStamp = prPvrPlayStruct->fgIgnoreTimeStamp;
        prPVRPlay->u2TimeStampFreqDiv = prPvrPlayStruct->u2TimeStampFreqDiv;
    }

    if (u4Flags & PVRPLAY_FLAGS_THRESHOLD)
    {
        prPVRPlay->u4ThresholdSize = prPvrPlayStruct->u4ThresholdSize;
    }

    if (u4Flags & PVRPLAY_FLAGS_CALLBACK)
    {
        prPVRPlay->pfnPVRNotify = prPvrPlayStruct->pfnPVRNotify;
    }

    if (u4Flags & PVRPLAY_FLAGS_MODE)
    {
        prPVRPlay->eMode = prPvrPlayStruct->eMode;
    }

    if (u4Flags & PVRPLAY_FLAGS_BUFFER)
    {
        prPVRPlay->fgAllocBuf = prPvrPlayStruct->fgAllocBuf;
        prPVRPlay->u4BufStart = prPvrPlayStruct->u4BufStart;
        prPVRPlay->u4BufEnd = prPvrPlayStruct->u4BufEnd;
        prPVRPlay->u4BufSize = prPvrPlayStruct->u4BufSize;
        prPVRPlay->u4ThresholdSize = prPvrPlayStruct->u4ThresholdSize;

        _DMX_Lock();
        if (u1TsOutIdx == 0)
        {
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP);
            u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP);
        }
        else if(u1TsOutIdx == 1)
        {
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP2);
            u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP2);
        }
		else
		{
            u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP3);
            u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP3);
		}
        _DMX_Unlock();

        prPVRPlay->u4Rp = VIRTUAL(u4Rp);
        prPVRPlay->u4Wp = VIRTUAL(u4Wp);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_Notify
    Call in HISR
*/
//-----------------------------------------------------------------------------
VOID _DMX_PVRPlay_Notify(UINT8 u1TsOutIdx)
{
    DMX_PVR_PLAY_T* prPvrPlayStruct;
#ifndef CC_DMX_EMULATION
    UINT32 u4OrgRp, u4Rp, u4Threshold, u4BufEnd, u4BufSize;
#endif

    if (u1TsOutIdx >= DMX_MAX_TSOUT)
    {
        LOG(3, "Invalid Ts Out Index: %u\n", u1TsOutIdx);
        return ;
    }

    _DMX_Lock();
    prPvrPlayStruct = &_arPVRPlay[u1TsOutIdx];
    _DMX_Unlock();

    if (prPvrPlayStruct->eMode == DMX_PVR_PLAY_SINGLEMOVE)
    {
        VERIFY(x_sema_unlock(_hDMXPVRPlaySema[u1TsOutIdx]) == OSR_OK);
#ifdef CC_DMX_EMULATION
		_PVR_EMU_NotifyEx(u1TsOutIdx);
#endif
        return;
    }

#ifdef CC_DMX_EMULATION
    _PVR_EMU_NotifyEx(u1TsOutIdx);
#else
    if (prPvrPlayStruct->eState != DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(3, "Interrupt occur when play disable\n");
        return;
    }

    _DMX_Lock();
    u4OrgRp = prPvrPlayStruct->u4Rp;
    u4BufEnd = prPvrPlayStruct->u4BufEnd;
    u4BufSize = prPvrPlayStruct->u4BufSize;
    u4Threshold = prPvrPlayStruct->u4ThresholdSize;
    u4Rp = u4OrgRp + u4Threshold;
    if (u4Rp >= u4BufEnd)
    {
        u4Rp -= u4BufSize;
    }
    prPvrPlayStruct->u4Rp = u4Rp;
    _DMX_Unlock();

    if (prPvrPlayStruct->pfnPVRNotify != NULL)
    {
        if (!prPvrPlayStruct->pfnPVRNotify(u4OrgRp, u4Threshold))
        {
            LOG(3, "PVR play notfiy fail\n");
        }
    }
#endif
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_UpdateWp
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_UpdateWp(UINT8 u1TsOutIdx, UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 u4NewWp, u4BufStart, u4BufEnd;
    UINT8 *pu1Tmp;
    DMX_PVR_PLAY_T* prPvrPlayStruct;

    if (u1TsOutIdx >= DMX_MAX_TSOUT)
    {
        LOG(3, "Invalid Ts Out Index: %u\n", u1TsOutIdx);
        return FALSE;
    }

    _DMX_Lock();
    prPvrPlayStruct = &_arPVRPlay[u1TsOutIdx];
    u4BufStart = prPvrPlayStruct->u4BufStart;
    u4BufEnd = prPvrPlayStruct->u4BufEnd;
    _rPVRPlayCounter[u1TsOutIdx].u4MoveCount++;
    _rPVRPlayCounter[u1TsOutIdx].u4MoveSize += u4Size;
    _DMX_Unlock();

    u4NewWp = _DmxPVRPlay_UpdateAddr(u4Addr, u4Size, u4BufStart, u4BufEnd);

    _DMX_Lock();
    prPvrPlayStruct->u4Wp = u4NewWp;
    if (u1TsOutIdx == 0)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, PHYSICAL(u4NewWp));
    }
    else if(u1TsOutIdx == 1)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP2, PHYSICAL(u4NewWp));
    }
	else
	{
	    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP3, PHYSICAL(u4NewWp));
	}
    _DMX_Unlock();

    // Check
    u4NewWp = u4Addr + 4;
    if (u4NewWp > u4BufEnd)
    {
        u4NewWp -= (u4BufEnd - u4BufStart);
    }
    pu1Tmp = (UINT8*)u4NewWp;
    if (pu1Tmp[0] == 0x47)
    {
        _DMX_Lock();
        _rPVRPlayCounter[u1TsOutIdx].u4MoveCheck++;
        _DMX_Unlock();
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_FreeBuf
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_FreeBuf(UINT8 u1TsOutIdx)
{
    BOOL fgAllocBuf;
    UINT32 u4BufStart;
    DMX_PVR_PLAY_T* prPvrPlayStruct;

    if (u1TsOutIdx >= DMX_MAX_TSOUT)
    {
        LOG(3, "Invalid Ts Out Index: %u\n", u1TsOutIdx);
        return FALSE;
    }

    _DMX_Lock();
    prPvrPlayStruct = &_arPVRPlay[u1TsOutIdx];
    _DMX_Unlock();

    _DMX_Lock();
    fgAllocBuf = prPvrPlayStruct->fgAllocBuf;
    u4BufStart = prPvrPlayStruct->u4BufStart;
    _DMX_Unlock();

    if (fgAllocBuf && (u4BufStart != 0))
    {
        if (!BSP_FreeAlignedDmaMemory(u4BufStart))
        {
            LOG(3, "Fail to free PVR play buffer!, addr: 0x%08x\n", u4BufStart);
            return FALSE;
        }
    }

    _DMX_Lock();
    prPvrPlayStruct->u4BufStart = 0x0;
    prPvrPlayStruct->u4BufEnd = 0x0;
    prPvrPlayStruct->u4Wp = 0x0;
    prPvrPlayStruct->u4Rp = 0x0;

    if (u1TsOutIdx == 0)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, 0x0);
    }
    else if (u1TsOutIdx == 1)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START2, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END2, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP2, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP2, 0x0);
    }
	else
	{
	    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START3, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END3, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP3, 0x0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP3, 0x0);
	}
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_FlushBuf
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_FlushBuf(UINT8 u1TsOutIdx)
{
    UINT32 u4BufStart;
    DMX_PVR_PLAY_T* prPvrPlayStruct;

    if (u1TsOutIdx >= DMX_MAX_TSOUT)
    {
        LOG(3, "Invalid Ts Out Index: %u\n", u1TsOutIdx);
        return FALSE;
    }

    _DMX_Lock();
    prPvrPlayStruct = &_arPVRPlay[u1TsOutIdx];
    u4BufStart = prPvrPlayStruct->u4BufStart;

    if (u1TsOutIdx == 0)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, u4BufStart);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, u4BufStart);
    }
    else if (u1TsOutIdx == 1)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP2, u4BufStart);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP2, u4BufStart);
    }
	else
	{
	    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP3, u4BufStart);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP3, u4BufStart);
	}

    prPvrPlayStruct->u4Rp = u4BufStart;
    prPvrPlayStruct->u4Wp = u4BufStart;
    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SingleMove
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SingleMove(UINT8 u1TsOutIdx, const DMX_MM_DATA_T* prData)
{
    UINT32 u4PhyBufStart, u4PhyBufEnd, u4PhyRp, u4PhyWp;
    UINT32 u4Wp, u4BufSize;
    BOOL fgRet;
    //UINT32 u4CheckAddr;
    //UINT8 *pu1Tmp;
#ifdef DMX_PVRPLAY_MEASURE_TIME
    HAL_TIME_T rTime1, rTime2, rTimeDiff;
    HAL_GetTime(&rTime1);
#endif

    if (prData == NULL)
    {
        return FALSE;
    }

    ASSERT(DMX_IsAligned(prData->u4BufStart, DMX_PVR_BUF_ALIGNMENT));
    ASSERT(DMX_IsAligned(prData->u4BufEnd,   DMX_PVR_BUF_ALIGNMENT));
    // PVR WP, RP is byte-alignment
    //ASSERT(DMX_IsAligned(prData->u4StartAddr,DMX_PVR_POINTER_ALIGNMENT));
    //ASSERT(DMX_IsAligned(prData->u4FrameSize,DMX_PVR_POINTER_ALIGNMENT));

    ASSERT(_arPVRPlay[u1TsOutIdx].eMode == DMX_PVR_PLAY_SINGLEMOVE);
    ASSERT(_arPVRPlay[u1TsOutIdx].fgIgnoreTimeStamp);

    if (_arPVRPlay[u1TsOutIdx].eState != DMX_PVRPLAY_STATE_STOP)
    {
        return FALSE;
    }

    if (prData->u4FrameSize == 0)
    {
        if (prData->fgEOS)
        {
            // Wait uCode really finish handle final data chunk
            // Because EOS, delay 10ms is not easy detectable
            x_thread_delay(10);
            LOG(3, "Send EOS\n");
            _DmxPVRPlay_SyncVideoWp(prData->u1Idx);
            _DmxPVRPlay_SendEOS(prData->u1Idx);
        }
        return TRUE;
    }
    
    if ((prData->u4StartAddr < prData->u4BufStart) ||
            (prData->u4StartAddr >= prData->u4BufEnd))
    {
        LOG(3, "Addr invalid\n");
        return FALSE;
    }

    u4BufSize = prData->u4BufEnd - prData->u4BufStart;
    if (prData->u4FrameSize >= u4BufSize)
    {
        LOG(3, "Size is too large\n");
        return FALSE;
    }

    u4Wp = prData->u4StartAddr + prData->u4FrameSize;
    if (u4Wp >= prData->u4BufEnd)
    {
        u4Wp -= u4BufSize;
    }

    u4PhyBufStart = PHYSICAL(prData->u4BufStart);
    u4PhyBufEnd = PHYSICAL(prData->u4BufEnd);
    u4PhyRp = PHYSICAL(prData->u4StartAddr);
    u4PhyWp = PHYSICAL(u4Wp);

#ifndef  CC_DMX_FLUSH_ALL
    if (IS_KERN_LOGICAL_ADDRESS(prData->u4StartAddr))
    {
        if ((prData->u4StartAddr+prData->u4FrameSize)> prData->u4BufEnd)
        {
            HalFlushInvalidateDCacheMultipleLine(prData->u4StartAddr,(prData->u4BufEnd-prData->u4StartAddr));
            HalFlushInvalidateDCacheMultipleLine(prData->u4BufStart,(prData->u4StartAddr+prData->u4FrameSize-prData->u4BufEnd));
        }
        else
        {
            HalFlushInvalidateDCacheMultipleLine(prData->u4StartAddr,prData->u4FrameSize);
        }
    }
#else
    HalFlushInvalidateDCache();
#endif

#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_MT5890)
    {
        DMX_MMSECURITY_STRUCT_T rDataInfo;

        x_memset((void*)&rDataInfo, 0, sizeof(rDataInfo));
        rDataInfo.e_SetType = DMX_MMSECURITY_MOVEDATA_PVR;
        rDataInfo.u1TsIdx = prData->u1Idx;
        rDataInfo.u1TsOutIdx = u1TsOutIdx;
        rDataInfo.u4BufAddr = u4PhyBufStart;
        rDataInfo.u4BufEnd = u4PhyBufEnd;
        rDataInfo.u4SendAddr = PHYSICAL(prData->u4StartAddr);
        rDataInfo.u4SendSize = prData->u4FrameSize;
        rDataInfo.u4SkipLen = prData->u4SikpLen;
        if (!TZ_DMX_SetMMSecurity(&rDataInfo, sizeof(rDataInfo)))
        {
            LOG(0, "Fail to Move Data Secure Check!\n");
            return FALSE;
        }
    }
#else
     _DMX_Lock();
    if (u1TsOutIdx == 0)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, u4PhyBufStart);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, u4PhyBufEnd - 1);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, u4PhyRp);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, u4PhyWp);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD, prData->u4FrameSize);
    }
    else  if (u1TsOutIdx == 1)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START2, u4PhyBufStart);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END2, u4PhyBufEnd - 1);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP2, u4PhyRp);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP2, u4PhyWp);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD2, prData->u4FrameSize);
    }
    else
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START3, u4PhyBufStart);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END3, u4PhyBufEnd - 1);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP3, u4PhyRp);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP3, u4PhyWp);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_THRESHOLD3, prData->u4FrameSize);
    }    
    _DMX_Unlock();
#endif

    // PVR Start
    _DMX_Lock();
    _arPVRPlay[u1TsOutIdx].u4BufStart = VIRTUAL(prData->u4BufStart);
    _arPVRPlay[u1TsOutIdx].u4BufEnd = VIRTUAL(prData->u4BufEnd);
    _arPVRPlay[u1TsOutIdx].u4BufSize = u4BufSize;
    _arPVRPlay[u1TsOutIdx].u4Wp = VIRTUAL(u4Wp);
    _arPVRPlay[u1TsOutIdx].u4Rp = VIRTUAL(prData->u4StartAddr);
    _arPVRPlay[u1TsOutIdx].u4ThresholdSize = prData->u4FrameSize;

    //// Counters
    _rPVRPlayCounter[u1TsOutIdx].u4SingleCount++;
    _rPVRPlayCounter[u1TsOutIdx].u4SingleSize += prData->u4FrameSize;
#if 0
    u4CheckAddr = prData->u4StartAddr + 4;
    if (u4CheckAddr > _arPVRPlay[u1TsOutIdx].u4BufEnd)
    {
        u4CheckAddr -= _arPVRPlay[u1TsOutIdx].u4BufSize;
    }
    pu1Tmp = (UINT8*)u4CheckAddr;
    if (pu1Tmp[0] == 0x47)
    {
        _rPVRPlayCounter[u1TsOutIdx].u4SingleCheck++;
    }
#else
    _rPVRPlayCounter[u1TsOutIdx].u4SingleCheck++;
#endif
    _DMX_Unlock();

    if (!_DMX_PVRPlay_Start(u1TsOutIdx))
    {
        return FALSE;
    }

    fgRet = TRUE;

    VERIFY(OSR_OK == x_sema_lock(_hDMXPVRPlaySema[u1TsOutIdx], X_SEMA_OPTION_WAIT));
     
#if 0
    HAL_GetTime(&rTime2);
    HAL_GetDeltaTime(&rTimeDiff, &rTime2, &rTime1);
    LOG(3, "Single Move: size:%d .%06d\n", prData->u4FrameSize, rTimeDiff.u4Micros);
#endif

    if (!_DMX_PVRPlay_Stop(u1TsOutIdx,TRUE))
    {
        LOG(3, "Stop error\n");
        fgRet = FALSE;
    }

#ifdef DMX_PVRPLAY_MEASURE_TIME
    HAL_GetTime(&rTime2);
    HAL_GetDeltaTime(&rTimeDiff, &rTime2, &rTime1);
    LOG(5, "Single Move: size:%d .%06d\n", prData->u4FrameSize, rTimeDiff.u4Micros);
#endif

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetBufPointer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_GetBufPointer(UINT8 u1TsOutIdx, DMX_PVRPLAY_BUFPTR_T *prPtr)
{
    if (prPtr == NULL)
    {
        return FALSE;
    }

    _DMX_Lock();

    if (u1TsOutIdx == 0)
    {
        prPtr->u4HwWp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP));
        prPtr->u4HwRp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP));
    }
    else if (u1TsOutIdx == 1)
    {
        prPtr->u4HwWp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP2));
        prPtr->u4HwRp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP2));
    }
	else
	{
	    prPtr->u4HwWp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP3));
        prPtr->u4HwRp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP3));
    }

    prPtr->u4SwWp = _arPVRPlay[u1TsOutIdx].u4Wp;
    prPtr->u4SwRp = _arPVRPlay[u1TsOutIdx].u4Rp;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SetBuffer
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SetBuffer(UINT8 u1TsOutIdx, UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Wp, UINT32 u4Rp)
{
    ASSERT(u4BufStart < u4BufEnd);
    ASSERT((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
    ASSERT((u4Rp >= u4BufStart) && (u4Rp < u4BufEnd));
    ASSERT((u4BufEnd % 4) == 3);        // End - 1
    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (_arPVRPlay[u1TsOutIdx].eState != DMX_PVRPLAY_STATE_STOP)
    {
        return FALSE;
    }

    _DMX_Lock();
    _arPVRPlay[u1TsOutIdx].u4BufStart = u4BufStart;
    _arPVRPlay[u1TsOutIdx].u4BufEnd = u4BufEnd + 1;
    _arPVRPlay[u1TsOutIdx].u4Wp = u4Wp;
    _arPVRPlay[u1TsOutIdx].u4Rp = u4Rp;

    if (u1TsOutIdx == 0)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, PHYSICAL(u4BufStart));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, PHYSICAL(u4BufEnd));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, PHYSICAL(u4Wp));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, PHYSICAL(u4Rp));
    }
    else  if (u1TsOutIdx == 1)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START2, PHYSICAL(u4BufStart));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END2, PHYSICAL(u4BufEnd));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP2, PHYSICAL(u4Wp));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP2, PHYSICAL(u4Rp));
    }
	else
	{
	    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START3, PHYSICAL(u4BufStart));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END3, PHYSICAL(u4BufEnd));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP3, PHYSICAL(u4Wp));
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP3, PHYSICAL(u4Rp));
	}
    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_RequestReset(UINT8 u1TsOutIdx, UINT8 u1TsIdx)
{
    LOG(3, "_DMX_PVRPlay_RequestReset\n");

    if (_arPVRPlay[u1TsOutIdx].eState == DMX_PVRPLAY_STATE_PLAYING)
    {
        ASSERT(0);
    }

    _DMX_PVRPlay_Reset(u1TsOutIdx);

    _DMX_Lock();
    _arPVRPlay[u1TsOutIdx].u4BufStart = 0;
    _arPVRPlay[u1TsOutIdx].u4BufEnd = 0;
    _arPVRPlay[u1TsOutIdx].u4Wp = 0;
    _arPVRPlay[u1TsOutIdx].u4Rp = 0;

    if (u1TsOutIdx == 0)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP, 0);
    }
    else if(u1TsOutIdx == 1)
    {
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START2, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END2, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP2, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP2, 0);
    }
	else
	{
	    DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_START3, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_END3, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_WP3, 0);
        DMXCMD_WRITE32(DMX_REG_PVR_PLAY_BUF_RP3, 0);
	}
    _DMX_Unlock();

    // Reset DMX
    if (!_DDI_TsResetDmxBuffers(u1TsIdx))
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetCounter
 */
//-----------------------------------------------------------------------------
void _DMX_PVRPlay_GetCounter(UINT8 u1TsOutIdx, DMX_PVRPLAY_COUNTER_T *prCounter)
{
    UINT32 u4Rp, u4Wp;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (prCounter != NULL)
    {
        _DMX_Lock();

        if (u1TsOutIdx == 0)
        {
            u4Wp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP));
            u4Rp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP));
        }
        else if (u1TsOutIdx == 1)
        {
            u4Wp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP2));
            u4Rp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP2));
        }
		else
		{
		    u4Wp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP3));
            u4Rp = VIRTUAL(DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP3));
		}

        _rPVRPlayCounter[u1TsOutIdx].u4DataSize = DATASIZE(u4Rp, u4Wp, _arPVRPlay[u1TsOutIdx].u4BufSize);
        *prCounter = _rPVRPlayCounter[u1TsOutIdx];
        _DMX_Unlock();
    }
}


//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_ResetCounter
 */
//-----------------------------------------------------------------------------
void _DMX_PVRPlay_ResetCounter(UINT8 u1TsOutIdx)
{
    _DMX_Lock();
    x_memset((void*)&_rPVRPlayCounter[u1TsOutIdx], 0, sizeof(DMX_PVRPLAY_COUNTER_T));
    _DMX_Unlock();
}

//-----------------------------------------------------------------------------
/** add new function:set the pb_steer_bit9 for ic_design test
 */
//-----------------------------------------------------------------------------
void _DMX_Set_PB_STEER_BIT_9(UINT8 u1TsOutIdx,BOOL bit_9_enable)
{
   UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (u1TsOutIdx == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & (~0x200);
		if(bit_9_enable)
		{
		  u4Reg |=(1<<9);
		}

        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
    }
	else if(u1TsOutIdx == 1)
	{
	    u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL) & (~0x200);
		if(bit_9_enable)
		{
		  u4Reg |=(1<<9);
		}

        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
	}
	else if(u1TsOutIdx == 2)
	{
	   	u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL) & (~0x200);
		if(bit_9_enable)
		{
		  u4Reg |=(1<<9);
		}

        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
	}
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SetPort
 */
//-----------------------------------------------------------------------------
void _DMX_PVRPlay_SetPort(UINT8 u1TsOutIdx, DMX_PVRPLAY_PORT_T ePort)
{
    UINT8 u1TsIdx = 0;
    BOOL fgTSOut = FALSE;
    BOOL fgFeConfig = FALSE;
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);
    
    switch (ePort)
    {
    case DMX_PVRPLAY_PORT_FRAMER0:
        u1TsIdx = 0;
        fgTSOut = TRUE;
        fgFeConfig = TRUE;
        break;

    case DMX_PVRPLAY_PORT_FRAMER1:
        u1TsIdx = 1;
        fgTSOut = TRUE;
        fgFeConfig = TRUE;
        break;

#if defined(CC_MT5890)
    case DMX_PVRPLAY_PORT_FRAMER2:
        u1TsIdx = 2;
        fgTSOut = TRUE;
        fgFeConfig = TRUE;

        //Set input source =  Framer;
        DMX_SetDbm_InputSource(2,DMX_DBM_INPUT_FRAMER);
        break;
#endif

    case DMX_PVRPLAY_PORT_FRAMER3:
        u1TsIdx = 3;
        fgTSOut = TRUE;
        fgFeConfig = TRUE;

        //Set input source =  Framer;
        DMX_SetDbm_InputSource(3,DMX_DBM_INPUT_FRAMER);
        break;

    case DMX_PVRPLAY_PORT_FRAMER2_BYPASS:
        u1TsIdx = 2;
        fgTSOut = FALSE;
        fgFeConfig = FALSE;

        //Set input source =  PlayBack;
        if (u1TsOutIdx == 0)
        {
            DMX_SetDbm_InputSource(2,DMX_DBM_INPUT_PB);
        }
        else if(u1TsOutIdx == 1)
        {
            DMX_SetDbm_InputSource(2,DMX_DBM_INPUT_PB2);
        }
        break;

    case DMX_PVRPLAY_PORT_FRAMER3_BYPASS:
        u1TsIdx = 3;
        fgTSOut = FALSE;
        fgFeConfig = FALSE;

        //Set input source =  PlayBack;
        if (u1TsOutIdx == 0)
        {
            DMX_SetDbm_InputSource(3,DMX_DBM_INPUT_PB);
        }
        else if(u1TsOutIdx == 1)
        {
            DMX_SetDbm_InputSource(3,DMX_DBM_INPUT_PB2);
        }
        break;

	case DMX_PVRPLAY_PORT_FRAMER4_BYPASS:
		u1TsIdx = 4;
        fgTSOut = FALSE;
        fgFeConfig = FALSE;

        //Set input source =  PlayBack;
        if (u1TsOutIdx == 0)
        {
            DMX_SetDbm_InputSource(4,DMX_DBM_INPUT_PB);
        }
        else if(u1TsOutIdx == 1)
        {
            DMX_SetDbm_InputSource(4,DMX_DBM_INPUT_PB2);
        }
        else if(u1TsOutIdx == 2)
        {
            DMX_SetDbm_InputSource(4,DMX_DBM_INPUT_PB3);
        }
        break;
		
    case DMX_PVRPLAY_PORT_TSOUT:

        //Only output
        u1TsIdx = u1TsOutIdx;
        fgTSOut = TRUE;
        fgFeConfig = FALSE;
        break;
	case DMX_PVRPLAY_PORT_DBM:
		{
		  u1TsIdx =  4;
		  fgTSOut = FALSE;
		  fgFeConfig = FALSE;
		   if (u1TsOutIdx == 0)
          {
            DMX_SetDbm_InputSource(4,DMX_DBM_INPUT_PB);
          }
           else if(u1TsOutIdx == 1)
          {
            DMX_SetDbm_InputSource(4,DMX_DBM_INPUT_PB2);
          }
		  else
		  {
		    _DMX_Set_PB_STEER_BIT_9(2,TRUE); //must enable bit[9].
		    DMX_SetDbm_InputSource(4,DMX_DBM_INPUT_PB3);
		  }
		  
		}
	    break;
    default:
        ASSERT(0);
    }

    if (fgTSOut)
    {
         //disable PES/DBM path, Enable output
        DMX_TsOut_SetRountDBMTsout(u1TsOutIdx, FALSE, TRUE);

        //Set output ts clock 108M
        _DMX_TsOut_SetClock(u1TsOutIdx, DMX_PVR_OUTPUT_CLOCK_108M);

        //config mux before framer
        _DMX_TsIn_SetRountMux(u1TsIdx, u1TsOutIdx);


        // Config PVR TS-out
        if (u1TsOutIdx == 0)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
            u4Reg &= ~0xFC0000;
            u4Reg |= (0 << 18) |        // 0: MSB, 1: LSB
                     (0 << 19) |        // 0: 8 sync cycle, 1: 1 sync cycle
                     (0 << 20) |        // 0 ~ 3: valid space cycle
                     (1 << 22) |        // 0:pos edge, 1:neg edge
                     (1 << 23);         // 0:serial, 1:paralle
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
        }
        else  if(u1TsOutIdx == 1)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
            u4Reg &= ~0xFC0000;
            u4Reg |= (0 << 18) |        // 0: MSB, 1: LSB
                     (0 << 19) |        // 0: 8 sync cycle, 1: 1 sync cycle
                     (0 << 20) |        // 0 ~ 3: valid space cycle
                     (1 << 22) |        // 0:pos edge, 1:neg edge
                     (1 << 23);         // 0:serial, 1:paralle
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
        }
		else
		{
		    u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
            u4Reg &= ~0xFC0000;
            u4Reg |= (0 << 18) |        // 0: MSB, 1: LSB
                     (0 << 19) |        // 0: 8 sync cycle, 1: 1 sync cycle
                     (0 << 20) |        // 0 ~ 3: valid space cycle
                     (1 << 22) |        // 0:pos edge, 1:neg edge
                     (1 << 23);         // 0:serial, 1:paralle
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
		   
		}

        // TS-out framer selection
        if (fgFeConfig)
        {
           _DMX_SetFrontEndEx(u1TsIdx, DMX_FE_TSOUT);
        }
    }
    else
    {
        //Enable PES/DBM path, Disable output
        DMX_TsOut_SetRountDBMTsout(u1TsOutIdx, TRUE, FALSE);
    }

    _DMX_Lock();
    _fgTSOut[u1TsOutIdx] = fgTSOut;
    _ePVRPort[u1TsOutIdx] = ePort;
    _DMX_Unlock();
}

BOOL _DMX_PVRPlay_SetPortEx(UINT8 u1PvrIdx, UINT8 u1TsIdx, UINT8 u1Pidx, 
                            DMX_PVRPLAY_PORT_T ePort, BOOL fgIsTs, BOOL fgIsOutput)
{
    DMX_DBM_INPUT_SOURCE_T eSource;
    UINT8 au1Offset[5] = { 0, 0, 28, 29, 25 };
    UINT32 u4PidCtrl, u4DbmBypassPidReg;

    if (u1PvrIdx >= DMX_MAX_TSOUT)
    {
        return FALSE;
    }

    if (ePort == DMX_PVRPLAY_PORT_DBM)
    {
        switch (u1PvrIdx)
        {
        case 0:
            eSource = DMX_DBM_INPUT_PB;
            break;
        case 1:
            eSource = DMX_DBM_INPUT_PB2;
            break;
        case 2:
            eSource = DMX_DBM_INPUT_PB3;
            break;
        default:
            ASSERT(0);
        }

        switch (u1TsIdx)
        {
        case 2:
            u4DbmBypassPidReg = DMX_REG_DBM_BYPASS_PID_3;
            u4PidCtrl = 0x20;
            break;
        case 3:
            u4DbmBypassPidReg = DMX_REG_DBM_BYPASS_PID_4;
            u4PidCtrl = 0x20;
            break;
        case 4:
            u4DbmBypassPidReg = DMX_REG_DBM_BYPASS_PID_5;
            u4PidCtrl = 0x21;
            break;
        default:
            ASSERT(0);
        }
        
        if (fgIsTs)
        {
            DMXCMD_WRITE32(u4DbmBypassPidReg, 0);
   
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) & ~(1 << au1Offset[u1TsIdx]));

            _DMX_SetDbm_InputSource(u1TsIdx, eSource);
        
            _DmxPVRPlay_SetFramerMode(u1PvrIdx, FALSE, TRUE);
        }
        else
        {
            DMXCMD_WRITE32(u4DbmBypassPidReg, u1Pidx << 16 | u4PidCtrl);
            
            DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, DMXCMD_READ32(DMX_REG_DBM_CONTROL) | (1 << au1Offset[u1TsIdx]));
            
            _DMX_SetDbm_InputSource(u1TsIdx, eSource);
            
            _DmxPVRPlay_SetFramerMode(u1PvrIdx, TRUE, TRUE);
        }
    }
    
    UNUSED(fgIsOutput);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_GetPort
 */
//-----------------------------------------------------------------------------
DMX_PVRPLAY_PORT_T _DMX_PVRPlay_GetPort(UINT8 u1TsOutIdx)
{
    DMX_PVRPLAY_PORT_T ePort;

    _DMX_Lock();
    ePort = _ePVRPort[u1TsOutIdx];
    _DMX_Unlock();

    return ePort;
}

//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SetMoveMode
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SetMoveMode(UINT8 u1TsOutIdx, DMX_TSFMT_T eTSFmt, BOOL fgForce)
{
    BOOL fgContainTimestamp, fgUseTimestamp;
    DMX_PVR_PLAY_T rPlay;
    DMX_PVR_PLAYSTATE_T eState;

    _DMX_Lock();
    eState = _arPVRPlay[u1TsOutIdx].eState;
    _DMX_Unlock();

    if (eState == DMX_PVRPLAY_STATE_PLAYING)
    {
        LOG(0, "PVRPLAY is in Playing mode.\n");
        return FALSE;
    }

    if (fgForce)
    {
        if (_eDmxPVRTSFmt[u1TsOutIdx] == eTSFmt)
        {
            return TRUE;
        }
    }

    fgContainTimestamp = FALSE;
    fgUseTimestamp = FALSE;

    if ((eTSFmt == DMX_TSFMT_192) || (eTSFmt == DMX_TSFMT_192_ENCRYPT))
    {
        fgContainTimestamp = TRUE;
    }
    else if ((eTSFmt == DMX_TSFMT_130) || (eTSFmt == DMX_TSFMT_134))
    {
        fgContainTimestamp = FALSE;
        fgUseTimestamp = FALSE;
    }
    else if (eTSFmt == DMX_TSFMT_TIMESHIFT)
    {
        fgContainTimestamp = TRUE;
        fgUseTimestamp = TRUE;
    }

    if (fgContainTimestamp && fgUseTimestamp)
    {
        rPlay.eMode = DMX_PVR_PLAY_STREAM;
    }
    else
    {
        rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;
    }

    rPlay.fgContainTimeStamp = fgContainTimestamp;
    rPlay.fgIgnoreTimeStamp = !fgUseTimestamp;
    rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    if (!_DMX_PVRPlay_Set(u1TsOutIdx, (UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rPlay))
    {
        return FALSE;
    }

    _DMX_Lock();
    _eDmxPVRTSFmt[u1TsOutIdx] = eTSFmt;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_EMU_PalyBack_130byteEnable
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_130byteEnable(UINT8 u1TsoutIdx, BOOL fgEnable, UINT8 u1PktSize)
{
    UINT32 u4Reg;

    if (u1TsoutIdx == 0)
    {
        if (fgEnable)
        {
            //set input packet size register 0x17908 [31:24] for 130 packet size
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL2);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL2, u4Reg);

            //set output packet size register 0x17904 [31:24]
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Reg);

            //set input output enable .and playback 130 byte enable
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
            u4Reg |= (0x3 << 5);
            u4Reg |= (0x1 << 3);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Reg);

        }
        else
        {
            //set input packet size register 0x17908 [31:24] for 188 packet size
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL2);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL2, u4Reg);

            //set output packet size register 0x17904 [31:24]
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Reg);

            //set input output enable .and playback 130 byte enable
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
            u4Reg &= ~(0x3 << 5);
            u4Reg &= ~(0x1 << 3);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Reg);
        }
    }
    else  if (u1TsoutIdx == 1)
    {
        if (fgEnable)
        {
            //set input packet size register 0x17608 [31:24] for 130 packet size
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL2);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL2, u4Reg);

            //set output packet size register 0x17604 [31:24]
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Reg);

            //set input output enable .and playback 130 byte enable
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1);
            u4Reg |= (0x3 << 5);
            u4Reg |= (0x1 << 3);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Reg);

        }
        else
        {
            //set input packet size register 0x17608 [31:24] for 188 packet size
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL2);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL2, u4Reg);

            //set output packet size register 0x17604 [31:24]
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Reg);

            //set input output enable .and playback 130 byte enable
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL1);
            u4Reg &= ~(0x3 << 5);
            u4Reg &= ~(0x1 << 3);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL1, u4Reg);
        }
    }
	else
    {
        if (fgEnable)
        {
            //set input packet size register 0x17608 [31:24] for 130 packet size
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL2);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL2, u4Reg);

            //set output packet size register 0x17604 [31:24]
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4Reg);

            //set input output enable .and playback 130 byte enable
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1);
            u4Reg |= (0x3 << 5);
            u4Reg |= (0x1 << 3);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4Reg);

        }
        else
        {
            //set input packet size register 0x17608 [31:24] for 188 packet size
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL2);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL2, u4Reg);

            //set output packet size register 0x17604 [31:24]
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1);
            u4Reg &= ~((UINT32)0xFF << 24);
            u4Reg |= (u1PktSize << 24);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4Reg);

            //set input output enable .and playback 130 byte enable
            u4Reg = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER3_CTRL1);
            u4Reg &= ~(0x3 << 5);
            u4Reg &= ~(0x1 << 3);
            DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER3_CTRL1, u4Reg);
        }
    }

    return TRUE;

}

//-----------------------------------------------------------------------------
/** _DMX_OpenTimeshift
 */
//-----------------------------------------------------------------------------
BOOL _DMX_OpenTimeshift(UINT8 u1TsOutIdx, BOOL fgOpen)
{
    if (fgOpen)
    {
        //// Fixme, check resource conflict
        if (!_DMX_PVRPlay_SetMoveMode(u1TsOutIdx, DMX_TSFMT_TIMESHIFT, TRUE))
        {
            return FALSE;
        }
    }

    _DMX_Lock();
    _fgDmxTimeshiftOpen[u1TsOutIdx] = fgOpen;
    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_IsTimeshiftOpen
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IsTimeshiftOpen(UINT8 u1TsOutIdx)
{
    BOOL fgOpen;

    _DMX_Lock();
    fgOpen = _fgDmxTimeshiftOpen[u1TsOutIdx];
    _DMX_Unlock();

    return fgOpen;
}

#if defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)
//-----------------------------------------------------------------------------
/** _DMX_PVRPlay_SetTSOut
 *  For emulation
 */
//-----------------------------------------------------------------------------
BOOL _DMX_PVRPlay_SetTSOut(UINT8 u1TsOutIdx, BOOL fgEnable)
{
    //UINT32 u4Cfg;

    if (fgEnable)
    {
    }
    else
    {
    }

    _fgTSOut[u1TsOutIdx] = fgEnable;

    return TRUE;
}

#endif  // defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)

