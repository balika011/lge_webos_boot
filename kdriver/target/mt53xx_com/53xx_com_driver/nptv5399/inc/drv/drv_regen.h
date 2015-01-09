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
 /*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_regen.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Date: 2015/01/09 $
 *
 * $Revision: #1 $
*************************************************************************/
#ifndef _DRV_REGEN_H_
#define _DRV_REGEN_H_
#include "drv_scaler.h"

#define REGEN_WA1_FOR_MIB_HEIGHT
#define REGEN_BOTH_IN_PREREGEN_AND_FSCREGEN
extern UINT8 bREGENModeChanged[VDP_MAX_NS];

typedef struct
{
    UINT8 bREGENModeChanged;
    VRM_INFO_T regenIn;
    VRM_INFO_T regenOut;
	UINT32 u4InputWidth;			        //input width(pixel based)
	UINT32 u4InputHeight;                   //input height(pixel based)
	UINT32 u4OutputX;			            //output X(pixel based)
	UINT32 u4OutputY;                       //output Y(pixel based)
	UINT32 u4OutputWidth;                   //output Width(pixel based)
	UINT32 u4OutputHeight;	                //output Height(pixel based)    
}REGEN_INFO_T;

typedef enum
{
    REGEN_MODULE_PRE_REGEN,
    REGEN_MODULE_FSC_REGEN,
    REGEN_MODULE_MONOUT_REGEN,
}REGEN_MODULE_E;

typedef enum{
    REGEN_IN_FMT_2D=0,
    REGEN_IN_FMT_FP=2,
    REGEN_IN_FMT_FS=3,
    REGEN_IN_FMT_TB=4,
    REGEN_IN_FMT_SS=8,
    REGEN_IN_FMT_RD=10,
    REGEN_IN_FMT_QC=11,
    REGEN_IN_FMT_LI=12,
    REGEN_IN_FMT_PR=13,
    REGEN_IN_FMT_CI=14,
    REGEN_IN_FMT_CB=15
}REGEN_IN_FMT;

#define REGEN_WRITE_FLD(reg,value,fld) vIO32WriteFldAlign(reg, value, fld)
#define REGEN_READ_FLD(reg,fld) IO32ReadFldAlign(reg,fld)
#define getRegenInfo(bPath,mod) ((mod == REGEN_MODULE_PRE_REGEN)? &_regenInfo[bPath]: &_fscRegenInfo[bPath])

void vRegenSwInit(UINT8 bPath,REGEN_MODULE_E mod);
void vRegenHwInit(UINT8 bPath,REGEN_MODULE_E mod); 
void vRegenSetModeChanged(UINT8 bPath,REGEN_MODULE_E mod);
UINT8 fgRegenIsModeChanged(UINT8 bPath,REGEN_MODULE_E mod);
void vRegenClearModeChanged(UINT8 bPath,REGEN_MODULE_E mod);
void vRegenLoop(UINT8 bPath,REGEN_MODULE_E mod);

UINT32 u4RegenGetInHeight(UINT8 bPath,REGEN_MODULE_E mod);
UINT32 u4RegenGetInWidth(UINT8 bPath,REGEN_MODULE_E mod);
UINT32 u4RegenGetOutHeight(UINT8 bPath,REGEN_MODULE_E mod);
UINT32 u4RegenGetOutWidth(UINT8 bPath,REGEN_MODULE_E mod);

void vRegenSetInputResolutionReg(UINT32 u4VdpId, REGEN_MODULE_E mod,UINT32 u4InW, UINT32 u4InH);
void vRegenSetOutputResolutionReg(UINT32 u4VdpId, REGEN_MODULE_E mod, UINT32 u4InW, UINT32 u4InH);
void vRegenSetLStartReg(UINT32 u4VdpId, REGEN_MODULE_E mod, UINT32 u4StaH, UINT32 u4StaV);
void vRegenSetRStartReg(UINT32 u4VdpId, REGEN_MODULE_E mod, UINT32 u4StaH, UINT32 u4StaV);
void vRegenSetRegenEnableReg(UINT8 bPath, REGEN_MODULE_E mod, UINT8 bOnOff);
void vRegenSetClipEnableReg(UINT8 bPath, REGEN_MODULE_E mod, UINT8 bOnOff); 
void vRegenClearSetResOutput(UINT8 bPath, REGEN_MODULE_E mod);
void vRegenClearSetResInput(UINT8 bPath, REGEN_MODULE_E mod);
UINT8 bRegenGetSetResOutputStatus(UINT8 bPath, REGEN_MODULE_E mod);
UINT8 bRegenGetSetResInputStatus(UINT8 bPath, REGEN_MODULE_E mod);
void vRegenSetRegenInFormat(UINT8 bPath, REGEN_MODULE_E mod,REGEN_IN_FMT eInFmt);

void vRegenSetImport(UINT8 bPath,REGEN_MODULE_E mod);
void vRegenSetResolutionMode(UINT8 bPath, REGEN_MODULE_E mod, UINT8 bOnOff);
void vRegenSetResolutionInTriSel(UINT8 bPath, REGEN_MODULE_E mod, UINT8 bOnOff);
void vRegenSetResolutionBypass(UINT8 bPath, REGEN_MODULE_E mod, UINT8 bBypass);

void vDrvRegenClrSetRes(UINT8 u1VdpId, REGEN_MODULE_E mod, SET_RES_STATUS_TYPE_E eStatusType);
BOOL bDrvRegenIsSetResDone(UINT8 u1VdpId, REGEN_MODULE_E mod, SET_RES_STATUS_TYPE_E eStatusType);
void vDrvRegenInOutStatusDump(UINT8 bPath,REGEN_MODULE_E mod);

#endif
