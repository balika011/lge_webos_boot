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
 * $RCSfile: vdo_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_venc_ctrl.h
 *  Venc VDP control interface
 */

#ifndef _VDP_VENC_CTRL_H_
#define _VDP_VENC_CTRL_H_

#ifdef CC_SUPPORT_VENC

//#include "mtvenc.h"
#include "venc_if.h"

/** MTVENC_VDP_RES_t
 *  Encoder input resolution
 */
typedef struct _VDP_VENC_INPUT_RES
{
    UINT32 u4EncInWidth;
    UINT32 u4EncInHeight;
    UINT8  fgInterlace;
    UINT32 u4Fps;
}VDP_VENC_INPUT_RES_t;


//VENC api
typedef enum{
    E_VDP_VENC_BOTH_FLD = 0,
    E_VDP_VENC_TOP_FLD,
    E_VDP_VENC_BOTTOM_FLD,
    E_VDP_VENC_PROGRESSIVE,  
} E_VDP_VENC_FIELD_MODE; 


/** MTVENC_SRC_TYPE_T
 *  set the record input point
 */
typedef enum
{
	E_VDP_VENC_MIXER = 0,					 ///< encoder MIXER
	E_VDP_VENC_MAIN_FSC_OUT,
	E_VDP_VENC_SUB_FSC_OUT,
	E_VDP_VENC_MAIN_MIB_OUT,
	E_VDP_VENC_SUB_MIB_OUT,
	E_VDP_VENC_MAIN_BEFORE_PRED ,			///< encoder before MAIN predown
	E_VDP_VENC_SUB_BEFORE_PRED, 			///< encoder before SUB predown
	E_VDP_VENC_OSTG,						///< encode output stage
	E_VDP_VENC_OSTG_OSD  ,					///< encode output stage + OSD
	E_VDP_VENC_NOT_SELECTED,				///< encode input is not selected
}E_VDP_VENC_INPUT;

typedef enum
{
	E_SUB_VENC_SUB_SCALER_IN = 0,		   ///< encoder MIXER
	E_SUB_VENC_MAIN_BEFORE_PRED ,		   ///< encoder MIXER
	E_SUB_VENC_SUB_BEFORE_PRED ,		   ///< encoder MIXER
	E_SUB_VENC_OSTG,					   ///< encode input is not selected
	E_SUB_VENC_OSTG_OSD,				   ///< encode input is not selected
	E_SUB_VENC_NOT_SELECTED,			   ///< encode input is not selected
}E_VDP_VENC_SUB_INPUT;


/** MTVENC_SRC_TYPE_T
 *  set the recording module
 */
typedef enum
{
    E_VDP_VENC_MAIN_SCALER = 0,                    ///< using main scaler to encode
    E_VDP_VENC_SUB_SCALER  = 1,                    ///< using sub scaler to encode
    E_VDP_VENC_MONITOR_OUT = 2,                    ///< using Monitor out to encode
    E_VDP_VENC_SUB_DI      = 3,                    ///< using sub DI to encode
} E_VDP_VENC_MODULE;

typedef struct VDP_VENC_INFO
{
    VENC_PROFILE_T           eProfileDef;
    E_VDP_VENC_INPUT         eEncInput; 
	E_VDP_VENC_SUB_INPUT     eSubEncInput;
    E_VDP_VENC_MODULE        eEncModule;
    E_VDP_VENC_FIELD_MODE    eEncFldType;
    UINT32                   u4TrgtWidth;    //recording width
    UINT32                   u4TrgtHeight;   //recording height
}VDP_VENC_INFO_t;

//#define VENC_SUB_SCALER_SCALING


void vDrvVencVDPDropFrame(UINT8 u1OnOff, UINT8 u1M, UINT8 u1N);
void vDrvVencVDPSetScalerBuf(UINT32 u4BaseAdd, UINT32 u4YBufSize);

void vDrvVencVDPSetParam(VENC_PROFILE_T eProfileDef, UINT32 u4RecWidth, UINT32 u4RecHeight);
void vDrvVencVDPStop(void);
void vDrvVencVDPStart(void *handle, void *ptFrmBuffMgr, void *ptResizeFrmBuffMgr, void *ptFilterPts);

UINT8 u1DrvVencVDPVRMReady(void);
UINT8 u1DrvVencVDPSignalReady(void);

void vDrvVencCtrlStopModule(void);
UINT8 bDrvVencVDPIsOneFldMode(void);
void vDrvVencVDPInit(void);
void vDrvVencVDPCtrlMLoop(void);

void vDrvVencVDPSetEncFldType(E_VDP_VENC_FIELD_MODE eEncFldType);
void vDrvVencVDPSetEncModule(E_VDP_VENC_MODULE eEncModule);
void vDrvVencVDPSetEncInput(E_VDP_VENC_INPUT eEncInput);
void vDrvVencVDPGetEncInRes(VDP_VENC_INPUT_RES_t* tRes);

E_VDP_VENC_MODULE     eDrvVencVDPGetEncModule(void);
E_VDP_VENC_INPUT      eDrvVencVDPGetEncInput(void);

UINT32 u4DrvVencVDPCtrlCheckFrameRate(UINT32 u4APFR, UINT32 u4InFR);

#endif // CC_SUPPORT_VENC

#endif // _VDP_VENC_CTRL_H_
 
