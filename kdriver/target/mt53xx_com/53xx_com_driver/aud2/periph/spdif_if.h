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
 * $RCSfile: spdif_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file spdif_if.h
 *  Brief of file spdif_if.h.
 *  Details of file spdif_if.h (optional).
 */

#ifndef _SPDIF_IF_H_
#define _SPDIF_IF_H_

// *********************************************************************
// Type definitions
// *********************************************************************
typedef struct _SPDIF_IN_FLAG_T
{
  /* BYTE 0 */
  UINT32 fgSpdifChk:1;
  UINT32 fgSpdifLock:1;
  UINT32 fgChkTimeOut:1;
  UINT32 fgAUDION:1;
  UINT32 Freq:4;
  /* BYTE 1 */
  UINT32 fgPEM:1;
  UINT32 Reserve:23;
} SPDIF_IN_FLAG_T;

// *********************************************************************
// Constant definitions
// *********************************************************************

#define RTJ_16BIT                   0x00
#define RTJ_18BIT                   0x01
#define RTJ_20BIT                   0x02
#define RTJ_24BIT                   0x03
#define I2S_16BIT                   0x04
#define I2S_18BIT                   0x05
#define I2S_20BIT                   0x06
#define I2S_24BIT                   0x07
#define LTJ_16BIT                   0x08
#define LTJ_18BIT                   0x09
#define LTJ_20BIT                   0x0A
#define LTJ_24BIT                   0x0B


#define FS256_16K                   0x00
#define FS256_22K                   0x01
#define FS256_24K                   0x02
#define FS256_32K                   0x03
#define FS256_44K                   0x04
#define FS256_48K                   0x05
#define FS256_64K                   0x06
#define FS256_88K                   0x07
#define FS256_96K                   0x08
#define FS256_176K                  0x09
#define FS256_192K                  0x0A
#define FS512_44K                   0x0B

#define FS384_16K                   0x00
#define FS384_22K                   0x01
#define FS384_24K                   0x02
#define FS384_32K                   0x03
#define FS384_44K                   0x04
#define FS384_48K                   0x05
#define FS384_64K                   0x06
#define FS384_88K                   0x07
#define FS384_96K                   0x08
#define FS384_176K                  0x09
#define FS384_192K                  0x0A
#define FS768_44K                   0x0B


#define AK4114_DEV_ADDR    0x10
// setting sif clk 
#define SIF_CLK_DIV 0x100

// for Audion Sampling rate
#define AFS512_48K                  0
#define AFS256_96K                  1
#define AFS128_192K                 2

#define DSP_AIN_DISABLE     0x00
#define DSP_AIN_SPDIF       0x01
#define DSP_AIN_LINE        0x03
#define DSP_AIN_AUDION      0x04
#define DSP_AIN_PEM         0x08

#define DSP_AIN_FREQ_MASK   0xf0
#define DSP_AIN_FREQ44_1K   0x00
#define DSP_AIN_FREQ48K     0x20
#define DSP_AIN_FREQ32K     0x30
#define DSP_AIN_FREQ88_2K   0x80
#define DSP_AIN_FREQ96K     0xa0
#define DSP_AIN_FREQ176_4K  0xc0
#define DSP_AIN_FREQ192K    0xe0

//  for karaoke scoring
#define  DSP_KSCORE_OFF      0x0
#define  DSP_KSCORE_ON       0x1
#define  DSP_KSCORE_RESET    0x2
#define  DSP_KSCORE_MAXSCORE 100
#define  DSP_KSCORE_MINSCORE 0

// for voice detection
#define  DSP_VOCDTKN_ON      0x1
#define  DSP_VOCDTKN_OFF     0x0
#define DSP_VOCDTKN_MAN_TH   0x02

// for ADAC_CMD_AIN_SEL
#define AIN_SEL_OFF                 0x00
#define AIN_SEL_D0                  0x10
#define AIN_SEL_D1                  0x11
#define AIN_SEL_D2                  0x12
#define AIN_SEL_D3                  0x13
#define AIN_SEL_L0                  0x20
#define AIN_SEL_L1                  0x21
#define AIN_SEL_L2                  0x22
#define AIN_SEL_L3                  0x23

#define SR_AIN_FLAG         0x04
#define SR_ADAC_ST          0x05

// *********************************************************************
// Export API
// *********************************************************************
void vSpdifDataFormat(BYTE bDataFormat);
void vSpdifChgMCLK(BYTE bSampleRate);
UINT8 bSpdifDetSmpRate(void);
BOOL fgSpdifDetPreEmphasis(void);
BOOL fgSpdifDetAudioBitOut(void);
void vSpdifDeEmphasisEN(BYTE bEnable);
void vSpdifPWN(BYTE bEnable);
void vSpdifChInSel(BYTE bCh);
void vSpdifClkMode(BYTE bClkMode);
void vSpdifINT0(BYTE bUlockEn , BYTE bPAREn);

/** SPDIF Transciver Initial Routine.
 *
 *  @retval void
 */
extern void SPDIF_InInit(void);

/** SPDIF lock check.
 *
 *  @retval void
 */
extern BOOL SPDIF_InLockCheck(void);
  
/** Rigister data dump.
 *
 *  @retval void
 */
extern void SPDIF_InRegisterDump(void);

/** SPDIF in channel select.
 *
 *  @param[in]     u1Ch   "0":RX0,"1":RX1,"2":RX2,"3":RX3.
 *
 *  @retval void
 */
extern void SPDIF_InChSel(UINT8 u1Ch);

/** SPDIF in enable.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for power down.
 *
 *  @retval void
 */
extern void SPDIF_InEnable(BOOL fgEnable);

#endif  //_SPDIF_IF_H_

