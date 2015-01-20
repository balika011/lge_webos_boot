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
 * $Date: 2015/01/20 $
 * $RCSfile: drv_bnr_int.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _DRV_BNR_INT_H_
#define _DRV_BNR_INT_H_

#ifdef _C_MODEL_
#include "drv_common.h"
#endif

#define BNR_NEW_FW 1

#define SUPPORT_BNR_SIZE_PHASE_PREDICT

/*
 * definitions
 */
#define _NXNR_BNR_FW_MAX_WIDTH  1920
#define _NXNR_BNR_FW_MAX_HEIGHT 1088

#ifndef BNR_NEW_FW
#define _NXNR_BNR_FW_MAX_SIZE   1920 // max of width and height
#define _NXNR_BNR_FW_MIN_PERIOD     8
#define _NXNR_BNR_FW_MAX_PERIOD     33
#else   // BNR_NEW_FW
#define _NXNR_BNR_FW_MAX_DIMENSION   1920 // max of width and height
#define _NXNR_BNR_FW_MIN_PERIOD     3
#define _NXNR_BNR_FW_MAX_PERIOD     32
#define _NXNR_BNR_FW_MAX_MAJOR_PERIOD       128
#define _NXNR_BNR_FW_MAX_MINOR_PERIOD_COUNT 10
#define _NXNR_HBLK 0
#define _NXNR_VBLK 1
#define _NXNR_DEF_MNR_GAIN 0x16;
#endif  // BNR_NEW_FW

#define _NXNR_BNR_FW_STATE_STOP        -1
#define _NXNR_BNR_FW_STATE_INIT         0
#define _NXNR_BNR_FW_STATE_ACTIVE       1
#define _NXNR_BNR_FW_STATE_VSYNC        2
#define _NXNR_BNR_FW_STATE_DBG          3
#define _NXNR_BNR_FW_STATE_VSYNC_DBG    4

#define _NXNR_VBLK_GROUP_COUNT 2
#define _NXNR_VBLK_GROUP_MEMBER_COUNT 9

#define _NXNR_BNR_FW_LOG_STA              1
#define _NXNR_BNR_FW_LOG_VBLK_PEROID_HIST 2
#define _NXNR_BNR_FW_LOG_HBLK_PEROID_HIST 3

#ifdef SUPPORT_BNR_SIZE_PHASE_PREDICT
#define MAX_NUM_GRID (9)
#define BNR_MAX_SIZE (128)
#define BNR_NODE(n, r) (((n)<<4) | (r))
#define BNR_NODE_N(node) ((node)>>4)
#define BNR_NODE_R(node) ((node)&0xF)
#define BNR_SIZE(p, node) ((p)*BNR_NODE_N(node) + BNR_NODE_R(node))

#define NUM_NODE (28)

typedef struct
{
    UINT32 u4S;
    UINT32 u4P;
    UINT32 u4Start;
    UINT32 u4StartIdx;
    UINT32 u4LsInside;
    UINT32 u4LsOutside;
    UINT8  u1GridPos[MAX_NUM_GRID];
    UINT8  u1PSeq[MAX_NUM_GRID];
} BNR_DESC_T;

#endif

/*
 * forward declarations
 */
#ifdef _C_MODEL_
class CNXNR;
#endif

/*
 * functions
 */
#ifdef _C_MODEL_
void BNR_FW_Initialize ( CNXNR * _pNXNR, const char * _szPatternName );
void BNR_FW_ActiveProcess (void);
void BNR_FW_VSyncProcess (void);
BOOL BNR_FW_IsStop(void);
void BNR_FW_Stop(void);
void BNR_FW_Resume(void);
void BNR_FW_Dbg(void);
void BNR_FW_SetLogCfg(UINT8 u1Sel, UINT32 u4Cnt);
#else
EXTERN void BNR_FW_Initialize(UINT32 u4Width, UINT32 u4Height);
EXTERN void BNR_FW_ActiveProcess (void);
EXTERN void BNR_FW_VSyncProcess (void);
EXTERN BOOL BNR_FW_IsStop(void);
EXTERN void BNR_FW_Stop(void);
EXTERN void BNR_FW_Resume(void);
EXTERN void BNR_FW_Dbg(void);
EXTERN void BNR_FW_SetLogCfg(UINT8 u1Sel, UINT32 u4Cnt);
#endif
#endif //_DRV_BNR_INT_H_
