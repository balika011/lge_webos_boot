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
 * $RCSfile: vdp_hal.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_hal.h
 *  Brief of file vdp_hal.h.
 *  Details of file vdp_hal.h (optional).
 */

#ifndef VDP_HAL_H
#define VDP_HAL_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"

#ifdef __MODEL_slt__
#define VDP_B2R_PSCAN_CRC_FLOW
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// By Pass FRC
//#define VDP_BYPASS_FRC

// Log FRC status, Print if FRC happen
//#define VDP_FRC_LOG

// Freeze SCPOS, dump SCPOS frame buffer
//#define VDP_FRC_TRAP

// Log FRC status, Print if invalid FRC happen
//#define VDP_FRC_LOG_CHECK


// Send 1st Field Only
#define VDP_FRC_WA2

// for test only
//#define VDP_STATUS_TEST


// Use PSCAN interface to control AUTO STOP
#define VDP_FRC_AUTO_STOP_CTRL


// Use inverse field control to manual setup output field (currently used in 3:2, TargetNs = 3 case)
#define VDP_FRC_INV_FIELD_CTRL



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#define VDP_PD_VIDEO_MODE                   0
#define VDP_PD_32_MODE                      1
#define VDP_PD_22_MODE                      2
#define VDP_PD_STOP_MODE                    3

#define VDP_FRC_NORMAL                      0
#define VDP_FRC_NONE                        1   // no FRC
#define VDP_FRC_STOP_AUTO                   2   // stop auto at last field
#define VDP_FRC_TRICK_PLAY                  3   // Trick Play at 20081001
#define VDP_FRC_CANCEL                      4   // vdp cancel for 4 frame buffer at 20090630

#define VDP_ISR_VEI                         0x00000080
#define VDP_ISR_ADI                         0x00000100
#define VDP_ISR_FI                          0x00000200

#define VDP_CLK_148_5_MHZ                   1
#define VDP_CLK_148_3515_MHZ                2   // 148.5/1.001


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//EXTERN void _B2R_B2rSwitch(UCHAR ucVdpId, UCHAR ucB2rId);
//EXTERN void _VDP_HalInit(void);
//EXTERN void _VDP_HalIsrFin(UCHAR ucB2RId);
//EXTERN void _VDP_HalReset(void);
//EXTERN void _VDP_HalSetEnable(UCHAR ucB2RId, UCHAR ucEnable);
//EXTERN void _VDP_HalSetChromaMode(UCHAR ucB2rId, UCHAR ucFrameEncode, UCHAR ucTff);
//EXTERN void _VDP_HalSetSrcSize(UCHAR ucB2rId, UINT32 u4Width, UINT32 u4Height, UINT32 u4Pitch);
//EXTERN void _VDP_HalSetOutClock(UCHAR ucB2rId, UINT32 u4Clock);
//EXTERN void _VDP_HalSetOutSize(UCHAR ucB2rId, UINT32 u4Width, UINT32 u4Height, UINT32 u4Interlace, UINT32 u4DoubleClk, UINT32 u4ClkMode);
//EXTERN void _VDP_HalSetBg(UCHAR ucB2rId, UINT32 u4BgColor);
//EXTERN void _VDP_HalForceBg(UCHAR ucB2rId, UCHAR ucForce);
//EXTERN void _VDP_HalSetAddr(UCHAR ucB2rId, UINT32 u4AddrY, UINT32 u4AddrC);
//EXTERN void _VDP_HalSetRViewAddr(UCHAR ucB2rId, UINT32 u4AddrY, UINT32 u4AddrC);
//EXTERN void _VDP_HalConfig(UCHAR ucB2rId);
//EXTERN void _VDP_HalSetFrc(UCHAR ucB2rId, UCHAR u2TargetNs, UCHAR u2ChangeFieldNs, UCHAR ucMode);
//EXTERN void _VDP_HalSetFormat(UCHAR ucB2rId, UINT32 u4Mode422, UINT32 u4AddrMode);
//EXTERN UINT32 _VDP_HalInsideBlankRegion(UCHAR ucB2rId);
//EXTERN void _VDP_HalSetBlack(UCHAR ucB2rId, BOOL fgBlack);
//EXTERN void VDP_HalPowerOffB2R(void);
//EXTERN void _VDP_HalStatusTest(UCHAR ucB2rId);
//EXTERN void _VDP_HalDisablePattern (UCHAR ucB2rId);
//EXTERN void VDP_HalsetMirror(UCHAR ucB2rId, BOOL fgOn);
//EXTERN void VDP_HalGetYCStart(UCHAR ucB2rId,UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive);
//EXTERN VOID VDP_HalSetLRState(UCHAR ucB2rId,BOOL fgLView);
//EXTERN VOID VDP_HalSetTBState(UCHAR ucB2rId,BOOL fgTopField);
//EXTERN VOID VDP_HalSetB2RPower(UCHAR ucB2rId,BOOL fgPowerOn);
//EXTERN VOID VDP_EnableDumpRegister(BOOL fgOn);
//EXTERN VOID VDP_EnableB2RFieldLog(BOOL fgOn);
//EXTERN VOID VDP_ForceResetClk(BOOL fgDo);
//EXTERN void VDP_EnableBurstRead(UCHAR ucB2rId, BOOL fgEn);
#if defined(CC_FLIP_MIRROR_SUPPORT)
///EXTERN void _VdpDoMirrorFlip(UCHAR ucB2rId,BOOL fgDo);
#endif
//EXTERN void _B2r_HAL_SetObject(UCHAR ucB2rId, VOID *pvData);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif    // VDP_HAL_H

