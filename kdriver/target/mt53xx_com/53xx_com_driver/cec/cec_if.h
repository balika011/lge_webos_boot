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
 * $RCSfile: cec_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _CEC_IF_H_
#define _CEC_IF_H_

#include "x_typedef.h"

// *********************************************************************
// Type definitions
// *********************************************************************
typedef struct
{
	UINT8 destination : 4;
	UINT8 initiator   : 4;
} CEC_HEADER_BLOCK;

typedef struct
{
	CEC_HEADER_BLOCK header;
	UINT8 opcode;
	UINT8 operand[16];
} CEC_FRAME_BLOCK;

typedef struct
{
	UINT8 size; // Actual frame block size.
	UINT8 sendidx;
	UINT8 reTXcnt;
	void* txtag;
	CEC_FRAME_BLOCK blocks;
} CEC_FRAME_DESCRIPTION;
typedef struct
{
	UINT8 tvLA1:4;
	UINT8 tvLA2:4;
}CEC_TV_LA_t;
#ifdef CC_FOR_POC_DEMO
typedef struct 
{
    BOOL         fgActSrcReved;
    CEC_FRAME_DESCRIPTION eFrameDescription;
}CEC_FRAME_ACTSRCREVED;
#else
typedef struct 
{
    BOOL         fgActSrcReved;
    UINT8        u1PhyAddr[2];
}CEC_FRAME_ACTSRCREVED;
#endif
#ifdef LG_CEC
typedef struct
{
    UINT8 ui1_txResult;
    CEC_FRAME_DESCRIPTION tFrame;
}CEC_TX_RESULT;
#endif

/* Notify function */
typedef void (*x_cec_drv_nfy_data_arrival) (CEC_FRAME_DESCRIPTION* frame);
typedef void (*x_cec_drv_nfy_tx_result) (UINT8 result,CEC_FRAME_DESCRIPTION *frame, void* txtag);
typedef void (*x_cec_drv_nfy_tx_header_result) (UINT8 result, CEC_FRAME_DESCRIPTION *frame,void *arg);

//#define CECNFY5VSTATUS 0
#ifdef CECNFY5VSTATUS
typedef void (*x_cec_drv_nfy_5v_status) (UINT8 bSwitch, BOOL fgIs5VOn,void *arg);
#endif

// *********************************************************************
// Constant definitions
// *********************************************************************

// *********************************************************************
// Export API
// *********************************************************************
EXTERN void CEC_SetRxCallback(x_cec_drv_nfy_data_arrival nfyinfo,void *arg);
EXTERN void CEC_SetTxCallback(x_cec_drv_nfy_tx_result nfyinfo,void *arg);

#ifdef CECNFY5VSTATUS
EXTERN void CEC_Set5VCallback(x_cec_drv_nfy_5v_status nfyfp,void *arg);
EXTERN void _Cec_Notify5VStatus(UINT8 u1Port, UINT8 u15vStatus);
#endif

EXTERN void CEC_SetTxHeaderCallback(x_cec_drv_nfy_tx_header_result nfyfp,void *arg);
#if defined(SYS_ANDROID_TV_TIF_SUPPORT)
EXTERN void CEC_HALSetMWReady(UINT8 mwrdy_val);
#else
EXTERN void CEC_SetMWReady(void);
#endif
EXTERN void CEC_SetLogicAddress(UINT8 u1LogicalAddr);
EXTERN void CEC_ClrLogicAddress(void);
EXTERN void CEC_SendStandby(UINT8 u1Standby);
EXTERN void CEC_SetWakeUp(UINT8 u1WakeUp);
EXTERN void CEC_SendStandardCmd(UINT8 u1StandardCmd);
EXTERN void CEC_SetEnable(BOOL fgEnable);
EXTERN UINT8 CEC_GetLogicalAddr(void);
EXTERN void CEC_CheckActiveSource(CEC_FRAME_ACTSRCREVED *prActiveSrc);
EXTERN void CEC_Init(void);
EXTERN void CEC_Mainloop(void);
EXTERN void CEC_SetSysVtrl(UINT8 u1Onoff);
EXTERN void CECSetDbgLvl(UINT8 u1dbglvl);
EXTERN UINT8 u1CecDbgLvl;

#ifdef CECNFY5VSTATUS
EXTERN void _Cec_Notify5VStatus(UINT8 u1Port, UINT8 u15vStatus);
#endif
EXTERN UINT8 CEC_EnqueueTx(CEC_FRAME_DESCRIPTION* frame);
EXTERN void _CEC_TestCmd(UINT8 cmd);
//cec Arc control
#if defined (CC_HDMI_ARC_SPDIF_CTRL)|| defined (CC_HDMI_ARC_SPDIF_CTRL_89)
//#if defined(CC_MT5396) ||defined(CC_MT5368) || defined(CC_MT5389)  ||defined(CC_MT5398) ||defined(CC_MT5880) || defined(CC_MT5881)
EXTERN void CEC_SetArcEnable(BOOL fgEnable);

//#endif
#endif
#ifdef LG_CEC
EXTERN void CEC_ReadRxCommand(CEC_FRAME_DESCRIPTION *pt_frame);
EXTERN void CEC_ReadTxResult(CEC_TX_RESULT *pt_txResult);
EXTERN UINT16 CEC_ReadResponse(void);
#endif

#ifdef CC_FOR_POC_DEMO
typedef void (*x_philips_cec_drv_nfy_5v_status) (UINT8 u1HdmiChangeType,UINT8 bSwitch, BOOL fgIs5VOn);
extern void CEC_SetPhilips5VCallback(x_philips_cec_drv_nfy_5v_status nfyfp);
#endif

#endif /* _CEC_IF_H_ */

