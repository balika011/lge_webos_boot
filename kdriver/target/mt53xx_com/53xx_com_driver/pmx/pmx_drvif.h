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
 * $RCSfile: pmx_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pmx_drvif.h
 *  plane mixer(pmx) public functions
 */

#ifndef PMX_DRVIF_H
#define PMX_DRVIF_H

/*lint -e717*/

/*
e717 : do ... while(0)
*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** maximum number of plane mixer */
#define PMX_MAX_NS 1
/** pmx id 1 */
#define PMX_1 0

/** pmx set value fail */
#define PMX_SET_ERROR 0
/** pmx set value ok */
#define PMX_SET_OK 1

#define PMX_MAIN 	0 // VDP1
#define PMX_PIP  	1 // VDP2
#define PMX_OSD1 	2 // VDP3
#define PMX_OSD2 	3 // VDP4
#define PMX_OSD3 	4 // VDP5
#define PMX_ALPHA 	5 // MIX alpha
#define PMX_AL_11 	5 // OSD_MIX_AL_11
#define PMX_AL_12 	6 // OSD_MIX_AL_12
#define PMX_AL_2 	7 // OSD_MIX_AL_2

// Maximum number of input plane for each mixer
#define PMX_MAX_INPORT_NS 5

// Background update delay vsync
#define PMX_UPDATE_DELAY 4

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define IGNORE_RET(X) \
    do \
    { \
        INT32 i4Ignore; \
        i4Ignore = (INT32)(X); \
        UNUSED(i4Ignore); \
    } while (0)


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/** initialize pmx
 */
EXTERN void PMX_Init(void);

/** get plane z-order
 *  @param  ucPlaneId plane id:{PMX_MAIN, PMX_PIP, PMX_OSD2, PMX_OSD3}
 *  @param  pucPlaneOrder order pointer:{PLA_MXR_Z_ORDER_1...PLA_MXR_Z_ORDER_4}
 *  @retval PMX_SET_OK success
 */
EXTERN UINT32 PMX_GetPlaneOrder(UCHAR ucPlaneId, UCHAR* pucPlaneOrder);

/** set plane z-order
 *  @param  ucPlaneId plane id:{PMX_MAIN, PMX_PIP, PMX_OSD2, PMX_OSD3}
 *  @param  ucPlaneOrder order:{PLA_MXR_Z_ORDER_1...PLA_MXR_Z_ORDER_4}
 *  @retval PMX_SET_OK success
 */
EXTERN UINT32 PMX_SetPlaneOrder(UCHAR ucPlaneId, UCHAR ucPlaneOrder);

/** set pmx background color
 *  @param  u4BgColor background color(0xrrggbb)
 *  @param  fgInVsync update it in next vsync or immediately
 *  @retval PMX_SET_OK success
 */
EXTERN UINT32 PMX_SetBg(UINT32 u4BgColor, BOOL fgInVsync);

#ifdef CC_FOR_POC_DEMO
/** set pmx blanking color
 *  @param  u4BlkColor blanking color(0xrrggbb)
 *  @retval PMX_SET_OK success
 */
EXTERN UINT32 PMX_SetBlk(UINT32 u4BgColor);
#endif

/** setup alpha value for video plane
 *  @param  ucPlaneId plane id:{PMX_MAIN, PMX_PIP}
 *  @param  ucValue alpha value(0~255)
 *  @retval PMX_SET_OK success
 */
EXTERN UINT32 PMX_SetAlpha(UCHAR ucPlaneId, UCHAR ucValue);

EXTERN void PMX_Set_ALPHA_MIX_GRP1_0(BOOL fgEnable);

EXTERN void PMX_Set_ALPHA_MIX_GRP12_0(BOOL fgEnable);


EXTERN UINT32 PMX_SetPlaneOrderArray(const UINT32* pu4PlaneOrder);
EXTERN UINT32 PMX_GetPlaneOrderArray(UINT32* pu4PlaneOrder);
EXTERN UINT32 PMX_SetMixAlpha(UCHAR ucType,const UINT32* pu4Alpha);

EXTERN void PMX_DIAG_DumpScpos(void);

EXTERN void PMX_OnOutputVSync(void);

EXTERN void PMX_ShowPattern(BOOL fgEnable);

EXTERN void PMX_SetFastBlank(BOOL fgEnable);

#ifdef DRV_SUPPORT_EXTMJC
EXTERN void PMX_SetYuvMode(BOOL fgYuvMode);
#endif

EXTERN void PMX_SetPlaneHide(UCHAR ucPlaneId,BOOL fgHide);

EXTERN void PMX_GetPlaneHide(UCHAR ucPlaneId,BOOL* pHide);

EXTERN void PMX_SetAllPlaneHide(BOOL fgHide);

#endif    // PMX_DRVIF_H

