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
/*
 * $Author: p4admin $
 * $Date: 2015/02/16 $
 * $RCSfile: drv_dvi.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
/**
 * @file DVI/HDMI/CCIR decoder driver interface
 *
 * @author daniel_hsiao@mtk.com.tw
 */

#ifndef _DRV_DVI_H_
#define _DRV_DVI_H_

#include "typedef.h"

#define CCIR_decode_444	 1

extern BOOL   _IsDviDetectDone;
extern UINT8   _bDviExtSrc;
extern UINT8   _bSync1Vsyn2;
extern UINT8   _bDviTiming;
extern UINT16   _wDviWidth;
extern UINT16   _wDviHeight;
extern UINT8   _bDviVclk;
extern UINT8   _bDviModeDetState;
extern UINT8 _bDviChkState;
extern UINT32 _bQueryMode;

#define fgIsExtSrcDVI() (_bDviExtSrc&SV_DV_DVI)
#define fgIsExtSrcHDMI() (_bDviExtSrc&SV_DV_HDMI)
#define fgIsExtSrcAdYPbPr() (_bDviExtSrc&SV_DV_AD_YPbPr)
#define fgIsExtSrcAdRGB() (_bDviExtSrc&SV_DV_AD_RGB)
#define fgIsExtSrcCCIR() (_bDviExtSrc&SV_DV_CCIR)
#define fgIsExtSrcFromCCIR() (_bDviExtSrc&SV_DV_FROM_CCIR)
#define fgIsExtSrcDTV() (_bDviExtSrc&SV_DV_DTV)

#define RANGE_CHECKING(a, b, offset)  ((UINT32)((a)+(offset)-(b)) <= ((offset)*2))

extern void vCCIROSDOnOff(UINT8 fgIsOn);
extern void vCCIROSDInitial(void);
extern void vCCIROSDConnect(UINT8 bchannel, UINT8 fgIsOn) ;
extern void vCCIROSDConnect(UINT8 bchannel, UINT8 fgIsOn);
extern UINT16 wCCIROSDInputHTotal(void);
extern UINT16 wCCIROSDInputVTotal(void);
extern UINT16 wCCIROSDInputWidth(void);
extern UINT16 wCCIROSDInputHeight(void);
extern UINT8 bCCIROSDRefreshRate(void);
extern UINT8 bCCIROSDInterlace(void);
extern void vCCIROSDSETVSTART(void);
extern UINT8 bCCIROSDSigStatus(void);
extern void vCCIROSDDebugMsg(void);

extern void vDviConnect(UINT8 bchannel, UINT8 fgIsOn) ;
extern UINT16 wDviInputWidth(void) ;
extern UINT16 wDviInputHeight(void) ;
extern UINT8 bDviRefreshRate(void) ;
extern UINT8 bDviInterlace(void) ;
extern UINT8 bDviSigStatus(void) ;
extern UINT8 bDviIsVideoTiming(void);
extern void vDviISR(void) ;
extern UINT8 bGetSupportHdmiModeVideoTiming(void);
extern void vDviModeDetect(void) ;
extern void vDviChkModeChange(void) ;

extern void vDviSetAvStart(UINT16 wStart) ;
extern void vDviSetAvWidth(UINT16 wWidth)  ;
extern void vDviSetVsStart(UINT16 wStart) ;
extern void vDviSetVsWidth(UINT16 wWidth)  ;
extern void vHdmiInformModeChg(void);
extern UINT16 wDviGetAvWidth(void) ;

UINT8 bHDMI3DIPCheck(void);
UINT8 bDviStdTimingSearch(UINT8 ) ;
void vDviInitial(void);
void vDVIDebugMsg(void);
extern UINT8 bDVICENStatus(void);
extern UINT8 bCCIRCENStatus(void);
extern UINT16 wDviInputHTotal(void);
extern UINT16 wDviInputVTotal(void);
extern void vDviTDTVCB(void *pvArgv);
extern void bHDMI3DLRFreeRunControl(UINT8 bvalue);
#ifdef CC_AUD_HDMI_SPDIF_CFG
extern UINT8 API_Get_Audio_SPDIF_STATUS(void);
#endif
//extern void vHDMISwitchInit(void);
#endif
void vDviSetConnetForAudio(UINT8 bchannel,UINT8 fgIsOn);

