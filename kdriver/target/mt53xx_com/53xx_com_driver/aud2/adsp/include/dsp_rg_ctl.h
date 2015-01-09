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
 * $RCSfile: dsp_rg_ctl.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_rg_ctl.h
 *  Brief of file dsp_rg_ctl.h.
 *  Details of file dsp_rg_ctl.h (optional).
 */

#ifdef AUD_EXTERN_FILE
#error "Please don't include this header file in release C file"
#endif

#ifndef DSP_RG_CTL_H
#define DSP_RG_CTL_H

#include "x_typedef.h"
#include "dsp_common.h"
#include "adsp_task.h"
 
#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */
#ifndef DSP_RGCTL_C
#define DSP_RGCTL_C
/* Function Declaration */
    extern void IniBlkRegs (UINT8 u1DspId);
    extern void HdStpDsp (UINT8 u1DspId);
    extern void HdRstDsp (UINT8 u1DspId);
    extern void vFlushDspICache (UINT8 u1DspId, BOOL fgWaitDsp);
    extern UINT32 dwGetDSPNSADR (UINT8 u1DspId, UINT32 u4Type);
    extern UINT32 dwGetDSPCSADR (UINT8 u1DspId, UINT32 u4Type);
    extern UINT32 dwGetDspIcacheAdr (UINT8 u1DspId, UINT16 u2RamCodeType);
    extern BOOL fgDspBusy (UINT8 u1DspId);
    extern void vDspSendInt (UINT8 u1DspId);
    extern void vCloseIec (void);
    extern void vSetIecConfigReg(UINT8 u1DecId, AUDIO_CFG_T * prAudCfg);
    extern void vSetIecChannel(AUD_IEC_CH_T eIecChannel);
    extern void vSendDspLongInt(UINT8 u1DspId, UINT32 u4Addr, UINT32 u4ShortD, UINT32 u4LongD, BOOL fgWait);
    extern UINT32 dwReadDspLongData (UINT8 u1DspId);
    extern void vSetGetBitsNotHold(BOOL fgGetBitsHold);
    extern void vOpenIec (void);
    extern void vSetIecDownSample (UINT32 u4DownSample);
    extern void vSetCacheSize (UINT8 u1DspId, UINT8 bSize);
    extern void vSetCacheInstLen (UINT8 u1DspId, UINT8 bSize);
    extern void vSetCmptRDThreshold(UINT8 u1DspId, UINT8 bThreshold);

    // Read REG
    extern UINT32 u4DspRegR_D2rSda(UINT8 u1DspId);

    // Function
    // ADC function
    extern void vADCInit(void);
    //extern void vADCPowerOn(void);
    //extern void vADCPowerDown(void);

    extern void DSP_SetAIFMux(UINT8 uAIFPath);
    extern void DSP_SPDIFOutSetSrc(UINT8 u1DecId);
    extern void DSP_ClearInt(UINT8 u1DspId);

    extern void DSP_SetAdcTuneCfg(void);

    extern void vTriggerDSPSLT(void);
    extern void DSP_ResetDsp(UINT8 u1DspId);
    extern CHAR DSP_SetAudDramBlock(UINT8 u1DspId, UINT8 u1MemBlock);
    extern void IecInfoInit(void);
    extern void _ADSP_CFG_Reset(UINT8 u1DspId);
	
#endif
#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif  // DSP_RG_CTL_H

