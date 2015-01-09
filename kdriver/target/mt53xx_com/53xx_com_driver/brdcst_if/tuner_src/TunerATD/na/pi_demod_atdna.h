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
 * $RCSfile: pi_demod_atdna.h,v $ 
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_demod_atdna.h
 *  
 */

#ifndef PI_DEMOD_ATDNA_H
#define PI_DEMOD_ATDNA_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "pi_demod_atd.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define cATDNA_VIF_PARTIAL_LOCK_RANGE   1500 //kHz
#define cATDNA_VIF_FULL_LOCK_RANGE      50   //kHz

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define mcMSG_IN_SEARCHNEARBY_AUTO_TEST(LO_STEPSIZE) if(1==fgEnableATP){mcDELAY_MS(200);mcSHOW_DBG_MSG(("##Start\n##Freq : 0x%X\n##Noise : 0x%X\n##ChromaGain : %d\n##Video : %s\n##End\n",((wFRTmp+45750) * 100)/(LO_STEPSIZE),IO_READ8(0x20022000, 0x80),IO_READ8(0x20022000, 0x9D) & 0x01,(((IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4)?"On":"Off")));}


#define mcMSG_IN_MONITOR_AUTO_TEST(LO_STEPSIZE) if(1==fgEnableATP){if((IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4){mcSHOW_DBG_MSG2(("##Start \n##Freq : 0x%X\n##Video : On\n##ChromaGain : %d\n##End\n",((Freq+45750) * 100)/(LO_STEPSIZE),IO_READ8(0x20022000, 0x9D) & 0x01)); }}

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
UINT8 ATDNA_Monitor(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, void *pvArg);
UINT8 ATDNA_SearchNearbyFreq(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct,
                                 void *pvArg, UINT32 u4FreqBDKHz);
#endif /* PI_DEMOD_ATDNA_H */

