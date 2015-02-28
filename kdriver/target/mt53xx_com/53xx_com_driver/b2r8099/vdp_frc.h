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
 * $Date: 2015/02/28 $
 * $RCSfile: vdp_frc.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_frc.h
 *  Brief of file vdp_frc.h.
 *  Details of file vdp_frc.h (optional).
 */

#ifndef VDP_FRC_H
#define VDP_FRC_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "b2r_if.h"
#include "b2r_drvif.h"

#ifndef CC_DISABLE_EARLY_DISPLAY_MUTE
#define VDP_MUTE_EARLY_DISPLAY
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

VOID _B2R_FrcInit(void);
UINT32 _B2R_FrcHandlerEvent(UCHAR ucB2rId, UINT32 u4Event);
UINT32 _B2R_FrcProc(B2R_OBJECT_T * this, UCHAR ucBottom, UCHAR ucRightView);
VOID _B2R_IsrFin(B2R_OBJECT_T* this);
VOID _B2R_FrcProcEarly(B2R_OBJECT_T * this, UCHAR ucBottom);
BOOL _B2R_FrcGetUnMuteStatus(UCHAR ucB2rId);
UCHAR _B2R_FrcGetLockFrameBuffer(UCHAR ucB2rId);
UINT32 _B2R_GetCurrentPts(UCHAR ucB2rId);
VOID _B2R_SetCurrentTick(UCHAR ucB2rId, UINT32 u4Tick);
UINT32 _B2R_GetLastPts(UCHAR ucB2rId);
UCHAR _B2R_GetFbg(UCHAR ucB2rId);
UCHAR _B2R_GetOutFrameRate(UCHAR ucB2rId);
UINT32 _B2R_GetFilePosition(UCHAR ucB2rId,
                                   INT32 *pi4TempRef, UINT16 *pu2DecodingOrder,
                                   UINT64 *pu8Offset, UINT64 *pu8OffsetI);
UINT32 _B2R_GetPosInfo(UCHAR ucB2rId, VDP_POS_INTO_T *prPosInfo);
UCHAR _B2R_GetPendingFB(UCHAR ucB2rId, BOOL fgSubFbg);
VOID _B2R_CleanPendingFB(UCHAR ucB2rId, BOOL fgSubFbg);
VOID _B2R_SetReleaseDispQ(UCHAR ucB2rId, BOOL fgSubFbg);
VOID _B2R_FrcSetLipSyncNGOKCb(UCHAR ucNgOk, UCHAR ucRemove, UINT32 u4Thrsd, UINT32 u4FrmCnt);
UINT32 _B2R_TrickModeSkipFrameNo(UCHAR ucB2rId);
/*VDP Set Current MM Speed*/
VOID _B2R_FrcStcSpeed(UCHAR ucB2rId,STC_SPEED_TYPE_T eSpeed);
VOID _B2R_SetB2rNoLock(UINT8 u1B2rId, BOOL fgNoLock);
VOID _B2R_Set3DInfo(UCHAR ucB2rId,UINT8 u13DType,BOOL fg3DEnable);
BOOL _B2R_IsStartToPlay(UCHAR ucB2rId);
UINT32 _B2R_GetDispBufLocked(UCHAR ucB2rId, UINT32 u4BufAddr, BOOL *pfgLocked);
BOOL _B2R_SendB2RChgFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg);
BOOL _B2R_SendB2RAysncRenderFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg);
BOOL _B2R_FlushB2RChgFrameMsg(UCHAR ucVdpId);
VOID _B2R_ResetPts(UCHAR ucB2rId);
void _B2R_FrcResetPort(UCHAR ucB2rId, UCHAR ucPort);
UCHAR _B2R_Fbg2B2r(UCHAR ucFbgId);
#ifdef VDP_B2R_PSCAN_CRC_FLOW
/* B2R CRC check on PTS value */
void _VDPSetChkPTS(UINT32 u4Pts);
void _VDP_SetEncTrigger(BOOL fgEnable);
#endif

B2R_VAR_T* _B2R_GetB2rVar(UCHAR ucB2rId);
B2R_PRM_T* _B2R_GetB2rPrm(UCHAR ucB2rId);
UINT32 _B2R_GetFrcEsmQCnt(void);
#ifdef CC_MAPLE_CUST_DRV
INT32 _B2R_Get_Cust_info(UCHAR              ucB2rId,
                              B2R_CUST_INFO_T*    pt_cust_info);
INT32 _B2R_GetAspInfo(UCHAR     ucB2rId,
                           UINT8*    pu1_asp_info);
#endif

#if defined(CC_MT5890) && defined(CC_SUPPORT_4KBE)
UINT32 VDP_SetVB1ControlBit(B2R_OBJECT_T *this);
UINT32 VDP_SetVB1ControlBitTest(B2R_OBJECT_T *this,UINT32 u4SrcWidth,UINT32 u4SrcHeight);
#endif

VOID _B2R_RTCliActive(UCHAR ucB2rId,  UCHAR ucEnable);
VOID _B2R_SetLatency(UCHAR ucB2rId,  UINT32 u4Latency);
VOID _B2R_QueryLatency(UCHAR ucB2rId);
VDP_CFG_T* _B2R_GetVdpConf(UCHAR ucVdpId);
void _B2R_VdpCfgReset(UCHAR ucVdpId);
UINT32 _VDP_HalSetRenderRegion(UCHAR ucB2rId, VDP_REGION_T* prRenderRegion);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
VOID _B2R_FrcSetChapterEnable(VDP_CHAPTER_T*    prChapter);
#endif
#endif

INT32 _B2R_Height_Align(UINT32*  pu4_height);
VOID _B2R_TriggerAudReceive(UCHAR ucVdpId, UINT32 u4Pts);

INT32 _B2R_AudTrigger(UINT8 u1B2rId, VDP_TRIG_AUD_CFG_T* pt_trig_cfg);

#ifdef CC_B2R_RES_SUPPORT
BOOL _B2R_ChkHwSwitch(UCHAR ucVdpId,
                               UCHAR ucB2rId,
                               UCHAR* pucRetId);

UCHAR _B2R_ChkHwConflict(UCHAR ucVdpId,
                               UCHAR ucB2rId);

void _B2R_HandleScartOut(UCHAR ucVdpId, 
                               UCHAR ucEsId, 
                               UCHAR ucPort);
#endif

#endif    // VDP_FRC_H

