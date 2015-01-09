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
 * $RCSfile: dsp_rg_ctl.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_rg_ctl.c
 *  Brief of file dsp_rg_ctl.c.
 *  Details of file dsp_rg_ctl.c (optional).
 */

#define DSP_RGCTL_C
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "dsp_common.h"
#include "dsp_reg.h"
#include "dsp_data.h"
#include "dsp_rg_ctl.h"
#include "dsp_func.h"
#include "dsp_shm.h"
#include "adsp_task.h"
#include "dsp_table.h"
#include "dsp_intf.h"
#include "x_assert.h"

#include "aud_if.h"
#include "aud_debug.h"
#include "drv_adsp.h"
#include "d2rc_shm.h"
#include "aud_hw.h"
#include "drvcust_if.h"

#include "x_os.h"
#include "x_hal_5381.h"
#include "x_ckgen.h"
#include "x_pinmux.h"

LINT_EXT_HEADER_END

//Check DRAM Allocation
#include "../../fbm8290/fbm_pool_config.h"

#if ((DSP_SHARED_INFO_SZ(0) + DSP_SHARED_INFO_SZ(1) + DSP_CODE_SZ(0) +DSP_CODE_SZ(1) + DSP_DATA_SZ(0) +DSP_DATA_SZ(1)) != FBM_AUD_BASE)
#error "DSP_SHARED_INFO_SZ + DSP_CODE_SZ + DSP_DATA_SZ!= FBM_AUD_BASE"
#endif
#if (DSP_AFIFO_SZ(0) != FBM_AUD_AFIFO_DSP0)
#error "DSP_AFIFO_SZ(AUD_DSP0) != FBM_AUD_AFIFO(AUD_DSP0)"
#endif
#if (DSP_AFIFO_SZ(1) != FBM_AUD_AFIFO_DSP1)
#error "DSP_AFIFO_SZ(AUD_DSP1) != FBM_AUD_AFIFO_DSP1"
#endif
#if (DSP_AFIFO_SZ_DEC2(0) != FBM_AUD_AFIFO2_DSP0)
#error "DSP_AFIFO_SZ_DEC2(AUD_DSP0) != FBM_AUD_AFIFO2(AUD_DSP0)"
#endif
#if (DSP_AFIFO_SZ_DEC2(1) != FBM_AUD_AFIFO2_DSP1)
#error "DSP_AFIFO_SZ_DEC2(AUD_DSP1) != FBM_AUD_AFIFO2(AUD_DSP1)"
#endif
#if (DSP_AFIFO_SZ_DEC3(0) != FBM_AUD_AFIFO3_DSP0)
#error "DSP_AFIFO_SZ_DEC3(AUD_DSP0) != FBM_AUD_AFIFO3(AUD_DSP0)"
#endif
#if (DSP_AFIFO_SZ_DEC3(1) != FBM_AUD_AFIFO3_DSP1)
#error "DSP_AFIFO_SZ_DEC3(AUD_DSP1) != FBM_AUD_AFIFO3(AUD_DSP1)"
#endif
#if (DSP_AFIFO_SZ_DEC4(0) != FBM_AUD_AFIFO4_DSP0)
#error "DSP_AFIFO_SZ_DEC4(AUD_DSP0) != FBM_AUD_AFIFO4(AUD_DSP0)"
#endif
#if (DSP_AFIFO_SZ_DEC4(1) != FBM_AUD_AFIFO4_DSP1)
#error "DSP_AFIFO_SZ_DEC4(AUD_DSP1) != FBM_AUD_AFIFO4(AUD_DSP1)"
#endif
#if (DSP_AFIFO_HDMI_PARSER_SZ != FBM_AUD_HDMI_PARSER)
#error "DSP_AFIFO_HDMI_PARSER_SZ != FBM_AUD_HDMI_PARSER"
#endif
#if (DSP_MIXSNDBUF_ALL_SZ != FBM_AUD_MIXSNDBUF)
#error "DSP_MIXSNDBUF_ALL_SZ != FBM_AUD_MIXSNDBUF"
#endif
#if (DSP_APROC_SZ != FBM_AUD_APROC_WORKSPACE)
#error "DSP_APROC_SZ != FBM_AUD_APROC_WORKSPACE"
#endif

//End

UINT8 _uDspState[AUD_DSP_NUM];
BOOL _fgRamBoot[AUD_DSP_NUM];
UINT8 _uDspSIntAddr[AUD_DSP_NUM];
UINT32 _u4DspSIntSD[AUD_DSP_NUM];
UINT32 _u4DspSIntLD[AUD_DSP_NUM];
extern HANDLE_T _hSendDspIntSema[AUD_DSP_NUM];

extern void vCloseIec(void);
extern void vOpenIec(void);
extern INT16 AUD_SPDIFGet(void);

//-----------------------------------------------------------------------------
// IniBlkRegs
//
/** Initial (1) Start block registers.
 *          (2) Putbits registers.
 *          (3) Compact mode block registers.
 *          (4) Instruction cache registers.
 */
//-----------------------------------------------------------------------------
void IniBlkRegs (UINT8 u1DspId)
{
    UINT32 dwBlkData;

    dwBlkData = (_u4DspDramBuf[u1DspId][0] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[u1DspId][1] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_DRAM_SBLK01(u1DspId), dwBlkData);

    dwBlkData = (_u4DspDramBuf[u1DspId][2] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[u1DspId][3] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_DRAM_SBLK23(u1DspId), dwBlkData);

    dwBlkData = (_u4DspDramBuf[u1DspId][4] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[u1DspId][5] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_DRAM_SBLK45(u1DspId), dwBlkData);

    dwBlkData = (_u4DspDramBuf[u1DspId][6] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspDramBuf[u1DspId][7] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_DRAM_SBLK67(u1DspId), dwBlkData);

    dwBlkData = (_u4DspCmptBuf[u1DspId][0] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[u1DspId][1] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_CMPT_DRAM_SBLK01(u1DspId), dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = (_u4DspCmptBuf[u1DspId][2] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[u1DspId][3] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_CMPT_DRAM_SBLK23(u1DspId), dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = (_u4DspCmptBuf[u1DspId][4] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[u1DspId][5] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_CMPT_DRAM_SBLK45(u1DspId), dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = (_u4DspCmptBuf[u1DspId][6] >> 8) & 0xFFFF;
    dwBlkData = dwBlkData | ((_u4DspCmptBuf[u1DspId][7] << 8) & 0xFFFF0000);
    AUD_WRITE32(REG_DSP_CMPT_DRAM_SBLK67(u1DspId), dwBlkData);  // Temp hack for cmpt mode

    dwBlkData = _u4DspICacheAddr[u1DspId][0] & 0xFFFF;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[u1DspId][1] << 16);
    AUD_WRITE32(REG_DSP_IC_DRAM_SBLK01(u1DspId), dwBlkData);

    dwBlkData = ((UINT32) (_u4DspICacheAddr[u1DspId][2])) & 0xFFFF;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[u1DspId][3] << 16);
    AUD_WRITE32(REG_DSP_IC_DRAM_SBLK23(u1DspId), dwBlkData);

    dwBlkData = ((UINT32) (_u4DspICacheAddr[u1DspId][4])) & 0xFFFF;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[u1DspId][5] << 16);
    AUD_WRITE32(REG_DSP_IC_DRAM_SBLK45(u1DspId), dwBlkData);

    dwBlkData = ((UINT32) (_u4DspICacheAddr[u1DspId][6])) & 0xFFFF;
    dwBlkData = dwBlkData | (_u4DspICacheAddr[u1DspId][7] << 16);
    AUD_WRITE32(REG_DSP_IC_DRAM_SBLK67(u1DspId), dwBlkData);

#ifndef DSP_SUPPORT_DUAL_DECODE
    AUD_WRITE32(REG_DSP_IPGSET0,
                (((DSP_POS_PG_OFST>>10)&0xff)<<0)|
                (((DSP_CTR_PG_OFST>>10)&0xff)<<8)|
                (((DSP_CMM_PG_OFST>>10)&0xff)<<16) |
                (((DSP_DEC4_PG_OFST>>10)&0xff)<<24));

    AUD_WRITE32(REG_DSP_IPGSET1,
                (((DSP_DEC1_PG_OFST>>10)&0xff)<<0)|
                (((DSP_DEC3_PG_OFST>>10)&0xff)<<8)|
                (((DSP_DEC2_PG_OFST>>10)&0xff)<<16));
#else
    AUD_WRITE32(REG_DSP_IPGSET0(u1DspId),
                (((DSP_CMMA_PG_OFST(u1DspId)>>10)&0xff)<<0)|
                (((DSP_CTR_PG_OFST(u1DspId)>>10)&0xff)<<8)|
                (((DSP_CMMAEXT_PG_OFST(u1DspId)>>10)&0xff)<<16) |
                (((DSP_DEC3_PG_OFST(u1DspId)>>10)&0xff)<<24));

    AUD_WRITE32(REG_DSP_IPGSET1(u1DspId),
                (((DSP_DEC1_PG_OFST(u1DspId)>>10)&0xff)<<0)|
                (((DSP_DEC2_PG_OFST(u1DspId)>>10)&0xff)<<8)|
                (((DSP_ROM2_PG_OFST(u1DspId)>>10)&0xff)<<16));

#endif  //dual decode

#if 1 //def DSP_TEST_PUTBITS                         // Putbits test
    {
    extern UINT32 _u4DspMemBufAddr;
    UINT32 _u4DspPutbBufAddr = _u4DspMemBufAddr;
    UINT32 _u4DspPutbBuf0=0, _u4DspPutbBuf1=0;

    dwBlkData= ((_u4DspPutbBufAddr + DSP_PUTBITS_SA) >> 8) & 0x0ffff;
    _u4DspPutbBuf0 |= (dwBlkData << 16);
    dwBlkData += 4;
    _u4DspPutbBuf0 |= dwBlkData;
    AUD_WRITE32(REG_PUTBS_BLK0_RISC, _u4DspPutbBuf0);
    _u4DspPutbBuf1 |= (dwBlkData << 16);
    dwBlkData += 4;
    _u4DspPutbBuf1 |= dwBlkData;
    AUD_WRITE32(REG_PUTBS_BLK1_RISC, _u4DspPutbBuf1);
    }
#endif
}
//-----------------------------------------------------------------------------
// fgDspBusy
//
/** Check DSP Status.
 *
 *  @retval     TRUE (DSP busy) / FALSE (DSP not busy).
 */
//-----------------------------------------------------------------------------
BOOL fgDspBusy (UINT8 u1DspId)
{
    return (AUD_READ32(REG_R2D_STA(u1DspId)) & DSP_B);
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// vDSPSendInt
//
/** Send Interrupt to DSP.
 */
//-----------------------------------------------------------------------------
void vDspSendInt (UINT8 u1DspId)
{
    static INT32 i4IntCnt = 0;

    LOG(9, "[DSP_TASK] vDspSendInt (%d)\n",i4IntCnt);
    i4IntCnt ++;
    AUD_WRITE32(REG_LD(u1DspId), _u4DspSIntLD[u1DspId]); // long data
    SendDSPInt(u1DspId, _uDspSIntAddr[u1DspId], _u4DspSIntSD[u1DspId]);   // short data & interrupt
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspSInt = FALSE;
}
#endif

//-----------------------------------------------------------------------------
// dwReadDspLongData
//
/** Read Long data from DSP.
 */
//-----------------------------------------------------------------------------
UINT32 dwReadDspLongData (UINT8 u1DspId)
{
    return (AUD_READ32(REG_D2R_RD(u1DspId)));
}

//-----------------------------------------------------------------------------
// vSendDspLongInt
//
/** Send Interrupt to DSP.
 */
//-----------------------------------------------------------------------------
void vSendDspLongInt (UINT8 u1DspId, UINT32 u4Addr, UINT32 u4ShortD, UINT32 u4LongD, BOOL fgwait)
{
    static INT32 i4IntCnt = 0;
    INT32 i4Ret;
    
    if (fgADSPIsIdle(u1DspId))
    {
        LOG(1, "Don't send interrupt if DSP is powered off\n");
        return;
    }

    if (fgwait)  //interrupt must be sent out, can't be called in ISR
    {
        VERIFY(x_sema_lock(_hSendDspIntSema[u1DspId], X_SEMA_OPTION_WAIT) == OSR_OK);

        while (fgDspBusy(u1DspId)) // wait until DSP not busy
        {
           x_thread_delay(1);
        }
                
        if (_rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspSInt)
        {
            i4IntCnt ++; 
            LOG(10, "[DSP_TASK] vSendDspLongInt Wait (%d), \n", i4IntCnt);
        }
        AUD_WRITE32(REG_LD(u1DspId), u4LongD);  // long data
        SendDSPInt(u1DspId, u4Addr, u4ShortD);  // short data & interrupt
        vLogDSPIntHistory(u1DspId, u4Addr,  u4ShortD,  u4LongD,  fgwait);
        VERIFY(x_sema_unlock(_hSendDspIntSema[u1DspId])== OSR_OK);
    }
    else  //some interrupts sent can't be wait (like dmx send pts)
    {
        i4Ret = x_sema_lock(_hSendDspIntSema[u1DspId], X_SEMA_OPTION_NOWAIT);

        if (i4Ret == OSR_OK)  //we only send interrupt to DSP when semaphore is locked OK  
        {
            if (fgDspBusy(u1DspId)) //although have sema but dsp busy..wow..left and release sema
            {
                LOG(1,"Driver miss to send DSP int cause of DSP busy !!\n");
                VERIFY(x_sema_unlock(_hSendDspIntSema[u1DspId])== OSR_OK);
                return; 
            } 

            i4IntCnt ++; 
            LOG(10, "[DSP_TASK] vSendDspLongInt No Wait(%d)\n", i4IntCnt);
            
            AUD_WRITE32(REG_LD(u1DspId), u4LongD);  // long data
            SendDSPInt(u1DspId, u4Addr, u4ShortD);  // short data & interrupt  
            vLogDSPIntHistory(u1DspId, u4Addr,  u4ShortD,  u4LongD, fgwait);
            VERIFY(x_sema_unlock(_hSendDspIntSema[u1DspId])== OSR_OK);
        }      
        else  //OSR_WOULD_BLOCK does't send interrrupt to DSP either to avoid DSP interrupt overwriten
        {
            LOG(1, "Driver miss to send DSP int cause of Sema protect!!!\n");
        }
    } 
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// vSetIecDownSample
//
/** Set IEC down sample.
 *
 *  @param  u4DownSample        Down sample value.
 */
//-----------------------------------------------------------------------------
void vSetIecDownSample(UINT32 u4DownSample)
{
    UINT32 u4IecConfig;

    u4IecConfig = AUD_READ32(REG_SPOCFG0);
    u4IecConfig &= (~SPOUT_DOWNSAMPLE_MASK);
    u4IecConfig |= (u4DownSample << SPOUT_DOWNSAMPLE_POS);
    AUD_WRITE32(REG_SPOCFG0, u4IecConfig);
}
#endif

//-----------------------------------------------------------------------------
// vSetIecConfigReg
//
/** Set audio configuration (only for first decoder).
 *
 *  @param  u1DecId      Decoder ID.
 *  @param  prAudCfg    Audio config structure.
 */
//-----------------------------------------------------------------------------
/*lint -e{818}*/
void vSetIecConfigReg(UINT8 u1DecId, AUDIO_CFG_T * prAudCfg)
{
    UINT32 u4IecConfigOrg, u4IecConfig;

    if (prAudCfg == NULL)
    {
        return;
    }

    u4IecConfigOrg = AUD_READ32(REG_SPOCFG0);
    u4IecConfig = u4IecConfigOrg & (~SPOUT_BASIC_MASK);
    u4IecConfig |= (((UINT32)prAudCfg->u1IecOutBitNum) << SPOUT_BIT_NUM_POS) |
        (((UINT32)prAudCfg->u1IecChSel) << SPOUT_CHSEL_POS) |
        (((UINT32)prAudCfg->u1IecDownSamp) << SPOUT_DOWNSAMPLE_POS) |
        (((UINT32)prAudCfg->u1IecDecSel) << SPOUT_SPDF_SEL_POS);

#ifndef CC_AUD_DDI
    if (prAudCfg->fgIecMute)
    {
        //MUTE
        vCloseIec();
    }
    else
    {
        //UnMUTE
        vOpenIec();
    }
#endif
    AUD_WRITE32(REG_SPOCFG0, u4IecConfig);
}

#if 0   // Unused
void vSetIecChannel(AUD_IEC_CH_T eIecChannel)
{
    UINT32 u4IecConfigOrg, u4IecConfig;

    u4IecConfigOrg = AUD_READ32(REG_SPOCFG0);
    u4IecConfig = (u4IecConfigOrg & ~(SPOUT_CHSEL_MASK | SPOUT_SPDF_SEL_MASK));
    if (eIecChannel == AUD_IEC_AUX)
    {
        u4IecConfig |= (SPOUT_SPDF_SEL_AUX<<SPOUT_SPDF_SEL_POS);
    }
    else
    {
        u4IecConfig |= ((SPOUT_SPDF_SEL_MAIN<<SPOUT_SPDF_SEL_POS) | (eIecChannel<<SPOUT_CHSEL_POS));
    }

    AUD_WRITE32(REG_SPOCFG0, u4IecConfig);
}
#endif

static CHAR* _aszIecCh[] = {"LR","SLR","CLFE","CH78","LINEIN", "CH910"};

void vIecQuery(void)
{
    UINT8 u1Index;
    UINT32 u4IecConfig = AUD_READ32(REG_SPOCFG0);

    Printf("[IEC Setting]\n");

    if (AUD_SPDIFGet() == 0)
    {
        Printf("IEC Mute (SPDIF0 pinmux=GPIO)\n");
    }
    else
    {
        Printf("IEC UnMute (SPDIF0 pinmux=SPDIF)\n");
    }

    if (u4IecConfig & (SPOUT_SPDF_SEL_AUX<<SPOUT_SPDF_SEL_POS))
    {
        Printf("IEC Ch: AUX\n");
    }
    else
    {
        u1Index = ((u4IecConfig&SPOUT_CHSEL_MASK)>>SPOUT_CHSEL_POS);
        if (u1Index < 6)
            Printf("IEC Ch: %s\n", _aszIecCh[u1Index]);
    }
}

//-----------------------------------------------------------------------------
// vFlushDspICache
//
/** Directly flush the instruction cache. If we need to take care the timing issue,
 *  we run it in the flush state machine. See DspState.c.
 *
 *  @param  fgWaitDsp   No use.
 */
//-----------------------------------------------------------------------------
void vFlushDspICache (UINT8 u1DspId, BOOL fgWaitDsp)
{
    UINT32 dwTmp;
    UINT32 dwDramStatus;
    UINT32 dwCounter = 0;

    if(u1DspId == AUD_DSP0)
    {
    dwTmp = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp & (~ADSP_CFG_ICACHE_FLUSH)));
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_ICACHE_FLUSH));
    do
    {
        dwDramStatus = AUD_READ32(REG_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH;
        dwCounter++;
    }
    while ( (dwDramStatus != 0) && (dwCounter < 5) );
    }
    else
    {
        dwTmp = AUD_READ32(REG_ADSP_CFG_DSPB);
        AUD_WRITE32(REG_ADSP_CFG_DSPB, (dwTmp & (~ADSP_CFG_ICACHE_FLUSH)));
        AUD_WRITE32(REG_ADSP_CFG_DSPB, (dwTmp | ADSP_CFG_ICACHE_FLUSH));
        do
        {
            dwDramStatus = AUD_READ32(REG_ADSP_CFG_DSPB) & ADSP_CFG_ICACHE_FLUSH;
            dwCounter++;
        }
        while ( (dwDramStatus != 0) && (dwCounter < 5) );
    }

    UNUSED(fgWaitDsp);
}

//-----------------------------------------------------------------------------
// vResetAndHoldDsp
//
/** Reset and hold DSP.
 */
//-----------------------------------------------------------------------------
void vResetAndHoldDSP (UINT8 u1DspId)
{
    UINT32 dwTmp;
    if(u1DspId == AUD_DSP0)
    {
        dwTmp = AUD_READ32(REG_ADSP_CFG);
        AUD_WRITE32(REG_ADSP_CFG, (dwTmp & (~ADSP_CFG_RST_B)));  // reset
    }
    else
    {
        dwTmp = AUD_READ32 (REG_ADSP_CFG);    
        AUD_WRITE32 (REG_ADSP_CFG, (dwTmp & (~ADSP_CFG_RST_B_DSPB)));  // reset        
    }
}

//-----------------------------------------------------------------------------
// HdStpDsp
//
/** Harrware stop DSP.
 */
//-----------------------------------------------------------------------------
void HdStpDsp (UINT8 u1DspId)
{
    vResetAndHoldDSP (u1DspId);
}

//-----------------------------------------------------------------------------
// vSetCacheSize
//
/** Set cache size.
 *
 *  @param  u1Size      Cache size value.
 */
//-----------------------------------------------------------------------------
void vSetCacheSize (UINT8 u1DspId, UINT8 bSize)
{
    UINT32 dwTmp;

    if(u1DspId == AUD_DSP0)
    {  
    dwTmp = AUD_READ32(REG_ADSP_CFG);
    dwTmp &= (~ADSP_CACHE_SIZE_MASK);
    dwTmp |= ((bSize << ADSP_CACHE_SIZE_POS) & ADSP_CACHE_SIZE_MASK);
    AUD_WRITE32(REG_ADSP_CFG, dwTmp);
}
    else
    {
        dwTmp = AUD_READ32 (REG_ADSP_CFG);
        dwTmp &= (~ADSP_CACHE_SIZE_MASK_DSPB);
        dwTmp |= ((bSize << ADSP_CACHE_SIZE_POS_DSPB) & ADSP_CACHE_SIZE_MASK_DSPB);
        AUD_WRITE32 (REG_ADSP_CFG, dwTmp);    
    }
}

//-----------------------------------------------------------------------------
// vSetCacheInstLen
//
/** Set cache buffer size.
 *
 *  @param  u1Size      Cache buffer size value.
 */
//-----------------------------------------------------------------------------
void vSetCacheInstLen (UINT8 u1DspId, UINT8 bSize)
{
    UINT32 dwTmp;
    if(u1DspId == AUD_DSP0)
    {  
    dwTmp = AUD_READ32(REG_ADSP_CFG);
    dwTmp &= (~ADSP_CACHE_INST_SIZE_MASK);
    dwTmp |= ((bSize << ADSP_CACHE_INST_SIZE_POS) & ADSP_CACHE_INST_SIZE_MASK);
    AUD_WRITE32(REG_ADSP_CFG, dwTmp);
}
    else
    {
        dwTmp = AUD_READ32 (REG_ADSP_CFG_DSPB);
        dwTmp &= (~ADSP_CACHE_INST_SIZE_MASK);
        dwTmp |= ((bSize << ADSP_CACHE_INST_SIZE_POS) & ADSP_CACHE_INST_SIZE_MASK);
        AUD_WRITE32 (REG_ADSP_CFG_DSPB, dwTmp);
    }
}

//-----------------------------------------------------------------------------
// vSetCmptRDThreshold
//
/** Set DSP compact read pre-fetch threshold.
 *
 *  @param  u1Threshold     Compact read threshold value.
 */
//-----------------------------------------------------------------------------
void vSetCmptRDThreshold(UINT8 u1DspId, UINT8 bThreshold)
{
    UINT32 dwTmp;
    if(u1DspId == AUD_DSP0)
    {  
    dwTmp = AUD_READ32(REG_ADSP_CFG);
    dwTmp &= (~ADSP_CMPTRD_THRES_MASK);
    dwTmp |= ((bThreshold << ADSP_CMPTRD_THRES_POS) & ADSP_CMPTRD_THRES_MASK);
    AUD_WRITE32(REG_ADSP_CFG, dwTmp);
}
    else
    {
        dwTmp = AUD_READ32 (REG_ADSP_CFG_DSPB);
        dwTmp &= (~ADSP_CMPTRD_THRES_MASK);
        dwTmp |= ((bThreshold << ADSP_CMPTRD_THRES_POS) & ADSP_CMPTRD_THRES_MASK);
        AUD_WRITE32 (REG_ADSP_CFG_DSPB, dwTmp);     
    }
}

void DownloadRomCode(UINT8 u1DspId)
{
    if (BSP_IsFPGA())
    {
        //Rice: load ROM code into FPGA SRAM
        UINT32 dwOfst;
        UINT32 dwSz;
        UINT32 dwFlashOfst;
        //UINT32 u4Datatmp[1024];

#if 1 //PROM
        dwFlashOfst = u4GetDspImgAddr (u1DspId);   // set to dsp image addr
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PROM_SA << 2));
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PROM_SZ << 2));

        if (dwSz >= 1)
        {
            UINT32 i;
            UINT32* puSrc= ((UINT32*) (u4GetDspImgAddr (u1DspId) + ((dwOfst)*4)));

            // Src: u4GetDspImgAddr () + (dwOfst * 4)
            // Size: (dwSz - 1) * 4 bytes
            WriteREG (RW_ADSPCR(u1DspId), (DR_RD_EN|PMEM_AUTO_INC|PROM_WR |HOT_DNLD));  // disable dsp
            WriteREG (RW_DNLD_CNTR(u1DspId), 0); // reset download counter to 0

            for (i=0; i<(dwSz-1); i++)
            {
                WriteREG (RW_PROM_DNLD(u1DspId), puSrc[i]);
            }

            // Verify
            WriteREG (RW_DNLD_CNTR(u1DspId), 0); // reset download counter to 0
            for (i= 0; i<(dwSz-1); i++)
            {
                //u4Datatmp[i] = ReadREG (RW_PROM_DNLD);
                UINT32 u4Data = ReadREG (RW_PROM_DNLD(u1DspId));
                if (u4Data != puSrc[i])
                {
                    break;
                }
            }

            WriteREG (RW_ADSPCR(u1DspId), 0);    // clear datarom write
#if 0 //For PROM test pattern
            //PROM Size-1
            if(dwSz>257)    dwSz=257;
            WriteDspDecCacheDram(0x1, (dwSz-1)-1);
            WriteDspDecCacheDram(0x3, 0x800500);

            // write to share memory
            for (i= 0; i<(dwSz-1); i++)
            {
                WriteDspDecCacheDram (0x5+i, puSrc[i]);
            }
#endif
        }
#endif
#if 0 //DROM
        dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr
        //dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SA << 2));
        //dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SZ << 2));
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DROM_SA << 2));
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DROM_SZ << 2));

        if (dwSz >= 1)
        {
            UINT32 i;
            UINT32* puSrc= ((UINT32*) (u4GetDspImgAddr () + ((dwOfst)*4)));

            // Src: u4GetDspImgAddr () + (dwOfst * 4)
            // Size: (dwSz - 1) * 4 bytes
            WriteREG (RW_ADSPCR, DROM_WR);  // disable dsp
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0

            for (i=0; i<(dwSz-1); i++)
            {
                WriteREG (RW_DROM_DNLD, puSrc[i]);
            }

            // Verify
            WriteREG (RW_DNLD_CNTR, 0); // reset download counter to 0
            for (i= 0; i<(dwSz-1); i++)
            {
                UINT32 u4Data = ReadREG (RW_DROM_DNLD);
                //u4Datatmp[i] = ReadREG (RW_DROM_DNLD);
                if (u4Data != puSrc[i])
                {
                    break;
                }
            }

            WriteREG (RW_ADSPCR, 0);    // clear datarom write
#if 0 //For DROM test pattern
            //DROM Size-1
            if(dwSz>257)    dwSz=257;
            WriteDspDecCacheDram(0x2, (dwSz-1)-1);
            WriteDspDecCacheDram(0x4, 0x800500);

            // write to share memory
            for (i= 0; i<(dwSz-1)*4; i+=3)
            {
                tmpW = (puSrc[i]<<16)|(puSrc[i+1]<<8)|(puSrc[i+2]);
                WriteDspDecCacheDram (0x105+(i/3), tmpW);    // clear datarom write
            }
#endif
        }
#endif
    }
}

void vDspDramAgentSet(void)
{
#if defined(CC_MT5399)
    //if (BSP_IsFPGA()) AUD_WRITE32(REG_TEST_CFG, 0x6000);  //change to 0x6000 by Shihhsin/Gallen for MT5399 U2 FPGA
    //else AUD_WRITE32(REG_TEST_CFG, 0x4000);               // for real MT5399 IC
    AUD_WRITE32(0x1000, 0xF000F000); 
    AUD_WRITE32(REG_TEST_CFG, 0x0000);
#elif defined(CC_MT5890) || defined(CC_MT5882) 
#ifdef CC_FPGA
    AUD_WRITE32(0x1000, 0xF000F000);                    //6000[29] : agent 15 enable
                                                                           //6000[28] : agent 14_1 enable
#endif
    //6000[29] : agent 15 enable, 6000[28] : agent 14_1 enable
    AUD_WRITE32(0x1000, AUD_READ32(0x1000) | (0x3 << 28)); 
    AUD_WRITE32(REG_TEST_CFG, 0x0000);               //5250[14] 1: DSPB merge with DSPA
                                                                          //             0 : DSPB not merge with DSPA(not that if not merge, DSPB is using agnet 14_1)
                                                                          //5250[13] 1: DSPA select aent 20
                                                                          //             0: DSPA select agent 15
#else
    //AUD_WRITE32(REG_TEST_CFG, 0x2000);  //change to DRAM agent group 2
#endif
    
}

//-----------------------------------------------------------------------------
// vTriggerDsp
//
/** Let DSP go.
 */
//-----------------------------------------------------------------------------
void vTriggerDSP(UINT8 u1DspId)
{
  UINT32 dwTmp;
  UINT32 dwDramStatus;
  UINT32 dwRetryCnt=0;

#ifndef CC_AUD_ARM_SUPPORT //With ARM case, set it in vAprocMain_Init 
    vDspDramAgentSet();
#endif

    if (u1DspId == AUD_DSP0)
    {
  dwTmp = AUD_READ32(REG_ADSP_CFG);
  if ((dwTmp & ADSP_CFG_RST_B) == ADSP_CFG_RST_B)
  {
      x_thread_delay(300);
  }
  while(1)
  {
    #if 1
    AUD_WRITE32(REG_ADSP_CFG, ((dwTmp & (~ADSP_CFG_ICACHE_FLUSH)) & (~ADSP_CFG_RST_B)));
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_ICACHE_FLUSH));

    // Light: Remove delay
    //x_thread_delay(10);                                      // delay 10 ms
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_RST_B));
    #else
    AUD_WRITE32(REG_ADSP_CFG, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
    AUD_WRITE32(REG_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
    AUD_WRITE32(REG_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
    #endif

    // wait cycles to flush
    x_thread_delay(20); // Light: delay 20 ms
    // check if flush taken
    dwDramStatus = AUD_READ32(REG_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH ;
    if( (dwDramStatus == 0) || (dwRetryCnt > 20) )
    {
      if(dwDramStatus != 0)
      {
        //_dwDspFlushFailCnt ++;
        LOG(1, "DSP0 flush cache fail\n");
      }
      break;
    }
    dwRetryCnt ++;
  }

  // Light add LOG of Icache flush retry count
  if(dwRetryCnt)
  {
      LOG(9,"DSP0 ICache flush retry count = %d\n",dwRetryCnt);
  }
#if 0
  dwTmp = AUD_READ32(REG_ADSP_CFG);
  AUD_WRITE32(REG_ADSP_CFG, (dwTmp & (~ADSP_CFG_RST_B)));  // Reset again
  dwTmp |= ADSP_CFG_RST_B;  // enable dsp
  AUD_WRITE32(REG_ADSP_CFG, dwTmp);  // enable dsp
#endif

    } //end AUD_DSP0
    else
     {
      dwTmp = AUD_READ32 (REG_ADSP_CFG);
      if ((dwTmp & ADSP_CFG_RST_B_DSPB) == ADSP_CFG_RST_B_DSPB)
      {
          x_thread_delay(300); 
      }      
      dwTmp = AUD_READ32 (REG_ADSP_CFG_DSPB);
      while(1)
      {
        #if 1
        //WriteREG (REG_ADSP_CFG_DSPB, ((dwTmp & (~ADSP_CFG_ICACHE_FLUSH)) & (~ADSP_CFG_RST_B)));
        AUD_WRITE32 (REG_ADSP_CFG_DSPB, (dwTmp & (~ADSP_CFG_ICACHE_FLUSH)));
        AUD_WRITE32 (REG_ADSP_CFG, ((AUD_READ32 (REG_ADSP_CFG)) & (~ADSP_CFG_RST_B_DSPB)));
        AUD_WRITE32 (REG_ADSP_CFG_DSPB, (dwTmp | ADSP_CFG_ICACHE_FLUSH));

        // Light: Remove delay
        //x_thread_delay(10);                                      // delay 10 ms
        //WriteREG (REG_ADSP_CFG_DSPB, (dwTmp | ADSP_CFG_RST_B));
        AUD_WRITE32 (REG_ADSP_CFG, (AUD_READ32 (REG_ADSP_CFG) | ADSP_CFG_RST_B_DSPB));
        #else
        WriteREG (REG_ADSP_CFG_DSPB, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
        WriteREG (REG_ADSP_CFG_DSPB, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
        WriteREG (REG_ADSP_CFG_DSPB, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);    
        #endif
        
        // wait cycles to flush
        x_thread_delay(20); // Light: delay 20 ms
        // check if flush taken
        dwDramStatus =   AUD_READ32(REG_ADSP_CFG_DSPB) & ADSP_CFG_ICACHE_FLUSH ;
        if( (dwDramStatus == 0) || (dwRetryCnt > 20) )
        {
          if(dwDramStatus != 0)
          {
            //_dwDspFlushFailCnt ++;
            LOG(1, "DSP1 flush cache fail\n");
          }
          break;
        }
        dwRetryCnt ++;
      }

      // Light add LOG of Icache flush retry count
      if(dwRetryCnt)
      {
          LOG(9,"DSP1 ICache flush retry count = %d\n",dwRetryCnt);
      }
     }

}

//-----------------------------------------------------------------------------
// vSetDspBootFromRam
//
/** Set boot from RAM flag. It is needed to be set when DSP is in reset and hold status.
 */
//-----------------------------------------------------------------------------
void vSetDspBootFromRam (UINT8 u1DspId)
{
    UINT32 u4Data;

    if(u1DspId == AUD_DSP0)
    {
    u4Data = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, (u4Data | ADSP_BOOT_FROM_RAM));
}
    else
    {
        u4Data = AUD_READ32 (REG_ADSP_CFG);
        AUD_WRITE32 (REG_ADSP_CFG, (u4Data | ADSP_BOOT_FROM_RAM_DSPB));
    }
}

#ifdef CC_AUD_FPGA
//-----------------------------------------------------------------------------
// vSetDspBootFromRom
//
/** Set boot from ROM flag. It is needed to be set when DSP is in reset and hold status.
 */
//-----------------------------------------------------------------------------
void vSetDspBootFromRom (UINT8 u1DspId)
{
    UINT32 u4Data;

    if(u1DspId == AUD_DSP0)
    {
    u4Data = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, (u4Data & (~ADSP_BOOT_FROM_RAM)));
}
    else
    {
        u4Data = AUD_READ32(REG_ADSP_CFG);
        AUD_WRITE32(REG_ADSP_CFG, (u4Data & (~ADSP_BOOT_FROM_RAM_DSPB)));
    }    
}
#endif

//-----------------------------------------------------------------------------
// HdRstDsp
//
/** Reset DSP hardware. Set cache size, flush cache, load rom code if needed.
 */
//-----------------------------------------------------------------------------
void HdRstDsp (UINT8 u1DspId)
{
    if(u1DspId == AUD_DSP0)
{
    // Disable DRAM timeout reset function
    AUD_WRITE32(REG_ADSP_CFG, ((AUD_READ32(REG_ADSP_CFG)) & (~(ADSP_DRAM_TIMEOUT_RESET | ADSP_DRAM_TIMEOUT_FLUSH))));

    // Enable DSP clock
    AUD_WRITE32(REG_ADSP_CFG, ((AUD_READ32(REG_ADSP_CFG)) | ADSP_CFG_CLK_ENA));
    }
    else
    {
        // Disable DRAM timeout reset function
        AUD_WRITE32(REG_ADSP_CFG_DSPB, ((AUD_READ32(REG_ADSP_CFG_DSPB)) & (~ADSP_DRAM_TIMEOUT_RESET))); 

        // Enable DSP clock
        AUD_WRITE32(REG_ADSP_CFG_DSPB, ((AUD_READ32(REG_ADSP_CFG_DSPB)) | ADSP_CFG_CLK_ENA)); 
    }

    //Reset Dsp
    // First reset and hold dsp
    vResetAndHoldDSP (u1DspId);
    // Set cache size
    //vSetCacheSize (CACHE_256);
    vSetCacheSize (u1DspId, CACHE_SIZE_2K);
    // set instruction buffer size
    vSetCacheInstLen (u1DspId, CBUF_SIZE_16X128);
    // set compact read threshold
    vSetCmptRDThreshold(u1DspId, THRES_UNFULL);

    // Download Rom Code for FPGA
    DownloadRomCode(u1DspId);

#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882) 
    _fgRamBoot[AUD_DSP0] = FALSE;
    _fgRamBoot[AUD_DSP1] = FALSE;
#elif defined(CC_MT5398)
    if(BSP_GetIcVersion() == IC_VER_5398_AA) // RamBoot
    {
        _fgRamBoot = TRUE;
    }
    else // IC_VER_5398_AB, RomBoot
    {
        _fgRamBoot = FALSE;
    }
#else
    _fgRamBoot = TRUE;
#endif

#ifdef DSP_TEST_CODE_FETCH
    vDspLoadRamCode (AUD_ROMCOD);
    vDspLoadRamCode (AUD_MPEG_DEC3);
    vDspLoadRamCode (AUD_MPEG_DEC1);
    vDspLoadRamCode (AUD_MPEG_DEC2);
    vDspLoadRamCode (AUD_SBCENC_DEC4);
    vDspLoadRamCode (AUD_POSTRAM);
#endif

    // Rice: changed and modified _fgRamBoot to FALSE @20060731
    /*lint -e{506}*/
    /*lint -e{774}*/

    #ifdef DSP_SUPPORT_17BIT
    // Load Rom Code
    vDspLoadRamCode (u1DspId, AUD_ROMCOD);
    #endif

    if (_fgRamBoot[u1DspId])
    {
        #ifndef DSP_SUPPORT_17BIT
        // Load Rom Code
        vDspLoadRamCode (u1DspId, AUD_ROMCOD);
        #endif

        // Enable Ram Boot
        vSetDspBootFromRam (u1DspId);
    }

    vDspLoadRamCode (u1DspId, AUD_AININTRAM);

#ifdef DSP_TEST_CORDIC
    {
        UINT32 i;
        UINT32 u4Size;
        UINT32 *Dst;

        u4Size = sizeof(u4CordicTest)/sizeof(UINT32);
        Dst = _u4DspDramBuf[u1DspId][TYPE_DEC1_DRAM_IDX] + (0x400*4);
        for (i=0; i<u4Size; i++)
        {
            *Dst++ = u4CordicTest[i];
        }
        DSP_FlushInvalidateDCache(u1DspId, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);
    }
#endif
    // Let dsp go
    vTriggerDSP (u1DspId);

    LOG(0, "HdRstDsp....u1DspId=%d\n", u1DspId);
    // ramboot
    // Flush cache
    //vFlushDspICache (FALSE);

}

void vResetDSP (UINT8 u1DspId)
{
    UINT32 dwTmp;
    if(u1DspId == AUD_DSP0)
    {
    dwTmp = AUD_READ32(REG_ADSP_CFG);

    AUD_WRITE32(REG_ADSP_CFG, ((dwTmp & (~ADSP_CFG_ICACHE_FLUSH)) & (~ADSP_CFG_RST_B)));
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_ICACHE_FLUSH));
    // Light: Remove delay
    //x_thread_delay(10);                                      // delay 10 ms
    AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_RST_B));
}
    else
    {
        AUD_WRITE32(REG_ADSP_CFG, (AUD_READ32(REG_ADSP_CFG) & (~ADSP_CFG_RST_B_DSPB)));   

        dwTmp = AUD_READ32(REG_ADSP_CFG_DSPB);
        AUD_WRITE32(REG_ADSP_CFG_DSPB, ((dwTmp & (~ADSP_CFG_ICACHE_FLUSH)) ));
        AUD_WRITE32(REG_ADSP_CFG_DSPB, (dwTmp | ADSP_CFG_ICACHE_FLUSH));

        AUD_WRITE32(REG_ADSP_CFG, (AUD_READ32(REG_ADSP_CFG) | ADSP_CFG_RST_B_DSPB));        
    }
}

//-----------------------------------------------------------------------------
// dwGetDSPNSADR
//
/** Get DSP normal DRAM start block address.
 *
 *  @param  u4Type      Type.
 *
 *  @retval  Normal DRAM buffer block address.
 */
//-----------------------------------------------------------------------------
UINT32 dwGetDSPNSADR (UINT8 u1DspId, UINT32 u4Type)
{
    UINT32 u4Addr;

    u4Addr = (_u4DspDramBuf[u1DspId][TYPE_DEC1_DRAM_IDX]);
    if (u4Type < TYPE_AUD_DEC2)
    {
        u4Addr = (_u4DspDramBuf[u1DspId][TYPE_DEC1_DRAM_IDX]);
    }
    else if (( (u4Type >= TYPE_AUD_DEC2) && (u4Type < TYPE_AUD_MIX) ))
    {
        u4Addr = _u4DspDramBuf[u1DspId][TYPE_DEC2_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC3) && (u4Type < TYPE_AUD_DEC4) ))
    {
        u4Addr = _u4DspDramBuf[u1DspId][TYPE_DEC3_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC4) && (u4Type < TYPE_AUD_CMM) ))
    {
        u4Addr = _u4DspDramBuf[u1DspId][TYPE_DEC4_DRAM_IDX];
    }
    else
    {
        u4Addr = _u4DspDramBuf[u1DspId][TYPE_COMM_DRAM_IDX];
    }
    return u4Addr;
}

//-----------------------------------------------------------------------------
// dwGetDSPCSADR
//
/** Get DSP compact DRAM start block address.
 *
 *  @param  u4Type      Type.
 *
 *  @retval  Compact DRAM buffer block address.
 */
//-----------------------------------------------------------------------------
UINT32 dwGetDSPCSADR (UINT8 u1DspId, UINT32 u4Type)
{
    UINT32 u4Addr;

    u4Addr = _u4DspCmptBuf[u1DspId][TYPE_DEC1_DRAM_IDX];
    if (u4Type < TYPE_AUD_DEC2)
    {
        u4Addr = _u4DspCmptBuf[u1DspId][TYPE_DEC1_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC2) && (u4Type < TYPE_AUD_MIX) ))
    {
        u4Addr = _u4DspCmptBuf[u1DspId][TYPE_DEC2_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_MIX) && (u4Type < TYPE_AUD_DEC4) ))
    {
        u4Addr = _u4DspCmptBuf[u1DspId][TYPE_DEC3_DRAM_IDX];
    }
    else if (( (u4Type >= TYPE_AUD_DEC4) && (u4Type < TYPE_AUD_CMM) ))
    {
        u4Addr = _u4DspCmptBuf[u1DspId][TYPE_DEC4_DRAM_IDX];
    }
    else
    {
        u4Addr = _u4DspCmptBuf[u1DspId][TYPE_COMM_DRAM_IDX];
    }
    return u4Addr;
}

//-----------------------------------------------------------------------------
// dwGetDspIcacheAdr
//
/** Get DSP instruction cache address.
 *
 *  @param  uRamCodeType        RAM code type.
 *
 *  @retval  DSP instruction cache address.
 */
//-----------------------------------------------------------------------------
UINT32 dwGetDspIcacheAdr (UINT8 u1DspId, UINT16 u2RamCodeType)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    return (((UINT32) _u4DspICacheAddr[u1DspId][u2RamCodeType]) << 6);
}

//-----------------------------------------------------------------------------
// vSetGetBitsNotHold
//
/** Set get bits hold or not hold when bitstream buffer is empty.
 *  It is depend on decoder setting, for ex. vorbis is set to not hold, default is hold.
 *  It is configured in LoadDramTable.
 *
 *  @param  fgGetBitsNotHold    TRUE(not hold) / FALSE(hold).
 */
//-----------------------------------------------------------------------------
void vSetGetBitsNotHold(BOOL fgGetBitsNotHold)
{
    UINT32 u4Data;

    u4Data = AUD_READ32(REG_BS_CFG(AUD_DSP0));
    u4Data = u4Data & (~NO_HOLD_GBS);
    if (fgGetBitsNotHold)
    {
        u4Data |= NO_HOLD_GBS;
    }
    AUD_WRITE32(REG_BS_CFG(AUD_DSP0), u4Data);

    u4Data = AUD_READ32(REG_BS_CFG(AUD_DSP1));
    u4Data = u4Data & (~NO_HOLD_GBS);
    if (fgGetBitsNotHold)
    {
        u4Data |= NO_HOLD_GBS;
    }
    AUD_WRITE32(REG_BS_CFG(AUD_DSP1), u4Data);    
}

//-----------------------------------------------------------------------------
// vSetSPDIFInFIFO
//
/** Set SPDIF/Line-in FIFO.
 *
 *  @param  u4StartAdr    FIFO start address.
 *  @param  u4EndAdr      FIFO end address.
 */
//-----------------------------------------------------------------------------
void vSetSPDIFInFIFO(UINT8 u1LineInId, UINT32 u4StartAdr,UINT32 u4EndAdr)
{
    UINT32 u4SpLinBlk=0;
    UINT32 u4RegLinBlk[3] = {REG_LINBLK, REG_DEC2_LINBLK, REG_DEC3_LINBLK};
    u4SpLinBlk = (u4StartAdr << 8) | ((u4EndAdr >> 8) & 0xFFFF);

    AUD_WRITE32(u4RegLinBlk[u1LineInId], u4SpLinBlk); 
}

//-----------------------------------------------------------------------------
// u4DspRegR_D2rSda
//
/** Read DSP REG - D2R_SDA.
 *
 *  @retval     D2R_SDA data.
 */
//-----------------------------------------------------------------------------
UINT32 u4DspRegR_D2rSda(UINT8 u1DspId)
{
    return (AUD_READ32(REG_D2R_SDA(u1DspId)));
}

//-----------------------------------------------------------------------------
// vADCInit
//
/** Initialize TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCInit(void)
{
    UINT32 u4Data;
    UINT32 u4AudioTunerATD = 0;

    // Set AUDIN_CFG
    u4Data = (AUD_READ32(REG_AUDIN_CFG) & (~REG_AUDIN_CFG_MASK));
    u4Data = u4Data | APGA_GAIN_0DB | APGA_OFFSET_0V | APGA_GAIN_BY_DSP | ADCK_INV_RISING;    // | ADC_SEL_DEMOD;  default set sif ADC
    if (DRVCUST_OptQuery(eAudioTunerATD,&u4AudioTunerATD) == 0)
    {
        if ((BOOL)u4AudioTunerATD == TRUE)
        {
            u4Data = u4Data | ADC_SEL_DEMOD;
        }
    }
    AUD_WRITE32(REG_AUDIN_CFG, u4Data);

    #ifdef CC_AUD_SUPPORT_SIF_ADC
    ANA_WRITE32(REG_CVBS_CFG4, (AUD_READ32(REG_CVBS_CFG4)&0x00ffff7f) | 0x40000080);
    //[31:24] RG_AISEL_2    => 8'b01000000
    //[7] RG_C_SELA_2       => 1
    ANA_SET_BIT(REG_CVBS_CFG6, 0x20000000);
    //[29] RG_SIFA_EN_2     => 1
    ANA_CLR_BIT(REG_CVBS_CFG7, 0x00a0b880);
    //[23] RG_CVBSADC_SEL_CKPLL_2 => 0
    //[21] RG_CVBSADC_PWD_2 => 0
    //[15] RG_CVBS_PWD_2 => 0
    //[13] RG_INMUX_PWD_2 => 0
    //[12] RG_PROT_PWD_2 => 0
    //[11] RG_PGABUFNA_PWD_2 => 0
    //[9] RG_SHIFTA_PWD_2 => 1
    //[7] RG_OFFCURA_PWD_2 => 0

    u4Data = (AUD_READ32(REG_AUDIN_CFG) & (~ADC_SEL_DEMOD));
    AUD_WRITE32(REG_AUDIN_CFG, u4Data);

    #endif
}

#if 0
//-----------------------------------------------------------------------------
// vADCPowerOn
//
/** Power on TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCPowerOn(void)
{
#if defined(CC_MT5365) || defined(CC_MT5395)
    // Power on APGA & ADC
    UINT32 u4Data;

    u4Data = AUD_READ32(REG_ATVCFG0);
    u4Data = (u4Data & (~RG_PGA_SIF_PWD_MASK)) | RG_PGA_SIF_PWD_ON;
    AUD_WRITE32(REG_ATVCFG0, u4Data);
    u4Data = AUD_READ32(REG_ATVCFG1);
    u4Data = (u4Data & (~RG_SIF_ADC_PWD_MASK) & (~RG_BIAS_SIF_PWD_MASK))
             | RG_SIF_ADC_PWD_ON| RG_BIAS_SIF_PWD_ON;
    AUD_WRITE32(REG_ATVCFG1, u4Data);
#endif
}

//-----------------------------------------------------------------------------
// vADCPowerDown
//
/** Power down TV audio ADC.
 */
//-----------------------------------------------------------------------------
void vADCPowerDown(void)
{
#if defined(CC_MT5365) || defined(CC_MT5395)
     UINT32 u4Data;

    // Power down APGA & ADC
    u4Data = AUD_READ32(REG_ATVCFG0);
    u4Data = (u4Data & (~RG_PGA_SIF_PWD_MASK)) | RG_PGA_SIF_PWD_DOWN;
    AUD_WRITE32(REG_ATVCFG0, u4Data);

    u4Data = AUD_READ32(REG_ATVCFG1);
    u4Data = (u4Data & (~RG_SIF_ADC_PWD_MASK) & (~RG_BIAS_SIF_PWD_MASK))
             | RG_SIF_ADC_PWD_DOWN | RG_BIAS_SIF_PWD_DOWN;
    AUD_WRITE32(REG_ATVCFG1, u4Data);
#endif
}
#endif

//-----------------------------------------------------------------------------
// DSP_SetAIFMux
//
/** Set TV audio ADC for AF or SIF path (NTSC).
 *
 *  @param  uAIFPath    0: SIF path,1: AF path.
 */
//-----------------------------------------------------------------------------
void DSP_SetAIFMux(UINT8 uAIFPath)
{
#if 1
    UNUSED(uAIFPath);
#else
    UINT32 u4Data;

    if (uAIFPath==AF_PATH)
    {
        // Set AF path, bias current isn't set (default)
        u4Data = AUD_READ32(REG_ATVCFG0);
        u4Data = (u4Data & (~RG_SIF_SEL_MASK)) | SIF_SEL_AF;
        AUD_WRITE32(REG_ATVCFG0, u4Data);
    }
    else // SIF path
    {
        u4Data = AUD_READ32(REG_ATVCFG0);
        u4Data = (u4Data & (~RG_SIF_SEL_MASK)) | SIF_SEL_SIF;
        AUD_WRITE32(REG_ATVCFG0, u4Data);
    }
#endif
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// DSP_SPDIFOutSetSrc
//
/** Set the input PCM data of SPDIF-out from dec1 or dec2.
 *  Also change the AOUT CLK to the sampling rate of specified decoder.
 *
 *  @param  u1DecId     Decoder ID.
 */
//-----------------------------------------------------------------------------
void DSP_SPDIFOutSetSrc(UINT8 u1DecId)
{
    UINT32 u4Tmp;
    UINT32 u4Set = (UINT32)u1DecId;

    u4Tmp = AUD_READ32(REG_SPOCFG0);
    u4Tmp &= ~(SPOUT_SPDF_SEL_MASK);
    u4Tmp |= (u4Set << SPOUT_SPDF_SEL_POS);
    AUD_WRITE32(REG_SPOCFG0, u4Tmp);
}
#endif

//-----------------------------------------------------------------------------
// DSP_ClearInt
//
/** Clear DSP interrupt.
 */
//-----------------------------------------------------------------------------
void DSP_ClearInt(UINT8 u1DspId)
{
    AUD_WRITE32(REG_D2R_SDA(u1DspId), 0x0);
}

//-----------------------------------------------------------------------------
// DSP_SetAdcTuneCfg
//
/** Set ADC tuner configuration.
 */
//-----------------------------------------------------------------------------
void DSP_SetAdcTuneCfg(void)
{
    UINT32 u4DspRIntData;
    UINT32 u4TempData;

    DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    u4DspRIntData = dReadDspCommDram(AUD_DSP0, ADDR_D2RC_ADC_TUNE_CFG);

    // Set ADC gain
    u4TempData = AUD_READ32(REG_AUDIN_CFG);
    u4TempData = (u4TempData & (~APGA_GAIN_MASK)) |
        (((u4DspRIntData & DRAM_ADC_GAIN_MASK) >> DRAM_ADC_GAIN_POS) << APGA_GAIN_POS);
    AUD_WRITE32(REG_AUDIN_CFG, u4TempData);

    // Set Offset
    u4TempData = AUD_READ32(REG_AUDIN_CFG);
    u4TempData = (u4TempData & (~APGA_OFFSET_MASK)) |
        (((u4DspRIntData & DRAM_ADC_OFFSET_MASK) >> DRAM_ADC_OFFSET_POS) << APGA_OFFSET_POS);
    AUD_WRITE32(REG_AUDIN_CFG, u4TempData);
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// vTriggerDSPSLT
//
/** SLT DSP Trigger.
 */
//-----------------------------------------------------------------------------
void vTriggerDSPSLT(void)
{
    UINT32 dwTmp;
    UINT32 dwDramStatus;
    UINT32 dwRetryCnt = 0;

    dwTmp = AUD_READ32(REG_ADSP_CFG);

    while(1)
    {
        #if 1
        AUD_WRITE32(REG_ADSP_CFG, ((dwTmp & (~ADSP_CFG_ICACHE_FLUSH)) & (~ADSP_CFG_RST_B)));
        AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_ICACHE_FLUSH));
        //x_thread_delay(10);                                      // delay 10 ms
        AUD_WRITE32(REG_ADSP_CFG, (dwTmp | ADSP_CFG_RST_B));
        #else
        AUD_WRITE32(REG_ADSP_CFG, dwTmp & (~ADSP_CFG_ICACHE_FLUSH) & (~ADSP_CFG_RST_B));
        AUD_WRITE32(REG_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
        AUD_WRITE32(REG_ADSP_CFG, dwTmp | ADSP_CFG_ICACHE_FLUSH | ADSP_CFG_RST_B);
        #endif
        // wait cycles to flush
        //x_thread_delay(10);                                      // delay 10 ms
        // check if flush taken
        dwDramStatus = AUD_READ32(REG_ADSP_CFG) & ADSP_CFG_ICACHE_FLUSH;
        if ( (dwDramStatus == 0) || (dwRetryCnt > 20) )
        {
            if (dwDramStatus != 0)
            {
                //_dwDspFlushFailCnt ++;
                Printf("DSP flush cache fail\n");
            }
            break;
        }
        dwRetryCnt ++;
    }
    // Light add LOG of Icache flush retry count
    if (dwRetryCnt)
    {
        Printf("ADSP ICache flush retry count = %d\n",dwRetryCnt);
    }
    #if 0
    AUD_WRITE32(REG_ADSP_CFG, dwTmp & ~ADSP_CFG_RST);  // Reset again
    dwTmp |= ADSP_CFG_RST;                         // enable dsp
    x_thread_delay(20);                              // delay 10 ms
    AUD_WRITE32(REG_ADSP_CFG, dwTmp);                    // enable dsp
    #endif
}
#endif

//-----------------------------------------------------------------------------
// DSP_ResetDsp
//
/** Reset DSP.
 */
//-----------------------------------------------------------------------------
void DSP_ResetDsp(UINT8 u1DspId)
{
    if(u1DspId == AUD_DSP0)
{
    AUD_WRITE32(REG_ADSP_CFG, (AUD_READ32(REG_ADSP_CFG) & (~ADSP_CFG_RST_B)));
    AUD_WRITE32(REG_ADSP_CFG, (AUD_READ32(REG_ADSP_CFG) | ADSP_CFG_RST_B));
}
    else
    {
        WriteREG(REG_ADSP_CFG, (ReadREG(REG_ADSP_CFG) & (~ADSP_CFG_RST_B_DSPB)));
        WriteREG(REG_ADSP_CFG, (ReadREG(REG_ADSP_CFG) | ADSP_CFG_RST_B_DSPB));
    }
}

//-----------------------------------------------------------------------------
// DSP_SetAudDramBlock
//
/** Setup Audio DRAM block.
 *
 *  @retval     Sucess or failed.
 */
//-----------------------------------------------------------------------------
CHAR DSP_SetAudDramBlock(UINT8 u1DspId, UINT8 u1MemBlock)
{
    UNUSED(u1DspId);
    if (u1MemBlock > 127)
    {
        return 1;
    }
    AUD_WRITE32(REG_ADSP_CFG, (AUD_READ32(REG_ADSP_CFG) & (~ADSP_AUDIO_BLOCK_MASK)) | (((UINT32)u1MemBlock)<<ADSP_AUDIO_BLOCK_POS));

    return 0;
}

//-----------------------------------------------------------------------------
// IecInfoInit
//
/** Iec power on init (for both decoders).
 */
//-----------------------------------------------------------------------------
void IecInfoInit(void)
{
    UINT32 dwDspData   = 0;
    UINT16 wPcmChan    = 0;          // Default is LR output
    UINT16 wIecMute    = 0;
    BOOL fgIecMute     = FALSE;
    BOOL fgIecFlag     = FALSE;
    UINT32 dwOtherCfg;

    dwOtherCfg = AUD_READ32(REG_SPOCFG0) & 0x000000FF; // Remaining not iec config

    /* for first decoder */
    fgIecFlag = uReadDspShmBYTE (AUD_DSP0, B_IECFLAG)  & 0x01;
    fgIecMute = uReadDspShmBYTE (AUD_DSP0, B_IEC_MUTE) & 0x01;    // IEC signal on/off state
    _rDspStatus[AUD_DSP0][AUD_DEC_MAIN].fgIecMuteState = FALSE;    // IEC mute on DSP side

    // get IEC output selection
    wPcmChan = ((UINT32) uReadDspShmBYTE (AUD_DSP0, B_IEC_PCMCH)) << SPOUT_CHSEL_POS; // bit[4:2]
    // update IEC info
    dwDspData = ((wPcmChan | dwOtherCfg) & (~(1<<SPOUT_MUTE_POS))) | wIecMute;

    // update REG_SPOUT_CFG
    AUD_WRITE32(REG_SPOCFG0, (UINT32) (dwDspData));

#ifndef CC_AUD_DDI
    if (fgIecFlag && fgIecMute)
    {
        //MUTE
        vCloseIec();
    }
    else
    {
        //UnMUTE
        vOpenIec();
    }
#endif
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// _ADSP_CFG_Reset
//
/** Reset ADSP Cfg.
 */
//-----------------------------------------------------------------------------
void _ADSP_CFG_Reset(UINT8 u1DspId)
{
    AUD_WRITE32(REG_ADSP_CFG, ((AUD_READ32(REG_ADSP_CFG)) | ADSP_CFG_CLK_ENA));    // Enable DSP clock
    vResetAndHoldDSP(u1DspId);                         // Reset and hold DSP
    vSetCacheSize(u1DspId, CACHE_SIZE_2K);               // Set cache size
    vSetCacheInstLen(u1DspId, CBUF_SIZE_16X128);         // Set instruction buffer size
    vSetCmptRDThreshold(u1DspId, THRES_UNFULL);          // Set compact read threshold
}
#endif

#ifdef CC_AUD_FPGA
void vSetDspRomCode(UINT8 u1DspId, UINT32* puSrc,UINT32 dwSz)
{
    UINT32 i;

    // Src: u4GetDspImgAddr () + (dwOfst * 4)
    // Size: (dwSz - 1) * 4 bytes
    WriteREG (RW_ADSPCR(u1DspId), (DR_RD_EN|PMEM_AUTO_INC|PROM_WR |HOT_DNLD));  // disable dsp
    WriteREG (RW_DNLD_CNTR(u1DspId), 0); // reset download counter to 0

    for (i=0; i<dwSz; i++)
    {
        WriteREG (RW_PROM_DNLD(u1DspId), puSrc[i]);
    }

    // Verify
    WriteREG (RW_DNLD_CNTR(u1DspId), 0); // reset download counter to 0
    for (i= 0; i<dwSz; i++)
    {
        //u4Datatmp[i] = ReadREG (RW_PROM_DNLD);
        UINT32 u4Data = ReadREG (RW_PROM_DNLD(u1DspId));
        if (u4Data != puSrc[i])
        {
            Printf("!!! Rom code download fail !!!\n");
            break;
        }
    }

    WriteREG (RW_ADSPCR(u1DspId), 0);    // clear datarom write
}
#endif

