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
 * $Date: 2015/04/20 $
 * $RCSfile: adsp_task.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file adsp_task.c
 *  Brief of file adsp_task.c.
 *  Source file for ADSP related control routine.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "x_ckgen.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "dsp_common.h"
#include "ctrl_para.h"
#include "adsp_task.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_intf.h"
#include "dsp_func.h"
#include "dsp_rg_ctl.h"
#include "aud_debug.h"
#include "aud_hw.h"
#include "fbm_drvif.h"  // for DSP memory allocation
#include "drv_common.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#include "psr_drvif.h"
#include "aud_if.h"

#include "x_os.h"
#include "x_assert.h"

#include "hw_ckgen.h"

#define AUD_STATISTICS

#ifdef AUD_STATISTICS
#include "drv_dbase.h"
#include "x_timer.h"
#endif

#ifdef LINUX_TURNKEY_SOLUTION
#include <linux/module.h>
#endif

LINT_EXT_HEADER_END

//lint -e950 use __align()

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define DSP_DRAM_BLOCK_TEST
#define AUD_STATISTICS

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define _ADSP_ENV_BLOCK       1
#define DSP_WORKING_BUF_ALIGN  0x100
#define MSG_QUEUE_TIME_OUT  1000   // In times of 10 ms => total 10 sec

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

extern BOOL fgTriggerDspErrorLogs(void);
extern BOOL fgGetDspErrorLogs(ADSP_ERROR_LOG_T* ptADSPErrLog);
extern void vDspSetSamplingRate(UINT8 u1DspId, UINT8 u1DecId);
extern void AUD_DspClkSel(UINT8 u1ADSPclkSel);
extern UINT32 BSP_GetMemSize(void);

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
extern UINT32 u4Aproc_IsEnable(void);
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

HANDLE_T hDspCmdQueue;
HANDLE_T hDspCmdDataQueue;  //ADSP Data Queue.
HANDLE_T _hAdspThread;
static HANDLE_T _hRdDspSramSema[AUD_DSP_NUM];
HANDLE_T _hSendDspIntSema[AUD_DSP_NUM];

#ifdef AUD_STATISTICS

static UINT32 _u4TriggerPeriod = 2;    // Seconds for Trigger Log
static UINT32 _u4LogPeriod = 3;        // Seconds per message
static PARAM_AUD_T* _prAud = NULL;

#endif  // PSR_STATISTICS

INT8 _i1DspMemBlock = -1;
UINT32 _u4DspMemBufAddr = (UINT32)NULL;
static UINT32 _u4DspMemBufSize = 0;

#if   defined(DSP_NEW_SCRAMBLE) 
static BOOL fgCheckSumAll = FALSE;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define DSP_MEM_BLOCK(u4Addr)   (PHYSICAL(u4Addr)/DSP_BUF_BLOCK)

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
BOOL _fgTaskInit = FALSE;

void vSendADSPTaskMessage(UINT8 u1DspId, UINT32 u4Msg);
void vSendADSPTaskMessageNoWait(UINT8 u1DspId, UINT32 u4Msg);
void vADSPTaskMain_Init(void);

UINT32 _AudAlign(UINT32 u4Addr, UINT32 u4Alignment)
{
    UINT32 u4Unaligned;

    if (u4Alignment <= 1)
    {
        return u4Addr;
    }

    u4Unaligned = u4Addr % u4Alignment;
    if (u4Unaligned != 0)
    {
        u4Addr += u4Alignment - u4Unaligned;
    }

    return u4Addr;
}

//-----------------------------------------------------------------------------
//  fgDspGetDspMem
//
/** Brief: Get DSP memory address and size
 *  @param
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL fgDspGetDspMem(UINT32* pu4Addr, UINT32* pu4Size)
{
    if ((pu4Addr != NULL) && (pu4Size != NULL) && (_u4DspMemBufAddr != (UINT32)NULL))
    {
        *pu4Addr = _u4DspMemBufAddr;
        *pu4Size = _u4DspMemBufSize;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
//  u4DspInternalLogicalAddress
//
/** Brief: DSP inside logical address (0 ~ 16 MB)
 *  @param
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4DspInternalLogicalAddress(UINT32 u4Addr)
{
    // Check if memory is initilized
    VERIFY(_i1DspMemBlock >= 0);
    return (u4Addr & (DSP_BUF_BLOCK-1));
}


//-----------------------------------------------------------------------------
//  u4DspPhysicalAddress
//
/** Brief: Physical address (0 ~ 64 MB)
 *  @param
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4DspPhysicalAddress(UINT32 u4Addr)
{
    UINT32 u4MemSize;

    // Check if memory is initialized
    VERIFY(_i1DspMemBlock >= 0);

    // Check 256 MB boundary
    u4MemSize = BSP_GetMemSize();
    VERIFY(PHYSICAL(u4Addr / u4MemSize) == 0);

    // If within 16 MB
    if ((PHYSICAL(u4Addr) / DSP_BUF_BLOCK) == 0)
    {
        return (PHYSICAL(u4Addr) + (UINT32)((_i1DspMemBlock)*DSP_BUF_BLOCK));
    }
    else if (((PHYSICAL(u4Addr)/DSP_BUF_BLOCK) > 0) &&
            ((PHYSICAL(u4Addr)/DSP_BUF_BLOCK) < 128))    // If within 16 ~ 2048 MB
    {
        return PHYSICAL(u4Addr);
    }
    else
    {
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return PHYSICAL(u4Addr);
    }
}

//-----------------------------------------------------------------------------
//  u4DspVirtualAddress
//
/** Brief: Virtual address (0 ~ 64 MB | 0x30000000)
 *  @param
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4DspVirtualAddress(UINT32 u4Addr)
{
    return VIRTUAL(u4DspPhysicalAddress(u4Addr));
}

#ifdef DSP_TEST_DRAM_BLOCK
__align(0x100) static UINT8 _au1AdspWorkingBufferUnAligned[DSP_WORK_BUF_SIZE + DSP_WORKING_BUF_ALIGN] ;
#endif

#if defined(CC_AUD_ARM_SUPPORT)
static UINT32 u4AudDspWorkBufBase = 0;
UINT32 u4GetDspWorkBufNonInit(void)
{
    ASSERT (u4AudDspWorkBufBase != (UINT32)NULL); // ADSP working buffer has not been not initialized

    return u4AudDspWorkBufBase;
}

UINT32 u4GetAprocMemoryMap (UINT32 u4Type)
{
    UINT32 u4Addr;

    u4Addr = _gu4AprocMemMap[u4Type];

    ASSERT (u4Addr != (UINT32)NULL); // Audio processor memory has not been not initialized

    return u4Addr;
}
#endif

//-----------------------------------------------------------------------------
//  u4GetDspWorkBuf
//
/** Brief:  Allocate DSP working buffer from FB pool
 *  @param
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4GetDspWorkBuf(void)
{
#ifndef DSP_TEST_DRAM_BLOCK
    FBM_POOL_T* prFbmPool;
#endif
    UINT32  u4BufAddr;
    UINT32  u4BufSize;
    UINT32  u4MemSize;

#ifndef DSP_TEST_DRAM_BLOCK
    // Get buffer from FB pool
    prFbmPool = FBM_GetPoolInfo((UINT8) FBM_POOL_TYPE_DSP);

    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != (UINT32)NULL);
    u4BufAddr= prFbmPool->u4Addr;
    u4BufSize = prFbmPool->u4Size;
    _u4DspMemBufAddr = u4BufAddr;
    _u4DspMemBufSize = u4BufSize;
#else
    UINT32 u4TmpAddr;
#if defined(CC_AUD_ARM11_SUPPORT)
    u4TmpAddr = _AudAlign((UINT32)_au1AdspWorkingBufferUnAligned, 0x400); // ensure 1KB alignment
    u4BufAddr = u4TmpAddr + (56*1024*1024);
#else
    u4TmpAddr = _AudAlign((UINT32)_au1AdspWorkingBufferUnAligned,0x100);
    u4BufAddr = u4TmpAddr + (56*1024*1024);
#endif // CC_AUD_ARM11_SUPPORT
    u4BufSize = DSP_WORK_BUF_SIZE;
    _u4DspMemBufAddr = u4BufAddr;
    _u4DspMemBufSize = u4BufSize;
#endif

    // Check 256 Bytes alignment
    VERIFY(_AudAlign(VIRTUAL(u4BufAddr), 0x100) == VIRTUAL(u4BufAddr));

#if defined(CC_AUD_ARM11_SUPPORT)
        // Check 1KB alignament
        VERIFY(_AudAlign(VIRTUAL(u4BufAddr), 0x400) == VIRTUAL(u4BufAddr));
#endif // CC_AUD_ARM11_SUPPORT

    // Check 64 MB boundary
    u4MemSize = BSP_GetMemSize();
    VERIFY(PHYSICAL((u4BufAddr + u4BufSize) / u4MemSize) == 0);

    // Check if start and end address are in the same 16 MB boundary
    VERIFY(DSP_MEM_BLOCK(u4BufAddr) == DSP_MEM_BLOCK(u4BufAddr + u4BufSize));

    // Setup memory block to register
    _i1DspMemBlock = (INT8)DSP_MEM_BLOCK(u4BufAddr);
    if (DSP_SetAudDramBlock(AUD_DSP0, (UINT8)_i1DspMemBlock)||DSP_SetAudDramBlock(AUD_DSP1, (UINT8)_i1DspMemBlock))
    {
        ASSERT(0);
    }

    // Clear memory

#ifndef DSP_TEST_DRAM_BLOCK
    x_memset((VOID *)VIRTUAL(u4BufAddr), 0, prFbmPool->u4Size);
    DSP_FlushInvalidateDCacheFree(u4BufAddr, prFbmPool->u4Size);
#else
    x_memset((VOID *)VIRTUAL(u4BufAddr), 0, u4BufSize);
#endif


#if defined(CC_AUD_ARM_SUPPORT)
    u4AudDspWorkBufBase = VIRTUAL(u4BufAddr); // init AUD DSP working buffer base
#endif

    return VIRTUAL(u4BufAddr);
}

/******************************************************************************
* Function      : u4GetAFIFOStart
* Description   : get audio FIFO start address in physical address
* Parameter     : uDecIndex: 0: first decoder,1: second decoder
* Return        : None
******************************************************************************/
UINT32 u4GetAFIFOStart(UINT8 u1DspId, UINT8 uDecIndex)
{
    return _u4AFIFO[u1DspId][uDecIndex][0];
}

/******************************************************************************
* Function      : u4GetAFIFOEnd
* Description   : get audio FIFO end address
* Parameter     : uDecIndex: 0: first decoder,1: second decoder
* Return        : None
******************************************************************************/
UINT32 u4GetAFIFOEnd(UINT8 u1DspId, UINT8 uDecIndex)
{
    return _u4AFIFO[u1DspId][uDecIndex][1];
}

#if defined(CC_ENABLE_AOMX) ||defined (ALSA_PCMDEC_PATH) || defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_ADECOMX)
UINT32 u4GetABufPnt(UINT8 u1DspId, UINT8 uDecIndex)
{
    UINT32 u4Reg;

    if (uDecIndex == AUD_DEC_MAIN)
    {
        u4Reg = REG_ABUF_PNT(u1DspId);
    }
    else if (uDecIndex == AUD_DEC_AUX)
    {
        u4Reg = REG_ABUF_PNT_DEC2(u1DspId);
    }
    else if (uDecIndex == AUD_DEC_THIRD)
    {
        u4Reg = REG_ABUF_PNT_DEC3(u1DspId);
    }
    else
    {
        u4Reg = REG_ABUF_PNT_DEC4(u1DspId);
    }

    AUD_WRITE32(u4Reg,0);
    return ((_u4AFIFO[u1DspId][uDecIndex][0]&0xff000000) | AUD_READ32(u4Reg));
}
#endif

UINT32 u4GetAWritePnt(UINT8 u1DspId, UINT8 uDecIndex)
{
    if (uDecIndex == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_ABUF_PNT(u1DspId),1);
        return ((_u4AFIFO[u1DspId][0][0]&0xff000000) | AUD_READ32(REG_ABUF_PNT(u1DspId)));
    }
    else
    {
        AUD_WRITE32(REG_ABUF_PNT_DEC2(u1DspId),1);
        return ((_u4AFIFO[u1DspId][1][0]&0xff000000) | AUD_READ32(REG_ABUF_PNT_DEC2(u1DspId)));
    }
}

extern BOOL PSR_RiscSetAudioWp(UINT8 u1PsrId, UINT8 u1DeviceId, UINT32 u4WritePointer);

#if defined(CC_ENABLE_AOMX) ||defined(ALSA_PCMDEC_PATH) ||defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_ADECOMX)
void vSetAWritePnt(UINT8 uDecIndex, UINT32 u4WritePointer)
{
    if (uDecIndex == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_USER1_WP, u4WritePointer);
    }
    else if (uDecIndex == AUD_DEC_AUX) //AOMX2_TEST_TODO
    {
        AUD_WRITE32(REG_DMX_WRITE_PNT, u4WritePointer);
    }
    else if (uDecIndex == AUD_DEC_THIRD)
    {
        AUD_WRITE32(REG_USER0_WP, u4WritePointer);
    }
    else
    {
        AUD_WRITE32(REG_USER1_WP, u4WritePointer);
    }
}
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
/******************************************************************************
* Function      : u4GetHDMIParserFIFOStart
* Description   : get HDMI parser FIFO start address in physical address
* Parameter     : void
* Return        : HDMI parser FIFO start address
******************************************************************************/
UINT32 u4GetHDMIParserFIFOStart(void)
{
    return _u4AFIFOHdmiParser[0];
}

/******************************************************************************
* Function      : u4GetHDMIParserFIFOEnd
* Description   : get HDMI parser FIFO end address in physical address
* Parameter     : void
* Return        : HDMI parser FIFO end address
******************************************************************************/
UINT32 u4GetHDMIParserFIFOEnd(void)
{
    return _u4AFIFOHdmiParser[1];
}
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
UINT32 u4GetSBCEncFIFOStart(void)
{
    return _u4SBCEncBuf[0];
}

UINT32 u4GetSBCEncFIFOEnd(void)
{
    return _u4SBCEncBuf[1];
}
UINT32 u4GetSBCEncWritePnt(void)
{
    DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    return ((_u4SBCEncBuf[0]&0xff000000) | dReadDspCommDram(AUD_DSP0, ADDR_RC2D_SBCE_WRITE_PTR_DEC4));
}
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
UINT32 u4GetUploadFIFOStart(void)
{
    return (_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE);
}

UINT32 u4AUD_UPLOAD_BUFFER_SIZE = AUD_UPLOAD_BUFFER_SIZE;
UINT32 u4GetUploadFIFOEnd(void)
{
    return (_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE + u4AUD_UPLOAD_BUFFER_SIZE);
}

UINT32 u4GetUploadWritePnt(void)
{
    DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    //DSP_FlushInvalidateDCacheSmall2(ADDR_RC2D_UPLOAD_WRITE_PTR);
    return ((_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX]&0xff000000) | dReadDspCommDram(AUD_DSP0, ADDR_RC2D_UPLOAD_WRITE_PTR));
}
//#endif
#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
UINT32 u4GetBluetoothFIFOStart(void)
{
    return (_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + AUD_BLUETOOTH_BUFFER_BASE);
}

UINT32 u4GetBluetoothFIFOEnd(void)
{
    return (_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + AUD_BLUETOOTH_BUFFER_BASE + AUD_BLUETOOTH_BUFFER_SIZE);
}
UINT32 u4GetBluetoothWritePnt(void)
{
    DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    //DSP_FlushInvalidateDCacheSmall2(ADDR_RC2D_UPLOAD_WRITE_PTR);
    return ((_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX]&0xff000000) | dReadDspCommDram(AUD_DSP0, ADDR_RC2D_BLUETOOTH_WRITE_PTR));
}
#endif

void vSetUploadReadPtr(UINT8 u1DspId, UINT32 u4ReadPtr)
{
    WriteDspCommDram(u1DspId, ADDR_RC2D_UPLOAD_READ_PTR,(u4ReadPtr&0xffffff));
}

/******************************************************************************
* Function      : vAdspTaskMain
* Description   : main routine for ADSP Task
* Parameter     : None
* Return        : None
******************************************************************************/
//lint -e{818}
static void vADSPTaskMain(VOID* pvArg)
{
    DSP_CMD_QUEUE_T rDspCmdMsg;
    UINT16 u2MsgIdx;
    BOOL fgGetRet;

    VERIFY(pvArg == NULL);

    // Move initialzation relatrd code to vADSPTaskMainInit. To make sure the AUD_Init flow correctness.

    while (1)
    {
        SIZE_T zMsgSize = sizeof(rDspCmdMsg);
        INT32  i4MsgRet;

        i4MsgRet = x_msg_q_receive(&u2MsgIdx,(VOID*)&rDspCmdMsg, &zMsgSize, &hDspCmdQueue, 1, X_MSGQ_OPTION_WAIT); // Light: Set queue as wait type

        VERIFY((i4MsgRet == OSR_OK) || (i4MsgRet == OSR_NO_MSG));
        VERIFY((rDspCmdMsg.bDspId== AUD_DSP0) || (rDspCmdMsg.bDspId== AUD_DSP1));

#ifdef ADSP_CHECK_RAM_CODE
        if (_rDspStatus[rDspCmdMsg.bDspId][AUD_DEC_MAIN].fgDspWakeUpOk)
        {
            LOG(7, "[DSP]========= Check RAM Start =========\n");

            if (fgCheckDspData())
            {
                LOG(7, "[DSP] RAM check pass\n");
            }
            else
            {
                LOG(7, "[DSP] Error: RAM check fail\n");
            }

            LOG(7, "[DSP]========== Check RAM End ==========\n");
        }
#endif

        if (i4MsgRet == OSR_OK)
        {
            switch(rDspCmdMsg.u4Msg & 0xFFFF)
            {
            case ADSPTASK_MSG_INTERRUPT_DATA:
                vDspIntSvc_Data(rDspCmdMsg.bDspId);  //ADSP Data Queue.
                break;
            case ADSPTASK_MSG_POWER_ON:
                // set to power initial state
                vDspStateInit(rDspCmdMsg.bDspId); // initialization for ADSP state machine
                fgGetRet = fgDspInitState (rDspCmdMsg.bDspId); // Light: wait for DSP wakeup in fgDspInitState
                if (fgGetRet)
                {
                    _uDspState[rDspCmdMsg.bDspId] = (UINT8)DSP_CK_INT;
                }
                else // wakeup isn't finished...
                {
                    LOG(1, "DSP(%d) wake up fail\n", rDspCmdMsg.bDspId);
                    ASSERT(0);
                }
                LOG(9 , "DspId %d, DSPTASK_MSG_POWER_ON\n", rDspCmdMsg.bDspId);

//modify by ling                
#ifdef CC_AUD_ARM_SUPPORT // fix me!!!!
    #ifdef CC_AUD_ARM_RENDER
                // open aproc output
                // aproc output registers are switched by adsp kernel code
                // aproc output must be opened after adsp kernel ready
                // Aproc read aout ctrl register(0xF0000A5C) will always return 0 when DSPA is not power on
                // So Aproc open must wait DSPA kernel init finished
                if (rDspCmdMsg.bDspId == AUD_DSP0)
                {
                    if (bIsSupportAproc () == TRUE)
                    {
                        while (1)
                        {
                            // wait aproc power-on
                            if (u4Aproc_IsEnable())
                            {
                                break;
                            }
                            x_thread_delay (1);
                        }
                    }
                    vAproc_Open ();
                }
    #endif                
#endif                
                
                break;
            case ADSPTASK_MSG_POWER_OFF:
                if (_rDspStatus[rDspCmdMsg.bDspId][AUD_DEC_MAIN].fgDspPlay)
                {
                    vDspCmd(rDspCmdMsg.bDspId, DSP_STOP);
                }
                if (_rDspStatus[rDspCmdMsg.bDspId][AUD_DEC_AUX].fgDspPlay)
                {
                    vDspCmd(rDspCmdMsg.bDspId, DSP_STOP_DEC2);
                }
#ifdef CC_MT5391_AUD_3_DECODER
                if (_rDspStatus[rDspCmdMsg.bDspId][AUD_DEC_THIRD].fgDspPlay)
                {
                    vDspCmd(rDspCmdMsg.bDspId, DSP_STOP_DEC3);
                }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
                if (_rDspStatus[rDspCmdMsg.bDspId][AUD_DEC_4TH].fgDspPlay)
                {
                    vDspCmd(rDspCmdMsg.bDspId, DSP_STOP_DEC4);
                }
#endif
                _uDspState[rDspCmdMsg.bDspId] = (UINT8)DSP_WAIT_POWER_OFF;
                LOG(9 , "DSPTASK_MSG_POWER_OFF, DspId = %d\n", rDspCmdMsg.bDspId);
                break;
            case ADSPTASK_MSG_INT_SAMPLE_RATE:
                vDspSetSamplingRate(rDspCmdMsg.bDspId, AUD_DEC_MAIN);
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_SAMPLING_RATE);
                break;
            case ADSPTASK_MSG_INT_SAMPLE_RATE_DEC2:
                vDspSetSamplingRate(rDspCmdMsg.bDspId, AUD_DEC_AUX);
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC2);
                break;
            case ADSPTASK_MSG_INT_AOUT_ENABLE:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_AOUT_ENABLE);
                break;
            case ADSPTASK_MSG_INT_AOUT_ENABLE_DEC2:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2);
                break;
            case ADSPTASK_MSG_INT_STOP:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_FLUSH_DONE);
        #ifdef CC_AUD_EFFECT_MUTE_PROTECT
                vDspEffectReInitUnMuteSurround();
                vDspEffectReInitUnMuteVBass();      
        #endif
                break;
            case ADSPTASK_MSG_INT_STOP_DEC2:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_FLUSH_DONE_DEC2);
                break;
            case ADSPTASK_MSG_INT_MODE_CHANGE:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_MODE_CHANGE);
                break;
            case ADSPTASK_MSG_INT_MODE_CHANGE_DEC2:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2);
                break;
            case ADSPTASK_MSG_INT_PAL_MODE:
                vDspDemodDetModeNotify(INT_DSP_PAL_DETECTED_MODE, rDspCmdMsg.u4Msg>>16);
                break;
            case ADSPTASK_MSG_INT_JPN_MODE:
                vDspDemodDetModeNotify(INT_DSP_JPN_DETECTED_MODE, rDspCmdMsg.u4Msg>>16);
                break;
            case ADSPTASK_MSG_INT_MTS_MODE:
                vDspDemodDetModeNotify(INT_DSP_MTS_DETECTED_MODE, rDspCmdMsg.u4Msg>>16);
                break;
            case ADSPTASK_MSG_INT_DETECTOR_NOTIFY:
                vDspTvSysDetectedNotify(AUD_DEC_MAIN, (TV_AUD_SYS_T)(rDspCmdMsg.u4Msg>>16));
                break;
            case ADSPTASK_MSG_INT_DETECTOR_DEC2_NOTIFY:
                vDspTvSysDetectedNotify(AUD_DEC_AUX, (TV_AUD_SYS_T)(rDspCmdMsg.u4Msg>>16));
                break;
            case ADSPTASK_MSG_INT_DETECTOR_DEC3_NOTIFY:
                vDspTvSysDetectedNotify(AUD_DEC_THIRD, (TV_AUD_SYS_T)(rDspCmdMsg.u4Msg>>16));
                break;                
            case ADSPTASK_MSG_INT_ATV_CHANGE:
                vDspTvSysChangeNotify((TV_AUD_SYS_T)(rDspCmdMsg.u4Msg>>16));
                break;
            case ADSPTASK_MSG_INT_ATV_HDEV_SWITCH:
                //vDspDemodDetModeNotify(INT_DSP_HDEV_AUTO_SWITCH, rDspCmdMsg.u4Msg>>16);
                vDspHdevModeChangeNotify(rDspCmdMsg.u4Msg>>16);
                break;
            case ADSPTASK_MSG_INT_FM_RADIO_DET:
                vDspFMRadioDetectionNotify((AUD_FM_RADIO_DET_T)(rDspCmdMsg.u4Msg>>16));
                break;
            case ADSPTASK_MSG_INT_MINER_NOTIFY:
                vDspDemodDetModeNotify(INT_DSP_MINER_NOTIFY, rDspCmdMsg.u4Msg>>16);
                break;
            case ADSPTASK_MSG_INT_UPDATE_EFFECT:
                vDspUpdatePostEffect();
                break;
        #ifdef CC_AUD_EFFECT_MUTE_PROTECT
            case ADSPTASK_MSG_INT_UPDATE_EFFECT_END:
                vDspEffectReInitUnMuteSurround();
                vDspEffectReInitUnMuteVBass();
                break;                   
        #endif
#ifdef CC_MT5391_AUD_3_DECODER
            case ADSPTASK_MSG_INT_SAMPLE_RATE_DEC3:
                vDspSetSamplingRate(rDspCmdMsg.bDspId, AUD_DEC_THIRD);
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC3);
                break;
            case ADSPTASK_MSG_INT_AOUT_ENABLE_DEC3:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC3);
                break;
            case ADSPTASK_MSG_INT_STOP_DEC3:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_FLUSH_DONE_DEC3);
                break;
            case ADSPTASK_MSG_INT_MODE_CHANGE_DEC3:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_MODE_CHANGE_DEC3);
                break;
#endif
            case ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE);
                break;
            case ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE_DEC2:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE_DEC2);
                break;
#ifdef CC_AUD_4_DECODER_SUPPORT
            case ADSPTASK_MSG_INT_SAMPLE_RATE_DEC4:
                vDspSetSamplingRate(rDspCmdMsg.bDspId, AUD_DEC_4TH);
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC4);
                break;
            case ADSPTASK_MSG_INT_AOUT_ENABLE_DEC4:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC4);
                break;
            case ADSPTASK_MSG_INT_STOP_DEC4:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_FLUSH_DONE_DEC4);
                break;
            case ADSPTASK_MSG_INT_MODE_CHANGE_DEC4:
                vDspFlowControlNotify(rDspCmdMsg.bDspId, D2RC_FLOW_CONTROL_MODE_CHANGE_DEC4);
                break;
#endif
            default:
                break;
            }

            vDspState(rDspCmdMsg.bDspId, rDspCmdMsg.u4Msg);

        }
    }
}


#ifdef DSP_FORCE_RESET_ENABLE
/******************************************************************************
* Function      : u4ADSP_DspReset
* Description   : Reset DSP state and registers, called by DSP monitor thread.
* Parameter     : None
* Return        : None
******************************************************************************/
INT32 u4ADSP_DspReset(void) //  -- DSP Force --
{
    BOOL    fgGetRet;
    INT32  i4MsgRet = 0;

    vDspStateInit(AUD_DSP0); // initialization for ADSP state machine
#ifndef CC_AUD_DISABLE_2ND_DSP       
    vDspStateInit(AUD_DSP1);
#endif
    fgGetRet = fgDspInitState (AUD_DSP0);
    if (fgGetRet)
    {
         _uDspState[AUD_DSP0] = (UINT8)DSP_CK_INT;
    }
    else // wakeup isn't finished...
    {
        LOG(0, "DSP0 wake up fail\n");
        ASSERT(0);
    }

    LOG(1, "DSP0 ack ......\n");

#ifndef CC_AUD_DISABLE_2ND_DSP   
    fgGetRet = fgDspInitState (AUD_DSP1);
    if (fgGetRet)
    {
         _uDspState[AUD_DSP1] = (UINT8)DSP_CK_INT;
    }
    else // wakeup isn't finished...
    {
        LOG(0, "DSP1 wake up fail\n");
        ASSERT(0);
    }

    LOG(1, "DSP1 ack ......\n");    
#endif
    return i4MsgRet;
}
#endif

void vSendADSPTaskMessage(UINT8 u1DspId, UINT32 u4Msg)
{
    DSP_CMD_QUEUE_T rDspCmdMsg;
    INT32 i4Ret;
    INT32 i4MsgOverFlowCnt = 0;

    rDspCmdMsg.u4Msg = u4Msg;
    rDspCmdMsg.bDspId = u1DspId;

    do
    {
        i4Ret = x_msg_q_send(hDspCmdQueue,(VOID *)&rDspCmdMsg,sizeof(rDspCmdMsg),1);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
        if (i4Ret == OSR_TOO_MANY)
        {
            x_thread_delay(10);
            i4MsgOverFlowCnt ++;
            if (i4MsgOverFlowCnt >= MSG_QUEUE_TIME_OUT)
            {
                LOG(1, "Audio message queue over flow\n");
        #ifndef DEMO_BOARD                
                ASSERT(0);
        #endif
            }
        }
    } while(i4Ret != OSR_OK);
}

void vSendADSPTaskMessageNoWait(UINT8 u1DspId, UINT32 u4Msg)
{
    DSP_CMD_QUEUE_T rDspCmdMsg;
    INT32 i4Ret;

    rDspCmdMsg.u4Msg = u4Msg;
    rDspCmdMsg.bDspId = u1DspId;

    i4Ret = x_msg_q_send(hDspCmdQueue,(VOID *)&rDspCmdMsg,sizeof(rDspCmdMsg),1);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "Audio message queue over flow\n");
    #ifndef DEMO_BOARD        
        ASSERT(0);
    #endif
    }
}

void vSendADSPTaskData(UINT8 u1DspId, UINT32 u4Msg) //ADSP Data Queue.
{
    DSP_CMD_QUEUE_T rDspCmdMsg;
    INT32 i4Ret;

    rDspCmdMsg.u4Msg = u4Msg;
    rDspCmdMsg.bDspId = u1DspId;

    i4Ret = x_msg_q_send(hDspCmdDataQueue,(VOID *)&rDspCmdMsg,sizeof(rDspCmdMsg),1);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "DSP Data queue over flow\n");
    #ifndef DEMO_BOARD        
        ASSERT(0);
    #endif
    }
}

void vGetADSPTaskData(UINT8 *u1DspId, UINT32 *u4Msg) //ADSP Data Queue.
{
    DSP_CMD_QUEUE_T rDspCmdMsg;
    INT32 i4Ret;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(rDspCmdMsg);

    i4Ret = x_msg_q_receive(&u2MsgIdx, (VOID*)&rDspCmdMsg, &zMsgSize, &hDspCmdDataQueue, 1, X_MSGQ_OPTION_WAIT);
    if (i4Ret != OSR_OK)  //Should follow "Put 1 cmd, put 1 data", so there is no case the that "No Data"
    {
        LOG(1, "DSP Data queue no data\n");
    #ifndef DEMO_BOARD        
        ASSERT(0);
    #endif
    }
    
    *u4Msg = rDspCmdMsg.u4Msg;
    *u1DspId = rDspCmdMsg.bDspId;
}

#ifdef AUD_STATISTICS

/* Set error log period in Second */
void SetAudLogPeriod(UINT32 u4Period)
{
    if (u4Period == 0) // no log
    {
        _u4TriggerPeriod = 0;
        _u4LogPeriod = 0;
    }
    else if (u4Period == 1) // _u4TriggerPeriod at lease 1 sec
    {
        _u4TriggerPeriod = 1;
        _u4LogPeriod = 2;
    }
    else // u4Period >= 2
    {
        _u4TriggerPeriod = u4Period - 1;
        _u4LogPeriod = u4Period;
    }
}

//lint -e{727}
//lint -e{830}
//lint -e{550}
BOOL fgAudStatistics(void)
{
    static HAL_TIME_T _rStartTime;
    static HAL_TIME_T _rLastTime;
    HAL_TIME_T rCurTime = {0};
    BOOL fgRet = TRUE;
    static UINT8 uLogState = 0;
    static UINT16 u2TriggerCnt = 0;
    static UINT16 u2LogCnt = 0;
    static UINT16 u2ErrorCnt = 0;

#ifdef CC_AUD_4_DECODER_SUPPORT
    if (fgDecoderRealPlay(AUD_DSP0, AUD_DEC_MAIN) || fgDecoderRealPlay(AUD_DSP0, AUD_DEC_AUX) ||
        fgDecoderRealPlay(AUD_DSP0, AUD_DEC_THIRD) || fgDecoderRealPlay(AUD_DSP0, AUD_DEC_4TH))
#elif defined(CC_MT5391_AUD_3_DECODER)
    if (fgDecoderRealPlay(AUD_DSP0, AUD_DEC_MAIN) || fgDecoderRealPlay(AUD_DSP0, AUD_DEC_AUX) || fgDecoderRealPlay(AUD_DSP0, AUD_DEC_THIRD))
#else
    if (fgDecoderRealPlay(AUD_DSP0, AUD_DEC_MAIN) || fgDecoderRealPlay(AUD_DSP0, AUD_DEC_AUX))
#endif
    {
        if ((_rStartTime.u4Seconds == 0) && (_rStartTime.u4Micros == 0))
        {
            HAL_GetTime(&_rStartTime);
            _rLastTime = _rStartTime;
        }

        HAL_GetTime(&rCurTime);
        // trigger log at 2nd second
        if ((uLogState == 0) && ( (rCurTime.u4Seconds - _rLastTime.u4Seconds) >= _u4TriggerPeriod))
        {
            fgRet = fgTriggerDspErrorLogs();
            u2TriggerCnt ++;
            if (!fgRet)
            {
                LOG(7, "fgTriggerDspErrorLogs Time out (Dsp Busy)\n");
            }
            else
            {
                uLogState = 1;         // switch to get state
            }
        }
        // get log at 3rd second
        if ((uLogState == 1) && ( (rCurTime.u4Seconds - _rLastTime.u4Seconds) >= _u4LogPeriod))
        {
            // Get DSP Previous Error Logs
            fgRet = fgGetDspErrorLogs((ADSP_ERROR_LOG_T *)_prAud);
            if (fgRet)
            {
                u2LogCnt++;            // DSP data ready
                uLogState = 0;         // switch to trigger state
                _rLastTime = rCurTime;
            }
            else
            {
                u2ErrorCnt ++;                      // DSP data not ready
                LOG(7, "fgGetDspErrorLogs Time out %d (Command Loss)\n", u2ErrorCnt);

                // If Command loss over 3 times, switch to trigger state
                if ((u2ErrorCnt % 3) == 0)
                {
                     uLogState = 0;  // switch to trigger state
                }
            }
        }
    }
    return fgRet;
}
#endif

static void vADSPIRQHandler(UINT16 u2Vector)
{
    UINT32 u4Msg;
    UINT32 u4Dsp2RiscInt;

    u4Dsp2RiscInt = AUD_READ32(REG_RISC_INT);
    if ((u4Dsp2RiscInt & DSPA2RC_INTR) != 0)
    {
        AUD_SET_BIT(REG_RISC_INT, DSPA2RC_INTR);
    }
#ifndef CC_AUD_DISABLE_2ND_DSP      
    if ((u4Dsp2RiscInt & DSPB2RC_INTR) != 0)
    {
        AUD_SET_BIT(REG_RISC_INT, DSPB2RC_INTR);
    }
#endif    

    //vADSPIRQEnable(FALSE);

    if (u4Dsp2RiscInt&DSPA2RC_INTR)  //DSP0 INT
    { 
        u4Msg = u4DspIRQSvc(AUD_DSP0);
    //vSendADSPTaskMessage(u4Msg);
        vSendADSPTaskMessageNoWait(AUD_DSP0, u4Msg);  //fix me
    }  
#ifndef CC_AUD_DISABLE_2ND_DSP    
    if (u4Dsp2RiscInt&DSPB2RC_INTR)  //DSP1 INT
    { 
        u4Msg = u4DspIRQSvc(AUD_DSP1);
    //vSendADSPTaskMessage(u4Msg);
        vSendADSPTaskMessageNoWait(AUD_DSP1, u4Msg);  //fix me
    }
#endif    
    UNUSED(u2Vector);
}

#if 0   // Unused
void vADSPIRQEnable(BOOL fgEnable)
{
    BOOL u1Ret;
    // Enable ADSP interrupt
    if(fgEnable)
    {
        u1Ret = BIM_EnableIrq(VECTOR_AUDIO);
    }
    else
    {
        u1Ret = BIM_DisableIrq(VECTOR_AUDIO);
    }
    UNUSED(u1Ret);
}
#endif
void CheckSumAll(void);   
   
void vADSPTaskMain_Init(void)
{
    UINT32* pu4DspWorkBuf;

    vADCInit();
#if defined(CC_MT5880)
    AUD_DspClkSel(4); // viper adsp runs at 450MHz
#else
    AUD_DspClkSel(1); //1: sawlesspll_d1_ck (432MHz) or 4: enetpll_d1p5_ck (450MHz)
#endif

    vIO32WriteFldAlign(CKGEN_REG_AIN_CKCFG, 0, FLD_AIN_PD);
    vIO32WriteFldAlign(CKGEN_REG_AIN_CKCFG, 0, FLD_AIN_TST);
    vIO32WriteFldAlign(CKGEN_REG_AIN_CKCFG, 1, FLD_AIN_SEL); // 1:syspll_d4_ck (432/4 = 108), 2: syspll_d6_ck (648/6=108 for 5368/96)
    
    pu4DspWorkBuf = (UINT32 *)u4GetDspWorkBuf();
    vDspMemInit(AUD_DSP0, pu4DspWorkBuf);
#ifndef CC_AUD_DISABLE_2ND_DSP    
    vDspMemInit(AUD_DSP1, pu4DspWorkBuf);
#endif
    vDspShareInfoInit(AUD_DSP0); // initialization for shared information
#ifndef CC_AUD_DISABLE_2ND_DSP       
    vDspShareInfoInit(AUD_DSP1); 
#endif
    //initial AFIFO hardware register
    vSetSPDIFInFIFO(AUD_LINEIN_MAIN, u4GetAFIFOStart(AUD_DSP0, AUD_DEC_MAIN), u4GetAFIFOEnd(AUD_DSP0, AUD_DEC_MAIN));
    #ifdef DEC3_USE_LINEIN2
    vSetSPDIFInFIFO(AUD_LINEIN_AUX, u4GetAFIFOStart(AUD_DSP0, AUD_DEC_THIRD), u4GetAFIFOEnd(AUD_DSP0, AUD_DEC_THIRD));
    #endif
    //initial Paser register
    VERIFY(PSR_SoftSetAudioFIFO(AUD_DEC_MAIN, AUD_DSP0, u4GetAFIFOStart(AUD_DSP0, AUD_DEC_MAIN), u4GetAFIFOEnd(AUD_DSP0, AUD_DEC_MAIN)));
    PSR_RiscSetAudioWp(AUD_DSP0, AUD_DEC_MAIN, u4GetAFIFOStart(AUD_DSP0, AUD_DEC_MAIN));
#ifndef CC_AUD_DISABLE_2ND_DSP     
    VERIFY(PSR_SoftSetAudioFIFO(AUD_DEC_MAIN, AUD_DSP1, u4GetAFIFOStart(AUD_DSP1, AUD_DEC_MAIN), u4GetAFIFOEnd(AUD_DSP1, AUD_DEC_MAIN)));
    PSR_RiscSetAudioWp(AUD_DSP1, AUD_DEC_MAIN, u4GetAFIFOStart(AUD_DSP1, AUD_DEC_MAIN));
#endif    
    vOplFuncInit();

    vWriteDspWORD (AUD_DSP0, ADDR_RC2D_DDCO_FLAG, 0x0);
#ifndef CC_AUD_ARM_SUPPORT
    vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_CONTROL, 0x8000); // bypass post   
#endif    
#ifndef CC_AUD_DISABLE_2ND_DSP     
    vWriteDspWORD (AUD_DSP1, ADDR_RC2D_APROC_CONTROL, 0x8000); // bypass post
#endif    

#ifdef CC_AUD_ARM_RENDER
    vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_RENDER_CTRL, 0x0001); // enable ARM11 render  
#else
    vWriteDspWORD (AUD_DSP0, ADDR_RC2D_APROC_RENDER_CTRL, 0x0000); // disable ARM11 render     
#endif
#ifndef CC_AUD_DISABLE_2ND_DSP     
    vWriteDspWORD (AUD_DSP1, ADDR_RC2D_APROC_RENDER_CTRL, 0x0000); // disable ARM11 render
#endif    

#if   defined(DSP_NEW_SCRAMBLE)
    if(fgCheckSumAll != TRUE)
    {
        CheckSumAll();
        fgCheckSumAll = TRUE;
    }
    
#endif
    /* initialization for state machine*/
    vDspStateInit(AUD_DSP0); // initialization for ADSP state machine
    _uDspState[AUD_DSP0] = (UINT8)DSP_IDLE; // initially, we enter suspend mode
#ifndef CC_AUD_DISABLE_2ND_DSP     
    vDspStateInit(AUD_DSP1); // initialization for ADSP state machine
    _uDspState[AUD_DSP1] = (UINT8)DSP_IDLE; // initially, we enter suspend mode
#endif    

	/*LG A5lr modification. Init dmx getbits str and end addr*/
	AUD_WRITE32(REG_DMX_STR, DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, AUD_DEC_AUX) >> 2));
    AUD_WRITE32(REG_DMX_END, DSP_INTERNAL_ADDR(u4GetAFIFOEnd(AUD_DSP0, AUD_DEC_AUX) >> 2));
}

/******************************************************************************
* Function      : vADSPTaskInit
* Description   : initialization routine for ADSP Task
* Parameter     : None
* Return        : None
******************************************************************************/
void vADSPTaskInit(void) // first entry point of ADSP control routine
{
    DSP_CMD_QUEUE_T rDspCmdMsg;
#ifdef AUD_STATISTICS
    {
        CRIT_STATE_T rState;
        DRV_DBASE_T* prDbase = DBS_Lock(&rState);

        ASSERT(prDbase != NULL);
        //_prAud = &(prDbase->rAud);
        VERIFY(DBS_Unlock(prDbase, rState));
    }
#endif  // PSR_STATISTICS

    vADSPTaskMain_Init();

    if (!_fgTaskInit)
    {
        x_os_isr_fct pfnOldIsr;

        // Create message queue
        VERIFY(x_msg_q_create(&hDspCmdQueue, ADSPTASK_CMD_Q_NAME, sizeof(rDspCmdMsg),256) == OSR_OK);
        VERIFY(x_msg_q_create(&hDspCmdDataQueue, ADSPTASK_CMD_DATA_Q_NAME, sizeof(rDspCmdMsg),1024) == OSR_OK); //ADSP Data Queue.
        // Register ISR
        VERIFY(x_reg_isr(ADSP_INT_VECTOR, vADSPIRQHandler, &pfnOldIsr) == OSR_OK);
        //vADSPIRQEnable(TRUE);

        // Create ADSP task
        VERIFY(x_thread_create(&_hAdspThread, ADSPTASK_NAME, ADSPTASK_STACK_SIZE, ADSPTASK_THREAD_PRIORITY,
            vADSPTaskMain, 0, NULL) == OSR_OK);

        VERIFY(x_sema_create(&_hRdDspSramSema[AUD_DSP0], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hRdDspSramSema[AUD_DSP1], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hSendDspIntSema[AUD_DSP0], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hSendDspIntSema[AUD_DSP1], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _fgTaskInit = TRUE;
    }
    /*else
    {
        vADSPIRQEnable(TRUE); // remove because irq has been enabled, do not need enable twice
    }*/
}

/******************************************************************************
* Function      : fgDecoderStopped,fgDecoderRealPlay,vSendADSPCmd,vADSPPowerON,
*                 vADSPPowerOFF
* Description   : APIs for external use
* Parameter     : None
* Return        : None
******************************************************************************/
BOOL fgDecoderStopped(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    if (u1DecId < AUD_DEC_NUM)
    {
        return (_rDspStatus[u1DspId][u1DecId].fgDspStop);
    }
    return (FALSE); /* default value*/
}

BOOL fgDecoderRealPlay(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    if (u1DecId < AUD_DEC_NUM)
    {
      return(_rDspStatus[u1DspId][u1DecId].fgDspRealPlay);
    }
    return(FALSE); /* default value*/
}

#if 0   // Unused
BOOL fgDecoderPlaySent(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    if (u1DecId < AUD_DEC_NUM)
    {
      return (_rDspStatus[u1DspId][u1DecId].fgDspPlay);
    }
    return(FALSE); /* default value*/
}
#endif

BOOL fgADSPIsIdle(UINT8 u1DspId)
{
    return ( (!_rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspWakeUp) || (!_rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspWakeUpOk) );
}

void vADSPPowerON(UINT8 u1DspId)
{
    vSendADSPTaskMessage(u1DspId, ADSPTASK_MSG_POWER_ON);
}

#if 0   // Unused
void vADSPPowerOFF(UINT8 u1DspId)
{
    vSendADSPTaskMessage(u1DspId, ADSPTASK_MSG_POWER_OFF);
}
#endif
/******************************************************************************
* Function      : u4ReadShmUINT32
* Description   : read content of shared memory in UINT32
* Parameter     : u2Addr: address of shared memory
* Return        : None
******************************************************************************/
UINT32 u4ReadShmUINT32(UINT8 u1DspId, UINT16 u2Addr)
{
    return(u4ReadDspShmDWRD(u1DspId, u2Addr));
}
/******************************************************************************
* Function      : u4ReadShmUINT16
* Description   : read content of shared memory in UINT16
* Parameter     : u2Addr: address of shared memory
* Return        : None
******************************************************************************/
UINT16 u2ReadShmUINT16(UINT8 u1DspId, UINT16 u2Addr)
{
    return(u2ReadDspShmWORD(u1DspId, u2Addr));
}
/******************************************************************************
* Function      : u4ReadShmUINT32
* Description   : read content of shared memory in UINT8
* Parameter     : u2Addr: address of shared memory
* Return        : None
******************************************************************************/
UINT8 uReadShmUINT8(UINT8 u1DspId, UINT16 u2Addr)
{
    return(uReadDspShmBYTE(u1DspId, u2Addr));
}
/******************************************************************************
* Function      : vWriteShmUINT32
* Description   : write content of shared memory in UINT32
* Parameter     : u2Addr: address of shared memory, u4Value: value in UINT32
* Return        : None
******************************************************************************/
void vWriteShmUINT32(UINT8 u1DspId, UINT16 u2Addr,UINT32 u4Value)
{
    vWriteDspShmDWRD(u1DspId, u2Addr,u4Value);
}
/******************************************************************************
* Function      : vWriteShmUINT16
* Description   : write content of shared memory in UINT16
* Parameter     : u2Addr: address of shared memory, u2Value: value in UINT16
* Return        : None
******************************************************************************/
void vWriteShmUINT16(UINT8 u1DspId, UINT16 u2Addr,UINT16 u2Value)
{
    vWriteDspShmWORD(u1DspId, u2Addr,u2Value);
}

/******************************************************************************
* Function      : vWriteShmUINT8
* Description   : write content of shared memory in UINT8
* Parameter     : u2Addr: address of shared memory, uValue: value in UINT8
* Return        : None
******************************************************************************/
void vWriteShmUINT8(UINT8 u1DspId, UINT16 u2Addr,UINT32 uValue)
{
    vWriteDspShmBYTE(u1DspId, u2Addr,uValue);
}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
UINT32 _AUD_DspGetMixSndReadPtr(UINT8 u1StreamID)
{
    UINT32 u4ReadPtr;
    DSP_FlushInvalidateDCacheSmall2(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP + 16*u1StreamID);
    u4ReadPtr = (((UINT32)_i1DspMemBlock)<<24) + dReadDspCommDram (AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP + 16*u1StreamID); //TEST
    return u4ReadPtr;
}

void _AUD_DspSetMixSndWritePtr(UINT8 u1StreamID, UINT32 u4WritePtr)
{
    u4WritePtr &= 0xffffff;
    WriteDspCommDram2 (AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_WP + 16*u1StreamID, u4WritePtr); //TEST
}

void _AUD_DspShowMixSndStatus(UINT8 u1StreamID)
{
    Printf("ADDR_RC2D_DRAM_MIXSOUND_BASE: 0x%04x\n", (0x8000+ADDR_RC2D_DRAM_MIXSOUND_BASE));
    Printf("ADDR_RC2D_DRAM_MIXSOUND_STATUS(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_STATUS, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_STATUS));
    Printf("  1: working, 0: idle\n");
    Printf("ADDR_RC2D_DRAM_MIXSOUND_CONFIG(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_CONFIG,  dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CONFIG));
    Printf("  bit  0: 0-> no need upsampling, 1-> 2x upsampling\n");
    Printf("  bit  1: 2-> 4x upsampling, 3-> 8x upsampling\n");
    Printf("  bit  4: source mode : 0->mono, 1->stereo\n");
    Printf("  bit  8: mixing data to left & right output\n");
    Printf("  bit  9: mixing data to surround output\n");
    Printf("  bit 10: mixing data to center output\n");
    Printf("  bit 11: mixing data to channel 7 & 8\n");
    Printf("  bit 12: mixing data to channel 9 & 10\n");
    Printf("ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR));
    Printf("ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN));
    Printf("ADDR_RC2D_DRAM_MIXSOUND_GAIN(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_GAIN, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_GAIN));

    if (u1StreamID < MAX_AUD_MIXSND_STREAM_NUM)
    {
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_RP(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP+(UINT32)u1StreamID*16, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP+(UINT32)u1StreamID*16)); //TEST
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_WP(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_WP+(UINT32)u1StreamID*16, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_WP+(UINT32)u1StreamID*16)); //TEST
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_SA(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_SA+(UINT32)u1StreamID*16, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_SA+(UINT32)u1StreamID*16)); //TEST
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_EA(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA+(UINT32)u1StreamID*16, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA+(UINT32)u1StreamID*16)); //TESt
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_GAIN(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+(UINT32)u1StreamID*16, dReadDspCommDram(AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+(UINT32)u1StreamID*16)); //TEST
    }
    else
    {
        Printf("u1StreamID must be less than %d\n", MAX_AUD_MIXSND_STREAM_NUM);
    }
}

UINT32 u4GetMixSndGain(UINT8 u1StreamId)
{
    return dReadDspCommDram (AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN + 16*u1StreamId);
}

void vSetMixSndGain(UINT8 u1StreamId,UINT32 u4Gain)
{
    WriteDspCommDram2 (AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+u1StreamId*16, u4Gain);
}

#ifdef CC_AUD_MIXSND_FOR_ALSA
#ifdef ALSA_MIXSND_PATH
extern UINT32 u4GetMixSndRingFifoRP(UINT8 u1StreamId);
#endif
UINT32 u4GetMixSndReadPtr(UINT8 u1StreamId)
{
#ifdef ALSA_MIXSND_PATH
    return u4GetMixSndRingFifoRP(u1StreamId);
#else
    return _AUD_DspGetMixSndReadPtr(u1StreamId+ALSA_MIXSND_STREAM_ID);
#endif
}

#ifdef ALSA_MIXSND_PATH
extern void vSetMixSndRingFifoWP(UINT8 u1StreamId, UINT32 u4WritePtr);
#endif
void vSetMixSndWritePtr(UINT8 u1StreamId, UINT32 u4WritePtr)
{
#ifdef ALSA_MIXSND_PATH
    vSetMixSndRingFifoWP(u1StreamId, u4WritePtr);
#else
    _AUD_DspSetMixSndWritePtr(u1StreamId+ALSA_MIXSND_STREAM_ID, u4WritePtr);
#endif
}
#endif
#if defined(CC_AUD_MIXSND_FOR_ALSA) || defined(CC_ENABLE_AOMX)
UINT32 u4GetMixSndFIFOStart(UINT8 u1StreamId)
{
#ifdef ALSA_MIXSND_PATH
    return u4GetMixSoundStartByteAddr3()+u1StreamId*u4GetMixSoundBufSize3()/MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;
#else
    UINT32 u4SA;
    u4SA = (((UINT32)_i1DspMemBlock)<<24) + dReadDspCommDram (AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_SA + (UINT32)16*(u1StreamId+ALSA_MIXSND_STREAM_ID));
    return u4SA;
#endif
}

UINT32 u4GetMixSndFIFOEnd(UINT8 u1StreamId)
{
#ifdef ALSA_MIXSND_PATH
    return u4GetMixSoundStartByteAddr3()+(u1StreamId+1)*u4GetMixSoundBufSize3()/MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;
#else
    UINT32 u4EA;
    u4EA = (((UINT32)_i1DspMemBlock)<<24) + dReadDspCommDram (AUD_DSP0, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA + (UINT32)16*(u1StreamId+ALSA_MIXSND_STREAM_ID));
    return u4EA;
#endif
}
#endif
#endif

#include "dsp_data.h"
#if defined(CC_MT5890) || defined(CC_MT5882)
#include "../../fbm8290/fbm_pool_config.h"
#else
#include "../../fbm8199/fbm_pool_config.h"
#endif
extern UINT32 u4ReadDspSram(UINT8 u1DspId, UINT32 u4Addr);
extern void AudShowAsrcBufferStatus(UINT8 u1SrcID,UINT8 u1ChMsk,BOOL fgUpdateRWP);

extern UINT32 u4GetMixSoundStartByteAddr(void);
extern UINT32 u4GetMixSoundBufSize(void);
extern UINT32 u4GetMixSoundStartByteAddr2(void);
extern UINT32 u4GetMixSoundBufSize2(void);
#ifdef ALSA_MIXSND_PATH
extern UINT32 u4GetMixSoundStartByteAddr3(void);
extern UINT32 u4GetMixSoundBufSize3(void);
#endif

void AUD_QueryDspMemory(UINT8 u1MemSpace)
{
    if (u1MemSpace == 0)
    {
        Printf("  AUD_DSP0 :\n");
        Printf("  DSP_SHARED_INFO: %x\n", DSP_SHARED_INFO_SZ(AUD_DSP0));
        Printf("  DSP_CODE_SZ: %x\n", DSP_CODE_SZ(AUD_DSP0));
        Printf("  DSP_DATA_SZ: %x\n", DSP_DATA_SZ(AUD_DSP0));
        Printf("  DSP_AFIFO_SZ: %x\n", DSP_AFIFO_SZ(AUD_DSP0));
        Printf("  DSP_AFIFO_SZ_DEC2: %x\n", DSP_AFIFO_SZ_DEC2(AUD_DSP0));
        Printf("  DSP_AFIFO_SZ_DEC3: %x\n", DSP_AFIFO_SZ_DEC3(AUD_DSP0));
        Printf("  DSP_AFIFO_SZ_DEC4: %x\n", DSP_AFIFO_SZ_DEC4(AUD_DSP0));
        Printf("  DSP_AFIFO_HDMI_PARSER_SZ: %x\n", DSP_AFIFO_HDMI_PARSER_SZ);
        Printf("+ DSP_MIXSNDBUF_ALL_SZ: %x\n", DSP_MIXSNDBUF_ALL_SZ);
        Printf("================================================\n");
        Printf("  %x\n", FBM_DSP_SIZE);

        Printf("  AUD_DSP1 :\n");
        Printf("  DSP_SHARED_INFO: %x\n", DSP_SHARED_INFO_SZ(AUD_DSP1));
        Printf("  DSP_CODE_SZ: %x\n", DSP_CODE_SZ(AUD_DSP1));
        Printf("  DSP_DATA_SZ: %x\n", DSP_DATA_SZ(AUD_DSP1));
        Printf("  DSP_AFIFO_SZ: %x\n", DSP_AFIFO_SZ(AUD_DSP1));
        Printf("  DSP_AFIFO_SZ_DEC2: %x\n", DSP_AFIFO_SZ_DEC2(AUD_DSP1));
        Printf("  DSP_AFIFO_SZ_DEC3: %x\n", DSP_AFIFO_SZ_DEC3(AUD_DSP1));
        Printf("  DSP_AFIFO_SZ_DEC4: %x\n", DSP_AFIFO_SZ_DEC4(AUD_DSP1));
        Printf("================================================\n");
        Printf("  %x\n", FBM_DSP_SIZE);        

        
    }

    if (u1MemSpace == 1)
    {
#ifndef DSP_SUPPORT_DUAL_DECODE
        Printf("[DSP Code Space]\n");
        Printf("  ROM : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[0]<<8), VIRTUAL(_u4DspICacheAddr[0]<<8), DSP_ROM_SZ);
        Printf("  POS : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[1]<<8), VIRTUAL(_u4DspICacheAddr[1]<<8), DSP_POS_RAM_SZ);
        Printf("  CRI : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[2]<<8), VIRTUAL(_u4DspICacheAddr[2]<<8), DSP_CTR_RAM_SZ);
        Printf("  COM : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[3]<<8), VIRTUAL(_u4DspICacheAddr[3]<<8), DSP_CMM_RAM_SZ);
        Printf("  DEC4: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[4]<<8), VIRTUAL(_u4DspICacheAddr[4]<<8), DSP_DEC4_RAM_SZ);
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[5]<<8), VIRTUAL(_u4DspICacheAddr[5]<<8), DSP_DEC1_RAM_SZ);
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[6]<<8), VIRTUAL(_u4DspICacheAddr[6]<<8), DSP_DEC3_RAM_SZ);
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[7]<<8), VIRTUAL(_u4DspICacheAddr[7]<<8), DSP_DEC2_RAM_SZ);
#else
        Printf("[DSP0 Code Space 1]\n");
        Printf("  ROM : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][0]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][0]<<8), DSP_ROM_SZ(AUD_DSP0));
        Printf("  CMMA : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][1]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][1]<<8), DSP_CMMA_RAM_SZ(AUD_DSP0));
        Printf("  CRI : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][2]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][2]<<8), DSP_CTR_RAM_SZ(AUD_DSP0));
        Printf("  COMAX : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][3]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][3]<<8), DSP_CMMAEXT_RAM_SZ(AUD_DSP0));
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][4]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][4]<<8), DSP_DEC3_RAM_SZ(AUD_DSP0));
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][5]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][5]<<8), DSP_DEC1_RAM_SZ(AUD_DSP0));
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][6]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][6]<<8), DSP_DEC2_RAM_SZ(AUD_DSP0));
        Printf("[DSP0 Code Space 2]\n");
        Printf("  ROM : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][7]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][7]<<8), DSP_ROM_SZ(AUD_DSP0));
        Printf("  POS : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][8]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][8]<<8), DSP_POS_RAM_SZ(AUD_DSP0));
        Printf("  CMMB : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][9]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][9]<<8), DSP_CMMB_RAM_SZ(AUD_DSP0));
        Printf("  RESER : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][10]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][10]<<8), DSP_RESER_RAM_SZ(AUD_DSP0));
        Printf("  ENC1 : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][11]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][11]<<8), DSP_DEC4_RAM_SZ(AUD_DSP0));
        Printf("  ENC2 : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP0][12]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP0][12]<<8), DSP_ENC2_RAM_SZ(AUD_DSP0));
        
        Printf("[DSP1 Code Space 1]\n");
        Printf("  ROM : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][0]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][0]<<8), DSP_ROM_SZ(AUD_DSP1));
        Printf("  CMMA : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][1]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][1]<<8), DSP_CMMA_RAM_SZ(AUD_DSP1));
        Printf("  CRI : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][2]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][2]<<8), DSP_CTR_RAM_SZ(AUD_DSP1));
        Printf("  COMAX : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][3]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][3]<<8), DSP_CMMAEXT_RAM_SZ(AUD_DSP1));
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][4]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][4]<<8), DSP_DEC3_RAM_SZ(AUD_DSP1));
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][5]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][5]<<8), DSP_DEC1_RAM_SZ(AUD_DSP1));
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][6]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][6]<<8), DSP_DEC2_RAM_SZ(AUD_DSP1));
        Printf("[DSP1 Code Space 2]\n");
        Printf("  ROM : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][7]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][7]<<8), DSP_ROM_SZ(AUD_DSP1));
        Printf("  POS : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][8]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][8]<<8), DSP_POS_RAM_SZ(AUD_DSP1));
        Printf("  CMMB : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][9]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][9]<<8), DSP_CMMB_RAM_SZ(AUD_DSP1));
        Printf("  RESER : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][10]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][10]<<8), DSP_RESER_RAM_SZ(AUD_DSP1));
        Printf("  ENC1 : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][11]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][11]<<8), DSP_DEC4_RAM_SZ(AUD_DSP1));
        Printf("  ENC2 : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[AUD_DSP1][12]<<8), VIRTUAL(_u4DspICacheAddr[AUD_DSP1][12]<<8), DSP_ENC2_RAM_SZ(AUD_DSP1));        
#endif
    }

    if (u1MemSpace == 2)
    {
        Printf("[DSP0 Normal Space]\n");
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP0][0]), VIRTUAL(_u4DspDramBuf[AUD_DSP0][0]), DSP_DAT_DEC1_SZ(AUD_DSP0));
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP0][1]), VIRTUAL(_u4DspDramBuf[AUD_DSP0][1]), DSP_DAT_DEC2_SZ(AUD_DSP0));
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP0][2]), VIRTUAL(_u4DspDramBuf[AUD_DSP0][2]), DSP_DAT_DEC3_SZ(AUD_DSP0));
        Printf("  COMM: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP0][3]), VIRTUAL(_u4DspDramBuf[AUD_DSP0][3]), DSP_DAT_CMM_NORMAL_SZ(AUD_DSP0));
        Printf("  DEC4: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP0][4]), VIRTUAL(_u4DspDramBuf[AUD_DSP0][4]), DSP_DAT_DEC4_SZ(AUD_DSP0));
        Printf("  MIX : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP0][5]), VIRTUAL(_u4DspDramBuf[AUD_DSP0][5]), DSP_MIXSNDBUF_ALL_SZ);
        
        printf("[DSP1 Normal Space]\n");
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP1][0]), VIRTUAL(_u4DspDramBuf[AUD_DSP1][0]), DSP_DAT_DEC1_SZ(AUD_DSP1));
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP1][1]), VIRTUAL(_u4DspDramBuf[AUD_DSP1][1]), DSP_DAT_DEC2_SZ(AUD_DSP1));
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP1][2]), VIRTUAL(_u4DspDramBuf[AUD_DSP1][2]), DSP_DAT_DEC3_SZ(AUD_DSP1));
        Printf("  COMM: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP1][3]), VIRTUAL(_u4DspDramBuf[AUD_DSP1][3]), DSP_DAT_CMM_NORMAL_SZ(AUD_DSP1));
        Printf("  DEC4: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP1][4]), VIRTUAL(_u4DspDramBuf[AUD_DSP1][4]), DSP_DAT_DEC4_SZ(AUD_DSP1));
        Printf("  MIX : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[AUD_DSP1][5]), VIRTUAL(_u4DspDramBuf[AUD_DSP1][5]), DSP_MIXSNDBUF_ALL_SZ);
    }

    if (u1MemSpace == 3)
    {
        Printf("[DSP0 Compact Space]\n");
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP0][0]), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][0]), DSP_DAT_DEC1_SZ(AUD_DSP0));
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP0][1]), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][1]), DSP_DAT_DEC2_SZ(AUD_DSP0));
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP0][2]), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][2]), DSP_DAT_DEC3_SZ(AUD_DSP0));
        Printf("  COMM: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP0][3]), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][3]), DSP_DAT_CMM_SZ(AUD_DSP0));
        Printf("  DEC4: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP0][4]), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][4]), DSP_DAT_DEC4_SZ(AUD_DSP0));
        Printf("  MIX : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP0][5]), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][5]), DSP_MIXSNDBUF_ALL_SZ);
#if defined(CC_AUD_ARM11_SUPPORT)
        Printf("  APROC: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP0][6]), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][6]), DSP_APROC_SZ);
#endif // CC_AUD_ARM11_SUPPORT
        Printf("[DSP1 Compact Space]\n");
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP1][0]), VIRTUAL(_u4DspCmptBuf[AUD_DSP1][0]), DSP_DAT_DEC1_SZ(AUD_DSP1));
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP1][1]), VIRTUAL(_u4DspCmptBuf[AUD_DSP1][1]), DSP_DAT_DEC2_SZ(AUD_DSP1));
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP1][2]), VIRTUAL(_u4DspCmptBuf[AUD_DSP1][2]), DSP_DAT_DEC3_SZ(AUD_DSP1));
        Printf("  COMM: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP1][3]), VIRTUAL(_u4DspCmptBuf[AUD_DSP1][3]), DSP_DAT_CMM_SZ(AUD_DSP1));
        Printf("  DEC4: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP1][4]), VIRTUAL(_u4DspCmptBuf[AUD_DSP1][4]), DSP_DAT_DEC4_SZ(AUD_DSP1));
        Printf("  MIX : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP1][5]), VIRTUAL(_u4DspCmptBuf[AUD_DSP1][5]), DSP_MIXSNDBUF_ALL_SZ);    
#if defined(CC_AUD_ARM11_SUPPORT)
        Printf("  APROC: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[AUD_DSP1][6]), VIRTUAL(_u4DspCmptBuf[AUD_DSP1][6]), DSP_APROC_SZ);
#endif // CC_AUD_ARM11_SUPPORT
    }

    if (u1MemSpace == 4)
    {
        Printf("[DSP0 AFIFO Space]\n");
        Printf("  AFIFO1: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[AUD_DSP0][0][0], VIRTUAL(_u4AFIFO[AUD_DSP0][0][0]), _u4AFIFO[AUD_DSP0][0][1], VIRTUAL(_u4AFIFO[AUD_DSP0][0][1]));
        Printf("  AFIFO2: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[AUD_DSP0][1][0], VIRTUAL(_u4AFIFO[AUD_DSP0][1][0]), _u4AFIFO[AUD_DSP0][1][1], VIRTUAL(_u4AFIFO[AUD_DSP0][1][1]));
        Printf("  AFIFO3: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[AUD_DSP0][2][0], VIRTUAL(_u4AFIFO[AUD_DSP0][2][0]), _u4AFIFO[AUD_DSP0][2][1], VIRTUAL(_u4AFIFO[AUD_DSP0][2][1]));
        Printf("  AFIFO4: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[AUD_DSP0][3][0], VIRTUAL(_u4AFIFO[AUD_DSP0][3][0]), _u4AFIFO[AUD_DSP0][3][1], VIRTUAL(_u4AFIFO[AUD_DSP0][3][1]));

        Printf("[DSP1 AFIFO Space]\n");
        Printf("  AFIFO1: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[AUD_DSP1][0][0], VIRTUAL(_u4AFIFO[AUD_DSP1][0][0]), _u4AFIFO[AUD_DSP1][0][1], VIRTUAL(_u4AFIFO[AUD_DSP1][0][1]));
        Printf("  AFIFO2: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[AUD_DSP1][1][0], VIRTUAL(_u4AFIFO[AUD_DSP1][1][0]), _u4AFIFO[AUD_DSP1][1][1], VIRTUAL(_u4AFIFO[AUD_DSP1][1][1]));
        Printf("  AFIFO3: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[AUD_DSP1][2][0], VIRTUAL(_u4AFIFO[AUD_DSP1][2][0]), _u4AFIFO[AUD_DSP1][2][1], VIRTUAL(_u4AFIFO[AUD_DSP1][2][1]));
        Printf("  AFIFO4: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[AUD_DSP1][3][0], VIRTUAL(_u4AFIFO[AUD_DSP1][3][0]), _u4AFIFO[AUD_DSP1][3][1], VIRTUAL(_u4AFIFO[AUD_DSP1][3][1]));
    }

    if (u1MemSpace == 5)
    {
        Printf("[Internal Buffer Space]\n");
        Printf("  IntBuf1: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF*4), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF*4), DRAM_INTERNAL_BUF_SIZE*4);
        Printf("  IntBuf2: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF2*4), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF2*4), DRAM_INTERNAL_BUF2_SIZE*4);
        Printf("  IntBuf3: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF3*4), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF3*4), DRAM_INTERNAL_BUF3_SIZE*4);
        Printf("  IntBuf4: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF4*4), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF4*4), DRAM_INTERNAL_BUF4_SIZE*4);
        Printf("  IntBuf5: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF5*4), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF5*4), DRAM_INTERNAL_BUF5_SIZE*4);
        Printf("  IntBuf6: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF6*4), VIRTUAL(_u4DspCmptBuf[AUD_DSP0][3] + DRAM_INTERNAL_BUF6*4), DRAM_INTERNAL_BUF6_SIZE*4);
    }

    if (u1MemSpace == 6)
    {
        Printf("[ASRC Space]\n");
        AudShowAsrcBufferStatus(0,0x1f,FALSE);
    }

    if (u1MemSpace == 7)
    {
        int i;
        UINT32 u4Addr;
        UINT32 u4AoutCmptPage;

        Printf("[DSP0 AOUT Space]\n");

        u4AoutCmptPage = ((u4ReadDspSram(AUD_DSP0, REG_DSP_AOUT_CTRL)>>12)&0x7);
        Printf("  AOUT1 Cmpt Page: %d Ch Num: %d\n", u4AoutCmptPage, (u4ReadDspSram(AUD_DSP0, REG_DSP_CH_NUM)>>8));
        Printf("        CH CFG: L: %x R: %x SL: %x SR: %x C: %x LFE: %x CH7: %x CH8: %x CH9: %x CH10: %x\n",
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_0)>>8)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_0)>>12)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_1)>>8)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_1)>>12)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_0)>>16)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_1)>>16)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_0)>>20)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_1)>>20)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_2)>>8)&0xf),
                  ((u4ReadDspSram(AUD_DSP0, REG_DSP_CH_CFG_2)>>12)&0xf));
        for (i=0;i<11;i++)
        {
            u4Addr = _u4DspCmptBuf[AUD_DSP0][u4AoutCmptPage] + (u4ReadDspSram(AUD_DSP0, REG_DSP_CHL_1_ADDR+i)<<2);
            Printf("        CH%d addr: (P)0x%08x/(V)0x%08x\n", i+1, u4Addr, VIRTUAL(u4Addr));
        }

        u4AoutCmptPage = ((u4ReadDspSram(AUD_DSP0, REG_DSP_AOUT_CTRL_2)>>12)&0x7);
        Printf("  AOUT2 Cmpt Page: %d Ch Num: %d\n", u4AoutCmptPage, (u4ReadDspSram(AUD_DSP0, REG_DSP_CH_NUM_2)>>8));
        for (i=0;i<3;i++)
        {
            u4Addr = _u4DspCmptBuf[AUD_DSP0][u4AoutCmptPage] + (u4ReadDspSram(AUD_DSP0, REG_DSP_CHL_1_ADDR_2+i)<<2);
            Printf("        CH%d addr: (P)0x%08x/(V)0x%08x\n", i+1, u4Addr, VIRTUAL(u4Addr));
        }
        
        Printf("[DSP1 AOUT Space]\n");

        u4AoutCmptPage = ((u4ReadDspSram(AUD_DSP1, REG_DSP_AOUT_CTRL)>>12)&0x7);
        Printf("  AOUT1 Cmpt Page: %d Ch Num: %d\n", u4AoutCmptPage, (u4ReadDspSram(AUD_DSP1, REG_DSP_CH_NUM)>>8));
        Printf("        CH CFG: L: %x R: %x SL: %x SR: %x C: %x LFE: %x CH7: %x CH8: %x CH9: %x CH10: %x\n",
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_0)>>8)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_0)>>12)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_1)>>8)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_1)>>12)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_0)>>16)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_1)>>16)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_0)>>20)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_1)>>20)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_2)>>8)&0xf),
                  ((u4ReadDspSram(AUD_DSP1, REG_DSP_CH_CFG_2)>>12)&0xf));
        for (i=0;i<11;i++)
        {
            u4Addr = _u4DspCmptBuf[AUD_DSP1][u4AoutCmptPage] + (u4ReadDspSram(AUD_DSP1, REG_DSP_CHL_1_ADDR+i)<<2);
            Printf("        CH%d addr: (P)0x%08x/(V)0x%08x\n", i+1, u4Addr, VIRTUAL(u4Addr));
        }

        u4AoutCmptPage = ((u4ReadDspSram(AUD_DSP1, REG_DSP_AOUT_CTRL_2)>>12)&0x7);
        Printf("  AOUT2 Cmpt Page: %d Ch Num: %d\n", u4AoutCmptPage, (u4ReadDspSram(AUD_DSP1, REG_DSP_CH_NUM_2)>>8));
        for (i=0;i<3;i++)
        {
            u4Addr = _u4DspCmptBuf[AUD_DSP1][u4AoutCmptPage] + (u4ReadDspSram(AUD_DSP1, REG_DSP_CHL_1_ADDR_2+i)<<2);
            Printf("        CH%d addr: (P)0x%08x/(V)0x%08x\n", i+1, u4Addr, VIRTUAL(u4Addr));
        }
    }

    if (u1MemSpace == 8)
    {
        Printf("[Upload Buffer]\n");
        Printf("   SA: (P)0x%08x/(V)0x%08x Size: %x\n",
                u4GetUploadFIFOStart(),
                VIRTUAL(u4GetUploadFIFOStart()),
                u4GetUploadFIFOEnd() - u4GetUploadFIFOStart());
    }

    if (u1MemSpace == 9)
    {
        Printf("[MixSnd Buffer]\n");
        Printf("  MIXSND1 SA: (P)0x%08x/(V)0x%08x Size: %x\n",
                u4GetMixSoundStartByteAddr(),
                VIRTUAL(u4GetMixSoundStartByteAddr()),
                u4GetMixSoundBufSize());
        Printf("  MIXSND2 SA: (P)0x%08x/(V)0x%08x Size: %x\n",
                u4GetMixSoundStartByteAddr2(),
                VIRTUAL(u4GetMixSoundStartByteAddr2()),
                u4GetMixSoundBufSize2());
        Printf("  MIXSND3 SA: (P)0x%08x/(V)0x%08x Size: %x\n",
                u4GetMixSoundStartByteAddr3(),
                VIRTUAL(u4GetMixSoundStartByteAddr3()),
                u4GetMixSoundBufSize3());
    } 
}

void AUD_DumpDspMemory(UINT8 u1Option, UINT32 u4Size)
{
    UINT32 i;
    UINT32 u4SA;

    switch (u1Option)
    {
        case 0:
            Printf("[Dump AFIFO1]\n");
            u4SA = VIRTUAL(_u4AFIFO[AUD_DSP0][0][0]);
            break;
        case 1:
            Printf("[Dump AFIFO2]\n");
            u4SA = VIRTUAL(_u4AFIFO[AUD_DSP0][1][0]);
            break;
        case 2:
            Printf("[Dump AFIFO3]\n");
            u4SA = VIRTUAL(_u4AFIFO[AUD_DSP0][2][0]);
            break;
        case 3:
            Printf("[Dump AFIFO4]\n");
            u4SA = VIRTUAL(_u4AFIFO[AUD_DSP0][3][0]);
            break;
        case 4:
            Printf("[Dump UPLOAd]\n");
            u4SA = VIRTUAL(u4GetUploadFIFOStart());
            break;
        case 5:
            Printf("[Dump ALSA]\n");
            u4SA = VIRTUAL(u4GetMixSoundStartByteAddr3());
            break;
        case 6:
            Printf("[Dump MIXCLIP]\n");
            u4SA = VIRTUAL(u4GetMixSoundStartByteAddr3()-0x4000);
            break;

        default:
            return;
}

    for (i=0;i<u4Size;i++)
    {
        if ((i&0xf)==0)
        {
            Printf("\n%08xh: ", (u4SA+i));
        }
        Printf(" %02x", *((UINT8*)(u4SA+i)));
    }
}

void vDSPLockReadSramSema(UINT8 u1DspId)
{
    VERIFY(x_sema_lock(_hRdDspSramSema[u1DspId], X_SEMA_OPTION_WAIT)== OSR_OK);
}

void vDSPUnlockReadSramSema(UINT8 u1DspId)
{
    VERIFY(x_sema_unlock(_hRdDspSramSema[u1DspId])== OSR_OK);
}

void ADAC_SIFSrcSel(UINT32 isUseSIF)
{
    UINT32 u4Data;

    // Set AUDIN_CFG
    u4Data = (AUD_READ32(REG_AUDIN_CFG) & (~REG_AUDIN_CFG_MASK));
    u4Data = u4Data | APGA_GAIN_0DB | APGA_OFFSET_0V | APGA_GAIN_BY_DSP | ADCK_INV_RISING;
    AUD_WRITE32(REG_AUDIN_CFG, u4Data);

    if(isUseSIF)
    {
        ANA_WRITE32(REG_CVBS_CFG4, (AUD_READ32(REG_CVBS_CFG4)&0x00ffff7f) | 0x40000080);
        ANA_SET_BIT(REG_CVBS_CFG6, 0x20000000);
        ANA_CLR_BIT(REG_CVBS_CFG7, 0x00a0b880);
        
        u4Data = (AUD_READ32(REG_AUDIN_CFG) & (~ADC_SEL_DEMOD));
        AUD_WRITE32(REG_AUDIN_CFG, u4Data);
    }
    else //use ATD
    {
        u4Data = (AUD_READ32(REG_AUDIN_CFG)) | (ADC_SEL_DEMOD);
        AUD_WRITE32(REG_AUDIN_CFG, u4Data);
    }

}

#ifdef CC_AUD_DDI
void vSoundBarUpdate(void)
{
    vWriteDspWORD (AUD_DSP0, ADDR_RC2D_IEC_CHANNEL_CFG2, ((UINT16) uReadDspShmBYTE (AUD_DSP0, B_IEC_SOUNDBAR)));
}
#endif

UINT32 vGetFBMCMMSize(void)
{
    return DSP_DAT_CMM_SZ(AUD_DSP0);
}

