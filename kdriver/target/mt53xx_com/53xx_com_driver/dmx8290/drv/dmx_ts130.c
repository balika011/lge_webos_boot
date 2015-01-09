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
 * $RCSfile: dmx_vcode.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_vcode.c
 *  Demux driver - Video start code
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_mm.h"
#include "dmx_debug.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_os.h"

LINT_EXT_HEADER_END

#ifdef CC_DMX_TS130

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DMX_TS130_SYNC_COUNT        (3)
#define DMX_TS130_SYNC_BUFSIZE      (DMX_TS130_SYNC_COUNT * 130)
#define DMX_TS130_PID_BUFSIZE       (512 * 1024)
#define DMX_TS134_PID_BUFSIZE       (512 * 1024)
#define DMX_TS130_DUMMY_PKTCOUNT    (5)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
// Store PID value
static UINT16 _au2Dmx130PidArray[DMX_NUM_PID_INDEX];
static UINT8 _u1Dmx130PidCount = 0;

// Following 2 variables handle data chunck that not 130 alignment (due to lost sync)
static UINT32 _u4Dmx130Remainder;
static UINT32 _u4Dmx130PktBufStart, _u4Dmx130PktBufEnd;

// Framer sync buffer
static UINT32 _u4Dmx130SyncBufStart, _u4Dmx130SyncBufEnd, _u4Dmx130SyncWp, _u4Dmx130SyncRp;
static BOOL _fgDmx130FramerSync;
static UINT8 _u1Dmx130LastToggleBit;

// Pid filter buffer
static UINT32 _u4Dmx130PidBufStart, _u4Dmx130PidBufEnd, _u4Dmx130PidBufWp, _u4Dmx130PidBufRp;

static UINT8 _u1Dmx130Pidx = 0;

static HANDLE_T _hDmxDmx130Sema = NULL_HANDLE;  // Make sure _rDmxMMData is synchronization

// Dummy packet buffer
static UINT32 _u4Dmx130DummyBufStart, _u4Dmx130DummyBufEnd;
static UINT32 _u4Dmx134PidBufStart, _u4Dmx134PidBufEnd;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// _DmxTS130GetData
//-----------------------------------------------------------------------------
static UINT8 _DmxTS130GetData(UINT32 u4DataAddr, UINT32 u4DataOffset,
                              UINT32 u4BufStart, UINT32 u4BufEnd)
{
    UINT32 u4BufLen, u4BufPointer;
    UINT8 u1Data;

    u4BufLen = u4BufEnd - u4BufStart;
    u4BufPointer = u4DataAddr + u4DataOffset;
    if(u4BufPointer >= u4BufEnd)
    {
        u4BufPointer -= u4BufLen;
    }

    u1Data = (*(UINT8*)(u4BufPointer));

    return u1Data;
}


//-----------------------------------------------------------------------------
// _DmxTS130AdvanceAddr
//-----------------------------------------------------------------------------
static UINT32 _DmxTS130AdvanceAddr(UINT32 u4Addr, UINT32 u4Advance,
                                   UINT32 u4BufStart, UINT32 u4BufEnd)
{
    UINT32 u4RetAddr;

    u4RetAddr = u4Addr + u4Advance;
    if(u4RetAddr >= u4BufEnd)
    {
        u4RetAddr -= (u4BufEnd - u4BufStart);
    }

    return u4RetAddr;
}


//-----------------------------------------------------------------------------
// _DmxTS130GenDummyPacket
//-----------------------------------------------------------------------------
static void _DmxTS130GenDummyPacket(UINT16 u2Pid)
{
    UINT8 *pu1Data, i, u1Toggle;

    ASSERT(_u4Dmx130DummyBufStart != 0);

    x_memset((void*)_u4Dmx130DummyBufStart, 0xFF, (188 * DMX_TS130_DUMMY_PKTCOUNT));
    pu1Data = (UINT8*)_u4Dmx130DummyBufStart;
    u1Toggle = 0;
    for(i=0; i<DMX_TS130_DUMMY_PKTCOUNT; i++)
    {
        pu1Data[(188*i) + 0] = ((u2Pid & 0xF00) >> 8) | u1Toggle;
        pu1Data[(188*i) + 1] = (u2Pid & 0xFF);
        //pu1Data[(188*i) + 2] = (i << 4) | 0x1;
        pu1Data[(188*i) + 2] = (i << 4) | 0x02;

        u1Toggle = (~u1Toggle) & 0x80;
    }
}


//-----------------------------------------------------------------------------
// _DmxTS130DoFilter
//-----------------------------------------------------------------------------
static BOOL _DmxTS130DoFilter(UINT16 u2Pid)
{
    UINT8 i;

    for(i=0; i<_u1Dmx130PidCount; i++)
    {
        if((_au2Dmx130PidArray[i] & 0xFFF) == u2Pid)
        {
            return TRUE;
        }
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
// _DmxTs130CopyPacket
//-----------------------------------------------------------------------------
static UINT32 _DmxTs130CopyPacket(UINT32 u4Addr, UINT32 u4PktCount,
                                UINT32 u4BufStart, UINT32 u4BufEnd)
{
    UINT32 i, u4DataPtr, u4CopyCount;
    UINT16 u2Pid;
    UINT8 au1Data[2];

    ASSERT(u4PktCount > 0);
    ASSERT((u4Addr>=u4BufStart)&&(u4Addr<u4BufEnd));

    u4CopyCount = 0;
    u4DataPtr = u4Addr;
    for(i=0; i<u4PktCount; i++)
    {
        au1Data[0] = _DmxTS130GetData(u4DataPtr, 0, u4BufStart, u4BufEnd);
        au1Data[1] = _DmxTS130GetData(u4DataPtr, 1, u4BufStart, u4BufEnd);
        u2Pid = (((UINT16)au1Data[0] & 0xF) << 8) | (UINT16)au1Data[1];
        if(_DmxTS130DoFilter(u2Pid))
        {
            UNUSED(_DMX_CopyRingBuffer(_u4Dmx130PidBufWp, _u4Dmx130PidBufStart, _u4Dmx130PidBufEnd,
                                       u4DataPtr, u4BufStart, u4BufEnd, 130));
            _u4Dmx130PidBufWp = _DmxTS130AdvanceAddr(_u4Dmx130PidBufWp, 188,
                                            _u4Dmx130PidBufStart, _u4Dmx130PidBufEnd);
            u4CopyCount += 188;
        }
        u4DataPtr = _DmxTS130AdvanceAddr(u4DataPtr, 130, u4BufStart, u4BufEnd);
    }

    return u4CopyCount;
}



//-----------------------------------------------------------------------------
// _DmxTs130DDIMove
//-----------------------------------------------------------------------------
static BOOL _DmxTs130DDIMove(UINT8 u1TsIdx, UINT32 u4Addr, UINT32 u4SendSize,
                             UINT32 u4BufStart, UINT32 u4BufEnd)
{
    UINT32 u4Reg;
    INT32 i4Ret;

    // Set DMX
    u4Reg = RV_SETTING_CTRL;
    u4Reg &= 0xFFFF0000;
    u4Reg |= ((u4SendSize / 188) & 0xFFFF);
    RV_SETTING_CTRL = u4Reg;

    if((u4Addr+u4SendSize) < u4BufEnd)
    {
        HalFlushInvalidateDCacheMultipleLine(u4Addr,u4SendSize);
    }
	else
	{
	    HalFlushInvalidateDCacheMultipleLine(u4Addr,(u4BufEnd-u4Addr));
		HalFlushInvalidateDCacheMultipleLine(u4BufStart,(u4Addr+u4SendSize-u4BufEnd));
	}
    
/*
    //if((*(volatile UINT32*)_u4Dmx130PidBufRp) == 0x000464b0)
    if((*(volatile UINT32*)u4Addr) == 0x00046530)
    {
        u4Reg++;
    }
*/
    // Set DDI
    if(!_DMX_DDI_WaitTransfer(u1TsIdx, u4BufStart, u4BufEnd, u4Addr, u4SendSize, 0)) // ignore 3
    {
        LOG(3, "DDI transaction failed!\n");
        return FALSE;
    }

    i4Ret = x_sema_lock_timeout(_hDmxDmx130Sema, 5000);
    if(i4Ret != OSR_OK)
    {
        if(i4Ret == OSR_TIMEOUT)
        {
            LOG(3, "Timeout\n");
            //_DMX_DDI_HWReset();
            return FALSE;
        }
        else
        {
            ASSERT(0);
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
// _DmxTs130SendPacket
//-----------------------------------------------------------------------------
static BOOL _DmxTs130SendPacket(UINT8 u1TsIdx, UINT32 u4Addr, UINT32 u4PktCount,
                                UINT32 u4BufStart, UINT32 u4BufEnd)
{
    UINT32 u4SendSize;

    u4SendSize = _DmxTs130CopyPacket(u4Addr, u4PktCount, u4BufStart, u4BufEnd);
    if(u4SendSize == 0)
    {
        return TRUE;
    }

    ASSERT(DATASIZE(_u4Dmx130PidBufRp, _u4Dmx130PidBufWp, (_u4Dmx130PidBufEnd - _u4Dmx130PidBufStart)) ==
        u4SendSize);

    //LOG(3, "u4SendSize=0x%x, Rp=0x%x, Wp=0x%x", u4SendSize, _u4Dmx130PidBufRp, _u4Dmx130PidBufWp);
    	
    if(!_DmxTs130DDIMove(u1TsIdx, _u4Dmx130PidBufRp, u4SendSize, _u4Dmx130PidBufStart, _u4Dmx130PidBufEnd))
    {
        return FALSE;
    }

    _u4Dmx130PidBufRp = _DmxTS130AdvanceAddr(_u4Dmx130PidBufRp, u4SendSize,
                                             _u4Dmx130PidBufStart, _u4Dmx130PidBufEnd);

    return TRUE;
}


//-----------------------------------------------------------------------------
// _DmxTS130FramerLockCheck
//-----------------------------------------------------------------------------
static BOOL _DmxTS130FramerLockCheck(UINT8 u1TsIdx, UINT8 u1Data)
{
    UINT32 u4DataSize;
    UINT8 au1Data[3], u1ToggleData;
    BOOL fgRet = FALSE;

    ASSERT(_u4Dmx130SyncBufStart!=0);
    ASSERT((_u4Dmx130SyncWp>=_u4Dmx130SyncBufStart)&&(_u4Dmx130SyncWp<_u4Dmx130SyncBufEnd));
    ASSERT((_u4Dmx130SyncRp>=_u4Dmx130SyncBufStart)&&(_u4Dmx130SyncRp<_u4Dmx130SyncBufEnd));

    if(_fgDmx130FramerSync)
    {
        u1ToggleData = u1Data >> 7;
        if(_u1Dmx130LastToggleBit != (u1ToggleData & 0x1))
        {
            _u1Dmx130LastToggleBit = u1ToggleData & 0x1;
            return TRUE;
        }
        else
        {
            // ******** From sync to not sync ********
            _fgDmx130FramerSync = FALSE;
            _u4Dmx130SyncWp = _u4Dmx130SyncBufStart;
            _u4Dmx130SyncRp = _u4Dmx130SyncBufStart;
        }
    }

    // Following handle not sync
    *(UINT8*)_u4Dmx130SyncWp = u1Data;
    _u4Dmx130SyncWp = _DmxTS130AdvanceAddr(_u4Dmx130SyncWp, 1, _u4Dmx130SyncBufStart,
                                           _u4Dmx130SyncBufEnd);
    u4DataSize = DATASIZE(_u4Dmx130SyncRp, _u4Dmx130SyncWp,
                          (_u4Dmx130SyncBufEnd - _u4Dmx130SyncBufStart));

    if(u4DataSize >= ((130 * 2) + 1))
    {
        // Search sync
        au1Data[0] = (_DmxTS130GetData(_u4Dmx130SyncRp, 0, _u4Dmx130SyncBufStart, _u4Dmx130SyncBufEnd)) >> 7;
        au1Data[1] = (_DmxTS130GetData(_u4Dmx130SyncRp, 130, _u4Dmx130SyncBufStart, _u4Dmx130SyncBufEnd)) >> 7;
        au1Data[2] = (_DmxTS130GetData(_u4Dmx130SyncRp, 260, _u4Dmx130SyncBufStart, _u4Dmx130SyncBufEnd)) >> 7;

        if(((au1Data[0]&0x1) == (au1Data[2]&0x1)) && ((au1Data[0]&0x1) != (au1Data[1]&0x1)))
        {
            // ******** From not sync to sync ********
            _u1Dmx130LastToggleBit = au1Data[2] & 0x1;
            fgRet = _DmxTs130SendPacket(u1TsIdx, _u4Dmx130SyncRp, 2, _u4Dmx130SyncBufStart, _u4Dmx130SyncBufEnd);
            _u4Dmx130SyncRp = _DmxTS130AdvanceAddr(_u4Dmx130SyncRp, (2 * 130), _u4Dmx130SyncBufStart,
                                                   _u4Dmx130SyncBufEnd);
            _fgDmx130FramerSync = TRUE;
            return fgRet;
        }
        else
        {
            _u4Dmx130SyncRp = _DmxTS130AdvanceAddr(_u4Dmx130SyncRp, 1, _u4Dmx130SyncBufStart,
                                                   _u4Dmx130SyncBufEnd);
        }
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
// _DmxTS130PacketHandler
//-----------------------------------------------------------------------------
static BOOL _DmxTS130PacketHandler(UINT8 u1TsIdx, UINT32 u4Addr, UINT32 u4Size,
                                 UINT32 u4BufStart, UINT32 u4BufEnd)
{
    UINT32 u4Count, u4DataPtr, u4RemainSize;
    BOOL fgLock, fgPreLock;
    UINT32 i, u4PreDataPtr;
    UINT8 *pu1DataPtr, u1Data;

    ASSERT(_u4Dmx130Remainder < 130);
    ASSERT(u4Size >= 130);
    if(u4Size < 130)
    {
        return FALSE;
    }

    u4DataPtr = u4Addr;

    // If last remainder not equal zero
    if(_u4Dmx130Remainder > 0)
    {
        pu1DataPtr = (UINT8*)_u4Dmx130PktBufStart;
        fgLock = _DmxTS130FramerLockCheck(u1TsIdx, pu1DataPtr[0]);
        if(fgLock)
        {
            // Copy and send one packet data
            UNUSED(_DMX_CopyRingBuffer(_u4Dmx130PktBufStart + _u4Dmx130Remainder,
                                       _u4Dmx130PktBufStart, _u4Dmx130PktBufEnd,
                                       u4DataPtr, u4BufStart, u4BufEnd, 130 - _u4Dmx130Remainder));
            if(!_DmxTs130SendPacket(u1TsIdx, _u4Dmx130PktBufStart, 1, _u4Dmx130PktBufStart, _u4Dmx130PktBufEnd))
            {
                LOG(1,"Ts 130 byte send data fail.\n");
            }
        }
        else
        {
            if(_u4Dmx130Remainder > 1)
            {
                // Accumulate following data to sync buffer
                for(i=1; i<_u4Dmx130Remainder; i++)
                {
                    UNUSED(_DmxTS130FramerLockCheck(u1TsIdx, pu1DataPtr[i]));
                }
            }
        }
    }

    u4RemainSize = u4Size;
    if(_u4Dmx130Remainder > 0)
    {
        u4DataPtr = _DmxTS130AdvanceAddr(u4DataPtr, 130 - _u4Dmx130Remainder, u4BufStart, u4BufEnd);
        u4RemainSize -= (130 - _u4Dmx130Remainder);
    }

    _u4Dmx130Remainder = 0;

    u4Count = 0;
    fgPreLock = FALSE;
    u4PreDataPtr = u4DataPtr;
    while(u4RemainSize >= 130)
    {
        u1Data = _DmxTS130GetData(u4DataPtr, 0, u4BufStart, u4BufEnd);
        fgLock = _DmxTS130FramerLockCheck(u1TsIdx, u1Data);
        if(fgLock)
        {
            if(!fgPreLock)
            {
                // **** From unlock to lock
                u4PreDataPtr = u4DataPtr;
                ASSERT(u4Count == 0);
            }
            fgPreLock = TRUE;
            u4DataPtr = _DmxTS130AdvanceAddr(u4DataPtr, 130, u4BufStart, u4BufEnd);
            u4RemainSize -= 130;
            u4Count += 130;
        }
        else
        {
            if(fgPreLock)
            {
                // ***** From lock to unlock, move data to DMX
                ASSERT(u4Count >= 130);
                ASSERT((u4Count % 130) == 0);
                if(!_DmxTs130SendPacket(u1TsIdx, u4PreDataPtr, (u4Count / 130), u4BufStart, u4BufEnd))
               	{
               	    LOG(1,"TS 130 byte move data fail 2....\n"); 
               	}
                u4PreDataPtr = u4DataPtr;
                u4Count = 0;
            }

            fgPreLock = FALSE;
            // sync lost
            u4DataPtr = _DmxTS130AdvanceAddr(u4DataPtr, 1, u4BufStart, u4BufEnd);
            u4RemainSize--;
        }
    }  // while

    if(u4Count > 0)
    {
        if(!_DmxTs130SendPacket(u1TsIdx, u4PreDataPtr, (u4Count / 130), u4BufStart, u4BufEnd))
        {
            LOG(1,"TS 130byte move data fail 3.....\n");
        }
    }

    _u4Dmx130Remainder = u4RemainSize;
    if(u4RemainSize > 0)
    {
        UNUSED(_DMX_CopyRingBuffer(_u4Dmx130PktBufStart, _u4Dmx130PktBufStart, _u4Dmx130PktBufEnd,
                                   u4DataPtr, u4BufStart, u4BufEnd, u4RemainSize));
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_TS130_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TS130_Init(void)
{
    static BOOL _fgInit = FALSE;
    UINT32 u4Addr, i;
    UINT32 u4DummyBufsize = 0;
    
    if(!_fgInit)
    {
        u4Addr = (UINT32)BSP_AllocAlignedDmaMemory(DMX_TS130_PID_BUFSIZE, 32);
        if(u4Addr == 0)
        {
            return FALSE;
        }
        _u4Dmx130PidBufStart = VIRTUAL(u4Addr);
        _u4Dmx130PidBufEnd = VIRTUAL(u4Addr + DMX_TS130_PID_BUFSIZE);
        _u4Dmx130PidBufWp = VIRTUAL(u4Addr);
        _u4Dmx130PidBufRp = VIRTUAL(u4Addr);

        u4Addr = (UINT32)BSP_AllocAlignedDmaMemory((DMX_TS130_SYNC_BUFSIZE + (16 - (DMX_TS130_SYNC_BUFSIZE%16))), 32);
        if(u4Addr == 0)
        {
            UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130PidBufStart));
            return FALSE;
        }
        _u4Dmx130SyncBufStart = VIRTUAL(u4Addr);
        _u4Dmx130SyncBufEnd = VIRTUAL(u4Addr + (DMX_TS130_SYNC_BUFSIZE + (16 - (DMX_TS130_SYNC_BUFSIZE%16))));
        _u4Dmx130SyncWp = VIRTUAL(u4Addr);
        _u4Dmx130SyncRp = VIRTUAL(u4Addr);

        u4Addr = (UINT32)BSP_AllocAlignedDmaMemory(256, 32);
        if(u4Addr == 0)
        {
            UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130PidBufStart));
            UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130SyncBufStart));
            return FALSE;
        }
        _u4Dmx130PktBufStart = VIRTUAL(u4Addr);
        _u4Dmx130PktBufEnd = VIRTUAL(u4Addr + 256);
        u4DummyBufsize =  (DMX_TS130_DUMMY_PKTCOUNT * 188) + 32;
        
        u4Addr = (UINT32)BSP_AllocAlignedDmaMemory(
                            (u4DummyBufsize + (16 - u4DummyBufsize%16)), 32);
        if(u4Addr == 0)
        {
            UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130PidBufStart));
            UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130SyncBufStart));
            UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130PktBufStart));
            return FALSE;
        }
        _u4Dmx130DummyBufStart = VIRTUAL(u4Addr);
        _u4Dmx130DummyBufEnd = VIRTUAL(u4Addr +  (u4DummyBufsize + (16 - u4DummyBufsize%16)));

		u4Addr =(UINT32)BSP_AllocAlignedDmaMemory(DMX_TS134_PID_BUFSIZE,32);

		if(u4Addr == 0)
		{
			UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130PidBufStart));
			UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130SyncBufStart));
			UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130PktBufStart));
			UNUSED(BSP_FreeAlignedDmaMemory(_u4Dmx130DummyBufStart));
			return FALSE;
		}

        _u4Dmx134PidBufStart = VIRTUAL(u4Addr);
		_u4Dmx134PidBufEnd = VIRTUAL(u4Addr + DMX_TS134_PID_BUFSIZE);

        for(i=0; i<DMX_NUM_PID_INDEX; i++)
        {
            _au2Dmx130PidArray[i] = 0xFFFF;
        }
        _u1Dmx130PidCount = 0;

        _fgDmx130FramerSync = FALSE;

        VERIFY(OSR_OK ==x_sema_create(&_hDmxDmx130Sema,
                        X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));

        RV_SETTING_CTRL = 0x10000;

        _fgInit = TRUE;
    }

    RV_SETTING_CTRL = 0x10000;	

	//clear 130 byte golbal region
    x_memset((void*)RV_SETTING_BASE, 0, RV_SETTING_SIZE * 4);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_TS130_SetPidArray
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TS130_SetPidArray(void)
{
    PID_STRUCT_T *prPid;
    UINT32 u4Reg;
    UINT8 i, u4Count;

    // Reset
    RV_SETTING_CTRL = 0x10000;
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        _au2Dmx130PidArray[i] = 0xFFFF;
    }
    _u1Dmx130PidCount = 0;

    // Fill value
    u4Count = 0;
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        prPid = _DMX_GetPidStruct(i);
        if(prPid != NULL)
        {
            if((prPid->fgEnable) && (prPid->fgTS130))
            {
                _u1Dmx130Pidx = i;
                _au2Dmx130PidArray[_u1Dmx130PidCount++] = prPid->u2Pid;
                u4Reg = (UINT32)(prPid->u2Pid & 0xFFF) | (((UINT32)(i)) << 24);
                *((volatile UINT32*)(RV_SETTING_BASE + 4 + (u4Count * 4))) = u4Reg;
                u4Count++;
            }
        }
    }
    u4Reg = RV_SETTING_CTRL;
    u4Reg &= 0xFFFFFF;
    u4Reg |= ((u4Count & 0xFF) << 24);
    RV_SETTING_CTRL = u4Reg;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_TS130_MoveData
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TS130_MoveData(DMX_MM_DATA_T *prData)
{
    UINT32 u4Reg;

    if(prData == NULL)
    {
        return FALSE;
    }

    ASSERT(prData->u1Idx < DMX_FRAMER_COUNT);

    u4Reg = ((UINT32)_u1Dmx130Pidx << 16) | (UINT32)0x20;
    DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID + prData->u1Idx, u4Reg);

    if(!_DmxTS130PacketHandler(prData->u1Idx, prData->u4StartAddr, prData->u4FrameSize,
                               prData->u4BufStart, prData->u4BufEnd)) {
        return FALSE;
    }

    if(prData->fgEOS)
    {
        // Send dummy packet for segment complete
        if(!_DDI_SendAllDummyPackets(prData->u1Idx))
        {
            return FALSE;
        }

        // Wait uCode really finish handle final data chunk
        // Because EOS, delay 10ms is not easy detectable
        x_thread_delay(10);

        if(!_DDI_Sync_VideoWp(prData->u1Idx))
        {
            return FALSE;
        }

        if(!_DDI_SendTSEOS(prData->u1Idx))
        {
            return FALSE;
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_TS130_RequestReset
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TS130_RequestReset(UINT8 u1TsIdx)
{
    _fgDmx130FramerSync = FALSE;

    // Reset DMX buffer
    if(!_DDI_TsResetDmxBuffers(u1TsIdx))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_TS130_ISR
 */
//-----------------------------------------------------------------------------
void _DMX_TS130_ISR(void)
{
    VERIFY(OSR_OK == x_sema_unlock(_hDmxDmx130Sema));
}

//-----------------------------------------------------------------------------
/** _DMX_TS130_SendDummyPacket
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TS130_SendDummyPacket(UINT8 u1TsIdx, UINT8 u1Pidx)
{
    PID_STRUCT_T *prPidStruct;
    DMX_TSFMT_T eTSFmt;

    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    eTSFmt = _DMX_MUL_GetTSMode(_DMX_MUL_GetPidxInstId(u1Pidx));
    
	if (_DDI_UsePVR(u1TsIdx))
    {
        _DMX_PVRPlay_SetMoveMode(0, eTSFmt, FALSE);
        if (!_DMX_PVRPlay_SetPortEx(0, u1TsIdx, u1Pidx, DMX_PVRPLAY_PORT_DBM, FALSE, FALSE))
        {
            return FALSE;
        }
    }
    else
    {
        if (u1TsIdx == 0)
        {
            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER0, u1Pidx, TRUE, FALSE);
        }
        else if (u1TsIdx == 1)
        {
            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, u1Pidx, TRUE, FALSE);
        }
        else if (u1TsIdx == 2)
        {
            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER2, u1Pidx, TRUE, FALSE);
        }
        else if (u1TsIdx == 3)
        {
            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER3, u1Pidx, TRUE, FALSE);
        }
        else
        {
            return FALSE;
        }
        
        if(!_DMX_SetFramerPktSize(u1TsIdx, 188))
        {
            return FALSE;
        }
        
        if(!_DMX_DDI_SetPacketSize(u1TsIdx, 188))
        {
            return FALSE;
        }
    }
    
    if(prPidStruct != NULL)
    {
        _DmxTS130GenDummyPacket(prPidStruct->u2Pid);
        LOG(3, "_DmxTS130GenDummyPacket: pid=0x%x\n", prPidStruct->u2Pid);
        if(!_DmxTs130DDIMove(u1TsIdx, _u4Dmx130DummyBufStart, 188 * DMX_TS130_DUMMY_PKTCOUNT, 
                         _u4Dmx130DummyBufStart, _u4Dmx130DummyBufEnd))
        {
            return FALSE;
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_TS134_MoveData
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TS134_MoveData(DMX_MM_DATA_T *prData)
{
    DMX_MM_DATA_T u4TsMMData;
	UINT32 u4PacketCounter;
	UINT32 u4PacketAddr = 0;
	UINT32 i;

    if(prData == NULL)
    {
        return FALSE;
    }

    ASSERT(prData->u1Idx < DMX_FRAMER_COUNT);

    x_memset((void*)_u4Dmx134PidBufStart, 0xFF, DMX_TS134_PID_BUFSIZE);

    if((prData->u4FrameSize%134)!=0)
    {
        LOG(3,"Frame size is not 134 byte align.\n");
        return FALSE;
    }
	
    u4PacketCounter = (prData->u4FrameSize/134);

    u4PacketAddr = prData->u4StartAddr +4;

	for(i=0;i<u4PacketCounter;i++)
	{
	     if((u4PacketAddr+134) < prData->u4BufEnd)
	     {
	         UNUSED(_DMX_CopyRingBuffer((_u4Dmx134PidBufStart + (i*130)), _u4Dmx134PidBufStart, _u4Dmx134PidBufEnd, 
			 	u4PacketAddr, prData->u4BufStart, prData->u4BufEnd,130));
			 u4PacketAddr =  u4PacketAddr + 134;
	     }
		 else if(prData->u4BufEnd < (u4PacketAddr+130))
		 {
		     UNUSED(_DMX_CopyRingBuffer((_u4Dmx134PidBufStart + (i*130)), _u4Dmx134PidBufStart, _u4Dmx134PidBufEnd, 
			 	u4PacketAddr, prData->u4BufStart, prData->u4BufEnd,(prData->u4BufEnd-u4PacketAddr)));
			 UNUSED(_DMX_CopyRingBuffer((_u4Dmx134PidBufStart + (i*130))+(prData->u4BufEnd-u4PacketAddr), _u4Dmx134PidBufStart, _u4Dmx134PidBufEnd, 
			 	prData->u4BufStart, prData->u4BufStart, prData->u4BufEnd,130-(prData->u4BufEnd-u4PacketAddr)));
			 u4PacketAddr = prData->u4BufStart + 134 - (prData->u4BufEnd-u4PacketAddr);
		 }
		 else if(((u4PacketAddr+130)<=(prData->u4BufEnd))&&((prData->u4BufEnd)<(u4PacketAddr+134)))
		 {
		     UNUSED(_DMX_CopyRingBuffer((_u4Dmx134PidBufStart + (i*130)), _u4Dmx134PidBufStart, _u4Dmx134PidBufEnd, 
			 	u4PacketAddr, prData->u4BufStart, prData->u4BufEnd,130));
			 u4PacketAddr = prData->u4BufStart + 134 - (prData->u4BufEnd-u4PacketAddr);
		 }
		 else if((u4PacketAddr+134)== prData->u4BufEnd )
		 {
		     UNUSED(_DMX_CopyRingBuffer((_u4Dmx134PidBufStart + (i*130)), _u4Dmx134PidBufStart, _u4Dmx134PidBufEnd, 
			 	u4PacketAddr, prData->u4BufStart, prData->u4BufEnd,130));
			 u4PacketAddr =  prData->u4BufStart;		     
		 }
		 else
		 {
		     LOG(3,"Invalid u4PacketAddr 0x%x\n",u4PacketAddr);
			 break;
		 }
	}
	
	HalFlushInvalidateDCacheMultipleLine(_u4Dmx134PidBufStart,(prData->u4FrameSize-4*u4PacketCounter));

    u4TsMMData.u4BufStart = _u4Dmx134PidBufStart;
	u4TsMMData.u4BufEnd = _u4Dmx134PidBufEnd;
	u4TsMMData.u4StartAddr = _u4Dmx134PidBufStart;
	u4TsMMData.u4FrameSize = prData->u4FrameSize - 4*u4PacketCounter;
	u4TsMMData.fgEOS = prData->fgEOS;
	u4TsMMData.fgFrameHead = prData->fgFrameHead;
	u4TsMMData.fgIgnoreData = prData->fgIgnoreData;
	u4TsMMData.u1Idx = prData->u1Idx;
	u4TsMMData.u4Dts = prData->u4Dts;
	u4TsMMData.u4Pts = prData->u4Pts;

	return _DMX_TS130_MoveData(&u4TsMMData);
	
}

#endif //CC_DMX_TS130

