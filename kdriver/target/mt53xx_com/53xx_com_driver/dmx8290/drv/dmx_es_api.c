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
 * $RCSfile: dmx_es_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_es_api.c
 *  Demux elementary stream driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"

LINT_EXT_HEADER_END

#ifdef CC_TRUSTZONE_SUPPORT
#include "tz_if.h"
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT8 _u1DmxESIdx = 0x0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _Dmx_ES_SetBuffer
 */
// Possible side effect:
//      1. PID data memory
//      2. the data pointed to by the argement prPidStruct
//-----------------------------------------------------------------------------
static BOOL _Dmx_ES_SetBuffer(UINT8 u1Pidx, const DMX_ES_T *prES,
                              PID_STRUCT_T *prPidStruct)
{
    UINT32 u4BufStart, u4PhyBufStart, u4W0 = 0;

    ASSERT(u1Pidx < DMX_MM_STRUCT_NUM);
    ASSERT(prES != NULL);
    ASSERT(prPidStruct != NULL);

    if (prES->ePidType == DMX_PID_TYPE_NONE)
    {
        return TRUE;    // No PID type, can't setup buffer
    }

    // Check if the buffer has been allocated.  (Avoid memory leakage.)
    if ((prPidStruct->u4BufStart != 0) || (prPidStruct->u4HeaderBufAddr != 0))
    {
        return FALSE;
    }

    if ((prES->ePidType == DMX_PID_TYPE_PSI) ||
        (prES->ePidType == DMX_PID_TYPE_PES))
    {
        LOG(3, "ES/VC1 data should not be of this type!\n");
        return FALSE;
    }
    else if (prES->ePidType == DMX_PID_TYPE_ES_OTHER)
    {
        // TBD
        return FALSE;
    }
    else
    {
        // [ Wrod 0 of the PID data memory ]
        // bit 0: don't care
        // bit 1: don't care (valid bit)
        // bit 2: don't care (output mode)
        // bit 3: don't care
        // bit 4: Toggle bit (for use along with bit 8 of PID index table)
        // bit 5: don't care (PID disable)
        // bit 6: To CD-FIFO (1: hardware processing, 0: simply written out)
        // bit 7: Enable picture finder
        // bit [15:8]: AES mode
        // bit [23:16]: Header Count (for uP's internal use)
        // bit [31:24]: Decryption Scheme (0: bypass)
        u4W0 = (UINT32)((1 << 6) |
                (((prES->ePidType == DMX_PID_TYPE_ES_VIDEO) ? 1 : 0) << 7));

        if(prES->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            //set byte2 to 0xff for default picture search by TS_IDX
            u4W0 |= 0x00FF0000;
        }
        // Setup header buffer
        u4PhyBufStart = (UINT32)BSP_AllocAlignedDmaMemory(PES_HEADER_BUF_SIZE,
                                                DMX_HEADER_FIFO_ALIGNMENT);
        if (u4PhyBufStart == 0)
        {
            LOG(3, "%s:%d: Memory allocation failed!\n", __FILE__, __LINE__);
            return FALSE;
        }

        _DMX_Lock();
        u4BufStart = VIRTUAL(u4PhyBufStart);
        prPidStruct->u4HeaderBufAddr = u4BufStart;
        prPidStruct->u4NextPic = 0;
        PID_S_W(u1Pidx, 10) = u4PhyBufStart;    // (Header) FIFO start
        PID_S_W(u1Pidx, 12) = u4PhyBufStart;    // (Header) Start
        PID_S_W(u1Pidx, 13) = u4PhyBufStart;    // (Header) Write pointer
        PID_S_W(u1Pidx, 14) = u4PhyBufStart;    // (Header) Read pointer
        PID_S_W(u1Pidx, 11) = (u4PhyBufStart + PES_HEADER_BUF_SIZE) - 1;  // end address of Header buffer
        // FIXME
        // Temporary solution: Write 2 to disable "Decoder Message Queue Usage".
        PID_S_W(u1Pidx, 19) = (PID_S_W(u1Pidx, 19) & 0xFFFF0000) | 2;
        _DMX_Unlock();
    }

    // FIFO starting address
    if (prES->fgAllocateBuffer)
    {
        u4PhyBufStart = (UINT32)BSP_AllocAlignedDmaMemory(prES->u4BufSize,
                                                DMX_ES_FIFO_ALIGNMENT);
        if (u4PhyBufStart == 0)
        {
            VERIFY(BSP_FreeAlignedDmaMemory(
                    PHYSICAL(prPidStruct->u4HeaderBufAddr)));
            
            _DMX_Lock();
            prPidStruct->u4HeaderBufAddr = 0;
            _DMX_Unlock();
    
            LOG(3, "%s:%d: Memory allocation failed!\n", __FILE__, __LINE__);
            return FALSE;
        }
    }
    else
    {
        ASSERT(prES->u4BufAddr != 0);
        Printf("%s - Pidx %d sets buffer addr 0x%08lx\n", __FUNCTION__, u1Pidx, prES->u4BufAddr);
        /* Add PHYSICAL for safety check, remove it in the future */
        u4PhyBufStart = PHYSICAL(prES->u4BufAddr);
    }

    _DMX_Lock();
    // Preserve the Toggle bit.
    PID_S_W(u1Pidx, 0) = (PID_S_W(u1Pidx, 0) & (1 << 4)) | u4W0;
    // Encryption length: It must be set to the correct value for each round
    // since uP won't trigger the PES INT until this length of data is consumed.
    PID_S_W(u1Pidx, 1) = 0;
    // Offset (not used in uCode for now)
    PID_S_W(u1Pidx, 2) = 0;
    // Skipped-byte count = 0 (new in 3.04);
    // Pre-byte = 0 (need re-init);
    // # of start codes = 0 (need re-init);
    // Trigger: inform uP of the first packet (for decryption), init to 0.
    PID_S_W(u1Pidx, 3) = 0;
    PID_S_W(u1Pidx, 4) = 0x0;     // Reset previous payload counter to 0
    u4BufStart    = VIRTUAL(u4PhyBufStart);
    prPidStruct->u4BufStart       = u4BufStart;
    prPidStruct->u4Wp             = u4BufStart;
    prPidStruct->u4Rp             = u4BufStart;
    prPidStruct->u4SectionRp      = u4BufStart;
    prPidStruct->u4PesRp          = u4BufStart;
    prPidStruct->u4BufEnd         = (u4BufStart + prES->u4BufSize) - 1;
    prPidStruct->u4BufLen         = prES->u4BufSize;    // FIFO size
    prPidStruct->fgAllocateBuffer = prES->fgAllocateBuffer;
    prPidStruct->ePidType         = prES->ePidType;
    prPidStruct->u1ChannelId = prES->u1ChannelId;
    _DMX_Unlock();
    
#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_MT5890)
    {
        DMX_MMSECURITY_STRUCT_T rDataInfo;

        x_memset((void*)&rDataInfo, 0, sizeof(rDataInfo));
        rDataInfo.e_SetType = DMX_MMSECURITY_SETBUFFER;
        rDataInfo.u1PidIdx = u1Pidx;
        rDataInfo.u4BufAddr = u4PhyBufStart;
        rDataInfo.u4BufEnd = (u4PhyBufStart + prES->u4BufSize);
        LOG(2, "TZ_DMX_SetMMSecurity ES Secure Set Buff!\n");
        if (!TZ_DMX_SetMMSecurity(&rDataInfo, sizeof(rDataInfo)))
        {
            LOG(0, "Fail to Secure Set Buff!\n");
            goto lbErrorExit;
        }
        // FIFO end address
    }
#else
    _DMX_Lock();
    PID_S_W(u1Pidx, 5) = u4PhyBufStart;         // FIFO start
    PID_S_W(u1Pidx, 7) = u4PhyBufStart;         // PES_start
    PID_S_W(u1Pidx, 8) = u4PhyBufStart;         // Write pointer
    PID_S_W(u1Pidx, 9) = u4PhyBufStart;         // Read pointer
    PID_S_W(u1Pidx, 6) = (u4PhyBufStart + prES->u4BufSize) - 1; // End pointer
    _DMX_Unlock();
#endif
    

    if(prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if (!_DMX_AllocateVideoChannel(u1Pidx, prPidStruct->u1ChannelId))
        {
            goto lbErrorExit;
        }
    }

#ifdef CC_DMX_FLUSH_ALL
    HalFlushInvalidateDCache();
#else 
    /* We need to flush cache to avoid CPU overwriting the buffers 
     * handled by ftup when cache replacement happens
     */
    if(prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
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

    DMXPIDLOG(DMX_LOG_PID 6, u1Pidx, "Set pixd %u buffer: 0x%08x - 0x%08x\n", u1Pidx, u4BufStart,
            prPidStruct->u4BufEnd);

    return TRUE;

lbErrorExit:
    if (prPidStruct->u4HeaderBufAddr)
    {
        VERIFY(BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4HeaderBufAddr)));
            
        _DMX_Lock();
        prPidStruct->u4HeaderBufAddr = 0;
        _DMX_Unlock();
    }
    
    if (prPidStruct->fgAllocateBuffer && prPidStruct->u4BufStart)
    {
        VERIFY(BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4BufStart)));
    
        _DMX_Lock();
        prPidStruct->u4BufStart = 0;
        prPidStruct->fgAllocateBuffer = FALSE;
        _DMX_Unlock();
    }
    
        return FALSE;
}


//-----------------------------------------------------------------------------
/** _Dmx_ES_FreeBuffer
 *  Free PID buffer(s)
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_ES_FreeBuffer(UINT8 u1Pidx)
{
    UINT32 u4PhyAddr;
    PID_STRUCT_T* prPidStruct;

    ASSERT(u1Pidx < DMX_MM_STRUCT_NUM);

    prPidStruct = _DMX_GetPidStruct(u1Pidx);

    DMXPIDLOG(DMX_LOG_PID 6, u1Pidx, "Free pidx %u buffer\n", u1Pidx);

#ifdef DEBUG_MEMORY_INTRUSION

    if (_DmxIsProtectedMemory(prPidStruct))
    {
        VERIFY(_DmxResetProtectedMemory());
    }

#endif

    // Flush the header/payload buffers so that Demux u-code has the space to
    // output the remaining data of the current transaction without stalling
    // in Full-Pull mode.
    // FIXME: Set the pointers to 0 since the Full-Pull mode is not used any more.
    _DMX_Lock();
    u4PhyAddr = PID_S_W(u1Pidx, 8);         // Write pointer
    PID_S_W(u1Pidx, 9) = u4PhyAddr;         // Read pointer
    u4PhyAddr = PID_S_W(u1Pidx, 13);        // (Header) Write pointer
    PID_S_W(u1Pidx, 14) = u4PhyAddr;        // (Header) Read pointer
    _DMX_Unlock();

    // Delay 1 ms so that Demux has the time to finish the current transaction.
    x_thread_delay(1);

    // Free buffer
    if (prPidStruct->fgAllocateBuffer)
    {
        if (!BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4BufStart)))
        {
            LOG(3, "Failed to free PID buffer! pidx: %u, addr: 0x%08x\n",
                    u1Pidx, prPidStruct->u4BufStart);
            return FALSE;
        }
    }

    // Free header buffer
    if (!BSP_FreeAlignedDmaMemory(PHYSICAL(prPidStruct->u4HeaderBufAddr)))
    {
        LOG(3, "Failed to free PID header buffer! pidx: %u, addr: 0x%08x\n",
                u1Pidx, prPidStruct->u4HeaderBufAddr);
        return FALSE;
    }

    _DMX_Lock();
    // Update PID state
    prPidStruct->fgAllocateBuffer = FALSE;
    prPidStruct->u4BufStart = 0;
    prPidStruct->u4BufEnd = 0;
    prPidStruct->u4BufLen = 0;
    prPidStruct->u4Rp = 0;
    prPidStruct->u4Wp = 0;
    prPidStruct->u4HeaderBufAddr = 0;
    _DMX_Unlock();
    
    if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        // Reset pidx-to-channel mapping
        _DMX_FreeVideoChannel(u1Pidx);
    }
    
#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_MT5890)
    {
        DMX_MMSECURITY_STRUCT_T rDataInfo;

        x_memset((void*)&rDataInfo, 0, sizeof(rDataInfo));
        rDataInfo.e_SetType = DMX_MMSECURITY_FREEBUFFER;
        rDataInfo.u1PidIdx = u1Pidx;
        if (!TZ_DMX_SetMMSecurity(&rDataInfo, sizeof(rDataInfo)))
        {
            LOG(0, "Fail to Secure Free Buffer!\n");
            return FALSE;
        }
    }
#endif
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_ES_ResetPidBuffer
 *
 *  @param  u1Pidx          PID index
 *  @parm   prPidStruct     PID data structure
 *
 *  @retval TRUE            Sucessful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_ES_ResetPidBuffer(UINT8 u1Pidx, PID_STRUCT_T* prPidStruct)
{
    UINT32 u4StartAddr;

    ASSERT(prPidStruct != NULL);

    _DMX_Lock();

    // Check if PID is inactive.
    // The Valid bit in the PID Index Table is always 0 for ES.
    //if (prPidStruct->fgEnable || ((PID_INDEX_TABLE(u1Pidx) & 0x02) != 0))
    if (prPidStruct->fgEnable)
    {
        _DMX_Unlock();
        LOG(3, "Reset buffer of an active pid\n");
        return FALSE;
    }

    PID_S_W(u1Pidx, 1) = 0;                 // Encryption Length
    PID_S_W(u1Pidx, 2) = 0;                 // Offset Length (not used for now)
    PID_S_W(u1Pidx, 3) = 0;                 // Skip-byte , Pre-byte, # of start codes, Trigger
    PID_S_W(u1Pidx, 4) = 0xFFFFFFFF;        // Previous Payload Bytes

    // Reset payload FIFO
    u4StartAddr = prPidStruct->u4BufStart;
    prPidStruct->u4Rp = u4StartAddr;
    prPidStruct->u4Wp = u4StartAddr;
    prPidStruct->u4SectionRp = u4StartAddr; // of no use
    prPidStruct->u4PesRp = u4StartAddr;     // of no use
    u4StartAddr = PHYSICAL(u4StartAddr);
    PID_S_W(u1Pidx, 7) = u4StartAddr;       // PES_start
    PID_S_W(u1Pidx, 8) = u4StartAddr;       // Write pointer
    PID_S_W(u1Pidx, 9) = u4StartAddr;       // Read pointer

    // Reset header FIFO
    u4StartAddr = PHYSICAL(prPidStruct->u4HeaderBufAddr);
    PID_S_W(u1Pidx, 12) = u4StartAddr;      // Start
    PID_S_W(u1Pidx, 13) = u4StartAddr;      // Write pointer
    PID_S_W(u1Pidx, 14) = u4StartAddr;      // Read pointer
    prPidStruct->u4NextPic = 0;

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _Dmx_ES_EnablePid
 *
 *  @param  u1Pidx          PID index
 *  @parm   prES            ES data structure
 *
 *  @retval TRUE            Sucessful
 *  @retval FALSE           Failed
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_ES_EnablePid(UINT8 u1Pidx, BOOL fgEnable, UINT32 *pu4W0, PID_STRUCT_T *prPidStruct)
{
//    UINT32 u4ActiveFlag;

    ASSERT(pu4W0 != NULL);
    ASSERT(prPidStruct != NULL);

    if (fgEnable)
    {
        if (!_Dmx_ES_ResetPidBuffer(u1Pidx, prPidStruct))
        {
            return FALSE;
        }
        *pu4W0 |=  0x02;        // set Valid bit of "Section Filter Flags"

        // Always set bit4 to 0, no use toggle mechanism
        *pu4W0 &= (~0x10);
        *pu4W0 |= 0x4;
    }
    else
    {
        *pu4W0 &= ~0x02;        // clear Valid bit of "Section Filter Flags"
        prPidStruct->u1SerialNumber++;
        if (prPidStruct->u1SerialNumber > DMX_MAX_SERIAL_NUM)
        {
            prPidStruct->u1SerialNumber = 0;
        }
    }

    prPidStruct->fgEnable = fgEnable;

    // Notify handler
    _DMX_NotifyEnablePid(u1Pidx, fgEnable);

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _DMX_ES_SetPid
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ES_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_ES_T *prES)
{
    PID_STRUCT_T rPidStruct;
    UINT32 u4W0;

    ASSERT(prES != NULL);
    ASSERT(u1Pidx < DMX_MM_STRUCT_NUM);

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();
    u4W0 = PID_S_W(u1Pidx, 0);
    PID_S_W(u1Pidx, 0) = u4W0 & ~0x02;          // Disable PID first
    rPidStruct = *(_DMX_GetPidStruct(u1Pidx));
    _DMX_Unlock();

    // TS index
    rPidStruct.u1TsIndex = (UINT8)DDI_TS_INDEX_FOR_NO_FRAMER;

    // InputType
    rPidStruct.eInputType = DMX_IN_PLAYBACK_ES;
    rPidStruct.eSource = DMX_SOURCE_DRAM0;

    if ((u4Flags & DMX_PID_FLAG_CALLBACK) != 0)
    {
        rPidStruct.pfnNotify = prES->pfnNotify;
        rPidStruct.pvNotifyTag = prES->pvNotifyTag;
    }

    if ((u4Flags & DMX_PID_FLAG_INSTANCE_TAG) != 0)
    {
        rPidStruct.pvInstanceTag = prES->pvInstanceTag;
    }

    if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
    {
        rPidStruct.u1DeviceId = prES->u1DeviceId;
    }

    if ((u4Flags & DMX_PID_FLAG_BUFFER) != 0)
    {
        if (!_Dmx_ES_SetBuffer(u1Pidx, prES, &rPidStruct))
        {
            return FALSE;
        }
    }

    if ((u4Flags & DMX_PID_FLAG_COMPLETE_MSG) != 0)
    {
        rPidStruct.fgSendCompleteMsg = prES->fgSendCompleteMsg;
    }

    // Get PID Word 0 after buffer allocation.
    _DMX_Lock();
    u4W0 = PID_S_W(u1Pidx, 0);
    _DMX_Unlock();

    // Note: The Valid bit here means the one in the PID data memory, but not
    // the one in the PID index table!  During elementary stream playback, all
    // valid bits in PID index table must be cleared, DBM must be set to bypass
    // mode, and packets are steered toward FTuP.
    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        if (!_Dmx_ES_EnablePid(u1Pidx, prES->fgEnable, &u4W0, &rPidStruct))
        {
            return FALSE;
        }
    }

    _DMX_Lock();
    *(_DMX_GetPidStruct(u1Pidx)) = rPidStruct;
    PID_S_W(u1Pidx, 0) = u4W0;
    _DMX_Unlock();

//    LOG(7, "PidTable[%u]\n", u1Pidx);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ES_GetPid
 *  Note: the returned buffer address (e.g. start, write pointer, and read pointer)
 *  are kernel logical address.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ES_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_ES_T *prES)
{
    UINT32 u4W0;
    PID_STRUCT_T *prPidStruct;

    ASSERT(prES != NULL);
    ASSERT(u1Pidx < DMX_MM_STRUCT_NUM);

    if (u4Flags == DMX_PID_FLAG_NONE)
    {
        return TRUE;
    }

    _DMX_Lock();
    u4W0 = PID_S_W(u1Pidx, 0);
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    _DMX_Unlock();

    if ((u4Flags & DMX_PID_FLAG_CALLBACK) != 0)
    {
        prES->pfnNotify   = prPidStruct->pfnNotify;
        prES->pvNotifyTag = prPidStruct->pvNotifyTag;
    }

    if ((u4Flags & DMX_PID_FLAG_INSTANCE_TAG) != 0)
    {
        prES->pvInstanceTag = prPidStruct->pvInstanceTag;
    }

    if ((u4Flags & DMX_PID_FLAG_DEVICE_ID) != 0)
    {
        prES->u1DeviceId = prPidStruct->u1DeviceId;
    }

    if ((u4Flags & DMX_PID_FLAG_BUFFER) != 0)
    {
        prES->fgAllocateBuffer = prPidStruct->fgAllocateBuffer;
        prES->u4BufAddr = prPidStruct->u4BufStart;
        prES->u4BufSize = prPidStruct->u4BufLen;
        prES->u4Wp = prPidStruct->u4Wp;
        prES->u4Rp = prPidStruct->u4Rp;
        prES->ePidType = prPidStruct->ePidType;
        prES->u1ChannelId = prPidStruct->u1ChannelId;
    }

    if ((u4Flags & DMX_PID_FLAG_COMPLETE_MSG) != 0)
    {
        prES->fgSendCompleteMsg = prPidStruct->fgSendCompleteMsg;
    }

    if ((u4Flags & DMX_PID_FLAG_VALID) != 0)
    {
        BOOL fgEnable;

        fgEnable = (u4W0 & 0x2) ? TRUE : FALSE;
        if (prPidStruct->fgEnable != fgEnable)
        {
            LOG(3, "Warning: inconsistent states.\n");
            return FALSE;
        }
        prES->fgEnable = fgEnable;
    }

    if ((u4Flags & DMX_PID_FLAG_DATA_POINTERS) != 0)
    {
        prES->u4Rp = prPidStruct->u4Rp;
        prES->u4Wp = prPidStruct->u4Wp;
        prES->u4PeakBufFull = prPidStruct->u4PeakBufFull;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ES_FreePid
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ES_FreePid(UINT8 u1Pidx)
{
    PID_STRUCT_T *prPidStruct, rPidStruct;
    BOOL fgEnable;
    UINT8 u1SerialNumber;

    ASSERT(u1Pidx < DMX_MM_STRUCT_NUM);

    _DMX_Lock();
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    fgEnable = (PID_S_W(u1Pidx, 0) & 0x2) ? TRUE : FALSE;
    _DMX_Unlock();

    if (prPidStruct->fgEnable != fgEnable)
    {
        LOG(1, "Inconsistent state for the Enable flag!\n");
        return FALSE;
    }

    if (fgEnable)
    {
        UINT32 u4W0;

        LOG(3, "Try to free an active PID! (pidx: %u)\n", u1Pidx);

        // Stop PID
        _DMX_Lock();
        u4W0 = PID_S_W(u1Pidx, 0);
        rPidStruct = *prPidStruct;
        _DMX_Unlock();

        if (!_Dmx_ES_EnablePid(u1Pidx, FALSE, &u4W0, &rPidStruct))
        {
            LOG(5, "Failed to disable the enabled PID!\n");
            return FALSE;
        }

        _DMX_Lock();
        *prPidStruct = rPidStruct;
        PID_S_W(u1Pidx, 0) = u4W0;
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
        UINT8 u1Channel;

        // Sync with MPV
        VERIFY(_DMX_GetVideoChannel(u1Pidx, &u1Channel));
        MPV_FlushEsmQ(u1Channel, TRUE);

        // No need to send purge message if serail number is applied
        // Discard intermediate pictures, clean FIFO
        // VERIFY(_DMX_SendPurgeMessage(u1Pidx, FALSE));
        
        // free pidx video type
        if(!_DMX_FreeVideoTypeByPidx(u1Pidx))
        {
            return FALSE;
        }

        if(!_DMX_ClearPicFlag(u1Pidx))
        {
            return FALSE;
        }
    }

    // Free buffer
    if (!_Dmx_ES_FreeBuffer(u1Pidx))
    {
        return FALSE;
    }

    // Clear PID structures and PID memory
    // Note that serial number must be preserved across freeing PID
    // Also, peak buffer fullness is stored while channel changed
    _DMX_Lock();
    u1SerialNumber = prPidStruct->u1SerialNumber;
    x_memset((void*)prPidStruct, 0, sizeof (PID_STRUCT_T));
    prPidStruct->u1SerialNumber = u1SerialNumber;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ES_SetFileLength
 *
 *  This function set the file length to be decoded.  Users need to pad
 *  the end of the file into the alignment specified by the "DDI QUAD_LIMIT"
 *  or TS_PSIZE.  The de-multiplexing process stops when the specified length
 *  is sent to the Demux.  The statements above apply to DRM micro-code 3.04.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ES_SetFileLength(UINT8 u1Pidx, UINT32 u4Len)
{
    ASSERT(u1Pidx < DMX_MM_STRUCT_NUM);

    _DMX_Lock();
    PID_S_W(u1Pidx, 1) = u4Len;
    PID_S_W(u1Pidx, 3) = 0x1;  // Set the Trigger bit and clear all others.
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ES_SetMoveIndex
 */
//-----------------------------------------------------------------------------
void _DMX_ES_SetMoveIndex(UINT8 u1Idx)
{
    UINT32 u4Ctrl;

    _u1DmxESIdx = u1Idx;

    u4Ctrl = ((UINT32)u1Idx << 16) | 0x20;              // Steer to FTuP
    DMXCMD_WRITE32(DMX_REG_DBM_BYPASS_PID_4, u4Ctrl);
}


//-----------------------------------------------------------------------------
/** _DMX_ES_GetMoveIndex
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_ES_GetMoveIndex(void)
{
    return _u1DmxESIdx;
}


//-----------------------------------------------------------------------------
/** _DMX_ES_FlushBuffer
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ES_FlushBuffer(UINT8 u1Idx)
{
    PID_STRUCT_T* prPidStruct;
    UINT32 u4BufStart, u4PhyBufStart;

    if (u1Idx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid index (%d)!\n", u1Idx);
        return FALSE;
    }

    prPidStruct = _DMX_GetPidStruct(u1Idx);

    _DMX_Lock();

    // For DRM uCode, do not set byte 0 of word 3 to non-zero value.
    PID_S_W(u1Idx, 4) = 0xFFFFFFFF;     // Reset previous payload counter to FFFFFFFF
    PID_S_W(u1Idx, 18) = 0xFFFFFFFF;    // Reset previous payload counter to FFFFFFFF

    u4BufStart = prPidStruct->u4BufStart;

    prPidStruct->u4Wp = u4BufStart;
    prPidStruct->u4Rp = u4BufStart;
    prPidStruct->u4PesRp = u4BufStart;
    u4PhyBufStart = PHYSICAL(u4BufStart);
    PID_S_W(u1Idx, 7) = u4PhyBufStart;     // PES start
    PID_S_W(u1Idx, 8) = u4PhyBufStart;     // Write pointer
    PID_S_W(u1Idx, 9) = u4PhyBufStart;     // Read pointer

    switch (prPidStruct->ePidType)
    {
    case DMX_PID_TYPE_PSI:
        PID_S_W(u1Idx, 3) = 0xffff0002;
        prPidStruct->u4SectionRp = prPidStruct->u4BufStart;
        break;

    case DMX_PID_TYPE_ES_VIDEO:
    case DMX_PID_TYPE_ES_AUDIO:
    case DMX_PID_TYPE_ES_VIDEOCLIP:
    case DMX_PID_TYPE_ES_OTHER:
        u4BufStart = PHYSICAL(prPidStruct->u4HeaderBufAddr);
        PID_S_W(u1Idx, 12) = u4BufStart;    // Header Start
        PID_S_W(u1Idx, 13) = u4BufStart;    // Header Write pointer
        PID_S_W(u1Idx, 14) = u4BufStart;    // Header Read pointer
        prPidStruct->u4NextPic = 0;
        break;
    case DMX_PID_TYPE_PES:
        prPidStruct->u4PesRp = prPidStruct->u4BufStart;
        break;
    default:
        break;
    }

    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_ES_Reset
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ES_Reset(UINT8 u1Idx)
{
    PID_STRUCT_T *prPidStruct;

    _DMX_Lock();
    prPidStruct = _DMX_GetPidStruct(u1Idx);
    if(!prPidStruct)
    {
        _DMX_Unlock();
        return FALSE;
    }

    if(!prPidStruct->fgEnable)
    {
        _DMX_Unlock();
        return TRUE;
    }

    // Disable PID index table again.
    prPidStruct->fgEnable = FALSE;
    prPidStruct->u1SerialNumber++;
    if (prPidStruct->u1SerialNumber > DMX_MAX_SERIAL_NUM)
    {
        prPidStruct->u1SerialNumber = 0;
    }
    _DMX_Unlock();

    if(!_Dmx_ES_ResetPidBuffer(u1Idx, prPidStruct))
    {
        return FALSE;
    }

    _DMX_Lock();
    prPidStruct->fgEnable = TRUE;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ES_MoveData
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ES_MoveData(UINT8 u1Idx, DMX_MM_DATA_T *prData)
{
    PID_STRUCT_T *prPidStruct;

    if(!_DMX_MM_MoveData(u1Idx, prData, 5000))
    {
        return FALSE;
    }

    if(prData->fgEOS)
    {
        prPidStruct = _DMX_GetPidStruct(u1Idx);
        if(prPidStruct == NULL)
        {
            return FALSE;
        }

        if (prPidStruct->ePidType == DMX_PID_TYPE_ES_AUDIO)
        {
            DMX_AUDIO_PES_T rPes;
            x_memset((void*)&rPes, 0, sizeof(rPes));
            rPes.fgEOS = TRUE;
            rPes.u1PidIndex = u1Idx;
            rPes.u1DeviceId = _DMX_GetPidStruct(u1Idx)->u1DeviceId;
            if (!_DMX_SendAudioPes(&rPes))
            {
                return FALSE;
            }
        }
        if (prPidStruct->ePidType == DMX_PID_TYPE_ES_VIDEO)
        {
            if (!_DMX_SendEOSMessage(u1Idx))
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

