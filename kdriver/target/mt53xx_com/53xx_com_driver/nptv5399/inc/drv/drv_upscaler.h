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
 *   $Workfile: drv_upscaler.h $
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
 * $Modtime: 04/05/28 9:33a $
 *
 * $Revision: #1 $
*************************************************************************/

#ifndef _DRV_UPSCALER_H_
#define _DRV_UPSCALER_H_

#include "hw_scpip_pq.h"

enum SCPQ_COEF_TYPE
{
	COEFTYP_HOR_Y=0,
	COEFTYP_HOR_C,
	COEFTYP_VER_Y,
	COEFTYP_VER_C,
	COEFTYP_MAX
};

enum SCPQ_DIR_TYPE
{
	SCPQ_DIR_H=0,
	SCPQ_DIR_V,
	SCPQ_DIR_MAX
};

enum SCPQ_INPUTBIT_NUM
{
	SCIN_422=0,
	SCIN_444,
	COEFMODE_MAX
};


#define SCPQ_PHASE_NUM 129
#define SCPQ_COEF_LEN 3
#define SCPQ_COEF_TBLSIZE (SCPQ_COEF_LEN*SCPQ_PHASE_NUM)
#define SCPQ_COEFF_TYPENUM 4
#define SCPQ_COEF_TOTAL_SIEZ (SCPQ_COEFF_TYPENUM*SCPQ_COEF_TBLSIZE)


void vDrvSCPQInit(void);
void vDrvSCPQSramCoeffOnOff(UINT8 bOnOff);
UINT8 bDrvSCPQCoeffValidate(UINT8 bType, UINT32 dwCoeffTblR[SCPQ_COEF_TBLSIZE], UINT32 dwCoeffTblW[SCPQ_COEF_TBLSIZE]);
void vDrvSCPQSetSramCoeff(UINT8 bType, UINT32 dwCoeff[SCPQ_COEF_TBLSIZE]);
UINT8 vDrvSCPQSetVCoeff(UINT8 bPath);
UINT8 vDrvSCPQSetHCoeff(UINT8 bPath);
void vDrvDeRingInit(void);
void vDrvDeRingOnOff(UINT32 u4Path, UINT32 u4Dir, UINT32 u4OnOff);
void vDrvSCPQCoeffRead(UINT8 bType, UINT32 dwResult[SCPQ_COEF_TBLSIZE]);
void vDrvSCPQTapAdapInit(void);
void vDrvSCPQTapAdapOnOff(UINT32 u4Path, UINT32 u4Dir, UINT32 u4OnOff);
void vDrvSCPQVCTIInit(void);
void vDrvSCPQVCTIOnOff(UINT8 bOnOff);
void vDrvSCPQIRRInit(void);
void vDrvSCPQIRROnOff(UINT8 bOnOff);
void vDrvUsPeiInit(void);
void vDrvUsPeiOnOff(UINT32 u4Path, UINT32 u4OnOff);

#endif

