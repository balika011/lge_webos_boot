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
#ifndef _MPEG_DEC_H_
#define _MPEG_DEC_H_

#include "general.h"

typedef struct
{
	BOOL fgB2R3DEnable;
	UINT8 u1B2R3DType;
}B2R_3D_INFO;

extern void vMpegSdConnect(UINT8 bchannel, UINT8 fgIsOn);
extern void vMpegHdConnect(UINT8 bchannel, UINT8 fgIsOn);
extern UINT16 u2MpegHdInputWidth(void);
extern UINT16 u2MpegSdInputWidth(void);
#if defined(CC_MT5882)
extern UINT16 u2MpegHdInputValidDataWidth(UINT8 bPath);
extern UINT16 u2MpegSdInputValidDataWidth(UINT8 bPath);
extern UINT16 u2MpegHdInputHeight(UINT8 bPath); 
extern UINT16 u2MpegSdInputHeight(UINT8 bPath);
extern UINT8 u1MpegHdRefreshRate(UINT8 bPath);
extern UINT8 u1MpegSdRefreshRate(UINT8 bPath);
extern UINT16 u2MpegHdHTotal(UINT8 bPath);
extern UINT16 u2MpegSdHTotal(UINT8 bPath);
extern UINT16 u2MpegHdVTotal(UINT8 bPath);
extern UINT16 u2MpegSdVTotal(UINT8 bPath);
extern UINT8 u1MpegHdTiming(UINT8 bPath);
extern UINT8 u1MpegSdTiming(UINT8 bPath);
extern UINT8 u1MpegSdInterlace(UINT8 bPath);
extern UINT8 u1MpegHdInterlace(UINT8 bPath);
#else
extern UINT16 u2MpegHdInputValidDataWidth(void);
extern UINT16 u2MpegSdInputValidDataWidth(void);
extern UINT16 u2MpegHdInputHeight(void); 
extern UINT16 u2MpegSdInputHeight(void);
extern UINT8 u1MpegHdRefreshRate(void);
extern UINT8 u1MpegSdRefreshRate(void);
extern UINT16 u2MpegHdHTotal(void);
extern UINT16 u2MpegSdHTotal(void);
extern UINT16 u2MpegHdVTotal(void);
extern UINT16 u2MpegSdVTotal(void);
extern UINT8 u1MpegHdTiming(void);
extern UINT8 u1MpegSdTiming(void);
extern UINT8 u1MpegSdInterlace(void);
extern UINT8 u1MpegHdInterlace(void);
#endif
extern UINT8 u1MpegHdSigStatus(UINT8 bPath);
extern UINT8 u1MpegSdSigStatus(UINT8 bPath);
extern UINT8 u1MpegHdClock(void);
extern UINT8 u1MpegSdClock(void);

extern UINT32 u4MpegHdQuality(void);
extern UINT32 u4MpegSdQuality(void);
extern UINT8 u1MpegHdColorSpace709(void);
extern UINT8 u1MpegSdColorSpace709(void);
extern UINT8 u1MpegHdColourPrimary(void);
extern UINT8 u1MpegSdColourPrimary(void);
extern UINT8 u1MpegHdTransCharacter(void);
extern UINT8 u1MpegSdTransCharacter(void);
extern UINT8 u1MpegHdMatrixCoeff(void);
extern UINT8 u1MpegSdMatrixCoeff(void);
extern UINT8 u1MpegHdColorDescrip(void);
extern UINT8 u1MpegSdColorDescrip(void);
extern UINT32 u4MpegHdHdeRatio(void);
extern VOID vMpegGet3DInfo(UINT8 u1B2RId,B2R_3D_INFO* prB2R3DInfo);
extern UINT32 u4MpegHdPreciseRefreshRate(void);
extern UINT32 u4MpegSdPreciseRefreshRate(void);

extern VOID vMpegInit(void);
extern VOID vMpegInitB2rConf(void);
extern VOID vMpegModeChg(UCHAR ucVdpId);
extern VOID vMpegModeDetDone(UCHAR ucVdpId);

#endif
